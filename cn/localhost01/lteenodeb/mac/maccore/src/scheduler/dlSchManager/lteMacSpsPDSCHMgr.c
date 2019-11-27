/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
******************************************************************************
 *
 *  File Description : THIS FILE CONTAINS THE FUNCTIONS RELATED TO SPS PDSCH
 *                          Allocation for DL SPS.
******************************************************************************

*****************************************************************************/


/******************************************************************************
 * Standard Library Includes
******************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacSpsPDSCHmgr.h"
#include "lteCommonStatsManager.h"
#include "lteMacUEConfMgr.h"
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacDRXMgr.h"
#include "lteMacDLResourceAllocator.h"
#include "lteMacDLPowerControlData.h"
#ifdef TDD_CONFIG
#include "lteMacTddMgr.h"
#endif
/******************************************************************************
  Project Includes
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

static void traverseAndUpdateTtiBitMapArray(
        UInt8 spsTTIBitMap[], UInt8 index, UInt8 lowerbound,
        UInt8 upperbound, UInt8 startindex, UInt8 endindex, UInt8 res);

static void updateTtiBitMap(UInt8 spsTTIBitMap[],UInt8 ttiMapindex, 
        UInt8 startindex,UInt8 endindex,UInt8 res);

static void getRAT2LocalizedFromSpsTti(
        UInt8 spsTTIBitMap[], RAT2LocalizedVRBInfo localizedVRBInfo[],
        UInt8 startindex,UInt8 bitMapIndex, UInt8 prevSetBitIndex);

static void updateSpsTtiBitMap(UInt8 spsTTIBitMap[],UInt8 startindex,
                                UInt8 endindex, UInt8 res);
static void processDLResourceReserveRequest(DLUEContext *ueContext_p, 
                                LP_MacDlSpsResReq macSpsResReq_p,
                                InternalCellIndex InternalCellIndex);

static void reserveCommonChannelRBInRM( 
        RAT2LocalizedInfo * spsRAT2LocalizedInfo_p, 
        UInt8 spsTTIBitMap[],
        InternalCellIndex InternalCellIndex);

static MacRetType processDlWBRsvReq( UInt8 *wideBandMcsIndex,
    LP_SpsDlInfo spsDlInfo_p, 
    /* +- SPR 17777 */
    BestVRBSpanInfo *bestVRBSpanInfo_p,
    UInt8 *ttiIdx, UInt32 *maxTBSize,
    /* + SPS_TDD_Changes */
    DLUEContext *ueContext_p,
    /* - SPS_TDD_Changes */
    InternalCellIndex InternalCellIndex);

static MacRetType processDlSBRsvReq( LP_SpsDlInfo spsDlInfo_p, 
     RAT0NonSpatialMulCQIInfo *rat0CQIInfo_p,
     /* +- SPR 17777 */
     UInt8* sbMCSIndex, 
     BestVRBSpanInfo *bestVRBSpanInfo_p,
     UInt8 *ttiIdx, UInt32 *maxTBSize,
     InternalCellIndex internalCellIndex);

SpsStrIntervalTTISorted sortedArr_g[MAX_NUM_CELL][SPS_INTERVAL_40] = {{{0}}};

/******************************************************************************
 * Function Name    : dlSpsResourceReserveHandler
 * Inputs           : qCount - Size of Queue
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs          : None
 * Returns          : None
 * Description      : This will process the DL Resource Reserve request if the 
 *                    Request type is MAC_SPS_RES_RSV_REQ_ACT of UE.
 ******************************************************************************/
void dlSpsResourceReserveHandler( UInt32 qCount ,InternalCellIndex internalCellIndex)
{
    LP_MacDlSpsResReq   macSpsResReq_p     = PNULL;
    DLUEContext         *ueContext_p       = PNULL;
    UInt32 retVal                          = MAC_SUCCESS;

    while( qCount-- )
    {
        retVal = DEQUEUE_MAC_DL_SPS_Q( dlSpsResRsvQ_gp[internalCellIndex], macSpsResReq_p );
        if( (PNULL == macSpsResReq_p) || (MAC_FAILURE == retVal) )
        {
            LOG_MAC_MSG( MAC_SPS_NULL_NODE, LOGWARNING, MAC_PDSCH, getCurrentTick(),
                    __LINE__, 1, 0,0,0, 0,0, __func__, "dlSpsResRsvQ_g");
            return;
        }

        /*! \code
         * Get UE-Context
         * \endcode
         * */
        if (! dlUECtxInfoArr_g[macSpsResReq_p->ueIndex].pendingDeleteFlag)
        {
            ueContext_p = dlUECtxInfoArr_g[macSpsResReq_p->ueIndex].dlUEContext_p;
            if( PNULL == ueContext_p )
            {
                LOG_MAC_MSG( MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID, LOGFATAL, MAC_PDSCH, 
                        getCurrentTick(), __LINE__, macSpsResReq_p->ueIndex, 0,0,0,
                        0,0, __func__, "SPS_PDSCH");                
                /* Free the memory */
                freeMemPool ( macSpsResReq_p );
                macSpsResReq_p = PNULL;
                continue;
            }
            if( MAC_SPS_RES_RSV_REQ_ACT == macSpsResReq_p->requestType )
            {
                processDLResourceReserveRequest(ueContext_p,macSpsResReq_p,internalCellIndex);
            }
        }
        else
        {
            /* Free the memory */
            freeMemPool ( macSpsResReq_p );
            macSpsResReq_p = PNULL;    
        }
    }
}

/******************************************************************************
 * Function Name  : dlSpsResourceReleaseHandler
 * Inputs         : qCount - Size of Q 
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This will release the SPS RM Information.
 ******************************************************************************/
void dlSpsResourceReleaseHandler( UInt32 qCount ,InternalCellIndex internalCellIndex)
{
    LP_MacDlSpsResReq   macSpsResReq_p     = PNULL;
    DLUEContext         *ueContext_p       = PNULL;

    while( qCount-- )
    {
        DEQUEUE_MAC_DL_SPS_Q( dlSpsResRelQ_gp[internalCellIndex], macSpsResReq_p );
        if( PNULL == macSpsResReq_p )
        {
            LOG_MAC_MSG( MAC_SPS_NULL_NODE, LOGWARNING, MAC_PDSCH, getCurrentTick(),
                    __LINE__, 1, 0,0,0, 0,0, __func__, "dlSpsResRelQ_g");
            return;
        }

        /*! \code
         * Get UE-Context
         * \endcode
         * */
        if (! dlUECtxInfoArr_g[macSpsResReq_p->ueIndex].pendingDeleteFlag)
        {
            ueContext_p = dlUECtxInfoArr_g[macSpsResReq_p->ueIndex].dlUEContext_p;
            if( PNULL == ueContext_p )
            {
                LOG_MAC_MSG( MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID, LOGFATAL, MAC_PDSCH, 
                        getCurrentTick(), __LINE__, macSpsResReq_p->ueIndex, 0,0,0,
                        0,0, __func__, "SPS_PDSCH");                
                /* Free the memory */
                freeMemPool ( macSpsResReq_p );
                macSpsResReq_p = PNULL;
                continue;
            }

            if( MAC_SPS_RES_REL_REQ == macSpsResReq_p->requestType )
            {
                processDLResourceReleaseRequest(ueContext_p,internalCellIndex);
                freeMemPool ( macSpsResReq_p );
                macSpsResReq_p = PNULL;
            }
        }
        else
        {
            /* Free the memory */
            freeMemPool ( macSpsResReq_p );
            macSpsResReq_p = PNULL;    
        }
    }
}

/******************************************************************************
 * Function Name    : updateSpsTtiBitMap
 * Inputs           : spsTTIBitMap - SPS TTI Bitmap to be updated,
 *                    startindex - start index of array of SPS TTI Bitmap,
 *                    endindex - end index of array of SPS TTI Bitmap,
 *                    res - (0=Release and 1=Reserve)
 * Outputs          : None
 * Returns          : None
 * Description      : This function is called by processDlSpsResResrvRelQ to
 *                    to update SPS TTI Bitmap. It finds out upperbound
 *                    lowerbound SPS TTI Bitmap array index.
 ******************************************************************************/
