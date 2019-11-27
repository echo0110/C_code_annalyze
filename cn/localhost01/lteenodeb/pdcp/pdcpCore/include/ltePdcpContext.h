/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpContext.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : PDCP context definitions
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpContext.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.1  2009/05/20 13:22:17  gur20548
 * Changed header file name
 *
 * Revision 1.6  2009/05/15 18:43:22  gur19140
 * fixes
 *
 * Revision 1.5  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _LIB_PDCP_CONTEXT_DEF_
#define _LIB_PDCP_CONTEXT_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>

#include "ltePdcpLteDefs.h"
#include "ltePdcpProtocolTools.h"
/* ECN changes start*/
#include "ltePdcpRRMInterface.h"
#include "ltePdcpRRMUtils.h"
/* ECN changes end*/
#include "ltePdcpOamSap.h"
#ifdef PDCP_ASYNC_INTERFACE
#include "ltePdcpEncSap.h"
#endif
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*PDCP Entities types list*/
enum PdcpEntityTypeT
{
    PDCP_ENTITY_SRB,
    PDCP_ENTITY_DRB,
};

/*PDCP Entity RLC link modes list*/
enum PdcpEntityRlcModeT
{
    PDCP_ENTITY_RLC_UM = 0,     /*0*/
    PDCP_ENTITY_RLC_AM,         /*1*/
};

/*PDCP Entity RB directions list*/
enum PdcpEntityRbDirectionT
{
    PDCP_ENTITY_RB_TX=0,	/* =0 for array indexing*/
    PDCP_ENTITY_RB_RX=1,	/* =1 for array indexing*/
    PDCP_ENTITY_RB_BOTH,
};

/* SPR_1235_FIX_START*/
/* enum to check if integrity/ciphering can be configured to PDCP Context*/
typedef enum PdcpEntityDirectionSetT
{
    PDCP_DIRECTION_NOT_SET =0,
    PDCP_DIRECTION_SET
}PdcpEntityDirectionSet;
/* SPR_1235_FIX_END*/

/* SPR 3607 FIX START*/
/* enum to check if integrity/ciphering can be configured to PDCP Context*/
typedef enum PdcpEntitySecurityOffsetT
{
    PDCP_SECURITY_OFFSET_DISABLED =0,
    PDCP_SECURITY_OFFSET_ENABLED
}PdcpEntitySecurityOffset;
/* SPR 3607 FIX END*/

/*PDCP states at TX processing */
typedef enum PdcpStatesTxEnumT
{
    PDCP_TX_STATE_IDLE = 0,
    PDCP_TX_STATE_CONNECTED, /*1*/
    PDCP_TX_STATE_SUSPENDED, /*2*/
    PDCP_TX_STATE_RESUME,    /*3*/
    PDCP_TX_STATE_HO,        /*4*/
    /* SRP 3608 changes start */
    PDCP_TX_STATE_AWAITED,   /*5*/ 
    /* SPR 3608 changes end */
    PDCP_TX_MAX_STATE,
    PDCP_TX_INVALID_STATE        
}PdcpStatesTxEnum;

/*PDCP states at RX processing */
typedef enum PdcpStatesRxEnumT
{
    PDCP_RX_STATE_IDLE = 0,
    PDCP_RX_STATE_CONNECTED, /*1*/
    PDCP_RX_STATE_HO,        /*2*/
        
    PDCP_RX_MAX_STATE,
    PDCP_RX_INVALID_STATE        
}PdcpStatesRxEnum;

/*PDCP events for data processing */
typedef enum PdcpEventsTxEnumT
{
    PDCP_SRB_TX = 0,
    PDCP_DRB_TX,        /*1*/
    PDCP_SOURCE_DRB_FWD_TX,    /*2*/
    PDCP_TARGET_DRB_FWD_TX,    /*3*/
        
    PDCP_MAX_TX_EVENT,
    PDCP_INVALID_TX_EVENT        
}PdcpEventsTxEnum;

typedef enum PdcpEventsRxEnumT
{
    PDCP_SRB_RX = 0,    
    PDCP_DRB_RX,      /*1*/
        
    PDCP_MAX_RX_EVENT,
    PDCP_INVALID_RX_EVENT        
}PdcpEventsRxEnum;

/* PDCP events for setting state */
typedef enum PdcpRrcTxEventsEnumT
{
    PDCP_CONNECTED = 0,    
    PDCP_SUSPENDED,   /*1*/
    PDCP_RESUME,      /*2*/
    PDCP_HO,          /*3*/
    /* SRP 3608 changes start */
    PDCP_AWAITED,     /*4*/
    /* SPR 3608 changes end */
    
    PDCP_MAX_RRC_TX_EVENT,
    PDCP_INVALID_RRC_TX_EVENT        
}PdcpRrcTxEventsEnum;

/* ECN start changes*/

typedef struct PdcpRrmEcnConfigParamT
{
    EcnFeatureStatus      ecnFeatureStatus;
    EcnTriggerInfo        ecnTrigger;
    UInt16                lowWaterMarkSduCount;
    UInt16                highWaterMarkSduCount;
    UInt16                dlWaterMarkEnabled;
}PdcpRrmEcnConfigParam;    

/* ECN changes end*/

/* IP Packet type received by PDCP */
typedef enum PdcpIpPktTypeT
{
    NOT_DETERMINED = 0,
    IP_TYPE_IPV4   = 1,
    IP_TYPE_IPV6   = 2
} PdcpIpPktType;

/* PDCP data packet type received i.e. voice or silence */
typedef enum PdcpVoiceSilenceTypeT
{
    PDCP_VOICE_PKT   = 0,
    PDCP_SILENCE_PKT = 1,
    PDCP_CONTROL_PDU = 2,   
    PDCP_NO_PACKET   = 2,
    
    PDCP_MAX_SPS_PKT_TYPE,
    PDCP_INVALID_SPS_PKT_TYPE
}PdcpVoiceSilenceType;

/* Enum for SPS Periodicity that can be configured */
typedef enum PdcpSpsPeriodicityT
{
    PDCP_SPS_INTERVAL_10  = 10,
    PDCP_SPS_INTERVAL_20  = 20,
    PDCP_SPS_INTERVAL_32  = 32,
    PDCP_SPS_INTERVAL_40  = 40,
    PDCP_SPS_INTERVAL_64  = 64,
    PDCP_SPS_INTERVAL_80  = 80,
    PDCP_SPS_INTERVAL_128 = 128,
    PDCP_SPS_INTERVAL_160 = 160,
    PDCP_SPS_INTERVAL_320 = 320,
    PDCP_SPS_INTERVAL_640 = 640, 
    
    PDCP_SPS_INTERVAL_MAX
} PdcpSpsPeriodicity ;

