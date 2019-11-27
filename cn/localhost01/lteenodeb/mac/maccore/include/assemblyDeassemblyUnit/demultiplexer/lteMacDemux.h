/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDemux.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Thid file contains the data structures and function
 *                     declaraions of the Demultiplexer 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacDemux.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.13  2010/03/30 10:18:37  gur18550
 * Uplink Throughput Calc
 *
 * Revision 1.12  2009/07/13 12:12:30  gur15697
 * DEMUX added
 *
 * Revision 1.11  2009/06/26 03:56:15  gur15697
 * numOfDemuxThreads argument removed from putDataInDemuxQueue()
 *
 * Revision 1.10  2009/06/19 13:25:55  gur15697
 * updated for sendCCCHMsgToRRC()
 *
 * Revision 1.9  2009/06/09 14:15:08  gur15697
 * decodeAndSendCCCHMsgToRRC() modified
 *
 * Revision 1.8  2009/06/05 08:06:20  gur15697
 * init and cleanup functions added
 *
 * Revision 1.7  2009/06/03 12:07:01  gur15697
 * initDeMuxMgr() added
 *
 * Revision 1.6  2009/05/27 09:33:48  gur15697
 * RACH handling added
 *
 * Revision 1.5  2009/05/20 05:35:42  gur15697
 * sendPowerHeadroomToRRC() funtion added
 *
 * Revision 1.4  2009/05/18 05:57:00  gur15697
 * Incorporated the comments
 *
 * Revision 1.3  2009/05/08 14:08:00  gur15697
 * compilation errors removed
 *
 * Revision 1.2  2009/05/05 11:42:30  gur15697
 * modified to add logic in the functions
 *
 * Revision 1.1  2009/04/30 06:57:39  gur15697
 * Initial vertion
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_DEMUX_H
#define LTE_MAC_DEMUX_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacULQueueLoad.h"
#include "lteMacRandomAccess.h"
#include "lteMacParseUtil.h"
#include "lteMacCCCH.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/


/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define DEMUX "demux"

#define BSR_MAC_CONTROL     1
#define UL_DATA             0
#define DERIVED_BSR         2

#define MAC_UL_PDU_EXTN_FLAG          0x20
#define CRNTI_MAC_CE_LENGTH           2
#define LONG_BSR_MAC_CE_LENGTH        3
#define SHORT_BSR_MAC_CE_LENGTH       1
#define POWER_HEADROOM_MAC_CE_LENGTH  1
/* Review comment fix start LJ6 */
#define MAC_UL_PDU_RESERVE_BIT_FLAG   0xC0
#define MAC_UL_PDU_RESERVE_BIT1_FLAG  0x80
#define MAC_UL_PDU_RESERVE_BIT2_FLAG  0x40
/* Review comment fix end LJ6 */

/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct DemuxDataBaseT
{
    UInt8  lchId; /*Logical channel ID*/
    UInt32 length; /*Data length for this logical channel id*/
}DemuxDataBase;

#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD) 

/* SPR 11331 Fix Start */
#ifdef TDD_CONFIG
typedef struct ueToBeProcessedT
{
    UInt16 ueIndex;
    /* SPR 15909 fix start */
    tickType_t rcvdTTI;
    /* SPR 15909 fix end */
}ueToBeProcessed;
#endif
/* SPR 11331 Fix End */
typedef struct DemuxProcessMapT
{
/*SPR 21653 changes start*/
    UInt16 demuxIndex;
/*SPR 21653 changes end*/

/* SPR 11331 Fix Start */
#ifdef FDD_CONFIG
    UInt16 ueIdToBeProcessed[MAX_UE_SUPPORTED];
#else
   ueToBeProcessed ueIdToBeProcessed[MAX_UE_SUPPORTED];
#endif
/* SPR 11331 Fix End */
}DemuxProcessMap;
#endif

