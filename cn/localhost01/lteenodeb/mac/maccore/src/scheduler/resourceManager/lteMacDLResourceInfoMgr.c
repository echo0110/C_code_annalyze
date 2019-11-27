/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacDLResourceInfoMgr
 *
 ******************************************************************************
 *
 *  File Description :  This file contains the Funtions needed for RB  
 *                      Allocation as per cell center and cell edge resources in 
 *                      RAT 0,RAT 1,RAT 2 Localized or RAT 2 Distributed.
 ******************************************************************************
*****************************************************************************/


 /******************************************************************************
  * Standard Library Includes
  *****************************************************************************/

 /******************************************************************************
  * Project Includes
  *****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacDLResourceInfoMgr.h"
#include "lteMacRRMInterface.h"
#include "lteMacPRSMgr.h"
/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* Array of two is used. As one of the index can be used per TTI
 * Another one can be updated from low priority thread. */
DLResourceAllocationInfo *dlRAInfo_gp[MAX_NUM_CELL];
UInt8 dlRAInfoIndex_g[MAX_NUM_CELL] = {0};

/* for storing partition info */
DLResourcePartitionConfigInfo dlResourcePartitionConfig_g[MAX_NUM_CELL];

/* +- SPR 20073 fix */
extern UInt8 mibOverlapRBs_g[MAX_NUM_CELL][MAX_USER_LOCATION];
/******************************************************************************
 * Exported Constants
 *****************************************************************************/
  
 /******************************************************************************
   Private Definitions
  *****************************************************************************/

/****************************************************************************
 * Function Name  : reconfigRAT0Info 
 * Inputs         : rat0Info_p
 *                  resourcePartitionInfo_p - CC/CE resource partition info
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function modifies RAT 0 bitmap for cell center and
 *                  cell edge users.
 ****************************************************************************/

void reconfigRAT0Info(RAT0Info *rat0Info_p,
        DLResourcePartitionConfigInfo *resourcePartitionInfo_p,
        InternalCellIndex internalCellIndex)
{
    UInt32 index = 0;
    UInt32 startBit = 0;
    UInt32 numOfBits = 0;   
    UInt32 rbgSize = 0;
    UInt32 rbIndex = 0;
    UInt32 rbgIndex = 0;

     UInt8   dlAvailableRBs = cellSpecificParams_g.cellConfigAndInitParams_p\
                  [internalCellIndex]->cellParams_p->dlAvailableRBs;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,DL_RESOURCE_MGR,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    rbgSize = rat0Info_p->rbgSize; 
    rat0Info_p->rat0BitMap[CC_USER] = 0;
    rat0Info_p->rat0BitMap[CE_USER] = 0;
   
    /* Update RAT0 bitmap for unblocked RBGs for all cell centre regions */ 
    for(index = 0; 
            index < resourcePartitionInfo_p->cellCenterRegionCount;
            index++)
    {
        startBit = ceil_wrapper((UDouble32)resourcePartitionInfo_p->ccResourceInfo[index].
                startRBIndex/rbgSize);

        numOfBits = ceil_wrapper((UDouble32)(resourcePartitionInfo_p->
                ccResourceInfo[index].endRBIndex + 1) /rbgSize) -
                ceil_wrapper((UDouble32)resourcePartitionInfo_p->ccResourceInfo[index].
                startRBIndex / rbgSize);

        /*SPR 20460 +*/
        if ((numOfBits > 1) &&((dlAvailableRBs != (resourcePartitionInfo_p->
                    ccResourceInfo[index].endRBIndex + 1)) &&
                ((resourcePartitionInfo_p->ccResourceInfo[index].endRBIndex + 
                  1) % rbgSize)))
        {
        /*SPR 20460 -*/
            numOfBits--;
        }
        rat0Info_p->rat0BitMap[CC_USER] |= ((UInt32)pow_wrapper(2,numOfBits) - 1) << startBit;

        /* We should mark those RBG to Cell Centre region, which contain both 
         * cell centre & cell edge PRBs 
         */
        if (resourcePartitionInfo_p->ccResourceInfo[index].startRBIndex % rbgSize)
        {
            rbIndex = resourcePartitionInfo_p->ccResourceInfo[index].startRBIndex;
            
            /* check if neighbouring PRB is unblocked, in this case mark such
             * RBG in cell centre region
             */
            if ((dlRAInfo_gp[internalCellIndex] + (!dlRAInfoIndex_g[internalCellIndex]))->rbToResourceTypeMapDL[rbIndex - 1])
            {
                rbgIndex = rbIndex / rbgSize;
                rat0Info_p->rat0BitMap[CC_USER] |= (1 << rbgIndex);
            }
        }

        if ((resourcePartitionInfo_p->ccResourceInfo[index].endRBIndex + 1) % 
                rbgSize)
        {
            rbIndex = resourcePartitionInfo_p->ccResourceInfo[index].endRBIndex;
            
            /* check if neighbouring PRB is unblocked, in this case mark such
             * RBG in cell centre region
             */
            if ((dlRAInfo_gp[internalCellIndex] + (!dlRAInfoIndex_g[internalCellIndex]))->rbToResourceTypeMapDL[rbIndex + 1])
            {
                rbgIndex = rbIndex / rbgSize;
                rat0Info_p->rat0BitMap[CC_USER] |= (1 << rbgIndex);
            }
        }

    }

    if(0 != resourcePartitionInfo_p->totalCellEdgeRBDL)
    {
        /* In case some of RBs are present in cell edge partition,
         * bitmap for cell edge is updated.If partial RBs of RBG are available
         * then overall RBG is unset.
         * While parsing through partition, we are setting up bit for only those RBG
         * which are available in partition.
         */
        startBit = ceil_wrapper((UDouble32)resourcePartitionInfo_p->ceResourceInfo.
                startRBIndex/rbgSize);
        numOfBits = ceil_wrapper((UDouble32)(resourcePartitionInfo_p->ceResourceInfo.
                endRBIndex + 1) /rbgSize) -
                ceil_wrapper((UDouble32)resourcePartitionInfo_p->ceResourceInfo.
                startRBIndex / rbgSize);
        /*SPR 20460 +*/
        if ((numOfBits > 1) && ((dlAvailableRBs != (resourcePartitionInfo_p->
                    ceResourceInfo.endRBIndex + 1)) &&
                ((resourcePartitionInfo_p->ceResourceInfo.endRBIndex + 1 ) % 
                 rbgSize)))
        {
        /*SPR 20460 -*/
            numOfBits--;
        }

        rat0Info_p->rat0BitMap[CE_USER] |= ((UInt32)pow_wrapper(2,numOfBits) - 1) << startBit;
        /* Cell center bitmap also include cell edge resources */
        rat0Info_p->rat0BitMap[CC_USER] |=  rat0Info_p->rat0BitMap[CE_USER];

    }
    else
    {
        rat0Info_p->rat0BitMap[CE_USER] = 0;
    }
    
    LOG_UT(MAC_RAT0_BITMAP,LOGDEBUG,
            MAC_PDSCH,getCurrentTick(),(UInt32)rat0Info_p->rat0BitMap[CC_USER],
            (UInt32)rat0Info_p->rat0BitMap[CE_USER],DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"\nRAT 0 Bitmap for cell centre and cell edge users");
    
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,DL_RESOURCE_MGR,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
}

