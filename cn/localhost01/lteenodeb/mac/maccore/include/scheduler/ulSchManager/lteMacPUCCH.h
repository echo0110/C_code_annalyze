/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPUCCH.h,v 1.1.1.1.6.1.20.1 2010/11/08 06:06:19 gur10121 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacPUCCH.h,v $
 * Revision 1.1.1.1.6.1.20.1  2010/11/08 06:06:19  gur10121
 * Semi static changes merging
 *
 * Revision 1.1.1.1.6.1.14.2  2010/10/27 17:13:50  gur10121
 * UT fixes + review comments incorporation
 *
 * Revision 1.1.1.1.6.1.12.1  2010/10/15 18:53:14  gur10121
 * Semi staic Changesdone and checked in
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.16  2009/08/20 07:03:25  gur19413
 * removed cqiMode Parameter.
 *
 * Revision 1.15  2009/07/28 00:13:43  gur19413
 * changed function return Type
 *
 * Revision 1.14  2009/07/23 08:41:28  gur19413
 * added new macro UCI_FORMAT_TWO, UCI_FORMAT_TW0_A
 *
 * Revision 1.13  2009/07/16 13:05:41  gur19413
 * added prototype of cleanupPUCCH fun
 *
 * Revision 1.12  2009/07/15 13:15:55  gur19413
 * Added new macro ALREADY_CONSIDER_CCE_INDEX
 *
 * Revision 1.11  2009/07/13 04:15:00  gur19413
 * added new Macro for log
 *
 * Revision 1.10  2009/07/01 05:48:51  gur19413
 * processPUCCHAndPRACHReports function added new parameter
 *
 * Revision 1.9  2009/06/23 23:06:36  gur19413
 * function prototype change initSR,initCQI
 *
 * Revision 1.8  2009/06/22 06:14:10  gur19413
 * protoype change reConfigureRIAndCQIMap
 *
 * Revision 1.7  2009/06/15 07:59:02  gur19413
 * added new parameter in prototype updatePUCCHConf
 *
 * Revision 1.6  2009/06/10 14:46:08  gur19413
 * comments incorporated
 *
 * Revision 1.5  2009/06/10 11:54:44  gur19413
 * comments incorporated
 *
 * Revision 1.4  2009/06/05 06:57:30  gur19413
 * Rmoved Compilation Errors
 *
 * Revision 1.3  2009/06/05 06:52:04  gur19413
 * Removed Compilation Erros
 *
 * Revision 1.2  2009/06/01 22:45:14  gur19413
 * added new functions prototype
 *
 * Revision 1.1  2009/04/30 12:18:30  gur19413
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_PUCCH_H
#define LTE_MAC_PUCCH_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacULUEContext.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacMeasGapHandler.h" /* MEAS_GAP_CHG */ 
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
/* Rel9_upgrade_CR257 */
#define TOTAL_NUM_ROW_IN_SR_CONFIG  7
/* Rel9_upgrade_CR257 */
#define TOTAL_NUM_ROW_IN_CQI_CONFIG 10 
#define TOTAL_NUM_ROW_IN_CQI_TDD_CONFIG 7 
#define TOTAL_NUM_ROW_IN_RI_CONFIG  6
#ifdef FDD_CONFIG
#define MAX_SR_PERIODCITY           80
#define MAX_RI_PERIODICITY          32
#define MAX_CQI_PERIOIDICITY        160
#define ALREADY_CONSIDER_CCE_INDEX  250
#endif
#define MAX_PERIODICITY_SUBFRAME    160
#define PUCCH_MODULE                "PUCCH"

#define MAC_RI_CONFIG_INDEX_INVALID 1000 /* SS_S2 */     
#ifdef FDD_CONFIG
/* Check Collison */
#define isAnyCollisionPrevReport(SubFrameOffset, ReportOffset, ReportPeriodicity )\
( (abs_wrapper(SubFrameOffset - ReportOffset) % ReportPeriodicity == 0)? TRUE : FALSE ) 
#endif /*FDD_CONFIG */

/* + CQI_4.1 */ 

