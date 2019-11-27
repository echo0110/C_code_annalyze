/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2UEContext.h,v 1.2.16.1 2010/09/21 15:53:17 gur20491 Exp $
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
 * $Log: lteLayer2UEContext.h,v $
 * Revision 1.2.16.1  2010/09/21 15:53:17  gur20491
 * FAPI changes
 *
 * Revision 1.2  2010/02/13 10:43:55  gur15697
 * changes for UL flow Stability
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2  2009/07/14 14:44:54  gur20439
 * rrc interface code.
 *
 * Revision 1.1  2009/03/30 10:20:16  gur19836
 * Initial Version
 *
 * Revision 1.22  2009/03/23 08:32:30  gur19140
 * Integration changes first waves
 *
 * Revision 1.21  2009/02/20 14:32:44  gur18569
 * performance changes
 *
 * Revision 1.20  2009/01/20 07:53:44  gur19836
 * Changes done for interface with RRC Layer
 *
 * Revision 1.19  2009/01/19 15:10:48  gur18569
 * removed UT_TESTING
 *
 * Revision 1.18  2009/01/16 14:27:05  gur18569
 * added UT_TESTING flag and removed mod scheme from harq process
 *
 * Revision 1.17  2009/01/08 18:57:38  gur18569
 * Review comments incorporated
 *
 * Revision 1.16  2009/01/05 13:02:30  gur18569
 * added prototype for updateLCQueueLoad
 *
 * Revision 1.15  2009/01/04 14:39:54  gur18569
 *  added rbsReservedInCurrentTick and rbsReservedForTA in ueContext
 *
 * Revision 1.14  2009/01/04 07:54:43  gur11912
 * Phase 2 UT Defects Fixed
 *
 * Revision 1.13  2008/12/30 14:24:33  gur11912
 * Phase 2 UT Defect Fixed
 *
 * Revision 1.12  2008/12/23 11:57:44  gur11912
 * code review comments incorporated
 *
 * Revision 1.11  2008/12/23 05:14:13  gur11912
 * code review comments incorporated for Phase2
 *
 * Revision 1.10  2008/12/19 06:59:45  gur18569
 * review comments incorporated for phase2
 *
 * Revision 1.9  2008/12/11 14:10:16  gur11912
 * Downlink Phase 2 changes
 *
 * Revision 1.8  2008/12/05 13:50:27  gur18569
 * multicore changes
 *
 * Revision 1.7  2008/12/02 12:10:41  gur11912
 * Incorporated multiple worker threads changes
 *
 * Revision 1.6  2008/10/22 12:26:06  gur11912
 * Fixed for performance changes
 *
 * Revision 1.5  2008/10/14 05:33:49  gur11912
 * Structure defined for ARQ
 *
 * Revision 1.4  2008/09/27 15:18:10  gur11912
 * UT Defect Fixed
 *
 * Revision 1.3  2008/09/26 07:09:51  gur11912
 * UT Defect Fixed
 *
 * Revision 1.2  2008/09/19 12:34:28  gur11912
 * Updated for Integration
 *
 * Revision 1.1  2008/09/19 07:10:49  gur11912
 * INITIAL VERSION
 *
 * Revision 1.3  2008/09/09 07:28:48  gur11912
 * Type of Harq Process Id changed to SInt8
 *
 * Revision 1.2  2008/09/09 05:03:18  gur11974
 * Updated for integration changes.
 *
 * Revision 1.1  2008/09/04 06:12:22  gur11912
 * Initial Version of Layer 2 MAC Harq Header Files.
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_UE_CONTEXT_H
#define INCLUDED_UE_CONTEXT_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "time.h"
#include "lteLayer2Types.h"
#include "lteLayer2MacRlcInterface.h"


