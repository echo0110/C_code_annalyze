/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacULResourceInfoMgr.c
 *
 ******************************************************************************
 *
 *  File Description :  This file contains the Funtions needed for RB  
 *                      Allocation as per cell center and cell edge resources in 
 *                      RAT 2 Localized .
 ******************************************************************************
*****************************************************************************/


 /******************************************************************************
  * Standard Library Includes
  *****************************************************************************/

 /******************************************************************************
  * Project Includes
  *****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacULResourceAllocator.h"
#include "lteMacULResourceInfoMgr.h"
#include "lteMacCellConfMgr.h"

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* Array of two will be used for each cell. As one of the index can be used per TTI
 * Another one can be updated from low priority thread. */
/*CA Changes start  */
ULResourceAllocationInfo* ulRAInfo_gp[MAX_NUM_CELL] = {PNULL};
/* This index specifies the updated RRM information */
UInt8  ulRAInfoIndex_g[MAX_NUM_CELL];

ULPowerMaskInfo* ulPowerMaskInfo_gp[MAX_NUM_CELL] = {PNULL};
UInt8 ulPowerMaskIndex_g[MAX_NUM_CELL];
/*CA Changes end  */

/* CA changes start */
extern RRMMACCellGlobal *rrmMacCellGlobal_gp[MAX_NUM_CELL];
/* CA changes end */


 /******************************************************************************
   Private Definitions
  *****************************************************************************/

/* ICIC changes start */
/*****************************************************************************
 * Function Name  : configureULResourceAllocationInfo
 * Inputs         : resourcePartitionInfo_p - This contains information about 
 *                                            CE/CC user resource partition indexes
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None 
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function initializes resource map which is currently 
 *                  not in use with the information provided in 
 *                  resourcePartitionInfo_p. 
 *****************************************************************************/