void updateSpsTtiBitMap(UInt8 spsTTIBitMap[],UInt8 startindex,
                                UInt8 endindex, UInt8 res)
{
    UInt8 ttiUpperbound =0, i = 0;
    UInt8 ttiarrayindex = 0;
    UInt8 ttiLowerbound =  ttiUpperbound-32;

    for ( i = 1; i<= 13 ; i++)
    {
        /*+- SPR 20899 Fix*/
         if( MULTIPLY_BY_EIGHT(i) >= startindex )
        /*+- SPR 20899 Fix*/
         {
        /*+- SPR 20899 Fix*/
            ttiUpperbound = MULTIPLY_BY_EIGHT(i) - 1;
        /*+- SPR 20899 Fix*/
            ttiarrayindex = (i - 1);
            break;
         }
    }

    ttiLowerbound =  ttiUpperbound- 7;
    traverseAndUpdateTtiBitMapArray(spsTTIBitMap, ttiarrayindex, 
                                    ttiLowerbound, ttiUpperbound, startindex, 
                                    endindex, res);
}

/*************************************************************************************
 * Function Name    : traverseAndUpdateTtiBitMapArray
 * Inputs           : spsTTIBitMap - SPS TTI Bitmap to be updated,
 *                    index - Index of array of SPS TTI Bitmap,
 *                    lowerbound - lower limit of SPS TTI Bitmap as per array index,
 *                    upperbound - upper limit of SPS TTI Bitmap as per array index,
 *                    startindex - start index of array of SPS TTI Bitmap,
 *                    endindex - end index of array of SPS TTI Bitmap,
 *                    res - (0=Release and 1=Reserve)
 * Outputs          : None
 * Returns          : None
 * Description      : This function is called by updateSpsTtiBitMap to update SPS
 *                    TTI Bitmap. It recursively update SPS TTI Bitmap.
 *************************************************************************************/
void traverseAndUpdateTtiBitMapArray(UInt8 spsTTIBitMap[], UInt8 index, 
        UInt8 lowerbound, UInt8 upperbound, UInt8 startindex, 
        UInt8 endindex, UInt8 res)
{
    /*!
     * \code
     * If endindex is less than Upperbound of startindex, update 
     * ttiBitMap[index] from startIndex to endIndex
     * \endcode
     * */ 
    /* SPR 16725 Fix Start */
    if(endindex <= upperbound)
    /* SPR 16725 Fix End */
    {
        updateTtiBitMap( spsTTIBitMap, index, startindex - lowerbound, 
                         endindex - lowerbound, res);
    }
    /*!
     * \code
     * If endindex is greater than Upperbound of startindex, update 
     * ttiBitMap[index] from startIndex to Upperbound
     *     Move to ttiBitMap[index++] & update ttiBitMap from Lowerbound to
     *     endIndex.
     * \endcode
     * */ 
    else if(endindex>upperbound)
    {
        updateTtiBitMap( spsTTIBitMap, index, startindex-lowerbound, 7, res);
        index++;
        upperbound+=8;
        lowerbound+=8;
        traverseAndUpdateTtiBitMapArray( spsTTIBitMap, index, lowerbound, 
                                       upperbound, lowerbound, endindex, res);
    }
}
/************************************************************************************
 * Function Name    : updateTtiBitMap
 * Inputs           : spsTTIBitMap - SPS TTI Bitmap to be updated,
 *                    ttiMapindex - Map Index of TTI Bitmap,
 *                    startindex - start index of array of SPS TTI Bitmap,
 *                    endindex - end index of array of SPS TTI Bitmap,
 *                    res - (0=Release and 1=Reserve)
 * Outputs          : None
 * Returns          : None
 * Description      : This function is called by processDlSpsResResrvRelQ to update
 *                    SPS TTI Bitmap.
 ************************************************************************************/
void updateTtiBitMap( UInt8 spsTTIBitMap[], UInt8 ttiMapindex, 
        UInt8 startindex, UInt8 endindex, UInt8 res)
{
    /*!
     * \code
     * If request is of Reserve, set the bits from startIndex to endIndex
     * If request is of Release, reset the bits from startIndex to endIndex
     * \endcode
     * */ 
   UInt8 index=0;
   while(startindex <= endindex)
   {
        index|= (UInt8)1<<startindex;
        startindex++;
   }
   if(MAC_SPS_RES_REL_REQ == res)
   {
        spsTTIBitMap[ttiMapindex]|=index;
   }
   else
   {
        index=~index;
        spsTTIBitMap[ttiMapindex]&=index;
   }
}
/* SPR 17277 Fix Start */
/******************************************************************************
 * Function Name    : getRAT2LocalizedFromSpsTti
 * Inputs           : spsTTIBitMap - SPS TTI Bitmap to be updated,
 *                    localizedVRBInfo - Localized VRBInfo as output,
 *                    startindex - start index of array of SPS TTI Bitmap,
 *                    bitMapIndex - SPS TTI Bitmap[bitMapIndex],
 *                    prevSetBitIndex - Previous set bit Index
 * Outputs          : None
 * Returns          : None
 * Description      : This function is called by processDlSpsResResrvRelQ to
 *                    convert SPS TTI Bitmap into RAT2LocalizedVRBInfo. It
 *                    Populates RAT2LocalizedVRBInfo from spsTTIBitMap.
 ******************************************************************************/
void getRAT2LocalizedFromSpsTti( UInt8 spsTTIBitMap[], 
        RAT2LocalizedVRBInfo localizedVRBInfo[], UInt8 startindex, 
        UInt8 bitMapIndex, UInt8 prevSetBitIndex)
{
    UInt8 ttiMap = spsTTIBitMap[bitMapIndex];
    UInt8 currttiSetBitIndex=0;

    UInt8 ttiSetBitIndex=0;
    UInt8 multiplier = 1;
    UInt8 iterator = 0;
    /*!
     * \code
     * Find out first set bit starting from LSB of ttiMap=spsTTIBitMap[bitMapIndex]. 
     * \endcode
     * */
    /* SPR 7385 Changes Starts */
    ttiSetBitIndex = FFS(ttiMap);
    /* SPR 7385 Changes Ends */
    ttiMap &= (~((UInt8)1 << (ttiSetBitIndex-1)));
    /*!
     * \code
     * Reset that bit so that now its occupied in ttiMap
     * \endcode
     * */
    if(0 != ttiSetBitIndex )
    {
        if (0 != prevSetBitIndex)
        {
            localizedVRBInfo[startindex+ttiSetBitIndex-1].previousVRBfreeIndex = prevSetBitIndex;
        } 
        else 
        {
            if (localizedVRBInfo[prevSetBitIndex].nextfreeVRBIndex == (startindex+ttiSetBitIndex-1))
            {
                localizedVRBInfo[startindex+ttiSetBitIndex-1].previousVRBfreeIndex = prevSetBitIndex;
            }
            else
            {
                localizedVRBInfo[startindex+ttiSetBitIndex-1].previousVRBfreeIndex = startindex+ttiSetBitIndex-1;
            }
        }
    }
    /*!
     * \code
     * Update this index information in RAT2LocalizedVRBInfo so that Free VRB know the next
     * Free VRB Index and previous Free VRB Index
     * \endcode
     * */   
    while (ttiMap)
    {
        /* find out first set tti bit*/
        /* SPR 7385 Changes Starts */
        currttiSetBitIndex = FFS(ttiMap);
        /* SPR 7385 Changes Ends */
        localizedVRBInfo[startindex+ttiSetBitIndex-1].nextfreeVRBIndex = 
            currttiSetBitIndex+startindex-1;
        prevSetBitIndex = ttiSetBitIndex;
        ttiSetBitIndex = currttiSetBitIndex;
        localizedVRBInfo[startindex+ttiSetBitIndex-1].previousVRBfreeIndex =
            prevSetBitIndex+startindex-1;
        ttiMap&= (~((UInt8)1 << (currttiSetBitIndex-1)));
    }
    /*!
     * \code
     * Keep traversing the loop till all set bits in ttiMap are covered 
     * \endcode
     * */
    if(13 <= ++bitMapIndex)
    {
        localizedVRBInfo[MAX_DISTRIBUTED_VRB-1].nextfreeVRBIndex = 
                                                 MAX_DISTRIBUTED_VRB - 1;
        return;
    }
    else
    {
        iterator = bitMapIndex;
        if (0 != ttiSetBitIndex)
        {
            /* SPR 7385 Changes Starts */
            while ((!ffs(spsTTIBitMap[iterator]) && (iterator <= 13)))
            {
                iterator ++;
                multiplier ++;
                continue;
            }
            currttiSetBitIndex = ffs(spsTTIBitMap[iterator]);
            /* SPR 7385 Changes Ends */
            if (currttiSetBitIndex)
            {
                localizedVRBInfo[startindex+ttiSetBitIndex-1].nextfreeVRBIndex =
                    currttiSetBitIndex+(startindex-1)+(8*multiplier);
                prevSetBitIndex = ttiSetBitIndex;
                prevSetBitIndex = prevSetBitIndex+startindex-1;
            }
            else
            {
                localizedVRBInfo[startindex+ttiSetBitIndex-1].nextfreeVRBIndex =
                    startindex+ttiSetBitIndex-1;
            }
        }
    }
    /*!
     * \code
     * Repeat above procedure for all remaining 13 ttiMaps i.e. for next bitMapIndex
     * \endcode
     * */
    getRAT2LocalizedFromSpsTti(spsTTIBitMap, localizedVRBInfo, startindex+(8 * multiplier),
            iterator, prevSetBitIndex);
}
/* SPR 17277 Fix End */