/* States maintained by PDCP in UL direction for SPS */
typedef enum PdcpSpsStateRxEnumT
{
    PDCP_SPS_IDLE   = 0,
    PDCP_SPS_CONFIGURED,                      /*1*/
    PDCP_SPS_ACTIVATED_SILENCE_NOT_DETECTED,  /*2*/
    PDCP_SPS_ACTIVATED_SILENCE_DETECTED,      /*3*/
    PDCP_SPS_DEACTIVATED_VOICE_NOT_DETECTED,  /*4*/
    PDCP_SPS_DEACTIVATED_VOICE_DETECTED,      /*5*/

    PDCP_MAX_SPS_RX_STATE,
    PDCP_INVALID_SPS_RX_STATE
}PdcpSpsStateRxEnum ;

/* Events received from MAC by PDCP in case of SPS */
typedef enum PdcpSpsEventRxEnumT
{
    PDCP_SPS_RRC_CONFIG = 0,
    PDCP_SPS_CONFIGURE,   /*1*/
    PDCP_SPS_ACTIVATED,   /*2*/
    PDCP_SPS_DEACTIVATED, /*3*/
   
    PDCP_MAX_SPS_RX_EVENT,
    PDCP_INVALID_SPS_RX_EVENT        
}PdcpSpsEventRxEnum;

#define MAX_DEACTIVATED_RECEIVED_IN_UL  10
/* SPR 10894 fix start*/
/* Following macros defines the type of handover triggered for forwarding */
#define NORMAL_HO     0
#define INTRA_ENB_HO  1

typedef UInt8 PdcpHoType;
/* SPR 10894 fix end*/

/* + SPR 19066 */
/* For Ue Inactivity In MiliSecond */
typedef enum PdcpUeInactiveTimeMsT
{
    UE_INACTIVITY_TIME_S1_MS = 1,
    UE_INACTIVITY_TIME_S2_MS = 2,
    UE_INACTIVITY_TIME_S3_MS = 3,
    UE_INACTIVITY_TIME_S5_MS = 5,
    UE_INACTIVITY_TIME_S7_MS = 7,
    UE_INACTIVITY_TIME_S10_MS = 10,
    UE_INACTIVITY_TIME_S15_MS = 15,
    UE_INACTIVITY_TIME_S20_MS = 20,
    UE_INACTIVITY_TIME_S25_MS = 25,
    UE_INACTIVITY_TIME_S30_MS = 30,
    UE_INACTIVITY_TIME_S40_MS = 40,
    UE_INACTIVITY_TIME_S50_MS = 50,
    UE_INACTIVITY_TIME_MIN1_MS = 60,
    UE_INACTIVITY_TIME_MIN1S20_MS = 80,
    UE_INACTIVITY_TIME_MIN1S40_MS = 100,
    UE_INACTIVITY_TIME_MIN2_MS = 120,
    UE_INACTIVITY_TIME_MIN2S30_MS = 150,
    UE_INACTIVITY_TIME_MIN3_MS = 180,
    UE_INACTIVITY_TIME_MIN3S30_MS = 210,
    UE_INACTIVITY_TIME_MIN4_MS = 240,
    UE_INACTIVITY_TIME_MIN5_MS = 300,
    UE_INACTIVITY_TIME_MIN6_MS = 360,
    UE_INACTIVITY_TIME_MIN7_MS = 420,
    UE_INACTIVITY_TIME_MIN8_MS = 480,
    UE_INACTIVITY_TIME_MIN9_MS = 540,
    UE_INACTIVITY_TIME_MIN10_MS = 600,
    UE_INACTIVITY_TIME_MIN12_MS = 720,
    UE_INACTIVITY_TIME_MIN14_MS = 840,
    UE_INACTIVITY_TIME_MIN17_MS = 1020,
    UE_INACTIVITY_TIME_MIN20_MS = 1200,
    UE_INACTIVITY_TIME_MIN24_MS = 1440,
    UE_INACTIVITY_TIME_MIN28_MS = 1680,
    UE_INACTIVITY_TIME_MIN33_MS = 1980,
    UE_INACTIVITY_TIME_MIN38_MS = 2280,
    UE_INACTIVITY_TIME_MIN44_MS = 2640,
    UE_INACTIVITY_TIME_MIN50_MS = 3000,
    UE_INACTIVITY_TIME_HR1_MS = 3600,
    UE_INACTIVITY_TIME_HR1MIN30_MS = 5400,
    UE_INACTIVITY_TIME_HR2_MS = 7200,
    UE_INACTIVITY_TIME_HR2MIN30_MS = 9000,
    UE_INACTIVITY_TIME_HR3_MS = 10800,
    UE_INACTIVITY_TIME_HR3MIN30_MS = 12600,
    UE_INACTIVITY_TIME_HR4_MS = 14400,
    UE_INACTIVITY_TIME_HR5_MS = 18000,
    UE_INACTIVITY_TIME_HR6_MS = 21600,
    UE_INACTIVITY_TIME_HR8_MS = 28800,
    UE_INACTIVITY_TIME_HR10_MS = 36000,
    UE_INACTIVITY_TIME_HR13_MS = 46800,
    UE_INACTIVITY_TIME_HR16_MS = 57600,
    UE_INACTIVITY_TIME_HR20_MS = 72000,
    UE_INACTIVITY_TIME_DAY1_MS = 86400,
    UE_INACTIVITY_TIME_DAY1HR12_MS = 21600,
    UE_INACTIVITY_TIME_DAY2_MS =  172800,
    UE_INACTIVITY_TIME_DAY2HR12_MS = 216000,
    UE_INACTIVITY_TIME_DAY3_MS = 259200,
    UE_INACTIVITY_TIME_DAY4_MS = 345600,
    UE_INACTIVITY_TIME_DAY5_MS = 432000,
    UE_INACTIVITY_TIME_DAY7_MS = 604800,
    UE_INACTIVITY_TIME_DAY10_MS = 864000,
    UE_INACTIVITY_TIME_DAY14_MS = 1209600,
    UE_INACTIVITY_TIME_DAY19_MS = 1641600,
    UE_INACTIVITY_TIME_DAY24_MS = 2073600,
    UE_INACTIVITY_TIME_DAY30_MS = 2592000,
    UE_INACTIVITY_TIME_DAYMORETHAN30_MS 
} PdcpUeInactiveTimeMs;

