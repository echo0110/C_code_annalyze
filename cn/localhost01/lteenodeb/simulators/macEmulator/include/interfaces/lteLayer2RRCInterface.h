/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2RRCInterface.h,v 1.1.1.1.16.1 2010/09/21 15:53:25 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations for the RRC Interface
 *                     with the LTE MAC Layer.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLayer2RRCInterface.h,v $
 * Revision 1.1.1.1.16.1  2010/09/21 15:53:25  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3  2009/07/17 06:25:59  gur20439
 * changes related to rrc interface modification incorporated.
 *
 * Revision 1.2  2009/07/14 14:45:02  gur20439
 * rrc interface code.
 *
 * Revision 1.1  2009/03/30 10:20:28  gur19836
 * Initial Version
 *
 * Revision 1.26  2009/02/20 14:34:20  gur18569
 * added maxUeIndex_g
 *
 * Revision 1.25  2009/01/20 07:54:00  gur19836
 * Changes done for interface with RRC Layer
 *
 * Revision 1.23  2009/01/09 16:09:13  gur18569
 * added downlink nack ql wt
 *
 * Revision 1.22  2009/01/08 10:29:13  gur20052
 * Added uplkLCGPriorityWt config parameter for UL LCG Priority Weight
 *
 * Revision 1.21  2009/01/05 09:06:37  gur20052
 * Default values of UL parameters added
 *
 * Revision 1.20  2008/12/30 14:17:34  gur11912
 * Phase 2 UT Defect Fixed
 *
 * Revision 1.19  2008/12/26 05:38:07  gur11912
 * modified max value of RBs
 *
 * Revision 1.18  2008/12/22 13:43:10  gur18569
 * review comments incorporated for phase 2
 *
 * Revision 1.17  2008/12/19 09:20:36  gur18569
 * added  rbsULConfiguredSem_g and rbsDLConfiguredSem_g
 *
 * Revision 1.16  2008/12/19 05:41:22  gur18569
 *  removed redeclaration
 *
 * Revision 1.15  2008/12/19 05:37:43  gur18569
 * change in addMACDLLogicalChannel argument
 *
 * Revision 1.14  2008/12/18 13:37:19  gur18569
 * "shifted delCnfFromLayerT to lteLayer2RRCInterface.h"
 *
 * Revision 1.13  2008/12/11 14:12:01  gur11912
 * Downlink Phase 2 changes
 *
 * Revision 1.12  2008/12/10 07:54:17  gur20052
 * Initial version for Phase 2
 *
 * Revision 1.11  2008/12/10 06:05:24  gur19836
 * updated for linking
 *
 * Revision 1.10  2008/12/09 11:51:19  gur19836
 * Updated during linking
 *
 * Revision 1.9  2008/12/08 12:55:50  gur12905
 * Modified during Phase - 2 merging
 *
 * Revision 1.8  2008/12/05 13:23:43  gur18569
 * multicore changes
 *
 * Revision 1.7  2008/10/17 09:24:00  gur19836
 * Enhancement: cleanupMACLayer functionality added
 *
 * Revision 1.6  2008/10/16 05:34:58  gur18569
 * added prototype of deleteNUeEntities
 *
 * Revision 1.5  2008/09/30 09:32:56  gur19836
 * UT Defect fix
 *
 * Revision 1.4  2008/09/19 12:16:12  gur11912
 * MAX MIN limit used
 *
 * Revision 1.3  2008/09/19 07:15:18  gur11912
 * review comments incorporated
 *
 * Revision 1.2  2008/09/09 16:10:12  gur11974
 * Updating for integration
 *
 * Revision 1.1  2008/09/09 05:13:56  gur11974
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTELAYER2_RRC_INTERFACE_H
#define LTELAYER2_RRC_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteLayer2Types.h"
#include "lteLayer2UEContext.h"



/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#ifdef TDD_CONFIG
/* Sunny changes */
/* SPR 7964 Fixes Start */
/* 
 *  Array size changed from 4x4 to 10x10 
 *  
*/
 extern UInt8 tb1_ack_nack_value_array[MAX_NUM_CELLS][10][10];
 extern UInt8 tb2_ack_nack_value_array[MAX_NUM_CELLS][10][10];
