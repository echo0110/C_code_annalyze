/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacOAMInterface.c,v 1.2.6.1.6.2 2010/10/25 09:38:31 gur23971 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functional implementation of which
 *                     is called when the OAM message is parsed corresponding to
 *                     to the request sent by the OAM simulator.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacOAMInterface.c,v $
 * Revision 1.2.6.1.6.2  2010/10/25 09:38:31  gur23971
 * warning removed
 *
 * Revision 1.2.6.1.6.1  2010/09/27 16:49:14  gur10121
 * SPR 662 Fix
 *
 * Revision 1.2.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.12.28.1  2010/07/21 09:29:19  gur24420
 * Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
 * Overall Changes Cover:-
 * 1. Merged Endian fixes for receiving values from RRC.
 *
 * Revision 1.12  2009/11/11 07:37:03  gur19413
 * Num of harq process related changes.
 *
 * Revision 1.11  2009/08/28 13:27:25  gur19413
 * removed older logger system
 *
 * Revision 1.10  2009/07/29 07:02:18  gur11083
 * Changed for LC count
 *
 * Revision 1.9  2009/07/15 08:23:01  gur11083
 * changed for simultaneousAckNackAndCQI in UL and LC count in DL
 *
 * Revision 1.8  2009/07/14 12:53:54  gur11083
 * review comments incorporated
 *
 * Revision 1.7  2009/07/14 08:06:00  gur18569
 * replaced MAX_NB_ENCODER_THREADS with NUM_OF_ENCODER_THREADS
 *
 * Revision 1.6  2009/06/18 11:01:26  gur20470
 * added coding rate info for UL and DL status info
 *
 * Revision 1.5  2009/06/10 15:15:00  gur18569
 * removed linking and compilation errors
 *
 * Revision 1.4  2009/06/10 10:05:00  gur20470
 * updated for mac1.2
 *
 * Revision 1.3  2009/06/07 09:06:23  gur19413
 * removing warning
 *
 * Revision 1.2  2009/06/07 08:45:39  gur20470
 * updated for mac1.2
 *
 * Revision 1.1  2009/05/05 11:17:53  gur19413
 * Initial Version
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacExecutionLegs.h"
#include "lteOamInterface.h"

/******************************************************************************
 * Project Includes
 *****************************************************************************/

/******************************************************************************
 Private Definitions
 *****************************************************************************/


/******************************************************************************
 Private Types
*****************************************************************************/

/*****************************************************************************
  * Private Function Prototypes
****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*CA Changes start */
StatsInfo statsInfo_g[MAX_NUM_CELL] = {{0}};
extern UDouble32 ulBandwidth_g[MAX_NUM_CELL];
extern UDouble32 dlBandwidth_g[MAX_NUM_CELL];

/* CA Changes end */
/******************************************************************************
 * Exported Functions
 *****************************************************************************/
#ifdef KPI_STATS
extern void  macResetKpiCounters(void);
extern void sendMacKpiStatsInd(LteMacKPIStats  *pLteMacKPIStats);
#endif


/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/****************************************************************************
 * Function Name  : fillUeStatus
 * Inputs         : cnfBuff_p - Buffer to fill status of UE,
                    current_p - Buffer Index,
                    ueIndex_p - UE Index
 *                  Internal cell index for which data is requested
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This API is used to fill the response buffer with the 
 *                  status of UE with their logical channels. If ueIndex_p is 
 *                  not NULL, then fill the buffer for that particular UE else
 *                  send the response status of all active UE.
 ****************************************************************************/