/*************************************************************************************
 * Function Name    : spsDLAllocateStatusDataReq
 * Inputs           : ueIndex - Index of UE,
 *                    response - Strategy receives following type of responses from
 *                               PDSCH or HARQ module (Pdsch res reserv Success,
 *                               Pdsch res reserv Failure, Harq First Ack /
 *                               NACK for Activation, HARQ DTX or timer expiry info),
 *                    requestType - Type of request for Reserve or release resources,
 *                    spsOpp - Latest Opportunity provided to SPS LC,
 *                    numRbs - RB's occupied
 * Outputs          : None
 * Returns          : NoneLP_PdcpDataReq pointer on Status request structure or NULL
 * Description      : To allocate and init LP_MacDlSpsStatus.
 *************************************************************************************/
/* + coverity 54165 */
LP_MacDlSpsStatus spsDLAllocateStatusDataReq( UInt16 ueIndex, SpsCause   response,
/* - coverity 54165 */
        UInt8    requestType, UInt32  spsOpp, UInt16   numRbs)
{
    LP_MacDlSpsStatus dataReq_p = PNULL;
    /*!
     * \code
     *  Allocate data request structure 
     * \endcode
     * */
    dataReq_p = (LP_MacDlSpsStatus)getMemFromPool( sizeof(MacDlSpsStatus), PNULL);

    if ( PNULL == dataReq_p  )
    {
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacDlSpsStatus), 0,0,0,
                0.0,0.0,__FILE__,__func__);
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s",__FUNCTION__);
        return PNULL;
    }
    /*!
     * \code
     *  Init dataReq_p. 
     * \endcode
     * */
    dataReq_p->ueIndex = ueIndex;
    dataReq_p->response = response;
    dataReq_p->requestType = requestType;
    dataReq_p->spsOpp = spsOpp;
    dataReq_p->numRbs = numRbs;

    return dataReq_p;
}

/******************************************************************************
 * Function Name    : initSpsRAT2LocalizedInfo
 * Inputs           : rat2Localized_p - This holds pointer to
 *                                      RAT2LocalizedInfo structure,
 *                    dlRBs - This holds the downlink bandwidth in terms of RBs
 * Outputs          : None
 * Returns          : None
 * Description      : This will intialize the SPS RAT 2 Localized Information.
 ******************************************************************************/
void initSpsRAT2LocalizedInfo(
        RAT2LocalizedInfo *rat2Localized_p,
        UInt32 dlRBs)
{
    UInt32 maxVRBLocalized = 0;
    RAT2LocalizedVRBInfo *localizedVRBInfo_p = PNULL;

    rat2Localized_p->maxLocalizedVRB = dlRBs;
    rat2Localized_p->numOfVRBAvailable = dlRBs;
    maxVRBLocalized  = dlRBs;

    localizedVRBInfo_p = &(rat2Localized_p->rat2LocalizedVRBInfo[
                                     maxVRBLocalized + 2]);

    localizedVRBInfo_p->nextfreeVRBIndex = maxVRBLocalized + 3;
    localizedVRBInfo_p->previousVRBfreeIndex = maxVRBLocalized;

    localizedVRBInfo_p--;
    localizedVRBInfo_p->nextfreeVRBIndex = maxVRBLocalized + 2;
    localizedVRBInfo_p->previousVRBfreeIndex = maxVRBLocalized;

    localizedVRBInfo_p--;
    /*!
     * \code
     * Below Logic is poluating ratLocalizedInfo_p structure such that
     * Intializing all VRB as free  and also a Free VRB know the next
     * Free VRB Index and previous Free VRB Index 
     * \endcode
     * */

    localizedVRBInfo_p->nextfreeVRBIndex = maxVRBLocalized + 2;
    localizedVRBInfo_p->previousVRBfreeIndex = maxVRBLocalized - 1;
    localizedVRBInfo_p--;
    maxVRBLocalized--;
    while ( maxVRBLocalized )
    {
        localizedVRBInfo_p->nextfreeVRBIndex = maxVRBLocalized + 1;
        localizedVRBInfo_p->previousVRBfreeIndex = maxVRBLocalized - 1;
        localizedVRBInfo_p--;
        maxVRBLocalized--;
    }
    localizedVRBInfo_p->nextfreeVRBIndex = 1;
    localizedVRBInfo_p->previousVRBfreeIndex = 0;
}

/******************************************************************************
 * Function Name  : initDLSpsTTIContextMap
 * Inputs         : rmSize - Size of Resource Map which is LCM of sps 
 *                           peridocities. For 20ms & 40ms support, RM size=40
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This will intialize the DL SpsTTIContextMap Information.
 ******************************************************************************/
void initDLSpsTTIContextMap(UInt32 rmSize
        ,InternalCellIndex internalCellIndex)
{
    UInt8  ctr   = 0;    
    UInt32 index = 0;
    UInt32 dlRBs = (dlRAInfo_gp[internalCellIndex] +
       dlRAInfoIndex_g[internalCellIndex])->totalAllowedRB;
    
    for (index = 0; index < rmSize; index++ )
    {
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        spsStrTTIMap_g[internalCellIndex][index].sfType = getSubframeType((index % MAX_SUB_FRAME),internalCellIndex);
#endif
        /* - SPS_TDD_Changes */

        spsStrTTIMap_g[internalCellIndex][index].index = index;
        spsStrTTIMap_g[internalCellIndex][index].numDlSpsDciPerTti=0;
        for( ctr = 0; ctr < SPS_INTERVAL_MAX; ctr++ )
        {
            spsStrTTIMap_g[internalCellIndex][index].numSpsRbsPerTtiPerInterval[ctr] = 0;
        }
        spsStrTTIMap_g[internalCellIndex][index].numDlSpsOccasionPerTti = 0;        
        spsStrTTIMap_g[internalCellIndex][index].numDlSpsRbsPerTti=0;
        spsStrTTIMap_g[internalCellIndex][index].indexInSortedArray = index;
        initSpsRAT2LocalizedInfo( &spsStrTTIMap_g[internalCellIndex][index].spsRAT2LocalizedInfo,
                                  dlRBs);
        QINIT_MAC_SPS_UE_LIST_PER_TTI(spsStrTTIMap_g[internalCellIndex][index].spsDciList);
        QINIT_MAC_SPS_UE_LIST_PER_TTI(spsStrTTIMap_g[internalCellIndex][index].spsOccasionList);
        
        /* Initialize sorted arrays for 20 and 40 ms periodicity */
        sortedArr_g[internalCellIndex][index].index=index;
        sortedArr_g[internalCellIndex][index].numSpsUeinRm = 0;
        
        for( ctr = 0; ctr < 13; ctr++ )
        {
            spsStrTTIMap_g[internalCellIndex][index].spsTTIBitMap[ctr] = 0xFF;
        }
        
        /* Reserve MIB and P-SS/S-SS resources at index=0*/
        if(index %10 == 0)
        {
            reserveCommonChannelRBInRM( 
            &spsStrTTIMap_g[internalCellIndex][index].spsRAT2LocalizedInfo,
            &spsStrTTIMap_g[internalCellIndex][index].spsTTIBitMap[0] 
            ,internalCellIndex);
        }
    }
}
/*<!-CSR 51542 CHANGE START-!>*/
/******************************************************************************
 * Function Name  : deInitAfterCellDeleteDL 
 * Inputs         : rmSize
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This will release the semaphore resources
 ******************************************************************************/
void deInitAfterCellDeleteDL (UInt32 rmSize, InternalCellIndex internalCellIndex)
{
    UInt32 ctr = 0;
    for(ctr =0 ;ctr < rmSize;ctr++)
    {
        semDestroy((LTE_SEM*)&(spsStrTTIMap_g[internalCellIndex][ctr].spsDciList).sem_lock);
        semDestroy((LTE_SEM*)&(spsStrTTIMap_g[internalCellIndex][ctr].spsOccasionList).sem_lock);
    }
}
/*<!-CSR 51542 CHANGE END-!>*/

