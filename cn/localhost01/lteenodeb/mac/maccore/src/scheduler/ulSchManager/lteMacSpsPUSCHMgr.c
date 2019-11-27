/*!
 * \file lteMacSpsPUSCHMgr.c
 * */
/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
******************************************************************************
 *
 *  File Description : THIS FILE CONTAINS THE FUNCTIONS RELATED TO SPS PUSCH
 *                          Allocation for UL SPS.
******************************************************************************

*****************************************************************************/


/******************************************************************************
 * Standard Library Includes
******************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteCommonStatsManager.h"
#include "lteMacUEConfMgr.h"
#include "lteMacDRXMgr.h"
#include "lteMacULSpsScheduler.h"
#include "lteMacULResourceInfoMgr.h"
#include "lteMacPRACH.h"
#include "lteMacSpsPUSCHmgr.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacPuschPowerControl.h"
#include "lteMacPUSCH.h"

/******************************************************************************
  Project Includes
*****************************************************************************/

/******************************************************************************
  Private Definitions
*****************************************************************************/
#define FIRST_TIME_SP_RB_ALLOC_COUNT 5

/******************************************************************************
  Private Types
 *****************************************************************************/
/*CA Changes start  */
SpsULStrIntervalTTISorted* ulSortedArr_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end  */
UInt8 sps_max_pucch_resource_g=3;

extern UInt8  maxpowerOfTwoThreeFive_g[MAX_NUM_RBS + 1];
extern UInt8  minpowerOfTwoThreeFive_g[MAX_NUM_RBS + 1];

extern RACHConfigInfo rachConfigInfo_g;
//Code Removed - SPS_TDD_Changes

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
MacRetType checkSpsUplinkResources( UInt32 numOfResourcesReq,
    UInt32 minReqdRBs,
    AllocatedResInfo *allocatedResInfo_p,
    UInt32 strictAllocatedFlag,
    AvailableResInfo *availRes_p,
    UInt8 *rbMap,
    UInt8 hiIndex,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex);
    /*CA Changes end  */
/* + SPS_TDD_Changes */
static  MacRetType checkDRXAndMeasGapForTtiIndex(UInt8 index,
#ifdef TDD_CONFIG
        SInt8 subFrame_offset,
	InternalCellIndex internalCellIndex,
#endif
        LP_SpsUlInfo      spsUlInfo_p
        );
/* - SPS_TDD_Changes */

void processULResourceReserveRequest(ULUEContext *ueContext_p, LP_MacULSpsResReq macSpsResReq_p,
                                     /*CA Changes start  */
                                     UInt8 ulDelay, InternalCellIndex internalCellIndex
                                     /*CA Changes end  */
/* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
                                     ,UInt8 subFrameNum
#endif
/* - SPS_TDD_Changes */
                                     );

void processUnionRbMapNode(RbMapNode* rbMapNodeUnion_p);
void updateRbMapNode(AllocatedResInfo* resourceReserved_p, RbMapNode* rbMapNode_p);

void updateRbMapsFromUnionRbMap( AllocatedResInfo* resourceReserved_p, 
        RbMapNode* rbMapNode1_p, RbMapNode* rbMapNode2_p);

LP_MacULSpsStatus spsULAllocateStatusDataReq( UInt16 ueIndex, UInt8   response,
        UInt8    requestType, UInt32  spsOpp, UInt16   numRbs);

void initSpsRbMapNode( RbMapNode* rbMapNode_p, UInt16 ulRBs);
/*CA Changes start  */
void  reservePRACHPUCCHRBInRM(UInt32 rmSize, InternalCellIndex internalCellIndex);
/*CA Changes end  */

/* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
                               /* SPR 15909 fix start */
UInt8 getHIIndexFor20Periodicity(tickType_t currentGlobalTick, UInt8 current_tti_index, 
                               /* SPR 15909 fix end */
              UInt8 tti_index1, UInt8 tti_index2, UInt8 ulDelay, UInt8 *target_tti_p);
#else
static void getTargetTTIIndexForTDD(UInt8 current_tti_index,
        UInt8 tti_index1, UInt8 tti_index2,  UInt8 *target_tti_p);
#endif
/* - SPS_TDD_Changes */

/*CA Changes start  */
void markHiResourceForAllSpsUEs(UInt8 hiIndex, UInt8 target_tti, InternalCellIndex internalCellIndex);
/*CA Changes end  */

UInt8 checkPhichGrpAndPhichSeq(UInt8 riStart,
                               UInt8 hiIndex,
                               /*CA Changes start  */
                               InternalCellIndex internalCellIndex);
                               /*CA Changes end  */

UInt8 markPhichGrpAndPhichSeq(UInt8 riStart,
                              UInt8 hiIndex,
                              /*CA Changes start  */
                              InternalCellIndex internalCellIndex);
                              /*CA Changes end  */
/*CA Changes start  */
#define SPS_PUSCH_ENQUEUE_FAILURE_STATUS(internalCellIndex) \
{\
    dataReq_p = spsULAllocateStatusDataReq( macSpsResReq_p->ueIndex, SPS_PDSCH_PUSCH_FAILURE,\
            MAC_SPS_RES_RSV_REQ_ACT, maxTBSize, requiredRBCount);\
    /*** Coverity 63545 Fix : Start ****/ \
    if (!ENQUEUE_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex], dataReq_p )) \
    {\
        freeMemPool(dataReq_p); \
        dataReq_p = PNULL; \
    }\
    /*** Coverity 63545 Fix : End ****/ \
    else\
    {\
        /* CA Stats Changes Start */\
        LTE_MAC_QUEUE_USED( UL_SPS_STATUS_Q, QCOUNT_MAC_UL_SPS_Q(\
                    ulSpsStatusQ_gp[internalCellIndex] ), internalCellIndex);\
        /* CA Stats Changes End */\
        freeMemPool ( macSpsResReq_p ); \
        macSpsResReq_p = PNULL; \
    }\
}
/*CA Changes end  */

/*****************************************************************************/
/*!
 * \fn   : void processDlSpsResResrvRelQ( UInt32 qCount )

 * \brief  RM(Resource Map) would be an array of 40 elements of data structure 
           spsRMInfo[40]. Each index of this array would contain resources of
           UEs whose periodicity correspond to that index i.e. SPS_TTI%40.

    For each index, following should be maintained:
    typedef struct SpsStrIntervalTTIContext
    {
    RAT2LocalizedInfo spsRat2Localized;
    UInt32 spsRBBitMap[4]; // for MAX 100 RB support, 1 RB/bit
    }spsRMInfo;

    For adding a new UE of periodicity 40ms, then
    1)    Calculate index of RM with least number of UEs (spsUECount). 
          After calculating this index i, give  RAT2LocalizedInfo spsRat2Localized
          to Sub-band CQI to know if given SPS TB Size can be accommodated.
    2)    If success, return Activation Index=i.
          Update spsRMInfo[i]. spsRBBitMap[4] with spsRMInfo[i].spsRat2Localized.
          If failure, calculate next least loaded index & repeat Step(1).

    For adding a new UE of periodicity 20ms, then
    3)    Among the first 20 indexes (i.e. 0<= index <=19), calculate index of RM
          with least number of UEs. LetÃ¢â¬â¢s say this index=j .
          By ORing of  spsRMInfo[j]. spsRBBitMap[4]  and  spsRMInfo[j+20].
          spsRBBitMap[4], calculate spsRBBitMapUnion[4]. Give this  (spsRBBitMapUnion[4])
          as input to Sub-band CQI to know if given SPS TB Size can be accommodated.

    Increment spsRMInfo[j]. spsUECount , spsRMInfo[j+20]. spsUECount for this UE in RM.
    Update spsRMInfo[j]. spsRBBitMap[4]  with spsRBBitMapUnion[4]
    Update spsRMInfo[j+20]. spsRBBitMap[4] with spsRBBitMapUnion[4].

    If failure, calculate next least loaded index & repeat Step(3).
    After finding activation index it would calculate SPS activation TTI and
    pass this information along with chosen MCS to DL SPS Strategy in dlSpsStatusQ.
 * */
 /***************************************************************************/
/******************************************************************************
 * Function Name  : ulSpsResourceReserveHandler
 * Inputs         : qCount - Number of nodes to be processed 
 *                  ulDelay -
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This will release the SPS  resources
 ******************************************************************************/
/*CA Changes start  */
void ulSpsResourceReserveHandler( UInt32 qCount, UInt8 ulDelay, 
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        UInt32 subframe,
#endif
        InternalCellIndex internalCellIndex
        )
        /* - SPS_TDD_Changes */
/*CA Changes end  */
{

    LP_MacULSpsResReq   macSpsResReq_p     = PNULL;
    ULUEContext         *ueContext_p       = PNULL;

    while( qCount-- )
    {   
        /*CA Changes start  */
        DEQUEUE_MAC_UL_SPS_Q( ulSpsResRsvQ_gp[internalCellIndex], macSpsResReq_p );
        /*CA Changes end  */
        if( PNULL == macSpsResReq_p )
        {
            /* +- SPR 17777 */
            LOG_MSG( MAC_SPS_NULL_NODE, LOGWARNING, MAC_PUSCH, getCurrentTick(),
                    __LINE__, ulDelay, 0,0,0, 0,0, __func__, "ulSpsResRsvQ_g");
            /* +- SPR 17777 */
            return;
        }

        /*!
         * \code
         * Get UE-Context
         * \endcode
         * */
        if (! ulUECtxInfoArr_g[macSpsResReq_p->ueIndex].pendingDeleteFlag)
        {
            ueContext_p = ulUECtxInfoArr_g[macSpsResReq_p->ueIndex].ulUEContext_p;
            if( PNULL == ueContext_p )
            {
                /* +- SPR 17777 */
                LOG_MSG( MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID, LOGFATAL, MAC_PUSCH, 
                        getCurrentTick(), __LINE__, macSpsResReq_p->ueIndex, 0,0,0,
                        0,0, __func__, "SPS_PUSCH");                
                /* Free the memory */
                freeMemPool ( macSpsResReq_p );
                macSpsResReq_p = PNULL;
                continue;
            }
            if( MAC_SPS_RES_RSV_REQ_ACT == macSpsResReq_p->requestType )
            {
#ifdef FDD_CONFIG
                /*CA Changes start  */
                processULResourceReserveRequest(ueContext_p,macSpsResReq_p,  ulDelay, internalCellIndex);
#else
                /* + SPS_TDD_Changes */
                processULResourceReserveRequest(ueContext_p,macSpsResReq_p,  ulDelay, internalCellIndex, subframe);
                /* - SPS_TDD_Changes */
                /*CA Changes end  */
#endif
                macSpsResReq_p = PNULL;
            }
            else
            {
               /* Free the memory */
               freeMemPool ( macSpsResReq_p );
               macSpsResReq_p = PNULL;    
            }
        }
    }
}

/******************************************************************************
 * Function Name  : ulSpsResourceReleaseHandler
 * Inputs         : qCount - Number of nodes to be processed 
 *                  internalCellIndex -  Cell-Index used at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This will release the SPS  resources
 ******************************************************************************/
