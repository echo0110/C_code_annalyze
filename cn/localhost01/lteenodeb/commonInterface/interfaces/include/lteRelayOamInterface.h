/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRelayOamInterface.h,v 1.1.1.1 2014/02/01 04:51:36 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains variable declarations for Packet
 *                   : Relay and OAM Interface.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRelayOamInterface.h,v $
 * Revision 1.1.1.1  2014/02/01 04:51:36  cm_intel
 * eNB framework for intel
 *
 ****************************************************************************/

#ifndef LTE_RELAY_OAM_INTERFACE_H
#define LTE_RELAY_OAM_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"

#define RELAY_GTPU_IPv4_HDR       4 
#define RELAY_GTPU_IPv6_HDR       16

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

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/* SPR 13765 fix Start */
// enum for indicating type of error to be used in sending error indication to OAM 
typedef enum ErrorTypeT
{
	S1U_LINK_FAILURE, 
	S1U_LINK_FAILURE_RECOVERED
}ErrorType;
/* SPR 13765 fix END */

/* structure used for Get Timer Request between Relay and OAM Interface*/

typedef struct relayGtpuGetTimerReqTag
{
    UInt8 timerType; /*^ M, 0 ^*/          /* To indicate Timer Type */
}relayGtpuGetTimerReq; /*^ API, LM_GET_TIMER_REQ ^*/

/* structure used for Get Timer Cnf between Relay and OAM Interface*/

typedef struct relayGtpuGetTimerCnfTag
{
    UInt8 result;  /*^ M, 0 ^*/          /* To indicate the result */
    UInt8 timerType;  /*^ M, 0 ^*/       /* To indicate the timer type */
    UInt32 timerDuration; /*^ M, 0 ^*/   /* To indicate timer duration */
}relayGtpuGetTimerCnf; /*^ API, LM_GET_TIMER_CNF ^*/

/* structure used for Set Timer Request between Relay and OAM Interface*/

typedef struct relayGtpuSetTimerReqTag
{
    UInt8 timerType; /*^ M, 0 ^*/       /* To indicate timer type */
    UInt32 timerDuration; /*^ M, 0 ^*/  /* To indicate timer duration */
}relayGtpuSetTimerReq; /*^ API, LM_SET_TIMER_REQ ^*/ 

/* structure used for Set Timer Cnf between Relay and OAM Interface*/

typedef struct relayGtpuSetTimerCnfTag
{
    UInt8 result;  /*^ M, 0 ^*/         /* To indicate result */
    UInt8 timerType; /*^ M, 0 ^*/       /* To indicate the timer type */
}relayGtpuSetTimerCnf; /*^ API, LM_SET_TIMER_CNF ^*/

/* structure used for Max Retires Request between Relay and OAM Interface*/

typedef struct relayGtpuGetMaxRetriesReqTag
{
    UInt8 timerType; /*^ M, 0 ^*/       /* To indicate timer type */
}relayGtpuGetMaxRetriesReq; /*^ API, LM_GET_MAX_RETIRES_REQ ^*/

/* structure used for Max  Retries Cnf between Relay and OAM Interface*/

typedef struct relayGtpuGetMaxRetriesCnfTag
{
    UInt8 result;  /*^ M, 0 ^*/         /* To indicate result */ 
    UInt8 timerType;  /*^ M, 0 ^*/      /* To indicate timer type */
    UInt16 maxNumOfRetries; /*^ M, 0 ^*/ /* To indicate maxinum no of retries */
}relayGtpuGetMaxRetriesCnf; /*^ API, LM_GET_MAX_RETIRES_CNF ^*/

/* structure used for Set Max Retries Req between Relay and OAM Interface*/

typedef struct relayGtpuSetMaxRetriesReqTag
{
    UInt8 timerType; /*^ M, 0 ^*/       /* To indicate timer type */
    UInt16 maxNumOfRetries; /*^ M, 0 ^*/ /* To indicate maximum no of retries */
}relayGtpuSetMaxRetriesReq; /*^ API, LM_SET_MAX_RETIRES_REQ ^*/

/* structure used for Set Max Retries Cnf between Relay and OAM Interface*/
typedef struct relayGtpuSetMaxRetriesCnfTag
{
    UInt8 result; /*^ M, 0 ^*/          /* To indicate result */
    UInt8 timerType; /*^ M, 0 ^*/      /* To indicate timer type */
}relayGtpuSetMaxRetriesCnf; /*^ API, LM_SET_MAX_RETIRES_CNF ^*/

/* structure used for Set Trace Level Req between Relay and OAM Interface*/

typedef struct relaySetTraceLvlReqTag
{
    UInt8 traceFlag; /*^ M, 0 ^*/      /* To indicate trace flag */
    UInt8 traceLevel;  /*^ M, 0 ^*/    /* To indicate trace level */
}relaySetTraceLvlReq; /*^ API, LM_SET_TRACE_LEVEL_REQ ^*/