#define GET_REQUIRED_TICK(sfn ,sf, incrementFactor)\
{\
    UInt32 sf1 = 0;\
        sf1  = sf + incrementFactor;\
        \
        if (sf1 >=  MAX_SUB_FRAME)\
        {\
            sf = sf1 % 10;\
\
            sfn  += (sf1 / 10);\
\
            if ( sfn >= MAX_SFN_VALUE )\
            {\
                sfn -= MAX_SFN_VALUE;\
            }\
        }\
        else\
        {\
            sf = sf1;\
        }\
}     


/* - CQI_4.1 */ 
 
/* Review comment fix start GG15 */
#define MAC_MIN_2VAR(A,B) \
    ((A) < (B) ) ? (A) : (B)
#define MAC_MAX_2VAR(A,B) \
    ((A) > (B) ) ? (A) : (B)
#define MAX_RB_FOR_BW_1_4                            6
#define MIN_PUCCH_RES_MSG3_SPS_FOR_1_4BW             2
#define MIN_PUCCH_RES_MSG3_SPS_FOR_OTHRE_THAN_1_4BW  4
/* Review comment fix end GG15 */
/* this is shared so eMTC flag is not required*/
#define MAX_SUBFRAME_OFFSET 160

 
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
typedef struct SrConfigTableT
{
   UInt16 startIndex;
   UInt16 endIndex;
   UInt8 srPeriodicity;
   SInt16 srSubframeOffset;
}SrConfigTable;


typedef struct CqiPmiConfigT
{
    UInt16 cqiPmiConfigStartIndex;
    UInt16 cqiPmiConfigEndIndex;
    UInt8  cqiPeriodicity;
    SInt16 nOffsetCQI;
}CqiPmiConfig;

typedef struct RiConfigT
{
    UInt16 riConfigStartIndex;
    UInt16 riConfigEndIndex;
    UInt8  riPeriodicity;
    SInt16 nOffsetRI;
}RiConfig;

typedef struct UeCellInfoT
{
    /* Whether PCell or SCell */
    UInt8 isPCell;
    /* Cell Index of SCell */
    InternalCellIndex sCellIndex;
    /* Internal Cell Index of SCell */
    InternalCellIndex sCellInternalCellIndex;
}UeCellInfo;
#ifdef FDD_CONFIG


typedef enum PeriodicCQIModeT 
{
   MODE_10 = 10,
   MODE_20 = 20,
   MODE_11 = 11,
   MODE_21 = 21
}PeriodicCQIMode;

typedef struct PeriodicParametersT
{
    UInt8 cqiFlag;
    UInt8 riFlag;
    UInt8 srFlag;
    UInt8 cqiPeriodicity;
    UInt8 srPeriodicity;
    UInt8 riPeriodicity;
    UInt8 cqiOffset;
    UInt8 srOffset;
    UInt8 riOffset;
}PeriodicParameters;
#endif /* FDD_CONFIG*/

typedef enum ReportAndOpportunityT
{
    NO_SCHEDULING_REQUEST_OR_CQI_PMI_RI_REPORT = 0,
    CQI_REPORT,
    WIDEBAND_CQI_REPORT,
    RI_REPORT,
    SCHEDULING_REQUEST,

    ALL_REPORTS
}ReportAndOpportunity;

#ifdef TDD_CONFIG
typedef struct kAndSmallMT
{
    UInt8 k;
    UInt8 smallM;
}kAndSmallM;


typedef struct DlAssociationSetNodeT
{
    UInt8 count;
    kAndSmallM sortedKwithSmallM[MAX_SUB_FRAME];
}DlAssociationSetNode;

/* SPR 19679: TDD HARQ Multiplexing Changes Start */
/* Structure is used for caculating HARQ resources 
 * in case of HARQ multiplexing. Referring table 10.1.3.1-1
 * Specification 36.2123
 */
typedef struct DlAssociationSetNodeForHARQMultiplexingT
{
    UInt8 count;
    kAndSmallM kwithSmallM[MAX_SUB_FRAME];
}DlAssociationSetNodeForHARQMultiplexing;
extern DlAssociationSetNodeForHARQMultiplexing \
              * globalDlAssociationSetForHarqMultiplexing_gp[MAX_NUM_CELL];
/* SPR 19679: TDD HARQ Multiplexing Changes End */
#endif

