/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPHICH.h,v 1.1.1.1.6.1.4.2.2.2.8.1 2010/11/08 06:06:18 gur10121 Exp $
 *
 ****************************************************************************
 *
 *  File Description : THIS FILE CONTAINS THE DATA STRUCTURES AND THE FUNCTION
 *                     DECLEARATION OF PHICH MODULE. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacPHICH.h,v $
 * Revision 1.1.1.1.6.1.4.2.2.2.8.1  2010/11/08 06:06:18  gur10121
 * Semi static changes merging
 *
 * Revision 1.1.1.1.6.1.4.2.2.2.2.2  2010/10/27 17:13:49  gur10121
 * UT fixes + review comments incorporation
 *
 *
 * Revision 1.1.1.1.6.1.4.2.2.1.6.1  2010/10/15 18:53:13  gur10121
 * Semi staic Changesdone and checked in
 *
 * Revision 1.1.1.1.6.1.4.2.2.2  2010/10/14 17:18:12  gur19413
 * DMRS FIXES
 *
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:45:58  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.23  2009/09/14 07:18:10  gur18550
 * Logs Updated
 *
 * Revision 1.22  2009/07/27 07:17:13  gur19413
 * added new macro
 *
 * Revision 1.21  2009/07/15 13:06:58  gur18550
 * CRC INVALID HANDLING
 *
 * Revision 1.20  2009/07/14 06:00:30  gur18550
 * #ifdef  PDCCH_DELAY_THREE
 *
 * Revision 1.19  2009/07/14 05:45:08  gur18550
 * Function Def Changed
 *
 * Revision 1.18  2009/07/13 10:11:16  gur18550
 * ulSTATS
 *
 * Revision 1.17  2009/07/07 13:07:20  gur18550
 * File Updated
 *
 * Revision 1.16  2009/07/02 07:00:26  gur12140
 * Incorporated Comments
 *
 * Revision 1.15  2009/07/01 06:47:30  gur18550
 * File Updated
 *
 * Revision 1.14  2009/06/25 11:02:13  gur18569
 * passing number of rbs and start rb index in presence of PHYSIM_TESTING macro
 *
 * Revision 1.13  2009/06/23 15:57:54  gur18550
 * Final Review Comments Inc
 *
 * Revision 1.12  2009/06/22 08:05:48  gur18550
 * Final Comments Inc
 *
 * Revision 1.11  2009/06/09 12:24:49  gur18550
 * ValidateRB Updated
 *
 * Revision 1.10  2009/06/05 14:18:23  gur18550
 * Function Updated
 *
 * Revision 1.9  2009/06/04 09:54:48  gur18550
 * Warnings Removed
 *
 * Revision 1.8  2009/06/04 07:44:49  gur18550
 * Comments Incorporated
 *
 * Revision 1.7  2009/06/03 15:45:32  gur18550
 * File Updated
 *
 * Revision 1.4  2009/05/29 05:51:02  gur18569
 * removed compilation errors
 *
 * Revision 1.3  2009/05/12 10:25:39  gur18550
 * file Updated
 *
 * Revision 1.2  2009/05/06 07:41:29  gur18550
 * File Updated
 *
 * Revision 1.1  2009/04/30 05:34:45  gur18550
 * INITIAL VERSION
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_PHICH_H
#define LTE_MAC_PHICH_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacDLUEContext.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacULHarqMgr.h"
#include "lteSQueue.h"
#include "lteMacStatsManager.h"
#include "lteCommonStatsManager.h"
#include "lteMacRandomAccess.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_REDUNDANCY_VERSION 8 
#define UL_MCS_28 28
#define UL_MCS_29 29
#define UL_MCS_30 30
#define UL_MCS_31 31
#define THRESHOLD_TTI 1000

