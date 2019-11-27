/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2OAM.c,v 1.1.1.1 2010/02/11 04:51:22 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file includes the Management APIs required to 
 *                     determine the current status of the MAC system such as
 *                     the UE Statistics, the Logical Channel Statistics,
 *                     system build configuration, setting the debug/trace
 *                     levels.
 *                     
 *                     This file basically contains the OAM APIs for the MAC
 *                     system.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLayer2OAM.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3  2009/07/29 15:18:38  gur20439
 * look and feel to simulator fix.
 *
 * Revision 1.2  2009/07/14 14:48:54  gur20439
 * rrc interface code change.
 *
 * Revision 1.1  2009/03/30 10:20:41  gur19836
 * Initial Version
 *
 * Revision 1.25  2009/03/13 08:00:08  gur19836
 * Modified for OAM and RLC Interface Changes
 *
 * Revision 1.24  2009/03/02 11:01:13  gur18569
 * removed funcNamp_p and replaced it with __func__
 *
 * Revision 1.23  2009/02/24 04:57:01  gur18569
 * added code for initializing sm thread only whn stats are enabled for the first time
 *
 * Revision 1.22  2009/02/20 14:25:52  gur18569
 * dl through put changes
 *
 * Revision 1.21  2009/02/17 06:57:41  gur18569
 * removed unnecessary printf
 *
 * Revision 1.20  2009/02/17 06:49:59  gur18569
 * starting stats manager whn stats are enabled for the first time
 *
 * Revision 1.19  2009/01/31 18:45:04  gur18569
 * added buildStamp logs
 *
 * Revision 1.18  2009/01/29 11:59:59  gur19836
 * LTE_LOG replaced with printf in getMACUELogicalChannelStats
 *
 * Revision 1.17  2009/01/29 11:48:14  gur03939
 * Code changed for Uplink and Downlink throughput
 *
 * Revision 1.16  2009/01/27 16:49:41  gur18569
 * removed warnings
 *
 * Revision 1.15  2009/01/27 13:57:23  gur18569
 * changed stats print from LTE_LOG to printf
 *
 * Revision 1.14  2009/01/27 12:19:51  gur19413
 * throughput function added
 *
 * Revision 1.13  2009/01/25 11:50:57  gur20052
 * Added during Regression Testing
 *
 * Revision 1.12  2009/01/22 11:19:16  gur19836
 * Option added for enabling/disabling stats for UL Scheduler
 *
 * Revision 1.11  2009/01/20 07:47:00  gur19836
 * Changes done for interface with RRC Layer
 *
 * Revision 1.10  2009/01/12 09:43:30  gur19836
 * numLogicalChannels variable added in UL UE Context
 *
 * Revision 1.9  2009/01/08 09:06:30  gur19836
 * UT Bug Fixed
 *
 * Revision 1.8  2008/12/05 12:27:03  gur18569
 * multicore changes
 *
 * Revision 1.7  2008/10/22 12:34:07  gur11912
 * fixed for performance changes
 *
 * Revision 1.6  2008/10/20 08:31:36  gur18569
 * changes in enableStats()
 *
 * Revision 1.5  2008/10/20 06:30:50  gur18569
 * added enableStats()
 *
 * Revision 1.4  2008/10/17 09:24:00  gur19836
 * Enhancement: cleanupMACLayer functionality added
 *
 * Revision 1.3  2008/10/14 06:18:58  gur11912
 * Log Level added
 *
 * Revision 1.2  2008/09/19 07:13:36  gur11912
 * review comments incorporated
 *
 * Revision 1.1  2008/09/09 05:12:53  gur11974
 * Initial Version
 *
 *
 ****************************************************************************/
#if 0

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2OAM.h"
#include "lteLayer2StatsManager.h"
#include "lteLayer2Tick.h"
#include "lteLayer2ParseUtil.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/



