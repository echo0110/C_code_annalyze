/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacEncoder.c,v 1.3.6.1.6.2 2010/10/11 12:57:01 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the execution code necessary for
 *                     Transport block encoding.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacEncoder.c,v $
 *
 * Revision 1.3.6.1.6.3  2012/12/03 12:57:01  gur31292
 * Changes done for Performance Events
 *
 * Revision 1.3.6.1.6.2  2010/10/11 12:57:01  gur10121
 * SPR #706 fix
 *
 * Revision 1.3.6.1.6.1.4.1  2010/10/08 07:42:44  gur10121
 *  SPR 706 Fix - KlockWork Warnings removed
 *
 * Revision 1.3.6.1.6.1  2010/09/21 15:46:01  gur20491
 * FAPI changes
 *
 * Revision 1.3.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.50.24.1  2010/07/21 09:13:14  gur24420
 * Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
 * Overall Changes Cover:-
 * 1. rlcErrorDataInd() function call commented to avoid integrated compilations
 *
 * Revision 1.50  2010/03/18 05:56:05  gur18550
 * Mimo Second TB Bug
 *
 * Revision 1.49  2010/02/24 09:03:24  gur18550
 * MIMO case bug Fixed
 *
 * Revision 1.48  2009/11/14 14:30:47  gur11912
 * removed RESET_CCE in case of retransmission
 *
 * Revision 1.47  2009/10/09 10:56:38  gur11912
 * optimization changes
 *
 * Revision 1.46  2009/10/09 04:59:17  gur11912
 * created MemPool for encoderQueueNode and TBs
 *
 * Revision 1.45  2009/09/24 17:38:27  gur11912
 * modified ONLY_TA case
 *
 * Revision 1.44  2009/09/23 04:42:13  gur11912
 * optimized for harqSubFrameNum
 *
 * Revision 1.43  2009/09/16 09:50:22  gur11912
 * corrected throughput
 *
 * Revision 1.42  2009/08/30 13:49:16  gur19413
 * commented the unused variable.
 *
 * Revision 1.41  2009/08/11 09:46:27  gur11912
 * modified calculation of formed TBSize in case of retransmission
 *
 * Revision 1.40  2009/08/11 05:40:19  gur11912
 * Only data portion is copied in createTBFromHarqProcessData
 *
 * Revision 1.39  2009/08/04 13:33:55  gur11912
 * Used tb2StartOffset instead of harqProcessTBTwo_p->tbStartOffset
 *
 * Revision 1.38  2009/07/31 09:06:59  gur18569
 * replaced lteMacRlcInterface.h with lteLayer2MacRlcInterface.h
 *
 * Revision 1.37  2009/07/29 14:29:40  gur11912
 * removed updation of assignedRBQueueLoad
 *
 * Revision 1.36  2009/07/29 11:51:44  gur11912
 * Handled remaining retransmission nodes if dummy node found
 *
 * Revision 1.35  2009/07/28 14:30:50  gur11912
 * Updated for HARQ Timer start in case of retransmission
 *
 * Revision 1.34  2009/07/23 14:03:27  gur18569
 *  UT fixes
 *
 * Revision 1.33  2009/07/23 07:12:22  gur18569
 * replaced resetCCEInfo with RESET_CCE and changed in resetHarqState logic
 *
 * Revision 1.32  2009/07/22 04:31:27  gur18569
 * removed warning
 *
 * Revision 1.31  2009/07/21 14:28:47  gur18569
 * added UT_TESTING logs and fixed UT bugs
 *
 * Revision 1.30  2009/07/16 12:30:12  gur18569
 * added logs
 *
 * Revision 1.29  2009/07/15 15:09:59  gur18569
 * fixed UT bug and added some test code
 *
 * Revision 1.28  2009/07/14 08:03:10  gur18569
 * added throughput calculation changes
 *
 * Revision 1.27  2009/07/13 08:53:40  gur18569
 *
 * Revision 1.26  2009/07/13 07:24:17  gur18569
 * replaced prefix string ENCODER with MAC_MOD_ENCODER macro
 *
 * Revision 1.25  2009/07/10 10:56:09  gur18569
 * incorporated review comments
 *
 * Revision 1.24  2009/07/08 11:00:51  gur18569
 * review comment incorporation
 *
 * Revision 1.23  2009/07/07 15:14:58  gur18569
 * incorporated code review changes
 *
 * Revision 1.22  2009/07/02 16:04:48  gur18569
 * fixed UT bugs
 *
 * Revision 1.21  2009/07/02 10:28:25  gur18569
 * added an argument ueIndex to putEntryInEncoderQueue
 *
 * Revision 1.20  2009/07/02 05:28:49  gur18569
 * fixec UT bugs
 *
 * Revision 1.17  2009/06/24 13:53:51  gur18569
 * doing queueCount-- while process queue from sched
 *
 * Revision 1.16  2009/06/19 12:50:03  gur18569
 * assigning cceIndex in dciPdu
 *
 * Revision 1.15  2009/06/18 07:48:22  gur18569
 * using NEW_TX inplace of NEW_TRANSMISSION , similarly for RE_TX
 *
 * Revision 1.14  2009/06/16 12:22:31  gur18569
 * changes in putEntryInEncoderNode signature
 *
 * Revision 1.13  2009/06/08 14:24:35  gur18569
 *  replaced numEncoderThreads_g with NUM_OF_ENCODER_THREADS
 *
 * Revision 1.12  2009/06/08 13:37:26  gur18569
 * removed compilation error
 *
 * Revision 1.11  2009/06/08 05:04:20  gur18569
 * settind pending TA and pending DRX after they are sent
 *
 * Revision 1.10  2009/06/03 07:05:57  gur18569
 *  changes regarding DATA_MSG_API_LENGTH
 *
 * Revision 1.9  2009/06/01 05:52:02  gur18569
 * added functionality of writing into tx queues
 *
 * Revision 1.8  2009/05/28 11:57:46  gur18569
 * removed compilation errors
 *
 * Revision 1.7  2009/05/19 16:05:41  gur18569
 * incorporated review comments
 *
 * Revision 1.6  2009/05/19 11:25:27  gur18569
 * before code review
 *
 * Revision 1.5  2009/05/15 04:52:57  gur18569
 * added code for testing under macro UT_TESTING
 *
 * Revision 1.4  2009/05/14 13:25:01  gur18569
 * added more function definitions
 *
 * Revision 1.3  2009/05/10 13:17:07  gur18569
 * added function definitions
 *
 * Revision 1.2  2009/05/10 13:07:34  gur18569
 * added functions definitions
 *
 * Revision 1.1  2009/05/05 04:40:57  gur18569
 * Initial version (kenobi@gur18569)
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteShmLogger.h"
#include "lteMacEncoder.h"
#include "lteMacTransmitter.h"
#include "lteMacSFNMgr.h"
#include "lteMacExecutionLegs.h"
#include "lteMacDLHarqMgr.h"
#include "lteMacParseOAMMsg.h"
#include "lteLayer2MacRlcInterface.h"
#include "lteMacUtilityAndSharedStructs.h"
#include "lteMacRRMPeriodicMgr.h"
#include "ltePerfStats.h"
#include "lteCommonStatsManager.h"
/* SPR 21819 changes */
#include "lteMacParseUtil.h"
/* SPR 21819 changes */
/* + SPR 11209 Changes */
#include "lteMacCellConfMgr.h" 
 
/* - SPR 11209 Changes */
/* SPR 17957 Fix Start */
/* SPR 17957 Fix End */
/* SPR 23402 Changes Start */
 /* MOCN CHANGES START */
#include "lteMacOAMInterface.h"
 /* MOCN CHANGES END */
/* SPR 23402 Changes End */
/******************************************************************************
 Private Definitions
 *****************************************************************************/


/******************************************************************************
 Private Types
 *****************************************************************************/

 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/

/* Stage-III SCELL Deactivation Timer Related Changes Start */
STATIC void scellStateMngrUpdateScellDeactTimer(DLUEContext *ueDLContext_p,InternalCellIndex internalCellIndex);
/* Stage-III SCELL Deactivation Timer Related Changes End */
MacRetType resetHARQIfReTx(DLHARQProcess * hProcess_p,
                           DLUEContext * ueContext_p,
                           DciCCEInfo * dciCCEInfo_p,
                           InternalCellIndex internalCellIndex);

static MacRetType encodeNonSpatialMuxNode(DLHARQProcess * hProcess_p,
                              DciCCEInfo* dciCCEInfo_p,
                              TransmissionAndRLCData * transmissionAndRLCData_p,
                              DLUEContext * ueContext_p,
                              DLHARQTBInfo* harqProcessTB_p,
                              TBPopulationStruct *tbPopulationStruct_p,
                              /* SPR 431 changes start */
                              UInt8 tbIdentifier,
                              /* SPR 431 changes end */
                              InternalCellIndex internalCellIndex
                              /* + SPR_14039 */
#ifdef TDD_CONFIG                                           
                              /* SPR 15909 fix start */
                              ,tickType_t scheduledTick
                              /* SPR 15909 fix end */
#endif                                     
                              /* - SPR_14039 */
                              );

static MacRetType encodeSpatialMuxNode(DLHARQProcess * hProcess_p,
                          DciCCEInfo* dciCCEInfo_p,
                          TransmissionAndRLCData * transmissionAndRLCData_p,
                          DLUEContext * ueContext_p,
                          InternalCellIndex internalCellIndex
                          /* + SPR_14039 */
#ifdef TDD_CONFIG 
                          /* SPR 15909 fix start */
                          ,tickType_t scheduledTick
                          /* SPR 15909 fix end */
#endif                                         
                          /* - SPR_14039 */
                          );

void testprocessEncoderNode(EncoderQueueNode* encoderQueueNode_p,
                              UInt32 dciCCEInfoContainerIndex ,
                              DLUEContext* ueContext_p,
                              InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG 
							  /* SPR 15909 fix start */
                              ,tickType_t scheduledTick
                              /* SPR 15909 fix end */
#endif
							);

/* SPR 5599 changes start (ZIP ID 129111) */
/* +- SPR 17777 */
void populateTXInfo(
        /* SPR 5599 changes end (ZIP ID 129111) */
                            UInt32 isTxFunctionalityAdded,
                            UInt32 cceIndex,
                            UInt8 * cceIndexArr_p,
        UInt8 * writeIndex_p
                          );
/* +- SPR 17777 */

static void putDummyNodeInEncoderQueue(EncoderNode * encoderNode_p, InternalCellIndex internalCellIndex);

/** SPR 3657 Changes Start **/
/* SPR 3608 changes start */
static MacRetType createTB(TBPopulationStruct *tbPopulationStruct_p,
              UInt8 *dataTB_p,rlcDataInfo *rlcPduInfo_p, DLUEContext* ueContext_p);
/** SPR 3657 Changes End **/
static MacRetType createTBFromRLCData(TBPopulationStruct * tbPopulationStruct_p,
                         UInt8 *dataTB_p,
                         UInt32 *newQueueLoadEncoded,  DLUEContext* ueContext_p,
                         InternalCellIndex internalCellIndex);
/* SPR 3608 changes end */

static MacRetType processEncoderNode( EncoderQueueNode* encoderQueueNode_p,
                              UInt32 dciCCEInfoContainerIndex ,
                              DLUEContext* ueContext_p,
                              InternalCellIndex internalCellIndex
                              /* + SPR_14039 */
#ifdef TDD_CONFIG                              
                              /* SPR 15909 fix start */
                              ,tickType_t scheduledTick
                              /* SPR 15909 fix end */
#endif                        
                             /* - SPR_14039 */
                              );

#ifdef MAC_AUT_TEST
void testprocessEncoderNode(EncoderQueueNode* encoderQueueNode_p,
                              UInt32 dciCCEInfoContainerIndex ,
                              DLUEContext* ueContext_p,
                              InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG
							  /* SPR 15909 fix start */
                              ,tickType_t scheduledTick
                              /* SPR 15909 fix end */
#endif
							  )

{

processEncoderNode( encoderQueueNode_p,
                    dciCCEInfoContainerIndex ,
                    ueContext_p,
                    internalCellIndex
#ifdef TDD_CONFIG                      
                    ,scheduledTick
#endif                               
					);


}

#endif

/* SPR 5599 changes start (ZIP ID 129111) */
static void resetHarqState(DLHARQProcess * hProcess_p, DCIPduInfo *dciPduInfo_p);

static MacRetType processEncoderReTxNode(EncoderQueueNode *encoderQueueNode_p,
/* SPR 5599 changes end (ZIP ID 129111) */
                                                /* SPR 15909 fix start */                                        
                                                tickType_t currentTick,
                                                /* SPR 15909 fix end */
                                                UInt32 delayToPhy,
                                                UInt32 dciContainerIndex,
                                                InternalCellIndex internalCellIndex
/* + SPR 15127 changes */
#ifdef TDD_CONFIG
                                                ,UInt8 subFrameNum
#endif
/* - SPR 15127 changes */
                                                );
/* 32.425 Events code changes start*/
#ifdef KPI_STATS
void lteMacUpdateKpiStatsIPThpDlOnEnQueue (
        UInt8 lcId,
        UInt16 ueIndex,
        UInt8 lcCountForKpiStats ,
        TransmissionAndRLCData * transmissionAndRLCData_p,
        DLHARQProcess * hProcess_p
        );

#endif
/* 32.425 Events code changes end */
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* + SPR 11209 Changes */
EncoderContainerNode *encoderContainerArr_gp[MAX_NUM_CELL][MAX_NUM_CELL];
/* - SPR 11209 Changes */
UInt8 scheduleScellCE_g =0;
#ifdef CIRC_QUEUE_IMPL
extern SEM_T waitForEncoderQueueInd_g[MAX_NUM_CELL];
#endif
/* warning removed*/
#ifdef DL_UL_SPLIT
    /* +- SPR 17777 */
extern SInt32 recvMsgWithIccQ(SInt32 mqdes, const void *msg_ptr);
extern MacRetType putEntryInEncoderQueueUsingICC(SInt32 encoderQueueIdUL, EncoderQueueNode *encoderQueueNode_p); 
#endif
/*START_DYN_PDCCH_TDD_CHANGE*/
/*END_DYN_PDCCH_TDD_CHANGE*/
/*START : DYN_PDCCH*/
extern UInt8 currentCFIValue_g[MAX_NUM_CELL];/*DYN_PDCCH*/
/*END   : DYN_PDCCH*/

 /* 32.425 Events code changes start*/
#ifdef KPI_STATS
ScheduledIPThroughputDLT scheduledIPThroughputDL_g[MAX_UE_SUPPORTED][MAX_LOGICAL_CHANNEL - 3];
#endif
 /* 32.425 Events code changes end*/

/* + SPR 11209 Changes */
extern ServingCellSpecificParams cellSpecificParams_g;
/* - SPR 11209 Changes */
extern LTE_SEM cellSyncSem_g;
extern UInt8 usableAbsPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN]; /* Victim*/
extern  UInt8 usableAbsPatternIndex_g[MAX_NUM_CELL];
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
extern mocnOperatorParams mocnOperatorParams_g;
/* MOCN Changes End */
/* SPR 23402 Changes End */

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/** SPR 3657 Changes Start **/
/** SPR 3657 Changes End **/

/*****************************************************************************
 * Function Name  : createTBForSCELLTADRXMsg
 * Inputs         : timingAdvanceValue - TA value from ue context,
 *                  drxValue - drxFlag value from ue context,
 *                  harqTB_p - Pointer to DLHARQTBInfo of either TB 1 or
 *                             TB 2,
 *                  dataTB_p - Pointer to Transmission block to be encoded
 *                  macScellCEFlag
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is invoked in case of encoding only TA or
 *                  or only DRX or only TA and DRX.
 *****************************************************************************/
MacRetType createTBForSCELLTADRXMsg(UInt32 timingAdvanceValue ,
                         UInt32 drxValue ,
                         DLHARQTBInfo * harqTB_p,
                         UInt8 *dataTB_p,
                         UInt8 macScellCEFlag)
{
    UInt32 paddingCount =0;
    UInt32 paddingFlag =0;
    UInt32 tbSizeInBytes = harqTB_p->tbSize;
    UInt32 index = 0;
    UInt8 *tempDataTB_p = dataTB_p;
    /* SPR  3444 changes start */
    QSEGMENT segment = {0};
    /* SPR  3444 changes end */
    dataTB_p = msgSegNext(dataTB_p,0,&segment);
    /* +COVERITY 10065 */
    if(PNULL == dataTB_p)
    {
        return MAC_FAILURE;
    }
    /* -COVERITY 10065 */
    dataTB_p = segment.base;

    paddingCount = tbSizeInBytes;
   
/* This is the handling of the scenaro of one or two byte tbsize or more and only for padding */
    if (INVALID_TA_VALUE != timingAdvanceValue)
    {
        /*  TBsize is subtraceted by two to calculate padding count
         *  because one byte is for control element and one byte for
         *  control element Value.
         */
        paddingCount -= 2;
    }

    if (INVALID_DRX_VALUE != drxValue)
    {
        /*  TBsize is subtraceted by one to calculate padding count
         *  because one byte is for control element.
         */
        paddingCount -= 1;
    }
    /*CA_ENCODER_CHG*/
    
    if (INVALID_SCELL_ACT_DEACT_VALUE != macScellCEFlag)
    {
       paddingCount -= 2;
    } 
    if (1 ==  paddingCount)
    {
        MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_PADDING_LCH);
        index++;
    }
    else if (2 == paddingCount)
    {
        MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_PADDING_LCH);
        MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_PADDING_LCH);
        index += MAC_SUBHEADER_LENGTH_TWO_BYTE;
    }
    else if (paddingCount > 2)
    {                   
        paddingFlag = 1;
    }
    /* If SCELL Control element is present */
    if (macScellCEFlag != INVALID_SCELL_ACT_DEACT_VALUE)
    { 
        /*If TA CE is present */
        if (timingAdvanceValue != INVALID_TA_VALUE)
        {
           /*ADD Subheader for SCELL*/
           MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_SCELL_LCH);
           index++;
           /* If DRX CE is present */
           if(drxValue != INVALID_DRX_VALUE)
           {
              /*ADD Subheader for DRX */
              MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_TA_LCH);
              index++;
              if (paddingFlag)
              {
                 MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_DRX_LCH);
                 MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_PADDING_LCH);
                 index += MAC_SUBHEADER_LENGTH_THREE_BYTE;
              }
              else
              {
                 MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_DRX_LCH);
                 index += MAC_SUBHEADER_LENGTH_TWO_BYTE;
              }
           }
           /*if TA and DRX CE is present*/
           else
           {
              if (paddingFlag)
              {
                 MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_TA_LCH);
                 MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_PADDING_LCH);
                 index += MAC_SUBHEADER_LENGTH_TWO_BYTE;
              }
              else
              {
                 MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_TA_LCH);
                 index += 1;
              }
           }
        }
        /*If DRX and SCELL_ACT_DEACT is present*/
        else if (drxValue != INVALID_DRX_VALUE)
        {
            MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_SCELL_LCH);
            index++;
            if (paddingFlag)
            {
               MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_DRX_LCH);
               MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_PADDING_LCH);
               index += MAC_SUBHEADER_LENGTH_TWO_BYTE;
            }
            else
            {
               MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_DRX_LCH);
               index += 1;
            }
        }
        /* Only SCELL is present */
        else if (paddingFlag)
        {
            MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_SCELL_LCH);
            MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_PADDING_LCH);
            index += MAC_SUBHEADER_LENGTH_TWO_BYTE;
        }
        else
        {
            MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_SCELL_LCH);
            index += 1;
        }
    }
    /* If TA and DRX is present */
    else if (timingAdvanceValue != INVALID_TA_VALUE)
    {
       if(drxValue != INVALID_DRX_VALUE)
       {
          MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_TA_LCH);
          index++;
          if (paddingFlag)
          {
             MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_DRX_LCH);
             MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_PADDING_LCH);
             index += MAC_SUBHEADER_LENGTH_TWO_BYTE;
          }
          else
          {
             MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_DRX_LCH);
             index += MAC_SUBHEADER_LENGTH_TWO_BYTE;
          }
       }
       /* If only TA CE is present */
       else
       {
          if (paddingFlag)
          {
              MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_TA_LCH);
              MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_PADDING_LCH);
              index += MAC_SUBHEADER_LENGTH_TWO_BYTE;
          }
          else
          {
              MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_TA_LCH);
              index += 1;
          }
       }
     }
     /*Only DRX CE is present*/
     else
     {
         if (drxValue != INVALID_DRX_VALUE)
         {
             if (paddingFlag)
             {
                 MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_DRX_LCH);
             }
             else
             {
                 MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_DRX_LCH);            
             }
             index += 1;
         }
         /* SPR 12085 Start */	
        /*  No Need to return now.
         else
         {
             return MAC_FAILURE;
         }
        */         
         /* SPR 12085 End */	
         if (paddingFlag)
         {
             MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_PADDING_LCH);
             index += 1;
         }
     }
     /* We need to store data start index and data end index, 
     * this is necesssary for RETX*/
     harqTB_p->dataStartIndex = index;
     if ( timingAdvanceValue != INVALID_TA_VALUE )
     {
        *dataTB_p = timingAdvanceValue;
        index++;
     }
     if( macScellCEFlag != INVALID_SCELL_ACT_DEACT_VALUE )
     {
        *dataTB_p = macScellCEFlag;
        index++;
     }
     /* padding bits updation is necessary for retransmision in
     * case when TB size changes*/
     harqTB_p->paddingBytes = paddingCount;
     dataTB_p = tempDataTB_p;
     /* SPR 4591 Fix Start */
     if ( harqTB_p->tbSize  < MAX_MAC_HEADER_MAC_CCE_LEN )
     {
        msgRemove(tempDataTB_p, 
        harqTB_p->tbSize + DATA_MSG_API_HEADER_LEN, 
        MAX_MAC_HEADER_MAC_CCE_LEN - harqTB_p->tbSize-DATA_MSG_API_HEADER_LEN);
     }
     /*SPR 4591 Fix End*/
     return MAC_SUCCESS;
}


