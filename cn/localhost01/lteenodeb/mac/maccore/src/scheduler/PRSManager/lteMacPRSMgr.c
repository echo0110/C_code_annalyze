
/****************************************************************************
 * *
 * *  ARICENT -
 * *
 * *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 * *
 * ****************************************************************************
 * *
 * *  $Id: lteMacPRSMgr.c,v $
 * *
 * ****************************************************************************
 * *
 * *  File Description : This file contains all the state machine
 *     functions and handler functions that are required by the PRS manager
 * ****************************************************************************
 * *
 * * Revision Details
 * * ----------------
 * * $Log: lteMacPRSMgr.c 
 * ***************************************************************************/

/******************************************************************************
 *  * Standard Library Includes
 ***************************************************************************/

/******************************************************************************
 *  * Project Includes
 *****************************************************************************/
#include "lteMacPRSMgr.h"
#include "lteMacDLResourceInfoMgr.h"
/******************************************************************************
 *  Private Definitions
 *****************************************************************************/
/*CA Changes start  */
LTE_LIST prsPeriodicMap_g[MAX_NUM_CELL][MAX_NUM_PRS_MAPS][MAX_PRS_PERIODICITY];
/*CA Changes end  */
/******************************************************************************
 *  Private Types
 *****************************************************************************/
typedef struct PRSReservedRBsTableT  
{
  UInt8 startRB;
  UInt8 endRB;
}PRSReservedRBsTable;

/*****************************************************************************
 ** Function Prototypes
 *****************************************************************************/
/******************************************************************************
 *  * Private Constants
 ******************************************************************************/
#define MAX_NUM_DL_BANDWIDTH 6

static PRSConfig prsConfigTable_g [] =
{  /* PRS configuration   value of PRS   Value of PRS subframe
      Start  End Index    Periodicity      Offset    */
    {  0    , 159  ,       160  ,                0    },
    {  160  , 479  ,       320  ,               -160  },
    {  480  , 1119 ,       640  ,               -480  },
    {  1120 , 2399 ,       1280 ,               -1120 }
};
 

static UInt32  prsReservedRAT0RBGTable_g[MAX_NUM_DL_BANDWIDTH][MAX_NUM_DL_BANDWIDTH] =
{ 
/* PRS 
 *Bandwidth ->*/ /* 6         15           25           50            75        100 */
/* System
  Bandwidth*/ 
 /*   6 */    {  0x0000000,  0x0000000,  0x0000000,   0x0000000,   0x0000000,   0x0000000 },
 /*  15 */    {  0x1FFFFC3,  0x0000000,  0x0000000,   0x0000000,   0x0000000,   0x0000000 },
 /*  25 */    {  0x1FFFF0F,  0x1FFFC03,  0x0000000,   0x0000000,   0x0000000,   0x0000000 },
 /*  50 */    {  0x1FFFC7F,  0x1FFF81F,  0x1FFE00F,   0x0000000,   0x0000000,   0x0000000 },
 /*  75 */    {  0x1FFF8FF,  0x1FFF07F,  0x1FFE03F,   0x1FF0007,   0x0000000,   0x0000000 },         
 /* 100 */    {  0x1FFC7FF,  0x1FF83FF,  0x1FF01FF,   0x1F8003F,   0x1C00007,   0x0000000 }

 };

static PRSReservedRBsTable  prsReservedRBsTable_g[MAX_NUM_DL_BANDWIDTH][MAX_NUM_DL_BANDWIDTH] =
{
/* PRS
 * Bandwidth ->*/ /*6        15       25         50       75       100 */

/* System
 *Bandwidth*/    
 /*   6 */       { {0,0}   , {0,0}   , {0,0}   , {0,0}   , {0,0}   ,{0,0} },
 /*  15 */       { {4,10}  , {0,0}   , {0,0}   , {0,0}   , {0,0}   ,{0,0} },
 /*  25 */       { {9,15}  , {5,19}  , {0,0}   , {0,0}   , {0,0}   ,{0,0} },
 /*  50 */       { {22,27} , {17,32} , {12,37} , {0,0}   , {0,0}   ,{0,0} },
 /*  75 */       { {34,40} , {30,44} , {25,49} , {12,62} , {0,0}   ,{0,0} },
 /* 100 */       { {47,52} , {42,57} , {37,62} , {25,74} , {12,87} ,{0,0} }
};
/******************************************************************************
 *  * Exported Variables
 ******************************************************************************/