/* Following macro used for converting the time to sec from ms */
#define ONE_THOUSAND 1000
/* - SPR 19066 */

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*PDCP Entities Integrity protection parameters*/
typedef struct PdcpIntegrityParamsT
{
    UInt16	algorithmId;   /*Integrity protection algorithm id */
    UInt8	key[LTE_SECURITY_KEY_LENGTH];	/*integrity key */
} PdcpIntegrityParams, *LP_PdcpIntegrityParams;

/*PDCP Entities Ciphering parameters*/
typedef struct PdcpCipheringParamsT
{
    UInt16	algorithmId;       /* New Ciphering algorithm id */
    /* SPR 2739 changes start */
    UInt16	newAlgorithmId;    /* New Ciphering algorithm id */
    /* SPR 2739 changes end */
    UInt8	key[LTE_SECURITY_KEY_LENGTH];	  /* ciphering key*/
    /* SPR 2739 changes start */
    UInt8	newKey[LTE_SECURITY_KEY_LENGTH];  /*new ciphering key*/
    /* SPR 2739 changes end */
} PdcpCipheringParams, *LP_PdcpCipheringParams;

/*PDCP Entities ROHC parameters*/
typedef struct PdcpRohcParamsT
{
    void*	context;	/*Context for ROHC (compressor or decompressor), PNULL if no ROHC applicable*/
} PdcpRohcParams, *LP_PdcpRohcParams;

/*PDCP UL Bitmap */
typedef struct PdcpStatusInfoT
{
/* SPR 3608 changes start */
    UInt8    noOfBytes;
/* SPR 3608 changes end */
    UInt8    bitmap[BITMAP_SIZE];
}PdcpStatusInfo, *LP_PdcpStatusInfoT;



/*PDCP Entity context (for all types of entities)*/
typedef struct PdcpEnityContextT
{
    /* Following queue is for handling packets at TX side */
    LTE_QUEUE   pdcpTxDataReqQ;
    LTE_QUEUE	pdcpStorageQ;      /*storage at TX side*/

    /* Following queue is for handling packets at RX side */
    LTE_QUEUE   pdcpRxDataIndQ;
    LTE_QUEUE   pdcpUlReorderingQ; /*storage at RX side*/

    UInt8       radioBearer;    /* Radio Bearer Id */

    UInt16	type;           /*Entity type (PDCP_ENTITY_SRB, PDCP_ENTITY_DRB)*/
    UInt16	rlcMode;        /*RLC mode (PDCP_ENTITY_RLC_UM, PDCP_ENTITY_RLC_AM)*/
    UInt16	rbDirection;    /*Direction (PDCP_ENTITY_RB_TX,PDCP_ENTITY_RB_RX)*/

    UInt16      snSize;         /*SN size in bits*/
    UInt16      next_SN;	/*next SN*/
    UInt16      last_Submitted_SN; /*Last_Submitted_PDCP_RX_SN*/
    UInt32      hfn;		/*HFN*/
    UInt16      discardTimer;   /*DisacrdTimer value,  0 = no discard*/

    PdcpCipheringParams ciphering;      /*Ciphering parameters*/

    PdcpRohcParams	    rohc;			/*ROHC parameters. DRB only*/
    UInt8               statusReportRequired;   /*Status report sending allowed for RX DRB Only*/
    UInt16            fms;
    UInt8             ueStatusReportReq;
    /* This flag will be set when PSR is received for LC */
    UInt8             isPsrRecvd;

    PdcpIntegrityParams	integrity;      /*Integrity protection parameters, SRB only*/
    /*
     ** indicates if integrity/ciphering configuration can be performed at 
     ** SRB for context 
     */
    PdcpEntityDirectionSet  isDirectionSetSrb;  
    /*
     ** indicates if ciphering configuration can be performed at DRB for 
     ** context
     */
    PdcpEntityDirectionSet  isDirectionSetDrb;
    PdcpStatusInfo    pdcpUlStatusInfo;

    UInt8             isEndMarkerIndRecvd;   /* Sets when EMI recvd from PR */
    UInt8 isEMIFwded ;                       /* Sets when EMI is frwded to RP*/
    /* This field is set when Lossy HO tag is received and reset 
     * once data buffer stop is received */
    UInt8 lossyHoRab;
    UInt8 congestionStatus;
    UInt8 ecnIndtobeSent;
    /* This flag checks if the forwarding is enabled for this particular RAB */
    UInt8 isFwdingEnabled;
    /* Following fields are for data forwarding */
    UInt32          numberOfNodesFwded;
    LP_PdcpDataReq  dataNode;
    UInt8  ipPktType; /* Defines the packet types received by this LC */
    /* Packet Arrival Time received from RLC in RX */
    tickType_t pktArrivalTs; 
#ifndef UE_SIM_TESTING
    UInt8  qci;       /* QCI value for this LC */
    UInt8  isSpsLc;   /* Sets if it is SPS LC */
    UInt8  numValidSdu; /* Number of valid SDUs */
    /* Index in sduArray for last SDU received pointer */
    UInt8  arrIndex;
    /* Counter to forward Average SDU size to MAC in UL */
    UInt8  numPktsAvgSduProvided; 
    /* Sum of sizes of last NUM_RLC_SDU_FOR_RUNNING_AVG voice packets */
    UInt32 sumSduSize;
    /* Array for computing running average SDU */
    UInt16 sduArray[NUM_RLC_SDU_FOR_RUNNING_AVG];
#endif
#ifdef PDCP_ASYNC_INTERFACE
    LP_PdcpSecContextInfo pdcpSecurityContext_p ;
#endif
    /* SPR 3607 Fix Start */
    PdcpEntitySecurityOffset securityOffset;
    /* New Integrity protection parameters, SRB only - Intra ENB HO*/
    PdcpIntegrityParams	new_integrity;      
    /* New Ciphering parameters - Intra ENB HO*/
    PdcpCipheringParams new_ciphering;      
    UInt64 storageCount;

#ifndef UE_SIM_TESTING
	UInt8 reconfigUeFlag;
#endif
#ifdef ENDC_ENABLED
    /* Flag to indicate whether traffic is allowed on default MeNB bearer.
     * This flag will be updated by RRC*/
    UInt8               isTrafficAllowed;  
#endif
} PdcpEnityContext, *LP_PdcpEnityContext;

