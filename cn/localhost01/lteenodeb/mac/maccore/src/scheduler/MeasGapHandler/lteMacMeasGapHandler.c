
/****************************************************************************
 * *
 * *  ARICENT -
 * *
 * *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 * *
 * ****************************************************************************
 * *
 * *  $Id: lteMacDRXMgr.c,v $
 * *
 * ****************************************************************************
 * *
 * *  File Description : This file contains all the state machine
 * functions and handler functions that are required by the Measurement Gap
 * Handler 
 * ****************************************************************************
 * *
 * * Revision Details
 * * ----------------
 * * $Log: lteMacMeasGapHandler.c,v $
 * ***************************************************************************/

/******************************************************************************
 *  * Standard Library Includes
 *  **************************************************************************/

/******************************************************************************
 *  * Project Includes
 *****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacMeasGapHandler.h"
#include "lteMacDLUEContext.h"
#include "lteMacULUEContext.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#endif
#include "lteMacParseUtil.h"
/* Review comment fix start GG1 */   
#include "lteMacPDCCHMgr.h"
/* Review comment fix end GG1 */    
    /* + SPR 21535 fix */
#include "lteMacUEConfMgr.h"
    /* - SPR 21535 fix */
/******************************************************************************
 *  Private Definitions
 *****************************************************************************/
#ifndef TDD_CONFIG
#define TOTAL_NUM_UL_DL_CONFIG 7
#endif

/******************************************************************************
 *  Private Types
 *****************************************************************************/
 #ifdef TDD_CONFIG
/* SPR 15306 + */
UInt16 dlFeedbackMap_g[TOTAL_NUM_UL_DL_CONFIG][MAX_SUB_FRAME] =
{
    /*This table gives the number of subframes by which DCI0 precedes UL data*/
    /*cfg0*/
    {4,5,6,3,4,4,5,6,3,4},
    /*cfg1*/
    {5,6,7,4,4,5,6,7,4,4},
    /*cfg2*/
    {6,7,8,4,5,6,7,8,4,5},
    /*cfg3*/
    {9,10,11,6,5,0,0,6,7,8},
    /*cfg4*/
    {10,11,12,7,0,0,0,7,8,9},
    /*cfg5*/
    {11,12,13,0,0,0,0,8,9,10}
};
/* SPR 15306 - */
#endif
UInt16 uplinkToGrantMap_g[TOTAL_NUM_UL_DL_CONFIG][MAX_SUB_FRAME] =
{
    /*This table gives the number of subframes by which DCI0 precedes UL data*/
    /*cfg0*/
    {0xFF,0xFF,6,7,4,0xFF,0xFF,6,7,4},
    /*cfg1*/
    {0xFF,0xFF,6,4,0xFF,0xFF,0xFF,6,4,0xFF},
    /*cfg2*/
    {0xFF,0xFF,4,0xFF,0xFF,0xFF,0xFF,4,0xFF,0xFF},
    /*cfg3*/
    {0xFF,0xFF,4,4,4,0xFF,0xFF,0xFF,0xFF,0xFF},
    /*cfg4*/
    {0xFF,0xFF,4,4,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
    /*cfg5*/
    {0xFF,0xFF,4,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
    /* SPR 11369 Fix Start */
    /*cfg6*/
    {0xFF,0xFF,7,7,5,0xFF,0xFF,7,7,0xFF}
    /* SPR 11369 Fix End */
};
/*****************************************************************************
 ** Private Function Prototypes
 *****************************************************************************/
/* + SPS_TDD_Changes */
STATIC void resetSPSMeasGapTTI( MeasGapContextDL *dlMeasGapContext_p,
                              MeasGapContextUL *ulMeasGapContext_p,
                              /* SPR 16994 changes - START */
                              UInt16 ueIndex,
                              UInt8 gapPatternId,
                              InternalCellIndex internalCellIndex);

void setSPSMeasGapTTI( MeasGapContextDL *dlMeasGapContext_p,
        MeasGapContextUL *ulMeasGapContext_p,
        UInt16 ueIndex,
        InternalCellIndex internalCellIndex);
/* SPR 16994 changes - END*/
/* - SPS_TDD_Changes */

/*SPR 16855 +-*/
void processMeasGapConfig (UeContextForDownlink *dwlkUEInfo_p,
                           MeasGapContextDL *dlMeasGapContext,
                           MeasGapContextUL *ulMeasGapContext,
                           UInt8 *dlMeasGapConfigType,
                           UInt8 *ulMeasGapConfigType,
                           InternalCellIndex internalCellIndex);
void processMeasGapReConfig (UeContextForDownlink *dwlkUEInfo_p,
                             MeasGapContextDL *dlMeasGapContext,
                             MeasGapContextUL *ulMeasGapContext,
                             UInt8 *dlMeasGapConfigType,
                             UInt8 *ulMeasGapConfigType,
                             UInt16 ueIndex,
                             UInt8 gapPatternId, 
                             UInt8 *gapBitMask,
                             /*CA Changes start */
                             InternalCellIndex internalCellIndex);
                             /*CA Changes end */
/*SPR 16855 +-*/
/* SPR 15909 fix start */
UInt8 isTTIInMeasGapPeriod(ULUEContext *ulUEContext_p, tickType_t tti);
/* SPR 15909 fix end */
/*SPR 16855 +-*/
void deleteUEMeasGapConfig (MeasGapContextDL *dlMeasGapContext,
                MeasGapContextUL *ulMeasGapContext,
                UInt8 *dlMeasGapConfigType,
                UInt8 *ulMeasGapConfigType,
                UInt16 ueIndex,
                UInt8 gapPatternId,
                /*CA Changes start  */
                InternalCellIndex internalCellIndex);
                /*CA Changes end  */
/*SPR 16855 +-*/
void createMeasGapOffsetMap (MeasGapConfig *measGapConfig,
                MeasGapContextDL *dlMeasGapContext_p,
                MeasGapContextUL *ulMeasGapContext_p, UInt16 ueIndex,
                InternalCellIndex internalCellIndex);
/*CA Changes start  */
/* SPR 15909 fix start */
void processAllMeasGapOffsetMaps (tickType_t globalAdvanceTick, InternalCellIndex internalCellIndex);
void processMeasGapOffsetMap (LTE_LIST  *measGapMap_p, tickType_t globalTick,UInt32
                              count, InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */
/*CA Changes end  */


/******************************************************************************
 *  * Private Constants
 ******************************************************************************/

/******************************************************************************
 *  * Exported Variables
 ******************************************************************************/

/*****************************************************************************
 *  * Private Variables 
 *****************************************************************************/
/*SPR 16855 +-*/
/*CA Changes start  */
/* This array is used to store the start and end of the UE measurement period, 
 *  * for the UEs who are configured with measurement gap period gp1*/
LTE_LIST measGap80MsMap_g[MAX_NUM_CELL][MAX_NUM_MAPS][MEASUREMENT_GAP_PERIODICITY_80];

/* This array is used to store the start and end of the UE measurement period, 
 *  * for the UEs who are configured with measurement gap period gp0*/
LTE_LIST measGap40MsMap_g[MAX_NUM_CELL][MAX_NUM_MAPS][MEASUREMENT_GAP_PERIODICITY_40];
#ifdef ENDC_ENABLED
/*Meas_Gap_Changes_Start*/
/* This array is used to store the start and end of the UE measurement period, 
 *  * for the UEs who are configured with measurement gap period gp4, gp6, gp10*/
LTE_LIST measGap20MsMap_g[MAX_NUM_CELL][MAX_NUM_MAPS][MEASUREMENT_GAP_PERIODICITY_20];
/* This array is used to store the start and end of the UE measurement period, 
 *  * for the UEs who are configured with measurement gap period gp5, gp9, gp11*/
LTE_LIST measGap160MsMap_g[MAX_NUM_CELL][MAX_NUM_MAPS][MEASUREMENT_GAP_PERIODICITY_160];
/*Meas_Gap_Changes_End*/
#endif
/*SPR 16855 +-*/
/*CA Changes start  */
extern DL_MEAS_GAP_ON_OFF_TRIGGER_TYPE* dlMeasGapOnOffTriggerQueue_gp[MAX_NUM_CELL];
extern UL_MEAS_GAPON_OFF_TRIGGER_TYPE* ulMeasGapOnOffTriggerQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */

/******************************************************************************
 * Private Functions
 *****************************************************************************/

/*****************************************************************************
 * Function Name  : checkForMeasGapEvents 
 * Inputs         : onOffTrigger
 *                  measGapNodeDuplicate_p
 *                  ueIndex
 *                  globalTick - present tick,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function will check for meas gap events.
 *****************************************************************************/
void   checkForMeasGapEvents(MeasGapTrigger onOffTrigger,
        MeasGapOnOffTriggerNode *measGapNodeDuplicate_p, 
        UInt16 ueIndex,
        /* SPR 15909 fix start */
        tickType_t globalTick,
        /* SPR 15909 fix end */
        InternalCellIndex   internalCellIndex)
{
    MeasGapContextDL *measGapContext_p =  
        &dlUECtxInfoArr_g[ueIndex].dlUEContext_p->measGapContext;
    MeasGapContextUL *ulMeasGapContext_p = 
        &ulUECtxInfoArr_g[ueIndex].ulUEContext_p->measGapContext;
    /*SPR 15998 Fix Start*/ 
    DLUEContext       *dlUEContext_p = PNULL;
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    if (MEASUREMENT_PERIOD_ON_DL == onOffTrigger && SETUP == dlUEContext_p->measGapConfigType)
    { /*SPR 15998 Fix end*/
        /*push the node in trigger queue for DL*/
        /*Cov_fix_30April_26395_start*/ 
        /* SPR 23483 Changes Start */
        if(CIRC_SUCCESS == ENQUEUE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode,
        /* SPR 23483 Changes End */
                    &(measGapNodeDuplicate_p->anchorNode), internalCellIndex))
        {   
            measGapContext_p->isMeasGapPeriodOn = TRUE;
        }
        else
        {
            FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapNodeDuplicate_p);
        }
        /*Cov_fix_30April_26395_stop*/

    }
    else if (MEASUREMENT_PERIOD_OFF_DL ==  onOffTrigger)
    {
        /*push the node in trigger queue for DL*/
        /*Cov_fix_30April_26395_start*/ 
        /* SPR 23483 Changes Start */
        if(CIRC_SUCCESS == ENQUEUE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode,
        /* SPR 23483 Changes End */
                    &(measGapNodeDuplicate_p->anchorNode), internalCellIndex))
        {    
            /*Cov_fix_30April_26395_stop*/ 
            measGapContext_p->isMeasGapPeriodOn = FALSE;
            /*In case there is no UL subframe in the measurement
             * period, then the NS/NE should still be updated.
             * Hence added with DL Triggers*/
            ulMeasGapContext_p->ulMeasGapNextEndTTI = 
                (globalTick +
                 ulMeasGapContext_p->measGapPeriodicity);
            ulMeasGapContext_p->ulMeasGapNextStartTTI =
                ulMeasGapContext_p->ulMeasGapNextEndTTI -
                MEAS_GAP_DURATION;
            /*Cov_fix_30April_26395_start*/ 
        }
        else
        {
            FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapNodeDuplicate_p);
        }
        /*Cov_fix_30April_26395_stop*/

    }/*SPR 15998 Fix Start*/
    else if (MEASUREMENT_PERIOD_ON_UL == onOffTrigger && SETUP == dlUEContext_p->measGapConfigType)
    { /*SPR 15998 Fix end*/
        /*push the node in trigger queue for UL*/
        /*Cov_fix_30April_26395_start*/ 
        /* SPR 23483 Changes Start */
        if(CIRC_SUCCESS == ENQUEUE_UL_MEAS_GAPON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode,
        /* SPR 23483 Changes End */
                    &(measGapNodeDuplicate_p->anchorNode), internalCellIndex))
        {
            ulMeasGapContext_p->isMeasGapPeriodOn = 
                TRUE;
        }
        else
        {
            FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapNodeDuplicate_p);
        }
        /*Cov_fix_30April_26395_stop*/ 
    }
    else if (MEASUREMENT_PERIOD_OFF_UL == onOffTrigger)
    {
        /*push the node in trigger queue for UL*/
        /*Cov_fix_30April_26395_start*/ 
        /* SPR 23483 Changes Start */
        if(CIRC_SUCCESS == ENQUEUE_UL_MEAS_GAPON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode,
        /* SPR 23483 Changes End */
                    &(measGapNodeDuplicate_p->anchorNode), internalCellIndex))
        {
            ulMeasGapContext_p->isMeasGapPeriodOn = 
                FALSE;
        }
        else
        {
            FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapNodeDuplicate_p);
        }
        /*Cov_fix_30April_26395_stop*/ 
    }
    /* Coverity 25495 Fix Start*/
    else
    {
        FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(measGapNodeDuplicate_p);
    }
    /* Coverity 25495 Fix End*/
}

