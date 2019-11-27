/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2RLCInterface.c,v 1.4.6.2.4.2.2.3 2010/10/29 10:50:10 gur20056 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains interface functions between the
 *                     MAC and the RLC Layer in the LTE system. It contains
 *                     different functions for:
 *                     - Requesting data from the RLC Layer for different 
 *                       logical channels for a UE.
 *                     - Updation of the queueload for a Logical Channel by
 *                       RLC.
 *                     - Requesting the buffer occupancy for a UE's Logical 
 *                       Channel.
 *                     - Sending an ARQ indication to RLC.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLayer2RLCInterface.c,v $
 * Revision 1.4.6.2.4.2.2.3  2010/10/29 10:50:10  gur20056
 * SPR  618 fix - deepa
 *
 * Revision 1.4.6.2.4.2.2.2  2010/10/14 10:04:22  gur23971
 * RLC BUG SOLVED
 *
 * Revision 1.4.6.2.4.2.2.1  2010/09/21 15:53:34  gur20491
 * FAPI changes
 *
 * Revision 1.4.6.2  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.4  2010/02/18 15:08:59  gur15697
 * logs removed
 *
 * Revision 1.3  2010/02/18 15:07:32  gur15697
 * printf commented
 *
 * Revision 1.2  2010/02/13 10:47:06  gur15697
 * changes for UL flow Stability
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.21  2009/11/14 09:52:43  gur11912
 * modified for numOfSubHdrCount
 *
 * Revision 1.20  2009/11/04 10:40:49  gur11912
 * Fixed for End Padding( >2 Bytes)
 *
 * Revision 1.19  2009/11/04 07:42:25  gur11912
 * set FormatIndicator to 1 incase LCID Length is greater than 128
 *
 * Revision 1.18  2009/09/22 10:21:14  gur19413
 * commented the unwanted func
 *
 * Revision 1.17  2009/09/17 13:40:15  gur11912
 * removed unused variables
 *
 * Revision 1.16  2009/09/17 12:40:34  gur11912
 * modified for SR
 *
 * Revision 1.15  2009/09/16 11:00:12  gur19413
 * optimised code
 *
 * Revision 1.14  2009/09/03 14:25:57  gur20439
 * ltePanic put in macro LTE_MAC_UT_LOG.
 *
 * Revision 1.13  2009/08/31 14:48:53  gur11912
 * fixed for uplink flow in case padding is zero
 *
 * Revision 1.12  2009/07/29 15:19:11  gur20439
 * look and feel to simulator fix.
 *
 * Revision 1.11  2009/07/28 14:47:14  gur15697
 * UT bugfix
 *
 * Revision 1.10  2009/07/15 14:25:02  gur20439
 * bug fix for no BSR.
 *
 * Revision 1.9  2009/07/14 14:48:54  gur20439
 * rrc interface code change.
 *
 * Revision 1.8  2009/07/12 10:17:06  gur15697
 * removed compilation warnings
 *
 * Revision 1.7  2009/07/12 08:11:46  gur15697
 * UL flow bug fix
 *
 * Revision 1.6  2009/06/25 11:46:04  gur18569
 * fixed UT bugs
 *
 * Revision 1.5  2009/06/15 13:54:51  gur20439
 * modified for release 1.2.
 *
 * Revision 1.4  2009/06/01 10:53:31  gur20439
 * include/interfaces/phySimulatorApiHandler.h
 *
 * Revision 1.3  2009/05/26 07:29:28  gur20439
 * file modified for mac release 1.2.
 *
 * Revision 1.2  2009/05/01 06:58:39  gur20439
 * msg type include As SR.
 *
 * Revision 1.1  2009/03/30 10:20:41  gur19836
 * Initial Version
 *
 * Revision 1.26  2009/03/23 08:32:30  gur19140
 * Integration changes first waves
 *
 * Revision 1.25  2009/03/13 08:00:08  gur19836
 * Modified for OAM and RLC Interface Changes
 *
 * Revision 1.24  2009/03/02 11:00:13  gur18569
 * removed funcNamp_p and replaced it with __func__
 *
 * Revision 1.23  2009/01/27 16:04:59  gur18569
 * commented macUpdateBufferOccupancyReq invoked log
 *
 * Revision 1.22  2009/01/27 09:43:14  gur18569
 * review comments incorporated
 *
 * Revision 1.21  2009/01/23 12:55:21  gur18569
 * shifted macUeDataInd to this file
 *
 * Revision 1.20  2009/01/19 15:43:21  gur18569
 * name changes for RLC interfaces
 *
 * Revision 1.19  2009/01/16 14:24:49  gur18569
 * added num of lcs field in sendArqIndication
 *
 * Revision 1.18  2009/01/10 09:02:54  gur11912
 * Phase2 UT Defects fixed
 *
 * Revision 1.17  2009/01/08 18:56:19  gur18569
 * Review comments incorporated
 *
 * Revision 1.16  2009/01/05 12:57:41  gur18569
 * added new arg in receiveRLCData i.e requestFailure flag
 *
 * Revision 1.15  2008/12/26 05:49:28  gur11912
 * enumDataType used
 *
 * Revision 1.14  2008/12/23 12:05:32  gur11912
 * code review comments incorporated
 *
 * Revision 1.13  2008/12/23 05:06:52  gur11912
 * comment incorporatd for phase2
 *
 * Revision 1.12  2008/12/19 06:52:42  gur18569
 * review comments incorporated for phase2
 *
 * Revision 1.11  2008/12/11 14:23:22  gur11912
 * Downlink Phase 2 changes
 *
 * Revision 1.10  2008/12/05 12:17:36  gur18569
 * multicore changes
 *
 * Revision 1.9  2008/10/22 12:35:01  gur11912
 * fixed for performance changes
 *
 * Revision 1.8  2008/10/17 09:24:00  gur19836
 * Enhancement: cleanupMACLayer functionality added
 *
 * Revision 1.7  2008/10/14 06:16:37  gur11912
 * ARQ Interfaces modified and Log level added
 *
 * Revision 1.6  2008/09/27 15:44:20  gur11912
 * UT Defect Fixed
 *
 * Revision 1.5  2008/09/26 07:20:04  gur11912
 * UT Defect Fixed
 *
 * Revision 1.4  2008/09/19 12:20:15  gur11912
 * Updated for Integration
 *
 * Revision 1.3  2008/09/19 07:13:36  gur11912
 * review comments incorporated
 *
 * Revision 1.2  2008/09/10 04:50:53  gur11974
 * Updated for RLC simulator
 *
 * Revision 1.1  2008/09/09 05:12:53  gur11974
 * Initial Version
 *
 *
 ****************************************************************************/


/****************************************************************************
* Standard Library Includes
****************************************************************************/
//#define ENABLE_FLOW_DEBUG
#ifdef ENABLE_FLOW_DEBUG
  #define FLOW_DEBUG     fprintf
#else
  #define FLOW_DEBUG(...)
#endif


/****************************************************************************
* Project Includes
****************************************************************************/
#include "lteLayer2Types.h"
#include "lteLayer2RLCInterface.h"
#include "lteLayer2UEContext.h"
#include "lteLayer2UplinkUEContext.h"
#include "lteFapiControlMsgHandler.h"
#include "phySimulatorApiHandler.h"
/****************************************************************************
 Private Definitions
****************************************************************************/
/* +CA_NA */
extern UInt32 stoppedProcessing_g[MAX_NUM_CELLS];
/* -CA_NA */
extern UInt8 phrFlag_g;

/****************************************************************************
*Private Types
****************************************************************************/

 /****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
* Private Constants
****************************************************************************/

/****************************************************************************
* Exported Variables
****************************************************************************/
extern UInt64 prepHeaderInfo(UInt8 **sendBufer_p,UInt8 transportInfo,
        UInt8 controlUlRa, UInt16 crntiOrRarnti,UInt32 ttiCounter);
extern ueContextForUplink *uplinkContextInfoMap_g[MAX_UE_SUPPORTED];
extern dynamicUEContext *dynUEContextInfoMap_g[MAX_UE_SUPPORTED];

extern void prepareSduSubHeader(UInt8** sendBufer_p,UInt8 R1,
        UInt8 R2, UInt8 E, UInt8 lcId,UInt8 format, UInt32 length);
extern UInt8 getBSRIdxfrmBytes( UInt32 bytes );
extern void prepareBSR(UInt8 **sendBufer_p,UInt8 isLongBSR,
        UInt8 bsr0,UInt8 bsr1, UInt8 bsr2, UInt8 bsr3,UInt8 lcId);
extern void fapiPrepareSRPdu(UInt16 crnti, UInt8 cellIndex);
extern QueuePointerNode ULControlAndDataArr_g[MAX_SUBFRAME];

