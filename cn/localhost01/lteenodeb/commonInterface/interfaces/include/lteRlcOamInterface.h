/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcOamInterface.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:46 gur20491 Exp $
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
 * $Log: lteRlcOamInterface.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:46  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:07  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.20.16.4  2010/08/03 12:19:51  gur21148
 * UM_STATS_CNF_LENGTH
 *
 * Revision 1.20.16.3  2010/07/17 12:52:54  gur21148
 * RLC_UM_MERGES
 *
 * Revision 1.20.16.2  2010/07/17 11:36:35  gur23054
 * Review Comments fixed
 *
 * Revision 1.20.16.1  2010/06/17 10:59:45  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.20  2009/04/15 16:04:22  gur19140
 * rlc latency + logging related changes
 *
 * Revision 1.19  2009/04/09 17:23:38  gur20548
 * RESET Stats API name changed
 *
 * Revision 1.18  2009/04/09 11:17:34  gur20548
 * Added macro for TM status req/cnf
 *
 * Revision 1.17  2009/04/08 16:58:33  gur20548
 * Added macros for log level cnf
 *
 * Revision 1.16  2009/04/07 16:04:43  gur20548
 * update in rrc and oam
 *
 * Revision 1.15  2009/04/07 15:02:16  gur19140
 * peer to peer fixes
 *
 * Revision 1.14  2009/04/07 06:28:51  gur19140
 * um length changes
 *
 * Revision 1.13  2009/04/03 13:35:21  gur20439
 * Macro added to support log level for RLC.
 *
 * Revision 1.12  2009/04/02 14:27:51  gur19140
 * changes for intg and peer to peer testing
 *
 * Revision 1.11  2009/03/27 14:33:54  gur19140
 * Initgration testing related changes
 *
 * Revision 1.10  2009/03/25 07:39:39  gur20439
 * new function added to create RLC Log module context.
 *
 * Revision 1.10  2009/03/26 11:41:17  gur20439
 * function added to create RLC log Module detail
 *
 * Revision 1.9  2009/03/23 08:37:17  gur19140
 * Integration changes first waves
 *
 * Revision 1.8  2009/03/02 10:12:08  gur19836
 * Compilation warnings removed
 *
 * Revision 1.7  2009/03/02 09:10:54  gur19140
 * Changes reqarding Zero copy,pdcp discard and execution legs.
 *
 * Revision 1.6  2009/03/02 05:34:08  gur19836
 * Updated for OAM Enhancement
 *
 * Revision 1.5  2009/02/10 07:23:21  gur19836
 * UT Defects Fixed
 *
 * Revision 1.4  2009/02/05 11:01:01  gur19836
 * CR comments fixed
 *
 * Revision 1.3  2009/02/05 09:30:23  gur19140
 * *** empty log message ***
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_RLC_OAM_INTERFACE_H
#define LTE_RLC_OAM_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define RLC_VAR_SIZE(x) 1

/* RLC OAM APIs */
/* API Ids 51-100 are reserved for OAM */

#define RLC_COMMON_CHANNEL_STATUS_REQ       72
#define RLC_COMMON_CHANNEL_STATUS_CNF       81

/* Error Codes */
#define RLC_LAYER_NOT_INITIALIZED       14
#define RLC_LAYER_ALREADY_INITIALIZED   15
#define RLC_NO_ACTIVE_UE_IN_SYSTEM      16

#ifndef LINUX_PC_TEST //the same as the value of rrc_rlc_error_code_et in rrc_rlc_intf.h 
#define RLC_SYNTAX_ERROR                22
#endif
#define NB_CELL_SRB0_SRB1_LC_ID   1
#define NB_CELL_SRBbis_LC_ID   3

/** Rel 4.0 changes Start **/
#define MAX_NUM_DRB 9
/** Rel 4.0 changes End **/
/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * RLC_INIT_LAYER_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
 /** Rel 4.0 Changes Start **/

#ifndef UE_SIM_TESTING
typedef enum LogicalChannelTypeT {
    RLC_GBR = 0,
    RLC_NGBR 
}rlcLogicalChannelType;