/*****************************************************************************
 * Function Name  : processMeasGapOffsetMap 
 * Inputs         : measGapMap_p - pointer to LTE_LIST,
 *                  globalTick - present tick,
 *                  count
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function will read th measuremnt map and on the basis
 *                  of the triggers saved in the nodes, will populate the
 *                  triggger queue(s).  
 *****************************************************************************/
    /* SPR 15909 fix start */
void processMeasGapOffsetMap (LTE_LIST  *measGapMap_p,tickType_t globalTick, UInt32
        count, InternalCellIndex internalCellIndex)
    /* SPR 15909 fix end */
{
    UInt32 measGapMapTraverseIndex = 0;
    MeasGapOnOffTriggerNode *measGapNode_p = PNULL;
    MeasGapOnOffTriggerNode *measGapNodeDuplicate_p = PNULL;
    DLUEContextInfo *dlUEContextInfo_p = PNULL;

    if (count)
    {
        measGapNode_p = (MeasGapOnOffTriggerNode *)getListNode(
                measGapMap_p, 0);
        for(measGapMapTraverseIndex = 0; 
                measGapMapTraverseIndex < count ; 
                measGapMapTraverseIndex++)
        {
            if (PNULL != measGapNode_p)
            {

                dlUEContextInfo_p = 
                    &dlUECtxInfoArr_g[measGapNode_p->ueIndex];
                if (FALSE == dlUEContextInfo_p->pendingDeleteFlag)
                {
                    GET_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q( 
                            measGapNodeDuplicate_p, 
                            MeasGapOnOffTriggerNode );
                    if (measGapNodeDuplicate_p)
                    {
                        /* Get the first node from the list*/
                        measGapNodeDuplicate_p->ueIndex =
                            measGapNode_p->ueIndex;
                        measGapNodeDuplicate_p->onOffTrigger  = 
                            measGapNode_p->onOffTrigger;

                        checkForMeasGapEvents( measGapNode_p->onOffTrigger,
                                measGapNodeDuplicate_p, 
                                measGapNode_p->ueIndex, globalTick,
                                internalCellIndex );
                        LOG_MAC_MSG(MAC_MEAS_TRIGGER_HANDLER_ID,LOGDEBUG,
                                MAC_MEAS_HANDLER,
                                getCurrentTick(),
                                measGapNode_p->ueIndex, 
                                measGapNode_p->onOffTrigger,
                                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"MEAS_GAP_HANDLER_TRIGGER_SENT");
                    }
                    else
                    {
                        lteWarning("Memory Alloc Failed for Duplicate Node");
                    }
                }
                measGapNode_p = (MeasGapOnOffTriggerNode *)getNextListNode(
                        (LTE_LIST_NODE *)measGapNode_p);
            }
        }
    }    
}

/*****************************************************************************
 * Function Name  : processAllMeasGapOffsetMaps
 * Inputs         : globalAdvanceTick
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function wraps the globalTick around 40 and 80ms to
 *                  find the correct index for the measuremnt gap MAP and then
 *                  using that index, checks for presence of nodes in the MAP.
 *                  If nodes are found, processing is done usign helper functions.                    
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
void processAllMeasGapOffsetMaps (tickType_t globalAdvanceTick, InternalCellIndex internalCellIndex)
/* SPR 15909 fix end */
/*CA Changes end  */
{
    UInt32 measGapMapOffset40msMap =0;
    UInt32 measGapMapOffset80msMap =0;
    UInt32 measMapListCount = 0;

    measGapMapOffset40msMap = globalAdvanceTick%MEASUREMENT_GAP_PERIODICITY_40;
    measGapMapOffset80msMap = globalAdvanceTick%MEASUREMENT_GAP_PERIODICITY_80;
#ifdef ENDC_ENABLED
	/*Meas_Gap_Changes_Start*/
    UInt32 measGapMapOffset20msMap = globalAdvanceTick%MEASUREMENT_GAP_PERIODICITY_20;
    UInt32 measGapMapOffset160msMap = globalAdvanceTick%MEASUREMENT_GAP_PERIODICITY_160;

	measMapListCount = listCount(&(measGap20MsMap_g[internalCellIndex][measGap20msInfo_gp[internalCellIndex]->activeIndex][measGapMapOffset20msMap]));
    if (0 !=  measMapListCount)
    {
        processMeasGapOffsetMap(&measGap20MsMap_g[internalCellIndex][measGap20msInfo_gp[internalCellIndex]->activeIndex][measGapMapOffset20msMap],
                globalAdvanceTick, measMapListCount, internalCellIndex);
    }
	measMapListCount = listCount(&(measGap160MsMap_g[internalCellIndex][measGap160msInfo_gp[internalCellIndex]->activeIndex][measGapMapOffset160msMap]));
    if (0 !=  measMapListCount)
    {
        processMeasGapOffsetMap(&measGap160MsMap_g[internalCellIndex][measGap160msInfo_gp[internalCellIndex]->activeIndex][measGapMapOffset160msMap],
                globalAdvanceTick, measMapListCount, internalCellIndex);
    }
	/*Meas_Gap_Changes_End*/
#endif
    /*SPR 16855 +-*/
    /*CA Changes start  */
    measMapListCount = listCount(&(measGap40MsMap_g[internalCellIndex][measGap40msInfo_gp[internalCellIndex]->activeIndex][measGapMapOffset40msMap]));
    /*SPR 16855 +-*/
    if (0 !=  measMapListCount)
    {
        /* Process the Map for 40ms periodicity */
        /*SPR 16855 +-*/
        /*CA Changes start  */
        processMeasGapOffsetMap(&measGap40MsMap_g[internalCellIndex][measGap40msInfo_gp[internalCellIndex]->activeIndex][measGapMapOffset40msMap],
                globalAdvanceTick, measMapListCount, internalCellIndex);
        /*SPR 16855 +-*/
    }

    /*SPR 16855 +-*/
    /*CA Changes start  */
    measMapListCount = listCount(&(measGap80MsMap_g[internalCellIndex][measGap80msInfo_gp[internalCellIndex]->activeIndex][measGapMapOffset80msMap]));
    /*SPR 16855 +-*/
    if (0 !=  measMapListCount)
    {
        /* Process the Map for 80ms periodicity */
    /*SPR 16855 +-*/
    processMeasGapOffsetMap(&measGap80MsMap_g[internalCellIndex][measGap80msInfo_gp[internalCellIndex]->activeIndex][measGapMapOffset80msMap], 
                 globalAdvanceTick, measMapListCount, internalCellIndex);
    /*SPR 16855 +-*/
    }
}


