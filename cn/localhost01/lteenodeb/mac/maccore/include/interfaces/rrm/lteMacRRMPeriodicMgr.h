/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: lteMacRRMPeriodicMgr.h 
 *
 *************************************************************************
 *
 * File Description : This file contains the declaration of structure that
 *                    is used for storing periodic report configuration.
 *************************************************************************/
/**************************************************************************
 * Project Includes
 *************************************************************************/
#ifndef LTE_MAC_RRM_PERIODIC_MGR_H
#define LTE_MAC_RRM_PERIODIC_MGR_H

#include "lteMacTypes.h"
#include "lteMacRRMInterface.h"


/**************************************************************************
 * Exported Definitions
 *************************************************************************/
#define MAX_NUM_OF_BATCH MAX_UE_SUPPORTED
#define MAX_PERIODIC_REPORT_INDEX 2

#define MIN_PERIODICITY_PERIODIC_REPORT 100
#define MIN_PERIODICITY 20
#define MIN_UE_BATCH_SIZE 1
#define MAX_UE_BATCH_SIZE 25
#define MAX_BATCH 25
#define TIME_AVG_COEFFICIENT_IIR 820

#define RNTP_THRESHOLD_MIN_VAL 0 
#define RNTP_THRESHOLD_MAX_VAL 15

#define TRIGGER_PERIODIC_REPORT         1
#define TRIGGER_L1_MEAS_REPORT          2
/* SPR 21201 start */
#define TRIGGER_L2_ICIC_MEAS_REPORT     4
/* EICIC Change Phase1 */
#define TRIGGER_L2_EICIC_MEAS_REPORT    8
/* EICIC Change Phase1 */
/* SPR 21201 end   */

#define MAX_REPORT_INDEX 2
/****************************************************************************
 * Exported Types
 ***************************************************************************/

typedef struct ULPeriodicReportInfoT
{
    UInt32 uplinkSINRValue;
    UInt32 uplinkBlerValue;
    UInt32 uplinkPathlossValue;
    /* TTIB_Code Start */
    /* SPR 8104 */
    /* + SPR 19724 */
    UInt32 uplinkPRB;
    /* - SPR 19724 */
    /* SPR 8104 */
    /* TTIB_Code End */
}ULPeriodicReportInfo;

/* Structure for storing DL UE periodic report */
typedef struct UEPeriodicReportT
{
    SInt32               shmId;
    ULPeriodicReportInfo *ulPeriodicReportInfo_p;
}ULUEPeriodicReport;
/* Structure used for storing UE's in a batch */
typedef struct PeriodicReportUEBatchT
{
    /* +- SPR 18268 */
    UInt16 numOfUE;
    UInt16 ueID[MAX_UE_SUPPORTED];
    /* +- SPR 18268 */
}PeriodicReportUEBatch;
typedef struct PeriodicReportParametersT
{
    /* Data communicated from RRM */
    UInt32 periodicReportsRequired;
    UInt32 periodicity;
    UInt32 ueBatchSize;

    /* Information derrived from above data */
    UInt32 numOfBatches;
    UInt32 batchInterval;
}PeriodicReportParameters;

/* Structure used for storing UE indices and to which batch a UE belongs to */
typedef struct UEIDToBatchNumMapT
{
    UInt32 ueIndex;
    UInt32 batchNum;
}UEIDToBatchNumMap;

/* +DYNAMIC_ICIC */
typedef struct DLRntpReportConfigParamsT
{
    UInt32 bitmask;
    UInt32 rntpReportRequired;
    SInt32 rntpThreshold;
    SInt32 maxNominalEPRE;
}DLRntpReportConfigParams;
typedef struct Layer1ReportConfigParamsT
{
    /* Data communicated from RRM */
    UInt32 layer1MeasReportRequired;
    UInt32 layer1ReportAveragingEnabled;
    UInt32 periodicity;
}Layer1ReportConfigParams;
typedef struct ICICReportConfigParamsT
{
    /* Data communicated from RRM */
    UInt32 cellEdgePrbUsageRequired;
    DLRntpReportConfigParams rntpReportingConfigParams;
    UInt32 periodicity;
}ICICReportConfigParams;

/* Structure which is to be written on shared memory for sending
 * L1 and ICIC report to RRM */
typedef struct EICICReportConfigParamsT
{
    /* Data communicated from RRM */
    UInt32 absUsageRequired;
    UInt32 periodicity;
}EICICReportConfigParams;

