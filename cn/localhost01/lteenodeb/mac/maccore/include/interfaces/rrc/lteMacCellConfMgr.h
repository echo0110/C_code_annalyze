/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacCellConfMgr.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacCellConfMgr.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.12  2009/08/24 12:54:10  gur11912
 * cellConfigured_g declared as volatile
 *
 * Revision 1.11  2009/07/17 06:24:27  gur20439
 * changes related to rrc interface modification incorporated.
 *
 * Revision 1.10  2009/07/13 04:07:08  gur19413
 * added new macro for log
 *
 * Revision 1.9  2009/06/22 06:23:03  gur19413
 * added new parameter in cell config structure
 *
 * Revision 1.8  2009/06/05 15:03:11  gur18569
 * change in process cell config req
 *
 * Revision 1.7  2009/06/05 14:16:35  gur19413
 * rename function name
 *
 * Revision 1.6  2009/06/01 12:45:47  gur18569
 * changed dlAvailiableRBs to dlAvailableRBs
 *
 * Revision 1.5  2009/05/29 10:21:01  gur18569
 * removed compilation error
 *
 * Revision 1.4  2009/05/27 09:13:11  gur19413
 * changes in structures parameters
 *
 * Revision 1.3  2009/05/21 09:51:50  gur20052
 * Updated uplink parameters
 *
 * Revision 1.2  2009/05/10 08:08:21  gur19413
 * adding cell config structure
 *
 * Revision 1.1  2009/04/30 12:12:54  gur19413
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_CELL_CONF_MGR_H
#define LTE_MAC_CELL_CONF_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacParseUtil.h"
#include "lteMacTBSize.h"
#include "lteMacOAMIntf.h"
#ifdef LTE_EMBMS_SUPPORTED
#include "lteMacEMbmsSchedulerStrategy.h"
#endif
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define CELL_CONF "Cell Conf"

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* SS_S2 */

#define MAX_BANDWIDTH_INFO_ENTRIES  5
/* + CQI_4.1 */
#define MAX_UE_SELECTED_SUBBAND 7
#define MAX_UE_SELECTED_N 26
/* - CQI_4.1 */
/** SPR 14853 Changes Start **/
/** SPR 14505 Changes **/
#define DATA_BUFFER_SIZE            20000
/** SPR 14505 Changes **/
/** SPR 14853 Changes End **/
typedef struct BandwidthParamsInfoT
{
    UInt8   bandWidthStart;
    UInt8   bandWidthEnd;
    UInt8   kForM;
    UInt8   kForJ;
    UInt8   M;
    UInt8   J;
}BandwidthParamsInfo;

extern UInt8 dataForSpecialSubFrame;
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UInt8 puschBlerNackWeightage_g;
extern UInt8 puschBlerAckWeightage_g;
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/*fill the Cell Config structure */
MacRetType processCellConfig( ConfigCellReq *configCellReq_p
,InternalCellIndex internalCellIndex); /* SS_S2 */
/* + CQI_4.1 */
UInt32  getBinomialCoefficient(UInt8 N , UInt8 M,
        InternalCellIndex internalCellIndex);
/* - CQI_4.1 */

/* Begin TDD changes */
#ifdef TDD_CONFIG
 MacRetType checkRIValidityAtTick(
        UInt16 Iri,
        UInt16 sysFrameNumber,
        UInt8 subFrameNum);

 MacRetType checkCQIValidityAtTick(
        UInt16 Icqi,
        UInt16 sysFrameNumber,
        UInt8 subFrameNum);

 MacRetType checkWbCQIValidityAtTick(
        UInt16 Icqi,
        UInt16 sysFrameNumber,
        UInt8 subFrameNum,
        UInt8 k,
        UInt8 J);

 MacRetType checkSRValidityAtTick(
        UInt16 Isr,
        UInt16 sysFrameNumber,
        UInt8 subFrameNum);

 void fillSrDataInReportMapArray(
        UInt16 configIndex,
        UInt32 bitString);

 void fillRiDataInReportMapArray(
        UInt16 configIndex,
        UInt32 bitString);

 void fillCqiDataInReportMapArray(
        UInt16 configIndex,
        UInt32 bitString);

 void fillWbCqiDataInReportMapArray(
        UInt16 configIndex,
        UInt8  k,
        UInt32 bitString);


