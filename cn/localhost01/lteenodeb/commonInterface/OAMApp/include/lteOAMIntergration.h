/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteOAMIntergration.h,v 1.1.1.1.16.1 2010/10/25 09:04:23 gur21010 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file provide APIs to RRC Layer 
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteOAMIntergration.h,v $
 * Revision 1.1.1.1.16.1  2010/10/25 09:04:23  gur21010
 * Warnings Removed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.6.2.3  2009/12/05 13:15:17  gur19479
 * updated for thread binding
 *
 * Revision 1.1.2.6.2.2  2009/09/30 12:56:32  gur20439
 * file modified to support seperate OAM and RRC IP
 *
 * Revision 1.1.2.6.2.1  2009/09/11 12:48:38  gur12140
 * modified to build with MAC 1.2
 *
 * Revision 1.1.2.6  2009/08/06 13:25:58  gur19836
 * Graceful Exit code added
 *
 * Revision 1.1.2.5  2009/07/16 05:04:01  gur19140
 * tick related problem resolve
 *
 * Revision 1.1.2.4  2009/07/09 02:46:55  gur19479
 * updated for port and Ip
 *
 * Revision 1.1.2.3  2009/07/07 11:31:33  gur20435
 *
 * Resolved merge conflicts
 *
 * Revision 1.1.2.2  2009/06/30 22:47:52  gur19479
 * updated for integration
 *
 * Revision 1.1.2.1  2009/06/09 12:04:18  gur19140
 * first wave of Integration
 *
 ****************************************************************************/


#ifndef OAM_INTEGRATION_H
#define OAM_INTEGRATION_H

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"
#include "lteLog.h"
#include "lteMemPool.h"
#include "lteSQueue.h"
#include "lteRlcOamIntf.h"
#include "lteMsgPool.h"
#include "lteMemPool.h"
#include "lteMisc.h"
#include "lteLayer2CommanTypes.h"
#include "lteMacTypes.h"
#include "lteMacULUEContext.h"
#include "lteCommonStatsManager.h"
#include "ltePerfStats.h"
/****************************************************************************
 * Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/
#define CLEANUP_LAYER2        0x2FFF
/* L2 Porting Start */
#define RLC_MAX_REQ_DATASIZE  1000
#define FAPI_CTRL_MSG_MAX_BUF_SIZE  2000
/* L2 Porting End */
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#define OAM_SUCCESS 1
#define OAM_FAILURE 0
/* + SPR 17439 */
extern UInt32 cleanUpRlcLayer(void);
extern UInt32 cleanUpPDCPLayer (void);
extern UInt32 initPDCPLayer (void);
extern void setPdcpCommPort(UInt16 flag,UInt16 pdcpRxPort,UInt16 oamTxPort,UInt16 rrcTxPort,Char8 *oam_Ip, Char8 *rrc_Ip
        ,Char8 *pdcpIp,Char8 *rrmIp_p, UInt16 rrmTxPort
        /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF 
        ,Char8 *gtpuIp_p, UInt16 gtpuRxPort, UInt16 pdcpGtpuRxPort
#endif
        /*SPR 21188 -*/
        );
/*SPR 20863 FIX*/
extern UInt8 initMAC(void);
extern UInt8 cleanupMAC(void);
void cleanUpPdcpUser(void);
Int InitPdcpUser(void);
extern void getUplkThroughPut(void);
extern void getDwlkThroughPut(void);
/* - SPR 17439 */
/* CA Stats Changes Start */
#ifdef DL_UL_SPLIT
void lteMacUpdateStatsPerfEnd(UInt32, InternalCellIndex);
#else
void lteMacUpdateStatsPerfEnd(InternalCellIndex);
#endif
/* CA Stats Changes End */
/* + SPR 17439 */
UInt32 initStats(void);
#ifdef PERF_STATS
UInt32 initPerfStats(void);
/* - SPR 17439 */
#endif

/* Layer2 Init Ind Changes Starts */
extern UInt8 rlcInitInd_g;
extern UInt8 pdcpInitInd_g;

/*SPR 17585 +*/
extern UInt8 s1UInitInd_g;
/*SPR 17585 -*/