typedef struct RlcQosQciInfoT{
    rlcLogicalChannelType eResType; /*^ M, 0 ^*/   /** Resource type of the logical channel GBR/NGBR **/
    UInt16      pktDlyBdgt; /*^ M, 0 ^*/ /** packet delay bydget of the logical channel **/
    UInt8       qci;  /*^ M, 0 ^*/      /** QCI of the logical channel **/
    UInt8       priority;  /*^ M, 0 ^*/ /** Priority of the logical channel **/
    UInt8       pktErrLossRate; /*^ M, 0 ^*/ /** packet loss rate  **/ 
    /* For byte alignment */
    UInt8  pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}RlcQosQciInfo;

/* Following structure defines the SPS interval profile parameters */
typedef struct RlcSpsIntervalProfileT
{
    /* When enabled, segmentation and concatenation of RLC SDU is not allowed
     * for SPS LC when being transmitted on SPS occasion */
    UInt8 enableSegConcatSpsRlcSdu; /*^ M, 0 ^*/
    /* When packets accumulate in DL RLC SDU Q, then following options are
     * supported -
     * 1. Multiple SDUs scheduled in bulk using dynamic grant
     * 2. occasionally drop SDUs from head of the Q
     */
    UInt8 dlJitterHandlingOption; /*^ M, 0 ^*/
} *LP_RlcSpsIntervalProfile, RlcSpsIntervalProfile ;
/** Rel 4.0 changes End **/