#define MAX_RI_PERIODICITY 966
#define MAX_CQI_PERIODICITY_TDD 316
#define MID1_CQI_PERIODICITY 317
#define MID2_CQI_PERIODICITY 318
#define MAX_SR_PERIODICITY 155
#define MAX_SLOT_NUMBER 20
#define PERIODICITY_0 0
#define PERIODICITY_1 1
#define PERIODICITY_2 2
#define PERIODICITY_4 4
#define PERIODICITY_5 5
#define PERIODICITY_8 8
#define PERIODICITY_10 10
#define PERIODICITY_16 16
#define PERIODICITY_20 20
#define PERIODICITY_32 32
#define PERIODICITY_40 40
#define PERIODICITY_64 64
#define PERIODICITY_80 80
#define PERIODICITY_128 128
#define PERIODICITY_160 160
#define OFFSET_0 0
#define OFFSET_1 1
#define OFFSET_2 2
#define OFFSET_3 3
#define OFFSET_4 4
#define OFFSET_5 5
#define OFFSET_6 6
#define OFFSET_7 7
#define OFFSET_8 8
#define OFFSET_9 9
#define OFFSET_10 10
#define OFFSET_11 11
#define OFFSET_12 12
#define OFFSET_13 13
#define OFFSET_14 14
#define OFFSET_15 15
#define OFFSET_16 16
#define OFFSET_17 17
#define OFFSET_18 18
#define OFFSET_19 19
#define OFFSET_20 20
#define OFFSET_21 21
#define OFFSET_22 22
#define OFFSET_23 23
#define OFFSET_24 24
#define OFFSET_25 25
#define OFFSET_26 26
#define OFFSET_27 27
#define OFFSET_28 28
#define OFFSET_29 29
#define OFFSET_30 30
#define OFFSET_31 31
#define OFFSET_32 32
#define OFFSET_33 33
#define OFFSET_34 34
#define OFFSET_35 35
#define OFFSET_36 36
#define OFFSET_37 37
#define OFFSET_38 38
#define OFFSET_39 39
#define OFFSET_40 40
#define OFFSET_41 41
#define OFFSET_42 42
#define OFFSET_43 43
#define OFFSET_44 44
#define OFFSET_45 45
#define OFFSET_46 46
#define OFFSET_47 47
#define OFFSET_48 48
#define OFFSET_49 49
#define OFFSET_50 50
#define OFFSET_51 51
#define OFFSET_52 52
#define OFFSET_53 53
#define OFFSET_54 54
#define OFFSET_55 55
#define OFFSET_56 56
#define OFFSET_57 57
#define OFFSET_58 58
#define OFFSET_59 59
#define OFFSET_60 60
#define OFFSET_61 61
#define OFFSET_62 62
#define OFFSET_63 63
#define OFFSET_64 64
#define OFFSET_65 65
#define OFFSET_66 66
#define OFFSET_67 67
#define OFFSET_68 68
#define OFFSET_69 69
#define OFFSET_70 70
#define OFFSET_71 71
#define OFFSET_72 72
#define OFFSET_73 73
#define OFFSET_74 74
#define OFFSET_75 75
#define OFFSET_76 76
#define OFFSET_77 77
#define OFFSET_78 78
#define OFFSET_79 79
#define OFFSET_80 80
#define OFFSET_81 81
#define OFFSET_82 82
#define OFFSET_83 83
#define OFFSET_84 84
#define OFFSET_85 85
#define OFFSET_86 86
#define OFFSET_87 87
#define OFFSET_88 88
#define OFFSET_89 89
#define OFFSET_90 90
#define OFFSET_91 91
#define OFFSET_92 92
#define OFFSET_93 93
#define OFFSET_94 94
#define OFFSET_95 95
#define OFFSET_96 96
#define OFFSET_97 97
#define OFFSET_98 98
#define OFFSET_99 99
#define OFFSET_100 100
#define OFFSET_101 101
#define OFFSET_102 102
#define OFFSET_103 103
#define OFFSET_104 104
#define OFFSET_105 105
#define OFFSET_106 106
#define OFFSET_107 107
#define OFFSET_108 108
#define OFFSET_109 109
#define OFFSET_110 110
#define OFFSET_111 111
#define OFFSET_112 112
#define OFFSET_113 113
#define OFFSET_114 114
#define OFFSET_115 115
#define OFFSET_116 116
#define OFFSET_117 117
#define OFFSET_118 118
#define OFFSET_119 119
#define OFFSET_120 120
#define OFFSET_121 121
#define OFFSET_122 122
#define OFFSET_123 123
#define OFFSET_124 124
#define OFFSET_125 125
#define OFFSET_126 126
#define OFFSET_127 127
#define OFFSET_128 128
#define OFFSET_129 129
#define OFFSET_130 130
#define OFFSET_131 131
#define OFFSET_132 132
#define OFFSET_133 133
#define OFFSET_134 134
#define OFFSET_135 135
#define OFFSET_136 136
#define OFFSET_137 137
#define OFFSET_138 138
#define OFFSET_139 139
#define OFFSET_140 140
#define OFFSET_141 141
#define OFFSET_142 142
#define OFFSET_143 143
#define OFFSET_144 144
#define OFFSET_145 145
#define OFFSET_146 146
#define OFFSET_147 147
#define OFFSET_148 148
#define OFFSET_149 149
#define OFFSET_150 150
#define OFFSET_151 151
#define OFFSET_152 152
#define OFFSET_153 153
#define OFFSET_154 154
#define OFFSET_155 155
#define OFFSET_156 156
#define OFFSET_157 157
#define OFFSET_158 158
#define OFFSET_159 159
#define OFFSET_160 160