/*SPR 16855 +-*/
#ifdef KPI_STATS
void calculateProcessKPITimeout(TimeSpec *temp_timeout);
#endif
extern SInt32 sendMsgToOAM(UInt8 *cnfBuff_p, UInt16 msgLen
        );
/* SPR 5599 changes start (ZIP ID 129187) */
extern void prepareRlcInitIndHeader(UInt8 *msg_p, UInt16 apiId, UInt16 *msgLength); 
/* SPR 5599 changes end (ZIP ID 129187) */
void oamCmdPrepInitInd( UInt8 *responseBuf, UInt16 *length,
        InternalCellIndex intCellId );
/* Layer2 Init Ind Changes Ends */
#ifdef KPI_STATS
/* kpi stats function declaration */
/* 100MS_SUPPORT : start */  
/*spr 19296 fix start*/
void sendMacKpiThpStatsInd(UInt32 macKpiIndex);
/*spr 19296 fix end*/
/* 100MS_SUPPORT : end */  
void sendMacKpiStatsInd(LteMacKPIStats  *pLteMacKPIStats, void *pLteKPIConfig, UInt8 moduleIndex, UInt32 durationTTI);
/* + KPI_CA */
void  macResetKpiCounters(UInt32 passiveIndexForMacStats, InternalCellIndex internalCellIndex);
/* - KPI_CA */
void  macResetModuleKpiCounters(void* localStats, UInt32 size);
/* SPR 10579 changes start*/
void  macResetModuleKpiThpCounters(void* localStats, UInt32 size);
/* SPR 10579 changes end*/
void sendMacRrmKpiThpStatsInd(LteMacKPIThpStats  *pLteMacKPIThpStats, LteMacRrmKPIConfig *pLteMacKPIConfig, UInt16 respMsgId);
/* KPi Warning Removed*/
/* 100MS_SUPPORT : start */  
/* + SPR 17439 */
UInt32 checkForKPIUpdateReporting(void);
/* - SPR 17439 */
void distributeKPIStats(UInt32 moduleBitmask);
/* 100MS_SUPPORT : end */  
/* + SPR 17439 */
void resetThpStats(void);
void calculateBestDuration(TimeSpec *temp_timeout);
/* KPi Warning Removed*/
/* + SPR_12546_12547 */
void  checkAndSendKPIStats(void);
void resetKPILastCalculatedTicks(void);
/* -SPR 17439 */
/* - SPR_12546_12547 */
#endif /*KPI_STATS*/

#ifdef PERF_STATS
UInt32 checkForPerfStatsUpdateReporting(void);
void distributePerfStats(UInt8 processingAllowed);
void calculateBestDurationPerfStats(struct timespec *temp_timeout);
#ifdef FLEXRAN

#define IPC_SEMAPHORE

typedef enum
{
	FAPI_SHMA_CH_PARA_REQ = 0,
	FAPI_SHMA_CH_CFG_REQ,
	FAPI_SHMA_CH_START_REQ,
	FAPI_SHMA_CH_STOP_REQ,
	FAPI_SHMA_CH_DL_CFG_REQ,
	FAPI_SHMA_CH_UL_CFG_REQ,
	FAPI_SHMA_CH_HI_DCI0,
	FAPI_SHMA_CH_TX_REQ,
	FAPI_SHMA_CH_PARA_RSP,
	FAPI_SHMA_CH_CFG_RSP,
	FAPI_SHMA_CH_STOP_IND,
	FAPI_SHMA_CH_ERROR_IND,
	FAPI_SHMA_CH_SUBFRAME_IND,
	FAPI_SHMA_CH_HARQ_IND,
	FAPI_SHMA_CH_CRC_IND,
	FAPI_SHMA_CH_RX_ULSCH_IND,
	FAPI_SHMA_CH_RACH_IND,
	FAPI_SHMA_CH_SRS_IND,
	FAPI_SHMA_CH_RX_SR_IND,
	FAPI_SHMA_CH_RX_CQI_IND,
	FAPI_SHMA_DL_BUFFER,
	FAPI_SHMA_UL_BUFFER,	
	FAPI_SHMA_BBU_PKG_IN, // for L1D test
	FAPI_SHMA_BBU_PKG_OUT,// for L1D test
	FAPI_SHMA_CH_END,
} FAPI_SHMA_CH_T;	

#define IPC_READY_MAGIC_NUMBER 0x6970637265616479 //ASCII "IPCREADY"