/*****************************************************************************
 * Function Name  : createMeasGapOffsetMap 
 * Inputs         : measGapConfig_p - pointer to MeasGapConfig,
 *                  dlMeasGapContext_p - pointer to MeasGapContextDL,
 *                  ulMeasGapContext_p - pointer to MeasGapContextUL,
 *                  ueIndex - Index of UE
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None 
 * Returns        : None
 * Description    : This function calculates the measurement pattern start and
 *                  end time for both FDD and TDD systems. Once this is
 *                  calculated, the function starts inserting nodes at the
 *                  start and end indices of the measurement gap MAP
 *****************************************************************************/
void createMeasGapOffsetMap (MeasGapConfig *measGapConfig_p,
        MeasGapContextDL *dlMeasGapContext_p,
        MeasGapContextUL *ulMeasGapContext_p, UInt16 ueIndex,
        InternalCellIndex internalCellIndex)
        
{
    UInt16 measGapSfn = 0;
    UInt16 measGapSf = 0;
    UInt32 measGapPatternStartDl = 0;
    UInt32 measGapPatternStartUl = 0;
    UInt32 measGapPatternEndDl = 0;
    UInt32 measGapPatternEndUl = 0;
    UInt8 measGapTTI = 0;
   /* SPR 15909 fix start */
    tickType_t globalTick = 0;
    /* SPR 15909 fix end */
	/* SPR 15306 + */
#ifdef TDD_CONFIG    
    UInt8 advanceDLStop = 0;
#endif    
	/* SPR 15306 - */
    UInt8 grantAdvanceULStart = 0;
    UInt8 uplinkSfStartDelay = 0;
    UInt8 measGapPeriodicity =0;
    UInt16 foundUl = FALSE;
    /* SPR 15909 fix start */
    tickType_t measGapElapsedTTI = 0;
    /* SPR 15909 fix end */
#ifndef FDD_CONFIG
    UInt32 loopCounter = 0;
    UInt8 firstUplink = 0;
    UInt8 tddConfig = 0;
    UInt8 subFrameType = 0;
#endif
    MeasGapTrigger measGapTrigger = MEASUREMENT_PERIOD_INVALID;
    LTE_LIST                *measGapMap_p = PNULL;
    MeasGapOnOffTriggerNode *measGapOnOffTriggerNode_p = PNULL;
    measGapSfn = (measGapConfig_p->gapOffset/10);
    measGapSf  = (measGapConfig_p->gapOffset % 10);

    measGapTTI = measGapSfn * 10 + measGapSf;
    /* + SPR_16082 */
    UInt32 passiveIdx = 0; 
    /* - SPR_16082 */
#ifdef FDD_CONFIG    
    measGapPatternStartDl = measGapTTI;
    measGapPatternEndDl   = (measGapPatternStartDl + MEAS_GAP_DURATION);
    grantAdvanceULStart = 4;
    foundUl = TRUE;
#else
    tddConfig = getTddConfig(internalCellIndex);
    /* SPR 2446 Changes Start */
    if (TOTAL_NUM_UL_DL_CONFIG <= tddConfig)
    {
        return;
    }
    /* SPR 2446 Changes End */
    
    foundUl = FALSE;
    measGapPatternStartDl = measGapTTI;
    measGapPatternEndDl   = measGapTTI + MEAS_GAP_DURATION;
    for (loopCounter = 0; loopCounter <= MEAS_GAP_DURATION; loopCounter++)
    { /* first uplink and last uplink dont actually carry subframe values in 
         subframe range 0-10, but carry an absolute value*/
        subFrameType = getSubframeType((measGapSf + loopCounter)%10, internalCellIndex);
        if(UL == subFrameType)
        {
            if (FALSE == foundUl)
            {
                firstUplink = measGapSf + loopCounter;
                uplinkSfStartDelay = loopCounter;
            }
            foundUl = TRUE;
        }
    }
    if (TRUE == foundUl)
    {
	/* SPR 15306 + */
	advanceDLStop = dlFeedbackMap_g[tddConfig][measGapSf];
        grantAdvanceULStart =  
            uplinkToGrantMap_g[tddConfig][firstUplink % 10];
	/* SPR 15306 - */
    }
#endif
    globalTick = getCurrentTick();
    /*Find the correct MAP and the periodicity*/
    if (GP0 == measGapConfig_p->gapPatternId
#ifdef ENDC_ENABLED
		/*Meas_Gap_Changes_Start*/
		||(GP7_R15 == measGapConfig_p->gapPatternId)
		/*Meas_Gap_Changes_End*/
#endif
    	)
    {
        /*SPR 16855 +-*/
        /*CA Changes start  */
        measGapMap_p = &(measGap40MsMap_g[internalCellIndex][measGap40msInfo_gp[internalCellIndex]->passiveIndex][0]);
        /* + SPR_16082 */
        passiveIdx = measGap40msInfo_gp[internalCellIndex]->passiveIndex;
        /* - SPR_16082 */
        /*SPR 16855 +-*/
        measGapPeriodicity = MEASUREMENT_GAP_PERIODICITY_40;
    }
#ifdef ENDC_ENABLED
		/*Meas_Gap_Changes_Start*/
	else if(GP4_R15 == measGapConfig_p->gapPatternId ||
		GP6_R15 == measGapConfig_p->gapPatternId ||
		GP10_R15 == measGapConfig_p->gapPatternId)
	{
    	measGapMap_p = &(measGap20MsMap_g[internalCellIndex][measGap20msInfo_gp[internalCellIndex]->passiveIndex][0]);
        passiveIdx = measGap20msInfo_gp[internalCellIndex]->passiveIndex;
        measGapPeriodicity = MEASUREMENT_GAP_PERIODICITY_20;
	}
	else if(GP5_R15 == measGapConfig_p->gapPatternId ||
		GP9_R15 == measGapConfig_p->gapPatternId ||
		GP11_R15 == measGapConfig_p->gapPatternId)
	{
    	measGapMap_p = &(measGap160MsMap_g[internalCellIndex][measGap160msInfo_gp[internalCellIndex]->passiveIndex][0]);
        passiveIdx = measGap160msInfo_gp[internalCellIndex]->passiveIndex;
        measGapPeriodicity = MEASUREMENT_GAP_PERIODICITY_160;
	}
		/*Meas_Gap_Changes_End*/

#endif
    else
    {
        /*SPR 16855 +-*/
        /*CA Changes start  */
        measGapMap_p = &(measGap80MsMap_g[internalCellIndex][measGap80msInfo_gp[internalCellIndex]->passiveIndex][0]);
        /* + SPR_16082 */
        passiveIdx = measGap80msInfo_gp[internalCellIndex]->passiveIndex; 
        /* - SPR_16082 */
        /*SPR 16855 +-*/
        measGapPeriodicity = MEASUREMENT_GAP_PERIODICITY_80;
    }
    ulMeasGapContext_p->measGapPeriodicity = measGapPeriodicity;
/* Review comment fix start GG1 */    
#ifdef FDD_CONFIG   
    measGapPatternStartDl = ((measGapPatternStartDl + measGapPeriodicity)
                               - FDD_HARQ_OFFSET) % measGapPeriodicity;
#else
	/* SPR 15306 + */
    measGapPatternStartDl = ((measGapPatternStartDl + measGapPeriodicity)
                               - advanceDLStop) % measGapPeriodicity;
	/* SPR 15306 - */
#endif 
/* Review comment fix end GG1 */    
    /*Reach a TTI which is atleast equal to the current time and satisfies
     * 	Measurement Offset criterion*/

    ulMeasGapContext_p->ulMeasGapNextStartTTI = measGapTTI;
    measGapElapsedTTI = (
            globalTick - measGapTTI) /
        measGapPeriodicity;
/*SPR 3676 changes start*/
    if (( /*check for a race condition when this calculation returns a 
            start value which has a difference equal to periodicity from the 
            globalTick*/
                (((measGapElapsedTTI + 1) * measGapPeriodicity) + measGapTTI) -
                globalTick) > measGapPeriodicity
       )
    {
/*SPR 3676 changes end*/
        /* When measGapTTI is 0 and globalTick is a multiple
         * of periodicity, then this check is needed for nextStartTTI*/
        ulMeasGapContext_p->ulMeasGapNextStartTTI = 
            (measGapElapsedTTI * measGapPeriodicity);
    }
    else
    {
        ulMeasGapContext_p->ulMeasGapNextStartTTI =
            (measGapElapsedTTI + 1) * measGapPeriodicity
            + measGapTTI;
    }


    ulMeasGapContext_p->ulMeasGapNextEndTTI = 
        ulMeasGapContext_p->ulMeasGapNextStartTTI + 
        MEAS_GAP_DURATION;

    /*	To check for wrap-around condition -->
     *  Actual UL start node = 
     *  [measGapTTI + uplinkSfStartDelay - grantAdvanceULStart] 	

     *  where uplinkSfStartDelay is the number of Ms from the gapOffset to the 
     *  first UL Subframe. For eg. if gapOffset is 1, in TDD UL-DL Config, 
     *  uplinkSfStartDelay = 1; grantAdvanceULStart = k which is 4 for FDD and 
     *  varies as per specifications in TDD; measGapTTI is the offset in the 
     *  40Ms/80Ms window
     *  */

    if (measGapTTI + uplinkSfStartDelay < grantAdvanceULStart)
    {
        measGapPatternStartUl = 
            (measGapTTI + uplinkSfStartDelay
             + measGapPeriodicity - grantAdvanceULStart);
    }
    else
    {
        measGapPatternStartUl = (measGapTTI + uplinkSfStartDelay -
                grantAdvanceULStart);
#ifdef TDD_CONFIG	
        /*This will never happen in FDD*/
        if (measGapPatternStartUl > measGapPeriodicity)
        {
            measGapPatternStartUl = 
                (measGapPatternStartUl % measGapPeriodicity);
        } 
#endif
    }

    measGapPatternEndUl = 
        (measGapTTI + MEAS_GAP_DURATION)% measGapPeriodicity; 
    measGapPatternEndDl = measGapPatternEndDl % measGapPeriodicity;


    /* Enter Start Node in the list*/
    measGapTrigger = MEASUREMENT_PERIOD_ON_DL;
    GET_MEM_FROM_POOL(MeasGapOnOffTriggerNode,
            measGapOnOffTriggerNode_p,
            sizeof(MeasGapOnOffTriggerNode), PNULL);
    if (measGapOnOffTriggerNode_p)
    {
        measGapOnOffTriggerNode_p->ueIndex = ueIndex;
        measGapOnOffTriggerNode_p->onOffTrigger = measGapTrigger;
        /* + SPR_16082 */
        dlMeasGapContext_p->measGapStartOffsetDL[passiveIdx] = measGapPatternStartDl;
        dlMeasGapContext_p->measGapEndOffsetDL[passiveIdx] = measGapPatternEndDl;
        /*SPR 16855 +-*/
        dlMeasGapContext_p->measGapStartNodeDL_p[passiveIdx] = measGapOnOffTriggerNode_p;
        /* - SPR_16082 */
        /*SPR 16855 +-*/
        listInsertNode(&(measGapMap_p[measGapPatternStartDl]),
                &measGapOnOffTriggerNode_p->anchorNode);

    }
    else
    {
        LOG_MAC_ERROR(MAC_RRC_INF,"Failed in getting memory for DL ON Trigger Node UE %u",
                ueIndex);
    }

    /* Enter End Node in the list*/
    measGapTrigger = MEASUREMENT_PERIOD_OFF_DL;
    GET_MEM_FROM_POOL(MeasGapOnOffTriggerNode,measGapOnOffTriggerNode_p,
            sizeof(MeasGapOnOffTriggerNode), PNULL);
    if (measGapOnOffTriggerNode_p)
    {
        measGapOnOffTriggerNode_p->ueIndex = ueIndex;
        measGapOnOffTriggerNode_p->onOffTrigger = measGapTrigger;
        /* + SPR_16082 */
        dlMeasGapContext_p->measGapStartOffsetDL[passiveIdx] = 
            measGapPatternStartDl;
        dlMeasGapContext_p->measGapEndOffsetDL[passiveIdx] =
            measGapPatternEndDl;
        /*SPR 16855 +-*/
        dlMeasGapContext_p->measGapEndNodeDL_p[passiveIdx] = 
                measGapOnOffTriggerNode_p;
        /*SPR 16855 +-*/
        /* - SPR_16082 */
        listInsertNode(&(measGapMap_p[measGapPatternEndDl]),
                &measGapOnOffTriggerNode_p->anchorNode);

    }
    else
    {
        LOG_MAC_ERROR(MAC_RRC_INF,"Failed in getting memory for DL OFF Trigger Node UE %u\n",
                ueIndex);
    }


    /* Add UL Node*/
    if (TRUE == foundUl) 
        /*In case there is NO uplink SF inside the measurement window, then we 
         * 	don't need to add UL nodes*/
    {
        /* Enter Start Node in the list*/
        measGapTrigger = MEASUREMENT_PERIOD_ON_UL;
        GET_MEM_FROM_POOL(MeasGapOnOffTriggerNode,measGapOnOffTriggerNode_p,
                sizeof(MeasGapOnOffTriggerNode), PNULL);
        if (measGapOnOffTriggerNode_p)
        {
            measGapOnOffTriggerNode_p->ueIndex = ueIndex;
            measGapOnOffTriggerNode_p->onOffTrigger = measGapTrigger;
            /* + SPR_16082 */
            ulMeasGapContext_p->measGapStartOffsetUL[passiveIdx] = 
                measGapPatternStartUl;
            ulMeasGapContext_p->measGapEndOffsetUL[passiveIdx] =
                measGapPatternEndUl;
            /*SPR 16855 +-*/
            ulMeasGapContext_p->measGapStartNodeUL_p[passiveIdx]= 
                    measGapOnOffTriggerNode_p;
            /* - SPR_16082 */
            /*SPR 16855 +-*/
           
            listInsertNode(&(measGapMap_p[measGapPatternStartUl]),
                    &measGapOnOffTriggerNode_p->anchorNode);

        }
        else
        {
            LOG_MAC_ERROR(MAC_RRC_INF,"Failed in getting memory for UL ON Trigger Node UE %u\n",
                    ueIndex);
        }
        /* Enter End Node in the list*/
        measGapTrigger = MEASUREMENT_PERIOD_OFF_UL;
        GET_MEM_FROM_POOL(MeasGapOnOffTriggerNode,measGapOnOffTriggerNode_p,
                sizeof(MeasGapOnOffTriggerNode), PNULL);
        if (measGapOnOffTriggerNode_p)
        {
            measGapOnOffTriggerNode_p->ueIndex = ueIndex;
            measGapOnOffTriggerNode_p->onOffTrigger = measGapTrigger;
            /* + SPR_16082 */
            ulMeasGapContext_p->measGapStartOffsetUL[passiveIdx] = 
                measGapPatternStartUl;
            ulMeasGapContext_p->measGapEndOffsetUL[passiveIdx] =
                measGapPatternEndUl;
            /*SPR 16855 +-*/
            ulMeasGapContext_p->measGapEndNodeUL_p[passiveIdx] = 
                    measGapOnOffTriggerNode_p;
            /* - SPR_16082 */
            /*SPR 16855 +-*/
            listInsertNode(&(measGapMap_p[measGapPatternEndUl]),
                    &measGapOnOffTriggerNode_p->anchorNode);

        }
        else
        {
            LOG_MAC_ERROR(MAC_RRC_INF,"Failed in getting memory for UL Off Trigger Node UE %u\n",
                    ueIndex);
        }
    }
    LOG_MAC_MSG(MAC_MEAS_MAP_HANDLER_ID,LOGDEBUG,MAC_MEAS_HANDLER,
            getCurrentTick(),
            ueIndex,measGapPatternStartDl,measGapPatternEndDl,
            measGapPatternStartUl,measGapPatternEndUl,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_MEAS_MAP_CREATE_SUCCESS");
}

/*****************************************************************************
 * Function Name  : deleteUEMeasGapConfig 
 * Inputs         : dlMeasGapContext_p - pointer to MeasGapContextDL,
 *                  ulMeasGapContext_p - pointer to MeasGapContextUL,
 *                  dlMeasGapConfigType,
 *                  ulMeasGapConfigType,
 *                  ueIndex - Index of UE,
 *                  gapPatternId
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function uses the context passed on by the caller to
 *                  be deleted, calculates the kind of MAP that needs to be
 *                  checked and then, traverses the MAP to find the UE id and
 *                  thereafter release the nodes.                    
 *****************************************************************************/
/*SPR 16855 +-*/
void deleteUEMeasGapConfig (MeasGapContextDL *dlMeasGapContext_p,
                MeasGapContextUL *ulMeasGapContext_p,
                UInt8 *dlMeasGapConfigType,
                UInt8 *ulMeasGapConfigType,
                UInt16 ueIndex,
                UInt8 gapPatternId, 
                /*CA Changes start  */
                InternalCellIndex internalCellIndex)
                /*CA Changes end  */
/*SPR 16855 +-*/
{
    LTE_LIST                *measGapMap_p = PNULL;
    MeasGapOnOffTriggerNode *measNode_p = PNULL;
    /*SPR 16855 +-*/
    MeasGapOnOffTriggerNode *measGapNodeDLDuplicate_p = PNULL;
    MeasGapOnOffTriggerNode *measGapNodeULDuplicate_p = PNULL;
    MeasGapOnOffTriggerNode *measGapMapEndDL_p = PNULL;
    MeasGapOnOffTriggerNode *measGapMapStartUL_p = PNULL;
    MeasGapOnOffTriggerNode *measGapMapEndUL_p = PNULL;
    /* + SPR_16082 */
    UInt32 passiveIdx = 0;
    /* - SPR_16082 */
    /*SPR 16855 +-*/
    /*SPR 16855 +-*/
    GET_MEM_FROM_POOL(MeasGapOnOffTriggerNode,
            measGapNodeDLDuplicate_p,
            sizeof(MeasGapOnOffTriggerNode), PNULL);
    /* + Coverity 24488 */
    if( PNULL == measGapNodeDLDuplicate_p )
    {
        /*CRITICAL! Memory error*/
        LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MeasGapOnOffTriggerNode), 
                0,0, 0, 0,0, __FILE__, __func__);
        ALARM_MSG(MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        return;
    }
    /* - Coverity 24488 */
    measGapNodeDLDuplicate_p->ueIndex =
        ueIndex;
    measGapNodeDLDuplicate_p->onOffTrigger  = 
        MEASUREMENT_PERIOD_OFF_DL;

    GET_MEM_NODE_UL_MEAS_GAPON_OFF_TRIGGER_Q(measGapNodeULDuplicate_p,MeasGapOnOffTriggerNode);
    /* + Coverity 25475 */
    if( PNULL == measGapNodeULDuplicate_p )
    {
        freeMemPool( measGapNodeDLDuplicate_p );
        /*CRITICAL! Memory error*/
        LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(MeasGapOnOffTriggerNode), 
                0,0, 0, 0,0, __FILE__, __func__);
        ALARM_MSG(MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        return;
    }
    /* - Coverity 25475 */
    measGapNodeULDuplicate_p->ueIndex =
        ueIndex;
    measGapNodeULDuplicate_p->onOffTrigger  = 
        MEASUREMENT_PERIOD_OFF_UL;

    /*Cov_fix_30April_26394_start*/ 
    if(!ENQUEUE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode,
            /*CA Changes start  */
            &(measGapNodeDLDuplicate_p->anchorNode), internalCellIndex))
            /*CA Changes end  */
    {
        LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, DEFAULT_INT_VALUE, 
                0,0, 0, 0,0, __FILE__, __func__);
        freeMemPool( measGapNodeDLDuplicate_p );
    }
    /*Cov_fix_30April_26394_stop*/ 

    /*Cov_fix_30April_26394_start*/ 
    if(!ENQUEUE_UL_MEAS_GAPON_OFF_TRIGGER_Q(MeasGapOnOffTriggerNode,
            /*CA Changes start  */
            &(measGapNodeULDuplicate_p->anchorNode), internalCellIndex))
            /*CA Changes end  */
    {
        LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__,DEFAULT_INT_VALUE, 
                0,0, 0, 0,0, __FILE__, __func__);
        freeMemPool( measGapNodeULDuplicate_p );
    }
    /*Cov_fix_30April_26394_stop*/ 

    /* + SPR_16082 */
    if (GP0 == gapPatternId
#ifdef ENDC_ENABLED
		/*Meas_Gap_Changes_Start*/
    	|| (GP7_R15 == gapPatternId)
    	/*Meas_Gap_Changes_End*/
#endif
    	)
    {
        /*CA Changes start  */
        measGapMap_p = &(measGap40MsMap_g[internalCellIndex][measGap40msInfo_gp[internalCellIndex]->passiveIndex][0]);
        passiveIdx = measGap40msInfo_gp[internalCellIndex]->passiveIndex;
        /*CA Changes end  */
    }