#define HI_ACK 1
#define HI_NACK 0
#define MAX_NO_OF_HI_PDU 100
#ifdef TDD_CONFIG
#define MAX_NUMBER_OF_CONTAINER 15
#elif FDD_CONFIG
#define MAX_NUMBER_OF_CONTAINER 8
#endif
#define MIN_UL_GRANT_PUSCH 2
#define CRC_NACK_UL_GRANT_REQ_MSG 5
#define LOG_PHICH "PHICH"
#define MAX_DMRS_VALUE 8
#define TWO_SPREAD_FACTOR 8
/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*
   The following structure would be used in the case of Delay
   Path 3 to store the UE Index for those which are scheduled 
   at a certain tick. This information would be used by the
   PDCCH to allocate cce Index in the + 8 th tick, assuming
   that all NACK has been received.
*/
 
 
typedef struct ExpectedUeInfoT
{
    UInt8 harqProcessId;
    UInt16 ueIndex;
}ExpectedUeInfo;

typedef struct expectedULAckNackContainerNodeT
{
  UInt8 count;
  /* This array will contain the UE Index for which the UL
     ACK/NACK is expected */  
  ExpectedUeInfo expectedULAckNackArray[MAX_NO_OF_HI_PDU];
}expectedULAckNackContainerNode;
/* SPR 9658 changes start*/
#define INVALID_RI_START    255
/* SPR 9658 changes end*/


/*
   The HI PDU has to be sent with the information about the 
   lowestULRBIndex hiValue (ACK/NACK) crnti, All these inforamtion
   would be sent by the Transmitter along with the control PDU, The
   Transmitter will access the global array of the following structure
   to extract the required information.
 */
typedef struct MacHIPduInfoT
{
  UInt8  nPhichGrp;
  UInt8  nPhichSeq;
  UInt8  rbStart;
  UInt8  nDmrs;
  UInt8  hiValue;
  UInt16 crnti;
  /* CR changes start */
#ifdef FDD_CONFIG
  UInt16 txPower;
#endif
  /* CR changes end */
/*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
/* Value of iPhich is 1 corresponding to UL Tx in subframe 4 and 
* subframe 9 for TDD_CONFIG 0 else it's value is 0*/
  UInt8  iPhich;
#endif
/*TDD Config 0 Changes End*/ 
}MacHIPduInfo;

typedef struct HIPduArrayT
{
  UInt16 count;
  MacHIPduInfo macHIPduInfo[MAX_NO_OF_HI_PDU];
}HIPduArray;


/*Structure of Container's ACK Node*/    
typedef struct ContainerAckQueueNodeT
{
  LTE_SNODE ackNodeAnchor;
  /* SPR 15909 fix start */
  tickType_t    ttiCounter;
  /* SPR 15909 fix end */
  UInt32    schUsed;
  UInt16    ueIdx;    
  UInt8     crcValid;
  UInt8     ulHarqProcessId;
#ifdef TDD_CONFIG	
  UInt8     allocatedLength;
  UInt8     allocatedIndex;
#endif	
/* msg3_retx_tdd_support_start */
/*#ifdef FDD_CONFIG*/
    UInt8     tcRntiFlag;
/*#endif*/
/* msg3_retx_tdd_support_end */

  /*bug_10441_changes start*/
  UInt8 isLastRetx;
  /*bug_10441_changes end*/
}ContainerAckQueueNode;

/*structure for container's NACK_QUEUE node	*/	
typedef struct ContainerNackQueueNodeT
{
    LTE_SNODE 		nackNodeAnchor;
    UInt16	ueIdx;	/*Index of the UE in uplinkContextInfoMap_g array   */
    /* SPS_CHG */
    UInt32    schUsed;
    /* SPS_CHG */
    /* SPR 15909 fix start */
    tickType_t  ttiCounter;
    /* SPR 15909 fix end */
    UInt8     ulHarqProcessId;
    /*TDD Config 0 Changes start*/
#ifdef TDD_CONFIG
    /*UL Sub Frame in which the Retransmission will occure*/
    UInt8 ulSubFrame;
#endif
    /*TDD Config 0 Changes end*/
}ContainerNackQueueNode;

