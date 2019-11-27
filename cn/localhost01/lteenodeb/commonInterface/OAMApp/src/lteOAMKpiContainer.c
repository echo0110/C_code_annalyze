/***************************************************************************
*
* ARICENT -
 *
* Copyright (C) 2014 Aricent Inc . All Rights Reserved.
*
****************************************************************************
*
* \$Id: lteOAMKpiContainer.c
*
****************************************************************************
*
* File Description: This file contains data structures and functions for
*                   managing eNodeB KPIs
*
****************************************************************************
* Revision Details
* ----------------------
*
***************************************************************************/

#include "lteMacCellConfMgr.h"
#include "lteLayer2CommanTypes.h"
#include "lteCommonStatsManager.h"
#ifdef LAYER2_DLTG_THREAD
#include "lteLayer2DLTG.h"
#endif
#include "lteOAMIntergration.h"
#include "lteMsgPool.h"
#include "lteMacExecutionScheduler.h"
#include "lteMacParseUtil.h"
#include "lteRelay.h"
#include "logging.h"
#include "lteCircQueue.h" 
#ifdef DL_UL_SPLIT
#include "lteMacPhyInterface.h"
#endif
#include "lteMacParseOAMMsg.h"
/*#include "lteMacRRMParseUtil.h"*/
/*#include "lteMacSONParseUtil.h"*/

#include "lteMacOAMInterface.h"
/* KPI changes start*/
#include "lteL2RRMInterface.h"

#include "ltePdcpRRMInterface.h"
extern SInt32 sendPdcpMsgToRRM(UInt8 *buffer,UInt16 length);
/* KPI changes end*/

#if defined(KPI_STATS) || defined(PERF_STATS)
extern SInt32 pdcpOamMsgEventFd_g;
#endif
 
#ifdef PERF_STATS
void pdcpProcessPerfCounter(void);
void updatePdcpPerfContainer(void);
extern void macProcessPerfCounterforRRM(void);
#endif
extern UInt8 macIsCellRunning(InternalCellIndex cellIndex);

#ifdef KPI_STATS
extern void  macResetKpiCounters(UInt32 passiveIndexForMacStats,InternalCellIndex internalCellIndex);

extern void  pdcpResetKpiCounters(UInt32 passiveIndexForPdcpStats, InternalCellIndex internalCellIndex);
extern void macGetKpiStats(UInt8 *cnfBuff, UInt16 *current_p, 
                           UInt16 kpiBitmap, LteMacKPIStats  *pLteMacKPIStats, 
                           tickType_t durationTTI);
extern SInt32 sendMsgToRRM(UInt8 *buffer,UInt16 length
        );
extern void sendPdcpKpiThpStatsInd(UInt32 pdcpKpiIndex);
extern void updateStatsForAllModules(InternalCellIndex internalCellIndex);

/*Global KPI Container*/
kpiScanner g_kpiScannerObj;
/* SPR 14329 Changes Start */
/* Global IN-session flag */
UInt8 insessionConfigFlag_g = 0;
/* SPR 14329 Changes End */

/* information about all the KPI subscribed modules */
/* 100MS_SUPPORT : start */
kpiModuleInfo g_kpiModuleInfo[MAX_KPI_CONTAINER]=
/* 100MS_SUPPORT : end */
{
        /* KPI_CONTAINER_OAM_MAC  */
	{
         MAC_MODULE_ID,                    /* SRC MODULE */  
         OAM_MODULE_ID,                    /* DST MODULE */  
         MAC_API_HEADER_LEN,               /* HEADER LEN */   
         MAC_CONFIGURE_KPI_STATS_CNF,      /* CONFIG CNF MSG ID */      
         sendMsgToOAM,                     /* SEND FUNC */
         MAC_KPI_STATS_IND,                /* Periodic Stat Ind MSG ID */ 
         MAC_GET_KPI_STATS_CNF,            /* Aperiodic Stat Ind MSG ID */
         MAC_KPI_THP_STATS_IND,            /* Periodic Throughput Stat Ind MSG ID */
         MAC_GET_KPI_THP_STATS_CNF         /* Aperiodic Throughput Stat Ind MSG ID */
        },

        /* KPI_CONTAINER_OAM_PDCP */
	{
         PDCP_MODULE_ID,                   /* SRC MODULE */  
         OAM_MODULE_ID,                    /* DST MODULE */ 
         MAC_API_HEADER_LEN,               /* HEADER LEN */     
         /* SPR 9680 changes start */
         PDCP_CONFIGURE_KPI_STATS_RESP,     /* CONFIG CNF MSG ID */       
         /* SPR 9680 changes end */
         pdcpOamSendMessage,               /* SEND FUNC */
         PDCP_KPI_STATS_IND,               /* Periodic Stat Ind MSG ID */
         /* SPR 9680 changes start */
         PDCP_GET_KPI_STATS_RESP,          /* Aperiodic Stat Ind MSG ID */
         /* SPR 9680 changes end */
         PDCP_KPI_THP_STATS_IND,           /* Periodic Throughput Stat Ind MSG ID */
         /* SPR 9680 changes start */
         PDCP_GET_KPI_THP_STATS_RESP       /* Aperiodic Throughput Stat Ind MSG ID */
         /* SPR 9680 changes end */
        },

        /* KPI_CONTAINER_RRM_MAC  */
	{
         MAC_MODULE_ID,                    /* SRC MODULE */   
         RRM_MODULE_ID,                    /* DST MODULE */ 
/* SPR 5464 */         
         L2_RRM_API_HEADER_LEN,           /* HEADER LEN */ 
/* SPR 5464 */         
         RRM_MAC_CONFIGURE_KPI_STATS_CNF,  /* CONFIG CNF MSG ID */       
         sendMsgToRRM,                     /* SEND FUNC */
         RRM_MAC_KPI_STATS_IND,            /* Periodic Stat Ind MSG ID */
         RRM_MAC_GET_KPI_STATS_CNF,        /* Aperiodic Stat Ind MSG ID */
         RRM_MAC_KPI_THP_STATS_IND,        /* Periodic Throughput Stat Ind MSG ID */
         RRM_MAC_GET_KPI_THP_STATS_CNF     /* Aperiodic Throughput Stat Ind MSG ID */
        },

        /* KPI_CONTAINER_RRM_PDCP */
	{
         PDCP_MODULE_ID,                   /* SRC MODULE */  
         RRM_MODULE_ID,                    /* DST MODULE */ 
         L2_RRM_API_HEADER_LEN,            /* HEADER LEN */ 
         RRM_PDCP_CONFIGURE_KPI_STATS_CNF, /* CONFIG CNF MSG ID */       
         sendPdcpMsgToRRM,                 /* SEND FUNC */
         RRM_PDCP_KPI_STATS_IND,            /* Periodic Stat Ind MSG ID */
         RRM_PDCP_GET_KPI_STATS_CNF,        /* Aperiodic Stat Ind MSG ID */
         RRM_PDCP_KPI_THP_STATS_IND,        /* Periodic Throughput Stat Ind MSG ID */
         RRM_PDCP_GET_KPI_THP_STATS_CNF     /* Aperiodic Throughput Stat Ind MSG ID */
        }, 

        /* KPI_CONTAINER_SON_MAC  */
	{
         MAC_MODULE_ID,                    /* SRC MODULE */   
         SON_MODULE_ID,                    /* DST MODULE */ 
         MAC_API_HEADER_LEN,               /* HEADER LEN */     
         0,                             /* CONFIG CNF MSG ID */
         NULL,                             /* SEND FUNC */
         0,                             /* Periodic Stat Ind MSG ID */
         0,                             /* Aperiodic Stat Ind MSG ID */
         0,                             /* Periodic Throughput Stat Ind MSG ID */
         0                              /* Aperiodic Throughput Stat Ind MSG ID */
        },

        /* KPI_CONTAINER_SON_PDCP */
	{
         PDCP_MODULE_ID,                   /* SRC MODULE */  
         SON_MODULE_ID,                    /* DST MODULE */ 
         MAC_API_HEADER_LEN,               /* HEADER LEN */     
         0,                             /* CONFIG CNF MSG ID */
         NULL,                             /* SEND FUNC */
         0,                             /* Periodic Stat Ind MSG ID */
         0,                             /* Aperiodic Stat Ind MSG ID */
         0,                             /* Periodic Throughput Stat Ind MSG ID */
         0                              /* Aperiodic Throughput Stat Ind MSG ID */
        },

        /* KPI_CONTAINER_THP_OAM_MAC  */
	{
         MAC_MODULE_ID,                    /* SRC MODULE */  
         OAM_MODULE_ID,                    /* DST MODULE */  
         MAC_API_HEADER_LEN,               /* HEADER LEN */   
         0,                                /* CONFIG CNF MSG ID */      
         sendMsgToOAM,                     /* SEND FUNC */
         0,                                /* Periodic Stat Ind MSG ID */ 
         0,                                /* Aperiodic Stat Ind MSG ID */
         MAC_KPI_THP_STATS_IND,            /* Periodic Throughput Stat Ind MSG ID */
         MAC_GET_KPI_THP_STATS_CNF         /* Aperiodic Throughput Stat Ind MSG ID */
        },

        /* KPI_CONTAINER_THP_OAM_PDCP */
	{
         PDCP_MODULE_ID,                   /* SRC MODULE */  
         OAM_MODULE_ID,                    /* DST MODULE */ 
         MAC_API_HEADER_LEN,               /* HEADER LEN */     
         0,                                /* CONFIG CNF MSG ID */       
         pdcpOamSendMessage,               /* SEND FUNC */
         0,                                /* Periodic Stat Ind MSG ID */
         0,                                /* Aperiodic Stat Ind MSG ID */
         PDCP_KPI_THP_STATS_IND,           /* Periodic Throughput Stat Ind MSG ID */
         /* SPR 9680 changes start */
         PDCP_GET_KPI_THP_STATS_RESP        /* Aperiodic Throughput Stat Ind MSG ID */
         /* SPR 9680 changes end */
        },

        /* KPI_CONTAINER_THP_RRM_MAC  */
	{
         MAC_MODULE_ID,                    /* SRC MODULE */   
         RRM_MODULE_ID,                    /* DST MODULE */ 
         L2_RRM_API_HEADER_LEN,            /* HEADER LEN */ 
         0,                                /* CONFIG CNF MSG ID */     
         sendMsgToRRM,                     /* SEND FUNC */
         0,                                /* Periodic Stat Ind MSG ID */
         0,                                /* Aperiodic Stat Ind MSG ID */
         RRM_MAC_KPI_THP_STATS_IND,        /* Periodic Throughput Stat Ind MSG ID */
         RRM_MAC_GET_KPI_THP_STATS_CNF     /* Aperiodic Throughput Stat Ind MSG ID */
        },

        /* KPI_CONTAINER_THP_RRM_PDCP */
	{
         PDCP_MODULE_ID,                    /* SRC MODULE */  
         RRM_MODULE_ID,                     /* DST MODULE */ 
         L2_RRM_API_HEADER_LEN,             /* HEADER LEN */ 
         0,                                 /* CONFIG CNF MSG ID */       
         sendPdcpMsgToRRM,                  /* SEND FUNC */
         0,                                 /* Periodic Stat Ind MSG ID */
         0,                                 /* Aperiodic Stat Ind MSG ID */
         RRM_PDCP_KPI_THP_STATS_IND,        /* Periodic Throughput Stat Ind MSG ID */
         RRM_PDCP_GET_KPI_THP_STATS_CNF     /* Aperiodic Throughput Stat Ind MSG ID */
        }, 

        /* KPI_CONTAINER_THP_SON_MAC  */
	{
         MAC_MODULE_ID,                    /* SRC MODULE */   
         SON_MODULE_ID,                    /* DST MODULE */ 
         MAC_API_HEADER_LEN,               /* HEADER LEN */     
         0,                                /* CONFIG CNF MSG ID */
         NULL,                             /* SEND FUNC */
         0,                                /* Periodic Stat Ind MSG ID */
         0,                                /* Aperiodic Stat Ind MSG ID */
         0,                                /* Periodic Throughput Stat Ind MSG ID */
         0                                 /* Aperiodic Throughput Stat Ind MSG ID */
        },

        /* KPI_CONTAINER_THP_SON_PDCP */
	{
         PDCP_MODULE_ID,                   /* SRC MODULE */  
         SON_MODULE_ID,                    /* DST MODULE */ 
         MAC_API_HEADER_LEN,               /* HEADER LEN */     
         0,                                /* CONFIG CNF MSG ID */
         NULL,                             /* SEND FUNC */
         0,                                /* Periodic Stat Ind MSG ID */
         0,                                /* Aperiodic Stat Ind MSG ID */
         0,                                /* Periodic Throughput Stat Ind MSG ID */
         0                                 /* Aperiodic Throughput Stat Ind MSG ID */
        }
};