#ifdef ENDC_ENABLED
	/*Meas_Gap_Changes_Start*/
	else if(GP4_R15 == gapPatternId || GP6_R15 == gapPatternId
		||GP10_R15 == gapPatternId)
	{
    	measGapMap_p = &(measGap20MsMap_g[internalCellIndex][measGap20msInfo_gp[internalCellIndex]->passiveIndex][0]);
        passiveIdx = measGap20msInfo_gp[internalCellIndex]->passiveIndex;
	}
	else if(GP5_R15 == gapPatternId || GP9_R15 == gapPatternId
		||GP11_R15 == gapPatternId)
	{
    	measGapMap_p = &(measGap160MsMap_g[internalCellIndex][measGap160msInfo_gp[internalCellIndex]->passiveIndex][0]);
        passiveIdx = measGap160msInfo_gp[internalCellIndex]->passiveIndex;
	}
	/*Meas_Gap_Changes_End*/	
#endif
    else
    {
        /*CA Changes start  */
        measGapMap_p = &(measGap80MsMap_g[internalCellIndex][measGap80msInfo_gp[internalCellIndex]->passiveIndex][0]);
        passiveIdx = measGap80msInfo_gp[internalCellIndex]->passiveIndex; 
        /*CA Changes end  */
    }
    measNode_p = dlMeasGapContext_p->measGapStartNodeDL_p[passiveIdx]; 
    measGapMapEndDL_p = dlMeasGapContext_p->measGapEndNodeDL_p[passiveIdx]; 
    measGapMapStartUL_p = ulMeasGapContext_p->measGapStartNodeUL_p[passiveIdx]; 
    measGapMapEndUL_p = ulMeasGapContext_p->measGapEndNodeUL_p[passiveIdx];
    /* - SPR_16082 */
    /*SPR 16855 +-*/
    /* SPR 9967 Fix Start*/
    if((PNULL != measNode_p) && 
            /* + SPR_16082 */
            (listCount(&measGapMap_p[dlMeasGapContext_p->measGapStartOffsetDL[passiveIdx]])))
    {
        listDeleteNode(
                &(measGapMap_p[dlMeasGapContext_p->measGapStartOffsetDL[passiveIdx]]),
                (LTE_LIST_NODE *)measNode_p);

        freeMemPool((void*)measNode_p);
        /* Resetting the Node in context with NULL */

        dlMeasGapContext_p->measGapStartNodeDL_p[passiveIdx] = PNULL;
        /* - SPR_16082 */
    }
    /* SPR 9967 Fix End */
    /*SPR 16855 +-*/
    measNode_p = measGapMapEndDL_p;
    /*SPR 16855 +-*/
    /* SPR 6753 changes start */
    /* SPR 9967 Fix Start*/
    if((PNULL != measNode_p) && 
            /* + SPR_16082 */
            (listCount(&measGapMap_p[dlMeasGapContext_p->measGapEndOffsetDL[passiveIdx]])))
    {
    /* SPR 6753 changes end */
        listDeleteNode(
                &(measGapMap_p[dlMeasGapContext_p->measGapEndOffsetDL[passiveIdx]]),
                (LTE_LIST_NODE *)measNode_p);

        freeMemPool((void*)measNode_p);
        /* Resetting the Node in context with NULL */
        dlMeasGapContext_p->measGapEndNodeDL_p[passiveIdx] = PNULL;
        /* - SPR_16082 */
    }
    /* SPR 9967 Fix End */
    /*SPR 16855 +-*/
    measNode_p = measGapMapStartUL_p;
    /*SPR 16855 +-*/
    /*In case of TDD if UL frame is not present in MEAS GAP period
      There will not be any UL nodes to delete*/
    /* SPR 9967 Fix Start*/
    if((PNULL != measNode_p) && 
            /* + SPR_16082 */
            (listCount(&measGapMap_p[ulMeasGapContext_p->measGapStartOffsetUL[passiveIdx]])))
    {
        listDeleteNode(
                &(measGapMap_p[ulMeasGapContext_p->measGapStartOffsetUL[passiveIdx]]),
                (LTE_LIST_NODE *)measNode_p);

        freeMemPool((void*)measNode_p);
        /* Resetting the Node in context with NULL */
        ulMeasGapContext_p->measGapStartNodeUL_p[passiveIdx] = PNULL;
        /* - SPR_16082 */
    }
    /* SPR 9967 Fix End */
    /*SPR 16855 +-*/
    measNode_p = measGapMapEndUL_p;
    /*SPR 16855 +-*/

    /* SPR 9967 Fix Start*/
    if((PNULL != measNode_p) && 
            /* + SPR_16082 */
            (listCount(&measGapMap_p[ulMeasGapContext_p->measGapEndOffsetUL[passiveIdx]])))
    {
        listDeleteNode(
                &(measGapMap_p[ulMeasGapContext_p->measGapEndOffsetUL[passiveIdx]]),
                (LTE_LIST_NODE *)measNode_p);

        freeMemPool((void*)measNode_p);
        /* Resetting the Node in context with NULL */
        ulMeasGapContext_p->measGapEndNodeUL_p[passiveIdx] = PNULL;
        /* - SPR_16082 */
    }
    /* + SPS_TDD_Changes */
    /* SPR 16994 changes - START */
    resetSPSMeasGapTTI(dlMeasGapContext_p, ulMeasGapContext_p, ueIndex, gapPatternId, internalCellIndex);
    /* SPR 16994 changes - END */
    /* - SPS_TDD_Changes */
    /* SPR 9967 Fix End */
    /*SPR 16855 +-*/
    *ulMeasGapConfigType = RELEASE;
    *dlMeasGapConfigType = RELEASE;
    LOG_MAC_MSG(MAC_MEAS_EVENT_HANDLER_ID,LOGDEBUG,MAC_MEAS_HANDLER,
            getCurrentTick(),
            ueIndex,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE
            ,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_MEAS_DELETE_SUCCESS");
    /*SPR 16855 +-*/
    return;
}

