
/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2RRCInterface.c,v 1.1.1.1.16.1 2010/09/21 15:53:35 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the Interface API between the 
 *                     MAC and RRC Layer in a LTE system. It contains
 *                     different functions that perform :
 *
 *                     - Initialisation of the MAC Layer with the different
 *                       threads and data structures like queues, hash, 
 *                       memory pool, semaphores.
 *                     - Creating, reconfiguring and deleting MAC UE entities 
 *                       and their logical channels.
 *                     - Configuring the various scheduler related parameters
 *                     - Configuring the various global system wide parameter
 *                       like total UE Count, the HARQ profile, number of  
 *                       worker threads concurrently running in the system, 
 *                       the available resource blocks.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLayer2RRCInterface.c,v $
 * Revision 1.1.1.1.16.1  2010/09/21 15:53:35  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.8  2009/09/02 13:34:21  gur20439
 * printf put in UT_TESTING flag.
 *
 * Revision 1.7  2009/07/28 15:19:05  gur20439
 * delete ue from rrc simulaotr support.
 *
 * Revision 1.6  2009/07/27 08:51:06  gur20439
 * periodic cqi enhancement.
 *
 * Revision 1.5  2009/07/24 14:41:12  gur20439
 * support for aperiodic cqi.
 *
 * Revision 1.4  2009/07/15 14:25:27  gur20439
 * bug fix for rrc interface code.
 *
 * Revision 1.3  2009/07/14 14:48:54  gur20439
 * rrc interface code change.
 *
 * Revision 1.2  2009/06/08 13:58:00  gur20439
 * ut bug fix.
 *
 * Revision 1.1  2009/03/30 10:20:41  gur19836
 * Initial Version
 *
 * Revision 1.58  2009/03/13 08:38:21  gur19836
 * Warnings removed
 *
 * Revision 1.57  2009/03/13 08:00:08  gur19836
 * Modified for OAM and RLC Interface Changes
 *
 * Revision 1.56  2009/03/02 10:59:57  gur18569
 * removed funcNamp_p and replaced it with __func__
 *
 * Revision 1.55  2009/02/20 14:27:25  gur18569
 * optimization in initRBMapofTick
 *
 * Revision 1.54  2009/01/27 17:08:24  gur18569
 * adjusted logs
 *
 * Revision 1.53  2009/01/27 09:42:55  gur18569
 * deducting ueQueueLoad with qload of lc deleted
 *
 * Revision 1.52  2009/01/27 06:41:31  gur18569
 * freeing hRttTImerStarted_p of harq process in deletion case
 *
 * Revision 1.51  2009/01/21 09:01:46  gur19836
 * Changed format from dos to unix
 *
 * Revision 1.50  2009/01/20 07:47:00  gur19836
 * Changes done for interface with RRC Layer
 *
 * Revision 1.37  2009/01/08 18:56:19  gur18569
 * Review comments incorporated
 *
 * Revision 1.36  2009/01/08 10:29:48  gur20052
 * Added Uplink Logical Channel Group Priority Weight
 *
 * Revision 1.35  2009/01/06 04:34:56  gur18569
 * removed DL_CQI_WT
 *
 * Revision 1.34  2009/01/05 09:07:22  gur20052
 * config parameters for uplink added
 *
 * Revision 1.33  2009/01/04 14:33:29  gur18569
 *  Uncommenting stats manager thread creation
 *
 * Revision 1.32  2009/01/04 08:01:14  gur11912
 * Phase 2 UT Defects Fixed
 *
 * Revision 1.31  2008/12/30 14:16:04  gur11912
 * Phase 2 UT Defect Fixed
 *
 * Revision 1.30  2008/12/29 10:10:06  gur19836
 * Fn Name changed to getMemPoolStats
 *
 * Revision 1.29  2008/12/26 05:48:46  gur11912
 * UE Max RB check applied for RA Type0 and 1
 *
 * Revision 1.28  2008/12/23 05:07:44  gur11912
 * comment incorporatd for phase2
 *
 * Revision 1.27  2008/12/22 13:42:42  gur18569
 * review comments incorporated for phase 2
 *
 * Revision 1.26  2008/12/19 09:15:34  gur18569
 * added  rbsULConfiguredSem_g and rbsDLConfiguredSem_g
 *
 * Revision 1.25  2008/12/18 15:24:13  gur18569
 * review comments incorporated
 *
 * Revision 1.24  2008/12/11 14:23:56  gur11912
 * Downlink Phase 2 changes
 *
 * Revision 1.23  2008/12/10 07:54:23  gur20052
 * Initial version for Phase 2
 *
 * Revision 1.22  2008/12/09 11:49:55  gur19836
 * Updated during linking
 *
 * Revision 1.21  2008/12/05 12:38:08  gur18569
 * multicore changes
 *
 * Revision 1.20  2008/12/02 12:24:30  gur11912
 * Incorporated multiple worker threads changes
 *
 * Revision 1.19  2008/11/13 11:32:26  gur11912
 * Updated for performance
 *
 * Revision 1.18  2008/10/22 12:36:01  gur11912
 * fixed for performance changes
 *
 * Revision 1.17  2008/10/20 07:08:54  gur18569
 * added initializers for statsManager
 *
 * Revision 1.16  2008/10/17 09:24:00  gur19836
 * Enhancement: cleanupMACLayer functionality added
 *
 * Revision 1.15  2008/10/16 05:25:01  gur18569
 * performance code changes
 *
 * Revision 1.14  2008/10/14 06:41:20  gur11912
 * ARQ structure initialized and LC Priority Validation applied
 *
 * Revision 1.13  2008/10/07 05:02:30  gur19836
 * UT Defects fixed
 *
 * Revision 1.12  2008/10/01 10:53:12  gur19836
 * lastScheduledTick init with globalTTITickCount_g
 *
 * Revision 1.11  2008/09/30 11:38:06  gur19836
 * Setting LogLevel at the end of initMACLayer()
 *
 * Revision 1.10  2008/09/30 09:32:56  gur19836
 * UT Defect fix
 *
 * Revision 1.9  2008/09/26 09:08:55  gur19836
 * UT Defect Fix & Code Indentation
 *
 * Revision 1.8  2008/09/26 07:17:53  gur11912
 * UT Defect Fixed
 *
 * Revision 1.7  2008/09/19 14:25:24  gur11912
 * updated for integration
 *
 * Revision 1.6  2008/09/19 12:21:32  gur11912
 * MAX MIN LIMIT Used
 *
 * Revision 1.5  2008/09/19 07:13:36  gur11912
 * review comments incorporated
 *
 * Revision 1.4  2008/09/10 04:55:36  gur11974
 * Removed the extra comments
 *
 * Revision 1.3  2008/09/09 16:08:16  gur11974
 * Updated for Integration
 *
 * Revision 1.2  2008/09/09 06:03:54  gur11974
 * Updated For Integration changes
 *
 * Revision 1.1  2008/09/09 05:12:53  gur11974
 * Initial Version
 * Rajat Budhiraja , Ravindra Patil - 0.1
 *
 ****************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2RRCInterface.h"
#include "lteLayer2TBSize.h"
#include "lteLayer2UplinkController.h"
#include "lteLayer2ParseUtil.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/

extern UInt16 lowerTempCRNTIBound_g;
extern UInt16 upperTempCRNTIBound_g;
//extern UInt16 *crntiToIdxTbl_g;
extern UInt16  startRARNTI_g;
/*LTE_REG_FIX*/
extern UInt16  endRARNTI_g[MAX_NUM_CELLS];
/*LTE_REG_FIX*/
//extern APeriodicCQI *apcqiInfo_p; 
extern UInt16 crntiRange_g;
extern ueContextForUplink* uplinkContextInfoMap_g[MAX_UE_SUPPORTED];
extern RNTIInfo *rntiToIdxTbl_g[MAX_NUM_CELLS];
extern CellConfig cellConfigUeSim_g[MAX_NUM_CELLS];
DLUEContextInfo dlUECtxInfoArr_g[MAX_UE_SUPPORTED];
extern RNTIInfo *getRntiInfofromRNTIMap(UInt16 rnti, UInt8 cellIndex );
UInt8 rlfDebugFlag = 0;
/*CLPC_CHG*/
extern TpcInfo tpcRntiInfoMap[TPC_RNTI_COUNT];
extern UInt8 perAckTb1_g[MAX_NUM_CELLS];
extern UInt8 perNackTb1_g[MAX_NUM_CELLS];
extern UInt8 perUnsureTb1_g[MAX_NUM_CELLS];
extern UInt8 perAckTb2_g[MAX_NUM_CELLS];
extern UInt8 perNackTb2_g[MAX_NUM_CELLS];
extern UInt8 perUnsureTb2_g[MAX_NUM_CELLS];
/*CLPC_CHG*/
/* + CQI_4.1 */
ueProfile ueProfileInfo_g[MAX_NUM_CELLS][MAX_UE_PROFILES];
/* - CQI_4.1 */
/* SPR 12298 fix */
/* SPR 14223 Fix : Start */
//extern UInt8 ueProfileIndex_g[MAX_NUM_CELLS];
/* SPR 14223 Fix : End */

/* + MEAS_GAP_CHG */
inline void processMeasGapParam( ueContextForDownlink *ueDlConfigInfo_p ,
                                  DLUEContext *newUEcontext_p
                               );

inline  void  processMeasGapReConfig( ueContextForDownlink *dwlkUEInfo_p,
                                       DLUEContext *dlUEContext_p 
                                    );

/* - MEAS_GAP_CHG */
/*CLPC_CHG*/
static MacRetType reconfigureTpcPdcchConfigInfoForPucch(
        UInt8 release,
        UInt16 rnti,
        UInt8 tpcBitMapIndex,
        UInt8 dciFormat,
        DLUEContext *ueContext_p);
/*CLPC_CHG*/

#define NUMBER_OF_BITS_IN_1BYTE 8

/*****************************************************************************
 * Function Name  : rrc_cleanup_handler
 * Inputs         : arg - pointer to void
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function is a callback function when thread is
 *                  asynchronously cancelled during cleanup process. Here
 *                  thread specific data can be cleaned up if required.
 *****************************************************************************/

void rrc_cleanup_handler(void *arg)
{
    printf("In RRC Interface Thread - cleanup handler\n");
}


/*****************************************************************************
 * Function Name  : initRRCIntThread
 * Inputs         : arg - pointer to void
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This is entry function for the RRC Interface thread that
 *                  receives messages from RRC Layer on socket and process them.
 *****************************************************************************/

void * initRRCIntThread(void* arg)
{
    /* Set thread attributes for asynchronous cancellation */
    threadSetCancelState(THREAD_CANCEL_ENABLE, PNULL);
    threadSetCancelType(THREAD_CANCEL_ASYNCHRONOUS, PNULL);
    threadCleanupPush(&rrc_cleanup_handler, PNULL);

    readRRCMsgFromSocket();

    threadCleanupPop(1);
    return PNULL;
}