typedef struct CqiPmiPeriodicityMapT
{
    UInt16 cqiPmiPeriodicity;
    UInt16 subFrameOffset;
}CqiPmiPeriodicityMap;

typedef struct RiPeriodicityMapT
{
    UInt16 riPeriodicity;
    UInt16 subFrameOffset;
}RiPeriodicityMap;

typedef struct SrPeriodicityMapT
{
        UInt16 srPeriodicity;
        UInt16 subFrameOffset;
}SrPeriodicityMap;


/* This is used by cqi_pmi, wideband cqi_pmi and ri maps. 
 * hence its defined in one place only*/
extern CqiPmiPeriodicityMap cqiPmiPeriodicity_g[MAX_CQI_PERIODICITY_TDD];
extern RiPeriodicityMap riPeriodicity_g[MAX_RI_PERIODICITY];

#endif /* TDD CONFIG */ 


typedef struct CellConfigParamsT
{
    /* Cell Start Stop Chg */
    /*  Hold MAC Cell State */
    UInt8 macCellState;
    /* Cell Start Stop Chg */

    UInt8   duplexingMode;
    UInt8   dlAvailableRBs;
    UInt8   ulAvailableRBs;

    /* Number of HARQ Process per UE*/
    UInt8   numHARQProcess;
    /* Maximum number of HARQ re-transmission */
    UInt8   maxHarqRetransmission;
  
    UInt8   phichDuration;
    UInt8   phichResource;
    UInt8   cyclicPrefix;

    UInt8   numOfTxAnteenas;
    /* Refer Section 7.2.2 of 36.213 for below values */
    UInt8   bandwidthParts; /* Value J that span across S(set of subbands) */
    /* BYTE_ALLIGNMENT_FIX START */
    /* Total active UE's in the system */                
    UInt16  totalActiveUE;    
    /* BYTE_ALLIGNMENT_FIX END */

    UInt8   preferredSubbands; /* Value of M */
    UInt8   N; /* Number of Subbands */ 
    UInt8   LBitLabel; /* Value of L as per iSpec 36.213 section 7.2 */ 
    /* + CQI_4.1 */
    UInt8 ueSelectedN; /* value of N as per Spec 36.213 section 7.2.1
                        * (Aperiodic UE selected Mode), table for K ,M and
                        * sysytem bandwidth */
    UInt8 LBitLabelForAperiodic;
    /* ULA_CHG */
    /* FREQ_SELECTIVE CHG */
    UInt8 freqSelectiveSchUL;
    UInt8 freqSelectiveSchDL;
    /* FREQ_SELECTIVE CHG */
    /* ULA_CHG */

    /* SRS_CHG */
    /* SPR 1383 Start */
    UInt8  srsConfigType;
    /* SPR 1383 End */
    /*SPR 3600 Start*/
    /*Flag to indicate whether 64QAM is enabled:updated from MAC_CONFIG_CELL_REQ API API*/
    UInt8 macEnable64QAM;
    /*SPR 3600 End*/

    /* Used for Paging Repetitions */
    UInt8  sfnGap;
    /* Used for deriving Paging Repetitions */
    UInt8  Ns;

    /* SPR 19309 + */
    /* The CFI value to be used at init time */
    UInt8  cfi;
    /* SPR 19309 - */
    /* SPR 19310 + */
    /* This is used to implement the extended UL where the UE is provided
     * additional grant even if the BSR is 0 */
    UInt16  numEUL; /* Number of extra UL grants provided to UE */
    UInt16  sizeEUL; /* Size of the UL grant in bytes to be sent to UE */
    /* SPR 19310 - */

    /* CLPC_CHG */
    /* Number of TPC PUCCH RNTI configured as indicated by range configured*/
    UInt8 maxTpcPucchRntiRange;

    /* Number of TPC PUSCH RNTI configured as indicated by range configured*/
    UInt8 maxTpcPuschRntiRange;
    /* CLPC_CHG END */

    /*Dynamic Pdcch Changes Start*/
    UInt8 isDynamicPdcchDisabled;

    UInt16  startRARNTI;
    UInt16  endRARNTI;

    UInt16 contnFreeRachTimer;

    /* SS_S2 */
    /*For storing start and end values of TPC RNTI*/
    TPCRntiRange tpcRntiRange;

    /*Parameters for Power Control*/
    /*Contains power offsets for PCFICH, PHICH, PDCCH, PBCH and PCH*/
    CommonDLPowerControlInfo commonDLPowerControlInfo;
    /*Contains the downlink reference-signal transmit
    ** power used to derive downlink reference-signal*/
    PDSCHConfigCommonInfo pdschConfigCommonInfo;

    /*Indicates whether closed loop power control feature is enabled or not*/
    PowerControlEnableInfo powerControlEnableInfo;

    /*Contains a cell specific nominal component for PUSCH and
    ** a 3-bit cell specific parameter for power control*/
    CommonULPowerControlInfo commonULPowerControlInfo;

    /* SS_S2 */
    UInt32 maxCombinatorialIndex;
    /* - CQI_4.1 */
#ifdef TDD_CONFIG
    /*TDDFRAME Config start*/
    UInt16 subFrameAssign;
    UInt16 splSubFramePatterns;
    /*TDDFRAME Config end*/
#endif   
    /*SRS common setup information received from RRC*/
    SRSCommonSetupInfo  srsCommonSetupInfo; 
    /* SRS_CHG */
    ULPRBUsageStatistic ulPRBUsageInfo;
    /*SPR_3061_FIX*/
    UInt16 pucchCQISINRThreshold;
    UInt16 ulSyncLossTimer;
    /*SPR_3061_FIX*/

    /* Indicates type of format used for scheduling of SI mesage for ETWS
     * feature */ 
    /* SPR 6208 changes start */
    DCIFormat dciFormatForCommonChnlMsg;
    /* dci_format_parameter_name chaged */

    DlBandwidth dlBandwidth;
    /*Dynamic Pdcch Changes End*/

    /* Indicates SPS parameters in cell setup */
    SpsCommonCellInfo spsCommonCellInfo;

   /* PRS configuration */
    PRSConfigInfo  prsConfigInfo;
    /*SPR 7086 Fix Start*/
    /*SRS Frequency domain start RB as calculated in 36.211 Sec 5.5.3.2*/
    UInt16 srsFreqDomainStartRB;
    /*SRS Frequency domain End RB as calculated in 36.211 Sec 5.5.3.2*/
    UInt16 srsFreqDomainEndRB;
    /*SPR 7086 Fix End*/

   /* Start of change for VARIBLE Declaration */
	UInt32 NumOfOverLappingRbsInfo[RES_BIT_MAP_ARRAY_SIZE];
	UInt8 centralSixRbsInfo[MAX_NUM_OVERLAPPING_RB];
	/* End of change for VARIBLE Declaration */
    DlUeCategoryInfo dlUeCategoryInfo[MAX_UE_CATEGORY]; 
	UInt8 RBSelectionTableDCI1A[MAX_TBS_INDEX_DCI_1A];
	UInt8 RBSelectionTableDCI1C[MAX_TBS_INDEX_DCI_1C];
    UInt32  gapValue;
    RACHConfigInfo rachConfigInfo;
    /*Contains new and current MIB message information*/
    MIBMsgInfo   mibInfo;
   /*Contains new and current SIB-1 message*/
    SIBType1Info sib1MsgInfo;
    /*Due to Stack Overuse coverity warning, this varibale is moved from local scope of
      decodeSIMsg to global scope and memory is allocated dynamically */
    SIMsgInfoFromRRC siMsgReceived[MAX_SI_MSG];
    SIInfo       siMsgInfo;
    SIMsg8 siMsg8buff[MAX_NO_SI8_MSG_INDX];
    UInt8 siMsg8latestIndexFilled;
    resAllocInitInfo_t resAllocInitInfo;
    UInt32 maxInActivityTime;
   /*This stores the Number of PHICH Group that can be sent on PDCCH Region*/
    UInt32 numPHICHGroup;
    ULSchedulerStrategyConfig ulSchdConfig;
    UInt16 totalActiveUEsDL;
    UInt16 totalActiveUEsUL;
#ifdef LTE_EMBMS_SUPPORTED
    UInt16 eMbmsTransPower;
#endif
}CellConfigParams;
/* SPR 19288 change start */
/* SPR 19288 change end */
/*individual cell parameters */
typedef struct CellAndInitParamsT
{
    CellState cellState;
    UInt8 explicitStartRequired;
    /* This variable is used to track the cell configuration if done once. This is used for checking
     * for creating memory pools which are done only once */
    /* This flag is use as safeguard for createMemPool.
     * All Pools should be created only on first time of system 
     * startup i.e. upto cell create. After that no pool 
     * will be allowed to create even after cell delete and again 
     * cell create scenario. In this appraoch cell reconfig is not
     * supported i.e. cell recreate with new parameters is not 
     * supported
     */
    UInt8 isCellConfiguredOnce;
    InitParams *initParams_p;
    CellConfigParams *cellParams_p;
}CellAndInitParams;