typedef struct UplinkContainerT
{
#ifndef DL_UL_SPLIT
        /*NACK queue containing temp UEIds in case NACK for MSG3 transmission*/
        LTE_SQUEUE         nackQueueForTempUE;           
#elif CIRC_QUEUE_IMPL
        /*NACK queue containing temp UEIds in case NACK for MSG3 transmission*/
        NACK_CIRC_TYPE nackCircQueueForTempUE;           
        /*NACK queue containing UEIds in case NACK for UL transmission*/
        NACK_CIRC_TYPE nackCircQueue;           
#endif
        /*ACK queue containing UEIds in case ACK for UL transmission*/
        LTE_SQUEUE         ackQueue;
    	/*NACK queue containing UEIds in case NACK for UL transmission*/
        LTE_SQUEUE         nackQueue; /*SPR 19221 +-*/ 

  
  /*ACK queue containing UEIds in case ACK for UL transmission*/
  LTE_SQUEUE         nackQueueForSps;

#ifdef DL_UL_SPLIT
        SInt32 ulContainerQIdUL;
        SInt32 ulContainerQIdDL; 
        SInt32 ulContainerNackQueueForTempUEIdUL; 
        SInt32 ulContainerNackQueueForTempUEIdDL; 
#endif
        /* SPR 8998 Fix Start End*/
        UInt8 schedulerFlag;
        /* SPR 5798 Fix Start */
        UInt8 nackQueueCount;
        /* SPR 5798 Fix End */
#ifdef FDD_CONFIG
        NACK_QUEUE_FOR_TTIB_UE   nackQueueForTtiBundledUe;
#endif

}UplinkContainer;

/************************************************************************
 * Exported Constants
 ************************************************************************/

/************************************************************************
 * Exported Variables
 ************************************************************************/
/*CA Changes start  */
extern HIPduArray* hiPduContainer_gp[MAX_NUM_CELL]; 

extern UplinkContainer* ulContainerPool_gp[MAX_NUM_CELL];
/*CA Changes end  */

extern expectedULAckNackContainerNode expectedULAckNackContainer_g[MAX_NUMBER_OF_CONTAINER] ;

#ifdef FDD_CONFIG

#define TTIB_PHICH_POWER_INCR 1
/*CA Changes start  */
extern PHICHSequenceNoPerPhichGroup   phichSequenceNoPerPhichGroup_g[MAX_NUM_CELL][MAX_PHICH_CONTAINER][MAX_PHICH_GRP]; 
#elif TDD_CONFIG
extern UInt8   phichSequenceNoPerPhichGroup_g[MAX_NUM_CELL][MAX_SUB_FRAME][MAX_PHICH_GRP]; 
/*CA Changes end  */
#endif


/************************************************************************
 * Exported Functions
 ************************************************************************/
#ifdef FDD_CONFIG                           
/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType allocatePUSCHResourcesForNackReceived( 
/* SPR 5599 changes end (ZIP ID 129059) */
    ULHarqInfo* tempULHarqInfo,
    /* SPR_1987 Start */
    UInt8 maxUlHarqTx,
    /* SPR_1987 End */
    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p ,
    AllocatedResInfo* resourceReserved_p,
    UInt8 rbmapNumAdvanced,
    UInt8 subFrameNumber
    /* SPR 1996 Start*/
    ,DciCCEInfo *tempDciCCEInfo_p
    /* SPR 1996 End*/
    );
/*SPR_3305_FIX_START*/
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType processULNackForNonAdaptiveReTxOfMsg3(UInt16 tempUeIndex,
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt8 containerTick,
        /* +- SPR 17777 */
        TempCRNTICtx *tempUEContext_p,
        UInt32 rbMapNum,
        UInt8* numberOfULUEsToBeScheudled_p,
        UInt32 * tempPUSCHResources_p,
        /* SPR 15909 fix start */
        tickType_t timerExpiredTTI,
        /* SPR 15909 fix end */
        UInt32 timerExpsubFrameNum,
        UInt32 ulHarqSysFrameNum,
        UInt32 hiIndex,
        UInt16 *numberOfInfoPresent_p,
        InternalCellIndex internalCellIndex);