/****************************************************************************
 * Function Name  : createMACDLUEEntity
 * Inputs         :
 *                  ueDlConfigInfo_p contains following fields
 *                  - ueIndex -           Index into ueDLContextArr_g
 *                  - ueId -              UE Identifier
 *                  - uePriority -        UE Priority
 *                  - transportFormat_p   Pointer to the Transport Format 
 *                                        structure consisting of the MCSIndex
 *                                        and Modulation Scheme.
 *                  - maxRB -             Maximum number of Resource Blocks 
 *                                        that can be allocated to this UE 
 *                                        per TTI.
 *                  - mBitRate -          Maximum Bit Rate for this UE.
 *                  - gBitRate -          Guaranteed Bit Rate for this UE.
 *                  - hScheme  -          HARQ Scheme to be used for this UE 
 *                                        in case of retransmission.
 *                  - logicalChCount -    Number of logical channels being 
 *                                        currently configured for this UE.
 *                  - logicalChConfig_p - A pointer to an array for 
 *                                        different Logical Channel Config-
 *                                        ration data.
 *                                      
 * Outputs        : 
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS, MAC_LOG_CH_ERROR]
 * Variables      : 
 * Description    : The function is used to create a new UE context and add 
 *                  it's entry in the ueDLContextArr_g containig the UE Context 
 *                  pointers. It also increments the global count for the 
 *                  totalActiveUE using the semaphore ueQueueLoadSem.
 ****************************************************************************/

MacRetType createMACDLUEEntity ( ueContextForDownlink* ueDlConfigInfo_p, 
        UInt8 cellIndex)
{
    DLUEContext *newUEcontext_p = PNULL;
    DLLogicalChannelInfo *dlLogicalChInfo_p     = PNULL;
    UInt8 n = 0;
    UInt8 ackNackIndex = 0;
    UInt16 ueIndex =0;
    UInt8 resp = MAC_SUCCESS;
     /* SPR 15909 fix start */
    tickType_t currentTick = 0;
     /* SPR 15909 fix end */
    currentTick =  getCurrentglobaltick();
    ueIndex = ueDlConfigInfo_p->ueIndex;
    /*CLPC_CHG*/
    UInt8 row = 0;
    UInt8 column = 0;
    /*CLPC_CHG*/

    /* EICIC +*/
    UInt8 absLoop = 0;
    UInt8 numBitsCsiPattern;
    /* EICIC -*/

    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Entry. UE Index %d\n",__func__, ueIndex);
    if ( cellConfigUeSim_g[cellIndex].totalActiveUE == MAX_UE_SUPPORTED )
    {
        lteWarning("Already MAX_UE_SUPPORTED \n");
        return MAC_FAILURE;
    }
    if (dlUECtxInfoArr_g[ueIndex].dlUEContext_p != PNULL)
    {
        lteWarning (" Entry already exists in the UEContext Hash Table.\n");
        return MAC_FAILURE;
    }

    /* Allocating memory for UE Context Info*/
    newUEcontext_p = (DLUEContext *)getMemFromPool(sizeof (DLUEContext),PNULL);

    if (PNULL == newUEcontext_p)
    {
        ltePanic(" getMemFromPool() failed in createMACUEEntity() for \
            UEContext.\n");
        return MAC_INTERNAL_ERROR;    
    }
    
    /*CLPC_CHG*/

/* SPR_11713_Fix Start */

    /*
     * This tpcAckNackContext for all UE's is filled according to 
     * configuration file "AckNackCfg" file
     * for PCell . (cellIndex can be 0 / 1)
     * 
     * */

    ackNackIndex = cellIndex;

    newUEcontext_p->tpcAckNackContext.perAckTb1UE = perAckTb1_g[ackNackIndex];
    newUEcontext_p->tpcAckNackContext.perAckTb2UE = perAckTb2_g[ackNackIndex];
    newUEcontext_p->tpcAckNackContext.perNackTb1UE = perNackTb1_g[ackNackIndex];
    newUEcontext_p->tpcAckNackContext.perNackTb2UE = perNackTb2_g[ackNackIndex];
    newUEcontext_p->tpcAckNackContext.perSureTb1UE = 100;
    newUEcontext_p->tpcAckNackContext.perSureTb2UE = 100;
    newUEcontext_p->tpcAckNackContext.perUnsureTb1UE = 0;
    newUEcontext_p->tpcAckNackContext.perUnsureTb2UE = 0;
    /* SPS_CHG */
    /* Initializing UE COntext parameters */
    newUEcontext_p->spsDlInfo.requestType = SPS_RELEASE;
    newUEcontext_p->spsDlInfo.semiPersistentSchedIntervalDL = SPS_INTERVAL_MAX;
    newUEcontext_p->spsDlInfo.numberOfConfSpsProcesses = 0;
    /* SPS_CHG */
    /* 4x2 MIMO S */
    newUEcontext_p->ueCategory = ueDlConfigInfo_p->ueCategory;
    fprintf(stderr,"###### UE DL Category [%d] #######\n",newUEcontext_p->ueCategory);
    /* 4x2 MIMO E */
    /*Filling the ackNackCrcArray with ALL NACK*/
    for(row = 0; row < ACK_NACK_ARRAY_SIZE; row++)
    {
        for(column = 0; column < ACK_NACK_ARRAY_SIZE; column++)
        {
            newUEcontext_p->tpcAckNackContext.tb1_ack_nack_perUe_array[row][column]
                = tb1_ack_nack_value_array[ackNackIndex][row][column];
            newUEcontext_p->tpcAckNackContext.tb2_ack_nack_perUe_array[row][column]
                = tb2_ack_nack_value_array[ackNackIndex][row][column];

        }
    }
    /*CLPC_CHG*/
    /*
     * This tpcAckNackContextScell for CA UE's
     * is filled according to 
     * configuration file "AckNackCfg" for SCell . 
     * (cellIndex can be 1 / 0)
     * Below Table can be used in case data is scheduled on SCELL.
     * */
    
    ackNackIndex = !cellIndex;

    newUEcontext_p->tpcAckNackContextScell.perAckTb1UE = perAckTb1_g[ackNackIndex];
    newUEcontext_p->tpcAckNackContextScell.perAckTb2UE = perAckTb2_g[ackNackIndex];
    newUEcontext_p->tpcAckNackContextScell.perNackTb1UE = perNackTb1_g[ackNackIndex];
    newUEcontext_p->tpcAckNackContextScell.perNackTb2UE = perNackTb2_g[ackNackIndex];
    newUEcontext_p->tpcAckNackContextScell.perSureTb1UE = 100;
    newUEcontext_p->tpcAckNackContextScell.perSureTb2UE = 100;
    newUEcontext_p->tpcAckNackContextScell.perUnsureTb1UE = 0;
    newUEcontext_p->tpcAckNackContextScell.perUnsureTb2UE = 0;
    /* SPS_CHG */
    /* Initializing UE COntext parameters */
    newUEcontext_p->spsDlInfo.requestType = SPS_RELEASE;
    newUEcontext_p->spsDlInfo.semiPersistentSchedIntervalDL = SPS_INTERVAL_MAX;
    newUEcontext_p->spsDlInfo.numberOfConfSpsProcesses = 0;
    /* SPS_CHG */
    /* 4x2 MIMO S */
    newUEcontext_p->ueCategory = ueDlConfigInfo_p->ueCategory;
    fprintf(stderr,"###### UE DL Category [%d] #######\n",newUEcontext_p->ueCategory);
    /* 4x2 MIMO E */
    /*Filling the ackNackCrcArray with ALL NACK*/
    for(row = 0; row < ACK_NACK_ARRAY_SIZE; row++)
    {
        for(column = 0; column < ACK_NACK_ARRAY_SIZE; column++)
        {
            newUEcontext_p->tpcAckNackContextScell.tb1_ack_nack_perUe_array[row][column]
                = tb1_ack_nack_value_array[ackNackIndex][row][column];
            newUEcontext_p->tpcAckNackContextScell.tb2_ack_nack_perUe_array[row][column]
                = tb2_ack_nack_value_array[ackNackIndex][row][column];

        }
    }
    /*CLPC_CHG*/

/* SPR_11713_Fix End */

    
    /*CLPC_CHG*/

    /* Logical Channel Structure Initialisation */
    /* Initialising all the Logical Channels with default values */ 
    for (n = 0; n < MAX_NUMBER_OF_LOGICAL_CHANNEL; n++)
    {
        dlLogicalChInfo_p = &newUEcontext_p->logicalChannel[n];
        dlLogicalChInfo_p->logicalChannelId  = INVALID_LCID;
        dlLogicalChInfo_p->lastScheduledTick = currentTick;
        dlLogicalChInfo_p->queueLoad         = 0;
        dlLogicalChInfo_p->rlcMode           = INVALID_RLC_MODE;
        dlLogicalChInfo_p->rlcSNFieldLength  = 0; 
        dlLogicalChInfo_p->logicalChannelPriority = 0;
    }

    newUEcontext_p->maxLcId = 0;
    if(ueDlConfigInfo_p->dwlkLogicalChCount != 0)
    {
        if (MAC_FAILURE == addMACDLLogicalChannel(newUEcontext_p,
                                                 ueDlConfigInfo_p->dwlkLogicalChCount,
                                               ueDlConfigInfo_p->logicalChConfig))
        {
            lteWarning ("Failed in adding Logical Channels for a UE.\n");
            resp = MAC_LOG_CH_ERROR;
        }
    }
                                                
    /* Initializations */
    newUEcontext_p->crnti = ueDlConfigInfo_p->cRnti;
    newUEcontext_p->ueIndex = ueIndex;
    /*indicates the priority of this UE*/
    newUEcontext_p->uePriority = ueDlConfigInfo_p->uePriority;
    newUEcontext_p->maxNbOfRBs = ueDlConfigInfo_p->maxRB;
    newUEcontext_p->isPdcchAllocatedFlag = FALSE;
    newUEcontext_p->pendingDRXAckNack    = FALSE;
    //newUEcontext_p->dlCodingRate = ueDlConfigInfo_p->codingRate; -- Anuj
    newUEcontext_p->ueState                 = MAC_UE_ACTIVE;
    newUEcontext_p->drxFlag                 = FALSE;
    newUEcontext_p->timingAdvanceValue      = INVALID_TA_VALUE;
    newUEcontext_p->isAlreadyConsideredFlag = INVALID_ALREADY_CONSIDERED_FLAG;
    newUEcontext_p->lastScheduledTick = currentTick;
    newUEcontext_p->assignedRBQueueLoadSchIndex = 0;
    newUEcontext_p->assignedRBQueueLoadRLCIndex =0;
    newUEcontext_p->effectiveBitRate = 0;
    newUEcontext_p->ueQueueLoad = 0;
	/* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
    newUEcontext_p->isHDFddFlag = ueDlConfigInfo_p->isHDFddFlag;
    if(TRUE == newUEcontext_p->isHDFddFlag)
    {
        ueDlConfigInfo_p->spsDownlinkInfo.requestType = SPS_RELEASE;
    }
#endif
	/* HD FDD Changes End */
    /* + MEAS_GAP_CHG */
    newUEcontext_p->measGapConfigType = RELEASE;
    if( TRUE == ueDlConfigInfo_p->measurementGapRequestType )
    {
        newUEcontext_p->measGapConfigType = ueDlConfigInfo_p->measGapConfigType;

        if( SETUP ==  newUEcontext_p->measGapConfigType )
        {   
            processMeasGapParam(ueDlConfigInfo_p , newUEcontext_p);
        }
    }    
   /* - MEAS_GAP_CHG */
   /*CLPC_CHG*/
   if( !ueDlConfigInfo_p->tpcPdcchConfigPucch.release )
   {
       memcpy(&(newUEcontext_p->tpcPdcchConfigPucch),&(ueDlConfigInfo_p->tpcPdcchConfigPucch),
               sizeof (ueDlConfigInfo_p->tpcPdcchConfigPucch));
   }
   /*CLPC_CHG*/
   newUEcontext_p->ueState = MAC_UE_ACTIVE;
   /* SPS_CHG */
   if (ueDlConfigInfo_p->isSpsDlInfoPresent)
   {
       newUEcontext_p->spsDlInfo.requestType = ueDlConfigInfo_p->spsDownlinkInfo.requestType;
       if (SPS_SETUP ==  ueDlConfigInfo_p->spsDownlinkInfo.requestType)
       {
           newUEcontext_p->spsDlInfo.semiPersistentSchedIntervalDL = 
           ueDlConfigInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.semiPersistentSchedIntervalDL;
           newUEcontext_p->spsDlInfo.numberOfConfSpsProcesses = 
           ueDlConfigInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.numberOfConfSpsProcesses;
       }
   }

   	/* EICIC +*/
#ifdef FDD_CONFIG
       numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellConfigUeSim_g[cellIndex].subFrameAssignment)
   {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellConfigUeSim_g[cellIndex].subFrameAssignment)
   {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
   }
   else
   {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
   }
