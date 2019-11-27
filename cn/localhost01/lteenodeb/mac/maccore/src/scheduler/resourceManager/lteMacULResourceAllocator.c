/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacULResourceAllocator.c,v 1.1.1.1.6.1.6.1 2010/10/25 10:20:02 gur23971 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the methods that are used for 
 *                     allocating Resources in Uplink for Ack/Nack and PUCCH.
 *                     - 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacULResourceAllocator.c,v $
 * Revision 1.1.1.1.6.1.6.1  2010/10/25 10:20:02  gur23971
 * warning removed
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.28  2009/08/24 05:49:04  gur19413
 * added UT_TESTING Flag
 *
 * Revision 1.27  2009/08/23 13:44:18  gur19413
 * removed warnings unused variables
 *
 * Revision 1.26  2009/08/20 11:57:36  gur19413
 * power 2,3,5 functionality added under UT_TESTING flag.
 *
 * Revision 1.25  2009/08/20 06:56:17  gur19413
 * added UL Stat functionality for PUCCH Report
 *
 * Revision 1.24  2009/07/31 12:17:00  gur19413
 * ut bug fixed
 *
 * Revision 1.23  2009/07/28 07:12:00  gur19413
 * removed else keyword
 *
 * Revision 1.22  2009/07/27 23:58:43  gur19413
 * ut bugs fixed
 *
 * Revision 1.21  2009/07/23 08:41:49  gur19413
 * removed warning already reserved resources.
 *
 * Revision 1.20  2009/07/17 06:25:24  gur20439
 * changes related to rrc interface modification incorporated.
 *
 * Revision 1.19  2009/07/15 09:47:56  gur19413
 * ut bug fixed
 *
 * Revision 1.18  2009/07/15 06:35:12  gur19413
 * UT bugs fixed
 *
 * Revision 1.17  2009/07/13 10:00:00  gur19413
 * remove warnings
 *
 * Revision 1.16  2009/07/13 09:42:29  gur19413
 * ut bug fixed
 *
 * Revision 1.15  2009/07/13 09:30:02  gur19413
 *
 * Revision 1.14  2009/07/13 04:50:05  gur19413
 * removed warnings
 *
* Revision 1.13  2009/07/13 04:43:18  gur19413
* added Log and UT bugs Fixed
*
* Revision 1.12  2009/07/07 13:31:01  gur19413
* reset the endIndex in reset function
*
* Revision 1.11  2009/06/24 08:09:20  gur19413
* bug fixed in UT
*
* Revision 1.10  2009/06/23 03:45:40  gur19413
* updated allocateUplinkResources func
*
* Revision 1.9  2009/06/22 09:25:02  gur19413
* remove compilation error
*
* Revision 1.8  2009/06/22 09:22:23  gur19413
* change function prototype allocateUplinkResouces
*
* Revision 1.7  2009/06/08 12:22:11  gur19413
* adding new header file
*
* Revision 1.6  2009/06/08 09:37:59  gur19413
* relative mapping regarding changes
*
* Revision 1.5  2009/06/02 07:47:02  gur18569
* removed compilation errors
*
* Revision 1.4  2009/06/01 12:26:51  gur19413
* remove compilation error
*
* Revision 1.3  2009/05/26 14:33:49  gur19413
* added file description
*
* Revision 1.2  2009/05/26 14:11:19  gur19413
* added functions body
*
* Revision 1.1  2009/04/30 12:16:34  gur19413
* initial version
*
*
*****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
/* ICIC changes start */
#include "lteMacULResourceAllocator.h"
#include "lteMacULResourceInfoMgr.h"
#include "lteMacPUSCH.h"
/* ICIC changes end */
#include "lteMacCellConfMgr.h"
#include "lteMacPRACH.h"
#include "lteMacStatsManager.h"
#include "lteMacOAMInterface.h"
#ifdef FDD_CONFIG
#include "lteMacULSpsScheduler.h"
#endif
/******************************************************************************
 * Project Includes
 *****************************************************************************/

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/* Start of change for VARIBLE Declaration */
/*CA Changes start  */
ULResourcePartitionConfigInfo* ulResourcePartitionConfig_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end  */
/* End of change for VARIBLE Declaration */


/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
  Private Function Prototypes
 ****************************************************************************/
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
extern UInt8 numRBGConfigured_g[MAX_NUM_CELL];
extern UInt8 rbgSize_g[MAX_NUM_CELL];
extern UInt32 lastRbgIdxMask_g;
extern UInt8 setBandwidth_g[MAX_NUM_CELL];
#endif
/* SPR 21958 PUSCH RAT1 Support End */
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/*CA Changes start  */
RbMapNode *rbMapNodeArr_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end  */
/* SPR 1010 changes start */
UInt8  maxpowerOfTwoThreeFive_g[MAX_NUM_RBS + 1];
UInt8  minpowerOfTwoThreeFive_g[MAX_NUM_RBS + 1];
/* SPR 1010 changes end */
/*SPR 8451 Changes Start*/
#ifdef ULCODERATE_PLUGIN
extern UInt8 TBSUlArr[MAX_MCS_INDEX];
extern UInt32 maxUlNumberOfCodedBitsMap_g[ULCODERATE_MOD_INDEX][MAX_NUM_OF_RB][ULCODERATE_MAX_MCS_INDEX];
extern McsULRBInfo mcsULRBMapForCodeRate_g;
#endif
/*SPR 8451 Changes End*/
/*****************************************************************************
 * Function Name  : isPowerOfTwoThreeOrFive 
 * Inputs         : dividend - Number to be check in power of 2,3 or 5 
 * Outputs        : None
 * Returns        : TRUE/FALSE
 * Description    : This function is used to calculate the given number is Power
 *                  of 2,3 or 5. 
 *****************************************************************************/
/* SPR 1010 changes start */
 UInt32 isPowerOfTwoThreeOrFive( UInt32 dividend )
{

  LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,"[%s] Entry. ",__func__);

  while ( 0 == dividend % 2)
  {
    dividend  /= 2;
  }

  if ( 1 != dividend )
  {
    while ( 0 == dividend % 3)
    {
      dividend  /= 3;
    }
  }
  else
  {
    return TRUE;
  }

  if ( 1 != dividend)
  {
    while ( 0 == dividend % 5)
    {
      dividend  /= 5;
    }
  }
  else
  {
    return TRUE;
  }

  if ( 1 != dividend )
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }

}
/* SPR 1010 changes end */
/*****************************************************************************
 * Function Name  : initializeResourceMap
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function initializes resource map. 
 *****************************************************************************/
/*CA Changes start  */
void initializeResourceMap(InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,"[%s] Entry. ",__func__);
    UInt32 idx = 0;
    /* SPR 1010 changes start */
    UInt32 num = 0;
    UInt32 prevValidNum = 0;
    /* SPR 1010 changes end */

    /* ICIC changes start */
    /*CA Changes start  */
    ULResourceAllocationInfo *ulRAInfo = ulRAInfo_gp[internalCellIndex];
    /*CA Changes end  */
    UInt8 ulAvailableRBs = 
             cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs;
    UInt8 ulRAInfoIndex = 0;
    /* ICIC changes end */

    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    UInt8 rbgIdx = 0;
    UInt8 totalfree = MAX_RBG_INDEX;
#endif
    /* SPR 21958 PUSCH RAT1 Support End */

    /* ulRAInfoIndex_g is initialized to zero.This is done for the first 
     * time */
    /*CA Changes start  */
    ulRAInfoIndex_g[internalCellIndex] = 0;
    ulPowerMaskIndex_g[internalCellIndex] = 0;
    ulRAInfoIndex = ulRAInfoIndex_g[internalCellIndex];

    /* The mem pool will be created only first time when cell was configured */ 
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        createMemPool(sizeof(RbMapNode) * NUM_OF_RB_MAP, 1);
    }
    GET_MEM_FROM_POOL(RbMapNode, rbMapNodeArr_gp[internalCellIndex], sizeof(RbMapNode) * NUM_OF_RB_MAP, PNULL); 
    for (idx = 0; idx < NUM_OF_RB_MAP; idx++)
    {
        memSet(rbMapNodeArr_gp[internalCellIndex] + idx, 0, sizeof(RbMapNode));
    }

    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    /* Calculation of combinatorial index at init time */
    populateCombinatorialIndexTable(internalCellIndex);
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
    
    for ( idx = 0; idx < NUM_OF_RB_MAP; idx++)
    {
        (rbMapNodeArr_gp[internalCellIndex] + idx)->availableResourceInfo[CC_CE_REGION].freeRBCounter =
            ulAvailableRBs ;
        (rbMapNodeArr_gp[internalCellIndex] + idx)->availableResourceInfo[CC_CE_REGION].endIndex =
            ulAvailableRBs -1;   
        (rbMapNodeArr_gp[internalCellIndex] + idx)->pucchFlag = FALSE;   
        (rbMapNodeArr_gp[internalCellIndex] + idx)->prachFlag = RET_FALSE; 
        /* SPR 9729 fix start */
        /* Initialize uciRachFlag at init time */
        (rbMapNodeArr_gp[internalCellIndex] + idx)->uciRachFlag = FALSE;
        /* SPR 9729 fix end */
        /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
         totalfree = MAX_RBG_INDEX;
        for(rbgIdx = numRBGConfigured_g[internalCellIndex]; rbgIdx < MAX_RBG_INDEX;rbgIdx++)
        {
            (rbMapNodeArr_gp[internalCellIndex] + idx)->rbgMapForRAT1[rbgIdx] = BARRED_RESOURCE;
            (rbMapNodeArr_gp[internalCellIndex] + idx)->rat1bitmap |= (1 << rbgIdx);
             totalfree--;
        }

        (rbMapNodeArr_gp[internalCellIndex] + idx)->rat1bitmap &= REFRESHBITMASK_27BITS;
        (rbMapNodeArr_gp[internalCellIndex] + idx)->rat1bitmap |= totalfree << SHIFT27BITS;
#endif
        /* SPR 21958 PUSCH RAT1 Support End */

    }
    /*CA Changes end  */

    /* ulRAInfo of index 0 is updated here,This will be used initially
     * and details from RRM will be updated at index 1,
     */
    /*CA Changes start  */
    ulRAInfo[ulRAInfoIndex].rbMapInitInfo = *((RbMapNode*)(rbMapNodeArr_gp[internalCellIndex] + 0));
    /*CA Changes end  */
    ulRAInfo[ulRAInfoIndex].totalCellEdgeRB = 0;
    ulRAInfo[ulRAInfoIndex].totalAvailableRB = ulAvailableRBs;
    memSet(&ulRAInfo[ulRAInfoIndex].rbMapRegion,CELL_CENTRE_ONLY_REGION,
            sizeof(ulRAInfo[ulRAInfoIndex].rbMapRegion));
    /* +DYNAMIC_ICIC */
    (ulPowerMaskInfo_gp[internalCellIndex] + 0)->powerMask[CC_USER] =  MAX_PC_MAX;
    (ulPowerMaskInfo_gp[internalCellIndex] + 0)->powerMask[CE_USER] =  MAX_PC_MAX;


    (ulPowerMaskInfo_gp[internalCellIndex] + 0)->qciDeltaPowerMask[0] = 0;
    (ulPowerMaskInfo_gp[internalCellIndex] + 0)->qciDeltaPowerMask[1] = 0;
    (ulPowerMaskInfo_gp[internalCellIndex] + 0)->qciDeltaPowerMask[2] = 0;
    (ulPowerMaskInfo_gp[internalCellIndex] + 0)->qciDeltaPowerMask[3] = 0;
    (ulPowerMaskInfo_gp[internalCellIndex] + 0)->qciDeltaPowerMask[4] = 0;
    (ulPowerMaskInfo_gp[internalCellIndex] + 0)->qciDeltaPowerMask[5] = 0;
    (ulPowerMaskInfo_gp[internalCellIndex] + 0)->qciDeltaPowerMask[6] = 0;
    (ulPowerMaskInfo_gp[internalCellIndex] + 0)->qciDeltaPowerMask[7] = 0;
    (ulPowerMaskInfo_gp[internalCellIndex] + 0)->qciDeltaPowerMask[8] = 0;

    *(ULPowerMaskInfo*)(ulPowerMaskInfo_gp[internalCellIndex] + 1) = *(ULPowerMaskInfo*)ulPowerMaskInfo_gp[internalCellIndex];
    /* -DYNAMIC_ICIC */

    ulRAInfo[!ulRAInfoIndex] = ulRAInfo[ulRAInfoIndex];

    /* SPR 1010 changes start */
    /* create a Map or global Array  of Power of 2,3,5*/
    for ( num = 1; num <= ulAvailableRBs ;num++ )
    {
#ifdef FLEXRAN
        //in integration with astri L1, we found if ul_rb=50 in 20MHz bw, or ul_rb=25 in 10MHz bw, PUSCH always wrong from UE
        //so avoid these rb configurations, set ul_rb=50 in 20MHz bw, or ul_rb=25 in 10MHz bw as invalid rb number
        if (((100 == ulAvailableRBs) && (50 == num))
            || ((50 == ulAvailableRBs) && (25 == num)))
        {
            minpowerOfTwoThreeFive_g[num] = prevValidNum;
            continue;
        }
#endif
        if ( isPowerOfTwoThreeOrFive(num))
        {
            minpowerOfTwoThreeFive_g[num] = num;
            prevValidNum = num;
        }
        else
        {
            minpowerOfTwoThreeFive_g[num] = prevValidNum;
        }
        LTE_MAC_UT_LOG(LOG_INFO,"[%s] MIN TABLE = %d %d\n",__func__,num,minpowerOfTwoThreeFive_g[num]);
    }
    for( num = ulAvailableRBs; num > 0; num-- )
    {
        if(minpowerOfTwoThreeFive_g[num] == num)
        {
            prevValidNum = num;
            maxpowerOfTwoThreeFive_g[num] = num;
        }
        else
        {
            maxpowerOfTwoThreeFive_g[num] = prevValidNum;   
        }     
        LTE_MAC_UT_LOG(LOG_INFO,"[%s] MAX TABLE = %d %d\n",__func__,num,maxpowerOfTwoThreeFive_g[num]);
    }

    /* SPR 5785 Fix start */
    maxpowerOfTwoThreeFive_g[0] = 1;
    minpowerOfTwoThreeFive_g[0] = 1;
    /* SPR 5785 Fix end */

    /* SPR 1010 changes end */
    LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,"[%s] Exit. ",__func__);
}

