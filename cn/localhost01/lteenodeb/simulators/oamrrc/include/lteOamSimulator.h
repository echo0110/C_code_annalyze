/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteOamSimulator.h,v 1.1.4.2.4.2.2.1 2010/09/21 16:09:10 gur20491 Exp $ 

 ****************************************************************************
 *
 *  File Description : This file contains declarations for the OAM Interface
 *                     of MAC & RLC and OAM simulator
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------

 *
 ****************************************************************************/

#ifndef LTE_OAM_SIM_H
#define LTE_OAM_SIM_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMemPool.h"
#include "lteTypes.h"
#include "lteLog.h"
#include "lteSQueue.h"
#include "lteThread.h"
#include "lteSync.h"
#include "lteMisc.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "port.h"

#include "lteLayer2CommanTypes.h"

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
#define MAC_SUCCESS 1

#define NUM_OF_UE 100

/* OAM Simulator constants*/
//#define RX_PORT_MAC 13457
#define TX_PORT_MAC 12345
//#define RX_PORT_RLC 13555
#define TX_PORT_RLC 12340
//#define RX_PORT_PDCP 34323
#define TX_PORT_PDCP 34324

#define RX_PORT_OAM 13444

#define MAX_LINE_SIZE 100
#define MIN_UE_ID 10
#define MAX_VALID_UE_ID 0xFFF2
#define BUFFERSIZE 65535
#define WAIT_TIME 1

#define FILE_NAME 20
#define USER_INPUT 16
#define NUM_PARAM 28
#define START_UE_ID 10
#define START_CRNTI_ID 1000
#define IDX 1
#define UE_STATUS 2
#define MAX_LC 11
#define AM_MODE 2 
#define UM_MODE 1
#define RX 0
#define TX 1

#define MAC_LAYER 1
#define RLC_LAYER 2
#define PDCP_LAYER 3

/* 255 UE changes start */
/* +- SPR 18268 */
/* SPR 20526 Fix Start */
#define UE_MAX_RANGE MAX_UE_SUPPORTED
/* 255 UE changes end */
#define MAX_UE  MAX_UE_SUPPORTED
/* SPR 20526 Fix End */
/* +- SPR 18268 */
#define MIN_UE  0
/* MAC OAM API Ids */
/* API Ids 51-100 are reserved for OAM */
#define MAC_INIT_LAYER_REQ          51
#define MAC_INIT_LAYER_CNF          52
#define MAC_CLEANUP_LAYER_REQ       53
#define MAC_CLEANUP_LAYER_CNF       54
#define MAC_GET_BUILD_INFO_REQ      55
#define MAC_GET_BUILD_INFO_CNF      56
#define MAC_SET_LOG_LEVEL_REQ       57
#define MAC_ENABLE_LOG_REQ          58
#define MAC_GET_STATUS_REQ          59 
#define MAC_GET_STATUS_CNF          60
#define MAC_RESET_STATS_REQ         61
#define MAC_GET_STATS_REQ           62 
#define MAC_GET_STATS_CNF           63
#define MAC_ENABLE_DL_SCH_STATS_REQ 64
#define MAC_ENABLE_UL_SCH_STATS_REQ 65
#define MAC_ENABLE_DISABLE_LOG_REQ  66

/* Tags */
#define GET_PER_UE_STATUS       1
#define MAC_UE_STATUS           2
#define MAC_DL_LOG_CH_STATUS    3
#define MAC_UL_LOG_CH_STATUS    4
#define MAC_DL_THROUGHPUT       5
#define MAC_UL_THROUGHPUT       6

#define FAILURE 0
#define SUCCESS 1
#define DOWNLINK 1
#define UPLINK 2
#define GET_PER_UE_STATUS_LENGTH 6