void ulSpsResourceReleaseHandler( UInt32 qCount, InternalCellIndex internalCellIndex )
{
    LP_MacULSpsResReq   macSpsResReq_p     = PNULL;
    ULUEContext         *ueContext_p       = PNULL;

    while( qCount-- )
    {
        DEQUEUE_MAC_UL_SPS_Q( ulSpsResRelQ_gp[internalCellIndex], macSpsResReq_p );
        if( PNULL == macSpsResReq_p )
        {
            /* +- SPR 17777 */
            LOG_MSG( MAC_SPS_NULL_NODE, LOGWARNING, MAC_PUSCH, getCurrentTick(),
                    __LINE__, 1, 0,0,0, 0,0, __func__, "ulSpsResRelQ_g");
            return;
        }

        /*!
         * \code
         * Get UE-Context
         * \endcode
         * */
        if (! ulUECtxInfoArr_g[macSpsResReq_p->ueIndex].pendingDeleteFlag)
        {
            ueContext_p = ulUECtxInfoArr_g[macSpsResReq_p->ueIndex].ulUEContext_p;
            if( PNULL == ueContext_p )
            {
                /* +- SPR 17777 */
                LOG_MSG( MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID, LOGFATAL, MAC_PUSCH, 
                        getCurrentTick(), __LINE__, macSpsResReq_p->ueIndex, 0,0,0,
                        0,0, __func__, "SPS_PUSCH");                
                /* Free the memory */
                freeMemPool ( macSpsResReq_p );
                macSpsResReq_p = PNULL;
                continue;
            }
            if( MAC_SPS_RES_REL_REQ == macSpsResReq_p->requestType 
#ifdef FDD_CONFIG
            || (MAC_SPS_TTIB_RES_REL_REQ == macSpsResReq_p->requestType)
#endif
            )
            {
                processULResourceReleaseRequest(ueContext_p
#ifdef FDD_CONFIG
                                    ,macSpsResReq_p->requestType
#endif
                                    /*CA Changes start  */
                                    ,internalCellIndex
                                    /*CA Changes end  */
                                    );
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


#ifdef FDD_CONFIG
/** TTIB_Code Starts */
/******************************************************************************
 * Function Name  : getAvailableRbsFromMaps
 * Inputs         : ttiIndex - For loop Index till MAX_SPS_PERIODICITY
 *                  ulDelay -
 *                  ueContext_p - UL Ue Context
 *                  numOfTicksToCheck -  4, incase of TTIB UE
 *                                       else 1, for Non TTIB Ue
 *                  requiredRBCount - Number of Rbs required to be reserved
 *                  spsPeriodicity - 20 or 40.
 *                  internalCellIndex -  Cell-Index used at MAC
 *
 * Outputs        : rbMapNodeUnion - Map contains Union of all the Rb mAPS
 *                                   1) Non TTIB + SPS_PERIODICITY_40 - 
 *                                   Checks for current SPS MAp at Index spsIndex
 *                                   and Main RB Map at spsIndex+4,
 *                                   2)Non TTIB + SPS_PERIODICITY_20 - 
 *                                   Checks for current SPS MAp at Index spsIndex,
 *                                   spsIndex + 20,and Main RB Map at spsIndex+4,
 *                                   spsIndex + 20 + 4
 *                                   3) TTIB UE + SPS_PERIODICITY_40
 *                                   Checks for current SPS MAp at Index spsIndex,
 *                                   spsIndex +1,spsIndex+2,spsIndex+3
 *                                   and Main RB Map at spsIndex+4,
 *                                   spsIndex +1 +4,spsIndex+2+4,spsIndex+3+4
 *                                   4) TTIB UE + SPS_PERIODICITY_40
 *                                   Checks for current SPS MAp at Index spsIndex,
 *                                   spsIndex +1,spsIndex+2,spsIndex+3,spsIndex+20
 *                                   spsIndex +1+20,spsIndex+2+20,spsIndex+3+20
 *                                   and Main RB Map at spsIndex+4,
 *                                   spsIndex +1 +4,spsIndex+2+4,spsIndex+3+4,
 *                                   spsIndex+4 +20,
 *                                   spsIndex +1 +4+20,spsIndex+2+4+20,spsIndex+3+4
 *                                   +20
 * Returns        : return MAC_SUCCESS/MAC_CHECK_NEXT_TICK
 * Description    : This will fill the RbMapNode with union of all the Maps
 *                  (can be 2,4,8,16)
 ******************************************************************************/

MacRetType getAvailableRbsFromMaps(UInt8 ttiIndex,
                                   /* +- SPR 17777 */
                                   ULUEContext *ueContext_p,
                                   UInt8  numOfTicksToCheck,
                                   UInt8  requiredRBCount,
                                   RbMapNode* rbMapNodeUnion,
                                   UInt8  spsPeriodicity,
                                   /*CA Changes start  */
                                   InternalCellIndex internalCellIndex)
                                   /*CA Changes end  */
{
    LP_SpsULStrIntervalTTISorted spsTTIInfoArr_p    = PNULL;
    LP_SpsUlInfo                 spsUlInfo_p        = PNULL;
    UInt8                        bundleTick         = 0;
    UInt8                        spsIndex           = 0;
    UInt16 spsInterval = 
        ueContext_p-> spsUlInfo.spsUlSetupInfo_p->semiPersistentSchedIntervalUL;
    /*CA Changes start  */
    spsTTIInfoArr_p = ulSortedArr_gp[internalCellIndex];
    /*CA Changes end  */
    spsUlInfo_p = &ueContext_p->spsUlInfo;

    /*num of maps=8 for 40 periodicity, 16 for 20 periodicity*/
    RbMapNode *rbMapNode[MAX_RB_NODE_TO_CHECK]={0};
 /* Coverity 31811 fix start */
    UInt8     mapIndex=0;
 /* Coverity 31811 fix End */
    UInt8      idx = 0;
    /* numOfTicksToCheck = 1 or 4 */
    /*SPS_TTIB : Check whether in TTIB HARQ Map, that map is free at +4 of actTTIIndex
    value at index n+4,+5,+6,+7 shoukd be 8(i.e. MAp is free) where n = spsTTIInfoArr_p[ttiIndex].index
    for allocation of reaources for SPS for TTI Bundling. */
    /* SPR 13524 fix start */
    if(ueContext_p->ttiBundlingEnabled)
    {
    /* SPR 13524 fix end*/
        for (bundleTick = 0;bundleTick < numOfTicksToCheck; bundleTick++)
        {
            spsIndex = (spsTTIInfoArr_p[ttiIndex].index + bundleTick)% spsPeriodicity;
            if(TTI_MAP_FREE != ueContext_p->ttiHarqMap[MODULO_SIXTEEN(spsIndex + FDD_HARQ_OFFSET)])
            {
                return MAC_CHECK_NEXT_TICK;
            }
        } 
    /* SPR 13524 fix start */
    }
    /* SPR 13524 fix end */
 
    for (bundleTick = 0;bundleTick < numOfTicksToCheck; bundleTick++)
    {
        spsIndex = (spsTTIInfoArr_p[ttiIndex].index + bundleTick)% spsPeriodicity;

        /*CA Changes start  */
        rbMapNode[mapIndex++]= (RbMapNode*)(rbMapNodeArr_gp[internalCellIndex] + MODULO_SIXTEEN(spsIndex + FDD_HARQ_OFFSET));
        /*CA Changes end  */
        rbMapNode[mapIndex++]= &((spsULStrTTIMap_gp[internalCellIndex] + spsIndex)->rbMapNode);
        if(SPS_INTERVAL_20 ==  spsPeriodicity)
        {
            /*CA Changes start  */
            rbMapNode[mapIndex++]=rbMapNodeArr_gp[internalCellIndex] + MODULO_SIXTEEN(spsIndex + FDD_HARQ_OFFSET + SPS_INTERVAL_20);
            /*CA Changes end  */
            rbMapNode[mapIndex++]= &((spsULStrTTIMap_gp[internalCellIndex] + ((spsIndex + SPS_INTERVAL_20) % spsPeriodicity))->rbMapNode);
        }

        /*CA Changes start  */
        if( ((spsULStrTTIMap_gp[internalCellIndex] + spsIndex)->\
                    numULSpsDciPerTti == (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
                    spsUlSchInfoPerTti.maxUlSpsDciPerTti) ||
                ( ( requiredRBCount + (spsULStrTTIMap_gp[internalCellIndex] + spsIndex)->numULSpsRbsPerTti ) >
                  (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
                  spsUlSchInfoPerTti.maxUlSpsRbsPerTti) ||
                ( ( requiredRBCount + (spsULStrTTIMap_gp[internalCellIndex] + spsIndex)->numULSpsRbsPerTtiPerInterval[
                    spsInterval] ) >
                  (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
                  spsUlSchInfoPerTti.maxUlSpsRbsPerTtiPerInterval[spsInterval] ) ) 
        /*CA Changes end  */
        {
            return MAC_CHECK_NEXT_TICK;
        }
        /* + SPS_TDD_Changes */
        if(MAC_FAILURE == checkDRXAndMeasGapForTtiIndex(spsIndex,spsUlInfo_p))
        {
            return MAC_CHECK_NEXT_TICK;
        }
        /* - SPS_TDD_Changes */
    }   
    /* Initialise rbMapNodeUnion with parameter present at mapIndex-1 and then compare among all the Maps */
    /* Coverity 73239 Fix Start*/
    if(mapIndex)
    {
        rbMapNodeUnion->availableResourceInfo[0].freeRBCounter = rbMapNode[mapIndex-1]->availableResourceInfo[0].freeRBCounter;
        rbMapNodeUnion->availableResourceInfo[0].freeRBIndex   = rbMapNode[mapIndex-1]->availableResourceInfo[0].freeRBIndex;
        rbMapNodeUnion->availableResourceInfo[0].endIndex      = rbMapNode[mapIndex-1]->availableResourceInfo[0].endIndex;   
        for(idx= 0 ; idx < rbMapNode[0]->availableResourceInfo[0].endIndex ;idx++)
        {
            rbMapNodeUnion->rbMap[idx] = rbMapNode[mapIndex - 1]->rbMap[idx];
        }
    }
    /* Coverity 73239 Fix End*/

    while(mapIndex)
    {             
        mapIndex--; 

        rbMapNodeUnion->pucchFlag = rbMapNodeUnion->pucchFlag || rbMapNode[mapIndex]->pucchFlag;
        if (rbMapNodeUnion->availableResourceInfo[0].freeRBCounter > rbMapNode[mapIndex]->availableResourceInfo[0].freeRBCounter)
        {    
            rbMapNodeUnion->availableResourceInfo[0].freeRBCounter = rbMapNode[mapIndex]->availableResourceInfo[0].freeRBCounter;
        }

        if (rbMapNodeUnion->availableResourceInfo[0].freeRBIndex > rbMapNode[mapIndex]->availableResourceInfo[0].freeRBIndex) 
        {
            rbMapNodeUnion->availableResourceInfo[0].freeRBIndex = rbMapNode[mapIndex]->availableResourceInfo[0].freeRBIndex;
        }
        /*--coverity--530--54179*/
        rbMapNodeUnion->prachFlag = (BoolRetType)(rbMapNodeUnion->prachFlag || rbMapNode[mapIndex]->prachFlag);
        /*--coverity--530--54179*/

        /* Find Union of both RB map */
        for(idx = rbMapNodeUnion->availableResourceInfo[0].freeRBIndex; idx < rbMapNodeUnion->availableResourceInfo[0].endIndex; idx++)
            rbMapNodeUnion->rbMap[idx] = rbMapNodeUnion->rbMap[idx] || rbMapNode[mapIndex]->rbMap[idx];
    }

    return MAC_SUCCESS;
}
/******************************************************************************
 * Function Name  : processULResourceReserveRequest
 * Inputs         : ueContext_p - UL UEContext 
 *                  macSpsResReq_p. Node filled by UL SPS Strategy for SPS-PDSCH
 *                  ulDelay -
 *                  internalCellIndex - Cell-Index at MAc
 * Outputs        : None
 * Returns        : None
 * Description    : This will intialize the SPS SpsTTIContextMap Information
 ******************************************************************************/
void processULResourceReserveRequest(ULUEContext *ueContext_p, LP_MacULSpsResReq macSpsResReq_p,
                                     /*CA Changes start  */
                                     UInt8 ulDelay, InternalCellIndex internalCellIndex)
                                     /*CA Changes end  */
{
    UInt8  index           = 0;
    UInt8  requiredRBCount = 0;
    UInt8  spsPeriodicity  = 0;
    UInt8  ttiIndex        = 0;
    UInt32 maxTBSize = 0;
    LP_MacULSpsStatus dataReq_p  = PNULL;
    LP_SpsUlInfo      spsUlInfo_p  = PNULL;
    LP_SpsULStrIntervalTTISorted  spsTTIInfoArr_p = PNULL;            
    LP_SpsULStrIntervalTTIContext ttiContext_p  = PNULL;
    LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;
    AllocatedResInfo resourceReserved={0};
    
    UInt16 spsInterval = 
        ueContext_p-> spsUlInfo.spsUlSetupInfo_p->semiPersistentSchedIntervalUL;
    RbMapNode *rbMapNode_p = PNULL;
    RbMapNode *rbMapNode1_p = PNULL; 
    RbMapNode *rbMapNode2_p = PNULL;
    RbMapNode rbMapNodeUnion =  {0};
    MCSToPrbMapInfo *mcsToPrbMap_p = ueContext_p->csiContext.mcsToPrbMap_p;
    
    UInt8 current_tti_index = 0;
    UInt8 tti_index1 = 0;
    UInt8 tti_index2 = 0;
    UInt8 target_tti = 0;
    /* SPR 15909 fix start */
    tickType_t currentGlobalTick =  getCurrentTick();
     /* SPR 15909 fix end */
    UInt8 hiIndex = 0;
    UInt8 numofTicksToCheck = 0; 
    UInt8 bundleTick = 0;

    /* SPR 15909 fix start */
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */
    /* CA Changes start */
    ULSchedulerStrategyConfig *ulSchdConfig_p = & cellSpecificParams_g.\
                      cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig;
    /* CA Changes end */
    /*!
     * \code
     * X is the effective wideband MCS for determining number of RBs
     * \endcode
     * */
    SInt8 mcsIndex = ueContext_p->mcsIndex;
    /*CA Changes start  */
    if(mcsIndex < ulSchdConfig_p->spsUlIntervalProfile[spsInterval].ulMinMcsSps \
            + ulSchdConfig_p->spsUlIntervalProfile[spsInterval].ulMcsMargin)
    {
        SPS_PUSCH_ENQUEUE_FAILURE_STATUS(internalCellIndex) ;
        return;
    }
    spsUlInfo_p = &ueContext_p->spsUlInfo; 
    mcsIndex = SPS_MIN(15,SPS_MAX(ulSchdConfig_p->spsUlIntervalProfile[spsInterval].ulMinMcsSps ,
                mcsIndex - ulSchdConfig_p->spsUlIntervalProfile[spsInterval].ulMcsMargin));
    /*CA Changes end  */

    if (SPS_RES_RSV_REQ_INITIATED == ueContext_p->spsUlInfo.spsState)
    {
        maxTBSize = macSpsResReq_p->avgRlcSduSize;
    }

    /* + SPS PC */
    /*CA Changes start  */
    for(; mcsIndex >= ulSchdConfig_p->spsUlIntervalProfile[spsInterval].ulMinMcsSps; mcsIndex--)
    /*CA Changes end  */
    {
        /* SPR 5999 changes start */
        /*CA Changes start  */
        requiredRBCount = DETERMINE_RBs_UL(mcsIndex, maxTBSize, internalCellIndex);
        /*CA Changes end  */
        /* SPR 5999 changes end */
        requiredRBCount = getUplinkMaxRBsInPowerOfTwoThreeFive(requiredRBCount);

        /*CA Changes start  */
        if ( MAC_FAILURE ==  validateNprbFromATBForSps(requiredRBCount,mcsIndex, ueContext_p, 0, internalCellIndex))
        /*CA Changes end  */
        {
            continue;
        }
        else
        {
            break;
        }
    }

    /*CA Changes start  */
    if (mcsIndex < ulSchdConfig_p->spsUlIntervalProfile[spsInterval].ulMinMcsSps)
    {
        SPS_PUSCH_ENQUEUE_FAILURE_STATUS(internalCellIndex) ;
        return;
    }
    /*CA Changes end  */
    /* - SPS PC */
    spsPeriodicity = spsUlInfo_p->spsUlSetupInfo_p->\
                     semiPersistentSchedIntervalUL;
    /*CA Changes start  */
    spsTTIInfoArr_p = ulSortedArr_gp[internalCellIndex];
    /*CA Changes end  */
    /* value of numofTicksToCheck = 4 in case of UE is TTI Bundled 
        else numofTicksToCheck = 1 */
    if(ueContext_p->ttiBundlingEnabled) 
    {
        numofTicksToCheck = TTI_BUNDLE_SIZE;
    }
    else
    {
        numofTicksToCheck = 1;
    }
    if( SPS_INTERVAL_40 ==  spsIntervalMap_g[spsPeriodicity] )
    {

        for( ttiIndex = 0; ttiIndex < SPS_INTERVAL_40; ttiIndex++ ) 
        {
            if(MAC_CHECK_NEXT_TICK == getAvailableRbsFromMaps(ttiIndex,
                        /* +- SPR 17777 */
                        ueContext_p,
                        numofTicksToCheck,
                        requiredRBCount,
                        &rbMapNodeUnion,
                        spsIntervalMap_g[spsPeriodicity],
                        /*CA Changes start  */
                        internalCellIndex))
                        /*CA Changes end  */
            {
                continue;
            }
            processUnionRbMapNode( &rbMapNodeUnion);
            /*CA Changes start  */
            if (FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.\
                cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->freqSelectiveSchUL)
            /*CA Changes end  */
            {
                resourceReserved.allocatedIndex = checkAvailableRBsToReturnLongestSet(
                        &mcsToPrbMap_p[mcsIndex], 
                        requiredRBCount, \
                        &resourceReserved.allocatedLength, 
                        &rbMapNodeUnion.availableResourceInfo[0],
                        rbMapNodeUnion.rbMap
                        );
                if (resourceReserved.allocatedLength >= requiredRBCount)
                {
                    for(bundleTick = 0; bundleTick < numofTicksToCheck ; bundleTick++)
                    {
                        rbMapNode_p =  &(spsULStrTTIMap_gp[internalCellIndex] + ((spsTTIInfoArr_p[ttiIndex].index +  
                                bundleTick) % MAX_SPS_INTERVAL))->rbMapNode;
                        updateRbMapNode(&resourceReserved,rbMapNode_p);
                    }
                    break;
                }
                else
                {
                    resourceReserved.allocatedIndex = 0;
                }
            }
            else 
            {
                /* calculate HI Index */
                current_tti_index = (currentGlobalTickWithULDelay) %  MAX_SPS_INTERVAL;
                tti_index1 = spsTTIInfoArr_p[ttiIndex].index;

                if (current_tti_index >= tti_index1)
                {
#ifdef FDD_CONFIG                        
                    hiIndex = ( ( (MAX_SPS_INTERVAL - (current_tti_index- tti_index1)) + FDD_DATA_OFFSET +  
                                FDD_HARQ_OFFSET + currentGlobalTickWithULDelay ) % MAX_PHICH_CONTAINER );
#endif
                }
                else
                {
#ifdef FDD_CONFIG                        
                    hiIndex = ( ( (tti_index1-current_tti_index) + FDD_DATA_OFFSET + 
                                   FDD_HARQ_OFFSET + currentGlobalTickWithULDelay ) % MAX_PHICH_CONTAINER );
#endif
                }
                if(ueContext_p->ttiBundlingEnabled)
                {
                    hiIndex = (hiIndex + TTIB_FIRST_TO_LAST_TICK_OFFSET) % MAX_PHICH_CONTAINER;
                }
                /*CA Changes start  */
                markHiResourceForAllSpsUEs(hiIndex, tti_index1, internalCellIndex);
                /*CA Changes end  */

                if( MAC_SUCCESS == checkSpsUplinkResources(requiredRBCount, requiredRBCount, &resourceReserved,
                            RB_DEFINETELY_REQD, &rbMapNodeUnion.availableResourceInfo[0], 
                            /*CA Changes start  */
                            rbMapNodeUnion.rbMap, hiIndex, internalCellIndex ))
                            /*CA Changes end  */
                { 
                    for(bundleTick = 0; bundleTick < numofTicksToCheck ; bundleTick++)
                    {
                        rbMapNode_p =  &(spsULStrTTIMap_gp[internalCellIndex] + ((spsTTIInfoArr_p[ttiIndex].index + 
                                bundleTick) % MAX_SPS_INTERVAL))->rbMapNode; 
                        updateRbMapNode(&resourceReserved,rbMapNode_p);
                    }
                    break;

                }

            }    /* Non frequency slective*/
        }/* for Loop ttiIndex */
    }/* If SPS_INTERVAL_40 */
    else if( SPS_INTERVAL_20 == spsIntervalMap_g[spsPeriodicity] )
    {
        for( ttiIndex = 0; ttiIndex <  SPS_INTERVAL_20; ttiIndex++ ) 
        {           
            if(MAC_CHECK_NEXT_TICK == getAvailableRbsFromMaps(ttiIndex,
                        /* +- SPR 17777 */
                        ueContext_p,
                        numofTicksToCheck,
                        requiredRBCount,
                        &rbMapNodeUnion,
                        spsIntervalMap_g[spsPeriodicity],
                        /*CA Changes start  */
                        internalCellIndex))
                        /*CA Changes end  */
            {
                continue;
            }
            rbMapNode1_p =  &(spsULStrTTIMap_gp[internalCellIndex] + (spsTTIInfoArr_p[ttiIndex].index))->rbMapNode; 
            rbMapNode2_p =  &(spsULStrTTIMap_gp[internalCellIndex] + ((spsTTIInfoArr_p[ttiIndex].index + SPS_INTERVAL_20) % MAX_SPS_INTERVAL))->rbMapNode;


            processUnionRbMapNode( &rbMapNodeUnion);

            /* Calculate HI index */
            current_tti_index = (currentGlobalTickWithULDelay) %  MAX_SPS_INTERVAL;
            tti_index1 = spsTTIInfoArr_p[ttiIndex].index  %  MAX_SPS_INTERVAL;
            tti_index2 = (spsTTIInfoArr_p[ttiIndex].index+20)  %  MAX_SPS_INTERVAL;

            hiIndex = getHIIndexFor20Periodicity(currentGlobalTick, current_tti_index, tti_index1, tti_index2, ulDelay, &target_tti);
            if(ueContext_p->ttiBundlingEnabled)
            {
                hiIndex = (hiIndex + TTIB_FIRST_TO_LAST_TICK_OFFSET) % MAX_PHICH_CONTAINER;
            }
            /*CA Changes start  */
            markHiResourceForAllSpsUEs(hiIndex, target_tti, internalCellIndex);
            /*CA Changes end  */

            /*CA Changes start  */
            if (FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->freqSelectiveSchUL)
            /*CA Changes end  */
            {
                resourceReserved.allocatedIndex = checkAvailableRBsToReturnLongestSet(
                        &mcsToPrbMap_p[mcsIndex], 
                        requiredRBCount, \
                        &resourceReserved.allocatedLength, 
                        &rbMapNodeUnion.availableResourceInfo[0],
                        rbMapNodeUnion.rbMap
                        );  
                if (resourceReserved.allocatedLength >= requiredRBCount)
                {
                    for(bundleTick = 0; bundleTick < numofTicksToCheck ; bundleTick++)
                    {
                        rbMapNode1_p =  &(spsULStrTTIMap_gp[internalCellIndex] + ((spsTTIInfoArr_p[ttiIndex 
                                ].index  + bundleTick) % SPS_INTERVAL_20))->rbMapNode; 
                        rbMapNode2_p =  &(spsULStrTTIMap_gp[internalCellIndex] + ((spsTTIInfoArr_p[ttiIndex 
                                ].index  + bundleTick + SPS_INTERVAL_20) % MAX_SPS_INTERVAL))->rbMapNode;

                        updateRbMapsFromUnionRbMap(&resourceReserved, rbMapNode1_p, rbMapNode2_p );
                    }
                    break;

                }
            }
            else 
            {
                if( MAC_SUCCESS == checkSpsUplinkResources(requiredRBCount, requiredRBCount, &resourceReserved,
                            RB_DEFINETELY_REQD, &rbMapNodeUnion.availableResourceInfo[0], 
                            /*CA Changes start  */
                            rbMapNodeUnion.rbMap, hiIndex, internalCellIndex))
                            /*CA Changes end  */
                {    
                    for(bundleTick = 0; bundleTick < numofTicksToCheck ; bundleTick++)
                    {
                        rbMapNode1_p =  &(spsULStrTTIMap_gp[internalCellIndex] + ((spsTTIInfoArr_p[ttiIndex 
                                ].index + bundleTick) % MAX_SPS_INTERVAL))->rbMapNode; 
                        rbMapNode2_p =  &(spsULStrTTIMap_gp[internalCellIndex] + ((spsTTIInfoArr_p[ttiIndex 
                                ].index  + bundleTick + SPS_INTERVAL_20) % MAX_SPS_INTERVAL))->rbMapNode;

                        updateRbMapsFromUnionRbMap(&resourceReserved, rbMapNode1_p, rbMapNode2_p );
                    }
                    break;
                }
            }    
        }
    }    
    /*!
     * \code
     * RM Failure:Put entry in dlSPSStatusQ with PDSCH fail.
     * \endcode
     * */
    if (!resourceReserved.allocatedLength 
    /* + Coverity 21504 */        
        || (SPS_INTERVAL_40 <= ttiIndex) )
    /* - Coverity 21504 */        
    {
        /*CA Changes start  */
        SPS_PUSCH_ENQUEUE_FAILURE_STATUS(internalCellIndex);
        /*CA Changes end  */
        return;
    }
    /*!
     * \code
     * Update spsTTIInfoArr_p[ttiIndex].spsRat2Localized with bestVRBSpanInfo such that a Free VRB knows
     * the next Free VRB Index and also previous Free VRB Index
     * \endcode
     * */

    dataReq_p = spsULAllocateStatusDataReq( macSpsResReq_p->ueIndex,
            SPS_PDSCH_PUSCH_SUCCESS, MAC_SPS_RES_RSV_REQ_ACT, maxTBSize, requiredRBCount);
/* Klockwork Warning Changes Start */
    if ( PNULL == dataReq_p  )
    {
        /* +- SPR 17777 */
        LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacULSpsStatus), 0,0,0,
                0.0,0.0,__FILE__,__func__);
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s",__FUNCTION__);
        return ;
    }
/* Klockwork Warning Changes End */
    /* Fill TTI Index for activation */
    dataReq_p->index = spsTTIInfoArr_p[ttiIndex].index;
    spsUlInfo_p->ulResAllocInfo.allocatedMCS = mcsIndex;
    spsUlInfo_p->ulResAllocInfo.startingVRBIndex = resourceReserved.allocatedIndex;
    spsUlInfo_p->ulResAllocInfo.allocatedRB = resourceReserved.allocatedLength;          
    spsUlInfo_p->ulResAllocInfo.allocatedRiv = resourceReserved.allocatedRiv;         
    spsUlInfo_p->actTtiIndex = dataReq_p->index;         
    for(bundleTick = 0; bundleTick < numofTicksToCheck ; bundleTick++)
    {
        ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + ((spsTTIInfoArr_p[ttiIndex].index + bundleTick)% MAX_SPS_INTERVAL);

        ttiContext_p->numULSpsRbsPerTti += resourceReserved.allocatedLength;
        ttiContext_p->numULSpsRbsPerTtiPerInterval[spsPeriodicity]+=resourceReserved.allocatedLength;
    } 
    /*! \code
     * RM Success:Put entry in dlSPSStatusQ with PDSCH success. 
     * Output:ttiIndex,MCS,RBs
     * \endcode
     * */
    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsInterval] )
    { 

        index = dataReq_p->index;

        for(bundleTick = 0; bundleTick < numofTicksToCheck ; bundleTick++)
        {
            ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + 
                ((index + bundleTick + SPS_INTERVAL_20)% MAX_SPS_INTERVAL);
            /* Update key */
            /*CA Changes start  */
            ((ulSortedArr_gp[internalCellIndex] + ttiContextPeriodicity20_p->indexInSortedArray)->\
             numSpsUeinRm)++;
            /*CA Changes end  */
            ttiContextPeriodicity20_p->numULSpsRbsPerTti += resourceReserved.allocatedLength;
            ttiContextPeriodicity20_p->numULSpsRbsPerTtiPerInterval[spsPeriodicity]+=resourceReserved.allocatedLength;
        }
    }
    for(bundleTick = 0; bundleTick < numofTicksToCheck ; bundleTick++)
    {
        ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + ((spsTTIInfoArr_p[ttiIndex ].index  + bundleTick) % MAX_SPS_INTERVAL);

        /* Coverity_73572 Fix Start */
#ifdef SSI_DEBUG_STATS
        ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + (index + bundleTick + SPS_INTERVAL_20);
#endif
        /* Coverity_73572 Fix End */

        /* Update RM stats */
        /* CA Stats Changes Start */
        LTE_MAC_UPDATE_SPS_PUSCH_TTI_MAP_STATS( spsUlInfo_p, ttiContext_p,
                ttiContextPeriodicity20_p, getCurrentTick(), internalCellIndex);
        /* CA Stats Changes End */

        /* Update key */
        /*CA Changes start  */
        ((ulSortedArr_gp[internalCellIndex] + ttiContext_p->indexInSortedArray)->numSpsUeinRm)++;

        /* Sort periodicity TTI map */   
        spsUlSort( ulSortedArr_gp[internalCellIndex], ttiContext_p->indexInSortedArray, 
                /*CA Changes start  */
                SPS_INTERVAL_40, internalCellIndex);
                /*CA Changes end  */
    } 
    /* + Coverity Id - 32616 */
    if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex], dataReq_p ))
    {
        freeMemPool(dataReq_p);
    }
    /* - Coverity Id - 32616 */
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( UL_SPS_STATUS_Q, 
            QCOUNT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex] ), 
            internalCellIndex); 
    /* CA Stats Changes End */
    /*CA Changes end  */
    /* Free the memory */
    freeMemPool ( macSpsResReq_p );
}

