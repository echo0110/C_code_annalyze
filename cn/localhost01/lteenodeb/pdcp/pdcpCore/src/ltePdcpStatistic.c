/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpStatistic.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : Statistic unit implementation. 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpStatistic.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.9.2.2  2009/07/13 11:04:58  gur20435
 *
 * PDCP Optimizations
 *
 * Revision 1.9.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.9  2009/05/28 16:26:52  gur19140
 * *** empty log message ***
 *
 * Revision 1.8  2009/05/28 15:28:41  gur20548
 * stats done
 *
 * Revision 1.7  2009/05/21 07:10:46  gur19140
 * memory leak fixes
 *
 * Revision 1.6  2009/05/20 15:47:18  gur20548
 * *** empty log message ***
 *
 * Revision 1.5  2009/05/20 14:17:09  gur20548
 * name change
 *
 * Revision 1.4  2009/05/20 04:13:32  gur19836
 * fixed the get stats issue
 *
 * Revision 1.3  2009/05/15 08:02:37  gur19836
 * CR Comments Incorporated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/

#include "ltePdcpLteFramework.h"
#include "ltePdcpLteDefs.h"
#include "ltePdcpErrors.h"
#include "ltePdcpContext.h"
#include "ltePdcpStatistic.h"
#include "ltePdcpTlvUtils.h"


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

PdcpStats pdcpStats ;
 
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/


/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : statsReset
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Resets all statistic data to initial values
 ****************************************************************************/
void    statsReset( void )
{
    UInt16 ueIndex = 0, lcId = 0;
    
    //  SPR 3804 changes start
    UInt16 tempLcId = 0;
    // SPR 3804 changes end

    memSet(&pdcpStats.statsIntegrity, 0, sizeof(PdcpIntegrityStats) );
    memSet(&pdcpStats.statsCiphering, 0, sizeof(PdcpCipheringStats) );
    memSet(&pdcpStats.statsRohc, 0, sizeof(PdcpRohcStats) );

    // SPR 3804 start
    /*CSR 40799 chg start*/
    for ( ueIndex = 0; ueIndex < MAX_UE_SUPPORTED; ueIndex++ )
    {/*CSR 40799 chg end*/
         for (lcId = LTE_SRB_LC_ID_LOW_BOUND; lcId <= LTE_SRB_LC_ID_HIGH_BOUND; lcId++)
         {
              tempLcId = pdcpStats.statsUEs[ueIndex].srbs[lcId - 1].srbId;
              memSet( &pdcpStats.statsUEs[ueIndex].srbs[lcId - 1], 0, sizeof(StatsSrbData) );
             pdcpStats.statsUEs[ueIndex].srbs[lcId - 1].srbId = tempLcId;

         }
         for (lcId = LTE_DRB_LC_ID_LOW_BOUND; lcId <= LTE_DRB_LC_ID_HIGH_BOUND; lcId++)
         {
             tempLcId = pdcpStats.statsUEs[ueIndex].drbs[lcId - 3].lcId;
             memSet( &pdcpStats.statsUEs[ueIndex].drbs[lcId - 3], 0, sizeof(StatsDrbData) );
             pdcpStats.statsUEs[ueIndex].drbs[lcId - 3].lcId = tempLcId;
         }
     }
     // SPR 3804 end
}

/****************************************************************************
 * Function Name  : pdcpInitStats
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Resets all statistic data to initial values
 ****************************************************************************/
void    pdcpInitStats ( void )
{
    UInt16 ueIndex = 0;

    memSet(&pdcpStats.statsIntegrity, 0, sizeof(PdcpIntegrityStats) );
    memSet(&pdcpStats.statsCiphering, 0, sizeof(PdcpCipheringStats) );
    memSet(&pdcpStats.statsRohc, 0, sizeof(PdcpRohcStats) );

    pdcpStats.statNumActiveUe = 0;
    /*CSR 40799 chg start*/
    for ( ueIndex = 0; ueIndex < MAX_UE_SUPPORTED; ueIndex++ )
    {/*CSR 40799 chg end*/
        memSet( &pdcpStats.statsUEs[ueIndex], 0, sizeof(StatsUe) );
    }
}

/* SPR 15692 fix start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
/****************************************************************************
 * Function Name  : pdcpUpdateKPIBeforeLcDelete
 * Inputs         : ueIndex, lcId, qci 
 * Outputs        : None
 * Returns        : None 
 * Description    : This function will update KPIs when before deleting RAB 
 *                  of the UE. This function will be called at the time of 
 *                  cleanup of TX entity. RX entity will be deleted from there
 *                  itself without processing any new packet in this duration
 *                  since PDCP runs as a single thread.
 ****************************************************************************/