/* SPR 7964 Fixes End */
#elif FDD_CONFIG
 extern UInt8 tb1_ack_nack_value_array[MAX_NUM_CELLS][10][10];
 extern UInt8 tb2_ack_nack_value_array[MAX_NUM_CELLS][10][10];
#endif
/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* Enum for differentiating between the addition, deletion and reconfiguring operations for the Logical Channels of the UE */
typedef enum LogicalChannelOperationTypeT
{
    ADD =0,                             /* Add the corresponding Logical Channel */
    RECONFIGURE,                         /* Reconfigure the corresponding Logical Channel */
    DELETE                               /* Delete the corresponding Logical Channel */ 
}LCOperationType;

/*Structure maintaining all the Logical Channel Configuration Information */
typedef struct LogicalChannelConfigInfoT
{
    LCOperationType operationType;      /* Logical Channel Operation Type */
    UInt8  lchId;                       /* Logical Channel Identifier */
    UInt8  lchPriority;                 /* Logical Channel Priority */
    //UInt8  maxRB;                       /* Maximum number of resource blocks for this logical channel per TB */
    UInt8  rlcMode;
    UInt8  rlcSNFieldLength;
    UInt16 result;                      /* Result of LC Config Req to send response to RRC Layer */
}LogicalChannelConfigInfo;
typedef struct ueContextForDownlinkT
{
    //HarqScheme hScheme;
    LogicalChannelConfigInfo logicalChConfig[10];
    UInt16 cRnti;
    UInt16 ueIndex;
    UInt8  precodingIndex;
    UInt8 transmissionMode;
    UInt8 numOfLayer;
    //UInt16 cqiTimer;
    UInt8 uePriority;
    //UInt32 codingRate;
    UInt8 mcsIndex;
    UInt8 maxRB;
    UInt8 numHarqProcess;
    UInt8 dwlkLogicalChCount;
    //UInt8  modulationScheme;     
    UInt8  hScheme;
    UInt8 rankIndicator;
    UInt8 rlcMode;
    UInt8 serviceType;
    /* + CQI_4.1 */
    UInt8 cqiInfoFlag;
    /* - CQI_4.1 */
    UInt8 cqiIndication;
    UInt8 cqiMode;
    /* + DRX CHG */
    UInt8 drxConfigType;
    DRXContext drxContext;
    /* - DRX CHG */
    /* + MEAS_GAP_CHG */
    UInt8 measGapConfigType;
    MeasurmentGapConfig measGapConfig;
    UInt8  measurementGapRequestType;
    /* - MEAS_GAP_CHG */
    /*CLPC_CHG*/
    /*Configuration information of TPC for PUCCH*/
    TpcPdcchConfig tpcPdcchConfigPucch;
    UInt8 pA;
    UInt8 isPAPresent;
    UInt8 isTpcPdcchConfigPucchPresent;
    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
    UInt8 isHDFddFlag;
#endif
    /* HD FDD Changes End */
    /*CLPC_CHG*/
    /* SPS_CHG_START */
    UInt8 isSpsDlInfoPresent;
    /* SPS Configuration Information for Downlink */
    spsDlInfo spsDownlinkInfo;
    /* SPS_CHG_END */
    UInt8 ueCategory;
    /* + TM7_8 Changes Start */
    UInt8 pmiRiReportV920;
    /* - TM7_8 Changes End */
	/* + CA_PHASE2 changes start*/
	UInt8     trigger1r10;
	UInt8     trigger2r10;
	UInt8 ueCategoryV1020;
	UInt8 ueCategoryReqType ;
	PucchFormatR10 pucchFormatR10;
	CarrierAggrConfig carrierAggrConfig;
	/* - CA_PHASE2 changes end*/
#if 0
    /*CA changes start*/
    /*primary cell index*/
    UInt8 cellIndex;
    /*Count of Scells configured for this UE*/
    UInt8 scellCount;
    /*Secondary Cell contexts for each of the SCELL configured for the UE*/
    DLUESCellContext *dlUeScellContext_p[MAX_NUM_SCELL];
    /*CA changes end*/
#endif 
	/* EICIC +*/
     UInt8 cqiConfigRequestType;
     UInt16 cqiPMIConfigIndex_2;
     UInt16 riConfigIndex_2;
     UInt8 cqiSubsetConfigRequestType;
     UInt8 csiMeasurementSubset1[NUM_BITS_ABS_PATTERN];
     UInt8 csiMeasurementSubset2[NUM_BITS_ABS_PATTERN];
    UInt8 dlIMcs;
    UInt8 timeAlignmentTimerR13;
    UInt8 nPdcchNumRepetitionR13;
    UInt8 nPdcchStartSfUssR13;
    UInt8 nPdcchOffsetUssR13;

    /*NbDrxContext nbDrxInfo;
    DlCarrierConfigDedicated dlCarrierConfig;*/

     /* EICIC -*/
}ueContextForDownlink;