/****************************************************************************
 * Exported Includes
 ****************************************************************************/


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define MAX_CQI_VALUES_PER_PROFILE  15                        
/* SPS_CHG */
#define MAX_SPS_INTERVALS 16
#define MAX_N1_PUCCH_AN_PERSISTENT 4
#ifdef UESIM_ENHANCEMENTS
#define MAX_HARQ_PROCESS_NB        8
#endif
typedef enum implicitRelT
{
    IMPLICIT_RELEASE_AFTER_TWO,
    IMPLICIT_RELEASE_AFTER_THREE,
    IMPLICIT_RELEASE_AFTER_FOUR,
    IMPLICIT_RELEASE_AFTER_EIGHT,

    INVALID_IMPLICIT_RELEASE
}implicitRel_en;
typedef enum spsIntervalE
{
    SPS_ITTERVAL_10,
    SPS_ITTERVAL_20,
    SPS_ITTERVAL_32,
    SPS_ITTERVAL_40,
    SPS_ITTERVAL_64,
    SPS_ITTERVAL_80,
    SPS_ITTERVAL_128,
    SPS_ITTERVAL_160,
    SPS_ITTERVAL_320,
    SPS_ITTERVAL_640,
    SPS_INTERVAL_MAX
}spsInterval;

typedef enum requestTypeE
{
    SPS_RELEASE,
    SPS_SETUP
}requestTypeE;

/* Following enum defines the supported interval profiles */
#if 0
typedef enum SpsSupportedIntervalT
{
    SPS_INTERVAL_10  = 10,
    SPS_INTERVAL_20  = 20,
    SPS_INTERVAL_32  = 32,
    SPS_INTERVAL_40  = 40,
    SPS_INTERVAL_64  = 64,
    SPS_INTERVAL_80  = 80,
    SPS_INTERVAL_128 = 128,
    SPS_INTERVAL_160 = 160,
    SPS_INTERVAL_320 = 320,
    SPS_INTERVAL_640 = 640,
    
    MAX_SPS_INTERVAL = SPS_INTERVAL_40
} SpsSupportedInterval;
#endif
/* N1 PUCCH Persistant list */
typedef struct N1PucchAnPersistentListT
{
    /* Number of  N1PucchAnPersistent present in N1PucchAnPersistentList*/
    UInt8 numOfN1PucchAnPersistent;
    /*  N1PucchAnPersistent value for PUCCH */
    UInt16 N1PucchAnPersistent[MAX_N1_PUCCH_AN_PERSISTENT];
} *LP_N1PucchAnPersistentList, N1PucchAnPersistentList;

/* Following structure contains SPS DL Setup information */
typedef struct SpsDlSetupInfoT
{
    /* SPS scheduling interval in Downlink.*/
    UInt8 semiPersistentSchedIntervalDL;
    /* Number of configured SPS processes.*/
    UInt8 numberOfConfSpsProcesses;
    /* Maximum number of SPS  harq retransmission. */
    UInt8 maxSpsHarqRetx;
    /* Number of empty transmission allowed (explicit-release-after) for DL 
     * SPS occasion */
    UInt8 explicitReleaseAfter;
    /*  N1PucchAnPersistentList parameter */
    N1PucchAnPersistentList N1PucchAnPersistentListVal; 
}*LP_SpsDlSetupInfo, SpsDlSetupInfo;

/* Following structure contains SPS DL Setup information for Interface */
typedef struct SpsDlSetupInfoIntfT
{
    /* SPS scheduling interval in Downlink.*/
    UInt8 semiPersistentSchedIntervalDL;
    /* Number of configured SPS processes.*/
    UInt8 numberOfConfSpsProcesses;
    /* Flag to check whether maxSpsHarqRetx is present or not */
    UInt8 isMaxSpsHarqRetxPresent;
    /* Maximum number of SPS  harq retransmission. */
    UInt8 maxSpsHarqRetx;
    /* Flag to check whether explicitReleaseAfter is present or not */
    UInt8 isExplicitReleaseAfterPresent;
    /* Number of empty transmission allowed (explicit-release-after) for DL 
     * SPS occasion */
    UInt8 explicitReleaseAfter;
    /*  N1PucchAnPersistentList parameter */
    N1PucchAnPersistentList N1PucchAnPersistentListVal; 
}SpsDlSetupInfoIntf;
typedef struct spsDlInfoT
{
    /* 0 - RELEASE, 1 - SETUP */
    UInt8 requestType;
    /* Present if request is Setup */
    SpsDlSetupInfoIntf spsDownlinkSetupInfo;
}spsDlInfo;

typedef struct p0PersistentT
{
    /* for calculation of SPS UE Transmit power for PUSCH */
    SInt8 p0NominalPuschPersistent;
    /* for calculation of SPS UE Transmit power for PUSCH */
    SInt8 p0UePuschPersistent;
}p0Persistent;