#endif

		for (absLoop = 0; absLoop < numBitsCsiPattern ; absLoop++)
		{	
		newUEcontext_p->csiMeasurementSubset1[absLoop] = 0;
		newUEcontext_p->csiMeasurementSubset2[absLoop] = 0;
		}

        newUEcontext_p->riConfigIndex_2 = ueDlConfigInfo_p->riConfigIndex_2 ;
        newUEcontext_p->cqiPMIConfigIndex_2 =  ueDlConfigInfo_p->cqiPMIConfigIndex_2 ;

    	if (SETUP >= ueDlConfigInfo_p->cqiSubsetConfigRequestType)
	{
		for (absLoop = 0; absLoop < numBitsCsiPattern; absLoop++)
		{	
		newUEcontext_p->csiMeasurementSubset1[absLoop] = ueDlConfigInfo_p->csiMeasurementSubset1[absLoop];
		newUEcontext_p->csiMeasurementSubset2[absLoop] = ueDlConfigInfo_p->csiMeasurementSubset2[absLoop];
		}
    	}

	/* EICIC -*/
   /* SPS_CHG */
   dlUECtxInfoArr_g[ueIndex].dlUEContext_p = newUEcontext_p;
   LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Exit.\n",__func__);

    return resp;    
}


/****************************************************************************
 * Function Name  : reconfigureMACDLUEEntity
 * Inputs         : - ueIndex -           UE Identifier 
 *                  - maxRB -             Maximum number of Resource Blocks 
 *                                        that can be allocated to this UE 
 *                                        per TTI.
 *                  - logicalChCount -    Number of logical channels being 
 *                                        currently configured for this UE.
 *                  - logicalChConfig_p - A pointer to an array for 
 *                                        different Logical Channel Config-
 *                                        ration data.
 *                  - cqiReportInd
 *                  - cqiMode
 *                  - cqiTimer
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Variables      : 
 * Description    : This API is used to fetch the UE Context pointer from the
 *                  global array and then reconfigure the various UE Context 
 *                  parameters for the requested UE.
 ****************************************************************************/

MacRetType reconfigureMACDLUEEntity (ueContextForDownlink *dwlkUEInfo_p,UInt8 cellIndex)
{
    LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,"%s Entry.\n",__func__);
    UInt32 n       = 0;
    UInt16 ueIndex = 0;
    UInt32 logicalChCount  = 0;
    UInt32 tempLogicalChID = 0;
    UInt32 lcOperationType = 0;
    //DLCQIInfo   *dlCQIInfo_p    = PNULL;
    UInt32 logChConfigResp      = MAC_SUCCESS;
    UInt32 returnType           = MAC_SUCCESS;
    DLUEContext *newUEcontext_p = PNULL;
    LogicalChannelConfigInfo* logicalChConfig_p = PNULL;
    /* EICIC +*/
    UInt8 numBitsCsiPattern;
    UInt8 absLoop;
    /* EICIC -*/
    
    logicalChConfig_p = dwlkUEInfo_p->logicalChConfig;
    logicalChCount    = dwlkUEInfo_p->dwlkLogicalChCount; 
    
    ueIndex = dwlkUEInfo_p->ueIndex;
    /* Going to search UE context in index table */
    newUEcontext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    if ( PNULL == newUEcontext_p )
    {
        LTE_LOG(LOG_DETAIL,macModuleLogDetail_p," Unable to\
        reconfigure the UE Entity. \n");
        return MAC_FAILURE;
    }

    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
    newUEcontext_p->isHDFddFlag = dwlkUEInfo_p->isHDFddFlag;
    if(TRUE == newUEcontext_p->isHDFddFlag)
    {
        dwlkUEInfo_p->spsDownlinkInfo.requestType = SPS_RELEASE;
    }
#endif
    /* HD FDD Changes End */
    /* Reconfiguring the provisioned UE Context parameters */
    /*CLPC Change*/
    if (dwlkUEInfo_p->isPAPresent == TRUE)
    {
        newUEcontext_p->pA = dwlkUEInfo_p->pA;
    }

   if (dwlkUEInfo_p->isTpcPdcchConfigPucchPresent == TRUE)
   {
       returnType = reconfigureTpcPdcchConfigInfoForPucch (
               dwlkUEInfo_p->tpcPdcchConfigPucch.release,
               dwlkUEInfo_p->tpcPdcchConfigPucch.tpcRnti,
               dwlkUEInfo_p->tpcPdcchConfigPucch.tpcBitMapIndex,
               dwlkUEInfo_p->tpcPdcchConfigPucch.isConfiguredForDci3Or3A,
               newUEcontext_p);

       if (returnType ==  MAC_FAILURE)
       {
           return returnType;
       }
   }
    
        /* CQI related Info */
    if ( dwlkUEInfo_p->maxRB )
    {
        newUEcontext_p->maxNbOfRBs = dwlkUEInfo_p->maxRB;
    }
   /* SPS_CHG */
   if (dwlkUEInfo_p->isSpsDlInfoPresent &&  newUEcontext_p->spsDlInfo.requestType != 
       dwlkUEInfo_p->spsDownlinkInfo.requestType)
   {
       newUEcontext_p->spsDlInfo.requestType = dwlkUEInfo_p->spsDownlinkInfo.requestType;
       if (SPS_SETUP == dwlkUEInfo_p->spsDownlinkInfo.requestType)
       {
           newUEcontext_p->spsDlInfo.semiPersistentSchedIntervalDL = 
           dwlkUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.semiPersistentSchedIntervalDL;
           newUEcontext_p->spsDlInfo.numberOfConfSpsProcesses = 
           dwlkUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.numberOfConfSpsProcesses;
       }
       else
       {
           newUEcontext_p->spsDlInfo.semiPersistentSchedIntervalDL = SPS_INTERVAL_MAX;
           newUEcontext_p->spsDlInfo.numberOfConfSpsProcesses = 0;
       }
   }
   /* SPS_CHG */

    if ( PNULL == logicalChConfig_p)
    {
        LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,
                "Received a NULL Pointer for Logical Channels.\n");
        return MAC_SUCCESS;
    }

    /* Reconfiguring the Logical Channel Structures 
       and setting the rest as INVALID_LCID 255 */
    for (n = 0; n < logicalChCount; n++)
    {
        tempLogicalChID = logicalChConfig_p[n].lchId;
        lcOperationType = logicalChConfig_p[n].operationType;
        if ( ADD == lcOperationType )
        {
            if ( INVALID_LCID != newUEcontext_p->
                   logicalChannel[tempLogicalChID].logicalChannelId )
            {
                lteWarning ("Logical Channel already added.\n");
                logicalChConfig_p[n].result = MAC_LOG_CH_EXISTS;
                logChConfigResp = MAC_LOG_CH_ERROR;
                continue;
            }
            LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,
                      "Adding a new logical channel. \n");

            returnType = addMACDLLogicalChannel(newUEcontext_p,1,
                                                &logicalChConfig_p[n]);
            if( MAC_FAILURE == returnType )
            {
                logChConfigResp = MAC_LOG_CH_ERROR;
            }
        }
        else if ( RECONFIGURE == lcOperationType )
        {
            if ( INVALID_LCID == newUEcontext_p->
                              logicalChannel[tempLogicalChID].
                    logicalChannelId)
            {
                lteWarning ("Trying to reconfigure a Logical Channel \
                        that does not exists.\n");
                logChConfigResp = MAC_LOG_CH_ERROR;
                logicalChConfig_p[n].result = MAC_LOG_CH_NOT_EXISTS;
                continue;
            }

            LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,
                                     "Reconfiguring a logical channel. \n");
            returnType = reconfigureMACDLLogicalChannel( newUEcontext_p,
                                                       &logicalChConfig_p[n]);
            if( MAC_FAILURE == returnType)
            {
                logChConfigResp = MAC_LOG_CH_ERROR;
            }
        }
        else if ( DELETE == lcOperationType)
        {
            if ( INVALID_LCID == newUEcontext_p->
                                 logicalChannel[tempLogicalChID].
                    logicalChannelId)
            {
                lteWarning ("Trying to delete a Logical Channel that \
                        does not exists.\n");
                logChConfigResp = MAC_LOG_CH_ERROR;
                logicalChConfig_p[n].result = MAC_LOG_CH_NOT_EXISTS;
                continue;
            }
            deleteDLUELogicalChannel(newUEcontext_p, tempLogicalChID);
            
            LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,
                                   "Deleting a logical channel.\n");
            logicalChConfig_p[n].result = MAC_SUCCESS;
        }
        else
        {
            LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,
                          "Invalid operation to be performed.\n");
            continue;
        }
    }

    #ifdef FDD_CONFIG
       numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellConfigUeSim_g[cellIndex].subFrameAssignment)
   {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellConfigUeSim_g[cellIndex].subFrameAssignment)
   {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
   }
   else
   {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
   }