#define CHECK_BIT_SET(str, bitPos)  ((str) & (1 << (bitPos) ))
#define SET_BIT_AT_POS(str, bitPos) ( (str) = (str) | (1 << (bitPos) ))
/* TDD SB_CQI */
#define UNSET_BIT_AT_POS(str, bitPos) ( (str) = (str) & ~(1 << (bitPos) ))
/* TDD SB_CQI */
#ifdef TDD_CONFIG
typedef enum tddAckNackFeedbackModeT
{
    HARQ_BUNDLING,
    HARQ_MULTIPLEXING
}tddAckNackFeedbackMode;

#define MAX_CONFIG_INDEX 1024
#define MAX_J_VALUE 4
#define MAX_K_VALUE 4
#define INVALID_CONFIG_INDEX 0xffff
#endif

#define NORMAL_CYCLIC_PREFIX_VALUE 3
#define EXTENDED_CYCLIC_PREFIX_VALUE 2
#define NSCRB 12

#define PUCCH_INVALID 0xFF
#define HARQ_INVALID 0xFFFF

typedef struct UESrAndReportListT
{
      LTE_LIST ueList;
}UESrAndReportList;



typedef enum UCIFormatT
{
   UCI_FORMAT_ONE = 1,
   UCI_FORMAT_ONE_A,
   UCI_FORMAT_ONE_B,
   UCI_FORMAT_TWO,
   UCI_FORMAT_TWO_A,
   UCI_FORMAT_TWO_B,
   UCI_FORMAT_INVALID,
}UCIFormat;

/* Macros to insert node in a PUCCH Queue */
#define QINIT_PUCCH_REF( pucchRef_p )\
    queueInit( &pucchRef_p );

#define QCOUNT_PUCCH_REF( pucchRef_p )\
    queueCount( pucchRef_p )

#define ENQUEUE_PUCCH_REF( pucchRef_p, pucchNode_p ){\
    enQueue( pucchRef_p, &(pucchNode_p->nodeAnchor) ); \
}

#define DELETE_PUCCH_REF(pucchRef_p, pucchNode_p){\
    delQueueNode( pucchRef_p, &(pucchNode_p->nodeAnchor) );\
}

#define GET_FIRST_PUCCH_REF( pucchRef_p, pucchNode_p ){\
    pucchNode_p = (void *)getFirstQueueNode( pucchRef_p );\
}

#define GET_NEXT_PUCCH_REF( pucchRef_p, pucchNode_p ){\
    pucchNode_p = (void *)getNextQueueNode( pucchRef_p, \
                                    &(pucchNode_p->nodeAnchor ));\
}

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
typedef struct SCellSchedulingQueueNodeT
{
    /* data type of anchor */
    LTE_SNODE ackNackAnchor;
    /* Index of UE in ueDLContextArr_g */
    UInt16 ueIndex;
    InternalCellIndex serveCellIndex;
}SCellSchedulingQueueNode;

typedef enum ScheduledTBsT
{
    NO_TB = 0,
    SINGLE_TB,
    TWO_TB
}ScheduledTBs;  
typedef enum UCI_ACK_NACK_MODE_T
{
  FORMAT_1A_1B = 0,
  CHANNEL_SELECTION,
#ifdef TDD_CONFIG
  CHANNEL_SELECTION_MODE_2,
#endif
  UCI_FORMAT_3
}UCI_ACK_NACK_MODE;
/*CA Changes end  */
/* This function initialize the list and create mempool.*/
void initPUCCH(InternalCellIndex internalCellIndex);
void cleanupPUCCH(InternalCellIndex internalCellIndex);
void deInitPUCCH(InternalCellIndex internalCellIndex);

/* This function update PUCCH Configuration used in Mapping PUCCH Resource to
   PRB Resource Index*/
void updatePUCCHConfig( UInt32 deltaPUCCHSHIFT,
        UInt32 nRBCQI,
        UInt32 nCSAN,
        UInt32 n1PucchDLAckNack,
        InternalCellIndex internalCellIndex
        );

/* This function called by PUSCH*/
void processPUCCHAndPRACHReports( PdcchULOutputInfo *pdcchULOutputInfo_p,
        UInt32 rbMapNum,
        /* SPR 15909 fix start */
        tickType_t tti,
        /* SPR 15909 fix end */
  /* TDD Config 0 Changes Start */
/* ulSchUciRachContainer_g[MAX_PDCCH_CONTAINER] which was earlier D based.
 * has been changed to U based.
 * No need for this parameter in TDD now. As this 
 * will be calculated from ulSubframe , which is
 * passed as parameter.
 */
#ifdef FDD_CONFIG
        UInt8 containerTick,
#endif
/* TDD Config 0 Changes End */
#ifdef TDD_CONFIG
        UInt32 ulSubFrame
#else
        UInt8 ulDelay
#endif
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        );