typedef struct Layer1ReportInfoT
{
    SInt32 interferencePerPRB[100];
    SInt32 thermalNoise;
    SInt32 dlRsTxPower;
}Layer1ReportInfo;
/* Strycture for containing DL ICIC related parameters */
typedef struct DLICICReportInfoT
{
    UInt32 rntpValuePerPRB[100];
    UInt32 dlCEPrbUsage;
}DLICICReportInfo;

typedef struct DLEICICReportInfoT
{
    UInt32 numABSoccasionsKpi;
    UInt32 absPRBUsage;
    UInt32 absPerQci[MAX_QCI];
    UInt32 cePRBUsage;
    UInt32 ccPRBUsage;
    UInt32 totalCEPrb;
    UInt32 totalCCPrb;
}DLEICICReportInfo;

/* Structure for containing UL ICIC related parameters */
typedef struct ULICICReportInfoT
{
    UInt32 ulCEPrbUsage;
}ULICICReportInfo;

typedef struct Layer1ReportIndT
{
    SInt32 shmId;
    Layer1ReportInfo *layer1ReportInfo_p;
}Layer1ReportInd;

typedef struct ABSCountInfoT
{
    UInt32 absSubframeCount;
}ABSCountInfo;

typedef struct ICICReportIndT
{
    SInt32 dlShmId;
    SInt32 ulShmId;
    /* Making two seperate structures for DL and UL */

    /* Array of 2 is declared for active passive indices concept,
     * High priority thread will write the values at one index and low priority
     * thread will read from other.
     */
    DLICICReportInfo *dlICICReportInfo_p;
    ULICICReportInfo *ulICICReportInfo_p;
    ABSCountInfo *absCountInfo_p;
}ICICReportInd;

/* -DYNAMIC_ICIC */

/*EICIC*/

typedef struct EICICReportIndT
{
 
    DLEICICReportInfo *dlEICICReportInfo_p;
   
}EICICReportInd;
/*EICIC*/

typedef struct DlSCellPeriodicReportT
{
    UInt8 downlinkSINRValueCW0;
    UInt8 downlinkSINRValueCW1;
    UInt8 downlinkBlerValueCW0;
    UInt8 downlinkBlerValueCW1;
    /*+EICIC */
    UInt8 downlinkSINRValueCW0_2;
    UInt8 downlinkSINRValueCW1_2;
    UInt8 downlinkBlerValueCW0_2;
    UInt8 downlinkBlerValueCW1_2;
   /*-EICIC */
}DlSCellPeriodicReport;

typedef struct DLPeriodicReportInfoT
{
    UInt32 downlinkSINRValueCW0;
    UInt32 downlinkSINRValueCW1;
    UInt32 downlinkBlerValueCW0;
    UInt32 downlinkBlerValueCW1;
    UInt32 timingAdvanceValue;
    /*4x4 DL MIMO CHG START*/
    UInt32 downlinkSINRValueCW0Layers2;
    UInt32 downlinkSINRValueCW1Layers2;
    UInt32 downlinkBlerValueCW0Layers2;
    UInt32 downlinkBlerValueCW1Layers2;
    /*4x4 DL MIMO CHG END*/
   /* SPR 5033 changes start */
    UInt32 pucchSINRValue;
    UInt32 pucchBlerValue;
   /* SPR 5033 changes end */
    /* CA changes start */
    /* Currently only 1 scell with Scell Index 1 is supported */
    DlSCellPeriodicReport dlSCellPeriodicReport[MAX_NUM_SCELL];
    /* CA changes end*/
    /*EICIC*/
	csiSubframeSet2PeriodicReport csiSubframeSet2Report;
    /*EICIC*/	
    /*+EICIC */
    UInt32 downlinkSINRValueCW0_2;
    UInt32 downlinkSINRValueCW1_2; 
   /*-EICIC */
    UInt8 downlinkBlerValueCW0_2;
    UInt8 downlinkBlerValueCW1_2;
    UInt8 downlinkSINRValueCW0Layers2_2;
    UInt8 downlinkSINRValueCW1Layers2_2;
    UInt8 downlinkBlerValueCW0Layers2_2;
    UInt8 downlinkBlerValueCW1Layers2_2;
	
}DLPeriodicReportInfo;

/* Structure for storing DL UE periodic report */
typedef struct DLUEPeriodicReportT
{
    SInt32               shmId;
    DLPeriodicReportInfo *dlPeriodicReportInfo_p;
}DLUEPeriodicReport;