UInt16 fillUeStatus(UInt8 *cnfBuff_p, UInt16 *current_p, UInt16 *ueIndex_p, InternalCellIndex internalCellIndex)
{
    UInt16 numActiveUE = 0,count = 0, loopIdx = 0;
    DLUEContext *currentUEContext_p = PNULL;
    ULUEContext *uplkUEContext_p = PNULL;
    UInt16 index = 0;
    MacUEStatusResp *macUEStatusResp_p = (MacUEStatusResp *)(cnfBuff_p + *current_p);
    MacPerUEStatusResp *macUEStatusInfo = PNULL;
    UInt16 ueCount = 0; 
    UInt32 numOfLC=0;
    if ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]) &&
        (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p))
    {
        numActiveUE = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->totalActiveUE;
    }

    if(0 == numActiveUE)
    {
        /* SPR 2153 Fix Start */
        *current_p += sizeof(MacUEStatusResp);
        /* SPR 2153 Fix End */
        return MAC_NO_ACTIVE_UE_IN_SYSTEM;
    }

    /* Set the loop either for all active UEs or one UE whose index
       is passed as an argument - *ueIndex_p */
    if(ueIndex_p) 
    {
        count = *ueIndex_p;
        loopIdx = *ueIndex_p;
        currentUEContext_p = dlUECtxInfoArr_g[count].dlUEContext_p;
        uplkUEContext_p = ulUECtxInfoArr_g[count].ulUEContext_p;
        /* If single UE and its context doesnt exist then return FAILURE */
        if((PNULL == currentUEContext_p) || (PNULL == uplkUEContext_p))
        {
            *current_p += sizeof(MacUEStatusResp);
            return MAC_UE_NOT_EXISTS;
        }
        /* If cell index does not match with cell in which UE is attached */
        if (internalCellIndex != currentUEContext_p->internalCellIndex)
        {
            *current_p += sizeof(MacUEStatusResp);
            return MAC_UE_NOT_VALID_ON_CELL;
        }
    }
    else
    {
        count = 0;
        /* SPR 5681 changes start */
        loopIdx = MAX_UE_SUPPORTED-1;
        /* SPR 5681 changes end */
    }

    for(; count <= loopIdx; count++)
    {
        currentUEContext_p = dlUECtxInfoArr_g[count].dlUEContext_p;
        uplkUEContext_p = ulUECtxInfoArr_g[count].ulUEContext_p;
        /* if either UE does not exists or UE does not belong to same cell
         * go to next UE */
        if((PNULL == currentUEContext_p) || (PNULL == uplkUEContext_p) ||
           (internalCellIndex != currentUEContext_p->internalCellIndex))
        {
/*4nov +*/
/**/
            continue;
        }
        macUEStatusInfo = &(macUEStatusResp_p->macUEStatusInfo[ueCount]);

        /* Fill Downlink Status Info */
        macUEStatusInfo->rnti = currentUEContext_p->crnti;
        macUEStatusInfo->uePriority = currentUEContext_p->uePriority;
        macUEStatusInfo->numTxAntenna = 
                 cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->numOfTxAnteenas;
        macUEStatusInfo->dlTxMode = currentUEContext_p->dlMIMOInfo.transmissionMode; 
        macUEStatusInfo->dlMaxRB =  currentUEContext_p->maxNbOfRBs;
        macUEStatusInfo->dlModSchemeCodeWord0 = currentUEContext_p->dlCQIInfo.modulationSchemeCodeWordOne;

        /* SPR 1737 changes start */
        /* + CQI_5.0 */
        macUEStatusInfo->dlNumOfLayer = currentUEContext_p->dlMIMOInfo.dlRi;
        /* - CQI_5.0 */
        if((macUEStatusInfo->dlTxMode == TX_MODE_3) || 
           (macUEStatusInfo->dlTxMode == TX_MODE_4) ||
           (macUEStatusInfo->dlTxMode == TX_MODE_8))
        {
            macUEStatusInfo->dlModSchemeCodeWord1 = currentUEContext_p->dlCQIInfo.modulationSchemeCodeWordTwo;
            macUEStatusInfo->dlMcsIndexCodeWord1 = currentUEContext_p->dlCQIInfo.mcsIndexCodeWordTwo;
        }
        else
        {
            macUEStatusInfo->dlModSchemeCodeWord1 = 0;
            macUEStatusInfo->dlMcsIndexCodeWord1  = 0;
        }
        macUEStatusInfo->simultaneousAckNackAndCQI = 
            uplkUEContext_p->pucchConfigInfo.simultaneousAckNackAndCQI; 

        /* SPR 1737 changes end */ 
        macUEStatusInfo->dlMcsIndexCodeWord0 = currentUEContext_p->dlCQIInfo.mcsIndexCodeWordOne;
        macUEStatusInfo->dlQueueLoad = currentUEContext_p->ueQueueLoad; 
        macUEStatusInfo->dlCodeBookIndex = currentUEContext_p->dlMIMOInfo.precodingIndex;
        macUEStatusInfo->cqiIndicator = currentUEContext_p->dlCQIInfo.cqiTypeIndicator;
        macUEStatusInfo->bsrNet0 = uplkUEContext_p->bsrNet[0];
        macUEStatusInfo->bsrNet1 = uplkUEContext_p->bsrNet[1];
        macUEStatusInfo->bsrNet2 = uplkUEContext_p->bsrNet[2];
        macUEStatusInfo->bsrNet3 = uplkUEContext_p->bsrNet[3];
        macUEStatusInfo->ulModScheme = uplkUEContext_p->modulationScheme;
        macUEStatusInfo->ulMcsIndex = uplkUEContext_p->mcsIndex;

        ueCount++;
        numOfLC = 0;

        /* Fill DL_LC_INFO */
        for(index=0;index < MAX_NUMBER_OF_LOGICAL_CHANNEL;index++)
        {
            if (currentUEContext_p->logicalChannel[index].logicalChannelId !=INVALID_LCID)
            {
                macUEStatusInfo->macUeDLLogChStatus[numOfLC].lcId = 
                    currentUEContext_p->logicalChannel[index].logicalChannelId;
                macUEStatusInfo->macUeDLLogChStatus[numOfLC].lcPriority = 
                    currentUEContext_p->logicalChannel[index].logicalChannelPriority;
                macUEStatusInfo->macUeDLLogChStatus[numOfLC].queueLoad = 
                    currentUEContext_p->logicalChannel[index].queueLoad;
                numOfLC++;    
            }
        }
        /*Fill Total number of LC in DL*/
        macUEStatusInfo->dlNumOfLCs =  numOfLC;

        /* Fill UL_LC_INFO */
        numOfLC = 0;
        for(index=0;index < MAX_NUMBER_OF_LOGICAL_CHANNEL;index++)
        {
            if (uplkUEContext_p->lcInfo[index].lcGId != LCGID_INVALID_VALUE)
            {
                macUEStatusInfo->macUeULLogChStatus[numOfLC].lcId = 
                    uplkUEContext_p->lcInfo[index].lchId;
                macUEStatusInfo->macUeULLogChStatus[numOfLC].lcgId = 
                    uplkUEContext_p->lcInfo[index].lcGId;
                macUEStatusInfo->macUeULLogChStatus[numOfLC].bsrNetOfLCG = 
                    uplkUEContext_p->bsrNet[uplkUEContext_p->lcInfo[index].lcGId];
                numOfLC++;
            }
        }

        macUEStatusInfo->ulNumOfLCs =  numOfLC;

        macUEStatusInfo->tpcPUCCHRNTI = currentUEContext_p->tpcInfoForPucch.tpcRnti;
        macUEStatusInfo->tpcPUSCHRNTI = uplkUEContext_p->tpcInfoForPusch.tpcRnti;
        macUEStatusInfo->absolutePowerCorrectionPUSCH = uplkUEContext_p->currPowerCorrectionForPusch;
        macUEStatusInfo->absolutePowerCorrectionPUCCH = currentUEContext_p->currPowerCorrectionForPucch;
    }

    macUEStatusResp_p->numOfUe = ueCount;

    if(ueCount > 1)
    {
        *current_p += sizeof(MacUEStatusResp) + ((ueCount-1)*sizeof(MacPerUEStatusResp)) ;
    }
    else
    {
        *current_p += sizeof(MacUEStatusResp);
    }
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : setLogLevel
 * Inputs         : level - Log level to be enabled
 * Outputs        : None
 * Returns        : None
 * Description    : Sets the tracing/logging level for the MAC Layer.
 ****************************************************************************/