#ifdef TDD_CONFIG
/* + KPI_CA TDD */
/*CA TDD CHANGES START*/
RCVD_OAM_KPI_MSG_TYPE rcvdOAMKPIMsg_g[MAX_NUM_CELL];
/*CA TDD CHANGES END*/
extern void processMACGetKpiStatReq(MacGetKpiStatsReqParams *macGetKpiStatsReqParams_p, UInt8 modCounter,
                             UInt8 *cnfBuff_p, UInt16 *currentBuf_p, InternalCellIndex internalCellIndex);
/* - KPI_CA */
/* Flag warning removed*/
#endif

/* SPR 5062 Changes Start */
/* 100MS_SUPPORT : start */
/* + SPR 17439 */
UInt32 checkForKPIUpdateReporting(void);
/* - SPR 17439 */
void distributeKPIStats(UInt32 moduleBitmask);
/* 100MS_SUPPORT : end */
/* SPR 5062 Changes Start */


/* CA Changes start */
void prepareMACApiHeader( UInt8 *msgBuf,
        UInt16 msgId,
        UInt16 destModuleId,
        UInt16 transactionId,
        UInt16 length,
		InternalCellIndex internalCellIndex
)
{
    if(RRM_MODULE_ID == destModuleId)  
    {
        prepareMACRRMApiHeader( (L2RrmMsg *)msgBuf, msgId, transactionId, 
                length, internalCellIndex);
    }
    else 
    {
        prepareMacOamApiHeader(msgBuf, msgId,
                transactionId, length,
		/* SPR 22296 Fix Start */
                internalCellIndex);
		/* SPR 22296 Fix End */
    }
}
/* CA Changes end */

void resetMacThpStats(UInt32 passiveIndex)
{
    if(gMacStats.pStats)
    {
        /*SPR 21551 Fix Start*/ 
        memSet((void *)(&(gMacStats.pStats->lteCommonUeKPIStats.lteMacKPIThpStats[passiveIndex])), 0x0, sizeof(LteMacKPIThpStats));
        /*SPR 21551 Fix End*/ 
        gMacStats.pStats->lteCommonUeKPIStats.lteMacKPIThpStats[passiveIndex].durationTTI = 0;
    }
    return;
}

void resetPdcpThpStats(UInt32 passiveIndex)
{
    if(gMacStats.pStats)
    {
        /*SPR 10021 Fix Start*/
        memSet((void *)(&(gMacStats.pStats->lteCommonUeKPIStats.ltePdcpKPIThpStats[passiveIndex])),
                0x0, sizeof(LtePdcpKPIThpStats));
        /*SPR 10021 Fix End*/
        gMacStats.pStats->lteCommonUeKPIStats.ltePdcpKPIThpStats[passiveIndex].durationTTI = 0;
    }
    return;
}

	/* - KPI_CA */
/* 100MS_SUPPORT : start */
    /* + KPI_CA */   
/* + SPR 17439 */
void reportMacThpStats(void)
/* - SPR 17439 */
{
    UInt32             moduleCounter              = 0;
    /* SPR 10579 changes start*/
    LteKPIThpStatus   *pLteMacKPIThpStatus        = PNULL;
    LteMacKPIConfig   *lteMacKPIConfig_p          = PNULL;  
    LteMacKPIThpStats *lteMacKPIThpStats_p        = PNULL;  
    /* SPR 10579 changes end*/
    /*SPR 15909 fix start*/
    tickType_t             activeMapDurationTTI       = 0;
    /*SPR 15909 fix end*/
    /* SPR 10579 changes start*/
    for(moduleCounter = KPI_CONTAINER_THP_OAM_MAC; 
            (g_kpiScannerObj.module_bitmask && moduleCounter<MAX_KPI_CONTAINER); 
            moduleCounter++)
    {
        /* SPR 10579 changes start*/
        /* + KPI_Review_Comments */
        if(!(MAC_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[moduleCounter].macOrPdcpStats &&
                    g_kpiScannerObj.kpiStatsContainer[moduleCounter].config.lteMacKPIConfig.kpiBitmap))
        {
            continue;
        }
        /* SPR 10579 changes end*/
        pLteMacKPIThpStatus = &(g_kpiScannerObj.kpiStatsContainer[moduleCounter].status.lteMacKPIThpStatus);

        if(!pLteMacKPIThpStatus->servingRespId)
        {
            /* SPR 10579 changes end*/
            lteMacKPIConfig_p = PNULL;
            /* - KPI_Review_Comments */
            if(pLteMacKPIThpStatus->kpiThpStatIndFlag)
            {
                pLteMacKPIThpStatus->servingRespId = MAC_KPI_THP_STATS_IND;
                pLteMacKPIThpStatus->kpiThpStatIndFlag = 0;
                pLteMacKPIThpStatus->moduleBitmap    |= (0x01 << moduleCounter);
            }
            else if(pLteMacKPIThpStatus->kpiGetThpStatRespCount)
            {
                lteMacKPIConfig_p = &(g_kpiScannerObj.kpiStatsContainer[moduleCounter].config.lteMacKPIConfig);
                /* SPR 10579 changes start*/
                lteMacKPIThpStats_p = &(g_kpiScannerObj.kpiStatsContainer[moduleCounter].stats.lteMacKPIThpStats);
                activeMapDurationTTI = lteMacKPIThpStats_p->durationTTI; 
                /* SPR 10579 changes end*/
                if(lteMacKPIConfig_p->durationTTI >= 1000 && 
                        activeMapDurationTTI < 1000)
                {
                    pLteMacKPIThpStatus->servingRespId = WAIT_FOR_1SEC;
                    return;
                } 
                pLteMacKPIThpStatus->servingRespId = MAC_GET_KPI_THP_STATS_CNF;
                pLteMacKPIThpStatus->kpiGetThpStatRespCount--;
                pLteMacKPIThpStatus->moduleBitmap    |= (0x01 << moduleCounter);
            }
            /* SPR 10579 changes start*/
            /*Code Removed*/
            pLteMacKPIThpStatus->startUeIdNxtBatch = 0;
            /* SPR 10579 changes end*/
        }
        if(pLteMacKPIThpStatus->servingRespId && 
                pLteMacKPIThpStatus->servingRespId != WAIT_FOR_1SEC)
        {
/*spr 19296 fix start*/
            sendMacKpiThpStatsInd(moduleCounter);
/*spr 19296 fix end*/
        }
    }
}