/* RLC OAM APIs */
/* API Ids 51-100 are reserved for OAM */
#define RLC_INIT_LAYER_REQ      51
#define RLC_INIT_LAYER_CNF      52
#define RLC_CLEANUP_LAYER_REQ   53
#define RLC_CLEANUP_LAYER_CNF   54
#define RLC_GET_BUILD_INFO_REQ  55
#define RLC_GET_BUILD_INFO_CNF  56
#define RLC_RESET_STATS_REQ     57
#define RLC_GET_STATS_REQ       58
#define RLC_GET_STATS_CNF       59
#define RLC_GET_STATUS_REQ      60
#define RLC_GET_STATUS_CNF      61
#define RLC_RESET_STATS_CNF     69
#define RLC_SET_LOG_LEVEL_REQ   70
#define RLC_SET_LOG_LEVEL_CNF   71
#define RLC_ENABLE_DISABLE_LOG_REQ 66
/* Tags */
#define RLC_UE_STATS            1
#define RLC_UE_STATUS           2
#define GET_TM_STATS_REQ        3
#define GET_TM_STATS_CNF        4
#define GET_UM_STATS_REQ        5
#define GET_UM_STATS_CNF        6
#define GET_AM_STATS_REQ        7
#define GET_AM_STATS_CNF        8
#define GET_UE_STATS_REQ        9
#define GET_UE_STATS_CNF        10
#define RLC_TX_UM_ENTITY_STATUS 11
#define RLC_RX_UM_ENTITY_STATUS 12
#define RLC_TX_AM_ENTITY_STATUS 13
#define RLC_RX_AM_ENTITY_STATUS 14
#define GET_LOAD_STATS_REQ 	17
#define GET_LOAD_STATS_CNF	18

#define RLC_COMMON_CHANNEL_STATUS_REQ       72
#define RLC_COMMON_CHANNEL_STATUS_CNF       81

#define CCCH_TX_SET              1
#define CCCH_RX_SET              2
#define CCCH_BI_SET		 3

#define BCCH_TX_SET              1
#define PCCH_TX_SET              1



/* RLC Return Types */
#define RLC_SUCCESS             1 
#define RLC_FAILURE             0
#define RLC_PARTIAL_SUCCESS     2

/* Error Codes */
#define RLC_SYNTAX_ERROR                12
#define RLC_LAYER_NOT_INITIALIZED       14
#define RLC_LAYER_ALREADY_INITIALIZED   15
#define RLC_NO_ACTIVE_UE_IN_SYSTEM      16

/* PDCP OAM APIs */
/* API Ids 51-100 are reserved for OAM */
/*
#define PDCP_INIT_LAYER_REQ      51
#define PDCP_INIT_LAYER_CNF      52
#define PDCP_CLEANUP_LAYER_REQ   53
#define PDCP_CLEANUP_LAYER_CNF   54
#define PDCP_GET_STATS_REQ       55
#define PDCP_GET_STATS_CNF       56
#define PDCP_GET_INTEGRITY_STATS_REQ 57
#define PDCP_GET_INTEGRITY_STATS_CNF 58 
#define PDCP_GET_ROHC_STATS_REQ  59
#define PDCP_GET_ROHC_STATS_CNF  60
#define PDCP_GET_CIPHERING_STATS_REQ 61
#define PDCP_GET_CIPHERING_STATS_CNF 62 
#define PDCP_GET_UE_STATS_REQ    63
#define PDCP_GET_UE_STATS_CNF    64
 */
/* If possible we can include pdcpErrors.h  then we can remove following error enums from this file*/ 
 /* General response codes */
  #define PDCP_FAIL               0
 #define PDCP_SUCCESS            1
 #define PDCP_PARTIAL_SUCCESS    2


 /* PDCP errors groups */
 #define PDCP_ERR_SYSTEM_GROUP       0x0100
 #define PDCP_ERR_CONTEXT_GROUP      0x0200
 #define PDCP_ERR_TLV_PARSING_GROUP  0x0300
 #define PDCP_ERR_DATA_TRANSFER      0x0400

/* PDCP general systaem errors */
enum PdcpSystemErrorsT
{
    PDCP_ERR_SYSTEM_MEMORY_ALLOCATION = PDCP_ERR_SYSTEM_GROUP,
    PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER,
    PDCP_ERR_FILE_IO,

    PDCP_ERR_SYSTEM_LAST
};