/* SPR 5599 changes start (ZIP ID 129059) */
void processULNackForAdaptiveReTxOfMsg3(UInt16 tempUeIndex,
/* SPR 5599 changes end (ZIP ID 129059) */
        UInt8 containerTick,
        TempCRNTICtx *tempUEContext_p,
        AllocatedResInfo* resourceReserved,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt8 ulDelay,
        UInt32 rbMapNum,
        UInt8* numberOfULUEsToBeScheudled_p,
        UInt32 * tempPUSCHResources_p,
        /* SPR 15909 fix start */
        tickType_t timerExpiredTTI,
        /* SPR 15909 fix end */
        UInt32 timerExpsubFrameNum,
        UInt32 ulHarqSysFrameNum,
        UInt32 hiIndex,
        UInt16 *numberOfInfoPresent_p,
        /* ICIC changes start */
        UInt8 regionAllocated,
        /* ICIC changes end */
        /*CA Changes start */
        InternalCellIndex internalCellIndex);
        /*CA Changes end */

/*SPR_3305_FIX_END*/

#elif TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
extern MacRetType allocatePUSCHResourcesForNackReceived(
/* SPR 5599 changes end (ZIP ID 129059) */
    ULHarqInfo* tempULHarqInfo,
    /* SPR_1987 Start */
    UInt8 maxUlHarqTx,
    /* SPR_1987 End */
    UeScheduledInfo *uescheduledInfo_p,
    AllocatedResInfo* resourceReserved_p,
    UInt8 rbmapNumAdvanced,
    UInt8 subFrameNumber
    /* SPR 1996 Start*/
    ,DciCCEInfo *tempDciCCEInfo_p
    /* SPR 1996 End*/
    );
#endif

extern UInt32 validateUplinkRB (
    ULUEContext* ueULContext_p,
    UInt32 allocatedRB,
    UInt32 numberOfAvailableRBs ,
    UInt32* strictRBAllocation
    /* ULA_CHG */
    ,UInt32* minDataSize,
    UInt32* maxDataSize,
    /* ULA_CHG */
    /* ATB Changes Start */
    /* SPR 13532 Fix Start */
    UInt8 *mcsVal,
    /*SPR 16417 Changes Start*/
#ifdef FDD_CONFIG
    /* SPR 13532 Fix Start */
    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithULDelay,
    /* SPR 15909 fix end */
    /* SPR 13532 Fix End */
#elif TDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t currentGlobalTTITickCount,
    /* SPR 15909 fix end */
    UInt8 ulSubFrameNum,
#endif
    /*SPR 16417 Changes End*/
    /* SPR 13532 Fix End */
    /* ATB Changes End */
    InternalCellIndex internalCellIndex
    );

extern UInt8 getPhichGrpAndPhichSeq(
    UInt8  riStart,
    UInt8  *nPhichGrp_p,
    UInt8  *nPhichSeq_p,
    UInt8  *sfUsed,
    UInt8 *nDmrs_p,
    UInt8  hiIndex,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex
    /*CA Changes end  */
    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    /*Value of iPhich may be 1 or 0 in TDD config 0*/
    ,UInt8 iPhich
#endif
    /*TDD Config 0 Changes End*/ 
    );

/*SPR_3305_FIX_START*/
extern MacRetType getPhichGrpAndPhichSeqForMsg3(
        UInt8  riStart,
        UInt8  *nPhichGrp_p,
        UInt8  *nPhichSeq_p,
        UInt8  *sfUsed,
        UInt8 *nDmrs_p,
        UInt8  hiIndex,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
        /*Value of iPhich may be 1 or 0 in TDD config 0*/
        ,UInt8 iPhich
#endif
        /*TDD Config 0 Changes End*/ 
        );