enum
{

  UL_DL_CONFIG_0 = 0,
  UL_DL_CONFIG_1,
  UL_DL_CONFIG_2,
  UL_DL_CONFIG_3,
  UL_DL_CONFIG_4,
  UL_DL_CONFIG_5,
  UL_DL_CONFIG_6,
  UL_DL_CONFIG_MAX
};

/*Structure for RNTIs required by UL*/
typedef struct RntiT{
    UInt16  CRnti;
    UInt16  SpsCRnti;  /*semi persistent scheduling */
    UInt16  TpcPucchRnti ;
    UInt16  TpcPuschRnti;
}ULRnti;

/* newly added structure */
/* structure for logical channel information Uplink*/
typedef struct ulLogicalChannelInfoT
{
    LCOperationType     operationType;
    UInt8               lchId;
    UInt8               rlcMode;
    UInt8               rlcSNFieldLength;
    UInt8               lcGId; //if invalid, then Set XX value, where XX>3
    UInt8               qci;
    UInt8               isSpsLc; 
    UInt8               bearerType; /* 0 - GBR, 1 - NONGBR */
    UInt16              result;
    UInt32              queueLoad;
} ulLogicalChannelInfo;

typedef enum ApiIdT {
    CREATE_UE_ENTITY_REQ = 1,
    DELETE_UE_ENTITY_REQ,
    RECONFIG_UE_ENTITY_REQ,
    CONFIG_AVAIL_RES_BLOCKS_REQ,
    GET_T_CRNTI_REQ,
    RLC_REESTABLISH_REQ,
    CONFIG_HARQ_PROFILE,
    CREATE_UE_ENTITY_RESP,
    DELETE_UE_ENTITY_RESP,
    RECONFIG_UE_ENTITY_RESP,
    GET_T_CRNTI_RESP,
    RLC_REESTABLISH_RESP,
    POWER_HEADROOM_INFO,
    CCCH_PDU,
    CRNTI_MAC_ELEMENT,
    MAX_MSG_ID = 255
} ApiId;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define FREQUENCY_SELECTIVE_OFF       0
#define FREQUENCY_SELECTIVE_ON        1


//#define MAX_NUM_UE 350
#define MAC_UE_ACTIVE 1
#define MAC_UE_IDLE 0
#define MAC_RLC_BUFFER_MEMCOUNT 16

/* MAC default values */
/* MAC Downlink Scheduler Weights */
#define MAC_SCH_MAX_DWLK_UE_NACK_QL_WT    20
#define MAC_SCH_DEFAULT_DWLK_UE_NACK_QL_WT     10

//HARQ Processes
#define MAC_MIN_HARQ_PROCESSES 6
#define MAC_MAX_HARQ_PROCESSES 8
#define MAC_DEFAULT_NUM_HARQ_PROCESSES 8

/*HARQ Timer*/
#define MAC_MIN_HARQ_TIMER_VALUE 5
#define MAC_MAX_HARQ_TIMER_VALUE 8
#define MAC_DEFAULT_HARQ_TIMER_VALUE 6

/*HARQ Retransmissions*/
#define MAC_MIN_HARQ_RETRANSMISSIONS 6
#define MAC_MAX_HARQ_RETRANSMISSIONS 8
#define MAC_DEFAULT_HARQ_RETRANSMISSIONS 6

#define MAC_DEFAULT_NUM_RESOURCE_BLOCKS 25
#define MAC_DEFAULT_MODULATION_SCHEME 0
#define MAC_DEFAULT_HARQ_SCHEME 2