void pdcpUpdateKPIBeforeLcDelete( UInt16 ueIndex, UInt8 lcId, 
        UInt8 qci )
{
    UInt16 deleteUEIndex = MAX_UE_SUPPORTED;
    /* SPR 20831 Changes Start */
    UInt32 activeIdxForThpStats = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats;
    LtePdcpKPIThpStats * pLtePdcpKPIThpStats = &(gMacStats.pStats->
            lteCommonUeKPIStats.ltePdcpKPIThpStats[activeIdxForThpStats]);
    /* SPR 20831 Changes End */

    /* SPR23184 Start */
    LtePdcpKPIThpStats *pPdcpOamKPIThpStats =
                &(g_kpiScannerObj.kpiStatsContainer[KPI_CONTAINER_THP_OAM_PDCP].stats.ltePdcpKPIThpStats);
    /* SPR23184 End */
    /* Subtracting lcId with LTE_DRB_LC_ID_LOW_BOUND since DRB index starts 
     * with LTE_DRB_LC_ID_LOW_BOUND */
    lcId -= LTE_DRB_LC_ID_LOW_BOUND;

    /* Copy the deleted RABs entries to the deleted entry to be reported
     * to OAM */
    pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[deleteUEIndex][qci] += 
        pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId];
    /* SPR23184 Start */
    /* Adding the kpi_scanner stats for the ueIndex as the ueIndex
     * will become inactive and kpi_scanner data will be lost for this UE */
    pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[deleteUEIndex][qci] += 
        pPdcpOamKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId];

    pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[deleteUEIndex][qci] += 
        pPdcpOamKPIThpStats->SharedMemoryULPDCPSDU[ueIndex][lcId];
    /* Reset the kpi_scanner counter as we have already used the 
     * counters for ueIndex here */
    pPdcpOamKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId] = 0;
    pPdcpOamKPIThpStats->SharedMemoryULPDCPSDU[ueIndex][lcId] = 0;
    /* SPR23184 End */

    pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[deleteUEIndex][qci] += 
        pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[ueIndex][lcId];

    /* Reset the values at actual UE and LC value */
    pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId] = 0;
    pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[ueIndex][lcId] = 0;
}

/****************************************************************************
 * Function Name  : pdcpFillStatsOfDeletedLc
 * Inputs         : moduleThpId, cnfBuff
 * Outputs        : idx_p - index till which entries for Deleted UE is filled 
 * Returns        : None
 * Description    : This function will update container for deleted RABs. 
 ****************************************************************************/
void pdcpFillStatsOfDeletedLc( UInt8 moduleThpId,
        LP_PdcpKpiThpStatsIndParams cnfBuff, UInt8 *idx_p )
{
    UInt8 qciCount = 0;
    UInt16 deleteUeIndex = MAX_UE_SUPPORTED;
    UInt64 tempSharedMemoryDLPDCPSDU = 0;
    UInt64 tempSharedMemoryULPDCPSDU = 0;
    LtePdcpKPIThpStats *pLtePdcpKPIThpStats =
        &(g_kpiScannerObj.kpiStatsContainer[moduleThpId].stats.ltePdcpKPIThpStats);

    for (qciCount = 0; qciCount < MAX_QCI ; qciCount++)
    {
        tempSharedMemoryDLPDCPSDU =
            pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[deleteUeIndex][qciCount];
        tempSharedMemoryULPDCPSDU =
            pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[deleteUeIndex][qciCount];

        if( (tempSharedMemoryDLPDCPSDU != 0) || 
                (tempSharedMemoryULPDCPSDU != 0) )
        {
/*spr 19711 fix start*/
	/*code deleted*/
/*spr 19711 fix end*/
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/

            cnfBuff->pdcpSduBitDl[*idx_p] = INVALID_THP_STATS_ENTRY;
            cnfBuff->pdcpSduBitUl[*idx_p] = INVALID_THP_STATS_ENTRY;

            cnfBuff->ueIndex[*idx_p] = deleteUeIndex;
            cnfBuff->qci[*idx_p] = qciCount+1;
            if (tempSharedMemoryDLPDCPSDU != 0)
            {
                cnfBuff->pdcpSduBitDl[*idx_p] = tempSharedMemoryDLPDCPSDU;
            }
            if (tempSharedMemoryULPDCPSDU != 0)
            {
                cnfBuff->pdcpSduBitUl[*idx_p] = tempSharedMemoryULPDCPSDU;
            }
            (*idx_p)++;
        }
    }
}