/* + Layer2 NON CA changes */
typedef struct RlcSCellInfoT
{
    RrcCellIndex  cellId;/*^ M, 0 ^*/ /* cell index for the Secondary Cell, value range 0-7*/

    /* for enbling/disabling SPS scheduling in MAC */
    UInt8  enableSps; /*^ M, 0 ^*/

    UInt16 numOfUEs; /*^ M, 0 ^*/   /* Maximum number of supported UEs for this cell 
                                       of given cellId*/

    /* list of DL SPS intervals supported. By default only 20ms shall be
     * enabled and rest of the values disabled.
     * Values from index 10 to 15 are for future extensions */
    UInt16 supportedDlIntervalList[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* list of UL SPS intervals supported. By default only 20ms shall be
     * enabled and rest of the values disabled.
     * Values from index 10 to 15 are for future extensions */
    UInt16 supportedUlIntervalList[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* SPS interval profile parameters */
    RlcSpsIntervalProfile rlcSpsIntervalProfileParams[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* Array containing list of QCIs for which SPS is enabled. Indices
     * corresponds to QCI. Value corr to SPS enabled/disabled for that QCI. */
    UInt8  qciSpslist[MAX_QCI] ; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* For byte alignment */
    UInt8  pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
} RlcSCellInfo;
/* - Layer2 NON CA changes */

#endif

typedef struct RlcOamInitLayerReqT
{
    UInt16   numOfUEs; /*^ M, 0 ^*/   /* Maximum number of supported UEs for cell whose 
                                cellId is in header*/
    UInt16   rntiStartRange; /*^ M, 0 ^*/ /* the starting value of rnti to be supported
                                   for ueId */
    UInt16   rntiEndRange; /*^ M, 0 ^*/  /* the maximum rnti value to be supported 
                                   for ueId */
    UInt8    logLevel; /*^ M, 0 ^*/
    UInt8    sCellCount; /*^ M, 0 ^*/ /*Number of secondary cells value range 0-7*/
#ifndef UE_SIM_TESTING
    UInt32   qSizeFactorGBR; /*^ M, 0 ^*/
    UInt32   qSizeFactorNGBR; /*^ M, 0 ^*/
    UInt16   healthMonitoringTimeInterval; /*^ M, 0 ^*/
    /* for enbling/disabling SPS scheduling in MAC */
    UInt8  enableSps; /*^ M, 0 ^*/
    /* Array containing list of QCIs for which SPS is enabled. Indices
     * corresponds to QCI. Value corr to SPS enabled/disabled for that QCI. */
    UInt8  qciSpslist[MAX_QCI] ; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* MAX_INTERNAL_CELL_INDEX is number of total cells - 1, as this IE includes
     * info for second cell onwards */
    /* Array containning serving cell Info */
    RlcSCellInfo  sCellInfo[MAX_INTERNAL_CELL_INDEX]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    RlcQosQciInfo  rlcQosQciInfo[MAX_QCI]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* list of DL SPS intervals supported. By default only 20ms shall be
     * enabled and rest of the values disabled.
     * Values from index 10 to 15 are for future extensions */
    UInt16 supportedDlIntervalList[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* list of UL SPS intervals supported. By default only 20ms shall be
     * enabled and rest of the values disabled.
     * Values from index 10 to 15 are for future extensions */
    UInt16 supportedUlIntervalList[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* SPS interval profile parameters */
    RlcSpsIntervalProfile rlcSpsIntervalProfileParams[SPS_INTERVAL_LIST]; /*^ M, 0, OCTET_STRING, FIXED ^*/

#endif
} RlcOamInitLayerReq, *LP_RlcOamInitLayerReq; /*^ API, RLC_INIT_LAYER_REQ ^*/

/****************************************************************************
 * RLC_INIT_LAYER_CNF,
 * RLC_SET_LOG_LEVEL_CNF,
 * RLC_RESET_STATS_CNF,
 * RLC_SET_LOG_CATEGORY_RESP - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct RlcOamCnfT
{
    UInt16      responseCode; /*^ M, 0 ^*/   /* Response code */
} RlcOamCnf, *LP_RlcOamCnf; /*^ API, RLC_INIT_LAYER_CNF ^*/

/****************************************************************************
 * RLC_SET_LOG_LEVEL_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct RlcOamSetLogLevelReqT
{
    UInt32       logLevel; /*^ M, 0 ^*/
} RlcOamSetLogLevelReq, *LP_RlcOamSetLogLevelReq; /*^ API, RLC_SET_LOG_LEVEL_REQ ^*/

/****************************************************************************
 * RLC_GET_LOG_LEVEL_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
/* No Payload Required */
/****************************************************************************
 * RLC_GET_LOG_LEVEL_RESP API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct RlcOamGetLogLevelRespT
{
    UInt32       logLevel; /*^ M, 0 ^*/
} RlcOamGetLogLevelResp, *LP_RlcOamGetLogLevelResp; /*^ API, RLC_GET_LOG_LEVEL_RESP ^*/
/******************************************************************************
 *  RLC_ENABLE_LOG_CATEGORY_REQ
 ******************************************************************************/
typedef struct RlcOamEnableLogCategoryReqT
{
    UInt64 logCategory; /*^ M, 0 ^*/
} RlcOamEnableLogCategoryReq;  /*^ API, RLC_ENABLE_LOG_CATEGORY_REQ ^*/

/******************************************************************************
 *  RLC_DISABLE_LOG_CATEGORY_REQ
 ******************************************************************************/
typedef struct RlcOamDisableLogCategoryReqT
{
    UInt64 logCategory; /*^ M, 0 ^*/
} RlcOamDisableLogCategoryReq; /*^ API, RLC_DISABLE_LOG_CATEGORY_REQ ^*/

/****************************************************************************
 * RLC_GET_LOG_CATEGORY_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
/* No Payload Required */

/****************************************************************************
 * RLC_GET_LOG_CATEGORY_RESP API - Mandatory parameters structure definition 
 ****************************************************************************/
typedef struct RlcOamGetLogCategoryRespT
{
    UInt64      logCategory; /*^ M, 0 ^*/
} RlcOamGetLogCategoryResp, *LP_RlcOamGetLogCategoryResp; /*^ API, RLC_GET_LOG_CATEGORY_RESP ^*/

typedef struct lteRlcTMStatInfoT {
UInt32 rxTmdPdu; /*^ M, 0 ^*/
UInt32 txTmdPdu; /*^ M, 0 ^*/
}lteRlcTMStatInfo;

typedef struct lteRlcUMStatInfoT {
UInt32 rxUmdSduDiscarded; /*^ M, 0 ^*/
UInt32 txUmdPdu; /*^ M, 0 ^*/
UInt32 rxUmdSduTxUL; /*^ M, 0 ^*/
UInt32 rxIncompleteUmdSduDiscarded; /*^ M, 0 ^*/
UInt32 rxUmdPdu; /*^ M, 0 ^*/
UInt32 rxUmdPduDropped; /*^ M, 0 ^*/
UInt32 umdSduReceivedFromPdcp; /*^ M, 0 ^*/
UInt32 umdSduTransmittedToMac; /*^ M, 0 ^*/
}lteRlcUMStatInfo;

typedef struct lteRlcAMStatInfoT {
UInt32 amdPduTransmitted; /*^ M, 0 ^*/
UInt32 amdPduReceived ; /*^ M, 0 ^*/
UInt32 amdPduDropped; /*^ M, 0 ^*/
UInt32 amdIncompSduDiscard; /*^ M, 0 ^*/ /* new Added */
UInt32 amdSduReceivedFromPdcp; /*^ M, 0 ^*/
UInt32 amdSduTransmittedToPdcp; /*^ M, 0 ^*/
UInt32 amdSduDropped; /*^ M, 0 ^*/
UInt32 amdSduTransmitedToMac; /*^ M, 0 ^*/
}lteRlcAMStatInfo;

typedef struct lteRlcUEStatsT {
UInt16 crnti; /*^ M, 0 ^*/
UInt32 sduReceivedFromPdcp; /*^ M, 0 ^*/
UInt32 pduTransmittedToMac; /*^ M, 0 ^*/
UInt32 sduTransmittedToPdcp; /*^ M, 0 ^*/
UInt32 pduReceivedFromMac; /*^ M, 0 ^*/
}lteRlcUEStats;

/**************************************************************************
 * * RLC GET STATS CNF response message Structure
 * ***************************************************************************/

typedef struct lteRlcUEStatCnfT {
UInt16 numOfActiveUE; /*^ M, 0 ^*/
lteRlcUEStats rlcUeStats[RLC_VAR_SIZE(numOfActiveUE)]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}lteRlcUEStatCnf;


typedef struct lteRlcGetStatCnfT {
UInt16 responseCode; /*^ M, 0 ^*/
lteRlcTMStatInfo  lteTMStat; /*^ M, 0 ^*/
lteRlcUMStatInfo  lteUMStat; /*^ M, 0 ^*/
lteRlcAMStatInfo  lteAMStat; /*^ M, 0 ^*/
lteRlcUEStatCnf   lteUEStat; /*^ M, 0 ^*/
}lteRlcGetStatCnf; /*^ API,  RLC_GET_STATS_CNF ^*/


/**************************************************************************
 RLC GET UE STATUS REQ messages Structures
 **** Request send from OAM to RLC for UE Status ***
***************************************************************************/

typedef struct lteRlcUEStatusReqT {
UInt16 ueIndex; /*^ M, 0 ^*/
UInt8  isUEIndexPresent; /*^ M, 0 ^*/
}lteRlcUEStatusReq; /*^ API, RLC_GET_STATUS_REQ ^*/

typedef struct lteRlcTxUMEntityStatusT {
UInt16 snFieldLength; /*^ M, 0 ^*/
UInt16 vtUS; /*^ M, 0 ^*/
UInt8 lcId; /*^ M, 0 ^*/
/* For byte alignment */
UInt8  pading[3]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}lteRlcTxUMEntityStatus;

typedef struct lteRlcRxUMEntityStatusT {
UInt16 snFieldLength; /*^ M, 0 ^*/
UInt16 tReordering; /*^ M, 0 ^*/
UInt16 vrUR; /*^ M, 0 ^*/
UInt16 vrUH; /*^ M, 0 ^*/
UInt16 vrUX; /*^ M, 0 ^*/
UInt8  lcId; /*^ M, 0 ^*/
/* For byte alignment */
UInt8  pading; /*^ M, 0 ^*/
}lteRlcRxUMEntityStatus;

typedef struct lteRlcTxAMEntityStatusT {
UInt16 vtA; /*^ M, 0 ^*/
UInt16 vtMS; /*^ M, 0 ^*/
UInt16 vtS; /*^ M, 0 ^*/
UInt16 pollSN; /*^ M, 0 ^*/
UInt16 pduWithoutPoll; /*^ M, 0 ^*/
UInt16 maxRetxThreshold; /*^ M, 0 ^*/
UInt32 pollPDU; /*^ M, 0 ^*/
UInt32 pollByte; /*^ M, 0 ^*/
UInt32 txQueueSize; /*^ M, 0 ^*/
UInt32 reTxQueueSize; /*^ M, 0 ^*/
UInt32 rxStatusPduSize; /*^ M, 0 ^*/
UInt8  lcId; /*^ M, 0 ^*/
UInt8  istRetransmitRunning; /*^ M, 0 ^*/
/* For byte alignment */
UInt8  pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}lteRlcTxAMEntityStatus; 

typedef struct lteRlcRxAMEntityStatusT {
UInt8 lcId; /*^ M, 0 ^*/
UInt8 sendStatusInd; /*^ M, 0 ^*/
UInt16 vrR; /*^ M, 0 ^*/
UInt16 vrMR; /*^ M, 0 ^*/
UInt16 vrX; /*^ M, 0 ^*/
UInt16 vrMS; /*^ M, 0 ^*/
UInt16 vrH; /*^ M, 0 ^*/
UInt8 isRTimerRunning; /*^ M, 0 ^*/
UInt8 isSPTimerRunning; /*^ M, 0 ^*/
/* For byte alignment */
UInt8  pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}lteRlcRxAMEntityStatus;

typedef struct lteRlcUEStatusT {
UInt16 crnti; /*^ M, 0 ^*/
UInt8 numOfRxAMEntities; /*^ M, 0, READ ^*/
UInt8 numOfTxAMEntities; /*^ M, 0, READ ^*/
UInt8 numOfRxUMEntities; /*^ M, 0, READ ^*/
UInt8 numOfTxUMEntities; /*^ M, 0, READ ^*/
/* For byte alignment */
UInt8  pading[2]; /*^ M, 0, OCTET_STRING, FIXED ^*/
lteRlcTxUMEntityStatus rlcTxUMEntityStatus[MAX_NUM_DRB]; /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfTxUMEntities ^*/
lteRlcRxUMEntityStatus rlcRxUMEntityStatus[MAX_NUM_DRB]; /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfRxUMEntities ^*/
lteRlcTxAMEntityStatus rlcTxAMEntityStatus[MAX_NUM_DRB]; /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfTxAMEntities ^*/
lteRlcRxAMEntityStatus rlcRxAMEntityStatus[MAX_NUM_DRB]; /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numOfRxAMEntities ^*/
}lteRlcUEStatus;

/**************************************************************************
 RLC GET UE STATUS CNF messages Structures
 **** Resp Send from RLC to OAM for UE Status ***
*************************************************************************/

typedef struct lteRlcUEStatusCnfT {
UInt16 responseCode; /*^ M, 0 ^*/
/** SPR 2166 Changes Start **/
UInt16 numOfUE; /*^ M, 0 ^*/
lteRlcUEStatus rlcUEStatus[RLC_VAR_SIZE(numOfUE)]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/
/** SPR 2166 Changes End **/
}lteRlcUEStatusCnf; /*^ API, RLC_GET_STATUS_CNF ^*/
/*spr 22474 start*/
/* RLC_OAM_GET_DEBUG_INFO_RESP */
typedef struct RlcGetDebugInfoReqParamsT {
    /* This field informs the type of debugging information needed */
    UInt16 debugType; /*^ M, 0 ^*/ /* Check Enum GetDebugType */
}RlcGetDebugInfoReqParams;

/* RLC_OAM_GET_DEBUG_INFO_RESP */
typedef struct RlcGetDebugInfoRespParamsT {
    UInt16 responseCode; /*^ M, 0 ^*/   // SUCCESS or an Error Code
    /* This field informs the type of debugging information sent */
    UInt16 debugType; /*^ M, 0 ^*/ /* Check Enum GetDebugType */
    union {
         L2MemPoolStats csplMemPoolStats;
         L2MsgPoolStats csplMsgPoolStats;
    }u;
}RlcGetDebugInfoRespParams;

/*spr 22474 end*/

#endif
