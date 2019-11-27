/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcGetStats.h
 *
 ****************************************************************************
 *
 *  File Description : contains the function definitions  needed for 
 *          stats manager to dump the stats info into the stats file.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcStat.h"
#include "lteMisc.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/
/****************************************************************************
 * Private Types
****************************************************************************/
LteRlcShmStatsInfo gRlcStats;
/****************************************************************************
 * Private Definitions
 ***************************************************************************/
UChar8 RLC_PDCP_API_STRING[MAX_RLC_PDCP_API_ID][100] =
{
    {"RLC_UE_DATA_REQ"},
    {"RLC_UE_DATA_IND"},
    {"RLC_DLVRY_STATUS_IND"},
    {"RLC_DATA_DISCARD_REQ"}
};
UChar8 RLC_MAC_API_STRING[MAX_RLC_MAC_API_ID][100] =
{
    {"MAC_UE_OPPOR_IND"},
    {"MAC_UE_DATA_IND"},
    {"MAC_COMCH_OPPOR_IND"},
    {"MAC_COMCH_DATA_IND"},
    {"MAC_UE_HARQ_FAILURE_IND"}
};

/****************************************************************************
 * Private  Constants
 ****************************************************************************/
/****************************************************************************
 * Private Variables
 ****************************************************************************/
/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
* Function Name  : initRlcStats
* Inputs         : None
* Outputs        : None
* Returns        : SUCCESS/FAILURE
* Description    : This function is used to Initialize RLC Stats.
****************************************************************************/
UInt32 initRlcStats()
{
    /* Get the shared memory ID */
    gRlcStats.shmId = shmget_wrapper(RLC_STATS_SHM_KEY, sizeof(LteRlcStats),
                            RLC_STATS_SHM_FLAGS);
    if (0 > gRlcStats.shmId)
    {
        ltePanic("SHM get failed");
    }

    /* Map Shared memory */
    gRlcStats.pRlcStats = (void *)shmat_wrapper(gRlcStats.shmId, NULL, 0);
    if ((void *)-1 == gRlcStats.pRlcStats)
    {
        /* SPR 7385 Changes Starts */
        ltePanic("SHM attach failed");
        /* SPR 7385 Changes Ends */
        return 1;
    }

    return 0;
}
/****************************************************************************
* Function Name  : printRlcPdcpApiStats
* Inputs         : None
* Outputs        : None
* Returns        : None 
* Description    : This  is used to print Rlc Pdcp Api Stats.
****************************************************************************/
UInt32 printRlcPdcpApiStats ()
{
    UInt32 apiId = 0;

    
    printf("\n     Api Name    , NumofCalls, TotalCycleCount , TotalClkTime ,"
            "HighestClockTime , HighestCycleCount , TickAtHighestCycles ,  "
            "HighestTimestamp\n");
    for( apiId = 0; apiId < MAX_RLC_PDCP_API_ID; apiId ++)
    {
        LteRlcApiStats* apiStats_p = &(gRlcStats.pRlcStats->lteRlcApiStat.
                                       rlcPdcpApiStats[apiId]); 
        printf("\n%-30s ,%u, %u, %u, %u, %u, %u,\
                %lu.%lu", RLC_PDCP_API_STRING[apiId], apiStats_p->numOfCalls,
                apiStats_p->totalCycleCount, apiStats_p->totalTime, 
                apiStats_p->highestTime, apiStats_p->highestCycleCount, 
                apiStats_p->highTick, apiStats_p->highTimeStamp.tv_sec,
                apiStats_p->highTimeStamp.tv_nsec);
    }
}
/****************************************************************************
* Function Name  : printRlcMacApiStats
* Inputs         : None
* Outputs        : None
* Returns        : None 
* Description    : This  is used to print Rlc Mac Api Stats.
****************************************************************************/
UInt32 printRlcMacApiStats ()
{
    UInt32 apiId = 0;

    
    printf("\n     Api Name    , NumofCalls, TotalCycleCount , TotalClkTime ,"
            "HighestClockTime , HighestCycleCount , TickAtHighestCycles ,"
            "HighestTimestamp\n");
    for( apiId = 0; apiId < MAX_RLC_MAC_API_ID; apiId ++)
    {
        LteRlcApiStats* apiStats_p = &(gRlcStats.pRlcStats->lteRlcApiStat.
                                       rlcMacApiStats[apiId]); 
        printf("\n%-30s ,%-10u, %-10u, %-10u, %-10u, %-10u, %-10u,\
                %lu.%lu", RLC_MAC_API_STRING[apiId], apiStats_p->numOfCalls,
                apiStats_p->totalCycleCount, apiStats_p->totalTime, 
                apiStats_p->highestTime, apiStats_p->highestCycleCount, 
                apiStats_p->highTick, apiStats_p->highTimeStamp.tv_sec,
                apiStats_p->highTimeStamp.tv_nsec);
    }
}