/* PDCP Context management errors */
enum PdcpContextErrorsT
{
    PDCP_ERR_CONTEXT_NOT_INITIALIZED    = PDCP_ERR_CONTEXT_GROUP,
    PDCP_ERR_CONTEXT_ALREADY_INITIALIZED,
    PDCP_ERR_UE_CONTEXT_ALREADY_CREATED,
    PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED,
    PDCP_ERR_ENTITY_ALREADY_CREATED,
    PDCP_ERR_ENTITY_WRONG_DIRECTION,
    PDCP_ERR_ENTITY_WRONG_TYPE,
    PDCP_ERR_ENTITY_NOT_FOUND,
    PDCP_ERR_ENTITY_SET_SN_SIZE,
    PDCP_ERR_ENTITY_SET_STATUS_REPORT_REQUIRED,

    PDCP_ERR_CONTEXT_LAST
};

/* PDCP TLV parsing errors */
enum PdcpTlvParsingErrorsT
{
    PDCP_ERR_TLV_PARSING_INVALID_LENGTH   = PDCP_ERR_TLV_PARSING_GROUP,
    PDCP_ERR_TLV_PARSING_INVALID_UE_ID,
    PDCP_ERR_TLV_PARSING_INVALID_OPTIONAL_PARAMETERS_LENGTH,
    PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH,

    PDCP_ERR_TLV_PARSING_INVALID_LC_ID,
    PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE,

    PDCP_ERR_TLV_PARSING_INVALID_CRNTI,
    PDCP_ERR_TLV_PARSING_INVALID_TAG_ID,

    PDCP_ERR_TLV_PARSING_INVALID_RNTI_RANGE,
    PDCP_ERR_TLV_PARSING_INVALID_API_ID,

    PDCP_ERR_TLV_PARSING_LAST
};

/* PDCP data transfer fails */
enum PdcpDataTransferErrorsT
{
    PDCP_ERR_DT_INTEGRITY_FAIL = PDCP_ERR_DATA_TRANSFER,
    PDCP_ERR_DT_ROHC_COMPRESS_FAIL,
    PDCP_ERR_DT_RLC_NACK,
    PDCP_ERR_DT_DISCARD_TIMER_EXPIRED,

    PDCP_ERR_DT_LAST
};

 
/* OAM API commands list */
enum OamCmdListT
{
    PDCP_INIT_LAYER_REQ = 0,    /*0*/
    PDCP_CLEANUP_LAYER_REQ,     /*1*/
    PDCP_GET_BUILD_INFO_REQ,    /*2*/
    PDCP_RESET_STATS_REQ,       /*3*/
    PDCP_GET_STATS_REQ,         /*4*/
    PDCP_GET_STATUS_REQ,        /*5*/

    PDCP_OAM_LAST_REQ,
};

/* OAM API responses list */
enum OamResponseCmdListT
{
    PDCP_INIT_LAYER_CNF = 0,        /*0*/
    PDCP_CLEANUP_LAYER_CNF,         /*1*/
    PDCP_GET_BUILD_INFO_CNF,        /*2*/
    PDCP_RESET_STATS_CNF,           /*3*/
    PDCP_GET_STATS_CNF,             /*4*/
    PDCP_GET_STATUS_CNF,            /*5*/

    PDCP_OAM_LAST_CNF,
};

/* OAM API TAGs list for commands */
enum OamCmdTagsListT
{
    PDCP_GET_INTEGRITY_STATS_REQ = 0,        /*0*/
    PDCP_GET_ROHC_STATS_REQ,                 /*1*/
    PDCP_GET_CIPHERING_STATS_REQ,            /*2*/
    PDCP_GET_UE_STATS_REQ,                   /*3*/

    PDCP_OAM_CMD_TAG_LAST,
};

/* OAM API TAGs list for responses */
enum OamResponseTagsListT
{
    PDCP_GET_INTEGRITY_STATS_CNF = 0,        /*0*/
    PDCP_GET_ROHC_STATS_CNF,                 /*1*/
    PDCP_GET_CIPHERING_STATS_CNF,            /*2*/
    PDCP_GET_UE_STATS_CNF,                   /*3*/
    PDCP_UE_STATS,                          /*4*/
    SRB_PDCP_UE_STATS,                       /*5*/
    DRB_PDCP_UE_STATS,                       /*6*/
    PDCP_UE_STATUS,                          /*7*/
    PDCP_SRB_ENTITY_STATUS,                  /*8*/
    PDCP_DRB_ENTITY_STATUS,                  /*9*/