void setLogLevel(UInt32 level)
{
    LTE_MAC_UT_LOG(LOG_DETAIL,OAM_INTF_MODULE," %s Entry.\n",__func__);
   
    if (level <= MAX_LOG_LEVEL )
    {
        /* SPR#662 Changes Start */
        lteShmSetModuleLogLevel(MAC_MODULE,level);
        /* SPR#662 Changes End */
    }
    else
    {
        lteWarning ("Provisioned a wrong log level value.\n");

        LOG_MAC_MSG(OAM_INVALID_LOGLEVEL_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(0),
                level,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"INVALID_LOGLEVEL_FLAG_ID");
    }

    LTE_MAC_UT_LOG(LOG_DETAIL,OAM_INTF_MODULE," %s Exit.\n",__func__);

}

/****************************************************************************
 * Function Name  : enableLog
 * Inputs         : flag - 1 enables 0 disables. 
 * Outputs        : tracing/logging
 * Returns        : None
 * Description    : Sets the tracing/logging for the MAC Layer ON/OFF.
 ****************************************************************************/
void enableLog( UInt8 flag )
{
    LTE_MAC_UT_LOG(LOG_DETAIL,OAM_INTF_MODULE," %s Entry.\n",__func__);

    if (flag == 0)
        lteSetLogState (LOG_OFF);
    else if (flag == 1)
        lteSetLogState (LOG_ON);
    else
    {
        lteWarning ("Provisioning an invalid value. (Valid values - 0/1)\n");

        LOG_MAC_MSG(OAM_INVALID_ENABLE_LOG_FLAG_ID,LOGWARNING,MAC_OAM_INF,
                GET_CURRENT_TICK_FOR_CELL(0),
                flag,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"INVALID_ENABLE_LOG_FLAG");


    }
    LTE_MAC_UT_LOG(LOG_DETAIL,OAM_INTF_MODULE," %s Exit.\n",__func__);

}