/*PDCP UE context definition */
typedef struct PdcpUeContextT
{   
    PdcpStatesTxEnum  pdcpTxSrb1State;    /*State at TX side for SRB1*/
    PdcpStatesTxEnum  pdcpTxSrb2DrbState; /*State at TX side for SRB2 and DRB*/

    PdcpStatesRxEnum  pdcpRxState;        /*State at RX side for an UE*/

    PdcpRrcTxEventsEnum event;           /* Event for changing PDCP state */
    PdcpSpsStateRxEnum pdcpRxSpsState;  /*State at RX side for an SPS UE */
    tickType_t            pdcpPSRTimer;
    UInt16            crnti;             /* unique identification of UE */
    /* Tick associated when Delete UE request received*/
    tickType_t            tickAss; 
    /* + SPR 19066 */
    /* Following field indicates the inactivity timer value beyond lapse 
     * of this time, UE is inactive. Value = 0 indicates inactivity feature
     * is disabled. This value is in ms */
    tickType_t            inactivityTimerVal; 
    /* Following field indicates the tick at which last packet is deleted
     * from DRB storage queues in Downlink */
    tickType_t            lastSchedTickDl;

    /* Following field indicates the tick at which data is sent to
     * Packet Relay */
    tickType_t            lastSchedTickUl;

    /* Following field indicates the lapsed inactive time at source eNB.
     * The value is in ms. This field is applicable at target till UE is 
     * not declared inactive once */
    tickType_t            lapsedInactiveTime;
    /* - SPR 19066 */
    /* Stores the HO type for data forwarding (Default: NORMAL_HO ) */
    UInt16            newUeIndex;
    PdcpHoType        hoType;           
    UInt8             numOfActiveSrbLc;  /* total number of active LCs */
    UInt8             numOfActiveDrbLc;  /* total number of active LCs */

    /* Sets if forwarding API has been recvd from Packet Relay */
    UInt8             isFwdingApiRecvd;
    /* Total Number of PSR to be sent to RLC */
    UInt8             totNumOfPSRtoSend;
    /* Number of PSR to sent to RLC */
    UInt8             numOfPSRSent;
    /* total number of PSR to be received */
    UInt8             totNumPsrRecvd;
    /* Number of PDCP Status Report Recvd from UE in AM Mode */
    UInt8             numOfPsrRecvd;
    /* Following arrays consists of list of Active LC */
    UInt8             activeLcInfo[PDCP_MAX_DRB];
    UInt8             activeLcIndex[PDCP_MAX_DRB];
    /* SPS interval in DL */
    UInt8             semiPersistentSchedIntervalDL;
    /* SPS interval in UL */
    UInt8             semiPersistentSchedIntervalUL;
    /* SETUP or RELEASE for SPS UE in DL */
    UInt8             dlSpsReqType;
    /* SETUP or RELEASE for SPS UE in UL */
    UInt8             ulSpsReqType;
    /* Count indicating number of Deactivation received from MAC */
    UInt8             numDeactivatedReceived;
    /* Set when HO trigger tag is recvd at target eNB, it will be reset once
     * Data_Buffer_Stop_Ind is recveived  */
    UInt8             hoTriggerRecvd;
    /* Set if DL forwarding queue is initialized */
    UInt8             dlFwdingQInitilized;
    /*
    ** Following queue is to receive data packets from src eNB PDCP. This queue
    ** will only be used at the time of handover 
    */
    LTE_QUEUE        pdcpDataForwardingDlQ;

    /*PDCP entities contexts array for TX([0]),RX([1])*/
    LP_PdcpEnityContext  pdcpEntities[LTE_MAX_LC_ID][PDCP_ENTITY_RB_BOTH];
#ifndef UE_SIM_TESTING
    UInt16 rrcUeIndex;
    InternalCellIndex  cellIndex;
#endif
} PdcpUeContext, *LP_PdcpUeContext;

typedef struct PdcpTxPendingDeletionReqT {
    LTE_SNODE   sNode;
    UInt8    lcId;
    UInt16   ueIndex ;
    LP_PdcpUeContext ueContext_p ;
}PdcpTxPendingDeletionReq, *LP_PdcpTxPendingDeletionReq;

typedef struct PdcpRxPendingDeletionReqT {
    LTE_SNODE   sNode;
    UInt8    lcId;
    UInt8      deleteContext ;
    UInt16   ueIndex ;
    LP_PdcpUeContext ueContext_p ;
}PdcpRxPendingDeletionReq, *LP_PdcpRxPendingDeletionReq;

#ifndef UE_SIM_TESTING
typedef struct PdcpSCellInfoT
{
    UInt8               cellIndex;  
    UInt16              maxUeNumber;    /* Maximum number of supported UEs for 
                                           this cell of given cellIndex*/
    UInt32      thresholdValue; /* the maximum threshold value for 
                                   Count Wraparound*/
    /* for enbling/disabling SPS scheduling in MAC */
    UInt8               isSpsEnable;   
    /* Array containing list of QCIs for which SPS is enabled. Indices
     * corresponds to QCI. Value corr to SPS enabled/disabled for that QCI. */
    UInt8               qciSpslist[MAX_QCI] ;
    /* list of DL SPS intervals supported. By default only 20ms shall be
     * enabled and rest of the values disabled.
     * Values from index 10 to 15 are for future extensions */
    UInt8 supportedSpsDlIntervalList[SPS_INTERVAL_LIST];
    /* list of UL SPS intervals supported. By default only 20ms shall be
     * enabled and rest of the values disabled.
     * Values from index 10 to 15 are for future extensions */
    UInt8 supportedSpsUlIntervalList[SPS_INTERVAL_LIST];
    /* SPS interval profile parameters */
    LP_PdcpSpsIntervalProfile spsProfileParams_p[SPS_INTERVAL_LIST];

}PdcpSCellInfo;
#endif