/*CA changes start*/
typedef struct HpLpPeriodicReportIndT
{
    InternalCellIndex internalCellIndex;
    UInt8 periodicReportTriggerType;
}HpLpPeriodicReportInd;
/*grouping all cell scpecific data structure for RRM MAC interface*/
typedef struct RRMMACPeriodicMgrCellGlobalT
{
    PeriodicReportUEBatch periodicReportUEBatch[MAX_NUM_OF_BATCH];
    DLUEPeriodicReport dlUEPeriodicReport;
    ULUEPeriodicReport ulUEPeriodicReport;
    PeriodicReportParameters periodicReportParams;
    Layer1ReportInd layer1ReportInd;
    ICICReportInd icicReportInd;
    Layer1ReportConfigParams layer1ReportConfigParams;
    ICICReportConfigParams icicReportConfigParams;
    /* EICIC Change Phase1 */	
    EICICReportConfigParams eicicReportConfigParams;
    EICICReportInd eicicReportInd;
    /* EICIC Change Phase1 */	
    UInt32 ueIDToBatchNumMapping[MAX_UE_SUPPORTED];
    UInt32 updateUEBatchCurrentIndex;
    UInt32 periodicReportCurrentBatchIndex;
    /* SPR 15909 fix start */
    tickType_t nextPeriodicReportOppor;
    /* SPR 15909 fix end */
    /* SPR 15909 fix start */
    tickType_t lastPeriodicReportTick;
    UInt32 currentBatchNum;
    UInt32 reportPeriodicity;
    tickType_t nextL1ReportOppor;
    tickType_t nextICICReportOppor;
    /* SPR 15909 fix end */
    /* EICIC Change Phase1 */		
    UInt32 nextEICICABSReportOppor;
    /* EICIC Change Phase1 */	
    UInt8 *bufferForPeriodicReport_p;
    UInt8 *bufferForLayer1Report_p;
    UInt8 *bufferForICICReport_p;
    /* EICIC Change Phase1 */	
    UInt8 *bufferForEICICReport_p;
    /* EICIC Change Phase1 */
    UInt8 activeReportIndex;
    UInt8 activeReportIndexEicic;
    UInt32 nextEICICReportOppor;
}RRMMACPeriodicMgrCellGlobal;

extern RRMMACPeriodicMgrCellGlobal *rrmMacPeriodicMgrCellGlobal_gp[MAX_NUM_CELL];
/*ca changes end*/
/****************************************************************************
 * Exported Variables
 ***************************************************************************/

/***************************************************************************
 *  Exported Functions
 ****************************************************************************/
/* This klockwork has fixed 10683 SPR for this file */
/*KLOCKWORK_CHANGES_START_162*/
void  addUEPeriodicReportBatchInfo(UInt32 ueIndex, 
                        InternalCellIndex internalCellIndex);
/*KLOCKWORK_CHANGES_STOP_162*/
MacRetType deleteUEPeriodicReportBatchInfo(UInt32 ueIndex, 
                        InternalCellIndex internalCellIndex);
/* SPR 5599 changes start (ZIP ID 129059) */
void updateULSINR(UInt32 ueIndex, UInt8 averageSINR, 
                        InternalCellIndex internalCellIndex);
void updateULBler(UInt32 ueIndex, SInt8 puschBler, 
                        InternalCellIndex internalCellIndex);