/***********************************************************************************
 * Function Name  : processDlWBRsvReq
 * Inputs         : wideBandMcsIndex,
 *                  spsDlInfo_p - DL SPS Context,
 *                  macSpsResReq_p -  Node filled by DL SPS Strategy for SPS-PDSCH,
 *                  bestVRBSpanInfo_p - Pointer to BestVRBSpanInfo,
 *                  ttiIdx - TTI Index,
 *                  maxTBSize - Maximum TB size
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will process UE WB information for resource reserve in RM.
 ***********************************************************************************/
MacRetType processDlWBRsvReq( UInt8 *wideBandMcsIndex,
    LP_SpsDlInfo spsDlInfo_p, 
    /* +- SPR 17777 */
    BestVRBSpanInfo *bestVRBSpanInfo_p,
    UInt8 *ttiIdx, 
    UInt32 *maxTBSize, 
    /* + SPS_TDD_Changes */
    DLUEContext *ueContext_p,
    /* - SPS_TDD_Changes */
    InternalCellIndex internalCellIndex)
{
    UInt8  ctr                  = 0;
    UInt8  requiredRBCount      = 0;
    UInt8  ttiIndex             = 0;
    UInt8 spsTTIBitMapUnion[13] = {0};
    LP_SpsStrIntervalTTISorted  spsTTIInfoArr_p  = PNULL;            
    RAT2LocalizedVRBInfo  *localizedVRBInfo_p    = PNULL;
    UInt8 *spsTTIBitMapUnion_p  = spsTTIBitMapUnion;
    RAT2LocalizedInfo           tempLocalizedInfo;
    MacRetType  retVal          = MAC_SUCCESS;
    UInt16 spsInterval          = 
        spsDlInfo_p->spsDlSetupInfo_p->semiPersistentSchedIntervalDL;
    /* + SPS_TDD_Changes */
    /*SPR 16904 changes - START */
    UInt8  actTtiIndex  = 0;
    /*SPR 16904 changes - END */
    UInt8 orgRequiredRB = 0;
    /* - SPS_TDD_Changes */

    InitParams *initParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    memSet(&tempLocalizedInfo,0,sizeof(RAT2LocalizedInfo));
    spsTTIInfoArr_p = sortedArr_g[internalCellIndex];
    if(*wideBandMcsIndex > 
        (initParams_p->dlSchdConfig.spsDlIntervalProfile[spsInterval].dlMinMcsSps + \
         initParams_p->dlSchdConfig.spsDlIntervalProfile[spsInterval].dlMcsMargin) )
    {
        *wideBandMcsIndex = SPS_MIN( 15, SPS_MAX( 
              initParams_p->dlSchdConfig.spsDlIntervalProfile[spsInterval].dlMinMcsSps ,
              *wideBandMcsIndex - initParams_p->dlSchdConfig.spsDlIntervalProfile[
                                                 spsInterval].dlMcsMargin) );

        /* SPR 5999 changes start */
        /* + SPS_TDD_Changes */
        orgRequiredRB = DETERMINE_RBs_DL(*wideBandMcsIndex, *maxTBSize,internalCellIndex);
        /* - SPS_TDD_Changes */
        /* SPR 5999 changes end */
            
        if( SPS_INTERVAL_40 == spsIntervalMap_g[spsInterval] )
        {
            for( ttiIndex = 0 ; (ttiIndex < SPS_INTERVAL_40) ; ttiIndex++ ) 
            {
                /* + SPS_TDD_Changes */
                /*SPR 16904 changes - START */
                actTtiIndex = spsTTIInfoArr_p[ttiIndex].index;
                /*SPR 16904 changes - END */
                if( (spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].\
                            numDlSpsDciPerTti == (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                            spsDlSchInfoPerTti.maxDlSpsDciPerTti)  ||

                        ((spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].\
                          numDlSpsOccasionPerTti + spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].\
                          numDlSpsDciPerTti) >= (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                         spsDlSchInfoPerTti.maxDlSpsOccasionPerTti)  ||

                        ( DRX_STATE_ON == spsDlInfo_p->ueDrxTTI[spsTTIInfoArr_p[ttiIndex].index])  || 
                        ( MEAS_GAP_ON == spsDlInfo_p->ueMeasGapTTI[spsTTIInfoArr_p[ttiIndex].index]) 
#ifdef TDD_CONFIG
                        ||(UL == spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].sfType) 
#endif
                  ) 
                    /* - SPS_TDD_Changes */

                {
                    continue;
                }
                /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                if((SP == spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].sfType) &&
                        ((SPECIAL_SUB_FRAME_CONFIG_0 == cellSpecificParams_g.\
                          cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                          splSubFramePatterns)||
                         (SPECIAL_SUB_FRAME_CONFIG_5 == cellSpecificParams_g.\
                          cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                          splSubFramePatterns)))  
                {
                    continue;
                }
#endif
                /*In case of special subframe , we can use 75% area of allocated RB
                 * As we can't segment the SPS data so we need to increase the RB such that SPS data 
                 *can be fit in that RB.
                 *Here is the algo which has been applied through simulation
                 */
                requiredRBCount = orgRequiredRB;
#ifdef TDD_CONFIG
                if (SP == spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].sfType)
                {
                    requiredRBCount = ceil_wrapper(FOUR_BY_THREE(orgRequiredRB));
                }