/*PDCP layer context definition */
typedef struct PdcpContextT
{
#ifndef UE_SIM_TESTING
    UInt8               sCellCount; /* Maximum Number of total secondary 
                                       cell's */
    PdcpSCellInfo sCellInfo[MAX_NUM_CELL]; /* Array containing SPS Info of
                                                   secondary cell's */

#endif

    UInt16              isInitialized;  /* Initialization flag */
    UInt16              maxUeNumber;    /* Maximum number of supported UEs */
    UInt16              rntiStartRange; /* the starting value of rnti to be supported for ueId */
    UInt16              rntiEndRange;   /* the maximum rnti value to be supported for ueId */
	UInt16				core_bind;		/* which core should be bind to */


    UInt16              numOfActiveUe;  /* number of active UEs */
    /* Following arrays consists of list of Active UEs */
    UInt16              activeUeInfo[MAX_UE_SUPPORTED];
    UInt16              activeUeIndex[MAX_UE_SUPPORTED];
    MSGPOOL             msgPool;
    LP_PdcpUeContext    ueContexts[MAX_UE_SUPPORTED];     /* UE contexts array */

    /* Array Containing ECN Configuration parameters */
    PdcpRrmEcnConfigParam pdcpEcnConfigParam[MAX_NUM_CELL];


} PdcpContext, *LP_PdcpContext;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define PDCP_GET_UE_CONTEXT(ueIndex) \
   ( ueIndex >= pdcpContext.maxUeNumber ) ? PNULL :\
       pdcpContext.ueContexts[ueIndex] \

#define PDCP_GET_VALID_UE_CONTEXT(ueIndex) \
   pdcpContext.ueContexts[ueIndex]

#define PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, lcId, rbDirection ) \
   ueCtx_p->pdcpEntities[lcId - 1][rbDirection]

#define PDCP_GET_CELL_INDEX(ueIndex) \
   pdcpContext.ueContexts[ueIndex]->cellIndex

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
typedef UInt32 (*pdcpHandlerRrcStateT) (UInt16 ueIndex, UInt8 lcId, 
                  UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p);

extern pdcpHandlerRrcStateT pdcpRrcTxStateMachine[PDCP_TX_MAX_STATE]
                                                 [PDCP_MAX_RRC_TX_EVENT];

extern PdcpContext    pdcpContext;
extern UInt16 pdcpNumOfSupportedUsers_g;
extern UInt16 pdcpNumOfSupportedLC_g;
extern UInt8  pdcpDlMemFac_g;
extern UInt8  pdcpUlMemFac_g;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpContextCreateMemPool 
 * Inputs         : maxUeNumber - Maximum number of supported UEs
                    txReservationCoef - Number of blocks for TX purposes
                    rxReservationCoef - Number of blocks for RX purposes
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code.
 * Description    : Reserve memory in pools for PDCP Context purposes.
 ****************************************************************************/
UInt16 pdcpContextCreateMemPool( UInt16 maxUeNumber, UInt16 txReservationCoef);

/****************************************************************************
 * Function Name  : pdcpContextGetMsgPool 
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : MSGPOOL or PNULL if pool isn't allocated.
 * Description    : Returns memory pools for using with msgAlloc.
 ****************************************************************************/
MSGPOOL pdcpContextGetMsgPool( void );

/****************************************************************************
 * Function Name  : pdcpContextInit 
 * Inputs         : LP_PdcpOamInitLayerReq initParams_p
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code.
 * Description    : It inits PDCP Context data.
 ****************************************************************************/
UInt16 pdcpContextInit(LP_PdcpOamInitLayerReq initParams_p
#ifndef UE_SIM_TESTING
                       ,RrcCellIndex cellId
#endif
                      );


/****************************************************************************
 * Function Name  : pdcpContextCleanup 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code.
 * Description    : It cleans PDCP Context data.
 ****************************************************************************/
UInt16 pdcpContextCleanup(void);

/****************************************************************************
 * Function Name  : pdcpContextIsInitialized
 * Inputs         :     
 * Outputs        : None.
 * Returns        : 1 - if PDCP context is initialized else returns 0.
 * Description    : .
 ****************************************************************************/
UInt16 pdcpContextIsInitialized(void);

/****************************************************************************
 * Function Name  : pdcpContextGetMaxUeNumber
 * Inputs         :     
 * Outputs        : None.
 * Returns        : Maximum number of supported UEs.
 * Description    : .
 ****************************************************************************/
UInt16 pdcpContextGetMaxUeNumber(void);

/****************************************************************************
 * Function Name  : pdcpNumActiveUe
 * Inputs         : None   
 * Outputs        : None.
 * Returns        : Number of Active UEs
 * Description    : The function gives the number of active UEs
 ****************************************************************************/
UInt16 pdcpNumActiveUe (void);

/****************************************************************************
 * Function Name  : pdcpUeContextCreate
 * Inputs         : ueIndex - UE Id.
                    crnti - unique identification of UE.
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It creates PDCP UE context.
 ****************************************************************************/
UInt16 pdcpUeContextCreate( UInt16 ueIndex, UInt16 crnti);

/****************************************************************************
 * Function Name  : pdcpUeContextDelete
 * Inputs         : ueIndex - UE Id.
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It deletes PDCP UE context and all PDCP Entities within this UE.
 ****************************************************************************/
UInt16 pdcpUeContextDelete( UInt16 ueIndex );

/* SPR_1235_FIX_START*/
/****************************************************************************
 * Function Name  : pdcpEntitySetDirectionFlag
 * Inputs         : ueIndex - UE Id.
 *                  lcId - logical channel id.
 *                  rbDirection - Radio bearer link type (TX, RX, Both).
 *                  type - entity type (PDCP_ENTITY_SRB, PDCP_ENTITY_DRB).
 * Outputs        : None.
 * Returns        : void
 * Description    : It sets isDirection field set for SRB reconfiguration 
 *                  request.
 ****************************************************************************/
void pdcpEntitySetDirectionFlag( 
    UInt16 ueIndex, 
    UInt16 lcId, 
    UInt16 rbDirection,
    UInt16 type  );
/* SPR_1235_FIX_END*/

/****************************************************************************
 * Function Name  : pdcpEntityCreate
 * Inputs         : ueIndex - UE Id.
                    lcId - logical channel id.
                    type - entity type (PDCP_ENTITY_SRB, PDCP_ENTITY_DRB).
                    rlcMode - RLC link mode (UM, AM).
                    rbDirection - Radio bearer link type (TX, RX, Both).
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It creates TX and/or RX PDCP Entities for RB which is identified ueIndex, lcId.
 ****************************************************************************/
UInt16 pdcpEntityCreate( UInt16 ueIndex, UInt16 lcId, UInt16 type, 
        UInt16 rlcMode, UInt16 rbDirection,
        /* SPR 3276 changes start */
        UInt8 radioBearer );
        /* SPR 3276 changes end */

/****************************************************************************
 * Function Name  : pdcpSrbEntityFree
 * Inputs         : ueIndex - UE Id.
                    lcId - logical channel id.
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : To free TX/RX contexts for given ueIndex and lcId(SRB)
 ****************************************************************************/