/****************************************************************************
 * Function Name  : displayMemPoolStats
 * Inputs         : None
 * Outputs        : Displays the memory pool stats
 * Returns        : None
 * Description    : This is a wrapper API for displaying memory pool stats.
 ****************************************************************************/

void displayMemPoolStats(void)
{
    getMemPoolStats();
}

/* SPR 2127 changes start */
/****************************************************************************
* Function Name  : calculateUplinkThroughPut()
* Inputs         : internalCellIndex
* Outputs        : None
* Returns        : None
* Description    : Calculate & Update the Uplink Throughput in global variable.
****************************************************************************/
/* CA Changes start */
void calculateUplinkThroughPut(InternalCellIndex internalCellIndex)
{
    UInt32 timesInSec = 0;

    timesInSec = MAC_STATS_INT_TICK_COUNT/1000;

    /* + SPR 19808 */
    /* Update the UL throughput in terms of bits per second */
    statsInfo_g[internalCellIndex].lastMacUlThroughput = 
        ( ((UDouble32)statsInfo_g[internalCellIndex].totalUplinkBytesSend) 
          / (timesInSec*TICK_IN_MS) ); 
    /* - SPR 19808 */

    LTE_MAC_UT_LOG(LOG_DETAIL,OAM_INTF_MODULE,"timesInsec = %d, globalTTITickCount_g = %u\
            totalUlBytesSend_g =%u, lastMacUlThroughput =%f\n",timesInSec, GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            statsInfo_g[internalCellIndex].totalUplinkBytesSend,
            statsInfo_g[internalCellIndex].lastMacUlThroughput);

    statsInfo_g[internalCellIndex].totalUplinkBytesSend = 0;

    return ; 
}