#endif

                if (((requiredRBCount + spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].numDlSpsRbsPerTti) >
                            ((macReconfigSchedulerParamsDL_gp[internalCellIndex] +  schParamsIndexDL_g[internalCellIndex])->\
                            spsDlSchInfoPerTti.maxDlSpsRbsPerTti)) ||
                        ((requiredRBCount + spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].numSpsRbsPerTtiPerInterval[spsInterval]) >
                            ((macReconfigSchedulerParamsDL_gp[internalCellIndex] +  schParamsIndexDL_g[internalCellIndex])->\
                            spsDlSchInfoPerTti.maxDlSpsRbsPerTtiPerInterval[spsInterval]))) 
                {
                    continue;
                }
                /* - SPS_TDD_Changes */
                RAT2LocalizedInfo *spsTTIRAT2Localized_p =
                  &spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].\
                  spsRAT2LocalizedInfo;
                if( MAC_SUCCESS == getBestContLocalizedVRBNewTx(
                                requiredRBCount,
                                bestVRBSpanInfo_p,
                                spsTTIRAT2Localized_p,
                                CC_CE_REGION,
                                requiredRBCount,
                                internalCellIndex))
                {
                   break;
                }
            }
        }
        else if( SPS_INTERVAL_20 == spsIntervalMap_g[spsInterval] )
        {
            for( ttiIndex = 0; ttiIndex <  SPS_INTERVAL_20; ttiIndex++ ) 
            {                
                /* + SPS_TDD_Changes */
                /*SPR 16904 changes - START */
                if (SETUP == ueContext_p->drxConfigType || SETUP == ueContext_p->measGapConfigType)
                {
                    actTtiIndex = ttiIndex;

                    if( (spsStrTTIMap_g[internalCellIndex][ttiIndex].\
                                numDlSpsDciPerTti == (macReconfigSchedulerParamsDL_gp[internalCellIndex] + \
                                schParamsIndexDL_g[internalCellIndex])->\
                                spsDlSchInfoPerTti.maxDlSpsDciPerTti) ||

                            ( (spsStrTTIMap_g[internalCellIndex][ttiIndex].numDlSpsOccasionPerTti + 
                               spsStrTTIMap_g[internalCellIndex][ttiIndex].\
                                numDlSpsDciPerTti) >= (macReconfigSchedulerParamsDL_gp[internalCellIndex] + \
                                schParamsIndexDL_g[internalCellIndex])->\
                              spsDlSchInfoPerTti.maxDlSpsOccasionPerTti) ||

                            ( ( ( DRX_STATE_ON == spsDlInfo_p->ueDrxTTI[ttiIndex] ) || 
                                ( DRX_STATE_ON == spsDlInfo_p->ueDrxTTI[ttiIndex + SPS_INTERVAL_20]) ) ||

                              ( (MEAS_GAP_ON == spsDlInfo_p->ueMeasGapTTI[ttiIndex]) ||
                                (MEAS_GAP_ON == spsDlInfo_p->ueMeasGapTTI[ttiIndex + SPS_INTERVAL_20]) ) )
#ifdef TDD_CONFIG
                            || (UL == spsStrTTIMap_g[internalCellIndex][ttiIndex].sfType) 
#endif
                      ) 
                        /* - SPS_TDD_Changes */
                    {
                        continue;
                    }
                }
                else
                {
                    actTtiIndex = spsTTIInfoArr_p[ttiIndex].index;
                    if( (spsStrTTIMap_g[internalCellIndex][actTtiIndex].\
                                numDlSpsDciPerTti == (macReconfigSchedulerParamsDL_gp[internalCellIndex] + \
                                schParamsIndexDL_g[internalCellIndex])->\
                                spsDlSchInfoPerTti.maxDlSpsDciPerTti) || 

                            ((spsStrTTIMap_g[internalCellIndex][actTtiIndex].\
                              numDlSpsOccasionPerTti + spsStrTTIMap_g[internalCellIndex][actTtiIndex].
                              numDlSpsDciPerTti ) >= (macReconfigSchedulerParamsDL_gp[internalCellIndex] + \
                              schParamsIndexDL_g[internalCellIndex])->\
                             spsDlSchInfoPerTti.maxDlSpsOccasionPerTti)
#ifdef TDD_CONFIG
                            || (UL == spsStrTTIMap_g[internalCellIndex][actTtiIndex].sfType) 
#endif
                      )
                    {
                        continue;
                    } 
                }
#ifdef TDD_CONFIG
                if((SP == spsStrTTIMap_g[internalCellIndex][actTtiIndex].sfType) &&
                        ((SPECIAL_SUB_FRAME_CONFIG_0 == cellSpecificParams_g.\
                          cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                          splSubFramePatterns)||
                         (SPECIAL_SUB_FRAME_CONFIG_5 == cellSpecificParams_g.\
                          cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                          splSubFramePatterns)))
                {
                    continue;
                }
#endif
                /*In case of special subframe , we can use 75% area of allocated RB
                 * As we can't segment the SPS data so we need to increase the RB such that SPS data 
                 *can be fit in that RB.
                 *Here is the algo which has been applied through simulation
                 */
                requiredRBCount = orgRequiredRB ;
#ifdef TDD_CONFIG
                if (SP == spsStrTTIMap_g[internalCellIndex][actTtiIndex].sfType)
                {
                    requiredRBCount = ceil_wrapper(FOUR_BY_THREE(orgRequiredRB));
                }
#endif
                if(((requiredRBCount + spsStrTTIMap_g[internalCellIndex][actTtiIndex].numDlSpsRbsPerTti) >
                            (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                            spsDlSchInfoPerTti.maxDlSpsRbsPerTti) ||
                        ((requiredRBCount + spsStrTTIMap_g[internalCellIndex][actTtiIndex].numSpsRbsPerTtiPerInterval[spsInterval]) >
                         (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
                         spsDlSchInfoPerTti.maxDlSpsRbsPerTtiPerInterval[spsInterval]) )
                {
                    continue;
                }
                /* - SPS_TDD_Changes */
               for (ctr = 0 ; ctr < 13; ctr++ )
               {
                   spsTTIBitMapUnion[ctr] = spsStrTTIMap_g[internalCellIndex][
                       actTtiIndex].spsTTIBitMap[ctr] & 
                       spsStrTTIMap_g[internalCellIndex][(actTtiIndex + \
                       SPS_INTERVAL_20) % MAX_SPS_INTERVAL ].\
                       spsTTIBitMap[ctr];
               }
                /*SPR 16904 changes - END */
               localizedVRBInfo_p =&tempLocalizedInfo.rat2LocalizedVRBInfo[0];
               tempLocalizedInfo.maxLocalizedVRB = MAX_LOCALIZED_VRB;
               getRAT2LocalizedFromSpsTti( spsTTIBitMapUnion_p, 
                                           localizedVRBInfo_p, 0, 0, 0);
               if( MAC_SUCCESS == getBestContLocalizedVRBNewTx(requiredRBCount,
                                          bestVRBSpanInfo_p,
                                          &tempLocalizedInfo,
                                          CC_CE_REGION,
                                          requiredRBCount,
                                          internalCellIndex))
                   break;
             }
        }
    }
    /*!
     * \code
     * RM Failure:Put entry in dlSPSStatusQ with PDSCH fail.
     * \endcode
     * */
    if (!bestVRBSpanInfo_p->startingVRBIndex)
    {
        retVal =  MAC_FAILURE;
    }
    if(MAX_SPS_INTERVAL > ttiIndex)
    {
        /* + SPS_TDD_Changes */
        /*SPR 16904 changes - START */
        *ttiIdx = actTtiIndex;
        /*SPR 16904 changes - END */
        /* - SPS_TDD_Changes */
    }

    return retVal;
}

/*********************************************************************************
 * Function Name  : processDLResourceReserveRequest
 * Inputs         : ueContext_p - DL UEContext,
 *                  macSpsResReq_p - Node filled by DL SPS Strategy for SPS-PDSCH,
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This will process the DL Resource Reserve request.
 **********************************************************************************/