/* structure used for Set Trace Level Cnf between Relay and OAM Interface*/

typedef struct relaySetTraceLvlCnfTag
{
    UInt8 result; /*^ M, 0 ^*/         /* To indicate result */
    UInt8 traceFlag; /*^ M, 0 ^*/      /* To indicate trace flag */
}relaySetTraceLvlCnf; /*^ API, LM_SET_TRACE_LEVEL_CNF ^*/

/* structure used for Get Trace Level Req between Relay and OAM Interface*/

typedef struct relayGetTraceLvlReqTag
{
    UInt8 traceFlag; /*^ M, 0 ^*/      /* To indicate trace flag */
}relayGetTraceLvlReq; /*^ API, LM_GET_TRACE_LEVEL_REQ ^*/

/* structure used for Get Trace Level Cnf between Relay and OAM Interface*/

typedef struct relayGetTraceLvlCnfTag
{ 
    UInt8 result;  /*^ M, 0 ^*/        /* To indicate result */
    UInt8 traceFlag;   /*^ M, 0 ^*/    /* To indicate trace flag */
    UInt8 traceLevel; /*^ M, 0 ^*/     /* To indicate trace level */
}relayGetTraceLvlCnf; /*^ API, LM_GET_TRACE_LEVEL_CNF ^*/ 

/* structure used for Init Trace Level Cnf between Relay and OAM Interface*/

typedef struct relayInitTraceLvlCnfTag
{
    UInt8 result; /*^ M, 0 ^*/          /* To indicate result */
}relayInitTraceLvlCnf; /*^ API, LM_INIT_TRACE_LEVEL_CNF ^*/

/* This structure is used for lv Format */

typedef struct lvHeaderTag
{ 
    UInt16 length; /*^ M, 0 ^*/
    UInt8 *value; /*^ M, 0 ^*/
}lvHeader;

/* This struture defines RAB Context Database Size */
typedef struct numRabRecTag
{
    UInt32 initSize; /*^ M, 0 ^*/
    UInt32 stepSize; /*^ M, 0 ^*/
    UInt32 maxSize; /*^ M, 0 ^*/
}numRabRec;
#ifdef ENDC_ENABLED
/*Dual Connectivity SeNB changes*/
typedef struct x2uDeliveryStatusTriggerThresholdTag
{
    /*timer for triggering the DL data delivery status 
      indication to MeNB*/
    UInt16 x2uDeliveryStatusTriggerTimer; 
	/*threshold count used for triggering DL data delivery 
	   status indication to MeNB*/
    UInt16 x2uDeliveryStatusTriggerPktCnt; 
	/*threshold data size used for triggering DL data delivery
	  status indication to MeNB*/
    UInt16 x2uDeliveryStatusTriggerPktSize; 
}x2uDeliveryStatusTriggerThreshold;
#endif
/* This structure is used for Init Database Request */
typedef struct relayGtpuInitDBReqTag
{
    numRabRec noRabRec; /*^ M, 0 ^*/          /* To indicate Rab Context DB Size (Mandatory)*/
    UInt16 noPeerRec; /*^ M, 0 ^*/            /* To indicate Peer Context DB Size (mandatory)*/
    UInt8  logLevel; /*^ M, 0 ^*/
    UInt8 numMyAddr4; /*^ M, 0 ^*/
    UInt8 numMyAddr6; /*^ M, 0 ^*/
    /*IPV6_CHANGE - START*/
    /* Either any one of IPv4 and IPv6 or both should be present */
    lvHeader myAddr4; /*^ M, 0 ^*/             /* To indicate RNC IPv4 Address (Mandatory) */
    lvHeader myAddr6; /*^ M, 0 ^*/             /* To indicate RNC IPv6 Address (Mandatory) */
    /*IPV6_CHANGE - END*/
    /*+SPR #3456*/
    /* SPR 10597 Changes Start */
    UInt32  echoReqTimer; /*^ M, 0 ^*/         /* ECHO REQ TIMER(>60sec)*/
    /* SPR 10597 Changes End */
    UInt8  maxRetry; /*^ M, 0 ^*/             /*Max retry before declaring PATH FAILURE*/
    /*-SPR #3456*/
    /*HSM_CHG_START*/
    UInt16 healthMonitoringTimeInterval; /*^ M, 0 ^*/
    /*HSM_CHG_END*/
    UInt8 maxMspToBeBuffered;
#ifdef ENDC_ENABLED	
	UInt8 isGtpuNRHeaderEnabled;
	/*Dual Connectivity SeNB changes*/
	/*Indicates the time interval for which X2U will retain bearer context 
	  while data forwarding is going on it, applicable for DC UEs only*/
	UInt16 dataForwardingGuardTimerInterval;
	
	/*specifies configurable threshold parameters used for triggering DL Data 
	  Delivery Status report towards MeNB*/
	x2uDeliveryStatusTriggerThreshold x2uDelStatusThreshold;
#endif	
}relayGtpuInitDBReq; /*^ API, LM_INITDB_REQ ^*/

