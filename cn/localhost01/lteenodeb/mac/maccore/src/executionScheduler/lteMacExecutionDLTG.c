/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacExecutionDLTG.c $
 *
 ******************************************************************************
 *
 *  File Description : DLTG Thread Implementation
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * Revision 1.1  2011/9/20 17:37:08  gur26001
 * DLTG Layer2 merge code
 *****************************************************************************/
/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129058) */
#ifdef LAYER2_DLTG_THREAD
/* SPR 5599 changes end (ZIP ID 129058) */
/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteTypes.h"
#include "lteMisc.h"
#include "lteLayer2CommanTypes.h"
#include "lteLayer2DLTG.h"
#include "logging.h"
#include "egtpu_glob.h"
#ifdef L2_FAST_PATH
#include "lteMsgPool.h"
#endif
#ifdef LAYER2_DLTG_THREAD
#include "../../../../simulators/trafficGenerator/src/gtpSimulator.c" 
#endif
/******************************************************************************
  Private Definitions
 *****************************************************************************/
#define SEQ_MISMATCH_VALIDATION
#define MSG_POOL_HIGH_WATERMARK         70
#define MSG_POOL_LOW_WATERMARK          60
#define MEM_POOL_HIGH_WATERMARK         70
#define MEM_POOL_LOW_WATERMARK          60

#define MAX_LC_ID 8

LTE_SQUEUE dlTgDataPktQ_g;
SInt16 lastUe_g = -1;
UInt32 missTickCount_tx_g;
UInt64 missTimeCount_tx_g;
UInt32 seqNum_g;

/*Scheduled UE*/
UInt16    ueTurn_g = 0; 
UInt16    lcId_g = 0; 
//FILE      *MissCntrs_g = NULL;
UInt64 txTickCount_g =0;
UDouble32 totalNoOfPktVar_g ;
UInt32 packetSize_g, noOfPktVar_g,timeInMs_g,timeInMs_g_rx;
/*SPR 15909 Fix Start*/
tickType_t noOfTicks_g;
UInt32 fixedPktPerTick_g, remainingPkt_g, extraBurstCounter_g;
/*SPR 15909 Fix End*/
UInt32 pktCounter_g;
SInt16 ueId_rx;
UDouble32  totalThrputVar_g;
UDouble32  totalNoOfPktVar_g;
UInt32 trafficProfileVar_g;
UInt32 dlTgCoreNum_g;
UInt32 putDisplayVar_g;
UInt8 * pDlDataQueue = PNULL;

UInt16    ueStart_g = 0; 
SInt16 ueId_g;

#ifdef L2_FAST_PATH
#ifndef UE_SIM_TESTING
extern void lteUserPlaneFastPath(UInt8 *recvData_p, UInt32 bytesRead);
#endif
#endif
UInt32 processPeerGtpuDataInd
(
    UInt8 *gtpuDataMsg_p,
    UInt32 transId,
    UInt32 totalLen);


extern UInt32 processPeerGtpuDataInd
(
    UInt8 *gtpuDataMsg_p,
    UInt32 transId,
    UInt32 totalLen
);

 
/****************************************************************************
* Function Name  : lteMacCheckDLTGInput
* Inputs         : 
*                  ueId - UE Identification ID,
*                  ueStart - UE Start,
*                  argcount - Count of Arguments
* Outputs        : None
* Returns        : SUCCESS
* Description    : This function cheacked the command line argument given 
*                  by the user for DLTG.
****************************************************************************/
/* +- SPR 17777 */
UInt8 lteMacCheckDLTGInput(
        /* +- SPR 17777 */
                           UInt32 ueId, 
                           UInt32 ueStart,
                           int argcount)
{
    if(argcount > 8)
    {
        ueId_g = ueId;
    	/* SPR 15553 fix start */
        if(ueId_g < 0 || ueId_g >= MAX_UE_SUPPORTED)
        {
    	    /* SPR 15553 fix end */
            ueId_g = 0;
        }
        fprintf(stderr,"Rigbht ueId is %d\n",ueId_g);
    }
//    MissCntrs_g= fopen("UL_dropped_seq.txt","w+");
    if(argcount > 9)
    {
      ueStart_g = ueStart;
     /* SPR 3444 changes start */
     /*  ueStart_g can never be negative
      *  if(ueStart_g < 0 || ueStart_g >= MAX_UE_SUPPORTED)
      */
      if(ueStart_g >= MAX_UE_SUPPORTED)
     /* SPR 3444 changes end */
      {
          ueStart_g = ueId_g;
      }
      if(ueId_g < ueStart_g)
      {
         ueStart_g = ueId_g;
      }
    }
    fprintf(stderr,"\n DL TG Traffic Started %d to %d UE \n", ueStart_g, ueId_g);
    return 0;
}