/*****************************************************************************
 *  * Private Variables 
 *****************************************************************************/

/******************************************************************************
 * Private Functions
 *****************************************************************************/


/*****************************************************************************
 * Function Name  : createPRSPeriodicityMap 
 * Inputs         : prsConfigIndex - PRS configuration index,
 *                  prsIndex - PRS Index
                    internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None 
 * Description    : This function calculate PRS offset and periodcity and 
 *                  creates PRS periodic maps.
 *****************************************************************************/

static void createPRSPeriodicityMap(UInt16 prsConfigIndex, UInt8 prsIndex,
                                    /*CA Changes start */
                                    InternalCellIndex internalCellIndex)
                                    /*CA Changes end */
{ 
    PRSPeriodicNode *nodeEntry_p = PNULL;
    UInt16 prsPeriodicity = 0;
    UInt16 prsSubframeOffset = 0;
    UInt16 idx  = 0;

    /* Calculating  prsPeriodicity and prsOffset */
    for ( idx = 0 ; idx < TOTAL_NUM_ROW_IN_PRS_CONFIG; idx++)
    {
	if ((prsConfigIndex >= prsConfigTable_g[idx].prsConfigStartIndex &&
		    prsConfigIndex <= 
		    prsConfigTable_g[idx].prsConfigEndIndex) )
	{
	    prsPeriodicity = prsConfigTable_g[idx].prsPeriodicity;
	    prsSubframeOffset = prsConfigTable_g[idx].prsSubframeOffset + prsConfigIndex;
	    break;
	}
    }

    for (idx = prsSubframeOffset; idx < MAX_PRS_PERIODICITY; idx+=prsPeriodicity)
    { 
	GET_MEM_FROM_POOL(PRSPeriodicNode , nodeEntry_p,
                                     sizeof(PRSPeriodicNode),PNULL);
    /*CA Changes start  */
	listInsertNode(&prsPeriodicMap_g[internalCellIndex][prsIndex][idx], 
                                         &nodeEntry_p->nodeAnchor);
    /*CA Changes end  */
    }
}

/*****************************************************************************
 * Function Name  :  initPRSMaps
 * Inputs         :  prsParseConfigInfo - Parsed PRS structure from RRC 
 *                                        msg buffer,
 *                   dlBandwidth - System bandwidth
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None 
 * Returns        :  None 
 * Description    :  This function calculate required PRS parameters.
 *****************************************************************************/