/* This function process the Advance PUCCH Reports And set the Start Index 
   for PUSCH Resources */
void processAdvancePUCCHAndPRACHReports ( UInt32 rbMapNum,
        /* SPR 15909 fix start */
        tickType_t tti
        /* SPR 15909 fix end */
#ifdef TDD_CONFIG
        , UInt8 ulSubFrame
#endif
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        ); 

/*SPR 2617 End*/

MacRetType initSchedReqInPUCCHMap(
        /*SPR 16855 +-*/
        UInt16 ueIndex,
        PucchConfig *pucchConfigInfo_p,
        /*SPR 16855 +-*/
        ULUEContext  *currentUEContext_p,
        UInt32 srConfigurationIndex,
        UInt32 srPucchResourceIndex,
        /* Rel9_upgrade_CR410 */
        UInt16 srProhibitTimerValv920,
        /* Rel9_upgrade_CR410 */
        InternalCellIndex internalCellIndex
        );


/* SPR 3172 Fixes Begins */
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType isUeSISO(
        /* SPR 5599 changes end (ZIP ID 129059) */
        UInt32  ueIndex);
/* SPR 3172 Fixes End */


MacRetType reConfigSchedReqInPUCCHMap(
        ULUEContext *ulUeCtx_p,
        /* +- SPR 17777 */
        UeContextForUplink *uplkUEInfo_p
        /*SPR 16855 +-*/
        ,ULReconfigSRMsgType *srReconfigRetValue_p,
        UeContextForReconfig *reconfigInfo_p
        /*SPR 16855 +-*/
        ,InternalCellIndex internalCellIndex
        );
/*Reconfig Changes End */

