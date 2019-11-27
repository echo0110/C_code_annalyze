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

#ifndef LTE_RLC_OAM_INTF_H
#define LTE_RLC_OAM_INTF_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcOamInterface.h"
#include "lteRlcStat.h"
#include "lteRlcPdcpInterface.h"
#include "lteRlcMacInterface.h"
#include "lteRlcRrcInterface.h"
#include "lteRlcMem.h"
#include "lteRlcUeContext.h"
#include "lteRlcTimer.h"
#include "lteRlcRxProcessor.h"
#include "lteRlcTxProcessor.h"
#include "lteOamInterface.h"
#include "lteCommonStatsManager.h"
/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct RlcStatsReqT {
    UInt32   sz ;
    //RlcStat *statList ;
} RlcStatsReq ;

/* Memory handling changes start */
typedef struct RlcPendingRrcOamMsgNodeT
{
    LTE_SNODE   anchorNode;
    void * msg_p;
    SInt32 msgLenth;
}RlcPendingRrcOamMsgNode;
/* Memory handling changes end */
/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define TM_STATS_CNF_LENGTH             12
#define UM_STATS_CNF_LENGTH            40 
/*RLC Rel 2.0 Changes Start*/
#define AM_STATS_CNF_LENGTH             16 /**/
/*RLC Rel 2.0 Changes End*/
#define LOAD_STATS_CNF_LENGTH		12


#define RLC_UE_STATS_LENGTH             14
#define RLC_TX_UM_ENTITY_STATUS_LENGTH   9
/*RLC Rel 2.0 Changes Start*/
#define RLC_RX_UM_ENTITY_STATUS_LENGTH  15
#define RLC_TX_AM_ENTITY_STATUS_LENGTH  34
#define RLC_RX_AM_ENTITY_STATUS_LENGTH  18
/*RLC Rel 2.0 Changes End*/
#define RLC_GET_BUILD_INFO_REQ_LENGTH   10   
#define RLC_RESET_STATS_REQ_LENGTH      10
#define RLC_GET_STATS_REQ_LENGTH        4        
#define GET_UM_STATS_REQ_LENGTH         4
#define GET_AM_STATS_REQ_LENGTH         4
#define GET_TM_STATS_REQ_LENGTH         4
#define GET_UE_STATS_REQ_LENGTH         4
#define GET_LOAD_STATS_REQ_LENGTH       4
#define RLC_GET_STATUS_REQ_LENGTH       4
#define RLC_UE_STATUS_LENGTH            8 
#define UE_STAT_MANDATORY_LENGTH        8

#define CCCH_TX_SET             1
#define CCCH_RX_SET             2
#define BCCH_TX_SET             1
#define PCCH_TX_SET             1

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
/*RLC Rel 2.0 Changes Start*/
#define RLC_TX_AM_ENTITY_STATUS 13
#define RLC_RX_AM_ENTITY_STATUS 14
/*RLC Rel 2.0 Changes End*/
#define GET_LOAD_STATS_REQ      17
#define GET_LOAD_STATS_CNF	18

/* Error Codes */
#define RLC_LAYER_NOT_INITIALIZED       14
#define RLC_LAYER_ALREADY_INITIALIZED   15
#define RLC_NO_ACTIVE_UE_IN_SYSTEM      16
#define RLC_MODULE_LOGING_DISABLE       13
#define RLC_SYNTAX_ERROR                22


/** Rel 4.0 changes Start **/
#define RLC_GBR_LC  0
#define RLC_NGBR_LC 1
/** Rel 4.0 changes End **/

/* This macro is used for declaring array of variable length */
#ifdef PERF_STATS
#define RLC_MAX_VAL_PERF_TIMER_T 60*60
#define RLC_MIN_VAL_PERF_TIMER_T 1
#define MAX_OAM_PERF_STATS_SIZE 10000 
#define MAX_LC_IDENTITY 8
#endif

/* + Layer2 NON CA changes */
#ifndef UE_SIM_TESTING

#define RLC_UPDATE_AND_GET_INTERNAL_CELL_INDEX(cellId)\
    layer2CommonUpdateCellIndexMapping(cellId)

#define RLC_GET_INTERNAL_CELL_INDEX(cellId)\
    layer2CommonGetInternalCellIndex(cellId)

#define RLC_GET_RRC_CELL_INDEX(cellIndex)\
    layer2CommonGetRRCCellIndex(cellIndex)


#endif
/* - Layer2 NON CA changes */

/****************************************************************************
 * Private Types
 ****************************************************************************/
/*RLC Rel 2.0 Changes Start*/
typedef struct amtxStatusT
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
}amtxStatus;

typedef struct amrxStatusT
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
}amrxStatus;
/*RLC Rel 2.0 Changes End*/

typedef struct txStatusT
{
    UInt8 lcId;
    UInt16 snFieldLength;
    UInt16 US;
}txStatus;

typedef struct rxStatusT
{
    UInt8 lcId;
    UInt16 snFieldLength;
    UInt16 tReordering;
    UInt16 UR;
    UInt16 UH;
    UInt16 UX;
}rxStatus;
    
typedef  struct ueStatsT
{
    UInt16 crnti;
    UInt8 numRxEntities;
    UInt8 numTxEntities;
    txStatus txstatus[MAX_UE_ID];
    rxStatus rxstatus[MAX_UE_ID];
/*RLC Rel 2.0 Changes Start*/
    amtxStatus amtxstatus[MAX_UE_ID];
    amrxStatus amrxstatus[MAX_UE_ID];
/*RLC Rel 2.0 Changes End*/
}UeStats;    