void  initPRSMaps(PRSParseConfigInfo prsParseConfigInfo,
                  DlBandwidth  dlBandwidth,
                  /*CA Changes start */
                  InternalCellIndex internalCellIndex)
                  /*CA Changes end */
{
    UInt8 idx = 0;
    UInt8 count = 0;
    UInt16 bitsTobeRead = prsParseConfigInfo.bitsTobeRead; 
    UInt16 prsMutingBitmap = prsParseConfigInfo.prsMutingConfigIndex;
    /*CA Changes start  */
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    UInt8 prsIndex = cellParams_p->prsConfigInfo.dlPRSMapIndex;
    /*CA Changes end  */
    UInt8 dlPRSRATPoliciesIndex = cellParams_p->\
                                  prsConfigInfo.dlPRSRATPoliciesIndex;
    PRSResInfo *prsResInfo_p = PNULL;

    /* SPR 15201 changes start */
    UInt8 prsMutingBitIndex = 0;
    /* SPR 15201 changes end */

    /* Memory handling changes start */
    /* The mem pool will be created only first time when cell was configured */ 
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        createMemPool(sizeof(PRSPeriodicNode), (MAX_PRS_PERIODICITY*MAX_NUM_PRS_MAPS));
    }
    /* Memory handling changes end */
    /* Creating PRS periodic map  */  
    createPRSPeriodicityMap(prsParseConfigInfo.prsConfigIndex,prsIndex, internalCellIndex);

    /* Saving Muting configuration */
    if (bitsTobeRead)
    {
        /* SPR 15201 changes start */ 
        /* PRS muting bits will be read from MSB to LSB.
         * Eg. if bitsToBeRead = 2 and sequence = 2 then received string 
         * will be 10000000 and MAC will read 10 from MSB.
         * So in case bitsTobeRead is  2 or 4,  "prsMutingBitmap" is shifted 
         * from MSB to LSB to make consistent logic for any bitsTobeRead */

        prsMutingBitIndex = bitsTobeRead - 1;
        if(bitsTobeRead == 2)
        {
            prsMutingBitmap = (prsMutingBitmap >> 6);
        }
        else if(bitsTobeRead == 4)
        {
            prsMutingBitmap = (prsMutingBitmap >> 4);
        }

        /* The PRS configuration is now read from LSB, moving right to left */
        /*Coverity 87067_87066 Fix Start*/
        for (idx = prsMutingBitIndex; idx < MAX_PRS_MUTING_BITS; idx--)
        /*Coverity 87067_87066 Fix End*/
        /* SPR 15201 changes end */
        {
            for (count = 0; count < MAX_PRS_MUTING_BITS ; )
            {
                if ((idx + count) < MAX_PRS_MUTING_BITS)
                {
                    /* SPR 15201 changes start */
                    cellParams_p->prsConfigInfo.prsMutingConfig[prsMutingBitIndex - idx + count] =
                    /* SPR 15201 changes end */
                        (((prsMutingBitmap &(1 << idx)) == 0)? FALSE:TRUE);
                }   
                count = count + bitsTobeRead;
            }
        }
    }
    else 
    {
        for ( idx = 0; idx < MAX_PRS_MUTING_BITS; idx++ ) 
            cellParams_p->prsConfigInfo.prsMutingConfig[idx] = 1;
    }

    cellParams_p->prsConfigInfo.prsSubframeCounter = 0;    
    cellParams_p->prsConfigInfo.prsOccasionCounter = 0;  
    cellParams_p->prsConfigInfo.expiredPRSSubframeCounterTTI = 0;

    /* Saving RAT0 bitmap for reserving PRs */
    prsResInfo_p =  &(cellParams_p->prsConfigInfo.prsResInfo[dlPRSRATPoliciesIndex]);
    prsResInfo_p->rat0PRSBitmap = 
        prsReservedRAT0RBGTable_g[dlBandwidth][prsParseConfigInfo.prsBandwidth];

    /* Saving start and end reserved PRs number*/
    prsResInfo_p->startPRB =
        prsReservedRBsTable_g[dlBandwidth][prsParseConfigInfo.prsBandwidth].startRB;

    prsResInfo_p->endPRB =
        prsReservedRBsTable_g[dlBandwidth][prsParseConfigInfo.prsBandwidth].endRB;

    /* Marking PRS RBs as blocked so as to be used during ICIC interaction */
    memSet (prsResInfo_p->prsReserveRBMap,1, MAX_RES_BLOCKS);

    memSet (&prsResInfo_p->prsReserveRBMap[prsResInfo_p->startPRB],
            0, (prsResInfo_p->endPRB - prsResInfo_p->startPRB + 1));

    /* Copying parameters from 0th index to 1st index */
    cellParams_p->prsConfigInfo.prsResInfo[!dlPRSRATPoliciesIndex] =
        cellParams_p->prsConfigInfo.prsResInfo[dlPRSRATPoliciesIndex];

    LOG_MAC_MSG(MAC_RRC_PRS_CONFIG_INFO, LOGINFO, MAC_RRC_INF,
            getCurrentTick(),\
            cellParams_p->prsConfigInfo.prsBandwidth,\
            cellParams_p->prsConfigInfo.prsSubframes,\
            prsParseConfigInfo.prsConfigIndex,\
            prsResInfo_p->startPRB,\
            prsResInfo_p->endPRB,\
            DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,__func__,"PRS configured");
    return;
} 
/****************************************************************************
 * Function Name  : processPrsTransmission
 * Inputs         : currnetGlobalTick - Present Global Tick
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall process PRS node in PRS occcasions.        
 ****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
void processPrsTransmission(tickType_t currnetGlobalTick, InternalCellIndex internalCellIndex)
/* SPR 15909 fix end */    
{

    PRSPeriodicNode *node_p = PNULL;
    UInt16 prsSubframeOffset = 0;
    /* SPR_8199_FIX_START */
#ifdef FDD_CONFIG
    /* SPR_8125_FIX_START */
    prsSubframeOffset = (currnetGlobalTick + PHY_DL_DELAY) % MAX_PRS_PERIODICITY;
    /* SPR_8125_FIX_END */
#else
    /* SPR 22525 Fix + */
    prsSubframeOffset = (currnetGlobalTick + PHY_DL_DELAY ) % MAX_PRS_PERIODICITY;
    /* SPR 22525 Fix - */
#endif
    /* SPR_8199_FIX_END */
    node_p = (PRSPeriodicNode*)
            getListNode(&prsPeriodicMap_g[internalCellIndex][cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p-> \
                                                          prsConfigInfo.dlPRSMapIndex][prsSubframeOffset],0);

    if(PNULL != node_p)
    {
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo.prsOccasionCounter++;
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo.prsOccasionCounter = 
               (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo.prsOccasionCounter % MAX_PRS_MUTING_BITS); 
        if(0 == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo.prsOccasionCounter)
        {
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo.prsOccasionCounter = MAX_PRS_MUTING_BITS;
        }

        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo.prsSubframeCounter = 
                                       cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->prsConfigInfo.prsSubframes;   
    }
}