/****************************************************************************
 Private Types
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
extern ueContextForUplink* uplinkContextInfoMap_g[TBL_SIZE];
extern dynamicUEContext *dynUEContextInfoMap_g[TBL_SIZE];


extern UInt32 ulThroughPutEnable_g;
extern UInt32 dlThroughPutEnable_g;
extern UInt32 dlStartThroughputTick_g;
extern UInt32 ulStartThroughputTick_g;
extern UInt32 totalUplinkBytesSend_g;
extern UInt32 totalDlBytesSend_g[MAX_NB_ENCODER_THREADS];
UInt64  SMThID_g = 0;

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
#if 0
static UInt8 smRunForFirstTime = 1;


/****************************************************************************
 * Function Name  : fillUeStatus
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Variables      : 
 * Description    : This API is used to fill the response buffer with the 
 *                  status of UE with their logical channels. If ueIndex_p is 
 *                  not NULL, then fill the buffer for that particular UE else
 *                  send the response status of all active UE.
 ****************************************************************************/
UInt16 fillUeStatus(UInt8 *cnfBuff_p, UInt16 *current_p, UInt16 *ueIndex_p)
{
    UInt16 numActiveUE = 0,count = 0, loopIdx = 0;
    DLUEContext *currentUEContext_p = PNULL;
    dynamicUEContext *uplkDynUEContext_p = PNULL;
    ueContextForUplink *uplkUEContext_p = PNULL;
    UInt32 bsrNet = 0;
    UInt16 tagLen = 0;
    UInt16 index = 0, lenIdx = 0, current = *current_p;

    numActiveUE = rrcConfig_g.totalActiveUE_g;
    printf("Total Active UE in the system.%u \n",numActiveUE);

    printf("Current Global Tick %u \n",globalTTITickCount_g);

    if(0 == numActiveUE) {
        return MAC_NO_ACTIVE_UE_IN_SYSTEM;
    }

    /* Set the loop either for all active UEs or one UE whose index
       is passed as an argument - *ueIndex_p */
    if(ueIndex_p) {
        count = *ueIndex_p;
        loopIdx = *ueIndex_p;
    } else {
        count = 0;
        loopIdx = TBL_SIZE - 1;
    }   

    for(; count <= loopIdx; count++)
    {
        currentUEContext_p = ueDLContextArr_g[count];
        uplkUEContext_p = uplinkContextInfoMap_g[count];
        uplkDynUEContext_p = dynUEContextInfoMap_g[count];
        if((PNULL == currentUEContext_p) || (PNULL == uplkUEContext_p) ||
                                         (PNULL == uplkDynUEContext_p)) {
            if(ueIndex_p) {
                return MAC_UE_NOT_EXISTS;
            }    
            continue;
        }
        tagLen = 0;

        cnfBuff_p[current++] = MAC_UE_STATUS & 0xFF;
        cnfBuff_p[current++] = (MAC_UE_STATUS >> 8) & 0xFF;
        /* Fill the length at the end of buffer encoding */
        lenIdx = current;
        current += 2;
        /* Fill Downlink Status Info */
        cnfBuff_p[current++] = currentUEContext_p->ueId & 0xFF;
        cnfBuff_p[current++] = (currentUEContext_p->ueId >> 8) & 0xFF;
        cnfBuff_p[current++] = currentUEContext_p->uePriority & 0xFF;
        cnfBuff_p[current++] = currentUEContext_p->numLogicalChannels & 0xFF;
        cnfBuff_p[current++] = currentUEContext_p->maxNbOfRBs & 0xFF;
        cnfBuff_p[current++] = currentUEContext_p->modulationScheme & 0xFF;
        cnfBuff_p[current++] = currentUEContext_p->codingRate & 0xFF;
        cnfBuff_p[current++] = (currentUEContext_p->codingRate >> 8) & 0xFF;
        cnfBuff_p[current++] = (currentUEContext_p->codingRate >> 16) & 0xFF;
        cnfBuff_p[current++] = (currentUEContext_p->codingRate >> 24) & 0xFF;
        cnfBuff_p[current++] = currentUEContext_p->mcsIndex & 0xFF;
        cnfBuff_p[current++] = currentUEContext_p->ueQueueLoad & 0xFF;
        cnfBuff_p[current++] = (currentUEContext_p->ueQueueLoad >> 8) & 0xFF;
        cnfBuff_p[current++] = (currentUEContext_p->ueQueueLoad >> 16) & 0xFF;
        cnfBuff_p[current++] = (currentUEContext_p->ueQueueLoad >> 24) & 0xFF;
        
        cnfBuff_p[current++] = currentUEContext_p->lastScheduledTick & 0xFF;
        cnfBuff_p[current++] = (currentUEContext_p->lastScheduledTick >> 8) & 0xFF;
        cnfBuff_p[current++] = (currentUEContext_p->lastScheduledTick >> 16) & 0xFF;
        cnfBuff_p[current++] = (currentUEContext_p->lastScheduledTick >> 24) & 0xFF;
        
        /* Fill Uplink Status Info */
        cnfBuff_p[current++] = uplkUEContext_p->numLogicalChannels & 0xFF;
        cnfBuff_p[current++] = uplkUEContext_p->maxNbOfRBs & 0xFF;
        cnfBuff_p[current++] = uplkDynUEContext_p->bsrNet[0] & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->bsrNet[0] >> 8) & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->bsrNet[0] >> 16) & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->bsrNet[0] >> 24) & 0xFF;
        cnfBuff_p[current++] = uplkDynUEContext_p->bsrNet[1] & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->bsrNet[1] >> 8) & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->bsrNet[1] >> 16) & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->bsrNet[1] >> 24) & 0xFF;
        cnfBuff_p[current++] = uplkDynUEContext_p->bsrNet[2] & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->bsrNet[2] >> 8) & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->bsrNet[2] >> 16) & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->bsrNet[2] >> 24) & 0xFF;
        cnfBuff_p[current++] = uplkDynUEContext_p->bsrNet[3] & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->bsrNet[3] >> 8) & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->bsrNet[3] >> 16) & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->bsrNet[3] >> 24) & 0xFF;
        //cnfBuff_p[current++] = uplkUEContext_p->modulation & 0xFF;
        cnfBuff_p[current++] = uplkUEContext_p->codingrate & 0xFF;
        cnfBuff_p[current++] = (uplkUEContext_p->codingrate >> 8) & 0xFF;
        cnfBuff_p[current++] = (uplkUEContext_p->codingrate >> 16) & 0xFF;
        cnfBuff_p[current++] = (uplkUEContext_p->codingrate >> 24) & 0xFF;
        
        cnfBuff_p[current++] = uplkUEContext_p->mcsIndex & 0xFF;
        cnfBuff_p[current++] = uplkDynUEContext_p->lastScheduledTick & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->lastScheduledTick >> 8) & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->lastScheduledTick >> 16) & 0xFF;
        cnfBuff_p[current++] = (uplkDynUEContext_p->lastScheduledTick >> 24) & 0xFF;

        tagLen += MAC_UE_STATUS_LENGTH;

        /* Fill DL_LC_INFO */
        for(index=0;index < MAX_NUMBER_OF_LOGICAL_CHANNEL;index++)
        {
            if (currentUEContext_p->logicalChannel[index].logicalChannelId > 0)
            {
                cnfBuff_p[current++] = MAC_DL_LOG_CH_STATUS & 0xFF;
                cnfBuff_p[current++] = (MAC_DL_LOG_CH_STATUS >> 8) & 0xFF;
                cnfBuff_p[current++] = MAC_DL_LOG_CH_STATUS_LENGTH & 0xFF;
                cnfBuff_p[current++] = (MAC_DL_LOG_CH_STATUS_LENGTH >> 8) & 0xFF;

                cnfBuff_p[current++] = 
                    currentUEContext_p->logicalChannel[index].logicalChannelId & 0xFF;
                cnfBuff_p[current++] =
                    currentUEContext_p->logicalChannel[index].logicalChannelPriority & 0xFF;
                cnfBuff_p[current++] = 
                    currentUEContext_p->logicalChannel[index].queueLoad & 0xFF;
                cnfBuff_p[current++] = 
                    (currentUEContext_p->logicalChannel[index].queueLoad >> 8) & 0xFF;
                cnfBuff_p[current++] = 
                    (currentUEContext_p->logicalChannel[index].queueLoad >> 16) & 0xFF;
                cnfBuff_p[current++] = 
                    (currentUEContext_p->logicalChannel[index].queueLoad >> 24) & 0xFF;
                tagLen += MAC_DL_LOG_CH_STATUS_LENGTH;    
            }
        }

        /* Fill UL_LC_INFO */
        for(index=0;index < MAX_NUMBER_OF_LOGICAL_CHANNEL;index++)
        {
            if (uplkUEContext_p->lcInfo[index].lcGId != LCGID_INVALID_VALUE)
            {
                cnfBuff_p[current++] = MAC_UL_LOG_CH_STATUS & 0xFF;
                cnfBuff_p[current++] = (MAC_UL_LOG_CH_STATUS >> 8) & 0xFF;
                cnfBuff_p[current++] = MAC_UL_LOG_CH_STATUS_LENGTH & 0xFF;
                cnfBuff_p[current++] = (MAC_UL_LOG_CH_STATUS_LENGTH >> 8) & 0xFF;

                cnfBuff_p[current++] = 
                    uplkUEContext_p->lcInfo[index].lchId & 0xFF;
                cnfBuff_p[current++] =
                    uplkUEContext_p->lcInfo[index].lcGId & 0xFF;
                bsrNet = uplkDynUEContext_p->bsrNet[uplkUEContext_p->lcInfo[index].lcGId];    
                cnfBuff_p[current++] = bsrNet & 0xFF;
                cnfBuff_p[current++] = (bsrNet >> 8) & 0xFF; 
                cnfBuff_p[current++] = (bsrNet >> 16) & 0xFF; 
                cnfBuff_p[current++] = (bsrNet >> 24) & 0xFF; 
                tagLen += MAC_UL_LOG_CH_STATUS_LENGTH;    
            }
        }
        cnfBuff_p[lenIdx] = tagLen & 0xFF;
        cnfBuff_p[lenIdx + 1] = (tagLen >> 8) & 0xFF;
    }

    *current_p = current;
    return MAC_SUCCESS;
}