/* Cyclomatic Complexity changes - starts here */
/*Complexity Start*/
/*****************************************************************************
 * Function Name  : updateABSperQciUsage
 * Inputs         : internalCellIndex,
 *                  ueContext_p-Pointer to Downlink UE context
 *                  numRBs,
 *                  lcId-Index of logical channel
 * Outputs        : None
 * Returns        : None
 * Description    : This Function increment the per QCI RBs usage if 
 *                  user is a victim .
 *                  
 *****************************************************************************/

void updateABSperQciUsage(InternalCellIndex internalCellIndex,DLUEContext* ueContext_p,UInt32 numRBs,UInt8 lcId)
{
   UInt8 victimStatus=0;
   victimStatus=ueContext_p->userType;
   UInt8 qci= 0;
   qci = ueContext_p->logicalChannel[lcId].dlLcQosInfo.QCI;

   if( rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->eicicReportConfigParams.absUsageRequired )
   {
        if ( 1==victimStatus )
	    {
           rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->\
   	             eicicReportInd.dlEICICReportInfo_p[rrmMacPeriodicMgrCellGlobal_gp[internalCellIndex]->\
   	               activeReportIndexEicic].absPerQci[qci-1] += numRBs;
	    }	

    }
}
/*Complexity End*/
/*****************************************************************************
 * Function Name  : removeExtraSpaceTB
 * Inputs         : tempLcInfo_p,
 *                  lcCount,
 *                  maxLcCount,
 *                  dataTB_p - Pointer to Transmission block to be encoded,
 *                  index
 * Outputs        : None
 * Returns        : None
 * Description    : This Function Process the PDU and remove the 
 *                  unused size/space of TB based on RLC PDU LRN.
 *****************************************************************************/
static  void removeExtraSpaceTB(RLCDataBuffer *tempLcInfo_p,
                                        UInt32 lcCount,
                                        UInt32 maxLcCount,
                                        UInt8 *dataTB_p,
                                        UInt32 *index)
{
    UInt32 blockSize = 0;

    while (lcCount < maxLcCount)
    {
        blockSize = 0;
        /* Populate TB with data from RLC*/

        if (tempLcInfo_p->statusPdu_p )
        {
            UInt32 rlcMsgSize =  msgSize(tempLcInfo_p->statusPdu_p,&blockSize);

            if (rlcMsgSize != tempLcInfo_p->statusPduLen)
            {  /*SPR 4591 Fix Start*/
                msgRemove(tempLcInfo_p->statusPdu_p, tempLcInfo_p->statusPduLen, rlcMsgSize - tempLcInfo_p->statusPduLen );
                /*SPR 4591 Fix End*/
            }
            msgJoinAndFree(dataTB_p,tempLcInfo_p->statusPdu_p);
            (*index)    += tempLcInfo_p->statusPduLen;  

        }
        UInt32 reTxPduCount;
        reTxPduCount = QUEUE_COUNT_AMD_RETX_PDU_Q(tempLcInfo_p->reTxRlcPduQ);
        if (reTxPduCount)
        {
            rlcReTxPduNode *rlcReTxPduNode_p = PNULL;
            DEQUEUE_AMD_RETX_PDU_Q(rlcReTxPduNode_p,tempLcInfo_p->reTxRlcPduQ); 
            while( rlcReTxPduNode_p && reTxPduCount--)
            {
                UInt32 rlcMsgSize =  msgSize(rlcReTxPduNode_p->retxRlcPdu_p,&blockSize);

                if (rlcMsgSize != rlcReTxPduNode_p->retxPduLen)
                {   /*SPR 4591 Fix Start*/
                    msgRemove(rlcReTxPduNode_p->retxRlcPdu_p, rlcReTxPduNode_p->retxPduLen, rlcMsgSize - rlcReTxPduNode_p->retxPduLen );
                    /*SPR 4591 Fix End*/
                }

                msgJoinAndFree(dataTB_p,rlcReTxPduNode_p->retxRlcPdu_p);
                (*index)    += rlcReTxPduNode_p->retxPduLen;
                freeMemPool(rlcReTxPduNode_p);
                rlcReTxPduNode_p = PNULL;
                DEQUEUE_AMD_RETX_PDU_Q(rlcReTxPduNode_p,tempLcInfo_p->reTxRlcPduQ); 
            }
        }
        /** SPR 3657 Changes End **/

        if (tempLcInfo_p->newRlcPdu_p)
        {
            UInt32 rlcMsgSize =  msgSize(tempLcInfo_p->newRlcPdu_p,&blockSize);
            if (rlcMsgSize != tempLcInfo_p->newPduLen)
            {   /*SPR 4591 Fix Start*/
                msgRemove(tempLcInfo_p->newRlcPdu_p, tempLcInfo_p->newPduLen, rlcMsgSize - tempLcInfo_p->newPduLen );
                /*SPR 4591 Fix End*/
            }
            msgJoinAndFree(dataTB_p,tempLcInfo_p->newRlcPdu_p);
            (*index)    += tempLcInfo_p->newPduLen;  

        }

        tempLcInfo_p++;
        lcCount++;
    }
}
/* Cyclomatic Complexity changes - ends here */

/* Commenting this function as retx does nto pass through encoder. currently, 
 * change in TB size for rext is not supported */

/*****************************************************************************
 * Function Name  : createTB
 * Inputs         : tbPopulationStruct_p - This contains data from which 
 *                                         TB is populated,
 *                  dataTB_p - Pointer to transmission block which is 
 *                             populated while encoding,
 *                  rlcPduInfo_p - pointer to rlcDataInfo,
 *                  ueContext_p - Pointer to DLUEContext
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called to create new transmision block.
 *****************************************************************************/
/** SPR 3657 Changes Start **/
MacRetType createTB( TBPopulationStruct *tbPopulationStruct_p,
                     UInt8 *dataTB_p,rlcDataInfo *rlcPduInfo_p,
                     /* SPR 3608 changes start */
                     DLUEContext* ueContext_p
                     /* SPR 3608 changes end */
        )
/** SPR 3657 Changes End **/

{
    RLCDataBuffer *lcInfo_p     = tbPopulationStruct_p->rlcDataBuffer_p;
    RLCDataBuffer *tempLcInfo_p = PNULL;
    UInt32 paddingCount = tbPopulationStruct_p->padding;
    DLHARQTBInfo * harqTB_p = tbPopulationStruct_p->harqTB_p;
    UInt8 taValue = tbPopulationStruct_p->taValue; 
    UInt32 drxValue = tbPopulationStruct_p->drxValue;
    UInt8 macScellCE = tbPopulationStruct_p->macScellCEFlag;
    UInt32 index = 0;
    UInt32 maxLcCount = 0;
    /** SPR 3657 Changes Start **/
    UInt32 rlcPduCount = 0;
    UInt32 reTxPduCount = 0;
    /** SPR 3657 Changes End **/

    UInt32 paddingFlag =0;
    /* paddingFlag stores the information if padding is more than 2 bytes 
     * or not .If it is 1 this means we need to add Padding Header at the
     * end of MAC SDU*/
	

    UInt32 lcCount = 0;
    UInt8 *tempDataTB_p = dataTB_p;
    /* SPR  3444 changes start */
    QSEGMENT segment = {0};
    /* SPR  3444 changes end */
    dataTB_p = msgSegNext(tempDataTB_p, 0, &segment);
    
    /* +COVERITY 10064 */
    if(PNULL == dataTB_p)
    {
        return MAC_FAILURE;
    }
    /* -COVERITY 10064 */
    dataTB_p = segment.base + DATA_MSG_API_HEADER_LEN;
    /* Calculating the padding */
    maxLcCount = tbPopulationStruct_p->rlcBufferStartIndex +
                           tbPopulationStruct_p->lcCount;

    LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Encoding SUBHEADERS",__func__);
    /* Convention X(n)   - 
     *                     n - number of bytes.
     *                     X - name of TB element ( P - padding, SH - 
     *                     sdu header , SP - sdu payload)
     */
    LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Encoding PADDING",__func__);

    if (0 == paddingCount)
    {
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]PaddingCount == 0",__func__);
        /* When padding count == 0
         *  (a)When last lc data size is < 127
         *         SH(3)SH(2) becomes P(1)SH(3)SH(1)SP(n)
         *  (a)When last lc data size is > 127
         *         SH(3)SH(3) becomes P(1)P(1)SH(3)SH(1)SP(n)
         */

        /** SPR 3657 Changes Start **/
        if(rlcPduInfo_p->lastPduLen > MAX_TWO_BYTE_SUBHEADER_LENGTH) 
        /** SPR 3657 Changes End **/
        {
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Last Lc buff leng %d > 127 ,Adding Two byte padding",__func__,
                    lcInfo_p[maxLcCount-1].totalPduLen);

            paddingCount = 2;
            MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_PADDING_LCH);
            index++;
            MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_PADDING_LCH);
            index++;
            LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                    LOGBRIEF,
                    MAC_RRC_INF,
                    getCurrentTick(),
                    LINE_NUMBER,
                    ueContext_p->ueIndex,
                    MAC_PADDING_LCH,
                    2, /*header*/
                    rlcPduInfo_p->lastPduLen,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "");
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Last Lc buff leng %d < 127 ,Adding One byte padding",__func__,
                    lcInfo_p[maxLcCount-1].totalPduLen);

            paddingCount = 1;
            MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_PADDING_LCH);
            index++;
            LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                    LOGBRIEF,
                    MAC_RRC_INF,
                    getCurrentTick(),
                    LINE_NUMBER,
                    ueContext_p->ueIndex,
                    MAC_PADDING_LCH,
                    1, /*header*/
                    rlcPduInfo_p->lastPduLen,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "");
        }
    }
    else if (1 == paddingCount)
    {
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]PaddingCount == 1",__func__);
        /* When padding count == 1
         *  (a)When last lc data size is < 127
         *         P(1)SH(3)SH(2) becomes P(1)P(1)SH(3)SH(1)SP(n)
         *  (a)When last lc data size is > 127
         *         P(1)SH(3)SH(3) becomes SH(3)SH(1)P(1)SP(n)
         */
        /** SPR 3657 Changes Start **/
        if(rlcPduInfo_p->lastPduLen > MAX_TWO_BYTE_SUBHEADER_LENGTH)
        /** SPR 3657 Changes End **/
        {
            paddingCount = 3;
            paddingFlag =1;
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Last Lc buff leng  > 127 ,Padding == 3",__func__);
        }
        else
        {
            /* If padding count == 1 and buffer length of last lc multiplexed
             * is less than 127 then , the last LC sub header will be
             * of size 1 byte and remaining 1 byte is added to padding ,
             * resulting in two byte padding
             */
            paddingCount = 2;
            MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_PADDING_LCH);
            index++;
            MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_PADDING_LCH);
            index++;
            LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                    LOGBRIEF,
                    MAC_RRC_INF,
                    getCurrentTick(),
                    LINE_NUMBER,
                    ueContext_p->ueIndex,
                    MAC_PADDING_LCH,
                    2, /*header*/
                    rlcPduInfo_p->lastPduLen,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "");

            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Last Lc buff leng %d < 127 ,Padding == 2",__func__,
                    lcInfo_p[maxLcCount-1].totalPduLen);
        }
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Padding > 2 bytes",__func__);
        /* In all other case i.e padding >= 2
         *         P(1)P(1)P(n)SH(3)SH(3) becomes SH(3)SH(1)P(1)SP(n)
         */
        
        paddingFlag  = 1;
        if (paddingCount == 2)
        {
        }
    }

    /* Checking is taValue need to be sent */
    if ( taValue != INVALID_TA_VALUE )
    {
        /* Filling TB with TA Control Element */
        MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_TA_LCH);
        index++;        
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Encoding TA SUBHEADER",__func__);
        LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                LOGINFO,
                MAC_RRC_INF,
                getCurrentTick(),
                LINE_NUMBER,
                ueContext_p->ueIndex,
                MAC_TA_LCH,
                1, /*header*/
                0,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "");
    }
    /* Checking is DRX Value need to be sent */
    if (drxValue!= INVALID_DRX_VALUE )
    {
        /*  Filling TB with Drx Control Element */
        MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_DRX_LCH);
        index++;        
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Encoding DRX SUBHEADER",__func__);
        LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                LOGINFO,
                MAC_RRC_INF,
                getCurrentTick(),
                LINE_NUMBER,
                ueContext_p->ueIndex,
                MAC_DRX_LCH,
                1, /*header*/
                0,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "");
    }
    /*CA_ENCODER_CHG*/
    if(macScellCE != INVALID_SCELL_ACT_DEACT_VALUE)
    {
        MAC_SUBHEADER_ONE_BYTE(dataTB_p,1,MAC_SCELL_LCH);
        index++;
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Encoding TA SUBHEADER",__func__);
        LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                LOGINFO,
                MAC_RRC_INF,
                getCurrentTick(),
                LINE_NUMBER,
                ueContext_p->ueIndex,
                MAC_SCELL_LCH,
                1, /*header*/
                0,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "");
    }
    /*  Iterate through the loop for number of times = number of 
     *  logical channel -1 .We are iterating number of logical channel
     *  -1 times because for last MAC SubHeader We can have a scenraio 
     *  in which we don't have have F and L field .This scneraio comes 
     *  when we have padding less than 2 bytes */    

    lcCount = tbPopulationStruct_p->rlcBufferStartIndex;
    /* set the start Index, for TB 1 this will be == 0 
     * For TB 2 this will be equal to index from where 
     * the LCs corresponding to TB 2 begin*/
    LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Encoding LC SUBHEADERS",__func__);

    tempLcInfo_p = lcInfo_p + lcCount;
    /** SPR 3657 Changes Satrt **/
    /* As We come to know that RLC has send x number of PDUs to MAC 
    so MAC will claculate Bytes required for Header */
    while (lcCount <  maxLcCount)
    {
    	/* SPR 21819 changes */
        /* Update Control information state on sending the SRB1-DL packet */
	if (SRB_1_LCID == tempLcInfo_p->lcID)
	{
            macUpdateControlStateOnSRB1DLPacket(ueContext_p->ueIndex);
    	}
	/* SPR 21819 changes */

    	   /* For Status Pdu */
    	if ((tempLcInfo_p->statusPdu_p) && ((rlcPduCount + 1) != rlcPduInfo_p->rlcPduCount ))
     	{
            rlcPduCount++;
        /** SPR 3657 Changes End **/   
            if ( tempLcInfo_p->statusPduLen > MAX_TWO_BYTE_SUBHEADER_LENGTH )
            {
                /* Filling TB with MAC SubHeader of Three Bytes */
                MAC_SUBHEADER_THREE_BYTE(dataTB_p, 1, tempLcInfo_p->statusPduLen,
                        tempLcInfo_p->lcID);

                index += MAC_SUBHEADER_LENGTH_THREE_BYTE;
                LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                        LOGBRIEF,
                        MAC_RRC_INF,
                        getCurrentTick(),
                        LINE_NUMBER,
                        ueContext_p->ueIndex,
                        tempLcInfo_p->lcID,
                        3, /*header*/
                        tempLcInfo_p->statusPduLen,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "");

                LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]LCID %d LC data len %d [3 byte sub hdr]",
                        __func__, tempLcInfo_p->lcID, tempLcInfo_p->statusPduLen);


            }
            else
            {
                /* If the Logical Channel Data is <= 127 
                 * ,in this case MAC SubHeader would be Two
                 * Bytes */

                /* Filling TB with MAC SubHeader of Two Bytes */

                MAC_SUBHEADER_TWO_BYTE(dataTB_p, 1, tempLcInfo_p->statusPduLen,
                        tempLcInfo_p->lcID); 
                index += MAC_SUBHEADER_LENGTH_TWO_BYTE;
                LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                        LOGBRIEF,
                        MAC_RRC_INF,
                        getCurrentTick(),
                        LINE_NUMBER,
                        ueContext_p->ueIndex,
                        tempLcInfo_p->lcID,
                        2, /*header*/
                        tempLcInfo_p->statusPduLen,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "");

                LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]LCID %d LC data len %d [2 byte sub hdr]",
                        __func__,tempLcInfo_p->lcID,tempLcInfo_p->statusPduLen);
            }
        }

        /**** For Retx PDU ***************/ 
        /** SPR 3657 Changes Start **/ 
        reTxPduCount = QUEUE_COUNT_AMD_RETX_PDU_Q(tempLcInfo_p->reTxRlcPduQ);
        if(reTxPduCount )
        {
            rlcReTxPduNode *rlcReTxPduNode_p = PNULL;
            GET_QUEUE_NODE_AMD_RETX_PDU_Q(rlcReTxPduNode_p,
                tempLcInfo_p->reTxRlcPduQ);
            while(rlcReTxPduNode_p && (reTxPduCount--) && ((rlcPduCount + 1) != rlcPduInfo_p->rlcPduCount ))
            {
                rlcPduCount++;
                if ( rlcReTxPduNode_p->retxPduLen > MAX_TWO_BYTE_SUBHEADER_LENGTH )
                {
                    /* Filling TB with MAC SubHeader of Three Bytes */

                    MAC_SUBHEADER_THREE_BYTE(dataTB_p,1,rlcReTxPduNode_p->retxPduLen,
                        tempLcInfo_p->lcID);
                    index += MAC_SUBHEADER_LENGTH_THREE_BYTE;
                    LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                            LOGBRIEF,
                            MAC_RRC_INF,
                            getCurrentTick(),
                            LINE_NUMBER,
                            ueContext_p->ueIndex,
                            tempLcInfo_p->lcID,
                            3, /*header*/
                            rlcReTxPduNode_p->retxPduLen,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,
                        "");
                    LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]LCID %d LC data len %d [3 byte sub hdr]",
                        __func__,tempLcInfo_p->lcID,rlcReTxPduNode_p->retxPduLen);

                }
                else
                {
                    /* If the Logical Channel Data is <= 127 
                    * ,in this case MAC SubHeader would be Two
                    * Bytes */

                    /* Filling TB with MAC SubHeader of Three Bytes */

                    MAC_SUBHEADER_TWO_BYTE(dataTB_p,1,rlcReTxPduNode_p->retxPduLen,
                        tempLcInfo_p->lcID); 
                    index += MAC_SUBHEADER_LENGTH_TWO_BYTE;
                    LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                            LOGBRIEF,
                            MAC_RRC_INF,
                            getCurrentTick(),
                            LINE_NUMBER,
                            ueContext_p->ueIndex,
                            tempLcInfo_p->lcID,
                            2, /*header*/
                            rlcReTxPduNode_p->retxPduLen,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,
                        "");
                    LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]LCID %d LC data len %d [2 byte sub hdr]",
                        __func__,tempLcInfo_p->lcID,rlcReTxPduNode_p->retxPduLen);
                }
                SET_NEXT_AMD_RETX_PDU_Q(tempLcInfo_p->reTxRlcPduQ,rlcReTxPduNode_p);
            }/** End Of While loop **/
        }/** End of If **/
        /** SPR 3657 Changes End **/
 
        /****** For NEW RLC PDU ****************/
        /** SPR 3657 Changes Start **/ 
        if ((tempLcInfo_p->newRlcPdu_p) && ((rlcPduCount + 1) != rlcPduInfo_p->rlcPduCount ))
        {
            rlcPduCount++;
        /** SPR 3657 Changes End **/
            if ( tempLcInfo_p->newPduLen > MAX_TWO_BYTE_SUBHEADER_LENGTH )
            {
                /* Filling TB with MAC SubHeader of Three Bytes */

                MAC_SUBHEADER_THREE_BYTE(dataTB_p,1,tempLcInfo_p->newPduLen,
                        tempLcInfo_p->lcID);
                index += MAC_SUBHEADER_LENGTH_THREE_BYTE;
                LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                        LOGBRIEF,
                        MAC_RRC_INF,
                        getCurrentTick(),
                        LINE_NUMBER,
                        ueContext_p->ueIndex,
                        tempLcInfo_p->lcID,
                        3, /*header*/
                        tempLcInfo_p->newPduLen,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "");
                LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]LCID %d LC data len %d [3 byte sub hdr]",
                        __func__,tempLcInfo_p->lcID,tempLcInfo_p->newPduLen);

            }
            else
            {
                /* If the Logical Channel Data is <= 127 
                 * ,in this case MAC SubHeader would be Two
                 * Bytes */

                /* Filling TB with MAC SubHeader of Three Bytes */

                MAC_SUBHEADER_TWO_BYTE(dataTB_p,1,tempLcInfo_p->newPduLen,
                        tempLcInfo_p->lcID); 
                index += MAC_SUBHEADER_LENGTH_TWO_BYTE;
                LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                        LOGBRIEF,
                        MAC_RRC_INF,
                        getCurrentTick(),
                        LINE_NUMBER,
                        ueContext_p->ueIndex,
                        tempLcInfo_p->lcID,
                        2, /*header*/
                        tempLcInfo_p->newPduLen,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "");
                LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]LCID %d LC data len %d [2 byte sub hdr]",
                        __func__,tempLcInfo_p->lcID,tempLcInfo_p->newPduLen);
            }
        }

        tempLcInfo_p++;
        lcCount++;
    }
   /** SPR 3657 Changes Start **/
   /** SPR 3657 Changes End **/
   
    /* Checking if padding Flag is true .If true in this case last 
     * SubHeader would be padding SubHeader  and then MAC SubHeader 
     * corrosponding to last Logical Channel would have F and L Field.
     * MAC SubHeader for Padding would be of one byte that would be 
     * added after the MAC SUBHeaders of Logical Channel */
    if (1 == paddingFlag)
    {

        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Encoding Last LC :Padding > 2",__func__);
        /* Checking if the Logical Channel Data is > 127 ,in this case 
        * MAC SubHeader would be three Bytes */
     
        /** SPR 3657 Changes Start **/
        if(rlcPduInfo_p->lastPduLen > MAX_TWO_BYTE_SUBHEADER_LENGTH)
        {
            /* Filling TB with MAC SubHeader of Three Bytes */
            MAC_SUBHEADER_THREE_BYTE(dataTB_p,1, rlcPduInfo_p->lastPduLen,
                    lcInfo_p[rlcPduInfo_p->lastIndex].lcID);
            index += MAC_SUBHEADER_LENGTH_THREE_BYTE;
            LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                    LOGBRIEF,
                    MAC_RRC_INF,
                    getCurrentTick(),
                    LINE_NUMBER,
                    ueContext_p->ueIndex,
                    lcInfo_p[rlcPduInfo_p->lastIndex].lcID,
                    3, /*header*/
                    rlcPduInfo_p->lastPduLen,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "");
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]LCID %d LC data len %d [3 byte sub hdr]",
                    __func__,lcInfo_p[lcCount - 1].lcID, rlcPduInfo_p->lastPduLen);
        }
        else
        {
            /* This case comes when the Logical Channel Data is <= 127 
             * ,in this case MAC SubHeader would be Two Bytes */
            /* Filling TB with MAC SubHeader of Three Bytes */

            MAC_SUBHEADER_TWO_BYTE(dataTB_p, 1, rlcPduInfo_p->lastPduLen,
                    lcInfo_p[rlcPduInfo_p->lastIndex].lcID);
            index += MAC_SUBHEADER_LENGTH_TWO_BYTE;
            LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                    LOGBRIEF,
                    MAC_RRC_INF,
                    getCurrentTick(),
                    LINE_NUMBER,
                    ueContext_p->ueIndex,
                    lcInfo_p[rlcPduInfo_p->lastIndex].lcID,
                    2, /*header*/
                    rlcPduInfo_p->lastPduLen,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "");

            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]LCID %d LC data len %d [2 byte sub hdr]",
                    __func__, lcInfo_p[lcCount-1].lcID, rlcPduInfo_p->lastPduLen);
        /** SPR 3657 Changes End **/
        }
        /* Filling TB with Last  Mac SubHeader corrosponding to
         * Padding.In this case MAC SubHeader that would be added 
         * is One Byte .In this case E field of MAC SubHeader is 
         * 0*/

        MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,MAC_PADDING_LCH);
        LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                LOGBRIEF,
                MAC_RRC_INF,
                getCurrentTick(),
                LINE_NUMBER,
                ueContext_p->ueIndex,
                MAC_PADDING_LCH,
                1, /*header*/
                rlcPduInfo_p->lastPduLen,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "");
        index++;

    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Encoding Last LC :Padding < 2",__func__);
        /* This case will come when the last Mac SubHeader would 
         * corresponds to Logical Channel.In this case this MAC 
         * SubHeader will be one Byte Long and it would have F 
         * and L Field and also E field will be 0 */
        MAC_SUBHEADER_ONE_BYTE(dataTB_p,0,lcInfo_p[rlcPduInfo_p->lastIndex].lcID);
        LOG_MAC_MSG(MAC_TB_HEADER_INFO,
                LOGBRIEF,
                MAC_RRC_INF,
                getCurrentTick(),
                LINE_NUMBER,
                ueContext_p->ueIndex,
                lcInfo_p[rlcPduInfo_p->lastIndex].lcID,
                1, /*header*/
                0,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "");
        index++;

    }

    /* Store the index from which data starts , this is
     * useful to reduce retx effort whn TB size changes.*/
    harqTB_p->dataStartIndex = index;


    if ( INVALID_TA_VALUE != taValue )
    {
        /*  Filling TB with TA Control Element */
        *dataTB_p++ = taValue;
        index++;        
    }
    if (INVALID_SCELL_ACT_DEACT_VALUE != macScellCE)
    {
       *dataTB_p++ = macScellCE;
        index++;
    }
    /*now that header and CCE have been inserted, remove the extra space allocated*/
    dataTB_p = tempDataTB_p;
    msgRemove(dataTB_p,index+DATA_MSG_API_HEADER_LEN, MAX_MAC_HEADER_MAC_CCE_LEN-index-DATA_MSG_API_HEADER_LEN);
    lcCount = tbPopulationStruct_p->rlcBufferStartIndex;

    tempLcInfo_p = lcInfo_p + lcCount;