#endif


	/* EICIC +*/
		for (absLoop = 0; absLoop < numBitsCsiPattern ; absLoop++)
		{	
		newUEcontext_p->csiMeasurementSubset1[absLoop] = 0;
		newUEcontext_p->csiMeasurementSubset2[absLoop] = 0;
		}

        newUEcontext_p->riConfigIndex_2 = dwlkUEInfo_p->riConfigIndex_2 ;
        newUEcontext_p->cqiPMIConfigIndex_2 =  dwlkUEInfo_p->cqiPMIConfigIndex_2 ;

    	if (SETUP >= dwlkUEInfo_p->cqiSubsetConfigRequestType)
	{
		for (absLoop = 0; absLoop < numBitsCsiPattern ; absLoop++)
		{	
		newUEcontext_p->csiMeasurementSubset1[absLoop] = dwlkUEInfo_p->csiMeasurementSubset1[absLoop];
		newUEcontext_p->csiMeasurementSubset2[absLoop] = dwlkUEInfo_p->csiMeasurementSubset2[absLoop];
		}
    	}

	/* EICIC -*/

    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Exit.\n",__func__);

    return logChConfigResp;
}
/****************************************************************************
 * Function Name  : deleteMACDLUEEntity
 * Inputs         : ueIndex - index of ueContext in ueDLContextArr_g
 * Outputs        : None.
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Variables      : 
 * Description    : This API is used to delete a UE Context from the MAC 
 *                  system. 
 *                  - Then it is validated that whether this UE is currently 
 *                    being processed by any of the worker threads using the 
 *                    'isUEContextInUse' variable.
 *                  - If the UE is currently being processed then the pending
 *                    Delete is set for this UE and the scheduler thread on 
 *                    completion will delete the UE,
 *                    else,
 *                    The UE is deleted.
 ****************************************************************************/

MacRetType deleteMACDLUEEntity ( UInt16 ueIndex, UInt16 transactionId, UInt8 cnfFlag)
{
	UInt16 ueId = 0;
	LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Entry.\n",__func__);

	DLUEContext *ueContext_p = PNULL;

	ueContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
	if (ueContext_p == PNULL)
	{
		lteWarning (" Entry does not exists in the UEContext Array.\n");
		LTE_LOG(LOG_WARNING,&MACModuleLogDetail_g," Unable to delete the UE Entity. \n");
		return MAC_FAILURE;
	}
	if(FALSE == cnfFlag)
	{
		ueId = 0xFFFF;
	}
	else {
		ueId = ueContext_p->crnti;
	}    
	freeMemPool(ueContext_p);
	dlUECtxInfoArr_g[ueIndex].dlUEContext_p = PNULL;
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : addMACDLLogicalChannel
 * Inputs         : newUEContext_p - pointer to UE context.
 *                  logicalChCount - number of logical channels of UE.
 *                  logicalChConfig_p - A pointer to the LogicalChannelConfig
 *                                      array containing the various
 *                                      configuration parameters for the 
 *                                      logical channel.
 *                 
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Variables      : 
 * Description    : This API is used to add a logical channel as per the 
 *                  configuration for a given UE.
 *                  If a UE does not exist, for which this API is invoked then
 *                  return MAC_FAILURE.
 ****************************************************************************/

MacRetType addMACDLLogicalChannel (DLUEContext* newUEContext_p, UInt8 logicalChCount,
                                 LogicalChannelConfigInfo* logicalChConfig_p)
{
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Entry.\n",__func__);
    
    UInt8 count = 0;
	UInt32 tempLogicalChId = 0;
    UInt8 n = 0;
	DLLogicalChannelInfo *dlLogicalChannel_p = PNULL; 
    if (PNULL == logicalChConfig_p)
    {
        LTE_LOG(LOG_WARNING,&MACModuleLogDetail_g,"Received a NULL Pointer for Logical Channels.\n");
        return MAC_FAILURE;
    }

    for (n = 0; n < logicalChCount; n++)
    {
        tempLogicalChId = logicalChConfig_p[n].lchId;
		dlLogicalChannel_p = &newUEContext_p->logicalChannel[tempLogicalChId];

		if ( INVALID_LCID != dlLogicalChannel_p->logicalChannelId )
           {
               lteWarning("The logical channel has already been added \
                   before - Not adding again.\n");
               logicalChConfig_p[n].result = MAC_LOG_CH_EXISTS;
               continue;
           }
           
		dlLogicalChannel_p->rlcMode          = logicalChConfig_p[n].rlcMode;
		dlLogicalChannel_p->rlcSNFieldLength = logicalChConfig_p[n].rlcSNFieldLength;
		dlLogicalChannel_p->logicalChannelId = logicalChConfig_p[n].lchId;
		dlLogicalChannel_p->logicalChannelPriority = logicalChConfig_p[n].lchPriority;
		if ( newUEContext_p->maxLcId < tempLogicalChId )
		{
			newUEContext_p->maxLcId = tempLogicalChId;
		}

            count++;
            logicalChConfig_p[n].result = MAC_SUCCESS;
        }
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Exit.\n",__func__);

    if(count < logicalChCount)
    {
        return MAC_FAILURE;
    }
    
    return MAC_SUCCESS;    
}

/****************************************************************************
 * Function Name  : reconfigureMACDLLogicalChannel
 * Inputs         : ueIndex - UE Identifier for which the logical channel 
 *                            needs to be reconfigured.
 *                  logicalChConfig_p - A pointer to the LogicalChannelConfig
 *                                      array containing the various
 *                                      configuration parameters for the 
 *                                      logical channel.
 *                 
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Variables      : 
 * Description    : This API is used to reconfigure a logical channel as 
 *                  per the configuration for a given UE.
 *                  If a UE does not exist, for which this API is invoked or 
 *                  the logical channel does not exist,then return MAC_FAILURE.
  ****************************************************************************/

MacRetType reconfigureMACDLLogicalChannel (DLUEContext* newUEcontext_p,
                         LogicalChannelConfigInfo* logicalChConfig_p)
{
    LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,"%s Entry.\n",__func__);
    DLLogicalChannelInfo *logicalChannel_p = PNULL;
    /* Temporary variable for Logical Channel Id to be reconfigured */
    UInt8 tempLogicalChId = 0;

    tempLogicalChId = logicalChConfig_p->lchId;

    if ( INVALID_LCID == newUEcontext_p->
            logicalChannel[tempLogicalChId].logicalChannelId)
    {
        lteWarning ("The Logical Channel requested to be reconfigured \
                has not been added yet. \n");
        logicalChConfig_p->result = MAC_LOG_CH_NOT_EXISTS;    
        return MAC_FAILURE;
    }
    logicalChannel_p = &newUEcontext_p->logicalChannel[tempLogicalChId];

    logicalChannel_p->rlcMode             = logicalChConfig_p->rlcMode;
    logicalChannel_p->rlcSNFieldLength    = logicalChConfig_p->rlcSNFieldLength;

    logicalChConfig_p->result = MAC_SUCCESS;
    LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,"[%s] Exit. ",__func__);
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : createMACUEEntity
 * Inputs         : dwlkUEInfo_p - Pointer to ueContextForDownlink required
 *                                 to configure MAC DL UE Context.
 *                  uplkUEInfo_p - Pointer to ueContextForUplink required
 *                                 to configure MAC UL UE Context.
 *                  rlcConfigInfo_p - Pointer to RlcConfigReq required to
 *                                    configure RLC UE Context.
 *                  uplkLCCount - Number of LCs configured at MAC UL.
 *                  transactionId - transaction Id received in request from RRC
 * Outputs        : None
 * Returns        : ACK on Success, NACK on Failure
 * Variables      :
 * Description    : This API takes pointers to MAC & RLC Info required to 
 *                  configure UE context at Layer2. It returns ACK if SUCCESS
 *                  and ueIdx allocated for UE will be stored in the 
 *                  dwlkUEInfo_p passed as an input arguement.
 ****************************************************************************/