/****************************************************************************
 * Function Name  : getMACUEStats
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Variables      : 
 * Description    : Prints the UE id and logical channel info for 
 *                  all active UE.
 ****************************************************************************/
void getMACUEStats(void)
{
    UInt16 numActiveUE = 0,count = 0;
    DLUEContext *currentUEContext_p = PNULL;
    dynamicUEContext *uplkDynUEContext_p = PNULL;
    ueContextForUplink *uplkUEContext_p = PNULL;

    numActiveUE = rrcConfig_g.totalActiveUE_g;
    printf("Total Active UE in the system.%u \n",numActiveUE);

    printf("Current Global Tick %u \n",globalTTITickCount_g);
    printf ("***** Downlink UE Context Information *****\n");

    printf (
            "UE ID | PRIORITY | NumOfLcCh | maxNbOfRB| CurrentBitRate | ModulationScheme | CodingRate | MCS Index | QueueLoad | LastScheduledTick\n");

    for(count=0; count < TBL_SIZE; count++)
    {
        currentUEContext_p = ueDLContextArr_g[count];
        if(PNULL != currentUEContext_p)
        {
            printf (
                    "%5u   %8u  %10u   %8u   %12u   %12u      %10u   %10u   %8u  %12u\n",
                    currentUEContext_p->ueId,
                    currentUEContext_p->uePriority,
                    currentUEContext_p->numLogicalChannels,
                    currentUEContext_p->maxNbOfRBs,
                    currentUEContext_p->effectiveBitRate,
                    currentUEContext_p->modulationScheme,
                    currentUEContext_p->codingRate,
                    currentUEContext_p->mcsIndex,
                    currentUEContext_p->ueQueueLoad,
                    currentUEContext_p->lastScheduledTick);

        }
    }
    printf ("***** Uplink UE Context Information *****\n");
    printf (
            "CRNTI | NumOfLcCh | maxNbOfRB | BSRNET 0 | BSRNET 1 | BSRNET 2 | BSRNET 3 | ModulationScheme | CodingRate | MCS Index | LastScheduledTick\n");
    for(count=0; count < TBL_SIZE; count++)
    {
        uplkUEContext_p = uplinkContextInfoMap_g[count];
        uplkDynUEContext_p = dynUEContextInfoMap_g[count];
        if(PNULL != uplkUEContext_p)
        {
            printf ("%5u  %8u   %8u   %8u   %7u   %7u   %7u   %14u       %10u   %8u  %14u\n",
                    uplkUEContext_p->rntis.CRnti,
                    uplkUEContext_p->numLogicalChannels,
                    uplkUEContext_p->maxNbOfRBs,
                    uplkDynUEContext_p->bsrNet[0],
                    uplkDynUEContext_p->bsrNet[1],
                    uplkDynUEContext_p->bsrNet[2],
                    uplkDynUEContext_p->bsrNet[3],
                    uplkUEContext_p->modulation,
                    uplkUEContext_p->codingrate,
                    uplkUEContext_p->mcsIndex,
                    uplkDynUEContext_p->lastScheduledTick);

        }
    }
    fflush(stdout);
    return;
}


