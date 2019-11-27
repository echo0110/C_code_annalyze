/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacPUCCH.c,v 1.1.1.1.6.1.20.2 2010/11/09 13:32:58 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains a Periodic Report Map which allocate
 *                     the resource for Periodic Reports (SR , CQI , RI)
 *                     Its also allocate the Resources for DL-ACK/Nack;
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacPUCCH.c,v $
 * Revision 1.1.1.1.6.1.20.2  2010/11/09 13:32:58  gur10121
 *  Semi static UT bug fixed
 *
 * Revision 1.1.1.1.6.1.20.1  2010/11/08 06:06:30  gur10121
 * Semi static changes merging
 *
 * Revision 1.1.1.1.6.1.14.3  2010/10/29 10:25:49  gur10121
 *  Semi Static UT fixes
 *
 * Revision 1.1.1.1.6.1.14.2  2010/10/27 17:13:58  gur10121
 * UT fixes + review comments incorporation
 *
 * Revision 1.1.1.1.6.1.12.3  2010/10/21 07:49:32  gur10121
 * semi static UT Fix
 *
 * Revision 1.1.1.1.6.1.12.2  2010/10/19 06:46:50  gur10121
 * Compilation changes checked in
 *
 * Revision 1.1.1.1.6.1.12.1  2010/10/15 18:53:18  gur10121
 * Semi staic Changesdone and checked in
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.38  2009/08/30 13:59:51  gur19413
 * removed warning.
 *
 * Revision 1.37  2009/08/23 13:45:25  gur19413
 * added UT_FLAG
 *
 * Revision 1.36  2009/08/20 07:24:30  gur19413
 * added new parameter in setFreeRbs fun
 *
 * Revision 1.35  2009/08/20 06:51:40  gur19413
 * added UL Stats functionality
 *
 * Revision 1.34  2009/07/30 14:02:13  gur19413
 * added new functionality to handle collision
 *
 * Revision 1.33  2009/07/29 13:59:39  gur19413
 * modify in logic of createPeriodicityMap.
 *
 * Revision 1.32  2009/07/29 05:52:34  gur19413
 * added return MAC_SUCCESS statement
 *
 * Revision 1.31  2009/07/28 00:18:23  gur19413
 * changed func return type
 *
 * Revision 1.30  2009/07/23 08:23:59  gur19413
 * customized the whole file.
*
* Revision 1.29  2009/07/21 13:11:16  gur19413
* added entry ue context in Reconfigure func
*
* Revision 1.28  2009/07/20 12:08:55  gur19413
* ut bug fixed
*
* Revision 1.27  2009/07/17 13:11:24  gur19413
* UT bugs fixed
*
* Revision 1.26  2009/07/16 07:39:59  gur19413
* added deletingReference_p & collisonReference_p default set NULL.
*
* Revision 1.25  2009/07/15 15:38:51  gur19413
* ut bugs fixed
*
* Revision 1.24  2009/07/15 13:15:17  gur19413
* changed in collision dl ack/nack with pusch
*
* Revision 1.23  2009/07/14 07:12:18  gur19413
* removed handling subband
*
* Revision 1.21  2009/07/13 04:14:38  gur19413
* change log type and UT bugs fixed
*
* Revision 1.19  2009/07/07 09:31:48  gur19413
* modify the slot number
*
* Revision 1.18  2009/07/03 11:37:18  gur19413
* added &
*
* Revision 1.17  2009/07/03 11:30:50  gur19413
* updated processPUCCHAndPRACH
*
* Revision 1.16  2009/07/03 09:56:12  gur19413
* system frame offset modify
*
* Revision 1.15  2009/07/02 08:14:41  gur19413
* changed in processPUCCHAndPRACHReports
*
* Revision 1.14  2009/07/01 05:47:47  gur19413
* function prototype change processPUCCHAndPRACHReports
*
* Revision 1.13  2009/06/23 23:03:02  gur19413
* change function prototype initSR,initCQI
*
* Revision 1.12  2009/06/23 08:15:36  gur18569
* added checks not to call setFreeRbsAndPUCCHFlag
*
* Revision 1.11  2009/06/22 06:13:21  gur19413
* prototype change reConfigCQIAndRI
*
* Revision 1.10  2009/06/15 07:58:28  gur19413
* added new paramter in updatePUCCHConfg
*
* Revision 1.9  2009/06/10 11:53:51  gur19413
* comments incorporated
*
* Revision 1.8  2009/06/08 12:15:46  gur19413
* setFree function add new argument
*
* Revision 1.7  2009/06/05 06:57:10  gur19413
* Removed Compilation Errors
*
* Revision 1.6  2009/06/05 06:51:32  gur19413
* Removed Compilation Errors
*
* Revision 1.5  2009/06/02 05:19:35  gur19413
* added comments
*
* Revision 1.4  2009/06/02 05:13:37  gur19413
* added new lines
*
* Revision 1.3  2009/06/02 04:44:48  gur19413
* added new macro
*
* Revision 1.2  2009/06/01 22:44:30  gur19413
* added new function
*
* Revision 1.1  2009/04/30 12:17:34  gur19413
* Initial Version
*
*
*****************************************************************************/
/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/

#include "lteMacPUCCH.h"
#include "lteMacCellConfMgr.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacULResourceAllocator.h"
#include "lteMacDLUEContext.h"
#include "lteMacULUEContext.h"
#include "lteMacPUSCH.h"
#include "lteMacRandomAccess.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacMeasGapHandler.h" 
#include "lteMacULResourceInfoMgr.h"
#include "lteMacPRACH.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#include "lteMacTddMgr.h"
#endif
#ifdef FDD_CONFIG
#include "ltePerfStats.h"
#endif
/******************************************************************************
  Private Definitions
 *****************************************************************************/
#define MAX_P_VAL 4
#define INVALID_SMALL_M 255
#define MAX_PUCCH_ACK_NACK_RESOURCES 4
#define MAX_TICK_VAL 10240
#define LTE_MAC_SS_TDD_CALC_HARQ_SIZE(pucchFormat) UCI_FORMAT_ONE_A == (pucchFormat)?1:2
#define CHECK_BIT_SET_OTHER_THAN(str, bitPos)  ((str) & ~(1 << (bitPos) ))
/* OPT */
#define MAX_SUBFRAME_OFFSET 160
/* OPT */
/* Rel9_upgrade_CR257 */
#define SHORT_SR_PERIODICITY 2
/* Rel9_upgrade_CR257 */
/*TDD SB_CQI*/
#define UPDATE_BANDWIDTHPART_ON_WIDEBAND_REPORT(ulUEContext_p,sfn,sf)\
{\
    UInt32 nextCQISFN = sfn;\
    UInt32  nextCQISF = sf;\
    /* SPR 7799 Fix Start */ \
    UInt32  nextCQIOccurance = 0;\
    /* SPR 7799 Fix End */ \
    ulUEContext_p->reportedBandwidthPartNum =0; \
    \
    /* Updating next wideband occurance*/\
    GET_REQUIRED_TICK(nextCQISFN,\
            nextCQISF,\
            ulUEContext_p->\
            pucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity);\
    ulUEContext_p->nextWidebandOccurrence = ((nextCQISFN * 10) + nextCQISF);\
    \
    /* Updating next subband occurance*/\
    nextCQISFN = sfn;\
    nextCQISF = sf;\
    GET_REQUIRED_TICK(nextCQISFN,\
            nextCQISF,\
            ulUEContext_p->pucchConfigInfo.cqiPeriodicity);\
    \
    /* SPR 7799 Fix Start */ \
    nextCQIOccurance = ((nextCQISFN * 10) + nextCQISF);\
    /* Check for wrap around condition.If it is, then next cqi report\
     * will be Wideband and bandwidthpart starts from 0*/\
    if(ulUEContext_p->nextSubbandOccurrence > nextCQIOccurance) \
    { \
        ulUEContext_p->nextWidebandOccurrence = nextCQIOccurance; \
        ulUEContext_p->nextSubbandOccurrence = nextCQIOccurance; \
        ulUEContext_p->reportedBandwidthPartNum = 0; \
    } \
    else\
    {\
        ulUEContext_p->nextSubbandOccurrence = nextCQIOccurance;\
    }\
    /* SPR 7799 Fix End */\
}


#define UPDATE_BANDWIDTHPART_ON_SUBBAND_REPORT(ulUEContext_p,sfn,sf,internalCellIndex)\
{\
    UInt32 nextCQISFN = sfn;\
    UInt32  nextCQISF = sf;\
    UInt32  nextCQIOccurance = 0;\
    \
    /* Updating bandwidthpart*/ \
    ulUEContext_p->subframeToBandwidthPartNo[sf]  = \
    ulUEContext_p->reportedBandwidthPartNum;\
    ulUEContext_p->reportedBandwidthPartNum++;\
    if(ulUEContext_p->reportedBandwidthPartNum >=\
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->bandwidthParts)\
    {\
        ulUEContext_p->reportedBandwidthPartNum = 0;\
    }\
    \
    GET_REQUIRED_TICK(nextCQISFN,\
            nextCQISF,\
            ulUEContext_p->pucchConfigInfo.cqiPeriodicity);\
    nextCQIOccurance = ((nextCQISFN * 10) + nextCQISF);\
    \
    /* Check for wrap around condition.If it is, then next cqi report\
     * will be Wideband and bandwidthpart starts from 0*/ \
    if(ulUEContext_p->nextSubbandOccurrence > nextCQIOccurance)\
    { \
        ulUEContext_p->nextWidebandOccurrence = nextCQIOccurance; \
        ulUEContext_p->reportedBandwidthPartNum = 0;\
    } \
    else\
    {\
        ulUEContext_p->nextSubbandOccurrence = nextCQIOccurance;\
        \
    }\
}
/*TDD SB_CQI*/

/* EICIC +*/
#define UPDATE_BANDWIDTHPART_ON_WIDEBAND_REPORT_2(ulUEContext_p,sfn,sf)\
{\
    UInt32 nextCQISFN = sfn;\
    UInt32  nextCQISF = sf;\
    /* SPR 7799 Fix Start */ \
    UInt32  nextCQIOccurance = 0;\
    /* SPR 7799 Fix End */ \
    ulUEContext_p->reportedBandwidthPartNum_2 =0; \
    \
    /* Updating next wideband occurance*/\
    GET_REQUIRED_TICK(nextCQISFN,\
            nextCQISF,\
            ulUEContext_p->\
            pucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity_2);\
    ulUEContext_p->nextWidebandOccurrence_2 = ((nextCQISFN * 10) + nextCQISF);\
    \
    /* Updating next subband occurance*/\
    nextCQISFN = sfn;\
    nextCQISF = sf;\
    GET_REQUIRED_TICK(nextCQISFN,\
            nextCQISF,\
            ulUEContext_p->pucchConfigInfo.cqiPeriodicity_2);\
    \
    /* SPR 7799 Fix Start */ \
    nextCQIOccurance = ((nextCQISFN * 10) + nextCQISF);\
    /* Check for wrap around condition.If it is, then next cqi report\
     * will be Wideband and bandwidthpart starts from 0*/\
    if(ulUEContext_p->nextSubbandOccurrence_2 > nextCQIOccurance) \
    { \
        ulUEContext_p->nextWidebandOccurrence_2 = nextCQIOccurance; \
        ulUEContext_p->nextSubbandOccurrence_2 = nextCQIOccurance; \
        ulUEContext_p->reportedBandwidthPartNum_2 = 0; \
    } \
    else\
    {\
        ulUEContext_p->nextSubbandOccurrence_2 = nextCQIOccurance;\
    }\
    /* SPR 7799 Fix End */\
}


#define UPDATE_BANDWIDTHPART_ON_SUBBAND_REPORT_2(ulUEContext_p,sfn,sf,internalCellIndex)\
{\
    UInt32 nextCQISFN = sfn;\
    UInt32  nextCQISF = sf;\
    UInt32  nextCQIOccurance = 0;\
    \
    /* Updating bandwidthpart*/ \
    ulUEContext_p->subframeToBandwidthPartNo_2[sf]  = \
    ulUEContext_p->reportedBandwidthPartNum_2;\
    ulUEContext_p->reportedBandwidthPartNum_2++;\
    if(ulUEContext_p->reportedBandwidthPartNum_2 >=\
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->bandwidthParts)\
    {\
        ulUEContext_p->reportedBandwidthPartNum_2 = 0;\
    }\
    \
    GET_REQUIRED_TICK(nextCQISFN,\
            nextCQISF,\
            ulUEContext_p->pucchConfigInfo.cqiPeriodicity_2);\
    nextCQIOccurance = ((nextCQISFN * 10) + nextCQISF);\
    \
    /* Check for wrap around condition.If it is, then next cqi report\
     * will be Wideband and bandwidthpart starts from 0*/ \
    if(ulUEContext_p->nextSubbandOccurrence_2 > nextCQIOccurance)\
    { \
        ulUEContext_p->nextWidebandOccurrence_2 = nextCQIOccurance; \
        ulUEContext_p->reportedBandwidthPartNum_2 = 0;\
    } \
    else\
    {\
        ulUEContext_p->nextSubbandOccurrence_2 = nextCQIOccurance;\
        \
    }\
}
/*TDD SB_CQI*/

/* EICIC -*/

/*TDD SB_CQI*/
#define UPDATE_SCELL_BANDWIDTHPART_ON_WIDEBAND_REPORT(ulUEContext_p,sfn,sf,servCellIndex)\
{\
    UInt32 nextCQISFN = sfn;\
    UInt32  nextCQISF = sf;\
    /* SPR 7799 Fix Start */ \
    UInt32  nextCQIOccurance = 0;\
    /* SPR 7799 Fix End */ \
    ulUEContext_p->ulUeScellContext_p[servCellIndex]->reportedBandwidthPartNum =0; \
    \
    /* Updating next wideband occurance*/\
    GET_REQUIRED_TICK(nextCQISFN,\
            nextCQISF,\
            ulUEContext_p->ulUeScellContext_p[servCellIndex]->\
            scellpucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity);\
    ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextWidebandOccurrence = ((nextCQISFN * 10) + nextCQISF);\
    \
    /* Updating next subband occurance*/\
    nextCQISFN = sfn;\
    nextCQISF = sf;\
    GET_REQUIRED_TICK(nextCQISFN,\
            nextCQISF,\
            ulUEContext_p->ulUeScellContext_p[servCellIndex]->scellpucchConfigInfo.cqiPeriodicity);\
    \
    /* SPR 7799 Fix Start */ \
    nextCQIOccurance = ((nextCQISFN * 10) + nextCQISF);\
    /* Check for wrap around condition.If it is, then next cqi report\
     * will be Wideband and bandwidthpart starts from 0*/\
    if(ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextSubbandOccurrence > nextCQIOccurance) \
    { \
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextWidebandOccurrence = nextCQIOccurance; \
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextSubbandOccurrence = nextCQIOccurance; \
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->reportedBandwidthPartNum = 0; \
    } \
    else\
    {\
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextSubbandOccurrence = nextCQIOccurance;\
    }\
    /* SPR 7799 Fix End */\
}


#define UPDATE_SCELL_BANDWIDTHPART_ON_SUBBAND_REPORT(ulUEContext_p,sfn,sf,servCellIndex,internalCellIndex)\
{\
    UInt32 nextCQISFN = sfn;\
    UInt32  nextCQISF = sf;\
    UInt32  nextCQIOccurance = 0;\
    \
    /* Updating bandwidthpart*/ \
    ulUEContext_p->ulUeScellContext_p[servCellIndex]->subframeToBandwidthPartNo[sf]  = \
    ulUEContext_p->ulUeScellContext_p[servCellIndex]->reportedBandwidthPartNum;\
    ulUEContext_p->ulUeScellContext_p[servCellIndex]->reportedBandwidthPartNum++;\
    if(ulUEContext_p->reportedBandwidthPartNum >=\
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->bandwidthParts)\
    {\
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->reportedBandwidthPartNum = 0;\
    }\
    \
    GET_REQUIRED_TICK(nextCQISFN,\
            nextCQISF,\
            ulUEContext_p->ulUeScellContext_p[servCellIndex]->scellpucchConfigInfo.cqiPeriodicity);\
    nextCQIOccurance = ((nextCQISFN * 10) + nextCQISF);\
    \
    /* Check for wrap around condition.If it is, then next cqi report\
     * will be Wideband and bandwidthpart starts from 0*/ \
    if(ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextSubbandOccurrence > nextCQIOccurance)\
    { \
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextWidebandOccurrence = nextCQIOccurance; \
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->reportedBandwidthPartNum = 0;\
    } \
    else\
    {\
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextSubbandOccurrence = nextCQIOccurance;\
        \
    }\
}

/* EICIC +*/
#define UPDATE_SCELL_BANDWIDTHPART_ON_WIDEBAND_REPORT_2(ulUEContext_p,sfn,sf,servCellIndex)\
{\
    UInt32 nextCQISFN = sfn;\
    UInt32  nextCQISF = sf;\
    /* SPR 7799 Fix Start */ \
    UInt32  nextCQIOccurance = 0;\
    /* SPR 7799 Fix End */ \
    ulUEContext_p->ulUeScellContext_p[servCellIndex]->reportedBandwidthPartNum_2 =0; \
    \
    /* Updating next wideband occurance*/\
    GET_REQUIRED_TICK(nextCQISFN,\
            nextCQISF,\
            ulUEContext_p->ulUeScellContext_p[servCellIndex]->\
            scellpucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity_2);\
    ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextWidebandOccurrence_2 = ((nextCQISFN * 10) + nextCQISF);\
    \
    /* Updating next subband occurance*/\
    nextCQISFN = sfn;\
    nextCQISF = sf;\
    GET_REQUIRED_TICK(nextCQISFN,\
            nextCQISF,\
            ulUEContext_p->ulUeScellContext_p[servCellIndex]->scellpucchConfigInfo.cqiPeriodicity_2);\
    \
    /* SPR 7799 Fix Start */ \
    nextCQIOccurance = ((nextCQISFN * 10) + nextCQISF);\
    /* Check for wrap around condition.If it is, then next cqi report\
     * will be Wideband and bandwidthpart starts from 0*/\
    if(ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextSubbandOccurrence_2 > nextCQIOccurance) \
    { \
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextWidebandOccurrence_2 = nextCQIOccurance; \
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextSubbandOccurrence_2 = nextCQIOccurance; \
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->reportedBandwidthPartNum_2 = 0; \
    } \
    else\
    {\
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextSubbandOccurrence_2 = nextCQIOccurance;\
    }\
    /* SPR 7799 Fix End */\
}


#define UPDATE_SCELL_BANDWIDTHPART_ON_SUBBAND_REPORT_2(ulUEContext_p,sfn,sf,servCellIndex,internalCellIndex)\
{\
    UInt32 nextCQISFN = sfn;\
    UInt32  nextCQISF = sf;\
    UInt32  nextCQIOccurance = 0;\
    \
    /* Updating bandwidthpart*/ \
    ulUEContext_p->ulUeScellContext_p[servCellIndex]->subframeToBandwidthPartNo_2[sf]  = \
    ulUEContext_p->ulUeScellContext_p[servCellIndex]->reportedBandwidthPartNum_2;\
    ulUEContext_p->ulUeScellContext_p[servCellIndex]->reportedBandwidthPartNum_2++;\
    if(ulUEContext_p->reportedBandwidthPartNum >=\
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->bandwidthParts)\
    {\
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->reportedBandwidthPartNum_2 = 0;\
    }\
    \
    GET_REQUIRED_TICK(nextCQISFN,\
            nextCQISF,\
            ulUEContext_p->ulUeScellContext_p[servCellIndex]->scellpucchConfigInfo.cqiPeriodicity_2);\
    nextCQIOccurance = ((nextCQISFN * 10) + nextCQISF);\
    \
    /* Check for wrap around condition.If it is, then next cqi report\
     * will be Wideband and bandwidthpart starts from 0*/ \
    if(ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextSubbandOccurrence_2 > nextCQIOccurance)\
    { \
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextWidebandOccurrence_2 = nextCQIOccurance; \
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->reportedBandwidthPartNum_2 = 0;\
    } \
    else\
    {\
        ulUEContext_p->ulUeScellContext_p[servCellIndex]->nextSubbandOccurrence_2 = nextCQIOccurance;\
        \
    }\
}

/* EICIC -*/

#define MAX_SC_RB 12
#ifdef FDD_CONFIG
/* Review comment fix start GG15 */
#define MAC_MIN_2VAR(A,B) \
    ((A) < (B) ) ? (A) : (B)
#define MAC_MAX_2VAR(A,B) \
    ((A) > (B) ) ? (A) : (B)
#define MAX_RB_FOR_BW_1_4                            6
#define MIN_PUCCH_RES_MSG3_SPS_FOR_1_4BW             2
#define MIN_PUCCH_RES_MSG3_SPS_FOR_OTHRE_THAN_1_4BW  4
/* Review comment fix end GG15 */
#endif              

/******************************************************************************
  Private Types
 *****************************************************************************/

#if (defined (DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)) 
SCELL_SCHEDULING_CIRC_Q dlSecondayCellScheulingQueue_g[MAX_NUM_CELL];
#else
LTE_SQUEUE dlSecondayCellScheulingQueue_g[MAX_NUM_CELL];
#endif

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

#ifdef FDD_CONFIG

void processDlSecondaryCellQueue(
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt32 subFrameNum,
        UInt32 rbMapNum,
        /* SPR 15909 fix start */
        tickType_t ttiWithDelay,
        /* SPR 15909 fix end */
        /** SPR 12457 Fix : Start**/
#ifdef PUCCH_RESERVE_ALWAYS
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif
        /** SPR 12457 Fix : End **/
        InternalCellIndex internalCellIndex
        );
#else
void processDlSecondaryCellQueue(
        /* +- SPR 17777 */
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt32 subFrameNum,
        UInt32 rbMapNum,
        /* SPR 15909 fix start */
        tickType_t ttiWithDelay,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex
        );
#endif

/* This function is used to reconfigure the PUCCH Resource Index only 
   then traverse the whole map and update the physical resouce block index. */
STATIC MacRetType traverseAndUpdatePUCCHResourceIndex( 
        /* Review comment fix start PUCCH_ENH_2 */
        PucchConfig *pucchConfigInfo,
        UInt32 reportType,
        /* Review comment fix end PUCCH_ENH_2 */
        UInt32 pucchFormat,
        UInt32 pucchResourceIndex,
        /*CA Changes start  */
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        );

 void updateBwPartOnWideBandSubBandReport(ULUEContext *ulUEContext_p,
        /* SPR 15909 fix start */
        tickType_t currentTTI,
        /* SPR 15909 fix end */
        UInt32 sysFrameNum,
        UInt32 subFrameNum,
        UeCellInfo * ueCellInfo_p,
        InternalCellIndex internalCellIndex
        );

#ifdef FDD_CONFIG
STATIC  void updateIsCQIMasked(ULUEContext *ulUEContext_p,
        /*14766 start*/
        DLUEContext *ueDLContext_p,
        /*14766 end */
        UInt8 *isCQIMasked,
        /* SPR 15909 fix start */
        tickType_t ttiWithDelay,
        /* SPR 15909 fix end */
        UInt8 ulUEScheduledFlag);
#endif
STATIC MacRetType updatePUCCHResourceIdxForShortSR(
        /* Review comment fix start PUCCH_ENH_2 */
        PucchConfig *pucchConfigInfo,
        UInt32 reportType,
        /* Review comment fix end PUCCH_ENH_2 */
        UInt32 pucchFormat,
        UInt32 pucchResourceIndex,
        InternalCellIndex internalCellIndex
        );
#ifdef FDD_CONFIG
STATIC void  processLongSrOrReport(
        UInt16 subFrameOffset,
        /* SPR 15909 fix start */
        tickType_t tti,
        /* SPR 15909 fix end */
        UInt32 subFrameNum,
        UInt32 sysFrameNum,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        UInt32 rbMapNum,
        UInt8 containerTick,
#ifdef PUCCH_RESERVE_ALWAYS        
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif        
        UInt8 cellIndex
        );
#else
STATIC void  processLongSrOrReport(
        UInt32 ulSubFrame,
        /* SPR 15909 fix start */
        tickType_t tti,
        /* SPR 15909 fix end */
        UInt32 subFrameNum,
        UInt32 sysFrameNum,
        /* +- SPR 17777 */
        UInt32 rbMapNum,
        UInt8 cellIndex
        );

#endif
/* SPR 11215 Changes Start */
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
STATIC MacRetType fillPucchHarqInfo(
        UInt16 ueIndex,
        ULSchUciInfo * ulschUciInfo_p,
        UInt32 nPucch,
        /** SPR 12457 Fix : Start**/
        UInt32 rbMapNum,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif
        /* +- SPR 17777 */
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        /* +- SPR 17777 */
#else
        UInt8 pCellTb,
#endif
        /** SPR 12457 Fix : End **/
        UInt32 subFrameNum,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex
        );
MacRetType fillPucchHarqInfoForPCell1SCell1TB(
        DLUEContext     *dlUEContext_p,
        ULUEContext     *ulUEContext_p,
        ULSchUciInfo * ulschUciInfo_p,
        UInt32 nPucch,
        UInt8 pCellHarqIndex,
        UInt8 tpcVal_Dci,
        /** SPR 12457 Fix : Start**/
        UInt32 rbMapNum,
        UInt8 ulUEScheduledFlag,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif
#endif
        UInt8 cellIndex,
        InternalCellIndex internalCellIndex
        /** SPR 12457 Fix : End **/
        );
MacRetType fillPucchHarqInfoForPCell1SCell2TB(
        DLUEContext     *dlUEContext_p,
        ULUEContext     *ulUEContext_p,
        ULSchUciInfo * ulschUciInfo_p,
        UInt32 nPucch,
        UInt8 pCellHarqIndex,
        UInt8 tpcVal_Dci,
        /** SPR 12457 Fix : Start**/
        UInt32 rbMapNum,
        UInt8 ulUEScheduledFlag,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif
#endif
        UInt8 cellIndex,
        InternalCellIndex internalCellIndex
        /** SPR 12457 Fix : End **/
        );
MacRetType fillPucchHarqInfoForPCell2SCell1TB(
        DLUEContext     *dlUEContext_p,
        ULUEContext     *ulUEContext_p,
        ULSchUciInfo * ulschUciInfo_p,
        UInt32 nPucch,
        UInt8 pCellHarqIndex,
        UInt8 tpcVal_Dci,
        /** SPR 12457 Fix : Start**/
        UInt32 rbMapNum,
        UInt8 ulUEScheduledFlag,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif
#endif
        UInt8 cellIndex,
        InternalCellIndex internalCellIndex
        /** SPR 12457 Fix : End **/
        );
MacRetType fillPucchHarqInfoForPCell2SCell2TB(
        DLUEContext     *dlUEContext_p,
        ULUEContext     *ulUEContext_p,
        ULSchUciInfo * ulschUciInfo_p,
        UInt32 nPucch,
        UInt8 pCellHarqIndex,
        UInt8 tpcVal_Dci,
        /** SPR 12457 Fix : Start**/
        UInt32 rbMapNum,
        UInt8 ulUEScheduledFlag,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif
#endif
        UInt8 cellIndex,
        InternalCellIndex internalCellIndex
        /** SPR 12457 Fix : End **/
        );
#endif
/* SPR 11215 Changes End */
STATIC MacRetType createPUCCHMapForReportOrSR(
        /* SPR 16855 fix start */
        UInt16  ueIndex,
        PucchConfig *pucchConfigInfo_p,
        /* SPR 16855 fix end */
        ULUEContext  *currentUEContext_p,
        UInt32 subFrameOffset,
        UInt32 periodicity,
        ReportAndOpportunity reportType,
        UInt8 * maxWrapAround_p
        ,UInt32 pucchResourceIndex
        ,UInt32 pucchFormat
        ,UeCellInfo * ueCellInfo_p
        ,InternalCellIndex internalCellIndex
        );

/* This function is used to calculate the mapping of PUCCH Resource Index
   to Physical Resource Block Index */
STATIC MacRetType getPRBFromPUCCHResourceIndex( UInt32 pucchResourceIndex,
        UInt32 pucchFormat,
        UInt8 *prbIndex1,
        UInt8 *prbIndex2,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);
/*CA Changes end  */

STATIC  globalSrOpportunityAndPeriodicReports * createSrOppAndReportNode(
        UInt32 ueIndex,
        UInt8 type
        ,UeCellInfo * ueCellInfo_p
        );
/*SPR 16855 fix start*/
STATIC globalSrOpportunityAndPeriodicReports * insertSrOrReportNodeInPucchDb(
        UInt16  ueIndex,
        UInt16 subFrameOffset,
        UInt8 ueReportOrOpportunityType,
        /*CA Changes start  */
        UeCellInfo * ueCellInfo_p,
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        );
/*SPR 16855 fix end*/

/* Coverity Start : 59096 */
#ifdef TDD_CONFIG
STATIC  void prachSetFrequencyResBits(
        UInt16 numOfFreqRes,
        UInt8 * freqResBitString);
#endif
/* Coverity End : 59096 */

#ifdef TDD_CONFIG
STATIC void processPUCCHSrOrReportAndAckNack(
        UInt8 *ueReportAndOpportunity,
        ULUEContext   *ulUEContext_p,
        UeScheduledInfo *ulUeScheduledInfo_p,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt8 subFrameNum,
        UInt16 sysFrameNum,
        UInt32 rbMapNum,
        /* +- SPR 17777 */
        UeCellInfo *ueCellInfo_p,
        /* SPR 15909 fix start */
        tickType_t ulTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
void allocateResourceForTddHarq(
        UInt32 ulSubFrame,
        UInt32  ueIndex,
        UInt32  rbMapNum,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UeScheduledInfo * ulUeScheduledInfo_p,
        InternalCellIndex internalCellIndex);
#endif
#else
STATIC void processPUCCHSrOrReportAndAckNack(
        UInt8 *ueReportAndOpportunity_p,
        ULUEContext      *ulUEContext_p,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt8 subFrameNum,
        /* +- SPR 17777 */
        UInt32 rbMapNum,
        UInt32 ulUEScheduledFlag,
        UInt32 cceIndex,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        UeCellInfo *ueCellInfo_p
#ifdef PUCCH_RESERVE_ALWAYS
        ,UInt8 puschReserveOnlyFlagForSingleUE
#endif
        /* SPR 15909 fix start */
        ,tickType_t ulTTI
        /* SPR 15909 fix end */
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */

        );
#endif
/* Rel9_upgrade_CR410 */

/* SPR 1101 changes start*/
/* UCI_HARQ for Contention Resolution Messages */
STATIC  void buildUciHarqForTempUE(
        PdcchTemporaryContextUEInfo *contentionInfo_p,
        UInt8 contentionUECount,
        UInt32  rbMapNum,
#ifdef TDD_CONFIG
        UInt8 ulSubFrame,
#endif
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        );
/* SPR 1101 changes end*/

STATIC void processPUCCHSrOrReportOnly(
        UInt8 ueReportAndOpportunity,
        ULUEContext * ulUEContext_p,
#ifdef TDD_CONFIG
        UeScheduledInfo *ulUeScheduledInfo_p,
        /* +- SPR 17777 */
        UInt16 sysFrameNum,
#endif
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt8 subFrameNum,
        /* +- SPR 17777 */
        UInt32 rbMapNum,
        /* Rel9_upgrade_CR410 */
        /* SPR 15909 fix start */
        tickType_t ulTTI
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG
        ,PdcchULOutputInfo *pdcchULOutputInfo_p
#endif
        ,UeCellInfo *ueCellInfo_p
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        );
/* Rel9_upgrade_CR410 */

/* Rel9_upgrade_CR257 */
STATIC void processPUCCHShortSrOnly(
        ULUEContext * ulUEContext_p,
#ifdef TDD_CONFIG
        UeScheduledInfo *ulUeScheduledInfo_p,
#endif
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt32 rbMapNum
#ifdef FDD_CONFIG
        ,PdcchULOutputInfo *pdcchULOutputInfo_p
#endif
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */

        );


#ifdef TDD_CONFIG
STATIC void processPUCCHShortSr(
        ULUEContext * ulUEContext_p,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt32 rbMapNum,
        InternalCellIndex internalCellIndex
        );

STATIC void processPUCCHShortSrAndAckNack(
        ULUEContext         *ulUEContext_p,
        UeScheduledInfo     *ulUeScheduledInfo_p,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt32 ulSubFrame,
        UInt32 rbMapNum,
        InternalCellIndex internalCellIndex
        );
#else
STATIC void processPUCCHShortSrAndAckNack(
        ULUEContext         *ulUEContext_p,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt32 rbMapNum
        ,UInt32 cceIndex
        ,UInt32 subFrameNum
        ,PdcchULOutputInfo *pdcchULOutputInfo_p
#ifdef PUCCH_RESERVE_ALWAYS
        ,UInt8 puschReserveOnlyFlagForSingleUE
#endif
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */

        );
#endif
/* Rel9_upgrade_CR257 */

#ifdef TDD_CONFIG
STATIC  void processPUCCHAckNacksOnly(
        UInt32 subFrameNum,
        UInt32  ueIndex,
        UInt32  rbMapNum,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p
        ,UeScheduledInfo * ulUeScheduledInfo_p,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex
        /* +- SPR 17777 */
        );
#endif

#ifdef TDD_CONFIG
STATIC  void pucchHandleAcksOnlyByMux(
        UInt32 subFrameNum,
        UInt32  ueIndex,
        UInt32  rbMapNum,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p
        ,UeScheduledInfo * ulUeScheduledInfo_p
        ,InternalCellIndex internalCellIndex
        );

#endif

void storeCqiPmiRiData(
        ULSchUciRachPoolInfo* ulschUciRachInfo_p, 
        UInt16 numberOfInfoPresent, 
        ULUEContext * ulUEContext_p,
        /* +- SPR 17777 */
        UInt8 report, UInt8 subFrameNum,
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        /* +- SPR 17777 */
        UeCellInfo * ueCellInfo_p,
        InternalCellIndex internalCellIndex
        );

#ifdef TDD_CONFIG
 MacRetType isUeSISO(
        UInt32  ueIndex);

/* Coverity Start : 59097 */
STATIC  void pucchGetNpsForCCE(
        UInt8   cceIndex,
        UInt32  *Np,
        UInt32  *Npnext,
        InternalCellIndex internalCellIndex
        );
/* Coverity End : 59097 */

STATIC  UInt32 calculateNp(
        UInt32  NumDlRb,
        UInt32  NumRbSc,
        UInt8   p);
/* + SPR 17439 */
STATIC void initTddMacDlAssociationSet(void);
/* - SPR 17439 */
#endif
/*SPR 2617 Start*/
extern  PeriodicReportingModes getPeriodicReportingMode
(UInt8 transMode, PeriodicFeedbackType periodicFeedBackType,
 /* + TM7_8 Changes Start */
 UInt8 pmiRiReportV920); 
/* - TM7_8 Changes End */
/*SPR 2617 End*/

/* TDD SB_CQI */
STATIC MacRetType allocMemForPucchNode(
        pucchNodeForReport *pucchNodeForReport_p,
        UInt32 periodicity
        );	

 UInt8 getCountForMaxNode (UInt32 periodicity);

STATIC UInt32 computeGCD (
        UInt32 periodicity,
        UInt32 subframeOffset
        );
/* TDD SB_CQI */
/*ca-tdd pucch code chnages*/
/*
#ifdef DL_UL_SPLIT
STATIC void convertULConfigPduTypeAllocFail (   UInt16 ueIndex,
        ULSchUciInfo* ulSchUciInfo_p,
        UInt8 sCellIndex
        );
#endif
*/
STATIC void dropCQIReport(
        ULUEContext *ulUEContext_p,
        DLUEContext *ueDLContext_p,
        UInt8 isPCell,
        /*** Coverity 62981_82_83_84_85_86 Fix : Start ****/
        /** SPR 12165 Fix : Start **/
        UInt8 *report,
        /** SPR 12165 Fix : End **/
        /*** Coverity 62981_82_83_84_85_86 Fix : End ****/
        UInt8 internalCellIndex
        );

/*Cyclomatic Complexity changes - starts here*/
STATIC  MacRetType validateAndUpdatePucchSrAndReportsNode
(
 ULUEContextInfo   *uLUEContextInfo_p,
 UInt32  ueIndex,
 /* SPR 15909 fix start */
 tickType_t globalTick
 /* SPR 15909 fix end */
 );
#ifdef TDD_CONFIG
STATIC  MacRetType validateAndUpdatePduTypeForSrShortOpportunity
(
 UInt8 pduType,
 ULSchUciRachPoolInfo* ulschUciRachInfo_p,
 UInt16 numInfo
 );
#endif

/*Reconfig Changes Start */
MacRetType reConfigRIAndCQIMap ( 
        ULUEContext *ulUeCtx_p, 
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink *uplkUEInfo_p
        /*SPR 16855 fix start*/
        , ULReconfigCQIRIMsgType *cqiRiReconfigRetValue_p,
        UeContextForReconfig *reconfigInfo_p 
        /*SPR 16855 fix end*/
        ,UeCellInfo * ueCellInfo_p
        ,PucchConfig *pucchConfig_p
        ,UInt8 J
        ,InternalCellIndex internalCellIndex
        );

/*Reconfig Changes Start */
MacRetType reConfigRIAndCQIMapAbs ( 
        ULUEContext *ulUeCtx_p, 
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink *uplkUEInfo_p
        /*SPR 16855 fix start*/
        , ULReconfigCQIRIMsgType *cqiRiReconfigRetValue_p,
        UeContextForReconfig *reconfigInfo_p 
        /*SPR 16855 fix end*/
        ,UeCellInfo * ueCellInfo_p
        ,PucchConfig *pucchConfig_p
        ,UInt8 J
        ,InternalCellIndex internalCellIndex
        );
void deletePucchNodesForUE( 
        /* PUCCH TDD ADAPTATION CHANGES start */
        PucchConfig *pucchConfigInfo_p,
        /* PUCCH TDD ADAPTATION CHANGES end */
        ReportAndOpportunity reportType,
        UeCellInfo * ueCellInfo_p,
        InternalCellIndex internalCellIndex );

/* Rel9_upgrade_Cr257 */
MacRetType initCQIAndRIinPUCCHMap (
        /*SPR 16855 fix start*/
        UInt16 ueIndex,
        PucchConfig *pucchConfigInfo_p,
        /*SPR 16855 fix end*/
        ULUEContext  *currentUEContext_p,
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink*  uplkUEInfo_p,
        UeCellInfo  * ueCellInfo_p,
        UInt8 J,
        /*SPR 11541 Start */
        InternalCellIndex internalCellIndex,
        UInt8 rrcLoopIndex
        /*SPR 11541 End*/
        );
/*SPR 2617 End*/
/*Cyclomatic Complexity changes - ends here*/
/*E TDD changes*/
#ifdef TDD_CONFIG
/* TDD Config 0 Changes Start */
void getMAndMaxCfiValue(UInt8 ulSubframe,
        UInt8 *cfi,
        UInt8 *M,
        InternalCellIndex internalCellIndex);
/* TDD Config 0 Changes End */
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
void fillPucchHarqInfoForMMoreThanOne(
        UInt16 ueIndex,
        ULSchUciInfo * ulschUciInfo_p,
        UInt32 *nPucch,
        UInt32  rbMapNum,
        UInt8   M,
        UInt8   resouceAllocCount,
        UeScheduledInfo * ulUeScheduledInfo_p,
        UInt32 subFrameNum,
        InternalCellIndex internalCellIndex
        );
/* SPR 19679: TDD HARQ Multiplexing Changes Start */
void fillHARQSizeForPUSCHWithChannelSelection(
         UInt16 ueIndex
        ,ULSchUciRachPoolInfo* ulschUciRachInfo_p
        ,UInt32 ulSubFrame
        ,UInt16  numberOfInfoPresent
        ,InternalCellIndex internalCellIndex
        );
/* SPR 19679: TDD HARQ Multiplexing Changes End */
#endif
#endif
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
#ifdef TDD_CONFIG              
extern UInt16 pucchUciUlschMap_g[MAX_UE_SUPPORTED];
#endif
extern UInt32 resetBitmask[MAX_SUB_FRAME];
extern UInt32 bitmask[MAX_SUB_FRAME];
#ifdef FDD_CONFIG
#ifdef PERF_STATS
/* + PERF_CA */
extern LteCellPerfStats *gMacCellPerfStats_p[MAX_NUM_CELL];
/* - PERF_CA */
#endif
#endif
/*****************************************************************************
 * Private Variables (Must be declared STATIC)
 *****************************************************************************/


/* 
   Refer Specs 36213-860 Table 10.1-5
   UE-specific SR periodicity and subframe offset configuration */
STATIC SrConfigTable srConfigPeriodicityAndSubFrame_g[] = 
{
    /* SR configuration Index   SR periodicity (ms)   SR subframe offset
       start   end                                                      */
    {    0,     4,                   5,                   0 },
    {    5,     14,                  10,                 -5 },
    {    15,    34,                  20,                 -15},
    {    35,    74,                  40,                 -35},
    {    75,    154,                 80,                 -75},
    /* Rel9_upgrade_CR257 */
    {    155,   156,                 2,                  -155},
    {    157,   157,                 1,                  -157}
    /* Rel9_upgrade_CR257 */
};

#ifdef TDD_CONFIG
/* Refer 36213-860 Table 7.2.2-1C
   Mapping of cqi-pmi-ConfigIndex to NP and NOFFSET,CQI for TDD.
   cqi-pmi-ConfigIndex = 317  Reserved 
   cqi-pmi-ConfigIndex = 542- 1023   Reserved */
STATIC CqiPmiConfig cqiPmiConfigTable_g [] =
{  /* CQI PMI Index   Value of Np    Value of Noffset cqi
      Start  End Index  Periodicity      Offset CQI   */
    {  0 ,   0 ,         1,               0   },
    {  1 ,   5 ,         5,              -1   },
    {  6,    15,        10,              -6   },
    {  16 ,  35,        20,              -16  },
    {  36,   75,        40,              -36  },          
    {  76 ,  155,       80,              -76  },
    {  156 , 315,       160,             -156 }
};
#else
/* Refer 36213-860 Table 7.2.2-1A
   Mapping of cqi-pmi-ConfigIndex to NP and NOFFSET,CQI for FDD.
   cqi-pmi-ConfigIndex = 317  Reserved 
   cqi-pmi-ConfigIndex = 542- 1023   Reserved */
STATIC CqiPmiConfig cqiPmiConfigTable_g [] =
{  /* CQI PMI Index   Value of Np    Value of Noffset cqi
      Start  End Index  Periodicity      Offset CQI   */
    {  0 ,   1 ,         2,               0   },
    {  2 ,   6 ,         5,              -2   },
    {  7,    16,        10,              -7   },
    {  17 ,  36,        20,              -17  },
    {  37,   76,        40,              -37  },          
    {  77 ,  156,       80,              -77  },
    {  157 , 316,       160,             -157 },
    {  318 , 349,       32,              -318  },
    {  350 , 413,       64,              -350 },
    {  414 , 541,       128,             -414 }
};
#endif

/* Refer specs 36214-860 Table 7.2.2-1B
   Mapping of ri-ConfigIndex to MRI and NOFFSET,RI.
   ri-ConfigIndex = 966 - 1023   Reserved  */
STATIC RiConfig riConfigTable_g [] = 
{ 
    /*  ri-ConfigIndex   Value of MRI    Value of NOFFSET,RI
        start  end                                            */
    { 0  ,  160,            1,                 0   },
    { 161,  321,            2,                -161 },
    { 322,  482,            4 ,               -322 },
    { 483,  643,            8 ,               -483 },
    { 644,  804,            16,               -644 },          
    { 805,  965,            32,               -805 }
};


/*Used in Formula getPRBFromPUCCHResourceIndex*/
STATIC UInt32 nRBCQI_g[MAX_NUM_CELL]          = {0};
STATIC UInt32 nCsAN_g[MAX_NUM_CELL]           = {0};
STATIC UInt32 deltaPUCCHSHift_g[MAX_NUM_CELL] = {0};
/* PUCCH Resource Index for Downlink Ack/Nack */
STATIC UInt32 n1PucchDLAckNack_g[MAX_NUM_CELL] = {0};
/* This represent the extended or normal Cyclic perfix*/
STATIC UInt32 cyclicPrefix_g[MAX_NUM_CELL]    = {0};

/* OPT */
STATIC UESrAndReportList pucchSrAndReportsDb_g[MAX_NUM_CELL][MAX_NUM_MAPS][MAX_SUBFRAME_OFFSET];
/* OPT */
/* Rel9_upgrade_CR257 */
STATIC UESrAndReportList pucchSrShortReportDb_g[MAX_NUM_CELL][MAX_NUM_MAPS][SHORT_SR_PERIODICITY];



/* Rel9_upgrade_CR257 */
#ifdef TDD_CONFIG
STATIC UInt32 NpTable_g[MAX_NUM_CELL][MAX_P_VAL] = {{0}};
STATIC UInt8 Nrbsc_g = MAX_SC_RB;
STATIC DlAssociationSetNode tddMacDlAssociationSet_g [UL_DL_CONFIG_MAX][MAX_SUB_FRAME] ;
 /*CA TDD CHANGES START*/
DlAssociationSetNode * globalDlAssociationSet_gp[MAX_NUM_CELL] = {PNULL};
 /*CA TDD CHANGES END*/
/* SPR 19679: TDD HARQ Multiplexing Changes Start */
STATIC DlAssociationSetNodeForHARQMultiplexing \
                tddMacDlAssociationSetForHarqMUX [UL_DL_CONFIG_MAX][MAX_SUB_FRAME] ;
DlAssociationSetNodeForHARQMultiplexing \
              * globalDlAssociationSetForHarqMultiplexing_gp[MAX_NUM_CELL] = {PNULL};

STATIC void initTddMacDlAssociationSetForHARQMultiplexing(void);
/* SPR 19679: TDD HARQ Multiplexing Changes End */
#else
/* ICIC changes start */
STATIC UInt8 advInitAvailCCRes[MAX_NUM_CELL][NUM_OF_RB_MAP] = {{0}};
/* ICIC changes end */
extern PdcchULOutputInfo* pdcchULOutputInfo_gp[MAX_NUM_CELL];
#endif

/*eicic changes*/

UInt8 updateCQIMapForAbs_g[MAX_NUM_CELL] = { LTE_FALSE };



/*****************************************************************************
 * Function Name  : initPUCCH
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function Initialize the the LIST POINTER Array of
 *                  Max Periodicity of SR/CQI/RI. This is called at cell Config
 *****************************************************************************/
void initPUCCH(InternalCellIndex internalCellIndex)
{
    UInt8 containerIndx = 0;
#ifdef FDD_CONFIG
    /* +- SPR 18268 */
    UInt16 ueIndex = 0;
    /* +- SPR 18268 */
#endif
    InitParams *initParams_p = PNULL;
    UInt32 subFrameNum;
    UInt8 index = 0;

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry.",__func__);

    initParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    /* CA Changes start */
    /* The mem pool will be created only first time when cell was configured */ 
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        /* Start 128 UE: Memory Changes */
        createMemPool(sizeof (globalSrOpportunityAndPeriodicReports),
                (initParams_p->macMaxNumOfUeSupported * MAX_PERIODICITY_SUBFRAME)); 
        /* End 128 UE: Memory Changes */

        /* Memory handling changes start */
        createMemPool(sizeof(globalSrShortOpportunity),
                (initParams_p->macMaxNumOfUeSupported * SHORT_SR_PERIODICITY)); 
        /* Memory handling changes end */

        /* Create mem pool for longPeriodicSRCQIPMIRIMapInfo_gp and shortPeriodicSRMapShortInfo_gp */
        createMemPool(sizeof(mapInfo), 2);

        /*SPR 20526 Changes Start*/
#ifdef FDD_CONFIG
        createMemPool(sizeof(ULSchUciRachPoolInfo) * MAX_PDCCH_CONTAINER, 1);


#else
        createMemPool(sizeof(ULSchUciRachPoolInfo) * MAX_ULSCH_UCI_CONTAINER, 1);
#endif
        /*SPR 20526 Changes End*/
    }

    for(index = 0; index < MAX_NUM_MAPS ; index++ )
    {
        for (subFrameNum = 0; subFrameNum < MAX_SUBFRAME_OFFSET;
                subFrameNum++)
        {
            listInit(&pucchSrAndReportsDb_g[internalCellIndex][index][subFrameNum].ueList);
        }

        for (subFrameNum = 0; subFrameNum < SHORT_SR_PERIODICITY;
                subFrameNum++)
        {
            listInit(&pucchSrShortReportDb_g[internalCellIndex][index][subFrameNum].ueList);
        }
    }


    GET_MEM_FROM_POOL(mapInfo, shortPeriodicSRMapShortInfo_gp[internalCellIndex], sizeof(mapInfo), PNULL);
    if (PNULL == shortPeriodicSRMapShortInfo_gp[internalCellIndex])
    {
        return ;
    }    

    GET_MEM_FROM_POOL(mapInfo, longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex], sizeof(mapInfo), PNULL);
    if (PNULL == longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex])
    {
        return ;
    }    

    shortPeriodicSRMapShortInfo_gp[internalCellIndex]->passiveIndex = 0;
    shortPeriodicSRMapShortInfo_gp[internalCellIndex]->activeIndex = 1;
    longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex = 0;
    longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex = 1;

#ifdef FDD_CONFIG
    SCELL_SCHEDULING_QUEUE_INIT (internalCellIndex)


    GET_MEM_FROM_POOL(ULSchUciRachPoolInfo, ulSchUciRachContainer_gp[internalCellIndex], sizeof(ULSchUciRachPoolInfo) * MAX_PDCCH_CONTAINER, PNULL);
    if (PNULL == ulSchUciRachContainer_gp[internalCellIndex])
    {
        return ;
    }

    for(containerIndx = 0; containerIndx < MAX_PDCCH_CONTAINER; containerIndx++)
    {
        memSet(ulSchUciRachContainer_gp[internalCellIndex] + containerIndx, 0, sizeof(ULSchUciRachPoolInfo));
    }

#endif
#ifdef TDD_CONFIG
    /* TDD Config 0 Changes Start*/ 
    GET_MEM_FROM_POOL(ULSchUciRachPoolInfo, ulSchUciRachContainer_gp[internalCellIndex], sizeof(ULSchUciRachPoolInfo) * MAX_ULSCH_UCI_CONTAINER, PNULL);
    if (PNULL == ulSchUciRachContainer_gp[internalCellIndex])
    {
        return ;
    }

    for(containerIndx = 0; containerIndx < MAX_ULSCH_UCI_CONTAINER; containerIndx++)
    {
        memSet(ulSchUciRachContainer_gp[internalCellIndex] + containerIndx, 0, sizeof(ULSchUciRachPoolInfo));
    }
    /* TDD Config 0 Changes End*/ 
    /* SPR 19080 start. This function call is moved from here to updatePUCCHConfig*/
    /*initTddMacDlAssociationSet();*/
    /* SPR 19080 end*/

    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    initTddMacDlAssociationSetForHARQMultiplexing();
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
#else
    /* Rel9_upgrade_CR257 */ 
    for(containerIndx = 0; containerIndx < MAX_PDCCH_CONTAINER; containerIndx++)
    {
        for(ueIndex = 0; ueIndex < MAX_UE_SUPPORTED; ueIndex++)
        {
            (pdcchULOutputInfo_gp[internalCellIndex] + containerIndx)->directIndexingUEInfo[ueIndex].
                ulschUciInfoIndex = ULSCH_UCI_INVALID_INDEX;
                        /* SPR 14877 SRS Changes*/
            (pdcchULOutputInfo_gp[internalCellIndex] + containerIndx)->directIndexingUEInfo[ueIndex].
                ulschInfoIndex = ULSCH_UCI_INVALID_INDEX;
                        /* SPR 14877 SRS Changes*/
        }
    }
#endif

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}

/*****************************************************************************
 * Function Name  : cleanupPUCCH
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function reInitialize the the LIST POINTER Array of
 *                  Max Periodicity of SR/CQI/RI. This is called at cell Stop
 *****************************************************************************/
void cleanupPUCCH(InternalCellIndex internalCellIndex)
{
    UInt32 count          = 0;
    UInt8 subFrameNum     = 0;
    LTE_LIST_NODE *listNode_p = PNULL;
    UInt8 index = 0;

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);

    for( index = 0; index < MAX_NUM_MAPS; index++)
    {
        for (subFrameNum = 0; subFrameNum < MAX_SUBFRAME_OFFSET; subFrameNum++)
        {
            count = listCount(&(pucchSrAndReportsDb_g[internalCellIndex][index][subFrameNum].ueList));
            while(count) 
            {
                listNode_p = getListNode(&pucchSrAndReportsDb_g[internalCellIndex][index][subFrameNum].ueList, 0);
                if(listNode_p) 
                {
                    listDeleteNode(&(pucchSrAndReportsDb_g[internalCellIndex][index][subFrameNum].ueList), listNode_p);
                    freeMemPool(listNode_p);
                    listNode_p = PNULL;
                }
                count--;
            }
        }
        /* Rel9_upgrade_CR257 */
        for (subFrameNum = 0; subFrameNum < SHORT_SR_PERIODICITY; subFrameNum++)
        {
            count = listCount(&(pucchSrShortReportDb_g[internalCellIndex][index][subFrameNum].ueList));
            while(count) 
            {
                listNode_p = getListNode(&pucchSrShortReportDb_g[internalCellIndex][index][subFrameNum].ueList, 0);
                if(listNode_p) 
                {
                    listDeleteNode(&(pucchSrShortReportDb_g[internalCellIndex][index][subFrameNum].ueList), listNode_p);
                    freeMemPool(listNode_p);
                    listNode_p = PNULL;
                }
                count--;
            }
        }
    }

#ifdef FDD_CONFIG
    /* + SPR_11108 */
#if (defined (DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL))
    UInt8 loopVar=0;
    count=0;
    SCellSchedulingQueueNode * sCellSchedulingQueueNode_p = PNULL;

    /* cleanup the queue dlSecondayCellScheulingQueue_g */
    count = SCELL_SCHEDULING_QUEUE_COUNT(internalCellIndex);
    for (loopVar = 0; loopVar < count; loopVar++)
    {
        DEQUEUE_SCELL_SCHEDULING_NODE(sCellSchedulingQueueNode_p,internalCellIndex);
        if (sCellSchedulingQueueNode_p)
        {
            FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellSchedulingQueueNode_p);
        }
    }
#endif
    /* - SPR_11108 */
#endif
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}

/*****************************************************************************
 * Function Name  : deInitPUCCH
 * Inputs         : InternalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function free the the memory 
 *                  This is called at cell delete
 *****************************************************************************/
void deInitPUCCH(InternalCellIndex internalCellIndex)
{
    if (shortPeriodicSRMapShortInfo_gp[internalCellIndex])
    {
        freeMemPool(shortPeriodicSRMapShortInfo_gp[internalCellIndex]);
        shortPeriodicSRMapShortInfo_gp[internalCellIndex] = PNULL;
    }

    if (longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex])
    {
        freeMemPool(longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]);
        longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex] = PNULL;
    }

    if (ulSchUciRachContainer_gp[internalCellIndex])
    {
        freeMemPool(ulSchUciRachContainer_gp[internalCellIndex]);
        ulSchUciRachContainer_gp[internalCellIndex] = PNULL;
    }

  

#ifdef FDD_CONFIG
    SCELL_SCHEDULING_QUEUE_DEINIT(internalCellIndex);
#endif

    return;
}

/*****************************************************************************
 * Function Name  : updatePUCCHConfig
 * Inputs         : deltaPUCCHSHIFT - ds1,ds2,ds3 where ds1 corresponds
 *                                     to value 1 ds2 to 2 etc
 *                  nRBCQI - Bandwidth available for use by PUCCH formats
 *                           2/2a/2b, expressed in multiples of frequency
 *                  nCSAN -  Number of cyclic shifts used for PUCCH formats 1/1a/1b
 *                           in a resource block with a mix of formats
 *                           1/1a/1b and 2/2a/2b
 *                 n1PucchDLAckNack - Resource index for PUCCH formats 1/1a/1b
 *                  internalCellIndex - Cell-Index of PCell or SCell
 * Outputs        : Update the global variables.
 * Returns        : None
 * Description    : This function update the global variable realted to CQI
 *                  cyclicPrefix_g,nRBCQI_g,nCsAN_g,deltaPUCCHSHift_g
 *****************************************************************************/
/* CA Changes start */
void updatePUCCHConfig( UInt32 deltaPUCCHSHIFT,
        UInt32 nRBCQI,
        UInt32 nCSAN,
        UInt32 n1PucchDLAckNack,
        InternalCellIndex internalCellIndex
        )
/* CA Changes end */
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry.",__func__);
    /*  Refer :- Spec 36311-850 , 6.3.2 PUCCH-Config
        PUCCH-ConfigCommon                  SEQUENCE {
        deltaPUCCH-Shift                    ENUMERATED {ds1, ds2, ds3},
        nRB-CQI                             INTEGER (0..98),
        nCS-AN                              INTEGER (0..7),
        n1PUCCH-AN                          INTEGER (0..2047)
        */
#ifdef TDD_CONFIG
    UInt8 p = 0;
#endif
    CellConfigParams* cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    deltaPUCCHSHift_g[internalCellIndex] = deltaPUCCHSHIFT;
    nRBCQI_g[internalCellIndex]          = nRBCQI;
    nCsAN_g[internalCellIndex]           = nCSAN;
    /* Refer spec 36211-850 5.4.3 Page No. 21*/
    if ( cellParams_p->cyclicPrefix == 1 )
    {
        /* Normal Cyclic Prefix*/
        cyclicPrefix_g[internalCellIndex]    = NORMAL_CYCLIC_PREFIX_VALUE;
    }
    else /* cellConfig_g.cyclicPrefix == 2*/
    {
        /*Extended Cyclic Prefix */
        cyclicPrefix_g[internalCellIndex]    = EXTENDED_CYCLIC_PREFIX_VALUE;
    }
    n1PucchDLAckNack_g[internalCellIndex] = n1PucchDLAckNack;

    LTE_MAC_UT_LOG( LOG_INFO, PUCCH_MODULE,"[%s] deltaPUCCHSHift_g = %d ,"
            "nRBCQI_g =%d , nCsAN_g = %d, cyclicPrefix_g =%d,"
            "n1PucchDLAckNack_g = %d ", __func__,deltaPUCCHSHift_g,
            nRBCQI_g, nCsAN_g, cyclicPrefix_g, n1PucchDLAckNack_g
            );
#ifdef TDD_CONFIG    
    /*
     * set up the dl_assoc set ptr to appropriate UL/DL config
     * Hopefully they wont call this func before processing cell request otherwise
     * cellConfig_g.subFrameAssign will be invalid
     */
    /* SPR 19080 start */
    /* Initializing tddMacDlAssociationSet_g before it is actually getting used*/
    initTddMacDlAssociationSet();
    /* SPR 19080 end */
    /*CA TDD CHANGES START*/
    globalDlAssociationSet_gp[internalCellIndex] = tddMacDlAssociationSet_g[cellParams_p->subFrameAssign];
    /*CA TDD CHANGES END*/
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    globalDlAssociationSetForHarqMultiplexing_gp[internalCellIndex] = 
        tddMacDlAssociationSetForHarqMUX[cellParams_p->subFrameAssign];
    /* SPR 19679: TDD HARQ Multiplexing Changes Endt */
    /*
     * Calculate all Np values beforehand
     */
    for (p = 0; p < MAX_P_VAL; p++)
    {
        NpTable_g[internalCellIndex][p] = calculateNp(cellParams_p->dlAvailableRBs, Nrbsc_g, p);
    }
#endif
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}



/*****************************************************************************
 * Function Name  : getPRBFromPUCCHResourceIndex
 * Inputs         : n1PUCCH -
 *                  pucchFormat - Different PUCCH Format 1,1a,1b or 2,2a,2b
 *                  *prbIndex1 - This function fills the prbIndex correspondance
 *                               to pucchResourceIndex.
 *                  *prbIndex2 - This function fills the prbIndex correspondance
 *                               to pucchResourceIndex.
 *                  internalCellIndex - Cell-Index of PCell or SCell
 * Outputs        : This function Fills the *prbIndex1 and *prbIndex2 parameter.
 * Returns        : MacRetType
 * Description    : This function calculates mapping of PUCCH Resource Index to
 Physical Resource Block
 *****************************************************************************/
STATIC MacRetType getPRBFromPUCCHResourceIndex( UInt32 n1PUCCH,
        UInt32 pucchFormat,
        UInt8 *prbIndex1,
        UInt8 *prbIndex2,
        InternalCellIndex internalCellIndex )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry.",__func__);

    /* Refer Specs 36211-850 Section 5.4.3 Mapping to physical resources
       First calculate the value of m , Configured the value of c, nRB, nCS_An,
       deltaPUCCH_SHIFT by RRC (higher layers), c = 2 extended cyclic prefix
       c = 3 normal cyclic prefix */
    UInt32 tempVal       = 0;
    UInt32 m             = 0;

    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    /* the variable m depends on the PUCCH format. For formats 1, 1a and 1b
       cyclicPrefix_g = 2 or 3
       c = 2 extended cyclic prefix or  c = 3 normal cyclic prefix
       nCsAN_g represent - Ncs for format 1
       deltaPUCCHSHift_g -ds1,ds2,ds3 where ds1 corresponds to value 1 ds2 to 2 etc.
       */
    if ( pucchFormat    == UCI_FORMAT_ONE   ||
            pucchFormat == UCI_FORMAT_ONE_A ||
            pucchFormat == UCI_FORMAT_ONE_B    )
    {
        tempVal = cyclicPrefix_g[internalCellIndex] * nCsAN_g[internalCellIndex] / deltaPUCCHSHift_g[internalCellIndex];
        if ( n1PUCCH  <  tempVal )
        {
            m = nRBCQI_g[internalCellIndex]; /*Represent Nrb for format 2*/
        }
        else
        {       /*Nsc =12*/
            m = (( n1PUCCH - tempVal ) / (cyclicPrefix_g[internalCellIndex]* NSCRB/deltaPUCCHSHift_g[internalCellIndex])) +nRBCQI_g[internalCellIndex] +
                ceil_wrapper(DIVIDE_BY_EIGHT(nCsAN_g[internalCellIndex] ));
        }
    }
    else if ( pucchFormat == UCI_FORMAT_TWO ||
            pucchFormat == UCI_FORMAT_TWO_A ||
            pucchFormat == UCI_FORMAT_TWO_B )
    {
        m = n1PUCCH / NSCRB;
    }
    else
    {
        /* Review comment fix start GG13 */
        LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                getCurrentTick(),
                DEFAULT_INT_VALUE,
                n1PUCCH,
                pucchFormat,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "Invalid PUCCH Format(ueIndex DEFAULT_INT_VALUE)");
        return MAC_FAILURE;
        /* Review comment fix end GG13 */
    }

    *prbIndex1 = (DIVIDE_BY_TWO( m));
    *prbIndex2  = cellParams_p->ulAvailableRBs -1 - (*prbIndex1);

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] PRB Index1 =%d , PRB Index =%d Exit.",
            __func__,*prbIndex1,*prbIndex2);
    return MAC_SUCCESS;

}

/*****************************************************************************
 * Function Name  : deletePucchNodesForUE
 * Inputs         : pucchConfigInfo_p - PUCCH config info of selected UE-Context 
 *                  reportType - report type to be cleaned
 *                  ueCellInfo_p -
 *                  internalCellIndex - Cell-Index of PCell or SCell
 * Outputs        :
 * Returns        : None
 * Description    : This function processes the pucch db for all the possible
 *                  reports and SR opportunities for given UE
 *****************************************************************************/
void deletePucchNodesForUE(
        PucchConfig *pucchConfigInfo_p,
        ReportAndOpportunity reportType,
        UeCellInfo * ueCellInfo_p,
        InternalCellIndex internalCellIndex )
{
    /*SPR 16855 fix start*/
    UInt32 passiveIdx = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
    /*SPR 16855 fix end*/
    /* TDD SB_CQI */
    UInt8 count = 0;
    UInt8 maxNodeCount = 0; 
    UInt8 * storeReportType = PNULL;
    pucchNodeForReport * pucchNodeForReport_p = PNULL;
    /** SPR 12320 Changes Start **/
    UInt32 nodeCount = 0;
    /** SPR 12320 Changes End **/

    switch (reportType)
    {
        case SCHEDULING_REQUEST :
            {
                /*SPR 16855 fix start*/
                pucchNodeForReport_p = 
                    &(pucchConfigInfo_p->pucchNodesForSR[passiveIdx]);
                /*SPR 16855 fix end*/    
                break;
            }
        /*Cov 69350 +*/    
        case WIDEBAND_CQI_REPORT :
            {
                if (LTE_TRUE == updateCQIMapForAbs_g[internalCellIndex])
                {
                    /*SPR 16855 fix start*/
                    pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForWbCqi2[passiveIdx]);
                    /*SPR 16855 fix end*/
                }
                else
                {
                    /*SPR 16855 fix start*/
                    pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForWbCqi[passiveIdx]);
                    /*SPR 16855 fix end*/
                }
                break;
            }
        case CQI_REPORT :
            {
                if (LTE_TRUE == updateCQIMapForAbs_g[internalCellIndex])
                {
                    /*SPR 16855 fix start*/
                    pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForCqi2[passiveIdx]);
                    /*SPR 16855 fix end*/
                }
                else
                {
                    /*SPR 16855 fix start*/
                    pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForCqi[passiveIdx]);
                    /*SPR 16855 fix end*/
                }
                break;
            }
        case RI_REPORT:
            {
                if (LTE_TRUE == updateCQIMapForAbs_g[internalCellIndex])
                {
                    /*SPR 16855 fix start*/
                    pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForRI2[passiveIdx]);
                    /*SPR 16855 fix end*/
                }
                else
                {
                    /*SPR 16855 fix start*/
                    pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForRI[passiveIdx]);
                    /*SPR 16855 fix end*/
                }
                break;
            }
            /*Cov 69350 -*/
        default :
            {
                lteWarning("Invalid Report Type\n");
                /* + coverity 32691 */
                return;
                /* - coverity 32691 */
            }
    }

    pucchNode *tempUePucchNode_p = pucchNodeForReport_p->pucchNodePtr;
    maxNodeCount = pucchNodeForReport_p->count;
    if (PNULL == tempUePucchNode_p || maxNodeCount == 0)
    {
        return;
    }
    while(count++ < maxNodeCount)
    {
        if (ueCellInfo_p->isPCell)
        {
            storeReportType = &(tempUePucchNode_p->pucchNodeRef_p->reportBitMapPCell.ueReportAndOpportunity);    
            UNSET_BIT_AT_POS(*storeReportType,reportType);
        }
        else
        {
            storeReportType = &(tempUePucchNode_p->pucchNodeRef_p->reportBitMapSCell\
                    [(ueCellInfo_p->sCellIndex - 1)].ueReportAndOpportunity);

            UNSET_BIT_AT_POS(*storeReportType,reportType);
            if (!(*storeReportType))
                tempUePucchNode_p->pucchNodeRef_p->refCntr--;    
        }
        if((!(*storeReportType)) && !(tempUePucchNode_p->pucchNodeRef_p->refCntr))
        {
            /** SPR 12320 Changes Start **/
            nodeCount = listCount(&(pucchSrAndReportsDb_g[internalCellIndex][passiveIdx]
                                    [tempUePucchNode_p->subFrameOffset].ueList));
            if(nodeCount)
            {
                /*SPR 16855 fix start*/
            listDeleteNode( &(pucchSrAndReportsDb_g[internalCellIndex][passiveIdx]
                        [tempUePucchNode_p->subFrameOffset].ueList ),
                    &(tempUePucchNode_p->pucchNodeRef_p->nodeAnchor));
                /*SPR 16855 fix end*/
            }
            /** SPR 12320 Changes End **/
            freeMemPool( tempUePucchNode_p->pucchNodeRef_p );
            tempUePucchNode_p->pucchNodeRef_p = PNULL;
        }
        tempUePucchNode_p++;
    }
    freeMemPool(pucchNodeForReport_p->pucchNodePtr);
    pucchNodeForReport_p->pucchNodePtr = PNULL;
    pucchNodeForReport_p->count =0;
    /* TDD SB_CQI */
}

/* Rel9_upgrade_CR257 */
/*****************************************************************************
 * Function Name  : deletePucchShortSRNodesForUE
 * Inputs         : pucchConfig_p - PUCCH config of selected UE-Context
 *                  reconfigInfo_p - UE Context for reconfiguration of UE
 *                  internalCellIndex - Cell-Index of PCell or SCell
 * Outputs        :
 * Returns        : None
 * Description    : This function populates the pucch db for all the possible
 *                  reports and SR opportunities to be processed for given UE
 *****************************************************************************/
/*Reconfig Changes Start */
void deletePucchShortSRNodesForUE(
        PucchConfig *pucchConfig_p,
        UeContextForReconfig *reconfigInfo_p,
        InternalCellIndex internalCellIndex)
{
    UInt32 srOffset              = 0;
    UInt32 periodicity           = 0;
    globalSrShortOpportunity *current_p = PNULL;

    srOffset              = pucchConfig_p->srOffset;
    periodicity           = pucchConfig_p->srPeriodicity;
    if(reconfigInfo_p && !(reconfigInfo_p->srReconfigBitMap & UL_RECONFIG_DELETE_SHORT_SR_NODE))
    {
        reconfigInfo_p->srOffset              = srOffset;
        reconfigInfo_p->srPeriodicity         = periodicity;
    }
    /*Reconfig Changes End */
    /*SPR 16855 fix start*/
    UInt32 passiveIdx = shortPeriodicSRMapShortInfo_gp[internalCellIndex]->passiveIndex;
    globalSrShortOpportunity *start_p = 
        pucchConfig_p->pucchShortSRNodeHead_p[passiveIdx];
    /*SPR 16855 fix end*/

    while ( PNULL != start_p )
    {
        /*SPR 16855 fix start*/
        listDeleteNode(&pucchSrShortReportDb_g[internalCellIndex][passiveIdx][srOffset].ueList, 
                &start_p->nodeAnchor);
        /*SPR 16855 fix end*/

        /* if periodicity is 1 ms, 2 nodes are created so delete next node */
        if(periodicity == 1)
        {
            current_p = start_p;
            start_p   = start_p->nextNode_p;
            freeMemPool((void*)current_p);
            current_p = PNULL;
            srOffset += periodicity;
        }
        else
        {
            freeMemPool((void*)start_p);
            start_p = PNULL;
        }
    }
    /*SPR 16855 fix start*/
    pucchConfig_p->pucchShortSRNodeHead_p[passiveIdx] = PNULL;
    /*SPR 16855 fix end*/
}
/* Rel9_upgrade_CR257 */

/*TDD SB_CQI */
/*****************************************************************************
 * Function Name  : findUeNode
 * Inputs         : subFrameOffset - Current subframe offset
 *                  currentUEContext_p - Current UL-UE Context
 *                  internalCellIndex - Cell-Index of PCell or SCell
 *                  passiveIdx -
 * Outputs        : 
 * Returns        : globalSrOpportunityAndPeriodicReports * - This function 
 *                       returns the UE node for PUCCH report processing
 * Description    : This function returns the UE Node for the pucch report 
 *                       processing.
 ******************************************************************************/
STATIC globalSrOpportunityAndPeriodicReports * findUeNode(
        UInt16 subFrameOffset, 
        ULUEContext *currentUEContext_p, 
        InternalCellIndex internalCellIndex
        /*SPR 16855 fix start*/
        ,UInt32 passiveIdx
        /*SPR 16855 fix end*/
        )
{
    globalSrOpportunityAndPeriodicReports * node_p = PNULL;
    /*SPR 16855 fix start*/
    node_p =(globalSrOpportunityAndPeriodicReports *)
        getListNode(&pucchSrAndReportsDb_g[internalCellIndex][passiveIdx][subFrameOffset].ueList, 0);
    /*SPR 16855 fix end*/
    while (node_p)
    {
        if( node_p->ueIndex == currentUEContext_p->ueIndex)
        {
            break;
        }
        node_p =(globalSrOpportunityAndPeriodicReports *)getNextListNode((LTE_LIST_NODE *)node_p);
    }
    return node_p;
}
/* TDD SB_CQI */

/*****************************************************************************
 * Function Name  : updateNodeInPucchDb
 * Inputs         : ueIndex - ue index of current UE
 *                  pucchNodeForReport_p - PUCCH report node
 *                  currentUEContext_p - ul context of the current UE 
 *                  subFrameOffset - current sub-frame offset 
 *                  ueReportOrOpportunityType - PUCCH repor types
 *                  exactWrapRound - Wrap around for PUCCH reports
 *                  ueCellInfo_p -
 *                  internalCellIndex - Cell-Index of PCell or SCell
 * Outputs        :
 * Returns        : None
 * Description    : This function populates the pucch db for all the possible
 *                  reports and SR opportunities to be processed for given UE
 *****************************************************************************/
/* OPT */
/* TDD SB_CQI */
STATIC  void updateNodeInPucchDb(
        /*SPR 16855 fix start*/
        UInt16  ueIndex,
        /*SPR 16855 fix end*/
        pucchNodeForReport *pucchNodeForReport_p,
        ULUEContext * currentUEContext_p,
        UInt16 subFrameOffset,
        ReportAndOpportunity ueReportOrOpportunityType,
        UInt16 exactWrapRound,
        UeCellInfo * ueCellInfo_p,
        InternalCellIndex internalCellIndex)
{
    pucchNode * pucchNodePtr_p = ((pucchNode *)pucchNodeForReport_p->pucchNodePtr) + (pucchNodeForReport_p->count) ;
    /*SPR 16855 fix start*/
    UInt32 passiveIdx = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
    UInt8 count = listCount(&(pucchSrAndReportsDb_g[internalCellIndex][passiveIdx][subFrameOffset].ueList));
    /*SPR 16855 fix end*/
    UInt8 reportOrOppTypeBitString = 0;
    globalSrOpportunityAndPeriodicReports *listNode_p = PNULL;

    SET_BIT_AT_POS(reportOrOppTypeBitString, ueReportOrOpportunityType);
    if (!count)
    {
        /*SPR 16855 fix start*/
        listNode_p = insertSrOrReportNodeInPucchDb(ueIndex,
                subFrameOffset,reportOrOppTypeBitString, 
                ueCellInfo_p,
                internalCellIndex);
        /*SPR 16855 fix end*/
        /* TDD SB_CQI */
        pucchNodePtr_p->pucchNodeRef_p = listNode_p;
        pucchNodePtr_p->subFrameOffset = subFrameOffset;
        pucchNodeForReport_p->count++;
        /* TDD SB_CQI */
    }
    else
    {
        /* TDD SB_CQI */
        listNode_p = findUeNode(subFrameOffset, currentUEContext_p, internalCellIndex
                /*SPR 16855 fix start*/
                ,passiveIdx
                /*SPR 16855 fix end*/
                );
        if(listNode_p && ueCellInfo_p->isPCell) 
            {
                /* Update the bitstring to include given report type */
                SET_BIT_AT_POS(listNode_p->reportBitMapPCell.ueReportAndOpportunity, ueReportOrOpportunityType);
            }
        /*This below code needs to be commented to avoid crash due to re-establishment in CA when same
         * CQI_PMI config index is allocated by RRM for PCELL and SCELL of same UE*/
/*+ spr 19359*/
          else if (listNode_p && !(ueCellInfo_p->isPCell))
          {
             if (!(listNode_p->reportBitMapSCell[(ueCellInfo_p->sCellIndex - 1)].ueReportAndOpportunity))
             {
               listNode_p->refCntr++;
             }
             SET_BIT_AT_POS(listNode_p->reportBitMapSCell[(ueCellInfo_p->sCellIndex - 1)].ueReportAndOpportunity, ueReportOrOpportunityType);
          }
/*- spr 19359*/
          else
          {
                /*SPR 16855 fix start*/
                listNode_p = insertSrOrReportNodeInPucchDb(ueIndex,
                        subFrameOffset,reportOrOppTypeBitString, 
                        ueCellInfo_p,
                        internalCellIndex);
                /*SPR 16855 fix end*/
                /* TDD SB_CQI */
            }
        pucchNodePtr_p->pucchNodeRef_p = listNode_p;
        pucchNodePtr_p->subFrameOffset = subFrameOffset;
        pucchNodeForReport_p->count++;
        /* TDD SB_CQI */
    }
    /* OPT */
    /* TDD SB_CQI */
    if(listNode_p && ueCellInfo_p->isPCell)
    {
        /*Complexity Start*/
        eicicUpdateNodeInPucchDb(
                ueReportOrOpportunityType,
                listNode_p,
                internalCellIndex,
                exactWrapRound);

        /*Complexity End*/
    }
    else if (listNode_p && !(ueCellInfo_p->isPCell))
    {
            switch (ueReportOrOpportunityType)
            {
                case SCHEDULING_REQUEST:
                listNode_p->reportBitMapSCell[(ueCellInfo_p->sCellIndex - 1)].exactSrWrapRound = exactWrapRound;
                break;
                case WIDEBAND_CQI_REPORT:
                listNode_p->reportBitMapSCell[(ueCellInfo_p->sCellIndex - 1)].exactWbCQIWrapRound = exactWrapRound;
                break;
                case CQI_REPORT:
                listNode_p->reportBitMapSCell[(ueCellInfo_p->sCellIndex - 1)].exactCQIWrapRound = exactWrapRound;
                break;
                case RI_REPORT:
                listNode_p->reportBitMapSCell[(ueCellInfo_p->sCellIndex - 1)].exactRIWrapRound = exactWrapRound;
                break;
                default:
                lteWarning("Invalid reportType being set\n");
            }
        }
    /* OPT */
}

/*****************************************************************************
 * Function Name  : eicicUpdateNodeInPucchDb 
 * Inputs         : ueReportOrOpportunityType -PUCCH report types
 *                  listNode_p- Pointer to be updated
 *                  internalCellIndex- Cell Index
 *                  exactWrapRound-Wrap around for PUCCH reports
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates node in pucch Db for EICIC
 *                  
 ******************************************************************************/
/*Complexity Start*/
void eicicUpdateNodeInPucchDb(
	ReportAndOpportunity ueReportOrOpportunityType,
	globalSrOpportunityAndPeriodicReports *listNode_p,
	InternalCellIndex internalCellIndex,
	UInt16 exactWrapRound)
{

    switch (ueReportOrOpportunityType)
    {
        case SCHEDULING_REQUEST:
        listNode_p->reportBitMapPCell.exactSrWrapRound = exactWrapRound;
        break;
        case WIDEBAND_CQI_REPORT:
        /*EICIC +*/
        if (LTE_TRUE == updateCQIMapForAbs_g[internalCellIndex])
        {
            listNode_p->reportBitMapPCell.exactWbCQIWrapRound2 = exactWrapRound;
        }
        else
        {
            /*EICIC -*/
            listNode_p->reportBitMapPCell.exactWbCQIWrapRound = exactWrapRound;
        }
        break;
        case CQI_REPORT:
        /*EICIC +*/
        if (LTE_TRUE == updateCQIMapForAbs_g[internalCellIndex])
        {
            listNode_p->reportBitMapPCell.exactCQIWrapRound2 = exactWrapRound;
        }
        else
        {
            /*EICIC -*/
            listNode_p->reportBitMapPCell.exactCQIWrapRound = exactWrapRound;
        }
        break;
        case RI_REPORT:
        /*EICIC +*/
        if (LTE_TRUE == updateCQIMapForAbs_g[internalCellIndex])
        {
            listNode_p->reportBitMapPCell.exactRIWrapRound2 = exactWrapRound;
        }
        else
        {
            /*EICIC -*/
            listNode_p->reportBitMapPCell.exactRIWrapRound = exactWrapRound;
        }
        break;
        default:
        lteWarning("Invalid reportType being set\n");
    }
}
/*Complexity End*/
/* TDD SB_CQI */
/*****************************************************************************
 * Function Name  : computeGCD 
 * Inputs         : periodicity
 *             subframeOffset : Represents the MAX_SUBFRAME_OFFSET
 * Outputs        : 
 * Returns        : Returns the periodicity.
 * Description    : This function computes the GCD
 ******************************************************************************/
STATIC UInt32 computeGCD ( UInt32 periodicity,
        UInt32 subframeOffset )
{
    UInt32 t =0;
    while(subframeOffset != 0)
    {
        t = subframeOffset;
        subframeOffset = periodicity % t;
        periodicity = t;
    }
    return  periodicity;
}

/*****************************************************************************
 * Function Name  : getCountForMaxNode
 * Inputs         : periodicity - Periodicity of the reports
 * Outputs        : UInt8
 * Returns        : UInt8
 * Description    : This function returns the node count for a periodicity.
 ******************************************************************************/
 UInt8 getCountForMaxNode    (
        UInt32 periodicity
        )
{
    UInt32 gcd = 0;
    gcd = computeGCD(periodicity,MAX_SUBFRAME_OFFSET); 
    return (MAX_SUBFRAME_OFFSET/gcd);
}

/*****************************************************************************
 * Function Name  : allocMemForPucchNode 
 * Inputs         : pucchNodeForReport_p : Pointer of type pucchNodeForReport
 *                  periodicity -  Periodicity of PUCCH reports.
 * Outputs        :
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function allocates memory for a new PUCCH report node.
 *******************************************************************************/
STATIC MacRetType allocMemForPucchNode (
        pucchNodeForReport *pucchNodeForReport_p,
        UInt32 periodicity
        )
{
    UInt8 count =0;
    pucchNode * tempPucchNode_p = pucchNodeForReport_p->pucchNodePtr;

    if( tempPucchNode_p == PNULL )
    {
        count  = getCountForMaxNode(periodicity);
        GET_MEM_FROM_POOL(pucchNode,tempPucchNode_p,(count*sizeof(pucchNode)),PNULL);    
        if(PNULL == tempPucchNode_p)
        {
            ltePanic("Get Mem from Pool failed : Not able to create pucchNode\n");
            return MAC_FAILURE; 
        }
        pucchNodeForReport_p->pucchNodePtr = tempPucchNode_p;
   /** SPR 12320 Changes Start **/
        pucchNodeForReport_p->count = 0;
    /** SPR 12320 Changes End **/
    }
    return MAC_SUCCESS;
}
/* TDD SB_CQI */

/*****************************************************************************
 * Function Name  : createPUCCHMapForReportOrSR 
 * Inputs         : ueIndex - Ue-Index of current UE
 *                  pucchConfigInfo_p - PUCCH Config info for current UE.
 *                  currentUEContext_p - Its represent the UL UE Context.
 *                  subFrameOffset - Its represent the Periodic Report Offset
 *                  periodicity - Its represent the Periodicity of the Report.
 *                  reportType  - Its indicate the type of Periodic Report
 *                                SR, CQI , RI
 *                  maxWrapAround_p - Variable for storing max wrap around.
 *                  pucchResourceIndex - PUCCH resource index for the selected 
 *                                report
 *                  pucchFormat - PUCCH format for the selected periodic report.
 * Outputs        : create Map or List of Periodic Report Nodes.
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function create the Periodicity Map for Periodic Reports
 *                  SR, CQI, RI.
 *****************************************************************************/
STATIC MacRetType createPUCCHMapForReportOrSR( 
        /*SPR 16855 fix start*/
        UInt16  ueIndex,
        PucchConfig *pucchConfigInfo_p,
        /*SPR 16855 fix end*/
        ULUEContext  *currentUEContext_p,
        UInt32 subFrameOffset,
        UInt32 periodicity,
        ReportAndOpportunity reportType,
        UInt8 * maxWrapAround_p
        ,UInt32 pucchResourceIndex
        ,UInt32 pucchFormat
        ,UeCellInfo * ueCellInfo_p
        ,InternalCellIndex internalCellIndex                              
        )
{
    /*
     * Please visualise map as below
     *
     *  0 1 2 3 4 .............................. 159
     *  ___________________________________________
     * |_|_|_|_|_|_______________________________|_| 0th wrapround
     * |_|_|_|_|_|_______________________________|_| 1st wrapround
     * .                                           .
     * .                                           .
     * .                                           .
     * .                                           .
     * .                                           .
     *  ___________________________________________
     * |_|_|_|_|_|_______________________________|_| nth wrapround
     * 
     */
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    LTE_MAC_UT_LOG( LOG_INFO, PUCCH_MODULE,
            "subFrameOffset[%d] periodicity [%d],reportType [%d] pucchResourceIndex[%d]",
            subFrameOffset, periodicity, reportType, currentUEContext_p->pucchConfigInfo.srPUCCHRBIndex);
    /*    UInt16  ueIndex        = 0; TDD Warning Fix */
    UInt32 startOffset     = 0;
    UInt32 prevSubFrameOffset      = 0;
    UInt32 remaingToCompleteOneMap = 0;
    UInt32 remaingSubFrameOffset   = 0;
    UInt32 wrapRound       = 0;
    UInt32 totalNumberOfWrapRound   = 0;
    UInt32 totalSubFrameOffset      = 0;
    UInt32 startSecondWrap = 0 ;
    UInt32 firstWrapRound = 0 ; /* In case first subframe offset is > 160 */
    UInt8   rbIndexSlot1   = 0;
    UInt8   rbIndexSlot2   = 0;   
    /*SPR 16855 fix start*/
    UInt32 passiveIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
    /*SPR 16855 fix end*/

    /* TDD SB_CQI */
    pucchNodeForReport * pucchNodeForReport_p = PNULL;


    switch (reportType)
    {
        case SCHEDULING_REQUEST:
            {
                /*SPR 17583 Fix Start*/
                /*SPR 16855 fix start*/
                pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForSR[passiveIndex]);
                    /*SPR 16855 fix end*/
                break;
                /*SPR 17583 Fix End*/
            }
        /*Cov 69349 + */
        case WIDEBAND_CQI_REPORT:
            {
                /*EICIC +*/
                if (LTE_TRUE == updateCQIMapForAbs_g[internalCellIndex])
                {
                    /*SPR 16855 fix start*/
                    pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForWbCqi2[passiveIndex]);
                    /*SPR 16855 fix end*/
                }
                else
                {
                    /*EICIC -*/
                    /*SPR 16855 fix start*/
                    pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForWbCqi[passiveIndex]);
                    /*SPR 16855 fix end*/
                }
                break;
                /* Coverity_85631 Fix End */
            }
        case CQI_REPORT:
            {
                /*EICIC +*/
                if (LTE_TRUE == updateCQIMapForAbs_g[internalCellIndex])
                {
                    /*SPR 16855 fix start*/
                    pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForCqi2[passiveIndex]);
                    /*SPR 16855 fix end*/
                }
                else
                {
                    /*EICIC -*/
                    /*SPR 16855 fix start*/
                    pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForCqi[passiveIndex]);
                    /*SPR 16855 fix end*/
                }
                break;
            }
        case RI_REPORT:
            {
                /*EICIC +*/
                if (LTE_TRUE == updateCQIMapForAbs_g[internalCellIndex])
                {
                    /*SPR 16855 fix start*/
                    pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForRI2[passiveIndex]);
                    /*SPR 16855 fix end*/
                }
                else
                {
                    /*EICIC -*/
                    /*SPR 16855 fix start*/
                    pucchNodeForReport_p = 
                        &(pucchConfigInfo_p->pucchNodesForRI[passiveIndex]);
                    /*SPR 16855 fix end*/
                }

                break;

            }
            /*Cov 69349 -*/
        default :
            {
                LOG_MAC_ERROR(MAC_RRC_INF,"Invalid Report Type %u UE %u\n",
                reportType,ueIndex);
                /*+COVERITY 5.2.2- 32690*/
                return MAC_FAILURE;
                /*-COVERITY 5.2.2- 32690*/
            }
    }
    if(MAC_SUCCESS != allocMemForPucchNode(pucchNodeForReport_p,periodicity))
    {
        LOG_MAC_ERROR(MAC_RRC_INF,"allocMemForPucchNode Failed UE %u\n",ueIndex);
        LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
        return MAC_FAILURE;
    }
    /* TDD SB_CQI */

    /* Calling function already check.No need to check currentUEContext_p to PNULL.*/ 
    /*    ueIndex = currentUEContext_p->ueIndex; TDD Warning Fix */

    /* 
     * In case of RI, first subframe offset can be greater than 160, 
     * when the subframe offset turns out to be negative
     * please see getCqiRiPeriodicityAndOffset() 
     */
    if ( subFrameOffset >= MAX_SUBFRAME_OFFSET )
    {
        totalNumberOfWrapRound = ceil_wrapper((UDouble32) subFrameOffset / MAX_SUBFRAME_OFFSET);
        subFrameOffset = subFrameOffset % MAX_SUBFRAME_OFFSET;
        wrapRound = totalNumberOfWrapRound - 1;
        /* 
         * This is required so that we 
         * can position the totalSubFrameOffset 
         * value to the correct wrap around row starting 
         * (please see above diagram)
         */
        firstWrapRound = wrapRound; 
    }
    startOffset = subFrameOffset;
    prevSubFrameOffset = subFrameOffset;
    if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex( pucchResourceIndex,
                pucchFormat,
                &rbIndexSlot1,
                &rbIndexSlot2,
                internalCellIndex
                ))
    {
        freeMemPool((void *)pucchNodeForReport_p->pucchNodePtr);
        /*SPR 16979 Fix Start*/
        pucchNodeForReport_p->pucchNodePtr = PNULL;
        /*SPR 16979 Fix End*/
        LOG_MAC_ERROR(MAC_RRC_INF,"getPRBFromPUCCHResourceIndex Failed ue %u\n",ueIndex);
        return MAC_FAILURE;
    }   

    /** SPR 12491 Fix : Start**/
    pucchConfigInfo_p->prbIndexSlot1[passiveIndex][reportType] = rbIndexSlot1;
    pucchConfigInfo_p->prbIndexSlot2[passiveIndex][reportType] = rbIndexSlot2;
    /** SPR 12491 Fix : End **/

    updateNodeInPucchDb(
            /*SPR 16855 fix start*/
            ueIndex,
            /*SPR 16855 fix end*/
            pucchNodeForReport_p,
            currentUEContext_p,
            subFrameOffset,
            reportType, 
            wrapRound,
            ueCellInfo_p,
            internalCellIndex
            );
    subFrameOffset += periodicity;

    while (1) 
    { 
        if ( subFrameOffset >= MAX_SUBFRAME_OFFSET )
        {
            remaingToCompleteOneMap = MAX_SUBFRAME_OFFSET - prevSubFrameOffset;
            remaingSubFrameOffset = periodicity - remaingToCompleteOneMap;
            /* When first subframe offset was > 160 */
            if (firstWrapRound)
            {
                /*Go to the starting of that 160*nth row */
                totalSubFrameOffset = firstWrapRound * MAX_SUBFRAME_OFFSET; 
                startSecondWrap = firstWrapRound;
                firstWrapRound = 0;
            }

            totalSubFrameOffset += subFrameOffset ;
            totalNumberOfWrapRound = ceil_wrapper((UDouble32) totalSubFrameOffset / MAX_SUBFRAME_OFFSET);
            subFrameOffset = remaingSubFrameOffset % MAX_SUBFRAME_OFFSET;  
            if ( subFrameOffset == 0 )
            {
                totalNumberOfWrapRound++;
            }
            /*
             * Allign totalSubFrameOffset back to starting of the 160 row
             */
            totalSubFrameOffset -= subFrameOffset; 
            wrapRound = totalNumberOfWrapRound - 1;        
        } 

        /* When we have finally formed a loop, we end this journey */
        if ( subFrameOffset == startOffset )
        {
            LTE_MAC_UT_LOG(LOG_INFO, PUCCH_MODULE, "[%s]Start Point [%d] "
                    "End Point [%d]  maxWrapRound [%d]",
                    __func__,startOffset, prevSubFrameOffset,
                    wrapRound - startSecondWrap);
            //updateMaxWrapRound ( currentUEContext_p, (wrapRound- startSecondWrap) , reportType );
            (*maxWrapAround_p) = (wrapRound- startSecondWrap);
            break;
        }
        updateNodeInPucchDb(
                /*SPR 16855 fix start*/
                ueIndex,
                /*SPR 16855 fix end*/
                pucchNodeForReport_p,
                currentUEContext_p,
                subFrameOffset,
                reportType, 
                wrapRound,
                ueCellInfo_p,
                internalCellIndex
                );
        prevSubFrameOffset = subFrameOffset;
        subFrameOffset += periodicity;
    }
    /* SPR 16855 +- */
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
    return MAC_SUCCESS;
}

/* Rel9_upgrade_CR257 */
/*****************************************************************************
 * Function Name  : createPUCCHMapForShortSR 
 * Inputs         : ueIndex - Ue Index of current UE
 *                  pucchConfigInfo_p - PUCCH config info of current UE.
 *                  currentUEContext_p - Its represent the UL UE Context.
 *                  subFrameOffset - Its represent the Periodic Report Offset
 *                  periodicity - Its represent the Periodicity of the Report.
 *                  pucchResourceIndex - PUCCH resource index for selected 
 *                                       report.
 *                  internalCellIndex -  Cell-Index of PCell or SCell
 * Outputs        : create Map or List of Periodic Report Nodes.
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function create the Periodicity Map for Periodic Reports
 *                  SR.
 *****************************************************************************/
STATIC MacRetType createPUCCHMapForShortSR(
        /*SPR 16855 fix start*/
        UInt16 ueIndex,
        PucchConfig *pucchConfigInfo_p,
        /*SPR 16855 fix end*/
        UInt32 subFrameOffset,
        UInt32 periodicity
        ,UInt32 pucchResourceIndex
        ,InternalCellIndex internalCellIndex 
        )
{
    /* UInt32 startOffset     = 0;TDD Warning Fix */
    globalSrShortOpportunity *listNode_p = PNULL;
    globalSrShortOpportunity *nextlistNode_p = PNULL;
    UInt8 rbIndexSlot1 = 0;
    UInt8 rbIndexSlot2 = 0;
    /* SPR 10607 fix Start */
    globalSrShortOpportunity *prevNode_p = PNULL;
    /* SPR 10607 fix End */
    /* No need to check currentUEContext_p to PNULL,
       already checked in calling fn. */
    /* startOffset = subFrameOffset; TDD Warning Fix*/

    GET_MEM_FROM_POOL(globalSrShortOpportunity,listNode_p,
            sizeof(globalSrShortOpportunity) ,PNULL);
    /*coverity-530-25318*/
    if(PNULL == listNode_p)
    {
        LOG_MAC_ERROR(MAC_RRC_INF,"Memory Allocation failed for globalSrShortOpportunity listNode_p UE %u\n",
                ueIndex);
        return MAC_FAILURE;
    }
    /*coverity-530-25318*/
    listNode_p->nextNode_p = PNULL;
    /* If sr periodicty is 1 ms, there 2 nodes will be inserted in the map */
    if(periodicity == 1)
    {
        /* SPR 10607 fix Start */
        prevNode_p = listNode_p;
        /* SPR 10607 fix End */
        GET_MEM_FROM_POOL(globalSrShortOpportunity,nextlistNode_p,
                sizeof(globalSrShortOpportunity) ,PNULL);
        /*coverity-530-25318*/
        if(PNULL == nextlistNode_p)
        {
            LOG_MAC_ERROR(MAC_RRC_INF,"Memory Allocation failed for globalSrShortOpportunity nextlistNode_p UE %u\n",
                    ueIndex);
            freeMemPool((void *)listNode_p);
            return MAC_FAILURE;
        }
        /*coverity-530-25318*/
        listNode_p->nextNode_p = PNULL;
        /*coverity-530-25318*/
        /* SPR 10607 fix Start */
        prevNode_p->nextNode_p = listNode_p;
        /* SPR 10607 fix End */
    }

    /* Mapping of PUCCH Resouce Index to Physical Resource Block */
    if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex( pucchResourceIndex,
                UCI_FORMAT_ONE,
                &rbIndexSlot1,
                &rbIndexSlot2,
                internalCellIndex
                ))
    {
        freeMemPool((void *)listNode_p);
        /* Review comments fix start */
        listNode_p = PNULL;	
        if(periodicity == 1)
        {
            freeMemPool((void *)nextlistNode_p);
            nextlistNode_p = PNULL;
        }
        /* Review comments fix end */
        LOG_MAC_ERROR(MAC_RRC_INF,"getPRBFromPUCCHResourceIndex Failed %u\n",ueIndex);
        return MAC_FAILURE;
    }

    /*SPR 16855 fix start*/
    listNode_p->ueIndex = ueIndex;
    listNode_p->nextNode_p = PNULL;
    listInsertNode( &(pucchSrShortReportDb_g[internalCellIndex][shortPeriodicSRMapShortInfo_gp[internalCellIndex]->passiveIndex][subFrameOffset].ueList),
            &listNode_p->nodeAnchor);

    pucchConfigInfo_p->pucchShortSRNodeHead_p[shortPeriodicSRMapShortInfo_gp[internalCellIndex]->passiveIndex]   = listNode_p;
    pucchConfigInfo_p->srOffset       = subFrameOffset;
    pucchConfigInfo_p->srPeriodicity  = periodicity;
    pucchConfigInfo_p->prbIndexSlot1[shortPeriodicSRMapShortInfo_gp[internalCellIndex]->passiveIndex][SCHEDULING_REQUEST] = rbIndexSlot1;
    pucchConfigInfo_p->prbIndexSlot2[shortPeriodicSRMapShortInfo_gp[internalCellIndex]->passiveIndex][SCHEDULING_REQUEST] = rbIndexSlot2;
    /*SPR 16855 fix end*/

    if (periodicity == 1)
    {
        subFrameOffset += periodicity;
        /*SPR 16855 fix start*/
        nextlistNode_p->ueIndex = ueIndex;
        nextlistNode_p->nextNode_p = PNULL;
        listNode_p->nextNode_p = nextlistNode_p;
        listInsertNode( &(pucchSrShortReportDb_g[internalCellIndex][shortPeriodicSRMapShortInfo_gp[internalCellIndex]->passiveIndex][subFrameOffset].ueList), 
                &nextlistNode_p->nodeAnchor);
        /*SPR 16855 fix end*/

    }


    return MAC_SUCCESS;
}
/* Rel9_upgrade_CR257 */

/*****************************************************************************
 * Function Name  : getSRPeriodicityAndSrOffset
 * Inputs         : srConfigIndex - Its represent the SR configuration Index.
 *                  *srPeriodicity - Its indicate the SR Reports occur this 
 *                                    periodicity (Interval).
 *                  *srOffset - Its represent the which subframe start SR Reports.
 * Outputs        :
 * Returns        : MAC_SUCCES or MAC_FAILURE
 * Description    : This function fill the value of SR Periodicity and Sub Frame
 *                  offset in which subframe SR report is comming.
 *****************************************************************************/
STATIC MacRetType getSRPeriodicityAndSrOffset( UInt32 srConfigIndex,
        UInt8 *srPeriodicity, 
        UInt8 *subFrameOffset 
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry.",__func__);
    UInt32 index = 0;
    /* + coverity 32573 */
    MacRetType resp = MAC_FAILURE;
    /* - coverity 32573 */
    for ( index = 0; index < TOTAL_NUM_ROW_IN_SR_CONFIG  ; index ++ )
    {
        if ( srConfigIndex >= srConfigPeriodicityAndSubFrame_g[index].startIndex
                && srConfigIndex <=  srConfigPeriodicityAndSubFrame_g[index].endIndex )
        {
            *srPeriodicity  = srConfigPeriodicityAndSubFrame_g[index].srPeriodicity;
            *subFrameOffset =  srConfigPeriodicityAndSubFrame_g[index].srSubframeOffset +
                srConfigIndex;
            resp = MAC_SUCCESS; 
            break;
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,
            "[%s] srPeriodicity =%d, subFrameOffset =%d Exit.",
            __func__, *srPeriodicity, *subFrameOffset);
    return resp;
}
/*****************************************************************************
 * Function Name  : getCqiRiPeriodicityAndOffset
 * Inputs         : 
 *                  cqiPmiConfigIndex - Its represent the CQI configuration
 *                                      index lies between 0 -1023.
 *                  riConfigIndex - Its indicates the RI config Index and lies
 *                                   between 0 - 1023.
 *                  cqiPeriodicity , nOffsetCQI , riPeriodicity, nOffsetRI
 *                  These are the pointer and fill by this function 
 *                  H - It is used in RI periodcity calculation
 * Outputs        : Fills the cqiPeriodicity,,nOffsetCQI,riPeriodicity,nOffsetRI 
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function fills the value of cqiPeriodicity,nOffsetCQI
 *                  and riPeriodicity, nOffsetRI
 *****************************************************************************/
STATIC MacRetType getCqiRiPeriodicityAndOffset( 
        UInt32 cqiPmiConfigIndex,
        UInt32 riConfigIndex,
        UInt32 *cqiPeriodicity_p,
        UInt32 *nOffsetCQI_p,
        UInt32 *riPeriodicity_p,
        UInt32 *nOffsetRI_p,
        UInt8 H 
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry.",__func__);
    UInt32 idx  = 0;
    /* + coverity 32573 */
    MacRetType resp = MAC_FAILURE;
    /* - coverity 32573 */
    UInt32 cqiOffset = 0;
    UInt32 riOffset  = 0;
    UInt32 cqiPeriodicity = 0;
    UInt32 riPeriodicity  = 0;
    UInt32 tempRIOffset   = 0;
#ifdef TDD_CONFIG    
    for ( idx = 0 ; idx < TOTAL_NUM_ROW_IN_CQI_TDD_CONFIG; idx++)
#else
        for ( idx = 0 ; idx < TOTAL_NUM_ROW_IN_CQI_CONFIG; idx++)        
#endif        
        {
            /*check for cqiPeriodicity , riPeriodicity, , cqiOffset, riOffset*/
            if ((cqiPmiConfigIndex >= cqiPmiConfigTable_g[idx].cqiPmiConfigStartIndex &&
                        cqiPmiConfigIndex <= 
                        cqiPmiConfigTable_g[idx].cqiPmiConfigEndIndex) )
            {
                cqiPeriodicity = cqiPmiConfigTable_g[idx].cqiPeriodicity;
                cqiOffset = cqiPmiConfigTable_g[idx].nOffsetCQI + cqiPmiConfigIndex;
                break;
            }
        }
    *cqiPeriodicity_p = cqiPeriodicity;
    *nOffsetCQI_p     = cqiOffset;

    if (MAC_MAX_RI_CONFIG_INDEX >= riConfigIndex)
    { 
        for ( idx = 0 ; idx < TOTAL_NUM_ROW_IN_RI_CONFIG; idx++)
        {
            if  ((riConfigIndex  >= riConfigTable_g[idx].riConfigStartIndex &&
                        riConfigIndex <= riConfigTable_g[idx].riConfigEndIndex))
            {
                riPeriodicity = riConfigTable_g[idx].riPeriodicity;
                riOffset    = riConfigTable_g[idx].nOffsetRI + riConfigIndex;
                resp = MAC_SUCCESS;
                break;
            }
        }
        /*
         * In case of only wideband feedback:
         * NoffsetRi can be in the set {0,-1,...,-(Np-1)}
         * In case of wideband + +subband feedback
         * NoffsetRi can be in the set {0,-1,...,-(Np)}
         */
        /* + CQI_5.0 */
        if (riOffset >= cqiPeriodicity)  
            /* - CQI_5.0 */
        {
            lteWarning("Incorrect riConfigIndex, riOffset \
                    should be in the range {0,-1,..,-Np} for wideband+subband\
                    and {0,-1,..-(Np-1)} for only wideband\n");
            resp = MAC_FAILURE;
        }
        else
        {         

            /*
             * we will subtract since riOffset is supposed to be negative 
             * and we've stored it as a positive entity
             */
            tempRIOffset = abs_wrapper(cqiOffset - riOffset); 
            riPeriodicity = cqiPeriodicity * riPeriodicity * H;

            /*
             * Consider the formula for RI eligible subframes:
             * (X - CQIoffset - RIoffset)%riPeriodicty
             * Now since RIoffset is actually negative, 
             * so in the case where abs_wrapper(riOffset) > abs_wrapper(cqiOffset)
             * total offset will be added to X. 
             * Hence starting offset will be when (X + totalloffset) = riPeriodicty
             * i.e X should be riPeriodicty - totalloffset
             */
            if (riOffset > cqiOffset)
            {
                tempRIOffset = abs_wrapper(riPeriodicity - tempRIOffset);
            }

            *nOffsetRI_p = tempRIOffset;
            *riPeriodicity_p = riPeriodicity;
        }
    }
    else
    {
        resp = MAC_SUCCESS;
    }
    LTE_MAC_UT_LOG(LOG_INFO, PUCCH_MODULE,"[%s] cqiPeriodicity =%d cqiOffset =%d"
            "riPeriodicity_p =%d,nOffsetRI_p =%d",
            __func__,*cqiPeriodicity_p,*nOffsetCQI_p,
            *riPeriodicity_p,*nOffsetRI_p );

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
    return resp;
}

/*****************************************************************************
 * Function Name  : initCQIAndRIinPUCCHMap
 * Inputs         : ueIndex - UE Index of current UE
 *                  pucchConfigInfo_p - PUCCH Config info of current UE
 *                  currentUEContext_p - ul context of the current UE
 *                  dwlkUEInfo_p - Ue Context for Downlink
 *                  uplkUEInfo_p - Ue Context for Uplink
 *                  ueCellInfo_p, J
 *                  internalCellIndex - Cell-Index of Pcell or Scell
 *                  rrcLoopIndex -
 * Outputs        :
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function initializes the pucch db for all the possible
 *                  reports and SR opportunities to be processed for given UE
 *****************************************************************************/
/* SPR 2617 Start*/
MacRetType initCQIAndRIinPUCCHMap (
        /*SPR 16855 fix start*/
        UInt16 ueIndex,
        PucchConfig *pucchConfigInfo_p,
        /*SPR 16855 fix end*/
        ULUEContext  *currentUEContext_p,
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink*  uplkUEInfo_p,
        UeCellInfo * ueCellInfo_p,
        UInt8 J,
        /*SPR 11541 Start */
        InternalCellIndex internalCellIndex,
        UInt8 rrcLoopIndex
        /*SPR 11541 End*/
        )
/* SPR 2617 End*/
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry.",__func__);
    UInt32 cqiPeriodicity    = 0;
    UInt32 cqiSubFrameOffset = 0;
    UInt32 riPeriodicity     = 0;
    UInt32 riSubFrameOffset  = 0;
    /* SPR 2617 Start*/
    UInt32 cqiPMIConfigIndex = uplkUEInfo_p->cqiPMIConfigIndex;
    UInt32 riConfigIndex = uplkUEInfo_p->riConfigIndex;
    /*EICIC +*/
    UInt32 cqiPeriodicity_2    = 0;
    UInt32 cqiSubFrameOffset_2 = 0;
    UInt32 riPeriodicity_2     = 0;
    UInt32 riSubFrameOffset_2  = 0;
    /* SPR 2617 Start*/
    UInt32 cqiPMIConfigIndex_2 = uplkUEInfo_p->cqiPMIConfigIndex_2;
    UInt32 riConfigIndex_2 = uplkUEInfo_p->riConfigIndex_2;
    /*EICIC -*/

    UInt32 cqiPucchResourceIndex = uplkUEInfo_p->cqiPucchResourceIndex;
#ifdef TDD_CONFIG
    J = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->bandwidthParts;
#endif
    /* SPR 11683 Fix start */
    UInt8 ri = dwlkUEInfo_p->rankIndicator;
    CQIMode cqiMode = dwlkUEInfo_p->cqiMode;
    TransmissonMode  transmissionMode = dwlkUEInfo_p->transmissionMode; 
    UInt8 pmiRiReportV920 = dwlkUEInfo_p->pmiRiReportV920;
    /* SPR 11683 Fix end */
    UInt8   K = uplkUEInfo_p->cqiformatIndicatorPeriodic_subband_k;
    UInt8   H = 1; /* Since valid valeu of H is always greater than 1 */

    PeriodicFeedbackType periodicFeedBack = uplkUEInfo_p->cqiformatIndicatorPeriodic;
    /*SPR 16855 fix start*/
    UInt32 passiveIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
    /*SPR 19623 +*/
    if(!ueCellInfo_p->isPCell)
    { 
        /* Periodic CQI Configurtion received for Scell of UE, UE should be
         * Rel10 Compliance.So If ueCat v1020 is valid then parse otherwise 
         * return failure.*/
    	ULUEContext *ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
        if((INVALID_UE_CATEGORY != dwlkUEInfo_p->ueCategoryV1020) || (RELEASE_10 == ulUEContext_p->ueComplianceRelease))    
        {
            /*SPR 11541 Start */
            cqiPMIConfigIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[rrcLoopIndex].\
                                cqiInfo.cqiPMIConfigIndexV1020;
            /*EICIC +*/
            cqiPMIConfigIndex_2 = uplkUEInfo_p->carrierAggrConfig.scellInfo[rrcLoopIndex].\
                                  cqiInfo.cqiPMIConfigIndex_2;
            /*EICIC -*/
            riConfigIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[rrcLoopIndex].\
                            cqiInfo.riConfigIndexV1020;
            /*EICIC +*/
            riConfigIndex_2 = uplkUEInfo_p->carrierAggrConfig.scellInfo[rrcLoopIndex].\
                              cqiInfo.riConfigIndex_2;
            /*EICIC -*/
            cqiPucchResourceIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[rrcLoopIndex].\
                                    cqiInfo.cqiPucchResourceIndexV1020;
            K = uplkUEInfo_p->carrierAggrConfig.scellInfo[rrcLoopIndex].\
                cqiInfo.cqiformatIndicatorPeriodic_subband_kV1020;
            /*Coverity 63969 fix starts*/
            periodicFeedBack = (PeriodicFeedbackType)uplkUEInfo_p->carrierAggrConfig.scellInfo[rrcLoopIndex].\
                               cqiInfo.cqiformatIndicatorPeriodicV1020;
            /*Coverity 63969 fix ends*/
            passiveIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
            /* SPR 11683 Fix */
            ri = dwlkUEInfo_p->carrierAggrConfig.scellInfo[rrcLoopIndex].\
                 rankIndicator;
            cqiMode = dwlkUEInfo_p->carrierAggrConfig.scellInfo[rrcLoopIndex].\
                      cqiInfo.cqiMode;
            transmissionMode = dwlkUEInfo_p->carrierAggrConfig.scellInfo[rrcLoopIndex].\
                               transmissionMode;
            pmiRiReportV920 = dwlkUEInfo_p->carrierAggrConfig.scellInfo[rrcLoopIndex].\
                              cqiInfo.pmiRiReportV920;
            /*SPR 11541 End*/
        }
        else
        {
            LOG_MAC_ERROR(MAC_RRC_INF,"Scell Configuring, ueCat should be Rel10 compliance %u\n",ueIndex);
            return MAC_FAILURE;
        }
    }
    /*SPR 19623 -*/
    /*SPR 16855 fix start*/

    /* SPR 2617 End*/
    /* Calling function checks that currentUEContext_p is not NULL */

    /* cqi feedback will be both wideband + subband*/
    if (PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack) 
    {
        //calculate H Factor
        H = (J*K) + 1; /* If H is set, it will always be greater than 1 */
    }

    if ( MAC_SUCCESS == getCqiRiPeriodicityAndOffset( 
                cqiPMIConfigIndex, 
                riConfigIndex, 
                &cqiPeriodicity,
                &cqiSubFrameOffset,
                &riPeriodicity,
                &riSubFrameOffset,
                H))
    {
        /*
         * cqiPeriodicity will be returned without being multiplied by H
         * because we need it for subband cqi reporting instances in case of
         * wideband + subband reporting
         * 
         * riPeriodicity will on the other hand be returned after 
         * being multiplied by H
         */
        LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,
                "[%s]=== Creating CQI Nodes Map==",__func__);
        if (MAC_FAILURE == createPUCCHMapForReportOrSR(
                    /*SPR 16855 fix start*/
                    ueIndex,
                    pucchConfigInfo_p,
                    /*SPR 16855 fix end*/
                    currentUEContext_p, 
                    cqiSubFrameOffset, 
                    cqiPeriodicity,
                    CQI_REPORT /*reportType*/,
                    /*SPR 16855 fix start*/
                    &(pucchConfigInfo_p->maxCQIWrapAround[passiveIndex])
                    /*SPR 16855 fix end*/
                    ,cqiPucchResourceIndex
                    ,UCI_FORMAT_TWO
                    ,ueCellInfo_p
                    ,internalCellIndex
                    ) ) 
                    {
                        LOG_MAC_ERROR(MAC_RRC_INF,"Couldnt create PUCCH map for CQI UE %u\n",
                                ueIndex);
                        return MAC_FAILURE;       
                    }
        /* when feedback type is wideband + subband*/
        if (PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack) 
        {
            if (MAC_FAILURE == createPUCCHMapForReportOrSR(
                        /*SPR 16855 fix start*/
                        ueIndex,
                        pucchConfigInfo_p,
                        /*SPR 16855 fix end*/
                        currentUEContext_p, 
                        cqiSubFrameOffset, 
                        cqiPeriodicity*H,
                        WIDEBAND_CQI_REPORT /*reportType*/,
                        /*SPR 16855 fix start*/
                        &(pucchConfigInfo_p->maxWbCQIWrapAround[passiveIndex])
                        /*SPR 16855 fix end*/
                        ,cqiPucchResourceIndex
                        ,UCI_FORMAT_TWO
                        ,ueCellInfo_p
                        ,internalCellIndex
                        ) ) 
                        {
                            LOG_MAC_ERROR(MAC_RRC_INF,"Couldnt create PUCCH map for WBCQI UE %u\n",
                                    ueIndex);
                            return MAC_FAILURE;       
                        }
        }

        pucchConfigInfo_p->cqiPeriodicity = cqiPeriodicity;
        pucchConfigInfo_p->cqiPUCCHRBIndex = cqiPucchResourceIndex;
        /* TDD SB_CQI */
        pucchConfigInfo_p->cqiPmiRiInfo.widebandPeriodicity = cqiPeriodicity*H;
        /* TDD SB_CQI */
        /*SPR 16855 fix end*/

        LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,
                "[%s]==== Creating RI Nodes Map =====",__func__);
        if ( MAC_MAX_RI_CONFIG_INDEX /*MAC_RI_CONFIG_INDEX_INVALID*/ >= riConfigIndex)
        {    
            if (MAC_FAILURE == createPUCCHMapForReportOrSR(
                        /*SPR 16855 fix start*/
                        ueIndex,
                        pucchConfigInfo_p, 
                        /*SPR 16855 fix end*/
                        currentUEContext_p, 
                        riSubFrameOffset, 
                        riPeriodicity,
                        RI_REPORT /*reportType*/,
                        /*SPR 16855 fix start*/
                        &(pucchConfigInfo_p->maxRIWrapAround[passiveIndex])
                        /*SPR 16855 fix end*/
                        ,cqiPucchResourceIndex
                        ,UCI_FORMAT_TWO
                        ,ueCellInfo_p
                        ,internalCellIndex
                        ) )
                        {
                            LOG_MAC_ERROR(MAC_RRC_INF,"Couldnt create PUCCH map for RI UE %u\n",
                                    ueIndex);
                            return MAC_FAILURE;       
                        }
        }

    }
    else
    {
        LOG_MAC_ERROR(MAC_RRC_INF,"getCqiRiPeriodicityAndOffset Failed UE %u\n",
                ueIndex);
        return MAC_FAILURE;
    }
    /*SPR 2617 Start*/
    /* TDD SB_CQI*/
    pucchConfigInfo_p->cqiOffset = cqiSubFrameOffset;
    pucchConfigInfo_p->riOffset = riSubFrameOffset;
    /* TDD SB_CQI*/
    /*SPR 16855 fix start*/
    /* + CL-MIMO LJA*/
    /* SPR 11683 Fix start */
    pucchConfigInfo_p->ueReportedRI =  ri;
    /* - CL-MIMO LJA*/
    /*periodic cqi decoding changes*/
    pucchConfigInfo_p->ueReportedPeriodicRI = ri;
    /*SPR 16855 fix end*/
    pucchConfigInfo_p->cqiPmiRiInfoAperiodic.cqiMode = cqiMode;
    pucchConfigInfo_p->cqiPmiRiInfo.periodicFeedBackType = 
        periodicFeedBack;

    pucchConfigInfo_p->cqiPmiRiInfo.k = K; 
    pucchConfigInfo_p->cqiPmiRiInfo.periodicRepMode = 
        getPeriodicReportingMode(transmissionMode, 
                pucchConfigInfo_p->cqiPmiRiInfo.periodicFeedBackType,
                /* + TM7_8 Changes Start */
                pmiRiReportV920);
    /* SPR 11683 Fix end */
    /* - TM7_8 Changes End */
    pucchConfigInfo_p->cqiPmiConfigIndex =  cqiPMIConfigIndex;
    pucchConfigInfo_p->riConfigIndex = riConfigIndex;
    /* TDD SB_CQI */
    pucchConfigInfo_p->riPeriodicity = riPeriodicity;
    /* TDD SB_CQI */
    /* SS_S2 */
    /*EICIC +*/
    if (uplkUEInfo_p->cqiConfigRequestType == SETUP)
    {


        if ( MAC_SUCCESS == getCqiRiPeriodicityAndOffset( 
                    cqiPMIConfigIndex_2, 
                    riConfigIndex_2, 
                    &cqiPeriodicity_2,
                    &cqiSubFrameOffset_2,
                    &riPeriodicity_2,
                    &riSubFrameOffset_2,
                    H))
        {
            /*
             * cqiPeriodicity will be returned without being multiplied by H
             * because we need it for subband cqi reporting instances in case of
             * wideband + subband reporting
             * 
             * riPeriodicity will on the other hand be returned after 
             * being multiplied by H
             */
            LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,
                    "[%s]=== Creating CQI Nodes Map==",__func__);

            updateCQIMapForAbs_g[internalCellIndex] = LTE_TRUE;


            if (MAC_FAILURE == createPUCCHMapForReportOrSR(
                        /*SPR 16855 fix start*/
                        ueIndex,
                        pucchConfigInfo_p, 
                        /*SPR 16855 fix end*/
                        currentUEContext_p, 
                        cqiSubFrameOffset_2, 
                        cqiPeriodicity_2,
                        CQI_REPORT /*reportType*/,
                        /*SPR 16855 fix start*/
                        &(pucchConfigInfo_p->maxCQIWrapAround2[passiveIndex])
                        /*SPR 16855 fix end*/
                        ,cqiPucchResourceIndex
                        ,UCI_FORMAT_TWO
                        ,ueCellInfo_p
                        ,internalCellIndex
                        ) ) 
                        {
                            LOG_MAC_ERROR(MAC_RRC_INF,"Couldnt create PUCCH map for CQI UE %u\n",
                                    ueIndex);
                            updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
                            return MAC_FAILURE;      

                        }
            /* when feedback type is wideband + subband*/
            if (PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack) 
            {
                if (MAC_FAILURE == createPUCCHMapForReportOrSR(
                            /*SPR 16855 fix start*/
                            ueIndex,
                            pucchConfigInfo_p, 
                            /*SPR 16855 fix end*/
                            currentUEContext_p, 
                            cqiSubFrameOffset_2, 
                            cqiPeriodicity_2*H,
                            WIDEBAND_CQI_REPORT /*reportType*/,
                            /*SPR 16855 fix start*/
                            &(pucchConfigInfo_p->maxWbCQIWrapAround2[passiveIndex])
                            /*SPR 16855 fix end*/
                            ,cqiPucchResourceIndex
                            ,UCI_FORMAT_TWO
                            ,ueCellInfo_p
                            ,internalCellIndex
                            ) ) 
                            {
                                LOG_MAC_ERROR(MAC_RRC_INF,"Couldnt create PUCCH map for WBCQI UE %u\n",
                                        ueIndex);
                                updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
                                return MAC_FAILURE;       
                            }
            }

            pucchConfigInfo_p->cqiPeriodicity_2 = cqiPeriodicity_2;
            pucchConfigInfo_p->cqiPUCCHRBIndex = cqiPucchResourceIndex;
            /* TDD SB_CQI */
            pucchConfigInfo_p->cqiPmiRiInfo.widebandPeriodicity_2 = cqiPeriodicity_2*H;

            /* TDD SB_CQI */
            /*SPR 16855 fix end*/

            LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,
                    "[%s]==== Creating RI Nodes Map =====",__func__);
            if ( MAC_MAX_RI_CONFIG_INDEX /*MAC_RI_CONFIG_INDEX_INVALID*/ >= riConfigIndex)
            {    
                if (MAC_FAILURE == createPUCCHMapForReportOrSR(
                            /*SPR 16855 fix start*/
                            ueIndex,
                            pucchConfigInfo_p, 
                            /*SPR 16855 fix end*/
                            currentUEContext_p, 
                            riSubFrameOffset_2, 
                            riPeriodicity_2,
                            RI_REPORT /*reportType*/,
                            /*SPR 16855 fix start*/
                            &(pucchConfigInfo_p->maxRIWrapAround2[passiveIndex])
                            /*SPR 16855 fix end*/
                            ,cqiPucchResourceIndex
                            ,UCI_FORMAT_TWO
                            ,ueCellInfo_p
                            ,internalCellIndex
                            ) )
                            {
                                LOG_MAC_ERROR(MAC_RRC_INF,"Couldnt create PUCCH map for RI UE %u\n",
                                        ueIndex);
                                updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
                                return MAC_FAILURE;       
                            }
            }

        }
        else
        {
            LOG_MAC_ERROR(MAC_RRC_INF,"getCqiRiPeriodicityAndOffset Failed UE %u\n",ueIndex);
            updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
            return MAC_FAILURE;
        }
        pucchConfigInfo_p->cqiPmiConfigIndex_2 =  cqiPMIConfigIndex_2;
        pucchConfigInfo_p->riConfigIndex_2 = riConfigIndex_2;
        pucchConfigInfo_p->riPeriodicity_2 = riPeriodicity_2;
        pucchConfigInfo_p->cqiOffset_2 = cqiSubFrameOffset_2;
        pucchConfigInfo_p->riOffset_2 = riSubFrameOffset_2;
    }


    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit.",__func__);
    updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
    /*EICIC -*/
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : initSchedReqinPUCCHMap
 * Inputs         : ueIndex - UE Index of current UE
 *                  pucchConfigInfo_p - PUCCH Config Index of current UE
 *                  currentUEContext_p - ul context of the current UE
 *                  srConfigurationIndex - Represents SR configuration index 
 *                  srPucchResourceIndex - Its represent the PUCCH Resource Index
 *                  srProhibitTimerValv920 - Represents the PUCCH sr prohibit 
 *                           timer
 *                  internalCellIndex - Cell-Index of Pcell or Scell
 * Outputs        :
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function initializes the pucch db for SR 
 *                  opportunities to be processed for given UE
 *****************************************************************************/
MacRetType initSchedReqInPUCCHMap(
        /*SPR 16855 fix start*/
        UInt16  ueIndex,
        PucchConfig   *pucchConfigInfo_p,
        /*SPR 16855 fix end*/
        ULUEContext  *currentUEContext_p,
        UInt32 srConfigurationIndex,
        UInt32 srPucchResourceIndex,
        UInt16 srProhibitTimerValv920,
        InternalCellIndex internalCellIndex
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry.",__func__);
    UInt8 srPeriodicity    = 0;
    UInt8 srSubFrameOffset = 0;
    UeCellInfo ueCellInfo = {0};
    ueCellInfo.isPCell = TRUE;
    /*Calling function checks that currentUEContext_p is not null*/
    if (MAC_SUCCESS == getSRPeriodicityAndSrOffset( srConfigurationIndex, 
                &srPeriodicity, &srSubFrameOffset))
    {
        LTE_MAC_UT_LOG(LOG_INFO, PUCCH_MODULE,
                "[%s] SRConfIndex = %d,srPeriodicity =%d,srSubFrameOffset =%d",
                __func__, srConfigurationIndex,srPeriodicity,srSubFrameOffset);
        LTE_MAC_UT_LOG(LOG_INFO, PUCCH_MODULE,
                "[%s]***** Creating SR Nodes Map.*****",__func__);
        /* Rel9_upgrade_CR257 */
        if (srPeriodicity <= SHORT_SR_PERIODICITY )
        {
            /*SPR 16855 fix start*/
            /*coverity-530-25318*/
            if(MAC_FAILURE == createPUCCHMapForShortSR( ueIndex,
                        pucchConfigInfo_p,
                        srSubFrameOffset,
                        srPeriodicity
                        ,srPucchResourceIndex
                        ,internalCellIndex
                        ))
           /*SPR 16855 fix end*/     
                {
                    LOG_MAC_MSG(MAC_PUCCH_INIT_SR_MAP_FAIL,LOGERROR,MAC_PUCCH,\
                            getCurrentTick(),\
                            ueIndex,\
                            srConfigurationIndex,\
                            srPeriodicity,\
                            0,0,0.0,0.0,__func__,"PUCCH_SR_MAP_RECONFIG_FAILED");

                    return MAC_FAILURE;
                }
            /*coverity-530-25318*/
        }
        else
        {
            /* Rel9_upgrade_CR257 */
            if ( MAC_FAILURE == createPUCCHMapForReportOrSR(
                        /*SPR 16855 fix start*/
                        ueIndex,
                        pucchConfigInfo_p, 
                        /*SPR 16855 fix end*/
                        currentUEContext_p, 
                        srSubFrameOffset, 
                        srPeriodicity,
                        SCHEDULING_REQUEST /*reportType*/,
                        /*SPR 16855 fix start*/
                        &(pucchConfigInfo_p->maxSrWrapAround\
                            [longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex])
                        /*SPR 16855 fix end*/
                        ,srPucchResourceIndex
                        ,UCI_FORMAT_ONE
                        ,&ueCellInfo
                        ,internalCellIndex
                        ))
                        {
                            LOG_MAC_ERROR(MAC_RRC_INF,"Couldnt create PUCCH map for SR UE %u\n",
                                    ueIndex);
                            return MAC_FAILURE;
                        }
        }
        /*SPR 16855 fix start*/
        pucchConfigInfo_p->srPUCCHRBIndex       = srPucchResourceIndex;
        pucchConfigInfo_p->srPeriodicity  = srPeriodicity;
        pucchConfigInfo_p->srProhibitTimerValv920  = srProhibitTimerValv920 * srPeriodicity;
        /* TDD SB_CQI */
        pucchConfigInfo_p->srOffset = srSubFrameOffset; 
        /* TDD SB_CQI */
        /*SPR 16855 fix end*/


    }

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit",__func__);
    return MAC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : insertSrOrReportNodeInPucchDb
 * Inputs         : ueIndex - Ue Index of current UE
 *                  subFrameOffset - current Sub frame offset
 *                  ueReportOrOpportunityType - Periodic Report type
 *                  ueCellInfo_p -
 *                  internalCellIndex - Cell-Index of Pcell or Scell
 * Outputs        : This function returns the periodic node
 * Returns        : globalSrOpportunityAndPeriodicReports *
 * Description    : Inserts node in Pucch DB using the input values
 *****************************************************************************/
/* OPT */
/*SPR 16855 fix start*/
STATIC globalSrOpportunityAndPeriodicReports * insertSrOrReportNodeInPucchDb(
        UInt16  ueIndex,
        UInt16 subFrameOffset,
        UInt8 ueReportOrOpportunityType,
        UeCellInfo * ueCellInfo_p,
        InternalCellIndex internalCellIndex
        )
/*SPR 16855 fix end*/
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    globalSrOpportunityAndPeriodicReports   * srOppAndReportNode_p = PNULL;
    /*SPR 16855 fix start*/
    UInt32 passiveIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
    /*SPR 16855 fix end*/
    if (PNULL == (srOppAndReportNode_p =
                createSrOppAndReportNode(ueIndex,ueReportOrOpportunityType
                    ,ueCellInfo_p
                    ) ) )
        {
            /*
             * Log error and neturn
             */
            ltePanic("Could not create node for pucch db\n");
            return PNULL;
        }
    /*SPR 16855 fix start*/
    listInsertNode(&(pucchSrAndReportsDb_g[internalCellIndex][passiveIndex][subFrameOffset].ueList),
            &srOppAndReportNode_p->nodeAnchor);
    /*SPR 16855 fix end*/

    if (!ueCellInfo_p->isPCell)
    {
        srOppAndReportNode_p->refCntr++;
    }
    /*
     * Have to calculate the PRB at runtime because we dont know the pucchFormat yet
     */
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
    return srOppAndReportNode_p;
}
/* OPT */

/*****************************************************************************
 * Function Name  : createSrOppAndReportNode
 * Inputs         : ueIndex - Ue Index of current UE
 *                  type - Periodic report type
 *                  ueCellInfo_p -
 * Outputs        :
 * Returns        :globalSrOpportunityAndPeriodicReports * 
 * Description    : Function to create periodic node
 *****************************************************************************/
STATIC  globalSrOpportunityAndPeriodicReports * createSrOppAndReportNode(
        UInt32 ueIndex,
        UInt8 type
        ,UeCellInfo * ueCellInfo_p
        )

{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    globalSrOpportunityAndPeriodicReports   * nodeEntry_p = PNULL;
    GET_MEM_FROM_POOL(globalSrOpportunityAndPeriodicReports,nodeEntry_p,
            sizeof(globalSrOpportunityAndPeriodicReports) ,PNULL);
    /*coverity-530-25319*/
    if(nodeEntry_p && ueCellInfo_p->isPCell)
        {
            /* SPR 11109 FIX START */    
            memSet(nodeEntry_p,0,sizeof(globalSrOpportunityAndPeriodicReports)); 
            /* SPR 11109 FIX END */    
            nodeEntry_p->ueIndex = ueIndex;
            nodeEntry_p->reportBitMapPCell.ueReportAndOpportunity = type;
            LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
        }
        else if(nodeEntry_p && !ueCellInfo_p->isPCell)
        {
            /* SPR 11109 FIX START */    
            memSet(nodeEntry_p,0,sizeof(globalSrOpportunityAndPeriodicReports)); 
            /* SPR 11109 FIX END */    
            nodeEntry_p->ueIndex = ueIndex;
            nodeEntry_p->reportBitMapSCell[(ueCellInfo_p->sCellIndex - 1)].ueReportAndOpportunity = type;
            LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
        }
    return nodeEntry_p;
    /*coverity-530-25319*/
}
/* OPT */
/*ca-tdd pucch code changes start*/
/*****************************************************************************
 * Function Name  : getValidReportTypesForWrapAroundsForSCell
 * Inputs         : node_p - Periodic report node
 *                  ulUEContext_p - UL UE Context
 *                  totalWrapNum - Wraparound count
 *                  sCellIndex -
 *                  internalCellIndex - Cell-Index of Pcell or Scell
 * Outputs        :
 * Returns        :
 * Description    : Based on the wrap around values, this function returns the
 *                  actual valid report bitstring
 *****************************************************************************/
STATIC UInt8 getValidReportTypesForWrapAroundsForSCell(
        globalSrOpportunityAndPeriodicReports * node_p, 
        ULUEContext *ulUEContext_p, 
        UInt16 totalWrapNum,
        UInt8 sCellIndex,
        InternalCellIndex  internalCellIndex)
{
    UInt8 validReportString = 0x0;
    ScellPucchConfig * scellpucchConfigInfo_p =\
                                               &(ulUEContext_p->ulUeScellContext_p[sCellIndex]->scellpucchConfigInfo);
    /*SPR 16855 fix start*/
    UInt32 activeIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex;
    /*SPR 16855 fix end*/

    if (CHECK_BIT_SET(node_p->reportBitMapSCell[(sCellIndex - 1)].ueReportAndOpportunity, CQI_REPORT) )
    {
        if (
                /*SPR 16855 fix start*/
                scellpucchConfigInfo_p->maxCQIWrapAround[activeIndex] != 0
                /*SPR 16855 fix end*/
           ) 
        {
        if (node_p->reportBitMapSCell[(sCellIndex - 1)].exactCQIWrapRound == 
                /*SPR 16855 fix start*/
                (totalWrapNum % scellpucchConfigInfo_p->maxCQIWrapAround[activeIndex]) 
                /*SPR 16855 fix end*/
           )
        {
            SET_BIT_AT_POS(validReportString, CQI_REPORT);
        }
        }
        else if (
                /*SPR 16855 fix start*/
                scellpucchConfigInfo_p->maxCQIWrapAround2[activeIndex] != 0
                /*SPR 16855 fix end*/
                ) 
        {
            if (node_p->reportBitMapSCell[(sCellIndex - 1)].exactCQIWrapRound2 ==
                    /*SPR 16855 fix start*/
                    (totalWrapNum % scellpucchConfigInfo_p->maxCQIWrapAround2[activeIndex]) 
                    /*SPR 16855 fix end*/
               )
            {
                SET_BIT_AT_POS(validReportString, CQI_REPORT);
            }
        }
    }    
    if (CHECK_BIT_SET(node_p->reportBitMapSCell[(sCellIndex - 1)].ueReportAndOpportunity, WIDEBAND_CQI_REPORT) )
    {

        if (
                /*SPR 16855 fix start*/
                scellpucchConfigInfo_p->maxWbCQIWrapAround[activeIndex] != 0
                /*SPR 16855 fix end*/
           ) 
        {
        if (node_p->reportBitMapSCell[(sCellIndex - 1)].exactWbCQIWrapRound ==
                /*SPR 16855 fix start*/
                (totalWrapNum % scellpucchConfigInfo_p->maxWbCQIWrapAround[activeIndex]) 
                /*SPR 16855 fix end*/
           )
        {
            SET_BIT_AT_POS(validReportString, WIDEBAND_CQI_REPORT);
        }
        }
        else if (
                /*SPR 16855 fix start*/
                scellpucchConfigInfo_p->maxWbCQIWrapAround2[activeIndex] != 0
                /*SPR 16855 fix end*/
                ) 
        {
            if (node_p->reportBitMapSCell[(sCellIndex - 1)].exactWbCQIWrapRound2 == 
                    /*SPR 16855 fix start*/
                    (totalWrapNum % scellpucchConfigInfo_p->maxWbCQIWrapAround2[activeIndex]) 
                    /*SPR 16855 fix end*/
               )
            {
                SET_BIT_AT_POS(validReportString, WIDEBAND_CQI_REPORT);
            }
        }
    }    

    if (CHECK_BIT_SET(node_p->reportBitMapSCell[(sCellIndex - 1)].ueReportAndOpportunity, RI_REPORT) )
    {

        if (
                /*SPR 16855 fix start*/
                scellpucchConfigInfo_p->maxRIWrapAround[activeIndex] != 0
                /*SPR 16855 fix end*/
           ) 
        {
        if (node_p->reportBitMapSCell[(sCellIndex - 1)].exactRIWrapRound == 
                /*SPR 16855 fix start*/
                (totalWrapNum % scellpucchConfigInfo_p->maxRIWrapAround[activeIndex]) 
                /*SPR 16855 fix end*/
           )
        {
            SET_BIT_AT_POS(validReportString, RI_REPORT);
        }
        }
        else if (
                /*SPR 16855 fix start*/
                scellpucchConfigInfo_p->maxRIWrapAround2[activeIndex] != 0
                /*SPR 16855 fix end*/
                ) 
        {
            if (node_p->reportBitMapSCell[(sCellIndex - 1)].exactRIWrapRound2 ==
                    /*SPR 16855 fix start*/
                    (totalWrapNum % scellpucchConfigInfo_p->maxRIWrapAround2[activeIndex]) 
                    /*SPR 16855 fix end*/
               )
            {
                SET_BIT_AT_POS(validReportString, RI_REPORT);
            }
        }
    }    
    return validReportString;
}
/*ca-tdd pucch code changes end*/
/*****************************************************************************
 * Function Name  : getValidReportTypesForWrapArounds
 * Inputs         : node_p - Periodic report node
 *                  ulUEContext_p - UL UE Context
 *                  totalWrapNum - Wraparound count
 *                  internalCellIndex - Cell-Index of Pcell or Scell
 * Outputs        :
 * Returns        :
 * Description    : Based on the wrap around values, this function returns the
 *                  actual valid report bitstring
 *****************************************************************************/
STATIC UInt8 getValidReportTypesForWrapArounds(
        globalSrOpportunityAndPeriodicReports * node_p, 
        ULUEContext *ulUEContext_p, 
        UInt16 totalWrapNum,
        InternalCellIndex internalCellIndex)
{
    UInt8 validReportString = 0x0;

    if (CHECK_BIT_SET(node_p->reportBitMapPCell.ueReportAndOpportunity, SCHEDULING_REQUEST) )
    {
        if (node_p->reportBitMapPCell.exactSrWrapRound ==
                /*SPR 16855 fix start*/
                (totalWrapNum % ulUEContext_p->pucchConfigInfo.maxSrWrapAround[
                 longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex]) 
                /*SPR 16855 fix end*/
           )
        {
            SET_BIT_AT_POS(validReportString, SCHEDULING_REQUEST);
        }
    }
    if (CHECK_BIT_SET(node_p->reportBitMapPCell.ueReportAndOpportunity, CQI_REPORT) )
    {
        if (
                /*SPR 16855 fix start*/
                ulUEContext_p->pucchConfigInfo.maxCQIWrapAround[
                longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex] != 0
                /*SPR 16855 fix end*/
           ) 
        {
        if (node_p->reportBitMapPCell.exactCQIWrapRound ==
                /*SPR 16855 fix start*/
                (totalWrapNum % ulUEContext_p->pucchConfigInfo.maxCQIWrapAround[
                 longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex]) 
                /*SPR 16855 fix end*/
           )
        {
            SET_BIT_AT_POS(validReportString, CQI_REPORT);
        }
        }
        else if (
                /*SPR 16855 fix start*/
                ulUEContext_p->pucchConfigInfo.maxCQIWrapAround2[
                longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex] != 0
                /*SPR 16855 fix end*/
                ) 
        {
            if (node_p->reportBitMapPCell.exactCQIWrapRound2 ==
                    /*SPR 16855 fix start*/
                    (totalWrapNum % ulUEContext_p->pucchConfigInfo.maxCQIWrapAround2[
                     longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex]) 
                    /*SPR 16855 fix end*/
               )
            {
                SET_BIT_AT_POS(validReportString, CQI_REPORT);
            }
        }
    }  

    if (CHECK_BIT_SET(node_p->reportBitMapPCell.ueReportAndOpportunity, WIDEBAND_CQI_REPORT) )
    {

        if (
                /*SPR 16855 fix start*/
                ulUEContext_p->pucchConfigInfo.maxWbCQIWrapAround[
                longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex] != 0
                /*SPR 16855 fix end*/
           ) 
        {
        if (node_p->reportBitMapPCell.exactWbCQIWrapRound ==
                /*SPR 16855 fix start*/
                (totalWrapNum % ulUEContext_p->pucchConfigInfo.maxWbCQIWrapAround[
                 longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex]) 
                /*SPR 16855 fix end*/
           )
        {
            SET_BIT_AT_POS(validReportString, WIDEBAND_CQI_REPORT);
        }
        }
        else if (
                /*SPR 16855 fix start*/
                ulUEContext_p->pucchConfigInfo.maxWbCQIWrapAround2[
                longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex] != 0
                /*SPR 16855 fix end*/
                ) 
        {
            if (node_p->reportBitMapPCell.exactWbCQIWrapRound2 ==
                    /*SPR 16855 fix start*/
                    (totalWrapNum % ulUEContext_p->pucchConfigInfo.maxWbCQIWrapAround2[
                     longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex]) 
                    /*SPR 16855 fix end*/
               )
            {
                SET_BIT_AT_POS(validReportString, WIDEBAND_CQI_REPORT);
            }
        }
    }    

    if (CHECK_BIT_SET(node_p->reportBitMapPCell.ueReportAndOpportunity, RI_REPORT) )
    {

        if (
                /*SPR 16855 fix start*/
                ulUEContext_p->pucchConfigInfo.maxRIWrapAround[
                longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex] != 0
                /*SPR 16855 fix end*/
           ) 
        {
        if (node_p->reportBitMapPCell.exactRIWrapRound ==
                /*SPR 16855 fix start*/
                (totalWrapNum % ulUEContext_p->pucchConfigInfo.maxRIWrapAround[
                 longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex]) 
                /*SPR 16855 fix end*/
           )
        {
            SET_BIT_AT_POS(validReportString, RI_REPORT);
        }
        }
        else if (
                /*SPR 16855 fix start*/
                ulUEContext_p->pucchConfigInfo.maxRIWrapAround2[
                longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex] != 0
                /*SPR 16855 fix end*/
                ) 
        {
            if (node_p->reportBitMapPCell.exactRIWrapRound2 ==
                    /*SPR 16855 fix start*/
                    (totalWrapNum % ulUEContext_p->pucchConfigInfo.maxRIWrapAround2[
                     longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex]) 
                    /*SPR 16855 fix end*/
               )
            {
                SET_BIT_AT_POS(validReportString, RI_REPORT);
            }
        }
    }    
    return validReportString;
}
/* OPT */

#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : processLongSrOrReport 
 * Inputs         : subFrameOffset 
 *                  tti - Global TTI
 *					subFrameNum - SF
 *					sysFrameNum - SFN
 *					pdcchULOutputInfo_p - PDDCH UL Output
 *                  rbMapNum - rbMap number
 *                  containerTick - Container tick
 *                  puschReserveOnlyFlagForSingleUE - TRUE if only one ue 
 *                                                    FALSE  otherwise
 *                  cellIndex - cell index for primary or secondary cell.
 * Outputs        : NONE
 * Returns        : Pointer to node globalSrOpportunityAndPeriodicReports.
 * Description    : This function will provide node to operate on.
 *****************************************************************************/
STATIC void  processLongSrOrReport(
        UInt32 ulSubFrame,
        /* SPR 15909 fix start */
        tickType_t tti,
        /* SPR 15909 fix end */
        UInt32 subFrameNum,
        UInt32 sysFrameNum,
        /* +- SPR 17777 */
        UInt32 rbMapNum,
        UInt8 internalCellIndex
        )
{
    UInt16 subFrameOffset = 0;
    /* SPR 15909 fix start */
    tickType_t ulTti = 0;
    /* SPR 15909 fix end */
    UInt32 totalWrapNum = 0;
    UInt8 cellCount = 0;
    UInt16 ueIndex = 0;
    UInt16 selectedCellIndex = 0;
    UInt8 isCSIScheduled = TRUE;
    UInt8 isSCellScheduled = FALSE;
    /** SPR 12165 Fix : Start **/
    UInt8 ueReportOrOpportunityType = 0;
    UInt8 pCellUeReportOrOpportunityType = 0;
    UInt8 sCellUeReportOrOpportunityType = 0;
    /** SPR 12165 Fix : End **/
    ULUEContextInfo   *uLUEContextInfo_p = PNULL; 
    ULUEContext *ulUEContext_p = PNULL;
    SCellSchedulingQueueNode *sCellScheduledNode_p = PNULL; 
    globalSrOpportunityAndPeriodicReports *longSrOrReportNode_p = PNULL;
    DLUEContext     *ueDLContext_p = PNULL;
    UInt8 ulUEScheduledFlag = 0;
    UeScheduledInfo * ulUeScheduledInfo_p = PNULL;
    /* TDD Config 0 Changes start */
    UInt8 containerTick = (ulSubFrame % MAX_ULSCH_UCI_CONTAINER);
    /* TDD Config 0 Changes end  */
    ULSchUciRachPoolInfo* ulschUciRachInfo_p 
        =ulSchUciRachContainer_gp[internalCellIndex]  + containerTick ;  
    UInt16 sCellQCount = 0;
    UInt16 numberOfInfoPresent = 0;
    /**** Coverity Fix 63673: Start ****/
    UeCellInfo ueCellInfo = {0};
    /**** Coverity Fix 63673: End ****/
    UInt32 ulUeScheduledInfoArrayIndex = 0;

    ulTti = sysFrameNum * MAX_SUBFRAME + subFrameNum;
    totalWrapNum = ulTti/MAX_PERIODICITY_SUBFRAME;
    /* MEAS_GAP_CHG */
    /* MEAS_GAP_CHG */
    UInt32 ulSysFrameNum    = sysFrameNum;
    /* SPR 15909 fix start */
    tickType_t globalTick = 0;
    /* SPR 15909 fix end */
    /* ICIC changes start */
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;

    if(rbMapNode_p->advanceAllocFlag == FALSE)
    {
        *rbMapNode_p   = (ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
        /* TDD Config 0 Changes Start */
    }

    if (ulSubFrame < subFrameNum)
    {
        ulSysFrameNum++;
        if (MAX_SYS_FRAME_NUM < ulSysFrameNum)
        {
            ulSysFrameNum = 0;
        }
        globalTick = tti + (MAX_SUB_FRAME - subFrameNum ) + ulSubFrame;
    }
    else
    {
        globalTick = (ulSubFrame-subFrameNum) + tti;
    }

    ulTti = ulSysFrameNum*MAX_SUBFRAME + ulSubFrame;
    subFrameOffset = ulTti % MAX_SUBFRAME_OFFSET;
    totalWrapNum = ulTti / MAX_SUBFRAME_OFFSET;
    
    /*SPR 16855 fix start*/
    UInt16 activeIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex;
    /*SPR 16855 fix end*/

    /*SPR 16855 fix start*/
    longSrOrReportNode_p =(globalSrOpportunityAndPeriodicReports *) getListNode(
            &(pucchSrAndReportsDb_g[internalCellIndex][activeIndex][subFrameOffset].ueList), 0);
    /*SPR 16855 fix end*/
    /* TDD Config 0 Changes start */
    ulUeScheduledInfoArrayIndex = ulSubFrame ;
    /* TDD Config 0 Changes End */
    /* Algo :
       1. Get long node.
       2. Validate the node & check of report type expected.
       2. Check if scell is active for selected node.if yes get report type.
       3. Compare pcell or scell reports according to priority & drop CSI Report.
       e.g. SR > RI > WB_CQI > SB_CQI report of any cell,if same report on 
       both cells PCell_REPORT > SCell_REPORT & as soon as node
       found break as priority is to be given lowest serving 
       SCell Index.
       4. if no SCell has report process node for PCell only.
       */
    while (PNULL != longSrOrReportNode_p)
    {
        /**** Coverity Fix 62962 : Start ****/
        ueCellInfo.isPCell = TRUE;
        isCSIScheduled = FALSE;
        /**** Coverity Fix 62962 : End ****/
        isSCellScheduled = FALSE;
        /* + SPR 11795*/
        selectedCellIndex = internalCellIndex;
        ueReportOrOpportunityType = pCellUeReportOrOpportunityType = sCellUeReportOrOpportunityType = 0;
        /* - SPR 11795*/
        ueIndex = longSrOrReportNode_p->ueIndex;
        /* CLPC CHANGE */
        //RESET_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex, subFrameNum);
        /* CLPC CHANGE */

        uLUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
        ulUEContext_p = uLUEContextInfo_p->ulUEContext_p;
        ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

        /* Validate PUCCH node for measgap & empty checks*/
        if (MAC_FAILURE == validateAndUpdatePucchSrAndReportsNode(
                    uLUEContextInfo_p, ueIndex, globalTick) )
        {
            longSrOrReportNode_p = (globalSrOpportunityAndPeriodicReports *)
                getNextListNode((LTE_LIST_NODE *)(longSrOrReportNode_p));
            continue;
        }

        /*Cyclomatic Complexity changes - ends here*/
        /** SPR 14204 Fix : Start **/
        ulUeScheduledInfo_p = uLUEContextInfo_p->ulUEContext_p->
            ulUeScheduledInfoPointerArray[internalCellIndex][ulUeScheduledInfoArrayIndex];
        /** SPR 14204 Fix : End **/
        if (PNULL == ulUeScheduledInfo_p)
        {
            lteWarning("Couldn't get ulUEScheduled info for UE Index[%d], \
                    current SF/SFN: [%d]/[%d], UL SFN/SF [%d]/[%d]", 
                    ueIndex, sysFrameNum, subFrameNum, ulSysFrameNum, ulSubFrame);
            longSrOrReportNode_p =(globalSrOpportunityAndPeriodicReports *)getNextListNode((LTE_LIST_NODE *)longSrOrReportNode_p);
            continue;
        }
        else
        {
            ulUEScheduledFlag = ulUeScheduledInfo_p->ulCCEIndex;
        }

        /* Calculate which type of report type expected */
        pCellUeReportOrOpportunityType = getValidReportTypesForWrapArounds(longSrOrReportNode_p, 
                ulUEContext_p, totalWrapNum, internalCellIndex);
        
        if (CHECK_BIT_SET(pCellUeReportOrOpportunityType,CQI_REPORT) || \
                CHECK_BIT_SET(pCellUeReportOrOpportunityType,WIDEBAND_CQI_REPORT) ||
                CHECK_BIT_SET(pCellUeReportOrOpportunityType,RI_REPORT))
        {
            isCSIScheduled = TRUE;
        }

        for (cellCount = START_SCELL_INDEX; cellCount <= MAX_NUM_SCELL; cellCount++)
        {
            /* Proceed only for Scell which are active */
            /*SPR 20192 Changes Start*/
            if (ulUEContext_p->ulUeScellContext_p[cellCount]
                    && (ueDLContext_p) && ueDLContext_p->dlUeScellContext_p[cellCount]
            /*SPR 20192 Changes End*/
                        /* + SPR_14768 */
                    /* &&  IS_SCELL_ACTIVE_AT_TTI(ueIndex, cellCount)*/)
                        /* - SPR_14768 */
            {
                /* Check if ue is scheduled in SCell */
                if ((ueDLContext_p->dlUeScellContext_p[cellCount]->pucchReqInfo.schedulingStatus > 0) &&
                        (ueDLContext_p->dlUeScellContext_p[cellCount]->pucchReqInfo.schedulingStatus < 3))
                {
                    isSCellScheduled = TRUE;
                }
                /* Calculate which type of report type expected */
                sCellUeReportOrOpportunityType = getValidReportTypesForWrapAroundsForSCell(longSrOrReportNode_p, 
                        ulUEContext_p, totalWrapNum, cellCount,internalCellIndex);
                if (!sCellUeReportOrOpportunityType)
                {
                    continue;
                }
                /* Priority is to be given to RI_REPORT > WIDEBAND_CQI > SUBABND_CQI for 
                   any cell & then if same report on both cells PCell Reports is given priority 
                   over SCell reports */
                /**** Coverity Fix 62962 : Start ****/
                /** SPR 12165 Fix : Start **/
                if (CHECK_BIT_SET(pCellUeReportOrOpportunityType,SCHEDULING_REQUEST))
                    /** SPR 12165 Fix : End **/
                {
                    ueCellInfo.isPCell = FALSE;
                    selectedCellIndex = cellCount;
                    ueCellInfo.sCellIndex = cellCount;
                    ueCellInfo.sCellInternalCellIndex = ulUEContext_p->ulUeScellContext_p[cellCount]\
                                                        ->internalCellIndex;
                    ueReportOrOpportunityType = sCellUeReportOrOpportunityType;
                    /** SPR 12165 Fix : Start **/
                    if (isCSIScheduled)
                    {
                        UNSET_BIT_AT_POS(pCellUeReportOrOpportunityType,SCHEDULING_REQUEST);
                        if (pCellUeReportOrOpportunityType >= sCellUeReportOrOpportunityType)
                        {
                            ueCellInfo.isPCell = TRUE;
                            selectedCellIndex = internalCellIndex;
                            ueReportOrOpportunityType = pCellUeReportOrOpportunityType;
                            if (INVALID_CCE_INDEX == ulUEScheduledFlag)
                            {
                                dropCQIReport(ulUEContext_p,ueDLContext_p,!ueCellInfo.isPCell,\
                                        &sCellUeReportOrOpportunityType,cellCount);
                            }
                        }
                        else
                        {
                            if (INVALID_CCE_INDEX == ulUEScheduledFlag)
                            {
                                dropCQIReport(ulUEContext_p,ueDLContext_p,!ueCellInfo.isPCell,\
                                        &pCellUeReportOrOpportunityType,internalCellIndex);
                            }
                        }    
                    }
                    isCSIScheduled = TRUE;
                    SET_BIT_AT_POS(ueReportOrOpportunityType,SCHEDULING_REQUEST);
                    break;
                }
                if (pCellUeReportOrOpportunityType >= sCellUeReportOrOpportunityType)
                {
                    ueCellInfo.isPCell = TRUE;
                    selectedCellIndex = internalCellIndex;
                    ueReportOrOpportunityType = pCellUeReportOrOpportunityType;
                    isCSIScheduled = TRUE;
                    if (INVALID_CCE_INDEX == ulUEScheduledFlag)
                    {
                        dropCQIReport(ulUEContext_p,ueDLContext_p,!ueCellInfo.isPCell,\
                                &sCellUeReportOrOpportunityType,cellCount);
                    }
                    break;
                }
                else 
                {
                    isCSIScheduled = TRUE;
                    ueCellInfo.isPCell = FALSE;
                    selectedCellIndex = cellCount;
                    ueCellInfo.sCellIndex = cellCount;
                    ueCellInfo.sCellInternalCellIndex = ulUEContext_p->ulUeScellContext_p[cellCount]\
                                                        ->internalCellIndex;
                    ueReportOrOpportunityType = sCellUeReportOrOpportunityType;
                    if (INVALID_CCE_INDEX == ulUEScheduledFlag)
                    {
                        dropCQIReport(ulUEContext_p,ueDLContext_p,!ueCellInfo.isPCell,\
                                &pCellUeReportOrOpportunityType,internalCellIndex);
                    }
                    break;
                }
            }//end of active SCell if
        }//end of active SCell for Loop

        /* if only primary cell CSI or SR report needs to be sent*/
        if (pCellUeReportOrOpportunityType && !sCellUeReportOrOpportunityType)
        {
            ueCellInfo.isPCell = TRUE;
            selectedCellIndex = internalCellIndex;
            ueReportOrOpportunityType = pCellUeReportOrOpportunityType;
        }
        else if(!pCellUeReportOrOpportunityType && !sCellUeReportOrOpportunityType)
        {

            longSrOrReportNode_p = (globalSrOpportunityAndPeriodicReports *)
                getNextListNode((LTE_LIST_NODE *)longSrOrReportNode_p);
            continue;
        }

        /*Cyclomatic Complexity changes - starts here*/
        updateBwPartOnWideBandSubBandReport(ulUEContext_p,
                ulTti,sysFrameNum,subFrameNum,&ueCellInfo,internalCellIndex);
          /* OPT */

        /* Review Comment fix GK14 start */
        /*Cyclomatic Complexity changes - starts here*/

        numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
        /*Cyclomatic Complexity changes - ends here*/
        /*** Coverity Fix 63551 : Start ***/
        ulschUciRachInfo_p->ulschUciInfo[ulschUciRachInfo_p->numberOfInfoPresent].\
                rnti = uLUEContextInfo_p->ulUEContext_p->crnti;
        /* Algo :
         *   1. if scheduling on SCell drop CSI reports of both cells if simultaneousAckNackAndCQI  
         *      is true & in case of false also need to drop so only UCI_HARQ_PDU will go.
         *   2. if only scheduled in PCell & CSI is there then if simultaneousAckNackAndCQI 
         *      is true then multiplexed pdu will go.
         */ 
        /** SPR 11207 Fix : Start ***/
        if(isSCellScheduled)
        {
            /* UCI HARQ of secondary cell*/
            sCellQCount = SCELL_SCHEDULING_QUEUE_COUNT(internalCellIndex);
            while (sCellQCount--)
            {
                DEQUEUE_SCELL_SCHEDULING_NODE(sCellScheduledNode_p, internalCellIndex);

                /*** Coverity Fix 63550 : Start ***/
                if ((sCellScheduledNode_p) && \
                        (sCellScheduledNode_p->ueIndex == ulUEContext_p->ueIndex))
                    /*** Coverity Fix 63550 : Start ***/
                {
                    /** SPR 12165 Fix : Start **/
                    if (CHECK_BIT_SET(ueReportOrOpportunityType,SCHEDULING_REQUEST) ||
                            (INVALID_CCE_INDEX != ulUEScheduledFlag) )
                    {
                        if ((isCSIScheduled) && (INVALID_CCE_INDEX == ulUEScheduledFlag))
                        {
                            dropCQIReport(ulUEContext_p, ueDLContext_p, ueCellInfo.isPCell,\
                                    &ueReportOrOpportunityType,selectedCellIndex);
                        }
                            /*check  parameter simultaneousAckNackAndCQI is TRUE*/
                            /** For FDD Only remove after some time**/
                        processPUCCHSrOrReportAndAckNack(&ueReportOrOpportunityType,
                           uLUEContextInfo_p->ulUEContext_p,ulUeScheduledInfo_p,
                           /* +- SPR 17777 */
                           ulschUciRachInfo_p,ulSubFrame,
#ifdef TDD_CONFIG
                           ulSysFrameNum,
#endif
                           /* +- SPR 17777 */
                           /* +- SPR 17777 */
                           rbMapNum,
#ifdef FDD_CONFIG
                           ulUEScheduledFlag,
#endif
                           &ueCellInfo,
                           /* +- SPR 17777 */
                           /* Rel9_upgrade_CR410 */
                           globalTick,internalCellIndex);
                    }
                    else
                    {

                        if (isCSIScheduled)
                        {
                            dropCQIReport(ulUEContext_p, ueDLContext_p, ueCellInfo.isPCell,\
                                    &ueReportOrOpportunityType,selectedCellIndex);
                        }
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_UCI_HARQ;
                        /** SPR 12457 Fix : Start**/
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
                        allocateResourceForTddHarq(ulSubFrame,
                                ulUEContext_p->ueIndex, 
                                rbMapNum,
                                ulschUciRachInfo_p,
                                ulUeScheduledInfo_p,
                                internalCellIndex);
#endif                        
                    }

                    FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                    /* + SPR 11471 Changes */
                    /** SPR 12457 Fix : Start**/
                    ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus  = 0x0;
                    /** SPR 12457 Fix : End **/
                    /* - SPR 11471 Changes */
                    /* SPR 14760 */
                    SET_HARQ_BLER_RECEIVED_ON_PUCCH( ueDLContext_p->ueIndex,ulSubFrame)
                    /* SPR 14760 */
                    break;                    
                }
                /*** Coverity Fix 63536 : Start ***/
                if (!ENQUEUE_SCELL_SCHEDULING_NODE(sCellScheduledNode_p, internalCellIndex))
                {
                    FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                    continue;
                }
                /*** Coverity Fix 63536 : End ***/
            }
            if (MAC_SS_PDU_INVALID != ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType)
            {
                longSrOrReportNode_p = (globalSrOpportunityAndPeriodicReports *)
                getNextListNode((LTE_LIST_NODE *)longSrOrReportNode_p);
                continue;    
            }
        }
        else
        {
            if (ulUeScheduledInfo_p->ueInsertedInPdcchDlUeInfo > 0 )  
            {
                /*check  parameter simultaneousAckNackAndCQI is TRUE*/
                processPUCCHSrOrReportAndAckNack(&ueReportOrOpportunityType,
                        uLUEContextInfo_p->ulUEContext_p, ulUeScheduledInfo_p,ulschUciRachInfo_p,
                        /* +- SPR 17777 */
                        ulSubFrame,
#ifdef TDD_CONFIG
                        ulSysFrameNum,
#endif
                        rbMapNum,
#ifdef FDD_CONFIG
                        ulUEScheduledFlag,
#endif
                        &ueCellInfo ,globalTick, internalCellIndex);
                /* +- SPR 17777 */

                if (MAC_SS_PDU_INVALID != ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType)
                {
                    longSrOrReportNode_p = (globalSrOpportunityAndPeriodicReports *)
                        getNextListNode((LTE_LIST_NODE *)longSrOrReportNode_p);
                    continue;
                }
                /* Rel9_upgrade_CR410 */
            }
        }
        /** SPR 11207 Fix : End ***/

        /*
         * Otherwise simply SR or report needs to be sent
         */

        processPUCCHSrOrReportOnly(ueReportOrOpportunityType,
                uLUEContextInfo_p->ulUEContext_p,ulUeScheduledInfo_p, 
                /* +- SPR 17777 */
                ulSysFrameNum,
                ulschUciRachInfo_p, 
                /* + SPR_14099 */
                ulSubFrame,
                /* +- SPR 17777 */
                /* - SPR_14099 */
                rbMapNum,
                /* Rel9_upgrade_CR410 */
                tti,
                &ueCellInfo,
                internalCellIndex 
                );

        longSrOrReportNode_p = (globalSrOpportunityAndPeriodicReports *)
            getNextListNode((LTE_LIST_NODE *)longSrOrReportNode_p);
    }// end of PCell while
}
/*****************************************************************************
 * Function Name  : processPUCCHAndPRACHReports
 * Inputs         : pdcchULOutputInfo_p - Its point to PdcchULOutputInfo structure.
 *                  rbMapNum,
 *                  tti - Its represent the current Tick.
 *                  containerTick,ulSubFrame
 *                  internalCellIndex - Cell-Index of Pcell or Scell
 * Outputs        :
 * Returns        :
 * Description    : This function process the PUCCH Report corresponding to the
 *                  subFrame and allocates resources if UL grant is not given for the
 *                  UE which is being processed.
 *                  IF the UE has a UL grant and only SR was scheduled to be sent,
 *                  fills up ueCollisionArray to inform PUSCH module
 *                  It then processes resource allocation for all UEs whose ack/nack
 *                  is to be sent, but dont have UL grant
 *****************************************************************************/
void processPUCCHAndPRACHReports( PdcchULOutputInfo *pdcchULOutputInfo_p,
        UInt32 rbMapNum,
        /*SPR 15909 fix start*/
        tickType_t tti,
        /*SPR 15909 fix end*/
         /* TDD Config 0 Changes Start */
        /*UInt8 containerTick,*/
         /* TDD Config 0 Changes End */
        UInt32 ulSubFrame,
        InternalCellIndex internalCellIndex
        )
{
    LTE_MAC_UT_LOG( LOG_INFO,PUCCH_MODULE,"[%s] rbMapNum =%d, tti =%d Entry. ",
            __func__,rbMapNum, tti);
    UInt32  ueIndex               = 0;
    ULUEContextInfo   *uLUEContextInfo_p  = PNULL;
    /* MEAS_GAP_CHG */
    ULUEContext       *ulUEContext_p = PNULL;   
    /* MEAS_GAP_CHG */

    /* TDD Config 0 Changes start */
    UInt8 containerTick = (ulSubFrame % MAX_ULSCH_UCI_CONTAINER);
    /* TDD Config 0 Changes end  */
    globalSrOpportunityAndPeriodicReports *node_p = PNULL;
    UeScheduledInfo * ulUeScheduledInfo_p = PNULL;
    PdcchDLUEInfo   * pdcchDLUEInfo_p = PNULL;
    UInt32 ulUeScheduledInfoArrayIndex = 0;
    UInt32 pdcchDlUECount = 0;
    UInt32 pdcchDlUeIndex = 0;
    UInt32 subFrameNum    = tti % MAX_SUB_FRAME;
    UInt32 sysFrameNum    = (MODULO_ONEZEROTWOFOUR(tti/MAX_SUB_FRAME));
    UInt32 numPhysicalResources  = 0;
    UInt32 numPrachFreqs = 0;
    UInt32 prachFrequencyOffset = 0;
    /* SPR 1540 changes start */
    UInt8 prachConfigIndex = 0;
    UInt32 prachActiveIndex = 0; 
    /* SPR 1540 changes end */
    RACHConfigInfo *rachConfigInfo_p = 
        &(cellSpecificParams_g.cellConfigAndInitParams_p[
                internalCellIndex]->cellParams_p->rachConfigInfo);
    UInt32 ulSysFrameNum    = sysFrameNum;
    /* SPR 15909 fix start */
    tickType_t ulTti = 0;
    /* SPR 15909 fix end */
    ULSchUciRachPoolInfo* ulschUciRachInfo_p =ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
    /*SPR 15909 fix start*/
    tickType_t globalTick = 0;
    /*SPR 15909 fix end*/
    /* Rel9_upgrade_CR257 */
    globalSrShortOpportunity *nodeShort_p = PNULL;
    UInt8 shortSubframeOffset = 0;
    UInt16 numInfo = 0;
    UInt8 pduType = 0;
    /* Rel9_upgrade_CR257 */

    /* ICIC changes start */
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    UInt8 initAvailCCRes = 0;
    UInt8 initAvailCERes = 0;
    UInt8 allocCERBs = 0;
    UInt8 allocCCRBs = 0;
    /* ICIC changes end */
    /* +- SPR 17777 */
    UInt8 cellCount = 0;
    DLUEContext     *ueDLContext_p = PNULL;
    SCellSchedulingQueueNode *sCellScheduledNode_p = PNULL; 
    UInt16 sCellQCount = 0;
    UInt8 isSCellScheduled = LTE_FALSE;

    if(rbMapNode_p->advanceAllocFlag == FALSE)
    {
        *rbMapNode_p   = (ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
        /* TDD Config 0 Changes Start */
    }
    initAvailCCRes = rbMapNode_p->availableResourceInfo[CC_CE_REGION].initAvailRes;
    initAvailCERes = rbMapNode_p->availableResourceInfo[CE_REGION].initAvailRes;
    /* TDD Config 0 Changes End */

    if (ulSubFrame < subFrameNum)
    {
        ulSysFrameNum++;
        if (MAX_SYS_FRAME_NUM < ulSysFrameNum)
        {
            ulSysFrameNum = 0;
        }
        globalTick = tti + (MAX_SUB_FRAME - subFrameNum ) + ulSubFrame;
    }
    else
    {
        globalTick = (ulSubFrame-subFrameNum) + tti;
    }

    ulTti = ulSysFrameNum*MAX_SUBFRAME + ulSubFrame;
    /* Rel9_upgrade_CR257 */
    shortSubframeOffset = ulTti % SHORT_SR_PERIODICITY;
    /* Rel9_upgrade_CR257 */
    
    ulUeScheduledInfoArrayIndex = ulSubFrame;

    /* +- SPR 17777 */
    processLongSrOrReport(ulSubFrame, tti, subFrameNum,
                          sysFrameNum,
                          /* +- SPR 17777 */
                          rbMapNum,internalCellIndex); 

    /*SPR 16855 fix start*/
    nodeShort_p =(globalSrShortOpportunity *) getListNode(
            &(pucchSrShortReportDb_g[internalCellIndex][shortPeriodicSRMapShortInfo_gp[internalCellIndex]->activeIndex][shortSubframeOffset].ueList), 0);
    /*SPR 16855 fix end*/

    /*
     * We need to reserve resources (SR) for each UL
     * The ulUEScheduledInfoPointerArray uses the same SF where DCI_0 
     * is sent for corresponding UL in the DL
     * So ulSubFrame needs to be calculated only for finding the correct list of UEs
     * to be processed for SR w/ or w/o ACKNACK
     */
    /* TDD Config 0 start */
    /* Assigned above, no need to do again as
     * this value is not changed
     */
    //    ulUeScheduledInfoArrayIndex = subFrameNum;
    /* TDD Config 0 end */


    /*
     * Start processing UE's with SR or Reports to be sent
     */
    while (PNULL != nodeShort_p)
    {
        ueIndex = nodeShort_p->ueIndex;
        uLUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];

        /*Cyclomatic Complexity changes - starts here*/
        if(MAC_FAILURE == validateAndUpdatePucchSrAndReportsNode(uLUEContextInfo_p, 
							ueIndex, globalTick) )
        {
            node_p = (globalSrOpportunityAndPeriodicReports *)
                getNextListNode((LTE_LIST_NODE *)(node_p));
            continue;
        }
        ulUEContext_p = uLUEContextInfo_p->ulUEContext_p;
        /*Cyclomatic Complexity changes - ends here*/

        /*
         * Algo
         * 1) In UEContext index into
         *    ulUeScehduledInfoPointerArray correctly to obtain ulUeScheduledInfo.
         * 2) get ulcce value from ulUeScheduledInfo
         * 3) check if ulcce is valid => has to be processed by PUSCH
         * 4) if ulcce is invalid => has to be processed in PUCCH
         * 5) check value of ueInsertedInPdcchDlUeInfo > 0 => dl data had been 
         *    scheduled for this UE and ack/nack should be expected as well
         *
         */
        /** SPR 14204 Fix : Start **/
        ulUeScheduledInfo_p = uLUEContextInfo_p->ulUEContext_p->
            ulUeScheduledInfoPointerArray[internalCellIndex][ulUeScheduledInfoArrayIndex];
        /** SPR 14204 Fix : End **/
        if (PNULL == ulUeScheduledInfo_p)
        {
            /*
             * Log error and raise panic
             */
            lteWarning("Couldn't get ulUEScheduled info for UE Index[%d], \
                    current SF/SFN: [%d]/[%d], UL SFN/SF [%d]/[%d]", 
                    ueIndex, sysFrameNum, subFrameNum, ulSysFrameNum, ulSubFrame);
            nodeShort_p =(globalSrShortOpportunity *)getNextListNode((LTE_LIST_NODE *)nodeShort_p);
            continue;
        }
        numInfo = ulUeScheduledInfo_p->ulschUciInfoIndex;

        if (ULSCH_UCI_INVALID_INDEX != numInfo)
        {
            pduType = ulschUciRachInfo_p->ulschUciInfo[numInfo].pduType;

            nodeShort_p = (globalSrShortOpportunity *)
                getNextListNode((LTE_LIST_NODE *)nodeShort_p);
            /*Cyclomatic Complexity changes - starts here*/  
            if(MAC_FAILURE == validateAndUpdatePduTypeForSrShortOpportunity(
                        pduType, ulschUciRachInfo_p, numInfo))
            {
                continue;
            }
            /*Cyclomatic Complexity changes - ends here*/  

            processPUCCHShortSr(
                    uLUEContextInfo_p->ulUEContext_p, 
                    ulschUciRachInfo_p, 
                    rbMapNum,
                    internalCellIndex);
        }
        else
        {
            ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
            isSCellScheduled = LTE_FALSE;

            for (cellCount = START_SCELL_INDEX; (cellCount <= MAX_NUM_SCELL) && !(isSCellScheduled); cellCount++)
            {
                /* Proceed only for Scell which are active */
                if (uLUEContextInfo_p->ulUEContext_p->ulUeScellContext_p[cellCount]  
                        /* + SPR_14768 */
                    /* &&   IS_SCELL_ACTIVE_AT_TTI(ueIndex, cellCount)*/)
                        /* - SPR_14768 */
                {
                    /* Check if ue is scheduled in SCell */
                    /** SPR 12457 Fix : Start**/
                    if (ueDLContext_p && ueDLContext_p->dlUeScellContext_p[cellCount] && \
                            (ueDLContext_p->dlUeScellContext_p[cellCount]->pucchReqInfo.schedulingStatus > 0) &&
                            (ueDLContext_p->dlUeScellContext_p[cellCount]->pucchReqInfo.schedulingStatus < 3))
                        /** SPR 12457 Fix : End **/
                    {
                        isSCellScheduled = LTE_TRUE;
                        /* UCI HARQ of secondary cell*/
                        sCellQCount = SCELL_SCHEDULING_QUEUE_COUNT(internalCellIndex);
                        while (sCellQCount--)
                        {
                            DEQUEUE_SCELL_SCHEDULING_NODE(sCellScheduledNode_p, internalCellIndex);

                            /*** Coverity Fix 63550 : Start ***/
                            if ((sCellScheduledNode_p) && \
                                    (sCellScheduledNode_p->ueIndex == uLUEContextInfo_p->ulUEContext_p->ueIndex))
                            {
                                FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                                ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus  = 0x0;
                                break;
                            }
                            /*** Coverity Fix 63550 : End ***/

                            /*** Coverity Fix 63536 : Start ***/
                            if (!ENQUEUE_SCELL_SCHEDULING_NODE(sCellScheduledNode_p, internalCellIndex))
                            {
                                FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                            }
                            /*** Coverity Fix 63536 : End ***/
                        }
                    }
                }
            }

            /* Check if ACK/NACK along with SR report has to be sent */
            if ((0 < ulUeScheduledInfo_p->ueInsertedInPdcchDlUeInfo)
                    || isSCellScheduled
               )
            {
                processPUCCHShortSrAndAckNack(
                        uLUEContextInfo_p->ulUEContext_p,
                        ulUeScheduledInfo_p,
                        ulschUciRachInfo_p,
                        ulSubFrame,
                        rbMapNum,
                        internalCellIndex);
                nodeShort_p = (globalSrShortOpportunity *)
                    getNextListNode((LTE_LIST_NODE *)nodeShort_p);
                continue;
            }
            /* Otherwise simply SR or report needs to be sent */
            processPUCCHShortSrOnly(
                    uLUEContextInfo_p->ulUEContext_p, 
                    ulUeScheduledInfo_p, 
                    ulschUciRachInfo_p, 
                    rbMapNum,
                    internalCellIndex);

            nodeShort_p = (globalSrShortOpportunity *)
                getNextListNode((LTE_LIST_NODE *)nodeShort_p);
        }
    }
    /* Rel9_upgrade_CR257 */
    /*
     * Start processing UE's which had no SR or Report to be sent
     * but may have ack/nack arriving
     */
    /* SPR 1101 changes start*/
    /* SPR 4301 chg */
    if (pdcchULOutputInfo_p->pdcchTcrntiUEInfo.cumulativeUeCountForTempUlGroup)
    {
        buildUciHarqForTempUE(&pdcchULOutputInfo_p->pdcchTcrntiUEInfo, 
                pdcchULOutputInfo_p->pdcchTcrntiUEInfo.cumulativeUeCountForTempUlGroup,   
                rbMapNum, ulSubFrame, ulschUciRachInfo_p, internalCellIndex);
        pdcchULOutputInfo_p->pdcchTcrntiUEInfo.cumulativeUeCountForTempUlGroup = 0;
    }
    /* SPR 4301 chg */
    /* SPR 1101 changes end*/

    pdcchDLUEInfo_p = &(pdcchULOutputInfo_p->pdcchDLUEInfo);
    uLUEContextInfo_p = PNULL;
    for (pdcchDlUECount = 0; pdcchDlUECount < pdcchDLUEInfo_p->cumulativeUeCountForDlGroup;
            pdcchDlUECount++)
    {
        LTE_MAC_UT_LOG(LOG_MINOR,PUCCH_MODULE,"pucch Working for ack/nacks only");
        pdcchDlUeIndex = pdcchDLUEInfo_p->pdcchDLUEArrayForDlGroup[pdcchDlUECount];
        uLUEContextInfo_p = &ulUECtxInfoArr_g[pdcchDlUeIndex];
        ueDLContext_p = dlUECtxInfoArr_g[pdcchDlUeIndex].dlUEContext_p;

        /*coverity-530-55306*/
        /*covde removed */
        /*coverity-530-55306*/
        if (uLUEContextInfo_p->pendingDeleteFlag)
        {
            lteWarning("Pending Delete Flag is set For this UE Index [%d], "
                    "not processing ack nacks for this UE", pdcchDlUeIndex);
            continue;
        }

        /* MEAS_GAP_CHG */
        ulUEContext_p = uLUEContextInfo_p->ulUEContext_p;
        /*coverity-530-55306*/
        if (PNULL == ulUEContext_p)
        {
            lteWarning("No UL UE context created for UE Index [%d]", pdcchDlUeIndex);
            continue;
        }
        /*coverity-530-55306*/
        if (SETUP == ulUEContext_p->measGapConfigType)
        {
            if (TRUE == isTTIInMeasGapPeriod(ulUEContext_p, globalTick))
            {
                continue;
            }
        }
        /* MEAS_GAP_CHG */

        /** SPR 14150_14151 Fix : Start **/
        if (IS_CA_ENABLED() && !IS_PCELL_OF_UE(ueDLContext_p,internalCellIndex))
        {
            continue;
        } 
        /** SPR 14150_14151 Fix : End **/

        /** SPR 14204 Fix : Start **/
        ulUeScheduledInfo_p = uLUEContextInfo_p->ulUEContext_p->
            ulUeScheduledInfoPointerArray[internalCellIndex][ulUeScheduledInfoArrayIndex];
        /** SPR 14204 Fix : End **/
        if (PNULL == ulUeScheduledInfo_p)
        {
            /*
             * Log error and raise panic
             */
            lteWarning("Couldn't get ulUEScheduled info for UE Index[%d], current SF/SFN: [%d]/[%d]", 
                    pdcchDlUeIndex, sysFrameNum, subFrameNum);
            continue;
        }
        if (!ulUeScheduledInfo_p->ueInsertedInPdcchDlUeInfo)
        {
            LTE_MAC_UT_LOG(LOG_MINOR,PUCCH_MODULE,"Ack nacks for ue[%d] already taken care of in"
                    " report+ackNack handling for ul sfn/sf: [%d]/[%d]\n", ulSysFrameNum, ulSubFrame);
            continue;
        }

        processPUCCHAckNacksOnly(ulSubFrame, pdcchDlUeIndex, rbMapNum, 
                ulschUciRachInfo_p, 
                ulUeScheduledInfo_p,
                /* +- SPR 17777 */
                internalCellIndex);
        
        /** SPR 14150_14151 Fix : Start **/
        for (cellCount = START_SCELL_INDEX; cellCount <= MAX_NUM_SCELL; cellCount++)
        {
            if ((ueDLContext_p) && (ueDLContext_p->dlUeScellContext_p[cellCount]) &&
                (ueDLContext_p->dlUeScellContext_p[cellCount]->pucchReqInfo.schedulingStatus > 0x00) && 
                (ueDLContext_p->dlUeScellContext_p[cellCount]->pucchReqInfo.schedulingStatus < 0x03))
            { 
                ueDLContext_p->dlUeScellContext_p[cellCount]->pucchReqInfo.schedulingStatus  = 0x00;
            }
        }
        /** SPR 14150_14151 Fix : End **/
    }

    if (IS_CA_ENABLED())
    {
        /** SPR 12457 Fix : Start**/
        /** SPR 14191 Fix : Start **/
        /* +- SPR 17777 */
        processDlSecondaryCellQueue(ulschUciRachInfo_p, ulSubFrame, \
                /* +- SPR 17777 */
                rbMapNum, globalTick,
                internalCellIndex);
        /** SPR 14191 Fix : End **/

        /** SPR 12457 Fix : End **/
    }
    /*Always reset the count*/
    pdcchDLUEInfo_p->cumulativeUeCountForDlGroup = 0;

    /* PUCCH Resources is allocated and set PUCCH Flag True*/

    /*Fix for SPR 860 */
    /*SPR 2446 Fix Begins*/
    setFreeRbsAndPUCCHFlag(rbMapNum,
            internalCellIndex);
    /*SPR 2446 Fix Ends*/

    /* TDD Config 0 Changes Start */
    allocCERBs = initAvailCERes -
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter;
    allocCCRBs = initAvailCCRes  -
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter;
    if( rbMapNode_p->ulAvailRB.cellEdgeRBs > allocCERBs )
    {
        rbMapNode_p->ulAvailRB.cellEdgeRBs -= allocCERBs;
    }
    else
    {
        rbMapNode_p->ulAvailRB.cellEdgeRBs = 0;
    }
    if( rbMapNode_p->ulAvailRB.totalRBs > allocCCRBs )
    {
        rbMapNode_p->ulAvailRB.totalRBs -= allocCCRBs;
    }
    else
    {
        rbMapNode_p->ulAvailRB.totalRBs = 0;
    }
    /* TDD Config 0 Changes End */
    do
    {
        prachActiveIndex = rachConfigInfo_p->prachActiveIndex;
        prachConfigIndex = rachConfigInfo_p->prachConfiguration[
            prachActiveIndex].prachConfigIndex;

        /* Get the Preamble format from the PRACH Config Index */
        /* +- SPR 17777 */
#ifdef FDD_CONIFG
        PreambleFormat preambleFormat = preambleFormatFrmPrachConfigIndex(
                internalCellIndex );
#else
        PreambleFormat preambleFormat = preambleFormatFrmPrachConfigIndex(
                prachConfigIndex);
#endif
        /* +- SPR 17777 */

        if( PREAMBLE_FORMAT_4 == preambleFormat ||
                INVALID_PREAMBLE_FORMAT == preambleFormat )
        {
            LOG_MAC_MSG( MAC_NO_ALLOC_PRACH_RES, LOGDEBUG ,MAC_PUCCH,
                    getCurrentTick(), __LINE__, prachConfigIndex,
                    preambleFormat, ulSysFrameNum, ulSubFrame,
                    0, 0, __func__, "");
            break;
        }

        /* Check for PRACH processing. Pass this the correct U SF */
        if ( MAC_FAILURE == isValidFrameForPRACH( ulSubFrame, ulSysFrameNum,
                    &prachFrequencyOffset, &numPhysicalResources,
                    internalCellIndex ) )
        {
            LOG_MAC_MSG( MAC_NO_ALLOC_PRACH_RES, LOGDEBUG ,MAC_PUCCH,
                    getCurrentTick(), __LINE__, prachConfigIndex,
                    preambleFormat, ulSysFrameNum, ulSubFrame,
                    0, 0, __func__, "");
            break;
        }

        /* For Preamble format 0, UL RBs for 1 SF needs to be reserved.
           For Preamble format 1 and 2, UL RBs for 2 SFs needs to be
           reserved. For Preamble format 3, UL RBs for 3 SFs needs to be
           reserved, as derived from 36.211, Section 5.7 */
        /* Allocate the resources for first SF */
        if(!(isPRACHResourcesAllocated( rbMapNum, internalCellIndex )) )
        {
            LOG_MAC_MSG( MAC_ALLOC_PRACH_RES, LOGDEBUG ,MAC_PUCCH,
                    getCurrentTick(), prachFrequencyOffset,
                    numPhysicalResources, preambleFormat,
                    ulSysFrameNum, ulSubFrame,
                    0, 0, __func__, "");

            /* Allocate the resources for first SF */
            updateUplinkRbMapForPrach( prachConfigIndex,
                    prachFrequencyOffset, rbMapNum, ulSubFrame, 
                    internalCellIndex );

            updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                    prachFrequencyOffset, numPhysicalResources, 
                    internalCellIndex );

            /* set PRACH flag in RBMap */
            setPRACHResourcesAllocated( rbMapNum, numPhysicalResources,
                    internalCellIndex );
            /* SPR 9729 fix start */
            /* Set the uciRachFlag to indicate RACH flag needs to
             * be set in UL Config. */
            if (PREAMBLE_FORMAT_0 == preambleFormat)
            {
                rbMapNode_p->uciRachFlag = TRUE;
            }
            /* SPR 9729 fix end */
        }
        /* Check for Preamble format to allocate the resources for next
           consecutive SF */
        if( PREAMBLE_FORMAT_1 == preambleFormat ||
                PREAMBLE_FORMAT_2 == preambleFormat ||
                PREAMBLE_FORMAT_3 == preambleFormat )
        {
            /* Get the RB map for next SF */
            UInt32 nextRbMapNum = (rbMapNum + 1 ) % getMaxULRBMaps( 
                    internalCellIndex );
            RbMapNode *nextRbMapNode_p = 
                rbMapNodeArr_gp[internalCellIndex] + nextRbMapNum;
            /* Reset RB map info */
            if(FALSE == nextRbMapNode_p->advanceAllocFlag)
            {
                *nextRbMapNode_p =
                    (ulRAInfo_gp[internalCellIndex] + 
                     ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
                /* We have allocated PRACH Resource only but we need to
                 * reserve advance PUCCH resources for this rbMap, thats why 
                 * this value is set, which means, no need to do init now.
                 */
                nextRbMapNode_p->advanceAllocFlag = ADVANCE_ALLOC_INIT_DONE;
            }
            if( !(isPRACHResourcesAllocated( nextRbMapNum, 
                            internalCellIndex ) ) )
            {
                /* Allocate the resources for first next SF */
                updateUplinkRbMapForPrach( prachConfigIndex,
                        prachFrequencyOffset, nextRbMapNum, ulSubFrame,
                        internalCellIndex );

                updateCEAvailRes(
                        &nextRbMapNode_p->availableResourceInfo[CE_REGION],
                        prachFrequencyOffset, numPhysicalResources,
                        internalCellIndex );

                setPRACHResourcesAllocated( nextRbMapNum,
                        numPhysicalResources, internalCellIndex );
                /* Set advanceAllocFlag to TRUE so that resources are
                 * reserved and will not be flushed in next tick */
                /* SPR 9729 fix start */
                /* Set the uciRachFlag to indicate RACH flag needs to
                 * be set in UL Config. */
                if( PREAMBLE_FORMAT_1 == preambleFormat ||
                        PREAMBLE_FORMAT_2 == preambleFormat )
                {
                    nextRbMapNode_p->uciRachFlag = TRUE;
                }
                /* SPR 9729 fix end */
            }
        }
        /* Check for Preamble format 3 to allocate the resources for
           SF + 2 */
        if( PREAMBLE_FORMAT_3 == preambleFormat )
        {
            /* Get the RB map for next SF */
            UInt32 nextRbMapNum = (rbMapNum + 2 ) % getMaxULRBMaps( 
                    internalCellIndex );
            RbMapNode *nextRbMapNode_p = 
                rbMapNodeArr_gp[internalCellIndex] + nextRbMapNum;
            /* Reset RB map info */
            if(FALSE == nextRbMapNode_p->advanceAllocFlag)
            {
                *nextRbMapNode_p =
                    (ulRAInfo_gp[internalCellIndex] + 
                     ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
                /* We have allocated PRACH Resource only but we need to
                 * reserve advance PUCCH resources for this rbMap, thats why 
                 * this value is set, which means, no need to do init now.
                 */
                nextRbMapNode_p->advanceAllocFlag = ADVANCE_ALLOC_INIT_DONE;
            }
            if( !( isPRACHResourcesAllocated( nextRbMapNum, 
                            internalCellIndex ) ) )
            {
                /* Allocate the resources for first next SF */
                updateUplinkRbMapForPrach( prachConfigIndex,
                        prachFrequencyOffset, nextRbMapNum, ulSubFrame,
                        internalCellIndex );

                updateCEAvailRes(
                        &nextRbMapNode_p->availableResourceInfo[CE_REGION],
                        prachFrequencyOffset, numPhysicalResources,
                        internalCellIndex );

                setPRACHResourcesAllocated( nextRbMapNum,
                        numPhysicalResources, internalCellIndex );
                /* Set advanceAllocFlag to TRUE so that resources are
                 * reserved and will not be flushed in next tick */
                /* SPR 9729 fix start */
                /* Set the uciRachFlag to indicate RACH flag needs to
                 * be set in UL Config. */
                nextRbMapNode_p->uciRachFlag = TRUE;
                /* SPR 9729 fix end */
            }
        }
    } while( 0 );

    /* SPR 9729 fix start */
    if( TRUE == rbMapNode_p->uciRachFlag )
    {
        numPrachFreqs = rbMapNode_p->allocatedPrachNumRbs /
            NUM_PRACH_PHYSICAL_RESOURCES;

        prachSetFrequencyResBits( numPrachFreqs,
                &(ulschUciRachInfo_p->isPrachResourceReserved) );
    }
    /* SPR 9729 fix end */
}
#else
/*****************************************************************************
 * Function Name  : processPUCCHAndPRACHReports
 * Inputs         : pdcchULOutputInfo_p - Its point to PdcchULOutputInfo structure.
 *                  rbMapNum - rbMap number
 *                  tti - Current TTI 
 *                  containerTick - Container tick
 *                  ulDelay - UL Delay
 *                  internalCellIndex - Cell-Index of Pcell or Scell
 * Outputs        :
 * Returns        : void
 * Description    : This function process the PUCCH Report corresponding to the
 *                  subFrame and allocates resources if UL grant is not given for the
 *                  UE which is being processed.
 *                  IF the UE has a UL grant and only SR was scheduled to be sent,
 *                  fills up ueCollisionArray to inform PUSCH module
 *                  It then processes resource allocation for all UEs whose ack/nack
 *                  is to be sent, but dont have UL grant
 *****************************************************************************/
void processPUCCHAndPRACHReports( 
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        UInt32 rbMapNum,
        /* SPR 15909 fix start */
        tickType_t tti,
        /* SPR 15909 fix end */
        UInt8 containerTick,
        UInt8 ulDelay,
        InternalCellIndex internalCellIndex )
{
    UInt8 allocCERBs = 0, allocCCRBs = 0;
    UInt32 prachFrequencyOffset = 0;
    SpsDlInfo *spsDlInfo_p = PNULL;
    UInt32 pdcchDlUECount = 0, pdcchDlUeIndex = 0;
    UInt8 nPUCCH = 0;
    UInt8  initAvailCCRes = 0;
    UInt8  initAvailCERes = 0;
    globalSrShortOpportunity *nodeShort_p = PNULL;
    UInt32 subFrameNum    = 0; 
    UInt32 sysFrameNum    = 0;
    UInt8 shortSubframeOffset = 0; 
    UInt32  ueIndex       = 0;
    UInt32  numPhysicalResources  = 0;
    ULUEContextInfo *uLUEContextInfo_p  = PNULL;
    DLUEContext     *ueDLContext_p = PNULL;
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /* SPR 10770, 10717 FIX START */
    /* SPR 15909 fix start */
    tickType_t ttiWithDelay = (tti + 4 + ulDelay);
    /* SPR 15909 fix end */
    UInt32 subFrameOffset = ttiWithDelay % MAX_PERIODICITY_SUBFRAME;
    /* SPR 10770, 10717 FIX END */
    UInt8 cceIndex = 0;
    UInt8 ulUEScheduledFlag = 0;
    UInt8 prbIndexSlot1 = 0;
    UInt8 prbIndexSlot2 = 0;
    ULSchUciRachPoolInfo* ulschUciRachInfo_p 
        =ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
    PdcchDLUEInfo *pdcchDLUEInfo_p = PNULL; 
    RACHConfigInfo *rachConfigInfo_p = 
        &(cellSpecificParams_g.cellConfigAndInitParams_p[
                internalCellIndex]->cellParams_p->rachConfigInfo);
    UInt8 cellCount = 0;
    SCellSchedulingQueueNode *sCellScheduledNode_p = PNULL; 
    UInt16 sCellQCount = 0;
    UInt8 isSCellScheduled = LTE_FALSE;

#ifdef PUCCH_RESERVE_ALWAYS
    UInt8 numberOfUEsScheduledInUplink = 0;
    UInt8 puschReserveOnlyFlagForSingleUE = 0;
    numberOfUEsScheduledInUplink = pdcchULOutputInfo_p->pdcchULNackUEInfo.nonAdaptiveCount +
        pdcchULOutputInfo_p->pdcchULNackUEInfo.adaptiveCount[CC_USER] +
        pdcchULOutputInfo_p->pdcchULNackUEInfo.adaptiveCount[CE_USER] +
        pdcchULOutputInfo_p->pdcchULNewUEInfo[CC_USER].count +
        pdcchULOutputInfo_p->pdcchULNewUEInfo[CE_USER].count     ;
    if (numberOfUEsScheduledInUplink ==  1)
    {
        puschReserveOnlyFlagForSingleUE = 1;
    }
    else
    {
        puschReserveOnlyFlagForSingleUE = 0;
    }        

#endif     

    /* SPR 10770, 10717 FIX START */
    subFrameNum = ttiWithDelay % MAX_SUB_FRAME;    
    sysFrameNum = (MODULO_ONEZEROTWOFOUR(ttiWithDelay/MAX_SUB_FRAME));
    initAvailCCRes = advInitAvailCCRes[internalCellIndex][rbMapNum];
    initAvailCERes = rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter;

    shortSubframeOffset = ttiWithDelay % SHORT_SR_PERIODICITY;
    /* SPR 10770, 10717 FIX END */

    /* CA Changes start*/
    /* Start processing UE's with SR(periodicity>2ms) or Reports to be sent
    */
    /* SPR 10770, 10717 FIX START */
    processLongSrOrReport(subFrameOffset, ttiWithDelay, subFrameNum,\
                          sysFrameNum, pdcchULOutputInfo_p,\
                          rbMapNum, containerTick, 
#ifdef PUCCH_RESERVE_ALWAYS        
                          puschReserveOnlyFlagForSingleUE, 
#endif
                          internalCellIndex);
    /* SPR 10770, 10717 FIX END */
    /* Finished processing UE's with SR(periodicity>2ms) or Reports to be sent
    */

    /* Rel9_upgrade_CR257 */
    /*SPR 16855 fix start*/
    nodeShort_p =(globalSrShortOpportunity *) getListNode(
            &(pucchSrShortReportDb_g[internalCellIndex][shortPeriodicSRMapShortInfo_gp[internalCellIndex]->activeIndex][shortSubframeOffset].ueList), 0);
    /*SPR 16855 fix end*/


    /*
     * Start processing UE's with SR or Reports to be sent
     */
    while (PNULL != nodeShort_p)
    {
        ueIndex = nodeShort_p->ueIndex;
        uLUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
        /** Coverity Fix 69675 : Start**/
        ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
        /** Coverity Fix 69675 : End **/

        if(MAC_FAILURE == validateAndUpdatePucchSrAndReportsNode(uLUEContextInfo_p, 
                     ueIndex, (sysFrameNum * MAX_SUBFRAME + subFrameNum)) )
        {
            nodeShort_p = (globalSrShortOpportunity *)
                getNextListNode((LTE_LIST_NODE *)(nodeShort_p));
            continue;
        }

        /* cceIndex is allocated for downlink schedule UE */
        cceIndex = 
            pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].dlCCEIndex;
        isSCellScheduled = LTE_FALSE; 

        for (cellCount = START_SCELL_INDEX; (cellCount <= MAX_NUM_SCELL) && !(isSCellScheduled); cellCount++)
        {
            /* Proceed only for Scell which are active */
            if (uLUEContextInfo_p->ulUEContext_p->ulUeScellContext_p[cellCount] && 
                    IS_SCELL_ACTIVE_AT_TTI(ueIndex, cellCount))
            {
                /* Check if ue is scheduled in SCell */
                /** SPR 12457 Fix : Start**/
                if ((ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]) &&
                        (ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus > 0x00) && 
                        (ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus < 0x03))
                { 
                    isSCellScheduled = LTE_TRUE; 

                    /* UCI HARQ of secondary cell*/
                    sCellQCount = SCELL_SCHEDULING_QUEUE_COUNT(internalCellIndex);
                    while (sCellQCount--)
                    {
                        DEQUEUE_SCELL_SCHEDULING_NODE(sCellScheduledNode_p, internalCellIndex);

                        /*** Coverity Fix 63550 : Start ***/
                        if ((sCellScheduledNode_p) && \
                                (sCellScheduledNode_p->ueIndex == uLUEContextInfo_p->ulUEContext_p->ueIndex))
                        {
                            FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                            ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus  = 0x0;
                            break;
                        }
                        /*** Coverity Fix 63550 : End ***/

                        /*** Coverity Fix 63536 : Start ***/
                        if (!ENQUEUE_SCELL_SCHEDULING_NODE(sCellScheduledNode_p, internalCellIndex))
                        {
                            FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                        }
                        /*** Coverity Fix 63536 : End ***/
                    }
                }
            }
        }
        /* Check if ACK/NACK along with SR report has to be sent */
        if ((INVALID_CCE_INDEX != cceIndex) 
                || isSCellScheduled
                )
        {
            processPUCCHShortSrAndAckNack(
                    uLUEContextInfo_p->ulUEContext_p,
                    ulschUciRachInfo_p,
                    rbMapNum
                    ,cceIndex
                    ,subFrameNum
                    ,pdcchULOutputInfo_p
#ifdef PUCCH_RESERVE_ALWAYS
                    ,puschReserveOnlyFlagForSingleUE
#endif
                    ,internalCellIndex
                    );
            nodeShort_p = (globalSrShortOpportunity *)
                getNextListNode((LTE_LIST_NODE *)nodeShort_p);
            continue;
        }
        /* Otherwise simply SR or report needs to be sent */
        processPUCCHShortSrOnly(
                uLUEContextInfo_p->ulUEContext_p, 
                ulschUciRachInfo_p, 
                rbMapNum
                ,pdcchULOutputInfo_p
                ,internalCellIndex
                );

        nodeShort_p = (globalSrShortOpportunity *)
            getNextListNode((LTE_LIST_NODE *)nodeShort_p);
    }
    /*
     * Start processing UE's which had no SR or Report to be sent
     * but may have ack/nack arriving
     */
    /* SPR 1101 changes start*/
    /* SPR 4301 chg */
    if (pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count)
    {
        buildUciHarqForTempUE(&pdcchULOutputInfo_p->pdcchTcrntiUEInfo, 
                pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count,   
                rbMapNum, ulschUciRachInfo_p, internalCellIndex);
    }
    /* SPR 4301 chg */
    /* SPR 1101 changes end*/
    pdcchDLUEInfo_p = &(pdcchULOutputInfo_p->pdcchDLUEInfo);
    uLUEContextInfo_p = PNULL;
    for (pdcchDlUECount = 0; pdcchDlUECount < pdcchDLUEInfo_p->count;
            pdcchDlUECount++)
    {
        pdcchDlUeIndex = pdcchULOutputInfo_p->pdcchDLUEInfo.
            pdcchDLUEArray[pdcchDlUECount];

        /*CCE Index allocated for Downlink Schedule UE.*/
        cceIndex = pdcchULOutputInfo_p->
            directIndexingUEInfo[pdcchDlUeIndex].dlCCEIndex;        

        /* UE is scheduled for uplink */
        if (INVALID_CCE_INDEX != cceIndex) 
        {
            /* allocate the Resources for Downlink Ack/Nack */
            uLUEContextInfo_p = &ulUECtxInfoArr_g[pdcchDlUeIndex];
            ueDLContext_p = dlUECtxInfoArr_g[pdcchDlUeIndex].dlUEContext_p;

            /** SPR 11205 Fix : Start ***/
            if (ueDLContext_p && IS_PCELL_OF_UE(ueDLContext_p, internalCellIndex))
            /** SPR 11205 Fix : End ***/
            {

                pdcchULOutputInfo_p->directIndexingUEInfo[pdcchDlUeIndex].
                    dlCCEIndex = INVALID_CCE_INDEX ;
                ulUEScheduledFlag = pdcchULOutputInfo_p->directIndexingUEInfo[pdcchDlUeIndex].
                    ulCCEIndex;    

		/* +- SPR 22413 */

                /* SPR 5369 changes start */
                if ( uLUEContextInfo_p->pendingDeleteFlag || 
                        PNULL == uLUEContextInfo_p->ulUEContext_p )
                {
                    lteWarning("Pending Delete Flag is set For UE Index cceIndex = [%d]",cceIndex);
                    continue;
                }
                /* SPR 5369 changes end */

                if (SETUP == uLUEContextInfo_p->ulUEContext_p->measGapConfigType)
                {
                    /* SPR 10770, 10717 FIX START */
                    if (TRUE == isTTIInMeasGapPeriod(uLUEContextInfo_p->ulUEContext_p, ttiWithDelay))
                        /* SPR 10770, 10717 FIX END */
                    {
                        /*ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                         *   pduType = MAC_SS_PDU_INVALID;
                         */
                        continue;
                    }
                }

                if ( MAX_ULSCH_UCI_SUPPORTED ==  ulschUciRachInfo_p->numberOfInfoPresent)
                {
                    ltePanic("[%s]:2. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                            __func__);
                    break;
                }        

                /* Calculate the nPUCCH DL ACK/NACK */
                spsDlInfo_p = &(ueDLContext_p->spsDlInfo);
                if(cceIndex < MAX_CCE)
                {
                    nPUCCH = cceIndex + n1PucchDLAckNack_g[internalCellIndex];
                }
                else
                {
                    nPUCCH = spsDlInfo_p->spsDlSetupInfo_p->N1PucchAnPersistentListVal.N1PucchAnPersistent[0];
                }

                /* PUCCH_RESERVE_ALWAYS Start */
#ifndef PUCCH_RESERVE_ALWAYS
                if (INVALID_CCE_INDEX == ulUEScheduledFlag)
#else
                    /* SPR 6825 Changes starts   */
                if((!puschReserveOnlyFlagForSingleUE )||
                    (INVALID_CCE_INDEX == ulUEScheduledFlag))
                   /* SPR 6825 Changes ends   */
#endif
                {
                     /* PUCCH_RESERVE_ALWAYS End */
                 	if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex (
                            nPUCCH, 
                            UCI_FORMAT_ONE, 
                            &prbIndexSlot1,
                            &prbIndexSlot2,
                            internalCellIndex
                            )
                       )
                     {
                            /* Review comment fix start GG13 */
                            LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                            getCurrentTick(),
                            /* CA-Ph2 Integration fix start */
                            uLUEContextInfo_p->ulUEContext_p->ueIndex,
                            /* CA-Ph2 Integration fix end */
                            nPUCCH,
                            UCI_FORMAT_ONE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,
                            "Invalid Configuraton For PUCCH Resources For DL Ack-Nack");
                            continue;
                            /* Review comment fix end GG13 */
                     }

                     allocateResourcesForPUCCHReports(
                     prbIndexSlot1,
                     prbIndexSlot2,
                     rbMapNum,
                     internalCellIndex
                     );
                }

                ulschUciRachInfo_p->ulschUciInfo[ulschUciRachInfo_p->numberOfInfoPresent].
                    pduType = MAC_SS_PDU_UCI_HARQ;

                /* Fill SS - DL Harq Info */
                /*SPR 1115 CHG*/
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
                /* CA-Ph2 Integration fix start */
                fillPucchHarqInfo(uLUEContextInfo_p->ulUEContext_p->ueIndex, \
                        &(ulschUciRachInfo_p->ulschUciInfo[ulschUciRachInfo_p->numberOfInfoPresent]), \
                        /* CA-Ph2 Integration fix end */
                        /** SPR 12457 Fix : Start**/
                        nPUCCH, rbMapNum, 
#ifdef PUCCH_RESERVE_ALWAYS
                        puschReserveOnlyFlagForSingleUE,
#endif
                        /* +- SPR 17777 */
                        pdcchULOutputInfo_p,subFrameNum,internalCellIndex);
                /* +- SPR 17777 */
#else           
                        /** SPR 12457 Fix : End **/
                ulschUciRachInfo_p->ulschUciInfo[ulschUciRachInfo_p->numberOfInfoPresent].
                    dlHarqInfo.pucchIndex = nPUCCH;
                lteMacSsCalcHarqSize(
                        /*Coverity_31965 Fix Start*/
                        &ulschUciRachInfo_p->ulschUciInfo
                        [ulschUciRachInfo_p->numberOfInfoPresent].
                        dlHarqInfo.harqSize ,
                        /*Coverity_31965 Fix End*/
                        pdcchULOutputInfo_p->directIndexingUEInfo[pdcchDlUeIndex].
                        dlDciFormat);
#endif
                /* SPR 1684 changes start */
                /* Fill rnti value */
                if( (SPS_SCHEDULING == ueDLContext_p->schType) ||
                        /* SPR 12079 fix start */
                        (pdcchULOutputInfo_p->directIndexingUEInfo[                        
                         uLUEContextInfo_p->ulUEContext_p->ueIndex].isSPSCCE == LTE_TRUE) )
                        /* SPR 12079 fix end */
                {
                    ulschUciRachInfo_p->ulschUciInfo
                        [ulschUciRachInfo_p->numberOfInfoPresent].\
                        rnti = spsDlInfo_p->SpsCrnti ;
                }
                else
                {
                    ulschUciRachInfo_p->ulschUciInfo
                        [ulschUciRachInfo_p->numberOfInfoPresent].\
                        rnti = uLUEContextInfo_p->ulUEContext_p->crnti;
                }
                /* SPR 1684 changes end */

                pdcchULOutputInfo_p->
                    directIndexingUEInfo[pdcchDlUeIndex].ulschUciInfoIndex = 
                    ulschUciRachInfo_p->numberOfInfoPresent;
                ulschUciRachInfo_p->ulschUciInfo
                    [ulschUciRachInfo_p->numberOfInfoPresent].ueDirectIndexInfo_p 
                    = &(pdcchULOutputInfo_p->directIndexingUEInfo[pdcchDlUeIndex]);

                ulschUciRachInfo_p->numberOfInfoPresent++;
            /* SPR# 13538 fix start */
                SET_HARQ_BLER_RECEIVED_ON_PUCCH(uLUEContextInfo_p->ulUEContext_p->ueIndex, subFrameNum);
            /* SPR# 13538 fix end */

                /** SPR 11916 Fix : Start**/
                /** SPR 12457 Fix : Start**/
                if ((ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]) &&
                    (ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus > 0x00) && 
                    (ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus < 0x03))
                { 
                    ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus  = 0x00;
                }
                /** SPR 12457 Fix : End **/
                /** SPR 11916 Fix : End **/
            }        
        }
    }
    /*Always reset the count*/
    pdcchDLUEInfo_p->count = 0;
    /* PUCCH Resources is allocated and set PUCCH Flag True*/
    if (IS_CA_ENABLED())
    {
        /** SPR 12457 Fix : Start**/
        processDlSecondaryCellQueue(pdcchULOutputInfo_p, ulschUciRachInfo_p, subFrameNum, \
                                    rbMapNum, ttiWithDelay,
#ifdef PUCCH_RESERVE_ALWAYS        
                                    puschReserveOnlyFlagForSingleUE,
#endif        
                                    internalCellIndex);
        
        /** SPR 12457 Fix : End **/
    }

    /*Fix for SPR 860 */
    /*SPR 2446 Fix Begins*/
    setFreeRbsAndPUCCHFlag(rbMapNum, internalCellIndex);
    /*SPR 2446 Fix Ends*/

    ulBufferPool_g[internalCellIndex].cellEdgeRBs -= (initAvailCERes - 
            rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter);
    ulBufferPool_g[internalCellIndex].totalRBs -= (initAvailCCRes  - 
            rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter);

    /* reset init resources */
    allocCERBs = initAvailCERes -
        rbMapNode_p->availableResourceInfo[CE_REGION].freeRBCounter;
    allocCCRBs = initAvailCCRes  -
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter;
    if(ulBufferPool_g[internalCellIndex].cellEdgeRBs > allocCERBs)
    {
        ulBufferPool_g[internalCellIndex].cellEdgeRBs -= allocCERBs; 
    }
    else
    {
        ulBufferPool_g[internalCellIndex].cellEdgeRBs = 0;
    }
    if(ulBufferPool_g[internalCellIndex].totalRBs > allocCCRBs)
    {
        ulBufferPool_g[internalCellIndex].totalRBs -= allocCCRBs;
    }
    else
    {
        ulBufferPool_g[internalCellIndex].totalRBs = 0;
    }

    /* Allocate the Resources for PRACH */
    if ( isValidFrameForPRACH( subFrameNum, sysFrameNum,
                &prachFrequencyOffset, &numPhysicalResources, 
                internalCellIndex ) )
    {
        /* Filling PRACH Info */
        /*SPR 1115 CHG*/
        ulschUciRachInfo_p->isPrachResourceReserved = FALSE;        

        if ( numPhysicalResources )
        {
            /* Get the Preamble format from the PRACH Config Index */
            PreambleFormat preambleFormat =
                preambleFormatFrmPrachConfigIndex( internalCellIndex );

            /* For Preamble format 0, UL RBs for 1 SF needs to be reserved.
               For Preamble format 1 and 2, UL RBs for 2 SFs needs to be
               reserved. For Preamble format 3, UL RBs for 3 SFs needs to be
               reserved, as derived from 36.211, Section 5.7 */

            /* Allocate the resources for first SF */
            if(!(isPRACHResourcesAllocated(rbMapNum, internalCellIndex )))
            {
                LOG_MAC_MSG( MAC_ALLOC_PRACH_RES, LOGDEBUG ,MAC_PUCCH,
                        getCurrentTick(), prachFrequencyOffset,
                        numPhysicalResources, preambleFormat, sysFrameNum,
                        subFrameNum, 0, 0, __func__, "");

                if (MAC_SUCCESS == allocateResourcesUsingRBIndex(
                            prachFrequencyOffset, numPhysicalResources, 
                            rbMapNum, internalCellIndex ) )
                {
                    updateCEAvailRes(
                            &rbMapNode_p->availableResourceInfo[CE_REGION],
                            prachFrequencyOffset, numPhysicalResources,
                            internalCellIndex );
                    setPRACHResourcesAllocated(rbMapNum, internalCellIndex );

                    /* SPR 9729 fix start */
                    /* Set the uciRachFlag to indicate RACH flag needs to
                     * be set in UL Config. */
                    if( PREAMBLE_FORMAT_0 == preambleFormat )
                    {
                        rbMapNode_p->uciRachFlag = TRUE;
                    }
                    /* SPR 9729 fix end */
                }
            }

            /* Check for Preamble format to allocate the resources for next
               consecutive SF */
            if( PREAMBLE_FORMAT_1 == preambleFormat ||
                    PREAMBLE_FORMAT_2 == preambleFormat ||
                    PREAMBLE_FORMAT_3 == preambleFormat )
            {
                /* Fetch the global context of RB map for next SF  */
                UInt32 nextRbMap = (rbMapNum + 1) % NUM_OF_RB_MAP;
                RbMapNode *nextRbMapNode_p = 
                    rbMapNodeArr_gp[internalCellIndex] + nextRbMap;
                /* Reset RB map info */
                if(FALSE == nextRbMapNode_p->advanceAllocFlag)
                {
                    *nextRbMapNode_p =
                        (ulRAInfo_gp[internalCellIndex] + 
                        ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
                }
                if(!(isPRACHResourcesAllocated(nextRbMap, internalCellIndex )))
                {
                    /* Allocate the resources for next SF */
                    if( MAC_SUCCESS == allocateResourcesUsingRBIndex(
                                prachFrequencyOffset, numPhysicalResources,
                                nextRbMap, internalCellIndex ) )
                    {
                        updateCEAvailRes(
                                &nextRbMapNode_p->availableResourceInfo[
                                CE_REGION ], prachFrequencyOffset,
                                numPhysicalResources, internalCellIndex );
                        setPRACHResourcesAllocated( nextRbMap, 
                                internalCellIndex );
                        /* SPR 9729 fix start */
                        /* Set the uciRachFlag to indicate RACH flag needs to
                         * be set in UL Config. */
                        if( PREAMBLE_FORMAT_1 == preambleFormat ||
                                PREAMBLE_FORMAT_2 == preambleFormat )
                        {
                            nextRbMapNode_p->uciRachFlag = TRUE;
                        }
                        /* SPR 9729 fix end */

                        /* Set advanceAllocFlag to TRUE so that resources are
                         * reserved and will not be flushed in next tick */
                        nextRbMapNode_p->advanceAllocFlag = 
                            ADVANCE_ALLOC_INIT_DONE;
                    }
                }
            }
            /* Check for Preamble format 3 to allocate the resources for
               SF + 2 */
            if( PREAMBLE_FORMAT_3 == preambleFormat )
            {
                /* Fetch the global context of RB map for next SF  */
                UInt32 nextRbMap = (rbMapNum + 2) % NUM_OF_RB_MAP;
                RbMapNode *nextRbMapNode_p = 
                    rbMapNodeArr_gp[internalCellIndex] + nextRbMap;
                /* Reset RB map info */
                if(FALSE == nextRbMapNode_p->advanceAllocFlag)
                {
                    *nextRbMapNode_p =
                        (ulRAInfo_gp[internalCellIndex] + 
                        ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
                }
                if(!(isPRACHResourcesAllocated(nextRbMap, internalCellIndex )))
                {
                    /* Allocate the resources for next SF */
                    if( MAC_SUCCESS == allocateResourcesUsingRBIndex(
                                prachFrequencyOffset, numPhysicalResources,
                                nextRbMap, internalCellIndex ) )
                    {
                        updateCEAvailRes(
                                &nextRbMapNode_p->availableResourceInfo[
                                CE_REGION ], prachFrequencyOffset,
                                numPhysicalResources, internalCellIndex );
                        setPRACHResourcesAllocated( nextRbMap, 
                                internalCellIndex );
                        /* SPR 9729 fix start */
                        /* Set the uciRachFlag to indicate RACH flag needs to
                         * be set in UL Config. */
                        nextRbMapNode_p->uciRachFlag = TRUE;
                        /* SPR 9729 fix end */

                        /* Set advanceAllocFlag to TRUE so that resources are
                         * reserved and will not be flushed in next tick */
                        nextRbMapNode_p->advanceAllocFlag = 
                            ADVANCE_ALLOC_INIT_DONE;
                    }
                }
            }
        }        
    }

    /* Filling PRACH Info */
    /* SPR 9729 fix start */
    if( TRUE == rbMapNode_p->uciRachFlag )
    {
        PRACHConfiguration *prachConfig_p =
            &rachConfigInfo_p->prachConfiguration[
            rachConfigInfo_p->prachActiveIndex];

        ulschUciRachInfo_p->isPrachResourceReserved = TRUE;
        ulschUciRachInfo_p->prachFreqOffset =
            prachConfig_p->prachFrequencyOffset;
    }
    /* SPR 9729 fix end */
    else
    {
        ulschUciRachInfo_p->isPrachResourceReserved = FALSE;
    }
    /* reset init resources */
    advInitAvailCCRes[internalCellIndex][rbMapNum] = 0;
}
#endif
/*ca-tdd pucch cod changes start*/
#ifdef TDD_CONFIG
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
/*****************************************************************************
 * Function Name  : allocateResourceForTddHarq 
 * Inputs         : ulSubFrame - SF
 *					ueIndex - ueId
 *                  rbMapNum - rbMap number
 *                  containerTick - Container tick
 *                  cellIndex - cell index for primary or secondary cell.
 * Outputs        : NONE
 * Returns        : 
 * Description    : This function allocates pucch hars resources.
 *****************************************************************************/
void allocateResourceForTddHarq(
        UInt32 ulSubFrame,
        UInt32  ueIndex,
        UInt32  rbMapNum,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UeScheduledInfo * ulUeScheduledInfo_p,
        InternalCellIndex internalCellIndex)
{
    UInt32 Np = 0;
    UInt32 Npnext = 0;
    UInt32 cceIndexI = 0;
    UInt32 nPUCCH[2] = {HARQ_INVALID,HARQ_INVALID};
    UInt8  k = 0;
    UInt8  i = 0;
    UInt8  m = 0;
    UInt8  dlIndex = 0;
    UInt8 rbIndexSlot1 = 0;
    UInt8 rbIndexSlot2 = 0;
    UInt8  M = globalDlAssociationSet_gp[internalCellIndex][ulSubFrame].count;
    SInt32 signedOffset = 0;
    UInt16  numberOfInfoPresent = 0;
    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    UInt8 resouceAllocCount = 0;
    UInt8 resourceIndex = 1;
    DLUEContext * dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    LTE_MAC_UT_LOG(LOG_MINOR,PUCCH_MODULE,"ACK/NACKS will be sent via muxing with M: [%d]", M);
    if (1 == M)
    {
        return acksOnlyByBundlingOrMequalsOne(ulSubFrame, ueIndex, 
                rbMapNum, 
                ulschUciRachInfo_p,
                ulUeScheduledInfo_p,
                /*SPR 19504 fix start*/
                FALSE,
                /*SPR 19504 fix end*/
                internalCellIndex);
    }
    /*
     * When M >1
     */
    /* SPR 16569 Changes */
    for (i = M; i > 0; i--)
    {
        k = globalDlAssociationSet_gp[internalCellIndex][ulSubFrame].sortedKwithSmallM[i-1].k;
        signedOffset = (ulSubFrame - k);
        signedOffset = signedOffset % MAX_SUB_FRAME;
        dlIndex = (signedOffset) < 0?(signedOffset + MAX_SUB_FRAME):(signedOffset);
        m = globalDlAssociationSet_gp[internalCellIndex][ulSubFrame].sortedKwithSmallM[i-1].smallM;
    /* SPR 16569 Changes */
        if (INVALID_CCE_INDEX != ulUeScheduledInfo_p->dlCceIndex[dlIndex] && (resouceAllocCount < 2))
        {
            cceIndexI = ulUeScheduledInfo_p->dlCceIndex[dlIndex];

            pucchGetNpsForCCE(cceIndexI, &Np, &Npnext,internalCellIndex);
            /*
             * using m here instead of i in formula, 
             * because we are reading K values in sorted order 
             * and not as given in table 10.1-1 .
             * Hence m is actually i.
             */
            LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"M=[%d], m=[%d], Np=[%d], Np+1=[%d], cceIndexI");
            if (2 == M)
            {
                resourceIndex = resourceIndex - i;
                nPUCCH[resourceIndex] = (M - m -1)*Np + (m*Npnext) + cceIndexI + n1PucchDLAckNack_g[internalCellIndex];
            }
            else    
            {    
                nPUCCH[resouceAllocCount] = (M - m -1)*Np + (m*Npnext) + cceIndexI + n1PucchDLAckNack_g[internalCellIndex];
            }    
            /*+COVERITY 5.3.0 - 25303*/
            if(MAC_FAILURE == getPRBFromPUCCHResourceIndex(nPUCCH[resouceAllocCount], UCI_FORMAT_ONE_B, 
                        &rbIndexSlot1, &rbIndexSlot2, internalCellIndex))
            {
                return;    
            }
            /*-COVERITY 5.3.0 - 25303*/
            if (MAC_SS_PDU_UCI_CQI_HARQ != ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType)
               allocateResourcesForPUCCHReports(rbIndexSlot1, rbIndexSlot2, rbMapNum, internalCellIndex);
            resouceAllocCount++;
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"No cces allocated for ueIndex:[%d], "
                    "for which ack/nck is expected in UL subframe:[%d]\n", ueIndex, ulSubFrame);
        }
        /** SPR 14277 Changes Start **/
        /* + SPR_14522 */
        if(dlUEContext_p && dlUEContext_p->scellCount &&
              (dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]))
        /* - SPR_14522 */
        {    
            if(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.sCellSchInfo[dlIndex])
            {
                /* + SPR_14768 */
                /* dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus = 1; */
                /* - SPR_14768 */
                dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.sCellSchInfo[dlIndex] = 0;
            }
        }
        /** SPR 14277 Changes End **/
    }
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType 
        = MAC_SS_PDU_UCI_HARQ;
    /* SPR 1684 changes start */
    /* Fill rnti value */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        rnti = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->crnti;
    /* SPR 1684 changes start */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        dlHarqInfo.harqSize = LTE_MAC_SS_TDD_CALC_HARQ_SIZE(UCI_FORMAT_ONE_B); 

//#ifdef FAPI_4_0_COMPLIANCE
#if 1
    fillPucchHarqInfoForMMoreThanOne(ueIndex,&(ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent]), \
            nPUCCH, rbMapNum,M,resouceAllocCount,ulUeScheduledInfo_p,
            ulSubFrame, /*pdcchULOutputInfo_p,*/internalCellIndex);
#else
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.harqSize = LTE_MAC_SS_TDD_CALC_HARQ_SIZE(pucchFormat);

    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        dlHarqInfo.acknackMode = ulUECtxInfoArr_g[ueIndex].
        ulUEContext_p->tddAckNackFeedbackMode;
    /* Fill extended harq info */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.numOfPUCCHResource = resouceAllocCount;
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_0 = nPUCCH[0];
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_1 = nPUCCH[1];
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_2 = 0;
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_3 = 0;

#endif
    /*Set this index in ueScheduledInfo. It will be used by SRS Mgr 
      to check collisions with PUCCH pdu types*/
    ulUeScheduledInfo_p->ulschUciInfoIndex = numberOfInfoPresent;
    /*Set the pointer in ueScheduledInfo in ulschUciRachInfo. This will
      be used to reset the ulschUciInfoIndex during prepareULConfigReq*/
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        ueScheduledInfo_p = ulUeScheduledInfo_p;
    ulschUciRachInfo_p->numberOfInfoPresent++;

}
#endif
#endif

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : processLongSrOrReport 
 * Inputs         : subFrameOffset 
 *                  tti - Global TTI
 *					subFrameNum - SF
 *					sysFrameNum - SFN
 *					pdcchULOutputInfo_p - PDDCH UL Output
 *                  rbMapNum - rbMap number
 *                  containerTick - Container tick
 *                  puschReserveOnlyFlagForSingleUE - TRUE if only one ue 
 *                                                    FALSE  otherwise
 *                  cellIndex - cell index for primary or secondary cell.
 * Outputs        : NONE
 * Returns        : Pointer to node globalSrOpportunityAndPeriodicReports.
 * Description    : This function will provide node to operate on.
 *****************************************************************************/
STATIC void  processLongSrOrReport(
        UInt16 subFrameOffset,
        /* SPR 15909 fix start */
        tickType_t tti,
        /* SPR 15909 fix end */
        UInt32 subFrameNum,
        UInt32 sysFrameNum,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        UInt32 rbMapNum,
        UInt8 containerTick,
#ifdef PUCCH_RESERVE_ALWAYS        
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif        
        UInt8 cellIndex
        )
{
    /* SPR 15909 fix start */
    tickType_t ulTti = 0;
    /* SPR 15909 fix end */
    UInt32 totalWrapNum = 0;
    UInt8 isCQIMasked = FALSE;
    UInt8 cellCount = 0;
    UInt16 ueIndex = 0;
    UInt16 selectedCellIndex = 0;
    UInt8 isCSIScheduled = TRUE;
    UInt8 isSCellScheduled = FALSE;
    /** SPR 12165 Fix : Start **/
    UInt8 ueReportOrOpportunityType = 0;
    UInt8 pCellUeReportOrOpportunityType = 0;
    UInt8 sCellUeReportOrOpportunityType = 0;
    /** SPR 12165 Fix : End **/
    ULUEContextInfo   *uLUEContextInfo_p = PNULL; 
    ULUEContext *ulUEContext_p = PNULL;
    SCellSchedulingQueueNode *sCellScheduledNode_p = PNULL; 
    globalSrOpportunityAndPeriodicReports *longSrOrReportNode_p = PNULL;
    DLUEContext     *ueDLContext_p = PNULL;
    UInt8 cceIndex = 0;
    UInt8 ulUEScheduledFlag = 0;
    ULSchUciRachPoolInfo* ulschUciRachInfo_p 
        =ulSchUciRachContainer_gp[cellIndex] + containerTick;
    SpsDlInfo *spsDlInfo_p = PNULL;
    UInt8 nPUCCH = 0;
    UInt16 sCellQCount = 0;
    UInt16 numberOfInfoPresent = 0;
    /**** Coverity Fix 63673: Start ****/
    UeCellInfo ueCellInfo = {0};
    /**** Coverity Fix 63673: End ****/

    ulTti = sysFrameNum * MAX_SUBFRAME + subFrameNum;
    totalWrapNum = ulTti/MAX_PERIODICITY_SUBFRAME;

    /*SPR 16855 fix start*/
    UInt16 activeIndex = longPeriodicSRCQIPMIRIMapInfo_gp[cellIndex]->activeIndex;
    /*SPR 16855 fix end*/

    /*SPR 16855 fix start*/
    longSrOrReportNode_p =(globalSrOpportunityAndPeriodicReports *) getListNode(
            &(pucchSrAndReportsDb_g[cellIndex][activeIndex][subFrameOffset].ueList), 0);
    /*SPR 16855 fix end*/
    /* Algo :
       1. Get long node.
       2. Validate the node & check of report type expected.
       2. Check if scell is active for selected node.if yes get report type.
       3. Compare pcell or scell reports according to priority & drop CSI Report.
       e.g. SR > RI > WB_CQI > SB_CQI report of any cell,if same report on 
       both cells PCell_REPORT > SCell_REPORT & as soon as node
       found break as priority is to be given lowest serving 
       SCell Index.
       4. if no SCell has report process node for PCell only.
       */
    while (PNULL != longSrOrReportNode_p)
    {
        /**** Coverity Fix 62962 : Start ****/
        ueCellInfo.isPCell = TRUE;
        isCSIScheduled = FALSE;
        /**** Coverity Fix 62962 : End ****/
        isSCellScheduled = FALSE;
        /* + SPR 11795*/
        selectedCellIndex = cellIndex;
        ueReportOrOpportunityType = pCellUeReportOrOpportunityType = sCellUeReportOrOpportunityType = 0;
        /* - SPR 11795*/
        ueIndex = longSrOrReportNode_p->ueIndex;
        /* CLPC CHANGE */
        RESET_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex, subFrameNum);
        /* CLPC CHANGE */

        uLUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
        ulUEContext_p = uLUEContextInfo_p->ulUEContext_p;
        ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

        /* Validate PUCCH node for measgap & empty checks*/
        if (MAC_FAILURE == validateAndUpdatePucchSrAndReportsNode(
                    uLUEContextInfo_p, ueIndex, tti) )
        {
            longSrOrReportNode_p = (globalSrOpportunityAndPeriodicReports *)
                getNextListNode((LTE_LIST_NODE *)(longSrOrReportNode_p));
            continue;
        }

        /*Cyclomatic Complexity changes - ends here*/
        /** SPR 12165 Fix : Start **/
        /* + SPR 11795*/
        ulUEScheduledFlag =
            pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].ulCCEIndex;
        /* - SPR 11795*/
        /** SPR 12165 Fix : Start **/

        /* Calculate which type of report type expected */
        pCellUeReportOrOpportunityType = getValidReportTypesForWrapArounds(longSrOrReportNode_p, 
                ulUEContext_p, totalWrapNum, cellIndex);

        /** SPR 12165 Fix : Start **/
        if (CHECK_BIT_SET(pCellUeReportOrOpportunityType,CQI_REPORT) || \
                CHECK_BIT_SET(pCellUeReportOrOpportunityType,WIDEBAND_CQI_REPORT) ||
                CHECK_BIT_SET(pCellUeReportOrOpportunityType,RI_REPORT))
            /** SPR 12165 Fix : End **/
        {
            isCSIScheduled = TRUE;
        }

        for (cellCount = START_SCELL_INDEX; cellCount <= MAX_NUM_SCELL; cellCount++)
        {
            /* Proceed only for Scell which are active */
            if (ulUEContext_p->ulUeScellContext_p[cellCount] && 
                    IS_SCELL_ACTIVE_AT_TTI(ueIndex, cellCount))
            {

                /* Check if ue is scheduled in SCell */
                /** SPR 12457 Fix : Start**/
                if ((ueDLContext_p->dlUeScellContext_p[cellCount]->pucchReqInfo.schedulingStatus > 0) &&
                        (ueDLContext_p->dlUeScellContext_p[cellCount]->pucchReqInfo.schedulingStatus < 3))
                /** SPR 12457 Fix : End **/
                {
                    isSCellScheduled = TRUE;
                }

                /* Calculate which type of report type expected */
                sCellUeReportOrOpportunityType = getValidReportTypesForWrapAroundsForSCell(longSrOrReportNode_p, 
                        ulUEContext_p, totalWrapNum, cellCount,cellIndex);
                if (!sCellUeReportOrOpportunityType)
                {
                    continue;
                }

                /* Priority is to be given to RI_REPORT > WIDEBAND_CQI > SUBABND_CQI for 
                   any cell & then if same report on both cells PCell Reports is given priority 
                   over SCell reports */
                /**** Coverity Fix 62962 : Start ****/
                /** SPR 12165 Fix : Start **/
                if (CHECK_BIT_SET(pCellUeReportOrOpportunityType,SCHEDULING_REQUEST))
                    /** SPR 12165 Fix : End **/
                {
                    ueCellInfo.isPCell = FALSE;
                    selectedCellIndex = cellCount;
                    ueCellInfo.sCellIndex = cellCount;
                    ueCellInfo.sCellInternalCellIndex = ulUEContext_p->ulUeScellContext_p[cellCount]\
                                                        ->internalCellIndex;
                    ueReportOrOpportunityType = sCellUeReportOrOpportunityType;
                    /** SPR 12165 Fix : Start **/
                    if (isCSIScheduled)
                    {
                        UNSET_BIT_AT_POS(pCellUeReportOrOpportunityType,SCHEDULING_REQUEST);
                        if (pCellUeReportOrOpportunityType >= sCellUeReportOrOpportunityType)
                        {
                            ueCellInfo.isPCell = TRUE;
                            selectedCellIndex = cellIndex;
                            ueReportOrOpportunityType = pCellUeReportOrOpportunityType;
                            if (INVALID_CCE_INDEX == ulUEScheduledFlag)
                            {
                                dropCQIReport(ulUEContext_p,ueDLContext_p,!ueCellInfo.isPCell,\
                                        &sCellUeReportOrOpportunityType,cellCount);
                            }
                        }
                        else
                        {
                            if (INVALID_CCE_INDEX == ulUEScheduledFlag)
                            {
                                dropCQIReport(ulUEContext_p,ueDLContext_p,!ueCellInfo.isPCell,\
                                        &pCellUeReportOrOpportunityType,cellIndex);
                            }
                        }    
                    }
                    isCSIScheduled = TRUE;
                    /** SPR 12165 Fix : End **/
                    SET_BIT_AT_POS(ueReportOrOpportunityType,SCHEDULING_REQUEST);
                    break;
                }
                /**** Coverity Fix 62962 : End ****/
                if (pCellUeReportOrOpportunityType >= sCellUeReportOrOpportunityType)
                {
                    ueCellInfo.isPCell = TRUE;
                    selectedCellIndex = cellIndex;
                    ueReportOrOpportunityType = pCellUeReportOrOpportunityType;
                    isCSIScheduled = TRUE;
                    /** SPR 12165 Fix : Start **/
                    if (INVALID_CCE_INDEX == ulUEScheduledFlag)
                    {
                        dropCQIReport(ulUEContext_p,ueDLContext_p,!ueCellInfo.isPCell,\
                                &sCellUeReportOrOpportunityType,cellCount);
                    }
                    /** SPR 12165 Fix : End **/
                    break;
                }
                else 
                {
                    isCSIScheduled = TRUE;
                    ueCellInfo.isPCell = FALSE;
                    selectedCellIndex = cellCount;
                    ueCellInfo.sCellIndex = cellCount;
                    ueCellInfo.sCellInternalCellIndex = ulUEContext_p->ulUeScellContext_p[cellCount]\
                                                        ->internalCellIndex;
                    ueReportOrOpportunityType = sCellUeReportOrOpportunityType;
                    /** SPR 12165 Fix : Start **/
                    if (INVALID_CCE_INDEX == ulUEScheduledFlag)
                    {
                        dropCQIReport(ulUEContext_p,ueDLContext_p,!ueCellInfo.isPCell,\
                                &pCellUeReportOrOpportunityType,cellIndex);
                    }
                    /** SPR 12165 Fix : End **/
                    break;
                }
            }//end of active SCell if
        }//end of active SCell for Loop

        /* if only primary cell CSI or SR report needs to be sent*/
        if (pCellUeReportOrOpportunityType && !sCellUeReportOrOpportunityType)
        {
            ueCellInfo.isPCell = TRUE;
            selectedCellIndex = cellIndex;
            ueReportOrOpportunityType = pCellUeReportOrOpportunityType;
        }
        else if(!pCellUeReportOrOpportunityType && !sCellUeReportOrOpportunityType)
        {

            longSrOrReportNode_p = (globalSrOpportunityAndPeriodicReports *)
                getNextListNode((LTE_LIST_NODE *)longSrOrReportNode_p);
            continue;
        }

        /*Cyclomatic Complexity changes - starts here*/
        updateBwPartOnWideBandSubBandReport(ulUEContext_p,
                ulTti,sysFrameNum,subFrameNum,&ueCellInfo,cellIndex);

        /* Review Comment fix GK14 start */
        /*Cyclomatic Complexity changes - starts here*/
        updateIsCQIMasked(ulUEContext_p,
                /*14766 start*/
                ueDLContext_p,
                /*14766 end */
                &isCQIMasked,
                ulTti,
                ulUEScheduledFlag);
        /*Cyclomatic Complexity changes - ends here*/
        /* Review Comment fix GK14 end */

        /* OPT */

        /*
         * Check if ACK/NACK along with SR/report has to be sent
         */
        if ( !CHECK_BIT_SET(pCellUeReportOrOpportunityType,SCHEDULING_REQUEST) )
        {
            /* ++ SPR 19426 -code Removed -- */
            if (isCQIMasked)
            {
                longSrOrReportNode_p =(globalSrOpportunityAndPeriodicReports *)
                    getNextListNode((LTE_LIST_NODE *)longSrOrReportNode_p);
                continue;
            }
        }

        numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
        /*Cyclomatic Complexity changes - ends here*/

        /* cceIndex is allocated for downlink schedule UE */ 
        cceIndex = 
            pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].dlCCEIndex;
        /*ulUEScheduledFlag(cceIndex is allocated for uplink schedule UE)*/
        /* PUCCH_RESERVE_ALWAYS Start */
        /* SPR 6825 Changes Starts */

#ifdef FDD_CONFIG
        /* Calculate the nPUCCH DL ACK/NACK */
        if (INVALID_CCE_INDEX != cceIndex)
        {        
            spsDlInfo_p = &(ueDLContext_p->spsDlInfo);
            if(cceIndex < MAX_CCE)
            {
                /* + SPR 11795*/
                nPUCCH = cceIndex + n1PucchDLAckNack_g[cellIndex];
                /* - SPR 11795*/
            }
            else
            {
                nPUCCH = spsDlInfo_p->spsDlSetupInfo_p->N1PucchAnPersistentListVal.N1PucchAnPersistent[0];
            }
        }
#else
        /* Calculate the nPUCCH DL ACK/NACK */
        if (INVALID_CCE_INDEX != cceIndex)
        {        
            if(cceIndex < MAX_CCE)
            {
                /* + SPR 11795*/
                nPUCCH = cceIndex + n1PucchDLAckNack_g[cellIndex];
                /* - SPR 11795*/
            }
        }
#endif
        /* SPR 1684 changes start */
        /* + SPS_TDD_Changes */
        /* + SPR 16336 */
        ulschUciRachInfo_p->ulschUciInfo[ulschUciRachInfo_p->numberOfInfoPresent].\
            rnti = uLUEContextInfo_p->ulUEContext_p->crnti;
        /* - SPR 16336 */
        /* - SPS_TDD_Changes */
        /* Algo :
         *   1. if scheduling on SCell drop CSI reports of both cells if simultaneousAckNackAndCQI  
         *      is true & in case of false also need to drop so only UCI_HARQ_PDU will go.
         *   2. if only scheduled in PCell & CSI is there then if simultaneousAckNackAndCQI 
         *      is true then multiplexed pdu will go.
         */ 
        /** SPR 11207 Fix : Start ***/
        if(isSCellScheduled)
        {
            /* UCI HARQ of secondary cell*/
            sCellQCount = SCELL_SCHEDULING_QUEUE_COUNT(cellIndex);
            while (sCellQCount--)
            {
                DEQUEUE_SCELL_SCHEDULING_NODE(sCellScheduledNode_p, cellIndex);

                /*** Coverity Fix 63550 : Start ***/
                if ((sCellScheduledNode_p) && \
                        (sCellScheduledNode_p->ueIndex == ulUEContext_p->ueIndex))
                    /*** Coverity Fix 63550 : Start ***/
                {
                    /** SPR 12165 Fix : Start **/
                    if (CHECK_BIT_SET(ueReportOrOpportunityType,SCHEDULING_REQUEST) ||
                            (INVALID_CCE_INDEX != ulUEScheduledFlag) )
                    {
                        if ((isCSIScheduled) && (INVALID_CCE_INDEX == ulUEScheduledFlag))
                        {
                            dropCQIReport(ulUEContext_p, ueDLContext_p, ueCellInfo.isPCell,\
                                    &ueReportOrOpportunityType,selectedCellIndex);
                        }
                        /* + SPR 11471 Changes */
                        /*if ((ulUEContext_p->pucchConfigInfo.simultaneousAckNackAndCQI))*/
                        /* - SPR 11471 Changes */
                        {
                            /*check  parameter simultaneousAckNackAndCQI is TRUE*/
                            processPUCCHSrOrReportAndAckNack(&ueReportOrOpportunityType,
                                    uLUEContextInfo_p->ulUEContext_p, ulschUciRachInfo_p,
                                    /* +- SPR 17777 */
                                    subFrameNum,
#ifdef TDD_CONFIG
                                    ssFrameNum,
#endif
                                    rbMapNum,
                                    /* +- SPR 17777 */
                                    /* Rel9_upgrade_CR410 */
                                    /* +- SPR 17777 */
#ifdef FDD_CONFIG
                                    ulUEScheduledFlag,
#endif
                                    cceIndex, pdcchULOutputInfo_p, &ueCellInfo
                                    /* +- SPR 17777 */
#ifdef PUCCH_RESERVE_ALWAYS
                                    ,puschReserveOnlyFlagForSingleUE
#endif
                                    ,tti, cellIndex);
                        }
                    }
                    else
                    {

                        if (isCSIScheduled)
                        {
                            dropCQIReport(ulUEContext_p, ueDLContext_p, ueCellInfo.isPCell,\
                                    &ueReportOrOpportunityType,selectedCellIndex);
                        }
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_UCI_HARQ;
                        /** SPR 12457 Fix : Start**/

#ifdef FAPI_4_0_COMPLIANCE
                        fillPucchHarqInfo(ulUEContext_p->ueIndex,
                                &(ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent]), \
                                nPUCCH, rbMapNum,
#ifdef PUCCH_RESERVE_ALWAYS
                                puschReserveOnlyFlagForSingleUE,
#endif
                                /* +- SPR 17777 */
                              pdcchULOutputInfo_p,  subFrameNum,  cellIndex);
                        /* +- SPR 17777 */
#else           
                        /** SPR 12457 Fix : End **/
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            dlHarqInfo.pucchIndex = nPUCCH;
                        lteMacSsCalcHarqSize(
                                /*Coverity_31965 Fix Start*/
                                &ulschUciRachInfo_p->ulschUciInfo
                                [ulschUciRachInfo_p->numberOfInfoPresent].
                                dlHarqInfo.harqSize ,
                                /*Coverity_31965 Fix End*/
                                pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].
                                dlDciFormat);
#endif
                        ulschUciRachInfo_p->ulschUciInfo
                            [ulschUciRachInfo_p->numberOfInfoPresent].ueDirectIndexInfo_p 
                            = &(pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex]);
                        pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].dlCCEIndex = INVALID_CCE_INDEX;
                        pdcchULOutputInfo_p->
                            directIndexingUEInfo[ueIndex].ulschUciInfoIndex = 
                            ulschUciRachInfo_p->numberOfInfoPresent;
                        ulschUciRachInfo_p->numberOfInfoPresent++;
                    }

                    FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                    /* + SPR 11471 Changes */
                    /** SPR 12457 Fix : Start**/
                    ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus  = 0x0;
                    /** SPR 12457 Fix : End **/
                    /* - SPR 11471 Changes */
                    break;                    
                }
                /*** Coverity Fix 63536 : Start ***/
                if (!ENQUEUE_SCELL_SCHEDULING_NODE(sCellScheduledNode_p, cellIndex))
                {
                    FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                    continue;
                }
                /*** Coverity Fix 63536 : End ***/
            }
            if (MAC_SS_PDU_INVALID != ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType)
            {
                longSrOrReportNode_p = (globalSrOpportunityAndPeriodicReports *)
                getNextListNode((LTE_LIST_NODE *)longSrOrReportNode_p);
                continue;    
            }
        }
        else
        {
            if ((INVALID_CCE_INDEX != cceIndex) /*&& 
                    (ulUEContext_p->pucchConfigInfo.simultaneousAckNackAndCQI)*/)
            {
                /*check  parameter simultaneousAckNackAndCQI is TRUE*/
                processPUCCHSrOrReportAndAckNack(&ueReportOrOpportunityType,
                        uLUEContextInfo_p->ulUEContext_p, ulschUciRachInfo_p,
                        /* +- SPR 17777 */
                        subFrameNum,
#ifdef TDD_CONFIG
                        ssFrameNum,
#endif
                        rbMapNum,
                        /* +- SPR 17777 */
                        /* Rel9_upgrade_CR410 */
                        /* +- SPR 17777 */
#ifdef FDD_CONFIG
                        ulUEScheduledFlag,
#endif
                        cceIndex, pdcchULOutputInfo_p, &ueCellInfo
                        /* +- SPR 17777 */
#ifdef PUCCH_RESERVE_ALWAYS
                        ,puschReserveOnlyFlagForSingleUE
#endif
                        ,tti, cellIndex);
                if (MAC_SS_PDU_INVALID != ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType)
                {
                    longSrOrReportNode_p = (globalSrOpportunityAndPeriodicReports *)
                        getNextListNode((LTE_LIST_NODE *)longSrOrReportNode_p);
                    continue;
                }
        /** SPR 12165 Fix : End **/
                /* Rel9_upgrade_CR410 */
            }
        }
        /** SPR 11207 Fix : End ***/

        if ( CHECK_BIT_SET(ueReportOrOpportunityType, SCHEDULING_REQUEST) )
        {
            if ( MAX_ULSCH_UCI_SUPPORTED ==  ulschUciRachInfo_p->numberOfInfoPresent)
            {
                ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                        __func__);
                /*Coverity 59094 Fix Start <Out of Bounds Access if not returned from here>*/
                return;
                /*Coverity 59094 Fix End <Out of Bounds Access if not returned from here>*/
            }
            UNSET_BIT_AT_POS(ulUEContext_p->reportFormatMap[subFrameNum].
                    cellIdxOfExpectedReportType,selectedCellIndex);                            
        }    
        /*
         * Otherwise simply SR or report needs to be sent
         */

        processPUCCHSrOrReportOnly(ueReportOrOpportunityType,
                uLUEContextInfo_p->ulUEContext_p, 
                ulschUciRachInfo_p, 
                subFrameNum,
                /* +- SPR 17777 */
                rbMapNum,
                /* Rel9_upgrade_CR410 */
                tti
                ,pdcchULOutputInfo_p
                ,&ueCellInfo
                ,cellIndex 
                );

        longSrOrReportNode_p = (globalSrOpportunityAndPeriodicReports *)
            getNextListNode((LTE_LIST_NODE *)longSrOrReportNode_p);
    }// end of PCell while
}
#endif
/*ca-tdd pucch code changes end*/
/*Cyclomatic Complexity changes - starts here*/
/*****************************************************************************
 * Function Name  : validateAndUpdatePucchSrAndReportsNode 
 * Inputs         : uLUEContextInfo_p - UL UE Context info
 *                  ueIndex - Ue Index of current UE
 *                  globalTick - Global TTI
 * Outputs        : NONE
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function will validate the PucchSrAndReports List Node
 *****************************************************************************/
STATIC  MacRetType validateAndUpdatePucchSrAndReportsNode(
        ULUEContextInfo   *uLUEContextInfo_p,
        UInt32 ueIndex,
        /* SPR 15909 fix start */
        tickType_t globalTick)
        /* SPR 15909 fix end */
{
    ULUEContext *ulUEContext_p = PNULL;   
    (void)ueIndex;
    if (PNULL == uLUEContextInfo_p)
    {
        lteWarning("No UL UE context created for UE Index [%d]", ueIndex);
        return MAC_FAILURE;
    }

    if (uLUEContextInfo_p->pendingDeleteFlag)
    {
        lteWarning("Pending Delete Flag is set For this UE Index [%d]",ueIndex);
        return MAC_FAILURE;
    }

    /* MEAS_GAP_CHG */
    ulUEContext_p = uLUEContextInfo_p->ulUEContext_p;
    if (PNULL == ulUEContext_p)
    {
        lteWarning("No UL UE context created for UE Index [%d]", ueIndex);
        return MAC_FAILURE;
    }
    if (SETUP == ulUEContext_p->measGapConfigType)
    {
        if (TRUE == isTTIInMeasGapPeriod(ulUEContext_p, globalTick))
        {
            return MAC_FAILURE;
        }
    }
    /* MEAS_GAP_CHG */

    /* Rel9_upgrade_CR410 */
    if(ulUEContext_p->pucchConfigInfo.srProhibitExpiryTTI >= globalTick)
    {
        lteWarning("SR prohibit timer is on for UE Index [%d] at Tick [%d]", ueIndex, globalTick);
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}

#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : validateAndUpdatePduTypeForSrShortOpportunity
 * Inputs         : pduType - current PDU type
 *                  ulschUciRachInfo_p - Output container for PUCCH reports
 *                  numInfo - index in output container
 * Outputs        : NONE
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : validate and update pduType of ulschUciRachInfo
 *****************************************************************************/
STATIC  MacRetType validateAndUpdatePduTypeForSrShortOpportunity(
        UInt8 pduType,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt16 numInfo)
{
    if(pduType == MAC_SS_PDU_UCI_HARQ)
    {
        ulschUciRachInfo_p->ulschUciInfo[numInfo].pduType = 
            MAC_SS_PDU_UCI_SR_HARQ;
    }
    else if(pduType == MAC_SS_PDU_UCI_CQI_HARQ)
    {
        ulschUciRachInfo_p->ulschUciInfo[numInfo].pduType = 
            MAC_SS_PDU_UCI_CQI_SR_HARQ;
    }
    else if (pduType == MAC_SS_PDU_UCI_CQI)
    {
        ulschUciRachInfo_p->ulschUciInfo[numInfo].pduType = 
            MAC_SS_PDU_UCI_CQI_SR;
    }
    else
    {
        /* In case of TDD Config 0, when Retx for SF 7
         * has been done in SF 0. PUCCH allocation for SF 7
         * will be done in SF 1. At that time pduType will
         * be of MAC_SS_ULSCH_* type . In that case if failure
         * needs to be returned and no short SR resources will be
         * allocated.
         */
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : prachSetFrequencyResBits
 * Inputs         : numOfFreqRes - Count to be set
 *                  freqResBitString - Bit string to be set
 * Outputs        :
 * Returns        : void
 * Description    : sets the prach freq resource bits in ulconfig pdu
 *****************************************************************************/
/* Coverity Start : 59096 */
STATIC  void prachSetFrequencyResBits(
        UInt16 numOfFreqRes,
        UInt8 * freqResBitString)
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    UInt16 iter = 0;
    UInt8 tempBitString = 0;
    for (iter = 0; iter < numOfFreqRes; iter++)
    {
        SET_BIT_AT_POS(tempBitString, iter);
    }
    *freqResBitString = tempBitString;

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}
/* Coverity End : 59096 */

/*****************************************************************************
 * Function Name  : processPUCCHSrOrReportAndAckNack
 * Inputs         : ueReportAndOpportunity,
 *                  ulUEContext_p,
 *                  ulUeScheduledInfo_p,
 *                  ulschUciRachInfo_p,
 *                  subFrameNum,
 *                  sysFrameNum,
 *                  rbMapNum,
 *                  ulTTI,
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        :
 * Description    : This func handles all resource alloc and choosing correct
 *                  PUCCH format for processing SR and/or reports along with
 *                  ACK/NACKS
 *****************************************************************************/
STATIC void processPUCCHSrOrReportAndAckNack(
        UInt8 *ueReportAndOpportunity,
        ULUEContext   *ulUEContext_p,
        UeScheduledInfo  *ulUeScheduledInfo_p,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt8 subFrameNum,
        UInt16 sysFrameNum,
        UInt32 rbMapNum,
        /* +- SPR 17777 */
        UeCellInfo *ueCellInfo_p,
        /* SPR 15909 fix start */
        tickType_t ulTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
/* Rel9_upgrade_CR410 */
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    /*
     * no need to check ack/nack type i.e whether its bundling or muxing
     * if sr => 1b (pg 51, para just before table 7.3-1 of 36.213>
     * if cqi => 2b or 2 <pg 52 of 36.213>
     * sec 7.3 of 36.213
     */
    UInt32 nPUCCH = 0;
    UInt8  pduType = 0;
    CellConfigParams* cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                     cellParams_p;
    UInt8 rbIndexSlot1 = cellParams_p->ulAvailableRBs; //Setting to max values which are not possible
    UInt8 rbIndexSlot2 = cellParams_p->ulAvailableRBs ;
    UCIFormat pucchFormat = UCI_FORMAT_INVALID;
    UInt32 ueIndex = ulUEContext_p->ueIndex;
    /* Rel9_upgrade_CR396 */
    /* TDD SB_CQI */    
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = (&dlUECtxInfoArr_g[ueIndex])->dlUEContext_p;
    /* TDD SB_CQI */    
    PucchConfig *pucchConfigInfo_p = &(ulUEContext_p->pucchConfigInfo);
    if (!ueCellInfo_p->isPCell)
    {
        pucchConfigInfo_p = &(ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]\
                ->scellpucchConfigInfo);
    }
    UInt32 ttiOffset = 0;
    UInt32 drxOffset = 0;
    UInt8 isCQIMasked = FALSE;
    UInt8 drxOnDurationTimer = 0;
    UInt32 numDRXCycleTicksExpired = 0;
    UInt32 numDRXCyclesExpired = 0;
    /* SPR 15909 fix start */
    tickType_t drxCycleExpiryTTI =0;
    /* SPR 15909 fix end */
    /* + SPS_TDD_Changes */
    UInt8  M = globalDlAssociationSet_gp[internalCellIndex][subFrameNum].count;
    SInt32 signedOffset = 0;
    UInt8  k = 0;
    UInt8  local = 0;
    UInt16  dlIndex = 0;
    /* - SPS_TDD_Changes */

    if (ulUEContext_p->ulDrxContext.cqiMask)
    {
        if (ulUEContext_p->ulDrxContext.isDRXShortCycleConfigured)
        {
            numDRXCycleTicksExpired = (ulTTI -
                    ulUEContext_p->ulDrxContext.drxShortCycleCurrentOnDurationTTI);
            numDRXCyclesExpired = numDRXCycleTicksExpired /
                ulUEContext_p->ulDrxContext.drxShortCycle;

            ulUEContext_p->ulDrxContext.drxShortCycleCurrentOnDurationTTI =
                ulUEContext_p->ulDrxContext.drxShortCycleCurrentOnDurationTTI +
                ((tickType_t)numDRXCyclesExpired * ulUEContext_p->ulDrxContext.drxShortCycle); /*CID 108840*/

            drxCycleExpiryTTI = tddConvertTimerVal(ulUEContext_p->ulDrxContext.
                    drxOnDurationTimer,ulUEContext_p->ulDrxContext.
                    drxShortCycleCurrentOnDurationTTI, internalCellIndex); 
            drxOnDurationTimer = drxCycleExpiryTTI - 
                ulUEContext_p->ulDrxContext.drxShortCycleCurrentOnDurationTTI;

            ttiOffset = ((sysFrameNum *10)+ subFrameNum) 
                % ulUEContext_p->ulDrxContext.drxShortCycle;
            drxOffset = ulUEContext_p->ulDrxContext.drxOffsetModShortCycle;
        }
        else
        {
            numDRXCycleTicksExpired = (ulTTI -
                    ulUEContext_p->ulDrxContext.drxLongCycleCurrentOnDurationTTI);
            numDRXCyclesExpired = numDRXCycleTicksExpired /
                ulUEContext_p->ulDrxContext.drxLongCycle;

            ulUEContext_p->ulDrxContext.drxLongCycleCurrentOnDurationTTI =
                ulUEContext_p->ulDrxContext.drxLongCycleCurrentOnDurationTTI +
                ((tickType_t)numDRXCyclesExpired * ulUEContext_p->ulDrxContext.drxLongCycle); /*CID 108844*/

            /* + COVERITY 5.0 */
            drxCycleExpiryTTI = tddConvertTimerVal(ulUEContext_p->ulDrxContext.
                    drxOnDurationTimer,ulUEContext_p->ulDrxContext.
                    drxLongCycleCurrentOnDurationTTI, internalCellIndex);
            /* - COVERITY 5.0 */
            drxOnDurationTimer = drxCycleExpiryTTI -
                ulUEContext_p->ulDrxContext.drxLongCycleCurrentOnDurationTTI;

            ttiOffset = ((sysFrameNum *10)+ subFrameNum) 
                % ulUEContext_p->ulDrxContext.drxLongCycle;
            drxOffset = ulUEContext_p->ulDrxContext.drxStartOffset;
        }
        if ((ttiOffset - drxOffset) >= drxOnDurationTimer)
        {
            isCQIMasked = TRUE;
        }
    }
    /* Rel9_upgrade_CR396 */

    /* How to calculate nPUCCH?? 
     * Ans (ref pg 51 & 52): 
     * 1)use SR PUCCH resource when (+ve SR) +ACK/NACK 
     2)use ACK/NACK PUCCH resource when (-ve SR)+ACK/NACK 
     *      => i.e when no SR, so not catered here.
     * Spec not clear on what resource to use 
     * when CQI/PMI or RI with ACK/NACK (pg 52 second last para)
     * update: use cqi resource (n2pucch)
     */

    //fill HARQ info here because all that is needed is 
    /*
     * a)HARQ Size 
     *  ---> When no special bundling (i.e ACK/NACK w/o report/SR)
     *      ---> For bundling, its simple - 2 bits in case tx mode 3/4 else 1 bit
     *      ---> For multiplexing,  - - no. of dl subframes scheduled in ul
     *            subframe
     *  ---> Special Bundling (Size is not in bits but expected number of ACK/NACKS)
     *  ---> ULSCH_HARQ  (No of bits is the expected number of ACK/NACKS)
     *      ---> For the above two, 
     *           this can be calculated from ulUeScheduledInfo_p->ueInsertedInPdcchDlUeInfo
     * b)Delta Offset HARQ (as done in FDD)
     * c)ACK_NACK mode (from UEContext)
     */
    /* TDD Config 0 Changes start */
    UInt16  numberOfInfoPresent = 0;
    UInt8 skipPucchResAlloc =FALSE;
    if(ULSCH_UCI_INVALID_INDEX != ulUeScheduledInfo_p->ulschUciInfoIndex)
    {
        skipPucchResAlloc = TRUE;
        numberOfInfoPresent  = ulUeScheduledInfo_p->ulschUciInfoIndex;
//#ifndef FAPI_4_0_COMPLIANCE
#if 0
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.acknackMode = ulUEContext_p->tddAckNackFeedbackMode;
#else
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.ackNackMode = ulUEContext_p->tddAckNackFeedbackMode;

#endif
        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
        /*SPR 20193 Changes Start*/ 
        if(ueDLContext_p && ueDLContext_p->scellCount)
        /*SPR 20193 Changes End*/ 
        {
            fillHARQSizeForPUSCHWithChannelSelection(ulUEContext_p->ueIndex
                    ,ulschUciRachInfo_p,subFrameNum,numberOfInfoPresent,internalCellIndex);
        }
        else
#endif
        {
	/*SPR 20564 Changes Start*/
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.harqSize =
                fillHARQSizeForMultiplexingOrBundling(ulUEContext_p->tddAckNackFeedbackMode,
                        ulUEContext_p->ueIndex,subFrameNum
                        ,internalCellIndex,ulUeScheduledInfo_p);
	/*SPR 20564 Changes End*/
        }
        /* SPR 19679: TDD HARQ Multiplexing Changes End  */

        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.deltaOffsetHARQ =
            ulUEContext_p->puschConfigInfo.betaOffsetAckIndex;

        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_ULSCH_HARQ;
        cleanUpUeScheduledInfo(ulUeScheduledInfo_p);
        /* DOUBTFULLL Case */
        /* + SPR_5323 */
        RESET_HARQ_BLER_RECEIVED_ON_PUCCH(ulUEContext_p->ueIndex,
                subFrameNum);
        /* - SPR_5323 */

    }
    else
    {
        numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_INVALID;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = ulUEContext_p->crnti;
        /* Fill basic HARQ info */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.harqSize = ulUeScheduledInfo_p->ueInsertedInPdcchDlUeInfo;
//#ifndef FAPI_4_0_COMPLIANCE
#if 0
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.acknackMode = ulUEContext_p->tddAckNackFeedbackMode;
#else
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.ackNackMode = ulUEContext_p->tddAckNackFeedbackMode;
#endif
        /* For special bundling: num of pucch resources is 0 */
        /*SPR 1115 CHG*/
        /* SPR_10033_FIX_START */
        /* Fill basic HARQ info */
        /* SPR_10033_FIX_START */

//#ifdef FAPI_4_0_COMPLIANCE
#if 1
        /*SPR 20193 Changes Start*/ 
        if(ueDLContext_p && ueDLContext_p->scellCount)
        /*SPR 20193 Changes End*/ 
        {
            fillHARQSizeForPUSCHWithChannelSelection(ulUEContext_p->ueIndex
                    ,ulschUciRachInfo_p,subFrameNum,numberOfInfoPresent,internalCellIndex);
        }
        else
#endif
        {
	/*SPR 20564 Changes Start*/ 
            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.harqSize =
                fillHARQSizeForMultiplexingOrBundling(ulUEContext_p->tddAckNackFeedbackMode,
                        ulUEContext_p->ueIndex,subFrameNum
                        ,internalCellIndex,ulUeScheduledInfo_p);
	/*SPR 20564 Changes End*/
            /* SPR 19679: TDD HARQ Multiplexing Changes End  */
        }

        /* SPR_10033_FIX_END */

        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.numOfPUCCHResource = 0;
    }
    /*
     * We need to reserve resources for 
     * reports and/or SR  with ACK/NACK
     */
    /* ICIC changes start */
    /* 
     * Possibe PDU types in this leg:
     * - UCI_SR_HARQ
     */
    if (CHECK_BIT_SET(*ueReportAndOpportunity, SCHEDULING_REQUEST) &&
            /* TDD Config 0 Changes Start */
            (!skipPucchResAlloc)
            /* TDD Config 0 Changes End */ )
    {
        /* Rel9_upgrade_CR410 */
        if(ulUEContext_p->pucchConfigInfo.srProhibitExpiryTTI < ulTTI)
        {
            /* Rel9_upgrade_CR410 */
            /* SPR 15620 fix start */
            if (INVALID_CCE_INDEX == ulUeScheduledInfo_p->ulCCEIndex)
            {/* SPR 15620 fix end */
                /* + SPS_TDD_Changes */

                /* if all downlink subframe scheduled by sps then we take resources of sps else\
                 *      * we take normal resources*/

                for (local = 0; local< M; local++)
                {
                    /*
                     *          *      * Get the smallest k
                     *                   *           */
                    k = globalDlAssociationSet_gp[internalCellIndex][subFrameNum].sortedKwithSmallM[local].k;
                    signedOffset = (subFrameNum - k);
                    signedOffset = (signedOffset)%MAX_SUB_FRAME;
                    dlIndex = (signedOffset) < 0?(signedOffset + MAX_SUB_FRAME):(signedOffset);

                    if (INVALID_CCE_INDEX != ulUeScheduledInfo_p->dlCceIndex[dlIndex])
                    {
                        if(ulUeScheduledInfo_p->dlCceIndex[dlIndex] < MAX_CCE)
                        {
                            nPUCCH = ulUEContext_p->pucchConfigInfo.srPUCCHRBIndex;
                            break;
                        }
                        else if(ueDLContext_p && ulUeScheduledInfo_p->dlCceIndex[dlIndex] < (MAX_CCE + MAX_SPS_CCE))
                        {
                            nPUCCH = ueDLContext_p->spsDlInfo.spsDlSetupInfo_p->\
                                     N1PucchAnPersistentListVal.N1PucchAnPersistent[0];
                        }
                    }
                }
                /* - SPS_TDD_Changes */
                if (MAC_FAILURE == getPRBFromPUCCHResourceIndex(nPUCCH,
                            UCI_FORMAT_ONE_B,
                            &rbIndexSlot1,
                            &rbIndexSlot2,
                            internalCellIndex) )
                {
                    /*
                     * log error and return
                     */
                    return;
                }
                allocateResourcesForPUCCHReports(rbIndexSlot1,rbIndexSlot2,rbMapNum,internalCellIndex);
                /* Fill SR info */
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                    srInfo.pucchIndex = nPUCCH;
                /* SPR 3927 Chg */
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.numOfPUCCHResource = 1;
                /* SPR 3927 Chg */
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType =
                    MAC_SS_PDU_UCI_SR_HARQ;
            }
        }
    }

    /* 
     * Possibe PDU types in this leg:
     * - UCI_CQI_SR_HARQ
     * - UCI_CQI_HARQ
     */
    if (CHECK_BIT_SET_OTHER_THAN(*ueReportAndOpportunity,SCHEDULING_REQUEST))
    {
        /* Rel9_upgrade_CR396 */
        if (!isCQIMasked || 
                /* + SPR 6864 */
                (INVALID_CCE_INDEX != ulUeScheduledInfo_p->ulCCEIndex) |
                /* - SPR 6864 */
                /* TDD Config 0 Changes Start */
                skipPucchResAlloc)
            /* TDD Config 0 Changes End */
        {
            /* Rel9_upgrade_CR396 */
            /* 
             * if normal cp =>2b
             * if extended => 2 
             * accn to pg 52 of 36.213 2nd last para
             */
            if (ulUEContext_p->pucchConfigInfo.simultaneousAckNackAndCQI  || 
                    /* + SPR 6864 */
                    (INVALID_CCE_INDEX != ulUeScheduledInfo_p->ulCCEIndex) ||
                    /* - SPR 6864 */
                    /* TDD Config 0 Changes Start */
                    skipPucchResAlloc )
                /* TDD Config 0 Changes End */
            {
                /* TDD Config 0 Changes Start */
                if(skipPucchResAlloc)
                {

                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                        pduType = MAC_SS_PDU_ULSCH_CQI_HARQ_RI;

                    storeCqiPmiRiData(
                            /*SPR 1115 CHG*/
                            ulschUciRachInfo_p, numberOfInfoPresent,
                            ulUEContext_p, *ueReportAndOpportunity, subFrameNum,
                            /* +- SPR 17777 */
#ifdef TDD_CONFIG
                            sysFrameNum,
#endif
                            ueCellInfo_p,internalCellIndex);
                    /* +- SPR 17777 */

                    if (ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            cqiRiInfo.dlCqiPmiSizeRank_1)
                    {
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            cqiRiInfo.dlCqiPmiSizeRankGT_1 =
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            cqiRiInfo.dlCqiPmiSizeRank_1;
                    }
                    else
                    {
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            cqiRiInfo.dlCqiPmiSizeRank_1 =
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            cqiRiInfo.dlCqiPmiSizeRankGT_1;
                    }



                }
                else
                {
                    /* TDD Config 0 Changes End */
                    nPUCCH = pucchConfigInfo_p->cqiPUCCHRBIndex;
                    if ( cellParams_p->cyclicPrefix == 1 )
                    {
                        /* Normal Cyclic Prefix*/
                        pucchFormat = UCI_FORMAT_TWO_B;
                    }
                    else /* cellParams_p->cyclicPrefix == 2*/
                    {
                        /*Extended Cyclic Prefix */
                        pucchFormat = UCI_FORMAT_TWO;
                    }
                    /* Klockwork warning changes start */
                    /* coverity 64945 28June2014 */
                    if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex(nPUCCH,
                                pucchFormat,
                                &rbIndexSlot1,
                                &rbIndexSlot2,
                                internalCellIndex) )
                    {
                        return;
                    }
                    /* coverity 64945 28June2014 */

                    /* Klockwork warning changes end */
                    /* SPR 16420 fix start */
                    if (INVALID_CCE_INDEX == ulUeScheduledInfo_p->ulCCEIndex)
                    {

                        allocateResourcesForPUCCHReports(rbIndexSlot1, rbIndexSlot2,
                                rbMapNum, internalCellIndex);
                    }
                    /* SPR 16420 fix start */
                    /*
                     * This is FAPI STAGE 2 psuedo code
                     * hence commented out
                     */
                    //            FILL_CQI_RI_INFO(nPUCCH);
                    if (MAC_SS_PDU_UCI_SR_HARQ == 
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType)
                    {
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            pduType = MAC_SS_PDU_UCI_CQI_SR_HARQ;
                    }
                    else
                    {
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            pduType = MAC_SS_PDU_UCI_CQI_HARQ;
                    }

                    /* + SPR 16336 */
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = ulUEContext_p->crnti;
                    /* - SPR 16336 */
                    /** SPR 13146 Fix : Start **/
                    if (ueCellInfo_p->isPCell)
                    {
                        /** SPR 14032 Fix : Start **/
                        SET_BIT_AT_POS(ulUEContext_p->reportFormatMap[sysFrameNum % 2][subFrameNum].   
                                cellIdxOfExpectedReportType, 0);
                    }
                    else
                    {
                        SET_BIT_AT_POS(ulUEContext_p->reportFormatMap[sysFrameNum % 2][subFrameNum].
                                cellIdxOfExpectedReportType, ueCellInfo_p->sCellIndex);
                        /** SPR 14032 Fix : End **/
                    }
                    /** SPR 13146 Fix : End **/
                    /* SPR 21494 End */

                    storeCqiPmiRiData(
                            /*SPR 1115 CHG*/
                            ulschUciRachInfo_p, numberOfInfoPresent,
                            ulUEContext_p, *ueReportAndOpportunity, subFrameNum,
                            /* +- SPR 17777 */
#ifdef TDD_CONFIG
                            sysFrameNum,
#endif
                            ueCellInfo_p, internalCellIndex);
                    /* +- SPR 17777 */
                }
            }
            else
            {
                /* TDD SB_CQI */
                if (ueCellInfo_p->isPCell)
                {    
                    dropCQIReport(ulUEContext_p,ueDLContext_p,TRUE,\
                            ueReportAndOpportunity,internalCellIndex);
                }
                else 
                {
                    dropCQIReport(ulUEContext_p,ueDLContext_p,FALSE,\
                            ueReportAndOpportunity,ueCellInfo_p->sCellIndex);
                }
            }
        }
        /*
         * if cqi needs to be dropped, and there was no SR, 
         * i.e basically no resources were allocated in this func,
         * then we will not cleanup ueScheduledInfo
         * and let processPUCCHAckNacksOnly() handle the ack/nack resource calc
         */
    }
    /*Check if resources were allocated here then cleanup*/
    /* SPR 12738 FIX START */
    /*SPR 16236 TDD Merge start*/
    if(rbIndexSlot1 != cellParams_p->ulAvailableRBs)
        /*SPR 16236 end*/    
    {
        /* Rel9_upgrade */
        if(ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType ==
                MAC_SS_PDU_UCI_SR_HARQ)
        {
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
            /* SPR 20193 Changes Start */
            if(ueDLContext_p && ueDLContext_p->scellCount)
            /*SPR 20193 Changes End*/
            {    
                allocateResourceForTddHarq(subFrameNum,
                        ulUEContext_p->ueIndex,
                        rbMapNum,
                        ulschUciRachInfo_p,
                        ulUeScheduledInfo_p,
                        internalCellIndex);
            } 
            else
#endif
            {    
                /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                if(HARQ_MULTIPLEXING == ulUEContext_p->tddAckNackFeedbackMode)
                {
                    pucchHandleAcksOnlyByMux(subFrameNum, 
                            ulUEContext_p->ueIndex, 
                            rbMapNum,
                            ulschUciRachInfo_p,
                            ulUeScheduledInfo_p,
                            internalCellIndex); 
                }
                else
                {
                acksOnlyByBundlingOrMequalsOne(subFrameNum, 
                        ulUEContext_p->ueIndex, 
                        rbMapNum,
                        ulschUciRachInfo_p,
                        ulUeScheduledInfo_p,
                        /*SPR 19504 fix start*/
                        FALSE,
                        /*SPR 19504 fix end*/
                        internalCellIndex); 
                }
                /* SPR 19679: TDD HARQ Multiplexing Changes End */
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = ulUEContext_p->crnti;
            }
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_UCI_SR_HARQ;
            /* SPR_10033_FIX_START */

//#ifndef FAPI_4_0_COMPLIANCE
#if 0
            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
	        /*SPR 20564 Changes Start*/
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.harqSize =
                fillHARQSizeForMultiplexingOrBundling(ulUEContext_p->tddAckNackFeedbackMode,
                        ulUEContext_p->ueIndex,
                        subFrameNum ,internalCellIndex,ulUeScheduledInfo_p);
    	    /*SPR 20564 Changes End*/
            /* SPR 19679: TDD HARQ Multiplexing Changes End */
#endif
            /* SPR_10033_FIX_END */
            /* SPR 7179 Fix Start */
            /* Resetting harq size to special bundling value */
            /* ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
               dlHarqInfo.harqSize = ulUeScheduledInfo_p->ueInsertedInPdcchDlUeInfo;*/
            /* SPR 7179 Fix End */
        }
        else
        {
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
             /*SPR 20193 Changes Start*/
        if(ueDLContext_p && ueDLContext_p->scellCount)
             /*SPR 20193 Changes End*/
            {    
                pduType = ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType;
                allocateResourceForTddHarq(subFrameNum,
                        ulUEContext_p->ueIndex,
                        rbMapNum,
                        ulschUciRachInfo_p,
                        ulUeScheduledInfo_p,
                        internalCellIndex);
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = (MACUlConfigPduInfo)pduType;
            }
            else
#endif
            {    
                pduType = ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType;
                /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                if(HARQ_MULTIPLEXING == ulUEContext_p->tddAckNackFeedbackMode)
                {
                    pucchHandleAcksOnlyByMux(subFrameNum, 
                            ulUEContext_p->ueIndex, 
                            rbMapNum,
                            ulschUciRachInfo_p,
                            ulUeScheduledInfo_p,
                            internalCellIndex); 
                }
                else
                {
                    acksOnlyByBundlingOrMequalsOne(subFrameNum, 
                            ulUEContext_p->ueIndex, 
                            rbMapNum,
                            ulschUciRachInfo_p,
                            ulUeScheduledInfo_p,
                            /*SPR 19504 fix start*/
                            FALSE,
                            /*SPR 19504 fix end*/
                            internalCellIndex); 
                }
                /* SPR 19679: TDD HARQ Multiplexing Changes End */
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = (MACUlConfigPduInfo)pduType;

                /*Set this index in ueScheduledInfo. It will be used by SRS Mgr 
                  to check collisions with PUCCH pdu types*/
                ulUeScheduledInfo_p->ulschUciInfoIndex = numberOfInfoPresent;
                /*Set the pointer in ueScheduledInfo in ulschUciRachInfo. This will
                  be used to reset the ulschUciInfoIndex during prepareULConfigReq*/
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                    ueScheduledInfo_p = ulUeScheduledInfo_p;
            }
        }
        cleanUpUeScheduledInfo(ulUeScheduledInfo_p);
        SET_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex, subFrameNum)
    }
    /*
     * UCI PDU END
     */
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}

/*****************************************************************************
 * Function Name  : processPrachReportForSpecialSubframes
 * Inputs         : ulSubFrameNum
 *                  globalTick
 *                  internalCellIndex - cell index of L2
 * Outputs        : None
 * Returns        : MAC_SUCCESSS/MAC_FAILURE
 * Description    : This function processes the PRACH reports (Preamble Format
 *                  4) that needs to be sent in S frame.
 *****************************************************************************/
void processPrachReportForSpecialSubframes( UInt8 ulSubFrameNum,
/* SPR 15909 fix start */
       tickType_t globalTick, InternalCellIndex internalCellIndex )
/* SPR 15909 fix end */
{
    RACHConfigInfo *rachConfigInfo_p = 
        &(cellSpecificParams_g.cellConfigAndInitParams_p[
                internalCellIndex]->cellParams_p->rachConfigInfo);
    UInt8 prachActiveIndex = rachConfigInfo_p->prachActiveIndex;
    UInt8 containerTick = 0;
    UInt32 prachFrequencyOffset = 0;
    UInt32 numPhysicalResources = 0;
    UInt32 numPrachFreqs = 0;
    UInt32 prachConfigIndex = (rachConfigInfo_p->prachConfiguration[
            prachActiveIndex].prachConfigIndex);
    UInt32 currSysFrameNum = (MODULO_ONEZEROTWOFOUR( globalTick /
                MAX_SUB_FRAME) );

    /* SPR 23393 Changes Start */
    UInt32 currSubFrameNum = globalTick % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    /* SPR 23393 Changes End */
    UInt32 ulSysFrameNum = 0;

    if( ulSubFrameNum < currSubFrameNum )
    {
        ulSysFrameNum = (currSysFrameNum + 1 ) % (MAX_SYS_FRAME_NUM + 1);
    }
    else
    {
        ulSysFrameNum = currSysFrameNum;
    }

    containerTick = globalTick % MAX_PDCCH_CONTAINER;

    ULSchUciRachPoolInfo* ulschUciRachInfo_p =
        ulSchUciRachContainer_gp[internalCellIndex] + containerTick;

    /* Get the Preamble format from the PRACH Config Index */
    /* +- SPR 17777 */
#ifdef FDD_CONFIG
    PreambleFormat preambleFormat = preambleFormatFrmPrachConfigIndex(
            internalCellIndex );
#else
    PreambleFormat preambleFormat = preambleFormatFrmPrachConfigIndex(
            prachConfigIndex);
#endif
    /* +- SPR 17777 */

    do
    {
        if( PREAMBLE_FORMAT_4 != preambleFormat ||
                INVALID_PREAMBLE_FORMAT == preambleFormat )
        {
            LOG_MAC_MSG( MAC_NO_ALLOC_PRACH_RES, LOGDEBUG ,MAC_PUCCH,
                    getCurrentTick(), __LINE__, prachConfigIndex,
                    preambleFormat, ulSysFrameNum, ulSubFrameNum,
                    0, 0, __func__, "");
            break;
        }
        /* Check for PRACH processing. Pass this the correct U SF */
        if ( MAC_FAILURE == isValidFrameForPRACH( ulSubFrameNum, ulSysFrameNum,
                    &prachFrequencyOffset, &numPhysicalResources, 
                    internalCellIndex) )
        {
            LOG_MAC_MSG( MAC_NO_ALLOC_PRACH_RES, LOGDEBUG ,MAC_PUCCH,
                    getCurrentTick(), __LINE__, prachConfigIndex,
                    preambleFormat, ulSysFrameNum, ulSubFrameNum,
                    0, 0, __func__, "");
            break;
        }

        LOG_MAC_MSG( MAC_ALLOC_PRACH_RES, LOGDEBUG ,MAC_PUCCH,
                getCurrentTick(), prachFrequencyOffset,
                numPhysicalResources, preambleFormat, ulSysFrameNum,
                ulSubFrameNum, 0, 0, __func__, "");

        numPrachFreqs = numPhysicalResources / NUM_PRACH_PHYSICAL_RESOURCES;
        /* Set PRACH Frequency Res bits in UL Config */
        prachSetFrequencyResBits( numPrachFreqs,
                &(ulschUciRachInfo_p->isPrachResourceReserved) );

        /* No need to allocate the PRACH resources as data is not sceduled
         * for special subframe */
    }while( 0 );
}

#else //FDD_CONFIG
/*****************************************************************************
 * Function Name  : processPUCCHSrOrReportAndAckNack
 * Inputs         : ueReportAndOpportunity_p - Bitmap indicating report types
 *                  ulUEContext_p - UL UE Context
 *                  ulschUciRachInfo_p - Output container
 *                  subFrameNum - Subframe number
 *                  sysFrameNum - System frame number
 *                  rbMapNum - rbmap number
 *                  ulUEScheduledFlag - UL UE scheduled flag
 *                  cceIndex - CCE Index for DL Allocations
 *                  pdcchULOutputInfo_p - Pointer to PdcchULOutputInfo 
 *                  ueCellInfo_p -
 *                  puschReserveOnlyFlagForSingleUE - flag for single UE
 *                  ulTTI -
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        : void
 * Description    : This func handles all resource alloc and choosing correct
 *                  PUCCH format for processing SR and/or reports along with
 *                  ACK/NACKS
 *****************************************************************************/
STATIC void processPUCCHSrOrReportAndAckNack(
        UInt8 *ueReportAndOpportunity_p,
        ULUEContext      *ulUEContext_p,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt8 subFrameNum,
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        /* +- SPR 17777 */
        UInt32 rbMapNum,
        UInt32 ulUEScheduledFlag,
        UInt32 cceIndex,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        UeCellInfo *ueCellInfo_p
#ifdef PUCCH_RESERVE_ALWAYS
        ,UInt8 puschReserveOnlyFlagForSingleUE
#endif
        /* SPR 15909 fix start */
        ,tickType_t ulTTI
        /* SPR 15909 fix end */
        ,InternalCellIndex internalCellIndex 
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    /*
     * no need to check ack/nack type i.e whether its bundling or muxing
     * if sr => 1b (pg 51, para just before table 7.3-1 of 36.213>
     * if cqi => 2b or 2 <pg 52 of 36.213>
     * sec 7.3 of 36.213
     */

    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt32 nPUCCH = 0, nPUCCH_1 = 0;
    UInt8 rbIndexSlot1 = cellParams_p->ulAvailableRBs; //Setting to max values which are not possible
    UInt8 rbIndexSlot2 = cellParams_p->ulAvailableRBs ;
    UInt8 prbIndexForDlAckNack1 = 0;
    UInt8 prbIndexForDlAckNack2 = 0;
    UInt32 ueIndex = ulUEContext_p->ueIndex;
    UInt32 activeIndex = 0;
    PucchConfig *pucchConfigInfo_p = &(ulUEContext_p->pucchConfigInfo);
    activeIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex;
#ifdef FDD_CONFIG
    if (!ueCellInfo_p->isPCell)
    {
        pucchConfigInfo_p = &(ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]\
                ->scellpucchConfigInfo);
    }
#endif

    /* Rel9_upgrade_CR396 */
    /* TDD SB_CQI */    
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = (&dlUECtxInfoArr_g[ueIndex])->dlUEContext_p;
    /* TDD SB_CQI */    

    UInt16  numberOfInfoPresent = 0;
    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;

    UInt16 reportType = 0;

    LP_SpsDlInfo spsDlInfo_p = PNULL;

    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_INVALID;

    spsDlInfo_p = &(ueDLContext_p->spsDlInfo);

    /* Calculate the nPUCCH DL ACK/NACK */
    /** SPR 11645 Fix: Start **/
    if (INVALID_CCE_INDEX != cceIndex)
    {
        if(cceIndex < MAX_CCE)
        {
            nPUCCH_1 = cceIndex + n1PucchDLAckNack_g[internalCellIndex];
        }
        else
        {
            nPUCCH_1 = spsDlInfo_p->spsDlSetupInfo_p->\
                       N1PucchAnPersistentListVal.N1PucchAnPersistent[0];
        }
    }
    /** SPR 11645 Fix: End **/
    /* +  SPR 16336 */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].\
        rnti = ulUEContext_p->crnti;
    /* -  SPR 16336 */


    /*
     * We need to reserve resources for 
     * reports and/or SR  with ACK/NACK
     */
    /* ICIC changes start */
    /* 
     * Possibe PDU types in this leg:
     * - UCI_SR_HARQ
     */
    if (CHECK_BIT_SET(*ueReportAndOpportunity_p, SCHEDULING_REQUEST))
    {
        /* Rel9_upgrade_CR410 */
        if (ulUEContext_p->pucchConfigInfo.srProhibitExpiryTTI < ulTTI)
        {
            /* Rel9_upgrade_CR410 */
            nPUCCH = ulUEContext_p->pucchConfigInfo.srPUCCHRBIndex;
            rbIndexSlot1 = ulUEContext_p->pucchConfigInfo.prbIndexSlot1\
                           [longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex]
                           [SCHEDULING_REQUEST];
            rbIndexSlot2 = ulUEContext_p->pucchConfigInfo.prbIndexSlot2\
                           [longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex]
                           [SCHEDULING_REQUEST];
            allocateResourcesForPUCCHReports(rbIndexSlot1,rbIndexSlot2,rbMapNum, internalCellIndex);
            /* Fill SR info */
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                srInfo.pucchIndex = nPUCCH;
            /* SPR 3927 Chg */
            /* SPR 3927 Chg */
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType =
                MAC_SS_PDU_UCI_SR_HARQ;
        }
    }

    /* 
     * Possibe PDU types in this leg:
     * - UCI_CQI_SR_HARQ
     * - UCI_CQI_HARQ
     */
    if (CHECK_BIT_SET_OTHER_THAN(*ueReportAndOpportunity_p,SCHEDULING_REQUEST))
    {
        /* Rel9_upgrade_CR396 */
        /* 
         * if normal cp =>2b
         * if extended => 2 
         * accn to pg 52 of 36.213 2nd last para
         */
        if (ulUEContext_p->pucchConfigInfo.simultaneousAckNackAndCQI)
        {

            /** SPR 12165 Fix : Start **/
            if ( CHECK_BIT_SET(*ueReportAndOpportunity_p, RI_REPORT) )
            {
                reportType = RI_REPORT;
            }
            else if ( CHECK_BIT_SET(*ueReportAndOpportunity_p, WIDEBAND_CQI_REPORT) )
            {
                reportType = WIDEBAND_CQI_REPORT;
            }
            else if ( CHECK_BIT_SET(*ueReportAndOpportunity_p, CQI_REPORT) )
            {
                reportType = CQI_REPORT;
            }
            /** SPR 12165 Fix : End **/

            rbIndexSlot1 = pucchConfigInfo_p->prbIndexSlot1[activeIndex][reportType];
            rbIndexSlot2 = pucchConfigInfo_p->prbIndexSlot2[activeIndex][reportType];

            /* PUCCH_RESERVE_ALWAYS Start */
#ifndef PUCCH_RESERVE_ALWAYS
            if (INVALID_CCE_INDEX == ulUEScheduledFlag)
#else
                /* SPR 6825 Changes Starts  */
                if((!puschReserveOnlyFlagForSingleUE) ||
                        (INVALID_CCE_INDEX == ulUEScheduledFlag))
                    /* SPR 6825 Changes ends   */
#endif
                    /* PUCCH_RESERVE_ALWAYS End */
                {

                    allocateResourcesForPUCCHReports(rbIndexSlot1, rbIndexSlot2,
                            rbMapNum, internalCellIndex);

#ifdef PERF_STATS
					/* + PERF_CA */
                    gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedULPerfStats.
                        totalPUCCHResAllocationAttempts++;
					/* - PERF_CA */
#endif                        
                }
            /*
             * This is FAPI STAGE 2 psuedo code
             * hence commented out
             */
            //            FILL_CQI_RI_INFO(nPUCCH);
            if (MAC_SS_PDU_UCI_SR_HARQ == 
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType)
            {
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                    pduType = MAC_SS_PDU_UCI_CQI_SR_HARQ;
            }
            else
            {
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                    pduType = MAC_SS_PDU_UCI_CQI_HARQ;
            }
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                cqiRiInfo.pucchIndex =  pucchConfigInfo_p->cqiPUCCHRBIndex;                    

            /** SPR 13146 Fix : Start **/
            if (ueCellInfo_p->isPCell)
            {   
                SET_BIT_AT_POS(ulUEContext_p->reportFormatMap[subFrameNum].
                        cellIdxOfExpectedReportType, 0);
            }
            else
            {
                SET_BIT_AT_POS(ulUEContext_p->reportFormatMap[subFrameNum].
                        cellIdxOfExpectedReportType, ueCellInfo_p->sCellIndex);
            }
            /** SPR 13146 Fix : End **/

            lteMacSsCalcHarqSize(
                    /*Coverity_31965 Fix start*/
                    &ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                    dlHarqInfo.harqSize,
                    /*Coverity_31965 Fix end*/
                    pdcchULOutputInfo_p->
                    directIndexingUEInfo[ueIndex].dlDciFormat);                
            /*FDD_SRS*/
            pdcchULOutputInfo_p->
                directIndexingUEInfo[ueIndex].ulschUciInfoIndex = 
                numberOfInfoPresent;
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ueDirectIndexInfo_p 
                = &(pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex]); 
            /*FDD_SRS*/
            /* + SPR 16336 */
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].\
                rnti = ulUEContext_p->crnti;
            /* - SPR 16336 */
            storeCqiPmiRiData(
                    /*SPR 1115 CHG*/
                    ulschUciRachInfo_p, numberOfInfoPresent,
                    ulUEContext_p, *ueReportAndOpportunity_p, subFrameNum,
                    /* +- SPR 17777 */
#ifdef TDD_CONFIG
                    sysFrameNum,
#endif
                    /* +- SPR 17777 */
                    ueCellInfo_p, 
                    internalCellIndex);
        }
        else
        {
            /** SPR 12165 Fix : Start **/
            if (ueCellInfo_p->isPCell)
            {    
                dropCQIReport(ulUEContext_p,ueDLContext_p,TRUE,\
                        ueReportAndOpportunity_p,internalCellIndex);
            }
            else 
            {
                dropCQIReport(ulUEContext_p,ueDLContext_p,FALSE,\
                        ueReportAndOpportunity_p,ueCellInfo_p->sCellIndex);
            }
            /** SPR 12165 Fix : End **/
        }
        /*
         * if cqi needs to be dropped, and there was no SR, 
         * i.e basically no resources were allocated in this func,
         * then we will not cleanup ueScheduledInfo
         * and let processPUCCHAckNacksOnly() handle the ack/nack resource calc
         */
    }

    if (  MAC_SS_PDU_INVALID != ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType )
    {
        ulschUciRachInfo_p->numberOfInfoPresent++;
        /* SPR 10345,10347 Fix : Start */
        pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].dlCCEIndex = INVALID_CCE_INDEX;
        /* SPR 10345,10347 Fix : End */
        pdcchULOutputInfo_p->directIndexingUEInfo[ulUEContext_p->ueIndex].ulschUciInfoIndex 
            = numberOfInfoPresent;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ueDirectIndexInfo_p
            = &pdcchULOutputInfo_p->directIndexingUEInfo[ulUEContext_p->ueIndex];

        /* Allocate resources for HARQ */

        /* PUCCH_RESERVE_ALWAYS Start */
        /** SPR 12492 Fix : Start**/
#ifndef PUCCH_RESERVE_ALWAYS
        if ((INVALID_CCE_INDEX == ulUEScheduledFlag) &&
                (MAC_SS_PDU_UCI_CQI_HARQ != ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType))
#else
            /* SPR 6825 Changes starts    */
            if ((!puschReserveOnlyFlagForSingleUE) &&
                    (MAC_SS_PDU_UCI_CQI_HARQ != ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType))
#endif
                /** SPR 12492 Fix : End **/
            {
                /* PUCCH_RESERVE_ALWAYS End */
                if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex (
                            nPUCCH_1, 
                            UCI_FORMAT_ONE_A, 
                            &prbIndexForDlAckNack1,
                            &prbIndexForDlAckNack2,
                            internalCellIndex
                            ))
                {
                    /* Review comment fix start GG13 */
                    LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                            getCurrentTick(),
                            ulUEContext_p->ueIndex,
                            nPUCCH_1,
                            UCI_FORMAT_ONE_A,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,
                            "Invalid Configuraton For PUCCH Resources For DL Ack-Nack");
                    return;
                    /* Review comment fix end GG13 */
                }

                allocateResourcesForPUCCHReports
                    (
                     prbIndexForDlAckNack1,
                     prbIndexForDlAckNack2,
                     rbMapNum,
                     internalCellIndex
                    );
            }


        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.deltaOffsetHARQ = 
            ulUEContext_p->puschConfigInfo.betaOffsetAckIndex;
        /** SPR 12457 Fix : Start**/
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
        fillPucchHarqInfo(ulUEContext_p->ueIndex, 
                &(ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent]), \
                nPUCCH_1, rbMapNum, 
#ifdef PUCCH_RESERVE_ALWAYS
                puschReserveOnlyFlagForSingleUE,
#endif
                /* +- SPR 17777 */
                pdcchULOutputInfo_p,subFrameNum, internalCellIndex);
        /* +- SPR 17777 */
        /** SPR 12457 Fix : End **/
#else           
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.pucchIndex = nPUCCH_1;
        lteMacSsCalcHarqSize(
                /*Coverity_31965 Fix Start*/
                &ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                dlHarqInfo.harqSize,
                /*Coverity_31965 Fix End*/
                pdcchULOutputInfo_p->
                directIndexingUEInfo[ulUEContext_p->ueIndex].dlDciFormat);                    
#endif

    }

    /* CLPC_CHG*/
    SET_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex, subFrameNum);
    /* CLPC_CHG END*/
    /*
     * UCI PDU END
     */
    /* ICIC changes end */
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}
#endif
/*ca-tdd pucch changes start*/
/*****************************************************************************
 * Function Name  : dropCQIReport 
 * Inputs         : ulUEContext_p - UL UE Context
 *                  ueDLContext_p - DL UE Context
 *                  isPCell - TRUE if PCell 
 *                            FALSE otherwise
 *                  report - bitmask for all kind of reports.
 *                  cellIndex - 
 * Outputs        :
 * Returns        : void
 * Description    : This func is used to drop the periodic CSI report & increase
 *                  its counter if it reaches certain threshold request for 
 *                  aperiodic report & reset counter.
 *****************************************************************************/
STATIC void dropCQIReport(
        ULUEContext *ulUEContext_p,
        DLUEContext *ueDLContext_p,
        UInt8 isPCell,
        /*** Coverity 62981_82_83_84_85_86 Fix : Start ****/
        /** SPR 12165 Fix : Start **/
        UInt8 *report, 
        /** SPR 12165 Fix : End **/
        /*** Coverity 62981_82_83_84_85_86 Fix : End ****/
        UInt8 cellIndex
        )
{           
    UInt16 ueIndex = ulUEContext_p->ueIndex; 
    /* SPR 15909 fix start */
    tickType_t currentTick = 0;
    /* SPR 15909 fix end */
    ULUESCellContext *ulUeScellContext_p = PNULL;
    DLUESCellContext *dlUeScellContext_p = PNULL;
    UInt8 bitMaskForAperiodicReport = 0; //Compilation purpose only


    /** SPR 12165 Fix : Start **/
    if (CHECK_BIT_SET(*report, CQI_REPORT) || 
            CHECK_BIT_SET(*report, WIDEBAND_CQI_REPORT) ||
            CHECK_BIT_SET(*report, RI_REPORT) 
       )
        /** SPR 12165 Fix : End **/
    {
        if (isPCell)
        {
            currentTick = GET_CURRENT_TICK_FOR_CELL(cellIndex);
            /* TDD SB_CQI */
            ulUEContext_p->periodicCQIDroppedCounter++;
            /* TDD SB_CQI */
            if ( (APERIODIC_CQI_REPORTING_MODE ==
                        ueDLContext_p->dlCQIInfo.cqiTypeIndicator) ||
                    (PERIODIC_APERIODIC_MODE ==
                     ueDLContext_p->dlCQIInfo.cqiTypeIndicator) )
            {
                if( (currentTick -
                            ulUEContext_p->aperiodicCqiRequestTTI
                            > APERIODIC_CQI_REQ_OFFSET) &&
                        (currentTick - ueDLContext_p->dlCQIInfo.
                         aperiodicCqiRequestTTI >
                         APERIODIC_CQI_REQ_OFFSET) &&
                        (PERIODIC_CQI_DROP_OFFSET <= ulUEContext_p->
                         periodicCQIDroppedCounter) )
                {
                    bitMaskForAperiodicReport = servingCellBitMask_g[0];
                    putEntryInULAperiodicCQIQueue(ueIndex, cellIndex, bitMaskForAperiodicReport);
                    ulUEContext_p->aperiodicCqiRequestTTI = currentTick;
                    ulUEContext_p->periodicCQIDroppedCounter = 0;
                }
            }
        }
        else
        {
            ulUeScellContext_p = ulUEContext_p->ulUeScellContext_p[cellIndex];
            dlUeScellContext_p = ueDLContext_p->dlUeScellContext_p[cellIndex];
            currentTick = GET_CURRENT_TICK_FOR_CELL(ulUeScellContext_p->internalCellIndex);
            /* TDD SB_CQI */
            ulUeScellContext_p->periodicCQIDroppedCounter++;
            /* TDD SB_CQI */
            if ( (APERIODIC_CQI_REPORTING_MODE ==
                        dlUeScellContext_p->ScelldlCQIInfo.cqiTypeIndicator) ||
                    (PERIODIC_APERIODIC_MODE ==
                     dlUeScellContext_p->ScelldlCQIInfo.cqiTypeIndicator) )
            {
                if( (currentTick - ulUeScellContext_p->aperiodicCqiRequestTTI
                            > APERIODIC_CQI_REQ_OFFSET) &&
                        (currentTick - dlUeScellContext_p->ScelldlCQIInfo.aperiodicCqiRequestTTI >
                         APERIODIC_CQI_REQ_OFFSET) && 
                        (PERIODIC_CQI_DROP_OFFSET <= ulUeScellContext_p->periodicCQIDroppedCounter) )
                {
                    bitMaskForAperiodicReport = servingCellBitMask_g[START_SCELL_INDEX];
                    /* entry to be done always in Pcell's queue */
                    putEntryInULAperiodicCQIQueue(ueIndex, ulUEContext_p->internalCellIndex,
                            bitMaskForAperiodicReport);
                    ulUeScellContext_p->aperiodicCqiRequestTTI = currentTick;
                    ulUeScellContext_p->periodicCQIDroppedCounter = 0;
                }
            } 
        }
        /*** Coverity 62987_88 Fix : Start ****/
        /** SPR 12165 Fix : Start **/
        UNSET_BIT_AT_POS(*report, CQI_REPORT);
        UNSET_BIT_AT_POS(*report, WIDEBAND_CQI_REPORT);
        UNSET_BIT_AT_POS(*report, RI_REPORT);
        /** SPR 12165 Fix : End **/
        /*** Coverity 62987_88 Fix : End ****/
        /* spr 21260 + */
        ulUEContext_p->isCqiDropped = TRUE;
        ulUEContext_p->cqiDropTick  = getCurrentTick();
        /* spr 21260 - */
    }
}
/*ca-tdd pucch code changes end*/
/* Rel9_upgrade_CR257 */
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : processPUCCHShortSr
 * Inputs         : ulUEContext_p - UL UE COntext
 *                  ulschUciRachInfo_p - Output Container
 *                  rbMapNum - rbMap number
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        : void
 * Description    : This func handles all resource alloc and choosing correct
 *                  PUCCH format for processing SR 
 *****************************************************************************/
STATIC void processPUCCHShortSr(
        ULUEContext * ulUEContext_p,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt32 rbMapNum,
        InternalCellIndex internalCellIndex)
{
    UInt32 nPUCCH = 0;
    UInt8 rbIndexSlot1 = 0;
    UInt8 rbIndexSlot2 = 0;
    UInt16  numberOfInfoPresent = 0;
    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;

    /*Fill rnti value */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti 
        = ulUEContext_p->crnti;

    nPUCCH = ulUEContext_p->pucchConfigInfo.srPUCCHRBIndex;
    if (MAC_FAILURE == getPRBFromPUCCHResourceIndex(nPUCCH,
                UCI_FORMAT_ONE,
                &rbIndexSlot1,
                &rbIndexSlot2,
                internalCellIndex) )
    {
        return;
    }
    allocateResourcesForPUCCHReports(rbIndexSlot1, rbIndexSlot2, rbMapNum, internalCellIndex);
    /* Fill SR info */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].srInfo.pucchIndex 
        = nPUCCH;
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}

/*****************************************************************************
 * Function Name  : processPUCCHShortSrAndAckNack
 * Inputs         : ulUEContext_p - UL UE COntext
 *                  ulUeScheduledInfo_p - Pointer to UeScheduledInfo
 *                  ulschUciRachInfo_p - Output Container
 *                  ulSubFrame - UL Subframe
 *                  rbMapNum - rbMap number
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        : void
 * Description    : This func handles all resource alloc and choosing correct
 *                  PUCCH format for processing SR  report along with
 *                  ACK/NACKS
 *****************************************************************************/
STATIC void processPUCCHShortSrAndAckNack(
        ULUEContext         *ulUEContext_p,
        UeScheduledInfo     *ulUeScheduledInfo_p,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt32 ulSubFrame,
        UInt32 rbMapNum,
        InternalCellIndex internalCellIndex
        )
{
    /*
     * no need to check ack/nack type i.e whether its bundling or muxing
     * if sr => 1b (pg 51, para just before table 7.3-1 of 36.213>
     */
    UInt32 nPUCCH = 0;
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
    UInt8  pduType = 0;
#endif    
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* Setting to max values which are not possible */
    UInt8 rbIndexSlot1 = cellParams_p->ulAvailableRBs;
    UInt8 rbIndexSlot2 = cellParams_p->ulAvailableRBs ;
    UInt16  numberOfInfoPresent = 0;
    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    /* + SPS_TDD_Changes */
    UInt8  M = globalDlAssociationSet_gp[internalCellIndex][ulSubFrame].count;
    UInt8  local = 0;
    UInt8  k = 0;
    SInt32 signedOffset = 0;
    UInt16  dlIndex = 0;
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = (&dlUECtxInfoArr_g[ulUEContext_p->ueIndex])->dlUEContext_p;
    /* - SPS_TDD_Changes */

    /* How to calculate nPUCCH?? 
     * Ans (ref pg 51 & 52): 
     * 1)use SR PUCCH resource when (+ve SR) +ACK/NACK 
     * 2)use ACK/NACK PUCCH resource when (-ve SR)+ACK/NACK 
     *      => i.e when no SR, so not catered here.
     */

    //fill HARQ info here because all that is needed is 
    /*
     * a)HARQ Size 
     *  ---> When no special bundling (i.e ACK/NACK w/o report/SR)
     *      ---> For bundling, its simple - 2 bits in case tx mode 3/4 else 1 bit
     *      ---> For multiplexing,  - always 2 bits
     *  ---> Special Bundling (Size is not in bits but expected number of ACK/NACKS)
     *  ---> ULSCH_HARQ  (No of bits is the expected number of ACK/NACKS)
     *      ---> For the above two, 
     *           this can be calculated from ulUeScheduledInfo_p->ueInsertedInPdcchDlUeInfo
     * b)Delta Offset HARQ (as done in FDD)
     * c)ACK_NACK mode (from UEContext)
     */

    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType 
        = MAC_SS_PDU_INVALID;

    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti 
        = ulUEContext_p->crnti;
    /* Fill basic HARQ info */

    /* SPR_10033_FIX_START */
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
    /*SPR 20193 Changes Start*/
    if(ueDLContext_p && ueDLContext_p->scellCount)
    /*SPR 20193 Changes End*/
    {
            fillHARQSizeForPUSCHWithChannelSelection(ulUEContext_p->ueIndex
                    ,ulschUciRachInfo_p,ulSubFrame,numberOfInfoPresent,internalCellIndex);

    }
    else
#endif        
    {
	    /*SPR 20564 Changes Start*/
	    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
	    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.harqSize =
		    fillHARQSizeForMultiplexingOrBundling(ulUEContext_p->tddAckNackFeedbackMode,
				    ulUEContext_p->ueIndex,ulSubFrame
				    ,internalCellIndex,ulUeScheduledInfo_p);
	    /*SPR 20564 Changes End*/
	    /* SPR 19679: TDD HARQ Multiplexing Changes End  */
    }
    /* SPR_10033_FIX_END */
//#ifndef FAPI_4_0_COMPLIANCE
#if 0
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        dlHarqInfo.acknackMode = ulUEContext_p->tddAckNackFeedbackMode;
#else
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        dlHarqInfo.ackNackMode = ulUEContext_p->tddAckNackFeedbackMode;
#endif

    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        dlHarqInfo.deltaOffsetHARQ = ulUEContext_p->puschConfigInfo.betaOffsetAckIndex;
    /*
     * UCI PDU BEGIN
     */
    /* ICIC changes start */
    /* For special bundling: num of pucch resources is 0 */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        dlHarqInfo.numOfPUCCHResource = 0;
    /*
     * We need to reserve resources for 
     * reports and/or SR  with ACK/NACK
     */
    /* 
     * Possibe PDU types in this leg:
     * - UCI_SR_HARQ
     */
     /* + SPS_TDD_Changes */
     /* if all downlink subframe scheduled by sps then we take resources of sps else\
 *       * we take normal resources*/
     for (local = 0; local< M; local++)
     {
         k = globalDlAssociationSet_gp[internalCellIndex][ulSubFrame].sortedKwithSmallM[local].k;
         signedOffset = (ulSubFrame- k);
         signedOffset = (signedOffset)%MAX_SUB_FRAME;
         dlIndex = (signedOffset) < 0?(signedOffset + MAX_SUB_FRAME):(signedOffset);

         if (INVALID_CCE_INDEX != ulUeScheduledInfo_p->dlCceIndex[dlIndex])
         {
             if(ulUeScheduledInfo_p->dlCceIndex[dlIndex] < MAX_CCE)
             {
                 nPUCCH = ulUEContext_p->pucchConfigInfo.srPUCCHRBIndex;
                 break;
             }
             else if (ueDLContext_p && ulUeScheduledInfo_p->dlCceIndex[dlIndex] < (MAX_CCE + MAX_SPS_CCE))
             {
                 nPUCCH = ueDLContext_p->spsDlInfo.spsDlSetupInfo_p->\
                          N1PucchAnPersistentListVal.N1PucchAnPersistent[0];
             }
         }
     }
     /* - SPS_TDD_Changes */
            
        if (MAC_FAILURE == getPRBFromPUCCHResourceIndex(nPUCCH,
                    UCI_FORMAT_ONE_B,
                    &rbIndexSlot1,
                    &rbIndexSlot2,
                    internalCellIndex) )
        {
            return;
        }

        /*SPR 19504 fix start*/
        if(MAC_FAILURE == allocateResourcesForPUCCHReports(rbIndexSlot1, rbIndexSlot2, rbMapNum, internalCellIndex))
        {
            LOG_MAC_WARNING(MAC_PUCCH,"Harq PUCCH Resource allocation fails\n");
            return;
        }    
        /*SPR 19504 fix end*/
    
    /* Fill SR info */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        srInfo.pucchIndex = nPUCCH;
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = 
        MAC_SS_PDU_UCI_SR_HARQ;
    /* SPR 3927 Chg */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        dlHarqInfo.numOfPUCCHResource = 1;
    /* SPR 3927 Chg */
    /*Check if resources were allocated here then cleanup*/
    if (rbIndexSlot1 != cellParams_p->ulAvailableRBs) 
    {
        if ((0 == ulUeScheduledInfo_p->ueInsertedInPdcchDlUeInfo) ||
            (ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType ==
                MAC_SS_PDU_UCI_SR_HARQ))
        {
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
            /*SPR 20193 Changes Start*/
            if(ueDLContext_p && ueDLContext_p->scellCount)
            /*SPR 20193 Changes End*/
            {     
                allocateResourceForTddHarq(ulSubFrame,
                        ulUEContext_p->ueIndex, 
                        rbMapNum,
                        ulschUciRachInfo_p,
                        ulUeScheduledInfo_p,
                        internalCellIndex);
            }
            else
#endif
            {     
                /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                if(HARQ_MULTIPLEXING == ulUEContext_p->tddAckNackFeedbackMode)
                {
                    pucchHandleAcksOnlyByMux(ulSubFrame, 
                            ulUEContext_p->ueIndex, 
                            rbMapNum,
                            ulschUciRachInfo_p,
                            ulUeScheduledInfo_p,
                            internalCellIndex); 
                }
                else
                {

                acksOnlyByBundlingOrMequalsOne(ulSubFrame, 
                        ulUEContext_p->ueIndex, 
                        rbMapNum,
                        ulschUciRachInfo_p,
                        ulUeScheduledInfo_p,
                        /*SPR 19504 fix start*/
                        FALSE,
                        /*SPR 19504 fix end*/
                        internalCellIndex); 
                }
                /* SPR 19679: TDD HARQ Multiplexing Changes End */
                /* + SPS_TDD_Changes */
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = ulUEContext_p->crnti;
                /* - SPS_TDD_Changes */

            }
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType =
                MAC_SS_PDU_UCI_SR_HARQ;
            /* SPR_10033_FIX_START */

            /* SPR 7179 Fix Start */
            /* Resetting harq size to special bundling value */
            //ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            //  dlHarqInfo.harqSize = ulUeScheduledInfo_p->ueInsertedInPdcchDlUeInfo;
            /* SPR 7179 Fix End */
//#ifndef FAPI_4_0_COMPLIANCE
#if 0
            /* SPR 19679: TDD HARQ Multiplexing Changes Start */
	    /*SPR 20564 Changes Start*/
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.harqSize =
                fillHARQSizeForMultiplexingOrBundling(ulUEContext_p->tddAckNackFeedbackMode,
                        ulUEContext_p->ueIndex,ulSubFrame
                        ,internalCellIndex,ulUeScheduledInfo_p);
	    /*SPR 20564 Changes End*/
            /* SPR 19679: TDD HARQ Multiplexing Changes End */
            /* SPR_10033_FIX_END */
#endif

        }
        else
        {
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
             /*SPR 20193 Changes Start*/
            if(ueDLContext_p && ueDLContext_p->scellCount)
             /*SPR 20193 Changes End*/
            {     
                pduType = ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType;
                allocateResourceForTddHarq(ulSubFrame,
                        ulUEContext_p->ueIndex, 
                        rbMapNum,
                        ulschUciRachInfo_p,
                        ulUeScheduledInfo_p,
                        internalCellIndex);
                /*cov-id 69712 fix*/
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = (MACUlConfigPduInfo)pduType;
            }
            else
#endif
            {    
                /*Set this index in ueScheduledInfo. It will be used by SRS Mgr 
                  to check collisions with PUCCH pdu types*/
                ulUeScheduledInfo_p->ulschUciInfoIndex = numberOfInfoPresent;
                /*Set the pointer in ueScheduledInfo in ulschUciRachInfo. This will
                  be used to reset the ulschUciInfoIndex during prepareULConfigReq*/
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                    ueScheduledInfo_p = ulUeScheduledInfo_p;
                ulschUciRachInfo_p->numberOfInfoPresent++;
            }
        }
        cleanUpUeScheduledInfo(ulUeScheduledInfo_p);
    }
    /* ICIC changes end */    
    /*
     * UCI PDU END
     */

    return;
}
#else
/*****************************************************************************
 * Function Name  : processPUCCHShortSrAndAckNack
 * Inputs         : ulUEContext_p - UL UE COntext
 *                  ulschUciRachInfo_p - Output Container
 *                  rbMapNum - rbMap number
 *                  cceIndex - allocated cce index for DL Ack/Nack
 *                  subFrameNum -
 *                  pdcchULOutputInfo_p - Pointer to PdcchULOutputInfo
 *                  puschReserveOnlyFlagForSingleUE - PUSCH reserve flag for 
 *                             single UE
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        : void
 * Description    : This func handles all resource alloc and choosing correct
 *                  PUCCH format for processing SR  report along with
 *                  ACK/NACKS
 *****************************************************************************/
STATIC void processPUCCHShortSrAndAckNack(
        ULUEContext         *ulUEContext_p,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt32 rbMapNum
        ,UInt32 cceIndex
        ,UInt32 subFrameNum        
        ,PdcchULOutputInfo *pdcchULOutputInfo_p
#ifdef PUCCH_RESERVE_ALWAYS
        ,UInt8 puschReserveOnlyFlagForSingleUE
#endif
        ,InternalCellIndex internalCellIndex 
        )
{
    /*
     * no need to check ack/nack type i.e whether its bundling or muxing
     * if sr => 1b (pg 51, para just before table 7.3-1 of 36.213>
     */
    UInt32 ueIndex = ulUEContext_p->ueIndex;
    UInt32 nPUCCH = INVALID_CCE_INDEX;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* Setting to max values which are not possible */
    UInt8 prbIndexSlot1 = cellParams_p->ulAvailableRBs;
    UInt8 prbIndexSlot2 = cellParams_p->ulAvailableRBs ;
    UInt8 prbIndexForDlAckNack1 = 0;
    UInt8 prbIndexForDlAckNack2 = 0;
    UInt16 numberOfInfoPresent = 0;
    UInt32 activeIndex = shortPeriodicSRMapShortInfo_gp[internalCellIndex]->activeIndex;
#ifndef PUCCH_RESERVE_ALWAYS
    UInt8 ulUEScheduledFlag = 0;
#endif    
    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;

    DLUEContext *ueDLContext_p =
        (&dlUECtxInfoArr_g[ulUEContext_p->ueIndex])->dlUEContext_p;


    if ( MAX_ULSCH_UCI_SUPPORTED <=  numberOfInfoPresent)
    {
        ltePanic("[%s]:1. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                __func__);
        return;
    }

    numberOfInfoPresent = pdcchULOutputInfo_p->directIndexingUEInfo[ulUEContext_p->ueIndex].ulschUciInfoIndex;

    prbIndexSlot1 = ulUEContext_p->pucchConfigInfo.prbIndexSlot1[activeIndex][SCHEDULING_REQUEST]; 
    prbIndexSlot2 = ulUEContext_p->pucchConfigInfo.prbIndexSlot2[activeIndex][SCHEDULING_REQUEST];

    /* if numberOfInfoPresent != 255 then PDUs are present */
    if ( ULSCH_UCI_INVALID_INDEX != numberOfInfoPresent )
    {
        switch (ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType)
        {
            case MAC_SS_PDU_UCI_CQI:
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_UCI_CQI_SR;
            break;

            case MAC_SS_PDU_UCI_CQI_HARQ:
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_UCI_CQI_SR_HARQ;
            break;

            default:
            break;
        }
    }
    else
    {
        numberOfInfoPresent = ulschUciRachInfo_p->numberOfInfoPresent;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_UCI_SR_HARQ;
    }

    if ( numberOfInfoPresent == ulschUciRachInfo_p->numberOfInfoPresent )
    {
        /* SPR 22413 Fix Start */
        if(INVALID_CCE_INDEX != pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].dlCCEIndex)
        {
            ulschUciRachInfo_p->numberOfInfoPresent++;
	    /* reset dlCCEIndex for the UE as it is already considered for creating
	     * SR_HARQ Pdu, and should not be considered again while creating UCI_HARQ pdu */
            pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].dlCCEIndex = INVALID_CCE_INDEX;
        }
        /* SPR 22413 Fix End */
    }

    /* Fill SR Info */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        srInfo.pucchIndex =  
        ulUEContext_p->pucchConfigInfo.srPUCCHRBIndex;

    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti 
        = ulUEContext_p->crnti;

    pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].ulschUciInfoIndex = numberOfInfoPresent;

    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ueDirectIndexInfo_p = 
        &pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex];

#ifndef PUCCH_RESERVE_ALWAYS
    ulUEScheduledFlag = pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].ulCCEIndex;
    if (INVALID_CCE_INDEX == ulUEScheduledFlag)
#else
        /* SPR 6825 Changes starts   */
        if (!puschReserveOnlyFlagForSingleUE) 
#endif
        {
            allocateResourcesForPUCCHReports(prbIndexSlot1, prbIndexSlot2, rbMapNum, internalCellIndex);
#ifdef PERF_STATS
	     /* + PERF_CA */
         gMacCellPerfStats_p[internalCellIndex]->lteCellMacSchedULPerfStats.
                totalPUCCHResAllocationAttempts++;
		 /* + PERF_CA */
#endif            
        }

    /* DL HARQ case , create SR_HARQ pdu */
    if ( INVALID_CCE_INDEX != cceIndex )
    {
        /* Calculate the nPUCCH DL ACK/NACK */
        if (cceIndex < MAX_CCE)
        {
            nPUCCH = cceIndex + n1PucchDLAckNack_g[internalCellIndex];
        }
        else
        {
            nPUCCH = ueDLContext_p->spsDlInfo.spsDlSetupInfo_p->N1PucchAnPersistentListVal.N1PucchAnPersistent[0];
        }        
    }

        /* PUCCH_RESERVE_ALWAYS Start */
#ifndef PUCCH_RESERVE_ALWAYS
    if (INVALID_CCE_INDEX == ulUEScheduledFlag)
#else
    /* SPR 6825 Changes starts    */
    if (!puschReserveOnlyFlagForSingleUE )
#endif
    {
        /* PUCCH_RESERVE_ALWAYS End */
        if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex (
                    nPUCCH, 
                    UCI_FORMAT_ONE_A, 
                    &prbIndexForDlAckNack1,
                    &prbIndexForDlAckNack2,
                    internalCellIndex
                    ))
        {
            /* Review comment fix start GG13 */
            LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                    getCurrentTick(),
                    ulUEContext_p->ueIndex,
                    nPUCCH,
                    UCI_FORMAT_ONE_A,
                    subFrameNum, /*SPR 19565 warning fix*/
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "Invalid Configuraton For PUCCH Resources For DL Ack-Nack");
            return;
            /* Review comment fix end GG13 */
        }
        allocateResourcesForPUCCHReports
            (
             prbIndexForDlAckNack1,
             prbIndexForDlAckNack2,
             rbMapNum,
             internalCellIndex
            );
    }


    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        dlHarqInfo.deltaOffsetHARQ = 
        ulUEContext_p->puschConfigInfo.betaOffsetAckIndex;
    /** SPR 12457 Fix : Start**/
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
    fillPucchHarqInfo(ulUEContext_p->ueIndex, 
            &(ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent]), \
            nPUCCH, rbMapNum, 
#ifdef PUCCH_RESERVE_ALWAYS
            puschReserveOnlyFlagForSingleUE,
#endif
            /* +- SPR 17777 */
           pdcchULOutputInfo_p, subFrameNum, internalCellIndex);
    /* +- SPR 17777 */
    /** SPR 12457 Fix : End **/

#else
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        dlHarqInfo.pucchIndex = nPUCCH;
    lteMacSsCalcHarqSize(
            /*Coverity_31965 Fix Start*/
            &ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.harqSize,
            /*Coverity_31965 Fix End*/
            pdcchULOutputInfo_p->
            directIndexingUEInfo[ulUEContext_p->ueIndex].dlDciFormat);                    
#endif

    return;
}
#endif
/* Rel9_upgrade_CR257 */

/*****************************************************************************
 * Function Name  : processPUCCHSrOrReportOnly
 * Inputs         : ueReportAndOpportunity - Report type
 *                  ulUEContext_p - UL UE COntext
 *                  ulschUciRachInfo_p - Output Container
 *                  ulschUciRachInfo_p -
 *                  subFrameNum - subframe number
 *                  sysFrameNum - system frame number
 *                  rbMapNum - rbMap number
 *                  ulTTI - global TTI 
 *                  pdcchULOutputInfo_p - 
 *                  ueCellInfo_p -
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        :
 * Description    : This func handles all resource alloc and choosing correct
 *                  PUCCH format for processing SR and/or reports without
 *                  ACK/NACKS
 *****************************************************************************/
STATIC void processPUCCHSrOrReportOnly(
        UInt8 ueReportAndOpportunity,
        ULUEContext * ulUEContext_p,
#ifdef TDD_CONFIG
        UeScheduledInfo *ulUeScheduledInfo_p,
        /* +- SPR 17777 */
        UInt16 sysFrameNum,
#endif
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt8 subFrameNum,
        /* +- SPR 17777 */
        UInt32 rbMapNum,
        /* Rel9_upgrade_CR410 */
        /* SPR 15909 fix start */
        tickType_t ulTTI
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG
        ,PdcchULOutputInfo *pdcchULOutputInfo_p
#endif
        ,UeCellInfo * ueCellInfo_p
        ,InternalCellIndex internalCellIndex
        )
/* Rel9_upgrade_CR410 */
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);

    /*
     * sec 10.1 of 36.213
     * if sr format 1 => nPucch = n1pucch,sr  <Pg 72 of 36.213>
     *
     * sec 7.2 of 36.213
     * if cqi/ri/pmi format 2 => nPucch = n2pucch <pg 42 of 36.213>
     */
    UInt32 nPUCCH = 0;
#ifdef FDD_CONFIG    
    UInt8 rbIndexSlot1 = 0;
    UInt8 rbIndexSlot2 = 0;
#else
    UInt8 rbIndexSlot1 = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs;
    UInt8 rbIndexSlot2 = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs;
#endif
    UInt16  numberOfInfoPresent = 0;
    UInt32 ueIndex = ulUEContext_p->ueIndex;
#ifdef FDD_CONFIG    
    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
#endif    
#ifdef TDD_CONFIG            
    UInt8  isCQIMasked = FALSE;
#endif    
    UInt8 reportType = 0;
    UInt32 activeIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->activeIndex;
    PucchConfig *pucchConfigInfo_p = PNULL;
    if (ueCellInfo_p->isPCell)
    {
        pucchConfigInfo_p = &(ulUEContext_p->pucchConfigInfo);
    }
    else
    {
        pucchConfigInfo_p = &(ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]->scellpucchConfigInfo);
    }

#ifdef TDD_CONFIG 
    /* Rel9_upgrade_CR396 */
    UInt32 ttiOffset = 0;
    UInt32 drxOffset = 0;
    UInt8 drxOnDurationTimer = 0;
    UInt32 numDRXCycleTicksExpired = 0;
    UInt32 numDRXCyclesExpired = 0;
    /* SPR 15909 fix start */
    tickType_t drxCycleExpiryTTI = 0;
    /* SPR 15909 fix end */

    if (ulUEContext_p->ulDrxContext.cqiMask)
    {
        if (ulUEContext_p->ulDrxContext.isDRXShortCycleConfigured)
        {
            numDRXCycleTicksExpired = (ulTTI -
                    ulUEContext_p->ulDrxContext.drxShortCycleCurrentOnDurationTTI);
            numDRXCyclesExpired = numDRXCycleTicksExpired /
                ulUEContext_p->ulDrxContext.drxShortCycle;

            ulUEContext_p->ulDrxContext.drxShortCycleCurrentOnDurationTTI =
                ulUEContext_p->ulDrxContext.drxShortCycleCurrentOnDurationTTI +
                ((tickType_t)numDRXCyclesExpired * ulUEContext_p->ulDrxContext.drxShortCycle); /*CID 108842*/

            drxCycleExpiryTTI = tddConvertTimerVal(ulUEContext_p->ulDrxContext.
                    drxOnDurationTimer,ulUEContext_p->ulDrxContext.
                    drxShortCycleCurrentOnDurationTTI, internalCellIndex); 
            drxOnDurationTimer = drxCycleExpiryTTI - 
                ulUEContext_p->ulDrxContext.drxShortCycleCurrentOnDurationTTI;

            ttiOffset = ((sysFrameNum *10)+ subFrameNum) 
                % ulUEContext_p->ulDrxContext.drxShortCycle;
            drxOffset = ulUEContext_p->ulDrxContext.drxOffsetModShortCycle;
        }
        else
        {
            numDRXCycleTicksExpired = (ulTTI -
                    ulUEContext_p->ulDrxContext.drxLongCycleCurrentOnDurationTTI);
            numDRXCyclesExpired = numDRXCycleTicksExpired /
                ulUEContext_p->ulDrxContext.drxLongCycle;

            ulUEContext_p->ulDrxContext.drxLongCycleCurrentOnDurationTTI =
                ulUEContext_p->ulDrxContext.drxLongCycleCurrentOnDurationTTI +
                ((tickType_t)numDRXCyclesExpired * ulUEContext_p->ulDrxContext.drxLongCycle); /*CID 108843*/

            drxCycleExpiryTTI = tddConvertTimerVal(ulUEContext_p->ulDrxContext.
                    drxOnDurationTimer,ulUEContext_p->ulDrxContext.
                    drxLongCycleCurrentOnDurationTTI, internalCellIndex);
            drxOnDurationTimer = drxCycleExpiryTTI - 
                ulUEContext_p->ulDrxContext.drxLongCycleCurrentOnDurationTTI;

            ttiOffset = ((sysFrameNum *10)+ subFrameNum) 
                % ulUEContext_p->ulDrxContext.drxLongCycle;
            drxOffset = ulUEContext_p->ulDrxContext.drxStartOffset;
        }
        if ((ttiOffset - drxOffset) >= drxOnDurationTimer)
        {
            isCQIMasked = TRUE;
        }
    }
    UInt8 skipPucchResAlloc =FALSE;
    if(ULSCH_UCI_INVALID_INDEX != ulUeScheduledInfo_p->ulschUciInfoIndex)
    {
        skipPucchResAlloc = TRUE;
        numberOfInfoPresent  = ulUeScheduledInfo_p->ulschUciInfoIndex;
    }
    else
    {
        /* TDD Config 0 end */
        numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
        /*Fill rnti value */    
        /* Rel9_upgrade_CR396 */

        /*Fill rnti value */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = 
            ulUEContext_p->crnti;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = 
            MAC_SS_PDU_INVALID;
        /* ICIC changes start */
    }
#else
    /* Rel9_upgrade_CR396 */

    /*Fill rnti value */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = 
        ulUEContext_p->crnti;
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = 
    MAC_SS_PDU_INVALID;
#endif
#ifdef TDD_CONFIG
    if (CHECK_BIT_SET(ueReportAndOpportunity, SCHEDULING_REQUEST) &&
       /* TDD Config 0 Changes Start */
       (!skipPucchResAlloc)
       /* TDD Config 0 Changes End */)
#else
    if (CHECK_BIT_SET(ueReportAndOpportunity, SCHEDULING_REQUEST))
#endif
    {
        /* Rel9_upgrade_CR410 : if sr prohibit timer period dont 
        process SR pdus */
        if(pucchConfigInfo_p->srProhibitExpiryTTI < ulTTI)
        {
            /* Rel9_upgrade_CR410 */
            nPUCCH = pucchConfigInfo_p->srPUCCHRBIndex;
            rbIndexSlot1 = pucchConfigInfo_p->prbIndexSlot1\
                      [activeIndex][SCHEDULING_REQUEST];
            rbIndexSlot2 = pucchConfigInfo_p->prbIndexSlot2\
                      [activeIndex][SCHEDULING_REQUEST];

        }
	    /* SPR 16420 fix start */
#ifdef TDD_CONFIG
		    if (INVALID_CCE_INDEX == ulUeScheduledInfo_p->ulCCEIndex)
		    {
#endif

        allocateResourcesForPUCCHReports(rbIndexSlot1, rbIndexSlot2,
              rbMapNum, internalCellIndex);
#ifdef TDD_CONFIG
		    }
#endif
	    /* SPR 16420 fix end */
                /* Fill SR info */
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                    srInfo.pucchIndex = nPUCCH;
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                    pduType = MAC_SS_PDU_UCI_SR;
    }
    if (CHECK_BIT_SET_OTHER_THAN(ueReportAndOpportunity, SCHEDULING_REQUEST))
    {
        /* Rel9_upgrade_CR396 */
        /* + SPR_15087 */
       /* SPR 20404 fix start*/
#ifdef TDD_CONFIG            
        {        
            /* + SPR 6864 */
            if ( (!isCQIMasked) || (INVALID_CCE_INDEX != ulUeScheduledInfo_p->ulCCEIndex) ||
                    /* SPR 20404 fix end*/
                    skipPucchResAlloc)
                /* - SPR_15087 */
            {
                /* TDD Config 0 Changes Start */
                if(skipPucchResAlloc)
                {

                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                        pduType = MAC_SS_PDU_ULSCH_CQI_RI;

                    storeCqiPmiRiData(
                            /*SPR 1115 CHG*/
                            ulschUciRachInfo_p, numberOfInfoPresent,
                            ulUEContext_p, ueReportAndOpportunity, subFrameNum,
                            /* +- SPR 17777 */
#ifdef TDD_CONFIG
                            sysFrameNum,
#endif
                            ueCellInfo_p,internalCellIndex);
                            /* +- SPR 17777 */

                    if (ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            cqiRiInfo.dlCqiPmiSizeRank_1)
                    {
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            cqiRiInfo.dlCqiPmiSizeRankGT_1 =
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            cqiRiInfo.dlCqiPmiSizeRank_1;
                    }
                    else
                    {
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            cqiRiInfo.dlCqiPmiSizeRank_1 =
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            cqiRiInfo.dlCqiPmiSizeRankGT_1;
                    }



                }
                else
#endif
                {
                    /* TDD Config 0 Changes End */
                    if ( CHECK_BIT_SET(ueReportAndOpportunity, CQI_REPORT) )
                    {
                        reportType = CQI_REPORT;
                    }
                    else if ( CHECK_BIT_SET(ueReportAndOpportunity, WIDEBAND_CQI_REPORT) )
                    {
                        reportType = WIDEBAND_CQI_REPORT;
                    }
                    else
                    {
                        reportType = RI_REPORT;
                    }

                    rbIndexSlot1 = pucchConfigInfo_p->prbIndexSlot1[activeIndex][reportType];
                    rbIndexSlot2 = pucchConfigInfo_p->prbIndexSlot2[activeIndex][reportType];
                    /* SPR 13931 fix start */
                    /* Rel9_upgrade_CR396 */
			/* SPR 16420 fix start */
#ifdef TDD_CONFIG
				if (INVALID_CCE_INDEX == ulUeScheduledInfo_p->ulCCEIndex)
				{
#endif
                    allocateResourcesForPUCCHReports(rbIndexSlot1, rbIndexSlot2, 
                            rbMapNum, internalCellIndex);
#ifdef TDD_CONFIG
				}
#endif

			/* SPR 16420 fix end */
                    /*
                     * FAPI Stage 2 psuedo code
                     * treat as comments
                     */
                    //            FILL_CQI_RI_INFO();
                    if (MAC_SS_PDU_UCI_SR == ulschUciRachInfo_p->
                            ulschUciInfo[numberOfInfoPresent].pduType)
                    {
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType
                            = MAC_SS_PDU_UCI_CQI_SR;
                    }
                    else
                    {
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType
                            = MAC_SS_PDU_UCI_CQI;
                    }
                    /* SPR 13931 fix end */
                    /* + SPR_15087 */
                    storeCqiPmiRiData(
                            /*SPR 1115 CHG*/
                            ulschUciRachInfo_p, numberOfInfoPresent,
                            ulUEContext_p, ueReportAndOpportunity, subFrameNum,
                            /* +- SPR 17777 */
#ifdef TDD_CONFIG
                            sysFrameNum, 
#endif
                            /* +- SPR 17777 */
                            ueCellInfo_p, 
                            internalCellIndex);
                    /** SPR 13146 Fix : Start **/
                    if (ueCellInfo_p->isPCell)
                    {
#ifdef FDD_CONFIG        
                        SET_BIT_AT_POS(ulUEContext_p->reportFormatMap[subFrameNum]. 
                                cellIdxOfExpectedReportType, 0);
#else
                        /* SPR 18122 Changes Start*/
                        SET_BIT_AT_POS(ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum]. 
                                cellIdxOfExpectedReportType, 0);
                        /* SPR 18122 Changes End*/

#endif        
                    }
                    else
                    {
#ifdef FDD_CONFIG
                        SET_BIT_AT_POS(ulUEContext_p->reportFormatMap[subFrameNum].
                                cellIdxOfExpectedReportType, ueCellInfo_p->sCellIndex);
#else
                        /* SPR 18122 Changes Start*/
                        SET_BIT_AT_POS(ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][subFrameNum].
                                cellIdxOfExpectedReportType, ueCellInfo_p->sCellIndex);
                        /* SPR 18122 Changes End*/

#endif
                    }
                    /** SPR 13146 Fix : End **/
                }
#ifdef TDD_CONFIG
            }
        }
#endif
        /* - SPR_15087 */
    }

#ifdef FDD_CONFIG
    /*Set this index in ueScheduledInfo. It will be used by SRS Mgr 
      to check collisions with PUCCH pdu types*/
    if (MAC_SS_PDU_INVALID != ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType)
    {
        pdcchULOutputInfo_p->directIndexingUEInfo[ulUEContext_p->ueIndex].ulschUciInfoIndex
            = numberOfInfoPresent;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ueDirectIndexInfo_p
            = &pdcchULOutputInfo_p->directIndexingUEInfo[ulUEContext_p->ueIndex];

        ulschUciRachInfo_p->numberOfInfoPresent++;
    }
    /* CLPC_CHG */
    SET_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex, subFrameNum);
    /* CLPC_CHG END */
#else
    /* TDD Config 0 Changes Start */
    if (rbIndexSlot1 != cellSpecificParams_g.\
            cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs)

    {
        /* TDD Config 0 Changes End */
        /*Set this index in ueScheduledInfo. It will be used by SRS Mgr
          to check collisions with PUCCH pdu types*/
        ulUeScheduledInfo_p->ulschUciInfoIndex = numberOfInfoPresent;
        /*Set the pointer in ueScheduledInfo in ulschUciRachInfo. This will
          be used to reset the ulschUciInfoIndex during prepareULConfigReq*/
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            ueScheduledInfo_p = ulUeScheduledInfo_p;
        ulschUciRachInfo_p->numberOfInfoPresent++;
        /* CLPC_CHG */
        /* TDD Config 0 Changes Start */
        SET_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex, subFrameNum);
        /* TDD Config 0 Changes End */
        /* CLPC_CHG END */
        /* ICIC changes end */
    }
    /* ICIC changes end */
#endif
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}

/* Rel9_upgrade_CR257 */
/*****************************************************************************
 * Function Name  : processPUCCHShortSrOnly
 * Inputs         : ulUEContext_p - UL UE COntext
 *                  ulschUciRachInfo_p - Output Container
 *                  rbMapNum - rbMap number,
 *                  pdcchULOutputInfo_p,
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        : ulschUciRachInfo_p - pointer to ULSCH UCI RACH info for UL
 *                  Config
 * Returns        : void 
 * Description    : This func handles all resource alloc and choosing correct
 *                  PUCCH format for processing SR and/or reports without
 *                  ACK/NACKS
 *****************************************************************************/
STATIC void processPUCCHShortSrOnly(
        ULUEContext * ulUEContext_p,
#ifdef TDD_CONFIG
        UeScheduledInfo *ulUeScheduledInfo_p,
#endif
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt32 rbMapNum
#ifdef FDD_CONFIG
        ,PdcchULOutputInfo *pdcchULOutputInfo_p
#endif
        ,InternalCellIndex internalCellIndex
        )
{
    UInt32 nPUCCH = 0;
    UInt8 rbIndexSlot1 = 0;
    UInt8 rbIndexSlot2 = 0;
    UInt16  numberOfInfoPresent = 0;
#ifdef FDD_CONFIG
    numberOfInfoPresent = pdcchULOutputInfo_p->directIndexingUEInfo[ulUEContext_p->ueIndex].ulschUciInfoIndex;
#else
    numberOfInfoPresent = ulUeScheduledInfo_p->ulschUciInfoIndex;
#endif

    if ( ULSCH_UCI_INVALID_INDEX == numberOfInfoPresent )
    {
        numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    }

    /* Coverity Start : 59094 */
    if (MAX_ULSCH_UCI_SUPPORTED > numberOfInfoPresent)
    {
        /*Fill rnti value */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti 
            = ulUEContext_p->crnti;

        rbIndexSlot1 = ulUEContext_p->pucchConfigInfo.prbIndexSlot1\
                       [shortPeriodicSRMapShortInfo_gp[internalCellIndex]->activeIndex][SCHEDULING_REQUEST]; 
        rbIndexSlot2 = ulUEContext_p->pucchConfigInfo.prbIndexSlot2\
                       [shortPeriodicSRMapShortInfo_gp[internalCellIndex]->activeIndex][SCHEDULING_REQUEST];

        allocateResourcesForPUCCHReports(rbIndexSlot1, rbIndexSlot2, rbMapNum, internalCellIndex);
        /* Fill SR info */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            srInfo.pucchIndex = nPUCCH;

        if ( MAC_SS_PDU_UCI_CQI == ulschUciRachInfo_p->ulschUciInfo
                [numberOfInfoPresent].pduType )
        {
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                pduType = MAC_SS_PDU_UCI_CQI_SR;
        }
        else
        {
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                pduType = MAC_SS_PDU_UCI_SR;
        }
#ifdef TDD_CONFIG
        /*Set this index in ueScheduledInfo. It will be used by SRS Mgr 
          to check collisions with PUCCH pdu types*/
        ulUeScheduledInfo_p->ulschUciInfoIndex = numberOfInfoPresent;
        /*Set the pointer in ueScheduledInfo in ulschUciRachInfo. This will
          be used to reset the ulschUciInfoIndex during prepareULConfigReq*/
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            ueScheduledInfo_p = ulUeScheduledInfo_p;
#else
        pdcchULOutputInfo_p->directIndexingUEInfo[ulUEContext_p->ueIndex].ulschUciInfoIndex 
            = numberOfInfoPresent;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].ueDirectIndexInfo_p
            = &pdcchULOutputInfo_p->directIndexingUEInfo[ulUEContext_p->ueIndex];
#endif
        if ( numberOfInfoPresent == ulschUciRachInfo_p->numberOfInfoPresent )
        {
            ulschUciRachInfo_p->numberOfInfoPresent++;
        }
    }
    /* Coverity end : 59094 */
    /* ICIC changes end */
    return;
}
/* Rel9_upgrade_CR257 */

#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : processPUCCHAckNacksOnly
 * Inputs         : ulSubFrame,
 *                  ueIndex,
 *                  rbMapNum,
 *                  ulschUciRachInfo_p,
 *                  ulUeScheduledInfo_p,
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        :
 * Description    : This func handles all resource alloc and choosing correct
 *                  PUCCH format for processing ACK/NACKS only. 
 *                  If UE had UL grant, no allocation is done
 *****************************************************************************/
STATIC  void processPUCCHAckNacksOnly(
        UInt32 ulSubFrame,
        UInt32  ueIndex,
        UInt32  rbMapNum,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UeScheduledInfo * ulUeScheduledInfo_p,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
    /*SPR 20193 Changes Start*/ 
    DLUEContext *ueDLContext_p = (&dlUECtxInfoArr_g[ueIndex])->dlUEContext_p;
    /*SPR 20193 Changes End*/ 
#endif
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
    ULUEContextInfo * uLUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
    UInt16  numberOfInfoPresent = 0;
    /* TDD Config 0 Changes start */
    if(ULSCH_UCI_INVALID_INDEX != ulUeScheduledInfo_p->ulschUciInfoIndex)
    {
        numberOfInfoPresent  = ulUeScheduledInfo_p->ulschUciInfoIndex;
//#ifndef FAPI_4_0_COMPLIANCE
#if 0
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.acknackMode = uLUEContextInfo_p->ulUEContext_p->tddAckNackFeedbackMode;
#else
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.ackNackMode = uLUEContextInfo_p->ulUEContext_p->tddAckNackFeedbackMode;

#endif

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
        /*SPR 20193 Changes Start*/ 
        if(ueDLContext_p && ueDLContext_p->scellCount)
        /*SPR 20193 Changes End*/ 
        {
            fillHARQSizeForPUSCHWithChannelSelection(uLUEContextInfo_p->ulUEContext_p->ueIndex
                    ,ulschUciRachInfo_p,ulSubFrame,numberOfInfoPresent,internalCellIndex);
        }
        else
#endif
	{
		/*SPR 20564 Changes Start*/
		ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.harqSize =
			fillHARQSizeForMultiplexingOrBundling(
					uLUEContextInfo_p->ulUEContext_p->tddAckNackFeedbackMode,
					uLUEContextInfo_p->ulUEContext_p->ueIndex,
					ulSubFrame,internalCellIndex,ulUeScheduledInfo_p);
	}
	/*SPR 20564 Changes End*/
        /* SPR 19679: TDD HARQ Multiplexing Changes End  */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.deltaOffsetHARQ =
            uLUEContextInfo_p->ulUEContext_p->puschConfigInfo.betaOffsetAckIndex;

		//ysl remove
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_ULSCH_HARQ;

        RESET_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex,ulSubFrame);


    }
    else
    {
        /*
         * No need to fill rnti , it is done inside the
         * function
         */
        numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
        /*Fill rnti value */
        //        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti =
        //            ulUEContext_p->crnti;
        /* TDD Config 0 Changes end */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType =
            MAC_SS_PDU_INVALID;
        /*
         * Call appropriate func for bundling or multiplexing
         * of ack nacks
         */
        /* cceIndex is allocated for downlink schedule UE */

//#ifdef FAPI_4_0_COMPLIANCE
#if 1
         /*SPR 20193 Changes Start*/
        if(ueDLContext_p && ueDLContext_p->scellCount)
         /*SPR 20193 Changes End*/
        {
            allocateResourceForTddHarq(
                    ulSubFrame, ueIndex, rbMapNum,
                    ulschUciRachInfo_p,
                    ulUeScheduledInfo_p,
                    internalCellIndex);
        }
        else if (HARQ_BUNDLING == uLUEContextInfo_p->ulUEContext_p->tddAckNackFeedbackMode)
#else   
        if (HARQ_BUNDLING == uLUEContextInfo_p->ulUEContext_p->tddAckNackFeedbackMode)
#endif            
         {
             acksOnlyByBundlingOrMequalsOne(ulSubFrame, 
                     ueIndex, 
                     rbMapNum,
                     ulschUciRachInfo_p,
                     ulUeScheduledInfo_p,
                     /*SPR 19504 fix start*/
                     FALSE,
                     /*SPR 19504 fix end*/
                     internalCellIndex); 
         }
         else
         {
             pucchHandleAcksOnlyByMux(ulSubFrame, ueIndex, rbMapNum, 
                     ulschUciRachInfo_p,
                     ulUeScheduledInfo_p,
                     internalCellIndex);
         }
         SET_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex, ulSubFrame);
    }
    cleanUpUeScheduledInfo(ulUeScheduledInfo_p);
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}

/*****************************************************************************
 * Function Name  : acksOnlyByBundlingOrMequalsOne
 * Inputs         : subFrameNum,
 *                  ueIndex,
 *                  rbMapNum,
 *                  ulschUciRachInfo_p,
 *                  ulUeScheduledInfo_p,
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        :
 * Description    : This func handles all resource alloc and choosing correct
 *                  PUCCH format for processing ACK/NACKS only by bundling
 *                  or multiplexing when M=1
 *****************************************************************************/

/*SPR 19504 fix start*/
 void  acksOnlyByBundlingOrMequalsOne(
        /*SPR 19504 fix end*/
        UInt32 subFrameNum,
        UInt32  ueIndex,
        UInt32  rbMapNum,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UeScheduledInfo * ulUeScheduledInfo_p,
        /*SPR 19504 fix start*/
        UInt8 puschFailureFlag,
        /*SPR 19504 fix end*/
        InternalCellIndex internalCellIndex)
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    UInt32 Np = 0;
    UInt32 Npnext = 0;
    /*CA TDD CHANGES START*/
    UInt8   M = globalDlAssociationSet_gp[internalCellIndex][subFrameNum].count;
    /*CA TDD CHANGES END*/
    UInt8   count = 0;
    UInt8   k = 0;
    UInt8   m = 0;
    UInt16   dlIndex = 0;
    /* SPR 16626 FIX */
    UInt8   cceIndex = INVALID_CCE_INDEX;
    /* SPR 16626 FIX */
    UInt32  nPUCCH = 0;
    UInt8 rbIndexSlot1 = 0;
    UInt8 rbIndexSlot2 = 0;
    SInt32 signedOffset = 0;
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
    UInt8 pcellTb = NO_TB;
    DLUEContext * dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    DLHARQProcess * dlHarqProcess_p = PNULL;
#endif
    UCIFormat pucchFormat = UCI_FORMAT_INVALID;
    UInt16  numberOfInfoPresent = 0;
    /*SPR 19504 fix start*/
    if(puschFailureFlag == FALSE)
    {    
        numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    }
    else
    {
        numberOfInfoPresent  = ulUeScheduledInfo_p->ulschUciInfoIndex;
    }    
    /*SPR 19504 fix end*/
    /* + SPS_TDD_Changes */
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = (&dlUECtxInfoArr_g[ueIndex])->dlUEContext_p;
    LP_SpsDlInfo spsDlInfo_p = &(ueDLContext_p->spsDlInfo);
    /* - SPS_TDD_Changes */
    /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
    UInt8  ackNackFeedbackMode = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->\
                                                       tddAckNackFeedbackMode;
    /* SPR 19679: TDD HARQ Multiplexing  Changes End */
    UInt8 harqSizeForMuxMOneInMIMO = 0;                                                   
    /*
     * if SISO use 1a
     * else if MIMO use 1b
     */

    /* 
     * nPUCCH? Refer sec 10.1 of 36.213 pg 69
     * nPUCCH = (M -m -1)*Np + m*Np_plus_1 + cceIndex + n1PucchDLAckNack_g;
     */

    /*
     * Calculate smallest Km
     */
    /* Below loop is required for Bundling */
    for (count = 0; count < M; count++)
    {
        /*
         * Get the smallest k
         */
        /* + SPS_TDD_Changes */
        /* if all downlink subframe scheduled by sps then we take resources of sps else\
         *   * we take normal resources*/
        /*
        */
        /*CA TDD CHANGES START*/
        k = globalDlAssociationSet_gp[internalCellIndex][subFrameNum].sortedKwithSmallM[count].k;
        /*CA TDD CHANGES END*/
        signedOffset = (subFrameNum - k);
#if 0	//ysl bugfix 0723: wrong code
        signedOffset = (signedOffset)%MAX_SUB_FRAME;
#endif
        dlIndex = (signedOffset) < 0?(signedOffset + MAX_SUB_FRAME):(signedOffset);

        /*
         * see if we had a pdcch in subframe n - k
         */
        if (INVALID_CCE_INDEX != ulUeScheduledInfo_p->dlCceIndex[dlIndex])
        {
            if (UCI_FORMAT_INVALID == pucchFormat)
            {
                if (MAC_SUCCESS == isUeSISO(ueIndex))
                {
                    pucchFormat = UCI_FORMAT_ONE_A;
                    /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
                    harqSizeForMuxMOneInMIMO = 1;
                    /* SPR 19679: TDD HARQ Multiplexing  Changes End */
                }
                else
                {
                    pucchFormat = UCI_FORMAT_ONE_B;
                    /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
                    if ((HARQ_MULTIPLEXING == ackNackFeedbackMode) && (M == 1))

                    /*SPR 21500 fix end*/
                    {    
                        harqSizeForMuxMOneInMIMO = ulUeScheduledInfo_p->\
                                                   harqSizeForMuxMOneInMIMO[dlIndex];
                    }
                    /* SPR 19679: TDD HARQ Multiplexing  Changes End */
                }

//#ifdef FAPI_4_0_COMPLIANCE      
#if 1
                if (dlUEContext_p && dlUEContext_p->scellCount)
                {  
                    if (MAC_SUCCESS == isUeSISO(ueIndex))
                    {
                        pucchFormat = UCI_FORMAT_ONE_A;
                    }
                    else
                    {
                        pucchFormat = UCI_FORMAT_ONE_B;
                    }
                } 
#endif   
            }
            /*
             *  * see end of pg 69 of 36.213,
             *   * where for M=1/bundling, UE detects PDSCH without corresponding PDCCH i.e SPS
             *    */

            if(ulUeScheduledInfo_p->dlCceIndex[dlIndex] < MAX_CCE)
            {
                cceIndex = ulUeScheduledInfo_p->dlCceIndex[dlIndex];
                m = globalDlAssociationSet_gp[internalCellIndex][subFrameNum].sortedKwithSmallM[count].smallM;
                pucchGetNpsForCCE(cceIndex, &Np, &Npnext, internalCellIndex);
                nPUCCH = (M - m -1)*Np + (m*Npnext) + cceIndex + n1PucchDLAckNack_g[internalCellIndex];
                break;
            }
            else if (ulUeScheduledInfo_p->dlCceIndex[dlIndex] < (MAX_CCE + MAX_SPS_CCE))
            {
                nPUCCH = ueDLContext_p->spsDlInfo.spsDlSetupInfo_p->\
                         N1PucchAnPersistentListVal.N1PucchAnPersistent[0];
            }
        }
    }
    if (UCI_FORMAT_INVALID != pucchFormat)
    {
//#ifdef FAPI_4_0_COMPLIANCE   
#if 1
        /* SPR 16626 FIX  */
        if (INVALID_CCE_INDEX != cceIndex)
            /* SPR 16626 FIX */
#endif            
        {
            /* SPR 12738 FIX START */
            /* - SPS_TDD_Changes */
		    /* SPR 16420 fix start */
            if (INVALID_CCE_INDEX == ulUeScheduledInfo_p->ulCCEIndex)
            {
                /* SPR 12738 FIX END */
                if(MAC_FAILURE == getPRBFromPUCCHResourceIndex(nPUCCH,pucchFormat,&rbIndexSlot1,
                            &rbIndexSlot2, internalCellIndex))
                {
                    return;    
                }
            /*-COVERITY 5.3.0 - 25302*/
            if (MAC_SS_PDU_UCI_CQI_HARQ != ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType)
                allocateResourcesForPUCCHReports(rbIndexSlot1, rbIndexSlot2, rbMapNum, internalCellIndex);
        }
		    /* SPR 16420 fix end */
		    /*-COVERITY 5.3.0 - 25302*/
	    }

//#ifdef FAPI_4_0_COMPLIANCE       
#if 1
        if (dlUEContext_p && dlUEContext_p->dlHarqContext_p)
        {     
            dlHarqProcess_p = &dlUEContext_p->dlHarqContext_p->harqProcess[ulUeScheduledInfo_p->pcellHarqProcessId];
            if (dlHarqProcess_p)
            {    
                if (dlHarqProcess_p->isTBOneValid && dlHarqProcess_p->isTBTwoValid) 
                {
                    pcellTb = TWO_TB;
                } 
                else if ((!dlHarqProcess_p->isTBOneValid && dlHarqProcess_p->isTBTwoValid) ||
                     (dlHarqProcess_p->isTBOneValid && !dlHarqProcess_p->isTBTwoValid))
                {
                    pcellTb = SINGLE_TB;
                }
            }        
        }    
#endif            

        /* Fill basic harq info */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType 
            = MAC_SS_PDU_UCI_HARQ;
        /* SPR 1684 changes start */
        /* Fill rnti value */
        /* + SPS_TDD_Changes */
        /* Fill rnti value */
        for (count = 0; count < M; count++)
        {
            k = globalDlAssociationSet_gp[internalCellIndex][subFrameNum].sortedKwithSmallM[count].k;
            signedOffset = (subFrameNum - k);
            signedOffset = (signedOffset)%MAX_SUB_FRAME;
            dlIndex = (signedOffset) < 0?(signedOffset + MAX_SUB_FRAME):(signedOffset);

            /* If schType of ueDLContext_p is SPS_SCHEDULING (this is depend upon last downink subframe of ue)
             *      * or if we find any downlink subframe scheduled by sps\
             *           * i.e isSPSCCE[dlIndex] is true then we take sps rnti else we take crnti*/

            if( (SPS_SCHEDULING == ueDLContext_p->schType) ||
                    (ulUeScheduledInfo_p->isSPSCCE[dlIndex] == LTE_TRUE))
            {
                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].\
                    rnti = spsDlInfo_p->SpsCrnti ;
                break;
            }
            else
            {

                ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                    rnti = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->crnti;
            }
        }
        /* - SPS_TDD_Changes */
        /* SPR 1684 changes start */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.harqSize = LTE_MAC_SS_TDD_CALC_HARQ_SIZE(pucchFormat); 

//#ifdef FAPI_4_0_COMPLIANCE
#if 1
        fillPucchHarqInfo(ueIndex,&(ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent]), \
                nPUCCH, rbMapNum,pcellTb,
                /* +- SPR 17777 */
                subFrameNum,internalCellIndex);
        /* +- SPR 17777 */

#else
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.harqSize = LTE_MAC_SS_TDD_CALC_HARQ_SIZE(pucchFormat);

        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.acknackMode = ulUECtxInfoArr_g[ueIndex].
            ulUEContext_p->tddAckNackFeedbackMode;
        //ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.deltaOffsetHARQ 
        //  = ulUEContext_p->puschConfigInfo.betaOffsetAckIndex;
        /* Fill extended harq info */
        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.numOfPUCCHResource = 1;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_0 = nPUCCH;
        
        /* SPR 20027 Fix Start */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_1 = HARQ_INVALID;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_2 = HARQ_INVALID;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;
        /* SPR 20027 Fix End */
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
#endif
        /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
         /*SPR 20193 Changes Start*/
        if ((ueDLContext_p)&& 
            ((!ueDLContext_p->scellCount ) && 
            (HARQ_MULTIPLEXING == ackNackFeedbackMode)
           ))
        {
         /*SPR 21500 fix end*/
           ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.harqSize = 
                                                       harqSizeForMuxMOneInMIMO;
        }    
        /* SPR 19679: TDD HARQ Multiplexing  Changes End */
        /*SPR 19504 fix start*/
        if(puschFailureFlag == FALSE)
        {
        /*Set the pointer in ueScheduledInfo in ulschUciRachInfo. This will
          be used to reset the ulschUciInfoIndex during prepareULConfigReq*/
        ulUeScheduledInfo_p->ulschUciInfoIndex = numberOfInfoPresent;
        /*Set this index in ueScheduledInfo. It will be used by SRS Mgr 
          to check collisions with PUCCH pdu types*/
            ulschUciRachInfo_p->numberOfInfoPresent++;
        }
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            ueScheduledInfo_p = ulUeScheduledInfo_p;
        /*SPR 19504 fix end*/
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"No cces allocated for ueIndex:[%d], "
                "for which ack/nck is expected in UL subframe:[%d]\n", ueIndex, subFrameNum);
    }

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}
#endif

/*****************************************************************************
 * Function Name  : isUeSISO
 * Inputs         : ueIndex - Ue Index
 * Outputs        :
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : Helper fucntion to determine if UE is SISO or MIMO
 *****************************************************************************/


 MacRetType isUeSISO(
        UInt32  ueIndex)
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    DLUEContext * dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    /*
     * Mode 1&2 are SISO
     * Mode 3&4 MIMO
     * Other modes are not supported
     */
    if (dlUEContext_p->dlMIMOInfo.transmissionMode == TX_MODE_1 || 
            dlUEContext_p->dlMIMOInfo.transmissionMode == TX_MODE_2)
    {
        LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
        return MAC_SUCCESS;
    }
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
    return MAC_FAILURE;
}
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : pucchHandleAcksOnlyByMux
 * Inputs         : ulSubFrame,
 *                  ueIndex,
 *                  rbMapNum,
 *                  ulschUciRachInfo_p,
 *                  ulUeScheduledInfo_p,
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        : 
 * Description    : This func handles all resource alloc and choosing correct
 *                  PUCCH format for processing multiplexed ACK/NACKS on PUCCH
 *****************************************************************************/

STATIC  void pucchHandleAcksOnlyByMux(
        UInt32 ulSubFrame,
        UInt32  ueIndex,
        UInt32  rbMapNum,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UeScheduledInfo * ulUeScheduledInfo_p,
        InternalCellIndex internalCellIndex)
/* + coverity 55309*/
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    /*
     * if M > 1, use 1b
     * else same as bundling
     * Update: M is fixed for that UL SF: its the max number of DL Ack/NACks which can be sent
     * on this UL. This can be found from Swati's code globalDlAssociationSet_p[ulSubFrame].count
     *  nPUCCH? Refer sec 10.1 of 36.213 pg 70 when M > 1
     * for (i=0;i<M-1;i++) 
     *      nPUCCH = (M - i - 1)*Np + i*Np_plus_1 + cceIndex_i + n1PucchDLAckNack_g;
     * where cceIndex_i is cce for pdcch in n-ki (Refer pg 70)
     */

    UInt32 Np = 0;
    UInt32 Npnext = 0;
    UInt32 cceIndexI = 0;
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    UInt32 nPUCCH[MAX_PUCCH_ACK_NACK_RESOURCES] = {HARQ_INVALID, HARQ_INVALID, 
        HARQ_INVALID, HARQ_INVALID};
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
    UInt8  k = 0;
    UInt8  i = 0;
    UInt8  m = 0;
    UInt8  dlIndex = 0;
    UInt8 rbIndexSlot1 = 0;
    UInt8 rbIndexSlot2 = 0;
    /*CA TDD CHANGES START*/
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    UInt8  M = globalDlAssociationSetForHarqMultiplexing_gp\
               [internalCellIndex][ulSubFrame].count;
    /*CA TDD CHANGES END*/
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    UInt8 resouceAllocCount = 0;
    UInt8 isSPSRntiFilled = 0;
    /* + SPS_TDD_Changes */
    DLUEContext *ueDLContext_p = PNULL;
    ueDLContext_p = (&dlUECtxInfoArr_g[ueIndex])->dlUEContext_p;
    LP_SpsDlInfo spsDlInfo_p = &(ueDLContext_p->spsDlInfo);
    UInt8 arrayIndex = 0;
    /* + SPS_TDD_Changes */
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
    SInt32 signedOffset = 0;
    UInt16  numberOfInfoPresent = 0;
    numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;

    LTE_MAC_UT_LOG(LOG_MINOR,PUCCH_MODULE,"ACK/NACKS will be sent via muxing with M: [%d]", M);
    if (1 == M)
    {
        return acksOnlyByBundlingOrMequalsOne(ulSubFrame, ueIndex, 
                rbMapNum, 
                ulschUciRachInfo_p,
                ulUeScheduledInfo_p,
                /*SPR 19504 fix start*/
                FALSE,
                /*SPR 19504 fix end*/
                internalCellIndex);
    }

    /*
     * When M >1
     */
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    for (i = M; i > 0; i-- , arrayIndex++)
    {
        /*CA TDD CHANGES START*/
        k = globalDlAssociationSetForHarqMultiplexing_gp[internalCellIndex][ulSubFrame].\
            kwithSmallM[i - 1].k;
        /*CA TDD CHANGES END */
        signedOffset = (ulSubFrame - k);
        signedOffset = signedOffset % MAX_SUB_FRAME;
        dlIndex = (signedOffset) < 0?(signedOffset + MAX_SUB_FRAME):(signedOffset);
        /*CA TDD CHANGES START*/
        m = globalDlAssociationSetForHarqMultiplexing_gp[internalCellIndex][ulSubFrame].\
            kwithSmallM[i - 1].smallM;
        /*CA TDD CHANGES END*/
        if (INVALID_CCE_INDEX != ulUeScheduledInfo_p->dlCceIndex[dlIndex])
        {
            if(ulUeScheduledInfo_p->dlCceIndex[dlIndex] < MAX_CCE)
            {
                cceIndexI = ulUeScheduledInfo_p->dlCceIndex[dlIndex];

                pucchGetNpsForCCE(cceIndexI, &Np, &Npnext, internalCellIndex);
                /*
                 * using m here instead of i in formula, 
                 * because we are reading K values in sorted order 
                 * and not as given in table 10.1-1 .
                 * Hence m is actually i.
                 */
                LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"M=[%d], m=[%d], Np=[%d], Np+1=[%d], cceIndexI");
                nPUCCH [arrayIndex] = (M - m -1)*Np + (m*Npnext) + cceIndexI + n1PucchDLAckNack_g[internalCellIndex];

            }
            else if (ulUeScheduledInfo_p->dlCceIndex[dlIndex] < (MAX_CCE + MAX_SPS_CCE))
            {
                nPUCCH[arrayIndex] = ueDLContext_p->spsDlInfo.spsDlSetupInfo_p->\
                                     N1PucchAnPersistentListVal.N1PucchAnPersistent[0];

            }
            /* SPR 19679: TDD HARQ Multiplexing Changes End */
            if (INVALID_CCE_INDEX == ulUeScheduledInfo_p->ulCCEIndex) 
            {
                if(MAC_FAILURE == getPRBFromPUCCHResourceIndex(nPUCCH[i], UCI_FORMAT_ONE_B, 
                            &rbIndexSlot1, &rbIndexSlot2, internalCellIndex))
                {
                    return;    
                }
                /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                if (MAC_SS_PDU_UCI_CQI_HARQ != ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType)
                { 
                    allocateResourcesForPUCCHReports(rbIndexSlot1, rbIndexSlot2, rbMapNum, internalCellIndex);
                }
            }

            /* Fill rnti value */
            /* If schType of ueDLContext_p is SPS_SCHEDULING 
             * (this is depend upon last downink subframe of ue)
             * or if we find any downlink subframe scheduled by sps\
             * i.e isSPSCCE[dlIndex] is true then we take sps rnti else we take crnti*/
            if (!isSPSRntiFilled)
            {    
                if( (SPS_SCHEDULING == ueDLContext_p->schType) ||
                        (ulUeScheduledInfo_p->isSPSCCE[dlIndex] == LTE_TRUE))
                {
                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].\
                        rnti = spsDlInfo_p->SpsCrnti ;
                    isSPSRntiFilled = LTE_TRUE;
                }
                else
                {

                    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                        rnti = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->crnti;
                }
            }
            resouceAllocCount++;
        }
        else
        {
            nPUCCH[arrayIndex] = 0xFFFF; 
            /* SPR 19679: TDD HARQ Multiplexing Changes End */
            LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"No cces allocated for ueIndex:[%d], "
                    "for which ack/nck is expected in UL subframe:[%d]\n", ueIndex, ulSubFrame);
        }

    }
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        dlHarqInfo.harqSize = M; 
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
//#ifndef FAPI_4_0_COMPLIANCE
#if 0
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        dlHarqInfo.acknackMode = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->tddAckNackFeedbackMode;
#else
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        dlHarqInfo.ackNackMode = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->tddAckNackFeedbackMode;
#endif

    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_UCI_HARQ;
    /*Set this index in ueScheduledInfo. It will be used by SRS Mgr 
      to check collisions with PUCCH pdu types*/
    ulUeScheduledInfo_p->ulschUciInfoIndex = numberOfInfoPresent;
    /*Set the pointer in ueScheduledInfo in ulschUciRachInfo. This will
      be used to reset the ulschUciInfoIndex during prepareULConfigReq*/
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
        ueScheduledInfo_p = ulUeScheduledInfo_p;

    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    /*SPR 20564 Changes Start*/
    /*Code Removed */
    /*SPR 20564 Changes End*/
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.numOfPUCCHResource = resouceAllocCount;
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_0 = nPUCCH[0];
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_1 = nPUCCH[1];
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_2 = nPUCCH[2];
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_3 = nPUCCH[3];
    ulschUciRachInfo_p->numberOfInfoPresent++;
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}
#endif
/* - coverity 55309 */
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : pucchGetNpsForCCE
 * Inputs         : cceIndex,
 *                  Np,
 *                  Npnext,
 *                  internalCellIndex
 * Outputs        :
 * Returns        :
 * Description    : This func calculates the correct value of Np and Np+1 as 
 *                  required by formula (ref spec 36.213: sec 10.1)
 *****************************************************************************/
STATIC  void pucchGetNpsForCCE(
        UInt8   cceIndex,
        UInt32  *Np,
        UInt32  *Npnext,
        /* CA TDD CHANGES START */
        InternalCellIndex internalCellIndex
        /* CA TDD CHANGES END */
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    UInt8   p = 0;

    for (p = 0; p < MAX_P_VAL - 1; p++)
    {
        *Np = NpTable_g[internalCellIndex][p];
        *Npnext = NpTable_g[internalCellIndex][p+1];
        if (*Np <= cceIndex && cceIndex < *Npnext)
        {
            break;
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}


/*****************************************************************************
 * Function Name  : calculateNp
 * Inputs         : NumDlRb,
 *                  NumRbSc,
 *                  P
 * Outputs        :
 * Returns        : 
 * Description    : helper function to calculate value of Np
 *****************************************************************************/

STATIC  UInt32 calculateNp(
        UInt32  NumDlRb,
        UInt32  NumRbSc,
        UInt8   p)
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    SInt32 Np = ((SInt32)(NumDlRb * (NumRbSc * p - 4) ) ) / (SInt32)36;
    Np = (Np > 0)?(UInt32)(Np):0;
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
    return (UInt32)Np;
}
#endif
/* Coverity End : 59098 */

/*****************************************************************************
 * Function Name  : cleanUpUeScheduledInfo
 * Inputs         : ulUeScheduledInfo_p
 * Outputs        :
 * Returns        : 
 * Description    : helper func to clean up uescheduled info instance
 *****************************************************************************/
#ifdef TDD_CONFIG
 void cleanUpUeScheduledInfo(
        UeScheduledInfo * ulUeScheduledInfo_p)
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    UInt8 iter = 0;

    /*Resetting values that are accessed by PUCCH*/

    ulUeScheduledInfo_p->ueInsertedInPdcchDlUeInfo = 0;
    for (iter = 0; iter < MAX_SUB_FRAME; iter++)
    {
        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
	    if (INVALID_CCE_INDEX != ulUeScheduledInfo_p->dlCceIndex[iter])
	    {   
		    ulUeScheduledInfo_p->dlCceIndexMuxPusch[iter] = ulUeScheduledInfo_p->dlCceIndex[iter];
	    } 
        /* SPR 19679: TDD HARQ Multiplexing Changes end */
        ulUeScheduledInfo_p->dlCceIndex[iter] = INVALID_CCE_INDEX;
    }
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}
#endif

/*****************************************************************************
 * Function Name  : processAdvancePUCCHAndPRACHReports
 * Inputs         : tti - Its indicate the tick for resources reserved.
 *                  ulSubFrame (TDD Only),
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        : None
 * Description    : This function process the Advance pucch reports and
 *                  set the start Index for PUSCH.
 *****************************************************************************/
void processAdvancePUCCHAndPRACHReports ( UInt32 rbMapNum,
        /*SPR 15909 fix start*/
        tickType_t tti
        /*SPR 15909 fix start*/
#ifdef TDD_CONFIG
        , UInt8 ulSubFrame
#endif
        ,InternalCellIndex internalCellIndex
        )
/* + coverity 55317 */
{
    UInt8  subFrameNum    = tti % 10;
#ifdef FDD_CONFIG
    UInt8 ulSubFrame      = subFrameNum;
#endif
    UInt32 sysFrameNum    = (MODULO_ONEZEROTWOFOUR(tti/MAX_SUB_FRAME));
    UInt32 ulSysFrameNum  = sysFrameNum;
    UInt32 numPhysicalResources = 0;
    UInt32 prachFrequencyOffset = 0;
#ifdef TDD_CONFIG
    UInt8 prachConfigIndex = 0;
    UInt32 prachActiveIndex = 0; 
#endif
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
#ifdef TDD_CONFIG
    /* SPR 11363 Fix Start */
    UInt8 ulAvailRBs = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->ulAvailableRBs;
    /* SPR 11363 Fix End */
#endif
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE, "%s called for tti: %d, rbMapNum: %d, "
            "subFrameNum: %d, sysFrameNum: %d\n", __func__, 
            tti, rbMapNum, subFrameNum, sysFrameNum);

    if(rbMapNode_p->advanceAllocFlag == TRUE)
    {
        return;
    }    
    /* In case of TDD, Uplink Retx and PRACH formats, 
     * some information like retxRb and other parameters will be overwritten. 
     * Similar is case in FDD for TTI-Bundling.
     * To avoid that one more check is added. 
     * It means rbMapint has been already done.
     */
#ifdef FDD_CONFIG
    else if ( ADVANCE_ALLOC_INIT_DONE == rbMapNode_p->advanceAllocFlag)
    {
        rbMapNode_p->advanceAllocFlag = TRUE;
    }
    else
    {
        *rbMapNode_p = (ulRAInfo_gp[internalCellIndex] + 
                ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
        rbMapNode_p->advanceAllocFlag = TRUE;
    }
    /*TDD 0-6 merge change start */
    advInitAvailCCRes[internalCellIndex][rbMapNum] =  
        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter; 
    /*TDD 0-6 merge change End */

    UInt32 rbMapNumPrevTick = ((rbMapNum + (NUM_OF_RB_MAP -1)) % NUM_OF_RB_MAP);
    RbMapNode *rbMapNodePrevTick_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNumPrevTick;
    if(FALSE == rbMapNodePrevTick_p->advanceAllocFlag)
    {
        *rbMapNodePrevTick_p =  (ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
        advInitAvailCCRes[internalCellIndex][rbMapNumPrevTick] =  
            rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter; 
    }
    setFreeRbsAdvanceAndPUCCHFlag(rbMapNum, internalCellIndex);
#elif TDD_CONFIG
     /* TDD Config 0 Changes Start
      *
      * In case of Uplink Retx some information like retxRb and
      * other parameters will be overwritten. To avoid that one more
      * check is added. It means rbMapint has been already done.
      */

     else if(TDD_ONLY_INIT_DONE == rbMapNode_p->advanceAllocFlag)
     {
         rbMapNode_p->advanceAllocFlag = TRUE;
     }
     else
     {
         *rbMapNode_p = (ulRAInfo_gp[internalCellIndex] + ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
         rbMapNode_p->advanceAllocFlag = TRUE;
     }
     /* TDD Config 0 Changes End */

     /* TDD Config 0 Changes Start */
     //    advInitAvailCCRes =
     //        rbMapNode_p->availableResourceInfo[CC_CE_REGION].freeRBCounter;
     /* TDD Config 0 Changes End */

    if (ulSubFrame < subFrameNum)
    { 
        ulSysFrameNum++; 
        if (MAX_SYS_FRAME_NUM < ulSysFrameNum) 
        {
            ulSysFrameNum = 0; 
        }
    } 

    /*SPR 2446 Fix Begins*/
    /* ICIC changes start */
    /* SPR 11363 Fix Start - Refix*/
    if ( (NUM_SIX != ulAvailRBs) ||
         !( (isPRACHResourcesAllocated( rbMapNum ,internalCellIndex)) ||
            ( MAC_SUCCESS == isValidFrameForPRACH( ulSubFrame,
                                  ulSysFrameNum, &prachFrequencyOffset,
                                  &numPhysicalResources,
                                  internalCellIndex))))

    {
        setFreeRbsAdvanceAndPUCCHFlag(rbMapNum,internalCellIndex,
                /* TDD Config 0 Changes Start */
                ulSubFrame);
    }
#endif
    /* ICIC changes end */
    /*SPR 2446 Fix Ends*/
    /*
     * Check for PRACH processing
     * Pass this the correct U frame
     * PUCCH module is called in with the D SF on which DCI 0 grant 
     * can be sent. Thus we need to get the U frame for which this 
     * D frame's DCI 0 grant is scheduled.
     */

    do
    {
#ifdef TDD_CONFIG
        RACHConfigInfo *rachConfigInfo_p = 
            &(cellSpecificParams_g.cellConfigAndInitParams_p[
                    internalCellIndex]->cellParams_p->rachConfigInfo);
        prachActiveIndex = rachConfigInfo_p->prachActiveIndex;
        prachConfigIndex = rachConfigInfo_p->prachConfiguration[
            prachActiveIndex].prachConfigIndex;
#endif

        /* Get the Preamble format from the PRACH Config Index */
        /* +- SPR 17777 */
#ifdef FDD_CONFIG
        PreambleFormat preambleFormat = preambleFormatFrmPrachConfigIndex(
                internalCellIndex );
#else
        PreambleFormat preambleFormat = preambleFormatFrmPrachConfigIndex(
                prachConfigIndex);
#endif
        /* +- SPR 17777 */

#ifdef TDD_CONFIG        
        if( PREAMBLE_FORMAT_4 == preambleFormat ||
                INVALID_PREAMBLE_FORMAT == preambleFormat )
        {
            LOG_MAC_MSG( MAC_NO_ALLOC_PRACH_RES, LOGDEBUG ,MAC_PUCCH,
                    getCurrentTick(), __LINE__, prachConfigIndex,
                    preambleFormat, ulSysFrameNum, ulSubFrame,
                    0, 0, __func__, "");
            break;
        }
#endif

        if ( MAC_FAILURE == isValidFrameForPRACH( ulSubFrame,
                    ulSysFrameNum, &prachFrequencyOffset,
                    &numPhysicalResources, internalCellIndex ) )
        {
#ifdef TDD_CONFIG
            LOG_MAC_MSG( MAC_NO_ALLOC_PRACH_RES, LOGDEBUG ,MAC_PUCCH,
                    getCurrentTick(), __LINE__, prachConfigIndex,
                    preambleFormat, ulSysFrameNum, ulSubFrame,
                    0, 0, __func__, "");
#else
            LOG_MAC_MSG( MAC_NO_ALLOC_PRACH_RES, LOGDEBUG ,MAC_PUCCH,
                    getCurrentTick(), prachFrequencyOffset, numPhysicalResources,
                    preambleFormat, ulSysFrameNum, ulSubFrame,
                    0, 0, __func__, "");
#endif
            break;
        }

        /* For Preamble format 0, UL RBs for 1 SF needs to be reserved.
           For Preamble format 1 and 2, UL RBs for 2 SFs needs to be
           reserved. For Preamble format 3, UL RBs for 3 SFs needs to be
           reserved, as derived from 36.211, Section 5.7 */

        /* Allocate the resources for first SF */
        if(!(isPRACHResourcesAllocated(rbMapNum, internalCellIndex )))
        {
            LOG_MAC_MSG( MAC_ALLOC_PRACH_RES, LOGDEBUG ,MAC_PUCCH,
                    getCurrentTick(), prachFrequencyOffset,
                    numPhysicalResources, preambleFormat,
                    ulSysFrameNum, ulSubFrame,
                    0, 0, __func__, "");

#ifdef TDD_CONFIG
            updateUplinkRbMapForPrach( prachConfigIndex, prachFrequencyOffset, 
                    rbMapNum, ulSubFrame, internalCellIndex );

            updateCEAvailRes( &rbMapNode_p->availableResourceInfo[CE_REGION],
                    prachFrequencyOffset, numPhysicalResources, 
                    internalCellIndex );
#elif FDD_CONFIG
            if( MAC_SUCCESS ==  allocateResourcesUsingRBIndex( 
                        prachFrequencyOffset, numPhysicalResources, 
                        rbMapNum, internalCellIndex ) )
            {
                updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                        prachFrequencyOffset,
                        numPhysicalResources, internalCellIndex );
            }
#endif
            /* set PRACH flag in RBMap */
            setPRACHResourcesAllocated( rbMapNum, 
#ifdef TDD_CONFIG
                    numPhysicalResources,
#endif
                    internalCellIndex );
            /* SPR 9729 fix start */
            /* Set the uciRachFlag to indicate RACH flag needs to be set
             * in UL Config. */
            if( PREAMBLE_FORMAT_0 == preambleFormat )
            {
                rbMapNode_p->uciRachFlag = TRUE;
            }
        }
        /* Check for Preamble format to allocate the resources for next
           consecutive SF */
        if( PREAMBLE_FORMAT_1 == preambleFormat ||
                PREAMBLE_FORMAT_2 == preambleFormat ||
                PREAMBLE_FORMAT_3 == preambleFormat )
        {
            /* Get the RB map for next SF */
#ifdef TDD_CONFIG
            UInt32 nextRbMapNum = (rbMapNum + 1 ) % getMaxULRBMaps( 
                    internalCellIndex );
#elif FDD_CONFIG
            UInt32 nextRbMapNum = (rbMapNum + 1) % NUM_OF_RB_MAP;
#endif
            RbMapNode *nextRbMapNode_p = 
                rbMapNodeArr_gp[internalCellIndex] + nextRbMapNum;
            /* Reset RB map info */
            if(FALSE == nextRbMapNode_p->advanceAllocFlag)
            {
                *nextRbMapNode_p =
                    (ulRAInfo_gp[internalCellIndex] + 
                    ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
#ifdef FDD_CONFIG
                nextRbMapNode_p->advanceAllocFlag = ADVANCE_ALLOC_INIT_DONE;
#else
                nextRbMapNode_p->advanceAllocFlag = TDD_ONLY_INIT_DONE;
#endif
            }
            if(!(isPRACHResourcesAllocated(nextRbMapNum, internalCellIndex )))
            {
#ifdef TDD_CONFIG
                /* Allocate the resources for first next SF */ 
                updateUplinkRbMapForPrach( prachConfigIndex, 
                        prachFrequencyOffset, nextRbMapNum, ulSubFrame,
                        internalCellIndex );

                updateCEAvailRes( 
                        &nextRbMapNode_p->availableResourceInfo[CE_REGION],
                        prachFrequencyOffset, numPhysicalResources, 
                        internalCellIndex );
#elif FDD_CONFIG
                if( MAC_SUCCESS ==  allocateResourcesUsingRBIndex( 
                            prachFrequencyOffset, numPhysicalResources, 
                            nextRbMapNum, internalCellIndex ) )
                {
                    updateCEAvailRes(&nextRbMapNode_p->availableResourceInfo[CE_REGION],
                            prachFrequencyOffset,
                            numPhysicalResources, internalCellIndex );
                }
#endif
                /* set PRACH flag in RBMap */
                setPRACHResourcesAllocated( nextRbMapNum, 
#ifdef TDD_CONFIG
                        numPhysicalResources,
#endif
                        internalCellIndex );
                /* SPR 9729 fix start */
                /* Set the uciRachFlag to indicate RACH flag needs to be set
                 * in UL Config. */
                if( PREAMBLE_FORMAT_1 == preambleFormat ||
                        PREAMBLE_FORMAT_2 == preambleFormat )
                {
                    nextRbMapNode_p->uciRachFlag = TRUE;
                }
            }
        }
        /* Check for Preamble format 3 to allocate the resources for
           SF + 2 */
        if( PREAMBLE_FORMAT_3 == preambleFormat )
        {
            /* Get the RB map for next SF */
#ifdef TDD_CONFIG
            UInt32 nextRbMapNum = (rbMapNum + 2 ) % getMaxULRBMaps( 
                    internalCellIndex );
#elif FDD_CONFIG
            UInt32 nextRbMapNum = (rbMapNum + 2) % NUM_OF_RB_MAP;
#endif
            RbMapNode *nextRbMapNode_p = 
                rbMapNodeArr_gp[internalCellIndex] + nextRbMapNum;
            /* Reset RB map info */
            if(FALSE == nextRbMapNode_p->advanceAllocFlag)
            {
                *nextRbMapNode_p =
                    (ulRAInfo_gp[internalCellIndex] + 
                    ulRAInfoIndex_g[internalCellIndex])->rbMapInitInfo;
#ifdef FDD_CONFIG
                nextRbMapNode_p->advanceAllocFlag = ADVANCE_ALLOC_INIT_DONE;
#else
                nextRbMapNode_p->advanceAllocFlag = TDD_ONLY_INIT_DONE;
#endif
            }
            if(!(isPRACHResourcesAllocated(nextRbMapNum, internalCellIndex )))
            {
                /* Allocate the resources for first next SF */ 
#ifdef TDD_CONFIG
                updateUplinkRbMapForPrach( prachConfigIndex, 
                        prachFrequencyOffset, nextRbMapNum, ulSubFrame,
                        internalCellIndex );

                updateCEAvailRes( 
                        &nextRbMapNode_p->availableResourceInfo[CE_REGION],
                        prachFrequencyOffset, numPhysicalResources, 
                        internalCellIndex );
#elif FDD_CONFIG
                if( MAC_SUCCESS ==  allocateResourcesUsingRBIndex( 
                            prachFrequencyOffset, numPhysicalResources, 
                            nextRbMapNum, internalCellIndex ) )
                {
                    updateCEAvailRes(&nextRbMapNode_p->availableResourceInfo[CE_REGION],
                            prachFrequencyOffset,
                            numPhysicalResources, internalCellIndex );
                }
#endif

                /* set PRACH flag in RBMap */
                setPRACHResourcesAllocated( nextRbMapNum, 
#ifdef TDD_CONFIG
                        numPhysicalResources,
#endif
                        internalCellIndex );
                /* SPR 9729 fix start */
                /* Set the uciRachFlag to indicate RACH flag needs to be set
                 * in UL Config. */
                nextRbMapNode_p->uciRachFlag = TRUE;
            }
        }

    } while( 0 );
}
/* - coverity 55317 */

/*****************************************************************************
 * Function Name  : initTddMacDlAssociationSet
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This fills the dl association set table 10.1-1 of 36.213
 *****************************************************************************/

#ifdef TDD_CONFIG
/* + SPR 17439 */
STATIC void initTddMacDlAssociationSet(void)
/* - SPR 17439 */    
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    /* Filling the value of tddMacDlAssociationSet and count */
    memSet(tddMacDlAssociationSet_g, INVALID_SMALL_M, sizeof(tddMacDlAssociationSet_g));
    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][2].sortedKwithSmallM[0].k = 6;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][2].sortedKwithSmallM[0].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][2].count = 1;
    /* TDD Config 0 Changes Start */
    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][3].count = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][8].count = 0;
    /* TDD Config 0 Changes End */
    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][4].sortedKwithSmallM[0].k = 4;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][4].sortedKwithSmallM[0].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][4].count = 1;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][7].sortedKwithSmallM[0].k = 6;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][7].sortedKwithSmallM[0].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][7].count = 1;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][9].sortedKwithSmallM[0].k = 4;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][9].sortedKwithSmallM[0].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_0][9].count = 1;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][2].sortedKwithSmallM[1].k = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][2].sortedKwithSmallM[0].k = 6;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][2].sortedKwithSmallM[0].smallM = 1;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][2].sortedKwithSmallM[1].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][2].count = 2;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][3].sortedKwithSmallM[0].k = 4;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][3].sortedKwithSmallM[0].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][3].count = 1;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][7].sortedKwithSmallM[1].k = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][7].sortedKwithSmallM[0].k = 6;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][7].sortedKwithSmallM[0].smallM = 1;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][7].sortedKwithSmallM[1].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][7].count = 2;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][8].sortedKwithSmallM[0].k = 4;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][8].sortedKwithSmallM[0].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_1][8].count = 1;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][2].sortedKwithSmallM[3].k = 8;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][2].sortedKwithSmallM[2].k = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][2].sortedKwithSmallM[0].k = 4;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][2].sortedKwithSmallM[1].k = 6;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][2].sortedKwithSmallM[0].smallM = 2;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][2].sortedKwithSmallM[1].smallM = 3;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][2].sortedKwithSmallM[2].smallM = 1;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][2].sortedKwithSmallM[3].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][2].count = 4;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][7].sortedKwithSmallM[3].k = 8;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][7].sortedKwithSmallM[2].k = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][7].sortedKwithSmallM[0].k = 4;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][7].sortedKwithSmallM[1].k = 6;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][7].sortedKwithSmallM[0].smallM = 2;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][7].sortedKwithSmallM[1].smallM = 3;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][7].sortedKwithSmallM[2].smallM = 1;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][7].sortedKwithSmallM[3].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_2][7].count = 4;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][2].sortedKwithSmallM[1].k = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][2].sortedKwithSmallM[0].k = 6;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][2].sortedKwithSmallM[2].k = 11;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][2].sortedKwithSmallM[0].smallM = 1;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][2].sortedKwithSmallM[1].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][2].sortedKwithSmallM[2].smallM = 2;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][2].count = 3;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][3].sortedKwithSmallM[1].k = 6;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][3].sortedKwithSmallM[0].k = 5;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][3].sortedKwithSmallM[0].smallM = 1;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][3].sortedKwithSmallM[1].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][3].count = 2;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][4].sortedKwithSmallM[1].k = 5;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][4].sortedKwithSmallM[0].k = 4;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][4].sortedKwithSmallM[0].smallM = 1;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][4].sortedKwithSmallM[1].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_3][4].count = 2;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][2].sortedKwithSmallM[3].k = 12;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][2].sortedKwithSmallM[1].k = 8;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][2].sortedKwithSmallM[0].k = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][2].sortedKwithSmallM[2].k = 11;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][2].sortedKwithSmallM[0].smallM = 2;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][2].sortedKwithSmallM[1].smallM = 1;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][2].sortedKwithSmallM[2].smallM = 3;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][2].sortedKwithSmallM[3].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][2].count = 4;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][3].sortedKwithSmallM[2].k = 6;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][3].sortedKwithSmallM[1].k = 5;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][3].sortedKwithSmallM[0].k = 4;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][3].sortedKwithSmallM[3].k = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][3].sortedKwithSmallM[0].smallM = 2;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][3].sortedKwithSmallM[1].smallM = 1;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][3].sortedKwithSmallM[2].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][3].sortedKwithSmallM[3].smallM = 3;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_4][3].count = 4;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[8].k = 13;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[7].k = 12;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[5].k = 9;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[4].k = 8;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[3].k = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[1].k = 5;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[0].k = 4;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[6].k = 11;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[2].k = 6;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[0].smallM = 6;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[1].smallM = 5;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[2].smallM = 8;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[3].smallM = 4;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[4].smallM = 3;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[5].smallM = 2;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[6].smallM = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[7].smallM = 1;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].sortedKwithSmallM[8].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_5][2].count = 9;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][2].sortedKwithSmallM[0].k = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][2].sortedKwithSmallM[0].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][2].count = 1;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][3].sortedKwithSmallM[0].k = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][3].sortedKwithSmallM[0].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][3].count = 1;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][4].sortedKwithSmallM[0].k = 5;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][4].sortedKwithSmallM[0].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][4].count = 1;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][7].sortedKwithSmallM[0].k = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][7].sortedKwithSmallM[0].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][7].count = 1;

    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][8].sortedKwithSmallM[0].k = 7;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][8].sortedKwithSmallM[0].smallM = 0;
    tddMacDlAssociationSet_g[UL_DL_CONFIG_6][8].count = 1;

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}
#endif

/* OPT */
/* OPT */

/*Begin SS 2 */


/*****************************************************************************
 * Function Name  : getUeExpectedReportType 
 * Inputs         : periodicReportMode - Periodic report Mode
 *                  report - Periodic report type
 * Outputs        : None
 * Returns        : Feedback report type
 * Description    : This function returns the feedbak report type of the report
 *                  from the input values
 *****************************************************************************/
 ueExpectedReportType getUeExpectedReportType(
        PeriodicReportingModes periodicReportMode, 
        UInt8 report
        )
{
    LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"Calculating feedBacktype for "
            "Report = %d, periodic report type = %d\n", report, 
            /* SPR 1257 changes start */
            periodicReportMode);
    /* SPR 1257 changes end */

    /*
     * README: :) if WIDEBAND_CQI_REPORT is set, CQI_REPORT will also be set
     * coz WIDEBAND_CQI_REPORT can come only wherever CQI_REPORT comes (periodicity is a multiple)
     */
    switch(periodicReportMode)
    {
        case PERIODIC_MODE_1_0:
#ifdef FDD_CONFIG
            if  ( CHECK_BIT_SET(report,CQI_REPORT) || CHECK_BIT_SET(report,WIDEBAND_CQI_REPORT)  )
            {
#endif
                LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"FeedBack type is "
                        "PERIODIC_MODE_1_0_TYPE_4");
                return PERIODIC_MODE_1_0_TYPE_4;
#ifdef FDD_CONFIG
            }
            else if ( CHECK_BIT_SET(report,RI_REPORT)   )
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"FeedBack type is "
                        "PERIODIC_MODE_1_0_TYPE_3");
                return PERIODIC_MODE_1_0_TYPE_3;
            }
#endif
            break;
        case PERIODIC_MODE_1_1:
#ifdef FDD_CONFIG
            if  ( CHECK_BIT_SET(report,CQI_REPORT)  || CHECK_BIT_SET(report,WIDEBAND_CQI_REPORT)   )
            {
#endif
                LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"FeedBack type is "
                        "PERIODIC_MODE_1_1_TYPE_2");
                return PERIODIC_MODE_1_1_TYPE_2;
#ifdef FDD_CONFIG
            }
            else if (  CHECK_BIT_SET(report,RI_REPORT) )
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"FeedBack type is "
                        "PERIODIC_MODE_1_1_TYPE_3");
                /* CA UT Fix : start*/
                return PERIODIC_MODE_1_1_TYPE_3;
                /* CA UT Fix : end*/
            }
#endif
            break;
        case PERIODIC_MODE_2_0:
#ifdef FDD_CONFIG
            if (  CHECK_BIT_SET(report,RI_REPORT) )
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"FeedBack type is "
                        "PERIODIC_MODE_2_0_TYPE_3");
                /* CA UT Fix : start*/
                return PERIODIC_MODE_2_0_TYPE_3;
                /* CA UT Fix : end*/
            }
#endif
            if (!CHECK_BIT_SET(report, WIDEBAND_CQI_REPORT) )
            { 
                LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"FeedBack type is "
                        "PERIODIC_MODE_2_0_TYPE_1");
                return PERIODIC_MODE_2_0_TYPE_1;
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"FeedBack type is "
                        "PERIODIC_MODE_2_0_TYPE_4");
                return PERIODIC_MODE_2_0_TYPE_4;
            }
            break;
        case PERIODIC_MODE_2_1:
#ifdef FDD_CONFIG
            if (  CHECK_BIT_SET(report,RI_REPORT) )
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"FeedBack type is "
                        "PERIODIC_MODE_2_1_TYPE_3");
                /* CA UT Fix : start*/
                return PERIODIC_MODE_2_1_TYPE_3;
                /* CA UT Fix : end*/
            }
#endif
            if (!CHECK_BIT_SET(report, WIDEBAND_CQI_REPORT) )
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"FeedBack type is "
                        "PERIODIC_MODE_2_1_TYPE_1");
                return PERIODIC_MODE_2_1_TYPE_1;
            }
            else
            {
                LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"FeedBack type is "
                        "PERIODIC_MODE_2_1_TYPE_2");
                return PERIODIC_MODE_2_1_TYPE_2;
            }
            break;
        default:
            break;
    }    

    LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"Failure in calculating "
            "feedbackreport returning FEEDBACK_REPORT_INVALID");
    return EXPECTED_REPORT_INVALID;
}

/*****************************************************************************
 * Function Name  : storeCqiPmiRiData 
 * Inputs         : 1. ulschUciRachInfo_p - pointer to the container node where
 *                     information for UL config is set 
 *                  2. numberOfInfoPresent - container index for 
 *                     ulschUciRachInfo_p
 *                  3. ulUEContext_p - pointer to UL UE context
 *                  4. reportType - type of report
 *                  5. subFrameNum - Subframe number
 *                  6. sysFrameNum - system frame number
 *                  7. ueCellInfo_p,
 *                  8. internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        : None 
 * Returns        : None
 * Description    : This function stores the necessary information in
 *                  1. ulschUciRachInfo_p which shall be used to encode UL
 *                  config PDUs
 *                  2. uLUEContextInfo_p which shall be rememberd in MAC and
 *                  will be used at the time of decoding raw PDUs received from
 *                  UE
 *****************************************************************************/
/*ca-tdd pucch changes*/
void storeCqiPmiRiData(
        /*SPR 1115 CHG*/
        ULSchUciRachPoolInfo* ulschUciRachInfo_p, 
        UInt16 numberOfInfoPresent, 
        ULUEContext * ulUEContext_p,
        /* +- SPR 17777 */
        UInt8 report, UInt8 subFrameNum,
#ifdef TDD_CONFIG
        UInt16 sysFrameNum,
#endif
        /* +- SPR 17777 */
        UeCellInfo * ueCellInfo_p,
        InternalCellIndex internalCellIndex
        )

{
    ueExpectedReportType ueExpectedReport = EXPECTED_REPORT_INVALID;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt8   bitWidth = 0;
    /* + CQI_5.0 */
    UInt8   rankIndicator = 0;
    /* - CQI_5.0 */
    UInt8   reportMapIndex = 0;
    PeriodicReportingModes periodicRepMode;

    if (ueCellInfo_p->isPCell)
    {
        periodicRepMode = ulUEContext_p->pucchConfigInfo.cqiPmiRiInfo.periodicRepMode;
        rankIndicator = ulUEContext_p->pucchConfigInfo.ueReportedPeriodicRI;
        /* TDD SB_CQI */
        ulUEContext_p->periodicCQIDroppedCounter = 0;
        /* TDD SB_CQI */
    }
    else
    {
        cellParams_p = cellSpecificParams_g.\
                       cellConfigAndInitParams_p[ueCellInfo_p->sCellInternalCellIndex]->cellParams_p;
        periodicRepMode = ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]\
                          ->scellpucchConfigInfo.cqiPmiRiInfo.periodicRepMode;
        rankIndicator = ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]\
                        ->scellpucchConfigInfo.ueReportedPeriodicRI;
        /* TDD SB_CQI */
        ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]\
            ->periodicCQIDroppedCounter = 0;
        /* TDD SB_CQI */
    }

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"Storing information for report %d\n",
            report);

    reportMapIndex = subFrameNum;
    /* TM8_4*2_Mimo */
    DLUEContext *ueDLContext_p = 
        (&dlUECtxInfoArr_g[ulUEContext_p->ueIndex])->dlUEContext_p;
    /* TM8_4*2_Mimo */

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"Expected report type calculated as %d\n",
            ueExpectedReport);


    if (CHECK_BIT_SET(report, RI_REPORT) )
    {
        /* Store bitwidth in UL UE context */
        /* 4x2 MIMO S */
        if ( (NUM_OF_TX_ANTENNAS_4 == cellParams_p->numOfTxAnteenas) && 
                (5 == ulUEContext_p->ueCategory) )
        {
            bitWidth = PERIODIC_MODE_ALL_TYPE_3_FOR_4_ANT_BITS;
        }
        else
        {
            bitWidth = PERIODIC_MODE_ALL_TYPE_3_BITS;
        }
        /* 4x2 MIMO E */
        /* TDD SB_CQI */
        switch (periodicRepMode)
        {
            case PERIODIC_MODE_1_0:
                ueExpectedReport = PERIODIC_MODE_1_0_TYPE_3;
                break;
            case PERIODIC_MODE_1_1:
                ueExpectedReport = PERIODIC_MODE_1_1_TYPE_3;
                break;
            case PERIODIC_MODE_2_0:
                ueExpectedReport = PERIODIC_MODE_2_0_TYPE_3;
                break;
            case PERIODIC_MODE_2_1:
                ueExpectedReport = PERIODIC_MODE_2_1_TYPE_3;
                break;
            default:
                ueExpectedReport = EXPECTED_REPORT_INVALID;
                break;

        }

        /* TDD SB_CQI */
        /*SPR 5620 START*/
        /* SPR 18122 Changes Start*/
        /*ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][reportMapIndex].
          bitWidth = bitWidth; */
          /* SPR 18122 Changes End*/
        /*SPR 5620 END*/
        LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"Bitwidth calculated as %d\n",
                bitWidth);
        /* Removed SPR_10087_FIX as earlier fix of 7414/7415 is correct*/
        /* Periodic RI expected on ULSCH:
         * DL CQI/PMI Size Rank = 1 and DL CQI/PMI Size Rank>1  will be
         * filled with same values corresponding to the size of the expected RI
         * report. RI size field should be set to 0. MAC shall expect RI in
         * PDU buffer in RX_CQI indication. (This change is valid from Rel 5.2.2
         * and onwards
         */
/*SPR 20289 fix start*/
/*SPR 20289 fix end*/
        /* SPR 7414/7415 changes start */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
            riSize = 0;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
            dlCqiPmiSizeRank_1 = bitWidth;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
            dlCqiPmiSizeRankGT_1 = bitWidth;
        /* SPR 7414/7415 changes end */

    }
    else
    {

        /* get the expected report type */
        ueExpectedReport = getUeExpectedReportType
            /* + coverity 32570 */
            (periodicRepMode, report);


        /* - coverity 32570 */

        /* Store bitwidth in UL UE context */
        /* 4x2 MIMO S */
        if (ueCellInfo_p->isPCell)
        {
            bitWidth = calculateReportFormatBitWidthPeriodic(ueExpectedReport, 
                    rankIndicator,ulUEContext_p->ueCategory,
                    /* TM8_4*2_Mimo */
                    ueDLContext_p->dlMIMOInfo.transmissionMode, internalCellIndex);
        }
        else
        {
            bitWidth = calculateReportFormatBitWidthPeriodic(ueExpectedReport, 
                    rankIndicator,ulUEContext_p->ueCategory,
                    /* TM8_4*2_Mimo */
                    ueDLContext_p->dlUeScellContext_p[ueCellInfo_p->sCellIndex]->\
                    ScelldlMIMOInfo.transmissionMode, ueCellInfo_p->sCellInternalCellIndex);
        }
        /* TM8_4*2_Mimo */
        /* 4x2 MIMO E */
        /* BUG FOUND */
        /*SPR 5620 START*/
        /* SPR 18122 Changes Start*/
        /* ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][reportMapIndex].
           bitWidth = bitWidth; */
           /* SPR 18122 Changes End*/
        /*SPR 5620 END*/
        /* BUG FOUND */
        LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"Bitwidth calculated as %d\n",
                bitWidth);

        if (1 == rankIndicator)
        {
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
                dlCqiPmiSizeRank_1 = bitWidth;
            /* TDD SB_CQI */
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
                dlCqiPmiSizeRankGT_1 = 0;
            /* TDD SB_CQI */
        }
        else
        {
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
                dlCqiPmiSizeRankGT_1 = bitWidth;
            /* TDD SB_CQI */
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
                dlCqiPmiSizeRank_1 = 0;
            /* TDD SB_CQI */
        }

        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
            riSize = 0; 
    }
    /* Store feedbacktype in UL UE context */
    /*SPR 5620 START*/
    if (ueCellInfo_p->isPCell)
    {
#ifdef FDD_CONFIG
        ulUEContext_p->reportFormatMap[reportMapIndex].
            ueExpectedReport = ueExpectedReport;
#else
            /* SPR 18122 Changes Start*/
        ulUEContext_p->reportFormatMap[MODULO(sysFrameNum,2)][reportMapIndex].
            ueExpectedReport = ueExpectedReport;
            /* SPR 18122 Changes End*/
#endif

    }
    else
    {
        /** SPR 14032 Fix : Start **/
#ifdef FDD_CONFIG
        ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]->\
            cqiPmiRiReportFormatMapSCell[reportMapIndex].
            ueExpectedReport = ueExpectedReport;
#elif TDD_CONFIG
            /* SPR 18122 Changes Start*/
        ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]->\
            cqiPmiRiReportFormatMapSCell[MODULO(sysFrameNum,2)][reportMapIndex].
            ueExpectedReport = ueExpectedReport;
            /* SPR 18122 Changes End*/
#endif
        /** SPR 14032 Fix : End **/
    }
    /*SPR 5620 END*/

    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
        deltaOffsetCQI = ulUEContext_p->puschConfigInfo.
        betaOffsetCQIIndex;

    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
        deltaOffsetRI = ulUEContext_p->puschConfigInfo.
        betaOffsetRiIndex;

    if (ueCellInfo_p->isPCell)
    {
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
            pucchIndex = ulUEContext_p->pucchConfigInfo.
            cqiPUCCHRBIndex;
    }
    else
    {
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
            pucchIndex = ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]->\
            scellpucchConfigInfo.cqiPUCCHRBIndex;
    }
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
        rankIndicator = rankIndicator;
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].cqiRiInfo.
        reportType =CQI_RI_PERIODIC_MODE ;


    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"CQI/ PMI/ RI Data stored in UL UE "
            "context");
    return;
}

/*End SS 2 */

/* SPR 1101 changes start*/
/*****************************************************************************
 * Function Name  : buildUciHarqForTempUE
 * Inputs         : contentionInfo_p - Contention info
 *                  contentionUECount - Ue Count
 *                  rbMapNum - rb map number,
 *                  ulSubFrame
 *                  ulschUciRachInfo_p - pointer to the container node where
 *                     information for UL config is set
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates the UCI information for UL Config
 *****************************************************************************/
/* UCI_HARQ for Contention Resolution Messages */
STATIC  void buildUciHarqForTempUE(
        PdcchTemporaryContextUEInfo *contentionInfo_p,
        UInt8 contentionUECount,
        UInt32  rbMapNum,
#ifdef TDD_CONFIG    
        UInt8 ulSubFrame,
#endif
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        InternalCellIndex internalCellIndex
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
#ifdef TDD_CONFIG    
    UInt32 Np = 0;
    UInt32 Npnext = 0;
#endif    
    UInt32  nPUCCH = 0;
    UInt8 rbIndexSlot1 = 0;
    UInt8 rbIndexSlot2 = 0;
#ifdef TDD_CONFIG    
    SInt32 kValue = 0;
    /*CA TDD CHANGES START*/
    UInt8   M = globalDlAssociationSet_gp[internalCellIndex][ulSubFrame].count;
    /*CA TDD CHANGES END*/
    UInt8   count = 0;
    UInt8   k = 0;
    UInt8   m = 0;
#endif    
    TcrntiCCEIndexInfo *tcrntiInfo_p = PNULL;
    UInt16  numberOfInfoPresent = 0;

    UCIFormat pucchFormat = UCI_FORMAT_ONE_A;


    while(contentionUECount--)
    {
        /* SPR# 9506 start */ 
        numberOfInfoPresent = ulschUciRachInfo_p->numberOfInfoPresent;
        /* SPR# 9506 end */ 
#ifdef TDD_CONFIG        
        /* SPR 4301 chg */
        tcrntiInfo_p = &(contentionInfo_p->pdcchULUEArrayForTempUlGroup[contentionUECount]); 
#else
        tcrntiInfo_p = &(contentionInfo_p->pdcchTempUEArray[contentionUECount]);
#endif    

        /*Fill rnti value */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].rnti = 
            tcrntiInfo_p->tcrnti;
        /* SPR 4301 chg */

#ifdef TDD_CONFIG    
        kValue = ulSubFrame - tcrntiInfo_p->dlSubFrame;
        /* SPR 12112 Fix Start */
        kValue = (kValue) < 4?(kValue + MAX_SUB_FRAME):(kValue);
        /* SPR 12112 Fix End */

        for (count = 0; count < M; count++)
        {
            /*
             * Get the smallest k
             */
            /*CA TDD CHANGES START*/
            k = globalDlAssociationSet_gp[internalCellIndex][ulSubFrame].sortedKwithSmallM[count].k;
            /*CA TDD CHANGES END*/
            if (k == kValue)
            {
                /*CA TDD CHANGES START*/
                m = globalDlAssociationSet_gp[internalCellIndex][ulSubFrame].sortedKwithSmallM[count].smallM;
                /*CA TDD CHANGES END*/
                LTE_MAC_UT_LOG(LOG_INFO, PUCCH_MODULE, "ulSubFrame [%d] m [%d] count[%d] kValue[%d] k[%d] dlSubframe [%d]\n",
                        ulSubFrame, m, count, kValue, k, tcrntiInfo_p->dlSubFrame);
                break;
            }
        }

        /* 
         * nPUCCH? Refer sec 10.1 of 36.213 pg 69
         * nPUCCH = (M -m -1)*Np + m*Np_plus_1 + cceIndex + n1PucchDLAckNack_g;
         */
        pucchGetNpsForCCE(tcrntiInfo_p->cceIndex, &Np, &Npnext, internalCellIndex);
        nPUCCH = (M - m -1)*Np + (m*Npnext) + tcrntiInfo_p->cceIndex + n1PucchDLAckNack_g[internalCellIndex];
#else
        nPUCCH =  tcrntiInfo_p->cceIndex + n1PucchDLAckNack_g[internalCellIndex];
#endif
        /*+COVERITY 5.3.0 - 10013*/
        if(MAC_FAILURE == getPRBFromPUCCHResourceIndex(nPUCCH, pucchFormat, &rbIndexSlot1, &rbIndexSlot2, internalCellIndex))
        {
            return;    
        }
        /*-COVERITY 5.3.0 - 10013*/
        allocateResourcesForPUCCHReports(rbIndexSlot1, rbIndexSlot2, rbMapNum, internalCellIndex);

        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType = MAC_SS_PDU_UCI_HARQ;
#ifdef TDD_CONFIG    
        /* Fill basic harq info */
        /** SPR 14172 Fix : Start **/
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.harqSize = 0;
        /** SPR 14172 Fix : End **/
        /* Fill extended harq info */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.numOfPUCCHResource = 1;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_0 = nPUCCH;
        /** SPR 14172 Fix : Start **/
        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_1 = 
                                                                         HARQ_INVALID;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_2 = 
                                                                         HARQ_INVALID;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.n_PUCCH_1_3 =
                                                                         HARQ_INVALID;
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
        
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.ackNackMode = ACK_NACK_BUNDLING;
#else
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.acknackMode = ACK_NACK_BUNDLING;
#endif
        /** SPR 14172 Fix : End **/

#else
        /* Fill harq info */
//#ifdef FAPI_4_0_COMPLIANCE
#if 1
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.n_PUCCH_1_0 = nPUCCH;
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.numOfPUCCHResource=1;
#else						
        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
            dlHarqInfo.pucchIndex = nPUCCH;
#endif


#endif    
        /*Coverity_31965 Fix Start*/
        lteMacSsCalcHarqSize(
                &ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                dlHarqInfo.harqSize ,
                /*Coverity_31965 Fix End*/
                DCI_FORMAT_FOR_CONTENTION_MSG);
        /** SPR 14172 Fix : Start **/
        ulschUciRachInfo_p->numberOfInfoPresent++;
        /** SPR 14172 Fix : End **/
    }
    /* -COVERITY 10013 */    

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}
/* SPR 1101 changes end*/

/*****************************************************************************
 * Function Name  : reConfigRIAndCQIMap 
 * Inputs         : ulUeCtx_p - UL UE Context
 *                  dwlkUEInfo_p - DL UE Context params.
 *                  uplkUEInfo_p - UL UE Context params. 
 *                  cqiRiReconfigRetValue_p - Bitmap.
 *                  reconfigInfo_p - UE reconfig info
 *                  ueCellInfo_p,pucchConfig_p,J
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        :
 * Description    : This function handles both CQI and RI Reconfiguration.
 *                  Firstly Check that offset and Periodicity are same then
 *                  only PUCCH ResourceIndex is chnaged then traverse and 
 *                  update the PRB index. otherwise delete the List and 
 *                  create new nodes.
 *****************************************************************************/
/*Reconfig Changes Start */
MacRetType reConfigRIAndCQIMap ( 
        ULUEContext *ulUeCtx_p, 
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink *uplkUEInfo_p
        /*SPR 16855 fix start*/
        ,ULReconfigCQIRIMsgType *cqiRiReconfigRetValue_p
        ,UeContextForReconfig *reconfigInfo_p
        /*SPR 16855 fix end*/
        ,UeCellInfo * ueCellInfo_p
/* + CA_TDD_HARQ_CHANGES */    
        ,PucchConfig *pucchConfig_p
        ,UInt8 J
/* - CA_TDD_HARQ_CHANGES */    
        ,InternalCellIndex internalCellIndex
        )
/*Reconfig Changes End */
{
    /*SPR 16855 fix start*/
    UInt16 ueIndex = ulUeCtx_p->ueIndex;
    UInt32 passiveIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
    /*SPR 16855 fix end*/
    UInt32 cqiPeriodicity    = 0;
    UInt32 cqiSubFrameOffset = 0;
    UInt32 riPeriodicity     = 0;
    UInt32 riSubFrameOffset  = 0;
    UInt32 cqiPMIConfigIndex = uplkUEInfo_p->cqiPMIConfigIndex;
    UInt32 riConfigIndex     = uplkUEInfo_p->riConfigIndex;
    UInt32 pucchResourceIndex = uplkUEInfo_p->cqiPucchResourceIndex;
    UInt8  K = uplkUEInfo_p->cqiformatIndicatorPeriodic_subband_k;
    UInt8  H = 1; /* Since valid valeu of H is always greater than 1 */
    PeriodicFeedbackType periodicFeedBack = 
        uplkUEInfo_p->cqiformatIndicatorPeriodic;

    /* SPR 14325 fix start */
    TransmissonMode  transmissionMode = INVALID_TX_MODE; 
    DLUEContext * dlUEContext_p = PNULL;
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    DLUESCellContext *dlUeScellContext_p = PNULL;
    /* SPR 14325 fix end */
    /*SPR 16855 fix start*/
    if (!ueCellInfo_p->isPCell)
    {
        cqiPMIConfigIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                           cqiInfo.cqiPMIConfigIndex;
        riConfigIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                        cqiInfo.riConfigIndex;
        pucchResourceIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                                cqiInfo.cqiPucchResourceIndex;
        K = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
             cqiInfo.cqiformatIndicatorPeriodic_subband_k;
        /*Coverity 63969 fix starts*/
        periodicFeedBack = (PeriodicFeedbackType)uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                            cqiInfo.cqiformatIndicatorPeriodic;
        /*Coverity 63969 fix ends*/
    }
    /*SPR 16855 fix end*/
/* - CA_TDD_HARQ_CHANGES */    
    /* cqi feedback will be both wideband + subband*/
    if (PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack) 
    {
        /* calculate H Factor */
        H = (J*K) + 1; /* If H is set, it will always be greater than 1 */
    }

    if ( MAC_SUCCESS == getCqiRiPeriodicityAndOffset( 
                cqiPMIConfigIndex, 
                riConfigIndex, 
                &cqiPeriodicity,
                &cqiSubFrameOffset,
                &riPeriodicity,
                &riSubFrameOffset,
                H))
    {
        /*
         * cqiPeriodicity will be returned without being multiplied by H
         * because we need it for subband cqi reporting instances in case of
         * wideband + subband reporting
         * 
         * riPeriodicity will on the other hand be returned after 
         * being multiplied by H
         */
        /* TDD SB_CQI */
        if( 
                (pucchConfig_p->cqiPmiConfigIndex != cqiPMIConfigIndex || 
                 pucchConfig_p->cqiPmiRiInfo.periodicFeedBackType != periodicFeedBack )
          )
        {
            /* HD FDD Changes Start */
#ifdef FDD_CONFIG
#ifdef HD_FDD_CONFIG
            /*SPR 16855 fix start*/
            if ((ulUeCtx_p->isHDFddFlag) &&
                    ((cqiPeriodicity != 0) && (cqiPeriodicity <= HD_UE_MIN_REPORT_PERIODICITY )))
                /*SPR 16855 fix end*/
                {
                    LOG_MAC_MSG(MAC_HD_FDD_MIN_REPORTING, LOGWARNING, MAC_RRC_INF,\
                            getCurrentTick(), \
                            ueIndex,cqiPeriodicity,\
                            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE, \
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE, \
                            FUNCTION_NAME,"CQI_PERIODICITY");
                }
#endif
#endif
            /*HD FDD Changes End*/        
            /* TDD SB_CQI */
            /* RECONFIGURE CQI */
            /* Delete CQI nodes from PUCCH queue */
            deletePucchNodesForUE( pucchConfig_p, CQI_REPORT , 
                    ueCellInfo_p,
                    internalCellIndex);
            /*Reconfig Changes Start */
            reconfigInfo_p->cqiRiReconfigBitMap |= UL_RECONFIG_DELETE_CQI_NODE;
            /*Reconfig Changes End */

            if (MAC_FAILURE == createPUCCHMapForReportOrSR(
                        /*SPR 16855 fix start*/
                        ueIndex,
                        pucchConfig_p, 
                        /*SPR 16855 fix end*/
                        ulUeCtx_p, 
                        cqiSubFrameOffset, 
                        cqiPeriodicity,
                        CQI_REPORT /*reportType*/,
                        /*SPR 16855 fix start*/
                        &(pucchConfig_p->maxCQIWrapAround[passiveIndex])
                        /*SPR 16855 fix end*/
                        ,pucchResourceIndex
                        ,UCI_FORMAT_ONE_B
                        ,ueCellInfo_p
                        ,internalCellIndex
                        ) ) 
                        {
                            return MAC_FAILURE;
                        }
            if (PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack) 
            {
                /* Delete SB-CQI nodes from PUCCH queue */ 
                deletePucchNodesForUE( pucchConfig_p, WIDEBAND_CQI_REPORT,
                        ueCellInfo_p,
                        internalCellIndex);
                if (MAC_FAILURE == createPUCCHMapForReportOrSR(
                            /*SPR 16855 fix start*/
                            ueIndex,
                            pucchConfig_p, 
                            /*SPR 16855 fix end*/
                            ulUeCtx_p, 
                            cqiSubFrameOffset, 
                            cqiPeriodicity*H,
                            WIDEBAND_CQI_REPORT /*reportType*/,
                            /*SPR 16855 fix start*/
                            &(pucchConfig_p->maxWbCQIWrapAround[passiveIndex])
                            /*SPR 16855 fix end*/
                            ,pucchResourceIndex
                            ,UCI_FORMAT_ONE_B
                            ,ueCellInfo_p
                            ,internalCellIndex 
                            ) ) 
                            {
                                return MAC_FAILURE;       
                            }
            }
            /*SPR 16855 fix start*/
            *cqiRiReconfigRetValue_p |= CQI_MAP_RECONFIGURE_IND;
            /*set the CQI bit to update the passive index after getting response from HP thread*/
            /*Reconfig Changes Start */
            reconfigInfo_p->cqiRiReconfigBitMap |= UL_RECONFIG_ADD_CQI_NODE;
            /*Reconfig Changes End */
            /*SPR 16855 fix end*/
        }
        /* TDD SB_CQI */
        if (pucchResourceIndex != pucchConfig_p->cqiPUCCHRBIndex)
        { 
            /* TDD SB_CQI */
            /* Only PUCCH Resource Index is chaned */
            pucchConfig_p->cqiPUCCHRBIndex = pucchResourceIndex;
            /*SPR 16855 fix start*/
            *cqiRiReconfigRetValue_p |= CQI_RESOURCE_INDEX_RECONFIGURE_IND;
            /*SPR 16855 fix end*/
        }
        else
        {
            /*All Parameters are same. No need to Reconfigure */
            /*SPR 16855 fix start*/
            *cqiRiReconfigRetValue_p |= NO_CQI_RECONFIGURE_IND;
            /*SPR 16855 fix end*/
        }

        /* TDD SB_CQI */
        if(MAC_RI_CONFIG_INDEX_INVALID != riConfigIndex)
        {                
            if( 
                    (pucchConfig_p->cqiPmiConfigIndex != cqiPMIConfigIndex || 
                     pucchConfig_p->riConfigIndex != riConfigIndex )
              )
            {
                /* Reconfigure RI Map */
                /* Delete SB-CQI nodes from PUCCH queue */ 
                //                if(pucchConfig_p->riConfigIndex != MAC_RI_CONFIG_INDEX_INVALID)
                {    
                    deletePucchNodesForUE( pucchConfig_p, RI_REPORT, 
                            ueCellInfo_p,
                            internalCellIndex);
                    /*Reconfig Changes Start */
                    reconfigInfo_p->cqiRiReconfigBitMap |= UL_RECONFIG_DELETE_RI_NODE;
                    /*Reconfig Changes End */
                }
                if (MAC_FAILURE == createPUCCHMapForReportOrSR( 
                            /*SPR 16855 fix start*/
                            ueIndex,
                            pucchConfig_p,
                            /*SPR 16855 fix end*/
                            ulUeCtx_p, 
                            riSubFrameOffset, 
                            riPeriodicity,
                            RI_REPORT /*reportType*/,
                            /*SPR 16855 fix start*/
                            &(pucchConfig_p->maxRIWrapAround[passiveIndex])
                            /*SPR 16855 fix end*/
                            ,pucchResourceIndex
                            ,UCI_FORMAT_ONE_B
                            ,ueCellInfo_p
                            ,internalCellIndex
                            ) )
                            {
                                return MAC_FAILURE;
                            }
                /*SPR 16855 fix start*/
                *cqiRiReconfigRetValue_p |= RI_MAP_RECONFIGURE_IND;
                /*set the RI bit to update the passive index after getting response from HP thread*/
                /*Reconfig Changes Start */
                reconfigInfo_p->cqiRiReconfigBitMap |= UL_RECONFIG_ADD_RI_NODE;
                /*Reconfig Changes End */
                /*SPR 16855 fix end*/
            }
            else
            {
                /* All Parameters are same. No need to Reconfigure */
                /*SPR 16855 fix start*/
                *cqiRiReconfigRetValue_p |= NO_RI_RECONFIGURE_IND;
                /*SPR 16855 fix end*/
            }
        }
        else if(pucchConfig_p->riConfigIndex != MAC_RI_CONFIG_INDEX_INVALID)
        {
            /* Delete SB-CQI nodes from PUCCH queue */ 
            deletePucchNodesForUE( pucchConfig_p, RI_REPORT, 
                    ueCellInfo_p,
                    internalCellIndex);
            /*Reconfig Changes Start */
            reconfigInfo_p->cqiRiReconfigBitMap |= UL_RECONFIG_DELETE_RI_NODE;
            /*Reconfig Changes End */
            /*SPR 9160 fix start*/
            /*SPR 16855 fix start*/
            *cqiRiReconfigRetValue_p |= RI_MAP_RECONFIGURE_IND;
            /*SPR 16855 fix end*/
            /*SPR 9160 fix end*/
        }
        else
        {
            /* All Parameters are same. No need to Reconfigure */
            /*SPR 16855 fix start*/
            *cqiRiReconfigRetValue_p |= NO_RI_RECONFIGURE_IND;
            /*SPR 16855 fix end*/
        }
        /* TDD SB_CQI */
    }
    else
    {
        return MAC_FAILURE;
    }

    pucchConfig_p->cqiPeriodicity = cqiPeriodicity; 
    pucchConfig_p->cqiOffset = cqiSubFrameOffset; 
    pucchConfig_p->riPeriodicity = riPeriodicity; 
    pucchConfig_p->riOffset = riSubFrameOffset; 
    pucchConfig_p->cqiPmiRiInfo.periodicFeedBackType = 
        uplkUEInfo_p->cqiformatIndicatorPeriodic;
    pucchConfig_p->cqiPmiRiInfo.k = K;
    pucchConfig_p->cqiPmiRiInfo.widebandPeriodicity = 
        (H * cqiPeriodicity);
    /* Check if transmission Mode is Valid then only update periodic
       Report Mode */
    /*SPR 9160 fix start*/
    if (  INVALID_TX_MODE  != dwlkUEInfo_p->transmissionMode  )
    {
        pucchConfig_p->cqiPmiRiInfo.periodicRepMode = 
            getPeriodicReportingMode(dwlkUEInfo_p->transmissionMode, 
                    pucchConfig_p->cqiPmiRiInfo.periodicFeedBackType,
                    /* + TM7_8 Changes Start */
                    dwlkUEInfo_p->pmiRiReportV920);
        /* - TM7_8 Changes End */
    }
    /*SPR 9160 fix end*/
    /* SPR 14325 fix start */
    else
    {
        /* if RRC sends CQI tlv in Reconfig Request but does not send TX Mode,then
         * the initial transmissionMode should be used to get the periodic report type */
        if((FALSE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && (PNULL != dlUEContext_p))
        {
            if (!ueCellInfo_p->isPCell)
            {
                dlUeScellContext_p = dlUEContext_p->dlUeScellContext_p[ueCellInfo_p->sCellIndex];
                if(dlUeScellContext_p != PNULL)
                {
                    transmissionMode = dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode;
                }
            }
            else
            {
                transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
            }

            pucchConfig_p->cqiPmiRiInfo.periodicRepMode = 
                getPeriodicReportingMode(transmissionMode, 
                        pucchConfig_p->cqiPmiRiInfo.periodicFeedBackType,
                        /* + TM7_8 Changes Start */
                        dwlkUEInfo_p->pmiRiReportV920);
                        /* - TM7_8 Changes End */
        }

    }
    /* SPR 14325 fix end */
    pucchConfig_p->cqiPmiConfigIndex = cqiPMIConfigIndex;
    pucchConfig_p->riConfigIndex = riConfigIndex;

    if (!ueCellInfo_p->isPCell)
    {
        if (SETUP == uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].cqiInfo.cqiConfigRequestType)
        {

            /*         reConfigRIAndCQIMapAbs (ulUeCtx_p,dwlkUEInfo_p,uplkUEInfo_p, cqiRiReconfigRetValue_p
                       ,reconfigInfo_p,ueCellInfo_p,pucchConfig_p, J, internalCellIndex);*/

            reConfigRIAndCQIMapAbs(ulUeCtx_p, dwlkUEInfo_p, uplkUEInfo_p
                    /*SPR 16855 fix start*/
                    ,cqiRiReconfigRetValue_p, reconfigInfo_p 
                    /*SPR 16855 fix end*/
                    ,ueCellInfo_p
                    ,pucchConfig_p
                    ,J
                    ,internalCellIndex);


        }
    }
    else
    {
        if (SETUP == uplkUEInfo_p->cqiConfigRequestType)
        {

            /*         reConfigRIAndCQIMapAbs (ulUeCtx_p,dwlkUEInfo_p,uplkUEInfo_p, cqiRiReconfigRetValue_p
                       ,reconfigInfo_p,ueCellInfo_p,pucchConfig_p, J, internalCellIndex);*/

            reConfigRIAndCQIMapAbs(ulUeCtx_p, dwlkUEInfo_p, uplkUEInfo_p
                    /*SPR 16855 fix start*/
                    ,cqiRiReconfigRetValue_p, reconfigInfo_p 
                    /*SPR 16855 fix end*/
                    ,ueCellInfo_p
                    ,pucchConfig_p
                    ,J
                    ,internalCellIndex);


        }
    }
    return MAC_SUCCESS;
}
MacRetType reConfigRIAndCQIMap_dummy( ULUEContext *ulUeCtx_p,
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink *uplkUEInfo_p,
        /*SPR 16855 fix start*/
        ULReconfigCQIRIMsgType *cqiRiReconfigRetValue_p,
        UeContextForReconfig *reconfigInfo_p,
        /*SPR 16855 fix end*/
        UeCellInfo * ueCellInfo_p,
        PucchConfig *pucchConfig_p,
        UInt8 J,
        InternalCellIndex internalCellIndex
        )
{
    MacRetType resp;
    resp =  reConfigRIAndCQIMap(ulUeCtx_p,
            dwlkUEInfo_p,
            uplkUEInfo_p,
            /*SPR 16855 fix start*/
            cqiRiReconfigRetValue_p,
            reconfigInfo_p,
            /*SPR 16855 fix end*/
            ueCellInfo_p,
            pucchConfig_p,
            J,
            internalCellIndex
            );
    return resp;
}

/*****************************************************************************
 * Function Name  : reConfigSchedReqInPUCCHMap 
 * Inputs         : ulUeCtx_p - UL UE Context
 *                  dwlkUEInfo_p - DL UE Context params.
 *                  uplkUEInfo_p - UL UE Context params. 
 *                  srReconfigRetValue_p bitmap 
 *                  reconfigInfo_p - UE reconfig info
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        : -
 * Returns        : None
 * Description    : This function Reconfigure the SR Periodic Nodes.
 *****************************************************************************/
/*Reconfig Changes Start */
MacRetType reConfigSchedReqInPUCCHMap( 
        ULUEContext *ulUeCtx_p,
        /* +- SPR 17777 */
        UeContextForUplink *uplkUEInfo_p
        /*SPR 16855 fix start*/
        ,ULReconfigSRMsgType *srReconfigRetValue_p,
        /*SPR 16855 fix end*/
        UeContextForReconfig *reconfigInfo_p
        ,InternalCellIndex internalCellIndex
        )
/*Reconfig Changes End */
{
    UInt8 srPeriodicity    = 0;
    UInt8 srSubFrameOffset = 0;
    PucchConfig *pucchConfig_p = PNULL;
    UInt32 srConifgIndex = uplkUEInfo_p->srConfigurationIndex;
    UInt16 srProhibitTimerValv920 = uplkUEInfo_p->srProhibitTimerValv920;
    UInt32 srPucchResourceIndex = uplkUEInfo_p->srPucchResourceIndex;
    UInt16 ueIndex = ulUeCtx_p->ueIndex;

    UeCellInfo ueCellInfo = {0};

    ueCellInfo.isPCell = TRUE;

    getSRPeriodicityAndSrOffset( srConifgIndex, 
            &srPeriodicity, 
            &srSubFrameOffset
            );

    pucchConfig_p = &ulUeCtx_p->pucchConfigInfo; 

    /* check that SR periodicity or srSubFrameOffset is not equal 
       to previous one*/
       /* SPR 16841 fix start */
    if ( 
            (reconfigInfo_p->srOffset != srSubFrameOffset ||
            reconfigInfo_p->srPeriodicity != srPeriodicity)
       )
    {
#ifdef FDD_CONFIG
        /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
        /* Check the HD UE SR periodicity met the minimum report periodicity*/
        /*SPR 16855 fix start*/
        if ((ulUeCtx_p->isHDFddFlag)
                &&
                ((srPeriodicity != 0) && (srPeriodicity <= HD_UE_MIN_REPORT_PERIODICITY)))
            /*SPR 16855 fix end*/
            {
                LOG_MAC_MSG(MAC_HD_FDD_MIN_REPORTING, LOGWARNING, MAC_RRC_INF,\
                        getCurrentTick(), \
                        ueIndex,srPeriodicity,\
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE, \
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE, \
                        FUNCTION_NAME,"SR_PERIODICITY");

            }
#endif
#endif

        /* If previous periodicty was < 2 ms delete the existing 
           short map */
        if (SHORT_SR_PERIODICITY >= reconfigInfo_p->srPeriodicity )
            /* SPR 16841 fix end */
        {
            deletePucchShortSRNodesForUE(  pucchConfig_p,reconfigInfo_p, internalCellIndex );
            /*SPR 16891 Fix Start*/
            reconfigInfo_p->srReconfigBitMap |= UL_RECONFIG_DELETE_SHORT_SR_NODE;
            /*SPR 16891 Fix End*/
        }
        /* If previous periodicty was > 2 ms delete the existing map */
        else
        {
            deletePucchNodesForUE(  pucchConfig_p, SCHEDULING_REQUEST, 
                    &ueCellInfo,
                    internalCellIndex);
            /*SPR 16891 Fix Start*/
            reconfigInfo_p->srReconfigBitMap |= UL_RECONFIG_DELETE_LONG_SR_NODE;
            /*SPR 16891 Fix End*/
        }

        /* if reconfigured value of sr periodicty is < 2 ms create 
           short map */
        if (SHORT_SR_PERIODICITY >= srPeriodicity )
        {
            /*coverity-530-25318*/
            if(MAC_FAILURE == createPUCCHMapForShortSR( 
                        /*SPR 16855 fix start*/
                        ueIndex,
                        pucchConfig_p,
                        /*SPR 16855 fix end*/
                        srSubFrameOffset,
                        srPeriodicity 
                        ,srPucchResourceIndex
                        ,internalCellIndex
                        ))
            {
                LOG_MAC_MSG(MAC_PUCCH_INIT_SR_MAP_FAIL,LOGERROR,MAC_PUCCH,\
                        getCurrentTick(),\
                        ueIndex,\
                        srConifgIndex,\
                        srPeriodicity,\
                        0,0,0.0,0.0,__func__,"PUCCH_SR_MAP_RECONFIG_FAILED");

                return MAC_FAILURE;
            }
            /*coverity-530-25318*/
            /*SPR 16855 fix start*/
            *srReconfigRetValue_p =  UL_RECONFIG_SR_MAP_MSG;
            /*SPR 16855 fix end*/
        }
        else
        {
            /* if reconfigured value of sr periodicty is > 2 ms create 
               normal map */
            if ( MAC_FAILURE == createPUCCHMapForReportOrSR( 
                        /*SPR 16855 fix start*/
                        ueIndex,
                        pucchConfig_p,
                        /*SPR 16855 fix end*/
                        ulUeCtx_p, 
                        srSubFrameOffset, 
                        srPeriodicity,
                        SCHEDULING_REQUEST /*reportType*/,
                        /*SPR 16855 fix start*/
                        &(pucchConfig_p->maxSrWrapAround[
                            longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex])
                        /*SPR 16855 fix end*/
                        ,srPucchResourceIndex
                        ,UCI_FORMAT_ONE_B
                        ,&ueCellInfo
                        ,internalCellIndex
                        ))
                        {
                            LOG_MAC_MSG(MAC_PUCCH_INIT_SR_MAP_FAIL,LOGERROR,MAC_PUCCH,\
                                    getCurrentTick(),\
                                    ueIndex,\
                                    srConifgIndex,\
                                    srPeriodicity,\
                                    0,0,0.0,0.0,__func__,"PUCCH_SR_MAP_RECONFIG_FAILED");

                            return MAC_FAILURE;
                        }
            /*SPR 16855 fix start*/
            *srReconfigRetValue_p = UL_RECONFIG_SR_MAP_MSG;
            /*SPR 16855 fix end*/
        }
    }
    else if (srPucchResourceIndex != pucchConfig_p->srPUCCHRBIndex)
    {
        /* Only PUCCH Resource Index is chaned */
        pucchConfig_p->srPUCCHRBIndex = srPucchResourceIndex;
        /*SPR 16855 fix start*/
        *srReconfigRetValue_p |= UL_RECONFIG_SR_RESOURCE_INDEX_MSG;
        /*SPR 16855 fix end*/
    }
    else
    {
        /* No need to Reconfigure SR Reports. All parameters are same */
        /*SPR 16855 fix start*/
        *srReconfigRetValue_p =  UL_RECONFIG_NO_SR_RECONFIGURE_MSG;
        /*SPR 16855 fix end*/
    }
    pucchConfig_p->srPUCCHRBIndex = srPucchResourceIndex;
    pucchConfig_p->srPeriodicity  = srPeriodicity;
    pucchConfig_p->srProhibitTimerValv920 = srProhibitTimerValv920 * srPeriodicity;

    return MAC_SUCCESS;
}
/*Reconfig Changes Start */
/*****************************************************************************
 * Function Name  : reconfigureCqiRiPassiveContextAbs
 * Inputs         : ulUeCtx_p - UL UE Context
 *                  ueIndex - Ue Index
 *                  tempPucchConfigInfo_p - PUCCH config Info
 *                  dwlkUEInfo_p - DL UE Context params.
 *                  uplkUEInfo_p - UL UE Context params. 
 *                  reconfigInfo_p- Reconfiguration Info 
 *                  ueCellInfo_p,
 *                  J,
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        : -
 * Returns        : None
 * Description    : This function Reconfigure the CQI Periodic Nodes of passive context.
 *****************************************************************************/
  MacRetType reconfigureCqiRiPassiveContextAbs(
        ULUEContext          *ulUeCtx_p,
        UInt16               ueIndex,
        PucchConfig          *tempPucchConfigInfo_p,
        /* +- SPR 17777 */
        UeContextForUplink   *uplkUEInfo_p,
        UeContextForReconfig *reconfigInfo_p,
        UeCellInfo * ueCellInfo_p,
        UInt8 J,
        InternalCellIndex    internalCellIndex)

{
    UInt32 cqiPeriodicity  = 0;
    UInt32 nOffsetCQI      = 0;
    UInt32 riPeriodicity   = 0;
    UInt32 nOffsetRI       = 0;
    UInt32 cqiConfigIndex  = uplkUEInfo_p->cqiPMIConfigIndex_2;
    UInt32 riConfigIndex   = uplkUEInfo_p->riConfigIndex_2;
    UInt8  K = uplkUEInfo_p->cqiformatIndicatorPeriodic_subband_k;
    UInt8   H = 1; /* Since valid valeu of H is always greater than 1 */
    PeriodicFeedbackType periodicFeedBack = 
        uplkUEInfo_p->cqiformatIndicatorPeriodic;
    UInt32 passiveIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;

    if (!ueCellInfo_p->isPCell)
    {
        cqiConfigIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                           cqiInfo.cqiPMIConfigIndex_2;
        riConfigIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                        cqiInfo.riConfigIndex_2;
        K = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
             cqiInfo.cqiformatIndicatorPeriodic_subband_k;
        /*Coverity 63969 fix starts*/
        periodicFeedBack = (PeriodicFeedbackType)uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                            cqiInfo.cqiformatIndicatorPeriodic;
        /*Coverity 63969 fix ends*/
    }

    /* cqi feedback will be both wideband + subband*/
    if (PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack) 
    {
        //calculate H Factor
        H = (J*K) + 1; /* If H is set, it will always be greater than 1 */
    }
    /*coverity-530-55304*/
    if(MAC_SUCCESS != getCqiRiPeriodicityAndOffset( cqiConfigIndex, riConfigIndex, 
                &cqiPeriodicity, &nOffsetCQI, &riPeriodicity, &nOffsetRI, H))
    {
        return MAC_FAILURE;
    }
    /*coverity-530-55304*/
    updateCQIMapForAbs_g[internalCellIndex] = LTE_TRUE;
    if(reconfigInfo_p->cqiRiReconfigBitMap & UL_RECONFIG_DELETE_CQI_NODE2)
    {
        deletePucchNodesForUE( tempPucchConfigInfo_p, CQI_REPORT, 
                ueCellInfo_p,
                internalCellIndex);
    }
    if(reconfigInfo_p->cqiRiReconfigBitMap & UL_RECONFIG_DELETE_RI_NODE2)
    {
        deletePucchNodesForUE( tempPucchConfigInfo_p, RI_REPORT, 
                ueCellInfo_p,
                internalCellIndex);
    }

    if(reconfigInfo_p->cqiRiReconfigBitMap & UL_RECONFIG_ADD_CQI_NODE2) 
    {

        if (MAC_FAILURE == createPUCCHMapForReportOrSR( 
                    ueIndex,
                    tempPucchConfigInfo_p, 
                    ulUeCtx_p, 
                    nOffsetCQI, 
                    cqiPeriodicity,
                    CQI_REPORT /*reportType*/,
                    &(tempPucchConfigInfo_p->maxCQIWrapAround2[passiveIndex])
                    ,uplkUEInfo_p->cqiPucchResourceIndex
                    ,UCI_FORMAT_ONE_B
                    ,ueCellInfo_p
                    ,internalCellIndex                
                    ) ) 
        {
           updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
            return MAC_FAILURE;
        }
        if (PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack) 
        {
            /* Delete SB-CQI nodes from PUCCH queue */ 
            deletePucchNodesForUE( tempPucchConfigInfo_p, WIDEBAND_CQI_REPORT, 
                    ueCellInfo_p,
                    internalCellIndex);

            if (MAC_FAILURE == createPUCCHMapForReportOrSR( 
                        ueIndex,
                        tempPucchConfigInfo_p,
                        ulUeCtx_p, 
                        nOffsetCQI, 
                        cqiPeriodicity*H,
                        WIDEBAND_CQI_REPORT /*reportType*/,
                        &(tempPucchConfigInfo_p->maxWbCQIWrapAround2[passiveIndex])
                        ,uplkUEInfo_p->cqiPucchResourceIndex
                        , UCI_FORMAT_ONE_B
                        ,ueCellInfo_p
                        ,internalCellIndex 
                        ) ) 
            {
                           updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
                return MAC_FAILURE;       
            }
        }
    }
    else if(reconfigInfo_p->cqiRiReconfigBitMap & UL_RECONFIG_UPDATE_CQI_NODE2) 
    {
        if ( MAC_FAILURE == traverseAndUpdatePUCCHResourceIndex
                ( 
                 tempPucchConfigInfo_p,
                 CQI_REPORT,
                 UCI_FORMAT_TWO , 
                 /* +- SPR 17777 */
                 uplkUEInfo_p->cqiPucchResourceIndex,internalCellIndex
                 /* +- SPR 17777 */
                ))
        {
                      updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
            return MAC_FAILURE;
        }
    }
    if(reconfigInfo_p->cqiRiReconfigBitMap & UL_RECONFIG_ADD_RI_NODE2) 
    {

        if (MAC_FAILURE == createPUCCHMapForReportOrSR( 
                    ueIndex,
                    tempPucchConfigInfo_p, 
                    ulUeCtx_p, 
                    nOffsetRI, 
                    riPeriodicity,
                    RI_REPORT /*reportType*/,
                    &(tempPucchConfigInfo_p->maxRIWrapAround2[passiveIndex])
                    ,uplkUEInfo_p->cqiPucchResourceIndex
                    , UCI_FORMAT_ONE_B
                    ,ueCellInfo_p
                    ,internalCellIndex 
                    ) )
        {
                     updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
            return MAC_FAILURE;
        }
    }
    else   if(reconfigInfo_p->cqiRiReconfigBitMap & UL_RECONFIG_UPDATE_RI_NODE2)
    {
        if ( MAC_FAILURE == traverseAndUpdatePUCCHResourceIndex
                ( 
                 tempPucchConfigInfo_p,
                 RI_REPORT, 
                 UCI_FORMAT_TWO, 
                 uplkUEInfo_p->cqiPucchResourceIndex,
                 /* +- SPR 17777 */
                 internalCellIndex
                 /* +- SPR 17777 */
                ))
        {
                    updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
            return MAC_FAILURE;
        }
    }
     updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : reconfigureCqiRiPassiveContext 
 * Inputs         : ulUeCtx_p - UL UE Context
 *                  ueIndex - Ue Index
 *                  tempPucchConfigInfo_p - PUCCH config Info
 *                  dwlkUEInfo_p - DL UE Context params.
 *                  uplkUEInfo_p - UL UE Context params. 
 *                  reconfigInfo_p- Reconfiguration Info 
 *                  ueCellInfo_p,
 *                  J,
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        : -
 * Returns        : None
 * Description    : This function Reconfigure the CQI Periodic Nodes of passive context.
 *****************************************************************************/
  MacRetType reconfigureCqiRiPassiveContext(
        ULUEContext          *ulUeCtx_p,
        UInt16               ueIndex,
        PucchConfig          *tempPucchConfigInfo_p,
        /* +- SPR 17777 */
        UeContextForUplink   *uplkUEInfo_p,
        UeContextForReconfig *reconfigInfo_p,
        UeCellInfo * ueCellInfo_p,
        UInt8 J,
        InternalCellIndex    internalCellIndex)

{
    UInt32 cqiPeriodicity  = 0;
    UInt32 nOffsetCQI      = 0;
    UInt32 riPeriodicity   = 0;
    UInt32 nOffsetRI       = 0;
    UInt32 cqiConfigIndex  = uplkUEInfo_p->cqiPMIConfigIndex;
    UInt32 riConfigIndex   = uplkUEInfo_p->riConfigIndex;
    UInt8  K = uplkUEInfo_p->cqiformatIndicatorPeriodic_subband_k;
    UInt8   H = 1; /* Since valid valeu of H is always greater than 1 */
    PeriodicFeedbackType periodicFeedBack = 
        uplkUEInfo_p->cqiformatIndicatorPeriodic;
    UInt32 passiveIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
    if (!ueCellInfo_p->isPCell)
    {
        cqiConfigIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                           cqiInfo.cqiPMIConfigIndex;
        riConfigIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                        cqiInfo.riConfigIndex;
        K = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
             cqiInfo.cqiformatIndicatorPeriodic_subband_k;
        /*Coverity 63969 fix starts*/
        periodicFeedBack = (PeriodicFeedbackType)uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                            cqiInfo.cqiformatIndicatorPeriodic;
        /*Coverity 63969 fix ends*/
    }

    /* cqi feedback will be both wideband + subband*/
    if (PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack) 
    {
        //calculate H Factor
        H = (J*K) + 1; /* If H is set, it will always be greater than 1 */
    }
    /*coverity-530-55304*/
    if(MAC_SUCCESS != getCqiRiPeriodicityAndOffset( cqiConfigIndex, riConfigIndex, 
                &cqiPeriodicity, &nOffsetCQI, &riPeriodicity, &nOffsetRI, H))
    {
        return MAC_FAILURE;
    }
    /*coverity-530-55304*/
    if(reconfigInfo_p->cqiRiReconfigBitMap & UL_RECONFIG_DELETE_CQI_NODE)
    {
        deletePucchNodesForUE( tempPucchConfigInfo_p, CQI_REPORT, 
                ueCellInfo_p,
                internalCellIndex);
    }
    if(reconfigInfo_p->cqiRiReconfigBitMap & UL_RECONFIG_DELETE_RI_NODE)
    {
        deletePucchNodesForUE( tempPucchConfigInfo_p, RI_REPORT, 
                ueCellInfo_p,
                internalCellIndex);
    }
    if(reconfigInfo_p->cqiRiReconfigBitMap & UL_RECONFIG_ADD_CQI_NODE) 
    {
        if (MAC_FAILURE == createPUCCHMapForReportOrSR( 
                    ueIndex,
                    tempPucchConfigInfo_p, 
                    ulUeCtx_p, 
                    nOffsetCQI, 
                    cqiPeriodicity,
                    CQI_REPORT /*reportType*/,
                    &(tempPucchConfigInfo_p->maxCQIWrapAround[passiveIndex])
                    ,uplkUEInfo_p->cqiPucchResourceIndex
                    ,UCI_FORMAT_ONE_B
                    ,ueCellInfo_p
                    ,internalCellIndex                
                    ) ) 
        {
            return MAC_FAILURE;
        }
        if (PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack) 
        {
            /* Delete SB-CQI nodes from PUCCH queue */ 
            deletePucchNodesForUE( tempPucchConfigInfo_p, WIDEBAND_CQI_REPORT, 
                    ueCellInfo_p,
                    internalCellIndex);
            if (MAC_FAILURE == createPUCCHMapForReportOrSR( 
                        ueIndex,
                        tempPucchConfigInfo_p,
                        ulUeCtx_p, 
                        nOffsetCQI, 
                        cqiPeriodicity*H,
                        WIDEBAND_CQI_REPORT /*reportType*/,
                        &(tempPucchConfigInfo_p->maxWbCQIWrapAround[passiveIndex])
                        ,uplkUEInfo_p->cqiPucchResourceIndex
                        , UCI_FORMAT_ONE_B
                        ,ueCellInfo_p
                        ,internalCellIndex 
                        ) ) 
            {
                return MAC_FAILURE;       
            }
        }
    }
    else if(reconfigInfo_p->cqiRiReconfigBitMap & UL_RECONFIG_UPDATE_CQI_NODE) 
    {
        if ( MAC_FAILURE == traverseAndUpdatePUCCHResourceIndex
                ( 
                 tempPucchConfigInfo_p,
                 CQI_REPORT,
                 UCI_FORMAT_TWO , 
                 /* +- SPR 17777 */
                 uplkUEInfo_p->cqiPucchResourceIndex,internalCellIndex
                 /* +- SPR 17777 */
                ))
        {
            return MAC_FAILURE;
        }
    }
    if(reconfigInfo_p->cqiRiReconfigBitMap & UL_RECONFIG_ADD_RI_NODE) 
    {
        if (MAC_FAILURE == createPUCCHMapForReportOrSR( 
                    ueIndex,
                    tempPucchConfigInfo_p, 
                    ulUeCtx_p, 
                    nOffsetRI, 
                    riPeriodicity,
                    RI_REPORT /*reportType*/,
                    &(tempPucchConfigInfo_p->maxRIWrapAround[passiveIndex])
                    ,uplkUEInfo_p->cqiPucchResourceIndex
                    , UCI_FORMAT_ONE_B
                    ,ueCellInfo_p
                    ,internalCellIndex 
                    ) )
        {
            return MAC_FAILURE;
        }
    }
    else   if(reconfigInfo_p->cqiRiReconfigBitMap & UL_RECONFIG_UPDATE_RI_NODE)
    {
        if ( MAC_FAILURE == traverseAndUpdatePUCCHResourceIndex
                ( 
                 tempPucchConfigInfo_p,
                 RI_REPORT, 
                 UCI_FORMAT_TWO, 
                 uplkUEInfo_p->cqiPucchResourceIndex,
                 /* +- SPR 17777 */
                 internalCellIndex
                 /* +- SPR 17777 */
                ))
        {
            return MAC_FAILURE;
        }
    }
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : reconfigureSRPassiveContext 
 * Inputs         : ulUeCtx_p - UL UE Context
 *                  dwlkUEInfo_p - DL UE Context params.
 *                  ueIndex - UE Index
 *                  tempPucchConfigInfo_p - PUCCH config Info
 *                  uplkUEInfo_p - UL UE Context params. 
 *                  reconfigInfo_p- Reconfiguration Info, 
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        : -
 * Returns        : None
 * Description    : This function Reconfigure the SR Periodic Nodes of passive context.
 *****************************************************************************/
 MacRetType reconfigureSRPassiveContext( 
        ULUEContext          *ulUeCtx_p,
        /* +- SPR 17777 */
        UInt16               ueIndex,
        PucchConfig          *tempPucchConfigInfo_p,
        UeContextForUplink   *uplkUEInfo_p,
        UeContextForReconfig *reconfigInfo_p,
        InternalCellIndex internalCellIndex )

{
    UInt8 srPeriodicity    = 0;
    UInt8 srSubFrameOffset = 0;
    UInt32 srConifgIndex = uplkUEInfo_p->srConfigurationIndex;
    /* SPR 16841 fix start */
    ULReconfigSRMsgType srReconfigRetValue         = UL_RECONFIG_SR_MAP_MSG;
    /* SPR 16841 fix end */

    UeCellInfo ueCellInfo = {0};

    ueCellInfo.isPCell = TRUE;

    getSRPeriodicityAndSrOffset( srConifgIndex, 
            &srPeriodicity, 
            &srSubFrameOffset
            );
    if(reconfigInfo_p->srReconfigBitMap & UL_RECONFIG_DELETE_SHORT_SR_NODE)
    {
        deletePucchShortSRNodesForUE( &ulUeCtx_p->pucchConfigInfo,reconfigInfo_p, internalCellIndex );
    }
    /*SPR 9160 fix start*/
    else if(reconfigInfo_p->srReconfigBitMap & UL_RECONFIG_DELETE_LONG_SR_NODE)
    /*SPR 9160 fix end*/
    {
        deletePucchNodesForUE( &ulUeCtx_p->pucchConfigInfo, SCHEDULING_REQUEST, 
                &ueCellInfo,
                internalCellIndex);
    }
    if(reconfigInfo_p->srReconfigBitMap & UL_RECONFIG_ADD_SHORT_SR_NODE )
    {
        if ( MAC_FAILURE == createPUCCHMapForShortSR( 
                    ueIndex,
                    tempPucchConfigInfo_p,
                    srSubFrameOffset,
                    srPeriodicity 
                    ,uplkUEInfo_p->srPucchResourceIndex
                    ,internalCellIndex            
                    ))
        {
            LOG_MAC_MSG(MAC_PUCCH_INIT_SR_MAP_FAIL,LOGERROR,MAC_PUCCH,\
                    getCurrentTick(),\
                    ueIndex,\
                    srConifgIndex,\
                    srPeriodicity,\
                    0,0,0.0,0.0,__func__,"PUCCH_SR_MAP_RECONFIG_FAILED");

            return MAC_FAILURE;
        }
    }
    /*SPR 9160 fix start*/
    else if(reconfigInfo_p->srReconfigBitMap & UL_RECONFIG_ADD_LONG_SR_NODE)
    /*SPR 9160 fix end*/
    {
        /* SPR 16841 fix start */
        if(MAC_FAILURE == reConfigSchedReqInPUCCHMap(ulUeCtx_p,
                            uplkUEInfo_p,
                    &srReconfigRetValue,
                    reconfigInfo_p,internalCellIndex
                    ))

/* SPR 16841 fix end */
            {
                LOG_MAC_MSG(MAC_PUCCH_INIT_SR_MAP_FAIL,LOGERROR,MAC_PUCCH,\
                        getCurrentTick(),\
                        ueIndex,\
                        srConifgIndex,\
                        srPeriodicity,\
                        0,0,0.0,0.0,__func__,"PUCCH_SR_MAP_RECONFIG_FAILED");

                return MAC_FAILURE;
            }
    }
/* - CA_TDD_HARQ_CHANGES */    
/*SPR 16143 Fix Start*/
    else if (reconfigInfo_p->srReconfigBitMap & UL_RECONFIG_UPDATE_SHORT_SR_NODE) 
/*SPR 16143 Fix End*/
    {
        if (MAC_FAILURE == updatePUCCHResourceIdxForShortSR( 
                    tempPucchConfigInfo_p,
                    SCHEDULING_REQUEST,
                    UCI_FORMAT_ONE, 
                    uplkUEInfo_p->srPucchResourceIndex,
                    internalCellIndex
                    ))
        {
            LOG_MAC_MSG(MAC_PUCCH_UPDATE_RES_INDEX_FAILED,LOGERROR,MAC_PUCCH,\
                    getCurrentTick(),\
                    ueIndex,\
                    srConifgIndex,\
                    srPeriodicity,\
                    0,0,0.0,0.0,__func__,"PUCCH_SR_RESOURCE_INDEX_UPDATE_FAILED");
            return MAC_FAILURE;
        }
    }
/*SPR 16143 Fix Start*/
    else if (reconfigInfo_p->srReconfigBitMap & UL_RECONFIG_UPDATE_LONG_SR_NODE) 
/*SPR 16143 Fix End*/
    {
        if (MAC_FAILURE == traverseAndUpdatePUCCHResourceIndex( 
                    tempPucchConfigInfo_p,
                    SCHEDULING_REQUEST,
                    UCI_FORMAT_ONE, 
                    /* +- SPR 17777 */
                    uplkUEInfo_p->srPucchResourceIndex,
                    /* +- SPR 17777 */
                    internalCellIndex
                    ))
        {
            LOG_MAC_MSG(MAC_PUCCH_UPDATE_RES_INDEX_FAILED,LOGERROR,MAC_PUCCH,\
                    getCurrentTick(),\
                    ueIndex,\
                    uplkUEInfo_p->srPucchResourceIndex,\
                    srPeriodicity,\
                    0,0,0.0,0.0,__func__,"PUCCH_SR_RESOURCE_INDEX_UPDATE_FAILED");
            return MAC_FAILURE;
        }
    }    
    return MAC_SUCCESS;
}
/* Reconfig Changes End */
/*ca-tdd pucch code changes*/
/*****************************************************************************
 * Function Name  : updateBwPartOnWideBandSubBandReport 
 * Inputs         : ulUEContext_p - UL UE Context
 *                  currentTTI - Current TTI
 *                  sysFrameNum - System frame number
 *                  subFrameNum - Sub frame number
 *                  ueCellInfo_p,
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        : -
 * Returns        : void
 * Description    : This function update the Bandwidth for CQI reports. 
 *****************************************************************************/
  void updateBwPartOnWideBandSubBandReport(ULUEContext *ulUEContext_p,
        /*SPR 15909 fix start*/
        tickType_t currentTTI,
        /*SPR 15909 fix end*/
        UInt32 sysFrameNum,
        UInt32 subFrameNum,
        UeCellInfo * ueCellInfo_p,
        InternalCellIndex internalCellIndex)
{

    if (ueCellInfo_p->isPCell)
    {
        /* + CQI_4.1 */
        if(PERIODIC_FEEDBACK_SUBBAND == 
                ulUEContext_p->pucchConfigInfo.cqiPmiRiInfo.periodicFeedBackType &&
                FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p
                [internalCellIndex]->cellParams_p->freqSelectiveSchDL)
        {  
            if( currentTTI == ulUEContext_p->nextWidebandOccurrence)
            { 
                UPDATE_BANDWIDTHPART_ON_WIDEBAND_REPORT(ulUEContext_p,
                        sysFrameNum,subFrameNum);    
            }   
            else  if(currentTTI == ulUEContext_p->nextSubbandOccurrence)
            {
                UPDATE_BANDWIDTHPART_ON_SUBBAND_REPORT(ulUEContext_p,
                        sysFrameNum,subFrameNum, internalCellIndex);
            }    
           else if( currentTTI == ulUEContext_p->nextWidebandOccurrence_2) /* EICIC +*/
            { 
                UPDATE_BANDWIDTHPART_ON_WIDEBAND_REPORT_2(ulUEContext_p,
                        sysFrameNum,subFrameNum);    
            }   
            else  if(currentTTI == ulUEContext_p->nextSubbandOccurrence_2)
            {
                UPDATE_BANDWIDTHPART_ON_SUBBAND_REPORT_2(ulUEContext_p, /* EICIC -*/
                        sysFrameNum,subFrameNum, internalCellIndex);
            }  
        }
        /* - CQI_4.1 */
    }
    else
    {
        /* + CQI_4.1 */
        if(PERIODIC_FEEDBACK_SUBBAND == 
                ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]->\
                scellpucchConfigInfo.cqiPmiRiInfo.periodicFeedBackType &&
                FREQUENCY_SELECTIVE_ON == cellSpecificParams_g.cellConfigAndInitParams_p
                [ueCellInfo_p->sCellInternalCellIndex]->cellParams_p->freqSelectiveSchDL)
        {  
            if( currentTTI == ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]\
                    ->nextWidebandOccurrence)
            {
                /** SPR 12577 Fix : Start **/
                UPDATE_SCELL_BANDWIDTHPART_ON_WIDEBAND_REPORT(ulUEContext_p,
                        sysFrameNum,subFrameNum,ueCellInfo_p->sCellIndex);    
            }   
            else  if(currentTTI == ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]\
                    ->nextSubbandOccurrence)
            {
                UPDATE_SCELL_BANDWIDTHPART_ON_SUBBAND_REPORT(ulUEContext_p,
                        sysFrameNum,subFrameNum, ueCellInfo_p->sCellIndex,ueCellInfo_p->sCellInternalCellIndex);
                /** SPR 12577 Fix : End **/
            }    /* EICIC +*/
             else if( currentTTI == ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]\
                    ->nextWidebandOccurrence_2)
            {
                /** SPR 12577 Fix : Start **/
                UPDATE_SCELL_BANDWIDTHPART_ON_WIDEBAND_REPORT_2(ulUEContext_p,
                        sysFrameNum,subFrameNum,ueCellInfo_p->sCellIndex);    
            }   
            else  if(currentTTI == ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex]\
            	->nextSubbandOccurrence_2)
            {
                UPDATE_SCELL_BANDWIDTHPART_ON_SUBBAND_REPORT_2(ulUEContext_p,
                        sysFrameNum,subFrameNum, ueCellInfo_p->sCellIndex,ueCellInfo_p->sCellInternalCellIndex);
                /** SPR 12577 Fix : End **/
            }    /* EICIC -*/

        }
        /* - CQI_4.1 */
    }
}
 void updateBwPartOnWideBandSubBandReport_dummy(ULUEContext *ulUEContext_p,
         /* SPR 15909 fix start */
         tickType_t currentTTI,
         /* SPR 15909 fix end */
         UInt32 sysFrameNum,
         UInt32 subFrameNum,
         UeCellInfo * ueCellInfo_p,
         InternalCellIndex internalCellIndex)
{
    updateBwPartOnWideBandSubBandReport(ulUEContext_p,
            currentTTI,
            sysFrameNum,
            subFrameNum,
            ueCellInfo_p,
            internalCellIndex);
}

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : macValidateUePucchResources
 * Inputs         : UeContextForUplink  *uplkUEInfo_p,
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks the PUCCH resources required for
 *                  the UE is less than or equal to the reserved value or 
 *                  not.Returns SUCCESS if PUCCH resources required is less 
 *                  than reserved value otherwise returns FAILURE.
 *****************************************************************************/
MacRetType macValidateUePucchResources(UeContextForUplink  *uplkUEInfo_p,
        InternalCellIndex internalCellIndex)
{
    UInt8   rbIndexSlot1   = 0;
    UInt8   rbIndexSlot2   = 0;

    /* Check PUCCH Resources for SR */
    if ( uplkUEInfo_p->requestType == TRUE)
    {
        if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex( 
                    uplkUEInfo_p->srPucchResourceIndex,
                    UCI_FORMAT_ONE,
                    &rbIndexSlot1,
                    &rbIndexSlot2,
                    internalCellIndex
                    ))
        {
            LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                    getCurrentTick(),
                    uplkUEInfo_p->ueIdx,
                    uplkUEInfo_p->srPucchResourceIndex,
                    UCI_FORMAT_ONE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "Invalid Configuraton For PUCCH Res for SR");
            return MAC_FAILURE;
        }
        if (rbIndexSlot1 > rbsForPUCCH_g[internalCellIndex])
        {
            LOG_MAC_MSG(MAC_PUCCH_MIN_RB_FAIL,LOGERROR,MAC_PUCCH,
                    getCurrentTick(),
                    uplkUEInfo_p->ueIdx,
                    uplkUEInfo_p->srPucchResourceIndex,
                    UCI_FORMAT_ONE,
                    rbIndexSlot1,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "PUCCH Resources required for SR is greater than the reserved value");
            return MAC_FAILURE;
        } 
    }
    /* Check PUCCH Resources for CQI/PMI/RI */
    if ( uplkUEInfo_p->cqiRequestType == TRUE )
    {
        if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex( 
                    uplkUEInfo_p->cqiPucchResourceIndex,
                    UCI_FORMAT_TWO,
                    &rbIndexSlot1,
                    &rbIndexSlot2,
                    internalCellIndex
                    ))
        {
            LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                    getCurrentTick(),
                    uplkUEInfo_p->ueIdx,
                    uplkUEInfo_p->cqiPucchResourceIndex,
                    UCI_FORMAT_TWO,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "Invalid Configuraton For PUCCH Res for SR");
            return MAC_FAILURE;
        }
        if (rbIndexSlot1 > rbsForPUCCH_g[internalCellIndex])
        {
            LOG_MAC_MSG(MAC_PUCCH_MIN_RB_FAIL,LOGERROR,MAC_PUCCH,
                    getCurrentTick(),
                    uplkUEInfo_p->ueIdx,
                    uplkUEInfo_p->cqiPucchResourceIndex,
                    UCI_FORMAT_TWO,
                    rbIndexSlot1,
                    rbsForPUCCH_g[internalCellIndex],
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "PUCCH Resources required for SR is greater than the reserved value");
            return MAC_FAILURE;
        } 
    }
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : calculateMaxPucchRegion
 * Inputs         : internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks the PUCCH resources for MSG3/SPS 
 *                  Returns SUCCESS if PUCCH resources required is less 
 *                  than reserved value otherwise returns FAILURE.
 *****************************************************************************/
MacRetType calculateMaxPucchRegion(InternalCellIndex internalCellIndex)
{
    UInt8   cceIndex       = 0;
    SInt8   ulPucchRes     = 0;
    UInt8   rbIndexSlot1   = 0;
    UInt8   rbIndexSlot2   = 0;
    UInt32  nPUCCH         = 0;
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* SPR 19309 + */
    cceIndex = maxCCEForCFI_g[internalCellIndex][cellParams_p->cfi - 1];
    /* SPR 19309 - */
    /* cceIndex start and endrange is 0 to maxCCE-1 */
    nPUCCH = (cceIndex - 1) + n1PucchDLAckNack_g[internalCellIndex];

    /* Check PUCCH Resources for HARQ ACK */
    if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex(
                nPUCCH,
                UCI_FORMAT_ONE_A,
                &rbIndexSlot1,
                &rbIndexSlot2,
                internalCellIndex
                ))
    {
        LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                getCurrentTick(),
                0,
                n1PucchDLAckNack_g[internalCellIndex],
                UCI_FORMAT_ONE_A,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "Invalid Configuraton For PUCCH Res");
        return MAC_FAILURE;
    }
    /* If UL BW is 1.4 set ulPucchRes = 2 othrewise set ulPucchRes = 4*/
    if (cellParams_p->ulAvailableRBs > MAX_RB_FOR_BW_1_4)
    {
        ulPucchRes = MIN_PUCCH_RES_MSG3_SPS_FOR_OTHRE_THAN_1_4BW;
    }
    else
    {
        ulPucchRes = MIN_PUCCH_RES_MSG3_SPS_FOR_1_4BW;
    }

    /* Check if RB left for MSG3/SPS or not if not retunr Failure */
        /*SPR 20899 Fix Start*/
    if(ulPucchRes > (DIVIDE_BY_TWO (cellParams_p->ulAvailableRBs)  - rbIndexSlot1))
        /*SPR 20899 Fix Stop*/
    {
        LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                getCurrentTick(),
                0,
                n1PucchDLAckNack_g[internalCellIndex],
                UCI_FORMAT_ONE_A,
                ulPucchRes,
                rbsForPUCCH_g[internalCellIndex],
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "Error RB not left for MSG3/SPS ");
        return MAC_FAILURE;
    }
	/* SPR 21859 fix start */
    /* SPR 21603 has been reverted in SPR 21859 */
	rbsForPUCCH_g[internalCellIndex] = rbIndexSlot1;
	/* SPR 21859 fix end */
    return MAC_SUCCESS;

}

/*cov warning fix ca-tdd*/
/*****************************************************************************
 * Function Name  :updateIsCQIMasked  
 * Inputs         :ulUEContext_p - UL UE Context
 *                 ttiWithDelay - TTI with ul delay 
 *                  ulUEScheduledFlag  
 * Outputs        :isCQIMasked
 * Returns        :void
 * Description    :This function checks and updates if cqi is masked. 
 *****************************************************************************/
STATIC  void updateIsCQIMasked(ULUEContext *ulUEContext_p,
        /*14766 start*/
        DLUEContext *ueDLContext_p,
        /*14766 end */
        UInt8 *isCQIMasked,
        /* SPR 15909 fix start */
        tickType_t ttiWithDelay,
        /* SPR 15909 fix end */
        UInt8 ulUEScheduledFlag)
{
    /* Rel9_upgrade_CR396 */
    UInt32 ttiOffset = 0;
    UInt32 drxOffset = 0;
    /* Rel9_upgrade_CR396 */
    /* Rel9_upgrade_CR396 */

    /* If the report is coming on PUSCH, cqiMasked is not to be considered */
    /* + SPR 6864 */
    if (INVALID_CCE_INDEX != ulUEScheduledFlag)
    {
        *isCQIMasked = FALSE;
    }
    else
    {    
        /* - SPR 6864 */
        if (ulUEContext_p->ulDrxContext.cqiMask)
        {
            if (ulUEContext_p->ulDrxContext.isDRXShortCycleConfigured)
            {
                ttiOffset = ttiWithDelay %
                    ulUEContext_p->ulDrxContext.drxShortCycle;
                drxOffset = ulUEContext_p->ulDrxContext.drxOffsetModShortCycle;
            }
            else
            {
                ttiOffset = ttiWithDelay % ulUEContext_p->ulDrxContext.drxLongCycle;
                drxOffset = ulUEContext_p->ulDrxContext.drxStartOffset;
            }

            if ((ttiOffset - drxOffset) >= ulUEContext_p->ulDrxContext.
                    drxOnDurationTimer)
            {
                *isCQIMasked = TRUE;
            }
        }
        else
        {
            /*14766 start*/	   
            if(ueDLContext_p->drxContext.drxStateVariables.drxCurrentState == DRX_STATE_ON)
            {
                /* SPR 15582 Fix Start */
                if (ulUEContext_p->ulDrxContext.isDRXShortCycleConfigured 
                        && DRX_SHORT_CYCLE == ueDLContext_p->drxContext.drxStateVariables.drxNextCycle)
                {
                    if (ttiWithDelay < ueDLContext_p->drxContext.drxStateVariables.drxShortCycleNextOnDurationTTI)
                    {
                        *isCQIMasked = TRUE;
                    }
                    else if (ttiWithDelay == ueDLContext_p->drxContext.drxStateVariables.drxShortCycleNextOnDurationTTI)
                    {
                        *isCQIMasked = FALSE;
                    }
                    else
                    {
                        if (ttiWithDelay > 
                                (ueDLContext_p->drxContext.drxStateVariables.drxShortCycleNextOnDurationTTI + 
                                 ulUEContext_p->ulDrxContext.drxOnDurationTimer)
                           )
                        {
                            *isCQIMasked = TRUE;
                        }
                        else
                        {
                            *isCQIMasked = FALSE;
                        }
                    }
                }
                else
                {
                    if (ttiWithDelay < ueDLContext_p->drxContext.drxStateVariables.drxLongCycleNextOnDurationTTI)
                    {
                        *isCQIMasked = TRUE;
                    }
                    else if (ttiWithDelay == ueDLContext_p->drxContext.drxStateVariables.drxLongCycleNextOnDurationTTI)
                    {
                        *isCQIMasked = FALSE;
                    }
                    else
                    {
                        if (ttiWithDelay > 
                                (ueDLContext_p->drxContext.drxStateVariables.drxLongCycleNextOnDurationTTI + 
                                 ulUEContext_p->ulDrxContext.drxOnDurationTimer)
                           )
                        {
                            *isCQIMasked = TRUE;
                        }
                        else
                        {
                            *isCQIMasked = FALSE;
                        }
                    }
                }
            }
            /* SPR 15582 Fix End */
            else
            {
                *isCQIMasked = FALSE;
            }
            /*14766 end*/


        }
    }
}
/*cov warning fix ca-tdd*/
#endif

/*****************************************************************************
 * Function Name  : lteMacSsCalcHarqSize 
 * Inputs         : dlDciFormat - DL DCI Format
 * Outputs        : harqSize - Calculated harq size
 * Returns        : void
 * Description    : This function returns the Harq size for DCI Format
 *****************************************************************************/
/*Coverity_31965 Fix Start*/
void lteMacSsCalcHarqSize(UInt8 * harqSize, 
        DCIFormat dlDciFormat)
/*Coverity_31965 Fix End*/
{
    /* Here,calculate HARQ Size can be changed
     * at transmitter under SPR 3053
     */ 
    switch(dlDciFormat)
    {
        case DCI_FORMAT_0 :
        case DCI_FORMAT_1 :
        case DCI_FORMAT_1A :
        case DCI_FORMAT_1B :
        case DCI_FORMAT_1C :
        case DCI_FORMAT_1D :
            /*Coverity_31965 Fix Start*/
            *harqSize = 1;
            /*Coverity_31965 Fix End*/
            break;
        case DCI_FORMAT_2 :
        case DCI_FORMAT_2A:
        case DCI_FORMAT_3 :
        case DCI_FORMAT_3A :
            /*Coverity_31965 Fix Start*/
            *harqSize = 2;
            /*Coverity_31965 Fix End*/
            break;
        default:
            /*Coverity_31965 Fix Start*/
            *harqSize = 2;
            /*Coverity_31965 Fix End*/
            break;
    }
}

/*****************************************************************************
 * Function Name  : updatePUCCHResourceIdxForShortSR
 * Inputs         : 
 *                  reportType - Its represent the  the Periodic
 *                           Report ( SR)
 *                  pucchFormat,
 *                  pucchResourceIndex - Its represent the PUCCH Resource Index of
 *                                       Periodic Reports.
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        : -pucchConfigInfo :Stores the prbIndexSlot1 and prbIndexSlot2
 *                   for the report
 * Returns        : MAC_SUCCESS or MAC_FAILURE 
 * Description    : This function  update the prb index correspondence to the Periodic
 *                  report (SR,CQI,RI).
 *****************************************************************************/
STATIC MacRetType updatePUCCHResourceIdxForShortSR( 
        /* Review comment fix start PUCCH_ENH_2 */
        PucchConfig *pucchConfigInfo,
        UInt32 reportType,
        /* Review comment fix end PUCCH_ENH_2 */
        UInt32 pucchFormat,
        UInt32 pucchResourceIndex,
        InternalCellIndex internalCellIndex
        )
{
    UInt8 prbIndexSlot1 = 0;
    UInt8 prbIndexSlot2 = 0;
    UInt8 passiveIndex  = 0;

    passiveIndex = shortPeriodicSRMapShortInfo_gp[internalCellIndex]->passiveIndex;
    /*Mapping of PUCCH Resouce Index to 
      Physical Resource Block */
    if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex( pucchResourceIndex,
                pucchFormat, 
                &prbIndexSlot1,
                &prbIndexSlot2,
                internalCellIndex
                ))
    {
        /*coverity-530-54142*/
        lteWarning("Invalid RRC Configuration For PUCCH Resource Index.\n");
        /*coverity-530-54142*/
        return MAC_FAILURE;
    }

    /* Review comment fix start PUCCH_ENH_2 */
    pucchConfigInfo->prbIndexSlot1[passiveIndex][reportType] = prbIndexSlot1;
    pucchConfigInfo->prbIndexSlot2[passiveIndex][reportType] = prbIndexSlot2;
    /* Review comment fix end PUCCH_ENH_2 */

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : traverseAndUpdatePUCCHResourceIndex
 * Inputs         : reportType - Its represent the Periodic Reports (SR, CQI, RI)
 *                  pucchFormat - Its represent the UCI format of Periodic Reports
 *                                (SR -1 , CQI / RI -2);
 *                  pucchResourceIndex - Its represent the PUCCH Resource Index of
 *                                       Periodic Reports.
 *                  ueCellInfo_p,
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        : -pucchConfigInfo :Stores the prbIndexSlot1 and prbIndexSlot2
 *                   for the report
 * Returns        : MAC_SUCCESS or MAC_FAILURE 
 * Description    : This function update the prb index of the periodic 
 *                  report (SR,CQI,RI) in pucchConfigInfo.
 *****************************************************************************/
STATIC MacRetType traverseAndUpdatePUCCHResourceIndex( 
        /* Review comment fix start PUCCH_ENH_2 */
        PucchConfig *pucchConfigInfo,
        UInt32 reportType,
        /* Review comment fix end PUCCH_ENH_2 */
        UInt32 pucchFormat,
        UInt32 pucchResourceIndex,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    UInt8 prbIndexSlot1 = 0;
    UInt8 prbIndexSlot2 = 0;
    UInt8 passiveIndex  = 0;

    passiveIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
    /*Mapping of PUCCH Resouce Index to 
      Physical Resource Block */
    if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex( pucchResourceIndex,
                pucchFormat, 
                &prbIndexSlot1,
                &prbIndexSlot2,
                internalCellIndex
                ))
    {
        LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,\
                getCurrentTick(),\
                /* Review comment fix start PUCCH_ENH_2 */
                0,\
                /* Review comment fix end PUCCH_ENH_2 */
                pucchResourceIndex,\
                0,0,0,0.0,0.0,__func__,"Invalid RRC Configuration"
                "For PUCCH Resource Index");
        return MAC_FAILURE;
    }
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] PRB Index1 =%d PRB Index =%d",
            __func__,prbIndexSlot1,prbIndexSlot2);
    /* Review comment fix start PUCCH_ENH_2 */
    pucchConfigInfo->prbIndexSlot1[passiveIndex][reportType] = prbIndexSlot1;
    pucchConfigInfo->prbIndexSlot2[passiveIndex][reportType] = prbIndexSlot2;
    /* Review comment fix end PUCCH_ENH_2 */
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
    return MAC_SUCCESS;
}

#ifdef HD_FDD_CONFIG
/*SPR 16855 fix start*/
/*****************************************************************************
 * Function Name  : updateHDFddSemiStaticMap
 * Inputs         : ueIndex - UE Index
 *                  reportType - periodic report type
 * Outputs        : -
 * Returns        : void
 * Description    : This function updates the semi static map in HD_FDD
 *****************************************************************************/
void updateHDFddSemiStaticMap(UInt16 ueIndex, UInt8 reportType )
/*SPR 16855 fix end*/    
{
    /*SPR 15909 fix start*/
    tickType_t subFrameOffset = getCurrentTick() + PHY_DL_DELAY;
    /*SPR 15909 fix end*/
    /*SPR 16855 fix start*/
    /*SPR 16855 fix end*/
    /*SPR 16855 fix start*/
    /** CQI **/
    switch(reportType)
    {
        case HD_UE_STATIC_MAP_CQI_PMI:
            HD_UE_SET_REPORT_INFO(subFrameOffset, 
                    ulUECtxInfoArr_g[ueIndex].ulUEContext_p->pucchConfigInfo.cqiOffset,
                    ulUECtxInfoArr_g[ueIndex].ulUEContext_p->pucchConfigInfo.cqiPeriodicity,
                    HD_UE_STATIC_MAP_CQI_PMI,
                    ueIndex);

            break;

        case HD_UE_STATIC_MAP_RI:
            HD_UE_SET_REPORT_INFO(subFrameOffset,
                    ulUECtxInfoArr_g[ueIndex].ulUEContext_p->pucchConfigInfo.riOffset,
                    ulUECtxInfoArr_g[ueIndex].ulUEContext_p->pucchConfigInfo.riPeriodicity,
                    HD_UE_STATIC_MAP_RI,
                    ueIndex);
            break;

        case HD_UE_STATIC_MAP_SR:
            HD_UE_SET_REPORT_INFO(subFrameOffset,
                    ulUECtxInfoArr_g[ueIndex].ulUEContext_p->pucchConfigInfo.srOffset,
                    ulUECtxInfoArr_g[ueIndex].ulUEContext_p->pucchConfigInfo.srPeriodicity,
                    HD_UE_STATIC_MAP_SR,
                    ueIndex);
            /** SR **/
            break;
        case HD_UE_STATIC_MAP_SRS:
            /** SRS **/
            HD_UE_SET_REPORT_INFO(subFrameOffset,
                    ulUECtxInfoArr_g[ueIndex].ulUEContext_p->srsConfigInfo.srsSubFrameOffset,
                    ulUECtxInfoArr_g[ueIndex].ulUEContext_p->srsConfigInfo.srsPeriodicity,
                    HD_UE_STATIC_MAP_SRS,
                    ueIndex);
    }
    /*SPR 16855 fix end*/
    hdUeContextInfo_g.configChange[ueIndex] = TRUE;
}
#endif

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : updateSCellPUCCHConfig
 * Inputs         : deltaPUCCHSHIFT - ds1,ds2,ds3 where ds1 corresponds
 *                                     to value 1 ds2 to 2 etc
 *                  nRBCQI - Bandwidth available for use by PUCCH formats
 *                           2/2a/2b, expressed in multiples of frequency
 *                  nCSAN -  Number of cyclic shifts used for PUCCH formats 1/1a/1b
 *                           in a resource block with a mix of formats
 *                           1/1a/1b and 2/2a/2b
 *                 n1PucchDLAckNack - Resource index for PUCCH formats 1/1a/1b
 *                 ueCellInfo_p,
 *                 ueIndex
 * Outputs        : Update the global variables.
 * Returns        : None
 * Description    : This is wrapper function that update the global variable 
 *                  realted to CQI cyclicPrefix_g,nRBCQI_g,nCsAN_g,
 *                  deltaPUCCHSHift_g according to cellIndex passed.
 *****************************************************************************/
void updateSCellPUCCHConfig( UInt32 deltaPUCCHSHIFT,
        UInt32 nRBCQI,
        UInt32 nCSAN,
        UInt32 n1PucchDLAckNack,
        UeCellInfo * ueCellInfo_p,
        UInt16 ueIndex
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    ULUEContext     *ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    ULUESCellContext *sCellContext_p = PNULL;

    /*** Coverity Fix 62976 : Start ***/
    if (ulUEContext_p && MAX_NUM_SCELL >= ueCellInfo_p->sCellIndex)
    /*** Coverity Fix 62976 : End ***/
    {
        sCellContext_p = ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex];
        updatePUCCHConfig(deltaPUCCHSHIFT, nRBCQI, nCSAN, n1PucchDLAckNack, sCellContext_p->internalCellIndex); 
    }
    else
    {
        LOG_MAC_MSG(MAC_INVALID_CELL_ID,LOGERROR,MAC_PUCCH,
                getCurrentTick(), ueCellInfo_p->sCellIndex, DEFAULT_INT_VALUE, 
                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                DEFAULT_FLOAT_VALUE, FUNCTION_NAME, "Invalid Cell Id");
    }
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}
#endif //FDD_CONFIG
/*****************************************************************************
 * Function Name  : deletePCellPucchNodesForUE
 * Inputs         : pucchConfigInfo_p,
 *                  reportType - report type to be cleaned
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 * Outputs        :
 * Returns        : None
 * Description    : This is wrapper function that processes the pucch db for all the
 *                  possible reports and SR opportunities for given UE according to 
 *                  cell Index passed.
 *****************************************************************************/
void deletePCellPucchNodesForUE(
        PucchConfig *pucchConfigInfo_p,
        ReportAndOpportunity reportType,
        UInt8 internalCellIndex
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    UeCellInfo  ueCellInfo;

    ueCellInfo.isPCell = TRUE;
    ueCellInfo.sCellIndex = PUCCH_INVALID;
    deletePucchNodesForUE(pucchConfigInfo_p, reportType, 
            &ueCellInfo, 
            internalCellIndex);

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}
/*****************************************************************************
 * Function Name  : deleteSCellPucchNodesForUE
 * Inputs         : pucchConfigInfo_p,
 *                  reportType - report type to be cleaned
 *                  ueCellInfo_p,
 *                  internalCellIndex - Primary Cell or Secondary Cell index
 *                  ueIndex  
 * Outputs        :
 * Returns        : None
 * Description    : This is wrapper function that processes the pucch db for all the
 *                  possible reports and SR opportunities for given UE according to 
 *                  cell Index passed.
 *****************************************************************************/
void deleteSCellPucchNodesForUE(
/** SPR 12320 Fix : START **/
        PucchConfig * pucchConfigInfo_p,
/** SPR 12320 Fix : END **/
        ReportAndOpportunity reportType,
        UeCellInfo * ueCellInfo_p,
        UInt8 internalCellIndex,
        UInt16 ueIndex
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    ULUEContext     *ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
//klockworks fix
    if (ulUEContext_p && pucchConfigInfo_p) 
    {
        /** SPR 12320 Fix : START **/
        ueCellInfo_p->isPCell = FALSE;
        deletePucchNodesForUE(pucchConfigInfo_p, reportType,\
                ueCellInfo_p, internalCellIndex);
        /** SPR 12320 Fix : END **/
    }
    else
    {
        LOG_MAC_MSG(MAC_INVALID_CELL_ID,LOGERROR,MAC_PUCCH,
                getCurrentTick(), ueCellInfo_p->sCellIndex, DEFAULT_INT_VALUE, 
                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                DEFAULT_FLOAT_VALUE, FUNCTION_NAME, "Invalid Cell Id");
    }

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}
/*****************************************************************************
 * Function Name  : initPCellCQIAndRIinPUCCHMap
 * Inputs         : ueIndex - UE Index of current UE
 *                  pucchConfigInfo_p,
 *                  currentUEContext_p - ul context of the current UE
 *                  dwlkUEInfo_p - Ue Context for Downlink
 *                  uplkUEInfo_p - Ue Context for Uplink
 *                  internalCellIndex - Primary Cell or Secondary Cell index 
 * Outputs        :
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This is wrapper function to initializes the pucch db for all
 *                  the possible reports and SR opportunities to be processed for
 *                  given UE according to cell Index passed.
 *****************************************************************************/
MacRetType initPCellCQIAndRIinPUCCHMap (
        /*SPR 16855 fix start*/
        UInt16 ueIndex,
        PucchConfig * pucchConfigInfo_p,
        /*SPR 16855 fix end*/
        /* +- SPR 17777 */
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink*  uplkUEInfo_p,
        UInt8 internalCellIndex
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    MacRetType resp = MAC_FAILURE;
    ULUEContext     *ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    UInt8   J = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->bandwidthParts;
    UeCellInfo  ueCellInfo;

    ueCellInfo.isPCell = TRUE;

    resp = initCQIAndRIinPUCCHMap(
            /*SPR 16855 fix start*/
            ueIndex, pucchConfigInfo_p,
            /*SPR 16855 fix end*/
            ulUEContext_p, dwlkUEInfo_p, uplkUEInfo_p
            ,&ueCellInfo
           , J
/*SPR 11541 Start*/
            ,internalCellIndex,0/*not Used For Pcell*/);
/*SPR 11541 End*/

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
    return resp;
}
/*****************************************************************************
 * Function Name  : initSCellCQIAndRIinPUCCHMap
 * Inputs         : ueIndex - UE Index of current UE
 *                  currentUEContext_p - ul context of the current UE
 *                  dwlkUEInfo_p - Ue Context for Downlink
 *                  uplkUEInfo_p - Ue Context for Uplink
 *                  ueCellInfo_p,
 *                  internalCellIndex - Primary Cell or Secondary Cell index ,
 *                  rrcLoopIndex
 * Outputs        :
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This is wrapper function to initializes the pucch db for all
 *                  the possible reports and SR opportunities to be processed for
 *                  given UE according to cell Index passed.
 *****************************************************************************/
MacRetType initSCellCQIAndRIinPUCCHMap (
        /*SPR 16855 fix start*/
        UInt16 ueIndex,
        /*SPR 16855 fix end*/
        /* +- SPR 17777 */
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink*  uplkUEInfo_p,
        UeCellInfo * ueCellInfo_p,
        UInt8 internalCellIndex,
        /*SPR 11541 Start*/
        UInt8 rrcLoopIndex
        /*SPR 11541 End*/
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    MacRetType resp = MAC_FAILURE;
    ULUEContext     *ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    ULUESCellContext *sCellContext_p = PNULL;
    UInt8   J = 0; 

    if (ulUEContext_p) 
    {
        /*** Coverity 62991 Fix : Start ****/
        sCellContext_p = ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex];
        if (sCellContext_p)
        /*** Coverity 62991 Fix : End ****/
        {
            ueCellInfo_p->isPCell = FALSE;
            ueCellInfo_p->sCellInternalCellIndex = sCellContext_p->internalCellIndex;
            J = cellSpecificParams_g.cellConfigAndInitParams_p[sCellContext_p->internalCellIndex]\
                ->cellParams_p->bandwidthParts;
            resp = initCQIAndRIinPUCCHMap(
                    /*SPR 16855 fix start*/
                    ueIndex, &(sCellContext_p->scellpucchConfigInfo),
                    /*SPR 16855 fix end*/
                    ulUEContext_p, dwlkUEInfo_p, uplkUEInfo_p, ueCellInfo_p,
                    J,\
        /*SPR 11541 Start*/
                    internalCellIndex,rrcLoopIndex); 
        /*SPR 11541 End*/
        }   
        else
        {
            LOG_MAC_MSG(MAC_SCELL_NOT_ACTIVE,LOGERROR,MAC_PUCCH,
                    getCurrentTick(), ueCellInfo_p->sCellIndex, DEFAULT_INT_VALUE, 
                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                    DEFAULT_FLOAT_VALUE, FUNCTION_NAME, "SCell not active");
        }
    }
    else
    {
        LOG_MAC_MSG(MAC_INVALID_CELL_ID,LOGERROR,MAC_PUCCH,
                getCurrentTick(), ueCellInfo_p->sCellIndex, DEFAULT_INT_VALUE, 
                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                DEFAULT_FLOAT_VALUE, FUNCTION_NAME, "Invalid Cell Id");
    }

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
    return resp;
}

/*****************************************************************************
 * Function Name  : reConfigRIAndCQIMapAbs
 * Inputs         : ulUEContext_p - UL UE Context
 *                  dwlkUEInfo_p - DL UE Context params.
 *                  uplkUEInfo_p - UL UE Context params. 
 *                  cqiRiReconfigRetValue_p - Bitmap.
 *                  reconfigInfo_p - UE reconfig info
 *                  internalCellIndex - Primary Cell or Secondary Cell index 
 * Outputs        :
 * Returns        :
 * Description    : This is wrapper function that handles both CQI and RI 
 *                  Reconfiguration for particular cell for abs subframe.
 *****************************************************************************/
MacRetType reConfigRIAndCQIMapAbs( 
        ULUEContext *ulUeCtx_p, 
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink *uplkUEInfo_p
        /*SPR 16855 fix start*/
        ,ULReconfigCQIRIMsgType *cqiRiReconfigRetValue_p
        ,UeContextForReconfig *reconfigInfo_p
        /*SPR 16855 fix end*/
        ,UeCellInfo * ueCellInfo_p
        ,PucchConfig *pucchConfig_p
        ,UInt8 J
        ,InternalCellIndex internalCellIndex
        )
/*Reconfig Changes End */
{
    /*SPR 16855 fix start*/
    UInt16 ueIndex = ulUeCtx_p->ueIndex;
    UInt32 passiveIndex = longPeriodicSRCQIPMIRIMapInfo_gp[internalCellIndex]->passiveIndex;
    /*SPR 16855 fix end*/
    UInt32 cqiPeriodicity    = 0;
    UInt32 cqiSubFrameOffset = 0;
    UInt32 riPeriodicity     = 0;
    UInt32 riSubFrameOffset  = 0;
    UInt32 cqiPMIConfigIndex = uplkUEInfo_p->cqiPMIConfigIndex_2;
    UInt32 riConfigIndex     = uplkUEInfo_p->riConfigIndex_2;
    UInt32 pucchResourceIndex = uplkUEInfo_p->cqiPucchResourceIndex;
    
    /* SPR 14325 fix start */
    TransmissonMode  transmissionMode = INVALID_TX_MODE; 
    DLUEContext * dlUEContext_p = PNULL;
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    DLUESCellContext *dlUeScellContext_p = PNULL;
    /* SPR 14325 fix end */
    UInt8  K = uplkUEInfo_p->cqiformatIndicatorPeriodic_subband_k;
    UInt8  H = 1; /* Since valid valeu of H is always greater than 1 */
    PeriodicFeedbackType periodicFeedBack = 
        uplkUEInfo_p->cqiformatIndicatorPeriodic;

    /*SPR 16855 fix start*/
    if (!ueCellInfo_p->isPCell)
    {
        cqiPMIConfigIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                           cqiInfo.cqiPMIConfigIndex_2;
        riConfigIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                        cqiInfo.riConfigIndex_2;
        pucchResourceIndex = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                                cqiInfo.cqiPucchResourceIndex;
        K = uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
             cqiInfo.cqiformatIndicatorPeriodic_subband_k;
        /*Coverity 63969 fix starts*/
        periodicFeedBack = (PeriodicFeedbackType)uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].\
                            cqiInfo.cqiformatIndicatorPeriodic;
        /*Coverity 63969 fix ends*/

    }
    /*SPR 16855 fix end*/
    /* cqi feedback will be both wideband + subband*/
    if (PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack) 
    {
        /* calculate H Factor */
        H = (J*K) + 1; /* If H is set, it will always be greater than 1 */
    }

    if ( MAC_SUCCESS == getCqiRiPeriodicityAndOffset( 
                cqiPMIConfigIndex, 
                riConfigIndex, 
                &cqiPeriodicity,
                &cqiSubFrameOffset,
                &riPeriodicity,
                &riSubFrameOffset,
                H))
    {

	 updateCQIMapForAbs_g[internalCellIndex] = LTE_TRUE;
	 
        /*
         * cqiPeriodicity will be returned without being multiplied by H
         * because we need it for subband cqi reporting instances in case of
         * wideband + subband reporting
         * 
         * riPeriodicity will on the other hand be returned after 
         * being multiplied by H
         */
        /* TDD SB_CQI */

        
        if( 
                (pucchConfig_p->cqiPmiConfigIndex != cqiPMIConfigIndex || 
                 pucchConfig_p->cqiPmiRiInfo.periodicFeedBackType != periodicFeedBack )
          )
        {
            /* HD FDD Changes Start */
#ifdef FDD_CONFIG
#ifdef HD_FDD_CONFIG
            /*SPR 16855 fix start*/
            if ((ulUeCtx_p->isHDFddFlag) &&
                    ((cqiPeriodicity != 0) && (cqiPeriodicity <= HD_UE_MIN_REPORT_PERIODICITY )))
                /*SPR 16855 fix end*/
                {
                    LOG_MAC_MSG(MAC_HD_FDD_MIN_REPORTING, LOGWARNING, MAC_RRC_INF,\
                            getCurrentTick(), \
                            ueIndex,cqiPeriodicity,\
                            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE, \
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE, \
                            FUNCTION_NAME,"CQI_PERIODICITY");
                }
#endif
#endif
            /*HD FDD Changes End*/        
            /* TDD SB_CQI */
            /* RECONFIGURE CQI */
            /* Delete CQI nodes from PUCCH queue */
            deletePucchNodesForUE( pucchConfig_p, CQI_REPORT , 
                    ueCellInfo_p,
                    internalCellIndex);
            /*Reconfig Changes Start */
            reconfigInfo_p->cqiRiReconfigBitMap |= UL_RECONFIG_DELETE_CQI_NODE2;
            /*Reconfig Changes End */

            if (MAC_FAILURE == createPUCCHMapForReportOrSR(
                        /*SPR 16855 fix start*/
                        ueIndex,
                        pucchConfig_p, 
                        /*SPR 16855 fix end*/
                        ulUeCtx_p, 
                        cqiSubFrameOffset, 
                        cqiPeriodicity,
                        CQI_REPORT /*reportType*/,
                        /*SPR 16855 fix start*/
                        &(pucchConfig_p->maxCQIWrapAround2[passiveIndex])
                        /*SPR 16855 fix end*/
                        ,pucchResourceIndex
                        ,UCI_FORMAT_ONE_B
                        ,ueCellInfo_p
                        ,internalCellIndex
                        ) ) 
                        {
                            updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
                            return MAC_FAILURE;
                        }
            if (PERIODIC_FEEDBACK_SUBBAND == periodicFeedBack) 
            {
                /* Delete SB-CQI nodes from PUCCH queue */ 
                deletePucchNodesForUE( pucchConfig_p, WIDEBAND_CQI_REPORT,
                        ueCellInfo_p,
                        internalCellIndex);

          
                if (MAC_FAILURE == createPUCCHMapForReportOrSR(
                            /*SPR 16855 fix start*/
                            ueIndex,
                            pucchConfig_p, 
                            /*SPR 16855 fix end*/
                            ulUeCtx_p, 
                            cqiSubFrameOffset, 
                            cqiPeriodicity*H,
                            WIDEBAND_CQI_REPORT /*reportType*/,
                            /*SPR 16855 fix start*/
                            &(pucchConfig_p->maxWbCQIWrapAround2[passiveIndex])
                            /*SPR 16855 fix end*/
                            ,pucchResourceIndex
                            ,UCI_FORMAT_ONE_B
                            ,ueCellInfo_p
                            ,internalCellIndex 
                            ) ) 
                            {
                               updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
                                return MAC_FAILURE;       
                            }
            }
            /*SPR 16855 fix start*/
            *cqiRiReconfigRetValue_p |= CQI_MAP_RECONFIGURE_IND2;
            /*set the CQI bit to update the passive index after getting response from HP thread*/
            /*Reconfig Changes Start */
            reconfigInfo_p->cqiRiReconfigBitMap |= UL_RECONFIG_ADD_CQI_NODE2;
            /*Reconfig Changes End */
            /*SPR 16855 fix end*/
        }
        /* TDD SB_CQI */
        if (pucchResourceIndex != pucchConfig_p->cqiPUCCHRBIndex)
        { 
            /* TDD SB_CQI */
            /* Only PUCCH Resource Index is chaned */
            pucchConfig_p->cqiPUCCHRBIndex = pucchResourceIndex;
            /*SPR 16855 fix start*/
            *cqiRiReconfigRetValue_p |= CQI_RESOURCE_INDEX_RECONFIGURE_IND2;
            /*SPR 16855 fix end*/
        }
        else
        {
            /*All Parameters are same. No need to Reconfigure */
            /*SPR 16855 fix start*/
            *cqiRiReconfigRetValue_p |= NO_CQI_RECONFIGURE_IND2;
            /*SPR 16855 fix end*/
        }


        /* TDD SB_CQI */
        if(MAC_RI_CONFIG_INDEX_INVALID != riConfigIndex)
        {                
            if( 
                    (pucchConfig_p->cqiPmiConfigIndex != cqiPMIConfigIndex || 
                     pucchConfig_p->riConfigIndex != riConfigIndex )
              )
            {
                /* Reconfigure RI Map */
                /* Delete SB-CQI nodes from PUCCH queue */ 
                //                if(pucchConfig_p->riConfigIndex != MAC_RI_CONFIG_INDEX_INVALID)
                {    
                    deletePucchNodesForUE( pucchConfig_p, RI_REPORT, 
                            ueCellInfo_p,
                            internalCellIndex);
                    /*Reconfig Changes Start */
                    reconfigInfo_p->cqiRiReconfigBitMap |= UL_RECONFIG_DELETE_RI_NODE2;
                    /*Reconfig Changes End */
                }

               
                if (MAC_FAILURE == createPUCCHMapForReportOrSR(
                            /*SPR 16855 fix start*/
                            ueIndex,
                            pucchConfig_p, 
                            /*SPR 16855 fix end*/
                            ulUeCtx_p, 
                            riSubFrameOffset, 
                            riPeriodicity,
                            RI_REPORT /*reportType*/,
                            /*SPR 16855 fix start*/
                            &(pucchConfig_p->maxRIWrapAround2[passiveIndex])
                            /*SPR 16855 fix end*/
                            ,pucchResourceIndex
                            ,UCI_FORMAT_ONE_B
                            ,ueCellInfo_p
                            ,internalCellIndex
                            ) )
                            {
                                updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
                                return MAC_FAILURE;
                            }
                /*SPR 16855 fix start*/
                *cqiRiReconfigRetValue_p |= RI_MAP_RECONFIGURE_IND2;
                /*set the RI bit to update the passive index after getting response from HP thread*/
                /*Reconfig Changes Start */
                reconfigInfo_p->cqiRiReconfigBitMap |= UL_RECONFIG_ADD_RI_NODE2;
                /*Reconfig Changes End */
                /*SPR 16855 fix end*/
            }
            else
            {
                /* All Parameters are same. No need to Reconfigure */
                /*SPR 16855 fix start*/
                *cqiRiReconfigRetValue_p |= NO_RI_RECONFIGURE_IND2;
                /*SPR 16855 fix end*/
            }
        }
        else if(pucchConfig_p->riConfigIndex != MAC_RI_CONFIG_INDEX_INVALID)
        {
            /* Delete SB-CQI nodes from PUCCH queue */ 
            deletePucchNodesForUE( pucchConfig_p, RI_REPORT, 
                    ueCellInfo_p,
                    internalCellIndex);
            /*Reconfig Changes Start */
            reconfigInfo_p->cqiRiReconfigBitMap |= UL_RECONFIG_DELETE_RI_NODE2;
            /*Reconfig Changes End */
            /*SPR 9160 fix start*/
            /*SPR 16855 fix start*/
            *cqiRiReconfigRetValue_p |= RI_MAP_RECONFIGURE_IND2;
            /*SPR 16855 fix end*/
            /*SPR 9160 fix end*/
        }
        else
        {
            /* All Parameters are same. No need to Reconfigure */
            /*SPR 16855 fix start*/
            *cqiRiReconfigRetValue_p |= NO_RI_RECONFIGURE_IND2;
            /*SPR 16855 fix end*/
        }
        /* TDD SB_CQI */
    }
    else
    {
        updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
        return MAC_FAILURE;
    }

    pucchConfig_p->cqiPeriodicity_2 = cqiPeriodicity; 
    pucchConfig_p->cqiOffset_2 = cqiSubFrameOffset; 
    pucchConfig_p->riPeriodicity_2 = riPeriodicity; 
    pucchConfig_p->riOffset_2 = riSubFrameOffset; 
    /* Check if transmission Mode is Valid then only update periodic
       Report Mode */
    /*SPR 9160 fix start*/
    if (  INVALID_TX_MODE  != dwlkUEInfo_p->transmissionMode  )
    {
        pucchConfig_p->cqiPmiRiInfo.periodicRepMode = 
            getPeriodicReportingMode(dwlkUEInfo_p->transmissionMode, 
                    pucchConfig_p->cqiPmiRiInfo.periodicFeedBackType,
                    /* + TM7_8 Changes Start */
                    dwlkUEInfo_p->pmiRiReportV920);
        /* - TM7_8 Changes End */
    }
    /*SPR 9160 fix end*/
    /* SPR 14325 fix start */
    else
    {
        /* if RRC sends CQI tlv in Reconfig Request but does not send TX Mode,then
         * the initial transmissionMode should be used to get the periodic report type */
        if((FALSE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag) && (PNULL != dlUEContext_p))
        {
            if (!ueCellInfo_p->isPCell)
            {
                dlUeScellContext_p = dlUEContext_p->dlUeScellContext_p[ueCellInfo_p->sCellIndex];
                if(dlUeScellContext_p != PNULL)
                {
                    transmissionMode = dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode;
                }
            }
            else
            {
                transmissionMode = dlUEContext_p->dlMIMOInfo.transmissionMode;
            }

            pucchConfig_p->cqiPmiRiInfo.periodicRepMode = 
                getPeriodicReportingMode(transmissionMode, 
                        pucchConfig_p->cqiPmiRiInfo.periodicFeedBackType,
                        /* + TM7_8 Changes Start */
                        dwlkUEInfo_p->pmiRiReportV920);
                        /* - TM7_8 Changes End */
        }

    }
    /* SPR 14325 fix end */
    pucchConfig_p->cqiPmiConfigIndex_2 = cqiPMIConfigIndex;
    pucchConfig_p->riConfigIndex_2 = riConfigIndex;
	
     updateCQIMapForAbs_g[internalCellIndex] = LTE_FALSE;
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : reConfigPCellRIAndCQIMap 
 * Inputs         : ulUEContext_p - UL UE Context
 *                  dwlkUEInfo_p - DL UE Context params.
 *                  uplkUEInfo_p - UL UE Context params. 
 *                  cqiRiReconfigRetValue_p - Bitmap.
 *                  reconfigInfo_p - UE reconfig info
 *                  internalCellIndex - Primary Cell or Secondary Cell index 
 * Outputs        :
 * Returns        :
 * Description    : This is wrapper function that handles both CQI and RI 
 *                  Reconfiguration for particular cell.
 *****************************************************************************/
MacRetType reConfigPCellRIAndCQIMap ( 
        ULUEContext *ulUEContext_p, 
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink *uplkUEInfo_p
        /*SPR 16855 fix start*/
        ,ULReconfigCQIRIMsgType *cqiRiReconfigRetValue_p,
        UeContextForReconfig *reconfigInfo_p 
        /*SPR 16855 fix end*/
        ,UInt8 internalCellIndex
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    MacRetType resp = MAC_FAILURE;
    UeCellInfo ueCellInfo;
    UInt8  J = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->bandwidthParts;

    ueCellInfo.isPCell = TRUE;
    ueCellInfo.sCellIndex = PUCCH_INVALID;
    resp = reConfigRIAndCQIMap(ulUEContext_p, dwlkUEInfo_p, uplkUEInfo_p
            /*SPR 16855 fix start*/
            ,cqiRiReconfigRetValue_p, reconfigInfo_p 
            /*SPR 16855 fix end*/
            ,&ueCellInfo
            ,&(ulUEContext_p->pucchConfigInfo)
            ,J
            ,internalCellIndex);

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
    return resp;
}
/*****************************************************************************
 * Function Name  : reConfigSCellRIAndCQIMap 
 * Inputs         : ulUEContext_p - UL UE Context
 *                  dwlkUEInfo_p - DL UE Context params.
 *                  uplkUEInfo_p - UL UE Context params. 
 *                  cqiRiReconfigRetValue_p - Bitmap.
 *                  reconfigInfo_p - UE reconfig info
 *                  internalCellIndex - Primary Cell or Secondary Cell index 
 * Outputs        :
 * Returns        :
 * Description    : This is wrapper function that handles both CQI and RI 
 *                  Reconfiguration for particular cell.
 *****************************************************************************/
MacRetType reConfigSCellRIAndCQIMap ( 
        ULUEContext *ulUEContext_p, 
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink *uplkUEInfo_p
        /*SPR 16855 fix start*/
        ,ULReconfigCQIRIMsgType *cqiRiReconfigRetValue_p,
        UeContextForReconfig *reconfigInfo_p 
        /*SPR 16855 fix end*/
        ,UeCellInfo *ueCellInfo_p
        ,UInt8 internalCellIndex
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    MacRetType resp = MAC_FAILURE;
    ULUESCellContext *sCellContext_p = PNULL;
    UInt8  J = 0; 

    if (ulUEContext_p) 
    {
        /*** Coverity 62992 Fix : Start ****/
        sCellContext_p = ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex];
        if (sCellContext_p)
        /*** Coverity 62992 Fix : End ****/
        {
            ueCellInfo_p->isPCell = FALSE;
            ueCellInfo_p->sCellInternalCellIndex = sCellContext_p->internalCellIndex;
            J = cellSpecificParams_g.cellConfigAndInitParams_p[sCellContext_p->internalCellIndex]\
                ->cellParams_p->bandwidthParts;
            resp = reConfigRIAndCQIMap(ulUEContext_p, dwlkUEInfo_p, uplkUEInfo_p
                    /*SPR 16855 fix start*/
                    ,cqiRiReconfigRetValue_p, reconfigInfo_p 
                    /*SPR 16855 fix end*/
                    ,ueCellInfo_p
                    ,&(sCellContext_p->scellpucchConfigInfo)
                    ,J
                    ,internalCellIndex);
        }
        else
        {
            LOG_MAC_MSG(MAC_SCELL_NOT_ACTIVE,LOGERROR,MAC_PUCCH,
                    getCurrentTick(), ueCellInfo_p->sCellIndex, DEFAULT_INT_VALUE, 
                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                    DEFAULT_FLOAT_VALUE, FUNCTION_NAME, "SCell not active");
        }
    }
    else
    {
        LOG_MAC_MSG(MAC_INVALID_CELL_ID,LOGERROR,MAC_PUCCH,
                getCurrentTick(), ueCellInfo_p->sCellIndex, DEFAULT_INT_VALUE, 
                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                DEFAULT_FLOAT_VALUE, FUNCTION_NAME, "Invalid Cell Id");
    }

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
    return resp;
}
/*****************************************************************************
 * Function Name  : reconfigurePCellCqiRiPassiveContext 
 * Inputs         : ulUEContext_p - UL UE Context
 *                  ueIndex - Ue Index
 *                  dwlkUEInfo_p - DL UE Context params.
 *                  uplkUEInfo_p - UL UE Context params. 
 *                  reconfigInfo_p- Reconfiguration Info
 *                  internalCellIndex - Primary Cell or Secondary Cell index 
 * Outputs        : -
 * Returns        : None
 * Description    : This is wrapper function to Reconfigure the CQI Periodic 
 *                  Nodes of passive context according to cell Index passed.
 *****************************************************************************/
 MacRetType reconfigurePCellCqiRiPassiveContext(
        ULUEContext          *ulUEContext_p,
        UInt16               ueIndex,
        PucchConfig          *pucchConfigInfo_p,
        /* +- SPR 17777 */
        UeContextForUplink   *uplkUEInfo_p,
        UeContextForReconfig *reconfigInfo_p,
        UInt8    internalCellIndex
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    MacRetType resp = MAC_FAILURE;
    UeCellInfo  ueCellInfo;
    UInt8  J = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->bandwidthParts;

    ueCellInfo.isPCell = TRUE;
    ueCellInfo.sCellIndex = PUCCH_INVALID;
    resp = reconfigureCqiRiPassiveContext(ulUEContext_p, ueIndex, pucchConfigInfo_p, 
            /* +- SPR 17777 */
            uplkUEInfo_p,reconfigInfo_p
            /* +- SPR 17777 */
            , &ueCellInfo, J
            , internalCellIndex);

 if (SETUP == uplkUEInfo_p->cqiConfigRequestType)
 {
    resp = reconfigureCqiRiPassiveContextAbs(ulUEContext_p, ueIndex, pucchConfigInfo_p, 
            /* +- SPR 17777 */
            uplkUEInfo_p,reconfigInfo_p
            /* +- SPR 17777 */
            , &ueCellInfo, J
            , internalCellIndex);
 }
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit ",__func__);
    return resp;
}

/*****************************************************************************
 * Function Name  : reconfigureSCellCqiRiPassiveContext 
 * Inputs         : ulUEContext_p - UL UE Context
 *                  ueIndex - Ue Index
 *                  uplkUEInfo_p - UL UE Context params. 
 *                  reconfigInfo_p- Reconfiguration Info
 *                  ueCellInfo_p,
 *                  internalCellIndex - Primary Cell or Secondary Cell index 
 * Outputs        : -
 * Returns        : None
 * Description    : This is wrapper function to Reconfigure the CQI Periodic 
 *                  Nodes of passive context according to cell Index passed.
 *****************************************************************************/
 MacRetType reconfigureSCellCqiRiPassiveContext(
        ULUEContext          *ulUEContext_p,
        UInt16               ueIndex,
        /* +- SPR 17777 */
        UeContextForUplink   *uplkUEInfo_p,
        UeContextForReconfig *reconfigInfo_p,
        UeCellInfo * ueCellInfo_p,
        UInt8    internalCellIndex
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entry. ",__func__);
    MacRetType resp = MAC_FAILURE;
    ULUESCellContext *sCellContext_p = PNULL; 
    UInt8  J = 0;

    if (ulUEContext_p) 
    {
        /*** Coverity 62992 Fix : Start ****/
        sCellContext_p = ulUEContext_p->ulUeScellContext_p[ueCellInfo_p->sCellIndex];
        if (sCellContext_p)
        /*** Coverity 62992 Fix : End ****/
        { 
            ueCellInfo_p->isPCell = FALSE;
            ueCellInfo_p->sCellInternalCellIndex = sCellContext_p->internalCellIndex;
            J = cellSpecificParams_g.cellConfigAndInitParams_p[sCellContext_p->internalCellIndex]\
                ->cellParams_p->bandwidthParts;
            resp = reconfigureCqiRiPassiveContext(ulUEContext_p, ueIndex,\
                    &(sCellContext_p->scellpucchConfigInfo),\
                    /* +- SPR 17777 */
                    uplkUEInfo_p,reconfigInfo_p, ueCellInfo_p, J,\
                    /* +- SPR 17777 */
                    internalCellIndex);
		if (SETUP == uplkUEInfo_p->carrierAggrConfig.scellInfo[ueCellInfo_p->sCellInternalCellIndex-1].cqiInfo.cqiConfigRequestType)
		{
		   resp = reconfigureCqiRiPassiveContextAbs(ulUEContext_p, ueIndex,\
                    &(sCellContext_p->scellpucchConfigInfo),\
                    /* +- SPR 17777 */
                    uplkUEInfo_p,reconfigInfo_p, ueCellInfo_p, J,\
                    /* +- SPR 17777 */
                    internalCellIndex);
		}
        }
        else
        {
            LOG_MAC_MSG(MAC_SCELL_NOT_ACTIVE,LOGERROR,MAC_PUCCH,
                    getCurrentTick(), ueCellInfo_p->sCellIndex, DEFAULT_INT_VALUE, 
                    DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                    DEFAULT_FLOAT_VALUE, FUNCTION_NAME, "SCell not active");
        }
    }
    else
    {
        LOG_MAC_MSG(MAC_INVALID_CELL_ID,LOGERROR,MAC_PUCCH,
                getCurrentTick(), ueCellInfo_p->sCellIndex, DEFAULT_INT_VALUE, 
                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                DEFAULT_FLOAT_VALUE, FUNCTION_NAME, "Invalid Cell Id");
    }

    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
    return resp;
}

//#ifdef FAPI_4_0_COMPLIANCE
#if 1
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : fillPucchHarqInfoForMMoreThanOne 
 * Inputs         : ueIndex - Ue Index
 *                  ulschUciInfo_p - ulSch UCI info.
 *                  nPucch - nPUCCH. 
 *                  rbMapNum- RbMap
 *                  resouceAllocCount,ulUeScheduledInfo_p,
 *                  subFrameNum - subFrame
 *                  internalCellIndex - Primary Cell or Secondary Cell index 
 * Outputs        : -
 * Returns        : None
 * Description    : This function fills harq information for M more than 1.
 *****************************************************************************/
void fillPucchHarqInfoForMMoreThanOne(
        UInt16 ueIndex,
        ULSchUciInfo * ulschUciInfo_p,
        UInt32 *nPucch,
        UInt32  rbMapNum,
        UInt8   M,
        UInt8   resouceAllocCount,
        UeScheduledInfo * ulUeScheduledInfo_p,
        UInt32 subFrameNum,
        InternalCellIndex internalCellIndex
        )
{
    UInt16 E  = HARQ_INVALID;
    UInt16 F  = HARQ_INVALID;
    UInt16 G1 = HARQ_INVALID;
    UInt16 G2 = HARQ_INVALID;
    UInt16 c  = HARQ_INVALID;
    UInt16 d  = HARQ_INVALID;
    UInt8 tpcVal_Dci = 0;
    UInt8 cellCount = 0;
    UInt8 sCellResCount = 0;
    UInt8 sCellTb = 0;
    UInt8 ulUEScheduledFlag = INVALID_CCE_INDEX;
    UInt8 prbIndexForDlAckNack1 = 0;
    UInt8 prbIndexForDlAckNack2 = 0;
    ULUEContext      * ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    /* + SPR 13926 Changes */
    DLUEContext      * dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    /* - SPR 13926 Changes */
    DLUESCellContext * dlUESCellContext_p = PNULL;

    /** if eNB doesn' support CA **/  
    ulschUciInfo_p->dlHarqInfo.ackNackMode = ulUEContext_p->tddAckNackFeedbackMode;
    ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 1;
    if (nPucch[0] == 65535)
    {
         nPucch[0] = 0;
    }
    ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = nPucch[0];    
    ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = nPucch[1];    

    if (IS_CA_ENABLED())
    {
        ulschUciInfo_p->dlHarqInfo.ackNackMode = CHANNEL_SELECTION_MODE_2;
        for (cellCount = START_SCELL_INDEX; cellCount <= MAX_NUM_SCELL; cellCount++)
        {
            dlUESCellContext_p = dlUEContext_p->dlUeScellContext_p[cellCount];

            /* SCell Context exists & scell is active */ 
            if (dlUESCellContext_p)
            {
                tpcVal_Dci = dlUESCellContext_p->pucchReqInfo.tpc;
                sCellTb = dlUESCellContext_p->pucchReqInfo.schedulingStatus;
                if(sCellTb )
                {
                    c = ulUEContext_p->pucchFormatR10.n1Pucch[0][tpcVal_Dci];
                    d = ulUEContext_p->pucchFormatR10.n1Pucch[1][tpcVal_Dci];
                    sCellResCount = 2;
                    /** SPR 14204 Fix : Start **/
                    ulUeScheduledInfo_p = ulUEContext_p->
                        ulUeScheduledInfoPointerArray[internalCellIndex][subFrameNum];
                    /** SPR 14204 Fix : End **/
                    if (ulUeScheduledInfo_p)
                    {    
                        ulUEScheduledFlag = ulUeScheduledInfo_p->ulCCEIndex;
                    }    
#ifndef PUCCH_RESERVE_ALWAYS
                    if (INVALID_CCE_INDEX == ulUEScheduledFlag)
#else
                    /* SPR 6825 Changes starts   */
                    if ((INVALID_CCE_INDEX == ulUEScheduledFlag) &&
                            (MAC_SS_PDU_UCI_CQI_HARQ != ulschUciInfo_p->pduType))
                    /* SPR 6825 Changes ends   */
#endif
                    {
                            if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex (
                                c, UCI_FORMAT_ONE_B,&prbIndexForDlAckNack1,
                                &prbIndexForDlAckNack2, internalCellIndex)
                               )
                            {
                                /* Review comment fix start GG13 */
                                LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                                getCurrentTick(),ueIndex,c,d,
                                UCI_FORMAT_ONE_B,
                                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
                                "Invalid Configuraton For PUCCH Resources For DL Ack-Nack");
                                return ;
                                /* Review comment fix end GG13 */
                            }
                            allocateResourcesForPUCCHReports(
                                prbIndexForDlAckNack1, prbIndexForDlAckNack2,
                                rbMapNum, internalCellIndex);
                    }

                }
                switch(M)
                {
                    case 2:
                         ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = (sCellResCount + resouceAllocCount);
                         ulschUciInfo_p->dlHarqInfo.harqSize = (M * 2);
                             E = nPucch[0];
                             F = nPucch[1];
                             ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = E;    
                             ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = F;    
                             ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = c;    
                             ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = d;

                    break;
                    case 3:
                    case 4:
                         ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = (sCellResCount + resouceAllocCount);
                         ulschUciInfo_p->dlHarqInfo.harqSize = (M * 2);
                             G1 = nPucch[0];
                             G2 = nPucch[1];
                             ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = G1;    
                             ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = G2;    
                             ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = c;    
                             ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = d;

                    break;
                }

            }

        }
		
    }
}
#endif
#endif


//#ifdef FAPI_4_0_COMPLIANCE
#if 1
/*****************************************************************************
 * Function Name  : fillPucchHarqInfo 
 * Inputs         : ueIndex 
 *                  ulschUciInfo_p - pointer to UCI info structure.
 *                  nuberOfInfoPresent - number of PDU.
 *                  nPucch - resourceIndex
 *                  rbMapNum - rb Map num
 *                  puschReserveOnlyFlagForSingleUE -set if only single UE
 *                  subFrameNum - SF.
 *                  pdcchULOutputInfo_p - Pointer to PdcchULOutputInfo 
 *                  internalCellIndex - cellIndex of PCell
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This is wrapper function that fills harqInfo for UCI packet 
 *                  according to transmodes in different cell.
 *****************************************************************************/
MacRetType fillPucchHarqInfo(
        UInt16 ueIndex,
        ULSchUciInfo * ulschUciInfo_p,
        UInt32 nPucch,
        /** SPR 12457 Fix : Start**/
        UInt32  rbMapNum,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif
        /* +- SPR 17777 */
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        /* +- SPR 17777 */
#else
        UInt8 pCellTb,
#endif
        /** SPR 12457 Fix : End **/
        UInt32 subFrameNum,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex
        )
{
    UInt8           tpcVal_Dci = 0;
    UInt8           cellCount = 0;
    MacRetType      resp = MAC_FAILURE;
    ULUEContext     *ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    DLUEContext     *dlUEContext_p = PNULL;
    DLUESCellContext *dlUESCellContext_p = PNULL;
    UInt8 numOfTxAnteenas = 0;
    UInt8 tbsSupportedCell[MAX_NUM_CELL] = {0,0};
    /** SPR 12457 Fix : Start**/
#ifdef TDD_CONFIG   
    UeScheduledInfo * ulUeScheduledInfo_p = PNULL;
#endif
    UInt8 ulUEScheduledFlag = 0;
    /** SPR 12457 Fix : End **/
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
#ifdef FDD_CONFIG
    UInt8 dlHarqTimerNodeArrayIndex = 0;
    UInt8           pCellHarqIndex = 0;
    DLHarqTimerEntityList *dlHarqEntityTimerList_p = PNULL;
    dlHarqTimerNodeArrayIndex = dlUEContext_p->dlHarqContext_p->\
                                dlHarqTimerNodeArrrayIndex[subFrameNum];
    dlHarqEntityTimerList_p = &(dlHarqGlobals_g[internalCellIndex].\
            dlHarqTimerEntityList[subFrameNum]);
    pCellHarqIndex = dlHarqEntityTimerList_p->\
            dlHarqTimerNodeArr[dlHarqTimerNodeArrayIndex].harqProcessId;
#endif
    
    /* Filling default info if non ca ue */
    /*ca-tdd pucch changes start*/
    ulschUciInfo_p->dlHarqInfo.ackNackMode = FORMAT_1A_1B;
#ifdef FDD_CONFIG
    //ulschUciInfo_p->dlHarqInfo.ackNackMode = FORMAT_1A_1B;
    lteMacSsCalcHarqSize(
            /*Coverity_31965 Fix Start*/
            &(ulschUciInfo_p->dlHarqInfo.harqSize),
            /*Coverity_31965 Fix End*/
            pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].dlDciFormat);                    
#else
        ulschUciInfo_p->dlHarqInfo.ackNackMode = ulUEContext_p->tddAckNackFeedbackMode;
#endif
    /*ca-tdd pucch changes end*/
    ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 1;
    ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = nPucch;    
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    /* SPR 20027 Fix Start */
    ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = HARQ_INVALID;
    ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = HARQ_INVALID;
    ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;
    /* SPR 20027 Fix End */
    /* SPR 19679: TDD HARQ Multiplexing Changes End */

    if (IS_CA_ENABLED())
    {
        /** SPR 12457 Fix : Start**/
#ifdef FDD_CONFIG
        ulUEScheduledFlag =
            pdcchULOutputInfo_p->directIndexingUEInfo[ueIndex].ulCCEIndex;
#elif TDD_CONFIG
        /** SPR 14204 Fix : Start **/
        ulUeScheduledInfo_p = ulUEContext_p->
            ulUeScheduledInfoPointerArray[internalCellIndex][subFrameNum];
        /** SPR 14204 Fix : End **/
        if (ulUeScheduledInfo_p)
        {    
            ulUEScheduledFlag = ulUeScheduledInfo_p->ulCCEIndex;
        }    
#endif
        /** SPR 12457 Fix : End **/

        numOfTxAnteenas =
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                ->cellParams_p->numOfTxAnteenas;

        TBS_SUPPORTED_IN_CELL(dlUEContext_p->dlMIMOInfo.transmissionMode, 
                                numOfTxAnteenas, internalCellIndex)

        /* Running loop for all Secondary Cell */
        for (cellCount = START_SCELL_INDEX; cellCount <= MAX_NUM_SCELL; cellCount++)
        {
            dlUESCellContext_p = dlUEContext_p->dlUeScellContext_p[cellCount];

            /* SCell Context exists & scell is active */ 
            if (dlUESCellContext_p)
            {
                /** SPR 12457 Fix : Start**/
                tpcVal_Dci = dlUESCellContext_p->pucchReqInfo.tpc; 
                /** SPR 12457 Fix : End **/
                /** SPR 11916 Fix : Start**/
                numOfTxAnteenas =
                        cellSpecificParams_g.cellConfigAndInitParams_p
                        [dlUESCellContext_p->internalCellIndex]->cellParams_p->numOfTxAnteenas;

                TBS_SUPPORTED_IN_CELL(dlUESCellContext_p->ScelldlMIMOInfo.transmissionMode, \
                        numOfTxAnteenas, dlUESCellContext_p->internalCellIndex)
                /** SPR 11916 Fix : End **/
                /*  Checking which cell supports how many TB & fill harq info accordingly 
                    eg. : PCell Supports 1Tb SCell Supports 2Tb 
                */
                /* When in PCell SISO is supported */
                if (ONE_TB_SUPPORTED == tbsSupportedCell[internalCellIndex])
                {
                    /* When in SCell SISO is supported */
                    /** SPR 11206 Fix : Start ***/
                    if(ONE_TB_SUPPORTED == 
                        tbsSupportedCell[dlUESCellContext_p->internalCellIndex]) 
                    /** SPR 11206 Fix : End ***/
                    {
                        /** SPR 12457 Fix : Start**/
                        resp = fillPucchHarqInfoForPCell1SCell1TB(
                                dlUEContext_p, ulUEContext_p, ulschUciInfo_p,
                                nPucch,
#ifdef FDD_CONFIG
                                pCellHarqIndex,
#else
                                pCellTb, 
#endif 
                                tpcVal_Dci, rbMapNum, ulUEScheduledFlag,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
                                puschReserveOnlyFlagForSingleUE, 
#endif
#endif
                                cellCount, internalCellIndex);
                        /** SPR 12457 Fix : End **/
                    }
                    /* When in SCell MIMO is supported */
                    else
                    {
                        /** SPR 12457 Fix : Start**/
                        resp = fillPucchHarqInfoForPCell1SCell2TB(
                                dlUEContext_p, ulUEContext_p, ulschUciInfo_p,
                                nPucch,
#ifdef FDD_CONFIG
                                pCellHarqIndex,
#else
                                pCellTb, 
#endif 
                                tpcVal_Dci, rbMapNum, ulUEScheduledFlag,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
                                puschReserveOnlyFlagForSingleUE, 
#endif
#endif
                                cellCount, internalCellIndex);
                        /** SPR 12457 Fix : End **/
                    }
                }
                /* When in PCell MIMO is supported */
                else
                {
                    /* When in SCell SISO is supported */
                    if(ONE_TB_SUPPORTED == 
                            tbsSupportedCell[dlUESCellContext_p->internalCellIndex]) 
                    {
                        /** SPR 12457 Fix : Start**/
                        resp = fillPucchHarqInfoForPCell2SCell1TB(
                                dlUEContext_p, ulUEContext_p, ulschUciInfo_p,
                                nPucch, 
#ifdef FDD_CONFIG
                                pCellHarqIndex,
#else
                                pCellTb, 
#endif 
                                tpcVal_Dci, rbMapNum, ulUEScheduledFlag,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
                                puschReserveOnlyFlagForSingleUE, 
#endif
#endif
                                cellCount, internalCellIndex);
                        /** SPR 12457 Fix : End **/
                    }
                    /* When in SCell MIMO is supported */
                    else
                    {
                        /** SPR 12457 Fix : Start**/
                        resp = fillPucchHarqInfoForPCell2SCell2TB(
                                dlUEContext_p ,ulUEContext_p, ulschUciInfo_p,
                                 nPucch, 
#ifdef FDD_CONFIG
                                pCellHarqIndex,
#else
                                pCellTb, 
#endif 
                                tpcVal_Dci, rbMapNum, ulUEScheduledFlag,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
                                puschReserveOnlyFlagForSingleUE,
#endif
#endif
                                cellCount, internalCellIndex);
                        /** SPR 12457 Fix : End **/
                    }
                }
            }
        }
    }

    return resp;
}
#endif

//#ifdef FAPI_4_0_COMPLIANCE
#if 1
/*****************************************************************************
 * Function Name  : fillPucchHarqInfoForPCell1SCell1TB
 * Inputs         : dlUEContext_p - downlink context of UE.
 *                  ulUEContext_p - uplink context of UE.
 *                  ulschUciInfo_p - pointer to UCI info structure.
 *                  nPucch - resourceIndex
 *                  pCellHarqIndex - harqIndex at PCell.
 *                  tpcVal_Dci - tpc value send to UE in DCI.
 *                  rbMapNum - rbmapNum
 *                  ulUEScheduledFlag - if scheduled in uplink
 *                  puschReserveOnlyFlagForSingleUE - set if single UE
 *                  cellIndex - scell internalCellIndex
 *                  internalCellIndex - pcell internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This is function that fills harqInfo for UCI packet if both
 *                  cells supports 1TB transmode.
 *****************************************************************************/
MacRetType fillPucchHarqInfoForPCell1SCell1TB(
        DLUEContext     *dlUEContext_p,
        ULUEContext     *ulUEContext_p,
        ULSchUciInfo * ulschUciInfo_p,
        UInt32 nPucch,
#ifdef FDD_CONFIG
        UInt8 pCellHarqIndex,
#else
        UInt8 pCellTbNum,
#endif
        UInt8 tpcVal_Dci,
        /** SPR 12457 Fix : Start**/
        UInt32  rbMapNum,
        UInt8 ulUEScheduledFlag,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif
#endif
        UInt8 cellIndex,
        InternalCellIndex internalCellIndex
        /** SPR 12457 Fix : End **/
        )
{
    UInt16 a = 0;
    UInt16 c = 0;
    UInt8 pCellTb = 0;
    UInt8 sCellTb = 0;
    /** SPR 12457 Fix : Start**/
    UInt8 prbIndexForDlAckNack1 = 0;
    UInt8 prbIndexForDlAckNack2 = 0;
    /** SPR 12457 Fix : End **/
    MacRetType resp = MAC_SUCCESS;
    DLUESCellContext *dlUESCellContext_p = dlUEContext_p->dlUeScellContext_p[cellIndex];

    /* HARQ Information */
    /*  Refer :- Spec 36.213, Table 10.1.2.3.1-2 */

    //a = nPucch;
    a = ((INVALID_CCE_INDEX != nPucch) ? nPucch : 0xFFFF);
    c = ulUEContext_p->pucchFormatR10.n1Pucch[0][tpcVal_Dci]; 

    /*  Refer :- Spec 36.213, Table 10.1.2.3.1-1 */
    /*ca-tdd pucch change start*/ 
#ifdef TDD_CONFIG
    ulschUciInfo_p->dlHarqInfo.ackNackMode = CHANNEL_SELECTION_MODE_2;
#else
    ulschUciInfo_p->dlHarqInfo.ackNackMode = CHANNEL_SELECTION;  
#endif
    /*ca-tdd pucch change end*/ 
    ulschUciInfo_p->dlHarqInfo.harqSize = ONE_TB_SUPPORTED + ONE_TB_SUPPORTED;

    /* Calculating Number of valid TB in PCell */
    /*Coverity ID <65219> Fix Changes Start*/
#ifdef FDD_CONFIG
    if ((dlUEContext_p->dlHarqContext_p) && (8 > pCellHarqIndex))
    /*Coverity ID <65219> Fix Changes End*/
    {
        if (dlUEContext_p->dlHarqContext_p->harqProcess[pCellHarqIndex].isTBOneValid)
            pCellTb = SINGLE_TB;
    }
#else
    pCellTb = pCellTbNum;
#endif

    /* Calculating Number of valid TB in SCell */
    if (dlUESCellContext_p)
    {
        /** SPR 12457 Fix : Start**/
        sCellTb = dlUESCellContext_p->pucchReqInfo.schedulingStatus;
        /** SPR 12457 Fix : End **/
    }

    if (PUCCH_INVALID == sCellTb)
    {
        return MAC_FAILURE;
    }

    /* Number of Tb in PCell is 1 & in SCell is 0 */
    if (SINGLE_TB == pCellTb && NO_TB == sCellTb)
    {
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = a;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = HARQ_INVALID;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = HARQ_INVALID;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;
        ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 1;
    }
    /* Number of Tb in PCell is 1 & in SCell is 1 */
    else 
    {  
        /** SPR 1245_12492 Fix : Start**/
#ifndef PUCCH_RESERVE_ALWAYS
        if (INVALID_CCE_INDEX == ulUEScheduledFlag)
#else
            /* SPR 6825 Changes starts   */
        if ((
#ifdef FDD_CONFIG
            (!puschReserveOnlyFlagForSingleUE )||
#endif
                (INVALID_CCE_INDEX == ulUEScheduledFlag)) &&
                (MAC_SS_PDU_UCI_CQI_HARQ != ulschUciInfo_p->pduType))
            /* SPR 6825 Changes ends   */
#endif
        {
            if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex (
                        c, UCI_FORMAT_ONE_B,&prbIndexForDlAckNack1,
                        &prbIndexForDlAckNack2, internalCellIndex)
               )
            {
                /* Review comment fix start GG13 */
                LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                        getCurrentTick(),ulUEContext_p->ueIndex,c,
                        UCI_FORMAT_ONE_B,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
                        "Invalid Configuraton For PUCCH Resources For DL Ack-Nack");
                return MAC_FAILURE;
                /* Review comment fix end GG13 */
            }
            allocateResourcesForPUCCHReports(
                    prbIndexForDlAckNack1, prbIndexForDlAckNack2,
                    rbMapNum, internalCellIndex);
        }
        /** SPR 12457_12492 Fix : End **/

        if (SINGLE_TB == pCellTb && SINGLE_TB == sCellTb)
        {
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = a;    
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = c;    
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = HARQ_INVALID;    
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;
            ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 2;
        }
        /* Number of Tb in PCell is 0 & in SCell is 1 */
        else if (NO_TB == pCellTb && SINGLE_TB == sCellTb)
        {
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = HARQ_INVALID;    
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = c;    
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = HARQ_INVALID;    
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;
            ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 1;
        }
        else
        {
            resp = MAC_FAILURE;
        }
    }
    /* mac thread changes + */
        /* mac thread changes - */

    return resp;
}

/*****************************************************************************
 * Function Name  : fillPucchHarqInfoForPCell2SCell1TB
 * Inputs         : dlUEContext_p - downlink context of UE.
 *                  ulUEContext_p - uplink context of UE.
 *                  ulschUciInfo_p - pointer to UCI info structure.
 *                  nPucch - resourceIndex
 *                  pCellHarqIndex - harqIndex at PCell.
 *                  tpcVal_Dci - tpc value send to UE in DCI.
 *                  rbMapNum - rbmapNum
 *                  ulUEScheduledFlag - if scheduled in uplink
 *                  puschReserveOnlyFlagForSingleUE - set if single UE
 *                  cellIndex - scell internalCellIndex
 *                  internalCellIndex - pcell internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This is function that fills harqInfo for UCI packet if PCell
 *                  cells supports 2TB & SCell supports 1TB transmode.
 *****************************************************************************/
 MacRetType fillPucchHarqInfoForPCell2SCell1TB(
        DLUEContext     *dlUEContext_p,
        ULUEContext     *ulUEContext_p,
        ULSchUciInfo * ulschUciInfo_p,
        UInt32 nPucch,
#ifdef FDD_CONFIG
        UInt8 pCellHarqIndex,
#else
        UInt8 pCellTbNum,
#endif
        UInt8 tpcVal_Dci,
        /** SPR 12457 Fix : Start**/
        UInt32 rbMapNum,
        UInt8 ulUEScheduledFlag,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif
#endif
        UInt8 cellIndex,
        InternalCellIndex internalCellIndex
        /** SPR 12457 Fix : End **/
        )
{
    UInt16 a = 0;
    UInt16 b = 0;
    UInt16 c = 0;
    UInt8 pCellTb = 0;
    UInt8 sCellTb = 0;
    /** SPR 12457 Fix : Start**/
    UInt8 prbIndexForDlAckNack1 = 0;
    UInt8 prbIndexForDlAckNack2 = 0;
    /** SPR 12457 Fix : End **/
    MacRetType resp = MAC_SUCCESS;
    DLUESCellContext *dlUESCellContext_p = dlUEContext_p->dlUeScellContext_p[cellIndex];

    /* HARQ Information */
    /*  Refer :- Spec 36.213, Table 10.1.2.3.1-2 */
    //a = nPucch;
    a = ((INVALID_CCE_INDEX != nPucch) ? nPucch : 0xFFFF);
    b = ((0xFFFF != a) ? (a + 1) : 0xFFFF);
    c = ulUEContext_p->pucchFormatR10.n1Pucch[0][tpcVal_Dci]; 

    /*  Refer :- Spec 36.213, Table 10.1.2.3.1-1 */
    /*ca-tdd pucch change start*/ 
#ifdef TDD_CONFIG
    ulschUciInfo_p->dlHarqInfo.ackNackMode = CHANNEL_SELECTION_MODE_2;
#else
    ulschUciInfo_p->dlHarqInfo.ackNackMode = CHANNEL_SELECTION;  
#endif
    /*ca-tdd pucch change end*/ 
    ulschUciInfo_p->dlHarqInfo.harqSize = TWO_TB_SUPPORTED + ONE_TB_SUPPORTED ;

    /* Calculating Number of valid TB in PCell */
    /*Coverity ID <65219> Fix Changes Start*/
#ifdef FDD_CONFIG
    if (dlUEContext_p->dlHarqContext_p && (8 > pCellHarqIndex))
    /*Coverity ID <65219> Fix Changes End*/
    {
        if(dlUEContext_p->dlHarqContext_p->harqProcess[pCellHarqIndex].isTBOneValid && \
                dlUEContext_p->dlHarqContext_p->harqProcess[pCellHarqIndex].isTBTwoValid)
        {
            pCellTb = TWO_TB;
        }
        else if (dlUEContext_p->dlHarqContext_p->harqProcess[pCellHarqIndex].isTBOneValid)
        {
            pCellTb = SINGLE_TB;
        }
    }
#else
    pCellTb = pCellTbNum;
#endif


    /* Calculating Number of valid TB in SCell */
    if (dlUESCellContext_p)
    {
        /** SPR 12457 Fix : Start**/
        sCellTb = dlUESCellContext_p->pucchReqInfo.schedulingStatus;
        /** SPR 12457 Fix : End **/
    }

    if (PUCCH_INVALID == sCellTb)
    {
        return MAC_FAILURE;
    }

    /* Number of Tb in SCell is NO_TB means UE is not scheduled in SCell*/
    if (NO_TB == sCellTb)
    {
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = a;    
        /* SPR 14081 fix start */
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = HARQ_INVALID;
        /* SPR 14081 fix end */
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = HARQ_INVALID;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;
        /* SPR 14081 fix start */
        ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 1;
        /* SPR 14081 fix end */
    }
    /* Number of Tb in PCell is >1 */
    else 
    {
        /** SPR 12457_12492 Fix : Start**/
#ifndef PUCCH_RESERVE_ALWAYS
        if ((INVALID_CCE_INDEX == ulUEScheduledFlag) &&
            (MAC_SS_PDU_UCI_CQI_HARQ != ulschUciInfo_p->pduType))
#else
            /* SPR 6825 Changes starts   */
        if ((
#ifdef FDD_CONFIG
            (!puschReserveOnlyFlagForSingleUE )||
#endif
                (INVALID_CCE_INDEX == ulUEScheduledFlag)) &&
                (MAC_SS_PDU_UCI_CQI_HARQ != ulschUciInfo_p->pduType))
                
            /* SPR 6825 Changes ends   */
#endif
        {
            if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex (
                        c, UCI_FORMAT_ONE_B,&prbIndexForDlAckNack1,
                        &prbIndexForDlAckNack2, internalCellIndex)
                )
             {
                 /* Review comment fix start GG13 */
                 LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                         getCurrentTick(),ulUEContext_p->ueIndex,c,
                         UCI_FORMAT_ONE_B,DEFAULT_INT_VALUE,
                         DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                         DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
                         "Invalid Configuraton For PUCCH Resources For DL Ack-Nack");
                 return MAC_FAILURE;
                /* Review comment fix end GG13 */
            }
            allocateResourcesForPUCCHReports(
                    prbIndexForDlAckNack1, prbIndexForDlAckNack2,
                    rbMapNum, internalCellIndex);
        }
        /** SPR 12457_12492 Fix : End **/

        if (NO_TB < pCellTb)
        {
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = a;    
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = b;    
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = c;    
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;
            ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 3;
        }
        /* Number of Tb in PCell is 0 & in SCell is 1 */
        else
        {
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = HARQ_INVALID;    
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = HARQ_INVALID;    
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = c;    
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;
            ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 1;
        }
    }

    return resp;
}

/*****************************************************************************
 * Function Name  : fillPucchHarqInfoForPCell1SCell2TB
 * Inputs         : dlUEContext_p - downlink context of UE.
 *                  ulUEContext_p - uplink context of UE.
 *                  ulschUciInfo_p - pointer to UCI info structure.
 *                  nPucch - resourceIndex
 *                  pCellHarqIndex - harqIndex at PCell.
 *                  tpcVal_Dci - tpc value send to UE in DCI.
 *                  rbMapNum - rbmapNum
 *                  ulUEScheduledFlag - if scheduled in uplink
 *                  puschReserveOnlyFlagForSingleUE - set if single UE
 *                  cellIndex - scell internalCellIndex
 *                  internalCellIndex - pcell internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This is function that fills harqInfo for UCI packet if PCell
 *                  cells supports 1TB & SCell supports 2TB transmode.
 *****************************************************************************/
 MacRetType fillPucchHarqInfoForPCell1SCell2TB(
        DLUEContext     *dlUEContext_p,
        ULUEContext     *ulUEContext_p,
        ULSchUciInfo * ulschUciInfo_p,
        UInt32 nPucch,
#ifdef FDD_CONFIG
        UInt8 pCellHarqIndex,
#else
        UInt8 pCellTbNum,
#endif
        UInt8 tpcVal_Dci,
        /** SPR 12457 Fix : Start**/
        UInt32 rbMapNum,
        UInt8 ulUEScheduledFlag,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif
#endif
        UInt8 cellIndex,
        InternalCellIndex internalCellIndex
        /** SPR 12457 Fix : End **/
        )
{
    UInt16 a = 0;
    UInt16 c = 0;
    UInt16 d = 0;
    UInt8 pCellTb = 0;
    UInt8 sCellTb = 0;
    /** SPR 12457 Fix : Start**/
    UInt8 prbIndexForDlAckNack1 = 0;
    UInt8 prbIndexForDlAckNack2 = 0;
    /** SPR 12457 Fix : End **/
    MacRetType resp = MAC_SUCCESS;
    DLUESCellContext *dlUESCellContext_p = dlUEContext_p->dlUeScellContext_p[cellIndex];

    /* HARQ Information */
    /*  Refer :- Spec 36.213, Table 10.1.2.3.1-2 */
    //a = nPucch;
    a = ((INVALID_CCE_INDEX != nPucch) ? nPucch : 0xFFFF);
    c = ulUEContext_p->pucchFormatR10.n1Pucch[0][tpcVal_Dci]; 
    d = ulUEContext_p->pucchFormatR10.n1Pucch[1][tpcVal_Dci];

    /*  Refer :- Spec 36.213, Table 10.1.2.3.1-1 */
    /*ca-tdd pucch change start*/ 
#ifdef TDD_CONFIG
    ulschUciInfo_p->dlHarqInfo.ackNackMode = CHANNEL_SELECTION_MODE_2;
#else
    ulschUciInfo_p->dlHarqInfo.ackNackMode = CHANNEL_SELECTION;  
#endif
    /*ca-tdd pucch change end*/ 
    ulschUciInfo_p->dlHarqInfo.harqSize = ONE_TB_SUPPORTED + TWO_TB_SUPPORTED;
    ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 2;

    /* Calculating Number of valid TB in PCell */
    /*Coverity ID <65221> Fix Changes Start*/
#ifdef FDD_CONFIG
    if ((dlUEContext_p->dlHarqContext_p) && (8 > pCellHarqIndex))
    /*Coverity ID <65221> Fix Changes End*/
    {
        if (dlUEContext_p->dlHarqContext_p->harqProcess[pCellHarqIndex].isTBOneValid)
            pCellTb = SINGLE_TB;
    }
#else
    pCellTb = pCellTbNum;
#endif

    /* Calculating Number of valid TB in SCell */
    if (dlUESCellContext_p)
    {
        /** SPR 12457 Fix : Start**/
        sCellTb = dlUESCellContext_p->pucchReqInfo.schedulingStatus;
        /** SPR 12457 Fix : End **/
    }

    if (PUCCH_INVALID == sCellTb)
    {
        return MAC_FAILURE;
    }

    /** Allocating resources for c & d **/
    /** SPR 12457_12492 Fix : Start**/
    if ((NO_TB < sCellTb) && (TWO_TB >= sCellTb) && 
        (MAC_SS_PDU_UCI_CQI_HARQ != ulschUciInfo_p->pduType))
    {
#ifndef PUCCH_RESERVE_ALWAYS
        if (INVALID_CCE_INDEX == ulUEScheduledFlag)
#else
        /* SPR 6825 Changes starts   */
        if(
#ifdef FDD_CONFIG
            (!puschReserveOnlyFlagForSingleUE )||
#endif
                (INVALID_CCE_INDEX == ulUEScheduledFlag))
            /* SPR 6825 Changes ends   */
#endif
        {
            if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex (
                        c, UCI_FORMAT_ONE_B,&prbIndexForDlAckNack1,
                        &prbIndexForDlAckNack2, internalCellIndex)
               )
            {
                /* Review comment fix start GG13 */
                LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                        getCurrentTick(),ulUEContext_p->ueIndex,c,
                        UCI_FORMAT_ONE_B,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
                        "Invalid Configuraton For PUCCH Resources For DL Ack-Nack");
                return MAC_FAILURE;
                /* Review comment fix end GG13 */
            }
            allocateResourcesForPUCCHReports(
                    prbIndexForDlAckNack1, prbIndexForDlAckNack2,
                    rbMapNum, internalCellIndex);

            if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex (
                        d, UCI_FORMAT_ONE_B,&prbIndexForDlAckNack1,
                        &prbIndexForDlAckNack2, internalCellIndex)
               )
            {
                /* Review comment fix start GG13 */
                LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                        getCurrentTick(),ulUEContext_p->ueIndex,d,
                        UCI_FORMAT_ONE_B,DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
                        "Invalid Configuraton For PUCCH Resources For DL Ack-Nack");
                return MAC_FAILURE;
                /* Review comment fix end GG13 */
            }
            allocateResourcesForPUCCHReports(
                    prbIndexForDlAckNack1, prbIndexForDlAckNack2,
                    rbMapNum, internalCellIndex);
        }
    }
    /** SPR 12457_12492 Fix : End **/

    /* Number of Tb in PCell is 0 means UE is not scheduled in PCell */
    if (NO_TB == pCellTb)
    {
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = c;    
	/* SPR 14081 fix start */
	ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = HARQ_INVALID;
	/* SPR 14081 fix end */
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = HARQ_INVALID;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;
	/* SPR 14081 fix start */
	ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 1;
	/* SPR 14081 fix end */

    }
    /* Number of Tb in SCell is > 1 */
    else if (NO_TB < sCellTb)
    {
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = c;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = d;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = a;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;
        ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 3;
    }
    /* Number of Tb in PCell is 1 & in SCell is 0 */
    else
    {
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = HARQ_INVALID;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = HARQ_INVALID;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = a;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;
	/** SPR 12144 Fix : Start **/
    	ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 1;
	/** SPR 12144 Fix : End **/
    }

    return resp;
}

/*****************************************************************************
 * Function Name  : fillPucchHarqInfoForPCell2SCell2TB
 * Inputs         : dlUEContext_p - downlink context of UE.
 *                  ulUEContext_p - uplink context of UE.
 *                  ulschUciInfo_p - pointer to UCI info structure.
 *                  nPucch - resourceIndex
 *                  pCellHarqIndex - harqIndex at PCell.
 *                  tpcVal_Dci - tpc value send to UE in DCI.
 *                  rbMapNum - rbmapNum
 *                  ulUEScheduledFlag - if scheduled in uplink
 *                  puschReserveOnlyFlagForSingleUE - set if single UE
 *                  cellIndex - scell internalCellIndex
 *                  internalCellIndex - pcell internalCellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This is function that fills harqInfo for UCI packet if both
 *                  cells supports 2TB transmode.
 *****************************************************************************/
MacRetType fillPucchHarqInfoForPCell2SCell2TB(
        DLUEContext     *dlUEContext_p,
        ULUEContext     *ulUEContext_p,
        ULSchUciInfo * ulschUciInfo_p,
        UInt32 nPucch,
#ifdef FDD_CONFIG
        UInt8 pCellHarqIndex,
#else
        UInt8 pCellTbNum,
#endif
        UInt8 tpcVal_Dci,
        /** SPR 12457 Fix : Start**/
        UInt32 rbMapNum,
        UInt8 ulUEScheduledFlag,
#ifdef FDD_CONFIG
#ifdef PUCCH_RESERVE_ALWAYS
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif
#endif
        UInt8 cellIndex,
        InternalCellIndex internalCellIndex
        /** SPR 12457 Fix : End **/
        )
{
    UInt16 a = 0;
    UInt16 b = 0;
    UInt16 c = 0;
    UInt16 d = 0;
    UInt8 pCellTb = 0;
    UInt8 sCellTb = 0;
    /** SPR 12457 Fix : Start**/
    UInt8 prbIndexForDlAckNack1 = 0;
    UInt8 prbIndexForDlAckNack2 = 0;
    /** SPR 12457 Fix : End **/
    MacRetType resp = MAC_SUCCESS;
    DLUESCellContext *dlUESCellContext_p = dlUEContext_p->dlUeScellContext_p[cellIndex];

    /* HARQ Information */
    /*  Refer :- Spec 36.213, Table 10.1.2.3.1-2 */
    //a = nPucch;
    a = ((INVALID_CCE_INDEX != nPucch) ? nPucch : 0xFFFF);
    b = ((0xFFFF != a) ? (a + 1) : 0xFFFF);
    c = ulUEContext_p->pucchFormatR10.n1Pucch[0][tpcVal_Dci]; 
    d = ulUEContext_p->pucchFormatR10.n1Pucch[1][tpcVal_Dci];

    /*  Refer :- Spec 36.213, Table 10.1.2.3.1-1 */
    /*ca-tdd pucch change start*/ 
#ifdef TDD_CONFIG
    ulschUciInfo_p->dlHarqInfo.ackNackMode = CHANNEL_SELECTION_MODE_2;
#else
    ulschUciInfo_p->dlHarqInfo.ackNackMode = CHANNEL_SELECTION;  
#endif
    /*ca-tdd pucch change end*/ 
    ulschUciInfo_p->dlHarqInfo.harqSize = (TWO_TB_SUPPORTED + TWO_TB_SUPPORTED);
    /** SPR 12144 Fix : Start **/
    ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 4;
    /** SPR 12144 Fix : End **/

    /* Calculating Number of valid TB in PCell */
    /*Coverity ID <65223> Fix Changes Start*/
#ifdef FDD_CONFIG
    if (dlUEContext_p->dlHarqContext_p && (8 > pCellHarqIndex))
    /*Coverity ID <65223> Fix Changes End*/
    {
        if(dlUEContext_p->dlHarqContext_p->harqProcess[pCellHarqIndex].isTBOneValid && \
                dlUEContext_p->dlHarqContext_p->harqProcess[pCellHarqIndex].isTBTwoValid)
        {
            pCellTb = TWO_TB;
        }
        else if (dlUEContext_p->dlHarqContext_p->harqProcess[pCellHarqIndex].isTBOneValid)
        {
            pCellTb = SINGLE_TB;
        }
    }
#else
    pCellTb = pCellTbNum;
#endif

    /* Calculating Number of valid TB in SCell */
    if (dlUESCellContext_p)
    {
        /** SPR 12457 Fix : Start**/
        sCellTb = dlUESCellContext_p->pucchReqInfo.schedulingStatus;
        /** SPR 12457 Fix : End **/
    }

    if (PUCCH_INVALID == sCellTb)
    {
        return MAC_FAILURE;
    }

    /** Allocating resources for c & d **/
    /** SPR 12457_12492 Fix : Start**/
    if ((NO_TB < sCellTb) && (TWO_TB >= sCellTb) && 
        (MAC_SS_PDU_UCI_CQI_HARQ != ulschUciInfo_p->pduType))
    {
#ifndef PUCCH_RESERVE_ALWAYS
        if (INVALID_CCE_INDEX == ulUEScheduledFlag)
#else
        /* SPR 6825 Changes starts   */
        if(
#ifdef FDD_CONFIG
            (!puschReserveOnlyFlagForSingleUE )||
#endif
                (INVALID_CCE_INDEX == ulUEScheduledFlag))
             /* SPR 6825 Changes ends   */
#endif
        {
             if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex (
                        c, UCI_FORMAT_ONE_B,&prbIndexForDlAckNack1,
                        &prbIndexForDlAckNack2, internalCellIndex)
                )
             {
                 /* Review comment fix start GG13 */
                 LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                         getCurrentTick(),ulUEContext_p->ueIndex,c,
                         UCI_FORMAT_ONE_B,DEFAULT_INT_VALUE,
                         DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                         DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
                         "Invalid Configuraton For PUCCH Resources For DL Ack-Nack");
                return MAC_FAILURE;
                /* Review comment fix end GG13 */
            }
            allocateResourcesForPUCCHReports(
                    prbIndexForDlAckNack1, prbIndexForDlAckNack2,
                    rbMapNum, internalCellIndex);

            if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex (
                         d, UCI_FORMAT_ONE_B,&prbIndexForDlAckNack1,
                         &prbIndexForDlAckNack2, internalCellIndex)
                )
            {
                 /* Review comment fix start GG13 */
                 LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                         getCurrentTick(),ulUEContext_p->ueIndex,d,
                         UCI_FORMAT_ONE_B,DEFAULT_INT_VALUE,
                         DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE,
                         DEFAULT_FLOAT_VALUE, FUNCTION_NAME,
                         "Invalid Configuraton For PUCCH Resources For DL Ack-Nack");
                 return MAC_FAILURE;
                 /* Review comment fix end GG13 */
            }
            allocateResourcesForPUCCHReports(
                     prbIndexForDlAckNack1, prbIndexForDlAckNack2,
                     rbMapNum, internalCellIndex);
         }
    }
    /** SPR 12457_12492 Fix : End **/

    /* Number of Tb in PCell is 0 & in SCell is > 0 means UE is not scheduled in PCell */
    if (NO_TB == pCellTb)
    {
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = HARQ_INVALID;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = HARQ_INVALID;    
	/* SPR 14081 fix start */
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = HARQ_INVALID;
	/* SPR 14081 fix end */
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = d;
	/* SPR 14081 fix start */
	ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 1;
	/* SPR 14081 fix end */
    }
    /* Number of Tb in PCell is 0 > & in SCell is 0 means UE is not scheduled in SCell */
    else if (NO_TB == sCellTb)
    {
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = a;    
	/* SPR 14081 fix start */
	ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = HARQ_INVALID;    
	/* SPR 14081 fix end */
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = HARQ_INVALID;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;
	/* SPR 14081 fix start */
	ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 1;
	/* SPR 14081 fix end */
    }
    /* 
       Number of Tb in PCell is 2 & in SCell is 1 
       Number of Tb in PCell is 1 & in SCell is 2 
       Number of Tb in PCell is 2 & in SCell is 2 
       */
    else
    {
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = a;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = b;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = c;    
        ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = d;
    }

    return resp;
}
#endif
#ifdef DL_UL_SPLIT
/*****************************************************************************
 * Function Name  : convertULConfigPduTypeAllocFail
 * Inputs         : ulSchUciInfo_p - pointer to ULSchUciInfo
 *                  scellIndex - cellIndex of scell
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts the UL Config PDU Type when RB 
 *                  Allocation fails in PDSCH and UL config needs to be send 
 *                  without Harq.
 *****************************************************************************/
/*
STATIC void convertULConfigPduTypeAllocFail (   UInt16 ueIndex,
        ULSchUciInfo* ulschUciInfo_p,
        UInt8 sCellIndex
        )
{
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Entring. ",__func__);
    DLUEContext             *dlUEContext_p = PNULL;
    DLUESCellContext        *dlUESCellContext_p = PNULL;
    UInt8 numOfTxAnteenas = 0;
    UInt8 tbsSupportedCell[MAX_NUM_CELL] = {0,0};

    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    dlUESCellContext_p = dlUEContext_p->dlUeScellContext_p[sCellIndex];
    dlUESCellContext_p->pucchReqInfo.schedulingStatus = NO_TB;

    switch (ulschUciInfo_p->pduType)
    {
        case MAC_SS_PDU_ULSCH_HARQ:
            ulschUciInfo_p->pduType = MAC_SS_PDU_ULSCH ;
            break;
        case MAC_SS_PDU_ULSCH_CQI_HARQ_RI:
            ulschUciInfo_p->pduType = MAC_SS_PDU_ULSCH_CQI_RI ;
            break;
        case MAC_SS_PDU_UCI_SR_HARQ:
            ulschUciInfo_p->pduType = MAC_SS_PDU_UCI_SR ;
            break;
        case MAC_SS_PDU_UCI_CQI_HARQ:
            ulschUciInfo_p->pduType = MAC_SS_PDU_UCI_CQI ;
            break;
        case MAC_SS_PDU_UCI_CQI_SR_HARQ:
            ulschUciInfo_p->pduType = MAC_SS_PDU_UCI_CQI_SR ;
            break;
        case MAC_SS_PDU_UCI_HARQ:
            ulschUciInfo_p->pduType = MAC_SS_PDU_INVALID ;
            break;
        default:
            lteWarning("Incorrect ulSchUciInfo_p->pduType = %d\n",
                    ulschUciInfo_p->pduType);
            break;
    }

    numOfTxAnteenas =
        cellSpecificParams_g.cellConfigAndInitParams_p[PCELL_SERV_CELL_IX]->cellParams_p->numOfTxAnteenas;

    TBS_SUPPORTED_IN_CELL(dlUEContext_p->dlMIMOInfo.transmissionMode, numOfTxAnteenas, PCELL_SERV_CELL_IX)

    numOfTxAnteenas =
        cellSpecificParams_g.cellConfigAndInitParams_p[sCellIndex]->cellParams_p->numOfTxAnteenas;

    TBS_SUPPORTED_IN_CELL(dlUEContext_p->dlMIMOInfo.transmissionMode, numOfTxAnteenas, sCellIndex)
*/
        /* When in PCell SISO is supported */
/*  
    if (ONE_TB_SUPPORTED == tbsSupportedCell[PCELL_SERV_CELL_IX])
    {
*/        
        /* When in SCell SISO is supported */
/*  
        if(ONE_TB_SUPPORTED == tbsSupportedCell[sCellIndex]) 
        {
            ulschUciInfo_p->dlHarqInfo.ackNackMode = 1;
            ulschUciInfo_p->dlHarqInfo.harqSize = 2;
            ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 1;
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = HARQ_INVALID;  
        }
*/        
        /* When in SCell MIMO is supported */
/*
        else
        {
            ulschUciInfo_p->dlHarqInfo.ackNackMode = 1;
            ulschUciInfo_p->dlHarqInfo.harqSize = 3;
            ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 2;
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = HARQ_INVALID;  
        }
    }
*/    
    /* When in PCell MIMO is supported */
/*    
    else
    {
*/    
        /* When in SCell SISO is supported */
/*        
        if(ONE_TB_SUPPORTED == tbsSupportedCell[sCellIndex]) 
        {
            ulschUciInfo_p->dlHarqInfo.ackNackMode = 1;
            ulschUciInfo_p->dlHarqInfo.harqSize = 3;
            ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 2;
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_0 = HARQ_INVALID;  
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_1 = HARQ_INVALID;  
        }
*/        
        /* When in SCell MIMO is supported */
/*        else
        {
            ulschUciInfo_p->dlHarqInfo.ackNackMode = 1;
            ulschUciInfo_p->dlHarqInfo.harqSize = 4;
            ulschUciInfo_p->dlHarqInfo.numOfPUCCHResource = 2;
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_2 = HARQ_INVALID;  
            ulschUciInfo_p->dlHarqInfo.n_PUCCH_1_3 = HARQ_INVALID;  
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,PUCCH_MODULE,"[%s] Exit. ",__func__);
}
*/
#endif


#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : processDlSecondaryCellQueue
 * Inputs         : pdcchULOutputInfo_p - Its point to PdcchULOutputInfo structure.
 *                  ulschUciInfo_p - pointer to UCI info structure.
 *					subFrameNum - SF
 *                  rbMapNum - rbMap number
 *                  ttiWithDelay - tti
 *                  puschReserveOnlyFlagForSingleUE -set only for singleUE
 *                  internalCellIndex - cellIndex of PCell
 * Outputs        : None
 * Returns        : None
 * Description    : This function prepares the UL Config PDU of those UE's which are
 *                  scheduled only in scell.
 *****************************************************************************/

void processDlSecondaryCellQueue(
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt32 subFrameNum,
        UInt32 rbMapNum,
        /* SPR 15909 fix start */
        tickType_t ttiWithDelay,
        /* SPR 15909 fix end */
        /** SPR 12457 Fix : Start**/
#ifdef PUCCH_RESERVE_ALWAYS
        UInt8 puschReserveOnlyFlagForSingleUE,
#endif
        /** SPR 12457 Fix : End **/
        InternalCellIndex internalCellIndex
        )
{
    UInt16 sCellQCount = 0;
    UInt32 numberOfInfoPresent = 0;
    LP_SpsDlInfo spsDlInfo_p = PNULL;
    ULSchUciInfo *ulschUciInfo_p = PNULL;
    ULUEContextInfo *uLUEContextInfo_p = PNULL;
    ULUEContext *ulUEContext_p = PNULL;
    DLUEContext *ueDLContext_p = PNULL;
    SCellSchedulingQueueNode *sCellScheduledNode_p = PNULL; 

    sCellQCount = SCELL_SCHEDULING_QUEUE_COUNT(internalCellIndex);

    /* Coverity_59094_Fix Start */
    while ((sCellQCount--) && (MAX_ULSCH_UCI_SUPPORTED > ulschUciRachInfo_p->numberOfInfoPresent))
        /* Coverity_59094_Fix End */
    {
        DEQUEUE_SCELL_SCHEDULING_NODE(sCellScheduledNode_p, internalCellIndex);

        /* UE is scheduled for uplink */
        if (sCellScheduledNode_p)
        {
            /* + SPR 11471 Changes */
            ueDLContext_p = dlUECtxInfoArr_g[sCellScheduledNode_p->ueIndex].dlUEContext_p;
            /* - SPR 11471 Changes */
            /* SPR 11712  - cceIndex check for Pcell is removed */
            /** SPR 11916 Fix : Start**/
            /** SPR 12457 Fix : Start**/
            if (ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus)
            /** SPR 12457 Fix : End **/
            {
                uLUEContextInfo_p = &ulUECtxInfoArr_g[sCellScheduledNode_p->ueIndex];
                ulUEContext_p = ulUECtxInfoArr_g[sCellScheduledNode_p->ueIndex].ulUEContext_p;
                numberOfInfoPresent = ulschUciRachInfo_p->numberOfInfoPresent;
                ulschUciInfo_p = &(ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent]);

                /* SPR 5369 changes start */
                if ( uLUEContextInfo_p->pendingDeleteFlag || 
                        PNULL == uLUEContextInfo_p->ulUEContext_p )
                {
                    lteWarning("Pending Delete Flag is set For UE Index = %d", sCellScheduledNode_p->ueIndex);
                    FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                    continue;
                }

                /* SPR 5369 changes end */

                if (SETUP == uLUEContextInfo_p->ulUEContext_p->measGapConfigType)
                {
                    if (TRUE == isTTIInMeasGapPeriod(uLUEContextInfo_p->ulUEContext_p, ttiWithDelay))
                    {
                        /*ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                         *   pduType = MAC_SS_PDU_INVALID;
                         */
                        FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                        continue;
                    }
                }

                if ( MAX_ULSCH_UCI_SUPPORTED ==  ulschUciRachInfo_p->numberOfInfoPresent)
                {
                    /*SPR 19565 warning fix+*/
                    ltePanic("[%s]:2. OverFlow occured in the ULSchUCIRachPoolInfo pool subFrameNum[%d] rbMapNum[%d] puschReserveOnlyFlagForSingleUE[%d]",
                            __func__,subFrameNum,rbMapNum,puschReserveOnlyFlagForSingleUE);
                    /*SPR 19565 warning fix-*/
                    FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                    break;
                }        

                /* Calculate the nPUCCH DL ACK/NACK */
                spsDlInfo_p = &(ueDLContext_p->spsDlInfo);
                ulschUciInfo_p->pduType = MAC_SS_PDU_UCI_HARQ;

                /** SPR 12457 Fix : Start**/
#ifdef FAPI_4_0_COMPLIANCE
                fillPucchHarqInfo(sCellScheduledNode_p->ueIndex, ulschUciInfo_p,\
                        INVALID_CCE_INDEX, rbMapNum, 
#ifdef PUCCH_RESERVE_ALWAYS
                        puschReserveOnlyFlagForSingleUE,
#endif
                        /* +- SPR 17777 */
                        pdcchULOutputInfo_p,subFrameNum, internalCellIndex);
                /* +- SPR 17777 */
#endif
                /** SPR 12457 Fix : End **/

                pdcchULOutputInfo_p->
                    directIndexingUEInfo[sCellScheduledNode_p->ueIndex].ulschUciInfoIndex = 
                    ulschUciRachInfo_p->numberOfInfoPresent;
                ulschUciInfo_p->ueDirectIndexInfo_p 
                    = &(pdcchULOutputInfo_p->directIndexingUEInfo[sCellScheduledNode_p->ueIndex]);

                /* SPR 1684 changes start */
                /* Fill rnti value */
                if( (SPS_SCHEDULING == ueDLContext_p->schType) ||
                        (pdcchULOutputInfo_p->directIndexingUEInfo[sCellScheduledNode_p->ueIndex].\
                         isSPSCCE == LTE_TRUE) )
                {
                    ulschUciInfo_p->rnti = spsDlInfo_p->SpsCrnti ;
                }
                else
                {
                    ulschUciInfo_p->rnti = ulUEContext_p->crnti;
                }
                /* SPR 1684 changes end */

                ulschUciRachInfo_p->numberOfInfoPresent++;
                /* + SPR 11471 Changes */
                /*cov warning fix 12 june*/
                /** SPR 12457 Fix : Start**/
                ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus  = 0x0;
                /** SPR 12457 Fix : End **/
                /* - SPR 11471 Changes */
            }    
            FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
            /** SPR 11916 Fix : End **/
        }
    }
}
#else
/*****************************************************************************
 * Function Name  : processDlSecondaryCellQueue
 * Inputs         : pdcchULOutputInfo_p - Its point to PdcchULOutputInfo structure.
 *                  ulschUciInfo_p - pointer to UCI info structure.
 *					subFrameNum - SF
 *                  rbMapNum - rbMap number
 *                  ttiWithDelay - tti
 *                  puschReserveOnlyFlagForSingleUE -set only for singleUE
 *                  internalCellIndex - cellIndex of PCell
 * Outputs        : None
 * Returns        : None
 * Description    : This function prepares the UL Config PDU of those UE's which are
 *                  scheduled only in scell.
 *****************************************************************************/
void processDlSecondaryCellQueue(
        /* +- SPR 17777 */
        ULSchUciRachPoolInfo* ulschUciRachInfo_p,
        UInt32 subFrameNum,
        UInt32 rbMapNum,
        /* SPR 15909 fix start */
        tickType_t ttiWithDelay,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex
        )
{
    UInt16 sCellQCount = 0;
    ULUEContextInfo *uLUEContextInfo_p = PNULL;
    DLUEContext *ueDLContext_p = PNULL;
    SCellSchedulingQueueNode *sCellScheduledNode_p = PNULL; 
    UeScheduledInfo * ulUeScheduledInfo_p = PNULL;

    sCellQCount = SCELL_SCHEDULING_QUEUE_COUNT(internalCellIndex);

    /* Coverity_59094_Fix Start */
    while ((sCellQCount--) && (MAX_ULSCH_UCI_SUPPORTED > ulschUciRachInfo_p->numberOfInfoPresent))
        /* Coverity_59094_Fix End */
    {
        DEQUEUE_SCELL_SCHEDULING_NODE(sCellScheduledNode_p, internalCellIndex);

        /* UE is scheduled for uplink */
        if (sCellScheduledNode_p)
        {
            /* + SPR 11471 Changes */
            ueDLContext_p = dlUECtxInfoArr_g[sCellScheduledNode_p->ueIndex].dlUEContext_p;
            /* - SPR 11471 Changes */
            /* SPR 11712  - cceIndex check for Pcell is removed */
            /** SPR 11916 Fix : Start**/
            /** SPR 12457 Fix : Start**/
            /* + SPR_14522 */
            if ((ueDLContext_p) && (ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]) &&
                    ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus)
            /* - SPR_14522 */
            /** SPR 12457 Fix : End **/
            {
                uLUEContextInfo_p = &ulUECtxInfoArr_g[sCellScheduledNode_p->ueIndex];

                /* SPR 5369 changes start */
                if ( uLUEContextInfo_p->pendingDeleteFlag || 
                        PNULL == uLUEContextInfo_p->ulUEContext_p )
                {
                    lteWarning("Pending Delete Flag is set For UE Index = %d", sCellScheduledNode_p->ueIndex);
                    FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                    continue;
                }

                /* SPR 5369 changes end */

                if (SETUP == uLUEContextInfo_p->ulUEContext_p->measGapConfigType)
                {
                    if (TRUE == isTTIInMeasGapPeriod(uLUEContextInfo_p->ulUEContext_p, ttiWithDelay))
                    {
                        /*ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                         *   pduType = MAC_SS_PDU_INVALID;
                         */
                        FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                        continue;
                    }
                }

                if ( MAX_ULSCH_UCI_SUPPORTED ==  ulschUciRachInfo_p->numberOfInfoPresent)
                {
                    ltePanic("[%s]:2. OverFlow occured in the ULSchUCIRachPoolInfo pool",
                            __func__);
                    FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
                    break;
                }        

                /** SPR 14204 Fix : Start **/
                ulUeScheduledInfo_p = uLUEContextInfo_p->ulUEContext_p->
                    ulUeScheduledInfoPointerArray[internalCellIndex][subFrameNum];
                /** SPR 14204 Fix : End **/

                if (ulUeScheduledInfo_p)
                {    

//#ifdef FAPI_4_0_COMPLIANCE
#if 1
                    allocateResourceForTddHarq(subFrameNum,
                            sCellScheduledNode_p->ueIndex,
                            rbMapNum,
                            ulschUciRachInfo_p,
                            ulUeScheduledInfo_p,
                            internalCellIndex); 
#endif
                }
                /* + SPR_14262 */
                SET_HARQ_BLER_RECEIVED_ON_PUCCH(sCellScheduledNode_p->ueIndex, subFrameNum);
                /* - SPR_14262 */
                ueDLContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus  = 0x0;
            }    
            FREE_MEM_NODE_SCELL_SCHEDULING_Q(sCellScheduledNode_p);
            /** SPR 11916 Fix : End **/
        }
    }
}
#endif

#ifdef TDD_CONFIG
/* TDD Config 0 Changes Start */
/*****************************************************************************
 * Function Name  : getMAndMaxCfiValue
 * Inputs         : ulSubframe - UL Subframe for which M value
 *                  will be calculated. And based on whether DL/Special
 *                  subframe for which Ack/Nack will be received on this
 *                  ulSubframe CFI value will be set.
 *
 * Outputs        : cfi - If bundling of only Special subframe, then
 *                  cfi will be set as 2 else cfi will be 3.
 *                  M - Number of entries in Downlink Association set.
 * Returns        : None
 * Description    : This function will check  the number of
 *                  entries in Downlink Association set. And for each entry
 *                  if subframe is DL then CFI is set as 3 otherwise CFI
 *                  will be set as 2.
 *                  .
 *****************************************************************************/
void getMAndMaxCfiValue(UInt8 ulSubframe,
        UInt8 *cfi,
        UInt8 *M,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        )
{
    /* SPR 19309 + */
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    UInt8 splSubFramePatterns = cellParams_p->splSubFramePatterns;
    UInt8 dlAvailableRBs = cellParams_p->dlAvailableRBs;
    UInt8 counter = 0;
    UInt8 cfiValue =  cellParams_p->cfi;
    /* SPR 19309 - */
    SInt8 signedOffset = 0;
    UInt8   dlIndex = 0;
    UInt8 k =0;
    /*CA TDD CHANGES START*/
    UInt8 temp_M = globalDlAssociationSet_gp[internalCellIndex][ulSubframe].count;
    /*CA TDD CHANGES END*/
    *M = temp_M;
    if(temp_M)
    {
        for(; counter < temp_M ; counter++)
        {
            /*CA TDD CHANGES START*/
            k = globalDlAssociationSet_gp[internalCellIndex][ulSubframe].sortedKwithSmallM[counter].k;
            /*CA TDD CHANGES END*/
            signedOffset = (ulSubframe - k);
            signedOffset = (signedOffset)%MAX_SUB_FRAME;
            dlIndex = (signedOffset) < 0?(signedOffset + MAX_SUB_FRAME):(signedOffset);

            if(DL == getSubframeType(dlIndex,internalCellIndex))
            {
                if(6 == dlAvailableRBs)
                {
                    cfiValue = MAX_POSSIBLE_CFI_VALUE;
                }
                else
                {
                    /* SPR 19309 + */
                    cfiValue = cellParams_p->cfi + 1;
                    /* SPR 19309 - */
                }

                break;
            }
        }
        /* SPR 19309 + */
        if (cellParams_p->cfi == cfiValue)
        /* SPR 19309 - */
        {
            /* This means only Special Subframe is in Downlink Association
             * set Index
             */
            if(6 == dlAvailableRBs)
            {
                /* SPR 19309 + */
                cfiValue = cellParams_p->cfi + 1;
                /* SPR 19309 - */
            }

            if((SPECIAL_SUB_FRAME_CONFIG_0 == splSubFramePatterns ||
                        SPECIAL_SUB_FRAME_CONFIG_5 == splSubFramePatterns))
            {
                *M = 0;
            }
        }
    }
    *cfi = cfiValue;
    return ;

}
/*****************************************************************************
 * Function Name  : calculateMaxPucchRegion
 * Inputs         : None
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function checks the PUCCH resources for MSG3/SPS 
 *                  Returns SUCCESS if PUCCH resources required is less 
 *                  than reserved value otherwise returns FAILURE.
 *****************************************************************************/
MacRetType calculateMaxPucchRegion(InternalCellIndex internalCellIndex)
{
    UInt8   cceIndex       = 0;
    SInt8   ulPucchRes     = 0;
    UInt8   rbIndexSlot1   = 0;
    UInt8   rbIndexSlot2   = 0;
    UInt32  nPUCCH         = 0;
    UInt8 cfiValue         = 0;

    UInt32 Np = 0;
    UInt32 Npnext = 0;
    UInt8 subframe =0;
    UInt8 M = 0;
    UInt8 miValue = 0;
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    for ( ; subframe < MAX_SUB_FRAME ; subframe++)
    {

        if(UL == getSubframeType(subframe,internalCellIndex))
        {
            miValue = 1;
            getMAndMaxCfiValue(subframe,&cfiValue,&M,internalCellIndex);
            if(M)
            {
                /* This condition check if TDD Config 0 and D 
                 * subframe then miValue will be 2 */
                if((UL_DL_CONFIG_0 == getTddConfig(internalCellIndex)) && 
                        ( (subframe == 4) || (subframe == 9) ))
                {
                    miValue = 2; 

                }
                cceIndex = maxCCEForCFI_g[internalCellIndex][cfiValue-1][miValue] -1 ;
                pucchGetNpsForCCE(cceIndex, &Np, &Npnext, internalCellIndex);
                nPUCCH = (M - 1) * Npnext + cceIndex + n1PucchDLAckNack_g[internalCellIndex]; 
            }
            else
            {
                nPUCCH = n1PucchDLAckNack_g[internalCellIndex];               

            }
            /* Check PUCCH Resources for HARQ ACK */
            if ( MAC_FAILURE == getPRBFromPUCCHResourceIndex(
                        nPUCCH,
                        UCI_FORMAT_ONE_A,
                        &rbIndexSlot1,
                        &rbIndexSlot2,
                        internalCellIndex
                        ))
            {
                LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                        getCurrentTick(),
                        0,
                        n1PucchDLAckNack_g[internalCellIndex],
                        UCI_FORMAT_ONE_A,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "Invalid Configuraton For PUCCH Res");
                return MAC_FAILURE;
            }
            /* If UL BW is 1.4 set ulPucchRes = 2 othrewise set ulPucchRes = 4*/
            if (cellParams_p->ulAvailableRBs > MAX_RB_FOR_BW_1_4)
            {
                ulPucchRes = MIN_PUCCH_RES_MSG3_SPS_FOR_OTHRE_THAN_1_4BW;
            }
            else
            {
                ulPucchRes = MIN_PUCCH_RES_MSG3_SPS_FOR_1_4BW;
            }

            /* Check if RB left for MSG3 or not if not retunr Failure */
            /*+- SPR 20899 Fix*/
            if(ulPucchRes > (DIVIDE_BY_TWO(cellParams_p->ulAvailableRBs) - rbIndexSlot1))
            /*+- SPR 20899 Fix*/
            {
                LOG_MAC_MSG(MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,
                        getCurrentTick(),
                        0,
                        n1PucchDLAckNack_g[internalCellIndex],
                        UCI_FORMAT_ONE_A,
                        ulPucchRes,
                        rbsForPUCCH_g[internalCellIndex][subframe],
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "Error RB not left for MSG3/SPS ");
                return MAC_FAILURE;
            }
            else
            {
                /* SPR 10899 Fix Start */
                rbsForPUCCH_g[internalCellIndex][subframe] = rbIndexSlot1 + 1;
                /* SPR 10899 Fix End */
            }
        }
        else
        {
            rbsForPUCCH_g[internalCellIndex][subframe] = 0;
        }
    }
    return MAC_SUCCESS;

}
/* TDD Config 0 Changes End */

/* SPR 19679: TDD HARQ Multiplexing Changes Start */
/*********************************************************************************
 * Function Name : fillHARQSizeForMultiplexingOrBundling
 * Inputs        : ackNackMode : TDD Ack-Nack feedback mode
 *                 harqSizeForPUSCHWithDCI0 : HARQ size when HARQ on PUSCH with DCI0
 *                 ueIndex: UeIndex
 *                 subFrameNum: Ul SF
 *                 internalCellIndex : internal cellIndex
 *
 * Outputs       : harq size 
 * Returns       : harqSize 
 * Description   : This function will get the harq mode and fill HARQ size
 * ********************************************************************************/
UInt8 fillHARQSizeForMultiplexingOrBundling ( UInt8 ackNackMode,
                					      /*SPR 20564 Changes Start*/
				                    	      /*Code Removed */
                					      /*SPR 20564 Changes End*/ 
                                              UInt16 ueIndex,
                                              UInt8 subFrameNum,
                                              /* SPR 19679: TDD HARQ
                                               * Multiplexing  Changes Start */
                                              InternalCellIndex internalCellIndex,
                                              UeScheduledInfo * ulUeScheduledInfo_p)
/* SPR 19679: TDD HARQ Multiplexing  Changes End */

{
    UInt8 harqSize = 0;
    UInt8  M = globalDlAssociationSetForHarqMultiplexing_gp\
               [internalCellIndex][subFrameNum].count;
      /*SPR 20564 Changes Start*/
      /*Code Removed */
      /*SPR 20564 Changes End*/ 
    UInt8  k = 0;
    UInt8  dlIndex = 0;
    SInt32 signedOffset = 0; 

    /* Check if mode is multiplexing and M > 1
     *    1) If DCI0 is sent for this PDU i.e.new transmission or
     *        adaptive retransmission then HARQ size will be equal to
     *        scheduled DL Sfs.
     *    2) If this is PUCCH transmission or PUSCH and DCI0 is not sent 
     *       i.e. non-adaptive retransmission and UL SPS occasion 
     *       then HARQ size will be equal to  M.
     *
     * if HARQ mode is bundling or HARQ mode is multiplexing and M == 1
     *   then HARQ size will be according to transmission mode.
     *   i.e. if it is SISO or transmit diversity then HARQ size will be 1
     *   else 2.    
     * Referring specs 36.213, sec 7.3.
     *
     */

    if ((HARQ_MULTIPLEXING == ackNackMode) && (M > 1))
    {	
	    /*SPR 20564 Changes Start*/
	    /*Code Removed */
	    /*SPR 20564 Changes End*/ 
	    harqSize = M;
    }
    else
    {
        if(MAC_SUCCESS == isUeSISO(ueIndex))
        {
            harqSize = 1;
        }
        else
        {
            if ((HARQ_BUNDLING == ackNackMode))
            {
                harqSize = 2;
            }
            else
            {
                k = globalDlAssociationSetForHarqMultiplexing_gp[internalCellIndex][subFrameNum].\
                    kwithSmallM[0].k;
                signedOffset = (subFrameNum - k);
                signedOffset = signedOffset % MAX_SUB_FRAME;
                dlIndex = (signedOffset) < 0?(signedOffset + MAX_SUB_FRAME):(signedOffset);
                harqSize = ulUeScheduledInfo_p->harqSizeForMuxMOneInMIMO[dlIndex];
            }
        }
    }
    return harqSize;
} 


/*****************************************************************************
 * Function Name  : initTddMacDlAssociationSetForHARQMultiplexing
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This fills the dl association set table 10.1.3.1-1 of 
 *                   specification 36.213
 *****************************************************************************/
STATIC void initTddMacDlAssociationSetForHARQMultiplexing(void)
{
    /* Filling the value of tddMacDlAssociationSetForHarqMUX and count */
    memSet(tddMacDlAssociationSetForHarqMUX, 
               INVALID_SMALL_M, sizeof(tddMacDlAssociationSetForHarqMUX));
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][2].kwithSmallM[0].k = 6;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][2].kwithSmallM[0].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][2].count = 1;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][3].count = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][8].count = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][4].kwithSmallM[0].k = 4;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][4].kwithSmallM[0].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][4].count = 1;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][7].kwithSmallM[0].k = 6;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][7].kwithSmallM[0].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][7].count = 1;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][9].kwithSmallM[0].k = 4;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][9].kwithSmallM[0].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_0][9].count = 1;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][2].kwithSmallM[1].k = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][2].kwithSmallM[0].k = 6;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][2].kwithSmallM[0].smallM = 1;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][2].kwithSmallM[1].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][2].count = 2;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][3].kwithSmallM[0].k = 4;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][3].kwithSmallM[0].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][3].count = 1;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][7].kwithSmallM[1].k = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][7].kwithSmallM[0].k = 6;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][7].kwithSmallM[0].smallM = 1;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][7].kwithSmallM[1].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][7].count = 2;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][8].kwithSmallM[0].k = 4;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][8].kwithSmallM[0].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_1][8].count = 1;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][2].kwithSmallM[3].k = 8;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][2].kwithSmallM[2].k = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][2].kwithSmallM[1].k = 4;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][2].kwithSmallM[0].k = 6;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][2].kwithSmallM[0].smallM = 3;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][2].kwithSmallM[1].smallM = 2;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][2].kwithSmallM[2].smallM = 1;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][2].kwithSmallM[3].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][2].count = 4;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][7].kwithSmallM[3].k = 8;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][7].kwithSmallM[2].k = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][7].kwithSmallM[1].k = 4;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][7].kwithSmallM[0].k = 6;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][7].kwithSmallM[0].smallM = 3;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][7].kwithSmallM[1].smallM = 2;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][7].kwithSmallM[2].smallM = 1;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][7].kwithSmallM[3].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_2][7].count = 4;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][2].kwithSmallM[2].k = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][2].kwithSmallM[1].k = 6;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][2].kwithSmallM[0].k = 11;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][2].kwithSmallM[0].smallM = 2;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][2].kwithSmallM[1].smallM = 1;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][2].kwithSmallM[2].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][2].count = 3;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][3].kwithSmallM[1].k = 6;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][3].kwithSmallM[0].k = 5;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][3].kwithSmallM[0].smallM = 1;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][3].kwithSmallM[1].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][3].count = 2;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][4].kwithSmallM[1].k = 5;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][4].kwithSmallM[0].k = 4;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][4].kwithSmallM[0].smallM = 1;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][4].kwithSmallM[1].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_3][4].count = 2;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][2].kwithSmallM[3].k = 12;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][2].kwithSmallM[2].k = 8;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][2].kwithSmallM[1].k = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][2].kwithSmallM[0].k = 11;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][2].kwithSmallM[0].smallM = 3;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][2].kwithSmallM[1].smallM = 2;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][2].kwithSmallM[2].smallM = 1;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][2].kwithSmallM[3].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][2].count = 4;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][3].kwithSmallM[3].k = 6;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][3].kwithSmallM[2].k = 5;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][3].kwithSmallM[1].k = 4;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][3].kwithSmallM[0].k = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][3].kwithSmallM[0].smallM = 3;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][3].kwithSmallM[1].smallM = 2;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][3].kwithSmallM[2].smallM = 1;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][3].kwithSmallM[3].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_4][3].count = 4;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[8].k = 13;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[7].k = 12;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[6].k = 9;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[5].k = 8;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[4].k = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[3].k = 5;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[2].k = 4;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[1].k = 11;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[0].k = 6;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[0].smallM = 8;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[1].smallM = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[2].smallM = 6;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[3].smallM = 5;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[4].smallM = 4;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[5].smallM = 3;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[6].smallM = 2;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[7].smallM = 1;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].kwithSmallM[8].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_5][2].count = 9;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][2].kwithSmallM[0].k = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][2].kwithSmallM[0].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][2].count = 1;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][3].kwithSmallM[0].k = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][3].kwithSmallM[0].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][3].count = 1;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][4].kwithSmallM[0].k = 5;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][4].kwithSmallM[0].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][4].count = 1;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][7].kwithSmallM[0].k = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][7].kwithSmallM[0].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][7].count = 1;

    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][8].kwithSmallM[0].k = 7;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][8].kwithSmallM[0].smallM = 0;
    tddMacDlAssociationSetForHarqMUX[UL_DL_CONFIG_6][8].count = 1;

}

//#ifdef FAPI_4_0_COMPLIANCE
#if 1
/*****************************************************************************
 * Function Name  : fillHARQSizeForPUSCHWithChannelSelection 
 * Inputs         : ULSchUciInfo and internalCellIndex 
 * Outputs        : none
 * Returns        : none
 * Description    : This fills the ackNackMode and harqSize according to the
 *                  value of M
 *****************************************************************************/
void fillHARQSizeForPUSCHWithChannelSelection(
         UInt16 ueIndex
        ,ULSchUciRachPoolInfo* ulschUciRachInfo_p
        ,UInt32 ulSubFrame
        ,UInt16  numberOfInfoPresent
        ,InternalCellIndex internalCellIndex
        )
{
    UInt8           cellCount = 0;
    DLUESCellContext * dlUESCellContext_p = PNULL;
    UInt8 numOfTxAnteenas = 0;
    UInt8 tbsSupportedCell[MAX_NUM_CELL] = {0,0};
    UInt8 numOfTxAnteenasPCell = 0;
    DLUEContext * dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    /*fill ackNackMode */
    ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].\
        dlHarqInfo.ackNackMode = CHANNEL_SELECTION_MODE_2;
    /*calculate M*/
    UInt8 M = globalDlAssociationSet_gp[internalCellIndex][ulSubFrame].count;
    /*fill harq size according to M*/
    if (1 == M)
    {
        for (cellCount = START_SCELL_INDEX; cellCount <= MAX_NUM_SCELL; cellCount++)
        {
            dlUESCellContext_p = dlUEContext_p->dlUeScellContext_p[cellCount];

            /* SCell Context exists & scell is active */ 
            if (dlUESCellContext_p)
            {
                numOfTxAnteenas =
                    cellSpecificParams_g.cellConfigAndInitParams_p
                    [dlUESCellContext_p->internalCellIndex]->cellParams_p->numOfTxAnteenas;

                    TBS_SUPPORTED_IN_CELL(dlUESCellContext_p->ScelldlMIMOInfo.transmissionMode, \
                        numOfTxAnteenas, dlUESCellContext_p->internalCellIndex)
                    /*  Checking which cell supports how many TB & fill harq info accordingly 
                        eg. : PCell Supports 1Tb SCell Supports 2Tb 
                        */
                    /* When in PCell SISO is supported */
                    numOfTxAnteenasPCell =
                    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                    ->cellParams_p->numOfTxAnteenas;
 
                    TBS_SUPPORTED_IN_CELL(dlUEContext_p->dlMIMOInfo.transmissionMode,
                        numOfTxAnteenasPCell, internalCellIndex)

                    if (ONE_TB_SUPPORTED == tbsSupportedCell[internalCellIndex])
                    {
                        /* When in SCell SISO is supported */
                        if(ONE_TB_SUPPORTED == 
                                tbsSupportedCell[dlUESCellContext_p->internalCellIndex]) 

                        {
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].\
                                        dlHarqInfo.harqSize = ONE_TB_SUPPORTED + ONE_TB_SUPPORTED;

                        }
                        /* When in SCell MIMO is supported */
                        else
                        {
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].\
                                          dlHarqInfo.harqSize = ONE_TB_SUPPORTED + TWO_TB_SUPPORTED;
                        }
                    }
                /* When in PCell MIMO is supported */
                    else
                    {
                        /* When in SCell SISO is supported */
                        if(ONE_TB_SUPPORTED == 
                                tbsSupportedCell[dlUESCellContext_p->internalCellIndex]) 
                        {
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].\
                                           dlHarqInfo.harqSize = TWO_TB_SUPPORTED + ONE_TB_SUPPORTED;
                        }
                        /* When in SCell MIMO is supported */
                        else
                        {
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].\
                                            dlHarqInfo.harqSize = TWO_TB_SUPPORTED + TWO_TB_SUPPORTED;
                        }

                    }
            }
        }
    }

    else if (1 < M)
    {
#ifdef MAC_CA_WORKAROUND
        if(ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].pduType==MAC_SS_PDU_ULSCH_HARQ)
        {
           ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.harqSize = 2;//(M * 2);
        }
		else
#endif
		{
            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].dlHarqInfo.harqSize = (M * 2);
		}
    }
}

/*SPR 19679: TDD HARQ Multiplexing Changes End */
#endif 
#endif 