/*SPR_3305_FIX_END*/

/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
extern MacRetType checkAvailabilityOfPhichGrpAndPhichSeq(UInt8 riStart,
        UInt8 *nDmrs_p,
        UInt8 hiIndex,
        UInt8 iPhich,
        InternalCellIndex internalCellIndex 
        );
#endif
/*TDD Config 0 Changes End*/
void releasePhich(UInt8 nPhichSf, 
    UInt8 nPhichGrp, 
    UInt8 nPhichSeq,
    /*CA Changes start */
    InternalCellIndex internalCellIndex);
    /*CA Changes end */
#ifdef FDD_CONFIG
extern void addEntryToPhichGrpAndSeqArrays(InternalCellIndex internalCellIndex);
#else
extern void addEntryToPhichGrpAndSeqArrays(UInt8 numPHICHGroup,InternalCellIndex internalCellIndex);
#endif

/* SPR 7435 Changes Starts */
/* SPR 5599 changes start (ZIP ID 129059) */
 void addEntryTOHIPduArray(
/* SPR 5599 changes end (ZIP ID 129059) */
/* SPR 7435 Changes Ends */
    ULHarqInfo* uLHarqInfo_p,
    UInt8 hiValue,
    UInt16 crnti,
    /* CR changes start */
#ifdef FDD_CONFIG
    ULUEContext *ueULContext_p,
    UInt8 hiIndex,
#endif
    /* CR changes end */
    HIPduArray* tempHIPduNode,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex
    /*CA Changes end  */
/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    ,UInt8 ulSubFrameNum
 /* +- SPR 17777 */
#endif
/*TDD Config 0 Changes End*/ 
     );

/* SPR 5599 changes start (ZIP ID 129059) */
void processULAckInPUSCH(
/* SPR 5599 changes end (ZIP ID 129059) */
    UInt8 ulDelay,
    /* SPR 15909 fix start */
    tickType_t currentGlobalTick,
    /* SPR 15909 fix end */
    /* CR changes start */
#ifdef FDD_CONFIG
    UInt8 hiIndex,
#endif
    /* CR changes end */
    UInt8 hiPduContainerTick,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex);
    /*CA Changes end  */

#ifdef FDD_CONFIG

void processULNackNonAdaptiveBundleInPUSCH(
        UInt8 ulDelay,
        UInt32 rbMapNum, 
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        UInt8 containerTick,
        UInt8 subFrame,
        InternalCellIndex internalCellIndex);
void processULNackAdaptiveBundleInPUSCH(
        UInt8 ulDelay,
        UInt32 rbMapNum, 
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        UInt8 containerTick,
        UInt8 subFrame,
        UInt8 locType,
        InternalCellIndex internalCellIndex);
/* ICIC changes start */
/* SPR 5599 changes start (ZIP ID 129059) */
void processULNackNonAdaptiveInPUSCH(
/* SPR 5599 changes end (ZIP ID 129059) */
    UInt8 ulDelay,
    UInt32 subFrameNumber,
    /* SPR 15909 fix start */
    tickType_t currentGlobalTick,
    /* SPR 15909 fix end */
    UInt8* numberOfULUEsToBeScheudled_p,
    UInt8 containerTick,
    /* CR changes start */
    UInt8 hiIndex,
    /* CR changes end */
    UInt8 subFrameNum,
    InternalCellIndex internalCellIndex);
/* SPR 5599 changes start (ZIP ID 129059) */
void processULNackAdaptiveInPUSCH(
/* SPR 5599 changes end (ZIP ID 129059) */
    UInt8 ulDelay,
    UInt32 subFrameNumber,
    /* SPR 15909 fix start */
    tickType_t currentGlobalTick,
    /* SPR 15909 fix end */
    UInt8* numberOfULUEsToBeScheudled_p,
    UInt8 containerTick,
    /* CR changes start */
    UInt8 hiIndex,
    /* CR changes end */
    UInt8 subFrameNum,
    UInt8 userLocType,
    InternalCellIndex internalCellIndex);