typedef struct spsUlSetupInfoT
{
    /* SPS scheduling interval in UPlink.*/
    UInt8 semiPersistentSchedIntervalUL;
    /* Number of empty transmission allowed (explicit-release-after) for UL 
     * SPS occasion */
    UInt8 implicitReleaseAfter;
#if 0
    /* Transmit power Claculation */
    p0Persistent p0PersistentInfo;
    /* Flag to check whether p0Persistent is present or not*/
    UInt8 isP0TagPresent;
#endif
    /* + SPS_TDD_Changes */
    /* present in case of TDD only TRUE/FALSE */
    UInt8 twoIntervalsConfig;
    /* Flag to check whether twoIntervalsConfig is present or not*/
    UInt8 isTwoIntervalConfigPresent;
    /* - SPS_TDD_Changes */
}spsUlSetupInfo;

typedef struct spsUlInfoT
{
    /* 0 - RELEASE, 1 - SETUP */
    UInt8 requestType;
    /* Present if request is Setup */
    spsUlSetupInfo spsUplinkSetupInfo;
}spsUlInfo;
/* Following structure contains the SPS information for DL for an UE */
typedef struct SpsDlInfoT
{
    /* 0 - RELEASE, 1 - SETUP */
    UInt8  requestType;
    /* SPS scheduling interval in Downlink.*/
    UInt8 semiPersistentSchedIntervalDL;
    /* Number of configured SPS processes.*/
    UInt8 numberOfConfSpsProcesses;
}SpsDlInfo;
/* SPS_CHG */
/* New Structure for the DRX Context */
/* + DRX_CHG */
typedef struct DRXContextT
{

    UInt8     drxOnDurationTimer;
    UInt16    drxInactivityTimer;
    UInt8     drxRetransmissionTimer;
    UInt16    longDRXCycle;
    UInt16    drxStartOffset;
    UInt8     isDRXShortCycleConfigured;
    UInt16    shortDRXCycle;
    UInt8     drxShortCycleTimer;
    
}DRXContext;

/*-  DRX_CHG */

/* + MEAS_GAP_CHG */
/* New Structure for Measurement Gap Context */
typedef struct MeasurmentGapConfigT
{

    UInt8    gapPatternId;
    UInt8    gapOffset;

}MeasurmentGapConfig;

typedef struct MeasurmentGapStaticParamT
{
     UInt8    measurementGapPeriodicity;
     UInt8    flooredGapOffset;
     UInt8    modGapOffset;
}MeasurmentGapStaticParam;

typedef struct MeasurmentGapContextT
{
     MeasurmentGapConfig measurementGapConfig;
     MeasurmentGapStaticParam measGapStaticParam;
}MeasurmentGapContext;

/* + CQI_4.1 */
typedef struct cqiPeriodicReportMode10T
{
    UInt8 wideBandCQI;
}cqiPeriodicReportMode10;

typedef struct cqiPeriodicReportMode11T
{
    UInt8 wideBandCQI_RI_1;
    UInt8 wideBandCQI_RI_2;
    UInt8 spatialDiffCQI_RI_2;
    UInt8 PMI_RI_1;
    UInt8 PMI_RI_2;
}cqiPeriodicReportMode11;

typedef struct cqiPeriodicReportMode20T
{
    UInt8 subBandCQI;
    UInt8 subBandLabel;
}cqiPeriodicReportMode20;

typedef struct cqiPeriodicReportMode21T
{
    UInt8 subBandCQI_RI_1;
    UInt8 subBandCQI_RI_2;
    UInt8 spatialDiffCQI_RI_2;
    /* SPR 8067 Fix Start */
    UInt8 PMI_RI_1;
    UInt8 PMI_RI_2;
    /* SPR 8067 Fix End */
    UInt8 subBandLabel_RI_1;
    UInt8 subBandLabel_RI_2;
}cqiPeriodicReportMode21;

typedef struct cqiAperiodicReportMode12T
{
    UInt8 wideBandCQICW0_RI_1;
    UInt8 wideBandCQICW0_RI_2;
    UInt8 wideBandCQICW1_RI_2;
    UInt8 pmi_RI_1[13];
    UInt8 pmi_RI_2[13];
}cqiAperiodicReportMode12;