//#define MAX_CELL_NUM 16
#define FLEXRAN_MAX_CELL_NUM 2

#define IPC_QUEUE_NUMBER FAPI_SHMA_CH_END
#define IPC_USED_BY_PHY 0
#define IPC_USED_BY_MAC 1

#define FLEXRAN_MAX_QUEUE_SIZE 16
#define FLEXRAN_MAX_MSG_SIZE 3072
#define MAX_BBU_PKG_SIZE 9000  // for L1D test

#define FLEXRAN_DL_BUFFER_SIZE 0x1000000 // 16MB
#define FLEXRAN_UL_BUFFER_SIZE 0x1000000 // 16MB

#define IPC_SUCCESS (0)
#define IPC_NOT_INIT_ERR (-1)
#define IPC_INPUT_PARA_ERR (-2)
#define IPC_SEND_QUEUE_FULL_ERR (-3)
#define IPC_SEND_EDQUOT_ERR (-4)
#define IPC_SEND_ENOBUFS_ERR (-5)
#define IPC_RECV_BUF_ERR (-6)
#define IPC_UNKNOWN_ERR (-7)

typedef struct
{
	char* name;
	unsigned int counter;
	unsigned int size;
}ipc_queue_s;

typedef struct
{
	/*write index, can only be updated by sending side after sending a message*/
	unsigned int nWriteIdx;
	
	/*read index, can only be updated by receiving side after received a message,
		no messages when nWriteIdx = nReadIdx*/
	unsigned int nReadIdx;
	
	/*in each message, the 1st 4 byte is length, remaining bytes are payload*/
	/* Variable length and payload field:
		Each PDUData block includes:
			1)UWORD32 nLen: message length
			2)UWORD8 pMsg[nLen]: message payload
			
			|-|---|-|-|---|-------|---|-------------|---------------|
Byte 		|0|...|3|4|...|3+nLen|....| ... ...     |ring counter-1|
			|--------|------------|---|-----------------------------|
pMsgBuf 	|nLen- |ringSize |.....	| ... ... |
			|--------|------------|---|-----------------------------|
			|---No.1 message ------|.... |---No.(ring counter-1) message--|*/
	unsigned char pMsgBuf[1];
	
}memory_header_s;

typedef struct
{
	unsigned short maxQueueNumPerQueue[IPC_QUEUE_NUMBER];
	unsigned long totalBytesPerQueue[IPC_QUEUE_NUMBER];
	unsigned int errorNumPerQueue[IPC_QUEUE_NUMBER];
}ipcStatisticStruct;

extern int flexran_ipc_init(unsigned int cell_num,unsigned char type, int mac_cell_id);
extern void flexran_ipc_exit();
extern int flexran_ipc_is_channel_ready(unsigned int cell_id, FAPI_SHMA_CH_T queue_id, char* buf, unsigned int len);
extern int flexran_ipc_send_msg(unsigned int cell_index, FAPI_SHMA_CH_T ipcId, char* buf, unsigned int len);
extern int flexran_ipc_recv_msg(unsigned int cell_index, FAPI_SHMA_CH_T ipcId, char* buf, unsigned int len);
extern int flexran_ipc_set_statistic(ipcStatisticStruct* data);

extern char* flexran_ipc_get_dl_databuffer(unsigned int cell_index);
extern char* flexran_ipc_get_ul_databuffer(unsigned int cell_index);

extern char g_txBBUBuf[MAX_BBU_PKG_SIZE];

#ifdef IPC_SEMAPHORE
extern int flexran_ipc_notify_mac_ready(unsigned int cell_index);
extern int flexran_ipc_notify_phy_ready(unsigned int cell_index);

extern int flexran_ipc_timewait_phy_ready(unsigned int cell_index, const struct timespec *timeout);
extern int flexran_ipc_timewait_mac_ready(unsigned int cell_index, const struct timespec *timeout);
extern int flexran_ipc_wait_phy_ready(unsigned int cell_index);
extern int flexran_ipc_wait_mac_ready(unsigned int cell_index);
extern int flexran_ipc_trywait_phy_ready(unsigned int cell_index);
extern int flexran_ipc_trywait_mac_ready(unsigned int cell_index);
#endif
#endif
#endif
#endif