void processDLResourceReserveRequest(DLUEContext *ueContext_p, 
          LP_MacDlSpsResReq macSpsResReq_p
          ,InternalCellIndex internalCellIndex)
{
    UInt8        mcsIndex  = 0;
    UInt8        ttiIndex  = 0;
    UInt8        index     = 0;
    LP_SpsDlInfo spsDlInfo_p = &(ueContext_p->spsDlInfo);
    MacRetType retVal = MAC_SUCCESS;
    BestVRBSpanInfo   bestVRBSpanInfo           = {0,0};
    UInt32 vrbIndex             = 0;
    UInt32 previousFreeVRBIndex = 0;
    UInt32 nextFreeVRBIndex     = 0;
    UInt8  requiredRBCount      = 0;
    UInt8 *spsTtiBitMap_p       = PNULL;
    LP_MacDlSpsStatus dataReq_p = PNULL;
    LP_SpsStrIntervalTTIContext ttiContext_p              = PNULL;
    LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;
    RAT2LocalizedVRBInfo        *localizedVRBInfo_p       = PNULL;
    UInt32 maxTBSize            = 0;
    UInt16 spsInterval          = 
          spsDlInfo_p->spsDlSetupInfo_p->semiPersistentSchedIntervalDL;

        /* Provide SPS opp only on basis of OAM configured grant in SPS PH-1 */  
        maxTBSize = macSpsResReq_p->avgRlcSduSize;

    if (!ueContext_p->dlCQIInfo.isSubBandReportAvailable)
    {
         mcsIndex =  ueContext_p->dlCQIInfo.mcsIndexCodeWordOne;
         retVal = processDlWBRsvReq( &mcsIndex, 
                     /* +- SPR 17777 */
                     spsDlInfo_p,&bestVRBSpanInfo, &ttiIndex,
                     /* +- SPR 17777 */
                     /* + SPS_TDD_Changes */
                     &maxTBSize, ueContext_p, internalCellIndex);
                     /* - SPS_TDD_Changes */

    }
    else
    {
        retVal = processDlSBRsvReq( spsDlInfo_p, &(ueContext_p->dlCQIInfo.\
                   subBandReportInfo_p->cqiInfo.cqiInfoNonSpatialMul.\
                   /* +- SPR 17777 */
                   rat0NonSpatialMulCQIInfo),
                   /* +- SPR 17777 */
                   &mcsIndex, &bestVRBSpanInfo, &ttiIndex, &maxTBSize 
                   ,internalCellIndex);
    }
    
    if ( MAC_FAILURE == retVal )
    {
          dataReq_p = spsDLAllocateStatusDataReq( macSpsResReq_p->ueIndex,
                         SPS_PDSCH_PUSCH_FAILURE, MAC_SPS_RES_RSV_REQ_ACT, 
                         maxTBSize, requiredRBCount);
          /*! \code
           * RM Success:Put entry in dlSPSStatusQ with PDSCH fail. 
           * Output:ttiIndex,MCS,RBs
           * \endcode
           * */
	        /* KLOCWORK_FIX_START_62_63 */
	        if(dataReq_p == PNULL)
	        {
        	    ltePanic("ttiContextPeriodicity20_p = NULL %s",__FUNCTION__);
                /* Free the memory */
                freeMemPool ( macSpsResReq_p );
                return;
	        }
	        /* KLOCWORK_FIX_END_62_63 */
    }
    else
    {
        /*! \code
	 * Update spsTTIInfoArr_p[ttiIndex].spsRat2Localized with 
         * bestVRBSpanInfo such that a Free VRB knows the next free VRB Index
         * and also previous Free VRB Index
	 * \endcode
	 * */        
        requiredRBCount = bestVRBSpanInfo.endingVRBIndex - \
				  bestVRBSpanInfo.startingVRBIndex + 1;

	localizedVRBInfo_p = &spsStrTTIMap_g[internalCellIndex][ttiIndex].spsRAT2LocalizedInfo.\
                                                     rat2LocalizedVRBInfo[0];
	spsTtiBitMap_p = spsStrTTIMap_g[internalCellIndex][ttiIndex].spsTTIBitMap;
	vrbIndex = bestVRBSpanInfo.startingVRBIndex;
	previousFreeVRBIndex = localizedVRBInfo_p[vrbIndex].\
                                              previousVRBfreeIndex;
	nextFreeVRBIndex = localizedVRBInfo_p[vrbIndex + \
                               requiredRBCount - 1].nextfreeVRBIndex;
	localizedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex =
					nextFreeVRBIndex;
	localizedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex =
					previousFreeVRBIndex;

    spsStrTTIMap_g[internalCellIndex][ttiIndex].spsRAT2LocalizedInfo.numOfVRBAvailable -= 
                                requiredRBCount;

	/*! \code
	 * Update spsTTIInfoArr_p[ttiIndex].spsTTIBitMap with bestVRBSpanInfo
	 * \endcode
	 * */
	updateSpsTtiBitMap(spsTtiBitMap_p,bestVRBSpanInfo.startingVRBIndex,
				bestVRBSpanInfo.endingVRBIndex,SPS_RESERVE);

        /*SPR 6690 fix Start */
        localizedVRBInfo_p[bestVRBSpanInfo.endingVRBIndex].nextfreeVRBIndex=0xff;
        /*SPR 6690 fix End */
	if( SPS_INTERVAL_20 == spsIntervalMap_g[spsInterval])
	{
	    localizedVRBInfo_p =
	        &spsStrTTIMap_g[internalCellIndex][(ttiIndex + SPS_INTERVAL_20) % 
                MAX_SPS_INTERVAL].spsRAT2LocalizedInfo.rat2LocalizedVRBInfo[0];
            spsTtiBitMap_p = spsStrTTIMap_g[internalCellIndex][(ttiIndex + SPS_INTERVAL_20) % 
                        MAX_SPS_INTERVAL].spsTTIBitMap;
	    vrbIndex = bestVRBSpanInfo.startingVRBIndex;
	    previousFreeVRBIndex = 
                  localizedVRBInfo_p[vrbIndex].previousVRBfreeIndex;
	    nextFreeVRBIndex = localizedVRBInfo_p[vrbIndex + \
                          requiredRBCount - 1].nextfreeVRBIndex;
	    localizedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex =
							nextFreeVRBIndex;
	    localizedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex =
           						previousFreeVRBIndex;

        spsStrTTIMap_g[internalCellIndex][(ttiIndex + SPS_INTERVAL_20) %MAX_SPS_INTERVAL].spsRAT2LocalizedInfo.numOfVRBAvailable -=
                             requiredRBCount;
	    /*! \code
	     * Update spsTTIInfoArr_p[ttiIndex].spsTTIBitMap with 
             * bestVRBSpanInfo
	     * \endcode
	     * */
	    updateSpsTtiBitMap( spsTtiBitMap_p, 
                                bestVRBSpanInfo.startingVRBIndex,
                                bestVRBSpanInfo.endingVRBIndex,
                                SPS_RESERVE );
        /*SPR 6690 fix Start */
            localizedVRBInfo_p[bestVRBSpanInfo.endingVRBIndex].nextfreeVRBIndex=0xff;
        /*SPR 6690 fix End */
	}

	ttiContext_p = &spsStrTTIMap_g[internalCellIndex][ttiIndex];

	spsDlInfo_p->resAllocInfo.allocatedMCS = mcsIndex;
	spsDlInfo_p->resAllocInfo.startingVRBIndex = 
                                 bestVRBSpanInfo.startingVRBIndex;
	spsDlInfo_p->resAllocInfo.allocatedRB = requiredRBCount;          
	ttiContext_p->numDlSpsRbsPerTti += requiredRBCount;
	ttiContext_p->numSpsRbsPerTtiPerInterval[spsInterval] += 
                                                    requiredRBCount;

	/*! \code
	 * RM Success:Put entry in dlSPSStatusQ with PDSCH success. 
	 * Output:ttiIndex,MCS,RBs
	 * \endcode
	 * */         
	if( SPS_INTERVAL_20 == spsIntervalMap_g[spsInterval] )
	{ 
	    ttiContextPeriodicity20_p = 
                &spsStrTTIMap_g[internalCellIndex][(ttiIndex + SPS_INTERVAL_20) % 
                MAX_SPS_INTERVAL];
	    ttiContextPeriodicity20_p->numDlSpsRbsPerTti += requiredRBCount;
	    ttiContextPeriodicity20_p->numSpsRbsPerTtiPerInterval[
        				     spsInterval] +=requiredRBCount;
	    index = SPS_MIN( ttiIndex, 
	             (ttiIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL);

	    ttiContext_p = &spsStrTTIMap_g[internalCellIndex][index];
        /* + Coverity 10825 */
	    ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
					(index + SPS_INTERVAL_20) % MAX_SPS_INTERVAL];
        /* - Coverity 10825 */
	    /* Update key */
		(sortedArr_g[internalCellIndex][ttiContextPeriodicity20_p->indexInSortedArray].\
		 numSpsUeinRm)++;
	}
	/* CA Stats Changes Start */
	/* Update RM stats */
	LTE_MAC_UPDATE_SPS_PDSCH_TTI_MAP_STATS( spsDlInfo_p, ttiContext_p,
			ttiContextPeriodicity20_p, getCurrentTick(), internalCellIndex);
	/* CA Stats Changes End */
	/* Update key */
	/*coverity fix 19397*/
	(sortedArr_g[internalCellIndex][ttiContext_p->indexInSortedArray].numSpsUeinRm)++;
	/*coverity fix 19397*/

	/* Sort periodicity TTI map */   
	spsSort( sortedArr_g[internalCellIndex], ttiContext_p->indexInSortedArray, 
			SPS_INTERVAL_40
			,internalCellIndex);

	dataReq_p = spsDLAllocateStatusDataReq( macSpsResReq_p->ueIndex,
			SPS_PDSCH_PUSCH_SUCCESS, MAC_SPS_RES_RSV_REQ_ACT, 
                        maxTBSize, requiredRBCount);
	/* Fill TTI Index for activation */
	/* KLOCWORK_FIX_START_62_63 */
	if(dataReq_p == PNULL)
	{
        	ltePanic("ttiContextPeriodicity20_p = NULL %s",__FUNCTION__);
            /* Free the memory */
            freeMemPool ( macSpsResReq_p );
            return;
	}
	/* KLOCWORK_FIX_END_62_63 */
	dataReq_p->index = ttiIndex;
    }
    /* + Coverity Id - 32597 */
    if(!ENQUEUE_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex], dataReq_p ))
    {
        freeMemPool(dataReq_p);
    }
    /* - Coverity Id - 32597 */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
            QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex] ),
            internalCellIndex); 
    /* CA Stats Changes End */
    /* Free the memory */
    freeMemPool ( macSpsResReq_p );
}

/**********************************************************************************
 * Function Name  : processDlSBRsvReq
 * Inputs         : spsDlInfo_p - SPS DL Context,
 *                  rat0CQIInfo_p - Pointer to RAT0NonSpatialMulCQIInfo,
 *                  macSpsResReq_p - Node filled by DL SPS Strategy for SPS-PDSCH,
 *                  sbMCSIndex - MCS Index,
 *                  bestVRBSpanInfo_p Pointer to BestVRBSpanInfo,
 *                  ttiIdx - TTI Index,
 *                  maxTBSize - Maximum TB Size
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will process DL UE SB information for resource reserve 
 *                  in RM.
 **********************************************************************************/