/****************************************************************************
* Function Name  : getUplinkThroughPut()
* Inputs         : internalCellIndex
* Outputs        : Displays Uplink Through Put of MAC Layer
* Returns        : None
* Description    : This is a wrapper API for displaying uplink ThroughPut
*                  of MAC Layer.
****************************************************************************/
UDouble32 getUplinkThroughPut(InternalCellIndex internalCellIndex)
{
   return statsInfo_g[internalCellIndex].lastMacUlThroughput; 
}

/****************************************************************************
* Function Name  : calculateDownlinkThroughPut()
* Inputs         : internalCellIndex
* Outputs        : None
* Returns        : None
* Description    : Calculate & update the Downlink Throughput in global variable.
****************************************************************************/
void calculateDownlinkThroughPut(InternalCellIndex internalCellIndex)
{
    UInt64 totalDlBytesSend = 0;
    UInt32 timesInSec = 0;

    timesInSec = MAC_STATS_INT_TICK_COUNT/1000;

    totalDlBytesSend += statsInfo_g[internalCellIndex].totalDlBytesSend;
    statsInfo_g[internalCellIndex].totalDlBytesSend = 0;

    /* + SPR 19808 */
    /* Update the DL throughput in terms of bits per second */
    statsInfo_g[internalCellIndex].lastMacDlThroughput =
        ( ((UDouble32)totalDlBytesSend) / (timesInSec*TICK_IN_MS) ); 
    /* - SPR 19808 */

    LTE_MAC_UT_LOG(LOG_DETAIL,OAM_INTF_MODULE,"timesInsec = %d, globalTTITickCount_g = %u\
            totalDlBytesSend =%u, lastMacDlThroughput =%f\n",timesInSec, GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            totalDlBytesSend,statsInfo_g[internalCellIndex].lastMacDlThroughput);

    return ; 
}

/****************************************************************************
* Function Name  : getDownlinkThroughPut()
* Inputs         : internalCellIndex
* Outputs        : Displays Uplink ThroughPut of MAC Layer
* Returns        : None
* Description    : This is a wrapper API for displaying Downlink ThroughPut
*                  of MAC Layer.
****************************************************************************/
UDouble32 getDownlinkThroughPut(InternalCellIndex internalCellIndex)
{
   return statsInfo_g[internalCellIndex].lastMacDlThroughput; 
   
}

/****************************************************************************
* Function Name  : getDlSpectralEfficiency()
* Inputs         : internalCellIndex
* Outputs        : Displays Uplink SpectralEfficiency  of MAC Layer
* Returns        : None
* Description    : This is a wrapper API for displaying Uplink SpectralEfficiency
*                  of MAC Layer.
****************************************************************************/
UDouble32 getDlSpectralEfficiency(InternalCellIndex internalCellIndex)
{
    /* SPR 20675 FIX + */
    UDouble32 bwInHertz = dlBandwidth_g[internalCellIndex]*1000000;

    /* Since lastMacDlThroughput is calculated bits per second, DlSpectralEfficiency must also be calculated accordingly. */
    return (((UDouble32)statsInfo_g[internalCellIndex].lastMacDlThroughput)/bwInHertz);
    /* SPR 20675 FIX - */
}

/****************************************************************************
* Function Name  : getUlSpectralEfficiency()
* Inputs         : internalCellIndex
* Outputs        : Displays Uplink SpectralEfficiency  of MAC Layer
* Returns        : None
* Description    : This is a wrapper API for displaying Uplink SpectralEfficiency
*                  of MAC Layer.
****************************************************************************/
UDouble32 getUlSpectralEfficiency(InternalCellIndex internalCellIndex)
{
    /* SPR 20675 FIX + */
    UDouble32 bwInHertz = ulBandwidth_g[internalCellIndex]*1000000;

    /* Since lastMacUlThroughput is calculated bits per second, UlSpectralEfficiency must also be calculated accordingly. */
    return (((UDouble32)statsInfo_g[internalCellIndex].lastMacUlThroughput)/bwInHertz);
    /* SPR 20675 FIX - */
}
/* SPR 2127 changes end */