/* Cyclomatic Complexity changes - starts here */
    removeExtraSpaceTB(tempLcInfo_p,
                              lcCount,
                              maxLcCount,
                              dataTB_p,
                              &index);
/* Cyclomatic Complexity changes - ends here */
        tbPopulationStruct_p->padding = paddingCount;
    if (index  > harqTB_p->tbSize)
    {
        ltePanic("This shouldn't happen again\n");
    }

    return MAC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : createTBFromRLCData
  * Inputs         : tbPopulationStruct_p - This contains data from which 
 *                                         TB is populated,
 *                  dataTB_p - Pointer to transmission block which is 
 *                             populated while encoding,
 *                  newQueueLoadEncoded_p - This is filled with the queueload of
 *                                        data encoded in the case of new tx.
 *                  ueContext_p - Pointer to DLUEContext
 *                  internalCellIndex - Cell-Indexc at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called in case of new transmission .
 *                  The purpose of this function is to validate the data 
 *                  sent by RLC if padding bytes < 2 else validation is skipped
 *                  and is done in createTB.
 *****************************************************************************/
MacRetType createTBFromRLCData(TBPopulationStruct * tbPopulationStruct_p,
                         UInt8 *dataTB_p,
                         UInt32 *newQueueLoadEncoded_p,
                         /* SPR 3608 changes start */
                         DLUEContext* ueContext_p,
                         InternalCellIndex internalCellIndex)
                         /* SPR 3608 changes end */
{
    MacRetType retVal = MAC_FAILURE;
    RLCDataBuffer *rlcDataBuffer_p = tbPopulationStruct_p->rlcDataBuffer_p;
    UInt32 lcCount = tbPopulationStruct_p->lcCount;
    UInt32 index =0;
    /** SPR 3657 Changes Start **/
    UInt32 reTxPduCount = 0;
    rlcDataInfo rlcPduInfo ;
    rlcPduInfo.lastPduLen = 0;
    rlcPduInfo.rlcPduCount = 0;
    /** SPR 3657 Changes End **/
    /* FDD_Warning_Fix */
    rlcPduInfo.lastIndex = 0;
    /* FDD_Warning_Fix */
    /* Array storing the LCInfo pointers */
    UInt32    numOfLCMultiplexed = 0;   
    UInt32    qLoadSum = 0;
    /* SPR# 2998 Changes Starts */
    /*This code is added to take care scenario when padding from RLC would be
     * zero and Padding count should be calculated at encoder from TB size -Qos
     * Integration*/
    UInt16 remByte=0;
    UInt16 macPduByte=0;
    /* SPR# 2998 Changes Ends */
    /* Number of LCs whose data is successfully returned by RLC    */

    /* The rlcBufferStartIndex indicates the index from which the 
     * LC's corresponding to the TB Being encoded start in the
     * RLC buffer array. lcCount represent the number of LCs from
     * rlcBufferStartIndex belong to the TB being encoded.*/
    index = tbPopulationStruct_p->rlcBufferStartIndex;
    lcCount += index;

/* SPR# 2998 Changes Starts */
        /* Checking is taValue need to be sent */
    if ( tbPopulationStruct_p->taValue != INVALID_TA_VALUE )
    {   
        /* SPR 5123 Fix Start */
        macPduByte += 2;
        /* SPR 5123 Fix End */
    }
    if (tbPopulationStruct_p->drxValue!= INVALID_DRX_VALUE )
    {   
        macPduByte +=1;
    }    
    /*CA_ENCODER_CHG*/
    if (tbPopulationStruct_p->macScellCEFlag != INVALID_SCELL_ACT_DEACT_VALUE)
    {
        macPduByte += 2;
    } 
/* SPR# 2998 Changes Ends */

    /* + CQI_5.0 */
    if(PNULL != rlcDataBuffer_p)
    {
        for ( ; index < lcCount ;index++)
        {
            /* SPR 3608 changes start */
            if(ueContext_p->prioritySchedulingReqDL)
            {
                if(MAC_LCID_1 < rlcDataBuffer_p[index].lcID)
                {
                    ueContext_p->recvdEnbPsrCount++;
                    if (ueContext_p->recvdEnbPsrCount >= ueContext_p->enbPsrCount)
                    {
                        /* SPR 9203 Fix Starts*/
                        ueContext_p->recvdEnbPsrCount=0;
                        /* SPR 9203 Fix Ends*/
                        ueContext_p->prioritySchedulingReqDL = FALSE;
                    }
                }
            }
            /* SPR 3608 changes end */
            /* Checking if bufferLength sent by RLC is greater than requested */
            if ( rlcDataBuffer_p[index].totalPduLen ) 
            {
                if ( rlcDataBuffer_p[index].totalPduLen > rlcDataBuffer_p[index].requestedDataSize)
                {
                    ltePanic("For testing \n");     
                    
		         }
                 else
                 {
                    if ( rlcDataBuffer_p[index].statusPduLen )
                    {
                        qLoadSum +=  rlcDataBuffer_p[index].statusPduLen;
                        /** SPR 3657 Changes Start **/
                        rlcPduInfo.rlcPduCount++;
                        rlcPduInfo.lastPduLen = rlcDataBuffer_p[index].statusPduLen;
                        rlcPduInfo.lastIndex = index;
                        /** SPR 3657 Changes End **/
                        if(rlcDataBuffer_p[index].statusPduLen > MAX_TWO_BYTE_SUBHEADER_LENGTH)
                            macPduByte +=3;
                        else
                            macPduByte +=2;
                    }
                    /** SPR 3657 Changes Start **/
                    reTxPduCount = QUEUE_COUNT_AMD_RETX_PDU_Q(rlcDataBuffer_p[index].reTxRlcPduQ);
                    if(reTxPduCount)
                    {
                        rlcReTxPduNode *rlcReTxPduNode_p = PNULL; 
                        GET_QUEUE_NODE_AMD_RETX_PDU_Q(rlcReTxPduNode_p, 
                                rlcDataBuffer_p[index].reTxRlcPduQ);
                        while(rlcReTxPduNode_p && reTxPduCount--)
                        {
                            qLoadSum +=  rlcReTxPduNode_p->retxPduLen;
                            rlcPduInfo.rlcPduCount++;
                            rlcPduInfo.lastPduLen = rlcReTxPduNode_p->retxPduLen;
							rlcPduInfo.lastIndex = index;
                            if(rlcReTxPduNode_p->retxPduLen > MAX_TWO_BYTE_SUBHEADER_LENGTH)
                                macPduByte +=3;
                            else
                                macPduByte +=2;
                            SET_NEXT_AMD_RETX_PDU_Q(rlcDataBuffer_p[index].reTxRlcPduQ,rlcReTxPduNode_p);
                        }
                    }
                    /** SPR 3657 Changes End **/
                    if ( rlcDataBuffer_p[index].newPduLen )
                    {
                        qLoadSum +=  rlcDataBuffer_p[index].newPduLen;
                        /** SPR 3657 Changes Start **/
                        rlcPduInfo.rlcPduCount++;
                        rlcPduInfo.lastPduLen = rlcDataBuffer_p[index].newPduLen;
						rlcPduInfo.lastIndex = index;
                        /** SPR 3657 Changes End **/
                        if(rlcDataBuffer_p[index].newPduLen > MAX_TWO_BYTE_SUBHEADER_LENGTH)
                            macPduByte +=3;
                        else
                            macPduByte +=2;
                    }

                    numOfLCMultiplexed +=1;
                 }
                 /* SPR# 2998 Changes Ends */
                 LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]LCID %d req Len %d recvd Len %d",__func__,
                        rlcDataBuffer_p[index].lcID,rlcDataBuffer_p[index].requestedDataSize,
                        rlcDataBuffer_p[index].totalPduLen);
            }
        }
        /* + CQI_5.0 */
    }
    /* - CQI_5.0 */
    
    if (statsInfo_g[internalCellIndex].dlThroughPutEnable)
    {
        /* Throughput correction */
        statsInfo_g[internalCellIndex].totalDlBytesSend += 
            MULTIPLY_BY_EIGHT(tbPopulationStruct_p->harqTB_p->tbSize);
        /* Throughput correction */
    }
    *newQueueLoadEncoded_p = qLoadSum;
    LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Num of LCs to be encoded %d padding %d\n",__func__,
            numOfLCMultiplexed,tbPopulationStruct_p->padding);

    if ( qLoadSum + macPduByte <= tbPopulationStruct_p->harqTB_p->tbSize)	
    {
        remByte = tbPopulationStruct_p->harqTB_p->tbSize - ( qLoadSum + macPduByte );	
    }
    else
    {
        ltePanic("Not possible\n");
    }
    /* SPR# 2998 Changes Starts */
    tbPopulationStruct_p->padding = remByte;

/* SPR# 2998 Changes Ends */

    /* Calling function to form Encoded TB */
    if(numOfLCMultiplexed)
    {

      /** SPR 3657 Changes Start **/
      retVal = createTB(tbPopulationStruct_p,dataTB_p,&rlcPduInfo, ueContext_p);
      /** SPR 3657 Changes End **/
    }
    else
    {
        /*SPR 1741 changes start */
          retVal = createTBForSCELLTADRXMsg( tbPopulationStruct_p->taValue,
                                     tbPopulationStruct_p->drxValue,
                                     tbPopulationStruct_p->harqTB_p,
                                     dataTB_p,
                                     tbPopulationStruct_p->macScellCEFlag);
		return retVal;
        /*SPR 1741 changes  end */
     }
    return retVal;
}


/*****************************************************************************
 * Function Name  : processEncoderNode
 * Inputs         : encoderQueueNode_p - Node popped from encoder queue 
 *                                       containing details of encoding request.
 *                  dciCCEInfoContainerIndex - Index of 
 *                                             dciCCEContainerInfo_g
 *                  ueContext_p - pointer to UE context.
 *                  internalCellIndex - Cell-Indexc at MAC
 *                                   
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called by processEncoderQueue per popped
 *                  encoder queue node ,each containing encoding request.
 *****************************************************************************/