/* + SPR 17439 */
void reportPdcpThpStats(void)
/* - SPR 17439 */
{
    UInt32             moduleCounter              = 0;
    /* SPR 10579 changes start*/
    LteKPIThpStatus    *pLtePdcpKPIThpStatus      = PNULL; 
    /* SPR 10579 changes end*/

    for(moduleCounter=KPI_CONTAINER_THP_OAM_PDCP; 
            (g_kpiScannerObj.module_bitmask && moduleCounter<MAX_KPI_CONTAINER); 
            moduleCounter++)
    {
        /* SPR 10579 changes start*/
		/* + KPI_Review_Comments */
		if(!(PDCP_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[moduleCounter].macOrPdcpStats &&
			g_kpiScannerObj.kpiStatsContainer[moduleCounter].config.ltePdcpKPIConfig.kpiBitmap))
        {
           continue;
        }
        pLtePdcpKPIThpStatus = &(g_kpiScannerObj.kpiStatsContainer[moduleCounter].status.ltePdcpKPIThpStatus);
        if(!pLtePdcpKPIThpStatus->servingRespId)
        {
            /* SPR 10579 changes end*/
            /* - KPI_Review_Comments */
            if(pLtePdcpKPIThpStatus->kpiThpStatIndFlag)
            {
                pLtePdcpKPIThpStatus->servingRespId = PDCP_KPI_THP_STATS_IND;
                pLtePdcpKPIThpStatus->kpiThpStatIndFlag = 0;
                pLtePdcpKPIThpStatus->moduleBitmap    |= (0x01 << moduleCounter);
            }
		/* +- SPR 17637 */
            /* SPR 10579 changes start*/
            /*Code Removed*/
            pLtePdcpKPIThpStatus->startUeIdNxtBatch = 0;
            /* SPR 10579 changes end*/
        }
	    if(pLtePdcpKPIThpStatus->servingRespId )
        {
            /* +- SPR 17637 */
            /* SPR 10579 changes start*/
            /* +- SPR 17777 */
            sendPdcpKpiThpStatsInd(moduleCounter);
            /* +- SPR 17777 */
            /* SPR 10579 changes end*/
        }
    }
}

/* - KPI_CA */
/* 100MS_SUPPORT : end */
/****************************************************************************
 * Function Name  : resetAndReportThpStats
 * Inputs         : void
 * Outputs        : None
 * Returns        : void
 * Variables      :
 * Description    : This function resets throughput counters periodically. 
 *                  
 ****************************************************************************/
/* 100MS_SUPPORT : start */
/* + SPR 17439 */
void resetAndReportThpStats(void)
/* - SPR 17439 */
{
	reportMacThpStats();
	reportPdcpThpStats();
}
/* 100MS_SUPPORT : end */
/****************************************************************************
 * Function Name  : calculateBestDuration
 * Inputs         : void
 * Outputs        : None
 * Returns        : finalTTI
 * Variables      :
 * Description    : This function calculates bestTTI that can be used to 
 *                  for updating KPI Container
 ****************************************************************************/
 /* + KPI_CA */
void calculateBestDuration(
        TimeSpec *temp_timeout
        )
{
    UInt32 modCounter;
    /*SPR 15909 fix start*/
    tickType_t localTTI = 0;
    tickType_t leastTTI = MAC_MAX_VAL_KPI_TIMER_T*1000;
    tickType_t durationTTI        = 0;
    /*SPR 15909 fix end*/
    UInt32 activeMacIndex        = 0;
	/* + SPR 10579 Merge Fix */
    UInt32 activePdcpIndex       = 0;
	/* - SPR 10579 Merge Fix */
    /*SPR 15909 fix start*/
    tickType_t activeMacDurationTTI  = 0;
    tickType_t activePdcpDurationTTI = 0;
    /*SPR 15909 fix end*/
    /* SPR 10579 changes start*/
    for(modCounter=0;modCounter<MAX_KPI_CONTAINER;modCounter++)
    {
        /* for throughput time */
        if((g_kpiScannerObj.kpiStatsContainer[modCounter].status.lteMacKPIThpStatus.servingRespId && 
                    WAIT_FOR_1SEC != g_kpiScannerObj.kpiStatsContainer[modCounter].status.lteMacKPIThpStatus.servingRespId)
            || (g_kpiScannerObj.kpiStatsContainer[modCounter].status.ltePdcpKPIThpStatus.servingRespId &&
                 WAIT_FOR_1SEC != g_kpiScannerObj.kpiStatsContainer[modCounter].status.ltePdcpKPIThpStatus.servingRespId)
            )
        {
            /* SPR 5443 */
            leastTTI = 1;
            if(WAIT_FOR_1SEC == g_kpiScannerObj.kpiStatsContainer[modCounter].status.lteMacKPIThpStatus.servingRespId)
            {
                g_kpiScannerObj.kpiStatsContainer[modCounter].status.lteMacKPIThpStatus.servingRespId = 0;
            }
            if(WAIT_FOR_1SEC == g_kpiScannerObj.kpiStatsContainer[modCounter].status.ltePdcpKPIThpStatus.servingRespId) 
            {
                g_kpiScannerObj.kpiStatsContainer[modCounter].status.ltePdcpKPIThpStatus.servingRespId = 0;
            }
        }
        else
        {
            if(WAIT_FOR_1SEC == g_kpiScannerObj.kpiStatsContainer[modCounter].status.lteMacKPIThpStatus.servingRespId 
                || WAIT_FOR_1SEC == g_kpiScannerObj.kpiStatsContainer[modCounter].status.ltePdcpKPIThpStatus.servingRespId
              )
            {
                activeMacIndex  = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForMacThpStats;
				/* + SPR 10579 Merge Fix */
                activePdcpIndex  = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats;
                activeMacDurationTTI = gMacStats.pStats->lteCommonUeKPIStats.lteMacKPIThpStats[activeMacIndex].durationTTI;
                activePdcpDurationTTI = gMacStats.pStats->lteCommonUeKPIStats.ltePdcpKPIThpStats[activePdcpIndex].durationTTI;
				/* - SPR 10579 Merge Fix */
                
				if(WAIT_FOR_1SEC == g_kpiScannerObj.kpiStatsContainer[modCounter].status.lteMacKPIThpStatus.servingRespId)
                {
                    leastTTI = 1000 - activeMacDurationTTI;
                    g_kpiScannerObj.kpiStatsContainer[modCounter].status.lteMacKPIThpStatus.servingRespId = 0;
                }
                if(WAIT_FOR_1SEC == g_kpiScannerObj.kpiStatsContainer[modCounter].status.ltePdcpKPIThpStatus.servingRespId) 
                {
                    if(leastTTI > (1000 - activePdcpDurationTTI))
                    {
                        leastTTI = 1000 - activePdcpDurationTTI;
                    }
                    g_kpiScannerObj.kpiStatsContainer[modCounter].status.ltePdcpKPIThpStatus.servingRespId = 0;
                }
            }
            localTTI = 0;
    /* SPR 10579 changes end*/
            /* 100MS_SUPPORT : start */
            if ((MAC_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats) ||
                    (MAC_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats))
            {
                durationTTI = (g_kpiScannerObj.kpiStatsContainer[modCounter].config.lteMacKPIConfig.durationTTI);
            }

            else if ((PDCP_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats) ||
                    (PDCP_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats))
            {
                durationTTI = (g_kpiScannerObj.kpiStatsContainer[modCounter].config.ltePdcpKPIConfig.durationTTI);
            }

            if((g_kpiScannerObj.module_bitmask & (1<<modCounter))&& durationTTI > 0)
            {
                localTTI = g_kpiScannerObj.timeout_tti[modCounter];
            }
            if(localTTI && (leastTTI > localTTI))
            {
                leastTTI = localTTI;
            }
        } 
        /* 100MS_SUPPORT : end */
    }
    /* SPR 5443 */
    /* SPR 5675 fix starts*/

    if( FALSE == rlcInitInd_g 
        || FALSE == pdcpInitInd_g
      )
    {
        leastTTI=1000;
    }

    /* SPR 5675 fix ends*/

    /* SPR 14329 Changes Start */
    if(insessionConfigFlag_g)
    {
        /*SPR 15909 fix start*/
        tickType_t diff = (SAMPLING_TIME_FOR_KPI_IN_SESSION + g_kpiScannerObj.prevKpiInSessionTick) - macTTITickCount_g;
        /*SPR 15909 fix end*/
        if(leastTTI > diff)
        {
            leastTTI = diff;
        }
    }

    temp_timeout->tv_sec = (leastTTI) / 1000;
    temp_timeout->tv_nsec = (leastTTI) % 1000;
    temp_timeout->tv_nsec = (unsigned long) (temp_timeout->tv_nsec * 1000000);
}

/****************************************************************************
 * Function Name  : processStatsBuffBeforeConfig
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Variables      :
 * Description    : This function will reset or update depending upon the new bitmask
 *                  of mac and configuration of other modules bitmask.
 ****************************************************************************/