/*****************************************************************************
 * Function Name  : processMeasGapConfig 
 * Inputs         : dwlkUEInfo_p - Pointer to downlink UE info 
 *                  dlMeasGapContext - Pointer to Downlink Measeurement context 
 *                  ulMeasGapContext - pointer to Uplink Measurement context
 *                  dlMeasGapConfigType - Type of measurement gap in DL 
 *                  ulMeasGapConfigType -  Type of measurement gap in UL
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None 
 * Returns        : None
 * Description    : This function checks the requested configuration and
 *                  accordingly deletes the old context and creates fresh MAP 
 *                  or if the request is not for setup,it just deletes the old
 *                  context.                    
 *****************************************************************************/
/*SPR 16855 +-*/
void processMeasGapConfig (UeContextForDownlink *dwlkUEInfo_p,
                           MeasGapContextDL *dlMeasGapContext,
                           MeasGapContextUL *ulMeasGapContext,
                           UInt8 *dlMeasGapConfigType,
                           UInt8 *ulMeasGapConfigType,
                           InternalCellIndex internalCellIndex)
{
    createMeasGapOffsetMap(&(dwlkUEInfo_p->measGapConfig),
                             dlMeasGapContext,
                             ulMeasGapContext,
                             dwlkUEInfo_p->ueIndex,
                             internalCellIndex);
     dlMeasGapContext->measGapConfig = 
        dwlkUEInfo_p->measGapConfig;  

     *dlMeasGapConfigType = 
        dwlkUEInfo_p->measGapConfigType;
     *ulMeasGapConfigType = 
         *dlMeasGapConfigType;
     ulMeasGapContext->measGapConfig = 
         dlMeasGapContext->measGapConfig;
/*SPR 16855 +-*/
     /* SPR 16102 Changes Start */
     /* SPR 16994 changes - START*/
     setSPSMeasGapTTI(dlMeasGapContext,ulMeasGapContext,dwlkUEInfo_p->ueIndex,internalCellIndex);
     /* SPR 16994 changes - END*/
     /* SPR 16102 Changes End */
}