MacRetType processEncoderNode( EncoderQueueNode* encoderQueueNode_p,
                               UInt32 dciCCEInfoContainerIndex ,
                               DLUEContext* ueContext_p,
                               InternalCellIndex internalCellIndex
                               /* + SPR_14039 */
#ifdef TDD_CONFIG                               
                               /* SPR 15909 fix start */
                               ,tickType_t scheduledTick
                               /* SPR 15909 fix end */
#endif                               
                               /* - SPR_14039 */
                               )
{
    TransmissionAndRLCData * transmissionAndRLCData_p =
                                encoderQueueNode_p->transmissionAndRLCData_p;
    DLHARQProcess * hProcess_p = PNULL;
    DLHARQTBInfo*       harqProcessTBOne_p = PNULL;
    DLHARQTBInfo*       harqProcessTBTwo_p = PNULL;
    TBPopulationStruct tbPopulationStruct = {0};
    DciCCEInfo* dciCCEInfo_p = PNULL;

#if defined(KPI_STATS) || defined(PERF_STATS) 
    /* +GCC 4.7 */
#ifdef KPI_STATS 
    LteMacKPIStats  *pLteMacKPIStats       = PNULL;
    /* -GCC 4.7 */
    UInt32          activeIndexForMacStats = 0;
#endif
    UInt8           lcCountForKpiStats     = 0;
    UInt8           lcId                   = 0; 
    UInt8           qci                    = 0;
/* L2_PERFORMANCE_OPT_CHANGES_START */
    UInt32          qciBitsInTB        = 0;
    UInt32          tbSizeInBits       = 0;
    UInt64          numRBs             = 0;
    UInt32          qciPerTBSize       = 0;
/* L2_PERFORMANCE_OPT_CHANGES_END */
#endif
    /* + SPR 11469 Changes */
    
    if(IS_PCELL_OF_UE(ueContext_p,internalCellIndex ))
    {
        hProcess_p = &(ueContext_p->dlHarqContext_p->
                       harqProcess[encoderQueueNode_p->harqProcessId]);
    }
    else
    {
        hProcess_p = &(ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p->
                       harqProcess[encoderQueueNode_p->harqProcessId]);
    }
    /* - SPR 11469 Changes */

    harqProcessTBOne_p =  &(hProcess_p->dlHARQTBOneInfo);
    harqProcessTBTwo_p =  &(hProcess_p->dlHARQTBTwoInfo);
    {	
    dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciCCEInfoContainerIndex)->
                       dciCCEInfo[encoderQueueNode_p->cceIndex]);
    }

    LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Encoding Request for UEID %d harqId %d",
             __func__,
             ueContext_p->crnti,hProcess_p->harqProcessId);

    if (hProcess_p->isTBOneValid)
    {
        if (hProcess_p->isTBTwoValid)
        {
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Both TBs are Valid",__func__);

#if defined(KPI_STATS) || defined(PERF_STATS)

   /* + SPR_5711 */
#ifdef KPI_STATS 
		/* + KPI_CA */
	    activeIndexForMacStats = (gMacStats.
		    pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
	    pLteMacKPIStats = &(gMacStats.
		    pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
		/* - KPI_CA */
#endif
	    for ( lcCountForKpiStats = 0;
		    lcCountForKpiStats < transmissionAndRLCData_p->lcCountTBOne;
		    lcCountForKpiStats++ )
	    {
		lcId = transmissionAndRLCData_p->rlcDataBufferArr[lcCountForKpiStats].lcID;
                /* 32.425 Events code changes start*/
#ifdef KPI_STATS
                /* Update the Common Index and pdcpBytes from the Global Struct at Harq Process */
                lteMacUpdateKpiStatsIPThpDlOnEnQueue(lcId,encoderQueueNode_p->ueIndex,
                        lcCountForKpiStats,transmissionAndRLCData_p ,hProcess_p);
#endif
                /* 32.425 Events code changes end*/
                qciBitsInTB = transmissionAndRLCData_p->rlcDataBufferArr
		    [lcCountForKpiStats].totalPduLen<<3;
		tbSizeInBits = harqProcessTBOne_p->tbSize<<3;
		numRBs = (hProcess_p->assignedResourcesNB>>1); 

		qciPerTBSize = ((qciBitsInTB<<SCALE_UP_MULTIPLYING_FACTOR)/tbSizeInBits);

#ifdef PERF_STATS
		if (harqProcessTBOne_p->current_TX_NB == 0)
		{
		    /* EVENT : EVENT_PARAM_DCCH_CCCH_DL_RES The total number of PRB for HARQ initial transmissions 
		     * used for Signaling Radio Bearers (SRB) in the downlink*/
		    if (SRB_1_LCID == lcId)
		    {
			/* + PERF_CA */
			gMacCellPerfStats_p[internalCellIndex]->lteCellMacDLPerfStats.totalDLPRBUsageSRB[SRB_1_LCID] += 
                                                                                 numRBs*qciPerTBSize;
		    }
		    if (SRB_2_LCID == lcId)
		    {
			gMacCellPerfStats_p[internalCellIndex]->lteCellMacDLPerfStats.totalDLPRBUsageSRB[SRB_2_LCID] +=
			/* - PERF_CA */
                                                                                 numRBs*qciPerTBSize; 
		    }
		}
#endif
		  qci = ueContext_p->logicalChannel[lcId].dlLcQosInfo.QCI;
		/* SPR 5469 : FIX start */ 
		  if(qci)
		  {
		    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
		    /* SPR 5462 : FIX start */ 
		       if (harqProcessTBOne_p->current_TX_NB == 0)
		       {
			      /* + PERF_CA */
			      gMacCellPerfStats_p[internalCellIndex]->lteCellMacDLPerfStats. \
			      totalDLPRBUsageforHarq[qci-1] += numRBs;
			      /* - PERF_CA */
		       }
		    /* SPR 5462 : FIX end */
#endif
		    /* - PERF_STATS_PH1D1 */            

		    /* - SPR_5711 */
#ifdef KPI_STATS

		        pLteMacKPIStats->totalDLPRBUsagePerQCI[qci-1] += numRBs*qciPerTBSize;
                /* SPR 23402 Changes Start */
                /*MOCN SPR 21913 Start*/                                                                      
                ueContext_p->qciPerTBSize[qci-1] = (UInt64)numRBs*qciPerTBSize;
                /*MOCN SPR 21913 End*/                                                                      
                /* MOCN Changes Start */
                if(mocnOperatorParams_g.isMocnEnabled && (ueContext_p->operatorId < mocnOperatorParams_g.numOfOperator))
                {    
                    pLteMacKPIStats->totalDLPRBUsagePerOperatorPerQCI[ueContext_p->operatorId][qci-1] += numRBs*qciPerTBSize;
                    /*MOCN SPR 21913 Start*/                                                                      
                    ueContext_p->qciPerTBSizePerOperatorPerQCI[ueContext_p->operatorId][qci-1] = (UInt64)numRBs*qciPerTBSize;
                    /*MOCN SPR 21913 End*/                                                                      

                }    
                /* MOCN Changes End */
                /* SPR 23402 Changes End */
#endif

/* EICIC +*/
/* Check if user is victim then increment the per QCI RBs usage*/
               /*Complexity Start*/
                updateABSperQciUsage(internalCellIndex,ueContext_p,numRBs,lcId);
               /* SPR 16916 FIX START */
               /*Complexity End*/
#ifdef KPI_STATS
                    if(0 == ueContext_p->logicalChannel[lcId].queueLoad)
                    {
                        ueContext_p->zeroQloadFlag[qci-1][hProcess_p->harqProcessId] = TRUE;
                            LOG_MAC_MSG (MAC_DL_ACTIVE_UE_QCI_LOG_ID, LOGDEBUG,
                                    MAC_DL_Strategy, getCurrentTick (), 
                                    ueContext_p->ueIndex,lcId,qci,
                                    hProcess_p->harqProcessId,
                                    gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci-1],
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "SetDlZeroQloadFlag");
                    }
#endif
                    /* SPR 16916 FIX END */

          }
                /* SPR 5469 : FIX end */ 
        }

            for (lcCountForKpiStats = 0; lcCountForKpiStats < transmissionAndRLCData_p->lcCountTBTwo;
                  lcCountForKpiStats++ )
            {
                lcId = transmissionAndRLCData_p->rlcDataBufferArr[lcCountForKpiStats].lcID;
                /* 32.425 Events code changes start*/
#ifdef KPI_STATS
                /* Update the Common Index and pdcpBytes from the Global Struct at Harq Process */
                lteMacUpdateKpiStatsIPThpDlOnEnQueue(lcId,encoderQueueNode_p->ueIndex,
                        lcCountForKpiStats,transmissionAndRLCData_p ,hProcess_p);
#endif
                /* 32.425 Events code changes end*/
                    qciBitsInTB = transmissionAndRLCData_p->rlcDataBufferArr
                        [lcCountForKpiStats].totalPduLen<<3;
                    tbSizeInBits = harqProcessTBTwo_p->tbSize<<3;
                    numRBs = (hProcess_p->assignedResourcesNB>>1); 

                qciPerTBSize = ((qciBitsInTB<<SCALE_UP_MULTIPLYING_FACTOR)/tbSizeInBits);

    /*  + SPR_5711 */
#ifdef PERF_STATS
                /* EVENT : EVENT_PARAM_DCCH_CCCH_DL_RES The total number of PRB for HARQ initial transmissions 
                * used for Signaling Radio Bearers (SRB) in the downlink*/

                if (harqProcessTBTwo_p->current_TX_NB == 0)
		{  
		    if (SRB_1_LCID == lcId)
		    {
			/* + PERF_CA */
			gMacCellPerfStats_p[internalCellIndex]->lteCellMacDLPerfStats.totalDLPRBUsageSRB[SRB_1_LCID] += 
			    numRBs*qciPerTBSize;
		    }
		    if (SRB_2_LCID == lcId)
		    {
			gMacCellPerfStats_p[internalCellIndex]->lteCellMacDLPerfStats.totalDLPRBUsageSRB[SRB_2_LCID] += 
			    numRBs*qciPerTBSize;
			/* - PERF_CA */
		    }
		}   
#endif      
    /* - SPR_5711 */
                qci = ueContext_p->logicalChannel[lcId].dlLcQosInfo.QCI;

                /* SPR 5469 : FIX start */ 
                if(qci)
                {
 
                /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                /* SPR 5462 : FIX start */ 
                if (harqProcessTBTwo_p->current_TX_NB == 0)
                {
					/* + PERF_CA */
                    gMacCellPerfStats_p[internalCellIndex]->lteCellMacDLPerfStats. \
                                      totalDLPRBUsageforHarq[qci-1] += numRBs;
					/* - PERF_CA */
                }
                /* SPR 5462 : FIX end */ 
#endif
                /* - PERF_STATS_PH1D1 */            
 
#ifdef KPI_STATS
                pLteMacKPIStats->totalDLPRBUsagePerQCI[qci-1] += numRBs*qciPerTBSize;
                /* SPR 23402 Changes Start */
                /*MOCN SPR 21913 Start*/                                                                      
                ueContext_p->qciPerTBSize[qci-1] = (UInt64)numRBs*qciPerTBSize;
                /*MOCN SPR 21913 End*/                                                                      
                /* MOCN Changes Start */
                if(mocnOperatorParams_g.isMocnEnabled && (ueContext_p->operatorId < mocnOperatorParams_g.numOfOperator))
                {    
                    pLteMacKPIStats->totalDLPRBUsagePerOperatorPerQCI[ueContext_p->operatorId][qci-1] += numRBs*qciPerTBSize;
                    /*MOCN SPR 21913 Start*/                                                                      
                    ueContext_p->qciPerTBSizePerOperatorPerQCI[ueContext_p->operatorId][qci-1] = (UInt64)numRBs*qciPerTBSize;
                    /*MOCN SPR 21913 End*/                                                                      
                }
                /* MOCN Changes End */
                /* SPR 23402 Changes End */
#endif
/* EICIC +*/
/* Check if user is victim then increment the per QCI RBs usage*/
              /*Complexity Start*/
               updateABSperQciUsage(internalCellIndex,ueContext_p,numRBs,lcId);
              /*Complexity End*/
                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                    if(0 == ueContext_p->logicalChannel[lcId].queueLoad)
                    {
                        ueContext_p->zeroQloadFlag[qci-1][hProcess_p->harqProcessId] = TRUE;
                            LOG_MAC_MSG (MAC_DL_ACTIVE_UE_QCI_LOG_ID, LOGDEBUG,
                                    MAC_DL_Strategy, getCurrentTick (), 
                                    ueContext_p->ueIndex,lcId,qci,
                                    hProcess_p->harqProcessId,
                                    gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci-1],
                                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "SetDlZeroQloadFlag");
                    }
#endif
                    /* SPR 16916 FIX END */
	 
/* EICIC -*/	
                }
                /* SPR 5469 : FIX end */ 
            }
#endif

            return encodeSpatialMuxNode( hProcess_p,
                                         dciCCEInfo_p,
                                         encoderQueueNode_p->transmissionAndRLCData_p,
                                         ueContext_p,
                                         internalCellIndex
                                       /* + SPR_14039 */
#ifdef TDD_CONFIG 
                                         ,scheduledTick
#endif                                         
                                      /* - SPR_14039 */
                                         );
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Only TB 1 Valid",__func__);
            /* COVERITY 10901 */
            /* Only TB 1 is valid*/
            if (transmissionAndRLCData_p)
            {
                tbPopulationStruct.rlcDataBuffer_p = 
                                    transmissionAndRLCData_p->rlcDataBufferArr;
                transmissionAndRLCData_p->tbTwoLCStartIndex =
                     tbPopulationStruct.lcCount = transmissionAndRLCData_p->
                                                               lcCountTBOne;
                tbPopulationStruct.padding = 
                                (transmissionAndRLCData_p->paddingBytesForTBOne);
                               
                tbPopulationStruct.rlcBufferStartIndex = 0;
            
#if defined(KPI_STATS) || defined(PERF_STATS)
/* + SPR_5711 */
#ifdef KPI_STATS
                /* SPR 2953 Changes Starts*/
				/* + KPI_CA */
                activeIndexForMacStats = (gMacStats.
                        pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
                pLteMacKPIStats = &(gMacStats.
                        pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
				/* - KPI_CA */
                /* SPR 2953 Changes Ends*/
#endif 
                for (lcCountForKpiStats = 0 ;
                        lcCountForKpiStats < transmissionAndRLCData_p->lcCountTBOne;
                        lcCountForKpiStats++ )
                {
                    lcId = transmissionAndRLCData_p->rlcDataBufferArr[lcCountForKpiStats].lcID;
                    /* 32.425 Events code changes start*/
#ifdef KPI_STATS
                    /* Update the Common Index and pdcpBytes from the Global Struct at Harq Process */
                    lteMacUpdateKpiStatsIPThpDlOnEnQueue(lcId,encoderQueueNode_p->ueIndex,
                            lcCountForKpiStats,transmissionAndRLCData_p ,hProcess_p);
#endif
                    /* 32.425 Events code changes end*/
                    
                    qciBitsInTB = transmissionAndRLCData_p->rlcDataBufferArr
                            [lcCountForKpiStats].totalPduLen<<3;
                    tbSizeInBits = harqProcessTBOne_p->tbSize<<3;
                    numRBs = (hProcess_p->assignedResourcesNB); 
                    qciPerTBSize = ((qciBitsInTB<<SCALE_UP_MULTIPLYING_FACTOR)/tbSizeInBits);


#ifdef PERF_STATS 
		    /* EVENT : EVENT_PARAM_DCCH_CCCH_DL_RES The total number of PRB for HARQ initial transmissions 
		     * used for Signaling Radio Bearers (SRB) in the downlink*/

                    if (harqProcessTBOne_p->current_TX_NB == 0)
		    {
			if (SRB_1_LCID == lcId)
			{
				/* + PERF_CA */
			    gMacCellPerfStats_p[internalCellIndex]->lteCellMacDLPerfStats.totalDLPRBUsageSRB[SRB_1_LCID] += 
                                                                                    numRBs*qciPerTBSize;
			}
			if (SRB_2_LCID == lcId)
			{
			    gMacCellPerfStats_p[internalCellIndex]->lteCellMacDLPerfStats.totalDLPRBUsageSRB[SRB_2_LCID] += 
				/* - PERF_CA */
                                                                                    numRBs*qciPerTBSize;
			}
                    }  
#endif
		    qci = ueContext_p->logicalChannel[lcId].dlLcQosInfo.QCI;
                    /* SPR 5469 : FIX start */ 
                    if(qci)
                    {
                      /* + PERF_STATS_PH1D1 */
/* - SPR_5711 */
#ifdef PERF_STATS
                        /* SPR 5462 : FIX start */ 
                        if (harqProcessTBOne_p->current_TX_NB == 0)
                        {
							/* + PERF_CA */
                            gMacCellPerfStats_p[internalCellIndex]->lteCellMacDLPerfStats.\
                                totalDLPRBUsageforHarq[qci-1] += numRBs;
							/* - PERF_CA */
                        }
                        /* SPR 5462 : FIX end */ 
#endif
                        /* - PERF_STATS_PH1D1 */            

#ifdef KPI_STATS
                        /* SPR 2953 Changes Starts*/
                        /* SPR 2953 Changes Ends*/
                        pLteMacKPIStats->totalDLPRBUsagePerQCI[qci-1] += numRBs*qciPerTBSize;
                        /* SPR 23402 Changes Start */
                        /*MOCN SPR 21913 Start*/                                                                      
                        ueContext_p->qciPerTBSize[qci-1] = (UInt64)numRBs*qciPerTBSize;
                        /*MOCN SPR 21913 End*/                                                                      
                        /* MOCN Changes Start */
                        if(mocnOperatorParams_g.isMocnEnabled && (ueContext_p->operatorId < mocnOperatorParams_g.numOfOperator))
                        {    
                            pLteMacKPIStats->totalDLPRBUsagePerOperatorPerQCI[ueContext_p->operatorId][qci-1] += numRBs*qciPerTBSize;
                            /*MOCN SPR 21913 Start*/                                                                      
                            ueContext_p->qciPerTBSizePerOperatorPerQCI[ueContext_p->operatorId][qci-1] = (UInt64)numRBs*qciPerTBSize;
                            /*MOCN SPR 21913 End*/                                                                      
                        }    
                        /* MOCN Changes End */
                        /* SPR 23402 Changes End */
#endif
                        /* SPR 16916 FIX START */
#ifdef KPI_STATS
                        if(0 == ueContext_p->logicalChannel[lcId].queueLoad)
                        {
                            ueContext_p->zeroQloadFlag[qci-1][hProcess_p->harqProcessId] = TRUE;
                                LOG_MAC_MSG (MAC_DL_ACTIVE_UE_QCI_LOG_ID, LOGDEBUG, 
                                        MAC_DL_Strategy, getCurrentTick (), 
                                        ueContext_p->ueIndex,lcId,qci,
                                        hProcess_p->harqProcessId,
                                        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci-1],
                                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "SetDlZeroQloadFlag");
                        }
#endif
                        /* SPR 16916 FIX END */

/* EICIC +*/
/* Check if user is victim then increment the per QCI RBs usage*/
                       /*Complexity Start*/
                        updateABSperQciUsage(internalCellIndex,ueContext_p,numRBs,lcId);
                       /*Complexity End*/
/* EICIC -*/
                    /* SPR 16916 FIX START */
                        /* +- */
                    /* SPR 16916 FIX END */    
                    }
                    /* SPR 5469 : FIX end */ 
                }
#endif 
            }
            /* COVERITY 10901 */

            tbPopulationStruct.harqTB_p = harqProcessTBOne_p;
            return encodeNonSpatialMuxNode(hProcess_p,
                                           dciCCEInfo_p,
                                           transmissionAndRLCData_p,
                                           ueContext_p,
                                           &hProcess_p->dlHARQTBOneInfo,
                                           &tbPopulationStruct,
                                           /* SPR 431 changes start */
                                           TB_ONE,
                                           /* SPR 431 changes end */
                                           internalCellIndex
                                          /* + SPR_14039 */
#ifdef TDD_CONFIG                                           
                                           ,scheduledTick
#endif                                     
                                          /* - SPR_14039 */
                                           );
        }
    }
    else if (hProcess_p->isTBTwoValid)
    {
        /* COVERITY 10930 */
        /* Only  TB 2 is valid*/
        if (transmissionAndRLCData_p)
        {
        /* COVERITY 10930 */
#if defined(KPI_STATS) || defined(PERF_STATS)        
/* + SPR_5711 */ 
#ifdef KPI_STATS
			/* + KPI_CA */
            activeIndexForMacStats = (gMacStats.
                    pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
            pLteMacKPIStats = &(gMacStats.
                    pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
			/* - KPI_CA */
#endif
/* - SPR_5711 */ 
        for ( lcCountForKpiStats = 0;
              lcCountForKpiStats < transmissionAndRLCData_p->lcCountTBTwo;
              lcCountForKpiStats++ )
        {
            lcId = transmissionAndRLCData_p->rlcDataBufferArr[lcCountForKpiStats].lcID;
            qci = ueContext_p->logicalChannel[lcId].dlLcQosInfo.QCI;

            /* 32.425 Events code changes start*/
#ifdef KPI_STATS
            /* Update the Common Index and pdcpBytes from the Global Struct at Harq Process */
            lteMacUpdateKpiStatsIPThpDlOnEnQueue(lcId,encoderQueueNode_p->ueIndex,
                    lcCountForKpiStats,transmissionAndRLCData_p ,hProcess_p);
#endif
            /* 32.425 Events code changes end*/

            /* SPR 5469 : FIX start */ 
            if(qci)
            {
                qciBitsInTB = transmissionAndRLCData_p->rlcDataBufferArr
                    [lcCountForKpiStats].totalPduLen<<3;
                tbSizeInBits = harqProcessTBTwo_p->tbSize<<3;
                numRBs = (hProcess_p->assignedResourcesNB); 

                qciPerTBSize = ((qciBitsInTB<<SCALE_UP_MULTIPLYING_FACTOR)/tbSizeInBits);

                /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                /* SPR 5462 : FIX start */ 
                if (harqProcessTBTwo_p->current_TX_NB == 0)
                {
					/* + PERF_CA */
                    gMacCellPerfStats_p[internalCellIndex]->lteCellMacDLPerfStats. \
                        totalDLPRBUsageforHarq[qci-1] += numRBs;
					/* - PERF_CA */
                }
                /* SPR 5462 : FIX end */ 
#endif
                /* - PERF_STATS_PH1D1 */            

#ifdef KPI_STATS           
                pLteMacKPIStats->totalDLPRBUsagePerQCI[qci-1] += numRBs*qciPerTBSize;
                /* SPR 23402 Changes Start */
                /*MOCN SPR 21913 Start*/                                                                      
                ueContext_p->qciPerTBSize[qci-1] = (UInt64)numRBs*qciPerTBSize;
                /*MOCN SPR 21913 End*/                                                                      
                /* MOCN Changes Start */
                if(mocnOperatorParams_g.isMocnEnabled && (ueContext_p->operatorId < mocnOperatorParams_g.numOfOperator))
                {
                    pLteMacKPIStats->totalDLPRBUsagePerOperatorPerQCI[ueContext_p->operatorId][qci-1] += numRBs*qciPerTBSize;
                    /*MOCN SPR 21913 Start*/                                                                      
                    ueContext_p->qciPerTBSizePerOperatorPerQCI[ueContext_p->operatorId][qci-1] = (UInt64)numRBs*qciPerTBSize;
                    /*MOCN SPR 21913 End*/                                                                      
                }
                /* MOCN Changes End */
                /* SPR 23402 Changes End */
#endif
/* EICIC +*/
/* Check if user is victim then increment the per QCI RBs usage*/
              /*Complexity Start*/
              updateABSperQciUsage(internalCellIndex,ueContext_p,numRBs,lcId);
              /*Complexity End*/
/* EICIC -*/
                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                if(0 == ueContext_p->logicalChannel[lcId].queueLoad)
                {
                    ueContext_p->zeroQloadFlag[qci-1][hProcess_p->harqProcessId] = TRUE;
                        LOG_MAC_MSG (MAC_DL_ACTIVE_UE_QCI_LOG_ID, LOGDEBUG, 
                                MAC_DL_Strategy, getCurrentTick (), 
                                ueContext_p->ueIndex,lcId,qci,
                                hProcess_p->harqProcessId,
                                gMacStats.pStats->lteCommonKPIStats[internalCellIndex].totalActiveUsersPerQciDL[qci-1],
                                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "SetDlZeroQloadFlag");
                }
#endif
                /* SPR 16916 FIX END */

            }
            /* SPR 5469 : FIX end */ 
        }
#endif
            tbPopulationStruct.lcCount = 
                            transmissionAndRLCData_p->lcCountTBTwo;
            tbPopulationStruct.padding = 
                            (transmissionAndRLCData_p->paddingBytesForTBTwo);

            tbPopulationStruct.rlcBufferStartIndex = 
                             transmissionAndRLCData_p->tbTwoLCStartIndex;
        }
        tbPopulationStruct.harqTB_p = harqProcessTBTwo_p;

        return encodeNonSpatialMuxNode(hProcess_p,
                                       dciCCEInfo_p,
                                       transmissionAndRLCData_p,
                                       ueContext_p,
                                       &hProcess_p->dlHARQTBTwoInfo,
                                       &tbPopulationStruct,
                                       /* SPR 431 changes start */
                                       TB_TWO,
                                       /* SPR 431 changes end */
                                       internalCellIndex
                                       /* + SPR_14039 */
#ifdef TDD_CONFIG                                       
                                       ,scheduledTick
#endif                                       
                                       /* - SPR_14039 */
                                       );
    }
    return MAC_FAILURE;
}


/*****************************************************************************
 * Function Name  : processEncoderQueue
 * Inputs         : currentTick - Present value of global Tick.
 *                  delayToPhy -  Delay to PHY layer from Encoder module.
 *                  systemFrameNum - Present system frame number.
 *                  subFrameNumber - Present sub frame number.
 *                  isTxFunctionalityAdded - If this is true encoder itself
 *                                           sends the TB.
 *                  encoderThdCount - Identifies execution scheduler flow
 *                                    index.
 *                  internalCellIndex - Cell-Indexc at MAC
 *                  rlcCellIndex - Cell-Indexc at RLC
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion is called by execution scheduler flow to
 *                  execute encoding functionality.This function processes all
 *                  the encoding requests present encoder queue corresponding 
 *                  to particular container identified by encoderContainerIndex.
 *****************************************************************************/
/** CA Phase2 Changes Start **/
/** CA Phase2 Changes End **/
                        /* SPR 15909 fix start */
void processEncoderQueue(tickType_t currentTick,
                        /* SPR 15909 fix end */
                         UInt32 delayToPhy,
                         /* +- SPR 17777 */
                         UInt32 subFrameNumber,
                         UInt32 isTxFunctionalityAdded,
                         /* +- SPR 17777 */
                         InternalCellIndex internalCellIndex,
/* + SPR 11209 Changes */
                         InternalCellIndex rlcCellIndex)
/* - SPR 11209 Changes */
{
/* SPR 8712 changes */
/* Stage-III SCELL Deactivation Timer Related Changes Start */
DLUEContext           *ueDLContext_p          = PNULL;
/* Stage-III SCELL Deactivation Timer Related Changes End */
    EncoderQueueNode *encoderQueueNode_p = PNULL;
    LTE_SQUEUE       *encoderReTxQueue_p = PNULL;
#ifndef DL_UL_SPLIT
    /* dummy node fix start */
    EncoderQueueNode *nextEncoderQueueNode_p = PNULL;
    /* dummy node fix end */
    LTE_SQUEUE       *encoderNewTxQueue_p = PNULL;
#elif CIRC_QUEUE_IMPL
    LTE_CIRCQUEUE       *encoderNewTxQueue_p = PNULL;
#endif

    UInt32 encoderContainerIndex = 0;
    UInt32 dciContainerIndex = 0;
    DLUEContext * ueContext_p = PNULL;
    UInt32 lcCount = 0;
    UInt32 maxLcCount = 0;
    UInt8 * cceIndexArr_p = PNULL;
    UInt8 * writeIndex_p = PNULL;
/*  RLCDataBuffer *lcInfo_p = PNULL; TDD Warning Fix */
    DciCCEInfo* dciCCEInfo_p = PNULL;
    DLHARQProcess * hProcess_p = PNULL;
    UInt32 harqSubFrameNum = 0;
    /* +- SPR 17777 */
    EncoderNode *encoderNode_p = PNULL;

    /* SPR 15909 fix start */
    tickType_t currentTickWithDelay = currentTick + delayToPhy;
    /* SPR 15909 fix end */
/* SPR 15279 */
    UInt32 nodeCount = 0;
/* SPR 15279 */
    harqSubFrameNum = subFrameNumber + delayToPhy + 4;

    if (harqSubFrameNum >= NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME)
    {
        harqSubFrameNum -= NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    }
    /* +- SPR 17777 */

    dciContainerIndex = currentTickWithDelay % MAX_CCE_CONTAINER;
    encoderContainerIndex = dciContainerIndex;

/* + SPR 11209 Changes */
    encoderNode_p = &((encoderContainerArr_gp[internalCellIndex][rlcCellIndex] + encoderContainerIndex)->
                               encoderNode); 
/* - SPR 11209 Changes */

    cceIndexArr_p = encoderNode_p->cceIndexArr; 

    encoderNode_p->writeIndex = 0;
    encoderNode_p->readIndex = 0;

    writeIndex_p = &(encoderNode_p->writeIndex);

    encoderReTxQueue_p = &(encoderNode_p->encoderQueueForScheduler);

#ifndef DL_UL_SPLIT
    encoderNewTxQueue_p = &(encoderNode_p->encoderQueue);
#elif CIRC_QUEUE_IMPL
    encoderNewTxQueue_p = &(encoderNode_p->encoderCircQueue);
#endif


    /* Process encoder nodes one by one from Encoder Retransmission Queue
       and Encoder New Transmission Queue till dummy node is found or 
       required queue count is reached.*/  
#ifndef DL_UL_SPLIT
    while (1)
    {
/* SPR 15279 */
#endif            
        nodeCount = sQueueCount(encoderReTxQueue_p);
        while(nodeCount--)
        {
/* SPR 15279 */
            encoderQueueNode_p = (EncoderQueueNode *)popNode(encoderReTxQueue_p);
            if (encoderQueueNode_p != PNULL)
            {
                if (MAC_SUCCESS == processEncoderReTxNode(encoderQueueNode_p, 
                                                          currentTick,
                                                          delayToPhy,
                                                          dciContainerIndex,
                                                          internalCellIndex
/* + SPR 15127 changes */
#ifdef TDD_CONFIG
                                                          , subFrameNumber
#endif

/* - SPR 15127 changes */
                                                          ))
                {
                    /* +- SPR 17777 */
                    populateTXInfo(
                                    isTxFunctionalityAdded,
                                    encoderQueueNode_p->cceIndex,
                                    cceIndexArr_p,
                                    writeIndex_p
                                    );
                    /* +- SPR 17777 */
                    /* SPR 14954 Fix Start */
                    dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].dlUEContext_p->advlastScheduledTick = 
                    currentTick + delayToPhy;
                    /* SPR 14954 Fix End */
                    /* Stage-III SCELL Deactivation Timer Related Changes Start */
                    ueDLContext_p    = dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].dlUEContext_p;
                    if(ueDLContext_p)
                    {
                        scellStateMngrUpdateScellDeactTimer(ueDLContext_p,internalCellIndex);
                    }
                /* Stage-III SCELL Deactivation Timer Related Changes End */
                }
                freeMemPool((void *)encoderQueueNode_p);
                encoderQueueNode_p = PNULL;
            }
/* SPR 15279 */
        }
/* SPR 15279 */
#ifndef DL_UL_SPLIT        
        encoderQueueNode_p = (EncoderQueueNode *)popNode(encoderNewTxQueue_p);
#else
    while (1)
    {        
#ifdef CIRC_QUEUE_IMPL
        semWaitDefault(&waitForEncoderQueueInd_g[internalCellIndex]);
        popNodeCircQ(encoderNewTxQueue_p, (void *)&encoderQueueNode_p);
#else
        /* +- SPR 17777 */
        recvMsgWithIccQ(encoderNode_p->encoderQueueIdDL, (const void *)&encoderQueueNode_p);
        /* +- SPR 17777 */
#endif   
#endif   
/*SPR 11844 fix start*/     
        if (PNULL == encoderQueueNode_p) 
        {
            lteWarning("[%s] [%u] no dummy node from other cell expected! Shouldnt be here :(.  encCell[%d] rlcCell[%d]\n", __func__, currentTick, internalCellIndex, rlcCellIndex);
            break;
            /* - Critical section changes */
        }
/*SPR 11844 fix end*/     
        /*Coverity 64287 Dereference Null Return Value Check Change starts*/ 
        if ((encoderQueueNode_p) && (!encoderQueueNode_p->dummyNode))
        /*Coverity 64287 Dereference Null Return Value Check Change ends*/ 
        {

            if ((dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].pendingDeleteFlag))
            {
		{
                dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                               dciCCEInfo[encoderQueueNode_p->cceIndex]);
/*mandeep changes start*/
/*mandeep changes stop*/
                mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                        dciCCEInfo_p,internalCellIndex);
                /*pdcch for scell changes start*/
                mac_reset_extra_cce(dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex,dciCCEInfo_p,
                dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].dlUEContext_p,internalCellIndex);
		}
                /*pdcch for scell changes end*/
                lteWarning(" %s ueContext_p is null, Breaking from ENCODER "
                        "LOOP on receiving dummy Node\n",__func__);
                freeMemPool((void *)encoderQueueNode_p);
                /* + SPR#7373 */
                continue;
                /* - SPR#7373 */
            }

            ueContext_p = dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].
                                                                dlUEContext_p;

            if (PNULL == ueContext_p)
            {
                LOG_MAC_MSG(MAC_DLUECONTEXT_NOT_FOUND_ID,
                        LOGERROR,
                        MAC_RRC_INF,
                        getCurrentTick(),
                        encoderQueueNode_p->ueIndex,
                        LINE_NUMBER,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "MAC_DLUECONTEXT_NOT_FOUND");
                /* +COVERITY 10746 */
		{
                dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                                 dciCCEInfo[encoderQueueNode_p->cceIndex]);

/*mandeep changes start*/
/*mandeep changes stop*/
                mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                         dciCCEInfo_p,internalCellIndex);
                /*pdcch for scell changes start*/
                mac_reset_extra_cce(dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex,dciCCEInfo_p,
                dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].dlUEContext_p,internalCellIndex);
		}
                /*pdcch for scell changes end*/
                freeMemPool((void *)encoderQueueNode_p);
                encoderQueueNode_p = PNULL;
                continue;
                /* -COVERITY 10746 */
            }
            if (encoderQueueNode_p->scheduledTick != currentTickWithDelay)
            {
		{
                dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                               dciCCEInfo[encoderQueueNode_p->cceIndex]);
/*mandeep changes start*/
/*mandeep changes stop*/
		}
                if (encoderQueueNode_p->transmissionAndRLCData_p)
                {
                    maxLcCount = ((TransmissionAndRLCData*)encoderQueueNode_p->
                            transmissionAndRLCData_p)->lcCountTBOne + 
                                 ((TransmissionAndRLCData*)encoderQueueNode_p->
                             transmissionAndRLCData_p)->lcCountTBTwo;

             /*    lcInfo_p = (RLCDataBuffer*) encoderQueueNode_p->
                           transmissionAndRLCData_p; TDD Warning Fix */
                    lcCount = 0;
                    while (lcCount < maxLcCount)
                    {
                        lcCount++;
                    }
                }
                lteWarning("%s from Mux Tick mismatch schtick %d curr tick %d\n",
                        __func__,encoderQueueNode_p->scheduledTick,
                        currentTickWithDelay);

                /* + SPR 11469 Changes */
                if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex))
                {                
                    hProcess_p = &(ueContext_p->dlHarqContext_p->
                             harqProcess[encoderQueueNode_p->harqProcessId]);
                }
                else
                {
                    hProcess_p = &(ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p->
                             harqProcess[encoderQueueNode_p->harqProcessId]);
                }
                /* - SPR 11469 Changes */

		{
                if (MAC_FAILURE == resetHARQIfReTx(hProcess_p,
                                                   ueContext_p,
                                                   dciCCEInfo_p,
                                                   internalCellIndex))
                {
                    mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                                  dciCCEInfo_p,internalCellIndex);
                    /*pdcch for scell changes start*/
                    mac_reset_extra_cce(dciCCEContainerInfo_gp[internalCellIndex]
                            + dciContainerIndex, 
                            dciCCEInfo_p,
                            dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].dlUEContext_p,
                            internalCellIndex);
                    /*pdcch for scell changes end*/
                    freeMemPool((void *)encoderQueueNode_p);
                    encoderQueueNode_p = PNULL;
                    continue;
                }
		}
            }
            else
            {
                if (MAC_FAILURE == processEncoderNode( encoderQueueNode_p,
                                                       dciContainerIndex,
                                                       ueContext_p,
                                                       internalCellIndex
                                                       /* + SPR_14039 */
#ifdef TDD_CONFIG
                                                       ,(currentTick + delayToPhy) 
#endif                                                       
                                                       /* - SPR_14039 */
                                                       )) 
                {
                    freeMemPool((void *)encoderQueueNode_p);
                    encoderQueueNode_p = PNULL;
                    continue;        
                }
                /* Stage-III SCELL Deactivation Timer Related Changes Start */
                else
                {
                    scellStateMngrUpdateScellDeactTimer(ueContext_p,internalCellIndex);
                }
                /* Stage-III SCELL Deactivation Timer Related Changes End */
            }

            /* +- SPR 17777 */
            populateTXInfo(
                            isTxFunctionalityAdded,
                            encoderQueueNode_p->cceIndex,
                            cceIndexArr_p,
                            writeIndex_p
                           );
            /* +- SPR 17777 */
            /* SPR 14954 Fix Start */
            ueContext_p->advlastScheduledTick = currentTick + delayToPhy;
            /* SPR 14954 Fix End */
        }
        else
        {
            /*free the memory of dummy node*/    
               /*6.0 DL_UL_SPLIT Fix, adding the below #ifdef*/ 
#ifndef DL_UL_SPLIT
            /* + SPR 22269 fix */
            while(PNULL != (nextEncoderQueueNode_p = (EncoderQueueNode *)popNode(encoderNewTxQueue_p)) )
	    {
                freeMemPool(nextEncoderQueueNode_p);
		nextEncoderQueueNode_p = PNULL;
	    }
#endif            
	    freeMemPool((void *)encoderQueueNode_p);
	    encoderQueueNode_p = PNULL;
            /* - SPR 22269 fix */
            break;
        }
        freeMemPool((void *)encoderQueueNode_p);
        encoderQueueNode_p = PNULL;
    }

    /* TO be reset by transmitter , uncomment if enhancement added in tx*/
    cceIndexArr_p[(*writeIndex_p)++] = INVALID_CCE_INDEX;