    PDCP_OAM_RESPONSE_TAG_LAST,
};

/* Header Length */
#define API_HEADER_LEN 10





/****************************************************************************
 * Exported Variables
 ****************************************************************************/

typedef struct LCinfo
{
    UInt8   lcId;
    UInt8   lcPriority;
    UInt32  queueLoad;
}LCinfo;

typedef struct ulLCinfo
{
    UInt8   lcId;
    UInt8   lcgId;
    UInt32  bsrNetofLCG;
}ulLCinfo;

typedef struct ULuestatus
{
    UInt8   ulNumofLCs;
    UInt8   ulMaxRB;
    UInt32  bsrNet0;
    UInt32  bsrNet1;
    UInt32  bsrNet2;
    UInt32  bsrNet3;
    UInt8   ulModScheme;
    UInt32  ulCodingRate;
    UInt8   ulMcsIndex;
    /* SPR 15909 fix start */
    UInt64  ulLastScheduleTick;
    /* SPR 15909 fix end */
    ulLCinfo  lcinfo[10];
            
}ULuestatus;

typedef struct DLuestatus
{
    UInt16  crnti;
    UInt8   uePriority;
    UInt8   dlNumofLCs;
    UInt8   dlMaxRB;
    UInt8   dlModScheme;
    UInt32  dlCodingRate;
    UInt8   dlMcsIndex;
    UInt32  queueLoad;
    /* SPR 15909 fix start */
    UInt64  dlLastScheduleTick;
    /* SPR 15909 fix end */
    LCinfo lcinfo[10];
}DLuestatus;

typedef struct macUeStatus 
{
    UInt16 numofActiveUE;
    DLuestatus dluestatus[NUM_OF_UE];
    ULuestatus uluestatus[NUM_OF_UE];
}macUeStatus;

typedef struct amTxUeStatusT
{
    UInt8  lcId;
    UInt8  MaxRetxThreshold;
    UInt8  PollPDU;
    UInt8  IsRetransmitTimerRunning;
    UInt16  PollByte;
    UInt16 VTA;
    UInt16 VTMS;
    UInt16 VTS;
    UInt16 PollSN;
    UInt16 PduWithoutPoll;
    UInt16 ByteWithoutPoll;
    UInt32 TxQueueSize;
    UInt32 ReTxQueueSize;
    UInt32 RxStatusPduSize;
}txamentity;

typedef struct amRxUeStatusT
{
    UInt8  lcId;
    UInt8  SendStatusInd;
    UInt8  IsRTimerRunning;
    UInt8  IsSPTimerRunning;
    UInt16 VRR;
    UInt16 VRMR;
    UInt16 VRX;
    UInt16 VRMS;
    UInt16 VRH;
}rxamentity;
typedef struct RXuestatus
{
    UInt8   lcId;
    UInt16  SnfieldLength;
    UInt16  tReordering;
    UInt16  UR;
    UInt16  UH;
    UInt16  UX;
}rxumentity;

typedef struct TXuestatus
{
    UInt8   lcId;
    UInt16  SnfieldLength;
    UInt16  US;
}txumentity;

typedef struct rlcuestatus
{
    UInt16       crnti;
    UInt8        numRxEntities;
    UInt8        numTxEntities;
    txumentity   TXumentity[MAX_LC];
    rxumentity   RXumentity[MAX_LC];
    txamentity   TXamentity[MAX_LC];
    rxamentity   RXamentity[MAX_LC];
}rlcUeStatus;

typedef struct TMstats
{
UInt32  rxTmdPdu;
UInt32  txTmdPdu;
}TMstats;