typedef struct cqiAperiodicReportMode20T
{
    UInt8 wideBandCQICW;
    UInt8 subBandDiffCQI;
    UInt32 posOfMSelSubband;
}cqiAperiodicReportMode20;

typedef struct cqiAperiodicReportMode22T
{
    UInt8 wideBandCQICW0_RI_1;
    UInt8 wideBandCQICW0_RI_2;
    UInt8 wideBandCQICW1_RI_2;
    UInt8 subBandDiffCQICW0_RI_1;
    UInt8 subBandDiffCQICW0_RI_2;
    UInt8 subBandDiffCQICW1_RI_2;
    UInt32 posOfMSelSubband_RI_1;
    UInt32 posOfMSelSubband_RI_2;
    UInt8 pmi_RI_1;
    UInt8 pmi_RI_2;
}cqiAperiodicReportMode22;

typedef struct cqiAperiodicReportMode30T
{
    UInt8 wideBandCQICW;
    UInt8 subBandDiffCQI[13];
}cqiAperiodicReportMode30;

typedef struct cqiAperiodicReportMode31T
{
    UInt8 wideBandCQICW0_RI_1;
    UInt8 wideBandCQICW0_RI_2;
    UInt8 wideBandCQICW1_RI_2;
    UInt8 subBandDiffCQICW0_RI_1[13];
    UInt8 subBandDiffCQICW0_RI_2[13];
    UInt8 subBandDiffCQICW1_RI_2[13];
    UInt8 pmi_RI_1;
    UInt8 pmi_RI_2;
}cqiAperiodicReportMode31;

typedef struct DLCQIInfoT
{
    UInt32 indexTrack;/*EICIC Enhancement */
    UInt8 periodicModeType;
    UInt8 aperiodicModeType;
    UInt8 aperiodicSubBandSize;
    UInt8 ueSelectedN;
    UInt8 N;
    UInt8 preferredSubbands;
    UInt8 posOfMSelSBAperiodic;
    UInt8 LBitLabelForPeriodic;
    UInt8 bandwidthParts;
    UInt8 ueCqiProfileNum;
    UInt8 ueCqiIndexNum;
    UInt8 aperiodicCQIRequest[10];
    UInt8 cqiReportType[10];
    /* + CQI_4.1 */
    UInt16 cqiOffset;
    UInt8 lastWideband;
    UInt16 cqiPeriodicity;
    UInt16 tempRecvSfnSf;
    UInt8 bandwidthPartCount;
    UInt32 widebandPeriodicity;
    UInt32 nextSubbandOccurrence;
    UInt32 nextWidebandOccurrence;
    /* EICIC +*/
     UInt16 cqiOffset_2;
    UInt16 cqiPeriodicity_2;
    UInt8 bandwidthPartCount_2;
    UInt32 widebandPeriodicity_2;
    UInt32 nextSubbandOccurrence_2;
    UInt32 nextWidebandOccurrence_2;
    /* EICIC -*/
    UInt8 calculateBandwidthPartFlag;
    /* - CQI_4.1 */
    /* + CQI_5.0 */
    UInt8 *riValue_p;
    UInt8 *riValueAbs_p; /* EICIC +-*/
    UInt8 isULSCHPDUType[10];
    UInt8 ueRIIndexNum;
    UInt8 RI;
    UInt8 cqiInfoFlagInCreate;
    UInt8 dlCqiPmiSizeRank_1[10];
    UInt8 dlCqiPmiSizeRankGT_1[10];
    UInt8 periodicRIAtULSCH ;
    UInt8 cqiformatIndicatorPeriodic;
    /* - CQI_5.0 */
    cqiAperiodicReportMode12 *aperiodicCQIMode12_p;
    cqiAperiodicReportMode20 *aperiodicCQIMode20_p;
    cqiAperiodicReportMode22 *aperiodicCQIMode22_p; 
    cqiAperiodicReportMode30 *aperiodicCQIMode30_p;
    cqiAperiodicReportMode31 *aperiodicCQIMode31_p;
    cqiPeriodicReportMode10 *periodicCQIMode10_p;
    cqiPeriodicReportMode11 *periodicCQIMode11_p; 
    cqiPeriodicReportMode20 *periodicCQIMode20_p;
    cqiPeriodicReportMode21 *periodicCQIMode21_p;
    /* EICIC +*/
   cqiAperiodicReportMode12 *aperiodicCQIMode12Abs_p;
    cqiAperiodicReportMode20 *aperiodicCQIMode20Abs_p;
    cqiAperiodicReportMode22 *aperiodicCQIMode22Abs_p; 
    cqiAperiodicReportMode30 *aperiodicCQIMode30Abs_p;
    cqiAperiodicReportMode31 *aperiodicCQIMode31Abs_p;
    cqiPeriodicReportMode10 *periodicCQIMode10Abs_p;
    cqiPeriodicReportMode11 *periodicCQIMode11Abs_p; 
    cqiPeriodicReportMode20 *periodicCQIMode20Abs_p;
    cqiPeriodicReportMode21 *periodicCQIMode21Abs_p;
    /* EICIC -*/
}DLCQIInfo;
/* Aperiodic CQI Changes Start */
extern cqiPeriodicReportMode10 cqiPeriodicReportMode10_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiPeriodicReportMode11 cqiPeriodicReportMode11_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiPeriodicReportMode20 cqiPeriodicReportMode20_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiPeriodicReportMode21 cqiPeriodicReportMode21_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];