typedef struct ServingCellSpecificParamsT
{
    UInt8 numOfCells;
    CellAndInitParams *cellConfigAndInitParams_p[MAX_NUM_CELL];
} ServingCellSpecificParams;

extern ServingCellSpecificParams cellSpecificParams_g;

/* SPR 609 changes start */
void initCellSpecificSRSConfig(InternalCellIndex internalCellIndex);
/* SPR 609 changes end */
/* SPR 15909 fix start */
void resetULPRBUsage(ULPRBUsageStatistic *ulPRBUsageInfo_p, tickType_t currentTTI);
/* SPR 15909 fix end */
void calculateULPRBUsage(
    ULPRBUsageStatistic *ulPRBUsageInfo_p, 
    /* SPR 15909 fix start */
    tickType_t currentTTI);
    /* SPR 15909 fix end */
void fillMinRBsForAperiodicModes(UInt8 dlAvailableRBs,
                      InternalCellIndex internalCellIndex); 

/*moved from BCCH.h for klockwork compilation*/
extern UInt32 decodeMIBMsg(UInt8  **msg_p,
                           UInt32 *remLen_p,
                           UInt32 cellCfgFlag,
                           CellConfigParams *cellParams_p);

extern UInt32 decodeSIBType1Msg(UInt8  **msg_p,
                                UInt32 *remLen_p,
                                UInt32 cellCfgFlag,
                                UInt32 sysFrameNum,
                                CellConfigParams *cellParams_p
#ifdef LTE_EMBMS_SUPPORTED
                                ,InternalCellIndex internalCellIndex
#endif
                                );