UInt16 pdcpSrbEntityFree( UInt16 ueIndex, UInt16 lcId );

/****************************************************************************
 * Function Name  : pdcpDrbEntityFree
 * Inputs         : ueIndex - UE Id.
                    lcId - logical channel id.
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : To free TX/RX contexts for given ueIndex and lcId(DRB)
 ****************************************************************************/
UInt16 pdcpDrbEntityFree( UInt16 ueIndex, UInt16 lcId );


/****************************************************************************
 * Function Name  : pdcpGetEntityContext
 * Inputs         : 
 * Outputs        : 
 * Returns        : LP_PdcpEnityContext, PNULL if does not exist.
 * Description    : returns context entity for given ueIndex, lcId, rbDirection.
 ****************************************************************************/
LP_PdcpEnityContext pdcpGetEntityContext( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection );

/****************************************************************************
 * Function Name  : pdcpEntityConfigureIntegrity
 * Inputs         : ueIndex - . 
                    lcId - .
                    algorithmId - .
                    key - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures Integrity protection parameters for Entities in any direction.
 ****************************************************************************/
void pdcpEntityConfigureIntegrity( UInt16 ueIndex, UInt16 lcId, 
        /*SPR 10446 fix start */
        UInt16 algorithmId, UInt8* key, UInt8 securityOffset );
        /*SPR 10446 fix end */ 

/****************************************************************************
 * Function Name  : pdcpEntityConfigureDiscardTimer
 * Inputs         : ueIndex - . 
                    lcId - .
                    rbDirection - .
                    discardTimer - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures DiscardTimer feature for Entities in any direction.
 ****************************************************************************/
UInt16 pdcpEntityConfigureDiscardTimer( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection, UInt16 discardTimer );

/****************************************************************************
 * Function Name  : pdcpEntityConfigureSnSize
 * Inputs         : ueIndex - . 
                    lcId - .
                    rbDirection - .
                    snSize - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures snSize value for Entities in any direction.
 ****************************************************************************/
UInt16 pdcpEntityConfigureSnSize( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection, UInt16 snSize );

/****************************************************************************
 * Function Name  : pdcpEntityQci
 * Inputs         : ueIndex - . 
                    lcId - .
                    rbDirection - .
                    qci - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures snSize value for Entities in any direction.
 ****************************************************************************/
UInt16 pdcpEntityQci( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection, UInt8 qci );

/****************************************************************************
 * Function Name  : pdcpEntityConfigureStatusReportRequired
 * Inputs         : ueIndex - . 
                    lcId - .
                    rbDirection - .
                    snSize - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures statusReportRequired value for Entities in DL direction.
 ****************************************************************************/
UInt16 pdcpEntityConfigureStatusReportRequired( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection, UInt8 StatusReportRequired );

/****************************************************************************
 * Function Name  : pdcpEntityConfigureSrbCiphering
 * Inputs         : ueIndex - . 
                    lcId - .
                    rbDirection - .
                    algorithmId - .
                    key - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures Ciphering parameters for Entities in any direction.
 ****************************************************************************/
void pdcpEntityConfigureSrbCiphering( UInt16 ueIndex, UInt16 lcId,
        /*SPR 10446 fix start */
        UInt16 algorithmId, UInt8* key, UInt8 securityOffset );
        /*SPR 10446 fix end */ 

/* SPR_1235_FIX_START*/
/****************************************************************************
 * Function Name  : pdcpEntityConfigureDrbCiphering
 * Inputs         : ueIndex - . 
 *                  lcId - .
 *                  algorithmId - .
 *                  key - .
 * Outputs        : 
 * Returns        : void
 * Description    : It configures Ciphering parameters for Entities in 
 *                  any direction.
 ****************************************************************************/
void pdcpEntityConfigureDrbCiphering( UInt16 ueIndex, UInt16 lcId, 
        UInt16 algorithmId, UInt8* key );
/* SPR_1235_FIX_END*/

/****************************************************************************
 * Function Name  : pdcpEntityConfigureRohc
 * Inputs         : ueIndex - . 
                    lcId - .
                    rbDirection - .
                    profileIDsMask - .
                    maxCid - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures ROHC for Entities in any direction.
 ****************************************************************************/
UInt16 pdcpEntityConfigureRohc( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection, UInt16 profileIDsMask, UInt16 maxCid );

/****************************************************************************
 * Function Name  : pdcpEntityReConfigureRohc
 * Inputs         : ueIndex - UE Id. lcId rbDir,profileIDsMask,maxCid 
 * Outputs        : 
 * Returns        : LP_PdcpUeContext, PNULL if does not exist.
 * Description    : returns pointer on UE context for given ueIndex.
 ****************************************************************************/

UInt16 pdcpEntityReConfigureRohc( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection, UInt16 profileIDsMask, UInt16 maxCid );

UInt16 pdcpEntityContextFree( LP_PdcpEnityContext entity_p, 
        UInt16 ueIndex, 
        UInt16 lcId, 
        /* SPR 3159 changes start */
        UInt16 rbDirection
        /* SPR 3159 changes end */);

/****************************************************************************
 * Function Name  : pdcpSuspendAllRbEntities
 * Inputs         : ueIndex - UE Id. 
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE.
 * Description    : Suspends all the entities configured for an UE.
 ****************************************************************************/
UInt16 pdcpSuspendAllRbEntities( UInt16 ueIndex );

/****************************************************************************
 * Function Name  : pdcpResumeRbEntity
 * Inputs         : ueIndex - UE Id, lcId - LC ID
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE.
 * Description    : Resumes RB entity configured for an UE.
 ****************************************************************************/
UInt16 pdcpResumeRbEntity( UInt16 ueIndex, UInt8 lcId,
        /* SPR 3569 changes start */
        UInt16 resumeDirection );
        /* SPR 3569 changes end */



/****************************************************************************
 * Function Name  : pdcpReEstablishDl
 * Inputs         : ueIndex - UE Id, lcId - LC ID
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE.
 * Description    : Re-establishes RB entity configured for an UE for DL.
 ****************************************************************************/
UInt16 pdcpReEstablishDl( UInt16 ueIndex, UInt8 lcId );

/****************************************************************************
 * Function Name  : pdcpReEstablishUl
 * Inputs         : ueIndex - UE Id, lcId - LC ID
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE.
 * Description    : Re-establishes RB entity configured for an UE for UL.
 ****************************************************************************/