/* SPR 8712 changes */
}

/*****************************************************************************
 * Function Name  : putEntryInEncoderQueue 
 * Inputs         : ueIndex - The index of the UE context of the UE being
 *                            processed in the global UE context array,
 *                  lcCount - Total numbers of LC's multiplexed,
 *                  transmissionAndRLCData_p - This is not NULL if the 
 *                                             encoding request is received
 *                                             from RLC,
 *                  encoderContainerIndex - index of encoderContainerArr_g,
 *                  currentTick
 *                  internalCellIndex - Cell-Indexc at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to insert a node into a encoder 
 *                  Queue, which is used by processEncoderQueue to process 
 *                  encoding requests.
 *****************************************************************************/
void  putEntryInEncoderQueue(UInt32 ueIndex,
                             UInt32 lcCount,
                             /* + SPR 5584 (COVERITY CID 25564) FIX */
                             TransmissionAndRLCData** transmissionAndRLCData_p,
                             /* - SPR 5584 (COVERITY CID 25564) FIX */
                             UInt32 encoderContainerIndex
#ifdef DL_DATA_SEPARATION
                            /* SPR 15909 fix start */
                             , tickType_t currentTick
                             /* SPR 15909 fix end */
#endif
                             ,InternalCellIndex internalCellIndex
                             )
{
    /** CA-TDD Changes Start:06 **/
    EncoderNode      *encoderThdNode_p = PNULL;
    EncoderQueueNode *encoderQueueNode_p = PNULL;
    /** CA Phase2 Changes Start **/
    UInt8 encoderCellIndex = (*transmissionAndRLCData_p)->encoderCellIndex;

    /** CA-TDD Changes End:06 **/

/* + SPR 11209 Changes */
    encoderThdNode_p = &((encoderContainerArr_gp[encoderCellIndex][internalCellIndex] +\
            encoderContainerIndex)->encoderNode);
/* - SPR 11209 Changes */
    /** CA Phase2 Changes End **/

    /*If transmissionAndRLCData_p is not NULL then this is a 
     * request from RLC and has to be pushed into 
     * encoderQueue*/
    if (lcCount == 0 && ueIndex == INVALID_UE_ID)
    {
        /* + SPR 5584 COVERITY CID 25564 FIX */
        /** SPR 13599 Changes Start **/
        /* In case of Dummy Node, transmissionAndRLCData_p is Global memory,
         * So no need to free it now, it will be freed at the time of clean up */
        /** SPR 13599 Changes End **/
        /* - SPR 5584 COVERITY CID 25564 FIX */
        /* when endIndication is set a dummy node is added in all 
           the encoder queues, this dummy node acts as end 
           indicator while iterating the encoderQueues.*/

        /** CA Phase2 Changes Start **/
/* + SPR 11209 Changes */
        putDummyNodeInEncoderQueue(&((encoderContainerArr_gp[encoderCellIndex][internalCellIndex] +\
                        encoderContainerIndex)->encoderNode), internalCellIndex);
/* - SPR 11209 Changes */
        /** CA Phase2 Changes End **/
        return;
    }
    else
    {
        GET_MEM_NODE_ENCODER_CIRC_Q(encoderQueueNode_p,EncoderQueueNode);

        /* Rel 5.3: Coverity 24421 Fix Start */
        if (PNULL == encoderQueueNode_p)
        {
            /* Not able to allocate Memory */
            return;
        }
        /* Rel 5.3: Coverity Fix End */

        encoderQueueNode_p->dummyNode = 0;
        encoderQueueNode_p->ueIndex = ueIndex;    
#ifndef DL_DATA_SEPARATION            
        encoderQueueNode_p->scheduledTick = (*transmissionAndRLCData_p)->
            scheduledTick;    
#else                                                       
        encoderQueueNode_p->scheduledTick = currentTick; 
#endif            
        encoderQueueNode_p->harqProcessId = (*transmissionAndRLCData_p)
            ->harqProcessId;    
        encoderQueueNode_p->cceIndex = (*transmissionAndRLCData_p)->cceIndex;    
        encoderQueueNode_p->transmissionAndRLCData_p = 
            *transmissionAndRLCData_p;
/* SPR 8712 changes */
#if (!(defined(DL_UL_SPLIT)) || (defined(DL_DATA_SEPARATION)))
/* SPR 8712 changes */
        /* COVERITY CID 25406 fixes start */
        if ( pushNode(&(encoderThdNode_p->encoderQueue),
                &(encoderQueueNode_p->encoderNodeAnchor)) )
        {
            lteWarning ("pushNode() failed for EncoderNode");
            FREE_MEM_NODE_ENCODER_CIRC_Q (encoderQueueNode_p);
        }
        /* COVERITY CID 25406 fixes end */
#else
#ifdef CIRC_QUEUE_IMPL
        ENQUEUE_ENCODER_CIRC_Q(&(encoderThdNode_p->encoderCircQueue),
                EncoderQueueNode,&(encoderQueueNode_p->encoderNodeAnchor));
        semPostDefault(&waitForEncoderQueueInd_g[internalCellIndex]);
#else
        putEntryInEncoderQueueUsingICC(encoderThdNode_p->encoderQueueIdUL, encoderQueueNode_p); 
#endif
#endif                      
    }
}


/*****************************************************************************
 * Function Name  : putDummyNodeInEncoderQueue
 * Inputs         : encoderNodeArr_p - Encoder queue will be
 *                                     pushed with a dummy node.
 *                  internalCellIndex - Cell-Indexc at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to put dummy node is all the
 *                  Queue present in the array encoderNodeArr_p given as
 *                  argument. Dummy node act as end indicator for iteration on
 *                  these queues.
 *****************************************************************************/
static void putDummyNodeInEncoderQueue(EncoderNode * encoderNode_p, InternalCellIndex internalCellIndex)
{
    EncoderQueueNode *encoderQueueNode_p = PNULL;

        GET_MEM_NODE_ENCODER_CIRC_Q(encoderQueueNode_p,EncoderQueueNode);        

        /* Rel 5.3: Coverity 24420 Fix Start */
        if (PNULL == encoderQueueNode_p)
        {
            /* Not able to allocate Memory */
            return;
        }
        /* Rel 5.3: Coverity Fix End */

        encoderQueueNode_p->dummyNode = 1;
        /* CA - Ph2 Integration Fix Start */
        encoderQueueNode_p->scheduledTick = getCurrentTickForCell(internalCellIndex);
        /* CA - Ph2 Integration Fix End */

/* SPR 8712 changes */
#if (!(defined(DL_UL_SPLIT)) || (defined(DL_DATA_SEPARATION)))
/* SPR 8712 changes */
        /* COVERITY CID 60992 Fix start */ 
        if( pushNode(&(encoderNode_p->encoderQueue),
                &(encoderQueueNode_p->encoderNodeAnchor)) )
        {
            lteWarning ("pushNode() failed for Encoder queue");
            freeMemPool(encoderQueueNode_p);
            return;
        }
        /* COVERITY CID 60992 Fix end */ 
#else        
#ifdef CIRC_QUEUE_IMPL
        ENQUEUE_ENCODER_CIRC_Q(&(encoderNode_p->encoderCircQueue),
                EncoderQueueNode,&(encoderQueueNode_p->encoderNodeAnchor));
        semPostDefault(&waitForEncoderQueueInd_g[internalCellIndex]);
#else
        putEntryInEncoderQueueUsingICC(encoderNode_p->encoderQueueIdUL, encoderQueueNode_p);
#endif
#endif        
}


/*****************************************************************************
 * Function Name  : initEncoderModule
 * Inputs         : numOfCells
 * Outputs        : None
 * Returns        : None
 * Description    : This function is invoked while MAC init and contains the
 *                  necessary code to intialize the globals required by
 *                  encoder module to function.
 *****************************************************************************/
/* + SPR 11209 Changes */
void initEncoderModule(UInt8  numOfCells)
{
    UInt32 encoderContainerCount =0 ;
    UInt8 cellIndex1 = 0;
    UInt8 cellIndex2 = 0;
    LTE_SQUEUE *encoderQueue_p = PNULL;

    createMemPool(sizeof(EncoderContainerNode) * MAX_ENCODER_CONTAINERS,(numOfCells));
    createMemPool(sizeof(EncoderQueueNode), 2* MAX_DL_UE_SCHEDULED);

    for (cellIndex1 = 0; cellIndex1 < numOfCells; cellIndex1 ++)
    {
        for (cellIndex2 = 0; cellIndex2 < numOfCells; cellIndex2 ++)
        {
            GET_MEM_FROM_POOL(EncoderContainerNode,encoderContainerArr_gp[cellIndex1][cellIndex2], sizeof(EncoderContainerNode)*MAX_ENCODER_CONTAINERS, PNULL);

            for (encoderContainerCount=0; encoderContainerCount < MAX_ENCODER_CONTAINERS;encoderContainerCount++)
            {
                    /* SPR 8712 changes */
    #if (!(defined(DL_UL_SPLIT)) || (defined(DL_DATA_SEPARATION)))
                    /* SPR 8712 changes */
                    encoderQueue_p = &((encoderContainerArr_gp[cellIndex1][cellIndex2] + encoderContainerCount)->\
                            encoderNode.encoderQueue);
                    sQueueInit(encoderQueue_p);
    #endif        
                    encoderQueue_p = &((encoderContainerArr_gp[cellIndex1][cellIndex2] + encoderContainerCount)->\
                            encoderNode.encoderQueueForScheduler);
                    sQueueInit(encoderQueue_p);
            }
        }
    }
}
/* - SPR 11209 Changes */


/*****************************************************************************
 * Function Name  : cleanupEncoderModule
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function is invoked while MAC layer clean up. This
 *                  frees all the memory taken by encoder global data structs.
 *****************************************************************************/