typedef struct UMstats
{
    	UInt32   rxUmdSduDiscarded;
    	UInt32   txUmdPdu;
    	UInt32   rxUmdSduTxUL;
    	UInt32   rxIncompleteUmdSduDiscarded;
    	UInt32   rxUmdPdu;
    	UInt32   rxUmdPduDropped;
    	UInt32   sduReceivedFromUl;
	    UInt32   sduTransmitted;
	    UInt32   rxOutofOrderPDU;
}UMstats;

typedef struct AMstats
{
    UInt32 amdPduTransmitted;
    UInt32 amdPduReceived;
    UInt32 amdPduDropped;
}AMstats;

typedef struct uestatscnf
{
	    UInt16  crnti;
	    UInt32  sdurx;
        UInt32  pdutx;
}uestatscnf;

typedef struct UEstats
{
    	UInt16 numofactiveue;
	    uestatscnf uestats_cnf[NUM_OF_UE];
}UEstats;

typedef struct PFstats
{
	    UInt32 txLoadPerfstats;
    	UInt32 rxLoadPerfstats;
}PFstats;

typedef struct rlcstat
{
    UInt32  TMstats;
    UInt32  UMstats;
    UInt32  AMstats;
    UInt32  UEstats;
    UInt32  PFstats;
    TMstats tmstats;
    AMstats amstats;
    UMstats umstats;
    UEstats uestats;
    PFstats pfstats;
}rlcstatistics;

typedef struct ulLogChArr {
    UInt8 lcId;
    UInt8 lcgId;
    UInt16 bsrNetOfLCG;
} ULLogChArr;
                
typedef struct dlLogChArr {
    UInt8 lcId;
    UInt8 lcPriority;
    UInt16 queueLoad;
} DLLogChArr;
 
typedef struct logChstatus {
    UInt8 dlLcCount;
    UInt8 ulLcCount;
    DLLogChArr dlLogChArr[10];
    ULLogChArr ulLogChArr[10];
} LogChstatus;    

typedef struct ConfigReqParamsT {
    UInt8 dlUeQueueLoadWt;
    UInt8 dlLcQueueLoadWt;
    UInt8 dlUeNackQueueLoadWt;
    UInt8 dlMsWt;
    UInt8 dlUePriorityWt;
    UInt8 dlLcPriorityWt;
    UInt8 dlUeDelayWt;
    UInt8 dlLcDelayWt;
    UInt8 dlCQIWt;
    UInt8 numDemuxThreads;
    UInt8 numEncoderThreads;
    UInt8 numMultiplexerThreads;
    UInt8 ulMsWt;
    UInt8 ulUePriorityWt;
    UInt8 ulUeDelayWt;
    UInt8 ulUeBsrLoadWt;
    UInt8 ulRetransType;
    UInt8 ulLcgPriorityWt;
    UInt8 logLevel;
    /* it indicates the type of downlink scheduler Strategy to run i.e. Fair Scehduler Strategy or Round Robin*/
    UInt8 dlSchedulerStrategy;
    /*it indicates the type of Uplink scheduler Strategy to run i.e. Fair Scehduler Strategy or Round Robin*/
    UInt8 ulSchedulerStrategy;
    /*it indicates the maximum number of UEs to be  scehduled in a TTI in downlink */
    UInt8 maxUEsToBeScheduledDL;
    /*it indicates the maximum number of UEs to be  scehduled in a TTI in Uplink */
    UInt8 maxUEsToBeScheduledUL;
    
} ConfigReqParams;


/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* PDCP stats printing functions */
void printIntegrityStats(UInt16 paramIdx,UInt32 paramItem);
void printRohcStats(UInt16 paramIdx,UInt32 paramItem);
void printCipheringStats(UInt16 paramIdx,UInt32 paramItem);
void printSrbStats(UInt8 *data_p,UInt16 *curr);
void printDrbStats(UInt8 *data_p,UInt16 *curr);
void printPdcpStats(UInt8 *data_p,UInt16 *curr,UInt16 msgLen);

UInt8 isNumber(char range[],UInt8 size);
UInt8 getRange(UInt32* r);
#ifndef BUILD_EXE
void initOamSim();
#endif
#endif /* Included LTE_OAM_SIM_H */
