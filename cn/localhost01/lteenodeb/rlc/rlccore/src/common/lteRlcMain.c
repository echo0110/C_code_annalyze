/******************************************************************************
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  File: lteRlcMain.c
 *
 ******************************************************************************
 *
 *  File Description : The RLC layer Execution Legs implementation 
 *
 ******************************************************************************/
/*SPR 21727 Changes Start*/
#if RLC_PDCP_IPC_INTF && !UE_SIM_TESTING
/*SPR 21727 Changes End*/
#include "lteTypes.h"
#include "lteMisc.h"
#include "lteRlcRxProcessor.h"
#include "lteRlcRrcInterface.h"
#include "lteRlcUeContext.h"
#include "lteRlcTmdSdu.h"
#include "lteRlcTmRxOperation.h"
#include "lteLayer2CommanTypes.h"
#include "lteRlcAmRxOperation.h"
#include "logging.h"
#include "alarm.h"
#include "lteCommonStatsManager.h"
#include "lteRlcErrorCode.h"
#include "lteRlcEntityDef.h"
#include "lteRlcMem.h"
#include "lteRlcPdcpInterface.h"
#include "lteCommonQueueDefs.h"

#ifdef FLEXRAN
#include <sys/prctl.h>
#endif

/*************************** EXTERNAL GLOBAL VARIABLE*********************/
extern tickType_t globalTTITickCount_g;
extern UInt16 rlcLayerInitialized_g;
extern LTE_CIRCQUEUE pdcpRlcUeDataDiscardReqQ_g;
extern UInt8 numOfRlcCellConfigured_g;
extern RLC_TRIGGER_STATUS_REPORT_Q rlcTriggerStatusReportQueue_g[MAX_NUM_CELL];
extern LTE_CIRCQUEUE rlcReconfigTxEntityQ_g;
extern LTE_CIRCQUEUE rlcReconfigRxEntityQ_g;

/*************************** EXTERNAL FUNCTIONS **************************/
extern LTE_CIRCQUEUE rlcUlSchedToLPCircQ_g;
extern LTE_CIRCQUEUE rlcLPtoRlcUlSchedCircQ_g; 


/*************************** EXTERNAL FUNCTIONS **************************/
extern UInt16 pdcpContextIsInitialized(void);
extern void processRlcTriggerStatusReport(UInt32 statusReportsQueueCount,InternalCellIndex cellIndex);
extern UInt32 rlcHandleDataFromMac(UInt32 count,InternalCellIndex internalCellIndex);
extern void  expireUMROTimer(InternalCellIndex cellIndex);
extern void expireAMROTimer(InternalCellIndex cellIndex);
extern UInt32 rlcProcessReEstabMsg(LTE_CIRCQUEUE *reEstabQ_p);
extern UInt32 rlcProcessTxReconfigMsg(LTE_CIRCQUEUE *rlcReconfigTxEntityQ_p);
extern UInt32 rlcProcessRxReconfigMsg(LTE_CIRCQUEUE *rlcReconfigRxEntityQ_p);
extern UInt32 rlcHandleUeDataDiscardReq(UInt32 cQCount);
extern UInt32 rlcHandleUeDataReq(UInt32 cQCount);





/**************************************************************************
 * Function: rlcULExecutionScheduler
 * 
 * Description: This is entry function for UL RLC thread.
 *
 * ***********************************************************************/