extern cqiAperiodicReportMode12 cqiAperiodicReportMode12_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiAperiodicReportMode20 cqiAperiodicReportMode20_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiAperiodicReportMode22 cqiAperiodicReportMode22_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiAperiodicReportMode30 cqiAperiodicReportMode30_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiAperiodicReportMode31 cqiAperiodicReportMode31_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
/* Aperiodic CQI Changes Start */

/* EICIC +*/
extern cqiPeriodicReportMode10 cqiPeriodicReportMode10_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiPeriodicReportMode11 cqiPeriodicReportMode11_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiPeriodicReportMode20 cqiPeriodicReportMode20_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiPeriodicReportMode21 cqiPeriodicReportMode21_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiAperiodicReportMode12 cqiAperiodicReportMode12_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiAperiodicReportMode20 cqiAperiodicReportMode20_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiAperiodicReportMode22 cqiAperiodicReportMode22_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiAperiodicReportMode30 cqiAperiodicReportMode30_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
extern cqiAperiodicReportMode31 cqiAperiodicReportMode31_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
/* EICIC -*/
/* - CQI_4.1 */
/* + CQI_5.0 */
/* + SPR 13126 Changes */
extern UInt8 RIValues_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE]; 
extern UInt8 RIValues_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE]; /*EICIC +-*/
/* - SPR 13126 Changes */
/* - CQI_5.0 */


typedef enum MeasGapPatternIdTypeT
{
    GP0 = 0,
    GP1 = 1
    /*Meas_Gap_Changes_Start*/
    ,GP4_R15
    ,GP5_R15
    ,GP6_R15
    ,GP7_R15
    ,GP8_R15
    ,GP9_R15
    ,GP10_R15
    ,GP11_R15
    /*Meas_Gap_Changes_End*/

}MeasGapPatternIdType;

typedef enum RequestTypeT
{
    RELEASE=0, 
    SETUP=1,
    INVALID_REQUEST_TYPE
}RequestType;

/* - MEAS_GAP_CHG */