/****************************************************************************
 * Function Name  : cleanupPRSMgr
 * Inputs         : prsIndex - Index of PRS
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall delete PRS nodes which were create at 
 *                  PRS configuration time.
 ****************************************************************************/
void cleanupPRSMgr(UInt8 prsIndex, InternalCellIndex internalCellIndex)
{
    UInt32          count              = 0;
    UInt16          prsSubframeOffset  = 0;
    PRSPeriodicNode *listNode_p        = PNULL;

    for (prsSubframeOffset = 0; prsSubframeOffset < MAX_PRS_PERIODICITY; prsSubframeOffset++)    
    {
        count = listCount(&prsPeriodicMap_g[internalCellIndex][prsIndex][prsSubframeOffset]);
        while(count)
        {
            listNode_p = 
                (PRSPeriodicNode *) getListNode(&prsPeriodicMap_g[internalCellIndex][prsIndex][prsSubframeOffset], 0);
            if(listNode_p) 
            {
                listDeleteNode(&prsPeriodicMap_g[internalCellIndex][prsIndex][prsSubframeOffset], 
                        &(listNode_p->nodeAnchor));
                freeMemPool(listNode_p);
                listNode_p = PNULL;
            }
            count--;
        }   
        listInit(&prsPeriodicMap_g[internalCellIndex][prsIndex][prsSubframeOffset]);
    }
}
/*CA Changes end */

/*******************************************************************************
 * Function Name  : initPRSRATPoliciesBitmaps
 * Inputs         : raPolicyInfo_p - Pointer to ResourceAllocationPolicyInfo,
 *                  prsIndex - Index of PRS
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall update all the policies bitmap for PRS.
 *******************************************************************************/