#define MAC_MIN_RESOURCE_BLOCKS 6
#define MAC_MAX_RESOURCE_BLOCKS 110
#define MAC_MIN_RANGE_VALUE 1
#define MAC_MAX_RANGE_VALUE 110

#define MIN_ALLOCATABLE_RB 1
#define CONFIG_CELL_REQ_MIN_LEN 25
#define TAG_LEN 4
#define RACH_CONFIG_LEN 5
#define PHICH_CONFIG_LEN 2
#define PUCCH_CONFIG_LEN 5
#define MAC_MAX_NRB_CQI 98
#define MAC_MAX_NCS_AN 7
#define MAC_MAX_N1_PUCCH_AN 2047
#define INVALID_LCID 255
#define INVALID_RLC_MODE   255
#define INVALID_TA_VALUE 255
#define INVALID_ALREADY_CONSIDERED_FLAG  255
#define LCGID_INVALID_VALUE 5
#define INVALID_SIMULTANEOUS_ACK_NACK_VALUE 255
#define INVALID_CODE_BOOK_INDEX 100
/*CLPC_CHG*/
#define ULP_CONTROL_DEDICATED_PARAM_LEN 3
#define TPC_PDCCH_CONFIG_PARAM_LENGTH 1
#define TPC_RNTI_INFO_PARAM_LENGTH 4
#define MIN_TPC_INDEX_VALUE 0
#define MAX_TPC_INDEX_VALUE 31
#define MIN_P0_UE_PUSCH_VALUE -8
#define MAX_P0_UE_PUSCH_VALUE 7
#define MIN_TPC_INDEX_VALUE_DCI3    1
#define MAX_TPC_INDEX_VALUE_DCI3    15
#define MIN_TPC_INDEX_VALUE_DCI3A   1
#define MAX_TPC_INDEX_VALUE_DCI3A   31

typedef struct TPCRntiRangeT
{
    /*Indicates start value for range of TPC RNTI to be configured by L3*/
    UInt16 startTpcRntiPucch;
    /*Indicates end value for range of TPC RNTI to be configured by L3*/
    UInt16 endTpcRntiPucch;
    /*Indicates start value for range of TPC RNTI to be configured by L3*/
    UInt16 startTpcRntiPusch;
    /*Indicates end value for range of TPC RNTI to be configured by L3*/
    UInt16 endTpcRntiPusch;
}TPCRntiRange;
/*CLPC_CHG*/

/* + CA_AB */
#define MAX_QCI 9
typedef struct ueSpsSysWideParamT
{
    /* QCI sps map */
    UInt8  qciSpslist[MAX_QCI];
    
}ueSpsSysWideParam;

/* + CA_AB */
typedef struct CellConfigT
{
    UInt8   dlAvailableRBs;
    UInt8   ulAvailableRBs;
    /* Start of the Resource Block Range */
    //UInt8   dlStartOfRange;
    //UInt8   ulStartOfRange;
    
    /* End of the Resource Block Range */
    //UInt8   dlEndofRange;                    
    //UInt8   ulEndOfRange;
    /* Number of HARQ Process per UE*/
    UInt8   numHARQProcess;    
    /* Maximum number of HARQ re-transmission */ 
    UInt8   maxHarqRetransmission;            
  
    UInt8   phichDuration;
    UInt8   phichResource;
    UInt8   cyclicPrefix;
        
    UInt16 spscrntiRange;
    UInt16  startSpsCrnti;//UeSim
    UInt16  endSpsCrnti;//UeSim
    UInt16 crntiRange;
    UInt16 lowerCRNTIBound;
    UInt16 upperCRNTIBound;
    UInt8   numOfTxAnteenas;
    /* Total active UE's in the system */                
    UInt16  totalActiveUE;    
    UInt8 prachConfigIndex;                 // Lookup "prach-ConfigurationIndex" in specs
#ifdef UESIM_ENHANCEMENTS
    UInt16 n1PUCCH_AN;
#endif    

#ifdef TDD_CONFIG
#if 0
        /* PUCCH Config params */
        UInt8 deltaPUCCHShift;
        UInt8 nRB_CQI;
        UInt8 nCS_AN;
        UInt16 n1PUCCH_AN;
#if 0
        UInt8 tddAckNackFeedbackMode;   /* grep BUNDLING or MULTIPLEXING for values */
#endif

        UInt8 duplexingMode;    /* Whether TDD (1) or FDD (0) */
        TddSubFrameAssignment subFrameAssignment;
        TddSpecialSubFramePattern specialSubFramePattern;
#endif
    UInt8 subFrameAssignment;      
    UInt8 specialSubFramePattern;
#endif  
#ifdef UESIM_ENHANCEMENTS
    UInt8 rat0RbgSize;
    UInt32 bitsMaskForSpan;
    UInt32 rat1MaxRBsAllocated;  
    UInt32 bitsMaskForSubsetCount;
#endif    
    /*CLPC_CHG*/
    /*For storing start and end values of TPC RNTI*/
    TPCRntiRange tpcRntiRange;
    /*CLPC_CHG*/
	/* + CA_AB */
    ueSpsSysWideParam ueSpsSysWideParam;
	/* - CA_AB */
}CellConfig;
    /*LTE_REG_FIX*/