/*****************************************************************************
 * Function Name  : isPUCCHResourcesAllocated
 * Inputs         : NONE 
 * Outputs        :
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function check that PUCCH Resource Allocated.
 *****************************************************************************/
/*
   MacRetType isPUCCHResourcesAllocated(UInt32 rbMapNum)
   {
   LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,"[%s] RbMapNum[%d] Return [%d] ",
   __func__,rbMapNum,rbMapNodeArr_g[rbMapNum].pucchFlag);
   return rbMapNodeArr_g[rbMapNum].pucchFlag;  

   }
 */
/*****************************************************************************
 * Function Name  : setPRACHResourcesAllocated
 * Inputs         : rbMapNum, 
 *                  numPhysicalResources (in TDD only),
 *                  internalCellIndex 
 * Outputs        : None
 * Returns        : None
 * Description    : This function will set the Flag if PRACH resource is 
 *                  allocated.
 *****************************************************************************/
void setPRACHResourcesAllocated(UInt32 rbMapNum, 
#ifdef TDD_CONFIG
        UInt32 numPhysicalResources, 
#endif
        InternalCellIndex internalCellIndex)
{
    LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,"[%s] RbMapNum[%d] "
            "Return [%d] ",  __func__,rbMapNum,
            (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->prachFlag);

    if(RET_FALSE ==(rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->prachFlag )
    {
        (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->prachFlag = RET_TRUE;
#ifdef TDD_CONFIG
        /* Store the number of RBs allocated for PRACH */
        (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->allocatedPrachNumRbs =
            numPhysicalResources;
#endif
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,"[%s] PRACH FLAG "
                "ALREADY SET RbMapNum %d PRACH FLAG %d\n", __func__, rbMapNum,
                (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->prachFlag );
    }
}

/*****************************************************************************
 * Function Name  : isPRACHResourcesAllocated
 * Inputs         : rbMapNum
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : PRACH Flag which indicates allocated or not
 * Description    : This function check that PUCCH Resource Allocated.
 *****************************************************************************/
/*coverity-530 CID-32309*/
BoolRetType isPRACHResourcesAllocated( UInt32 rbMapNum,
        InternalCellIndex internalCellIndex)
/*coverity-530 CID-32309*/
{
    LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,"[%s] RbMapNum[%d] Return [%d] ",
        __func__,rbMapNum,(rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->prachFlag);
    return (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->prachFlag;  
}

/*****************************************************************************
 * Function Name  : allocateResourcesForPUCCHReports
 * Inputs         : prbIndexSlot1 - Its represent the physical resource block 
 *                                  for slot first,
 *                  prbIndexSlot2 - Its represent the physical resource block
 *                                  for slot second,
 *                  rbMapNum - RB Map Number              
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function allocate resources for PUCCH Reports.
 *****************************************************************************/
MacRetType allocateResourcesForPUCCHReports( UInt32 prbIndexSlot1,
    UInt32 prbIndexSlot2,
    UInt32 rbMapNum,
    InternalCellIndex internalCellIndex
    )
{
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*SPR 19504 fix +-*/

    rbMapNode_p->rbMap[prbIndexSlot1] = BUSY_RESOURCE;
    rbMapNode_p->rbMap[prbIndexSlot2] = BUSY_RESOURCE;
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    UInt8 rbgIdx = 0;
    UInt8 totalfree = (rbMapNode_p->rat1bitmap & BITMASK_27BITS)>>27;


    rbgIdx = floor_wrapper(prbIndexSlot1 / rbgSize_g[internalCellIndex]);
    if (rbMapNode_p->rbgMapForRAT1[rbgIdx] == FREE_RESOURCE)
    {   
        rbMapNode_p->rbgMapForRAT1[rbgIdx] = BUSY_RESOURCE;
        rbMapNode_p->rat1bitmap |= (1 << rbgIdx);
        totalfree--;
    }
    rbgIdx = floor_wrapper(prbIndexSlot2 / rbgSize_g[internalCellIndex]);
    if (rbMapNode_p->rbgMapForRAT1[rbgIdx] == FREE_RESOURCE)
    {   
        rbMapNode_p->rbgMapForRAT1[rbgIdx] = BUSY_RESOURCE;
        rbMapNode_p->rat1bitmap |= (1 << rbgIdx);
        totalfree--;
    }
    rbMapNode_p->rat1bitmap = rbMapNode_p->rat1bitmap & REFRESHBITMASK_27BITS;
    rbMapNode_p->rat1bitmap |= totalfree<<SHIFT27BITS;
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
    
    /* calling function ensure that prbIndexSlot1 & prbIndexSlot2
       must be range of uplink start and end range*/
    /* Check the greater value of  prbIndex 
       then set the value in minIndex.*/         
    if ( rbMapNode_p->minIndex <= prbIndexSlot1 )
    {    
        rbMapNode_p->minIndex = prbIndexSlot1+1;
    }
    if(rbMapNode_p->startIndex <= prbIndexSlot1)
    {
        rbMapNode_p->startIndex = prbIndexSlot1+1;
    }
    LOG_MAC_MSG(MAC_PUCCH_RES_ALLOC,LOGDEBUG,MAC_PUSCH,\
            getCurrentTick(),\
            rbMapNum,\
            prbIndexSlot1,\
            prbIndexSlot2,\
            rbMapNode_p->minIndex,\
            rbMapNode_p->startIndex,\
            0.0,0.0,\
            __func__,"PUCCH_ALLOC");

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : setFreeRbsAdvanceAndPUCCHFlag
 * Inputs         : rbMapNum - Its represent the RB Map Number. 
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : set the PUCCH Flag TRUE and available resources for CC/CE
 *                  region.
 * Returns        : None
 * Description    : This function sets the free resource before pucch resource
 *                  allocation.This function works on advance subframe.
 *****************************************************************************/
/* SPR 2446 Fix Begins*/
void setFreeRbsAdvanceAndPUCCHFlag( UInt32 rbMapNum
                                    ,InternalCellIndex internalCellIndex
                                	/* TDD Config 0 Changes Start */ 
#ifdef TDD_CONFIG                
                                    ,UInt8 ulSubframeNum
#endif
                                	/* TDD Config 0 Changes End */ 
                                    )
    /* SPR 2446 Fix Ends*/
{
    LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,"[%s] Entry. ",__func__);
    RbMapNode *rbMapNode_p;
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    UInt8 ulAvailRBs = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs;
    AvailableResInfo *availCCResInfo_p = 
        &rbMapNode_p->availableResourceInfo[CC_CE_REGION];

    LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
            "[%s] RbMapNum [%d] ",
            __func__,rbMapNum );
    /* Resources Allocated for Advance SubFrame */
    if (  !rbMapNode_p->minIndex )
        /* SPR 1688 changes end */
    {
        /* SPR 10899 Fix Start */
#ifdef FDD_CONFIG  
            /*SPR 19581 +*/
            /*CA Changes start  */
		/* SPR 21859 fix start */
#ifndef MAC_AUT_TEST
		rbMapNode_p->minIndex = rbsForPUCCH_g[internalCellIndex] + 1;
#else
		rbMapNode_p->minIndex = rbsForPUCCH_g[internalCellIndex];
#endif
		/* SPR 21859 fix end */
            /*SPR 19581 -*/
            /*CA Changes end  */
#else
        /* SPR 10899 Fix End */
        /* TDD Config 0 Changes Start */
		rbMapNode_p->minIndex = rbsForPUCCH_g[internalCellIndex][ulSubframeNum] ;
#endif
        /* TDD Config 0 Changes End */ 
    }

    /* ICIC changes start */
    /* Separate CC/CE free resouces are calculated and updated
     * freeRBCounter is deducted based on the assumption that single/double rb 
     * blocked region will not exist in RRM Configuration
     * */

    /* Resource Info is updated for advance subframe 
     * freeRBIndex is set to minIndex so that PUCCH resources can be assigned later
     * from minIndex,thereby avoiding the overwriting of PUCCH resources 
     * over PRACH resources
     * freeRBCounter is deducted by nonblocked region till minIndex
     * */
    if (availCCResInfo_p->freeRBIndex < rbMapNode_p->minIndex)
    {
        availCCResInfo_p->freeRBCounter -= 
            (rbMapNode_p->minIndex - availCCResInfo_p->freeRBIndex);
		/* SPR 21859 fix start */
#ifndef MAC_AUT_TEST
	/* SPR 16926 Start */
		availCCResInfo_p->freeRBIndex = rbMapNode_p->minIndex;
#else
		availCCResInfo_p->freeRBIndex = rbMapNode_p->minIndex + 1;
#endif
		/* SPR 21859 fix end */
	/* SPR 16926 End */
    }

    /* check whether endIndex points to the end of rbMap say 100 for 20 MHZ,
     * only then subtract it from CC resources
     * endIndex is set as below so that PUCCH resources can be assigned later
     * before this Index,thereby avoiding the overwriting of PUCCH resources
     * over PRACH resources
     * freeRBCounter is deducted by nonblocked region before minIndex from back
     *
     * */

     /*SPR 14862 changes */
    if (( UInt8 )(ulAvailRBs - rbMapNode_p->minIndex - 1) <
            availCCResInfo_p->endIndex)
     /*SPR 14862 changes */
    {
        availCCResInfo_p->freeRBCounter -= (availCCResInfo_p->endIndex -
                (ulAvailRBs - rbMapNode_p->minIndex -1));
        availCCResInfo_p->endIndex  = ulAvailRBs - rbMapNode_p->minIndex -1;
    }
    LOG_MAC_MSG(MAC_ADV_CC_AVAIL_RES_INFO,LOGDEBUG,MAC_PUSCH,\
            getCurrentTick(),\
            rbMapNode_p->minIndex,
            rbMapNode_p->startIndex,\
            availCCResInfo_p->freeRBIndex,\
            availCCResInfo_p->endIndex,\
            availCCResInfo_p->freeRBCounter,\
            0.0,0.0,\
            __func__,"ADVANCE_RES_ALLOC");
}
/*****************************************************************************
 * Function Name  : setFreeRbsAndPUCCHFlag
 * Inputs         : rbMapNum - Its represent the RB Map Number. 
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : set the PUCCH Flag TRUE and free RBCounter
 * Returns        : None
 * Description    : This function set the free resource after the allocated 
 *                  pucch resources.
 *****************************************************************************/
/* SPR 2446 Fix Begins*/
void setFreeRbsAndPUCCHFlag( UInt32 rbMapNum ,
        InternalCellIndex internalCellIndex)
    /* SPR 2446 Fix Ends*/
{
    LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,"[%s] Entry. ",__func__);
    RbMapNode *rbMapNode_p;
    /*CA Changes start  */
    rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    /* ICIC changes start */
    UInt8 ulAvailRBs = cellSpecificParams_g.cellConfigAndInitParams_p\
               [internalCellIndex]->cellParams_p->ulAvailableRBs;
    UInt8 freeRBAtStart = 0;
    UInt8 freeRBAtEnd = 0;
    UInt8 idx = 0;
    AvailableResInfo *availCCResInfo_p = 
        &rbMapNode_p->availableResourceInfo[CC_CE_REGION];
    AvailableResInfo *availCEResInfo_p = 
        &rbMapNode_p->availableResourceInfo[CE_REGION];


    /* ICIC changes end */
    {
        LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
                "[%s] RbMapNum [%d] advanceFlag[%d]. ",
                __func__,rbMapNum,rbMapNode_p->advanceAllocFlag );
            /* SPR 1688 changes start */

            /* SPR 1802 changes start */
            /* update the endIndex after allocating PUCCH Resources*/

            /* ICIC changes start */
            /* Separate CC/CE free resouces are calculated and updated 
             * freeRBCounter is deducted based on the assumption that single/double rb 
             * blocked region will not exist in RRM Configuration
             * */

            /* Resource Info is updated for current subframe */

            /* When advanceAllocFlag is false ,that means no advance allocation has been done
             * in this rbMap,therefore no squeezing is required,we only need to update the 
             * available resources.
             * freeRBIndex is set to startIndex as the PUCCH resources are reserved 
             * till startIndex
             * freeRBCounter is deducted by nonblocked region till startIndex
             * */

            if(FALSE == rbMapNode_p->advanceAllocFlag)
            {
                if (availCCResInfo_p->freeRBIndex < rbMapNode_p->startIndex)
                {
                    availCCResInfo_p->freeRBCounter -= 
                        (rbMapNode_p->startIndex - availCCResInfo_p->freeRBIndex);
                    availCCResInfo_p->freeRBIndex = rbMapNode_p->startIndex;
                }

                /* check whether endIndex points to the end of rbMap say 100 for 20 MHZ,
                 * only then subtract it from CC resources
                 * endIndex is set as below as the PUCCH resources are reserved till startIndex
                 * from the back of rbMap
                 * freeRBCounter is deducted by nonblocked region before startIndex
                 * from back
                 */
                  /*SPR 14862 changes */
                if (( UInt8 )(ulAvailRBs - rbMapNode_p->startIndex - 1) <
                        availCCResInfo_p->endIndex)
                  /*SPR 14862 changes */
                {
                    availCCResInfo_p->freeRBCounter -= (availCCResInfo_p->endIndex -
                            (ulAvailRBs - rbMapNode_p->startIndex -1));
                    availCCResInfo_p->endIndex  = ulAvailRBs - rbMapNode_p->startIndex -1;
                }
                /* Cell Edge region is updated only when cell edge region exist */
                /*CA Changes start  */
                if(ulResourcePartitionConfig_gp[internalCellIndex]->cellEdgeRegionCount > 0)
                {
                    updateCEAvailRes(availCEResInfo_p,
                            0,
                            rbMapNode_p->startIndex,
                            internalCellIndex);
                /*CA Changes end  */
                    /* Same logic for CE region is applied as that of CC region */
                  /*SPR 14862 changes */
                    if(( UInt8 )(ulAvailRBs - rbMapNode_p->startIndex - 1) <
                            availCEResInfo_p->endIndex)
                  /*SPR 14862 changes */
                    {
                        availCEResInfo_p->freeRBCounter -= (availCEResInfo_p->endIndex -
                                (ulAvailRBs - rbMapNode_p->startIndex -1));
                        availCEResInfo_p->endIndex  = ulAvailRBs - rbMapNode_p->startIndex -1;
                    }
                }
            }
            else 
                /* This part of the function is executed when advance allocation is done 
                 * in this rbMap and it is again called in the current subframe,therefore
                 * squeezing is required 
                 * */
            { 
                idx = rbMapNode_p->startIndex;
                while(idx < rbMapNode_p->minIndex)
                {
                    /* first free rb from the startIndex is set as freeRBIndex */
                    if(FREE_RESOURCE == rbMapNode_p->rbMap[idx])
                    {
                        if(0 == freeRBAtStart)
                        {
                            availCCResInfo_p->freeRBIndex = idx;
                        }
                        freeRBAtStart++;
                        idx++;
                    }
                    else
                    {
                        /* jump to next free rb using blocked count*/
                        idx += rbMapNode_p->rbMap[idx];
                    }

                }
                /* freeRBCounter is increased by free rbs available at the start 
                 * before the minIndex */
                availCCResInfo_p->freeRBCounter += freeRBAtStart;
                /* Update CE resources */
                /*CA Changes start  */
                updateCEAvailRes(availCEResInfo_p,
                        0,
                        rbMapNode_p->startIndex,
                        internalCellIndex);
                /*CA Changes end  */
                idx = ulAvailRBs - rbMapNode_p->startIndex -1;
                  /*SPR 14862 changes */
                while(idx > ((UInt8 )(ulAvailRBs - rbMapNode_p->minIndex -1)))
                  /*SPR 14862 changes */
                {
                    if(FREE_RESOURCE == rbMapNode_p->rbMap[idx])
                            
                    {
                        if(freeRBAtEnd == 0)
                        {
                            availCCResInfo_p->endIndex = idx;
                        }
                        freeRBAtEnd++;
                    }
                    idx--;
                }
                /* freeRBCounter is increased by free rbs available at the end 
                 * before the minIndex from the back*/
                availCCResInfo_p->freeRBCounter += freeRBAtEnd;
                  /*SPR 14862 changes */
                if(( UInt8 )(ulAvailRBs - rbMapNode_p->startIndex - 1) <
                        availCEResInfo_p->endIndex)
                  /*SPR 14862 changes */
                { 
                    availCEResInfo_p->freeRBCounter -= (availCEResInfo_p->endIndex -
                            (ulAvailRBs - rbMapNode_p->startIndex -1));
                    availCEResInfo_p->endIndex  = ulAvailRBs - rbMapNode_p->startIndex -1;
                }

        }                   
        LOG_MAC_MSG(MAC_CC_AVAIL_RES_INFO,LOGDEBUG,MAC_PUSCH,\
                getCurrentTick(),\
                rbMapNode_p->minIndex,
                rbMapNode_p->startIndex,\
                availCCResInfo_p->freeRBIndex,\
                availCCResInfo_p->endIndex,\
                availCCResInfo_p->freeRBCounter,\
                0.0,0.0,\
                __func__,"PUCCH_RES_ALLOC");
    }
}
/* ICIC changes end */

/*****************************************************************************
 * Function Name  : allocateResourcesUsingRBIndex
 * Inputs         : startIndex - Its represent the prb Index,
 *                  length - Number of resource blocks to be reserved,
 *                  rbMapNum - It represents the rb Map number
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function allocate the resources using RB index and
 *                  number of resource reserved into a particular resource 
 *                  block Map.
 *****************************************************************************/
MacRetType allocateResourcesUsingRBIndex( UInt32 startIndex,
    UInt32 length,
    UInt32 rbMapNum,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex
    /*CA Changes end  */
    )
{
  LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,"[%s] RbMapNum[%d] Entry. ",
      __func__, rbMapNum );
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef TDD_CONFIG
  UInt8 pos = 0;
#endif
    /* SPR 21958 PUSCH RAT1 Support Start */
  /*CA Changes start  */
  RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
  /*CA Changes end  */
  /* SPR 1996 Start*/
  /* Adjust start and num resources so that RBs are reserved from 
   * minIndex
   */
  if(startIndex < rbMapNode_p->minIndex)
  {
    if((startIndex + length <= rbMapNode_p->minIndex))
    {
      return MAC_SUCCESS;
    }
    else
    {
        /* SPR 15620 fix start */
	/*  set the Resources of PUCCH region as BUSY_RESOURCE  */
#ifdef TDD_CONFIG
            /* SPR 21958 PUSCH RAT1 Support Start */
            /* Setting PUCCH resources busy in case of FDD is now been marked in the following
             * function */
        for ( pos = startIndex; pos < rbMapNode_p->minIndex; pos++)
        {
            rbMapNode_p->rbMap[pos] = (startIndex + length - pos);
        }
        /* SPR 15620 fix end */
#else
            setUplinkResourcesAsBusy(rbMapNode_p,startIndex,rbMapNode_p->minIndex,0,0,FALSE,
                    internalCellIndex);
#endif
            /* SPR 21958 PUSCH RAT1 Support End */
      length -= (rbMapNode_p->minIndex - startIndex);
      startIndex = rbMapNode_p->minIndex;
    }
  }
  /* ICIC changes start */
  if(startIndex  > rbMapNode_p->availableResourceInfo[CC_CE_REGION].endIndex)
  {
    return MAC_SUCCESS;
  }
  /* +- SPR 15620 */
  /* ICIC changes end */
  /* SPR 1996 End*/

  LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,
      "[%s] RbMapNum[%d] StartIndex[%d] Length [%d]",
      __func__, rbMapNum, startIndex, length);

  /* SPR 1996 Start*/
  /* If start value is >= length, then simply return SUCCESS
   * (resources would have already been reserved
   */        
  if(rbMapNode_p->rbMap[startIndex] >= length)
  {
    /* Resources are already reserved. */
    return MAC_SUCCESS;
  }      
  /* SPR 1996 End*/
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef TDD_CONFIG
    /* Setting PUCCH resources busy for FDD in done in setUplinkResourcesAsBusy
     * function */
  for ( pos = startIndex; pos < startIndex+length; pos++)
  {
    rbMapNode_p->rbMap[pos] = (startIndex + length - pos);
  }