/* ICIC changes end */

/* SPR 5599 changes start (ZIP ID 129059) */
void handleAckNodeForTcrnti(ContainerAckQueueNode* ackNode_p,
/* SPR 5599 changes end (ZIP ID 129059) */
                                   UInt8 ulDelay,
                                   UInt8 hiPduContainerTick,
                                   UInt8 hiIndex,
                                   /* SPR 15909 fix start */
                                   tickType_t currentGlobalTick,
                                   /* SPR 15909 fix end */
                                   InternalCellIndex internalCellIndex);

#elif TDD_CONFIG                               
/* ICIC changes start */
/* SPR 5599 changes start (ZIP ID 129059) */
void processULNackNonAdaptiveInPUSCH(
/* SPR 5599 changes end (ZIP ID 129059) */
                               PdcchULOutputInfo* tempPdcchULOutput_p,
                               UInt32 rbMapNum,
                               UInt32 subFrameNumber,
                               /* SPR 15909 fix start */
                               tickType_t currentGlobalTick,
                               /* SPR 15909 fix end */
                               UInt16* numberOfULUEsToBeScheudled_p,
                               UInt8 containerTick,
                               InternalCellIndex internalCellIndex,
                               /*TDD Config 0 Changes Start*/ 
                               UInt8 ulSubFrameNum);
                               /*TDD Config 0 Changes End*/ 
/* SPR 5599 changes start (ZIP ID 129059) */
void processULNackAdaptiveInPUSCH(
/* SPR 5599 changes end (ZIP ID 129059) */
                               PdcchULOutputInfo* tempPdcchULOutput_p,
                               UInt32 rbMapNum,
                               UInt32 subFrameNumber,
                               /* SPR 15909 fix start */
                               tickType_t currentGlobalTick,
                               /* SPR 15909 fix end */
                               UInt16* numberOfULUEsToBeScheudled_p,
                               UInt8 containerTick,
                               UInt8 locType,
                               InternalCellIndex internalCellIndex,
                               /*TDD Config 0 Changes Start*/
                               UInt8 ulSubFrameNum);
                               /*TDD Config 0 Changes End*/ 
/* ICIC changes end */
/* msg3_retx_tdd_start */
MacRetType processULNackForNonAdaptiveReTxOfMsg3Tdd(UInt16 tempUeIndex,
        UInt8 containerTick,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        TempCRNTICtx *tempCRNTICtx_p,
        UInt32 rbMapNum,
        UInt16* numberOfULUEsToBeScheudled_p,
        UInt32 * tempPUSCHResources_p,
        /* SPR 15909 fix start */
        tickType_t timerExpiredTTI,
        /* SPR 15909 fix end */
        UInt32 timerExpsubFrameNum,
        UInt32 ulHarqSysFrameNum,
        UInt16 *numberOfInfoPresent_p,
        /*CA Changes start */
        InternalCellIndex internalCellIndex,
        /*CA Changes end */
        /*TDD Config 0 Changes Start*/ 
        UInt8 ulSubFrameNum);
        /*TDD Config 0 Changes End*/

void processULNackForAdaptiveReTxOfMsg3Tdd(UInt16 tempUeIndex,
        UInt8 containerTick,
        TempCRNTICtx *tempCRNTICtx_p,
        AllocatedResInfo* resourceReserved,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt8 ulDelay,
        UInt32 rbMapNum,
        UInt16 *numberOfULUEsToBeScheudled_p,
        UInt32 *tempPUSCHResources_p,
        /* SPR 15909 fix start */
        tickType_t timerExpiredTTI,
        /* SPR 15909 fix end */
        UInt32 timerExpsubFrameNum,
        UInt32 ulHarqSysFrameNum,
        UInt16 *numberOfInfoPresent_p,
        UInt8 regionAllocated,
        /*CA Changes start */
        InternalCellIndex internalCellIndex,
        /*CA Changes end */
        /*TDD Config 0 Changes Start*/ 
        UInt8 ulSubFrameNum);
        /*TDD Config 0 Changes End*/