/****************************************************************************
 * Function Name  : reconfigRAT1Info 
 * Inputs         : rat1Info_p - pointer to RAT1Info
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function modifies subset bitmaps for cell center and
 *                  cell edge users.
 ****************************************************************************/

void reconfigRAT1Info(RAT1Info *rat1Info_p,
        InternalCellIndex internalCellIndex)
{
    SubsetInfo *subsetInfo_p = PNULL;
    UInt32 subsetCount = 0;
    UInt32 subsetSize = 0;
    UInt32 prbNumber = 0;
    UInt32 subsetSizeCount = 0;
    UInt32 temp = 0;
    UInt32 prbJumpInSubset = 0;
    
    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,DL_RESOURCE_MGR,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
 
    subsetCount = rat1Info_p->subsetCount;
    subsetSize = subsetCount;
    subsetInfo_p = &rat1Info_p->subsetInfo[subsetCount - 1];
    prbJumpInSubset = subsetSize * (subsetSize - 1);

    while ( subsetCount-- )
    {
        subsetInfo_p->subsetBitMap[CC_USER] = 0;
        subsetInfo_p->subsetBitMap[CE_USER] = 0;
        subsetInfo_p->numPRBFree = 0;
        prbNumber = subsetSize * subsetCount;  
        /* Parse each PRB in given subset and check if it is blocked or not.
         * For unblocked PRBs update subsetBitMap[CC_USER]
         * If PRB lies in cell edge resource region, update 
         * subsetBitMap[CE_USER] 
         */
        while (prbNumber < cellSpecificParams_g.cellConfigAndInitParams_p\
                        [internalCellIndex]->cellParams_p->dlAvailableRBs)
        {
            temp = ((prbNumber / subsetSize) - subsetCount);
            subsetSizeCount = subsetSize;
            
            while(subsetSizeCount--)
            {
                if((dlRAInfo_gp[internalCellIndex] + (!dlRAInfoIndex_g[internalCellIndex]))->rbToResourceTypeMapDL[prbNumber])
                {
                    subsetInfo_p->subsetBitMap[CC_USER] |= 1 << temp;
                    subsetInfo_p->numPRBFree++;
                    
                    if (CELL_EDGE_ONLY_REGION == (dlRAInfo_gp[internalCellIndex] + (!dlRAInfoIndex_g[internalCellIndex]))->\
                            rbToResourceTypeMapDL[prbNumber])
                    {
                        subsetInfo_p->subsetBitMap[CE_USER] |= 1 << temp;
                    }
                }
                prbNumber++;
                temp++;
            }
            prbNumber += prbJumpInSubset;
        }
        /* bitMapWithSpanZero and bitMapWithSpanNonZero are updated
         * with subsetBitMap value.
         */
        subsetInfo_p->bitMapWithSpanZero = subsetInfo_p->subsetBitMap[CC_USER] >>
            subsetInfo_p->spanWidth;
        subsetInfo_p->bitMapWithSpanNonZero = subsetInfo_p->bitMapWithSpanZero
            <<  subsetInfo_p->spanWidth;
        
        LOG_UT(MAC_RAT1_SUBSET_INFO,LOGDEBUG,
                MAC_PDSCH,getCurrentTick(),
                (UInt32)subsetCount,
                (UInt32)subsetInfo_p->subsetBitMap[CC_USER],
                (UInt32)subsetInfo_p->subsetBitMap[CE_USER],
                (UInt32)subsetInfo_p->spanWidth,
                (UInt32)subsetInfo_p->numPRBFree,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"\nRAT 1 subset Information");
        
        subsetInfo_p--;
    }
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,DL_RESOURCE_MGR,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
}