/* + SPR 11209 Changes */
void cleanupEncoderModule(InternalCellIndex internalCellIndex)
{
    /* TDD-SPLIT-GCC Warning */
#if (!defined(DL_UL_SPLIT) || defined(DL_DATA_SEPARATION))
    LTE_SQUEUE       *encoderQueue_p = PNULL;
#elif CIRC_QUEUE_IMPL 
    LTE_CIRCQUEUE    *encoderNewQueue_p = PNULL;
#endif
    LTE_SQUEUE       *encoderSchedQueue_p = PNULL;
    /* TDD-SPLIT-GCC Warning */
    EncoderQueueNode *encoderQueueNode_p = PNULL;
    UInt32 encoderContainerCount =0 ;
    UInt32 nodeCount = 0;
    UInt8 cellIndex = 0;

    for (cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex ++)
    {
        for (encoderContainerCount=0; encoderContainerCount < MAX_ENCODER_CONTAINERS;
                                  encoderContainerCount++)
        {
            /* SPR 8712 changes */
#if (!(defined(DL_UL_SPLIT)) || (defined(DL_DATA_SEPARATION)))

            nodeCount = sQueueCount(encoderQueue_p);
            while (nodeCount--)
            {
                encoderQueueNode_p = (EncoderQueueNode *)popNode(encoderQueue_p);
                if (encoderQueueNode_p)
                {
                    freeMemPool(encoderQueueNode_p);
                    encoderQueueNode_p = PNULL;
                }
            }
#else
            while(1)
            {
#ifdef CIRC_QUEUE_IMPL
                encoderNewQueue_p = (&((encoderContainerArr_gp[internalCellIndex][cellIndex] + encoderContainerCount)->
                            encoderNode.encoderCircQueue));
                DEQUEUE_ENCODER_CIRC_Q(encoderNewQueue_p,EncoderQueueNode,
                        (void *)&encoderQueueNode_p);
#else
                recvMsgWithIccQ((encoderContainerArr_gp[internalCellIndex][cellIndex] + \
                            encoderContainerCount)->encoderNode.encoderQueueIdDL, 
                        /* +- SPR 17777 */
                        (const void *)&encoderQueueNode_p);
                /* +- SPR 17777 */
#endif

                if(encoderQueueNode_p && encoderQueueNode_p->ueIndex != INVALID_UE_INDEX)
                {
                    FREE_MEM_NODE_ENCODER_CIRC_Q(encoderQueueNode_p);
                    continue;
                }
                /* +COVERITY 10490 */
                else if(encoderQueueNode_p && encoderQueueNode_p->ueIndex == INVALID_UE_INDEX)
                    /* -COVERITY 10490 */
                {
                    FREE_MEM_NODE_ENCODER_CIRC_Q(encoderQueueNode_p);
                    break;
                }
            }

#endif            
            encoderSchedQueue_p = (&((encoderContainerArr_gp[internalCellIndex][cellIndex] + encoderContainerCount)->\
                        encoderNode.encoderQueueForScheduler));
            nodeCount = sQueueCount(encoderSchedQueue_p);

            while (nodeCount--)
            {
                encoderQueueNode_p = (EncoderQueueNode *)popNode(encoderSchedQueue_p);

                if (encoderQueueNode_p)
                {
                    freeMemPool(encoderQueueNode_p);
                    encoderQueueNode_p = PNULL; /* COVERITY 26434 */
                }
            }
        }
    }
}
/* - SPR 11209 Changes */

/*****************************************************************************
 * Function Name  : resetHarqState
 * Inputs         : hProcess_p- pointer to harq process whose data is to
 *                              be reset,
 *                  dciPduInfo_p - The data TB pointer at this dciPduInfo_p
 *                                 is set to NULL so that no transmission
 *                                 occurs
 * Outputs        : None
 * Returns        : None
 * Description    : This function is invoked to fail a transmission and when 
 *                  we need to reset Harq state to as if the TX never occured.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129111) */
void resetHarqState(DLHARQProcess * hProcess_p, DCIPduInfo *dciPduInfo_p)
/* SPR 5599 changes end (ZIP ID 129111) */
{
    if (hProcess_p->dlHARQTBOneInfo.retransmissionTBSize)
    {
        hProcess_p->dlHARQTBOneInfo.retransmissionTBSize = 
            hProcess_p->dlHARQTBOneInfo.tbSize;
    }
    if (hProcess_p->dlHARQTBTwoInfo.retransmissionTBSize)
    {
        hProcess_p->dlHARQTBTwoInfo.retransmissionTBSize = 
            hProcess_p->dlHARQTBTwoInfo.tbSize;
    }
    dciPduInfo_p->dataTB_p = PNULL;
}


/*****************************************************************************
 * Function Name  : putEntryInSchEncoderQueue 
 * Inputs         : ueIndex - index of UE in the UE context array.
 *                  encoderValFromSch_p - This is not NULL if the encoding
 *                                        request is received with out
 *                                        any multiplexing.
 *                  encoderContainerIndex - index of encoderContainerArr_g
 *                  internalCellIndex - Cell-Indexc at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to insert a node into a encoder 
 *                  Queue for scheduler, which is used by processEncoderQueue 
 *                  to process encoding requests.
 *****************************************************************************/
void  putEntryInSchEncoderQueue(UInt32 ueIndex,
                EncoderNodeInfo* encoderValFromSch_p,
                UInt32 encoderContainerIndex,
                InternalCellIndex internalCellIndex)
{
    /** CA-TDD Changes Start:06 **/
    EncoderNode      *encoderThdNode_p = PNULL;
    EncoderQueueNode *encoderQueueNode_p = PNULL;

    /** CA-TDD Changes End:06 **/

/* + SPR 11209 Changes */
    encoderThdNode_p = &((encoderContainerArr_gp[internalCellIndex][internalCellIndex] + encoderContainerIndex)->\
                       encoderNode);
/* - SPR 11209 Changes */

    if (encoderValFromSch_p)
    {
        /*If encoderValFromSch_p is not NULL then push it into
         * encoderQueueForScheduler*/

        GET_MEM_FROM_POOL(EncoderQueueNode,encoderQueueNode_p,sizeof(EncoderQueueNode),PNULL);

        /* Rel 5.3: Coverity 24422 Fix Start */
        if (PNULL == encoderQueueNode_p)
        {
            /* Not able to allocate Memory */
            return;
        }
        /* Rel 5.3: Coverity Fix End */

        encoderQueueNode_p->dummyNode = 0;
        encoderQueueNode_p->ueIndex = ueIndex;    
        encoderQueueNode_p->scheduledTick = encoderValFromSch_p->scheduledTick;    
        encoderQueueNode_p->harqProcessId = encoderValFromSch_p->
            harqProcessId;    
        encoderQueueNode_p->cceIndex = encoderValFromSch_p->cceIndex;    
        encoderQueueNode_p->transmissionAndRLCData_p = PNULL;
        /* COVERITY CID 25407 fixes start */
        if( pushNode(&(encoderThdNode_p->encoderQueueForScheduler),
                &(encoderQueueNode_p->encoderNodeAnchor)) )
        {
            lteWarning ("pushNode() failed for EncoderNode");
            freeMemPool(encoderQueueNode_p);
        }
        /* COVERITY CID 25407 fixes end */
    }
    else
    {
        lteWarning("[%s] encoderValFromSch_p is NULL\n",__func__);
    }
}


/*****************************************************************************
 * Function Name  : populateTXInfo
 * Inputs         : dciCCEInfoContainerIndex - Index of 
 *                                             dciCCEContainerInfo_g,
 *                  isTxFunctionalityAdded - If this is true then encoder 
 *                                           itself transmits data after 
 *                                           preparing TB,
 *                  sysFrameNumber - system frame number,
 *                  subFrameNumber - sub frame number,
 *                  cceIndex - Index of CCE,
 *                  cceIndexArr_p -  After processing encoder node if 
 *                                   isTxFunctionalityAdded is not 1 then
 *                                   This array is filled with cceIndex of 
 *                                   the encoding request at index 
 *                                   writeIndex_p,
 *                  writeIndex_p - The index of cceIndexArr_p where the 
 *                                 cceIndex of the current encoding request
 *                                 is filled,
 *                  harqSubFrameNum - It holds the (current tick + delay to Phy
 *                                    + 4)%10 i.e. the subframe in which ACK/NACK
 *                                    should be received for this Transmission 
 *                                    from UE,
 *                  harqRTTExpiryTick- Tick at which this HARQ timer should 
 *                                     be expired
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called by processEncoderNode per popped
 *                  encoder queue node to fill info cceIndexArr_p if TX 
 *                  functionality is false else it does calls 
 *                  createAndSendDataBufferReq.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129111) */
/* +- SPR 17777 */
void populateTXInfo(
/* SPR 5599 changes end (ZIP ID 129111) */
                      UInt32 isTxFunctionalityAdded,
                      UInt32 cceIndex,
                      UInt8 * cceIndexArr_p,
                      UInt8 * writeIndex_p)
/* +- SPR 17777 */
{

    /* Check is the Transmitter funtionality is incorporated within
     * Encoder. In this case Data would be send one by one to PHY */

    if (1 == isTxFunctionalityAdded)
    {
         /* harqSubFrameNum is the subframe in which UE should send
            ACK/NACK for this transmission. 
            harqRTTExpiryTick is calculated by adding 4 subframes so as 
            to make total round trip time equal to transmission time + 8 subframes*/
    lteWarning("Wrong Execution Leg. Encoder with Tx functionality is not supported in\
                      case of FAPI\n");
    }
    else
    {
        cceIndexArr_p[(*writeIndex_p)++] = cceIndex;
    }
}


/*****************************************************************************
 * Function Name  : encodeTB
 * Inputs         : hProcess_p - pointer to harqProcess,
 *                  transmissionAndRLCData_p - contains RLC data buffer and 
 *                                             other information like number
 *                                             of LCs in TB one etc necessary
 *                                             to encode TB,
 *                  ueContext_p - Context of UE for which encoding is to  be
 *                                done,
 *                  dataTB_p - pointer to buffer which should be used to
 *                             encode TB,
 *                  newTBCreationNeeded - If this is true then in case of 
 *                                         retx new TB has to be encoded.
 *                                        in this TB belonging to new tx,
 *                  tbPopulationStruct_p - Pointer to TBPopulationStruct,
 *                  newQueueLoadEncoded - The sum of queue load encoded
 *                                        in this TB belonging to new tx,
 *                  tbIdentifier
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : none
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called to encode first TB in case of 
 *                  spatial multiplexing and also TB in case of non 
 *                  spatial multiplexing. 
 *****************************************************************************/
MacRetType encodeTB( DLHARQProcess * hProcess_p,
                      TransmissionAndRLCData * transmissionAndRLCData_p ,
                      DLUEContext* ueContext_p,
                      UInt8 * dataTB_p,
                      /* +- SPR 17777 */
                      TBPopulationStruct * tbPopulationStruct_p,
                      /* SPR 430 changes start */
                      UInt32* newQueueLoadEncoded,
                      /* +- SPR 17777 */
                      /* SPR 430 changes end */
                      InternalCellIndex internalCellIndex)
{
    DLHARQTBInfo* harqProcessTB_p = tbPopulationStruct_p->harqTB_p;
    UInt32 taValue = INVALID_TA_VALUE;
    UInt32 drxFlag = INVALID_DRX_VALUE;
    UInt8 macScellCEFlag = INVALID_SCELL_ACT_DEACT_VALUE;
    /* SPR 3444 changes start */
    MacRetType retVal = MAC_FAILURE;
    UInt8 scheduleScellCE = ueContext_p->scheduleScellCE;
    /* SPR 3444 changes end */


    if (NEW_TX == harqProcessTB_p->txIndicator)
    {
        /*taDrxFlag in DLHARQTBInfo is set with correponding 
         * value enum of msgType*/
        if (harqProcessTB_p->taDrxFlag == TA_MSG)
        {
            taValue = ueContext_p->timingAdvanceValue;
            ueContext_p->timingAdvanceValue = INVALID_TA_VALUE;
            
            /* ICIC changes start */
             updateDLTA(ueContext_p->ueIndex,ueContext_p->timingAdvanceAbsValue,internalCellIndex);
             /* ICIC changes end */
             
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,
                    "[%s]Encoding TB :TA present %d ",__func__,
                    taValue);
        }
        else if (harqProcessTB_p->taDrxFlag == DRX_MSG)
        {
            drxFlag = 1;
            /* + SPR 22243 Fix*/
            /* reset drxFlag in successful case */
            ueContext_p->drxFlag = FALSE;
            /* - SPR 22243 Fix*/
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,
                    "[%s]Encoding TB :DRX present",__func__);
        }
        else if (harqProcessTB_p->taDrxFlag == TA_DRX_MSG)
        {
            taValue = ueContext_p->timingAdvanceValue;
            drxFlag = 1;
            ueContext_p->timingAdvanceValue = INVALID_TA_VALUE;
            
            /* ICIC changes start */
            updateDLTA(ueContext_p->ueIndex,ueContext_p->timingAdvanceAbsValue,internalCellIndex);
            /* ICIC changes end */
            
            /* + SPR 22243 Fix*/
            /* reset drxFlag in successful case */
            ueContext_p->drxFlag = FALSE;
            /* - SPR 22243 Fix*/
            ueContext_p->pendingDRXAckNack = 1;
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,
                    "[%s]Encoding TB :TA DRX present",__func__);
        }
        /*CA_ENCODER_CHG*/
        /* + SPR 11196 Changes */
        if(harqProcessTB_p->macScellCEFlag != INVALID_SCELL_ACT_DEACT_MSG) 
        /* - SPR 11196 Changes */
        {
            if(hProcess_p->scellActBitmask)
            {
               macScellCEFlag  = hProcess_p->scellActBitmask | scheduleScellCE ;
            }
            else
            {
               macScellCEFlag = (!hProcess_p->scellDeactBitmask) & scheduleScellCE;
            }
        }
        
        /* If this is a new transmission and transmissionAndRLCData_p is 
         * not NULL , then TB to be formed has RLC data to be encoded*/
        if (transmissionAndRLCData_p && (tbPopulationStruct_p->lcCount != 0))
        {
            tbPopulationStruct_p->taValue = taValue;
            tbPopulationStruct_p->drxValue = drxFlag;
            /*CA_ENCODER_CHG*/
            tbPopulationStruct_p->macScellCEFlag = macScellCEFlag;
	    scheduleScellCE_g = tbPopulationStruct_p->macScellCEFlag;
           /* SPR 3444 changes start */
            retVal = createTBFromRLCData(tbPopulationStruct_p,dataTB_p,

                                        newQueueLoadEncoded, ueContext_p,internalCellIndex);
            /* SPR 3444 changes end */
            harqProcessTB_p->transmissionAndRLCData_p = transmissionAndRLCData_p;

            /*The padding bytes and lc count  are to be updated because 
             * they might change after validation and are necessary in
             * the case of retx where TB size changes*/
            harqProcessTB_p->paddingBytes = (tbPopulationStruct_p->padding);
        }
        else
        {
            /* If this is a new transmission and transmissionAndRLCData_p is 
             * NULL , then TB to be formed has no RLC data to be encoded
             * It is only TA or DRX or both*/
            /*SPR_1500 starts
               if (harqProcessTB_p->taDrxFlag == TA_MSG ||
                harqProcessTB_p->taDrxFlag == DRX_MSG ||
                harqProcessTB_p->taDrxFlag == TA_DRX_MSG )
	    SPR_1500 ends */
            {
                /*CA_ENCODER_CHG*/ 
                retVal = createTBForSCELLTADRXMsg( taValue,
                        drxFlag,
                        harqProcessTB_p,
                        dataTB_p,
                        /*CA_ENCODER_CHG*/
                        macScellCEFlag);
                if (harqProcessTB_p->transmissionAndRLCData_p != PNULL )
                {
                    /* SPR 5314 End */	
                    harqProcessTB_p->transmissionAndRLCData_p->numValidTBs--;				
                    if (! harqProcessTB_p->transmissionAndRLCData_p->numValidTBs)
                    {
                        /* SPR  10020 Changes */
                        freeMemPool(harqProcessTB_p->transmissionAndRLCData_p);
                        harqProcessTB_p->transmissionAndRLCData_p = PNULL; 
                    }

					//ysl add
                     harqProcessTB_p->transmissionAndRLCData_p = PNULL;
                    /* SPR 10020 Changes */
                }
                /* SPR 4704 End */			
    
            }    
        }
       
        /* + SPR_3939 */ 
        if ((retVal == MAC_SUCCESS)  && (taValue != INVALID_TA_VALUE))
        {
            /* SPR:21532 Fix - Start */
            ueContext_p->lastTATriggeredTick = getCurrentTick();
            /* SPR:21532 Fix - End */
            /*SPR 15208 Fix*/    
            ueContext_p->TA_ScheduleFlag = LTE_FALSE;
            /*SPR 15208 Fix*/    
            /* + SPR 22352 fix */
            /* Reset the TA_BIT on successful encoding of TA Msg */
            ueContext_p->TaDrxScellBitmap &= ~(TA_BIT);
        }
        /* - SPR_3939 */ 
        /* Reset the SCELL_ACT_BIT/SCELL_DEACT_BIT on successful encoding of 
         * Scell Activation/Deactivation MAC CE */
        if((retVal == MAC_SUCCESS) && (macScellCEFlag != INVALID_SCELL_ACT_DEACT_MSG))
        {
            ueContext_p->TaDrxScellBitmap &= ~(SCELL_ACT_BIT | SCELL_DEACT_BIT);
            ueContext_p->TaDrxScellBitmap &= RESET_SCELL_ID_BIT;
        }
        /* - SPR 22352 fix */
    }
/*    return MAC_SUCCESS; TDD Warning Fix */
      return retVal; 
}

/*****************************************************************************
 * Function Name  : validateTBSize
 * Inputs         : harqProcessTBOne_p,
 *                  harqProcessTBTwo_p,
 *                  dciPduInfo_p,
 *                  totalTBSize,
 *                  hProcess_p,
 *                  newTB1CreationNeeded,
 *                  newTB2CreationNeeded,
 *                  tbOneSize
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will validate and fill the TB size based 
                    on NEW TX, RETX.
*****************************************************************************/
static  MacRetType validateTBSize(DLHARQTBInfo* harqProcessTBOne_p, DLHARQTBInfo* harqProcessTBTwo_p,
                    DCIPduInfo *dciPduInfo_p,UInt32* totalTBSize,DLHARQProcess *hProcess_p,
                    UInt32* newTB1CreationNeeded,UInt32* newTB2CreationNeeded, 
                    UInt32 *tbOneSize)
{
    /* Both TB 1 and TB 2 are valid*/
    if (harqProcessTBOne_p->txIndicator == NEW_TX)
    {
        *tbOneSize = harqProcessTBOne_p->tbSize;

        if (harqProcessTBTwo_p->txIndicator == RE_TX)
        {
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]NEWTX TB 1 size: %d RETX TB2 size: %d ",
                    __func__,
                    harqProcessTBOne_p->tbSize,
                    harqProcessTBTwo_p->retransmissionTBSize);

            *totalTBSize = harqProcessTBOne_p->tbSize +
                harqProcessTBTwo_p->retransmissionTBSize;
             /* SPR 3679 Changes Start */
            *newTB2CreationNeeded = 0;
             /* SPR 3679 Changes End */
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]NEWTX TB 1 size: %d NEWTX TB2 size: %d ",
                    __func__,
                    harqProcessTBOne_p->tbSize,
                    harqProcessTBTwo_p->tbSize);
            *totalTBSize = harqProcessTBOne_p->tbSize +
                harqProcessTBTwo_p->tbSize;
        }
    }
    else
    {
        /* SPR 3679 Changes Start */
        /*Coverity_31961 Fix Start*/
/*SPR 21129 changes start*/                  
        *newTB1CreationNeeded = 0;
/*SPR 21129 changes end*/                  
        /*Coverity_31961 Fix End*/
        /* SPR 3679 Changes End */
        *tbOneSize = harqProcessTBOne_p->retransmissionTBSize;

        if (harqProcessTBTwo_p->txIndicator == NEW_TX)
        {
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]RETX TB 1 size: %d NEWTX TB2 size: %d ",
                    __func__,
                    harqProcessTBOne_p->retransmissionTBSize,
                    harqProcessTBTwo_p->tbSize);
            *totalTBSize = harqProcessTBOne_p->retransmissionTBSize +
                harqProcessTBTwo_p->tbSize;
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]TB1 old TBSize %d new TB Size %d "
                    "TB2 old TB Size %d new TBSize %d",__func__,harqProcessTBOne_p->tbSize,
                    harqProcessTBOne_p->retransmissionTBSize,
                    harqProcessTBTwo_p->tbSize,harqProcessTBTwo_p->retransmissionTBSize);

            /* If there is no change in total TB size and If
             * retranmsssion TB size as previous TB size
             * then there is no need to create a new TB*/
            if ((harqProcessTBOne_p->tbSize == 
                        harqProcessTBOne_p->retransmissionTBSize) &&
                    (harqProcessTBTwo_p->tbSize ==
                     harqProcessTBTwo_p->retransmissionTBSize))
            {
                LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]No need To create New TB for RETX ",
                        __func__);
                /* SPR 6136 start */
                if( (harqProcessTBOne_p->mcsIndex == 0) &&
                        /* SPR 23393 Changes Start */
                        (1 == (MODULO_FOUR(harqProcessTBOne_p->irVersion + 1 ) ) ) ) 
                        /* SPR 23393 Changes End */
                {
                    harqProcessTBOne_p->irVersion = MODULO_FOUR((harqProcessTBOne_p->irVersion + 1));    
                }
                /* SPR 6136 end */ 
                harqProcessTBOne_p->current_TX_NB++;
                harqProcessTBOne_p->irVersion = 
                    MODULO_FOUR((harqProcessTBOne_p->irVersion + 1));
                /*Else if new TB size is same as old TB size then
                 * we use the previously transmitted block*/
                dciPduInfo_p->tb1Info.tbSize = 
                    harqProcessTBOne_p->retransmissionTBSize;
                LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Hpid %d TX_NB %d irVersion %d",__func__,
                        hProcess_p->harqProcessId,harqProcessTBOne_p->current_TX_NB,
                        harqProcessTBOne_p->irVersion);

                harqProcessTBTwo_p->current_TX_NB++;
                harqProcessTBTwo_p->irVersion = 
                    MODULO_FOUR((harqProcessTBTwo_p->irVersion + 1));
                dciPduInfo_p->tb2Info.tbSize = 
                    harqProcessTBTwo_p->retransmissionTBSize;
                /*Else if new TB size is same as old TB size then
                 * we use the previously transmitted block*/
                LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Hpid %d TX_NB %d irVersion %d",__func__,
                        hProcess_p->harqProcessId,harqProcessTBTwo_p->current_TX_NB,
                        harqProcessTBTwo_p->irVersion);
                dciPduInfo_p->dataTB_p = hProcess_p->transmissionBuffer_p;
                /* SPR 3679 Changes Start */
                dciPduInfo_p->dataTB2_p = hProcess_p->transmissionBufferTB2_p;
                /* SPR 3679 Changes End */
                return MAC_SUCCESS;
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]RETX TB 1 size: %d RETX TB2 size: %d ",
                        __func__,
                        harqProcessTBOne_p->retransmissionTBSize,
                        harqProcessTBTwo_p->retransmissionTBSize);
                *totalTBSize = harqProcessTBOne_p->retransmissionTBSize +
                    harqProcessTBTwo_p->retransmissionTBSize;
            }
        }
    }
    return MAC_FAILURE;
}