void initPRSRATPoliciesBitmaps(ResourceAllocationPolicyInfo *raPolicyInfo_p, 
                               UInt8 prsIndex,
                               InternalCellIndex internalCellIndex)
{
    UInt32 previousFreeVRBIndex = 0;
    UInt32 nextFreeVRBIndex = 0;
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL; 
    RAT2LocalizedVRBInfo *tempLocalizedVRBInfo_p = PNULL; 
    UInt32 rat0PRSBitmap = 0;
    UInt8 startPRBIdx = 0;
    UInt8 endPRBIdx = 0;
    RAT0Info   *rat0Info_p = PNULL;
    SubsetInfo *subSetInfo_p = PNULL;
    UInt8 prbNumber = 0;
    subSetInfo_p = &(raPolicyInfo_p->rat1Info.subsetInfo[0]);
    PRSResInfo *prsResInfo_p =  &(cellSpecificParams_g.\
                   cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                    prsConfigInfo.prsResInfo[prsIndex]);
    rat0PRSBitmap = prsResInfo_p->rat0PRSBitmap; 
    rat0Info_p = &(raPolicyInfo_p->rat0Info);        
    startPRBIdx = prsResInfo_p->startPRB + 1;    
    endPRBIdx = prsResInfo_p->endPRB + 1;   

    /* Updating RAT 0 */
    rat0Info_p->rat0BitMap[CE_USER] &= rat0PRSBitmap;         
    rat0Info_p->rat0BitMap[CC_USER] &= rat0PRSBitmap;       


    localizedVRBInfo_p = raPolicyInfo_p->rat2LocalizedInfo.rat2LocalizedVRBInfo;


    for (prbNumber = startPRBIdx; prbNumber <= endPRBIdx; )
    {
        tempLocalizedVRBInfo_p = &localizedVRBInfo_p[prbNumber];    

        if(CELL_EDGE_ONLY_REGION == 
                (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])->rbToResourceTypeMapDL[prbNumber])
        {
            raPolicyInfo_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable -= 1;

        }

        raPolicyInfo_p->rat2LocalizedInfo.numOfVRBAvailable--;

        /* Updating RAT 1 */
        updateRAT1InfoFromRAT2LocalizedInfo(subSetInfo_p, 
                tempLocalizedVRBInfo_p->subsetBitMap,
                tempLocalizedVRBInfo_p->subsetIndex); 

        /* Updating RAT 2 distributed */
        updateRAT2DistributedInfoFromRAT2LocalizedInfo(
                tempLocalizedVRBInfo_p,raPolicyInfo_p);
        prbNumber++;
    }

    /* Updating RAT 2 localized */
    /*Updating the localizedVRBInfo_p array such that a Free VRB knows
     *the next Free VRB Index and also previous Free VRB Index*/

    tempLocalizedVRBInfo_p = &localizedVRBInfo_p[startPRBIdx];

    previousFreeVRBIndex = tempLocalizedVRBInfo_p->previousVRBfreeIndex;
    nextFreeVRBIndex = localizedVRBInfo_p[endPRBIdx].nextfreeVRBIndex;

    /* ICIC changes start */
    if (raPolicyInfo_p->rat2LocalizedInfo.numOfCellEdgeVRBAvailable)
    {
        /* Here we are checking, if VRBs corresponding to allocated  
         * RBG are same as firstCellEdgeFreeIndex or endCellEdgeFreeIndex. 
         * If so, we are modifying these indices.
         */
        if ((raPolicyInfo_p->rat2LocalizedInfo.firstCellEdgeFreeIndex < 
                    nextFreeVRBIndex) &&
                (raPolicyInfo_p->rat2LocalizedInfo.firstCellEdgeFreeIndex > 
                 previousFreeVRBIndex))
        {
            raPolicyInfo_p->rat2LocalizedInfo.firstCellEdgeFreeIndex = 
                nextFreeVRBIndex;
        }
        if ((raPolicyInfo_p->rat2LocalizedInfo.endCellEdgeFreeIndex < 
                    nextFreeVRBIndex) &&
                (raPolicyInfo_p->rat2LocalizedInfo.endCellEdgeFreeIndex > 
                 previousFreeVRBIndex))
        {
            raPolicyInfo_p->rat2LocalizedInfo.endCellEdgeFreeIndex = 
                previousFreeVRBIndex;
        }
    }
    /* ICIC changes end */

    localizedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex = 
        nextFreeVRBIndex;
    localizedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex = 
        previousFreeVRBIndex;


    return;
}

/****************************************************************************
 * Function Name  : reconfigureParsePRSConfig
 * Inputs         : prsParseConfigInfo_p - Pointer to the parsed values
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall update the bitmaps and maps of PRS if
 *                  bandwidth and configuration index is changed repectively.                
 ****************************************************************************/