#else
  setUplinkResourcesAsBusy(rbMapNode_p,startIndex,(startIndex+length),0,0,FALSE,
          internalCellIndex);
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
  /* SPR 15620 fix start */
  /* Calculate the length of resources allocated beyond PUCCH region */
  if(startIndex + length > rbMapNode_p->availableResourceInfo[CC_CE_REGION].endIndex)
  {
      length = rbMapNode_p->availableResourceInfo[CC_CE_REGION].endIndex - startIndex + 1;

  }
  /* SPR 15620 fix end */

  /* ICIC changes start */
  /* updated the freeRBCounter */
  rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter -= length;
  /* Update the FreeRBIndex */
  if ( rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBIndex 
      == startIndex )
  {
    rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBIndex 
      += length;
  }
  LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,
      "[%s] RbMapNum[%d] FreeRBCounter[%d] Exit",
      __func__,rbMapNum,
      rbMapNode_p->availableResourceInfo[CC_CE_REGION].
      freeRBCounter);
  LOG_MAC_MSG(MAC_PRACH_RES_ALLOC,LOGDEBUG,MAC_PUSCH,\
          getCurrentTick(),\
          startIndex,\
          length,\
          rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBIndex,\
          rbMapNode_p->availableResourceInfo[CC_CE_REGION].endIndex,\
          rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter,\
          0.0,0.0,\
          __func__,"RESOURCE_ALLOCATION");
  /* ICIC changes end */
  return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : getFreePUSCHResources
 * Inputs         : rbMapNum - It represents the Resource Map Number.
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : Number of Free Resources for PUSCH.
 * Description    : This function return the free Number of RBs.
 *****************************************************************************/
UInt32 getFreePUSCHResources(UInt32 rbMapNum, InternalCellIndex internalCellIndex)
{  
  LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
      "[%s] RbMapNum[%d] Entry. ",
      __func__,rbMapNum);

  RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;

  LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,
      "[%s] RbMapNum[%d] FreeRBCounter[%d] Exit. ",
      __func__, rbMapNum, rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter);
  return rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter;

}

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : checkResConflictForSpsRachRbMaps
 * Inputs         : startIndex -Start index of RbMap,
 *                  tickToReserve - Tick at which DCI will be prepared for uplink
 *                                  bundle,
 *                  ulDelay - Uplink Delay
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : Max value at index startIndex from SPS and next three RB Maps
 *                  and  PRACH resources
 * Description    : This function checks resources for tick next
                    3 ticks of bundle considering more priortised and reserved 
                    resources of PRACH,SPS and TTI Bundling retx.
 *****************************************************************************/
UInt8 checkResConflictForSpsRachRbMaps( UInt8 startIndex,
        /* SPR 15909 fix start */
        tickType_t tickToReserve,
        tickType_t ulDelay,
        /* SPR 15909 fix end */ 
        InternalCellIndex internalCellIndex)
{
    UInt8               spsCurrentTick          = 0;
    UInt8               maxValAtSpsCurrentRbMap = 0;
    UInt8               bundleCurrentTick       = 0;
    UInt8               subFrameNum             = 0;
    UInt8               idx                     = 0;
    UInt16              sysFrameNum             = 0;
    UInt32              prachFrequencyOffset    = 0;
    UInt32              numPhysicalResources    = 0; 
    PRACHConfiguration *prachConfig_p                 = PNULL;
    /* SPR 15909 fix start */
    tickType_t              reserveResForTick       = 0;
    /* SPR 15909 fix end */

    /* Start_11285_Fix Start */
    InitParams *initParams_p = cellSpecificParams_g.
                               cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    /*SPR_11285_Fix End */
    prachConfig_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
            rachConfigInfo.prachConfiguration[cellSpecificParams_g.\
            cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
            rachConfigInfo.prachActiveIndex]);
    /*CA Changes end  */
    idx = startIndex;
    spsCurrentTick = (tickToReserve + ulDelay)% MAX_SPS_INTERVAL;              
    for( bundleCurrentTick = 1; bundleCurrentTick < TTI_BUNDLE_SIZE; bundleCurrentTick++)
    {
        reserveResForTick = tickToReserve + ulDelay + FDD_HARQ_OFFSET + bundleCurrentTick;
        subFrameNum = reserveResForTick % MAX_NUM_OF_SUBFRAME;
        sysFrameNum = MODULO_ONEZEROTWOFOUR( reserveResForTick / MAX_NUM_OF_SUBFRAME);
        UInt32 rbMapNum = MODULO_SIXTEEN( reserveResForTick );

        if(prachConfig_p->prachFrequencyOffset <= idx &&
                (prachConfig_p->prachFrequencyOffset + NUM_PRACH_PHYSICAL_RESOURCES)>=idx)
        {
            /* Check for first SF only */
            if( isValidFrameForPRACH( subFrameNum, sysFrameNum,
                        &prachFrequencyOffset, &numPhysicalResources,
                        internalCellIndex))
            {
                if( maxValAtSpsCurrentRbMap < 
                    (( prachFrequencyOffset + NUM_PRACH_PHYSICAL_RESOURCES ) -
                       idx) )
                {
                    maxValAtSpsCurrentRbMap = 
                        ( prachFrequencyOffset + NUM_PRACH_PHYSICAL_RESOURCES )
                        - idx;
                }
            }
            /* Check for other SFs in case of Preamble format other that 0 */
            else if ( RET_TRUE == isPRACHResourcesAllocated( rbMapNum, 
                        internalCellIndex ) )
            {
                prachFrequencyOffset = prachConfig_p->prachFrequencyOffset;
                if( maxValAtSpsCurrentRbMap <
                        (( prachFrequencyOffset + NUM_PRACH_PHYSICAL_RESOURCES )
                         - idx))
                {
                    maxValAtSpsCurrentRbMap =
                        ( prachFrequencyOffset + NUM_PRACH_PHYSICAL_RESOURCES )
                        - idx;
                }
            }
        }
        /*SPR_11285_Fix Start */
        else if( ( TRUE == initParams_p->spsSysWideParam.enableSps ) && 
                 ( maxValAtSpsCurrentRbMap < 
                   ( spsULStrTTIMap_gp[internalCellIndex] + 
                       ((spsCurrentTick + bundleCurrentTick) % 
                       MAX_SPS_INTERVAL))->rbMapNode.rbMap[idx] ) )
            /*SPR_11285_Fix End */
        {
            maxValAtSpsCurrentRbMap = 
                (spsULStrTTIMap_gp[internalCellIndex] + 
                ((spsCurrentTick + bundleCurrentTick) % MAX_SPS_INTERVAL))->
                rbMapNode.rbMap[idx];
        }

        if( maxValAtSpsCurrentRbMap < ( rbMapNodeArr_gp[internalCellIndex] +
                    MODULO_SIXTEEN(reserveResForTick))->rbMap[idx] )
        {
            LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
                    "\n[%s] [%d] Nest 3 RbMap maxValAtSpsCurrentRbMap %d reserveResForTick %d \n",
                    __func__, getCurrentTick(),maxValAtSpsCurrentRbMap,reserveResForTick);
            maxValAtSpsCurrentRbMap = 
                (rbMapNodeArr_gp[internalCellIndex] + 
                 MODULO_SIXTEEN(reserveResForTick))->rbMap[idx];
        }
    }
    return maxValAtSpsCurrentRbMap;
}