/* SPR 2030 fix Start */
/* Structure definition for enabling logs in Packet Relay + eGTPu*/

/****************************************************************************
 * PR_GTPU_SET_LOG_LEVEL_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct relayGtpuSetLogLevReqTag
{
    UInt32 logLevel; /*^ M, 0 ^*/
}relayGtpuSetLogLevReq; /*^ API, PR_GTPU_SET_LOG_LEVEL_REQ ^*/

/****************************************************************************
 * PR_GTPU_GET_LOG_LEVEL_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
/* No Payload Required */

/****************************************************************************
 * PR_GTPU_GET_LOG_LEVEL_RESP API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct relayGtpuGetLogLevRespTag
{
    UInt32 logLevel; /*^ M, 0 ^*/
}relayGtpuGetLogLevResp;  /*^ API, PR_GTPU_GET_LOG_LEVEL_RESP ^*/

/******************************************************************************
 *  PR_GTPU_ENABLE_LOG_CATEGORY_REQ / PR_GTPU_DISABLE_LOG_CATEGORY_REQ
 ******************************************************************************/
typedef struct relayGtpuLogCategoryReqTag
{
    UInt64 logCategory; /*^ M, 0 ^*/
}relayGtpuLogCategoryReq;  /*^ API, PR_GTPU_ENABLE_LOG_CATEGORY_REQ ^*/

/****************************************************************************
 * PR_GTPU_GET_LOG_CATEGORY_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
/* No Payload Required */

/****************************************************************************
 * PR_GTPU_GET_LOG_CATEGORY_RESP API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct relayGtpuGetLogCategoryRespTag
{
    UInt64      logCategory; /*^ M, 0 ^*/
} relayGtpuGetLogCategoryResp; /*^ API, PR_GTPU_GET_LOG_CATEGORY_RESP ^*/
/* SPR 2030 fix End */

/* SPR 7085 fix start */
#define RELAY_STATS_RESET 0
#define RELAY_STATS_NO_RESET 1
#define RELAY_MAX_STATS_TYPE 4 // EGTPU_STATS_INTERNAL in stack

/****************************************************************************
 * PR_GTPU_GET_STATS_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct relayGtpuGetStatsReqTag
{
    UInt8 statsType; /*^ M, 0 ^*/
    UInt8 resetFlag; /*^ M, 0 ^*/
}relayGtpuGetStatsReq; /*^ API, PR_GTPU_GET_STATS_REQ ^*/

/****************************************************************************
 * PR_GTPU_GET_STATS_RESP API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct relayGtpuGetStatsRespTag
{
    UInt8 result; /*^ M, 0 ^*/
    UInt8 statsType; /*^ M, 0 ^*/
    UInt32 stats[]; /*^ M, 0 ^*/
}relayGtpuGetStatsResp; /*^ API, PR_GTPU_GET_STATS_RESP ^*/

/* SPR 7085 fix End */

/* SPR 13765 fix Start */
/****************************************************************************
 * PR_GTPU_ERROR_IND API - Mandatory parameters structure definition
 ****************************************************************************/
typedef struct relayGtpuErrorIndMsgTag
{
	UInt32 typeOfError;      /*^ M, 0 ^*/ 
 	UInt8 errStr[80];        /*^ M, 0 ^*/
}relayGtpuErrorIndMsg;       /*^ API, PR_GTPU_ERROR_IND ^*/
/* SPR 13765 fix END */
/*spr 22474 start*/
/* OAM_GTPU_GET_DEBUG_INFO_RESP */
typedef struct GtpuGetDebugInfoReqParamsT {
    /* This field informs the type of debugging information needed */
    UInt16 debugType; /*^ M, 0 ^*/ /* Check Enum GetDebugType */
}GtpuGetDebugInfoReqParams;

/* GTPU_OAM_GET_DEBUG_INFO_RESP */
typedef struct GtpuGetDebugInfoRespParamsT {
    UInt16 responseCode; /*^ M, 0 ^*/   // SUCCESS or an Error Code
    /* This field informs the type of debugging information sent */
    UInt16 debugType; /*^ M, 0 ^*/ /* Check Enum GetDebugType */
    union {
         L2MemPoolStats csplMemPoolStats;
         L2MsgPoolStats csplMsgPoolStats;
    }u;
}GtpuGetDebugInfoRespParams;

/*spr 22474 end*/

#endif