MacRetType processDlSBRsvReq( LP_SpsDlInfo spsDlInfo_p, 
     RAT0NonSpatialMulCQIInfo *rat0CQIInfo_p,
     /* +- SPR 17777 */
     UInt8* sbMCSIndex, 
     BestVRBSpanInfo *bestVRBSpanInfo_p,
     UInt8 *ttiIdx, UInt32 *maxTBSize 
     ,InternalCellIndex internalCellIndex)
{
    UInt8  ctr                  = 0;
    UInt8  ttiIndex             = 0;
    UInt8  cqiReportIndex       = 0;
    UInt8  reportedMCSIndex     = 0;
    UInt8  spsRBs               = 0;
    UInt8  requiredRBCount      = 0;
    UInt16 reportedCQIBitMap    = 0;
    UInt32 cqiIndexCount        = 0;
    UInt32 cqiBitMap            = 0;
    UInt8 spsTTIBitMapUnion[13] = {0};
    UInt8 *spsTTIBitMapUnion_p  = spsTTIBitMapUnion;
    MacRetType  retVal          = MAC_SUCCESS;
    LP_SpsStrIntervalTTISorted spsTTIInfoArr_p = PNULL;            
    UInt16 spsInterval       = 
          spsDlInfo_p->spsDlSetupInfo_p->semiPersistentSchedIntervalDL;
    spsTTIInfoArr_p = sortedArr_g[internalCellIndex];
    /* + SPS_TDD_Changes */
    UInt8 orgRequiredRB = 0;
    /* - SPS_TDD_Changes */
    for (ttiIndex = 0; ttiIndex < MAX_SPS_INTERVAL; ttiIndex++)
    {
        /* + SPS_TDD_Changes */
        /*Coverity 87589:Fix Indentation corrected*/
        if( (spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].\
            numDlSpsDciPerTti == (macReconfigSchedulerParamsDL_gp[internalCellIndex] +schParamsIndexDL_g[internalCellIndex])->\
            spsDlSchInfoPerTti.maxDlSpsDciPerTti)  ||
            ( DRX_STATE_ON == spsDlInfo_p->ueDrxTTI[spsTTIInfoArr_p[ttiIndex].index]) || 
            ( MEAS_GAP_ON == spsDlInfo_p->ueMeasGapTTI[spsTTIInfoArr_p[ttiIndex].index]) 
#ifdef TDD_CONFIG
                ||(UL == spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].sfType) 
#endif
          ) 
            /* - SPS_TDD_Changes */
        {
            continue;
        }
        /* + SPS_TDD_Changes */
        if( SPS_INTERVAL_20 == spsIntervalMap_g[spsInterval] )
        {
            if (( DRX_STATE_ON == spsDlInfo_p->ueDrxTTI[spsTTIInfoArr_p[ (ttiIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL ].index]) || 
                    ( MEAS_GAP_ON == spsDlInfo_p->ueMeasGapTTI[spsTTIInfoArr_p[ (ttiIndex + SPS_INTERVAL_20)% MAX_SPS_INTERVAL ].index]))
            {
                continue;
            }

        }
#ifdef TDD_CONFIG
        if((SP == spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].sfType) &&
                ((SPECIAL_SUB_FRAME_CONFIG_0 == cellSpecificParams_g.\
                  cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                  splSubFramePatterns) ||
                 (SPECIAL_SUB_FRAME_CONFIG_5 == cellSpecificParams_g.\
                  cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                  splSubFramePatterns)))  
        {
            continue;
        }
#endif
        /* - SPS_TDD_Changes */
        RAT2LocalizedInfo *spsTTIRAT2Localized_p =
            &spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].\
            spsRAT2LocalizedInfo;
        reportedCQIBitMap = rat0CQIInfo_p->reportedCQIBitMap;
        while (reportedCQIBitMap)
        {
            /* find out first reported CQI */
            cqiReportIndex = FFS(reportedCQIBitMap);
            reportedCQIBitMap &= (~((UInt16)1 << (cqiReportIndex-1)));
            cqiIndexCount = (MAX_CQI_INDEX - cqiReportIndex);

            reportedMCSIndex = getMCSFromCQI(cqiIndexCount
						);
            if(reportedMCSIndex > 15 || 
                reportedMCSIndex < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                dlSchdConfig.spsDlIntervalProfile[
                  spsInterval].dlMinMcsSps + \
                  cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
               dlSchdConfig.spsDlIntervalProfile[spsInterval].dlMcsMargin )
            {
                continue;
            }
            /*start with the highest reported CQI Index*/
            cqiBitMap = rat0CQIInfo_p->rat0CQIBitMap[cqiIndexCount];

            /* min and max RBs required at reported CQI for min and max TB size
             * as instructed by DL Strategy*/
            /* SPR 5999 changes start */
            /* + SPS_TDD_Changes */
            orgRequiredRB = DETERMINE_RBs_DL(reportedMCSIndex, *maxTBSize,internalCellIndex);
            /* - SPS_TDD_Changes */
            /* SPR 5999 changes end */
            /* + SPS_TDD_Changes */
            /*In case of special subframe , we can use 75% area of allocated RB
             * As we can't segment the SPS data so we need to increase the RB such that SPS data 
             *can be fit in that RB.
             *Here is the algo which has been applied through simulation
             */
            spsRBs = orgRequiredRB;
#ifdef TDD_CONFIG
            if (SP == spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].sfType)
            {
                    spsRBs = ceil_wrapper(FOUR_BY_THREE(orgRequiredRB));
            }
#endif
            /* - SPS_TDD_Changes */

            if( SPS_INTERVAL_20 == spsIntervalMap_g[spsInterval] )
            {
               for (ctr = 0 ; ctr < 13; ctr++ )
               {
                    spsTTIBitMapUnion[ctr] = 
                         spsStrTTIMap_g[internalCellIndex][spsTTIInfoArr_p[ttiIndex].index].\
                             spsTTIBitMap[ctr] &
                         spsStrTTIMap_g[internalCellIndex][((spsTTIInfoArr_p[ttiIndex].index + 
                             SPS_INTERVAL_20)%MAX_SPS_INTERVAL)].\
                             spsTTIBitMap[ctr];
               }
               if( 1 == *spsTTIBitMapUnion )
                   continue;
               RAT2LocalizedInfo tempLocalizedVRBInfo;
               memSet(&tempLocalizedVRBInfo,0,sizeof(RAT2LocalizedInfo));
               RAT2LocalizedVRBInfo *localizedVRBInfo_p =
                        tempLocalizedVRBInfo.rat2LocalizedVRBInfo;
               getRAT2LocalizedFromSpsTti( spsTTIBitMapUnion_p, 
                                           localizedVRBInfo_p, 0, 0, 0);

               /* For each reported CQI and their corresponding reported 
                * subbands try to find out contiguous VRBs which are available
                * and reported too */
               if( MAC_SUCCESS == getReportedLocalizedVRBNewTx(
                                bestVRBSpanInfo_p,
                                &tempLocalizedVRBInfo,
                                spsRBs,
                                spsRBs,
                                cqiBitMap,
                                CC_CE_REGION))
               {
                   requiredRBCount = (bestVRBSpanInfo_p->endingVRBIndex -
                               bestVRBSpanInfo_p->startingVRBIndex) + 1;
                   if(requiredRBCount<spsRBs)
                      requiredRBCount=0;
                   else
                      break;
               }
            }
            else if( SPS_INTERVAL_40 == spsIntervalMap_g[spsInterval] )
            {
                /* For each reported CQI and their corresponding reported 
                 * subbands try to find out contiguous VRBs which are available
                 * and reported too */
                if( MAC_SUCCESS == getReportedLocalizedVRBNewTx(
                                bestVRBSpanInfo_p,
                                spsTTIRAT2Localized_p,
                                spsRBs,
                                spsRBs,
                                cqiBitMap,
                                CC_CE_REGION))
                {
                    requiredRBCount = (bestVRBSpanInfo_p->endingVRBIndex -
                              bestVRBSpanInfo_p->startingVRBIndex) + 1;
                    if(requiredRBCount<spsRBs)
                        requiredRBCount=0;
                    else
                        break;
                }
            }
        }
        if(requiredRBCount)
           break;
    }

    if(!requiredRBCount)
    {
        /*! \code
         * Update spsTTIInfoArr_p[ttiIndex].spsRat2Localized with 
         * bestVRBSpanInfo such that a Free VRB knows the next Free VRB Index
         * and also previous Free VRB Index
         * \endcode
         * */
        retVal = MAC_FAILURE;

    }

    *sbMCSIndex = reportedMCSIndex;
    if(MAX_SPS_INTERVAL > ttiIndex)
    {
	    *ttiIdx =  spsTTIInfoArr_p[ttiIndex].index;
    }
    return retVal;
}

/******************************************************************************
 * Function Name  : processDLResourceReleaseRequest
 * Inputs         : ueContext_p - DL UEContext
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This will process the DL Resource Release Request.
 ******************************************************************************/