/* + KPI_CA */
void processStatsBuffBeforeConfig(UInt16 macBitmask, UInt8 pdcpBitmask,
			InternalCellIndex internalCellIndex)
{
	UInt32 counter           = 0;
	UInt32 isMacConfigured   = 0;
	UInt32 isPdcpConfigured  = 0;
    /* SPR 10579 changes start*/
    UInt32 isMacThpConfigured   = 0;
    UInt32 isPdcpThpConfigured  = 0;
    /* SPR 10579 changes end*/

	if(!macBitmask && !pdcpBitmask) return;

	for(counter=0; (macBitmask && counter<MAX_KPI_CONTAINER); counter++)
	{
        /* SPR 10579 changes start*/
        UInt16 macBitmaskConfig = g_kpiScannerObj.kpiStatsContainer[counter].config.lteMacKPIConfig.kpiBitmap;
        if( MAC_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[counter].macOrPdcpStats && macBitmaskConfig)
	{
		isMacConfigured=1;
        /* SPR 10579 changes end*/
        }
        /* SPR 10579 changes start*/
        else if( MAC_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[counter].macOrPdcpStats &&
                 IS_KPI_MAC_BITSET(macBitmaskConfig, KPI_MAC_DL_LC_THROUGHPUT))
        {
            isMacThpConfigured=1;
        }
        /* SPR 10579 changes end*/
	}

	for(counter=0; (pdcpBitmask && counter<MAX_KPI_CONTAINER); counter++)
	{
       /* SPR 10579 changes start*/
        UInt8 pdcpBitmaskConfig = g_kpiScannerObj.kpiStatsContainer[counter].config.ltePdcpKPIConfig.kpiBitmap;
        if(PDCP_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[counter].macOrPdcpStats && pdcpBitmaskConfig)
        {
            isPdcpConfigured=1;
        /* SPR 10579 changes end*/
        }
        /* SPR 10579 changes start*/
        else if(PDCP_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[counter].macOrPdcpStats && 
                (IS_KPI_PDCP_BITSET(pdcpBitmaskConfig, KPI_PDCP_DL_LC_THROUGHPUT) ||
                 IS_KPI_PDCP_BITSET(pdcpBitmaskConfig, KPI_PDCP_UL_LC_THROUGHPUT)))
        {
            isPdcpThpConfigured=1;
        }
        /* SPR 10579 changes end*/
	}

	if(macBitmask && !isMacConfigured)
	{
		/* reset mac stats buffer and make active = 0 */
		macResetKpiCounters(0,internalCellIndex);
        /* SPR 10579 changes start*/
        macResetKpiCounters(1, internalCellIndex);
        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats = 0;
        /* SPR 10579 changes end*/
	}
   /* SPR 10579 changes start*/
    if(IS_KPI_MAC_BITSET(macBitmask, KPI_MAC_DL_LC_THROUGHPUT) &&
            !isMacThpConfigured)
    {
        /* reset mac thp stats buffer and make active = 0 */
        resetMacThpStats(0);
        resetMacThpStats(1);
        gMacStats.pStats->lteCommonUeKPIStats.activeIndexForMacThpStats = 0;
    }

	if(pdcpBitmask && !isPdcpConfigured)
    /* SPR 10579 changes end*/
	{
		/* reset PDCP stats buffer and make active = 0 */
		pdcpResetKpiCounters(0,internalCellIndex);
        /* SPR 10579 changes start*/
        pdcpResetKpiCounters(1, internalCellIndex);
        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats = 0;
        /* SPR 10579 changes end*/
	}
   /* SPR 10579 changes start*/
    if((IS_KPI_PDCP_BITSET(pdcpBitmask, KPI_PDCP_DL_LC_THROUGHPUT) ||
                IS_KPI_PDCP_BITSET(pdcpBitmask, KPI_PDCP_UL_LC_THROUGHPUT)) &&
            !isPdcpThpConfigured)
    {
        /* reset pdcp thp stats buffer and make active = 0 */
        resetPdcpThpStats(0);
        resetPdcpThpStats(1);
        gMacStats.pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats = 0;
    }

    if(isMacConfigured 
        || isPdcpConfigured 
        || isMacThpConfigured 
        || isPdcpThpConfigured
        )
        /* SPR 10579 changes end*/
	{
		/* Update the KPI Containers*/
		updateStatsForAllModules(internalCellIndex);
	}
}
/* - KPI_CA */


/****************************************************************************
 * Function Name  : handleMACConfigureModKpiStatReq
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Variables      :
 * Description    : This function will handle KPI Config Req from OAM
 ****************************************************************************/
/* 100MS_SUPPORT : start */
/* + KPI_CA */
UInt16 handleMACConfigureModKpiStatReq(UInt8 *msgBuf_p,  UInt16 transactionId, UInt8 moduleId)
{
    UInt8 cellIndex = 0;
    UInt16 respCode = MAC_SUCCESS;
    LteMacKPIConfig *pKpiConfig = PNULL;
    LteMacKPIConfig *pKpiTHPConfig = PNULL;
    UInt8 moduleTHPId = 0;
    /* SPR 10579 changes start*/
    LteKPIThpStatus *pLteMacKPIThpStatus    = &(g_kpiScannerObj.kpiStatsContainer[moduleId].status.lteMacKPIThpStatus);
    /* SPR 10579 changes end*/
    MacConfigureKpiStatsReqParams *macConfigureKpiStatsReqParams_p = PNULL;
    /* coverity 52371 fix*/
    UInt32 duration = 0;
#ifdef TDD_CONFIG
    UInt32    modCounter           = 0;
    /* SPR 15909 fix start */
    tickType_t    curr_tick        = 0;
    /* SPR 15909 fix end */
    UInt32    diff                 = 0;
#endif
    /* coverity 52371 fix*/

    pKpiConfig = &(g_kpiScannerObj.kpiStatsContainer[moduleId].config.lteMacKPIConfig);
    if(KPI_CONTAINER_RRM_MAC == moduleId)
    {
        macConfigureKpiStatsReqParams_p = (MacConfigureKpiStatsReqParams *)(msgBuf_p);
        moduleTHPId = KPI_CONTAINER_THP_RRM_MAC;
    }
    else if(KPI_CONTAINER_OAM_MAC == moduleId)
    {
        macConfigureKpiStatsReqParams_p = (MacConfigureKpiStatsReqParams *)(msgBuf_p + g_kpiModuleInfo[moduleId].hdrLen);
        moduleTHPId = KPI_CONTAINER_THP_OAM_MAC;
    }
    else if(KPI_CONTAINER_SON_MAC == moduleId)
    {
        macConfigureKpiStatsReqParams_p = (MacConfigureKpiStatsReqParams *)(msgBuf_p + g_kpiModuleInfo[moduleId].hdrLen);
        moduleTHPId = KPI_CONTAINER_THP_SON_MAC;
    }

    pKpiTHPConfig =  &(g_kpiScannerObj.kpiStatsContainer[moduleTHPId].config.lteMacKPIConfig);

    /*coverity 52372*/
    if(!macConfigureKpiStatsReqParams_p)
    {
        return MAC_FAILURE;
    }
    /*coverity 52372*/
    /*SPR_9013_FIX*/
    /* coverity fix 10056 + */
    if((macConfigureKpiStatsReqParams_p->durationT < 1) ||(macConfigureKpiStatsReqParams_p->durationT > 3600))
    /* coverity fix 10056 - */
    {
        return MAC_FAILURE;
    }
    /*SPR_9013_FIX*/

    if(macConfigureKpiStatsReqParams_p->durationT <= 3600)  
    {
        duration = (macConfigureKpiStatsReqParams_p->durationT) * 1000;
    }
    else 
    {
        duration = (macConfigureKpiStatsReqParams_p->durationT) - KPI_CONFIG_ENUM_BASE;
    }

    LTE_MAC_UT_LOG(LOG_DETAIL,PARSE_OAM_MSG,
            "parseMACInitLayerReq : durationT = %u\n periodicReporting = %u\n kpiToReportBitMap= %u\n ",
            macConfigureKpiStatsReqParams_p->durationT, macConfigureKpiStatsReqParams_p->periodicReporting, 
            macConfigureKpiStatsReqParams_p->kpiToReportBitMap);
#ifdef TDD_CONFIG
    /* coverity 52371 fix*/
    /****Reseting timer list : start****/
    /* + SPR_12546_12547 */
    curr_tick  = macTTITickCount_g;
    /* - SPR_12546_12547 */
    diff       = curr_tick - g_kpiScannerObj.prevkpi_tick;
    /* + KPI_CA */
    for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
    {
        gMacStats.pStats->lteCommonKPIStats[cellIndex]. \
            lteMacKPIStats[gMacStats.pStats->lteCommonKPIStats[cellIndex].activeIndexForMacStats].lastCalculatedTTI   += diff;
        gMacStats.pStats->lteCommonKPIStats[cellIndex]. \
            ltePdcpKPIStats[gMacStats.pStats->lteCommonKPIStats[cellIndex].activeIndexForPdcpStats].lastCalculatedTTI += diff;
    }
    gMacStats.pStats->lteCommonUeKPIStats. \
        lteMacKPIThpStats[gMacStats.pStats->lteCommonUeKPIStats.activeIndexForMacThpStats].durationTTI += diff;
    gMacStats.pStats->lteCommonUeKPIStats. \
        ltePdcpKPIThpStats[gMacStats.pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats].durationTTI += diff;
    /* - KPI_CA */

    for(modCounter=0;modCounter<MAX_KPI_CONTAINER;modCounter++)
    {
        if(g_kpiScannerObj.module_bitmask & (1<<modCounter))
        {
            g_kpiScannerObj.timeout_tti[modCounter]-=diff;
        }
    }
    g_kpiScannerObj.prevkpi_tick = curr_tick;
    /*******Reseting timer list : end ****/
    /* coverity 52371 fix*/
#endif 

    if(macConfigureKpiStatsReqParams_p->kpiToReportBitMap)
    {
        /* + KPI_CA */
        for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
        {
            processStatsBuffBeforeConfig(macConfigureKpiStatsReqParams_p->kpiToReportBitMap, 0,cellIndex);
        }
        /* - KPI_CA */

        if (macConfigureKpiStatsReqParams_p->kpiToReportBitMap & CELL_SPECIFIC_MAC_COUNTERS_BITMAP)
        {
            pKpiConfig->kpiBitmap = 
                   (macConfigureKpiStatsReqParams_p->kpiToReportBitMap & CELL_SPECIFIC_MAC_COUNTERS_BITMAP);
            g_kpiScannerObj.module_bitmask  |= (0x01 << moduleId);

            pKpiConfig->kpiConfigured = 1;
            pKpiConfig->durationTTI  = duration;
            pKpiConfig->periodicReporting = macConfigureKpiStatsReqParams_p->periodicReporting;
            pKpiConfig->transactionId     = transactionId;
            g_kpiScannerObj.kpiStatsContainer[moduleId].macOrPdcpStats = MAC_KPI_STATS;

            for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
            {
                resetKpiContainer(moduleId, cellIndex);
            }
            g_kpiScannerObj.timeout_tti[moduleId] = pKpiConfig->durationTTI;
            /* SPR 13492 Changes Start */
            g_kpiScannerObj.timeoutInSessionTTI[moduleId] = SAMPLING_TIME_FOR_KPI_IN_SESSION;
            /* SPR 13492 Changes End */

            /* SPR 14329 Changes Start */
            if(IS_KPI_MAC_BITSET(pKpiConfig->kpiBitmap, KPI_IN_SESSION_ACTIVITY_TIME))
            {
                insessionConfigFlag_g = 1;
            }
            else if((!(IS_KPI_MAC_BITSET(g_kpiScannerObj.kpiStatsContainer[KPI_CONTAINER_RRM_MAC].config.lteMacKPIConfig.kpiBitmap, KPI_IN_SESSION_ACTIVITY_TIME))) && (!(IS_KPI_MAC_BITSET(g_kpiScannerObj.kpiStatsContainer[KPI_CONTAINER_OAM_MAC].config.lteMacKPIConfig.kpiBitmap, KPI_IN_SESSION_ACTIVITY_TIME))))
            {
                insessionConfigFlag_g = 0; 
            }
            /* SPR 14329 Changes End */
        }
        else
        {
            g_kpiScannerObj.module_bitmask &= ~(0x01 << moduleId);
            pKpiConfig->kpiConfigured = 0;
        }

        /* Updating throughput stats container */  
        if(IS_KPI_MAC_BITSET(macConfigureKpiStatsReqParams_p->kpiToReportBitMap, KPI_MAC_DL_LC_THROUGHPUT))
        {
            pKpiTHPConfig->durationTTI = duration;
            /* Check if throughput stats exceeds its lower limit */
            /* SPR 10579 changes start*/
            memSet((void *)((pLteMacKPIThpStatus)), 0x00, sizeof(LteKPIThpStatus));

            /* SPR 10579 changes end*/
            pKpiTHPConfig->kpiBitmap = 
                    (macConfigureKpiStatsReqParams_p->kpiToReportBitMap & (1 << KPI_MAC_DL_LC_THROUGHPUT));
            pKpiTHPConfig->kpiConfigured = 1;
            g_kpiScannerObj.module_bitmask  |= (0x01 << moduleTHPId);
            g_kpiScannerObj.timeout_tti[moduleTHPId] = pKpiTHPConfig->durationTTI;
            pKpiTHPConfig->periodicReporting = macConfigureKpiStatsReqParams_p->periodicReporting;
            pKpiTHPConfig->transactionId     = transactionId;
            g_kpiScannerObj.kpiStatsContainer[moduleTHPId].macOrPdcpStats = MAC_KPI_THP_STATS;

            resetKpiContainer(moduleTHPId, 0); /* Here cell index 0 is passed because THP stats are UE specific and do not use cell index for reset */
        }
        else
        {
            g_kpiScannerObj.module_bitmask &= ~(0x01 << moduleTHPId);
            pKpiTHPConfig->kpiConfigured = 0;
        }
        /* 100MS_SUPPORT : end */
    }
    else
    {
        g_kpiScannerObj.module_bitmask &= ~(0x01 << moduleId);
        g_kpiScannerObj.module_bitmask &= ~(0x01 << moduleTHPId);
        pKpiConfig->kpiConfigured = 0;
        pKpiTHPConfig->kpiConfigured = 0;
    }
#ifdef TDD_CONFIG
    eventfd_write_wrapper(pdcpOamMsgEventFd_g,1);
#endif
    return respCode;
}