/****************************************************************************
 * Function Name  : getMACUELogicalChannelStats
 * Inputs         : crnti - UE identifier
 * Outputs        :
 * Returns        : None
 * Variables      :
 * Description    : Prints the logical channel info for particular UE
 *                  
 ****************************************************************************/
void getMACUELogicalChannelStats (UInt16 crnti)
{
    UInt16 ueIndex = 0;
    DLUEContext *ueContext_p = PNULL;
    dynamicUEContext *uplkDynUEContext_p = PNULL;
    ueContextForUplink *uplkUEContext_p = PNULL;

    ueIndex = getUeIdxFromRNTIMap(crnti);
    LTE_LOG(LOG_DETAIL, &MACModuleLogDetail_g,"IDX = %d\n", ueIndex);
    if(ueIndex >= TBL_SIZE) {
        lteWarning("UE Context does not exist\n");
        return;
    }
    
    UInt16 index = 0;
    ueContext_p = ueDLContextArr_g[ueIndex];
    uplkDynUEContext_p = dynUEContextInfoMap_g[ueIndex];
    uplkUEContext_p = uplinkContextInfoMap_g[ueIndex];
   
    if((PNULL == ueContext_p) || (PNULL == uplkUEContext_p))
    {
        LTE_LOG (LOG_MAJOR,&MACModuleLogDetail_g,"UE Context with Idx = %d Not Found",ueIndex);
        return;
    }
    printf ("****** DWLK UE Context LC Stats ******\n");
    printf("LC ID\tPriority\tQueue Load\n");

    for(index=0;index < MAX_NUMBER_OF_LOGICAL_CHANNEL;index++)
    {
        if (ueContext_p->logicalChannel[index].logicalChannelId > 0)
        {
            printf("%4d\t%8d\t%4d\n",
                     ueContext_p->logicalChannel[index].logicalChannelId,
                     ueContext_p->logicalChannel[index].logicalChannelPriority,
                     ueContext_p->logicalChannel[index].queueLoad);
        }
    }

    printf("****** UPLK UE Context LC Stats ******\n");
    printf("LC ID\tLCG ID\t BSR NET of LCG\n");

    for(index=0;index < MAX_NUMBER_OF_LOGICAL_CHANNEL;index++)
    {
        if (uplkUEContext_p->lcInfo[index].lcGId != LCGID_INVALID_VALUE)
        {
            printf("%d\t%d\t%d\n",
                     uplkUEContext_p->lcInfo[index].lchId,
                     uplkUEContext_p->lcInfo[index].lcGId,
                     uplkDynUEContext_p->bsrNet[uplkUEContext_p->lcInfo[index].lcGId]);
        }
    }

    return;
}