UInt16 createMACUEEntity(ueContextForDownlink *dwlkUEInfo_p, ueContextForUplink*
		uplkUEInfo_p, UInt8 uplkLCCount, 
		UInt16 transactionId, UInt8 cellIndex) 
{
	UInt16 resp = NACK;
	UInt16 ulResp = MAC_SUCCESS;
	UInt16 dlResp = MAC_SUCCESS;
	UInt8 i = 0, loop = 0;
	UInt16 ueIndex = 0;
	RNTIInfo *rntiInfo_p = PNULL;
    /* + CQI_4.1 */
    DLUEContext *dlUEContext_p = PNULL;
    /* - CQI_4.1 */
    /* + CQI_5.0 */
    UInt8 riIndex = 0;
    /* - CQI_5.0 */
    ueIndex = dwlkUEInfo_p->ueIndex;
    /* Create UE Context at MAC Dwlk & Uplk */
    dlResp = createMACDLUEEntity(dwlkUEInfo_p, cellIndex);
    if((MAC_SUCCESS == dlResp) || (MAC_LOG_CH_ERROR == dlResp))
    {
        if(MAC_LOG_CH_ERROR == dlResp)
        {
            for(i=0; i<dwlkUEInfo_p->dwlkLogicalChCount; i++)
            {
                if(MAC_SUCCESS != dwlkUEInfo_p->logicalChConfig[i].result)
                {
                    for(loop = 0; loop <uplkUEInfo_p->numLogicalChannels; loop++)
                    {
                        if(uplkUEInfo_p->lcInfo[loop].lchId ==
                           dwlkUEInfo_p->logicalChConfig[i].lchId)
                        {
                            uplkUEInfo_p->lcInfo[loop].result = 
                                dwlkUEInfo_p->logicalChConfig[i].result;
                        }
                    }
                }
            }    
        }
        ulResp = createMACULUEEntity(dwlkUEInfo_p->ueIndex, 
                uplkLCCount, uplkUEInfo_p, cellIndex);

        if((MAC_FAILURE == ulResp) || (MAC_INTERNAL_ERROR == ulResp))
        {
            lteWarning("Error creating MAC UL Entity\n");
            deleteMACDLUEEntity(dwlkUEInfo_p->ueIndex, transactionId, FALSE);
        }
        else
        {
            /* Set DL, UL flags to 1 */
            LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"Created UL & DL UE Context at MAC Layer\n");
            resp = ACK;
        }
    }

    if(NACK != resp) 
    {
        /* Mark entry into crntiToIdxTbl_g */
#if 0
        crntiToIdxTbl_g[(dwlkUEInfo_p->cRnti - lowerCRNTIBound_g)
            %crntiRange_g] = dwlkUEInfo_p->ueIndex;
#endif
        rntiInfo_p = getRntiInfofromRNTIMap(dwlkUEInfo_p->cRnti, cellIndex);
        if ( rntiInfo_p == PNULL ||  rntiInfo_p->rntiType == CRNTI )
        {
            lteWarning("ue already exist \n");
            return MAC_FAILURE;
        }
        rntiInfo_p->ueIndex = dwlkUEInfo_p->ueIndex;
        //rntiInfo_p->state = UE_ACTIVE;
        rntiInfo_p->state = SR_NOT_SENT;
        rntiInfo_p->rntiType = CRNTI;
        rntiInfo_p->rnti = dwlkUEInfo_p->cRnti;
        /* + CQI_4.1 */
        dlUEContext_p = dlUECtxInfoArr_g[rntiInfo_p->ueIndex].dlUEContext_p;
        dlUEContext_p->dlCQIInfo.indexTrack = 0;/*EICIC Enhancement*/
        dlUEContext_p->dlCQIInfo.ueCqiIndexNum = 0;
        dlUEContext_p->dlCQIInfo.bandwidthPartCount = 0;
        dlUEContext_p->dlCQIInfo.lastWideband = 0;
        dlUEContext_p->dlCQIInfo.tempRecvSfnSf = 0;
        dlUEContext_p->dlCQIInfo.bandwidthPartCount = 0;
        dlUEContext_p->dlCQIInfo.widebandPeriodicity = 0;
        dlUEContext_p->dlCQIInfo.nextSubbandOccurrence = 0;
        dlUEContext_p->dlCQIInfo.nextWidebandOccurrence = 0;
        /* Bandwidth Fix */
        dlUEContext_p->dlCQIInfo.calculateBandwidthPartFlag = FALSE;
        /* Bandwidth Fix */
        /* EICIC +*/
         dlUEContext_p->dlCQIInfo.widebandPeriodicity_2 = 0;
        dlUEContext_p->dlCQIInfo.nextSubbandOccurrence_2 = 0;
        dlUEContext_p->dlCQIInfo.nextWidebandOccurrence_2= 0;
         dlUEContext_p->dlCQIInfo.bandwidthPartCount_2= 0;
        /* EICIC -*/
        uplkUEInfo_p->periodicModeType = 0;
        dlUEContext_p->dlCQIInfo.periodicModeType = 0;
        dlUEContext_p->dlCQIInfo.aperiodicModeType = 255; 
        dlUEContext_p->dlCQIInfo.cqiformatIndicatorPeriodic = 
                      uplkUEInfo_p->cqiformatIndicatorPeriodic;
        /* - CQI_4.1 */
        /* + CQI_5.0 */ 
        dlUEContext_p->dlCQIInfo.cqiInfoFlagInCreate = dwlkUEInfo_p->cqiInfoFlag;
        dlUEContext_p->dlCQIInfo.ueRIIndexNum = 0;
        /* - CQI_5.0 */ 
#ifdef UT_TESTING
        printf("entry in rnti info map for index = %d\n", rntiInfo_p->ueIndex);
#endif

    }
        /* SPR 8748 FIX START */
    else
    {
        return MAC_FAILURE;
    }
        /* SPR 8748 FIX END */
  /* + TM7_8 Changes Start */
    if(8 == dwlkUEInfo_p->transmissionMode)
    {
        dlUEContext_p->isPmiRiConfigured_V920 = dwlkUEInfo_p->pmiRiReportV920;
    }
    /* - TM7_8 Changes End */
    /* + CQI_4.1 */
    if ( (uplkUEInfo_p->cqiformatIndicatorPeriodic == 0) && 
         ((1 == dwlkUEInfo_p->transmissionMode) ||
         (2 == dwlkUEInfo_p->transmissionMode) ||
         (3 == dwlkUEInfo_p->transmissionMode) ||      
         /* + TM7_8 Changes Start */
         (7 == dwlkUEInfo_p->transmissionMode) ||
         ((8 == dwlkUEInfo_p->transmissionMode)&&
         (TRUE != dlUEContext_p->isPmiRiConfigured_V920))))      
         /* - TM7_8 Changes End */

    {
        dlUEContext_p->dlCQIInfo.periodicModeType = CQI_PERIODIC_TYPE_10;
        dlUEContext_p->dlCQIInfo.periodicCQIMode10_p = &cqiPeriodicReportMode10_g[cellIndex][0];
         dlUEContext_p->dlCQIInfo.periodicCQIMode10Abs_p = &cqiPeriodicReportMode10_g_2[cellIndex][0];

        uplkUEInfo_p->periodicModeType = CQI_PERIODIC_TYPE_10;
    }
    else if ( (uplkUEInfo_p->cqiformatIndicatorPeriodic == 0) &&
              ((4 == dwlkUEInfo_p->transmissionMode) ||
              /* + TM6_5.2 */
              (6 == dwlkUEInfo_p->transmissionMode)||         
              /* - TM6_5.2 */
              /* + TM7_8 Changes Start */
              ((8 == dwlkUEInfo_p->transmissionMode)&&
              (TRUE == dlUEContext_p->isPmiRiConfigured_V920))))         
              /* - TM7_8 Changes End */

    {
        dlUEContext_p->dlCQIInfo.periodicModeType = CQI_PERIODIC_TYPE_11;
        dlUEContext_p->dlCQIInfo.periodicCQIMode11_p = &cqiPeriodicReportMode11_g[cellIndex][0];
         dlUEContext_p->dlCQIInfo.periodicCQIMode11Abs_p = &cqiPeriodicReportMode11_g_2[cellIndex][0];
        uplkUEInfo_p->periodicModeType = CQI_PERIODIC_TYPE_11;
    }
    else if ( (uplkUEInfo_p->cqiformatIndicatorPeriodic == 1) && 
              ((1 == dwlkUEInfo_p->transmissionMode) ||
              (2 == dwlkUEInfo_p->transmissionMode) ||
              (3 == dwlkUEInfo_p->transmissionMode) ||         
              /* + TM7_8 Changes Start */
              (7 == dwlkUEInfo_p->transmissionMode) ||
              ((8 == dwlkUEInfo_p->transmissionMode)&&
              (TRUE != dlUEContext_p->isPmiRiConfigured_V920))))      
              /* - TM7_8 Changes End */
    {
        dlUEContext_p->dlCQIInfo.periodicModeType = CQI_PERIODIC_TYPE_20;
        dlUEContext_p->dlCQIInfo.periodicCQIMode20_p = &cqiPeriodicReportMode20_g[0];
        dlUEContext_p->dlCQIInfo.periodicCQIMode20Abs_p = &cqiPeriodicReportMode20_g_2[0];
        /* Bandwidth Fix */
        dlUEContext_p->dlCQIInfo.calculateBandwidthPartFlag = TRUE;
        /* Bandwidth Fix */
        uplkUEInfo_p->periodicModeType = CQI_PERIODIC_TYPE_20;
    }   
    else if ( (uplkUEInfo_p->cqiformatIndicatorPeriodic == 1) &&
              ((4 == dwlkUEInfo_p->transmissionMode) ||       
              /* + TM6_5.2 */
              (6 == dwlkUEInfo_p->transmissionMode)||         
              /* - TM6_5.2 */
              /* + TM7_8 Changes Start */
              ((8 == dwlkUEInfo_p->transmissionMode)&&         
              (TRUE == dlUEContext_p->isPmiRiConfigured_V920))))         
              /* - TM7_8 Changes End */
              /* - TM7_8 Changes End */
    {
        dlUEContext_p->dlCQIInfo.periodicModeType = CQI_PERIODIC_TYPE_21;
        dlUEContext_p->dlCQIInfo.periodicCQIMode21_p = &cqiPeriodicReportMode21_g[0];
		dlUEContext_p->dlCQIInfo.periodicCQIMode21_p = &cqiPeriodicReportMode21_g_2[0];
        /* Bandwidth Fix */
        dlUEContext_p->dlCQIInfo.calculateBandwidthPartFlag = TRUE;
        /* Bandwidth Fix */
        uplkUEInfo_p->periodicModeType = CQI_PERIODIC_TYPE_21;
    }
    dlUEContext_p->dlCQIInfo.aperiodicModeType = dwlkUEInfo_p->cqiMode; 
    fprintf(stderr,"Aperiodic Maode Type[%d] \n", dlUEContext_p->dlCQIInfo.aperiodicModeType);
    if (dlUEContext_p->dlCQIInfo.aperiodicModeType == 0)
        dlUEContext_p->dlCQIInfo.aperiodicModeType = CQI_MODE_1_2;
    if (CQI_MODE_1_2 == dlUEContext_p->dlCQIInfo.aperiodicModeType)  
    {
        dlUEContext_p->dlCQIInfo.aperiodicCQIMode12_p = &cqiAperiodicReportMode12_g[cellIndex][0];
		/* SPR 14062 fix start */
        dlUEContext_p->dlCQIInfo.RI = dwlkUEInfo_p->numOfLayer;
		/* SPR 14062 fix end */
    }    
    else if (CQI_MODE_2_0 == dlUEContext_p->dlCQIInfo.aperiodicModeType)  
    {
        dlUEContext_p->dlCQIInfo.aperiodicCQIMode20_p = &cqiAperiodicReportMode20_g[cellIndex][0];
		/* SPR 14062 fix start */
        dlUEContext_p->dlCQIInfo.RI = dwlkUEInfo_p->numOfLayer;
		/* SPR 14062 fix end */
    }    
    else if (CQI_MODE_2_2 == dlUEContext_p->dlCQIInfo.aperiodicModeType)  
    {
        dlUEContext_p->dlCQIInfo.aperiodicCQIMode22_p = &cqiAperiodicReportMode22_g[cellIndex][0];
		/* SPR 14062 fix start */
        dlUEContext_p->dlCQIInfo.RI = dwlkUEInfo_p->numOfLayer;
		/* SPR 14062 fix end */
    }    
    else if (CQI_MODE_3_0 == dlUEContext_p->dlCQIInfo.aperiodicModeType)  
    {
        dlUEContext_p->dlCQIInfo.aperiodicCQIMode30_p = &cqiAperiodicReportMode30_g[cellIndex][0];
		/* SPR 14062 fix start */
        dlUEContext_p->dlCQIInfo.RI = dwlkUEInfo_p->numOfLayer;
		/* SPR 14062 fix end */
    }    
    else if (CQI_MODE_3_1 == dlUEContext_p->dlCQIInfo.aperiodicModeType)  
    {
        dlUEContext_p->dlCQIInfo.aperiodicCQIMode31_p = &cqiAperiodicReportMode31_g[cellIndex][0];
		/* SPR 14062 fix start */
        dlUEContext_p->dlCQIInfo.RI = dwlkUEInfo_p->numOfLayer;
		/* SPR 14062 fix end */
    }    
    dlUEContext_p->dlCQIInfo.ueCqiProfileNum =\
		ueProfileInfo_g[cellIndex][(ueIndex%MAX_UE_PROFILES)].dlCqiProfileId;
    /* - CQI_4.1 */
 
   /* + CQI_5.0 */
   /* + SPR 13126 Changes */ 
   dlUEContext_p->dlCQIInfo.riValue_p = &RIValues_g[cellIndex][0]; 
   dlUEContext_p->dlCQIInfo.riValueAbs_p = &RIValues_g_2[cellIndex][0];      /* EICIC +-*/
   /* - SPR 13126 Changes */ 
   dlUEContext_p->dlCQIInfo.isULSCHPDUType[0] = 0;
   dlUEContext_p->transmissionMode =  dwlkUEInfo_p->transmissionMode;

   if( ( uplkUEInfo_p->periodicModeType == CQI_PERIODIC_TYPE_10) ||  
       ( uplkUEInfo_p->periodicModeType == CQI_PERIODIC_TYPE_11) || 
       ( uplkUEInfo_p->periodicModeType == CQI_PERIODIC_TYPE_20) || 
       ( uplkUEInfo_p->periodicModeType == CQI_PERIODIC_TYPE_21) )
   {   
       dlUEContext_p->dlCQIInfo.RI = dwlkUEInfo_p->numOfLayer;
   }
   /* - CQI_5.0 */
   /* + TM6_5.2 */
   if(6 == dlUEContext_p->transmissionMode ) 
   {
       dlUEContext_p->dlCQIInfo.RI = 1;
   }
   /* - TM6_5.2 */


    if(dlResp != MAC_SUCCESS)
    {
        resp = dlResp;
    }
    else if(ulResp != MAC_SUCCESS)
    {
        resp = ulResp;
    }
    else 
        resp = MAC_SUCCESS;

    return resp;
}