/* 100MS_SUPPORT : end */
/*SPR 16855 +-*/
/* - KPI_CA */
/****************************************************************************
 * Function Name  : handleMACConfigureKpiStatReq
 * Inputs         : msgBuf_p - Pointer to msg read from socket
 *                  transactionId - this will be used to send periodic stats_ind   
 * Outputs        : None
 * Returns        : MAC_SUCCESS or FAILURE
 * Variables      :
 * Description    : This API parses the KPI configure requests and if request
 *                  is successful then updates KPI configuration and resets
 *                  Cumulative counters
 ****************************************************************************/
UInt16 handleMACConfigureKpiStatReq(UInt8 *msgBuf_p,  UInt16 transactionId_p, UInt8 moduleId)
{
    UInt16 respCode = MAC_SUCCESS;

    if(!(g_kpiScannerObj.module_bitmask))
    {
         /* + SPR_12546_12547 */
         g_kpiScannerObj.prevkpi_tick = macTTITickCount_g;
         /* - SPR_12546_12547 */
         /* SPR 14329 Change ---Code Removed */ 
    }
    /* SPR 14329 Changes Start */
    if(!(insessionConfigFlag_g))
    {
        g_kpiScannerObj.prevKpiInSessionTick = macTTITickCount_g;
    }
    /* SPR 14329 Changes End */
    respCode = handleMACConfigureModKpiStatReq(msgBuf_p,transactionId_p, moduleId);
    return respCode;
}


/****************************************************************************
 * Function Name  : checkForKPIUpdateReporting
 * Inputs         : none
 * Outputs        : none
 * Returns        : modCounter - 0xFF is invalid module id
 * Variables      :
 * Description    : This function will return module for which Config KPI Ind
 *                  needs to be triggered
 ****************************************************************************/
	/* + KPI_CA */
/* 100MS_SUPPORT : start  */
/* + SPR 17439 */
UInt32 checkForKPIUpdateReporting(void)
/* - SPR 17439 */
{
    UInt32 retVal     = 0x00;
/* 100MS_SUPPORT : end  */
    UInt32 modCounter = 0;
	/* KPI_CA Review */
    /* + SPR_12546_12547 */
    tickType_t curr_tick  = macTTITickCount_g;
    /* - SPR_12546_12547 */
	/* - KPI_CA Review */
    UInt32 diff       = 0 ;
	LteMacKPIStats *pLteMacKPIStats = PNULL ;
	LtePdcpKPIStats *pLtePdcpKPIStats = PNULL ;
	LteMacKPIThpStats *pActiveLteMacKPIThpStats = PNULL ;
	LtePdcpKPIThpStats *pActiveLtePdcpKPIThpStats = PNULL ;
	UInt32 activeIndexForMacStats = 0;
	UInt32 activeIndexForPdcpStats = 0;
	UInt32  activeIndexForMacThpStats = 0;
	UInt32  activeIndexForPdcpThpStats = 0;
	InternalCellIndex cellIndex = 0;
    /* SPR 13492 Changes Start */
    UInt32 diffInSession       = 0;
    UInt16 ueIndex             = INVALID_UE_INDEX;
    UInt8 qci                  = INVALID_QCI;
    UInt8 ueCountFlag          = FALSE;
    /* SPR 13492 Changes End */

	diff = curr_tick - g_kpiScannerObj.prevkpi_tick;
	for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
	{
		activeIndexForMacStats = (gMacStats.pStats->lteCommonKPIStats[cellIndex].activeIndexForMacStats);
		activeIndexForPdcpStats = (gMacStats.pStats->lteCommonKPIStats[cellIndex].activeIndexForPdcpStats);
		pLteMacKPIStats =
			&(gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[activeIndexForMacStats]);
		pLtePdcpKPIStats =
			&(gMacStats.pStats->lteCommonKPIStats[cellIndex].ltePdcpKPIStats[activeIndexForPdcpStats]);

    pLteMacKPIStats->lastCalculatedTTI  += diff;
    pLtePdcpKPIStats->lastCalculatedTTI += diff;
	}
	
	
	activeIndexForMacThpStats  = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForMacThpStats;
	activeIndexForPdcpThpStats  = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats;
	pActiveLteMacKPIThpStats   = &(gMacStats.pStats->lteCommonUeKPIStats.lteMacKPIThpStats[activeIndexForMacThpStats]);
	pActiveLtePdcpKPIThpStats   = &(gMacStats.pStats->lteCommonUeKPIStats.ltePdcpKPIThpStats[activeIndexForPdcpThpStats]);


    pActiveLteMacKPIThpStats->durationTTI += diff;
    pActiveLtePdcpKPIThpStats->durationTTI += diff;

    for(modCounter=0;modCounter<MAX_KPI_CONTAINER;modCounter++)
    {
        /* + SPR_12546_12547 */
        if((g_kpiScannerObj.module_bitmask & (1<<modCounter)) &&
                (0>=(SInt32)(g_kpiScannerObj.timeout_tti[modCounter]-=diff))
          )
       /* - SPR_12546_12547 */
        {
            /* 100MS_SUPPORT : start */
            if((( MAC_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats) || 
                ( MAC_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats)) &&
               ( g_kpiScannerObj.kpiStatsContainer[modCounter].config.lteMacKPIConfig.periodicReporting))
            {
                g_kpiScannerObj.timeout_tti[modCounter] = \
                        (g_kpiScannerObj.kpiStatsContainer[modCounter].config.lteMacKPIConfig.durationTTI);
				retVal |= (1<<modCounter);
            }
            else if ((( PDCP_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats) ||
                      (PDCP_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats)) &&
                     ( g_kpiScannerObj.kpiStatsContainer[modCounter].config.ltePdcpKPIConfig.periodicReporting))
            {
                g_kpiScannerObj.timeout_tti[modCounter] = \
                         (g_kpiScannerObj.kpiStatsContainer[modCounter].config.ltePdcpKPIConfig.durationTTI);
				retVal |= (1<<modCounter);
            }
            /* 100MS_SUPPORT : end */
        }
    }

    /* SPR 13492 Changes Start */
    /* SPR 13947 Changes Start */
    /* SPR 14329 Changes Start */
    if(insessionConfigFlag_g)
    {
        diffInSession = curr_tick - g_kpiScannerObj.prevKpiInSessionTick;
        if(diffInSession >= SAMPLING_TIME_FOR_KPI_IN_SESSION)
        {
            g_kpiScannerObj.prevKpiInSessionTick = curr_tick; 
            for (modCounter=0; modCounter<MAX_KPI_CONTAINER; ++modCounter)
            {
                if (g_kpiScannerObj.module_bitmask & (1<<modCounter)) 
                {
                    if(IS_KPI_MAC_BITSET(g_kpiScannerObj.kpiStatsContainer[modCounter].
                                config.lteMacKPIConfig.kpiBitmap, KPI_IN_SESSION_ACTIVITY_TIME))
                    {
                        for (ueIndex = 0; ueIndex < MAX_UE_SUPPORTED; ++ueIndex)
                        {
                            /*+SPR 20127 fix+*/
                            if(dlUECtxInfoArr_g[ueIndex].dlUEContext_p == PNULL)
                            {
                                continue;
                            }
                            /*-SPR 20127 fix-*/ 
                            for (qci = 0; qci < MAX_QCI; ++qci)
                            {
                                if ((TRUE == ulUECtxInfoArr_g[ueIndex].inSessionTimePerQCIFlag[qci]) || 
                                        (TRUE == dlUECtxInfoArr_g[ueIndex].inSessionTimePerQCIFlag[qci]))
                                {
                                    DLUEContext *ueDLContext_p  = PNULL;
                                    ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                                    if (PNULL != ueDLContext_p)
                                    {
                                        activeIndexForMacStats = 
                                            (gMacStats.pStats->lteCommonKPIStats[ueDLContext_p->internalCellIndex].
                                             activeIndexForMacStats);
                                        /* Check to increment counter only once for In session Activtiy
                                         *  time ue counter */ 
                                        if (FALSE == ueCountFlag)
                                        {
                                            gMacStats.pStats->lteCommonKPIStats[ueDLContext_p->internalCellIndex].
                                                lteMacKPIStats[activeIndexForMacStats].totalSessionTimeUE++;
                                            ueCountFlag = TRUE;
                                        }
                                        gMacStats.pStats->lteCommonKPIStats[ueDLContext_p->internalCellIndex].
                                            lteMacKPIStats[activeIndexForMacStats].totalSessionTimePerQCI[qci]++;
                                    }
                                    /* reset the flag */
                                    ulUECtxInfoArr_g[ueIndex].inSessionTimePerQCIFlag[qci] = FALSE;
                                    dlUECtxInfoArr_g[ueIndex].inSessionTimePerQCIFlag[qci] = FALSE; 
                                }
                            }/* qci loop ends here */
                            ueCountFlag = FALSE;
                        }/* UE Index loop ends here */
                    }
                }
            }/* Module Counter loop ends here */
        }
    }/* In-Session Flag check */
    /* SPR 14329 Changes End */
    /* SPR 13947 Changes End */
    /* SPR 13492 Changes End */
    g_kpiScannerObj.prevkpi_tick = curr_tick;
    return retVal;
}
	/* - KPI_CA */