/*moved from TBSize.h for klockwork compilation*/
void determineMCSFromTransmissionTypeAndMessageLengthForDCIFormate1A(
        UInt32 buffLenInBytes,
        /* SPR 13250 fix start */
        UInt16 *tbSizeFound_p,
        /* SPR 13250 fix end */
        UInt8 *numRB_p,
        UInt8 *mcsIndex_p,
        UInt8 *tpc_p,
        CellConfigParams *cellParams_p );

void  determineMCSFromTransmissionTypeAndMessageLengthForDCIFormate1C(
        UInt32 buffLenInBytes,
        /* SPR 13250 fix start */
        UInt16 *tbSizeFound_p,
        /* SPR 13250 fix end */
        UInt8 *numRB_p,
        UInt8 *mcsIndex_p,
        UInt8 *tpc_p,
        CellConfigParams *cellParams_p );




UInt8 fillRBSelectionTableforDCIFormat1A(UInt8 **msg_p, UInt8 mapLength,
                                         /* +- SPR 17777 */
                                         CellConfigParams *cellParams_p);
                                         /* - Bandwidth Fix */  
UInt8 fillRBSelectionTableforDCIFormat1C(UInt8 **msg_p, UInt8 mapLength,
                                         CellConfigParams *cellParams_p);
void defaultFillRBSelectionTableForDCI1A(CellConfigParams *cellParams_p);
void defaultFillRBSelectionTableForDCI1C(CellConfigParams *cellParams_p);
/* CA changes start */
void resetScellCSIValuesOnCqiReportValidTimerExpiry( DLUEContext *dlUEContext_p,
                                                     ServingCellIndex scellServCellIndex);
/* CA changes end */
#endif  /*LTE_MAC_CELL_CONF_MGR_H */