#else

/******************************************************************************
 * Function Name  : processULResourceReserveRequest
 * Inputs         : ueContext_p - UL UEContext 
 *                  macSpsResReq_p. Node filled by UL SPS Strategy for SPS-PDSCH
 *                  ulDelay -
 *                  internalCellIndex - Cell-Index at MAc
                    subFrameNum - DL Subframe to mark HI Resources
 * Outputs        : None
 * Returns        : None
 * Description    : This will intialize the SPS SpsTTIContextMap Information
 ******************************************************************************/
 /* CA Changes start */
/* + SPS_TDD_Changes */
void processULResourceReserveRequest(ULUEContext *ueContext_p, LP_MacULSpsResReq macSpsResReq_p,
                                     UInt8 ulDelay ,InternalCellIndex internalCellIndex,
                                     UInt8 subFrameNum)
{
    UInt8  index           = 0;
    UInt8  requiredRBCount = 0;
    UInt8  spsPeriodicity  = 0;
    UInt8  rbMax1 = 0;
    UInt8  rbMax2 = 0;
    UInt8  ttiIndex        = 0;
    SInt8 subFrame_offset = 0;
    UInt32 maxTBSize = 0;
    LP_MacULSpsStatus dataReq_p  = PNULL;
    LP_SpsUlInfo      spsUlInfo_p  = PNULL;
    LP_SpsULStrIntervalTTISorted  spsTTIInfoArr_p = PNULL;            
    LP_SpsULStrIntervalTTIContext ttiContext_p  = PNULL;
    LP_SpsULStrIntervalTTIContext ttiContextNextPeriodicity_p  = PNULL;
    LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;
    AllocatedResInfo resourceReserved={0};

    UInt16 spsInterval = 
        ueContext_p->spsUlInfo.spsUlSetupInfo_p->semiPersistentSchedIntervalUL;
    UInt8 idx = 0;
    RbMapNode *rbMapNode1_p = PNULL; 
    RbMapNode *rbMapNode2_p = PNULL;
    RbMapNode *rbMapNode3_p = PNULL;
    RbMapNode *rbMapNode4_p = PNULL;
    RbMapNode rbMapNodeUnion =  {0};
    MCSToPrbMapInfo *mcsToPrbMap_p = ueContext_p->csiContext.mcsToPrbMap_p;
    UInt8 current_tti_index = 0;
    UInt8 tti_index1 = 0;
    UInt8 tti_index2 = 0;
    UInt8 target_tti1 = 0;
    UInt8 target_tti2 = 0;
    UInt8 target_tti = 0;
    /* SPR 15909 fix start */
    tickType_t currentGlobalTick =  getCurrentTick();
    tickType_t currentGlobalTickWithULDelay = currentGlobalTick + ulDelay;
    /* SPR 15909 fix end */
    ULSchedulerStrategyConfig *ulSchdConfig_p = & cellSpecificParams_g.\
                      cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig;
    UInt8 freqSelectiveSchUL = cellSpecificParams_g.\
                cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->freqSelectiveSchUL;

    /*TDD Config 0 Changes Start*/
    /* Calculate iPhich and phichframe for the given U subframe*/
    UInt8 prbStartIndex = 0;
    UInt8 phichSubFrameNum = 0;
    UInt8 iPhich  =  0;
    UInt8 nDmrs =    MAX_DMRS_VALUE;
    phichSubFrameNum = (subFrameNum + getHiPduSubframeNum(subFrameNum,internalCellIndex)) % MAX_SUB_FRAME;
    iPhich = calculateIPhichValue(subFrameNum,internalCellIndex);
    /*TDD Config 0 Changes End*/

    /*!
     * \code
     * X is the effective wideband MCS for determining number of RBs
     * \endcode
     * */
    SInt8 mcsIndex = ueContext_p->mcsIndex;
    if(mcsIndex < ulSchdConfig_p->spsUlIntervalProfile[spsInterval].ulMinMcsSps
            + ulSchdConfig_p->spsUlIntervalProfile[spsInterval].ulMcsMargin)
    {
        /*CA Changes start  */
        SPS_PUSCH_ENQUEUE_FAILURE_STATUS(internalCellIndex);
        /*CA Changes end  */
        return;
    }
    spsUlInfo_p = &ueContext_p->spsUlInfo; 
    mcsIndex = SPS_MIN(15,SPS_MAX(ulSchdConfig_p->spsUlIntervalProfile[spsInterval].ulMinMcsSps ,
                mcsIndex - ulSchdConfig_p->spsUlIntervalProfile[spsInterval].ulMcsMargin));

    if (SPS_RES_RSV_REQ_INITIATED == ueContext_p->spsUlInfo.spsState)
    {
        maxTBSize = macSpsResReq_p->avgRlcSduSize;
    }
    else
    {
    }

    /* + SPS PC */
    for(; mcsIndex >= ulSchdConfig_p->spsUlIntervalProfile[spsInterval].ulMinMcsSps; mcsIndex--)
    {
        /* SPR 5999 changes start */
        requiredRBCount = DETERMINE_RBs_UL(mcsIndex, maxTBSize, internalCellIndex);
        /* SPR 5999 changes end */
        requiredRBCount = getUplinkMaxRBsInPowerOfTwoThreeFive(requiredRBCount);

        if ( MAC_FAILURE ==  validateNprbFromATBForSps(requiredRBCount,mcsIndex, ueContext_p, 0, internalCellIndex))
        {
            continue;
        }
        else
        {
            break;
        }
    }

    if (mcsIndex < ulSchdConfig_p->spsUlIntervalProfile[spsInterval].ulMinMcsSps)
    {
        /*CA Changes end  */
        SPS_PUSCH_ENQUEUE_FAILURE_STATUS(internalCellIndex) ;
        /*CA Changes end  */
        return;
    }
    /* - SPS PC */
    spsPeriodicity = spsUlInfo_p->spsUlSetupInfo_p->\
                     semiPersistentSchedIntervalUL;
    /*CA Changes start  */
    spsTTIInfoArr_p = ulSortedArr_gp[internalCellIndex];
    /*CA Changes end  */
    if( SPS_INTERVAL_40 ==  spsIntervalMap_g[spsPeriodicity] )
    {
        for( ttiIndex = 0; ttiIndex < SPS_INTERVAL_80; ttiIndex++ ) 
        {
            /* Resetting subframe offset to avoid use of incorrect value */
            subFrame_offset = 0;
            index = spsTTIInfoArr_p[ttiIndex].index;
            /*CA Changes start  */
            if( (UL != (spsULStrTTIMap_gp[internalCellIndex] + index)->sfType) || 
                    ( (spsULStrTTIMap_gp[internalCellIndex] + spsTTIInfoArr_p[ttiIndex].index)->\
                        numULSpsDciPerTti == (macReconfigSchedulerParamsUL_gp[internalCellIndex] + \
                        schParamsIndexUL_g[internalCellIndex])->\
                        spsUlSchInfoPerTti.maxUlSpsDciPerTti) ||
                    ( ( requiredRBCount + (spsULStrTTIMap_gp[internalCellIndex] + \
                        index)->numULSpsRbsPerTti ) >
                      (macReconfigSchedulerParamsUL_gp[internalCellIndex] + \
                        schParamsIndexUL_g[internalCellIndex])->\
                        spsUlSchInfoPerTti.maxUlSpsRbsPerTti) ||
                    ( ( requiredRBCount + (spsULStrTTIMap_gp[internalCellIndex] +
                        index)-> numULSpsRbsPerTtiPerInterval[spsInterval] ) >
                      (macReconfigSchedulerParamsUL_gp[internalCellIndex] + \
                        schParamsIndexUL_g[internalCellIndex])->\
                        spsUlSchInfoPerTti.maxUlSpsRbsPerTtiPerInterval[
                        spsInterval] ) || 
                    ( ( ( (spsULStrTTIMap_gp[internalCellIndex] + index)->numULSpsOccasionPerTti + \
                    (spsULStrTTIMap_gp[internalCellIndex] + index)->numULSpsDciPerTti) >=\
                    ( (macReconfigSchedulerParamsUL_gp[internalCellIndex] + \
                    schParamsIndexUL_g[internalCellIndex])->\
                    spsUlSchInfoPerTti.maxUlSpsOccasionPerTti) ) ) ) 
                /*CA Changes end  */
            {
                continue;
            }
            /* If twoIntervalConfig is enabled consider the subFrame offset for corresponding SubFrame*/
            if(TRUE == spsUlInfo_p->spsUlSetupInfo_p->twoIntervalsConfig)
            {
                subFrame_offset = getSubFrameOffsetForSpsTDD((index % MAX_SUBFRAME),internalCellIndex ) ;
            }
            if(MAC_FAILURE == checkDRXAndMeasGapForTtiIndex(index,
                        subFrame_offset,internalCellIndex,
                        spsUlInfo_p))
            {
                continue;
            }
            /* - SPS_TDD_Changes */
            /* Check if Rb's are available at all the indexes of resourceMap for 20 ms Periodicity */
            rbMapNode1_p =  
                &(spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode;

            rbMapNode2_p =  &(spsULStrTTIMap_gp[internalCellIndex] + ((index + \
                SPS_INTERVAL_40 + subFrame_offset) % MAX_UL_SPS_INTERVAL))->rbMapNode;

            rbMapNodeUnion.availableResourceInfo[0].freeRBCounter = SPS_MAX(rbMapNode1_p->availableResourceInfo[0].freeRBCounter ,
                    rbMapNode2_p->availableResourceInfo[0].freeRBCounter);

            rbMapNodeUnion.availableResourceInfo[0].freeRBIndex = SPS_MAX(rbMapNode1_p->availableResourceInfo[0].freeRBIndex ,
                    rbMapNode2_p->availableResourceInfo[0].freeRBIndex);

            rbMapNodeUnion.availableResourceInfo[0].endIndex = rbMapNode1_p->availableResourceInfo[0].endIndex;   
            rbMapNodeUnion.prachFlag = (BoolRetType) (rbMapNode1_p->prachFlag || rbMapNode2_p->prachFlag) ;

            /* Find Union of both RB map */
            for(idx = rbMapNodeUnion.availableResourceInfo[0].freeRBIndex; idx < rbMapNodeUnion.availableResourceInfo[0].endIndex; idx++)
            {
                rbMapNodeUnion.rbMap[idx] = rbMapNode1_p->rbMap[idx] || rbMapNode2_p->rbMap[idx]; 
            }

            processUnionRbMapNode( &rbMapNodeUnion);

            if (FREQUENCY_SELECTIVE_ON == freqSelectiveSchUL)
            {
                resourceReserved.allocatedIndex = checkAvailableRBsToReturnLongestSet(
                        &mcsToPrbMap_p[mcsIndex], 
                        requiredRBCount, \
                        &resourceReserved.allocatedLength, 
                        &rbMapNodeUnion.availableResourceInfo[0],
                        rbMapNodeUnion.rbMap,
                        &nDmrs,
                        phichSubFrameNum,
                        iPhich,
                        &prbStartIndex,
                        internalCellIndex
                        );  
                if (resourceReserved.allocatedLength >= requiredRBCount)
                {
                    updateRbMapsFromUnionRbMap(&resourceReserved, rbMapNode1_p, rbMapNode2_p );
                    break;
                }   
            }
            else 
            {
                /* calculate HI Index */
                current_tti_index = (currentGlobalTick + ulDelay) %  MAX_UL_SPS_INTERVAL;
                tti_index1 = index ;
                tti_index2 = (index + subFrame_offset + 40)  %  MAX_UL_SPS_INTERVAL;

                getTargetTTIIndexForTDD( current_tti_index, tti_index1, tti_index2, &target_tti);
                markHiResourceForAllSpsUEs(subFrameNum, target_tti, internalCellIndex);

                if( MAC_SUCCESS == checkSpsUplinkResources(requiredRBCount, requiredRBCount, &resourceReserved,
                            RB_DEFINETELY_REQD, &rbMapNodeUnion.availableResourceInfo[0], 
                            /*CA Changes start  */
                            rbMapNodeUnion.rbMap, subFrameNum, internalCellIndex ))
                    /*CA Changes end  */
                {    
                    updateRbMapsFromUnionRbMap(&resourceReserved, rbMapNode1_p, rbMapNode2_p );
                    break;
                }

            }    
        }
    }
    else if( SPS_INTERVAL_20 == spsIntervalMap_g[spsPeriodicity] )
    {
        for( ttiIndex = 0; ttiIndex <  SPS_INTERVAL_80; ttiIndex++ ) 
        {                
            /* Resetting subframe offset to avoid use of incorrect value */
            subFrame_offset = 0;
            index = spsTTIInfoArr_p[ttiIndex].index;
            /*CA Changes start  */
            if( (UL != (spsULStrTTIMap_gp[internalCellIndex] + index)->sfType) || 
                    ( (spsULStrTTIMap_gp[internalCellIndex] + spsTTIInfoArr_p[ttiIndex].index)->\
                        numULSpsDciPerTti == (macReconfigSchedulerParamsUL_gp[internalCellIndex] + \
                        schParamsIndexUL_g[internalCellIndex])->\
                        spsUlSchInfoPerTti.maxUlSpsDciPerTti) ||
                    ( ( requiredRBCount + (spsULStrTTIMap_gp[internalCellIndex] + \
                        index)->numULSpsRbsPerTti ) >
                      (macReconfigSchedulerParamsUL_gp[internalCellIndex] + \
                        schParamsIndexUL_g[internalCellIndex])->\
                        spsUlSchInfoPerTti.maxUlSpsRbsPerTti) ||
                    ( ( requiredRBCount + (spsULStrTTIMap_gp[internalCellIndex] +
                        index)-> numULSpsRbsPerTtiPerInterval[spsInterval] ) >
                      (macReconfigSchedulerParamsUL_gp[internalCellIndex] + \
                        schParamsIndexUL_g[internalCellIndex])->\
                        spsUlSchInfoPerTti.maxUlSpsRbsPerTtiPerInterval[
                        spsInterval] ) || 
                    ( ( ( (spsULStrTTIMap_gp[internalCellIndex] + index)->numULSpsOccasionPerTti + \
                    (spsULStrTTIMap_gp[internalCellIndex] + index)->numULSpsDciPerTti) >=\
                    ( (macReconfigSchedulerParamsUL_gp[internalCellIndex] + \
                    schParamsIndexUL_g[internalCellIndex])->\
                    spsUlSchInfoPerTti.maxUlSpsOccasionPerTti) ) ) ) 
                    /*CA Changes end  */
            {
                continue;
            }

            /* If twoIntervalConfig is enabled consider the subFrame offset for corresponding SubFrame*/
            if(TRUE == spsUlInfo_p->spsUlSetupInfo_p->twoIntervalsConfig)
            {
                subFrame_offset = getSubFrameOffsetForSpsTDD(((index + SPS_INTERVAL_20 )  % MAX_SUBFRAME),internalCellIndex)  ;
            }
            if(MAC_FAILURE == checkDRXAndMeasGapForTtiIndex(index,
                        subFrame_offset,internalCellIndex,
                        spsUlInfo_p))
            {
                continue;
            }
            /* Check if Rb's are available at all the indexes of resourceMap for 20 ms Periodicity */
            rbMapNode1_p =  &(spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode; 
            rbMapNode2_p =  &(spsULStrTTIMap_gp[internalCellIndex] + ((index + SPS_INTERVAL_20) % MAX_UL_SPS_INTERVAL))->rbMapNode;

            rbMapNode3_p =  &(spsULStrTTIMap_gp[internalCellIndex] + ((index + SPS_INTERVAL_40) % MAX_UL_SPS_INTERVAL))->rbMapNode;

            rbMapNode4_p =  &(spsULStrTTIMap_gp[internalCellIndex] + \
                    ((index + SPS_INTERVAL_20 + SPS_INTERVAL_40 + \
                    subFrame_offset) % MAX_UL_SPS_INTERVAL))->rbMapNode;

            rbMax1 = SPS_MAX(rbMapNode1_p->availableResourceInfo[0].freeRBCounter ,
                    rbMapNode2_p->availableResourceInfo[0].freeRBCounter);
            rbMax2 = SPS_MAX(rbMapNode3_p->availableResourceInfo[0].freeRBCounter ,
                    rbMapNode4_p->availableResourceInfo[0].freeRBCounter);
            rbMapNodeUnion.availableResourceInfo[0].freeRBCounter = SPS_MAX(rbMax1, rbMax2);

            rbMax1 = SPS_MAX(rbMapNode1_p->availableResourceInfo[0].freeRBIndex ,
                    rbMapNode2_p->availableResourceInfo[0].freeRBIndex);
            rbMax2 = SPS_MAX(rbMapNode3_p->availableResourceInfo[0].freeRBIndex,
                    rbMapNode4_p->availableResourceInfo[0].freeRBIndex);
            rbMapNodeUnion.availableResourceInfo[0].freeRBIndex= SPS_MAX(rbMax1, rbMax2);

            /* To decide which endIndex shall be used */ 
            rbMapNodeUnion.availableResourceInfo[0].endIndex = rbMapNode1_p->availableResourceInfo[0].endIndex;   

            rbMapNodeUnion.prachFlag = (BoolRetType) (rbMapNode1_p->prachFlag || rbMapNode2_p->prachFlag 
                    || rbMapNode3_p->prachFlag || rbMapNode4_p->prachFlag);

            /* Find Union of all RB map */
            for(idx = rbMapNodeUnion.availableResourceInfo[0].freeRBIndex; idx < rbMapNodeUnion.availableResourceInfo[0].endIndex; idx++)
            {
                rbMapNodeUnion.rbMap[idx] = rbMapNode1_p->rbMap[idx] || rbMapNode2_p->rbMap[idx] 
                    || rbMapNode3_p->rbMap[idx] || rbMapNode4_p->rbMap[idx];
            }

            rbMapNodeUnion.availableResourceInfo[0].endIndex = rbMapNode1_p->availableResourceInfo[0].endIndex;    
            /* + coverity 55807 */
            rbMapNodeUnion.prachFlag = (BoolRetType) (rbMapNode1_p->prachFlag || rbMapNode2_p->prachFlag);
            /* - coverity 55807 */

            /* Find Union of both RB map */
            for(idx = rbMapNodeUnion.availableResourceInfo[0].freeRBIndex; idx < rbMapNodeUnion.availableResourceInfo[0].endIndex; idx++)
            {
                rbMapNodeUnion.rbMap[idx] = rbMapNode1_p->rbMap[idx] || rbMapNode2_p->rbMap[idx];
            }

            processUnionRbMapNode( &rbMapNodeUnion);

            /* Calculate Target TTI Index for 20 ms periodicity  */
            current_tti_index = (currentGlobalTickWithULDelay) %  MAX_UL_SPS_INTERVAL;
            tti_index1 = index  ;
            tti_index2 = (index + 20 + subFrame_offset)  %  MAX_UL_SPS_INTERVAL;
            getTargetTTIIndexForTDD( current_tti_index, tti_index1, tti_index2, &target_tti1);

            tti_index1 = (index + 40 ) %  MAX_UL_SPS_INTERVAL;
            tti_index2 = (index + 60 + subFrame_offset) %  MAX_UL_SPS_INTERVAL;
            getTargetTTIIndexForTDD( current_tti_index, tti_index1, tti_index2, &target_tti2);

            getTargetTTIIndexForTDD( current_tti_index, target_tti1, target_tti2, &target_tti);
            /* Reserve HI Rsources */
            markHiResourceForAllSpsUEs(subFrameNum, target_tti, internalCellIndex);

            if (FREQUENCY_SELECTIVE_ON == freqSelectiveSchUL)
            {
                resourceReserved.allocatedIndex = checkAvailableRBsToReturnLongestSet(
                        &mcsToPrbMap_p[mcsIndex], 
                        requiredRBCount, \
                        &resourceReserved.allocatedLength, 
                        &rbMapNodeUnion.availableResourceInfo[0],
                        rbMapNodeUnion.rbMap,
                        &nDmrs,
                        phichSubFrameNum,
                        iPhich,
                        &prbStartIndex,
                        internalCellIndex
                        );  
                if (resourceReserved.allocatedLength >= requiredRBCount)
                {
                    updateRbMapsFromUnionRbMap(&resourceReserved, rbMapNode1_p, rbMapNode2_p );
                    updateRbMapsFromUnionRbMap(&resourceReserved, rbMapNode3_p, rbMapNode4_p );
                    break;
                }   
            }
            else 
            {
                if( MAC_SUCCESS == checkSpsUplinkResources(requiredRBCount, requiredRBCount, &resourceReserved,
                            RB_DEFINETELY_REQD, &rbMapNodeUnion.availableResourceInfo[0], 
                            /*CA Changes start  */
                            rbMapNodeUnion.rbMap, subFrameNum, internalCellIndex))
                    /*CA Changes end  */
                {    
                    updateRbMapsFromUnionRbMap(&resourceReserved, rbMapNode1_p, rbMapNode2_p );
                    break;
                }

            }    
        }
    }    
    /*!
     * \code
     * RM Failure:Put entry in dlSPSStatusQ with PDSCH fail.
     * \endcode
     * */
    if ((!resourceReserved.allocatedLength)
            /* + Coverity 21504 */        
            || (SPS_INTERVAL_80 <= ttiIndex) )
        /* - Coverity 21504 */        
    {
        /*CA Changes end  */
        SPS_PUSCH_ENQUEUE_FAILURE_STATUS(internalCellIndex);
        /*CA Changes end  */
        return;
    }
    /*!
     * \code
     * Update spsTTIInfoArr_p[ttiIndex].spsRat2Localized with bestVRBSpanInfo such that a Free VRB knows
     * the next Free VRB Index and also previous Free VRB Index
     * \endcode
     * */

    dataReq_p = spsULAllocateStatusDataReq( macSpsResReq_p->ueIndex,
            SPS_PDSCH_PUSCH_SUCCESS, MAC_SPS_RES_RSV_REQ_ACT, maxTBSize, requiredRBCount);

    /* coverity 32560 28June */
    if ( PNULL == dataReq_p  )
    {
        /* +- SPR 17777 */
        LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacULSpsStatus), 0,0,0,
                0.0,0.0,__FILE__,__func__);
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s",__FUNCTION__);
        return ;
    }
    /* coverity 32560 28June */

    /* Fill TTI Index for activation */
    dataReq_p->index = index;

    ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + index;
    if(SPS_INTERVAL_40 == spsIntervalMap_g[spsInterval])
    {
        ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + \
             ((index + SPS_INTERVAL_40 + subFrame_offset)% MAX_UL_SPS_INTERVAL);
    }
    else
    {
        ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + \
             ((index + SPS_INTERVAL_40)% MAX_UL_SPS_INTERVAL);
    }

    spsUlInfo_p->ulResAllocInfo.allocatedMCS = mcsIndex;
    spsUlInfo_p->ulResAllocInfo.startingVRBIndex = resourceReserved.allocatedIndex;
    spsUlInfo_p->ulResAllocInfo.allocatedRB = resourceReserved.allocatedLength;          
    spsUlInfo_p->ulResAllocInfo.allocatedRiv = resourceReserved.allocatedRiv;         
    spsUlInfo_p->actTtiIndex = dataReq_p->index;         

    ttiContext_p->numULSpsRbsPerTti += resourceReserved.allocatedLength;
    ttiContext_p->numULSpsRbsPerTtiPerInterval[spsPeriodicity]+=resourceReserved.allocatedLength;

    ttiContextNextPeriodicity_p->numULSpsRbsPerTti += resourceReserved.allocatedLength;
    ttiContextNextPeriodicity_p->numULSpsRbsPerTtiPerInterval[spsPeriodicity]+=resourceReserved.allocatedLength;

    /* Update key */
    ((ulSortedArr_gp[internalCellIndex] + ttiContext_p->indexInSortedArray)->\
     numSpsUeinRm)++;
    ((ulSortedArr_gp[internalCellIndex] + ttiContextNextPeriodicity_p->indexInSortedArray)->\
     numSpsUeinRm)++;
    /*! \code
     * RM Success:Put entry in dlSPSStatusQ with PDSCH success. 
     * Output:ttiIndex,MCS,RBs
     * \endcode
     * */
    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsInterval] )
    { 
        index = dataReq_p->index;

        ttiContextNextPeriodicity_p = PNULL; 

        ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + \
            ((index + subFrame_offset + SPS_INTERVAL_20)% MAX_UL_SPS_INTERVAL);
        ttiContextNextPeriodicity_p = spsULStrTTIMap_gp[internalCellIndex] + \
            ((index + subFrame_offset + SPS_INTERVAL_20 + \
            SPS_INTERVAL_40)% MAX_UL_SPS_INTERVAL);

        ttiContextPeriodicity20_p->numULSpsRbsPerTti += resourceReserved.allocatedLength;
        ttiContextPeriodicity20_p->numULSpsRbsPerTtiPerInterval[spsPeriodicity]+=resourceReserved.allocatedLength;

        ttiContextNextPeriodicity_p->numULSpsRbsPerTti += resourceReserved.allocatedLength;
        ttiContextNextPeriodicity_p->numULSpsRbsPerTtiPerInterval[spsPeriodicity]+=resourceReserved.allocatedLength;

        /* Update key */
        /*CA Changes start  */
        ((ulSortedArr_gp[internalCellIndex] + ttiContextPeriodicity20_p->indexInSortedArray)->\
         numSpsUeinRm)++;
        ((ulSortedArr_gp[internalCellIndex] + ttiContextNextPeriodicity_p->indexInSortedArray)->\
         numSpsUeinRm)++;
        /*CA Changes end  */
    }
    /* CA Stats Changes Start */
    /* Update RM stats */
    LTE_MAC_UPDATE_SPS_PUSCH_TTI_MAP_STATS( spsUlInfo_p, ttiContext_p,
            ttiContextPeriodicity20_p, getCurrentTick(), internalCellIndex);
    /* CA Stats Changes End */
    /* Sort periodicity TTI map */   
    spsUlSort( ulSortedArr_gp[internalCellIndex], ttiContext_p->indexInSortedArray, 
            /*CA Changes start  */
            MAX_UL_SPS_INTERVAL, internalCellIndex);
    /*CA Changes end  */

    /* coverity 63545 28June2014 */ 
    if (!ENQUEUE_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex], dataReq_p )) 
    {
        freeMemPool(dataReq_p); 
        return; 
    } 
    /* coverity 63545 28June2014 */ 
    /* CA Stats Changes Start */
    LTE_MAC_QUEUE_USED( UL_SPS_STATUS_Q, 
            QCOUNT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex] ), 
            internalCellIndex); 
    /* CA Stats Changes End */
    /*CA Changes end  */
    /* Free the memory */
    freeMemPool ( macSpsResReq_p );
    macSpsResReq_p = PNULL;

}
/* - SPS_TDD_Changes */

 /* CA Changes end */