/*****************************************************************************
 * Function Name  : freeBuffer
 * Inputs         : newTB1CreationNeeded,
 *                  newTB2CreationNeeded,
 *                  hProcess_p
 * Outputs        : None
 * Returns        : None
 * Description    : It free's the Buffer.
*****************************************************************************/
static  void freeBuffer(UInt32 newTB1CreationNeeded, UInt32 newTB2CreationNeeded,
                DLHARQProcess * hProcess_p)
{
    if (newTB1CreationNeeded) 
    {
        /* SPR 3679 Changes Start */
        /* TB1 and TB2 combining shall be done here, Transmitter shall take care
         * of this */
        /* SPR 3679 Changes End */
        if (hProcess_p->transmissionBuffer_p)
        {
            /* SPR 4190 Fix Start */
            msgFree(hProcess_p->transmissionBuffer_p);
            /* SPR 4190 Fix End */
            hProcess_p->transmissionBuffer_p = PNULL;
        }
    }

    /* SPR 3679 Changes Start */
    if (newTB2CreationNeeded) 
    {
        if (hProcess_p->transmissionBufferTB2_p)
        {
            /* SPR 4190 Fix Start */
            msgFree(hProcess_p->transmissionBufferTB2_p);
            /* SPR 4190 Fix End */
            hProcess_p->transmissionBufferTB2_p = PNULL;
        }
    }
}

/*****************************************************************************
 * Function Name  : encodeSpatialMuxNode
 * Inputs         : hProcess_p - pointer to DLHARQProcess,
 *                  dciCCEInfo_p - pointer to DciCCEInfo,
 *                  transmissionAndRLCData_p - pointer to TransmissionAndRLCData,
 *                  ueContext_p - pointer to DLUEContext,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called to TB in case of 
 *                  spatial multiplexing .
*****************************************************************************/
MacRetType encodeSpatialMuxNode(DLHARQProcess * hProcess_p,
                          DciCCEInfo* dciCCEInfo_p,
                          TransmissionAndRLCData * transmissionAndRLCData_p,
                          DLUEContext * ueContext_p,
                          InternalCellIndex internalCellIndex
                          /* + SPR_14039 */
#ifdef TDD_CONFIG
                          /* SPR 15909 fix start */
                          ,tickType_t scheduledTick
                          /* SPR 15909 fix end */
#endif                           
                          /* - SPR_14039 */
                          )
{
    /* SPR 16531 Fix Start */	
    MacRetType retValEncodeTBOne = MAC_SUCCESS;
    MacRetType retValEncodeTBTwo = MAC_SUCCESS;
    /* SPR 16531 Fix End */	
    UInt32 totalTBSize = 0;
    UInt32 tbOneSize = 0;
    UInt32 tb2StartOffset = 0;
    UInt32 newQLoadSumTB1 = 0;
    UInt32 newQLoadSumTB2 = 0;
    UInt8* dataTB_p = PNULL;
    /* SPR 3707 Changes Start */    
    UInt8* dataTB2_p = PNULL;
    /* SPR 3707 Changes End */    

    /* SPR 3679 Changes Start */
    UInt32 newTB1CreationNeeded = 1;
    UInt32 newTB2CreationNeeded = 1;
    /* SPR 3679 Changes End */
    TBPopulationStruct tbPopulationStruct = {0};
    DLHARQTBInfo*       harqProcessTBOne_p = PNULL;
    DLHARQTBInfo*       harqProcessTBTwo_p = PNULL;
    DCIPduInfo *dciPduInfo_p = PNULL;

    dciPduInfo_p  = &(dciCCEInfo_p->dciPduInfo);
    harqProcessTBOne_p =  &(hProcess_p->dlHARQTBOneInfo);
    harqProcessTBTwo_p =  &(hProcess_p->dlHARQTBTwoInfo);
    UInt32 ret = 0;

    UInt32* permissibleTBSize_p = 0;

    if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex))
    {
        permissibleTBSize_p = &(ueContext_p->permissibleTBSize);
    }
    else
    {
        permissibleTBSize_p = &(ueContext_p->dlUeScellContext_p[1]->permissibleTBSize);
    }


    LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Both TB 1 and TB2 Valid",__func__);

    ret = validateTBSize(harqProcessTBOne_p, harqProcessTBTwo_p,
            dciPduInfo_p,&totalTBSize,hProcess_p,
            &newTB1CreationNeeded,&newTB2CreationNeeded, &tbOneSize);
    if(ret)
    {
        return MAC_SUCCESS;
    }

    if (newTB1CreationNeeded) /* SPR 3679 changes Start */
    {
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Creating New TB size %d either prev TB is NULL or Change in TB size"
                ,__func__,totalTBSize);
        dataTB_p = msgAlloc(PNULL,0,0,0);
        msgReserve(dataTB_p,0, MAX_MAC_HEADER_MAC_CCE_LEN);
    }
    else
    {
        /* SPR 3679 Changes Start */
        /* SPR 6136 start */
        if( (harqProcessTBOne_p->mcsIndex == 0) &&
                (1 == ((harqProcessTBOne_p->irVersion + 1 ) % 4) ) ) 
        {
            harqProcessTBOne_p->irVersion = MODULO_FOUR((harqProcessTBOne_p->irVersion + 1));    
        }
        /* SPR 6136 end */ 
        harqProcessTBOne_p->current_TX_NB++;
        harqProcessTBOne_p->irVersion = 
            MODULO_FOUR((harqProcessTBOne_p->irVersion + 1));

        dciPduInfo_p->tb1Info.tbSize = 
            harqProcessTBOne_p->retransmissionTBSize;
        /* SPR 3679 Changes End */

        dataTB_p = hProcess_p->transmissionBuffer_p;
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Using previous TB since no change in TB size",__func__);
    }

    harqProcessTBOne_p->tbStartOffset = DATA_MSG_API_HEADER_LEN;

    LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Encoding TB1",__func__);
    tbPopulationStruct.harqTB_p = harqProcessTBOne_p;
    if (transmissionAndRLCData_p &&
            harqProcessTBOne_p->txIndicator == NEW_TX)
    {
        tbPopulationStruct.rlcDataBuffer_p = 
            transmissionAndRLCData_p->rlcDataBufferArr;
        transmissionAndRLCData_p->tbTwoLCStartIndex = transmissionAndRLCData_p->
            lcCountTBOne;
        tbPopulationStruct.lcCount = transmissionAndRLCData_p->
            lcCountTBOne;
        tbPopulationStruct.padding = 
            (transmissionAndRLCData_p->paddingBytesForTBOne);

        tbPopulationStruct.rlcBufferStartIndex = 0;

        retValEncodeTBOne = encodeTB( hProcess_p,
                transmissionAndRLCData_p ,
                ueContext_p,
                dataTB_p,
                /* +- SPR 17777 */
                &tbPopulationStruct,
                /* SPR 430 changes start */
                &newQLoadSumTB1,
                /* +- SPR 17777 */
                /* SPR 430 changes end */
                internalCellIndex);
        /* SPR 21568 Fix + */
        if (MAC_SUCCESS == retValEncodeTBOne)
        {
            UInt32 blockSize;
            UInt32 tbSizeEncoded =  msgSize(dataTB_p,&blockSize);
            /* SPR 12449 changes start */
            if(harqProcessTBOne_p->tbSize - tbSizeEncoded)
            {    
                /* SPR 12449 changes end */
                /* SPR 17957 Fix Start */
                msgReserve(dataTB_p,tbSizeEncoded, (harqProcessTBOne_p->tbSize - tbSizeEncoded));
                /* SPR 17957 Fix End */
                tbSizeEncoded =  msgSize(dataTB_p,&blockSize);
                /* SPR 12449 changes start */
            }
            /* SPR 12449 changes end */
            tbPopulationStruct.rlcBufferStartIndex = tbPopulationStruct.lcCount;
            transmissionAndRLCData_p->lcCountTBOne =
                tbPopulationStruct.lcCount;
        }
        else if (newTB1CreationNeeded)
        {
            msgFree(dataTB_p);
            dataTB_p = PNULL;
        }
        /* SPR 21568 Fix - */
    }

    /* SPR 3679 changes Start */
    if (newTB2CreationNeeded) 
    {
        dataTB2_p = msgAlloc(PNULL,0,0,0);
        msgReserve(dataTB2_p,0, MAX_MAC_HEADER_MAC_CCE_LEN);
    }
    else
    {
        /* SPR 6136 start */
        if( (harqProcessTBTwo_p->mcsIndex == 0) &&
                (1 == ((harqProcessTBTwo_p->irVersion + 1 ) % 4) ) ) 
        {
            harqProcessTBTwo_p->irVersion = MODULO_FOUR((harqProcessTBTwo_p->irVersion + 1));    
        }
        /* SPR 6136 end */ 
        harqProcessTBTwo_p->current_TX_NB++;
        harqProcessTBTwo_p->irVersion = 
            MODULO_FOUR((harqProcessTBTwo_p->irVersion + 1));

        dciPduInfo_p->tb2Info.tbSize = 
            harqProcessTBTwo_p->retransmissionTBSize;

        dataTB2_p = hProcess_p->transmissionBufferTB2_p;
    }
    /* SPR 3679 Changes End */


    tb2StartOffset= (harqProcessTBOne_p->tbStartOffset +
                                           tbOneSize);

    LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Encoding TB2",__func__);

    tbPopulationStruct.harqTB_p = harqProcessTBTwo_p;

    if (transmissionAndRLCData_p && 
               harqProcessTBTwo_p->txIndicator == NEW_TX)
    {
        tbPopulationStruct.rlcDataBuffer_p = 
            transmissionAndRLCData_p->rlcDataBufferArr;
        tbPopulationStruct.lcCount = 
            transmissionAndRLCData_p->lcCountTBTwo;
        tbPopulationStruct.padding = 
            (transmissionAndRLCData_p->paddingBytesForTBTwo);
        tbPopulationStruct.harqTB_p = harqProcessTBTwo_p;

        tbPopulationStruct.rlcBufferStartIndex = 
            transmissionAndRLCData_p->tbTwoLCStartIndex;

        retValEncodeTBTwo =  encodeTB( hProcess_p,
                transmissionAndRLCData_p ,
                ueContext_p,
                dataTB2_p,
                /* +- SPR 17777 */
                &tbPopulationStruct,
                /* SPR 430 changes start */
                &newQLoadSumTB2,
                /* +- SPR 17777 */
                internalCellIndex);
        /* SPR 430 changes end */
        /*+COVERITY 5.3.0 - 32122*/
        //retValEncodeTBTwo = retValEncodeTBTwo;/* TDD Warning Fix */
        /*-COVERITY 5.3.0 - 32122*/
        /* SPR 21568 Fix + */
        if (MAC_SUCCESS == retValEncodeTBTwo)
        {
            UInt32 blockSize;
            UInt32 tbSizeEncoded =  msgSize(dataTB2_p,&blockSize);
            /* SPR 12449 changes start */
            if(harqProcessTBTwo_p->tbSize - tbSizeEncoded)
            {    
                /* SPR 17957 Fix Start */
                /* SPR 12449 changes end */
                msgReserve(dataTB2_p,tbSizeEncoded, (harqProcessTBTwo_p->tbSize - tbSizeEncoded) );
                /* SPR 17957 Fix End */
                tbSizeEncoded =  msgSize(dataTB2_p,&blockSize);
                /* SPR 12449 changes start */
            }
            /* SPR 12449 changes end */
            transmissionAndRLCData_p->lcCountTBTwo =
                tbPopulationStruct.lcCount;
        }
        else if (newTB2CreationNeeded)
        {
            msgFree(dataTB2_p);
            dataTB2_p = PNULL;
        }
        /* SPR 21568 Fix - */
    }
    harqProcessTBTwo_p->tbStartOffset = tb2StartOffset;

    freeBuffer(newTB1CreationNeeded,newTB2CreationNeeded,hProcess_p);
     /* SPR 21568 fix start */ 
    if (MAC_SUCCESS == retValEncodeTBOne)
    {
        hProcess_p->transmissionBuffer_p = dataTB_p;
        hProcess_p->totalTBSize = totalTBSize;
        dciPduInfo_p->dataTB_p = dataTB_p;
        /*Soft Buffer Changes Start*/
        if (harqProcessTBOne_p->txIndicator == NEW_TX)
        {
            *permissibleTBSize_p -= harqProcessTBOne_p->tbSize;
        }
    }
    if (MAC_SUCCESS == retValEncodeTBTwo)
    {
        hProcess_p->transmissionBufferTB2_p = dataTB2_p;
        dciPduInfo_p->dataTB2_p = dataTB2_p;
        hProcess_p->totalTBSize = totalTBSize;
        /* SPR 3679 Changes End */
    if (harqProcessTBTwo_p->txIndicator == NEW_TX)
    {
            *permissibleTBSize_p -= harqProcessTBTwo_p->tbSize;
        }
    }
    /* SPR 21568 Fix - */

    /*Soft Buffer Changes End*/
    /*+COVERITY 5.3.0 - 32122*/
    if (retValEncodeTBOne == MAC_FAILURE || retValEncodeTBTwo == MAC_FAILURE)
        return MAC_FAILURE;
#ifdef TDD_CONFIG
     /* + SPR_14039 */
    else if (IS_CA_ENABLED()) 
    {
        /* Update DAI and staore the updated DAI in DCI Info */
        dciPduInfo_p->dai = updateDai(ueContext_p,
                scheduledTick,
                scheduledTick % MAX_SUB_FRAME,
                internalCellIndex);
    }     
     /* - SPR_14039 */
#endif
    /* coverity fix- 70269 start */ 
    return MAC_SUCCESS;
    /* coverity fix- 70269 end */ 
    /*-COVERITY 5.3.0 - 32122*/
}



/*****************************************************************************
 * Function Name  : encodeNonSpatialMuxNode
 * Inputs         : hProcess_p - pointer to DLHARQProcess,
 *                  dciCCEInfo_p - pointer to DciCCEInfo,
 *                  transmissionAndRLCData_p - pointer to TransmissionAndRLCData,
 *                  ueContext_p - pointer to DLUEContext,
 *                  harqProcessTB_p - pointer to DLHARQTBInfo,
 *                  tbPopulationStruct_p - pointer to TBPopulationStruct,
 *                  tbIdentifier
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function is called to create TB in case of 
 *                  non-spatial multiplexing.
*****************************************************************************/
MacRetType encodeNonSpatialMuxNode(DLHARQProcess * hProcess_p,
                              DciCCEInfo* dciCCEInfo_p,
                              TransmissionAndRLCData * transmissionAndRLCData_p,
                              DLUEContext * ueContext_p,
                              DLHARQTBInfo* harqProcessTB_p,
                              TBPopulationStruct *tbPopulationStruct_p,
                              /* SPR 431 changes start */
                              UInt8 tbIdentifier,
                              /* SPR 431 changes end */
                              InternalCellIndex internalCellIndex
                              /* + SPR_14039 */
#ifdef TDD_CONFIG                              
                             /* SPR 15909 fix start */
                             ,tickType_t scheduledTick
                             /* SPR 15909 fix end */
#endif                              
                              /* - SPR_14039 */
                              )
{
    MacRetType retValEncodeTB = MAC_FAILURE;
    UInt32 totalTBSize = 0;
    UInt32 newQLoadSumTB1 = 0;
    UInt8* dataTB_p = PNULL;
    /* +- SPR 17777 */
    DCIPduInfo *dciPduInfo_p = PNULL;

    dciPduInfo_p  = &(dciCCEInfo_p->dciPduInfo);

    LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Only single TB  Valid",__func__);

    /* Only TB 1 is valid*/
    if (harqProcessTB_p->txIndicator == NEW_TX)
    {
        totalTBSize = harqProcessTB_p->tbSize ;
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]New TX TbSize: %d",__func__,
                totalTBSize);
    }
    else
    {
        totalTBSize = harqProcessTB_p->retransmissionTBSize ;
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]RE TX TbSize: %d",__func__,
                totalTBSize);
        /*LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]RE TX TbSize: %d",__func__,
                totalTBSize);*/
    }
    /* totalTBSize is zero means no TB already exists with this
     * harq process and new TB needs to be created.*/
    if (!hProcess_p->totalTBSize || hProcess_p->totalTBSize !=
            totalTBSize)
    {
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Creating New TB either prev TB is NULL or Change in TB size"
                ,__func__);

        /* +- SPR 17777 */
        dataTB_p = msgAlloc(PNULL,0,0,0);
        msgReserve(dataTB_p,0, MAX_MAC_HEADER_MAC_CCE_LEN);
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Using previous TB since no change in TB size",__func__);
        /*LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Using previous TB since no change in TB size",__func__);*/
        harqProcessTB_p->current_TX_NB++;
        harqProcessTB_p->irVersion = 
            MODULO_FOUR((harqProcessTB_p->irVersion + 1));
        /*Else if new TB size is same as old TB size then
         * we use the previously transmitted block*/
        /* SPR 3679 Changes Start */
        if (TB_ONE == tbIdentifier)
        {
            dciPduInfo_p->dataTB_p = hProcess_p->transmissionBuffer_p;
        }
        else if (TB_TWO == tbIdentifier)
        {
            dciPduInfo_p->dataTB2_p = hProcess_p->transmissionBufferTB2_p;
        }
        /* SPR 3679 Changes End */
        
        LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Hpid %d TX_NB %d irVersion %d",__func__,
                hProcess_p->harqProcessId,harqProcessTB_p->current_TX_NB,
                harqProcessTB_p->irVersion);
        return MAC_SUCCESS;
    }
    harqProcessTB_p->tbStartOffset = DATA_MSG_API_HEADER_LEN;

    retValEncodeTB = encodeTB( hProcess_p,
            transmissionAndRLCData_p ,
            ueContext_p,
            dataTB_p,
            /* +- SPR 17777 */
            tbPopulationStruct_p,
            /* SPR 430 changes start */
            &newQLoadSumTB1,
            /* +- SPR 17777 */
            /* SPR 430 changes end */
            internalCellIndex);

    /* SPR 3444 changes start */
    /* SPR 3444 changes end */


    /* Since a new TB is created we free previous
     * TB in harq process if it exists.*/
    if (hProcess_p->transmissionBuffer_p && 
            TB_ONE == tbIdentifier) /* SPR 3679 Changes */
    {
        /* SPR 4190 Fix start */
        msgFree(hProcess_p->transmissionBuffer_p);
        /* SPR 4190 Fix End */
        hProcess_p->transmissionBuffer_p = PNULL;
    }
    /* SPR 3679 Changes Start */
    else if (hProcess_p->transmissionBufferTB2_p && 
            TB_TWO == tbIdentifier)
    {
            /* SPR 4190 Fix Start */
        msgFree(hProcess_p->transmissionBufferTB2_p);
            /* SPR 4190 Fix End */
        hProcess_p->transmissionBufferTB2_p = PNULL;
    }

    /* SPR 21568 Fix + */
    if (MAC_SUCCESS == retValEncodeTB)
    {
        if (TB_ONE == tbIdentifier)
        {
            hProcess_p->transmissionBuffer_p = dataTB_p;
            dciPduInfo_p->dataTB_p = dataTB_p;
        }
        /*coverity 25190 fix*/
        else if (TB_TWO == tbIdentifier) 
        {
            hProcess_p->transmissionBufferTB2_p = dataTB_p;
            dciPduInfo_p->dataTB2_p = dataTB_p;
        }
        else
        {
            /* Rel 5.3: Coverity 25190 Start */
            msgFree(dataTB_p);
            /* Rel 5.3: Coverity 25190 End */

            LOG_MAC_MSG( MAC_PDSCH_TB_INVALID, LOGWARNING, MAC_PDCCH,
                    getCurrentTick(), __LINE__,
                    ueContext_p->ueIndex,
                    tbIdentifier,
                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    __func__, "");

            LTE_MAC_UT_LOG(LOG_INFO,MAC_MOD_ENCODER,"[%s]Invalid TB Identifir received TBID[%d]",__func__,tbIdentifier);
        }
        /*coverity 25190 fix*/
        /* SPR 3679 Changes End */

        hProcess_p->totalTBSize = totalTBSize;
        /*Soft Buffer Changes Start*/
        if (harqProcessTB_p->txIndicator == NEW_TX)
        {
            if(IS_PCELL_OF_UE(ueContext_p,internalCellIndex ))
            {
                ueContext_p->permissibleTBSize -= harqProcessTB_p->tbSize;
            }
            else
            {
                ueContext_p->dlUeScellContext_p[1]->permissibleTBSize -= 
                    harqProcessTB_p->tbSize;
            }
        }
        /*Soft Buffer Changes End*/
        /*+COVERITY 5.3.0 - 32121*/
    }
    else
    {
        msgFree(dataTB_p);
        dataTB_p = PNULL;
    }
    /* SPR 21568 Fix - */