/****************************************************************************
 * Function Name  : getMACBuildConfiguration
 * Inputs         : None
 * Outputs        :
 * Returns        : None
 * Variables      :
 * Description    : prints the MAC build info and returns it to OAM 
 ****************************************************************************/
UInt16 getMACBuildConfiguration(UInt8 *buff_p, UInt16 *msgLen_p)
{
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g," %s Entry.\n",__func__);
    
    FILE *buildFile_p;
    UChar8 *buildStamp_p = PNULL;
    UInt32 size = 0;
    UInt32 retVal = 0;
    UInt16 i = 0;

    buildFile_p = fopen("buildStamp","r");
    if(buildFile_p == PNULL)
    {
        LTE_LOG (LOG_MAJOR,&MACModuleLogDetail_g,"Product Build Version File Not Found \n");
        return MAC_FAILURE;
    }
    if((retVal = getline(&buildStamp_p, &size, buildFile_p)) != -1)
    {
        printf("Product Build: %s\n",buildStamp_p);
        while(buildStamp_p[i] != '\n') {
            buff_p[*msgLen_p] = buildStamp_p[i];
            i++;
            *msgLen_p += 1; 
        }    
        buff_p[*msgLen_p] = '\0';
        *msgLen_p += 1; 
    }
    else
    {
        printf("buildStamp file doesn't exist in the current path\n");
    }
    if(buildStamp_p)
        free(buildStamp_p);
    
    fclose(buildFile_p);

    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g," %s Exit.\n",__func__);
    return MAC_SUCCESS;
}