/** TTIB_Code Ends */
#endif

/******************************************************************************
 * Function Name  : processUnionRbMapNode 
 * Inputs         : rbMapNodeUnion_p - Pointer to RB MAP node
 * Outputs        : rbMapNodeUnion_p
 * Returns        : None
 * Description    : This function will Update the RB Map start index 
 ******************************************************************************/
void processUnionRbMapNode(RbMapNode* rbMapNodeUnion_p)
{
    UInt8 idx=0;
    UInt8 alloc_start_idx=0;
    UInt8 flag=0;

    for (idx = rbMapNodeUnion_p->availableResourceInfo[0].freeRBIndex; 
               idx < rbMapNodeUnion_p->availableResourceInfo[0].endIndex ;idx++)
    {
        if (rbMapNodeUnion_p->rbMap[idx] != 0 && flag==FALSE)
        {
            alloc_start_idx = idx;
            flag = TRUE;
        } 
        else if (rbMapNodeUnion_p->rbMap[idx] == 0 && flag==TRUE)
        {
            rbMapNodeUnion_p->rbMap[alloc_start_idx] = idx - alloc_start_idx;
            flag = FALSE;
        }    
        
    }    
}    
/******************************************************************************
 * Function Name  : updateRbMapNode 
 * Inputs         : resourceReserved_p - pointer to the Resources allocated
 * Outputs        : rbMapNode_p - pointer to RB MAp node 
 * Returns        : None
 * Description    : This will Update the RB Map node of which RB's are allocated
 *                  from RB map
 ******************************************************************************/