extern PHRProfile phrProfileInfo_g[MAX_NUM_CELLS][MAX_PHR_PROFILES];
extern ueProfile ueProfileInfo_g[MAX_NUM_CELLS][MAX_UE_PROFILES];
/*Fix for SPR 11915*/
ttiBundlingHdrInfo ttiBundlingHdrInfo_g[MAX_UE_SUPPORTED];
/****************************************************************************
* Private Variables (Must be declared static)
****************************************************************************/

/*****************************************************************************
 * Function Name  : updateUEAndLCGQueueLoad 
 * Inputs         : ulUEContext_p - pointer to UE Context
 *                  qLoad - queue load that needs to be updated
 * Outputs        : Updated queue load of Uplink UE.
 * Returns        : None
 * Variables      :
 * Description    : This function updates the queue load of UE with the
 *                  value that has been sent in current TB for particular LCID.
 *****************************************************************************/
void updateUEAndLCGQueueLoad(ueContextForUplink *ulUEContext_p,
                             dynamicUEContext *ulDynUEContext_p,
                             SInt32 qLoad, SInt32 *lcgQLoad_p, 
                             UInt8 *isLcgQLoadPresent_p, UInt8 cellIndex)
{
    RNTIInfo *rntiInfo_p = PNULL;
    SInt32 oldQueueLoad = 0;
    UInt8 i = 0;
    UInt8 srTrigFlag = FALSE;
    SInt32 srTrigTimeDiff = 0;

    /* Check this for sending SR request when it becomes 0 to non zero */
    oldQueueLoad = ulUEContext_p->queueLoad;
    ulUEContext_p->queueLoad = qLoad;
    if(qLoad == 0)
    {
      ulUEContext_p->availGrantCounter = 0;
    }
    for(i=0;i<NUM_OF_LCG;i++)
    {
        if( TRUE == isLcgQLoadPresent_p[i] )
        {
            ulDynUEContext_p->bsrNet[i] = lcgQLoad_p[i];
        }
      //fprintf(stderr,"\n======== Setting LCG %d Qload [%d]\n",i,lcgQLoad_p[i]);
    }

    if (!(ulDynUEContext_p->bsrNet[0]+ ulDynUEContext_p->bsrNet[1]+ulDynUEContext_p->bsrNet[2]+ulDynUEContext_p->bsrNet[3]))
    {
      //fprintf(stderr,"Setting netQload to 0\n");
      //ulUEContext_p->queueLoad = 0; /* MRAB-MUE */
    }
//    fprintf(stderr,"-------- bsrNet = %d oldQueueLoad = %d ueContextQueueLoad = %d\n", ulDynUEContext_p->bsrNet[0],oldQueueLoad, ulUEContext_p->queueLoad);

//    fprintf(stderr,"updateUEAndLCGQueueLoad(): oldQueueLoad [%d] queueLoad [%d]\n",oldQueueLoad,ulUEContext_p->queueLoad);
//
 
 
        /* SPR 1298 */
        srTrigTimeDiff = globalTTITickCount_g - ulUEContext_p->latDci0RcvdTick;

#ifdef CONTINOUS_SR/* Fix for reporting SR at proper time*/
#ifdef FDD_CONFIG
        if(MAX_SUBFRAME < srTrigTimeDiff )
#elif TDD_CONFIG
        if(ulUEContext_p->srPeriodicity < srTrigTimeDiff )
#endif
        {
#else
        if(ulUEContext_p->availGrantCounter>=1) /* MRAB-MUE */
        {
#ifdef UT_TESTING
         fprintf(stderr,"\n No Grant Till %d TTI ",ulUEContext_p->availGrantCounter);
#endif
         ulUEContext_p->availGrantCounter = 0;
#endif
            srTrigFlag = TRUE;
        }

    if ( FALSE == ulUEContext_p->resetFlag ) 
    {
        /* SPR 1298 */
        if(((0 == oldQueueLoad) && (0 != ulUEContext_p->queueLoad)) ||
                ((0 != ulUEContext_p->queueLoad) && ( srTrigFlag )))  
        {
            /* Send SR Request for this UE */
            /*SPR 12404 fix, In case of inter Cell HO, ue-RLC was passing source cellIndex*/
            /*Fetching the cellIndex from RNTI*/  
            GET_RNTI_TO_CELL_INDEX(ulUEContext_p->crnti, cellIndex);
            rntiInfo_p =  getRntiInfofromRNTIMap(ulUEContext_p->crnti, cellIndex);
            if ( rntiInfo_p == PNULL )
            {
                lteWarning("UE index mismatch for crnti = %d \n", ulUEContext_p->crnti);
                return;
            }
            else if ( rntiInfo_p->ueIndex != ulUEContext_p->ueIdx )
            {
                lteWarning("UE index mismatch for crnti = %d ueIndex = %d\n", ulUEContext_p->crnti, ulUEContext_p->ueIdx);
                return;
            }
            rntiInfo_p->state = SG_WAIT;
            rntiInfo_p->rntiType = CRNTI; 
            if( ulUEContext_p->srTriggered == FALSE)/*Sending SR at proper time*/
            {
                fapiPrepareSRPdu(ulUEContext_p->crnti, cellIndex);
                ulUEContext_p->srTriggered = TRUE;/*Sending SR at proper time*/
                //fprintf(stderr,"SR Sent to MAC.....!!!!\n");
            }

        } 
    }
    return;
}


/****************************************************************************
 * Function Name  : updateBufferOccupancy
 * Inputs         : ueIndex -  UE Identifier
 *                  lcCount -  Number of logical channels
 *                  lcQueueLoadArr_p - Pointer to an array which contains
 *                                     queue loads for logical channels.
 *                  cellIndex - CELL Identifier 
 * Outputs        : None
 * Returns        : Success or Failure
 * Variables      : 
 * Description    : It calls the updateUEAndLCGQueueLoad() function for  
 *                  updating the Queue Load values for LCG and UE requested
 *                  by RLC. 
 ****************************************************************************/
UInt16 updateBufferOccupancy(UInt16 ueIndex, UInt16 lcCount,
                              LCQueueLoad * lcQueueLoadArr_p, UInt8 cellIndex)
{
    SInt32 lcgQLoad[NUM_OF_LCG] = {0};
    UInt8  isLcgQLoadPresent[NUM_OF_LCG] = {0};
    SInt32 tempValue = 0;
    UInt8 i=0;
    SInt8 lcId = -1;
    SInt32 qLoad = 0;
   UInt32 currSFNSF = 0;
   UInt8 refresh_lcg = FALSE;
   UInt8 srbBisDataFlag = LTE_FALSE;

    ueContextForUplink *ulUEContext_p = uplinkContextInfoMap_g[ueIndex];
    dynamicUEContext *ulDynUEContext_p = dynUEContextInfoMap_g[ueIndex];
   /* New Code for tracking the BO Request by RLC Start*/
   currSFNSF = (SFN_g[cellIndex] <<8 | SF_g[cellIndex]);
   if(ulUEContext_p->lastQUpdateTime != currSFNSF)
   {
     refresh_lcg = TRUE;
     ulUEContext_p->lastQUpdateTime = currSFNSF;
   }
   /* New Code for tracking the BO Request by RLC End*/

   tempValue =ulUEContext_p->queueLoad; /* MRAB-MUE */
    for (i = 0 ; i < lcCount; i++)
    {
        lcId = lcQueueLoadArr_p[i].lcId;
        qLoad = lcQueueLoadArr_p[i].lcQueueLoad;
        //if(2 /*RLC_AM_MODE*/ == ulUEContext_p->lcInfo[lcId].rlcMode)
        {
            qLoad += lcQueueLoadArr_p[i].ctrlQueueLoad + 
                lcQueueLoadArr_p[i].retxnPduSize;
        }
    
//        fprintf(stderr,"nupdateBufferOccupancy(): lcId [%d] qLoad [%d] lcGId [%d]\n",lcId,qLoad,ulUEContext_p->lcInfo[lcId].lcGId);

//        if(ulUEContext_p->lcInfo[3].lcGId == LCGID_INVALID_VALUE)
//        {
//            ulUEContext_p->lcInfo[3].lcGId = 3;
//            fprintf(stderr, "Setting lcGId to 3 for LC 3. Need to remove later once fix receive from Ue-state\n");
//        }
        if (lcId != -1 && (LCGID_INVALID_VALUE
                              != ulUEContext_p->lcInfo[lcId].lcGId))
        {
            /* MRAB-MUE */
   	    tempValue+=  (qLoad - ulUEContext_p->lcInfo[lcId].queueLoad); 
	    /* MRAB-MUE */
            ulUEContext_p->lcInfo[lcId].queueLoad = qLoad;
            if(refresh_lcg == TRUE)
            {
                /*Tauehed multi SR fix start */
#ifndef TDD_CONFIG
              ulUEContext_p->srTriggered = FALSE;
#endif
                /*Tauehed multi SR fix end */
              ulUEContext_p->availGrantCounter++;
              lcgQLoad[ulUEContext_p->lcInfo[lcId].lcGId] = qLoad;
              isLcgQLoadPresent[ulUEContext_p->lcInfo[lcId].lcGId] = TRUE;
              /* MRAB-MUE */ 
              //tempValue =qLoad; 
              /* MRAB-MUE */ 
#ifdef UT_TESTING
             fprintf(stderr,"\n START UEID %d LCGID %d LCID %d LCGQL/LCQL %d GNTCNT %d\n",
                                     ueIndex,ulUEContext_p->lcInfo[lcId].lcGId,lcId,qLoad,ulUEContext_p->availGrantCounter);
#endif
            }
            else
            {
              //ulUEContext_p->lcInfo[lcId].queueLoad = qLoad;
              lcgQLoad[ulUEContext_p->lcInfo[lcId].lcGId] += qLoad;
              isLcgQLoadPresent[ulUEContext_p->lcInfo[lcId].lcGId] = TRUE;
              //tempValue +=qLoad;  /* MRAB-MUE */
#ifdef UT_TESTING
              fprintf(stderr,"\n SYNC UEID %d LCGID %d LCID %d LCGQL/LCQL %d\n",
                                     ueIndex,ulUEContext_p->lcInfo[lcId].lcGId,lcId,qLoad);
#endif
            }
        }
    }
    updateUEAndLCGQueueLoad(ulUEContext_p, ulDynUEContext_p, tempValue, lcgQLoad, 
                            isLcgQLoadPresent, cellIndex );

    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : multiplexAndSendDataToPhysical
 * Inputs         : ueIndex - UE Identifier
 *                  lcCount - valid logical channel count.
 *                  transmissionAndRLCData_p - Pointer to buffer received
 * Outputs        :
 * Returns        : None.
 * Variables      :
 * Description    : Called by MAC when received data from RLC.
 *                  It multiplex the uplink data with BSR and send it to PHY.
 ****************************************************************************/
UInt16 multiplexAndSendDataToPhysical( UInt16 ueIndex,
                                       UInt8 lcCount,
                                       TransmissionAndRLCData *transmissionAndRLCData_p,
                                       RLCHarqFailureInfo *rlcHarqFailureInfo,
                                       UInt8 cellIndex,
                                       UInt8 ttiBundlingRtxFlag
                                     )
{
    UInt8 *sendBuffer_p = PNULL;
    UInt8 *header_p     = PNULL;
    UInt8 *data_p       = PNULL;
    UInt8 *start_p      = PNULL;
    UInt16 lcDataLen    = 0;
    UInt16 i            = 0;
    UInt32 lcSHLenArrIndex  = 0;
    UInt32 numOfSubHdrCount = 0;
    /** SPR 3657 Changes Start **/
    UInt32 rlcReTxPduCount = 0;
    rlcReTxPduNode *rlcReTxPduNode_p = PNULL;
    /** SPR 3657 Changes End **/
/* SPR 609 changes start */
#ifdef FAPI_UL_HARQ_ENABLE    
/* SPR 609 changes end */
    UInt16 currentSFNSF    = 0;
    UInt16 sendSFNSF    = 0;
/* SPR 609 changes start */
#endif    
/* SPR 609 changes end */
    UInt8* pduPointer=PNULL;
    /*SPR 1078 changes start */
    RLCHarqFailureInfo *rlcHarqFailureInfo_p=PNULL;
    /*SPR 1078 changes end */
    UInt8 bsr0 = 0;
    UInt8 bsr1 = 0; 
    UInt8 bsr2 = 0;
    UInt8 bsr3 = 0;
    //UInt8 bsrBuffer[24] = {0};
    //UInt8 *bsrBuffer_p  = bsrBuffer;
    UInt8 controlElementHeaderLen = 0;
    UInt8 sduSubHdrLen= 0;
    UInt8 controlElementTbLen = 0;
    UInt32 paddinglength = 0;
    UInt32 tbSize = 0;
    UInt8 bsrPresentFlag   = 0;
    UInt8 crntiCEFlag   = 0;
    UInt8 startPaddingFlag = FALSE; 
    ueContextForUplink   *ulUEContext_p = PNULL; 
    ulUEContext_p = uplinkContextInfoMap_g[ueIndex];
    ulschDataQueueNode *uldataNode_p = PNULL;
    UInt8 subFrame = 0;
    UInt8 qIndex = 0;

    /* Uplink ThroughPut Fix Start */
    UInt8 totRlcPduCount = 0;
    UInt8 lcPresentFlag  = FALSE; 
    UInt8 paddingFlag    = FALSE;
    UInt32 rlcLastPduLen = 0;
    /* Uplink ThroughPut Fix End */

    DLUEContext *newUEContext_p = PNULL;
    newUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    DLLogicalChannelInfo *dlLogicalChannel_p = PNULL;
    UInt8 lcID = 0;
/* SPR 609 changes start */
#ifdef FAPI_UL_HARQ_ENABLE    
/* SPR 609 changes end */
    extern volatile UInt16 SFN_g;
    extern volatile UInt8 SF_g;
    extern volatile unsigned long long printLogCnt;    
/* SPR 609 changes start */
#endif    
/* SPR 609 changes end */
#if 0
    FAPI_l1ApiMsg_st           *l1ApiMsg_p = PNULL;
    FAPI_rxULSCHIndication_st  *ulschDataInd_p = PNULL;
    FAPI_ulDataPduIndication_st *ulDataPdu_p = PNULL;
    UInt32                     l1ApiMsgSize = L1_PARTIAL_API_MSG_SIZE;  
#endif

   tbSize      = transmissionAndRLCData_p->tbSize;
#ifdef UT_TESTING    
    if ( tbSize == 0 )
    {
        ltePanic("in multiplexAndSendDataToPhysical: tbSize = 0 for ueIndex = %d\n", ueIndex);
    }
#endif    
    start_p = (UInt8 *) getMemFromPool(SBUFFERSIZE, PNULL);
    
  // ulDataPDU_p = transmissionAndRLCData_p->dataPtr_p;
  // dataLen_p   = transmissionAndRLCData_p->dataLen_p;
    //fprintf(stderr, "func [%s] tbSize dataPtr [%x]\n", __func__,*(transmissionAndRLCData_p->dataPtr_p));

    if (start_p == PNULL)
    {
        ltePanic("not able to get memory in multiplexAndSendDataToPhysical\n");
    }

#if 0    
    l1ApiMsg_p = ( FAPI_l1ApiMsg_st *) start_p;
    sendBuffer_p = start_p;
    l1ApiMsg_p->msgId = PHY_UL_RX_ULSCH_INDICATION;
    l1ApiMsg_p->lenVendorSpecific = 0;
    ulschDataInd_p = (FAPI_rxULSCHIndication_st *) &(l1ApiMsg_p->msgBody[0]);
    ulschDataInd_p->numOfPdu = 1;
    sendBuffer_p += (2*sizeof(UInt16)) + L1_PARTIAL_API_MSG_SIZE;
    ulDataPdu_p = ( FAPI_ulDataPduIndication_st *) 
                        &(ulschDataInd_p->ulDataPduInfo[0]);
    ulDataPdu_p->handle = 1;
    ulDataPdu_p->length = (UInt16) tbSize;
    ulDataPdu_p->dataOffset = sizeof(FAPI_ulDataPduIndication_st);
    ulDataPdu_p->timingAdvance = INVALID_TA_VALUE;
    ulDataPdu_p->ulCqi = 0;
    
    sendBuffer_p += ulDataPdu_p->dataOffset;
 
    if ( &(ulschDataInd_p->pduBuffer[0]) != sendBuffer_p )
    {
        printf("fapiMultiplexAndSendDataToPhysical: address are not same\n");
        exit (0);
    }
#endif
    bsrPresentFlag = ulUEContext_p->bsrPresentFlag;
    crntiCEFlag = ulUEContext_p->crntiCEFlag;
    paddinglength = tbSize;

    if ( crntiCEFlag && paddinglength >=3 )
    {
        controlElementHeaderLen += 1;
        controlElementTbLen += 2;
        numOfSubHdrCount++;
        paddinglength -= 3;
        sduSubHdrLen++;
    }
    else
    {
        if(crntiCEFlag)
        {
            fprintf(stderr, "Shouldn't be the case 5\n");
        }
    }
    /* SPR 3156 START */
    if ((!ttiBundlingRtxFlag) && (ulUEContext_p->phrFlag)
         && (ulUEContext_p->phrTime <= globalTTITickCount_g)
         && (paddinglength >=2 ))
    /* SPR 3156 END */
    {
            controlElementHeaderLen += 1;
            controlElementTbLen += 1;
            numOfSubHdrCount++;
            sduSubHdrLen++;
            paddinglength -= 2;
    }
   
    /* Assumption is that if bsr flag is not true always short BSR will go with data */   
    if ( bsrPresentFlag && paddinglength >= 4 )
    {
        controlElementHeaderLen += 1;
        controlElementTbLen += 3;
        numOfSubHdrCount++;
        sduSubHdrLen++;
        paddinglength -= 4;
    }
    else
    {
        if (bsrPresentFlag)
            bsrPresentFlag = FALSE;
    }

#if 0
    else
    {
        /* sending short BSR */
        controlElementHeaderLen += 1;
        controlElementTbLen += 1;
        numOfSubHdrCount++;
        sduSubHdrLen++;
    }
#endif

    for ( i = 0; i <lcCount;i++)
    {
        /** SPR 3657 Changes Start **/
        rlcReTxPduCount = QUEUE_COUNT_AMD_RETX_PDU_Q(transmissionAndRLCData_p->rlcDataBufferArr[i].reTxRlcPduQ); 
        /** SPR 3657 Changes End **/

        /* Uplink ThroughPut Fix Start */
        lcDataLen = 0;
 
        if ( (lcDataLen = transmissionAndRLCData_p->rlcDataBufferArr[i].statusPduLen) != 0 )
        {
            if(lcDataLen >= 128)
            {
                sduSubHdrLen +=3;
                paddinglength -= (lcDataLen + 3);
                rlcLastPduLen = lcDataLen;
            }
            else
            {
                sduSubHdrLen +=2;
                paddinglength -= (lcDataLen + 2);
                rlcLastPduLen = lcDataLen;
            }
            numOfSubHdrCount++;
            lcPresentFlag=TRUE;
            totRlcPduCount++;
        }   /** SPR 3657 Changes Start **/
        if (rlcReTxPduCount)   //(lcDataLen = transmissionAndRLCData_p->rlcDataBufferArr[i].retxPduLen) != 0 )
        {
            GET_QUEUE_NODE_AMD_RETX_PDU_Q(rlcReTxPduNode_p ,transmissionAndRLCData_p->rlcDataBufferArr[i].reTxRlcPduQ);
            lcDataLen  = rlcReTxPduNode_p->retxPduLen;
            /** SPR 3657 Changes End **/
            if(lcDataLen >= 128)
            {
                sduSubHdrLen +=3;
                paddinglength -= (lcDataLen + 3);
                rlcLastPduLen = lcDataLen;
            }
            else
            {
                sduSubHdrLen +=2;
                paddinglength -= (lcDataLen + 2);
                rlcLastPduLen = lcDataLen;
            }
            numOfSubHdrCount++;
            lcPresentFlag=TRUE;
            totRlcPduCount++;
        }
        if ( (lcDataLen = transmissionAndRLCData_p->rlcDataBufferArr[i].newPduLen) != 0 )
        {
            if(lcDataLen >= 128) 
            {
                if (paddinglength >= lcDataLen +3)
                {
                    sduSubHdrLen +=3;
                    paddinglength -= (lcDataLen + 3);
                    rlcLastPduLen = lcDataLen;
                } 
                else 
                {
                    fprintf(stderr, "Should'nt be the case 1\n");
                }
            } 
            else 
            {
                if (paddinglength >= lcDataLen +2)
                {
                sduSubHdrLen +=2;
                paddinglength -= (lcDataLen + 2);
                rlcLastPduLen = lcDataLen;
                }    
                else
                {
                    fprintf(stderr, "Should'nt be the case 2\n");
                }
            }    
            numOfSubHdrCount++;
            lcPresentFlag=TRUE;
            totRlcPduCount++;
        }
    }
    /*UESIM ADAPTIVE RETX FIX*/
 #if 0   
    if(lcPresentFlag == FALSE)
    {
        /*SPS_TDD_Changes*/
        uldataNode_p =(ulschDataQueueNode *)(transmissionAndRLCData_p->uldataNode_p);

        dynUEContextInfoMap_g[ueIndex]->bsrNet[0] = 0;
        dynUEContextInfoMap_g[ueIndex]->bsrNet[1] = 0;
        dynUEContextInfoMap_g[ueIndex]->bsrNet[2] = 0;
        dynUEContextInfoMap_g[ueIndex]->bsrNet[3] = 0;
        UInt8 cnt = 0;
        for (cnt = 0; cnt < 11; cnt++)
        {
            uplinkContextInfoMap_g[ueIndex]->lcInfo[cnt].queueLoad = 0;
        }
        fapiPrepareLongBSRAndPHR(dynUEContextInfoMap_g[ueIndex],uldataNode_p->rnti,uldataNode_p->sendSfnSf, uldataNode_p->qIndex, uldataNode_p,tbSize, ulUEContext_p->phrFlag); 
        /*SPS_TDD_Changes*/
        //freeMemPool(transmissionAndRLCData_p->uldataNode_p);
        freeMemPool(transmissionAndRLCData_p);
        freeMemPool(start_p);
        return MAC_SUCCESS;
    }
#endif
    /*UESIM ADAPTIVE RETX FIX*/
    if (paddinglength && (paddinglength <=2))
    {
        if ( paddinglength == 1)
        {
            if ( rlcLastPduLen < 128)
            {
                paddinglength = 2;
                numOfSubHdrCount += 2;
                sduSubHdrLen -= 1;
                startPaddingFlag = TRUE;
            }
            else if ( rlcLastPduLen >= 128)
            {
                /*End padding 1 byte padding SH required*/
                numOfSubHdrCount++;
                paddinglength = 1;
                paddingFlag = TRUE;
            }
        }
        else if ( paddinglength == 2 )
        {
            /*End padding 1 byte padding SH required*/
            numOfSubHdrCount++;
            paddinglength = 1;
            paddingFlag = TRUE; 
        }
    }
    else if (!paddinglength)
    {
        if ( rlcLastPduLen )
        {
            if (rlcLastPduLen >= 128)
            {
                sduSubHdrLen -= 2;
                startPaddingFlag = TRUE;
                numOfSubHdrCount += 2;
                paddinglength = 2;
            }
            else if (rlcLastPduLen < 128)
            {
                sduSubHdrLen -= 1;
                startPaddingFlag = TRUE;
                numOfSubHdrCount++;
                paddinglength = 1;
            }
        }
    }
    else
    {
        paddingFlag = TRUE;
        numOfSubHdrCount++;
        paddinglength =1;
    }
    /* Uplink ThroughPut Fix End */

    sendBuffer_p = start_p;
    header_p = sendBuffer_p;

    /*Pointer points to the data*/

   /*Fix for SPR 11915 start*/
   if(ulUEContext_p->ttiBundling && ttiBundlingRtxFlag)
   { 
       data_p   = header_p + ttiBundlingHdrInfo_g[ueIndex].hdrLen;
   }
   else
   {
    data_p   = header_p + sduSubHdrLen + paddinglength;
   }
   /*Fix for SPR 11915 end*/

    /* Prepare SDU Headers */
    /*Fill padding SH first*/
    if (paddinglength)
    {
        if (startPaddingFlag == TRUE)
        {
            if (paddinglength == 1)
            {
                if (!(numOfSubHdrCount - 1))
                {
                    UInt8 sduSubHeader = 0;
                    sduSubHeader   |= (0 << 7);
                    sduSubHeader   |= (0 << 6);
                    sduSubHeader   |= (0  << 5);
                    sduSubHeader   |=  31;
                    
                    *header_p++ = sduSubHeader;
                    
#if 0
                    /*This is the last subheader*/
                    prepareSduSubHeader(&header_p, 0/*R1*/,
                            0/*R2*/, 0/*E*/, 31/*padding lcId*/, 
                            0/*F*/, 0/*bsrSize*/);
#endif                            
                }
                else
                {
                    UInt8 sduSubHeader = 0;
                    sduSubHeader   |= (0 << 7);
                    sduSubHeader   |= (0 << 6);
                    sduSubHeader   |= (1  << 5);
                    sduSubHeader   |=  31;

                    *header_p++ = sduSubHeader;

#if 0                    
                    /*not last subheader*/
                    prepareSduSubHeader(&header_p, 0/*R1*/,
                            0/*R2*/, 1/*E*/, 31/*padding lcId*/, 
                            0/*F*/, 0/*bsrSize*/);
#endif                            
                }
                numOfSubHdrCount--;
            }
            else if ( paddinglength == 2 )
            {
                if (!(numOfSubHdrCount - 2))
                {
                    UInt8 sduSubHeader = 0;
                    sduSubHeader   |= (0 << 7);
                    sduSubHeader   |= (0 << 6);
                    sduSubHeader   |= (1  << 5);
                    sduSubHeader   |=  31;

                    *header_p++ = sduSubHeader;
#if 0                    
                    /*This is the last subheader*/
                    prepareSduSubHeader(&header_p, 0/*R1*/,
                            0/*R2*/, 1/*E*/, 31/*padding lcId*/, 
                            0/*F*/, 0/*bsrSize*/);
#endif                            

                    sduSubHeader   |= (0 << 7);
                    sduSubHeader   |= (0 << 6);
                    sduSubHeader   |= (0  << 5);
                    sduSubHeader   |=  31;

                    *header_p++ = sduSubHeader;
#if 0
                    
                    prepareSduSubHeader(&header_p, 0/*R1*/,
                            0/*R2*/, 0/*E*/, 31/*padding lcId*/, 
                            0/*F*/, 0/*bsrSize*/);
#endif                            
                }
                else
                {
                    UInt8 sduSubHeader = 0;
                    sduSubHeader   |= (0 << 7);
                    sduSubHeader   |= (0 << 6);
                    sduSubHeader   |= (1  << 5);
                    sduSubHeader   |=  31;

                    *header_p++ = sduSubHeader;
                    *header_p++ = sduSubHeader;
#if 0                    
                    /*not last subheader*/
                    prepareSduSubHeader(&header_p, 0/*R1*/,
                            0/*R2*/, 1/*E*/, 31/*padding lcId*/, 
                            0/*F*/, 0/*bsrSize*/);
                    prepareSduSubHeader(&header_p, 0/*R1*/,
                            0/*R2*/, 1/*E*/, 31/*padding lcId*/, 
                            0/*F*/, 0/*bsrSize*/);
#endif                            
                }
                numOfSubHdrCount -= 2;
            }
        }
        else
        {
            UInt8 *ptr = PNULL;
            ptr = header_p + sduSubHdrLen;

            UInt8 sduSubHeader = 0;
            sduSubHeader   |= (0 << 7);
            sduSubHeader   |= (0 << 6);
            sduSubHeader   |= (0  << 5);
            sduSubHeader   |=  31;

            *ptr++ = sduSubHeader;
#if 0            
            prepareSduSubHeader(&(ptr),0/*R1*/,
                    0/*R2*/, 0/*E*/, 31/*padding lcId*/, 
                    0/*F*/, 0/*bsrSize*/);
#endif                    
        }
    }        
    if ( ulUEContext_p->crntiCEFlag)
    {
        UInt8 sduSubHeader = 0;
        sduSubHeader |= (0 << 7);
        sduSubHeader |= (0 << 6);
        sduSubHeader |= (1  << 5);
        sduSubHeader |=  27 & 0x1F;
        *header_p++ = sduSubHeader;
        
        /* crnti ce control element */
        *data_p++ = (UInt8) (ulUEContext_p->crnti >> 8);
        *data_p++ = (UInt8) ulUEContext_p->crnti;
    }

    /*PowerHead room report*/
    /*CLPC_CHG*/
    if ((!ttiBundlingRtxFlag)&& (ulUEContext_p->phrFlag)&&(ulUEContext_p->phrTime <= globalTTITickCount_g))
    {
        UInt8 sduSubHeader = 0;
        sduSubHeader |= (0 << 7);
        sduSubHeader |= (0 << 6);
        sduSubHeader |= (1  << 5);
        sduSubHeader |=  26 & 0x1F;
        *header_p++ = sduSubHeader;
#if 0
        prepareSduSubHeader(&header_p, 0/*R1*/,
                0/*R2*/, 1/*E*/, 26/*power headroom LC ID*/, 
                0/*F*/, 0/*bsrSize*/);
#endif
        /* power headroom control element */
		*data_p = phrProfileInfo_g[cellIndex][ueProfileInfo_g[cellIndex]
			[ulUEContext_p->ueProfileId].phrProfileId].
			phrValues[ulUEContext_p->phrIndex] & 0x3F;
        ulUEContext_p->phrIndex = (ulUEContext_p->phrIndex + 1) % 
			phrProfileInfo_g[cellIndex][ueProfileInfo_g[cellIndex]
			[ulUEContext_p->ueProfileId].phrProfileId].phrCount;
#ifdef PHR_TESTING
        fprintf(stderr,"multiplexAndSendDataToPhysical:Value Encoded=%d\n",
				phrProfileInfo_g[cellIndex][ueProfileInfo_g[cellIndex]
				[ulUEContext_p->ueProfileId].phrProfileId].
				phrValues[ulUEContext_p->phrIndex]);
#endif

        data_p++;
        /*Increment the PhrTime after encoding the value from PhrValue Array*/
		ulUEContext_p->phrTime = globalTTITickCount_g + phrProfileInfo_g
			[cellIndex][ueProfileInfo_g[cellIndex]
            [ulUEContext_p->ueProfileId].phrProfileId].phrTimer;
#ifdef PHR_TESTING
        fprintf(stderr,"multiplexAndSendDataToPhysica##:phrTime=%d,globalTTITickCount_g=%d,phrTimer=%d\n",
                ulUEContext_p->phrTime,
                globalTTITickCount_g,
                phrProfileInfo_g[cellIndex][ueProfileInfo_g[cellIndex]
				[ulUEContext_p->ueProfileId].phrProfileId].phrTimer);
#endif
    }

    /*BSR Report*/
    if ( bsrPresentFlag )
    {
        dynamicUEContext *dynUEContext_p = PNULL;
        dynUEContext_p = dynUEContextInfoMap_g[ueIndex];
        UInt8 sduSubHeader = 0;
        sduSubHeader |= (0 << 7);
        sduSubHeader |= (0 << 6);
        sduSubHeader |= (1  << 5);
        sduSubHeader |=  30 & 0x1F;
        *header_p++ = sduSubHeader;
#if 0        
        prepareSduSubHeader(&header_p, 0/*R1*/,
                0/*R2*/, 1/*E*/, 30/*bsrId*/, 0/*F*/, 3/*bsrSize*/);
#endif                


        //ROHIT : FIX FOR LONG BSR

        UInt32 tempBsrNet[4] = {0};
        UInt8 idx = 0;

        for (idx = 0 ; idx < lcCount; idx++)
        {
            SInt8 lcId = -1;
            SInt32 qLoad = 0;
            lcId = transmissionAndRLCData_p->rlcDataBufferArr[idx].lcID;
            qLoad = transmissionAndRLCData_p->rlcDataBufferArr[idx].remainingDataSize;
            dlLogicalChannel_p = &newUEContext_p->logicalChannel[lcId];

            if (lcId != -1 && (LCGID_INVALID_VALUE
                        != ulUEContext_p->lcInfo[lcId].lcGId))
            {
                tempBsrNet[ulUEContext_p->lcInfo[lcId].lcGId] += qLoad;
            }
        }

        if(2 != dlLogicalChannel_p->rlcMode)
        {    
        /* Form BSR depending upon remaining Qload in each LCG*/
        bsr0 = getBSRIdxfrmBytes( dynUEContext_p->bsrNet[0]);
        bsr1 = getBSRIdxfrmBytes( dynUEContext_p->bsrNet[1]);
        bsr2 = getBSRIdxfrmBytes( dynUEContext_p->bsrNet[2]);
        bsr3 = getBSRIdxfrmBytes( dynUEContext_p->bsrNet[3]);
        //prepareBSR(&bsrBuffer_p,1,bsr0,bsr1,bsr2,bsr3,0);
#if 0        
        prepareBSR(&bsrBuffer_p,bsrPresentFlag,bsr0,bsr1,bsr2,bsr3,0);
#endif        
        }
        else //For AM Mode, temp fix 
        {
            /* Form BSR depending upon remaining Qload in each LCG*/
            bsr0 = getBSRIdxfrmBytes( tempBsrNet[0]);
            bsr1 = getBSRIdxfrmBytes( tempBsrNet[1]);
            bsr2 = getBSRIdxfrmBytes( tempBsrNet[2]);
            bsr3 = getBSRIdxfrmBytes( tempBsrNet[3]);
        }

        UInt8 tempBSR = 0;
        tempBSR |=  bsr0 << 2;
        tempBSR |= ((bsr1 >> 4) & 0x03);
        *data_p++ = tempBSR;

        tempBSR = 0;
        tempBSR |= bsr1 << 4;
        tempBSR |= ((bsr2 >> 2) & 0x0F);
        *data_p++ = tempBSR;

        tempBSR = 0;
        tempBSR |= bsr2 << 6;
        tempBSR |= (bsr3 & 0x3F);
        *data_p++ = tempBSR;
#ifdef UT_TESTING_PRINT
        fprintf(stderr,"BSR sent with data [ bsr0 = %d]\n ", bsr0);
#endif
#if 0
        *data_p = bsrBuffer[0];
        data_p++;
        *data_p = bsrBuffer[1];
        data_p++;
        *data_p = bsrBuffer[2];
        data_p++;
#endif 

    }

    UInt8 extFlag = TRUE;
    UInt32 index = 0;
    /* Uplink ThroughPut Fix Start */
    UInt8 rlcPduCount = 0;

    while (index < lcCount)
    {
        lcDataLen = 0;
        /** SPR 3657 Changes Start **/
        rlcReTxPduCount = QUEUE_COUNT_AMD_RETX_PDU_Q(
                              transmissionAndRLCData_p->rlcDataBufferArr[index].reTxRlcPduQ);
        rlcReTxPduNode_p = PNULL;
                /** SPR 3657 Changes End **/

        /*SPR 1078 changes start */
        rlcHarqFailureInfo_p = &rlcHarqFailureInfo[0];
        for(i = 0;i < MAX_NUMBER_OF_LOGICAL_CHANNEL;i++)
        {
            if (transmissionAndRLCData_p->rlcDataBufferArr[index].lcID != rlcHarqFailureInfo_p->lcId)
            {
                rlcHarqFailureInfo_p++;
            }
            else
            {
                break;
            }
            if(MAX_NUMBER_OF_LOGICAL_CHANNEL-1 == i)
            {
                lteWarning ("Unable to find the LC ID in array.\n");
                fprintf(stderr, "Unable to find the LC ID in array.\n");
                return MAC_FAILURE;
            }

        }
        /* Encode the SubHeader */

        /* For StatusPdu Encode Header */
        if((transmissionAndRLCData_p->rlcDataBufferArr[index].statusPdu_p) && ((rlcPduCount+1) != totRlcPduCount))
        {
            rlcPduCount++;
            if((lcDataLen = transmissionAndRLCData_p->rlcDataBufferArr[index].statusPduLen) >= 128)
            {
                /* Encode 3 bytes Header */
                UInt8 sduSubHeader = 0;
                UInt8 formatLen1   = 0;
                UInt8 formatLen2   = 0;
                UInt8 lcId         = transmissionAndRLCData_p->rlcDataBufferArr[index].lcID & 0x1F;
                sduSubHeader |= (0 << 7);
                sduSubHeader |= (0 << 6);
                sduSubHeader |= (extFlag  << 5);
                sduSubHeader |=  lcId;

                *header_p++ = sduSubHeader;

                if( lcId != 0 && lcId != 27 && lcId != 26 )
                {
                    formatLen1    |= 1 << 7;
                    formatLen1    |= (lcDataLen >>8 ) & 0x7F;
                    formatLen2    =  lcDataLen & 0xFF;
                    *header_p++ = formatLen1;
                    *header_p++ = formatLen2;
                }
            }
            else
            {
                /* Encode 2 bytes Header */
                UInt8 sduSubHeader = 0;
                UInt8 formatLen1   = 0;
                UInt8 lcId = transmissionAndRLCData_p->rlcDataBufferArr[index].lcID;
                sduSubHeader |= (0 << 7);
                sduSubHeader |= (0 << 6);
                sduSubHeader |= (extFlag  << 5);
                sduSubHeader |=  lcId & 0x1F;

                *header_p++ = sduSubHeader;

                if( lcId != 0 && lcId != 27 && lcId != 26 )
                {
                    formatLen1 |= 0 << 7;
                    formatLen1 |= lcDataLen & 0x7F;
                    *header_p++ = formatLen1;
                }
            }
        }
        /* For ReTx Encode Header */
        if((rlcReTxPduCount) && ((rlcPduCount+1) != totRlcPduCount))
        {
            rlcPduCount++;
            rlcReTxPduNode_p = PNULL;
            GET_QUEUE_NODE_AMD_RETX_PDU_Q(rlcReTxPduNode_p ,
                                          transmissionAndRLCData_p->rlcDataBufferArr[index].reTxRlcPduQ);
            lcDataLen = rlcReTxPduNode_p->retxPduLen;
            if(lcDataLen >= 128)
            {
                /* Encode 3 bytes Header */
                UInt8 sduSubHeader = 0;
                UInt8 formatLen1   = 0;
                UInt8 formatLen2   = 0;
                UInt8 lcId         = transmissionAndRLCData_p->rlcDataBufferArr[index].lcID & 0x1F;
                sduSubHeader |= (0 << 7);
                sduSubHeader |= (0 << 6);
                sduSubHeader |= (extFlag  << 5);
                sduSubHeader |=  lcId;

                *header_p++ = sduSubHeader;
                if( lcId != 0 && lcId != 27 && lcId != 26 )
                {
                    formatLen1    |= 1 << 7;
                    formatLen1    |= (lcDataLen >>8 ) & 0x7F;
                    formatLen2    =  lcDataLen & 0xFF;
                    *header_p++ = formatLen1;
                    *header_p++ = formatLen2;

                }
            }
            else
            {
                /* Encode 2 bytes Header */
                UInt8 sduSubHeader = 0;
                UInt8 formatLen1   = 0;
                UInt8 lcId = transmissionAndRLCData_p->rlcDataBufferArr[index].lcID;
                sduSubHeader |= (0 << 7);
                sduSubHeader |= (0 << 6);
                sduSubHeader |= (extFlag  << 5);
                sduSubHeader |=  lcId & 0x1F;

                *header_p++ = sduSubHeader;

                if( lcId != 0 && lcId != 27 && lcId != 26 )
                {
                    formatLen1 |= 0 << 7;
                    formatLen1 |= lcDataLen & 0x7F;
                    *header_p++ = formatLen1;
                }
            }
        }

        /* For NewPdu Encode Header */
        if((transmissionAndRLCData_p->rlcDataBufferArr[index].newRlcPdu_p) && ((rlcPduCount+1) != totRlcPduCount))
        {
            rlcPduCount++;
            if((lcDataLen = transmissionAndRLCData_p->rlcDataBufferArr[index].newPduLen) >= 128)
            {
                /* Encode 3 bytes Header */
                UInt8 sduSubHeader = 0;
                UInt8 formatLen1   = 0;
                UInt8 formatLen2   = 0;
                UInt8 lcId         = transmissionAndRLCData_p->rlcDataBufferArr[index].lcID & 0x1F;
                sduSubHeader |= (0 << 7);
                sduSubHeader |= (0 << 6);
                sduSubHeader |= (extFlag  << 5);
                sduSubHeader |=  lcId;

                *header_p++ = sduSubHeader;

                if( lcId != 0 && lcId != 27 && lcId != 26 )
                {
                    formatLen1    |= 1 << 7;
                    formatLen1    |= (lcDataLen >>8 ) & 0x7F;
                    formatLen2    =  lcDataLen & 0xFF;
                    *header_p++ = formatLen1;
                    *header_p++ = formatLen2;
                }
            }
            else
            {
                /* Encode 2 bytes Header */
                UInt8 sduSubHeader = 0;
                UInt8 formatLen1   = 0;
                UInt8 lcId = transmissionAndRLCData_p->rlcDataBufferArr[index].lcID;
                sduSubHeader |= (0 << 7);
                sduSubHeader |= (0 << 6);
                sduSubHeader |= (extFlag  << 5);
                sduSubHeader |=  lcId & 0x1F;

                *header_p++ = sduSubHeader;

                if( lcId != 0 && lcId != 27 && lcId != 26 )
                {
                    formatLen1 |= 0 << 7;
                    formatLen1 |= lcDataLen & 0x7F;
                    *header_p++ = formatLen1;
                }
            }
        }
        index++;
    }

    if(paddingFlag)
    {
        /* Encode Last Header */
        if (rlcLastPduLen >= 128)
        {
            //fprintf(stderr, "4: newprint \n");
            /* Encode 3 bytes Header */
            UInt8 sduSubHeader = 0;
            UInt8 formatLen1   = 0;
            UInt8 formatLen2   = 0;
            UInt8 lcId         = transmissionAndRLCData_p->rlcDataBufferArr[index-1].lcID & 0x1F;
            sduSubHeader |= (0 << 7);
            sduSubHeader |= (0 << 6);
            sduSubHeader |= (extFlag  << 5);
            sduSubHeader |=  lcId;
            *header_p++ = sduSubHeader;

            if( lcId != 0 && lcId != 27 && lcId != 26 )
            {
                formatLen1    |= 1 << 7;
                formatLen1    |= (rlcLastPduLen >>8 ) & 0x7F;
                formatLen2    =  rlcLastPduLen & 0xFF;
                *header_p++ = formatLen1;
                *header_p++ = formatLen2;
            }
        }
        else
        {
            /* Encode 2 bytes Header */
            UInt8 sduSubHeader = 0;
            UInt8 formatLen1   = 0;
            UInt8 lcId = transmissionAndRLCData_p->rlcDataBufferArr[index-1].lcID;
            sduSubHeader |= (0 << 7);
            sduSubHeader |= (0 << 6);
            sduSubHeader |= (extFlag  << 5);
            sduSubHeader |=  lcId & 0x1F;

            *header_p++ = sduSubHeader;

            if( lcId != 0 && lcId != 27 && lcId != 26 )
            {
                formatLen1 |= 0 << 7;
                formatLen1 |= rlcLastPduLen & 0x7F;
                *header_p++ = formatLen1;
            }
        }
    }
    else
    {
        //fprintf(stderr, "6: newprint \n");
        extFlag = FALSE;
        *header_p = 0;
        *header_p |= (extFlag << 5);
        *header_p++ |= transmissionAndRLCData_p->rlcDataBufferArr[index-1].lcID & 0x1F;
    }

    index = 0;
    while (index < lcCount)
    {
        /*SPR 1078 changes start */
        rlcHarqFailureInfo_p = &rlcHarqFailureInfo[0];
        for(i = 0;i < MAX_NUMBER_OF_LOGICAL_CHANNEL;i++)
        {
            if (transmissionAndRLCData_p->rlcDataBufferArr[index].lcID != rlcHarqFailureInfo_p->lcId)
            {
                rlcHarqFailureInfo_p++;
            }
            else
            {
                break;
            }
            if(MAX_NUMBER_OF_LOGICAL_CHANNEL-1 == i)
            {
                lteWarning ("Unable to find the LC ID in array.\n");
                fprintf(stderr, "Unable to find the LC ID in array.\n");
                return MAC_FAILURE;
            }

        }
        rlcReTxPduCount = QUEUE_COUNT_AMD_RETX_PDU_Q(transmissionAndRLCData_p->rlcDataBufferArr[index].reTxRlcPduQ);

        if (transmissionAndRLCData_p->rlcDataBufferArr[index].statusPdu_p)
        {
            /* Encode Status Pdu Data */
            msgExtract(transmissionAndRLCData_p->rlcDataBufferArr[index].statusPdu_p,0,
                       data_p,
                       transmissionAndRLCData_p->rlcDataBufferArr[index].statusPduLen);
            data_p += transmissionAndRLCData_p->rlcDataBufferArr[index].statusPduLen;
#ifdef FDD_CONFIG            
            if (!ulUEContext_p->ttiBundling)
#endif                
            {
                msgFree(transmissionAndRLCData_p->rlcDataBufferArr[index].statusPdu_p);
                transmissionAndRLCData_p->rlcDataBufferArr[index].statusPdu_p = PNULL;
            }
        }
        if (rlcReTxPduCount)
        {
            /* Encode Retx Data */
            rlcReTxPduNode_p = PNULL;
            DEQUEUE_AMD_RETX_PDU_Q(rlcReTxPduNode_p,
                                   transmissionAndRLCData_p->rlcDataBufferArr[index].reTxRlcPduQ);
#ifdef FDD_CONFIG            
            if (ulUEContext_p->ttiBundling)
            {
                ENQUEUE_AMD_RETX_PDU_Q(transmissionAndRLCData_p->rlcDataBufferArr[index].reTxRlcPduQ,
                                    rlcReTxPduNode_p);
            }
#endif            
            msgExtract(rlcReTxPduNode_p->retxRlcPdu_p,0,data_p,rlcReTxPduNode_p->retxPduLen);
            data_p += rlcReTxPduNode_p->retxPduLen;
            
#ifdef FDD_CONFIG            
            if (!ulUEContext_p->ttiBundling)
#endif          
            {
                msgFree( rlcReTxPduNode_p->retxRlcPdu_p);
                rlcReTxPduNode_p->retxRlcPdu_p = PNULL;
                freeMemPool(rlcReTxPduNode_p);
                rlcReTxPduNode_p = PNULL;
            }
        }
        if (transmissionAndRLCData_p->rlcDataBufferArr[index].newRlcPdu_p)
        {
            /* Encode NewPdu Data */
            msgExtract(transmissionAndRLCData_p->rlcDataBufferArr[index].newRlcPdu_p,0,
                    data_p,
                    transmissionAndRLCData_p->rlcDataBufferArr[index].newPduLen);
            data_p  += transmissionAndRLCData_p->rlcDataBufferArr[index].newPduLen;
        }

        lcID = transmissionAndRLCData_p->rlcDataBufferArr[index].lcID;
        dlLogicalChannel_p = &newUEContext_p->logicalChannel[lcID];
        if( (2 > dlLogicalChannel_p->rlcMode) || ( (2 == dlLogicalChannel_p->rlcMode)
            /*SPR 1078 changes start */
            && (!(rlcHarqFailureInfo_p->harqFailureReporting)) ) )
            /*SPR 1078 changes end */
        {
            if(transmissionAndRLCData_p->rlcDataBufferArr[index].newRlcPdu_p)
            {
                /*Free the memory*/
#ifdef FDD_CONFIG            
                if (!ulUEContext_p->ttiBundling)
#endif                    
                {
                    msgFree((void*)transmissionAndRLCData_p->rlcDataBufferArr[index].newRlcPdu_p);
                }
            }
        }
#ifdef FDD_CONFIG            
        if (!ulUEContext_p->ttiBundling)
#endif            
        {
            transmissionAndRLCData_p->rlcDataBufferArr[index].newRlcPdu_p = PNULL;
        }
        index++;
    }
    /* Uplink ThroughPut Fix End */

#if 0
    for(i=0; i<lcCount; i++) {
#if 0
        msgExtract(transmissionAndRLCData_p->rlcDataBufferArr[i].rlcBuffer_p, 0,
                sendBuffer_p,transmissionAndRLCData_p->rlcDataBufferArr[i].bufferLength);
#endif
        memCpy(sendBuffer_p,transmissionAndRLCData_p->rlcDataBufferArr[i].rlcBuffer_p,
                transmissionAndRLCData_p->rlcDataBufferArr[i].bufferLength);
        sendBuffer_p += transmissionAndRLCData_p->rlcDataBufferArr[i].bufferLength;
#if 0
        msgFree(transmissionAndRLCData_p->rlcDataBufferArr[i].rlcBuffer_p);
#endif
    }
    /* TODO Length of UL message shd be calculated, the present one is incorrect */
    ulDataLen = tempTBLen + tempSubHdrLen;
    *lenIdx_p =(ulDataLen & 0x00FF);
    lenIdx_p++;
    *lenIdx_p =(ulDataLen & 0x0000FF00) >> 8;
    lenIdx_p++;
    *lenIdx_p =(ulDataLen & 0x00FF0000) >> 16;
    lenIdx_p++;
    *lenIdx_p =(ulDataLen & 0xFF000000) >> 24;
    lenIdx_p++;
#endif

#if 0
    dynUEContextInfoMap_g[ueIndex]->ulDataPdu->PDULength = tbSize;
    dynUEContextInfoMap_g[ueIndex]->ulDataPdu->Pdudata_p = start_p;
#endif

#if 0

    uldataNode_p =(ulschDataQueueNode *)(transmissionAndRLCData_p->uldataNode_p);
    uldataNode_p->pdudata_p = start_p;
    l1ApiMsg_p->msgLen = (data_p - start_p) - l1ApiMsgSize;
    l1ApiMsgSize = (data_p - start_p);
    uldataNode_p->pduLen = l1ApiMsgSize;
    ulDataPdu_p->rnti = uldataNode_p->rnti;
    ulschDataInd_p->sfnsf = uldataNode_p->sendSfnSf;
    //fprintf(stderr,"------- l1ApiMsg_p->msgLen = %d--------\n",l1ApiMsg_p->msgLen);
    //fprintf(stderr,"--------- l1ApiMsgSize = %d------\n",uldataNode_p->pduLen);
#endif
    
    /*Fix for SPR 11915 start*/
    /* save the header in case of 1st transmission of TTIB  for retransmitted 
       packets of TTI bundle in structure ttiBundlingHdrInfo_gi, the header is
       saved in pointer .pduData_p, Use this structure to get the header*/  
    if(ulUEContext_p->ttiBundling)
    {
        if(ttiBundlingRtxFlag)
        {
            *(transmissionAndRLCData_p->dataPtr_p)  = start_p;
            memcpy( start_p, ttiBundlingHdrInfo_g[ueIndex].pduData_p,ttiBundlingHdrInfo_g[ueIndex].hdrLen);
            *(transmissionAndRLCData_p->dataLen_p) = ttiBundlingHdrInfo_g[ueIndex].dataLen;
        }
    else
    {
        if(!(ttiBundlingHdrInfo_g[ueIndex].pduData_p))
        {
            ttiBundlingHdrInfo_g[ueIndex].pduData_p = (UInt8 *) getMemFromPool(100, PNULL);
        }
        memcpy(ttiBundlingHdrInfo_g[ueIndex].pduData_p,start_p,sduSubHdrLen+paddinglength);
        ttiBundlingHdrInfo_g[ueIndex].dataLen = tbSize;
        ttiBundlingHdrInfo_g[ueIndex].hdrLen = (sduSubHdrLen+paddinglength);
        if ((ulUEContext_p->phrFlag)&&(ulUEContext_p->phrTime <= globalTTITickCount_g))
        {
            UInt8 * tempPtr = (ttiBundlingHdrInfo_g[ueIndex].pduData_p) ;
            tempPtr += (sduSubHdrLen+paddinglength);
            *tempPtr = (phrProfileInfo_g[cellIndex][ueProfileInfo_g[cellIndex][ulUEContext_p->ueProfileId].phrProfileId].phrValues[ulUEContext_p->phrIndex] & 0x3F); 
            ttiBundlingHdrInfo_g[ueIndex].hdrLen += 1;
       }

       *(transmissionAndRLCData_p->dataPtr_p)  = start_p;
       *(transmissionAndRLCData_p->dataLen_p) = tbSize;
    }
    }
    else
    {
    *(transmissionAndRLCData_p->dataPtr_p)  = start_p;
    *(transmissionAndRLCData_p->dataLen_p) = tbSize;
    }
    /*Fix for SPR 11915 end*/
    #if 0
    fprintf(stderr,"Data Recvd from RLC TBSIZE [%d] start_p[%x]\n",
             tbSize, start_p);
    #endif
    uldataNode_p =(ulschDataQueueNode *)(transmissionAndRLCData_p->uldataNode_p);

    pduPointer=start_p;

    subFrame = uldataNode_p->SF;
    qIndex   = uldataNode_p->qIndex;

// _ULHARQ_RETX_  start  
#ifdef FAPI_UL_HARQ_ENABLE
    
    if(uldataNode_p->SFN != SFN_g)
    {
        if ((!((uldataNode_p->SFN==0)&& (SFN_g==(MAX_SFN -1)))) && (!((uldataNode_p->SFN==(MAX_SFN -1) )&& (SFN_g==0))))
        {
            currentSFNSF=SFN_g * 10 + SF_g;
            sendSFNSF= uldataNode_p->SFN * 10 + qIndex;
        }
        else if((uldataNode_p->SFN==0)&& (SFN_g==(MAX_SFN -1)))
        {
            currentSFNSF=0;
            sendSFNSF=(MAX_SFN - 1);
        }
        else if((uldataNode_p->SFN==(MAX_SFN -1) )&& (SFN_g==0))
        {
            currentSFNSF=(MAX_SFN - 1);
            sendSFNSF=0;
        }
    }
    else
    {
        currentSFNSF=SF_g;
        sendSFNSF=qIndex;
    }
    //fix for sync.
    if(ulUEContext_p->ulDataPduInfo[qIndex].Pdudata_p)
    {
        FLOW_DEBUG(stderr,"In MUL DROP printLogCnt = %lld cSFN %d cSF %d sqI %d sSF %d sSFN %d cSFNSF %d sSFNSF %d pduDataPointer %016llX\n",++printLogCnt,SFN_g,SF_g,qIndex,subFrame,uldataNode_p->SFN,currentSFNSF,sendSFNSF,pduPointer);
    }
    else
    {
        FLOW_DEBUG(stderr,"In MUL TX  printLogCnt = %lld cSFN %d cSF %d sqI %d sSF %d sSFN %d cSFNSF %d sSFNSF %d pduDataPointer %016llX\n",++printLogCnt,SFN_g,SF_g,qIndex,subFrame,uldataNode_p->SFN,currentSFNSF,sendSFNSF,pduPointer);
    }     
    
    if (currentSFNSF<sendSFNSF)
    {   
#ifdef UE_SIM_ENABLE_PRINTF_LOGS
        fprintf(stderr,"\n RLC ->MULTIPLEX FUNC data pushed in ul data queue at index = %d of len = %d nodeSF = %d NODE SFN = %d\n",uldataNode_p->qIndex,uldataNode_p->pduLen, uldataNode_p->SF, uldataNode_p->SFN);
#endif
        /* +CA_NA */
        GET_RNTI_TO_CELL_INDEX(uldataNode_p->rnti, cellIndex);
        pushNode(&ulDataQueue[cellIndex][qIndex], &(uldataNode_p->ulDataAnchor));
        /* -CA_NA */
    }
    else
    {
        FLOW_DEBUG(stderr,"In [%s] printLogCnt = %lld INITHARQRETRY\n",__func__,++printLogCnt);
    }
    
  
    ulUEContext_p->ulDataPduInfo[qIndex].Pdudata_p = start_p;

    ulUEContext_p->ulDataPduInfo[qIndex].PDULength = tbSize;
   
#else

#ifdef UE_SIM_ENABLE_PRINTF_LOGS
        fprintf(stderr,"data pushed in ul data queue at index = %d of len = %d nodeSF = %d NODE SFN = %d\n",uldataNode_p->qIndex,uldataNode_p->pduLen, uldataNode_p->SF, uldataNode_p->SFN);
#endif
        //fprintf("current SF = %d surrent SFN = %d\n", SF_g,SFN_g);
        /* +CA_NA */
        GET_RNTI_TO_CELL_INDEX(uldataNode_p->rnti, cellIndex);
        
        pushNode(&ulDataQueue[cellIndex][qIndex], &(uldataNode_p->ulDataAnchor));
        /* -CA_NA */

#endif

#ifdef FDD_CONFIG
    /** For TTIB_Code */
    if (!ulUEContext_p->ttiBundling)
#endif
    {
        freeMemPool(transmissionAndRLCData_p);
        transmissionAndRLCData_p = PNULL;
    }

    /* set the power headroom report flag */
    /*CLPC_CHG*/
#if 0    
    if (ulUEContext_p->phrFlag != phrFlag_g )
        ulUEContext_p->phrFlag = phrFlag_g;
#endif
    if( ulUEContext_p->crntiCEFlag)
    {
        ulUEContext_p->crntiCEFlag=0x0;
    }

    return MAC_SUCCESS;
}




/****************************************************************************
 * Function Name  : macUpdateBufferOccupancyReq
 * Inputs         : ueId -  UE Identifier
 *                  lcId -  Logical channel id
 *                  value - Absolute Value for the QueueLoad
 * Outputs        : None
 * Returns        : void
 * Variables      : 
 * Description    : It calls the updateLCQueueLoadForUE() and 
 *                  updateUEQueueLoad () fucntions for updating the Queue Load
 *                  values for the Logical Channel and UE as requested by RLC. 
 ****************************************************************************/
#ifdef UESIM_PDCP_THREAD
extern LTE_SEM boSem_g;
#endif
UInt32 macUpdateBufferOccupancyReq(UInt16 ueIndex, UInt16 lcCount, 
        /* SPR 1273 Start */
                LCQueueLoad * lcQueueLoadArr_p, BOType  boFalg, InternalCellIndex cellIndex)
        /* SPR 1273 Start */
{
   
    if (stoppedProcessing_g[cellIndex])
    {
        lteWarning("Stopped Processing\n");
        return INTERFACE_FAILURE;
    }   

    if((lcCount == 0) || (lcQueueLoadArr_p == PNULL)) {
        return INTERFACE_FAILURE;
    }

    if(uplinkContextInfoMap_g[ueIndex] && dynUEContextInfoMap_g[ueIndex])
    {
#ifdef UESIM_PDCP_THREAD
        semWait(&boSem_g);
#endif
        updateBufferOccupancy(ueIndex, lcCount, lcQueueLoadArr_p, cellIndex);
#ifdef UESIM_PDCP_THREAD
        semPost(&boSem_g);
#endif
        return INTERFACE_SUCCESS;
    }

    return INTERFACE_FAILURE;

}

/****************************************************************************
 * Function Name  : macUeDataReq
 * Inputs         : ueIndex - UE Identifier
 *                  lcCount - valid logical channel count.
 *                  rlcBuffer_p - 
 *                  requestFailure - whether get rlc data request was success
 *                  or failure.
 * Outputs        :
 * Returns        : MAC_SUCCESS or MAC_FAILURE.
 * Variables      :
 * Description    : Called by RLC in response to request by MAC layer.
 *                  It creates a node and fills the info/data given by RLC
 *                  and puts the node into encoder queue.
 *                 
 ****************************************************************************/

UInt32 macUeDataReq(UInt16 ueIndex,UInt8 lcCount,RLCDataBuffer* rlcBuffer_p
                                                       ,UInt8
                                                       successOrFailure,RLCHarqFailureInfo*
                                                       rlcHarqFailureInfo,
                                                       UInt8 cellIndex)
{
    TransmissionAndRLCData* transmissionAndRLCData_p = PNULL;
    /*Fix for SPR 11915, the below flag would be 0 for new transmissions*/
    UInt8 ttiBundlingRtxFlag = 0;

    transmissionAndRLCData_p = (TransmissionAndRLCData *)rlcBuffer_p;

    if (transmissionAndRLCData_p == PNULL)
    {
        lteWarning("[%s] received rlcBuffer_p as PNULL\n",__func__);
        return MAC_FAILURE;
    }
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"[%s] received RLC data for ueIndex %d reqstd tick %d currtick %d\n"
                    ,__func__, ueIndex,transmissionAndRLCData_p->scheduledTick,
                    globalTTITickCount_g);

    if(uplinkContextInfoMap_g[ueIndex] && dynUEContextInfoMap_g[ueIndex])
    {

        return multiplexAndSendDataToPhysical(ueIndex, lcCount, 
				transmissionAndRLCData_p, rlcHarqFailureInfo, cellIndex, ttiBundlingRtxFlag);
        //return MAC_SUCCESS;
    }
    
    return MAC_FAILURE;

}
/*SPR 11244 Changes Start */
UInt32 macIsCellRunning(InternalCellIndex cellIndex)
{
    return 1;
}
/*SPR 11244 Changes End */