#endif
/****************************************************************************
 * Function Name  : setLogLevel
 * Inputs         : None
 * Outputs        :
 * Returns        : None
 * Variables      :
 * Description    : Sets the tracing/logging level for the MAC Layer.
 ****************************************************************************/
void setLogLevel(UInt8 level)
{
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g," %s Entry.\n",__func__);
   
    if (level > 0)
        lteLogLevel(level);
    else
        lteWarning ("Provisioned a wrong log level value.\n");

    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g," %s Exit.\n",__func__);

}

/****************************************************************************
 * Function Name  : enableLog
 * Inputs         : None
 * Outputs        :
 * Returns        : None
 * Variables      :
 * Description    : Sets the tracing/logging for the MAC Layer ON/OFF.
 ****************************************************************************/
void enableLog( UInt8 flag )
{
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g," %s Entry.\n",__func__);

    if (flag == 0)
        lteSetLogState (LOG_OFF);
    else if (flag == 1)
        lteSetLogState (LOG_ON);
    else
        lteWarning ("Provisioning an invalid value. (Valid values - 0/1)\n");
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g," %s Exit.\n",__func__);

}

#if 0

/****************************************************************************
 * Function Name  : enableMACDLSchedulerStats
 * Inputs         : flag- 1 enables 0 disables.
 * Outputs        :
 * Returns        : None
 * Variables      :
 * Description    : Set the enableStats_g global,which if set dispatcher fills
 *                  the global array statsInfoArr[][]
 ****************************************************************************/