/*****************************************************************************
 * Function Name  : processMeasGapReConfig 
 * Inputs         : dwlkUEInfo_p,
 *                  dlMeasGapContext - pointer to MeasGapContextDL,
 *                  ulMeasGapContext - pointer to MeasGapContextUL,
 *                  dlMeasGapConfigType,
 *                  ulMeasGapConfigType,
 *                  ueIndex - Index of UE,
 *                  gapPatternId - pattern(GP0 or GP1),
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : gapBitMask - bitmask to reconfgure passive context
 * Returns        : None
 * Description    : This function checks the requested configuration and
 *                  accordingly deletes the old context and creates fresh MAP 
 *                  or if the request is not for setup, it just deletes the 
 *                  old context.                    
 *****************************************************************************/
/*SPR 16855 +-*/
void processMeasGapReConfig (UeContextForDownlink *dwlkUEInfo_p,
                             MeasGapContextDL *dlMeasGapContext,
                             MeasGapContextUL *ulMeasGapContext,
                             UInt8 *dlMeasGapConfigType,
                             UInt8 *ulMeasGapConfigType, 
                             UInt16 ueIndex,
                             UInt8 gapPatternId ,
                             UInt8 *gapBitMask,
                             /*CA Changes start */
                             InternalCellIndex internalCellIndex)
                             /*CA Changes end */
/*SPR 16855 +-*/
{
    if (SETUP == dwlkUEInfo_p->measGapConfigType)
    {  
    /*SPR 16855 +-*/        
        if ((SETUP == *dlMeasGapConfigType)|| (*gapBitMask == TRUE))
        {
            /* clear old context if at all any*/
            deleteUEMeasGapConfig(dlMeasGapContext,ulMeasGapContext,
                  /*CA Changes start  */
                  dlMeasGapConfigType,ulMeasGapConfigType,ueIndex,gapPatternId,internalCellIndex);
                  /*CA Changes end  */
            *gapBitMask = TRUE;
        }

        processMeasGapConfig(dwlkUEInfo_p,dlMeasGapContext,
                ulMeasGapContext,dlMeasGapConfigType,
                ulMeasGapConfigType, internalCellIndex);
    /*SPR 16855 +-*/
    }
    else
    {
        /*SPR 16855 +-*/
        if ((SETUP == *dlMeasGapConfigType)|| (*gapBitMask == TRUE))
        {
            /* clear old context if at all any*/
            deleteUEMeasGapConfig(dlMeasGapContext,ulMeasGapContext,
                /*CA Changes start  */
                dlMeasGapConfigType,ulMeasGapConfigType,ueIndex,gapPatternId,internalCellIndex);
                /*CA Changes end  */
            *gapBitMask = TRUE;
            *dlMeasGapConfigType = RELEASE;
            *ulMeasGapConfigType = RELEASE;
            /* Review comment fix start GG2 */
            dlMeasGapContext->isMeasGapPeriodOn = FALSE;
            ulMeasGapContext->isMeasGapPeriodOn = FALSE;
            /* Review comment fix end GG2 */
        }
        /*SPR 16855 +-*/
    }
    /*SPR 16855 +-*/
    LOG_MAC_MSG(MAC_MEAS_EVENT_HANDLER_ID,LOGDEBUG,MAC_MEAS_HANDLER,
            getCurrentTick(),
            ueIndex,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"MAC_MEAS_RECONFIG_SUCCESS");
    /*SPR 16855 +-*/
}

/*****************************************************************************
 * Function Name  : initMeasGapMgr 
 * Inputs         : numOfCells - Number of cells configured
 * Outputs        : None 
 * Returns        : None
 * Description    : This function reserves memory for trigger queue and
 *                  intializes the MAP and the trigger queue.               
 *****************************************************************************/
/*CA Changes start  */
void initMeasGapMgr (UInt8 numOfCells)
/*CA Changes end  */
{
    UInt32 measGapMapIndex = 0;
    UInt8 cellCount = 0;

    /* Start 128 UE: Memory Changes */
    /* CA Changes start */
    for (cellCount = 0; cellCount < numOfCells; cellCount++)
    {    
        createMemPool(sizeof(MeasGapOnOffTriggerNode), (cellSpecificParams_g.\
                    cellConfigAndInitParams_p[cellCount]->initParams_p->macMaxNumOfUeSupported * 2));
    }
    /* CA Changes end */
    /* End 128 UE: Memory Changes */

    /*SPR 16855 +-*/
    UInt32 index = 0;
    for (cellCount = 0; cellCount < numOfCells; cellCount++)
    {        
        for( index = 0; index < MAX_NUM_MAPS; index++ )
        {
            for (measGapMapIndex = 0; measGapMapIndex < MEASUREMENT_GAP_PERIODICITY_40;
                measGapMapIndex++)
            {
                /*CA Changes start  */
                listInit(&measGap40MsMap_g[cellCount][index][measGapMapIndex]);
                /*CA Changes end  */
            }
            for (measGapMapIndex = 0; measGapMapIndex < MEASUREMENT_GAP_PERIODICITY_80;
                    measGapMapIndex++)
            {
                /*CA Changes start  */
                listInit(&measGap80MsMap_g[cellCount][index][measGapMapIndex]);
                /*CA Changes end  */
            }
#ifdef ENDC_ENABLED
			/*Meas_Gap_Changes_Start*/
            for (measGapMapIndex = 0; measGapMapIndex < MEASUREMENT_GAP_PERIODICITY_20;
                    measGapMapIndex++)
            {
                /*CA Changes start  */
                listInit(&measGap20MsMap_g[cellCount][index][measGapMapIndex]);
                /*CA Changes end  */
            }
            for (measGapMapIndex = 0; measGapMapIndex < MEASUREMENT_GAP_PERIODICITY_160;
                    measGapMapIndex++)
            {
                /*CA Changes start  */
                listInit(&measGap160MsMap_g[cellCount][index][measGapMapIndex]);
                /*CA Changes end  */
            }
            /*Meas_Gap_Changes_End*/
#endif
        }
    }        

    /*CA Changes start  */
    /* Create pool for both measGap40msInfo_g and measGap80msInfo_g */
    createMemPool(sizeof(mapInfo), numOfCells * 2);

    for (cellCount = 0; cellCount < numOfCells; cellCount++)
    {
        GET_MEM_FROM_POOL(mapInfo, measGap40msInfo_gp[cellCount], sizeof(mapInfo), PNULL);
        GET_MEM_FROM_POOL(mapInfo, measGap80msInfo_gp[cellCount], sizeof(mapInfo), PNULL);
    
        measGap80msInfo_gp[cellCount]->passiveIndex = 0;
        measGap80msInfo_gp[cellCount]->activeIndex = 1;
        measGap40msInfo_gp[cellCount]->passiveIndex = 0;
        measGap40msInfo_gp[cellCount]->activeIndex =1;
#ifdef ENDC_ENABLED
		/*Meas_Gap_Changes_Start*/
	    GET_MEM_FROM_POOL(mapInfo, measGap20msInfo_gp[cellCount], sizeof(mapInfo), PNULL);
        GET_MEM_FROM_POOL(mapInfo, measGap160msInfo_gp[cellCount], sizeof(mapInfo), PNULL);

		measGap20msInfo_gp[cellCount]->passiveIndex = 0;
        measGap20msInfo_gp[cellCount]->activeIndex = 1;
        measGap160msInfo_gp[cellCount]->passiveIndex = 0;
        measGap160msInfo_gp[cellCount]->activeIndex =1;
        /*Meas_Gap_Changes_End*/
#endif

    }
    /*CA Changes end  */

  /*SPR 16855 +-*/
    LOG_MAC_MSG(MAC_MEAS_DRX_INITIALISATION_ID, LOGINFO,
            MAC_MEAS_HANDLER, getCurrentTick(),DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            __func__,"MEAS_INIT_SUCCESS");
    return;
}

/*****************************************************************************
 * Function Name  : isTTIInMeasGapPeriod 
 * Inputs         : ulUEContext_p - pointer to UL UE context,
 *                  tti - TTI to check if it is in measurement gap period
 * Outputs        : None 
 * Returns        : TRUE/ FALSE
 * Description    : This function checks if the TTI passed as input lies in 
 *                  measurement gap period or not
*****************************************************************************/
/* SPR 15909 fix start */
 UInt8 isTTIInMeasGapPeriod(ULUEContext *ulUEContext_p, tickType_t tti)