typedef struct UeSimCellTypeT
{
    UInt8 cellType;
}UeSimCellType;
/*LTE_REG_FIX*/
UeSimCellType cellType_g[MAX_NUM_CELLS];
#define NB_CELL 1
#define IS_NB_IOT_CELL(cellIndex) cellType_g[cellIndex].cellType == NB_CELL? LTE_TRUE: LTE_FALSE
/*LTE_REG_FIX*/
typedef enum DeltaPucchShiftT
{
  DS1 = 1,
  DS2,
  DS3
}DeltaPucchShift;
typedef enum PhichResourceT
{
    PHICH_RESOURCE_0 = 0,
    PHICH_RESOURCE_1,
    PHICH_RESOURCE_2,
    PHICH_RESOURCE_3
}PhichResource;
typedef enum UlCyclicPrefixT 
{ 
    LEN1 = 1, 
    LEN2 =2 
}UlCyclicPrefix;

typedef struct ConfigCellReqT
 {
     UInt8 dlResBlocks;
     //UInt8 dlStartRange;
     //UInt8 dlEndRange;
     UInt8 ulResBlocks;
     //UInt8 ulStartRange;
     //UInt8 ulEndRange;
     UInt8 maxHarqRetrans;
     UInt8 numOfTxAntennas;
     UInt8 startRARntiRange;
     UInt8 endRARntiRange;
     UInt8 phichDuration;
     UInt8 phichResource;
     UInt8 ulCyclicLenthPrefix;
 } ConfigCellReq;


/****************************************************************************
 * Exported Variables
 ****************************************************************************/

extern DLUEContextInfo dlUECtxInfoArr_g[MAX_UE_SUPPORTED];
extern UInt8 frequencySelective_g;
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

extern void readRRCMsgFromSocket(void);

MacRetType deleteMACDLUEEntity ( UInt16 ueIndex, UInt16 transactionId, UInt8 cnfFlag);
MacRetType deleteMACDLLogicalChannel ( UInt16 ueIndex, SInt8 ueLCId);
MacRetType addMACDLLogicalChannel (DLUEContext* newUEContext_p, UInt8 logicalChCount,
                                 LogicalChannelConfigInfo* logicalChConfig_p);
MacRetType createMACDLUEEntity ( ueContextForDownlink* ueDlConfigInfo_p, 
        UInt8 cellIndex);
MacRetType reconfigureMACDLUEEntity (ueContextForDownlink *dwlkUEInfo_p, UInt8 cellIndex);
MacRetType reconfigureMACDLLogicalChannel (DLUEContext* newUEcontext_p,
        LogicalChannelConfigInfo* logicalChConfig_p);
MacRetType deleteMACDLLogicalChannel ( UInt16 ueIndex, SInt8 ueLCId);
MacRetType deleteDLUELogicalChannel( DLUEContext *ueContext_p,
                                                   UInt8 ueLCId );

void * initRRCIntThread(void *);

     /* SPR 15909 fix start */
extern tickType_t getCurrentglobaltick();
     /* SPR 15909 fix end */

/* SPR 21660 changes start_Sahil */
/* SPR 21660 changes end_Sahil */

#endif /* Included LTELAYER2__RRC_INTERFACE_H */