void * rlcULExecutionScheduler(void * param)
{
        InternalCellIndex cellCount = 0;
        UInt32 cQcount = 0;
	UInt32 ncount = 0;
#ifdef OBSELETE_REL_8
        UInt32 sQcount = 0;
#endif

        threadSetRtPriority(SCHED_FIFO, 20);
#ifdef FLEXRAN
        PROCESS_TO_BIND_SET_CORE_NO(syscall(SYS_gettid), lteLayer2CoreInfo_g.lteLayer2UlRlcCoreNum);
        prctl(PR_SET_NAME, "lte_rlc", NULL, NULL, NULL);
#else
        PROCESS_TO_BIND_SET_CORE_NO(syscall(SYS_gettid), lteLayer2CoreInfo_g.lteLayer2PdcpCoreNum);
#endif

        char shmRegionName[NAME_MAX];
        snprintf(shmRegionName, NAME_MAX, "%s", LTE_L2_LOG_REGION_SEVEN_NAME);
        logger_init(LTE_L2_LOG_REGION_SEVEN_ID, LTE_L2_LOG_REGION_SEVEN_NAME, l2Input.shmSize );

        while(1)
	{

		for (cellCount = 0; cellCount< MAX_NUM_CELL; cellCount++)
		{
			l2ThreadHealthStoreTick_g.rlcMacThread[cellCount].rlcThread[RLC_UL_THREAD] = globalTTITickCount_g;
		}

		if(RLC_FAILURE != rlcLayerInitialized_g)
		{

#ifdef RLC_PDCP_IPC_INTF
                   /* + EL : RLC_HANDLE_UE_DATA_REQ */
		   cQcount = circQueueCount(&pdcpRlcUeDataReqQ_g);
		   if(cQcount)
		   {
		      rlcHandleUeDataReq(cQcount);
		   }
                   /* - EL : RLC_HANDLE_UE_DATA_REQ */

                   /* + EL : RLC_HANDLE_UE_DATA_DISCARD_REQ */
		   cQcount = circQueueCount(&pdcpRlcUeDataDiscardReqQ_g);
		   if(cQcount)
		   {
		      rlcHandleUeDataDiscardReq(cQcount);
		   }
                   /* - EL : RLC_HANDLE_UE_DATA_DISCARD_REQ */
#endif
                   /* + EL : RLC_HANDLE_HARQ_FAILURE_IND */
#ifdef OBSELETE_REL_8
		   RDTSC_AND_CLOCK_START_FDD(RLC_HANDLE_HARQ_FAILURE_IND );
		   sQcount = sQueueCount(&macUeHarqFailureIndQ_g);
		   if(sQcount)
		   {
		      rlcHandleHarqFailureInd(sQcount);
		   }
		   for(cellCount = 0; cellCount < numOfRlcCellConfigured_g; cellCount++)
		   {
		       RDTSC_AND_CLOCK_END_RLC(RLC_HANDLE_HARQ_FAILURE_IND, cellCount);
		   }
#endif
                   /* - EL : RLC_HANDLE_HARQ_FAILURE_IND */

                   /* + EL : RLC_TRIGGER_STATUS_REPORT */
                   for( cellCount = 0; cellCount < numOfRlcCellConfigured_g; cellCount++)
                   {
                      cQcount = COUNT_RLC_TRIGGER_STATUS_REPORT_Q(cellCount);
                      if(cQcount)
                       {
                         processRlcTriggerStatusReport(cQcount,cellCount);
                       }
                   }
                   /* - EL : RLC_TRIGGER_STATUS_REPORT */

                   /* + EL : RLC_HANDLE_DATA_FROM_MAC */
		   for( cellCount = 0; cellCount < numOfRlcCellConfigured_g; cellCount++)
                   {
                      RDTSC_AND_CLOCK_START_FDD(RLC_HANDLE_DATA_FROM_MAC);
                      cQcount = COUNT_MAC_UE_DATA_IND_Q(cellCount);
                      if(cQcount)
                      {
                         rlcHandleDataFromMac(cQcount,cellCount);
                      }
                      RDTSC_AND_CLOCK_END_RLC(RLC_HANDLE_DATA_FROM_MAC,cellCount);
                   }
                   /* - EL : RLC_HANDLE_DATA_FROM_MAC */

                   /* + EL : RLC_TIMER_UPD_TICKCHECK_FOR_TIMER_EXE */
                   RDTSC_AND_CLOCK_START_FDD(RLC_TIMER_UPD_TICKCHECK_FOR_TIMER_EXE);
                   for(cellCount = 0;cellCount < numOfRlcCellConfigured_g;cellCount++)
                   {
                       expireUMROTimer(cellCount);
                       expireAMROTimer(cellCount);
                       RDTSC_AND_CLOCK_END_RLC(RLC_TIMER_UPD_TICKCHECK_FOR_TIMER_EXE, cellCount);
                   }
                   /* - EL : RLC_TIMER_UPD_TICKCHECK_FOR_TIMER_EXE */

                   /* + EL : RLC_DL_REESTABLISH_Q */
                   for(cellCount = 0; cellCount < numOfRlcCellConfigured_g; cellCount++)
                   {
                       RDTSC_AND_CLOCK_START_FDD(RLC_HDLR_REESTABLISH_Q);
                       cQcount=circQueueCount(&rlcTxReEstabQ_g[cellCount]);
                       if (cQcount)
                       {
                           rlcProcessReEstabMsg(&rlcTxReEstabQ_g[cellCount]);
                       }
                       RDTSC_AND_CLOCK_END_RLC(RLC_HDLR_REESTABLISH_Q,cellCount);
                   }
                   /* - EL : RLC_DL_REESTABLISH_Q */

                   /* + EL : RLC_UL_REESTABLISH_Q */
                   RDTSC_AND_CLOCK_START_FDD(RLC_HDLR_REESTABLISH_Q);
                   cQcount=circQueueCount(&rlcRxReEstabQ_g);
                   if (cQcount)
                   {
                       rlcProcessReEstabMsg(&rlcRxReEstabQ_g);
                   }
                   RDTSC_AND_CLOCK_END_RLC(RLC_HDLR_REESTABLISH_Q,0/*cellCount*/)
                   /* - EL : RLC_UL_REESTABLISH_Q */

                   if (ncount == 10)
                   {
                       /* + EL : RLC_DL_PEN_DEL_Q */ 
                       RDTSC_AND_CLOCK_START_FDD(RLC_HDLR_PEN_DEL_Q);
                       cQcount=circQueueCount(&rlcReconfigTxEntityQ_g);
                       if(cQcount)
                       {
                          rlcProcessTxReconfigMsg(&rlcReconfigTxEntityQ_g);
                       }
                       for(cellCount = 0; cellCount < numOfRlcCellConfigured_g; cellCount++)
                       {
                           RDTSC_AND_CLOCK_END_RLC(RLC_HDLR_PEN_DEL_Q, cellCount);
                       } 
                       /* - EL : RLC_DL_PEN_DEL_Q */

                       /* + EL : RLC_UL_PEN_DEL_Q */ 
                       RDTSC_AND_CLOCK_START_FDD(RLC_HDLR_PEN_DEL_Q);
                       cQcount=circQueueCount(&rlcReconfigRxEntityQ_g);
                       if(cQcount)
                       {
                          rlcProcessRxReconfigMsg(&rlcReconfigRxEntityQ_g);
                       }
                       for(cellCount = 0; cellCount < numOfRlcCellConfigured_g; cellCount++)
                       {
                           RDTSC_AND_CLOCK_END_RLC(RLC_HDLR_PEN_DEL_Q, cellCount);
                       }
                       /* - EL : RLC_UL_PEN_DEL_Q */
  
                       ncount=0; 
                   }
                   /* SPR 21221 Fix Start */
                   UInt32 sQcount = COUNT_RLC_LP_TO_RLC_UL_SCH_Q(); 
                   while(sQcount--)
                   {
                       RlcRrcOamHiPrioQNode *recvMsgBuf_p  = PNULL;
                       DEQUEUE_RLC_LP_TO_RLC_UL_SCH_Q(RlcRrcOamHiPrioQNode, (void**)&recvMsgBuf_p);
                       if(recvMsgBuf_p)
                       {
                           if(!(ENQUEUE_RLC_UL_SCH_TO_LP_Q(RlcRrcOamHiPrioQNode, recvMsgBuf_p)))
                           {
#ifdef PRINT
                               fprintf(stderr," Failed to Enqueue node in rlcUlSchedToLPCircQ_g for UE[%d]\n", recvMsgBuf_p->ueIndex);
#endif
                           }
                       }
                   }
                   /* SPR 21221 Fix End */
		}
		else
		{
		   usleep(200);
		}
		usleep(200);
	}
}
#endif