UInt8 * dlTempMsgPtr = PNULL;
/****************************************************************************
* Function Name  : normalModeForMacExecSchedular
* Inputs         : None
* Outputs        : None
* Returns        : None
* Description    : This function DEQUEUE the dltg data packet and process it.
****************************************************************************/
void normalModeForMacExecSchedular( void )
{
    static UInt8 msgPoolOverFlow;
    static UInt8 memPoolOverFlow;
    unsigned long  packetInSingleCycle = 0;
    SInt32 bytesRead = -1;
    struct timespec timer;
    struct timespec tp1, tp2;
    UInt8 *recvData_p = NULL;
    UInt32 transactionId = 0;
    dlTgDataPkt* dlTgDataPkt_p = NULL;
    long long nsec;
    UInt32 msgPoolUsage = 0;
    UInt32 memPoolUsage = 0;
    /* SPR 15909 fix start */
/* SPR 21804 Fix Start*/
#ifndef PDCP_GTPU_INTF
    tickType_t currentTickCount = getCurrentTick();
#else
    tickType_t currentTickCount = relayGtpuCurrentTick_g;
#endif
    /* SPR 15909 fix end */

    timer.tv_sec = 0;
    timer.tv_nsec = 200000;
    /* SPR 21804 Fix End */
    clock_gettime_real_time(&tp1);

     msgPoolUsage = qvMsgPoolGetUsagePercentage();
     if (MSG_POOL_HIGH_WATERMARK < msgPoolUsage)
     {
         msgPoolOverFlow = 1;
         LOG_MAC_MSG(PR_MSG_POOL_HIGH_WATERMARK, LOGINFO,PR_GTPU_DL_DATA,
                  currentTickCount ,msgPoolUsage,0,0,0,0,
                  0,0,__func__,"Stopping traffic");
     }
     else if (MSG_POOL_LOW_WATERMARK > msgPoolUsage)
     {
         msgPoolOverFlow = 0;
         LOG_MAC_MSG(PR_MSG_POOL_LOW_WATERMARK, LOGINFO, PR_GTPU_DL_DATA,
                  currentTickCount ,msgPoolUsage,0,0,0,0,
                  0,0,__func__,"Restarting traffic");

     }
     memPoolUsage = qvMemPoolGetUsagePercentage();
     if (MSG_POOL_HIGH_WATERMARK < memPoolUsage)
     {
         memPoolOverFlow = 1;
         LOG_MAC_MSG(PR_MEM_POOL_HIGH_WATERMARK, LOGINFO, PR_GTPU_DL_DATA,
                  currentTickCount ,memPoolUsage,0,0,0,0,
                  0,0,__func__,"Stopping traffic");

     }
     else if (MSG_POOL_LOW_WATERMARK > memPoolUsage)
     {
         memPoolOverFlow = 0;
         LOG_MAC_MSG(PR_MEM_POOL_LOW_WATERMARK, LOGINFO, PR_GTPU_DL_DATA,
                  currentTickCount ,memPoolUsage,0,0,0,0,
                  0,0,__func__,"Restrating traffic");

     }

    while(1) {
        if (QCOUNT_DLTG_DATA_PKT_Q())
        {
            dlTgDataPkt_p = DEQUEUE_DLTG_DATA_PKT_Q();
            if (dlTgDataPkt_p)
            {
                bytesRead = dlTgDataPkt_p->dataLength;
                recvData_p = dlTgDataPkt_p->dlTgDataPktBuf_p;
                dlTempMsgPtr = recvData_p;
            }
            else
            {   
                pselect_wrapper(0, NULL, NULL, NULL, &timer, NULL);
                return;
            }   
            /* Memory usage has hit high water mark, drop the packet */
            if (1 == msgPoolOverFlow || 1 == memPoolOverFlow)
            {
                if (dlTgDataPkt_p)
                {
/* SPR_7214_CHANGE_START */
#ifdef PRINT
                    lteLogDropedPkt("dropping packet watermark msgPoolUsage[%d] "
                             "memPoolUsage[%d]\n", msgPoolUsage, memPoolUsage);
#endif 
/* SPR_7214_CHANGE_END */
                    if (dlTgDataPkt_p->dlTgDataPktBuf_p)
                    {
                        freeMemPool(dlTgDataPkt_p->dlTgDataPktBuf_p);
                        dlTempMsgPtr = NULL;
                    }
                    freeMemPool(dlTgDataPkt_p);
                }
                dlTgDataPkt_p = NULL;
                recvData_p = NULL;
                return;
            }
            if (bytesRead > 0 && recvData_p)
            {
#ifdef L2_FAST_PATH
#ifndef UE_SIM_TESTING
              
              lteUserPlaneFastPath(recvData_p, bytesRead); 

              /* This pointer variable should have been null after its Usage */
              /* In Fast Path it is not used, setting it as NULL */
              /* In Slow Path via Fast Path after its Usage it should be NULL */ 
              dlTempMsgPtr = NULL;
#endif
#else

              processPeerGtpuDataInd(recvData_p,
                  transactionId++,
                  bytesRead);
#endif
                if (dlTgDataPkt_p)
                {
                    freeMemPool(dlTgDataPkt_p);
                }
                dlTgDataPkt_p = NULL;
                if (dlTempMsgPtr != PNULL)
                {
                    freeMemPool(dlTempMsgPtr);
                    dlTempMsgPtr = PNULL;
                }
                recvData_p = NULL;
            }
            else if (bytesRead <= 0)
            {
                return;
            }

            /* If sufficient packets are received in 1 ms then break */
            packetInSingleCycle += bytesRead;
        }
        else
        {   
            pselect_wrapper(0, NULL, NULL, NULL, &timer, NULL);
            return;
        }   
        clock_gettime_real_time(&tp2);
	nsec = tp2.tv_sec * 1000000000 + tp2.tv_nsec - tp1.tv_sec * 1000000000 - tp1.tv_nsec;
        if (nsec > 200000)
            return;
    } /* End of While */
}


#endif