/****************************************************************************
 * Function Name  : pdcpNumQciEntries
 * Inputs         : moduleId 
 * Outputs        : None
 * Returns        : count - number of entries 
 * Description    : This function will return the number of entries to update
 *                  the KPI stats information for deleted RABs.
 ****************************************************************************/
UInt8 pdcpNumQciEntries( UInt8 moduleId )
{
    UInt8 qciIdx = 0;
    UInt8 count = 0;
    UInt16 deleteUeIdx = MAX_UE_SUPPORTED;
    LtePdcpKPIThpStats *kpiThpStats_p = &( g_kpiScannerObj.
            kpiStatsContainer[moduleId].stats.ltePdcpKPIThpStats );

    for( ; qciIdx < MAX_QCI; qciIdx++ )
    {
        if( (0 != kpiThpStats_p->SharedMemoryDLPDCPSDU[deleteUeIdx][qciIdx]) ||
                (0 != kpiThpStats_p->SharedMemoryULPDCPSDU[deleteUeIdx][qciIdx]) )
        {
            count++;
        }
    }
    return count;
}

/****************************************************************************
 * Function Name  : pdcpUpdateKpiThpContainerForDeleteUEs
 * Inputs         : passiveIndexForKpiThpStats - passive index of stats
 *                  moduleId - module Id
 * Outputs        : None 
 * Returns        : None
 * Description    : This function will update container for deleted UEs. 
 ****************************************************************************/
void pdcpUpdateKpiThpContainerForDeleteUEs(UInt32 passiveIndexForKpiThpStats, 
        UInt32 moduleId,InternalCellIndex internalCellIndex )
{ 
    UInt8 qciCount = 0;
    UInt16 deleteUeIndex = MAX_UE_SUPPORTED;
    LtePdcpKPIThpStats *pPdcpOamKPIThpStats = 
        &(g_kpiScannerObj.kpiStatsContainer[moduleId].stats.
                ltePdcpKPIThpStats);

    /* SPR 20831 Changes Start */
    LtePdcpKPIThpStats *pLtePdcpKPIThpStats =
        &(gMacStats.pStats->lteCommonUeKPIStats.
                ltePdcpKPIThpStats[passiveIndexForKpiThpStats]);
    /* SPR 20831 Changes End */

    for (qciCount = 0; qciCount < MAX_QCI; qciCount++)
    {
        if (pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[deleteUeIndex][qciCount])
        {    
            pPdcpOamKPIThpStats->SharedMemoryDLPDCPSDU[deleteUeIndex][qciCount] += 
                pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[deleteUeIndex][qciCount];
        }
        if (pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[deleteUeIndex][qciCount])
        {    
            pPdcpOamKPIThpStats->SharedMemoryULPDCPSDU[deleteUeIndex][qciCount] += 
                pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[deleteUeIndex][qciCount];
        }
    }     
}        

/* + SPR 10577 */
/****************************************************************************
 * Function Name  : pdcpUpdateKpiThpStatsForLc
 * Inputs         : ueId - UE Identifier
 *                  lcId - LC identifier
 *                  entry - entry in response buffer
 *                  qci - QCI value for this LC
 *                  pLtePdcpKPIThpStats - shared memory container
 * Outputs        : cnfBuff - Pointer to confirmation buffer
 * Returns        : None.
 * Description    : This function fills the Throughput stats.
 ****************************************************************************/
void pdcpUpdateKpiThpStatsForLc( UInt16 ueId, UInt8 lcId, UInt8 entry, 
        UInt8 qci, LtePdcpKPIThpStats *pLtePdcpKPIThpStats, 
        LP_PdcpKpiThpStatsIndParams cnfBuff )
{
    /* SPR 21039 fix start */
    cnfBuff->ueIndex[entry] = getRrcUeIdxFromUeIdx(ueId); 
    /* SPR 21039 fix end */
    cnfBuff->qci[entry] = qci;

    /* Reset all counters before filling */
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
    cnfBuff->pdcpSduBitDl[entry] = INVALID_THP_STATS_ENTRY;
    cnfBuff->pdcpSduBitUl[entry] = INVALID_THP_STATS_ENTRY;

/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/

    if( 0 != pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueId][lcId] )
    {
        /* pdcpSduBitUl is sent in bytes to OAM */
        cnfBuff->pdcpSduBitDl[entry] = 
            pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueId][lcId];
    }
    if( 0 != pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[ueId][lcId] )
    {
        /* pdcpSduBitUl is sent in bytes to OAM */
        cnfBuff->pdcpSduBitUl[entry] = 
            pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[ueId][lcId];
    }
}
/* - SPR 10577 */
#endif
/* SPR 15692 fix end */