UInt16 pdcpReEstablishUl( UInt16 ueIndex, UInt8 lcId );

/* SRP 3608 changes start */
/****************************************************************************
 * Function Name  : pdcpStorageQFindNode
 * Inputs         : ueIndex,lcId, key
 * Outputs        : None.
 * Returns        : SUCCESS/FAIL
 * Description    : search storage for node and deletes the same with given key
 *                  and return LP_PdcpDataReq
 ****************************************************************************/
UInt32 pdcpStorageQFindNode(LP_PdcpEnityContext ctx_p, UInt16 key);
/****************************************************************************
 * Function Name  : pdcpFwdQFindNode
 * Inputs         : ueIndex,lcId, key
 * Outputs        : None.
 * Returns        : SUCCESS/FAIL
 * Description    : search Fwding Queue for node and deletes the same with given key
 *                  and return LP_PdcpDataReq
 *****************************************************************************/
UInt32 pdcpFwdQFindNode(LP_PdcpUeContext ueCtx_p, UInt16 lcId, UInt32 key);

/* SPR 3608 changes end */

/****************************************************************************
 * Function Name  : pdcpRxStorageQFindNode
 * Inputs         : PDCP context, key
 * Outputs        : None.
 * Returns        : pointer to found node in Q, PNULL if node not found
 * Description    : search storage for node and deletes the same with given key
 *                  and return LP_PdcpDataReq
 ****************************************************************************/
LP_PdcpDataInd pdcpRxStorageQFindNode(LP_PdcpEnityContext ctx_p, UInt32 key);

/******************************************************************************
 * Function Name  : pdcpEncodeUlBitmap
 * Inputs         : ueIndex, lcId, ctx_p - UE + LC context
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE.
 * Description    : Encodes bitmap for UL packets received for a LC. This
 *                  function is called only when StatusReportRequired field set
 *                  true by RRC.
 *****************************************************************************/
UInt16 pdcpEncodeUlBitmap( UInt16 ueIndex, UInt8 lcId,
        LP_PdcpEnityContext ctx_p);

/****************************************************************************
 * Function Name  : updateActiveUeInfo
 * Inputs         : activeUeInfo - list of Active UEs
 *                  activeUeIndex - indices for active UEs in activeUeInfo
 *                  ueIndex - UE Id. 
 * Outputs        : 
 * Returns        : None.
 * Description    : updates list of Active UEs in the system.
 ****************************************************************************/
void updateActiveUeInfo( UInt16 ueIndex, UInt16 *activeUeInfo_p, 
        UInt16 *activeUeIndex_p);

/****************************************************************************
 * Function Name  : updateActiveLcInfo
 * Inputs         : activeLcInfo - list of Active LCs
 *                  activeLcIndex - indices for active LCs in activeLcInfo
 *                  lcId - LC Id. 
 * Outputs        : 
 * Returns        : None.
 * Description    : updates list of Active DRBs in the UE.
 ****************************************************************************/
void updateActiveLcInfo( UInt16 ueIndex, UInt8 lcId, 
        UInt8 *activeLcInfo_p, UInt8 *activeLcIndex_p);

/****************************************************************************
 * Function Name  : pdcpSetConnectedState
 * Inputs         : ueIndex, lcId, state, event, ue Context
 * Outputs        : 
 * Returns        : None.
 * Description    : sets PDCP TX state to connected.
 ****************************************************************************/
UInt32 pdcpSetConnectedState( UInt16 ueIndex, UInt8 lcId, UInt32 state, 
                              UInt32 event, LP_PdcpUeContext ueCtx_p );

/****************************************************************************
 * Function Name  : pdcpInvalidState
 * Inputs         : ueIndex, lcId, state, event, ue Context
 * Outputs        : 
 * Returns        : None.
 * Description    : Logs an error for invalid state.
 ****************************************************************************/
UInt32 pdcpInvalidState( UInt16 ueIndex, UInt8 lcId, UInt32 state, 
                         UInt32 event, LP_PdcpUeContext ueCtx_p );

/****************************************************************************
 * Function Name  : pdcpSetHoState
 * Inputs         : ueIndex, lcId, state, event, ue Context
 * Outputs        : 
 * Returns        : None.
 * Description    : sets PDCP TX state to HO.
 ****************************************************************************/
UInt32 pdcpSetHoState( UInt16 ueIndex, UInt8 lcId, UInt32 state, 
                       UInt32 event, LP_PdcpUeContext ueCtx_p );

/****************************************************************************
 * Function Name  : pdcpSetSuspendedState
 * Inputs         : ueIndex, lcId, state, event, ue Context
 * Outputs        : 
 * Returns        : None.
 * Description    : sets PDCP TX state to suspended.
 ****************************************************************************/
UInt32 pdcpSetSuspendedState( UInt16 ueIndex, UInt8 lcId, UInt32 state, 
                              UInt32 event, LP_PdcpUeContext ueCtx_p );

/****************************************************************************
 * Function Name  : pdcpSetResumeState
 * Inputs         : ueIndex, lcId, state, event, ue Context
 * Outputs        : 
 * Returns        : None.
 * Description    : sets PDCP TX state to resume.
 ****************************************************************************/
UInt32 pdcpSetResumeState( UInt16 ueIndex, UInt8 lcId, UInt32 state, 
                           UInt32 event, LP_PdcpUeContext ueCtx_p );


#ifdef PDCP_ASYNC_INTERFACE
/****************************************************************************
 * Function Name  : pdcpCreateSecContext
 * Inputs         : entity context, radio bearer
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE
 * Description    : Creates shared descriptor with security engine
 ****************************************************************************/
UInt16 pdcpCreateSecContext (
        /* SPR 15236 fix start */
        UInt16 ueIndex, UInt8 lcId, 
        /* SPR 15236 fix start */
        LP_PdcpEnityContext entity_p,
        /* SPR 3276 changes start */
        UInt8 radioBearer );
        /* SPR 3276 changes end */
#endif

/* SPR 3608 changes start */
/****************************************************************************
 * Function Name  : pdcpEntityConfigureUeStatusReportReq
 * Inputs         : ueIndex - .
 *                : lcId - .
 *                : rbDirection - .
 *                : snSize - .
 * Outputs        :
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures UestatusReportRequired value for Entities in UL direction.
 *****************************************************************************/
UInt16 pdcpEntityConfigureUeStatusReportReq( UInt16 ueIndex, UInt8 lcId,
                UInt16 rbDirection, UInt8 StatusReportRequired );