void updateRbMapNode(AllocatedResInfo* resourceReserved_p, RbMapNode* rbMapNode_p)
{
    UInt8 idx=0;
    /* Mark Rbs to allocated RBs */
    for(idx = resourceReserved_p->allocatedIndex; idx < resourceReserved_p->allocatedIndex + resourceReserved_p->allocatedLength; idx++)
    {
        rbMapNode_p->rbMap[idx] = BUSY_RESOURCE;
    } 
    rbMapNode_p->rbMap[resourceReserved_p->allocatedIndex] = resourceReserved_p->allocatedLength;
    rbMapNode_p->availableResourceInfo[0].freeRBCounter -= resourceReserved_p->allocatedLength;
    
   if (rbMapNode_p->availableResourceInfo[0].freeRBIndex == resourceReserved_p->allocatedIndex)
   {
       rbMapNode_p->availableResourceInfo[0].freeRBIndex += resourceReserved_p->allocatedLength;
   }
}    

/******************************************************************************
 * Function Name  : updateRbMapsFromUnionRbMap 
 * Inputs         : resourceReserved_p - pointer to the Resources allocated
 * Outputs        : rbMapNode1_p,rbMapNode2_p - pointer to RB MAp node 
 * Returns        : None
 * Description    : This will Update the RB Map node of which RB's are allocated
 *                  from Union RB MAP
 ******************************************************************************/
void updateRbMapsFromUnionRbMap(AllocatedResInfo* resourceReserved_p, RbMapNode* rbMapNode1_p, RbMapNode* rbMapNode2_p)
{
    UInt8 idx=0;
    /* Mark Rbs to allocated RBs */
    for(idx = resourceReserved_p->allocatedIndex; idx < resourceReserved_p->allocatedIndex + resourceReserved_p->allocatedLength; idx++)
    {
        rbMapNode1_p->rbMap[idx] = BUSY_RESOURCE;
        rbMapNode2_p->rbMap[idx] = BUSY_RESOURCE;
    } 
    
    
    rbMapNode1_p->rbMap[resourceReserved_p->allocatedIndex] = resourceReserved_p->allocatedLength;
    rbMapNode2_p->rbMap[resourceReserved_p->allocatedIndex] = resourceReserved_p->allocatedLength;

    rbMapNode1_p->availableResourceInfo[0].freeRBCounter -= resourceReserved_p->allocatedLength;
    rbMapNode2_p->availableResourceInfo[0].freeRBCounter -= resourceReserved_p->allocatedLength;

   if (rbMapNode1_p->availableResourceInfo[0].freeRBIndex == resourceReserved_p->allocatedIndex)
   {
       rbMapNode1_p->availableResourceInfo[0].freeRBIndex += resourceReserved_p->allocatedLength; 
   }
   if (rbMapNode2_p->availableResourceInfo[0].freeRBIndex == resourceReserved_p->allocatedIndex)
   {
       rbMapNode2_p->availableResourceInfo[0].freeRBIndex += resourceReserved_p->allocatedLength; 
   }
   
}    
    
/*****************************************************************************
 * Function Name  : checkSpsUplinkResources
 * Inputs         : numOfResourcesReq - Number of resources to be reserved.
 *                  minReqdRBs - minimum required number of RB's
 *                  allocatedResInfo_p - pointer to AlocatedResInfo and fill 
 *                                       the parameter of this structure.
 *                  strictAllocatedFlag - RB_DEFINETELY_REQD or RB_REQUIRED
 *                  availRes_p -
 *                  rbMap - RB Map Number
 *                  hiIndex - 
 *                  internalCellIndex - Cell-Index at MAc
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function allocates the resources for PUSCH.
 *****************************************************************************/