/****************************************************************************
 * Function Name  : sendKPIStatsInd
 * Inputs         : moduleId
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API will send MAC_KPI_STATS_IND and PDCP_KPI_STATS_IND
 *                  with KPI cumulative counters maintained at MAC and PDCP
 ****************************************************************************/
/* + KPI_CA */
/* SPR 12341 changes start */
void sendKPIStatsInd(UInt8 moduleId, InternalCellIndex internalCellIndex)
/* SPR 12341 changes end */
{
    UInt8* cnfBuff_p = PNULL;
    UInt16                 current                      = 0;
    /* SPR 15909 fix start */
    tickType_t             durationTTIMac               = 0;
    /* SPR 15909 fix end */
    LteMacKPIStats        *pLteMacKPIStats              = PNULL;
    LteMacKPIConfig       *pLteMacKPIConfig             = PNULL;
    LtePdcpKPIStats       *pLtePdcpKPIStats             = PNULL;
    LtePdcpKPIConfig      *pLtePdcpKPIConfig            = PNULL;
    LteKPIThpStatus       *pLteMacKPIThpStatus          = PNULL;
    LteKPIThpStatus       *pLtePdcpKPIThpStatus         = PNULL;
        cnfBuff_p = (UInt8 *)getMemFromPool(MAX_ARRAY_DATA_SIZE,NULL);

        if(PNULL == cnfBuff_p)
        {
            lteWarning("In %s, MEMORY ALLOC FAILURE\n",__func__);
            return;
        }
    UInt8 *resp_p = cnfBuff_p;
    pLteMacKPIConfig   = &(g_kpiScannerObj.kpiStatsContainer[moduleId].config.lteMacKPIConfig);
    pLtePdcpKPIConfig  = &(g_kpiScannerObj.kpiStatsContainer[moduleId].config.ltePdcpKPIConfig);

    if( MAC_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[moduleId].macOrPdcpStats)
    {
        pLteMacKPIStats    = &(g_kpiScannerObj.kpiStatsContainer[moduleId].stats.lteMacKPIStats[internalCellIndex]);
        durationTTIMac     = pLteMacKPIStats->lastCalculatedTTI;
        if(!pLteMacKPIConfig->periodicReporting)
        {
            freeMemPool(cnfBuff_p);
            return;
        }
        current += g_kpiModuleInfo[moduleId].hdrLen;
        if(PNULL != ( resp_p + g_kpiModuleInfo[moduleId].hdrLen)) 
        {
            macGetKpiStats(( resp_p + g_kpiModuleInfo[moduleId].hdrLen), &current, pLteMacKPIConfig->kpiBitmap, pLteMacKPIStats,durationTTIMac);
        }	
        prepareMACApiHeader( resp_p, g_kpiModuleInfo[moduleId].periodicIndMsgType, 
                g_kpiModuleInfo[moduleId].dstModuleId, 
                pLteMacKPIConfig->transactionId, current,internalCellIndex); /* + KPI_CA Review */

        if(!(g_kpiModuleInfo[moduleId].sendToModule) || 
                (MAC_SEND_ERROR == g_kpiModuleInfo[moduleId].sendToModule( cnfBuff_p, current
                                                                         ))) 
        {
            ALARM_MSG (g_kpiModuleInfo[moduleId].srcModuleId, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        }
    }
    else if(( MAC_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[moduleId].macOrPdcpStats) &&
            (IS_KPI_MAC_BITSET(pLteMacKPIConfig->kpiBitmap, KPI_MAC_DL_LC_THROUGHPUT)))
    { 
        pLteMacKPIThpStatus = &(g_kpiScannerObj.kpiStatsContainer[moduleId].status.ltePdcpKPIThpStatus);  
        pLteMacKPIThpStatus->kpiThpStatIndFlag = 1;
        pLteMacKPIThpStatus->moduleBitmap = 0;
        pLteMacKPIThpStatus->startUeIdNxtBatch = 0;
        pLteMacKPIThpStatus->servingRespId = 0;
    }

    else if ( PDCP_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[moduleId].macOrPdcpStats)
    {
        pLtePdcpKPIStats   = &(g_kpiScannerObj.kpiStatsContainer[moduleId].stats.ltePdcpKPIStats[internalCellIndex]);
        if(!pLtePdcpKPIConfig->periodicReporting)
        {
            freeMemPool(cnfBuff_p);
            return;
        }
        if(IS_KPI_PDCP_BITSET(pLtePdcpKPIConfig->kpiBitmap, KPI_TOTAL_DL_DRB_DISCARD_RATE) ||
                IS_KPI_PDCP_BITSET(pLtePdcpKPIConfig->kpiBitmap, KPI_TOTAL_UL_DRB_LOSS_RATE) ||
                IS_KPI_PDCP_BITSET(pLtePdcpKPIConfig->kpiBitmap, KPI_PDCP_DL_CTR_SDU_BITRATE) ||
                IS_KPI_PDCP_BITSET(pLtePdcpKPIConfig->kpiBitmap, KPI_PDCP_UL_CTR_SDU_BITRATE))
        {
            sendPdcpKpiStatsInd(pLtePdcpKPIStats, pLtePdcpKPIConfig, &g_kpiModuleInfo[moduleId],internalCellIndex);
        }
    }
    else  if((PDCP_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[moduleId].macOrPdcpStats) &&
            ((IS_KPI_PDCP_BITSET(pLtePdcpKPIConfig->kpiBitmap, KPI_PDCP_DL_LC_THROUGHPUT)) ||
             (IS_KPI_PDCP_BITSET(pLtePdcpKPIConfig->kpiBitmap, KPI_PDCP_UL_LC_THROUGHPUT))))
    { 
        pLtePdcpKPIThpStatus = &(g_kpiScannerObj.kpiStatsContainer[moduleId].status.ltePdcpKPIThpStatus);
        pLtePdcpKPIThpStatus->kpiThpStatIndFlag = 1;
        pLtePdcpKPIThpStatus->moduleBitmap = 0;
        pLtePdcpKPIThpStatus->startUeIdNxtBatch = 0;
        pLtePdcpKPIThpStatus->servingRespId = 0;
    }

    /* 100MS_SUPPORT : end */
        freeMemPool(cnfBuff_p);
    LOG_MAC_MSG(MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,
            getCurrentTick(),
            DEFAULT_INT_VALUE,MAC_KPI_STATS_IND,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_OAM_RES_MSG_ID");
}

/**************************************************************************************
 * Function Name  : resetKpiContainer
 * Inputs         : moduleId for KPI
 * Outputs        : None
 * Returns        : void
 * Variables      :
 * Description    : This function will reset KPI Stats Container for specified module
 **************************************************************************************/
void resetKpiContainer(UInt8 moduleId, InternalCellIndex internalCellIndex)
{
    if ( PDCP_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[moduleId].macOrPdcpStats)
    {
       memSet(&(g_kpiScannerObj.kpiStatsContainer[moduleId].stats.ltePdcpKPIStats[internalCellIndex]), 0, sizeof(LtePdcpKPIStats));
    }
    else 
    if ( MAC_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[moduleId].macOrPdcpStats)
    {
       memSet(&(g_kpiScannerObj.kpiStatsContainer[moduleId].stats.lteMacKPIStats[internalCellIndex]), 0, sizeof(LteMacKPIStats));
    }
    /* SPR 10579 changes start*/
    else if ( PDCP_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[moduleId].macOrPdcpStats)
    {
        /* SPR 10579 changes start */
        memSet(&(g_kpiScannerObj.kpiStatsContainer[moduleId].stats.ltePdcpKPIThpStats), 0x0, sizeof(LtePdcpKPIThpStats));
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
        /* SPR 10579 changes end */
        g_kpiScannerObj.kpiStatsContainer[moduleId].stats.ltePdcpKPIThpStats.durationTTI = 0;
    }
    else if ( MAC_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[moduleId].macOrPdcpStats)
    {
        /*SPR 21551 Fix Start*/
        memSet(&(g_kpiScannerObj.kpiStatsContainer[moduleId].stats.lteMacKPIThpStats), 0x0, sizeof(LteMacKPIThpStats));
        /*SPR 21551 Fix End*/
        g_kpiScannerObj.kpiStatsContainer[moduleId].stats.lteMacKPIThpStats.durationTTI = 0;
    }
    /* SPR 10579 changes end*/
}


/****************************************************************************
 * Function Name  : sendKPIStats
 * Inputs         : moduleId
 * Outputs        : none
 * Returns        : 
 * Variables      :
 * Description    : 
 *                
 ****************************************************************************/
/* + KPI_CA */
/* 100MS_SUPPORT : start */
void sendKPIStats(UInt32 moduleBitmask, InternalCellIndex internalCellIndex)
/* 100MS_SUPPORT : end */
{
/* SPR 12341 changes start */
    UInt8 moduleId = 0;
/* SPR 12341 changes end */

    while (moduleBitmask)
    {
        /* + SPR 11125 Fix */
        if (moduleBitmask & 1)
        {
                sendKPIStatsInd(moduleId, internalCellIndex);
            /* SPR 10579 changes start*/
            if ((0x1 << moduleId)& CELL_SPECIFIC_KPI)
            {
                    resetKpiContainer(moduleId, internalCellIndex);
            }
            /* SPR 10579 changes end*/
        }
        /* - SPR 11125 Fix */
        moduleId++;
        moduleBitmask >>= 1;
    }
}
/* - KPI_CA */

/****************************************************************************
 * Function Name  : distributeKPIStats
 * Inputs         : processingAllowed
 * Outputs        : none
 * Returns        : 
 * Variables      :
 * Description    : This function will be called on timeout and update
 *                  KPI Container and send Config KPI ind to trigger Module
 ****************************************************************************/
/* 100MS_SUPPORT : start */
/* + KPI_CA */
void distributeKPIStats(UInt32 moduleBitmask)
{  
	InternalCellIndex cellIndex = 0;
/* SPR 10579 Changes start */
		for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
		{
            updateStatsForAllModules(cellIndex);
		}
/* SPR 10579 Changes end */
/* 100MS_SUPPORT : end */
	for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
	{
        sendKPIStats(moduleBitmask,cellIndex);
	}
}
/* - KPI_CA */

/****************************************************************************
 * Function Name  : macResetModuleKpiCounters
 * Inputs         : localStats - pointer to stats to be reset
 *                  size - Structure size to be reset
 * Outputs          None
 * Returns        : None
 * Variables      :
 * Description    : This API will reset all the KPI cumulative Module specific
 *                  counters maintained at MAC.
 ****************************************************************************/

void  macResetModuleKpiCounters(void *localStats, UInt32 size)
{
    if(gMacStats.pStats)
        memSet(localStats, 0, size); /* COVERITY 29820 */
    return;
}

/****************************************************************************
 * Function Name  : macResetModuleKpiThpCounters
 * Inputs         : localStats - pointer to stats to be reset
 *                  size - Structure size to be reset
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API will reset all the KPI THP cumulative Module 
 *                  specific counters maintained at MAC.
 ****************************************************************************/
/* SPR 10579 changes start*/
void macResetModuleKpiThpCounters(void * localStats, UInt32 size)
{ 
    if(gMacStats.pStats)
        memSet(localStats, 0xff, size);
    /* SPR 10579 changes end*/
    return;
}
/* + SPR_12546_12547 */
/****************************************************************************
 * Function Name  : checkAndSendKPIStats
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API send all the periodic configured stats, if 
 *                  all the cells in the system is stopped or delete.
 ****************************************************************************/
/* + SPR 17439 */
void  checkAndSendKPIStats(void)
/* - SPR 17439 */
{
    InternalCellIndex cellIndex = 0;
    UInt32 kpiRet     = 0x00;
    UInt32 modCounter = 0;
    /*SPR 15909 fix start*/
    tickType_t curr_tick  = macTTITickCount_g;
    /*SPR 15909 fix end*/
    UInt32 diff       = 0 ;
    LteMacKPIStats *pLteMacKPIStats = PNULL ;
    LtePdcpKPIStats *pLtePdcpKPIStats = PNULL ;
    LteMacKPIThpStats *pActiveLteMacKPIThpStats = PNULL ;
    LtePdcpKPIThpStats *pActiveLtePdcpKPIThpStats = PNULL ;
    UInt32 activeIndexForMacStats = 0;
    UInt32 activeIndexForPdcpStats = 0;
    UInt32  activeIndexForMacThpStats = 0;
    UInt32  activeIndexForPdcpThpStats = 0;

    if (INVALID_CELL_INDEX == layer2CommonGetActiveCellTick())
    {
        /* All cells are down so sending stats to respective modules */

        diff = curr_tick - g_kpiScannerObj.prevkpi_tick;
        for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
        {
            activeIndexForMacStats = (gMacStats.pStats->lteCommonKPIStats[cellIndex].activeIndexForMacStats);
            activeIndexForPdcpStats = (gMacStats.pStats->lteCommonKPIStats[cellIndex].activeIndexForPdcpStats);
            pLteMacKPIStats =
                &(gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[activeIndexForMacStats]);
            pLtePdcpKPIStats =
                &(gMacStats.pStats->lteCommonKPIStats[cellIndex].ltePdcpKPIStats[activeIndexForPdcpStats]);

            pLteMacKPIStats->lastCalculatedTTI  += diff;
            pLtePdcpKPIStats->lastCalculatedTTI += diff;
        }


        activeIndexForMacThpStats  = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForMacThpStats;
        activeIndexForPdcpThpStats  = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats;
        pActiveLteMacKPIThpStats   = &(gMacStats.pStats->lteCommonUeKPIStats.lteMacKPIThpStats[activeIndexForMacThpStats]);
        pActiveLtePdcpKPIThpStats   = &(gMacStats.pStats->lteCommonUeKPIStats.ltePdcpKPIThpStats[activeIndexForPdcpThpStats]);


        pActiveLteMacKPIThpStats->durationTTI += diff;
        pActiveLtePdcpKPIThpStats->durationTTI += diff;

        for(modCounter=0;modCounter<MAX_KPI_CONTAINER;modCounter++)
        {
            if (g_kpiScannerObj.module_bitmask & (1<<modCounter))
            {
                if((( MAC_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats) || 
                            ( MAC_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats)) &&
                        ( g_kpiScannerObj.kpiStatsContainer[modCounter].config.lteMacKPIConfig.periodicReporting))
                {   
                    kpiRet |= (1<<modCounter);
                }
                else if ((( PDCP_KPI_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats) ||
                            (PDCP_KPI_THP_STATS == g_kpiScannerObj.kpiStatsContainer[modCounter].macOrPdcpStats)) &&
                        ( g_kpiScannerObj.kpiStatsContainer[modCounter].config.ltePdcpKPIConfig.periodicReporting))
                {
                    /* If throughput stats batches are in progress then no need to send stats */
                    if (g_kpiScannerObj.kpiStatsContainer[modCounter].status.\
                            lteMacKPIThpStatus.servingRespId == PDCP_KPI_THP_STATS_IND)
                    {
                        return;
                    }  
                    kpiRet |= (1<<modCounter);
                }
            }
        }
        g_kpiScannerObj.prevkpi_tick = curr_tick;
        distributeKPIStats(kpiRet);
        resetAndReportThpStats();
    }
}

/****************************************************************************
 * Function Name  : resetKPILastCalculatedTicks
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function will reset all last calculated ticks when 
 *                  very first cell is started including cell start after
 *                  delete or stop.
 ****************************************************************************/
/* + SPR 17439 */
void resetKPILastCalculatedTicks(void)
/* - SPR 17439 */
{
    UInt32 modCounter = 0;
    UInt32 activeIndexForMacStats = 0;
    UInt32 activeIndexForPdcpStats = 0;
    UInt32  activeIndexForMacThpStats = 0;
    UInt32  activeIndexForPdcpThpStats = 0;
    InternalCellIndex cellIndex = 0;  

    g_kpiScannerObj.prevkpi_tick = macTTITickCount_g;

    for(modCounter=0; modCounter<MAX_KPI_CONTAINER; modCounter++)
    {
        g_kpiScannerObj.timeout_tti[modCounter] = 
            g_kpiScannerObj.kpiStatsContainer[modCounter].config.lteMacKPIConfig.durationTTI;
    }

    for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
    {
        activeIndexForMacStats = (gMacStats.pStats->lteCommonKPIStats[cellIndex].activeIndexForMacStats);
        activeIndexForPdcpStats = (gMacStats.pStats->lteCommonKPIStats[cellIndex].activeIndexForPdcpStats);

        gMacStats.pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[activeIndexForMacStats].lastCalculatedTTI = 0;
        gMacStats.pStats->lteCommonKPIStats[cellIndex].ltePdcpKPIStats[activeIndexForPdcpStats].lastCalculatedTTI = 0;

    }

    activeIndexForMacThpStats  = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForMacThpStats;
    activeIndexForPdcpThpStats  = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats;
    gMacStats.pStats->lteCommonUeKPIStats.lteMacKPIThpStats[activeIndexForMacThpStats].durationTTI = 0;
    gMacStats.pStats->lteCommonUeKPIStats.ltePdcpKPIThpStats[activeIndexForPdcpThpStats].durationTTI = 0;
}
/* - SPR_12546_12547 */
#endif /*KPI_STATS*/

#ifdef PERF_STATS
/****************************************************************************
 * Function Name  : checkForPerfStatsUpdateReporting
 * Inputs         : none
 * Outputs        : none
 * Returns        : modCounter - 0xFF is invalid module id
 * Variables      :
 * Description    : This function will return module for which Config Perf Stats
 *                  Ind needs to be triggered
 ****************************************************************************/
/* + SPR 17439 */
UInt32 checkForPerfStatsUpdateReporting(void)
/* - SPR 17439 */
{
    UInt32 retVal = 0x00;
    UInt32 modCounter = 0;
	/* PERF_CA Review */
    /* + SPR_12546_12547 */
    /* SPR 15909 fix start */
    tickType_t curr_tick  = macTTITickCount_g;
    /* - SPR_12546_12547 */
	/* - PERF_CA Review */
    tickType_t diff = curr_tick - g_perfStatsScannerObj.prevPerfStats_tick;
    /* SPR 15909 fix end */

    UInt32 activeIndexForMacStats = gPerfStats.perfStatsControlInfo->macActiveIndex;
    UInt32 activeIndexForPdcpStats = gPerfStats.perfStatsControlInfo->pdcpActiveIndex;
    UInt32 activeIndexForRlcDlStats = gPerfStats.perfStatsControlInfo->rlcDLActiveIndex;
 
    gPerfStats.perfStats[activeIndexForMacStats].macLastCalculatedTTI += diff;
    gPerfStats.perfStats[activeIndexForPdcpStats].pdcpLastCalculatedTTI += diff;
    gPerfStats.perfStats[activeIndexForRlcDlStats].rlcDLLastCalculatedTTI += diff;

    for (modCounter = 0; modCounter < MAX_PERF_STATS_CONTAINER; modCounter++)
    {
	if ((g_perfStatsScannerObj.module_bitmask & (1 << modCounter)) &&
	     (0 >= (int)(g_perfStatsScannerObj.timeoutTTI[modCounter] -= diff)))
	{
	    retVal |= (1 << modCounter);
            /* Perf stats RRM-MAC start */
	    switch (1 << modCounter)
            /* Perf stats RRM-MAC end */
	    {
		case PERF_STATS_CONTAINER_OAM_MAC_BITMASK:
		    g_perfStatsScannerObj.timeoutTTI[modCounter] = 
			g_perfStatsScannerObj.perfStatsContainer[modCounter].
			perfConfig.lteMacPerfStatsConfig.duration;
		    break;

		case PERF_STATS_CONTAINER_OAM_PDCP_BITMASK:
		    g_perfStatsScannerObj.timeoutTTI[modCounter] = 
			g_perfStatsScannerObj.perfStatsContainer[modCounter].
			perfConfig.ltePdcpPerfStatsConfig.duration;
		    break;

		case PERF_STATS_CONTAINER_OAM_RLC_BITMASK:
		    g_perfStatsScannerObj.timeoutTTI[modCounter] = 
			g_perfStatsScannerObj.perfStatsContainer[modCounter].
			perfConfig.lteRlcPerfStatsConfig.duration; 
		    break;

                /* Perf stats RRM-MAC start */
                case PERF_STATS_CONTAINER_RRM_MAC_BITMASK:
                    g_perfStatsScannerObj.timeoutTTI[modCounter] =
                        g_perfStatsScannerObj.perfStatsContainer[modCounter].
                        perfConfig.lteMacPerfStatsConfig.duration;
                    break;
                /* Perf stats RRM-MAC end */
		default:
		    break;       
	    }
	}
    }
    g_perfStatsScannerObj.prevPerfStats_tick = curr_tick;
    return retVal;
}
/* Perf stats RRM-MAC start */
/****************************************************************************
 * Function Name  : distributePerfStats
 * Inputs         : moduleBitmask 
 * Outputs        : None
 * Returns        : void
 * Variables      : 
 * Description    : This function will be called on timeout and update
 *                  Perf Stats Container and send Config Perf Stats ind 
 *                  to trigger Module 
 *****************************************************************************/
/* + PERF_CA */
void distributePerfStats(UInt8 moduleBitmask)
{

    updatePerfStatsForAllModules();

    UInt32 moduleId = 0;
    moduleId = ffs_wrapper(moduleBitmask);
    while (moduleId)
    {
	switch (moduleId - 1)
	{
	    case PERF_STATS_CONTAINER_OAM_MAC:
			    macProcessPerfCounter();
		break;

	    case PERF_STATS_CONTAINER_OAM_PDCP:
		        pdcpProcessPerfCounter();
		break;

	    case PERF_STATS_CONTAINER_OAM_RLC:
		        rlcProcessPerfCounter();
		break;

            /* Perf stats RRM-MAC start */
        case PERF_STATS_CONTAINER_RRM_MAC:
		        macProcessPerfCounterforRRM();
        break;
            /* Perf stats RRM-MAC end */

	    default:
		break;
	}
        moduleBitmask =  moduleBitmask & ( ~(1 << (moduleId - 1)));
        moduleId = ffs_wrapper(moduleBitmask);
    }
}
/* - PERF_CA */
/* Perf stats RRM-MAC end */
/****************************************************************************
 * Function Name  : calculateBestDurationPerfStats
 * Inputs         : void
 * Outputs        : None
 * Returns        : finalTTI
 * Variables      :
 * Description    : This function calculates bestTTI that can be used to 
 *                  for updating KPI Container
 ****************************************************************************/
void calculateBestDurationPerfStats(
        TimeSpec *temp_timeout
        )
{
    UInt8 modCounter;
    SInt64 leastTTI = MAC_MAX_VAL_PERF_TIMER_T * 1000;
    /*SPR 15909 fix start*/
     tickType_t durationTTI = 0;
     /*SPR 15909 fix start*/
    SInt64 localTTI = 0;

    if(g_perfStatsScannerObj.module_bitmask > 0) 
    {
	for(modCounter = 0; modCounter < MAX_PERF_STATS_CONTAINER ; modCounter++)
	{
	    localTTI = 0;

	    switch(modCounter)
	    {
		case PERF_STATS_CONTAINER_OAM_MAC:

		    durationTTI = g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_MAC].\
				 perfConfig.lteMacPerfStatsConfig.duration;
		    break;

		case PERF_STATS_CONTAINER_OAM_RLC:
		    durationTTI = g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_RLC].\
				  perfConfig.lteRlcPerfStatsConfig.duration;
		    break;

		case PERF_STATS_CONTAINER_OAM_PDCP:

		    durationTTI = g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_OAM_PDCP].\
				  perfConfig.ltePdcpPerfStatsConfig.duration;
                    break;

                /* Perf stats RRM-MAC start */
                case PERF_STATS_CONTAINER_RRM_MAC:
		    durationTTI = g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_RRM_MAC].\
				     perfConfig.lteMacPerfStatsConfig.duration;
                break;
                /* Perf stats RRM-MAC end */

	    }

	    if ((g_perfStatsScannerObj.module_bitmask & (1<<modCounter)) && durationTTI > 0)
	    {
		localTTI = g_perfStatsScannerObj.timeoutTTI[modCounter];
	    }

	    if(localTTI && (leastTTI > localTTI))
	    {
		leastTTI = localTTI;
	    }
	}
    }
    /* CA Changes start */
    /* + SPR 13605 Fix */
    if( ((PNULL != cellSpecificParams_g.cellConfigAndInitParams_p[0]) &&
         (CELL_INIT == cellSpecificParams_g.cellConfigAndInitParams_p[0]->cellState))
       || FALSE == rlcInitInd_g  ||   FALSE == pdcpInitInd_g )
    /* - SPR 13605 Fix */
    /* CA Changes end */
    {
	leastTTI = 1000;
    }
#ifdef KPI_STATS
    /*Coverity 87126 Fix Start*/
    if ((((temp_timeout->tv_sec * ((SInt64)1000)) > leastTTI) ||
     ((temp_timeout->tv_sec * ((SInt64)1000)) == leastTTI)) &&
                       (temp_timeout->tv_nsec > ((leastTTI) % 1000)))
    {
    /*Coverity 87126 Fix End*/
#endif 
	temp_timeout->tv_sec = (leastTTI) / 1000;
	temp_timeout->tv_nsec = (leastTTI) % 1000;
	temp_timeout->tv_nsec = (unsigned long) (temp_timeout->tv_nsec * 1000000);

#ifdef KPI_STATS
    }
#endif 
}
#endif