typedef struct TransmissionAndRLCDataT
{
    RLCDataBuffer rlcDataBufferArr[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    UInt32 paddingBits;
    /* SPR 15909 fix start */
    tickType_t scheduledTick;
    /* SPR 15909 fix end */
    UInt8 harqProcessId;
    UInt8  **dataPtr_p;
    UInt16 *dataLen_p;
    UInt32 tbSize;      
    void *uldataNode_p;
#ifdef FDD_CONFIG
    /** TTIB_Code Start */
    UInt8 lccount;
#endif
}TransmissionAndRLCData;
typedef struct LogicalChannelInfoT
{
    /*contain the information regarding when this logical channel was scheduled*/
     /* SPR 15909 fix start */
     tickType_t lastScheduledTick;          
     /* SPR 15909 fix end */

    /*indicates the data available for this logical channel*/
    UInt32 queueLoad;                  

    /* 0-TM,1 -UM, 2-AM */
    UInt8 rlcMode;

    /*1,2,3 Conversational -1, Interactive . 2, Backgorund - 3*/
    UInt8 rlcSNFieldLength;
    
    /*indicates the id of this logical channel*/
    UInt8  logicalChannelId;           

    /*indicates the priority of this logical channel*/
    UInt8  logicalChannelPriority;     
}DLLogicalChannelInfo;

/*CLPC_CHG*/
typedef struct TpcAckNackContextT
{
#ifdef TDD_CONFIG
    UInt8 tb1_ack_nack_perUe_array[4][4];
    UInt8 tb2_ack_nack_perUe_array[4][4];
#elif FDD_CONFIG
    UInt8 tb1_ack_nack_perUe_array[10][10];
    UInt8 tb2_ack_nack_perUe_array[10][10];
#endif
    SInt16 perAckTb1UE;
    SInt16 perNackTb1UE;
    SInt16 perUnsureTb1UE;
    SInt16 perAckTb2UE;
    SInt16 perNackTb2UE;
    SInt16 perUnsureTb2UE;
    SInt16 perSureTb1UE;
    SInt16 perSureTb2UE;
}TpcAckNackContext;
/*CLPC_CHG*/

#ifdef UESIM_ENHANCEMENTS
typedef struct DLHARQProcessT
{
    UInt8    harqProcessId;
    UInt8    numOfRBsAllocatedForTBOne;
    UInt8    numOfRBsAllocatedForTBTwo;
} DLHARQProcess;
#endif

/* EICIC +*/
#ifdef FDD_CONFIG
#define NUM_BITS_ABS_PATTERN 40
#else
#define NUM_BITS_ABS_PATTERN 70
#define NUM_BITS_ABS_PATTERN_CONFIG6 60
#define NUM_BITS_ABS_PATTERN_CONFIG1_5 20
#endif
/* EICIC -*/

typedef struct DLUEContextT
{
    /*indicates the C-RNTI assigned to this UE*/
    UInt16              crnti;                   
    /* index into ueDLContextArr_g */
    UInt16              ueIndex;                   
    /*indicates the composite queue load of all logical channels of the UE*/
    UInt32              ueQueueLoad;
    /*A pointer to nodeAnchor of the UE in ueNonZeroQueueLoadList_g*/
   /* An array of LogicalChannelInfo where each enty corresponds to details 
    * regarding the UEs logical channel*/
    DLLogicalChannelInfo logicalChannel[MAX_NUMBER_OF_LOGICAL_CHANNEL];

    /*indicates the current state of UE*/
    /*  */
    UInt8               assignedRBQueueLoadSchIndex;
    UInt8               assignedRBQueueLoadRLCIndex;
    UInt8               ueState;
    /*Its indicate the reserved Rbs for Ue. */
    UInt8               isAlreadyConsideredFlag ; // rbsReservedForUE;
    UInt8               uePriority; 
    UInt8              numLCWithNonZeroQueueLoad;
    /*indicates the maximum LCID available for this UE*/
    UInt8               maxLcId;
    /*signifies that this max number of resources can be used in one TTI for this UE*/
    UInt8               maxNbOfRBs;
    
    /*global tick count indicating when this UE was scheduled*/
     /* SPR 15909 fix start */
    tickType_t          lastScheduledTick;
     /* SPR 15909 fix end */
    /*Its represent the Timing Advance Value. */
    UInt8               timingAdvanceValue;
    /*It represent the DRX Flag (1-  ON or 0-OFF). */
    UInt8               drxFlag;
    /* Its indicate pdcch is allocated or not. */
    UInt8               isPdcchAllocatedFlag;
    /*indicates the current bit rate of the UE*/
    UInt32              effectiveBitRate;
    /* This Indicate is the ACK NACK is pending for TB in which DRX has been
       sent .If true this means we won't sent DRX Again */
    UInt8               pendingDRXAckNack;
    /* Pointes to the DL HARQ context*/
    UInt8                  simultaneousAckNackAndCQI;
    UInt32 		   dlCodingRate; 
    /* + MEAS_GAP_CHG */
    UInt8 measGapConfigType;
    MeasurmentGapContext measGapContext;
   /* - MEAS_GAP_CHG */
   /*CLPC_CHG*/
   TpcPdcchConfig tpcPdcchConfigPucch;
   SInt8 tpcPUCCH;
   /*CLPC_CHG*/
   TpcAckNackContext tpcAckNackContext;
   /*Bug 11713 Fix Start*/
   TpcAckNackContext tpcAckNackContextScell;
   /*Bug 11713 Fix End*/
   UInt8 pA;
   /* + CQI_5.0 */
   UInt8 transmissionMode;
   /* - CQI_5.0 */
    /* + CQI_4.1 */
    DLCQIInfo      dlCQIInfo;
    /* - CQI_4.1 */
    /* SPS_CHG */
    SpsDlInfo spsDlInfo;
    /* SPS_CHG */
    /* HD FDD Changes Start */
#ifdef HD_FDD_CONFIG
    UInt8 isHDFddFlag;
#endif
    /* HD FDD Changes End */
    UInt8               ueCategory; 
    /* + TM7_8 Changes Start */
    UInt8 isPmiRiConfigured_V920;
    /* - TM7_8 Changes End */
#ifdef UESIM_ENHANCEMENTS
    UInt32 tbSizeReceived;
    UInt8    pucchResource[10];
    DLHARQProcess  harqProcess[MAX_HARQ_PROCESS_NB];
#endif    
	/* EICIC +*/
     UInt8 cqiConfigRequestType;
     UInt16 cqiPMIConfigIndex_2;
     UInt16 riConfigIndex_2;
     UInt8 cqiSubsetConfigRequestType;
     UInt8 csiMeasurementSubset1[NUM_BITS_ABS_PATTERN];
     UInt8 csiMeasurementSubset2[NUM_BITS_ABS_PATTERN];
	/* EICIC -*/
}DLUEContext;

typedef struct DLUEContextInfoT
{
    /* This flag is set by RRC interface when UE deletion request comes
       and if PendingDelete is True or 1 then UE Context either
       deleted or is to be deleted.*/
    UInt8              pendingDeleteFlag;
    DLUEContext       *dlUEContext_p;
}DLUEContextInfo;
/* + CA_PHASE2*/
#define MAX_NUM_SCELL 1
#define MAX_CELLS_PER_UE 8
#define MAX_CBSR_VALUE_COUNT 14
#define MAX_PUCCH_AN_CS_LIST 2
#define MAX_PUCCH_AN_CS 4

/* typedef PucchConfig ScellPucchConfig; */
/*This structure contains Scell specific information
 * *required for uplink processing*/
typedef struct ULUESCellContextT
{
	UInt8 SCellIndex;
	UInt8 aperiodicCQIToBeRequested;
	UInt8 isHDFddFlag;
/*	ScellPucchConfig scellpucchConfigInfo; */
	UInt8  userLocationType;
	UInt8      isScellActivateStatus;
}ULUESCellContext;

/* + CL-MIMO RSM*/
typedef enum codebookSubsetRestrictionTypeT
{
    N2_TX_ANTENNA_TM3=0,
    N4_TX_ANTENNA_TM3=1,
    N2_TX_ANTENNA_TM4=2,
    N4_TX_ANTENNA_TM4=3,
    N2_TX_ANTENNA_TM5=4,
    N4_TX_ANTENNA_TM5=5,
    N2_TX_ANTENNA_TM6=6,
    N4_TX_ANTENNA_TM6=7,
    CBSR_INVALID = 255
} codebookSubsetRestrictionType;

#define MAX_CBSR_SIZE 8

typedef struct codebookSubsetRestrictionInfoT
{
    codebookSubsetRestrictionType cbsrType;
    UInt8 cbsrValue[MAX_CBSR_SIZE];
} codebookSubsetRestrictionInfo;
/* - CL-MIMO RSM*/

typedef struct CodebookSubsetRestrictionv1020T
{
    codebookSubsetRestrictionType cbsrType;
	    UInt8 cbsrValue[MAX_CBSR_VALUE_COUNT];

}CodebookSubsetRestrictionv1020;

typedef enum CQIModeT
{
    CQI_MODE_1_0 =10,
    CQI_MODE_1_1=11, 
    CQI_MODE_1_2=12,
    CQI_MODE_2_0=20,
    CQI_MODE_2_1=21,
    CQI_MODE_2_2=22,
    CQI_MODE_3_0=30, 
    CQI_MODE_3_1=31
} CQIMode;

typedef enum PeriodicFeedbackTypeT
{
    PERIODIC_FEEDBACK_WIDEBAND = 0,
    PERIODIC_FEEDBACK_SUBBAND,
}PeriodicFeedbackType;

typedef enum cqiModeReportedT
{
	INVALID_CQI_REPORTING_MODE = 0,
	PERIODIC_CQI_REPORTING_MODE,
	APERIODIC_CQI_REPORTING_MODE,
	PERIODIC_APERIODIC_MODE
		/* - CQI_4.1 */
}cqiModeReported;

typedef struct CqiInfoT
{
	CQIMode   cqiMode;
	UInt8     cqiIndication;
	UInt8     cqiRequestType;
	UInt16    cqiPMIConfigIndex;
	UInt16    cqiPMIConfigIndex_2; /* EICIC +-*/
	UInt16    cqiPucchResourceIndex;
	UInt8     cqiformatIndicatorPeriodic;
	UInt8     cqiformatIndicatorPeriodic_subband_k;
	UInt16    riConfigIndex;
	UInt16    riConfigIndex_2;/* EICIC +-*/
	UInt16    cqiPMIConfigIndexV1020;
	UInt16    cqiPucchResourceIndexV1020;
	UInt8     simultaneousACKNACKAndCQIV1020;
	UInt8     simultaneousACKNACKAndCQI;
	UInt8     cqiformatIndicatorPeriodicV1020;
	UInt8     cqiformatIndicatorPeriodic_subband_kV1020;
	UInt16    riConfigIndexV1020;
	UInt8     cqiMaskV920;
	UInt8     pmiRiReportV920;
	UInt8 cqiConfigRequestType;  /* EICIC +-*/

}CqiInfo;

typedef struct ScellInfoT
{
	UInt8      cellIndex;
	UInt8      sCellIndex;
	UInt8      operationType;
	UInt8      numOfLayer;
	UInt8      transmissionMode;
	UInt8      dlNumHarqProcess;
	UInt8      dlInitMCS;
	UInt8      rankIndicator;
	UInt8      precodingIndex;
	UInt8      isScellActivateStatus;
	UInt16 result;
	UInt8 isHDFddFlag;
	UInt8 userLocationType;
	UInt8 pA;
    UInt8      scellActDeactFlag;
    /*SPR 15909 fix start*/
    tickType_t    scellDeactivationExpirytti; 
    /*SPR 15909 fix end*/
	CodebookSubsetRestrictionv1020 codebookSubsetRestrictionv1020;
	CqiInfo   cqiInfo;
/* Aperiodic CQI Changes Start */
    DLCQIInfo      dlCQIInfo;
/* Aperiodic CQI Changes Start */
}ScellInfo;


typedef struct CarrierAggrConfigT
{
    UInt8 scellDeactivationTimer;
    UInt8 bandWidthDistribution[MAX_CELLS_PER_UE];
	UInt8 scellCount;
    ScellInfo scellInfo[MAX_NUM_SCELL];
}CarrierAggrConfig;

 /* Time after which secondary cell for the UE will be deactivated
  * Possible values are 
  * rf2 = 20ms, rf4=40ms, rf8=80ms, 
  * rf16=160ms, rf32=320ms, rf64=640ms, 
  * rf128=1280ms
  */
typedef struct PucchFormatR10T
{
	UInt8 requestType;
	UInt16 n1Pucch[MAX_PUCCH_AN_CS_LIST][MAX_PUCCH_AN_CS];
}PucchFormatR10;

/*Fix for SPR 11915 start*/
/*this structure is used to copy header info 
for TTIB retransmitted packets*/
typedef struct ttiBundlingHdrInfoT
{
   void *pduData_p;
   UInt16 dataLen;
   UInt8 hdrLen;
}ttiBundlingHdrInfo;
/*Fix for SPR 11915 end*/
/* - CA_PHASE2*/

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*represent the global counter for the TTI tick indications received*/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* SPR 15909 fix start */
volatile tickType_t             globalTTITickCount_g;
/* SPR 15909 fix end */
void deleteUELogicalChannels(DLUEContext *ueContext_p, UInt32 lcBitMask);


#endif  /* UE_CONTEXT_H */
