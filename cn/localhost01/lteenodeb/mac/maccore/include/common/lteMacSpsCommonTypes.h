
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacSpsCommonTypes.h,v  $
 *
 ****************************************************************************
 *
 *  File Description : The file contains the common definitions for SPS.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

#ifndef _LIB_MAC_COMMON_SPS_DEF_
#define _LIB_MAC_COMMON_SPS_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#define SPS_MAX(A,B) \
    ((A) > (B)) ? (A) :(B)

#define SPS_MIN(A,B) \
    ((A) < (B) ) ? (A) : (B)

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

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

/* Enum to be used for distinguish scheduling type */
typedef enum SchTypeT
{
    DRA_SCHEDULING = 0,
    SPS_SCHEDULING = 1
}SchType;

/* Following enum defines the SPS states in MAC sub-layer */
typedef enum SpsStatesT
{
   SPS_IDLE                   = 0,
   SPS_CONFIGURED             = 1,
   SPS_RES_RSV_REQ_INITIATED  = 2,
   SPS_ACTIVATION_INITIATED   = 3,
   SPS_ACTIVATED              = 4,
   SPS_DEACTIVATION_INITIATED = 5,
   SPS_DEACTIVATED            = 6,
   SPS_REACTIVATION_INITIATED = 7,
   UE_NOT_READY               = 8,
   /* SPR 8244 fix start */
   INVALID_STATE              = 9,
   MAX_SPS_STATE
   /* SPR 8244 fix end */
}SpsStates;

/* Following enums define the SPS causes */
typedef enum SpsCauseT
{
    UE_SYNC_LOSS_ON             = 0,
    UE_SYNC_LOSS_OFF            = 1,
    SPS_CONFIG_RECVD            = 2,
    FIRST_NON_ZERO_QUEUE_LOAD   = 3,
    VOICE_PACKET                = 4,
    SPS_PDSCH_PUSCH_SUCCESS     = 5,
    SPS_PDSCH_PUSCH_FAILURE     = 6,
    FIRST_ACK_NACK              = 7,
    PDCCH_CCE_ALLOC_FAILURE     = 8,
    HARQ_TIMER_EXPIRED          = 9,
    SILENCE_DETECTED            = 10,
    MCS_CHANGED                 = 11,
    BLER_THRESHOLD              = 12,
    PDU_SEG_IND                 = 13,
    MAX_DCI_ATTEMPT             = 14,
    MAX_EMPTY_TX                = 15,
    MAX_SPS_HARQ_RETX           = 16,

/* TTIB_Code Start */
    TTI_BUNDLING_CONFIG_CHANGED = 17,
/* TTIB_Code End */
    /* + SPS_TDD_Changes */
    MEAS_GAP_RECONFIG = 18,
    DRX_RECONFIG = 19,
/* SPR 19038 Fix Start */
    MEAS_GAP_COL_WITH_SPS_OCC   = 20,

    MAX_SPS_CAUSE,
    SDL_PROCESSED               = 22,
    OCC_PROCESSED               = 23,
    /* - SPS_TDD_Changes */
/* SPR 19038 Fix End */
    INVALID_CAUSE
}SpsCause;

/* Following enum defines SPS request tye to PDSCH */
typedef enum SpsResReqTypeT
{
    MAC_SPS_RES_REL_REQ       = 0,
    MAC_SPS_RES_RSV_REQ_ACT   = 1,
    MAC_SPS_RES_RSV_REQ_REACT = 2
/* TTIB_Code Start */
#ifdef FDD_CONFIG
    ,MAC_SPS_TTIB_RES_REL_REQ  = 3,
    SPS_PENDING_DELETE        = 4
#endif    
/* TTIB_Code End */
}SpsResReqType;

typedef enum implicitRelT
{
    IMPLICIT_RELEASE_AFTER_TWO,
    IMPLICIT_RELEASE_AFTER_THREE,
    IMPLICIT_RELEASE_AFTER_FOUR,
    IMPLICIT_RELEASE_AFTER_EIGHT,

    INVALID_IMPLICIT_RELEASE
}implicitRel_en;

#define MAX_N1_PUCCH_AN_PERSISTENT 4
#define MAX_SPS_HARQ_RETX_NUM 4
/* + SPS_TDD_Changes */
/* +- SPR 18490 */
#ifdef TDD_CONFIG
#define MAX_UL_SPS_INTERVAL 80
#endif
/* - SPS_TDD_Changes */ 

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

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
    /* Transmit power Claculation */
    p0Persistent p0PersistentInfo;
    /* present in case of TDD only TRUE/FALSE */
    UInt8 twoIntervalsConfig;
    /* Flag to check whether p0Persistent is present or not*/
    UInt8 isP0TagPresent;
    /* Flag to check whether twoIntervalsConfig is present or not*/
    UInt8 isTwoIntervalConfigPresent;
}spsUlSetupInfo;

typedef struct spsUlInfoT
{
    /* 0 - RELEASE, 1 - SETUP */
    UInt8 requestType;
    /* Present if request is Setup */
    spsUlSetupInfo spsUplinkSetupInfo;
}spsUlInfo;

#endif