void handleAckNodeForTcrntiTdd(ContainerAckQueueNode* ackNode_p,
        UInt8 hiPduContainerTick,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        /*CA Changes start */
        InternalCellIndex internalCellIndex);
        /*CA Changes end */
/* msg3_retx_tdd_end */
#endif        

/* SPR 10803 [BUG 848] FIX START */
MacRetType checkResourceAvailability(
                                UInt8 startIndex,
                                UInt8 length ,
                                UInt8  *rbMap);
/* SPR 10803 [BUG 848] FIX END */
void processPUCCHAndPRACHReports( PdcchULOutputInfo *pdcchULOutputInfo_p,
                                   UInt32 rbMapNum,
                                   /* SPR 15909 fix start */
                                   tickType_t tti,
                                   /* SPR 15909 fix end */
/* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                                   UInt8 containerTick,
#endif
/* TDD Config 0 Changes End */
#ifdef TDD_CONFIG
                                   UInt32 ulSubFrame
#else
                                   UInt8 ulDelay
#endif
                                   /*CA Changes start  */
                                   ,InternalCellIndex internalCellIndex
                                   /*CA Changes end  */
                                );
                                                /* SPR 15909 fix start */
extern void processAdvancePUCCHAndPRACHReports ( UInt32 rbMapNum, tickType_t tti 
                                                /* SPR 15909 fix end */
                                                 #ifdef TDD_CONFIG
                                                  , UInt8 ulSubFrame
                                                #endif
                                                 /*CA Changes start  */
                                                 ,InternalCellIndex internalCellIndex
                                                 /*CA Changes end  */
                                                  );


/* SPR 5599 changes start (ZIP ID 129059) */
void updateDciCCEInfoArray(
/* SPR 5599 changes end (ZIP ID 129059) */
                       ULUEContext* ulUEContext_p,
                       UInt16 cceNumber,
                       UInt8 tempHarqProcessId,
                       DciCCEInfo* dciCCEInfo_p,
					   AllocatedResInfo*  resourceReserved_p,
                       /* +- SPR 17777 */
                       UInt8 tpc
                       /* CR changes start */
#ifdef FDD_CONFIG
                       ,UInt8 hiIndex
                       /* +- SPR 17777 */
                       ,ULGrantRequestType ulGrantRequestType
#endif
                       /* UL_MU_MIMO_CHG Start */ 
                       ,UInt16 buddyUeIndex
                       /* UL_MU_MIMO_CHG End */   
                       /* +- SPR 17777 */
                       /* + SPS_TDD_CHANGES */
#ifdef TDD_CONFIG
                       ,UInt8 spsAlloc
#endif
                       /* - SPS_TDD_CHANGES */
                       ,InternalCellIndex internalCellIndex
                       /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
                       ,tickType_t currentGlobalTick
                       /* +- SPR 17777 */
                       ,UInt8 ulSubFrameNum
                       ,UInt8 scheduledCount
#endif
                       /*TDD Config 0 Changes End*/ 
);
                       /* CR changes end */
/*SPR_3305_FIX_START*/
/* SPR 5599 changes start (ZIP ID 129059) */
void updateDciCCEInfoArrayForMsg3(
/* SPR 5599 changes end (ZIP ID 129059) */
        TempCRNTICtx* tempUEContext_p,
        UInt16 cceNumber,
        /* +- SPR 17777 */
        DciCCEInfo* dciCCEInfo_p,
        /* +- SPR 17777 */
        AllocatedResInfo*  resourceReserved_p
#ifdef FDD_CONFIG
        ,UInt8 tempHarqProcessId
        /* +- SPR 17777 */
        ,UInt8 hiIndex
#endif
        ,InternalCellIndex internalCellIndex
        /* +- SPR 17777 */
        /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG        
        ,tickType_t currentGlobalTick
        ,UInt8 ulDelay
        /* +- SPR 17777 */
        ,UInt8 ulSubFrameNum
#endif        
        /*TDD Config 0 Changes End*/
        );