/****************************************************************************
 * Function Name  : pdcpEncodePSR
 * Inputs         : ueIndex, lcId, ctx_p
 * Outputs        : None.
 * Returns        : Success
 * Description    : Logs an error message and returns
 ****************************************************************************/
void pdcpEncodePSR(UInt16 ueIndex, UInt8 lcId,  LP_PdcpEnityContext ctx_p);

/****************************************************************************
 *** Function Name  : pdcpParsePSR
 *** Inputs         : PDCP context, Pointer to data, length of data
 *** Outputs        : None.
 *** Returns        : PDCP_SUCCESS/PDCP_FAILURE
 *** Description    : Process PSR recieved in control PDU.
 *****************************************************************************/
UInt32 pdcpParsePSR( LP_PdcpUeContext ueCtx_p,LP_PdcpEnityContext ctx_p, UInt16 lcId, void *data, UInt32 len);

/****************************************************************************
 ** Function Name  : pdcpModifyTxStorageQ
 ** Inputs         : PDCP context, sn
 ** Outputs        : None.
 ** Returns        : PDCP_SUCCESS/PDCP_FAILURE
 ** Description    : search storage for node and deletes the same with
 *      given key as sn
 ****************************************************************************/
UInt32 pdcpModifyTxStorageQ(LP_PdcpEnityContext ctx_p, UInt32 sn);

/****************************************************************************
 * Function Name  : pdcpSetAwaitedState
 * Inputs         : ueIndex, lcId, state, event, ue Context
 * Outputs        :
 * Returns        : None.
 * Description    : sets PDCP TX state to resume.
 *****************************************************************************/
UInt32 pdcpSetAwaitedState( UInt16 ueIndex, UInt8 lcId, UInt32 state,
        UInt32 event, LP_PdcpUeContext ueCtx_p );
/* SPR 3608 changes end */

/* SPR 3607 Fix start */
/****************************************************************************
 * Function Name  : pdcpEntityConfigureSecurityOffset
 * Inputs         : ueIndex - .
 *                  lcId - .
 *                  rbDirection - .
 * Outputs        :
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It configures Security Offset value for UE
 ****************************************************************************/
UInt16 pdcpEntityConfigureSecurityOffset( UInt16 ueIndex, UInt16 lcId);

/* ECN changes start*/

/****************************************************************************
 * Function Name  : pdcpSetEcnStatus
 * Inputs         :  .
 * Outputs        :
 * Returns        : None.
 * Description    : It sets the ECN status
 ****************************************************************************/
/* CA changes start */
void pdcpSetEcnStatus(RrmPdcpEcnConfigParamReq *rrmPdcpEcnConfigParam_p, InternalCellIndex cellIndex);
/* CA Changes end */

/****************************************************************************
 * Function Name  : pdcpHandleCongestionNotificationStatus
 * Inputs         :  .
 * Outputs        :
 * Returns        : PDCP_SUCCESS/PDCP_FAILURE
 * Description    : It sets the Congestion Notification status
 ****************************************************************************/
UInt16 pdcpHandleCongestionNotificationStatus ( RrmPdcpDrbEcnCeReq *rrmPdcpDrbEcnCeReq_p,
                                                RrmPdcpDrbEcnCeCnf *rrmPdcpDrbEcnCeCnf_p );

/****************************************************************************
 * Function Name  : calculateAndFillChecksum
 * Inputs         : buff_p .
 * Outputs        :
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It calculates the checksum value and fill the value in input buffer
 ****************************************************************************/
UInt16 pdcpGetIPHdrChecksum(UInt8 count, UInt8 *addr_c);

    /* ECN changes end*/
/*COUNTER_CHK_START*/
/****************************************************************************
 * Function Name  : pdcpGetDrbCountMsb
 * Inputs         : ueIndex - . 
                    lcId - .
                    rbDirection - .
 * Outputs        : 
 * Returns        : PDCP_SUCCESS or error code.
 * Description    : It get the DRB from the Entities in any direction.
 ****************************************************************************/
UInt16 pdcpGetDrbCountMsb( UInt16 ueIndex, UInt16 lcId, UInt16 rbDirection ,UInt32 *count_p);
/*COUNTER_CHK_END*/


#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : pdcpEntitySetsSpsLc
 * Inputs         : ueIndex - UE ID
 *                  ueCtx_p - Pointer to UE context.
 *                  rbDirection - Direction.
 * Outputs        :
 * Returns        : None
 * Description    : It sets or resets isSpsLc.
 ****************************************************************************/
void pdcpEntitySetsSpsLc( UInt16 ueIndex, LP_PdcpUeContext ueCtx_p,
        UInt16 rbDirection );

/******************************************************************************
 * Function Name  : pdcpCalculateAvgRlcSduSize
 * Inputs         : pktType,  RLC SDU size, ctx_p - UE + LC context
 * Outputs        :
 * Returns        : Average SDU size computed.
 * Description    : This function calculates Average RLC SDU size for UL/DL
 *                  packets and passes the same to MAC via BO in DL and queue
 *                  in UL
 *****************************************************************************/
UInt32 pdcpCalculateAvgRlcSduSize( UInt8 pktType, UInt32 size,
    LP_PdcpEnityContext ctx_p );

/****************************************************************************
 * Function Name  : pdcpSpsLogandIgnore
 * Inputs         : ueIndex, lcId, state, event, ctx_p
 * Outputs        : None.
 * Returns        : Success
 * Description    : Logs an error message and returns
 ****************************************************************************/
UInt32 pdcpSpsLogandIgnore(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p,
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx );
#endif



/* + SPR 19066 */
/****************************************************************************
 * Function Name  : pdcpGetInactiveTimeVal
 * Inputs         : ueIndex
 * Outputs        : inactTimeVal_p - Inactive time value for the UE
 * Returns        : Success/Failure
 * Description    : It computes the inactive time value of the UE.
 ****************************************************************************/
tickType_t pdcpGetInactiveTimeVal( UInt16 ueIndex, tickType_t *inactTimeVal_p );

/****************************************************************************
 * Function Name  : pdcpConvertTimetoMs
 * Inputs         : inactiveTimeVal
 * Output         : None
 * Returns        : Time in ms
 * Description    : It converts the time to miliseconds.
 *****************************************************************************/
tickType_t pdcpConvertTimetoMs(tickType_t inactiveTimeVal);
/* - SPR 19066 */
#endif  /* _LIB_PDCP_CONTEXT_DEF_ */