MacRetType configureULResourceAllocationInfo(
        ULResourcePartitionConfigInfo *resourcePartitionInfo_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
        /*CA Changes end  */
{
    SInt32 rbIdx = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                   cellParams_p->ulAvailableRBs - 1;
    SInt32 ceIdx = 0;
    SInt32 cellCenterRegionIdx = 
        resourcePartitionInfo_p->cellCenterRegionCount - 1;
    UInt8 blockedRegionCount = 1;
    /*CA Changes start  */
    UInt8 reqUlRAInfoIndex = !ulRAInfoIndex_g[internalCellIndex];
    ULResourceAllocationInfo *ulRAInfo = ulRAInfo_gp[internalCellIndex] + reqUlRAInfoIndex;
    /*CA Changes end  */

    /* CE Indexes Updation*/
    /* { freeRBCounter, freeRBIndex,endIndex } will be updated from CE
     * resource info given by RRM*/
     memSet(&ulRAInfo->rbMapInitInfo,0,
     sizeof(ulRAInfo->rbMapInitInfo));
     
     if(resourcePartitionInfo_p->totalCellEdgeRBUL > 0)
     {
         /* CE freeRBIndex,freeRBCounter and end Indexes Updation*/
         ulRAInfo->rbMapInitInfo.availableResourceInfo[CE_REGION].endIndex =
             resourcePartitionInfo_p->ceResourceInfo.endRBIndex;
         ulRAInfo->rbMapInitInfo.availableResourceInfo[CE_REGION].freeRBIndex =
             resourcePartitionInfo_p->ceResourceInfo.startRBIndex;
         ulRAInfo->rbMapInitInfo.availableResourceInfo[CE_REGION].freeRBCounter =
    /* TDD Config 0 Changes Start */
         ulRAInfo->rbMapInitInfo.availableResourceInfo[CE_REGION].initAvailRes =
    /* TDD Config 0 Changes End */
             resourcePartitionInfo_p->totalCellEdgeRBUL;
         ulRAInfo->totalCellEdgeRB = resourcePartitionInfo_p->totalCellEdgeRBUL;


         /* CC Indexes Updation*/
         /* { freeRBCounter, freeRBIndex,endIndex } will be updated from CC
          * resource info given by RRM*/
         if(resourcePartitionInfo_p->ccResourceInfo
                 [resourcePartitionInfo_p->cellCenterRegionCount - 1].endRBIndex <
                 resourcePartitionInfo_p->ceResourceInfo.endRBIndex)
         {
             ulRAInfo->rbMapInitInfo.availableResourceInfo[CC_CE_REGION].endIndex =
                 resourcePartitionInfo_p->ceResourceInfo.endRBIndex;
         }
         else
         {
             ulRAInfo->rbMapInitInfo.availableResourceInfo[CC_CE_REGION].endIndex =
                 resourcePartitionInfo_p->ccResourceInfo
                 [resourcePartitionInfo_p->cellCenterRegionCount - 1].endRBIndex;
         }
         if(resourcePartitionInfo_p->ccResourceInfo[0].startRBIndex > 
                 resourcePartitionInfo_p->ceResourceInfo.startRBIndex )
         {
             ulRAInfo->rbMapInitInfo.availableResourceInfo[CC_CE_REGION].freeRBIndex =
                 resourcePartitionInfo_p->ceResourceInfo.startRBIndex;
         }
         else
         {
             ulRAInfo->rbMapInitInfo.availableResourceInfo[CC_CE_REGION].freeRBIndex =
                 resourcePartitionInfo_p->ccResourceInfo[0].startRBIndex;
         }
     }
     else
     {
         ulRAInfo->rbMapInitInfo.availableResourceInfo[CC_CE_REGION].freeRBIndex =
             resourcePartitionInfo_p->ccResourceInfo[0].startRBIndex;
         ulRAInfo->rbMapInitInfo.availableResourceInfo[CC_CE_REGION].endIndex =
             resourcePartitionInfo_p->ccResourceInfo
             [resourcePartitionInfo_p->cellCenterRegionCount - 1].endRBIndex;
     }
     ulRAInfo->rbMapInitInfo.availableResourceInfo[CC_CE_REGION].freeRBCounter =
    /* TDD Config 0 Changes Start */
     ulRAInfo->rbMapInitInfo.availableResourceInfo[CC_CE_REGION].initAvailRes =
    /* TDD Config 0 Changes End */
         resourcePartitionInfo_p->totalAvailableRBUL;
     ulRAInfo->totalAvailableRB = resourcePartitionInfo_p->totalAvailableRBUL;


     /* Marking blocked region busy */
     while(rbIdx >= 0)
     {
        if((resourcePartitionInfo_p->totalCellEdgeRBUL > 0)
                &&(rbIdx == (SInt64)resourcePartitionInfo_p->ceResourceInfo.endRBIndex))
        {
            for(ceIdx = rbIdx; 
                    ((ceIdx >= (SInt64)resourcePartitionInfo_p->ceResourceInfo.startRBIndex)
                    && (ceIdx >=0));
                    --ceIdx)
            {
                /* Specifying that this rb belongs to cell edge region */
                ulRAInfo->rbMapRegion[ceIdx].regType = CELL_EDGE_ONLY_REGION;
            }
            rbIdx = resourcePartitionInfo_p->ceResourceInfo.startRBIndex - 1;
            blockedRegionCount = 1; /* Reset the counter for blocked region */
        }
        else if((cellCenterRegionIdx >= 0)&&
                ( rbIdx == (SInt64)resourcePartitionInfo_p->ccResourceInfo[cellCenterRegionIdx].endRBIndex))
        {
            rbIdx = resourcePartitionInfo_p->ccResourceInfo[cellCenterRegionIdx].startRBIndex - 1;
            cellCenterRegionIdx --;
            blockedRegionCount = 1; /* Reset the counter for blocked region */
        }
        else
        {
            ulRAInfo->rbMapInitInfo.rbMap[rbIdx] = blockedRegionCount ;
            /* Specifying this rb to be blocked */
            ulRAInfo->rbMapRegion[rbIdx].regType = BLOCKED_REGION;
            ulRAInfo->rbMapRegion[rbIdx].blockedCount = 
                blockedRegionCount;
            blockedRegionCount ++;
            rbIdx --;
        }
    }

    /* bitmask is set to signify that resource map is updated */
    //reconfigBitmaskFlag_g |= UL_RESOURCE_INFO_UPDATED_BITMASK ;
    LOG_UT(MAC_ULSCH_CE_RES_PARTITION_INFO,LOGDEBUG,MAC_PUSCH,\
            getCurrentTick(),
            /*CA Changes start  */
            (ulRAInfo_gp[internalCellIndex] + reqUlRAInfoIndex)->rbMapInitInfo.availableResourceInfo[CE_REGION].freeRBIndex,\
            (ulRAInfo_gp[internalCellIndex] + reqUlRAInfoIndex)->rbMapInitInfo.availableResourceInfo[CE_REGION].endIndex,
            (ulRAInfo_gp[internalCellIndex] + reqUlRAInfoIndex)->rbMapInitInfo.availableResourceInfo[CE_REGION].freeRBCounter,\
            0,0,0.0,0.0,\
            __func__,"RESOURCE_PARTITION");
    LOG_UT(MAC_ULSCH_RES_PARTITION_INFO,LOGDEBUG,MAC_PUSCH,\
            getCurrentTick(),
            (ulRAInfo_gp[internalCellIndex] + reqUlRAInfoIndex)->rbMapInitInfo.availableResourceInfo[CC_CE_REGION].freeRBIndex,\
            (ulRAInfo_gp[internalCellIndex] + reqUlRAInfoIndex)->rbMapInitInfo.availableResourceInfo[CC_CE_REGION].endIndex,\
            (ulRAInfo_gp[internalCellIndex] + reqUlRAInfoIndex)->rbMapInitInfo.availableResourceInfo[CC_CE_REGION].freeRBCounter,\
            (ulRAInfo_gp[internalCellIndex] + reqUlRAInfoIndex)->totalCellEdgeRB,\
            (ulRAInfo_gp[internalCellIndex] + reqUlRAInfoIndex)->totalAvailableRB,\
            /*CA Changes end  */
            0.0,0.0,\
            __func__,"RESOURCE_PARTITION");

    return MAC_SUCCESS;

}
/* ICIC changes end */
/* +DYNAMIC_ICIC */
/*****************************************************************************
 * Function Name  : configULPowerMaskInfo
 * Inputs         : ulPowerMask_p - This contains information about
 *                     ul powermask of resp CE/CC user and per qci power mask
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function initializes ulRAInfo with ul power mask.
 ******************************************************************************/
MacRetType configULPowerMaskInfo(
        ULPowerMask   *ulPowerMask_p, InternalCellIndex internalCellIndex)
{
    UInt32 qciIndex = 0;
    /*CA Changes start  */
    ULPowerMaskInfo *ulPowerMaskInfo_p = ulPowerMaskInfo_gp[internalCellIndex] + !(ulPowerMaskIndex_g[internalCellIndex]);
    /*CA Changes end  */
  
    /* UL Power mask of CE /CC user is updated */
    ulPowerMaskInfo_p->powerMask[CE_USER] = ulPowerMask_p->cellEdgeUserPowerMask;
    ulPowerMaskInfo_p->powerMask[CC_USER] = ulPowerMask_p->cellCenterUserPowerMask;

    /* Delta Power Mask per QCI is updated*/
    for (qciIndex = 0; qciIndex < MAX_QCI; qciIndex++)
    {
        ulPowerMaskInfo_p->qciDeltaPowerMask[qciIndex] = ulPowerMask_p->qciDeltaPowerMask[qciIndex];
    }

    rrmMacCellGlobal_gp[internalCellIndex]->ulPowerMaskValUpdated = TRUE;

    return MAC_SUCCESS;
}
/* -DYNAMIC_ICIC */

/*CA Changes start  */
/*****************************************************************************
 * Function Name  : initULResourceInfo
 * Inputs         :  - number Of Cells to be configured
 * Outputs        :
 * Returns        : NONE
 * Description    : This function is used to initialize the objects having the resource information
 *                  for UL.
 *                  This is called at MAC_INIT_LAYER_REQ
 ******************************************************************************/
void initULResourceInfo(UInt8 numOfCells)
{
    UInt8 cellCount = 0;

    /* Array of two will be used for each cell. As one of the index can be used per TTI
     ** Another one can be updated from low priority thread. */
    createMemPool(sizeof(ULResourceAllocationInfo) * 2, numOfCells);
    createMemPool(sizeof(ULPowerMaskInfo) * 2, numOfCells);
    createMemPool(sizeof(ULResourcePartitionConfigInfo), numOfCells);

    for (cellCount = 0; cellCount < numOfCells; cellCount++)
    {
        GET_MEM_FROM_POOL(ULResourceAllocationInfo, ulRAInfo_gp[cellCount], sizeof(ULResourceAllocationInfo) * 2, PNULL);
        /* SPR 11109 Fix start */
        memSet(ulRAInfo_gp[cellCount],0, sizeof(ULResourceAllocationInfo) * 2);
        /* SPR 11109 Fix end */

        GET_MEM_FROM_POOL(ULPowerMaskInfo, ulPowerMaskInfo_gp[cellCount], sizeof(ULPowerMaskInfo) * 2, PNULL);
         /* SPR 11109 Fix start */
         memSet(ulPowerMaskInfo_gp[cellCount],0,sizeof(ULPowerMaskInfo) * 2);
         /* SPR 11109 Fix end */

        GET_MEM_FROM_POOL(ULResourcePartitionConfigInfo, ulResourcePartitionConfig_gp[cellCount], sizeof(ULResourcePartitionConfigInfo), PNULL);
         /* SPR 11109 Fix start */
         memSet(ulResourcePartitionConfig_gp[cellCount],0,sizeof(ULResourcePartitionConfigInfo));
         /* SPR 11109 Fix end */



        (ulPowerMaskInfo_gp[cellCount] + 0)->powerMask[CC_USER] =  MAX_PC_MAX;
        (ulPowerMaskInfo_gp[cellCount] + 0)->powerMask[CE_USER] =  MAX_PC_MAX;

        (ulPowerMaskInfo_gp[cellCount] + 0)->qciDeltaPowerMask[0] = 0;
        (ulPowerMaskInfo_gp[cellCount] + 0)->qciDeltaPowerMask[1] = 0;
        (ulPowerMaskInfo_gp[cellCount] + 0)->qciDeltaPowerMask[2] = 0;
        (ulPowerMaskInfo_gp[cellCount] + 0)->qciDeltaPowerMask[3] = 0;
        (ulPowerMaskInfo_gp[cellCount] + 0)->qciDeltaPowerMask[4] = 0;
        (ulPowerMaskInfo_gp[cellCount] + 0)->qciDeltaPowerMask[5] = 0;
        (ulPowerMaskInfo_gp[cellCount] + 0)->qciDeltaPowerMask[6] = 0;
        (ulPowerMaskInfo_gp[cellCount] + 0)->qciDeltaPowerMask[7] = 0;
        (ulPowerMaskInfo_gp[cellCount] + 0)->qciDeltaPowerMask[8] = 0;

        /* Initialize the second map index also */
        *((ULPowerMaskInfo*)(ulPowerMaskInfo_gp[cellCount] + 1)) = *((ULPowerMaskInfo*)ulPowerMaskInfo_gp[cellCount]);
    }
    return;
}

/****************************************************************************
 * Function Name  : deInitULResourceInfo 
 * Inputs         : InternalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function Release the memory allocated for 
  *                 ULResourceInfoMgr at MAC_CLEANUP_LAYER_REQ
 ****************************************************************************/
void deInitULResourceInfo (InternalCellIndex internalCellIndex)
{
    /* free memory allocated to RA policy */ 
    if(ulRAInfo_gp[internalCellIndex])
    {
        freeMemPool(ulRAInfo_gp[internalCellIndex]);
        ulRAInfo_gp[internalCellIndex] = PNULL;
    }
    if(ulPowerMaskInfo_gp[internalCellIndex])
    {
        freeMemPool(ulPowerMaskInfo_gp[internalCellIndex]);
        ulPowerMaskInfo_gp[internalCellIndex] = PNULL;
    }
    if(ulResourcePartitionConfig_gp[internalCellIndex])
    {
        freeMemPool(ulResourcePartitionConfig_gp[internalCellIndex]);
        ulResourcePartitionConfig_gp[internalCellIndex] = PNULL;
    }
    return;
}

/****************************************************************************
 * Function Name  : initUlResInfo 
 * Inputs         : InternalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function allocates the memory allocated for 
  *                 ULResourceInfoMgr at cell config
 ****************************************************************************/
void initUlResInfo (InternalCellIndex internalCellIndex)
{
    initializeResourceMap(internalCellIndex);

    return;
}

/* +- SPR 17777 */

/****************************************************************************
 * Function Name  : deInitUlResInfo 
 * Inputs         : InternalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function Release the memory allocated for 
  *                 ULResourceInfoMgr at cell delete
 ****************************************************************************/
void deInitUlResInfo (InternalCellIndex internalCellIndex)
{
    /* free memory allocated to UL RA policy maps */ 
    if(rbMapNodeArr_gp[internalCellIndex])
    {
        freeMemPool(rbMapNodeArr_gp[internalCellIndex]);
        rbMapNodeArr_gp[internalCellIndex] = PNULL;
    }    
    return;
}

/*CA Changes end  */