void  reconfigureParsePRSConfig(PRSParseConfigInfo *prsParseConfigInfo_p,
                                /*CA Changes start */
                                InternalCellIndex internalCellIndex)
                                /*CA Changes end */
{
    UInt8 dlBandwidth = 0;
    UInt8 dlAvailableRBs  = 0;
    UInt8 *rbToResourceTypeMapDL_p  = PNULL;
    UInt8 *prsReserveRBMap_p = PNULL;
    UInt8 requiredRBs = 0;
    PRSResInfo *prsResInfo_p = PNULL;    
    UInt16 bitsToReset = 0;
    UInt16 tempBits = prsParseConfigInfo_p->prsChangeBits;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
               cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt8 tempPRSRATPoliciesIndex = cellParams_p->\
                                  prsConfigInfo.dlPRSRATPoliciesIndex;
    while (tempBits)
    {
        bitsToReset = FFS(tempBits) -1;
        switch(bitsToReset)
        {
            case PRS_BANDWIDTH_CHANGE:
                {
                    /* Reconfiguring PRS RAT policies bitmaps if PRS bandwidth is changed */
                    dlBandwidth = cellParams_p->dlBandwidth;

                    /* Saving RAT0 bitmap for reserving PRs */
                    prsResInfo_p =  &(cellParams_p->prsConfigInfo.prsResInfo[tempPRSRATPoliciesIndex]);
                    prsResInfo_p->rat0PRSBitmap = 
                        prsReservedRAT0RBGTable_g[dlBandwidth][prsParseConfigInfo_p->prsBandwidth];

                    /* Saving start and end reserved PRs number*/
                    prsResInfo_p->startPRB =
                        prsReservedRBsTable_g[dlBandwidth][prsParseConfigInfo_p->prsBandwidth].startRB;

                    prsResInfo_p->endPRB =
                        prsReservedRBsTable_g[dlBandwidth][prsParseConfigInfo_p->prsBandwidth].endRB;

                    prsResInfo_p->prsAllowedWithICIC = TRUE;
                    /* Marking PRS RBs as blocked so as to be used during ICIC interaction */
                    memSet (prsResInfo_p->prsReserveRBMap,1, MAX_RES_BLOCKS);

                    memSet (&prsResInfo_p->prsReserveRBMap[prsResInfo_p->startPRB],
                            0, (prsResInfo_p->endPRB - prsResInfo_p->startPRB + 1));


                    memCpy((&(dlRAInfo_gp[internalCellIndex] + tempPRSRATPoliciesIndex)->raPolicyInfoPRSReserved),  
                           (& (dlRAInfo_gp[internalCellIndex] +dlRAInfoIndex_g[internalCellIndex])->raPolicyInfo), 
                            sizeof(ResourceAllocationPolicyInfo ));


                    if ((cellParams_p->prsConfigInfo.isPRSConfigured) &&
                            (prsResInfo_p->startPRB != prsResInfo_p->endPRB))
                    {
                        dlAvailableRBs  = cellParams_p->dlAvailableRBs;  
                        rbToResourceTypeMapDL_p = (dlRAInfo_gp[internalCellIndex] +dlRAInfoIndex_g[internalCellIndex])->rbToResourceTypeMapDL;
                        prsReserveRBMap_p = prsResInfo_p->prsReserveRBMap; 
                        CHECK_REMAINING_RES_WITH_ICIC_PRS_ON(dlAvailableRBs,requiredRBs,
                                rbToResourceTypeMapDL_p,prsReserveRBMap_p);

                        if (requiredRBs ==  MAX_RES_REQUIRED_TO_SCHEDULE_SI_PAGING)
                        { 	
                            initPRSRATPoliciesBitmaps(&((dlRAInfo_gp[internalCellIndex] + tempPRSRATPoliciesIndex)->\
                                        raPolicyInfoPRSReserved),tempPRSRATPoliciesIndex,
                                        internalCellIndex);
                        }
                        else 
                        {
                            prsResInfo_p->prsAllowedWithICIC = FALSE;
                        }
                    }
                    break;
                }
            case PRS_CONFIGURATION_INDEX_CHANGE:
                {
                    /* Recongifuring PRS maps if configuration index is changed */
                    createPRSPeriodicityMap(prsParseConfigInfo_p->prsConfigIndex,
                            !(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                             cellParams_p->prsConfigInfo.dlPRSMapIndex), internalCellIndex);
                    break;
                }
            default:
                break;
        }
        tempBits &= (~(1 << (bitsToReset)));
    }
    return;
}

/****************************************************************************
 * Function Name  : reconfigurePRSConfig
 * Inputs         : prsParseConfigInfo_p - Pointer to the parsed values
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function shall update respective PRS parameters which 
 *                  are reconfigured.              
 ****************************************************************************/