void enableMACDLSchedulerStats( UInt8 flag )
{
    //UInt32 retTh = 0;

    if (flag == 0)
    {
        if(enableStats_g & DOWNLINK_STATS_ENABLED)
        {
            //resetStatsInfo(0, MAC_DL);
            enableStats_g ^= DOWNLINK_STATS_ENABLED;
        }
    }
    else if (flag == 1)
    {
        if (smRunForFirstTime ==1 )
        {
#if 0        
            /* Initialise the SM thread */
            retTh = threadCreate(&SMThID_g,THREAD_CREATE_DETACHED,
                THREAD_SCHED_DEFAULT,0,
                THREAD_INHERIT_SCHED_DEFAULT,
                THREAD_SCOPE_SYSTEM,statsManagerThread,PNULL);
            if (retTh != 0)
            {
                ltePanic(" Unable to initialise the SM thread.\n");
				return;
            }
#endif            
            smRunForFirstTime = 0;
        }
        if(!(enableStats_g & DOWNLINK_STATS_ENABLED))
        {
            //resetStatsInfo(1, MAC_DL);
            enableStats_g |= DOWNLINK_STATS_ENABLED;
        }
    }
    else
    {
        lteWarning ("[enableStats]Provisioning an invalid value. (Valid values - 0/1)\n");
    }
}


/****************************************************************************
 * Function Name  : enableMACULSchedulerStats
 * Inputs         : flag- 1 enables 0 disables.
 * Outputs        :
 * Returns        : None
 * Variables      :
 * Description    : Set the enableStats_g global,which if set dispatcher fills
 *                  the global array statsInfoArr[][]
 ****************************************************************************/
void enableMACULSchedulerStats( UInt8 flag )
{
    //UInt32 retTh = 0;

    if (flag == 0)
    {
        if(enableStats_g & UPLINK_STATS_ENABLED)
        {
            //resetStatsInfo(0, MAC_UL);
            enableStats_g ^= UPLINK_STATS_ENABLED;
        }
    }
    else if (flag == 1)
    {
        if (smRunForFirstTime ==1 )
        {
#if 0        
            /* Initialise the SM thread */
            retTh = threadCreate(&SMThID_g,THREAD_CREATE_DETACHED,
                THREAD_SCHED_DEFAULT,0,
                THREAD_INHERIT_SCHED_DEFAULT,
                THREAD_SCOPE_SYSTEM,statsManagerThread,PNULL);
            if (retTh != 0)
            {
                ltePanic(" Unable to initialise the SM thread.\n");
				return;
            }
#endif            
            smRunForFirstTime = 0;
        }
        if(!(enableStats_g & UPLINK_STATS_ENABLED))
        {
            //resetStatsInfo(1, MAC_UL);
            enableStats_g |= UPLINK_STATS_ENABLED;
        }
    }
    else
    {
        lteWarning ("[enableStats]Provisioning an invalid value. (Valid values - 0/1)\n");
    }
}

/****************************************************************************
 * Function Name  : displayMemPoolStats
 * Inputs         : None
 * Outputs        : Displays the memory pool stats
 * Returns        : None
 * Variables      :
 * Description    : This is a wrapper API for displaying memory pool stats.
 ****************************************************************************/

void displayMemPoolStats(void)
{
    getMemPoolStats();
}

/****************************************************************************
* Function Name  : getUplkThroughPut()
* Inputs         : None
* Outputs        : Displays Uplink Through Put of MAC Layer
* Returns        : None
* Variables      :
* Description    : This is a wrapper API for displaying uplink ThroughPut
*                  of MAC Layer.
****************************************************************************/
UDouble32 getUplinkThroughPut()
{
   UDouble32 throughput = 0;
   UDouble32 timesInSec = 0;
   if (globalTTITickCount_g > ulStartThroughputTick_g )
   { 
      timesInSec = ((UDouble32)(globalTTITickCount_g - ulStartThroughputTick_g))/1000;

      LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"timesInsec = %f, ulStartThroughputTick_g %u, globalTTITickCount_g %u\
            totalUlBytesSend_g =%u\n",timesInSec, ulStartThroughputTick_g, globalTTITickCount_g,
            totalUplinkBytesSend_g);

      if (timesInSec > 0)
          throughput =  ((((UDouble32)totalUplinkBytesSend_g) / (timesInSec*TICK_IN_MS))/MEGA_BITS_PER_SEC); 

      printf("Uplink throughput= %f\n",throughput);
   }
   ulStartThroughputTick_g = globalTTITickCount_g;
   totalUplinkBytesSend_g = 0;

   return throughput; 
}