void updateULPathLoss(UInt32 ueIndex, SInt16 pathLoss, 
                        InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */
UInt32 initSHMForRRMPeriodicReport(InternalCellIndex internalCellIndex);
void processPeriodicReport(InternalCellIndex internalCellIndex);
/* SPR 5599 changes start (ZIP ID 129059) */
void updateDLSINRCW0(UInt16 ueIndex,UInt8 cqiValue, 
                        InternalCellIndex internalCellIndex);
void updateDLSINRCW1(UInt16 ueIndex,UInt8 cqiValue, 
                        InternalCellIndex internalCellIndex);
void updateDLBlerCW0(UInt32 ueIndex, SInt8 bler, 
                        InternalCellIndex internalCellIndex);
void updateDLBlerCW1(UInt32 ueIndex, SInt8 bler, 
                        InternalCellIndex internalCellIndex);
void updateDLBlerCW0_2(UInt32 ueIndex, SInt8 bler, 
                        InternalCellIndex internalCellIndex);
void updateDLBlerCW1_2(UInt32 ueIndex, SInt8 bler, 
                        InternalCellIndex internalCellIndex);
void updateDLTA(UInt32 ueIndex, UInt16 timingAdvanceValue, 
                        InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */
/*+EICIC */
void updateDLSINRCW0_2(UInt16 ueIndex,UInt8 cqiValue, 
                        InternalCellIndex internalCellIndex);
void updateDLSINRCW1_2(UInt16 ueIndex,UInt8 cqiValue, 
                        InternalCellIndex internalCellIndex);
 void macUupdateSCellDLSINRCW0_2(UInt16 ueIndex, UInt8 cqiValue,
                        InternalCellIndex internalCellIndex, 
                        UInt8 scellIndex);
 void macUpdateSCellDLSINRCW1_2(UInt16 ueIndex,UInt8 cqiValue,
                        InternalCellIndex internalCellIndex,
                        UInt8 scellIndex);
 void macUpdateSCellDLBlerCW0_2(UInt32 ueIndex, SInt8 bler,
                        InternalCellIndex internalCellIndex,
                        UInt8 scellIndex);
 void macUpdateSCellDLBlerCW1_2(UInt32 ueIndex, SInt8 bler,
                        InternalCellIndex internalCellIndex, 
                        UInt8 scellIndex);
/*-EICIC */

 void updateBitMask(UInt16 ueIndex,UInt32 bitMask, 
                        InternalCellIndex internalCellIndex );
 void updateSCellReportBitMask(UInt16 ueIndex,
                        UInt8 scellReportBitMask,
                        InternalCellIndex internalCellIndex);

/* SPR 5033 changes start */
 void updatePucchSINR(UInt16 ueIndex,UInt8 pucchSINR, 
                        InternalCellIndex internalCellIndex);
 void updatePucchBler(UInt16 ueIndex,SInt8 pucchBler, 
                        InternalCellIndex internalCellIndex);
/* SPR 5033 changes end */
void initPeriodicReportInfo(UInt32 ueIndex, 
                        InternalCellIndex internalCellIndex);
    /*coverity-530 CID-32293*/
RrmMacCellConfigErrorCode reconfigPeriodicReport(
                PeriodicReportConfig *periodicReportConfig_p,
                InternalCellIndex internalCellIndex);
    /*coverity-530 CID-32293*/
void initReInitPeriodicReportManager(InternalCellIndex internalCellIndex);
void checkForRRMPerriodicReportOpp(InternalCellIndex internalCellIndex);
/* +DYNAMIC_ICIC */
/*coverity-530 CID-32291*/
RrmMacCellConfigErrorCode configureLayer1Report(
                Layer1MeasReportConfig *layer1MeasReportConfig_p,
                InternalCellIndex internalCellIndex);
/*coverity-530 CID-32291*/
/*coverity-530 CID-32292*/
RrmMacCellConfigErrorCode configureICICReport(
            Layer2IcicMeasReportConfig *layer2icicMeasReportConfig_p,
            InternalCellIndex internalCellIndex);

RrmMacCellConfigErrorCode configureEICICReport(
            Layer2EicicMeasReportConfig *layer2EicicMeasReportConfig_p,
            InternalCellIndex internalCellIndex);

/*coverity-530 CID-32292*/
MacRetType validateULPowerMask(ULPowerMask *ulPowerMask_p);
void processLayer1Report(InternalCellIndex internalCellIndex);
void processICICReport(InternalCellIndex internalCellIndex);
/* -DYNAMIC_ICIC */
/*+ EICIC*/
void processEICICReport(InternalCellIndex internalCellIndex);
/*-EICIC*/
/* TTIB_Code Start */
    /* SPR 8104 */
//CODE_COMMENT_L10_FIXED
/* SPR 18641_18843 start */
void updateULPHR(UInt16 ueIndex, UInt32 PHR, 
                        InternalCellIndex internalCellIndex);
/* SPR 18641_18843 end */
    /* SPR 8104 */
 /* TTIB_Code End */
/* CA changes start */
void macUupdateSCellDLSINRCW0(UInt16 ueIndex, UInt8 cqiValue,
        /* +- SPR 17777 */
                            UInt8 scellIndex);
void macUpdateSCellDLSINRCW1(UInt16 ueIndex,UInt8 cqiValue,
        /* +- SPR 17777 */
                            UInt8 scellIndex);
void macUpdateSCellDLBlerCW0(UInt32 ueIndex, SInt8 bler,
                            /* +- SPR 17777 */
                            UInt8 scellIndex);
void macUpdateSCellDLBlerCW1(UInt32 ueIndex, SInt8 bler,
                            /* +- SPR 17777 */
                            UInt8 scellIndex);
extern void macInterfaceProcessRRMPeriodicReports(UInt8 *buffer_p);
/* CA changes end */
#endif