typedef struct DataQueueInfoNodeT
{
    LTE_SNODE   dataQueueInfoNodeAnchor;
    UInt16  ueIdx;
    UInt16  rcvdRNTI; /*RNTI on which message received*/
    /* SPR 15909 fix start */
    tickType_t  rcvdTTI;  /*TTI in which PDU received*/
    /* SPR 15909 fix end */
    UInt32  dataLen;  
    /* CLPC_CHG */
    /* Store the sum of the values of deltaTF and 10LogM factors to be used in 
     * calculating pathLoss from the PHR. This node, when popped out while 
     * processing the demux, shall carry this value and will be passed in the 
     * function  calculatePathlossFromPHR */
    SInt8 powerCorrectionInfo;
    /*CLPC_CHG_RILEN*/
    UInt8 riLength;
    /* SPR 4751 changes */
    UInt8 harqProcessId;
    /* SPR 4751 changes */
    /*CLPC_CHG_RILEN*/
    /* CLPC_CHG END*/
    UInt8   *recvdUlData_p; /*Pointer to the received data*/
}DataQueueInfoNode;
typedef struct DeMuxThreadInfoT
{
    LTE_SQUEUE demuxQueue;/*Demux queue*/
}DeMuxThreadInfo;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/*CA Changes start  */
extern void initDeMuxMgr(UInt32 maxDemuxQueues, UInt8 numCellsConfigured);

extern void cleanupDeMuxMgr(UInt32 maxDemuxQueues, InternalCellIndex internalCellIndex);
/*CA Changes end  */

extern void  processDemuxQueue(UInt32 thdIndex, 
        UInt32 numNodesToProcess,
        UInt32 dlDelay,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex,
        /*CA Changes end  */
        ExecutionLegFunctionArg* execArgs
        );

extern MacRetType putDataInDemuxQueue(UInt16  ueIdx,
        UInt16  rcvdRNTI,
        /* SPR 15909 fix start */
        tickType_t  rcvdTTI,
        /* SPR 15909 fix end */
        UInt8   *recvdUlData_p,
        UInt32  dataLen,
        /* +CLPC_CHG_LJA*/
        SInt8   powerControlDeltaTFAndLogFactor,
        /* -CLPC_CHG_LJA*/
        /*CLPC_CHG_RILEN*/
        UInt8 riLength,
        /*CLPC_CHG_RILEN*/
        /* SPR 4751 changes */
        UInt8 harqProcessId,
        /* SPR 4751 changes */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        );

extern MacRetType processDemuxData( UInt16  ueIndex,
        UInt16  rcvdRNTI,
        /* SPR 15909 fix start */
        tickType_t  rcvdTTI,
        /* SPR 15909 fix end */
        void    *recvdUlData_p,
        UInt32  dataLen,
        /* +CLPC_CHG_LJA*/
        SInt8   powerControlDeltaTFAndLogFactor,
        /* -CLPC_CHG_LJA*/
        UInt32  thdIndex,
        UInt32  dlDelay,
        /* +- SPR 17777 */
        /* SPR 4751 changes */
        UInt8 harqProcessId,
        /* SPR 4751 changes */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        );

extern void decodeAndSendCCCHMsgToRRC(UInt32 *dataPtrPos_p,
        /* +- SPR 17777 */
        void   **org_p,
        UInt32 *length_p,
        UInt16 rnti,
        UInt16  ueIndex, /*CLPC Change*/
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
        /*CA Changes end  */

/* SPR 5599 changes start (ZIP ID 129059) */
void adjustTokensForLcId(
/* SPR 5599 changes end (ZIP ID 129059) */
        ULUEContext * ulCntxt_p,
        UInt8   lchId,
        UInt32  length,
        UInt8   ulHarqProcessId
        /* SPR 15909 fix start */
#if (defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
        ,tickType_t  currentGlobalTick
#endif
        );
        /* SPR 15909 fix end */

/* SPR 5599 changes start (ZIP ID 129059) */
void restoreTokensOfNonTransmittedLCs(
/* SPR 5599 changes end (ZIP ID 129059) */
        ULUEContext * ulCntxt_p,
        UInt8   ulHarqProcessId);
/*SPR_3065_FIX*/
/*CA Changes start  */
extern void sendSyncIndToRRC(UInt16 ueIndex, UInt16 rnti, SyncIndId syncStatus, InternalCellIndex internalCellIndex);
/*CA Changes end  */
/*SPR_3065_FIX*/


#endif  