/****************************************************************************
* Function Name  : deleteMACUEEntity
* Inputs         : idx - Index at which UE Context needs to be deleted.
*                  transactionId - identifier received in request from RRC
* Outputs        : None
* Returns        : ACK on SUCCESS, NACK on FAILURE.
* Variables      :
* Description    : This API takes ueIdx rom RRC and request registered Layer2
*                  modules to delete the UE Context.
*                  It will delete the ueId context from idx table if all 
*                  registered layers returns Success otherwise wait for the
*                  confirmation before freeing the index.
****************************************************************************/

UInt16 deleteMACUEEntity(UInt16 idx, UInt16 transactionId, UInt8 cellIndex)
{
    UInt16 resp = MAC_FAILURE;
    UInt16 ueId = 0;
    RNTIInfo *rntiInfo_p = PNULL; 
   
    if(uplinkContextInfoMap_g[idx] == NULL)
    {
       return MAC_FAILURE;
    }
	LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"%s called for idx %d\n",__func__,idx);
       ueId = uplinkContextInfoMap_g[idx]->crnti;

    deleteMACDLUEEntity(idx, transactionId, TRUE);
    /* SPR 11445 Changes Start */
    deleteMACULUEEntity(idx, transactionId, cellIndex);
    /* SPR 11445 Changes End */
    
    if((PNULL == uplinkContextInfoMap_g[idx]))
    {
        //crntiToIdxTbl_g[(ueId - lowerCRNTIBound_g)%crntiRange_g]
          //  = TBL_SIZE;
         rntiInfo_p = getRntiInfofromRNTIMap(ueId, cellIndex);
         if ( rntiInfo_p == PNULL )
         {
               lteWarning("Error: deleteMACUEEntity: no rnti info present for rnti = %d\n", ueId);
               resp = MAC_SUCCESS;
               return resp;
         }
         rntiInfo_p->state = UE_INACTIVE;
         rntiInfo_p->ueIndex = MAX_UE_SUPPORTED;
         rntiInfo_p->rntiType = NONE;
         memset(rntiInfo_p->ccchSDUArr, 0, MAX_CCCH_SDU);
         memset(rntiInfo_p->aperiodiccqiInfo, 0, sizeof(APeriodicCQI));
         memset(rntiInfo_p->periodiccqiInfo, 0, sizeof(PeriodicCQI));
         /* SPS_CHG */
         UInt16 id = 0;
            if (ueId > endRARNTI_g[cellIndex])  
            {
                UInt32 diff = 0;
                diff = endRARNTI_g[cellIndex] - startRARNTI_g;
                id = ueId - (diff + 1) - cellConfigUeSim_g[cellIndex].lowerCRNTIBound;
            }
            else
            {
                id = ueId - cellConfigUeSim_g[cellIndex].lowerCRNTIBound;
            }
         rntiInfo_p = getRntiInfofromRNTIMap(id + 
                 cellConfigUeSim_g[cellIndex].crntiRange, cellIndex);
         if ( rntiInfo_p == PNULL )
         {
               lteWarning("Error: deleteMACUEEntity: no rnti info present for rnti = %d\n", ueId);
               resp = MAC_SUCCESS;
               return resp;
         }
         rntiInfo_p->state = UE_INACTIVE;
         rntiInfo_p->ueIndex = MAX_UE_SUPPORTED;
         rntiInfo_p->rntiType = NONE;
         /* SPS_CHG */
         resp = MAC_SUCCESS;    
    }
    return resp;
}
/* EICIC +*/

 /****************************************************************************
 * Function Name  : updateCsiMeasurementSubset
 * Inputs         : UInt8 *MeasurementSubset1
 *                  UInt8 *MeasurementSubset2
 * Outputs        : None
 * Returns        : None
 * Description    : This API converts 40bit measurementsubset information received from RRC tlo 40 byte
 *                  and saves it is UPLINK ms context.
 ****************************************************************************/

void updateCsiMeasurementSubset(UInt8 MeasurementSubset1[], UInt8 MeasurementSubset2[],UInt8 internalCellIndex)
{
    UInt8 j_loop  = 0;
    UInt8 numBitsCsiPattern;

#ifdef FDD_CONFIG
    numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellConfigUeSim_g[internalCellIndex].subFrameAssignment)
    {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellConfigUeSim_g[internalCellIndex].subFrameAssignment)
    {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
    }
    else
    {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
    }
#endif

   for (j_loop = 0 ; j_loop < numBitsCsiPattern ; j_loop++)
   {
                                     
         if ((MeasurementSubset2[(j_loop/NUMBER_OF_BITS_IN_1BYTE)] & (0x80 >> (j_loop%NUMBER_OF_BITS_IN_1BYTE) )) == LTE_FALSE)
         {              

                  MeasurementSubset1[j_loop] = LTE_FALSE;
         }
         else
         {
                  MeasurementSubset1[j_loop] = LTE_TRUE;
                                                     
         }
   
  }
      
}
/* EICIC -*/


/****************************************************************************
 * Function Name  : reconfigureMACUEEntity
 * Inputs         : dwlkUEInfo_p - Pointer to MAC DL Info
 *                  uplkUEInfo_p - Pointer to MAC UL Info
 *                  rlcConfigInfo_p - Pointer to RLC Info
 *                  uplkLCCount - Number of LCs needs to be reconfigured
 * Outputs        : None
 * Returns        : ACK on SUCCESS, NACK on FAILURE
 * Variables      :
 * Description    : This API will reconfigure the UE Context at MAC for DL
 *                  and UL.
 ****************************************************************************/