/* SPR 15909 fix start */
void processPrachReportForSpecialSubframes( UInt8 ulSubFrameNum, 
    tickType_t globalTick, InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */

/* SPR 609 changes start */
/* SPR 5599 changes start (ZIP ID 129059) */
UInt8 calculateReportFormatBitWidthPeriodic(ueExpectedReportType feedbackType,
        /* SPR 5599 changes end (ZIP ID 129059) */
        /* TM8_4*2_Mimo */
        UInt8 ri, UInt8 ueCategory, UInt8 transMode,
        InternalCellIndex internalCellIndex);
/* TM8_4*2_Mimo */
/* SPR 609 changes end */
/* Review comment fix start GG15 */
MacRetType calculateMaxPucchRegion(InternalCellIndex internalCellIndex);
MacRetType macValidateUePucchResources(UeContextForUplink  *uplkUEInfo_p,
        InternalCellIndex internalCellIndex);
void calculatePHICHGroup(InternalCellIndex internalCellIndex);
void calculateMaxCCE(InternalCellIndex internalCellIndex);
void calculateCFIValue(InternalCellIndex internalCellIndex);
/* Review comment fix end GG15 */

/* CA PUCCH Phase2 changes start */
void deletePCellPucchNodesForUE(
        PucchConfig *pucchConfigInfo_p,
        ReportAndOpportunity reportType,
        InternalCellIndex internalCellIndex
        );

MacRetType initPCellCQIAndRIinPUCCHMap (
        /*SPR 16855 +-*/
        UInt16 ueIndex,
        PucchConfig * pucchConfigInfo_p,
        /*SPR 16855 +-*/
        /* +- SPR 17777 */
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink*  uplkUEInfo_p,
        InternalCellIndex internalCellIndex
        );

MacRetType reConfigPCellRIAndCQIMap ( 
        ULUEContext *ulUEContext_p, 
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink *uplkUEInfo_p
        /*SPR 16855 +-*/
        ,ULReconfigCQIRIMsgType *cqiRiReconfigRetValue_p,
        UeContextForReconfig *reconfigInfo_p 
        /*SPR 16855 +-*/
        ,InternalCellIndex internalCellIndex
        );

MacRetType reconfigurePCellCqiRiPassiveContext(
        ULUEContext          *ulUEContext_p,
        UInt16               ueIndex,
        PucchConfig          *pucchConfigInfo_p,
        /* +- SPR 17777 */
        UeContextForUplink   *uplkUEInfo_p,
        UeContextForReconfig *reconfigInfo_p,
        InternalCellIndex    internalCellIndex
        );

MacRetType initSCellCQIAndRIinPUCCHMap (
        /*SPR 16855 +-*/
        UInt16 ueIndex,
        /*SPR 16855 +-*/
        /* +- SPR 17777 */
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink*  uplkUEInfo_p,
        UeCellInfo * ueCellInfo_p,
        InternalCellIndex internalCellIndex,
        UInt8 rrcLoopIndex
        );
#ifdef FDD_CONFIG
void updateSCellPUCCHConfig( UInt32 deltaPUCCHSHIFT,
        UInt32 nRBCQI,
        UInt32 nCSAN,
        UInt32 n1PucchDLAckNack,
        UeCellInfo * ueCellInfo_p,
        UInt16 ueIndex
        );
#endif

void deleteSCellPucchNodesForUE(
/** SPR 12320 Fix : START **/
        PucchConfig * pucchConfigInfo_p,
/** SPR 12320 Fix : END **/
        ReportAndOpportunity reportType,
        UeCellInfo * ueCellInfo_p,
        InternalCellIndex internalCellIndex,
        UInt16 ueIndex
        );


/* EICIC +*/
MacRetType reConfigPCellRIAndCQIMapAbs ( 
        ULUEContext *ulUEContext_p, 
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink *uplkUEInfo_p,
        /*SPR 16855 +-*/
        ULReconfigCQIRIMsgType *cqiRiReconfigRetValue_p,
        UeContextForReconfig *reconfigInfo_p,
        /*SPR 16855 +-*/
        UInt8 internalCellIndex
        );
/* EICIC -*/

MacRetType reConfigSCellRIAndCQIMap ( 
        ULUEContext *ulUeCtx_p, 
        UeContextForDownlink *dwlkUEInfo_p,
        UeContextForUplink *uplkUEInfo_p
        /*SPR 16855 +-*/
        ,ULReconfigCQIRIMsgType *cqiRiReconfigRetValue_p,
        UeContextForReconfig *reconfigInfo_p, 
        UeCellInfo * ueCellInfo_p
        /*SPR 16855 +-*/
        ,InternalCellIndex internalCellIndex
        );

MacRetType reconfigureSCellCqiRiPassiveContext(
        ULUEContext          *ulUeCtx_p,
        UInt16               ueIndex,
        /* +- SPR 17777 */
        UeContextForUplink   *uplkUEInfo_p,
        UeContextForReconfig *reconfigInfo_p,
        UeCellInfo * ueCellInfo_p,
        InternalCellIndex    internalCellIndex
        );
/* CA PUCCH Phase2 changes end */
/*Complexity Start*/
void eicicUpdateNodeInPucchDb(
	ReportAndOpportunity ueReportOrOpportunityType,
	globalSrOpportunityAndPeriodicReports *listNode_p,
	InternalCellIndex internalCellIndex,
	UInt16 exactWrapRound);
/*Complexity End*/

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
/* SPR 15909 fix start */
void processPrachReportForSpecialSubframes( UInt8 ulSubFrameNum,
        tickType_t globalTick, InternalCellIndex internalCellIndex );
/* SPR 15909 fix end */
/* SPR 2617 End*/

/* SPR 19679: TDD HARQ Multiplexing Changes Start */
#ifdef TDD_CONFIG
/*SPR 19504 fix start*/
 void cleanUpUeScheduledInfo(
        UeScheduledInfo * ulUeScheduledInfo_p);
/*SPR 19504 fix end*/
UInt8 fillHARQSizeForMultiplexingOrBundling ( UInt8 ackNackMode,
					      /*SPR 20564 Changes Start*/
					      /*Code Removed*/
					      /*SPR 20564 Changes End*/ 
                                              UInt16 ueIndex,
                                              UInt8 subFrameNum,
                                              InternalCellIndex internalCellIndex
                                              ,UeScheduledInfo * ulUeScheduledInfo_p);
#endif


/* SPR 19679: TDD HARQ Multiplexing Changes End */
#endif  /* LTE_MAC_PUCCH_H */