/*****************************************************************************
 * Function Name  : allocateUplinkResourcesForTTIBundle
 * Inputs         : numOfResourcesReq - Number of resources required,
 *                  minReqdRBs - minimum required RBs,
 *                  allocatedResInfo_p - pointer to AllocatedResInfo,
 *                  strictAllocatedFlag,
 *                  availRes_p - pointer to AvailableResInfo,
 *                  rbMap,
 *                  ulDelay,
 *                  tickToReserve Tick at which DCI will be prepared for uplink
 *                                bundle,
 *                  ulUEContext_p - pointer to ULUEContext
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function allocates the resources for PUSCH for tick 
                    1 of bundle and checks for resources availability for next 
                    3 ticks of bundle considering more priortised and reserved 
                    resources of PRACH,SPS and TTI Bundling retx .
 *****************************************************************************/
MacRetType allocateUplinkResourcesForTTIBundle( UInt32 numOfResourcesReq,
    UInt32 minReqdRBs,
    AllocatedResInfo *allocatedResInfo_p,
    UInt32 strictAllocatedFlag,
    AvailableResInfo *availRes_p,
    UInt8 *rbMap,
    UInt8 ulDelay,
    /* SPR 15909 fix start */
    tickType_t tickToReserve,
    /* SPR 15909 fix end */
    ULUEContext* ulUEContext_p,
    InternalCellIndex internalCellIndex
    )
{
    UInt8               maxValAtSpsCurrentRbMap = 0;
    UInt8               bundleCurrentTick       = 0;
    UInt8               rbMapNum                = 0;
    /* SPR 15909 fix start */
    tickType_t	        currentTick		        = 0;
    /* SPR 15909 fix end */
    UInt32              localLength             = 0;
    UInt32              localIndex              = 0;
    UInt8               idx                     = 0;
    UInt32              allocatedFlag           = FALSE;
    UInt32              startIndex              = 0;
    UInt32              newFlag                 = TRUE;
    UInt32              length                  = 0;
    UInt32              lastIdx                 = 0;
    UInt32              startPoint              = 0;
    UInt32              tempLength              = 0;
    UInt32              tempStartPoint          = 0;
    AllocatedResInfo    *resInfo_p              =  PNULL;
    AvailableResInfo*   availResInfo_p          = PNULL;

    resInfo_p = allocatedResInfo_p;

    startIndex  = availRes_p->freeRBIndex;
    idx = startIndex;
    /* SPR_22579 Coverity Changes + */
    if (PNULL == ulUEContext_p)
    {
        return MAC_FAILURE;
    }
    /* SPR_22579 Coverity Changes - */
    while ( idx <= availRes_p->endIndex )
    {
        maxValAtSpsCurrentRbMap = checkResConflictForSpsRachRbMaps(idx,tickToReserve,ulDelay,
                                  internalCellIndex);
        if(rbMap[idx] > maxValAtSpsCurrentRbMap )
        {
            maxValAtSpsCurrentRbMap = rbMap[idx];
        }


        if (FREE_RESOURCE == maxValAtSpsCurrentRbMap)
        {
            if ( newFlag )
            {
                newFlag = FALSE;
                tempLength = 1;
                tempStartPoint = idx;
            }
            else
            {   
                tempLength++;     
            }
            /* Check for Required Number of Resoures got*/
            if ( tempLength ==  numOfResourcesReq )
            {
                allocatedFlag = TRUE;
                LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
                        "[%s] RbMapNum[%d] startPoint [%d] end Point[%d]. ",
                        __func__,rbMapNum,tempStartPoint,tempStartPoint+tempLength-1);
                break;
            }
            idx++;
        }
        else
        {
            idx +=  maxValAtSpsCurrentRbMap;
            if( tempLength > length)
            {
                startPoint = tempStartPoint;
                length     = tempLength;
            }
            newFlag    = TRUE;
        }
    }

    if ( allocatedFlag )
    {
        localIndex  = tempStartPoint;
        localLength = tempLength;
    }
    else
    {
        if ( RB_DEFINETELY_REQD == strictAllocatedFlag )
        {
            return MAC_FAILURE;
        }
        else
        {
            if( tempLength > length)
            {
                startPoint = tempStartPoint;
                length     = tempLength;
            }
            if ( length < minReqdRBs )
            {
                return MAC_FAILURE;
            }
            localIndex  = startPoint;
            localLength = length;
        }
    }
    localLength = minpowerOfTwoThreeFive_g[localLength];
    lastIdx = localIndex+localLength; 

    rbMapNum = MODULO_SIXTEEN(tickToReserve + ulDelay + FDD_HARQ_OFFSET);              
    for( bundleCurrentTick = 0; bundleCurrentTick < 4; bundleCurrentTick++)
    {
        /*CA Changes start  */
        RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + MODULO_SIXTEEN(rbMapNum + bundleCurrentTick);
        /*CA Changes end  */
        availResInfo_p = &rbMapNode_p->availableResourceInfo[ulUEContext_p->userLocationType];
        LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,"[%s] [%d] rbMapNum %d bundleCurrentTick %d \n",
                __func__,getCurrentTick(),rbMapNum,bundleCurrentTick);

        for ( idx = localIndex ; idx < lastIdx; idx++)
        {
	    /* Tick for which resources are to be reserved*/
	    currentTick = tickToReserve + ulDelay + FDD_HARQ_OFFSET + bundleCurrentTick; 
	    if( MAC_SUCCESS == isTTIInMeasGapPeriod
			         (ulUEContext_p, currentTick)
				)
        {
            /* No need to allocate resources if tick falls in Meas gap period */
            LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
                    "\n[%s] [%d] No  Allocation UE in MEas Gap\n",
                    __func__,getCurrentTick());

            continue;
        }
            rbMapNode_p->rbMap[idx] = lastIdx - idx;

            LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,"[%s] [%d] rbMapNode_p->rbMap[%d] = %d\n",
                    __func__,getCurrentTick(),idx,rbMapNode_p->rbMap[idx]);

        }
        availResInfo_p->freeRBCounter -= localLength;
        if ( availResInfo_p->freeRBIndex == localIndex )
        {
            availResInfo_p->freeRBIndex += localLength;
        }
    }

    resInfo_p->allocatedLength = localLength;
    resInfo_p->allocatedIndex  = localIndex ;
    resInfo_p->allocatedRiv    = getRIV(localIndex,localLength,internalCellIndex);

    LOG_MAC_MSG(MAC_RES_ALLOC_INFO,LOGDEBUG,MAC_PUSCH,\
            getCurrentTick(),\
            numOfResourcesReq,\
            minReqdRBs,\
            availRes_p->freeRBIndex,\
            availRes_p->endIndex,\
            availRes_p->freeRBCounter,\
            0.0,0.0,\
            __func__,"RESOURCE_ALLOCATION");
    LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
            "[%s] RbMapNum [%d] StartIndex[%d] Length [%d] FreeRBCounter[%d] Exit.",
            __func__, rbMapNum, localIndex, localLength, availRes_p->freeRBCounter);

    return MAC_SUCCESS;
}
#endif
/* TTIB_Code End */

/*****************************************************************************
 * Function Name  : allocateUplinkResources
 * inputs         : numOfResourcesReq - Number of resources required,
 *                  minReqdRBs - minimum required RBs,
 *                  allocatedResInfo_p - pointer to AllocatedResInfo,
 *                  strictAllocatedFlag,
 *                  availRes_p - pointer to AvailableResInfo,
 *                  rbMap
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function allocates the resources for PUSCH.
 *****************************************************************************/

MacRetType allocateUplinkResources( UInt32 numOfResourcesReq,
    UInt32 minReqdRBs,
    /* ICIC changes start */
    AllocatedResInfo *allocatedResInfo_p,
    UInt32 strictAllocatedFlag,
    AvailableResInfo *availRes_p,
    /* SPR 21958 PUSCH RAT1 Support Start */
    RbMapNode *rbMapNode_p,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex
    /* SPR 21958 PUSCH RAT1 Support End */
    /*CA Changes end  */
    /* ICIC changes end */
    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    ,UInt8 *nDmrs_p
    ,UInt8 phichSubFrameNum
    ,UInt8 iPhich
#endif
    /*TDD Config 0 Changes End*/
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    ,UInt8 isRAT1Supported
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
    )
{

  LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,
      "[%s] RbMapNum[%d] NumOfResReq[%d] minNumOfRes[%d] "
            " strictAllocatedFlag[%d]Entry. ", __func__, *(rbMapNode_p->rbMap), 
      numOfResourcesReq, minReqdRBs, strictAllocatedFlag);

  UInt32  localLength      = 0;
  UInt32  localIndex       = 0;
  UInt32  idx              = 0;
  UInt32  allocatedFlag    = FALSE;
  UInt32  startIndex       = 0;
  UInt32  newFlag         = TRUE;
  UInt32  length          = 0;
  UInt32  startPoint      = 0;
  UInt32  tempLength      = 0;
  UInt32  tempStartPoint  = 0;
  AllocatedResInfo *resInfo_p =  PNULL;
  resInfo_p = allocatedResInfo_p;
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    MacRetType retVal = MAC_FAILURE; 
    UInt8 resAllocType = DEFAULT_RESOURCE_ALLOC_TYPE;
    UInt8 totalfree = 0;
    UInt32 lastRbgIdxBandWidthwise = 0;
    CellConfigParams *cellParams_p =  cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;   
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
  /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
  UInt8 tempAvailableN2dmrsIndex     = MAX_DMRS_VALUE;
    UInt32  lastIdx         = 0;
#endif
  /*TDD Config 0 Changes End*/ 

  /* ICIC changes start end */
  /* Its point to the freeRBIndex */
  startIndex  = availRes_p->freeRBIndex;

  LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
      "[%s] RbMapNum[%d] StartIndex [%d] endIndex [%d]. ",
            __func__, *(rbMapNode_p->rbMap), startIndex, availRes_p->endIndex);

  idx = startIndex; 
          /* ICIC changes start */
  while ( idx <= availRes_p->endIndex )
  {
        if ( FREE_RESOURCE == rbMapNode_p->rbMap[idx])
    {
      if ( newFlag )
      {
        /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
        * nDmrs_p = MAX_DMRS_VALUE;
        /* Check if Phich resources are available at the given ristart*/
        if (MAC_SUCCESS != checkAvailabilityOfPhichGrpAndPhichSeq(idx,
                    nDmrs_p,
                    phichSubFrameNum,
                    iPhich,
                    internalCellIndex))
        {
            /*Ndmrs is not available at this RBIndex;
             * move to next free location; */
            idx++;
            continue;
        }
#endif
        /*TDD Config 0 Changes End*/ 
        newFlag = FALSE;
        tempLength = 1;
        tempStartPoint = idx;
      }
      else
      {   
        tempLength++;     
      }
      /* Check for Required Number of Resoures got*/
      if ( tempLength ==  numOfResourcesReq )
      {
        allocatedFlag = TRUE;
        LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
            "[%s] RbMapNum[%d] startPoint [%d] end Point[%d]. ",
                        __func__,*(rbMapNode_p->rbMap),tempStartPoint,tempStartPoint+tempLength-1);
        break;
      }
      idx++;
    }
    else
    {
            idx +=  rbMapNode_p->rbMap[idx];
      if( tempLength > length)
      {
        startPoint = tempStartPoint;
        length     = tempLength;
        /*TDD Config 0 Changes Start*/
        /*Update the temp Ndmrs value */
#ifdef TDD_CONFIG
        tempAvailableN2dmrsIndex = *(nDmrs_p);
#endif
        /*TDD Config 0 Changes End*/
      }
      newFlag    = TRUE;
    }
        /* SPR 22258 Fix Start */
        if (idx > availRes_p->endIndex)
        {
            if( tempLength > length)
            {
                startPoint = tempStartPoint;
                length     = tempLength;
                /*TDD Config 0 Changes Start*/
                /*Update the temp Ndmrs value */
#ifdef TDD_CONFIG
                tempAvailableN2dmrsIndex = *(nDmrs_p);
#endif
                /*TDD Config 0 Changes End*/
            }
        }    
        /* SPR 22258 Fix End */
  }

  if ( allocatedFlag )
  {
      localIndex  = tempStartPoint;
      localLength = tempLength;
      /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
      /*Update the temp Ndmrs value */
      tempAvailableN2dmrsIndex = *(nDmrs_p);
#endif
      /*TDD Config 0 Changes End*/
  }
  else
  {
      /*TDD Config 0 Changes End*/  
        /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
        if(isRAT1Supported)
        {
            totalfree = (((rbMapNode_p->rat1bitmap & BITMASK_27BITS) >> SHIFT27BITS) * rbgSize_g[internalCellIndex]); 
            /* This logic has been incorporated to correct the number of total RBs
             * calculated when there is incomplete RBG present in bandwidth
             * other than 20 MHz and 1.4 MHz */
            lastRbgIdxBandWidthwise = (rbMapNode_p->rat1bitmap) ^ lastRbgIdxMask_g;

            /* SPR 22579 Coverity Changes + */
            /* SPR 22263 Fix Start */
            if(((DL_BW_20_MHZ != cellParams_p->dlBandwidth) &&
                        /* SPR 22298 Fix Start */
                        (DL_BW_1DOT4_MHZ != cellParams_p->dlBandwidth)) && (lastRbgIdxBandWidthwise))
                /* SPR 22579 Coverity Changes - */
                /* SPR 22298 Fix End */
                /* SPR 22263 Fix Start */
            {
                /* reduce total RB count by 1 in case bandwidth other than 20
                 * and 1.4 MHz */
                totalfree--;
            }
            if(totalfree > length)
            {
                retVal = allocateResourceForUEThroughRAT1(numOfResourcesReq,minReqdRBs,
                        allocatedResInfo_p, strictAllocatedFlag,
                        availRes_p,rbMapNode_p,internalCellIndex);
                resAllocType = retVal;
                localLength = allocatedResInfo_p->totalRBsAllocatedForPUSCHRAT1;
            } 
        }
        if(MAC_FAILURE == retVal)
        {
#endif
            /* SPR 21958 PUSCH RAT1 Support End */
      if ( RB_DEFINETELY_REQD == strictAllocatedFlag )
      {
          return MAC_FAILURE;
      }
      else
      {
	      /* SPR 22258 Fix Start */
	      /* Code Removed */
	      /* SPR 22258 Fix End */
          /*+- 16793 */
          localIndex  = startPoint;
          localLength = length;
      }
            /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
  }
#endif
  }
    /* SPR 21958 PUSCH RAT1 Support End */ 
  /* SPR 16793 Fix Start */
  if ( !localLength )
  {
      return MAC_FAILURE;
  }
  /* SPR 16793 Fix End */
  /* + SPR 5576 */
  /* SPR 1010 changes start */
  /* if(numOfResourcesReq  >= localLength)
     {
              localLength = minpowerOfTwoThreeFive_g[localLength];
      } */
  /* SPR 1010 changes end */
   /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
  /*Populate nDmrs_p with the nDmrs value allocated*/
  if(tempAvailableN2dmrsIndex != MAX_DMRS_VALUE)
  {
      *(nDmrs_p) = tempAvailableN2dmrsIndex;
  }
  else
  {
      return MAC_FAILURE;
  }