UInt16 reconfigureMACUEEntity(ueContextForDownlink *dwlkUEInfo_p,
                         ueContextForUplink *uplkUEInfo_p, 
                         UInt32 uplkLCCount,
                         UInt8 cellIndex)
{
    UInt32 ulResp = MAC_SUCCESS;
    UInt32 dlResp = MAC_SUCCESS;
    UInt32 i = 0, loop = 0;
    DLUEContext *dlUEContext_p = PNULL;
    ueContextForUplink *ulUEContext_p = PNULL;
    /* + CQI_5.0 */
    UInt8 riIndex = 0;
    /* - CQI_5.0 */
    /* EICIC +*/
    UInt8 absLoop = 0;
    UInt8 numBitsCsiPattern;
    /* EICIC -*/

    UInt32 ueIndex = dwlkUEInfo_p->ueIndex;
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    ulUEContext_p = uplinkContextInfoMap_g[ueIndex];
    if (( PNULL == dlUEContext_p) && ( PNULL == ulUEContext_p) )
    {
        lteWarning("UE does not exist corresponding to this IDx [%d]\n", ueIndex);
        return MAC_FAILURE;
    }
    if ( dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag )
    {
        lteWarning("Pending Delete Flag is set for this UEIndex. %d\n",ueIndex);
        return MAC_FAILURE;

    }
    /* SPR 7963 Fix Start */
    dlUEContext_p->ueCategory = dwlkUEInfo_p->ueCategory;
    ulUEContext_p->ueCategory = uplkUEInfo_p->ueCategory;
    /* SPR 7963 Fix End */
    dlResp = reconfigureMACDLUEEntity(dwlkUEInfo_p,cellIndex);

    if (MAC_LOG_CH_ERROR == dlResp)
    {
        for (i = 0; i < dwlkUEInfo_p->dwlkLogicalChCount; i++)
        {
            if ((MAC_SUCCESS != dwlkUEInfo_p->logicalChConfig[i].result) &&
                    (ADD == dwlkUEInfo_p->logicalChConfig[i].operationType))
            {
                for ( loop = 0; loop <uplkUEInfo_p->numLogicalChannels; loop++)
                {
                    if((uplkUEInfo_p->lcInfo[loop].lchId ==
                                dwlkUEInfo_p->logicalChConfig[i].lchId) &&
                            (ADD == uplkUEInfo_p->lcInfo[loop].operationType))
                    {
                        uplkUEInfo_p->lcInfo[loop].result = 
                            dwlkUEInfo_p->logicalChConfig[i].result;
                    }
                }
            }
        }    
    }
    /* 4x4 DL MIMO CHG START */
    dlUEContext_p->ueCategory = dwlkUEInfo_p->ueCategory; 
    ulUEContext_p->ueCategory = uplkUEInfo_p->ueCategory;
    /* 4x4 DL MIMO CHG END */
    ulResp = reconfigureMACULUEEntity( dwlkUEInfo_p->ueIndex, 
            uplkUEInfo_p,
            uplkLCCount,
            uplkUEInfo_p->lcInfo,
            cellIndex);

    if (MAC_LOG_CH_ERROR == dlResp)
    {
        for (i = 0; i < uplkUEInfo_p->numLogicalChannels; i++)
        {
            if ((MAC_SUCCESS != uplkUEInfo_p->lcInfo[i].result) &&
                    (ADD == uplkUEInfo_p->lcInfo[i].operationType))
            {
                for ( loop = 0; loop < dwlkUEInfo_p->dwlkLogicalChCount; loop++)
                {
                    if((uplkUEInfo_p->lcInfo[i].lchId ==
                                dwlkUEInfo_p->logicalChConfig[loop].lchId) &&
                            (ADD == dwlkUEInfo_p->logicalChConfig[loop].operationType))
                    {
                        deleteDLUELogicalChannel(dlUEContext_p,dwlkUEInfo_p->logicalChConfig[loop].lchId);   
                    }
                }
            }
        }
    }

    /* + MEAS_GAP_CHG */
    if( TRUE == dwlkUEInfo_p->measurementGapRequestType )
    {
        processMeasGapReConfig(dwlkUEInfo_p ,dlUEContext_p);
                                                                  
    }
    /* - MEAS_GAP_CHG */
    /* + CQI_4.1 */
    /* + CQI_4.1 */
    /*Fix for SPR 12087 start*/
    if(dwlkUEInfo_p->cqiInfoFlag)
    {
        dlUEContext_p->dlCQIInfo.lastWideband = 0;
        dlUEContext_p->dlCQIInfo.tempRecvSfnSf = 0;
        dlUEContext_p->dlCQIInfo.bandwidthPartCount = 0;
        dlUEContext_p->dlCQIInfo.nextSubbandOccurrence = 0;
        dlUEContext_p->dlCQIInfo.nextWidebandOccurrence = 0;
        /* Bandwidth Fix */
        dlUEContext_p->dlCQIInfo.calculateBandwidthPartFlag = FALSE;
        /* Bandwidth Fix */
        /* EICIC +*/
        dlUEContext_p->dlCQIInfo.bandwidthPartCount_2 = 0;
        dlUEContext_p->dlCQIInfo.nextSubbandOccurrence_2 = 0;
        dlUEContext_p->dlCQIInfo.nextWidebandOccurrence_2 = 0;
        /* EICIC -*/
        /* + SPR_8414 */
        if (INVALID_REQUEST_TYPE == uplkUEInfo_p->cqiRequestType)
        {
            uplkUEInfo_p->periodicModeType = 
                dlUEContext_p->dlCQIInfo.periodicModeType;
            if(0 != uplkUEInfo_p->periodicModeType)
            { 
                uplkUEInfo_p->cqiformatIndicatorPeriodic =
                    dlUEContext_p->dlCQIInfo.cqiformatIndicatorPeriodic;
            }
        }
        /* - SPR_8414 */
        /* - CQI_4.1 */
       if(dwlkUEInfo_p->transmissionMode)
       {
           dlUEContext_p->transmissionMode =  dwlkUEInfo_p->transmissionMode;
       }
         /* + TM7_8 Changes Start */
        if(8 == dlUEContext_p->transmissionMode)
        {
            dlUEContext_p->isPmiRiConfigured_V920 = dwlkUEInfo_p->pmiRiReportV920;
        }
        /* - TM7_8 Changes End */
        if ( (uplkUEInfo_p->cqiformatIndicatorPeriodic == 0) && 
                ((1 == dwlkUEInfo_p->transmissionMode) ||
                 (2 == dwlkUEInfo_p->transmissionMode) ||
                 (3 == dwlkUEInfo_p->transmissionMode) ||      
                  /* + TM7_8 Changes Start */
                 (7 == dwlkUEInfo_p->transmissionMode) ||
                 ((8 == dwlkUEInfo_p->transmissionMode) &&
                 (TRUE != dlUEContext_p->isPmiRiConfigured_V920))))      
                  /* - TM7_8 Changes End */
        {
            dlUEContext_p->dlCQIInfo.periodicModeType = CQI_PERIODIC_TYPE_10;
            dlUEContext_p->dlCQIInfo.periodicCQIMode10_p = &cqiPeriodicReportMode10_g[cellIndex][0];
            dlUEContext_p->dlCQIInfo.periodicCQIMode10Abs_p = &cqiPeriodicReportMode10_g_2[cellIndex][0]; /* EICIC +- */
            uplkUEInfo_p->periodicModeType = CQI_PERIODIC_TYPE_10;
        }
        else if ( (uplkUEInfo_p->cqiformatIndicatorPeriodic == 0) &&
                ((4 == dwlkUEInfo_p->transmissionMode) ||
                /* + TM6_5.2 */
                (6 == dwlkUEInfo_p->transmissionMode)||         
                /* - TM6_5.2 */
                /* + TM7_8 Changes Start */
                ((8 == dwlkUEInfo_p->transmissionMode) &&         
                (TRUE == dlUEContext_p->isPmiRiConfigured_V920))))
                /* - TM7_8 Changes End */
        {
            dlUEContext_p->dlCQIInfo.periodicModeType = CQI_PERIODIC_TYPE_11;
            dlUEContext_p->dlCQIInfo.periodicCQIMode11_p = &cqiPeriodicReportMode11_g[cellIndex][0];
            dlUEContext_p->dlCQIInfo.periodicCQIMode11Abs_p = &cqiPeriodicReportMode11_g_2[cellIndex][0]; /* EICIC +- */
            uplkUEInfo_p->periodicModeType = CQI_PERIODIC_TYPE_11;
        }
        else if ( (uplkUEInfo_p->cqiformatIndicatorPeriodic == 1) && 
                ((1 == dwlkUEInfo_p->transmissionMode) ||
                 (2 == dwlkUEInfo_p->transmissionMode) ||
                 (3 == dwlkUEInfo_p->transmissionMode) ||        
                  /* + TM7_8 Changes Start */
                 (7 == dwlkUEInfo_p->transmissionMode) ||      
                 ((8 == dwlkUEInfo_p->transmissionMode) &&
                 (TRUE != dlUEContext_p->isPmiRiConfigured_V920))))      
                  /* - TM7_8 Changes End */
        {
            dlUEContext_p->dlCQIInfo.periodicModeType = CQI_PERIODIC_TYPE_20;
            dlUEContext_p->dlCQIInfo.periodicCQIMode20_p = &cqiPeriodicReportMode20_g[0];
             dlUEContext_p->dlCQIInfo.periodicCQIMode20Abs_p = &cqiPeriodicReportMode20_g_2[0]; /* EICIC +-*/
            /* Bandwidth Fix */
            dlUEContext_p->dlCQIInfo.calculateBandwidthPartFlag = TRUE;
            /* Bandwidth Fix */
            uplkUEInfo_p->periodicModeType = CQI_PERIODIC_TYPE_20;
        }   
        else if ( (uplkUEInfo_p->cqiformatIndicatorPeriodic == 1) &&
                ((4 == dwlkUEInfo_p->transmissionMode) ||
                /* + TM6_5.2 */
                (6 == dwlkUEInfo_p->transmissionMode)|| 
                /* - TM6_5.2 */
                /* + TM7_8 Changes Start */
                ((8 == dwlkUEInfo_p->transmissionMode)&&
                (TRUE == dlUEContext_p->isPmiRiConfigured_V920))))
                /* - TM7_8 Changes End */
        {
            dlUEContext_p->dlCQIInfo.periodicModeType = CQI_PERIODIC_TYPE_21;
            dlUEContext_p->dlCQIInfo.periodicCQIMode21_p = &cqiPeriodicReportMode21_g[0];
            dlUEContext_p->dlCQIInfo.periodicCQIMode21Abs_p = &cqiPeriodicReportMode21_g_2[0]; /* EICIC +-*/
            /* Bandwidth Fix */
            dlUEContext_p->dlCQIInfo.calculateBandwidthPartFlag = TRUE;
            /* Bandwidth Fix */
            uplkUEInfo_p->periodicModeType = CQI_PERIODIC_TYPE_21;
        }
        if(0 != dwlkUEInfo_p->cqiMode)
        {    
            dlUEContext_p->dlCQIInfo.aperiodicModeType = dwlkUEInfo_p->cqiMode; 
        }

        if (CQI_MODE_1_2 == dlUEContext_p->dlCQIInfo.aperiodicModeType)  
        {
            dlUEContext_p->dlCQIInfo.aperiodicCQIMode12_p = &cqiAperiodicReportMode12_g[cellIndex][0];
		    /* SPR 14062 fix start */
            dlUEContext_p->dlCQIInfo.RI = dwlkUEInfo_p->numOfLayer;
		    /* SPR 14062 fix end */
        }    
        else if (CQI_MODE_2_0 == dlUEContext_p->dlCQIInfo.aperiodicModeType) 
        {
            dlUEContext_p->dlCQIInfo.aperiodicCQIMode20_p = &cqiAperiodicReportMode20_g[cellIndex][0];
		    /* SPR 14062 fix start */
            dlUEContext_p->dlCQIInfo.RI = dwlkUEInfo_p->numOfLayer;
		    /* SPR 14062 fix end */
        }    
        else if (CQI_MODE_2_2 == dlUEContext_p->dlCQIInfo.aperiodicModeType)  
        {
            dlUEContext_p->dlCQIInfo.aperiodicCQIMode22_p = &cqiAperiodicReportMode22_g[cellIndex][0];
		    /* SPR 14062 fix start */
            dlUEContext_p->dlCQIInfo.RI = dwlkUEInfo_p->numOfLayer;
		    /* SPR 14062 fix end */
        }    
        else if (CQI_MODE_3_0 == dlUEContext_p->dlCQIInfo.aperiodicModeType)  
        {
            dlUEContext_p->dlCQIInfo.aperiodicCQIMode30_p = &cqiAperiodicReportMode30_g[cellIndex][0];
		    /* SPR 14062 fix start */
            dlUEContext_p->dlCQIInfo.RI = dwlkUEInfo_p->numOfLayer;
		    /* SPR 14062 fix end */
        }    
        else if (CQI_MODE_3_1 == dlUEContext_p->dlCQIInfo.aperiodicModeType)  
        {
            dlUEContext_p->dlCQIInfo.aperiodicCQIMode31_p = &cqiAperiodicReportMode31_g[cellIndex][0];
		    /* SPR 14062 fix start */
            dlUEContext_p->dlCQIInfo.RI = dwlkUEInfo_p->numOfLayer;
		    /* SPR 14062 fix end */
        }    
        /* SPR 14223 Fix : Start */
        //dlUEContext_p->dlCQIInfo.ueCqiProfileNum = \
             // ueProfileInfo_g[cellIndex][(ueIndex%MAX_UE_PROFILES)].dlCqiProfileId;
             /* SPR 12298 Fix Start */
        /*ueProfileInfo_g[cellIndex][ueProfileIndex_g[cellIndex]%MAX_UE_PROFILES].dlCqiProfileId;*/
        //ueProfileIndex_g[cellIndex]++;
        /* SPR 14223 Fix : End */
             /* SPR 12298 Fix End */
        /* - CQI_4.1 */

       /* + CQI_5.0 */
       /* + SPR 13126 Changes */ 
       dlUEContext_p->dlCQIInfo.riValue_p = &RIValues_g[cellIndex][0]; 
  		dlUEContext_p->dlCQIInfo.riValueAbs_p = &RIValues_g_2[cellIndex][0];         /* EICIC +-*/
       /* - SPR 13126 Changes */ 
       dlUEContext_p->dlCQIInfo.isULSCHPDUType[0] = 0;
       /* + SPR_11077 */
       if(( uplkUEInfo_p->periodicModeType == CQI_PERIODIC_TYPE_10) ||  
           ( uplkUEInfo_p->periodicModeType == CQI_PERIODIC_TYPE_11) || 
           ( uplkUEInfo_p->periodicModeType == CQI_PERIODIC_TYPE_20) || 
           ( uplkUEInfo_p->periodicModeType == CQI_PERIODIC_TYPE_21) ) 
       /* - SPR_11077 */
       {   
           dlUEContext_p->dlCQIInfo.RI = dwlkUEInfo_p->numOfLayer;
       } 
       /* - CQI_5.0 */
       /* + TM6_5.2 */
       if((6 == dlUEContext_p->transmissionMode ) &&
          (FALSE == dlUEContext_p->dlCQIInfo.cqiInfoFlagInCreate))
       {
           dlUEContext_p->dlCQIInfo.RI = 1;
       }
    }
    /*Fix for SPR 12087 end*/
    /*EICIC +*/
    ulUEContext_p->riConfigIndex_2 = uplkUEInfo_p->riConfigIndex_2 ;
    ulUEContext_p->cqiPMIConfigIndex_2 =  uplkUEInfo_p->cqiPMIConfigIndex_2 ;


#ifdef FDD_CONFIG
    numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellConfigUeSim_g[cellIndex].subFrameAssignment)
    {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellConfigUeSim_g[cellIndex].subFrameAssignment)
    {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
    }
    else
    {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
    }