/****************************************************************************
* Function Name  : enableUplinkThroughput()
* Inputs         : enable,internalCellIndex
* Outputs        : Enables/Disables the Uplink Throughput
* Returns        : None
* Description    : This is a wrapper API for enabling/disabling Uplink ThroughPut
*                  of MAC Layer.
****************************************************************************/
void enableUplinkThroughput(UInt32 enable, InternalCellIndex internalCellIndex)
{
    if (enable)
    {
        statsInfo_g[internalCellIndex].ulThroughPutEnable = 1;
        statsInfo_g[internalCellIndex].totalUplinkBytesSend = 0;

        statsInfo_g[internalCellIndex].ulStartThroughputTick =
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
        statsInfo_g[internalCellIndex].ulStartSpecEffTick =
            statsInfo_g[internalCellIndex].ulStartThroughputTick;
    }
    else
    {
        statsInfo_g[internalCellIndex].ulThroughPutEnable = 0;
        statsInfo_g[internalCellIndex].totalUplinkBytesSend = 0;
        statsInfo_g[internalCellIndex].ulStartThroughputTick = 0;
    }
}


/****************************************************************************
* Function Name  : enableDownlinkThroughput()
* Inputs         : enable, internalCellIndex
* Outputs        : Enables/Disables the Downlink Throughput
* Returns        : None
* Description    : This is a wrapper API for enabling/disabling Downlink ThroughPut
*                  of MAC Layer.
****************************************************************************/
void enableDownlinkThroughput(UInt32 enable, InternalCellIndex internalCellIndex)
{
    if (enable)
    {
        statsInfo_g[internalCellIndex].totalDlBytesSend = 0;
        /** KlocWork Changes Start **/
        statsInfo_g[internalCellIndex].dlStartThroughputTick = 
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex);
        statsInfo_g[internalCellIndex].dlStartSpecEffTick = 
            statsInfo_g[internalCellIndex].dlStartThroughputTick;
        /** KlocWork Changes End **/
        statsInfo_g[internalCellIndex].dlThroughPutEnable = 1;
    }
    else
    {
        statsInfo_g[internalCellIndex].dlThroughPutEnable = 0;
        statsInfo_g[internalCellIndex].totalDlBytesSend = 0;
        statsInfo_g[internalCellIndex].dlStartThroughputTick = 0;
    }
}