#endif
   /*TDD Config 0 Changes End*/
   localLength = minpowerOfTwoThreeFive_g[localLength];
  /* - SPR 5576 */

#ifdef TDD_CONFIG
  /*mark the allocated index as occupied in current tti*/
  lastIdx = localIndex+localLength; 

  for ( idx = localIndex ; idx < lastIdx; idx++)
  {
        rbMapNode_p->rbMap[idx] = lastIdx - idx;
  }
#endif

          /* ICIC changes start */
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    if(RESOURCE_ALLOC_TYPE_ONE == resAllocType) /*RAT 1 Allocation */
    {
        setUplinkResourcesAsBusy(rbMapNode_p,resInfo_p->startRbgIndexforSet1,
                resInfo_p->endRbgIndexforSet1,
                resInfo_p->startRbgIndexforSet2,
                resInfo_p->endRbgIndexforSet2,
                TRUE,internalCellIndex);
        resInfo_p->RAT1Allocation = TRUE;
    }
    else/*RAT 0 Allocation */
    {
        setUplinkResourcesAsBusy(rbMapNode_p,localIndex,(localIndex+localLength),
                0,0,FALSE,internalCellIndex);
#endif
  resInfo_p->allocatedLength = localLength;
  resInfo_p->allocatedIndex  = localIndex ;
  resInfo_p->allocatedRiv    = getRIV(localIndex,localLength,internalCellIndex);
#ifdef FDD_CONFIG
  resInfo_p->RAT1Allocation = FALSE;
#endif

  /* updated freeRBCounter */
  availRes_p->freeRBCounter -= localLength;
  /* Update the FreeRBIndex */
  if ( availRes_p->freeRBIndex == localIndex )
  {
      availRes_p->freeRBIndex += localLength;
  }
#ifdef FDD_CONFIG
    }
#endif
    /* SPR 21958 PUSCH RAT1 Support End */ 
  LOG_MAC_MSG(MAC_RES_ALLOC_INFO,LOGDEBUG,MAC_PUSCH,\
          getCurrentTick(),\
          numOfResourcesReq,\
          minReqdRBs,\
          availRes_p->freeRBIndex,\
          availRes_p->endIndex,\
          availRes_p->freeRBCounter,\
          0.0,0.0,\
          __func__,"RESOURCE_ALLOCATION");
  LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
      "[%s] RbMapNum [%d] StartIndex[%d] Length [%d] FreeRBCounter[%d] Exit.",
            __func__, *(rbMapNode_p->rbMap), localIndex, localLength, availRes_p->freeRBCounter);

  return MAC_SUCCESS;
}

/*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : allocateUplinkResourcesForSecondPuschExecutionTddConfig0
 * inputs         : startIndex
 *                  rbLength
 *                  rbMap
 *                  allocatedResInfo_p : pointer to AllocatedResInfo
 *                  availRes_p : pointer to AvailableResInfo
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function allocates the same resources to the
 *                  second execution of PUSCH as given to the first.
 *                  Returns MAC_FAILURE, if not successful.
 *****************************************************************************/
MacRetType allocateUplinkResourcesForSecondPuschExecutionTddConfig0 (
	UInt8 startIndex,
	UInt8 rbLength,
	AllocatedResInfo *allocatedResInfo_p,
	AvailableResInfo *availRes_p,
	UInt8 *rbMap ,
    InternalCellIndex internalCellIndex
    )
{
    UInt32 index       = 0;
    UInt32 tempLength  = 0;
    UInt32 localLength = 0;
    UInt32 localIndex  = 0;
    UInt32 lastIndex   = 0;
    UInt32 allocatedFlag = FALSE;
    AllocatedResInfo *resInfo_p =  PNULL;

    resInfo_p = allocatedResInfo_p;

     /* This condition checks if for second execution of PUSCH,
      * if at position startIndex in rbMap, the free resources are present or not.
      * If this position is not free we will never be able to allocate
      * same resources for second execution, hence we return failure and drop the U.
      */
    if( FREE_RESOURCE != rbMap[startIndex] )
    {
	return MAC_FAILURE;
    }
    else
    {
	/* If the start index is free and available, we check for the length
	 * if the rbLength number of RB's are available from this start index,
	 * We allocate otherwise, we return MAC_FAILURE
	 */
	for( index = startIndex ; index < (rbLength+startIndex) ; index++)
	{
	    if ( FREE_RESOURCE == rbMap[index])
	    {
		allocatedFlag = TRUE;
		tempLength++;
	    }
	    else
	    {
		allocatedFlag = FALSE;
		break;
	    }
	}
	if (FALSE == allocatedFlag)
	{
	    return MAC_FAILURE;
	}
	else
	{
	    localLength = tempLength;
	    localIndex = startIndex;
	    /* Update the last index */
	    lastIndex = localIndex + localLength;
	    /* Update the rbMap */
	    for(index = localIndex ; index < lastIndex ; index++)
	    {
		rbMap[index] = lastIndex - index;
	    }
	    resInfo_p->allocatedLength = localLength;
	    resInfo_p->allocatedIndex  = localIndex;
	    resInfo_p->allocatedRiv    = getRIV(localIndex,localLength,internalCellIndex);

	    /* updated freeRBCounter */
	    availRes_p->freeRBCounter -= localLength;

	    /* Update the FreeRBIndex */
	    if ( availRes_p->freeRBIndex == localIndex )
	    {
		availRes_p->freeRBIndex += localLength;
	    }
	    return MAC_SUCCESS;
	}
    }
}
#endif
/*TDD Config 0 Changes End*/

/* ULA_CHG */
/*****************************************************************************
 * Function Name  : getUplinkMinRBsInPowerOfTwoThreeFive
 * Inputs         : rbNum - RB Map Number
 * Outputs        : None
 * Returns        : RBs in power of 2x3y5z (Min value or lower value)
 * Description    : This function returns RBs in power of 2x3y5z (Min value or 
                     lower value)
******************************************************************************/
UInt8 getUplinkMinRBsInPowerOfTwoThreeFive (UInt8 rbNum) 
{
  return minpowerOfTwoThreeFive_g[rbNum];
}

/*****************************************************************************
 * Function Name  : getUplinkMaxRBsInPowerOfTwoThreeFive
 * Inputs         : rbNum - RB Map Number
 * Outputs        : None
 * Returns        : RBs in power of 2x3y5z (Max value or higher value)
 * Description    : This function returns RBs in power of 2x3y5z (Max value or
                    higher value) 
 *****************************************************************************/
UInt8 getUplinkMaxRBsInPowerOfTwoThreeFive (UInt8 rbNum) 
{
  return maxpowerOfTwoThreeFive_g[rbNum];
}

/*****************************************************************************
 * Function Name  : checkAvailableRBsToReturnLongestSet
 * Inputs         : mcsToPrbInfo_p - pointer to MCSToPrbMapInfo,
 *                  maxReqRBs - maximum required RBs
 *                  availableRBs,
 *                  rbMapNode_p - pointer to AvailableResInfo,
 *                  rbMap
 * Outputs        : available Rbs (max contiguous)
 * Returns        : start position to max contiguous available reported RBs
 * Description    : This function Checks the longest possible set of size less 
                    than or equal to maxReqRBs in the mcsToPrbInfo_p that can be 
                    allocated and returns the RBs in the form 2x3y5z  
 *****************************************************************************/
/*TDD Config 0 Changes Start*/
#ifdef FDD_CONFIG
UInt8 checkAvailableRBsToReturnLongestSet(
    MCSToPrbMapInfo *mcsToPrbInfo_p, 
    UInt8 maxReqRBs, 
    UInt8 *availableRBs, 
    /* ICIC changes start */
    AvailableResInfo *rbMapNode_p,
    UInt8 *rbMap
    )
#elif TDD_CONFIG
MacRetType checkAvailableRBsToReturnLongestSet(
    MCSToPrbMapInfo *mcsToPrbInfo_p, 
    UInt8 maxReqRBs, 
    UInt8 *availableRBs, 
    /* ICIC changes start */
    AvailableResInfo *rbMapNode_p,
    UInt8 *rbMap,
    UInt8 *nDmrs_p,
    UInt8 phichSubFrameNum,
    UInt8 iPhich,
    UInt8 *maxAvailableRBsStartIdx,
    InternalCellIndex internalCellIndex
    )
#endif
    /*TDD Config 0 Changes End*/ 
{
  UInt8 startIdx = 0;
  UInt8 idx = 0;
#ifdef FDD_CONFIG
  UInt8 maxAvailableRBsStartIdx = 0;
#endif  
  UInt8 mcsToPrbEndIndex = 0;
  UInt8 tempMax = 0, tempCount = 0;
  mcsToPrbEndIndex = mcsToPrbInfo_p->endIdx;
  
  /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
  UInt8 tempAvailableN2dmrsIndex     = MAX_DMRS_VALUE;
#endif
  /*TDD Config 0 Changes End*/ 

  /* Start Index should start from the available free RB Index */	
  if(rbMapNode_p->freeRBIndex > mcsToPrbInfo_p->startIdx)
  {
    startIdx = rbMapNode_p->freeRBIndex;
  }
  else
  {
    startIdx = mcsToPrbInfo_p->startIdx;
  }

  if (mcsToPrbEndIndex > rbMapNode_p->endIndex)
  {
    mcsToPrbEndIndex = rbMapNode_p->endIndex;
  }

  idx = startIdx;
          /* ICIC changes start */
  /*klockworks warning fix*/
  /*Coverity_Warning_Fix_Start*/        
  while(idx <= mcsToPrbEndIndex && idx < MAX_NUM_RBS)
  /*Coverity_Warning_Fix_End*/    
  {
    if (rbMap[idx])
    {
      idx += rbMap[idx];
      continue;
    }

    if (tempMax >= mcsToPrbInfo_p->prbMap[idx])
    {
      idx += mcsToPrbInfo_p->prbMap[idx] + 1;
      continue;
    }
    tempCount = 0;

    /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
    *nDmrs_p = MAX_DMRS_VALUE;
    /* Check if Phich resources are available at the given ristart*/
    if (MAC_SUCCESS != checkAvailabilityOfPhichGrpAndPhichSeq(idx,
                nDmrs_p,
                phichSubFrameNum,
                iPhich,
                internalCellIndex))
    {
        /*Ndmrs is not available at this RBIndex;
         * move to next free location; */
        idx++;
        continue;
    }
#endif
    /*TDD Config 0 Changes End*/ 
                               
                                                             
    while ((idx <= mcsToPrbEndIndex)&&
        (FREE_RESOURCE == rbMap[idx]) && 
        (mcsToPrbInfo_p->prbMap[idx]!= 0))
          /* ICIC changes end */
    {
      tempCount++;
      idx++;
      if(tempCount >= maxReqRBs)
      {
        *availableRBs = getUplinkMinRBsInPowerOfTwoThreeFive(tempCount);
        /*TDD Config 0 Changes Start*/ 
#ifdef FDD_CONFIG
        maxAvailableRBsStartIdx = idx-tempCount;
        return maxAvailableRBsStartIdx;
#elif TDD_CONFIG
       *maxAvailableRBsStartIdx = idx - tempCount;
       return MAC_SUCCESS;
#endif
       /*TDD Config 0 Changes End*/ 
      }
    }
    /* ICIC changes end */
    /*Update tempMax if tempCount > tempMax. Also store the startPosition */
    if(tempCount > tempMax)
    {
      tempMax = tempCount;
      /*TDD Config 0 Changes Start*/
#ifdef FDD_CONFIG
      maxAvailableRBsStartIdx = idx-tempCount;
#elif TDD_CONFIG
      /*Update the temp ndmrs value*/
      *maxAvailableRBsStartIdx = idx - tempCount;
      tempAvailableN2dmrsIndex = *(nDmrs_p);
#endif
      /*TDD Config 0 Changes End*/
    }
    idx++;
  }

  /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
  /* Populate nDmrs_p with the actual value allocated*/
  if( tempAvailableN2dmrsIndex != MAX_DMRS_VALUE)
  {
      *(nDmrs_p) = tempAvailableN2dmrsIndex;
  }
  else
  {
      return MAC_FAILURE;
  }
#endif
  /*TDD Config 0 Changes End*/

  if(tempMax)
  {
    tempMax = getUplinkMinRBsInPowerOfTwoThreeFive(tempMax);
  }

  *availableRBs = tempMax;
  /*TDD Config 0 Changes Start*/
#ifdef FDD_CONFIG
  return maxAvailableRBsStartIdx;
#elif TDD_CONFIG
  return MAC_SUCCESS;
#endif
  /*TDD Config 0 Changes End*/
}
/*****************************************************************************
 * Function Name  : getAvailableRbsForLongestSet
 * Inputs         : mcsToPrbMap_p - pointer to MCSToPrbMapInfo,
 *                  maxReqRBs - maximum required RBs
 *                  availableRBs_p,
 *                  rbMapNode_p - pointer to AvailableResInfo,
 *                  ulDelay - Uplink Delay,
 *                  currentTTI,
 *                  ulUEContext_p - pointer to ULUEContext,
 *                  startIdx_p
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function process  longest possible set of size less 
 *                  than or equal to maxReqRBs in the mcsToPrbInfo_p that can be 
 *                  allocated 
 *****************************************************************************/
