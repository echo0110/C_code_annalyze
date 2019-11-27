/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacPHICH.c,v 1.2.6.1.4.2.2.4.8.2 2010/11/09 13:32:57 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : THIS FILE CONTAINS THE FUNCTIONS RELATED TO PHICH i.e. 
 *                     CREATATION OF HI PDU.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacPHICH.c,v $
 * Revision 1.2.6.1.4.2.2.4.8.2  2010/11/09 13:32:57  gur10121
 *  invalid trace added
 *
 * Revision 1.56.26.11  2010/10/25 13:11:58  gur19413
 * Self Review fixes
 *
 * Revision 1.56.26.10  2010/10/25 08:16:19  gur19413
 * removed magic number and dead code
 *
 * Revision 1.56.26.9  2010/10/25 06:01:45  gur19413
 * code review bug fixed
 *
 * Revision 1.56.26.8  2010/10/24 08:42:56  gur24420
 * DAI Fixes
 *
 * Revision 1.56.26.7  2010/10/21 12:23:57  gur24420
 * 1.Comments updated in TDD manager file.
 * 2.DL Strategy function call during the special subframe is
 *   handeled based on Special subframe configuration.
 *
 * Revision 1.56.26.6  2010/10/21 06:54:51  gur24420
 * 1. Clock work fixes
 * 2. Self review Changes
 *
 * Revision 1.56.26.5  2010/09/29 07:25:01  gur24420
 * Bugs fixed in UT
 *
 * Revision 1.56.26.4  2010/09/20 15:13:35  gur19413
 * bug fixed
 *
 * Revision 1.56.26.3  2010/09/20 14:00:21  gur19413
 * bug fixed
 *
 * Revision 1.56.26.2  2010/09/07 08:44:44  gur19413
 * modifed func parameter
 *
 * Revision 1.56.26.1  2010/08/31 08:23:42  gur19413
 * added nDMRS functionality and tdd functionality
 *
 * Revision 1.56  2009/10/21 07:04:51  gur11912
 * removed warnings
 *
 * Revision 1.55  2009/10/21 06:01:20  gur19413
 * stats optimized
 *
 * Revision 1.54  2009/10/06 06:10:49  gur11912
 * optimization changes
 *
 * Revision 1.53  2009/09/24 05:02:00  gur11912
 * removed localCount from UT_TESTING
 *
 * Revision 1.52  2009/09/16 14:28:32  gur18550
 * Function Updated
 *
 * Revision 1.51  2009/09/16 14:22:17  gur18550
 * Function Optimised
 *
 * Revision 1.50  2009/09/14 07:22:18  gur18550
 * Removed Windows TABs
 *
 * Revision 1.49  2009/09/14 07:21:22  gur18550
 * Functon Updated as per Modified PDCCH
 *
 * Revision 1.48  2009/09/03 09:03:47  gur19413
 * set the isStopAdvancedAllocation = 0
 *
 * Revision 1.47  2009/09/01 15:14:52  gur12140
 * Solved Bug Related toUL and DL
 *
 * Revision 1.46  2009/08/24 12:36:45  gur18550
 * Warnings removed
 *
 * Revision 1.45  2009/08/20 06:45:50  gur19413
 * added new parameter in fillULStats
 *
 * Revision 1.44  2009/08/13 10:22:20  gur18550
 * Warnings Removed
 *
 * Revision 1.43  2009/08/11 14:09:34  gur18550
 * Functon Updated
 *
 * Revision 1.42  2009/08/04 09:09:42  gur18550
 * Function Updated
 *
 * Revision 1.41  2009/07/30 12:35:51  gur18550
 * Removed Bug
 *
 * Revision 1.40  2009/07/29 15:08:08  gur18550
 * Removed Warning
 *
 * Revision 1.39  2009/07/29 12:53:08  gur18550
 * Function Updated
 *
 * Revision 1.38  2009/07/29 10:38:02  gur18550
 * HAndling of UL Nack
 *
 * Revision 1.37  2009/07/27 14:42:44  gur18550
 * Function Updated
 *
 * Revision 1.36  2009/07/27 07:04:18  gur19413
 * stats related changes
 *
 * Revision 1.35  2009/07/23 07:12:06  gur18550
 * UL Ack Updated
 *
 * Revision 1.34  2009/07/20 07:05:02  gur18550
 * Functionality of UL NACK revisited
 *
 * Revision 1.33  2009/07/17 15:22:21  gur18550
 * Nack Handling modified
 *
 * Revision 1.32  2009/07/17 14:00:03  gur18550
 * Function Updated
 *
 * Revision 1.31  2009/07/17 12:20:38  gur18550
 * Function Updated
 *
 * Revision 1.30  2009/07/15 13:27:14  gur18550
 * Log Updated
 *
 * Revision 1.29  2009/07/15 13:06:29  gur18550
 * CRC INVALID HANDLING
 *
 * Revision 1.28  2009/07/14 05:59:45  gur18550
 * #ifdef  PDCCH_DELAY_THREE
 *
 * Revision 1.27  2009/07/14 05:44:49  gur18550
 * Function Updated
 *
 * Revision 1.26  2009/07/13 10:11:27  gur18550
 * ulStats
 *
 * Revision 1.25  2009/07/13 08:39:14  gur18550
 * Log Updated
 *
 * Revision 1.24  2009/07/10 06:19:52  gur18550
 * DELAY_THREE related changes
 *
 * Revision 1.23  2009/07/07 13:07:01  gur18550
 * Function Updated
 *
 * Revision 1.22  2009/07/02 06:59:27  gur12140
 * Incorporated Comments
 *
 * Revision 1.21  2009/07/01 07:12:53  gur18550
 * Changes due to ulCCEIndex
 *
 * Revision 1.20  2009/07/01 06:47:15  gur18550
 * Function Updatred
 *
 * Revision 1.19  2009/06/25 11:01:56  gur18569
 * passing number of rbs and start rb index in presence of PHYSIM_TESTING macro
 *
 * Revision 1.18  2009/06/24 15:15:30  gur18550
 * Errors removed
 *
 * Revision 1.17  2009/06/24 14:06:22  gur18550
 * Added isPdcchAllocated
 *
 * Revision 1.16  2009/06/23 15:56:47  gur18550
 * Final Review Comments Inc
 *
 * Revision 1.15  2009/06/22 08:05:28  gur18550
 * Final Comments Inc
 *
 * Revision 1.14  2009/06/15 04:30:46  gur18550
 * Function Updated
 *
 * Revision 1.13  2009/06/10 05:03:46  gur18550
 * Warnings Removed
 *
 * Revision 1.12  2009/06/09 14:19:24  gur18550
 * Function Updated
 *
 * Revision 1.11  2009/06/09 07:29:27  gur18550
 * Function Updated validateUplinkRB
 *
 * Revision 1.10  2009/06/09 07:02:03  gur18550
 * Function Updated
 *
 * Revision 1.9  2009/06/08 15:27:16  gur12140
 * Removed Linking Errors
 *
 * Revision 1.8  2009/06/05 15:45:41  gur18550
 * File Updated
 *
 * Revision 1.7  2009/06/04 09:54:26  gur18550
 * Warnings Removed
 *
 * Revision 1.6  2009/06/04 09:28:24  gur18550
 * Function Updation
 *
 * Revision 1.5  2009/06/04 07:43:58  gur18550
 * Comments Incorporated
 *
 * Revision 1.4  2009/06/03 16:05:16  gur18550
 * Comments incorporated
 *
 * Revision 1.3  2009/05/12 10:24:58  gur18550
 * Function Updated
 *
 * Revision 1.2  2009/05/05 14:16:18  gur18550
 * functon added
 *
 * Revision 1.1  2009/04/30 05:35:59  gur18550
 * INITIAL VERSION
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacPHICH.h"
#include "lteMacPUSCH.h"
#include "lteMacULSchedulerStrategy.h"
#include "lteMacULSpsScheduler.h"
#include "lteMacUtilityAndSharedStructs.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacULResourceAllocator.h"
#include "lteMacPuschPowerControl.h"
 /* CR changes start */
#ifdef FDD_CONFIG
#include "lteMacDLPowerControlData.h"
/* ICIC chnages start */
#include "lteMacULUEContext.h"
/* ICIC chnages end */
#endif
 /* CR changes end */
 /* +DYNAMIC_ICIC */
#include "lteMacULResourceInfoMgr.h"
 /* -DYNAMIC_ICIC */
#ifdef TDD_CONFIG
#include "lteMacReceiver.h"
#include "lteMacTransmitter.h"
#include "lteMacTDDConfig.h"
#endif
#include "ltePerfStats.h"
/******************************************************************************
 Private Definitions
 *****************************************************************************/



/******************************************************************************
 Private Types
 *****************************************************************************/
/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* Cyclomatic Complexity changes - starts here */
#ifdef FDD_CONFIG
static  void checkAndUpdatePowerCorrection (
        UInt32                        ueMaxPowerFlag, 
        UInt32                        ueMinPowerFlag, 
        TpcTriggeredToPowerCorrection powerCorrectionAndTpc, 
        ULUEContext                   *ulUEContext_p
#ifdef PERF_STATS
        ,UInt16                       tempUeIndex
#endif
        );
#endif        
/* Cyclomatic Complexity changes - ends here */

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* + SPR 6864 */
    /* + SPR_14994 */
extern void checkAndRestoreUCIPduType(
        ULUEContext * ulUEContext_p, 
#ifdef FDD_CONFIG
        DirectIndexingUEInfo *ueInfo_p,
#elif TDD_CONFIG
        UeScheduledInfo *ueInfo_p,
        UInt16 systemFrame,
#endif
        ULSchUciRachPoolInfo * ulschUciRachInfo_p,
        UInt8   subFrameNum,
        /*SPR 19504 fix start*/    
        UInt32 rbMapNum,
        InternalCellIndex internalCellIndex);
        /*SPR 19504 fix end*/    

    /* - SPR_14994 */
extern void dropCSIReportWithHarq(
        ULSchUciInfo *ulSchUciInfo_p);
/* - SPR 6864 */

#ifdef MAC_AUT
UInt8 prevUlSubFrameNo = 0;
UInt8 ulFrameOffset = 0;
#endif
/*CA Changes start  */
HIPduArray* hiPduContainer_gp[MAX_NUM_CELL] = {PNULL}; 

/*TDD Config 0 Changes Start*/
#ifdef FDD_CONFIG
UInt8   phichGroupFromPrbDmrsMap_g[MAX_NUM_CELL][MAX_NUM_RBS][MAX_DMRS_VALUE];
#elif TDD_CONFIG
/*phichGroupFromPrbDmrsMap_g[MAX_IPHICH_VALUE][MAX_NUM_RBS][MAX_DMRS_VALUE]*/
UInt8   phichGroupFromPrbDmrsMap_g[MAX_NUM_CELL][MAX_IPHICH_VALUE][MAX_NUM_RBS][MAX_DMRS_VALUE];
#endif
/*TDD Config 0 Changes End*/
UInt8   phichSequenceNoFromPrbDmrsMap_g[MAX_NUM_CELL][MAX_NUM_RBS][MAX_DMRS_VALUE];
/* CR changes start */
#ifdef FDD_CONFIG
PHICHSequenceNoPerPhichGroup   phichSequenceNoPerPhichGroup_g[MAX_NUM_CELL][MAX_PHICH_CONTAINER][MAX_PHICH_GRP]; 
/* SPR 16815 Fix Start  */
/*SPR 21635 +*/
UInt8 ueCntInPhichGrp[MAX_NUM_CELL][MAX_PHICH_GRP];
/*SPR 21635 -*/
/* SPR 16815 Fix End  */
#elif TDD_CONFIG
/* SPR 19679: TDD HARQ Multiplexing Changes Start */
extern UInt32 bitmask[MAX_SUB_FRAME];
/* SPR 19679: TDD HARQ Multiplexing Changes End */
UInt8   phichSequenceNoPerPhichGroup_g[MAX_NUM_CELL][MAX_SUB_FRAME][MAX_PHICH_GRP]; 
#endif
/*CA Changes end  */
/* CR changes end */
/* SPR 1010 changes start */
extern UInt8  maxpowerOfTwoThreeFive_g[MAX_NUM_RBS + 1];
extern UInt8  minpowerOfTwoThreeFive_g[MAX_NUM_RBS + 1];
/* SPR 1010 changes end */
/*START_DYN_PDCCH_TDD_CHANGE*/
#ifdef FDD_CONFIG
extern UInt8 maxCCEForCFI_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE];
#else
extern UInt8 maxCCEForCFI_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE][MAX_VALUE_M_I];
#endif
/*END_DYN_PDCCH_TDD_CHANGE*/
/*START : DYN_PDCCH*/
extern UInt8 currentCFIValue_g[MAX_NUM_CELL];
/*END   : DYN_PDCCH*/

/* SPR 21958 PUSCH RAT1 Support Start */
/* SPR 22262 Fix Start */
#ifdef FDD_CONFIG
extern UInt8 rbgSize_g[MAX_NUM_CELL];
#endif
/* SPR 22262 Fix End */
/* SPR 21958 PUSCH RAT1 Support End */

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/********************************************************************************
* Function Name         : addEntryTOHIPduArray
* Inputs                : uLHarqInfo_p - Pointer to ULHarqInfo,
*                         hiValue - ACK or NACK as according,
*                         crnti - The CRNTI value,
*                         ulUEContext_p - Pointer to ULUEContext,
*                         hiIndex,
*                         tempHIPdu_p - Pointer to the HIPDU Container,
*                         internalCellIndex - Cell-Index used at MAC
* Outputs               : None
* Returns               : none
* Description           : This function will create the HI PDU which would be
*                         used by Transmitter.
*********************************************************************************/

 void addEntryTOHIPduArray(
                                ULHarqInfo* uLHarqInfo_p,
                                UInt8 hiValue,
                                UInt16 crnti, 
                                /* CR changes start */
#ifdef FDD_CONFIG
                                ULUEContext *ulUEContext_p,
                                UInt8 hiIndex,
#endif
                                /* CR changes end */
                                HIPduArray* tempHIPdu_p,                                 
                                /*CA Changes start  */
                                InternalCellIndex internalCellIndex
                                /*CA Changes end  */
/*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG                                
                                 ,UInt8 ulSubFrameNum
                                 /* +- SPR 17777 */
#endif                       
/*TDD Config 0 Changes End*/
)
{  

/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    UInt8 prevUlSubFrameNum = ulSubFrameNum;
#endif
/*TDD Config 0 Changes End*/
#ifdef FDD_CONFIG
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
               cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    /* CR changes start */
    UInt8 cqiValue = 0;
    UInt16 txPowerOffset = 0;
    /* CR changes end */
    /* SPR 16815 Fix Start  */
    UInt8 numEntryInPhichGrp = 0;
    UInt8 hiPduCount = tempHIPdu_p->count;
    UInt8 count = 0;

    /* Resetting the values of prev TTI */
    /*SPR 21635 +*/
    if(0 == hiPduCount)
    {
        memSet((void*)ueCntInPhichGrp[internalCellIndex], 0, sizeof(ueCntInPhichGrp[0]));
    }
    ueCntInPhichGrp[internalCellIndex][uLHarqInfo_p->nPhichGrp]++;
    numEntryInPhichGrp = ueCntInPhichGrp[internalCellIndex][uLHarqInfo_p->nPhichGrp];
    /*SPR 21635 -*/
    /* SPR 16815 Fix End  */

#endif    
    MacHIPduInfo* ptrMacHIPduInfo = &tempHIPdu_p->macHIPduInfo[tempHIPdu_p->count];
    ptrMacHIPduInfo->nPhichGrp = uLHarqInfo_p->nPhichGrp;
    ptrMacHIPduInfo->nPhichSeq = uLHarqInfo_p->nPhichSeq;
    ptrMacHIPduInfo->rbStart = uLHarqInfo_p->riStart;
    ptrMacHIPduInfo->nDmrs = uLHarqInfo_p->nDmrs;
    ptrMacHIPduInfo->hiValue = hiValue;
    ptrMacHIPduInfo->crnti = crnti;
/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    /*Value of iPhich is 1 for TDD config 0 if the corresponding UL Tx occured
     * in subframe 4 or subframe 9 else it is 0*/
    ptrMacHIPduInfo->iPhich = calculateIPhichValue(prevUlSubFrameNum,internalCellIndex);
#endif
/*TDD Config 0 Changes End*/
    /* CR changes start */
#ifdef FDD_CONFIG
    /*CA Changes start  */
    if(phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][uLHarqInfo_p->nPhichGrp].count)
    /*CA Changes end  */
    {
        if((PNULL != ulUEContext_p) && (ulUEContext_p->isDLWideBandCQIReportAvailable))
        {
            if ((ulUEContext_p->dlWidebandCQICodeWordTwo != INVALID_CQI_VALUE)
                    && (ulUEContext_p->dlWidebandCQICodeWordOne >=
                        ulUEContext_p->dlWidebandCQICodeWordTwo))

            {
                cqiValue = ulUEContext_p->dlWidebandCQICodeWordTwo;
            }
            else
            {
                cqiValue = ulUEContext_p->dlWidebandCQICodeWordOne;
            }

            txPowerOffset = getPHICHPowerOffset(cqiValue, internalCellIndex);
            /*CA Changes start  */
            txPowerOffset += cellParams_p->commonDLPowerControlInfo.phichPowerOffset;
            /* SPR 16815 Fix Start */
            /*CA Changes end  */
        }
        else
        {
            /*CA Changes start  */
            txPowerOffset = cellParams_p->commonDLPowerControlInfo.phichPowerOffset/
                              phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][uLHarqInfo_p->nPhichGrp].count;
            /*CA Changes end  */
        }
        txPowerOffset /= numEntryInPhichGrp;
        for (count = 0; count < hiPduCount; count ++)
        {
            if (tempHIPdu_p->macHIPduInfo[count].nPhichGrp == uLHarqInfo_p->nPhichGrp)
            {
                tempHIPdu_p->macHIPduInfo[count].txPower = MAC_PHY_CONVERT_16(txPowerOffset);
            }
        }
        /* SPR 16815 Fix End */
    }
    else
    {
        /*CA Changes start  */
        txPowerOffset = cellParams_p->commonDLPowerControlInfo.phichPowerOffset;
        /*CA Changes end  */
    }

    /** TTIB_Code Start */

    if( ulUEContext_p && ulUEContext_p->ttiBundlingEnabled)
    {
        txPowerOffset += TTIB_PHICH_POWER_INCR;
    }

    ptrMacHIPduInfo->txPower = MAC_PHY_CONVERT_16(txPowerOffset);
#endif
    /* CR changes end */
    tempHIPdu_p->count++;
}



/*****************************************************************************************************
* Function Name :    processULAckInPUSCH 
* Inputs        :    ulDelay - The delay of execution legs in terms of the number
*                              subFrames w.r.t. PHY & the current processing being
*                              done at MAC for intended tick at PHY,
*                    currentGlobalTick - The present Global Tick,
*                    hiIndex,
*                    hiPduContainerTick - The Tick of the container being used,
*                    internalCellIndex - Cell-Index used at MAC
* Outputs       :    None
* Returns       :    None
* Description   :    This function will read the ack queue and free the HARQ process 
*                    and resources booked in advance. It will also create the HI PDUs. 
******************************************************************************************************/
 void processULAckInPUSCH(
                                UInt8 ulDelay,
                                /* SPR 2446 Fix Begins */
                                /* SPR 2446 Fix End */
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
                                InternalCellIndex internalCellIndex)
                                /*CA Changes end  */
{
    UInt8  ackQContainerTick =0;
    UInt32 ackQueueCount = 0;
    UInt16 tempUeIndex=0;
    UInt8 harqIndex = INVALID_HARQ_ID;     
    /* SPR 15909 fix start */
    tickType_t tempPhyTti = 0;
    /* SPR 15909 fix end */
    ContainerAckQueueNode* ackNode_p = PNULL;
    ULUEContext* ulUEContext_p = PNULL;
    ULHarqInfo*   tempULHarqInfo_p = PNULL;     
    HIPduArray* tempHIPdu_p  = PNULL;
    UplinkContainer* ulContainerQueue_p = PNULL;     
    ULUEContextInfo* ulUEContextInfo_p = PNULL;

    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */
    /*SPS + TTIB fix Start*/
#ifdef FDD_CONFIG
    UInt8 isSPSEnabled = 0;
#endif
    /*SPS + TTIB fix Ends*/
    /*QOS_KLOCKWORK_FIX*/
#ifdef LOG_PRINT_ENABLED    
    UInt8 scheduledTTI = 0;
#endif
    /*QOS_KLOCKWORK_FIX*/
    /* SPR 15909 fix start */
    tickType_t prevPhyTti = 0;
    /* SPR 15909 fix end */
#ifdef LOG_PRINT_ENABLED
    UInt32 ackRecvdCount=0x0;
#endif    

    /*
       We now handle the Ack received in the UpLink channel
     */
#ifdef TDD_CONFIG
    UInt8 packetSubframeNum = 0;
    UInt8 hiPduSubframeNum = 0;
    /*confirm regarding ulDelay */
    ackQContainerTick = ((currentGlobalTick  + ulDelay)% getMaxULCrcContainer(internalCellIndex));
#elif FDD_CONFIG
    ackQContainerTick = MODULO_EIGHT((currentGlobalTick  + ulDelay - 4));
#endif
    /*CA Changes start  */
    ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + ackQContainerTick;
    /*CA Changes end  */
    ackQueueCount = sQueueCount(&ulContainerQueue_p->ackQueue);

    /*CA Changes start  */
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + hiPduContainerTick;
    /*CA Changes end  */

    /* 
       This while loop runs for the entire ackQueueCount and creates the HI PDU for the 
       corresponding ACK received this will also realease the resources related to UL HARQ
     */


    LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "[%s] ackQueueCount = %d \n",__func__,ackQueueCount);
#ifdef LOG_PRINT_ENABLED
    ackRecvdCount=ackQueueCount;
#endif  
    while(ackQueueCount--)
    {
        ackNode_p = (ContainerAckQueueNode *)popNode(&ulContainerQueue_p->ackQueue);
        if(PNULL != ackNode_p)
        {
            tempUeIndex =  ackNode_p->ueIdx;
            tempPhyTti = ackNode_p->ttiCounter;
            if (prevPhyTti != tempPhyTti)
            {
#ifdef TDD_CONFIG           
                /*getting harq id from subframe map */
                packetSubframeNum = tempPhyTti%MAX_SUB_FRAME;

                LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, 
                        "[%s] [currentGlobalTick %d] Process ACK tempPhyTti [%d] packetSubframeNum[%d] ackQContainerTick[%d]\n",
                        __func__, currentGlobalTick, tempPhyTti, packetSubframeNum, ackQContainerTick);

                if (!isValidULSubFrameForHIPdu(packetSubframeNum, &hiPduSubframeNum , internalCellIndex))
                {
                    ltePanic("Invalid UL subframe [%d]\n", packetSubframeNum);
                }
                /* COVERITY CID 10185 FIX START */
                /*
#elif FDD_CONFIG
harqIndex =  MODULO_EIGHT(tempPhyTti);
                 */
                /* COVERITY CID 10185 FIX END */
#endif
                prevPhyTti = tempPhyTti; 
            }
            /*SPR_3305_FIX_START*/
            if (ackNode_p->tcRntiFlag)
            {
#ifdef FDD_CONFIG
                handleAckNodeForTcrnti(ackNode_p, 
                        ulDelay,
                        hiPduContainerTick,
                        hiIndex,
                        currentGlobalTick,
                        internalCellIndex);
#elif TDD_CONFIG
                /* msg3_retx_tdd_support_start */
                handleAckNodeForTcrntiTdd(ackNode_p, 
                        hiPduContainerTick,
                        currentGlobalTick,
                        internalCellIndex);
#endif
                /* msg3_retx_tdd_support_start */
            }
            else
            {

                ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];
                LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, 
                        "[%s] UE INDEX = %d , UL HARQ ID = %d \n",__func__, tempUeIndex, harqIndex);
                /*
                   We at first check whether the UE under consideration is being deleted by RRC
                   or not, only in the case when the pending delete flag is unset we move ahead.
                 */

                if (!ulUEContextInfo_p->pendingDeleteFlag)
                {                
                    ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
                    /* TTIB code : Get HARQ Id from ackNode/NackNode */
                    harqIndex = ackNode_p->ulHarqProcessId;
                    tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[harqIndex]);
                    /** TTIB_Code End */

                    /*SPS CHANGES */
                    /* In case if HARQ is busy in same tick HI ACKL has been sent from SPS strategy */
                    if(( ulUEContext_p->schType == SPS_SCHEDULING)
#ifdef FDD_CONFIG
                            && (!(ulUEContext_p->ttiBundlingEnabled))
#endif
                      )
                    {
                        freeMemPool((void *)ackNode_p);
                        continue;
                    }
                    /*SPS CHANGES */

                    if (ackNode_p->crcValid)
                    { 
                        /*
                           We now check that for the riLength be non zero i.e we have received the
                           ack for some valid entity. Hence we may proceed ahead.
                         */
                        /* SPR 6690 fix start */
                        if((!tempULHarqInfo_p->riLength) && (DRA_SCHEDULING == ackNode_p->schUsed))
                            /* SPR 6690 fix end */
                        {
                            LTE_MAC_UT_LOG(LOG_WARNING,LOG_PHICH,"ACK received " 
                                    "for  UE Index and non valid  HARQ ID =%d ackQContainerTick %d",
                                    tempUeIndex, harqIndex, ackQContainerTick); 

                            LOG_MAC_MSG(MAC_ACK_ZERO_BAND_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                                    currentGlobalTick,tempUeIndex,harqIndex, \
                                    tempULHarqInfo_p->nextReTransmissionType,\
                                    tempULHarqInfo_p->currentRetransCount,\
                                    ackRecvdCount,\
                                    0.0,0.0,__func__,"ACK_ZERO_BAND");
                            /* SPR 2308 Start */ 
                            /* SPR 16916 FIX START */
#ifdef KPI_STATS
                            freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                            freeULHarqProcess(tempULHarqInfo_p);
#endif
                            /* SPR 16916 FIX END */
                            /* SPR 2308 End */ 

#ifdef FDD_CONFIG
                            updateTTIHarqMap(harqIndex,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
#endif

                        }
                        else
#ifdef FDD_CONFIG                    
                            if (currentGlobalTickWithULDelay <= (tempPhyTti + 4))
#elif TDD_CONFIG
                                if (currentGlobalTickWithULDelay <= (tempPhyTti + hiPduSubframeNum))
#endif
                                {
                                    /*
                                       We now create the HI PDU corresponding to the ack received
                                     */
                                    /*bug_10441_changes start*/
                                    if (ackNode_p->isLastRetx)
                                    {
                                        addEntryTOHIPduArray(
                                                tempULHarqInfo_p,
                                                HI_NACK,
                                                ulUEContext_p->crnti,
                                                /* CR changes start */
#ifdef FDD_CONFIG
                                                ulUEContext_p,
                                                hiIndex,
#endif
                                                /* CR changes end */
                                                tempHIPdu_p,
                                                internalCellIndex
#ifdef TDD_CONFIG
                                                ,(ackNode_p->ttiCounter % MAX_SUB_FRAME)
                                                /* +- SPR 17777 */
#endif
                                                );
                                    }
                                    else
                                    {
                                        /*bug_10441_changes end*/
                                        addEntryTOHIPduArray(
                                                tempULHarqInfo_p,
                                                HI_ACK,
                                                ulUEContext_p->crnti,
                                                /* CR changes start */
#ifdef FDD_CONFIG
                                                ulUEContext_p,
                                                hiIndex,
#endif
                                                /* CR changes end */
                                                tempHIPdu_p,
                                                internalCellIndex
#ifdef TDD_CONFIG
                                                ,(ackNode_p->ttiCounter % MAX_SUB_FRAME)
                                                /* +- SPR 17777 */
#endif
                                                );
                                        /*bug_10441_changes start*/
                                    }
#ifdef LOG_PRINT_ENABLED    
                                    /*bug_10441_changes end*/
                                    LOG_UT(MAC_ACK_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                                            currentGlobalTick,tempUeIndex,harqIndex, \
                                            tempPhyTti,\
                                            tempULHarqInfo_p->nextReTransmissionType,\
                                            scheduledTTI,\
                                            0.0,0.0,__func__,"ACK_FREE");
#endif
                                    /*SPS + TTIB fix Starts*/
                                    /* set hpId[0].harqId to INVALID_HARQ_ID as 
                                       for TTIB UE ,Harq for occasions is changing 
                                       and will be filled at SPS strategy */ 
#ifdef FDD_CONFIG
                                    isSPSEnabled = tempULHarqInfo_p->isSpsEnabled;
#endif
                                    /*SPS + TTIB fix Ends*/
                                    /* SPR 16916 FIX START */
#ifdef KPI_STATS
                                    freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                                    freeULHarqProcess(tempULHarqInfo_p);
#endif
                                    /* SPR 16916 FIX END */
                                    /* SPS_CHG */
                                    /* SPS_CHG */
                                    /*SPR 6690 fix Start */
                                    /* If Harq is TTI Bundled then free HARQ process */

                                    /* SPR 19704 + */
#ifdef FDD_CONFIG
                                    if((ulUEContext_p->ttiBundlingEnabled ) &&
                                       (SPS_DEACTIVATION_INITIATED == ulUEContext_p->spsUlInfo.spsState ))
                                    /* SPR 19704 - */
                                    {
                                        tempULHarqInfo_p->harqStatus = HARQ_PROCESS_IN_USE;
                                    }
                                    else
                                    {
                                        /*SPS + TTIB fix Starts*/
                                        /* HARQ process is not parmanently blocked for SPS occasion,
                                         * different harq process is used for different opportunities.
                                         * so HARQ process is freed on getting ACK and hpId is reset*/
                                        if(isSPSEnabled)
                                        {
                                            ulUEContext_p->spsUlInfo.hpId[0].harqId = INVALID_HARQ_ID;
                                        }
                                        /*SPS + TTIB fix Ends*/
                                        updateTTIHarqMap(harqIndex,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
                                    }
#endif
                                    /* SPS_CHG */
                                }
                                else
                                {
#ifdef LOG_PRINT_ENABLED    
                                    LOG_MAC_MSG(MAC_DELAY_ACK_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                                            currentGlobalTick,tempUeIndex,harqIndex, \
                                            tempPhyTti,\
                                            tempULHarqInfo_p->nextReTransmissionType,\
                                            scheduledTTI,\
                                            0.0,0.0,__func__,"DELAY_ACK_FREE_ULHARQ");
#endif
                                    /* SPR 16916 FIX START */
#ifdef KPI_STATS
                                    freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                                    freeULHarqProcess(tempULHarqInfo_p);
#endif
                                    /* SPR 16916 FIX END */

#ifdef FDD_CONFIG
                                    updateTTIHarqMap(harqIndex,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
#endif
                                }
                    }
                    /*
                       We reach a stage where we handle the CRC invalid case.
                     */
                    else
                    {
                        LOG_MAC_MSG(MAC_INVALID_CRC_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                                currentGlobalTick,tempUeIndex,harqIndex,\
                                tempPhyTti,0,0,0.0,0.0,\
                                __func__,"INVALID_CRC_FREE_ULHARQ");

                        /* SPR 16916 FIX START */
#ifdef KPI_STATS
                        freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                        freeULHarqProcess(tempULHarqInfo_p);
#endif
                        /* SPR 16916 FIX END */
#ifdef FDD_CONFIG
                        updateTTIHarqMap(harqIndex,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
#endif
                    }  
                }
                else
                {
                    LOG_MAC_MSG(MAC_UEINDEX_DELETE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                            currentGlobalTick,tempUeIndex,harqIndex, \
                            tempPhyTti,0,0,\
                            0.0,0.0,__func__,"UEINDEX_DELETE");

                }
            }
            /*SPR_3305_FIX_END*/
            freeMemPool((void *)ackNode_p);
        }
        else
        {
            LOG_MAC_MSG(MAC_INVALID_ACKNODE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                    currentGlobalTick,\
                    0,0,0,0,0,\
                    0.0,0.0,__func__,"INVALID_ACKNODE");

        }
    }
}
/*SPR_3305_FIX_START*/
#ifdef FDD_CONFIG
/*************************************************************************************
* Function Name :    handleAckNodeForTcrnti 
* Inputs        :    ackNode_p - Pointer to the ack node,
*                    ulDelay - The delay of execution legs in terms of the number
*                              subFrames w.r.t. PHY & the current processing being
*                              done at MAC for intended tick at PHY,
*                    hiPduContainerTick - The Tick of the container being used,
*                    hiIndex,
*                    currentGlobalTick - The present Global Tick,
*                    internalCellIndex - Cell-Index used at MAC
* Outputs       :    None
* Returns       :    None
* Description   :    This function will read the ack queue and free the HARQ process 
*                    and resources booked in advance. It will also create the HI PDUs. 
***************************************************************************************/

 void handleAckNodeForTcrnti(ContainerAckQueueNode* ackNode_p, 
        UInt8 ulDelay,
        UInt8 hiPduContainerTick,
        UInt8 hiIndex,
       /* SPR 15909 fix start */
        tickType_t currentGlobalTick, 
      /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    TempCRNTICtx *tempUEContext_p = PNULL;
    ULHarqInfo *   tempULHarqInfo_p = PNULL;     

    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */

    /* FDD_Warning_Fix */
#ifdef TDD_CONFIG
    UInt8 nPhichGrp = 0;
    UInt8 nPhichSeq = 0;
#endif
    /* FDD_Warning_Fix */
    HIPduArray* tempHIPdu_p  = PNULL;
    /* SPR 15909 fix start */
    tickType_t tempPhyTti = ackNode_p->ttiCounter;
    /* SPR 15909 fix end */
#ifdef TDD_CONFIG 
	UInt8 hiPduSubframeNum = 0;
	UInt8 tempSF = currentGlobalTick % MAX_SUB_FRAME;
#endif
#ifdef LOG_PRINT_ENABLED
    UInt8 harqIndex = MODULO_EIGHT(tempPhyTti) ;     
#endif
    UInt16 index = ackNode_p->ueIdx;
    tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[index].tempCRNTICtx_p;
    if (tempUEContext_p != PNULL)
    {
        /*CA Changes start  */
        tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + hiPduContainerTick;
        /*CA Changes end  */
        tempULHarqInfo_p = &(tempUEContext_p->ulHarq);

        /* FDD_Warning_Fix */
#ifdef TDD_CONFIG
        nPhichGrp = tempULHarqInfo_p->nPhichGrp;
        nPhichSeq = tempULHarqInfo_p->nPhichSeq;
        /* FDD_Warning_Fix */
		if(!isValidULSubFrameForHIPdu(tempSF, &hiPduSubframeNum ,internalCellIndex))
		{
			ltePanic("Invalid UL subframe [%d]\n", tempSF);
		}

#endif
        if (ackNode_p->crcValid)
        {  
            /*
               We now check that for the riLength be non zero i.e we have received the
               ack for some valid entity. Hence we may proceed ahead.
               */
            if(!tempULHarqInfo_p->riLength)
            {
                LOG_MAC_MSG(MAC_ACK_ZERO_BAND_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                        currentGlobalTick,index,harqIndex, \
                        tempULHarqInfo_p->nextReTransmissionType,\
                        tempULHarqInfo_p->currentRetransCount,\
                        DEFAULT_INT_VALUE,\
                        0.0,0.0,__func__,"ACK_ZERO_BAND");
                /* SPR 2308 Start */ 
                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */
                /* SPR 2308 End */
            }
            else
#ifdef FDD_CONFIG                    
                if (currentGlobalTickWithULDelay <= (tempPhyTti + 4))
#elif TDD_CONFIG
                    if (currentGlobalTickWithULDelay <= (tempPhyTti + hiPduSubframeNum))
#endif
                    {

                        /*
                           We now create the HI PDU corresponding to the ack received
                           */
                        addEntryTOHIPduArray(
                                tempULHarqInfo_p,
                                HI_ACK,
                                tempUEContext_p->tcRNTI,
#ifdef FDD_CONFIG
                                PNULL,
                                hiIndex,
#endif
                                tempHIPdu_p,
                                internalCellIndex);
                       
                        /*
                           we must handle the case of when allocation in advance might have
                           been made and now as ACK has been received the same has to 
                           revert backed.
                           */
#ifdef TDD_CONFIG
                        releasePhich(tempULHarqInfo_p->sfUsed, nPhichGrp, nPhichSeq, internalCellindex);
#endif

                        /* SPR 16916 FIX START */
#ifdef KPI_STATS
                       freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
                        freeULHarqProcess(tempULHarqInfo_p);
#endif
                        /* SPR 16916 FIX END */
                    }
                    else
                    {
                        LOG_MAC_MSG(MAC_DELAY_ACK_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                                currentGlobalTick,index,harqIndex, \
                                tempPhyTti,\
                                tempULHarqInfo_p->nextReTransmissionType,\
                                DEFAULT_INT_VALUE,\
                                0.0,0.0,__func__,"DELAY_ACK_FREE_ULHARQ");

                        /* SPR 16916 FIX START */
#ifdef KPI_STATS
                        freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
                        freeULHarqProcess(tempULHarqInfo_p);
#endif
                        /* SPR 16916 FIX END */
                    }
        }
        /*
           We reach a stage where we handle the CRC invalid case.
           */
        else
        {
            LOG_MAC_MSG(MAC_INVALID_CRC_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                    currentGlobalTick,index,harqIndex,\
                    tempPhyTti,0,0,0.0,0.0,\
                    __func__,"INVALID_CRC_FREE_ULHARQ");
            /* SPR 16916 FIX START */
#ifdef KPI_STATS
            freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
            freeULHarqProcess(tempULHarqInfo_p);
#endif
            /* SPR 16916 FIX END */
        }  
    }
    else
    {
        LOG_MAC_MSG(MAC_UEINDEX_DELETE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                currentGlobalTick,index,harqIndex, \
                tempPhyTti,0,0,\
                0.0,0.0,__func__,"NullTempContext");
    }
}
#endif
/*SPR_3305_FIX_END*/
/* msg3_retx_tdd_support_start */
#ifdef TDD_CONFIG
/*************************************************************************************
* Function Name :    handleAckNodeForTcrntiTdd 
* Inputs        :    ackNode_p - Pointer to the ack node,
*                    ulDelay - The delay of execution legs in terms of the number
*                              subFrames w.r.t. PHY & the current processing being
*                              done at MAC for intended tick at PHY,
*                    hiPduContainerTick - The Tick of the container being used,
*                    hiIndex,
*                    currentGlobalTick - The present Global Tick                     
*                    internalCellIndex -  cell index used at MAC
* Outputs       :    None
* Returns       :    None
* Description   :    This function will read the ack queue and free the HARQ process 
*                    and resources booked in advance. It will also create the HI PDUs. 
***************************************************************************************/

 void handleAckNodeForTcrntiTdd(ContainerAckQueueNode* ackNode_p, 
        UInt8 hiPduContainerTick,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    TempCRNTICtx *tempUEContext_p = PNULL;
    ULHarqInfo *   tempULHarqInfo_p = PNULL;     

    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick;
    /* SPR 15909 fix end */
    //UInt8 nPhichGrp = 0;
    //UInt8 nPhichSeq = 0;
    HIPduArray* tempHIPdu_p  = PNULL;
    /* SPR 15909 fix start */
    UInt64 tempPhyTti = ackNode_p->ttiCounter;
    /* SPR 15909 fix end */
    UInt8 hiPduSubframeNum = 0;
    UInt8 tempSF = (ackNode_p->ttiCounter)% MAX_SUB_FRAME;
#ifdef LOG_PRINT_ENABLED
    UInt8 harqIndex = MODULO_EIGHT(tempPhyTti) ;     
#endif
    UInt16 index = ackNode_p->ueIdx;
     /*CA Changes start  */
    tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[index].tempCRNTICtx_p;
     /*CA Changes end  */
    if (tempUEContext_p != PNULL)
    {
         /*CA Changes start  */
        tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + hiPduContainerTick;
        /*CA Changes end  */
        tempULHarqInfo_p = &(tempUEContext_p->ulHarq);

        if(!isValidULSubFrameForHIPdu(tempSF, &hiPduSubframeNum ,internalCellIndex))
        {
            ltePanic("Invalid UL subframe [%d]\n", tempSF);
        }

        if (ackNode_p->crcValid)
        {  
            /*
               We now check that for the riLength be non zero i.e we have received the
               ack for some valid entity. Hence we may proceed ahead.
               */
            if(!tempULHarqInfo_p->riLength)
            {
#ifdef LOG_PRINT_ENABLED
                LOG_MAC_MSG(MAC_ACK_ZERO_BAND_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                        currentGlobalTick,index,harqIndex, \
                        tempULHarqInfo_p->nextReTransmissionType,\
                        tempULHarqInfo_p->currentRetransCount,\
                        DEFAULT_INT_VALUE,\
                        0.0,0.0,__func__,"ACK_ZERO_BAND");
#endif
                /* SPR 2308 Start */ 
                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */
                /* SPR 2308 End */
            }
            if (currentGlobalTickWithULDelay <= (tempPhyTti + hiPduSubframeNum))
            {
                /*
                   We now create the HI PDU corresponding to the ack received
                   */
                addEntryTOHIPduArray(
                        tempULHarqInfo_p,
                        HI_ACK,
                        tempUEContext_p->tcRNTI,
                        tempHIPdu_p,
                        /*CA Changes start  */
                        internalCellIndex,
                         /*CA Changes end  */
                        /*TDD Config 0 Changes Start*/
                        tempSF);
                /* +- SPR 17777 */
                        /*TDD Config 0 Changes End*/ 
                /*
                   we must handle the case of when allocation in advance might have
                   been made and now as ACK has been received the same has to 
                   revert backed.
                   */

                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */
            }
            else
            {
#ifdef LOG_PRINT_ENABLED
                LOG_MAC_MSG(MAC_DELAY_ACK_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,index,harqIndex, \
                        tempPhyTti,\
                        tempULHarqInfo_p->nextReTransmissionType,\
                        DEFAULT_INT_VALUE,\
                        0.0,0.0,__func__,"DELAY_ACK_FREE_ULHARQ");
#endif
                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */
            }
        }
        /*
           We reach a stage where we handle the CRC invalid case.
           */
        else
        {
#ifdef LOG_PRINT_ENABLED
            LOG_MAC_MSG(MAC_INVALID_CRC_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                    currentGlobalTick,index,harqIndex,\
                    tempPhyTti,0,0,0.0,0.0,\
                    __func__,"INVALID_CRC_FREE_ULHARQ");
#endif
            /* SPR 16916 FIX START */
#ifdef KPI_STATS
            freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
            freeULHarqProcess(tempULHarqInfo_p);
#endif
            /* SPR 16916 FIX END */
        }  
    }
    else
    {
#ifdef LOG_PRINT_ENABLED
        LOG_MAC_MSG(MAC_UEINDEX_DELETE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                currentGlobalTick,index,harqIndex, \
                tempPhyTti,0,0,\
                0.0,0.0,__func__,"NullTempContext");
#endif
    }
}
#endif
/* msg3_retx_tdd_support_end */
/*********************************************************************************
* Function Name     :  allocatePUSCHResourcesForAdaptiveReTx
* Inputs            :  tempULHarqInfo_p - The pointer to the ULHarqInfo,
*                      requiredRB,
*                      strictRBAllocation,
*                      resourceReserved_p - The pointer to the AllocatedResInfo,
*                      mcsToPrbArray - The pointer to MCSToPrbMapInfo,
*                      subFrameNumber,
*                      msg3Flag,
*                      regionAllocated,
*                      ulDelay,
*                      currentTick,
*                      ulUEContext_p - The pointer to ULUEContext,
*                      internalCellIndex - Cell-Index used at MAC
* Outputs           :  None
* Returns           :  MAC_SUCCESS /MAC_FAILURE
* Description       :  This function will work on per node basis and try to allocate 
*                      resources in advance for the next tick also. 
***********************************************************************************/
STATIC MacRetType allocatePUSCHResourcesForAdaptiveReTx( 
                            ULHarqInfo* tempULHarqInfo_p,
                            UInt8 requiredRB,
                            UInt8 strictRBAllocation,
                            AllocatedResInfo* resourceReserved_p,
                            MCSToPrbMapInfo* mcsToPrbArray,
                            UInt8 subFrameNumber,
                            UInt8 msg3Flag,
                            UInt8 regionAllocated
#ifdef FDD_CONFIG
/*  TTIB_Code Start */
                            ,UInt8 ulDelay
                            /* SPR 15909 fix start */
                            ,tickType_t currentTick
                            /* SPR 15909 fix end */
			                ,ULUEContext* ulUEContext_p
/*  TTIB_Code  End */
#endif
                            /*CA Changes start */
                            ,InternalCellIndex internalCellIndex
                            /*CA Changes end */
                            /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
                            ,UInt8 ulSubFrameNum
#endif
                            /*TDD Config 0 Changes End*/ 

                        )
{
    UInt8 foundRBs = 0;
    UInt8 prbStartIndex = 0;
    /* TTI bundling code changes starts*/
    UInt8 retValue = 0;
    /* TTI bundling code changes ends*/
#ifdef FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t  tickToReserve = 0;
    /* SPR 15909 fix end */
#endif
    /*coverity 97031 +-*/
 /* ICIC changes start */
    /*CA Changes start  */
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + subFrameNumber; 
    /*CA Changes end  */
 /* ICIC changes end */
    /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
    /* Calculate iPhich and phichframe for the given U subframe*/
    UInt8 phichSubFrameNum = 0;
    UInt8 iPhich  =  0;
    UInt8 nDmrs =    MAX_DMRS_VALUE;
    phichSubFrameNum = ulSubFrameNum + getHiPduSubframeNum(ulSubFrameNum,internalCellIndex);
    phichSubFrameNum = phichSubFrameNum % MAX_SUB_FRAME;
    iPhich = calculateIPhichValue(ulSubFrameNum,internalCellIndex);
#endif
/* SPR 21958 PUSCH RAT1 Support Start */
/* SPR 22317 Fix Start */
#ifdef FDD_CONFIG
    UInt8 isUplinkRAT1Supported = 0;
    if(FALSE == msg3Flag)
    {
        isUplinkRAT1Supported = ulUEContext_p->isUplinkRAT1Supported; 
    }
    else
    {
        isUplinkRAT1Supported = 0;
    }
#endif
    /* SPR 22317 Fix End */
    /* SPR 21958 PUSCH RAT1 Support End */
    /*TDD Config 0 Changes End*/ 
    if (( FALSE == msg3Flag) 
            && (FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->freqSelectiveSchUL))
    {
#ifdef FDD_CONFIG
        if(ulUEContext_p->ttiBundlingEnabled)
        {
        tickToReserve = currentTick + TTI_PHICH_TO_DCI_OFFSET;
        prbStartIndex = checkAvailableRBsToReturnLongestSetForTTIB(
                                  &mcsToPrbArray[tempULHarqInfo_p->mcsIndex],
                                  tempULHarqInfo_p->riLength,
                                  &foundRBs,
                                  &rbMapNode_p->availableResourceInfo[regionAllocated],
                                  rbMapNode_p->rbMap,
                                  ulDelay,
                                  tickToReserve,
                                  /* +- SPR 17777 */
                                  internalCellIndex);                                        
        }
        else
#endif
        {
       

        /*TDD Config 0 Changes Start*/
#ifdef FDD_CONFIG            
        prbStartIndex = checkAvailableRBsToReturnLongestSet(
                                  &mcsToPrbArray[tempULHarqInfo_p->mcsIndex],
                                  tempULHarqInfo_p->riLength, 
                                  &foundRBs, 
                                  &rbMapNode_p->availableResourceInfo[regionAllocated],
                                  rbMapNode_p->rbMap
                                  );
#elif TDD_CONFIG
        retValue = checkAvailableRBsToReturnLongestSet(
                                  &mcsToPrbArray[tempULHarqInfo_p->mcsIndex],
                                  tempULHarqInfo_p->riLength, 
                                  &foundRBs, 
                                  &rbMapNode_p->availableResourceInfo[regionAllocated],
                                  rbMapNode_p->rbMap,
                                  &nDmrs,
                                  phichSubFrameNum,
                                  iPhich,
                                  &prbStartIndex,
                                  internalCellIndex
                                  );
#endif
        /*TDD Config 0 Changes End*/

        }
        if ((foundRBs >= tempULHarqInfo_p->riLength)
        /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
            && (MAC_SUCCESS ==  retValue)
#endif
                )
        /*TDD Config 0 Changes End*/    
        {
            /* Need to update this resource information in HARQ */
            /* No need to call reserveAlreadyAvailableResourcesForRetx as we have 
             * already checked the availability of resources
             */
            reserveAlreadyAvailableResources (prbStartIndex, tempULHarqInfo_p->riLength,
                        &rbMapNode_p->availableResourceInfo[regionAllocated],
                        rbMapNode_p->rbMap);
            resourceReserved_p->allocatedLength = tempULHarqInfo_p->riLength;
            resourceReserved_p->allocatedIndex  = prbStartIndex;
            /*CA Changes start  */
            resourceReserved_p->allocatedRiv    = getRIV(prbStartIndex, tempULHarqInfo_p->riLength, internalCellIndex);
            /*CA Changes end  */
            updateULHarqProcess(tempULHarqInfo_p, resourceReserved_p, UPDATE);
            /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
            /* Marking RB and RBG Maps for RAT 1 post allocation */
            setUplinkResourcesAsBusy((rbMapNodeArr_gp[internalCellIndex] + subFrameNumber),
                    prbStartIndex,
                    (prbStartIndex + tempULHarqInfo_p->riLength),
                    0,0,FALSE,internalCellIndex);
#endif
            /* SPR 21958 PUSCH RAT1 Support End */

            /*TDD Config 0 Changes Start*/
            /* Update the ndmrs value available*/
#ifdef TDD_CONFIG
            tempULHarqInfo_p->nDmrs = nDmrs;
#endif
            /*TDD Config 0 Changes End*/
#ifdef FDD_CONFIG
            if(tempULHarqInfo_p->isTTIBundled)
            {
                reserveAlreadyAvailableResForThreeTicks(prbStartIndex,tempULHarqInfo_p->riLength,
                        resourceReserved_p,tickToReserve,
                        ulDelay,ulUEContext_p,
                        ulUEContext_p->userLocationType,
                        internalCellIndex);

                tempULHarqInfo_p->isTTIDataRcvd = FALSE;
                tempULHarqInfo_p->ttiBundleFirstTick = currentTick + ulDelay + 
                                    FDD_HARQ_OFFSET + TTI_PHICH_TO_DCI_OFFSET;
            }
#endif            
 
        }
        else
        {
#ifdef FDD_CONFIG
            if(tempULHarqInfo_p->isTTIBundled)
            {
                tickToReserve = currentTick + TTI_PHICH_TO_DCI_OFFSET;
                retValue = allocateUplinkResourcesForTTIBundle(
                        requiredRB,
                        tempULHarqInfo_p->riLength,
                        resourceReserved_p,
                        (UInt32)strictRBAllocation,
                        &rbMapNode_p->availableResourceInfo[regionAllocated],
                        rbMapNode_p->rbMap
                        ,ulDelay,
			            tickToReserve,
			            ulUEContext_p,
                        internalCellIndex);
            }
            else
#endif
            {
#ifdef TDD_CONFIG
               nDmrs = MAX_DMRS_VALUE;
#endif               
               retValue  = allocateUplinkResources(requiredRB,
                        tempULHarqInfo_p->riLength,
        /* ICIC changes start */
                        resourceReserved_p,
                        (UInt32)strictRBAllocation,
                        &rbMapNode_p->availableResourceInfo[regionAllocated],
                        /* SPR 21958 PUSCH RAT1 Support Start */
                        rbMapNode_p,
                        /* SPR 21958 PUSCH RAT1 Support End */
                        /* ICIC changes end */
                        /*CA Changes start  */
                        internalCellIndex
                        /*CA Changes end  */
                        /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
                        ,&nDmrs
                        ,phichSubFrameNum
                        ,iPhich
#endif
                        /*TDD Config 0 Changes End*/ 
                        /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
                        /* SPR 22317 Fix Start */
                        ,isUplinkRAT1Supported	
                        /* SPR 22317 Fix End */
#endif
                        /* SPR 21958 PUSCH RAT1 Support End */
                       );
            }
            if(MAC_SUCCESS == retValue)
            {
                updateULHarqProcess(tempULHarqInfo_p, resourceReserved_p, UPDATE);
            
            /*TDD Config 0 Changes Start*/
            /* Update the ndmrs value available*/
#ifdef TDD_CONFIG
            tempULHarqInfo_p->nDmrs = nDmrs;
#endif
            /*TDD Config 0 Changes End*/
#ifdef FDD_CONFIG
                if(tempULHarqInfo_p->isTTIBundled)
                {
                    tempULHarqInfo_p->isTTIDataRcvd = FALSE;
                    tempULHarqInfo_p->ttiBundleFirstTick = currentTick + ulDelay + 
                        FDD_HARQ_OFFSET + TTI_PHICH_TO_DCI_OFFSET;
                }
#endif                
            }
            else
            {
                return MAC_FAILURE;
            }
        }
    }
    else
    {
#ifdef FDD_CONFIG
            if(tempULHarqInfo_p->isTTIBundled)
            {
                tickToReserve = currentTick + TTI_PHICH_TO_DCI_OFFSET;
                retValue = allocateUplinkResourcesForTTIBundle(
                        requiredRB,
                        tempULHarqInfo_p->riLength,
                        resourceReserved_p,
                        (UInt32)strictRBAllocation,
                        /* ICIC changes start */
                        &rbMapNode_p->availableResourceInfo[regionAllocated],
                        rbMapNode_p->rbMap
                        ,ulDelay,
                        tickToReserve,
                        ulUEContext_p,
                        internalCellIndex);
            }
            else
#endif
            {
               retValue  = allocateUplinkResources(requiredRB,
                    tempULHarqInfo_p->riLength,
        /* ICIC changes start */
                    resourceReserved_p,
                    (UInt32)strictRBAllocation,
                    &rbMapNode_p->availableResourceInfo[regionAllocated],
                    /* SPR 21958 PUSCH RAT1 Support Start */
                    rbMapNode_p,
                    /* SPR 21958 PUSCH RAT1 Support End */
                    /*CA Changes start  */
                    internalCellIndex
                    /*CA Changes end  */
                        /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
                        ,&nDmrs
                        ,phichSubFrameNum
                        ,iPhich
#endif
                        /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
                        /* SPR 22317 Fix Start */
                        ,isUplinkRAT1Supported	
                        /* SPR 22317 Fix End */
#endif
                        /* SPR 21958 PUSCH RAT1 Support End */
                        );
                        /*TDD Config 0 Changes End*/
            }
        if( MAC_SUCCESS == retValue)
        {
            updateULHarqProcess(tempULHarqInfo_p, resourceReserved_p, UPDATE);

                /*TDD Config 0 Changes Start*/
            /* Update the ndmrs value available*/
#ifdef TDD_CONFIG
            tempULHarqInfo_p->nDmrs = nDmrs;
#endif
            /*TDD Config 0 Changes End*/

#ifdef FDD_CONFIG
            /** TTIB_Code Start */
            if(tempULHarqInfo_p->isTTIBundled)
            {
                tempULHarqInfo_p->isTTIDataRcvd = FALSE;
                tempULHarqInfo_p->ttiBundleFirstTick = currentTick + ulDelay + 
                    FDD_HARQ_OFFSET + TTI_PHICH_TO_DCI_OFFSET;
            }
#endif                
        }
        else
        {
            
#ifdef ULHARQ_TIMER_PROC
            LOG_MAC_MSG(MAC_ADAPTIVE_RES_ALLOC_FAIL_ID,LOGINFO,MAC_UL_HARQ,\
                    0/*coverity 97031 +-*/,\
                    tempULHarqInfo_p->nextReTransmissionType,\
                    requiredRB,\
                    tempULHarqInfo_p->riLength,\
                    subFrameNumber,\
                    (UInt32)strictRBAllocation,\
                    (UDouble32)tempULHarqInfo_p->ulHarqProcessId,
                    0.0,__func__,"ADAPTIVE_RES_ALLOC_FAIL");

#else                        
            LOG_MAC_MSG(MAC_ADAPTIVE_RES_ALLOC_FAIL_ID,LOGINFO,MAC_UL_HARQ,\
                    0/*coverity 97031 +-*/,\
                    tempULHarqInfo_p->nextReTransmissionType,\
                    requiredRB,\
                    tempULHarqInfo_p->riLength,\
                    subFrameNumber,\
                    (UInt32)strictRBAllocation,\
                    (UDouble32)0,
                    0.0,__func__,"ADAPTIVE_RES_ALLOC_FAIL");

#endif                        
            return MAC_FAILURE;
        }
    }
    return MAC_SUCCESS;
}
#ifdef FDD_CONFIG 
/** TTIB_Code Start */
/*************************************************************************************
 * Function Name    : processULNackNonAdaptiveBundleInPUSCH
 * Inputs:            ulDelay - The delay of execution legs in terms of the number
 *                              number subFrames w.r.t. PHY & the current processing 
 *                              being done at MAC for intended tick at PHY,
 *                    rbMapNum,
 *                    currentGlobalTick - The present Global Tick
 *                    numberOfULUEsToBeScheudled_p - Pointer to no. of UE to be Scheduled,
 *                    containerTick - The Tick of the container being used,
 *                    subFrame,
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function will read the TTI Bundle Nack non-Adaptive queue 
 *                    and update the HARQ process, and book the resources for 
 *                    (n+9)in advance. It will also create the HI PDUs. 
 ************************************************************************************/

 void processULNackNonAdaptiveBundleInPUSCH(
                               UInt8 ulDelay,
                               UInt32 rbMapNum, /* SS_S2 */
                               /* SPR 15909 fix start */
                               tickType_t currentGlobalTick,
                               /* SPR 15909 fix end */
                               /* +- SPR 17777 */
                               UInt8 containerTick,
                               UInt8 subFrame,
                               InternalCellIndex internalCellIndex)
{
    UInt8                   localCount          = 0;
    UInt8                   tempHarqProcessId   = INVALID_HARQ_ID; /* Invalid Value*/
    UInt8                   ceOverlapRBs        = 0;
    UInt8                   bundleTick          = 0;
    UInt8                   enqueueTick         = 0;
    UInt8                   hiContainerIndex    = 0;
    UInt8                   spsTick             = 0;
    UInt8                   spsOppPresent       = 0;
    UInt16                  tempUeIndex         = 0;
    UInt32                  tempPUSCHResources[MAX_USER_LOCATION] = {0};
    UInt32                  hiIndex             = 0;
    HIPduArray*             tempHIPdu_p         = PNULL;
    PdcchULOutputInfo*      tempPdcchULOutput_p = PNULL;
    ULUEContext*            ulUEContext_p       = PNULL;
    ULUEContextInfo*        ulUEContextInfo_p   = PNULL;
    ULHarqInfo*             tempULHarqInfo_p    = PNULL;
    RbMapNode*              rbMapNode_p         = PNULL;
    AllocatedResInfo        resourceReserved     = {0};
    DirectIndexingUEInfo*   tempDirectIndexingUEInfo_p = PNULL;
    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick + ulDelay;
#ifdef ULHARQ_TIMER_PROC
    tickType_t                 timerExpiredTTI     = currentGlobalTickWithULDelay  + TTIB_ULHARQ_TIMER_TTI_RETX_OFFSET;
    /* SPR 15909 fix end */
     UInt32                 ulHarqSysFrameNum   = 0;
     UInt32                 ulHarqSubFrameNum   = 0;
     UInt8                  timerExpsubFrameNum = 0;   

     /*CA Changes start  */
     getSFAndSFN(&ulHarqSysFrameNum, &ulHarqSubFrameNum, internalCellIndex);            
     /*CA Changes end  */
     timerExpsubFrameNum = subFrame + ulDelay  + TTIB_ULHARQ_TIMER_TTI_RETX_OFFSET ;
     ulHarqSysFrameNum = MODULO_ONEZEROTWOFOUR(ulHarqSysFrameNum +  (timerExpsubFrameNum / MAX_NUM_OF_SUBFRAME));
     timerExpsubFrameNum = timerExpsubFrameNum % MAX_NUM_OF_SUBFRAME;
#endif
    hiIndex = ( (currentGlobalTickWithULDelay + FDD_DATA_OFFSET + TTIB_ULHARQ_TIMER_TTI_RETX_OFFSET 
                            ) % MAX_PHICH_CONTAINER );
    /*CA Changes start  */
    tempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    hiContainerIndex = (currentGlobalTickWithULDelay) % MAX_PDCCH_CONTAINER;
    /*CA Changes start  */
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + hiContainerIndex;
    /*CA Changes end  */

    rbMapNum = MODULO_SIXTEEN(currentGlobalTickWithULDelay + FDD_HARQ_OFFSET + TTI_PHICH_TO_DCI_OFFSET);
    /*CA Changes start  */
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */

    /* First serve the Non Adaptive UEs */
    localCount = tempPdcchULOutput_p->pdcchULNackTtiBundledUEInfo.nonAdaptiveCount;
    tempPUSCHResources[CC_CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter; 
    tempPUSCHResources[CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter; 

    LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "[%d] \n########\n containerTick = %d "
            " tempPdcchULOutput_p->pdcchULNackTtiBundledUEInfo.count = %d \n"
        " tempPUSCHResources = %d  rbMapNum = %d hiIndex %d \n"
            " numberOfULUEsToBeScheudled_p = %u\n########\n"
            ,getCurrentTick(),containerTick, tempPdcchULOutput_p->pdcchULNackTtiBundledUEInfo.nonAdaptiveCount, 
        tempPUSCHResources, rbMapNum,hiIndex,*numberOfULUEsToBeScheudled_p); /* SS_S2 */
    while(localCount--)
    {
        tempUeIndex = \
                      tempPdcchULOutput_p->pdcchULNackTtiBundledUEInfo.pdcchULNackNonAdaptiveUEArray[localCount];

        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];         
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "\ntempUeIndex = %d",tempUeIndex);

        if (!ulUEContextInfo_p->pendingDeleteFlag)
        {
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
            /* SPS TTIB changes */
            /* Don't schedule normal Retx if SPS occasion is to be scheduled in current & next 3 ticks */
            for (bundleTick = 0 ; bundleTick < TTI_BUNDLE_SIZE; bundleTick++)
            {
                spsTick =  (currentGlobalTick + ulDelay + bundleTick + TTI_PHICH_TO_DCI_OFFSET )%MAX_SPS_INTERVAL ;
                if(( SPS_ACTIVATED == ulUEContext_p->spsUlInfo.spsState)
                    || ( SPS_ACTIVATION_INITIATED == ulUEContext_p->spsUlInfo.spsState)
                    || ( SPS_DEACTIVATION_INITIATED == ulUEContext_p->spsUlInfo.spsState)
                        || ( SPS_DEACTIVATED == ulUEContext_p->spsUlInfo.spsState))
                {
                    if ((spsTick == ulUEContext_p->spsUlInfo.actTtiIndex ) ||
                        ((SPS_INTERVAL_20 == ulUEContext_p->spsUlInfo.spsUlSetupInfo_p->semiPersistentSchedIntervalUL)
                            && (spsTick == (ulUEContext_p->spsUlInfo.actTtiIndex + SPS_INTERVAL_20)%MAX_SPS_INTERVAL)) )
                    {
                        spsOppPresent = TRUE;
                        break;
                    }
                    if( (SPS_DEACTIVATION_INITIATED == ulUEContext_p->spsUlInfo.spsState ) &&
                         (spsTick == (ulUEContext_p->spsUlInfo.ttiIndex)  % MAX_SPS_INTERVAL))
                    {
                        spsOppPresent = TRUE;
                         break;
                    }
                }
            }        
            /* SPS TTIB changes */

           ulUEContext_p->isPdcchAllocated = 0;
            tempDirectIndexingUEInfo_p = \
                                         &(tempPdcchULOutput_p->directIndexingUEInfo[tempUeIndex]); 
            tempHarqProcessId = tempDirectIndexingUEInfo_p->harqIndex;

            /* MEAS_GAP_CHG */
            tempULHarqInfo_p = \
                               &(ulUEContext_p->ulresInfo[tempHarqProcessId]);
              /* Sending forced Ack(instead of Nack) for bundled
                *              Data pending retransmisions, if UE is TTI bundling 
                                 Disabled  */
            if(!(ulUEContext_p->ttiBundlingEnabled) ||  (TRUE == spsOppPresent))
            {
                addEntryTOHIPduArray(
                        &ulUEContext_p->ulresInfo[tempHarqProcessId],
                        HI_ACK,
                        ulUEContext_p->crnti,
                        ulUEContext_p,
                        tempULHarqInfo_p->sfUsed,
                        tempHIPdu_p,
                        internalCellIndex);

                LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, 
                "[%s] [%d] Sending forced Ack(instead of Nack) for bundled Data recieved"\
                "if UE is TTI bundling disabled ,freeing Harq process %d.\n",__func__,getCurrentTick(),tempHarqProcessId);
                
                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */
                updateTTIHarqMap(tempHarqProcessId,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
                continue;
            }

            if ((MEAS_GAP_RE_TX_NO_CCE_NO_GRANT == tempDirectIndexingUEInfo_p->flag) || 
                    /* 5798 Fix Start */
                    (MAX_UE_SCHEDULED_NO_RETX_NO_GRANT == tempDirectIndexingUEInfo_p->flag))
            {
                /* 5798 Fix End */
#ifdef ULHARQ_TIMER_PROC
                ulHarqTimerStart(tempUeIndex, tempHarqProcessId,
                        timerExpsubFrameNum, 
                        /* + SPS_TDD_Changes */
                        DRA_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        timerExpiredTTI,ulHarqSysFrameNum,
                        /*CA Changes start  */
                        internalCellIndex);
                        /*CA Changes end  */
#endif
                /* + SPR 20701 */
                tempULHarqInfo_p->currentRetransCount = tempULHarqInfo_p->currentRetransCount + TTI_BUNDLE_SIZE;
                /* - SPR 20701 */
                /* should numGrantsAllocated,stats be updated */
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                /* 5798 Fix Start */
                ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;
                /* 5798 Fix End */
                /*mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->
                  ulCCEIndex].dciPduInfo.aggregationLvl,
                  (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->
                  ulCCEIndex])); */

                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                addEntryTOHIPduArray(
                        &ulUEContext_p->ulresInfo[tempHarqProcessId],
                        /* SPR 5467 changes start*/                      
                        HI_ACK,
                        /* SPR 5467 changes end*/
                        ulUEContext_p->crnti,
                        /* CR changes start */
                        ulUEContext_p,
                        tempULHarqInfo_p->sfUsed,
                        /* CR changes end */
                        tempHIPdu_p,
                        internalCellIndex);

                /* CR changes start */
                /*CA Changes start  */
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                    ~(ONE << tempULHarqInfo_p->nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                tempULHarqInfo_p->sfUsed = hiIndex;
                /*CA Changes end  */
                /* CR changes end */
                continue;
            }
            /* MEAS_GAP_CHG */
            /*++ SPR 19297 */
            /*
               We now create the HI PDU corresponding to the Nack received
               */

            addEntryTOHIPduArray(
                    &ulUEContext_p->ulresInfo[tempHarqProcessId],
                    HI_NACK,
                    ulUEContext_p->crnti,
                    ulUEContext_p,
                    tempULHarqInfo_p->sfUsed,
                    tempHIPdu_p,
                    internalCellIndex);
            /*-- SPR 19297 */
            /*
               We at first check whether the UE has completed it's NACK re
               transmission limit, if yes we will free the HARQ Process and
               continue to the next UE.
               */
            if (tempDirectIndexingUEInfo_p->flag == RECEIVED_NACK_RE_TRANS_COMPELETE)
            {
                /*
                 * If a UL MAX RTXN Complete then the
                 * corresponding  nPhichSeq th bit from the LBS of the array
                 * phichSequenceNoPerPhichGroup_g[Subframe No][  ] 
                 * should be made to set.
                 */
                /* CR changes start */

                /* CR changes end */
                LOG_MAC_MSG(MAC_NACK_RETX_COMP_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        0,0,0,0.0,0.0,__func__,"NACK_RETX_COMP_FREE");

                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */

                updateTTIHarqMap(tempHarqProcessId,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
                /*Qos Changes*/
                restoreTokensOfNonTransmittedLCs(ulUEContext_p, 
                        tempULHarqInfo_p->ulHarqProcessId);
                /*Qos Changes*/
                tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                continue;  
            } 
            /* ICIC changes start */
            if (tempPUSCHResources[CC_CE_REGION] >= tempULHarqInfo_p->riLength)
                /* ICIC changes end */
            {
                /*We use 0 for Retransmission */ 
                ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;

                /* SPR 10027 FIX START */
                /* Check if isDrbPresent = TRUE and Update the lastScheduledTick */
                CHECK_AND_UPDATE_LAST_SCHEDULED_TICK(
                        tempULHarqInfo_p->isDrbPresent,ulUEContext_p)
                /* SPR 10027 FIX END */

                /* ICIC changes start */
                tempPUSCHResources[CC_CE_REGION] -= reserveAlreadyAvailableResourcesForBundleRetx( \
                        tempULHarqInfo_p->riStart, \
                        tempULHarqInfo_p->riLength,\
                        &rbMapNode_p->availableResourceInfo[CC_CE_REGION],\
                        /* +- SPR 17777 */
                        ulDelay,
                        currentGlobalTick,
                        internalCellIndex);
                /* CC resouces are updated always irrespective of user location,
                 * we check here whether the RBs lied in CE region and update the 
                 * resouce info if necessary*/
                rbMapNum = MODULO_SIXTEEN(currentGlobalTickWithULDelay + FDD_HARQ_OFFSET + TTI_PHICH_TO_DCI_OFFSET);
                for(bundleTick = 0; bundleTick < TTI_BUNDLE_SIZE; bundleTick++)
                {
                    /*CA Changes start  */
                    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + (MODULO_SIXTEEN(rbMapNum + bundleTick));
                    /* update CEAvailRbMap for all the four ticks*/
                    ceOverlapRBs = updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                            tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength, internalCellIndex);
                    /*CA Changes end  */
                    /* update variable tempPUSCHResources[] for current RbMap and not for rest three*/
                    if((0 < ceOverlapRBs) && (0 == bundleTick))
                    {
                        tempPUSCHResources[CE_REGION] -= ceOverlapRBs;
                    }
                }
            }
            else
            {
                tempPUSCHResources[CC_CE_REGION]  = 0;
            }

            /*CA Changes start  */
            phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                ~(ONE << tempULHarqInfo_p->nPhichSeq);
            phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
            /*CA Changes end  */
            tempULHarqInfo_p->sfUsed = hiIndex;
            /* CR changes end */
            /* Required resources are not available, hence increase the current
             * ReTx count and start the UL Timer */
            updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
            if(tempULHarqInfo_p->isTTIBundled)
            {
                tempULHarqInfo_p->isTTIDataRcvd = FALSE;
                tempULHarqInfo_p->ttiBundleFirstTick = currentGlobalTickWithULDelay +
                                    FDD_HARQ_OFFSET + TTI_PHICH_TO_DCI_OFFSET;
            }
            ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;

#ifdef ULHARQ_TIMER_PROC
            ulHarqTimerStart(tempUeIndex,tempHarqProcessId,timerExpsubFrameNum,
                    /* + SPS_TDD_Changes */
                    DRA_SCHEDULING,
                    /* - SPS_TDD_Changes */
                    timerExpiredTTI,ulHarqSysFrameNum,
                    /*CA Changes start  */
                    internalCellIndex);
                    /*CA Changes end  */
#endif 
            if(ulUEContext_p->ttiBundlingEnabled && tempULHarqInfo_p->isTTIBundled)
            {

                enqueueTick = currentGlobalTickWithULDelay + TTI_PHICH_TO_DCI_OFFSET;
                enqueueNodesInTTIBScheduleQ(enqueueTick,tempUeIndex,tempHarqProcessId,0,FALSE,internalCellIndex);
            }

            tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK;
        }
    }

}
/******************************************************************************
 * Function Name    : processULNackAdaptiveBundleInPUSCH
 * Inputs           : ulDelay - The delay of execution legs in terms of the number
 *                              number subFrames w.r.t. PHY & the current processing 
 *                              being done at MAC for intended tick at PHY,\
 *                    rbMapNum,
 *                    currentGlobalTick - The present Global Tick,
 *                    numberOfULUEsToBeScheudled_p - Pointer to no. of UL UE's to be
 *                                                   scheduled,
 *                    containerTick - The Tick of the container being used,
 *                    subFrame,
 *                    locType - location type,
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description:       This function will read the TTI Bundle Nack Adaptive queue 
 *                    and update the HARQ process, and book the resources for 
 *                    (n+9)in advance. It will also create the HI PDUs. 
 ******************************************************************************/

 void processULNackAdaptiveBundleInPUSCH(
    UInt8 ulDelay,
    UInt32 rbMapNum, /* SS_S2 */
    /* SPR 15909 fix start */
    tickType_t currentGlobalTick,
    /* SPR 15909 fix end */
    /* +- SPR 17777 */
    UInt8 containerTick,
    UInt8 subFrame,
    UInt8 locType,
    InternalCellIndex internalCellIndex)
{
    UInt8                   resAllocFailure             = 0;
    UInt8                   ceOverlapRBs                = 0;
    UInt8                   tempHarqProcessId           = INVALID_HARQ_ID; /* Invalid Value*/
    UInt8                   localCount                  = 0;
    UInt8                   regionAllocated             = CC_CE_ANY_REGION;
    /* SPR 15909 fix start */
    tickType_t               enqueueTick                 = 0;
    /* SPR 15909 fix end */
    UInt8                   hiContainerIndex            = 0;
    UInt8                   bundleTick                  = 0;
    UInt8                   spsTick                     = 0;
    UInt8                   spsOppPresent               = 0;
    UInt16                  tempUeIndex                 = 0;
    SInt32                  regionPowerMask             = INVALID_UL_POWER_MASK;
    UInt32                  ueMinPowerFlag              = 0;
    UInt32                  ueMaxPowerFlag              = 0;
    UInt32                  hiIndex                     = 0;
    UInt32                  tempPUSCHResources[MAX_USER_LOCATION] = {0};
    HIPduArray*             tempHIPdu_p                 = PNULL;
    AllocatedResInfo        resourceReserved            = {0};
    DirectIndexingUEInfo*   tempDirectIndexingUEInfo_p  = PNULL;
    DciCCEContainerInfo*    dciCCEContainerInfo_p       = PNULL;
    DciCCEInfo*             tempDciCCEInfo_p            = PNULL;
    PdcchULOutputInfo*      tempPdcchULOutput_p         = PNULL;
    ULUEContext*            ulUEContext_p               = PNULL;
    ULUEContextInfo*        ulUEContextInfo_p           = PNULL;
    ULHarqInfo*             tempULHarqInfo_p            = PNULL;
    /* Initialized by any region and changed in case of Cell edge user 
     * as CE_REGION_PREFERRED if configured so */
    RbMapNode*              rbMapNode_p                 = PNULL;
    MacRetType              retVal                      = MAC_FAILURE;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    /*CA Changes start  */
    ULPowerMaskInfo         *ulPowerMaskInfo_p          = ulPowerMaskInfo_gp[internalCellIndex] + ulPowerMaskIndex_g[internalCellIndex];
    /*CA Changes end  */

   /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick + ulDelay;

#ifdef ULHARQ_TIMER_PROC
     tickType_t             timerExpiredTTI     =   currentGlobalTickWithULDelay  + TTIB_ULHARQ_TIMER_TTI_RETX_OFFSET;
     /* SPR 15909 fix end */
     UInt32                 ulHarqSysFrameNum   = 0;
     UInt32                 ulHarqSubFrameNum   = 0;
     UInt8                  timerExpsubFrameNum = 0;   

     /*CA Changes start  */
     getSFAndSFN(&ulHarqSysFrameNum, &ulHarqSubFrameNum, internalCellIndex);            
     /*CA Changes end  */
     timerExpsubFrameNum = subFrame + ulDelay  + TTIB_ULHARQ_TIMER_TTI_RETX_OFFSET ;
     ulHarqSysFrameNum = MODULO_ONEZEROTWOFOUR(ulHarqSysFrameNum +  (timerExpsubFrameNum / MAX_NUM_OF_SUBFRAME));
     timerExpsubFrameNum = timerExpsubFrameNum % MAX_NUM_OF_SUBFRAME;
#endif
    hiIndex = ( (currentGlobalTickWithULDelay + FDD_DATA_OFFSET + TTIB_ULHARQ_TIMER_TTI_RETX_OFFSET 
                            ) % MAX_PHICH_CONTAINER );

    /*CA Changes start  */
    tempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */

    hiContainerIndex = (currentGlobalTickWithULDelay ) % MAX_PDCCH_CONTAINER;
    /*CA Changes start  */
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + hiContainerIndex;
    /*CA Changes end  */
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;  /* For DCI at n+5 */
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];


    rbMapNum = MODULO_SIXTEEN(currentGlobalTickWithULDelay  + FDD_HARQ_OFFSET + TTI_PHICH_TO_DCI_OFFSET);
    /*CA Changes start  */
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    /* Available PUSCH resources are fetched for each location type */
    tempPUSCHResources[CC_CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter; 
    tempPUSCHResources[CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter; 

    localCount = tempPdcchULOutput_p->pdcchULNackTtiBundledUEInfo.adaptiveCount[locType];
    while(localCount--)
    {
        tempUeIndex =
            tempPdcchULOutput_p->pdcchULNackTtiBundledUEInfo.pdcchULNackAdaptiveUEArray[locType][localCount];
        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];         

        /* SPR 11548 Fix Start */
        /* resetting the resAllocFailure to FALSE */ 
        resAllocFailure = FALSE;
        /* SPR 11548 Fix End */

        if (!ulUEContextInfo_p->pendingDeleteFlag)
        {
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
            ulUEContext_p->isPdcchAllocated = 0;
            /* SPS TTIB changes */

            /* Don't schedule normal Retx if SPS occasion is to be scheduled in current & next 3 ticks */
            for (bundleTick = 0 ; bundleTick < TTI_BUNDLE_SIZE; bundleTick++)
            {
                spsTick =  (currentGlobalTick + ulDelay + bundleTick + TTI_PHICH_TO_DCI_OFFSET )%MAX_SPS_INTERVAL ;
                if(( SPS_ACTIVATED == ulUEContext_p->spsUlInfo.spsState)
                     || ( SPS_ACTIVATION_INITIATED == ulUEContext_p->spsUlInfo.spsState)
                      || ( SPS_DEACTIVATION_INITIATED == ulUEContext_p->spsUlInfo.spsState)
                           || ( SPS_DEACTIVATED == ulUEContext_p->spsUlInfo.spsState))
                {
                    if((spsTick == ulUEContext_p->spsUlInfo.actTtiIndex) ||
                        ((SPS_INTERVAL_20 == ulUEContext_p->spsUlInfo.spsUlSetupInfo_p->semiPersistentSchedIntervalUL)
                            && (spsTick == (ulUEContext_p->spsUlInfo.actTtiIndex + SPS_INTERVAL_20)%MAX_SPS_INTERVAL)) )
                    {
                        spsOppPresent = TRUE;
                        break;
                    }
                    if( (SPS_DEACTIVATION_INITIATED == ulUEContext_p->spsUlInfo.spsState ) &&
                         (spsTick == (ulUEContext_p->spsUlInfo.ttiIndex)  % MAX_SPS_INTERVAL))
                    {
                        spsOppPresent = TRUE;
                         break;
                    }
                }
            }        
            /* SPS TTIB changes */

            tempDirectIndexingUEInfo_p = \
                                         &(tempPdcchULOutput_p->directIndexingUEInfo[tempUeIndex]); 
            tempHarqProcessId = tempDirectIndexingUEInfo_p->harqIndex;
            /* MEAS_GAP_CHG */
            tempULHarqInfo_p = \
                               &(ulUEContext_p->ulresInfo[tempHarqProcessId]);

            /* TTI Bundling changes starts (to send forced ACK)*/
            /* Need not send DCI when UE is bundled and current HARQ process is for 
             * non bundled Transmission
             In this case transmission will be forcefully ACKed*/

            if( !(ulUEContext_p->ttiBundlingEnabled ) || (TRUE == spsOppPresent))
            {
                addEntryTOHIPduArray(
                    tempULHarqInfo_p,
                    HI_ACK,
                    ulUEContext_p->crnti,
                    ulUEContext_p,
                    tempULHarqInfo_p->sfUsed,
                    tempHIPdu_p,
                    internalCellIndex);

                /* SPR 20929 FIX Start */
                if ((tempDirectIndexingUEInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                        (INVALID_CCE_INDEX != tempDirectIndexingUEInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
                {
                    mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].\
                            dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex]),
                            internalCellIndex);
                    LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "[%s] [%d] INVALID CCE INDEX for UE [%d].\n",__func__,
                                ulUEContext_p->ueIndex,getCurrentTick());
                }
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, 
                "[%s] [%d] Sending forced Ack(instead of Nack) for bundled Data recieved"\
                "if UE is TTI bundling disabled,freeing Harq process %d.\n",__func__,getCurrentTick(),tempHarqProcessId);
               
                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */

                updateTTIHarqMap(tempHarqProcessId,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
                continue;
            }

            if ((MEAS_GAP_RE_TX_NO_CCE_NO_GRANT ==
                    tempDirectIndexingUEInfo_p->flag)
                /* FORCED_ACK SPR#9374 Changes Start */
                    ||(tempDirectIndexingUEInfo_p->flag == ADAPTIVE_RETRANS_NO_CCE))
                /* FORCED_ACK SPR#9374 Changes End */
            {
                lteWarning("should not come here: NO_CCE_NO_Grant + Adaptive");
#ifdef ULHARQ_TIMER_PROC
                ulHarqTimerStart(tempUeIndex, tempHarqProcessId,timerExpsubFrameNum,
                        /* + SPS_TDD_Changes */
                        DRA_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        timerExpiredTTI,ulHarqSysFrameNum,
                        /*CA Changes start  */
                        internalCellIndex);
                        /*CA Changes end  */
#endif
                /* + SPR 20701 */
                tempULHarqInfo_p->currentRetransCount = tempULHarqInfo_p->currentRetransCount + TTI_BUNDLE_SIZE;
                /* - SPR 20701 */

                /* should numGrantsAllocated,stats be updated */                 
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;

                /* +COVERITY 10718 */
                /* SPR 20929 FIX Start */
                if ((tempDirectIndexingUEInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                        (INVALID_CCE_INDEX != tempDirectIndexingUEInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
                {
                    mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->
                            ulCCEIndex].dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->
                             ulCCEIndex]), internalCellIndex);
                }
                /* -COVERITY 10718 */
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                addEntryTOHIPduArray(
                        &ulUEContext_p->ulresInfo[tempHarqProcessId],
                        /* SPR 5467 changes start*/
                        HI_ACK,
                        /* SPR 5467 changes end*/
                        ulUEContext_p->crnti,
                        /* CR changes start */
                        ulUEContext_p,
                        tempULHarqInfo_p->sfUsed,                        
                        /* CR changes end */
                        tempHIPdu_p,
                        internalCellIndex);
                /* CR changes start */
                /*CA Changes start  */
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                    ~(ONE << tempULHarqInfo_p->nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                /*CA Changes end  */
                tempULHarqInfo_p->sfUsed = hiIndex;
                /* CR changes end */
                continue;
            }
            /* MEAS_GAP_CHG */

            /*
               We now create the HI PDU corresponding to the Nack received
               */
            addEntryTOHIPduArray(
                    tempULHarqInfo_p,
                    HI_ACK,
                    ulUEContext_p->crnti,
                    ulUEContext_p,
                    tempULHarqInfo_p->sfUsed,
                    tempHIPdu_p,
                    internalCellIndex);

            /* ICIC changes start */
            /* Incase of Cell Edge user, we can specify the region to be used as 
             * any of CC/CE region or 
             * CE region to be preferred first, if we want to 
             * allocate with cell edge resource first and then move to cell center if cell edge fails */

            if ((CE_USER == locType) &&  
               (tempPUSCHResources[CE_REGION_PREFERRED] >= tempULHarqInfo_p->riLength))
            {
                    regionAllocated = CE_REGION_PREFERRED; 
            }

            if ((tempPUSCHResources[regionAllocated] >= tempULHarqInfo_p->riLength))
                /* ICIC changes end */
            {
                /* This will allocate resources based on the regionAllocated chosen */
                retVal = allocatePUSCHResourcesForAdaptiveReTx(
                        tempULHarqInfo_p,
                        tempULHarqInfo_p->riLength,
                        tempULHarqInfo_p->strictRBAllocation,
                        &resourceReserved,
                        ulUEContext_p->csiContext.mcsToPrbMap_p,
                        rbMapNum,
                        FALSE,
                        regionAllocated
                        ,ulDelay,
                        currentGlobalTick,
                        ulUEContext_p,
                        internalCellIndex
                        );
                /* This will get executed in case of cell edge user and
                 * if contiguous cell edge resources were less than riLength
                 */
                if((MAC_FAILURE == retVal)&& 
                        CE_REGION_PREFERRED == regionAllocated) 
                {
                    regionAllocated = CC_CE_ANY_REGION;
                    retVal = allocatePUSCHResourcesForAdaptiveReTx(
                            tempULHarqInfo_p,
                            tempULHarqInfo_p->riLength,
                            tempULHarqInfo_p->strictRBAllocation,
                            &resourceReserved,
                            ulUEContext_p->csiContext.mcsToPrbMap_p,
                            rbMapNum,
                            FALSE,
                            regionAllocated
                            ,ulDelay,
                            currentGlobalTick,
                            ulUEContext_p,
                            internalCellIndex
                        );
                }
           
            
                if(MAC_SUCCESS == retVal) 
                    /* SS_S2 */
                {
                    /* Time Averaging changes start */
                    powerCorrectionAndTpc = getTpcForPusch(ulUEContext_p, currentGlobalTickWithULDelay, internalCellIndex);

                    /* +DYNAMIC_ICIC */
                    regionPowerMask = ulPowerMaskInfo_p->powerMask[ulUEContext_p->userLocationType];

                    CHECK_QCI_POWER_MASK_AND_UPDATE_REGION_MASK(ulUEContext_p,regionPowerMask);
                    /* -DYNAMIC_ICIC */

                    validateATB(tempULHarqInfo_p->riLength,\
                            &tempULHarqInfo_p->mcsIndex,\
                            ulUEContext_p, \
                            powerCorrectionAndTpc.powerCorrectionVal, \
                            &ueMaxPowerFlag,&ueMinPowerFlag, \
                            regionPowerMask,RE_TX, internalCellIndex);


                    /* Reset the tpcTriggeredValue in UE Context */
                    ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue = 
                        TPC_NO_CHANGE;

                    if (powerCorrectionAndTpc.powerCorrectionVal)
                    {
                        ulUEContext_p->prevPowerCorrectionForPusch =
                            ulUEContext_p->currPowerCorrectionForPusch;

                        /* Cyclomatic Complexity changes - starts here */
                        checkAndUpdateFiCorrection(
                                   ueMaxPowerFlag,
                                   ueMinPowerFlag,
                                   powerCorrectionAndTpc,
                                   ulUEContext_p);
                        /* Cyclomatic Complexity changes - ends here */

                    }

#ifdef ULHARQ_TIMER_PROC
                    ulHarqTimerStart(tempUeIndex,tempHarqProcessId,timerExpsubFrameNum,
                            /* + SPS_TDD_Changes */
                            DRA_SCHEDULING,
                            /* - SPS_TDD_Changes */
                            timerExpiredTTI,ulHarqSysFrameNum,
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */
#endif 
                    /*We use 0 for Retransmission */ 
                    ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;
                    updateDciCCEInfoArray(
                            ulUEContext_p,
                            tempDirectIndexingUEInfo_p->ulCCEIndex,
                            tempHarqProcessId,
                            tempDciCCEInfo_p,
                            &resourceReserved,
                            powerCorrectionAndTpc.tpcCommand,
                            /* +- SPR 17777 */
                            /* CR changes start */
                            hiIndex, /* SS_S2 */
                            /* CR changes end */
                            tempDirectIndexingUEInfo_p->ulGrantRequestType,
                            INVALID_UE_INDEX, 
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */

                    /* SPR 10027 FIX START */
                    /* Check if isDrbPresent = TRUE and Update the lastScheduledTick */
                    CHECK_AND_UPDATE_LAST_SCHEDULED_TICK(
                            ulUEContext_p->ulresInfo[tempHarqProcessId].isDrbPresent,ulUEContext_p)
                    /* SPR 10027 FIX END */

                    tempPUSCHResources[regionAllocated] -= 
                        resourceReserved.allocatedLength;

                    rbMapNum = MODULO_SIXTEEN(currentGlobalTickWithULDelay + FDD_HARQ_OFFSET + TTI_PHICH_TO_DCI_OFFSET);
                    enqueueTick =  currentGlobalTickWithULDelay + TTI_PHICH_TO_DCI_OFFSET;

                    for(bundleTick = 0; bundleTick < TTI_BUNDLE_SIZE; bundleTick++)
                    {
                        /*CA Changes start  */
                        rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + (MODULO_SIXTEEN(rbMapNum + bundleTick));
                        /*CA Changes end  */
                        if(CE_REGION_PREFERRED == regionAllocated)
                        {
                            if(0 == bundleTick)
                            {
                                tempPUSCHResources[CC_CE_REGION] -= resourceReserved.allocatedLength;
                            }
                            updateCCAvailRes(&rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                                    &resourceReserved);
                        }
                        else
                        {
                            /*CA Changes start  */
                            updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                                    tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                                    internalCellIndex);
                            /*CA Changes end  */

                        }
                    }
                        /* Enqueue nodes in TTI Bundle queue for n+9,+10,+11,+12 */
                    enqueueNodesInTTIBScheduleQ(enqueueTick,tempUeIndex,tempHarqProcessId,0,FALSE,internalCellIndex);
                    resourceReserved.allocatedLength = 0;


                } 
                else
                {
                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_FAIL_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                            currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                            rbMapNum, 
                            tempULHarqInfo_p->nextReTransmissionType,
                            tempULHarqInfo_p->currentRetransCount,
                            0.0,0.0,__func__,"NACK_RES_ALLOC_FAIL");

                    /*
                       We now revert back the  cce allocated 
                       by PDCCH as the allocation was not done
                       */

                    /* SPR 5467 changes start*/
                    /* SPR 14279 Changes Start */
               /* FORCED_ACK SPR#9374 Changes Start */
                   /* tempULHarqInfo_p->currentRetransCount++;*/
               /* FORCED_ACK SPR#9374 Changes End */
                    updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
                    /* SPR 14279 Changes End */
                    tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                    /* SPR 5467 changes end*/

                /* SPR 20929 FIX Start */
                    if ((tempDirectIndexingUEInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                            (INVALID_CCE_INDEX != tempDirectIndexingUEInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
                    {
                        mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].\
                                dciPduInfo.aggregationLvl,
                                (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex]),
                                internalCellIndex); 
                    }
                    tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                    /* reserve the last given resources as UE will assume NON Adaptive
                       ReTx */
                    resAllocFailure = TRUE;
                    /* CC resouces are updated always irrespective of user location,
                     * we check here whether the RBs lied in CE region and update the 
                     * resouce info if necessary*/
                    rbMapNum = MODULO_SIXTEEN( currentGlobalTickWithULDelay + FDD_HARQ_OFFSET + TTI_PHICH_TO_DCI_OFFSET);
                    enqueueTick = currentGlobalTickWithULDelay + TTI_PHICH_TO_DCI_OFFSET;

                    for(bundleTick = 0; bundleTick < TTI_BUNDLE_SIZE; bundleTick++)
                    {
                        /* Enqueue nodes in TTI Bundle queue for n+9,+10,+11,+12 */
                        /*CA Changes start  */
                        ceOverlapRBs = updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                                       tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                                       internalCellIndex);
                        /*CA Changes end  */
                        if(0 < ceOverlapRBs && (0 == bundleTick)) 
                        {
                            tempPUSCHResources[CE_REGION] -= ceOverlapRBs;
                        }
                    }
                    enqueueNodesInTTIBScheduleQ(enqueueTick,tempUeIndex,tempHarqProcessId,0,FALSE,internalCellIndex);

#ifdef ULHARQ_TIMER_PROC
                    ulHarqTimerStart(tempUeIndex,tempHarqProcessId,timerExpsubFrameNum,
                            /* + SPS_TDD_Changes */
                            DRA_SCHEDULING,
                            /* - SPS_TDD_Changes */
                            timerExpiredTTI,ulHarqSysFrameNum,
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */
#endif 
                    /* CR changes start */
                    /*CA Changes start  */
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                        ~(ONE << tempULHarqInfo_p->nPhichSeq);
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                    /*CA Changes end  */
                    tempULHarqInfo_p->sfUsed = hiIndex;
                }
            }
            else
            {
                /*
                   We now revert back the  cce allocated by PDCCH 
                   as no more PUSCH resources are avaliable
                 */
                    /* SPR 5467 changes start*/
                    /* SPR 14279 Changes Start */
                    /* tempULHarqInfo_p->currentRetransCount++; */
                    updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
                    /* SPR 14279 Changes End */
                    tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                    /* SPR 5467 changes end*/

                /* + COVERITY 10718 */
                /* SPR 20929 FIX Start */
                if ((tempDirectIndexingUEInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                        (INVALID_CCE_INDEX != tempDirectIndexingUEInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
                {
                    mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].\
                            dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex]),
                            internalCellIndex); 
                }
                /* - COVERITY 10718 */
                    tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                    resAllocFailure = TRUE;


                /* Required resources are not available, hence increase the current
                 * ReTx count and start the UL Timer */
#ifdef ULHARQ_TIMER_PROC           
                ulHarqTimerStart(tempUeIndex,tempHarqProcessId,timerExpsubFrameNum,
                        /* + SPS_TDD_Changes */
                        DRA_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        timerExpiredTTI,ulHarqSysFrameNum,
                        /*CA Changes start  */
                        internalCellIndex);
                        /*CA Changes end  */
#endif 
                /* CR changes start */
                /*CA Changes start  */
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                    ~(ONE << tempULHarqInfo_p->nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                /*CA Changes end  */
                tempULHarqInfo_p->sfUsed = hiIndex;
                /* CR changes end */
            } 
            /*We use 0 for Retransmission */ 
            ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;

            /* SPR 5467 changes start*/
            if(resAllocFailure != TRUE)
            {
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK;
            }
            /* SPR 5467 changes end*/
        }
    }
}
/* TTIB_Code End */
#endif
/******************************************************************************
 * Function Name    : processULNackNonAdaptiveInPUSCH
 * Inputs           : ulDelay - The delay of execution legs in terms of the number
 *                              number subFrames w.r.t. PHY & the current processing 
 *                              being done at MAC for intended tick at PHY,
 *                    rbMapNum,
 *                    currentGlobalTick - The present Global Tick,
 *                    numberOfULUEsToBeScheudled_p - Pointer to no. of UL UE's to be
 *                                                   scheduled,
 *                    containerTick - The Tick of the container being used,
 *                    hiIndex,
 *                    subFrame, 
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function will read the Nack Adaptive queue and update 
 *                    the HARQ process, and book the resources in advance. It 
 *                    will also create the HI PDUs. 
 ******************************************************************************/
#ifdef FDD_CONFIG
 void processULNackNonAdaptiveInPUSCH(
                               UInt8 ulDelay,
                               UInt32 rbMapNum, /* SS_S2 */
                               /* SPR 15909 fix start */
                               tickType_t currentGlobalTick,
                               /* SPR 15909 fix end */
                               UInt8* numberOfULUEsToBeScheudled_p,
                               UInt8 containerTick,
                               /* CR changes start */
                               UInt8 hiIndex,
                               /* CR changes end */
                               UInt8 subFrame,
                               InternalCellIndex internalCellIndex)
{
    UInt16 tempUeIndex=0;
    HIPduArray* tempHIPdu_p  = PNULL;
    AllocatedResInfo resourceReserved = {0};
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; /* Invalid Value*/
    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p = PNULL;
    PdcchULOutputInfo* tempPdcchULOutput_p = PNULL;
    ULUEContext* ulUEContext_p = PNULL;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    UInt8 localCount = 0;
    ULHarqInfo* tempULHarqInfo_p = PNULL;
    /* ICIC changes start */
    RbMapNode *rbMapNode_p = PNULL;
    UInt32 tempPUSCHResources[MAX_USER_LOCATION] = {0};
    UInt8 ceOverlapRBs = 0;
    /* ICIC changes end */
    UInt16  numberOfInfoPresent = 0;

    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */
    /* SPR 9173 changes start */
    UInt8 ulSubFrameNum  = (currentGlobalTick + FDD_HARQ_OFFSET + ulDelay )% MAX_SUB_FRAME;
    /* SPR 9173 changes end */
#ifdef ULHARQ_TIMER_PROC
    /* SPR 15909 fix start */
    tickType_t timerExpiredTTI = currentGlobalTickWithULDelay  + START_ULHARQ_TIMER_OFFSET;
    /* SPR 15909 fix end */
     /* SPR 4386 changes start */
     UInt8 subFrameNum = (subFrame + ulDelay);
     UInt8 timerExpsubFrameNum = (subFrameNum + START_ULHARQ_TIMER_OFFSET);		
     UInt32 ulHarqSysFrameNum = 0;
     UInt32 ulHarqSubFrameNum = 0;
     /*CA Changes start  */
     getSFAndSFN(&ulHarqSysFrameNum, &ulHarqSubFrameNum, internalCellIndex);             
     /*CA Changes end  */
     /* SPR 4386 changes end */
     if(timerExpsubFrameNum >= MAX_SUB_FRAME)
     {
         timerExpsubFrameNum = timerExpsubFrameNum % MAX_SUB_FRAME;

         ulHarqSysFrameNum++;
         if (ulHarqSysFrameNum == MAX_SFN_VALUE)
         {
             ulHarqSysFrameNum = 0;
         }
     }
#endif
    ULSchUciRachPoolInfo* ulschUciRachInfo_p 
        /*CA Changes start  */
        = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
        /*CA Changes end  */

    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    /*CA Changes start  */
    tempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    /*+ 5406 */    
    TempCRNTICtx *tempUEContext_p;

    /* First serve the Non Adaptive UEs */
    localCount = tempPdcchULOutput_p->pdcchULNackTempUEInfo.nonAdaptiveCount;
    /* ICIC changes start */
    /*CA Changes start  */
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    tempPUSCHResources[CC_CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter; 
    tempPUSCHResources[CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter; 
    /* ICIC changes end */

    while(localCount--)
    {
        tempUeIndex = 
           tempPdcchULOutput_p->pdcchULNackTempUEInfo.pdcchULNackNonAdaptiveUEArray[localCount];

        tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[tempUeIndex].\
                          tempCRNTICtx_p;
        if ((tempUEContext_p != PNULL) && (RAR_SENT == tempUEContext_p->status))
        {
            if (MAC_FAILURE == processULNackForNonAdaptiveReTxOfMsg3(tempUeIndex,
                        containerTick,
                        /* +- SPR 17777 */
                        tempUEContext_p,
                        rbMapNum,
                        numberOfULUEsToBeScheudled_p,
                        &tempPUSCHResources[CC_CE_REGION],
                        timerExpiredTTI,
                        timerExpsubFrameNum,
                        ulHarqSysFrameNum,
                        hiIndex,
                        &numberOfInfoPresent,
                        internalCellIndex))
            {
                continue;
            }
        }
    }

    /* First serve the Non Adaptive UEs */
    localCount = tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount;
    tempPUSCHResources[CC_CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter; 
    tempPUSCHResources[CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter; 
    /*- 5406 */    
    LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "\n########\n containerTick = %d "
            " tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount = %d \n"
        " tempPUSCHResources = %d  rbMapNum = %d\n"
            " numberOfULUEsToBeScheudled_p = %d\n########\n"
            ,containerTick, tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount, 
        tempPUSCHResources, rbMapNum,*numberOfULUEsToBeScheudled_p); /* SS_S2 */
    while(localCount--)
    {
        tempUeIndex = \
                      tempPdcchULOutput_p->pdcchULNackUEInfo.pdcchULNackNonAdaptiveUEArray[localCount];
        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];         
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "\ntempUeIndex = %d",tempUeIndex);

        if (!ulUEContextInfo_p->pendingDeleteFlag)
        {
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
            /* + SPR_17858_209_CHANGES */
            if( !ulUEContext_p )
            {
                //fprintf(stderr, "Adaptive UE context is NULL. ueIndex %d, localCount %d \n ",tempUeIndex,localCount );
                continue;
            }
            /* - SPR_17858_209_CHANGES */


            ulUEContext_p->isPdcchAllocated = 0;
            tempDirectIndexingUEInfo_p = \
                                         &(tempPdcchULOutput_p->directIndexingUEInfo[tempUeIndex]); 
            tempHarqProcessId = tempDirectIndexingUEInfo_p->harqIndex;
            /* MEAS_GAP_CHG */
            tempULHarqInfo_p = \
                               &(ulUEContext_p->ulresInfo[tempHarqProcessId]);
            /* TTIB_Code Starts*/
            /*We use 0 for Retransmission */ 
            //ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;
            if(tempULHarqInfo_p->isTTIBundled)
            {
                /* + SPR 20701 */
                if (MEAS_GAP_RE_TX_NO_CCE_NO_GRANT == tempDirectIndexingUEInfo_p->flag )
                {
                    updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
                    continue;
                }
                /* - SPR 20701 */
                /* For first tick of bundle while retx of bundle,tempULHarqInfo_p is
                            getting updated while allocating resources in map 
                    checking if it is first ULconfig for retx Bundle so no need to update retx count and IRV.*/
                if(currentGlobalTick != (tempULHarqInfo_p->ttiBundleFirstTick - (FDD_HARQ_OFFSET + ulDelay)) )                                
                {
                    updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
                }

                /* Resorces are already reserved in first tick of bundle so only 
                 *      UL config is to be prepared */
                if(ULSCH_UCI_INVALID_INDEX != tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
                {
                    /* Fill UL Harq Info*/
                    fillSemiStaticInfoForULSCH (ulschUciRachInfo_p,
                            tempDirectIndexingUEInfo_p,
                            ulUEContextInfo_p,
                            tempULHarqInfo_p,
                            tempHarqProcessId,
			    /* SPR 9173 changes start */
                            ulSubFrameNum,0, internalCellIndex);
			    /* SPR 9173 changes end */
                }
                else
                {
                    if ( MAX_ULSCH_UCI_SUPPORTED <=  numberOfInfoPresent)
                    {
                        LTE_MAC_UT_LOG(LOG_WARNING, LOG_PHICH, "[%s] OverFlow occured in the ULSchUCIRachPoolInfo pool.\n",__func__);
                        ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                                __func__);
                        break;
                    }
                    /* SPR 10195 Fix Start */
                    /*SPR 7086 Fix Start*/
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS =
                        nSRSDetermineOnCollision(ulUEContext_p, 
                        tempHarqProcessId, ulSubFrameNum,internalCellIndex);
                    /*SPR 7086 Fix End*/
                    /* SPR 10195 Fix End */

                    /* Fill UL Harq Info*/
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p =
                        tempULHarqInfo_p;
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId =
                        tempHarqProcessId;
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType =
                        MAC_SS_PDU_ULSCH;
                    /* + SPS_TDD_Changes */
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti =
                            ulUEContext_p->crnti;
                    /* - SPS_TDD_Changes */

                    numberOfInfoPresent++;
                }
                
                continue;
            }
            else if( ( ulUEContext_p->ttiBundlingEnabled && !(tempULHarqInfo_p->isTTIBundled) ))
            {
                /* Sending forced Ack(instead of Nack) for non-bundled or bundled 
                 *              Data recieved if UE is TTI bundling Enabled or 
                                Disabled respectively */
                addEntryTOHIPduArray(
                        tempULHarqInfo_p,
                        HI_ACK,
                        ulUEContext_p->crnti,
                        ulUEContext_p,
                        tempULHarqInfo_p->sfUsed,
                        tempHIPdu_p,
                        internalCellIndex);
                /* SPR 16605 Fix Start */
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                    ~(ONE << tempULHarqInfo_p->nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                tempULHarqInfo_p->sfUsed = hiIndex;
                /* SPR 16605 Fix End */
                LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, 
                "[%s] [%d] Sending forced Ack(instead of Nack) for non-bundled Data recieved"\
                "if UE is TTI bundling Enabled ,freeing Harq process %d.\n",__func__,getCurrentTick(),tempHarqProcessId);
                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */
                updateTTIHarqMap(tempHarqProcessId,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
                continue;
            }
            /* TTIB_Code Ends*/
            if ((MEAS_GAP_RE_TX_NO_CCE_NO_GRANT == tempDirectIndexingUEInfo_p->flag) || 
                    /* 5798 Fix Start */
                    (MAX_UE_SCHEDULED_NO_RETX_NO_GRANT == tempDirectIndexingUEInfo_p->flag))
            {
                /* + SPR 6864 */
                if (ULSCH_UCI_INVALID_INDEX !=
                        tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
                {   
                      /* + SPR_14994 */
                    checkAndRestoreUCIPduType(ulUEContext_p, tempDirectIndexingUEInfo_p,
                                  ulschUciRachInfo_p,ulSubFrameNum,
                                  /*SPR 19504 fix start*/    
                                  rbMapNum,
                                  internalCellIndex);
                                  /*SPR 19504 fix end*/    
                                  
                     /* - SPR_14994 */
                }
                /* - SPR 6864 */
                /* 5798 Fix End */
#ifdef ULHARQ_TIMER_PROC
                ulHarqTimerStart(tempUeIndex, tempHarqProcessId,
                        timerExpsubFrameNum,
                        /* + SPS_TDD_Changes */
                        DRA_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        timerExpiredTTI,ulHarqSysFrameNum,
                        internalCellIndex);
#endif
                 /* SPR 14279 Changes Start */
                /* tempULHarqInfo_p->currentRetransCount++; */
                updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
                /* SPR 14279 Changes End */

                /* should numGrantsAllocated,stats be updated */
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                /* SPR 17465 FIX +- */
                /* 5798 Fix Start */
                ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;
                /* 5798 Fix End */

                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                /*SPR Fix 17959 start */
                if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                {
                    /*SPR Fix 17959 end */

                addEntryTOHIPduArray(
                        &ulUEContext_p->ulresInfo[tempHarqProcessId],
                        /* SPR 5467 changes start*/                      
                        HI_ACK,
                        /* SPR 5467 changes end*/
                        ulUEContext_p->crnti,
                        /* CR changes start */
                        ulUEContext_p,
                        tempULHarqInfo_p->sfUsed,
                        /* CR changes end */
                        tempHIPdu_p,
                        internalCellIndex);
                    /*SPR Fix 17959 start */
                    tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                }
                /*SPR Fix 17959 end */
                
                /* CR changes start */
                /*CA Changes start  */
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                    ~(ONE << tempULHarqInfo_p->nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                /*CA Changes end  */
                tempULHarqInfo_p->sfUsed = hiIndex;
                /* CR changes end */
                continue;
            }
            /* MEAS_GAP_CHG */
            /*
               We at first check whether the UE has completed it's NACK re
               transmission limit, if yes we will free the HARQ Process and
               continue to the next UE.
               */
            if (tempDirectIndexingUEInfo_p->flag == RECEIVED_NACK_RE_TRANS_COMPELETE)
            {
                /*
                 * If a UL MAX RTXN Complete then the
                 * corresponding  nPhichSeq th bit from the LBS of the array
                 * phichSequenceNoPerPhichGroup_g[Subframe No][  ] 
                 * should be made to set.
                 */
                /* CR changes start */

                /* CR changes end */
                LOG_MAC_MSG(MAC_NACK_RETX_COMP_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        0,0,0,0.0,0.0,__func__,"NACK_RETX_COMP_FREE");
                    /* SPR Fix 17959 start */
                    /* SPR Fix 17959 end */
                /* SPR 16916 FIX START */
/* SPR 19288 change start */
/* SPR 19288 change end */ 
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */
                updateTTIHarqMap(tempHarqProcessId,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
                /*Qos Changes*/
                restoreTokensOfNonTransmittedLCs(ulUEContext_p, 
                        tempULHarqInfo_p->ulHarqProcessId);
                /*Qos Changes*/
                tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                /* SPR 2509 CHG start */
                /* SPR 2509 CHG end */
                continue;  
            } 
            /* ICIC changes start */
            if (tempPUSCHResources[CC_CE_REGION] >= tempULHarqInfo_p->riLength)
                /* ICIC changes end */
            {
                /*We use 0 for Retransmission */ 
                ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;

                /* SPR 10027 FIX START */
                /* Check if isDrbPresent = TRUE and Update the lastScheduledTick */
                CHECK_AND_UPDATE_LAST_SCHEDULED_TICK(
                        tempULHarqInfo_p->isDrbPresent,ulUEContext_p)
                /* SPR 10027 FIX END */
            
               /* SPR 10803 [BUG 848] FIX STRAT */
                MacRetType retVal = MAC_FAILURE;
                /* SPR 21958 PUSCH RAT1 Support Start */
                if(TRUE == tempULHarqInfo_p->RAT1Allocation)
                {
                    retVal = checkNumRBGFromRBGSet1StartIndex(
                            tempULHarqInfo_p->startRbgIndexforSet1,
                            tempULHarqInfo_p->endRbgIndexforSet1,
                            tempULHarqInfo_p->startRbgIndexforSet2,
                            tempULHarqInfo_p->endRbgIndexforSet2,
                            rbMapNum,
                            internalCellIndex); 
                }
                else
                {
                    /* SPR 21958 PUSCH RAT1 Support End */
                retVal = checkResourceAvailability(tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,rbMapNode_p->rbMap);
                /* SPR 21958 PUSCH RAT1 Support Start */
                }
                /* SPR 21958 PUSCH RAT1 Support End */
                if (retVal == MAC_FAILURE)
                {
                     /* SPR 15203 Fix Start */
#ifdef ULHARQ_TIMER_PROC
                    ulHarqTimerStart(tempUeIndex, tempHarqProcessId,
                        timerExpsubFrameNum,
                        /* + SPS_TDD_Changes */
                        DRA_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        timerExpiredTTI,ulHarqSysFrameNum,internalCellIndex);
#endif
                    tempULHarqInfo_p->currentRetransCount++; 
                    ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;
                    /* SPR 15203 Fix End */
                    /* SPR Fix 17959 start */
                    tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                    if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                    {
                        addEntryTOHIPduArray(
                                tempULHarqInfo_p,
                                HI_ACK,
                                ulUEContext_p->crnti,
                                /* CR changes start */
                                ulUEContext_p,
                                tempULHarqInfo_p->sfUsed,
                                /* CR changes end */
                                tempHIPdu_p, 
                                internalCellIndex);
                        tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                    }
                    /*SPR Fix 17959 end */
                    /* SPR 16605 Fix Start */
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                        ~(ONE << tempULHarqInfo_p->nPhichSeq);
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                    tempULHarqInfo_p->sfUsed = hiIndex;
                    /* SPR 16605 Fix End */

                    continue;
                }
                else
                {
                    addEntryTOHIPduArray(
                    tempULHarqInfo_p,
                    HI_NACK,
                    ulUEContext_p->crnti,
                    /* CR changes start */
                    ulUEContext_p,
                    tempULHarqInfo_p->sfUsed,
                    /* CR changes end */
                    tempHIPdu_p,
                    internalCellIndex);
                }

            /* SPR 10803 [BUG 848] FIX END */

                /* SPR 21958 PUSCH RAT1 Support Start */
                /* SPR 22262 Fix Start */
                /* Code moved */
                /* SPR 22262 Fix End */
#ifdef FDD_CONFIG
                /* Marking RB and RBG Maps for RAT 1 post allocation */
                if(TRUE == tempULHarqInfo_p->RAT1Allocation)
                {
                    setUplinkResourcesAsBusy((rbMapNodeArr_gp[internalCellIndex] + rbMapNum),
                            tempULHarqInfo_p->startRbgIndexforSet1,
                            tempULHarqInfo_p->endRbgIndexforSet1,
                            tempULHarqInfo_p->startRbgIndexforSet2,
                            tempULHarqInfo_p->endRbgIndexforSet2,
                            tempULHarqInfo_p->RAT1Allocation,internalCellIndex);

                 /* SPR 22262 Fix Start */
                    UInt8 tempRILength = ((tempULHarqInfo_p->endRbgIndexforSet1 - tempULHarqInfo_p->startRbgIndexforSet1) 
                            * rbgSize_g[internalCellIndex]);
                    
                    tempPUSCHResources[CC_CE_REGION] -= reserveAlreadyAvailableResourcesForRetx( \
                            tempULHarqInfo_p->riStart, \
                            tempRILength,\
                            &rbMapNode_p->availableResourceInfo[CC_CE_REGION],\
                            rbMapNode_p->rbMap);

                    tempRILength = ((tempULHarqInfo_p->endRbgIndexforSet2 - tempULHarqInfo_p->startRbgIndexforSet2) 
                                               * rbgSize_g[internalCellIndex] );

                    tempPUSCHResources[CC_CE_REGION] -= reserveAlreadyAvailableResourcesForRetx( \
                            (tempULHarqInfo_p->startRbgIndexforSet2 * rbgSize_g[internalCellIndex]), \
                            tempRILength,\
                            &rbMapNode_p->availableResourceInfo[CC_CE_REGION],\
                            rbMapNode_p->rbMap);
                 /* SPR 22262 Fix End */
                }
                else
                {
                    setUplinkResourcesAsBusy((rbMapNodeArr_gp[internalCellIndex] + rbMapNum),
                            tempULHarqInfo_p->riStart,
                            (tempULHarqInfo_p->riStart + tempULHarqInfo_p->riLength),
                            0, 0, tempULHarqInfo_p->RAT1Allocation,internalCellIndex);
                    /* SPR 22262 Fix Start */
                /* ICIC changes start */
                tempPUSCHResources[CC_CE_REGION] -= reserveAlreadyAvailableResourcesForRetx( \
                        tempULHarqInfo_p->riStart, \
                        tempULHarqInfo_p->riLength,\
                        &rbMapNode_p->availableResourceInfo[CC_CE_REGION],\
                        rbMapNode_p->rbMap); 
                    /* SPR 22262 Fix End */
                }    
#endif
                    /* SPR 22262 Fix Start */
                /* CC resouces are updated always irrespective of user location,
                 * we check here whether the RBs lied in CE region and update the 
                 * resouce info if necessary*/
                ceOverlapRBs = updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                                        tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                                        internalCellIndex);
                    /* SPR 22262 Fix End */
                /* SPR 21958 PUSCH RAT1 Support End */
                if(0 < ceOverlapRBs)
                {
                    tempPUSCHResources[CE_REGION] -= ceOverlapRBs;
                }
                /* ICIC changes end */
                LOG_MAC_MSG(MAC_NACK_RES_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        rbMapNum,NON_ADAPTIVE_RE_TX,
                        tempULHarqInfo_p->currentRetransCount,
                        tempULHarqInfo_p->riLength,tempULHarqInfo_p->riStart,
                        __func__,"NACK_RES_ULHARQ_PASS");


                LOG_MAC_MSG(MAC_NACK_RES_ALLOC_INFO,LOGDEBUG,MAC_UL_HARQ,\
                        getCurrentTick(),\
                        tempULHarqInfo_p->riStart,
                        tempULHarqInfo_p->riLength,\
                        tempPUSCHResources[CC_CE_REGION],\
                        tempPUSCHResources[CE_REGION],\
                        0,0.0,0.0,\
                        __func__,"NACK_RES_ALLOC");
            }
            else
            {
                /*SPR Fix 17959 start */
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX; 
                if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                {
            /* SPR 10803 [BUG 848] FIX STRAT */
                   addEntryTOHIPduArray(
                    tempULHarqInfo_p,
                    HI_NACK,
                    ulUEContext_p->crnti,
                    /* CR changes start */
                    ulUEContext_p,
                    tempULHarqInfo_p->sfUsed,
                    /* CR changes end */
                    tempHIPdu_p,
                    internalCellIndex);
            /* SPR 10803 [BUG 848] FIX END */
                    tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                }
                /*SPR Fix 17959 end */

                LOG_MAC_MSG(MAC_NACK_RES_ZERO_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        rbMapNum, 
                        tempULHarqInfo_p->nextReTransmissionType,
                        tempULHarqInfo_p->currentRetransCount,
                        0.0,0.0,__func__,"NACK_RES_ZERO_FAIL");

                /* ICIC changes start */
                tempPUSCHResources[CC_CE_REGION]  = 0;
                /* ICIC changes end */
            }
            /* CR changes start */

            /*CA Changes start  */
            phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                ~(ONE << tempULHarqInfo_p->nPhichSeq);
            phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
            /*CA Changes end  */
            tempULHarqInfo_p->sfUsed = hiIndex;
            /* CR changes end */

            LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                    tempUeIndex,
                    currentGlobalTick,
                    NON_ADAPTIVE_RE_TX,
                    tempHarqProcessId,
                    tempULHarqInfo_p->riLength,
                    tempULHarqInfo_p->riStart,
                    tempDirectIndexingUEInfo_p->ulGrantRequestType,
                    tempULHarqInfo_p->deltabsrLoad,
                    FALSE,
                    tempULHarqInfo_p->mcsIndex,
                    0,
                    TPC_VALUE_1 
                     /* UL_MU_MIMO_CHG START */
                     , ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                     tempDirectIndexingUEInfo_p->buddyUeIndex,
                     ulUEContext_p->userLocationType,
                     tempULHarqInfo_p->nDmrs,
                     ulUEContext_p->modulationScheme,
                     /* UL_MU_MIMO_CHG END */
                     /* CA Stats Changes Start */
                     internalCellIndex
                     /* CA Stats Changes End */
                   );
            
            /* Required resources are not available, hence increase the current
             * ReTx count and start the UL Timer */
            updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
            /*We use 0 for Retransmission */ 
            ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;

#ifdef ULHARQ_TIMER_PROC
            ulHarqTimerStart(tempUeIndex,tempHarqProcessId,timerExpsubFrameNum,
                    /* + SPS_TDD_Changes */
                    DRA_SCHEDULING,
                    /* - SPS_TDD_Changes */
                    timerExpiredTTI,ulHarqSysFrameNum,
                    /*CA Changes start  */
                    internalCellIndex);
                    /*CA Changes end  */
#endif 
            (*numberOfULUEsToBeScheudled_p)--;

            /* Semi static changes Start*/
            if(ULSCH_UCI_INVALID_INDEX != tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
            {
                /* Fill UL Harq Info*/
                fillSemiStaticInfoForULSCH (ulschUciRachInfo_p,
                        tempDirectIndexingUEInfo_p,
                        ulUEContextInfo_p,
                        tempULHarqInfo_p,
                        tempHarqProcessId,
			/* SPR 9173 changes start */
                        ulSubFrameNum,0, internalCellIndex);
			/* SPR 9173 changes end */
            }
            else
            {
                if ( MAX_ULSCH_UCI_SUPPORTED <=  numberOfInfoPresent)
                {
                    ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                            __func__);
                    break;
                }
                /*SPR 7086 Fix Start*/
                /* SPR 10195 Fix Start */
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS =
                    nSRSDetermineOnCollision(ulUEContext_p, tempHarqProcessId, ulSubFrameNum, internalCellIndex);
                /* SPR 10195 Fix End */
                /*SPR 7086 Fix End*/

                /* Fill UL Harq Info*/
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p = 
                  tempULHarqInfo_p;
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId = 
                  tempHarqProcessId;
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = 
                  MAC_SS_PDU_ULSCH;
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = 
                  ulUEContext_p->crnti;

                numberOfInfoPresent++;
                LOG_UT(MAC_UCI_ULSCH_PDU_ASSIGN,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    tempUeIndex,
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType,\
                    tempDirectIndexingUEInfo_p->ulschUciInfoIndex,\
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId,\
                    0,0.0,0.0,\
                    __func__,"ULSCH_PDU_ASSIGN");

            }
            /* Semi static changes End*/


            tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK;
        }
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, 
                "\ntempPUSCHResources = %d numberOfULUEsToBeScheudled_p = %d\n"
                ,tempPUSCHResources,*numberOfULUEsToBeScheudled_p);
    }
    /*Update Number of numberOfInfoPresent in the global context */ 
    ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;

}
#elif TDD_CONFIG
/******************************************************************************
 * Function Name    : processULNackNonAdaptiveInPUSCH
 * Inputs           : tempPdcchULOutput_p - The pointer to PdcchULOutputInfo,
 *                    rbMapNum,
 *                    subFrameNumber
 *                    currentGlobalTick - The present Global Tick,
 *                    numberOfULUEsToBeScheudled_p - Pointer to no. of UL UE's to be
 *                                                   scheduled,
 *                    containerTick - The Tick of the container being used,
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function will read the Nack Adaptive queue and update 
 *                    the HARQ process, and book the resources in advance. It 
 *                    will also create the HI PDUs. 
 ******************************************************************************/
 void processULNackNonAdaptiveInPUSCH(
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
                               UInt8 ulSubFrameNum
                               /*TDD Config 0 Changes End*/ 
                               )
{
    UInt16 tempUeIndex=0;
    HIPduArray* tempHIPdu_p  = PNULL;
    AllocatedResInfo resourceReserved = {0};
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; /* Invalid Value*/
    DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    ULUEContext* ulUEContext_p = PNULL;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    UInt8 localCount = 0;
    ULHarqInfo* tempULHarqInfo_p = PNULL;
    /* ICIC changes start */
    RbMapNode *rbMapNode_p = PNULL;
    UInt32 tempPUSCHResources[MAX_USER_LOCATION] = {0};
    UInt8 ceOverlapRBs = 0;
    /* ICIC changes end */
    /* TDD Config 6 Changes Start */
    UInt8 prevUlSubFrameNum  = 0;
    UInt16 prevUlSysFrameNum = 0;
    /* SPR 9173 fix start */
    UInt16 ulSysFrameNum = 0;
    /* SPR 9173 fix end */
    /* TDD Config 6 Changes End */
    PdcchULNackUEInfo *pdcchULNackUEInfo_p  = PNULL;
    UeScheduledInfo *ueScheduledInfo_p = PNULL;
    /*CA Changes start  */
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];
    /*TDD Config 0 Changes Start*/
    UInt8 subFrameNum = currentGlobalTick % MAX_SUB_FRAME;
    UInt8 ulSubFrameOffset =  getUlOffset(subFrameNum,ulSubFrameNum);
#ifdef MAC_AUT
    ulFrameOffset = ulSubFrameOffset;
#endif
    /* SPR 15909 fix start */
    tickType_t ulGlobalTick = currentGlobalTick + ulSubFrameOffset;
   /* SPR 15909 fix end */
    UInt8 phichSubFrameNum = 0;
     /*TDD Config 0 Changes End*/
    UInt16 sysFrameNum = (currentGlobalTick / MAX_SUB_FRAME) % MAX_SFN_VALUE;
    /* Time Averaging changes start */
    UInt16  numberOfInfoPresent = 0;
    /* msg3_retx_tdd_support_start */
    /* +- SPR 17777 */
    TempCRNTICtx *tempUEContext_p = PNULL;
    PdcchULNackUEInfo *pdcchULNackTempUEInfo_p  = PNULL;
    /*TDD Config 0 Changes Start*/
    /* SPR 15909 fix start */
    tickType_t timerExpiredTTI = currentGlobalTick  + ulSubFrameOffset;
    /* SPR 15909 fix end */
    /* SPR 4386 changes start */
    UInt8 timerExpsubFrameNum = subFrameNumber + ulSubFrameOffset;
    /*TDD Config 0 Changes End*/
    UInt32 ulHarqSysFrameNum = 0;
    /*SPR_11092 Start*/
    ulHarqSysFrameNum = (MODULO_ONEZEROTWOFOUR((currentGlobalTick)/MAX_SUB_FRAME));
    /*SPR_11092 End*/
    /* SPR 4386 changes end */
    if(timerExpsubFrameNum >= MAX_SUB_FRAME)
    {
        timerExpsubFrameNum = timerExpsubFrameNum % MAX_SUB_FRAME;

        ulHarqSysFrameNum++;
        if (ulHarqSysFrameNum == MAX_SFN_VALUE)
        {
            ulHarqSysFrameNum = 0;
        }
    }
    /* msg3_retx_tdd_support_end */

    /*TDD Config 0 Changes Start*/
    /*ulSchUciRachContainer_g is now accessed on the basis of U frame*/
    ULSchUciRachPoolInfo* ulschUciRachInfo_p
        /*CA Changes start  */
        = ulSchUciRachContainer_gp[internalCellIndex] + (ulSubFrameNum % MAX_ULSCH_UCI_CONTAINER); 
        /*CA Changes end  */
    /*TDD Config 0 Changes End*/ 
    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    pdcchULNackUEInfo_p = &(tempPdcchULOutput_p->pdcchULNackUEInfo);
    /*Cov 85825 +-*/
    /* msg3_retx_tdd_support_start */
    pdcchULNackTempUEInfo_p = &(tempPdcchULOutput_p->pdcchULNackTempUEInfo);
    /* msg3_retx_tdd_support_end */
    /* ICIC changes start */
    /*CA Changes start  */
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    tempPUSCHResources[CC_CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter; 
    tempPUSCHResources[CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter; 
    /* ICIC changes end */

    /* SPR 9173 fix start */
    if (ulSubFrameNum < subFrameNumber)
    {
        ulSysFrameNum = MODULO_ONEZEROTWOFOUR((sysFrameNum + 1));
    }
    else
    {
        ulSysFrameNum = sysFrameNum;
    }
    /* SPR 9173 fix end */
    /*TDD Config 0 Changes Start*/
    prevUlSubFrameNum = (ulGlobalTick - getprevUlSfOffsetFromCurrentUlSf(ulSubFrameNum, 
                        /* CA TDD CHANGES START */
                        internalCellIndex
                        /* CA TDD CHANGES END */
                        )) % MAX_SUB_FRAME;
    GET_UL_SYS_FRAME_NUM_FROM_PHICH(prevUlSubFrameNum, subFrameNumber, prevUlSysFrameNum, sysFrameNum);
    /*TDD Config 0 Changes End*/
#ifdef MAC_AUT
    prevUlSubFrameNo = prevUlSubFrameNum;
#endif

    /* msg3_retx_tdd_support_start */
    /* First serve the Non Adaptive UEs */
    localCount = pdcchULNackTempUEInfo_p->nonAdaptiveCount;

    while(localCount--)
    {
        tempUeIndex = \
                      tempPdcchULOutput_p->pdcchULNackTempUEInfo.
                      pdcchULNackNonAdaptiveUEArray[localCount];
        tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[tempUeIndex].tempCRNTICtx_p;


        if ((tempUEContext_p != PNULL) && (RAR_SENT == tempUEContext_p->status))
        {
            if (MAC_FAILURE == processULNackForNonAdaptiveReTxOfMsg3Tdd(tempUeIndex,
                        containerTick,
                        currentGlobalTick,
                        /* +- SPR 17777 */
                        tempUEContext_p,
                        rbMapNum,
                        numberOfULUEsToBeScheudled_p,
                        &tempPUSCHResources[CC_CE_REGION],
                        timerExpiredTTI,
                        timerExpsubFrameNum,
                        ulHarqSysFrameNum,
                        &numberOfInfoPresent,
                        internalCellIndex,
                        /*TDD Config 0 Changes Start*/
                        ulSubFrameNum))
                        /*TDD Config 0 Changes End*/ 
            {
                continue;
            }
        }

    }
    /* msg3_retx_tdd_support_end */
    
    
    localCount = pdcchULNackUEInfo_p->nonAdaptiveCount;
    while(localCount--)
    {
        tempUeIndex = \
                      pdcchULNackUEInfo_p->pdcchULNackNonAdaptiveUEArray[localCount];
        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];         

        if (!ulUEContextInfo_p->pendingDeleteFlag)
        {
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
            /* + SPR_17858_209_CHANGES */
            if( !ulUEContext_p )
            {
                //fprintf(stderr, "Adaptive UE context is NULL. ueIndex %d, localCount %d \n ",tempUeIndex,localCount );
                continue;
            }
            /* - SPR_17858_209_CHANGES */

            if ( !isValidSubframeNumForDCI_0(subFrameNumber, internalCellIndex))
            {
                ltePanic("Invalid DL Subframe[%d] For DCI 0 \n",subFrameNumber);
            }
            /*TDD Config 0 Changes Start*/
            /*Now ulUeScheduledInfoPointerArray is accessed on the basis of U
             * frame*/
            /** SPR 14204 Fix : Start **/
            ueScheduledInfo_p = ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulSubFrameNum];
            /** SPR 14204 Fix : End **/
            /*TDD Config 0 Changes End*/ 


            if ( !ueScheduledInfo_p )
            {
                ltePanic("[%s] ueScheduledInfo_p is null\n",
                        __func__);
            }
            tempHarqProcessId = ueScheduledInfo_p->ulHarqProcessId;
            /* MEAS_GAP_CHG */
            tempULHarqInfo_p = \
                               &(ulUEContext_p->ulresInfo[tempHarqProcessId]);
            /*TDD Config 0 Changes Start*/
            tempULHarqInfo_p->isPdcchAllocated = 0;
            phichSubFrameNum = ulSubFrameNum + getHiPduSubframeNum(ulSubFrameNum,internalCellIndex);
            phichSubFrameNum = phichSubFrameNum % MAX_SUB_FRAME;
            /*TDD Config 0 Changes End*/

            if (MEAS_GAP_RE_TX_NO_CCE_NO_GRANT == 
                    ueScheduledInfo_p->flag ||
                    /* 5798 Fix Start */
                    (MAX_UE_SCHEDULED_NO_RETX_NO_GRANT == ueScheduledInfo_p->flag))
                /* 5798 Fix End */
            {

                /* + SPR 6864 */
                if (ULSCH_UCI_INVALID_INDEX !=
                        ueScheduledInfo_p->ulschUciInfoIndex)
                {
                      /* + SPR_14994 */
                    checkAndRestoreUCIPduType(ulUEContext_p, ueScheduledInfo_p,
                               ulSysFrameNum, ulschUciRachInfo_p, ulSubFrameNum,
                                /*SPR 19504 fix start*/    
                                rbMapNum,
                                internalCellIndex);
                                /*SPR 19504 fix end*/    
                      /* - SPR_14994 */
                }
                /* - SPR 6864 */


                tempULHarqInfo_p->currentRetransCount++; 
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX; 
		/* SPR 13196 fix start */
                tempULHarqInfo_p->newDataIndicator = 0;
		/* SPR 13196 fix end */

                resetHarqId(prevUlSubFrameNum, ulUEContext_p, internalCellIndex,prevUlSysFrameNum);
                setHarqBusy(ulSubFrameNum, tempHarqProcessId, ulUEContext_p,internalCellIndex, ulSysFrameNum);
                
                ulHarqTimerStart(
                        subFrameNumber,
                        sysFrameNum,
                        tempUeIndex,
                        tempHarqProcessId,
                        /* + SPS_TDD_Changes */
                        DRA_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        /*CA Changes start  */
                        internalCellIndex,
                        /*CA Changes end  */
                        /* TDD Config 0 Changes Start */
                        ulSubFrameNum);
                        /* TDD Config 0 Changes End */
		      /* SPR 13196 fix start */
                 ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;
     		/* SPR 13196 fix end */
                 /* + COVERITY 5.0 */
                 if ((ueScheduledInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) &&
                         (INVALID_CCE_INDEX != ueScheduledInfo_p->ulCCEIndex))
                 /* - COVERITY 5.0 */
                 {
                    mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                            dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                            internalCellIndex);
                 }

                ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            /*SPR Fix 12758 start */
            if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
            {
               /*SPR Fix 12758 end */
                addEntryTOHIPduArray(
                        &ulUEContext_p->ulresInfo[tempHarqProcessId],
                        /* SPR 5467 changes start*/
                        HI_ACK,
                        /* SPR 5467 changes end*/
                        ulUEContext_p->crnti,
                        tempHIPdu_p,
                        internalCellIndex,
                        /* TDD Config 0 Changes Start */
                        /* +- SPR 17777 */
                        prevUlSubFrameNum);
                /* TDD Config 0 Changes End */
               /*SPR Fix 12758 start */
               tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
            }
            /*SPR Fix 12758 end */
                /*SPR 19504 fix start*/
                /*SPR 20145 fix start*/
                //cleanUpUeScheduledInfo(ueScheduledInfo_p);
                /*SPR 20145 fix end*/
                /*SPR 19504 fix end*/
                continue;
            }
            /* MEAS_GAP_CHG */

            /*
               We now create the HI PDU corresponding to the Nack received
               */
            /* SPR 10803 [BUG 848] FIX START */
            /*addEntryTOHIPduArray(
                    tempULHarqInfo_p,
                    HI_NACK,
                    ulUEContext_p->crnti,
                    tempHIPdu_p,
                    internalCellIndex,
                    prevUlSubFrameNum,
                    currentGlobalTick);*/
            /* SPR 10803 [BUG 848] FIX END */

            /*
               We at first check whether the UE has completed it's NACK re
               transmission limit, if yes we will free the HARQ Process and
               continue to the next UE.
               */
            if (ueScheduledInfo_p->flag == RECEIVED_NACK_RE_TRANS_COMPELETE)
            {
                /*
                 * If a UL MAX RTXN Complete then the
                 * corresponding  nPhichSeq th bit from the LBS of the array
                 * phichSequenceNoPerPhichGroup_g[Subframe No][  ]
                 * should be made to set.
                 */
                LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "Ret Trans complete Now Free the Harq Process Id [%d]\n", subFrameNumber);
                LOG_MAC_MSG(MAC_NACK_RETX_COMP_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                       currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                       0,0,0,0.0,0.0,__func__,"NACK_RETX_COMP_FREE");

                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */
                /* SPR 2437 Start */
                ueScheduledInfo_p->flag = UNSET_FLAG; 
                ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                /** CA-TDD Changes Start:05 **/
                resetHarqId( prevUlSubFrameNum,
                             ulUEContext_p, internalCellIndex,prevUlSysFrameNum
                           );
                /** CA-TDD Changes End:05 **/
                /* SPR 2437 End */
                /*SPR 19504 fix start*/
                /*SPR 20145 fix start*/
                //cleanUpUeScheduledInfo(ueScheduledInfo_p);
                /*SPR 20145 fix end*/
                /*SPR 19504 fix end*/

                continue;  
            } 

            /* ICIC changes start */
            /* SPR 10803 [BUG 848] FIX START */
            MacRetType   retVal = MAC_FAILURE;
            /* SPR 10803 [BUG 848] FIX END */
            if (tempPUSCHResources[CC_CE_REGION] >= tempULHarqInfo_p->riLength)
                /* ICIC changes end */
            {
                /*We use 0 for Retransmission */ 
                ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;

                /* SPR 10027 FIX START */
                /* Check if isDrbPresent = TRUE and Update the lastScheduledTick */
                CHECK_AND_UPDATE_LAST_SCHEDULED_TICK(
                        tempULHarqInfo_p->isDrbPresent,ulUEContext_p)
                /* SPR 10027 FIX END */

                retVal = checkResourceAvailability(tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,rbMapNode_p->rbMap);

                if (retVal == MAC_FAILURE)
                {
            /* SPR 10803 [BUG 848] FIX START */
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX; 
            /* SPR 10803 [BUG 848] FIX START */
               /*PHICH_SPR Fix 12758 start */
               if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
               {
                    addEntryTOHIPduArray(
                    tempULHarqInfo_p,
                    HI_ACK,
                    ulUEContext_p->crnti,
                    tempHIPdu_p,
                    internalCellIndex,
                    /* TDD Config 0 Changes Start */
                    /* +- SPR 17777 */
                    prevUlSubFrameNum);
                    /* TDD Config 0 Changes End */
                  tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
               }
               /*PHICH_SPR Fix 12758 end */
            
            /* SPR 10803 [BUG 848] FIX START */
                    //continue;
            /* SPR 10803 [BUG 848] FIX END */
                }
                else
                {
                    addEntryTOHIPduArray(
                    tempULHarqInfo_p,
                    HI_NACK,
                    ulUEContext_p->crnti,
                    tempHIPdu_p,
                    internalCellIndex,
                    /* TDD Config 0 Changes Start */
                    /* +- SPR 17777 */
                    prevUlSubFrameNum);
                    /* TDD Config 0 Changes End */

                    /* ICIC changes start */
                    tempPUSCHResources[CC_CE_REGION] -= reserveAlreadyAvailableResourcesForRetx( \
                            tempULHarqInfo_p->riStart, \
                            tempULHarqInfo_p->riLength,\
                            &rbMapNode_p->availableResourceInfo[CC_CE_REGION],\
                            rbMapNode_p->rbMap); 
                    /* CC resouces are updated always irrespective of user location,
                     * we check here whether the RBs lied in CE region and update the 
                     * resouce info if necessary*/
                    ceOverlapRBs = updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                            tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                            internalCellIndex);
                    if(0 < ceOverlapRBs)
                    {
                        tempPUSCHResources[CE_REGION] -= ceOverlapRBs;
                    }
                    /* ICIC changes end */

                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_INFO,LOGDEBUG,MAC_UL_HARQ,\
                            getCurrentTick(),\
                            tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,\
                            tempPUSCHResources[CC_CE_REGION],\
                            tempPUSCHResources[CE_REGION],\
                            0,0.0,0.0,\
                            __func__,"NACK_RES_ALLOC");
                    /* SPR 10803 [BUG 848] FIX START */
                }
                /* SPR 10803 [BUG 848] FIX END */
            }
            else
            {
            /* SPR 10803 [BUG 848] FIX START */
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX; 
            /* SPR 10803 [BUG 848] FIX END */
                /* required resources are not available hence start UL Timer*/
            /* SPR 10803 [BUG 848] FIX START */
                /* We now create the HI PDU corresponding to the Nack received */
            /*SPR Fix 12758 start */
            if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
            {
                addEntryTOHIPduArray(
                    tempULHarqInfo_p,
            /* SPR 10803 [BUG 848] FIX START */
                    HI_ACK,
            /* SPR 10803 [BUG 848] FIX END */
                    ulUEContext_p->crnti,
                    tempHIPdu_p,
                    internalCellIndex,
                    /* TDD Config 0 Changes Start */
                    /* +- SPR 17777 */
                    prevUlSubFrameNum);
                    /* TDD Config 0 Changes End */
               tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
            }
            /*SPR Fix 12758 end */
            /* SPR 10803 [BUG 848] FIX END */
                /* ICIC changes start */
                tempPUSCHResources[CC_CE_REGION]  = 0;
                /* ICIC changes end */
                LOG_MAC_MSG(MAC_NACK_RES_ZERO_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        rbMapNum, 
                        tempULHarqInfo_p->nextReTransmissionType,
                        tempULHarqInfo_p->currentRetransCount,
                        0.0,0.0,__func__,"NACK_RES_ZERO_FAIL");
            }

            /*TDD Config 0 Changes Start*/
            /*Block the same phich resources as given earlier in Non adaptive
             * Retx*/
			/* CA-TDD Changes Start */
            phichSequenceNoPerPhichGroup_g[internalCellIndex][phichSubFrameNum][tempULHarqInfo_p->nPhichGrp] &=
                ~(ONE << tempULHarqInfo_p->nPhichSeq);
			/* CA-TDD Changes End */
            tempULHarqInfo_p->sfUsed = phichSubFrameNum;
            /*TDD Config 0 Changes End*/
            LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                    tempUeIndex,
                    currentGlobalTick,
                    NON_ADAPTIVE_RE_TX,
                    tempHarqProcessId,
                    tempULHarqInfo_p->riLength,
                    tempULHarqInfo_p->riStart,
                    ueScheduledInfo_p->ulGrantRequestType,
                    tempULHarqInfo_p->deltabsrLoad,
                    FALSE,
                    tempULHarqInfo_p->mcsIndex,
                    0,
                    TPC_VALUE_1 
                    /* UL_MU_MIMO_CHG START */
                    , ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                    ueScheduledInfo_p->buddyUeIndex,
                    ulUEContext_p->userLocationType,
                    tempULHarqInfo_p->nDmrs,
                    ulUEContext_p->modulationScheme,
                    /* UL_MU_MIMO_CHG END */
                     /* CA Stats Changes Start */
                     internalCellIndex
                     /* CA Stats Changes End */
                   );
            /* TDD Config 6 Changes Start */
            /* Required resources are not available, hence increase the current
             * ReTx count and start the UL Timer */
            resetHarqId(prevUlSubFrameNum, ulUEContext_p, internalCellIndex,prevUlSysFrameNum);
            setHarqBusy(ulSubFrameNum, tempHarqProcessId, ulUEContext_p,internalCellIndex, ulSysFrameNum);
            /* TDD Config 6 Changes End */
            ulHarqTimerStart( subFrameNumber,
                    sysFrameNum,
                    tempUeIndex,
                    tempHarqProcessId,
                    /* + SPS_TDD_Changes */
                    DRA_SCHEDULING,
                    /* - SPS_TDD_Changes */
                    /*CA Changes start  */
                    internalCellIndex,
                    /*CA Changes end  */
                    /* TDD Config 0 Changes Start */
                    ulSubFrameNum);
                    /* TDD Config 0 Changes End */
            /* Required resources are not available, hence increase the current
             * ReTx count and start the UL Timer */
            updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
            (*numberOfULUEsToBeScheudled_p)--;

            /* Semi static changes Start*/
            /* SPR 10803 [BUG 848] FIX START */
            if (retVal != MAC_FAILURE)
            {		    
            if(ULSCH_UCI_INVALID_INDEX != ueScheduledInfo_p->ulschUciInfoIndex)
            {
                /* Fill UL Harq Info*/
                fillSemiStaticInfoForULSCH (ulschUciRachInfo_p,
                        ueScheduledInfo_p,
                        ulUEContextInfo_p,
                        tempULHarqInfo_p,
                        tempHarqProcessId,
			/* SPR 9173 changes start */
                        /* SPR 5620 START*/
                        ulSysFrameNum,
                        /* SPR 5620 END*/
                        ulSubFrameNum,0, internalCellIndex);
			/* SPR 9173 changes end */
                /* SPR 10136 fix Start*/
                ueScheduledInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
                /* SPR 10136 fix end*/
            }
            else
            {
                if ( MAX_ULSCH_UCI_SUPPORTED ==  numberOfInfoPresent)
                {
                    ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                            __func__);
                    break;
                }
                /*TDD Config 0 Changes Start*/
                /*srsCellConfigMap_g should be accessed on the basis of U frame*/
                /*SPR 7086 Fix Start*/
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS =
                    nSRSDetermineOnCollision(ulUEContext_p, tempHarqProcessId, ulSubFrameNum, internalCellIndex);
                /*SPR 7086 Fix End*/
                /*TDD Config 0 Changes End*/

                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p = 
                    tempULHarqInfo_p;
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId = 
                    tempHarqProcessId;
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = 
                    MAC_SS_PDU_ULSCH;
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = 
                    ulUEContext_p->crnti;
                
                /*TDD Config 0 Changes Start*/
                /*Populate ulschUciInfoIndex in ueScheduledInfo_p and
                 * ueScheduledInfo_p in ulschUciRachInfo_p.This info is
                 * needed in PUCCH for TDD_CONFIG0*/
                ueScheduledInfo_p->ulschUciInfoIndex = numberOfInfoPresent;
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ueScheduledInfo_p = ueScheduledInfo_p;
                /*TDD Config 0 Changes End*/

                LOG_UT(MAC_UCI_ULSCH_PDU_ASSIGN,LOGDEBUG,MAC_PUSCH,\
                        getCurrentTick(),\
                        tempUeIndex,
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType,\
                        ueScheduledInfo_p->ulschUciInfoIndex,\
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId,\
                        0,0.0,0.0,\
                        __func__,"ULSCH_PDU_ASSIGN");
						
                numberOfInfoPresent++;
            }
            /* SPR 10803 [BUG 848] FIX START */
	    }
            /* SPR 10803 [BUG 848] FIX END */
            /* Semi static changes End*/
            /*SPR 19504 fix start*/
	    /*SPR 20145 fix start*/
            //cleanUpUeScheduledInfo(ueScheduledInfo_p);
	    /*SPR 20145 fix end*/
            /*SPR 19504 fix end*/

            ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            ueScheduledInfo_p->flag = RECEIVED_NACK;

        }
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, 
                "\ntempPUSCHResources = %d numberOfULUEsToBeScheudled_p = %d\n"
                ,tempPUSCHResources,*numberOfULUEsToBeScheudled_p);
    }
    /*Update Number of numberOfInfoPresent in the global context */ 
    ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;

}
#endif
            /* SPR 10803 [BUG 848] FIX START */
MacRetType checkResourceAvailability(UInt8 startIndex,
    UInt8 length ,UInt8  *rbMap)
{
    SInt8 pos = 0;
    SInt8 sStartIndex = startIndex;
    UInt8 endIndex = sStartIndex + length - 1;
    MacRetType retVal = MAC_FAILURE;


    pos = endIndex;
    while(pos >= sStartIndex)
    {
        if( FREE_RESOURCE != rbMap[pos])
            return retVal;
        else
            pos--;
    }
    retVal = MAC_SUCCESS;

    return retVal;
}
            /* SPR 10803 [BUG 848] FIX END */


/******************************************************************************
 * Function Name    : processULNackAdaptiveInPUSCH
 * Inputs           : ulDelay - The delay of execution legs in terms of the number
 *                              number subFrames w.r.t. PHY & the current processing 
 *                              being done at MAC for intended tick at PHY,
 *                    rbMapNum,
 *                    currentGlobalTick - The present Global Tick,
 *                    numberOfULUEsToBeScheudled_p - Pointer to no. of UL UE's to be
 *                                                   scheduled,
 *                    containerTick - The Tick of the container being used,
 *                    hiIndex,
 *                    subFrame,
 *                    locType,
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None      
 * Description      : This function will read the Nack Adaptive queue and update the 
 *                    HARQ process, and book the resources in advance. It will 
 *                    also create the HI PDUs. 
 ******************************************************************************/
#ifdef FDD_CONFIG
 void processULNackAdaptiveInPUSCH(
    UInt8 ulDelay,
    UInt32 rbMapNum, /* SS_S2 */
    /* SPR 15909 fix end */
    tickType_t currentGlobalTick,
    /* SPR 15909 fix end */
    UInt8* numberOfULUEsToBeScheudled_p,
    UInt8 containerTick,
    /* CR changes start */
    UInt8 hiIndex,
    /* CR changes end */
    UInt8 subFrame,
    UInt8 locType,
    InternalCellIndex internalCellIndex)
{
    UInt16 tempUeIndex=0;
    HIPduArray* tempHIPdu_p  = PNULL;
    AllocatedResInfo resourceReserved = {0};
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; /* Invalid Value*/
    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p = PNULL;
    DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    PdcchULOutputInfo* tempPdcchULOutput_p = PNULL;
    ULUEContext* ulUEContext_p = PNULL;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    UInt8 localCount = 0;
    ULHarqInfo* tempULHarqInfo_p = PNULL;
    /* ICIC changes start */
    /* Initialized by any region and changed in case of Cell edge user 
     * as CE_REGION_PREFERRED if configured so */
    UInt8 regionAllocated = CC_CE_ANY_REGION;
    RbMapNode *rbMapNode_p = PNULL;
    UInt32 tempPUSCHResources[MAX_USER_LOCATION] = {0};
    MacRetType   retVal = MAC_FAILURE;
    UInt8 ceOverlapRBs = 0;
    /* ICIC changes end */
    UInt16  numberOfInfoPresent = 0;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    /* Time Averaging changes start */
    UInt32 ueMaxPowerFlag= 0;
    /* Time Averaging changes end */
    /*SPR_3305_FIX_START*/
    TempCRNTICtx *tempUEContext_p = PNULL;
    /*SPR_3305_FIX_END*/
    /* +DYNAMIC_ICIC */
    /*CA Changes start  */
    ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + ulPowerMaskIndex_g[internalCellIndex];
    /*CA Changes end  */
    SInt32 regionPowerMask = INVALID_UL_POWER_MASK;
    /* -DYNAMIC_ICIC */
    UInt32 ueMinPowerFlag = 0;


    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */
    /* SPR 9173 changes start */
    UInt8 ulSubFrameNum  = (currentGlobalTick + FDD_HARQ_OFFSET + ulDelay )% MAX_SUB_FRAME;
    /* SPR 9173 changes end */
#ifdef ULHARQ_TIMER_PROC    
    /* SPR 15909 fix start */
    tickType_t timerExpiredTTI = currentGlobalTickWithULDelay  + START_ULHARQ_TIMER_OFFSET;
    /* SPR 15909 fix end */
    /* SPR 4386 changes start */
    UInt8 subFrameNum = (subFrame + ulDelay);
    UInt8 timerExpsubFrameNum = (subFrameNum + START_ULHARQ_TIMER_OFFSET); 		
    UInt32 ulHarqSysFrameNum = 0;
    UInt32 ulHarqSubFrameNum = 0;
    /*CA Changes start  */
    getSFAndSFN(&ulHarqSysFrameNum, &ulHarqSubFrameNum, internalCellIndex);              
    /*CA Changes end  */
    /* SPR 4386 changes end */
    if(timerExpsubFrameNum >= MAX_SUB_FRAME)
    {
        timerExpsubFrameNum = timerExpsubFrameNum % MAX_SUB_FRAME;

        ulHarqSysFrameNum++;
        if (ulHarqSysFrameNum == MAX_SFN_VALUE)
        {
            ulHarqSysFrameNum = 0;
        }
    }
#endif
    /* SPR 5467 changes start*/
    UInt8 resAllocFailure = 0;
    /* SPR 5467 changes end*/

    ULSchUciRachPoolInfo* ulschUciRachInfo_p 
        /*CA Changes start  */
        = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
        /*CA Changes end  */

    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    /*CA Changes start  */
    tempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];


    /* ICIC changes start */
    /*CA Changes start  */
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    /* Available PUSCH resources are fetched for each location type */
    tempPUSCHResources[CC_CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter; 
    tempPUSCHResources[CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter; 
    /* ICIC changes end */



    LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH,"[%s] containerTick = %d "
            " tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount = %d \n"
            " tempPUSCHResources = %d  rbMapNum = %d\n"
            " numberOfULUEsToBeScheudled_p = %d \n",__func__
            ,containerTick, tempPdcchULOutput_p->pdcchULNackUEInfo.nonAdaptiveCount, 
            tempPUSCHResources, rbMapNum,*numberOfULUEsToBeScheudled_p,(getCurrentTick() -8)); /* SS_S2 */

    localCount = tempPdcchULOutput_p->pdcchULNackTempUEInfo.adaptiveCount[locType];

    while(localCount--)
    {
        tempUeIndex = \
                      tempPdcchULOutput_p->pdcchULNackTempUEInfo.
                      pdcchULNackAdaptiveUEArray[locType][localCount];
        tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[tempUeIndex].\
                          tempCRNTICtx_p;
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "\ntempUeIndex = %d",tempUeIndex);


        if ((tempUEContext_p != PNULL) && (RAR_SENT == tempUEContext_p->status))
        {
            processULNackForAdaptiveReTxOfMsg3(tempUeIndex, containerTick, tempUEContext_p,
                    &resourceReserved, currentGlobalTick, ulDelay, rbMapNum,
                    numberOfULUEsToBeScheudled_p, &tempPUSCHResources[CC_CE_REGION],
                    timerExpiredTTI, timerExpsubFrameNum,
                    ulHarqSysFrameNum,hiIndex,
                    &numberOfInfoPresent,regionAllocated,
                    internalCellIndex);
        }

    }


    /* ICIC changes start */ 
    /* Adaptive Handling*/
    localCount = tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount[locType];
    /* ICIC changes end */ 
    while(localCount--)
    {
            /* SPR 10803 [BUG 848] FIX START */
        resAllocFailure = FALSE;
            /* SPR 10803 [BUG 848] FIX END */
        tempUeIndex =
            tempPdcchULOutput_p->pdcchULNackUEInfo.pdcchULNackAdaptiveUEArray[locType][localCount];
        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];         
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "\ntempUeIndex = %d",tempUeIndex);
        /* SPR 11548 Fix Start */
        /* resetting the resAllocFailure to FALSE */ 
        /* SPR 11548 Fix End */

        if (!ulUEContextInfo_p->pendingDeleteFlag)
        {
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
            ulUEContext_p->isPdcchAllocated = 0;
            tempDirectIndexingUEInfo_p = \
                                         &(tempPdcchULOutput_p->directIndexingUEInfo[tempUeIndex]); 
            tempHarqProcessId = tempDirectIndexingUEInfo_p->harqIndex;
            /* MEAS_GAP_CHG */
            tempULHarqInfo_p = \
                               &(ulUEContext_p->ulresInfo[tempHarqProcessId]);

            /** TTIB_Code Start */
            /* TTI Bundling changes starts (to send forced ACK)*/
            /* Need not send DCI when UE is bundled and current HARQ process is for 
             * non bundled Transmission
             In this case transmission will be forcefully ACKed*/

            if(( ulUEContext_p->ttiBundlingEnabled && !(tempULHarqInfo_p->isTTIBundled)))
            {
                addEntryTOHIPduArray(
                        &ulUEContext_p->ulresInfo[tempHarqProcessId],
                        HI_ACK,
                        ulUEContext_p->crnti,
                        ulUEContext_p,
                        tempULHarqInfo_p->sfUsed,                        
                        tempHIPdu_p,
                        internalCellIndex);
                /* SPR 16605 Fix Start */
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                    ~(ONE << tempULHarqInfo_p->nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                tempULHarqInfo_p->sfUsed = hiIndex;
                /* SPR 16605 Fix End */

                /* SPR 20929 FIX Start */
                if ((tempDirectIndexingUEInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                        (INVALID_CCE_INDEX != tempDirectIndexingUEInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
                {
                    mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].\
                            dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex]),
                            internalCellIndex);
                    LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "[%s] [%d] INVALID CCE INDEX for UE [%d].\n",__func__,
                            ulUEContext_p->ueIndex,getCurrentTick());
                }
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, 
                        "[%s] [%d] Sending forced Ack(instead of Nack) for non-bundled Data recieved"\
                        "if UE is TTI bundling Enabled ,freeing Harq process %d.\n",__func__,getCurrentTick(),tempHarqProcessId);

                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */

                updateTTIHarqMap(tempHarqProcessId,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
                continue;
            }
            /** TTIB_Code End */
            /* SPR 10803 [BUG 848] FIX START */
            if ((MEAS_GAP_RE_TX_NO_CCE_NO_GRANT ==
                    tempDirectIndexingUEInfo_p->flag)
                /* FORCED_ACK SPR#9374 Changes Start */
                ||(tempDirectIndexingUEInfo_p->flag == ADAPTIVE_RETRANS_NO_CCE))
                /* FORCED_ACK SPR#9374 Changes End */
            {
                /* + SPR 6864 */
                if (ULSCH_UCI_INVALID_INDEX !=
                        tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
                {
                      /* + SPR_14994 */
                    checkAndRestoreUCIPduType(ulUEContext_p, tempDirectIndexingUEInfo_p,
                                                ulschUciRachInfo_p,ulSubFrameNum,
                                                /*SPR 19504 fix start*/    
                                                rbMapNum,
                                                internalCellIndex);
                                                /*SPR 19504 fix end*/    
                      /* - SPR_14994 */
                }
                /* - SPR 6864 */

                lteWarning("should not come here: NO_CCE_NO_Grant + Adaptive");
#ifdef ULHARQ_TIMER_PROC
                ulHarqTimerStart(tempUeIndex, tempHarqProcessId,
                        timerExpsubFrameNum,
                        /* + SPS_TDD_Changes */
                        DRA_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        timerExpiredTTI,ulHarqSysFrameNum,
                        /*CA Changes start  */
                        internalCellIndex);
                        /*CA Changes end  */
#endif
                /* SPR 14279 Changes Start */
                /* tempULHarqInfo_p->currentRetransCount++; */
                updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
                /* SPR 14279 Changes End */
                /* should numGrantsAllocated,stats be updated */                 
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
		/* SPR 13196 fix start */
                tempULHarqInfo_p->newDataIndicator = 0;
		/* SPR 13196 fix end */

                /* +COVERITY 10718 */
                /* SPR 20929 FIX Start */
                if ((tempDirectIndexingUEInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                        (INVALID_CCE_INDEX != tempDirectIndexingUEInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
                {
                    mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->
                            ulCCEIndex].dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->
                             ulCCEIndex]), internalCellIndex);
                }
			   /* SPR 13196 fix start */
                tempULHarqInfo_p->newDataIndicator = 0;
	        	/* SPR 13196 fix end */
                /* -COVERITY 10718 */
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                /*SPR Fix 17959 start */
                if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                {
                    /*SPR Fix 17959 end */

                addEntryTOHIPduArray(
                        &ulUEContext_p->ulresInfo[tempHarqProcessId],
                        /* SPR 5467 changes start*/
                        HI_ACK,
                        /* SPR 5467 changes end*/
                        ulUEContext_p->crnti,
                        /* CR changes start */
                        ulUEContext_p,
                        tempULHarqInfo_p->sfUsed,                        
                        /* CR changes end */
                        tempHIPdu_p,
                        internalCellIndex);
                    /*SPR Fix 17959 start */
                    tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                }
                /*SPR Fix 17959 end */				   
                /* CR changes start */
                /*CA Changes start  */
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                    ~(ONE << tempULHarqInfo_p->nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                /*CA Changes end  */
                tempULHarqInfo_p->sfUsed = hiIndex;
                /* CR changes end */
                continue;
            }
            /* MEAS_GAP_CHG */

            /*
               We now create the HI PDU corresponding to the Nack received
               */
            /* SPR 9658 changes start*/
                /*SPR Fix 17959 start */
                /*SPR Fix 17959 end */
            /* SPR 9658 changes end*/
            /* ICIC changes start */
            /* Incase of Cell Edge user, we can specify the region to be used as 
             * any of CC/CE region or 
             * CE region to be preferred first, if we want to 
             * allocate with cell edge resource first and then move to cell center if cell edge fails */
            if ((CE_USER == locType) && 
                    (tempPUSCHResources[CE_REGION_PREFERRED] >= tempULHarqInfo_p->riLength))
            {
                regionAllocated = CE_REGION_PREFERRED; 
            }
            /* TTI Bundling changes Ends*/
            if ((tempPUSCHResources[regionAllocated] >= tempULHarqInfo_p->riLength))
                /* ICIC changes end */
            {
                /* This will allocate resources based on the regionAllocated chosen */
                retVal = allocatePUSCHResourcesForAdaptiveReTx(
                        tempULHarqInfo_p,
                        tempULHarqInfo_p->riLength,
                        tempULHarqInfo_p->strictRBAllocation,
                        &resourceReserved,
                        ulUEContext_p->csiContext.mcsToPrbMap_p,
                        rbMapNum,
                        FALSE,
                        regionAllocated
                        ,ulDelay,
                        currentGlobalTick,
                        ulUEContext_p,
                        internalCellIndex
                        );
                /* This will get executed in case of cell edge user and
                 * if contiguous cell edge resources were less than riLength
                 */
                if((MAC_FAILURE == retVal)&& 
                        CE_REGION_PREFERRED == regionAllocated) 
                {
                    regionAllocated = CC_CE_ANY_REGION;
                    retVal = allocatePUSCHResourcesForAdaptiveReTx(
                            tempULHarqInfo_p,
                            tempULHarqInfo_p->riLength,
                            tempULHarqInfo_p->strictRBAllocation,
                            &resourceReserved,
                            ulUEContext_p->csiContext.mcsToPrbMap_p,
                            rbMapNum,
                            FALSE,
                            regionAllocated
                            ,ulDelay,
                            currentGlobalTick,
                            ulUEContext_p,
                            internalCellIndex
                            );

                }


                if(MAC_SUCCESS == retVal) 
                    /* SS_S2 */
                {
                    LOG_MAC_MSG(MAC_NACK_ADAPTIVE_RES_ALLOC,LOGDEBUG,MAC_PUSCH,\
                            currentGlobalTick,\
                            ulUEContext_p->userLocationType,\
                            regionAllocated,\
                            resourceReserved.allocatedIndex,\
                            resourceReserved.allocatedRiv,\
                            resourceReserved.allocatedLength,\
                            0.0,0.0,\
                            __func__,"MAC_NACK_ADAPTIVE_RES_ALLOC");

                    /* Time Averaging changes start */
                    powerCorrectionAndTpc = getTpcForPusch(ulUEContext_p, currentGlobalTickWithULDelay, internalCellIndex);

                    /* +DYNAMIC_ICIC */
                    regionPowerMask = ulPowerMaskInfo_p->powerMask[ulUEContext_p->userLocationType];

                    CHECK_QCI_POWER_MASK_AND_UPDATE_REGION_MASK(ulUEContext_p,regionPowerMask);
                    /* -DYNAMIC_ICIC */

                    validateATB(tempULHarqInfo_p->riLength,\
                            &tempULHarqInfo_p->mcsIndex,\
                            ulUEContext_p, \
                            powerCorrectionAndTpc.powerCorrectionVal, \
                            &ueMaxPowerFlag,&ueMinPowerFlag, \
                            regionPowerMask,RE_TX, internalCellIndex);

                    /* SPR 10753 Changes */
                    tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS_AFTER_HI_ACK;
                    /* SPR 10753 Changes */

                    /*SPR 20784 Fix + */
                    tempULHarqInfo_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
                    /*SPR 20784 Fix - */
                    
                    /* Reset the tpcTriggeredValue in UE Context */
                    ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue = 
                        TPC_NO_CHANGE;

                    if (powerCorrectionAndTpc.powerCorrectionVal)
                    {
                        /* Cyclomatic Complexity changes - starts here */
                        checkAndUpdatePowerCorrection (ueMaxPowerFlag, 
                                ueMinPowerFlag, powerCorrectionAndTpc, ulUEContext_p
#ifdef PERF_STATS
                                ,tempUeIndex
#endif
                                );
                        /* Cyclomatic Complexity changes - ends here */
                    }

#ifdef ULHARQ_TIMER_PROC
                    ulHarqTimerStart(tempUeIndex,tempHarqProcessId,timerExpsubFrameNum,
                            /* + SPS_TDD_Changes */
                            DRA_SCHEDULING,
                            /* - SPS_TDD_Changes */
                            timerExpiredTTI,ulHarqSysFrameNum,
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */
#endif 
                    /*We use 0 for Retransmission */ 
                    ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;
                    updateDciCCEInfoArray(
                            ulUEContext_p,
                            tempDirectIndexingUEInfo_p->ulCCEIndex,
                            tempHarqProcessId,
                            tempDciCCEInfo_p,
                            &resourceReserved,
                            powerCorrectionAndTpc.tpcCommand,
                            /* +- SPR 17777 */
                            /* CR changes start */
                            hiIndex, /* SS_S2 */
                            /* CR changes end */
                            /* UL_MU_MIMO_CHG Start */  
                            tempDirectIndexingUEInfo_p->ulGrantRequestType,
                            INVALID_UE_INDEX,/*Coverity 96745 fix */ 
                            /* UL_MU_MIMO_CHG Changes End */    
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */

                    /* SPR 10027 FIX START */
                    /* Check if isDrbPresent = TRUE and Update the lastScheduledTick */
                    CHECK_AND_UPDATE_LAST_SCHEDULED_TICK(
                            tempULHarqInfo_p->isDrbPresent,ulUEContext_p)
                    /* SPR 10027 FIX END */

                    /* ICIC changes start */
                    tempPUSCHResources[regionAllocated] -= 
                        resourceReserved.allocatedLength;
                    if(CE_REGION_PREFERRED == regionAllocated)
                    {
                        tempPUSCHResources[CC_CE_REGION] -= resourceReserved.allocatedLength;
                        updateCCAvailRes(&rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                                &resourceReserved);
                    }
                    else
                    {
                        /*CA Changes start  */
                        updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                                tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                                internalCellIndex);
                        /*CA Changes end  */

                    }

                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_INFO,LOGDEBUG,MAC_UL_HARQ,\
                            getCurrentTick(),\
                            tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,\
                            tempPUSCHResources[CC_CE_REGION],\
                            tempPUSCHResources[CE_REGION],\
                            0,0.0,0.0,\
                            __func__,"NACK_RES_ALLOC");
                    /* ICIC changes end */
                    resourceReserved.allocatedLength = 0;

                    LOG_MAC_MSG(MAC_NACK_RES_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                            currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                            rbMapNum,ADAPTIVE_RE_TX,
                            tempULHarqInfo_p->currentRetransCount,
                            tempULHarqInfo_p->riLength,tempULHarqInfo_p->riStart,
                            __func__,"NACK_RES_ULHARQ_PASS");

                    LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                            tempUeIndex,
                            currentGlobalTick,
                            ADAPTIVE_RE_TX,
                            tempHarqProcessId,
                            tempULHarqInfo_p->riLength,
                            tempULHarqInfo_p->riStart,
                            tempDirectIndexingUEInfo_p->ulGrantRequestType,
                            tempULHarqInfo_p->deltabsrLoad,
                            TRUE,
                            tempULHarqInfo_p->mcsIndex,
                            0,
                            powerCorrectionAndTpc.tpcCommand 
                            /* UL_MU_MIMO_CHG START */
                            , ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                            tempDirectIndexingUEInfo_p->buddyUeIndex,
                            ulUEContext_p->userLocationType,
                            tempULHarqInfo_p->nDmrs,
                            ulUEContext_p->modulationScheme,
                            /* UL_MU_MIMO_CHG END */
                            /* CA Stats Changes Start */
                            internalCellIndex
                            /* CA Stats Changes End */
                            );

                } 
                else
                {
                    /*SPR Fix 17959 start */
                    if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                    {
                        addEntryTOHIPduArray(
                                tempULHarqInfo_p,
                                /* SPR 5467 changes start*/
                                HI_ACK,
                                /* SPR 5467 changes end*/
                                ulUEContext_p->crnti,
                                /* CR changes start */
                                ulUEContext_p,
                                tempULHarqInfo_p->sfUsed,
                                /* CR changes end */
                                tempHIPdu_p, 
                                internalCellIndex);
                    /* + SPR 6864 */
                        tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                    }
                        /*SPR Fix 17959 end */
                    if (ULSCH_UCI_INVALID_INDEX !=
                            tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
                    {
                      /* + SPR_14994 */
                        checkAndRestoreUCIPduType(ulUEContext_p, tempDirectIndexingUEInfo_p,
                                                ulschUciRachInfo_p,ulSubFrameNum,
                                                /*SPR 19504 fix start*/    
                                                rbMapNum,
                                                internalCellIndex);
                                                /*SPR 19504 fix end*/    
                      /* - SPR_14994 */
                    }
                    /* - SPR 6864 */

                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_FAIL_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                            currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                            rbMapNum, 
                            tempULHarqInfo_p->nextReTransmissionType,
                            tempULHarqInfo_p->currentRetransCount,
                            0.0,0.0,__func__,"NACK_RES_ALLOC_FAIL");

                    /*
                       We now revert back the  cce allocated 
                       by PDCCH as the allocation was not done
                     */

                    /* SPR 5467 changes start*/
                    /* SPR 14279 Changes Start */
                    /*    tempULHarqInfo_p->currentRetransCount++; */
                    updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
                    /* SPR 14279 Changes End */
                    tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                    /* SPR 5467 changes end*/
                    /* SPR 10753 Changes */
                    tempULHarqInfo_p->forceAckNextRetxType = ADAPTIVE_RETRANS_AFTER_HI_ACK;
                    /* SPR 10753 Changes */

                    /* SPR 9658 changes start*/
                    tempULHarqInfo_p->riStart = INVALID_RI_START;
                    /* SPR 9658 changes end*/

                    /* +COVERITY 10718 */
                /* SPR 20929 FIX Start */
                    if ((tempDirectIndexingUEInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                            (INVALID_CCE_INDEX != tempDirectIndexingUEInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
                    {
                        mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].\
                                dciPduInfo.aggregationLvl,
                                (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex]),
                                internalCellIndex); 
                    }
                    /* -COVERITY 10718 */
                    /* SPR 5467 changes start*/
                    /* SPR 5557 changes start*/
                    /* SPR 5557 changes end*/
                    tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                    /* reserve the last given resources as UE will assume NON Adaptive
                       ReTx */
                    resAllocFailure = TRUE;
                    /* SPR 5467 changes end*/
                    /* CC resouces are updated always irrespective of user location,
                     * we check here whether the RBs lied in CE region and update the 
                     * resouce info if necessary*/
                    /*CA Changes start  */
                    ceOverlapRBs = updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                            tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                            internalCellIndex);
                    /*CA Changes end  */
                    if(0 < ceOverlapRBs) 
                    {
                        tempPUSCHResources[CE_REGION] -= ceOverlapRBs;
                    }
                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_INFO,LOGDEBUG,MAC_UL_HARQ,\
                            getCurrentTick(),\
                            tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,\
                            tempPUSCHResources[CC_CE_REGION],\
                            tempPUSCHResources[CE_REGION],\
                            0,0.0,0.0,\
                            __func__,"NACK_RES_ALLOC");
                    /* ICIC changes end */
#ifdef ULHARQ_TIMER_PROC
                    ulHarqTimerStart(tempUeIndex,tempHarqProcessId,timerExpsubFrameNum,
                            /* + SPS_TDD_Changes */
                            DRA_SCHEDULING,
                            /* - SPS_TDD_Changes */
                            timerExpiredTTI,ulHarqSysFrameNum,
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */
#endif 
                    /* CR changes start */
                    /*CA Changes start  */
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                        ~(ONE << tempULHarqInfo_p->nPhichSeq);
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                    /*CA Changes end  */
                    tempULHarqInfo_p->sfUsed = hiIndex;
                    /* CR changes end */
                    /* SPR 5467 changes start*/
                    /* SPR 5467 changes end*/
                }
            }
            else
            {
                    /*SPR Fix 17959 start */
                    if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                    {
                        addEntryTOHIPduArray(
                                tempULHarqInfo_p,
                                /* SPR 5467 changes start*/
                                HI_ACK,
                                /* SPR 5467 changes end*/
                                ulUEContext_p->crnti,
                                /* CR changes start */
                                ulUEContext_p,
                                tempULHarqInfo_p->sfUsed,
                                /* CR changes end */
                                tempHIPdu_p, 
                                internalCellIndex);
                        /* + SPR 6864 */
                        tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                    }
                        /*SPR Fix 17959 end */
                /* + SPR 6864 */
                if (ULSCH_UCI_INVALID_INDEX !=
                        tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
                {
                      /* + SPR_14994 */
                    checkAndRestoreUCIPduType(ulUEContext_p, tempDirectIndexingUEInfo_p, 
                                             ulschUciRachInfo_p,ulSubFrameNum,
                                             /*SPR 19504 fix start*/    
                                             rbMapNum,
                                             internalCellIndex);
                                             /*SPR 19504 fix end*/    
                      /* - SPR_14994 */
                }
                /* - SPR 6864 */

                /*
                   We now revert back the  cce allocated by PDCCH 
                   as no more PUSCH resources are avaliable
                   */
                /* SPR 5467 changes start*/
                /* SPR 14279 Changes Start */
                /*    tempULHarqInfo_p->currentRetransCount++; */
                updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
                /* SPR 14279 Changes End */
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                /* SPR 5467 changes end*/

                /* + COVERITY 10718 */
                /* SPR 20929 FIX Start */
                if ((tempDirectIndexingUEInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                        (INVALID_CCE_INDEX != tempDirectIndexingUEInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
                {
                    mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].\
                            dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex]),
                            internalCellIndex); 
                }
                /* - COVERITY 10718 */
                /* SPR 5467 changes start*/
                /* SPR 5557 changes start*/
                /* SPR 5557 changes end*/
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                resAllocFailure = TRUE;


                /* SPR 5467 changes end*/ 
                /* Required resources are not available, hence increase the current
                 * ReTx count and start the UL Timer */
#ifdef ULHARQ_TIMER_PROC
                ulHarqTimerStart(tempUeIndex,tempHarqProcessId,timerExpsubFrameNum,
                        /* + SPS_TDD_Changes */
                        DRA_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        timerExpiredTTI,ulHarqSysFrameNum,
                        /*CA Changes start  */
                        internalCellIndex);
                        /*CA Changes end  */
#endif 
                /* CR changes start */
                /*CA Changes start  */
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                    ~(ONE << tempULHarqInfo_p->nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                /*CA Changes end  */
                tempULHarqInfo_p->sfUsed = hiIndex;
                /* CR changes end */
                LOG_MAC_MSG(MAC_NACK_RES_ZERO_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        rbMapNum, 
                        tempULHarqInfo_p->nextReTransmissionType,
                        tempULHarqInfo_p->currentRetransCount,
                        0.0,0.0,__func__,"NACK_RES_ZERO_FAIL");
                /* SPR 5467 changes start*/
                /* SPR 5467 changes end */              
            } 
            /*We use 0 for Retransmission */ 
            ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 0;

            /* SPR 5467 changes start*/
            if(resAllocFailure != TRUE)
            {
                (*numberOfULUEsToBeScheudled_p)--;
                /* SPR 5467 changes end*/

                /* Semi static changes Start*/
                if(ULSCH_UCI_INVALID_INDEX != tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
                {
                    /* Fill UL Harq Info*/
                    fillSemiStaticInfoForULSCH (ulschUciRachInfo_p,
                            tempDirectIndexingUEInfo_p,
                            ulUEContextInfo_p,
                            tempULHarqInfo_p,
                            tempHarqProcessId,
    			    /* SPR 9173 changes start */
                            ulSubFrameNum,0, internalCellIndex);
    			    /* SPR 9173 changes end */
                }
                else
                {
                    if ( MAX_ULSCH_UCI_SUPPORTED ==  numberOfInfoPresent)
                    {
                        ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                                __func__);
                        break;
                    }
                    /*SPR 7086 Fix Start*/
                    /* SPR 10195 Fix Start */
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS =
                        nSRSDetermineOnCollision(ulUEContext_p, tempHarqProcessId, ulSubFrameNum, internalCellIndex);
                    /* SPR 10195 Fix End */
                    /*SPR 7086 Fix End*/

                    /* Fill UL Harq Info*/
                    ulschUciRachInfo_p->
                        ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p = 
                        tempULHarqInfo_p;
                    ulschUciRachInfo_p->
                        ulschUciInfo[numberOfInfoPresent].ulHarqProcessId =
                        tempHarqProcessId;
                    ulschUciRachInfo_p->
                        ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_ULSCH;

                    /** For TTIB_Code Start */
                    /* SPR 13526 fix start */
                    /* + SPS_TDD_Changes */
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti =
                            ulUEContext_p->crnti;
                    /* - SPS_TDD_Changes */
                    numberOfInfoPresent++;

                    LOG_UT(MAC_UCI_ULSCH_PDU_ASSIGN,LOGDEBUG,MAC_PUSCH,\
                            getCurrentTick(),\
                            tempUeIndex,
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType,\
                            tempDirectIndexingUEInfo_p->ulschUciInfoIndex,\
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId,\
                            0,0.0,0.0,\
                            __func__,"ULSCH_PDU_ASSIGN");
                }
                /* Semi static changes End*/

                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK;
            }
            /* SPR 5467 changes end*/
        }
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, 
                "\ntempPUSCHResources = %d numberOfULUEsToBeScheudled_p = %d\n"
                ,tempPUSCHResources,*numberOfULUEsToBeScheudled_p);
    }

    /*Update Number of numberOfInfoPresent in the global context */ 
    ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;
}
#elif TDD_CONFIG
/******************************************************************************
 * Function Name    : processULNackAdaptiveInPUSCH
 * Inputs           : tempPdcchULOutput_p - The pointer to PdcchULOutputInfo,
 *                    rbMapNum,
 *                    subFrameNumber
 *                    currentGlobalTick - The present Global Tick,
 *                    numberOfULUEsToBeScheudled_p - Pointer to no. of UL UE's to be
 *                                                   scheduled,
 *                    containerTick - The Tick of the container being used,
 *                    locType,
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function will read the Nack Adaptive queue and update the 
 *                    HARQ process, and book the resources in advance. It will 
 *                    also create the HI PDUs. 
 * ******************************************************************************/
 void processULNackAdaptiveInPUSCH(
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
        UInt8 ulSubFrameNum)
        /*TDD Config 0 Changes End*/ 

/* ICIC changes start */
{
    UInt16 tempUeIndex=0;
    HIPduArray* tempHIPdu_p  = PNULL;
    AllocatedResInfo resourceReserved = {0};
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; /* Invalid Value*/
    DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    ULUEContext* ulUEContext_p = PNULL;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    UInt8 localCount = 0;
    ULHarqInfo* tempULHarqInfo_p            = PNULL;
    PdcchULNackUEInfo *pdcchULNackUEInfo_p  = PNULL;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    /* ICIC changes start */
    /* Initialized by any region and changed in case of Cell edge user 
     * as CE_REGION_PREFERRED if configured so */
    UInt8 regionAllocated = CC_CE_ANY_REGION;
    RbMapNode *rbMapNode_p = PNULL;
    UInt32 tempPUSCHResources[MAX_USER_LOCATION] = {0};
    MacRetType   retVal = MAC_FAILURE;
    UInt8 ceOverlapRBs = 0;
    /* msg3_retx_tdd_support_start */
    TempCRNTICtx *tempUEContext_p = PNULL;
    PdcchULNackUEInfo *pdcchULNackTempUEInfo_p  = PNULL;
    /* msg3_retx_tdd_support_end */

    /* ICIC changes end */
    /* TDD Config 6 Changes Start */
    UInt8 prevUlSubFrameNum  = 0;
    UInt16 prevUlSysFrameNum = 0;
    /* TDD Config 6 Changes End */
    /*TDD Config 0 Changes Start*/
    UInt8 subFrameNum = currentGlobalTick % MAX_SUB_FRAME;
    UInt8 ulSubFrameOffset =  getUlOffset(subFrameNum,ulSubFrameNum);
    /* SPR 15909 fix start */
    tickType_t ulGlobalTick = currentGlobalTick + ulSubFrameOffset;
    /* SPR 15909 fix end */
    UInt8 iPhich    = 0;
    UInt8 phichSubFrameNum = 0;
    /*TDD Config 0 Changes End*/
    /* SPR 9173 fix start */
    UInt16 ulSysFrameNum = 0;
    /*SPR 9173 fix end */

    UeScheduledInfo *ueScheduledInfo_p = PNULL;
    /*CA Changes start  */
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    UInt16 sysFrameNum = (currentGlobalTick / MAX_SUB_FRAME) % MAX_SFN_VALUE;
    /* Time Averaging changes start */
    UInt32 ueMaxPowerFlag= 0;
    UInt32 ueMinPowerFlag= 0;

    UInt16  numberOfInfoPresent = 0;
    /* +DYNAMIC_ICIC */
    /*CA Changes start  */
    ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + ulPowerMaskIndex_g[internalCellIndex];
    /*CA Changes end  */
    SInt32 regionPowerMask = INVALID_UL_POWER_MASK;
    /* -DYNAMIC_ICIC */
    /*TDD Config 0 Changes Start*/
    ULSchUciRachPoolInfo* ulschUciRachInfo_p
        /*CA Changes start  */
        = ulSchUciRachContainer_gp[internalCellIndex] +(ulSubFrameNum % MAX_ULSCH_UCI_CONTAINER) ;
        /*CA Changes end  */

            /* SPR 10803 [BUG 848] FIX START */
 /* FORCED_ACK SPR#9391 Changes Start */
    UInt8 resAllocFailure = 0;
 /* FORCED_ACK SPR#9391 Changes End */
            /* SPR 10803 [BUG 848] FIX END */

    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    /* msg3_retx_tdd_support_start */
    UInt8 ulDelay = 0;
    /*TDD Config 0 Changes Start*/ 
    /* SPR 15909 fix start */
    tickType_t timerExpiredTTI = currentGlobalTick  + ulSubFrameOffset;
    /* SPR 15909 fix end */
    /* SPR 4386 changes start */
    UInt8 timerExpsubFrameNum = subFrameNumber + ulSubFrameOffset;
    /*TDD Config 0 Changes End*/ 
    UInt32 ulHarqSysFrameNum = 0;
    /*SPR_11092 Start*/
    /*-coverity CID-63832-*/
    ulHarqSysFrameNum = (MODULO_ONEZEROTWOFOUR((currentGlobalTick)/MAX_SUB_FRAME));
    /*-coverity CID-63832-*/
    /*SPR_11092 End*/
    /* SPR 4386 changes end */
    if(timerExpsubFrameNum >= MAX_SUB_FRAME)
    {
        timerExpsubFrameNum = timerExpsubFrameNum % MAX_SUB_FRAME;

        ulHarqSysFrameNum++;
        if (ulHarqSysFrameNum == MAX_SFN_VALUE)
        {
            ulHarqSysFrameNum = 0;
        }
    }
    /* msg3_retx_tdd_support_end */


    /* TDD Config 6 Changes Start */
    if (ulSubFrameNum < subFrameNumber)
    {
        ulSysFrameNum = MODULO_ONEZEROTWOFOUR((sysFrameNum + 1));
    }
    else
    {
        ulSysFrameNum = sysFrameNum;
    }
    /*SPR 9173 fix end */
    /*TDD Config 0 Changes Start*/
    prevUlSubFrameNum = (ulGlobalTick - getprevUlSfOffsetFromCurrentUlSf(ulSubFrameNum,
                        /* CA TDD CHANGES START */
                        internalCellIndex
                        /* CA TDD CHANGES END */
                        )) % MAX_SUB_FRAME;
    GET_UL_SYS_FRAME_NUM_FROM_PHICH(prevUlSubFrameNum, subFrameNumber, prevUlSysFrameNum, sysFrameNum);
    /* TDD Config 0 Changes End */
#ifdef MAC_AUT
    prevUlSubFrameNo = prevUlSubFrameNum;
    ulFrameOffset = ulSubFrameOffset;
#endif
    

    /* We now process the NACK Queue Load */


    LTE_MAC_UT_LOG( LOG_MAJOR, LOG_PHICH,
            "[%s] Entry rbMapNum[%d] subFrameNumber [%d] currentGlobalTick [%d] containerTick [%d] \n",
            __func__, rbMapNum, subFrameNumber, currentGlobalTick, containerTick);

    /* ICIC changes start */
    /*CA Changes start  */
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    /* Available PUSCH resources are fetched for each location type */
    tempPUSCHResources[CC_CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter; 
    tempPUSCHResources[CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter; 
    /* ICIC changes end */


    LTE_MAC_UT_LOG( LOG_MAJOR, LOG_PHICH, "\n########\n containerTick = %d "
            " tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount = %d \n"
            " tempPUSCHResources = %d  rbMapNum = %d\n"
            " numberOfULUEsToBeScheudled_p = %d\n########\n"
            ,containerTick, tempPdcchULOutput_p->pdcchULNackUEInfo.adaptiveCount[locType], 
            tempPUSCHResources, rbMapNum,*numberOfULUEsToBeScheudled_p);

    pdcchULNackUEInfo_p = &(tempPdcchULOutput_p->pdcchULNackUEInfo);
    pdcchULNackTempUEInfo_p = &(tempPdcchULOutput_p->pdcchULNackTempUEInfo);
    
    /* msg3_retx_tdd_support_start */
    localCount = pdcchULNackTempUEInfo_p->adaptiveCount[locType];
    while(localCount--)
    {
       tempUeIndex = \
                   tempPdcchULOutput_p->pdcchULNackTempUEInfo.
                   pdcchULNackAdaptiveUEArray[locType][localCount];
       tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[tempUeIndex].tempCRNTICtx_p
       LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "\ntempUeIndex = %d",tempUeIndex);


       if ((tempUEContext_p != PNULL) && (RAR_SENT == tempUEContext_p->status))
      {
           processULNackForAdaptiveReTxOfMsg3Tdd(tempUeIndex, containerTick, tempUEContext_p,
                 &resourceReserved, currentGlobalTick,ulDelay,rbMapNum,
                 numberOfULUEsToBeScheudled_p, &tempPUSCHResources[CC_CE_REGION],
                 timerExpiredTTI, timerExpsubFrameNum,
                 ulHarqSysFrameNum,
                 &numberOfInfoPresent,
                 regionAllocated,
                 internalCellIndex,
                 /*TDD Config 0 Changes Start*/
                 ulSubFrameNum
                 );
                 /*TDD Config 0 Changes End*/ 
      }

    } 

    /* msg3_retx_tdd_support_end */
    /* Adaptive Handling*/
    localCount = pdcchULNackUEInfo_p->adaptiveCount[locType];

    while(localCount--)
    {
            /* SPR 10803 [BUG 848] FIX START */
        resAllocFailure = FALSE;
            /* SPR 10803 [BUG 848] FIX END */
        tempUeIndex = \
                      pdcchULNackUEInfo_p->pdcchULNackAdaptiveUEArray[locType][localCount];

        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];         
        /* SPR 11548 Fix Start */
        /* resetting the resAllocFailure to FALSE */ 
        resAllocFailure = FALSE;
        /* SPR 11548 Fix End */

        if (!ulUEContextInfo_p->pendingDeleteFlag)
        {
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;

            if ( !isValidSubframeNumForDCI_0(subFrameNumber, internalCellIndex))
            {
                ltePanic("Invalid DL Subframe[%d] For DCI 0 \n",subFrameNumber);
            }

            /*SPR 13925 fix start*/ 
            /** SPR 14204 Fix : Start **/
            ueScheduledInfo_p = ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulSubFrameNum];
            /** SPR 14204 Fix : End **/
            /*SPR 13925 fix end*/ 

            if ( !ueScheduledInfo_p )
            {
                ltePanic("[%s] ueScheduledInfo_p is null\n",
                        __func__);
            }
            tempHarqProcessId = ueScheduledInfo_p->ulHarqProcessId;

            /*
               We now create the HI PDU corresponding to the Nack received
               */
            LTE_MAC_UT_LOG( LOG_MAJOR, LOG_PHICH,
                    "Entry in HI PDU in PHICH module for hpID[%d]\n",
                    tempHarqProcessId);
            tempULHarqInfo_p = \
                               &(ulUEContext_p->ulresInfo[tempHarqProcessId]);
            
            /*TDD Config 0 Changes Start*/
            tempULHarqInfo_p->isPdcchAllocated = 0;
            /*TDD Config 0 Changes End*/
            /* MEAS_GAP_CHG */
            /* SPR 10803 [BUG 848] FIX START */
            if ((ueScheduledInfo_p->flag ==
                    MEAS_GAP_RE_TX_NO_CCE_NO_GRANT)
                /* FORCED_ACK SPR#9374 Changes Start */
                || (ueScheduledInfo_p->flag == ADAPTIVE_RETRANS_NO_CCE))
                    /* FORCED_ACK SPR#9374 Changes End */
            /* SPR 10803 [BUG 848] FIX END */
            {

                /* + SPR 6864 */
                if (ULSCH_UCI_INVALID_INDEX !=
                        ueScheduledInfo_p->ulschUciInfoIndex)
                {
                      /* + SPR_14994 */
                    checkAndRestoreUCIPduType(ulUEContext_p, ueScheduledInfo_p, 
                          ulSysFrameNum, ulschUciRachInfo_p,ulSubFrameNum,
                          /*SPR 19504 fix start*/    
                          rbMapNum,
                          internalCellIndex);
                         /*SPR 19504 fix end*/    
                      /* - SPR_14994 */
                }
                /* - SPR 6864 */


                tempULHarqInfo_p->currentRetransCount++;
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX; 
		/* SPR 13196 fix start */
                tempULHarqInfo_p->newDataIndicator = 0;
		/* SPR 13196 fix end */

                /* TDD Config 6 Changes Start */
                resetHarqId(prevUlSubFrameNum, ulUEContext_p,internalCellIndex, prevUlSysFrameNum);
                setHarqBusy(ulSubFrameNum, tempHarqProcessId, ulUEContext_p,internalCellIndex, ulSysFrameNum);
                /* TDD Config 6 Changes End */

                ulHarqTimerStart(
                        subFrameNumber,
                        sysFrameNum,
                        tempUeIndex,
                        tempHarqProcessId,
                        /* + SPS_TDD_Changes */
                        DRA_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        /*CA Changes start  */
                        internalCellIndex,
                        /*CA Changes end  */
                        /* TDD Config 0 Changes Start */
                        ulSubFrameNum
                        /* TDD Config 0 Changes End */
                        );

                if ((ueScheduledInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) &&
                        /* +COVERITY 10718 */
                        (INVALID_CCE_INDEX != ueScheduledInfo_p->ulCCEIndex))
                    /* -COVERITY 10718 */
                {
                    mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                            dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                            internalCellIndex);
                }

                ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                /*SPR Fix 12758 start */
                if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                {
                   /*SPR Fix 12758 end */

                addEntryTOHIPduArray(
                        &ulUEContext_p->ulresInfo[tempHarqProcessId],
            /* SPR 10803 [BUG 848] FIX START */
	 /* FORCED_ACK SPR#9391 Changes Start */
			            HI_ACK, /*HI_NACK,*/
	 /* FORCED_ACK SPR#9391 Changes End */
            /* SPR 10803 [BUG 848] FIX END */
                        ulUEContext_p->crnti,
                        tempHIPdu_p,
                        internalCellIndex,
                        /* TDD Config 0 Changes Start */
                        /* +- SPR 17777 */
                        prevUlSubFrameNum);
                        /* TDD Config 0 Changes End */
                   /*SPR Fix 12758 start */
                   tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                   /*SPR Fix 12758 end */
                }
                /*SPR 19504 fix start*/
                /*SPR 20145 fix start*/
                //cleanUpUeScheduledInfo(ueScheduledInfo_p);
                /*SPR 20145 fix end*/
                /*SPR 19504 fix end*/
                continue;
            }
            /* MEAS_GAP_CHG */

            /* ICIC changes start */
            /* Incase of Cell Edge user, we can specify the region to be used as
             * any of CC/CE region or
             * CE region to be preferred first, if we want to
             * allocate with cell edge resource first and then move to cell center if cell edge fails */
            if ((CE_USER == locType) && 
                    (tempPUSCHResources[CE_REGION_PREFERRED] >= tempULHarqInfo_p->riLength))
            {
                regionAllocated = CE_REGION_PREFERRED;
            }


            if (tempPUSCHResources[regionAllocated] >= tempULHarqInfo_p->riLength)
            {       
                /* This will allocate resources based on the regionAllocated chosen */
                retVal = allocatePUSCHResourcesForAdaptiveReTx(
                        tempULHarqInfo_p,
                        tempULHarqInfo_p->riLength,
                        tempULHarqInfo_p->strictRBAllocation,
                        &resourceReserved,
                        ulUEContext_p->csiContext.mcsToPrbMap_p,
                        rbMapNum,
                        FALSE,
                        regionAllocated,
                        internalCellIndex,
                        /*TDD Config 0 Changes Start*/ 
                        ulSubFrameNum
                        );
                        /*TDD Config 0 Changes End*/ 
                /* This will get executed in case of cell edge user and
                 * if contiguous cell edge resources were less than riLength
                 */
                if((MAC_FAILURE == retVal)&& 
                        CE_REGION_PREFERRED == regionAllocated) 
                {
                    regionAllocated = CC_CE_ANY_REGION;
                    retVal = allocatePUSCHResourcesForAdaptiveReTx(
                            tempULHarqInfo_p,
                            tempULHarqInfo_p->riLength,
                            tempULHarqInfo_p->strictRBAllocation,
                            &resourceReserved,
                            ulUEContext_p->csiContext.mcsToPrbMap_p,
                            rbMapNum,
                            FALSE,
                            regionAllocated,
                            internalCellIndex,
                            /*TDD Config 0 Changes Start*/ 
                            ulSubFrameNum
                            );
                            /*TDD Config 0 Changes End*/

                }
                if(MAC_SUCCESS == retVal)
                {
                    LOG_MAC_MSG(MAC_NACK_ADAPTIVE_RES_ALLOC,LOGDEBUG,MAC_PUSCH,\
                            currentGlobalTick,\
                            ulUEContext_p->userLocationType,\
                            regionAllocated,\
                            resourceReserved.allocatedIndex,\
                            resourceReserved.allocatedRiv,\
                            resourceReserved.allocatedLength,\
                            0.0,0.0,\
                            __func__,"MAC_NACK_ADAPTIVE_RES_ALLOC");

                    /* Time Averaging changes start */
                    powerCorrectionAndTpc = getTpcForPusch(ulUEContext_p, currentGlobalTick, internalCellIndex,
                                                           /*TDD Config 0 Changes Start*/ 
                                                           ulSubFrameNum);
                                                           /*TDD Config 0 Changes End*/ 

                    /* +DYNAMIC_ICIC */
                    regionPowerMask = ulPowerMaskInfo_p->powerMask[ulUEContext_p->userLocationType];
                    CHECK_QCI_POWER_MASK_AND_UPDATE_REGION_MASK(ulUEContext_p,regionPowerMask);

                    validateATB(tempULHarqInfo_p->riLength,\
                            &tempULHarqInfo_p->mcsIndex,\
                            ulUEContext_p, \
                            powerCorrectionAndTpc.powerCorrectionVal, \
                            &ueMaxPowerFlag,&ueMinPowerFlag, \
                            regionPowerMask,RE_TX,internalCellIndex);
                    /* -DYNAMIC_ICIC */
                    /* SPR 10753 Changes Start */
                    tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS_AFTER_HI_ACK;
                    /* SPR 10753 Changes End */
                    
                    /*SPR 20784 Fix + */
                    tempULHarqInfo_p->nextReTransmissionType = NON_ADAPTIVE_RE_TX;
                    /*SPR 20784 Fix - */

                    /* Reset the tpcTriggeredValue in UE Context */
                    ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue = 
                        TPC_NO_CHANGE;

                    if (powerCorrectionAndTpc.powerCorrectionVal)
                    { 
                        ulUEContext_p->prevPowerCorrectionForPusch =
                            ulUEContext_p->currPowerCorrectionForPusch;

                        checkAndUpdateFiCorrection(
                                ueMaxPowerFlag,
                                ueMinPowerFlag,
                                powerCorrectionAndTpc,
                                ulUEContext_p);


                    }
                    /*We use 0 for Retransmission */ 
                    /* SPR 2183 Fixes Begins */
                    tempULHarqInfo_p->newDataIndicator = 0;            
                    /* SPR 2183 Fixes Ends */
                    /*TDD Config 0 Changes Start*/
                    /* Block the phich resources for the given ristart and
                     * ndmrs*/
                    phichSubFrameNum = ulSubFrameNum + getHiPduSubframeNum(ulSubFrameNum,internalCellIndex);
                    phichSubFrameNum = phichSubFrameNum % MAX_SUB_FRAME;
                    iPhich = calculateIPhichValue(ulSubFrameNum,internalCellIndex);
                    /*Reserve the HI resources and update the harq Info*/
                    reserveHiAndUpdateHarqInfo(iPhich,
                                               tempULHarqInfo_p->riStart,
                                               tempULHarqInfo_p->nDmrs,
                                               phichSubFrameNum,
                                               tempULHarqInfo_p,
                                               internalCellIndex);
                    /* TDD Config 0 Changes End */

                    updateDciCCEInfoArray(
                            ulUEContext_p,
                            ueScheduledInfo_p->ulCCEIndex,
                            tempHarqProcessId,
                            tempDciCCEInfo_p,
                            &resourceReserved,
                            powerCorrectionAndTpc.tpcCommand,
                            /* +- SPR 17777 */
                            /* UL_MU_MIMO_CHG Start */
                            INVALID_UE_INDEX,
                            /* UL_MU_MIMO_CHG End */
                            /* +- SPR 17777 */
                            //SPR_16896_Changes
                            FALSE,
                            //SPR_16896_Changes
                            /*CA Changes start  */
                            internalCellIndex,
                            /* TDD Config 0 Changes Start */
                            /* +- SPR 17777 */
                            currentGlobalTick,
                            ulSubFrameNum,
                            /*Scheduled count is applicable for New UEs only*/
                            INVALID_COUNT);
                            /* TDD Config 0 Changes End */

                    /* SPR 10027 FIX START */
                    /* Check if isDrbPresent = TRUE and Update the lastScheduledTick */
                    CHECK_AND_UPDATE_LAST_SCHEDULED_TICK(
                            tempULHarqInfo_p->isDrbPresent,ulUEContext_p)
                    /* SPR 10027 FIX END */

                    /* ICIC changes start */
                    tempPUSCHResources[regionAllocated] -= 
                        resourceReserved.allocatedLength;
                    if(CE_REGION_PREFERRED == regionAllocated)
                    {
                        tempPUSCHResources[CC_CE_REGION] -= resourceReserved.allocatedLength;
                        updateCCAvailRes(&rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                                &resourceReserved);
                    }
                    else
                    {
                        /*CA Changes start  */
                        ceOverlapRBs = updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                                tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                                internalCellIndex);
                        /*CA Changes end  */
                        tempPUSCHResources[CE_REGION] -= ceOverlapRBs;


                    }

                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_INFO,LOGDEBUG,MAC_UL_HARQ,\
                            getCurrentTick(),\
                            tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,\
                            tempPUSCHResources[CC_CE_REGION],\
                            tempPUSCHResources[CE_REGION],\
                            0,0.0,0.0,\
                            __func__,"NACK_RES_ALLOC");
                    /* ICIC changes end */
                    resourceReserved.allocatedLength = 0;

                    /*  LOG_MAC_MSG(MAC_NACK_RES_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        rbMapNum,ADAPTIVE_RE_TX,
                        tempULHarqInfo_p->currentRetransCount,
                        tempULHarqInfo_p->riLength,tempULHarqInfo_p->riStart,
                        ,__func__,"NACK_RES_ULHARQ_PASS");
                        */
                    LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                            tempUeIndex,
                            currentGlobalTick,
                            ADAPTIVE_RE_TX,
                            tempHarqProcessId,
                            tempULHarqInfo_p->riLength,
                            tempULHarqInfo_p->riStart,
                            ueScheduledInfo_p->ulGrantRequestType,
                            tempULHarqInfo_p->deltabsrLoad,
                            TRUE,
                            tempULHarqInfo_p->mcsIndex,
                            0,
                            powerCorrectionAndTpc.tpcCommand
                            /* UL_MU_MIMO_CHG START */
                            , ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                            INVALID_UE_INDEX,
                            ulUEContext_p->userLocationType,
                            tempULHarqInfo_p->nDmrs,
                            ulUEContext_p->modulationScheme,
                            /* UL_MU_MIMO_CHG END */
                            /* CA Stats Changes Start */
                            internalCellIndex
                            /* CA Stats Changes End */
                            );

                } 
                else
                {
                   /*SPR Fix 12758 start */
                  if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                  { 
                     addEntryTOHIPduArray(
                           tempULHarqInfo_p,
                           /* SPR 10803 [BUG 848] FIX START */
                           /* FORCED_ACK SPR#9391 Changes Start */ 
                           HI_ACK,/*HI_NACK,*/
                           /* FORCED_ACK SPR#9391 Changes End */
                           /* SPR 10803 [BUG 848] FIX END */
                           ulUEContext_p->crnti,
                           tempHIPdu_p,
                           internalCellIndex,
                           /* TDD Config 0 Changes Start */
                           /* +- SPR 17777 */
                           prevUlSubFrameNum);
                           /* TDD Config 0 Changes End */
                     tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                  }
                  /*SPR Fix 12758 end*/

                    /* + SPR 6864 */
                    if (ULSCH_UCI_INVALID_INDEX !=
                            ueScheduledInfo_p->ulschUciInfoIndex)
                    {
                      /* + SPR_14994 */
                        checkAndRestoreUCIPduType(ulUEContext_p, ueScheduledInfo_p,
                                   ulSysFrameNum, ulschUciRachInfo_p, ulSubFrameNum,
                                   /*SPR 19504 fix start*/    
                                   rbMapNum,
                                   internalCellIndex);
                                   /*SPR 19504 fix end*/    
                      /* - SPR_14994 */
                    }
                    /* - SPR 6864 */

            /* SPR 10803 [BUG 848] FIX START */
	 /* FORCED_ACK SPR#9391 Changes Start */
                tempULHarqInfo_p->currentRetransCount++;
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
	 /* FORCED_ACK SPR#9391 Changes End */
            /* SPR 10803 [BUG 848] FIX END */
                    /* SPR 10753 Changes Start */
                    tempULHarqInfo_p->forceAckNextRetxType = ADAPTIVE_RETRANS_AFTER_HI_ACK;
                    /* SPR 10753 Changes End */

                    /* required resources are not available, hence increment the 
                     * current ReTx count and start UL Timer*/
                    /* +COVERITY 10718 */
                /* SPR 20929 FIX Start */
                    if ((ueScheduledInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                            (INVALID_CCE_INDEX != ueScheduledInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
                    {
                        mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                                dciPduInfo.aggregationLvl,
                                (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                                internalCellIndex);
                    }
                    /* -COVERITY 10718 */
            /* SPR 10803 [BUG 848] FIX START */

	 /* FORCED_ACK SPR#9391 Changes Start */
                ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                resAllocFailure = TRUE;
	 /* FORCED_ACK SPR#9391 Changes End */
                    /* CC resouces are updated always irrespective of user location,
                     * we check here whether the RBs lied in CE region and update the 
                     * resouce info if necessary*/
                    /*CA Changes start  */
                    /*ceOverlapRBs = updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                            tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                            internalCellIndex);*/
                    /*CA Changes end  */
                    /*if(0 < ceOverlapRBs) 
                    {
                        tempPUSCHResources[CE_REGION] -= ceOverlapRBs;
                    }*/
                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_INFO,LOGDEBUG,MAC_UL_HARQ,\
                            getCurrentTick(),\
                            tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,\
                            tempPUSCHResources[CC_CE_REGION],\
                            tempPUSCHResources[CE_REGION],\
                            0,0.0,0.0,\
                            __func__,"NACK_RES_ALLOC");
                    /* ICIC changes end */

                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_FAIL_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                            currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                            rbMapNum, 
                            tempULHarqInfo_p->nextReTransmissionType,
                            tempULHarqInfo_p->currentRetransCount,
                            0.0,0.0,__func__,"NACK_RES_ALLOC_FAIL");

                    LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                            tempUeIndex,
                            currentGlobalTick,
                            NON_ADAPTIVE_RE_TX,
                            tempHarqProcessId,
                            tempULHarqInfo_p->riLength,
                            tempULHarqInfo_p->riStart,
                            ueScheduledInfo_p->ulGrantRequestType,
                            tempULHarqInfo_p->deltabsrLoad,
                            FALSE,
                            tempULHarqInfo_p->mcsIndex,
                            0,
                            TPC_VALUE_1 
                            /* UL_MU_MIMO_CHG START */
                            , ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                            INVALID_UE_INDEX,
                            ulUEContext_p->userLocationType,
                            tempULHarqInfo_p->nDmrs,
                            ulUEContext_p->modulationScheme,
                            /* UL_MU_MIMO_CHG END */
                            /* CA Stats Changes Start */
                            internalCellIndex
                            /* CA Stats Changes End */
                            );             
                }
            }
            else
            {
                   /*SPR Fix 12758 start */
                  if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                  { 
                     addEntryTOHIPduArray(
                           tempULHarqInfo_p,
                           /* SPR 10803 [BUG 848] FIX START */
                           /* FORCED_ACK SPR#9391 Changes Start */ 
                           HI_ACK,/*HI_NACK,*/
                           /* FORCED_ACK SPR#9391 Changes End */
                           /* SPR 10803 [BUG 848] FIX END */
                           ulUEContext_p->crnti,
                           tempHIPdu_p,
                           internalCellIndex,
                           /* TDD Config 0 Changes Start */
                           /* +- SPR 17777 */
                           prevUlSubFrameNum);
                     tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                  }
                /* required resources are not available, hence increment the 
                 * current ReTx count and start UL Timer*/
                /* +COVERITY 10718 */
            /* SPR 10803 [BUG 848] FIX START */
	 /* FORCED_ACK SPR#9391 Changes Start */
                tempULHarqInfo_p->currentRetransCount++;
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
	 /* FORCED_ACK SPR#9391 Changes End */
            /* SPR 10803 [BUG 848] FIX END */

                /* SPR 20929 FIX Start */
                if ((ueScheduledInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                        (INVALID_CCE_INDEX != ueScheduledInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
                {
                    mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                            dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                            internalCellIndex);
                }
                /* -COVERITY 10718 */
                /* + SPR 6864 */
                if (ULSCH_UCI_INVALID_INDEX !=
                            ueScheduledInfo_p->ulschUciInfoIndex)
                {
                      /* + SPR_14994 */
                    checkAndRestoreUCIPduType(ulUEContext_p, ueScheduledInfo_p,
                           ulSysFrameNum, ulschUciRachInfo_p ,ulSubFrameNum,
                           /*SPR 19504 fix start*/    
                           rbMapNum,
                           internalCellIndex);
                           /*SPR 19504 fix end*/    
                      /* - SPR_14994 */
                }
                /* - SPR 6864 */

            /* SPR 10803 [BUG 848] FIX START */
	 /* FORCED_ACK SPR#9391 Changes Start */
	        ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                resAllocFailure = TRUE;
	 /* FORCED_ACK SPR#9391 Changes End */
            /* SPR 10803 [BUG 848] FIX END */
                LOG_MAC_MSG(MAC_NACK_RES_ZERO_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        rbMapNum, 
                        tempULHarqInfo_p->nextReTransmissionType,
                        tempULHarqInfo_p->currentRetransCount,
                        0.0,0.0,__func__,"NACK_RES_ZERO_FAIL");

                LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                        tempUeIndex,
                        currentGlobalTick,
                        NON_ADAPTIVE_RE_TX,
                        tempHarqProcessId,
                        tempULHarqInfo_p->riLength,
                        tempULHarqInfo_p->riStart,
                        ueScheduledInfo_p->ulGrantRequestType,
                        tempULHarqInfo_p->deltabsrLoad,
                        FALSE,
                        tempULHarqInfo_p->mcsIndex,
                        0,
                        TPC_VALUE_1 
                        /* UL_MU_MIMO_CHG START */
                        , ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                        INVALID_UE_INDEX,
                        ulUEContext_p->userLocationType,
                        tempULHarqInfo_p->nDmrs,
                        ulUEContext_p->modulationScheme,
                        /* UL_MU_MIMO_CHG END */
                        /* CA Stats Changes Start */
                        internalCellIndex
                        /* CA Stats Changes End */
                        );                  
            }
            /*We use 0 for Retransmission */ 
            tempULHarqInfo_p->newDataIndicator = 0;            
            
            /* TDD Config 6 Changes Start */
            resetHarqId(prevUlSubFrameNum, ulUEContext_p, internalCellIndex,prevUlSysFrameNum);
            setHarqBusy(ulSubFrameNum, tempHarqProcessId, ulUEContext_p, internalCellIndex,ulSysFrameNum);
            /* TDD Config 6 Changes End */

            ulHarqTimerStart( subFrameNumber,
                    sysFrameNum,
                    tempUeIndex,
                    tempHarqProcessId,
                    /* + SPS_TDD_Changes */
                    DRA_SCHEDULING,
                    /* - SPS_TDD_Changes */
                    /*CA Changes start  */
                    internalCellIndex,
                    /*CA Changes end  */
                    /* TDD Config 0 Changes Start */
                    ulSubFrameNum);
                    /* TDD Config 0 Changes End */
            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            /* Check if this is HARQ pdu then fill HARQ size according HARQ
             * on PUSCH and set bit to indicate harq on PUSCH with DCI 0 */  
            if (MULTIPLEXING == ulUEContext_p->tddAckNackFeedbackMode && (resAllocFailure != TRUE))
            {
                SET_HARQ_SIZE_AND_BIT_FOR_UL_TX_WITH_DCI0_FOR_MUX(ulschUciRachInfo_p,
                        ulUEContext_p,internalCellIndex,ulSubFrameNum,
                        ueScheduledInfo_p->ulschUciInfoIndex,ueScheduledInfo_p);
            }
            /* SPR 19679: TDD HARQ Multiplexing Changes End */

            /* SPR 10803 [BUG 848] FIX START */
	/* FORCED_ACK SPR#9391 Changes Start */
           if(resAllocFailure != TRUE)
           {
	/* FORCED_ACK SPR#9391 Changes End */
            /* SPR 10803 [BUG 848] FIX END */
            (*numberOfULUEsToBeScheudled_p)--;

            /* Semi static changes Start*/
            if(ULSCH_UCI_INVALID_INDEX != ueScheduledInfo_p->ulschUciInfoIndex)
            {
                /* Fill UL Harq Info*/
                fillSemiStaticInfoForULSCH (ulschUciRachInfo_p,
                        ueScheduledInfo_p,
                        ulUEContextInfo_p,
                        tempULHarqInfo_p,
                        tempHarqProcessId,
			/* SPR 9173 changes start */
                        /* SPR 5620 START*/
                        ulSysFrameNum,
                        /* SPR 5620 END*/
                        ulSubFrameNum,0, internalCellIndex);
			/* SPR 9173 changes end */
                /* SPR 10136 fix Start*/
                ueScheduledInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
                /* SPR 10136 fix end*/
            }
            else
            {
                if ( MAX_ULSCH_UCI_SUPPORTED ==  numberOfInfoPresent)
                {
                    ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                            __func__);
                    break;
                }
                /*SPR 7086 Fix Start*/
                /* SPR 10195 Fix Start */
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS =
                    nSRSDetermineOnCollision(ulUEContext_p, tempHarqProcessId, ulSubFrameNum, internalCellIndex);
                /* SPR 10195 Fix End */
                /*SPR 7086 Fix End*/

                ulschUciRachInfo_p->
                    ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p =
                    tempULHarqInfo_p;
                ulschUciRachInfo_p->
                    ulschUciInfo[numberOfInfoPresent].ulHarqProcessId =
                    tempHarqProcessId;

                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                    pduType = MAC_SS_PDU_ULSCH;

                /* SPR 2307 Fix Start */
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = 
                    ulUEContext_p->crnti;
                /* SPR 2307 Fix End */
                /*TDD Config 0 Changes Start*/
                /*Populate ulschUciInfoIndex in ueScheduledInfo_p and
                 *ueScheduledInfo_p in ulschUciRachInfo_p.This info is
                 * retrieved in PUCCH for TDD_CONFIG0*/
                ueScheduledInfo_p->ulschUciInfoIndex = numberOfInfoPresent;
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ueScheduledInfo_p = ueScheduledInfo_p;
                /*TDD Config 0 Changes End*/

                LOG_UT(MAC_UCI_ULSCH_PDU_ASSIGN,LOGDEBUG,MAC_PUSCH,\
                        getCurrentTick(),\
                        tempUeIndex,
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType,\
                        ueScheduledInfo_p->ulschUciInfoIndex,\
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId,\
                        0,0.0,0.0,\
                        __func__,"ULSCH_PDU_ASSIGN");
                numberOfInfoPresent++;
            }
            /* Semi static changes End*/


            ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            ueScheduledInfo_p->flag = RECEIVED_NACK;
            /* SPR 10803 [BUG 848] FIX START */
 /* FORCED_ACK SPR#9391 Changes Start */
	   }
  /* FORCED_ACK SPR#9391 Changes End */
            /* SPR 10803 [BUG 848] FIX END */
        }
        /*SPR 19504 fix start*/    
	/*SPR 20145 fix start*/
        //cleanUpUeScheduledInfo(ueScheduledInfo_p);
	/*SPR 20145 fix end*/
        /*SPR 19504 fix end*/    
    }

    /*Update Number of numberOfInfoPresent in the global context */ 
    ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;

}
#endif

#ifdef TDD_CONFIG
/******************************************************************************
 * Function Name    : processULNackForAdaptiveReTxOfMsg3Tdd
 * Inputs           : tempUeIndex - The temp UE Index,
 *                    containerTick - The Tick of the container being used.
 *                    currentGlobalTick - The present Global Tick,
 *                    ulDelay - The delay of execution legs in terms of the number
 *                              number subFrames w.r.t. PHY & the current processing 
 *                              being done at MAC for intended tick at PHY,
 *                              It is zero in case of TDD.
 *                    rbMapNum,
 *                    numberOfULUEsToBeScheudled_p - Pointer to number of UL UE's to be
 *                                                   scheduled,
 *                    tempPUSCHResources_p,
 *                    timerExpiredTTI,
 *                    timerExpsubFrameNum,
 *                    ulHarqSysFrameNum,
 *                    numberOfInfoPresent_p,
 *                    regionAllocated,
 *                    internalCellIndex - current cell index for MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will read the Nack queue and update the HARQ process, 
 *                    and book the resources in advance for msg3. It will also create the 
 *                    HI PDUs. 
 ******************************************************************************/
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
        InternalCellIndex internalCellIndex,
        /*TDD Config 0 Changes Start*/ 
        UInt8  ulSubFrameNum)
        /*TDD Config 0 Changes End*/
{
    UeScheduledInfoForMsg3 *ueScheduledInfo_p = PNULL;
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; /* Invalid Value*/
    ULHarqInfo* tempULHarqInfo_p = PNULL;
    ULHarqInfo tempULHarqInfoBeforeRetxResourceAlloc = {0};
    HIPduArray* tempHIPdu_p  = PNULL;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
    ULSchUciRachPoolInfo* ulschUciRachInfo_p = PNULL;
    UInt16 tcRNTI = INVALID_UE_INDEX;
    /*SPR 7086 Fix Start*/
    UInt8                   ulStartRB = 0;
    UInt8                   ulEndRB = 0;
    /*SPR 7086 Fix End*/

    ueScheduledInfo_p = &tempCRNTICtx_p->ulUeScheduledInfoPointerArray;
    /*TDD Config 0 Changes Start*/
    ulschUciRachInfo_p  = ulSchUciRachContainer_gp[internalCellIndex] + (ulSubFrameNum % MAX_ULSCH_UCI_CONTAINER) ;
    /*TDD Config 0 Changes End*/
    tempHarqProcessId = ueScheduledInfo_p->ulHarqProcessId;
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick ;

    tempCRNTICtx_p->isPdcchAllocated = 0;
    tempULHarqInfo_p = &(tempCRNTICtx_p->ulHarq);
    tempULHarqInfoBeforeRetxResourceAlloc = tempCRNTICtx_p->ulHarq;
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];
    /*TDD Config 0 Changes Start*/
    UInt8 iPhich    = 0;
    UInt8 phichSubFrameNum = 0;
    UInt8 prevUlSubFrameNum = 0;
    UInt8 subFrameNum = currentGlobalTick % MAX_SUB_FRAME;
    UInt8 ulSubFrameOffset =  getUlOffset(subFrameNum,ulSubFrameNum);
    /* SPR 15909 fix start */
    tickType_t ulGlobalTick = currentGlobalTick + ulSubFrameOffset;
    /* SPR 15909 fix end */
    prevUlSubFrameNum = (ulGlobalTick - getprevUlSfOffsetFromCurrentUlSf(ulSubFrameNum,
                        /* CA TDD CHANGES START */
                        internalCellIndex
                        /* CA TDD CHANGES END */
                        )) % MAX_SUB_FRAME;
    /*TDD Config 0 Changes End*/
#ifdef MAC_AUT
    prevUlSubFrameNo = prevUlSubFrameNum;
    ulFrameOffset = ulSubFrameOffset;
#endif

    if (*tempPUSCHResources_p >= tempULHarqInfo_p->riLength &&
            (MAC_SUCCESS == allocatePUSCHResourcesForAdaptiveReTx(
                                tempULHarqInfo_p,
                                tempULHarqInfo_p->riLength,
                                tempULHarqInfo_p->strictRBAllocation,
                                resourceReserved,
                                PNULL,
                                rbMapNum,
                                TRUE,/* SS_S2 */
                                regionAllocated,
                                internalCellIndex,
                                /*TDD Config 0 Changes Start*/ 
                                ulSubFrameNum
                                )))
                                /*TDD Config 0 Changes End*/ 
    {
        addEntryTOHIPduArray(
                &tempULHarqInfoBeforeRetxResourceAlloc,
                HI_NACK,
                tempCRNTICtx_p->tcRNTI,
                tempHIPdu_p,
                /*CA Changes start  */
                internalCellIndex,
                /*CA Changes end  */
                /*TDD Config 0 Changes Start*/
                /* +- SPR 17777 */
                prevUlSubFrameNum);

        ulHarqTimerStartForMsg3(tempCRNTICtx_p->tcRNTI,
                tempHarqProcessId,
                timerExpsubFrameNum,
                timerExpiredTTI,
                ulHarqSysFrameNum,
                /*CA Changes start  */
                internalCellIndex
                /*CA Changes end  */);

        tempCRNTICtx_p->ulHarq.newDataIndicator = 0;
        /*TDD Config 0 Changes Start*/
        /* Block the phich resources for the given ristart and
         * ndmrs*/
        phichSubFrameNum = ulSubFrameNum + getHiPduSubframeNum(ulSubFrameNum,internalCellIndex);
        phichSubFrameNum = phichSubFrameNum % MAX_SUB_FRAME;
        iPhich = calculateIPhichValue(ulSubFrameNum,internalCellIndex);
        reserveHiAndUpdateHarqInfo(iPhich,
                                   tempULHarqInfo_p->riStart,
                                   tempULHarqInfo_p->nDmrs,
                                   phichSubFrameNum,
                                   tempULHarqInfo_p,
                                   internalCellIndex);
        /* TDD Config 0 Changes End */

        updateDciCCEInfoArrayForMsg3(
                tempCRNTICtx_p,
                ueScheduledInfo_p->ulCCEIndex,
                /* +- SPR 17777 */
                tempDciCCEInfo_p,
                resourceReserved,
                /* +- SPR 17777 */
                internalCellIndex,
                currentGlobalTick,
                ulDelay,
                /* +- SPR 17777 */
                /*TDD Config 0 Changes Start*/
                ulSubFrameNum);
                /*TDD Config 0 Changes End*/
        
        *tempPUSCHResources_p -= resourceReserved->allocatedLength;
        resourceReserved->allocatedLength = 0;
        
        LOG_MAC_MSG(MAC_NACK_RES_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                rbMapNum,ADAPTIVE_RE_TX,
                tempULHarqInfo_p->currentRetransCount,
                tempULHarqInfo_p->riLength,tempULHarqInfo_p->riStart,
                __func__,"NACK_RES_ULHARQ_PASS");
        /*We use 0 for Retransmission */
        tempCRNTICtx_p->ulHarq.newDataIndicator = 0;

        (*numberOfULUEsToBeScheudled_p)--;
        /* Semi static changes Start*/
        if(ULSCH_UCI_INVALID_INDEX != ueScheduledInfo_p->ulschUciInfoIndex)
        {
            /* Fill UL Harq Info*/
            ulschUciRachInfo_p->
                ulschUciInfo[ueScheduledInfo_p->ulschUciInfoIndex].
                ulHarqInfo_p = tempULHarqInfo_p;
            ulschUciRachInfo_p->
                ulschUciInfo[ueScheduledInfo_p->ulschUciInfoIndex].
                ulHarqProcessId = tempHarqProcessId;
            /* SPR 2307 Fix Start */
            ulschUciRachInfo_p->ulschUciInfo[ueScheduledInfo_p->\
                ulschUciInfoIndex].rnti = tempCRNTICtx_p->tcRNTI;
            /* SPR 2307 Fix End */
            ueScheduledInfo_p->ulschUciInfoIndex =
                ULSCH_UCI_INVALID_INDEX;
        }
        else
        {
            if ( MAX_ULSCH_UCI_SUPPORTED <=  *numberOfInfoPresent_p)
            {
                ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                        __func__);
            }
            /* + COVERITY 5.0 */
            else
            {
                /*SPR 7086 Fix Start*/
                /*Check the SRS Overlap*/
                if(LTE_TRUE == *(srsCellConfigMap_gp[internalCellIndex] + ulSubFrameNum))
                {
                    ulStartRB = tempULHarqInfo_p->riStart;
                    ulEndRB = (UInt8)(tempULHarqInfo_p->riStart + tempULHarqInfo_p->riLength);
                    if((ulEndRB < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                      cellParams_p->srsFreqDomainStartRB) 
                                 || (ulStartRB > cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                         cellParams_p->srsFreqDomainEndRB 
                                              ))
                    {
                        ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].nSRS =
                            SRS_ULSCH_NO_OVERLAP;
                    }
                    else
                    {
                        ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].nSRS =
                            SRS_ULSCH_OVERLAP;
                    }
                }
                else
                {
                    ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].nSRS =
                        SRS_ULSCH_NO_OVERLAP;
                }
                /*SPR 7086 Fix End*/
                /* - COVERITY 5.0 */
                /* Fill UL Harq Info*/
                ulschUciRachInfo_p->
                    ulschUciInfo[*numberOfInfoPresent_p].ulHarqInfo_p =
                    tempULHarqInfo_p;
                ulschUciRachInfo_p->
                    ulschUciInfo[*numberOfInfoPresent_p].ulHarqProcessId =
                    tempHarqProcessId;

                ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].
                    pduType = MAC_SS_PDU_ULSCH;

                ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].rnti = tempCRNTICtx_p->tcRNTI;
                
                (*numberOfInfoPresent_p)++;
            }
        }
        /* Semi static changes End*/
    }
    else
    {
        /*In case of Msg3 if Resources arenot available HI_ACK will go*/
        addEntryTOHIPduArray(
                &tempULHarqInfoBeforeRetxResourceAlloc,
                HI_ACK,
                tempCRNTICtx_p->tcRNTI,
                tempHIPdu_p,
                internalCellIndex,
                /*TDD Config 0 Changes Start*/
                /* +- SPR 17777 */
                prevUlSubFrameNum);
                /*TDD Config 0 Changes End*/

        if(ULSCH_UCI_INVALID_INDEX != ueScheduledInfo_p->ulschUciInfoIndex)
        {
            ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].
                pduType = MAC_SS_PDU_INVALID;
        }
	/* Coverity Fix 94874 Start */
        /*
        We now revert back the  cce allocated by PDCCH
        as no more PUSCH resources are avaliable
        */
        mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                dciPduInfo.aggregationLvl,
                (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                internalCellIndex);
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
        /* Coverity Fix 94874 End */
        LOG_MAC_MSG(MAC_NACK_RES_ZERO_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                rbMapNum,
                tempULHarqInfo_p->nextReTransmissionType,
                tempULHarqInfo_p->currentRetransCount,
                0.0,0.0,__func__,"NACK_RES_ZERO_FAIL");

        tcRNTI = tempCRNTICtx_p->tcRNTI;

        deleteTempCRNTICtx(tempCRNTICtx_p->tcRNTI,internalCellIndex);
        if(MAC_FAILURE == freeRNTI(tcRNTI,internalCellIndex))
        {
            LOG_UT(MAC_TCRNTI_FAIL_ID,
                    LOGWARNING,
                    MAC_UL_HARQ,
                    getCurrentTick(),
                    tcRNTI,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    __func__,"Adap_FREE_TCRNTI_FAIL");
        }

    }

    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
    ueScheduledInfo_p->flag = RECEIVED_NACK;

}
#endif


/* Cyclomatic Complexity changes - starts here */
#ifdef FDD_CONFIG
/******************************************************************************
 * Function Name : checkAndUpdatePowerCorrection  
 * Inputs        : ueMaxPowerFlag - Flag representing UE maximum power,
 *                 ueMinPowerFlag - Flag representing UE minimum power,
 *                 powerCorrectionAndTpc - Power correction and TPC,
 *                 ulUEContext_p - Pointer to UE UL context,
 *                 tempUeIndex
 * Outputs       : None
 * Returns       : None
 * Description   : This function will check and update the Power Correction 
 *                 for PUSCH. 
 ******************************************************************************/
static  void checkAndUpdatePowerCorrection (
        UInt32                        ueMaxPowerFlag, 
        UInt32                        ueMinPowerFlag, 
        TpcTriggeredToPowerCorrection powerCorrectionAndTpc, 
        ULUEContext                   *ulUEContext_p
#ifdef PERF_STATS
        ,UInt16                       tempUeIndex
#endif
        )
{
    ulUEContext_p->prevPowerCorrectionForPusch =
        ulUEContext_p->currPowerCorrectionForPusch;

    checkAndUpdateFiCorrection(
            ueMaxPowerFlag,
            ueMinPowerFlag,
            powerCorrectionAndTpc,
            ulUEContext_p);

#ifdef PERF_STATS
        if (ueMaxPowerFlag == FALSE)
        {
            /* Increment the Counter when  Transport Blocks on MAC level scheduled in uplink
             * where the UE was NOT considered to be power limited */
            gMacUePerfStats_p[tempUeIndex].lteUeMacSchedULPerfStats.\
                totalTbScheduledInUlOnNoPowerRestriction++;
        }
        else
        {
            /* Increment the Counter when  Transport Blocks on MAC level scheduled in uplink
             *  where the UE was considered to be power limited */
            gMacUePerfStats_p[tempUeIndex].lteUeMacSchedULPerfStats.\
                totalTbScheduledOnPowerRestriction++;
        }
#endif
    CHECK_AND_UPDATE_POWER_CORRECTION_FOR_PUSCH(
            ulUEContext_p->currPowerCorrectionForPusch);
}
#endif
/* Cyclomatic Complexity changes - ends here */

/*SPR_3305_FIX_START*/
/****************************************************************************************
 * Function Name    : processULNackForNonAdaptiveReTxOfMsg3
 * Inputs           : tempUeIndex - The temp UE Index,
 *                    containerTick - The Tick of the container being used.
 *                    currentGlobalTick - The present Global Tick,
 *                    ulDelay - The delay of execution legs in terms of the number
 *                              number subFrames w.r.t. PHY & the current processing 
 *                              being done at MAC for intended tick at PHY,
 *                    tempUEContext_p - Pointer to TempCRNTICtx,
 *                    rbMapNum,
 *                    numberOfULUEsToBeScheudled_p - Pointer to number of UL UE's to be
 *                                                   scheduled,
 *                    tempPUSCHResources_p,
 *                    timerExpiredTTI,
 *                    timerExpsubFrameNum,
 *                    ulHarqSysFrameNum,
 *                    hiIndex,
 *                    numberOfInfoPresent_p, 
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will read the Nack queue and update the HARQ process, 
 *                    and book the resources in advance. It will also create the HI PDUs. 
 ******************************************************************************************/
#ifdef FDD_CONFIG

 MacRetType processULNackForNonAdaptiveReTxOfMsg3(UInt16 tempUeIndex,
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
        /*CA Changes start */
        InternalCellIndex internalCellIndex)
        /*CA Changes end */
{
    PdcchULOutputInfo* tempPdcchULOutput_p = PNULL;
    HIPduArray* tempHIPdu_p  = PNULL;
    AllocatedResInfo resourceReserved = {0};
    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p = PNULL;
    ULHarqInfo* tempULHarqInfo_p = PNULL;
    ULSchUciRachPoolInfo* ulschUciRachInfo_p = PNULL;
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; /* Invalid Value*/
    UInt16 tcRNTI = INVALID_UE_INDEX;
    /* ICIC changes start */
    RbMapNode *rbMapNode_p = PNULL;
    /*CA Changes start  */
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    /* ICIC changes end  */
    /*SPR 7086 Fix Start*/
    UInt8                   ulStartRB = 0;
    UInt8                   ulEndRB = 0;
    /*SPR 7086 Fix End*/

    /*CA Changes start  */
    tempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    tempULHarqInfo_p = &(tempUEContext_p->ulHarq);
    tempDirectIndexingUEInfo_p = \
                                 &(tempPdcchULOutput_p->directIndexingTempUEInfo[tempUeIndex]); 
    tempHarqProcessId = tempDirectIndexingUEInfo_p->harqIndex;
    /*CA Changes start  */
    ulschUciRachInfo_p = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */

    /*If CCE are not avialable for Adaptive Retx*/
    if ((NON_ADAPTIVE_NO_CCE_FOR_MSG3 == tempDirectIndexingUEInfo_p->flag)||
            /* SPR 5798 Fix Start */
        (MAX_UE_SCHEDULED_NO_RETX_NO_GRANT_FOR_MSG3 == tempDirectIndexingUEInfo_p->flag)||
        /* SPR 5798 Fix End */
            (*tempPUSCHResources_p < tempULHarqInfo_p->riLength))
    {
        addEntryTOHIPduArray(
                tempULHarqInfo_p,
                HI_ACK,
                tempUEContext_p->tcRNTI,
                PNULL,
                tempULHarqInfo_p->sfUsed,
                tempHIPdu_p,
                internalCellIndex);
        /* SPR 16605 Fix Start */
        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
            ~(ONE << tempULHarqInfo_p->nPhichSeq);
        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
        tempULHarqInfo_p->sfUsed = hiIndex;
        /* SPR 16605 Fix End */

        if(ULSCH_UCI_INVALID_INDEX != tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
        {
            ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].
                pduType = MAC_SS_PDU_INVALID;
        }
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
        LOG_MAC_MSG(MAC_NACK_RES_ZERO_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                DEFAULT_INT_VALUE,tempUeIndex,tempHarqProcessId, \
                rbMapNum, 
                tempULHarqInfo_p->nextReTransmissionType,
                tempULHarqInfo_p->currentRetransCount,
                0.0,0.0,__func__,"NACK_RES_ZERO_FAIL");

        tcRNTI = tempUEContext_p->tcRNTI;

        /*CA Changes start  */
        deleteTempCRNTICtx(tempUEContext_p->tcRNTI, internalCellIndex);
        /*CA Changes end  */

        if(MAC_FAILURE == freeRNTI(tcRNTI, internalCellIndex))
        {
            LOG_UT(MAC_TCRNTI_FAIL_ID,
                    LOGWARNING,
                    MAC_UL_HARQ,
                    getCurrentTick(),
                    tcRNTI,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    __func__,"NonAdap_FREE_TCRNTI_FAIL");

        }
        tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
        tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK;
        return MAC_SUCCESS;
    }

    /*
       We now create the HI PDU corresponding to the Nack received
       */
    addEntryTOHIPduArray(
            tempULHarqInfo_p,
            HI_NACK,
            tempUEContext_p->tcRNTI,
            PNULL,
            tempULHarqInfo_p->sfUsed,
            tempHIPdu_p,
            internalCellIndex);

    /*
       We at first check whether the UE has completed it's NACK re
       transmission limit, if yes we will free the HARQ Process and
       continue to the next UE.
       */
    if (tempDirectIndexingUEInfo_p->flag == RECEIVED_NACK_RE_TRANS_COMPELETE)
    {
        LOG_MAC_MSG(MAC_NACK_RETX_COMP_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                DEFAULT_INT_VALUE,tempUeIndex,tempHarqProcessId, \
                0,0,0,0.0,0.0,__func__,"NACK_RETX_COMP_FREE");
/*SPR 18970 fix start*/
        /* SPR 21869 Fix Start */
        /* SPR 16605 Fix Start */
        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
            ~(ONE << tempULHarqInfo_p->nPhichSeq);
        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
        tempULHarqInfo_p->sfUsed = hiIndex;
        /* SPR 16605 Fix End */
        /* SPR 21869 Fix End */
/*SPR 18970 fix end*/

        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
        tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
        tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

        tcRNTI = tempUEContext_p->tcRNTI;
        
        /*CA Changes start  */
        deleteTempCRNTICtx(tempUEContext_p->tcRNTI, internalCellIndex);
        /*CA Changes end  */
        
        if(MAC_FAILURE == freeRNTI(tcRNTI, internalCellIndex))
        {
            LOG_UT(MAC_TCRNTI_FAIL_ID,
                    LOGWARNING,
                    MAC_UL_HARQ,
                    getCurrentTick(),
                    tcRNTI,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    __func__,"Max_ReTrans_FREE_TCRNTI_FAIL");
        }
        return MAC_FAILURE;  
    }
    /* CR changes start */
    /*CA Changes start  */
    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
        ~(ONE << tempULHarqInfo_p->nPhichSeq);
    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
    /*CA Changes end  */
    tempULHarqInfo_p->sfUsed = hiIndex;
    /* CR changes end */

    /*We use 0 for Retransmission */ 
    tempUEContext_p->ulHarq.newDataIndicator = 0;
    /* + 5406 */
    if(*tempPUSCHResources_p >= tempULHarqInfo_p->riLength)
    {
        /* - 5406 */
        *tempPUSCHResources_p -= reserveAlreadyAvailableResourcesForRetx( \
                tempULHarqInfo_p->riStart, \
                tempULHarqInfo_p->riLength,\
                &rbMapNode_p->availableResourceInfo[CC_CE_REGION],\
                rbMapNode_p->rbMap); 
        LOG_MAC_MSG(MAC_NACK_RES_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                DEFAULT_INT_VALUE,tempUeIndex,tempHarqProcessId, \
                rbMapNum,NON_ADAPTIVE_RE_TX,
                tempULHarqInfo_p->currentRetransCount,
                tempULHarqInfo_p->riLength,tempULHarqInfo_p->riStart,
                __func__,"NACK_RES_ULHARQ_PASS");

        /* Required resources are not available, hence increase the current
         * ReTx count and start the UL Timer */
        updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
        /* + 5406 */

        /* CC resouces are updated always irrespective of user location,
         *                  * we check here whether the RBs lied in CE region and update the
         *                                   * resouce info if necessary*/
        /*SPR 21181 Start*/
        /*CA Changes start  */
        updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                internalCellIndex);
        /*CA Changes end  */
        /*Code removed, as we are decrementing freeRbCounters in
         * updateCEAvailRes as well*/
        /*SPR 21181 End*/
    }
    else
    {
        *tempPUSCHResources_p = 0;
    }
    /* - 5406 */

#ifdef ULHARQ_TIMER_PROC
    ulHarqTimerStartForMsg3(tempUEContext_p->tcRNTI,tempHarqProcessId,
            timerExpsubFrameNum,timerExpiredTTI,ulHarqSysFrameNum,
            /*CA Changes start  */
            internalCellIndex);
            /*CA Changes end  */
#endif 
    (*numberOfULUEsToBeScheudled_p)--;

    /* Semi static changes Start*/
    if(ULSCH_UCI_INVALID_INDEX != tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
    {
        /* Fill UL Harq Info*/
        ulschUciRachInfo_p->
            ulschUciInfo[tempDirectIndexingUEInfo_p->ulschUciInfoIndex].
            ulHarqInfo_p = tempULHarqInfo_p;
        ulschUciRachInfo_p->
            ulschUciInfo[tempDirectIndexingUEInfo_p->ulschUciInfoIndex].
            ulHarqProcessId = tempHarqProcessId;
        /* SPR 2307 Fix Start */
        ulschUciRachInfo_p->ulschUciInfo[tempDirectIndexingUEInfo_p->\
            ulschUciInfoIndex].rnti = tempUEContext_p->tcRNTI ;
        /* SPR 2307 Fix End */
        tempDirectIndexingUEInfo_p->ulschUciInfoIndex = 
            ULSCH_UCI_INVALID_INDEX;
    }
    else
    {
        if ( MAX_ULSCH_UCI_SUPPORTED <=  *numberOfInfoPresent_p)
        {
            ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                    __func__);
        }
        /* + COVERITY 5.0 */
        else
        {
            /*SPR 7086 Fix Start*/
            /*Check the SRS Overlap*/
            /* SPR 10682 changes start */
            if(LTE_TRUE == *(srsCellConfigMap_gp[internalCellIndex] + timerExpsubFrameNum))
            {
            /* SPR 10682 changes end */
                ulStartRB = tempULHarqInfo_p->riStart;
                ulEndRB = (UInt8)(tempULHarqInfo_p->riStart + tempULHarqInfo_p->riLength);
                if((ulEndRB < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                              cellParams_p->srsFreqDomainStartRB) || \
                   (ulStartRB > cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->srsFreqDomainEndRB))
                {
                    ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].nSRS = 
                        SRS_ULSCH_NO_OVERLAP;
                }
                else
                {
                    ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].nSRS = 
                        SRS_ULSCH_OVERLAP;
                }
            }
            else
            {
                ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].nSRS = 
                    SRS_ULSCH_NO_OVERLAP;
            }
            /*SPR 7086 Fix End*/
            /* - COVERITY 5.0 */
            /* Fill UL Harq Info*/
            ulschUciRachInfo_p->
                ulschUciInfo[*numberOfInfoPresent_p].ulHarqInfo_p = 
                tempULHarqInfo_p;
            ulschUciRachInfo_p->
                ulschUciInfo[*numberOfInfoPresent_p].ulHarqProcessId =
                tempHarqProcessId;

            ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].
                pduType = MAC_SS_PDU_ULSCH;
            ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].rnti = tempUEContext_p->tcRNTI;

            (*numberOfInfoPresent_p)++;
        }
    }
    /* Semi static changes End*/

    tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
    tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK;
    return MAC_SUCCESS;
}

/******************************************************************************
 * Function Name    : processULNackForAdaptiveReTxOfMsg3
 * Inputs           : tempUeIndex - The temp UE Index,
 *                    containerTick - The Tick of the container being used.
 *                    currentGlobalTick - The present Global Tick,
 *                    ulDelay - The delay of execution legs in terms of the number
 *                              number subFrames w.r.t. PHY & the current processing 
 *                              being done at MAC for intended tick at PHY,
 *                    tempUEContext_p - Pointer to TempCRNTICtx,
 *                    resourceReserved,
 *                    rbMapNum,
 *                    numberOfULUEsToBeScheudled_p - Pointer to number of UL UE's to be
 *                                                   scheduled,
 *                    tempPUSCHResources_p,
 *                    timerExpiredTTI,
 *                    timerExpsubFrameNum,
 *                    ulHarqSysFrameNum,
 *                    hiIndex,
 *                    numberOfInfoPresent_p,
 *                    regionAllocated,
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will read the Nack queue and update the HARQ process, 
 *                    and book the resources in advance. It will also create the HI PDUs. 
 ******************************************************************************/
 void processULNackForAdaptiveReTxOfMsg3(UInt16 tempUeIndex,
        UInt8 containerTick,
        TempCRNTICtx *tempUEContext_p,
        AllocatedResInfo* resourceReserved,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt8 ulDelay,
        UInt32 rbMapNum,
        UInt8 *numberOfULUEsToBeScheudled_p,
        UInt32 *tempPUSCHResources_p, 
        /* SPR 15909 fix start */
        tickType_t timerExpiredTTI,
        /* SPR 15909 fix end */
        UInt32 timerExpsubFrameNum,
        UInt32 ulHarqSysFrameNum,
        UInt32 hiIndex,
        UInt16 *numberOfInfoPresent_p,
        UInt8 regionAllocated,
        /*CA Changes start */
        InternalCellIndex internalCellIndex)
        /*CA Changes end */
{
    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p = PNULL;
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; /* Invalid Value*/
    ULHarqInfo* tempULHarqInfo_p = PNULL;
    ULHarqInfo tempULHarqInfoBeforeRetxResourceAlloc = {0};
    HIPduArray* tempHIPdu_p  = PNULL;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
    PdcchULOutputInfo* tempPdcchULOutput_p = PNULL;
    ULSchUciRachPoolInfo* ulschUciRachInfo_p = PNULL;
    UInt16 tcRNTI = INVALID_UE_INDEX;
    /*SPR 7086 Fix Start*/
    UInt8                   ulStartRB = 0;
    UInt8                   ulEndRB = 0;
    /*SPR 7086 Fix End*/

    /*CA Changes start  */
    tempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;
    ulschUciRachInfo_p  = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    tempDirectIndexingUEInfo_p = \
                                 &(tempPdcchULOutput_p->directIndexingTempUEInfo[tempUeIndex]); 
    tempHarqProcessId = tempDirectIndexingUEInfo_p->harqIndex;
    /*CA Changes start  */
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */

    tempUEContext_p->isPdcchAllocated = 0;
    tempULHarqInfo_p = &(tempUEContext_p->ulHarq);
    tempULHarqInfoBeforeRetxResourceAlloc = tempUEContext_p->ulHarq;
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

//Coverity 36224 Fixed

    if (*tempPUSCHResources_p >= tempULHarqInfo_p->riLength &&
#ifdef FDD_CONFIG
        (!tempULHarqInfo_p->isTTIBundled) && 
#endif
            (MAC_SUCCESS == allocatePUSCHResourcesForAdaptiveReTx(
                                                                  tempULHarqInfo_p,
                                                                  tempULHarqInfo_p->riLength,
                                                                  tempULHarqInfo_p->strictRBAllocation,
                                                                  resourceReserved,
                                                                  PNULL,
                                                                  rbMapNum,
                                                                  TRUE,/* SS_S2 */
                                                                  regionAllocated
#ifdef FDD_CONFIG
                                                                  ,ulDelay
                                                                  ,currentGlobalTick
                                                                  ,PNULL       
#endif
                                                                  ,internalCellIndex
                                                                  )))
    {
        addEntryTOHIPduArray(
                &tempULHarqInfoBeforeRetxResourceAlloc,
                HI_NACK,
                tempUEContext_p->tcRNTI,
                PNULL,
                tempULHarqInfoBeforeRetxResourceAlloc.sfUsed,
                tempHIPdu_p,
                internalCellIndex);

        /*CA Changes start  */
        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
            ~(ONE << tempULHarqInfo_p->nPhichSeq);
        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
        /*CA Changes end  */
        tempULHarqInfo_p->sfUsed = hiIndex;

#ifdef ULHARQ_TIMER_PROC
        ulHarqTimerStartForMsg3(tempUEContext_p->tcRNTI,tempHarqProcessId,
                timerExpsubFrameNum,timerExpiredTTI,ulHarqSysFrameNum,
                /*CA Changes start  */
                internalCellIndex);
                /*CA Changes end  */
#endif
        tempUEContext_p->ulHarq.newDataIndicator = 0;
        updateDciCCEInfoArrayForMsg3(
                tempUEContext_p,
                tempDirectIndexingUEInfo_p->ulCCEIndex,
                /* +- SPR 17777 */
                tempDciCCEInfo_p,
                resourceReserved,
                tempHarqProcessId,
                /* +- SPR 17777 */
                /* +- SPR 17777 */
                hiIndex,
                internalCellIndex); /* SS_S2 */
        *tempPUSCHResources_p -= resourceReserved->allocatedLength;
        resourceReserved->allocatedLength = 0;
        LOG_MAC_MSG(MAC_NACK_RES_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                rbMapNum,ADAPTIVE_RE_TX,
                tempULHarqInfo_p->currentRetransCount,
                tempULHarqInfo_p->riLength,tempULHarqInfo_p->riStart,
                __func__,"NACK_RES_ULHARQ_PASS");
        /*We use 0 for Retransmission */ 
        tempUEContext_p->ulHarq.newDataIndicator = 0;

        (*numberOfULUEsToBeScheudled_p)--;
        /* Semi static changes Start*/
        if(ULSCH_UCI_INVALID_INDEX != tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
        {
            /* Fill UL Harq Info*/
            ulschUciRachInfo_p->
                ulschUciInfo[tempDirectIndexingUEInfo_p->ulschUciInfoIndex].
                ulHarqInfo_p = tempULHarqInfo_p;
            ulschUciRachInfo_p->
                ulschUciInfo[tempDirectIndexingUEInfo_p->ulschUciInfoIndex].
                ulHarqProcessId = tempHarqProcessId;
            /* SPR 2307 Fix Start */ 
            ulschUciRachInfo_p->ulschUciInfo[tempDirectIndexingUEInfo_p->\
                ulschUciInfoIndex].rnti = tempUEContext_p->tcRNTI;
            /* SPR 2307 Fix End */ 
            tempDirectIndexingUEInfo_p->ulschUciInfoIndex = 
                ULSCH_UCI_INVALID_INDEX; 
        }
        else
        {
            if ( MAX_ULSCH_UCI_SUPPORTED <=  *numberOfInfoPresent_p)
            {
                ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                        __func__);
            }
            /* + COVERITY 5.0 */
            else
            {
                /*SPR 7086 Fix Start*/
                /*Check the SRS Overlap*/
                /* SPR 10682 changes start */
                if(LTE_TRUE == *(srsCellConfigMap_gp[internalCellIndex] + timerExpsubFrameNum))
                {
                /* SPR 10682 changes end */
                    ulStartRB = tempULHarqInfo_p->riStart;
                    ulEndRB = (UInt8)(tempULHarqInfo_p->riStart + tempULHarqInfo_p->riLength);
                    if((ulEndRB < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                  cellParams_p->srsFreqDomainStartRB) || \
                       (ulStartRB > cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                    cellParams_p->srsFreqDomainEndRB))
                    {
                        ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].nSRS = 
                            SRS_ULSCH_NO_OVERLAP;
                    }
                    else
                    {
                        ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].nSRS = 
                            SRS_ULSCH_OVERLAP;
                    }
                }
                else
                {
                    ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].nSRS = 
                        SRS_ULSCH_NO_OVERLAP;
                }
                /*SPR 7086 Fix End*/
                /* - COVERITY 5.0 */
                /* Fill UL Harq Info*/
                ulschUciRachInfo_p->
                    ulschUciInfo[*numberOfInfoPresent_p].ulHarqInfo_p = 
                    tempULHarqInfo_p;
                ulschUciRachInfo_p->
                    ulschUciInfo[*numberOfInfoPresent_p].ulHarqProcessId =
                    tempHarqProcessId;

                ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].
                    pduType = MAC_SS_PDU_ULSCH;

                ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].rnti = tempUEContext_p->tcRNTI;
                (*numberOfInfoPresent_p)++;
            }
        }
        /* Semi static changes End*/

    }
    else
    {
        /*In case of Msg3 if Resources arenot available HI_ACK will go*/
        addEntryTOHIPduArray(
                &tempULHarqInfoBeforeRetxResourceAlloc,
                HI_ACK,
                tempUEContext_p->tcRNTI,
                PNULL,
                tempULHarqInfoBeforeRetxResourceAlloc.sfUsed,
                tempHIPdu_p,
                internalCellIndex);
        /* SPR 16605 Fix Start */
        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
            ~(ONE << tempULHarqInfo_p->nPhichSeq);
        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
        tempULHarqInfo_p->sfUsed = hiIndex;
        /* SPR 16605 Fix End */

        if(ULSCH_UCI_INVALID_INDEX != tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
        {
            ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].
                pduType = MAC_SS_PDU_INVALID;
        }
        /*
           We now revert back the  cce allocated by PDCCH 
           as no more PUSCH resources are avaliable
         */
        /* SPR 14783 fix start */                             
                /* SPR 20929 FIX Start */
        if ((tempDirectIndexingUEInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                (INVALID_CCE_INDEX != tempDirectIndexingUEInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
        {                                                             

            mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].\
                    dciPduInfo.aggregationLvl,
                    (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex]),
                    internalCellIndex); 
        }
        /* SPR 14783 fix end */ 
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */

        LOG_MAC_MSG(MAC_NACK_RES_ZERO_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                rbMapNum, 
                tempULHarqInfo_p->nextReTransmissionType,
                tempULHarqInfo_p->currentRetransCount,
                0.0,0.0,__func__,"NACK_RES_ZERO_FAIL");

        tcRNTI = tempUEContext_p->tcRNTI;

        /*CA Changes start  */
        deleteTempCRNTICtx(tempUEContext_p->tcRNTI, internalCellIndex);
        /*CA Changes end  */

        if(MAC_FAILURE == freeRNTI(tcRNTI, internalCellIndex))
        {
            LOG_UT(MAC_TCRNTI_FAIL_ID,
                    LOGWARNING,
                    MAC_UL_HARQ,
                    getCurrentTick(),
                    tcRNTI,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    __func__,"Adap_FREE_TCRNTI_FAIL");
        }

    }

    tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
    tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK;

}
#endif
#ifdef TDD_CONFIG
/******************************************************************************
 * Function Name    : processULNackForNonAdaptiveReTxOfMsg3Tdd
 * Inputs           : tempUeIndex - The temp UE Index,
 *                    containerTick - The Tick of the container being used.
 *                    currentGlobalTick - The present Global Tick,
 *                    ulDelay - The delay of execution legs in terms of the number
 *                              number subFrames w.r.t. PHY & the current processing 
 *                              being done at MAC for intended tick at PHY,
 *                    tempUEContext_p - Pointer to TempCRNTICtx,
 *                    rbMapNum,
 *                    numberOfULUEsToBeScheudled_p - Pointer to number of UL UE's to be
 *                                                   scheduled,
 *                    tempPUSCHResources_p,
 *                    timerExpiredTTI,
 *                    timerExpsubFrameNum,
 *                    ulHarqSysFrameNum,
 *                    numberOfInfoPresent_p,
 *                    internalCellIndex
 * Outputs          : None
 * Returns          : MAC_SUCCESS/MAC_FAILURE
 * Description      : This function will read the Nack queue for nsg3 and update the HARQ 
 *                    process, and book the resources in advance. It will also create the HI PDUs. 
 ******************************************************************************/
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
        UInt8  ulSubFrameNum)
        /*TDD Config 0 Changes End*/
{
    HIPduArray* tempHIPdu_p  = PNULL;
    AllocatedResInfo resourceReserved = {0};
    UeScheduledInfoForMsg3 *ueScheduledInfo_p = PNULL;
    ULHarqInfo* tempULHarqInfo_p = PNULL;
    ULSchUciRachPoolInfo* ulschUciRachInfo_p = PNULL;
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; /* Invalid Value*/
    UInt16 tcRNTI = INVALID_UE_INDEX;
    /* ICIC changes start */
    RbMapNode *rbMapNode_p = PNULL;
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /* ICIC changes end  */
    /*SPR 7086 Fix Start*/
    UInt8                   ulStartRB = 0;
    UInt8                   ulEndRB = 0;
    /*SPR 7086 Fix End*/
    UInt8                   phichSubFrameNum = 0;
    UInt8                   prevUlSubFrameNum = 0;
  
    ueScheduledInfo_p = &tempCRNTICtx_p->ulUeScheduledInfoPointerArray;                         
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick;
    tempULHarqInfo_p = &(tempCRNTICtx_p->ulHarq);
    tempHarqProcessId = ueScheduledInfo_p->ulHarqProcessId;
    /*TDD Config 0 Changes Start*/
    ulschUciRachInfo_p = ulSchUciRachContainer_gp[internalCellIndex] + (ulSubFrameNum % MAX_ULSCH_UCI_CONTAINER);
    phichSubFrameNum = ulSubFrameNum + getHiPduSubframeNum(ulSubFrameNum,internalCellIndex);
    phichSubFrameNum = phichSubFrameNum % MAX_SUB_FRAME;
    UInt8 subFrameNum = currentGlobalTick % MAX_SUB_FRAME;
    UInt8 ulSubFrameOffset =  getUlOffset(subFrameNum,ulSubFrameNum);
    /* SPR 15909 fix start */
    tickType_t ulGlobalTick = currentGlobalTick + ulSubFrameOffset;
    /* SPR 15909 fix end */
    prevUlSubFrameNum = (ulGlobalTick - getprevUlSfOffsetFromCurrentUlSf(ulSubFrameNum,
                        /* CA TDD CHANGES START */
                        internalCellIndex
                        /* CA TDD CHANGES END */
                        )) % MAX_SUB_FRAME;
    /*TDD Config 0 Changes End*/
#ifdef MAC_AUT
    prevUlSubFrameNo = prevUlSubFrameNum;
    ulFrameOffset = ulSubFrameOffset;
#endif

    /*If CCE are not avialable for Adaptive Retx*/
    if ((NON_ADAPTIVE_NO_CCE_FOR_MSG3 == ueScheduledInfo_p->flag)||
            /* SPR 5798 Fix Start */
        (MAX_UE_SCHEDULED_NO_RETX_NO_GRANT_FOR_MSG3 == ueScheduledInfo_p->flag)||
        /* SPR 5798 Fix End */
            (*tempPUSCHResources_p < tempULHarqInfo_p->riLength))
    {
        addEntryTOHIPduArray(
                tempULHarqInfo_p,
                HI_ACK,
                tempCRNTICtx_p->tcRNTI,
                tempHIPdu_p,
                internalCellIndex,
                /*TDD Config 0 Changes Start*/ 
                /* +- SPR 17777 */
                prevUlSubFrameNum);
                /*TDD Config 0 Changes End*/ 


        if(ULSCH_UCI_INVALID_INDEX != ueScheduledInfo_p->ulschUciInfoIndex)
        {
            ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].
                pduType = MAC_SS_PDU_INVALID;
        }
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
        LOG_MAC_MSG(MAC_NACK_RES_ZERO_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                DEFAULT_INT_VALUE,tempUeIndex,tempHarqProcessId, \
                rbMapNum, 
                tempULHarqInfo_p->nextReTransmissionType,
                tempULHarqInfo_p->currentRetransCount,
                0.0,0.0,__func__,"NACK_RES_ZERO_FAIL");

        tcRNTI = tempCRNTICtx_p->tcRNTI;

        deleteTempCRNTICtx(tempCRNTICtx_p->tcRNTI,internalCellIndex);

        if(MAC_FAILURE == freeRNTI(tcRNTI,internalCellIndex))
        {
            LOG_UT(MAC_TCRNTI_FAIL_ID,
                    LOGWARNING,
                    MAC_UL_HARQ,
                    getCurrentTick(),
                    tcRNTI,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    __func__,"NonAdap_FREE_TCRNTI_FAIL");

        }
        ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
        ueScheduledInfo_p->flag = RECEIVED_NACK;
        return MAC_SUCCESS;
    }

    /*
       We now create the HI PDU corresponding to the Nack received
       */
    addEntryTOHIPduArray(
            tempULHarqInfo_p,
            HI_NACK,
            tempCRNTICtx_p->tcRNTI,
            tempHIPdu_p,
            internalCellIndex,
            /*TDD Config 0 Changes Start*/ 
            /* +- SPR 17777 */
            prevUlSubFrameNum);
            /*TDD Config 0 Changes End*/ 

    /*
       We at first check whether the UE has completed it's NACK re
       transmission limit, if yes we will free the HARQ Process and
       continue to the next UE.
       */
    if (ueScheduledInfo_p->flag == RECEIVED_NACK_RE_TRANS_COMPELETE)
    {
        LOG_MAC_MSG(MAC_NACK_RETX_COMP_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                DEFAULT_INT_VALUE,tempUeIndex,tempHarqProcessId, \
                0,0,0,0.0,0.0,__func__,"NACK_RETX_COMP_FREE");

        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(tempULHarqInfo_p,INVALID_UE_INDEX,internalCellIndex);
#else
        freeULHarqProcess(tempULHarqInfo_p);
#endif
        /* SPR 16916 FIX END */
        ueScheduledInfo_p->flag = UNSET_FLAG;
        ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

        tcRNTI = tempCRNTICtx_p->tcRNTI;
        
        deleteTempCRNTICtx(tempCRNTICtx_p->tcRNTI,internalCellIndex);
        
        if(MAC_FAILURE == freeRNTI(tcRNTI,internalCellIndex))
        {
            LOG_UT(MAC_TCRNTI_FAIL_ID,
                    LOGWARNING,
                    MAC_UL_HARQ,
                    getCurrentTick(),
                    tcRNTI,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    __func__,"Max_ReTrans_FREE_TCRNTI_FAIL");
        }
        return MAC_FAILURE;  
    }

    /*TDD Config 0 Changes Start*/ 
    /* CR changes start */
    phichSequenceNoPerPhichGroup_g[internalCellIndex][phichSubFrameNum][tempULHarqInfo_p->nPhichGrp] &=
        ~(ONE << tempULHarqInfo_p->nPhichSeq);
    tempULHarqInfo_p->sfUsed = phichSubFrameNum;
    /* CR changes end */
    /*TDD Config 0 Changes End*/

    /*We use 0 for Retransmission */ 
    tempCRNTICtx_p->ulHarq.newDataIndicator = 0;
    /* + 5406 */
    if(*tempPUSCHResources_p >= tempULHarqInfo_p->riLength)
    {
        /* - 5406 */
        *tempPUSCHResources_p -= reserveAlreadyAvailableResourcesForRetx( \
                tempULHarqInfo_p->riStart, \
                tempULHarqInfo_p->riLength,\
                &rbMapNode_p->availableResourceInfo[CC_CE_REGION],\
                rbMapNode_p->rbMap); 
        LOG_MAC_MSG(MAC_NACK_RES_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                DEFAULT_INT_VALUE,tempUeIndex,tempHarqProcessId, \
                rbMapNum,NON_ADAPTIVE_RE_TX,
                tempULHarqInfo_p->currentRetransCount,
                tempULHarqInfo_p->riLength,tempULHarqInfo_p->riStart,
                __func__,"NACK_RES_ULHARQ_PASS");

        /* Required resources are not available, hence increase the current
         * ReTx count and start the UL Timer */
        updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
        /* + 5406 */
        /* CC resouces are updated always irrespective of user location,
         *                  * we check here whether the RBs lied in CE region and update the
         *                                   * resouce info if necessary*/
        /*SPR 21181 Start*/
        updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                internalCellIndex);
        /*Code removed, as we are decrementing freeRbCounters in
         * updateCEAvailRes as well*/
        /*SPR 21181 End*/
    }
    else
    {
        *tempPUSCHResources_p = 0;
    }
    /* - 5406 */

    ulHarqTimerStartForMsg3(tempCRNTICtx_p->tcRNTI,tempHarqProcessId,
                            timerExpsubFrameNum,timerExpiredTTI,
                            ulHarqSysFrameNum,
                            internalCellIndex);
    (*numberOfULUEsToBeScheudled_p)--;

    /* Semi static changes Start*/
    if(ULSCH_UCI_INVALID_INDEX != ueScheduledInfo_p->ulschUciInfoIndex)
    {
        /* Fill UL Harq Info*/
        ulschUciRachInfo_p->
            ulschUciInfo[ueScheduledInfo_p->ulschUciInfoIndex].
            ulHarqInfo_p = tempULHarqInfo_p;
        ulschUciRachInfo_p->
            ulschUciInfo[ueScheduledInfo_p->ulschUciInfoIndex].
            ulHarqProcessId = tempHarqProcessId;
        /* SPR 2307 Fix Start */
        ulschUciRachInfo_p->ulschUciInfo[ueScheduledInfo_p->\
            ulschUciInfoIndex].rnti = tempCRNTICtx_p->tcRNTI ;
        /* SPR 2307 Fix End */
        ueScheduledInfo_p->ulschUciInfoIndex = 
            ULSCH_UCI_INVALID_INDEX;
    }
    else
    {
        if ( MAX_ULSCH_UCI_SUPPORTED <=  *numberOfInfoPresent_p)
        {
            ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                    __func__);
        }
        /* + COVERITY 5.0 */
        else
        {
            /*SPR 7086 Fix Start*/
            /*Check the SRS Overlap*/
            if(LTE_TRUE == *(srsCellConfigMap_gp[internalCellIndex] + ulSubFrameNum))
            {
                ulStartRB = tempULHarqInfo_p->riStart;
                ulEndRB = (UInt8)(tempULHarqInfo_p->riStart + tempULHarqInfo_p->riLength);
                if((ulEndRB < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                            cellParams_p->srsFreqDomainStartRB) || \
                        (ulStartRB > cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                         cellParams_p->srsFreqDomainEndRB))
                {
                    ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].nSRS = 
                        SRS_ULSCH_NO_OVERLAP;
                }
                else
                {
                    ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].nSRS = 
                        SRS_ULSCH_OVERLAP;
                }
            }
            else
            {
                ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].nSRS = 
                    SRS_ULSCH_NO_OVERLAP;
            }
            /*SPR 7086 Fix End*/
            /* - COVERITY 5.0 */
            /* Fill UL Harq Info*/
            ulschUciRachInfo_p->
                ulschUciInfo[*numberOfInfoPresent_p].ulHarqInfo_p = 
                tempULHarqInfo_p;
            ulschUciRachInfo_p->
                ulschUciInfo[*numberOfInfoPresent_p].ulHarqProcessId =
                tempHarqProcessId;

            ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].
                pduType = MAC_SS_PDU_ULSCH;
            ulschUciRachInfo_p->ulschUciInfo[*numberOfInfoPresent_p].rnti = tempCRNTICtx_p->tcRNTI;
            
            (*numberOfInfoPresent_p)++;
        }
    }
    /* Semi static changes End*/

    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
    ueScheduledInfo_p->flag = RECEIVED_NACK;
    return MAC_SUCCESS;
}
#endif


/*SPR_3305_FIX_END*/
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name    : addEntryToPhichGrpAndSeqArrays
 * Inputs           : internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function will populate a global array for PHICH group
 *                    from PRB nDMRS Map.
 *******************************************************************************/
/* CA Changes start */
void addEntryToPhichGrpAndSeqArrays(InternalCellIndex internalCellIndex)
{
    UInt8 iPrbRa = 0;
    UInt8 nDmrs = 0;
    UInt8 sfIndex = 0;
    UInt8 nPhichGrp = 0;
    UInt8 numPHICHGroup = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numPHICHGroup; 

    /* The n-goup-PHICH is calculated as the formula in 9.1.2 of 36.213.
     * For FDD the I-PHICH value  is ZERO
     */
    for( ;iPrbRa < 100;iPrbRa++)
    {
        for(nDmrs = 0;nDmrs < MAX_DMRS_VALUE;nDmrs++)
        {
            phichGroupFromPrbDmrsMap_g[internalCellIndex][iPrbRa][nDmrs] =
              (iPrbRa + nDmrs) % numPHICHGroup;
            phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][iPrbRa][nDmrs] =
                (UInt8)((UInt8)(floor_wrapper(iPrbRa / numPHICHGroup)  +  nDmrs) % TWO_SPREAD_FACTOR);
        }
    }
    /* Initialize the phichSequenceNoPerPhichGroup_g */
    /* + COVERITY 5.0 */
    for(sfIndex = 0;sfIndex < MAX_PHICH_CONTAINER;sfIndex++)
    /* + COVERITY 5.0 */
    {
        nPhichGrp = 0;
        while(nPhichGrp < numPHICHGroup)
        {
            /* CR changes start */
            phichSequenceNoPerPhichGroup_g[internalCellIndex][sfIndex][nPhichGrp].bitMask = 0xFF;
            phichSequenceNoPerPhichGroup_g[internalCellIndex][sfIndex][nPhichGrp].count   = 0;
            /* CR changes end */
            nPhichGrp++;
        }
   }
}
#elif TDD_CONFIG
/*****************************************************************************
 * Function Name    : addEntryToPhichGrpAndSeqArrays
 * Inputs           : numPHICHGroup-Max Number of PHICH group supported
 *                    internalCellIndex - cell Index for MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function will populate a global array for PHICH group
 *                    from PRB nDMRS Map.
 *******************************************************************************/
void addEntryToPhichGrpAndSeqArrays(UInt8 numPHICHGroup,
        InternalCellIndex internalCellIndex)
{
    UInt8 iPrbRa = 0;
    UInt8 nDmrs = 0;
    UInt8 sfIndex = 0;
    UInt8 nPhichGrp = 0;
    UInt8 iPhich = 0;
    /* The n-goup-PHICH is calculated as the formula in 9.1.2 of 36.213.
     * The value of IPHICH can be 0 or 1 for Tdd cofig 0.It is 0 when the 
     * corresponding UL Tx occurs in subframe 4 or subframe 9 otherwise 0*/
    /*TDD Config 0 Changes Start*/
    for( ;iPhich < 2;iPhich++)
    /*TDD Config 0 Changes End*/     
    {
        for(iPrbRa = 0;iPrbRa < 100;iPrbRa++)
        {
            for(nDmrs = 0;nDmrs < MAX_DMRS_VALUE;nDmrs++)
            {
                /*TDD Config 0 Changes Start*/
                /*Included iPhich in phichgrp calculation*/
                phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][iPrbRa][nDmrs] =
                    (iPrbRa + nDmrs) % numPHICHGroup + (iPhich * numPHICHGroup);
                /*TDD Config 0 Changes End*/
                phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][iPrbRa][nDmrs] =
                    (UInt8)((UInt8)(floor_wrapper(iPrbRa / numPHICHGroup)  +  nDmrs) % TWO_SPREAD_FACTOR);
            }
        }
    }

    /* Initialize the phichSequenceNoPerPhichGroup_g */
    /* + COVERITY 5.0 */
    for(sfIndex = 0;sfIndex < MAX_SUB_FRAME;sfIndex++)
    /* + COVERITY 5.0 */
    {
        nPhichGrp = 0;
       /* phichSequenceNoPerPhichGroup_g to be initialised for iPhich = 1 as
        * well*/            
        while(nPhichGrp < (numPHICHGroup * 2))
        {
            /*CA Changes start  */
            phichSequenceNoPerPhichGroup_g[internalCellIndex][sfIndex][nPhichGrp] = 0xFF;
            /* CA changes end */
            nPhichGrp++;
        }
    }
}
#endif

/******************************************************************************
 * Function Name    : getPhichGrpAndPhichSeq
 * Inputs           : riStart - Start PRB Index,
 *                    nPhichGrp_p,
 *                    nPhichSeq_p,
 *                    sfUsed_p,
 *                    hiIndex
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : nDmrs_p - nDmrs that is returned back
 * Return           : nDmrs
 * Description      : This function will provide the nPhichGroup adn nPhichSeq
 *                    value for the given riStart.
 *******************************************************************************/
UInt8 getPhichGrpAndPhichSeq(UInt8 riStart,
        UInt8 *nPhichGrp_p,
        UInt8 *nPhichSeq_p,
        UInt8 *sfUsed_p,
        UInt8 *nDmrs_p,
        UInt8 hiIndex,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
/*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
        ,UInt8 iPhich
#endif
/*TDD Config 0 Changes End*/ 
        )
{
    UInt8 nDmrs= 0;
    UInt8 nPhichGrp = 0;
    UInt8 nPhichSeq = 0;


    while(nDmrs < MAX_DMRS_VALUE)
    {
/*TDD Config 0 Changes Start*/        
#ifdef TDD_CONFIG
        nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][riStart][nDmrs];
#elif FDD_CONFIG
        nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][riStart][nDmrs];
#endif        
/*TDD Config 0 Changes End*/        
        /*CA Changes start  */
        nPhichSeq = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][riStart][nDmrs];
        /*CA Changes end  */

        /* CR changes start */
#ifdef FDD_CONFIG
        /*CA Changes start  */
        if(phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp].bitMask
                & (ONE << nPhichSeq))
        /*CA Changes end  */
            /* CR changes end */
        {
            /* CR changes start */


            /*CA Changes start  */
            phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp].bitMask &=
                ~(ONE << nPhichSeq);
            phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp].count++;
            /*CA Changes end  */
            /* CR changes end */

            *nPhichGrp_p = nPhichGrp;
            *nPhichSeq_p = nPhichSeq;
            *sfUsed_p = hiIndex;
            *nDmrs_p = nDmrs;
            break;
        }            
#elif TDD_CONFIG

        if(phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp]
                & (ONE << nPhichSeq))
        {
            phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp] &=
                ~(ONE << nPhichSeq);
            *nPhichGrp_p = nPhichGrp;
            *nPhichSeq_p = nPhichSeq;
            *sfUsed_p = hiIndex;
            *nDmrs_p = nDmrs;

            break;
        }
#endif

        else
        {
            nDmrs++;
        }
    }
    if(MAX_DMRS_VALUE == nDmrs)
    {
        /* SPR 13557 Fix Start */
         nDmrs = 0;
         *nDmrs_p = 0;
        /* SPR 13557 Fix end */
        LTE_MAC_UT_LOG(LOG_WARNING, LOG_PHICH, "Inavlid NDMRS value obtained\n");
    }
    return nDmrs;
}

/******************************************************************************
* Function Name :   releasePhich
* Inputs        :   nPhichSf,
*                   nPhichGrp,
*                   nPhichSeq
*                   internalCellIndex - Cell-Index used at MAC
* Outputs       :   None
* Returns       :   None
* Description   :   This function will acquire & provide the nPhichGroup and 
*                   nPhichSeq value for the given riStart.
*******************************************************************************/
void releasePhich(UInt8 nPhichSf, 
        UInt8 nPhichGrp, 
        UInt8 nPhichSeq,
        /*CA Changes start */
        InternalCellIndex internalCellIndex)
        /*CA Changes end */
{
    /* CR changes start */
#ifdef FDD_CONFIG
    /*CA Changes start  */
    phichSequenceNoPerPhichGroup_g[internalCellIndex][nPhichSf][nPhichGrp].bitMask
        |= (ONE << nPhichSeq);
    /*CA Changes end  */
#elif TDD_CONFIG
    phichSequenceNoPerPhichGroup_g[internalCellIndex][nPhichSf][nPhichGrp]
        |= (ONE << nPhichSeq);
#endif
    /* CR changes end */
}
/*SPR_3305_FIX_START*/
/******************************************************************************
* Function Name :   getPhichGrpAndPhichSeqForMsg3
* Inputs        :   riStart - Start PRB Index,
*                   nPhichGrp_p,
*                   nPhichSeq_p,
*                   sfUsed_p,
*                   hiIndex
*                   internalCellIndex - Cell-Index used at MAC
* Outputs       :   nDmrs_p - nDmrs that is returned back
* Returns       :   MAC_SUCCESS/MAC_FAILURE
* Description   :   This function will provide the nPhichGroup adn nPhichSeq
*                   value for the given riStart for Msg3.
*******************************************************************************/
MacRetType getPhichGrpAndPhichSeqForMsg3(UInt8 riStart,
        UInt8 *nPhichGrp_p,
        UInt8 *nPhichSeq_p,
        UInt8 *sfUsed_p,
        UInt8 *nDmrs_p,
        UInt8 hiIndex,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
        ,UInt8 iPhich
#endif
       /*TDD Config 0 Changes End*/ 
        )
{
    UInt8 nDmrs= 0;
    UInt8 nPhichGrp = 0;
    UInt8 nPhichSeq = 0;
    /*TDD Config 0 Changes Start*/
#ifdef FDD_CONFIG
    nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][riStart][nDmrs];
#elif TDD_CONFIG
    nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][riStart][nDmrs];
#endif
    /*CA Changes start  */
    nPhichSeq = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][riStart][nDmrs];
    /*CA Changes end  */

#ifdef FDD_CONFIG
    /*CA Changes start  */
    if(phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp].bitMask
            & (ONE << nPhichSeq))
        /* CR changes end */
    {
        /* CR changes start */


        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp].bitMask &=
            ~(ONE << nPhichSeq);
        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp].count++;
        /*CA Changes end  */
        /* CR changes end */

        *nPhichGrp_p = nPhichGrp;
        *nPhichSeq_p = nPhichSeq;
        *sfUsed_p = hiIndex;
        *nDmrs_p = nDmrs;

        return MAC_SUCCESS;
    } 
#elif TDD_CONFIG
    if(phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp]
            & (ONE << nPhichSeq))
    {
        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp] &=
            ~(ONE << nPhichSeq);
        *nPhichGrp_p = nPhichGrp;
        *nPhichSeq_p = nPhichSeq;
        *sfUsed_p = hiIndex;
        *nDmrs_p = nDmrs;

        return MAC_SUCCESS;
	}
#endif
    return MAC_FAILURE;
}
/*SPR_3305_FIX_END*/

/******************************************************************************
 * Function Name    : processULNackSPSNonAdaptiveInPUSCH
 * Inputs           : ulDelay - The delay of execution legs in terms of the number
 *                              number subFrames w.r.t. PHY & the current processing 
 *                              being done at MAC for intended tick at PHY,
 *                    rbMapNum,
 *                    currentGlobalTick - The present Global Tick,
 *                    numberOfULUEsToBeScheudled_p,
 *                    containerTick - The Tick of the container being used.                      
 *                    hiIndex,
 *                    subFrame,
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This function will read the Nack Adaptive queue and update 
 *                    the HARQ process, and book the resources in advance. It 
 *                    will also create the HI PDUs. 
 ******************************************************************************/
#ifdef FDD_CONFIG
 void processULNackSPSNonAdaptiveInPUSCH(
                               UInt8 ulDelay,
                               UInt32 rbMapNum, /* SS_S2 */
                               /* SPR 15909 fix start */
                               tickType_t currentGlobalTick,
                               /* SPR 15909 fix end */
                               UInt8* numberOfULUEsToBeScheudled_p,
                               UInt8 containerTick,
                               /* CR changes start */
                               UInt8 hiIndex,
                               /* CR changes end */
                               UInt8 subFrame,
                               /*CA Changes start */
                               InternalCellIndex internalCellIndex)
                               /*CA Changes end */
{
    UInt16 tempUeIndex=0;
    HIPduArray* tempHIPdu_p  = PNULL;
    AllocatedResInfo resourceReserved = {0};
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; /* Invalid Value*/
    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p = PNULL;
    PdcchULOutputInfo* tempPdcchULOutput_p = PNULL;
    ULUEContext* ulUEContext_p = PNULL;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    UInt8 localCount = 0;
    ULHarqInfo* tempULHarqInfo_p = PNULL;
    /* ICIC changes start */
    RbMapNode *rbMapNode_p = PNULL;
    UInt32 tempPUSCHResources[MAX_USER_LOCATION] = {0};
    UInt8 ceOverlapRBs = 0;
    /* ICIC changes end */
    UInt16  numberOfInfoPresent = 0;
    /* SPR 9173 changes start */
    UInt8 ulSubFrameNum  = (currentGlobalTick + FDD_HARQ_OFFSET + ulDelay )% MAX_SUB_FRAME;
    /* SPR 9173 changes end */
#ifdef ULHARQ_TIMER_PROC
    /* SPR 15909 fix start */
    tickType_t timerExpiredTTI = currentGlobalTick + ulDelay  + START_ULHARQ_TIMER_OFFSET;
    /* SPR 15909 fix end */
    /* SPR 4386 changes start */
    UInt8 subFrameNum = (subFrame + ulDelay);
    UInt8 timerExpsubFrameNum = (subFrameNum + START_ULHARQ_TIMER_OFFSET);
    UInt32 ulHarqSysFrameNum = 0;
    UInt32 ulHarqSubFrameNum = 0;
    /*CA Changes start  */
    getSFAndSFN(&ulHarqSysFrameNum, &ulHarqSubFrameNum, internalCellIndex);
    /*CA Changes end  */
    /* SPR 4386 changes end */
    if(timerExpsubFrameNum >= MAX_SUB_FRAME)
    {
        timerExpsubFrameNum = timerExpsubFrameNum % MAX_SUB_FRAME;

        ulHarqSysFrameNum++;
        if (ulHarqSysFrameNum == MAX_SFN_VALUE)
        {
            ulHarqSysFrameNum = 0;
        }
    }
#endif
    ULSchUciRachPoolInfo* ulschUciRachInfo_p 
        /*CA Changes start  */
        = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
        /*CA Changes end  */

    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    /*CA Changes start  */
    tempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */

    /* First serve the Non Adaptive UEs */
    localCount = tempPdcchULOutput_p->pdcchULNackSpsUEInfo.nonAdaptiveCount;
    /* ICIC changes start */
    /*CA Changes start  */
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    tempPUSCHResources[CC_CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter; 
    tempPUSCHResources[CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter; 
    /* ICIC changes end */


    LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "\n########\n containerTick = %d "
            " tempPdcchULOutput_p->pdcchULNackSpsUEInfo.nonAdaptiveCount = %d \n"
        " tempPUSCHResources = %d  rbMapNum = %d\n"
            " numberOfULUEsToBeScheudled_p = %d\n########\n"
            ,containerTick, tempPdcchULOutput_p->pdcchULNackSpsUEInfo.nonAdaptiveCount, 
        tempPUSCHResources, rbMapNum,*numberOfULUEsToBeScheudled_p); /* SS_S2 */

    while(localCount--)
    {
        tempUeIndex = \
                      tempPdcchULOutput_p->pdcchULNackSpsUEInfo.pdcchULNackNonAdaptiveUEArray[localCount];
        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];         
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "\ntempUeIndex = %d",tempUeIndex);

        if (!ulUEContextInfo_p->pendingDeleteFlag)
        {
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
            ulUEContext_p->isPdcchAllocated = 0;
            tempDirectIndexingUEInfo_p = \
                                         &(tempPdcchULOutput_p->directIndexingUEInfo[tempUeIndex]); 
            tempHarqProcessId = tempDirectIndexingUEInfo_p->harqIndex;
            /* MEAS_GAP_CHG */
            tempULHarqInfo_p = \
                               &(ulUEContext_p->ulresInfo[tempHarqProcessId]);
            if (MEAS_GAP_RE_TX_NO_CCE_NO_GRANT == 
                    tempDirectIndexingUEInfo_p->flag)
            {
#ifdef ULHARQ_TIMER_PROC
                ulHarqTimerStart(tempUeIndex, tempHarqProcessId,
                        timerExpsubFrameNum,
                        /* + SPS_TDD_Changes */
                        SPS_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        timerExpiredTTI,ulHarqSysFrameNum,
                        /*CA Changes start  */
                        internalCellIndex);
                        /*CA Changes end  */
#endif
                 tempULHarqInfo_p->currentRetransCount++; 
                /* should numGrantsAllocated,stats be updated */
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX; 

                /*mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->
                        ulCCEIndex].dciPduInfo.aggregationLvl,
                        (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->
                         ulCCEIndex])); */

                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                addEntryTOHIPduArray(
                        &ulUEContext_p->ulresInfo[tempHarqProcessId],
                        /* SPR 5467 changes start*/ 
                        HI_ACK,
                        /* SPR 5467 changes end*/
                        ulUEContext_p->crnti,
                        /* CR changes start */
                        ulUEContext_p,
                        tempULHarqInfo_p->sfUsed,
                        /* CR changes end */
                        tempHIPdu_p,
                        internalCellIndex);
                
                /* CR changes start */
                /*CA Changes start  */
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                    ~(ONE << tempULHarqInfo_p->nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                /*CA Changes end  */
                tempULHarqInfo_p->sfUsed = hiIndex;
                /* CR changes end */
                continue;
            }
            /* MEAS_GAP_CHG */
            /*
               We now create the HI PDU corresponding to the Nack received
             */
            addEntryTOHIPduArray(
                    tempULHarqInfo_p,
                    HI_NACK,
                    ulUEContext_p->crnti,
                    /* CR changes start */
                    ulUEContext_p,
                    tempULHarqInfo_p->sfUsed,
                    /* CR changes end */
                    tempHIPdu_p,
                    internalCellIndex);

            /*
               We at first check whether the UE has completed it's NACK re
               transmission limit, if yes we will free the HARQ Process and
               continue to the next UE.
               */
            if (tempDirectIndexingUEInfo_p->flag == RECEIVED_NACK_RE_TRANS_COMPELETE)
            {
                /*
                 * If a UL MAX RTXN Complete then the
                 * corresponding  nPhichSeq th bit from the LBS of the array
                 * phichSequenceNoPerPhichGroup_g[internalCellIndex][Subframe No][  ] 
                 * should be made to set.
                 */
                /* CR changes start */

                /* CR changes end */
                LOG_MAC_MSG(MAC_NACK_RETX_COMP_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        0,0,0,0.0,0.0,__func__,"NACK_RETX_COMP_FREE");

                /* SPR 16916 FIX START */
#ifdef KPI_STATS 
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */
                /* SPR 19704 + */
                /*
#ifdef FDD_CONFIG

                if(!(ulUEContext_p->ulresInfo[tempHarqProcessId].isTTIBundled) )
#endif          
                {      
                    tempULHarqInfo_p->harqStatus = HARQ_PROCESS_IN_USE;
                }
                */
                /* SPR 19704 - */

#ifdef FDD_CONFIG
                updateTTIHarqMap(tempHarqProcessId,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
#endif

                /*Qos Changes*/
                restoreTokensOfNonTransmittedLCs(ulUEContext_p, 
                        tempULHarqInfo_p->ulHarqProcessId);
                /*Qos Changes*/
                tempDirectIndexingUEInfo_p->flag = UNSET_FLAG;
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                continue;  
            } 
            /* ICIC changes start */
            if (tempPUSCHResources[CC_CE_REGION] >= tempULHarqInfo_p->riLength)
                /* ICIC changes end */
            {
                /* + SPR 16758 */
                /*We use 1 for Retransmission */ 
                ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 1;
                /* - SPR 16758 */

                /* SPR 10027 FIX START */
                /* Check if isDrbPresent = TRUE and Update the lastScheduledTick */
                CHECK_AND_UPDATE_LAST_SCHEDULED_TICK(
                        tempULHarqInfo_p->isDrbPresent,ulUEContext_p)
                /* SPR 10027 FIX END */

                /* ICIC changes start */
                tempPUSCHResources[CC_CE_REGION] -= reserveAlreadyAvailableResourcesForRetx( \
                        tempULHarqInfo_p->riStart, \
                        tempULHarqInfo_p->riLength,\
                        &rbMapNode_p->availableResourceInfo[CC_CE_REGION],\
                        rbMapNode_p->rbMap); 
                /* CC resouces are updated always irrespective of user location,
                 * we check here whether the RBs lied in CE region and update the 
                 * resouce info if necessary*/
                /*CA Changes start  */
                ceOverlapRBs = updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                                        tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                                        internalCellIndex);
                /*CA Changes end  */
                if(0 < ceOverlapRBs)
                {
                    tempPUSCHResources[CE_REGION] -= ceOverlapRBs;
                }
                /* ICIC changes end */
                LOG_MAC_MSG(MAC_NACK_RES_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        rbMapNum,NON_ADAPTIVE_RE_TX,
                        tempULHarqInfo_p->currentRetransCount,
                        tempULHarqInfo_p->riLength,tempULHarqInfo_p->riStart,
                        __func__,"NACK_RES_ULHARQ_PASS");


                LOG_MAC_MSG(MAC_NACK_RES_ALLOC_INFO,LOGDEBUG,MAC_UL_HARQ,\
                        getCurrentTick(),\
                        tempULHarqInfo_p->riStart,
                        tempULHarqInfo_p->riLength,\
                        tempPUSCHResources[CC_CE_REGION],\
                        tempPUSCHResources[CE_REGION],\
                        0,0.0,0.0,\
                        __func__,"NACK_RES_ALLOC");
            }
            else
            {
                LOG_MAC_MSG(MAC_NACK_RES_ZERO_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        rbMapNum, 
                        tempULHarqInfo_p->nextReTransmissionType,
                        tempULHarqInfo_p->currentRetransCount,
                        0.0,0.0,__func__,"NACK_RES_ZERO_FAIL");

                /* ICIC changes start */
                tempPUSCHResources[CC_CE_REGION]  = 0;
                /* ICIC changes end */
            }
            /* CR changes start */

            /*CA Changes start  */
            phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                ~(ONE << tempULHarqInfo_p->nPhichSeq);
            phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
            /*CA Changes end  */
            tempULHarqInfo_p->sfUsed = hiIndex;
            /* CR changes end */

            LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                    tempUeIndex,
                    currentGlobalTick,
                    NON_ADAPTIVE_RE_TX,
                    tempHarqProcessId,
                    tempULHarqInfo_p->riLength,
                    tempULHarqInfo_p->riStart,
                    tempDirectIndexingUEInfo_p->ulGrantRequestType,
                    tempULHarqInfo_p->deltabsrLoad,
                    FALSE,
                    tempULHarqInfo_p->mcsIndex,
                    0,
                    TPC_VALUE_1 
                     /* UL_MU_MIMO_CHG START */
                     , ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                     INVALID_UE_INDEX,
                     ulUEContext_p->userLocationType,
                     tempULHarqInfo_p->nDmrs,
                     ulUEContext_p->modulationScheme,
                     /* UL_MU_MIMO_CHG END */
                     /* CA Stats Changes Start */
                     internalCellIndex
                     /* CA Stats Changes End */
                   );
            
            /* Required resources are not available, hence increase the current
             * ReTx count and start the UL Timer */
            updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
            /* + SPR 16758 */
            /*We use 1 for Retransmission */ 
            ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 1;
            /* - SPR 16758 */

#ifdef ULHARQ_TIMER_PROC
            ulHarqTimerStart(tempUeIndex,tempHarqProcessId,timerExpsubFrameNum,
                    /* + SPS_TDD_Changes */
                    SPS_SCHEDULING,
                    /* - SPS_TDD_Changes */
                    timerExpiredTTI,ulHarqSysFrameNum,
                    /*CA Changes start  */
                    internalCellIndex);
                    /*CA Changes end  */
#endif 
            (*numberOfULUEsToBeScheudled_p)--;

            /* Semi static changes Start*/
            if(ULSCH_UCI_INVALID_INDEX != tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
            {
                /* Fill UL Harq Info*/
                fillSemiStaticInfoForULSCH (ulschUciRachInfo_p,
                        tempDirectIndexingUEInfo_p,
                        ulUEContextInfo_p,
                        tempULHarqInfo_p,
                        tempHarqProcessId,
			/* SPR 9173 changes start */
                        ulSubFrameNum,0, internalCellIndex);
			/* SPR 9173 changes end */
            }
            else
            {
                if ( MAX_ULSCH_UCI_SUPPORTED <=  numberOfInfoPresent)
                {
                    ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                            __func__);
                    break;
                }
                /*SPR 7086 Fix Start*/
                /* SPR 10195 Fix Start */
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS =
                    nSRSDetermineOnCollision(ulUEContext_p, tempHarqProcessId, ulSubFrameNum, internalCellIndex);
                /* SPR 10195 Fix End */
                /*SPR 7086 Fix End*/

                /* Fill UL Harq Info*/
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p = 
                  tempULHarqInfo_p;
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId = 
                  tempHarqProcessId;
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = 
                    MAC_SS_PDU_ULSCH;
                /** For TTIB_Code */
                /* + SPS_TDD_Changes */
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti =  ulUEContext_p->spsUlInfo.SpsCrnti;
                /* - SPS_TDD_Changes */

                numberOfInfoPresent++;
                LOG_UT(MAC_UCI_ULSCH_PDU_ASSIGN,LOGDEBUG,MAC_PUSCH,\
                    getCurrentTick(),\
                    tempUeIndex,
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType,\
                    tempDirectIndexingUEInfo_p->ulschUciInfoIndex,\
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId,\
                    0,0.0,0.0,\
                    __func__,"ULSCH_PDU_ASSIGN");

            }
            /* Semi static changes End*/


            tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK;
        }
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, 
                "\ntempPUSCHResources = %d numberOfULUEsToBeScheudled_p = %d\n"
                ,tempPUSCHResources,*numberOfULUEsToBeScheudled_p);
    }
    /*Update Number of numberOfInfoPresent in the global context */ 
    ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;

}
/* + SPS_TDD_Changes */
#elif TDD_CONFIG
/******************************************************************************
 * Function Name    : processULNackSPSNonAdaptiveInPUSCH
 * Inputs           : tempPdcchULOutput_p - The pointer to PdcchULOutputInfo,
 *                    rbMapNum,
 *                    subFrameNumber
 *                    currentGlobalTick - The present Global Tick,
 *                    numberOfULUEsToBeScheudled_p - Pointer to no. of UL UE's to be
 *                                                   scheduled,
 *                    containerTick - The Tick of the container being used
 * Outputs          : None
 * Returns          : None
 * Description      : This function will read the Nack Adaptive queue and update
 *                    the HARQ process, and book the resources in advance. It
 *                    will also create the HI PDUs.
 ******************************************************************************/
 void processULNackSPSNonAdaptiveInPUSCH(
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
        UInt8 ulSubFrameNum
        /*TDD Config 0 Changes End*/ 
        )

{
    UInt16 tempUeIndex=0;
    HIPduArray* tempHIPdu_p  = PNULL;
    AllocatedResInfo resourceReserved = {0};
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; /* Invalid Value*/
    DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    ULUEContext* ulUEContext_p = PNULL;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    UInt8 localCount = 0;
    ULHarqInfo* tempULHarqInfo_p = PNULL;
    /* ICIC changes start */
    RbMapNode *rbMapNode_p = PNULL;
    UInt32 tempPUSCHResources[MAX_USER_LOCATION] = {0};
    UInt8 ceOverlapRBs = 0;
    /* ICIC changes end */
    /* TDD Config 6 Changes Start */
    UInt8 prevUlSubFrameNum  = 0;
    UInt16 prevUlSysFrameNum = 0;
    /* TDD Config 6 Changes End */
    UInt16 ulSysFrameNum = 0;
    PdcchULNackUEInfo *pdcchULNackSpsUEInfo_p  = PNULL;
    UeScheduledInfo *ueScheduledInfo_p = PNULL;
    /*CA Changes start  */
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick;
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];
    /*TDD Config 0 Changes Start*/
    UInt8 subFrameNum = currentGlobalTick % MAX_SUB_FRAME;
    UInt8 ulSubFrameOffset =  getUlOffset(subFrameNum,ulSubFrameNum);

    UInt32 ulGlobalTick = currentGlobalTick + ulSubFrameOffset;
    UInt8 phichSubFrameNum = 0;
     /*TDD Config 0 Changes End*/

    UInt16 sysFrameNum = (currentGlobalTick / MAX_SUB_FRAME) % MAX_SFN_VALUE;
    /* Time Averaging changes start */
    UInt16  numberOfInfoPresent = 0;

    /*TDD Config 0 Changes Start*/
    /*ulSchUciRachContainer_g is now accessed on the basis of U frame*/
    ULSchUciRachPoolInfo* ulschUciRachInfo_p
        /*CA Changes start  */
        = ulSchUciRachContainer_gp[internalCellIndex] + (ulSubFrameNum % MAX_ULSCH_UCI_CONTAINER); 
        /*CA Changes end  */
    /*TDD Config 0 Changes End*/ 
    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    pdcchULNackSpsUEInfo_p = &(tempPdcchULOutput_p->pdcchULNackSpsUEInfo);
    /* First serve the Non Adaptive UEs */
    localCount = pdcchULNackSpsUEInfo_p->nonAdaptiveCount;
    /* ICIC changes start */
    /*CA Changes start  */
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    tempPUSCHResources[CC_CE_REGION] =
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter;
    tempPUSCHResources[CE_REGION] =
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter;
    /* ICIC changes end */

    if (ulSubFrameNum < subFrameNumber)
    {
        ulSysFrameNum = MODULO_ONEZEROTWOFOUR((sysFrameNum + 1));
    }
    else
    {
        ulSysFrameNum = sysFrameNum;
    }

    /*TDD Config 0 Changes Start*/
    prevUlSubFrameNum = (ulGlobalTick - getprevUlSfOffsetFromCurrentUlSf(ulSubFrameNum, 
                        /* CA TDD CHANGES START */
                        internalCellIndex
                        /* CA TDD CHANGES END */
                        )) % MAX_SUB_FRAME;
    GET_UL_SYS_FRAME_NUM_FROM_PHICH(prevUlSubFrameNum, subFrameNumber, prevUlSysFrameNum, sysFrameNum);
    /*TDD Config 0 Changes End*/
#ifdef MAC_AUT
    prevUlSubFrameNo = prevUlSubFrameNum;
#endif

    while(localCount--)
    {
        tempUeIndex = \
                      pdcchULNackSpsUEInfo_p->pdcchULNackNonAdaptiveUEArray[localCount];
        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];

        if (!ulUEContextInfo_p->pendingDeleteFlag)
        {
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
            ulUEContext_p->isEntryPushedInAdvanceQueue = 0;
            ulUEContext_p->isPdcchAllocated = 0;
            if ( !isValidSubframeNumForDCI_0(subFrameNumber, internalCellIndex))
            {
                ltePanic("Invalid DL Subframe[%d] For DCI 0 \n",subFrameNumber);
            }
            /* + SPR 20690 */
            ueScheduledInfo_p = ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulSubFrameNum];
            /* - SPR 20690 */

            if ( !ueScheduledInfo_p )
            {
                ltePanic("[%s] ueScheduledInfo_p is null\n",
                        __func__);
                /* + Coverity 88280 */
                break; 
                /* - Coverity 88280 */
            }
            tempHarqProcessId = ueScheduledInfo_p->ulHarqProcessId;

            /* MEAS_GAP_CHG */
            tempULHarqInfo_p = \
                               &(ulUEContext_p->ulresInfo[tempHarqProcessId]);
            /*TDD Config 0 Changes Start*/
            tempULHarqInfo_p->isPdcchAllocated = 0;
            phichSubFrameNum = ulSubFrameNum + getHiPduSubframeNum(ulSubFrameNum,internalCellIndex);
            phichSubFrameNum = phichSubFrameNum % MAX_SUB_FRAME;
            /*TDD Config 0 Changes End*/

            if (MEAS_GAP_RE_TX_NO_CCE_NO_GRANT ==
                        ueScheduledInfo_p->flag ||
                        (MAX_UE_SCHEDULED_NO_RETX_NO_GRANT == ueScheduledInfo_p->flag))
            {

                if (ULSCH_UCI_INVALID_INDEX !=
                        ueScheduledInfo_p->ulschUciInfoIndex)
                {
                    /* + SPR_14994 */
                    checkAndRestoreUCIPduType(ulUEContext_p, ueScheduledInfo_p,
                            ulSysFrameNum, ulschUciRachInfo_p, ulSubFrameNum,
                            /*SPR 19504 fix start*/    
                            rbMapNum,
                            internalCellIndex);
                            /*SPR 19504 fix end*/    
                    /* - SPR_14994 */
                }

                tempULHarqInfo_p->currentRetransCount++;
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;

                resetHarqId(prevUlSubFrameNum, ulUEContext_p, internalCellIndex,prevUlSysFrameNum);
                setHarqBusy(ulSubFrameNum, tempHarqProcessId, ulUEContext_p,internalCellIndex, ulSysFrameNum);

                ulHarqTimerStart(
                        subFrameNumber,
                        sysFrameNum,
                        tempUeIndex,
                        tempHarqProcessId,
                        /* + SPS_TDD_Changes */
                        SPS_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        /*CA Changes start  */
                        internalCellIndex,
                        /*CA Changes end  */
                        /* TDD Config 0 Changes Start */
                        ulSubFrameNum);
                /* TDD Config 0 Changes End */

                /* + SPR 16758 */
                ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 1; 
                /* - SPR 16758 */

                if ((ueScheduledInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) &&
                        (INVALID_CCE_INDEX != ueScheduledInfo_p->ulCCEIndex))
                {
                    mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                            dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                            internalCellIndex);
                }

                ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                {
                    addEntryTOHIPduArray(
                            &ulUEContext_p->ulresInfo[tempHarqProcessId],
                            HI_ACK,
                            ulUEContext_p->crnti,
                            tempHIPdu_p,
                            internalCellIndex,
                            /* TDD Config 0 Changes Start */
                            prevUlSubFrameNum);
                    /* TDD Config 0 Changes End */
                    tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                }
                continue;
            }
            /* MEAS_GAP_CHG */
            /*
               We at first check whether the UE has completed it's NACK re
               transmission limit, if yes we will free the HARQ Process and
               continue to the next UE.
             */
            if (ueScheduledInfo_p->flag == RECEIVED_NACK_RE_TRANS_COMPELETE)
            {
                /*
                 * If a UL MAX RTXN Complete then the
                 * corresponding  nPhichSeq th bit from the LBS of the array
                 * phichSequenceNoPerPhichGroup_g[Subframe No][  ]
                 * should be made to set.
                 */
                /*
                phichSequenceNoPerPhichGroup_g[tempULHarqInfo_p->sfUsed][tempULHarqInfo_p->nPhichGrp]
                    |= (ONE << tempULHarqInfo_p->nPhichSeq);*/
                LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "Ret Trans complete Now Free the Harq Process Id [%d]\n", subFrameNumber);
                LOG_MAC_MSG(MAC_NACK_RETX_COMP_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        0,0,0,0.0,0.0,__func__,"NACK_RETX_COMP_FREE");
                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                freeULHarqProcess(tempULHarqInfo_p,ulUEContext_p->ueIndex,internalCellIndex);
#else
                freeULHarqProcess(tempULHarqInfo_p);
#endif
                /* SPR 16916 FIX END */
                ueScheduledInfo_p->flag = UNSET_FLAG;
                ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                ulUEContext_p->isEntryPushedInAdvanceQueue = 0;
                /** CA-TDD Changes Start:05 **/
                resetHarqId( getULGrantSubframeNum(subFrameNumber, 0, internalCellIndex),
                        ulUEContext_p, internalCellIndex,prevUlSysFrameNum
                        );
                /** CA-TDD Changes End:05 **/

                continue;
            }
            /* ICIC changes start */
            MacRetType   retVal = MAC_FAILURE;
            if (tempPUSCHResources[CC_CE_REGION] >= tempULHarqInfo_p->riLength)
                /* ICIC changes end */
            {
                /* + SPR 16758 */
                /*We use 1 for Retransmission */
                ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 1;
                /* - SPR 16758 */

                /* Check if isDrbPresent = TRUE and Update the lastScheduledTick */
                CHECK_AND_UPDATE_LAST_SCHEDULED_TICK(
                        tempULHarqInfo_p->isDrbPresent,ulUEContext_p)

                    retVal = checkResourceAvailability(tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,rbMapNode_p->rbMap);

                if (retVal == MAC_FAILURE)
                {
                    tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                    if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                    {
                        addEntryTOHIPduArray(
                                tempULHarqInfo_p,
                                HI_ACK,
                                ulUEContext_p->crnti,
                                tempHIPdu_p,
                                internalCellIndex,
                                /* TDD Config 0 Changes Start */
                                prevUlSubFrameNum);
                        /* TDD Config 0 Changes End */
                        tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                    }
                    //SPS_TDD_Fix
                    ulHarqTimerStart( subFrameNumber,
                            sysFrameNum,
                            tempUeIndex,
                            tempHarqProcessId,
                            /* + SPS_TDD_Changes */
                            SPS_SCHEDULING,
                            /* - SPS_TDD_Changes */
                            /*CA Changes start  */
                            internalCellIndex,
                            /*CA Changes end  */
                            /* TDD Config 0 Changes Start */
                            ulSubFrameNum);
                    /* TDD Config 0 Changes End */
                    updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
                    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                    ueScheduledInfo_p->flag = RECEIVED_NACK;
                    continue;
                    //SPS_TDD_Fix
                }
                else
                {
                    addEntryTOHIPduArray(
                            tempULHarqInfo_p,
                            HI_NACK,
                            ulUEContext_p->crnti,
                            tempHIPdu_p,
                            /*CA Changes start  */
                            internalCellIndex,
                            /*CA Changes end  */
                            /* TDD Config 0 Changes Start */
                            prevUlSubFrameNum);
                    /* TDD Config 0 Changes End */
                    /* ICIC changes start */
                    tempPUSCHResources[CC_CE_REGION] -= reserveAlreadyAvailableResourcesForRetx( \
                            tempULHarqInfo_p->riStart, \
                            tempULHarqInfo_p->riLength,\
                            &rbMapNode_p->availableResourceInfo[CC_CE_REGION],\
                            rbMapNode_p->rbMap);
                    /* CC resouces are updated always irrespective of user location,
                     * we check here whether the RBs lied in CE region and update the
                     * resouce info if necessary*/
                    ceOverlapRBs = updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                            tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */
                    if(0 < ceOverlapRBs)
                    {
                        tempPUSCHResources[CE_REGION] -= ceOverlapRBs;
                    }
                    /* ICIC changes end */

                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_INFO,LOGDEBUG,MAC_UL_HARQ,\
                            getCurrentTick(),\
                            tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,\
                            tempPUSCHResources[CC_CE_REGION],\
                            tempPUSCHResources[CE_REGION],\
                            0,0.0,0.0,\
                            __func__,"NACK_RES_ALLOC");
                }
            }
            else
            {
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                /* required resources are not available hence start UL Timer*/
                /* SPR 10803 [BUG 848] FIX START */
                /* We now create the HI PDU corresponding to the Nack received */
                if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                {
                    addEntryTOHIPduArray(
                            tempULHarqInfo_p,
                            /* SPR 10803 [BUG 848] FIX START */
                            HI_ACK,
                            /* SPR 10803 [BUG 848] FIX END */
                            ulUEContext_p->crnti,
                            tempHIPdu_p,
                            /*CA Changes start  */
                            internalCellIndex,
                            /*CA Changes end  */
                            /* TDD Config 0 Changes Start */
                            prevUlSubFrameNum);
                    /* TDD Config 0 Changes End */
                    tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                }
                /* ICIC changes start */
                tempPUSCHResources[CC_CE_REGION]  = 0;
                /* ICIC changes end */
                LOG_MAC_MSG(MAC_NACK_RES_ZERO_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        rbMapNum,
                        tempULHarqInfo_p->nextReTransmissionType,
                        tempULHarqInfo_p->currentRetransCount,
                        0.0,0.0,__func__,"NACK_RES_ZERO_FAIL");
            }

            /*TDD Config 0 Changes Start*/
            /*Block the same phich resources as given earlier in Non adaptive
             * Retx*/
			/* CA-TDD Changes Start */
            phichSequenceNoPerPhichGroup_g[internalCellIndex][phichSubFrameNum][tempULHarqInfo_p->nPhichGrp] &=
                ~(ONE << tempULHarqInfo_p->nPhichSeq);
			/* CA-TDD Changes End */
            tempULHarqInfo_p->sfUsed = phichSubFrameNum;
            /*TDD Config 0 Changes End*/
            LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                    tempUeIndex,
                    currentGlobalTick,
                    NON_ADAPTIVE_RE_TX,
                    tempHarqProcessId,
                    tempULHarqInfo_p->riLength,
                    tempULHarqInfo_p->riStart,
                    ueScheduledInfo_p->ulGrantRequestType,
                    tempULHarqInfo_p->deltabsrLoad,
                    FALSE,
                    tempULHarqInfo_p->mcsIndex,
                    0,
                    TPC_VALUE_1
                    /* UL_MU_MIMO_CHG START */
                    , ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                    ueScheduledInfo_p->buddyUeIndex,
                    ulUEContext_p->userLocationType,
                    tempULHarqInfo_p->nDmrs,
                    ulUEContext_p->modulationScheme,
                    /* UL_MU_MIMO_CHG END */
                    /* CA Stats Changes Start */
                    internalCellIndex
                        /* CA Stats Changes End */
                    );
            /* TDD Config 6 Changes Start */
            /* Required resources are not available, hence increase the current
             * ReTx count and start the UL Timer */
            resetHarqId(prevUlSubFrameNum, ulUEContext_p, internalCellIndex,prevUlSysFrameNum);
            setHarqBusy(ulSubFrameNum, tempHarqProcessId, ulUEContext_p,internalCellIndex, ulSysFrameNum);
            /* TDD Config 6 Changes End */
            ulHarqTimerStart( subFrameNumber,
                    sysFrameNum,
                    tempUeIndex,
                    tempHarqProcessId,
                    /* + SPS_TDD_Changes */
                    SPS_SCHEDULING,
                    /* - SPS_TDD_Changes */
                    /*CA Changes start  */
                    internalCellIndex,
                    /*CA Changes end  */
                    /* TDD Config 0 Changes Start */
                    ulSubFrameNum);
            /* TDD Config 0 Changes End */
            /* Required resources are not available, hence increase the current
             * ReTx count and start the UL Timer */
            updateULHarqProcess(tempULHarqInfo_p, &resourceReserved, DO_NOT_UPDATE);
            (*numberOfULUEsToBeScheudled_p)--;

            /* Semi static changes Start*/
            if (retVal != MAC_FAILURE)
            {
                if(ULSCH_UCI_INVALID_INDEX != ueScheduledInfo_p->ulschUciInfoIndex)
                {
                    /* Fill UL Harq Info*/
                    fillSemiStaticInfoForULSCH (ulschUciRachInfo_p,
                            ueScheduledInfo_p,
                            ulUEContextInfo_p,
                            tempULHarqInfo_p,
                            tempHarqProcessId,
                            /* SPR 9173 changes start */
                            /* SPR 5620 START*/
                            ulSysFrameNum,
                            /* SPR 5620 END*/
                            /*CA Changes start  */
                            ulSubFrameNum,0, internalCellIndex);
                    /*CA Changes end  */
                    ueScheduledInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
                }
                else
                {
                    if ( MAX_ULSCH_UCI_SUPPORTED ==  numberOfInfoPresent)
                    {
                        ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                                __func__);
                        break;
                    }
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS =
                        /*CA Changes start  */
                        nSRSDetermineOnCollision(ulUEContext_p, tempHarqProcessId, ulSubFrameNum, internalCellIndex);
                    /*CA Changes end  */

                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p =
                        tempULHarqInfo_p;
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId =
                        tempHarqProcessId;
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType =
                        MAC_SS_PDU_ULSCH;
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti =
                        ulUEContext_p->spsUlInfo.SpsCrnti;

                    /*TDD Config 0 Changes Start*/
                    /*Populate ulschUciInfoIndex in ueScheduledInfo_p and
                     * ueScheduledInfo_p in ulschUciRachInfo_p.This info is
                     * needed in PUCCH for TDD_CONFIG0*/
                    ueScheduledInfo_p->ulschUciInfoIndex = numberOfInfoPresent;
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ueScheduledInfo_p = ueScheduledInfo_p;
                    /*TDD Config 0 Changes End*/

                    LOG_UT(MAC_UCI_ULSCH_PDU_ASSIGN,LOGDEBUG,MAC_PUSCH,\
                            getCurrentTick(),\
                            tempUeIndex,
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType,\
                            ueScheduledInfo_p->ulschUciInfoIndex,\
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId,\
                            0,0.0,0.0,\
                            __func__,"ULSCH_PDU_ASSIGN");

                     numberOfInfoPresent++;
               }
            }

            ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
            ueScheduledInfo_p->flag = RECEIVED_NACK;

        }
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH,
                "\ntempPUSCHResources = %d numberOfULUEsToBeScheudled_p = %d\n"
                ,tempPUSCHResources,*numberOfULUEsToBeScheudled_p);
    }
    /*Update Number of numberOfInfoPresent in the global context */
    ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;
}
/* - SPS_TDD_Changes */
#endif

/******************************************************************************
 * Function Name :    processULNackAdaptiveInPUSCH
 * Inputs        :    ulDelay - The delay of execution legs in terms of the number
 *                              subFrames w.r.t. PHY & the current processing being
 *                              done at MAC for intended tick at PHY,
 *                    rbMapNum,
 *                    currentGlobalTick - The present Global Tick
 *                    numberOfULUEsToBeScheudled_p,
 *                    containerTick - The Tick of the container being used,
 *                    hiIndex,
 *                    subFrame,
 *                    locType,
 *                    internalCellIndex - Cell-Index used at MAC
 * Outputs       :    None
 * Returns       :    None
 * Description   :    This function will read the Nack Adaptive queue and update the 
 *                    HARQ process, and book the resources in advance. It will 
 *                    also create the HI PDUs. 
 ******************************************************************************/
#ifdef FDD_CONFIG
 void processULNackSPSAdaptiveInPUSCH(
    UInt8 ulDelay,
    UInt32 rbMapNum, /* SS_S2 */
    /* SPR 15909 fix start */
    tickType_t currentGlobalTick,
    /* SPR 15909 fix end */
    UInt8* numberOfULUEsToBeScheudled_p,
    UInt8 containerTick,
    /* CR changes start */
    UInt8 hiIndex,
    /* CR changes end */
    UInt8 subFrame,
    UInt8 locType,
    /*CA Changes start */
    InternalCellIndex internalCellIndex)
    /*CA Changes end */
{
    UInt16 tempUeIndex=0;
    HIPduArray* tempHIPdu_p  = PNULL;
    AllocatedResInfo resourceReserved = {0};
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; /* Invalid Value*/
    DirectIndexingUEInfo*  tempDirectIndexingUEInfo_p = PNULL;
    DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    PdcchULOutputInfo* tempPdcchULOutput_p = PNULL;
    ULUEContext* ulUEContext_p = PNULL;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    UInt8 localCount = 0;
    ULHarqInfo* tempULHarqInfo_p = PNULL;
    /* ICIC changes start */
    /* Initialized by any region and changed in case of Cell edge user 
     * as CE_REGION_PREFERRED if configured so */
    UInt8 regionAllocated = CC_CE_ANY_REGION;
    RbMapNode *rbMapNode_p = PNULL;
    UInt32 tempPUSCHResources[MAX_USER_LOCATION] = {0};
    MacRetType   retVal = MAC_FAILURE;
    UInt8 ceOverlapRBs = 0;
    /* ICIC changes end */
    UInt16  numberOfInfoPresent = 0;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    /* Time Averaging changes start */
    UInt32 ueMaxPowerFlag= 0;
    UInt32 ueMinPowerFlag = 0;
    /* Time Averaging changes end */
    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */
    /* SPR 9173 changes start */
    UInt8 ulSubFrameNum  = (currentGlobalTick + FDD_HARQ_OFFSET + ulDelay )% MAX_SUB_FRAME;
    /* SPR 9173 changes end */
#ifdef ULHARQ_TIMER_PROC
     /* SPR 4386 changes start */
     UInt8 subFrameNum = (subFrame + ulDelay);
     UInt8 timerExpsubFrameNum = (subFrameNum + START_ULHARQ_TIMER_OFFSET); 		
     UInt32 ulHarqSysFrameNum = 0;
     UInt32 ulHarqSubFrameNum = 0;
     /*CA Changes start  */
     getSFAndSFN(&ulHarqSysFrameNum, &ulHarqSubFrameNum, internalCellIndex);              
     /*CA Changes end  */
     /* SPR 4386 changes end */
     if(timerExpsubFrameNum >= MAX_SUB_FRAME)
     {

         ulHarqSysFrameNum++;
         if (ulHarqSysFrameNum == MAX_SFN_VALUE)
         {
             ulHarqSysFrameNum = 0;
         }
     }
#endif

    /* SPR 5467 changes start*/
     UInt8 resAllocFailure = 0;
    /* SPR 5467 changes end*/

    ULSchUciRachPoolInfo* ulschUciRachInfo_p 
        /*CA Changes start  */
        = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
        /*CA Changes end  */

    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    /*CA Changes start  */
    tempPdcchULOutput_p = pdcchULOutputInfo_gp[internalCellIndex] + containerTick;
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick;
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];


    /* ICIC changes start */
    /*CA Changes start  */
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    /* Available PUSCH resources are fetched for each location type */
    tempPUSCHResources[CC_CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter; 
    tempPUSCHResources[CE_REGION] = 
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter; 
    /* ICIC changes end */


    LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "\n########\n containerTick = %d "
            " tempPdcchULOutput_p->pdcchULNackSpsUEInfo.adaptiveCount = %d \n"
        " tempPUSCHResources = %d  rbMapNum = %d\n"
            " numberOfULUEsToBeScheudled_p = %d\n########\n"
            ,containerTick, tempPdcchULOutput_p->pdcchULNackSpsUEInfo.nonAdaptiveCount, 
        tempPUSCHResources, rbMapNum,*numberOfULUEsToBeScheudled_p); /* SS_S2 */

   /* ICIC changes start */ 
    /* Adaptive Handling*/
    localCount = tempPdcchULOutput_p->pdcchULNackSpsUEInfo.adaptiveCount[locType];
    /* ICIC changes end */ 
    while(localCount--)
    {
        tempUeIndex =
            tempPdcchULOutput_p->pdcchULNackSpsUEInfo.pdcchULNackAdaptiveUEArray[locType][localCount];
        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];         
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, "\ntempUeIndex = %d",tempUeIndex);

        /* SPR 11548 Fix Start */
        /* resetting the resAllocFailure to FALSE */ 
        resAllocFailure = FALSE;
        /* SPR 11548 Fix End */

        if (!ulUEContextInfo_p->pendingDeleteFlag)
        {
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
            ulUEContext_p->isPdcchAllocated = 0;
            tempDirectIndexingUEInfo_p = \
                                         &(tempPdcchULOutput_p->directIndexingUEInfo[tempUeIndex]); 
            tempHarqProcessId = tempDirectIndexingUEInfo_p->harqIndex;
            /* MEAS_GAP_CHG */
            tempULHarqInfo_p = \
                               &(ulUEContext_p->ulresInfo[tempHarqProcessId]);
#ifdef ULHARQ_TIMER_PROC

        /* SPR 15909 fix start */
	    tickType_t             timerExpiredTTI     = 0;
        /* SPR 15909 fix end */
	    UInt32                 ulHarqSysFrameNum   = 0;
	    UInt32                 ulHarqSubFrameNum   = 0;
	    UInt8                  timerExpsubFrameNum = 0;   
	    UInt8 subFrameNum = (subFrame + ulDelay);

            /** For TTIB_Code */
	    if(tempULHarqInfo_p->isTTIBundled)
	    {
		    timerExpiredTTI     = currentGlobalTickWithULDelay  + TTIB_ULHARQ_TIMER_TTI_RETX_OFFSET;
		    timerExpsubFrameNum = subFrameNum + TTIB_ULHARQ_TIMER_TTI_RETX_OFFSET ;
	    }
	    else
	    {
		    timerExpiredTTI     = currentGlobalTickWithULDelay  + START_ULHARQ_TIMER_OFFSET;
		    timerExpsubFrameNum =  subFrameNum  + START_ULHARQ_TIMER_OFFSET;
	    }

        /*CA Changes start  */
	    getSFAndSFN(&ulHarqSysFrameNum, &ulHarqSubFrameNum, internalCellIndex);            
        /*CA Changes end  */

	    ulHarqSysFrameNum = MODULO_ONEZEROTWOFOUR(ulHarqSysFrameNum +  (timerExpsubFrameNum / MAX_NUM_OF_SUBFRAME));
	    timerExpsubFrameNum = timerExpsubFrameNum % MAX_NUM_OF_SUBFRAME;
#endif

            if (MEAS_GAP_RE_TX_NO_CCE_NO_GRANT ==
                    tempDirectIndexingUEInfo_p->flag)
            {
                lteWarning("should not come here: NO_CCE_NO_Grant + Adaptive");
#ifdef ULHARQ_TIMER_PROC
                ulHarqTimerStart(tempUeIndex, tempHarqProcessId,
                        timerExpsubFrameNum,
                        /* + SPS_TDD_Changes */
                        SPS_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        timerExpiredTTI,ulHarqSysFrameNum,
                        /*CA Changes start  */
                        internalCellIndex);
                        /*CA Changes end  */
#endif
                 tempULHarqInfo_p->currentRetransCount++;
                /* should numGrantsAllocated,stats be updated */                 
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;

                /* +COVERITY 10718 */
                /* SPR 20929 FIX Start */
                if ((tempDirectIndexingUEInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                        (INVALID_CCE_INDEX != tempDirectIndexingUEInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
                {
                    mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->
                            ulCCEIndex].dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->
                             ulCCEIndex]), internalCellIndex);
                }
                /* -COVERITY 10718 */
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                addEntryTOHIPduArray(
                        &ulUEContext_p->ulresInfo[tempHarqProcessId],
                        /* SPR 5467 changes start*/
                        HI_ACK,
                        /* SPR 5467 changes end*/
                        ulUEContext_p->crnti,
                        /* CR changes start */
                        ulUEContext_p,
                        tempULHarqInfo_p->sfUsed,                        
                        /* CR changes end */
                        tempHIPdu_p,
                        internalCellIndex);
                /* CR changes start */
                /*CA Changes start  */
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                    ~(ONE << tempULHarqInfo_p->nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                /*CA Changes end  */
                tempULHarqInfo_p->sfUsed = hiIndex;
                /* CR changes end */
                continue;
            }
            /* MEAS_GAP_CHG */

            /*
               We now create the HI PDU corresponding to the Nack received
               */
            addEntryTOHIPduArray(
                    tempULHarqInfo_p,
                    /* SPR 5467 changes start*/
                    HI_ACK,
                    /* SPR 5467 changes end*/
                    ulUEContext_p->crnti,
                    /* CR changes start */
                    ulUEContext_p,
                    tempULHarqInfo_p->sfUsed,
                    /* CR changes end */
                    tempHIPdu_p,
                    internalCellIndex);
            /* ICIC changes start */
            /* Incase of Cell Edge user, we can specify the region to be used as 
             * any of CC/CE region or 
             * CE region to be preferred first, if we want to 
             * allocate with cell edge resource first and then move to cell center if cell edge fails */
            if ((CE_USER == locType) &&  
               (tempPUSCHResources[CE_REGION_PREFERRED] >= tempULHarqInfo_p->riLength))
            {
                    regionAllocated = CE_REGION_PREFERRED; 
            }

            if (tempPUSCHResources[regionAllocated] >= tempULHarqInfo_p->riLength)
                /* ICIC changes end */
            {
                /* This will allocate resources based on the regionAllocated chosen */
                retVal = allocatePUSCHResourcesForAdaptiveReTx(
                        tempULHarqInfo_p,
                        tempULHarqInfo_p->riLength,
                        tempULHarqInfo_p->strictRBAllocation,
                        &resourceReserved,
                        ulUEContext_p->csiContext.mcsToPrbMap_p,
                        rbMapNum,
                        FALSE,
                        regionAllocated
                        ,ulDelay,
                        currentGlobalTick,
                        ulUEContext_p,
                        internalCellIndex
                        );
                /* This will get executed in case of cell edge user and
                 * if contiguous cell edge resources were less than riLength
                 */
                if((MAC_FAILURE == retVal)&& 
                        CE_REGION_PREFERRED == regionAllocated) 
                {
                    regionAllocated = CC_CE_ANY_REGION;
                    retVal = allocatePUSCHResourcesForAdaptiveReTx(
                            tempULHarqInfo_p,
                            tempULHarqInfo_p->riLength,
                            tempULHarqInfo_p->strictRBAllocation,
                            &resourceReserved,
                            ulUEContext_p->csiContext.mcsToPrbMap_p,
                            rbMapNum,
                            FALSE,
                            regionAllocated
                            ,ulDelay,
                            currentGlobalTick,
                            ulUEContext_p,
                            internalCellIndex
                            );

                }
           
            
                if(MAC_SUCCESS == retVal) 
                    /* SS_S2 */
                {
                    LOG_MAC_MSG(MAC_NACK_ADAPTIVE_RES_ALLOC,LOGDEBUG,MAC_PUSCH,\
                            currentGlobalTick,\
                            ulUEContext_p->userLocationType,\
                            regionAllocated,\
                            resourceReserved.allocatedIndex,\
                            resourceReserved.allocatedRiv,\
                            resourceReserved.allocatedLength,\
                            0.0,0.0,\
                            __func__,"MAC_NACK_ADAPTIVE_RES_ALLOC");

                    /* Time Averaging changes start */
                    powerCorrectionAndTpc = getTpcForPusch(ulUEContext_p, currentGlobalTickWithULDelay,internalCellIndex);
                    validateATB(tempULHarqInfo_p->riLength,\
                                 &tempULHarqInfo_p->mcsIndex,\
                                 ulUEContext_p, \
                                 powerCorrectionAndTpc.powerCorrectionVal, \
                                 &ueMaxPowerFlag,&ueMinPowerFlag, \
                                 ulUEContext_p->pcMax,RE_TX,internalCellIndex);
                    /* Reset the tpcTriggeredValue in UE Context */
                    ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue = 
                        TPC_NO_CHANGE;

                    if (powerCorrectionAndTpc.powerCorrectionVal)
                    {
                        ulUEContext_p->prevPowerCorrectionForPusch =
                            ulUEContext_p->currPowerCorrectionForPusch;

                        if (ueMaxPowerFlag == FALSE)
                        {
                            if (ulUEContext_p->dedicatedULPowerControlInfo.accumulationEnabled)
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
                        }
                        /* Store the TPC value in UE Context */
                        /* SPR 3531 START */
                        ulUEContext_p->powerControlPuschTPCValue =
                            powerCorrectionAndTpc.powerCorrectionVal;
                        /* SPR 3531 END */
                        CHECK_AND_UPDATE_POWER_CORRECTION_FOR_PUSCH(
                        ulUEContext_p->currPowerCorrectionForPusch);
                    }

#ifdef ULHARQ_TIMER_PROC
                    ulHarqTimerStart(tempUeIndex,tempHarqProcessId,timerExpsubFrameNum,
                            /* + SPS_TDD_Changes */
                            SPS_SCHEDULING,
                            /* - SPS_TDD_Changes */
                            timerExpiredTTI,ulHarqSysFrameNum,
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */
#endif 
                    /* + SPR 16758 */
                    /*We use 1 for Retransmission */ 
                    ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 1;
                    /* - SPR 16758 */
                    updateDciCCEInfoArray(
                            ulUEContext_p,
                            tempDirectIndexingUEInfo_p->ulCCEIndex,
                            tempHarqProcessId,
                            tempDciCCEInfo_p,
                            &resourceReserved,
                            powerCorrectionAndTpc.tpcCommand,
                            /* +- SPR 17777 */
                            /* CR changes start */
                            hiIndex, /* SS_S2 */
                            /* UL_MU_MIMO_CHG Start */  
                            tempDirectIndexingUEInfo_p->ulGrantRequestType,
                            INVALID_UE_INDEX,/*Coverity 96717 fix*/
                            /* UL_MU_MIMO_CHG Changes End */    
                            /* CR changes end */
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */

                    /* SPR 10027 FIX START */
                    /* Check if isDrbPresent = TRUE and Update the lastScheduledTick */
                    CHECK_AND_UPDATE_LAST_SCHEDULED_TICK(
                            tempULHarqInfo_p->isDrbPresent,ulUEContext_p)
                    /* SPR 10027 FIX END */

                    /* ICIC changes start */
                    tempPUSCHResources[regionAllocated] -= 
                        resourceReserved.allocatedLength;
                    if(CE_REGION_PREFERRED == regionAllocated)
                    {
                        tempPUSCHResources[CC_CE_REGION] -= resourceReserved.allocatedLength;
                        updateCCAvailRes(&rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                                          &resourceReserved);
                    }
                    else
                    {
                        /*CA Changes start  */
                        updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                                          tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                                          internalCellIndex);
                        /*CA Changes end  */
                        
                    }

                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_INFO,LOGDEBUG,MAC_UL_HARQ,\
                            getCurrentTick(),\
                            tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,\
                            tempPUSCHResources[CC_CE_REGION],\
                            tempPUSCHResources[CE_REGION],\
                            0,0.0,0.0,\
                            __func__,"NACK_RES_ALLOC");
                    /* ICIC changes end */
                    resourceReserved.allocatedLength = 0;
                    
                    LOG_MAC_MSG(MAC_NACK_RES_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                            currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                            rbMapNum,ADAPTIVE_RE_TX,
                            tempULHarqInfo_p->currentRetransCount,
                            tempULHarqInfo_p->riLength,tempULHarqInfo_p->riStart,
                            __func__,"NACK_RES_ULHARQ_PASS");

                    LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                            tempUeIndex,
                            currentGlobalTick,
                            ADAPTIVE_RE_TX,
                            tempHarqProcessId,
                            tempULHarqInfo_p->riLength,
                            tempULHarqInfo_p->riStart,
                            tempDirectIndexingUEInfo_p->ulGrantRequestType,
                            tempULHarqInfo_p->deltabsrLoad,
                            TRUE,
                            tempULHarqInfo_p->mcsIndex,
                            0,
                            powerCorrectionAndTpc.tpcCommand 
                            /* UL_MU_MIMO_CHG START */
                            , ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                            INVALID_UE_INDEX,
                            ulUEContext_p->userLocationType,
                            tempULHarqInfo_p->nDmrs,
                            ulUEContext_p->modulationScheme,
                            /* UL_MU_MIMO_CHG END */
                            /* CA Stats Changes Start */
                            internalCellIndex
                            /* CA Stats Changes End */
                                );
                    
                } 
                else
                {
                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_FAIL_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                            currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                            rbMapNum, 
                            tempULHarqInfo_p->nextReTransmissionType,
                            tempULHarqInfo_p->currentRetransCount,
                            0.0,0.0,__func__,"NACK_RES_ALLOC_FAIL");

                    /*
                       We now revert back the  cce allocated 
                       by PDCCH as the allocation was not done
                     */
                    /* SPR 5467 changes start*/
                    tempULHarqInfo_p->currentRetransCount++;
                    tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                    /* SPR 5467 changes end*/

                    /* +COVERITY 10718 */
                /* SPR 20929 FIX Start */
                if ((tempDirectIndexingUEInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                        (INVALID_CCE_INDEX != tempDirectIndexingUEInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
		    {
			    mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].\
					    dciPduInfo.aggregationLvl,
					    (&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex]),
                        internalCellIndex); 

			    /* SPR 5467 changes start*/
                            /* SPR 5557 changes start*/
                            /* SPR 5557 changes end*/
		    }
                    /* -COVERITY 10718 */
                    tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                    /* reserve the last given resources as UE will assume NON Adaptive
                       ReTx */
                    resAllocFailure = TRUE;
                    /* SPR 5467 changes end*/

                    /* CC resouces are updated always irrespective of user location,
                     * we check here whether the RBs lied in CE region and update the 
                     * resouce info if necessary*/
                    /*CA Changes start  */
                    ceOverlapRBs = updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                                       tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                                       internalCellIndex);
                    /*CA Changes end  */
                    if(0 < ceOverlapRBs) 
                    {
                        tempPUSCHResources[CE_REGION] -= ceOverlapRBs;
                    }
                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_INFO,LOGDEBUG,MAC_UL_HARQ,\
                            getCurrentTick(),\
                            tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,\
                            tempPUSCHResources[CC_CE_REGION],\
                            tempPUSCHResources[CE_REGION],\
                            0,0.0,0.0,\
                            __func__,"NACK_RES_ALLOC");
                    /* ICIC changes end */
#ifdef ULHARQ_TIMER_PROC
                    ulHarqTimerStart(tempUeIndex,tempHarqProcessId,timerExpsubFrameNum,
                            /* + SPS_TDD_Changes */
                            SPS_SCHEDULING,
                            /* - SPS_TDD_Changes */
                            timerExpiredTTI,ulHarqSysFrameNum,
                            /*CA Changes start  */
                            internalCellIndex);
                            /*CA Changes end  */
#endif 
                    /* CR changes start */
                    /*CA Changes start  */
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                        ~(ONE << tempULHarqInfo_p->nPhichSeq);
                    phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                    /*CA Changes end  */
                    tempULHarqInfo_p->sfUsed = hiIndex;
                    /* CR changes end */
                     /* SPR 5467 changes start*/
                          /* SPR 5467 changes end*/

                }
            }
            else
            {
                /*
                   We now revert back the  cce allocated by PDCCH 
                   as no more PUSCH resources are avaliable
                 */
                /* SPR 5467 changes start*/
                tempULHarqInfo_p->currentRetransCount++;
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                /* SPR 5467 changes end*/

                /* + COVERITY 10718 */
                /* SPR 20929 FIX Start */
                if ((tempDirectIndexingUEInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) && 
                        (INVALID_CCE_INDEX != tempDirectIndexingUEInfo_p->ulCCEIndex))/*CCE panic fix*/
                /* SPR 20929 FIX Stop */
		{
			mac_reset_cce(tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex].\
					dciPduInfo.aggregationLvl,
					(&tempDciCCEInfo_p[tempDirectIndexingUEInfo_p->ulCCEIndex]),
                    internalCellIndex); 
			/* SPR 5467 changes start*/
                        /* SPR 5557 changes start*/
                        /* SPR 5557 changes end*/
		}
                /* - COVERITY 10718 */
                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                resAllocFailure = TRUE;

                /* SPR 5467 changes end*/ 
                /* Required resources are not available, hence increase the current
                 * ReTx count and start the UL Timer */

#ifdef ULHARQ_TIMER_PROC
                ulHarqTimerStart(tempUeIndex,tempHarqProcessId,timerExpsubFrameNum,
                        /* + SPS_TDD_Changes */
                        SPS_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        timerExpiredTTI,ulHarqSysFrameNum,
                        /*CA Changes start  */
                        internalCellIndex);
                        /*CA Changes end  */
#endif 
                /* CR changes start */
                /*CA Changes start  */
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].bitMask &=
                    ~(ONE << tempULHarqInfo_p->nPhichSeq);
                phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][tempULHarqInfo_p->nPhichGrp].count++;                
                /*CA Changes end  */
                tempULHarqInfo_p->sfUsed = hiIndex;
                /* CR changes end */
                LOG_MAC_MSG(MAC_NACK_RES_ZERO_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        rbMapNum, 
                        tempULHarqInfo_p->nextReTransmissionType,
                        tempULHarqInfo_p->currentRetransCount,
                        0.0,0.0,__func__,"NACK_RES_ZERO_FAIL");
               
               /* SPR 5467 changes start*/
                /* SPR 5467 changes end */ 
                
            } 
            /* + SPR 16758 */
            /*We use 1 for Retransmission */ 
            ulUEContext_p->ulresInfo[tempHarqProcessId].newDataIndicator = 1;
            /* - SPR 16758 */

            /* SPR 5467 changes start*/
            if(resAllocFailure != TRUE)
            {
                (*numberOfULUEsToBeScheudled_p)--;
                 /* SPR 5467 changes end*/

                /* Semi static changes Start*/
                if(ULSCH_UCI_INVALID_INDEX != tempDirectIndexingUEInfo_p->ulschUciInfoIndex)
                {
                    /* Fill UL Harq Info*/
                    fillSemiStaticInfoForULSCH (ulschUciRachInfo_p,
                            tempDirectIndexingUEInfo_p,
                            ulUEContextInfo_p,
                            tempULHarqInfo_p,
                            tempHarqProcessId,
			    /* SPR 9173 changes start */
                            ulSubFrameNum,0, internalCellIndex);
			    /* SPR 9173 changes end */
                }
                else
                {
                    if ( MAX_ULSCH_UCI_SUPPORTED ==  numberOfInfoPresent)
                    {
                        ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                                __func__);
                        break;
                    }
                    /*SPR 7086 Fix Start*/
                    /* SPR 10195 Fix Start */
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS =
                        nSRSDetermineOnCollision(ulUEContext_p, tempHarqProcessId,ulSubFrameNum, internalCellIndex);
                    /* SPR 10195 Fix End */
                    /*SPR 7086 Fix End*/

                    /* Fill UL Harq Info*/
                    ulschUciRachInfo_p->
                        ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p = 
                        tempULHarqInfo_p;
                    ulschUciRachInfo_p->
                        ulschUciInfo[numberOfInfoPresent].ulHarqProcessId =
                        tempHarqProcessId;
                    ulschUciRachInfo_p->
                        ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_ULSCH;
                    /* + SPS_TDD_Changes */
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti =  ulUEContext_p->spsUlInfo.SpsCrnti;
                    /* - SPS_TDD_Changes */

                    numberOfInfoPresent++;

                    LOG_UT(MAC_UCI_ULSCH_PDU_ASSIGN,LOGDEBUG,MAC_PUSCH,\
                            getCurrentTick(),\
                            tempUeIndex,
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType,\
                            tempDirectIndexingUEInfo_p->ulschUciInfoIndex,\
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId,\
                            0,0.0,0.0,\
                            __func__,"ULSCH_PDU_ASSIGN");
                }
                /* Semi static changes End*/

                tempDirectIndexingUEInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                tempDirectIndexingUEInfo_p->flag = RECEIVED_NACK;
            }
        }    
        LTE_MAC_UT_LOG(LOG_INFO, LOG_PHICH, 
                "\ntempPUSCHResources = %d numberOfULUEsToBeScheudled_p = %d\n"
                ,tempPUSCHResources,*numberOfULUEsToBeScheudled_p);
    }

    /*Update Number of numberOfInfoPresent in the global context */ 
    ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;
}
/* + SPS_TDD_Changes */
#elif TDD_CONFIG
/******************************************************************************
 * Function Name    : processULNackSPSAdaptiveInPUSCH 
 * Inputs           : tempPdcchULOutput_p - The pointer to PdcchULOutputInfo,
 *                    rbMapNum,
 *                    subFrameNumber
 *                    currentGlobalTick - The present Global Tick,
 *                    numberOfULUEsToBeScheudled_p - Pointer to no. of UL UE's to be
 *                                                   scheduled,
 *                    containerTick - The Tick of the container being used,
 *                    locType
 * Outputs          : None
 * Returns          : None
 * Description      : This function will read the Nack Adaptive queue and update the 
 *                    HARQ process, and book the resources in advance. It will 
 *                    also create the HI PDUs. 
 * ******************************************************************************/
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
        /*CA Changes start */
        InternalCellIndex internalCellIndex,
        /*CA Changes end */
        /*TDD Config 0 Changes Start*/ 
        UInt8 ulSubFrameNum)
/*TDD Config 0 Changes End*/ 
{
    UInt16 tempUeIndex=0;
    HIPduArray* tempHIPdu_p  = PNULL;
    AllocatedResInfo resourceReserved = {0};
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; /* Invalid Value*/
    DciCCEContainerInfo* dciCCEContainerInfo_p = PNULL;
    DciCCEInfo* tempDciCCEInfo_p = PNULL;
    ULUEContext* ulUEContext_p = PNULL;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    UInt8 localCount = 0;
    ULHarqInfo* tempULHarqInfo_p            = PNULL;
    PdcchULNackUEInfo *pdcchULNackSpsUEInfo_p  = PNULL;
    TpcTriggeredToPowerCorrection powerCorrectionAndTpc = {0};
    /* ICIC changes start */
    /* Initialized by any region and changed in case of Cell edge user
     * as CE_REGION_PREFERRED if configured so */
    UInt8 regionAllocated = CC_CE_ANY_REGION;
    RbMapNode *rbMapNode_p = PNULL;
    UInt32 tempPUSCHResources[MAX_USER_LOCATION] = {0};
    MacRetType   retVal = MAC_FAILURE;
    UInt8 ceOverlapRBs = 0;
    /* ICIC changes end */
    /* TDD Config 6 Changes Start */
    UInt8 prevUlSubFrameNum  = 0;
    UInt16 prevUlSysFrameNum = 0;
    /* TDD Config 6 Changes End */
    /*TDD Config 0 Changes Start*/
    UInt8 subFrameNum = currentGlobalTick % MAX_SUB_FRAME;
    UInt8 ulSubFrameOffset =  getUlOffset(subFrameNum,ulSubFrameNum);
    UInt32 ulGlobalTick = currentGlobalTick + ulSubFrameOffset;
    UInt8 iPhich    = 0;
    UInt8 phichSubFrameNum = 0;
    /*TDD Config 0 Changes End*/
    UInt16 ulSysFrameNum = 0;

    UeScheduledInfo *ueScheduledInfo_p = PNULL;
    /*CA Changes start  */
    tempHIPdu_p = hiPduContainer_gp[internalCellIndex] + containerTick;
    dciCCEContainerInfo_p = dciCCEContainerInfo_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    tempDciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];

    UInt16 sysFrameNum = (currentGlobalTick / MAX_SUB_FRAME) % MAX_SFN_VALUE;
    /* Time Averaging changes start */
    UInt32 ueMaxPowerFlag= 0;
    UInt32 ueMinPowerFlag= 0;

    UInt16  numberOfInfoPresent = 0;
    /* +DYNAMIC_ICIC */
    ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + \
                                         ulPowerMaskIndex_g[internalCellIndex];
    SInt32 regionPowerMask = INVALID_UL_POWER_MASK;
    /* -DYNAMIC_ICIC */
    /*TDD Config 0 Changes Start*/
    ULSchUciRachPoolInfo* ulschUciRachInfo_p
        /*CA Changes start  */
        = ulSchUciRachContainer_gp[internalCellIndex] +(ulSubFrameNum % MAX_ULSCH_UCI_CONTAINER) ;
        /*CA Changes end  */
    /*TDD Config 0 Changes End*/
    /* FORCED_ACK SPR#9391 Changes Start */
    UInt8 resAllocFailure = 0;
    /* FORCED_ACK SPR#9391 Changes End */

    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;

    if (ulSubFrameNum < subFrameNumber)
    {
        ulSysFrameNum = MODULO_ONEZEROTWOFOUR((sysFrameNum + 1));
    }
    else
    {
        ulSysFrameNum = sysFrameNum;
    }
    /*TDD Config 0 Changes Start*/
    prevUlSubFrameNum = (ulGlobalTick - getprevUlSfOffsetFromCurrentUlSf(ulSubFrameNum,
                        /* CA TDD CHANGES START */
                        internalCellIndex
                        /* CA TDD CHANGES END */
                        )) % MAX_SUB_FRAME;
    GET_UL_SYS_FRAME_NUM_FROM_PHICH(prevUlSubFrameNum, subFrameNumber, prevUlSysFrameNum, sysFrameNum);
    /* TDD Config 0 Changes End */

    /* We now process the NACK Queue Load */


    LTE_MAC_UT_LOG( LOG_MAJOR, LOG_PHICH,
            "[%s] Entry rbMapNum[%d] subFrameNumber [%d] currentGlobalTick [%d] containerTick [%d] \n",
            __func__, rbMapNum, subFrameNumber, currentGlobalTick, containerTick);

    /* ICIC changes start */
    /*CA Changes start  */
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    /* Available PUSCH resources are fetched for each location type */
    tempPUSCHResources[CC_CE_REGION] =
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter;
    tempPUSCHResources[CE_REGION] =
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter;
    /* ICIC changes end */

     pdcchULNackSpsUEInfo_p = &(tempPdcchULOutput_p->pdcchULNackSpsUEInfo);
     localCount = pdcchULNackSpsUEInfo_p->adaptiveCount[locType];
    while(localCount--)
    {
        resAllocFailure = FALSE;
        tempUeIndex = \
                      pdcchULNackSpsUEInfo_p->pdcchULNackAdaptiveUEArray[locType][localCount];
        ulUEContextInfo_p = &ulUECtxInfoArr_g[tempUeIndex];
        if (!ulUEContextInfo_p->pendingDeleteFlag)
        {
            ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
            ulUEContext_p->isEntryPushedInAdvanceQueue = 0;
            ulUEContext_p->isPdcchAllocated = 0;

            if ( !isValidSubframeNumForDCI_0(subFrameNumber, internalCellIndex))
            {
                ltePanic("Invalid DL Subframe[%d] For DCI 0 \n",subFrameNumber);
            }

            /*SPR 13925 fix start*/ 
            /** SPR 14204 Fix : Start **/
            ueScheduledInfo_p = ulUEContext_p->ulUeScheduledInfoPointerArray[internalCellIndex][ulSubFrameNum];
            /** SPR 14204 Fix : End **/
            /*SPR 13925 fix end*/ 

            if ( !ueScheduledInfo_p )
            {
                ltePanic("[%s] ueScheduledInfo_p is null\n",
                        __func__);
                /* + Coverity 88282 */
                break; 
                /* - Coverity 88282 */
            }
            tempHarqProcessId = ueScheduledInfo_p->ulHarqProcessId;
            tempULHarqInfo_p = \
                               &(ulUEContext_p->ulresInfo[tempHarqProcessId]);
            /*TDD Config 0 Changes Start*/
            tempULHarqInfo_p->isPdcchAllocated = 0;
            /*TDD Config 0 Changes End*/
            if ((ueScheduledInfo_p->flag ==
                            MEAS_GAP_RE_TX_NO_CCE_NO_GRANT)
                        || (ueScheduledInfo_p->flag == ADAPTIVE_RETRANS_NO_CCE))
            {

                if (ULSCH_UCI_INVALID_INDEX !=
                        ueScheduledInfo_p->ulschUciInfoIndex)
                {
                    /* + SPR_14994 */
                    checkAndRestoreUCIPduType(ulUEContext_p, ueScheduledInfo_p, 
                            ulSysFrameNum, ulschUciRachInfo_p,ulSubFrameNum,
                            /*SPR 19504 fix start*/    
                            rbMapNum,
                            internalCellIndex);
                            /*SPR 19504 fix end*/    
                    /* - SPR_14994 */
                }


                /* should numGrantsAllocated,stats be updated */
                tempULHarqInfo_p->currentRetransCount++;
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                /* + SPR 16758 */
                tempULHarqInfo_p->newDataIndicator = 1;
                /* - SPR 16758 */

                ulHarqTimerStart(
                        subFrameNumber,
                        sysFrameNum,
                        tempUeIndex,
                        tempHarqProcessId,
                        /* + SPS_TDD_Changes */
                        SPS_SCHEDULING,
                        /* - SPS_TDD_Changes */
                        /*CA Changes start  */
                        internalCellIndex,
                        /*CA Changes end  */
                        /* TDD Config 0 Changes Start */
                        ulSubFrameNum
                        /* TDD Config 0 Changes End */
                        );

                if ((ueScheduledInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) &&
                        (INVALID_CCE_INDEX != ueScheduledInfo_p->ulCCEIndex))
                {
                    mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                            dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                            internalCellIndex);
                }
                ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;

                if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                {
                    addEntryTOHIPduArray(
                            &ulUEContext_p->ulresInfo[tempHarqProcessId],
                            HI_ACK, /*HI_NACK,*/
                            ulUEContext_p->crnti,
                            tempHIPdu_p,
                            internalCellIndex,
                            /* TDD Config 0 Changes Start */
                            prevUlSubFrameNum);
                        /* TDD Config 0 Changes End */
                    tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                }
                continue;

            }
            /* ICIC changes start */
            /* Incase of Cell Edge user, we can specify the region to be used as
             * any of CC/CE region or
             * CE region to be preferred first, if we want to
             * allocate with cell edge resource first and then move to cell center if cell edge fails */
            if ((CE_USER == locType) &&
                    (tempPUSCHResources[CE_REGION_PREFERRED] >= tempULHarqInfo_p->riLength))
            {
                regionAllocated = CE_REGION_PREFERRED;
            }
            if (tempPUSCHResources[regionAllocated] >= tempULHarqInfo_p->riLength)
            {
                /* This will allocate resources based on the regionAllocated chosen */
                retVal = allocatePUSCHResourcesForAdaptiveReTx(
                        tempULHarqInfo_p,
                        tempULHarqInfo_p->riLength,
                        tempULHarqInfo_p->strictRBAllocation,
                        &resourceReserved,
                        ulUEContext_p->csiContext.mcsToPrbMap_p,
                        rbMapNum,
                        FALSE,
                        regionAllocated,
                        internalCellIndex,
                        /*TDD Config 0 Changes Start*/ 
                        ulSubFrameNum
                        );
                        /*TDD Config 0 Changes End*/ 
                /* This will get executed in case of cell edge user and
                 * if contiguous cell edge resources were less than riLength
                 */
                if((MAC_FAILURE == retVal)&&
                        CE_REGION_PREFERRED == regionAllocated)
                {
                    regionAllocated = CC_CE_ANY_REGION;
                    retVal = allocatePUSCHResourcesForAdaptiveReTx(
                            tempULHarqInfo_p,
                            tempULHarqInfo_p->riLength,
                            tempULHarqInfo_p->strictRBAllocation,
                            &resourceReserved,
                            ulUEContext_p->csiContext.mcsToPrbMap_p,
                            rbMapNum,
                            FALSE,
                            regionAllocated,
                            internalCellIndex,
                            /*TDD Config 0 Changes Start*/ 
                            ulSubFrameNum
                            );
                            /*TDD Config 0 Changes End*/
                }
                if(MAC_SUCCESS == retVal)
                {
                    LOG_MAC_MSG(MAC_NACK_ADAPTIVE_RES_ALLOC,LOGDEBUG,MAC_PUSCH,\
                            currentGlobalTick,\
                            ulUEContext_p->userLocationType,\
                            regionAllocated,\
                            resourceReserved.allocatedIndex,\
                            resourceReserved.allocatedRiv,\
                            resourceReserved.allocatedLength,\
                            0.0,0.0,\
                            __func__,"MAC_NACK_ADAPTIVE_RES_ALLOC");

                    /* Time Averaging changes start */
                    powerCorrectionAndTpc = getTpcForPusch(ulUEContext_p, currentGlobalTick, internalCellIndex,
                                                           /*TDD Config 0 Changes Start*/ 
                                                           ulSubFrameNum);
                                                           /*TDD Config 0 Changes End*/ 

                    /* +DYNAMIC_ICIC */
                    regionPowerMask = ulPowerMaskInfo_p->powerMask[ulUEContext_p->userLocationType];
                    CHECK_QCI_POWER_MASK_AND_UPDATE_REGION_MASK(ulUEContext_p,regionPowerMask);

                    validateATB(tempULHarqInfo_p->riLength,\
                            &tempULHarqInfo_p->mcsIndex,\
                            ulUEContext_p, \
                            powerCorrectionAndTpc.powerCorrectionVal, \
                            &ueMaxPowerFlag,&ueMinPowerFlag, \
                            regionPowerMask,RE_TX, internalCellIndex);
                    /* -DYNAMIC_ICIC */
                    /* SPR 10753 Changes Start */
                    tempULHarqInfo_p->forceAckNextRetxType = NON_ADAPTIVE_RETRANS_AFTER_HI_ACK;
                    /* SPR 10753 Changes End */

                    /* Reset the tpcTriggeredValue in UE Context */
                    ulUEContext_p->tpcInfoForPusch.tpcTriggeredValue =
                        TPC_NO_CHANGE;

                    if (powerCorrectionAndTpc.powerCorrectionVal)
                    {
                        ulUEContext_p->prevPowerCorrectionForPusch =
                            ulUEContext_p->currPowerCorrectionForPusch;

                        checkAndUpdateFiCorrection(
                                ueMaxPowerFlag,
                                ueMinPowerFlag,
                                powerCorrectionAndTpc,
                                ulUEContext_p);


                    }
                    /* + SPR 16758 */
                    /*We use 1 for Retransmission */
                    /* rlf_fix start*/
                    tempULHarqInfo_p->newDataIndicator = 0;
                    /* rlf_fix end*/
                    /* - SPR 16758 */

                    /*TDD Config 0 Changes Start*/
                    /* Block the phich resources for the given ristart and
                     * ndmrs*/
                    phichSubFrameNum = ulSubFrameNum + getHiPduSubframeNum(ulSubFrameNum,internalCellIndex);
                    phichSubFrameNum = phichSubFrameNum % MAX_SUB_FRAME;
                    iPhich = calculateIPhichValue(ulSubFrameNum,internalCellIndex);
                    /*Reserve the HI resources and update the harq Info*/
                    reserveHiAndUpdateHarqInfo(iPhich,
                                               tempULHarqInfo_p->riStart,
                                               tempULHarqInfo_p->nDmrs,
                                               phichSubFrameNum,
                                               tempULHarqInfo_p,
                                               internalCellIndex);
                    /* TDD Config 0 Changes End */

                    updateDciCCEInfoArray(
                            ulUEContext_p,
                            ueScheduledInfo_p->ulCCEIndex,
                            tempHarqProcessId,
                            tempDciCCEInfo_p,
                            &resourceReserved,
                            powerCorrectionAndTpc.tpcCommand,
                            /* +- SPR 17777 */
                            /* UL_MU_MIMO_CHG Start */
                            INVALID_UE_INDEX,
                            /* UL_MU_MIMO_CHG End */
                            //SPR_16896_Changes
                            TRUE,
                            //SPR_16896_Changes
                            /*CA Changes start  */
                            internalCellIndex,
                            /* +- SPR 17777 */
                            currentGlobalTick,
                            /* TDD Config 0 Changes Start */
                            ulSubFrameNum,
                            /*Scheduled count is applicable for New UEs only*/
                            INVALID_COUNT);
                            /* TDD Config 0 Changes End */

                    /* Check if isDrbPresent = TRUE and Update the lastScheduledTick */
                    CHECK_AND_UPDATE_LAST_SCHEDULED_TICK(
                            tempULHarqInfo_p->isDrbPresent,ulUEContext_p)

                        /* ICIC changes start */
                        tempPUSCHResources[regionAllocated] -=
                        resourceReserved.allocatedLength;
                    if(CE_REGION_PREFERRED == regionAllocated)
                    {
                        tempPUSCHResources[CC_CE_REGION] -= resourceReserved.allocatedLength;
                        updateCCAvailRes(&rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                                &resourceReserved);
                    }
                    else
                    {
                        /*CA Changes start  */
                        ceOverlapRBs = updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                                tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength,
                                internalCellIndex);
                        /*CA Changes end  */
                        tempPUSCHResources[CE_REGION] -= ceOverlapRBs;


                    }

                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_INFO,LOGDEBUG,MAC_UL_HARQ,\
                            getCurrentTick(),\
                            tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,\
                            tempPUSCHResources[CC_CE_REGION],\
                            tempPUSCHResources[CE_REGION],\
                            0,0.0,0.0,\
                            __func__,"NACK_RES_ALLOC");
                    /* ICIC changes end */
                    resourceReserved.allocatedLength = 0;

                    LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                            tempUeIndex,
                            currentGlobalTick,
                            ADAPTIVE_RE_TX,
                            tempHarqProcessId,
                            tempULHarqInfo_p->riLength,
                            tempULHarqInfo_p->riStart,
                            ueScheduledInfo_p->ulGrantRequestType,
                            tempULHarqInfo_p->deltabsrLoad,
                            TRUE,
                            tempULHarqInfo_p->mcsIndex,
                            0,
                            powerCorrectionAndTpc.tpcCommand
                            /* UL_MU_MIMO_CHG START */
                            , ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                            INVALID_UE_INDEX,
                            ulUEContext_p->userLocationType,
                            tempULHarqInfo_p->nDmrs,
                            ulUEContext_p->modulationScheme,
                            /* UL_MU_MIMO_CHG END */
                            /* CA Stats Changes Start */
                            internalCellIndex
                            /* CA Stats Changes End */
                            );

                }
                else
                {
                    if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                    {
                        addEntryTOHIPduArray(
                                tempULHarqInfo_p,
                                /* FORCED_ACK SPR#9391 Changes Start */
                                HI_ACK,/*HI_NACK,*/
                                /* FORCED_ACK SPR#9391 Changes End */
                                ulUEContext_p->crnti,
                                tempHIPdu_p,
                                internalCellIndex,
                                /* TDD Config 0 Changes Start */
                                prevUlSubFrameNum);
                        /* TDD Config 0 Changes End */
                        tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                    }

                    if (ULSCH_UCI_INVALID_INDEX !=
                            ueScheduledInfo_p->ulschUciInfoIndex)
                    {
                        /* + SPR_14994 */
                        checkAndRestoreUCIPduType(ulUEContext_p, ueScheduledInfo_p,
                                ulSysFrameNum, ulschUciRachInfo_p, ulSubFrameNum,
                                /*SPR 19504 fix start*/    
                                rbMapNum,
                                internalCellIndex);
                                /*SPR 19504 fix end*/    
                        /* - SPR_14994 */
                    }

                    /* FORCED_ACK SPR#9391 Changes Start */
                    tempULHarqInfo_p->currentRetransCount++;
                    tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;
                    /* FORCED_ACK SPR#9391 Changes End */

                    /* SPR 10753 Changes Start */
                    tempULHarqInfo_p->forceAckNextRetxType = ADAPTIVE_RETRANS_AFTER_HI_ACK;
                    /* SPR 10753 Changes End */

                    /* required resources are not available, hence increment the
                     * current ReTx count and start UL Timer*/
                /* SPR 20929 FIX Start */
                    if ((ueScheduledInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) &&
                            (INVALID_CCE_INDEX != ueScheduledInfo_p->ulCCEIndex))
                /* SPR 20929 FIX Stop */
                    {
                        mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                                dciPduInfo.aggregationLvl,
                                (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                                internalCellIndex);
                    }

                    /* FORCED_ACK SPR#9391 Changes Start */
                    ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                    resAllocFailure = TRUE;
                    /* FORCED_ACK SPR#9391 Changes End */

                    /* CC resouces are updated always irrespective of user location,
                     * we check here whether the RBs lied in CE region and update the
                     * resouce info if necessary*/
                    /*ceOverlapRBs = updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                            tempULHarqInfo_p->riStart, tempULHarqInfo_p->riLength);*/
                    /*if(0 < ceOverlapRBs)
                    {
                        tempPUSCHResources[CE_REGION] -= ceOverlapRBs;
                    }*/
                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_INFO,LOGDEBUG,MAC_UL_HARQ,\
                            getCurrentTick(),\
                            tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->riLength,\
                            tempPUSCHResources[CC_CE_REGION],\
                            tempPUSCHResources[CE_REGION],\
                            0,0.0,0.0,\
                            __func__,"NACK_RES_ALLOC");
                    /* ICIC changes end */

                    LOG_MAC_MSG(MAC_NACK_RES_ALLOC_FAIL_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                            currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                            rbMapNum,
                            tempULHarqInfo_p->nextReTransmissionType,
                            tempULHarqInfo_p->currentRetransCount,
                            0.0,0.0,__func__,"NACK_RES_ALLOC_FAIL");

                }
            }
            else
            {
                if (tempULHarqInfo_p->forcedAckFlag != FORCED_ACK_SENT)
                {
                    addEntryTOHIPduArray(
                            tempULHarqInfo_p,
                            /* FORCED_ACK SPR#9391 Changes Start */
                            HI_ACK,/*HI_NACK,*/
                            /* FORCED_ACK SPR#9391 Changes End */
                            ulUEContext_p->crnti,
                            tempHIPdu_p,
                           internalCellIndex,
                           /* TDD Config 0 Changes Start */
                           prevUlSubFrameNum);
                    tempULHarqInfo_p->forcedAckFlag = FORCED_ACK_SENT;
                }
                /* required resources are not available, hence increment the
                 * current ReTx count and start UL Timer*/
                tempULHarqInfo_p->currentRetransCount++;
                tempULHarqInfo_p->nextReTransmissionType = ADAPTIVE_RE_TX;

                /* SPR 20929 FIX Start */
                if ((ueScheduledInfo_p->ulCCEIndex != CCE_NOT_ALLOCATED) &&
                /* SPR 20929 FIX Stop */
                        (INVALID_CCE_INDEX != ueScheduledInfo_p->ulCCEIndex))
                {
                    mac_reset_cce(tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex].\
                            dciPduInfo.aggregationLvl,
                            (&tempDciCCEInfo_p[ueScheduledInfo_p->ulCCEIndex]),
                            internalCellIndex);
                }

                /* FORCED_ACK SPR#9391 Changes Start */
                ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                resAllocFailure = TRUE;
                /* FORCED_ACK SPR#9391 Changes End */
                LOG_MAC_MSG(MAC_NACK_RES_ZERO_ULHARQ_ID,LOGINFO,MAC_UL_HARQ,\
                        currentGlobalTick,tempUeIndex,tempHarqProcessId, \
                        rbMapNum,
                        tempULHarqInfo_p->nextReTransmissionType,
                        tempULHarqInfo_p->currentRetransCount,
                        0.0,0.0,__func__,"NACK_RES_ZERO_FAIL");
                LTE_MAC_UPDATE_STATS_PUSCH_PER_TTI(
                        tempUeIndex,
                        currentGlobalTick,
                        NON_ADAPTIVE_RE_TX,
                        tempHarqProcessId,
                        tempULHarqInfo_p->riLength,
                        tempULHarqInfo_p->riStart,
                        ueScheduledInfo_p->ulGrantRequestType,
                        tempULHarqInfo_p->deltabsrLoad,
                        FALSE,
                        tempULHarqInfo_p->mcsIndex,
                        0,
                        TPC_VALUE_1
                        /* UL_MU_MIMO_CHG START */
                        , ulUEContext_p->ulMUMIMOInfo.ulMUMIMOStatus,
                        INVALID_UE_INDEX,
                        ulUEContext_p->userLocationType,
                        tempULHarqInfo_p->nDmrs,
                        ulUEContext_p->modulationScheme,
                        /* UL_MU_MIMO_CHG END */
                        /* CA Stats Changes Start */
                        internalCellIndex
                        /* CA Stats Changes End */
                            );
            }
            /* + SPR 16758 */
            /*We use 1 for Retransmission */
            tempULHarqInfo_p->newDataIndicator = 1;
            /* - SPR 16758 */
            
            /* TDD Config 6 Changes Start */
            resetHarqId(prevUlSubFrameNum, ulUEContext_p, internalCellIndex,prevUlSysFrameNum);
            setHarqBusy(ulSubFrameNum, tempHarqProcessId, ulUEContext_p, internalCellIndex,ulSysFrameNum);
            /* TDD Config 6 Changes End */

            ulHarqTimerStart( subFrameNumber,
                    sysFrameNum,
                    tempUeIndex,
                    tempHarqProcessId,
                    /* + SPS_TDD_Changes */
                    SPS_SCHEDULING,
                    /* - SPS_TDD_Changes */
                    /*CA Changes start  */
                    internalCellIndex,
                    /*CA Changes end  */
                    /* TDD Config 0 Changes Start */
                    ulSubFrameNum);
                    /* TDD Config 0 Changes End */

            /* FORCED_ACK SPR#9391 Changes Start */
            if(resAllocFailure != TRUE)
            {
                /* FORCED_ACK SPR#9391 Changes End */
                (*numberOfULUEsToBeScheudled_p)--;

                /* Semi static changes Start*/
                if(ULSCH_UCI_INVALID_INDEX != ueScheduledInfo_p->ulschUciInfoIndex)
                {
                    /* Fill UL Harq Info*/
                    fillSemiStaticInfoForULSCH (ulschUciRachInfo_p,
                            ueScheduledInfo_p,
                            ulUEContextInfo_p,
                            tempULHarqInfo_p,
                            tempHarqProcessId,
                            ulSysFrameNum,
                            ulSubFrameNum,0, internalCellIndex);
                    ueScheduledInfo_p->ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
                }
                else
                {
                    if ( MAX_ULSCH_UCI_SUPPORTED ==  numberOfInfoPresent)
                    {
                        ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                                __func__);
                        break;
                    }
                    /* SPR 10195 Fix Start */
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].nSRS =
                        nSRSDetermineOnCollision(ulUEContext_p, tempHarqProcessId, ulSubFrameNum, internalCellIndex);
                    /* SPR 10195 Fix End */

                    ulschUciRachInfo_p->
                        ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p =
                        tempULHarqInfo_p;
                    ulschUciRachInfo_p->
                        ulschUciInfo[numberOfInfoPresent].ulHarqProcessId =
                        tempHarqProcessId;

                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                        pduType = MAC_SS_PDU_ULSCH;

                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti =
                        ulUEContext_p->spsUlInfo.SpsCrnti;

                    /*TDD Config 0 Changes Start*/
                    /*Populate ulschUciInfoIndex in ueScheduledInfo_p and
                     *ueScheduledInfo_p in ulschUciRachInfo_p.This info is
                     * retrieved in PUCCH for TDD_CONFIG0*/
                    ueScheduledInfo_p->ulschUciInfoIndex = numberOfInfoPresent;
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ueScheduledInfo_p = ueScheduledInfo_p;
                    /*TDD Config 0 Changes End*/

                    LOG_UT(MAC_UCI_ULSCH_PDU_ASSIGN,LOGDEBUG,MAC_PUSCH,\
                            getCurrentTick(),\
                            tempUeIndex,
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType,\
                            ueScheduledInfo_p->ulschUciInfoIndex,\
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ulHarqProcessId,\
                            0,0.0,0.0,\
                            __func__,"ULSCH_PDU_ASSIGN");
                    numberOfInfoPresent++;
                }
                /* Semi static changes End*/
                ueScheduledInfo_p->ulCCEIndex = INVALID_CCE_INDEX;
                ueScheduledInfo_p->flag = RECEIVED_NACK;
                /* FORCED_ACK SPR#9391 Changes Start */
            }
            /* FORCED_ACK SPR#9391 Changes End */
        }

    }
    ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;

}

/* - SPS_TDD_Changes */
#endif
/******************************************************************************
* Function Name :   checkPhichGrpAndPhichSeq
* Inputs        :   riStart - Start PRB Index,
*                   hiIndex
*                   internalCellIndex - Cell-Index used at MAC
* Outputs       :   None
* Returns       :   LTE_TRUE/LTE_FALSE
* Description   :   This function will provide the nPhichGroup adn nPhichSeq
*                   value for the given riStart.
*******************************************************************************/
UInt8 checkPhichGrpAndPhichSeq(UInt8 riStart,
                               UInt8 hiIndex,
                               /*CA Changes start  */
                               InternalCellIndex internalCellIndex)
                               /*CA Changes end  */
{
    UInt8 nDmrs= 0;
    UInt8 nPhichGrp = 0;
    UInt8 nPhichSeq = 0;
#ifdef TDD_CONFIG
    UInt8 iPhich = 0;
#endif

    /*Coverity_Warning_Fix_Start*/
    if(riStart >=  MAX_NUM_RBS)
        return LTE_FALSE;
    /*Coverity_Warning_Fix_End*/   

#ifdef FDD_CONFIG
    nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][riStart][nDmrs];
#elif TDD_CONFIG
    nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][riStart][nDmrs];
#endif    

    /*CA Changes start  */
    nPhichSeq = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][riStart][nDmrs];
    /*CA Changes end  */
        
    /* CR changes start */
#ifdef FDD_CONFIG
    /*CA Changes start  */
    if(phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp].bitMask
                & (ONE << nPhichSeq))
    /*CA Changes end  */
     /* CR changes end */
    {
        return LTE_TRUE ;
    }            
#elif TDD_CONFIG

    if(phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp]
                & (ONE << nPhichSeq))
    {
        return LTE_TRUE ;
    }
#endif
    else
    {
        return LTE_FALSE;
    }
}
/******************************************************************************
* Function Name :   markPhichGrpAndPhichSeq 
* Inputs        :   riStart - Start PRB Index,
*                   hiIndex
*                   internalCellIndex - Cell-Index used at MAC
* Outputs       :   None
* Returns       :   TRUE
* Description   :   This function will provide the nPhichGroup adn nPhichSeq
*                   value for the given riStart.
*******************************************************************************/
UInt8 markPhichGrpAndPhichSeq(UInt8 riStart,
                              UInt8 hiIndex,
                              /*CA Changes start  */
                              InternalCellIndex internalCellIndex)
                              /*CA Changes end  */
{
    UInt8 nDmrs= 0;
    UInt8 nPhichGrp = 0;
    UInt8 nPhichSeq = 0;

#ifdef TDD_CONFIG
    UInt8 iPhich = 0;
#endif
#ifdef FDD_CONFIG
    nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][riStart][nDmrs];
#elif TDD_CONFIG
    nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][riStart][nDmrs];
#endif    
    /*CA Changes start  */
    nPhichSeq = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][riStart][nDmrs];
    /*CA Changes end  */
        
    /* CR changes start */
#ifdef FDD_CONFIG
    /*CA Changes start  */
    if(phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp].bitMask
                & (ONE << nPhichSeq))
    {
        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp].bitMask &=
                ~(ONE << nPhichSeq);
        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp].count++;
      
    }
    /*CA Changes end  */
#elif TDD_CONFIG
    if(phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp]
                & (ONE << nPhichSeq))
    {

        phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp] &=
            ~(ONE << nPhichSeq);
    }

#endif

    
    return TRUE ;
}
/******************************************************************************
* Function Name :   getSPSPhichGrpAndPhichSeq
* Inputs        :   riStart
*                   nPhichGrp_p,
*                   nPhichSeq_p,
*                   sfUsed_p,
*                   hiIndex,
*                   internalCellIndex - Cell-Index used at MAC
* Outputs       :   nDmrs_p - nDmrs that is returned back
* Returns       :   LTE_TRUE
* Description   :   This function will provide the nPhichGroup adn nPhichSeq
*                   value for the given riStart.
*******************************************************************************/
UInt8 getSPSPhichGrpAndPhichSeq(UInt8 riStart,
                             UInt8 *nPhichGrp_p,
                             UInt8 *nPhichSeq_p,
                             UInt8 *sfUsed_p,
                             UInt8 *nDmrs_p,
                             UInt8 hiIndex,
                             /*CA Changes start */
                             InternalCellIndex internalCellIndex)
                             /*CA Changes end */
{
    /* Coverity fix of 54499*/

#ifdef TDD_CONFIG
    UInt8 iPhich = 0;
    *nPhichGrp_p = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][riStart][0];
#else
    *nPhichGrp_p = phichGroupFromPrbDmrsMap_g[internalCellIndex][riStart][0];
#endif
    /*CA Changes start  */
    *nPhichSeq_p = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][riStart][0];
    /*CA Changes end  */
    *sfUsed_p = hiIndex;
    *nDmrs_p = 0;


    /* Coverity fix of 54499*/
    return LTE_TRUE ;
}


/*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
/******************************************************************************
 ** Function Name    : checkAvailabilityOfPhichGrpAndPhichSeq
 ** Inputs           : riStart - Start PRB Index,
 **                    hiIndex,iphich
 ** Outputs          : nDmrs_p - nDmrs that is returned back
 ** Description      : This function will provide the ndmrs for which phichgrp and 
                       phichseq are available for the given riStart.
 ********************************************************************************/
MacRetType checkAvailabilityOfPhichGrpAndPhichSeq(UInt8 riStart,
        UInt8 *nDmrs_p,
        UInt8 hiIndex,
        UInt8 iPhich,
        /*CA Changes start */                 
        InternalCellIndex internalCellIndex)  
        /*CA Changes end */                   
{
    UInt8 nDmrs= 0;
    UInt8 nPhichGrp = 0;
    UInt8 nPhichSeq = 0;
    
    /*Loop from 0 to maxNdmrs value to find the available phichgrp and phichseq
     * for the given ristart*/
    while(nDmrs < MAX_DMRS_VALUE)
    {
        nPhichGrp = phichGroupFromPrbDmrsMap_g[internalCellIndex][iPhich][riStart][nDmrs];
        nPhichSeq = phichSequenceNoFromPrbDmrsMap_g[internalCellIndex][riStart][nDmrs];
        if(phichSequenceNoPerPhichGroup_g[internalCellIndex][hiIndex][nPhichGrp]
                & (ONE << nPhichSeq))
        {
            *nDmrs_p = nDmrs;
            return MAC_SUCCESS;
        }
        else
        {
            nDmrs++;
        }
    }
    return MAC_FAILURE;
}
#endif
/*TDD Config 0 Changes End*/