/****************************************************************************
 * Function Name  : fillMACUeSinrTaReq
 * Inputs         : ueIndex - Internal UE Index
 *                  internal cell index for which OAM has requested
 *                  macUeSinrTaResp - response message
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the MAC UE SINR TA contents
****************************************************************************/
void fillMACUeSinrTaReq
(
    UInt16 ueIndex,
    InternalCellIndex internalCellIndex,
    MacUeSinrTaResp *macUeSinrTaRes_p
)
{
    ULUEContext *ulUEContext_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;

    if(ueIndex >= MAX_UE_SUPPORTED)
    {
        /* SPR 4501 fix */
        macUeSinrTaRes_p->responseCode = MAC_INVALID_UE_ID;
        /* SPR 4501 fix */
        macUeSinrTaRes_p->timingAdvanceValue = 65535;
        /* SPR 3100 START */
        macUeSinrTaRes_p->averageULSINR = 255;
        macUeSinrTaRes_p->averageDLSINRCW0 = 255;
        macUeSinrTaRes_p->averageDLSINRCW1 = 255;
        /* SPR 3100 END */
        macUeSinrTaRes_p->uplinkPathloss = 65535;
        macUeSinrTaRes_p->uplinkBler = 255;
        macUeSinrTaRes_p->downlinkBlerCW0 = 255;
        macUeSinrTaRes_p->downlinkBlerCW1 = 255;
        /* SPR 5033 changes start*/
        macUeSinrTaRes_p->averagePucchSINR = 255;
        macUeSinrTaRes_p->pucchBler = 255;
        /* SPR 5033 changes end*/
    }
    else
    {
        ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
        dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
        if((dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag) ||
                (PNULL == dlUEContext_p) ||
                (PNULL == ulUEContext_p))
        {
            /* SPR 4501 fix */
            macUeSinrTaRes_p->responseCode = MAC_INVALID_UE_ID;
            /* SPR 4501 fix */
            macUeSinrTaRes_p->timingAdvanceValue = 65535;
            /* SPR 3100 START */
            macUeSinrTaRes_p->averageULSINR = 255;
            macUeSinrTaRes_p->averageDLSINRCW0 = 255;
            macUeSinrTaRes_p->averageDLSINRCW1 = 255;
            /* SPR 3100 END */
            macUeSinrTaRes_p->uplinkPathloss = 65535;
            macUeSinrTaRes_p->uplinkBler = 255;
            macUeSinrTaRes_p->downlinkBlerCW0 = 255;
            macUeSinrTaRes_p->downlinkBlerCW1 = 255;
            /* SPR 5033 changes start*/
            macUeSinrTaRes_p->averagePucchSINR = 255;
            macUeSinrTaRes_p->pucchBler = 255;
            /* SPR 5033 changes end*/
        }
        /* If UE does not belong to same cell for which it is requested */
        else if(dlUEContext_p->internalCellIndex != internalCellIndex)
        {
            macUeSinrTaRes_p->responseCode = MAC_UE_NOT_VALID_ON_CELL;
            macUeSinrTaRes_p->timingAdvanceValue = 65535;
            macUeSinrTaRes_p->averageULSINR = 255;
            macUeSinrTaRes_p->averageDLSINRCW0 = 255;
            macUeSinrTaRes_p->averageDLSINRCW1 = 255;
            macUeSinrTaRes_p->uplinkPathloss = 65535;
            macUeSinrTaRes_p->uplinkBler = 255;
            macUeSinrTaRes_p->downlinkBlerCW0 = 255;
            macUeSinrTaRes_p->downlinkBlerCW1 = 255;
            macUeSinrTaRes_p->averagePucchSINR = 255;
            macUeSinrTaRes_p->pucchBler = 255;
        }
        else
        {
            /* SPR 4501 fix */
            macUeSinrTaRes_p->responseCode = MAC_SUCCESS;
            /* SPR 4501 fix */
            macUeSinrTaRes_p->timingAdvanceValue = dlUEContext_p->timingAdvanceAbsValue;
            /* SPR 3100 START */
            macUeSinrTaRes_p->averageULSINR = ulUEContext_p->csiContext.averageSINR;
            /* +- SPR 17777 */
            macUeSinrTaRes_p->averageDLSINRCW0 =
                getDLSinrValueFromCqi(dlUEContext_p->dlCQIInfo.widebandCQICodeWordOne);
            macUeSinrTaRes_p->averageDLSINRCW1 =
                getDLSinrValueFromCqi(dlUEContext_p->dlCQIInfo.widebandCQICodeWordTwo);
            /* +- SPR 17777 */
            /* SPR 3100 END */
            macUeSinrTaRes_p->uplinkPathloss = ulUEContext_p->pathLoss;
            macUeSinrTaRes_p->uplinkBler =
                ulUEContext_p->csiContext.puschBlerInfo.puschBler;
            macUeSinrTaRes_p->downlinkBlerCW0 = dlUEContext_p->dlCQIInfo.currentDLBLERForCW0;
            macUeSinrTaRes_p->downlinkBlerCW1 = dlUEContext_p->dlCQIInfo.currentDLBLERForCW1;
            /* SPR 5033 changes start*/
            macUeSinrTaRes_p->averagePucchSINR = dlUEContext_p->averageSinrForPucch;
            macUeSinrTaRes_p->pucchBler = dlUEContext_p->blerInfoForPUCCH.pucchBler;
            /* SPR 5033 changes end*/
        }
    }
    /* ICIC changes end */
    return;
}

/* CA Changes end */