/* TTIB_Code  Start*/
MacRetType getAvailableRbsForLongestSet(
                            MCSToPrbMapInfo *mcsToPrbMap_p,
                            UInt8 maxRBs,
                            UInt8 *availableRBs_p,
                            RbMapNode *rbMapNode_p,
#ifdef FDD_CONFIG
                            UInt8 ulDelay,
                            /* +- SPR 17777 */
                           /* SPR 15909 fix start */
                            tickType_t currentTTI,
                            /* SPR 15909 fix end */
#endif
                            /* +- SPR 17777 */
                            ULUEContext* ulUEContext_p,
                            UInt8 *startIdx_p
                            /*CA Changes start  */
                            ,InternalCellIndex internalCellIndex
                            /*CA Changes end  */
                            /*TDD Config 0 Changes Start*/ 
#ifdef TDD_CONFIG
                           ,UInt8 *nDmrs_p
                           ,UInt8 phichSubFrameNum
                           ,UInt8 iPhich
#endif
                           /*TDD Config 0 Changes End*/ 
                            )
{
#ifdef FDD_CONFIG
    if(ulUEContext_p->ttiBundlingEnabled)
    {
        *startIdx_p = checkAvailableRBsToReturnLongestSetForTTIB( 
                mcsToPrbMap_p, 
                maxRBs, 
                availableRBs_p, 
                &rbMapNode_p->availableResourceInfo[ulUEContext_p->userLocationType],
                rbMapNode_p->rbMap,
                ulDelay,
                currentTTI,
                /* +- SPR 17777 */
                internalCellIndex
                ); 
    }
    else
    {
        *startIdx_p = checkAvailableRBsToReturnLongestSet( 
                mcsToPrbMap_p, 
                maxRBs, 
                availableRBs_p, 
                &rbMapNode_p->availableResourceInfo[ulUEContext_p->userLocationType],
                rbMapNode_p->rbMap
                );
    }
    return MAC_SUCCESS;
    /*TDD Config 0 Changes Start*/
#elif TDD_CONFIG
    MacRetType retVal = MAC_FAILURE;
    retVal = checkAvailableRBsToReturnLongestSet(
            mcsToPrbMap_p,
            maxRBs,
            availableRBs_p,
            &rbMapNode_p->availableResourceInfo[ulUEContext_p->userLocationType],
            rbMapNode_p->rbMap,
            nDmrs_p,
            phichSubFrameNum,
            iPhich,
            startIdx_p,
            internalCellIndex
            );
     return retVal;
#endif
    /*TDD Config 0 Changes End*/    
}
/* TTIB_Code Start  */
#ifdef FDD_CONFIG 
/*****************************************************************************
 * Function Name  : checkAvailableRBsToReturnLongestSetForTTIB
 * Inputs         : mcsToPrbInfo_p
 *                  maxReqRBs - maximum required RBs
 *                  availableRBs,
 *                  rbMapNode_p - pointer to AvailableResInfo,
 *                  rbMap,
 *                  ulDelay,
 *                  tickToReserve,
 *                  ulUEContext_p - pointer to ULUEContext
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : available Rbs (max contiguous)
 * Returns        : start position to max contiguous available reported RBs
 * Description    : This function Checks the longest possible set of size less 
 *                  than or equal to maxReqRBs in the mcsToPrbInfo_p that can be 
 *                  allocated and returns the RBs in the form 2x3y5z.
 *****************************************************************************/
UInt8 checkAvailableRBsToReturnLongestSetForTTIB(
    MCSToPrbMapInfo *mcsToPrbInfo_p, 
    UInt8 maxReqRBs, 
    UInt8 *availableRBs, 
    AvailableResInfo *rbMapNode_p,
    UInt8 *rbMap,
    UInt8 ulDelay,
    /* SPR 15909 fix start */
    tickType_t tickToReserve,
    /* SPR 15909 fix end */
    /* +- SPR 17777 */
    /*CA Changes start  */
    InternalCellIndex internalCellIndex
    /*CA Changes end  */
    )
{
    UInt8 startIdx = 0;
    UInt8 idx = 0;
    UInt8 maxAvailableRBsStartIdx = 0;
    UInt8 mcsToPrbEndIndex = 0;
    UInt8 tempMax = 0, tempCount = 0;
    UInt8 maxValofAllMaps       = 0; 
    

    LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,"[%s] [%d] maxReqRBs %d ulDelay %d tickToReserve %d \n",
                    __func__,getCurrentTick(),maxReqRBs,ulDelay,tickToReserve); 

    mcsToPrbEndIndex = mcsToPrbInfo_p->endIdx;


    /* Start Index should start from the available free RB Index */	
    if(rbMapNode_p->freeRBIndex > mcsToPrbInfo_p->startIdx)
    {
        startIdx = rbMapNode_p->freeRBIndex;
    }
    else
    {
        startIdx = mcsToPrbInfo_p->startIdx;
    }

    if (mcsToPrbEndIndex > rbMapNode_p->endIndex)
    {
        mcsToPrbEndIndex = rbMapNode_p->endIndex;
    }

    idx = startIdx;
    /* ICIC changes start */
    while(idx <= mcsToPrbEndIndex)
    {
        if (rbMap[idx])
        {
            idx += rbMap[idx];
            continue;
        }
       
        /* check for next 3 ticks and in case of conflict add maximum value to idx*/
        maxValofAllMaps = checkResConflictForSpsRachRbMaps(idx,tickToReserve,ulDelay,
                                                           internalCellIndex);
        if(maxValofAllMaps)
        {
            idx += maxValofAllMaps;
            continue;
        }

        if (tempMax >= mcsToPrbInfo_p->prbMap[idx])
        {
            idx += mcsToPrbInfo_p->prbMap[idx] + 1;
            continue;
        }

        tempCount = 0;
        while ((idx <= mcsToPrbEndIndex)&&
                (FREE_RESOURCE == rbMap[idx]) && 
                (mcsToPrbInfo_p->prbMap[idx]!= 0) &&
                (0 == checkResConflictForSpsRachRbMaps(idx,tickToReserve,ulDelay,internalCellIndex)))
            /* ICIC changes end */
        {
            tempCount++;
            idx++;
            if(tempCount >= maxReqRBs)
            {
                *availableRBs = getUplinkMinRBsInPowerOfTwoThreeFive(tempCount);
                maxAvailableRBsStartIdx = idx-tempCount;
                LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,"Exiting [%s]\n",__func__); 
                return maxAvailableRBsStartIdx;
            }
        }
        /* ICIC changes end */
        /*Update tempMax if tempCount > tempMax. Also store the startPosition */
        if(tempCount > tempMax)
        {
            tempMax = tempCount;
            maxAvailableRBsStartIdx = idx-tempCount;
        }
        idx++;
    }

    if(tempMax)
    {
        tempMax = getUplinkMinRBsInPowerOfTwoThreeFive(tempMax);
    }


    *availableRBs = tempMax; 
    LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,"Exiting : [%s].\n",__func__); 
    return maxAvailableRBsStartIdx;

}
#endif
/* TTIB_Code  End*/
/*****************************************************************************
 * Function Name  : checkNumResourcesFromRBStartIndex
 * Inputs         : startIndex - start index.
 *                  length - Number of resources to be reserved.
 *                  rbMapNum - RB Map Number
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks if length(number) of resources are 
 *                   available or not.
 *****************************************************************************/
MacRetType checkNumResourcesFromRBStartIndex (
    /* SPR 5530 changes start */
    UInt8 startIndex,
    UInt8 length ,                        
    /* SPR 5530 changes end */
    UInt32 rbMapNum,
    InternalCellIndex internalCellIndex
    )
{
  UInt8 pos;
  UInt8 endIndex = startIndex + length - 1;
  /*CA Changes start  */
  RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
  /*CA Changes end  */

  /* validate the startIndex lies between startIndex and endIndex.*/
  /* ULA_UTP */
  if ( ( startIndex < rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBIndex) || 
      /* ULA_UTP */
          ( endIndex > rbMapNode_p->availableResourceInfo[CC_CE_REGION].endIndex))
  {
      return MAC_FAILURE;
  }
  else
  {
      for ( pos = startIndex; pos <= endIndex; pos++)
      {
          if ( FREE_RESOURCE != rbMapNode_p->rbMap[pos])
          {
              return MAC_FAILURE;
          }
      }
  }
    return MAC_SUCCESS; 
}

#ifdef FDD_CONFIG 
/*****************************************************************************
 * Function Name  : checkNumOfBundleResFromRBStartIndex 
 * Inputs         : startIndex - start index,
 *                   length - Number of resources to be reserved,
 *                   tickToReserve,
 *                   ulDelay
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function checks if length(number) of resources are 
 *                  available or not for 4 consecutive ticks at n+9,n+10,n+11,
 *                  n+12. If not then returns fail.It consider PRACH resources , 
 *                  SPS resources and retransmission resources reserved by 
 *                  TTI Bunldled UEs.
 *****************************************************************************/
MacRetType checkNumOfBundleResFromRBStartIndex( UInt8 startIndex,
        /* SPR 15909 fix start */
    UInt8 length, tickType_t tickToReserve, UInt8 ulDelay,
        /* SPR 15909 fix end */
    InternalCellIndex internalCellIndex )
{
    RbMapNode*    rbMapNode_p            = PNULL;
    PRACHConfiguration *prachConfig_p    = PNULL;
    UInt32        sysFrameNum            = 0;
    /* SPR 15909 fix start */
    tickType_t        reserveResForTick      = 0;
    /* SPR 15909 fix end */
    UInt32        prachFrequencyOffset   = 0;
    UInt32        numPhysicalResources   = 0;
    UInt8         subFrameNum            = 0;
    UInt8         bundleCurrentTick      = 0;
    UInt8         endIndex               = startIndex + length - 1;

    prachConfig_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
            rachConfigInfo.prachConfiguration[cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->
            rachConfigInfo.prachActiveIndex]);

    /* validate the startIndex lies between startIndex and endIndex.*/
    for( bundleCurrentTick = 0; bundleCurrentTick < TTI_BUNDLE_SIZE; bundleCurrentTick++)
    {
        reserveResForTick = tickToReserve + ulDelay + FDD_HARQ_OFFSET + bundleCurrentTick;
        rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + MODULO_SIXTEEN(reserveResForTick);

        if ( ( startIndex < rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBIndex) || 
                ( endIndex > rbMapNode_p->availableResourceInfo[CC_CE_REGION].endIndex))
        {
            return MAC_FAILURE;
        }
    }
    while ( startIndex <= endIndex )
    {
        for( bundleCurrentTick = 0; bundleCurrentTick < TTI_BUNDLE_SIZE; 
                bundleCurrentTick++ )
        {
            reserveResForTick = tickToReserve + ulDelay + FDD_HARQ_OFFSET + 
                bundleCurrentTick;
            subFrameNum = reserveResForTick % MAX_NUM_OF_SUBFRAME;
            sysFrameNum = MODULO_ONEZEROTWOFOUR( reserveResForTick / 
                    MAX_NUM_OF_SUBFRAME);
            UInt32 rbMapNum = MODULO_SIXTEEN( reserveResForTick );

            if( prachConfig_p->prachFrequencyOffset <= startIndex &&
                    ( prachConfig_p->prachFrequencyOffset + 
                      NUM_PRACH_PHYSICAL_RESOURCES ) >= startIndex )
            {
                /* Check for first SF only */
                if( isValidFrameForPRACH( subFrameNum, sysFrameNum,
                            &prachFrequencyOffset, &numPhysicalResources,
                            internalCellIndex))
                {
                    return MAC_FAILURE;
                }
                /* Check for other SFs in case of Preamble format other than 
                 * 0 */
                else if( RET_TRUE == isPRACHResourcesAllocated(rbMapNum, 
                            internalCellIndex) )
                {
                    return MAC_FAILURE;
                }
            }
            /* + SPR 14284 Changes */
            else if( (TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                        initParams_p->spsSysWideParam.enableSps) && 
                     ((spsULStrTTIMap_gp[internalCellIndex] + ((tickToReserve + ulDelay + bundleCurrentTick) 
                            % MAX_SPS_INTERVAL))->rbMapNode.rbMap[startIndex]))
            /* - SPR 14284 Changes */
            {
                return MAC_FAILURE;
            }

            if((rbMapNodeArr_gp[internalCellIndex] + MODULO_SIXTEEN(reserveResForTick))->rbMap[startIndex])
            {
                return MAC_FAILURE;
            }
        }
        startIndex++;
    }
    return MAC_SUCCESS; 
}