MacRetType checkSpsUplinkResources( UInt32 numOfResourcesReq,
    UInt32 minReqdRBs,
    AllocatedResInfo *allocatedResInfo_p,
    UInt32 strictAllocatedFlag,
    AvailableResInfo *availRes_p,
    UInt8 *rbMap,
    UInt8 hiIndex,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex
    /*CA Changes end  */
    )
{
  LTE_MAC_UT_LOG(LOG_INFO,UL_RESOURCE_ALLOCATOR,
      "[%s] RbMapNum[%d] NumOfResReq[%d] minNumOfRes[%d] "
      " strictAllocatedFlag[%d]Entry. ", __func__, *rbMap, 
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

  /* Its point to the freeRBIndex */
  startIndex  = availRes_p->freeRBIndex;

  LTE_MAC_UT_LOG(LOG_INFO, UL_RESOURCE_ALLOCATOR,
      "[%s] RbMapNum[%d] StartIndex [%d] endIndex [%d]. ",
      __func__, *rbMap, startIndex, availRes_p->endIndex);

  idx = startIndex;
  /*CA Changes start  */
  while ( ( (FALSE == checkPhichGrpAndPhichSeq(idx, hiIndex, internalCellIndex)) ||( FREE_RESOURCE != rbMap[idx]) ) && (idx <= availRes_p->endIndex)  )
  /*CA Changes end  */
  {
      idx++;
  }   
  /*klockworks wanring fix*/
  /*Coverity_Warning_Fix_Start*/
  while ( idx <= availRes_p->endIndex && idx < MAX_NUM_RBS)
  /*Coverity_Warning_Fix_End*/    
  {
    if ( FREE_RESOURCE == rbMap[idx])
    {
      if ( newFlag )
      {
        newFlag = FALSE;
        tempLength = 1;
 
        /*CA Changes start  */
        while ( ( (FALSE == checkPhichGrpAndPhichSeq(idx, hiIndex, internalCellIndex)) ||( FREE_RESOURCE != rbMap[idx]) ) && (idx <= availRes_p->endIndex)  )
        /*CA Changes end  */
        {    
            idx++;
        }    
        if (idx > availRes_p->endIndex) 
        {
            break;
        }    

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
            __func__,*rbMap,tempStartPoint,tempStartPoint+tempLength-1);

            break;
      }
      idx++;
    }
    else
    {
      idx +=  rbMap[idx];
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

  resInfo_p->allocatedLength = localLength;
  resInfo_p->allocatedIndex  = localIndex ;
  /*CA Changes start  */
  resInfo_p->allocatedRiv    = getRIV(localIndex,localLength, internalCellIndex);
  /*CA Changes end  */

  /* Mark HI Index */
  /*CA Changes start  */
  markPhichGrpAndPhichSeq (localIndex, hiIndex, internalCellIndex);
  /*CA Changes end  */
  
  return MAC_SUCCESS;
}
/******************************************************************************
 * Function Name  : initULSpsTTIContextMap
 * Inputs         : rmSize - Size of Resource Map which is LCM of sps 
 *                  peridocities. For 20ms & 40ms support, RM size=80
 *                  internalCellIndex - Cell-Index at MAc
 * Outputs        : None
 * Returns        : None
 * Description    : This will intialize the SPS SpsTTIContextMap Information
 ******************************************************************************/
/* CA Changes start  */
void initULSpsTTIContextMap(UInt32 rmSize, InternalCellIndex internalCellIndex)
{
    UInt8  ctr   = 0;    
    UInt32 index = 0;

    /* The mem pool will be created only first time when cell was configured */ 
    /* + SPS_TDD_CHANGES */
#ifdef FDD_CONFIG
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        createMemPool(sizeof(SpsULStrIntervalTTIContext) * MAX_SPS_INTERVAL, 1);

        createMemPool(sizeof(SpsULStrIntervalTTISorted) * MAX_SPS_INTERVAL, 1);
    }
    /* SPR 10604 FIX start */
    GET_MEM_FROM_POOL(SpsULStrIntervalTTIContext, spsULStrTTIMap_gp[internalCellIndex],
            sizeof(SpsULStrIntervalTTIContext) * MAX_SPS_INTERVAL, PNULL);
    GET_MEM_FROM_POOL(SpsULStrIntervalTTISorted, ulSortedArr_gp[internalCellIndex],
            sizeof(SpsULStrIntervalTTISorted) * MAX_SPS_INTERVAL, PNULL);
#else
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        createMemPool(sizeof(SpsULStrIntervalTTIContext) * MAX_UL_SPS_INTERVAL, 1);

        createMemPool(sizeof(SpsULStrIntervalTTISorted) * MAX_UL_SPS_INTERVAL, 1);
    }
    /* SPR 10604 FIX start */
    GET_MEM_FROM_POOL(SpsULStrIntervalTTIContext, spsULStrTTIMap_gp[internalCellIndex],
            sizeof(SpsULStrIntervalTTIContext) * MAX_UL_SPS_INTERVAL, PNULL);
    GET_MEM_FROM_POOL(SpsULStrIntervalTTISorted, ulSortedArr_gp[internalCellIndex],
            sizeof(SpsULStrIntervalTTISorted) * MAX_UL_SPS_INTERVAL, PNULL);
#endif
    /* - SPS_TDD_CHANGES */

    for (index = 0; index < rmSize; index++ )
    {
        (spsULStrTTIMap_gp[internalCellIndex] + index)->index = index;
        (spsULStrTTIMap_gp[internalCellIndex] + index)->numULSpsDciPerTti=0;
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        (spsULStrTTIMap_gp[internalCellIndex] + index)->sfType = 
            getSubframeType((index % MAX_SUB_FRAME),internalCellIndex);
#endif
        /* - SPS_TDD_Changes */
        for( ctr = 0; ctr < SPS_INTERVAL_MAX; ctr++ )
        {
            (spsULStrTTIMap_gp[internalCellIndex] + index)->numULSpsRbsPerTtiPerInterval[ctr] = 0;
        }
        (spsULStrTTIMap_gp[internalCellIndex] + index)->numULSpsOccasionPerTti = 0;        
        (spsULStrTTIMap_gp[internalCellIndex] + index)->numULSpsRbsPerTti=0;
#ifdef FDD_CONFIG
        (spsULStrTTIMap_gp[internalCellIndex] + index)->numULSpsTTIBUePerTti=0;
#endif
        (spsULStrTTIMap_gp[internalCellIndex] +index)->indexInSortedArray = index;
        initSpsRbMapNode( &(spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode,
                /*  ulRBs*/ cellSpecificParams_g.cellConfigAndInitParams_p\
                [internalCellIndex]->cellParams_p->ulAvailableRBs);
        QINIT_MAC_SPS_UE_LIST_PER_TTI((spsULStrTTIMap_gp[internalCellIndex] + index)->spsULDciList);
        QINIT_MAC_SPS_UE_LIST_PER_TTI((spsULStrTTIMap_gp[internalCellIndex] + index)->spsULOccasionList);
        /* SPR 10604 FIX End */

        /* Initialize sorted arrays for 20 and 40 ms periodicity */
        (ulSortedArr_gp[internalCellIndex] + index)->index=index;
        (ulSortedArr_gp[internalCellIndex] + index)->numSpsUeinRm = 0;
    }
    /* Reserve PUCCH and PRACH resources */
    reservePRACHPUCCHRBInRM(rmSize, internalCellIndex);
}
/*CA Changes end  */
/*<!-CSR 51542 CHANGE START-!>*/
/******************************************************************************
 * Function Name  : deInitAfterCellDeleteUL 
 * Inputs         : rmSize
 *                  internalCellIndex - Cell-Index at MAc
 * Outputs        : None
 * Returns        : None
 * Description    : This will release the semaphore resources 
 ******************************************************************************/
void deInitAfterCellDeleteUL(UInt32 rmSize, InternalCellIndex internalCellIndex)
{
    UInt32 ctr = 0;
    for(ctr =0 ;ctr < rmSize;ctr++)
    {
        /*SPR 16877 Fix Start*/
        semDestroy((LTE_SEM *)&((spsULStrTTIMap_gp[internalCellIndex] + ctr)->spsULDciList).sem_lock);
        semDestroy((LTE_SEM *)&((spsULStrTTIMap_gp[internalCellIndex] + ctr)->spsULOccasionList).sem_lock);
        /*SPR 16877 Fix End*/
    }

    if (spsULStrTTIMap_gp[internalCellIndex])
    {
        freeMemPool(spsULStrTTIMap_gp[internalCellIndex]);
        spsULStrTTIMap_gp[internalCellIndex] = PNULL;
    }

    if (ulSortedArr_gp[internalCellIndex])
    {
        freeMemPool(ulSortedArr_gp[internalCellIndex]);
        ulSortedArr_gp[internalCellIndex] = PNULL;
    }
    return;
}
/*<!-CSR 51542 CHANGE END-!>*/
#ifdef FDD_CONFIG
/** TTIB_Code Starts */
/******************************************************************************
 * Function Name  : processULResourceReleaseRequest 
 * Inputs         : ueContext_p - DL UEContext 
 *                  reqType - request type of release(TTIB/SPS) 
 *                  internalCellIndex - Cell-Index at MAc
 * Outputs        : None
 * Returns        : None
 * Description    : This will release the  UL SPS  resources
 ******************************************************************************/
void processULResourceReleaseRequest(ULUEContext *ueContext_p,
                                     UInt8 reqType,
                                     /*CA Changes start  */
                                     InternalCellIndex internalCellIndex
                                     /*CA Changes end  */
                                     )
{
    LTE_MAC_UT_LOG(LOG_INFO, LOG_PUSCH,
            "[%s] [%d] reqType %d \n.",__func__,getCurrentTick(),reqType);
    /*!
     * \code
     * Find out start VRBIndex & end VRBIndex of this UE which need to be freed.
     * \endcode
     * */
    UInt8                       index                     = 0;
    LP_SpsUlInfo      spsUlInfo_p  = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContext_p              = PNULL;
    LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;
    UInt8 startVRBIndex = ueContext_p->spsUlInfo.ulResAllocInfo.startingVRBIndex;
    UInt8 endVRBIndex = startVRBIndex + ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB -1;
    UInt8 spsPeriodicity = 0;
    UInt8 numOfTicksToRel = 0; 
    UInt8 bundleTick = 0;
    spsUlInfo_p = &ueContext_p->spsUlInfo;
    spsPeriodicity = 
        ueContext_p->spsUlInfo.spsUlSetupInfo_p->semiPersistentSchedIntervalUL;
    /*!
     * \code
     * Get SpsStrIntervalTTIContext from spsActivationIndex
     * \endcode
     * */
    /*req = MAC_SPS_TTIB_RES_REL_REQ ,in case if Sps context to be released
      was TTIB */

    /*Klockwork fix start*/
    if(endVRBIndex >= MAX_NUM_RBS+1)
    {
          return;
    } 
    /*Klockwork fix end*/

    if(MAC_SPS_TTIB_RES_REL_REQ == reqType || ueContext_p->ttiBundlingEnabled)
    {
        numOfTicksToRel = TTI_BUNDLE_SIZE;
    }
    else
    {
        numOfTicksToRel = 1;
    }
    UInt8 activationIndex = spsUlInfo_p->actTtiIndex;
    /*value of numOfTicksToRel = 4 in case of Ue is TTI Bundled else equals to 1 */
    for(bundleTick = 0; bundleTick < numOfTicksToRel ;bundleTick++)
    {

        ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + activationIndex;

        RbMapNode *rbMapNode_p = PNULL; 
        RbMapNode *rbMapNode20_p = PNULL; 

        (spsULStrTTIMap_gp[internalCellIndex] + activationIndex)->numULSpsRbsPerTti -= ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;
        (spsULStrTTIMap_gp[internalCellIndex] + activationIndex)->numULSpsRbsPerTtiPerInterval[spsPeriodicity] -= 
            ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;

        if( SPS_INTERVAL_20 == spsIntervalMap_g[spsPeriodicity] )
        { 
            (spsULStrTTIMap_gp[internalCellIndex] + ((activationIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL))->\
                numULSpsRbsPerTti -= 
                spsUlInfo_p->ulResAllocInfo.allocatedRB;

            (spsULStrTTIMap_gp[internalCellIndex] + ((activationIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL))->\
                numULSpsRbsPerTtiPerInterval[spsPeriodicity] -=
                spsUlInfo_p->ulResAllocInfo.allocatedRB;

            index = SPS_MAX( activationIndex, 
                    (activationIndex + SPS_INTERVAL_20) % MAX_SPS_INTERVAL);

            ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + index;
            ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] +
                ((index + SPS_INTERVAL_20) % MAX_SPS_INTERVAL);

            /* Update sorting key */
            /*CA Changes start  */
            ((ulSortedArr_gp[internalCellIndex] + ttiContextPeriodicity20_p->indexInSortedArray)->
             numSpsUeinRm)--;
            /*CA Changes end  */

            rbMapNode20_p = &ttiContextPeriodicity20_p->rbMapNode;
            rbMapNode_p = &ttiContext_p->rbMapNode;

            while(startVRBIndex <= endVRBIndex)
            {
                rbMapNode_p->rbMap[startVRBIndex] = FREE_RESOURCE;
                rbMapNode20_p->rbMap[startVRBIndex] = FREE_RESOURCE;
                startVRBIndex++;
            }
            rbMapNode_p->availableResourceInfo[0].freeRBCounter += ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;
            rbMapNode20_p->availableResourceInfo[0].freeRBCounter += ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;

            if (rbMapNode_p->availableResourceInfo[0].freeRBIndex == endVRBIndex +1)
            {
                rbMapNode_p->availableResourceInfo[0].freeRBIndex -= ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB; 
            }    
            if (rbMapNode20_p->availableResourceInfo[0].freeRBIndex == endVRBIndex +1)
            {
                rbMapNode20_p->availableResourceInfo[0].freeRBIndex -= ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB; 
            }    
        }
        else if( SPS_INTERVAL_40 == spsIntervalMap_g[spsPeriodicity] )
        {
            rbMapNode_p = &ttiContext_p->rbMapNode;

            while(startVRBIndex <= endVRBIndex)
            {
                rbMapNode_p->rbMap[startVRBIndex] = FREE_RESOURCE;
                startVRBIndex++;
            }
            rbMapNode_p->availableResourceInfo[0].freeRBCounter += ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;

            if (rbMapNode_p->availableResourceInfo[0].freeRBIndex == endVRBIndex +1)
            {
                rbMapNode_p->availableResourceInfo[0].freeRBIndex -= ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB; 
            }    
        }    

        /* CA Stats Changes Start */
        /* Update RM stats */
        LTE_MAC_UPDATE_SPS_PUSCH_TTI_MAP_STATS( spsUlInfo_p, ttiContext_p,
                ttiContextPeriodicity20_p, getCurrentTick(), internalCellIndex);
        /* CA Stats Changes End */

        /* Update sorting key */
        /*CA Changes start  */
        ((ulSortedArr_gp[internalCellIndex] + ttiContext_p->indexInSortedArray)->numSpsUeinRm)--;

        /* Sort periodicity TTI map */   
        spsUlSort( ulSortedArr_gp[internalCellIndex], 0, ttiContext_p->indexInSortedArray + 1, internalCellIndex);
        /*CA Changes end  */

        activationIndex++;
    }
    spsUlInfo_p->ulResAllocInfo.allocatedRB =0;
    spsUlInfo_p->actTtiIndex = MAX_SPS_INTERVAL;
}

#else
/******************************************************************************
 * Function Name  : processULResourceReleaseRequest 
 * Inputs         : ueContext_p - DL UEContext 
 *                  internalCellIndex - Cell-Index at MAc
 * Outputs        : None
 * Returns        : None
 * Description    : This will release the UL SPS  resources
 ******************************************************************************/
/* + SPS_TDD_Changes */
void processULResourceReleaseRequest(ULUEContext *ueContext_p, InternalCellIndex internalCellIndex)
{

    /*!
     * \code
     * Find out start VRBIndex & end VRBIndex of this UE which need to be freed.
     * \endcode
     * */
    UInt8                       nextTTIindex                     = 0;
    SInt8 subFrame_offset = 0;
    LP_SpsUlInfo      spsUlInfo_p  = PNULL;
    LP_SpsULStrIntervalTTIContext  ttiContext_p              = PNULL;
    LP_SpsULStrIntervalTTIContext ttiContextPeriodicity20_p = PNULL;
    LP_SpsULStrIntervalTTIContext ttiContextPeriodicity40_p = PNULL;
    LP_SpsULStrIntervalTTIContext ttiContextPeriodicity60_p = PNULL;
    UInt8 startVRBIndex = ueContext_p->spsUlInfo.ulResAllocInfo.startingVRBIndex;
    UInt8 endVRBIndex = startVRBIndex + ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB -1;
    UInt8 spsPeriodicty = 0;
    spsUlInfo_p = &ueContext_p->spsUlInfo;
    spsPeriodicty = 
        ueContext_p->spsUlInfo.spsUlSetupInfo_p->semiPersistentSchedIntervalUL;
    /*!
     * \code
     * Get SpsStrIntervalTTIContext from spsActivationIndex
     * \endcode
     * */
    UInt8 activationIndex = spsUlInfo_p->actTtiIndex;

    RbMapNode *rbMapNode_p = PNULL; 
    RbMapNode *rbMapNode20_p = PNULL; 
    RbMapNode *rbMapNode40_p = PNULL; 
    RbMapNode *rbMapNode60_p = PNULL; 
    /* If twoIntervalConfig is enabled consider the subFrame offset for corresponding SubFrame*/
    if(TRUE == spsUlInfo_p->spsUlSetupInfo_p->twoIntervalsConfig)
    {
        subFrame_offset = getSubFrameOffsetForSpsTDD((activationIndex % MAX_SUBFRAME),internalCellIndex ) ;
    }

    (spsULStrTTIMap_gp[internalCellIndex] + activationIndex)->numULSpsRbsPerTti -= 
        ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;

    (spsULStrTTIMap_gp[internalCellIndex] + activationIndex)->numULSpsRbsPerTtiPerInterval[spsPeriodicty] -= 
        ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;

    ttiContext_p = spsULStrTTIMap_gp[internalCellIndex] + activationIndex;

    if( SPS_INTERVAL_40 == spsIntervalMap_g[spsPeriodicty] )
    {
        nextTTIindex = (activationIndex + subFrame_offset + SPS_INTERVAL_40)% MAX_UL_SPS_INTERVAL;
    }
    else
    {
        nextTTIindex = (activationIndex +  SPS_INTERVAL_40)% MAX_UL_SPS_INTERVAL;
    }
    (spsULStrTTIMap_gp[internalCellIndex] + nextTTIindex)->numULSpsRbsPerTti -=
            ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;

    (spsULStrTTIMap_gp[internalCellIndex] + nextTTIindex)->numULSpsRbsPerTtiPerInterval[spsPeriodicty] -=
            ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;

    ttiContextPeriodicity40_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIindex;

    rbMapNode40_p = &ttiContextPeriodicity40_p->rbMapNode;
    rbMapNode_p = &ttiContext_p->rbMapNode;
    while(startVRBIndex <= endVRBIndex)
    {
        rbMapNode_p->rbMap[startVRBIndex] = FREE_RESOURCE;
        rbMapNode40_p->rbMap[startVRBIndex] = FREE_RESOURCE;
        startVRBIndex++;
    }
    rbMapNode_p->availableResourceInfo[0].freeRBCounter += ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;
    rbMapNode40_p->availableResourceInfo[0].freeRBCounter += ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;
    if (rbMapNode_p->availableResourceInfo[0].freeRBIndex == endVRBIndex +1)
    {
        rbMapNode_p->availableResourceInfo[0].freeRBIndex -= ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB; 
        startVRBIndex = ueContext_p->spsUlInfo.ulResAllocInfo.startingVRBIndex;
        while((startVRBIndex) && (FREE_RESOURCE == rbMapNode_p->rbMap[--startVRBIndex])) 
        {
            rbMapNode_p->availableResourceInfo[0].freeRBIndex--;
        }
    }    
    if (rbMapNode40_p->availableResourceInfo[0].freeRBIndex == endVRBIndex +1)
    {
        rbMapNode40_p->availableResourceInfo[0].freeRBIndex -= ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB; 
        startVRBIndex = ueContext_p->spsUlInfo.ulResAllocInfo.startingVRBIndex;
        while((startVRBIndex) && (FREE_RESOURCE == rbMapNode40_p->rbMap[--startVRBIndex])) 
        {
            rbMapNode40_p->availableResourceInfo[0].freeRBIndex--;
        }
    }    
    /* Update sorting key */
    ((ulSortedArr_gp[internalCellIndex] + ttiContext_p->indexInSortedArray)->numSpsUeinRm)--;
    ((ulSortedArr_gp[internalCellIndex] + ttiContextPeriodicity40_p->indexInSortedArray)->numSpsUeinRm)--;

    /*!
     * \code
     * Update SpsTTIBitMap of this spsActivationIndex with startVRBIndex &
     * endVRBIndex
     * \endcode
     * */
    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsPeriodicty] )
    {
        nextTTIindex = (activationIndex + subFrame_offset + SPS_INTERVAL_20)% MAX_UL_SPS_INTERVAL;

        (spsULStrTTIMap_gp[internalCellIndex] + nextTTIindex)->\
            numULSpsRbsPerTti -= 
            spsUlInfo_p->ulResAllocInfo.allocatedRB;

        (spsULStrTTIMap_gp[internalCellIndex] + nextTTIindex)->\
            numULSpsRbsPerTtiPerInterval[spsPeriodicty] -=
            spsUlInfo_p->ulResAllocInfo.allocatedRB;

        ttiContextPeriodicity20_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIindex;

        nextTTIindex = (activationIndex + subFrame_offset + SPS_INTERVAL_20 + SPS_INTERVAL_40) % MAX_UL_SPS_INTERVAL;

        (spsULStrTTIMap_gp[internalCellIndex] + nextTTIindex)->\
            numULSpsRbsPerTti -= 
            spsUlInfo_p->ulResAllocInfo.allocatedRB;

        (spsULStrTTIMap_gp[internalCellIndex] + nextTTIindex)->\
            numULSpsRbsPerTtiPerInterval[spsPeriodicty] -=
            spsUlInfo_p->ulResAllocInfo.allocatedRB;

        ttiContextPeriodicity60_p = spsULStrTTIMap_gp[internalCellIndex] + nextTTIindex;

        /* Update sorting key */
        /*CA Changes start  */
        ((ulSortedArr_gp[internalCellIndex] + ttiContextPeriodicity20_p->indexInSortedArray)->
         numSpsUeinRm)--;
        /*CA Changes end  */

        ((ulSortedArr_gp[internalCellIndex] + ttiContextPeriodicity60_p->indexInSortedArray)->
         numSpsUeinRm)--;
         
        rbMapNode60_p = &ttiContextPeriodicity60_p->rbMapNode;
        rbMapNode20_p = &ttiContextPeriodicity20_p->rbMapNode;

        startVRBIndex = ueContext_p->spsUlInfo.ulResAllocInfo.startingVRBIndex;
        while(startVRBIndex <= endVRBIndex)
        {
            rbMapNode20_p->rbMap[startVRBIndex] = FREE_RESOURCE;
            rbMapNode60_p->rbMap[startVRBIndex] = FREE_RESOURCE;
            startVRBIndex++;
        }
        rbMapNode20_p->availableResourceInfo[0].freeRBCounter += ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;
        rbMapNode60_p->availableResourceInfo[0].freeRBCounter += ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB;
        if (rbMapNode20_p->availableResourceInfo[0].freeRBIndex == endVRBIndex +1)
        {
            rbMapNode20_p->availableResourceInfo[0].freeRBIndex -= ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB; 
            startVRBIndex = ueContext_p->spsUlInfo.ulResAllocInfo.startingVRBIndex;
            while((startVRBIndex) && (FREE_RESOURCE == rbMapNode20_p->rbMap[--startVRBIndex])) 
            {
                rbMapNode20_p->availableResourceInfo[0].freeRBIndex--;
            }
        }    
        if (rbMapNode60_p->availableResourceInfo[0].freeRBIndex == endVRBIndex +1)
        {
            rbMapNode60_p->availableResourceInfo[0].freeRBIndex -= ueContext_p->spsUlInfo.ulResAllocInfo.allocatedRB; 
            startVRBIndex = ueContext_p->spsUlInfo.ulResAllocInfo.startingVRBIndex;
            while((startVRBIndex) && (FREE_RESOURCE == rbMapNode60_p->rbMap[--startVRBIndex])) 
            {
                rbMapNode60_p->availableResourceInfo[0].freeRBIndex--;
            }
        }    
    }
    spsUlInfo_p->ulResAllocInfo.allocatedRB =0;
    spsUlInfo_p->actTtiIndex = MAX_SPS_INTERVAL;

    /* CA Stats Changes Start */
    /* Update RM stats */
    LTE_MAC_UPDATE_SPS_PUSCH_TTI_MAP_STATS( spsUlInfo_p, ttiContext_p,
            ttiContextPeriodicity20_p, getCurrentTick(), internalCellIndex);
    /* CA Stats Changes End */

    /* Update sorting key */
    /*CA Changes start  */
    /* +- SPR 18510 Code Removed */

    /* Sort periodicity TTI map */   
    spsUlSort( ulSortedArr_gp[internalCellIndex], 0, ttiContext_p->indexInSortedArray + 1, internalCellIndex);
    /*CA Changes end  */
}
/* - SPS_TDD_Changes */
/** TTIB_Code Ends */
#endif

