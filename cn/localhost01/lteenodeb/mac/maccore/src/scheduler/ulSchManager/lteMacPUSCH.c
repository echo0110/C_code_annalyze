/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacPUSCH.c,v 1.4.6.1.4.2.2.2.8.2 2010/11/09 13:32:56 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the main logic for the pusch. the 
 *                     functionality required for dl delay path 2 and dl delay 
 *                     path 3 is avaliable in this file.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacPUSCH.c,v $
 * Revision 1.4.6.1.4.2.2.2.8.2  2010/11/09 13:32:56  gur10121
 * processAdvancePUCCHAndPRACHReports second parameter changed now introduced the delay
 *
 * Revision 1.4.6.1.4.2.2.2.8.1  2010/11/08 06:06:31  gur10121
 * Semi static changes merging
 *
 * Revision 1.4.6.1.4.2.2.2.2.3  2010/10/29 15:09:39  gur10121
 *  tempDirectIndexingUEInfo_p->flag updated in collision array
 *
 * Revision 1.4.6.1.4.2.2.2.2.2  2010/10/27 17:13:59  gur10121
 * UT fixes + review comments incorporation
 *
 *
 * Revision 1.4.6.1.4.2.2.1.6.5  2010/10/21 09:29:55  gur10121
 * removing traces
 *
 * Revision 1.4.6.1.4.2.2.1.6.4  2010/10/21 07:58:19  gur10121
 * Semi static UT fixes
 *
 * Revision 1.4.6.1.4.2.2.1.6.3  2010/10/15 19:40:13  gur10121
 * Semi static bug fixed
 *
 * Revision 1.4.6.1.4.2.2.1.6.2  2010/10/15 18:53:19  gur10121
 * Semi staic Changesdone and checked in
 *
 * Revision 1.4.6.1.4.2.2.1.6.1  2010/10/14 08:22:38  gur10121
 * Semi Static header file changes
 *
 * Revision 1.4.6.1.4.2.2.2  2010/10/14 17:18:12  gur19413
 * DMRS FIXES
 *
 * Revision 1.4.6.1.4.2.2.1  2010/09/21 15:46:23  gur20491
 * FAPI changes
 *
 * Revision 1.4.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.77  2010/03/30 10:20:32  gur18550
 * Uplink Throughput Calc moved to De-Mux
 *
 * Revision 1.76  2009/11/20 11:15:19  gur18550
 * Net BSR Properly Filled
 *
 * Revision 1.75  2009/10/26 04:58:44  gur19413
 * optimsied the stats related changes
 *
 * Revision 1.74  2009/10/21 07:04:31  gur11912
 * removed warnings
 *
 * Revision 1.73  2009/10/21 06:01:20  gur19413
 * stats optimized
*
* Revision 1.72  2009/10/06 09:44:13  gur18550
* Function Optimised
*
* Revision 1.71  2009/10/06 08:14:17  gur11912
* modified for uplink throughput
*
* Revision 1.70  2009/10/05 08:05:46  gur11912
* optimization changes
*
* Revision 1.69  2009/09/17 10:46:31  gur18550
* Fuction Updated
*
* Revision 1.68  2009/09/17 09:51:26  gur18550
* Bug solved introduced due to Optimisation
*
* Revision 1.67  2009/09/16 14:47:15  gur18550
* Function Updated
*
* Revision 1.66  2009/09/16 09:51:01  gur11912
* corrected throughput
*
* Revision 1.65  2009/09/14 07:13:48  gur18550
* Windows Editing Removed
*
* Revision 1.64  2009/09/14 07:12:58  gur18550
* Updated PUSCH as per PDCCH
*
* Revision 1.63  2009/09/03 14:27:05  gur19413
* removed tempDirectIndexingUEInfo_p->flag = UNSET_FLAG from processULGrants
*
* Revision 1.62  2009/09/03 09:04:08  gur19413
* bug fixed
*
* Revision 1.61  2009/09/01 15:15:29  gur12140
* Solved Bug Related toUL and DL
*
* Revision 1.60  2009/08/20 06:45:21  gur19413
* added new parameter (value 0) in fillULStat func
*
* Revision 1.59  2009/08/04 11:52:52  gur18550
* Bug removed
*
* Revision 1.58  2009/08/04 09:10:32  gur18550
* Function Updated
*
* Revision 1.57  2009/08/03 06:12:59  gur18550
* Function Updated
*
* Revision 1.56  2009/07/29 10:37:25  gur18550
* HAndling of UL Nack
*
* Revision 1.55  2009/07/28 08:55:13  gur18550
* RB freed in non Harq Allocation
*
* Revision 1.54  2009/07/27 14:42:19  gur18550
* Function Updated
*
* Revision 1.53  2009/07/27 08:04:41  gur18550
* Function Updated
*
* Revision 1.52  2009/07/27 06:50:37  gur19413
* stats related changes
*
* Revision 1.51  2009/07/27 05:39:42  gur18550
* Function Updated
*
* Revision 1.50  2009/07/27 05:36:56  gur18550
* Function Updated
*
* Revision 1.49  2009/07/23 14:18:01  gur18550
* Updated Nack Received
*
* Revision 1.48  2009/07/22 04:51:17  gur18550
* Printf removed
*
* Revision 1.47  2009/07/21 13:15:50  gur12140
* Solved Bug
*
* Revision 1.46  2009/07/21 11:14:00  gur18550
* Function Updated./mac/maccore/src/scheduler/ulSchManager/lteMacPUSCH.c
*
* Revision 1.45  2009/07/21 07:37:37  gur18550
* Function Updated
*
* Revision 1.44  2009/07/20 08:42:10  gur18550
* Func Updated
*
* Revision 1.43  2009/07/20 08:20:09  gur18550
* Cqi Handling
*
* Revision 1.42  2009/07/20 07:04:02  gur18550
* Functionality of UL NACK revisited
*
* Revision 1.41  2009/07/18 11:59:24  gur11912
* used RESET_CCE for marking CCE as Free
*
* Revision 1.40  2009/07/17 15:22:45  gur18550
* Nack Handling modified
*
* Revision 1.39  2009/07/17 12:22:23  gur18550
* Function Updated
*
* Revision 1.38  2009/07/17 11:17:08  gur11912
* Moved RESET_CCE to lteMacUtilityAndSharedStructs.h
*
* Revision 1.37  2009/07/17 07:08:59  gur15697
* UT bugfix, invalid trance removed
*
* Revision 1.36  2009/07/15 12:42:39  gur18550
* STATS Updated
*
* Revision 1.35  2009/07/14 05:59:45  gur18550
* #ifdef  PDCCH_DELAY_THREE
*
* Revision 1.34  2009/07/14 05:44:45  gur18550
* Function Updated
*
* Revision 1.33  2009/07/13 14:00:42  gur18550
* Func Updated
*
* Revision 1.32  2009/07/13 13:33:42  gur18550
* Log Updated
*
* Revision 1.31  2009/07/13 10:11:33  gur18550
* ulStats
*
* Revision 1.30  2009/07/13 08:39:09  gur18550
* Log Updated
*
* Revision 1.29  2009/07/12 12:57:38  gur15697
* Ul Flow bug fix
*
* Revision 1.28  2009/07/11 14:32:20  gur18550
* update INVALID_HARQ_ID field in dciCCE Array
*
* Revision 1.27  2009/07/11 14:08:12  gur18550
* Added UNSET_FLAG
*
* Revision 1.26  2009/07/10 06:19:46  gur18550
* DELAY_THREE related changes
*
* Revision 1.25  2009/07/07 13:16:07  gur18550
* Function Updated
*
* Revision 1.24  2009/07/07 13:07:06  gur18550
* Function Updated
*
* Revision 1.23  2009/07/03 12:19:07  gur19413
* added resetRBsAtLocation(rbMapNum) function
*
* Revision 1.22  2009/07/01 12:34:43  gur19413
* Function Name changed processPUCCHAndPRACHReports
*
* Revision 1.21  2009/07/01 07:13:00  gur18550
* Changes due to ulCCEIndex
*
* Revision 1.20  2009/07/01 06:47:11  gur18550
* Function Updatred
*
* Revision 1.19  2009/06/29 07:24:37  gur19413
* added initPUSCH function
*
* Revision 1.18  2009/06/25 11:02:01  gur18569
* passing number of rbs and start rb index in presence of PHYSIM_TESTING macro
*
* Revision 1.17  2009/06/25 10:05:31  gur18550
* added ACK_OR_NACK
*
* Revision 1.16  2009/06/25 10:04:08  gur18550
* added ACK_OR_NACK
*
* Revision 1.15  2009/06/24 16:07:12  gur18550
* tpc = 0
*
* Revision 1.14  2009/06/24 15:15:35  gur18550
* Errors removed
*
* Revision 1.13  2009/06/24 14:06:17  gur18550
* Added isPdcchAllocated
*
* Revision 1.12  2009/06/23 15:56:42  gur18550
* Final Review Comments Inc
*
* Revision 1.10  2009/06/22 08:05:24  gur18550
* Final Comments Inc
*
* Revision 1.9  2009/06/15 04:28:15  gur18550
* Function Updated
*
* Revision 1.8  2009/06/10 15:15:00  gur18569
* removed linking and compilation errors
*
* Revision 1.7  2009/06/09 14:18:38  gur18550
* Function Updated
*
* Revision 1.6  2009/06/08 15:27:05  gur12140
* Removed Compilation Errors
*
* Revision 1.5  2009/06/04 07:43:49  gur18550
* Comments Incorporated
*
* Revision 1.4  2009/06/03 16:06:04  gur18550
* Comments Incorporated
*
* Revision 1.3  2009/05/12 10:24:36  gur18550
* Function Updated
*
* Revision 1.2  2009/05/05 16:16:36  gur18550
* Function Updated
*
* Revision 1.1  2009/04/30 05:16:26  gur18550
* INITIAL VERSION
*
*
*****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacPHICH.h"
#include "lteMacPUSCH.h"
#include "lteMacUtilityAndSharedStructs.h"
#include "lteMacTBSize.h"
#include "lteMacULUEContext.h"
#include "lteMacULQosScheduler.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacPuschPowerControl.h"
#include "lteMacULCSIManager.h"
#include "lteMacTransmitter.h" 
#include "lteMacULResourceAllocator.h"
/* ICIC changes start */
#include "lteMacRRMInterface.h"
#include "lteMacULResourceInfoMgr.h"
/* ICIC changes end */
/* UL_MU_MIMO_CHG Start*/
#include "lteMacPDCCHMgr.h"
/* UL_MU_MIMO_CHG end */
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#endif

#include "lteMacULSpsScheduler.h" 
#include "ltePerfStats.h"
/* +DYNAMIC_ICIC */
#include "lteMacRRMPeriodicMgr.h"
#include "lteMacCellConfMgr.h"

/* -DYNAMIC_ICIC */
/******************************************************************************
  Private Definitions
 *****************************************************************************/
/* ICIC changes start */
/* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
/*CA Changes start  */
BufferPool ulBufferPool_g[MAX_NUM_CELL];
/*CA Changes end  */
#endif
/* TDD Config 0 Changes End */
extern UInt32 resetBitmask[MAX_SUB_FRAME];
/* ICIC changes end */

/*UL_MU_MIMO_CHG Start*/
/*CA Changes start  */
BuddyPairInfo* buddyPairInfo_gp[MAX_NUM_CELL] = {PNULL};
#ifdef FDD_CONFIG
extern UInt16 aggregateMaxRBsRequired_g[MAX_NUM_CELL];
#endif
/*CA Changes end  */
/*UL_MU_MIMO_CHG End*/

/* SPR 1859 FIX begins*/
UInt8 ulRedundancyVersionArray_g[] = {0, 29, 30, 31}; 
extern UInt8 nextIrvSequenceArray_g[4];

#ifdef DL_UL_SPLIT
#ifdef CIRC_QUEUE_IMPL
/*CA Changes start  */
PUSCH_SIGNAL_CIRC_TYPE* puschSignalCircQueue_gp[MAX_NUM_CELL] = {PNULL};
extern sem_t waitForPUSCHSignalInd_g[MAX_NUM_CELL];
/*CA Changes end  */
#else
/* warning removed*/
/* +- SPR 17777 */
extern SInt32 recvMsgWithIccQ(SInt32 mqdes, const void *msg_ptr);
/* +- SPR 17777 */
#endif
#endif

/* SPR 1859 FIX ends*/  
/* CR changes start */
/* UL - MIMO Changes Start */
/*CA Changes start  */
/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
/* SPR 19679: TDD HARQ Multiplexing Changes Start */
/* In this bitmap, bit is set whenever PUSCH transmission is done with DCI0 */
UInt16 puschNewOrAdaptiveTxMap_g[MAX_UE_SUPPORTED] = {0};
extern UInt32 bitmask[MAX_SUB_FRAME];
/* SPR 19679: TDD HARQ Multiplexing Changes End */
/*Value of iPhich can be 1 or 0 in TDD Config 0*/
extern UInt8   phichGroupFromPrbDmrsMap_g[MAX_NUM_CELL][MAX_IPHICH_VALUE][MAX_NUM_RBS][MAX_DMRS_VALUE];
#elif FDD_CONFIG
extern UInt8   phichGroupFromPrbDmrsMap_g[MAX_NUM_CELL][MAX_NUM_RBS][MAX_DMRS_VALUE];
#endif
/*TDD Config 0 Changes End*/ 
extern UInt8   phichSequenceNoFromPrbDmrsMap_g[MAX_NUM_CELL][MAX_NUM_RBS][MAX_DMRS_VALUE];
#ifdef FDD_CONFIG
extern PHICHSequenceNoPerPhichGroup   phichSequenceNoPerPhichGroup_g[MAX_NUM_CELL][MAX_PHICH_CONTAINER][MAX_PHICH_GRP]; 
/*CA Changes end  */
#endif 

/*Ref from spec 36.211 Table 5.5.2.1.1-1: 
 * Mapping of Cyclic Shift Field in DCI format 0 to N2dmrs values.
 ===========================================================
 Cyclic Shift Field in DCI format 0 [3]  |   N2dmrs Value 
 ===========================================================
 000                      |         0
 001                      |         6
 010                      |         3
 011                      |         4
 100                      |         2
 101                      |         8
 110                      |         10
 111                      |         9
 ===========================================================        
 */

#define MAX_POSSIBLE_NDMRS_PAIRS 4
#define INVALID_N2DMRS_INDEX_VALUE MAX_POSSIBLE_NDMRS_PAIRS

UInt8 orthoNdmrsMap_g[MAX_POSSIBLE_NDMRS_PAIRS][2] = { {0,1}, {4,5}, {2,7}, {3,6} };
/* UL_MU_MIMO_CHG Changes End */

/*START_DYN_PDCCH_TDD_CHANGE*/
/*END_DYN_PDCCH_TDD_CHANGE*/
/*START : DYN_PDCCH*/
extern UInt8 currentCFIValue_g[MAX_NUM_CELL];
/*END   : DYN_PDCCH*/
/* CR changes end */
TpcSentMap tpcSentMap_g[MAX_NUM_CELL][MAX_TPC_CONTAINER_MAP_SIZE];

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
STATIC  MacRetType allocateFrequencySelectiveUplinkResourcesULMUMIMO(
        UInt8 freeRBs,
        PdcchULNewUE *pdcchULNewUE_p,    
        UInt8  inputRBs,
        UInt8  rbMapNum,
        ULUEContext* ulUEContext_p,
        SInt8 *bufferRBs_p,
        AllocatedResInfo *resourceReserved_p,
        UInt8 *allocatedMCS_p,
        UInt8 hiIndex,
        /* SPR 15909 fix start */
        tickType_t currentTTI,
        /* SPR 15909 fix end */
        TpcTriggeredToPowerCorrection  tpc,
        UInt32 *ueMaxPowerFlag_p,
        UInt32 *ueMinPowerFlag_p,
#ifdef FDD_CONFIG
        /* +- SPR 17777 */
        DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p
#elif TDD_CONFIG
        UeScheduledInfo *ueScheduledInfo_p
#endif
#ifdef FDD_CONFIG 
,UInt8 subFrameNumber
/* +- SPR 17777 */
,UInt8 ulDelay
#endif
,InternalCellIndex internalCellIndex
/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
,UInt8 ulSubFrameNum
,UInt8 phichSubFrameNum
#endif
/*TDD Config 0 Changes End*/ 
);

static MacRetType checkAvailableRBsToReturnLongestSetULMUMIMO(
        MCSToPrbMapInfo *mcsToPrbInfo_p,
        MCSToPrbMapInfo *buddyMcsToPrbInfo_p,
        UInt8 maxReqRBs,
        UInt8 *availableRBs,
        AvailableResInfo *rbMapNode_p,
        UInt8 *rbMap,
        UInt8 *maxAvailableRBsStartIdx,
        /* +- SPR 17777 */
#ifdef FDD_CONFIG
        UInt8 hiIndex,
#endif
        /* +- SPR 17777 */
        UInt8* availableN2dmrs_p,
        UInt8 minRBReq
        ,InternalCellIndex internalCellIndex
        /*TDD Config 0 Changes Start*/                    
#ifdef TDD_CONFIG
        ,UInt8 iPhich
        ,UInt8 phichSubFrameNum
#endif
/*TDD Config 0 Changes End*/
);

static MacRetType updateResourcesFromBuddyUE(
        UInt8 harqIndex,
#ifdef TDD_CONFIG
        UInt8 buddyHarqIndex,
#endif 
        ULUEContext* ulUEContext_p,
        UInt8 *allocatedMCS_p,
        AllocatedResInfo* resourceReserved_p,
        UInt16 buddyUeIndex
        ,UInt8 avgMCS
        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
        ,UInt8 iPhich
#endif                    
        ,InternalCellIndex internalCellIndex
        );
/*TDD Config 0 Changes End*/

STATIC  UInt8 checkAvailableULResources(
        UInt8 numOfResourcesReq,
        AvailableResInfo* availRes_p,
        UInt8* rbMap,
        /* +- SPR 17777 */
#ifdef FDD_CONFIG
        UInt8 hiIndex,
#endif
        /* +- SPR 17777 */
        UInt8* availableN2dmrs_p,
        UInt32* startRBIndex_p
        ,InternalCellIndex internalCellIndex
        /*TDD Config 0 Changes Start*/                    
#ifdef TDD_CONFIG
        ,UInt8 iPhich
        ,UInt8 phichSubFrameNum
#endif
        /*TDD Config 0 Changes End*/       
        );

STATIC  MacRetType checkOrthoN2dmrsPair(
        UInt32 riStart, 
        UInt8 hiIndex, 
        UInt8 *availableN2dmrs_p
        ,InternalCellIndex internalCellIndex
        /*TDD Config 0 Changes Start*/                    
#ifdef TDD_CONFIG
        ,UInt8 iPhich
#endif 
        /*TDD Config 0 Changes End*/
        );

STATIC MacRetType  validateNprbFromATBULMUMIMO(UInt8 inputRBs, 
        UInt8 inputMCS,
        ULUEContext *ulUEContext_p,
        /* SPR 15909 fix start */
        tickType_t currentTick
        /* SPR 15909 fix end */
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
#ifdef TDD_CONFIG                                        
        ,UInt8 ulSubFrameNumber
#endif                                        
        );

STATIC void createBuddyPairFromCandidate(PdcchULOutputInfo *pdcchULOutputInfo_p, 
        /* SPR 15909 fix start */
        tickType_t currentTick
        /* SPR 15909 fix end */ 
        /* +- SPR 17777 */
        ,InternalCellIndex internalCellIndex
        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /*TDD Config 0 Changes End*/ 
        );

STATIC  MacRetType allocateNonFrequencySelectiveUplinkResources(
        ULUEContext* ulUEContext_p,
        PdcchULNewUE *pdcchULNewUE_p,
#ifdef FDD_CONFIG
        DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p,
#elif TDD_CONFIG
        UeScheduledInfo *ueScheduledInfo_p,
#endif
        AllocatedResInfo* resourceReserved_p,
        UInt8 subFrameNumber,
        UInt32 freeRBs,
        SInt8 *bufferRBs_p, 
        TpcTriggeredToPowerCorrection tpc,
        UInt32 *ueMaxPowerFlag_p,
        UInt32 *ueMinPowerFlag_p,
        UInt8 *allocatedMCS_p,
        UInt8 hiIndex, 
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG 
,UInt8 ulDelay
#endif
,InternalCellIndex internalCellIndex
/*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
,UInt8 ulSubFrameNum
,UInt8 phichSubFrameNum
#endif
/*TDD Config 0 Changes End*/ 
);

/*Cyclomatic Complexity changes - Starts here */
STATIC  void  calculateBuddyPairRBs(
        PdcchULMUMIMOCandidateInfo *pdcchULMUMIMOCandidateInfo_p,
        /* +- SPR 17777 */
        PdcchULNewUEInfo           *pdcchULNewUEInfo_p,
        ULUEContext                *ulUEContextFirst_p,
#ifdef FDD_CONFIG
        PdcchULOutputInfo          *pdcchULOutputInfo_p,
        /* +- SPR 17777 */
        DirectIndexingUEInfo       *firstUEIndexingInfo_p,
#elif TDD_CONFIG
        UeScheduledInfo            *firstUEIndexingInfo_p,
#endif
        UInt16                     *firstUeIndex,
        /* SPR 15909 fix start */
        tickType_t                currentTick,
        /* SPR 15909 fix end */
        UInt32                     ueCount,
        UInt16                     minRbLoss    
        /* +- SPR 17777 */
        ,InternalCellIndex         internalCellIndex
        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
,UInt8                     ulSubFrameNum
#endif
/*TDD Config 0 Changes End*/ 
);

STATIC  MacRetType validateMinRbReq(
        UInt8                * minRbReq,
#ifdef FDD_CONFIG
        DirectIndexingUEInfo * tempDirectIndexingUEInfo_p,
#elif TDD_CONFIG
        UeScheduledInfo      * tempDirectIndexingUEInfo_p,
#endif
        UInt8                * allocatedMCS,
        ULUEContext          * ulUEContext_p,
        TpcTriggeredToPowerCorrection  tpc,
        UInt32               * ueMaxPowerFlag_p,
        UInt32               * ueMinPowerFlag_p,
        RbMapNode            * rbMapNode_p,
        AllocatedResInfo     * resourceReserved_p, 
        SInt32                 regionPowerMask,
        UInt32                 freeRBs,
        /* SPR 15909 fix start */
        tickType_t             currentTTI
#ifdef FDD_CONFIG
        ,UInt8                 ulDelay
        ,tickType_t                currentGlobalTick
/* SPR 15909 fix end */
#endif
,InternalCellIndex     internalCellIndex
/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
,UInt8                 phichSubFrameNum
,UInt8                 iPhich
,UInt8               * nDmrs_p
#endif
/*TDD Config 0 Changes End*/
);

/*SPR_9204_START*/
STATIC  MacRetType selectbuddyUeFSS(
        UInt16                 secondUeIndex,
#ifdef FDD_CONFIG
        DirectIndexingUEInfo   *secondUEIndexingInfo_p,       
#elif TDD_CONFIG
        UeScheduledInfo        *secondUEIndexingInfo_p,      
#endif
        PdcchULNewUEInfo       *pdcchULNewUEInfo_p,
        UInt16                 *firstUeIndex,
        ULUEContext            *ulUEContextFirst_p,
#ifdef FDD_CONFIG
        DirectIndexingUEInfo   *firstUEIndexingInfo_p,
#elif TDD_CONFIG
        UeScheduledInfo        *firstUEIndexingInfo_p,
#endif            
        UInt16                 *minRbLoss,
        UInt32                 *firstUEMinStrictReqRB,
        UInt32                 *secondUEMinStrictReqRB,
        UInt16                 *tempBuddyUeIndex,
        /* SPR 15909 fix start */
        tickType_t                 currentTick,
        /* SPR 15909 fix end */
        BuddyPairInfo          **buddyPairInfo_p
#ifdef TDD_CONFIG
        ,UInt8                 ulSubFrameNumber
#endif
        ,InternalCellIndex     internalCellIndex);
        STATIC  MacRetType selectbuddyUeNonFSS(
                UInt16                 secondUeIndex,
#ifdef FDD_CONFIG
                DirectIndexingUEInfo   *secondUEIndexingInfo_p,       
#elif TDD_CONFIG
                UeScheduledInfo        *secondUEIndexingInfo_p,      
#endif
                PdcchULNewUEInfo       *pdcchULNewUEInfo_p,
                UInt16                 *firstUeIndex,
                ULUEContext            *ulUEContextFirst_p,
#ifdef FDD_CONFIG
                DirectIndexingUEInfo   *firstUEIndexingInfo_p,
#elif TDD_CONFIG
                UeScheduledInfo        *firstUEIndexingInfo_p,
#endif
                UInt16                 *minRbLoss,
                UInt32                 *firstUEMinStrictReqRB,
                UInt32                 *secondUEMinStrictReqRB,
                UInt16                 *tempBuddyUeIndex,
                /* SPR 15909 fix start */
                tickType_t                 currentTick,
                /* SPR 15909 fix end */
                BuddyPairInfo          **buddyPairInfo_p
#ifdef TDD_CONFIG
                ,UInt8                 ulSubFrameNum
#endif
                ,InternalCellIndex      internalCellIndex);
                STATIC  void checkAvailabilityOfHigherMCS(
                        UInt8                  *mcsIdx,
                        UInt8                  startIndx,
                        UInt8                  requiredRBs,
                        ULUEContext*           ulUEContext_p,
                        ULUEContext*           buddyUlUEContext_p,
                        BuddyPairInfo          *buddyPairInfo_p,
                        /* SPR 15909 fix start */
                        tickType_t                 currentTTI
                        /* SPR 15909 fix end */
#ifdef TDD_CONFIG
                        ,UInt8                  ulSubFrameNum
#endif               
                        ,InternalCellIndex      internalCellIndex);
                /*SPR_9204_END*/
                /******************************************************************************
                 * Private Constants
                 *****************************************************************************/
#ifdef DL_UL_SPLIT
// Semaphore on which tPUSCH processing logic blocks so that only after CCE allocation is completed by
// DL thread it sends the signal to unblock UL thread. 
extern SInt32 puschSignalQueueIdUL_g[MAX_NUM_CELL];
#endif

/*ca changes start*/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
        UplinkContainer* ulContainerPool_gp[MAX_NUM_CELL] =   {PNULL};


/*Semi Static changes start */
ULSchUciRachPoolInfo* ulSchUciRachContainer_gp[MAX_NUM_CELL] =   {PNULL};
/*Semi Static changes End */
/*ca changes end*/

#ifdef OVERSHOOT_CORRECTION
/** CA-TDD Changes Start **/
extern OverLoadLevel macOverLoadLevel_g[MAX_NUM_CELL];
/** CA-TDD Changes Stop **/
#endif
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
        /* SS_S2 */
        /* + CQI_5.0 */
        /* 4x2 MIMO S */
        /* SPR 5599 changes start (ZIP ID 129107) */
        UInt8 calculateReportFormatBitWidthAperiodic
        /* SPR 5599 changes end (ZIP ID 129107) */
        /* +- SPR 17777 */
        (CQIMode cqiMode,ueExpectedReportType *feedBack_p, UInt8 transMode,
         UInt8 *riBits_p, UInt8 *bitWidthiForRIGT_1, UInt8 ueCategory,
         InternalCellIndex internalCellIndex);
/* 4x2 MIMO E */

UInt8 determineRBsForAperiodicCQIReports(ULUEContext *ulUEContext_p,
        UInt32 *minDataSize_p,
        /*CA Changes start */
        InternalCellIndex internalCellIndex);
/*CA Changes end */
/* - CQI_5.0 */
void storeCqiPmiRiDataAperiodic(UInt8 transMode, 
        ULUEContextInfo *ulUEContextInfo_p,
        ULSchUciRachPoolInfo *ulschUciRachInfo_p,
        /*SPR 5620 START*/
#ifdef TDD_CONFIG     
        UInt16 sysFrameNum,
#endif
        /*SPR 5620 END*/
        UInt8 subFrameNumber, UInt8 ulschUciInfoIndex,
        InternalCellIndex internalCellIndex);
/* SS_S2 */

/* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
 void processULSPSUeInPUSCH(
        UInt8 ulDelay,
        UInt8 containerTick,
        /* +- SPR 17777 */
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        /* CR changes start */
        UInt8 hiIndex,
        /* CR changes end */
        SInt8 *bufferRBs_p,
        UInt8 locType,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs);
#elif TDD_CONFIG
 void processULSPSUeInPUSCH(PdcchULOutputInfo* tempPdcchULOutput_p, 
        UInt8 containerTick,
        UInt32 currentGlobalTick,
        UInt8 subFrameNum,
        UInt8 rbMapNum,
        /*SPR 5620 START*/
        UInt16 ulGrantSysFrameNum,
        /*SPR 5620 END*/
        UInt16 ulGrantSubFrameNum,
        SInt8 *bufferRBs_p,
        UInt8 locType,
        InternalCellIndex internalCellIndex,
        ExecutionLegFunctionArg* execArgs);
#endif
/* - SPS_TDD_Changes */

UInt8 getSPSPhichGrpAndPhichSeq(UInt8 riStart,
        UInt8 *nPhichGrp_p,
        UInt8 *nPhichSeq_p,
        UInt8 *sfUsed_p,
        UInt8 *nDmrs_p,
        UInt8 hiIndex,
        /*CA Changes start */
        InternalCellIndex internalCellIndex);
/*CA Changes end */
/* + SPR 6864 */
void checkAndRestoreUCIPduType(
        ULUEContext * ulUEContext_p, 
#ifdef FDD_CONFIG
        DirectIndexingUEInfo *ueInfo_p,
#elif TDD_CONFIG
        UeScheduledInfo *ueInfo_p,
        /* + SPR_14994 */
        UInt16 systemFrame,
        /* - SPR_14994 */
#endif
        ULSchUciRachPoolInfo * ulschUciRachInfo_p,
        /* CSI_P2_PUSCH_CHANGES Start */
        UInt8                         subFrameNum,
        /* CSI_P2_PUSCH_CHANGES End */
        /*SPR 19504 fix start*/    
        UInt32 rbMapNum,
        InternalCellIndex internalCellIndex
        /*SPR 19504 fix end*/    
        );

void dropCSIReportWithHarq(
        ULSchUciInfo *ulSchUciInfo_p);
/* - SPR 6864 */

/*Cyclomatic Complexity changes - Starts here */
static  void checkQciPowerMaskAndUpdateRegionMask (
        ULUEContext * ulUEContext_p,
        SInt32      * regionPowerMask
        );
#ifdef FDD_CONFIG
static  void updatepdcchULNackUEInfoTempUEInfo(PdcchULOutputInfo* tempPdcchULOutput_p,
        UInt8 locType,
        DirectIndexingUEInfo *tempDirectIndexingUEInfo_p);
STATIC  void updateUlschUciRachInfo(DirectIndexingUEInfo *tempDirectIndexingUEInfo_p,
        ULUEContext *ulUEContext_p,
        ULSchUciRachPoolInfo *ulschUciRachInfo_p,
        UInt16 numberOfInfoPresent,
        UInt8 subFrameNum,
        ULUEContextInfo* ulUEContextInfo_p,
        InternalCellIndex internalCellIndex);
#endif

static  void  MacUlUpdateLocalMaxTBIfRequired(
        UInt8  mcsIdx,
        UInt8  *localMaxRBs_p,
        UInt8  *localMaxMCS_p,
        UInt8  availableRBs,
        UInt8  startIdx,
        UInt8  *localMaxTBSizeRBStartIdx_p,
        UInt32 *localTBSize_p,
        UInt32 *localMaxTBSize_p
        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
        ,UInt8  *localnDmrs_p
        ,UInt8  nDmrs
#endif
        );
/*TDD Config 0 Changes End*/ 
/*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
STATIC  MacRetType allocateResourcesForSecondPuschExecutionTddConfig0(
        ULHarqInfo* ulHarqInfoForPreviousUlSubframe_p,
        PdcchULNewUE *pdcchULNewUE_p,
        UeScheduledInfo *ueIndexingInfo_p,
        SInt8 bufferRBs,
        UInt8 *allocatedMCS_p,
        ULUEContext *ulUEContext_p,
        RbMapNode *rbMapNode_p,
        AllocatedResInfo *resourceReserved_p,
        UInt32 freeRBs,
        /* SPR 15909 fix start */
        tickType_t currentTTI,
        /* SPR 15909 fix end */
        UInt8 iPhich,
        UInt8 phichSubFrameNum,
        InternalCellIndex internalCellIndex
        );

STATIC MacRetType allocateFrequencySelectiveUplinkResourcesForTddConfig0(
        ULHarqInfo* ulHarqInfoForPreviousUlSubframe_p,
        PdcchULNewUE *pdcchULNewUE_p,
        /* SPR 15909 fix start */
        /* +- SPR 17777 */
#ifdef SSI_DEBUG_STATS
        tickType_t currentGlobalTick,
        TpcTriggeredToPowerCorrection tpc,
#endif
        /* +- SPR 17777 */
        /* SPR 15909 fix end */
        ULUEContext* ulUEContext_p,
        UeScheduledInfo *ueIndexingInfo_p,
        SInt8 *bufferRBs_p,
        UInt8 *allocatedMCS_p,
        UInt8 rbMapNum,
        AllocatedResInfo *resourceReserved_p,
        UInt32 freeRBs,
        UInt8 phichSubFrameNum,
        /* +- SPR 17777 */
        UInt8 iPhich,
        InternalCellIndex internalCellIndex
        );

        STATIC void populateUlIndexForTdd(
                DciCCEInfo* tempDciCCEInfo_p,
                UInt8 ulSubFrameNum,
                UInt8 subframe,
                InternalCellIndex internalCellIndex);

STATIC ULHarqInfo* getPrevHarqInfoForTddConfig0(UInt8 hiIndex,
        UInt16 sysFrameNum,
        ULUEContext* ulUEContext_p,
        InternalCellIndex internalCellIndex
        );

#ifdef MAC_AUT_TEST
/*
 * This enum is used for testing the common function for FSS and non FSS allocation
 * For TDD Config 0, second execution of PUSCH, to track the reason of failure
 * of the function.
 */
        MacRbAllocationFailureInfo rbAllocationFailureInfo = MAC_INVALID_INFO;

        /*
         * The following variables are used for testing function processPUSCHLogicForDelayPath2 called
         * by EL, for PUSCH execution. This enum is introduced to track the changes done in the
         * function.
         */
UInt8 secondExecutionPUSCH = FALSE;
UInt8 callProcessPUCCHAndPRACHReports = FALSE;
UInt8 updateIcicReportInfo = FALSE;
UInt8 resetPhichInfo = FALSE;
UInt8 deleteScheduleLCNodeInfo = FALSE;
#endif
#endif
/*TDD Config 0 Changes End*/ 
/*Cyclomatic Complexity changes - end here */
/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
extern RbMapNode* rbMapNodeArr_gp[MAX_NUM_CELL];
extern UInt8 rbgSize_g[MAX_NUM_CELL];
extern UInt8 numRBGConfigured_g[MAX_NUM_CELL];
UInt16 combiIndexTable[MAX_RBG_INDEX][ MAX_RBG_INDEX][ MAX_RBG_INDEX][ MAX_RBG_INDEX]={{{{0}}}};
#endif
/* SPR 21958 PUSCH RAT1 Support End */

/* SES-491 fix Start */
extern UInt8  minpowerOfTwoThreeFive_g[MAX_NUM_RBS + 1];
/* SES-491 fix End */

/********************************************************************************
 * Function Name : initPUSCH
 * Inputs        : numOfCells
 * Outputs       : None
 * Returns       : None
 * Description   : This function is used to init the ack nack queue of the container.
 *********************************************************************************/
        /*CA Changes start */
        void initPUSCH(UInt8 numOfCells)
{
    UInt8 i = 0, cellCount = 0;
    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Entry..... \n",__func__);

    createMemPool(sizeof(BuddyPairInfo) * MAX_UE_SUPPORTED, numOfCells);
    createMemPool(sizeof(UplinkContainer) * MAX_NUMBER_OF_CONTAINER, numOfCells);
    createMemPool(sizeof (HIPduArray) , numOfCells);
    for (cellCount = 0; cellCount < numOfCells; cellCount++)
    {
        GET_MEM_FROM_POOL(BuddyPairInfo, buddyPairInfo_gp[cellCount], sizeof(BuddyPairInfo) * MAX_UE_SUPPORTED, PNULL);
        GET_MEM_FROM_POOL(UplinkContainer, ulContainerPool_gp[cellCount], sizeof(UplinkContainer) * MAX_NUMBER_OF_CONTAINER, PNULL);
        /* SPR 11109 Fix start */
        memSet (buddyPairInfo_gp[cellCount], 0, (sizeof(BuddyPairInfo) * MAX_UE_SUPPORTED));
        memSet (ulContainerPool_gp[cellCount], 0, (sizeof(UplinkContainer) * MAX_NUMBER_OF_CONTAINER));
        /* SPR 11109 Fix end */

        for ( i = 0; i < MAX_NUMBER_OF_CONTAINER ; i++)
        {
            sQueueInit(&(ulContainerPool_gp[cellCount] + i)->ackQueue);
#ifndef DL_UL_SPLIT
            sQueueInit(&(ulContainerPool_gp[cellCount] + i)->nackQueue);
            sQueueInit(&(ulContainerPool_gp[cellCount] + i)->nackQueueForTempUE);
            sQueueInit(&(ulContainerPool_gp[cellCount] + i)->nackQueueForSps);
#endif
#ifdef FDD_CONFIG
            /* For TTIB_Code */
            INIT_NACK_QUEUE_FOR_TTIB_UE_Q(&(ulContainerPool_gp[cellCount] + i)->nackQueueForTtiBundledUe);
#endif       
        }

        GET_MEM_FROM_POOL(HIPduArray,hiPduContainer_gp[cellCount], sizeof(HIPduArray)*MAX_PDCCH_CONTAINER, PNULL);
        /* SPR 11109 Fix start */
        memSet(hiPduContainer_gp[cellCount],0,sizeof(HIPduArray)*MAX_PDCCH_CONTAINER);
        /* SPR 11109 Fix end */

    }
    for (cellCount = 0; cellCount < numOfCells; cellCount++)
    {

        /* Initializing the MAP used for ERROR Handling */
        for( i = 0; i < MAX_TPC_CONTAINER_MAP_SIZE; i++ )
        {
            tpcSentMap_g[cellCount][i].numUe = 0;
            tpcSentMap_g[cellCount][i].tick = 0;
            memSet( &(tpcSentMap_g[cellCount][i].tpcSentInfo), 0, ( sizeof(TpcSent) * 
                        MAX_TPC_IN_A_TTI) );
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Exit..... \n",__func__);
}
/*CA Changes end */

/*********************************************************************************
 * Function Name : updateDciCCEInfoArray
 * Inputs        : ulUEContext_p,
 *                 cceNumber,
 *                 tempHarqProcessId,
 *                 dciCCEInfo_p,
 *                 resourceReserved_p,
 *                 tpc,
 *                 currentGlobalTick,
 *                 hiIndex,
 *                 buddyUeIndex,
 *                 ulGrantRequestType,
 *                 internalCellIndex
 *                 ulSubFrameNum
 *                 scheduledCount
 * Outputs       : None
 * Returns       : MAC_SUCCESS /MAC_FAILURE
 * Description   : This function will populate the dciCCEInfo_g array. 
 *                                         
 ****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129107) */
void updateDciCCEInfoArray(
        /* SPR 5599 changes end (ZIP ID 129107) */
        ULUEContext* ulUEContext_p,
        UInt16 cceNumber,
        UInt8 tempHarqProcessId,
        DciCCEInfo* dciCCEInfo_p,
        AllocatedResInfo*  resourceReserved_p,
        /* +- SPR 17777 */
        UInt8 tpc
        /* SPR 15909 fix start */
        /* +- SPR 17777 */
        /* SPR 15909 fix end */
        /* CR changes start */
#ifdef FDD_CONFIG
        ,UInt8 hiIndex
        /* +- SPR 17777 */
        ,ULGrantRequestType ulGrantRequestType
#endif                 
        /* UL_MU_MIMO_CHG Start */
        , UInt16 buddyUeIndex
        /* UL_MU_MIMO_CHG End */
/* +- SPR 17777 */
/* CR changes end */
//SPR_16896_Changes
#ifdef TDD_CONFIG
,UInt8 spsAllocFlag
#endif
//SPR_16896_Changes
/*CA Changes start  */
,InternalCellIndex internalCellIndex
/*CA Changes end  */
/*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
/* +- SPR 17777 */
,tickType_t currentGlobalTick
,UInt8 ulSubFrameNum
,UInt8 scheduledCount
#endif
/*TDD Config 0 Changes End*/
)

{    
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    ULHarqInfo *ulHarq_p = &ulUEContext_p->ulresInfo[tempHarqProcessId];
#ifdef TDD_CONFIG    
    UInt8 subframe = currentGlobalTick % MAX_SUB_FRAME;
    UInt8 tempToggleVal = 0;
#endif
    UInt8 modScheme = QPSK;
    /* SPR 21958 PUSCH RAT1 Support Start */
    UInt8 numRbsAllocated = 0;
#ifdef FDD_CONFIG
    UInt8 rbStart = 0;
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
    tempDciCCEInfo_p = &dciCCEInfo_p[cceNumber]; 
    /*TDD Config 0 Changes Start*/
    /* We do not need to prepare DCI pdu second time when a common DCI is sent for UL
     * TX in +k and +7 
     * The below condition is valid for the following:*
     * Valid for TDD config 1-6
     * Valid for Tdd config 0 in case a DCi PDu corresponds to single U
     * frame
     * Valid for Tdd config 0 in +k offset only when a single DCi goes
     * for +k and +7
     * Valid for Adaptive Retx as well
     * The above validation is checked with the variable scheduled Count which
     * can be 2 only in case of Tdd config 0 and when same dci0 is valid for +7 as
     * in +k*/
#ifdef TDD_CONFIG
    if ((COMMON_DCI_COUNT > scheduledCount) ||
            (INVALID_COUNT == scheduledCount)) 
#endif
        /*TDD Config 0 Changes End*/
    {
        tempDciCCEInfo_p->dciPduInfo.ueIndex = ulUEContext_p->ueIndex;
        tempDciCCEInfo_p->dciPduInfo.dciFormat = DCI_FORMAT_0;
        /* SPR 21958 PUSCH RAT1 Support Start */ 
#ifdef FDD_CONFIG
        if(TRUE ==ulHarq_p->RAT1Allocation)
        {
            tempDciCCEInfo_p->dciPduInfo.rbCodingForPUSCHRAT1 = 
                resourceReserved_p->rbCodingForPUSCHRAT1;
            numRbsAllocated = ((ulHarq_p->endRbgIndexforSet1 - ulHarq_p->startRbgIndexforSet1)+
                    (ulHarq_p->endRbgIndexforSet2 - ulHarq_p->startRbgIndexforSet2))* 
                rbgSize_g[internalCellIndex];
            rbStart = ulHarq_p->startRbgIndexforSet1 * rbgSize_g[internalCellIndex];
        }
        else
        {
#endif
            tempDciCCEInfo_p->dciPduInfo.resrcAssgmntBitMap = ulHarq_p->riv;
            numRbsAllocated = ulHarq_p->riLength;
#ifdef FDD_CONFIG
            rbStart = ulHarq_p->riStart;
#endif
#ifdef FDD_CONFIG
        }
#endif
        /* SPR 21958 PUSCH RAT1 Support End */ 

        /* SPR 1859 FIX begins*/
        if(! ulHarq_p->currentIrv)
        { /*current Irv = 0*/
            tempDciCCEInfo_p->dciPduInfo.tb1Info.modulationType = ulHarq_p->mcsIndex;
            tempDciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer = 0;
        }
        else
        {
            if((1 == ulHarq_p->isCqiRequested) &&
                    /* SPR 21958 PUSCH RAT1 Support Start */
                    (numRbsAllocated <= 4) &&
                    /* SPR 21958 PUSCH RAT1 Support End */
                    (1 == ulHarq_p->currentIrv))
            {
                /*SPR 3600 Start*/
                /* Cyclomatic_complexity_changes_start */
                getUlModSchemeFromMcsIndex(ulHarq_p->mcsIndex, &modScheme,\
                        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                        cellParams_p->macEnable64QAM, ulUEContext_p->ueCategory);
                /* Cyclomatic_complexity_changes_end */
                /*SPR 3600 End*/
                /*SPR 3600 End*/
                if(QPSK != modScheme)
                {
                    ulHarq_p->currentIrv = nextIrvSequenceArray_g[ulHarq_p->currentIrv];
                }
            }

            tempDciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer = \
                                                                 ulHarq_p->currentIrv;
            tempDciCCEInfo_p->dciPduInfo.tb1Info.modulationType = \
                                                                  ulRedundancyVersionArray_g[ulHarq_p->currentIrv]; 
        }
        /* SPR 1859 FIX ends*/
        tempDciCCEInfo_p->dciPduInfo.tb1Info.txIndicator = ulHarq_p->newDataIndicator;
        //SPR_16896_Changes
#ifdef TDD_CONFIG
        if ( TRUE == spsAllocFlag ) 
        { 
            tempDciCCEInfo_p->dciPduInfo.rnti = ulUEContext_p->spsUlInfo.SpsCrnti;    
        }
        else
        {
            tempDciCCEInfo_p->dciPduInfo.rnti = ulUEContext_p->crnti;    
        }
#else

        if ( SPS_SCHEDULING == ulUEContext_p->schType ) 
        { 
            tempDciCCEInfo_p->dciPduInfo.rnti = ulUEContext_p->spsUlInfo.SpsCrnti;    
        }
        else
        {
            tempDciCCEInfo_p->dciPduInfo.rnti = ulUEContext_p->crnti;    
        }
#endif
        //SPR_16896_Changes
        /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG   
        /* + SPR_14039 */
        if (!IS_CA_ENABLED())
        {    
            tempDciCCEInfo_p->dciPduInfo.dai  = 
                getDai(ulUEContext_p->ueIndex,currentGlobalTick,subframe, internalCellIndex);
        } 
        /* - SPR_14039 */
#endif   
        /*TDD Config 0 Changes End*/
#ifdef FDD_CONFIG
        if( SPS_SCHEDULING == ulUEContext_p->schType && 
                UL_SPS_DEACTIVATION != ulGrantRequestType )
        {        
            tempDciCCEInfo_p->dciPduInfo.nDmrs = getSPSPhichGrpAndPhichSeq( 
                    /* SPR 21958 PUSCH RAT1 Support Start */
                    rbStart,
                    /* SPR 21958 PUSCH RAT1 Support End */
                    &ulHarq_p->nPhichGrp,
                    &ulHarq_p->nPhichSeq,
                    &ulHarq_p->sfUsed,
                    &ulHarq_p->nDmrs
                    /* CR changes start */
                    ,hiIndex
                    ,internalCellIndex
                    );
        }                                /* CR changes end */
        /* UL_MU_MIMO_CHG Start */
        else if( buddyUeIndex < MAX_UE_SUPPORTED)
        {
            tempDciCCEInfo_p->dciPduInfo.nDmrs = ulHarq_p->nDmrs;
        }
        /* UL_MU_MIMO_CHG End */
        else
        {
            /*TDD Config 0 Changes Start*/ 
            tempDciCCEInfo_p->dciPduInfo.nDmrs = 
                getPhichGrpAndPhichSeq( 
                        /* SPR 21958 PUSCH RAT1 Support Start */
                        rbStart,
                        /* SPR 21958 PUSCH RAT1 Support End */
                        &ulHarq_p->nPhichGrp,
                        &ulHarq_p->nPhichSeq,
                        &ulHarq_p->sfUsed,
                        &ulHarq_p->nDmrs
                        /* CR changes start */
                        ,hiIndex
                        /*CA Changes start  */
                        ,internalCellIndex
                        /*CA Changes end  */
                        );
        }                                /* CR changes end */
#elif TDD_CONFIG
        if( buddyUeIndex < MAX_UE_SUPPORTED)
        {
            tempDciCCEInfo_p->dciPduInfo.nDmrs = ulHarq_p->nDmrs;
        }
        /* ndmrs is calculate at the time of resource allocation*/
        /*+ Coverity fix 85802
          Code removed
          -Coverity fix 85802*/
#endif
        // + SPS_TDD_CHANGES
        /*Updateion of Ndi bit on the basis of harq is done for TDD here only*/
        tempDciCCEInfo_p->dciPduInfo.harqProcessId = tempHarqProcessId;   
        // - SPS_TDD_CHANGES
        /*TDD Config 0 Changes End*/       
        tempDciCCEInfo_p->dciPduInfo.tpc = tpc;
        /* SPR 21958 PUSCH RAT1 Support Start */ 
#ifdef FDD_CONFIG
        if(TRUE == ulHarq_p->RAT1Allocation)
        {
            tempDciCCEInfo_p->dciPduInfo.startRBIndex = INVALID_VALUE_FOR_UPLINK_RAT1; 
            tempDciCCEInfo_p->dciPduInfo.numOfRBs = INVALID_VALUE_FOR_UPLINK_RAT1;
        }
        else
        {
#endif
            tempDciCCEInfo_p->dciPduInfo.startRBIndex = resourceReserved_p->allocatedIndex;
            tempDciCCEInfo_p->dciPduInfo.numOfRBs = resourceReserved_p->allocatedLength;
#ifdef FDD_CONFIG
        }
#endif
        /* SPR 21958 PUSCH RAT1 Support End */
#ifdef TDD_CONFIG
        LOG_UT(MAC_UPDATE_DCI_INFO_ONE_ID,LOGDEBUG,MAC_UL_HARQ,\
                currentGlobalTick,\
                tempDciCCEInfo_p->dciPduInfo.ueIndex,\
                tempDciCCEInfo_p->dciPduInfo.dciFormat,\
                tempDciCCEInfo_p->dciPduInfo.resrcAssgmntBitMap,\
                tempDciCCEInfo_p->dciPduInfo.tb1Info.modulationType,\
                tempDciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer,\
                0.0,0.0,__func__,"UPDATE_DCI_INFO_ONE");

        LOG_UT(MAC_UPDATE_DCI_INFO_TWO_ID,LOGDEBUG,MAC_UL_HARQ,\
                currentGlobalTick,\
                tempDciCCEInfo_p->dciPduInfo.tb1Info.txIndicator,\
                tempDciCCEInfo_p->dciPduInfo.rnti,\
                tempDciCCEInfo_p->dciPduInfo.nDmrs,\
                tempDciCCEInfo_p->dciPduInfo.harqProcessId,\
                tempDciCCEInfo_p->dciPduInfo.tpc,\
                0.0,0.0,__func__,"UPDATE_DCI_INFO_TWO");
#endif
    }
    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    /* Calculate ulIndex for TDD and also find the value of NDI
     * depending whether it's a fresh Tx or ReTx*/
                populateUlIndexForTdd(tempDciCCEInfo_p,ulSubFrameNum,subframe,internalCellIndex);
            /* Toggle the NDI bit in case of new Tx*/
            if(NEW_TX  == ulHarq_p->newDataIndicator)
            {
                tempToggleVal = ulHarq_p->txIndicator;
                ulHarq_p->txIndicator = tempToggleVal ^ NEW_TX;
                tempDciCCEInfo_p->dciPduInfo.tb1Info.txIndicator = ulHarq_p->txIndicator;
            }
            else
            {
                tempDciCCEInfo_p->dciPduInfo.tb1Info.txIndicator = ulHarq_p->txIndicator;
            }
#endif
            /*TDD Config 0 Changes End*/
}

/*********************************************************************************
 * Function Name :    updateDciCCEInfoArrayForMsg3                             
 * Inputs        : tempUEContext_p,
 *                 cceNumber,
 *                 tempHarqProcessId,
 *                 dciCCEInfo_p,
 *                 resourceReserved_p,
 *                 currentGlobalTick,
 *                 ulDelay,
 *                 hiIndex,
 *                 internalCellIndex
 * Outputs       : None
 * Description   : This function will populate the dciCCEInfo_g array. 
 ****************************************************************************/
        /* SPR 5599 changes start (ZIP ID 129107) */
        void updateDciCCEInfoArrayForMsg3(
                /* SPR 5599 changes end (ZIP ID 129107) */
                TempCRNTICtx* tempUEContext_p,
                UInt16 cceNumber,
                /* +- SPR 17777 */
                DciCCEInfo* dciCCEInfo_p,
                AllocatedResInfo*  resourceReserved_p
                /* SPR 15909 fix start */
                /* +- SPR 17777 */
                /* SPR 15909 fix end */
                /* +- SPR 17777 */
#ifdef FDD_CONFIG
                ,UInt8 tempHarqProcessId
                ,UInt8 hiIndex
#endif
                ,InternalCellIndex internalCellIndex
                /* +- SPR 17777 */
                /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                /* +- SPR 17777 */
                ,tickType_t currentGlobalTick
        ,UInt8 ulDelay
        ,UInt8 ulSubFrameNum
#endif
        /* +- SPR 17777 */
        /*TDD Config 0 Changes End*/
        )
{    
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    ULHarqInfo *ulHarq_p = &tempUEContext_p->ulHarq;

    RNTIInfo      *rntiInfo_p        = PNULL;
    /*Subframe value will be current Tick + 8 as After 4 Ticks CRC_ACK /NACK is
     * rcvd  and after 4 Ticks of that HI_NACK/ACK will go  
     */
#ifdef TDD_CONFIG    
    UInt8 subframe = (currentGlobalTick + ulDelay ) % MAX_SUB_FRAME_FOR_PHICH;
    UInt8 tempToggleVal = 0;
#endif    
    UInt8 modScheme = QPSK;

    tempDciCCEInfo_p = &dciCCEInfo_p[cceNumber]; 
    /*CA Changes start  */
    rntiInfo_p = getUeIdxFromRNTIMap(tempUEContext_p->tcRNTI, internalCellIndex);
    /*CA Changes end  */
    /* COVERITY 10772 START */
    if(PNULL == rntiInfo_p)
    {
        return;
    }
    /* COVERITY 10772 END */
    /* +COVERITY 5.0 */
    {
        tempDciCCEInfo_p->dciPduInfo.ueIndex = rntiInfo_p->index;
    }
    /* +COVERITY 5.0 */
    tempDciCCEInfo_p->dciPduInfo.dciFormat = DCI_FORMAT_0_MSG3;
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    if(FALSE==ulHarq_p->RAT1Allocation)
    {
#endif
        tempDciCCEInfo_p->dciPduInfo.resrcAssgmntBitMap = ulHarq_p->riv;
#ifdef FDD_CONFIG
    }
    else
    {
        tempDciCCEInfo_p->dciPduInfo.rbCodingForPUSCHRAT1 = FREE_RESOURCE;
        /* MSG3 is not transmistted/retrasmitted with RAT1 policy , So this 
         * field has been marked with default value which will only be filled 
         * in tranmision with RAT 1 policy */	
    }
#endif
    /* SPR 21958 PUSCH RAT1 Support End */

    /* SPR 1859 FIX begins*/
    if(! ulHarq_p->currentIrv)
    { /*current Irv = 0*/
        tempDciCCEInfo_p->dciPduInfo.tb1Info.modulationType = ulHarq_p->mcsIndex;
        tempDciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer = 0;
    }
    else
    {
        if((1 == ulHarq_p->isCqiRequested) &&
                (ulHarq_p->riLength <= 4) &&
                (1 == ulHarq_p->currentIrv))
        {
            /* Cyclomatic_complexity_changes_start */
            getUlModSchemeFromMcsIndex(ulHarq_p->mcsIndex, &modScheme, \
                    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                    macEnable64QAM,dciCCEInfo_p->dciPduInfo.ueCategory);
            /* Cyclomatic_complexity_changes_start */
            if(QPSK != modScheme)
            {
                ulHarq_p->currentIrv = nextIrvSequenceArray_g[ulHarq_p->currentIrv];
            }
        }

        tempDciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer = \
                                                             ulHarq_p->currentIrv;
        tempDciCCEInfo_p->dciPduInfo.tb1Info.modulationType = \
                                                              ulRedundancyVersionArray_g[ulHarq_p->currentIrv]; 
    }
    /* SPR 1859 FIX ends*/
#ifdef FDD_CONFIG
    tempDciCCEInfo_p->dciPduInfo.tb1Info.txIndicator = ulHarq_p->newDataIndicator;
#endif
    tempDciCCEInfo_p->dciPduInfo.rnti = tempUEContext_p->tcRNTI;    
#ifdef TDD_CONFIG    
    /* UL DL TDD compilation changes */
    tempDciCCEInfo_p->dciPduInfo.dai  = INVALID_DAI_VALUE;
    /* UL DL TDD compilation changes */
#endif    

#ifdef FDD_CONFIG
    tempDciCCEInfo_p->dciPduInfo.nDmrs = 
        getPhichGrpAndPhichSeq( ulHarq_p->riStart,
                &ulHarq_p->nPhichGrp,
                &ulHarq_p->nPhichSeq,
                &ulHarq_p->sfUsed,
                &ulHarq_p->nDmrs
#ifdef FDD_CONFIG
                ,hiIndex
#elif TDD_CONFIG
                ,subframe
#endif
                /*CA Changes start  */
                ,internalCellIndex
                /*CA Changes end  */
                );
#elif TDD_CONFIG
    /*Ndmrs is calculated at the time of PUSCH allocation*/
    tempDciCCEInfo_p->dciPduInfo.nDmrs = ulHarq_p->nDmrs;
#endif

    /*TDD Config 0 Changes Start*/
#ifdef FDD_CONFIG
    /* START SPR 1350 CHANGES */
    tempDciCCEInfo_p->dciPduInfo.harqProcessId = tempHarqProcessId;   
#endif
    /*TDD Config 0 Changes End*/
    /* END SPR 1350 CHANGES */
    tempDciCCEInfo_p->dciPduInfo.tpc = 1; 
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    if(FALSE==ulHarq_p->RAT1Allocation)
    {
#endif
        tempDciCCEInfo_p->dciPduInfo.startRBIndex = resourceReserved_p->allocatedIndex;
        tempDciCCEInfo_p->dciPduInfo.numOfRBs = resourceReserved_p->allocatedLength;
#ifdef FDD_CONFIG
    }
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
#ifdef TDD_CONFIG     
    LOG_UT(MAC_UPDATE_DCI_INFO_ONE_ID,LOGDEBUG,MAC_UL_HARQ,\
            currentGlobalTick,\
            tempDciCCEInfo_p->dciPduInfo.ueIndex,\
            tempDciCCEInfo_p->dciPduInfo.dciFormat,\
            tempDciCCEInfo_p->dciPduInfo.resrcAssgmntBitMap,\
            tempDciCCEInfo_p->dciPduInfo.tb1Info.modulationType,\
            tempDciCCEInfo_p->dciPduInfo.tb1Info.redundancyVer,\
            0.0,0.0,__func__,"UPDATE_DCI_INFO_ONE");

    LOG_UT(MAC_UPDATE_DCI_INFO_TWO_ID,LOGDEBUG,MAC_UL_HARQ,\
            currentGlobalTick,\
            tempDciCCEInfo_p->dciPduInfo.tb1Info.txIndicator,\
            tempDciCCEInfo_p->dciPduInfo.rnti,\
            tempDciCCEInfo_p->dciPduInfo.nDmrs,\
            tempDciCCEInfo_p->dciPduInfo.harqProcessId,\
            tempDciCCEInfo_p->dciPduInfo.tpc,\
            0.0,0.0,__func__,"UPDATE_DCI_INFO_TWO");
#endif
    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    /* Determine the ulIndex and toggle Ndi in case of New Tx*/
    populateUlIndexForTdd(tempDciCCEInfo_p,ulSubFrameNum,subframe,internalCellIndex);
    /* Toggle the NDI bit in case of new Tx*/
    if(NEW_TX  == ulHarq_p->newDataIndicator)
    {
        tempToggleVal = ulHarq_p->txIndicator;
        ulHarq_p->txIndicator = tempToggleVal ^ NEW_TX;
        tempDciCCEInfo_p->dciPduInfo.tb1Info.txIndicator = ulHarq_p->txIndicator;
    }
    else
    {
        tempDciCCEInfo_p->dciPduInfo.tb1Info.txIndicator = ulHarq_p->txIndicator;
    }
#endif
    /*TDD Config 0 Changes End*/
}

/* +CLPC_CHG_LJA */
/******************************************************************************
 * Function Name : fetchDeltaTFandLogMFactor
 * Inputs        : deltaMcsEnabled - Tells whether deltaMCS is enabled or not ,
 *                 mcsIndex - Configured MCS index for the UL transmission,
 *                 riLength - Number of RBs configured for this transmission
 * Outputs       : None
 * Returns       : Delta TF + 10LogM factor
 * Description   : This function takes the input as whether delta MCS is enabled 
 *                 or not, the configured MCS index and the number of RBs and returns
 *                 Delta TF and 10LogM factor.
 *****************************************************************************/
    /* SPR 5599 changes start (ZIP ID 129107) */
    static SInt8 fetchDeltaTFandLogMFactor (UInt8 deltaMcsEnabled, 
            /* SPR 5599 changes end (ZIP ID 129107) */
            UInt8 mcsIndex, 
            UInt8 riLength)
{
    SInt8 deltaTF = 0;

    if(deltaMcsEnabled)
    {
        deltaTF = mcsToDeltaTF_g[mcsIndex];
    }
    return deltaTF + tenLogMFactor_g[riLength - 1];
}
/* -CLPC_CHG_LJA */
/* CR changes start */
/*CA Changes start  */
#ifdef FDD_CONFIG
/********************************************************************************
 * Function Name : resetPhichSeqPerIndex
 * Inputs        : index,
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : None
 * Description   : This function initializes the bitMask and count in 
 *                 phichSequenceNoPerPhichGroup_g for all groups at a row index.
 ********************************************************************************/
            static void resetPhichSeqPerIndex(UInt8 index,
                    InternalCellIndex internalCellIndex)
{
    UInt8 nPhichGrp = 0;
    while(nPhichGrp < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->numPHICHGroup)
    {  
        phichSequenceNoPerPhichGroup_g[internalCellIndex][index][nPhichGrp].count   = 0; 
        phichSequenceNoPerPhichGroup_g[internalCellIndex][index][nPhichGrp].bitMask = 0xFF;
        nPhichGrp++;
    }
}
#else
/********************************************************************************
 * Function Name : resetPhichSeqPerIndex
 * Inputs        : index
 * Outputs       : None
 * Returns       : None
 * Description   : This function initializes the bitMask and count in 
 *                 phichSequenceNoPerPhichGroup_g for all groups at a row index.
 ********************************************************************************/
static void resetPhichSeqPerIndex(UInt8 index,InternalCellIndex internalCellIndex)
{    
    UInt8 nPhichGrp = 0;
    /* SPR 10732 Fix Start */
    while(nPhichGrp < 2* cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->numPHICHGroup)
        /* SPR 10732 Fix End */
    {    
        phichSequenceNoPerPhichGroup_g[internalCellIndex][index][nPhichGrp] = 0xFF;
        nPhichGrp++;
    }    
}
#endif
/*CA Changes end  */
/* CR changes end */
/* ULA_CHG*/
/******************************************************************************
 * Function Name : allocateFrequencySelectiveUplinkResources
 * Inputs        : freeRBs - Number of free RBs that can be allocated,
 *                 pdcchULNewUE_p - Pointer to PdcchULNewUE,
 *                 inputRBs - Number of RBs allocated by UL Strategy,
 *                 isStrictAllocated - If Strict allocation needs to be 
 *                                     followed,
 *                 rbMapNum - Index to rbMapNodeArr_g
 *                 ulUEContext_p - Pointer to UE's UL context
 *                 bufferRBs - Pointer to buffer RBs which specify the 
 *                             number by which PUSCH UL resource allocation differs
 *                             from the resources allocated by UL Strategy,
 *                 resourceReserved_p - Pointer to the UE UL 
 *                                      resources to be filled in this function,
 *                 allocatedMCS_p - Pointer to the UE UL MCS to be 
 *                                  allocated in this function,
 *                 currentTTI,
 *                 tpc,
 *                 ueMaxPowerFlag_p,
 *                 ueMinPowerFlag_p,
 *                 regionPowerMask,
 *                 harqIndex,
 *                 ulGrantType,
 *                 ulDelay,
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : MAC_SUCCESS/MAC_FAILURE
 * Description   : This function implements the frequency selective algorithm. 
 *                 For new transmission it shall try to allocate resources in 
 *                 the priority order starting from the best PRBs 
 *                 (on which higher SNR is reported) with the intent to 
 *                 increase the system throughput
 *                 It shall take the channel quality, MCS to PRB mapping of the
 *                  UE from CSI Manager.
 *
 *                 - FSS algo shall start searching for the required PRBs 
 *                 (sufficient to serve the data size) which are available from
 *                 highest MCS group to lower MCS group
 *
 *                - FSS Algorithm can change (reduce) the Number of RBs 
 *                (= sufficient RBs to serve the data size) allocated by 
 *                Strategy in case the RBs allocated on higher MCS than Average MCS
 * 
 *                - In case of number of RBs are reduced, it shall maintain 
 *                Buffer RBs so that it can be allocated to other users, if required
 *
 *	               - FSS Algorithm shall try to increase the Number of RBs 
 *                (= sufficient RBs to serve the data size) in case the RBs 
 *                allocated on lower MCS than Average MCS and Transmission type 
 *                is set to ‘strictAllocated’. In case desired number of RBs is 
 *                not available to serve the min data size and transmission type 
 *                is set to ‘StrictAllocated’, PUSCH shall not allocate the 
 *                resources to UE, such a UE will be pushed to PUSCH Failure Queue 
 *                (as in existing implementation) and prioritized by UL Strategy.
 *
 *                - FSS Algorithm shall change (increase) the Number of RBs 
 *                (= sufficient RBs to serve the data size) in case the RBs 
 *                allocated on lower MCS than Average MCS and Transmission type is 
 *                not set to ‘strictAllocated’ and Buffer RBs are available
 *
 *                - In case FSS is not able to find the preferred resources 
 *                (on which SINR is reported)  available, FSS will try to allocate 
 *                any resources available and request CSI to provide  the best 
 *                possible MCS of UE on allocated frequency
 *****************************************************************************/
        /* SPR 2446 Fix Start*/
        STATIC MacRetType allocateFrequencySelectiveUplinkResources(
                UInt8 freeRBs,
                PdcchULNewUE *pdcchULNewUE_p,    
                UInt8  inputRBs,
                UInt8  isStrictAllocated,
                UInt8  rbMapNum,
                ULUEContext* ulUEContext_p,
                SInt8 *bufferRBs_p,
                AllocatedResInfo *resourceReserved_p,
                UInt8 *allocatedMCS_p,
                /* SPR 15909 fix start */
                tickType_t currentTTI,
                /* SPR 15909 fix end */
                /* Time Averaging changes start */
                TpcTriggeredToPowerCorrection  tpc,
                UInt32 *ueMaxPowerFlag_p,
                UInt32 *ueMinPowerFlag_p

                /* Time Averaging changes end */
#ifdef SSI_DEBUG_STATS
                ,UInt8 harqIndex,
                ULGrantRequestType ulGrantType
#endif
                /* +DYNAMIC_ICIC */
                ,SInt32 regionPowerMask
                /* -DYNAMIC_ICIC */
#ifdef FDD_CONFIG
                ,UInt8 ulDelay
#endif
                /*CA Changes start */
                ,InternalCellIndex internalCellIndex
                /*CA Changes end */
                /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                ,UInt8 *nDmrs_p
                ,UInt8 phichSubFrameNum
                ,UInt8 iPhich
#endif
                /*TDD Config 0 Changes End*/
                )
                /* SPR 2446 Fix End*/
{
    freeRBs =  getUplinkMinRBsInPowerOfTwoThreeFive(freeRBs);
    UInt32 minDataSize = pdcchULNewUE_p->minDataSize;
    UInt32 maxDataSize = pdcchULNewUE_p->maxDataSize;
    UInt32 avgMCS = pdcchULNewUE_p->avgMCS;
    UInt8 mcsIdx = 0;
    UInt8 minRBs = 0; //PRBs corresponding to minDataSize for an MCS
    UInt8 maxRBs = 0; //PRBs corresponding to maxDataSize for an MCS  
    UInt8 availableRBs = 0;
    UInt8 startIdx = 0;

    UInt32 localTBSize = 0;
    UInt32 localMaxTBSize = 0;//Used when strictAllocated in not set.
    UInt8 localMaxRBs = 0;
    UInt8 localMaxMCS = 0;
    UInt8 localMaxTBSizeRBStartIdx = 0;
#ifdef TDD_CONFIG
    UInt8 localNDmrs = 0;
#endif

    /* ATB code FSS */
    UInt8 minRBsPrev = 0;
    UInt8 minRBsNew = 0;
    UInt8 mcsIdxPrev = 0;

    CSIContext *csiContext_p = &ulUEContext_p->csiContext;
    MCSToPrbMapInfo *mcsToPrbMap_p = csiContext_p->mcsToPrbMap_p;
    /* ICIC changes start */
    /*CA Changes start  */
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum; 
    /*CA Changes end  */
    /* ICIC changes end */
    /* TTI Bundling changes starts */
    MacRetType retValue = MAC_FAILURE;
    /* TTI Bundling changes Ends */

#ifdef FDD_CONFIG
    /* TTIB_Code Start */
    mcsIdx = csiContext_p->highestReportedMCS;
    UInt8 lowermcsIdx = csiContext_p->lowestReportedMCS;

    if(ulUEContext_p->ttiBundlingEnabled)
    {
        if(mcsIdx > MAX_TTI_BUNDLE_MCS)
        {
            mcsIdx = MAX_TTI_BUNDLE_MCS;
        }

        if(MAX_TTI_BUNDLE_MCS < lowermcsIdx  )
        {
            lowermcsIdx  = MAX_TTI_BUNDLE_MCS;
        }

        LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH,
                "[%s] [%d] mcsIndex  %d isUE TTIBundled %d   \n",
                __func__,getCurrentTick(),mcsIdx,ulUEContext_p->ttiBundlingEnabled);
    }
    /* TTIB_Code End */
#endif
    if(isStrictAllocated)
    {
        /* We try to give the best MCS to the UE. So loop from highest to 
         * lowest reported MCS.
         */ 
#ifdef FDD_CONFIG
        for(;
                (mcsIdx >= lowermcsIdx ) && \
                (mcsIdx != INVALID_MCS_INDEX);
                mcsIdx = mcsGroupingTable_g[mcsIdx].previousMCS )
#else
            for(mcsIdx = csiContext_p->highestReportedMCS;
                    (mcsIdx >= csiContext_p->lowestReportedMCS) && \
                    (mcsIdx != INVALID_MCS_INDEX);
                    mcsIdx = mcsGroupingTable_g[mcsIdx].previousMCS )
#endif
            {
                if(*allocatedMCS_p != mcsIdx)
                {
                    /* SPR 5999 changes start */
                    /*CA Changes start  */
                    minRBs = DETERMINE_RBs_UL(mcsIdx, minDataSize, internalCellIndex);
                    /*CA Changes end  */
                    /* SPR 5999 changes end */
                }
                else
                {
                    minRBs = inputRBs;
                }
                minRBs = getUplinkMaxRBsInPowerOfTwoThreeFive(minRBs);

#ifdef FDD_CONFIG
                if(ulUEContext_p->ttiBundlingEnabled)
                {
                    if( maxRBs > MAX_TTI_BUNDLE_RB)
                    {
                        maxRBs = MAX_TTI_BUNDLE_RB ;
                    }
                }
#endif

                if(freeRBs < minRBs)
                {
                    /* Number of RBs left to be allocated in PUSCH resource map 
                     * are less that that required by this UE on MCS=mcsIdx.
                     * No need to look further because a lower MCS will require 
                     * still more RBs to serve the minDataSize.
                     * Do non-FSS scheduling.
                     */
#ifdef LOG_PRINT_ENABLED                 
                    LOG_MAC_MSG(MAC_ULSCH_FREQ_SEL_SCH_ID,LOGDEBUG,MAC_PUSCH,\
                            currentTTI,\
                            freeRBs,\
                            minRBs,\
                            mcsIdx,\
                            mcsToPrbMap_p[mcsIdx].maxContiguous,\
                            0,\
                            0.0,0.0,__func__,"freeRBs < minRBs");                 
#endif                 
                    break;
                }
                else if (mcsToPrbMap_p[mcsIdx].maxContiguous < minRBs)
                {
                    /* Not enough PRBs are reported on this MCS to serve the
                     * minDataSize. Need to look at next lower MCS.
                     */
#ifdef LOG_PRINT_ENABLED                 
                    LOG_MAC_MSG(MAC_ULSCH_FREQ_SEL_SCH_ID,LOGDEBUG,MAC_PUSCH,\
                            currentTTI,\
                            freeRBs,\
                            minRBs,\
                            mcsIdx,\
                            mcsToPrbMap_p[mcsIdx].maxContiguous,\
                            0,\
                            0.0,0.0,__func__,"mcsToPrbMap_p[mcsIdx].maxContiguous < minRBs");
#endif                  
                    continue;
                }
                else
                {
                    /* ATB Code with FSS */
                    minRBsPrev = minRBs;
                    mcsIdxPrev = mcsIdx;
                    minRBs = validateATB( minRBs,&mcsIdx,ulUEContext_p,                        
                            tpc.powerCorrectionVal,
                            ueMaxPowerFlag_p,ueMinPowerFlag_p,
                            regionPowerMask,NEW_TX,internalCellIndex);

                    if ( mcsIdx == mcsIdxPrev )
                    {
                        if ( minRBsPrev > minRBs )
                        {
                            continue;
                        }
                    }
                    else
                    {
                        /* SPR 5999 changes start */
                        /*CA Changes start  */
                        minRBsNew = DETERMINE_RBs_UL(mcsIdx, minDataSize, internalCellIndex);
                        /*CA Changes end  */
                        /* SPR 5999 changes end */
                        minRBsNew = getUplinkMaxRBsInPowerOfTwoThreeFive(minRBsNew);
                        if ( minRBsNew > minRBs )
                        {
                            continue;
                        }
                        minRBs = minRBsNew;
                    }


                    if(avgMCS <= mcsIdx)
                    {
                        /* Try to serve maxDataSize*/
                        /* SPR 5999 changes start */
                        /*CA Changes start  */
                        maxRBs = DETERMINE_RBs_UL (mcsIdx, maxDataSize, internalCellIndex); 
                        /*CA Changes end  */
                        /* SPR 5999 changes end */
                        maxRBs = getUplinkMaxRBsInPowerOfTwoThreeFive(maxRBs);

                        if(inputRBs < maxRBs)
                        {
                            /* When trying to serve more data than that scehduled 
                             * by strategy, limit number of RBs as those given by 
                             * UL strategy.
                             */
                            maxRBs = inputRBs;
                        }
                        /* Need to call validateATB(maxRBs..) even though 
                         * inputRBs from strategy are already checked against ATB 
                         * because mcsIdx  above maybe different from avgMCS used 
                         * by strategy to check with ATB.
                         */
                        maxRBs = validateATB(maxRBs, \
                                /* Time Averaging changes start */
                                &mcsIdx, \
                                ulUEContext_p, \
                                tpc.powerCorrectionVal, \
                                ueMaxPowerFlag_p,ueMinPowerFlag_p,
                                regionPowerMask,NEW_TX,internalCellIndex);
                        /* Time Averaging changes end */
#ifdef FDD_CONFIG
                        if((ulUEContext_p->ttiBundlingEnabled) &&
                                ( maxRBs > MAX_TTI_BUNDLE_RB))
                        {
                            maxRBs = MAX_TTI_BUNDLE_RB ;
                        }
#endif                    
                        retValue = getAvailableRbsForLongestSet( &mcsToPrbMap_p[mcsIdx],
                                maxRBs,
                                &availableRBs,
                                rbMapNode_p,
#ifdef FDD_CONFIG
                                ulDelay,
                                /* +- SPR 17777 */
                                currentTTI,
#endif                                    
                                /* +- SPR 17777 */
                                ulUEContext_p,
                                &startIdx,
                                internalCellIndex
                                /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                ,nDmrs_p
                                ,phichSubFrameNum
                                ,iPhich
#endif
                                );

                        if((availableRBs >= minRBs) &&
                                (MAC_SUCCESS == retValue))
                        {
                            /*TDD Config 0 Changes End*/
                            /* ICIC changes start */
                            /*Able to serve atleast minDataSize*/
                            /*Coverity Id 32413 Fix Start*/
                            MAC_UL_RESERVE_RESOURCES(startIdx, availableRBs, \
                                    resourceReserved_p,\
                                    bufferRBs_p, inputRBs ,\
                                    ulUEContext_p->userLocationType,\
                                    rbMapNode_p,\
                                    rbMapNode_p->rbMap,
                                    internalCellIndex);

                            *allocatedMCS_p = mcsIdx;
                            /*Coverity Id 32413 Fix End*/
#ifdef FDD_CONFIG
                            /** TTIB_Code Start */
                            reserveAlreadyAvailableResForThreeTicks(startIdx, availableRBs,
                                    resourceReserved_p,currentTTI,
                                    ulDelay,ulUEContext_p,
                                    ulUEContext_p->userLocationType,
                                    internalCellIndex);
                            /** TTIB_Code End */
#endif
                            /* CA Stats Changes Start */
                            LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                                    ulUEContext_p->ueIndex,
                                    currentTTI,
                                    NEW_TX,
                                    harqIndex,
                                    availableRBs,
                                    startIdx,
                                    ulGrantType,
                                    determineUlTBSize(mcsIdx,availableRBs),
                                    TRUE,
                                    mcsIdx,
                                    0,
                                    /* Time Averaging changes start */
                                    tpc.tpcCommand
                                    /* Time Averaging changes end */
                                    /* UL_MU_MIMO_CHG START */
                                    ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                                    INVALID_UE_INDEX,
                                    ulUEContext_p->userLocationType,
                                    MAX_DMRS_VALUE,
                                    ulUEContext_p->modulationScheme,
                                    /* UL_MU_MIMO_CHG END */
                                    internalCellIndex
                                        );
                            /* CA Stats Changes End */
                            return retValue;
                        }
                        else
                        {
                            /* Not able to serve minDataSize on this mcsIdx. 
                             * Look at next lower MCS.
                             */
                            continue;
                        }                                            
                    }//avgMCS <= mcsIdx
                    else
                    {
                        retValue = getAvailableRbsForLongestSet(
                                &mcsToPrbMap_p[mcsIdx],
                                minRBs,
                                &availableRBs,
                                rbMapNode_p,
#ifdef FDD_CONFIG
                                ulDelay,
                                /* +- SPR 17777 */
                                currentTTI,
#endif                                           
                                /* +- SPR 17777 */
                                ulUEContext_p,
                                &startIdx,
                                internalCellIndex
                                /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                ,nDmrs_p
                                ,phichSubFrameNum
                                ,iPhich
#endif
                                );
                        /* + CQI_5.0 */
                        if((availableRBs >= minRBs) &&
                                (MAC_SUCCESS == retValue))
                            /* - CQI_5.0 */
                        {
                            /*TDD Config 0 Changes End*/
                            /*Able to serve minDataSize*/ 
                            /*Coverity Id 32413 Fix Start*/
                            MAC_UL_RESERVE_RESOURCES(startIdx, availableRBs,
                                    resourceReserved_p,
                                    bufferRBs_p, inputRBs,
                                    ulUEContext_p->userLocationType,
                                    rbMapNode_p,
                                    rbMapNode_p->rbMap,
                                    internalCellIndex);
                            *allocatedMCS_p = mcsIdx;
                            /*Coverity Id 32413 Fix End*/

#ifdef FDD_CONFIG
                            /** TTIB_Code Start */
                            reserveAlreadyAvailableResForThreeTicks(startIdx, availableRBs,
                                    resourceReserved_p,currentTTI,
                                    ulDelay,ulUEContext_p,
                                    ulUEContext_p->userLocationType,
                                    internalCellIndex);
                            /** TTIB_Code End */
#endif

                            /* CA Stats Changes Start */
                            LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                                    ulUEContext_p->ueIndex,
                                    currentTTI,
                                    NEW_TX,
                                    harqIndex,
                                    availableRBs,
                                    startIdx,
                                    ulGrantType,
                                    determineUlTBSize(mcsIdx,availableRBs),
                                    TRUE,
                                    mcsIdx,
                                    0,
                                    /* Time Averaging changes start */
                                    tpc.tpcCommand
                                    /* Time Averaging changes end */
                                    /* UL_MU_MIMO_CHG START */
                                    ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                                    INVALID_UE_INDEX,
                                    ulUEContext_p->userLocationType,
                                    MAX_DMRS_VALUE,
                                    ulUEContext_p->modulationScheme,
                                    /* UL_MU_MIMO_CHG END */
                                    internalCellIndex
                                        );
                            /* CA Stats Changes End */


                            return retValue;
                        }
                        else
                        {
                            continue;
                        }
                    }//avgMCS > mcsIdx
                }//if mcsToPrbMap_p[mcsIdx].maxContiguous >= minRBs
            }/*for(mcsIdx = csiContext_p->highestReportedMCS*/

        /* FSS was not successful. Do non-FSS using avg MCS. One of the reasons
         * can be that non-frequency hopping SRS configuration was selected.
         */
                            /* ICIC changes start */
                            /*  mac/maccore/src/scheduler/ulSchManager/lteMacPHICH.c +686 */
                            /* check for cell edge and cell center user type 
                             * can be made here and sent in a flag */
#ifdef FDD_CONFIG
                            /** TTIB_Code Start */
                            if(ulUEContext_p->ttiBundlingEnabled)
                            {
                                retValue = allocateUplinkResourcesForTTIBundle(inputRBs,
                                        inputRBs, \
                                        resourceReserved_p, \
                                        isStrictAllocated,
                                        &rbMapNode_p->availableResourceInfo[ulUEContext_p->userLocationType],
                                        rbMapNode_p->rbMap,
                                        ulDelay,
                                        currentTTI,
                                        ulUEContext_p,
                                        internalCellIndex
                                        );
                            }
                            else
                                /** TTIB_Code End */
#endif
                            {
                                retValue = allocateUplinkResources(inputRBs, 
                                        inputRBs, \
                                        resourceReserved_p, \
                                        isStrictAllocated,
                                        &rbMapNode_p->availableResourceInfo[ulUEContext_p->userLocationType],
                                        /* SPR 21958 PUSCH RAT1 Support Start */
                                        rbMapNode_p,
                                        /* SPR 21958 PUSCH RAT1 Support End */
                                        /*CA Changes start  */
                                        internalCellIndex
                                        /*CA Changes end  */
                                        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                        ,nDmrs_p
                                        ,phichSubFrameNum
                                        ,iPhich
#endif
                                        /*TDD Config 0 Changes End*/
                                        /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
                                        ,ulUEContext_p->isUplinkRAT1Supported
#endif
                                        /* SPR 21958 PUSCH RAT1 Support End */
                                        );
                            }
                        if(MAC_SUCCESS == retValue)
                        {
                            if(CE_USER == ulUEContext_p->userLocationType)
                            {
                                updateCCAvailRes(
                                        &rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                                        resourceReserved_p);
                            }
                            /* ICIC changes end */
                            *allocatedMCS_p = avgMCS;
                            /* No need to update buffer RBs as we are allocating the same 
                             * number of RBs as provided by UL Strategy
                             */
                            /* CA Stats Changes Start */
                            LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                                    ulUEContext_p->ueIndex,
                                    currentTTI,
                                    NEW_TX,
                                    harqIndex,
                                    resourceReserved_p->allocatedLength,
                                    resourceReserved_p->allocatedIndex,
                                    ulGrantType,
                                    determineUlTBSize(avgMCS,resourceReserved_p->allocatedLength),
                                    FALSE,
                                    avgMCS,
                                    0,
                                    /* Time Averaging changes start */
                                    tpc.tpcCommand
                                    /* Time Averaging changes end */
                                    /* UL_MU_MIMO_CHG START */
                                    ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                                    INVALID_UE_INDEX,
                                    ulUEContext_p->userLocationType,
                                    MAX_DMRS_VALUE,
                                    ulUEContext_p->modulationScheme,
                                    /* UL_MU_MIMO_CHG END */
                                    internalCellIndex
                                        );
                            /* CA Stats Changes End */

                            return retValue;
                        }
                        else
                        { 
                            return retValue;
                        }        
    }/*isStrictAllocated*/
    else
    {
        /* We try to give the best MCS to the UE. So loop from highest to 
         * lowest reported MCS.

*/ 
#ifdef FDD_CONFIG
        for(;
                (mcsIdx >= lowermcsIdx ) && \
                (mcsIdx != INVALID_MCS_INDEX);
                mcsIdx = mcsGroupingTable_g[mcsIdx].previousMCS )
#else
            for(mcsIdx = csiContext_p->highestReportedMCS;
                    (mcsIdx >= csiContext_p->lowestReportedMCS) && \
                    (mcsIdx != INVALID_MCS_INDEX);
                    mcsIdx = mcsGroupingTable_g[mcsIdx].previousMCS )
#endif
            {
                if(*allocatedMCS_p != mcsIdx)
                {
                    /* SPR 5999 changes start */
                    /*CA Changes start  */
                    minRBs = DETERMINE_RBs_UL(mcsIdx, minDataSize, internalCellIndex); 
                    /*CA Changes end  */
                    /* SPR 5999 changes end */
                }
                else
                {
                    minRBs = inputRBs;
                }
                minRBs = getUplinkMaxRBsInPowerOfTwoThreeFive(minRBs);

#ifdef FDD_CONFIG
                if(ulUEContext_p->ttiBundlingEnabled)
                {
                    if( maxRBs > MAX_TTI_BUNDLE_RB)
                    {
                        maxRBs = MAX_TTI_BUNDLE_RB ;
                    }
                }
#endif
                if(freeRBs <= minRBs)
                {
                    availableRBs = validateATB(freeRBs,
                            /* Time Averaging changes start */
                            &mcsIdx, 
                            ulUEContext_p, 
                            tpc.powerCorrectionVal, 
                            ueMaxPowerFlag_p,ueMinPowerFlag_p,
                            regionPowerMask,NEW_TX,internalCellIndex);

                    /* Time Averaging changes end */

                    /* Should use number of RBs in PUSCH format less than that 
                     * allowed by ATB
                     */
                    availableRBs = getUplinkMinRBsInPowerOfTwoThreeFive(
                            availableRBs);
                    /*ULA_UTP*/
                    if ( mcsToPrbMap_p[mcsIdx].maxContiguous < availableRBs)
                    {
                        /*TTIB cyclomatic complexity Fix*/
                        retValue = getAvailableRbsForLongestSet(&mcsToPrbMap_p[mcsIdx],
                                mcsToPrbMap_p[mcsIdx].maxContiguous,
                                &availableRBs,
                                rbMapNode_p,
#ifdef FDD_CONFIG
                                ulDelay,
                                /* +- SPR 17777 */
                                currentTTI,
#endif
                                /* +- SPR 17777 */
                                ulUEContext_p,
                                &startIdx,
                                internalCellIndex
                                /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                ,nDmrs_p
                                ,phichSubFrameNum
                                ,iPhich
#endif
                                );
                        /*TDD Config 0 Changes End*/
                        /*TTIB cyclomatic complexity Fix*/
                    }
                    else
                    {
                        /*TTIB cyclomatic complexity Fix*/
                        retValue = getAvailableRbsForLongestSet( &mcsToPrbMap_p[mcsIdx],
                                availableRBs,
                                &availableRBs,
                                rbMapNode_p,
#ifdef FDD_CONFIG
                                ulDelay,
                                /* +- SPR 17777 */
                                currentTTI,
#endif 
                                /* +- SPR 17777 */
                                ulUEContext_p,
                                &startIdx,
                                internalCellIndex
                                /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                ,nDmrs_p
                                ,phichSubFrameNum
                                ,iPhich
#endif
                                );
                        /*TDD Config 0 Changes End*/

                        /*TTIB cyclomatic complexity Fix*/
                    }

                    /* Keep updating the MCS on which maximum data can be sent*/
                    /* Cyclomatic Complexity changes - Starts here */

                    /*TDD Config 0 Changes Start*/
                    if (MAC_SUCCESS == retValue)
                    {
                        /*TDD Config 0 Changes End*/    
                        MacUlUpdateLocalMaxTBIfRequired(
                                mcsIdx,
                                &localMaxRBs,
                                &localMaxMCS,
                                availableRBs,
                                startIdx,
                                &localMaxTBSizeRBStartIdx,
                                &localTBSize,
                                &localMaxTBSize
                                /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                ,&localNDmrs
                                ,*nDmrs_p
#endif
                                /*TDD Config 0 Changes End*/
                                );

                    }

                    /* Cyclomatic Complexity changes - end here */
                    if(availableRBs >= freeRBs)
                    {
                        break;
                    }
                    //Should not break because it is possible that localMaxTBSize may be updated at lower MCS
                    //break;
                    /*ULA_UTP*/
                }
                else if (mcsToPrbMap_p[mcsIdx].maxContiguous < minRBs)
                {
                    /*TTIB cyclomatic complexity Fix*/
                    retValue = getAvailableRbsForLongestSet( &mcsToPrbMap_p[mcsIdx],
                            mcsToPrbMap_p[mcsIdx].maxContiguous,
                            &availableRBs,
                            rbMapNode_p,
#ifdef FDD_CONFIG
                            ulDelay,
                            /* +- SPR 17777 */
                            currentTTI,
#endif                    
                            /* +- SPR 17777 */
                            ulUEContext_p,
                            &startIdx,
                            internalCellIndex
                            /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                            ,nDmrs_p
                            ,phichSubFrameNum
                            ,iPhich
#endif
                            );
                    /*TDD Config 0 Changes End*/

                    /*TTIB cyclomatic complexity Fix*/
                    /* Keep updating the MCS on which maximum data can be sent*/
                    /* Cyclomatic Complexity changes - Starts here */

                    if (MAC_SUCCESS == retValue)
                    {
                        MacUlUpdateLocalMaxTBIfRequired(
                                mcsIdx,
                                &localMaxRBs,
                                &localMaxMCS,
                                availableRBs,
                                startIdx,
                                &localMaxTBSizeRBStartIdx,
                                &localTBSize,
                                &localMaxTBSize
                                /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                ,&localNDmrs
                                ,*nDmrs_p
#endif
                                /*TDD Config 0 Changes End*/
                                );
                        /* Cyclomatic Complexity changes - end here */
                    }
                    continue;
                }
                else
                {
                    if(avgMCS <= mcsIdx)
                    {
                        /* SPR 5999 changes start */
                        /*CA Changes start  */
                        maxRBs = DETERMINE_RBs_UL (mcsIdx, maxDataSize, internalCellIndex); 
                        /*CA Changes end  */
                        /* SPR 5999 changes end */
                        maxRBs = getUplinkMaxRBsInPowerOfTwoThreeFive(maxRBs);

                        if(inputRBs < maxRBs)
                        {
                            /* When trying to serve more data than that scehduled by
                             * strategy, limit number of RBs as those given by UL
                             * strategy.
                             */                       
                            maxRBs = inputRBs;
                        }
                        /* Need to call validateATB(maxRBs..) even though 
                         * inputRBs from strategy are already checked against ATB 
                         * because mcsIdx  above maybe different from avgMCS used 
                         * by strategy to check with ATB.
                         */
                        maxRBs = validateATB(maxRBs, \
                                &mcsIdx, \
                                /* Time Averaging changes start */
                                ulUEContext_p, \
                                tpc.powerCorrectionVal, \
                                ueMaxPowerFlag_p,ueMinPowerFlag_p,
                                regionPowerMask,NEW_TX,internalCellIndex);

                        /* Time Averaging changes end */
                        /*TTIB cyclomatic complexity Fix*/
#ifdef FDD_CONFIG
                        /* TTIB_Code Start */
                        if((ulUEContext_p->ttiBundlingEnabled) &&
                                ( maxRBs > MAX_TTI_BUNDLE_RB))
                        {
                            maxRBs = MAX_TTI_BUNDLE_RB ;
                        }
#endif
                        retValue = getAvailableRbsForLongestSet( &mcsToPrbMap_p[mcsIdx],
                                maxRBs,
                                &availableRBs,
                                rbMapNode_p,
#ifdef FDD_CONFIG 
                                ulDelay,
                                /* +- SPR 17777 */
                                currentTTI,
#endif
                                /* +- SPR 17777 */
                                ulUEContext_p,
                                &startIdx,
                                internalCellIndex
                                /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                ,nDmrs_p
                                ,phichSubFrameNum
                                ,iPhich
#endif
                                );
                        /*TDD Config 0 Changes End*/

                        /*TTIB cyclomatic complexity Fix*/
                        /*TDD Config 0 Changes Start*/
                        if (MAC_SUCCESS == retValue)
                        {
                            if(availableRBs >= minRBs)
                            {
                                /*TDD Config 0 Changes End*/
                                /* Able to serve atleast minDataSize*/
                                /*Coverity Id 32413 Fix Start*/
                                MAC_UL_RESERVE_RESOURCES(startIdx, availableRBs, \
                                        resourceReserved_p, \
                                        bufferRBs_p, inputRBs ,\
                                        ulUEContext_p->userLocationType,\
                                        rbMapNode_p,\
                                        rbMapNode_p->rbMap,
                                        internalCellIndex);

                                *allocatedMCS_p = mcsIdx;
                                /*Coverity Id 32413 Fix End*/
#ifdef FDD_CONFIG 
                                /* TTIB_Code Start */
                                reserveAlreadyAvailableResForThreeTicks(startIdx, availableRBs,
                                        resourceReserved_p,currentTTI,
                                        ulDelay,ulUEContext_p,
                                        ulUEContext_p->userLocationType,
                                        internalCellIndex);
                                /* TTIB_Code End */
#endif
                                /* CA Stats Changes Start */
                                LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                                        ulUEContext_p->ueIndex,
                                        currentTTI,
                                        NEW_TX,
                                        harqIndex,
                                        availableRBs,
                                        startIdx,
                                        ulGrantType,
                                        determineUlTBSize(mcsIdx,availableRBs),
                                        TRUE,
                                        mcsIdx,
                                        0,
                                        /* Time Averaging changes start */
                                        tpc.tpcCommand
                                        /* Time Averaging changes end */
                                        /* UL_MU_MIMO_CHG START */
                                        ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                                        INVALID_UE_INDEX,
                                        ulUEContext_p->userLocationType,
                                        MAX_DMRS_VALUE,
                                        ulUEContext_p->modulationScheme,
                                        /* UL_MU_MIMO_CHG END */
                                        internalCellIndex
                                            );
                                /* CA Stats Changes End */

                                return retValue;
                            }
                            else
                            {
                                /* Keep updating the MCS on which maximum data can be 
                                 * sent
                                 */
                                /* Cyclomatic Complexity changes - Starts here */
                                MacUlUpdateLocalMaxTBIfRequired(
                                        mcsIdx,
                                        &localMaxRBs,
                                        &localMaxMCS,
                                        availableRBs,
                                        startIdx,
                                        &localMaxTBSizeRBStartIdx,
                                        &localTBSize,
                                        &localMaxTBSize
                                        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                        ,&localNDmrs
                                        ,*nDmrs_p
#endif
                                        /*TDD Config 0 Changes End*/
                                        );
                                /* Cyclomatic Complexity changes - end here */
                            }
                        }
                    }//avgMCS <= mcsIdx
                    else
                    {
                        /* Since mcsIdx < avgMCS, more PRBs will be needed than 
                         * those allocated by UL Strategy to serve minDataSize. 
                         * Utilize buffer RBs in this case.
                         */
                        /* ICIC changes start */
                        if((avgMCS - mcsIdx)< WB_SB_MCS_DIFF_THRESHOLD)
                        {
                            if (*bufferRBs_p >= minRBs-inputRBs)
                            {
                                /*TTIB cyclomatic complexity Fix*/
                                retValue = getAvailableRbsForLongestSet( &mcsToPrbMap_p[mcsIdx],
                                        minRBs,
                                        &availableRBs,
                                        rbMapNode_p,
#ifdef FDD_CONFIG
                                        ulDelay,
                                        /* +- SPR 17777 */
                                        currentTTI,
#endif
                                        /* +- SPR 17777 */
                                        ulUEContext_p,
                                        &startIdx,
                                        internalCellIndex
                                        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                        ,nDmrs_p
                                        ,phichSubFrameNum
                                        ,iPhich
#endif
                                        );
                                /*TDD Config 0 Changes End*/

                                /*TTIB cyclomatic complexity Fix*/
                                if (MAC_SUCCESS == retValue)
                                {
                                    if(minRBs == availableRBs)
                                    {
                                        /*Coverity Id 32413 Fix Start*/
                                        MAC_UL_RESERVE_RESOURCES(startIdx, availableRBs, \
                                                resourceReserved_p,\
                                                bufferRBs_p, inputRBs ,\
                                                ulUEContext_p->userLocationType,\
                                                rbMapNode_p,\
                                                rbMapNode_p->rbMap,
                                                internalCellIndex);

                                        *allocatedMCS_p = mcsIdx;
                                        /*Coverity Id 32413 Fix End*/
#ifdef FDD_CONFIG 
                                        /** TTIB_Code Start */

                                        reserveAlreadyAvailableResForThreeTicks(startIdx, availableRBs,
                                                resourceReserved_p,currentTTI,
                                                ulDelay,ulUEContext_p,
                                                ulUEContext_p->userLocationType,
                                                internalCellIndex);
#endif
                                        /* CA Stats Changes Start */
                                        LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                                                ulUEContext_p->ueIndex,
                                                currentTTI,
                                                NEW_TX,
                                                harqIndex,
                                                localMaxRBs,
                                                localMaxTBSizeRBStartIdx,
                                                ulGrantType,
                                                determineUlTBSize(localMaxMCS,localMaxRBs),
                                                TRUE,
                                                localMaxMCS,
                                                0,
                                                /* Time Averaging changes start */
                                                tpc.tpcCommand
                                                /* Time Averaging changes end */
                                                /* UL_MU_MIMO_CHG_START */
                                                ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                                                INVALID_UE_INDEX,
                                                ulUEContext_p->userLocationType,
                                                MAX_DMRS_VALUE,
                                                ulUEContext_p->modulationScheme,
                                                /* UL_MU_MIMO_CHG_END */
                                                internalCellIndex
                                                    );
                                        /* CA Stats Changes End */


                                        return retValue; 
                                    }
                                    else
                                    {
                                        /* Cyclomatic Complexity changes - Starts here */
                                        MacUlUpdateLocalMaxTBIfRequired(
                                                mcsIdx,
                                                &localMaxRBs,
                                                &localMaxMCS,
                                                availableRBs,
                                                startIdx,
                                                &localMaxTBSizeRBStartIdx,
                                                &localTBSize,
                                                &localMaxTBSize
                                                /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                                ,&localNDmrs
                                                ,*nDmrs_p
#endif
                                                /*TDD Config 0 Changes End*/
                                                );

                                        /* Cyclomatic Complexity changes - end here */
                                    }
                                }
                            }
                            else
                            {
                                /*TTIB cyclomatic complexity Fix*/
                                retValue = getAvailableRbsForLongestSet( &mcsToPrbMap_p[mcsIdx],
                                        mcsToPrbMap_p[mcsIdx].maxContiguous,
                                        &availableRBs,
                                        rbMapNode_p,
#ifdef FDD_CONFIG 
                                        ulDelay,
                                        /* +- SPR 17777 */
                                        currentTTI,
#endif                          
                                        /* +- SPR 17777 */
                                        ulUEContext_p,
                                        &startIdx,
                                        internalCellIndex
                                        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                        ,nDmrs_p
                                        ,phichSubFrameNum
                                        ,iPhich
#endif
                                        );
                                /*TDD Config 0 Changes End*/

                                /*TTIB cyclomatic complexity Fix*/
                                /* ICIC changes end */
                                /* Cyclomatic Complexity changes - Starts here */
                                if (MAC_SUCCESS == retValue)
                                {
                                    MacUlUpdateLocalMaxTBIfRequired(
                                            mcsIdx,
                                            &localMaxRBs,
                                            &localMaxMCS,
                                            availableRBs,
                                            startIdx,
                                            &localMaxTBSizeRBStartIdx,
                                            &localTBSize,
                                            &localMaxTBSize
                                            /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                            ,&localNDmrs
                                            ,*nDmrs_p
#endif
                                            /*TDD Config 0 Changes End*/
                                            );
                                }
                                /* Cyclomatic Complexity changes - end here */
                            }/*if (*bufferRBs_p >= minRBs-inputRBs)*/
                        }/* threshold check */
                        else
                        {
                            /*TTIB cyclomatic complexity Fix*/
                            retValue = getAvailableRbsForLongestSet( &mcsToPrbMap_p[mcsIdx],
                                    inputRBs,
                                    &availableRBs,
                                    rbMapNode_p,
#ifdef FDD_CONFIG
                                    ulDelay,
                                    /* +- SPR 17777 */
                                    currentTTI,
#endif                        
                                    /* +- SPR 17777 */
                                    ulUEContext_p,
                                    &startIdx,
                                    internalCellIndex
                                    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                    ,nDmrs_p
                                    ,phichSubFrameNum
                                    ,iPhich
#endif
                                    );
                            /*TDD Config 0 Changes End*/

                            /*TTIB cyclomatic complexity Fix*/
                            if (MAC_SUCCESS == retValue)
                            {
                                if(availableRBs == inputRBs)
                                {
                                    /*Coverity Id 32413 Fix Start*/
                                    MAC_UL_RESERVE_RESOURCES(startIdx, availableRBs, \
                                            resourceReserved_p,\
                                            bufferRBs_p, inputRBs ,\
                                            ulUEContext_p->userLocationType,\
                                            rbMapNode_p,\
                                            rbMapNode_p->rbMap,
                                            internalCellIndex);

                                    *allocatedMCS_p = mcsIdx;
                                    /*Coverity Id 32413 Fix End*/
#ifdef FDD_CONFIG 
                                    /** TTIB_Code Start */

                                    reserveAlreadyAvailableResForThreeTicks(startIdx, availableRBs,
                                            resourceReserved_p,currentTTI,
                                            ulDelay,ulUEContext_p,
                                            ulUEContext_p->userLocationType,
                                            internalCellIndex);
#endif
                                    /* CA Stats Changes Start */
                                    LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                                            ulUEContext_p->ueIndex,
                                            currentTTI,
                                            NEW_TX,
                                            harqIndex,
                                            localMaxRBs,
                                            localMaxTBSizeRBStartIdx,
                                            ulGrantType,
                                            determineUlTBSize(localMaxMCS,localMaxRBs),
                                            TRUE,
                                            localMaxMCS,
                                            0,
                                            /* Time Averaging changes start */
                                            tpc.tpcCommand
                                            /* Time Averaging changes end */
                                            /* UL_MU_MIMO_CHG_START */
                                            ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                                            INVALID_UE_INDEX,
                                            ulUEContext_p->userLocationType,
                                            MAX_DMRS_VALUE,
                                            ulUEContext_p->modulationScheme,
                                            /* UL_MU_MIMO_CHG_END */
                                            internalCellIndex
                                                );
                                    /* CA Stats Changes End */
                                    return retValue;
                                }
                                else
                                {
                                    /* Cyclomatic Complexity changes - Starts here */
                                    MacUlUpdateLocalMaxTBIfRequired(
                                            mcsIdx,
                                            &localMaxRBs,
                                            &localMaxMCS,
                                            availableRBs,
                                            startIdx,
                                            &localMaxTBSizeRBStartIdx,
                                            &localTBSize,
                                            &localMaxTBSize
                                            /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                            ,&localNDmrs
                                            ,*nDmrs_p
#endif
                                            /*TDD Config 0 Changes End*/
                                            );

                                    /* Cyclomatic Complexity changes - end here */
                                }
                            }
                            else
                            {
                                /*TTIB cyclomatic complexity Fix*/
                                /* COVERITY 69297 START */
                                retValue = getAvailableRbsForLongestSet( &mcsToPrbMap_p[mcsIdx],
                                        mcsToPrbMap_p[mcsIdx].maxContiguous,
                                        &availableRBs,
                                        rbMapNode_p,
#ifdef FDD_CONFIG 
                                        ulDelay,
                                        /* +- SPR 17777 */
                                        currentTTI,
#endif                          
                                        /* +- SPR 17777 */
                                        ulUEContext_p,
                                        &startIdx,
                                        internalCellIndex
                                        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                        ,nDmrs_p
                                        ,phichSubFrameNum
                                        ,iPhich
#endif
                                        );
                                if( MAC_FAILURE == retValue )
                                {
                                    return MAC_FAILURE;
                                }
                                /* COVERITY 69297 END */
                                /*TTIB cyclomatic complexity Fix*/
                                /* ICIC changes end */
                                /* Cyclomatic Complexity changes - Starts here */
                                MacUlUpdateLocalMaxTBIfRequired(
                                        mcsIdx,
                                        &localMaxRBs,
                                        &localMaxMCS,
                                        availableRBs,
                                        startIdx,
                                        &localMaxTBSizeRBStartIdx,
                                        &localTBSize,
                                        &localMaxTBSize
                                        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                                        ,&localNDmrs
                                        ,*nDmrs_p
#endif
                                        /*TDD Config 0 Changes End*/
                                        );
                                /* Cyclomatic Complexity changes - end here */
                            }/*if (*bufferRBs_p >= minRBs-inputRBs)*/
                        }/* threshold check */

                        /* ICIC changes end */
                    }/*avgMCS > mcsIdx*/
                }/*if mcsToPrbMap_p[mcsIdx].maxContiguous >= minRBs*/
            }/*for(mcsIdx = csiContext_p->highestReportedMCS*/
        /* Serve with maximum TB Size found above*/
        if(localMaxTBSize >= UL_TB_MINIMUM_SIZE_IN_BYTES)
        {
            /*Coverity Id 32413 Fix Start*/
            MAC_UL_RESERVE_RESOURCES(localMaxTBSizeRBStartIdx, localMaxRBs, \
                    resourceReserved_p,\
                    bufferRBs_p, inputRBs,
                    ulUEContext_p->userLocationType,\
                    rbMapNode_p,\
                    rbMapNode_p->rbMap,
                    internalCellIndex);

            *allocatedMCS_p = localMaxMCS;
            /*Coverity Id 32413 Fix End*/
            /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
            /*Fill the Ndmrs finally allocated on the basis of longest RB set
             * available*/
            *nDmrs_p = localNDmrs;
#endif
            /*TDD Config 0 Changes End*/

#ifdef FDD_CONFIG
            /* TTIB_Code Start */
            reserveAlreadyAvailableResForThreeTicks(startIdx, availableRBs,
                    resourceReserved_p,currentTTI,
                    ulDelay,ulUEContext_p,
                    ulUEContext_p->userLocationType,
                    internalCellIndex);
#endif
            /* CA Stats Changes Start */
            LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                    ulUEContext_p->ueIndex,
                    currentTTI,
                    NEW_TX,
                    harqIndex,
                    localMaxRBs,
                    localMaxTBSizeRBStartIdx,
                    ulGrantType,
                    determineUlTBSize(localMaxMCS,localMaxRBs),
                    TRUE,
                    localMaxMCS,
                    0,
                    /* Time Averaging changes start */
                    tpc.tpcCommand
                    /* Time Averaging changes end */
                    /* UL_MU_MIMO_CHG START */
                    ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                    INVALID_UE_INDEX,
                    ulUEContext_p->userLocationType,
                    MAX_DMRS_VALUE,
                    ulUEContext_p->modulationScheme,
                    /* UL_MU_MIMO CHG_END */
                    internalCellIndex
                        );
            /* CA Stats Changes End */

            return MAC_SUCCESS;        
        }
        else
        {
            return MAC_FAILURE;          
        }/*Serve with maximum TB Size found above*/
    }/*!isStrictAllocated*/


}//allocateFrequencySelectiveUplinkResources
/* ULA_CHG*/

/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
/*********************************************************************************
 * Function Name : allocateResourcesForSecondPuschExecutionTddConfig0
 * Inputs        : ulHarqInfoForPreviousUlSubframe_p
 * 		  pdcchULNewUE_p
 * 		  ueIndexingInfo_p
 * 		  bufferRBs
 * 		  allocatedMCS_p
 * 		  ulUEContext_p
 * 		  rbMapNode_p
 * 		  resourceReserved_p
 * 		  freeRBs
 * 		  currentTTI
 * 		  iPhich
 * 		  subFrame
 * 		  phichSubFrameNum
 * Outputs       : None
 * Returns       : MAC_SUCCESS or MAC_FAILURE
 * Description   : This function will try to allocate the resources for second PUSCH 
 * 		  execution in case of TDD Config 0, and we are taking out a common 
 * 		  DCI for both +k and +7 U frame. 
 * 		  It will allocate same resources to +7 U frame, as was allocated 
 * 		  to +k U frame.
 * 		  This function is common to both FSS and Non-FSS allcations.
 ***********************************************************************************/
            STATIC  MacRetType allocateResourcesForSecondPuschExecutionTddConfig0(
                    ULHarqInfo* ulHarqInfoForPreviousUlSubframe_p,
                    PdcchULNewUE *pdcchULNewUE_p,
                    UeScheduledInfo *ueIndexingInfo_p,
                    SInt8 bufferRBs,
                    UInt8 *allocatedMCS_p,
                    ULUEContext *ulUEContext_p,
                    RbMapNode *rbMapNode_p,
                    AllocatedResInfo *resourceReserved_p,
                    UInt32 freeRBs,
                    /* SPR 15909 fix start */
                    tickType_t currentTTI,
                    /* SPR 15909 fix end */
                    UInt8 iPhich,
                    UInt8 phichSubFrameNum,
                    InternalCellIndex internalCellIndex
                    )
{
    UInt8 addReqRB         = 0;
    UInt8 nPhichGrp        = 0;
    UInt8 nPhichSeq        = 0;
    MacRetType retVal      = MAC_FAILURE;
    UInt8 avgMCS           = pdcchULNewUE_p->avgMCS;
    UInt32 minDataSize     = pdcchULNewUE_p->minDataSize;
    UInt32 maxDataSize     = pdcchULNewUE_p->maxDataSize;
    UInt8 minRBs           = 0;
    UInt8 maxRBs           = 0;

    /* Initially we check the availability of free RB's*/
    if(freeRBs < ulHarqInfoForPreviousUlSubframe_p->riLength)
    {
        /* Number of RBs left to be allocated in PUSCH resource map
         * are less than the RB's allocated in previous U frame.
         * Hence we will never be able to allocate the RB's
         * Drop the U and return MAC_FAILURE
         */
#ifdef MAC_AUT_TEST
        rbAllocationFailureInfo = MAC_INSUFFICIENT_FREE_RBs;
#endif
        LOG_MAC_MSG(MAC_PUSCH_FAILURE_SECOND_EXECUTION, LOGDEBUG, MAC_PUSCH,
                currentTTI, ulUEContext_p->ueIndex,
                ulUEContext_p->userLocationType,
                ulHarqInfoForPreviousUlSubframe_p->riLength,
                freeRBs, *allocatedMCS_p, 0, 0, __func__,
                "Max RBs that can be allocated for this UL are less than those assigned in +k");

        return retVal;
    }

    maxRBs = DETERMINE_RBs_UL(ulHarqInfoForPreviousUlSubframe_p->mcsIndex, maxDataSize,internalCellIndex);

    /* We shall try to allocate same resources for Common DCI0 in +7
     * only when riLength allocated in +k is less than the maxRBs
     * that can be scheduled by this U frame.
     */ 
    if (ulHarqInfoForPreviousUlSubframe_p->riLength <= maxRBs)
    {
        /* In case rbs reserved in +k frame are greater than the rb
         * requirement decided by strategy for +7, we need to check the
         * availability of buffer RBs. If buffer RBs are adequate only then
         * can we go ahead with resource allocation in +7 otherwise drop this
         * UL Tx*/
        if (ulHarqInfoForPreviousUlSubframe_p->riLength > ueIndexingInfo_p->requiredRB)
        {
            /* incase of non- strict allocation */
            if (!(ueIndexingInfo_p->strictRBAllocation))
            {
                addReqRB = ulHarqInfoForPreviousUlSubframe_p->riLength - ueIndexingInfo_p->requiredRB;

                /* if avgMCS is greater than the mcsIndex of previous U frame, we will be needing
                 * more PRB's than those allocated by UL strategy
                 * Hence we need to check the availability of buffer RB's
                 */
                if ((ulHarqInfoForPreviousUlSubframe_p->mcsIndex) < avgMCS)
                {
                    if ((avgMCS - ulHarqInfoForPreviousUlSubframe_p->mcsIndex) < WB_SB_MCS_DIFF_THRESHOLD)
                    {
                        if (bufferRBs >= addReqRB)
                        {
                            ueIndexingInfo_p->requiredRB += addReqRB;
                        }
                        else
                        {
#ifdef MAC_AUT_TEST
                            rbAllocationFailureInfo = MAC_INSUFFICIENT_BUFFER_RBs;
#endif
                            LOG_MAC_MSG(MAC_PUSCH_FAILURE_SECOND_EXECUTION, 
                                    LOGDEBUG, MAC_PUSCH, currentTTI, 
                                    ulUEContext_p->ueIndex,
                                    ulUEContext_p->userLocationType,
                                    ulHarqInfoForPreviousUlSubframe_p->riLength,
                                    ueIndexingInfo_p->requiredRB,
                                    *allocatedMCS_p, 0, 0, __func__,
                                    "No buffer RBs available incase mcsIndex < avgMCS,\
                                    Hence we cannot allocate RBs");
                            return retVal;
                        }
                    }
                    else
                    {
                        /* If incase the difference of the avgMCS and mcsIndex of previous U frame
                         * greater than WB_SB_MCS_DIFF_THRESHOLD, we shall not check for the buffer
                         * RBs. We will drop the U, and return MAC_FAILURE
                         */
#ifdef MAC_AUT_TEST
                        rbAllocationFailureInfo = MAC_EXCEEDING_WB_SB_MCS_DIFF_THRESHOLD;
#endif
                        LOG_MAC_MSG(MAC_PUSCH_FAILURE_SECOND_EXECUTION, LOGDEBUG, 
                                MAC_PUSCH, currentTTI, ulUEContext_p->ueIndex,
                                ulUEContext_p->userLocationType,
                                ulHarqInfoForPreviousUlSubframe_p->riLength,
                                ueIndexingInfo_p->requiredRB,
                                *allocatedMCS_p, 0, 0, __func__,
                                "The difference in the avgMCS and MCS index of previous U freme\
                                is greater than the WB_SB_MCS_DIFF_THRESHOLD, Hence we will not\
                                check for buffer RB's, and retuen MAC_FAILURE");
                        return retVal;
                    }
                }
                /* If avgMCS = mcsIndex of previous UL subframe, we check for buffer RB's
                 * Non FSS Scheduling
                 */
                else if (ulHarqInfoForPreviousUlSubframe_p->mcsIndex == avgMCS)
                {
                    if (bufferRBs >= addReqRB)
                    {
                        ueIndexingInfo_p->requiredRB += addReqRB;
                    }
                    else
                    {
#ifdef MAC_AUT_TEST
                        rbAllocationFailureInfo = MAC_INSUFFICIENT_BUFFER_RBs;
#endif
                        LOG_MAC_MSG(MAC_PUSCH_FAILURE_SECOND_EXECUTION, LOGDEBUG, 
                                MAC_PUSCH, currentTTI, ulUEContext_p->ueIndex,
                                ulUEContext_p->userLocationType,
                                ulHarqInfoForPreviousUlSubframe_p->riLength,
                                ueIndexingInfo_p->requiredRB,
                                *allocatedMCS_p, 0, 0,
                                __func__,"No buffer RBs available incase mcsIndex < avgMCS,\
                                Hence we cannot allocate RBs");
                        return retVal;
                    }
                }
                else
                {
                    /* In case avgMCS is less than the MCS allocated in the previous U frame, 
                     * We should not give extra RBs when mcsIndex > avgMCS. 
                     */
#ifdef MAC_AUT_TEST
                    rbAllocationFailureInfo = MAC_MCS_GREATER_THAN_AVG_MCS;
#endif
                    LOG_MAC_MSG(MAC_PUSCH_FAILURE_SECOND_EXECUTION, LOGDEBUG, 
                            MAC_PUSCH, currentTTI, ulUEContext_p->ueIndex,
                            ulUEContext_p->userLocationType,
                            ulHarqInfoForPreviousUlSubframe_p->riLength,
                            ueIndexingInfo_p->requiredRB,
                            *allocatedMCS_p, 0, 0, __func__,
                            "Should not give extra RBs when  mcsIndex > avgMCS");
                    return retVal;
                }
            }
            else
            {
                /* In case of strict RB allocation
                 * We limit the number of RB's required by RI length.
                 */
                ueIndexingInfo_p->requiredRB = ulHarqInfoForPreviousUlSubframe_p->riLength;
            }
        }
        else
        {
            /* If strict allocation flag is set to false, and RB's reserved in +k frame are
             * less than or equal to RB's decided by strategy for +7, we limit the number of RB's
             * required in +7 by the RB's reserved in +k frame.
             */
            if (!(ueIndexingInfo_p->strictRBAllocation))
            {
                ueIndexingInfo_p->requiredRB = ulHarqInfoForPreviousUlSubframe_p->riLength;
            }
            else
            {
                /* In case when strict allocation is true.
                 * We check for minDataSize, If RB length of RB's scheduled in +k frame is less than the
                 * min data size,we will return FAILURE.
                 */

                minRBs = DETERMINE_RBs_UL(ulHarqInfoForPreviousUlSubframe_p->mcsIndex, minDataSize,internalCellIndex);
                if (ulHarqInfoForPreviousUlSubframe_p->riLength < minRBs)
                {
#ifdef MAC_AUT_TEST
                    rbAllocationFailureInfo = MAC_RI_LENGTH_LESS_THAN_MIN_RBs;
#endif
                    LOG_MAC_MSG(MAC_PUSCH_FAILURE_SECOND_EXECUTION,LOGDEBUG,MAC_PUSCH,\
                            currentTTI,\
                            ulUEContext_p->ueIndex,\
                            ulUEContext_p->userLocationType,\
                            ulHarqInfoForPreviousUlSubframe_p->riLength,\
                            ueIndexingInfo_p->requiredRB,\
                            *allocatedMCS_p, 0, 0, __func__,
                            "In case of strict allocation, cannot give less than minRBs for this U-frame");
                    return retVal;
                }

                /* Limiting the number of required RB's with the rb length
                 * reserved in the +k frame
                 */
                else
                {
                    ueIndexingInfo_p->requiredRB = ulHarqInfoForPreviousUlSubframe_p->riLength;
                }
            }
        }

        /* Check if the same Ndmrs is available at +7 as that used for
         * allocation in +k*/
                    nPhichGrp  = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][ulHarqInfoForPreviousUlSubframe_p->riStart][ulHarqInfoForPreviousUlSubframe_p->nDmrs];
                nPhichSeq  = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][ulHarqInfoForPreviousUlSubframe_p->riStart][ulHarqInfoForPreviousUlSubframe_p->nDmrs];
                if(!(phichSequenceNoPerPhichGroup_g[internalCellIndex][phichSubFrameNum][nPhichGrp]\
                            & (ONE << nPhichSeq)))
                {
#ifdef MAC_AUT_TEST
                    rbAllocationFailureInfo = MAC_SAME_NDMRS_UNAVAILABLE;
#endif
                    return retVal;
                }

                /*Check if same set of resources are available in +7 as that of in
                 * +k*/
                        retVal = allocateUplinkResourcesForSecondPuschExecutionTddConfig0(
                                ulHarqInfoForPreviousUlSubframe_p->riStart,
                                ulHarqInfoForPreviousUlSubframe_p->riLength,
                                resourceReserved_p,
                                &rbMapNode_p->availableResourceInfo[ulUEContext_p->userLocationType],
                                rbMapNode_p->rbMap,
                                internalCellIndex);

                    if (MAC_SUCCESS != retVal)
                    {
#ifdef MAC_AUT_TEST
                        rbAllocationFailureInfo = MAC_ALLOCATION_FAILURE;
#endif
                        /*Required resources are not available in +7 U frame*/
                        LOG_MAC_MSG(MAC_PUSCH_FAILURE_SECOND_EXECUTION,LOGDEBUG,MAC_PUSCH,\
                                currentTTI,\
                                ulUEContext_p->ueIndex,\
                                ulUEContext_p->userLocationType,\
                                ulHarqInfoForPreviousUlSubframe_p->riLength,\
                                ueIndexingInfo_p->requiredRB,\
                                *allocatedMCS_p, 0, 0, __func__,
                                "Required resources not available in +7 U frame");

                        return retVal;
                    }
                    else
                    {
                        return retVal;
                    }
    }
    else
    {
        /*Required resources by +k frame is more than the max resources that can
         * be allocated in this U frame*/
#ifdef MAC_AUT_TEST
        rbAllocationFailureInfo = MAC_RI_LENGTH_GREATER_THAN_MAX_RBs;
#endif
        LOG_MAC_MSG(MAC_PUSCH_FAILURE_SECOND_EXECUTION,LOGDEBUG,MAC_PUSCH,\
                currentTTI,\
                ulUEContext_p->ueIndex,\
                ulUEContext_p->userLocationType,\
                ulHarqInfoForPreviousUlSubframe_p->riLength,\
                maxRBs,\
                *allocatedMCS_p, 0, 0, __func__,
                "Max required RBs for this UL are less than those assigned in +k U frame");
        return retVal;
    }
}

/*********************************************************************************
 * Function Name : allocateFrequencySelectiveUplinkResourcesForTddConfig0
 * Inputs        : ulHarqInfoForPreviousUlSubframe_p
 *                 pdcchULNewUE_p
 *                 currentGlobalTick
 *                 ulUEContext_p
 *		   ueIndexingInfo_p
 *		   bufferRBs_p
 *		   allocatedMCS_p
 *		   rbMapNum
 *		   resourceReserved_p
 *		   freeRBs
 *		   phichSubFrameNum
 *		   tpc
 *		   iPhich
 * Outputs       : None
 * Returns       : MAC_SUCCESS or MAC_FAILURE
 * Description   : This function Allocates resources for FSS Scheduling incase of a 
 * 		   single DCI for +7 U frame. It allocates the same resources as they
 * 		   were allocated in +k execution. If the same resources are not 
 * 		   available this funcion returns zero.
 ***********************************************************************************/     
                            STATIC MacRetType allocateFrequencySelectiveUplinkResourcesForTddConfig0(
                                    ULHarqInfo* ulHarqInfoForPreviousUlSubframe_p,
                                    PdcchULNewUE *pdcchULNewUE_p,
                                    /* SPR 15909 fix start */
                                    /* +- SPR 17777 */
#ifdef SSI_DEBUG_STATS
                                    tickType_t currentGlobalTick,
                                    TpcTriggeredToPowerCorrection tpc,
#endif
                                    /* +- SPR 17777 */
                                    /* SPR 15909 fix end */
                                    ULUEContext* ulUEContext_p,
                                    UeScheduledInfo *ueIndexingInfo_p,
                                    SInt8 *bufferRBs_p,
                                    UInt8 *allocatedMCS_p,
                                    UInt8 rbMapNum,
                                    AllocatedResInfo *resourceReserved_p,
                                    UInt32 freeRBs,
                                    UInt8 phichSubFrameNum,
                                    /* +- SPR 17777 */
                                    UInt8 iPhich,
                                    InternalCellIndex internalCellIndex
                                    )
{
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    MacRetType retValue = MAC_FAILURE;
    UInt8 inputRBs = ueIndexingInfo_p->requiredRB;
    UInt8 mcsIndex = ulHarqInfoForPreviousUlSubframe_p->mcsIndex;

    /* Update the allocated mcs for +7 execution with the same mcs 
     * of previous U subframe */
    *allocatedMCS_p = mcsIndex;

    /* In case of TDD Config 0 when the same dci 0 goes for +k and +7 then
     * while running the PUSCH EL for +7 U frame, we just need to check whether
     * the same set of resources are available at +7 as that in +k.
     * The mcs at +7 should be same as +k.
     * The NDmrs, Phich grp and phich seq shld also be same.
     * If any of the above conditions fail,then MAC_FAILURE is returned and the
     * UE is dropped for UL TX.*/

    retValue = allocateResourcesForSecondPuschExecutionTddConfig0( 
            ulHarqInfoForPreviousUlSubframe_p,
            pdcchULNewUE_p,
            ueIndexingInfo_p,
            *bufferRBs_p,
            allocatedMCS_p,
            ulUEContext_p,
            rbMapNode_p,
            resourceReserved_p,
            freeRBs,
            currentTTI,
            iPhich,
            phichSubFrameNum,
            internalCellIndex
            );
    if(MAC_SUCCESS == retValue)
    {
        /* Update the buffer RB's */
        *bufferRBs_p -= (ueIndexingInfo_p->requiredRB - inputRBs);
        if(CE_USER == ulUEContext_p->userLocationType)
        {
            updateCCAvailRes(
                    &rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                    resourceReserved_p);
        }

        LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                ulUEContext_p->ueIndex,
                currentGlobalTick,
                NEW_TX,
                ueIndexingInfo_p->ulHarqProcessId,
                resourceReserved_p->allocatedLength,
                resourceReserved_p->allocatedIndex,
                ueIndexingInfo_p->ulGrantRequestType,
                determineUlTBSize(mcsIndex,resourceReserved_p->allocatedLength),
                TRUE,
                mcsIndex,
                0,
                tpc.tpcCommand
                ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                INVALID_UE_INDEX,
                ulUEContext_p->userLocationType,
                MAX_DMRS_VALUE,
                ulUEContext_p->modulationScheme,
                internalCellIndex
                );

        return retValue;
    }
    else
    {
        return retValue;
    }
}
#endif
/*TDD Config 0 Changes End*/

/*********************************************************************************************
 * Function Name :      allocatePUSCHResourcesForNewUEs
 * Inputs        :      ulUEContext_p - The pointer to the ULUEContext,
 *                      pdcchULNewUE_p - The pointer to PdcchULNewUE,
 *                      tempDirectIndexingUEInfo_p - The pointer to the DirectIndexingUEInfo,
 *                      resourceReserved - The pointer to the AllocatedResInfo,
 *                      subFrameNumber - The subFrame number when it is sent,
 *                      freeRBs,
 *                      bufferRBs_p,
 *                      tpc - Triggered To Power Correction,
 *                      ueMaxPowerFlag_p,
 *                      ueMinPowerFlag_p,
 *                      spsAllocFlag,
 *                      hiIndex,
 *                      currentGlobalTick - Current Tick,
 *                      ulDelay - UL Delay
 *                      internalCellIndex - Cell-Index at MAC
 * Outputs              None
 * Returns       :      MAC_SUCCESS /MAC_FAILURE
 * Description   :      This function will work on per node basis and try to allocate.
 **********************************************************************************************/
#ifdef FDD_CONFIG
            /* SPR 5599 changes start (ZIP ID 129107) */
            MacRetType allocatePUSCHResourcesForNewUEs(
                    /* SPR 5599 changes end (ZIP ID 129107) */
                    ULUEContext* ulUEContext_p,
                    PdcchULNewUE *pdcchULNewUE_p,
                    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p ,
                    AllocatedResInfo* resourceReserved_p,
                    UInt8 subFrameNumber,
                    UInt32 freeRBs,
                    SInt8 *bufferRBs_p, 
                    /* Time Averaging changes start */
                    TpcTriggeredToPowerCorrection tpc,
                    UInt32 *ueMaxPowerFlag_p, 
                    UInt32 *ueMinPowerFlag_p,
                    UInt8 spsAllocFlag,
                    /* Time Averaging changes end */
                    /* UL_MU_MIMO_CHG Start */
                    UInt8 hiIndex, 
                    /* SPR 15909 fix start */
                    tickType_t currentGlobalTick, 
                    /* SPR 15909 fix end */
                    UInt8 ulDelay,
                    /* UL_MU_MIMO_CHG Changes End */
                    /*CA Changes start  */
                    InternalCellIndex internalCellIndex
                    /*CA Changes end  */
                    )
{
    ULHarqInfo* tempULHarqInfo_p = PNULL;
    UInt32  tempAllocTBSize = 0;
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
    UInt32 retValue = MAC_FAILURE;
    UInt8 allocatedMCS = pdcchULNewUE_p->avgMCS;
    /* +DYNAMIC_ICIC */
    /*CA Changes start  */
    ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + ulPowerMaskIndex_g[internalCellIndex];
    /*CA Changes end  */
    SInt32 regionPowerMask = ulPowerMaskInfo_p->powerMask[ulUEContext_p->userLocationType];
    /* -DYNAMIC_ICIC */
    /* UL_MU_MIMO_CHG Start */
    UInt16        buddyUeIndex       = tempDirectIndexingUEInfo_p->buddyUeIndex;
    ULUEContext*  ulBuddyUEContext_p = PNULL;
    /* UL_MU_MIMO_CHG End */

    /* + CQI_5.0 */
    /* +DYNAMIC_ICIC */    
    CHECK_QCI_POWER_MASK_AND_UPDATE_REGION_MASK(ulUEContext_p,regionPowerMask);
    /* -DYNAMIC_ICIC */
    /* + SPR 21811 Fix */
    /* When FSS is enabled , Resources will not be allocated using FSS algorithm 
     * until at least one SRS Indication is received for that UE. So, before 
     * receiving first SRS Indication for a UE, UL resources will be allocated 
     * in Non-FSS manner, i.e using WB MCS. ALso, If SRS indication is received 
     * for a UE but it has been observed that the highest reported MCS for subband 
     * is lower than the Wideband MCS, then also RB allocation will be done via 
     * Non-FSS procedure using WB MCS (because wideband MCS is assumed to be usable 
     * across complete system BW i.e., all subbands). With this approach, RBs can
     * be allocated in more efficient way.
     */
    UInt8 allocFreqSelectiveSchFlag = FALSE;
    if((FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->freqSelectiveSchUL) && 
            (TRUE == ulUEContext_p->firstSRSIndRecvd) && 
            (ulUEContext_p->csiContext.highestReportedMCS >= ulUEContext_p->mcsIndex))
    {
        allocFreqSelectiveSchFlag = TRUE;
    }
    /* - SPR 21811 Fix */
    /* SPR 8908 FIX START */
#ifdef OVERSHOOT_CORRECTION
    /* + SPR 21811 Fix */
    if ( (TRUE == allocFreqSelectiveSchFlag) &&
            /* - SPR 21811 Fix */
            (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG !=
             tempDirectIndexingUEInfo_p->ulGrantRequestType) &&
            /** CA-TDD Changes Start **/
            (OVERLOAD_LEVEL_NORMAL == macOverLoadLevel_g[internalCellIndex] ) )
        /** CA-TDD Changes Stop **/
#else
        /* + SPR 21811 Fix */
    if ( (TRUE == allocFreqSelectiveSchFlag) &&
            /* - SPR 21811 Fix */
            (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG !=
            tempDirectIndexingUEInfo_p->ulGrantRequestType) )
            /* - CQI_5.0 */
#endif
            /* SPR 8908 FIX END */
    {
        /* Coverity 55292*/    
        if ((buddyUeIndex < MAX_UE_SUPPORTED) && (PNULL != ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p))
        {
            ulBuddyUEContext_p = ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p;
        }  

        /* UL_MU_MIMO_CHG Start */

        /* ***************************************************************
         * Resource alloaction required for paired UE ;
         * Here allocation would be only for one of the UE from buddy pair
         * with consideration of both UEs RB requirement 
         * *************************************************************** */
        if ( (PNULL != ulBuddyUEContext_p ) && 
                (!ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated) )
        {
            /* Coverity 55292*/    
            retValue = allocateFrequencySelectiveUplinkResourcesULMUMIMO(
                    freeRBs,
                    pdcchULNewUE_p,
                    tempDirectIndexingUEInfo_p->requiredRB,
                    subFrameNumber,
                    ulUEContext_p,
                    bufferRBs_p,
                    resourceReserved_p,
                    &allocatedMCS,
                    hiIndex,  
                    currentTTI,
                    tpc,
                    ueMaxPowerFlag_p,
                    ueMinPowerFlag_p,
                    tempDirectIndexingUEInfo_p,
                    subFrameNumber,
                    ulDelay,
                    internalCellIndex);
        }
        /* ****************************************************
         * Resources are already allocated to UE's Buddy;
         * No allocation required  here;
         * Only update resourceReserved_p from its Buddy UE 
         * **************************************************** */
        /* Coverity 55292*/    
        else if ( (PNULL != ulBuddyUEContext_p ) && 
                (ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated) )
        {
            /*SPR_9204_START*/
            BuddyPairInfo* buddyPairInfo_p = buddyPairInfo_gp[internalCellIndex] + buddyUeIndex;
            /* Coverity 55292*/    
            retValue = updateResourcesFromBuddyUE(
                    tempDirectIndexingUEInfo_p->harqIndex,
                    ulUEContext_p,
                    &allocatedMCS,
                    resourceReserved_p,
                    buddyUeIndex,
                    buddyPairInfo_p->buddyAvgMCS,
                    internalCellIndex);
            /*SPR_9204_END*/
            /* Updated UE Context that PDSCH is allocated to UE */
            ulUEContext_p->ulMUMIMOInfo.isPuschAllocated = TRUE;


            *ueMaxPowerFlag_p = buddyPairInfo_p->buddyMaxPowerFlag;
            *ueMinPowerFlag_p = buddyPairInfo_p->buddyMinPowerFlag;
#ifdef SSI_DEBUG_STATS        
            /* CA Stats Changes Start */
            LTE_MAC_UPDATE_TOTAL_BUDDY_PAIR_SUCCESS(internalCellIndex);
#endif

            LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                    ulUEContext_p->ueIndex,
                    getCurrentTick(),
                    NEW_TX,
                    tempDirectIndexingUEInfo_p->harqIndex,
                    resourceReserved_p->allocatedLength,
                    resourceReserved_p->allocatedIndex,
                    tempDirectIndexingUEInfo_p->ulGrantRequestType,
                    determineUlTBSize(allocatedMCS,\
                        resourceReserved_p->allocatedLength),
                    TRUE,
                    allocatedMCS,
                    0,
                    tpc.tpcCommand 
                    ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                    buddyUeIndex,
                    ulUEContext_p->userLocationType,
                    ulUEContext_p->ulresInfo[tempDirectIndexingUEInfo_p->harqIndex].nDmrs,
                    ulUEContext_p->modulationScheme,
                    internalCellIndex
                    );
            /* CA Stats Changes End */
        }

        /* ********************************************************
         * Single UE Resorces allocation is required;
         * ******************************************************** */
        else
        {
            retValue = allocateFrequencySelectiveUplinkResources(
                    freeRBs,
                    pdcchULNewUE_p,
                    tempDirectIndexingUEInfo_p->requiredRB,
                    tempDirectIndexingUEInfo_p->strictRBAllocation,
                    subFrameNumber,
                    ulUEContext_p,
                    /* SPR 5295 changes start*/
                    bufferRBs_p,
                    /* SPR 5295 changes end*/
                    resourceReserved_p,
                    &allocatedMCS,
                    currentTTI,
                    /* Time Averaging changes start */
                    tpc,
                    ueMaxPowerFlag_p,
                    ueMinPowerFlag_p
                    /* Time Averaging changes end */
                    /* SPR 2446 Fix Start*/
#ifdef SSI_DEBUG_STATS
                    ,tempDirectIndexingUEInfo_p->harqIndex,
                    tempDirectIndexingUEInfo_p->ulGrantRequestType
#endif
                        /* +DYNAMIC_ICIC */
                        ,regionPowerMask
                        /* -DYNAMIC_ICIC */
#ifdef FDD_CONFIG
                        ,ulDelay
#endif
                        ,internalCellIndex
                        );
            /* SPR 2446 Fix End*/
        }
    }
    else
    {
        /* SPS Changes start */
        if ( (UL_SPS_ACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                (UL_SPS_DEACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                (UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType) 
#ifdef FDD_CONFIG
                || (TTIB_UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType) 
#endif
           )
        {
            if (FALSE == spsAllocFlag)
                return MAC_SUCCESS;
        }


        /* SPS Changes end */

        /* UL_MU_MIMO_CHG Start */
        retValue = allocateNonFrequencySelectiveUplinkResources(
                ulUEContext_p,
                pdcchULNewUE_p,
                tempDirectIndexingUEInfo_p ,
                resourceReserved_p,
                subFrameNumber,
                freeRBs,
                bufferRBs_p, 
                tpc,
                ueMaxPowerFlag_p,
                ueMinPowerFlag_p,
                &allocatedMCS,
                hiIndex,
                currentGlobalTick,
                ulDelay,
                internalCellIndex);
        /* UL_MU_MIMO_CHG Changes End */

    }


    /* 
       we look for allocating resources for tick when it is being sent 
       */
    if (MAC_SUCCESS == retValue)
    {
        /* ICIC changes start */
        LOG_MAC_MSG(MAC_RIV_RES_INFO,LOGDEBUG,MAC_PUSCH,\
                getCurrentTick(),\
                ulUEContext_p->ueIndex,
                subFrameNumber,\
                resourceReserved_p->allocatedLength,
                resourceReserved_p->allocatedIndex,\
                resourceReserved_p->allocatedRiv,\
                0.0,0.0,\
                __func__,"RESOURCE_ALLOCATION");

        /* ICIC changes end */
#ifdef SSI_DEBUG_STATS
        /* CA Stats Changes Start */
        gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
            qoSUELCStat[ulUEContext_p->ueIndex].grant[gMacStats.pStats->\
            writeTTIIndex[internalCellIndex] ] = determineUlTBSize(allocatedMCS,
                    resourceReserved_p->allocatedLength);
        /* CA Stats Changes End */

#endif

        tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[tempDirectIndexingUEInfo_p->harqIndex]);
        /*+-SPR 17669*/

        /* + CQI_5.0 */
        /* + SPR_4434 */ 
        tempULHarqInfo_p->freqEnabledFlag = 0;
        tempULHarqInfo_p->freqHoppingBits = 0;
        /* SPR 883 Fix Start */
        /*UL_MU_MIMO_CHG Start*/
        if(tempDirectIndexingUEInfo_p->buddyUeIndex < MAX_UE_SUPPORTED)
        {
            tempULHarqInfo_p->ulTxMode = MAC_UL_TX_MODE_MIMO;
        }
        else
        {
            tempULHarqInfo_p->ulTxMode = MAC_UL_TX_MODE_SISO;
        }
        /*UL_MU_MIMO_CHG End*/
        /* SPR 883 Fix End */
        /* - SPR_4434 */ 
        if ( (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG != tempDirectIndexingUEInfo_p->ulGrantRequestType) &&
                /* SPS changes */ 
                (UL_SPS_DEACTIVATION != tempDirectIndexingUEInfo_p->ulGrantRequestType) )
            /* SPS changes */ 
        {    

            /* + Coverity 31422 */
            if ( resourceReserved_p->allocatedLength > 0 )
            {
                /*  SPR 18258 fix start*/
                /* MCS = 6 and nPRB = 1 is used specifically for tti bundling, hence if tti  bundling is disbaled do not use mcs = 6 and nPRB = 1.
                   isTTIBundled is not updated yet in harq context so using the condition below.  
                   Also if it is SPS occasion, by decreasing MCS, tb size will also decrease significantly and hence the UL throughput will be decreased.
                   to optimize UL throughput in case of SPS, This if check should be placed where ul num rbs are calculated.
                   */
                if ((6 == allocatedMCS) && (1 == resourceReserved_p->allocatedLength) &&
                        (FALSE == ulUEContext_p->ttiBundlingEnabled))
                {
                    allocatedMCS--;
                    tempAllocTBSize =  determineUlTBSize(allocatedMCS,
                            resourceReserved_p->allocatedLength);
                }
                else
                {
                    tempAllocTBSize =  determineUlTBSize(allocatedMCS,
                            resourceReserved_p->allocatedLength);
                }
                /* SPR 18258 fix end*/
            }
            /* - Coverity 31422 */

            if (ulUEContext_p->bsrNetTotal < tempAllocTBSize)
            {
                tempAllocTBSize = ulUEContext_p->bsrNetTotal;
            } 

            ulUEContext_p->bsrNetTotal -= tempAllocTBSize;
            /*Adjust Qload changes start*/
            if (UL_QOS_SCHEDULING != cellSpecificParams_g.\
                    cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig.ulSchedulerStrategy)
            {
                /* BSR CHG */
                /* SPR 10027 FIX START */
                /* SPR 11864 Fix */
                /* +- SPR 17777 */
                updateBsrNetOfLCG( ulUEContext_p, tempAllocTBSize,tempULHarqInfo_p
#ifdef KPI_STATS
                        ,internalCellIndex
#endif
                        );        
                /* +- SPR 17777 */
                /* SPR 11864 Fix */ 
                /* SPR 10027 FIX END */
            }
            /*Adjust Qload changes end*/
            allocateULHarqProcess(tempULHarqInfo_p,
                    /* +- SPR 17777 */
                    resourceReserved_p);

            /* TTIB_Code Start */
            if((ulUEContext_p->ttiBundlingEnabled) &&(
                        !(UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType)))
            {
                tempULHarqInfo_p->isTTIBundled = TRUE;
                tempULHarqInfo_p->isTTIDataRcvd = FALSE;
                tempULHarqInfo_p->ttiBundleFirstTick = currentGlobalTick + ulDelay + FDD_HARQ_OFFSET;

                if ( (UL_SPS_ACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                        (UL_SPS_DEACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                        (TTIB_UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType)) 
                {
                    tempULHarqInfo_p->isSpsEnabled = TRUE;
                }
                LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH,
                        "[%s] [%d] HARQ ID %d isTTIBundled %d  isTTIDataRcvd %d ttiBundleFirstTick %d \n",__func__,
                        getCurrentTick(),tempDirectIndexingUEInfo_p->harqIndex,
                        tempULHarqInfo_p->isTTIBundled,tempULHarqInfo_p->isTTIDataRcvd,
                        tempULHarqInfo_p->ttiBundleFirstTick);
            }
            updateTTIHarqMap(tempDirectIndexingUEInfo_p->harqIndex,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
            /* TTIB_Code End */
        }
        /* + SPR_4434 */
        else if (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG == 
                tempDirectIndexingUEInfo_p->ulGrantRequestType)
        {
            /* SPR 21958 PUSCH RAT1 Support Start */ 
            if(TRUE == resourceReserved_p->RAT1Allocation)
            {
                tempULHarqInfo_p->startRbgIndexforSet1 = resourceReserved_p->startRbgIndexforSet1;
                tempULHarqInfo_p->endRbgIndexforSet1 = resourceReserved_p->endRbgIndexforSet1;
                tempULHarqInfo_p->startRbgIndexforSet2 = resourceReserved_p->startRbgIndexforSet2;
                tempULHarqInfo_p->endRbgIndexforSet2 = resourceReserved_p->endRbgIndexforSet2;
                tempULHarqInfo_p->RAT1Allocation = TRUE;
            }
            else
            {
                tempULHarqInfo_p->riStart = 
                    resourceReserved_p->allocatedIndex;
                tempULHarqInfo_p->riLength = 
                    resourceReserved_p->allocatedLength;
                tempULHarqInfo_p->RAT1Allocation = FALSE;
            }
            /* SPR 21958 PUSCH RAT1 Support End */
            /*  SPR 19031 fix start */
            if(ulUEContext_p->ttiBundlingEnabled)
            {
                ulUEContext_p->aperiodicCQIWithoutDataTTI = currentGlobalTick;
            }
            /*  SPR 19031 fix end */
        }

        /* - SPR_4434 */
        /* - CQI_5.0 */
        tempULHarqInfo_p->mcsIndex = allocatedMCS;        

        /* + SPR 16758 */       
        if ( (TRUE == spsAllocFlag) && ((tempDirectIndexingUEInfo_p->ulGrantRequestType == UL_SPS_ACTIVATION) ||
                    (tempDirectIndexingUEInfo_p->ulGrantRequestType == UL_SPS_DEACTIVATION) ||
                    (tempDirectIndexingUEInfo_p->ulGrantRequestType == UL_SPS_OCCASION)||
                    (TTIB_UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType)))
        {
            tempULHarqInfo_p->newDataIndicator = 0;
        }
        /* - SPR 16758 */

#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT)
        tempULHarqInfo_p->lastScheduledTTI = getCurrentTick();
#endif
        LOG_UT(MAC_ALLOCATE_ULHARQ_UEINFO_ID,LOGDEBUG,MAC_UL_HARQ,\
                currentTTI,ulUEContext_p->ueIndex,
                tempDirectIndexingUEInfo_p->harqIndex,\
                tempAllocTBSize,\
                resourceReserved_p->allocatedIndex,\
                resourceReserved_p->allocatedRiv,\
                (UDouble32)resourceReserved_p->allocatedLength,\
                (UDouble32)tempULHarqInfo_p->strictRBAllocation,\
                __func__,"ALLOCATE_ULHARQ");
    }
    else
    {
        /* ICIC changes start */
        /* When the allocation fails the rbs reserved by strategy for this UE is 
         * added to the buffer pool, so that next UE can get rbs in the buffer pool
         * gone waste */
        *bufferRBs_p += 
            tempDirectIndexingUEInfo_p->requiredRB;
        /* ICIC changes end */
        /* CA Stats Changes Start */
        LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                ulUEContext_p->ueIndex,
                currentTTI,
                INVALID_TX,
                tempDirectIndexingUEInfo_p->harqIndex,
                0,
                0,
                tempDirectIndexingUEInfo_p->ulGrantRequestType,
                0,
                FALSE,
                0,
                1, /*Allocation failed*/
                tpc.tpcCommand
                /* UL_MU_MIMO_CHG START */
                ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                INVALID_UE_INDEX,
                ulUEContext_p->userLocationType,
                MAX_DMRS_VALUE,
                ulUEContext_p->modulationScheme,
                /* UL_MU_MIMO_CHG END */
                internalCellIndex
                    );
        /* CA Stats Changes End */

        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}

#elif TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129107) */
/*********************************************************************************************
 * Function Name :      allocatePUSCHResourcesForNewUEs
 * Inputs        :      ulUEContext_p - The pointer to the ULUEContext,
 *                      pdcchULNewUE_p - The pointer to PdcchULNewUE,
 *                      ueScheduledInfo_p - The pointer to the UeScheduledInfo,
 *                      resourceReserved_p - The pointer to the AllocatedResInfo,
 *                      subFrameNumber - The subFrame number when it is sent,
 *                      freeRBs,
 *                      bufferRBs_p,
 *                      tpc - Triggered To Power Correction,
 *                      ueMaxPowerFlag_p,
 *                      ueMinPowerFlag_p,
 *                      hiIndex,
 *                      currentGlobalTick - Current Tick
 *                      internalCellIndex - Cell-Index at MAC
 *                      ulSchTTI -
 * Outputs              None
 * Returns       :      MAC_SUCCESS /MAC_FAILURE
 * Description   :      This function will work on per node basis and try to allocate.
 **********************************************************************************************/
                             MacRetType allocatePUSCHResourcesForNewUEs(
                                     /* SPR 5599 changes end (ZIP ID 129107) */
                                     ULUEContext* ulUEContext_p,
                                     PdcchULNewUE *pdcchULNewUE_p,
                                     UeScheduledInfo *ueScheduledInfo_p,
                                     AllocatedResInfo* resourceReserved_p,
                                     /*TDD Config 0 Changes Start*/
                                     UInt8 rbMapNumber,
                                     /*TDD Config 0 Changes End*/
                                     UInt32 freeRBs,
                                     SInt8 *bufferRBs_p, 
                                     /* Time Averaging changes start */
                                     TpcTriggeredToPowerCorrection tpc,
                                     UInt32 *ueMaxPowerFlag_p,
                                     UInt32 *ueMinPowerFlag_p,
                                     /* Time Averaging changes end */
                                     /* + SPS_TDD_Changes */
                                     UInt8 spsAllocFlag,
                                     /* - SPS_TDD_Changes */
                                     /* UL_MU_MIMO_CHG Start */
                                     UInt8 hiIndex,
                                     /* SPR 15909 fix start */
                                     tickType_t currentGlobalTick,
                                     /* UL_MU_MIMO_CHG End */
                                     InternalCellIndex internalCellIndex
                                     /* SPR 11331 Fix Start */
                                     ,tickType_t ulSchTTI
                                     /* SPR 15909 fix end */
                                     /* SPR 11331 Fix End */
                                     /*TDD Config 0 Changes Start*/ 
                                     ,UInt8 ulSubFrameNum
                                     /*TDD Config 0 Changes End*/
                                     ,ExecutionLegFunctionArg* execArgs
                                     )

{
    ULHarqInfo* tempULHarqInfo_p = PNULL;
    UInt32 tempAllocTBSize = 0;
    UInt32 retValue = MAC_FAILURE;
    /* SPR 15909 fix start */
    tickType_t currentTTI = execArgs->globalTick;
    /* SPR 15909 fix end */
    UInt8  allocatedMCS = pdcchULNewUE_p->avgMCS;
    UInt16 sysFrameNum = (currentGlobalTick / MAX_SUB_FRAME) % MAX_SFN_VALUE;
    UInt8  phichSubFrameNum = 0;
    UInt8 iPhich = 0;
    ULHarqInfo* ulHarqInfoForPreviousUlSubframe_p = PNULL;
    /*TDD Config 0 Changes End*/

    /* UL_MU_MIMO_CHG Start */
    UInt16        buddyUeIndex       = ueScheduledInfo_p->buddyUeIndex;
    ULUEContext*  ulBuddyUEContext_p = PNULL;
    if ((buddyUeIndex < MAX_UE_SUPPORTED) && (PNULL != ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p))
    {
        ulBuddyUEContext_p = ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p;
    }
    /* UL_MU_MIMO_CHG End */



    /* +DYNAMIC_ICIC */
    /*CA Changes start  */
    ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + ulPowerMaskIndex_g[internalCellIndex];
    /*CA Changes end  */
    SInt32 regionPowerMask = ulPowerMaskInfo_p->powerMask[ulUEContext_p->userLocationType];
    /* -DYNAMIC_ICIC */
    /* we look for allocating resources for tick when it is being sent */

    LTE_MAC_UT_LOG(LOG_CRITICAL, LOG_PUSCH,
            "[%s] Entry ulCCEIndex[%d] hpID [%d] subframe[%d].\n",
            __func__, ueScheduledInfo_p->ulCCEIndex, ueScheduledInfo_p->ulHarqProcessId,
            rbMapNumber
            );
    /* +DYNAMIC_ICIC */    
    CHECK_QCI_POWER_MASK_AND_UPDATE_REGION_MASK(ulUEContext_p,regionPowerMask);
    /* -DYNAMIC_ICIC */
    /* SPR 8908 FIX START */

    /*TDD Config 0 Changes Start*/
    /* Calculate the phichframe corresponding to the given U frame*/
    phichSubFrameNum = ulSubFrameNum + getHiPduSubframeNum(ulSubFrameNum,internalCellIndex);
    phichSubFrameNum = phichSubFrameNum % MAX_SUB_FRAME;
    iPhich = calculateIPhichValue(ulSubFrameNum,internalCellIndex);
    /*TDD Config 0 Changes End*/ 

#ifdef OVERSHOOT_CORRECTION
    if ( (FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->freqSelectiveSchUL) &&
            (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG !=
             ueScheduledInfo_p->ulGrantRequestType) &&
            /** CA-TDD Changes Start **/
            (OVERLOAD_LEVEL_NORMAL == macOverLoadLevel_g[internalCellIndex] ) )
        /** CA-TDD Changes Stop **/
#else
        if ( (FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->freqSelectiveSchUL) &&
                (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG !=
                 ueScheduledInfo_p->ulGrantRequestType) )
            /* - CQI_5.0 */
#endif
            /* SPR 8908 FIX END */
            /*TDD Tick overshoot changes end*/
        {
            /* UL_MU_MIMO_CHG Start */
            /****************************************************************
             * Resource alloaction required for paired UE ;
             * Here allocation would be only for one of the UE from buddy pair
             * with consideration of both UEs RB requirement
             *************************************************************** */

            if ( (buddyUeIndex < MAX_UE_SUPPORTED) &&
                    (PNULL != ulBuddyUEContext_p) &&
                    (!ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated) )
            {
                retValue = allocateFrequencySelectiveUplinkResourcesULMUMIMO(
                        freeRBs,
                        pdcchULNewUE_p,
                        ueScheduledInfo_p->requiredRB,
                        rbMapNumber,
                        ulUEContext_p,
                        bufferRBs_p,
                        resourceReserved_p,
                        &allocatedMCS,
                        hiIndex,
                        /*TDD Config 0 Changes Start*/
                        currentGlobalTick,
                        /*TDD Config 0 Changes End*/
                        tpc,
                        ueMaxPowerFlag_p,
                        ueMinPowerFlag_p,
                        ueScheduledInfo_p,
                        /* +- SPR 17777 */
                        internalCellIndex,
                        /*TDD Config 0 Changes Start*/ 
                        ulSubFrameNum,
                        phichSubFrameNum
                            /*TDD Config 0 Changes End*/
                            );
            }

            /*****************************************************
             * Resources are already allocated to UE's Buddy;
             * No allocation required  here;
             * Only update resourceReserved_p from its Buddy UE
             **************************************************** */
            else if ( (buddyUeIndex < MAX_UE_SUPPORTED) &&
                    (PNULL != ulBuddyUEContext_p) &&
                    (ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated) )
            {
                ULUEContext *buddyUlUEContext_p = ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p;
                /*TDD Config 0 Changes Start*/
                /*  UeScheduledInfo is now U based*/
                /** SPR 14204 Fix : Start **/
                UeScheduledInfo *buddyUeScheduledInfo_p = 
                    buddyUlUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulSubFrameNum];
                /** SPR 14204 Fix : End **/
                /*TDD Config 0 Changes End*/ 
                /*SPR_9204_START*/
                BuddyPairInfo* buddyPairInfo_p = buddyPairInfo_gp[internalCellIndex] + buddyUeIndex;

                retValue = updateResourcesFromBuddyUE(
                        ueScheduledInfo_p->ulHarqProcessId,
                        buddyUeScheduledInfo_p->ulHarqProcessId,
                        ulUEContext_p,
                        &allocatedMCS,
                        resourceReserved_p,
                        buddyUeIndex,
                        buddyPairInfo_p->buddyAvgMCS,
                        /*TDD Config 0 Changes Start*/
                        iPhich,
                        internalCellIndex);
                /*TDD Config 0 Changes End*/
                /*SPR_9204_END*/
                /* Updated UE Context that PDSCH is allocated to UE */
                ulUEContext_p->ulMUMIMOInfo.isPuschAllocated = TRUE;

#ifdef SSI_DEBUG_STATS
                /* CA Stats Changes Start */
                LTE_MAC_UPDATE_TOTAL_BUDDY_PAIR_SUCCESS(internalCellIndex);
#endif

                LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                        ulUEContext_p->ueIndex,
                        getCurrentTick(),
                        NEW_TX,
                        ueScheduledInfo_p->ulHarqProcessId,
                        resourceReserved_p->allocatedLength,
                        resourceReserved_p->allocatedIndex,
                        ueScheduledInfo_p->ulGrantRequestType,
                        determineUlTBSize(allocatedMCS,\
                            resourceReserved_p->allocatedLength),
                        TRUE,
                        allocatedMCS,
                        0,
                        tpc.tpcCommand
#ifdef MCS_PRB_MAP_DEBUG
                        ,ulUEContext_p->csiContext.mcsToPrbMap_p
#endif
                        ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                        buddyUeIndex,
                        ulUEContext_p->userLocationType,
                        ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId].nDmrs,
                        ulUEContext_p->modulationScheme,
                        internalCellIndex
                            );
                /* CA Stats Changes End */
            }

            /*********************************************************
             * Single UE Resorces allocation is required;
             ********************************************************* */
            else
            {
                /*TDD Config 0 Changes Start*/
                /* Calculate the iPhich value for the corresponding UL Tx*/
                UInt8 nDmrs = MAX_DMRS_VALUE;
                tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId]);

                if (COMMON_DCI_COUNT == pdcchULNewUE_p->scheduleCount)
                {
                    ulHarqInfoForPreviousUlSubframe_p = getPrevHarqInfoForTddConfig0(hiIndex,
                            sysFrameNum,
                            ulUEContext_p,
                            internalCellIndex
                            );
                    /* Incase of PUSCH failure for the +k U frame, when we were not able to 
                     * allocate resources, we will change the scheduled count of the UE in +7
                     * U frame to 1, and try to allocate resources in the +7 frame
                     */
                    if (!ulHarqInfoForPreviousUlSubframe_p ||
                            !ulHarqInfoForPreviousUlSubframe_p->riLength)
                    {
                        pdcchULNewUE_p->scheduleCount = COMMON_DCI_PUSCH_FAILURE;
                    }
                }
                /* This condition will be hit while processing +7 U frame and
                 * when a common DCI0 will go for both +k and +7 U frames*/
                if (COMMON_DCI_COUNT == pdcchULNewUE_p->scheduleCount)
                {
                    retValue = allocateFrequencySelectiveUplinkResourcesForTddConfig0(
                            ulHarqInfoForPreviousUlSubframe_p,
                            pdcchULNewUE_p,
                            /* +- SPR 17777 */
#ifdef SSI_DEBUG_STATS
                            currentGlobalTick,
                            tpc,
#endif
                            /* +- SPR 17777 */
                            ulUEContext_p,
                            ueScheduledInfo_p,
                            bufferRBs_p,
                            &allocatedMCS,
                            rbMapNumber,
                            resourceReserved_p,
                            freeRBs,
                            phichSubFrameNum,
                            /* +- SPR 17777 */
                            iPhich,
                            internalCellIndex
                            );
                    nDmrs = ulHarqInfoForPreviousUlSubframe_p->nDmrs;
                }
                else
                {
                    /*TDD Config 0 Changes End*/ 
                    retValue = allocateFrequencySelectiveUplinkResources(
                            freeRBs,
                            pdcchULNewUE_p,
                            ueScheduledInfo_p->requiredRB,
                            ueScheduledInfo_p->strictRBAllocation,
                            rbMapNumber,
                            ulUEContext_p,
                            bufferRBs_p,
                            resourceReserved_p,
                            &allocatedMCS,
                            currentTTI,
                            /* Time Averaging changes start */
                            tpc,
                            ueMaxPowerFlag_p,ueMinPowerFlag_p
                            /* Time Averaging changes end */
                            /* SPR 2446 Fix Start*/
#ifdef SSI_DEBUG_STATS
                            ,ueScheduledInfo_p->ulHarqProcessId,
                            ueScheduledInfo_p->ulGrantRequestType
#endif
                            /* SPR 2446 Fix End*/
                                /* +DYNAMIC_ICIC */
                                ,regionPowerMask
                                /* -DYNAMIC_ICIC */
                                ,internalCellIndex
                                /*TDD Config 0 Changes Start*/
                                ,&nDmrs
                                ,phichSubFrameNum
                                ,iPhich
                                );
                }
                /*Reserve the phich resources in case of successful resource
                 * allocation.Also update the HarqInfo*/
                if (MAC_SUCCESS == retValue)
                {
                    reserveHiAndUpdateHarqInfo(iPhich,
                            resourceReserved_p->allocatedIndex,
                            nDmrs,
                            phichSubFrameNum,
                            tempULHarqInfo_p,
                            internalCellIndex);
                }
                /*TDD Config 0 Changes End*/
            }
        }
        else
        {
            /* + SPS_TDD_Changes */ 
            if ( (UL_SPS_ACTIVATION == ueScheduledInfo_p->ulGrantRequestType) ||
                    (UL_SPS_DEACTIVATION == ueScheduledInfo_p->ulGrantRequestType) ||
                    (UL_SPS_OCCASION == ueScheduledInfo_p->ulGrantRequestType))
            {
                if (FALSE == spsAllocFlag)
                    return MAC_SUCCESS;
            }
            /* - SPS_TDD_Changes */ 
            retValue = allocateNonFrequencySelectiveUplinkResources(
                    ulUEContext_p,
                    pdcchULNewUE_p,
                    ueScheduledInfo_p,
                    resourceReserved_p,
                    rbMapNumber,
                    freeRBs,
                    bufferRBs_p,
                    tpc,
                    ueMaxPowerFlag_p,
                    ueMinPowerFlag_p,
                    &allocatedMCS,
                    hiIndex,
                    currentGlobalTick,
                    internalCellIndex,
                    /*TDD Config 0 Changes Start*/ 
                    ulSubFrameNum,
                    phichSubFrameNum
                    /*TDD Config 0 Changes End*/ 
                    );
        }
    /*UL_MU_MIMO_CHG End*/

    if( MAC_SUCCESS == retValue )
    {
        /* ICIC changes start */
        LOG_MAC_MSG(MAC_RIV_RES_INFO,LOGBRIEF,MAC_PUSCH,\
                getCurrentTick(),\
                ulUEContext_p->ueIndex,
                rbMapNumber,\
                resourceReserved_p->allocatedLength,
                resourceReserved_p->allocatedIndex,\
                resourceReserved_p->allocatedRiv,\
                0.0,0.0,\
                __func__,"RESOURCE_ALLOCATION");
        /* ICIC changes end */

        tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId]);

        /*+-SPR 17669*/
        /* TDD SB_CQI */

        tempULHarqInfo_p->freqEnabledFlag = 0;
        tempULHarqInfo_p->freqHoppingBits = 0;

        /* SPR 883 Fix Start */
        /*UL_MU_MIMO_CHG Start*/
        if(ueScheduledInfo_p->buddyUeIndex < MAX_UE_SUPPORTED)
        {
            tempULHarqInfo_p->ulTxMode = MAC_UL_TX_MODE_MIMO;
        }
        else
        {
            tempULHarqInfo_p->ulTxMode = MAC_UL_TX_MODE_SISO;
        }
        /*UL_MU_MIMO_CHG End*/
        /* SPR 883 Fix End */

        /* TDD SB_CQI */
        if (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG != ueScheduledInfo_p->ulGrantRequestType)
            /* TDD SB_CQI */
        {
            /*SPR 21501 fix start*/
            /* MCS = 6 and nPRB = 1 is used specifically for tti bundling,
               hence if tti  bundling is disabled do not use mcs = 6 and nPRB = 1.
               */
            if ((6 == allocatedMCS) && (1 == resourceReserved_p->allocatedLength) &&
                    (FALSE == ulUEContext_p->ttiBundlingEnabled))
            {
                allocatedMCS--;
            }
            /*SPR 21501 fix end*/
            tempAllocTBSize =  determineUlTBSize(allocatedMCS,
                    resourceReserved_p->allocatedLength);

            if (ulUEContext_p->bsrNetTotal < tempAllocTBSize)
            {
                tempAllocTBSize = ulUEContext_p->bsrNetTotal; 
            }

            ulUEContext_p->bsrNetTotal -= tempAllocTBSize;

            /*Adjust Qload changes start*/
            if (UL_QOS_SCHEDULING != cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig.ulSchedulerStrategy)
            {
                /* BSR CHG */
                /* SPR 10027 FIX START */
                /* SPR 11864 Fix */
                /* +- SPR 17777 */
                updateBsrNetOfLCG( ulUEContext_p, tempAllocTBSize,tempULHarqInfo_p
#ifdef KPI_STATS
                        ,internalCellIndex
#endif
                        );        
                /* +- SPR 17777 */
                /* SPR 11864 Fix */
                /* SPR 10027 FIX END */
            }
            /*Adjust Qload changes end*/

            allocateULHarqProcess(tempULHarqInfo_p,
                    /* +- SPR 17777 */
                    resourceReserved_p,
                    /* SPR 11331 Fix Start */
                    ulSchTTI
                    /* SPR 11331 Fix End */
                    );

            /*TDD Tick overshoot changes start*/
            /* SPR 11115 Fix Start */
            /*
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT_TDD)
tempULHarqInfo_p->ulSchTTI = rbMapNumber;
#endif
*/
            /* SPR 11115 Fix End */
            /*TDD Tick overshoot changes end*/
            LTE_MAC_UT_LOG(LOG_CRITICAL, LOG_PUSCH,\
                    "[%s] UL HARQ ID ALLOCATED = %d", __func__,
                    ueScheduledInfo_p->ulHarqProcessId);
        }
        /* TDD SB_CQI */
        else if (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG ==
                ueScheduledInfo_p->ulGrantRequestType)
        {
            tempULHarqInfo_p->riStart =
                resourceReserved_p->allocatedIndex;
            tempULHarqInfo_p->riLength =
                resourceReserved_p->allocatedLength;
        }

        tempULHarqInfo_p->mcsIndex = allocatedMCS;
        /* TDD SB_CQI */
    }
    else
    {
        /* ICIC changes start */
        /* When the allocation fails the rbs reserved by strategy for this UE is 
         * added to the buffer pool, so that next UE can get rbs in the buffer pool
         * gone waste */
        *bufferRBs_p += 
            ueScheduledInfo_p->requiredRB;
        /* ICIC changes end */
        /* CA Stats Changes Start */
        LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                ulUEContext_p->ueIndex,
                currentTTI,
                INVALID_TX,
                ueScheduledInfo_p->ulHarqProcessId,
                0,
                0,
                ueScheduledInfo_p->ulGrantRequestType,
                0,
                FALSE,
                0,
                1 /*Allocation failed*/,
                tpc.tpcCommand
                /* UL_MU_MIMO_CHG START */
                ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                INVALID_UE_INDEX,
                ulUEContext_p->userLocationType,
                MAX_DMRS_VALUE,
                ulUEContext_p->modulationScheme,
                /* UL_MU_MIMO_CHG END */
                internalCellIndex
                    );        
        /* CA Stats Changes End */
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
#endif

/********************************************************************************
 * Function Name :    processULNewUeInPUSCH
 * Inputs:            ulDelay - The delay in the uplink path,
 *                    containerTick,
 *                    subFrameNumWithoutAdvance - Subframe no. without advance,
 *                    currentGlobalTick - The present Global Tick
 *                    numberOfULUEsToBeScheudled_p - No. of UE to be Scheduled,
 *                    hiIndex,
 *                    bufferRBs_p,
 *                    locType                   
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs:           None
 *                    None
 * Description:       This function will read the Nack queue and update the HARQ process, 
 *                    and book the resources in advance. It will also create the HI PDUs. 
 ********************************************************************************/
#ifdef FDD_CONFIG
            /* SPR 5599 changes start (ZIP ID 129107) */
            void processULNewUeInPUSCH(
                    /* SPR 5599 changes end (ZIP ID 129107) */
                    UInt8 ulDelay,
                    UInt8 containerTick,
                    /* +- SPR 17777 */
                    /* SPR 15909 fix start */
                    tickType_t currentGlobalTick,
                    /* SPR 15909 fix end */
                    UInt8* numberOfULUEsToBeScheudled_p,
                    /* CR changes start */
                    UInt8 hiIndex,
                    /* CR changes end */
                    SInt8 *bufferRBs_p,
                    UInt8 locType,
                    /*CA Changes start  */
                    InternalCellIndex internalCellIndex,
                    /*CA Changes end  */
                    ExecutionLegFunctionArg* execArgs)
{
#ifdef HD_FDD_CONFIG
    tickType_t localTick = 0;
#endif
    UInt16 tempUeId=0;
    ULUEContext* ulUEContext_p=PNULL;
    UInt8 tempHarqProcessId = INVALID_HARQ_ID;
    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p = PNULL;
    DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    UInt8 rbMapNum = 255; /* Invalid Value */
    UInt16 numberOfInfoPresent = 0;
    ULSchUciRachPoolInfo* ulschUciRachInfo_p 
        = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;

    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick + ulDelay;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    tickType_t            enqueueTick        = 0;

    UInt32 localCount = 0;
    ULGrantInfo ulGrantInfo = {0};
    UInt32 tempPUSCHResources = 0;
    UInt32 loopCount = 0;
    PdcchULNewUE *pdcchULNewUE_p = PNULL;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    UInt8 subFrameNum  = (currentGlobalTick + FDD_HARQ_OFFSET + ulDelay )% MAX_SUB_FRAME; /* SS_S2 */

    rbMapNum = MODULO_SIXTEEN(currentGlobalTick + ulDelay + FDD_HARQ_OFFSET); /* SS_S2 */

    PdcchULOutputInfo* tempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];
    UInt32 ueMinPowerFlag = 0;
    UInt32 ueMaxPowerFlag = 0;

    /*
       Since the harq process ID will remain same irrespetive of the UE for a particular 
       tick, hence we derive the same here and place it into a local variable
       */
    tempHarqProcessId = MODULO_EIGHT(currentGlobalTick + ulDelay + FDD_HARQ_OFFSET);

    /*
       Among the newly scheduled UE we first consider those
       for which there has been collision and for whom the CCE
       has been allocated by PDCCH  
       */


    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum; 
    /* loop for 2 times */
    tempPUSCHResources = 
        rbMapNode_p->availableResourceInfo[locType].freeRBCounter;
    localCount = 0;
    /*
       We now consider the case of NEW UEs that has been scheduled by the Strategy this time 
       but no collision is reported and for whom the CCE has been allocated by PDCCH
       */
    loopCount = tempPdcchULOutput_p->pdcchULNewUEInfo[locType].count; 
    UInt8 callSemiStaticforAperiodicFlag = FALSE;

    UInt16       buddyUeIndex       = INVALID_UE_INDEX;
    ULUEContext* ulBuddyUEContext_p = PNULL;

    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, \
            "[%s] #####  No. of NewUE = %d  "
            "And No. of UL UE to be scheduled = %d loopCount = %d \n"
            ,__func__,loopCount, *numberOfULUEsToBeScheudled_p,loopCount);

    while((loopCount--))          
    {

        /* ATB Changes 5.1 Start */
        ueMinPowerFlag = 0;
        /* ATB Changes 5.1 End */

        ueMaxPowerFlag = 0;
        /* Time Averaging changes end */
        /* ICIC changes start */ 
        pdcchULNewUE_p = &(tempPdcchULOutput_p->pdcchULNewUEInfo[locType].pdcchULNewUEArray[localCount]);
        /* ICIC changes end */ 

        tempUeId = pdcchULNewUE_p->ueIndex;

        /* +COVERITY 10205, 10189 */
        if (INVALID_UE_ID == tempUeId)
        {
            ltePanic ("Invalid UE ID received in function %s", __func__);
            break;
        }
        /* -COVERITY 10205, 10189 */

        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeId];        
        tempDirectIndexingUEInfo_p = \
                                     &(tempPdcchULOutput_p->directIndexingUEInfo[tempUeId]); 

        if ( (tempDirectIndexingUEInfo_p->ulGrantRequestType == UL_SPS_OCCASION) ||
                (tempDirectIndexingUEInfo_p->ulGrantRequestType == UL_SPS_ACTIVATION)||
                (tempDirectIndexingUEInfo_p->ulGrantRequestType == UL_SPS_DEACTIVATION) 
                || (TTIB_UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType) 
           )
        {
            pdcchULNewUE_p->ueIndex = INVALID_UE_ID;
            localCount++;

            continue;
        }

        /* ICIC changes start */                             
        if((tempDirectIndexingUEInfo_p->flag != UNSET_FLAG) && 
                (tempDirectIndexingUEInfo_p->flag != MEAS_GAP_RE_TX_NO_CCE_NO_GRANT))
        {
            /* ICIC changes end */
            /*
               we call the function for allocating PUSCH resources
               for Newly Scheduled UEs without any collision
               */
            if (!ulUEContextInfo_p->pendingDeleteFlag)
            {
                ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
                /* TTIB_Code Start */
                if(ulUEContext_p->ttiBundlingEnabled)
                {
                    /*
                     * Harq id calculation will be different for TTIB UE's
                     * Max number of harq process will be 4 and calculation will be done
                     * based on bundle (consecutive 4 ticks)
                     */
                    tempHarqProcessId = GET_TTIB_HARQ_ID(currentGlobalTickWithULDelay + FDD_HARQ_OFFSET);
                    hiIndex = (hiIndex + TTIB_FIRST_TO_LAST_TICK_OFFSET) % MAX_PHICH_CONTAINER; 
                    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH,
                            "[%s] [%d] TTIB HARQ Process ID %d hiIndex %d  \n.",__func__,getCurrentTick(),tempHarqProcessId,hiIndex);             
                }
                else
                {
                    /*
                       Since the harq process ID will remain same irrespetive of the UE for a particular 
                       tick, hence we derive the same here and place it into a local variable
                       */
                    tempHarqProcessId = MODULO_EIGHT(currentGlobalTickWithULDelay + FDD_HARQ_OFFSET);
                    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH,
                            "[%s] [%d] Non TTIB HARQ Process ID %d \n.",__func__,getCurrentTick(),tempHarqProcessId);             
                }
                /* TTIB_Code End */
                ulUEContext_p->isPdcchAllocated = 0;

                /* UL_MU_MIMO_CHG Start */
                buddyUeIndex = tempDirectIndexingUEInfo_p->buddyUeIndex;
                if((buddyUeIndex < MAX_UE_SUPPORTED) && (PNULL != ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p))                    
                {   
                    ulBuddyUEContext_p = ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p;
                }
                /* UL_MU_MIMO_CHG End */

                if ((*numberOfULUEsToBeScheudled_p == 0) ||
                        ( (!tempPUSCHResources) && 
                          (( PNULL == ulBuddyUEContext_p) ||
                           (!ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated) ) ) ||
                        (ulUEContext_p->ulresInfo[tempHarqProcessId].riLength != 0))
                {
                    LOG_MAC_MSG(MAC_UL_HARQ_ALREADY_BUSY_ID, LOGINFO, MAC_UL_HARQ,\
                            currentGlobalTick, tempUeId,\
                            tempHarqProcessId,\
                            subFrameNum,\
                            0,0,0.0,0.0,__func__,"UL_HARQ_ALREADY_BUSY");
                    /* + SPR 6864 */
                    if (ULSCH_UCI_INVALID_INDEX !=
                            tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
                    {
                        checkAndRestoreUCIPduType(ulUEContext_p, tempDirectIndexingUEInfo_p, ulschUciRachInfo_p,
                                /* CSI_P2_PUSCH_CHANGES Start */
                                subFrameNum,
                                /*SPR 19504 fix start*/    
                                rbMapNum,
                                internalCellIndex
                                /*SPR 19504 fix end*/    
                                /* CSI_P2_PUSCH_CHANGES end */
                                );
                    }
                    /* - SPR 6864 */

                    if ((tempDirectIndexingUEInfo_p->ulGrantRequestType != NEW_ULGRANT_REQ_MSG)
                            &&(tempDirectIndexingUEInfo_p->ulGrantRequestType != NEW_TTIB_ULGRANT_REQ_MSG)
                       )
                    {
                        putEntryInPUSCHFailureQueue(
                                tempUeId,
                                tempDirectIndexingUEInfo_p->ttiCounter,
                                tempDirectIndexingUEInfo_p->ulGrantRequestType,
                                internalCellIndex);
                        /* CSI_P2_PUSCH_CHANGES Start */
                        ulUEContext_p->aperiodicCQIToBeRequested = FALSE;
                        /* CSI_P2_PUSCH_CHANGES End */
                    }

                    /* This can only occur when the memory allocation was not proper */
                    /* Cyclomatic Complexity changes */
                        mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].\
                                dciPduInfo.aggregationLvl,
                                (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex]),
                                internalCellIndex); 
                    tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;                       
                    tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                    localCount++;
                    /* Qos changes_v start*/
                    ulQosUpdateScheduledTokens(ulUEContext_p,0,INVALID_HARQ_ID,
                            internalCellIndex,execArgs);                    
                    /* Qos changes_v end*/
                    continue;
                }

                powerCorrectionAndTpc = getTpcForPusch(ulUEContext_p, currentGlobalTick + ulDelay, internalCellIndex);
                if ((LAST_UE == *numberOfULUEsToBeScheudled_p) &&
                        (!tempDirectIndexingUEInfo_p->strictRBAllocation)
                        &&(!ulUEContext_p->ttiBundlingEnabled)
                   )
                {
                    /* Time Averaging changes start */
                    assignRBsForMaxData(pdcchULNewUE_p, &tempDirectIndexingUEInfo_p->requiredRB,
                            tempPUSCHResources, powerCorrectionAndTpc.powerCorrectionVal ,&ueMaxPowerFlag, &ueMinPowerFlag
                            /* +DYNAMIC_ICIC */
                            ,ulUEContext_p
                            /* -DYNAMIC_ICIC */
                            ,internalCellIndex
                            );

                    ueMinPowerFlag = FALSE;
                    ueMaxPowerFlag = FALSE;
                    /* Time Averaging changes end */
                }


                /*
                   We now populate the structure that would be passed to the function 
                   processULGrants.
                   */
                            ulGrantInfo.tempUeIndex = tempUeId;
                        ulGrantInfo.ulUEContext_p = ulUEContext_p;
                        ulGrantInfo.tempHarqProcessId = tempHarqProcessId;
                        ulGrantInfo.subFrameNumber = rbMapNum;
                        ulGrantInfo.tempDciCCEInfo_p = tempDciCCEInfo_p;
                        ulGrantInfo.tempDirectIndexingUEInfo_p = tempDirectIndexingUEInfo_p;
                        ulGrantInfo.pdcchULNewUE_p = pdcchULNewUE_p;

                        LOG_UT(MAC_REQ_RB_UE_INFO,LOGDEBUG,MAC_PUSCH,\
                                getCurrentTick(),\
                                ulGrantInfo.ulUEContext_p->ueIndex,
                                ulGrantInfo.ulUEContext_p->userLocationType,\
                                tempPUSCHResources,\
                                *bufferRBs_p,\
                                tempDirectIndexingUEInfo_p->requiredRB,\
                                0.0,0.0,\
                                __func__,"REQ RB UE INFO");

                        /*Fix for SPR 12477, passing subFrameNum*/   
                        if (MAC_FAILURE == processULGrants(&ulGrantInfo,
                                    currentGlobalTick,
                                    &tempPUSCHResources,
                                    bufferRBs_p,
                                    /* Time Averaging changes start */
                                    powerCorrectionAndTpc,
                                    /* Rel9_upgrade_CR410 */
                                    ulDelay,
                                    /* Rel9_upgrade_CR410 */
                                    &ueMaxPowerFlag,
                                    /* Time Averaging changes end */
                                    /* CR changes start */
                                    hiIndex,&ueMinPowerFlag,FALSE,
                                    internalCellIndex,subFrameNum,execArgs))
                            /* CR changes end */
                        {
                            /* PUSCH RB Log Start */
                            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                                    LOGWARNING,MAC_UL_Strategy,\
                                    currentGlobalTick,\
                                    ulUEContext_p->ueIndex,\
                                    tempDirectIndexingUEInfo_p->flag,__LINE__,\
                                    tempDirectIndexingUEInfo_p->ulGrantRequestType,0,\
                                    0.0,0.0,__func__,
                                    "PUSCH_FAILprocessULGrants");
                            /* PUSCH RB Log End */
                            /* This can only occur when the memory allocation was not proper */
                            /* Cyclomatic Complexity changes */
                            mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].\
                                    dciPduInfo.aggregationLvl,
                                    (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex]),
                                    internalCellIndex); 
                            tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;                       
                            tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                            /* UL QOS CHANGE */
                            ulQosUpdateScheduledTokens(ulUEContext_p,0,INVALID_HARQ_ID,
                                    internalCellIndex,execArgs);
                            if (ULSCH_UCI_INVALID_INDEX !=
                                    tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
                            {
                                /* PUCCH_RESERVE_ALWAYS Start */
#ifndef PUCCH_RESERVE_ALWAYS
                                LOG_MAC_MSG(PUCCH_PUSCH_RES_OVERLAP_LOG_ID,LOGWARNING,MAC_PUSCH,\
                                        currentGlobalTick,\
                                        ulUEContext_p->ueIndex,0,0,0,0,\
                                        0.0,0.0,__func__,"PUCCH_PUSCH_RES_OVERLAP_LOG_ID");
#endif
                                /* PUCCH_RESERVE_ALWAYS End */
                                /* + SPR 6864 */
                                checkAndRestoreUCIPduType(ulUEContext_p, tempDirectIndexingUEInfo_p, ulschUciRachInfo_p,
                                        /* CSI_P2_PUSCH_CHANGES Start */
                                        subFrameNum,
                                        /*SPR 19504 fix start*/    
                                        rbMapNum,
                                        internalCellIndex
                                        /*SPR 19504 fix end*/    
                                        /* CSI_P2_PUSCH_CHANGES end */
                                        );
                                /* - SPR 6864 */
                            }
                        } 
                        else
                        {
                            /* TTI Bundling changes Starts*/
                            if(ulUEContext_p->ttiBundlingEnabled)
                            {

                                enqueueTick = currentGlobalTickWithULDelay;
                                enqueueNodesInTTIBScheduleQ(enqueueTick,tempUeId,tempHarqProcessId,1,FALSE,internalCellIndex);
                            } 
                            /* TTI Bundling changes Ends*/
#ifdef PERF_STATS
                            if (ueMaxPowerFlag == FALSE)
                            {
                                /* Increment the Counter when  Transport Blocks on MAC level scheduled in uplink
                                 *   where the UE was NOT considered to be power limited */
                                gMacUePerfStats_p[tempUeId].lteUeMacSchedULPerfStats.\
                                    totalTbScheduledInUlOnNoPowerRestriction++;
                            }
                            else
                            {
                                /* Increment the Counter when  Transport Blocks on MAC level scheduled in uplink
                                 *   where the UE was considered to be power limited */
                                gMacUePerfStats_p[tempUeId].lteUeMacSchedULPerfStats.\
                                    totalTbScheduledOnPowerRestriction++;
                            }
#endif
                            /* Reset the tpcTriggeredValue in UE Context */
                            ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue = 
                                TPC_NO_CHANGE;

                            /* +CLPC_CHG_LJA */
                            if (powerCorrectionAndTpc.powerCorrectionVal)
                            {
                                ulUEContext_p->prevPowerCorrectionForPusch =
                                    ulUEContext_p->currPowerCorrectionForPusch;

                                CHECK_AND_UPDATE_FI_CORRECTION(
                                        ueMaxPowerFlag,
                                        ueMinPowerFlag,
                                        powerCorrectionAndTpc,
                                        ulUEContext_p)

                            }
                            /* powerControlDeltaTFAndLogFactor will store the value of 
                             * deltaTF and Log10M factors in the UE context 
                             * so that MAC can calculate PL based on this
                             */
                            ulUEContext_p->powerControlDeltaTFAndLogFactor[subFrameNum] = 
                                fetchDeltaTFandLogMFactor(ulUEContext_p->dedicatedULPowerControlInfo.deltaMcsEnabled, 
                                        ulUEContext_p->ulresInfo[tempHarqProcessId].mcsIndex,
                                        ulUEContext_p->ulresInfo[tempHarqProcessId].riLength);
                            /* -CLPC_CHG_LJA */
                            tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                            /*START:6172*/
                            ulUEContext_p->advlastScheduledTick = currentGlobalTick + ulDelay;
                            /*END:6172*/
                            /* DRX_CHG */
                            if (SETUP == ulUEContext_p->drxConfigType)
                            {
                                putEntryInDRXULSchedulerEventQueue(tempUeId, 
                                        currentGlobalTick + ulDelay,
                                        internalCellIndex);
                            }
                            /* DRX_CHG */
                            (*numberOfULUEsToBeScheudled_p)--;

                            /* + CQI_5.0 */
                            if ( (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG != tempDirectIndexingUEInfo_p->ulGrantRequestType) &&
                                    /* SPS changes */ 
                                    (UL_SPS_DEACTIVATION != tempDirectIndexingUEInfo_p->ulGrantRequestType) )
                                /* SPS changes */ 
                            {
#ifdef ULHARQ_TIMER_PROC
                                UInt32 ulHarqSysFrameNum = 0;
                                UInt32 ulHarqSubFrameNum = 0; 
                                /* SPR 15909 fix start */
                                tickType_t timerExpiredTTI = 0; 
                                /* SPR 15909 fix end */
                                UInt32 timerExpsubFrameNum = 0;

                                getSFAndSFN(&ulHarqSysFrameNum, &ulHarqSubFrameNum, internalCellIndex); 

                                /* TTIB_Code Start */
                                timerExpiredTTI = (ulUEContext_p->ttiBundlingEnabled)?
                                    (currentGlobalTickWithULDelay + TTIB_ULHARQ_TIMER_TTI_TX_OFFSET):
                                    (currentGlobalTickWithULDelay + START_ULHARQ_TIMER_OFFSET);

                                timerExpsubFrameNum = (ulUEContext_p->ttiBundlingEnabled)?
                                    (ulHarqSubFrameNum + ulDelay + TTIB_ULHARQ_TIMER_TTI_TX_OFFSET):
                                    (ulHarqSubFrameNum + ulDelay + START_ULHARQ_TIMER_OFFSET);
                                /* TTIB_Code End */


                                if(timerExpsubFrameNum >= MAX_SUB_FRAME)
                                {
                                    timerExpsubFrameNum = timerExpsubFrameNum % MAX_SUB_FRAME;

                                    ulHarqSysFrameNum++;
                                    if (ulHarqSysFrameNum == MAX_SFN_VALUE)
                                    {
                                        ulHarqSysFrameNum = 0;
                                    }
                                }    

                                ulHarqTimerStart(tempUeId,tempHarqProcessId,timerExpsubFrameNum, 
                                        ulUEContext_p->schType,
                                        timerExpiredTTI,ulHarqSysFrameNum,internalCellIndex);

                                LTE_MAC_UT_LOG(LOG_INFO,LOG_PUSCH,
                                        "\n[%s] [%d] tempUeId %d tempHarqProcessId %d  timerExpsubFrameNum %d"
                                        "timerExpiredTTI %d ulHarqSysFrameNum %d\n "
                                        ,__func__,getCurrentTick(),tempUeId,tempHarqProcessId,
                                        timerExpsubFrameNum,timerExpiredTTI,ulHarqSysFrameNum);
#endif
                            }
                            /* - CQI_5.0 */

                            if( (ULSCH_UCI_INVALID_INDEX == tempDirectIndexingUEInfo_p->ulschUciInfoIndex) &&
                                    /* SPS CHANGES */
                                    (tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_DEACTIVATION ))
                                /* SPS CHANGES */
                            {

                                /* Semi static changes End*/
                                if ( MAX_ULSCH_UCI_SUPPORTED ==  numberOfInfoPresent)
                                {
                                    ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                                            __func__);
                                    break;
                                }
                                /*SPR 7086 Fix Start*/
                                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS = 
                                    nSRSDetermineOnCollision(ulUEContext_p, tempHarqProcessId,
                                            subFrameNum, internalCellIndex);
                                /*Update nSrsInitial in HARQ Context*/
                                ulUEContext_p->ulresInfo[tempHarqProcessId].nSrsInitial = 
                                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS;
                                /*SPR 7086 Fix End*/
                                /* Fill UL Harq Info*/
                                ulschUciRachInfo_p->
                                    ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p = 
                                    &(ulUEContext_p->ulresInfo[tempHarqProcessId]);

                                ulschUciRachInfo_p->
                                    ulschUciInfo[numberOfInfoPresent].ulHarqProcessId =
                                    tempHarqProcessId;
                                /* SPR 1723 changes start */
                                ulschUciRachInfo_p->
                                    ulschUciInfo[numberOfInfoPresent].rnti =
                                    ulUEContext_p->crnti;
                                /* SPR 1723 changes end */
                                /*Cyclomatic Complexity changes - starts here*/
                                updateUlschUciRachInfo(tempDirectIndexingUEInfo_p,
                                        ulUEContext_p,
                                        ulschUciRachInfo_p,
                                        numberOfInfoPresent,
                                        subFrameNum,
                                        ulUEContextInfo_p,
                                        internalCellIndex);
                                /*Cyclomatic Complexity changes - ends here*/
                                LOG_UT(MAC_UCI_ULSCH_PDU_ASSIGN,LOGDEBUG,MAC_PUSCH,\
                                        getCurrentTick(),\
                                        tempUeId,
                                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType,\
                                        tempDirectIndexingUEInfo_p->ulschUciInfoIndex,\
                                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId,\
                                        0,0.0,0.0,\
                                        __func__,"ULSCH_PDU_ASSIGN");
                                /* SPR 14877 SRS Changes*/
                                tempDirectIndexingUEInfo_p->ulschInfoIndex = numberOfInfoPresent;
                                /* SPR 14877 SRS Changes*/
                                numberOfInfoPresent++;
                            }
                            /* SPS CHANGES */
                            else if(tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_DEACTIVATION)
                                /* SPS CHANGES */
                            {
                                /* + CQI_5.0 */
                                if ( CQI_WITHOUT_DATA_ULGRANT_REQ_MSG ==
                                        tempDirectIndexingUEInfo_p->ulGrantRequestType
                                        ||
                                        /* CSI_P2_PUSCH_CHANGES Start */
                                        (ulUEContextInfo_p->ulUEContext_p->
                                         aperiodicCQIToBeRequested)
                                        /* CSI_P2_PUSCH_CHANGES End */
                                   )
                                {
                                    callSemiStaticforAperiodicFlag = TRUE;
                                }    
                                /* - CQI_5.0 */
                                /* ICIC changes start */
                                fillSemiStaticInfoForULSCH ( ulschUciRachInfo_p,
                                        tempDirectIndexingUEInfo_p, 
                                        ulUEContextInfo_p,
                                        &(ulUEContext_p->ulresInfo[tempHarqProcessId]),
                                        tempHarqProcessId,
                                        subFrameNum,
                                        callSemiStaticforAperiodicFlag,
                                        internalCellIndex);
                                /* SPR 10195 Fix Start */
                                /*Update nSrsInitial in HARQ Context*/
                                ulUEContext_p->ulresInfo[tempHarqProcessId].nSrsInitial =
                                    ulschUciRachInfo_p->ulschUciInfo[tempDirectIndexingUEInfo_p->ulschUciInfoIndex].nSRS;
                                /* SPR 10195 Fix End */
                                /* SPR 4635 Fix */ 
                                callSemiStaticforAperiodicFlag = FALSE;
                                /* SPR 4635 Fix */ 
                                /* ICIC changes end */
                            }
                            /* SS_S2 */

                            /* Semi static changes End*/

                            /** HD FDD Changes **/
#ifdef HD_FDD_CONFIG
                            if(ulUEContextInfo_p->ulUEContext_p->isHDFddFlag)
                            {
                                localTick = getCurrentTick() + PHY_DL_DELAY;
                                SCHEDULE_UL_FOR_HD_UE(localTick ,ulGrantInfo.ulUEContext_p->ueIndex , \
                                        hdUeContextInfo_g.hdfddUeSchedMap, internalCellIndex);
                            }
#endif
                            /** HD FDD Changes **/
                        }                  
                        /* UL_MU_MIMO_CHG Start */
                        /* If PDSCH allocation is done for Buddy pair, 
                         * reset isPuschAllocated Flag in both UE's context */
                                    if(buddyUeIndex < MAX_UE_SUPPORTED)                    
                                    {   
                                        /*ulBuddyUEContext_p = ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p;*/
                                        if( (ulUEContext_p->ulMUMIMOInfo.isPuschAllocated) &&
                                                ( PNULL != ulBuddyUEContext_p) &&
                                                (ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated) )
                                        {
                                            ulUEContext_p->ulMUMIMOInfo.isPuschAllocated      = FALSE;
                                            ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated = FALSE;
                                        }
                                    }
                                /* UL_MU_MIMO_CHG End */
            }
            else
            { 
                /*  
                    we must revert back the cce allocated by 
                    PDCCH as the UE is on Deletion by the RRC
                    */
                tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                /* Cyclomatic Complexity changes */
                mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].\
                        dciPduInfo.aggregationLvl,
                        (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex]),
                        internalCellIndex); 
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            }
        }
        else
        {
            /* PUSCH RB Log Start */
            /*Coverity 40422 Fix Start*/
            LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                    LOGWARNING,MAC_UL_Strategy,\
                    currentGlobalTick,\
                    ulUEContext_p?ulUEContext_p->ueIndex:INVALID_UE_ID,\
                    tempDirectIndexingUEInfo_p->flag,__LINE__,\
                    tempDirectIndexingUEInfo_p->ulGrantRequestType,0,\
                    0.0,0.0,__func__,
                    "PUSCH_FAIL");
            /*Coverity 40422 Fix End*/
            /* PUSCH RB Log End */
            tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
            LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH,\
                    "[%s]The UE already considered in collision"
                    "Handling PHASE\n" ,__func__);             
        }
        localCount++;
        pdcchULNewUE_p->ueIndex = INVALID_UE_ID;

        /* + CQI_5.0 */
        if(ulUEContext_p)
        {
            ulUEContext_p->aperiodicCQIToBeRequested = FALSE;
        }
        /* - CQI_5.0 */
    }

    /* ICIC changes start */
    tempPdcchULOutput_p->pdcchULNewUEInfo[locType].count = 0;
    /* ICIC changes end */
    /*
       We are still left with the scenario when the UE would have recived a 
       NACK in this tick but it hadn't faced any collision, the Flag of the 
       tempDirectIndexingUEInfo_p->flag would have been set to RECEIVED_NACK
       which needs to be re-set to UNSET_FLAG.
       */
                            /*Cyclomatic Complexity changes - starts here*/
                            updatepdcchULNackUEInfoTempUEInfo( tempPdcchULOutput_p,
                                    locType,
                                    tempDirectIndexingUEInfo_p);
                            /*Cyclomatic Complexity changes - ends here*/

                            /*Update Number of numberOfInfoPresent in the global context */ 
                            ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;

                            LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Exit...... \n",__func__);
}

/*Cyclomatic Complexity changes - starts here*/
/********************************************************************************
 * Function Name : updateUlschUciRachInfo    
 * Inputs        : tempDirectIndexingUEInfo_p - Pointer to DirectIndexingUEInfo,
 *                 ulUEContext_p - Pointer to ULUEContext,
 *                 ulschUciRachInfo_p - Pointer to ULSchUciRachPoolInfo,
 *                 numberOfInfoPresent,
 *                 subFrameNum - Subframe number,
 *                 ulUEContextInfo_p p - Pointer to ULUEContextInfo
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : None
 * Description:  : This function will Ulsch Uci Rach Info.
 ********************************************************************************/
                STATIC  void updateUlschUciRachInfo(
                        DirectIndexingUEInfo *tempDirectIndexingUEInfo_p,
                        ULUEContext *ulUEContext_p,
                        ULSchUciRachPoolInfo *ulschUciRachInfo_p,
                        UInt16 numberOfInfoPresent,
                        UInt8 subFrameNum,
                        ULUEContextInfo* ulUEContextInfo_p,
                        /*CA Changes start */
                        InternalCellIndex internalCellIndex)
                /*CA Changes end */
{
    if( CQI_WITH_DATA_ULGRANT_REQ_MSG == 
            tempDirectIndexingUEInfo_p->ulGrantRequestType
            || CQI_WITHOUT_DATA_ULGRANT_REQ_MSG == 
            tempDirectIndexingUEInfo_p->ulGrantRequestType 
            /* + CQI_5.0 */
            /* CSI_P2_PUSCH_CHANGES Start */
            || (ulUEContext_p->aperiodicCQIToBeRequested) )
        /* CSI_P2_PUSCH_CHANGES End */
        /* - CQI_5.0 */
    {
        /* SPR 1115 start */                        
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = 
            MAC_SS_PDU_ULSCH_CQI_RI;
        storeCqiPmiRiDataAperiodic(
                tempDirectIndexingUEInfo_p->transMode, 
                ulUEContextInfo_p, ulschUciRachInfo_p, subFrameNum, 
                numberOfInfoPresent, internalCellIndex);
    }
    else if(tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_DEACTIVATION )
    {
        ulschUciRachInfo_p->
            ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_ULSCH; 
    }
}

/********************************************************************************
 * Function Name : updatepdcchULNackUEInfoTempUEInfo    
 * Inputs        : tempPdcchULOutput_p - Pointer to PdcchULOutputInfo,
 *                 locType,
 *                 tempDirectIndexingUEInfo_p - Pointer to DirectIndexingUEInfo
 * Outputs       : None
 * Returns       : None
 * Description:  : This function will update Temp UE UL Nack Info.
 ********************************************************************************/
static  void updatepdcchULNackUEInfoTempUEInfo(
        PdcchULOutputInfo* tempPdcchULOutput_p,
        UInt8 locType,
        DirectIndexingUEInfo *tempDirectIndexingUEInfo_p)
{
    UInt32 nackUECount = 0; 
    UInt16 tempUeId=0;

    nackUECount = tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount;

    while(nackUECount--)
    {
        tempUeId = tempPdcchULOutput_p->pdcchULNackUEInfo.
            pdcchULNackNonAdaptiveUEArray[nackUECount];
        tempDirectIndexingUEInfo_p = &(tempPdcchULOutput_p->
                directIndexingUEInfo[tempUeId]);
        tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Unsetting the Flag "
                "for Non Adaptive Nack UE Index = %d \n",__func__,tempUeId);
    } 
    tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount = 0; 

    nackUECount = tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount[locType];
    while(nackUECount--)
    {
        tempUeId = tempPdcchULOutput_p->pdcchULNackUEInfo.
            pdcchULNackAdaptiveUEArray[locType][nackUECount];
        tempDirectIndexingUEInfo_p = &(tempPdcchULOutput_p->
                directIndexingUEInfo[tempUeId]);
        tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Unsetting the Flag "
                "for Adaptive Nack UE Index = %d \n",__func__,tempUeId);
    } 
    /* ICIC changes start */
    tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount[locType] = 0; 
    /* ICIC changes end */

    nackUECount = tempPdcchULOutput_p->pdcchULNackTempUEInfo.nonAdaptiveCount;
    while(nackUECount--)
    {
        tempUeId = tempPdcchULOutput_p->pdcchULNackTempUEInfo.
            pdcchULNackNonAdaptiveUEArray[nackUECount];
        tempDirectIndexingUEInfo_p = &(tempPdcchULOutput_p->
                directIndexingTempUEInfo[tempUeId]);
        tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Unsetting the Flag "
                "for Non Adaptive Nack UE Index = %d \n",__func__,tempUeId);
    } 
    tempPdcchULOutput_p->pdcchULNackTempUEInfo.nonAdaptiveCount = 0; 

    nackUECount = tempPdcchULOutput_p->pdcchULNackTempUEInfo.adaptiveCount[locType];
    while(nackUECount--)
    {
        tempUeId = tempPdcchULOutput_p->pdcchULNackTempUEInfo.
            pdcchULNackAdaptiveUEArray[locType][nackUECount];
        tempDirectIndexingUEInfo_p = &(tempPdcchULOutput_p->
                directIndexingTempUEInfo[tempUeId]);
        tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Unsetting the Flag "
                "for Adaptive Nack UE Index = %d \n",__func__,tempUeId);
    } 
    tempPdcchULOutput_p->pdcchULNackTempUEInfo.adaptiveCount[locType] = 0; 
}

#elif TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129107) */
/********************************************************************************
 * Function Name : processULNewUeInPUSCH
 * Inputs        : tempPdcchULOutput_p - Pointer to PdcchULOutputInfo,
 *                 containerTick,
 *                 currentGlobalTick - Global Tick,
 *                 subFrameNumber - Subframe number,
 *                 rbMapNum,
 *                 ulGrantSysFrameNum,
 *                 ulGrantSubFrameNum
 *                 numberOfULUEsToBeScheudled_p - No. of UL UE's to be scheduled,
 *                 bufferRBs_p,
 *                 locType
 *                 internalCellIndex - Cell-Index at MAC
 * Outputs       : None
 * Returns       : None
 * Description:  : This function will process the New UL UE in PUSCH.
 ********************************************************************************/
    void processULNewUeInPUSCH(
            /* SPR 5599 changes end (ZIP ID 129107) */
            PdcchULOutputInfo* tempPdcchULOutput_p,
            UInt8  containerTick,
            /* SPR 15909 fix start */
            tickType_t currentGlobalTick,
            /* SPR 15909 fix end */
            UInt8  subFrameNumber,
            UInt8  rbMapNum,
            /*SPR 5620 START*/
            UInt16 ulGrantSysFrameNum,
            /*SPR 5620 END*/
            UInt16 ulGrantSubFrameNum,
            UInt16* numberOfULUEsToBeScheudled_p,
            /* ICIC changes start */
            SInt8 *bufferRBs_p,
            UInt8 locType,
            InternalCellIndex internalCellIndex,
            ExecutionLegFunctionArg* execArgs
            )
    /* ICIC changes end */
{
    UInt16            tempUeId = 0;
    DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
    DciCCEInfo* tempDciCCEInfo_p               = PNULL;
    UeScheduledInfo *ueScheduledInfo_p         = PNULL;
    ULUEContext* ulUEContext_p                 = PNULL;
    ULUEContextInfo* ulUEContextInfo_p         = PNULL;
    UInt32 localCount                          = 0;
    ULGrantInfo ulGrantInfo                    = {0};
    UInt32 tempPUSCHResources                  = 0;
    UInt32 loopCount                           = 0;
    UInt32 nackUECount                         = 0; 
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    /* Time Averaging changes start */
    UInt32 ueMaxPowerFlag = 0;
    /* Time Averaging changes end */
    /* ATB Changes 5.1 Start */
    UInt32 ueMinPowerFlag = 0;
    /* ATB Changes 5.1 End */

    /* TDD Config 6 Changes Start*/
    UInt8 ulIndexFac = MODULO_TWO(ulGrantSysFrameNum);

    PdcchULNewUE *pdcchULNewUE_p = PNULL;
    UInt16 numberOfInfoPresent = 0;

    /*TDD Config 0 Changes Start*/
    ULSchUciRachPoolInfo* ulschUciRachInfo_p
        = ulSchUciRachContainer_gp[internalCellIndex] + (ulGrantSubFrameNum % MAX_ULSCH_UCI_CONTAINER);
    /*TDD Config 0 Changes End*/
    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;

    UInt16 sysFrameNum = (currentGlobalTick / MAX_SUB_FRAME) % MAX_SFN_VALUE;

    /* TDD Config 0 Changes Start */
    UInt8 tokenUpdateFromStr = FALSE;
    /* TDD Config 0 Changes End */

    LTE_MAC_UT_LOG(LOG_CRITICAL, LOG_PUSCH,
            "[%s] Entry containerTick[%d] currentGlobalTick[%d] ulGrantSubFrameNum [%d] \n",
            __func__, containerTick, currentGlobalTick, ulGrantSubFrameNum);

    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    /*
       Among the newly scheduled UE we first consider those
       for which there has been collision and for whom the CCE
       has been allocated by PDCCH  
       */
    /* ICIC changes start */
    /*CA Changes start  */
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum; 
    /*CA Changes end  */
    tempPUSCHResources = 
        rbMapNode_p->availableResourceInfo[locType].freeRBCounter;

    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, \
            "[%s] No. of Resources Allocated = %d "
            "in the Sub Frame = %d with numberOfULUEsToBeScheudled_p = %d\n"
            ,__func__,tempPUSCHResources, ulGrantSubFrameNum, *numberOfULUEsToBeScheudled_p);

    localCount = 0;
    /*
       We now consider the case of NEW UEs that has been scheduled by the Stategy this time 
       but no collision is reported and for whom the CCE has been allocated by PDCCH
       */

    loopCount = tempPdcchULOutput_p->pdcchULNewUEInfo[locType].count;
    /* UL_MU_MIMO_CHG Start */
    UInt16       buddyUeIndex       = INVALID_UE_INDEX;
    ULUEContext* ulBuddyUEContext_p = PNULL;
    /* UL_MU_MIMO_CHG End */
    /* TDD SB_CQI */
    UInt8 callSemiStaticforAperiodicFlag = FALSE;
    /* TDD SB_CQI */
    LTE_MAC_UT_LOG( LOG_CRITICAL, LOG_PUSCH, \
            "[%s] #####  No. of NewUE = %d  "
            "And No. of UL UE to be scheduled = %d loopCount = %d \n"
            ,__func__,loopCount, *numberOfULUEsToBeScheudled_p,loopCount);

    while((loopCount--))          
    {
        /* Time Averaging changes start */
        ueMaxPowerFlag = 0;
        /* Time Averaging changes end */

        /* ATB Changes 5.1 Start */
        ueMinPowerFlag = 0;
        /* ATB Changes 5.1 End */

        pdcchULNewUE_p = &(tempPdcchULOutput_p->pdcchULNewUEInfo[locType].pdcchULNewUEArray[localCount]);

        tempUeId = pdcchULNewUE_p->ueIndex;
        /*SPR 2446 Fix Begins*/
        /*pdcchULNewUE_p->ueIndex = INVALID_UE_ID;*/
        /*SPR 2446 Fix Ends*/

        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeId];       
        ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
        /* +COVERITY 5.2.2, 32700*/
        if ( PNULL == ulUEContext_p)
        {
            ltePanic("\n [%s] ulUEContext_p is null\n",__func__);
            /* coverity 32700 40422 28June2014 Start */
            return;
            /* coverity 32700 40422 28June2014 Start */

        }
        /* -COVERITY 5.2.2, 32700*/

        /*TDD Config 0 Changes Start*/
        /** SPR 14204 Fix : Start **/
        ueScheduledInfo_p = ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulGrantSubFrameNum];
        /** SPR 14204 Fix : End **/
        /*TDD Config 0 Changes End*/

        if ( PNULL == ueScheduledInfo_p)
        {
            ltePanic("\n [%s] ueScheduledInfo_p is null\n",__func__);

        }
        LTE_MAC_UT_LOG( LOG_CRITICAL, LOG_PUSCH, "\nsubFrameNumber [%d] ueScheduledInfo_p[%p]\n",
                subFrameNumber, ueScheduledInfo_p);
        /*  TDD REL 3.1.1 */
        if((ueScheduledInfo_p->flag != UNSET_FLAG) &&
                (ueScheduledInfo_p->flag != MEAS_GAP_RE_TX_NO_CCE_NO_GRANT))
            /*  TDD REL 3.1.1 */
        {
            /*
               we call the function for allocating PUSCH resources
               for Newly Scheduled UEs without any collision
               */
            if (!ulUEContextInfo_p->pendingDeleteFlag)
            {
                /* SPR 2509 CHG start */

                /*TDD Config 0 Changes Start*/
                ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId].isPdcchAllocated = 0;
                /*TDD Config 0 Changes End*/

                /* SPR 2509 CHG end */
                /*  TDD REL 3.1.1 */
                /* TDD Config 6 Changes Start */
                if((ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId].riLength != 0) ||
                        (INVALID_HARQ != ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[ulIndexFac][ulGrantSubFrameNum]))
                {
                    /* SPR 16916 FIX START */
#ifdef KPI_STATS
                    freeULHarqProcess(&ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId],
                            ulUEContext_p->ueIndex,internalCellIndex);
#else
                    freeULHarqProcess(&ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId]);
#endif
                    /* SPR 16916 FIX END */
                    resetHarqId(ulGrantSubFrameNum,ulUEContext_p, internalCellIndex,ulGrantSysFrameNum);
                    /* TDD Config 6 Changes End */
                }
                /* UL_MU_MIMO_CHG Start */
                buddyUeIndex = ueScheduledInfo_p->buddyUeIndex;
                if((buddyUeIndex < MAX_UE_SUPPORTED) && (PNULL != ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p))
                {
                    ulBuddyUEContext_p = ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p;
                }
                /* UL_MU_MIMO_CHG End */

                /* TDD SB_CQI */
                if(CQI_WITHOUT_DATA_ULGRANT_REQ_MSG == ueScheduledInfo_p->ulGrantRequestType && 
                        ULSCH_UCI_INVALID_INDEX != ueScheduledInfo_p->ulschUciInfoIndex)
                {
                    ULSchUciInfo* ulSchUciInfo_p = PNULL;
                    ulSchUciInfo_p = &ulschUciRachInfo_p->ulschUciInfo[ueScheduledInfo_p->ulschUciInfoIndex];
                    switch (ulSchUciInfo_p->pduType)
                    {
                        case MAC_SS_PDU_UCI_CQI:
                        case MAC_SS_PDU_UCI_CQI_HARQ:
                        case MAC_SS_PDU_UCI_HARQ:
                        {
                            break;				
                        }
                        case MAC_SS_PDU_UCI_SR:
                        case MAC_SS_PDU_UCI_SR_HARQ:
                        case MAC_SS_PDU_UCI_CQI_SR:
                        case MAC_SS_PDU_UCI_CQI_SR_HARQ:
                        {
                            ulUEContext_p->aperiodicCQIToBeRequested = TRUE;
                            /* Cyclomatic Complexity changes */
                            mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                                    dciPduInfo.aggregationLvl,
                                    (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                                    internalCellIndex);
                            ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                            ueScheduledInfo_p->flag = UNSET_FLAG;
                            /* TDD Config 0 Changes Start */
                            ulQosUpdateScheduledTokens(
                                    ulUEContext_p,
                                    0,
                                    INVALID_HARQ_ID,
                                    internalCellIndex,
                                    ulGrantSubFrameNum,
                                    tokenUpdateFromStr,
                                    execArgs
                                    );
                            /* TDD Config 0 Changes End */

                            localCount++;
                            /*SPR 20145 fix +-*/
                            continue;
                        }
                        default:
                        {
                            lteWarning("Invalid PDU Type [%d] received in func [%s]\n",
                                    ulSchUciInfo_p->pduType, __func__);
                            break;
                        }
                    }
                }
                /* TDD SB_CQI */ 

                if ((*numberOfULUEsToBeScheudled_p < 1) || 
                        ( (!tempPUSCHResources) && ( ( PNULL == ulBuddyUEContext_p) ||
                                                     (!ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated) ) ))
                    /*  TDD REL 3.1.1 */
                {
                    LTE_MAC_UT_LOG(LOG_WARNING, LOG_PUSCH, 
                            " processULGrants ::error Harq process is not free\n"); 

                    /* + SPR 6864 */
                    if (ULSCH_UCI_INVALID_INDEX !=
                            ueScheduledInfo_p->ulschUciInfoIndex)
                    {
                        /* + SPR_14994 */
                        checkAndRestoreUCIPduType(ulUEContext_p, ueScheduledInfo_p, sysFrameNum,
                                ulschUciRachInfo_p,
                                /* CSI_P2_PUSCH_CHANGES Start */
                                subFrameNumber,
                                /*SPR 19504 fix start*/    
                                rbMapNum,
                                internalCellIndex
                                /*SPR 19504 fix end*/    
                                /* CSI_P2_PUSCH_CHANGES end */
                                );
                        /* - SPR_14994 */
                    }
                    /* - SPR 6864 */


                    if (ueScheduledInfo_p->ulGrantRequestType != \
                            NEW_ULGRANT_REQ_MSG)
                    {
                        putEntryInPUSCHFailureQueue(
                                tempUeId,
                                ueScheduledInfo_p->ttiCounter,
                                ueScheduledInfo_p->ulGrantRequestType,
                                internalCellIndex);
                    }

                    /* Cyclomatic Complexity changes */
                    /* SPR 11065 Changes Start */
                    /*
                     ** In case of resource allocation failure for TDD Config 0 S subframe, CCE will
                     ** be reset if allocation is failed for both UL-Subframe while 1 DCI0 will be sent
                     ** for a UE in both ulsubframe.
                     */
                    if ((pdcchULNewUE_p->isNodePresentIn2UlSubframe == FALSE) &&
                            (tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].dciPduInfo.ulIndex == NUM_ZERO))
                    {
                        mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                                dciPduInfo.aggregationLvl,
                                (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                                internalCellIndex);
                    }
                    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                    ueScheduledInfo_p->flag = UNSET_FLAG;
                    /* TDD Config 0 Changes Start */
                    ulQosUpdateScheduledTokens(
                            ulUEContext_p,
                            0,
                            INVALID_HARQ_ID,
                            internalCellIndex,
                            ulGrantSubFrameNum,
                            tokenUpdateFromStr,
                            execArgs
                            );
                    /* TDD Config 0 Changes End */

                    localCount++;
                    /*SPR 20145 fix +-*/
                    continue;
                }

                powerCorrectionAndTpc = getTpcForPusch(ulUEContext_p, currentGlobalTick, internalCellIndex
                        /*TDD Config 0 Changes Start*/
                        ,ulGrantSubFrameNum);
                /*TDD Config 0 Changes End*/
                if ((LAST_UE == *numberOfULUEsToBeScheudled_p)  &&
                        (!ueScheduledInfo_p->strictRBAllocation))
                {
                    /* Time Averaging changes start */
                    assignRBsForMaxData(pdcchULNewUE_p, &ueScheduledInfo_p->requiredRB,
                            tempPUSCHResources, powerCorrectionAndTpc.powerCorrectionVal ,&ueMaxPowerFlag,&ueMinPowerFlag
                            /* +DYNAMIC_ICIC */
                            ,ulUEContext_p
                            /* -DYNAMIC_ICIC */
                            ,internalCellIndex
                            );
                    ueMaxPowerFlag = FALSE;
                    ueMinPowerFlag = FALSE;
                    /* Time Averaging changes end */
                }
                /*
                   We now populate the structure that would be passed to the function 
                   processULGrants.
                   */
                        ulGrantInfo.tempUeIndex       = tempUeId;
                        ulGrantInfo.ulUEContext_p     = ulUEContext_p;
                        ulGrantInfo.tempHarqProcessId = ueScheduledInfo_p->ulHarqProcessId;
                        ulGrantInfo.ulSubframe        = ulGrantSubFrameNum;
                        ulGrantInfo.rbMapNum          = rbMapNum;
                        ulGrantInfo.tempDciCCEInfo_p  = tempDciCCEInfo_p;
                        ulGrantInfo.ueScheduledInfo_p = ueScheduledInfo_p;
                        ulGrantInfo.pdcchULNewUE_p = pdcchULNewUE_p;

                        /* ICIC changes start */
                        LOG_UT(MAC_REQ_RB_UE_INFO,LOGDEBUG,MAC_PUSCH,\
                                getCurrentTick(),\
                                ulGrantInfo.ulUEContext_p->ueIndex,
                                ulGrantInfo.ulUEContext_p->userLocationType,\
                                tempPUSCHResources,\
                                *bufferRBs_p,\
                                ueScheduledInfo_p->requiredRB,\
                                0.0,0.0,\
                                __func__,"REQ RB UE INFO");
                        /* ICIC changes end */
                        LTE_MAC_UT_LOG( LOG_CRITICAL, LOG_PUSCH,
                                "ProcessULGrant .. subframe[%d] ulGrantSubFrameNum [%d] hpId[%d] ulCCEIndex[%d]\n",
                                subFrameNumber,
                                ulGrantSubFrameNum,
                                ueScheduledInfo_p->ulHarqProcessId,
                                ueScheduledInfo_p->ulCCEIndex);

                        if (MAC_FAILURE == processULGrants(&ulGrantInfo,
                                    currentGlobalTick,
                                    &tempPUSCHResources,
                                    bufferRBs_p, 
                                    /* Time Averaging changes start */
                                    powerCorrectionAndTpc,
                                    /* Rel9_upgrade_CR410 */
                                    subFrameNumber,
                                    /* Rel9_upgrade_CR410 */
                                    /* + SPS_TDD_Changes */
                                    &ueMaxPowerFlag,&ueMinPowerFlag, FALSE,
                                    /* - SPS_TDD_Changes */
                                    internalCellIndex,execArgs))
                            /* Time Averaging changes end */
                        {
                            /* + SPR 6864 */
                            if (ULSCH_UCI_INVALID_INDEX !=
                                    ueScheduledInfo_p->ulschUciInfoIndex)
                            {
                                /* + SPR_14994 */
                                checkAndRestoreUCIPduType(ulUEContext_p, ueScheduledInfo_p,sysFrameNum,
                                        ulschUciRachInfo_p,
                                        /* CSI_P2_PUSCH_CHANGES Start */
                                        subFrameNumber,
                                        /* CSI_P2_PUSCH_CHANGES end */
                                        /*SPR 19504 fix start*/    
                                        rbMapNum,
                                        internalCellIndex
                                        /*SPR 19504 fix end*/    
                                        );
                                /* - SPR_14994 */
                            }
                            /* - SPR 6864 */


                            /* Cyclomatic Complexity changes */
                            /* SPR 11065 Changes Start */
                            /*
                             ** In case of resource allocation failure for TDD Config 0 S subframe, CCE will
                             ** be reset if allocation is failed for both UL-Subframe while 1 DCI0 will be sent
                             ** for a UE in both ulsubframe.
                             */
                            if ((pdcchULNewUE_p->isNodePresentIn2UlSubframe == FALSE) &&
                                    (tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].dciPduInfo.ulIndex == NUM_ZERO))
                            {
                                mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                                        dciPduInfo.aggregationLvl,
                                        (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                                        internalCellIndex);
                            }
                            ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                            ueScheduledInfo_p->flag = UNSET_FLAG;

                            /* TDD Config 0 Changes Start */
                            ulQosUpdateScheduledTokens(
                                    ulUEContext_p,
                                    0,
                                    INVALID_HARQ_ID,
                                    internalCellIndex,
                                    ulGrantSubFrameNum,
                                    tokenUpdateFromStr,
                                    execArgs
                                    );
                            /* TDD Config 0 Changes End */

                            if (!tempPUSCHResources)
                            {
                                localCount++;
                                /*SPR 20145 fix +-*/
                                break;
                            }  
                        } 
                        else
                        {
                            /*TDD Config 0 Changes Start*/
                            /* 
                             * In case of second execution of PUSCH at +7 U frame, we will skip 
                             * the power correction and updation related checks, as they have 
                             * already been verified for +k execution.
                             */
                            if (COMMON_DCI_COUNT != pdcchULNewUE_p->scheduleCount)
                            {
                                /* Reset the tpcTriggeredValue in UE Context */
                                ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue = 
                                    TPC_NO_CHANGE;
                                /*SPR Fix 12758 start */
                                ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId].forcedAckFlag
                                    = FORCED_ACK_RESET;
                                /* SPR Fix 12758 end */

                                ulUEContext_p->powerControlDeltaTFAndLogFactor[ulGrantSubFrameNum] = \
                                                                                                     fetchDeltaTFandLogMFactor(ulUEContext_p->dedicatedULPowerControlInfo.deltaMcsEnabled, 
                                                                                                             ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId].mcsIndex,
                                                                                                             ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId].riLength);

                                /* Time Averaging changes start */

                                if (powerCorrectionAndTpc.powerCorrectionVal)
                                {
                                    ulUEContext_p->prevPowerCorrectionForPusch =
                                        ulUEContext_p->currPowerCorrectionForPusch;
                                    CHECK_AND_UPDATE_FI_CORRECTION(
                                            ueMaxPowerFlag,
                                            ueMinPowerFlag,
                                            powerCorrectionAndTpc,
                                            ulUEContext_p)

                                }
                                /* Time Averaging changes end */
                                /* -CLPC_CHG_LJA */
                            }
                            /*TDD Config 0 Changes End*/

                            /*  TDD REL 3.1.1 */
                            ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                            /* TDD REL 3.1.1 */
                            ueScheduledInfo_p->flag = UNSET_FLAG;
                            (*numberOfULUEsToBeScheudled_p)--;
                            /* TDD SB_CQI */
                            if(CQI_WITHOUT_DATA_ULGRANT_REQ_MSG != ueScheduledInfo_p->ulGrantRequestType)
                            {
                                ulHarqTimerStart( subFrameNumber,
                                        sysFrameNum,
                                        tempUeId,
                                        ulGrantInfo.tempHarqProcessId,
                                        /* + SPS_TDD_Changes */
                                        //SPR_16897_Changes
                                        DRA_SCHEDULING,
                                        //SPR_16897_Changes
                                        /* - SPS_TDD_Changes */
                                        /*CA Changes start  */
                                        internalCellIndex,
                                        /*CA Changes end  */
                                        /*TDD Config 0 Changes Start*/
                                        ulGrantSubFrameNum);
                                /* TDD Config 0 Changes Start*/

                            }
                            /*START:6172*/
                            ulUEContext_p->advlastScheduledTick = currentGlobalTick;
                            /*END:6172*/
                            /* DRX_CHG */
                            if (SETUP == ulUEContext_p->drxConfigType)
                            {
                                putEntryInDRXULSchedulerEventQueue(tempUeId,
                                        currentGlobalTick,
                                        internalCellIndex);
                            }
                            /* DRX_CHG */
                            if(ULSCH_UCI_INVALID_INDEX == ueScheduledInfo_p->ulschUciInfoIndex)
                            {

                                if ( MAX_ULSCH_UCI_SUPPORTED ==  numberOfInfoPresent)
                                {
                                    ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                                            __func__);
                                    break;
                                }
                                /*SPR 7086 Fix Start*/

                                /*TDD Config 0 Changes Start*/
                                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS = 
                                    nSRSDetermineOnCollision(ulUEContext_p, 
                                            ulGrantInfo.tempHarqProcessId, 
                                            ulGrantSubFrameNum,internalCellIndex);
                                /*TDD Config 0 Changes End*/

                                /*Update nSrsInitial in HARQ Context*/
                                ulUEContext_p->ulresInfo[ulGrantInfo.tempHarqProcessId].nSrsInitial = 
                                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS;
                                /*SPR 7086 Fix End*/
                                /*Fill rnti value */
                                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                                    rnti = ulUEContext_p->crnti;
                                /* Fill UL Harq Info*/
                                ulschUciRachInfo_p->
                                    ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p = 
                                    &(ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId]);

                                ulschUciRachInfo_p->
                                    ulschUciInfo[numberOfInfoPresent].ulHarqProcessId =
                                    ueScheduledInfo_p->ulHarqProcessId;
                                /* SS_S2 */

                                if ( CQI_WITH_DATA_ULGRANT_REQ_MSG == 
                                        ueScheduledInfo_p->ulGrantRequestType
                                        || CQI_WITHOUT_DATA_ULGRANT_REQ_MSG == 
                                        ueScheduledInfo_p->ulGrantRequestType
                                        /* TDD SB_CQI */
                                        /* + SPR_14430 */
                                        || (ulUEContext_p->aperiodicCQIToBeRequested) )
                                    /* - SPR_14430 */
                                    /* TDD SB_CQI */
                                {
                                    /* SPR 1115 CHG */      
                                    ulschUciRachInfo_p->
                                        ulschUciInfo[numberOfInfoPresent].pduType = 
                                        MAC_SS_PDU_ULSCH_CQI_RI;

                                    storeCqiPmiRiDataAperiodic(
                                            ueScheduledInfo_p->transMode, 
                                            ulUEContextInfo_p, ulschUciRachInfo_p, 
                                            /*SPR 5620 START*/
                                            ulGrantSysFrameNum,
                                            /*SPR 5620 END*/
                                            ulGrantSubFrameNum, numberOfInfoPresent,
                                            internalCellIndex);
                                }
                                else
                                {
                                    ulschUciRachInfo_p->
                                        ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_ULSCH; 
                                }
                                LOG_UT(MAC_UCI_ULSCH_PDU_ASSIGN,LOGDEBUG,MAC_PUSCH,\
                                        getCurrentTick(),\
                                        tempUeId,
                                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType,\
                                        ueScheduledInfo_p->ulschUciInfoIndex,\
                                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId,\
                                        0,0.0,0.0,\
                                        __func__,"ULSCH_PDU_ASSIGN");
                                numberOfInfoPresent++;
                            }
                            else
                            {
                                /* TDD SB_CQI */
                                if ( CQI_WITHOUT_DATA_ULGRANT_REQ_MSG ==
                                        ueScheduledInfo_p->ulGrantRequestType
                                        /* + SPR_14430 */
                                        || (ulUEContextInfo_p->ulUEContext_p->aperiodicCQIToBeRequested) )
                                    /* - SPR_14430 */
                                {
                                    callSemiStaticforAperiodicFlag = TRUE;
                                }    
                                /* TDD SB_CQI */
                                /* ICIC changes start */


                                /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                                /* Check if this is HARQ pdu then fill HARQ size according HARQ
                                 * on PUSCH and set bit to indicate harq on PUSCH with DCI 0 */  
                                if (MULTIPLEXING == ulUEContext_p->tddAckNackFeedbackMode)
                                {
                                    SET_HARQ_SIZE_AND_BIT_FOR_UL_TX_WITH_DCI0_FOR_MUX(ulschUciRachInfo_p,
                                            ulUEContext_p,internalCellIndex,ulGrantSubFrameNum,
                                            ueScheduledInfo_p->ulschUciInfoIndex,ueScheduledInfo_p);
                                }
                                /* SPR 19679: TDD HARQ Multiplexing Changes End */


                                fillSemiStaticInfoForULSCH ( ulschUciRachInfo_p,
                                        ueScheduledInfo_p, 
                                        ulUEContextInfo_p,
                                        &(ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId]),
                                        ueScheduledInfo_p->ulHarqProcessId,
                                        /*SPR 5620 START*/
                                        ulGrantSysFrameNum,
                                        /*SPR 5620 END*/
                                        ulGrantSubFrameNum,callSemiStaticforAperiodicFlag,
                                        internalCellIndex);
                                /* ICIC changes end */
                                /* TDD SB_CQI */
                                /* SPR 10195 Fix Start */
                                /*Update nSrsInitial in HARQ Context*/
                                ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId].nSrsInitial =
                                    ulschUciRachInfo_p->ulschUciInfo[ueScheduledInfo_p->ulschUciInfoIndex].nSRS;

                                ueScheduledInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
                                /* SPR 10195 Fix End */
                                /* SPR 11152 Fix Start */
                                callSemiStaticforAperiodicFlag = FALSE;
                                /* SPR 11152 Fix End */

                            }
                            /* Semi static changes End*/

                        }
                        /* UL_MU_MIMO_CHG Start */
                        if( buddyUeIndex < MAX_UE_SUPPORTED)
                        {
                            /*ulBuddyUEContext_p = ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p;*/
                            if( (ulUEContext_p->ulMUMIMOInfo.isPuschAllocated) &&
                                    ( PNULL != ulBuddyUEContext_p) &&
                                    (ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated) )
                            {
                                ulUEContext_p->ulMUMIMOInfo.isPuschAllocated      = FALSE;
                                ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated = FALSE;
                            }
                        }
                        /* UL_MU_MIMO_CHG End */

            }            
            else
            {
                /* Cyclomatic Complexity changes */
                mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                        dciPduInfo.aggregationLvl,
                        (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                        internalCellIndex);
                ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                ueScheduledInfo_p->flag = UNSET_FLAG;
            }
        }
        else
        {
            /*  TDD REL 3.1.1 */
            ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            /*  TDD REL 3.1.1 */
            ueScheduledInfo_p->flag = UNSET_FLAG;
            LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH,\
                    "[%s]The UE already considered in collision"
                    "Handling PHASE\n" ,__func__);             
        }
        /*SPR 20145 fix +-*/
        localCount++;
        /* TDD SB_CQI */
        if(ulUEContext_p)
        {
            ulUEContext_p->aperiodicCQIToBeRequested = FALSE;
        } 
        /* TDD SB_CQI */
    }
    /*  TDD REL 3.1.1 */

    /*
       In the scenario when we have either break-ed the above loop or the
       value pointed by the numberOfULUEsToBeScheudled_p has become zero
       we still need the CCE to be set to invalid.
       */
                                                           loopCount = tempPdcchULOutput_p->pdcchULNewUEInfo[locType].count; 

                                while(loopCount--)
                                {
                                    pdcchULNewUE_p = &(tempPdcchULOutput_p->pdcchULNewUEInfo[locType].pdcchULNewUEArray[loopCount]);

                                    tempUeId = pdcchULNewUE_p->ueIndex;

                                    ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeId];        
                                    ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

                                    /*TDD Config 0 Changes Start*/ 
                                    /** SPR 14204 Fix : Start **/
                                    ueScheduledInfo_p = ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulGrantSubFrameNum];
                                    /** SPR 14204 Fix : End **/
                                    /*TDD Config 0 Changes End*/

                                    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                                    ueScheduledInfo_p->flag = UNSET_FLAG;
                                }
                                tempPdcchULOutput_p->pdcchULNewUEInfo[locType].count = 0;
                                /*  TDD REL 3.1.1 */
                                /*
                                   We are still left with the scenario when the UE would have recived a 
                                   NACK in this tick but it hadn't faced any collision, the Flag of the 
                                   tempDirectIndexingUEInfo_p->flag would have been set to RECEIVED_NACK
                                   which needs to be re-set to UNSET_FLAG.
                                   */

                                nackUECount = tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount;
                                while(nackUECount--)
                                {
                                    tempUeId = \
                                               tempPdcchULOutput_p->pdcchULNackUEInfo.pdcchULNackNonAdaptiveUEArray[nackUECount];

                                    ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeId];
                                    ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

                                    /*TDD Config 0 Changes Start*/
                                    /** SPR 14204 Fix : Start **/
                                    ueScheduledInfo_p = ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulGrantSubFrameNum];
                                    /** SPR 14204 Fix : End **/
                                    /*TDD Config 0 Changes End*/

                                    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                                    ueScheduledInfo_p->flag = UNSET_FLAG;

                                    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Unsetting the Flag "
                                            "for Non Adaptive Nack UE Index = %d \n",__func__,tempUeId);
                                } 
                                tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount = 0; 

                                nackUECount = tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount[locType];
                                while(nackUECount--)
                                {
                                    tempUeId = \
                                               tempPdcchULOutput_p->pdcchULNackUEInfo.pdcchULNackAdaptiveUEArray
                                               [locType][nackUECount];

                                    ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeId];
                                    ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

                                    /*TDD Config 0 Changes Start*/   
                                    /** SPR 14204 Fix : Start **/
                                    ueScheduledInfo_p = ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulGrantSubFrameNum];
                                    /** SPR 14204 Fix : End **/
                                    /*TDD Config 0 Changes End*/

                                    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                                    ueScheduledInfo_p->flag = UNSET_FLAG;

                                    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Unsetting the Flag "
                                            "for Adaptive Nack UE Index = %d \n",__func__,tempUeId);
                                } 
                                tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount[locType] = 0; 

                                /*Update Number of numberOfInfoPresent in the global context */ 
                                ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;
                                LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Exit...... \n",__func__);
}
#endif
/*Cyclomatic Complexity changes - starts here*/
/********************************************************************************
 * Function Name : checkAndUpdateFiCorrection    
 * Inputs        : ueMaxPowerFlag
 *                 ueMinPowerFlag
 *                 powerCorrectionAndTpc
 *                 ulUEContext_p - Pointer to ULUEContext
 * Outputs       : None
 * Returns       : None
 * Description:  : This function will check and update Fi Correction.
 ********************************************************************************/
void checkAndUpdateFiCorrection(UInt32 ueMaxPowerFlag,
        UInt32 ueMinPowerFlag,
        TpcTriggeredToPowerCorrection powerCorrectionAndTpc,
        ULUEContext* ulUEContext_p)
{
    if (   ( ueMaxPowerFlag == FALSE  &&
                ueMinPowerFlag == FALSE  ) || 
            ( ueMinPowerFlag == TRUE   && 
              powerCorrectionAndTpc.powerCorrectionVal > 0  ) || 
            ( ueMaxPowerFlag == TRUE   && 
              powerCorrectionAndTpc.powerCorrectionVal < 0  )  
       )
    {
        if (ulUEContext_p->dedicatedULPowerControlInfo.
                accumulationEnabled)
        {
            ulUEContext_p->currPowerCorrectionForPusch +=
                powerCorrectionAndTpc.powerCorrectionVal;
            CHECK_AND_UPDATE_POWER_CORRECTION_FOR_PUSCH(
                    ulUEContext_p->currPowerCorrectionForPusch);
        }
        else
        {
            ulUEContext_p->currPowerCorrectionForPusch =
                powerCorrectionAndTpc.powerCorrectionVal;
        }
        ulUEContext_p->powerControlPuschTPCValue =
            powerCorrectionAndTpc.powerCorrectionVal;
    }
}
/*Cyclomatic Complexity changes - ends here*/

/********************************************************************************
 * Function Name     : processPUSCHLogicForDelayPath2
 * Inputs            : ulDelay - The amount of delay expected in the UpLink path,
 *                     currentGlobalTick - The present Global Tick,
 *                     subFrameNum -Subframe number
 *                     internalCellIndex - Cell-Index at MAC
 * Outputs           : None
 * Returns           : MAC_SUCCESS /MAC_FAILURE
 * Description       : This function will work for delay path 2 on per node basis  
 *                     This would be the main calling function of the entire PUSCH
 *                     logic in case of delay path 2. This function process te uplink 
 *                     allocation in this order
 *                                1.NonApadtive for both types of UEs
 *                                2.Cell Edge Adaptive
 *                                3.Cell Center Adaptive
 *                                4.Cell Edge New Txn
 *                                5.Cell Center New Txn
 *                     and then the ACKs.  
 *********************************************************************************/

#ifdef FDD_CONFIG
                                    /* SPR 8712 changes */
#ifdef DL_DATA_SEPARATION
                                    extern SInt32 ev_fd_pusch_g;
#endif
                                    /* SPR 8712 changes */
                                    /* SPR 5599 changes start (ZIP ID 129107) */
                                    MacRetType processPUSCHLogicForDelayPath2( UInt8 ulDelay,
                                            /* SPR 5599 changes end (ZIP ID 129107) */
                                            /* SPR 15909 fix start */
                                            tickType_t currentGlobalTick,
                                            /* SPR 15909 fix end */
                                            UInt8 subFrameNum,
                                            InternalCellIndex internalCellIndex,
                                            ExecutionLegFunctionArg* execArgs)
{
    UInt8 containerTick = 0;
    UInt32 rbmapNum = 0;
    /* CR changes start */
    UInt8 hiIndex = 0;
    UInt8 resetHiIndex = 0;
    /* CR changes end */
    /* ICIC changes start */
    SInt8 bufferRBs[MAX_USER_LOCATION] = {0};
    /* ICIC changes end */
    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */
    InitParams *initParams_p = cellSpecificParams_g.\
                               cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    UInt8 numberOfULUEsToBeScheudled = initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
#ifdef KPI_STATS
    UInt8 totalUplinkPRBUsage;
    /* + KPI_CA */
    UInt32 activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    /* - KPI_CA */
#endif
/* + SPR 22954 fix*/
/* - SPR 22954 fix*/

    /*
       we calculate the value of pdcchUpLinkOutputContainerTick, hiPduContainerTick .dciCCEContainerTick
       All ticks are with respect to the tick when the information is to be sent to PHY interface.
       */
    containerTick = (currentGlobalTick + ulDelay)% MAX_PDCCH_CONTAINER;

    /* For TTIB_Code */
    UInt8 containerTickForBundleRetx = 0;
    containerTickForBundleRetx = ( currentGlobalTickWithULDelay + TTI_PHICH_TO_DCI_OFFSET) % MAX_PDCCH_CONTAINER;
    /*CA Changes start  */
    PdcchULOutputInfo* tempPdcchULOutputRetx_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTickForBundleRetx;
    /*CA Changes end  */

#ifdef DL_UL_SPLIT
    while(1)
    {
        PuschSignalNodeP * node_p = PNULL;
#ifdef CIRC_QUEUE_IMPL
        /*CA Changes start  */
        semWaitDefault(&waitForPUSCHSignalInd_g[internalCellIndex]);
        DEQUEUE_PUSCH_SIGNAL_CIRC_Q(PuschSignalNodeP, (void *)&node_p, internalCellIndex);
        /*CA Changes end  */
#else
        /* +- SPR 17777 */
        recvMsgWithIccQ(puschSignalQueueIdUL_g[internalCellIndex],(const void *) &node_p);
        /* +- SPR 17777 */
#endif
        if (node_p == PNULL)
        {
            continue;
        }
        if (node_p->isCCEAllocated == CCE_ALLOCATED)
        {
            FREE_MEM_NODE_PUSCH_SIGNAL_CIRC_Q(node_p);
            break;
        }
    }
#endif

    /*CA Changes start  */
    PdcchULOutputInfo* tempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */

    rbmapNum = MODULO_SIXTEEN((currentGlobalTick + ulDelay + FDD_HARQ_OFFSET));

    /* CR changes start */
    hiIndex = ( (currentGlobalTick + FDD_DATA_OFFSET +  FDD_HARQ_OFFSET + ulDelay) % MAX_PHICH_CONTAINER );
    resetHiIndex = ((currentGlobalTick + ulDelay) % MAX_PHICH_CONTAINER);
    /* CR changes end */

/* + SPR 22954 fix*/
/* - SPR 22954 fix*/

    /*
       call the function for pucch and get the pucchUEColissionArray[]  populated by the entries.
       */
    /* ICIC changes start */

    LOG_UT(MAC_ULSCH_RA_INFO_INDEX,LOGDEBUG,MAC_PUSCH,\
            currentGlobalTick,\
            ulRAInfoIndex_g[internalCellIndex],\
            0,0,0,0,\
            0.0,0.0,__func__,"UPDATED_RES_MAP");

    /* ICIC changes end */
    processPUCCHAndPRACHReports(tempPdcchULOutput_p,
            rbmapNum,
            currentGlobalTick  ,
            containerTick,
            ulDelay,
            internalCellIndex);
    /* SPR 5295 changes start*/

    /*CA Changes start  */
    bufferRBs[CC_CE_REGION] = ulBufferPool_g[internalCellIndex].totalRBs;
    bufferRBs[CE_REGION] = ulBufferPool_g[internalCellIndex].cellEdgeRBs;
    /*CA Changes end  */
    /* SPR 5295 changes end*/
    /* SPS chnages start */
    /*SPR 6925 Optimization Start*/ 
    if((TRUE == initParams_p-> spsSysWideParam.enableSps) && 
            (tempPdcchULOutput_p->pdcchULNewUEInfo[CC_USER].count > 0))
        /*SPR 6925 Optimization End*/ 
    {
        processULSPSUeInPUSCH(ulDelay,
                containerTick,
                /* +- SPR 17777 */
                currentGlobalTick,
                /* +- SPR 17777 */
                /* CR changes start */
                hiIndex,
                /* CR changes end */
                &bufferRBs[CC_USER],
                CC_USER, internalCellIndex,execArgs);
    }

    /*===================================================================*/
    /* ICIC changes start */
    /*SPR 6925 Optimization Start*/ 
    if ((TRUE == initParams_p->spsSysWideParam.enableSps) && 
            (tempPdcchULOutput_p->pdcchULNackSpsUEInfo.nonAdaptiveCount > 0))
        /*SPR 6925 Optimization End*/ 
    {
        /* 
           We also try to reserve for Future, this arrangement has been done to cater
           to the non adaptive re-transmission of the UE. The 6 below denotes the 
           present subframe for which it is working while the 8 denotes the future
           reserve subframe.
           */

        //processAdvancePUCCHAndPRACHReports(rbmapNumAdvanced, globalTickAfterTwelveFrames);

        /* We first cater the Non Adaptive Retx for both type of users */
        processULNackSPSNonAdaptiveInPUSCH(ulDelay,
                rbmapNum,
                currentGlobalTick,
                &numberOfULUEsToBeScheudled,
                containerTick,
                /* CR changes start */
                hiIndex,
                /* CR changes end */
                subFrameNum,
                internalCellIndex);
    }
    /*SPR 6925 Optimization Start*/ 
    if ((TRUE == initParams_p->spsSysWideParam.enableSps) &&
            (tempPdcchULOutput_p->pdcchULNackSpsUEInfo.adaptiveCount[CC_USER] > 0))
        /*SPR 6925 Optimization End*/ 
    {
        processULNackSPSAdaptiveInPUSCH(ulDelay,
                rbmapNum,
                currentGlobalTick,
                &numberOfULUEsToBeScheudled,
                containerTick,
                /* CR changes start */
                hiIndex,
                /* CR changes end */
                subFrameNum,
                CC_USER,
                internalCellIndex);
    }



    /* ===================================================================   */



#ifdef PERF_STATS
    /*EVENT :EVENT_PARAM_AVAILABLE_UL_RES , Aggregating Available PRBs
     * available during a given sampling time*/
    /* + PERF_CA */
    gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedULPerfStats.totalULPRBAvailable += \
                                                                                              /*CA Changes start  */
                                                                                              getFreePUSCHResources(rbmapNum, internalCellIndex);
    /*CA Changes end  */
    /* - PERF_CA */
#endif

    /** TTIB_Code Start */
    if (tempPdcchULOutputRetx_p->pdcchULNackTtiBundledUEInfo.nonAdaptiveCount > 0)
    {
        /* We first cater the Non Adaptive Retx for both type of users */
        processULNackNonAdaptiveBundleInPUSCH(ulDelay,
                rbmapNum,
                currentGlobalTick,
                /* +- SPR 17777 */
                containerTickForBundleRetx,
                subFrameNum,
                internalCellIndex);
    }
    /* We now cater Adaptive Retx for Cell Edge user */
    if (tempPdcchULOutputRetx_p->pdcchULNackTtiBundledUEInfo.adaptiveCount[CE_USER] > 0)
    {
        processULNackAdaptiveBundleInPUSCH(ulDelay,
                rbmapNum,
                currentGlobalTick,
                /* +- SPR 17777 */
                containerTickForBundleRetx,
                subFrameNum,
                CE_USER,
                internalCellIndex);
    }

    if (tempPdcchULOutputRetx_p->pdcchULNackTtiBundledUEInfo.adaptiveCount[CC_USER] > 0)
    {
        processULNackAdaptiveBundleInPUSCH(ulDelay,
                rbmapNum,
                currentGlobalTick,
                /* +- SPR 17777 */
                containerTickForBundleRetx,
                subFrameNum,
                CC_USER,
                internalCellIndex);
    }
    tempPdcchULOutputRetx_p->pdcchULNackTtiBundledUEInfo.adaptiveCount[CC_USER] = 0;
    tempPdcchULOutputRetx_p->pdcchULNackTtiBundledUEInfo.adaptiveCount[CE_USER] = 0;
    tempPdcchULOutputRetx_p->pdcchULNackTtiBundledUEInfo.nonAdaptiveCount = 0;

    /** TTIB_Code End */

    /* ICIC changes start */
    if ((tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount > 0) || 
            (tempPdcchULOutput_p->pdcchULNackTempUEInfo.nonAdaptiveCount > 0))
    {
        /* 
           We also try to reserve for Future, this arrangement has been done to cater
           to the non adaptive re-transmission of the UE. The 6 below denotes the 
           present subframe for which it is working while the 8 denotes the future
           reserve subframe.
           */

        //processAdvancePUCCHAndPRACHReports(rbmapNumAdvanced, globalTickAfterTwelveFrames);

        /* We first cater the Non Adaptive Retx for both type of users */
        processULNackNonAdaptiveInPUSCH(ulDelay,
                rbmapNum,
                currentGlobalTick,
                &numberOfULUEsToBeScheudled,
                containerTick,
                /* CR changes start */
                hiIndex,
                /* CR changes end */
                subFrameNum,
                internalCellIndex);
    }
    /* We now cater Adaptive Retx for Cell Edge user */
    if ((tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount[CE_USER] > 0) ||
            (tempPdcchULOutput_p->pdcchULNackTempUEInfo.adaptiveCount[CE_USER] ))
    {
        processULNackAdaptiveInPUSCH(ulDelay,
                rbmapNum,
                currentGlobalTick,
                &numberOfULUEsToBeScheudled,
                containerTick,
                /* CR changes start */
                hiIndex,
                /* CR changes end */
                subFrameNum,
                CE_USER,
                internalCellIndex);
    }

    if ((tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount[CC_USER] > 0) ||
            (tempPdcchULOutput_p->pdcchULNackTempUEInfo.adaptiveCount[CC_USER] > 0))
    {
        processULNackAdaptiveInPUSCH(ulDelay,
                rbmapNum,
                currentGlobalTick,
                &numberOfULUEsToBeScheudled,
                containerTick,
                /* CR changes start */
                hiIndex,
                /* CR changes end */
                subFrameNum,
                CC_USER,
                internalCellIndex);
    }

    /* We now cater to the Acks */
    processULAckInPUSCH(ulDelay,
            /* SPR 2446 Fix Begins */
            /* SPR 2446 Fix End */
            currentGlobalTick,
            /* CR changes start */ 
            resetHiIndex,
            /* CR changes end */
            containerTick,
            internalCellIndex);

    /*UL_MU_MIMO_CHG Start*/
    /*If aggregateMaxRBsRequired is more than total RBs than try for sharing the resources in Uplink using Multi-User MIMO*/
    if (aggregateMaxRBsRequired_g[internalCellIndex] > cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->ulAvailableRBs)
    {
        createBuddyPairFromCandidate(tempPdcchULOutput_p, /*pointer of PdcchULOutputInfo*/
                currentGlobalTick + ulDelay, /*for the calculation of tpc, used for power check*/
                /*CA Changes start  */
                internalCellIndex);
        /*CA Changes end  */
    }
    /*UL_MU_MIMO_CHG End*/

    /* We now cater to the Newly Scheduled Cell Edge UEs */
    if(tempPdcchULOutput_p->pdcchULNewUEInfo[CE_USER].count > 0) 
    {
        processULNewUeInPUSCH(ulDelay, 
                containerTick,
                /* +- SPR 17777 */
                currentGlobalTick,
                &numberOfULUEsToBeScheudled,
                /* CR changes start */ 
                hiIndex,
                /* CR changes end */ 
                &bufferRBs[CE_USER],
                CE_USER, internalCellIndex,execArgs);
        /* SPR 5295 changes start*/
        /*CA Changes start  */
        bufferRBs[CC_USER] -= (ulBufferPool_g[internalCellIndex].cellEdgeRBs - bufferRBs[CE_USER]);
        /*CA Changes end  */
        /* SPR 5295 changes end*/
    }
    /* SPR 16753 fix start */
    else
    {
        updatepdcchULNackUEInfoTempUEInfo( &(pdcchULOutputInfo_gp[internalCellIndex][containerTick]),
                CE_USER, PNULL);
    }
    /* SPR 16753 fix end */
    /* We now cater to the Newly Scheduled Cell Center UEs */
    if(tempPdcchULOutput_p->pdcchULNewUEInfo[CC_USER].count > 0) 
    {
        processULNewUeInPUSCH(ulDelay, 
                containerTick,
                /* +- SPR 17777 */
                currentGlobalTick,
                &numberOfULUEsToBeScheudled,
                /* CR changes start */ 
                hiIndex,
                /* CR changes end */ 
                &bufferRBs[CC_USER],
                CC_USER, internalCellIndex,execArgs);
    }
    /* SPR 16753 fix start */
    else
    {
        updatepdcchULNackUEInfoTempUEInfo( &(pdcchULOutputInfo_gp[internalCellIndex][containerTick]),
                CC_USER, PNULL);
    }
    /* SPR 16753 fix end */

    /* ICIC changes end */
    resetPhichSeqPerIndex(resetHiIndex, internalCellIndex);
    /*GCC_4.7 Warning Fix Start*/
#ifdef KPI_STATS
    /*GCC_4.7 Warning Fix End*/
    /*Update UL PRB usage statistic*/
    totalUplinkPRBUsage = (ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->totalAvailableRB -\
                          /*CA Changes start  */
                          getFreePUSCHResources(rbmapNum, internalCellIndex);
    /*CA Changes end  */
    /* + KPI_CA */
    LTE_MAC_UPDATE_STATS_COMM_KPI(totalUplinkPRBUsage, activeIndexForMacStats,internalCellIndex);
    /* - KPI_CA */
#endif
    /* +DYNAMIC_ICIC */
    rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.ulICICReportInfo_p \
        [rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndex].ulCEPrbUsage +=
        ((ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->totalCellEdgeRB -\
         /*CA Changes start  */
         (rbMapNodeArr_gp[internalCellIndex] + rbmapNum)->availableResourceInfo[CE_REGION].freeRBCounter);
    /*CA Changes end  */

    /* -DYNAMIC_ICIC */

    resetRBsAtLocation(rbmapNum,internalCellIndex);

    /* We now encode the TPC BITMAP for DCI FORMAT 3/3A scheduled for 
       PUSCH power control */
            encodeTpcBitmapForPusch (containerTick, 
                    &tempPdcchULOutput_p->pdcchTpcMsgInfoForPusch,
                    (currentGlobalTick + ulDelay ),
                    internalCellIndex);

        /* SPR 8712 changes */
#if (defined(DL_DATA_SEPARATION) && (DL_UL_SPLIT))
        eventfd_write_wrapper(ev_fd_pusch_g,1);
#endif
        /* SPR 8712 changes */
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Exit...... \n",__func__);
        return MAC_SUCCESS; 
}

#elif TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129107) */
/********************************************************************************
 * Function Name     : processPUSCHLogicForDelayPath2
 * Inputs            : ulSubFrameNum - Uplink subframe number,
 *                     currentGlobalTick - The present Global Tick,
 *                     SFN - System Frame number,
 *                     subFrameNum - Subframe number
 *                     internalCellIndex - Cell-Index at MAC
 * Outputs           : None
 * Returns           : MAC_SUCCESS /MAC_FAILURE
 * Description       : This function will work for delay path 2 on per node basis  
 *                     This would be the main calling function of the entire PUSCH
 *                     logic in case of delay path 2. This function process te uplink 
 *                     allocation in this order
 *                                1.NonApadtive for both types of UEs
 *                                2.Cell Edge Adaptive
 *                                3.Cell Center Adaptive
 *                                4.Cell Edge New Txn
 *                                5.Cell Center New Txn
 *                     and then the ACKs.  
 *********************************************************************************/
MacRetType processPUSCHLogicForDelayPath2( UInt8 ulSubFrameNum,
        /* SPR 5599 changes end (ZIP ID 129107) */
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt16 SFN,
        UInt8  subFrameNum,
        InternalCellIndex internalCellIndex,
        ExecutionLegFunctionArg* execArgs
        )
{
    UInt8 containerTick      = 0;
    UInt32 rbmapNum          = 0;
    /*UInt8 rbmapNumAdvanced   = 0;TDD warning Fix */
    UInt8 ulDelay            = 0;
    UInt16 numberOfULUEsToBeScheudled = cellSpecificParams_g.\
                                        cellConfigAndInitParams_p[internalCellIndex]->initParams_p->
                                        ulSchdConfig.maxUEsToBeScheduledUL;
    UInt8   tdd0SecondUSubframe= FALSE; 
    /* TDD Config 0 start */
    UInt8 prevUlGrantSubFrameNum = 0;
    UInt16 pdcchULOutputInfoIndex = 0;
    UInt8 ulSubframeOffset = 0;
#if defined KPI_STATS
    UInt8 totalUplinkPRBUsage;
#endif
    /* TDD Config 0 end */
    /*SPR 5620 START*/
    UInt16 ulSysFrameNum;
    /*TDD Config 0 Changes Start*/
    RbMapNode *rbMapNode_p = PNULL;
    /*TDD Config 0 Changes End*/ 

    if ( ulSubFrameNum < subFrameNum )
    {
        ulSysFrameNum = SFN+1;
        if ( ulSysFrameNum > MAX_SYS_FRAME_NUM )
        {
            ulSysFrameNum = 0;
        }
    } else {
        ulSysFrameNum = SFN;
    }

    /* TDD Config 0 Changes Start
     * We update the value of ulSubframeOffset with ULTX_OFFSET, and set tdd0SecondUSubframe 
     * as TRUE, for second execution of PUSCH at +7 U-frame.
     */
    pdcchULOutputInfoIndex = MODULO_TWO(ulSysFrameNum);
    /*SPR 5620 END*/
    ulSubframeOffset = getExactULSubFrame(subFrameNum, internalCellIndex);
    if (((subFrameNum + ulSubframeOffset)% 
                MAX_SUB_FRAME) != ulSubFrameNum )
    {
#ifdef MAC_AUT_TEST
        secondExecutionPUSCH = TRUE;
#endif
        tdd0SecondUSubframe= TRUE; 
        ulSubframeOffset = ULTX_OFFSET; 
    }
    /* TDD Config 0 Changes End */
    /* ICIC changes start */
    SInt8 bufferRBs[MAX_USER_LOCATION] = {0};
    /* ICIC changes end */
#ifdef KPI_STATS
    /* + KPI_CA */
    UInt32 activeIndexForMacStats = (gMacStats.pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    /* - KPI_CA */
#endif 
    /*
       we calculate the value of pdcchUpLinkOutputContainerTick, hiPduContainerTick .dciCCEContainerTick
       All ticks are with respect to the tick when the information is to be sent to PHY interface.
       */

    /* TDD Config 0 Changes Start */
    /* Calculate the rbmapNum, based on the ulSubframeOffset */ 
    rbmapNum = (currentGlobalTick + ulSubframeOffset)% getMaxULRBMaps(internalCellIndex);
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbmapNum;
    /*
     * For TDD Config 0, incase of special subframe, for first execution of PUSCH at +k frame, 
     * when tdd0SecondUSubframe is false, we deduct the nackQueueCount from the 
     * ulSchdConfig_g.maxUEsToBeScheduledUL, to get numberOfULUEsToBeScheudled, whereas for all 
     * other configs and for +k execution of Config 0, with tdd0SecondUSubframe as TRUE, we set 
     * numberOfULUEsToBeScheudled, with maxUEsToBeScheduledUL.
     */
                                        if((UL_DL_CONFIG_0 == getTddConfig(internalCellIndex)) &&
                                                (SP == getSubframeType(subFrameNum, internalCellIndex)) &&
                                                (FALSE == tdd0SecondUSubframe))
                                        {
                                            numberOfULUEsToBeScheudled = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig.maxUEsToBeScheduledUL
                                                - rbMapNode_p->nackQueueCount;
                                        }
                                        else
                                        {
                                            numberOfULUEsToBeScheudled = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
                                        }
    /* TDD Config 0 Changes End */

    PdcchULOutputInfo* tempPdcchULOutput_p = 
        &pdcchULOutputInfo_g[internalCellIndex][pdcchULOutputInfoIndex][ulSubFrameNum]; 

    containerTick = (currentGlobalTick )% MAX_PDCCH_CONTAINER;

    /*
       call the function for pucch and get the pucchUEColissionArray[]  populated by the entries.
       */
        /* ICIC changes start */

        LOG_MAC_MSG(MAC_ULSCH_RA_INFO_INDEX,LOGDEBUG,MAC_PUSCH,\
                currentGlobalTick,\
                ulRAInfoIndex_g[internalCellIndex],\
                0,0,0,0,\
                0.0,0.0,__func__,"UPDATED_RES_MAP");

        /* ICIC changes end */
        /* TDD Config 0 Changes Start */
        /* We will not call processPUCCHAndPRACHReports incase of TDD Config 0, D subframe,
         * and +7 execution when tdd0SecondUSubframe is TRUE, for rest of the cases we process 
         * PUCCH and PRACH Reports.
         */ 
        if(!((DL == getSubframeType(subFrameNum,internalCellIndex)) && ( TRUE == tdd0SecondUSubframe )))
        {
#ifdef MAC_AUT_TEST
            callProcessPUCCHAndPRACHReports = TRUE;	
#endif
            /* TDD Config 0 Changes End */	
            processPUCCHAndPRACHReports(tempPdcchULOutput_p,
                    rbmapNum,
                    currentGlobalTick,
                    ulSubFrameNum,
                    internalCellIndex);
        }
    /*TDD Config 0 Changes Start*/ 
    /*Rb partition information is now available in RbMap*/
    /* SPR 5295 changes start*/
    bufferRBs[CC_CE_REGION] = rbMapNode_p->ulAvailRB.totalRBs;
    bufferRBs[CE_REGION] = rbMapNode_p->ulAvailRB.cellEdgeRBs;
    /* SPR 5295 changes end*/
    /*TDD Config 0 Changes End*/

#ifdef PERF_STATS
    /*EVENT :EVENT_PARAM_AVAILABLE_UL_RES , Aggregating Available PRBs
     * available during a given sampling time*/
    gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedULPerfStats.totalULPRBAvailable += \
                                                                                              /*CA Changes start  */
                                                                                              getFreePUSCHResources(rbmapNum, internalCellIndex);
    /*CA Changes end  */
#endif

    /* + SPS_TDD_Changes */
    if((TRUE ==  cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam.enableSps) && (tempPdcchULOutput_p->pdcchULNewUEInfo[CC_USER].count > 0))
    {
        processULSPSUeInPUSCH(tempPdcchULOutput_p, 
                containerTick,
                currentGlobalTick,
                subFrameNum,
                rbmapNum,
                /*SPR 5620 START*/
                ulSysFrameNum,
                /*SPR 5620 END*/
                ulSubFrameNum,
                &bufferRBs[CC_USER],
                CC_USER,
                /*CA Changes start  */
                internalCellIndex,execArgs);
        /*CA Changes end  */
    }
    if ((TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam.enableSps) && (tempPdcchULOutput_p->pdcchULNackSpsUEInfo.nonAdaptiveCount > 0))
    {
        processULNackSPSNonAdaptiveInPUSCH(tempPdcchULOutput_p,
                rbmapNum,
                subFrameNum,
                currentGlobalTick,
                &numberOfULUEsToBeScheudled,
                containerTick,
                internalCellIndex,
                /*TDD Config 0 Changes Start*/ 
                ulSubFrameNum);
        /*TDD Config 0 Changes End*/
    }
    if ((TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam.enableSps) && (tempPdcchULOutput_p->pdcchULNackSpsUEInfo.adaptiveCount[CC_USER] > 0))
    {
        processULNackSPSAdaptiveInPUSCH(tempPdcchULOutput_p,
                rbmapNum,
                subFrameNum,
                currentGlobalTick,
                &numberOfULUEsToBeScheudled,
                containerTick,
                CC_USER,
                internalCellIndex,
                /*TDD Config 0 Changes Start*/ 
                ulSubFrameNum);
        /*TDD Config 0 Changes End*/
    }

    /* - SPS_TDD_Changes */

    /* ICIC changes start */
    if ( ( tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount > 0 ) ||
            /* msg3_retx_tdd_support_start */
            ( tempPdcchULOutput_p->pdcchULNackTempUEInfo.nonAdaptiveCount > 0 ) )
        /* msg3_retx_tdd_support_end */
    {
        /* 
           We also try to reserve for Future, this arrangement has been done to cater
           to the non adaptive re-transmission of the UE. The 6 below denotes the 
           present subframe for which it is working while the 8 denotes the future
           reserve subframe.
           */

        //processAdvancePUCCHAndPRACHReports(rbmapNumAdvanced, globalTickAfterTwelveFrames);

        /* We first cater the Non Adaptive Retx for both type of users */
        processULNackNonAdaptiveInPUSCH(tempPdcchULOutput_p,
                rbmapNum,
                subFrameNum,
                currentGlobalTick,
                &numberOfULUEsToBeScheudled,
                containerTick,
                internalCellIndex,
                /*TDD Config 0 Changes Start*/ 
                ulSubFrameNum);
        /*TDD Config 0 Changes End*/
    }
    /* We now cater Adaptive Retx for Cell Edge user */
    if ((tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount[CE_USER] > 0) ||
            /* msg3_retx_tdd_support_start */
            (tempPdcchULOutput_p->pdcchULNackTempUEInfo.adaptiveCount[CE_USER] > 0))
        /* msg3_retx_tdd_support_end */
    {
        processULNackAdaptiveInPUSCH(tempPdcchULOutput_p,
                rbmapNum,
                subFrameNum,
                currentGlobalTick,
                &numberOfULUEsToBeScheudled,
                containerTick,
                CE_USER,
                internalCellIndex,
                /*TDD Config 0 Changes Start*/ 
                ulSubFrameNum);
        /*TDD Config 0 Changes End*/
    }

    if ((tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount[CC_USER] > 0) ||
            /* msg3_retx_tdd_support_start */
            (tempPdcchULOutput_p->pdcchULNackTempUEInfo.adaptiveCount[CC_USER] > 0))
        /* msg3_retx_tdd_support_end */
    {
        processULNackAdaptiveInPUSCH(tempPdcchULOutput_p,
                rbmapNum,
                subFrameNum,
                currentGlobalTick,
                &numberOfULUEsToBeScheudled,
                containerTick,
                CC_USER,
                internalCellIndex,
                /*TDD Config 0 Changes Start*/ 
                ulSubFrameNum);
        /*TDD Config 0 Changes End*/
    }

    /* We now cater to the Acks */
    processULAckInPUSCH(ulDelay,
            /* SPR 2446 Fix Begins */
            /* SPR 2446 Fix End */
            currentGlobalTick, 
            containerTick,
            internalCellIndex);

    /*UL_MU_MIMO_CHG Start*/
    /*If aggregateMaxRBsRequired is more than total RBs than try for sharing the resources in Uplink using Multi-User MIMO*/
    if (rbMapNode_p->aggregateMaxRBsRequired > 
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->ulAvailableRBs)
    {
        createBuddyPairFromCandidate(tempPdcchULOutput_p, /*pointer of PdcchULOutputInfo*/
                currentGlobalTick,/*for the calculation of tpc, used for power check*/
                /* +- SPR 17777 */
                internalCellIndex,
                ulSubFrameNum);
    }
    /*UL_MU_MIMO_CHG End*/

    /* We now cater to the Newly Scheduled Cell Edge UEs */
    if(tempPdcchULOutput_p->pdcchULNewUEInfo[CE_USER].count > 0) 
    {
        processULNewUeInPUSCH(tempPdcchULOutput_p, 
                containerTick,
                currentGlobalTick,
                subFrameNum,
                rbmapNum,
                /*SPR 5620 START*/
                ulSysFrameNum,
                /*SPR 5620 END*/
                ulSubFrameNum,
                &numberOfULUEsToBeScheudled,
                &bufferRBs[CE_USER],
                CE_USER, internalCellIndex,execArgs);
        /* SPR 5295 changes start*/
        /* TDD Config 0 Changes Start */
        bufferRBs[CC_USER] -= (rbMapNode_p->ulAvailRB.cellEdgeRBs - bufferRBs[CE_USER]);
        /* TDD Config 0 Changes End */
        /* SPR 5295 changes end*/
    }
    /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
    /* SPR 20039 Fix ++ */
    else
    {
        tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount[CE_USER]  = 0;
    }
    /* SPR 20039 Fix -- */
    /* SPR 19679: TDD HARQ Multiplexing  Changes End */
    /* We now cater to the Newly Scheduled Cell Center UEs */

    if(tempPdcchULOutput_p->pdcchULNewUEInfo[CC_USER].count > 0) 
    {
        processULNewUeInPUSCH(tempPdcchULOutput_p, 
                containerTick,
                currentGlobalTick,
                subFrameNum,
                rbmapNum,
                /*SPR 5620 START*/
                ulSysFrameNum,
                /*SPR 5620 END*/
                ulSubFrameNum,
                &numberOfULUEsToBeScheudled,
                &bufferRBs[CC_USER],
                CC_USER, internalCellIndex,execArgs);
    }
    /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
    /* SPR 20039 Fix ++ */
    else
    {
        tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount[CC_USER]  = 0;
        tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount  = 0;
    }
    /* SPR 20039 Fix -- */
    /* SPR 19679: TDD HARQ Multiplexing  Changes End */
#ifdef KPI_STATS
    totalUplinkPRBUsage = (ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->totalAvailableRB -\
                          /*CA Changes start  */
                          getFreePUSCHResources(rbmapNum, internalCellIndex);
    /*CA Changes end  */
    /* + KPI_CA */
    LTE_MAC_UPDATE_STATS_COMM_KPI(totalUplinkPRBUsage, activeIndexForMacStats,internalCellIndex);
    /* - KPI_CA */
#endif

    /* SPR 5983 Fix Start */
    /* +DYNAMIC_ICIC */
    /* TDD Config 0 Changes Start */
    /* We update ICIC Report Info everytime except for TDD Config 0, 
     * Dl subframe, and +7 execution when tdd0SecondUSubframe is TRUE.
     * For all other scenarios, we update ICIC Reports.
     */
    if(!((getSubframeType(subFrameNum,internalCellIndex) == DL) && (tdd0SecondUSubframe == TRUE)) )
    {
        rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->icicReportInd.ulICICReportInfo_p \
            [rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->activeReportIndex].ulCEPrbUsage +=
            ((ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->totalCellEdgeRB -\
             /*CA Changes start  */
             (rbMapNodeArr_gp[internalCellIndex] + rbmapNum)->availableResourceInfo[CE_REGION].freeRBCounter);
        /*CA Changes end  */

        /* -DYNAMIC_ICIC */
        /* SPR 5983 Fix End */
        resetRBsAtLocation(rbmapNum,internalCellIndex);
    }

    /* TDD Config 0 Changes Start */
    /* We reset PHICH, for all configs except for TDD Config 0, and second Execution 
     * of PUSCH at +7 U-frame.
     */
            if (!((UL_DL_CONFIG_0 == getTddConfig(internalCellIndex)) && (tdd0SecondUSubframe == FALSE)))
            {
#ifdef MAC_AUT_TEST
                resetPhichInfo = TRUE;	
#endif
                /* TDD Config 0 Changes End */
                resetPhichSeqPerIndex(subFrameNum, internalCellIndex);    
            }
        /* We now encode the TPC BITMAP for DCI FORMAT 3/3A scheduled for 
           PUSCH power control */
        encodeTpcBitmapForPusch (containerTick, 
                &tempPdcchULOutput_p->pdcchTpcMsgInfoForPusch,
                currentGlobalTick,
                internalCellIndex
                /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /*TDD Config 0 Changes End*/ 
                );
        /* Tdd Config 0 Chnages Start */
        /* If TDD config 0 and 2nd PUSCH call at S SF
         * then delete the scheduluedLCList elements */
    if(UL_QOS_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->
            initParams_p->ulSchdConfig.ulSchedulerStrategy)
    {
        if ((UL_DL_CONFIG_0 == getTddConfig(internalCellIndex))
                && ((SUB_FRAME_3 == ulSubFrameNum) || 
                    (SUB_FRAME_8 == ulSubFrameNum)))
        {
#ifdef MAC_AUT_TEST
            deleteScheduleLCNodeInfo = TRUE; 
#endif
            /* prevUlGrantSubFrameNum is offset from current subFrameNum */
            prevUlGrantSubFrameNum = ulSubFrameNum - 1; 
            deleteScheduledLcNode(prevUlGrantSubFrameNum,internalCellIndex);
        }
    }
    /* Tdd Config 0 Chnages End */

    return MAC_SUCCESS; 
}


/* +- SPR 17777 */

#endif

#ifdef FDD_CONFIG
/********************************************************************************
 * Function Name     : processULGrants
 * Inputs            : ulGrantInfo -
 *                     currentGlobalTick - The present Global Tick,
 *                     tempPUSCHResources_p,
 *                     bufferRBs_p,
 *                     tpc,
 *                     ulDelay - The amount of delay expected in the UpLink path,
 *                     ueMaxPowerFlag,
 *                     hiIndex,
 *                     ueMinPowerFlag,
 *                     spsAllocFlag
 *                     internalCellIndex - Cell-Index at MAC
 * Outputs           : None
 * Returns           : MAC_SUCCESS /MAC_FAILURE
 * Description       : This function will work on per node basis and try to  
 *                     allocate resources in advance for the next tick also. 
 ********************************************************************************/
/* SPR 5599 changes start (ZIP ID 129107) */
MacRetType processULGrants(ULGrantInfo* ulGrantInfo,
        /* SPR 5599 changes end (ZIP ID 129107) */
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick, 
        /* SPR 15909 fix end */
        UInt32* tempPUSCHResources_p,
        SInt8 *bufferRBs_p,
        /* Time Averaging changes start */
        TpcTriggeredToPowerCorrection tpc,
        /* Rel9_upgrade_CR410 */
        UInt8 ulDelay,
        /* Rel9_upgrade_CR410 */
        UInt32 *ueMaxPowerFlag,
        /* Time Averaging changes end */
        /* CR changes start */
        UInt8 hiIndex,
        UInt32 *ueMinPowerFlag, 
        /* CR changes end */
        UInt8 spsAllocFlag,
        /*CA Changes start */
        InternalCellIndex internalCellIndex,
        /*CA Changes end */
        UInt8 subFrameNum,
        ExecutionLegFunctionArg* execArgs)

{
    SInt16 allocatedTBSize;
    UInt32 localPUSCHResources = *tempPUSCHResources_p;
    AllocatedResInfo resourceReserved = {0};
    ULUEContext* ulUEContext_p = ulGrantInfo->ulUEContext_p;
    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p = \
                                                        ulGrantInfo->tempDirectIndexingUEInfo_p;
    DciCCEInfo* tempDciCCEInfo_p = ulGrantInfo->tempDciCCEInfo_p;
    UInt8 subFrameNumber = ulGrantInfo->subFrameNumber;
    UInt8 tempHarqProcessId = ulGrantInfo->tempHarqProcessId;
    /* +- SPR 18268 */
    UInt16 tempUeIndex = ulGrantInfo->tempUeIndex;
    /* +- SPR 18268 */
    /* UL_MU_MIMO_CHG Start */
    UInt16       buddyUeIndex       = tempDirectIndexingUEInfo_p->buddyUeIndex;
    ULUEContext* ulBuddyUEContext_p = PNULL;
    if(( buddyUeIndex < MAX_UE_SUPPORTED) && (PNULL != ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p))
    {
        ulBuddyUEContext_p = ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p;
    }
    /* UL_MU_MIMO_CHG End */
    /*  CSI_P2_PUSCH_CHANGES start */
    UInt8 ueServCellIndex = PCELL_SERV_CELL_IX;
    /*  CSI_P2_PUSCH_CHANGES end */

    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Entry...... \n",__func__);

    /* + CQI_5.0 */
    UInt32 tempMinDataSize = 0;
    /*  CSI_P2_PUSCH_CHANGES start */
    UInt32 aperiodicReportDataSize = 0;
    if(ulUEContext_p->aperiodicCQIToBeRequested)
    {

        if(ulUEContext_p->aperiodicCQIToBeRequested & servingCellBitMask_g[PCELL_SERV_CELL_IX])
        {
            /* aperiodic csi is requested for Pcell */
            determineRBsForAperiodicCQIReports(ulUEContext_p,
                    &tempMinDataSize,
                    internalCellIndex);
            aperiodicReportDataSize += tempMinDataSize;
        }
        /* Now checking if aperiodic report is requested for Scell also in case
         * of CA UE */
        for(ueServCellIndex = START_SCELL_INDEX; ueServCellIndex <= ulUEContext_p->scellCount;
                ueServCellIndex++)
        {
            if(ulUEContext_p->aperiodicCQIToBeRequested & servingCellBitMask_g[ueServCellIndex])
            {
                tempMinDataSize = 0;
                if(PNULL != ulUEContext_p->ulUeScellContext_p[ueServCellIndex])
                {
                    determineRBsForAperiodicCQIReports(ulUEContext_p,
                            &tempMinDataSize,
                            ulUEContext_p->
                            ulUeScellContext_p[ueServCellIndex]->internalCellIndex
                            );
                    aperiodicReportDataSize += tempMinDataSize;
                }
            }
        }
        /*TDD SB_CQI */
        if(aperiodicReportDataSize > ulGrantInfo->pdcchULNewUE_p->maxDataSize)
        {
            ulGrantInfo->pdcchULNewUE_p->minDataSize = ulGrantInfo->pdcchULNewUE_p->maxDataSize;
        }
        else if(aperiodicReportDataSize > ulGrantInfo->pdcchULNewUE_p->minDataSize)
        {
            ulGrantInfo->pdcchULNewUE_p->minDataSize = aperiodicReportDataSize;
        }
        /*TDD SB_CQI */
    }
    /*  CSI_P2_PUSCH_CHANGES end */

    /* - CQI_5.0 */

    /* UL_MU_MIMO_CHG Start */
    if ( (localPUSCHResources) || 
            ( (buddyUeIndex < MAX_UE_SUPPORTED) &&
              (PNULL != ulBuddyUEContext_p) &&
              (ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated)) )
        /* UL_MU_MIMO_CHG End */
    {
        switch (tempDirectIndexingUEInfo_p->ulGrantRequestType)
        {
            case SR_ULGRANT_REQ_MSG:
            case CQI_WITHOUT_DATA_ULGRANT_REQ_MSG:
            case CQI_WITH_DATA_ULGRANT_REQ_MSG:
            case CR_ULGRANT_REQ_MSG:                      
            case UL_TPC_WITHOUT_DATA:
            case UL_EXPLICIT_PHR_WITHOUT_DATA:
            case UL_TPC_WITH_DATA:
            case NEW_ULGRANT_REQ_MSG:
            case UL_SPS_ACTIVATION:
            case UL_SPS_DEACTIVATION:
            case UL_SPS_OCCASION:
            case NEW_TTIB_ULGRANT_REQ_MSG:
            case TTIB_UL_SPS_OCCASION:
            LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, 
                    "[%s] NEW_ULGRANT_REQ_MSG || SR_ULGRANT_REQ_MSG ||"
                    " CQI_WITHOUT_DATA_ULGRANT_REQ_MSG || "
                    " CQI_WITH_DATA_ULGRANT_REQ_MSG || CR_ULGRANT_REQ_MSG ",__func__);

            tempDirectIndexingUEInfo_p->harqIndex = tempHarqProcessId;
            /* DRX Log Start */
        /*SPR 22908 Changes Start*/
            if (ulUEContext_p->drxCurrentState == DRX_STATE_ON && tempDirectIndexingUEInfo_p->ulGrantRequestType!=SR_ULGRANT_REQ_MSG)
            {
        /*SPR 22908 Changes End*/
                LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                        LOGWARNING,MAC_UL_Strategy,\
                        currentGlobalTick,\
                        ulUEContext_p->ueIndex,\
                        tempDirectIndexingUEInfo_p->ulGrantRequestType,__LINE__,\
                        dlUECtxInfoArr_g[ulUEContext_p->ueIndex].dlUEContext_p->drxContext.drxStateVariables.drxCurrentState,ulUEContext_p->crnti,\
                        0.0,0.0,__func__,
                        "UeSchDRX_STATE_ON");
            }
            /* DRX Log End */

            if(MAC_SUCCESS == allocatePUSCHResourcesForNewUEs(
                        ulUEContext_p,
                        ulGrantInfo->pdcchULNewUE_p,
                        tempDirectIndexingUEInfo_p,
                        &resourceReserved,
                        subFrameNumber,
                        localPUSCHResources,
                        /* Time Averaging changes start */
                        /* SPR 5295 changes start*/
                        bufferRBs_p, tpc, ueMaxPowerFlag, ueMinPowerFlag, spsAllocFlag
                        /* SPR 5295 changes end*/
                        /* UL - MIMO Changes Start */
                        , hiIndex, currentGlobalTick,ulDelay, internalCellIndex))
                /* UL - MIMO Changes End */
                /* Time Averaging changes end */
            {
                if ( (FALSE == spsAllocFlag) &&
                        ( (tempDirectIndexingUEInfo_p->ulGrantRequestType == UL_SPS_ACTIVATION) ||
                          (tempDirectIndexingUEInfo_p->ulGrantRequestType == UL_SPS_DEACTIVATION) ||
                          (tempDirectIndexingUEInfo_p->ulGrantRequestType == UL_SPS_OCCASION)  
                          || (TTIB_UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType) 
                        ))
                {
                    return MAC_SUCCESS; 
                }       
                if ( (tempDirectIndexingUEInfo_p->ulGrantRequestType == \
                            CQI_WITHOUT_DATA_ULGRANT_REQ_MSG)
                        /* + CQI_5.0 */
                        /* CSI_P2_PUSCH_CHANGES:  == TRUE  removed */
                        || (ulUEContext_p->aperiodicCQIToBeRequested) )
                    /* - CQI_5.0 */
                {
                    /* CSI_P2_PUSCH_CHANGES start */
                    tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].dciPduInfo.cqiRequest = 
                        ulUEContext_p->aperiodicCQIToBeRequested; 
                    /* store value of bitmap aperiodicCQIToBeRequested in
                     * ulUEContext_p->reportFormatMap which will be used at
                     * receiver while decoding the reports */
                    /*Fix for SPR 12477, updating index with subFrameNum*/   
                    ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType = 
                        ulUEContext_p->aperiodicCQIToBeRequested;
                    /* CSI_P2_PUSCH_CHANGES end */
                    ulUEContext_p->lastTriggeredAperiodicTTI = (currentGlobalTick + ulDelay + 4);
                }
                else
                {
                    tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].dciPduInfo.cqiRequest = 0;
                }  

                tempDirectIndexingUEInfo_p->flag = ALREADY_RESOURCES_ALLOCATED;

                /* UL_MU_MIMO_CHG Start */
                /* If resources are shared among buddies; then update PUSCH resources 
                 * only for one of the UE */
                if( (buddyUeIndex == INVALID_UE_INDEX) ||
                        ( (buddyUeIndex < MAX_UE_SUPPORTED) && 
                          (PNULL != ulBuddyUEContext_p) &&
                          ( !ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated)) )
                {
                    if (localPUSCHResources > resourceReserved.allocatedLength)
                    {
                        localPUSCHResources -=  resourceReserved.allocatedLength;
                    }
                    else
                    {
                        localPUSCHResources = 0;
                    }
                }
                /* UL_MU_MIMO_CHG End */

                updateDciCCEInfoArray(
                        ulUEContext_p,
                        tempDirectIndexingUEInfo_p->ulCCEIndex,
                        tempHarqProcessId,
                        tempDciCCEInfo_p,
                        &resourceReserved,
                        /* Time Averaging changes start */
                        tpc.tpcCommand,
                        /* Time Averaging changes end */
                        /* +- SPR 17777 */
                        /* CR changes start */
                        hiIndex,
                        /* CR changes end */
                        /* UL_MU_MIMO_CHG Start */
                        tempDirectIndexingUEInfo_p->ulGrantRequestType,
                        tempDirectIndexingUEInfo_p->buddyUeIndex,/*Coverity 96753 fix*/
                        /* UL_MU_MIMOCHG End */
                        internalCellIndex);

                /* UL QOS CHANGE */
                // allocatedTBSize = tempULHarqInfo_p->deltabsrLoad;
                /* + Coverity 31422 */
                if ( resourceReserved.allocatedLength > 0 )
                {
                    /* SPR 18641_18843 start */
                    updateULPHR(ulUEContext_p->ueIndex,resourceReserved.allocatedLength,internalCellIndex);
                    /* SPR 18641_18843 end */
                    allocatedTBSize =
                        determineUlTBSize(
                                ulGrantInfo->pdcchULNewUE_p->avgMCS,
                                resourceReserved.allocatedLength);
                }
                else
                {
                    return MAC_FAILURE;
                }
                /* - Coverity 31422 */

                ulQosUpdateScheduledTokens(ulUEContext_p,
                        allocatedTBSize,tempHarqProcessId,
                        internalCellIndex,execArgs);

                LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_NEW_TX_COUNT
                    (ulUEContext_p->ueIndex);
            }
            else
            {
                if(tempDirectIndexingUEInfo_p->ulGrantRequestType == NEW_TTIB_ULGRANT_REQ_MSG)
                {
                    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] [%d] Resources allocation Fails"\
                            "Resources are not available\n",__func__,getCurrentTick());    
                }
                /* SPR 17211 Fix Start */ 
                if( /*(tempDirectIndexingUEInfo_p->ulGrantRequestType != NEW_ULGRANT_REQ_MSG) &&*/
                        /* SPR 17211 Fix End */ 
                        (tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_ACTIVATION) &&
                        (tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_DEACTIVATION) &&
                        (tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_OCCASION) 
                        && (TTIB_UL_SPS_OCCASION != tempDirectIndexingUEInfo_p->ulGrantRequestType) 
                        &&(tempDirectIndexingUEInfo_p->ulGrantRequestType != NEW_TTIB_ULGRANT_REQ_MSG)
                  )
                    /* TTIB_Code Start */
                {
                    putEntryInPUSCHFailureQueue( 
                            tempUeIndex,
                            tempDirectIndexingUEInfo_p->ttiCounter,
                            tempDirectIndexingUEInfo_p->ulGrantRequestType,
                            internalCellIndex);

                }
                /* TTIB_Code End */
                /* UL QOS CHANGE */
                allocatedTBSize = 0;
                ulQosUpdateScheduledTokens(ulUEContext_p,
                        allocatedTBSize,
                        INVALID_HARQ_ID,
                        internalCellIndex,execArgs);
                return MAC_FAILURE;
            }
            break;
            default:
            LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "Invalid Value "\
                    "Arrived in the  ulGrantRequestType ");    
            /* UL QOS CHANGE */
            allocatedTBSize = 0;
            ulQosUpdateScheduledTokens(ulUEContext_p,
                    allocatedTBSize,
                    INVALID_HARQ_ID,
                    internalCellIndex,execArgs);
            return MAC_FAILURE;
        }





        /* SPR 10027 FIX START */
        /*
           We need to store the Global Tick when this UE was last scheduled
           */
                        //ulUEContext_p->lastScheduledTick = currentGlobalTick;
                        /* SPR 10027 FIX END */
    }
    /* SPR 17211 Fix Start */
    else if( /*(tempDirectIndexingUEInfo_p->ulGrantRequestType != NEW_ULGRANT_REQ_MSG) &&*/
            /* SPR 17211 Fix End */
            (tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_ACTIVATION) &&
            (tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_DEACTIVATION) &&
            (tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_OCCASION) 
            && (TTIB_UL_SPS_OCCASION != tempDirectIndexingUEInfo_p->ulGrantRequestType) 
            &&(tempDirectIndexingUEInfo_p->ulGrantRequestType != NEW_TTIB_ULGRANT_REQ_MSG)
           )
    {
        putEntryInPUSCHFailureQueue( 
                tempUeIndex,
                tempDirectIndexingUEInfo_p->ttiCounter,
                tempDirectIndexingUEInfo_p->ulGrantRequestType,
                internalCellIndex);

        *tempPUSCHResources_p = localPUSCHResources;
        /* UL QOS CHANGE */
        allocatedTBSize = 0;
        ulQosUpdateScheduledTokens(ulUEContext_p,
                allocatedTBSize,
                INVALID_HARQ_ID,
                internalCellIndex,execArgs);
        return MAC_FAILURE;
    }
    else
    {
        /* UL QOS CHANGE */
        allocatedTBSize = 0;
        ulQosUpdateScheduledTokens(ulUEContext_p,
                allocatedTBSize,
                INVALID_HARQ_ID,
                internalCellIndex,execArgs);
        return MAC_FAILURE;        
    }
    *tempPUSCHResources_p = localPUSCHResources;
    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "\nlocalPUSCHResources = %d", localPUSCHResources);
    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Exit...... \n",__func__);



    tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;    
    return MAC_SUCCESS;
}
#elif TDD_CONFIG
/**************************************************************************
 * Function Name     : processULGrants
 * Inputs            : ulGrantInfo - Pointer to ULGrantInfo,
 *                     currentGlobalTick,
 *                     tempPUSCHResources_p,
 *                     bufferRBs_p,
 *                     tpc,
 *                     subFrameNumber,
 *                     ulGrantSubFrameNum,
 *                     ueMaxPowerFlag,
 *                     ueMinPowerFlag
 *                     internalCellIndex - Cell-Index at MAC
 * Outputs           : None
 * Returns           : MAC_SUCCESS/MAC_FAILURE
 * Description       : This function will work on per node basis and try to  
 *                     allocate resources in advance for the next tick also. 
 ***************************************************************************/
/* SPR 5599 changes start (ZIP ID 129107) */
MacRetType processULGrants(ULGrantInfo* ulGrantInfo,
        /* SPR 5599 changes end (ZIP ID 129107) */
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick, 
        /* SPR 15909 fix end */
        UInt32* tempPUSCHResources_p, 
        SInt8 *bufferRBs_p,
        /* Time Averaging changes start */
        TpcTriggeredToPowerCorrection tpc,
        /* Rel9_upgrade_CR410 */
        UInt8 subFrameNumber,
        /* Rel9_upgrade_CR410 */
        /*TDD Config 0 Changes Start*/
        //UInt8 ulGrantSubFrameNum,// TDD SB_CQI
        /*TDD Config 0 Changes End*/
        UInt32 *ueMaxPowerFlag,
        UInt32 *ueMinPowerFlag,
        /* + SPS_TDD_Changes */
        UInt8 spsAllocFlag, 
        /* - SPS_TDD_Changes */
        InternalCellIndex internalCellIndex,
        ExecutionLegFunctionArg* execArgs) 
        /* Time Averaging changes end */
{
    SInt16 allocatedTBSize = 0;
    ULHarqInfo*   tempULHarqInfo_p = PNULL; 
    UInt32 localPUSCHResources     = *tempPUSCHResources_p;
    AllocatedResInfo resourceReserved = {0};
    ULUEContext* ulUEContext_p         = ulGrantInfo->ulUEContext_p;
    UeScheduledInfo *ueScheduledInfo_p = PNULL;
    /*TDD Config 0 Changes Start*/
    UInt8 scheduleCount = 0;
    /*TDD Config 0 Changes Start*/

    DciCCEInfo* tempDciCCEInfo_p = ulGrantInfo->tempDciCCEInfo_p;
    UInt8 rbMapNum       = ulGrantInfo->rbMapNum;
    UInt8 ulSubFrameNum  = ulGrantInfo->ulSubframe;
    UInt8 tempHarqProcessId = ulGrantInfo->tempHarqProcessId;
    /* +- SPR 18268 */
    UInt16 tempUeIndex = ulGrantInfo->tempUeIndex;
    /* +- SPR 18268 */
    /* TDD Config 6 Changes Start */
    UInt8 tokenUpdateFromStr = LTE_FALSE;
    UInt16 sysFrameNum = MODULO_ONEZEROTWOFOUR((currentGlobalTick)/MAX_SUB_FRAME);
    UInt16 ulSysFrameNum = 0;
    /* + SPR_14408 */
    UInt32 aperiodicReportDataSize = 0;
    InternalCellIndex ueServCellIndex = PCELL_SERV_CELL_IX;
    /* - SPR_14408 */
    if (ulSubFrameNum < subFrameNumber )
    {
        ulSysFrameNum = MODULO_ONEZEROTWOFOUR((sysFrameNum + 1));
    }
    else
    {
        ulSysFrameNum = sysFrameNum;
    } 
    /* TDD Config 6 Changes End */

    /* + SPR 14089 Changes */
    UInt8 prevUlSubFrameNum  = 0;
    UInt16 prevUlSysFrameNum = 0;
    UInt8 subFrameNum = currentGlobalTick % MAX_SUB_FRAME;
    UInt8 ulSubFrameOffset =  getUlOffset(subFrameNum,ulSubFrameNum);
    /* SPR 15909 fix start */
    tickType_t ulGlobalTick = currentGlobalTick + ulSubFrameOffset;
    /* SPR 15909 fix end */
    prevUlSubFrameNum = ( ulGlobalTick -
            getprevUlSfOffsetFromCurrentUlSf(ulSubFrameNum, internalCellIndex)
            ) % MAX_SUB_FRAME;
    GET_UL_SYS_FRAME_NUM_FROM_PHICH(prevUlSubFrameNum, subFrameNumber, prevUlSysFrameNum, sysFrameNum);
    /* - SPR 14089 Changes */

    /* UL_MU_MIMO_CHG Start */
    ueScheduledInfo_p = ulGrantInfo->ueScheduledInfo_p; 
    UInt16       buddyUeIndex       = ueScheduledInfo_p->buddyUeIndex;
    ULUEContext* ulBuddyUEContext_p = PNULL;
    if(( buddyUeIndex < MAX_UE_SUPPORTED ) && (PNULL != ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p))
    {
        ulBuddyUEContext_p = ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p;
    }
    /* UL_MU_MIMO_CHG End */
    /* TDD SB_CQI */
    UInt32 tempMinDataSize = 0;
    /* TDD SB_CQI */

    /* SPR 11331 Fix Start */
    UInt8 ulSubframeOffset =  getExactULSubFrame(subFrameNumber,internalCellIndex);
    if ( (ulSubframeOffset + subFrameNumber) % MAX_SUB_FRAME != ulSubFrameNum)
    {
        ulSubframeOffset = OFFSET_7;
    }
    /* SPR 15909 fix start */
    tickType_t ulSchTTI = currentGlobalTick + ulSubframeOffset;
    /* SPR 15909 fix end */
    /* SPR 11331 Fix End */

    LTE_MAC_UT_LOG( LOG_MAJOR, LOG_PUSCH, 
            "[%s] RBMapNo [%d] ulSubFrame [%d] HarqProcessId[%d] tti[%d]\n",
            __func__, rbMapNum, ulSubFrameNum, tempHarqProcessId,
            currentGlobalTick );

    tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[tempHarqProcessId]);
    ueScheduledInfo_p = ulGrantInfo->ueScheduledInfo_p; 


    /* TDD SB_CQI */
    /* + SPR_14430 */
    if(ulUEContext_p->aperiodicCQIToBeRequested) 
        /* - SPR_14430 */
    {
        /* + SPR_14408 */
        if(ulUEContext_p->aperiodicCQIToBeRequested & servingCellBitMask_g[PCELL_SERV_CELL_IX])
        {
            /* aperiodic csi is requested for Pcell */
            determineRBsForAperiodicCQIReports(ulUEContext_p,
                    &tempMinDataSize,
                    internalCellIndex);
            aperiodicReportDataSize += tempMinDataSize;
        }
        /* Now checking if aperiodic report is requested for Scell also in case
         * of CA UE */
        for(ueServCellIndex = START_SCELL_INDEX; ueServCellIndex <= ulUEContext_p->scellCount;
                ueServCellIndex++)
        {
            if(ulUEContext_p->aperiodicCQIToBeRequested & servingCellBitMask_g[ueServCellIndex])
            {
                tempMinDataSize = 0;
                if(PNULL != ulUEContext_p->ulUeScellContext_p[ueServCellIndex])
                {
                    determineRBsForAperiodicCQIReports(ulUEContext_p,
                            &tempMinDataSize,
                            ulUEContext_p->
                            ulUeScellContext_p[ueServCellIndex]->internalCellIndex
                            );
                    aperiodicReportDataSize += tempMinDataSize;
                }
            }
        }
        /*TDD SB_CQI */
        if(aperiodicReportDataSize > ulGrantInfo->pdcchULNewUE_p->maxDataSize)
        {
            ulGrantInfo->pdcchULNewUE_p->minDataSize = ulGrantInfo->pdcchULNewUE_p->maxDataSize;
        }
        else if(aperiodicReportDataSize > ulGrantInfo->pdcchULNewUE_p->minDataSize)
        {
            ulGrantInfo->pdcchULNewUE_p->minDataSize = aperiodicReportDataSize;
        }
        /* - SPR_14408 */
    }
    /* TDD SB_CQI */
    /* UL_MU_MIMO_CHG Start */
    if ( (localPUSCHResources) ||
            ( (buddyUeIndex < MAX_UE_SUPPORTED) &&
              (PNULL != ulBuddyUEContext_p) &&
              (ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated)) )
        /* UL_MU_MIMO_CHG End */
    {
        switch (ueScheduledInfo_p->ulGrantRequestType)
        {
            case SR_ULGRANT_REQ_MSG:
            case CQI_WITHOUT_DATA_ULGRANT_REQ_MSG:
            case CQI_WITH_DATA_ULGRANT_REQ_MSG:
            case CR_ULGRANT_REQ_MSG:                      
            case UL_TPC_WITHOUT_DATA:
            case UL_EXPLICIT_PHR_WITHOUT_DATA:
            case UL_TPC_WITH_DATA:
            case NEW_ULGRANT_REQ_MSG:
            /* + SPS_TDD_Changes */
            case UL_SPS_ACTIVATION:
            case UL_SPS_DEACTIVATION:
            case UL_SPS_OCCASION:
            /* + SPS_TDD_Changes */

            if(MAC_SUCCESS == allocatePUSCHResourcesForNewUEs(
                        ulUEContext_p,
                        ulGrantInfo->pdcchULNewUE_p,
                        ueScheduledInfo_p,
                        &resourceReserved,
                        rbMapNum,
                        localPUSCHResources,
                        /* Time Averaging changes start */
                        bufferRBs_p, tpc, ueMaxPowerFlag,ueMinPowerFlag,
                        /* + SPS_TDD_Changes */
                        spsAllocFlag
                        /* - SPS_TDD_Changes */
                        /* UL_MU_MIMO_CHG START */
                        ,subFrameNumber,currentGlobalTick,
                        internalCellIndex
                        /* UL_MU_MIMO_CHG END */
                        /* Time Averaging changes end */
                        /* SPR 11331 Fix Start */
                        ,ulSchTTI
                        /* SPR 11331 Fix End */
                        ,ulSubFrameNum
                        ,execArgs))
                        {
                            /* + SPS_TDD_Changes */
                            if ( (FALSE == spsAllocFlag) &&
                                    ( (ueScheduledInfo_p->ulGrantRequestType == UL_SPS_ACTIVATION) ||
                                      (ueScheduledInfo_p->ulGrantRequestType == UL_SPS_DEACTIVATION) ||
                                      (ueScheduledInfo_p->ulGrantRequestType == UL_SPS_OCCASION)  ))
                            {
                                return MAC_SUCCESS; 
                            }       
                            /* - SPS_TDD_Changes */
                            /* SPR 11115 Fix Start */
#ifdef OVERSHOOT_CORRECTION
                            UInt8 ulSubframeOffset =  getExactULSubFrame(subFrameNumber, internalCellIndex);
                            if ( (ulSubframeOffset + subFrameNumber) % MAX_SUB_FRAME != ulSubFrameNum)
                            {
                                ulSubframeOffset = OFFSET_7;
                            }
                            tempULHarqInfo_p->ulSchTTI = currentGlobalTick + ulSubframeOffset; 
#endif      
                            /* SPR 11115 Fix Start */

                            if (( ueScheduledInfo_p->ulGrantRequestType == CQI_WITHOUT_DATA_ULGRANT_REQ_MSG)
                                    || ( ueScheduledInfo_p->ulGrantRequestType == CQI_WITH_DATA_ULGRANT_REQ_MSG)
                                    /* TDD SB_CQI */
                                    /* + SPR_14430 */
                                    || (ulUEContext_p->aperiodicCQIToBeRequested) )
                                /* - SPR_14430 */
                                /* TDD SB_CQI */
                            {
                                /* + SPR_14408 */
                                tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].dciPduInfo.cqiRequest = ulUEContext_p->aperiodicCQIToBeRequested;
                                /* - SPR_14408 */
                                /* TDD SB_CQI */
                                /* store value of bitmap aperiodicCQIToBeRequested in
                                 * ulUEContext_p->reportFormatMap which will be used at
                                 * receiver while decoding the reports */
                                /*Fix for SPR 12477, updating index with subFrameNum*/   
                                /* + SPR_14431 */
                                    ulUEContext_p->reportFormatMap[ulSysFrameNum % 2][ulSubFrameNum].cellIdxOfExpectedReportType = 
                                    ulUEContext_p->aperiodicCQIToBeRequested;
                                /* - SPR_14431 */
                                /* CSI_P2_PUSCH_CHANGES end */
                                if (subFrameNumber > ulSubFrameNum)
                                {	
                                    /* TDD SB_CQI */
                                    ulUEContext_p->lastTriggeredAperiodicTTI = (currentGlobalTick +  (MAX_SUB_FRAME - subFrameNumber + ulSubFrameNum));
                                    /* TDD SB_CQI */

                                }
                                else
                                {
                                    ulUEContext_p->lastTriggeredAperiodicTTI = (currentGlobalTick + (ulSubFrameNum - subFrameNumber));

                                }
                            }
                            /* TDD SB_CQI */
                            else
                            {
                                tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].dciPduInfo.cqiRequest = 0;
                            }                      
                            ueScheduledInfo_p->flag = ALREADY_RESOURCES_ALLOCATED;

                            /* + SPR 16758 */
                            if ( (TRUE == spsAllocFlag) && ( (ueScheduledInfo_p->ulGrantRequestType == UL_SPS_ACTIVATION) ||\
                                        (ueScheduledInfo_p->ulGrantRequestType == UL_SPS_DEACTIVATION) ||\
                                        (ueScheduledInfo_p->ulGrantRequestType == UL_SPS_OCCASION)  ))
                            {
                                tempULHarqInfo_p->newDataIndicator = 0;
                            }
                            else
                            {
                                tempULHarqInfo_p->newDataIndicator = 1;
                            }
                            /* - SPR 16758 */

                            /* UL_MU_MIMO_CHG Start */
                            /* If resources are shared among buddies; then update PUSCH resources
                             * only for one of the UE */
                            if( 
                                    ( PNULL !=  ulBuddyUEContext_p ) &&
                                    ( !ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated))
                            {

                                if (localPUSCHResources > resourceReserved.allocatedLength)
                                {
                                    localPUSCHResources -=  resourceReserved.allocatedLength;
                                }
                                else
                                {
                                    localPUSCHResources = 0;
                                }                    
                            }
                            /* UL_MU_MIMO_CHG End */
                            /* TDD SB_CQI */
                            /*SPR 17120 fix start*/
                            if(ueScheduledInfo_p->ulGrantRequestType != CQI_WITHOUT_DATA_ULGRANT_REQ_MSG)
                                /* TDD SB_CQI */
                            {
                                /* + SPR 14089 Changes */
                                resetHarqId(prevUlSubFrameNum, ulUEContext_p, internalCellIndex,prevUlSysFrameNum);
                                /* - SPR 14089 Changes */

                                /* TDD Config 6 Changes Start */
                                setHarqBusy(ulSubFrameNum, tempHarqProcessId, ulUEContext_p, internalCellIndex,ulSysFrameNum);
                                /* TDD Config 6 Changes End */
                                /*+ Coverity fix 31422*/
                                if ( resourceReserved.allocatedLength > 0 )
                                {
                                    allocatedTBSize = determineUlTBSize(
                                            ulGrantInfo->pdcchULNewUE_p->avgMCS,
                                            resourceReserved.allocatedLength);
                                }
                                else
                                {
                                    return MAC_FAILURE;
                                }    
                            }
                            /*SPR 17120 fix end*/
                            /*- Coverity fix 31422*/
                            /* UL QOS CHANGE */
                            /* TDD Config 0 Changes Start */
                            ulQosUpdateScheduledTokens(ulUEContext_p,
                                    allocatedTBSize,
                                    tempHarqProcessId,
                                    internalCellIndex,
                                    ulSubFrameNum,
                                    tokenUpdateFromStr,
                                    execArgs);
                            /* TDD Config 0 Changes End */

                            LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_NEW_TX_COUNT
                                (ulUEContext_p->ueIndex);

                            /*TDD Config 0 Changes Start*/
                            scheduleCount = ulGrantInfo->pdcchULNewUE_p->scheduleCount;
                            /*TDD Config 0 Changes End*/

                            updateDciCCEInfoArray(
                                    ulUEContext_p,
                                    ueScheduledInfo_p->ulCCEIndex,
                                    tempHarqProcessId,
                                    tempDciCCEInfo_p,
                                    &resourceReserved,
                                    /* Time Averaging changes start */
                                    tpc.tpcCommand,
                                    /* Time Averaging changes end */
                                    /* +- SPR 17777 */
                                    /* UL_MU_MIMO_CHG Start */
                                    ueScheduledInfo_p->buddyUeIndex,
                                    /* UL_MU_MIMO_CHG End */
                                    /* +- SPR 17777 */
                                    //SPR_16896_Changes
                                    spsAllocFlag,
                                    //SPR_16896_Changes
                                    internalCellIndex
                                    /*TDD Config 0 Changes Start*/
                                    /* +- SPR 17777 */
                                    ,currentGlobalTick
                                    ,ulSubFrameNum
                                    ,scheduleCount
                                    /*TDD Config 0 Changes End*/
                                    );
                        }    
            else
            {
                /*if (ueScheduledInfo_p->ulGrantRequestType != \
                  NEW_ULGRANT_REQ_MSG)*/
                /* SPR 16840 fix start */
                if ((ueScheduledInfo_p->ulGrantRequestType != UL_SPS_ACTIVATION) &&
                        (ueScheduledInfo_p->ulGrantRequestType != UL_SPS_DEACTIVATION) &&
                        (ueScheduledInfo_p->ulGrantRequestType != UL_SPS_OCCASION))
                    /* SPR 16840 fix end */
                {
                    putEntryInPUSCHFailureQueue( 
                            tempUeIndex,
                            ueScheduledInfo_p->ttiCounter,
                            ueScheduledInfo_p->ulGrantRequestType,
                            internalCellIndex);
                }
                /* UL QOS CHANGE */
                allocatedTBSize = 0;
                /* TDD Config 0 Changes Start */
                ulQosUpdateScheduledTokens(
                        ulUEContext_p,
                        allocatedTBSize,
                        INVALID_HARQ_ID,
                        internalCellIndex,
                        ulSubFrameNum,
                        tokenUpdateFromStr,
                        execArgs
                        );
                /* TDD Config 0 Changes End */
                return MAC_FAILURE;
            }
            break;
            default:
            LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "Invalid Value "\
                    "Arrived in the  ulGrantRequestType ");    
            /* UL QOS CHANGE */
            allocatedTBSize = 0;
            /* TDD Config 0 Changes Start */
            ulQosUpdateScheduledTokens(
                    ulUEContext_p,
                    allocatedTBSize,
                    INVALID_HARQ_ID,
                    internalCellIndex,
                    ulSubFrameNum,
                    tokenUpdateFromStr,
                    execArgs
                    );
            /* TDD Config 0 Changes End */
            return MAC_FAILURE;
        }

    }
    /* SPR 16840 fix start */
    /*else if (ueScheduledInfo_p->ulGrantRequestType != \
      NEW_ULGRANT_REQ_MSG)*/
    else if((ueScheduledInfo_p->ulGrantRequestType != UL_SPS_ACTIVATION) &&
            (ueScheduledInfo_p->ulGrantRequestType != UL_SPS_DEACTIVATION) && 
            (ueScheduledInfo_p->ulGrantRequestType != UL_SPS_OCCASION))
        /* SPR 16840 fix end */
    {
        putEntryInPUSCHFailureQueue( 
                tempUeIndex,
                ueScheduledInfo_p->ttiCounter,
                ueScheduledInfo_p->ulGrantRequestType,
                internalCellIndex);

        *tempPUSCHResources_p = localPUSCHResources;
        /* UL QOS CHANGE */
        /* TDD Config 0 Changes Start */
        ulQosUpdateScheduledTokens(
                ulUEContext_p,
                0,
                INVALID_HARQ_ID,
                internalCellIndex,
                ulSubFrameNum,
                tokenUpdateFromStr,
                execArgs
                );
        /* TDD Config 0 Changes End */
        return MAC_FAILURE;
    }
    else
    {
        /* UL QOS CHANGE */

        /* TDD Config 0 Changes Start */
        ulQosUpdateScheduledTokens(
                ulUEContext_p,
                0,
                INVALID_HARQ_ID,
                internalCellIndex,
                ulSubFrameNum,
                tokenUpdateFromStr,
                execArgs
                );
        /* TDD Config 0 Changes End */
        return MAC_FAILURE;        
    }

    *tempPUSCHResources_p = localPUSCHResources;
    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "\nlocalPUSCHResources = %d", localPUSCHResources);
    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Exit...... \n",__func__);
    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;    

    return MAC_SUCCESS;
}

#endif

/***********************************************************************************
 * Function Name : putEntryInPUSCHFailureQueue
 * Inputs        : ueIndex - The UE INDEX of the concerned UE,
 *                 ttiCounter - The tti field of the UE as set by the Scheduler,
 *                 ulGrantRequestType - A field of  type ulGrantRequestType
 *                                      which states the type of UL Grant
 *                 internalCellIndex - Cell-Index used at MAC
 * Outputs       : None
 * Returns       : MAC_SUCCESS /MAC_FAILURE
 * Description   : This function is the main function for the newly scheduled UE
 *                 from where the PUSCH resources are allocated for different Grants.
 *************************************************************************************/


/* SPR 5599 changes start (ZIP ID 129107) */
MacRetType putEntryInPUSCHFailureQueue(
        /* SPR 5599 changes end (ZIP ID 129107) */
        UInt16 ueIndex, 
        /* SPR 15909 fix start */
        tickType_t ttiCounter,
        /* SPR 15909 fix end */
        ULGrantRequestType ulGrantRequestType,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    ULGrantRequestQueueNode* ulGrantRequestQueueNode_p = PNULL;
    /* CSI_P2_PUSCH_CHANGES Start */
    ULUEContext *ulUEContext_p = PNULL;
    ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    /* CSI_P2_PUSCH_CHANGES End */
    /* SPR 16840 fix start */
    /*if ( (NEW_ULGRANT_REQ_MSG != ulGrantRequestType) &&*/
    /* SPR 16840 fix end */
    if ((UL_SPS_ACTIVATION != ulGrantRequestType) &&
            (UL_SPS_DEACTIVATION != ulGrantRequestType) &&
            (UL_SPS_OCCASION != ulGrantRequestType) 
#ifdef FDD_CONFIG
            && (TTIB_UL_SPS_OCCASION != ulGrantRequestType)
#endif          
       )
    {
        GET_MEM_FROM_POOL(ULGrantRequestQueueNode , ulGrantRequestQueueNode_p , 
                sizeof(ULGrantRequestQueueNode),PNULL);

        LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Entry..... \n",__func__);

        /* + coverity 24555 */
        if (PNULL == ulGrantRequestQueueNode_p)
        {
            return MAC_FAILURE;
        }
        /* - coverity 24555 */

        ulGrantRequestQueueNode_p->ueIndex = ueIndex;
        ulGrantRequestQueueNode_p->ttiCounter = ttiCounter;
        ulGrantRequestQueueNode_p->ulGrantRequestType = ulGrantRequestType;
        /* CSI_P2_PUSCH_CHANGES Start */
        if((CQI_WITH_DATA_ULGRANT_REQ_MSG == ulGrantRequestType) ||
                (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG == ulGrantRequestType))
        {
            ulGrantRequestQueueNode_p->aperiodicCQIToBeRequested = 
                ulUEContext_p->aperiodicCQIToBeRequested;
        }
        /* CSI_P2_PUSCH_CHANGES End */

        /* + coverity 32518 */
        /*CA Changes start  */
        if(!(ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(
                        ulGrantPUSCHFailureQueue_gp[internalCellIndex],ULGrantRequestQueueNode,
                        &(ulGrantRequestQueueNode_p->ulGrantRequestNodeAnchor))))
            /*CA Changes end  */
        {
            freeMemPool(ulGrantRequestQueueNode_p);
            return MAC_FAILURE;
        }
        /* - coverity 32518 */

        LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Exit...... \n",__func__);
        return MAC_SUCCESS;
    }
    return MAC_FAILURE;
}

/*****************************************************************************
 * Function Name  : storeCqiPmiRiDataAperiodic 
 * Inputs         : transMode - Mode of Transmission,
 *                  uLUEContextInfo_p - pointer to UL UE context,
 *                  ulschUciRachInfo_p - pointer to the container node where
 *                                      information for UL config is set,
 *                  subFrameNum - Subframe number,
 *                  ulschUciInfoIndex - container index for 
 *                                      ulschUciRachInfo_p
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None 
 * Returns        : None
 * Description    : This function stores the necessary information in
 *                  1. ulschUciRachInfo_p which shall be used to encode UL
 *                  config PDUs
 *                  2. uLUEContextInfo_p which shall be rememberd in MAC and
 *                  will be used at the time of decoding raw PDUs received from
 *                  UE.
 *****************************************************************************/
    void storeCqiPmiRiDataAperiodic(UInt8 transMode, 
            ULUEContextInfo *ulUEContextInfo_p,
            ULSchUciRachPoolInfo *ulschUciRachInfo_p, 
            /*SPR 5620 START*/
            /* PUCCH TDD ADAPTATION CHANGES Start */        
#ifdef TDD_CONFIG 
            UInt16 sysFrameNum,
#endif
            /* PUCCH TDD ADAPTATION CHANGES End */        
            /*SPR 5620 END*/
            UInt8 subFrameNumber, UInt8 ulschUciInfoIndex, 
            InternalCellIndex internalCellIndex)
{
    /* + CQI_5.0 */
    UInt8   bitWidthiForRI_1 = 0;
    UInt8   bitWidthiForRIGT_1 = 0;
    UInt8   riBits = 0;
    /* CSI_P2_PUSCH_CHANGES start */
    UInt8   reportMapIndex = 0;
    /* CSI_P2_PUSCH_CHANGES end */
    ueExpectedReportType ueExpectedReport = EXPECTED_REPORT_INVALID;
    /* CSI_P2_PUSCH_CHANGES Start */
    ULUEContext *ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
    UInt8 riValue = 0;
    /* +- SPR 18268 */
    UInt16 ueIndex = 0;
    /* +- SPR 18268 */
    if(ulUEContext_p->aperiodicCQIToBeRequested & servingCellBitMask_g[PCELL_SERV_CELL_IX])
    {
        /* report of PCell is expected */
        riValue = ulUEContext_p->pucchConfigInfo.ueReportedRI;

        /* TDD SB_CQI*/	
        ulUEContextInfo_p->ulUEContext_p->periodicCQIDroppedCounter = 0;    
        /* TDD SB_CQI*/	

        bitWidthiForRI_1  = calculateReportFormatBitWidthAperiodic
            (ulUEContext_p->pucchConfigInfo.
             /* +- SPR 17777 */
             cqiPmiRiInfoAperiodic.cqiMode,
             &ueExpectedReport, transMode, &riBits,&bitWidthiForRIGT_1,
             ulUEContextInfo_p->ulUEContext_p->ueCategory,
             internalCellIndex);

        /* CSI_P2_PUSCH_CHANGES */
        reportMapIndex = subFrameNumber; 

        /* PUCCH TDD ADAPTATION CHANGES Start */        
#ifdef TDD_CONFIG
        /*SPR 5620 START*/
        /* SPR 18122 Changes Start*/
        ulUEContextInfo_p->ulUEContext_p->reportFormatMap
            [MODULO(sysFrameNum,2)][reportMapIndex].ueExpectedReport = ueExpectedReport;

        /* ulUEContextInfo_p->ulUEContext_p->reportFormatMap
           [MODULO(sysFrameNum,2)][reportMapIndex].bitWidth = bitWidth;*/
        /* SPR 18122 Changes End*/
        /*SPR 5620 END*/
#elif FDD_CONFIG
        /* CSI_P2_PUSCH_CHANGES*/
        ulUEContext_p->reportFormatMap[reportMapIndex].ueExpectedReport =
            ueExpectedReport;
#endif
        /* PUCCH TDD ADAPTATION CHANGES End */        
        ulschUciRachInfo_p->
            ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.riSize =
            riBits; 
        ulschUciRachInfo_p->
            ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.
            dlCqiPmiSizeRank_1 = bitWidthiForRI_1; 

        ulschUciRachInfo_p->
            ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.
            dlCqiPmiSizeRankGT_1 = bitWidthiForRIGT_1; 
        ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.
            pucchIndex = ulUEContext_p->pucchConfigInfo.
            cqiPUCCHRBIndex;
        ulschUciRachInfo_p->
            ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.
            rankIndicator = riValue; 
        /* SPR 12013 Fix */
    }
    /* check if report for Scell is expected */
    if(ulUEContext_p->aperiodicCQIToBeRequested & servingCellBitMask_g[START_SCELL_INDEX])
    {
        riValue = ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->scellpucchConfigInfo.ueReportedRI;
        /* TDD SB_CQI*/	
        ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->periodicCQIDroppedCounter = 0;    
        /* TDD SB_CQI*/	
        ueIndex = ulUEContext_p->ueIndex;
        transMode = dlUECtxInfoArr_g[ueIndex].dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->
            ScelldlMIMOInfo.transmissionMode;

        bitWidthiForRI_1  = calculateReportFormatBitWidthAperiodic
            (ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->scellpucchConfigInfo.
             /* +- SPR 17777 */
             cqiPmiRiInfoAperiodic.cqiMode,
             &ueExpectedReport, transMode,
             &riBits,&bitWidthiForRIGT_1,
             ulUEContextInfo_p->ulUEContext_p->ueCategory,
             ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->internalCellIndex);

        reportMapIndex = subFrameNumber; 

        /* PUCCH TDD ADAPTATION CHANGES Start */        
#ifdef TDD_CONFIG
        /*SPR 5620 START*/
        /* CA TDD PUCCH Changes
           SPR 18122 Changes Start
           ulUEContext_p->reportFormatMap
           [MODULO(sysFrameNum,2)][reportMapIndex].ueExpectedReport = ueExpectedReport;
           */
        /** SPR 14032 Fix : Start **/
        ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->cqiPmiRiReportFormatMapSCell
            [MODULO(sysFrameNum,2)][reportMapIndex].ueExpectedReport = ueExpectedReport;
        /** SPR 14032 Fix : End **/

        /* ulUEContextInfo_p->ulUEContext_p->reportFormatMap
           [MODULO(sysFrameNum,2)][reportMapIndex].bitWidth = bitWidth;*/
        /* SPR 18122 Changes Start*/
        /*SPR 5620 END*/
#elif FDD_CONFIG
        ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->cqiPmiRiReportFormatMapSCell
            [reportMapIndex].ueExpectedReport = ueExpectedReport;
#endif
        /* PUCCH TDD ADAPTATION CHANGES End */        
        ulschUciRachInfo_p->
            ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.cqiRiScellInfo[START_SCELL_INDEX].riSize =
            riBits; 
        ulschUciRachInfo_p->
            ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.cqiRiScellInfo[START_SCELL_INDEX].
            dlCqiPmiSizeRank_1 = bitWidthiForRI_1; 

        ulschUciRachInfo_p->
            ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.cqiRiScellInfo[START_SCELL_INDEX].
            dlCqiPmiSizeRankGT_1 = bitWidthiForRIGT_1; 
        ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.
            pucchIndex = ulUEContextInfo_p->ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]->
            scellpucchConfigInfo.
            cqiPUCCHRBIndex;
        ulschUciRachInfo_p->
            ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.cqiRiScellInfo[START_SCELL_INDEX].
            rankIndicator = riValue; 
    }
    /* CSI_P2_PUSCH_CHANGES End */

    /* - CQI_5.0 */
    /* SPR 12013 Fix start */
    ulschUciRachInfo_p->
        ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.
        bitMaskForAperiodicReport = ulUEContext_p->aperiodicCQIToBeRequested ; 
    /* SPR 12013 Fix end */
    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.
        deltaOffsetCQI = ulUEContextInfo_p->ulUEContext_p->puschConfigInfo.
        betaOffsetCQIIndex;

    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].cqiRiInfo.
        deltaOffsetRI = ulUEContextInfo_p->ulUEContext_p->puschConfigInfo.
        betaOffsetRiIndex;


    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].rnti = 
        ulUEContextInfo_p->ulUEContext_p->crnti;
    /* fapi2.1 Changes start */

    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
        cqiRiInfo.reportType=CQI_RI_APERIODIC_MODE;
    /* fapi2.1 Changes  end*/

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"CQI/ PMI/ RI Data stored in UL UE "
            "context for Aperiodic report");
    return;
}

/*****************************************************************************
 * Function Name  : assignRBsForMaxData 
 * Inputs         : pdcchULNewUE_p - pointer to the new UE structure 
 *                                   containing fields minData, maxData, 
 *                                   avgMCS and UE Index,
 *                  requiredRB_p - pointer to the directIndexing->requiredRB
 *                                 or ueScheduledInfo->requiredRB based on 
 *                                 FDD or TDD mode,
 *                  tempPUSCHResources - available Uplink resources in a 
 *                                       subframe,
 *                  tpc,
 *                  ueMaxPowerFlag_p,
 *                  ueMinPowerFlag_p,
 *                  ulUEContext_p - Pointer to ULUEContext
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : Allocated PRBs for Max Data 
 * Returns        : None
 * Description    : This function checks for PRBs required to support MaxData
 *                  Size and if uplink resources are available then sets the
 *                  required Rbs and minDataSize with these calculated values.
 *****************************************************************************/
            /* SPR 5599 changes start (ZIP ID 129107) */
            void assignRBsForMaxData(PdcchULNewUE *pdcchULNewUE_p, 
                    /* SPR 5599 changes end (ZIP ID 129107) */
                    UInt8 *requiredRB_p,
                    /* Time Averaging changes start */
                    UInt32 tempPUSCHResources,
                    SInt8 tpc,
                    UInt32 *ueMaxPowerFlag_p,
                    UInt32 *ueMinPowerFlag_p
                    /* Time Averaging changes end */
                    /* +DYNAMIC_CICIC */
                    ,ULUEContext *ulUEContext_p
                    /* -DYNAMIC_CICIC */
                    /*CA Changes start  */
                    ,InternalCellIndex internalCellIndex)
            /*CA Changes end  */
{
    UInt32 dataSize = pdcchULNewUE_p->maxDataSize;
    /* +DYNAMIC_ICIC */
    /*CA Changes start  */
    ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + ulPowerMaskIndex_g[internalCellIndex];
    /*CA Changes end  */
    SInt32 regionPowerMask = ulPowerMaskInfo_p->powerMask[ulUEContext_p->userLocationType];
    /* -DYNAMIC_ICIC */
    /* SPR 5999 changes start */
    /*CA Changes start  */
    UInt8 ulPRBsRequired = DETERMINE_RBs_UL(pdcchULNewUE_p->avgMCS, 
            dataSize, internalCellIndex);
    /*CA Changes end  */
    /* SPR 5999 changes end */

    ulPRBsRequired = getUplinkMaxRBsInPowerOfTwoThreeFive(
            ulPRBsRequired);


    if (ulPRBsRequired > tempPUSCHResources)
    {
        ulPRBsRequired = getUplinkMinRBsInPowerOfTwoThreeFive(
                tempPUSCHResources);
    }

    /* +DYNAMIC_ICIC */
    CHECK_QCI_POWER_MASK_AND_UPDATE_REGION_MASK(ulUEContext_p,regionPowerMask);
    /* -DYNAMIC_ICIC */

    ulPRBsRequired = validateATB(ulPRBsRequired, \
            &pdcchULNewUE_p->avgMCS,\

            /* Time Averaging changes start */
            ulUECtxInfoArr_g[pdcchULNewUE_p->ueIndex].ulUEContext_p,\
            tpc, \
            ueMaxPowerFlag_p,ueMinPowerFlag_p, regionPowerMask,NEW_TX,
            internalCellIndex);

    *requiredRB_p = ulPRBsRequired;    
    pdcchULNewUE_p->minDataSize = determineUlTBSize(pdcchULNewUE_p->avgMCS, 
            *requiredRB_p);
}
/*SPR 7086 Fix Start*/
/*****************************************************************************
 * Function Name  : nSRSDetermineOnCollision
 * Inputs         : ulUEContext_p, ulHarqProcessId, ulSubFrameNum 
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        :  
 * Returns        : None
 * Description    : This function determined the SRS Collision on Cell
 *****************************************************************************/
UInt8 nSRSDetermineOnCollision(
        ULUEContext *ulUEContext_p, 
        UInt8 ulHarqProcessId, 
        UInt8 ulSubFrameNum,
        InternalCellIndex internalCellIndex)
{
    UInt8                   ulStartRB = 0;
    UInt8                   ulEndRB = 0;

    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    /*Set nSRS to TRUE when UE SRS Occassion is present
     * else, determine the nSRS by collision*/
    /* SPR 7086 RETX fix start */
    if(LTE_TRUE == *(srsCellConfigMap_gp[internalCellIndex] + (ulSubFrameNum % MAX_SUB_FRAME)))
        /* SPR 7086 RETX fix end */
    {
        ulStartRB = ulUEContext_p->ulresInfo[ulHarqProcessId].riStart;
        ulEndRB = (UInt8)(ulUEContext_p->ulresInfo[ulHarqProcessId].riStart +
                ulUEContext_p->ulresInfo[ulHarqProcessId].riLength);

        if((ulEndRB < cellParams_p->srsFreqDomainStartRB) || (ulStartRB > cellParams_p->srsFreqDomainEndRB))
        {
            return SRS_ULSCH_NO_OVERLAP;
        }
        else
        {
            return SRS_ULSCH_OVERLAP;
        }
    }
    else
    {
        return SRS_ULSCH_NO_OVERLAP;
    }
}
/*SPR 7086 Fix End*/

/* ICIC changes start */
/********************************************************************************
 * Function Name  : fillSemiStaticInfoForULSCH 
 * Inputs         : ulschUciRachInfo_p - Pointer to ULSchUciRachPoolInfo,
 *                  ueInfo_p - Pointer to DirectIndexingUEInfo incase of FDD
 *                             Pointer to UeScheduledInfo incase of TDD_CONFIG,
 *                  ulUEContextInfo_p - Pointer to ULUEContextInfo,
 *                  ulHarqInfo_p - Pointer to ULHarqInfo,
 *                  ulHarqProcessId,
 *                  sysFrameNum,
 *                  subFrameNum,
 *                  aperiodicCQIRequestedFlag
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        :  
 * Returns        : None
 * Description    : This function converts the UL Config PDU Type from PUCCH to
 *                  PUSCH and sets the corresponding fields for ULSCH.
 ********************************************************************************/
            void fillSemiStaticInfoForULSCH (ULSchUciRachPoolInfo *ulschUciRachInfo_p,
#ifdef FDD_CONFIG
                    DirectIndexingUEInfo *ueInfo_p,
#elif TDD_CONFIG
                    UeScheduledInfo *ueInfo_p,
#endif
                    ULUEContextInfo *ulUEContextInfo_p,
                    ULHarqInfo* ulHarqInfo_p,
                    UInt8   ulHarqProcessId,
                    /*SPR 5620 START*/
#ifdef TDD_CONFIG
                    UInt16 sysFrameNum,
#endif
                    /*SPR 5620 END*/
                    UInt8 subFrameNum,
                    /* + CQI_5.0 */
                    UInt8 aperiodicCQIRequestedFlag,
                    /* - CQI_5.0 */
                    /*CA Changes start */
                    InternalCellIndex internalCellIndex)
            /*CA Changes end */
{ 
    ULSchUciInfo* ulSchUciInfo_p = PNULL;
    UInt16 ulschUciInfoIndex = ueInfo_p->ulschUciInfoIndex;
    /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
#ifdef TDD_CONFIG
    UInt8 ackNackMode = 0;
#endif    
    /* SPR 19679: TDD HARQ Multiplexing  Changes End */
    ulSchUciInfo_p = &ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex];
    ulSchUciInfo_p->ulHarqInfo_p = ulHarqInfo_p;
    ulSchUciInfo_p->ulHarqProcessId = ulHarqProcessId;

    /*SPR 11980 Fix Start */
#ifdef FDD_CONFIG
#ifdef FAPI_4_0_COMPLIANCE
    UInt8 cellCount = 0;
    DLUEContext *dlUEContext_p = 
        dlUECtxInfoArr_g[ulUEContextInfo_p->ulUEContext_p->ueIndex].
        dlUEContext_p;
#endif
#endif
    /*SPR 11980 Fix end */

    /*SPR 7086 Fix Start*/
    ulSchUciInfo_p->nSRS = nSRSDetermineOnCollision(ulUEContextInfo_p->ulUEContext_p, 
            ulHarqProcessId, subFrameNum, internalCellIndex);
    /* ++ SPR 19426 */
#ifdef FDD_CONFIG   
    UInt8 isHarqTTIBundled = 0;
    isHarqTTIBundled = ulUEContextInfo_p->ulUEContext_p->ulresInfo[ulHarqProcessId].isTTIBundled;
#endif    
    /* -- SPR 19426 */
    /*SPR 7086 Fix End*/
    /* spr 21260 + */
    ULUEContext* ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

    if((ulUEContext_p->pucchConfigInfo.simultaneousAckNackAndCQI) && (TRUE == ulUEContext_p->isCqiDropped) && (ulUEContext_p->cqiDropTick == getCurrentTick()))
    {
        if (MAC_SS_PDU_UCI_HARQ == ulSchUciInfo_p->pduType)
        {
            ulSchUciInfo_p->pduType = MAC_SS_PDU_UCI_CQI_HARQ;
        }
        ulUEContext_p->isCqiDropped = FALSE;
        ulUEContext_p->cqiDropTick   = 0;
    }
    /* spr 21260 - */
    switch (ulSchUciInfo_p->pduType)
    {
        case MAC_SS_PDU_UCI_CQI:
        {
            /* ++ SPR 19426 */
#ifdef FDD_CONFIG    
            if(isHarqTTIBundled && (FALSE == aperiodicCQIRequestedFlag) )
            {
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH; 
            }
            else
#endif                
            {
                /* -- SPR 19426 */
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH_CQI_RI; 
            }
            /* + CQI_5.0 */
            /* CSI_P2_PUSCH_CHANGES  == TRUE  removed */
            if(aperiodicCQIRequestedFlag)
                /* - CQI_5.0 */
            {
                storeCqiPmiRiDataAperiodic(
                        ueInfo_p->transMode,
                        ulUEContextInfo_p, ulschUciRachInfo_p,
                        /*SPR 5620 START*/
#ifdef TDD_CONFIG
                        sysFrameNum,
#endif
                        /*SPR 5620 END*/
                        subFrameNum, ulschUciInfoIndex,
                        internalCellIndex);
            }
            /* + CQI_5.0 */
            else
            {
                if (ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1)
                {
                    ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1 = 
                        ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1;
                }
                else
                {
                    ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1 = 
                        ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1;
                }
            }    

            /* - CQI_5.0 */
            /* + SPR_5323 */ 
            RESET_HARQ_BLER_RECEIVED_ON_PUCCH(ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    subFrameNum);
            /* - SPR_5323 */ 
            LOG_UT(MAC_UCI_ULSCH_PDU_CHANGE,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    MAC_SS_PDU_UCI_CQI,\
                    ulSchUciInfo_p->pduType,\
                    ulschUciInfoIndex,\
                    ulSchUciInfo_p->ulHarqProcessId,\
                    0.0,\
                    0.0,\
                    __func__,"ADVANCE_RES_ALLOC");
            break;
        }
        case MAC_SS_PDU_UCI_CQI_HARQ:
        {    
#ifdef TDD_CONFIG
            /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
//#ifndef FAPI_4_0_COMPLIANCE
#if 0
            ackNackMode = ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                dlHarqInfo.acknackMode ;
#else
            ackNackMode =  ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                dlHarqInfo.ackNackMode;
#endif
            /*SPR 20564 Changes Start*/
            if ( HARQ_BUNDLING == ackNackMode)
            {
                if(MAC_SUCCESS == isUeSISO(ulUEContextInfo_p->ulUEContext_p->ueIndex))
                {
                    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                        dlHarqInfo.harqSize = 1;
                }
                else
                {
                    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                        dlHarqInfo.harqSize = 2;
                }
            }
            /*SPR 20564 Changes End*/
#endif
            /* SPR 19679: TDD HARQ Multiplexing  Changes End */
            /*SPR 11980 Fix Start */
#ifdef FDD_CONFIG
#ifdef FAPI_4_0_COMPLIANCE
            for( cellCount = START_SCELL_INDEX; cellCount <= MAX_NUM_SCELL; 
                    cellCount++)
            {
                if( IS_CA_ENABLED() &&
                        PNULL != dlUEContext_p->dlUeScellContext_p[cellCount] )
                { 
                    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                        dlHarqInfo.ackNackMode = 2;
                }
            }
#endif
#endif
            /* ++ SPR 19426 */
#ifdef FDD_CONFIG    
            if(isHarqTTIBundled && (FALSE == aperiodicCQIRequestedFlag) )
            {
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH_HARQ; 
            }
            else
#endif
            {
                /* -- SPR 19426 */
                /*SPR 11980 Fix End */
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH_CQI_HARQ_RI;
            }
            /* + CQI_5.0 */
            /* CSI_P2_PUSCH_CHANGES  == TRUE  removed */
            if(aperiodicCQIRequestedFlag)
                /* - CQI_5.0 */
            {
                storeCqiPmiRiDataAperiodic(
                        ueInfo_p->transMode,
                        ulUEContextInfo_p, ulschUciRachInfo_p, 
                        /*SPR 5620 START*/
#ifdef TDD_CONFIG
                        sysFrameNum,
#endif
                        /*SPR 5620 END*/
                        subFrameNum, ulschUciInfoIndex,
                        internalCellIndex);
            }
            /* + CQI_5.0 */
            else
            {
                if (ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1)
                {
                    ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1 =
                        ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1;
                }
                else
                {
                    ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1 =
                        ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1;
                }
            }
            /* - CQI_5.0 */
            /* SPR 4571 fix start */
            ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].dlHarqInfo.deltaOffsetHARQ =
                ulUEContextInfo_p->ulUEContext_p->puschConfigInfo.betaOffsetAckIndex;
            /* SPR 4571 fix end */

            RESET_HARQ_BLER_RECEIVED_ON_PUCCH(ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    subFrameNum);
            LOG_UT(MAC_UCI_ULSCH_PDU_CHANGE,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    MAC_SS_PDU_UCI_CQI_HARQ,\
                    ulSchUciInfo_p->pduType,\
                    ulschUciInfoIndex,\
                    ulSchUciInfo_p->ulHarqProcessId,\
                    0.0,\
                    0.0,\
                    __func__,"ADVANCE_RES_ALLOC");
            break;
        }
        case MAC_SS_PDU_UCI_HARQ:
        {
#ifdef TDD_CONFIG
            /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
//#ifndef FAPI_4_0_COMPLIANCE
#if 0
            ackNackMode = ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                dlHarqInfo.acknackMode ;
#else
            ackNackMode =  ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                dlHarqInfo.ackNackMode;
#endif
            /*SPR 20564 Changes Start*/
            if ( HARQ_BUNDLING == ackNackMode)
            {
                if(MAC_SUCCESS == isUeSISO(ulUEContextInfo_p->ulUEContext_p->ueIndex))
                {
                    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                        dlHarqInfo.harqSize = 1;
                }
                else
                {
                    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                        dlHarqInfo.harqSize = 2;
                }
            } 
            /*SPR 20564 Changes End*/
            /* SPR 19679: TDD HARQ Multiplexing  Changes End */
#endif
            /*SPR 11980 Fix Start */
#ifdef FDD_CONFIG
#ifdef FAPI_4_0_COMPLIANCE
            for( cellCount = START_SCELL_INDEX; cellCount <= MAX_NUM_SCELL; 
                    cellCount++)
            {
                if( IS_CA_ENABLED() &&
                        PNULL != dlUEContext_p->dlUeScellContext_p[cellCount] )
                {
                    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                        dlHarqInfo.ackNackMode = 2;
                }
            }
#endif
#endif
            /*SPR 11980 Fix End */
            /* + CQI_5.0 */
            /* CSI_P2_PUSCH_CHANGES  == TRUE  removed */
            if(aperiodicCQIRequestedFlag)
                /* - CQI_5.0 */
            {
                ulSchUciInfo_p->pduType =
                    MAC_SS_PDU_ULSCH_CQI_HARQ_RI;

                storeCqiPmiRiDataAperiodic(
                        ueInfo_p->transMode,
                        ulUEContextInfo_p, ulschUciRachInfo_p, 
                        /*SPR 5620 START*/
#ifdef TDD_CONFIG
                        sysFrameNum,
#endif
                        /*SPR 5620 END*/
                        subFrameNum, ulschUciInfoIndex,
                        internalCellIndex);
            }
            else
            {
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH_HARQ;
            }
            /* SPR 4571 fix start */
            ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].dlHarqInfo.deltaOffsetHARQ =
                ulUEContextInfo_p->ulUEContext_p->puschConfigInfo.betaOffsetAckIndex;
            /* SPR 4571 fix end */
            RESET_HARQ_BLER_RECEIVED_ON_PUCCH(ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    subFrameNum);
            LOG_UT(MAC_UCI_ULSCH_PDU_CHANGE,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    MAC_SS_PDU_UCI_HARQ,\
                    ulSchUciInfo_p->pduType,\
                    ulschUciInfoIndex,\
                    ulSchUciInfo_p->ulHarqProcessId,\
                    0.0,\
                    0.0,\
                    __func__,"ADVANCE_RES_ALLOC");

            break;
        }
        case MAC_SS_PDU_UCI_SR:
        {
            /* + CQI_5.0 */
            /* CSI_P2_PUSCH_CHANGES  == TRUE  removed */
            if(aperiodicCQIRequestedFlag)
                /* - CQI_5.0 */
            {
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH_CQI_RI;
                storeCqiPmiRiDataAperiodic(
                        ueInfo_p->transMode,
                        ulUEContextInfo_p, ulschUciRachInfo_p, 
                        /*SPR 5620 START*/
#ifdef TDD_CONFIG
                        sysFrameNum,
#endif
                        /*SPR 5620 END*/
                        subFrameNum,
                        ulschUciInfoIndex,
                        internalCellIndex);
            }
            else
            {
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH;
            }
            /*SPR 20783 Start*/
            RESET_HARQ_BLER_RECEIVED_ON_PUCCH(ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    subFrameNum);
            /*SPR 20783 End*/

            LOG_UT(MAC_UCI_ULSCH_PDU_CHANGE,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    MAC_SS_PDU_UCI_SR,\
                    ulSchUciInfo_p->pduType,\
                    ulschUciInfoIndex,\
                    ulSchUciInfo_p->ulHarqProcessId,\
                    0.0,\
                    0.0,\
                    __func__,"ADVANCE_RES_ALLOC");
            break;


        }
        case MAC_SS_PDU_UCI_SR_HARQ:
        {
#ifdef TDD_CONFIG
            /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
//#ifndef FAPI_4_0_COMPLIANCE
#if 0
            ackNackMode = ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                dlHarqInfo.acknackMode ;
#else
            ackNackMode =  ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                dlHarqInfo.ackNackMode;
#endif
            /*SPR 20564 Changes Start*/
            if ( HARQ_BUNDLING == ackNackMode)
            {
                if(MAC_SUCCESS == isUeSISO(ulUEContextInfo_p->ulUEContext_p->ueIndex))
                {
                    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                        dlHarqInfo.harqSize = 1;
                }
                else
                {
                    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                        dlHarqInfo.harqSize = 2;
                }
            }
            /*SPR 20564 Changes End*/

            /* SPR 19679: TDD HARQ Multiplexing  Changes End */

#endif
            /* + CQI_5.0 */
            /* CSI_P2_PUSCH_CHANGES  == TRUE  removed */
            if(aperiodicCQIRequestedFlag)
                /* - CQI_5.0 */
            {
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH_CQI_HARQ_RI;
                storeCqiPmiRiDataAperiodic(
                        ueInfo_p->transMode,
                        ulUEContextInfo_p, ulschUciRachInfo_p, 
                        /*SPR 5620 START*/
#ifdef TDD_CONFIG
                        sysFrameNum,
#endif
                        /*SPR 5620 END*/
                        subFrameNum,
                        ulschUciInfoIndex,
                        internalCellIndex);
            }
            else
            {
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH_HARQ;
            }
            /* SPR 4571 fix start */
            ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].dlHarqInfo.deltaOffsetHARQ =
                ulUEContextInfo_p->ulUEContext_p->puschConfigInfo.betaOffsetAckIndex;
            /* SPR 4571 fix end */
            RESET_HARQ_BLER_RECEIVED_ON_PUCCH(ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    subFrameNum);
            LOG_UT(MAC_UCI_ULSCH_PDU_CHANGE,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    MAC_SS_PDU_UCI_SR_HARQ,\
                    ulSchUciInfo_p->pduType,\
                    ulschUciInfoIndex,\
                    ulSchUciInfo_p->ulHarqProcessId,\
                    0.0,\
                    0.0,\
                    __func__,"ADVANCE_RES_ALLOC");
            break;
        }
        case MAC_SS_PDU_UCI_CQI_SR:
        {
            /* ++ SPR 19426 */
#ifdef FDD_CONFIG    
            if(isHarqTTIBundled && (FALSE == aperiodicCQIRequestedFlag) )
            {
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH; 
            }
            else
#endif                
            {
                /* -- SPR 19426 */
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH_CQI_RI;
            }
            /* + CQI_5.0 */
            /* CSI_P2_PUSCH_CHANGES  == TRUE  removed */
            if(aperiodicCQIRequestedFlag)
                /* - CQI_5.0 */
            {
                storeCqiPmiRiDataAperiodic(
                        ueInfo_p->transMode,
                        ulUEContextInfo_p, ulschUciRachInfo_p, 
                        /*SPR 5620 START*/
#ifdef TDD_CONFIG
                        sysFrameNum,
#endif
                        /*SPR 5620 END*/
                        subFrameNum,
                        ulschUciInfoIndex,
                        internalCellIndex);
            }
            /* + CQI_5.0 */
            else
            {
                if (ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1)
                {
                    ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1 =
                        ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1;
                }
                else
                {
                    ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1 =
                        ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1;
                }
            }

            /* + SPR_5323 */ 
            RESET_HARQ_BLER_RECEIVED_ON_PUCCH(ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    subFrameNum);
            /* - SPR_5323 */ 
            /* - CQI_5.0 */
            LOG_UT(MAC_UCI_ULSCH_PDU_CHANGE,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    MAC_SS_PDU_UCI_CQI_SR,\
                    ulSchUciInfo_p->pduType,\
                    ulschUciInfoIndex,\
                    ulSchUciInfo_p->ulHarqProcessId,\
                    0.0,\
                    0.0,\
                    __func__,"ADVANCE_RES_ALLOC");
            break;
        }
        case MAC_SS_PDU_UCI_CQI_SR_HARQ:
        {
#ifdef TDD_CONFIG
            /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
//#ifndef FAPI_4_0_COMPLIANCE
#if 0
            ackNackMode = ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                dlHarqInfo.acknackMode ;
#else
            ackNackMode =  ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                dlHarqInfo.ackNackMode;
#endif
            /*SPR 20564 Changes Start*/
            if ( HARQ_BUNDLING == ackNackMode)
            {
                if(MAC_SUCCESS == isUeSISO(ulUEContextInfo_p->ulUEContext_p->ueIndex))
                {
                    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                        dlHarqInfo.harqSize = 1;
                }
                else
                {
                    ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].
                        dlHarqInfo.harqSize = 2;
                }
            }
            /*SPR 20564 Changes End*/
            /* SPR 19679: TDD HARQ Multiplexing  Changes End */
#endif
            /* ++ SPR 19426 */            
#ifdef FDD_CONFIG    
            if(isHarqTTIBundled && (FALSE == aperiodicCQIRequestedFlag) )
            {
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH_HARQ; 
            }
            else
#endif
            {
                /* -- SPR 19426 */
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH_CQI_HARQ_RI;
            }
            /* + CQI_5.0 */
            /* CSI_P2_PUSCH_CHANGES  == TRUE  removed */
            if(aperiodicCQIRequestedFlag)
                /* - CQI_5.0 */
            {
                storeCqiPmiRiDataAperiodic(
                        ueInfo_p->transMode,
                        ulUEContextInfo_p, ulschUciRachInfo_p, 
                        /*SPR 5620 START*/
#ifdef TDD_CONFIG
                        sysFrameNum,
#endif
                        /*SPR 5620 END*/
                        subFrameNum,
                        ulschUciInfoIndex,
                        internalCellIndex);
            }
            /* + CQI_5.0 */
            else
            {
                if (ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1)
                {
                    ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1 =
                        ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1;
                }
                else
                {
                    ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRank_1 =
                        ulSchUciInfo_p->cqiRiInfo.dlCqiPmiSizeRankGT_1;
                }
            }
            /* - CQI_5.0 */
            /* SPR 4571 fix start */
            ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex].dlHarqInfo.deltaOffsetHARQ =
                ulUEContextInfo_p->ulUEContext_p->puschConfigInfo.betaOffsetAckIndex;
            /* SPR 4571 fix end */
            RESET_HARQ_BLER_RECEIVED_ON_PUCCH(ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    subFrameNum);
            LOG_UT(MAC_UCI_ULSCH_PDU_CHANGE,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    ulUEContextInfo_p->ulUEContext_p->ueIndex,
                    MAC_SS_PDU_UCI_CQI_SR_HARQ,\
                    ulSchUciInfo_p->pduType,\
                    ulschUciInfoIndex,\
                    ulSchUciInfo_p->ulHarqProcessId,\
                    0.0,\
                    0.0,\
                    __func__,"ADVANCE_RES_ALLOC");
            break;
        }
        default:
        {
            lteWarning("Invalid PDU Type [%d] received in func [%s]\n",
                    ulSchUciInfo_p->pduType, __func__);
            break;
        }         
    }

    /* SPS CHANGES */
    /*  + SPS_TDD */
    if( (ulUEContextInfo_p->ulUEContext_p->schType == SPS_SCHEDULING) &&
            (	(TRUE == checkSpsActTti(ulUEContextInfo_p->ulUEContext_p, ulHarqProcessId)) ||
                (TRUE == checkSpsOccasionTti(ulUEContextInfo_p->ulUEContext_p, ulHarqProcessId)) )) 
    {    
        /*  - SPS_TDD */
        ulSchUciInfo_p->rnti = ulUEContextInfo_p->ulUEContext_p->spsUlInfo.SpsCrnti;
    }    
    else 
    {    
        ulSchUciInfo_p->rnti = ulUEContextInfo_p->ulUEContext_p->crnti;
    }    
    /* SPS CHANGES */

    /*SPR 5588 Start*/
    /* Removed this line for FDD as when a DCI 2a message is filled with only
     * a single TB on pusch the harqsize for the dci message would not be 
     * corrected. This was due to ulschuciinfo being set to invalid for every 
     * single pusch dci message.*/
                    /*SPR 5588 End*/
}

/*****************************************************************************
 * Function Name  : updateCEAvailRes 
 * Inputs         : availRes_p - pointer to the available resources of 
 *                               CE region,
 *                  startIndex - Index where allocation starts,
 *                  numPhysicalResources - number of physical resources
 *                                         allocated  
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : CE resources and indexes are updated  
 * Returns        : MAC_SUCCESS  - Allocation made on CE region and it is 
 *                                 updated accordingly
 *                  MAC_FAILURE  - Allocation is made on CC region
 * Description    : This function updates CE region resources and indexes if 
 *                  allocation is done on CE region.
 *****************************************************************************/
UInt8 updateCEAvailRes(AvailableResInfo *availRes_p,
        UInt8  startIndex,
        UInt8  numPhysicalResources,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt8 overlapRBs = 0;
    UInt8 tempFreeRBIndex = 0;
    /*CA Changes start  */
    if(0 < ulResourcePartitionConfig_gp[internalCellIndex]->cellEdgeRegionCount)
        /*CA Changes end  */
    {
        if(startIndex <= availRes_p->freeRBIndex)
        {
            /* It will process only the overlapping condition.i.e
             * if both startIndex and (startIndex  + numPhysicalResources )
             *  are less than freeRBIndex,then no process will take place*/
            if (availRes_p->freeRBIndex < (startIndex  + numPhysicalResources )) 
            {
                if(availRes_p->endIndex > (startIndex  + numPhysicalResources ))
                {
                    overlapRBs = (startIndex  + numPhysicalResources ) -
                        availRes_p->freeRBIndex;
                    availRes_p->freeRBCounter -= overlapRBs; 
                    availRes_p->freeRBIndex = startIndex + numPhysicalResources;
                }
                else 
                {
                    overlapRBs = availRes_p->endIndex - availRes_p->freeRBIndex + 1;
                    availRes_p->freeRBCounter -= overlapRBs;
                    /* freeRBIndex is updated so becasue the check/loop in resource allocator
                     * will fail in the first go ,as all resources will be found busy though in
                     * some cases if freeRBCounter is 0, the function should not be called */
                    tempFreeRBIndex =  availRes_p->freeRBIndex;
                    availRes_p->freeRBIndex = availRes_p->endIndex + 1;
                    /* It is checked so,otherwise endIndex might point to -1,
                     * which is anyway invalid */
                    if (tempFreeRBIndex > 0)
                    {
                        availRes_p->endIndex = tempFreeRBIndex - 1;
                    }
                }
            }
        }
        else if(availRes_p->endIndex > (startIndex  + numPhysicalResources ))
        {
            /* This assumes that startIndex is greater than freeRBIndex,
             * other cases being handled in the above if condition */
            overlapRBs = numPhysicalResources;
            availRes_p->freeRBCounter -= overlapRBs;
        }
        else
        {
            /* This assumes that startIndex is greater than freeRBIndex,
             * other cases being handled in the above if condition .
             * It will process only the overlapping condition.i.e
             * if both startIndex and (startIndex  + numPhysicalResources )
             * are greater than endIndex,then no process will take place*/
            if(availRes_p->endIndex > startIndex)
            {
                overlapRBs = availRes_p->endIndex - startIndex + 1;
                availRes_p->freeRBCounter -= overlapRBs;
                availRes_p->endIndex = startIndex - 1;
            }
        }
    }
    LOG_MAC_MSG(MAC_UPD_CE_AVAIL_RES_INFO,LOGDEBUG,MAC_PUSCH,\
            getCurrentTick(),\
            availRes_p->freeRBIndex,\
            availRes_p->endIndex,\
            availRes_p->freeRBCounter,\
            overlapRBs,\
            0,\
            0.0,0.0,\
            __func__,"CE_RES_UPDATION");
    return overlapRBs;
}


/*****************************************************************************
 * Function Name  : updateCCAvailRes 
 * Inputs         : availRes_p - pointer to the available resources of 
 *                               CC region,
 *                  resourceReserved_p - Pointer to AllocatedResInfo 
 * Outputs        : CC resources and indexes are updated  
 * Returns        : MAC_SUCCESS  - Allocation made on CC region and it is 
 *                                 updated accordingly
 * Description    : This function updates CC region resources and indexes if 
 *                  allocation is done on CC region.
 *****************************************************************************/
UInt8 updateCCAvailRes(AvailableResInfo *availRes_p,
        AllocatedResInfo *resourceReserved_p)
{
    /* RBs has to be deducted from CC_CE_REGION always,
     * as we assume that it also contains CE_REGION */
    availRes_p->freeRBCounter -= resourceReserved_p->allocatedLength;
    /* freeRBIndex will be moved only when it coincides with the start of the 
     * resoruce allocation,This will happen only when CE region falls first*/\
    if( availRes_p->freeRBIndex ==
            resourceReserved_p->allocatedIndex)
    {
        availRes_p->freeRBIndex =
            resourceReserved_p->allocatedIndex + resourceReserved_p->allocatedLength;
    }
    /* endIndex will be moved only when it coincides with the end of the resource 
     * allocation,This will happen only when CE region falls last,
     * minus 1 is done inorder to equallize endindex*/
    if(availRes_p->endIndex  ==
            (resourceReserved_p->allocatedIndex + resourceReserved_p->allocatedLength -1))
    {
        availRes_p->endIndex = resourceReserved_p->allocatedIndex - 1;
    }
    LOG_MAC_MSG(MAC_UPD_CC_AVAIL_RES_INFO,LOGDEBUG,MAC_PUSCH,\
            getCurrentTick(),\
            availRes_p->freeRBIndex,\
            availRes_p->endIndex,\
            availRes_p->freeRBCounter,\
            0,\
            0,\
            0.0,0.0,\
            __func__,"CC_RES_UPDATION");
    return resourceReserved_p->allocatedLength;
}
/* ICIC changes end */

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : processULSPSUeInPUSCH
 * Inputs         : ulDelay,
 *                  containerTick,
 *                  subFrameNumWithoutAdvance,
 *                  currentGlobalTick,
 *                  numberOfULUEsToBeScheudled_p,
 *                  hiIndex,
 *                  bufferRBs_p,
 *                  locType
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None  
 * Returns        : None
 * Description    : This function updates ULSCH info for SPS UE
 *****************************************************************************/
             void processULSPSUeInPUSCH(
                    UInt8 ulDelay,
                    UInt8 containerTick,
                    /* +- SPR 17777 */
                    /* SPR 15909 fix start */
                    tickType_t currentGlobalTick,
                    /* SPR 15909 fix end */
                    /* +- SPR 17777 */
                    UInt8 hiIndex,
                    SInt8 *bufferRBs_p,
                    UInt8 locType,
                    /*CA Changes start  */
                    InternalCellIndex internalCellIndex,
                    /*CA Changes end  */
                    ExecutionLegFunctionArg* execArgs)
{

    UInt16 tempUeId=0;
    ULUEContext* ulUEContext_p=PNULL;
    UInt8 tempHarqProcessId = INVALID_HARQ_ID;
    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p = PNULL;
    DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    /* Review comment fix start GG16 */
    UInt8 rbMapNum = INVALID_RB_MAP_NUM; /* Invalid Value */
    /* Review comment fix end GG16 */
    UInt16 numberOfInfoPresent = 0;
    ULSchUciRachPoolInfo* ulschUciRachInfo_p 
        = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;

    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;

    UInt32 minPowerFlag = 0;
    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */

    UInt32 localCount = 0;
    ULGrantInfo ulGrantInfo = {0};
    UInt32 tempPUSCHResources = 0;
    UInt32 loopCount = 0;
    PdcchULNewUE *pdcchULNewUE_p = PNULL;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    UInt8 subFrameNum  = (currentGlobalTick + FDD_HARQ_OFFSET + ulDelay )% MAX_SUB_FRAME; /* SS_S2 */


    rbMapNum = MODULO_SIXTEEN(currentGlobalTick + ulDelay + FDD_HARQ_OFFSET); /* SS_S2 */

    /*CA Changes start  */
    PdcchULOutputInfo* tempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];
    /* Time Averaging changes start */
    UInt32 ueMaxPowerFlag = 0;
    /* Time Averaging changes end */

    /*
       Since the harq process ID will remain same irrespetive of the UE for a particular 
       tick, hence we derive the same here and place it into a local variable
       */
    tempHarqProcessId = MODULO_EIGHT(currentGlobalTick + ulDelay + FDD_HARQ_OFFSET);

    /*
       Among the newly scheduled UE we first consider those
       for which there has been collision and for whom the CCE
       has been allocated by PDCCH  
       */


    /* SPR 15909 fix start */
    tickType_t            enqueueTick        = 0;
    /* SPR 15909 fix end */
    UInt8             normalUeHiIndex    = hiIndex ;
    /* ICIC changes start */
    /*CA Changes start  */
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum; 
    /*CA Changes end  */
    /* loop for 2 times */
    tempPUSCHResources = 
        rbMapNode_p->availableResourceInfo[locType].freeRBCounter;
    localCount = 0;
    /*
       We now consider the case of NEW UEs that has been scheduled by the Strategy this time 
       but no collision is reported and for whom the CCE has been allocated by PDCCH
       */
    /* ICIC changes start */
    loopCount = tempPdcchULOutput_p->pdcchULNewUEInfo[locType].count; 
    /* ICIC changes end */ 
    /* + CQI_5.0 */
    UInt8 callSemiStaticforAperiodicFlag = FALSE;
    /* - CQI_5.0 */
    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, \
            "[%s] #####  No. of NewUE = %d  "
            "And No. of UL UE to be scheduled = %d loopCount = %d \n"
            ,__func__,loopCount, *numberOfULUEsToBeScheudled_p,loopCount);

    while((loopCount--))          
    {
        /* Time Averaging changes start */
        ueMaxPowerFlag = 0;
        /* Time Averaging changes end */
        /* ICIC changes start */ 
        pdcchULNewUE_p = &(tempPdcchULOutput_p->pdcchULNewUEInfo[locType].pdcchULNewUEArray[localCount]);
        /* ICIC changes end */ 

        tempUeId = pdcchULNewUE_p->ueIndex;

        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeId];        
        tempDirectIndexingUEInfo_p = \
                                     &(tempPdcchULOutput_p->directIndexingUEInfo[tempUeId]); 


        if ( (tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_OCCASION) &&
                (tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_ACTIVATION)&& 
                (tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_DEACTIVATION) 
                &&(tempDirectIndexingUEInfo_p->ulGrantRequestType != TTIB_UL_SPS_OCCASION)
           )
        {
            localCount++;

            continue; 
        }      
        /* ICIC changes start */                             
        if((tempDirectIndexingUEInfo_p->flag != UNSET_FLAG) && 
                (tempDirectIndexingUEInfo_p->flag != MEAS_GAP_RE_TX_NO_CCE_NO_GRANT))
        {
            /* ICIC changes end */
            /*
               we call the function for allocating PUSCH resources
               for Newly Scheduled UEs without any collision
               */
            if (!ulUEContextInfo_p->pendingDeleteFlag)
            {
                ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
                /* TTIB_Code Start*/
                /* Grant is TTIBundled or not */
                if(TTIB_UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType ||
                        ((ulUEContext_p->ttiBundlingEnabled )&&
                         ((UL_SPS_ACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                          (UL_SPS_DEACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) )) )
                {
                    tempHarqProcessId = GET_TTIB_HARQ_ID(currentGlobalTickWithULDelay + FDD_HARQ_OFFSET);
                    hiIndex = (hiIndex + TTIB_FIRST_TO_LAST_TICK_OFFSET) % MAX_PHICH_CONTAINER; 
                    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH,
                            "[%s] [%d] SPS TTIB HARQ Process ID %d hiIndex %d  \n.",__func__,getCurrentTick(),tempHarqProcessId,hiIndex);             
                }
                else
                {
                    tempHarqProcessId = MODULO_EIGHT(currentGlobalTickWithULDelay + FDD_HARQ_OFFSET);
                    hiIndex = normalUeHiIndex ; 
                    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH,
                            "[%s] [%d] Non TTIB SPS HARQ Process ID %d \n.",__func__,getCurrentTick(),tempHarqProcessId);             
                }
                /* TTIB_Code End*/

                ulUEContext_p->isPdcchAllocated = 0;



                /*
                   We now populate the structure that would be passed to the function 
                   processULGrants.
                   */
                ulGrantInfo.tempUeIndex = tempUeId;
                ulGrantInfo.ulUEContext_p = ulUEContext_p;
                ulGrantInfo.tempHarqProcessId = tempHarqProcessId;
                ulGrantInfo.subFrameNumber = rbMapNum;
                ulGrantInfo.tempDciCCEInfo_p = tempDciCCEInfo_p;
                ulGrantInfo.tempDirectIndexingUEInfo_p = tempDirectIndexingUEInfo_p;
                ulGrantInfo.pdcchULNewUE_p = pdcchULNewUE_p;

                /*Fix for SPR 12477, passing subFrameNum*/   
                if (MAC_FAILURE == processULGrants(&ulGrantInfo,
                            currentGlobalTick,
                            &tempPUSCHResources,
                            bufferRBs_p,
                            /* Time Averaging changes start */
                            powerCorrectionAndTpc,
                            /* Rel9_upgrade_CR410 */
                            ulDelay,
                            /* Rel9_upgrade_CR410 */
                            &ueMaxPowerFlag,
                            /* Time Averaging changes end */
                            /* CR changes start */
                            hiIndex, &minPowerFlag,TRUE,
                            internalCellIndex,
                            subFrameNum,execArgs))
                    /* CR changes end */
                {
                    tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;                       
                    tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                } 
                else
                {
                    /* TTIB_Code Starts*/
                    /*Enqueue 3 nodes for next three ticks of bundle */
                    if( (TTIB_UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                            ((ulUEContext_p->ttiBundlingEnabled )&&
                             ((UL_SPS_ACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType))) )
                    {
                        enqueueTick = currentGlobalTickWithULDelay ;
                        enqueueNodesInTTIBScheduleQ(enqueueTick,tempUeId,tempHarqProcessId,1,TRUE,internalCellIndex);
                    } 
                    /* TTIB_Code Ends */

                    /* Reset the tpcTriggeredValue in UE Context */
                    ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue = 
                        TPC_NO_CHANGE;

                    /* +CLPC_CHG_LJA */

                    if (powerCorrectionAndTpc.powerCorrectionVal)
                    {
                        ulUEContext_p->prevPowerCorrectionForPusch =
                            ulUEContext_p->currPowerCorrectionForPusch;

                        /* Time Averaging changes start */
                        if (ueMaxPowerFlag == FALSE)
                        {
                            if (ulUEContext_p->dedicatedULPowerControlInfo.\
                                    accumulationEnabled)
                            {
                                ulUEContext_p->currPowerCorrectionForPusch +=
                                    powerCorrectionAndTpc.powerCorrectionVal;
                                CHECK_AND_UPDATE_POWER_CORRECTION_FOR_PUSCH(\
                                        ulUEContext_p->currPowerCorrectionForPusch);
                            }
                            else
                            {
                                ulUEContext_p->currPowerCorrectionForPusch =
                                    powerCorrectionAndTpc.powerCorrectionVal;
                            }

                            /* Store the TPC value in UE Context */
                            ulUEContext_p->powerControlPuschTPCValue =
                                powerCorrectionAndTpc.powerCorrectionVal; 
                        }
                    }
                    ulUEContext_p->powerControlDeltaTFAndLogFactor[subFrameNum] = \
                                                                                  fetchDeltaTFandLogMFactor(ulUEContext_p->dedicatedULPowerControlInfo.deltaMcsEnabled, 
                                                                                          ulUEContext_p->ulresInfo[tempHarqProcessId].mcsIndex,
                                                                                          ulUEContext_p->ulresInfo[tempHarqProcessId].riLength);
                    /* -CLPC_CHG_LJA */
                    tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                    /*START:6172*/
                    ulUEContext_p->advlastScheduledTick = currentGlobalTick + ulDelay;
                    /*END:6172*/
                    /* DRX_CHG */
                    if (SETUP == ulUEContext_p->drxConfigType)
                    {
                        putEntryInDRXULSchedulerEventQueue(tempUeId, 
                                currentGlobalTick + ulDelay, internalCellIndex);
                    }
                    /* DRX_CHG */

                    /* + CQI_5.0 */
                    if ( (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG != tempDirectIndexingUEInfo_p->ulGrantRequestType) &&
                            /* SPS changes */ 
                            (UL_SPS_DEACTIVATION != tempDirectIndexingUEInfo_p->ulGrantRequestType) )
                        /* SPS changes */ 
                    {

#ifdef ULHARQ_TIMER_PROC
                        UInt32 ulHarqSysFrameNum = 0;
                        UInt32 ulHarqSubFrameNum = 0; 
                        /* SPR 15909 fix start */
                        tickType_t timerExpiredTTI = 0; 
                        /* SPR 15909 fix end */
                        UInt32 timerExpsubFrameNum = 0;

                        getSFAndSFN(&ulHarqSysFrameNum, &ulHarqSubFrameNum, internalCellIndex);    

                        /** TTIB_Code Starts */
                        timerExpiredTTI = (ulUEContext_p->ttiBundlingEnabled)?
                            (currentGlobalTickWithULDelay + TTIB_ULHARQ_TIMER_TTI_TX_OFFSET):
                            (currentGlobalTickWithULDelay + START_ULHARQ_TIMER_OFFSET);

                        timerExpsubFrameNum = (ulUEContext_p->ttiBundlingEnabled)?
                            (ulHarqSubFrameNum + ulDelay + TTIB_ULHARQ_TIMER_TTI_TX_OFFSET):
                            (ulHarqSubFrameNum + ulDelay + START_ULHARQ_TIMER_OFFSET);
                        /** TTIB_Code End */


                        if(timerExpsubFrameNum >= MAX_SUB_FRAME)
                        {
                            timerExpsubFrameNum = timerExpsubFrameNum % MAX_SUB_FRAME;

                            ulHarqSysFrameNum++;
                            if (ulHarqSysFrameNum == MAX_SFN_VALUE)
                            {
                                ulHarqSysFrameNum = 0;
                            }
                        }    

                        ulHarqTimerStart(tempUeId,tempHarqProcessId,timerExpsubFrameNum, 
                                /* + SPS_TDD_Changes */
                                ulUEContext_p->schType,
                                /* + SPS_TDD_Changes */
                                timerExpiredTTI,ulHarqSysFrameNum, internalCellIndex);

                        LTE_MAC_UT_LOG(LOG_INFO,LOG_PUSCH,
                                "\n[%s] [%d] tempUeId %d tempHarqProcessId %d  timerExpsubFrameNum %d"
                                "timerExpiredTTI %d ulHarqSysFrameNum %d\n "
                                ,__func__,getCurrentTick(),tempUeId,tempHarqProcessId,
                                timerExpsubFrameNum,timerExpiredTTI,ulHarqSysFrameNum);
#endif


                    }
                    /* CA Stats Changes Start */
                    /* - CQI_5.0 */
                    LTE_MAC_UPDATE_STATS_FOR_SPS_PUSCH_RESERVE( ulUEContext_p,
                            getCurrentTick(), internalCellIndex);
                    /* CA Stats Changes End */

                    if( (ULSCH_UCI_INVALID_INDEX == tempDirectIndexingUEInfo_p->ulschUciInfoIndex) &&
                            /* SPS CHANGES */
                            (UL_SPS_DEACTIVATION != tempDirectIndexingUEInfo_p->ulGrantRequestType) )
                    {

                        /* Semi static changes End*/
                        if ( MAX_ULSCH_UCI_SUPPORTED ==  numberOfInfoPresent)
                        {
                            ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                                    __func__);
                            break;
                        }
                        /*SPR 7086 Fix Start*/
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS =
                            nSRSDetermineOnCollision(ulUEContext_p, tempHarqProcessId,
                                    subFrameNum, internalCellIndex);
                        /*Update nSrsInitial in HARQ Context*/
                        ulUEContext_p->ulresInfo[tempHarqProcessId].nSrsInitial =
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS;
                        /*SPR 7086 Fix End*/
                        /* Fill UL Harq Info*/
                        ulschUciRachInfo_p->
                            ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p = 
                            &(ulUEContext_p->ulresInfo[tempHarqProcessId]);

                        ulschUciRachInfo_p->
                            ulschUciInfo[numberOfInfoPresent].ulHarqProcessId =
                            tempHarqProcessId;
                        if ( (tempDirectIndexingUEInfo_p->ulGrantRequestType == UL_SPS_ACTIVATION) ||
                                (tempDirectIndexingUEInfo_p->ulGrantRequestType == UL_SPS_OCCASION) 
                                ||(tempDirectIndexingUEInfo_p->ulGrantRequestType == TTIB_UL_SPS_OCCASION)
                           )
                        {
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = ulUEContext_p->spsUlInfo.SpsCrnti;
                        }
                        else
                        {
                            /* SPR 1723 changes start */
                            ulschUciRachInfo_p->
                                ulschUciInfo[numberOfInfoPresent].rnti =
                                ulUEContext_p->crnti;
                            /* SPR 1723 changes end */
                        }

                        if( CQI_WITH_DATA_ULGRANT_REQ_MSG == 
                                tempDirectIndexingUEInfo_p->ulGrantRequestType
                                || CQI_WITHOUT_DATA_ULGRANT_REQ_MSG == 
                                tempDirectIndexingUEInfo_p->ulGrantRequestType 
                                /* + CQI_5.0 */
                                /* + SPR_14430 */
                                || (ulUEContext_p->aperiodicCQIToBeRequested) )
                            /* - SPR_14430 */
                            /* - CQI_5.0 */
                        {
                            /* SPR 1115 start */                        
                            ulschUciRachInfo_p->
                                ulschUciInfo[numberOfInfoPresent].pduType = 
                                MAC_SS_PDU_ULSCH_CQI_RI;
                            storeCqiPmiRiDataAperiodic(
                                    tempDirectIndexingUEInfo_p->transMode, 
                                    ulUEContextInfo_p, ulschUciRachInfo_p, subFrameNum, 
                                    numberOfInfoPresent, internalCellIndex);
                        }
                        else
                        {
                            ulschUciRachInfo_p->
                                ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_ULSCH; 
                        }
                        LOG_UT(MAC_UCI_ULSCH_PDU_ASSIGN,LOGDEBUG,MAC_PUSCH,\
                                getCurrentTick(),\
                                tempUeId,
                                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType,\
                                tempDirectIndexingUEInfo_p->ulschUciInfoIndex,\
                                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId,\
                                0,0.0,0.0,\
                                __func__,"ULSCH_PDU_ASSIGN");
                        numberOfInfoPresent++;
                    }
                    else if (tempDirectIndexingUEInfo_p->ulGrantRequestType != UL_SPS_DEACTIVATION)
                    {
                        /* + CQI_5.0 */
                        if ( CQI_WITHOUT_DATA_ULGRANT_REQ_MSG ==
                                tempDirectIndexingUEInfo_p->ulGrantRequestType
                                /* + SPR_14430 */
                                || (ulUEContextInfo_p->ulUEContext_p->
                                    /* - SPR_14430 */
                                    aperiodicCQIToBeRequested) )
                        {
                            callSemiStaticforAperiodicFlag = TRUE;
                        }    
                        /* - CQI_5.0 */
                        /* ICIC changes start */
                        fillSemiStaticInfoForULSCH ( ulschUciRachInfo_p,
                                tempDirectIndexingUEInfo_p, 
                                ulUEContextInfo_p,
                                &(ulUEContext_p->ulresInfo[tempHarqProcessId]),
                                tempHarqProcessId,
                                subFrameNum,
                                callSemiStaticforAperiodicFlag,
                                internalCellIndex);
                        /* SPR 10195 Fix Start */
                        /*Update nSrsInitial in HARQ Context*/
                        ulUEContext_p->ulresInfo[tempHarqProcessId].nSrsInitial =
                            ulschUciRachInfo_p->ulschUciInfo[tempDirectIndexingUEInfo_p->ulschUciInfoIndex].nSRS;
                        /* SPR 10195 Fix End */
                        /* ICIC changes end */
                        /* SPR 11152 Fix Start */
                        callSemiStaticforAperiodicFlag = FALSE;
                        /* SPR 11152 Fix End */
                    }
                    /* SS_S2 */

                    /* Semi static changes End*/
                }                  
            }            
            else
            { 
                /*  
                    we must revert back the cce allocated by 
                    PDCCH as the UE is on Deletion by the RRC
                    */
                tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            }
        }
        localCount++;

        /* + CQI_5.0 */
        if(ulUEContext_p)
        {
            ulUEContext_p->aperiodicCQIToBeRequested = FALSE;
        }
        /* - CQI_5.0 */
    }
    /*Update Number of numberOfInfoPresent in the global context */
    ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;

    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH, "[%s] Exit...... \n",__func__);
}
/* + SPS_TDD_Changes */
#elif TDD_CONFIG
/********************************************************************************
 * Function Name : processULSPSUeInPUSCH 
 * Inputs        : tempPdcchULOutput_p - Pointer to PdcchULOutputInfo,
 *                 containerTick,
 *                 currentGlobalTick - Global Tick,
 *                 subFrameNumber - Subframe number,
 *                 rbMapNum,
 *                 ulGrantSysFrameNum,
 *                 ulGrantSubFrameNum
 *                 bufferRBs_p,
 *                 locType
 * Outputs       : None
 * Returns       : None
 * Description:  : This function will process the New UL UE in PUSCH.
 ********************************************************************************/
                         void processULSPSUeInPUSCH(PdcchULOutputInfo* tempPdcchULOutput_p, 
                                UInt8 containerTick,
                                UInt32 currentGlobalTick,
                                UInt8 subFrameNum,
                                UInt8 rbMapNum,
                                /*SPR 5620 START*/
                                UInt16 ulGrantSysFrameNum,
                                /*SPR 5620 END*/
                                UInt16 ulGrantSubFrameNum,
                                SInt8 *bufferRBs_p,
                                UInt8 locType,
                                InternalCellIndex internalCellIndex,
                                ExecutionLegFunctionArg* execArgs
                                )
{
    UInt16 tempUeId = 0;
    ULUEContext* ulUEContext_p = PNULL;
    DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    UeScheduledInfo *ueScheduledInfo_p         = PNULL;
    PdcchULNewUE *pdcchULNewUE_p = PNULL;
    ULGrantInfo ulGrantInfo                    = {0};
    UInt16 numberOfInfoPresent = 0;
    UInt32 ueMaxPowerFlag = 0;
    UInt32 ueMinPowerFlag = 0;
    UInt32 tempPUSCHResources = 0;
    UInt32 loopCount = 0;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    UInt16 sysFrameNum = (currentGlobalTick / MAX_SUB_FRAME) % MAX_SFN_VALUE;
    UInt32 localCount = 0;

    /* TDD Config 6 Changes Start*/
    UInt8 ulIndexFac = MODULO_TWO(ulGrantSysFrameNum);

    /*TDD Config 0 Changes Start*/
    ULSchUciRachPoolInfo* ulschUciRachInfo_p
        = ulSchUciRachContainer_gp[internalCellIndex] + (ulGrantSubFrameNum % MAX_ULSCH_UCI_CONTAINER);
    /*TDD Config 0 Changes End*/

    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    UInt8 callSemiStaticforAperiodicFlag = FALSE;

    LTE_MAC_UT_LOG(LOG_CRITICAL, LOG_PUSCH,
            "[%s] Entry containerTick[%d] currentGlobalTick[%d] ulGrantSubFrameNum [%d] \n",
            __func__, containerTick, currentGlobalTick, ulGrantSubFrameNum);

    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    /*CA Changes start  */
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum; 
    /*CA Changes end  */
    tempPUSCHResources =
        rbMapNode_p->availableResourceInfo[locType].freeRBCounter;
    localCount = 0;
    loopCount = tempPdcchULOutput_p->pdcchULNewUEInfo[locType].count;
    while((loopCount--))          
    {
        /* Time Averaging changes start */
        ueMaxPowerFlag = 0;

        ueMinPowerFlag = 0;

        pdcchULNewUE_p = &(tempPdcchULOutput_p->pdcchULNewUEInfo[locType].pdcchULNewUEArray[localCount]);
        tempUeId = pdcchULNewUE_p->ueIndex;
        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeId];
        ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

        if ( PNULL == ulUEContext_p)
        {
            ltePanic("\n [%s] ulUEContext_p is null\n",__func__);
            return;
        }

        /*TDD Config 0 Changes Start*/
        /** SPR 14204 Fix : Start **/
        ueScheduledInfo_p = ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulGrantSubFrameNum];
        /** SPR 14204 Fix : End **/
        /*TDD Config 0 Changes End*/

        if ( PNULL == ueScheduledInfo_p)
        {
            ltePanic("\n [%s] ueScheduledInfo_p is null\n",__func__);
            /* + Coverity 88286 */
            break; 
            /* - Coverity 88286 */
        }
        LTE_MAC_UT_LOG( LOG_CRITICAL, LOG_PUSCH, "\nsubFrameNumber [%d] ueScheduledInfo_p[%p]\n",
                subFrameNum, ueScheduledInfo_p);
        if((ueScheduledInfo_p->ulGrantRequestType != UL_SPS_OCCASION )&&
                (ueScheduledInfo_p->ulGrantRequestType != UL_SPS_ACTIVATION)&&
                (ueScheduledInfo_p->ulGrantRequestType != UL_SPS_DEACTIVATION))
        {
            localCount++;

            continue; 
        }
        if((ueScheduledInfo_p->flag != UNSET_FLAG) &&
                (ueScheduledInfo_p->flag != MEAS_GAP_RE_TX_NO_CCE_NO_GRANT))
        {
            if (!ulUEContextInfo_p->pendingDeleteFlag)
            {
                ulUEContext_p->isEntryPushedInAdvanceQueue = 0;
                ulUEContext_p->isPdcchAllocated = 0;
                /*SPS_TDD_Changes*/
                if((ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId].riLength != 0) ||
                        (INVALID_HARQ != ulUEContext_p->ulHarqProcessIdToPuschSubFrameMAp[ulIndexFac][ulGrantSubFrameNum]))
                {
                    /* SPR 16916 FIX START */
#ifdef KPI_STATS
                    freeULHarqProcess(&ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId],
                            ulUEContext_p->ueIndex,internalCellIndex);
#else
                    freeULHarqProcess(&ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId]);
#endif
                    /* SPR 16916 FIX END */
                    resetHarqId(ulGrantSubFrameNum,ulUEContext_p, internalCellIndex,ulGrantSysFrameNum );
                }
                /*SPS_TDD_Changes*/

                ulGrantInfo.tempUeIndex       = tempUeId;
                ulGrantInfo.ulUEContext_p     = ulUEContext_p;
                ulGrantInfo.tempHarqProcessId = ueScheduledInfo_p->ulHarqProcessId;
                ulGrantInfo.ulSubframe        = ulGrantSubFrameNum;
                ulGrantInfo.rbMapNum          = rbMapNum;
                ulGrantInfo.tempDciCCEInfo_p  = tempDciCCEInfo_p;
                ulGrantInfo.ueScheduledInfo_p = ueScheduledInfo_p;
                ulGrantInfo.pdcchULNewUE_p = pdcchULNewUE_p;
                if (MAC_FAILURE == processULGrants(&ulGrantInfo,
                            currentGlobalTick,
                            &tempPUSCHResources,
                            bufferRBs_p,
                            /* Time Averaging changes start */
                            powerCorrectionAndTpc,
                            /* Rel9_upgrade_CR410 */
                            subFrameNum,
                            /* Rel9_upgrade_CR410 */
                            /*TDD Config 0 Changes Start*/
                            //ulGrantSubFrameNum, //TDD SB_CQI
                            /*TDD Config 0 Changes End*/
                            /* + SPS_TDD_Changes */
                            &ueMaxPowerFlag,&ueMinPowerFlag,TRUE,
                            /* - SPS_TDD_Changes */
                            internalCellIndex,execArgs))
                {
                    /* + SPS_TDD_Changes */
                    mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                            dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                            internalCellIndex);
                    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                    ueScheduledInfo_p->flag = UNSET_FLAG;
                    /* - SPS_TDD_Changes */
                }
                else
                {
                    ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue = 
                        TPC_NO_CHANGE;
                    if (powerCorrectionAndTpc.powerCorrectionVal)
                    {
                        ulUEContext_p->prevPowerCorrectionForPusch =
                            ulUEContext_p->currPowerCorrectionForPusch;
                        CHECK_AND_UPDATE_FI_CORRECTION(
                                ueMaxPowerFlag,
                                ueMinPowerFlag,
                                powerCorrectionAndTpc,
                                ulUEContext_p)

                    }

                    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                    /* TDD REL 3.1.1 */
                    ueScheduledInfo_p->flag = UNSET_FLAG;
                    if((CQI_WITHOUT_DATA_ULGRANT_REQ_MSG != ueScheduledInfo_p->ulGrantRequestType)&&
                            (UL_SPS_DEACTIVATION != ueScheduledInfo_p->ulGrantRequestType))
                    {
                        ulHarqTimerStart( subFrameNum,
                                sysFrameNum,
                                tempUeId,
                                ulGrantInfo.tempHarqProcessId,
                                /* + SPS_TDD_Changes */
                                //SPR_16897_Changes
                                SPS_SCHEDULING,
                                //SPR_16897_Changes
                                /* - SPS_TDD_Changes */
                                /*CA Changes start  */
                                internalCellIndex,
                                /*CA Changes end  */
                                /*TDD Config 0 Changes Start*/
                                ulGrantSubFrameNum
                                /* TDD Config 0 Changes Start*/
                                );
                    }
                    /*START:6172*/
                    ulUEContext_p->advlastScheduledTick = currentGlobalTick;
                    /*END:6172*/
                    /* DRX_CHG */
                    if (SETUP == ulUEContext_p->drxConfigType)
                    {
                        putEntryInDRXULSchedulerEventQueue(tempUeId,
                                currentGlobalTick, internalCellIndex);
                    }
                    LTE_MAC_UPDATE_STATS_FOR_SPS_PUSCH_RESERVE( ulUEContext_p, getCurrentTick(), internalCellIndex);
                    if((ULSCH_UCI_INVALID_INDEX == ueScheduledInfo_p->ulschUciInfoIndex) &&
                            (UL_SPS_DEACTIVATION != ueScheduledInfo_p->ulGrantRequestType))
                    {

                        if ( MAX_ULSCH_UCI_SUPPORTED ==  numberOfInfoPresent)
                        {
                            ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                                    __func__);
                            break;
                        }
                        /*SPR 7086 Fix Start*/
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS =
                            nSRSDetermineOnCollision(ulUEContext_p, ulGrantInfo.tempHarqProcessId, 
                                    ulGrantSubFrameNum, internalCellIndex);
                        /*Update nSrsInitial in HARQ Context*/
                        ulUEContext_p->ulresInfo[ulGrantInfo.tempHarqProcessId].nSrsInitial =
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS;
                        /*SPR 7086 Fix End*/
                        /*Fill rnti value */
                        /*  + SPS_TDD_Changes */
                        if((UL_SPS_ACTIVATION == ueScheduledInfo_p->ulGrantRequestType) ||
                                (UL_SPS_OCCASION == ueScheduledInfo_p->ulGrantRequestType)) /*CID 110673*/
                            /* SPR 20115 Fix Start */
                        { 
                            /* SPR 20115 Fix End */
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = 
                                ulUEContext_p->spsUlInfo.SpsCrnti;
                            /* SPR 20115 Fix Start */
                        }
                        else
                        {
                            /* If SPS is enabled in the system and UE is not a SPS-UE then rnti should be 
                             * non-SPS rnti */
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = 
                                ulUEContext_p->crnti;
                        }
                        /* SPR 20115 Fix End */
                        /*  - SPS_TDD_Changes */
                        /* Fill UL Harq Info*/
                        ulschUciRachInfo_p->
                            ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p =
                            &(ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId]);

                        ulschUciRachInfo_p->
                            ulschUciInfo[numberOfInfoPresent].ulHarqProcessId =
                            ueScheduledInfo_p->ulHarqProcessId;
                        if ( CQI_WITH_DATA_ULGRANT_REQ_MSG ==
                                ueScheduledInfo_p->ulGrantRequestType
                                || CQI_WITHOUT_DATA_ULGRANT_REQ_MSG ==
                                ueScheduledInfo_p->ulGrantRequestType
                                /* TDD SB_CQI */
                                || (TRUE == ulUEContext_p->aperiodicCQIToBeRequested) )
                            /* TDD SB_CQI */
                        {
                            /* SPR 1115 CHG */
                            ulschUciRachInfo_p->
                                ulschUciInfo[numberOfInfoPresent].pduType =
                                MAC_SS_PDU_ULSCH_CQI_RI;

                            storeCqiPmiRiDataAperiodic(
                                    ueScheduledInfo_p->transMode,
                                    ulUEContextInfo_p, ulschUciRachInfo_p,
                                    /*SPR 5620 START*/
                                    ulGrantSysFrameNum,
                                    /*SPR 5620 END*/
                                    ulGrantSubFrameNum, numberOfInfoPresent,
                                    internalCellIndex);
                        }
                        else
                        {
                            ulschUciRachInfo_p->
                                ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_ULSCH;
                        }
                        numberOfInfoPresent++;
                    }
                    else if(ueScheduledInfo_p->ulGrantRequestType != UL_SPS_DEACTIVATION)
                    {
                        /* TDD SB_CQI */
                        if ( CQI_WITHOUT_DATA_ULGRANT_REQ_MSG ==
                                ueScheduledInfo_p->ulGrantRequestType
                                || (TRUE == ulUEContextInfo_p->ulUEContext_p->aperiodicCQIToBeRequested) )
                        {
                            callSemiStaticforAperiodicFlag = TRUE;
                        }
                        /* TDD SB_CQI */
                        /* ICIC changes start */
                        fillSemiStaticInfoForULSCH ( ulschUciRachInfo_p,
                                ueScheduledInfo_p,
                                ulUEContextInfo_p,
                                &(ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId]),
                                ueScheduledInfo_p->ulHarqProcessId,
                                /*SPR 5620 START*/
                                ulGrantSysFrameNum,
                                /*SPR 5620 END*/
                                ulGrantSubFrameNum,
                                callSemiStaticforAperiodicFlag,
                                internalCellIndex);
                        /* ICIC changes end */
                        /* TDD SB_CQI */
                        /* SPR 10195 Fix Start */
                        /*Update nSrsInitial in HARQ Context*/
                        ulUEContext_p->ulresInfo[ueScheduledInfo_p->ulHarqProcessId].nSrsInitial =
                            ulschUciRachInfo_p->ulschUciInfo[ueScheduledInfo_p->ulschUciInfoIndex].nSRS;

                        ueScheduledInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
                        /* SPR 10195 Fix End */
                        /* SPR 11152 Fix Start */
                        callSemiStaticforAperiodicFlag = FALSE;
                        /* SPR 11152 Fix End */
                    }
                }
            }
            else
            {
                /* + SPS_TDD_Changes */
                mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                        dciPduInfo.aggregationLvl,
                        (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                        internalCellIndex);
                ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                ueScheduledInfo_p->flag = UNSET_FLAG;
                /* - SPS_TDD_Changes */
            }
        }
        /* + SPS_TDD_Changes */
        else
        {
            /* + SPS_TDD_Changes */
            mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                    dciPduInfo.aggregationLvl,
                    (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                    internalCellIndex);
            ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            ueScheduledInfo_p->flag = UNSET_FLAG;
            /* - SPS_TDD_Changes */
        }
        /* - SPS_TDD_Changes */

        localCount++;
        ulUEContext_p->aperiodicCQIToBeRequested = FALSE;
    }
    /*Update Number of numberOfInfoPresent in the global context */
    ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;
}
#endif
/* - SPS_TDD_Changes */
/* SPR 5557 changes start*/
/* SPR 5557 changes end*/


/*UL_MU_MIMO_CHG Start*/

/********************************************************************************
 * Function Name    :  validateNprbFromATBULMUMIMO
 * Inputs           :  inputRBs - Represents the transmission bandwidth,
 *                     inputMCS - Represents the MCS of the transmission bandwidth,
 *                     ulUEContext_p - Pointer to UE's UL Context,
 *                     currentTick - Current Tick
 *                     internalCellIndex - Cell-Index used at MAC
 * Outputs          :  None
 * Returns          :  MAC_SUCCESS/MAC_FAILURE
 * Description      :  Based on the input of RBs and MCS required for the UE, this function
 *                     shall validate the scheduled transmission  bandwidth and MCS 
 *                     does not exceed max Power of the UE. And accordingly return 
 *                     SUCCESS or FAILURE.
 *********************************************************************************/
                                STATIC MacRetType  validateNprbFromATBULMUMIMO(UInt8 inputRBs, 
                                        UInt8 inputMCS,
                                        ULUEContext *ulUEContext_p,
                                        /* SPR 15909 fix start */
                                        tickType_t currentTick,
                                        /* SPR 15909 fix end */
                                        /*CA Changes start  */
                                        InternalCellIndex internalCellIndex
                                        /*CA Changes end  */
#ifdef TDD_CONFIG                                        
                                        ,UInt8  ulSubFrameNumber
#endif                                       
                                        )
{
    SInt16 tenLogMMax = 0;
    UInt8 tenLogMAllocated = tenLogMFactor_g[inputRBs - 1] ;
    SInt8 deltaTF = 0;
    TpcTriggeredToPowerCorrection tpc = {0};
    if(ulUEContext_p->dedicatedULPowerControlInfo.deltaMcsEnabled)
    {
        deltaTF = mcsToDeltaTF_g[inputMCS];
    }
    else
    {
        deltaTF = 0;
    }
    tpc = getTpcForPusch(ulUEContext_p, currentTick, internalCellIndex
#ifdef TDD_CONFIG    
            ,ulSubFrameNumber
#endif                  
            );
    tenLogMMax = ulUEContext_p->pcMax
        - (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->commonULPowerControlInfo.p0NominalPUSCH + \
                ulUEContext_p->dedicatedULPowerControlInfo.p0UePusch)
        - (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->commonULPowerControlInfo.alpha * ulUEContext_p->pathLoss)/10
        - deltaTF
        /* Time Averaging changes start */
        - (ulUEContext_p->currPowerCorrectionForPusch + tpc.powerCorrectionVal);
    /* Time Averaging changes end */

    if(tenLogMMax >= tenLogMAllocated)
    {
        return MAC_SUCCESS;
    }
    else
    {
#ifdef SSI_DEBUG_STATS
        /* CA Stats Changes Start */
        LTE_MAC_UPDATE_FAILURE_CAUSE(POWER_CHECK_FAILURE, internalCellIndex);
        /* CA Stats Changes End */
#endif
        return MAC_FAILURE;
    }
}
/********************************************************************************
 * Function Name :     createBuddyPairFromCandidate
 * Inputs:             pdcchULOutputInfo_p - pointer to PdcchULOutputInfo
 *                     currentTick - The present Global Tick+ ulDelay
 *                     subFrameNumber(TDD Only) -
 *                     internalCellIndex - Cell-Index used at MAC
 * Outputs:            MAC_SUCCESS /MAC_FAILURE
 * Description:        This function will work for delay path 2 on per node basis  
 *                     This would be the main calling function of the entire PUSCH
 *                     and then the ACKs.  
 *********************************************************************************/
                                STATIC void createBuddyPairFromCandidate(PdcchULOutputInfo *pdcchULOutputInfo_p, 
                                        /* SPR 15909 fix start */
                                        tickType_t currentTick
                                        /* SPR 15909 fix end */
                                        /* +- SPR 17777 */
                                        ,InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
                                        /* SPR 13836 Changes End */
                                        /*TDD Config 0 Changes Start*/ 
                                        ,UInt8 ulSubFrameNum
                                        /*TDD Config 0 Changes End*/ 
#endif
                                        )
{


    ULUEContext                *ulUEContextFirst_p           = PNULL;
    PdcchULMUMIMOCandidateInfo *pdcchULMUMIMOCandidateInfo_p = PNULL;
    PdcchULNewUEInfo           *pdcchULNewUEInfo_p           = PNULL;
#ifdef FDD_CONFIG
    DirectIndexingUEInfo       *firstUEIndexingInfo_p        = PNULL;
#elif TDD_CONFIG
    UeScheduledInfo            *firstUEIndexingInfo_p        = PNULL;
#endif
    /*SPR 21769 Changes Start*/
    UInt16 ueCount                                            = 0;   
    /*SPR 21769 Changes End*/
    UInt32 userLocationType                                  = 0;
    UInt16 minRbLoss                                         = MAX_NUM_RBS;
    UInt16 firstUeIndex                                      = INVALID_UE_INDEX;
    UInt8 userModulationType                                 = 0;

    /*Loop to iterate for each userLocationType and Modulation Type(QPSK,16QAM or 64QAM) as ULMUMIMOCandidateInfo array is filled with 
     * based on userLocationType and userModulationType only*/
    for (userLocationType = 0; userLocationType < MAX_USER_LOCATION; userLocationType++)
    {
        for (userModulationType = 0; userModulationType < MAC_MAX_NUM_MOD_SCHEME; userModulationType++)
        {
            pdcchULMUMIMOCandidateInfo_p = &pdcchULOutputInfo_p->pdcchULMUMIMOCandidateInfo[userLocationType][userModulationType];
            pdcchULNewUEInfo_p = &pdcchULOutputInfo_p->pdcchULNewUEInfo[userLocationType];


            /*Loop to iterate through candidate List for buddy selection*/
            if(!pdcchULMUMIMOCandidateInfo_p->count)
            {
                continue;
            }
            for (ueCount = 0; ueCount < (pdcchULMUMIMOCandidateInfo_p->count - 1); ueCount ++)
            {
                minRbLoss = MAX_NUM_RBS;
                firstUeIndex = pdcchULMUMIMOCandidateInfo_p->pdcchULMUMIMOCandidateUE[ueCount].ueIndex;
                /* Accessing the global structure of ULUEContext for firstUeIndex*/
                ulUEContextFirst_p = ulUECtxInfoArr_g[firstUeIndex].ulUEContext_p;
                /*To access requiredRB and strictRBAllocation of first UE*/
#ifdef FDD_CONFIG 
                firstUEIndexingInfo_p = &pdcchULOutputInfo_p->directIndexingUEInfo[firstUeIndex];
#elif TDD_CONFIG
                /*TDD Config 0 Changes Start*/
                /*ulUeScheduledInfoPointerArray is now indexed on the basis of U
                 * frame*/
                /** SPR 14204 Fix : Start **/
                firstUEIndexingInfo_p = ulUEContextFirst_p->\
                                        ulUeScheduledInfoPointerArray[internalCellIndex][ulSubFrameNum];
                /** SPR 14204 Fix : End **/
                /*TDD Config 0 Changes End*/
#endif
                /*To check whether this UE has any buddy UE or not. If there is any, than continue with other UEs*/
                if (firstUEIndexingInfo_p->buddyUeIndex < MAX_UE_SUPPORTED)
                {
                    continue;
                }

                /*Cyclomatic Complexity changes - Starts here */
                /* +- SPR 17777 */
                calculateBuddyPairRBs(pdcchULMUMIMOCandidateInfo_p,
                        pdcchULNewUEInfo_p, 
                        ulUEContextFirst_p,
#ifdef FDD_CONFIG
                        pdcchULOutputInfo_p,
#endif
                        firstUEIndexingInfo_p,
                        &firstUeIndex,currentTick, ueCount, 
                        minRbLoss
                        /* +- SPR 17777 */
                        /*TDD Config 0 Changes Start*/
                        ,internalCellIndex
#ifdef TDD_CONFIG
                        /* SPR 13836 Changes End */
                        ,ulSubFrameNum
                        /*TDD Config 0 Changes End*/ 
#endif
                        );
                /*Cyclomatic Complexity changes - end here */
            }/*close of for loop(ueCount)*/
        }/*close of for loop (userModulationType)*/
    }/*close of for loop(userLocationType)*/
}
/*UL_MU_MIMO_CHG End*/

/* UL_MU_MIMO_CHG Start */
/*************************************************************************************
 * Function Name     :  checkOrthoN2dmrsPair
 * Inputs            :  riStart - Start RB index,
 *                      hiIndex
 *                      internalCellIndex - Cell-Index used at MAC
 * Outputs           :  availableN2dmrs_p - available orthogonal n2dmrs index pointer
 * Returns           :  MAC_SUCCESS/MAC_FAILURE
 * Description:         This function checks availablity of the orthogonal N2dmrs 
 *                      pair for given riStart & hiIndex and update 
 *                      availableN2dmrs_p as available index from orthoNdmrsMap_g[][].
 ****************************************************************************************/
            STATIC  MacRetType checkOrthoN2dmrsPair(
                    UInt32 riStart, 
                    UInt8 hiIndex, 
                    UInt8 *availableN2dmrs_p,
                    InternalCellIndex internalCellIndex
                    /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG                    
                    ,UInt8 iPhich
#endif                    
                    /*TDD Config 0 Changes End*/ 
                    )
{
    UInt8 ndmrsIndexCounter = 0;
    UInt8 tempNdmrs         = 0;
    UInt8 tempOrthoNdmrs    = 0;
    UInt8 nPhichGrpfirst    = 0;
    UInt8 nPhichSeqfirst    = 0;
    UInt8 nPhichGrpSecond   = 0;
    UInt8 nPhichSeqSecond   = 0;

    for (; ndmrsIndexCounter < MAX_POSSIBLE_NDMRS_PAIRS; ndmrsIndexCounter++)
    {
        tempNdmrs       = orthoNdmrsMap_g[ndmrsIndexCounter][0];
        /*TDD Config 0 Changes Start*/
        /* phichGroupFromPrbDmrsMap_g is indexed on the basis of iPhich as well
         * in TDD*/
#ifdef FDD_CONFIG
        nPhichGrpfirst  = phichGroupFromPrbDmrsMap_g[internalCellIndex][riStart][tempNdmrs];
#elif TDD_CONFIG
        nPhichGrpfirst  = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][riStart][tempNdmrs];
#endif       
        nPhichSeqfirst  = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][riStart][tempNdmrs];
        tempOrthoNdmrs  = orthoNdmrsMap_g[ndmrsIndexCounter][1];
#ifdef FDD_CONFIG        
        nPhichGrpSecond = phichGroupFromPrbDmrsMap_g[internalCellIndex][riStart][tempOrthoNdmrs];
#elif TDD_CONFIG
        nPhichGrpSecond = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][riStart][tempOrthoNdmrs];
#endif
        /*TDD Config 0 Changes End*/
        nPhichSeqSecond = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][riStart][tempOrthoNdmrs];

        /* Check if tempNdmrs & tempOrthoNdmrs both are available */
#ifdef FDD_CONFIG
        if((phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrpfirst].bitMask\
                    & (ONE << nPhichSeqfirst)) && 
                (phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrpSecond].bitMask\
                 & (ONE << nPhichSeqSecond)))
            /*CA Changes end  */
        {

            *(availableN2dmrs_p) = ndmrsIndexCounter;

            LOG_MAC_MSG(MAC_ULMIMO_ORTHO_N2DMRS_INFO_ID,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    riStart,\
                    hiIndex,\
                    tempNdmrs,\
                    nPhichGrpfirst,\
                    nPhichSeqfirst,\
                    0.0,\
                    0.0,\
                    __func__,"");

            return MAC_SUCCESS;
        }
#elif TDD_CONFIG
        if((phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrpfirst]\
                    & (ONE << nPhichSeqfirst)) &&
                (phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrpSecond]\
                 & (ONE << nPhichSeqSecond)))
        {

            *(availableN2dmrs_p) = ndmrsIndexCounter;

            LOG_MAC_MSG(MAC_ULMIMO_ORTHO_N2DMRS_INFO_ID,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    riStart,\
                    hiIndex,\
                    tempNdmrs,\
                    nPhichGrpfirst,\
                    nPhichSeqfirst,\
                    0.0,\
                    0.0,\
                    __func__,"");

            return MAC_SUCCESS;
        }
#endif
    }
    return MAC_FAILURE;
}
/*************************************************************************************
 * Function Name     :  checkAvailableULResources
 * Inputs            :  numOfResourcesReq - number of resources required,
 *                      availRes_p - available resources map,
 *                      rbMap - resource map,
 *                      hiIndex -
 *                      internalCellIndex - Cell-Index used at MAC
 * Outputs           :  availableN2dmrs_p - available orthogonal n2dmrs index pointer,
 *                      startRBIndex_p - start RB index correspond to N2dmrs pair
 * Returns           :  length - return available length from RB map
 * Description       :  This function checks availablity of the orthogonal N2dmrs pair 
 *                      & corresponding available resources.
 **************************************************************************************/
        STATIC  UInt8 checkAvailableULResources(
                UInt8 numOfResourcesReq,
                AvailableResInfo* availRes_p,
                UInt8* rbMap,
                /* +- SPR 17777 */
#ifdef FDD_CONFIG
                UInt8 hiIndex,
#endif
                /* +- SPR 17777 */
                UInt8* availableN2dmrs_p,
                UInt32* startRBIndex_p,
                InternalCellIndex internalCellIndex
                /*TDD Config 0 Changes Start*/                    
#ifdef TDD_CONFIG
                ,UInt8 iPhich
                ,UInt8 phichSubFrameNum
#endif
                /*TDD Config 0 Changes End*/
                )
{
    UInt32  newFlag         = TRUE;
    UInt32  availableFlag   = FALSE;
    UInt32  idx             = 0;
    UInt32  tempLength      = 0;
    UInt32  tempStartPoint  = 0;
    UInt32  startPoint      = 0;
    UInt8   length          = 0;

    UInt8 tempAvailableN2dmrsIndex = INVALID_N2DMRS_INDEX_VALUE;

    idx = availRes_p->freeRBIndex;

    while ( idx <= availRes_p->endIndex )
    {
        if ( FREE_RESOURCE == rbMap[idx])
        {
            if ( newFlag )
            {   
                newFlag        = FALSE;     
                tempLength     = 1;    
                tempStartPoint = idx;   
                if(MAC_SUCCESS != checkOrthoN2dmrsPair(idx, 
#ifdef FDD_CONFIG
                            hiIndex, 
#elif TDD_CONFIG
                            phichSubFrameNum,
#endif
                            availableN2dmrs_p,
                            internalCellIndex
                            /*TDD Config 0 Changes Start*/                             
#ifdef TDD_CONFIG
                            ,iPhich
#endif
                            /*TDD Config 0 Changes End*/                             
                            ))
                {
                    idx++;
                    newFlag = TRUE;     
                    continue;
                }
            }
            else   
            {          
                tempLength++;   
            }
            /* Check for Required Number of Resoures got */
            if ( tempLength == numOfResourcesReq )
            {     
                availableFlag = TRUE;
                break;
            }
            idx++;
        }
        else
        {
            /* Resources are not free at this index */
            idx += rbMap[idx];
            if( tempLength > length)
            {
                startPoint = tempStartPoint;
                length     = tempLength;
                tempAvailableN2dmrsIndex = *(availableN2dmrs_p);
            }
            newFlag = TRUE;
        }
    }
    if(availableFlag == TRUE)
    {
        *(startRBIndex_p) = tempStartPoint;
        length            = tempLength;
        tempAvailableN2dmrsIndex = *(availableN2dmrs_p);
    }
    else
    {
        if( tempLength > length)
        {
            startPoint = tempStartPoint;
            length     = tempLength;
            tempAvailableN2dmrsIndex = *(availableN2dmrs_p);
        }
        *(startRBIndex_p) = startPoint;
    }
    if(tempAvailableN2dmrsIndex != INVALID_N2DMRS_INDEX_VALUE)
    {
        *(availableN2dmrs_p) = tempAvailableN2dmrsIndex;
    }
    if (length)
    {
        length = getUplinkMinRBsInPowerOfTwoThreeFive(length);
    }
    return length;
}

/*********************************************************************************
 * Function Name :      updateResourcesFromBuddyUE
 * Inputs:              harqIndex - Harq process Id 
 *                      buddyHarqIndex(TDD Only ) -
 *                      ulUEContext_p - The pointer to the ULUEContext
 *                      allocatedMCS_p -
 *                      resourceReserved_p -
 *                      buddyUeIndex -
 *                      avgMCS -
 * Outputs:             MacRetType: MAC_SUCCESS.
 * Description:         This function updates the resource info from 
 *                      Buddy UE in ulUEContext_p
 *********************************************************************************/
                static MacRetType updateResourcesFromBuddyUE(
                        UInt8 harqIndex, 
#ifdef TDD_CONFIG
                        UInt8 buddyHarqIndex,
#endif
                        ULUEContext* ulUEContext_p,
                        UInt8 *allocatedMCS_p,
                        AllocatedResInfo* resourceReserved_p,
                        UInt16 buddyUeIndex,
                        UInt8 avgMCS
                        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                        ,UInt8 iPhich
#endif
                        /* TDD Config 0 Changes Start */
                        ,InternalCellIndex internalCellIndex
                        )
{
    ULHarqInfo*  tempULHarqInfo_p      = PNULL;
    ULHarqInfo*  buddyTempULHarqInfo_p = PNULL;
    ULUEContext* ulBuddyUEContext_p    = PNULL;

    /* No check required for valid buddyUeIndex as this func is not
     * getting called otherwise */
    ulBuddyUEContext_p    = ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p;
    tempULHarqInfo_p      = &(ulUEContext_p->ulresInfo[harqIndex]);
#ifdef FDD_CONFIG
    buddyTempULHarqInfo_p = &(ulBuddyUEContext_p->ulresInfo[harqIndex]);
#elif TDD_CONFIG
    buddyTempULHarqInfo_p = &(ulBuddyUEContext_p->ulresInfo[buddyHarqIndex]);
#endif

    tempULHarqInfo_p->nDmrs      = buddyTempULHarqInfo_p->buddyNdmrs;
    /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
    tempULHarqInfo_p->nPhichGrp  = phichGroupFromPrbDmrsMap_g[internalCellIndex][buddyTempULHarqInfo_p->riStart][buddyTempULHarqInfo_p->buddyNdmrs];
#elif TDD_CONFIG
    tempULHarqInfo_p->nPhichGrp  = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][buddyTempULHarqInfo_p->riStart][buddyTempULHarqInfo_p->buddyNdmrs];
#endif
    tempULHarqInfo_p->nPhichSeq  = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][buddyTempULHarqInfo_p->riStart][buddyTempULHarqInfo_p->buddyNdmrs];
    /* TDD Config 0 Changes End */
    tempULHarqInfo_p->sfUsed     = buddyTempULHarqInfo_p->sfUsed;
#ifdef SSI_DEBUG_STATS
    tempULHarqInfo_p->ulMuMimoStatus = UL_MU_MIMO_ENABLED;
    buddyTempULHarqInfo_p->ulMuMimoStatus = UL_MU_MIMO_ENABLED;
#endif //SSI_DEBUG_STATS 

    resourceReserved_p->allocatedIndex  = buddyTempULHarqInfo_p->riStart;
    resourceReserved_p->allocatedLength = buddyTempULHarqInfo_p->riLength;
    resourceReserved_p->allocatedRiv    = buddyTempULHarqInfo_p->riv;
    /*SPR_9204_START*/
    *(allocatedMCS_p)                   = avgMCS;
    /*SPR_9204_END*/

    return MAC_SUCCESS;
}

/******************************************************************************************
 * Function Name :  allocateNonFrequencySelectiveUplinkResources
 * Inputs        :  ulUEContext_p - The pointer to the ULUEContext,
 *                  pdcchULNewUE_p - The pointer to PdcchULNewUE,
 *                  ueIndexingInfo_p - Pointer to DirectIndexingUEInfo incase of FDD
 *                                     and Pointer to UeScheduledInfo incase of TDD,
 *                  resourceReserved - The pointer to the AllocatedResInfo,
 *                  subFrameNumber - The subFrame number when it is sent,
 *                  freeRBs - Number of free RBs that can be allocated,
 *                  bufferRBs_p,
 *                  tpc,
 *                  ueMaxPowerFlag_p,
 *                  ueMinPowerFlag_p,
 *                  allocatedMCS_p,
 *                  hiIndex,
 *                  currentGlobalTick,
 *                  ulDelay
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs       :  None
 * Returns       :  MAC_SUCCESS/MAC_FAILURE
 * Description   :  This function implements the Non-FSS resources allocation algorithm
 *
 *      This function allocate resources in 3 ways:
 *      1. If resources need to be shared among buddies and none of them is considered.
 *         - Try to allocate same RBs to the buddy UEs on their respective
 *         average MCSs.
 *         - Check from available resources whether any orthogonal N2dmrs pair 
 *           is available at start free RB Index & corresponding number of maximum 
 *           available resources.
 *         - If no pair available at the particular start index then move to the 
 *           next free index & again check for orthogonality of ndmrs pair.
 *         - If no orthogonal ndmrs pair is available at any free RB Index ,discard buddy 
 *           pair & fallback to single UE allocation.
 *         - If orthogonal ndmrs pair is available, but max avaialble resources are not 
 *           sufficient, discard buddy pair & fallback to single UE allocation.
 *         - If orthogonal ndmrs pair as well as RBs more than minRB requirement are
 *           available, check for minStrictRBs [= Max(MinRBs of both UE) RBs required 
 *           if strict allocation is required for one/both of the UEs].If strict 
 *           allocation is not required, assign available resources.
 *         - Reserve available ndmrs pair & update Phich info corresponding to both UEs.
 *      2. If resources need to be shared among buddies and resources are already 
 *         allocated to one of the buddy pair UE.
 *         - No actual allocation is required here; 
 *           only upadate alreday reserved resource info from its Buddy UE 
 *      3. If no need to share resources 
 *         - Do single UE resource allocation based on the RBs provided by strategy.
 *
 *      BuddyPairinfo shall be provided as input to this func based on 
 *      buddy selection algorithm
 *******************************************************************************************/
    STATIC  MacRetType allocateNonFrequencySelectiveUplinkResources(
            ULUEContext* ulUEContext_p,
            PdcchULNewUE *pdcchULNewUE_p,
#ifdef FDD_CONFIG
            DirectIndexingUEInfo*  ueIndexingInfo_p,
#elif TDD_CONFIG
            UeScheduledInfo *ueIndexingInfo_p,
#endif
            AllocatedResInfo* resourceReserved_p,
            UInt8 subFrameNumber,
            UInt32 freeRBs,
            SInt8 *bufferRBs_p, 
            TpcTriggeredToPowerCorrection tpc,
            UInt32 *ueMaxPowerFlag_p,
            UInt32 *ueMinPowerFlag_p,
            UInt8 *allocatedMCS_p,
            UInt8 hiIndex, 
            /* SPR 15909 fix start */
            tickType_t currentGlobalTick
            /* SPR 15909 fix end */
#ifdef FDD_CONFIG 
    ,UInt8 ulDelay
#endif
    /*CA Changes start */
    ,InternalCellIndex internalCellIndex
    /*CA Changes end */
    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    ,UInt8 ulSubFrameNum
    ,UInt8 phichSubFrameNum
#endif
    /*TDD Config 0 Changes End*/ 
    )
{
    UInt8        minRbReq         = 0;
    UInt8        maxReqRB         = 0;
    UInt8        addReqRB         = 0;
    UInt8        allocatedMCS     = pdcchULNewUE_p->avgMCS;
    UInt32       dataSize         = pdcchULNewUE_p->minDataSize;
    UInt32       maxDataSize      = pdcchULNewUE_p->maxDataSize;
    /* SPR 15909 fix start */
    tickType_t       currentTTI       = getCurrentTick();
    /* SPR 15909 fix end */
    UInt8        nDmrs            = MAX_DMRS_VALUE;
    ULHarqInfo*  tempULHarqInfo_p = PNULL;
    /* Rel 5.3: Coverity 32365 Fix Start */
    MacRetType   retValue         = MAC_FAILURE;
    /* Rel 5.3: Coverity 32365 Fix End */
    /*CA Changes start  */
    RbMapNode*   rbMapNode_p      = rbMapNodeArr_gp[internalCellIndex] + subFrameNumber;
    /*CA Changes end  */

    /* +DYNAMIC_ICIC */
    /*CA Changes start  */
    ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + ulPowerMaskIndex_g[internalCellIndex];
    /*CA Changes end  */
    SInt32 regionPowerMask = ulPowerMaskInfo_p->powerMask[ulUEContext_p->userLocationType];\
                             /* -DYNAMIC_ICIC */


                             UInt8        minRBsforAperiodicCQIReports = 0;
    UInt8        availableN2dmrsIndex         = INVALID_N2DMRS_INDEX_VALUE;
    UInt16       buddyUeIndex                 = ueIndexingInfo_p->buddyUeIndex; 
    ULUEContext* ulBuddyUEContext_p           = PNULL;
    /*SPR_9204_START*/
    /*SPR 8451 Changes Start*/
    UInt8 firstUEAvgMCS                     = pdcchULNewUE_p->avgMCS;
    /*SPR 8451 Changes Ends*/
    UInt8 secondUEAvgMCS                    = 0;
    UInt8 firstUEMaxReqRB                   = 0;
    UInt8 secondUEMaxReqRB                  = 0;
    UInt32 firstUECategoryBytesSupport      = 0;
    UInt32 secondUECategoryBytesSupport     = 0;
    UInt8 firstUEMaxSupportedRB             = 0;
    UInt8 secondUEMaxSupportedRB            = 0;
    UInt8 maxSupportedRB                    = 0;
    /*SPR_9204_END*/

    /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
    UInt16 sysFrameNum = (currentGlobalTick / MAX_SUB_FRAME) % MAX_SFN_VALUE;
    ULHarqInfo* ulHarqInfoForPreviousUlSubframe_p = PNULL;
    UInt8 iPhich                                = 0;
    iPhich = calculateIPhichValue(ulSubFrameNum,internalCellIndex);
#endif
    /*TDD Config 0 Changes End*/
    /* +DYNAMIC_ICIC */     
    /*Cyclomatic Complexity changes - Starts here */
    checkQciPowerMaskAndUpdateRegionMask(ulUEContext_p,&regionPowerMask);
    /*Cyclomatic Complexity changes - end here */
    /* -DYNAMIC_ICIC */


    if(( buddyUeIndex < MAX_UE_SUPPORTED) && (PNULL != ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p))
    {
        ulBuddyUEContext_p = ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p;
    }
    /* Find minnimum RBReq of the UE on the reported AvgMCS;
     * updated tempDirectIndexingUEInfo_p->requiredRB will be used later in
     * case if even minStrictRBs are not available for UE.
     * In such case, even no need to fallback to Single Allocation as minimum RBs
     * are not available*/
    if (CQI_WITH_DATA_ULGRANT_REQ_MSG == 
            ueIndexingInfo_p->ulGrantRequestType)
    {    
        minRBsforAperiodicCQIReports = 
            /* SPR 5999 changes start */
            DETERMINE_RBs_UL(allocatedMCS,dataSize,internalCellIndex);
        /* SPR 5999 changes end */
        if (ueIndexingInfo_p->requiredRB < 
                minRBsforAperiodicCQIReports)
        {
            ueIndexingInfo_p->requiredRB = 
                minRBsforAperiodicCQIReports;
            ueIndexingInfo_p->strictRBAllocation = TRUE;
        }    
    }
    /* UL_MU_MIMO_CHG Start */
    /* ***************************************************************
     * Resource alloaction required for paired UE ;
     * Here allocation would be only for one of the UE from buddy pair
     * with consideration of both UEs RB requirement 
     * *************************************************************** */
    if ( (buddyUeIndex < MAX_UE_SUPPORTED) && 
            (PNULL != ulBuddyUEContext_p) && 
            (!ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated) )
    {
        /*CA Changes start  */
        BuddyPairInfo* buddyPairInfo_p          = buddyPairInfo_gp[internalCellIndex] + ulUEContext_p->ueIndex;
        /*CA Changes end  */
        UInt8          retAvailableRBLength     = 0;
        UInt8          requiredRBs              = 0;
        UInt32         availableStartRBIndex    = 0;
        UInt8          nPhichGrp                = 0;
        UInt8          nPhichSeq                = 0;
        UInt8          orthoN2Dmrs              = MAX_DMRS_VALUE;        
        UInt8          orthoNPhichGrp           = 0;
        UInt8          orthoNPhichSeq           = 0;
        /*SPR 16417 Changes Start*/
        TpcTriggeredToPowerCorrection buddyTpc = getTpcForPusch(
                ulBuddyUEContext_p 
#ifdef FDD_CONFIG
                ,currentGlobalTick + ulDelay
                ,internalCellIndex
#elif TDD_CONFIG
                ,currentGlobalTick
                ,internalCellIndex
                /*TDD Config 0 Changes Start*/ 
                ,ulSubFrameNum
                /*TDD Config 0 Changes End*/ 
#endif
                );
        /*SPR 16417 Changes End*/
#ifdef FDD_CONFIG
        UInt8 containerTick = (currentTTI + ulDelay)% MAX_PDCCH_CONTAINER;
        PdcchULOutputInfo* buddyTempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;

        DirectIndexingUEInfo*  buddyUEIndexingInfo_p = \
                                                       &(buddyTempPdcchULOutput_p->directIndexingUEInfo[buddyUeIndex]);
#elif TDD_CONFIG
        /*TDD Config 0 Changes Start*/ 
        /** SPR 14204 Fix : Start **/
        UeScheduledInfo *buddyUEIndexingInfo_p = ulBuddyUEContext_p->\
                                                 ulUeScheduledInfoPointerArray[internalCellIndex][ulSubFrameNum];
        /** SPR 14204 Fix : End **/
        /*TDD Config 0 Changes End*/ 
#endif

        /*SPR_9204_START*/
        /* Calculate the Min RBs required for buddy UEs*/
        minRbReq = MAX_BETWEEN_TWO(ueIndexingInfo_p->requiredRB,buddyUEIndexingInfo_p->requiredRB);
        minRbReq = getUplinkMaxRBsInPowerOfTwoThreeFive(minRbReq);
        requiredRBs = minRbReq;

        *bufferRBs_p += ueIndexingInfo_p->requiredRB + buddyUEIndexingInfo_p->requiredRB - minRbReq;    
        /* Try to serve Max RBs required for buddy UEs from bufferpool */
        if(*bufferRBs_p > 0)
        {
            secondUEAvgMCS = buddyPairInfo_p->buddyAvgMCS;
            firstUEMaxReqRB = DETERMINE_RBs_UL(firstUEAvgMCS,pdcchULNewUE_p->maxDataSize,internalCellIndex);
            secondUEMaxReqRB = DETERMINE_RBs_UL(secondUEAvgMCS, buddyPairInfo_p->buddyMaxDataSize,internalCellIndex);
            maxReqRB = MAX_BETWEEN_TWO(firstUEMaxReqRB,secondUEMaxReqRB);

            /* Find maxbytes supported by each UE as per its category */
            /* SPR 19600 Changes start */
            firstUECategoryBytesSupport = ulUeCategoryInfo_g[ulUEContext_p->ueCategory - 1].maxBytesULSCHTb;
            secondUECategoryBytesSupport = ulUeCategoryInfo_g[ulBuddyUEContext_p->ueCategory - 1].maxBytesULSCHTb;
            /* SPR 19600 Changes end */
            firstUEMaxSupportedRB = DETERMINE_RBs_UL(firstUEAvgMCS,firstUECategoryBytesSupport,internalCellIndex);
            secondUEMaxSupportedRB = DETERMINE_RBs_UL(secondUEAvgMCS,secondUECategoryBytesSupport,internalCellIndex);
            maxSupportedRB = MIN_BETWEEN_TWO(firstUEMaxSupportedRB,
                    secondUEMaxSupportedRB);
            if (maxReqRB > maxSupportedRB)
            {
                maxReqRB = maxSupportedRB;
            }

            if(minRbReq < maxReqRB)
            {
                addReqRB = maxReqRB - minRbReq;
                /* UE is given its max required RBs and still some RBs are
                 * left in Bufferpool, As additional rbs are given from bufferpool,
                 * it gets deducted from bufferpool*/
                if(*bufferRBs_p > addReqRB)
                {
                    requiredRBs  += addReqRB;

                }
                /* UE is given all the available RBs from bufferpool and
                 * it is made to zero as no more rbs exist in it*/
                else
                {
                    requiredRBs  += *bufferRBs_p;
                }

                /* +DYNAMIC_ICIC */
                /*CA Changes start  */
                ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + ulPowerMaskIndex_g[internalCellIndex];
                /*CA Changes end  */
                SInt32 buddyUeRegionPowerMask = ulPowerMaskInfo_p->powerMask[ulBuddyUEContext_p->userLocationType];
                /* -DYNAMIC_ICIC */

                /* ATB Changes 5.1 Start */
                UInt32 buddyUeMinPowerFlag = 0;
                /* ATB Changes 5.1 End */
                UInt32 buddyUeMaxPowerFlag = 0;
                /* Time Averaging changes end */

                UInt8 firstUeATBMcs, buddyUEATBMcs;
                /* +DYNAMIC_ICIC */     
                /*Cyclomatic Complexity changes - Starts here */
                checkQciPowerMaskAndUpdateRegionMask(ulBuddyUEContext_p, &buddyUeRegionPowerMask);
                /*Cyclomatic Complexity changes - end here */
                /* -DYNAMIC_ICIC */

                /* Power check for maxdatasize of both UEs */
                firstUeATBMcs = firstUEAvgMCS; 
                buddyUEATBMcs = secondUEAvgMCS;

                requiredRBs = MIN_BETWEEN_TWO (
                        validateATB(requiredRBs, &firstUeATBMcs, ulUEContext_p, 
                            tpc.powerCorrectionVal, ueMaxPowerFlag_p, ueMinPowerFlag_p, 
                            regionPowerMask, NEW_TX,internalCellIndex),

                        validateATB(requiredRBs, &buddyUEATBMcs, ulBuddyUEContext_p, 
                            buddyTpc.powerCorrectionVal, &buddyUeMaxPowerFlag, &buddyUeMinPowerFlag, 
                            buddyUeRegionPowerMask, NEW_TX,internalCellIndex)
                        );
                if (requiredRBs <  minRbReq)
                {
                    requiredRBs = minRbReq;
                }
                else
                {
                    firstUEAvgMCS = firstUeATBMcs;
                    buddyPairInfo_p->buddyAvgMCS = buddyUEATBMcs;
                    buddyPairInfo_p->buddyMaxPowerFlag = buddyUeMaxPowerFlag;
                    buddyPairInfo_p->buddyMinPowerFlag = buddyUeMinPowerFlag;
                }
                /* validateNprbFromATB returns rbs in min power of 2x3y5z */
            } 
        }
        /* Check Availablity of resources for both UEs */
        /* checkAvailableULResources returns rbs in min power of 2x3y5z */
        retAvailableRBLength = checkAvailableULResources(
                requiredRBs,
                &rbMapNode_p->availableResourceInfo\
                [ulUEContext_p->userLocationType],
                rbMapNode_p->rbMap,
                /* +- SPR 17777 */
#ifdef FDD_CONFIG
                hiIndex,
#endif
                /* +- SPR 17777 */
                &availableN2dmrsIndex,
                &availableStartRBIndex,
                internalCellIndex
                /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
                ,iPhich
                ,phichSubFrameNum
#endif
                /*TDD Config 0 Changes End*/ 
                );

        /* When Required RBs are available at valid ndmrs */       
        if ( availableN2dmrsIndex < INVALID_N2DMRS_INDEX_VALUE )
        {
            /* If either strict allocation is not required or 
             * resources are available for strict allocation */
            if ( retAvailableRBLength >= buddyPairInfo_p->minStrictRBReq )
            {
                /*SPR 8451 Changes Start*/
#ifdef ULCODERATE_PLUGIN
                /*In case of Buddy UE pair, only MCS needs to be reduced,
                 * Number of RB should not be reduced*/
                checkULCodeRateUECategory(ulUEContext_p,&firstUEAvgMCS,
                        &(ueIndexingInfo_p->requiredRB), BUDDY_UE_SET,
                        internalCellIndex);
#endif
                /*SPR 8451 Changes End*/

                /* Reserve required or maximum available resources 
                 * i.e. retAvailableRBLength */
                /*Coverity Id 32413 Fix Start*/
                MAC_UL_RESERVE_RESOURCES(
                        availableStartRBIndex,\
                        retAvailableRBLength,\
                        resourceReserved_p,\
                        bufferRBs_p,\
                        minRbReq,\
                        ulUEContext_p->userLocationType,\
                        rbMapNode_p,\
                        rbMapNode_p->rbMap,
                        internalCellIndex);

                *allocatedMCS_p =firstUEAvgMCS;
                /*Coverity Id 32413 Fix End*/

                LOG_MAC_MSG(MAC_BUFF_POOL_RES_INFO,LOGDEBUG,MAC_PUSCH,\
                        getCurrentTick(),\
                        ulUEContext_p->ueIndex,\
                        ulUEContext_p->userLocationType,\
                        *bufferRBs_p,\
                        maxReqRB,\
                        addReqRB,\
                        0.0,\
                        0.0,\
                        __func__,"ADVANCE_RES_ALLOC");

                /* Reserve orthogonal ndmrs pair from availableN2dmrsIndex */
                nDmrs     = orthoNdmrsMap_g[availableN2dmrsIndex][0];
                /*TDD Config 0 Changes Start*/ 
#ifdef FDD_CONFIG
                nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][availableStartRBIndex][nDmrs];
#elif TDD_CONFIG
                nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][availableStartRBIndex][nDmrs];
#endif
                nPhichSeq = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][availableStartRBIndex][nDmrs];
                orthoN2Dmrs    = orthoNdmrsMap_g\
                                 [availableN2dmrsIndex][1];
#ifdef FDD_CONFIG                
                orthoNPhichGrp = phichGroupFromPrbDmrsMap_g\
                                 [internalCellIndex][availableStartRBIndex][orthoN2Dmrs];
#elif TDD_CONFIG
                orthoNPhichGrp = phichGroupFromPrbDmrsMap_g\
                                 [internalCellIndex][iPhich][availableStartRBIndex][orthoN2Dmrs];
#endif
                /*TDD Config 0 Changes End*/ 
                orthoNPhichSeq = phichSequenceNoFromPrbDmrsMap_g\
                                 [internalCellIndex][availableStartRBIndex][orthoN2Dmrs];
#ifdef FDD_CONFIG
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp].bitMask &=
                    ~(ONE << nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][orthoNPhichGrp].bitMask &=
                    ~(ONE << orthoNPhichSeq);
#elif TDD_CONFIG
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp] &=
                    ~(ONE << nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][orthoNPhichGrp] &=
                    ~(ONE << orthoNPhichSeq);
#endif


                /* Update UE's Harq Info */
#ifdef FDD_CONFIG           
                tempULHarqInfo_p = &(ulUEContext_p->ulresInfo\
                        [ueIndexingInfo_p->harqIndex]);
#elif TDD_CONFIG
                tempULHarqInfo_p = &(ulUEContext_p->ulresInfo\
                        [ueIndexingInfo_p->ulHarqProcessId]);
#endif
                tempULHarqInfo_p->nDmrs      = nDmrs;
                tempULHarqInfo_p->buddyNdmrs = orthoN2Dmrs;
                tempULHarqInfo_p->nPhichGrp  = nPhichGrp;
                tempULHarqInfo_p->nPhichSeq  = nPhichSeq;
#ifdef FDD_CONFIG
                tempULHarqInfo_p->sfUsed     = hiIndex;
#elif TDD_CONFIG
                tempULHarqInfo_p->sfUsed     = phichSubFrameNum;
#endif

                /* Updated UE Context that PDSCH is allocated to UE */
                ulUEContext_p->ulMUMIMOInfo.isPuschAllocated = TRUE;

                /*updating buffer Rb pool with Rb's saved due to sharing by buddy pair*/

                /* 
                 * Since MAC_UL_RESERVE_RESOURCES has already taken care of updatng bufferRBs 
                 * when first buddy's resources are reserved in excess
                 * Therefore we only need to update the RBs saved due to second UE not being allocated
                 * seperate PUSCH region.
                 */


#ifdef SSI_DEBUG_STATS
                    /* CA Stats Changes Start */
                    LTE_MAC_UPDATE_TOTAL_RB_SHARED(retAvailableRBLength, internalCellIndex);
#endif

                LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                        ulUEContext_p->ueIndex,
                        currentTTI,
                        NEW_TX,
#ifdef FDD_CONFIG
                        ueIndexingInfo_p->harqIndex,
#elif TDD_CONFIG
                        ueIndexingInfo_p->ulHarqProcessId,
#endif
                        retAvailableRBLength,
                        availableStartRBIndex,
                        ueIndexingInfo_p->ulGrantRequestType,
                        determineUlTBSize(*allocatedMCS_p,retAvailableRBLength),
                        FALSE,
                        *allocatedMCS_p,
                        0,
                        tpc.tpcCommand 
                        ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                        buddyUeIndex,
                        ulUEContext_p->userLocationType,
                        nDmrs,
                        ulUEContext_p->modulationScheme,
                        internalCellIndex
                            );
                /* CA Stats Changes End */

                LOG_MAC_MSG(MAC_ULSCH_NON_FSS_INFO_ID,LOGDEBUG,MAC_PUSCH,\
                        getCurrentTick(),\
                        buddyPairInfo_p->minStrictRBReq,\
                        maxReqRB,\
                        *bufferRBs_p,\
                        retAvailableRBLength,\
                        *allocatedMCS_p,\
                        0.0,\
                        0.0,\
                        __func__,"");

                return MAC_SUCCESS; 
            }
            else if( (ueIndexingInfo_p->strictRBAllocation) &&
                    (retAvailableRBLength < ueIndexingInfo_p->requiredRB))
            {
                /* Revert back the buffer RBs */
                *bufferRBs_p += minRbReq -ueIndexingInfo_p->requiredRB - buddyUEIndexingInfo_p->requiredRB;

                /* Required resources are not available;
                 * Single Allocation is also not possible; No need to fallabck */
                ueIndexingInfo_p->buddyUeIndex = INVALID_UE_INDEX;
                buddyUEIndexingInfo_p->buddyUeIndex = INVALID_UE_INDEX;
                return MAC_FAILURE;
            }
            else 
            {
                /* Revert back the buffer RBs */
                *bufferRBs_p += minRbReq - ueIndexingInfo_p->requiredRB -  buddyUEIndexingInfo_p->requiredRB;
                /* Required resources are not available;
                 * Allocate resources as single UE as per RBreq given from strategy */
                ueIndexingInfo_p->buddyUeIndex = INVALID_UE_INDEX;
                buddyUEIndexingInfo_p->buddyUeIndex = INVALID_UE_INDEX;
#ifdef SSI_DEBUG_STATS
                /* CA Stats Changes Start */
                LTE_MAC_UPDATE_FAILURE_CAUSE(STRICT_ALLOC_FAILURE, 
                        internalCellIndex);
                /* CA Stats Changes End */
#endif

                LOG_MAC_MSG(MAC_ULSCH_NON_FSS_FAILURE_INFO_ID,LOGDEBUG,MAC_PUSCH,\
                        getCurrentTick(),\
                        ulUEContext_p->ueIndex,\
                        ulUEContext_p->userLocationType,\
                        buddyPairInfo_p->minStrictRBReq,\
                        retAvailableRBLength,\
                        firstUEAvgMCS,\
                        0.0,\
                        0.0,\
                        __func__,"RBs not available");
            }
        }
        else
        {   
            /* Revert back the buffer RBs */
            *bufferRBs_p += minRbReq - ueIndexingInfo_p->requiredRB - buddyUEIndexingInfo_p->requiredRB;
            /* Orthogonal Ndmrs not available ; 
             * Allocate resources as single UE as per RBreq given from strategy */
            ueIndexingInfo_p->buddyUeIndex = INVALID_UE_INDEX;
            buddyUEIndexingInfo_p->buddyUeIndex = INVALID_UE_INDEX;

#ifdef SSI_DEBUG_STATS
            /* CA Stats Changes Start */
            LTE_MAC_UPDATE_FAILURE_CAUSE(NDMRS_PAIR_FAILURE, internalCellIndex);
            /* CA Stats Changes End */
#endif

            LOG_MAC_MSG(MAC_ULSCH_NON_FSS_FAILURE_INFO_ID,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    ulUEContext_p->ueIndex,\
                    ulUEContext_p->userLocationType,\
                    buddyPairInfo_p->minStrictRBReq,\
                    retAvailableRBLength,\
                    firstUEAvgMCS,\
                    0.0,\
                    0.0,\
                    __func__,"OrthoN2dmrs not available");
        }
    }

    /* ****************************************************
     * Resources are already allocated to UE's Buddy;
     * No allocation required  here;
     * Only update resourceReserved_p from its Buddy UE 
     * **************************************************** */
    else if ( (buddyUeIndex < MAX_UE_SUPPORTED) && 
            (PNULL != ulBuddyUEContext_p) && 
            (ulBuddyUEContext_p->ulMUMIMOInfo.isPuschAllocated) )
    {
#ifdef TDD_CONFIG
        /*TDD Config 0 Changes Start*/ 
        /** SPR 14204 Fix : Start **/
        UeScheduledInfo *buddyUEIndexingInfo_p = ulBuddyUEContext_p->\
                                                 ulUeScheduledInfoPointerArray[internalCellIndex][ulSubFrameNum];
        /** SPR 14204 Fix : End **/
        /*TDD Config 0 Changes End*/ 
#endif
        BuddyPairInfo* buddyPairInfo_p = buddyPairInfo_gp[internalCellIndex] + buddyUeIndex;

        updateResourcesFromBuddyUE(
#ifdef FDD_CONFIG
                ueIndexingInfo_p->harqIndex,
#elif TDD_CONFIG
                ueIndexingInfo_p->ulHarqProcessId,
                buddyUEIndexingInfo_p->ulHarqProcessId,
#endif
                ulUEContext_p,
                allocatedMCS_p,
                resourceReserved_p,
                buddyUeIndex,
                buddyPairInfo_p->buddyAvgMCS
                /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                ,iPhich
#endif                
                ,internalCellIndex
                );
        /*TDD Config 0 Changes End*/
        /*SPR_9204_END*/

        /* Updated UE Context that PDSCH is allocated to UE */
        ulUEContext_p->ulMUMIMOInfo.isPuschAllocated = TRUE;


        *ueMaxPowerFlag_p = buddyPairInfo_p->buddyMaxPowerFlag;
        *ueMinPowerFlag_p = buddyPairInfo_p->buddyMinPowerFlag;

#ifdef SSI_DEBUG_STATS        
        /* CA Stats Changes Start */
        LTE_MAC_UPDATE_TOTAL_BUDDY_PAIR_SUCCESS(internalCellIndex);
#endif

        LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                ulUEContext_p->ueIndex,
                getCurrentTick(),
                NEW_TX,
#ifdef FDD_CONFIG
                ueIndexingInfo_p->harqIndex,
#elif TDD_CONFIG
                ueIndexingInfo_p->ulHarqProcessId,
#endif
                resourceReserved_p->allocatedLength,
                resourceReserved_p->allocatedIndex,
                ueIndexingInfo_p->ulGrantRequestType,
                determineUlTBSize(*allocatedMCS_p,\
                    resourceReserved_p->allocatedLength),
                FALSE,
                *allocatedMCS_p,
                0,
                tpc.tpcCommand 
                ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                buddyUeIndex,
                ulUEContext_p->userLocationType,
#ifdef FDD_CONFIG
                ulUEContext_p->ulresInfo[ueIndexingInfo_p->harqIndex].nDmrs,
#elif TDD_CONFIG
                ulUEContext_p->ulresInfo[ueIndexingInfo_p->ulHarqProcessId].nDmrs,
#endif
                ulUEContext_p->modulationScheme,
                internalCellIndex
                    );
        /* CA Stats Changes End */

        return MAC_SUCCESS; 
    }
    /* UL_MU_MIMO_CHG End */

    /* ********************************************************
     * Single UE Resorces allocation is required;
     * Either fallback from Non_FSS UL-MIMO or otherwise
     * ******************************************************** */

            /* Give extra rbs to UE when rbs are available in bufferpool.
             * If UE has more requirement of RBs,then it is giving from
             * bufferpool giving by strategy */
            /* SPR 5999 changes start */
            /*CA Changes start  */
            /*SPR 17120 fix start*/
            if(CQI_WITHOUT_DATA_ULGRANT_REQ_MSG != ueIndexingInfo_p->ulGrantRequestType)
            {
                maxReqRB = DETERMINE_RBs_UL(allocatedMCS, maxDataSize, internalCellIndex);     
            }
        /*SPR 17120 fix end*/
        /*CA Changes end  */
        UInt8 inputRBs = ueIndexingInfo_p->requiredRB;

        /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG

        if (COMMON_DCI_COUNT == pdcchULNewUE_p->scheduleCount)
        {
            /*Obtain the +k U frame so that we can access the +k Harq Info 
             * and use it to allocate resources for +7 U frame*/
            ulHarqInfoForPreviousUlSubframe_p = getPrevHarqInfoForTddConfig0(hiIndex,
                    sysFrameNum,
                    ulUEContext_p,
                    internalCellIndex
                    ); 
            /*Change the schedule count to 1, if PUSCH resource allocation fails for
             * +K */
            if (!ulHarqInfoForPreviousUlSubframe_p ||
                    !ulHarqInfoForPreviousUlSubframe_p->riLength)
            {
                pdcchULNewUE_p->scheduleCount = COMMON_DCI_PUSCH_FAILURE;
            }
        }

        /*This condition will be hit while processing +7 U frame and when a
         * common DCI0 will go for both +k and +7 U frames*/
        if (COMMON_DCI_COUNT == pdcchULNewUE_p->scheduleCount)
        {
            /*In case of TDD Config 0 when the same dci 0 goes for +k and +7 then 
             * while running the PUSCH EL for +7 U frame, we just need to check whether
             * the same set of resources are available at +7 as that in +k.
             * The mcs at +7 should be same as +k.
             * The NDmrs shld also be same.
             * Once, the same resources are available at +7 we need tocheck for buffer 
             * RBs too incase the required resources exceed the
             * resources allocated by strategy for +7 U frame.
             * If any of the above conditions fail,then MAC_FAILURE is returned and the
             * UE is dropped for UL TX.*/
            retValue = allocateResourcesForSecondPuschExecutionTddConfig0(
                    ulHarqInfoForPreviousUlSubframe_p,
                    pdcchULNewUE_p,
                    ueIndexingInfo_p,
                    *bufferRBs_p,
                    &allocatedMCS,
                    ulUEContext_p,
                    rbMapNode_p,
                    resourceReserved_p,
                    freeRBs,
                    currentTTI,
                    iPhich,
                    phichSubFrameNum,     
                    internalCellIndex
                    );
            nDmrs = ulHarqInfoForPreviousUlSubframe_p->nDmrs;
        }
        else
#endif
        {
            /* SPR 5999 changes end */
            if(*bufferRBs_p > 0
#ifdef FDD_CONFIG
                    && !(ulUEContext_p->ttiBundlingEnabled)
#endif
                    /* SPR 8206 Fix Start */
                    && !(ueIndexingInfo_p->strictRBAllocation))
                /* SPR 8206 Fix End */
            {
                if((ueIndexingInfo_p->requiredRB) < maxReqRB)
                {
                    addReqRB = maxReqRB - ueIndexingInfo_p->requiredRB;
                    /* UE is given its max required RBs and still some RBs are 
                     * left in Bufferpool, As additionalrbs are given from bufferpool,
                     * it gets deducted from bufferpool */
                    if(*bufferRBs_p > addReqRB)
                    {
                        ueIndexingInfo_p->requiredRB += addReqRB;
                    }
                    /* UE is given all the available RBs from bufferpool and 
                     * it is made to zero as no more rbs exist in it */
                    else
                    {
                        ueIndexingInfo_p->requiredRB += *bufferRBs_p;
                    }
                }
            }
            LOG_MAC_MSG(MAC_BUFF_POOL_RES_INFO,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    ulUEContext_p->ueIndex,\
                    ulUEContext_p->userLocationType,\
                    *bufferRBs_p,\
                    maxReqRB,\
                    addReqRB,\
                    (UDouble32)ueIndexingInfo_p->requiredRB,\
                    0.0,\
                    __func__,"ADVANCE_RES_ALLOC");


            /* Cyclomertic Complexity Starts Here*/
            retValue = validateMinRbReq(
                    &minRbReq, 
                    ueIndexingInfo_p, 
                    &allocatedMCS, 
                    ulUEContext_p, 
                    tpc, 
                    ueMaxPowerFlag_p, 
                    ueMinPowerFlag_p, 
                    rbMapNode_p, 
                    resourceReserved_p, 
                    regionPowerMask, 
                    freeRBs,
                    currentTTI
#ifdef FDD_CONFIG
                    ,ulDelay,
                    currentGlobalTick
#endif
                    ,internalCellIndex
                    /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
                    ,phichSubFrameNum
                    ,iPhich
                    ,&nDmrs
#endif
                    );
        }
        /*TDD Config 0 Changes End*/ 
        /* Cyclomertic Complexity end Here*/
        if(MAC_SUCCESS == retValue)
        {
            *bufferRBs_p -= (ueIndexingInfo_p->requiredRB - inputRBs);
            /* SPR 5295 changes start*/
            if(CE_USER == ulUEContext_p->userLocationType)
            {
                updateCCAvailRes(
                        &rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                        resourceReserved_p);
            }
            /* SPR 5295 changes end*/
            /*TDD Config 0 Changes Start*/
            /*Block the phich resources and update harq info*/
#ifdef TDD_CONFIG
            tempULHarqInfo_p = &(ulUEContext_p->ulresInfo\
                    [ueIndexingInfo_p->ulHarqProcessId]);

            reserveHiAndUpdateHarqInfo(iPhich,
                    resourceReserved_p->allocatedIndex,
                    nDmrs,
                    phichSubFrameNum,
                    tempULHarqInfo_p,
                    internalCellIndex);
#endif
            /*TDD Config 0 Changes End*/

            /* CA Stats Changes Start */
            LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                    ulUEContext_p->ueIndex,
                    currentTTI,
                    NEW_TX,
#ifdef FDD_CONFIG
                    ueIndexingInfo_p->harqIndex,
#elif TDD_CONFIG
                    ueIndexingInfo_p->ulHarqProcessId,
#endif
                    resourceReserved_p->allocatedLength,
                    resourceReserved_p->allocatedIndex,
                    ueIndexingInfo_p->ulGrantRequestType,
                    determineUlTBSize(allocatedMCS,resourceReserved_p->allocatedLength),
                    FALSE,
                    allocatedMCS,
                    0,
                    tpc.tpcCommand 
                    ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                    INVALID_UE_INDEX,
                    ulUEContext_p->userLocationType,
                    MAX_DMRS_VALUE,
                    ulUEContext_p->modulationScheme,
                    internalCellIndex
                        );
            /* CA Stats Changes End */
            *allocatedMCS_p = allocatedMCS;
        }
        return retValue;
}

/************************************************************************************
 * Function Name  : checkAvailableRBsToReturnLongestSetULMUMIMO
 * Inputs         : mcsToPrbInfo_p - UE's McsToPrbMap,
 *                  buddyMcsToPrbInfo_p - buddy UE's McsToPrbMap,
 *                  maxReqRBs - maximum required RBs,
 *                  rbMapNode_p - available RB map,
 *                  rbMap - Pointer to RB map,
 *                  hiIndex,
 *                  minRBReq
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : availableRBs - (max contiguous),
 *                  maxAvailableRBsStartIdx - startRBIndex of available resources
 *                  availableN2dmrs_p - available orthogonal N2dmrs pair Index pointer
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function Checks the longest possible set of size less 
 *                  than or equal to maxReqRBs in the mcsToPrbInfo_p & buddyMcsToPrbInfo_p
 *                  that can be allocated and returns the RBs in the form 2x3y5z.
 ***************************************************************************************/
            static MacRetType checkAvailableRBsToReturnLongestSetULMUMIMO(
                    MCSToPrbMapInfo *mcsToPrbInfo_p,
                    MCSToPrbMapInfo *buddyMcsToPrbInfo_p,
                    UInt8 maxReqRBs,
                    UInt8 *availableRBs,
                    AvailableResInfo *rbMapNode_p,
                    UInt8 *rbMap,
                    UInt8 *maxAvailableRBsStartIdx,
                    /* +- SPR 17777 */
#ifdef FDD_CONFIG
                    UInt8 hiIndex,
#endif
                    /* +- SPR 17777 */
                    UInt8* availableN2dmrs_p,
                    UInt8 minRBReq,
                    /*CA Changes start */
                    InternalCellIndex internalCellIndex
                    /*CA Changes end */
                    /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
                    ,UInt8 iPhich
            ,UInt8 phichSubFrameNum
#endif
            /*TDD Config 0 Changes End*/
            )
{
    UInt8 startIdx         = 0;
    UInt8 idx              = 0;
    UInt8 minPRBIndex      = 0;
    UInt8 mcsToPrbEndIndex = 0;
    UInt8 tempMax          = 0;
    UInt8 tempCount        = 0;

    UInt8 tempAvailableN2dmrsIndex     = INVALID_N2DMRS_INDEX_VALUE;

    mcsToPrbEndIndex = mcsToPrbInfo_p->endIdx;

    /* Start Index should start from the available free RB Index */
    if( rbMapNode_p->freeRBIndex > mcsToPrbInfo_p->startIdx )
    {
        startIdx = rbMapNode_p->freeRBIndex;
    }
    else
    {
        startIdx = mcsToPrbInfo_p->startIdx;
    }
    /* Start Index should be determined taking intersection 
     * of MSCTo PRBMap for both the UEs*/
    if( buddyMcsToPrbInfo_p->startIdx > startIdx )
    {
        startIdx = buddyMcsToPrbInfo_p->startIdx;
    }
    /* Determining the  mcsToPrbInfo_p->endIdx on intersecting 
     * the rbMap and MCSToPRBMaps for buddy UEs */
    if( mcsToPrbEndIndex > rbMapNode_p->endIndex )
    {
        mcsToPrbEndIndex = rbMapNode_p->endIndex;
    }
    if( mcsToPrbEndIndex > buddyMcsToPrbInfo_p->endIdx )
    {
        mcsToPrbEndIndex = buddyMcsToPrbInfo_p->endIdx;
    }

    idx = startIdx;

    while(idx <= mcsToPrbEndIndex)
    {
        /* Skip the portion of RB Map which is marked  BUSY_RESOURCE  */
        if (rbMap[idx])
        {
            idx += rbMap[idx];
            continue;
        }
        minPRBIndex = MIN_BETWEEN_TWO(
                mcsToPrbInfo_p->prbMap[idx], 
                buddyMcsToPrbInfo_p->prbMap[idx]);

        if (tempMax >= minPRBIndex)
        {
            idx += minPRBIndex + 1;
            continue;
        }
        /* If reported Rbs at this idx are not sufficient even to cater
         * minimum RB requirement, skip those number of reported RBs */
        if( minRBReq > minPRBIndex)
        {
            idx += minPRBIndex;
            continue;
        }
        tempCount = 0;
        /*  Checking if orthogonal  N2dmrsPair are available or not */
        if(MAC_SUCCESS != checkOrthoN2dmrsPair(idx,
                    /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
                    phichSubFrameNum,
#elif FDD_CONFIG
                    hiIndex,
#endif
                    availableN2dmrs_p,
                    internalCellIndex
#ifdef TDD_CONFIG
                    ,iPhich
#endif
                    /*TDD Config 0 Changes End*/ 
                    ))
        {
            idx++;
            /* Orthogonal  N2dmrsPair are not available at this RBIndex;
             * move to next free location; */
            continue;
        }
        /* Simultaneously, rbMap[idx], mcsToPrbInfo_p->prbMap[idx] and 
         * buddyMcsToPrbInfo_p->prbMap[idx] should be available */
        while ((idx <= mcsToPrbEndIndex)&&
                (FREE_RESOURCE == rbMap[idx]) &&
                (mcsToPrbInfo_p->prbMap[idx]!= 0 ) && 
                (buddyMcsToPrbInfo_p->prbMap[idx]!= 0 ))
        {
            tempCount++;
            idx++;
            if(tempCount >= maxReqRBs)
            {
                /* Here, maxReqRBs are available;no need to check further.*/
                *availableRBs = getUplinkMinRBsInPowerOfTwoThreeFive(tempCount);
                *maxAvailableRBsStartIdx = idx - tempCount;
                return MAC_SUCCESS;
            }
        }
        /* Update tempMax if tempCount > tempMax. 
         * Also store the startPosition & corresponding NdmrsIndex */
        if(tempCount > tempMax)
        {
            tempMax = tempCount;
            *maxAvailableRBsStartIdx = idx - tempCount;
            tempAvailableN2dmrsIndex = *(availableN2dmrs_p);
        }
        idx++;
    } /* Close of outer while loop */

    /*No orthogonal ndmrs pair found; return failure & check on lower mcs */
    if( *(availableN2dmrs_p) == INVALID_N2DMRS_INDEX_VALUE)
    {
        return MAC_FAILURE;
    }
    /* If we have received some contiguous RBs then return those many RBs*/   
    if(tempMax)
    {
        tempMax = getUplinkMinRBsInPowerOfTwoThreeFive(tempMax);
    }

    *availableRBs = tempMax;
    /* Update NdmrsIndex corresponding to maxContigous available */
    if( tempAvailableN2dmrsIndex != INVALID_N2DMRS_INDEX_VALUE)
    {
        *(availableN2dmrs_p) = tempAvailableN2dmrsIndex;
    }
    return MAC_SUCCESS;
}

/*****************************************************************************************
 * Function Name     :  allocateFrequencySelectiveUplinkResourcesULMUMIMO
 * Inputs            :  freeRBs - Number of free RBs that can be allocated,
 *                      pdcchULNewUE_p - Pointer to PdcchULNewUE,
 *                      inputRBs - Number of RBs allocated by UL Strategy,
 *                      rbMapNum - Index to rbMapNodeArr_g,
 *                      ulUEContext_p - Pointer to UE's UL context,
 *                      bufferRBs_p - Pointer to buffer RBs which specify the
 *                                    number by which PUSCH UL resource allocation
 *                                    differs from the resources allocated by UL Strategy,
 *                      resourceReserved_p - Pointer to the UE UL
 *                                           resources to be filled in this function,
 *                      allocatedMCS_p - Pointer to the UE UL MCS to be
 *                                       allocated in this function,
 *                      hiIndex,
 *                      currentTTI,
 *                      tpc,
 *                      ueMaxPowerFlag_p,
 *                      ueMinPowerFlag_p,
 *                      ueIndexingInfo_p - Pointer to DirectIndexingUEInfo in case of FDD
 *                                         Configured, if TDD Configured Pointer to
 *                                         UeScheduledInfo,
 *                      subFrameNumber - Subframe number,
 *                      ulDelay - UL Delay
 *                      internalCellIndex - Cell-Index used at MAC
 * Outputs           :  None
 * Returns           :  MAC_SUCCESS/MAC_FAILURE
 * Description       :  This function try to alloacte resources for FSS UL-MIMO.
 *                      If not able to share resources, fallback to Non-FSS UL-MIMO.
 *                      Again if not able to share resources, fallback to Non-FSS single
 *                      allocation.        
 *      Procedure to follow FSS UL-MIMO resource alloaction is :                     
 *      1. For new transmission it shall try to allocate resources in the priority order 
 *         starting from the best PRBs (on which higher SNR is reported) with the intent 
 *         to increase the system throughput. It shall take the channel quality, MCS to PRB 
 *         mapping of the UE & its buddy UE from CSI Manager. It shall take highestReported 
 *         MCSIndex as min of highestReported MCS of both UEs.
 *      2. Check whether sufficient free RBs are available for maxOfMinDataSize [ = max of
 *         min datasize of both UEs]. if not then fallback to Non-FSS UL-MIMO.
 *      3. Check whether maxContiguous RBs are available to fulfill minRB req of both UEs
 *         If either of PrbMap [either UE's mcsToPrbMap or UE's Buddy buddyMcsToPrbMap ]
 *         have RBs less than minRBs then move to lower mcs Index for more RBs.
 *      4. if minRbs are available, then try to serve maxDataSize. 
 *      5. As avgMCS used by strategy may be different from minAvgMCS, check again against
 *         ATB for inputRBs.
 *      6. Check for longest available RB set and corresponding orthogonal N2dmrs Index.
 *         If no orthogonal N2dmrs Index is available corresponding to all free RB indecies,
 *         move to lower MCS value.
 *      7. If orthogonal N2dmrs pair is available then check whether there are sufficient
 *         RBs are available or not to serve minRBReq of the buddy pair.
 *      8. If minStrictRBs are available, then
 *         - check if the same resources are available at a higher MCS(greater
 *         than min of Highest reportes MCSs for the two UEs) for a UE
 *         - reserve the resources.
 *         - reserve corresponding orthogonal N2dmrs pair.
 *         - update harqInfo with reserved Phich info
 *         - return successfully.
 *      9. If orthogonal N2dmrs pair is available but sufficient RBs are not available
 *         to serve minRBReq of the buddy pair then move to lower MCS.
 *      10. If not able to share resources for FSS UL-MIMO, fallback to Non-FSS UL-MIMO.       
 ****************************************************************************************/
STATIC  MacRetType allocateFrequencySelectiveUplinkResourcesULMUMIMO(
        UInt8 freeRBs,
        PdcchULNewUE *pdcchULNewUE_p,    
        UInt8  inputRBs,
        UInt8  rbMapNum,
        ULUEContext* ulUEContext_p,
        SInt8 *bufferRBs_p,
        AllocatedResInfo *resourceReserved_p,
        UInt8 *allocatedMCS_p,
        UInt8 hiIndex,
        /* SPR 15909 fix start */
        tickType_t currentTTI,
        /* SPR 15909 fix end */
        TpcTriggeredToPowerCorrection  tpc,
        UInt32 *ueMaxPowerFlag_p,
        UInt32 *ueMinPowerFlag_p,
#ifdef FDD_CONFIG
        /* +- SPR 17777 */
        DirectIndexingUEInfo*  ueIndexingInfo_p
#elif TDD_CONFIG
        UeScheduledInfo *ueIndexingInfo_p
#endif
#ifdef FDD_CONFIG
,UInt8 subFrameNumber
,UInt8 ulDelay
/* +- SPR 17777 */
#endif
,InternalCellIndex internalCellIndex
/*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
,UInt8 ulSubFrameNum
,UInt8 phichSubFrameNum
#endif
/*TDD Config 0 Changes End*/ 
)
{
    UInt16        firstUeIndex        = ulUEContext_p->ueIndex;
    UInt16        buddyUeIndex        = ueIndexingInfo_p->buddyUeIndex; 
    ULUEContext*  buddyUlUEContext_p  = PNULL;
    ULHarqInfo*   tempULHarqInfo_p    = PNULL;
    /* Rel 5.3: Coverity 32365 Fix Start */
    MacRetType    retValue            = MAC_FAILURE;
    /* Rel 5.3: Coverity 32365 Fix End */
    UInt8         startIdx            = 0;
    UInt8         mcsIdx              = 0;
    UInt8         minRequiredRBs      = 0; 
    UInt8         maxRBs              = 0;
    UInt8         availableRBs        = 0;
    /*533-GCC-warning-fix - code removed*/
    UInt8         nDmrs               = MAX_DMRS_VALUE;
    UInt8         orthoN2Dmrs         = MAX_DMRS_VALUE;
    UInt8         nPhichGrp           = 0;
    UInt8         orthoNPhichGrp      = 0;
    UInt8         nPhichSeq           = 0;
    UInt8         orthoNPhichSeq      = 0;

    UInt8   availableN2dmrsIndex    = INVALID_N2DMRS_INDEX_VALUE;
    UInt8        addReqRB           = 0;
    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    UInt8         iPhich              = 0;
#endif
    /*TDD Config 0 Changes End*/ 
    /* Obtaining maxOfMinDataSize, maxOfMaxDataSize and minAvgMCS for both the UEs */
    BuddyPairInfo* buddyPairInfo_p  = buddyPairInfo_gp[internalCellIndex] + firstUeIndex;
    /* No check required for valid buddyUeIndex as this func is not getting
     * called otherwise */
    buddyUlUEContext_p = ulUECtxInfoArr_g[buddyUeIndex].ulUEContext_p;
#ifdef FDD_CONFIG
    UInt8 containerTick = (currentTTI + ulDelay)% MAX_PDCCH_CONTAINER;
    PdcchULOutputInfo* buddyTempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;

    DirectIndexingUEInfo*  buddyUEIndexingInfo_p = \
                                                   &(buddyTempPdcchULOutput_p->directIndexingUEInfo[buddyUeIndex]);
#elif TDD_CONFIG
    /*TDD Config 0 Changes Start*/
    /** SPR 14204 Fix : Start **/
    UeScheduledInfo *buddyUEIndexingInfo_p = 
        buddyUlUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulSubFrameNum];
    /** SPR 14204 Fix : End **/
    /*TDD Config 0 Changes End*/
#endif

    UInt8  buddyInputRBs = buddyUEIndexingInfo_p->requiredRB; 


    /* Obtaining the MCSToPrbMapInfo for the buddy UEs */
    CSIContext      *csiContext_p       = &(ulUEContext_p->csiContext);
    CSIContext      *buddyCsiContext_p  = &(buddyUlUEContext_p->csiContext);
    MCSToPrbMapInfo *mcsToPrbMap_p      = csiContext_p->mcsToPrbMap_p;
    MCSToPrbMapInfo *buddyMcsToPrbMap_p = buddyCsiContext_p->mcsToPrbMap_p;
    /*CA Changes start  */
    RbMapNode       *rbMapNode_p        = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    /*SPR 16417 Changes Start*/
    TpcTriggeredToPowerCorrection buddyTpc = getTpcForPusch(
            buddyUlUEContext_p
#ifdef FDD_CONFIG 
            ,currentTTI + ulDelay
            ,internalCellIndex
#elif TDD_CONFIG
            ,currentTTI
            ,internalCellIndex
            ,ulSubFrameNum
#endif
            );
    /*SPR 16417 Changes End*/
    /* Obtaining minOfHighestReportedMCS for buddy UEs */
    buddyPairInfo_p->minOfHighestReportedMCS = MIN_BETWEEN_TWO( 
            csiContext_p->highestReportedMCS, 
            buddyCsiContext_p->highestReportedMCS);

    freeRBs =  getUplinkMinRBsInPowerOfTwoThreeFive(freeRBs);
    /* +DYNAMIC_ICIC */
    /*CA Changes start  */
    ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + ulPowerMaskIndex_g[internalCellIndex];
    /*CA Changes end  */
    SInt32 buddyUeRegionPowerMask = ulPowerMaskInfo_p->powerMask[buddyUlUEContext_p->userLocationType];
    SInt32 regionPowerMask = ulPowerMaskInfo_p->powerMask[ulUEContext_p->userLocationType];
    UInt8 minRBsPrev = 0, mcsIdxPrev = 0, mcsIdxBuddyUe = 0;
    UInt32 buddyUeMaxPowerFlag = 0, buddyUeMinPowerFlag = 0;
    /* -DYNAMIC_ICIC */

    for(mcsIdx = buddyPairInfo_p->minOfHighestReportedMCS;
            ( (mcsIdx >= buddyPairInfo_p->minAvgMCS) && \
              (mcsIdx != INVALID_MCS_INDEX) );
            mcsIdx = mcsGroupingTable_g[mcsIdx].previousMCS )
    {
        /* Determining minRequiredRBs for the buddy Ues */
        /* SPR 5999 changes start */
        /*CA Changes start  */
        minRequiredRBs = DETERMINE_RBs_UL(mcsIdx, (buddyPairInfo_p->maxOfMinDataSize), internalCellIndex);
        /*CA Changes end  */
        /* SPR 5999 changes end */
        minRequiredRBs = getUplinkMaxRBsInPowerOfTwoThreeFive(minRequiredRBs);

        if(freeRBs < minRequiredRBs)
        {
            /* Number of RBs left to be allocated in PUSCH resource map 
             * are less that that required by this UE on MCS=mcsIdx. 
             * No need to look further because a lower MCS will require still more 
             * RBs to serve the minDataSize. Do non-FSS scheduling for buddy pair. */
            LOG_MAC_MSG(MAC_ULSCH_FREQ_SEL_SCH_ID,LOGDEBUG,MAC_PUSCH,\
                    currentTTI,\
                    freeRBs,\
                    minRequiredRBs,\
                    mcsIdx,\
                    mcsToPrbMap_p[mcsIdx].maxContiguous,\
                    buddyMcsToPrbMap_p[mcsIdx].maxContiguous,\
                    0.0,0.0,__func__,"freeRBs < minRequiredRBs");   
            break;
        }
        else if ( (mcsToPrbMap_p[mcsIdx].maxContiguous < minRequiredRBs) || 
                (buddyMcsToPrbMap_p[mcsIdx].maxContiguous < minRequiredRBs) )
        {
            /* Not enough PRBs are reported on this MCS to 
             * serve the minDataSize of either UE . 
             * Need to look at next lower MCS.*/
            LOG_MAC_MSG(MAC_ULSCH_FREQ_SEL_SCH_ID,LOGDEBUG,MAC_PUSCH,\
                    currentTTI,\
                    freeRBs,\
                    minRequiredRBs,\
                    mcsIdx,\
                    mcsToPrbMap_p[mcsIdx].maxContiguous,\
                    buddyMcsToPrbMap_p[mcsIdx].maxContiguous,\
                    0.0,0.0,__func__,"mcsToPrbMap_p[mcsIdx].maxContiguous < minRequiredRBs");
            continue;
        }
        else
        {
            if(buddyPairInfo_p->minAvgMCS < mcsIdx)
            {    
                /* For new ATB code check if  higher mcs leads to
                 * a problem for minRBs itself
                 */

                /* ATB Code with FSS */
                minRBsPrev = minRequiredRBs;
                mcsIdxPrev = mcsIdxBuddyUe = mcsIdx;
                minRequiredRBs = MIN_BETWEEN_TWO(
                        validateATB( minRequiredRBs,&mcsIdx,ulUEContext_p,                        
                            tpc.powerCorrectionVal,
                            ueMaxPowerFlag_p,ueMinPowerFlag_p,regionPowerMask,NEW_TX,
                            internalCellIndex),

                        validateATB( minRequiredRBs,&mcsIdxBuddyUe, buddyUlUEContext_p,
                            buddyTpc.powerCorrectionVal,
                            &buddyUeMaxPowerFlag, &buddyUeMinPowerFlag, 
                            buddyUeRegionPowerMask,NEW_TX,internalCellIndex)
                        );
                mcsIdx = MIN_BETWEEN_TWO(mcsIdx, mcsIdxBuddyUe);

                /* Making two if statements for brevity */
                if ( mcsIdx == mcsIdxPrev )
                {
                    if ( minRBsPrev > minRequiredRBs )
                    {
                        continue;
                    }
                }
                else if ( (mcsIdx < buddyPairInfo_p->minAvgMCS) || 
                        (determineUlTBSize(mcsIdx, minRequiredRBs) < buddyPairInfo_p->maxOfMinDataSize) )
                {
                    continue;
                }
                /* ATB code change end */

                /* Try to serve maxDataSize*/   
                /* SPR 5999 changes start */
                /*CA Changes start  */
                maxRBs = DETERMINE_RBs_UL(mcsIdx, (buddyPairInfo_p->maxOfMaxDataSize), internalCellIndex);
                /*CA Changes end  */
                /* SPR 5999 changes end */
                UInt8 requiredRBs = minRequiredRBs;

                if(minRequiredRBs < maxRBs)
                {
                    addReqRB = maxRBs - minRequiredRBs;
                    /* UE is given its max required RBs and still some RBs are
                     * left in Bufferpool, As additional rbs are given from bufferpool,
                     * it gets deducted from bufferpool*/
                    *bufferRBs_p += inputRBs + buddyInputRBs - minRequiredRBs;
                    if(*bufferRBs_p > addReqRB)
                    {
                        requiredRBs  += addReqRB;

                    }
                    /* UE is given all the available RBs from bufferpool and
                     * it is made to zero as no more rbs exist in it*/
                    else
                    {
                        requiredRBs  += *bufferRBs_p;
                    }
                }


                /* Determining the totalInputRBs for the buddyUEs as given by strategy */

                mcsIdxPrev = mcsIdxBuddyUe = mcsIdx;
                requiredRBs = MIN_BETWEEN_TWO(
                        validateATB(requiredRBs, &mcsIdx,ulUEContext_p,                        
                            tpc.powerCorrectionVal,
                            ueMaxPowerFlag_p,ueMinPowerFlag_p,regionPowerMask,
                            NEW_TX,internalCellIndex),

                        validateATB( requiredRBs,&mcsIdxBuddyUe, buddyUlUEContext_p,
                            buddyTpc.powerCorrectionVal,
                            &buddyUeMaxPowerFlag, &buddyUeMinPowerFlag,
                            buddyUeRegionPowerMask,NEW_TX,internalCellIndex)
                        );
                mcsIdx = MIN_BETWEEN_TWO(mcsIdx, mcsIdxBuddyUe);

                if ( (mcsIdx < buddyPairInfo_p->minAvgMCS) || 
                        (determineUlTBSize(mcsIdx, requiredRBs) < buddyPairInfo_p->maxOfMinDataSize) )
                {
                    /* Revert buffer RBs */
                    *bufferRBs_p +=  minRequiredRBs - inputRBs - buddyInputRBs;
                    continue;
                }
                /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
                iPhich = calculateIPhichValue(ulSubFrameNum,internalCellIndex);
#endif
                /*TDD Config 0 Changes End*/
                if( MAC_FAILURE == checkAvailableRBsToReturnLongestSetULMUMIMO(
                            &mcsToPrbMap_p[mcsIdx],
                            &buddyMcsToPrbMap_p[mcsIdx],
                            requiredRBs,
                            &availableRBs,
                            &rbMapNode_p->availableResourceInfo[ulUEContext_p->userLocationType],
                            rbMapNode_p->rbMap,
                            &startIdx,
                            /* +- SPR 17777 */
#ifdef FDD_CONFIG
                            hiIndex,
#endif
                            /* +- SPR 17777 */
                            &availableN2dmrsIndex,
                            minRequiredRBs,
                            internalCellIndex
                            /*TDD Config 0 Changes Start*/                             
#ifdef TDD_CONFIG
                            ,iPhich
                            ,phichSubFrameNum
#endif
                            /*TDD Config 0 Changes End*/ 
                            ))
                            {
                                /* Revert back buffer RBs */
                                *bufferRBs_p +=  minRequiredRBs - inputRBs - buddyInputRBs;

                                /* No orhogonal Ndmrs pair is available on this mcsIdx.
                                 * Look at next lower MCS.*/
                                continue;
                            }
                if(
                        /*Coverity_73499 Fix Start*/
                        (0 < availableRBs) && 
                        /*Coverity_73499 Fix End*/

                        (availableRBs >= minRequiredRBs) 
                  )
                {
                    checkAvailabilityOfHigherMCS(&mcsIdx,startIdx,availableRBs,ulUEContext_p,
                            buddyUlUEContext_p,buddyPairInfo_p,currentTTI,internalCellIndex
#ifdef TDD_CONFIG
                            ,ulSubFrameNum
#endif
                            );
                    /*Able to serve atleast minDataSize*/
                    /* Call the functions below for both the buddy UEs */
                    /*Coverity Id 32413 Fix Start*/
                    MAC_UL_RESERVE_RESOURCES(
                            startIdx,\
                            availableRBs,\
                            resourceReserved_p,\
                            bufferRBs_p,\
                            minRequiredRBs,\
                            ulUEContext_p->userLocationType,\
                            rbMapNode_p,\
                            rbMapNode_p->rbMap,
                            internalCellIndex);

                    *allocatedMCS_p=mcsIdx;
                    /*Coverity Id 32413 Fix End*/
                    /* Reserve orthogonal ndmrs pair from availableN2dmrsIndex */
                    nDmrs     = orthoNdmrsMap_g[availableN2dmrsIndex][0];
                    /*TDD Config 0 Changes Start*/ 
#ifdef FDD_CONFIG
                    nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][startIdx][nDmrs];
#elif TDD_CONFIG                    
                    nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][startIdx][nDmrs];
#endif
                    nPhichSeq = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][startIdx][nDmrs];

                    orthoN2Dmrs    = orthoNdmrsMap_g[availableN2dmrsIndex][1];
#ifdef FDD_CONFIG                    
                    orthoNPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][startIdx][orthoN2Dmrs];
#elif TDD_CONFIG                    
                    orthoNPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][startIdx][orthoN2Dmrs];
#endif
                    /*TDD Config 0 Changes End*/ 
                    orthoNPhichSeq = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][startIdx][orthoN2Dmrs];
#ifdef FDD_CONFIG
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp].bitMask &=
                        ~(ONE << nPhichSeq);
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][orthoNPhichGrp].bitMask &=
                        ~(ONE << orthoNPhichSeq);

#elif TDD_CONFIG
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp] &=
                        ~(ONE << nPhichSeq);
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][orthoNPhichGrp] &=
                        ~(ONE << orthoNPhichSeq);

#endif


                    /* Update UE's Harq Info */
#ifdef FDD_CONFIG
                    tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[ueIndexingInfo_p->harqIndex]);
#elif TDD_CONFIG
                    tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[ueIndexingInfo_p->ulHarqProcessId]);
#endif

                    tempULHarqInfo_p->nDmrs      = nDmrs;
                    tempULHarqInfo_p->buddyNdmrs = orthoN2Dmrs;
                    tempULHarqInfo_p->nPhichGrp  = nPhichGrp;
                    tempULHarqInfo_p->nPhichSeq  = nPhichSeq;
#ifdef FDD_CONFIG
                    tempULHarqInfo_p->sfUsed     = hiIndex;
#elif TDD_CONFIG
                    tempULHarqInfo_p->sfUsed     = phichSubFrameNum;
#endif

                    *allocatedMCS_p = mcsIdx;

                    /* Updated UE Context that PDSCH is allocated to UE */
                    ulUEContext_p->ulMUMIMOInfo.isPuschAllocated = TRUE;

                    /*updating buffer Rb pool with Rb's saved due to sharing by buddy pair*/

                    /* 
                     * Since MAC_UL_RESERVE_RESOURCES has already taken care of updatng bufferRBs 
                     * when first buddy's resources are reserved in excess
                     * Therefore we only need to update the RBs saved due to second UE not being allocated
                     * seperate PUSCH region.
                     */

                        LOG_MAC_MSG(MAC_ULSCH_FSS_INFO_ID,LOGDEBUG,MAC_PUSCH,\
                                getCurrentTick(),\
                                *bufferRBs_p,\
                                minRequiredRBs,\
                                0,\
                                *allocatedMCS_p,\
                                0,\
                                0.0,\
                                0.0,\
                                __func__,"");

#ifdef SSI_DEBUG_STATS
                    /* CA Stats Changes Start */
                    LTE_MAC_UPDATE_TOTAL_RB_SHARED(availableRBs, internalCellIndex);
#endif

                    LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                            ulUEContext_p->ueIndex,
                            currentTTI,
                            NEW_TX,
#ifdef FDD_CONFIG
                            ueIndexingInfo_p->harqIndex,
#elif TDD_CONFIG
                            ueIndexingInfo_p->ulHarqProcessId,
#endif
                            availableRBs,
                            startIdx,
                            ueIndexingInfo_p->ulGrantRequestType,
                            determineUlTBSize(*allocatedMCS_p,availableRBs),
                            TRUE,
                            *allocatedMCS_p,
                            0,
                            tpc.tpcCommand
                            ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                            buddyUeIndex,
                            ulUEContext_p->userLocationType,
                            nDmrs,
                            ulUEContext_p->modulationScheme,
                            internalCellIndex
                                );
                    /* CA Stats Changes End */
                    return MAC_SUCCESS;
                }
                else
                {
                    /* Revert back buffer RBs */
                    *bufferRBs_p +=  minRequiredRBs - inputRBs - buddyInputRBs;

                    /* Not able to serve minDataSize on this mcsIdx. 
                     * Look at next lower MCS. */
                    continue;
                }
            }/* (buddyPairInfo_p->minAvgMCS <= mcsIdx) */
        }/* maxContiguous < minRequiredRBs */
    }/* End of for loop */

    /* FSS was not successful. Do non-FSS for pair using minAvgMCS. */   
    retValue = allocateNonFrequencySelectiveUplinkResources(
            ulUEContext_p,
            pdcchULNewUE_p,
            ueIndexingInfo_p,
            resourceReserved_p,
#ifdef FDD_CONFIG 
            subFrameNumber,
#elif TDD_CONFIG
            rbMapNum,
#endif
            freeRBs,
            bufferRBs_p,
            tpc,
            ueMaxPowerFlag_p,
            ueMinPowerFlag_p,
            allocatedMCS_p,
            hiIndex,
            currentTTI
#ifdef FDD_CONFIG
            ,ulDelay
#endif
            ,internalCellIndex
            /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
            ,ulSubFrameNum
            ,phichSubFrameNum
#endif
            /*TDD Config 0 Changes End*/            
            );
#ifdef SSI_DEBUG_STATS
    /* CA Stats Changes Start */
    LTE_MAC_UPDATE_FAILURE_CAUSE(FSS_TO_NONFSS_TRANSITION, internalCellIndex);
    /* CA Stats Changes End */
#endif
    return retValue;
}
/* UL_MU_MIMO_CHG End */

/*Cyclomatic Complexity changes - Starts here */
/****************************************************************************************
 * Function Name  : calculateBuddyPairRBs
 * Inputs         : pdcchULMUMIMOCandidateInfo_p - Pointer to PdcchULMUMIMOCandidateInfo,
 *                pdcchULOutputInfo_p - Pointer to PdcchULOutputInfo,
 *                pdcchULNewUEInfo_p - Pointer to PdcchULNewUEInfo,
 *                ulUEContextFirst_p - Pointer to ULUEContext,
 *                firstUEIndexingInfo_p Indexing info of first UE,
 *                firstUeIndex - Index of first UE,
 *                currentTick - Current Tick,
 *                ueCount -UE Count,
 *                minRbLoss,
 *                subFrameNumber - Subframe number
 *                internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This Function will select buddy UEs
 ***************************************************************************************/
                    STATIC  void calculateBuddyPairRBs(
                            PdcchULMUMIMOCandidateInfo *pdcchULMUMIMOCandidateInfo_p,
                            /* +- SPR 17777 */
                            PdcchULNewUEInfo           *pdcchULNewUEInfo_p,
                            ULUEContext                *ulUEContextFirst_p,
#ifdef FDD_CONFIG
                            PdcchULOutputInfo          *pdcchULOutputInfo_p,
                            /* +- SPR 17777 */
                            DirectIndexingUEInfo       *firstUEIndexingInfo_p,
#elif TDD_CONFIG
                            UeScheduledInfo            *firstUEIndexingInfo_p,
#endif
                            UInt16                     *firstUeIndex,
                            /* SPR 15909 fix start */
                            tickType_t                  currentTick,
                            /* SPR 15909 fix end */
                            UInt32                     ueCount,
                            UInt16                     minRbLoss
                            /* +- SPR 17777 */
                            ,InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
                    /*TDD Config 0 Changes Start*/ 
                    ,UInt8                     ulSubFrameNum
                    /*TDD Config 0 Changes End*/ 
#endif
                    )
{
    BuddyPairInfo              *buddyPairInfo_p              = PNULL;
#ifdef FDD_CONFIG
    DirectIndexingUEInfo       *secondUEIndexingInfo_p       = PNULL;
#elif TDD_CONFIG
    UeScheduledInfo            *secondUEIndexingInfo_p       = PNULL;
    ULUEContext                *ulUEContextSecond_p          = PNULL;
#endif

    UInt32 firstUEMinDataSize                                = 0;
    UInt32 firstUEMaxDataSize                                = 0;
    UInt32 secondUEMinDataSize                               = 0;
    UInt32 secondUEMaxDataSize                               = 0;
    UInt32 firstUEMinStrictReqRB                             = 0;
    UInt32 secondUEMinStrictReqRB                            = 0;
    UInt16 secondUeIndex                                     = INVALID_UE_INDEX;
    UInt16 tempBuddyUeIndex                                  = INVALID_UE_INDEX;
    UInt16 maxBytesSupportByMinCategory                      = 0;
    UInt16 firstUECategoryBytesSupport                       = 0;
    UInt16 secondUECategoryBytesSupport                      = 0;
    UInt32 nextUeCount                                       = 0;
    UInt8 firstReferenceIndex                                = 0;
    UInt8 secondReferenceIndex                               = 0;
    UInt8 firstUEAvgMCS                                      = 0;
    UInt8 secondUEAvgMCS                                     = 0;
    UInt8 retVal                                             = 0;

    for (nextUeCount = ueCount+1; nextUeCount < pdcchULMUMIMOCandidateInfo_p->count; nextUeCount++)
    {
        secondUeIndex = pdcchULMUMIMOCandidateInfo_p->pdcchULMUMIMOCandidateUE[nextUeCount].ueIndex;
#ifdef FDD_CONFIG
        secondUEIndexingInfo_p = &pdcchULOutputInfo_p->directIndexingUEInfo[secondUeIndex];
#elif TDD_CONFIG
        ulUEContextSecond_p = ulUECtxInfoArr_g[secondUeIndex].ulUEContext_p;
        /*TDD Config 0 Changes Start*/ 
        /** SPR 14204 Fix : Start **/
        secondUEIndexingInfo_p = ulUEContextSecond_p->\
                                 ulUeScheduledInfoPointerArray[internalCellIndex][ulSubFrameNum];
        /** SPR 14204 Fix : End **/
        /*TDD Config 0 Changes End*/ 
#endif
        /*To check whether this UE has any buddy UE or not. If there is any, than continue with other UEs*/
        if (secondUEIndexingInfo_p->buddyUeIndex < MAX_UE_SUPPORTED)
        {
            continue;
        }


        /*Get the reference index to fetch avgMCS and RequiredRBs from its UL UE Context*/
        firstReferenceIndex = firstUEIndexingInfo_p->uePosInPdcchULNewUEArray;
        secondReferenceIndex = secondUEIndexingInfo_p->uePosInPdcchULNewUEArray;

        firstUEMinDataSize  = pdcchULNewUEInfo_p->pdcchULNewUEArray[firstReferenceIndex].minDataSize;
        firstUEMaxDataSize  = pdcchULNewUEInfo_p->pdcchULNewUEArray[firstReferenceIndex].maxDataSize;
        /*To get the ReqRBs to cater both the UEs, first get the Data Size and AvgMCS for both UEs*/
        firstUEAvgMCS = pdcchULNewUEInfo_p->pdcchULNewUEArray[firstReferenceIndex].avgMCS;
        secondUEAvgMCS = pdcchULNewUEInfo_p->pdcchULNewUEArray[secondReferenceIndex].avgMCS;

        /*If the difference of avgMcs for both UEs is greater than UL_MU_MIMO_DIFFERENCE_THRESHOLD than proceed */
        if (ABS(firstUEAvgMCS - secondUEAvgMCS) > UL_MU_MIMO_DIFFERENCE_THRESHOLD )
        {
#ifdef SSI_DEBUG_STATS
            /* CA Stats Changes Start */
            LTE_MAC_UPDATE_FAILURE_CAUSE(AVGMCS_DIFF_THRESOLD_FAILURE, 
                    internalCellIndex);
            /* CA Stats Changes End */
#endif
            continue;
        }

        /*SPR_9204_START*/

        if (FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->freqSelectiveSchUL) 
        {
            retVal = selectbuddyUeFSS(secondUeIndex,secondUEIndexingInfo_p,pdcchULNewUEInfo_p,
                    firstUeIndex,ulUEContextFirst_p,firstUEIndexingInfo_p,
                    &minRbLoss,&firstUEMinStrictReqRB,&secondUEMinStrictReqRB,&tempBuddyUeIndex,currentTick,&buddyPairInfo_p,
                    internalCellIndex
#ifdef TDD_CONFIG                    
                    ,ulSubFrameNum
#endif                    
                    );
            if (MAC_FAILURE == retVal)
            {
                continue;
            }
        }
        else
        {
            retVal = selectbuddyUeNonFSS(secondUeIndex,secondUEIndexingInfo_p,pdcchULNewUEInfo_p,
                    firstUeIndex,ulUEContextFirst_p,firstUEIndexingInfo_p,
                    &minRbLoss,&firstUEMinStrictReqRB,&secondUEMinStrictReqRB,&tempBuddyUeIndex,currentTick,&buddyPairInfo_p,
                    internalCellIndex
#ifdef TDD_CONFIG                    
                    ,ulSubFrameNum
#endif                    
                    );
            if (MAC_FAILURE == retVal)
            {
                continue;
            }
        }
    }/*close of for loop(nextUeCount)*/
    /* Update the UL contxt for each UEs with their buddy UEs if pairing was successful*/
    /*Coverity_31957 Fix start*/
    if ( (PNULL != buddyPairInfo_p) && ( tempBuddyUeIndex != INVALID_UE_INDEX))
        /*Coverity_31957 Fix end*/
    {
        /*Assign the buddy UE index in its UE Context*/
        firstUEIndexingInfo_p->buddyUeIndex = tempBuddyUeIndex;
#ifdef FDD_CONFIG
        secondUEIndexingInfo_p = &pdcchULOutputInfo_p->directIndexingUEInfo[tempBuddyUeIndex];
#elif TDD_CONFIG
        ulUEContextSecond_p = ulUECtxInfoArr_g[tempBuddyUeIndex].ulUEContext_p;
        /*TDD Config 0 Changes Start*/
        /** SPR 14204 Fix : Start **/
        secondUEIndexingInfo_p = ulUEContextSecond_p->
            ulUeScheduledInfoPointerArray[internalCellIndex][ulSubFrameNum];
        /** SPR 14204 Fix : End **/
        /*TDD Config 0 Changes End*/
#endif
        secondUEIndexingInfo_p->buddyUeIndex = *firstUeIndex;
        secondReferenceIndex = secondUEIndexingInfo_p->uePosInPdcchULNewUEArray;
        secondUEMinDataSize  = pdcchULNewUEInfo_p->pdcchULNewUEArray[secondReferenceIndex].minDataSize;
        secondUEMaxDataSize  = pdcchULNewUEInfo_p->pdcchULNewUEArray[secondReferenceIndex].maxDataSize;
        /* SPR 19600 Changes start */
        secondUECategoryBytesSupport = ulUeCategoryInfo_g[(ulUECtxInfoArr_g[tempBuddyUeIndex].
                ulUEContext_p->ueCategory) - 1].maxBytesULSCHTb;
        firstUECategoryBytesSupport = ulUeCategoryInfo_g[ulUEContextFirst_p->ueCategory - 1].maxBytesULSCHTb;
        /* SPR 19600 Changes end */
        maxBytesSupportByMinCategory = MIN_BETWEEN_TWO(firstUECategoryBytesSupport, secondUECategoryBytesSupport);
        if (FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->freqSelectiveSchUL) 
        {
            /*Assigning the BuddyPairInfo structure elements starts here which can be used for Resource Allocation*/
            buddyPairInfo_p->maxOfMinDataSize = MAX_BETWEEN_TWO( firstUEMinDataSize, secondUEMinDataSize);
            buddyPairInfo_p->maxOfMaxDataSize = MIN_BETWEEN_TWO(maxBytesSupportByMinCategory,
                    MAX_BETWEEN_TWO( firstUEMaxDataSize, secondUEMaxDataSize) );
            buddyPairInfo_p->minAvgMCS = MAX_BETWEEN_TWO(pdcchULNewUEInfo_p->pdcchULNewUEArray[firstReferenceIndex].avgMCS,
                    pdcchULNewUEInfo_p->pdcchULNewUEArray [secondReferenceIndex].avgMCS);
            buddyPairInfo_p->buddyMaxDataSize = secondUEMaxDataSize;
            buddyPairInfo_p->buddyAvgMCS = pdcchULNewUEInfo_p->pdcchULNewUEArray [secondReferenceIndex].avgMCS;
            LOG_MAC_MSG(MAC_BUDDY_UE_INFO_ID,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    *firstUeIndex,\
                    tempBuddyUeIndex,\
                    buddyPairInfo_p->maxOfMinDataSize,\
                    buddyPairInfo_p->maxOfMaxDataSize,\
                    buddyPairInfo_p->minAvgMCS,\
                    0.0,\
                    0.0,\
                    __func__,"BuddySelectionSuccessfull");
        }
        else
        {
            /*Assigning the BuddyPairInfo structure elements starts here which can be used for Resource Allocation*/
            buddyPairInfo_p->buddyMaxDataSize = secondUEMaxDataSize;
            buddyPairInfo_p->buddyAvgMCS = pdcchULNewUEInfo_p->pdcchULNewUEArray [secondReferenceIndex].avgMCS;
        }
        /*SPR_9204_END*/
        /*Initialize the minStrictRBReq of buddypairinfo with the min RBs that can be allocated */
        buddyPairInfo_p->minStrictRBReq = MAC_MIN_UL_GRANT;
        /*If any of the UE has strictRB allocation, than change the previously calculated minStrictRBReq by taking the
         * maximum between both (first and second UE) and  make strictAllocFlag as TRUE*/
        if ((firstUEIndexingInfo_p->strictRBAllocation) || (secondUEIndexingInfo_p->strictRBAllocation))
        {
            buddyPairInfo_p->minStrictRBReq = MAX_BETWEEN_TWO(firstUEMinStrictReqRB, secondUEMinStrictReqRB);
            buddyPairInfo_p->strictAllocFlag = TRUE;
        }

        /* ATB changes */
        buddyPairInfo_p->buddyMaxPowerFlag = buddyPairInfo_p->buddyMinPowerFlag = FALSE;
        /* ATB changes */

        tempBuddyUeIndex = INVALID_UE_INDEX;
    }
}
/* UL_MU_MIMO_CHG End */

/********************************************************************************
 * Function Name :     validateMinRbReq
 * Inputsi       :     minRbReq,
 *                     tempDirectIndexingUEInfo_p, 
 *                     allocatedMCS,
 *                     ulUEContext_p,
 *                     tpc, 
 *                     ueMaxPowerFlag_p, 
 *                     ueMinPowerFlag_p,
 *                     rbMapNode_p,
 *                     resourceReserved_p, 
 *                     regionPowerMask, 
 *                     freeRBs
 *                     currentTTI
 *                     ulDelay
 *                     currentGlobalTick
 *                     internalCellIndex - Cell-Index used at MAC
 * Outputs        :    None
 * returns        :    MAC_SUCCESS /MAC_FAILURE
 * Description    :    This Function will update the MinRbReq for Non
 *                     Frequency Selective Uplink Resources.
 *********************************************************************************/
        STATIC  MacRetType validateMinRbReq(
                UInt8 * minRbReq, 
#ifdef FDD_CONFIG
                DirectIndexingUEInfo * tempDirectIndexingUEInfo_p,
#elif TDD_CONFIG
                UeScheduledInfo      * tempDirectIndexingUEInfo_p,
#endif
                /* + Coverity_31960 */
                UInt8 *allocatedMCS_p, 
                /* - Coverity_31960 */
                ULUEContext* ulUEContext_p, 
                TpcTriggeredToPowerCorrection tpc, 
                UInt32   * ueMaxPowerFlag_p,
                UInt32   * ueMinPowerFlag_p,
                RbMapNode * rbMapNode_p,
                AllocatedResInfo* resourceReserved_p,
                SInt32 regionPowerMask,
                UInt32 freeRBs,
                /* SPR 15909 fix start */
                tickType_t  currentTTI
                /* SPR 15909 fix end */
#ifdef FDD_CONFIG
        ,UInt8 ulDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick
        /* SPR 15909 fix end */
#endif
        /*CA Changes start */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end */
        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
        ,UInt8 phichSubFrameNum
        ,UInt8 iPhich
        ,UInt8 *nDmrs_p
#endif
        /*TDD Config 0 Changes End*/
        )
{
    LP_SpsUlInfo ulSpsInfo_p = PNULL;
    /* + Coverity 42188 & 32783 */
    MacRetType       retValue         = MAC_FAILURE;
    /* - Coverity 42188 & 32783 */

    /* + Coverity_31960 */
    UInt8 tempAllocatedMCS = *allocatedMCS_p;

#ifdef FDD_CONFIG
    if (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG != 
            tempDirectIndexingUEInfo_p->ulGrantRequestType &&
            /* SPS changes */ 
            (UL_SPS_ACTIVATION != tempDirectIndexingUEInfo_p->ulGrantRequestType) &&
            (UL_SPS_DEACTIVATION != tempDirectIndexingUEInfo_p->ulGrantRequestType) &&
            (UL_SPS_OCCASION != tempDirectIndexingUEInfo_p->ulGrantRequestType) &&
            (TTIB_UL_SPS_OCCASION != tempDirectIndexingUEInfo_p->ulGrantRequestType) &&
            /* SPR 8206 Fix Start */
            !(tempDirectIndexingUEInfo_p->strictRBAllocation))
        /* SPR 8206 Fix End */
        /* SPS changes */ 
#elif TDD_CONFIG
        /* + SPS_TDD_Changes */
        if ((CQI_WITHOUT_DATA_ULGRANT_REQ_MSG !=
                    tempDirectIndexingUEInfo_p->ulGrantRequestType)&& 
                (UL_SPS_ACTIVATION != tempDirectIndexingUEInfo_p->ulGrantRequestType)&&
                (UL_SPS_DEACTIVATION != tempDirectIndexingUEInfo_p->ulGrantRequestType) &&
                (UL_SPS_OCCASION != tempDirectIndexingUEInfo_p->ulGrantRequestType) &&
                !(tempDirectIndexingUEInfo_p->strictRBAllocation))
            /* - SPS_TDD_Changes */
#endif
        {    
            *minRbReq = validateATB(tempDirectIndexingUEInfo_p->requiredRB,\
                    &tempAllocatedMCS,\
                    ulUEContext_p, \
                    tpc.powerCorrectionVal, \
                    ueMaxPowerFlag_p, ueMinPowerFlag_p,
                    regionPowerMask,NEW_TX,
                    internalCellIndex);
            *allocatedMCS_p = tempAllocatedMCS;
            /* - Coverity_31960 */
        }
        else
        {
            *minRbReq = tempDirectIndexingUEInfo_p->requiredRB;  
        }
    if (!tempDirectIndexingUEInfo_p->strictRBAllocation)
    {
        if (*minRbReq < tempDirectIndexingUEInfo_p->requiredRB)
        {
            tempDirectIndexingUEInfo_p->requiredRB = *minRbReq;
        }

        if(*ueMaxPowerFlag_p == FALSE)
        {
            /* In this case if someone requires, further RB can be reduced
             * then MAC should not reduce the minRBs any further.*/
            *minRbReq = MAX_ATB_PRB_RESTRICT; 
        }
        else
        {
            /* In this case since ATB has already reduced the RBs to a minimum,
             * then MAC should not reduce the minRBs any further.*/
        }
    }
    else
    {  /* If validateNprbFromATB reduces the number of RBs to less than 
        * than that given by strategy when strict Allocated is set,
        * then resource allocation should not be done.
        */
        if( (*minRbReq < tempDirectIndexingUEInfo_p->requiredRB) &&
                (CQI_WITHOUT_DATA_ULGRANT_REQ_MSG != 
                 tempDirectIndexingUEInfo_p->ulGrantRequestType) )
        {
            /* CA Stats Changes Start */
            LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                    ulUEContext_p->ueIndex,
                    currentTTI,
                    INVALID_TX,
#ifdef FDD_CONFIG
                    tempDirectIndexingUEInfo_p->harqIndex,
#elif TDD_CONFIG
                    tempDirectIndexingUEInfo_p->ulHarqProcessId,
#endif
                    0,
                    0,
                    tempDirectIndexingUEInfo_p->ulGrantRequestType,
                    0,
                    FALSE,
                    0,
                    1 /*Allocation failed*/,
                    tpc.tpcCommand
                    ,ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                    INVALID_UE_INDEX,
                    ulUEContext_p->userLocationType,
                    MAX_DMRS_VALUE,
                    ulUEContext_p->modulationScheme,
                    internalCellIndex
                        );
            /* CA Stats Changes End */
#ifdef LOG_PRINT_ENABLED
            LOG_MAC_MSG(MAC_ULSCH_FREQ_SEL_SCH_ID,LOGDEBUG,MAC_PUSCH,\
                    currentTTI,\
                    *minRbReq,\
                    tempDirectIndexingUEInfo_p->requiredRB,\
                    /* + Coverity_31960 */
                    *allocatedMCS_p,\
                    /* - Coverity_31960 */
                    0,\
                    0,\
                    0.0,0.0,__func__,\
                    "ATB does not even minimum RBs to be allocated on this MCS when strict Allocated is set");
#endif
            return retValue;
        }
        *minRbReq = tempDirectIndexingUEInfo_p->requiredRB;
    }
    if (freeRBs > 0)
    {
#ifdef FDD_CONFIG
        /* SPS Changes start */
        if ( (UL_SPS_ACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                (UL_SPS_DEACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                (UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                (TTIB_UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType) )
        {
            if ( (UL_SPS_ACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                    (UL_SPS_DEACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) )
            { 
                ulSpsInfo_p = &ulUEContext_p->spsUlInfo;
                (ulSpsInfo_p->numDciAttempts++);
                /* CA Stats Changes Start */
                LTE_MAC_UPDATE_DCI_ATTEMPT_STATS(ulUEContext_p->ueIndex, 
                        ulSpsInfo_p->numDciAttempts, internalCellIndex);
                /* CA Stats Changes End */
            }   

            AllocatedResInfo ulResAllocInfo = {0};

            ulResAllocInfo.allocatedLength = ulUEContext_p->spsUlInfo.ulResAllocInfo.allocatedRB; 
            ulResAllocInfo.allocatedIndex = ulUEContext_p->spsUlInfo.ulResAllocInfo.startingVRBIndex;
            ulResAllocInfo.allocatedRiv = ulUEContext_p->spsUlInfo.ulResAllocInfo.allocatedRiv;

            /* + Coverity_31960 */
            MAC_UL_SPS_RESERVE_RESOURCES (ulResAllocInfo.allocatedIndex,
                    ulResAllocInfo.allocatedLength, &ulResAllocInfo, 
                    &tempAllocatedMCS, 
                    ulUEContext_p->spsUlInfo.ulResAllocInfo.allocatedMCS, 
                    ulUEContext_p->spsUlInfo.ulResAllocInfo.allocatedRB,
                    ulUEContext_p->userLocationType, rbMapNode_p,
                    rbMapNode_p->rbMap, internalCellIndex);
            /* SPR 8553 Starts*/
            if((( TTIB_UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                        ((UL_SPS_ACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType)
                         &&(ulUEContext_p->ttiBundlingEnabled ) ))&&
                    (MAX_TTI_BUNDLE_MCS < tempAllocatedMCS))
            {
                tempAllocatedMCS = MAX_TTI_BUNDLE_MCS;
            }
            /* SPR 8553 Ends*/
            *allocatedMCS_p = tempAllocatedMCS;
            /* - Coverity_31960 */
            *resourceReserved_p = ulResAllocInfo; 
            if(UL_SPS_DEACTIVATION != tempDirectIndexingUEInfo_p->ulGrantRequestType)
            {
                reserveAlreadyAvailableResForThreeTicks(ulResAllocInfo.allocatedIndex,
                        ulResAllocInfo.allocatedLength,
                        resourceReserved_p,currentTTI,
                        ulDelay,ulUEContext_p,
                        ulUEContext_p->userLocationType,
                        internalCellIndex);
            }
            retValue=MAC_SUCCESS; 
        }  
        else
        {
            /*SPR 8451 Changes Start*/
#ifdef ULCODERATE_PLUGIN
            checkULCodeRateUECategory(ulUEContext_p,&tempAllocatedMCS,
                    &(tempDirectIndexingUEInfo_p->requiredRB),BUDDY_UE_NOT_SET,internalCellIndex);
#endif
            *allocatedMCS_p = tempAllocatedMCS;
            /*SPR 8451 Changes End*/
            if(ulUEContext_p->ttiBundlingEnabled)
            {
                retValue = allocateUplinkResourcesForTTIBundle(
                        tempDirectIndexingUEInfo_p->requiredRB,
                        *minRbReq,
                        resourceReserved_p,
                        tempDirectIndexingUEInfo_p->strictRBAllocation,
                        &rbMapNode_p->availableResourceInfo[ulUEContext_p->userLocationType],
                        rbMapNode_p->rbMap
                        ,ulDelay,
                        currentGlobalTick,
                        ulUEContext_p,
                        internalCellIndex
                        );
            }
            else
            {
                retValue = allocateUplinkResources(
                        tempDirectIndexingUEInfo_p->requiredRB, 
                        *minRbReq,
                        resourceReserved_p,
                        tempDirectIndexingUEInfo_p->strictRBAllocation,
                        &rbMapNode_p->availableResourceInfo[ulUEContext_p->userLocationType],
                        /* SPR 21958 PUSCH RAT1 Support Start */
                        rbMapNode_p,
                        /* SPR 21958 PUSCH RAT1 Support End */
                        /*CA Changes start  */
                        internalCellIndex
                        /* SPR 21958 PUSCH RAT1 Support Start */
                        ,ulUEContext_p->isUplinkRAT1Supported);
                /* SPR 21958 PUSCH RAT1 Support End */
                /*CA Changes end  */
            }
        }
        /* SPS Changes end */
#elif TDD_CONFIG
        /* + SPS_TDD_Changes */
        if ( (UL_SPS_ACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                (UL_SPS_DEACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                (UL_SPS_OCCASION == tempDirectIndexingUEInfo_p->ulGrantRequestType)) 
        {
            if ( (UL_SPS_ACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) ||
                    (UL_SPS_DEACTIVATION == tempDirectIndexingUEInfo_p->ulGrantRequestType) )
            {
                ulSpsInfo_p = &ulUEContext_p->spsUlInfo;
                (ulSpsInfo_p->numDciAttempts++);
                LTE_MAC_UPDATE_DCI_ATTEMPT_STATS(ulUEContext_p->ueIndex, 
                        ulSpsInfo_p->numDciAttempts , internalCellIndex);
            }

            AllocatedResInfo ulResAllocInfo = {0};

            ulResAllocInfo.allocatedLength = ulUEContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;
            ulResAllocInfo.allocatedIndex = ulUEContext_p->spsUlInfo.ulResAllocInfo.startingVRBIndex;
            ulResAllocInfo.allocatedRiv = ulUEContext_p->spsUlInfo.ulResAllocInfo.allocatedRiv;

            /* + Coverity_31960 */
            MAC_UL_SPS_RESERVE_RESOURCES (ulResAllocInfo.allocatedIndex,
                    ulResAllocInfo.allocatedLength, &ulResAllocInfo,
                    &tempAllocatedMCS,
                    ulUEContext_p->spsUlInfo.ulResAllocInfo.allocatedMCS,
                    ulUEContext_p->spsUlInfo.ulResAllocInfo.allocatedRB,
                    ulUEContext_p->userLocationType, rbMapNode_p,
                    rbMapNode_p->rbMap, internalCellIndex);
            *allocatedMCS_p = tempAllocatedMCS;

            *resourceReserved_p = ulResAllocInfo; 

            retValue = allocateUplinkResources(
                    tempDirectIndexingUEInfo_p->requiredRB,
                    *minRbReq,
                    resourceReserved_p,
                    tempDirectIndexingUEInfo_p->strictRBAllocation,
                    /* ICIC changes start */
                    &rbMapNode_p->availableResourceInfo[ulUEContext_p->userLocationType],
                    /* SPR 21958 PUSCH RAT1 Support Start */
                    rbMapNode_p,
                    /* SPR 21958 PUSCH RAT1 Support End */
                    /* ICIC changes end */
                    internalCellIndex
                    /*TDD Config 0 Changes Start*/
                    ,nDmrs_p
                    ,phichSubFrameNum
                    ,iPhich
                    );
            /*TDD Config 0 Changes End*/
        }
        else
        {
            retValue = allocateUplinkResources(
                    tempDirectIndexingUEInfo_p->requiredRB,
                    *minRbReq,
                    resourceReserved_p,
                    tempDirectIndexingUEInfo_p->strictRBAllocation,
                    /* ICIC changes start */
                    &rbMapNode_p->availableResourceInfo[ulUEContext_p->userLocationType],
                    /* SPR 21958 PUSCH RAT1 Support Start */
                    rbMapNode_p,
                    /* SPR 21958 PUSCH RAT1 Support End */
                    /* ICIC changes end */
                    internalCellIndex
                    /*TDD Config 0 Changes Start*/
                    ,nDmrs_p
                    ,phichSubFrameNum
                    ,iPhich
                    );
            /*TDD Config 0 Changes End*/
        }
#endif
        /* - SPS_TDD_Changes */
    }
    return retValue; 
}

/********************************************************************************
 * Function Name :     checkQciPowerMaskAndUpdateRegionMask
 * Inputs        :     ulUEContext_p - Pointer to ULUEContext,
 *                     regionPowerMask - 
 * Outputs       :     None
 * Returns       :     None
 * Description   :     This Function will check QCI Power and update Region Mask.
 *********************************************************************************/
static  void checkQciPowerMaskAndUpdateRegionMask (
        ULUEContext * ulUEContext_p,
        SInt32      * regionPowerMask
        )
{
    SInt32 pcMax = 0;
    SInt16 count = 0;
    UInt8 lcgId = LCGID_INVALID_VALUE;
    pcMax = ulUEContext_p->pcMax;
    for( count  = ulUEContext_p->qciDeltaPowerMaskInfo.numOfQciInfo - 1;
            count >= 0 ; 
            count--)
    {
        lcgId =  ulUEContext_p->qciDeltaPowerMaskInfo.deltaPowerMaskInfo[count].lcgId;
        if ( lcgId != LCGID_INVALID_VALUE )
        {
            if(ulUEContext_p->bsrNet[lcgId] != 0)
            {
                /* for updating stats */
                ulUEContext_p->maxQciDeltaPowerMask = ulUEContext_p->qciDeltaPowerMaskInfo.
                    deltaPowerMaskInfo[count].maxQciDeltaPowerMask;
                *regionPowerMask += ulUEContext_p->qciDeltaPowerMaskInfo.
                    deltaPowerMaskInfo[count].maxQciDeltaPowerMask ;
                break;
            }
        }
    }
    if(pcMax < *regionPowerMask)
    {
        *regionPowerMask = pcMax;
    }
}

/**********************************************************************************
 * Function Name  :     MacUlUpdateLocalMaxTBIfRequired
 * Inputs         :     mcsIdx,
 *                      localMaxRBs,
 *                      localMaxMCS,
 *                      availableRBs
 *                      startIdx,
 *                      localMaxTBSizeRBStartIdx_p,
 *                      localTBSize_p,
 *                      localMaxTBSize_p
 * Outputs        :     None
 * Returns        :     None
 * Description    :     This Function will update MAC UL Local Maximum TB Required .
 ***********************************************************************************/
                    static  void  MacUlUpdateLocalMaxTBIfRequired(
                            UInt8  mcsIdx,
                            UInt8  *localMaxRBs_p,
                            UInt8  *localMaxMCS_p,
                            UInt8  availableRBs,
                            UInt8  startIdx,
                            UInt8  *localMaxTBSizeRBStartIdx_p,
                            UInt32 *localTBSize_p,
                            UInt32 *localMaxTBSize_p
                            /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                            ,UInt8  *localnDmrs_p
                            ,UInt8  nDmrs
#endif
                            /*TDD Config 0 Changes End*/ 
                            )
{
    if(availableRBs)
    {
        *localTBSize_p = determineTBSize((mcsIdx), (availableRBs),
                /* 4x4 DL MIMO CHG START */
                TB_MAPPED_TO_ONE_LAYER);
        /* 4x4 DL MIMO CHG END */
        if (*localMaxTBSize_p < *localTBSize_p)
        {
            *localMaxTBSize_p = *localTBSize_p;
            *localMaxRBs_p = availableRBs;
            *localMaxMCS_p = mcsIdx;
            *localMaxTBSizeRBStartIdx_p = startIdx;
            /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
            *localnDmrs_p = nDmrs;
#endif
            /*TDD Config 0 Changes End*/
        }
    }
}

/* + SPR 6864 */
/*************************************************************************************
 * Function Name :      checkAndRestoreUCIPduType
 * Inputs        :      ulUEContext_p - Pointer to ULUEContext,
 *                      ueInfo_p -
 *                      ulschUciRachInfo_p -
 *                      internalCellIndex - Cell-Index used at MAC
 * Outputs       :      None             
 * Returns       :      None
 * Description   :      This is a generic func to convert UCI pdu types to correct values
 *                      For now this only changes cqi+harq+x -> harq+x if required.
 ****************************************************************************************/
void checkAndRestoreUCIPduType(
        ULUEContext * ulUEContext_p, 
#ifdef FDD_CONFIG
        DirectIndexingUEInfo *ueInfo_p,
#elif TDD_CONFIG
        UeScheduledInfo *ueInfo_p,
        /* + SPR_14994 */
        UInt16 systemFrame, 
        /* - SPR_14994 */
#endif
        ULSchUciRachPoolInfo * ulschUciRachInfo_p,
        /* CSI_P2_PUSCH_CHANGES Start */
        UInt8                         subFrameNum,
        /* CSI_P2_PUSCH_CHANGES End */
        /*SPR 19504 fix start*/    
        UInt32 rbMapNum,
        InternalCellIndex internalCellIndex
        /*SPR 19504 fix end*/    
        )
{
    UInt16 ulschUciInfoIndex = ueInfo_p->ulschUciInfoIndex;
    ULSchUciInfo *ulSchUciInfo_p = &ulschUciRachInfo_p->ulschUciInfo[ulschUciInfoIndex];
    /* CSI_P2_PUSCH_CHANGES Start */
    /* + SPR_14994 */


    if(ulUEContext_p->ulUeScellContext_p[START_SCELL_INDEX]!= PNULL)
        {
            /* Bitmask cellIdxOfExpectedReportType will be set by PUCCH for
             * the cell for which report is expected */
#ifdef FDD_CONFIG        
            if((ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType
                        & servingCellBitMask_g[PCELL_SERV_CELL_IX])
                    && (!ulUEContext_p->pucchConfigInfo.simultaneousAckNackAndCQI))
#elif TDD_CONFIG            
                if((ulUEContext_p->reportFormatMap[systemFrame % 2][subFrameNum].cellIdxOfExpectedReportType
                            & servingCellBitMask_g[PCELL_SERV_CELL_IX])
                        && (!ulUEContext_p->pucchConfigInfo.simultaneousAckNackAndCQI))
#endif            
                {
                    /* for PCell */
                    dropCSIReportWithHarq(ulSchUciInfo_p);
                }
#ifdef FDD_CONFIG
            /*SPR 19649 +*/
            if((ulUEContext_p->reportFormatMap[subFrameNum].cellIdxOfExpectedReportType
                        & servingCellBitMask_g[START_SCELL_INDEX])
                    && (!ulUEContext_p->pucchConfigInfo.simultaneousAckNackAndCQI))
#elif TDD_CONFIG
                if((ulUEContext_p->reportFormatMap[systemFrame % 2][subFrameNum].cellIdxOfExpectedReportType
                            & servingCellBitMask_g[START_SCELL_INDEX])
                        && (!ulUEContext_p->pucchConfigInfo.simultaneousAckNackAndCQI))
                    /*SPR 19649 -*/
#endif
                {
                    /* for SCell */
                    dropCSIReportWithHarq(ulSchUciInfo_p);
                }
        }
    /* CSI_P2_PUSCH_CHANGES End */
        else
        {
            if (!ulUEContext_p->pucchConfigInfo.simultaneousAckNackAndCQI)
            {
                dropCSIReportWithHarq(ulSchUciInfo_p);
            }
        }
    /*SPR 19504 fix start*/    
#ifdef TDD_CONFIG    
    if(ulSchUciInfo_p->pduType == MAC_SS_PDU_ULSCH_HARQ)
    {    
        /* If no of UE per TTI is one and PUSCH allocation fails for UE
           then DL HARQ will be scheduled on PUCCH resource. As PUSCH 
           allocation fails for UE and only one UE can be scheduled
           due to 1 UE/TTI. So all UL resources are available for 
           PUCCH  allocation and PUCCH resource can be allocated blindly.

           If more than one UEs are configured to scheduled per TTI and 
           PUSCH allocation fails for particular UE then PUCCH resource
           can not be allocated blindly. As resources may be used by 
           other UEs for PUSCH transmission on this particular TTI.
           */  
        /*SPR 20145 fix start*/
        /*Code deleted*/
        /*SPR 20145 fix end*/
        {
            ulSchUciInfo_p->pduType=MAC_SS_PDU_INVALID;

        }    
    }        
#endif    
    /*SPR 19504 fix end*/
    /* - SPR_14994 */ 
}

/*************************************************************************************
 * Function Name     :  dropCSIReportWithHarq
 * Inputs            :  ulSchUciInfo_p - Pointer to ULSchUciInfo
 * Outputs           :  None
 * Returns           :  None
 * Description       :  This function only changes cqi+harq+x -> harq+x.                            
 ****************************************************************************************/
void dropCSIReportWithHarq(
        ULSchUciInfo *ulSchUciInfo_p)
{
    /* 
     * Its a pain to recalculate correct pucch
     * resource index for harq. Esp for TDD. Trust me. 
     * Also it might lead to pucch/pusch rb overlaps for eNB L1
     * The UE will anyway, send it on some other resource and 
     * if we're lucky, it would result in DL Retx only. 
     * Not UL retx of some other UE.
     */
    switch (ulSchUciInfo_p->pduType)
    {
        case MAC_SS_PDU_UCI_CQI_HARQ :
        ulSchUciInfo_p->pduType = MAC_SS_PDU_UCI_HARQ;
        break;
        case MAC_SS_PDU_UCI_CQI_SR_HARQ:
        ulSchUciInfo_p->pduType = MAC_SS_PDU_UCI_SR_HARQ;
        break;
        default:
        break;
    }
}
/* - SPR 6864 */
/*SPR_9204_START*/
/*************************************************************************************
 * Function Name     :  selectbuddyUeFSS
 * Inputs            :  secondUeIndex,secondUEIndexingInfo_p,pdcchULNewUEInfo_p,
 *                      firstUeIndex,ulUEContextFirst_p,firstUEIndexingInfo_p,minRbLoss,
 *                      currentTick,buddyPairInfo_p,internalCellIndex
 * Outputs           :  firstUEMinStrictReqRB,secondUEMinStrictReqRB,tempBuddyUeIndex
 * Returns           :  MAC_SUCCESS/MAC_FAILURE
 * Description       :  This function finds the buddy UE in FSS                           
 ****************************************************************************************/
                STATIC  MacRetType selectbuddyUeFSS(
                        UInt16                 secondUeIndex,
#ifdef FDD_CONFIG
                        DirectIndexingUEInfo   *secondUEIndexingInfo_p,       
#elif TDD_CONFIG
                        UeScheduledInfo        *secondUEIndexingInfo_p,      
#endif
                        PdcchULNewUEInfo       *pdcchULNewUEInfo_p,
                        UInt16                 *firstUeIndex,
                        ULUEContext            *ulUEContextFirst_p,
#ifdef FDD_CONFIG
                        DirectIndexingUEInfo   *firstUEIndexingInfo_p,
#elif TDD_CONFIG
                        UeScheduledInfo        *firstUEIndexingInfo_p,
#endif
                        UInt16                 *minRbLoss,
                        UInt32                 *firstUEMinStrictReqRB,
                        UInt32                 *secondUEMinStrictReqRB,
                        UInt16                 *tempBuddyUeIndex,
                        /* SPR 15909 fix start */
                        tickType_t                 currentTick,
                        /* SPR 15909 fix end */
                        BuddyPairInfo          **buddyPairInfo_p,
                        InternalCellIndex      internalCellIndex
#ifdef TDD_CONFIG        
                        ,UInt8                 ulSubFrameNumber
#endif
                        )
{
    ULUEContext                *ulUEContextSecond_p          = PNULL;
    UInt32 firstUEMinDataSize                                = 0;
    UInt32 secondUEMinDataSize                               = 0;
    UInt16 rbLoss                                            = MAX_NUM_RBS;
    UInt16 minRbReq                                          = 0;
    UInt16 firstUEMinReqRB                                   = 0;
    UInt16 secondUEMinReqRB                                  = 0;
    UInt16 maxBytesSupportByMinCategory                      = 0;
    UInt16 firstUECategoryBytesSupport                       = 0;
    UInt16 secondUECategoryBytesSupport                      = 0;
    UInt8 firstReferenceIndex                                = 0;
    UInt8 secondReferenceIndex                               = 0;
    UInt8 firstUEAvgMCS                                      = 0;
    UInt8 secondUEAvgMCS                                     = 0;
    UInt8 maxAvgMCS                                          = 0;

    ulUEContextSecond_p = ulUECtxInfoArr_g[secondUeIndex].ulUEContext_p;

    /*Get the reference index to fetch avgMCS and minDataSize*/
    firstReferenceIndex = firstUEIndexingInfo_p->uePosInPdcchULNewUEArray;
    secondReferenceIndex = secondUEIndexingInfo_p->uePosInPdcchULNewUEArray;

    firstUEMinDataSize  = pdcchULNewUEInfo_p->pdcchULNewUEArray[firstReferenceIndex].minDataSize;
    secondUEMinDataSize  = pdcchULNewUEInfo_p->pdcchULNewUEArray[secondReferenceIndex].minDataSize;

    /*To get the ReqRBs to cater both the UEs, first get the Data Size and
     * AvgMCS for both UEs*/
    firstUEAvgMCS = pdcchULNewUEInfo_p->pdcchULNewUEArray[firstReferenceIndex].avgMCS;
    secondUEAvgMCS = pdcchULNewUEInfo_p->pdcchULNewUEArray[secondReferenceIndex].avgMCS;

    maxAvgMCS = MAX_BETWEEN_TWO(firstUEAvgMCS, secondUEAvgMCS);

    if(maxAvgMCS == firstUEAvgMCS)
    {
        firstUEMinReqRB = firstUEIndexingInfo_p->requiredRB;
        secondUEMinReqRB = DETERMINE_RBs_UL(maxAvgMCS, secondUEMinDataSize, internalCellIndex);
    }
    else
    {
        firstUEMinReqRB = DETERMINE_RBs_UL(maxAvgMCS, firstUEMinDataSize, internalCellIndex);
        secondUEMinReqRB = secondUEIndexingInfo_p->requiredRB;
    }

    if(firstUEIndexingInfo_p->strictRBAllocation)
    {
        *firstUEMinStrictReqRB = firstUEIndexingInfo_p->requiredRB;
    }
    if(secondUEIndexingInfo_p->strictRBAllocation)
    {
        *secondUEMinStrictReqRB = secondUEIndexingInfo_p->requiredRB;
    }
    minRbReq = MAX_BETWEEN_TWO(firstUEMinReqRB, secondUEMinReqRB);

    /* Check the Power level (ATB) of each UE for sending max (reqdRB1st, reqdRB2nd) with min (avgMCS).
     * If  Power level (ATB) is not suitable then continue with other UE else proceed */
    /* Coverity 107247 Fix Start */
    if(((maxAvgMCS != firstUEAvgMCS) || (minRbReq != firstUEMinReqRB)) && (0 < minRbReq))
        /* Coverity 107247 Fix End */
    {
        if(MAC_SUCCESS != validateNprbFromATBULMUMIMO(minRbReq, maxAvgMCS,ulUEContextFirst_p,
                    currentTick,internalCellIndex
#ifdef TDD_CONFIG                          
                    ,ulSubFrameNumber
#endif                          
                    ))
        {
            return MAC_FAILURE;
        }
    }
    /* Coverity 107247 Fix Start */
    if(((maxAvgMCS != secondUEAvgMCS) || (minRbReq != secondUEMinReqRB)) && (0 < minRbReq))
        /* Coverity 107247 Fix End */
    {
        if(MAC_SUCCESS != validateNprbFromATBULMUMIMO (minRbReq, maxAvgMCS,ulUEContextSecond_p,
                    currentTick,internalCellIndex
#ifdef TDD_CONFIG                          
                    ,ulSubFrameNumber
#endif                          
                    ))
        {
            return MAC_FAILURE;
        }
    }


    *buddyPairInfo_p= buddyPairInfo_gp[internalCellIndex] + *firstUeIndex;

    (*buddyPairInfo_p)->maxOfMinDataSize = MAX_BETWEEN_TWO( firstUEMinDataSize, secondUEMinDataSize);


    /* Find maxbytes supported by each UE as per its category */
    /* SPR 19600 Changes start */
    firstUECategoryBytesSupport = ulUeCategoryInfo_g[ulUEContextFirst_p->ueCategory - 1].maxBytesULSCHTb;
    secondUECategoryBytesSupport = ulUeCategoryInfo_g[ulUEContextSecond_p->ueCategory - 1].maxBytesULSCHTb;
    /* SPR 19600 Changes end */
    maxBytesSupportByMinCategory = MIN_BETWEEN_TWO( firstUECategoryBytesSupport, 
            secondUECategoryBytesSupport);
    if ((*buddyPairInfo_p)->maxOfMinDataSize > maxBytesSupportByMinCategory)
    {
        return MAC_FAILURE;
    }

    /*For an efficient utilization there should be minimum RB Loss between both the UEs. So calculate rbLoss here*/
    rbLoss = ABS(firstUEMinReqRB - secondUEMinReqRB);
    /* updating the minRbLoss for each iteration*/
    if (*minRbLoss > rbLoss)
    {
        *minRbLoss = rbLoss;
        /*Each time in the loop update the secondUeIndex to tempBuddyUeIndex. And that will be considered as best pair
         * which has the least rbLoss*/
        *tempBuddyUeIndex = secondUeIndex;
    }
    else
    {
        LOG_MAC_MSG(MAC_BUDDY_UE_INFO_ID,LOGDEBUG,MAC_PUSCH,\
                getCurrentTick(),\
                *firstUeIndex,\
                secondUeIndex,\
                0,0,0,\
                0.0,\
                0.0,\
                __func__,"BuddySelectionFail");
    }

    return MAC_SUCCESS;
}

/*************************************************************************************
 * Function Name     :  selectbuddyUeNonFSS
 * Inputs            :  secondUeIndex,secondUEIndexingInfo_p,pdcchULNewUEInfo_p,
 *                      firstUeIndex,ulUEContextFirst_p,firstUEIndexingInfo_p,
 *                      minRbLoss,currentTick,buddyPairInfo_p 
 *                      internalCellIndex - Cell-Index used at MAC
 * Outputs           :  firstUEMinStrictReqRB,secondUEMinStrictReqRB,tempBuddyUeIndex
 * Returns           :  MAC_SUCCESS/MAC_FAILURE
 * Description       :  This function finds the buddy UE in NonFSS                           
 ****************************************************************************************/
        STATIC  MacRetType selectbuddyUeNonFSS(
                UInt16                 secondUeIndex,
#ifdef FDD_CONFIG
                DirectIndexingUEInfo   *secondUEIndexingInfo_p,       
#elif TDD_CONFIG
                UeScheduledInfo        *secondUEIndexingInfo_p,      
#endif
                PdcchULNewUEInfo       *pdcchULNewUEInfo_p,
                UInt16                 *firstUeIndex,
                ULUEContext            *ulUEContextFirst_p,
#ifdef FDD_CONFIG
                DirectIndexingUEInfo   *firstUEIndexingInfo_p,
#elif TDD_CONFIG
                UeScheduledInfo        *firstUEIndexingInfo_p,
#endif
                UInt16                 *minRbLoss,
                UInt32                 *firstUEMinStrictReqRB,
                UInt32                 *secondUEMinStrictReqRB,
                UInt16                 *tempBuddyUeIndex,
                /* SPR 15909 fix start */
                tickType_t                 currentTick,
                /* SPR 15909 fix end */
                BuddyPairInfo         * *buddyPairInfo_p,
                InternalCellIndex      internalCellIndex
#ifdef TDD_CONFIG        
                ,UInt8                  ulSubFrameNumber
#endif        
                )
{
    ULUEContext                *ulUEContextSecond_p          = PNULL;
    UInt16 rbLoss                                            = MAX_NUM_RBS;
    UInt16 minRbReq                                          = 0;
    UInt32 firstUECategoryBytesSupport                       = 0;
    UInt32 secondUECategoryBytesSupport                      = 0;
    UInt8 firstReferenceIndex                                = 0;
    UInt8 secondReferenceIndex                               = 0;
    UInt8 firstUEAvgMCS                                      = 0;
    UInt8 secondUEAvgMCS                                     = 0;
    UInt8 firstUEMaxSupportedRB                              = 0;
    UInt8 secondUEMaxSupportedRB                             = 0;
    UInt8 maxSupportedRB                                     = 0;

    ulUEContextSecond_p = ulUECtxInfoArr_g[secondUeIndex].ulUEContext_p;

    /*Get the reference index to fetch avgMCS*/
    firstReferenceIndex = firstUEIndexingInfo_p->uePosInPdcchULNewUEArray;
    secondReferenceIndex = secondUEIndexingInfo_p->uePosInPdcchULNewUEArray;


    firstUEAvgMCS = pdcchULNewUEInfo_p->pdcchULNewUEArray[firstReferenceIndex].avgMCS;
    secondUEAvgMCS = pdcchULNewUEInfo_p->pdcchULNewUEArray[secondReferenceIndex].avgMCS;

    minRbReq = MAX_BETWEEN_TWO(firstUEIndexingInfo_p->requiredRB,secondUEIndexingInfo_p->requiredRB);

    if(firstUEIndexingInfo_p->strictRBAllocation)
    {
        *firstUEMinStrictReqRB = firstUEIndexingInfo_p->requiredRB;
    }
    if(secondUEIndexingInfo_p->strictRBAllocation)
    {
        *secondUEMinStrictReqRB = secondUEIndexingInfo_p->requiredRB;
    }

    /* Check the Power level (ATB) of each UE for sending max (reqdRB1st, reqdRB2nd) with avgMCS.
     * If  Power level (ATB) is not suitable then continue with other UE else proceed */

    if (firstUEIndexingInfo_p->requiredRB !=  minRbReq)
    {
        if(MAC_SUCCESS != validateNprbFromATBULMUMIMO(minRbReq,firstUEAvgMCS,ulUEContextFirst_p,
                    currentTick,internalCellIndex
#ifdef TDD_CONFIG                         
                    ,ulSubFrameNumber
#endif                         
                    ))
        {
            return MAC_FAILURE;
        }
    }
    if (secondUEIndexingInfo_p->requiredRB !=  minRbReq)
    {
        if(MAC_SUCCESS != validateNprbFromATBULMUMIMO(minRbReq,secondUEAvgMCS,ulUEContextSecond_p,
                    currentTick,internalCellIndex
#ifdef TDD_CONFIG                          
                    ,ulSubFrameNumber
#endif                          
                    ))
        { 
            return MAC_FAILURE;
        }
    }
    *buddyPairInfo_p = buddyPairInfo_gp[internalCellIndex] + *firstUeIndex;

    /* Find maxbytes supported by each UE as per its category */
    /* SPR 19600 Changes start */
    firstUECategoryBytesSupport = ulUeCategoryInfo_g[ulUEContextFirst_p->ueCategory - 1].maxBytesULSCHTb;
    secondUECategoryBytesSupport = ulUeCategoryInfo_g[ulUEContextSecond_p->ueCategory - 1].maxBytesULSCHTb;
    /* SPR 19600 Changes end */
    firstUEMaxSupportedRB = DETERMINE_RBs_UL(firstUEAvgMCS,firstUECategoryBytesSupport,internalCellIndex);
    secondUEMaxSupportedRB = DETERMINE_RBs_UL(secondUEAvgMCS,secondUECategoryBytesSupport,internalCellIndex);
    maxSupportedRB = MIN_BETWEEN_TWO(firstUEMaxSupportedRB,
            secondUEMaxSupportedRB);
    if (minRbReq > maxSupportedRB)
    {
        return MAC_FAILURE;
    }

    /*For an efficient utilization there should be minimum RB Loss between both the UEs. So calculate rbLoss here*/
    rbLoss = ABS(firstUEIndexingInfo_p->requiredRB - secondUEIndexingInfo_p->requiredRB);
    /* updating the minRbLoss for each iteration of loop */
    if (*minRbLoss > rbLoss)
    {
        *minRbLoss = rbLoss;
        /*Each time in the loop update the secondUeIndex to tempBuddyUeIndex. And that will be considered as best pair
         * which has the least rbLoss*/
        *tempBuddyUeIndex = secondUeIndex;
    }
    else
    {
        LOG_MAC_MSG(MAC_BUDDY_UE_INFO_ID,LOGDEBUG,MAC_PUSCH,\
                getCurrentTick(),\
                *firstUeIndex,\
                secondUeIndex,\
                0,0,0,\
                0.0,\
                0.0,\
                __func__,"BuddySelectionFail");
    }

    return MAC_SUCCESS;
}
/*************************************************************************************
 * Function Name     :checkAvailabilityOfHigherMCS
 * Inputs            :mcsIdx,startIndx,requiredRBs,ulUEContext_p,buddyUlUEContext_p,
 *                    buddyPairInfo_p,currentTTI,
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs           :UE and buddy MCS Idx
 * Returns           :nothing
 * Description       :This function finds if resources available at a given mcsIndex are also 
 *                    available at a higher mcsIndex
 ****************************************************************************************/
        STATIC  void checkAvailabilityOfHigherMCS(
                UInt8                  *mcsIdx,
                UInt8                  startIndx,
                UInt8                  requiredRBs,
                ULUEContext*           ulUEContext_p,
                ULUEContext*           buddyUlUEContext_p,
                BuddyPairInfo          *buddyPairInfo_p,
                /* SPR 15909 fix start */
                tickType_t                 currentTTI,
                /* SPR 15909 fix end */
                InternalCellIndex      internalCellIndex
#ifdef TDD_CONFIG        
                ,UInt8                  ulSubFrameNumber
#endif        
                )
{
    MCSToPrbMapInfo *mcsToPrbMap_p = PNULL ;
    MCSToPrbMapInfo *buddyMcsToPrbMap_p = PNULL ;
    UInt8 index = 0;
    UInt8 higherMCS = 0;
    UInt8 ueMCSCalculationIdx = TRUE;
    UInt8 buddyMCSCalculationIdx = TRUE;

    CSIContext      *csiContext_p       = &(ulUEContext_p->csiContext);
    CSIContext      *buddyCsiContext_p  = &(buddyUlUEContext_p->csiContext);
    mcsToPrbMap_p = csiContext_p->mcsToPrbMap_p;
    buddyMcsToPrbMap_p = buddyCsiContext_p->mcsToPrbMap_p;

    buddyPairInfo_p->buddyAvgMCS = *mcsIdx;
    UInt8 commonMCS = *mcsIdx;

    higherMCS = MAX_BETWEEN_TWO(csiContext_p->highestReportedMCS,buddyCsiContext_p->highestReportedMCS);

    /* Try to allocate better MCS for the UEs at the required RB set */
    for (index = commonMCS + 1; 
            ((index <= higherMCS) && (index != INVALID_MCS_INDEX) &&
             (ueMCSCalculationIdx || buddyMCSCalculationIdx)); 
            index = mcsGroupingTable_g[index].nextMCS)
    {
        if (ueMCSCalculationIdx && index <= csiContext_p->highestReportedMCS)
        {
            /* If required RBs are not contiguously present at a lower MCS, then
             * they shall not be available at a higher MCS */
            if (mcsToPrbMap_p[index].maxContiguous < requiredRBs)
            {
                ueMCSCalculationIdx = FALSE;
            }
            else
            {
                if (mcsToPrbMap_p[index].prbMap[startIndx] >= requiredRBs)
                {
                    /* Validate the power  level for required RBs and mcsIndex */
                    if(MAC_SUCCESS != validateNprbFromATBULMUMIMO(requiredRBs,index,ulUEContext_p,
                                currentTTI,internalCellIndex
#ifdef TDD_CONFIG                                      
                                ,ulSubFrameNumber
#endif                                      
                                ))
                    {
                        ueMCSCalculationIdx = FALSE;
                    }
                    else
                    {
                        *mcsIdx = index;
                    }
                }
            }
        }
        if (buddyMCSCalculationIdx && index <= buddyCsiContext_p->highestReportedMCS)
        {
            /* If required RBs are not contiguously present at a lower MCS, then
             *they shall not be available at a higher MCS */
            if (buddyMcsToPrbMap_p[index].maxContiguous < requiredRBs)
            {
                buddyMCSCalculationIdx = FALSE;
            }
            else
            {
                if (buddyMcsToPrbMap_p[index].prbMap[startIndx] >= requiredRBs)
                {
                    /* Validate the power level for required RBs and mcsIndex*/
                    if(MAC_SUCCESS != validateNprbFromATBULMUMIMO(requiredRBs,index,buddyUlUEContext_p,
                                currentTTI,internalCellIndex
#ifdef TDD_CONFIG                    
                                ,ulSubFrameNumber
#endif                    
                                ))
                    {
                        buddyMCSCalculationIdx = FALSE;
                    }
                    else
                    {
                        buddyPairInfo_p->buddyAvgMCS = index;
                    }
                }
            }
        }
    }
}
/*SPR_9204_END*/
/*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
/*********************************************************************************
 * Function Name : populateUlIndexForTdd
 * Inputs        : dciCCEInfo_p,
 *                 ulSubFrameOffSet,
 *                 subframe,
 *                 ulSubFrameNum
 * Outputs       : None
 * Description   : This function will populate the ulIndex in DciCCEInfo for TDD.
 *                 For TDD Config 0: If UL Tx is to happen in n+7 then set LSB.
 *                 If UL Tx is to happen in n+k then set MSB.
 *                 Set both LSB and MSB if UL TX is to hapeen in both =k and +7
 *                 corresponding to Single DCI0.
 *                 For TDD Config 6:The value of ulIndex is 0
 *                                         
 ****************************************************************************/
STATIC void populateUlIndexForTdd(
        DciCCEInfo* tempDciCCEInfo_p,
        UInt8 ulSubFrameNum,
        UInt8 subframe,
        InternalCellIndex internalCellIndex
        )
{
    UInt8 ulSubFrameOffSet = 0;
    if (UL_DL_CONFIG_0 == getTddConfig(internalCellIndex))
    {
        GET_UL_SUBFRAME_OFFSET(ulSubFrameOffSet,subframe,ulSubFrameNum);
        /* If offset between DCI0 and ul Tx is 7,then ul Index is 1
         * If offset is k, then ulIndex is 2
         * If offset is 7 and k, ulIndex is 3*/
        if (ULTX_OFFSET == ulSubFrameOffSet)
        {
            tempDciCCEInfo_p->dciPduInfo.ulIndex |= SET_LSB;
        }
        else
        {
            tempDciCCEInfo_p->dciPduInfo.ulIndex |= SET_MSB;
        }

    }
    else /*UlIndex for other TDD configs is 0*/
    {
        tempDciCCEInfo_p->dciPduInfo.ulIndex = 0;
    }
}
/*********************************************************************************
 * Function Name : calculateIPhichValue
 * Inputs        : ulSubFrameNum
 * Outputs       : iPhich
 * Returns       : Nothing
 * Description   : This function will return the iPhich value corresponding to
 * the Ul frame.
 * ********************************************************************************/
UInt8 calculateIPhichValue(UInt8 ulSubFrameNum, InternalCellIndex internalCellIndex)
{
    UInt8 iPhich = 0;
    /* Value of iPhich is 1 only in case of TDD config 0 when Ul Tx takes 
     * place in subframe 4 or 9*/
    if ((UL_DL_CONFIG_0 == getTddConfig(internalCellIndex)) &&
            ((SUB_FRAME_4 == ulSubFrameNum) || (SUB_FRAME_9 == ulSubFrameNum)))
    {
        iPhich = 1;
    }
    else
    {
        iPhich = 0;
    }
    return iPhich;
}

/*********************************************************************************
 * Function Name : reserveHiAndUpdateHarqInfo
 * Inputs        : iPhich
 *                 riStart
 *                 nDmrs_p
 *                 phichSubFrameNum
 *                 tempULHarqInfo_p
 * Outputs       : tempULHarqInfo_p
 * Returns       : Nothing
 * Description   : This function will reserve the Hi Resources and update the
 * HarqInfo for the same
 * ********************************************************************************/
void reserveHiAndUpdateHarqInfo(UInt8 iPhich,
        UInt8 riStart,
        UInt8 nDmrs,
        UInt8 phichSubFrameNum,
        ULHarqInfo* tempULHarqInfo_p,
        InternalCellIndex      internalCellIndex)
{
    UInt8 nPhichGrp = 0;
    UInt8 nPhichSeq = 0;
    /*Calculate the phichgrp and phichseq for the given ristart and block Hi
     *resources*/
    nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][riStart][nDmrs];
    nPhichSeq = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][riStart][nDmrs];
    phichSequenceNoPerPhichGroup_g[internalCellIndex][phichSubFrameNum][nPhichGrp] &=
        ~(ONE << nPhichSeq);
    /*Update harq Info*/
    tempULHarqInfo_p->nDmrs = nDmrs;
    tempULHarqInfo_p->nPhichGrp = nPhichGrp;
    tempULHarqInfo_p->nPhichSeq  = nPhichSeq;
    tempULHarqInfo_p->sfUsed     = phichSubFrameNum;
}

/*********************************************************************************
 * Function Name : getPrevHarqInfoForTddConfig0
 * Inputs        : hiIndex
 *                 sysFrameNum
 *                 ulUEContext_p
 *                 ulHarqInfoForPreviousUlSubframe_p
 *
 * Outputs       : ulHarqInfoForPreviousUlSubframe_p
 * Returns       : Nothing
 * Description   : This function will get the harq Info for n+k in case of Tdd
 * config 0
 * ********************************************************************************/
STATIC  ULHarqInfo* getPrevHarqInfoForTddConfig0(UInt8 hiIndex,
        UInt16 sysFrameNum,
        ULUEContext* ulUEContext_p,
        InternalCellIndex internalCellIndex
        )
{
    UInt8 ulSubframeNumForOffsetK = 0;
    UInt16 ulSysFrameNumForOffsetK = 0;
    UInt8 harqProcessIdForPreviousUlSubframe = 0;
    ULHarqInfo*  ulHarqInfoForPreviousUlSubframe_p = PNULL;

    if(sysFrameNum > MAX_SYS_FRAME_NUM || hiIndex > MAX_SUB_FRAME)
    {
        lteWarning ("In function [%s] , Invalid sysFrameNum[%d] and hiIndex[%d] is received ",\
                __func__, sysFrameNum, hiIndex);
        ulHarqInfoForPreviousUlSubframe_p = PNULL;
        return ulHarqInfoForPreviousUlSubframe_p;
    }

    ulSubframeNumForOffsetK = getULGrantSubframeNum(hiIndex,
            GET_ULGRANT_SFN_FOR_DWNLINK_DATA_AND_DCI0,internalCellIndex);

    if (ulSubframeNumForOffsetK < hiIndex)
    {
        ulSysFrameNumForOffsetK = MODULO_ONEZEROTWOFOUR((sysFrameNum + 1));
    }
    else
    {
        ulSysFrameNumForOffsetK = sysFrameNum;
    }

    harqProcessIdForPreviousUlSubframe = getULHarqIDFromSubFrame(
            ulSubframeNumForOffsetK,
            ulUEContext_p,
            internalCellIndex,
            ulSysFrameNumForOffsetK );

    ulHarqInfoForPreviousUlSubframe_p  = &(ulUEContext_p->
            ulresInfo[harqProcessIdForPreviousUlSubframe]);
    return ulHarqInfoForPreviousUlSubframe_p;
}
#endif
/*TDD Config 0 Changes End*/
/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
/*************************************************************************************
 * Function Name      : calcBinomialCoef
 * Inputs             : tempVar1-variable 1
 *                    : tempVar2-variable 2
 * Outputs            : Binomial coefficient
 * Returns            : Binomial coefficient of two variables
 * Description        : Calculates binomial coefficient of two variables
 ****************************************************************************************/
UInt16 calcBinomialCoef(UInt8 tempVar1,UInt8 tempVar2)
{
    UInt16 binoCoeff = 1;
    UInt8  loopCounter = 0;
    UInt8  min = 0;
    if((tempVar2 > tempVar1))
    {
        return 0;
    }
    else if((0 == tempVar2)||(tempVar2 == tempVar1))
    {
        return 1;
    }
    else
    {
        if(tempVar1 <= tempVar2)
        {    
            min = tempVar1;
        }
        else
        {    
            min = tempVar2;
        } 
        for(loopCounter = 0;loopCounter < min; loopCounter++)
        {
            binoCoeff = (binoCoeff * (tempVar1 - loopCounter)) / (loopCounter + 1);

        }
        return binoCoeff;
    }
}

/*************************************************************************************
 * Function Name     : calculateCombiIndex
 * Inputs            : s0-start RBG index of Set 1
 *                   : s1-end RBG index of Set 1
 *                   : s2-start RBG index of Set 2
 *                   : s3-end RBG index of Set 32
 *                   : internalCellIndex
 * Outputs         	 : Combinatorial Index corresponding to set1 and set2
 * Returns        	 : None
 * Description       : Fetch the combinatorial Index
 ****************************************************************************************/
UInt16 calculateCombiIndex(UInt8 s0,UInt8 s1,UInt8 s2,UInt8 s3,
        InternalCellIndex internalCellIndex)
{
    UInt8 rbgIndex[MAX_RBG_SUBSET_RAT1] = {0};
    UInt8 M = NUM_SUBBAND_INDICES;
    UInt8 N = numRBGConfigured_g[internalCellIndex] + 1;
    UInt16 combiIndex = 0;
    UInt8 tempVar1 = 0;
    UInt8 tempVar2 = 0;
    UInt8 loopCounter = 0;

    rbgIndex[0] = s0;
    rbgIndex[1] = s1;
    rbgIndex[2] = s2;
    rbgIndex[3] = s3;

    if((s0 >= s1)||(s1 >= s2)||(s2 >= s3))
    {
        return MAC_FAILURE; /*s0>=s1;s1>=s2;s2>=s3;*/
    }

    for(loopCounter = 0 ;loopCounter < M ;loopCounter++)
    {
        tempVar1 = N - rbgIndex[loopCounter];
        tempVar2 = M - loopCounter;
        combiIndex +=  calcBinomialCoef(tempVar1,tempVar2);
    }
    return combiIndex;	
}

/*************************************************************************************
 * Function Name     : populateCombinatorialIndexTable
 * Inputs            : None 
 * Outputs           : Combinatorial Index corresponding to set1 and set2
 * Returns           : None
 * Description       : Calculation of combinatorial Index at Initialization Time
 ****************************************************************************************/
void populateCombinatorialIndexTable(InternalCellIndex internalCellIndex)
{
    UInt8 s0 = ZERO;	
    UInt8 s1 = ZERO;	
    UInt8 s2 = ZERO;	
    UInt8 s3 = ZERO;	

    for ( s0 = NUMONE; s0 < numRBGConfigured_g[internalCellIndex];s0++ )
    {
        for ( s1 = s0 + NUMONE; s1< numRBGConfigured_g[internalCellIndex];s1++)
        {
            for ( s2 = s1 + NUMONE; s2<= numRBGConfigured_g[internalCellIndex];s2++)
            {
                for( s3 = s2 + NUMONE; s3<= numRBGConfigured_g[internalCellIndex]+1;s3++)
                {
                    combiIndexTable[s0][s1][s2][s3] =  calculateCombiIndex(s0,s1,s2,s3,internalCellIndex);
                }
            }
        }
    }  
}
/*************************************************************************************
 * Function Name     : getCombinatorialIndex
 * Inputs            : s0
 *                   : s1
 *                   : s2
 *                   : s3
 * Outputs           : Combinatorial Index corresponding to set1 and set2
 * Returns           : combinatorial Index
 * Description       : Fetch the combinatorial Index
 ****************************************************************************************/
UInt16 getCombinatorialIndex(UInt8 s0,UInt8 s1,UInt8 s2,UInt8 s3)
{
    return combiIndexTable[s0+1][s1+1][s2+1][s3+1];
}

/*************************************************************************************
 * Function Name     : setUplinkResourcesAsBusy
 * Inputs            : rbMap --RB Map for RAT0 
 *                   : s0    --Start RB Index for RAT0, Start RBG Index for RAT1
 *                   : s1    --End RB in case of RAT0, s1 for RAT1 
 *                   : s2    --s2 for RAT1 
 *                   : s3    --s3 for RAT1 
 *                   : allocationType
 * Outputs           : None
 * Returns           : None
 * Description       : This Function updates the RB and RBG MAP when allocation is done
 ****************************************************************************************/
UInt8 setUplinkResourcesAsBusy(RbMapNode *rbMapNode_p, /*RB Map for RAT0 */
        UInt8 s0, /*Start RB Index for RAT0, Start RBG Index for RAT1 */
        UInt8 s1, /*End RB in case of RAT0, s1 for RAT1 */
        UInt8 s2, /* s2 for RAT1 */
        UInt8 s3, /* s3 for RAT1 */
        UInt8 allocationType, /* 0/1 indicating RAT0/RAT1 */
        InternalCellIndex internalCellIndex)
{
    UInt8 loopCounter = 0;
    UInt8 tempVar =0;
    UInt8 tempVar1 = 0; 
    UInt8 tempVar2 = 0;
    UInt8 tempVar3 = 0;
    UInt8 totalfree = 0;
    UInt8 numRbsAllocated = 0;

    totalfree = (rbMapNode_p->rat1bitmap & BITMASK_27BITS)>>27; 

    if(!allocationType) /*RAT 0 Allocation */
    {
        if((BARRED_RESOURCE != rbMapNode_p->rbgMapForRAT1[ s0/rbgSize_g[internalCellIndex ]])
                &&(FREE_RESOURCE == rbMapNode_p->rbgMapForRAT1[s0/rbgSize_g[internalCellIndex]]))
        {
            rbMapNode_p->rbgMapForRAT1[s0/rbgSize_g[internalCellIndex]] = BUSY_RESOURCE;
            rbMapNode_p->rat1bitmap |= (1 << (s0/rbgSize_g[internalCellIndex]));
            if(totalfree)
                totalfree--;
        }
        for ( loopCounter = s0 ; loopCounter < s1; loopCounter++)
        {
            if(!rbMapNode_p->rbMap[loopCounter])
            {
                numRbsAllocated++;
            }
            tempVar = s1 - loopCounter;
            rbMapNode_p->rbMap[loopCounter] =  tempVar; /*Marking RAT0 resources BUSY */
            if(!(loopCounter % rbgSize_g[internalCellIndex]))
            {
                tempVar = ceil_wrapper(tempVar/(double)rbgSize_g[internalCellIndex]);
                tempVar3 = loopCounter/rbgSize_g[internalCellIndex];
                if((BARRED_RESOURCE !=rbMapNode_p->rbgMapForRAT1[tempVar3])&&
                        ((BUSY_RESOURCE !=rbMapNode_p->rbgMapForRAT1[tempVar3])))
                {
                    rbMapNode_p->rbgMapForRAT1[tempVar3] = tempVar;
                    rbMapNode_p->rat1bitmap |= (1 << tempVar3);

                    if(totalfree)
                        totalfree--;
                }
            }
        }
        rbMapNode_p->rat1bitmap = rbMapNode_p->rat1bitmap & REFRESHBITMASK_27BITS;
        rbMapNode_p->rat1bitmap |= totalfree<<27;

    }
    else   /*RAT 1 Allocation */
    {
        tempVar1 = (s0 * rbgSize_g[internalCellIndex]);
        tempVar2 = (s1 * rbgSize_g[internalCellIndex]);
        tempVar = s0;
        for ( loopCounter = tempVar1 ; loopCounter < tempVar2; loopCounter++)
        {
            if(!rbMapNode_p->rbMap[loopCounter])
            {
                numRbsAllocated++;
            }
            rbMapNode_p->rbMap[loopCounter] = tempVar2 - loopCounter ; /*Marking RAT0 resources BUSY */
            if(!(loopCounter % rbgSize_g[internalCellIndex]))
            {
                if(BARRED_RESOURCE !=rbMapNode_p->rbgMapForRAT1[tempVar])
                {  
                    rbMapNode_p->rbgMapForRAT1[tempVar] = s1 - tempVar;
                    rbMapNode_p->rat1bitmap |= (1 << tempVar);
                    if(totalfree)
                        totalfree--;
                }
                tempVar++;
            }
        }
        tempVar1 = (s2 * rbgSize_g[internalCellIndex]);
        tempVar2 = (s3 * rbgSize_g[internalCellIndex]);
        tempVar = s2;
        for ( loopCounter = tempVar1 ; loopCounter < tempVar2; loopCounter++)
        {
            if(!rbMapNode_p->rbMap[loopCounter])
            {
                numRbsAllocated++;
            }
            rbMapNode_p->rbMap[loopCounter] = tempVar2 - loopCounter ; /*Marking RAT0 resources BUSY */
            if(!(loopCounter % rbgSize_g[internalCellIndex]))
            {
                if(BARRED_RESOURCE !=rbMapNode_p->rbgMapForRAT1[tempVar])
                {
                    rbMapNode_p->rbgMapForRAT1[tempVar] = s3 - tempVar;
                    rbMapNode_p->rat1bitmap |= (1 << tempVar);
                    if(totalfree)
                        totalfree--;
                }
                tempVar++;
            }
        }
        rbMapNode_p->rat1bitmap = rbMapNode_p->rat1bitmap & REFRESHBITMASK_27BITS;
        rbMapNode_p->rat1bitmap |= totalfree<<27;
    }
    return numRbsAllocated;
}

/*************************************************************************************
 * Function Name     : allocateResourceForUEThroughRAT1
 * Inputs            : numOfResourcesReq 
 *                   : minReqdRBs 
 *                   : allocatedResInfo_p
 *                   : strictAllocatedFlag
 *                   : availRes_p
 *                   : rbMapNode_p
 * Outputs        	 : None
 * Returns           : MacRetType
 * Description       : This Function allocates resources for UEs using RAT1 policy
 ****************************************************************************************/
MacRetType allocateResourceForUEThroughRAT1( UInt32 numOfResourcesReq,
        UInt32 minReqdRBs,
        AllocatedResInfo *allocatedResInfo_p,
        UInt32 strictAllocatedFlag,
        AvailableResInfo *availRes_p,
        RbMapNode *rbMapNode_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 s0 = 0;
    UInt8 s2 = 0;
    UInt8 s2Size = 0;
    UInt8 rbgNeed = 0;
    UInt8 arrIdx = 0;
    UInt8 freecnt = 0;
    UInt8 loop = 0;
    UInt8 lastBit = 0;
    RBGSet rbgarr[MAX_RBG_INDEX] = {{0,0}};
    UInt8 localBitmap = 0;	
    UInt8 actualSetsFound = 0;
    /* SES-491 fix Start */
    UInt8 sumRes = 0;
    UInt8 adjRes = 0;
    /* SES-491 fix End */


    freecnt = (rbMapNode_p->rat1bitmap & BITMASK_27BITS)>>27;

    if(ZERO == freecnt)
    {
        return MAC_FAILURE;    
    }
    else
    {
        for(loop = 0;loop < NUMTHREE;loop++)
        {
            localBitmap = (rbMapNode_p->rat1bitmap >> (loop*NUMEIGHT))&FIRST8;
            getAvailableFreeSets(rbgarr,localBitmap, &arrIdx, &lastBit,(loop * NUMEIGHT),rbMapNode_p,&actualSetsFound);
        }
        if(!((rbMapNode_p->rat1bitmap >>(loop * NUMEIGHT))& NUMONE))
        {
            if(lastBit)
            {
                rbgarr[arrIdx].size++;
            }
            else
            {
                rbgarr[arrIdx].size++;
                rbgarr[arrIdx].startRBG = (loop * NUMEIGHT);
                actualSetsFound++;
            }
        }

    }

    rbgNeed = ceil_wrapper(numOfResourcesReq / (double)rbgSize_g[internalCellIndex]);
    getRbgIndexes(rbgarr,actualSetsFound, &s0, &s2, &s2Size, rbgNeed);

    if((strictAllocatedFlag) && (numOfResourcesReq > (rbgSize_g[internalCellIndex] * (rbgarr[s0].size + s2Size))))
        return MAC_FAILURE;

    /* SES-491 fix Start */
    sumRes = rbgarr[s0].size + s2Size;
    adjRes = minpowerOfTwoThreeFive_g[sumRes];
    if (adjRes!=sumRes)
    {
        if ( ((sumRes - adjRes) < s2Size) && ((sumRes - adjRes) < rbgarr[s0].size))
        {
            s2Size = s2Size - (sumRes - adjRes);
        }
        else
        {
            return MAC_FAILURE;

        }
    }
    /* SES-491 fix End */

    allocatedResInfo_p->startRbgIndexforSet1 = rbgarr[s0].startRBG;
    allocatedResInfo_p->endRbgIndexforSet1 = allocatedResInfo_p->startRbgIndexforSet1 + rbgarr[s0].size;
    allocatedResInfo_p->startRbgIndexforSet2 = rbgarr[s2].startRBG;
    allocatedResInfo_p->endRbgIndexforSet2 = allocatedResInfo_p->startRbgIndexforSet2 + s2Size;

    /* SPR 22260 Fix End */
    if (((allocatedResInfo_p->endRbgIndexforSet1 - allocatedResInfo_p->startRbgIndexforSet1) +
                (allocatedResInfo_p->endRbgIndexforSet2 - allocatedResInfo_p->startRbgIndexforSet2)) == 1)
    {
        return MAC_FAILURE;
    }    
    /* SPR 22260 Fix End */

    if ((allocatedResInfo_p->startRbgIndexforSet1 == allocatedResInfo_p->endRbgIndexforSet1) 
            || ( allocatedResInfo_p->startRbgIndexforSet2 == allocatedResInfo_p->endRbgIndexforSet2) )
    {
        LOG_MAC_WARNING(MAC_PUSCH,"Incorrect values for startRbgIndexforSet1[%d] endRbgIndexforSet1[%d] startRbgIndexforSet2[%d] endRbgIndexforSet2[%d] received at Tick[%llu]\n",
                allocatedResInfo_p->startRbgIndexforSet1, \
                allocatedResInfo_p->endRbgIndexforSet1, \
                allocatedResInfo_p->startRbgIndexforSet2, \
                allocatedResInfo_p->endRbgIndexforSet2, \
                getCurrentTick());
    }    
    allocatedResInfo_p->totalRBsAllocatedForPUSCHRAT1 = rbgSize_g[internalCellIndex] *(rbgarr[s0].size + s2Size);
    allocatedResInfo_p->rbCodingForPUSCHRAT1 = 
        getCombinatorialIndex(allocatedResInfo_p->startRbgIndexforSet1,
                allocatedResInfo_p->endRbgIndexforSet1,
                allocatedResInfo_p->startRbgIndexforSet2,
                allocatedResInfo_p->endRbgIndexforSet2);

    allocatedResInfo_p->allocatedLength = allocatedResInfo_p->totalRBsAllocatedForPUSCHRAT1;
    allocatedResInfo_p->allocatedIndex = allocatedResInfo_p->startRbgIndexforSet1 * rbgSize_g[internalCellIndex];


    if(availRes_p->freeRBCounter > allocatedResInfo_p->totalRBsAllocatedForPUSCHRAT1)
    {
        availRes_p->freeRBCounter -= allocatedResInfo_p->totalRBsAllocatedForPUSCHRAT1;
    }
    else
    {
        availRes_p->freeRBCounter =0;
    }
    if((ZERO == s0)&&(availRes_p->freeRBIndex == allocatedResInfo_p->allocatedIndex))
    {
        if(BARRED_RESOURCE == rbMapNode_p->rbgMapForRAT1[allocatedResInfo_p->endRbgIndexforSet1])
        {
            availRes_p->freeRBIndex = rbgSize_g[internalCellIndex] * allocatedResInfo_p->endRbgIndexforSet1;
        }
        else
        {
            availRes_p->freeRBIndex = rbgSize_g[internalCellIndex] * (rbgarr[1].startRBG - 1);
        }
    }

    if(availRes_p->endIndex == (rbgSize_g[internalCellIndex] * (allocatedResInfo_p->endRbgIndexforSet2 -1)))
    {
        availRes_p->endIndex = (rbgSize_g[internalCellIndex] * allocatedResInfo_p->startRbgIndexforSet2) -1;
    }

    return MAC_SUCCESS;
}

/***********************************************************************
 * Function Name     : getAvailableFreeSets
 * Inputs            : rbgarr 
 *                   : bitmap 
 *                   : arrIdx
 *                   : lastBit
 *                   : startRBGId
 *                   : rbMapNode_p
 * Outputs     	     : None
 * Returns           : MacRetType
 * Description       : This Function retrieves free set of Rbs
 ************************************************************************/
void getAvailableFreeSets(RBGSet *rbgarr,
        UInt8 bitmap,
        UInt8 *arrIdx,
        UInt8 *lastBit,
        UInt8 startRBGId,
        RbMapNode *rbMapNode_p,
        UInt8 *actualSetsFound)
{
    UInt8 localBitmap=0;
    UInt8 loopCount =0 ;

    localBitmap = bitmap&FIRST8;
    if(NUMZERO == (localBitmap))
    {
        if(*lastBit)
        {
            rbgarr[*arrIdx].size += NUMEIGHT;
            return;
        }
        else
        {
            rbgarr[*arrIdx].size += NUMEIGHT;
            rbgarr[*arrIdx].startRBG = startRBGId;
            (*actualSetsFound)++;
            *lastBit = NUMONE;
            return;
        }

    }
    if(FIRST8==(localBitmap))
    {
        /* SPR 22259 Fix Start */
        if(*lastBit)
            (*arrIdx)++;
        *lastBit = NUMZERO;
        /* SPR 22259 Fix End */
        return;
    }
    else
    {
        loopCount= NUMZERO;
        while(loopCount < NUMEIGHT)
        {
            if(!(localBitmap & NUMONE))
            {
                if(*lastBit)
                {
                    rbgarr[*arrIdx].size++;
                }
                else
                {
                    rbgarr[*arrIdx].size++;
                    rbgarr[*arrIdx].startRBG = loopCount+startRBGId;
                    (*actualSetsFound)++;

                }
                *lastBit = NUMONE;
                localBitmap = localBitmap >> NUMONE;
                loopCount++;
            }
            else
            {
                if(BARRED_RESOURCE !=  rbMapNode_p->rbgMapForRAT1[loopCount + startRBGId])
                {
                    localBitmap = localBitmap >> rbMapNode_p->rbgMapForRAT1[loopCount + startRBGId];
                    loopCount += rbMapNode_p->rbgMapForRAT1[loopCount + startRBGId];
                }
                else
                {
                    localBitmap = localBitmap >> NUMONE;
                    loopCount++;

                }
                if(*lastBit)
                    (*arrIdx)++;
                *lastBit = NUMZERO; 
            }
        }
    }
}
/**************************************************************************************
 * Function Name     : getRbgIndexes
 * Inputs            : rbgarr 
 *                   : arrSize 
 *                   : s0
 *                   : s2
 *                   : s2Size
 *                   : rbgNeed
 * Outputs     	     : None
 * Returns           : void
 * Description       : This Function is used to get Rbs indexes
 ****************************************************************************************/
void getRbgIndexes(RBGSet *rbgarr,
        UInt8 arrSize,
        UInt8 *s0,
        UInt8 *s2,
        UInt8 *s2Size,
        UInt8 rbgNeed)
{

    UInt8 loopCounter1=0;
    UInt8 loopCounter2 =0;
    UInt8 max =0;
    UInt8 maxSetFound = 0;
    /* SPR 22280 Fix Start */
    for(loopCounter1 = 0;loopCounter1 < (arrSize - 1); loopCounter1++)
        /* SPR 22280 Fix End */
    {
        for(loopCounter2 = loopCounter1+1 ;loopCounter2 <arrSize; loopCounter2++)
        {
            if((rbgarr[loopCounter1].size + rbgarr[loopCounter2].size) == rbgNeed)
            {
                *s0 = loopCounter1;
                *s2 = loopCounter2;
                *s2Size = rbgarr[loopCounter2].size;
                return; 
            }
            else
            {
                if((!maxSetFound))
                {
                    if((rbgarr[loopCounter1].size + rbgarr[loopCounter2].size) > max) 
                    {
                        max = rbgarr[loopCounter1].size + rbgarr[loopCounter2].size;
                        *s0 = loopCounter1;
                        *s2 = loopCounter2;
                        *s2Size = rbgarr[loopCounter2].size;
                    }
                    if(max > rbgNeed)
                    {
                        *s2Size = rbgNeed - rbgarr[loopCounter1].size;
                        maxSetFound = TRUE; 
                    }
                }
            }
        }
    }
}
/*************************************************************************************
 * Function Name     : checkNumRBGFromRBGSet1StartIndex
 * Inputs            : s0
 *                   : s1 
 *                   : s2
 *                   : s3
 *                   : rbMapNum
 *                   : internalCellIndex
 * Outputs     	     : None
 * Returns           : MacRetType
 * Description       : This Function is used to check num of RBs for Set1
 ****************************************************************************************/
MacRetType checkNumRBGFromRBGSet1StartIndex(UInt8 s0,
        UInt8 s1,
        UInt8 s2,
        UInt8 s3,
        UInt8 rbMapNum,
        InternalCellIndex  internalCellIndex)
{
    UInt8 loopCounter = 0;

    for(loopCounter = s0 ;loopCounter < s1 ;loopCounter++)
    {
        if(FREE_RESOURCE != (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->rbgMapForRAT1[loopCounter])
        {
            return MAC_FAILURE;
        }
    }
    for(loopCounter = s2;loopCounter<s3;loopCounter++)
    {
        if(FREE_RESOURCE != (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->rbgMapForRAT1[loopCounter])
        {
            return MAC_FAILURE;
        }
    }
    return MAC_SUCCESS;
}
#endif
/* SPR 21958 PUSCH RAT1 Support End */