/****************************************************************************
 * Function Name  : reconfigRAT2LocalizedInfo 
 * Inputs         : rat2LocalizedInfo_p - pointer to RAT2LocalizedInfo,
 *                  resourcePartitionInfo_p - pointer to DLResourcePartitionConfigInfo
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function modifies RAT 2 Localized information 
 *                  according to non-blocked PRBs
 ****************************************************************************/

void reconfigRAT2LocalizedInfo(RAT2LocalizedInfo *rat2LocalizedInfo_p,
        DLResourcePartitionConfigInfo *resourcePartitionInfo_p,
        InternalCellIndex internalCellIndex)
{
    RAT2LocalizedVRBInfo *prevLocalizedVRBInfo_p = PNULL;
    RAT2LocalizedVRBInfo *newLocalizedVRBInfo_p = PNULL;
    UInt32 prevEndVRB = 0;
    UInt32 index = 0;
    UInt32 startRB = 0;
    UInt32 endRB = 0;
    UInt32 numOfRB = 0;
   
    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_PDSCH,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    /* Here we are iterating through partition and updating
     * next free vrb and previous free vrb so that we skip blocked
     * partition.
     * In case we have two partition, first one 10-20 and second one
     * 30-45. Then
     * VRB[0]->NextFree=10+1
     * VRB[10+1]->prevFree=0
     * VRB[20+1]->nextFree=30+1
     * VRB[30+1]->prevFree=20+1;
     * VRB[45+1]->nextFree=dlRBs+1;
     */
    prevLocalizedVRBInfo_p = newLocalizedVRBInfo_p = rat2LocalizedInfo_p->
        rat2LocalizedVRBInfo;
    
    if (0 == resourcePartitionInfo_p->cellEdgeRegionCount)    
    {  
        /* There are no cell edge resources, consider only CC resources */
        for(index=0; index < resourcePartitionInfo_p->cellCenterRegionCount;
                index++)
        {
            startRB = resourcePartitionInfo_p->ccResourceInfo[index].
                startRBIndex + 1;
            endRB = resourcePartitionInfo_p->ccResourceInfo[index].
                endRBIndex + 1;
            numOfRB = endRB - startRB + 1;

            newLocalizedVRBInfo_p = 
                &rat2LocalizedInfo_p->rat2LocalizedVRBInfo[startRB];
            prevLocalizedVRBInfo_p->nextfreeVRBIndex = startRB;
            newLocalizedVRBInfo_p->previousVRBfreeIndex = prevEndVRB;
            prevEndVRB = startRB + numOfRB - 1;
            prevLocalizedVRBInfo_p = newLocalizedVRBInfo_p + numOfRB - 1;

            /* Mark all the RBs in the segment as available by setting next
             * and previous indices
             */
             startRB++;
             newLocalizedVRBInfo_p->nextfreeVRBIndex = startRB;
             newLocalizedVRBInfo_p++;

             while (startRB < endRB)
             {
                 newLocalizedVRBInfo_p->nextfreeVRBIndex = startRB + 1;
                 newLocalizedVRBInfo_p->previousVRBfreeIndex = startRB - 1;
                 startRB++;
                 newLocalizedVRBInfo_p++;
             }
             newLocalizedVRBInfo_p->previousVRBfreeIndex = startRB - 1;
        }
    }
    else if (resourcePartitionInfo_p->ccResourceInfo[0].startRBIndex <  
            resourcePartitionInfo_p->ceResourceInfo.startRBIndex) 
    {
        for(index=0; index < resourcePartitionInfo_p->cellCenterRegionCount; 
                index++)
        {
            startRB = resourcePartitionInfo_p->ccResourceInfo[index].
                startRBIndex + 1;
            endRB = resourcePartitionInfo_p->ccResourceInfo[index].
                endRBIndex + 1;
            numOfRB = endRB - startRB + 1;

            newLocalizedVRBInfo_p = 
                &rat2LocalizedInfo_p->rat2LocalizedVRBInfo[startRB];
            prevLocalizedVRBInfo_p->nextfreeVRBIndex = startRB;
            newLocalizedVRBInfo_p->previousVRBfreeIndex = prevEndVRB;
            prevEndVRB = startRB + numOfRB - 1;
            prevLocalizedVRBInfo_p = newLocalizedVRBInfo_p + numOfRB - 1;
 
            /* Mark all the RBs in the segment as available by setting next
             * and previous indices
             */
            startRB++;
            newLocalizedVRBInfo_p->nextfreeVRBIndex = startRB;
            newLocalizedVRBInfo_p++;

            while (startRB < endRB)
            {
                newLocalizedVRBInfo_p->nextfreeVRBIndex = startRB + 1;
                newLocalizedVRBInfo_p->previousVRBfreeIndex = startRB - 1;
                startRB++;
                newLocalizedVRBInfo_p++;
            }
            newLocalizedVRBInfo_p->previousVRBfreeIndex = startRB - 1;

            /* update in the order CC-CE-CC if cell edge region is sandwitched 
             * between two cell centre regions 
             */
            /* +DYNAMIC_ICIC */
            if ((1 < resourcePartitionInfo_p->cellCenterRegionCount) && 
                    (resourcePartitionInfo_p->ceResourceInfo.startRBIndex < 
                     resourcePartitionInfo_p->ccResourceInfo[index + 1].
                     startRBIndex))
            /* -DYNAMIC_ICIC */
            {
                /* To make for loop to execute only once for CC_CE_REGION */
                break;
            }
        }
        /* For cell edge resources */
        startRB = resourcePartitionInfo_p->ceResourceInfo.startRBIndex + 1;
        endRB = resourcePartitionInfo_p->ceResourceInfo.endRBIndex + 1;
        numOfRB = endRB - startRB + 1;

        newLocalizedVRBInfo_p = 
            &rat2LocalizedInfo_p->rat2LocalizedVRBInfo[startRB];
        prevLocalizedVRBInfo_p->nextfreeVRBIndex = startRB;
        newLocalizedVRBInfo_p->previousVRBfreeIndex = prevEndVRB;
        prevEndVRB = startRB + numOfRB - 1;
        prevLocalizedVRBInfo_p = newLocalizedVRBInfo_p + numOfRB - 1;

        /* Mark all the RBs in the segment as available by setting next
         * and previous indices
         */
        startRB++;
        newLocalizedVRBInfo_p->nextfreeVRBIndex = startRB;
        newLocalizedVRBInfo_p++;

        while (startRB < endRB)
        {
            newLocalizedVRBInfo_p->nextfreeVRBIndex = startRB + 1;
            newLocalizedVRBInfo_p->previousVRBfreeIndex = startRB - 1;
            startRB++;
            newLocalizedVRBInfo_p++;
        }
        newLocalizedVRBInfo_p->previousVRBfreeIndex = startRB - 1;

        /* Update RAT2 Localized info for second cell centre region */
        /* +DYNAMIC_ICIC */
        if ((1 < resourcePartitionInfo_p->cellCenterRegionCount) && 
            (resourcePartitionInfo_p->ceResourceInfo.startRBIndex < 
            resourcePartitionInfo_p->ccResourceInfo[index + 1].
            startRBIndex))
        {
            /* Increment the index to point to next CC region after CE region */
            index++;

            for(; index < resourcePartitionInfo_p->cellCenterRegionCount; 
                    index++)
            {
                startRB = resourcePartitionInfo_p->ccResourceInfo[index].
                    startRBIndex + 1;
                endRB = resourcePartitionInfo_p->ccResourceInfo[index].
                    endRBIndex + 1;
                numOfRB = endRB - startRB + 1;

                newLocalizedVRBInfo_p = 
                    &rat2LocalizedInfo_p->rat2LocalizedVRBInfo[startRB];
                prevLocalizedVRBInfo_p->nextfreeVRBIndex = startRB;
                newLocalizedVRBInfo_p->previousVRBfreeIndex = prevEndVRB;
                prevEndVRB = startRB + numOfRB - 1;
                prevLocalizedVRBInfo_p = newLocalizedVRBInfo_p + numOfRB - 1;

                /* Mark all the RBs in the segment as available by setting next
                 * and previous indices
                 */
                startRB++;
                newLocalizedVRBInfo_p->nextfreeVRBIndex = startRB;
                newLocalizedVRBInfo_p++;

                while (startRB < endRB)
                {
                    newLocalizedVRBInfo_p->nextfreeVRBIndex = startRB + 1;
                    newLocalizedVRBInfo_p->previousVRBfreeIndex = startRB - 1;
                    startRB++;
                    newLocalizedVRBInfo_p++;
                }
                newLocalizedVRBInfo_p->previousVRBfreeIndex = startRB - 1;
            }
        }
        /* -DYNAMIC_ICIC */
    }
    else
    {
        /* consider cell edge resources first */
        startRB = resourcePartitionInfo_p->ceResourceInfo.startRBIndex + 1;
        endRB = resourcePartitionInfo_p->ceResourceInfo.endRBIndex + 1;
        numOfRB = endRB - startRB + 1;

        newLocalizedVRBInfo_p = 
            &rat2LocalizedInfo_p->rat2LocalizedVRBInfo[startRB];
        prevLocalizedVRBInfo_p->nextfreeVRBIndex = startRB;
        newLocalizedVRBInfo_p->previousVRBfreeIndex = prevEndVRB;
        prevEndVRB = startRB + numOfRB - 1;
        prevLocalizedVRBInfo_p = newLocalizedVRBInfo_p + numOfRB - 1;

        /* Mark all the RBs in the segment as available by setting next
         * and previous indices
         */
        startRB++;
        newLocalizedVRBInfo_p->nextfreeVRBIndex = startRB;
        newLocalizedVRBInfo_p++;

        while (startRB < endRB)
        {
            newLocalizedVRBInfo_p->nextfreeVRBIndex = startRB + 1;
            newLocalizedVRBInfo_p->previousVRBfreeIndex = startRB - 1;
            startRB++;
            newLocalizedVRBInfo_p++;
        }
        newLocalizedVRBInfo_p->previousVRBfreeIndex = startRB - 1;

        for(index=0; index < resourcePartitionInfo_p->cellCenterRegionCount;
                index++)
        {
            startRB = resourcePartitionInfo_p->ccResourceInfo[index].
                startRBIndex + 1;
            endRB = resourcePartitionInfo_p->ccResourceInfo[index].
                endRBIndex + 1;
            numOfRB = endRB - startRB + 1;

            newLocalizedVRBInfo_p = 
                &rat2LocalizedInfo_p->rat2LocalizedVRBInfo[startRB];
            prevLocalizedVRBInfo_p->nextfreeVRBIndex = startRB;
            newLocalizedVRBInfo_p->previousVRBfreeIndex = prevEndVRB;
            prevEndVRB = startRB + numOfRB - 1;
            prevLocalizedVRBInfo_p = newLocalizedVRBInfo_p + numOfRB - 1;

            /* Mark all the RBs in the segment as available by setting next
             * and previous indices
             */
            startRB++;
            newLocalizedVRBInfo_p->nextfreeVRBIndex = startRB;
            newLocalizedVRBInfo_p++;

            while (startRB < endRB)
            {
                newLocalizedVRBInfo_p->nextfreeVRBIndex = startRB + 1;
                newLocalizedVRBInfo_p->previousVRBfreeIndex = startRB - 1;
                startRB++;
                newLocalizedVRBInfo_p++;
            }
            newLocalizedVRBInfo_p->previousVRBfreeIndex = startRB - 1;
        }
    }
    /* make nextfreeVRBIndex point to 102, in between RBs are blocked */
    prevLocalizedVRBInfo_p->nextfreeVRBIndex = 
        rat2LocalizedInfo_p->maxLocalizedVRB + 2;
    
    /* Initiaalized RAT 2 localized structure from DL resource partition 
     * Information. 
     */
    rat2LocalizedInfo_p->numOfVRBAvailable = 
        (dlRAInfo_gp[internalCellIndex] + (!dlRAInfoIndex_g[internalCellIndex]))->totalAllowedRB;
    rat2LocalizedInfo_p->numOfCellEdgeVRBAvailable = 
        (dlRAInfo_gp[internalCellIndex] + (!dlRAInfoIndex_g[internalCellIndex]))->totalCellEdgeRB;
    rat2LocalizedInfo_p->firstCellEdgeFreeIndex = 
        resourcePartitionInfo_p->ceResourceInfo.startRBIndex + 1;
    rat2LocalizedInfo_p->endCellEdgeFreeIndex = 
        resourcePartitionInfo_p->ceResourceInfo.endRBIndex + 1;

    LOG_UT(MAC_RAT2_LOCALISED_RECONFIG,LOGDEBUG,MAC_PDSCH,
            getCurrentTick(),
            (UInt32)rat2LocalizedInfo_p->numOfVRBAvailable,
            (UInt32)rat2LocalizedInfo_p->numOfCellEdgeVRBAvailable,
            (UInt32)rat2LocalizedInfo_p->firstCellEdgeFreeIndex,
            (UInt32)rat2LocalizedInfo_p->endCellEdgeFreeIndex,
            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"\n RAT2 Distributed Reconfig");

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_PDSCH,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
}