/****************************************************************************
* Function Name  : main
* Inputs         : argc - Count of Arguments,
*                  argv - Arguments as strings
* Outputs        : None
* Returns        : SUCCESS
* Description    : This  main is invoked to dump Rlc stats to a file form shared
*                  memory.
****************************************************************************/
int main(int argc, char *argv[])
{
    UInt32 ueId = 0;
    UInt32 lcId = 0;
    UInt32 ueSduNumDl = 0;
    UInt32 ueSduBytesDl = 0;
    UInt32 uePduNumDl = 0;
    UInt32 uePduBytesDl = 0;
    UInt32 ueSduNumUl = 0;
    UInt32 ueSduBytesUl = 0;
    UInt32 uePduNumUl = 0;
    UInt32 uePduBytesUl = 0;
    UInt32 ueNumRetx = 0;
    UInt32 ueNumDlvryInd = 0;
    UInt32 ueNumPktsDropped = 0;
    UInt32 ueNumPktsDroppedUl = 0;
    UInt32 ueNumPktsDiscarded = 0;
    UInt32 numActiveLc = 0;
    UInt32 sysSduNumDl = 0;
    UInt32 sysSduBytesDl = 0;
    UInt32 sysPduNumDl = 0;
    UInt32 sysPduBytesDl = 0;
    UInt32 sysSduNumUl = 0;
    UInt32 sysSduBytesUl = 0;
    UInt32 sysPduNumUl = 0;
    UInt32 sysPduBytesUl = 0;
    UInt32 sysNumRetx = 0;
    UInt32 sysNumDlvryInd = 0;
    UInt32 sysNumPktsDropped = 0;
    UInt32 sysNumPktsDroppedUl = 0;
    UInt32 sysNumPktsDiscarded = 0;

    initRlcStats();
	printf("SHM ID: %d\n", gRlcStats.shmId);
    printf("Size of shared memory is %d\n",sizeof(LteRlcStats));

    printf("===============================================================\n");
    LteRlcSysStatInfo* sysStats_p = &(gRlcStats.pRlcStats->lteSysStat);

    printf("################### RLC per UE per LC stats####################\n");

    for (ueId = 0; ueId < sysStats_p->numActiveUE; ueId++)
    {
        numActiveLc = gRlcStats.pRlcStats->lteRlcUeStat[ueId].numActiveLC;
        printf("UE-ID: %-10d , num of Active LC: %-10d\n",ueId,numActiveLc);
        printf("LC-ID     , Num of Dl SDU , Num of DL PDU ,Dl SDU in bytes,"
                "Dl PDU in bytes,Num of pkts dropped in DL, Num of Ul PDU ,"
                " Num of Ul SDU ,Ul PDU in bytes,Ul SDU in bytes,"
                "Num of pkts dropped in UL, Num of ReTx   ,"
                "Num of Delivery Ind ,  Num of Pkts Discarded  ,"
                " DL ThroughPut , UL Throughput \n");
        for (lcId = 1; lcId <= numActiveLc; lcId++)
        {
            LteRlcLCStats* lcStats_p = &(gRlcStats.pRlcStats->lteRlcUeStat[ueId].
                    lcStats[lcId]);
            printf("%-10d, %15d,%-15d,%15d,%-15d,%-25d,%15d,%-15d,%15d,"
                    "%-15d,%-25d,%-15d,%-20d,%-25d,%-15d, %-15d\n",
                    lcId,
                    lcStats_p->sduNumDl,
                    lcStats_p->pduNumDl,
                    lcStats_p->sduBytesDl,
                    lcStats_p->pduBytesDl,
                    lcStats_p->numPktsDropped,
                    lcStats_p->pduNumUl,
                    lcStats_p->sduNumUl,
                    lcStats_p->pduBytesUl,
                    lcStats_p->sduBytesUl,
                    lcStats_p->numPktsDroppedUl,
                    lcStats_p->numRetx,
                    lcStats_p->numDlvryInd,
                    lcStats_p->numPktsDiscarded,
                    lcStats_p->lastDlThroughput,
                    lcStats_p->lastUlThroughput);
            ueSduNumDl += lcStats_p->sduNumDl;
            ueSduBytesDl += lcStats_p->sduBytesDl;
            uePduNumDl += lcStats_p->pduNumDl;
            uePduBytesDl += lcStats_p->pduBytesDl;
            ueSduNumUl += lcStats_p->sduNumUl;
            ueSduBytesUl += lcStats_p->sduBytesUl;
            uePduNumUl += lcStats_p->pduNumUl;
            uePduBytesUl = lcStats_p->pduBytesUl;
            ueNumRetx += lcStats_p->numRetx;
            ueNumDlvryInd += lcStats_p->numDlvryInd;
            ueNumPktsDropped += lcStats_p->numPktsDropped;
            ueNumPktsDroppedUl += lcStats_p->numPktsDroppedUl;
            ueNumPktsDiscarded += lcStats_p->numPktsDiscarded;
        }
        printf("===========================================================\n");
        printf("===========================================================\n");
        printf("Total for UE-Id:%-10d, %15d,%-15d,%15d,%-15d,%-25d,%15d,%-15d,\
                %15d,%-15d,%-25d,%-15d,%-20d,%-25d,%-15d, %-15d\n",\
                ueId,
                ueSduNumDl,
                uePduNumDl,
                ueSduBytesDl,
                uePduBytesDl,
                ueNumPktsDropped,
                uePduNumUl,
                ueSduNumUl,
                uePduBytesUl,
                ueSduBytesUl,
                ueNumPktsDroppedUl,
                ueNumRetx,
                ueNumDlvryInd,
                ueNumPktsDiscarded,
                gRlcStats.pRlcStats->lteRlcUeStat[ueId].lastDlThrput,
                gRlcStats.pRlcStats->lteRlcUeStat[ueId].lastUlThrput
                );
        printf("===========================================================\n");
        printf("===========================================================\n");
        printf("\n");
        sysSduNumDl += ueSduNumDl ;
        sysPduNumDl += uePduNumDl ;
        sysSduBytesDl += ueSduBytesDl ;
        sysPduBytesDl += uePduBytesDl;
        sysNumPktsDropped += ueNumPktsDropped;
        sysPduNumUl += uePduNumUl;
        sysSduNumUl += ueSduNumUl;
        sysPduBytesUl += uePduBytesUl;
        sysSduBytesUl += ueSduBytesUl;
        ueNumPktsDroppedUl += ueNumPktsDroppedUl;
        sysNumRetx += ueNumRetx;
        sysNumDlvryInd += ueNumDlvryInd;
        sysNumPktsDiscarded += ueNumPktsDiscarded;
        ueSduNumDl = 0;          
        uePduNumDl = 0;
        ueSduBytesDl = 0;
        uePduBytesDl = 0;
        ueNumPktsDropped = 0;
        uePduNumUl = 0;
        ueSduNumUl = 0;
        uePduBytesUl = 0;
        ueSduBytesUl = 0;
        ueNumPktsDroppedUl = 0;
        ueNumRetx = 0;
        ueNumDlvryInd = 0;
        ueNumPktsDiscarded = 0;
    }
    printf ("###################SYSTEM STATS OF RLC LAYER##################\n");
    printf ("Num of Active UE : %d\n",sysStats_p->numActiveUE);
    printf ("Dl throughput:  %d\n",sysStats_p->lastSysDlThroughput);
    printf ("Ul throughput:  %d\n",sysStats_p->lastSysUlThroughput);
    printf ("Pkts From Pdcp in DL:  %d\n",sysSduNumDl);
    printf ("Pkts To Mac in DL:  %d\n",sysPduNumDl);
    printf ("Bytes From Pdcp in DL:  %d\n",sysSduBytesDl);
    printf ("Bytes To Mac in DL:  %d\n",sysPduBytesDl);
    printf ("Total DL Pkts Dropped:  %d\n",sysNumPktsDropped);
    printf ("Total Pkts Discarded:  %d\n",sysNumPktsDiscarded);
    printf ("Total Num of ReTransmission:  %d\n",sysNumRetx);
    printf ("Pkts From Mac in UL:  %d\n",sysPduNumUl);
    printf ("Pkts To Pdcp in UL:  %d\n",sysSduNumUl);
    printf ("Bytes From Mac in UL:  %d\n",sysPduBytesUl);
    printf ("Bytes To Pdcp in uL:  %d\n",sysSduBytesUl);
    printf ("Total UL Pkts Dropped:  %d\n",sysNumPktsDroppedUl);
    printf ("Total Num od Delivery Indication:  %d\n",sysNumDlvryInd);
    printf ("\n");

    printf ("######################RLC PDCP Interface API STATS#############"); 
    printRlcPdcpApiStats();
    printf("\n");


    printf ("######################RLC MAC Interface API STATS#############"); 
    printRlcMacApiStats();
    printf("\n");

    return 0;
}