void processDLResourceReleaseRequest(DLUEContext *ueContext_p,InternalCellIndex internalCellIndex)
{

    /*!
     * \code
     * Find out start VRBIndex & end VRBIndex of this UE which need to be freed.
     * \endcode
     * */
    UInt8                       index                     = 0;
    UInt8                       startVRBIndex             = 0;
    UInt8                       endVRBIndex               = 0;
    UInt8                       *spsTtiBitMap_p           = PNULL;
    LP_SpsStrIntervalTTIContext ttiContext_p              = PNULL;
    LP_SpsStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;
    RAT2LocalizedInfo           *localizedInfo_p          = PNULL;
    RAT2LocalizedVRBInfo        *localizedVRBInfo_p       = PNULL;

    LP_SpsDlInfo spsDlInfo_p = &(ueContext_p->spsDlInfo);
    UInt8 spsPeriodicty = 0;
    UInt8  i            = 0;

    spsPeriodicty = 
        spsDlInfo_p->spsDlSetupInfo_p->semiPersistentSchedIntervalDL;
     /*!
     * \code
     * Get SpsStrIntervalTTIContext from spsActivationIndex
     * \endcode
     * */
    UInt8 activationIndex = spsDlInfo_p->ttiIndex;

    startVRBIndex = spsDlInfo_p->resAllocInfo.startingVRBIndex;
    endVRBIndex = startVRBIndex + 
                        spsDlInfo_p->resAllocInfo.allocatedRB -1;

    ttiContext_p = &spsStrTTIMap_g[internalCellIndex][activationIndex];
    ttiContextPeriodicity20_p = &spsStrTTIMap_g[internalCellIndex][
               (activationIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL];

    localizedInfo_p = &(ttiContext_p->spsRAT2LocalizedInfo);
    localizedVRBInfo_p = &(localizedInfo_p->rat2LocalizedVRBInfo[0]);

    spsTtiBitMap_p = &(ttiContext_p->spsTTIBitMap[0]);
    /*!
     * \code
     * Update SpsTTIBitMap of this spsActivationIndex with startVRBIndex &
     * endVRBIndex
     * \endcode
     * */
    updateSpsTtiBitMap( spsTtiBitMap_p, startVRBIndex, endVRBIndex, 
                        MAC_SPS_RES_REL_REQ);
    ttiContext_p->numDlSpsRbsPerTti -= spsDlInfo_p->resAllocInfo.allocatedRB;
     
    ttiContext_p->numSpsRbsPerTtiPerInterval[spsPeriodicty] -= 
         spsDlInfo_p->resAllocInfo.allocatedRB;
    /*!
     * \code
     * Update RAT2LocalizedVRBInfo of this spsActivationIndex with 
     * startVRBIndex & endVRBIndex
     * \endcode
     * */
    /*SPR 6690 fix Start */
    /* + coverity 31399 */
    for( i = endVRBIndex; ;i-- )
    /* - coverity 31399 */
    {
        if (( localizedVRBInfo_p[i].nextfreeVRBIndex !=0xff ) && 
            (localizedVRBInfo_p[i].nextfreeVRBIndex > endVRBIndex) )
        {
            localizedVRBInfo_p[endVRBIndex].nextfreeVRBIndex = localizedVRBInfo_p[i].nextfreeVRBIndex;
            localizedVRBInfo_p[i].nextfreeVRBIndex = startVRBIndex;
            localizedVRBInfo_p[startVRBIndex].previousVRBfreeIndex = i;
            break;
        }
        if( 0 == i )
        {
            break;
        }
    }
    /*SPR 6690 fix End */

    localizedInfo_p->numOfVRBAvailable += 
               spsDlInfo_p->resAllocInfo.allocatedRB;

    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsPeriodicty] )
    {
        startVRBIndex = spsDlInfo_p->resAllocInfo.startingVRBIndex;
        endVRBIndex = startVRBIndex + 
                        spsDlInfo_p->resAllocInfo.allocatedRB -1;

        localizedInfo_p = &(ttiContextPeriodicity20_p->spsRAT2LocalizedInfo);
        localizedVRBInfo_p = &(localizedInfo_p->rat2LocalizedVRBInfo[0]);
        spsTtiBitMap_p = &(ttiContextPeriodicity20_p->spsTTIBitMap[0]);
        /*! \code
         * Update SpsTTIBitMap of this spsActivationIndex with startVRBIndex &
         * endVRBIndex
         * \endcode
         * */
        updateSpsTtiBitMap( spsTtiBitMap_p, startVRBIndex, endVRBIndex, 
                           MAC_SPS_RES_REL_REQ);
        ttiContextPeriodicity20_p->numDlSpsRbsPerTti -= 
              spsDlInfo_p->resAllocInfo.allocatedRB;
     
        ttiContextPeriodicity20_p->numSpsRbsPerTtiPerInterval[spsPeriodicty] -=
              spsDlInfo_p->resAllocInfo.allocatedRB;
        /*! * \code
         * Update RAT2LocalizedVRBInfo of this spsActivationIndex with 
         * startVRBIndex & endVRBIndex
         * \endcode
         * */
        while(startVRBIndex <= endVRBIndex+1)
        {
            localizedVRBInfo_p[startVRBIndex-1].nextfreeVRBIndex = 
                                                       startVRBIndex;
            localizedVRBInfo_p[startVRBIndex].previousVRBfreeIndex = 
                                                       startVRBIndex-1;
            startVRBIndex++;
        }
        localizedInfo_p->numOfVRBAvailable += 
                 spsDlInfo_p->resAllocInfo.allocatedRB;
    }
    spsDlInfo_p->resAllocInfo.allocatedRB = 0;

    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsPeriodicty] )
    { 
       index = SPS_MAX( activationIndex, 
               (activationIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL);

       ttiContext_p = &spsStrTTIMap_g[internalCellIndex][index];
       ttiContextPeriodicity20_p = &(spsStrTTIMap_g[internalCellIndex][
                    (index + SPS_INTERVAL_20) % MAX_SPS_INTERVAL]);

       /* Update sorting key */
       (sortedArr_g[internalCellIndex][ttiContextPeriodicity20_p->indexInSortedArray].
                                    numSpsUeinRm)--;
    }
    /* CA Stats Changes Start */
    /* Update RM stats */
    LTE_MAC_UPDATE_SPS_PDSCH_TTI_MAP_STATS( spsDlInfo_p, ttiContext_p,
            ttiContextPeriodicity20_p, getCurrentTick(),
            internalCellIndex);  
    /* CA Stats Changes End */
    /* Update sorting key */
    (sortedArr_g[internalCellIndex][ttiContext_p->indexInSortedArray].numSpsUeinRm)--;
    /* Sort periodicity TTI map */   
    spsSort( sortedArr_g[internalCellIndex], 0, ttiContext_p->indexInSortedArray + 1, internalCellIndex );

    /* Reset TTI Index in Ue Context */
    spsDlInfo_p->ttiIndex = MAX_SPS_INTERVAL;
}

/******************************************************************************
 * Function Name  : reserveCommonChannelRBInRM
 * Inputs         : spsRAT2LocalizedInfo_p - Pointer to RAT2LocalizedInfo,
 *                  spsTTIBitMap[] -
 *                  internalCellIndex - Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This will reserve centralized 6 RBs in SPS SpsTTIContextMap
 *                  Information.
 ******************************************************************************/
void reserveCommonChannelRBInRM( RAT2LocalizedInfo * spsRAT2LocalizedInfo_p, 
    UInt8 spsTTIBitMap[]
    ,InternalCellIndex internalCellIndex)
{

    RAT2LocalizedVRBInfo *localizedVRBInfo_p= PNULL;
    UInt32 startingRBNumber = 0;
    UInt32 endingRBNumber = 0;
    UInt32 previousFreeVRBIndex = 0;        
    UInt32 nextFreeVRBIndex = 0;        
    UInt32 dlRBs = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
        ->cellParams_p->dlAvailableRBs;
    localizedVRBInfo_p = spsRAT2LocalizedInfo_p->rat2LocalizedVRBInfo;
    if (MODULO_TWO(dlRBs))
    {
        startingRBNumber = DIVIDE_BY_TWO(dlRBs) - 1;
        endingRBNumber =  startingRBNumber + 5;
        startingRBNumber--;
    }
    else
    {
        startingRBNumber = DIVIDE_BY_TWO(dlRBs) - 2;
        endingRBNumber =  startingRBNumber + 5;
        startingRBNumber--;
    }
    /*Updating the localizedVRBInfo_p array such that a Free VRB knows
        the next Free VRB Index and also previous Free VRB Index*/
    previousFreeVRBIndex = localizedVRBInfo_p[endingRBNumber - 5].previousVRBfreeIndex;
    nextFreeVRBIndex = localizedVRBInfo_p[endingRBNumber].nextfreeVRBIndex;
    localizedVRBInfo_p[previousFreeVRBIndex].nextfreeVRBIndex = nextFreeVRBIndex;
    localizedVRBInfo_p[nextFreeVRBIndex].previousVRBfreeIndex = previousFreeVRBIndex;
    spsRAT2LocalizedInfo_p->numOfVRBAvailable -= 6;
    updateSpsTtiBitMap(spsTTIBitMap,startingRBNumber,endingRBNumber, SPS_RESERVE);        
}