void reconfigurePRSConfig(PRSParseConfigInfo *prsParseConfigInfo_p,
                          /*CA Changes start */
                          InternalCellIndex internalCellIndex)
                          /*CA Changes end */
{
    UInt8 prsChangeBits = prsParseConfigInfo_p->prsChangeBits; 
    UInt8 changedBit = 0;
    UInt8 idx = 0;
    UInt8 count = 0;
    UInt16 bitsTobeRead = 0; 
    UInt16 prsMutingBitmap = 0;
    SInt8 diffOfConfiguredPrsSubframes = 0;
    UInt8 prsSubframeCounter = 0;
    SInt8 newPRSSubframes = 0;
    UInt8 isConfigurationIndexChanged = FALSE;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
          cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt8 dlPRSRATPoliciesIndex = cellParams_p->\
                                  prsConfigInfo.dlPRSRATPoliciesIndex;

    while (prsChangeBits)
    {
        changedBit = FFS(prsChangeBits) - 1;
        switch(changedBit) 
        {
            case PRS_CONFIGURATION_INDEX_CHANGE:
                {
                    cellParams_p->prsConfigInfo.prsOccasionCounter = 0;
                    cellParams_p->prsConfigInfo.prsSubframeCounter = 0;
                    cellParams_p->prsConfigInfo.expiredPRSSubframeCounterTTI = 0;
                    cleanupPRSMgr(cellParams_p->prsConfigInfo.dlPRSMapIndex, internalCellIndex);
                    cellParams_p->prsConfigInfo.dlPRSMapIndex = \
                          !(cellParams_p->prsConfigInfo.dlPRSMapIndex);   
                    isConfigurationIndexChanged = TRUE; 
                }
                break;

            case PRS_BANDWIDTH_CHANGE:
                {
                    dlPRSRATPoliciesIndex = !dlPRSRATPoliciesIndex;   
                    /* + SPR_8532 */
                    cellParams_p->prsConfigInfo.prsBandwidth = prsParseConfigInfo_p->prsBandwidth;  
                    /* - SPR_8532 */
                }
                break;

            case PRS_TRANSMISSION_POWER_CHANGE:
                {
                    cellParams_p->prsConfigInfo.prsTransmissionPower = 
                        prsParseConfigInfo_p->prsTransmissionPower;
                }
                break;

            case PRS_SUBFRAME_CHANGE:
                {   
                    /* If PRS configuration index is changed then only prsSubframes
                     * will be updated and no need to recalculate prsSubframeCounter.
                     */   
                    if (!isConfigurationIndexChanged)
                    {
                        diffOfConfiguredPrsSubframes = cellParams_p->prsConfigInfo.prsSubframes -
                            prsParseConfigInfo_p->prsSubframes;
                        prsSubframeCounter = cellParams_p->prsConfigInfo.prsSubframeCounter;
                        newPRSSubframes = prsSubframeCounter - diffOfConfiguredPrsSubframes;

                        /* PRS cycle is active so calculating for how many subframes PRS 
                         *  needs to be send according to new configuration.
                         */   
                        if (prsSubframeCounter)
                        {

                            if (newPRSSubframes < 0)
                            {
                                /* PRS is already send according to new PRS subframes */
                                cellParams_p->prsConfigInfo.prsSubframeCounter = 0;
                            }    
                            else
                            {
                                /* newPRSSubframes are left according to new configuration */
                                cellParams_p->prsConfigInfo.prsSubframeCounter = newPRSSubframes;
                            }  
                        }
                        /* PRS cycle was expired in this tick and  new PRS subframes are 
                         * greater than old subframes then run PRS cycle for remaining subframes
                         */    
                        else if ( (diffOfConfiguredPrsSubframes < 0)  &&
                                (cellParams_p->prsConfigInfo.expiredPRSSubframeCounterTTI == 
                                 getCurrentTick()))
                        {
                            cellParams_p->prsConfigInfo.prsSubframeCounter = newPRSSubframes;  
                        }
                    }
                    cellParams_p->prsConfigInfo.prsSubframes = prsParseConfigInfo_p->prsSubframes;   

                }
                break;

            case PRS_MUTING_CONFIGURATION_CHANGE:
                {

                    prsMutingBitmap = prsParseConfigInfo_p->prsMutingConfigIndex;
                    bitsTobeRead = prsParseConfigInfo_p->bitsTobeRead;

                    for (idx = 0; idx < bitsTobeRead; idx++)
                    {
                        for (count = 0; count < MAX_PRS_MUTING_BITS ; )
                        {
                            if ((idx + count) < MAX_PRS_MUTING_BITS)
                            {
                                cellParams_p->prsConfigInfo.prsMutingConfig[idx + count] = 
                                    (((prsMutingBitmap &(1 << idx)) == 0)? FALSE:TRUE);
                            }   
                            count = count + bitsTobeRead;
                        }
                    }

                }
                break;

            default:
                break;  

        }

        prsChangeBits &= (~(1 << (changedBit)));
    }
}