/* SPR 15909 fix end */
{

    if (RELEASE == ulUEContext_p->measGapConfigType)
    {
        return FALSE;
    }

    if (tti >= ulUEContext_p->measGapContext.ulMeasGapNextStartTTI && 
            tti < ulUEContext_p->measGapContext.ulMeasGapNextEndTTI)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/* + SPS_TDD_Changes */
/****************************************************************************
 * Function Name  :  setSPSMeasGapTTI 
 * Inputs         :  newUEcontext_p - Pointer to UE context 
 * Output         :  None
 * Returns        :  None
 * Description    :  This function determine the SPS Measurement Gap TTI for this UE to 
 *                   be used by RM
 ******************************************************************************/
/* SPR 16994 changes - START*/
void setSPSMeasGapTTI( MeasGapContextDL *dlMeasGapContext_p,
                              MeasGapContextUL *ulMeasGapContext_p,
                              UInt16 ueIndex,
                              InternalCellIndex internalCellIndex)
	/* SPR 16994 changes - END*/
{
    UInt8 measGapStartOffsetDL =0;
    UInt8 measGapStartOffsetUL =0;
    UInt8 measGapEndOffsetDL =0;
    UInt8 measGapEndOffsetUL =0;
    /* SPR 19038 Fix Start */
#ifdef FDD_CONFIG
    UInt8 bundleIndex       = 0;
    UInt8 numOfTicksToCheck = 1;
    /* ++ SPR 19287 */
    UInt8 measOffsetForDCI0 = 0;
    /* -- SPR 19287 */
    /* + SPR 21535 fix */
    UInt8  spsPeriodicity  = 0;
    UInt8  spsIndex        = 0;
    UInt8  spsIndex20        = 0;
    LP_SpsUlInfo    spsUlInfo_p  = PNULL;
    /* - SPR 21535 fix */
#endif
    /* SPR 19038 Fix End */
    /* SPR 16994 changes - START*/
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext     *dlUEContext_p = PNULL;
    ULUEContextInfo   *ulUEContextInfo_p  = PNULL;
    ULUEContext     *ulUEContext_p = PNULL;
    /* + SPR_16082 */
    /* + SPS_TDD_Changes */
    UInt32 passiveIdx = 0;
    if (GP0 == dlMeasGapContext_p->measGapConfig.gapPatternId
#ifdef ENDC_ENABLED
		/*Meas_Gap_Changes_Start*/
    	||(GP7_R15 == dlMeasGapContext_p->measGapConfig.gapPatternId)
    	/*Meas_Gap_Changes_End*/
#endif

    	)
    {
        passiveIdx = measGap40msInfo_gp[internalCellIndex]->passiveIndex;
    }
#ifdef ENDC_ENABLED
	/*Meas_Gap_Changes_Start*/
	else if(GP4_R15 == dlMeasGapContext_p->measGapConfig.gapPatternId 
		|| GP6_R15 == dlMeasGapContext_p->measGapConfig.gapPatternId
		|| GP10_R15 == dlMeasGapContext_p->measGapConfig.gapPatternId)
	{
        passiveIdx = measGap20msInfo_gp[internalCellIndex]->passiveIndex;
	}
	else if(GP5_R15 == dlMeasGapContext_p->measGapConfig.gapPatternId 
		|| GP9_R15 == dlMeasGapContext_p->measGapConfig.gapPatternId
		|| GP11_R15 == dlMeasGapContext_p->measGapConfig.gapPatternId)
	{
        passiveIdx = measGap160msInfo_gp[internalCellIndex]->passiveIndex;
	}
	/*Meas_Gap_Changes_End*/
#endif
    else
    {
        passiveIdx = measGap80msInfo_gp[internalCellIndex]->passiveIndex;
    } 
    measGapStartOffsetDL = dlMeasGapContext_p->measGapStartOffsetDL[passiveIdx];

    measGapStartOffsetUL = ulMeasGapContext_p->measGapStartOffsetUL[passiveIdx];

    measGapEndOffsetDL  = dlMeasGapContext_p->measGapEndOffsetDL[passiveIdx];
    measGapEndOffsetUL   = ulMeasGapContext_p->measGapEndOffsetUL[passiveIdx];
    /* - SPS_TDD_Changes */
    /* - SPR_16082 */

    dlUEContextInfo_p = 
        &dlUECtxInfoArr_g[ueIndex];
    dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
    ulUEContextInfo_p = 
        &ulUECtxInfoArr_g[ueIndex];
    ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
    /* SPR 16994 changes - END*/
    if (measGapStartOffsetDL > measGapEndOffsetDL)
    {
        while(measGapStartOffsetDL < (ulMeasGapContext_p->measGapPeriodicity + 
                    measGapEndOffsetDL))
        {
            dlUEContext_p->spsDlInfo.ueMeasGapTTI[measGapStartOffsetDL % MAX_SPS_INTERVAL] = MEAS_GAP_ON;
            measGapStartOffsetDL++;
        }
    }
    else
    {
        while(measGapStartOffsetDL < measGapEndOffsetDL)
        {
            dlUEContext_p->spsDlInfo.ueMeasGapTTI[measGapStartOffsetDL % MAX_SPS_INTERVAL] = MEAS_GAP_ON;
            measGapStartOffsetDL++;
        }
    }

    if (measGapStartOffsetUL > measGapEndOffsetUL)
    {
        while(measGapStartOffsetUL < (ulMeasGapContext_p->measGapPeriodicity + 
                    measGapEndOffsetUL))
        {
#ifdef FDD_CONFIG
            /* ++ SPR 19287 */
            /* Fill first four ticks(actually not in measGap period) with MEAS_GAP_ON_FOR_DCI0*/
            if(measOffsetForDCI0 < NUM_OF_MEASGAP_TICKS_FOR_DCI0)
            {
                ulUEContext_p->spsUlInfo.ueMeasGapTTI[measGapStartOffsetUL % MAX_SPS_INTERVAL] = MEAS_GAP_ON_FOR_DCI0 ;
                measOffsetForDCI0++;
            }
            else
            {
            /* -- SPR 19287 */
            ulUEContext_p->spsUlInfo.ueMeasGapTTI[measGapStartOffsetUL % MAX_SPS_INTERVAL] = MEAS_GAP_ON;
            }
#else
            ulUEContext_p->spsUlInfo.ueMeasGapTTI[measGapStartOffsetUL % MAX_UL_SPS_INTERVAL] = MEAS_GAP_ON;
            if (ulMeasGapContext_p->measGapPeriodicity != MEASUREMENT_GAP_PERIODICITY_80)
            {
                ulUEContext_p->spsUlInfo.ueMeasGapTTI[(measGapStartOffsetUL + 
                 ulMeasGapContext_p->measGapPeriodicity) % MAX_UL_SPS_INTERVAL] = MEAS_GAP_ON;
            }
#endif
            measGapStartOffsetUL++;
        }
    }
    else
    {
        while(measGapStartOffsetUL < measGapEndOffsetUL)
        {
#ifdef FDD_CONFIG
            /* ++ SPR 19287 */
            /* Fill first four ticks(actually not in measGap period) with MEAS_GAP_ON_FOR_DCI0*/
            if(measOffsetForDCI0 < NUM_OF_MEASGAP_TICKS_FOR_DCI0)
            {
                ulUEContext_p->spsUlInfo.ueMeasGapTTI[measGapStartOffsetUL % MAX_SPS_INTERVAL] = MEAS_GAP_ON_FOR_DCI0 ;
                measOffsetForDCI0++;
            }
            else
            {
            /* -- SPR 19287 */
            ulUEContext_p->spsUlInfo.ueMeasGapTTI[measGapStartOffsetUL % MAX_SPS_INTERVAL] = MEAS_GAP_ON;
            }
#else
            ulUEContext_p->spsUlInfo.ueMeasGapTTI[measGapStartOffsetUL % MAX_UL_SPS_INTERVAL] = MEAS_GAP_ON;
            if (ulMeasGapContext_p->measGapPeriodicity != MEASUREMENT_GAP_PERIODICITY_80)
            {
                ulUEContext_p->spsUlInfo.ueMeasGapTTI[(measGapStartOffsetUL + 
                 ulMeasGapContext_p->measGapPeriodicity) % MAX_UL_SPS_INTERVAL] = MEAS_GAP_ON;
            }
#endif
            measGapStartOffsetUL++;
        }
    }
    /* SPR 19038 Fix Start */
    /* Check If after reconfiguration of meas gap 
     * sps activation tti falls on Meas gap 
     * If yes then set the isUeMeasGapCollisionWithSpsOcc variable 
     * To true so that deactivation process can be started */
    if ( (MEAS_GAP_ON == dlUEContext_p->spsDlInfo.ueMeasGapTTI
                [dlUEContext_p->spsDlInfo.ttiIndex]) &&
            ((SPS_ACTIVATION_INITIATED == dlUEContext_p->spsDlInfo.spsState) ||
             (SPS_ACTIVATED == dlUEContext_p->spsDlInfo.spsState)))
    {
        dlUEContext_p->spsDlInfo.isUeMeasGapCollisionWithSpsOcc = LTE_TRUE;
    }

    /* Below code will handle the meas gap collison with sps occasion 
     * If Meas gap is reconfigured after sps activation in uplink
     * If tti bundling is enabled then It also checks 
     * If the tti bundled occasions are also not colliding 
     * with meas gap 
     */
#ifdef FDD_CONFIG
    if (ulUEContext_p->ttiBundlingEnabled)
    {
        numOfTicksToCheck = TTI_BUNDLE_SIZE;
    }

    if ((SPS_ACTIVATION_INITIATED == ulUEContext_p->spsUlInfo.spsState) ||
            (SPS_ACTIVATED == ulUEContext_p->spsUlInfo.spsState))
    {
        /* + SPR 21535 fix */
        spsUlInfo_p = &ulUEContext_p->spsUlInfo;
        spsPeriodicity = spsIntervalMap_g[spsUlInfo_p->spsUlSetupInfo_p->semiPersistentSchedIntervalUL];
        /* If TTI Bundling is on 
           check whether last 3 ticks of bundle  lie in MeasGap, 
           if it does then SPS deactivation- activation will take place. 
           same must be checked for periodicity 20 if SPS periodicity is 20*/
        if(!ulUEContext_p->ttiBundlingEnabled)
        {
            spsIndex = (ulUEContext_p->spsUlInfo.actTtiIndex + FDD_HARQ_OFFSET)% MAX_SPS_INTERVAL ;
            spsIndex20 = (ulUEContext_p->spsUlInfo.actTtiIndex + FDD_HARQ_OFFSET + SPS_INTERVAL_20 )% MAX_SPS_INTERVAL ;

            if(MEAS_GAP_OFF == ulUEContext_p->spsUlInfo.ueMeasGapTTI[spsIndex])
            {
                if((SPS_INTERVAL_20 == spsPeriodicity ) &&( MEAS_GAP_ON == ulUEContext_p->spsUlInfo.ueMeasGapTTI[spsIndex20]) )
                {
                    ulUEContext_p->spsUlInfo.isUeMeasGapCollisionWithSpsOcc = LTE_TRUE;
                }
            }
            else if(MEAS_GAP_ON == ulUEContext_p->spsUlInfo.ueMeasGapTTI[spsIndex])
            {
                ulUEContext_p->spsUlInfo.isUeMeasGapCollisionWithSpsOcc = LTE_TRUE;
            }
            else /*MEAS_GAP_ON_FOR_DCI0 -- can be deleted later*/
            {
                //do nothing
            }
        }
        else
        {
            for (bundleIndex = 1; bundleIndex < numOfTicksToCheck; bundleIndex++)
            {
                spsIndex = (ulUEContext_p->spsUlInfo.actTtiIndex + bundleIndex + FDD_HARQ_OFFSET)% MAX_SPS_INTERVAL ; 
                spsIndex20 = (ulUEContext_p->spsUlInfo.actTtiIndex + bundleIndex + FDD_HARQ_OFFSET + SPS_INTERVAL_20)% MAX_SPS_INTERVAL ;

                if((MEAS_GAP_ON == ulUEContext_p->spsUlInfo.ueMeasGapTTI[spsIndex])  || 
                        (   (SPS_INTERVAL_20 == spsPeriodicity ) && 
                            (MEAS_GAP_ON == ulUEContext_p->spsUlInfo.ueMeasGapTTI[spsIndex20])) )
                {
                    ulUEContext_p->spsUlInfo.isUeMeasGapCollisionWithSpsOcc = LTE_TRUE;
                    break;
                }

            }
        }
        /* - SPR 21535 fix */
    }
#endif
    /* SPR 19038 Fix End */
}
/****************************************************************************
 * Function Name  :  resetSPSMeasGapTTI 
 * Inputs         :  newUEcontext_p - Pointer to UE context 
 * Output         :  None
 * Returns        :  None
 * Description    :  This function determine the SPS Measurement Gap TTI for this UE to 
 *                   be used by RM
 ******************************************************************************/
STATIC void resetSPSMeasGapTTI( MeasGapContextDL *dlMeasGapContext_p,
                              MeasGapContextUL *ulMeasGapContext_p,
                              /* SPR 16994 changes - START */
                              UInt16 ueIndex,
                              UInt8 gapPatternId,
                              InternalCellIndex internalCellIndex)
/* SPR 16994 changes - END */
{
    UInt8 measGapStartOffsetDL =0;
    UInt8 measGapStartOffsetUL =0;
    UInt8 measGapEndOffsetDL =0;
    UInt8 measGapEndOffsetUL =0;
    DLUEContextInfo *dlUEContextInfo_p = PNULL;
    DLUEContext     *dlUEContext_p = PNULL;
    ULUEContextInfo   *ulUEContextInfo_p  = PNULL;
    ULUEContext     *ulUEContext_p = PNULL;

    /* + SPR_16082 */
    UInt32 passiveIdx = 0;
    /* SPR 16994 changes - START */
    if (GP0 == gapPatternId
#ifdef ENDC_ENABLED
		/*Meas_Gap_Changes_Start*/
    	|| GP7_R15 == gapPatternId
    	/*Meas_Gap_Changes_End*/
#endif
    	)
    /* SPR 16994 changes - END */
    {
        /* +SPS_TDD_CHANGE*/
        /* SPR 16994 changes - START */
        passiveIdx = measGap40msInfo_gp[internalCellIndex]->passiveIndex;
        /* SPR 16994 changes - END */
        /* -SPS_TDD_CHANGE*/
    }
#ifdef ENDC_ENABLED
	/*Meas_Gap_Changes_Start*/
	else if(GP4_R15 == gapPatternId || GP6_R15 == gapPatternId
		|| GP10_R15 == gapPatternId)
	{
		passiveIdx = measGap20msInfo_gp[internalCellIndex]->passiveIndex;
	}
	else if(GP5_R15 == gapPatternId || GP9_R15 == gapPatternId
		|| GP11_R15 == gapPatternId)
	{
		passiveIdx = measGap160msInfo_gp[internalCellIndex]->passiveIndex;
	}
	/*Meas_Gap_Changes_End*/
#endif
    else
    {
        /* +SPS_TDD_CHANGE*/
        /* SPR 16994 changes - START */
        passiveIdx = measGap80msInfo_gp[internalCellIndex]->passiveIndex;
        /* SPR 16994 changes - END */
        /* -SPS_TDD_CHANGE*/
    } 
    measGapStartOffsetDL = dlMeasGapContext_p->measGapStartOffsetDL[passiveIdx];
    measGapStartOffsetDL = dlMeasGapContext_p->measGapStartOffsetDL[passiveIdx];
    /* - SPR_16082 */

    measGapEndOffsetDL = dlMeasGapContext_p->measGapEndOffsetDL[passiveIdx];
    measGapEndOffsetUL = ulMeasGapContext_p->measGapEndOffsetUL[passiveIdx];

    dlUEContextInfo_p = 
        &dlUECtxInfoArr_g[ueIndex];
    dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;
    ulUEContextInfo_p = 
        &ulUECtxInfoArr_g[ueIndex];
    ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
    if (measGapStartOffsetDL > measGapEndOffsetDL)
    {
        while(measGapStartOffsetDL < (ulMeasGapContext_p->measGapPeriodicity + 
                    measGapEndOffsetDL))
        {
            dlUEContext_p->spsDlInfo.ueMeasGapTTI[measGapStartOffsetDL % MAX_SPS_INTERVAL] = MEAS_GAP_OFF;
            measGapStartOffsetDL++;
        }
    }
    else
    {
        while(measGapStartOffsetDL < measGapEndOffsetDL)
        {
            dlUEContext_p->spsDlInfo.ueMeasGapTTI[measGapStartOffsetDL % MAX_SPS_INTERVAL] = MEAS_GAP_OFF;
            measGapStartOffsetDL++;
        }
    }

    if (measGapStartOffsetUL > measGapEndOffsetUL)
    {
        while(measGapStartOffsetUL < (ulMeasGapContext_p->measGapPeriodicity + 
                    measGapEndOffsetUL))
        {
#ifdef FDD_CONFIG
            ulUEContext_p->spsUlInfo.ueMeasGapTTI[measGapStartOffsetUL % MAX_SPS_INTERVAL] = MEAS_GAP_OFF;
#else
            ulUEContext_p->spsUlInfo.ueMeasGapTTI[measGapStartOffsetUL % MAX_UL_SPS_INTERVAL] = MEAS_GAP_OFF;
            if (ulMeasGapContext_p->measGapPeriodicity != MEASUREMENT_GAP_PERIODICITY_80)
            {
                ulUEContext_p->spsUlInfo.ueMeasGapTTI[(measGapStartOffsetUL + 
                        ulMeasGapContext_p->measGapPeriodicity) % MAX_UL_SPS_INTERVAL] = MEAS_GAP_OFF;
            }
#endif
            measGapStartOffsetUL++;
        }
    }
    else
    {
        while(measGapStartOffsetUL < measGapEndOffsetUL)
        {
#ifdef FDD_CONFIG
            ulUEContext_p->spsUlInfo.ueMeasGapTTI[measGapStartOffsetUL % MAX_SPS_INTERVAL] = MEAS_GAP_OFF;
#else
            ulUEContext_p->spsUlInfo.ueMeasGapTTI[measGapStartOffsetUL % MAX_UL_SPS_INTERVAL] = MEAS_GAP_OFF;
            if (ulMeasGapContext_p->measGapPeriodicity != MEASUREMENT_GAP_PERIODICITY_80)
            {
                ulUEContext_p->spsUlInfo.ueMeasGapTTI[(measGapStartOffsetUL + 
                        ulMeasGapContext_p->measGapPeriodicity) % MAX_UL_SPS_INTERVAL] = MEAS_GAP_OFF;
            }
#endif
            measGapStartOffsetUL++;
        }
    }
}
/* - SPS_TDD_Changes */