/*SPR_3305_FIX_END*/

#ifdef FDD_CONFIG
/* ICIC changes start */
 void processULNackSPSNonAdaptiveInPUSCH(
    UInt8 ulDelay,
    UInt32 subFrameNumber,
    /* SPR 15909 fix start */
    tickType_t currentGlobalTick,
    /* SPR 15909 fix end */
    UInt8* numberOfULUEsToBeScheudled_p,
    UInt8 containerTick,
    /* CR changes start */
    UInt8 hiIndex,
    /* CR changes end */
    UInt8 subFrameNum,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex);
    /*CA Changes end  */
 void processULNackSPSAdaptiveInPUSCH(
    UInt8 ulDelay,
    UInt32 subFrameNumber,
    /* SPR 15909 fix start */
    tickType_t currentGlobalTick,
    /* SPR 15909 fix end */
    UInt8* numberOfULUEsToBeScheudled_p,
    UInt8 containerTick,
    /* CR changes start */
    UInt8 hiIndex,
    /* CR changes end */
    UInt8 subFrameNum,
    UInt8 userLocType,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex);
    /*CA Changes end  */
/* ICIC changes end */


UInt8 checkPhichGrpAndPhichSeq(UInt8 riStart,
                               UInt8 hiIndex,
                               /*CA Changes start  */
                               InternalCellIndex internalCellIndex);
                               /*CA Changes end  */

UInt8 markPhichGrpAndPhichSeq(UInt8 riStart,
                              UInt8 hiIndex,
                              /*CA Changes start  */
                              InternalCellIndex internalCellIndex);
                              /*CA Changes end  */

UInt8 getSPSPhichGrpAndPhichSeq(
        UInt8  riStart,
        UInt8  *nPhichGrp_p,
        UInt8  *nPhichSeq_p,
        UInt8  *sfUsed,
        UInt8 *nDmrs_p,
        UInt8  hiIndex,
        /*CA Changes start */
        InternalCellIndex internalCellIndex);
        /*CA Changes end */

/* + SPS_TDD_Changes */
#elif TDD_CONFIG
 void processULNackSPSAdaptiveInPUSCH(
        PdcchULOutputInfo* tempPdcchULOutput_p,
        UInt32 rbMapNum,
        UInt32 subFrameNumber,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt16* numberOfULUEsToBeScheudled_p,
        UInt8 containerTick,
        UInt8 locType,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /*CA Changes end  */
        /*TDD Config 0 Changes Start*/
        UInt8 ulSubFrameNum);
        /*TDD Config 0 Changes End*/

 void processULNackSPSNonAdaptiveInPUSCH(
        PdcchULOutputInfo* tempPdcchULOutput_p,
        UInt32 rbMapNum,
        UInt32 subFrameNumber,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt16* numberOfULUEsToBeScheudled_p,
        UInt8 containerTick,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex,
        /*CA Changes end  */
        /*TDD Config 0 Changes Start*/
        UInt8 ulSubFrameNum);
        /*TDD Config 0 Changes End*/
#endif
/* - SPS_TDD_Changes */

/* SPR 10027 FIX START */
/* Check if isDrbPresent = TRUE and Update the lastScheduledTick */
/*SPR 21133 Changes Start */
#define CHECK_AND_UPDATE_LAST_SCHEDULED_TICK(isDrbPresent,ulUEContext_p)\
if (TRUE == isDrbPresent)\
{\
    ulUEContext_p->lastScheduledTick = getCurrentTick();\
}
/*SPR 21133 Changes End */
/* SPR 10027 FIX END */




#endif  /* LTE_MAC_PHICH_H */