/*****************************************************************************
 * Function Name : spsULAllocateStatusDataReq
 * Inputs        : ueIndex - Index of UE
 *                  response - Strategy receives following type of responses from 
 *                    PDSCH or HARQ module:
 *                  1. Pdsch res reserv Success
 *                  2. Pdsch res reserv Failure (Mcs Poor)
 *                  3. Harq First Ack / NACK for Activation
 *                  4. HARQ DTX or timer expiry Information
 *                 requestType - Type of request for Reserve or release resources
 *                 spsOpp - Latest Opportunity provided to SPS LC
 *                 numRbs - RBs occupied
 * Returns       : LP_MacULSpsStatus 
 * Description   : To allocate and init LP_MacDlSpsStatus
***************************************************************************/
LP_MacULSpsStatus spsULAllocateStatusDataReq( UInt16 ueIndex, UInt8   response,
        UInt8    requestType, UInt32  spsOpp, UInt16   numRbs)
{
    LP_MacULSpsStatus dataReq_p = PNULL;
    /*!
     * \code
     *  Allocate data request structure 
     * \endcode
     * */
    dataReq_p = (LP_MacULSpsStatus)getMemFromPool( sizeof(MacULSpsStatus), PNULL);

    if ( PNULL == dataReq_p  )
    {
        /* +- SPR 17777 */
        LOG_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MacULSpsStatus), 0,0,0,
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
    /* + Coverity 55048 */
    dataReq_p->response = (SpsCause) response;
    /* - Coverity 55048 */
    dataReq_p->requestType = requestType;
    dataReq_p->spsOpp = spsOpp;
    dataReq_p->numRbs = numRbs;

    return dataReq_p;
}


/******************************************************************************
 * Function Name  : initSpsRbMapNode 
 * Inputs         : ulRBs
 * Outputs        : rbMapNode_p - pointer to RB map Node 
 * Returns        : None
 * Description    : This will initialise the SPS RB Map node
 ******************************************************************************/
void initSpsRbMapNode( RbMapNode* rbMapNode_p, UInt16 ulRBs)
{ 
   memSet(rbMapNode_p,0,sizeof(RbMapNode));
    
   rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter = ulRBs;
   rbMapNode_p->availableResourceInfo[CC_CE_REGION].endIndex =   ulRBs -1;   
   rbMapNode_p->pucchFlag = FALSE;   
        /*--coverity--530--54180*/
   rbMapNode_p->prachFlag = RET_FALSE;   
        /*--coverity--530--54180*/
}
/******************************************************************************
 * Function Name  : reservePRACHPUCCHRBInRM 
 * Inputs         : rmSize - Size of Resource Map which is LCM of sps 
 *                  peridocities. For 20ms & 40ms support, RM size=40
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None 
 * Returns        : None
 * Description    : This will reserve PRACH and PUCCH RB's in Resoure manager
 ******************************************************************************/