#ifdef TDD_CONFIG
     /* + SPR_14039 */
    if ((IS_CA_ENABLED()) && (MAC_FAILURE != retValEncodeTB))
    {
        /* Update DAI and staore the updated DAI in DCI Info */
        dciPduInfo_p->dai = updateDai(ueContext_p,
                                   scheduledTick,
                                   scheduledTick % MAX_SUB_FRAME,
                                    internalCellIndex);
    }    
     /* - SPR_14039 */
#endif
    return retValEncodeTB;
    /*-COVERITY 5.3.0 - 32121*/
}



/*****************************************************************************
 * Function Name  : resetHARQIfReTx
 * Inputs         : hProcess_p - Pointer to the given harq process,
 *                  ueContext_p - UE context pointer,
 *                  dciCCEInfo_p - Pointer to the CCE Array
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : The function when called free harq process if 
 *                  it's the case of new tx only else the harq state is 
 *                  reset.
*****************************************************************************/
MacRetType resetHARQIfReTx(DLHARQProcess * hProcess_p,
                           DLUEContext * ueContext_p,
                           DciCCEInfo * dciCCEInfo_p,
                           InternalCellIndex internalCellIndex)
{
    if (hProcess_p->isTBOneValid)
    {
        if (hProcess_p->dlHARQTBOneInfo.txIndicator == NEW_TX)
        {
            if (hProcess_p->isTBTwoValid)
            {
                /* free HarqProcess only whn it's the case of new transmission
                */
                if (hProcess_p->dlHARQTBTwoInfo.txIndicator == NEW_TX)
                {
                    freeDLHarqProcess(ueContext_p,hProcess_p,internalCellIndex);
                    return MAC_FAILURE;
                }
                else
                {
                    /* else when thr is retx also then we reset harq 
                     * state instead of freeing harq process */
                    hProcess_p->isTBOneValid = TB_INVALID;
                   
                    resetHarqState(hProcess_p, &dciCCEInfo_p->dciPduInfo);
                    return MAC_SUCCESS;
                }
            }
            else
            {
                freeDLHarqProcess(ueContext_p,hProcess_p,internalCellIndex);
                return MAC_FAILURE;
            }
        }
        else
        {
            resetHarqState(hProcess_p, &dciCCEInfo_p->dciPduInfo);
            if (hProcess_p->isTBTwoValid)
            {
                if (hProcess_p->dlHARQTBTwoInfo.txIndicator == NEW_TX)
                {
                    hProcess_p->isTBTwoValid = TB_INVALID;
                }
            }
            return MAC_SUCCESS;
        }
    }
    else
    {
        if (hProcess_p->dlHARQTBTwoInfo.txIndicator == NEW_TX)
        {
            freeDLHarqProcess(ueContext_p,hProcess_p,internalCellIndex);
            return MAC_FAILURE;
        }
        else
        {
            /* else when thr is retx also then we reset harq 
             * state instead of freeing harq process */
            resetHarqState(hProcess_p, &dciCCEInfo_p->dciPduInfo);
            return MAC_SUCCESS;
        }
    }
}



/*****************************************************************************
 * Function Name  : processEncoderReTxNode
 * Inputs         : encoderQueueNode_p - Encoder Retransmission Queue Node
 *                                       pointer,
 *                  currentTick - Value of global Tick at which this method
 *                                is called,
 *                  delayToPhy -  Overall delay of the system,
 *                  dciContainerIndex - Index of 
 *                                             dciCCEContainerInfo_g,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This funtion is called to handle the Retransmission
 *                  Nodes that are directly put into the Encoder Retransmission 
 *                  Queue by the PDSCH.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129111) */
MacRetType processEncoderReTxNode(EncoderQueueNode *encoderQueueNode_p,
/* SPR 5599 changes end (ZIP ID 129111) */
                                        /* SPR 15909 fix start */
                                        tickType_t currentTick,
                                        /* SPR 15909 fix end */
                                         UInt32 delayToPhy,
                                         UInt32 dciContainerIndex,
                                         InternalCellIndex internalCellIndex 
/* + SPR 15127 changes */
#ifdef TDD_CONFIG
                                        ,UInt8 subFrameNumber
#endif

/* - SPR 15127 changes */
                                         )
{    
    DLUEContext * ueContext_p = PNULL;
    DciCCEInfo* dciCCEInfo_p = PNULL;
    /* + SPR 11469 Changes */
    DLHARQProcess * hProcess_p = PNULL;
    /* - SPR 11469 Changes */

    if ( (encoderQueueNode_p->scheduledTick != (currentTick + delayToPhy)) ||
            (dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].pendingDeleteFlag))
    {
        lteWarning("%s Queue from Sch Tick mismatch schtick %d curr tick %d \
                or ueContext_p is NULL for HARQ Process ID %d\n",
                __func__,encoderQueueNode_p->scheduledTick,
                (currentTick + delayToPhy), encoderQueueNode_p->harqProcessId );
         
        dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                           dciCCEInfo[encoderQueueNode_p->cceIndex]);
        
        if (!dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].pendingDeleteFlag)
        {
            ueContext_p = dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex]
                .dlUEContext_p;
            /* + SPR 11469 Changes */
#ifdef TDD_CONFIG 
/* + SPR 15127 changes */       
            /* + CA_TDD_HARQ_CHANGES */
            ueContext_p->retxQueueNode_p[subFrameNumber] = PNULL;
            /* - CA_TDD_HARQ_CHANGES */
#endif        

/* - SPR 15127 changes */
            if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex))
            {
                hProcess_p = &(ueContext_p->dlHarqContext_p->\
                        harqProcess[encoderQueueNode_p->harqProcessId]);
            }
            else
            {
                hProcess_p = &(ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p->
                        harqProcess[encoderQueueNode_p->harqProcessId]);
            }
            return resetHARQIfReTx(hProcess_p,
                    ueContext_p,
                    dciCCEInfo_p,
                    internalCellIndex);
            /* - SPR 11469 Changes */
        }
        return MAC_FAILURE;
    }
    ueContext_p = dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].dlUEContext_p;
    
    return processEncoderNode( encoderQueueNode_p,
                               dciContainerIndex,
                               ueContext_p,
                               internalCellIndex
                              /* + SPR_14039 */
#ifdef TDD_CONFIG                               
                               ,(currentTick + delayToPhy)
#endif                          
                              /* - SPR_14039 */
                               );
}

#ifdef DL_DATA_SEPARATION
/*****************************************************************************
 * Function Name  : processRlcEncoderQueuePerUE
 * Inputs         : currentTick - Present value of global Tick.
 *                  delayToPhy -  Delay to PHY layer from Encoder module.
 *                  subFrameNumber - Present sub frame number.
 *                  isTxFunctionalityAdded - If this is true encoder itself
 *                                           sends the TB.
 *                  lastUeFlag_p
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE(with error code) 
 * Description    : This funtion is called by execution scheduler flow to
 *                  execute encoding functionality.This function processes all
 *                  the encoding requests present encoder queue corresponding 
 *                  to particular container identified by encoderContainerIndex.
 *****************************************************************************/

                        /* SPR 15909 fix start */
UInt16 processRlcEncoderQueuePerUE(tickType_t currentTick,
                        /* SPR 15909 fix end */
                         UInt32 delayToPhy,
                         /* +- SPR 18490 */
                         UInt32 subFrameNumber,
                         UInt32 isTxFunctionalityAdded,
                         UInt8 *lastUeFlag_p,
                         InternalCellIndex internalCellIndex,
                         InternalCellIndex rlcCellIndex)
{
    EncoderQueueNode *encoderQueueNode_p = PNULL;
    LTE_SQUEUE       *encoderNewTxQueue_p = PNULL;

    UInt32 encoderContainerIndex = 0;
    UInt32 dciContainerIndex = 0;
    DLUEContext * ueContext_p = PNULL;
    UInt8 * cceIndexArr_p = PNULL;
    UInt8 * writeIndex_p = PNULL;
    DciCCEInfo* dciCCEInfo_p = PNULL;
    DLHARQProcess * hProcess_p = PNULL;
    UInt32 harqSubFrameNum = 0;
    /* +- SPR 18490 */
    EncoderNode *encoderNode_p = PNULL;
    /* SPR 15909 fix start */
    tickType_t currentTickWithDelay = currentTick + delayToPhy;
    /* SPR 15909 fix end */

    harqSubFrameNum = subFrameNumber + delayToPhy + 4;

    if (harqSubFrameNum >= NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME)
    {
        harqSubFrameNum -= NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    }
    /* +- SPR 18490 */

    dciContainerIndex = currentTickWithDelay % MAX_CCE_CONTAINER;
    encoderContainerIndex = dciContainerIndex;

    /* + SPR 11209 Changes */
    /* Currently using 2nd Index of encoderContainerArr as internalCellIndex 
     * Need to change later */
    encoderNode_p = &((encoderContainerArr_gp[internalCellIndex][rlcCellIndex] + encoderContainerIndex)->\
            encoderNode);
    /* - SPR 11209 Changes */

    cceIndexArr_p = encoderNode_p->cceIndexArr; 

    encoderNode_p->writeIndex = 0;
    encoderNode_p->readIndex = 0;

    writeIndex_p = &(encoderNode_p->writeIndex);

    encoderNewTxQueue_p = &(encoderNode_p->encoderQueue);
    encoderQueueNode_p = (EncoderQueueNode *)popNode(encoderNewTxQueue_p);

    if (PNULL == encoderQueueNode_p) 
    {
        return MAC_NULL_ENCODER_NODE_RECVD;        
    }

    if (!encoderQueueNode_p->dummyNode)
    {
        if ((dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].pendingDeleteFlag))
        {
            dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                    dciCCEInfo[encoderQueueNode_p->cceIndex]);
            mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                    dciCCEInfo_p,internalCellIndex);
            /*pdcch for scell changes start*/
            mac_reset_extra_cce(dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex,dciCCEInfo_p,
                    dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].dlUEContext_p,internalCellIndex);
            /*pdcch for scell changes end*/
            lteWarning(" %s ueContext_p is null\n",__func__);
            freeMemPool((void *)encoderQueueNode_p);
            encoderQueueNode_p = PNULL;
            return MAC_UE_PENDING_FOR_DELETION;        
        }

        ueContext_p = dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].
            dlUEContext_p;

        if (PNULL == ueContext_p)
        {
            LOG_MAC_MSG(MAC_DLUECONTEXT_NOT_FOUND_ID,
                    LOGERROR,
                    MAC_RRC_INF,
                    getCurrentTick(),
                    encoderQueueNode_p->ueIndex,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "MAC_DLUECONTEXT_NOT_FOUND");
            dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                    dciCCEInfo[encoderQueueNode_p->cceIndex]);
            mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                    dciCCEInfo_p,internalCellIndex);
            /*pdcch for scell changes start*/
            mac_reset_extra_cce(dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex,dciCCEInfo_p,
                    dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].dlUEContext_p,internalCellIndex);
            /*pdcch for scell changes end*/
            freeMemPool((void *)encoderQueueNode_p);
            encoderQueueNode_p = PNULL;
            return MAC_NULL_UE_CONTEXT_FOUND;        

        }
        if (encoderQueueNode_p->scheduledTick != currentTickWithDelay)
        {
            dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                    dciCCEInfo[encoderQueueNode_p->cceIndex]);
            if (encoderQueueNode_p->transmissionAndRLCData_p)
            {

                lteWarning("%s from Mux Tick mismatch schtick %d curr tick %d\n",
                        __func__,encoderQueueNode_p->scheduledTick,
                        currentTickWithDelay);

                /* + SPR 11469 Changes */
                if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex))
                {                
                    hProcess_p = &(ueContext_p->dlHarqContext_p->
                             harqProcess[encoderQueueNode_p->harqProcessId]);
                }
                else
                {
                    hProcess_p = &(ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p->
                             harqProcess[encoderQueueNode_p->harqProcessId]);
                }
                /* - SPR 11469 Changes */

                if (MAC_FAILURE == resetHARQIfReTx(hProcess_p,
                            ueContext_p,
                            dciCCEInfo_p,
                            internalCellIndex))
                {
                    mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                            dciCCEInfo_p,internalCellIndex);
                /*pdcch for scell changes start*/
                mac_reset_extra_cce(dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex,dciCCEInfo_p,
                dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].dlUEContext_p,internalCellIndex);
                /*pdcch for scell changes end*/
                }
            }
            freeMemPool((void *)encoderQueueNode_p);
            encoderQueueNode_p = PNULL;
            return MAC_TICK_MISMATCH_ERROR;        
        }
        else
        {
            if (MAC_FAILURE == processEncoderNode( encoderQueueNode_p,
                        dciContainerIndex,
                        ueContext_p,
                        internalCellIndex
                        /* + SPR_14039 */
#ifdef TDD_CONFIG 
                        ,(currentTick + delayToPhy)
#endif                        
                       /* - SPR_14039 */
                        )) 
            {
                freeMemPool((void *)encoderQueueNode_p);
                encoderQueueNode_p = PNULL;
                return MAC_ENCODING_FAIL;        
            }
            /* Stage-III SCELL Deactivation Timer Related Changes Start */
            else
            {
                scellStateMngrUpdateScellDeactTimer(ueContext_p,internalCellIndex);
            }
            /* Stage-III SCELL Deactivation Timer Related Changes End */
        }

        /* +- SPR 17777 */
        populateTXInfo(
                isTxFunctionalityAdded,
                encoderQueueNode_p->cceIndex,
                cceIndexArr_p,
                writeIndex_p
                );
        /* +- SPR 17777 */
        ueContext_p->advlastScheduledTick = currentTick + delayToPhy;
    }
    else
    {
        *lastUeFlag_p=TRUE;
    }
    freeMemPool((void *)encoderQueueNode_p);
    encoderQueueNode_p = PNULL;

    cceIndexArr_p[(*writeIndex_p)++] = INVALID_CCE_INDEX;
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : processMACEncoderQueue
 * Inputs         : currentTick - Present value of global Tick.
 *                  delayToPhy -  Delay to PHY layer from Encoder module.
 *                  systemFrameNum - Present system frame number.
 *                  subFrameNumber - Present sub frame number.
 *                  isTxFunctionalityAdded - If this is true encoder itself
 *                                           sends the TB.
 *                  encoderThdCount - Identifies execution scheduler flow
 *                                    index.
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion is called by execution scheduler flow to
 *                  execute encoding functionality.This function processes all
 *                  the encoding requests present encoder queue corresponding 
 *                  to particular container identified by encoderContainerIndex.
 *****************************************************************************/
                            /* SPR 15909 fix start */
void processMACEncoderQueue(tickType_t currentTick,
                            /* SPR 15909 fix end */
        UInt32 delayToPhy,
        /* +- SPR 18490 */
        UInt32 subFrameNumber,
        UInt32 isTxFunctionalityAdded,
        /* +- SPR 18490 */
        InternalCellIndex internalCellIndex,
        InternalCellIndex rlcCellIndex)
{
/* Stage-III SCELL Deactivation Timer Related Changes Start */
    DLUEContext           *ueDLContext_p          = PNULL;
/* Stage-III SCELL Deactivation Timer Related Changes End */
    EncoderQueueNode *encoderQueueNode_p = PNULL;
    LTE_SQUEUE       *encoderReTxQueue_p = PNULL;

    UInt32 encoderContainerIndex = 0;
    UInt32 dciContainerIndex = 0;
    UInt8 * cceIndexArr_p = PNULL;
    UInt8 * writeIndex_p = PNULL;
    UInt32 harqSubFrameNum = 0;
    /* SPR 15909 fix start */
    /* +- SPR 18490 */
    tickType_t currentTickWithDelay = currentTick + delayToPhy;
    /* SPR 15909 fix end */
    EncoderNode *encoderNode_p = PNULL;

    UInt32 nodeCount = 0;

    harqSubFrameNum = subFrameNumber + delayToPhy + 4;

    if (harqSubFrameNum >= NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME)
    {
        harqSubFrameNum -= NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    }
    /* +- SPR 18490 */

    dciContainerIndex = currentTickWithDelay % MAX_CCE_CONTAINER;
    encoderContainerIndex = dciContainerIndex;

    /* + SPR 11209 Changes */
    /* Currently using 2nd Index of encoderContainerArr as internalCellIndex 
     * Need to change later */
    encoderNode_p = &((encoderContainerArr_gp[internalCellIndex][rlcCellIndex] + encoderContainerIndex)->\
            encoderNode); 
    /* - SPR 11209 Changes */

    cceIndexArr_p = encoderNode_p->cceIndexArr; 

    encoderNode_p->writeIndex = 0;
    encoderNode_p->readIndex = 0;

    writeIndex_p = &(encoderNode_p->writeIndex);

    encoderReTxQueue_p = &(encoderNode_p->encoderQueueForScheduler);

    nodeCount = sQueueCount(encoderReTxQueue_p);

    while(nodeCount--)
    {
        encoderQueueNode_p = (EncoderQueueNode *)popNode(encoderReTxQueue_p);
        if (encoderQueueNode_p != PNULL)
        {
            if (MAC_SUCCESS == processEncoderReTxNode(encoderQueueNode_p, 
                        currentTick,
                        delayToPhy,
                        dciContainerIndex,
                        internalCellIndex
/* + SPR 15127 changes */
#ifdef TDD_CONFIG                        
                        ,subFrameNumber 
#endif                                  
/* - SPR 15127 changes */
                        /* + SPR_14619 */
                        /* removed extra parameter passed */
                        /* + SPR_14619 */
                        ))

            {
                /* +- SPR 17777 */
                populateTXInfo(
                        isTxFunctionalityAdded,
                        encoderQueueNode_p->cceIndex,
                        cceIndexArr_p,
                        writeIndex_p
                        );
                /* +- SPR 17777 */
                /* Stage-III SCELL Deactivation Timer Related Changes Start */
                    ueDLContext_p    = dlUECtxInfoArr_g[encoderQueueNode_p->ueIndex].dlUEContext_p;
                    if(ueDLContext_p)
                    {
                        scellStateMngrUpdateScellDeactTimer(ueDLContext_p,internalCellIndex);
                        /* SPR 14954 Fix Start */
                        ueDLContext_p->advlastScheduledTick = currentTick + delayToPhy;
                        /* SPR 14954 Fix End */
                    }
                /* Stage-III SCELL Deactivation Timer Related Changes End */
            }
            freeMemPool((void *)encoderQueueNode_p);
            encoderQueueNode_p = PNULL;
        }
    }
    /* TO be reset by transmitter , uncomment if enhancement added in tx*/
    cceIndexArr_p[(*writeIndex_p)++] = INVALID_CCE_INDEX;
}
#endif

/* Stage-III SCELL Deactivation Timer Related Changes Start */
/*****************************************************************************
 * Function Name  : scellStateMngrUpdateScellDeactTimer
 * Inputs         : ueDLContext_p - DLUE context of the UE 
 *                  internalCellIndex
 * Outputs        : Shall update the Scell Deactivation Timer Node if valid 
 *                  timer node presents
 * Returns        : None
 * Description    : The funtion shall removes the timer node from the tick
 *                  (expiryindex in scellDeactExpiryTimerList_gp) which it
 *                  is about to expire and move the timer node to next
 *                  expiryIndex in scellDeactExpiryTimerList_gp
 *****************************************************************************/
STATIC void scellStateMngrUpdateScellDeactTimer(DLUEContext *ueDLContext_p,InternalCellIndex internalCellIndex)
{
    DLUESCellContext      *dlUeScellContext_p     = PNULL;
    ScellDeactTimerNode   *scellTimerNode_p       = PNULL;
    /* SPR 15909 fix start */
    tickType_t                 expiryTTI              = INVALID_SCELL_DEACT_EXPIRY_TTI;
    /* SPR 15909 fix end */
    UInt8                  sCellArrIndex          = INVALID_SERV_CELL_INDEX;

    sCellArrIndex  = getServeCellIndexFromInternalCellId(ueDLContext_p, internalCellIndex);
    if(INVALID_SERV_CELL_INDEX != sCellArrIndex)
    {
        /* if scell is in ACTIVE State */
        if(IS_SCELL_ACTIVE_AT_TTI(ueDLContext_p->ueIndex, sCellArrIndex))
        {
            dlUeScellContext_p = ueDLContext_p->dlUeScellContext_p[sCellArrIndex];
            scellTimerNode_p   = dlUeScellContext_p->scellStateVariables.scellTimerNode_p;
            /*scellDeactivationTimer in ueDLContext_p is in radio frame.
            * For subframe level it has to be multiplied by 10*/
            if((scellTimerNode_p) && (INVALID_SCELL_DEACT_TIMER != ueDLContext_p->scellDeactivationTimer)&& \
                (scellTimerNode_p->expiryTTI != INVALID_SCELL_DEACT_EXPIRY_TTI))
            {
                expiryTTI  = getCurrentTick() + (ueDLContext_p->scellDeactivationTimer * 10) + PHY_DL_DELAY + SCELL_DEACT_NEXT_TICK;
                /*Update the expiryTTI again */
                /*First remove the timer node from the tick which it was about to expire*/
                scellStateMngrStopScellDeactTimer(dlUeScellContext_p, internalCellIndex);
                /* Now move the timer node in new updated tick */
                scellStateMngrStartSCellDeactTimer(ueDLContext_p, expiryTTI, sCellArrIndex, internalCellIndex);
            }
        }
    }
    return;
}
/* Stage-III SCELL Deactivation Timer Related Changes End */