/****************************************************************************
 * Function Name  : reconfigRAT2DistributedInfo 
 * Inputs         : rat2DistributedInfo_p - pointer to RAT2DistributedInfo,
 *                  resourcePartitionInfo_p - pointer to DLResourcePartitionConfigInfo
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function modifies RAT 2 Distibuted information 
 *                  according to non-blocked PRBs.
 ****************************************************************************/

void reconfigRAT2DistributedInfo(RAT2DistributedInfo *rat2DistributedInfo_p,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
{
    RAT2DistributedVRBInfo *distributedVRBInfo_p = PNULL;
    RAT2DistributedVRBInfo *distributedVRBInfoTemp_p = PNULL;
    UInt32 maxDistributedVRB = 0;
    UInt32 tempFreeVRBIndex = 0;

    /* +- SPR 20073 fix */
 
    /* bestContinuousVRBCount is used to find maximum number of continuous 
     * VRBs in given ICIC partition scheme. bestContinuousVRBCount[0] will be 
     * incremented for every continuous VRB. If non-consecutive VRB is found,
     * this information will be stored in bestContinuousVRBCount[1]. 
     * At the end bestContinuousVRBCount[1] will contain maximum number of 
     * continuous VRBs.
     * bestContinuousVRBCount will be used for setting flag 
     * siSchedulingRATPolicy 
     */
    UInt32 bestContinuousVRBCount[2] = {0,0};
   
    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_PDSCH,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    
    maxDistributedVRB = rat2DistributedInfo_p->numOfVRBAvailable;

    /* reset the values to calculate after reconfiguration */
    rat2DistributedInfo_p->numOfVRBAvailable = 0;
    rat2DistributedInfo_p->numOfCellEdgeVRBAvailable = 0;

    distributedVRBInfo_p = &rat2DistributedInfo_p->
        rat2DistributedVRBInfo[maxDistributedVRB];
    
    tempFreeVRBIndex = maxDistributedVRB + 1;
    distributedVRBInfoTemp_p = distributedVRBInfo_p + 1;
    
    /* For each VRB, check both PRBs mapped to even and slot are available in 
     * non blocking frequency. Update next free vrb and previous free vrb so
     * that blocked partition is skipped.
     */
    while ( maxDistributedVRB )
    {   
        /* Check if both PRBs are availabe in current bandwith */
        if ((dlRAInfo_gp[internalCellIndex] + (!dlRAInfoIndex_g[internalCellIndex]))->rbToResourceTypeMapDL
                [distributedVRBInfo_p->prbMappedToEvenSlotOfVRB] &&
                (dlRAInfo_gp[internalCellIndex] + (!dlRAInfoIndex_g[internalCellIndex]))->rbToResourceTypeMapDL
                [distributedVRBInfo_p->prbMappedToOddSlotOfVRB])
        {   
            distributedVRBInfoTemp_p->previousVRBfreeIndex = maxDistributedVRB;
            distributedVRBInfoTemp_p = distributedVRBInfo_p; 
            distributedVRBInfo_p->nextfreeVRBIndex = tempFreeVRBIndex;
            tempFreeVRBIndex = maxDistributedVRB;
            bestContinuousVRBCount[0]++;
            rat2DistributedInfo_p->numOfVRBAvailable++;

            /* Check if both PRBs belong to cell edge region */
            if ((CELL_EDGE_ONLY_REGION == (dlRAInfo_gp[internalCellIndex] + (!dlRAInfoIndex_g[internalCellIndex]))->\
                    rbToResourceTypeMapDL[distributedVRBInfo_p->
                    prbMappedToEvenSlotOfVRB]) &&
                    (CELL_EDGE_ONLY_REGION == (dlRAInfo_gp[internalCellIndex] + (!dlRAInfoIndex_g[internalCellIndex]))->
                    rbToResourceTypeMapDL[distributedVRBInfo_p->
                    prbMappedToOddSlotOfVRB]))
            {
                rat2DistributedInfo_p->numOfCellEdgeVRBAvailable++;

                /* update end cell edge free index */
                if (1 == rat2DistributedInfo_p->numOfCellEdgeVRBAvailable)
                {
                    rat2DistributedInfo_p->endCellEdgeFreeIndex = 
                        maxDistributedVRB;
                }

                /* Update first cell edge free index */
                rat2DistributedInfo_p->firstCellEdgeFreeIndex = 
                    maxDistributedVRB; 
            }
        }
        else
        {
            if (bestContinuousVRBCount[0] > bestContinuousVRBCount[1])
            {
                bestContinuousVRBCount[1] = bestContinuousVRBCount[0];
            }
            bestContinuousVRBCount[0] = 0;
        }
        distributedVRBInfo_p--;
        maxDistributedVRB--;
    }
    
    if (bestContinuousVRBCount[0] > bestContinuousVRBCount[1])
    {
        bestContinuousVRBCount[1] = bestContinuousVRBCount[0];
    }
    
    distributedVRBInfo_p->nextfreeVRBIndex = tempFreeVRBIndex;
    distributedVRBInfo_p->previousVRBfreeIndex = 0;
    
    /* Check if maximum number of continuous VRBs are enough to send common 
     * channel message, else set siSchedulingRATPolicy flag as RAT_LOCALIZED
     */
    if (bestContinuousVRBCount[1] < MIN_RB_REQD_FOR_COMM_CH)
    {
        /* SPR 20073 fix start */
        macParams_g.siSchedulingRATPolicy = RAT_LOCALIZED;
        /* SPR 20073 fix end */
    }

    LOG_UT(MAC_RAT2_DISTRIBUTED_RECONFIG,LOGDEBUG,MAC_PDSCH,
            getCurrentTick(),
            (UInt32)rat2DistributedInfo_p->numOfVRBAvailable,
            (UInt32)rat2DistributedInfo_p->numOfCellEdgeVRBAvailable,
            (UInt32)bestContinuousVRBCount[1],
            (UInt32)rat2DistributedInfo_p->firstCellEdgeFreeIndex,
            (UInt32)rat2DistributedInfo_p->endCellEdgeFreeIndex,
            DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"\n RAT2 Distributed Reconfig");

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_PDSCH,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
}

/****************************************************************************
 * Function Name  : configureDLResourceAllocationInfo 
 * Inputs         : resourcePartitionInfo_p - pointer to DLResourcePartitionConfigInfo
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will configure resource allocation 
 *                  structures for cell centre & cell edge partitions.
 ****************************************************************************/

MacRetType configureDLResourceAllocationInfo(
        DLResourcePartitionConfigInfo *resourcePartitionInfo_p,
        InternalCellIndex internalCellIndex)
{
    UInt32 index = 0;
    UInt32 dlRAInfoIndexTemp = !dlRAInfoIndex_g[internalCellIndex];
    ResourceAllocationPolicyInfo *raPolicyInfoTemp_p = 
        &((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndexTemp)->raPolicyInfo);
    /* + PRS_CHANGES */
    UInt8 dlAvailableRBs = 0;
    UInt8 requiredRBs = 0;
    UInt8  *rbToResourceTypeMapDL_p = PNULL;
    UInt8 *prsReserveRBMap_p = PNULL;
    PRSResInfo *prsResInfo_p = PNULL;
    PRSResInfo *tempPrsResInfo_p  = PNULL;
    /* - PRS_CHANGES */
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
         cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt8 dlPRSRATPoliciesIndex = cellParams_p->\
                                  prsConfigInfo.dlPRSRATPoliciesIndex;

    UInt8 tempPRSIndex = cellParams_p->\
                         prsConfigInfo.dlPRSRATPoliciesIndex;;
    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,DL_RESOURCE_MGR,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndexTemp)->totalAllowedRB = 
        resourcePartitionInfo_p->totalAvailableRBDL;
    (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndexTemp)->totalCellEdgeRB = 
        resourcePartitionInfo_p->totalCellEdgeRBDL;
 
    memSet ((dlRAInfo_gp[internalCellIndex] + dlRAInfoIndexTemp)->rbToResourceTypeMapDL, 
            0, MAX_RES_BLOCKS); 

    /* rbToResourceTypeMapDL is array of resource blocks in given bandwith
     * 1. Value 0 indicates that PRB is blocked
     * 2. Value 1 indicates PRB is available in cell centre resource region 
     * 3. Value 2 indicates PRB is available in cell edge resource region 
     */
    for (index = 0;
            index < resourcePartitionInfo_p->cellCenterRegionCount;
            index++)
    {
        memSet(&(dlRAInfo_gp[internalCellIndex] + dlRAInfoIndexTemp)->rbToResourceTypeMapDL
                [resourcePartitionInfo_p->ccResourceInfo[index].startRBIndex], 
                CELL_CENTRE_ONLY_REGION,
                (resourcePartitionInfo_p->ccResourceInfo[index].endRBIndex - 
                 resourcePartitionInfo_p->ccResourceInfo[index].startRBIndex +
                 1));
    }
    
    if (resourcePartitionInfo_p->cellEdgeRegionCount)
    {
        memSet(&(dlRAInfo_gp[internalCellIndex] + dlRAInfoIndexTemp)->rbToResourceTypeMapDL
                [resourcePartitionInfo_p->ceResourceInfo.startRBIndex], 
                CELL_EDGE_ONLY_REGION,
                (resourcePartitionInfo_p->ceResourceInfo.endRBIndex - 
                 resourcePartitionInfo_p->ceResourceInfo.startRBIndex + 1));
    }

    reconfigRAT0Info(&raPolicyInfoTemp_p->rat0Info, resourcePartitionInfo_p, internalCellIndex); 
    reconfigRAT1Info(&raPolicyInfoTemp_p->rat1Info,internalCellIndex); 
    reconfigRAT2LocalizedInfo(&raPolicyInfoTemp_p->rat2LocalizedInfo,
            resourcePartitionInfo_p, internalCellIndex); 
    /* +- SPR 17777 */
    reconfigRAT2DistributedInfo(&raPolicyInfoTemp_p->rat2DistributedInfo,
            internalCellIndex); 
    /* +- SPR 17777 */

    /* update RAT policy for MIB subframe */
    memCpy(&(dlRAInfo_gp[internalCellIndex] + dlRAInfoIndexTemp)->raPolicyInfoMIBReserved,  
            &(dlRAInfo_gp[internalCellIndex] + dlRAInfoIndexTemp)->raPolicyInfo, 
            sizeof(ResourceAllocationPolicyInfo ));
    
    initializeRAPolicyMibScheduled(dlRAInfoIndexTemp,internalCellIndex);

    /* + PRS_CHANGES */
    
    /* update RAT policies for PRS subframes */


    memCpy(&((dlRAInfo_gp[internalCellIndex] + tempPRSIndex)->raPolicyInfoPRSReserved),  
            &((dlRAInfo_gp[internalCellIndex]+ dlRAInfoIndexTemp)->raPolicyInfo), 
            sizeof(ResourceAllocationPolicyInfo ));

    cellParams_p->prsConfigInfo.icicConfigured = 1;

    if (cellParams_p->prsConfigInfo.isPRSConfigured) 
    {

	prsResInfo_p =  &(cellParams_p->prsConfigInfo.prsResInfo[dlPRSRATPoliciesIndex]);
	tempPrsResInfo_p =  &(cellParams_p->prsConfigInfo.prsResInfo[tempPRSIndex]);
	/* Saving required parameters at temp index */
	tempPrsResInfo_p->rat0PRSBitmap =   prsResInfo_p->rat0PRSBitmap;  
	tempPrsResInfo_p->startPRB =  prsResInfo_p->startPRB;
	tempPrsResInfo_p->endPRB = prsResInfo_p->endPRB;
        tempPrsResInfo_p->prsAllowedWithICIC = TRUE;
	memCpy(tempPrsResInfo_p->prsReserveRBMap, prsResInfo_p->prsReserveRBMap, 
		(MAX_RES_BLOCKS * sizeof(UInt8)));
	if (tempPrsResInfo_p->startPRB != tempPrsResInfo_p->endPRB)
	{
	    dlAvailableRBs  = cellParams_p->dlAvailableRBs;  
	    rbToResourceTypeMapDL_p = (dlRAInfo_gp[internalCellIndex]+ dlRAInfoIndexTemp)->rbToResourceTypeMapDL;
	    prsReserveRBMap_p = tempPrsResInfo_p->prsReserveRBMap; 
	    CHECK_REMAINING_RES_WITH_ICIC_PRS_ON(dlAvailableRBs,requiredRBs,
		    rbToResourceTypeMapDL_p,prsReserveRBMap_p);

	    if (requiredRBs ==  MAX_RES_REQUIRED_TO_SCHEDULE_SI_PAGING)
	    { 	
		initPRSRATPoliciesBitmaps((&((dlRAInfo_gp[internalCellIndex]+ tempPRSIndex)->\
                                raPolicyInfoPRSReserved)),tempPRSIndex,
                                internalCellIndex);
	    }
	    else 
	    {
		tempPrsResInfo_p->prsAllowedWithICIC = FALSE;
	    }
	}
    }
    /* - PRS_CHANGES */

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_PDSCH,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : macInitDLResourceInfoMgr 
 * Inputs         : numOfCells 
 * Outputs        : None
 * Returns        : None
 * Description    : This function initialises DLResourceInfoMgr  at MAC_INIT_LAYER_REQ
 ****************************************************************************/
void macInitDLResourceInfoMgr(InternalCellIndex numOfCells)
{
    InternalCellIndex cellIndex = 0;
    for(cellIndex = 0; cellIndex < numOfCells; cellIndex++)
    {
      mibOverlapRBs_g[cellIndex][CC_CE_REGION] = MIB_OVERLAP_RB_COUNT;
      mibOverlapRBs_g[cellIndex][CE_REGION] = 0;

    }
	return;
}

/****************************************************************************
 * Function Name  : initDlResInfo 
 * Inputs         : InternalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function allocates the memory allocated for 
  *                 DLResourceInfoMgr at cell config
 ****************************************************************************/
void initDlResInfo (InternalCellIndex internalCellIndex)
{
    initAllResourceAllocationPolicyInfo(internalCellIndex);

	return;
}

/* +- SPR 17777 */

/****************************************************************************
 * Function Name  : deInitDlResInfo 
 * Inputs         : InternalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function Release the memory allocated for 
  *                 DLResourceInfoMgr at cell delete
 ****************************************************************************/
void deInitDlResInfo (InternalCellIndex internalCellIndex)
{
    /* Reset the index used for RA info */
    dlRAInfoIndex_g[internalCellIndex] = 0;

    /* free memory allocated to DL RA policy */ 
    if(dlRAInfo_gp[internalCellIndex])
    {
        freeMemPool(dlRAInfo_gp[internalCellIndex]);
        dlRAInfo_gp[internalCellIndex] = PNULL;
    }

    return;
}