void  reservePRACHPUCCHRBInRM(UInt32 rmSize,InternalCellIndex internalCellIndex)
{
    UInt8  index = 0;
    UInt8  rbIndex = 0;
    UInt8 pucch_resource_overlap_count=0;
    /*CA Changes start  */
    UInt32 endIndex = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                      cellParams_p->ulAvailableRBs -1;
    UInt32 prachFrequencyOffset = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                                  rachConfigInfo.prachConfiguration[cellSpecificParams_g.\
                                  cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                                  rachConfigInfo.prachActiveIndex].prachFrequencyOffset;
    /*CA Changes end  */
    /*Klockwork fix start*/
    if(prachFrequencyOffset+NUM_PRACH_PHYSICAL_RESOURCES >= MAX_NUM_RBS+1)
    {
          return;
    } 
    /*Klockwork fix end*/

    
    for (index = 0; index < rmSize; index++ )
    {
        (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.availableResourceInfo[0].endIndex -= sps_max_pucch_resource_g;


        /* PRACH resource reservation */
        for (rbIndex=prachFrequencyOffset; rbIndex < prachFrequencyOffset + NUM_PRACH_PHYSICAL_RESOURCES; rbIndex++)
        {
          (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.rbMap[rbIndex] =NUM_PRACH_PHYSICAL_RESOURCES -(rbIndex- prachFrequencyOffset);
          if ( (rbIndex == 0) || (rbIndex == 1) || (rbIndex == 2) || 
               (rbIndex == endIndex) || (rbIndex == endIndex-1) || (rbIndex == endIndex-2) )
          {
              pucch_resource_overlap_count++;
          }
        }
        if (prachFrequencyOffset == 0)
        {
            (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.availableResourceInfo[0].freeRBIndex = NUM_PRACH_PHYSICAL_RESOURCES;
        }
        (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.availableResourceInfo[0].freeRBCounter-= NUM_PRACH_PHYSICAL_RESOURCES;
       
        /* PUCCH resource reservatioon */  
        for (rbIndex=0; rbIndex < sps_max_pucch_resource_g; rbIndex++)
        {
            (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.rbMap[rbIndex] = sps_max_pucch_resource_g - rbIndex;
            (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.rbMap[endIndex - rbIndex]= rbIndex+1;
        }
    
        if (sps_max_pucch_resource_g < prachFrequencyOffset)
        {   
            (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.availableResourceInfo[0].freeRBIndex = sps_max_pucch_resource_g;
        }
        else
        {
            (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.availableResourceInfo[0].freeRBIndex = 
                               NUM_PRACH_PHYSICAL_RESOURCES + sps_max_pucch_resource_g - pucch_resource_overlap_count;
        }
        
        if ( ( prachFrequencyOffset + NUM_PRACH_PHYSICAL_RESOURCES >=  endIndex - sps_max_pucch_resource_g ) &&
            /* + SPR 13605 Fix */
             /* Required for frequencies > 1.4 MHz only */
             ( cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs > 6))
            /* - SPR 13605 Fix */
        { 
            for(rbIndex = (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.availableResourceInfo[0].endIndex ; 
               rbIndex > (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.availableResourceInfo[0].endIndex-NUM_PRACH_PHYSICAL_RESOURCES;
               rbIndex--)
           {
               if(0 ==  (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.rbMap[rbIndex])
               {
                   break; 
               }
           }
   
            (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.availableResourceInfo[0].endIndex = rbIndex; 
        }
        (spsULStrTTIMap_gp[internalCellIndex] + index)->rbMapNode.availableResourceInfo[0].freeRBCounter -= 
                              2*sps_max_pucch_resource_g - pucch_resource_overlap_count;  
         
        pucch_resource_overlap_count=0;
    }
}


/******************************************************************************
 * Function Name  : markULSpsRBsInRBMap 
 * Inputs         : rbMapNum - index of RB Map 
 *                  spsArrayTti - TTi to find the UL sps interval pointer
 *                  locationFlag - location type to update the RB usage
 *                  currentGlobalTick,ulDelay
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None 
 * Returns        : None
 * Description    : This will update/mark the SPS RB's in RB map node.
 ******************************************************************************/
void markULSpsRBsInRBMap(UInt8 rbMapNum, UInt8 spsArrayTti, 
        /* SPR 15909 fix start */
        /* +- SPR 17777 */
        UInt8 locationFlag,
#ifdef FDD_CONFIG
        tickType_t currentGlobalTick, UInt8 ulDelay,  
#endif
        /* +- SPR 17777 */
        /* SPR 15909 fix end */
        /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
        UInt8 subFrame,
#endif
        /* - SPS_TDD_Changes */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    /* read all UE and then mark the resource*/ 
    SpsULStrIntervalTTIContext *spsUlStrIntervalTTI_p = spsULStrTTIMap_gp[internalCellIndex] + spsArrayTti;
    UInt8 sdlCount = spsUlStrIntervalTTI_p->numULSpsDciPerTti;
    UInt8  spsOccasionUeCount = spsUlStrIntervalTTI_p->numULSpsOccasionPerTti;
    LP_MacULSpsDciListTTI  sdlNode_p        = PNULL;
    LP_MacULSpsOccList  occasionNode_p    = PNULL;
    ULUEContext          *ueULContext_p   = PNULL;
    LP_SpsUlInfo         spsUlInfo_p      = PNULL;
    AllocatedResInfo ulResAllocInfo = {0};
    /*CA Changes start  */
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
   
    SInt8   temp = 0;
#ifdef FDD_CONFIG
    UInt8   hiIndex = 0;
    hiIndex = ( (currentGlobalTick + FDD_DATA_OFFSET +  FDD_HARQ_OFFSET + ulDelay) % MAX_PHICH_CONTAINER );
#endif

    if ( sdlCount > 0)
    { 
        GET_FIRST_MAC_SPS_UE_LIST_PER_TTI(spsUlStrIntervalTTI_p->spsULDciList, sdlNode_p);
        /* + coverity 25240 */
        while( PNULL != sdlNode_p && sdlCount-- )
        /* - coverity 25240 */
        {        
            ueULContext_p = ulUECtxInfoArr_g[sdlNode_p->ueIndex].ulUEContext_p;
            if( PNULL == ueULContext_p )
            {
                /* Get next node of queue */
                GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlStrIntervalTTI_p->spsULDciList,
                    sdlNode_p);
                continue;            
            }

            /*! \code
             * If UE has moved to Pending delete, Clean the SPS context 
             * \endcode
             * */
            if( 0 != ulUECtxInfoArr_g[sdlNode_p->ueIndex].pendingDeleteFlag )
            {
                /* Get next node of queue */
                GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlStrIntervalTTI_p->spsULDciList,
                    sdlNode_p);

                continue;
            }
            spsUlInfo_p = &(ueULContext_p->spsUlInfo);

            if( (SPS_ACTIVATION_INITIATED == spsUlInfo_p->spsState ) &&
                 (FALSE == spsUlInfo_p->draRBsUpdationFlag) )
            { 
                /* mark RBs */
                 MAC_UL_RESERVE_RESOURCES(ueULContext_p->spsUlInfo.ulResAllocInfo.startingVRBIndex,\
                                         ueULContext_p->spsUlInfo.ulResAllocInfo.allocatedRB, \
                                         &ulResAllocInfo,\
                                         &temp,\
                                         ueULContext_p->spsUlInfo.ulResAllocInfo.allocatedRB,\
                                         ueULContext_p->userLocationType,\
                                         rbMapNode_p,\
                                         rbMapNode_p->rbMap,
                                         internalCellIndex);


                 /* Mark HI resource  */
                 markPhichGrpAndPhichSeq (spsUlInfo_p->ulResAllocInfo.startingVRBIndex 
                         /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
                         ,hiIndex 
#elif TDD_CONFIG
                         ,subFrame
#endif
                         /* - SPS_TDD_Changes */
                         /*CA Changes start  */
                         ,internalCellIndex
                         ); 
                 /*CA Changes end  */

                 spsUlInfo_p->draRBsUpdationFlag = TRUE;
    
                if (NORMAL_LOCATION_UPDATE == locationFlag)
                {
                    spsUlInfo_p->draRBsUpdationFlag = FALSE;
                }

            }
            /* Get next node of queue */
            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlStrIntervalTTI_p->spsULDciList,
                    sdlNode_p);
        }
    }

#ifdef FDD_CONFIG
    spsOccasionUeCount = spsOccasionUeCount - spsUlStrIntervalTTI_p->numULSpsTTIBUePerTti;
#endif
    if (  spsOccasionUeCount > 0)
    { 
        GET_FIRST_MAC_SPS_UE_LIST_PER_TTI( 
                spsUlStrIntervalTTI_p->spsULOccasionList, occasionNode_p);

        /* + coverity 25240 */
        while( PNULL != occasionNode_p && spsOccasionUeCount-- )
        /* - coverity 25240 */
        {        
            ueULContext_p = ulUECtxInfoArr_g[occasionNode_p->ueIndex].ulUEContext_p;
            if( PNULL == ueULContext_p )
            {
                /* Get next node of queue */
                GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlStrIntervalTTI_p->spsULOccasionList,
                        occasionNode_p);
                continue;            
            }
            spsUlInfo_p = &(ueULContext_p->spsUlInfo);

            /*! \code
             * If UE has moved to Pending delete, Clean the SPS context 
             * \endcode
             * */
            if( 0 != ulUECtxInfoArr_g[occasionNode_p->ueIndex].pendingDeleteFlag )
            {
                /* Get next node of queue */
                GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlStrIntervalTTI_p->spsULOccasionList,
                        occasionNode_p);

                continue;
            }

            if( (SPS_ACTIVATED == spsUlInfo_p->spsState ) &&
                    (FALSE == spsUlInfo_p->draRBsUpdationFlag) )
            { 
                /* mark RBs */
                MAC_UL_RESERVE_RESOURCES(ueULContext_p->spsUlInfo.ulResAllocInfo.startingVRBIndex,\
                        ueULContext_p->spsUlInfo.ulResAllocInfo.allocatedRB, \
                        &ulResAllocInfo,\
                        &temp,\
                        ueULContext_p->spsUlInfo.ulResAllocInfo.allocatedRB,\
                        ueULContext_p->userLocationType,\
                        rbMapNode_p,\
                        rbMapNode_p->rbMap,
                        internalCellIndex);

                /* Mark HI resource  */
                markPhichGrpAndPhichSeq (spsUlInfo_p->ulResAllocInfo.startingVRBIndex
                        /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
                        ,hiIndex 
#elif TDD_CONFIG
                        ,subFrame
#endif
                        /* - SPS_TDD_Changes */
                        /*CA Changes start  */
                        ,internalCellIndex
                        ); 
                /*CA Changes end  */

                spsUlInfo_p->draRBsUpdationFlag = TRUE;
            }

            if (NORMAL_LOCATION_UPDATE == locationFlag) 
            {
                spsUlInfo_p->draRBsUpdationFlag = FALSE;
            }

            /* Get next node of queue */
            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlStrIntervalTTI_p->spsULOccasionList,
                    occasionNode_p);
        }
    }
}
/* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
/******************************************************************************
 * Function Name  : getHIIndexFor20Periodicity 
 * Inputs         : currentGlobalTick - current Tick in MAC
 *                  current_tti_index - current TTI index
 *                  tti_index1  - next tti of SPS occation
 *                  tti_index2  - next + 20 TTI of sps occasion
 *                  ulDelay -
 * Outputs        : target_tti_p - update TTI index where sps data sent 
 * Returns        : hiIndex 
 * Description    : This will update Target TTI index of SPS occation to be sent
 *                  and return the Harq index for that TTI
 ******************************************************************************/
                /* SPR 15909 fix start */
UInt8 getHIIndexFor20Periodicity(tickType_t currentGlobalTick, UInt8 current_tti_index, 
                /* SPR 15909 fix end */
              UInt8 tti_index1, UInt8 tti_index2, UInt8 ulDelay, UInt8 *target_tti_p)
{
    UInt8 hiIndex =0;

    if (tti_index1 < tti_index2)
    {
        if ( (current_tti_index < tti_index1) )
        {
            hiIndex = ( (currentGlobalTick + (tti_index1 - current_tti_index) + FDD_DATA_OFFSET +  
                        FDD_HARQ_OFFSET + ulDelay) % MAX_PHICH_CONTAINER );
            *target_tti_p = tti_index1; 
        }
        else if ((current_tti_index == tti_index1) )
        {
            hiIndex = ( (currentGlobalTick + (tti_index2 - current_tti_index) + FDD_DATA_OFFSET +  
                        FDD_HARQ_OFFSET + ulDelay) % MAX_PHICH_CONTAINER );


            *target_tti_p = tti_index2; 
        }    
        else if ((current_tti_index > tti_index1) && (current_tti_index < tti_index2) )
        {
            hiIndex = ( (currentGlobalTick + (tti_index2 - current_tti_index) + FDD_DATA_OFFSET +  
                        FDD_HARQ_OFFSET + ulDelay) % MAX_PHICH_CONTAINER );
            *target_tti_p = tti_index2; 
        }    
        else if ((current_tti_index >= tti_index2) )
        {
            hiIndex = ( (currentGlobalTick + (MAX_SPS_INTERVAL - (current_tti_index- tti_index1)) + FDD_DATA_OFFSET +  
                        FDD_HARQ_OFFSET + ulDelay) % MAX_PHICH_CONTAINER );

            *target_tti_p = tti_index1; 

        }    
    }
    else
    {
        if ( (current_tti_index < tti_index2) )
        {
            hiIndex = ( (currentGlobalTick + (tti_index2 - current_tti_index) + FDD_DATA_OFFSET +  
                        FDD_HARQ_OFFSET + ulDelay) % MAX_PHICH_CONTAINER );
            *target_tti_p = tti_index2; 
        }
        else if ((current_tti_index == tti_index2) )
        {
            hiIndex = ( (currentGlobalTick + (tti_index1 - current_tti_index) + FDD_DATA_OFFSET +  
                        FDD_HARQ_OFFSET + ulDelay) % MAX_PHICH_CONTAINER );
            *target_tti_p = tti_index1; 
        }    
        else if ((current_tti_index > tti_index2) && (current_tti_index < tti_index1) )
        {
            hiIndex = ( (currentGlobalTick + (tti_index1 - current_tti_index) + FDD_DATA_OFFSET +  
                        FDD_HARQ_OFFSET + ulDelay) % MAX_PHICH_CONTAINER );
            *target_tti_p = tti_index1; 
        }    
        else if ((current_tti_index >= tti_index1) )
        {
            hiIndex = ( (currentGlobalTick + (MAX_SPS_INTERVAL - (current_tti_index- tti_index2)) + FDD_DATA_OFFSET +  
                        FDD_HARQ_OFFSET + ulDelay) % MAX_PHICH_CONTAINER );
            *target_tti_p = tti_index2; 
        }    
    }
    return hiIndex;
}
#endif
/* - SPS_TDD_Changes */

/******************************************************************************
 * Function Name  : markHiResourceForAllSpsUEs 
 * Inputs         : hiIndex- harq index
 *                  target_tti - target TTI of SPS 
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None 
 * Returns        : None
 * Description    : This will reserve the  HI resources for all SPS UE's 
 ******************************************************************************/
void markHiResourceForAllSpsUEs(UInt8 hiIndex, UInt8 target_tti, InternalCellIndex internalCellIndex)
{
    /* read all UE and then mark the resource*/ 
    SpsULStrIntervalTTIContext *spsUlStrIntervalTTI_p = spsULStrTTIMap_gp[internalCellIndex] + target_tti;
    UInt8 sdlCount = spsUlStrIntervalTTI_p->numULSpsDciPerTti;
    UInt8  spsOccasionUeCount = spsUlStrIntervalTTI_p->numULSpsOccasionPerTti;
    LP_MacULSpsDciListTTI  sdlNode_p        = PNULL;
    LP_MacULSpsOccList  occasionNode_p    = PNULL;
    ULUEContext          *ueULContext_p   = PNULL;
    LP_SpsUlInfo         spsUlInfo_p      = PNULL;

    if ( sdlCount > 0)
    { 
        GET_FIRST_MAC_SPS_UE_LIST_PER_TTI(spsUlStrIntervalTTI_p->spsULDciList, sdlNode_p);
        /* + Coverity 25239 */
        while( PNULL != sdlNode_p && sdlCount-- )
        /* - Coverity 25239 */
        {        
            ueULContext_p = ulUECtxInfoArr_g[sdlNode_p->ueIndex].ulUEContext_p;
            if( PNULL == ueULContext_p )
            {
                /* Get next node of queue */
                GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlStrIntervalTTI_p->spsULDciList,
                        sdlNode_p);
                continue;            
            }

            /*! \code
             * If UE has moved to Pending delete, Clean the SPS context 
             * \endcode
             * */
            if( 0 != ulUECtxInfoArr_g[sdlNode_p->ueIndex].pendingDeleteFlag )
            {
                /* Get next node of queue */
                GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlStrIntervalTTI_p->spsULDciList,
                        sdlNode_p);

                continue;
            }
            spsUlInfo_p = &(ueULContext_p->spsUlInfo);

            if( SPS_ACTIVATION_INITIATED == spsUlInfo_p->spsState )
            { 
                /* Mark HI resource  */
                /*CA Changes start  */
                markPhichGrpAndPhichSeq (spsUlInfo_p->ulResAllocInfo.startingVRBIndex, hiIndex, internalCellIndex); 
                /*CA Changes end  */
            }
            /* Get next node of queue */
            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlStrIntervalTTI_p->spsULDciList,
                    sdlNode_p);
        }
    }

#ifdef FDD_CONFIG
    spsOccasionUeCount = spsOccasionUeCount - spsUlStrIntervalTTI_p->numULSpsTTIBUePerTti;
#endif
    if ( spsOccasionUeCount > 0 )
    { 
        GET_FIRST_MAC_SPS_UE_LIST_PER_TTI(
                spsUlStrIntervalTTI_p->spsULOccasionList, occasionNode_p);

        /* + coverity 25239 */
        while( PNULL != occasionNode_p && spsOccasionUeCount-- )
        /* - coverity 25239 */
        {        
            ueULContext_p = ulUECtxInfoArr_g[occasionNode_p->ueIndex].ulUEContext_p;
            if( PNULL == ueULContext_p )
            {
                /* Get next node of queue */
                GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlStrIntervalTTI_p->spsULOccasionList,
                        occasionNode_p);
                continue;            
            }
            spsUlInfo_p = &(ueULContext_p->spsUlInfo);

            /*! \code
             * If UE has moved to Pending delete, Clean the SPS context 
             * \endcode
             * */
            if( 0 != ulUECtxInfoArr_g[occasionNode_p->ueIndex].pendingDeleteFlag )
            {
                /* Get next node of queue */
                GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlStrIntervalTTI_p->spsULOccasionList,
                        occasionNode_p);

                continue;
            }

            if( SPS_ACTIVATED == spsUlInfo_p->spsState )
            { 
                /* Mark HI resource  */
                /*CA Changes start  */
                markPhichGrpAndPhichSeq (spsUlInfo_p->ulResAllocInfo.startingVRBIndex, hiIndex, internalCellIndex); 
                /*CA Changes end  */
            }
            /* Get next node of queue */
            GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( spsUlStrIntervalTTI_p->spsULOccasionList,
                    occasionNode_p);
        }
    }
}
/* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
/******************************************************************************
 * Function Name  : getTargetTTIIndexForTDD 
 * Inputs         : tti_index1 - 1st tti Index 
 *                  tti_index2 = 2nd tti Index
 *                  current_tti_index - tti Index corresponding to current global TTI 
 *                  target_tti - target TTI of SPS 
 * Outputs        : None 
 * Returns        : None
 * Description    : This will reserve the  HI resources for all SPS UE's 
 ******************************************************************************/
static void getTargetTTIIndexForTDD(UInt8 current_tti_index,
        UInt8 tti_index1, UInt8 tti_index2,  UInt8 *target_tti_p)
{
    if (tti_index1 < tti_index2)
    {
        if ( (current_tti_index < tti_index1) )
        {
            *target_tti_p = tti_index1; 
        }
        else if ((current_tti_index == tti_index1) )
        {

            *target_tti_p = tti_index2; 
        }    
        else if ((current_tti_index > tti_index1) && (current_tti_index < tti_index2) )
        {
            *target_tti_p = tti_index2; 
        }    
        else if ((current_tti_index >= tti_index2) )
        {

            *target_tti_p = tti_index1; 

        }    
    }
    else
    {
        if ( (current_tti_index < tti_index2) )
        {
            *target_tti_p = tti_index2; 
        }
        else if ((current_tti_index == tti_index2) )
        {
            *target_tti_p = tti_index1; 
        }    
        else if ((current_tti_index > tti_index2) && (current_tti_index < tti_index1) )
        {
            *target_tti_p = tti_index1; 
        }    
        else if ((current_tti_index >= tti_index1) )
        {
            *target_tti_p = tti_index2; 
        }    
    }
}
#endif
/******************************************************************************
 * Function Name  : checkDRXAndMeasGapForTtiIndex 
 * Inputs         : tti_index1 - 1st tti Index 
                    subFrame_offset for TDD  
 *                  spsUlInfo_p   
 * Outputs        : None 
 * Returns        : MAC_SUCCESS or MAC_FAILURE 
 * Description    : This will reserve the  HI resources for all SPS UE's 
 ******************************************************************************/
static  MacRetType checkDRXAndMeasGapForTtiIndex(UInt8 index,
#ifdef TDD_CONFIG
        SInt8 subFrame_offset,
	InternalCellIndex internalCellIndex,
#endif
        LP_SpsUlInfo      spsUlInfo_p
        )
{
    UInt8  spsPeriodicity  = 0;
#ifdef TDD_CONFIG
	UInt8 prevDci0Distance = 0, prevDci0Index = 0, next_prevDci0Index = 0 ;
    UInt8 nextTTIindex = 0, nextTTIindex1 = 0, nextTTIindex2= 0, next_prevDci0Index1 = 0, next_prevDci0Index2 = 0 ;
#endif
    MacRetType retVal = MAC_SUCCESS;
    spsPeriodicity = spsUlInfo_p->spsUlSetupInfo_p->\
                     semiPersistentSchedIntervalUL;

	/* Find a corresponding D Index for this U Index and correspondingly check the DRX state*/
#ifdef TDD_CONFIG
    prevDci0Distance = getPrevSubFrameforDCI0((index % MAX_SUBFRAME),internalCellIndex);
	if (index >= prevDci0Distance)
	{
        prevDci0Index = index - prevDci0Distance;
	}
    else
	{
	    prevDci0Index = (index + MAX_UL_SPS_INTERVAL) - prevDci0Distance;
	}
#endif

#ifdef FDD_CONFIG
    if((DRX_STATE_ON == spsUlInfo_p->ueDrxTTI[index])||
            ( MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[index])||
                /* ++ SPR 19287 */
                ( MEAS_GAP_ON_FOR_DCI0 == spsUlInfo_p->ueMeasGapTTI[index] ) ||
                /* -- SPR 19287 */
            (( SPS_INTERVAL_20 ==  spsIntervalMap_g[spsPeriodicity] )&&
             (( DRX_STATE_ON == spsUlInfo_p->ueDrxTTI[(index + SPS_INTERVAL_20)% MAX_SPS_INTERVAL])||
              (MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[(index + SPS_INTERVAL_20)% MAX_SPS_INTERVAL])||
                /* ++ SPR 19287 */
                 ( MEAS_GAP_ON_FOR_DCI0 == spsUlInfo_p->ueMeasGapTTI[(index + SPS_INTERVAL_20)% MAX_SPS_INTERVAL] ) 
                /* -- SPR 19287 */
			))
      )
    {
        retVal = MAC_FAILURE;
    }
#else
    if( SPS_INTERVAL_40 ==  spsIntervalMap_g[spsPeriodicity] )
    {
        nextTTIindex = (index + subFrame_offset +  SPS_INTERVAL_40)% MAX_UL_SPS_INTERVAL;
        /* + SPR_16905*/
	    next_prevDci0Index =  (prevDci0Index + subFrame_offset +  SPS_INTERVAL_40) % MAX_UL_SPS_INTERVAL;

        if(( DRX_STATE_ON == spsUlInfo_p->ueDrxTTI[prevDci0Index])||
                ( MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[index])||
                ( MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[prevDci0Index])||
                ( MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[next_prevDci0Index])||
                (DRX_STATE_ON == spsUlInfo_p->ueDrxTTI[next_prevDci0Index])||
                (MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[nextTTIindex]))
        /* - SPR_16905*/
        {
            retVal =  MAC_FAILURE;
        }
    }
    else if( SPS_INTERVAL_20 ==  spsIntervalMap_g[spsPeriodicity] )
    {
        nextTTIindex = (index + subFrame_offset + SPS_INTERVAL_20)% MAX_UL_SPS_INTERVAL;
        /* + SPR_16910 */
        nextTTIindex1 = (nextTTIindex + subFrame_offset + SPS_INTERVAL_20)% MAX_UL_SPS_INTERVAL;
        nextTTIindex2 = (nextTTIindex1 + subFrame_offset + SPS_INTERVAL_20)% MAX_UL_SPS_INTERVAL;

        next_prevDci0Index =  (prevDci0Index + subFrame_offset +  SPS_INTERVAL_20)% MAX_UL_SPS_INTERVAL;
        next_prevDci0Index1 =  (next_prevDci0Index + subFrame_offset +  SPS_INTERVAL_20)% MAX_UL_SPS_INTERVAL;
        next_prevDci0Index2 =  (next_prevDci0Index1 + subFrame_offset +  SPS_INTERVAL_20)% MAX_UL_SPS_INTERVAL;

        if(( MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[index])||
                ( MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[nextTTIindex])||
                ( MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[prevDci0Index])||
                ( MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[next_prevDci0Index])||
                ( DRX_STATE_ON == spsUlInfo_p->ueDrxTTI[nextTTIindex])||
                ( DRX_STATE_ON == spsUlInfo_p->ueDrxTTI[index]))
        {
            retVal =  MAC_FAILURE;
        }
        if(( MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[nextTTIindex1])||
                ( MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[nextTTIindex2])||
                ( MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[next_prevDci0Index1])||
                ( MEAS_GAP_ON == spsUlInfo_p->ueMeasGapTTI[next_prevDci0Index2])||
                ( DRX_STATE_ON == spsUlInfo_p->ueDrxTTI[nextTTIindex1])||
                ( DRX_STATE_ON == spsUlInfo_p->ueDrxTTI[nextTTIindex2]))
        {
            /* + SPR_16910 */
            retVal =  MAC_FAILURE;
        }
    }
#endif
    return retVal;
}
/* - SPS_TDD_Changes */