/*****************************************************************************
 *   Function Name  : reserveAlreadyAvailableResourcesForBundleRetx
 *   Inputs         : startIndex - start index,
 *                    length - Number of resources to be reserved,
 *                    availRes_p - pointer to AvailableResInfo,
 *                    rbMap,
 *                    ulDelay,
 *                    currentTick
 *                    internalCellIndex - Cell-Index at MAC
 *   Outputs        : None
 *   Returns        : Actual number of resources marked busy
 *   Description    : This function reserves length(number) of 
 *                    resources for TTI bundled UE for consecutive resources
 *                    which were already checked for availability.
 *****************************************************************************/
UInt32 reserveAlreadyAvailableResourcesForBundleRetx (
    UInt32 startIndex,
    UInt32 length ,                        
    AvailableResInfo  *availRes_p,
    /* +- SPR 17777 */
    UInt8  ulDelay,
    /* SPR 15909 fix start */
    tickType_t currentTick,
    /* SPR 15909 fix end */
    InternalCellIndex internalCellIndex
    )
{
  SInt32    sStartIndex         = startIndex;
  SInt8     pos                 = 0;
  UInt8     reservedRbsCount    = 0;
  UInt8     endIndex            = sStartIndex + length - 1;
  UInt8     busyCount           = 1;
  UInt8     rbMapNum            = 0;
  UInt8     bundleTick          = 0;

  LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,"Entering : [%s].\n",__func__); 

  /* set the Resources as BUSY_RESOURCE */
  rbMapNum = MODULO_SIXTEEN(currentTick + ulDelay + FDD_HARQ_OFFSET + TTI_PHICH_TO_DCI_OFFSET);
  for(bundleTick = 0; bundleTick < TTI_BUNDLE_SIZE; bundleTick++)
  {
      pos = endIndex;
      reservedRbsCount = 0;
      busyCount = 0;
      /*CA Changes start  */
      RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + MODULO_SIXTEEN(rbMapNum + bundleTick);
      /*CA Changes end  */

      while(pos >= sStartIndex)
      {
          if(!rbMapNode_p->rbMap[pos])
          {
              reservedRbsCount++;
          }
          rbMapNode_p->rbMap[pos] = busyCount++;
          pos--;
      }
  }

  availRes_p->freeRBCounter -= reservedRbsCount;
  if ( availRes_p->freeRBIndex == sStartIndex )
  {
    availRes_p->freeRBIndex += length;
  }

  LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,"Exiting : [%s].\n",__func__); 
  return reservedRbsCount;
}
/********************************************************************************
* Function Name     : enqueueNodesInTTIBScheduleQ
* Inputs            : enqueueTick - tick at which DCI of  bundle is send,
*                     UeIndex - Index of UE,
*                     harqId - HARQ ID,
*                     bundleStartTick - 0 [for retransmision] or 1 [for
*                                       fresh bundle transmision]
*                                       For retx, 4 nodes will be enqueued and for fresh tx 3
*                                       nodes are to enqueued,
*                     isSpsEnabled
*                     internalCellIndex - Cell-Index at MAC
*
* Outputs           : None
* Returns           : None
* Description       : This function enqueue nodes to queue ttibScheduleQueue_g[]
*                     at (n+1)%16,(n+2)%16,(n+3)%16 tick where n is the ttick where DCI is
*                     to be prepared
********************************************************************************/

                                /* SPR 15909 fix start */
void enqueueNodesInTTIBScheduleQ(tickType_t enqueueTick,
                                /* SPR 15909 fix end */
                                 /* +- SPR 18268 */
                                 UInt16 UeIndex ,
                                 /* +- SPR 18268 */
                                 UInt8 harqId,
                                 UInt8 bundleStartTick,
                                 UInt8 isSpsEnabled,
                                 InternalCellIndex internalCellIndex)
{
    UInt16            rbMapNumForBundle  = 0;
    /* SPR 15909 fix start */
    tickType_t            bundleCurrentTick  = 0;
    /* SPR 15909 fix end */
    TtiBScheduleNode* ttiBScheduleNode_p = PNULL;

    for( bundleCurrentTick = bundleStartTick ; bundleCurrentTick < TTI_BUNDLE_SIZE; bundleCurrentTick++)
    {
//CODE_COMMENT_L18_FIXED
        rbMapNumForBundle = ((enqueueTick + bundleCurrentTick + FDD_DATA_OFFSET) & (NUM_OF_RB_MAP -1));
        GET_MEM_NODE_TTIB_SCHEDULE_Q(ttiBScheduleNode_p,TtiBScheduleNode);
        if(PNULL == ttiBScheduleNode_p)
        {
            ltePanic("getMemFromPool fails %s", __func__);
            /* Coverity Warning : 36221*/ 
            return ;
            /* Coverity Warning : 36221*/ 
        }     
        ttiBScheduleNode_p->ueIndex = UeIndex;
        ttiBScheduleNode_p->harqProcessId = harqId ;
        ttiBScheduleNode_p->isSpsEnable = isSpsEnabled;
        UInt8 ttibQueueIndex = (enqueueTick + bundleCurrentTick) % MAX_TTIB_SCHEDULE_QUEUE;
        /*coverity id 63528*/

        if(!ENQUEUE_TTIB_SCHEDULE_Q((ttibScheduleQueue_gp[internalCellIndex] + ttibQueueIndex),
                TtiBScheduleNode,ttiBScheduleNode_p))
        {
            FREE_MEM_NODE_TTIB_SCHEDULE_Q(ttiBScheduleNode_p);
        }
        (rbMapNodeArr_gp[internalCellIndex] + 
         rbMapNumForBundle)->advanceAllocFlag = ADVANCE_ALLOC_INIT_DONE;
    }
}
#endif

/*****************************************************************************
 * Function Name  : reserveAlreadyAvailableResourcesForRetx
 * Inputs         : startIndex - start index,
 *                  length - Number of resources to be reserved,
 *                  availRes_p - pointer to AvailableResInfo,
 *                  rbMap
 * Outputs        : None
 * Returns        : Actual number of resources marked busy
 * Description    : This function reserves length(number) of resources which were
 *                  already checked for availability.
 *****************************************************************************/
UInt32 reserveAlreadyAvailableResourcesForRetx (
    /* SPR 5530 changes start */
    UInt8 startIndex,
    UInt8 length ,                        
    /* SPR 5530 changes end */
    /* ICIC changes start */
    AvailableResInfo  *availRes_p,
    UInt8  *rbMap 
    /* ICIC changes end */
    )
{
  /*SPR 2389 Start*/
  /* SPR 5530 changes start */
  SInt8 sStartIndex = startIndex;
  /* SPR 5530 changes end */
  /*SPR 2389 End*/
  UInt8 reservedRbsCount  = 0;
  /*SPR 2389 Start*/
  /* UInt8 endIndex = sStartIndex + length - 1; */
  /*SPR 2389 End*/
#ifdef TDD_CONFIG
  SInt8 pos               = 0;
  UInt8 busyCount         = 1;
  /* set the Resources as BUSY_RESOURCE */
  pos = sStartIndex + length - 1;
#endif


  /*SPR 2389 Start*/
#ifdef TDD_CONFIG
  while(pos >= sStartIndex)
    /*SPR 2389 End*/
  {
    if(!rbMap[pos])
    {
      /*Resource is free*/
      reservedRbsCount++;
    }
    rbMap[pos] = busyCount++;
    pos--;
  }
#endif
  /* ICIC changes start */
  /* CC resource info is updated always. */
  /* updated the freeRBCounter */
  availRes_p->freeRBCounter -= reservedRbsCount;
  /* Update the FreeRBIndex */
  /*SPR 2389 Start*/
  if ( availRes_p->freeRBIndex == sStartIndex )
    /*SPR 2389 End*/
  {
    availRes_p->freeRBIndex += length;
  }
  LOG_MAC_MSG(MAC_NON_ADAPTIVE_RETXN_RES_ALLOC,LOGDEBUG,MAC_PUSCH,\
          getCurrentTick(),\
          startIndex,\
          length,\
          availRes_p->freeRBIndex,\
          availRes_p->endIndex,\
          availRes_p->freeRBCounter,\
          0.0,0.0,\
          __func__,"RETXN_RESOURCE_ALLOCATION");

  /* ICIC changes end */
  return reservedRbsCount;
}

/* ULA_CHG */

/*****************************************************************************
 * Function Name  : getRIV 
 * Inputs         : prbIndex - Its point to the Physical Resource Block Index,
 *                  length   - Number of Resource allocated.
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : resourceIndValue
 * Description    : This function return the Resource Indication Value
 *                  corresponding to start Resouce Block and length.
 *****************************************************************************/
/*CA Changes start  */
 UInt32 getRIV(UInt32 prbIndex , UInt32 length, InternalCellIndex internalCellIndex)
{
  LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
      "[%s] PRBIndex[%d] Length[%d] Entry. ",
      __func__,prbIndex, length );

  CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

  /* Refer Specs 36213-860 Section 7.1.6 Resource Allocation

     7.1.6.3 Resource Allocation type 2 

     Format 1A,1B, or 1D, a type 2 resource allocation field consists of 
     a resource indication value (RIV) corresponding to starting Resource
     block (RBstart) and a length in terms of virtually contiguously allocated
     resoucres blocks.
   */
  UInt32 resourceIndValue;   
  if ( (length - 1 ) <= ( DIVIDE_BY_TWO(cellParams_p->ulAvailableRBs)))
  {
    resourceIndValue = cellParams_p->ulAvailableRBs *(length-1)+ prbIndex ;
  }
  else
  {
    resourceIndValue = cellParams_p->ulAvailableRBs *
      ( cellParams_p->ulAvailableRBs - length + 1) +
      (cellParams_p->ulAvailableRBs - 1 - prbIndex +
       0);
  }
  LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,"[%s] RIV[%d] Exit. ",
      __func__,resourceIndValue);
  return resourceIndValue;
}
/*CA Changes end  */
/*****************************************************************************
 * Function Name  : freeResourceUsingRBIndex
 * Inputs         : length - Number of RBs to be free,
 *                  prbIndex - Its represent the physical resoucre block Index,
 *                  rbMapNum - It represent the RB Map Number
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : Free the Resources.
 * Returns        : None
 * Description    : This function free the RBs at particular prbIndex to specific
 *                  length and update the freeRBCounter.
 *****************************************************************************/
void freeResourceUsingRBIndex(  UInt32 length,
    UInt32 prbIndex,
    UInt32 rbMapNum,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex
    /*CA Changes end  */
    )
{
  LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,
      "[%s] RbMapNum[%d] prbIndex[%d] length[%d] Entry. ",
      __func__,rbMapNum, prbIndex, length);
  RbMapNode *rbMapNode_p  = PNULL;
  /*CA Changes start  */
  rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
  /*CA Changes end  */
  /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
  UInt32 idx               = 0;
  UInt32 cond              = 0;
  UInt8 rbgIdx = 0;
  UInt8 totalfree = (rbMapNode_p->rat1bitmap & BITMASK_27BITS)>>27;

  cond = prbIndex + length;
  for (idx = prbIndex; idx < cond ; idx++)
  {
    rbMapNode_p->rbMap[idx] = FREE_RESOURCE;
	  if((0 == (idx % rbgSize_g[internalCellIndex]))&&
			  ((cond - idx) >= rbgSize_g[internalCellIndex]))
	  {
		  rbgIdx = floor_wrapper(idx / rbgSize_g[internalCellIndex]);
		  rbMapNode_p->rbgMapForRAT1[rbgIdx] = FREE_RESOURCE;
		  rbMapNode_p->rat1bitmap &= ~(1 << rbgIdx);

		  totalfree++;
	  }
  }
  rbMapNode_p->rat1bitmap = rbMapNode_p->rat1bitmap & REFRESHBITMASK_27BITS;
  rbMapNode_p->rat1bitmap |= totalfree<<SHIFT27BITS;
#endif
  /* SPR 21958 PUSCH RAT1 Support End */
  /* updated the free number of Resources. */
  rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter += length;

  if ( rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBIndex -length == prbIndex)
  {
    rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBIndex -= length;
  }
  else
  {
    rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBIndex = rbMapNode_p->minIndex;
  }
  LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
      "[%s] RbMapNum[%d] Exit. ",
      __func__, rbMapNum);

}

/* + TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
#ifdef FDD_CONFIG
/**************************************************************************
 * Function:     freeAllTTIBRARResources
 * Description:  This function will free rbmap resources and harq if UE is 
 *               TTI Bundling enabled.
 * Input:        DLRARsp,
 *               rarStartIndex
 *               rarEndIndex
 * Output:       void
 *************************************************************************/ 
 
void freeAllTTIBRARResources(DLRARsp   *rarToDl, 
                             UInt32 rarStartIndex, 
                             UInt32 rarEndIndex,
                             InternalCellIndex internalCellIndex)
{
    UInt8              bundleTick       = 0;
    UInt8              rbMapNum         = 0;
    UInt32             index            = 0;
    ULUEContext       *ulUEContext_p    = PNULL;
    ULHarqInfo*        tempULHarqInfo_p = PNULL;
    RARspResourceInfo* resourceInfo_p   = PNULL;

        /* SPR 21870 Fix Start */
    for(index=rarStartIndex; index < rarEndIndex; index++)
    {
        /* SPR 21870 Fix End */
        resourceInfo_p = &(rarToDl->resourceInfo[index]);

        ulUEContext_p = ulUECtxInfoArr_g[resourceInfo_p->ueIndex].ulUEContext_p;
 
        if((FALSE == ulUECtxInfoArr_g[resourceInfo_p->ueIndex].pendingDeleteFlag) &&
           (PNULL != ulUEContext_p) && (ulUEContext_p->ttiBundlingEnabled))
        {
            tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[resourceInfo_p->harqProcessId]);
            for(bundleTick = 0; 
                    (bundleTick < TTI_BUNDLE_SIZE) && 
                                (tempULHarqInfo_p && tempULHarqInfo_p->isTTIBundled) ; bundleTick++)
            {
                rbMapNum = MODULO_SIXTEEN(rarToDl->tti + bundleTick);
                freeResourceUsingRBIndex(resourceInfo_p->riLength,resourceInfo_p->riStart, rbMapNum,
                                        internalCellIndex);
            }
#ifdef KPI_STATS
            freeULHarqProcess(tempULHarqInfo_p,resourceInfo_p->ueIndex,internalCellIndex);
#else            
            freeULHarqProcess(tempULHarqInfo_p);
#endif            
            updateTTIHarqMap(resourceInfo_p->harqProcessId,tempULHarqInfo_p,ulUEContext_p->ttiHarqMap);
        }
    }
    return;
}
#endif
/* - TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */

/*****************************************************************************
 * Function Name  : resetRBsAtLocation
 * Inputs         : rbMapNum - Its represent the rbMap Number
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : Reset With Zero
 * Returns        : None
 * Description    : This function  mark all the RBs at the rbMapNum to free.
 *****************************************************************************/
void resetRBsAtLocation(UInt32 rbMapNum,
        InternalCellIndex internalCellIndex)
{
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;

    /*advanceAllocFlag is set to FALSE after all the allocations are done
     * at the end of the execution leg*/
    rbMapNode_p->advanceAllocFlag = FALSE; 
#ifdef TDD_CONFIG
    /* Reset the number of RBs allocated for PRACH at the end of the execution
     * leg */
    rbMapNode_p->allocatedPrachNumRbs = 0;
#endif
    /* SPR 9729 fix start */
    /* Reset the uciRachFlag for PRACH at the end of the execution leg */
    rbMapNode_p->uciRachFlag = FALSE;
    /* SPR 9729 fix end */

    /* Set prachFlag to false at the end of the execution leg */
    rbMapNode_p->prachFlag = RET_FALSE;

    /* Reset RB map info once resources are allocated at the end of EL */
    *rbMapNode_p = (ulRAInfo_gp[internalCellIndex] + 
            ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
    
    LOG_UT(MAC_ADV_ALLOC_FLAG_RESET,LOGDEBUG,MAC_PUSCH,\
            getCurrentTick(),\
            rbMapNum,\
            (rbMapNodeArr_gp[internalCellIndex] + rbMapNum)->advanceAllocFlag,
            0,0,0,\
            0.0,0.0,\
            __func__,"FALSE");
}

/*****************************************************************************
 * Function Name  : reserveAlreadyAvailableResources 
 * Inputs         : startIndex - start index,
 *                  length - Number of resources to be reserved,
 *                  availRes_p - pointer to AvailableResInfo,
 *                  rbMap,
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function reserves length(number) of resources which were 
 *                  already checked for availability.
 *****************************************************************************/
MacRetType reserveAlreadyAvailableResources (
    UInt32 startIndex,
    UInt32 length ,                        
    /* ICIC changes start */
    AvailableResInfo  *availRes_p,
    UInt8  *rbMap 
    /* ICIC changes end */
    )
{
    UInt8 pos;
    UInt32 endIndex = startIndex+length;
    /* set the Resources the BUSY_RESOURCE */
    /* ICIC changes start */
    for ( pos = startIndex; pos < endIndex; pos++)
    {
        rbMap[pos] = (endIndex - pos);
    }
    /* updated the freeRBCounter */
    if (availRes_p->freeRBCounter > length)
    {
        availRes_p->freeRBCounter -= length;
        if( (endIndex - 1) == availRes_p->endIndex)
        {
            availRes_p->endIndex = startIndex - 1;
        }
    }
    else
    {
        availRes_p->freeRBCounter = 0;
    }
    /* Update the FreeRBIndex */
    if ( availRes_p->freeRBIndex == startIndex )
    {
        availRes_p->freeRBIndex += length;
    }
    LOG_MAC_MSG(MAC_DIRECT_RES_ALLOC_INFO,LOGDEBUG,MAC_PUSCH,\
            getCurrentTick(),\
            startIndex,
            length,
            availRes_p->freeRBIndex,\
            availRes_p->endIndex,\
            availRes_p->freeRBCounter,\
            0.0,0.0,\
            __func__,"RESOURCE_ALLOCATION");
    /* ICIC changes end */
    return MAC_SUCCESS;
}

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : reserveAlreadyAvailableResForThreeTicks
 * Inputs         : startIndex - start index,
 *                  length - Number of resources to be reserved,
 *                  res_p - pointer to AllocatedResInfo,
 *                  tickToReserve,
 *                  ulDelay,
 *                  ulUEContext_p - pointer to ULUEContext,
 *                  userLocType -
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function reserves length(number) of resources which were 
 *                  already checked for availability for three ticks.
 *****************************************************************************/
MacRetType reserveAlreadyAvailableResForThreeTicks (
    UInt32 startIndex,
    UInt32 length ,                        
    AllocatedResInfo  *res_p,
    /* SPR 15909 fix start */
    tickType_t tickToReserve,
    /* SPR 15909 fix end */
    UInt8 ulDelay,
    ULUEContext* ulUEContext_p,
    UInt8 userLocType,
    InternalCellIndex internalCellIndex
    )
{
    UInt32 endIndex = startIndex+length;
    UInt8 bundleCurrentTick = 0;
    UInt8 idx = 0;
    /* SPR 15909 fix start */
    tickType_t currentTick = 0;
    /* SPR 15909 fix end */
    UInt8 rbMapNum = MODULO_SIXTEEN(tickToReserve + ulDelay + FDD_HARQ_OFFSET);;
    AvailableResInfo *availRes_p = PNULL;
    /*TTIB cyclomatic complexity Fix*/
    if((!ulUEContext_p->ttiBundlingEnabled) 
    /* Klocworks fix start */
        || (endIndex > MAX_NUM_RBS+1) 
        || (startIndex > MAX_NUM_RBS+1))
    /* Klocworks fix end */
    {
        return MAC_FAILURE;
    }
    /*TTIB cyclomatic complexity Fix*/
    rbMapNum = MODULO_SIXTEEN(tickToReserve + ulDelay + FDD_HARQ_OFFSET);              
    for( bundleCurrentTick = 1; bundleCurrentTick < 4; bundleCurrentTick++)
    {
        /*CA Changes start  */
        RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + MODULO_SIXTEEN(rbMapNum + bundleCurrentTick);
        /*CA Changes end  */
        availRes_p = &rbMapNode_p->availableResourceInfo[userLocType];
        if (availRes_p->freeRBCounter > length)
        {
            availRes_p->freeRBCounter -= length;
            if( (endIndex - 1) == availRes_p->endIndex)
            {
                availRes_p->endIndex = startIndex - 1;
            }
        }
        else
        {
            availRes_p->freeRBCounter = 0;
        }
        if ( availRes_p->freeRBIndex == startIndex )
        {
            availRes_p->freeRBIndex += length;
        }


        for ( idx = startIndex ; idx < endIndex; idx++)
        {
            /* Tick for which resources are to be reserved*/
            currentTick = tickToReserve + ulDelay + FDD_HARQ_OFFSET + bundleCurrentTick; 
            if( MAC_SUCCESS == isTTIInMeasGapPeriod
                    (ulUEContext_p, currentTick)
              )
            {
                /* No need to allocate resources if tick falls in Meas gap period */
                LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,"[%s] No resources allocated if "
                        "[%d] tick falls in Meas gap period\n",__func__,currentTick); 
                continue;
            }
            rbMapNode_p->rbMap[idx] = endIndex - idx;
            if(CE_USER == userLocType)
            {
                updateCCAvailRes(&rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                        res_p);
            }


        }
    }

    /*
       for ( pos = startIndex; pos < endIndex; pos++)
       {
       rbMap[pos] = (endIndex - pos);
       }
     */
    /* updated the freeRBCounter */
    LOG_MAC_MSG(MAC_DIRECT_RES_ALLOC_INFO,LOGDEBUG,MAC_PUSCH,\
            getCurrentTick(),\
            startIndex,
            length,
            availRes_p->freeRBIndex,\
            availRes_p->endIndex,\
            availRes_p->freeRBCounter,\
            0.0,0.0,\
            __func__,"RESOURCE_ALLOCATION");
    /* ICIC changes end */
    return MAC_SUCCESS;
}
#endif
/*SPR 8451 Changes Start*/
#ifdef ULCODERATE_PLUGIN
/*****************************************************************************
Function Name  : checkULCodeRateUECategory
Inputs         : 
Outputs        :
Returns        : MAC_SUCCESS or MAC_FAILURE
Description    : This function check and allocate best combination of 
                 RB and MCS
 *****************************************************************************/
void checkULCodeRateUECategory
    (
        ULUEContext* ulUEContext_p,
        UInt8 *tempAllocatedMCS,
        UInt8 *rbAllocated,
        UInt8 buddyUeFlag,
        InternalCellIndex internalCellIndex
    )
{
    UInt8 ulCfi = 0;
    UInt8 modScheme = 0;
    UInt8 mcsIndex = 0;
    UInt8 maxAllocatedRB = 0;
    UInt32 ulSubframe = ((getCurrentTick()+PHY_DL_DELAY+FDD_HARQ_OFFSET)%MAX_SUB_FRAME);
    CellConfigParams *cellParams_p =
         cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
         cellParams_p;

    /*Check UL CFI of current SF
     * For Non SRS SF, CFI=2
     * For SRS SF, CFI=3*/
    if(LTE_FALSE ==  *(srsCellConfigMap_gp[internalCellIndex]+ ulSubframe))
    {
        ulCfi = UL_DEFAULT_CFI;
    }
    else
    {
        ulCfi = UL_EXTENDED_CFI;
    }

    /*Update MCS and RB according to UE category*/
    mcsIndex = *tempAllocatedMCS;
    maxAllocatedRB = *rbAllocated;

    /*Normal CP with DM-RS*/
    if((UL_DEFAULT_CFI == ulCfi) && (1 == cellParams_p->cyclicPrefix))
    {
        GET_UL_MOD_SCHEME_FROM_MCS_INDEX(mcsIndex,modScheme,cellParams_p->macEnable64QAM,ulUEContext_p->ueCategory);
        if((mcsIndex > 23) && (SIXTEENQAM == modScheme))
        {
            /*Change MCS to 23 and no need to change Number of RB
             * Code Rate problem observed only in MCS > 23*/
            *tempAllocatedMCS = 23;
        }
        return;
    }
    /*Extended CP with DM-RS*/
    else if((UL_DEFAULT_CFI == ulCfi) && (1 != cellParams_p->cyclicPrefix))
    {
        GET_UL_MOD_SCHEME_FROM_MCS_INDEX(mcsIndex,modScheme,cellParams_p->macEnable64QAM,ulUEContext_p->ueCategory);
        if((mcsIndex > 21) && (SIXTEENQAM == modScheme))
        {
            /*Change MCS to 21 and no need to change Number of RB
             * Code Rate problem observed only in MCS > 21*/
            *tempAllocatedMCS = 21;
        }
        return;
    }
    /*Normal CP with DM-RS and SRS*/
    else if((UL_EXTENDED_CFI == ulCfi) && (1 == cellParams_p->cyclicPrefix))
    {
        GET_UL_MOD_SCHEME_FROM_MCS_INDEX(mcsIndex,modScheme,cellParams_p->macEnable64QAM,ulUEContext_p->ueCategory);
        /*Code rate problem observed for MCS > 22 and Mod Scheme 16QAM*/
        if((mcsIndex > 22) && (modScheme >= SIXTEENQAM))
        {
            *tempAllocatedMCS = 
                mcsULRBMapForCodeRate_g.mcsULRBInfoNonSpatial[(((modScheme)/QPSK) - 1)][maxAllocatedRB-1][mcsIndex-19].\
                mcsIndexForUL;
            /*Number of RB's should not be reduced when Buddy UE lag is set, Only MCS to be lowered*/
            if(BUDDY_UE_NOT_SET == buddyUeFlag)
            {
                *rbAllocated = 
                    mcsULRBMapForCodeRate_g.mcsULRBInfoNonSpatial[(((modScheme)/QPSK) - 1)][maxAllocatedRB-1][mcsIndex-19].\
                    numberOfresourceBlocksForUL;
            }
        }
        return;
    }
    /*Extended CP with DM-RS and SRS*/
    else if((UL_EXTENDED_CFI == ulCfi) && (1 != cellParams_p->cyclicPrefix))
    {
        GET_UL_MOD_SCHEME_FROM_MCS_INDEX(mcsIndex,modScheme,cellParams_p->macEnable64QAM,ulUEContext_p->ueCategory);
        /*Code rate problem observed for MCS > 18*/
        if((mcsIndex > 18) && ((SIXTEENQAM == modScheme) || (SIXFOURQAM == modScheme)))
        {
            *tempAllocatedMCS = 
                mcsULRBMapForCodeRate_g.mcsULRBInfoNonSpatial[(((modScheme)/QPSK) - 1)][maxAllocatedRB-1][mcsIndex-19].\
                mcsIndexForUL;
            /*Number of RB's should not be reduced when Buddy UE lag is set, Only MCS to be lowered*/
            if(BUDDY_UE_NOT_SET == buddyUeFlag)
            {
                *rbAllocated = 
                    mcsULRBMapForCodeRate_g.mcsULRBInfoNonSpatial[(((modScheme)/QPSK) - 1)][maxAllocatedRB-1][mcsIndex-19].\
                    numberOfresourceBlocksForUL;
            }
        }
        return;
    }
    else
    {
        /*Not a code rate exceed condition*/
        return;
    }
}
#endif
/*SPR 8451 Changes End*/