/****************************************************************************
* Function Name  : getDwlkThroughPut()
* Inputs         : None
* Outputs        : Displays Uplink Through Put of MAC Layer
* Returns        : None
* Variables      :
* Description    : This is a wrapper API for displaying Downlink ThroughPut
*                  of MAC Layer.
****************************************************************************/

UDouble32 getDownlinkThroughPut()
{
   UDouble32 throughput = 0;
   UDouble32 timesInSec = 0;
   UInt32 totalDlBytesSend = 0;
   UInt8 i = 0;

   /*rev: disable dlThroughPut so tht no thread is modifying the throughput 
	* values*/
   dlThroughPutEnable_g = 0;

   for(i = 0 ; i < numEncoderThread_g; i++)
   {
	   totalDlBytesSend += totalDlBytesSend_g[i];
   }
   if (globalTTITickCount_g > dlStartThroughputTick_g )
   { 
      timesInSec = ((UDouble32)(globalTTITickCount_g - dlStartThroughputTick_g))/1000;

      LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g, "timesInsec = %f, dlStartThroughputTick_g %u, globalTTITickCount_g %u \
           totalDlBytesSend =%u\n",timesInSec, dlStartThroughputTick_g, globalTTITickCount_g,
           totalDlBytesSend);
  
      if (timesInSec > 0)
          throughput =  ((((UDouble32)totalDlBytesSend) / (timesInSec * TICK_IN_MS))/MEGA_BITS_PER_SEC); 
      printf("Downlink throughput= %f\n",throughput);
   }
   dlStartThroughputTick_g = globalTTITickCount_g;

   for(i = 0 ;  i < numEncoderThread_g ; i++)
   {
	   totalDlBytesSend_g[i] = 0;
   }

   dlThroughPutEnable_g = 1;
   return throughput; 
}


/****************************************************************************
* Function Name  : enableUplinkThroughput()
* Inputs         : None
* Outputs        : Enables/Disables the Uplink Throughput
* Returns        : None
* Variables      :
* Description    : This is a wrapper API for enabling/disabling Uplink ThroughPut
*                  of MAC Layer.
****************************************************************************/
void enableUplinkThroughput(UInt32 enable)
{
    if (enable)
        {
       ulThroughPutEnable_g = 1;
           totalUplinkBytesSend_g = 0;
           ulStartThroughputTick_g = globalTTITickCount_g;
        }
    else
    {
       ulThroughPutEnable_g = 0;
           totalUplinkBytesSend_g = 0;
           ulStartThroughputTick_g = 0;
    }
}

/****************************************************************************
* Function Name  : enableDownlinkThroughput()
* Inputs         : None
* Outputs        : Enables/Disables the Downlink Throughput
* Returns        : None
* Variables      :
* Description    : This is a wrapper API for enabling/disabling Downlink ThroughPut
*                  of MAC Layer.
****************************************************************************/
void enableDownlinkThroughput(UInt32 enable)
{
		UInt8 i = 0;
    if (enable)
        {
       dlThroughPutEnable_g = 1;
   			for(i = 0 ;  i < numEncoderThread_g ; i++)
   			{
	   			totalDlBytesSend_g[i] = 0;
   			}
           dlStartThroughputTick_g = globalTTITickCount_g;
        }
    else
    {
       dlThroughPutEnable_g = 0;
   			for(i = 0 ; i < numEncoderThread_g ; i++)
   			{
	   			totalDlBytesSend_g[i] = 0;
   			}
           dlStartThroughputTick_g = 0;
    }
}
#endif
#endif