/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UInt16 readPort_g, writePort_g, writeOAMPort_g;
#if defined(ENDC_ENABLED) && defined(PDCP_GTPU_INTF)
extern UInt16 X2uRxPort_g,rlcX2uRxDataPort_g;
#endif


/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#ifdef LTE_EMBMS_SUPPORTED
    UInt32 initEmbmsRLCLayer(void);
    UInt32 deInitEmbmsRLCLayer(void);
#endif
/* + SPR 17439 */
UInt32 initRLCLayer(void) ;

UInt32 cleanUpRlcLayer(void) ;

UInt32 rlcInitDbReq(void);

UInt32 rlcInitStatsReq(void);

UInt32 rlcGetStatsModeReq (void);

UInt32 rlcSetStatsModeReq (UInt8 statMode);

UInt32 rlcGetStatsReq (void);

UInt32 cleaupDataStructures(void);
/* - SPR 17439 */

void deleteQueue(LTE_SQUEUE *q);

void deleteList(LTE_LIST* list);


/* SPR 5599_129119 Fix Start */
/* +- SPR 17777 */
UInt32 handleOamRequests( UInt8* recvBuffer);
/* +- SPR 17777 */
/* SPR 5599_129119 Fix End */
/* + SPR 17439 */
SInt32 rlcDeInitTxThread(void);
SInt32 rlcDeInitTmThread(void);
SInt32 rlcDeInitRxThread(void);
/* - SPR 17439 */
void createRLCLogdetail ( void );
/* + SPR 17439 */
void dumpRlcLatency(void);
/* - SPR 17439 */
#ifdef PERF_STATS
/* + PERF_CA */
void handleRlcOamConfigurePerfStatsReq( UInt8* data_p,
        UInt16 transactionId, 
        UInt8 *respBuff,
        UInt32 sendInd,
	     RrcCellIndex  cellId);
/* - PERF_CA */
/* + stats_review_comments_ipc */
void handleRlcOamGetPerfStatsReq( UInt8* data_p,
        UInt8 *respBuff,
        UInt32 sendInd
#ifndef UE_SIM_TESTING
        ,RrcCellIndex cellId
#endif
        );
/* - stats_review_comments_ipc */
/* + SPR 17439 */
void rlcProcessPerfCounter(void);
void updateRlcPerfContainer(void);
/* - SPR 17439 */
#endif
/*spr 22474 start*/
void handleRlcOamGetDebugInfoReq ( UInt8* recvBuff, UInt16 transactionId, UInt8 *respBuff,UInt32 sendInd
#ifndef UE_SIM_TESTING
			,RrcCellIndex  cellId
#endif
			);
/*spr 22474 end*/

 /** Rel 4.0 Chanegs Start **/

#ifndef UE_SIM_TESTING

/* + Layer2 NON CA changes */
typedef struct RlcSecondaryCellInfoT
{
    RrcCellIndex  cellId; /*Cell index for the secondary cell, value range 0-7 */
    
    /* for enbling/disabling SPS scheduling in MAC */
    UInt8  enableSps;

    UInt16 numOfUEs; /* Maximum Number of supported UE's for this cell of given
                        cellId */

    /* Array containing list of QCIs for which SPS is enabled. Indices
     * corresponds to QCI. Value corr to SPS enabled/disabled for that QCI. */
    UInt8  qciSpslist[MAX_QCI] ;
    /* list of DL SPS intervals supported. By default only 20ms shall be
     * enabled and rest of the values disabled.
     * Values from index 10 to 15 are for future extensions */
    UInt16 supportedDlIntervalList[SPS_INTERVAL_LIST];
    /* list of UL SPS intervals supported. By default only 20ms shall be
     * enabled and rest of the values disabled.
     * Values from index 10 to 15 are for future extensions */
    UInt16 supportedUlIntervalList[SPS_INTERVAL_LIST];
    /* SPS interval profile parameters */

    LP_RlcSpsIntervalProfile rlcSpsIntProfileParams_p[SPS_INTERVAL_LIST];

} *LP_RlcSecondaryCellInfo, RlcSecondaryCellInfo;
/* - Layer2 NON CA changes */

#endif


typedef struct RlcOamPacketDropReqT
{
    UInt16 nodeId; /* 0-> Enb 1-> UE */ 
    UInt8  pktType; /* 0-> Status Packet, 1-> Data Packet */
    UInt32 noOfIteration; /* No. of times a packet should be drop */
    UInt32 startSN; /* in case of Control Packet it will be 0xFFF */
    UInt32 endSN;   /* in case of Control Packet it will be 0xFFF & in case of 1 data packet also*/
    UInt8 qLoadRedFlag; /* Flag to reduce qload at RLC, If 0->1/2 qload for ReTx PDU, 1->1/2 qload for New PDU,others ->Invalid  */
    UInt32 qLoadRedCnt; /* if Flag =0 or 1 , then no. of times need to reduce qload */


}RlcOamPacketDropReq;   
    
// defined for compilation
#ifndef UE_SIM_TESTING
extern RlcQosQciInfo rlcQosQciInfo_g[MAX_QCI];
#endif
#ifdef PERF_STATS
/* + SPR 17439 */
extern void updateRlcOamPerfContainer(void);
/* - SPR 17439 */
#endif

void prepareRlcInitIndication( UInt8 *msg_p, UInt16 *msgLength, 
        InternalCellIndex internalCellIdx );
#endif  /* INCLUDED_OAMINTERFACE_H */