#endif


    if (SETUP >= uplkUEInfo_p->cqiSubsetConfigRequestType)
	{
		for (absLoop = 0; absLoop < numBitsCsiPattern ; absLoop++)
		{	
		    ulUEContext_p->csiMeasurementSubset1[absLoop] = uplkUEInfo_p->csiMeasurementSubset1[absLoop];
		    ulUEContext_p->csiMeasurementSubset2[absLoop] = uplkUEInfo_p->csiMeasurementSubset2[absLoop];
		}
    }
    /* EICIC -*/
    	
    if(MAC_SUCCESS != dlResp)
    {
        LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,"[%s] Exit. ",__func__);
        return dlResp;
    }
    else if (MAC_SUCCESS != ulResp)
    {
        LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,"[%s] Exit. ",__func__);
        return ulResp;
    }

     return ulResp;
}



/****************************************************************************
 * Function Name  : deleteDLUELogicalChannel
 * Inputs         : ueContext_p - Pointer to UE context from which LC
 *                           needs to be deleted.
 *                  ueLCId - The Logical Channel ID that needs to be deleted.
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE,MAC_SUCCESS]
 * Description    : This API is used to delete a logical channel as per the
 *                  request for a given UE.
 *****************************************************************************/

MacRetType deleteDLUELogicalChannel( DLUEContext *ueContext_p,
                                                   UInt8 ueLCId
                                                 )
{
    LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,"[%s] Entry. ",__func__);

    DLLogicalChannelInfo *dlLogicalChInfo_p     = PNULL;
    SInt32 index = 0;
    dlLogicalChInfo_p = &ueContext_p->logicalChannel[ueLCId];
    //check if max LC ID is equal to delete LC ID
    if ( dlLogicalChInfo_p->logicalChannelId == ueContext_p->maxLcId)
    {
        index = ueContext_p->maxLcId - 1;
        /*identify the next max. Lc Id
          after deleting this LCID. */
        while ( INVALID_LCID == ueContext_p->
                logicalChannel[index].logicalChannelId && index >= 0 )
        {
            index--;
        }
        if ( index < 0 )
        {
            ueContext_p->maxLcId = INVALID_LCID;
        }
        else
        {
            ueContext_p->maxLcId = index;
        } 
    }
    
    dlLogicalChInfo_p->logicalChannelId  = INVALID_LCID;
    dlLogicalChInfo_p->queueLoad         = 0;
    dlLogicalChInfo_p->lastScheduledTick = 0;
    dlLogicalChInfo_p->rlcMode           = INVALID_RLC_MODE;
    dlLogicalChInfo_p->rlcSNFieldLength  = 0;
    dlLogicalChInfo_p->logicalChannelPriority = 0;
    LTE_LOG(LOG_DETAIL,macModuleLogDetail_p,"[%s] Exit. ",__func__);
    return MAC_SUCCESS;
}


/* +  MEAS_GAP_CHG */

/*****************************************************************************
 *  Function Name  : processMeasGapReConfig
 *  Inputs         : dwlkUEInfo_p - pointer to the ueContextForDownlink
 *                       dlUEcontext_p - pointer to the DLUEContext
 *
 *  Outputs        : 
 *  Returns        : void
 *  Description    : This function reconfigure the paramters of the Measurement
 *                    gap based upon the config type
 * 
 ********************************************************************************/
inline  void  processMeasGapReConfig( ueContextForDownlink *dwlkUEInfo_p,
                                              DLUEContext *dlUEContext_p )
{
    if ( SETUP ==   dwlkUEInfo_p->measGapConfigType)
    {
       /* Whether config type is SETUP or RELEASE same function is called. 
        * During SETUP,paramters are reintialized/recalculated and
        * during RELEASE,fresh copy is assiagned/calcualted 
        */    
        processMeasGapParam(dwlkUEInfo_p , dlUEContext_p );
        dlUEContext_p->measGapConfigType = dwlkUEInfo_p->measGapConfigType;

    }
    else
    {
        if ( SETUP == dlUEContext_p->measGapConfigType )
        {
            dlUEContext_p->measGapConfigType = dwlkUEInfo_p->measGapConfigType ;
        }
    }
}

/*****************************************************************************
 *  Function Name  : processMeasGapParam
 *  Inputs         : ueDlConfigInfo_p - pointer to the ueContextForDownlink
 *                       newUEcontext_p - pointer to the DLUEContext
 *
 *  Outputs        :
 *  Returns        : void
 *  Description    : This function copies the configurable paramaters and  
 *                   calculate some paramaters that will be used during 
 *                   validation.
 *
 ********************************************************************************/
inline void processMeasGapParam( ueContextForDownlink *ueDlConfigInfo_p ,
                                       DLUEContext *newUEcontext_p)                       
{     
     newUEcontext_p->measGapContext.measurementGapConfig =
                                              ueDlConfigInfo_p->measGapConfig;
   	/*Meas_Gap_Changes_Start*/ 
     if( GP0 == 
             newUEcontext_p->measGapContext.measurementGapConfig.gapPatternId 
             || GP7_R15 == newUEcontext_p->measGapContext.measurementGapConfig.gapPatternId)
     {
         newUEcontext_p->
         measGapContext.measGapStaticParam.measurementGapPeriodicity = 4;
     }
     else if( GP1 == 
              newUEcontext_p->measGapContext.measurementGapConfig.gapPatternId
             || GP8_R15 == newUEcontext_p->measGapContext.measurementGapConfig.gapPatternId)
     {
         newUEcontext_p->
         measGapContext.measGapStaticParam.measurementGapPeriodicity  = 8 ;
     }
     else if( GP4_R15 == 
              newUEcontext_p->measGapContext.measurementGapConfig.gapPatternId 
            ||GP6_R15 == newUEcontext_p->measGapContext.measurementGapConfig.gapPatternId
            ||GP10_R15 == newUEcontext_p->measGapContext.measurementGapConfig.gapPatternId)
     {
         newUEcontext_p->
         measGapContext.measGapStaticParam.measurementGapPeriodicity  = 2 ;
     }
	 else if( GP5_R15 == 
              newUEcontext_p->measGapContext.measurementGapConfig.gapPatternId 
            ||GP9_R15 == newUEcontext_p->measGapContext.measurementGapConfig.gapPatternId
            ||GP11_R15 == newUEcontext_p->measGapContext.measurementGapConfig.gapPatternId)
     {
         newUEcontext_p->
         measGapContext.measGapStaticParam.measurementGapPeriodicity  = 16 ;
     }
     /*Meas_Gap_Changes_End*/

     newUEcontext_p->measGapContext.measGapStaticParam.flooredGapOffset =
               floor ( newUEcontext_p->
                          measGapContext.measurementGapConfig.gapOffset / 10 );

     newUEcontext_p->measGapContext.measGapStaticParam.modGapOffset =
         ( newUEcontext_p->measGapContext.measurementGapConfig.gapOffset % 10 );

}

/* -  MEAS_GAP_CHG */


/****************************************************************************
 * Function Name  : reconfigureTpcPdcchConfigInfoForPucch
 * Inputs         : rnti - indicates the tpc rnti configured for UE
 *                  tpcBitMapIndex - indicated the tpc index configured for UE.
 *                  dciFormat - indicates whether tpc rnti is configured with
 *                  DCI FORMAT 3/3A.
 *                  release - Indicates whether tpc info needs to be configured
 *                  released.
 * Output         : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE in Success or failure Scenario
 * Description    : This API updates updates the tpc info in ue context and
 *                  tpc rnti corresponding to the tpc rnti configured for the
 *                  UE.
 ******************************************************************************/
static MacRetType reconfigureTpcPdcchConfigInfoForPucch(
        UInt8 release,
        UInt16 rnti,
        UInt8 tpcBitMapIndex,
        UInt8 dciFormat,
        DLUEContext *ueContext_p)
{
    MacRetType retVal = MAC_SUCCESS;
    UInt8 idx = 0;

    if ( release )
    {
        if (ueContext_p->tpcPdcchConfigPucch.tpcRnti == 0)
        {
            return MAC_SUCCESS;
        }
        TpcInfo tpcInfo = tpcRntiInfoMap[(ueContext_p->tpcPdcchConfigPucch.tpcRnti)- START_TPC_RNTI]; 
        tpcInfo.configuredCount--;

        for(idx = 0; idx< 16; idx++)
        {
            if(tpcInfo.ueIndex[idx] == ueContext_p->ueIndex)
            {
                tpcInfo.isValid[idx] = FALSE;
                break;
            }
        }
        ueContext_p->tpcPdcchConfigPucch.tpcRnti = 0;
        retVal = MAC_SUCCESS;
    }
    else if (ueContext_p->tpcPdcchConfigPucch.tpcRnti == 0)
    {
        ueContext_p->tpcPdcchConfigPucch.tpcRnti = rnti;
        ueContext_p->tpcPdcchConfigPucch.tpcBitMapIndex = tpcBitMapIndex;

        TpcInfo reconfigTpcInfo = tpcRntiInfoMap[(rnti) - START_TPC_RNTI];
        if(reconfigTpcInfo.isValid[tpcBitMapIndex] == FALSE)
        {
            reconfigTpcInfo.ueIndex[tpcBitMapIndex] = ueContext_p->ueIndex;
            reconfigTpcInfo.isValid[tpcBitMapIndex] = TRUE;
            reconfigTpcInfo.configuredCount++;
        }
        else
        {
            return MAC_FAILURE;
        }
        retVal = MAC_SUCCESS;
    }
    else if((ueContext_p->tpcPdcchConfigPucch.tpcRnti != rnti) ||
            (ueContext_p->tpcPdcchConfigPucch.tpcBitMapIndex != tpcBitMapIndex))
    {
        return MAC_FAILURE;
    }
    return retVal;
}

