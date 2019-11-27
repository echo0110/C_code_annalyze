/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: logging.c,v  2011/01/27 06.00:00 gur19170 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file contains implementation of logging framework
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: logging.c,v $
 * Revision 1.1.1 2012/08/03 12:10:27  gur31292
 * Changes related to DCI Format3A
 *
 * Initial version
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

#include <stdio.h> 
#include <string.h>
#include <time.h>
#include <sys/time.h>

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "logging.h"
/****************************************************************************
  Private Definitions
 ****************************************************************************/
#define FILE_MODE "w+"

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
void decode_fapi_message(char *msg_buffer, FILE *outFilePtr);
/*****************************************************************************
 * Private Constants
 ****************************************************************************/




/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/* MAC_TTI_LOG Start */
char *pduTypeString[13] = {"FAPI_ULSCH","FAPI_ULSCH_CQI_RI","FAPI_ULSCH_HARQ","FAPI_ULSCH_CQI_HARQ_RI","FAPI_UCI_CQI","FAPI_UCI_SR","FAPI_UCI_HARQ",
                      "FAPI_UCI_SR_HARQ","FAPI_UCI_CQI_HARQ","FAPI_UCI_CQI_SR","FAPI_UCI_CQI_SR_HARQ","FAPI_SRS","FAPI_INVALID" };
char *RATPolicyType[4] = {"RA_POLICY_0","RA_POLICY_1","RA_POLICY_2_LOCALIZED","RA_POLICY_2_DISTRIBUTED"};
char *RNTIType[5] = {"INVALID_CRNTI","C_RNTI","FAPI_RA_RNTI_P_RNTI_SI_RNTI","SPS_CRNTI","OTHER_CRNTI"};
char *TransType[6] = {"RE_TX","NEW_TX","ADAPTIVE_RE_TX","NON_ADAPTIVE_RE_TX","CCCH_TX","INVALID_TX"};
char *TransmissionScheme[7] = {"FAPI_SINGLE_ANTENNA_PORT_0","FAPI_TX_DIVERSITY","FAPI_LARGE_DELAY_CDD","FAPI_CLOSED_LOOP_SPATIAL_MULTIPLEXING",
	"FAPI_MULTI_USER_MIMO","FAPI_CLOSED_LOOP_RANK_1_PRECODING","FAPI_SINGLE_ANTENNA_PORT_5"};
char *dciStr[12] = {"DCI_FORMAT_0","DCI_FORMAT_1","DCI_FORMAT_1A","DCI_FORMAT_1B","DCI_FORMAT_1C","DCI_FORMAT_1D","DCI_FORMAT_2","DCI_FORMAT_2A",
	"DCI_FORMAT_3","DCI_FORMAT_3A","DCI_FORMAT_0_MSG3","INVALID_DCI_FORMAT"};

char *modulationString[7] = {"INVALID_MOD_SCHEME","INVALID_MOD_SCHEME","QPSK","INVALID_MOD_SCHEME",
	"SIXTEENQAM","INVALID_MOD_SCHEME","SIXFOURQAM"};

/* MAC_TTI_LOG End */
/*
 **************************************
 * Log Strings on basis of Log ID
 **************************************
 */
LogDefValues logCommValues[LOG_MAX_ID]={
{INVALID_LOG_ID, LOGFATAL, 0,\
"INVALID_LOG", \
{"LineNum","Size","","","", \
"","",\
FILENAME,FNAME,\
}},
{L2_GET_MEMFROMPOOL_FAIL_ID, LOGWARNING, L2_SYS_FAIL,\
"L2_GET_MEMFROMPOOL_FAIL", \
{"LineNum","Size","InternalCellId","","", \
"","",\
FILENAME,FNAME,\
}},
{L2_MSG_SEGNEXT_FAIL_ID, LOGFATAL, L2_SYS_FAIL,\
"L2_MSG_SEGNEXT_FAIL", \
{"LineNum","","","","", \
"","",\
FILENAME,FNAME,\
}},
{L2_PUSH_QUEUE_NODE_FAIL_ID, LOGFATAL, L2_SYS_FAIL,\
"L2_PUSH_QUEUE_NODE_FAIL", \
{"LineNum","","","","", \
"","",\
FILENAME,FNAME,\
}},
{L2_MSG_GET_FAIL_ID, LOGFATAL, L2_SYS_FAIL,\
"L2_MSG_GET_FAIL", \
{"LineNum","","","","", \
"","",\
FILENAME,FNAME,\
}},/*5*/
{L2_CREATE_SOCKET_FAIL_ID, LOGFATAL, L2_SYS_FAIL,\
"L2_CREATE_SOCKET_FAIL", \
{"LineNum","","","","", \
"","",\
FILENAME,FNAME,\
}},
/*SPR 2030 fix Start*/    
{L2_ACCEPT_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL, \
"L2_ACCEPT_SOCKET_FAIL", \
{"LineNum","","","","", \
"","",\
FILENAME,FNAME,\
}},
/*SPR 2030 fix End*/    
/*Coverity 95472_95476 Fix Start*/
{L2_CLOSE_SOCKET_FAIL_ID, LOGFATAL, L2_SYS_FAIL, \
"L2_ACCEPT_SOCKET_FAIL", \
{"LineNum","","","","", \
"","",\
FILENAME,FNAME,\
}},
/*Coverity 95472_95476 Fix End*/
{L2_STR_NET_CONV_FAIL_ID, LOGFATAL, L2_SYS_FAIL,\
"L2_STR_NET_CONV_FAIL", \
{"LineNum","","","","", \
"","",\
FILENAME,FNAME,\
}},
{L2_BIND_SOCKET_FAIL_ID, LOGFATAL, L2_SYS_FAIL,\
"L2_BIND_SOCKET_FAIL", \
{"LineNum","","","","", \
"","",\
FILENAME,FNAME,\
}},
{L2_SOCKET_SENDTO_FAIL_ID, LOGERROR, L2_SYS_FAIL,\
"L2_SOCKET_SENDTO_FAIL", \
{"LineNum","LengthWritten","InternalCellId","","", \
"","",\
FILENAME,FNAME,\
}},
 {L2_SOCKET_RECVFROM_FAIL_ID, LOGERROR, L2_SYS_FAIL,\
"L2_SOCKET_RECVFROM_FAIL_ID", \
{"LineNum","BytesRecvd","","","", \
"","",\
FILENAME,FNAME,\
}},
/*-----LogInfo--------*/
 {L2_SOCKET_RECVFROM_SUCCESS_ID, LOGINFO, PDCP_RRCOAM,\
"Msg Received from OAM/RRC", \
{"SourceModuleId","BytesRecvd","","","", \
"","",\
FNAME,EVENT\
}},/*12*/
/*-----LogInfo--------*/
{ L2_GET_HOSTBYNAME_FAIL_ID, LOGFATAL, PDCP_RRCOAM,\
"gethostbyname failed", \
{"LineNum","","","","", \
"","",\
"FileName ", FNAME,\
}},
{ L2_LOG_LOST, LOGERROR, L2_SYS_FAIL,\
"Layer logs were lost", \
{"LogNum","","","","", \
"","",\
"FileName ", FNAME,\
}},
{ L2_CRASH_ID, LOGFATAL, L2_SYS_FAIL,\
"L2 crashed", \
{"Signal","","","","", \
"","",\
"FileName ", FNAME,\
}},
{ L2_BACKTRACE_ID, LOGFATAL, L2_SYS_FAIL,\
"backtrace addresses", \
{"address","address","address","address","address", \
"","",\
"FileName ", FNAME,\
}},
{L2_MEM_POOL_HIGH_WATERMARK, LOGWARNING, L2_SYS_FAIL,
 "MemPool high WaterMark Reached",
{"MemPoolUsage","","","","",
 "","",
FNAME,"",
}},
{L2_MSG_POOL_HIGH_WATERMARK, LOGWARNING, L2_SYS_FAIL,
 "MsgPool high WaterMark Reached",
{"MsgPoolUsage","","","","",
 "","",
FNAME,"",
}},
/* SPR 16766 ++ */
/* SPR 16766 -- */
{MAC_DBG_LOG_ID, LOGERROR,MAC_L1_INF,\
"MAC_DBG_LOG", \
{"line","first","second","third","fourth", \
"","",\
FNAME,EVENT,\
}},
{MAC_PHY_SF_IND_ID, LOGDEBUG,MAC_L1_INF,\
"MAC_PHY_SF_IND", \
{"SFN","SF","","","", \
"","",\
FNAME,"RXINDICATION",\
}},  
{MAC_BCH_CTL_TX_ID, LOGDEBUG,MAC_L1_INF,\
"BCH_CTL_TX", \
{"MibDataLen","PduId","DEFAULT_TX_POWER","","", \
"","",\
FNAME,EVENT,\
}},
{MAC_DCI0_TX_ID, LOGBRIEF,MAC_L1_INF,\
"DCI0_TX",\
{"ue_index","HarqId","NewDataInd","mcs","rb", \
"","",\
FNAME,EVENT,\
}},
{MAC_DCI_TX_ID,LOGDEBUG,MAC_L1_INF,\
"DCI_TX", \
{"ue_index","HarqId","DciFormat","Tb1Valid","Tb2Valid",\
"","",\
FNAME,EVENT,\
}},/*20*/
{MAC_COMM_TX_ID,LOGDEBUG,MAC_L1_INF,\
"COMM_CTL_TX",
{"MsgType","ue_index","HarqId","DciFormat","Tb1Valid",\
"","",\
FNAME,EVENT,\
}},
{MAC_HI_TX_ID,LOGDEBUG,MAC_L1_INF,\
"HI_TX", \
{"RBSTART","NDMRS","HIVALUE","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_BCH_DATA_TX_ID,LOGDEBUG,MAC_L1_INF,\
"BCH_DATA_TX", \
{"MibDataLen","PduId","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_DATA_TX_ID,LOGDEBUG,MAC_L1_INF,\
"DATA_TX",\
{"ue_index","HarqId","DciFormat","Tb1Size","Tb2Size",\
"","",\
FNAME,EVENT,\
}},
{MAC_COMM_DATA_TX_ID,LOGDEBUG,MAC_L1_INF,\
"COMM_DATA_TX",\
{"MsgType","ue_index","HarqId","DciFormat","Tb1Size",\
"","",\
FNAME,EVENT,\
}},
{MAC_HI_RX_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
"HI_RX_RNTIERR",\
{"Rnti","RNTIINFOPOINTER","RntiFlag","RecvTTI","",\
"","",\
FNAME,EVENT,\
}},
{MAC_HI_RX_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
"HI_RX_UEDELETE",\
{"Rnti","ue_index","RecvTTI","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_HI_TCRNTI_RX_ID,LOGDEBUG,MAC_L1_INF,\
"HI_TCRNTI_RX",\
{"Rnti","ue_index","RecvTTI","","",\
"","",\
FNAME,EVENT,\
}},/*28*/
{MAC_HI_CRNTI_RX_ID,LOGDEBUG,MAC_L1_INF,\
"HI_CRNTI_RX",\
{"ue_index","Tb1Ack","Tb2Ack","SF","RecvTTI",\
"","",\
FNAME,EVENT,\
}},
{MAC_HI_RX_INS_ID,LOGDEBUG,MAC_L1_INF,\
"HI_RX_INS",\
{"ue_index","TB1TB2ACKNACK","RecvTTI","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_HARQ_IND_RECV_ID,LOGBRIEF,MAC_DL_HARQ,\
"MAC_HARQ_IND_RECV",\
{"RNTI","mode","numofAcknack","dlAckNack","cell",\
"","",\
FNAME,EVENT,\
}},
{MAC_CRC_RX_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
"CRC_RX_RNTIERR",\
{"Rnti","RNTIINFOPOINTER","RntiFlag","RecvTTI","",\
"","",\
FNAME,EVENT,\
}},
{MAC_CRC_RX_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
"CRC_RX_UEDELET",\
{"Rnti","ue_index","RecvTTI","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_CRC_CRNTI_RX_ID,LOGDEBUG,MAC_L1_INF,\
"CRC_CRNTI_RX",\
{"Rnti","ue_index","AckNack","SF","RecvTTI",\
"","",\
FNAME,EVENT,\
}},
{MAC_CRC_RX_UECONTEXT_NULL_ID,LOGDEBUG,MAC_L1_INF,\
"CRC_UECONTEXT_NULL",\
{"Rnti","ue_index","AckNack","SF","RecvTTI",\
"","",\
FNAME,EVENT,\
}},
{MAC_CRC_RX_CRCFLAG_INS_ID,LOGBRIEF,MAC_L1_INF,\
"CRC_RX_CRCFLAG_INS",\
{"AckNack","AckQCount","NackQCount","ContainerId","ue_index",\
"","",\
FNAME,EVENT,\
}},
{MAC_ULSCH_RX_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
"ULSCH_RX_RNTIERR",\
{"Rnti","RNTIINFOPOINTER","RntiFlag","RecvTTI","",\
"","",\
FNAME,EVENT,\
}},/*36*/
{MAC_ULSCH_RX_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
"ULSCH_RX_UEDELETE",\
{"Rnti","ue_index","RecvTTI","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_ULSCH_RX_DATA_ID,LOGDEBUG,MAC_L1_INF,\
"ULSCH_RX_DATA",\
{"ue_index","Rnti","RecvTTI","DataSize","WITHDEMUX",\
"ThreadId","DlDelay",\
FNAME,EVENT,\
}},
{MAC_ULSCH_FREQ_SEL_SCH_ID,LOGDEBUG,MAC_PUSCH,\
"MAC_ULSCH_FREQ_SEL_SCH_ID",\
{"ue_index","Rnti","RecvTTI","DataSize","WITHDEMUX",\
"ThreadId","DlDelay",\
FNAME,EVENT,\
}},
/* SPR 1600 changes start */
{MAC_ULCONFIG_ULSCH_ID,LOGDEBUG,MAC_L1_INF,\
"ULCONFIG_ULSCH",\
{"Rnti","HarqId","RiStart","NumOfRB","DELAYTICK",\
"ContainerId","",\
FNAME,EVENT,\
}},
/* SPR 1600 changes end */
{MAC_DEMUX_LC_ID,LOGDEBUG,MAC_L1_INF,\
"DEMUX_LC",\
{"LcId","Length","REM_PAYLOAD_LEN","DECODE_FLAG","EXTN_FLAG",\
"","",\
FNAME,EVENT,\
}},
{MAC_DEMUX_DATA_LC_ID,LOGDEBUG,MAC_L1_INF,\
"DEMUX_DATA_LC",\
{"LcId","Length","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_DEMUX_DATA_INVALID_LC_ID,LOGWARNING,MAC_L1_INF,\
"DATA_INVALID_LCID",\
{"LcId","Length","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_DEMUX_DATA_INVALID_LCG_ID,LOGWARNING,MAC_L1_INF,\
"DATA_INVALID_LCGID",\
{"LcId","Length","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_DEMUX_QUEUE_INS_ID,LOGDEBUG,MAC_L1_INF,\
"DEMUX_QUEUE_INS",\
{"ue_index","Rnti","RecvTTI","DataSize","DEMUXID",\
"","",\
FNAME,EVENT,\
}},/*45*/
{MAC_RACH_IND_RCVD_ID,LOGINFO,MAC_L1_INF,\
"RACH_IND_RCVD",\
{"RARNTI","RecvTTI","PreambleId","TA","TMCS",\
"","",\
FNAME,EVENT,\
}},
{MAC_CONTENTION_ACK_RCVD_ID,LOGDEBUG,MAC_L1_INF,\
"CONTENTION_ACK_RCVD",\
{"Rnti","ue_index","RecvTTI","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_SR_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
"SR_RX_RNTIERR",\
{"Rnti","RNTIINFOPOINTER","RntiFlag","RecvTTI","",\
"","",\
FNAME,EVENT,\
}},
{MAC_SR_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
"SR_RX_UEDELETE",\
{"Rnti","ue_index","RecvTTI","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_SR_RCV_IND_ID,LOGDEBUG,MAC_L1_INF,\
"SR_RCVD",\
{"ue_index","RecvTTI","NetBSRLoad","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_SR_DROP_IND_ID,LOGDEBUG,MAC_L1_INF,\
"SR_DROP",\
{"ue_index","RecvTTI","NetBSRLoad","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_CQI_RNTIERR_ID,LOGDEBUG,MAC_L1_INF,\
"CQI_RX_RNTIERR",\
{"Rnti","RNTIINFOPOINTER","RntiFlag","RecvTTI","",\
"","",\
FNAME,EVENT,\
}},
{MAC_CQI_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
"CQI_RX_UEDELETE",\
{"Rnti","ue_index","RecvTTI","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_CQI_DECODE_ID,LOGINFO,MAC_L1_INF,\
"CQI_DECODE",\
{"CqiReport","ue_index","RecvTTI","DataSize","RI",\
"","",\
FNAME,EVENT,\
}},/*55*/
{MAC_SRS_UE_RECV_ID,LOGDEBUG,MAC_L1_INF,\
"SRS_UE_RECV",\
{"HANDLE","Rnti","TA","NumOfRB","RBSTART",\
"NumOfUe","",\
FNAME,EVENT,\
}},
{MAC_SRS_RB_RECV_ID,LOGDEBUG,MAC_L1_INF,\
"SRS_RB_RECV",\
{"RB","SNR","NumOfRB","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_SRS_RNTIERR_ID,LOGWARNING,MAC_L1_INF,\
"SRS_RNTIERR",\
{"Rnti","RNTIINFOPOINTER","RntiFlag","RecvTTI","",\
"","",\
FNAME,EVENT,\
}},
{MAC_SRS_UEDELETE_ID,LOGDEBUG,MAC_L1_INF,\
"SRS_UEDELETE",\
{"Rnti","ue_index","RecvTTI","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_PHY_INIT_INV_MSG_LEN_ID,LOGWARNING,MAC_L1_INF,\
"MAC_PHY_INIT_INV_MSG_LEN_ID",\
{"BYTES","","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_PHY_INV_MSG_LEN_ID,LOGWARNING,MAC_L1_INF,\
"MAC_PHY_INV_MSG_LEN_ID",\
{"BYTES","","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_PHY_INV_MSG_ID,LOGWARNING,MAC_L1_INF,\
"MAC_PHY_INV_MSG_ID",\
{"BYTES","","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_PHY_INV_SFNSF_ID,LOGWARNING,MAC_L1_INF,\
"MAC_PHY_INV_SFNSF_ID",\
{"MsgId","RECVDSFN","RECVDSF","CURRENTSFN","CURRENTSF",\
"","",\
FNAME,EVENT,\
}},/*63*/
{MAC_PHY_ERR_SFNSF_MSG_ID, LOGERROR, MAC_L1_INF,\
"MAC receives error msg body type 1 from L1",\
{"ErrCnt","RECVDSFN","RECVDSF","EXPECTEDSFN","EXPECTEDSF",\
"","",\
FNAME, "RetVal",\
}},
{MAC_PHY_ERR_HI_MSG_ID, LOGERROR, MAC_L1_INF,\
"MAC receives error msg body type 3 from L1",\
{"ErrCnt","SubErrCode","PhichLowUlRbId","","",\
"","",\
FNAME, "RetVal",\
}},
{MAC_PHY_ERR_MSG_TX_ID, LOGERROR, MAC_L1_INF,\
"MAC receives error msg body type 4 from L1",\
{"LineNum", "ErrCnt","SubErrCode","PduId","",\
"","",\
FNAME,"RetVal",\
}},
{MAC_PHY_ERR_SUBFRAME_ID, LOGERROR, MAC_L1_INF,\
"MAC receives error msg body type 2 from L1",\
{"ErrCnt", "SubErrCode", "Direction", "Rnti", "PduType", \
"","",\
FNAME,"RetVal",\
}},
{MAC_PHY_ERR_BCH_MISSING_ID, LOGERROR, MAC_L1_INF,\
"MAC receives error in DL Config for missing BCH message from L1",\
{"LineNum", "ErrCnt","","","",\
"","",\
FNAME,"RetVal",\
}},
{MAC_PHY_TICK_MISS,LOGWARNING,MAC_L1_INF,\
"MAC_PHY_TICK_MISS_ID",\
{"RECVDSFN","RECVDSF","CURRENTSFN","CURRENTSF","",\
"","",\
FNAME,EVENT,\
}},
{MAC_ACK_ZERO_BAND_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
"ACK_ZERO_BAND_ULHARQ",\
{"ue_index","HarqId","ReTxType","ReTxCount","AckRecvCount",\
"","",\
FNAME,EVENT,\
}},
{MAC_ALLOCATE_ULHARQ_UEINFO_ID,LOGDEBUG,MAC_UL_HARQ,\
"ALLOCATE_ULHARQ_UEINFO",\
{"ue_index","HarqId","TbSize","AllocId","RIV",\
"NumOfRB","StrictRB",\
FNAME,EVENT,\
}},
{MAC_ULHARQ_ALLOC_FAIL_RI_NONZERO_ID,LOGWARNING,MAC_UL_HARQ,\
"ALLOCATE_BANDNONZERO_FAIL",\
{"ue_index","HarqId","TbSize","AllocId","RIV",\
"NumOfRB","StrictRB",\
FNAME,EVENT,\
}},
{MAC_ACK_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"ACK_FREE_ULHARQ",\
{"ue_index","HarqId","RecvTTI","ReTxType","",\
"","",\
FNAME,EVENT,\
}},
{MAC_INVALID_CRC_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"INVALID_CRC_FREE_ULHARQ",\
{"ue_index","HarqId","RecvTTI","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_NACK_DELAY_FREE_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
"NACK_DELAY_FREE_ULHARQ",\
{"ue_index","HarqId","RecvTTI","ReTxType","SchedTTI",\
"","",\
FNAME,EVENT,\
}},
{MAC_NACK_CCE_ALLOC_SUCC_ID,LOGDEBUG,MAC_UL_HARQ,\
"NACK_DCI_CCE_ALLOC_SUCC",\
{"ue_index","HarqId","RecvTTI","ReTxType","CceId",\
"","",\
FNAME,EVENT,\
}},/*75*/
{MAC_NACK_CCE_ALLOC_FAIL_ID,LOGDEBUG,MAC_UL_HARQ,\
"NACK_DCI_FAIL_ULHARQ",\
{"ue_index","HarqId","RecvTTI","ReTxType","ReTxCount",\
"","",\
FNAME,EVENT,\
}},
{MAC_NACK_DROP_UEDELETE_ID,LOGDEBUG,MAC_UL_HARQ,\
"NACK_DCI_DROP_UEDELETE",\
{"ue_index","HarqId","RecvTTI","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_INVALID_NACKNODE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"INVALID_NACKNODE_ULHARQ",\
{"","","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_NACK_RETX_COMP_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"NACK_RETX_COMP_FREE_ULHARQ",\
{"ue_index","HarqId","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_NACK_RES_ALLOC_FAIL_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
"NACK_RES_ALLOC_FAIL_ULHARQ",\
{"ue_index","HarqId","RBMapNum","ReTxType","ReTxCount",\
"","",\
FNAME,EVENT,\
}},
{MAC_NACK_RES_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"NACK_RES_ULHARQ",\
{"ue_index","HarqId","RBMapNum","ReTxType","ReTxCount",\
"RiLength","RiStart",\
FNAME,EVENT,\
}},
{MAC_NACK_RES_ZERO_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"NACK_RES_ZERO_ULHARQ",\
{"ue_index","HarqId","RBMapNum","ReTxType","ReTxCount",\
"","",\
FNAME,EVENT,\
}},
{MAC_UPDATE_DCI_INFO_ONE_ID,LOGDEBUG,MAC_UL_HARQ,\
"UPDATE_DCI_INFO_ONE",\
{"ue_index","DciFormat","BITMAP","Mode","REDVER",\
"","",\
FNAME,EVENT,\
}},
{MAC_UPDATE_DCI_INFO_TWO_ID,LOGDEBUG,MAC_UL_HARQ,\
"UPDATE_DCI_INFO_TWO",\
{"TxInd","Rnti","NDMRS","HarqId","TPC",\
"","",\
FNAME,EVENT,\
}},
{MAC_ALLOCATE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"ALLOCATE_ULHARQ",\
{"HarqId","DeltaBsrLoad","RiStart","NumOfRB","",\
"RIV","",\
FNAME,EVENT,\
}},
{MAC_UPDATE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"UPDATE_ULHARQ",\
{"HarqId","ReTxCount","ReTxType","RiStart","NumOfRB",\
"RIV","",\
FNAME,EVENT,\
}},
{MAC_ADAPTIVE_RES_ALLOC_FAIL_ID,LOGERROR,MAC_UL_HARQ,\
"ADAPTIVE_RES_ALLOC_FAIL",\
{"ReTxType","ReqRB","MinRB","RBMapNum","StrictRB",\
"HarqId","",\
FNAME,EVENT,\
}},
{MAC_NON_ADAPTIVE_RES_ALLOC_FAIL_ID,LOGERROR,MAC_UL_HARQ,\
"NON_ADAPTIVE_RES_ALLOC_FAIL",\
{"ReTxType","RiStart","NumOfRB","RIV","RBMapNum",\
"HarqId","",\
FNAME,EVENT,\
}},
{MAC_NON_ADAPTIVE_CCE_ALLOC_ID,LOGERROR,MAC_UL_HARQ,\
"NON_ADAPTIVE_CCE_ALLOC",\
{"CceId","ReTxType","RiStart","NumOfRB","RBMapNum",\
"HarqId","",\
FNAME,EVENT,\
}},/*90*/
{MAC_NACK_ADV_ALLOC_SUCC_ID,LOGDEBUG,MAC_UL_HARQ,\
"NACK_ADV_ALLOC_SUCC",\
{"ReTxType","RiStart","NumOfRB","RIV","RBMapNum",\
"HarqId","",\
FNAME,EVENT,\
}},
{MAC_NACK_ADV_ALLOC_FAIL_ID,LOGDEBUG,MAC_UL_HARQ,\
"NACK_ADV_ALLOC_FAIL",\
{"ReTxType","RiStart","NumOfRB","RIV","RBMapNum",\
"HarqId","",\
FNAME,EVENT,\
}},
{MAC_DELAY_ACK_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"DELAY_PROC_ACK_FREE_ULHARQ",\
{"ue_index","HarqId","RecvTTI","ReTxType","RBMapNum",\
"","",\
FNAME,EVENT,\
}},
{MAC_UEINDEX_DELETE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"UEINDEX_DELETE_ULHARQ",\
{"ue_index","HarqId","RecvTTI","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_INVALID_ACKNODE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"INVALID_ACKNODE_ULHARQ",\
{"","","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_INVALID_HID_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
"TIMER_INVALID_HID_ULHARQ",\
{"ue_index","ExpSF","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_TIMER_START_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"TIMER_START_ULHARQ",\
{"ue_index","HarqId","ExpSF","ExpTTI","Count",\
"","",\
FNAME,EVENT,\
}},
{MAC_HARQ_PROC_FREE_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
"TIMER_HARQ_PROC_FREE",\
{"ue_index","HarqId","ExpSF","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_TIMER_NODE_BUSY_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
"TIMER_NODE_BUSY_ULHARQ",\
{"ue_index","HarqId","ExpSF","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_TIMER_CNT_ZERO_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
"TIMER_CNT_ZERO_ULHARQ",\
{"ExpSF","","","","",\
"","",\
FNAME,EVENT,\
}},/*100*/
{MAC_TIMER_CNT_INVALID_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
"TIMER_CNT_INVALID_ULHARQ",\
{"ue_index","ExpSF","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_TIMER_STOP_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"TIMER_STOP_ULHARQ",\
{"ue_index","HarqId","ExpSF","ExpTTI","Count",\
"","",\
FNAME,EVENT,\
}},
{MAC_TIMER_NODE_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"NODE_ALREADY_FREE_ULHARQ",\
{"ue_index","ExpSF","Count","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_EXPIRY_CNT_ZERO_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"EXPIRY_CNT_ZERO_ULHARQ",\
{"ExpSF","","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_EXPIRY_SFMISMATCH_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
"EXPIRY_SFMISMATCH_ULHARQ",\
{"ExpSF","SF","ue_index","HarqId","ExpTTI",\
"","",\
FNAME,EVENT,\
}},
{MAC_EXPIRY_NACK_INS_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"EXPIRY_NACK_INS_ULHARQ",\
{"ue_index","HarqId","ExpSF","ExpTTI","ReTxCount", \
"ReTxType","",\
FNAME,EVENT,\
}},
{MAC_EXPIRY_HARQ_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"EXPIRY_HARQ_FREE_ULHARQ",\
{"ue_index","HarqId","ExpSF","ExpTTI","", \
"","",\
FNAME,EVENT,\
}},
{MAC_TICKDIFF_TIMER_START_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"TICKDIFF_TIMER_START_ULHARQ",\
{"ue_index","HarqId","ExpSF","ExpTTI","Count", \
"ReTxCount","ReTxType",\
FNAME,EVENT,\
}},
{MAC_TICKDIFF_NODE_BUSY_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
"TICKDIFF_NODE_BUSY_ULHARQ",\
{"ue_index","HarqId","ExpSF","ExpTTI","Count", \
"ReTxCount","ReTxType",\
FNAME,EVENT,\
}},
{MAC_TICKDIFF_EXPIRY_NACK_INS_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"TICKDIFF_EXPIRY_NACK_INS_ULHARQ",\
{"ue_index","HarqId","ExpSF","ExpTTI","NackTTI", \
"ReTxCount","ReTxType",\
FNAME,EVENT,\
}},/*110*/
{MAC_EXPIRY_STOP_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"EXPIRY_STOP_ULHARQ",\
{"ue_index","HarqId","ExpSF","ExpTTI","Count", \
"","",\
FNAME,EVENT,\
}},
{MAC_EXPIRY_UEDELETE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"EXPIRY_UEDELETE_ULHARQ",\
{"ue_index","ExpSF","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_EXPIRY_NODE_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
"EXPIRY_NODE_FREE_ULHARQ",\
{"ExpSF","ExpTimerCount","LOOPCNT","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_RB_UNAVAIL_RAR_ID,LOGERROR,MAC_RACH,\
"MAC_RB_UNAVAIL_RAR_ID",\
{"MAXRAR","AVAILABLE_RB","REQUIRED_RB","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_RARNTI_SCH_ID,LOGDEBUG,MAC_RACH,\
"MAC_RARNTI_SCH_ID",\
{"BITMAP","SHIFT_RARNTI_VAL","RARNTI","LineNum","",\
"","",\
FNAME,EVENT,\
}},
{MAC_UPDATE_RACH_CONFIG_FAIL_ID,LOGDEBUG,MAC_RACH,\
"UPDATE_RACH_CONFIG_FAIL",\
{"RAWINDOWSIZE","CONTIMER","Index","OFFSET","LineNum",\
"","",\
FNAME,EVENT,\
}},
{MAC_RAR_RB_TBZIZE_DCI1A_FAIL_ID,LOGWARNING,MAC_RACH,\
"RAR_RB_TBZIZE_DCI1A_FAIL",\
{"BUFFLEN","NUMRAR","BIFLAG","RARLEN","RARSUBHDRLEN",\
"","",\
FNAME,EVENT,\
}},  
{MAC_RAR_RB_ALLOC_SUCC_ID,LOGDEBUG,MAC_RACH,\
"RAR_RB_ALLOC_SUCC",\
{"RARNTI","NumOfRB","mcs","ExpTTI","BIFLAG",\
"","",\
FNAME,EVENT,\
}},
{MAC_RAR_RB_ALLOC_MSG3_ID,LOGDEBUG,MAC_RACH,\
"MAC_RAR_RB_ALLOC_MSG3_ID",\
{"RARNTI","TTI","RBMapNum","","",\
 "","",\
FNAME,EVENT,\
}},
{MAC_RAR_RB_ALLOC_FAIL_ID,LOGWARNING,MAC_RACH,\
"RAR_RB_ALLOC_FAIL",\
{"TCRNTI","UL_RB_FOR_RAR","RBMapNum","NUMRAR","LineNum",\
"","",\
FNAME,EVENT,\
}},/*120*/
{MAC_RAR_TEMP_CONTEXT_FAIL_ID,LOGWARNING,MAC_RACH,\
"RAR_TCRNTI_FAIL",\
{"TCRNTI","NUMRAR","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_NEG_RAR_TTI_EXPIRED_ID,LOGWARNING,MAC_RACH,\
"NEG_RAR_TTI_EXPIRED",\
{"RARNTI","NumOfRB","mcs","ExpTTI","TTI",\
"","",\
FNAME,EVENT,\
}},
{MAC_NEG_LESS_ULRB_ID,LOGWARNING,MAC_RACH,\
"NEG_LESS_ULRB",\
{"AvailRB","ReqRB","eqRAR","RARNTI","",\
"","",\
FNAME,EVENT,\
}},
{MAC_NEG_RAR_RB_ALLOC_SUCC_ID,LOGDEBUG,MAC_RACH,\
"NEG_RAR_RB_ALLOC_SUCC",\
{"RARNTI","NumOfRB","mcs","ExpTTI","TTI",\
"MAXRAR","",\
FNAME,EVENT,\
}},
{MAC_NEG_RAR_RB_ALLOC_FAIL_ID,LOGWARNING,MAC_RACH,\
"NEG_RAR_RB_ALLOC_FAIL",\
{"TCRNTI","UL_RB_FOR_RAR","RBMapNum","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_NEG_RAR_TCRNTI_FAIL_ID,LOGWARNING,MAC_RACH,\
"NEG_RAR_TCRNTI_FAIL",\
{"TCRNTI","Count","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_NEG_RAR_TIMER_NULL_ID,LOGWARNING,MAC_RACH,\
"NEG_RAR_TIMER_NULL",\
{"TCRNTI","Count","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_RAR_DCI_ALLOC_SUCC_ID,LOGDEBUG,MAC_RACH,\
"RAR_DCI_ALLOC_SUCC",\
{"RARNTI","NumOfRB","mcs","ExpTTI","TTI",\
"","",\
FNAME,EVENT,\
}},
{MAC_RAR_DCI_ALLOC_FAIL_ID,LOGDEBUG,MAC_RACH,\
"RAR_DCI_ALLOC_FAIL",\
{"RARNTI","NumOfRB","mcs","ExpTTI","TTI",\
"CceId","DciFormat",\
FNAME,EVENT,\
}},
{MAC_FREE_RAR_ID,LOGWARNING,MAC_RACH,\
"MAC_FREE_RAR_ID",\
{"RARNTI","mcs","ExpTTI","RARESCNT","LineNum",\
 "","",\
FNAME,EVENT,\
}},/*130*/
{MAC_FREE_RAR_MSG3_ID,LOGWARNING,MAC_RACH,\
"MAC_FREE_RAR_MSG3_ID",\
{"RARNTI","RBMapNum","TTI","LineNum","",\
 "","",   
FNAME,EVENT,\
}},
{MAC_RAR_AGGLEVEL_AVAILRB_LESS_ID,LOGWARNING,MAC_RACH,\
"RAR_AGGLEVEL_AVAILRB_LESS",\
{"AGGLEVEL","AVAILCCE","RARNTI","NumOfRB","mcs",\
"","",\
FNAME,EVENT,\
}},
/* SPR 1600 changes start */
{MAC_RAR_TCRNTI_NOT_FOUND_ID,LOGWARNING,MAC_RACH,\
"RAR_TCRNTI_NOT_FOUND",\
{"TCRNTI","","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_MSG3_ULSCH_ID,LOGBRIEF,MAC_L1_INF,\
"ULSCH_MSG3",\
{"Rnti","HarqId","RiStart","NumOfRB","DELAYTICK",\
"","",\
FNAME,EVENT,\
}},
/* SPR 1600 changes end */
{MAC_MSG3_ULSCH_CONTAINER_ID,LOGBRIEF,MAC_L1_INF,\
"MAC_MSG3_ULSCH_CONTAINER_ID",\
{"Rnti","HarqId","DELAYTICK","ContainerId","",\
 "","",\
 FNAME,EVENT,\
}},
{MAC_TCRNTI_START_TIMER_ID,LOGBRIEF,MAC_RACH,\
"TCRNTI_START_TIMER",\
{"ExpTTI","TCRNTI","DELETEFLAG","RARNTI","RecvTTI",\
"","",\
FNAME,EVENT,\
}},
{MAC_TCRNTI_STOP_TIMER_ID,LOGBRIEF,MAC_RACH,\
"TCRNTI_STOP_TIMER",\
{"ExpTTI","TCRNTI","DELETEFLAG","","",\
"","",\
FNAME,EVENT,\
}},
/* SPR 16773 fix start */
{MAC_TCRNTI_TIMER_EXPIRY_ID,LOGINFO,MAC_RACH,\
"Timer has expired",\
{"ExpTTI","TCRNTI","DELETEFLAG","TCRNTI STATUS","",\
"","",\
FNAME, "TimerType",\
}},
/* SPR 16773 fix end */
{MAC_TCRNTI_DELETE_ID,LOGINFO,MAC_RACH,\
"TCRNTI_DELETE",\
{"ExpTTI","TCRNTI","DELETEFLAG","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_TCRNTI_RNTIINFO_NULL_ID,LOGERROR,MAC_RACH,\
"TCRNTI_RNTIINFO_NULL",\
{"TCRNTI","LineNum","","","",\
"","",\
FNAME,EVENT,\
}},/*140*/
{MAC_CONTENT_RES_START_TIMER_ID,LOGBRIEF,MAC_RACH,\
"CONTENT_RES_START_TIMER",\
{"ExpTTI","TCRNTI","DELETEFLAG","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_CONTENT_RES_TIMER_EXPIRY_ID,LOGINFO,MAC_RACH,\
"CONTENT_RES_TIMER_EXPIRY",\
{"ExpTTI","TCRNTI","DELETEFLAG","TCRNTI STATUS","",\
"","",\
FNAME,EVENT,\
}},
{MAC_CONTENT_RES_TCRNTI_DELETE_ID,LOGBRIEF,MAC_RACH,\
"CONTENT_RES_TCRNTI_DELETE",\
{"ExpTTI","TCRNTI","DELETEFLAG","","",\
"","",\
FNAME,EVENT,\
}},
{OAM_RRC_INVALID_MSG_LEN_ID,LOGWARNING,MAC_RRC_INF,\
"OAM_RRC_INVALID_MSG",\
{"ERRNO","MsgLen","LineNum","","",\
"","",\
FNAME,EVENT,\
}},
/* + SPR_10412 */
{OAM_MAC_PARSE_MSG_ID,LOGDEBUG,MAC_OAM_INF,\
"OAM_MAC_PARSE_MSG_ID",\
{"MsgId","Trans_Id","","","",\
"","",\
FNAME,EVENT,\
}},
/* - SPR_10412 */
{OAM_RRC_MAC_INVALID_SRC_ID,LOGERROR, MAC_RRC_INF,\
"OAM RRC MAC INVALID SRC ID",\
{"DESTID","InternalCellId","","","",\
"","",\
FNAME,EVENT,\
}},
{OAM_MAC_STAT_INFO_ID,LOGDEBUG,MAC_OAM_INF,\
"OAM_MAC_STAT_INFO_ID",\
{"ULTHROUGHPUT","DLTHROUGHPUT","OPSTATUS","","",\
"","",\
FNAME,EVENT,\
}},
{OAM_INVALID_LOGLEVEL_ID,LOGWARNING,MAC_OAM_INF,\
"OAM_INVALID_LOGLEVEL_ID",\
{"Flag","","","","",\
"","",\
FNAME,EVENT,\
}},
{OAM_INVALID_ENABLE_LOG_FLAG_ID,LOGWARNING,MAC_OAM_INF,\
"OAM_INVALID_ENABLE_LOG_FLAG_ID",\
{"Flag","","","","",\
"","",\
FNAME,EVENT,\
}},
{OAM_INVALID_DLSCHSTAT_FLAG_ID,LOGWARNING,MAC_OAM_INF,\
"OAM_INVALID_DLSCHSTAT_FLAG",\
{"Flag","","","","",\
"","",\
FNAME,EVENT,\
}},/*150*/
{OAM_MAC_INVALID_MSG_ID,LOGWARNING,MAC_OAM_INF,\
"OAM_MAC_INVALID_MSG_ID",\
{"MsgId","","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,\
"MAC_RES_MSG_ID",\
{"MsgId","SRCMODID","DESTMODID","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_OAM_RES_MSG_ID,LOGDEBUG,MAC_OAM_INF,\
"MAC_OAM_RES_MSG_ID",\
{"REQID","RESPONSEID","RspCode","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_RRC_CCCH_MSG_IND,LOGWARNING,MAC_RRC_INF,\
"RRC_CCCH_MSG",\
{"MsgType","DataSize","Rnti","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_MAC_MSG_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_MAC_MSG_ID",\
{"DESTMODID","MsgId","MsgLen","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_MAC_CELL_CONFIG_ID,LOGINFO,MAC_RRC_INF,\
"RRC_MAC_CELL_CONFIG_ID",
{"CellStatus","UlRB","DlRB","StartRARNTI","EndRARNTI",\
"TxAntenna","phich_duration",\
FNAME,EVENT,\
}},
/* CA Changes start */
{RRC_MAC_INCORRECT_STATE_ID,LOGWARNING, MAC_RRC_INF,\
"RRC MAC INCORRECT STATE",
{"CELLSTATUS","MsgId","InternalCellId","","",\
"","",\
FNAME,EVENT,\
}},
/* CA Changes end */
{MAC_RRC_PHY_CELL_CONFIG_RES_ID,LOGDEBUG,MAC_RRC_INF,\
"PHY_CELL_CONFIG_RES",\
{"cell_index","RetVal","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_PHY_CREATE_UE_RES_ID,LOGDEBUG,MAC_RRC_INF,\
"MAC_PHY_CREATE_UE_RES_ID",\
{"ue_index","RetVal","Trans_Id","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_PHY_RECONFIG_UE_RES_ID,LOGINFO,MAC_RRC_INF,\
"MAC_PHY_RECONFIG_UE_RES_ID",\
{"MsgId","ue_index","Index","","",\
"","",\
FNAME,EVENT,\
}},/*160*/
{MAC_PHY_UE_DEL_ID,LOGINFO,MAC_RRC_INF,\
"MAC_PHY_UE_DEL_ID",\
{"MsgId","ue_index","Index","","",\
"","",\
FNAME,EVENT,\
}},
{RRC_CREATE_UE_ENTITY_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_CREATE_UE_ENTITY_ID",\
{"rrcUeIndex","ue_index","rnti","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_UE_CONREJ_MSG_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_UE_CONREJ_MSG_ID",\
{"TCRNTI","RntiFlag","Index","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_UE_CONREJ_INVALID_LEN_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_UE_CONREJ_INVALID_LEN_ID",\
{"MsgId","MsgLen","EXPECTEDLEN","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_PCCH_INVALID_LEN_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_PCCH_INVALID_LEN_ID",\
{"MsgId","MsgLen","EXPECTEDLEN","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_CCCH_SFN_ERR_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_CCCH_SFN_ERR_ID",\
{"MsgId","SFNERR","EXPECTEDLEN","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_CCCH_LEN_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_CCCH_LEN_ID",\
{"MsgId","MsgLen","EXPECTEDLEN","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_BCCH_INVALID_LEN_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_BCCH_INVALID_LEN_ID",\
{"MsgId","MsgLen","EXPECTEDLEN","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_MAC_PARSE_MSG_ID,LOGDEBUG,MAC_RRC_INF,\
"RRC_MAC_PARSE_MSG_ID",\
{"MsgId","MsgLen","Trans_Id","","",\
"","",\
FNAME,EVENT,\
}},
{RRC_MAC_INVALID_MSG_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_MAC_INVALID_MSG_ID",\
{"MsgId","MsgLen","Trans_Id","","",\
"","",\
FNAME,EVENT,\
}},

/*L2_FIX_6_start*/
{RRC_MAC_LC_INFO,LOGWARNING,MAC_RRC_INF,\
"RRC_MAC_LC_INFO",\
{"TagLen","InternalCellId","","","",\
"","",\
FNAME,EVENT,\
}},
/*L2_FIX_6_start*/

/* FREQ_SELECTIVE CHG */
{MAC_PARSE_CELL_CONFIGURE_ID,LOGWARNING,MAC_RRC_INF,\
"MAC_PARSE_CELL_CONFIGURE_ID",\
{"INVALID VALUE","InternalCellId","","",\
"","","",\
FNAME,EVENT,\
}},/*170*/
/* FREQ_SELECTIVE CHG */
{MAC_CELL_CONFIGURE_FAIL,LOGERROR,MAC_RRC_INF,\
"MAC_CELL_CONFIGURE_FAIL",\
{"FAILREASON","VAL1","VAL2","VAL3","VAL4",\
"VAL5","VAL6",\
FNAME,EVENT,\
}},
/*SPR_8630_CHANGES_START*/
{MAC_CELL_RECONFIGURE_SUCCESS,LOGDEBUG,MAC_RRC_INF,\
"MAC_CELL_RECONF_SUCCESS",\
{"SUCCESS","VAL1","VAL2","VAL3","VAL4",\
"VAL5","VAL6",\
FNAME,EVENT,\
}},
/*SPR_8630_CHANGES_END*/
{MAC_CELL_RECONFIGURE_FAIL,LOGERROR,MAC_RRC_INF,\
"MAC_CELL_RECONFIGURE_FAIL",\
{"FAILREASON","VAL1","VAL2","VAL3","VAL4",\
"VAL5","VAL6",\
FNAME,EVENT,\
}},
{MAC_UE_CREATE_CNF,LOGDEBUG,MAC_RRC_INF,\
"MAC_UE_CREATE_CNF",\
{"MsgId","ue_index","RspCode","crnti","InternalCellId",\
"","",\
FNAME,EVENT,\
}},
{MAC_UE_CREATE_FAIL_ID,LOGWARNING,MAC_RRC_INF,\
"MAC_UE_CREATE_FAIL_ID",\
{"MsgId","RetVal","MsgLen","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{MAC_UE_RECONFIGURE_CNF,LOGDEBUG,MAC_RRC_INF,\
"MAC_UE_RECONFIGURE_CNF",\
{"MsgId","ue_index","RspCode","crnti","InternalCellId",\
"","",\
FNAME,EVENT,\
}},
{MAC_UE_RECONFIGURE_FAIL,LOGWARNING,MAC_RRC_INF,\
"MAC_UE_RECONFIGURE_FAIL",\
{"MsgId","RetVal","MsgLen","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{MAC_UE_POWER_HEADROOM_IND,LOGDEBUG,MAC_RRC_INF,\
"UE_POWER_HEADROOM_IND",\
{"MsgId","ue_index","POWER","crnti","InternalCellId",\
"","",\
FNAME,EVENT,\
}},
{MAC_UE_LC_ERR_RESPONSE,LOGDEBUG,MAC_RRC_INF,\
"MAC_UE_LC_ERR_RESPONSE",\
{"LCSTATUS","LCOPCODE","LCRESULT","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_UE_DRX_MSG_ID,LOGDEBUG,MAC_RRC_INF,\
"RRC_UE_DRX_MSG_ID",\
{"FAILREASON","VAL1","VAL2","VAL3","VAL4",\
"","",\
FNAME,EVENT,\
}},
{RRC_DEL_UE_MSG_ID,LOGDEBUG,MAC_RRC_INF,\
"RRC_DEL_UE_MSG_ID",\
{"MsgId","ue_index","RspCode","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},/*180*/
{MAC_SENDTO_INVALID_MSG_ID,LOGWARNING,MAC_RRC_INF,\
"MAC_SENDTO_INVALID_MSG_ID",\
{"MsgId","InternalCellId","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_DELETE_UE_CNF,LOGINFO,MAC_RRC_INF,\
"MAC_DELETE_UE_CNF",\
{"MsgId","Index","RspCode","Trans_Id","InternalCellId",\
"","",\
FNAME,EVENT,\
}},
{MAC_CELL_CONFIG_CNF,LOGINFO,MAC_RRC_INF,\
"MAC_CELL_CONFIG_CNF",\
{"MsgId","RspCode","Trans_Id","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{MAC_ERR_SFN_IND,LOGDEBUG,MAC_RRC_INF,\
"MAC_ERR_SFN_IND",\
{"MsgId","Trans_Id","SFN","SF","InternalCellId",\
"","",\
FNAME,EVENT,\
}},
{MAC_CELL_RECONFIG_CNF,LOGINFO,MAC_RRC_INF,\
"MAC_CELL_RECONFIG_CNF",\
{"MsgId","RspCode","Trans_Id","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{MAC_SYSFRAME_CNF,LOGDEBUG,MAC_RRC_INF,\
"MAC_SYSFRAME_CNF",\
{"MsgId","RspCode","SFN","SF","Trans_Id",\
"","",\
FNAME,EVENT,\
}},
{MAC_SENDMSGTO_INVALID_MSG_ID,LOGWARNING,MAC_RRC_INF,\
"MAC_SENDMSGTO_INVALID_MSG_ID",\
{"MsgId","InternalCellId","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_UL_CSI_PUT_SRS_SINR_IN_QUEUE_ID,LOGDEBUG, MAC_UL_CSI,\
"MAC_UL_CSI_PUT_SRS_SINR_IN_QUEUE_ID", \
{"ue_index","SRS_REPORT_TYPE","START_PRB","NUM_OF_PRB","SINR_VALUE",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_UL_CSI_PUT_ULSCH_SINR_IN_QUEUE_ID,LOGDEBUG, MAC_UL_CSI,\
"MAC_UL_CSI_PUT_ULSCH_SINR_IN_QUEUE_ID", \
{"ue_index","SRS_REPORT_TYPE","START_PRB","NUM_OF_PRB","",\
"HARQ_PROC_ID","",\
FNAME,EVENT,\
}},
{MAC_UL_CSI_PROCESS_SINR_REPORT_ID,LOGDEBUG, MAC_UL_CSI,\
"MAC_UL_CSI_PROCESS_SINR_REPORT_ID", \
{"REPORT_TYPE","","","","",\
"","",\
FNAME,EVENT,\
}},/*190*/ 
{MAC_UL_CSI_PROCESS_SRS_SINR_ID,LOGDEBUG, MAC_UL_CSI,\
"MAC_UL_CSI_PROCESS_SRS_SINR_ID", \
{"START_PRB","NUM_OF_PRB","AVG_SINR_FROM_SRS_REPORT","AVG_SINR","mcs",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_UL_CSI_PROCESS_ULSCH_SINR_ID,LOGDEBUG, MAC_UL_CSI,\
"MAC_UL_CSI_PROCESS_ULSCH_SINR_ID", \
{"START_PRB","NUM_OF_PRB","SINR","AVG_SINR","mcs",\
"EFFECTIVE_MCS","",\
FNAME,EVENT,\
}}, 
{MAC_UL_CSI_UPDATE_MCS_TO_PRB_MAP_ID,LOGDEBUG, MAC_UL_CSI,\
"MAC_UL_CSI_UPDATE_MCS_TO_PRB_MAP_ID", \
{"START_PRB","NUM_OF_PRB","INPUT_MCS","HIGHEST_REPORTED_MCS",\
"LOWSET_REPORTED_MCS",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_UL_CSI_UPDATE_UE_UL_BLER_ID,LOGDEBUG, MAC_UL_CSI,\
"MAC_UL_CSI_UPDATE_UE_UL_BLER_ID", \
{"UE_BLER","NUM_CRC_ERRORS","NUM_GRANTS","","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_UL_CSI_INIT_CSI_MANAGER_ID,LOGINFO, MAC_UL_CSI,\
"MAC_UL_CSI_INIT_CSI_MANAGER_ID", \
{"","MAC_MAX_SINR_REPORTS_QUEUE_LENGTH","MAC_MCS_GROUPING_STRATEGY",\
"MAC_MCS_GROUP_SIZE","MAC_TARGET_BLER",\
"MAC_BLER_CORRECTION_PARAM_ALPHA","",\
FNAME,EVENT,\
}}, 
{MAC_RRC_INF_UE_UL_CSI_INIT_INFO_ID,LOGDEBUG, MAC_RRC_INF,\
"MAC_RRC_INF_UE_UL_CSI_INIT_INFO_ID", \
{"ue_index","mcs","AVG_SINR","PRB_MAP_VALUE",\
"FREQUENCY_SELECTIVE_SCHEDULING_UL",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_RRC_INF_MCS_TO_PRB_MAP_FREE_ID,LOGDEBUG, MAC_RRC_INF,\
"MAC_RRC_INF_MCS_TO_PRB_MAP_FREE_ID", \
{"ue_index","InternalCellId","","","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_L1_INF_SINR_QUEUE_FULL_ID,LOGWARNING, MAC_L1_INF,\
"MAC_L1_INF_SINR_QUEUE_FULL_ID", \
{"ue_index","RECV_TTI","","","",\
"","",\
FNAME,EVENT,\
}}, 
/* SPR 16766 fix Start */
{MAC_UL_STRATEGY_ERROR_ID,LOGERROR,MAC_UL_Strategy,\
"UL_STRATEGY_ERROR",\
{"ue_index","V1","V2","V3","V4",\
"","",\
FNAME,EVENT,\
}},
/* +- SPR 17779 */
{MAC_UL_STRATEGY_ERROR_ID_1,LOGDEBUG,MAC_UL_Strategy,\
"UL_STRATEGY_ERROR",\
{"ue_index","V1","V2","V3","V4",\
"","",\
FNAME,EVENT,\
}},
/* +- SPR 17779 */
/* SPR 16766 fix End */
{MAC_UL_MODIFY_UE_ID,LOGWARNING,MAC_UL_Strategy,\
"MAC_UL_MODIFY_UE",\
{"ModifiedUe","","","","",\
"","",\
FNAME,EVENT,\
}},/*200*/
/*ULA_UTP*/
{MAC_UL_STRATEGY_TX_NODE_ID,LOGDEBUG,MAC_UL_Strategy,\
"UL_STRATEGY_TX_NODE",\
{"ue_index","REQUIRED_RB","STRICT_ALLOCATED_RB_FLAG","MINDATASIZE",\
"MAXDATASIZE","AVGMCS","TX NODE TTI COUNT",\
FNAME,EVENT,\
}},
/*ULA_UTP*/
{MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
"MAC_FUNCTION_ENTRY",\
{"","","","","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
"MAC_FUNCTION_EXIT",\
{"","","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_FREEING_DLHARQ_ID,LOGDEBUG,MAC_DL_HARQ,\
"MAC_FREEING_DLHARQ_ID",\
{"ue_index","HarqId","","","",\
"","",\
FNAME,EVENT,\
}}, 
{DL_HARQ_RLC_MEM_FREE_AM_MODE_FALSE_HARQ_IND,LOGDEBUG,MAC_DL_HARQ,\
"DL_HARQ_RLC_MEM_FREE_AM_MODE_FALSE_HARQ_IND",\
{"LcId","RlcBuffer_p","rlc_mode","HarqFailureReporting","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_DL_HARQ_TIMER_START_INFO_1,LOGDEBUG,MAC_DL_HARQ,\
"MAC_DL_HARQ_TIMER_START_INFO_1",\
{"ue_index","HarqId","SN","ExpTick","TimerNode_p->isValid",\
"TimerArrIndex","TimerList_p->count",\
FNAME,EVENT,\
}}, 
{MAC_DL_HARQ_TIMER_START_INFO_2,LOGDEBUG,MAC_DL_HARQ,\
"MAC_DL_HARQ_TIMER_START_INFO_2",\
{"TimerNode_p->isValid","TimerArrIndex","TimerList_p->count","","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_TDD_TIMER_START_HARQ_SET_INFO,LOGDEBUG,MAC_DL_HARQ,\
"MAC_TDD_TIMER_START_HARQ_SET_INFO",\
{"currentSFNIndex","currentSubframe","HarqId","DlHarqRttExpiryTick","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_TDD_DL_HARQ_TIMER_START_INFO_1,LOGDEBUG,MAC_DL_HARQ,\
"MAC_TDD_DL_HARQ_TIMER_START_INFO_1",\
{"DL_data_expiry","dlAckNackSf","PdschTxBitMap[dlAckNackSf]","Bit_Set","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_TDD_DL_HARQ_TIMER_START_INFO_2,LOGDEBUG,MAC_DL_HARQ,\
"MAC_TDD_DL_HARQ_TIMER_START_INFO_2",\
{"Tx_sfn","Tx_sf","Node_P","ue_index","",\
"","",\
FNAME,EVENT,\
}},/*210*/ 
{MAC_TDD_TIMER_START_HARQ_NOT_FREED,LOGDEBUG,MAC_DL_HARQ,\
"MAC_TDD_TIMER_START_HARQ_NOT_FREED",\
{"HARQ ID NOT FREED","BIT MAP","Cur_SF","Cur_SFN","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_TDD_DL_HARQ_TIMER_NODE_ISVALID_IS_NOT_SET,LOGDEBUG,MAC_DL_HARQ,\
"MAC_TDD_DL_HARQ_TIMER_NODE_ISVALID_IS_NOT_SET",\
{"TimerNode_p->isValid","TimerList_p->count","TimerNodeArrrayIndex","FrameNumber","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_DL_HARQ_ENTITY_TIMER_ARRAY_COUNT_IS_ZERO,LOGDEBUG,MAC_DL_HARQ,\
"MAC_DL_HARQ_ENTITY_TIMER_ARRAY_COUNT_IS_ZERO",\
{"SubframeNumber","","","","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_EXPIRE_DL_HARQ_TIMER_NODE,LOGINFO,MAC_DL_HARQ,\
"DL HARQ Timer node is expired",\
{"SubframeNumber","tempHarqID","ue_index","isFreeHarqOnly","",\
"","",\
FNAME, "",\
}}, 
{MAC_DL_HARQ_ACK_NACK_QUEUE_COUNT,LOGDEBUG,MAC_DL_HARQ,\
"MAC_DL_HARQ_ACK_NACK_QUEUE_COUNT",\
{"ackNackQueueCount","","","","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_DL_HARQ_ACK_NACK_NODE_INFO,LOGBRIEF,MAC_DL_HARQ,\
"MAC_DL_HARQ_ACK_NACK_NODE_INFO",\
{"ue_index","Operation","SubFrameNumberk","","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_DL_HARQ_ACK_NACK_NODE_HARQ_INFO,LOGBRIEF,MAC_DL_HARQ,\
"MAC_DL_HARQ_ACK_NACK_NODE_HARQ_INFO",\
{"tempHarqID","DlHarqTimerArrayIndex","dlharqTimerNode_p->isValid","","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_QLOAD_ADDED_TO_NACK_QLOAD,LOGBRIEF,MAC_DL_HARQ,\
"MAC_QLOAD_ADDED_TO_NACK_QLOAD",\
{"TbSize","NackQLoad","","","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_NACK_QLOAD_CORRECTION_AFTER_FIRST_RETXN,LOGDEBUG,MAC_DL_HARQ,\
"MAC_NACK_QLOAD_CORRECTION_AFTER_FIRST_RETXN",\
{"TbSize","RetransmissionTBSize","NackQueueLoadCorrection","nackQLoad","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_PUT_ENTRY_IN_HARQ_ZERO_NON_ZERO_QLOAD_TRIGGER_QUEUE,LOGDEBUG,MAC_DL_HARQ,\
"MAC_PUT_ENTRY_IN_HARQ_ZERO_NON_ZERO_QLOAD_TRIGGER_QUEUE",\
{"ue_index","Flag","","","",\
"","",\
FNAME,EVENT,\
} },
{MAC_PUT_ENTRY_IN_DLSCHEDULER_NEWTX_QUEUE_ID,LOGBRIEF,MAC_DL_Strategy,\
"MAC_PUT_ENTRY_IN_DLSCHEDULER_NEWTX_QUEUE",\
{"ue_index","harq_proc_id","msgType","minTBsize","maxTBsize",\
"","",\
FNAME,"",\
} },
{MAC_PUT_ENTRY_IN_DLSCHEDULER_RETX_QUEUE_ID,LOGINFO,MAC_DL_HARQ,\
"MAC_PUT_ENTRY_IN_DLSCHEDULER_RETX_QUEUE",\
{"ue_index","harq_proc_id","msgType","tb1Valid","tb2Valid",\
"","",\
FNAME,"",\
} },
{MAC_PUT_ENTRY_IN_RETX_FAILURE_QUEUE_ID,LOGINFO,MAC_DL_HARQ,\
"MAC_PUT_ENTRY_IN_RETX_FAILURE_QUEUE",\
{"ue_index","harq_proc_id","msgType","tb1Valid","tb2Valid",\
"","",\
FNAME,"",\
} },
{MAC_PUT_ENTRY_IN_ULSCHEDULER_NEWTX_QUEUE_ID,LOGBRIEF,MAC_UL_Strategy,\
"MAC_PUT_ENTRY_IN_ULSCHEDULER_NEWTX_QUEUE",\
{"ue_index","harq_proc_id","requiredRB","minDatasize","maxDatasize",\
"","",\
FNAME,"",\
} },
{MAC_DL_HARQ_RETX_RI_ZERO_ID,LOGDEBUG,MAC_DL_HARQ,\
"Recieved RI LENGTH as ZERO in RETX",\
{"HarqId","ue_index","","","",\
"","",\
FNAME,EVENT\
}},
{MAC_UL_HARQ_ALREADY_BUSY_ID,LOGINFO, MAC_UL_HARQ,\
"MAC_UL_HARQ_ALREADY_BUSY",\
{"ue_index","HarqId","SF","","",
"","",\
FNAME,EVENT\
}},
{MAC_TDD_UL_HARQ_TIMER_START_ID,LOGBRIEF,MAC_UL_HARQ,\
"MAC_TDD_UL_HARQ_TIMER_START",\
{"ue_index","HarqId","SF","","",\
"","",\
FNAME,EVENT\
}}, 
{MAC_EXPIRE_UL_HARQ_TIMER_NODE_ID,LOGINFO,MAC_UL_HARQ,\
"MAC_EXPIRE_UL_HARQ_TIMER_NODE_ID",\
{"ue_index","tempPhyTTI","HarqId","","",\
"","",\
FNAME,EVENT\
}},
{MAC_PUSCH_RES_FAIL_FOR_NEW_TX_ID,LOGDEBUG,MAC_UL_HARQ,\
"PUSCH_RES_FAIL_FOR_NEW_TX",\
{"ue_index","REQRB","MINRB","RBMapNum","STRICTRBALLOC",\
"HarqId","",\
FNAME,EVENT\
}},
{MAC_UL_HARQ_RETX_MAX_TIME_ID,LOGINFO,MAC_UL_HARQ,\
"MAC_UL_HARQ_RETX_MAX_TIME",\
{"ue_index","harqId","currentRetxCnt","","",\
"","",
FNAME,""\
}},
{MAC_UL_HARQ_ADAP_RETX_ID,LOGBRIEF,MAC_UL_HARQ,\
"MAC_UL_HARQ_ADAP_RETX",\
{"ue_index","harqId","currentRetxCnt","","",\
"","",
FNAME,""\
}},
{MAC_UL_HARQ_NONADAP_RETX_ID,LOGBRIEF,MAC_UL_HARQ,\
"MAC_UL_HARQ_NONADAP_RETX",\
{"ue_index","harqId","currentRetxCnt","retxType","",\
"","",
FNAME,""\
}},
/* DRX_CHG */   
/* SPR 2377 changes start */
{MAC_UE_ADDED_IN_ACTIVE_LIST_IN_UL_LOG_ID,LOGBRIEF, MAC_UL_Strategy,\
"MAC_UE_ADDED_IN_ACTIVE_LIST_IN_UL_LOG_ID",\
{"ue_index", "TRIGGER","","","",\
"","",\
FNAME,"",\
}},   
{MAC_UE_ADDED_IN_ACTIVE_LIST_IN_DL_LOG_ID,LOGDEBUG, MAC_DL_Strategy,\
"MAC_UE_ADDED_IN_ACTIVE_LIST_IN_DL_LOG_ID",\
{"ue_index", "TRIGGER","","","",\
"","",\
FNAME,"",\
}},
{MAC_UE_DELETED_FROM_ACTIVE_LIST_IN_UL_LOG_ID,LOGBRIEF, MAC_UL_Strategy,\
"MAC_UE_DELETED_FROM_ACTIVE_LIST_IN_UL_LOG_ID",\
{"ue_index", "TRIGGER","","","",\
"","",\
FNAME,"",\
}}, 
{MAC_UE_DELETED_FROM_ACTIVE_LIST_IN_DL_LOG_ID,LOGDEBUG, MAC_DL_Strategy,\
"MAC_UE_DELETED_FROM_ACTIVE_LIST_IN_DL_LOG_ID",\
{"ue_index", "TRIGGER","LineNum","","",\
"","",\
FNAME,"",\
}},
{MAC_UE_NOT_SCHEDULED_IN_UL_LOG_ID,LOGDEBUG, MAC_UL_Strategy,\
"UE Not Scheduled In UL",\
{"ue_index", "DRX_STATE","MEAS_GAP_PERIOD","","",\
"","",\
FNAME,"",\
}},/*230*/
{MAC_UE_NOT_SCHEDULED_IN_DL_LOG_ID,LOGDEBUG, MAC_DL_Strategy,\
"UE Not Scheduled In DL",\
{"ue_index", "DRX_STATE","MEAS_GAP_PERIOD","","",\
"","",\
FNAME,"",\
}},
{MAC_UE_NOT_SCHEDULED_IN_PDCCH_LOG_ID,LOGDEBUG, MAC_PDCCH,\
"MAC_UE_NOT_SCHEDULED_IN_PDCCH_LOG_ID",\
{"ue_index", "DRX_STATE","MEAS_GAP_PERIOD","","",\
"","",\
FNAME,"",\
}},
/* SPR 2377 changes end */
{MAC_DRX_EVENT_TRIGGERED_LOG_ID,LOGDEBUG, MAC_DRX,\
"DRX Event Triggered",\
{"ue_index", "Event","State","SubState","NextCycle",\
"","",\
FNAME,"",\
}},
{MAC_DRX_EVENT_HANDLED_LOG_ID,LOGDEBUG, MAC_DRX,\
"DRX Event Handling Complete",\
{"State", "SubState","NextCycle","SHORT_ONDURATION_TTI","LONG_ONDURATION_TTI",\
"","",\
FNAME,"",\
}},
{MAC_DRX_TIMER_EVENT_LOG_ID,LOGDEBUG, MAC_DRX,\
"DRX Timer Event Occured",\
{"ue_index", "Event","ExpTTI","","",\
"","",\
FNAME,"",\
}},
{MAC_DRX_NEXT_ONDURATION_TTI_CAL_LOG_ID,LOGDEBUG, MAC_DRX,\
"DRX Next OnDurationTTI Calculation",\
{"CYCLE", "NUM_TICKS_EXPIRED","NUM_CYCLES_EXPIRED","NEXT_ONDURATION_TTI","",\
"","",\
FNAME,"",\
}},
{MAC_DRX_INVALID_EVENT_LOG_ID,LOGDEBUG, MAC_DRX,\
"DRX Invalid Event Occured",\
{"Event","State","","","",\
"","",\
FNAME,"",\
}},
/* DRX_CHG */    
/*DRX_MEAS_CHG*/
{MAC_TAG_RECEIVED_ID,LOGDEBUG,MAC_RRC_INF,\
"Tag received during Parsing",\
{"LineNum","ue_index","TagValue","InternalCellId","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_INVALID_PARAMETER_RECEIVED_ID,LOGWARNING,MAC_RRC_INF,\
"Invalid parameter received during Parsing",\
{"ue_index","Value","InternalCellId","","",\
"","",\
FNAME,EVENT,\
}}, 
/*MULTI_SECTOR changes starts*/
{RRC_MAC_MSG_CELL_INDEX_ERR,LOGWARNING,MAC_RRC_INF,\
"RRC API cellIndex is different from self cellIndex",\
{"cell_index","Selfcell_index","","","",\
"","",\
FNAME,EVENT,\
}}, 
/*MULTI_SECTOR changes ends*/
{MAC_TAG_PARSE_FAILURE_ID,LOGWARNING,MAC_RRC_INF,\
"Parsing failed",\
{"LineNum","ue_index","TagValue","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},/*240*/ 
{MAC_UE_CREATE_SUCCESS_ID,LOGINFO,MAC_RRC_INF,\
"UE created successfully",\
{"MsgId","RetVal","MsgLen","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{MAC_UE_RECONFIGURE_SUCCESS_ID,LOGINFO,MAC_RRC_INF,\
"UE reconfigured successfully",\
{"MsgId","","MsgLen","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
/*DRX_MEAS_CHG*/
/* MEAS_GAP_CHG*/
{MAC_MEAS_EVENT_HANDLER_ID,LOGDEBUG,MAC_MEAS_HANDLER,\
"MAC_MEAS_HANDLER_LOG_ID",\
{"ue_index","","","","",\
"","",\
FNAME,EVENT,\
} },
{MAC_MEAS_MAP_HANDLER_ID,LOGDEBUG,MAC_MEAS_HANDLER,\
"MAC_MEAS_MAP_LOG_ID",\
{"ue_index","DlStart","DlEnd","UlStart",\
"UlEnd","","",\
FNAME,EVENT,\
} },
{MAC_MEAS_TRIGGER_HANDLER_ID,LOGDEBUG,MAC_MEAS_HANDLER,\
"MAC_MEAS_TRIGGER_LOG_ID",\
{"ue_index","TRIGGER","","","",\
"","",\
FNAME,EVENT,\
} },
/* MEAS_GAP_CHG*/
/*RETX_CHG*/
{DRX_UE_NACK_RECIEVED_LOG_ID,LOGINFO, MAC_DL_HARQ,\
"UE Nack Recieved",\
{"ue_index","HarqId","DRXRetxExpiryTTI","","",\
"","",\
FNAME,EVENT,\
} },
{DRX_RETX_UE_NOT_SCHEDULED_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"Error in UE retransmission Scheduling",\
{"ue_index","HarqId","ALREADY Considered Flag","PENDING DELETE FLAG","",\
"","",\
FNAME,EVENT,\
} },
{DRX_RETX_UE_SCHEDULED_LOG_ID,LOGINFO, MAC_DL_Strategy,\
"Strategy Node of UE created",\
{"ue_index","HarqId","Transmission Type","",\
"","",\
FNAME,"",\
} },
{DRX_HARQ_RETX_TIMER_EXPIRE_LOG_ID,LOGINFO, MAC_DL_Strategy,\
"Harq/retransmission timer expiry",\
{"ue_index","HarqId","HARQ EXPIRY TTI",\
"RETRANSMISSION EXPIRY TTI","",\
"","",\
FNAME,EVENT,\
} },
/*RETX_CHG*/
/*UT_LOGS_DRX_MEAS*/
{MAC_MEAS_DRX_INITIALISATION_ID,LOGINFO,MAC_DRX,\
"MEAS/DRX INIT",\
{"","","","","",\
"","",\
FNAME,EVENT,\
}},/*250*/
{MAC_DRX_DELETE_TIMERS_LOG_ID,LOGINFO,MAC_DRX,\
"Deleting Timers",\
{"ue_index","DlConfigType",\
"UlConfigType","",\
"","","",\
FNAME,EVENT,\
}},
/*----LOG id  added for l3,oam msgs----*/ 
{OAM_MAC_MSG_ID,LOGWARNING,MAC_OAM_INF,\
"Received Msg from OAM successfully",\
{"DestModuleId","Trans_Id","bytesRead","InternalCellId","MsgId",\
"","",\
FNAME,EVENT,\
}},
{RRC_PHY_CELL_CONFIG_REQ_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_PHY_CELL_CONFIG_REQ",\
{"DestModuleId","MsgId","","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_PHY_CREATE_UE_REQ_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_PHY_CREATE_UE_REQ",\
{"DestModuleId","MsgId","InternalCellId","","",\
"","",\
FNAME,EVENT,\
}},
{RRC_PHY_RECONFIG_UE_ENTITY_REQ_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_PHY_RECONFIG_UE_ENTITY_REQ",\
{"DestModuleId","MsgId","InternalCellId","","",\
"","",\
FNAME,EVENT,\
}},
{RRC_PHY_DELETE_UE_ENTITY_REQ_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_PHY_DELETE_UE_ENTITY_REQ",\
{"DestModuleId","MsgId","InternalCellId","","",\
"","",\
FNAME,EVENT,\
}},
{RRC_PHY_CELL_RECONFIG_REQ_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_PHY_CELL_RECONFIG_REQ",\
{"DestModuleId","MsgId","InternalCellId","","",\
"","",\
FNAME,EVENT,\
}},
{RRC_PHY_CELL_RECONFIG_RES_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_PHY_CELL_RECONFIG_RES",\
{"RRC cell_index","RspCode","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_CONFIG_CELL_REQ_ID,LOGWARNING,MAC_RRC_INF,\
"Config Cell Request for MAC",\
{"DestModuleId","MsgId","MsgLen","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{MAC_DL_Q_LOAD_ID,LOGINFO,MAC_TB_DL,\
"UeQueueLoad Update",\
{"ue_index","crnti","OldUeQLoad","NewUeQLoad","",\
"","",\
FNAME,EVENT,\
}},/*260*/
{RRC_CHANGE_CRNTI_MSG_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_CHANGE_CRNTI_MSG_ID",\
{"MsgId","ue_index", "response", "OldCrnti",\
 "NewCrnti","","",\
FNAME,EVENT,\
}},
{RRC_HO_RACH_RESOURCE_REQ_MSG_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_HO_RACH_RESOURCE_REQ_MSG_ID",\
{"MsgId", "ue_index", "RspCode", "TCRNTI"\
, "PreambleId","","",\
FNAME,EVENT,\
}}, 
{RRC_UE_INACTIVE_TIME_MSG_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_UE_INACTIVE_TIME_MSG_ID",\
{"MsgId", "ue_index", "RspCode", "UeInactiveTime", "InternalCellId",\
"","",\
FNAME,EVENT,\
}},
{RRC_HO_INVALID_CRNTI,LOGWARNING,MAC_RRC_INF,\
"RRC_HO_INVALID_CRNTI",\
{"MsgId","ue_index","crnti","lowerRNTIBound_g","upperRNTIBound_g",\
"","",\
FNAME,EVENT,\
}},
{RRC_HO_REL_RACH_RESOURCE_IND,LOGWARNING,MAC_RRC_INF,\
"RRC_HO_REL_RACH_RESOURCE_IND",\
{"MsgId", "ue_index","RspCode", "crnti", "InternalCellId",\
 "","",\
FNAME,EVENT,\
}}, 
{MAC_RLF_IND_MSG_ID,LOGWARNING,MAC_RRC_INF,\
"MAC_RLF_IND_MSG_ID",\
{"MsgId","ue_index","RlfCause","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{MAC_SEND_AUDIT_REPORT_MSG_ID,LOGINFO,MAC_RRC_INF,\
"MAC_SEND_AUDIT_REPORT_MSG_ID",\
{"ue_index","NumOfInactiveUe","InactiveTime","InternalCellId","",\
"",""\
FNAME,EVENT,\
}}, 
{RRC_INVALID_CONTEXT_ID,LOGWARNING,MAC_RRC_INF,\
"RRC_INVALID_CONTEXT_ID",\
{"ue_index","Trans_Id","InternalCellId","","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_PDCCH_ORDER_TRIGGERED,LOGWARNING,MAC_PDCCH,\
"MAC_PDCCH_ORDER_TRIGGERED",\
{"PrembleIdx","pdcchOrderSentCount","allocatedCCEIdx","ExpiryTTI","ue_index",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_UL_SYNC_LOSS_DETECTED,LOGWARNING,MAC_PDCCH,\
"MAC_UL_SYNC_LOSS_DETECTED",\
{"ulSyncTimer","lastTASentTick","ulSyncLossState","ue_index","",\
"","",\
FNAME,EVENT,\
}},/*270*/ 
{MAC_TRANSMITTER_DCI_1A,LOGDEBUG,MAC_L1_INF,\
"MAC_TRANSMITTER_DCI_1A",\
{"PreambleIdx","prachMaskIdx","RNTIType","allocatePRACHFlag","HarqProcessNum",\
"","",\
FNAME,EVENT,\
}},
{MAC_UE_DELETED_FROM_ACTIVE_LIST_UL_SYNC_LOSS,LOGWARNING,MAC_DL_Strategy,\
"MAC_UE_DELETED_FROM_ACTIVE_LIST_UL_SYNC_LOSS",\
{"ue_index","ON_OFF_TRIGGER","","","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_UE_ADDED_IN_ACTIVE_LIST_UL_SYNC_LOSS,LOGWARNING,MAC_DL_Strategy,\
"MAC_UE_ADDED_IN_ACTIVE_LIST_UL_SYNC_LOSS",\
{"ue_index","ON_OFF_TRIGGER","LineNum","","",\
"","",\
FNAME,EVENT,\
}}, 
{RA_RNTI_PROC_RAR_NOT_FOUND,LOGWARNING,MAC_RACH,\
"RA_RNTI_PROC_RAR_NOT_FOUND",\
{"RARCNT","RARNTI","LineNum","","",\
"","",\
FNAME,EVENT,\
}},
{RA_REQ_PROC_ID,LOGBRIEF,MAC_RACH,\
"RA_REQ_PROC_ID",\
{"RARNTI","RecvTTI","PreambleId","MAXRAR","FREERB",\
"","",\
FNAME,EVENT,\
}},
{RAR_PDU_PROC_ID,LOGDEBUG,MAC_RACH,\
"RAR_PDU_PROC_ID",\
{"RARPDUCNT","RACNT","RARNTI","BIFLAG","LineNum",\
 "","",\
FNAME,EVENT,\
}},
{RAR_RARNTI_MAP_ID,LOGDEBUG,MAC_RACH,\
"RAR_RARNTI_MAP_ID",\
{"RARNTI","RARNTIBITMAP1","RARNTIBITMAP2","INFO1","INFO2",\
 "","",\
FNAME,EVENT,\
}},
{PREAMBLE_TIMER_EXPIRY_ID,LOGINFO,MAC_RACH,\
"PREAMBLE_TIMER_EXPIRY_ID",\
{"PreambleId","TRIGGERFLAG","ue_index","PDCCHORDERCNT","ExpTTI",\
"","",\
FNAME,EVENT,\
}},
/* SPR 16773 fix start */
{PREAMBLE_TIMER_STOP_ID,LOGDEBUG,MAC_RACH,\
"Timer is stopped",\
{"INFO1","INFO2","LineNum","","",\
"","",\
FNAME, "Timer",\
}},
/* SPR 16773 fix end */
{PREAMBLE_TIMER_START_ID,LOGDEBUG,MAC_RACH,\
"PREAMBLE_TIMER_START_ID",\
{"PreambleId","TRIGGERFLAG","ue_index","LineNum","ExpTTI",\
"","",\
FNAME,EVENT,\
}},/*280*/
{CREATE_DL_UE_CONTXT_HO_ID,LOGWARNING,MAC_RRC_INF,\
"CREATE_DL_UE_CONTXT_HO_ID",\
{"TCRNTI","ue_index","PreambleId","Flag","InternalCellId",\
"","",\
FNAME,EVENT,\
}},
{DEL_DL_UE_CONTXT_HO_ID,LOGWARNING,MAC_RRC_INF,\
"DEL_DL_UE_CONTXT_HO_ID",\
{"ue_index","PreambleId","InternalCellId","","",\
"","",\
FNAME,EVENT,\
}},
{PREAMBLE_NOT_BUSY_ID,LOGERROR,MAC_RACH,\
"PREAMBLE_NOT_BUSY_ID",\
{"ue_index","crnti","PreambleId","LineNum","InternalCellId",\
"","",\
FNAME,EVENT,\
}},
{RECVD_INVALID_PREAMBLE,LOGERROR,MAC_RACH,\
"RECVD_INVALID_PREAMBLE",\
{"PreambleId","INFO1","INFO2","INFO3","LineNum",\
"","",\
FNAME,EVENT,\
}},
{NO_HO_DL_UE_CONTEXT_ID,LOGERROR,MAC_RACH,\
"NO_HO_DL_UE_CONTEXT_ID",\
{"ue_index","crnti","LineNum","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_PDCCH_ORDER_TRIGGER_FAILURE,LOGWARNING,MAC_PDCCH,\
"MAC_PDCCH_ORDER_TRIGGER_FAILURE",\
{"ue_index","DlUECtxt->PreambleIdx","AllocPreambleIdx","PdcchOrderSentCount","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_QUEUE_NODE_NOT_PROCESSED,LOGDEBUG,MAC_DL_Strategy,\
"MAC_QUEUE_NODE_NOT_PROCESSED",\
{"PendingDeleteFlag","","","","",\
"","",\
FNAME,EVENT,\
}},
{INIT_PREAMBLE_POOL_FAIL_ID,LOGWARNING,MAC_RACH,\
"INIT_PREAMBLE_POOL_FAIL_ID",\
{"PreambleId","","","","",\
"","",\
FNAME,EVENT,\
}},
{INIT_PREAMBLE_POOL_ID,LOGDEBUG,MAC_RACH,\
"INIT_PREAMBLE_POOL_ID",\
{"currNumOfFreePreambles","FINDEX","LINDEX","","",\
 "","",\
FNAME,EVENT,\
}},
{BEFORE_ALLOC_PREAMBLE_ID,LOGBRIEF,MAC_RACH,\
"BEFORE_ALLOC_PREAMBLE_ID",\
{"BcurrNumOfFreePreambles","BFINDEX","BLINDEX","","",\
FNAME,EVENT,\
}},/*290*/
{AFTER_ALLOC_PREAMBLE_ID,LOGBRIEF,MAC_RACH,\
"AFTER_ALLOC_PREAMBLE_ID",\
{"AcurrNumOfFreePreambles","AFINDEX","ALINDEX","","",\
FNAME,EVENT,\
}},
{ALLOC_PREAMBLE_UE_ID,LOGWARNING,MAC_RACH,\
"ALLOC_PREAMBLE_UE_ID",\
{"ue_index","Flag","preambleIndex","","",\
 "","",\
FNAME,EVENT,\
}},
{ALLOC_PREAMBLE_FAIL_ID,LOGWARNING,MAC_RACH,\
"ALLOC_PREAMBLE_FAIL_ID",\
{"currNumOfFreePreambles","FINDEX","LINDEX","ue_index","Flag",
 "LineNum","",\
FNAME,EVENT,\
}},
{FREE_PREAMBLE_FAIL_ID,LOGWARNING,MAC_RACH,\
"FREE_PREAMBLE_FAIL_ID",\
{"currNumOfFreePreambles","FINDEX","LINDEX","LineNum",""
"","",\
FNAME,EVENT,\
}},
{FREE_PREAMBLE_UE_ID,LOGWARNING,MAC_RACH,\
"FREE_PREAMBLE_UE_ID",\
{"ue_index","Flag","preambleIndex","","",
 "","",\
FNAME,EVENT,\
}},
{BEFORE_FREE_PREAMBLE_INFO_ID,LOGBRIEF,MAC_RACH,\
"BEFORE_FREE_PREAMBLE_INFO_ID",\
{"BcurrNumOfFreePreambles","BFINDEX","BLINDEX","","",\
"","",\
FNAME,EVENT,\
}},
{AFTER_FREE_PREAMBLE_INFO_ID,LOGBRIEF,MAC_RACH,\
"AFTER_FREE_PREAMBLE_INFO_ID",\
{"AcurrNumOfFreePreambles","AFINDEX","ALINDEX","",\
"","",\
FNAME,EVENT,\
}},
{LOGICAL_CHAIN_ID,LOGDEBUG,MAC_RACH,\
"LOGICAL_CHAIN_ID",\
{"CALC_PREAMBLE_CNT","FREE_PREAMBLE_CNT","FINDEX",\
"LINDEX","TINDEX",\
"","",\
FNAME,EVENT,\
}},
{MAC_MAX_RB_FOR_RAR_ID,LOGDEBUG,MAC_RACH,\
"MAC_MAX_RB_FOR_RAR_ID",\
{"MAXRAR","RBMapNum","FREERB",\
"FREERARRB","",\
"","",\
FNAME,EVENT,\
}},
{MAC_RA_REQ_CNT_ID,LOGDEBUG,MAC_RACH,\
"MAC_RA_REQ_CNT_ID",\
{"MAXRAR","RBMapNum","CBRCNT","NEGCNT","CFRCNT",\
 "","",\
FNAME,EVENT,\
}},/*300*/
{RA_REQ_FREE_ID,LOGDEBUG,MAC_RACH,\
"RA_REQ_FREE_ID",\
{"RARNTI","RecvTTI","PreambleId","LineNum","INFO",\
 "","",\
FNAME,EVENT,\
}},
{PREAMBLE_NO_UECONTEXT_ID,LOGERROR,MAC_RACH,\
"PREAMBLE_NO_UECONTEXT_ID",\
{"PreambleId","Flag","ue_index","LineNum","",\
"","",\
FNAME,EVENT,\
}},
{CBR_RA_REQ_PREAMBLE_MAP_ID,LOGDEBUG,MAC_RACH,\
"CBR_RA_REQ_PREAMBLE_MAP_ID",\
{"TCRNTI","RecvTTI","PreambleId","PREMABLEMAP1","PREAMBLEMAP2",\
"","",\
FNAME,EVENT,\
}},
{RAR_PDU_FREE_ID,LOGDEBUG,MAC_RACH,\
"RAR_PDU_FREE_ID",\
{"RARNTI","RACNT","BIFLAG","ExpTTI","MSG3TTI",\
"LineNum","",\
FNAME,EVENT,\
}},
{MAC_TCRNTI_FAIL_ID,LOGWARNING,MAC_RACH,\
"MAC_TCRNTI_FAIL_ID",\
{"TCRNTI","LineNum","","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RA_REQ_RB_ALLOC_ID,LOGDEBUG,MAC_RACH,\
"RA_REQ_RB_ALLOC_ID",\
{"RARNTI","PreambleId","TCRNTI","RBSTART","RBLENGTH",\
 "","",\
FNAME,EVENT,\
}},
{RAR_SUBHEADER_VALUES_ID,LOGDEBUG,MAC_RACH,\
"RAR_SUBHEADER_VALUES_ID",\
{"SUBHDR1","SUBHDR2","SUBHDR3","SUBHDR4","",\
"","",\
FNAME,EVENT,\
}},
{RAR1_PAYLOAD_VALUES_ID,LOGDEBUG,MAC_RACH,\
"RAR1_PAYLOAD_VALUES_ID",\
{"BYTE1","BYTE2","BYTE3","BYTE4","BYTE5_6",\
"","",\
FNAME,EVENT,\
}},
{RAR2_PAYLOAD_VALUES_ID,LOGDEBUG,MAC_RACH,\
"RAR2_PAYLOAD_VALUES_ID",\
{"BYTE1","BYTE2","BYTE3","BYTE4","BYTE5_6",\
"","",\
FNAME,EVENT,\
}},
{RAR3_PAYLOAD_VALUES_ID,LOGDEBUG,MAC_RACH,\
"RAR3_PAYLOAD_VALUES_ID",\
{"BYTE1","BYTE2","BYTE3","BYTE4","BYTE5_6",\
"","",\
FNAME,EVENT,\
}},/*310*/
{MAC_UECONTEXT_FAIL_ID,LOGWARNING,MAC_RRC_INF,\
"MAC_UECONTEXT_FAIL_ID",\
{"ue_index","LineNum","InternalCellId","BYTE4","BYTE5_6",\
"","",\
FNAME,EVENT,\
}},
{CREATE_DL_UE_CONTXT_ID,LOGINFO,MAC_RRC_INF,\
"CREATE_DL_UE_CONTXT_ID",\
{"ue_index","PreambleId","Flag","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{MSG3_CRNTI_RECVD_ID,LOGWARNING,MAC_RACH,\
"MSG3_CRNTI_RECVD_ID",\
{"crnti","ue_index","TCRNTI","UETABEFOREUPDATE","UETAAFTERUPDATE",\
"","",\
FNAME,"",\
}},
{MSG3_CRNTI_TEMPCTXT_DEL_ID,LOGWARNING,MAC_L1_INF,\
"MSG3_CRNTI_TEMPCTXT_DEL_ID",\
{"crnti","TCRNTI","TEMPCTXTPOIN","CCCHFLAG","",\
"","",\
FNAME,EVENT,\
}},
{PREAMBLE_TIMER_DELETE_ID,LOGDEBUG,MAC_RACH,\
"PREAMBLE_TIMER_DELETE_ID",\
{"PreambleId","LineNum","","","",\
"","",\
FNAME,EVENT,\
}},
{PREAMBLE_TIMER_NOCONTEXT_ID,LOGWARNING,MAC_RACH,\
"PREAMBLE_TIMER_NOCONTEXT_ID",\
{"PreambleId","Flag","ue_index","ExpTTI","LineNum",\
"","",\
FNAME,EVENT,\
}},  
{MAC_HO_RACH_RESOURCE_RESP_MSG_ID,LOGINFO,MAC_RRC_INF,\
"MAC_HO_RACH_RESOURCE_RESP_MSG_ID",\
{"MsgId", "ue_index", "RspCode", "Tag", "TagLen",\
"", "",\
FNAME,EVENT,\
}}, 
{MAC_INACTIVE_UES_IND_MSG_ID,LOGINFO,MAC_RRC_INF,\
"MAC_INACTIVE_UES_IND_MSG_ID",\
{"MsgId", "ue_index", "", "InactiveUeInfo", "InactiveUeInfoLen",\
"", "",\
FNAME,EVENT,\
}},
{MAC_RECONFIG_CELL_PARAMS_INFO,LOGINFO,MAC_RRC_INF,\
"MAC_RECONFIG_CELL_PARAMS_INFO",\
{"MaxInActivityTime_g", "cfrTimer", "","","",\
"","",\
FNAME,EVENT,\
}}, 
{INVALID_RACH_CONFIG,LOGWARNING,MAC_RRC_INF,\
"INVALID_RACH_CONFIG",\
{"NumCountBasedPreambles","numOfPreamblesInGroupA","MessageSizeGroupA","LineNum","InternalCellId",\
"","",\
FNAME,EVENT,\
}},/*320*/ 
{MAC_CONFIG_CELL_PARAMS_INFO,LOGINFO,MAC_RRC_INF,\
"MAC_CONFIG_CELL_PARAMS_INFO",\
{"MaxInActivityTime", "ContnFreeRachTimer", "NumOfContnBasedPreamble", "InternalCellId","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_INAVLID_TAG_MSG_ID,LOGWARNING,MAC_RRC_INF,\
"MAC_INAVLID_TAG_MSG_ID",\
{"Tag", "TagLen", "InternalCellId","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_INVALID_TIMER_VAL_MSG_ID,LOGWARNING,MAC_RRC_INF,\
"MAC_INVALID_TIMER_VAL_MSG_ID",\
{"Tag", "TagLen", "ueInactiveTimeVal", "contnFreeRachTimer","",\
"InternalCellId", "",\
FNAME,EVENT,\
}},
{MAC_TIMER_VAL_MSG_ID,LOGINFO,MAC_RRC_INF,\
"MAC_TIMER_VAL_MSG_ID",\
{"Tag", "TagLen", "UeInactiveTime", "ContnFreeRachTimer","InternalCellId",\
"", "",\
FNAME,EVENT,\
}},
{MAC_HO_RACH_RESOURCE_RESP_PREAMBLE_INFO,LOGINFO,MAC_RRC_INF,\
"MAC_HO_RACH_RESOURCE_RESP_PREAMBLE_INFO",\
{"Tag", "TagLen", "PreambleId", "PrachMaskIndex", "InternalCellId",\
"", "",\
FNAME,EVENT,\
}}, 
{MAC_INVALID_TA_ULSYNC_TIMER_VAL,LOGWARNING,MAC_RRC_INF,\
"MAC_INVALID_TA_ULSYNC_TIMER_VAL",\
{"timeAlignmentTimer", "ulSyncTimer", "InternalCellId", "", "",\
"", "",\
FNAME,EVENT,\
}},
{MAC_TIMEALIGNMENT_ULSYNC_TIMER_VAL,LOGINFO,MAC_RRC_INF,\
"MAC_TIMEALIGNMENT_ULSYNC_TIMER_VAL",\
{"TimeAlignmentTimer", "UlSyncTimer", "InternalCellId", "", "",\
"", "",\
FNAME,EVENT,\
}},
{MAC_UE_INACTIVE_TIME_RESP_MSG_ID,LOGINFO,MAC_RRC_INF,\
"MAC_UE_INACTIVE_TIME_RESP_MSG_ID",\
{"MsgId", "ue_index", "RspCode", "Tag", "TagLen",\
"", "",\
FNAME,EVENT,\
}}, 
{MAC_INVALID_NODE,LOGWARNING,MAC_RRC_INF,\
"MAC_INVALID_NODE",\
{"ResetUeQueueCount", "InternalCellId", "", "", "",\
"", "",\
FNAME,EVENT,\
}},
{MAC_INAVLID_LEN_MSG_ID,LOGERROR,MAC_RRC_INF,\
"MAC_INAVLID_LEN_MSG_ID",\
{"MsgId", "MsgLen", "InternalCellId", "", "",\
"", "",\
FNAME,EVENT,\
}},/*330*/
{MAC_RESET_NODE_PARAM_VAL,LOGINFO,MAC_RRC_INF,\
"MAC_RESET_NODE_PARAM_VAL",\
{"ue_index", "Trans_Id", "TTI", "InternalCellId", "",\
"", "",\
FNAME,EVENT,\
}}, 
{PROCESS_PENDING_RESET_UE_PARAMS,LOGINFO,MAC_RRC_INF,\
"PROCESS_PENDING_RESET_UE_PARAMS",\
{"ue_index", "Trans_Id", "ResetUeQueueCount", "InternalCellId", "",\
"", "", \
FNAME,EVENT,\
}},
{PREAMBLE_POINTER_NULL,LOGWARNING,MAC_RACH,\
"PREAMBLE_POINTER_NULL",\
{"PreambleId","FINDEX","LINDEX","NUMPREAMBLES", "LineNum",\
"", "", \
FNAME,EVENT,\
}},
{CFR_ULHARQ_NOT_FREE,LOGDEBUG,MAC_RACH,\
"CFR_ULHARQ_NOT_FREE",\
{"ue_index","HarqId","RiStart","NumOfRB","RIV",\
"", "", \
FNAME,EVENT,\
}},
{DLUECONTEXT_VALUE_MSIMATCH_ID,LOGERROR,MAC_RACH,\
"CONTEXT_VALUE_MSIMATCH",\
{"ue_index","TCRNTI","CONCRNTI","TCRNTIFLAG","LineNum",\
"","", \
FNAME,EVENT,\
}},
{INVALID_PREAMBLE_INDEX_ID,LOGINFO,MAC_RACH,\
"INVALID_PREAMBLE_INDEX_ID",\
{"PreambleId","ue_index","crnti","LineNum","InternalCellId",\
"","",\
FNAME,EVENT,\
}},
{CFR_RA_REQ_FREE_ID,LOGERROR,MAC_RACH,\
"CFR_RA_REQ_FREE_ID",\
{"RARNTI","RecvTTI","PreambleId","TMCS","LineNum",\
"","",\
FNAME,EVENT,\
}},
{RRC_CHANGE_CRNTI_INVALID_PARAM,LOGWARNING,MAC_RRC_INF,\
"RRC_CHANGE_CRNTI_INVALID_PARAM",\
{"ue_index", "RspCode", "OldCrnti","NewCrnti", "InternalCellId",\
"", "",\
FNAME,EVENT,\
}},
{DEMUX_TCRNTI_MSG_ID,LOGINFO,MAC_RACH,\
"DEMUX_TCRNTI_MSG",\
{"TCRNTI","status","length","lchId","",\
"","",\
FNAME,"",\
}},
{MSG3_DATA_MSG_NOCRNTI_CE_ID,LOGERROR,MAC_RACH,\
"MSG3_DATA_MSG_NOCRNTI_CE_ID",\
{"","","","","",\
"","",\
FNAME,EVENT,\
}},
{MSG3_CRNTI_INCORRECT_STATE_ID,LOGERROR,MAC_RACH,\
"MSG3_CRNTI_INCORRECT_STATE_ID",\
{"crnti", "RntiFlag", "PENDINGDEL","LcId", "",\
"", "",\
FNAME,EVENT,\
}},/*340*/
{MSG3_CRNTI_DATA_RECVD_ID,LOGBRIEF,MAC_RACH,\
"MSG3_CRNTI_DATA_RECVD_ID",\
{"crnti", "ue_index", "LcId","Length", "",\
"", "",\
FNAME,EVENT,\
}},
{MAC_UE_DATA_IND_ERR_ID,LOGERROR,MAC_RACH,\
"MAC_UE_DATA_IND_ERR_ID",\
{"crnti", "ue_index", "","", "",\
"", "",\
FNAME,EVENT,\
}}, 
{PREAMBLE_GROUP_A_CONFIG_INFO,LOGINFO,MAC_RRC_INF,\
"PREAMBLE_GROUP_A_CONFIG_INFO",\
{"countOfContentionBasedRAPreambles","NumOfPreamblesInGroupA",\
 "MessageSizeGroupA","McsIdForMSG3GroupARACHRequest",\
 "NumRBReqdForMSG3GroupARACHRequest",\
"","",\
FNAME,EVENT,\
}}, 
{PREAMBLE_GROUP_B_CONFIG_INFO,LOGINFO,MAC_RRC_INF,\
"PREAMBLE_GROUP_B_CONFIG_INFO",\
{"PreambleGroupBConfigPresent","McsIdForMSG3GroupBRACHRequest",\
 "NumRBReqdForMSG3GroupBRACHRequest","numOfPreamblesInGroupB",\
"MinimumRBReqdForSingleRAR",\
"","",\
FNAME,EVENT,\
}},  
{CONTENTION_FREE_PREAMBLE_CONFIG,LOGINFO,MAC_RRC_INF,\
"CONTENTION_FREE_PREAMBLE_CONFIG",\
{"McsIdForConFreeHandOver","NumRBReqdForConFreeHandOver",\
 "McsIdForConFreePDCCHOrder","NumRBReqdForConFreePDCCHOrder",\
 "MaxRBReqdForAnyRAR",\
"","",\
FNAME,EVENT,\
}},
{MAC_UL_SYNC_ON, LOGINFO, MAC_PDCCH,\
"MAC_UL_SYNC_ON",\
{"lastSyncDetectedTick", "lastTASentTick", "ulSyncLossState", "ue_index",\
"LineNum",\
"", "",\
FNAME,EVENT,\
}}, 
{PDCCH_ORDER_TIMER_START_ID, LOGBRIEF, MAC_RACH,\
"PDCCH_ORDER_TIMER_START_ID",\
{"ue_index", "ExpTTI", "LineNum", "", "",\
"", "",\
FNAME,EVENT,\
}},
{PDCCH_ORDER_TIMER_STOP_ID, LOGBRIEF, MAC_RACH,\
"PDCCH_ORDER_TIMER_STOP_ID",\
{"ue_index", "ExpTTI", "LineNum", "", "",\
"", "",\
FNAME,EVENT,\
}},
{PDCCH_ORDER_TIMER_NOCONTEXT_ID, LOGWARNING, MAC_RACH,\
"PDCCH_ORDER_TIMER_NOCONTEXT_ID",\
{"ue_index", "ExpTTI", "LineNum", "", "",\
"", "",\
FNAME,EVENT,\
}},
{PDCCH_ORDER_TIMER_EXPIRY_ID, LOGINFO, MAC_RACH,\
"PDCCH_ORDER_TIMER_EXPIRY_ID",\
{"ue_index", "ExpTTI", "LineNum", "", "",\
"", "",\
FNAME,EVENT,\
}},/*350*/ 
{MAC_PDCCH_ORDER_PARAM, LOGDEBUG, MAC_PDCCH,\
"MAC_PDCCH_ORDER_PARAM",\
{"ue_index", "DrxState","MeasurementGap","ulSyncLossState",\
"totalPdcchOrderQCount",\
"", "",\
FNAME,EVENT,\
}},
{MAC_UL_SYNC_LOSS_DRX_MEAS, LOGDEBUG, MAC_DL_Strategy,\
"MAC_UL_SYNC_LOSS_DRX_MEAS",\
{"ue_index", "OnOffTrigger", "ulSyncLossState",\
"PendingPDCCHOrder","",\
"", "",\
FNAME,EVENT,\
}},
{MAC_TA_INSERTION, LOGDEBUG,MAC_L1_INF,\
"MAC_TA_INSERTION",\
{"CurTick", "lastTASentTick", "ue_index", "timingAdvance","",\
"", "",\
FNAME,EVENT,\
}}, 
{MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE, LOGDEBUG,MAC_DL_Strategy,\
"MAC_ULSYNCLOSS_DRX_MEASUREMENTGAP_STATE",\
{"ue_index", "ulSyncloss", "DrxState", "MeasurementGap","LineNum",\
"", "",\
FNAME,EVENT,\
}},    
/* SPR 15496 Fix Start */
{MAC_CELL_DELETE_REQ_ID,LOGINFO,MAC_RRC_INF,\
"MAC_CELL_DELETE_FAIL",\
{"FAILREASON","VAL1","VAL2","VAL3","VAL4",\
"VAL5","VAL6",\
FNAME,EVENT,\
}},/*355*/
{RRC_PHY_DELETE_CELL_REQ_ID,LOGINFO,MAC_L1_INF,\
"PHY_CELL_DELETE_FAIL",\
{"FAILREASON","VAL1","VAL2","InternalCellId","VAL4",\
"VAL5","VAL6",\
FNAME,EVENT,\
}},
{MAC_CELL_DELETE_CNF,LOGINFO,MAC_RRC_INF,\
"MAC_CELL_DELETE_CNF_FAIL",\
{"FAILREASON","VAL1","VAL2","InternalCellId","VAL4",\
"VAL5","VAL6",\
FNAME,EVENT,\
}},
{RRC_PHY_CELL_DELETE_RES_ID,LOGINFO,MAC_L1_INF,\
"PHY_CELL_DELETE_FAIL_RES",\
{"FAILREASON","VAL1","VAL2","VAL3","VAL4",\
"VAL5","VAL6",\
FNAME,EVENT,\
}},
/* SPR 15496 Fix End */
/* SPR 8194 Fix Start */
{MAC_PHY_START_REQ,LOGDEBUG,MAC_L1_INF,\
"SEND_START_PHY_REQ",\
{"ApiId","SrcModuleId","DestModuleId","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_PHY_CELL_CONFIG_REQ,LOGDEBUG,MAC_L1_INF,\
"SEND_CONFIG_PHY_CELL_REQ",\
{"ApiId","SrcModuleId","DestModuleId","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_PHY_PARAM_REQ,LOGDEBUG,MAC_L1_INF,\
"SEND_PHY_PARAM_REQ",\
{"ApiId","SrcModuleId","DestModuleId","","",\
"","",\
FNAME,EVENT,\
}},
{PHY_MAC_PARAM_RES,LOGDEBUG,MAC_L1_INF,\
"PHY_PARAM_RES_RECVD",\
{"ApiId","SrcModuleId","DestModuleId","","",\
"","",\
FNAME,EVENT,\
}},
{PHY_MAC_INVALID_MSG,LOGDEBUG,MAC_L1_INF,\
"PHY_INVALID_MSG_RECIEVED",\
{"MsgId","SrcModuleId","DestModuleId","LineNum","bytesread",\
"","",\
FNAME,EVENT,\
}},
{PHY_MAC_STATE_MACHINE_HANDLER,LOGDEBUG,MAC_L1_INF,\
"PHY_EVENT_ON_PHY_STATE",\
{"LineNum","State","Event","bytesRead","DelayToPhy",\
"","",\
FNAME, "",\
}},
{PHY_MAC_BYTES_READ,LOGERROR,MAC_L1_INF,\
"No. of bytes received are less",\
{"LineNum","BytesReceived","Size of msg","","",\
"","",\
FNAME,EVENT,\
}},
{PHY_MAC_TLV_TAG,LOGWARNING,MAC_L1_INF,\
"Missing/Invalid Tag received from PHY",\
{"LineNum","TagValue","","","",\
"","",\
FNAME,EVENT,\
}},
{MAC_PHY_MSG_RETRANSMISSION, LOGDEBUG, MAC_L1_INF,\
"Retransmission for PHY message",\
{"LineNum", "RetryValue", "State", "MaxRetxValue", "InternalCellId",\
"","",\
FNAME,EVENT, \
}}, 
/* SPR 8194 Fix End */
/* SPR 8196 Fix Start */
{MAC_OAM_INIT_IND,LOGDEBUG,MAC_OAM_INF,\
"SEND_MAC_OAM_INIT",\
{"ApiId","SrcModuleId","DestModuleId","","",\
"","",\
FNAME,EVENT,\
}}, 
/* SPR 8196 Fix End */
/* SPR 3894 change start */
{MAC_UE_CONTEXT_IS_NULL_IN_DL_LOG_ID,LOGFATAL,MAC_DL_Strategy,\
"MAC_UE_CONTEXT_IS_NULL_IN_DL",\
{"LineNum","ue_index","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_UE_CONTEXT_IS_NULL_IN_UL_LOG_ID,LOGFATAL,MAC_UL_Strategy,\
"MAC_UE_CONTEXT_IS_NULL_IN_UL",\
{"ue_index","LcId","LineNum","","",\
"","",\
FNAME,EVENT,\
} },/*360*/ 
{MAC_LC_NODE_NOT_PROCESSED_IN_DL_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"MAC_LC_NODE_NOT_PROCESSED_IN_DL",\
{"ue_index","LcId","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_LC_NODE_NOT_PROCESSED_IN_UL_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"MAC_LC_NODE_NOT_PROCESSED_IN_UL",\
{"ue_index","LcId","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_DL_STRATEGY_UE_ID,LOGBRIEF,MAC_DL_Strategy,\
"MAC_DL_STRATEGY_UE",\
{"ue_index","harqProcessId","txType","minTbsize","maxTbsize",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_SCHEDULED_UE_HAS_NULL_POINTER,LOGFATAL,MAC_DL_Strategy,\
"MAC_SCHEDULED_UE_HAS_NULL_POINTER",\
{"ueDLContext_p","DlStrategyTxNode_p","HarqProcess_p","LineNum","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_LC_PRIORITY_BEFORE_ONDURATON_DELAY_CONSIDERED_IN_DL_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"MAC_LC_PRIORITY_BEFORE_ONDURATON_DELAY_CONSIDERED_IN_DL",\
{"Total LC Weight","ue_index","LcId","DRX_TRIGGER","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_LC_PRIORITY_BEFORE_ONDURATON_DELAY_CONSIDERED_IN_UL_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"MAC_LC_PRIORITY_BEFORE_ONDURATON_DELAY_CONSIDERED_IN_UL",\
{"Total LC Weight","ue_index","LcId","DRX_TRIGGER","",\
"","",\
FNAME,EVENT,\
} }, 
/* SPR 3894 change End */
{MAC_LC_PRIORITY_AFTER_ONDURATON_DELAY_CONSIDERED,LOGDEBUG,MAC_DL_Strategy,\
"MAC_LC_PRIORITY_AFTER_ONDURATON_DELAY_CONSIDERED",\
{"Total LC Weight","ue_index","LcId","DRX_TRIGGER","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_LC_NODE_ALREADY_DELETED_FROM_TREE,LOGDEBUG,MAC_DL_Strategy,\
"MAC_LC_NODE_ALREADY_DELETED_FROM_TREE",\
{"ue_index","","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_TA_NODE_ALREADY_PROCESSED_FOR_UE,LOGDEBUG,MAC_DL_Strategy,\
"MAC_TA_NODE_ALREADY_PROCESSED_FOR_UE",\
{"ue_index","TA Value","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_ALL_HARQ_ARE_BUSY,LOGFATAL,MAC_DL_Strategy,\
"MAC_ALL_HARQ_ARE_BUSY",\
{"ue_index","first","second","third","forth",\
"","",\
FNAME,EVENT,\
} },
/* SPR 3894 change start */ 
{MAC_UE_DELETE_PENDING_IN_DL_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"MAC_UE_DELETE_PENDING_IN_DL",\
{"ue_index","","","","",\
"","",\
FNAME,EVENT,\
} },/*370*/ 
{MAC_UE_DELETE_PENDING_IN_UL_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"MAC_UE_DELETE_PENDING_IN_UL",\
{"ue_index","","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_UE_CANT_BE_SCHEDULED,LOGFATAL,MAC_UL_Strategy,\
"MAC_UE_CANT_BE_SCHEDULED",\
{"ue_index","","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_NUM_UE_PER_TICK_REACHED_IN_DL_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"MAC_UE_CANT_BE_SCHEDULED_IN_DL",\
{"ue_index","","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_NUM_UE_PER_TICK_REACHED_IN_UL_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"MAC_UE_CANT_BE_SCHEDULED_IN_UL",\
{"ue_index","","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_QUEUE_IS_EMPTY_IN_DL_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"MAC_QUEUE_IS_EMPTY_IN_DL",\
{"Count","","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_QUEUE_IS_EMPTY_IN_UL_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"MAC_QUEUE_IS_EMPTY_IN_UL",\
{"Count","","","","",\
"","",\
FNAME,EVENT,\
} },
/* SPR 3894 change End */ 
{MAC_BSR_ADD_TRIGGER,LOGDEBUG,MAC_UL_Strategy,\
"MAC_BSR_ADD_TRIGGER",\
{"ue_index","lcgId","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_BSR_DELETE_TRIGGER,LOGDEBUG,MAC_UL_Strategy,\
"MAC_BSR_DELETE_TRIGGER",\
{"ue_index","lcgId","","","",\
"","",\
FNAME,EVENT,\
} },
/* SPR 3894 change start */  
{MAC_PUSH_IN_QUEUE_FAILURE_IN_DL_LOG_ID,LOGFATAL,MAC_DL_Strategy,\
"MAC_PUSH_IN_QUEUE_FAILURE_IN_DL",\
{"ue_index","","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_PUSH_IN_QUEUE_FAILURE_IN_UL_LOG_ID,LOGFATAL,MAC_UL_Strategy,\
"MAC_PUSH_IN_QUEUE_FAILURE_IN_UL",\
{"","","","","",\
"","",\
FNAME,EVENT,\
} },/*380*/
/* SPR 3894 change End */ 
{MAC_UL_SCHEDULED_UE_HAS_NULL_POINTER,LOGFATAL,MAC_UL_Strategy,\
"MAC_UL_SCHEDULED_UE_HAS_NULL_POINTER",\
{"","","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_UL_QOS_STRATEGY_RETRAVERSING,LOGDEBUG,MAC_UL_Strategy,\
"MAC_UL_QOS_STRATEGY_RETRAVERSING",\
{"","","","","",\
"","",\
FNAME,EVENT,\
} }, 
{MAC_DL_STRATEGY_TX_NODE_CREATED,LOGDEBUG,MAC_DL_Strategy,\
"MAC_DL_STRATEGY_TX_NODE_CREATED",\
{"ue_index","DlQosStrategyLCNode_p","ueDLContext_p->dlStrategyTxNode_p","ScheduleFlag","NumDlUEsToBeScheduled",\
"","",
FNAME,EVENT,\
} },
{MAC_DL_MODIFY_UE_ID,LOGWARNING,MAC_DL_Strategy,\
"MAC_DL_MODIFY_UE",\
{"ModifiedUe","","","","","",\
"",\
FNAME,EVENT,\
} }, 
/* QOS PDSCH MUX LOGS Change Start */
{MAC_RB_NOT_AVAILABLE_FOR_RETX,LOGDEBUG,MAC_PDSCH,\
"MAC_RB_NOT_AVAILABLE_FOR_RETX",\
{"ue_index","AvailRB","ReqRB","","",\
"","",\
FNAME,EVENT,\
} },
{MAC_RESOURCE_ALLOCATED_AFTER_VALIDATING_TB_FOR_QOS_SPATIAL,LOGDEBUG,MAC_PDSCH,\
"MAC_RESOURCE_ALLOCATED_AFTER_VALIDATING_TB_FOR_QOS_SPATIAL",\
{"AllocatedRB","MCS1","TBSize1","MCS2","TBSize2",
"","",\
FNAME,EVENT,\
} },
{MAC_RESOURCE_ALLOCATED_AFTER_VALIDATING_TB_FOR_QOS_NONSPATIAL, LOGDEBUG, MAC_PDSCH,\
"MAC_RESOURCE_ALLOCATED_AFTER_VALIDATING_TB_FOR_QOS_NONSPATIAL",\
{"ue_index","AllocatedRB","MCS","TbSize","",\
"","",\
FNAME,EVENT,\
} },
{MAC_PDSCH_TBSIZE_LESS_THAN_DLSTRATEGY_MINTBS, LOGDEBUG, MAC_PDSCH,\
"MAC_PDSCH_TBSIZE_LESS_THAN_DLSTRATEGY_MINTBS",\
{"ue_index","PdschTB","MinTB","AllocatedRB","",\
"","",\
FNAME,EVENT,\
} },
{MAC_EXCESS_POOL_RB_DECREEMENTED, LOGDEBUG, MAC_PDSCH,\
"MAC_EXCESS_POOL_RB_DECREEMENTED",\
{"ue_index","ExccessPoolRB's","ExceeRBReq","","",\
"","",\
FNAME,EVENT,\
} },
{MAC_EXCESS_POOL_RB_INCREEMENTED,LOGDEBUG, MAC_PDSCH,\
"MAC_EXCESS_POOL_RB_INCREEMENTED",\
{"ue_index","ExccessPoolRB's","ExcessRBToPool","","",\
"","",\
FNAME,EVENT,\
} },/*390*/
{MAC_NO_RB_LEFT_FOR_ALLOCATION,LOGDEBUG,MAC_PDSCH,\
"MAC_NO_RB_LEFT_FOR_ALLOCATION",\
{"NumFreePRB","","","","",\
"","",\
FNAME,EVENT,\
} },
{MAC_AVAILABLE_TB_LESS_THAN_THREE_BYTES,LOGDEBUG, MAC_MUX,\
"MAC_AVAILABLE_TB_LESS_THAN_THREE_BYTES",\
{"ue_index","availableTBSize","","","",\
"","",\
FNAME,EVENT,\
} },
{MAC_QOS_LC_NODE_NULL,LOGFATAL,MAC_MUX,\
"MAC_QOS_LC_NODE_NULL",\
{"ue_index","lcCount","","","",\
"","",\
FNAME,EVENT,\
} },
/* QOS PDSCH MUX LOGS Change End */
/*RETX_CHG*/
{MAC_RETX_UE_SF_MISMATCH,LOGDEBUG,MAC_DL_Strategy,\
"UE Retxn Not Scheduled due to Subframe Mismatch",\
{"Current SF Type","Schedule SF Type", "", "", "",\
"","",\
FNAME,EVENT,\
} },
/* + CQI_4.1 */
{MAC_RECEIVED_CQI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
"WB_SB_CQI_RECEIVED_VALUES",\
{"ue_index","widebandCqiCW0 ","widebandCqiCW1 ",\
"SubbandCqiCW0 ","subbandCqiCW1 ",\
"","",\
FNAME,EVENT,\
} },
{MAC_RECEIVED_RI_LOG_ID,LOGDEBUG,MAC_L1_INF,\
"RI_RECEIVED_VALUES",\
{"ue_index","UeCat","RI ",\
"","","","",FNAME,EVENT,\
} },
{MAC_EFFECTIVE_CQI_CALC_LOG_ID,LOGINFO,MAC_CQI,\
"EFFECTIVE_CQI_CALC",\
{"Received CQI ","CQI Correction Factor ",\
"Effective CQI ","ue_index","",\
"","",\
FNAME,EVENT,\
} },
{MAC_CORRECTED_WIDEBAND_CQI_LOG_ID,LOGINFO,MAC_CQI,\
"BLER_CORRECTED_WIDEBAND_VALUES",\
{"ue_index","WBCQICWOne","McsCwOne","WBCQICWTwo","McsCwTwo",\
"","",\
FNAME,EVENT,\
} },
{MAC_EFFECTIVE_SINR_MCS_ID,LOGDEBUG,MAC_CQI,\
"MAC_EFFECTIVE_SINR_MCS",\
{"ue_index","WidebandCQICodeWordOne","WidebandCQICodeWordTwo","EffectiveSINR","",\
"","",\
FNAME,EVENT,\
} },
{MAC_PERIODIC_UE_SELECTED_MODE_INFO_LOG_ID,LOGINFO,MAC_CQI,\
"PERIODIC_UE_SELECTED_MODE_INFO",\
{"ue_index","BandwidthPart","ReportedSubBandNum",\
"CorrectedSubBandCQICW0","CorrectedSubBandCQICW1",\
"","",\
FNAME,EVENT,\
} },
{MAC_APERIODIC_CQI_TRIGGERED_LOG_ID,LOGINFO,MAC_DL_Strategy,\
"APERIODIC_CQI_TRIGGERED",\
{"ue_index","HarqId","aperiodicCqiRequestTTI ","","",\
"","",\
FNAME,EVENT,\
} },
/* SPR 3894 change start */
{MAC_SB_CQI_SUCCESS_LOG_ID,LOGDEBUG,MAC_PDSCH,\
"SB_REPORT_SUCC_AVAILABLE",\
{"ue_index","AvailableRB ","RequiredRB ","tempRBGBitMap ","allocatedRb ",\
"","",\
FNAME,EVENT,\
} },/*400*/
{MAC_SB_CQI_FAILURE_LOG_ID,LOGDEBUG,MAC_PDSCH,\
"SB_REPORT_FAIL_AVAILABLE",\
{"ue_index","AvailableRB ","RequiredRB ","tempRBGBitMap ","allocatedRb ",\
"","",\
FNAME,EVENT,\
} },
/* - CQI_4.1 */
/* SPR 15496 Fix Start */

/* SPR 10393 Changes Start */
{RRC_PHY_CELL_START_REQ_ID,LOGINFO,MAC_RRC_INF,\
"PHY_CELL_START_REQ",\
{"ModuleId","MsgId","InternalCellId","","",\
"","",\
FNAME,EVENT,\
}},
{RRC_PHY_CELL_START_RES_ID,LOGINFO,MAC_RRC_INF,\
"PHY_CELL_START_RES",\
{"ModuleId","MsgId","cell_index","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_PHY_CELL_START_RES_ID_DEBUG,LOGDEBUG,MAC_RRC_INF,\
"PHY_CELL_START_RES_DEBUG",\
{"cell_index","RetVal","","","",\
"","",\
FNAME,EVENT,\
}},
/* SPR 10393 Changes End */
{RRC_PHY_CELL_STOP_REQ_ID,LOGINFO,MAC_L1_INF,\
"PHY_CELL_STOP_REQ",\
{"ModuleId","MsgId","InternalCellId","","",\
"","",\
FNAME,EVENT,\
}},
{RRC_PHY_CELL_STOP_RES_ID,LOGINFO,MAC_L1_INF,\
"PHY_CELL_STOP_RES",\
{"ModuleId","MsgId","cell_index","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
{RRC_MAC_CELL_START_REQ_ID,LOGINFO,MAC_L1_INF,\
"MAC_CELL_START_REQ",\
{"ReConfigFailVal1","ReConfigFailVal2","ReConfigFailVal3","ReConfigFailVal4","ReConfigFailVal5",\
"ReConfigFailVal6","ReConfigFailVal7",\
FNAME,EVENT,\
}},
/* SPR 10393 Changes Start */
{RRC_MAC_CELL_START_RES_ID,LOGINFO, MAC_L1_INF,\
"MAC_CELL_START_RES",\
{"MsgId","RetVal","Trans_Id","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
/* SPR 10393 Changes End */
{RRC_MAC_CELL_STOP_REQ_ID,LOGINFO,MAC_L1_INF,\
"MAC_CELL_STOP_REQ",\
{"ReConfigFailVal1","ReConfigFailVal2","ReConfigFailVal3","ReConfigFailVal4","ReConfigFailVal5",\
"ReConfigFailVal6","ReConfigFailVal7",\
FNAME,EVENT,\
}},
{RRC_MAC_CELL_STOP_RES_ID,LOGINFO,MAC_L1_INF,\
"MAC_CELL_STOP_RES",\
{"MsgId","RetVal","Trans_Id","InternalCellId","",\
"","",\
FNAME,EVENT,\
}},
/* SPR 15496 Fix End */
/* SPR 3894 change End */
/* Rel9_upgrade_CR410 */
{MAC_SR_PROHIBIT_TIMER_ID,LOGDEBUG,MAC_L1_INF,\
"SR Prohibit Expiry Timer started on receiving SR indication",\
{"RecvRnti","ue_index","RecvTTI","SR ProhibitTimerVal","SR ProhibitTTI",\
"","",\
FNAME,EVENT,\
}},/*410*/ 
{MAC_SR_PROHIBIT_TIMER_ID_EXPIRY,LOGDEBUG,MAC_L1_INF,\
"SR Prohibit Expiry Timer updated for expiry on sending DCI 0",\
{"RecvRnti","ue_index","RecvTTI","SR ProhibitTimerVal","SR ProhibitTTI",\
"","",\
FNAME,EVENT,\
}}, 
/* Rel9_upgrade_CR410 */
{MAC_PUCCH_INIT_SR_MAP_FAIL,LOGERROR,MAC_PUCCH,\
"SR map initialise failed",\
{"ue_index","SrConfigurationIndex","srPeriodicity",\
"","","","",\
FNAME,EVENT,\
}},
{MAC_PUCCH_GET_PRB_FAIL,LOGERROR,MAC_PUCCH,\
"get PRB failed for PUCCH Resource Index, invalid RRC configuration",\
{"ue_index","PucchResourceIndex",\
"","","","","",\
FNAME,EVENT,\
}},
{MAC_PUCCH_UPDATE_RES_INDEX_FAILED,LOGERROR,MAC_PUCCH,\
"update resource index failed",\
{"ue_index","SrConfigIndex","srPeriodicity",\
"","","","",\
FNAME,EVENT,\
}},
/* Rel9_upgrade_CR257 */
/* CLPC_CHG*/
{MAC_TRANSMITTER_DCI_3Or3A,LOGDEBUG, MAC_L1_INF,\
"Filled DCI Format 3 or 3A",
{"AggregationLvl", "Rnti", \
"DciFormat","","","","",FNAME, "TPC-BITMAP", \
} }, 
{MAC_ADD_ENTRY_TO_TPC_RNTI_TRIGGERED_UE_LIST_ID, LOGDEBUG, MAC_UL_PC,\
"Adding Entry for DCI 3/3A in tpcRntiContext",\
{"ue_index", "TPCRntiId", "TrigCount",\
"TPCTrigVal", "", \
"", "", FNAME,EVENT,\
} },
{MAC_ADD_ENTRY_TO_TPC_TRIGGER_QUEUE_ID, LOGDEBUG, MAC_UL_PC,\
"Adding Entry to tpcTriggereQueueForPusch",\
{"ue_index", "ExoTick", "EXPIRY OFFSET CONFIGURED ",\
"TPCTrigVal", "TPCRntiId", "", "",\
FNAME,EVENT,\
} }, 
{MAC_ADD_ENTRY_EXPLICIT_TPC_FOR_PUSCH_ID, LOGDEBUG, MAC_UL_PC,\
"Expiry of TpcTriggerQueueForPusch",\
{"ue_index", "TPCTrigVal", "TPCRntiId",\
"ExpTick", "CurTick",\
"", "", FNAME,EVENT,\
} },
{CQI_SINR_CALC_PUCCH_ID,LOGDEBUG,MAC_UL_PC,\
"SINR based on CQI for PUCCH",
{"ue_index", "AverageSinr", "TPCTrigVal", \
"","","",FNAME,EVENT,\
} }, 
{HARQ_BLER_CALC_PUCCH_ID,LOGDEBUG,MAC_UL_PC,\
"BLER based on HARQ for PUCCH",
{"ue_index","BLER", "TPCTrigVal", "" \
"","","",FNAME,EVENT,\
} },/*420*/ 
{TPC_PUCCH_CMD_ID,LOGDEBUG,MAC_UL_PC,\
"TPC Command for PUCCH",
{"ue_index", "TPCRntiId", "TPCTrigVal",\
"Actual TPC command", "Power Correction Value", \
"", "",FNAME,EVENT,\
} }, 
{TPC_TRIGGER_Q_ENTRY_PUCCH_ID, LOGDEBUG, MAC_UL_PC,\
"Node Put in PUCCH TPC Trigger Queue ",
{"ue_index", "ExpTick", "TPCTrigVal", \
"","","","",FNAME,EVENT,\
} }, 
{TPC_EXPLICIT_TRIGGER_Q_ENTRY_PUCCH_ID, LOGDEBUG, MAC_UL_PC,\
"Node Put in PUCCH Explicit Trigger Queue ", \
{"ue_index", "TPCTrigVal", "", "" \
"","","",FNAME,EVENT,\
} },
{TPC_TRIGGER_UE_LIST_ENTRY_PUCCH_ID, LOGDEBUG, MAC_UL_PC,\
"Node Put in PUCCH Trigger UE List ", \
{"ue_index", "TPCId", "TrigCount" \
"Scheduled State","","","", FNAME,EVENT,\
}}, 
{TPC_TRIGGER_PUCCH, LOGDEBUG, MAC_UL_PC,\
"TPC Triggered ", \
{"CurTick", "Node Expiry Tick","Queue count in TPC trigger queue",\
"","","","", FNAME,EVENT,\
} }, 
{TPC_TRIGGER_FORMAT_3OR3AQ_ENTRY_PUCCH_ID, LOGDEBUG, MAC_UL_PC,\
"Node Put in PUCCH Scheduled Format 3or 3A Queue ", \
{"TPCId", "Scheduled State", "" \
"","","","", FNAME,EVENT,\
} },
{MAC_TPC_RNTI_ENTRY_IN_SCHEDULED_3OR3A_QUEUE_ID, LOGDEBUG, MAC_UL_PC,\
"Node put in PUSCH scheduled or pending 3 or 3A Queue",\
{"TPCRntiId", "ScheduledState ", \
"TrigCount ", "configuredCount ", "", "", "", FNAME,EVENT
} }, 
{MAC_SCHEDULED_DCI3OR3A_PUSCH_ID, LOGDEBUG, MAC_UL_PC,\
"CCEs allocated for DCi format 3/3A for PUSCH PC",\
{"TPCRntiId", "cceAllocated ", "DciFormat ",\
"TrigCount ", "appliedUlTick ", "", "",\
FNAME,EVENT}
},
{MAC_TPC_SENT_USING_DCI_FORMAT_0_ID, LOGDEBUG, MAC_UL_PC,\
"TPC SENT USING DCI FORMAT 0",\
{"ue_index", "TPCCmd", "lastAppliedTick ",\
"TPCTrigVal in UE Context ", "accumulationEnabled ",\
"", "", FNAME,EVENT}
}, 
{MAC_ADD_ENTRY_EXPLICIT_IN_DRX_PUSCH_ID, LOGDEBUG, MAC_UL_PC,\
"DRX/MEAS IS ON,THUS ENTRY in EXPLICIT",\
{"ue_index", "TPCRntiId", \
"TPCTrigVal",\
"isMeasGapPeriodOn", "DrxCurrentState",\
 "", "",\
FNAME,EVENT}
},/*430*/ 
{MAC_TPC_SENT_DCI_3OR3A_PUSCH_ID, LOGDEBUG, MAC_UL_PC,\
"TPC SENT USING DCI FORMAT 3/3A",\
{"ue_index", "PowerCorrectionVal ", "lastAppliedTick ",\
"PrevPowerCorrectionForPusch ", "currPowerCorrectionForPusch ",\
"", "", FNAME,EVENT
}}, 
{MAC_DELETE_TRIGGERED_DCI3_UE_NODE_ID, LOGDEBUG, MAC_UL_PC,\
 "triggered UE List node deleted from DCI 0 scheduling",\
{"ue_index", "TPCRntiId", \
 "TPCTrigVal", "TPCCmd", "", "", "",\
 FNAME,EVENT,\
}},
{DL_EXPLICIT_Q_PROCESS_ID, LOGDEBUG, MAC_UL_PC,\
"Explicit DL DCI Trigger Queue Processed",
{"ue_index", "QCount", "Grant Provided", "HarqId","",
"","",FNAME,EVENT,\
} }, 
{MAC_PDCCH_ALLOC_FAIL_ID, LOGDEBUG, MAC_DL_PC,\
"PDCCH Allocation failed for the UE",\
{"ue_index", "DciFormat", "AggregationLvl", "PdcchPowOrAggregationEnable",\
"isDLWideBandCQIReportAvailable", "", "", FNAME,EVENT,\
}}, 
{MAC_PDSCH_POWER_ID, LOGDEBUG, MAC_DL_PC,\
"PDSCH Power for the UE",\
{"ue_index","TotalPower","UeCat","",
"","","",FNAME,EVENT,\
}}, 
{ MAC_COMMON_CCES_ALLOCATED_ID, LOGDEBUG, MAC_DL_PC,\
"Primary and secondary dci format  is failed then, CCEs allocated on common search space",\
{"ue_index", "DciFormat", "CCE Allocated", "AggregationLvl", "",\
"", "", FNAME,EVENT,\
}},
{ MAC_INFO_IN_RAR, LOGDEBUG, MAC_RACH,\
"Info In RAR for Each RACH req after successful contention resolution",\
{"PreambleId", "RARNTI","", "", "", "", "",\
FNAME,EVENT}
},
{ MAC_CONTENTION_RESOLUTION, LOGDEBUG, MAC_RACH,\
"Pushed into Contention resolution Queue",\
{"TCRNTI", "", "", "", "", "", "",\
FNAME,EVENT}
},
{ MAC_DETERMINE_RB_FROM_TB_SIZE, LOGDEBUG, MAC_RACH,\
"DetermineRBFromTBSizeForMsg Failed",\
{"TCRNTI", "", "", "", "", "", "",\
FNAME,EVENT}
},
{ MAC_MSG3_RESOURCE_ALLOCATED, LOGDEBUG, MAC_RACH,\
"Resources are allocated for msg3 successfully",\
{"mcs", "PHICH Group", "SF used", "RiStart", "NPhichSeq",\
"", "", FNAME,EVENT}
},
/* + SPR 16766 */
{ PENDING_DELETE_UE, LOGERROR, MAC_RACH,\
"Pending delete flag is set for the UE",\
{"ue_index", "CRNTI", "pendingDeleteFlag", "LineNumber/tcrntiFlag", "RntiFlag",\
"", "", FNAME, EVENT}
},
/* - SPR 16766 */
{ MAC_TICK_LD_CHANGE, LOGWARNING, L2_SYS_FAIL,\
"Tick Overload changed.",\
/*+ Dyn UEs To Be SchPerTTI +*/
{"LineNum", "currTickOverLoad", "MacOverLoadLevel", "DlUeSchInLastTick",\
"ulUeSchInLastTick",\
/*- Dyn UEs To Be SchPerTTI -*/
"", "", "FNAME" ,""
}}, 
{ MAC_INVALID_NUM_SEGMENTS, LOGWARNING, MAC_BCCH,\
"Number of segments received are more than SI window size",\
{"LineNum", "SiWindowSize", "NumSegments", "", "",\
"","",\
FNAME,"",\
}},/*436*/
    /* RRM MAC Interface log */
    /* LOG Id 440 */
{RRM_MAC_MSG_ID,LOGWARNING,MAC_RRM_INF,\
"Received Msg from RRM",\
{"DestModuleId","Trans_Id","CellId","MsgId","MsgLen",\
"","",\
FNAME,"",\
}},
{MAC_RRM_INVALID_MSG_LEN_ID, LOGWARNING, MAC_RRM_INF, \
"Invalid API Length received at RRM-MAC Interface ",\
{"Length", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_WRONG_MODULE_ID, LOGERROR, MAC_RRM_INF, \
"Wrong Source or Destination API ID",\
{"SrcApiId", "DestApiId", "LineNum",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_API_ID, LOGWARNING, MAC_RRM_INF, \
"Invalid  API ID",\
{"ApiId", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
/* Corrected logs */
{MAC_RRM_INVALID_CELL_CONFIG_BITMASK, LOGWARNING, MAC_RRM_INF, \
"Bitmask field zero",\
{"bitmask", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_MESSAGE_SENT, LOGDEBUG, MAC_RRM_INF, \
"Message sent to RRM",\
{"RetVal", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_NUM_OF_CC_REGION, LOGWARNING, MAC_RRM_INF, \
"Invalid num of CC region sent to RRM",\
{"NumOfCCRegion", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_NUM_OF_CE_REGION, LOGWARNING, MAC_RRM_INF, \
"Invalid num of CE region sent to RRM",\
{"NumOfCERegion", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_RB_VALUE, LOGWARNING, MAC_RRM_INF, \
"Invalid Start RB or number of RB",\
{"NumOfRB", "StartRB", "LineNum",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_TOTAL_RB_VALUE, LOGWARNING, MAC_RRM_INF, \
"Invalid number of Total RB",\
{"totalRBCount", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_DL_CC_INFO, LOGWARNING, MAC_RRM_INF, \
"DL cell Center Info",\
{"DL CC Count", "CC1 startRBIndex", "CC1 endRBIndex",\
 "CC2 startRBIndex", "CC2 endRBIndex", "",\
 "",\
FNAME,"", \
}},
/* LOG Id 450 */
{MAC_RRM_DL_CC_INFO_TOTAL_RB, LOGWARNING, MAC_RRM_INF, \
"DL CC number of Total RB",\
{"totalAvailableRBDL", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_DL_CE_INFO, LOGWARNING, MAC_RRM_INF, \
"DL CellEdge Info ",\
{"DL CE Count", "StartRBIndex", "EndRBIndex",\
 "totalCellEdgeRBDL", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_UL_CC_INFO, LOGWARNING, MAC_RRM_INF, \
"UL cellCentre Info",\
{"UL CC Count", "UL CC1startRBIndex", "UL CC1endRBIndex",\
 "UL CC2 startRBIndex", "UL CC2endRBIndex", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_UL_TOTAL_RB, LOGWARNING, MAC_RRM_INF, \
"UL CC number of Total RB",\
{"UL totalAvailableRBUL", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_UL_CE_INFO, LOGWARNING, MAC_RRM_INF, \
"UL CellEdge Info ",\
{"UL CE Count", "UL CE startRBIndex", "UL CE endRBIndex",\
 "UL totalCellEdgeRBUL", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_UE_LOC_INFO_ERR, LOGDEBUG, MAC_RRM_INF, \
"UE Location Reconfigured ERR",\
{"DlUeId", "DlUeLocationType", "UlUeId",\
 "UlUeLocationType", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_DL_USER_LOC_TYPE_ERR ,LOGDEBUG, MAC_RRM_INF, \
"Dl user location error info",\
{"DlUeId", "DlUeLocationType", "TotalCEUserCount",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_UL_USER_LOC_TYPE_ERR ,LOGDEBUG, MAC_RRM_INF, \
"Ul user location error info",\
{"UlUeId", "UlUeLocationType", "TotalCEUserCount",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_API_LEN ,LOGDEBUG, MAC_RRM_INF, \
"invalid API Length",\
{"bytesRead", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_TOTAL_UE ,LOGDEBUG, MAC_RRM_INF, \
"invalid Total UE",\
{"NumOfUe", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
  /* LOG Id 460 */
{MAC_RRM_UE_DOES_NOT_EXIST ,LOGDEBUG, MAC_RRM_INF, \
"UE does not Exit",\
{"RetVal", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_UE_INDEX_ERROR,LOGDEBUG, MAC_RRM_INF, \
"Invalid UE Index Error",\
{"ue_index", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_USER_LOCATION_TYPE_ERROR,LOGDEBUG, MAC_RRM_INF, \
"Invalid Location Type",\
{"LocationType", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_NO_DL_CELL_EDGE_REGION_EXIST,LOGDEBUG, MAC_RRM_INF, \
"NO DL CE Region Exit",\
{"ErrorCodeForCE", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_NO_UL_CELL_EDGE_REGION_EXIST,LOGDEBUG, MAC_RRM_INF, \
"NO UL CE Region Exit",\
{"ErrorCodeForCE", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},

    /* LOG 465 */
{MAC_RRM_UE_LOC_INFO, LOGDEBUG, MAC_RRM_INF, \
"UE Location Reconfigured",\
{"DlUeId", "DlUeLocationType", "UlUeId",\
"UlUeLocationType", "", "",\
"",\
FNAME,"", \
}},

{MAC_RRM_DL_USER_LOC_TYPE ,LOGDEBUG, MAC_RRM_INF, \
"Dl user location info",\
{"DlUeId", "DlUeLocationType", "TotalCEUserCount",\
"", "", "",\
"",\
FNAME,"", \
}},

{MAC_RRM_UL_USER_LOC_TYPE ,LOGDEBUG, MAC_RRM_INF, \
"Ul user location info",\
{"UlUeId", "UlUeLocationType", "TotalCEUserCount",\
"", "", "",\
"",\
FNAME,"", \
}},

{MAC_RRM_PERIODIC_REPORT_PARAMS_1,LOGDEBUG,MAC_RRM_INF, \
"Periodic Report Reconfig Parameters_1",\
{"PeriodicReportsReq", "Periodicity",\
"ueBatchSize", "NumOfBatches", "batchInterval", \
"LineNum", "", \
FNAME,"", \
}},

{MAC_RRM_PERIODIC_REPORT_PARAMS_2,LOGDEBUG,MAC_RRM_INF, \
"Periodic Report Reconfig Parameters_2",\
{"ueBatchSize", "NumOfBatches","batchInterval",\
"", "", \
"", "", \
FNAME,"", \
}},

{MAC_RRM_PERIODIC_REPORT_IND,LOGDEBUG,MAC_RRM_INF, \
"Periodic Report Indication",\
{"Current Batch Index", "Next opportunity","batchInterval",\
"", "", \
"", "", \
FNAME,"", \
}},

{MAC_RRM_MESSAGE_SEND_FAIL,LOGERROR,MAC_RRM_INF, \
"RRM Msg Send Fail ",\
{"", "","",\
"", "", \
"", "", \
FNAME,"", \
}},

{MAC_RRM_INVALID_PERIODIC_REPORT_CONFIG_BITMASK, LOGDEBUG, MAC_RRM_INF, \
"Invalid Periodic Report Bitmask",\
{"bitmask", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_REPORT_REQUIERED_VALUE, LOGDEBUG, MAC_RRM_INF, \
"Invalid Periodic Report Required Value",\
{"PeriodicReportReq", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_PERIODICITY, LOGDEBUG, MAC_RRM_INF, \
"Invalid Periodicity",\
{"Periodicity", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
/* LOG Id 475 */
{MAC_RRM_PERIODICITY_VALUE_NOT_RECEIVED, LOGDEBUG, MAC_RRM_INF, \
"Periodicity value not received",\
{"Periodicity Value", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_BATCH_SIZE_RECONFIG_NOT_ALLOWED, LOGDEBUG, MAC_RRM_INF, \
"Batch Size Reconfig not alowed",\
{"Size", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_UE_BATCH_SIZE, LOGDEBUG, MAC_RRM_INF, \
"Invalid Batch Size ",\
{"Size", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_UE_BATCH_SIZE_VALUE_NOT_RECEIVED, LOGDEBUG, MAC_RRM_INF, \
"Batch Size Value not Received ",\
{"Size", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
    /* UL RA Logs */
    /* Log ID 479*/
{ MAC_ULSCH_RA_INFO_INDEX, LOGDEBUG, MAC_PUSCH,\
"Global index to which map is updated",\
{"ulRAInfoIndex_g", "", "", "", "",\
"","",\
FNAME,"",\
}},
{ MAC_ULSCH_RES_PARTITION_INFO, LOGDEBUG, MAC_PUSCH,\
"Resource Partition Information after updation",\
{"CC freeRBIndex", "CC endIndex", "CC freeRBCounter",
 "totalCellEdgeRB","totalAvailableRB","","",\
FNAME,"RESOURCE_PARTITION", \
}},
{ MAC_NACK_ADAPTIVE_RES_ALLOC, LOGDEBUG, MAC_PUSCH,\
"Nack Adaptive Resource Allocated Information ",\
{ "UeLocationType", "Region used for allocation",
 "allocated index", "allocated RIV", "allocated length",
 "","",\
FNAME,"ADAPTIVE_RES_ALLOC", \
}},
{MAC_ADV_CC_AVAIL_RES_INFO, LOGDEBUG, MAC_PUSCH,\
" Resource Information in advance subframe ",\
{"MinIndex", "StartIndex", "FreeRBIndex",
 "EndIndex", "FreeRBCounter", "","",\
 FNAME,"ADVANCE_RES_ALLOC", \
}},
{MAC_CC_AVAIL_RES_INFO, LOGDEBUG, MAC_PUSCH,\
" Resource Info of CC-CE region in current SF after PUCCH resource allocation",\
{"MinIndex", "StartIndex", "FreeRBIndex",
 "EndIndex", "FreeRBCounter", "","",\
 FNAME,"AFTER_PUCCH_RES_ALLOC", \
}},
{MAC_UPD_CE_AVAIL_RES_INFO, LOGDEBUG, MAC_PUSCH,\
" Resource Info of updated CE region ",\
{ "FreeRBIndex", "EndIndex", "freeRBCounter",
  "No of overlapped rbs", "","","",\
 FNAME,"CE_REGION_RES_UPDATION", \
}},
{MAC_UPD_CC_AVAIL_RES_INFO, LOGDEBUG, MAC_PUSCH,\
" Resource Info of updated CC region ",\
{ "FreeRBIndex", "EndIndex", "freeRBCounter",
  "", "","","",\
 FNAME,"CC_REGION_RES_UPDATION", \
}},
{MAC_RES_ALLOC_INFO, LOGDEBUG, MAC_PUSCH,\
" PUSCH resources allocated ",\
{ "NumOfResourcesReq", "MinReqdRBs", 
  "FreeRBIndex", "EndIndex","freeRBCounter",
  "",""\
 FNAME,"RESOURCE_ALLOCATION", \
}},
{MAC_UCI_ULSCH_PDU_CHANGE, LOGDEBUG, MAC_PUSCH,\
" uci pdu is changed to corresponding ulsch pdu ",\
{ "ue_index","uci pdu ", "ulsch pdu", 
  "ulschUciInfoIndex ", " ulHarqId "," ulGrantRequestType",
  "",\
 FNAME,"UCI TO ULSCH PDU ", \
}},
{MAC_REQ_RB_UE_INFO, LOGDEBUG, MAC_PUSCH,\
" ue requirements of rb and available rbs ",\
{ "ue_index","user location type ", "AvailRBs", 
  "buffer rbs ", " required rbs ","",
  "",\
 FNAME,"REQ RB UE INFO ", \
}},
{MAC_ADV_ALLOC_FLAG_RESET, LOGDEBUG, MAC_PUSCH,\
" advance alloc flag is reset to false  ",\
{ "RbMapNum ","advanceAllocFlag ", "", 
  "","","","",\
 FNAME,"ADV_ALLOC_FLAG_RESET ", \
}},
   /* Log ID 490*/
{MAC_BUFF_POOL_RES_INFO, LOGDEBUG, MAC_PUSCH,\
 " buffer pool   ",\
{ "ue_index","userLocationType ", "buffer rbs",
  "MaxReqRB","addReqRB","","",\
  FNAME,"ADV_ALLOC_FLAG_RESET ", \
}},
{MAC_RIV_RES_INFO, LOGDEBUG, MAC_PUSCH,\
 "Reserved resource information ",\
{"ue_index","RBMapNum",\
  "allocatedLength","allocatedIndex","allocatedRiv",\
  "","",\
  FNAME,"RIV_RES_INFO ", \
}},
{MAC_PUCCH_RES_ALLOC, LOGDEBUG, MAC_PUSCH,\
 "Pucch resource allocation ",\
{"RBMapNum","PrbIndexSlot1","prbIndexSlot2",\
  "MinIndex","StartIndex",\
  "","",\
  FNAME,"PUCCH_RESOURCE_ALLOCATION ",\
}},
{MAC_DIRECT_RES_ALLOC_INFO, LOGDEBUG, MAC_PUSCH,\
 "Resource allocation where start and length are specified",\
{ "StartIndex","Length",\
  "FreeRBIndex","EndIndex","freeRBCounter",\
  "","",\
  FNAME,"DIRECT_RES_ALLOC",\
}},
{ MAC_ULSCH_CE_RES_PARTITION_INFO, LOGDEBUG, MAC_PUSCH,\
 "Resource Partition Information of CE region ",\
 {"CE freeRBIndex", "CE endIndex", "CE freeRBCounter",
 "","","","",\
 FNAME,"RESOURCE_PARTITION", \
}},
{ MAC_NACK_RES_ALLOC_INFO, LOGDEBUG, MAC_UL_HARQ,\
 "Nack resource allocation information",\
 {"RiStart", "NumOfRB", "avail CC resources",
 "avail CE resources","","","",\
 FNAME,"RESOURCE_PARTITION", \
}},
{MAC_UCI_ULSCH_PDU_ASSIGN, LOGDEBUG, MAC_PUSCH,\
" uci pdu is assigned to ULSCH ",\
{ "ue_index","PduType", \
  "ulschUciInfoIndex ", " ulHarqId ","","",\
  "",\
 FNAME,"ULSCH_PDU_ASSIGN ", \
}},
{MAC_PRACH_RES_ALLOC, LOGDEBUG, MAC_PUSCH,\
 "Resource allocation of prach ",\
{ "StartIndex","Length",\
  "FreeRBIndex","EndIndex","freeRBCounter",\
  "","",\
  FNAME,"PRACH_RES_ALLOC",\
}},
{MAC_NON_ADAPTIVE_RETXN_RES_ALLOC, LOGDEBUG, MAC_PUSCH,\
 "Resource allocation of non-adaptive retranmission ",\
{ "StartIndex","Length",\
  "FreeRBIndex","EndIndex","freeRBCounter",\
  "","",\
  FNAME,"RETXN_RES_ALLOC",\
}},
{MAC_AVG_SINR_UPDATE, LOGDEBUG, MAC_PUSCH,\
 "avg sinr is calculated only for useful prbs ",\
{ "ue_index","UeLocationType",\
  "usefulPRBs","","",\
  "","",\
  FNAME,"MAC_AVG_SINR_UPDATE",\
}},
    /* DL RA Logs */
    /* Log ID 500 */
{MAC_PDSCH_INITIAL_BUFF_POOL_DETAILS,LOGDEBUG,MAC_PDSCH,\
"Initial values of Buffer Pool",\
{"Total Buff Pool RBs ","Cell Edge Buff Pool RBs","","","","",""\
FNAME,EVENT,\
} },
{MAC_PDSCH_BUFF_POOL_ALLOC_DETAILS,LOGDEBUG,MAC_PDSCH,\
"Values of Buffer Pool after allocation to UE",\
{"Total Buff Pool RBs ","Cell Edge Buff Pool RBs","","","","",""\
FNAME,EVENT,\
} },
{MAC_RAT0_BITMAP,LOGDEBUG,MAC_PDSCH,\
"RAT0 Bitmaps for cell centre and cell edge user",\
{"RAT0 Bitmap CC USER","RAT0 Bitmap CE USER","","","","",""\
FNAME,EVENT,\
} },
{MAC_RAT1_SUBSET_INFO,LOGDEBUG,MAC_PDSCH,\
"RAT1 Subset Information",\
{"Subset Count","Subset Bitmap CC USER","Subset Bitmap CE USER",\
"Span Width","Num of free PRB","",""\
FNAME,EVENT,\
} },
{MAC_PDCCH_SCHEDULING_DETAILS,LOGDEBUG,MAC_PDSCH,\
"PDCCH scheduling information",\
{"Common Channel message count",\
 "CC_USER message count","CE USER message count",\
"","","",""\
FNAME,EVENT,\
} },
{MAC_PDSCH_UE_SCHEDULING_INFO,LOGDEBUG,MAC_PDSCH,\
"UE scheduling information",\
{"ue_index","HarqId","AllocatedRB",\
"CW1 McsId","CW2 McsId","",""\
FNAME,EVENT,\
} },
{MAC_RAT2_DISTRIBUTED_RECONFIG,LOGDEBUG,MAC_PDSCH,\
"RAT2 Distributed reconfiguration details",\
{"Number of VRBs","Number of Cell Edge VRBs",\
"Best Continuous VRB count",\
"First Cell edge free index","End Cell edge free index","",""\
FNAME,EVENT,\
} },
{MAC_RAT2_LOCALISED_RECONFIG,LOGDEBUG,MAC_PDSCH,\
"RAT2 Localised reconfiguration details",\
{"Number of VRBs","Number of Cell Edge VRBs",\
"First Cell edge free index","End Cell edge free index","","",""\
FNAME,EVENT,\
} },
{MAC_RAT2_LOCALIZED_VRB_SPAN,LOGDEBUG,MAC_PDSCH,\
"RAT2 Localised VRB Span",\
{"Starting VRB Index","Ending VRB Index",\
"","","","",""\
FNAME,EVENT,\
} },
{MAC_PDSCH_UE_SCHEDULING_DETAILS,LOGDEBUG,MAC_PDSCH,\
"PDSCH UE Scheduling details",\
{"UeLocationType","TX Indicator",\
"Number of Cell Edge VRB available",\
"First Cell Edge Free Index","End Cell Edge Free Index","",""\
FNAME,EVENT,\
} },

/*  Logs IDs for Strategy */
/* Lod ID 510 */
{MAC_TOT_ACTIVE_UE_GT_MAX_UE_SCHEDULE, LOGDEBUG, MAC_DL_Strategy,\
"Total active UE is less than max UE to be scheduled DL, updating maxUEsToBeScheduledDL, assigning ",\
{"tempTotalActiveUE" , "to maxUEsToBeScheduledDL","tempTotalActiveUE","MaxUEsToBeScheduledDL","","","",\
FNAME,"",\
}},
/* Corrected logs */
{MAC_TOT_ACTIVE_UE_GT_MAX_CE_UE, LOGDEBUG, MAC_DL_Strategy,\
"Total active UE is less than max CE UE to be scheduled DL, updating maxCellEdgeUEsToBeScheduledDL_g, \
assigning ", {"tempTotalActiveUE" , "to maxCellEdgeUEsToBeScheduledDL_g","tempTotalActiveUE",\
"MaxCellEdgeUEsToBeScheduledDL_g","","","", FNAME,"",\
}},
{MAC_CE_RESOURCE_EXHAUSTED_PUT_IN_PRIORITY_Q, LOGDEBUG, MAC_DL_Strategy,\
"Can not schedule Cell edge user, pushing it in Cell edge priority queue",\
{"Cell edge available RBs" , "MaxCellEdgeUEsToBeScheduledDL_g","ue_index",\
"","","","", FNAME,"",\
}},
{MAC_RR_HARQ_PROCESS_ALLOCATED, LOGDEBUG, MAC_DL_Strategy,\
"Harq process allocated for ",\
{"ue_index" , "","", "","","","", FNAME,"",\
}},
{MAC_RR_UE_ADDED_IN_DL_USER_LIST, LOGDEBUG, MAC_DL_Strategy,\
"UE added in dlUserList ",\
{"UeLocationType" , "totalUEServed","MaxUEsToBeScheduledDL", "maxCellEdgeUEsToBeScheduledDL_g",\
"","","", FNAME,"",\
}},
{MAC_RR_CE_MAX_RB_GT_CC_MAX_RB, LOGDEBUG, MAC_DL_Strategy,\
"Max RB to allocated for CC_USER is greater than max CE_USER, assigning maxDLRBToBeAllocInRoundRobin to\
maxCellEdgeRBToBeAlloc", {"MaxCellEdgeRBToBeAlloc" , "maxDLRBToBeAllocInRoundRobin","", "",\
"","","", FNAME,"",\
}},
{MAC_RR_RB_NOT_AVAILABLE_PUSHING_IN_TA_Q, LOGDEBUG, MAC_DL_Strategy,\
"Available RBs is 0, pushing node in pending TA Queue ",\
{"ueCount" , "ue_index","", "",\
"","","", FNAME,"",\
}},
{MAC_RR_RB_NOT_AVAILABLE_FREE_HARQ, LOGDEBUG, MAC_DL_Strategy,\
"Available RBs is 0, Free harq process ",\
{"ueCount" , "ue_index","", "",\
"","","", FNAME,"",\
}},
{MAC_RR_UNUSED_RB_ADDED_TO_ALLOCATED_RB, LOGDEBUG, MAC_DL_Strategy,\
"Adding unusedRBsDL_g to allocatedRBFromRoundRobin",\
{"unusedRBsDL_g" , "allocatedRBFromRoundRobin","", "",\
"","","", FNAME,"",\
}},
{MAC_RR_SUBTRACT_USED_RB_FROM_ALLOCATED_RB, LOGDEBUG, MAC_DL_Strategy,\
"Subtracting usedRB from allocatedRB and assigning to unusedRBsDL_g",\
{"usedRB" , "AllocatedRB","current value of unusedRBsDL_g", "after subtracting unusedRBsDL_g",\
"","","", FNAME,"",\
}},
/* Log ID 520 */
{MAC_RR_MAKING_UNUSED_RB_ZERO, LOGDEBUG, MAC_DL_Strategy,\
"Making unusedRBsDL_g as 0",\
{"usedRB" , "AllocatedRB","", "",\
"","","", FNAME,"",\
}},
{MAC_UL_RR_MAX_UE_SCHEDULE_GT_NUM_OF_UE_SCHEDULE, LOGDEBUG, MAC_DL_Strategy,\
"MaxUEsToBeScheduledUL is greater than numOfUeToSchedule, updating maxUEsToBeScheduledUL \
with numOfUeToSchedule",{"MaxUEsToBeScheduledUL" , "NumOfUeToSchedule","tempTotalActiveUE",\
 "MaxCellEdgeUEsToBeScheduledUL_g", "","","", FNAME,"",\
}},
{MAC_UL_RR_MAX_UE_SCHEDULE_GT_TOTAL_ACTIVE_UE, LOGDEBUG, MAC_DL_Strategy,\
"MaxUEsToBeScheduledUL is greater than tempTotalActiveUE, updating maxUEsToBeScheduledUL \
with tempTotalActiveUE",{ "MaxUEsToBeScheduledUL","NumOfUeToSchedule" ,"tempTotalActiveUE", \
"MaxCellEdgeUEsToBeScheduledUL_g","","","", FNAME,"",\
}},
{MAC_UL_RR_MAX_CE_UE_GT_NUM_OF_UE_SCHEDULE, LOGDEBUG, MAC_DL_Strategy,\
"MaxCellEdgeUEsToBeScheduledUL_g > numOfUeToSchedule, updating maxCellEdgeUEsToBeScheduledUL_g \
with numOfUeToSchedule", {"MaxUEsToBeScheduledUL" , "NumOfUeToSchedule","tempTotalActiveUE", \
"MaxCellEdgeUEsToBeScheduledUL_g", "","","", FNAME,"",\
}},
{MAC_UL_RR_MAX_CE_UE_GT_TOTAL_ACTIVE_UE, LOGDEBUG, MAC_DL_Strategy,\
"MaxCellEdgeUEsToBeScheduledUL_g is greater than tempTotalActiveUE, updating ellEdgeUEsToBeScheduledUL_g \
with tempTotalActiveUE", {"MaxUEsToBeScheduledUL" , "NumOfUeToSchedule","tempTotalActiveUE", \
"MaxCellEdgeUEsToBeScheduledUL_g","","","", FNAME,"",\
}},
{MAC_UL_RR_PROCESSING_CE_PRIORITY_Q, LOGDEBUG, MAC_DL_Strategy,\
"Cell Edge Priority queue count is non-zero, processing Cell Edge queue first",\
{"cePriorityListCount" , "MaxCellEdgeUEsToBeScheduledUL_g","NumberOfAvailableRBs_p[CE_USER]", \
"","","","", FNAME,"",\
}},
{MAC_UL_RR_PUSHING_TO_CE_PRIORITY_Q, LOGDEBUG, MAC_DL_Strategy,\
"Pushing UE to Cell edge priority queue",\
{"UeLocationType" , "NumberOfAvailableRBs_p[CE_USER]","MaxCellEdgeUEsToBeScheduledUL_g", \
"","","","", FNAME,"",\
}},
{MAC_UL_RR_SUBTRACT_USED_RB_FROM_ALLOCATED_RB, LOGDEBUG, MAC_DL_Strategy,\
"Subtracting usedRB from rbAllocated and assigning to unusedULRb_g",\
{"usedRB" , "RbAllocated","current value of unusedULRb_g", "after subtracting unusedULRb_g",\
"","","", FNAME,"",\
}},
{MAC_UL_RR_MAKING_UNUSED_RB_ZERO, LOGDEBUG, MAC_DL_Strategy,\
"Making unusedULRb_g as 0",\
{"usedRB" , "RbAllocated","", "",\
"","","", FNAME,"",\
}},
{MAC_DL_CE_UE_NOT_SCHEDULED, LOGDEBUG, MAC_DL_Strategy,\
"Cell edge UE not scheduled,",\
{"MaxCellEdgeUEsToBeScheduledDL_g" , "","", "",\
"","","", FNAME,"",\
}},
/* 530 */
{MAC_DL_CC_RESOURCE_ALLOCATED_TO_CE_UE, LOGDEBUG, MAC_DL_Strategy,\
"Allocating cell center region to Cell Center UE,",\
{"MaxCellEdgeUEsToBeScheduledDL_g" , "","", "",\
"","","", FNAME,"",\
}},
{MAC_UL_CE_RESOURCE_GT_CC_CE_RESOURCE, LOGDEBUG, MAC_UL_Strategy,\
"CE resource is greater than CC resource, assigning CC resource to CE resource ",\
{"NumberOfAvailableRBs[CC_CE_REGION]" , "numberOfAvailableRBs[CE_REGION]","", "",\
"","","", FNAME,"",\
}},
     /* LOG Id 532 */
{MAC_RB_INFO_GBR_BEFORE_ALLOCATE_RB_IN_DL, LOGDEBUG, MAC_DL_Strategy,\
 "Rb info gbr before allocating RB to UE ",\
{"RB_CC_CE_REGION","RB_CE_REGION","UeLocationType",\
 "ue_index","SchedulableBytes","","",FNAME,"",\
}},
{MAC_RB_INFO_GBR_AFTER_ALLOCATE_RB_IN_DL,LOGDEBUG, MAC_DL_Strategy,\
 "RB info gbr after allocating RB to UE",\
{"RB_CC_CE_REGION","RB_CE_REGION","UeLocationType",\
 "DeltaRB","ue_index","","",FNAME,"",\
}},
{MAC_RB_INFO_NONGBR_BEFORE_ALLOCATE_RB_IN_DL, LOGDEBUG, MAC_DL_Strategy,\
 "Rb info nongbr before allocating RB to UE ",\
{"RB_CC_CE_REGION","RB_CE_REGION","UeLocationType",\
 "ue_index","SchedulableBytes","","",FNAME,"",\
}},
{MAC_RB_INFO_NONGBR_AFTER_ALLOCATE_RB_IN_DL,LOGDEBUG, MAC_DL_Strategy,\
 "RB info nongbr after allocating RB to UE",\
{"RB_CC_CE_REGION","RB_CE_REGION","UeLocationType",\
 "DeltaRB","ue_index","","",FNAME,"",\
}},
{MAC_RB_INFO_BEFORE_ALLOCATE_RB_IN_DL, LOGDEBUG, MAC_DL_Strategy,\
 "Rb info LC node with schdulable byte before allocating RB to UE ",\
{"RB_CC_CE_REGION","RB_CE_REGION","UeLocationType",\
 "ue_index","SchedulableBytes","","",FNAME,"",\
}},
{MAC_RB_INFO_AFTER_ALLOCATE_RB_IN_DL,LOGDEBUG, MAC_DL_Strategy,\
 "RB info LC node with schdulable bytes after allocating RB to UE",\
{"RB_CC_CE_REGION","RB_CE_REGION","UeLocationType",\
 "DeltaRB","ue_index","","",FNAME,"",\
}},
{MAC_RB_INFO_RETRAVERSAL_BEFORE_ALLOCATE_RB_IN_DL, LOGDEBUG, MAC_DL_Strategy,\
 "Rb info for retraversal before allocating RB to UE ",\
{"RB_CC_CE_REGION","RB_CE_REGION","UeLocationType",\
 "ue_index","SchedulableBytes","","",FNAME,"",\
}},
{MAC_RB_INFO_RETRAVERSAL_AFTER_ALLOCATE_RB_IN_DL,LOGDEBUG, MAC_DL_Strategy,\
 "RB info for retraversal after allocating RB to UE",\
{"RB_CC_CE_REGION","RB_CE_REGION","UeLocationType",\
 "DeltaRB","ResourceRegionAllocated","","",FNAME,"",\
}},
   /* Log id 540 */
 {MAC_RB_TEST_BEFORE_ALLOCATE_RB_IN_DL,LOGDEBUG, MAC_DL_Strategy,\
 "RB info for retraversal before allocating RB to UE",\
{"RB_CC_CE_REGION","assignedresource","UeLocationType",\
 "ue_index","RequiredRB","","",FNAME,"",\
}},
 {MAC_BUFFERPOOL_RB_INFO_AFTER_ALLOCATE_RB_IN_DL,LOGDEBUG, MAC_DL_Strategy,\
 "BufferpoolRB info after allocating RB to UE",\
{"RB_CC_CE_REGION","RB_CE_REGION","UeLocationType",\
 "totalRB of bufferpool","StrategyBufferRB","","",FNAME,"",\
}},
 {MAC_BUFFERPOOL_CE_RB_INFO_AFTER_ALLOCATE_RB_IN_DL,LOGDEBUG, MAC_DL_Strategy,\
 "BufferpoolRB of Cell edge after allocating RB to UE",\
{"RB_CC_CE_REGION","RB_CE_REGION","DL_QOS_UE_SCHEDULED_COUNT",\
 "CellEdgeRB of bufferpool","StrategyBufferRB","","",FNAME,"",\
}},
/* FOR UL QOS Strategy */
{MAC_UE_RB_INFO_BEFORE_ALLOCATE_RB_IN_UL,LOGDEBUG, MAC_UL_Strategy,\
 "RB Info Of UE Before allocation-",\
{"RequiredRB","required RB of satrategy node","UeLocationType",\
 "DeltaRB","QLoad","","",FNAME,"",\
}},

{MAC_RB_INFO_BEFORE_ALLOCATE_RB_IN_UL,LOGDEBUG, MAC_UL_Strategy,\
 "RB Info Of UE before allocation-",\
{"CC_CE_RB","CE_RB","UeLocationType",\
 "DeltaRB","QLoad","","",FNAME,"",\
}},
{MAC_RB_UPDATE_INFO_AFTER_ALLOCATE_RB_IN_UL,LOGDEBUG, MAC_UL_Strategy,\
 "RB Update Info Of UE after allocation-",\
{"CC_CE_RB","CE_RB","UeLocationType",\
 "ue_index","","","",FNAME,"",\
}},
{MAC_RB_RETRAVERSAL_INFO_BEFORE_ALLOCATE_RB_IN_UL,LOGDEBUG, MAC_UL_Strategy,\
 "RB Info For Retravesal Before allocation -",\
{"CC_CE_RB","CE_RB","UeLocationType",\
 "DeltaRB","SchedulableBytes","","",FNAME,"",\
}},
{MAC_RB_RETRAVERSAL_INFO_AFTER_ALLOCATE_RB_IN_UL,LOGDEBUG, MAC_UL_Strategy,\
 "RB Info For Retravesal After allocation -",\
{"CC_CE_RB","CE_RB","UeLocationType",\
 "DeltaRB","ue_index","","",FNAME,"",\
}},
{MAC_SCHDULED_UE_INFO_IN_UL,LOGDEBUG, MAC_UL_Strategy,\
 "Schduled UE Info: ",\
{"ue_index","MinDataSize","maxDataSize",\
 "RequiredRB","UeLocationType","","",FNAME,"",\
}},
{MAC_RB_INFO_PROCESS_TA_IN_DL,LOGDEBUG, MAC_DL_Strategy,\
 "Process TA RB Info Before allocation: ",\
{"ue_index","CC_CE_RB","CE_RB",\
 "UeLocationType","MaxTBSize","","",FNAME,"",\
}},
/* Log Id 550 */
{MAC_RB_UPDATE_INFO_PROCESS_TA_IN_DL,LOGDEBUG, MAC_DL_Strategy,\
 "Process TA RB Info After Allocation: ",\
{"ue_index","CC_CE_RB","CE_RB",\
 "UeLocationType","MIN_RB_FOR_TA_DRX_ONLY","","",FNAME,"",\
}},
{MAC_LC_NODE_NOT_PROCESSED_IN_DL,LOGDEBUG, MAC_DL_Strategy,\
 "cell edge ue can not be schduled ",\
{"CE_RB","NumberofmaxCellEdgeUECount","UeLocationType",\
 "ue_index","MIN_RB_FOR_TA_DRX_ONLY","","",FNAME,"",\
}},
{MAC_CELL_EDGE_UE_CSHDULED_IN_DL,LOGDEBUG, MAC_DL_Strategy,\
 "cell edge ue  schduled ",\
{"CE_RB","ue_index","UeLocationType",\
 "NumberofmaxCellEdgeUECount","NumOfUEToBeSchduled","","",FNAME,"",\
}},
{MAC_CE_UE_NOT_BE_SCHDULED_UL,LOGDEBUG, MAC_UL_Strategy,\
 "cell edge ue not schduled ",\
{"ue_index","CE_RB",\
 "NumberofmaxCellEdgeUECount","UeLocationType","","","",FNAME,"",\
}},
{MAC_CE_UE_SCHDULED_IN_UL,LOGDEBUG, MAC_UL_Strategy,\
 "cell edge ue schduled ",\
{"ue_index","UeLocationType",\
 "NumberofmaxCellEdgeUECount","TotalUEcount","","","",FNAME,"",\
}},
{MAC_CCE_ALLOC_FAIL_ID,LOGWARNING,MAC_PDCCH,\
"CCE allocation failed: ",\
{"ue_index","MsgType","CceId",\
"","","","",\
FNAME,EVENT,\
}},
{MAC_RECV_INVALID_RNTI_ID,LOGERROR,MAC_PDCCH,\
"Invalid RNTI Recieved: ",\
{"RNTI Recvd","","",\
"","","","",\
FNAME,EVENT,\
}},
{MAC_DLUECONTEXT_NOT_FOUND_ID,LOGERROR,MAC_RRC_INF,\
"DL UE Context Not Found: ",\
{"ue_index","LineNum","InternalCellId",\
"","","","",\
FNAME,EVENT,\
}},
{MAC_ULUECONTEXT_NOT_FOUND_ID,LOGERROR,MAC_RRC_INF,\
"UL UE Context Not Found: ",\
{"ue_index","LineNum","InternalCellId",\
"","","","",\
FNAME,EVENT,\
}},
{MAC_UEHARQCONTEXT_NOT_FOUND_ID,LOGERROR,MAC_RRC_INF,\
"DL UE Harq Context Not Found: ",\
{"ue_index","LineNum","InternalCellId",\
"","","","",\
FNAME,EVENT,\
}},
{MAC_RESET_DLUECONTEXT_NOT_FOUND_ID,LOGERROR,MAC_RRC_INF,\
"DL UE Reset Context Not Found: ",\
{"ue_index","LineNum","InternalCellId",\
"","","","",\
FNAME,EVENT,\
}},
{MAC_RESET_ULUECONTEXT_NOT_FOUND_ID,LOGERROR,MAC_RRC_INF,\
"UL UE Reset Context Not Found: ",\
{"ue_index","LineNum","InternalCellId",\
"","","","",\
FNAME,EVENT,\
}},
{DL_NEWTX_ONETBONLYRIONE_UE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"DL New Transmission(one TB RI) Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
{DL_NEWTX_TWOTBONLY_UE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"DL New Transmission(Two TB only) Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
{DL_NEWTX_ONETBONLY_UE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"DL New Transmission(One TB only) Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
/* LOG_Fix Start */
{DL_RETX_UE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"DL ReTransmission Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","","","",\
FNAME,EVENT,\
}},
/* LOG_Fix End */
{DL_EXPLICIT_DCI_SCHEDULED_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"DL Explicit DCI Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
{DL_TA_QUEUE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"DL TA Queue Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
{DL_DRX_QUEUE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"DL DRX Queue Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
{DL_PRIORITY_QUEUE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_DL_Strategy,\
"DL Priority Queue Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
{DL_MAX_UE_SCHEDULED_AND_AVAILABLE_RB_PER_TICK,LOGINFO,MAC_DL_Strategy,\
"DL Scheduled per tick: ",\
{"total Active UE","available RBs","user count","","",\
"","",\
FNAME,"",\
}},
{DL_PRIORITY_SCHEDULED_FAILURE_LOG_ID,LOGWARNING,MAC_DL_Strategy,\
"Ue Scheduling in Priority Queue is Failed Because of No free Harq Process: ",\
{"ue_index","","",\
"","","","",\
FNAME,EVENT,\
}},
{DL_TA_SCHEDULED_FAILURE_LOG_ID,LOGWARNING,MAC_DL_Strategy,\
"Ue Scheduling in TA Queue is Failed Because of No free Harq Process: ",\
{"ue_index","","",\
"","","","",\
FNAME,EVENT,\
}},
{DL_DRX_SCHEDULED_FAILURE_LOG_ID,LOGWARNING,MAC_DL_Strategy,\
"Ue Scheduling in DRX Queue is Failed Because of No free Harq Process: ",\
{"ue_index","","",\
"","","","",\
FNAME,EVENT,\
}},
{DL_NEWTX_FAIR_SCHEDULED_FAILURE_LOG_ID,LOGWARNING,MAC_DL_Strategy,\
"Ue Fair Scheduling in NewTX Queue is Failed Because of No RBs: ",\
{"ue_index","","",\
"","","","",\
FNAME,EVENT,\
}},
{DL_NEWTX_RR_SCHEDULED_FAILURE_LOG_ID,LOGWARNING,MAC_DL_Strategy,\
"Ue RR Scheduling in NewTX Queue is Failed Because of No RBs: ",\
{"ue_index","","",\
"","","","",\
FNAME,EVENT,\
}},
{UL_ROUND_ROBIN_SCHEDULED_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"Uplink New TX Round Robin Scheduling for UE is successful: ",\
{"ue_index","HarqId","",\
"UeQLoad","","","",\
FNAME,EVENT,\
}},
{UL_QOS_SCHEDULED_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"Uplink New TX QOS Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,"",\
}},
{UL_FAIR_SCHEDULED_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"Uplink New TX Fair Scheduling for UE is successful: ",\
{"ue_index","","Used RBs",\
"UeQLoad","Allocated  RBs","","",\
FNAME,EVENT,\
}},
{UL_PEND_GRANT_QUEUE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"Uplink Pending Grant Queue Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
{UL_CONTENTION_GRANT_QUEUE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"Uplink Contention Grant Queue Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
{UL_SR_GRANT_QUEUE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"Uplink SR Grant Queue Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
{UL_CQI_GRANT_QUEUE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"Uplink CQI Grant Queue Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
{UL_EXPLICIT_DCI_QUEUE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"Uplink Explicit DCI Queue Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
{UL_PRIORITY_QUEUE_SCHEDULED_LOG_ID,LOGDEBUG,MAC_UL_Strategy,\
"Uplink Priority Queue Scheduling for UE is successful: ",\
{"ue_index","HarqId","TotalRBs",\
"UeQLoad","MsgType","","",\
FNAME,EVENT,\
}},
{UL_MAX_UE_SCHEDULED_AND_AVAILABLE_RB_PER_TICK,LOGBRIEF,MAC_UL_Strategy,\
"UL Scheduled Ues and available RBs per tick: ",\
{"No of Scheduled UEs","AvailRBs","",\
"","","","",\
FNAME,"",\
}},
{ MAC_INVALID_CAUSE_FOR_SPS_STATE, LOGWARNING, MAC_DL_SPS_STRATEGY,\
"Invalid cause received for this state",\
{"LineNum", "ue_index", "State", "Cause", ""\
"","",\
FNAME,"",\
}},
{MAC_DL_SDL_TTI_FULL, LOGINFO, MAC_DL_SPS_STRATEGY,\
"SPS DCI List full in TTI interval",\
{"LineNum", "TTIIndex", "NumDlSpsDciPerTTI", "MaxDlSpsDciPerTTI", "",\
"","",\
FNAME,"",\
}},
{MAC_DL_SPS_CURR_STATE, LOGINFO, MAC_DL_SPS_STRATEGY,\
"SPS state in system",\
{"LineNum", "ue_index", "OldState", " Event", " NewState",\
"","",\
FNAME," FSM:",\
}},
{ MAC_PDSCH_TB1_INVALID, LOGWARNING, MAC_PDCCH,\
"TB One Invalid",\
{"LineNum", "ue_index", "TB1Valid", "TB2Valid", "",\
"","",\
FNAME,"",\
}},
{ MAC_PDSCH_TB2_INVALID, LOGWARNING, MAC_PDCCH,\
"TB Two Invalid",\
{"LineNum", "ue_index", "TB1Valid", "TB2Valid", "",\
"","",\
FNAME,"",\
}},
{ MAC_PDSCH_TB_INVALID, LOGWARNING, MAC_PDCCH,\
"TB Identifier Invalid",\
{"LineNum", "ue_index", "TBId", "", "",\
"","",\
FNAME,"",\
}},
{ MAC_SPS_NULL_SPS_CRNTI_LIST, LOGWARNING, MAC_RRC_INF,\
"SPS CRNTI list is not initialized",\
{"LineNum", "", "", "", "",\
"","",\
FNAME,"",\
}},
{ MAC_SPS_NULL_NODE, LOGWARNING, MAC_DL_SPS_STRATEGY,\
"NULL node dequeued by SPS Strategy",\
{"LineNum", "QCount", "", "", "",\
"","",\
FNAME,"QueueName",\
}},
{ MAC_DL_SDL_OCC_LIST_FULL, LOGINFO, MAC_DL_SPS_STRATEGY,\
"SPS Occasion List full in TTI interval",\
{"LineNum", "TTIIndex", "NumDlSpsOccPerTti", "MaxDlSpsOccPerTti", "",\
"","",\
FNAME,"",\
}},
{ MAC_MAX_SPS_LC_PER_UE, LOGWARNING, MAC_RRC_INF,\
"Only One SPS LC Per Ue Supported",\
{"LineNum", "LcId", "Qci", "ue_index", "InternalCellId",\
"","",\
FNAME,"",\
}},
{ MAC_NO_OCC_NODE, LOGWARNING, MAC_DL_SPS_STRATEGY,\
"Occasion node is NULL",\
{"LineNum", "ue_index", "State", "Cause", "",\
"","",\
FNAME,EVENT,\
}},
{ MAC_DL_UE_IN_DRX_MEAS_GAP, LOGWARNING, MAC_DL_SPS_STRATEGY,\
"Cannot Activate/Deactivate UE as in DRX or Meas gap",\
{"LineNum", "ue_index", "DrxState", "MeasGapState", "",\
"", "",\
FNAME, "SPS Event",\
}},
{ MAC_SPS_RECONFIG, LOGWARNING, MAC_RRC_INF,\
"MAC receives SPS setup after setup from L3",\
{"LineNum", "ue_index", "ConfiguredReqType", "RecvdReqType", "InternalCellId",\
"", "",\
FNAME, "",\
}},
{ MAC_UE_ID_RNTI_MAP, LOGWARNING, MAC_RRC_INF,\
"SPS CRNTI has no maaping with UE ID",\
{"LineNum", "SpsCrnti", "crnti", "ue_index", "InternalCellId",\
"", "",\
FNAME, "",\
}},
{ MAC_DRX_SPS_SUPPORT_UE, LOGWARNING, MAC_RRC_INF,\
"MAC receives SPS and DRX setup with mismatch of SPS Interval and DRX "\
"long/short cycle from L3",\
{"LineNum", "DRX Long Cycle", "SPS Interval", "ue_index", "DRX Short Cycle",\
"", "",\
FNAME, "",\
}},
/*HD FDD Changes Start*/
{MAC_HD_FDD_UE_ADDED,LOGINFO,MAC_UL_Strategy,\
"HD FDD UE added: ",\
{"ue_index","InternalCellId","","","","","",\
FNAME,EVENT,\
}},
{MAC_HD_FDD_UE_DELETED,LOGINFO,MAC_UL_Strategy,\
"HD FDD UE Deleted: ",\
{"ue_index","InternalCellId","","","","","",\
FNAME,EVENT,\
}},
{MAC_HD_FDD_INVALID_FLAG,LOGDEBUG,MAC_RRC_INF,\
"Invalid HD FDD Flag: ",\
{"Flag","InternalCellId","","","","","",\
FNAME,EVENT,\
}},
{MAC_HD_FDD_CONFIG_SWITCH,LOGINFO,MAC_RRC_INF,\
"HD FDD configuration Switch from config to reconfig: ",\
{"ue_index","InternalCellId","","","","","",\
FNAME,EVENT,\
}},
{MAC_HD_FDD_INVALID_CELL_DUPLEX_MODE,LOGFATAL,MAC_RRC_INF,\
"HD FDD-Invalid cell duplexing mode but got RF Params TRUE: ",\
{"ue_index","Cell Duplex mode","InternalCellId","","","","",\
FNAME,EVENT,\
}},
{MAC_HD_FDD_DISCARD_DATA,LOGINFO,MAC_L1_INF,\
"HD FDD-Discard Data due to hdFddUlNackHandlingFlag is TRUE: ",\
{"ue_index","HarqId","","","","","",\
FNAME,EVENT,\
}},
{MAC_HD_FDD_CHANGE_CRC_ACK_TO_NACK,LOGINFO,MAC_L1_INF,\
"HD FDD-Changes CRC ACK to NACK due to hdFddUlNackHandlingFlag is TRUE: ",\
{"ue_index","HarqId","hdFddUlNackHandlingFlag","CRC Received","UlContainerId","RecvTTI","",\
FNAME,EVENT,\
}},
{MAC_HD_FDD_MIN_REPORTING,LOGWARNING,MAC_RRC_INF,\
"HD FDD-Periodicty is less than 4ms effects Throughput: ",\
{"ue_index","Periodicity","","","","","",\
FNAME,EVENT,\
}},
{MAC_HD_FDD_WRONG_UE_INDEX,LOGWARNING,MAC_L1_INF,\
"HD FDD-UE index greater than max UE's: ",\
{"ue_index","","","","","","",\
FNAME,EVENT,\
}},
{MAC_HD_FDD_CELL_NOT_SUPPORTED,LOGFATAL,MAC_RRC_INF,\
"Cell is not configured for HD-FDD: ",\
{"ue_index","DuplexMode","InternalCellId","","","","",\
FNAME,EVENT,\
}},
{MAC_HD_FDD_DL_CLASH_WITH_UL,LOGFATAL,MAC_RRC_INF,\
"Trying to set DL bit but its already allocated UL for HD-FDD: ",\
{"ue_index","LineNum","Dl Scheduler","Ul Scheduler","","","",\
FNAME,EVENT,\
}},
{MAC_HD_FDD_UL_CLASH_WITH_DL,LOGFATAL,MAC_RRC_INF,\
"Trying to set UL bit but its already allocated DL for HD-FDD: ",\
{"ue_index","LineNum","Dl Scheduler","Ul Scheduler","","","",\
FNAME,EVENT,\
}},
/*HD FDD Changes End*/
/* MAC_TTI_LOG Start */
{MAC_TTI_DCI_LOG_INFO,LOGTTIDEBUG, MAC_OAM_INF,\
"MAC_TTI_DCI_LOG_INFO",\
{"","","",\
"","","","",\
FNAME,EVENT,\
}},
{MAC_TTI_UL_CONF_LOG_INFO,LOGTTIDEBUG, MAC_OAM_INF,\
"MAC_TTI_UL_CONF_LOG_INFO",\
{"","","",\
"","","","",\
FNAME,EVENT,\
}},
{MAC_TTI_CELL_LOG_INFO,LOGINFO, MAC_OAM_INF,\
"MAC_TTI_CELL_LOG_INFO",\
{"current tick","SFN","SF",\
"crnti UE","Tcrnti UE","","",\
FNAME,EVENT,\
}},
{MAC_PERIOD_CELL_LOG_INFO_ID,LOGINFO, MAC_L1_INF,\
"MAC_PERIOD_CELL_LOG_INFO",\
{"numPrach","numCRC Nack","All","numHarq Nack","All", \
"","",\
"","",\
}},
{MAC_TTI_UE_LOG_INFO ,LOGINFO, MAC_OAM_INF,\
"MAC_TTI_UE_LOG_INFO",\
{"","","",\
"","","","",\
FNAME,"",\
}},
/* Corrected logs */
/* - SPR_5470 */
/* SPR_7649 Fix Start */ 
{PUCCH_PUSCH_RES_OVERLAP_LOG_ID ,LOGWARNING, MAC_PUSCH,\
"PUCCH for this UE, may collide with PUSCH of other UE",\
{"ue_index","","",\
"","","","",\
FNAME,EVENT,\
}},
/* SPR_7649 Fix End */
/*START : 5352*/
{MAC_DL_DRX_TRIGGER_INFO,LOGDEBUG,MAC_DL_Strategy,
"DL Info if the DRX trigger recieved",
{"ue_index","DRXTriggerRecv ","","","","","",
FNAME,"",
}},
{MAC_UL_DRX_TRIGGER_INFO,LOGDEBUG,MAC_UL_Strategy,
"UL Info if the DRX trigger recieved",
{"ue_index","DRXTriggerRecv ","","","","","",
FNAME,"",
}},
/*END   : 5352*/
/* MAC_TTI_LOG End*/   
/*SPR 5414 Fix Start */
{MAC_PDSCH_TB1_MCS_BEFORE_CHANGE, LOGBRIEF, MAC_PDSCH,\
"TB1 Size before MCS change:",\
{"ue_index", "CFI", "SF","mcs","TB1Size",\
"","",\
FNAME,"",\
}},
{MAC_PDSCH_TB1_MCS_AFTER_CHANGE, LOGBRIEF, MAC_PDSCH,\
"TB1 Size after  MCS change:",\
{"ue_index", "CFI", "SF","mcs","TB1Size",\
"","",\
FNAME,"",\
}},
{MAC_PDSCH_TB2_MCS_BEFORE_CHANGE, LOGBRIEF, MAC_PDSCH,\
"TB2 Size before MCS change:",\
{"ue_index", "CFI", "SF","mcs","TB2Size",\
"","",\
FNAME,"",\
}},
{MAC_PDSCH_TB2_MCS_AFTER_CHANGE, LOGBRIEF, MAC_PDSCH,\
"TB2 Size after  MCS change:",\
{"ue_index", "CFI", "SF","mcs","TB2Size",\
"","",\
FNAME,"",\
}},
{MAC_PDCCH_CFI_ADAPTATION, LOGBRIEF, MAC_PDCCH,\
"CFI adaptaion:",\
{"MaxCceId","CFI","","",\
"","","",\
FNAME,"",\
}},
/* ATB logs changes */
{MAC_VALIDATE_ATB, LOGBRIEF, MAC_ATB,\
"validating ATB",\
{"RB Val","mcs","minUePowerFlag","maxUePowerFlag",\
"","","",\
FNAME,EVENT,\
}},
{MAC_PUSCH_POWER_ID, LOGBRIEF, MAC_UL_PC,\
"UL Total Transmission Power for PUSCH",\
{"ue_index","TotalPower","mcs","MaxPower","Path Loss"\
"","",FNAME,EVENT,\
}},
/*SPR 5414 Fix End */
/* + SPR_5470 */ 
{HARQ_PONITER_NULL_LOG_ID,LOGWARNING, MAC_L1_INF,\
"Harq pointer NULL received in preparing ULConfig",\
{"ue_index","","",\
"","","","",\
FNAME,EVENT,\
}},
/*UL MU MIMO Start*/ 
{MAC_CAND_SEL_UE_INFO_ID, LOGDEBUG, MAC_PDCCH,\
"Candidate buddies available for UL MIMO: ",\
{"ue_index","Num UE Count", "AggregateMaxRBsReq","AvgMCS","MaxDataSize",\
 "", "",\
 FNAME,EVENT,\
}},
{MAC_CAND_FAIL_UE_INFO_ID, LOGDEBUG, MAC_PDCCH,\
"Candidate Buddies failed for UL MIMO:",\
{"ue_index", "PendingDeleteFlag","UlMUMIMOStatus","AvgMCS","UlGrantRequestType",\
"", "", \
FNAME,EVENT,\
}},
{MAC_BUDDY_UE_INFO_ID, LOGDEBUG, MAC_PUSCH,\
"Buddy UEs for MU MIMO",\
{"ue_index","BuddyUeId", "MaxOfMinDataSize","maxOfMaxDataSize","minAvgMCS",\
"", "",\
FNAME,EVENT,\
}},
{MAC_ULMIMO_ORTHO_N2DMRS_INFO_ID, LOGDEBUG, MAC_PUSCH,\
"Orhtogonal nDMRS for ULMIMO",\
{"RiStartFirst","HiIndexFirst","Available nDMRS","PHICHGrpFirst","PHICHSeqFirst",\
"", "",\
FNAME,EVENT,\
}},
{MAC_ULSCH_NON_FSS_INFO_ID, LOGDEBUG, MAC_PUSCH,\
"MAC NON-FSS allocation for UE",\
{"MinReqRB","MaxReqRB","BufferRBs","RetRBAvailLength","allocatedMCS",\
"","",\
FNAME,EVENT,\
}},
{MAC_ULSCH_FSS_INFO_ID,LOGDEBUG, MAC_PUSCH,\
"MAC FSS allocation for UE",\
{"FreeRBs","MinRBs","MaxRBs","mcs","",\
"","",\
FNAME,EVENT,\
}},
{MAC_UL_MIMO_RE_TX_INFO_ID,LOGDEBUG, MAC_PDCCH,\
"ReTransmission Info for UE",\
{"ue_index","RiStart","NumOfRB","ReTxCount","ReTxType",\
"","",\
FNAME,EVENT,\
}},
{MAC_ULSCH_NON_FSS_FAILURE_INFO_ID, LOGDEBUG, MAC_PUSCH,\
"MAC NON-FSS Failure Info for UE",\
{"ue_index", "LocationType","MinStrictRBReq", "RetAvailableRBLength", "minAvgMCS",\
"","",\
FNAME,EVENT,\
}},
/*NEW_LOG_REQ*/    
{MAC_UL_QOS_UL_METRIC_CALC_PER_USER, LOGBRIEF, MAC_UL_Strategy,\
"MAC UL-QOS ULMETRIC CALC PER USER",\
{"ue_index", "LcId","TotalLcWeight", "", "",\
"","",\
FNAME,EVENT,\
}},
{MAC_DL_QOS_DL_METRIC_CALC_PER_USER, LOGBRIEF, MAC_DL_Strategy,\
"MAC DL-QOS DLMETRIC CALC PER USER",\
{"ue_index", "LcId","TotalLcWeight", "", "",\
"","",\
FNAME,"",\
}},
{PHY_CELL_CONFIG_FAIL, LOGERROR, MAC_L1_INF,\
"Failure response received",\
{"RetVal", "MissingTLVNum","UnsupportedTLVNum", "LineNum","bytesRead",\
"","",\
FNAME,EVENT,\
}},
{PHY_START_FAILURE, LOGERROR, MAC_L1_INF,\
"PHY_START_FAILURE",\
{"RetVal", "","", "", "",\
"","",\
FNAME,"",\
}},
{PHY_UPLINK_PACKET_DELAY,LOGERROR, MAC_L1_INF,\
"PHY_UPLINK_PACKET_DELAY",\
{"SFN","SF", "MsgId", "",\
"","",\
FNAME,"",\
}},
{TIMING_ADVANCE_VALUE,LOGINFO, MAC_TA,\
"TIMING_ADVANCE_VALUE",\
{ "ue_index","Absolute TA Value ", "Current TA Value ","", "",\
"","",\
FNAME,"",\
}},
{MAC_UPDATE_DL_RRM_RPT,LOGINFO,MAC_DL_HARQ,\
"MAC_UPDATE_DL_RRM_RPT",\
{"ue_index","Value", "", "",\
"","","",\
FNAME,EVENT,\
}},
/*UL MU MIMO End*/
{CELL_NOT_CONFIGURED, LOGWARNING, MAC_SCHEDULER,\
"Either cell is not configured or L1 is not in active state",\
{"cellConfigured_g", "StoppedProcessing_g", "RecvFromPhyDone_g", "", "",\
"","",\
FNAME,"",\
}},
{MAC_CTRL_SAP_STATE_MACHINE_HANDLER, LOGDEBUG, MAC_RRC_INF,\
"Control SAP state machine called",\
{"ue_index", "State", "Event", "InternalCellId", "",\
"","",\
FNAME,"",\
}},
{MAC_CTRL_SAP_INVALID_EVENT_RECVD, LOGWARNING, MAC_RRC_INF,\
"Invalid Event received in CONTROL SAP thread",\
{"ue_index", "Event", "InternalCellId", "", "",\
"","",\
FNAME,"",\
}},
/* + SPR 16766 */
{MAC_QUEUE_EMPTY, LOGWARNING, MAC_RRC_INF,\
"Node dequeued is NULL",\
{"LineNum", "InternalCellId","nodesToProcess", "", "",\
"","",\
FNAME,"QName",\
}},
/* - SPR 16766 */
{MAC_UPDATE_UL_RRM_RPT, LOGINFO, MAC_UL_HARQ,\
"MAC_UPDATE_UL_RRM_RPT ",\
{"ue_index","value", "", "", "","","",\
FNAME,EVENT,\
}},
{OAM_MAC_INVALID_PARAM_RECV, LOGERROR, MAC_OAM_INF,\
"Invalid Parameter(s) value from OAM",\
{"LineNum", "ParamVal", "", "", "","","",\
FNAME,"Reason",\
}},
{MAC_DATA_TX_ID2,LOGBRIEF,MAC_L1_INF,\
"MAC_DATA_TX_ID2",\
{"ue_index","DciFormat","Tb1Size","Tb2Size","",\
"","",\
FNAME,EVENT,\
}},
{MAC_DATA_TX_ID3,LOGBRIEF,LOGDEBUG,\
"COMM_DATA_MSG_TX",\
{"ue_index","MsgType","DciFormat","TbSize","",\
"","",\
FNAME,EVENT,\
}},
{L2_TX_DATA_SEND,LOGDEBUG,MAC_L1_INF,\
"L2_TX_DATA_SEND",\
{"Length","NumPdu","","","",\
"","",\
FNAME,"",\
}},
{MAC_TB_HEADER_INFO,LOGBRIEF,MAC_L1_INF,\
"MAC Header Info",\
{"LineNum","ue_index","LcId","HeaderSize","Length",\
"","",\
FNAME,"",\
}},
{MAC_QOS_INSERT_NODE_PRIORITY_LC_TREE,LOGBRIEF,MAC_DL_Strategy,\
"Inserting node in Priority Lc Tree",\
{"ue_index","LcId","","","",\
"","",\
FNAME,"",\
}},
{MAC_QOS_DELETE_NODE_PRIORITY_LC_TREE,LOGBRIEF,MAC_DL_Strategy,\
"Deleting node from Priority Lc Tree",\
{"ue_index","LcId","","","",\
"","",\
FNAME,"",\
}},
{MAC_INSERT_SCH_LIST_NODE,LOGBRIEF,MAC_DL_Strategy,\
"Inserting node in ScheduledLcList",\
{"LineNum","ue_index","LcId","ScheduledLcCount","",\
"","",\
FNAME,"",\
}},
{MAC_DELETE_SCH_LIST_NODE,LOGBRIEF,MAC_DL_Strategy,\
"Deleting node from ScheduledLcList",\
{"LineNum","ue_index","LcId","ScheduledLcCount","",\
"","",\
FNAME,"",\
}},
{MAC_RRM_ECID_REPORT_REQ_INFO,LOGINFO,MAC_RRM_INF,\
"UE E_CID measurement report info",\
{"ue_index", "TypeOfReportReq","","","",\
"","",\
FNAME,"",\
}},
{MAC_RRM_ECID_REPORT_RES_INFO,LOGINFO,MAC_RRM_INF,\
"UE E_CID measurement report info response",\
{"ue_index", "RspCode","","","",\
"","",\
FNAME,"",\
}},
{MAC_PDCCH_ORDER_FOR_ECID_REPORT,LOGDEBUG,MAC_RRM_INF,\
"Pdcch order triggered for positioning data",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},
/*4x4 DL MIMO CHG START*/
{OAM_MAC_INVALID_CQI_THRESHOLD_VAL_MSG_ID,LOGWARNING,MAC_OAM_INF,\
"CQI Threshold is configured from OAM",\
{"Configured CQI Threshold value","","",\
 "","","","",\
FNAME,"",\
}},
{MAC_RRC_TRANS_MODE_ID,LOGERROR,MAC_L1_INF,\
"MAC_RRC_TRANS_MODE_ID",
{"UeCat","Mode","RI",\
 "","","","",\
FNAME,"",\
}},
{MAC_RRC_NUM_ANT_ID,LOGERROR,MAC_L1_INF,\
"MAC_RRC_NUM_ANT_ID",\
{"UeCat","Mode","RI",\
 "Num of Antenna","","","",\
FNAME,"",\
}},
{MAC_RECEIVED_PMI_LOG_ID,LOGERROR,MAC_L1_INF,\
"MAC_RECEIVED_PMI_LOG_ID",\
{"UeCat","Mode","RI",\
 "PMI","","","",\
FNAME,"",\
}},
/*4x4 DL MIMO CHG END*/
/* + PRS_CHANGES */
{MAC_RRC_PRS_CONFIG_INFO,LOGINFO,MAC_RRC_INF,\
"PRS is configured from RRC",\
{"PRS Bandwidth"," PRS Subframes"," PRS ConfigIndex", "PRSstartRB","PRSendRB",\
"","",\
FNAME,"",\
}},
/* - PRS_CHANGES - */
/*HSM_CHG_START*/
{HSM_MAC_OAM_REQ_FAILURE, LOGFATAL, L2_SYS_FAIL,\
"HSM_MAC_OAM_REQ_FAILURE",\
{"Thread ID", "","", "", "",\
"","",\
FNAME,EVENT,\
}},
{HSM_RLC_OAM_REQ_FAILURE, LOGFATAL, L2_SYS_FAIL,\
"HSM_RLC_OAM_REQ_FAILURE",\
{"Thread ID", "","", "", "",\
"","",\
FNAME,EVENT,\
}},
{HSM_PDCP_OAM_REQ_FAILURE, LOGFATAL, L2_SYS_FAIL,\
"HSM_PDCP_OAM_REQ_FAILURE",\
{"Thread ID", "","", "", "",\
"","",\
FNAME,EVENT,\
}},
{HSM_GTPU_OAM_REQ_FAILURE, LOGFATAL, L2_SYS_FAIL,\
"HSM_GTPU_OAM_REQ_FAILURE",\
{"Thread ID", "","", "", "",\
"","",\
FNAME,EVENT,\
}},
{HSM_OAM_MAC_INIT_FAILURE, LOGFATAL, L2_SYS_FAIL,\
"HSM_OAM_MAC_INIT_FAILURE",\
{"Mac HSM Init TimerInterval", "","", "", "",\
"","",\
FNAME,EVENT,\
}},
{HSM_OAM_RLC_INIT_FAILURE, LOGFATAL, L2_SYS_FAIL,\
"HSM_OAM_RLC_INIT_FAILURE",\
{"Rlc HSM Init TimerInterval", "","", "", "",\
"","",\
FNAME,EVENT,\
}},
{HSM_OAM_PDCP_INIT_FAILURE, LOGFATAL, L2_SYS_FAIL,\
"HSM_OAM_PDCP_INIT_FAILURE",\
{"Pdcp HSM Init TimerInterval", "","", "", "",\
"","",\
FNAME,EVENT,\
}},
{HSM_OAM_GTPU_INIT_FAILURE, LOGFATAL, L2_SYS_FAIL,\
"HSM_OAM_GTPU_INIT_FAILURE",\
{"Gtpu HSM Init TimerInterval", "","", "", "",\
"","",\
FNAME,EVENT,\
}},
{HSM_THREAD_FAILURE, LOGFATAL, L2_SYS_FAIL,\
"HSM_THREAD_FAILURE",\
{"Hsm Thread Failure", "","", "", "",\
"","",\
FNAME,EVENT,\
}},
/* Review comment fix start MAC_RESET_9 */
{MAC_DL_RESET_FAILURE_ID,LOGERROR,MAC_RRC_INF,\
"Failure while DL UE Reset: ",\
{"ue_index","LineNum","InternalCellId",\
"","","","",\
FNAME,EVENT,\
}},
{MAC_SENT_SYNC_IND_TO_RRC_ID,LOGINFO,MAC_RRC_INF,\
"MAC_SENT_SYNC_IND_TO_RRC",\
{"ue_index", "Rnti", "Sync Indication ", "InternalCellId", "",\
"", "", \
FNAME,"",\
}},
{MAC_MANDATORY_CONFIG_CELL_REQ_PARAM_SET1,LOGINFO,MAC_RRC_INF,\
"MAC_MANDATORY_CONFIG_CELL_REQ_PARAM_SET1",\
{"DuplexingMode", "dlResBlocks", "ulResBlocks", "MaxHarqRetrans", "NumOfTxAntennas",\
"", "", \
FNAME,EVENT,\
}},
{MAC_MANDATORY_CONFIG_CELL_REQ_PARAM_SET2,LOGINFO,MAC_RRC_INF,\
"MAC_MANDATORY_CONFIG_CELL_REQ_PARAM_SET2",\
{"ulCyclicLenthPrefix", "StartRARntiRange", "EndRARntiRange", "ModificationPeriod", "sfnGap",\
"", "", \
FNAME,EVENT,\
}},
{MAC_MANDATORY_CONFIG_CELL_REQ_PARAM_SET3,LOGINFO,MAC_RRC_INF,\
"MAC_MANDATORY_CONFIG_CELL_REQ_PARAM_SET3",\
{"NB", "DciFormat", "InternalCellId", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_TDD_CONFIG_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_TDD_CONFIG_INFO_PARAM",\
{"SubFrameAssign", "splSubFramePatterns", "InternalCellId", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_RACH_CONFIG_INFO_PARAM_SET1,LOGINFO,MAC_RRC_INF,\
"MAC_RACH_CONFIG_INFO_PARAM_SET1",\
{"RaResponseWindowSize", "contentionResolutionTimer", "MaxHARQMsg3Tx", "PrachConfigurationIndex", "prachFrequencyOffset",\
"", "", \
FNAME,EVENT,\
}},
{MAC_RACH_CONFIG_INFO_PARAM_SET2,LOGINFO,MAC_RRC_INF,\
"MAC_RACH_CONFIG_INFO_PARAM_SET2",\
{"NumCountBasedPreambles", "PreambleInitialReceivedTargetPower", "InternalCellId", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_PREAMBLE_GROUP_A_CONFIG_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_PREAMBLE_GROUP_A_CONFIG_PARAM",\
{"NumOfPreamblesInGroupA", "MsgSizeInGroupA", "InternalCellId", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_PHICH_CONFIG_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_PHICH_CONFIG_INFO_PARAM",\
{"phich_duration", "phich_resource", "InternalCellId", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_PUCCH_CONFIG_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_PUCCH_CONFIG_INFO_PARAM",\
{"DeltaPUCCH_Shift", "NRB_CQI", "nCS_AN", "n1PUCCH_AN", "InternalCellId",\
"", "", \
FNAME,EVENT,\
}},
{MAC_RBS_FOR_DCI_1A_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_RBS_FOR_DCI_1A_PARAM",\
{"RBSelectionTableDCI1A[mapCount]", "MapCount", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_RBS_FOR_DCI_1C_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_RBS_FOR_DCI_1C_PARAM",\
{"RBSelectionTableDCI1C[mapCount]", "MapCount", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_SRS_COMMON_CONFIG_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_SRS_COMMON_CONFIG_PARAM",\
{"SrsConfigType", "", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_SRS_COMMON_SETUP_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_SRS_COMMON_SETUP_INFO_PARAM",\
{"SrsBandwidthConfig", "srsSubframeConfig", "ackNackSRSSimultaneousTrans", "InternalCellId", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_SRS_MAX_UPPTS_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_SRS_MAX_UPPTS_PARAM",\
{"SrsMaxUpPts", "InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_PDSCH_CONFIG_COMMON_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_PDSCH_CONFIG_COMMON_INFO_PARAM",\
{"ReferenceSigPower", "PB", "InternalCellId", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_DOWNLINK_POWER_CONTROL_COMMON_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_DOWNLINK_POWER_CONTROL_COMMON_PARAM",\
{"PcfichPowerOffset", "PhichPowerOffset", "PdcchPowerOffset", "PchTransmissionPower", "InternalCellId",\
"", "", \
FNAME,EVENT,\
}},
{MAC_UPLINK_POWER_CONTROL_COMMON_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_UPLINK_POWER_CONTROL_COMMON_PARAM",\
{"P0NominalPUSCH", "alpha", "InternalCellId", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_PUSCH_CONFIG_COMMON_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_PUSCH_CONFIG_COMMON_PARAM",\
{"Enable64QAM", "InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_FREQUENCY_SELECTIVE_ENABLE_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_FREQUENCY_SELECTIVE_ENABLE_PARAM",\
{"FreqSelectiveSchUL", "freqSelectiveSchDL", "InternalCellId", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_UL_SYNC_LOSS_TIMER_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_UL_SYNC_LOSS_TIMER_INFO_PARAM",\
{"ulSyncLossTimer", "InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_NGAP_VALUE_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_NGAP_VALUE_PARAM",\
{"NGapValue", "InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_EXPLICIT_CELL_START_REQUIRED_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_EXPLICIT_CELL_START_REQUIRED_PARAM",\
{"ExplicitStartReq", "InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_POWER_CONTROL_ENABLE_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_POWER_CONTROL_ENABLE_PARAM",\
{"HarqBlerClpcPucchEnable", "cqiSinrClpcPucchEnable", "clpcPuschEnable", "PdcchPowOrAggregationEnable", "clpcPuschfreqSelectiveEnable",\
"", "", \
FNAME,EVENT,\
}},
{MAC_TPC_RNTI_RANGE_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_TPC_RNTI_RANGE_PARAM",\
{"StartTpcRntiPucch", "EndTpcRntiPucch", "StartTpcRntiPusch", "EndTpcRntiPusch", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_DYNAMIC_PDCCH_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_DYNAMIC_PDCCH_INFO_PARAM",\
{"isDynamicPdcchDisabled", "InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_SPS_CRNTI_RANGE_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_SPS_CRNTI_RANGE_PARAM",\
{"StartSpsCRnti", "EndSpsCRnti", "", "InternalCellId", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_PRS_CONFIG_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_PRS_CONFIG_INFO_PARAM",\
{"isPRSConfigured", "InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_PRS_BANDWIDTH_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_PRS_BANDWIDTH_INFO_PARAM",\
{"PrsBandwidth", "InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_PRS_SUBFRAMES_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_PRS_SUBFRAMES_INFO_PARAM",\
{"PrsSubframes", "InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_PRS_CONFIG_INDEX_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_PRS_CONFIG_INDEX_INFO_PARAM",\
{"PrsConfigIndex", "InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_PRS_TRANSMISSION_POWER_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_PRS_TRANSMISSION_POWER_INFO_PARAM",\
{"PrsTransmissionPower", "InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_PRS_MUTING_CONFIG_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_PRS_MUTING_CONFIG_INFO_PARAM",\
{"BitsTobeRead", "PrsMutingConfigIndex", "InternalCellId", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_MIB_MSG_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_PRS_MSG_INFO_PARAM",\
{"SFN", "NumMIBs", "DataSize", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_SIBTYPE1_MSG_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_SIBTYPE1_MSG_INFO_PARAM",\
{"SiMappingChanged", "SFN", "MsgLen", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_SI_MSG_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_SI_MSG_INFO_PARAM",\
{"SiWindowSize", "NumSIMsg", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_SI_MSG_INFO_PARAM_SET1,LOGINFO,MAC_RRC_INF,\
"MAC_SI_MSG_INFO_PARAM_SET1",\
{"SiIndex","siPeriodicity", "startSFN", "startSF", "siMsgSgmnts",\
"", "", \
FNAME,EVENT,\
}},
{MAC_NUM_SI_REPETITIONS_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_NUM_SI_REPETITIONS_PARAM",\
{"Index","SiMsgReptitions", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_NUM_SI_TRANSMISSIONS_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_NUM_SI_TRANSMISSIONS_PARAM",\
{"NumOfTransmissions","", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_SI_START_OFFSET_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_SI_START_OFFSET_PARAM",\
{"offsetIndex","SiStartOffset", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_SI_MSG_BUFFER_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_SI_MSG_BUFFER_PARAM",\
{"MsgLen","SiStartOffset", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_SCHEDULING_REQD_FOR_SIB8_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_SCHEDULING_REQD_FOR_SIB8_INFO_PARAM",\
{"Si8Flag","", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_MANDATORY_RECONFIG_CELL_REQ_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_MANDATORY_RECONFIG_CELL_REQ_PARAM",\
{"SFN","SF", "InternalCellId", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_DCI_FORMAT_FOR_SI_MSG_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_DCI_FORMAT_FOR_SI_MSG_PARAM",\
{"DciFormatForCommonChnlMsg","InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_UE_INACTIVITY_TIMER_CONFIG_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_UE_INACTIVITY_TIMER_CONFIG_PARAM",\
{"ueInactiveTimeVal","InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_CONTENTION_FREE_RACH_TIMER_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_CONTENTION_FREE_RACH_TIMER_INFO_PARAM",\
{"ContnFreeRachTimer","InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_SFN_GAP_INFO_PARAM,LOGINFO,MAC_RRC_INF,\
"MAC_SFN_GAP_INFO_PARAM",\
{"SfnGap","InternalCellId", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_MSG_SEG_NEXT_FAIL_CONFIG_CELL_REQ,LOGINFO,MAC_RRC_INF,\
"MAC_MSG_SEG_NEXT_FAIL_CONFIG_CELL_REQ",\
{"DestModuleId", "MsgId", "MsgLen", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_MSG_SEG_NEXT_FAIL_PCCH_CCCH_BCCH_UE_CON_REJ_REQ,LOGINFO,MAC_RRC_INF,\
"MAC_MSG_SEG_NEXT_FAIL_PCCH_CCCH_BCCH_UE_CON_REJ_REQ",\
{"DestModuleId", "MsgId", "MsgLen", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_INCORRECT_CRNTI_RECVD_ID,LOGERROR,MAC_L1_INF,\
"MAC_INCORRECT_CRNTI_RECVD_ID",\
{"MAC_UL_PDU_RESERVE_BIT1", "MAC_UL_PDU_RESERVE_BIT2", "RecvRNTI", "", "",\
"", "", \
FNAME,EVENT,\
}},
/* + SPR 16766 */
{MAC_UL_PDU_RESERVE_BIT_SET_ID,LOGERROR,MAC_L1_INF,\
" Unable to process the data received",\
{"MAC_UL_PDU_RESERVE_BIT1", "MAC_UL_PDU_RESERVE_BIT2", "recvdRNTI", "rntiFlag", "",\
"", "", \
FNAME,EVENT,\
}},
/* - SPR 16766 */
{MAC_PUCCH_MIN_RB_FAIL,LOGERROR,MAC_PUCCH,\
"MAC_PUCCH_MIN_RB_FAIL",\
{"ue_index", "PUCCH Resource Index", "UCI Format", "RB Index", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_VALIDATE_UE_PUCCH_RES_FAILURE_ID,LOGERROR,MAC_RRC_INF,\
"MAC_VALIDATE_UE_PUCCH_RES_FAILURE_ID",\
{"ue_index", "", "InternalCellId", "", "",\
"", "", \
FNAME,EVENT,\
}},
{PHY_CELL_CONFIG_TDD_PARAM_FAIL, LOGERROR, MAC_L1_INF,\
"PHY_CELL_CONFIG_TDD_PARAM_FAIL",\
{"Bitmask ", "SubFrameAssign ","splSubFramePatterns ", "", "",\
"","",\
FNAME,"",\
}},
{MAC_PARSING_ERROR, LOGERROR, MAC_RRC_INF,\
"MAC_PARSING_ERROR: MAC unable to parse the message",\
{"Parameter", "InternalCellId","LineNum", "", "",\
"","",\
FNAME,"Event",\
}},
{MAC_RESET_UE_ENTITY_REQ_ID, LOGERROR, MAC_RRC_INF,\
"MAC_RESET_UE_ENTITY_REQ",\
{"ue_index", "InternalCellId","", "", "",\
"","",\
FNAME,"",\
}},
{MAC_RRC_OAM_HPTOLP_THDH_ID, LOGDEBUG, MAC_RRC_INF,\
"MAC Send msg to Low Priority thread",\
{"RecvMsgFrmHPThdFd", "RspCode","RrcMsgId", "Event", "ue_index",\
"","",\
FNAME,"Event",\
}},
{MAC_UL_UE_CONTEXT_NULL,LOGERROR,MAC_PUCCH,\
"MAC_UL_UE_CONTEXT_NULL",\
{"ue_index", "", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_RESET_UE_ENTITY_FAILURE_CNF,LOGINFO,MAC_RRC_INF,\
"MAC_RESET_UE_ENTITY_FAILURE_CNF",\
{"ue_index", "Trans_Id", "RspCode ", "InternalCellId", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_RRM_INVALID_UE_INDEX_VALUE ,LOGWARNING, MAC_RRM_INF, \
"INVALID UE Index",\
{"ue_index", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_MU_MIMO_STATUS ,LOGDEBUG, MAC_RRM_INF, \
"INVALID MU MIMO STATUS",\
{"UeMUMIMOStatus", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRM_INVALID_MU_MIMO_NUM_ANTENNA_CONFIG ,LOGDEBUG, MAC_RRM_INF, \
"INVALID NUM ANTENNA CONFIG FOR MU MIMO",\
{"cellConfig_g.numOfTxAnteenas", "LineNum", "",\
 "", "", "",\
 "",\
FNAME,"", \
}},
{MAC_RRC_INVALID_MU_MIMO_STATUS,LOGDEBUG,MAC_RRC_INF,\
"INVALID_MU_MIMO_STATUS",\
{"UlMUMIMOStatus","", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_RRC_NUM_TX_ANTENNA_INVALID,LOGWARNING,MAC_RRC_INF,\
"INVALID_MU_MIMO_NUM_TX_ANTENNA",\
{"cellConfig_g.numOfTxAnteenas","", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_DL_UE_TO_BE_SCHEDULED,LOGBRIEF,MAC_L1_INF,\
"DL UEs to be scheduled in current TTI",\
{"DlUeToBeSchdInThisTick","","","", "",\
"",""\
,FNAME,"",\
}},
{MAC_UL_UE_TO_BE_SCHEDULED,LOGBRIEF,MAC_L1_INF,\
"UL UEs to be scheduled in the next occasion",\
/*+BUG:9222*/
{"UlUeToBeSchdInThisTick","","","", "",\
/*-BUG:9222*/
"",""\
,FNAME,"",\
}},
{MAC_RRM_RECONFIG_UE_TO_BE_SCHEDULED_DL_UL,LOGWARNING,MAC_L1_INF,\
"RRM Reconfigured MAX UEs to be scheduled DL and UL per TTI",\
/*+BUG:9222*/
{"ReconfigDlUeToBeSchdPerTTI","reconfigUlUeToBeSchdPerTTI","Success(1)/Failure(0)",\
/*-BUG:9222*/
"InternalCellId", "",\
"",""\
,FNAME,"",\
}},
/*- Dyn UEs To Be SchPerTTI -*/
{MAC_UL_HARQ_TIMER_STOP_FAIL, LOGWARNING, MAC_UL_HARQ,\
"Unable to stop the UL Harq Timer",\
{"LineNum","ue_index","RecvTTI", "SF", "HarqId",\
"","",\
FNAME,"",\
}},
{MAC_NO_ALLOC_PRACH_RES, LOGDEBUG ,MAC_PUCCH,\
"MAC cannot allocate PRACH resources",\
{"LineNum","PrachConfigIndex","PreambleFormat","SFN","SF",\
"","",\
FNAME,"",\
}},
{L2_CREATE_MEM_POOL_FAIL, LOGFATAL, L2_SYS_FAIL,\
"L2_CREATE_MEMFROMPOOL_FAIL", \
{"LineNum","Size","","","", \
"","",\
FILENAME,FNAME,\
}},
{ MAC_PHY_TIMEOUT, LOGWARNING, MAC_L1_INF,\
"MAC does not receive response from L1", \
{"LineNum", "Trans_Id", "State", "InternalCellId", "", \
"","",\
FNAME, "ResponseAwaitedFromL1For",\
}},
{ MAC_OAM_SEND_ERR_IND, LOGDEBUG, MAC_OAM_INF,\
"MAC sends error indication received from L1 to OAM", \
{"LineNum", "ApiId", "ApiLength", "typeOfError", "", \
"","",\
FNAME, "",\
}},
{ MAC_RECV_ERR_IND_FRM_PHY, LOGERROR, MAC_L1_INF,\
"MAC receives error indication from L1", \
{"LineNum", "MsgId", "RetVal", "", "", \
"","",\
FNAME, "",\
}},
{ MAC_DELETE_ON_UL_HARQ_TIMER_EXPIRY, LOGDEBUG, MAC_UL_HARQ,\
"MAC deletes harq node for timer expiry", \
{"LineNum", "ue_index", "HarqId", "UlSubframe", "", \
"","",\
FNAME, "",\
}},
{ MAC_ON_SFN_MISMATCH, LOGDEBUG, MAC_L1_INF,\
"MAC will perform action for following variables on receipt of SFN mismatch",\
{"LineNum", "totalTickMissErrInd", "HarqRttTimerFactor", "ErrorWindow", "", \
"","",\
FNAME, "Direction",\
}},
{ MAC_REVERT_POWER_CONTROL, LOGDEBUG, MAC_L1_INF,\
"MAC revert Power Control parameters for Error received",\
{"MapIndex", "ue_index", "currPowerCorrectionVal", "PowerCorrectionVal", \
"NumOfUe", "","",\
FNAME, "PowerControlTriggerFor",\
}},
{ MAC_NO_ACTION_UL_CONFIG_ERR_PDU, LOGDEBUG, MAC_L1_INF,\
"MAC will perform no action as error occured in MCH or BCH PDU",\
{"LineNum", "Rnti", "", "", "", \
"","",\
FNAME, "",\
}},
{ MAC_UL_CONFIG_ERR_PDU, LOGDEBUG, MAC_L1_INF,\
"MAC found UL Config PDU for which error received",\
{"LineNum", "Rnti", "PduType", "totalNumOfPdu", "PduNum", \
"","",\
FNAME, "",\
}},
{MAC_NOT_EQ_SFN_SF_IN_SFI_WITH_L2_TICK, LOGWARNING, MAC_L1_INF,\
"SFI subframe or system frame not matched with L2 Tick",\
{"LineNum","SF","SFN", "L2_subframe", "L2_sysframe",\
"","",\
FNAME,"",\
}},
{ MAC_PUT_ENTRY_IN_TPC, LOGDEBUG, MAC_UL_PC,\
"Put entry in TPC sent global map",\
{"MapIdx","Rnti","PowerCtrlType", "powerCorrectionVal", "NumOfUe",\
"","",\
FNAME,"",\
}},
/* SPR 16143 fix start */
{ MAC_SR_INFO_DELETE, LOGINFO, MAC_RRC_INF,\
"SR reconfiguration received to release SR",\
{"ue_index", "srPUCCHRBIndex","srPeriodicity", "srOffset", "IndexForPUCCHMap",\
"","",\
FNAME,"",\
}},
{ MAC_SR_INFO_RECONFIG, LOGINFO, MAC_RRC_INF,\
"SR reconfiguration received to reconfig SR",\
{"ue_index", "srPUCCHRBIndex", "srPeriodicity", "srOffset", "IndexForPUCCHMap",\
"","",\
FNAME,"",\
}},
{ MAC_RECONFIGURE_SR_NODE_MAP, LOGINFO, MAC_PUCCH,\
"Reconfigure SR node map: check for srPeriodicity",\
{"ue_index", "longPeriodicSRCQIPMIRIMapInfoIndex", "shortPeriodicSRMapShortInfoIndex", "srPeriodicity (InContext)", "Recvd srPeriodicity",\
"","",\
FNAME,"",\
}},
{ MAC_RECONFIGURE_SR_NODE_MAP_1, LOGINFO, MAC_PUCCH,\
"Reconfigure SR node map: check for srOffset",\
{"ue_index", "longPeriodicSRCQIPMIRIMapInfoIndex", "shortPeriodicSRMapShortInfoIndex", "srOffset (InContext)", "Recvd srOffset",\
"","",\
FNAME,"",\
}},
{ MAC_RECONFIGURE_SR_NOT_REQD, LOGINFO, MAC_PUCCH,\
"No need to Reconfigure SR Reports. All parameters are same",\
{"ue_index", "longPeriodicSRCQIPMIRIMapInfoIndex", "shortPeriodicSRMapShortInfoIndex", "srPeriodicity", "srOffset",\
"","",\
FNAME,"",\
}},
{ MAC_CREATE_SR_MAP, LOGINFO, MAC_PUCCH,\
"Create SR map",\
{"ue_index", "longPeriodicSRCQIPMIRIMapInfoIndex", "shortPeriodicSRMapShortInfoIndex", "srPeriodicity", "srOffset",\
"","",\
FNAME,"",\
}},
{ MAC_HP_THREAD_PROCESS_RRC_MSG, LOGINFO, MAC_RRC_INF,\
"Switch active and passive index at HP thread",\
{"LineNum", "ue_index", "activeIndex", "passiveIndex", "",\
"","",\
FNAME,"ReconfigEvent",\
}},
/* SPR 16143 fix end */
/* SPR 16583 fix start */
{ MAC_RNTI_NOT_SAME, LOGWARNING, MAC_RRC_INF,\
"Crnti received from L3 is not same as present in UE context",\
{"ue_index", "CRNTI Received", "CRNTI in DL Context", "", "",\
"","",\
"FunctionName","L3Event",\
}},
/* SPR 16583 fix start */
{ MAC_RECV_ERR_FOR_TTIB_DATA, LOGDEBUG, MAC_L1_INF,\
"MAC will not stop UL HARQ timer and process CRC for TTIB UE for first 3 ticks"
" of Bundle", \
{"LineNum", "ue_index", "UlHpId", "ulschRecvTick", "TTIBFirstTick", \
"","",\
FNAME, "",\
}},
/* SPR 16766 fix start */
{MAC_MUX_ENCODE_FAIL_ID,LOGWARNING,MAC_MUX,\
"Multilpexer is failing to send opportunity to RLC",\
{"ue_index","crnti","LcCount","availableTBSize","V2",\
"","",\
FNAME,"Event",\
}},
{MAC_UE_DATA_REQ_ID,LOGBRIEF,MAC_MUX,\
"MAC_UE_DATA_REQ",\
{"ue_index","harqId","LcCount","sizeTBOne","sizeTBTwo",\
"","",\
FNAME,"",\
}},
{MAC_UE_DATA_REQ_EACH_LC_ID,LOGBRIEF,MAC_MUX,\
"MAC_UE_DATA_REQ_EACH_LC",\
{"lcId","statusPduLen","newPduLen","totalPduLen","remainDataSize",\
"","",\
FNAME,"",\
}},
{MAC_SEND_CCCH_TO_RRC_ID, LOGWARNING, MAC_CCCH,\
"MAC_SEND_CCCH_TO_RRC",\
{"RNTI", "datalen", "TA", "SINR", "cellIndex",\
"","",\
FNAME,EVENT,\
}},
{MAC_UL_CCCH_MSG_ID, LOGINFO, MAC_CCCH,\
"MAC_UL_CCCH_MSG",\
{"RNTI", "ue_index", "datalen", "cellIndex", "pmbstatus",\
"","",\
FNAME,"",\
}},
{MAC_UL_PHR_LCH_MSG_ID, LOGBRIEF, MAC_PUSCH,\
"MAC_UL_PHR_LCH_MSG",\
{"RNTI", "ue_index", "phr", "cellIndex", "",\
"","",\
FNAME,"",\
}},
{MAC_UL_SHORT_OR_TRUNCATED_BSR_LCH_MSG_ID, LOGBRIEF, MAC_PUSCH,\
"MAC_UL_SHORT_OR_TRUNCATED_BSR_LCH_MSG",\
{"ue_index", "bsrReport[0]", "bsrReport[1]","bsrReport[2]","bsrReport[3]"\
"","",\
FNAME,"",\
}},
{MAC_UL_LONG_BSR_LCH_MSG_ID, LOGBRIEF, MAC_PUSCH,\
"MAC_UL_LONG_BSR_LCH_MSG",\
{"ue_index", "lcgId", "bsrVal","bsrNetTot","cellIndex"\
"","",\
FNAME,"",\
}},
{ MAC_NULL_NODE, LOGWARNING, MAC_CCCH,\
"MAC node is null",\
{"LineNum", "", "", "", "",\
"","",\
FNAME,"QueueName",\
}},
{ MAC_DROP_CCCH_NODE, LOGWARNING, MAC_CCCH,\
"MAC is dropping CCCH node",\
{"LineNum", "ue_index", "permissibleTBSize", "ueContextPresentFlag", "rejFlag",\
"","",\
FNAME,"",\
}},
{ MAC_DROP_CCCH_NODE_1, LOGWARNING, MAC_CCCH,\
"MAC is dropping CCCH node more params",\
{"Rnti", "RequiredRB", "availableDLRBCount", "tbSizeFound", "",\
"","",\
FNAME,"",\
}},
{ MAC_CCCH_CCE_UNAVAILABLE, LOGWARNING, MAC_CCCH,\
"CCEs are not available",\
{"LineNum", "rnti", "availableCCE", "", "",\
"","",\
FNAME,"SearchSpace",\
}},
{ MAC_AGG_LVL_MORE_THAN_AV_CCE, LOGWARNING, MAC_CCCH,\
"aggregationLevel is more than availableCCE",\
{"LineNum", "Rnti", "aggregationLevel", "availableCCE", "",\
"","",\
FNAME,"SearchSpace",\
}},
{ MAC_TCRNTI_ALLOC, LOGINFO, MAC_RACH,\
"MAC allocated new TCRNTI",\
{"LineNum", "Rnti", "", "", "",\
"","",\
FNAME,"",\
}},
/* SPR 17112 Fix Start */
{ MAC_TX_DLSCH_UE_CAT, LOGERROR, MAC_L1_INF,\
"MAC_TX_DLSCH_UE_CAT",\
{"ueCategory", "ue_index", "rnti", "", "",\
"","",\
FNAME," ",\
}},
/* SPR 17112 Fix End */
/* SPR 16916 FIX START */
{ MAC_DL_ACTIVE_UE_QCI_LOG_ID, LOGDEBUG, MAC_DL_Strategy,\
"MAC_DL_ACTIVE_UE_QCI_LOG_ID", \
{"ue_index", "lcId", "qci", "V1", "V2", \
"","",\
FNAME, " ",\
}},
{ MAC_UL_ACTIVE_UE_QCI_LOG_ID, LOGDEBUG, MAC_UL_Strategy,\
"MAC_UL_ACTIVE_UE_QCI_LOG_ID", \
{"ue_index", "lcId", "qci", "lcgId", "V2", \
"","",\
FNAME, " ",\
}},
/* SPR 16916 FIX END */
{ MAC_TOT_ACTIVE_UE_ZERO, LOGERROR, MAC_RRC_INF,\
"Total Active UE has reached zero but delete request is received",\
{"LineNum", "ue_index", "", "", "",\
"","",\
FNAME,"",\
}},
/* SPR 16766 fix end */
{MAC_INVALID_HARQ,LOGERROR,MAC_DL_HARQ,\
"MAC_INVALID_HARQ",\
{"SubframeNumber","tempHarqID","ue_index","","",\
"","",\
FNAME,EVENT,\
}}, 
{MAC_PUSCH_FAILURE_SECOND_EXECUTION, LOGDEBUG, MAC_PUSCH,\
"MAC PUSCH Failure Info for UE for TDD Config 0, having common DCI",\
{"ue_index", "LocationType","MinStrictRBReq", "RetAvailableRBLength", "minAvgMCS",\
"","",\
FNAME,EVENT,\
}},
/* CA changes start */
{MAC_UE_SCELL_ERR_RESPONSE, LOGDEBUG, MAC_RRC_INF,\
"MAC_UE_SCELL_ERR_RESPONSE", \
{"Status","Opcode","Result","","", \
"","",\
FILENAME,FNAME,\
}},
/* CA changes start */
{MAC_INVALID_HARQ_ID,LOGWARNING,MAC_L1_INF,\
"MAC_INVALID_HARQ_ID",\
{"ue_index", "", "", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_HI_NUM_ACK_NACK,LOGDEBUG,MAC_L1_INF,\
"MAC_HI_NUM_ACK_NACK",\
{"Rnti", "NumOfAckNack", "RecvTTI", "", "",\
"", "", \
FNAME,EVENT,\
}},
{MAC_HI_MODE,LOGDEBUG,MAC_L1_INF,\
"MAC_HI_MODE",\
{"Rnti", "Mode","RecvTTI","", "",\
"", "", \
FNAME,EVENT,\
}},
{PHY_CELL_PRS_CONFIG_FAIL, LOGERROR, MAC_RRC_INF,\
"PRS Info Received for Release 8 L1 in  PHY  Cell Config",\
{ "bitmask","Release capability ","", "", "",\
"","",\
FNAME,"",\
}},
{MAC_INVALID_CELL_ID,LOGERROR, MAC_RRC_INF,\
"Invalid Cell Id",\
{ "cell_index","","", "", "",\
"","",\
FNAME,"",\
}},
{MAC_SCELL_NOT_ACTIVE,LOGERROR, MAC_PUCCH,\
"SCell not active",\
{ "cell_index","","", "", "",\
"","",\
FNAME,"",\
}},
{MAC_INVALID_TRANSMISSION_MODE,LOGERROR, MAC_RRC_INF,\
"Invalid Transmission Mode Received for Release 8 L1 ",\
{ "TransmissionMode","Release capability ","InternalCellId", "", "",\
"","",\
FNAME,"",\
}},
/* SPR 11230 Start */
{ MAC_UNABLE_STOP_DL_HARQ_TIMER, LOGWARNING, MAC_DL_HARQ,\
"MAC Unable to stop the DL HARQ timer",\
{"DlUEContext_p", "ContainerId","currentULSubFrame", "isErrForNode", "",\
"","",\
FNAME,"Cause",\
}},
{ MAC_DL_CONFIG_ERR_RECVD, LOGDEBUG, MAC_L1_INF ,\
"MAC set the ErrNode in HarqTimerList",\
{"LineNum", "ue_index", "indexSfn", "DlAckNackSf", "",\
"","",\
FNAME,"",\
}},
{ MAC_EL_DL_EXPIRY, LOGDEBUG, MAC_DL_HARQ,\
"MAC shall perform DL timer expiry as per following variables",\
{"FrameNumber", "loopNumberOfSubFrames","freeHarqOnly", "totalTickMissErrIndDl", "totalNumberOfTickDiffDl",\
"","",\
FNAME,"",\
}},
/* SPR 11230 End */
{ MAC_LOAD_WISE_PREF_CELL_LIST, LOGDEBUG, MAC_DL_Strategy,\
"Adding Cells in Preferred cell list",\
{"ue_index", "PrefCellId","currentLoadPcell", "currentLoadScell", "loadDisparityThreshold",\
"","",\
FNAME,"",\
}},
{ MAC_MCS_WISE_PREF_CELL_LIST, LOGDEBUG, MAC_DL_Strategy,\
"Adding Cells in Preferred cell list",\
{"ue_index", "PrefCellId","cellIdWithHighestMcs", "McsDiffThreshold", "",\
"","",\
FNAME,"",\
}},
{ MAC_PREF_CELL_LIST, LOGDEBUG, MAC_DL_Strategy,\
"Adding Cells in Preferred cell list",\
{"ue_index", "PrefCellId","priorityLcNodeCellIndex", "", "",\
"","",\
FNAME,"",\
}},
/* + SPR 13605 Fix */    
{ MAC_APPLY_PREF_FACTOR_1, LOGDEBUG, MAC_DL_Strategy,\
"After applying preference factor among cells",\
{"ue_index", "LcId","PrefCellId", "SchedulableTokensPcell", "schedulableTokensScell",\
"","",\
FNAME,"",\
}},
{ MAC_APPLY_PREF_FACTOR_2, LOGDEBUG, MAC_DL_Strategy,\
"After applying preference factor among cells",\
{"ue_index", "NumOfTokensBorrowed","MaxBorrowableTokens", "numOfTokensNeedToBorrow", "othreCellSchedulableTokens",\
"","",\
FNAME,"",\
}},
{ MAC_APPLY_PREF_FACTOR_3, LOGDEBUG, MAC_DL_Strategy,\
"After applying preference factor among cells",\
{"ue_index", "LcId","PrefCellId", "prefCellSchedulableTokens", "MaxBytesAvailableAsPerRestrictedRb",\
"","",\
FNAME,EVENT,\
}},
{ MAC_APPLY_PREF_FACTOR_4, LOGDEBUG, MAC_DL_Strategy,\
"After applying preference factor among cells",\
{"ue_index", "LcId","PrefCellId", "prefCellSchedulableTokens", "totalSchedulableBytes",\
"","",\
FNAME,EVENT,\
}},
/* LOG LEVEL change*/    
{ MAC_SCHD_LC_ON_PREF_CELL_1, LOGDEBUG, MAC_DL_Strategy,\
"Scheduled LC on Preferred cells",\
{"ue_index", "StrategyNode's maxTBSize","UE's maxTBSize", "servingCellIndex", "totalSchBytes",\
"","",\
FNAME,"",\
}},
{ MAC_SCHD_LC_ON_PREF_CELL_2, LOGDEBUG, MAC_DL_Strategy,\
"Scheduled LC on Preferred cells",\
{"ue_index", "ServingCellAvailToken","servingCellSchedulableBytes", "schedulableBytes", "DeltaRB",\
"","",\
FNAME,"",\
}},
{ MAC_SCHD_LC_ON_PREF_CELL_3, LOGDEBUG, MAC_DL_Strategy,\
"Scheduled LC on Preferred cells",\
{"ue_index", "LcId","SchedulableBytes", "ServCell_schGbrBytes", "OtherCell_schGbrBytes",\
"","",\
FNAME,"",\
}},
{ MAC_SCHD_LC_ON_PREF_CELL_4, LOGDEBUG, MAC_DL_Strategy,\
"Scheduled LC on Preferred cells",\
{"ue_index", "ServCell_AggGbrTTI","OtherCell_AggGbrTTI", "GbrSchFlag", "ReStartTraversingFlag",\
"","",\
FNAME,"",\
}},
{ MAC_SCHD_LC_ON_PREF_CELL_5, LOGDEBUG, MAC_DL_Strategy,\
"Scheduled LC",\
{"ue_index", "LcId","Lcweight", "totalLcToken", "LcQLoad",\
"","",\
FNAME,"",\
}},
/* LOG LEVEL change*/    
/* - SPR 13605 Fix */
{ MAC_ALLOCATED_RBS_FOR_RETX, LOGDEBUG, MAC_DL_Strategy,\
"RBs allocated to UE for Retx",\
{"ue_index", "InternalCellId","cell_index for Retx", "Rbs allocated", "HarqId",\
"","",\
FNAME,"",\
}},
{ MAC_RB_ALLOCATION_FAIL_FOR_RETX, LOGWARNING, MAC_DL_Strategy,\
"RB allocation failed for Retx",\
{"ue_index", "InternalCellId","cell_index for Retx", "HarqId", "No. of Cells for Retx",\
"","",\
FNAME,"",\
}},
{ MAC_PREF_CELL_LIST_FOR_RETX, LOGDEBUG, MAC_DL_Strategy,\
"Preferred Cell list prepared for Retx",\
{"ue_index", "PrefCellId","preferredCellListCount", "HarqId", "SCellCount for Retx",\
"","",\
FNAME,"",\
}},
{ MAC_RBS_ASSIGNED_FOR_RETX_1, LOGDEBUG, MAC_DL_Strategy,\
"RBs allocated for Retx",\
{"ue_index", "TB1_TxInd","TB2_TxInd", "ScheduleFlag", "HarqId",\
"","",\
FNAME,"",\
}},
{ MAC_RBS_ASSIGNED_FOR_RETX_2, LOGDEBUG, MAC_DL_Strategy,\
"RBs allocated for Retx",\
{"ue_index", "TB1_TxInd","ScheduleFlag", "HarqId", "tm6DCIFormat",\
"","",\
FNAME,"",\
}},
{ MAC_RBS_ASSIGNED_FOR_RETX_3, LOGDEBUG, MAC_DL_Strategy,\
"RBs allocated for Retx",\
{"ue_index", "TB1_TxInd","ScheduleFlag", "HarqId", "LineNum",\
"","",\
FNAME,"",\
}},
{ MAC_PREF_CELL_LIST_FOR_RETX_FAIL, LOGDEBUG, MAC_DL_Strategy,\
"Preferred Cell list prepared for Retx failure",\
{"ue_index", "PrefCellId","cellIdTobeAdded", "No. of preferredCells", "ueCount in preferredCellList",\
"","",\
FNAME,"",\
}},
{ MAC_SCHD_SCELL_ACT_DEACT_CE, LOGDEBUG, MAC_DL_Strategy,\
"Scell Act/deact CE Scheduled",\
{"ue_index", "PrefCellId","HarqId", "assignedResourcesNB", "dlCAQosEvent",\
"","",\
FNAME,"",\
}},
/* CA TDD Changes Start */
{ MAC_VALIDATE_TDD_CONFIG_CA_ENABLED_CELLS, LOGDEBUG, MAC_RRC_INF,\
"Special Subframe pattern/Subframe Assignment values are different for both CA enabled cells",\
{"InternalCellId1", "SubframeAssign1","SpclSubframePatterns1", "InternalCellId2", "SubframeAssign2",\
"SpclSubframePatterns2","",\
FNAME,"",\
}},
/* CA TDD Changes Start */

/* SPR 12055 Fix Start */
{ OAM_MAC_INVALID_SPS_QCI_MAP, LOGERROR, MAC_OAM_INF,\
"OAM enables SPS for NON_GBR QCI",\
{"LineNum", "QCI", "", "", "",\
"","",\
FNAME,"",\
}},
/* SPR 12055 Fix End */
{ MAC_LC_ALREADY_SCHEDULED, LOGERROR, MAC_DL_Strategy,\
"LC already scheduled",\
{"ue_index", "LcId", "SchedulableBytes", "", "",\
"","",\
FNAME,"",\
}},
{ MAC_MUX_LC_1, LOGDEBUG, MAC_DL_Strategy,\
"Multiplex LCs",\
{"ue_index", "LcId", "SchedulableBytes", "total_requestedDataSize", "ReDoForNextTB",\
"","",\
FNAME,"",\
}},
{ MAC_MUX_LC_2, LOGDEBUG, MAC_DL_Strategy,\
"Multiplex LCs",\
{"Tb1Size", "Tb2Size", "lcCountTBOne", "lcCountTBTwo", "localNumTB",\
"","",\
FNAME,"",\
}},
{ MAC_MUX_LC_3, LOGDEBUG, MAC_DL_Strategy,\
"Multiplex LCs",\
{"bytesAllocated", "total_requestedDataSize", "SchedulableBytes", "Tb1Size", "Tb2Size",\
"","",\
FNAME,"",\
}},
{ MAC_MUX_LC_4, LOGDEBUG, MAC_DL_Strategy,\
"Multiplex LCs",\
{"ue_index", "LcId", "lcCountTBOne", "lcCountTBTwo", "total_requestedDataSize",\
"","",\
FNAME,"",\
}},
{ MAC_MUX_LC_5, LOGDEBUG, MAC_DL_Strategy,\
"Multiplex LCs",\
{"LcId", "QLoad", "total_requestedDataSize", "availableTBSize", "tempArrCount",\
"","",\
FNAME,"",\
}},
{ MAC_MUX_LC_6, LOGDEBUG, MAC_DL_Strategy,\
"Multiplex LCs",\
{"LcId", "QLoad", "total_requestedDataSize", "availableTBSize", "tbIndex",\
"","",\
FNAME,"",\
}},
{ MAC_MUX_LC_7, LOGDEBUG, MAC_DL_Strategy,\
"Multiplex LCs",\
{"LcId", "QLoad", "total_requestedDataSize", "availableTBSize", "arrayIndex",\
"","",\
FNAME,"",\
}},
{ MAC_MUX_LC_8, LOGDEBUG, MAC_DL_Strategy,\
"Multiplex LCs",\
{"ue_index", "Tb1Size", "Tb2Size", "ScheduledLcCount", "",\
"","",\
FNAME,"",\
}},
{ MAC_ALLOC_PRACH_RES, LOGDEBUG, MAC_PUCCH,\
"MAC allocate UL resources for PRACH",\
{"PrachFrequencyOffset", "NumPhysicalResources", "PreambleFormat", \
"ulSysFrameNum", "ulSubFame",\
"","",\
FNAME,"",\
}},
{ MAC_PRACH_RB_MORE, LOGWARNING, MAC_RRC_INF,\
"Number of RBs required for PRACH is more than the total RBs", \
{"SubFrameAssign", "totalRbsReqdForPrach", "ulResBlocks", \
"PrachFrequencyOffset", "PrachConfigIndex",\
"","",\
FNAME,"",\
}},
/* SPR 13903 fix start */
{MAC_CHECK_CODE_RATE_FAILURE_LOG_ID,LOGDEBUG,MAC_PDSCH,\
"Check Code rate for UE category ",\
{"ue_index","RatPolicy ","allocatedRB ",\
"McsCW1 ","McsCW2 ",\
"","",\
FNAME, "",\
} },
{MAC_UL_SPS_CURR_STATE, LOGINFO, MAC_UL_SPS_STRATEGY,\
"SPS state in system",\
{"LineNum", "ue_index", "OldState", " Event", " NewState",\
"","",\
FNAME," FSM:",\
}},
{FAPI_INTF_LOG_ID, LOGDEBUG, MAC_L1_INF,
"FAPI_INTF_LOG",
{"LineNum",FNAME,"Message","","",
"","",
"","",
}},
/* SPR 16962 fix start */
{ MAC_DELETE_UE_CONTEXT_ON_OWN, LOGERROR, MAC_RRC_INF,\
"MAC is deleting the UE context at Cell stop/delete which should be triggered "
"by L3 ",\
{"LineNum", "ue_index", "crnti", "dlUeCtx_p", "ulUeCtx_p",\
"","",\
"FunctionName","",\
}},
/* SPR 16962 fix end */
   
/* SPR 13903 fix end */
/* PDCP LOGS */
{PDCP_CONTEXT_ALREADY_INITIALIZED, LOGWARNING, PDCP_RRCOAM,\
"PDCP context is already initialized",\
{"LineNum","", "", "", "",\
"","",\
FNAME,"",\
}},
{ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM,\
"ROHC context is not used",\
{"LineNum","v1", "v2", "v3", "v4",\
"","",\
FNAME,"",\
}},
{PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM,\
"PDCP context is not initialized",\
{"LineNum","", "", "", "",\
"","",\
FNAME,"",\
}},
{PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,\
"Unable to find the PDCP context at TX side",\
{"LineNum","ue_index", "LcId", "", "",\
"","",\
FNAME,"",\
}},
{PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING, PDCP_RX,\
"Unable to find the PDCP context at RX side",\
{"LineNum","ue_index","LcId","", "",\
"","",\
FNAME,"",\
}},
{PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM,\
"Current UE level transmitting state",\
{"LineNum","ue_index","OldState"," Event"," NewState",\
"LcId","",\
FNAME," FSM:",\
}},
{PDCP_UE_CONTEXT_STATE_RX, LOGINFO, PDCP_RRCOAM,\
"Current UE level receiving state",\
{"LineNum","ue_index","RxState","","",\
"", "",\
FNAME,"",\
}},
{PDCP_DISCARD_TIMER_ASSOCIATED, LOGINFO, PDCP_TX,\
"Associating PDCP SDU with discardTimer tick",\
{"LineNum","ue_index","LcId","CurTick", "PacketDuration",\
"","",\
FNAME,"",\
}},
{PDCP_NO_DISCARD_TIMER_ASSOCIATED, LOGDEBUG, (PDCP_TX | PDCP_RRCOAM),\
"DiscardTimer Tick not associated ",\
{"LineNum","ue_index","LcId","PacketDuration", "",\
"","",\
FNAME,"",\
}},
{PDCP_SDU_SN_HFN_TX, LOGDEBUG, PDCP_TX,\
"Associating SN, HFN to transmitted packet->", \
{"ue_index","LcId","SN","HFN","Count",\
"", "",\
FNAME,"",\
}},
{PDCP_SDU_SN_HFN_RX, LOGDEBUG, PDCP_RX,\
"Fetching SN, HFN of received packet->", \
{"LineNum","ue_index","LcId","SN","HFN",\
"","",\
FNAME,"",\
}},
{PDCP_SDU_COUNT, LOGDEBUG, PDCP_RX,\
"Fetching count of recieved packet->", \
{"LineNum","ue_index","LcId","Count","",\
"", "", \
FNAME,"",\
}},
{PDCP_ROHC_COMPRESSION_FAILED, LOGWARNING, PDCP_TX, \
"ROHC Compression failed for PDCP SDU->", \
{"LineNum","ue_index","LcId","Count", "",\
"","",\
FNAME,"",\
}},
{PDCP_CIPHERING_FAILED, LOGWARNING, PDCP_TX, \
"Ciphering Failed for PDCP SDU->", \
{"LineNum","ue_index","LcId","Count", "Size",\
"","",\
FNAME,"",\
}},
{PDCP_ROHC_DECOMPRESSION_FAILED, LOGWARNING, PDCP_RX,\
"ROHC Decompression failed for PDCP PDU->", \
{"LineNum","ue_index","LcId","Count", "",\
"","",\
FNAME,"",\
}},
{PDCP_DECIPHERING_FAILED, LOGWARNING, PDCP_RX,\
"Deciphering Failed for PDCP PDU->", \
{"LineNum","ue_index","LcId","Count", "Size",\
"","",\
FNAME,"",\
}},
{PDCP_INTEGRITY_VERIFICATION_FAILED, LOGWARNING, PDCP_RX,\
"Integrity Verification failed for PDCP PDU->", \
{"LineNum","ue_index","LcId","Count", "",\
"","",\
FNAME,"",\
}},
{PDCP_PDU_FROM_PDCP_TO_RLC, LOGDEBUG, PDCP_TX, \
"Sending PDU from PDCP to RLC->", \
{"LineNum","ue_index","LcId","Count", "",\
"","",\
FNAME,"",\
}},
{PDCP_PDU_FROM_RLC_TO_PDCP, LOGDEBUG, PDCP_RX,\
"Enqueueing PDU from RLC to PDCP->", \
{"LineNum","ue_index","LcId","", "",\
"","",\
FNAME,"",\
}},
{PDCP_HEADER_APPENDED, LOGDEBUG, PDCP_TX, \
"PDCP Header appended with value", \
{"LineNum","ue_index","LcId","Value", "",\
"","",\
FNAME,"",\
}},
{PDCP_DATA_BEFORE_CIPHERING, LOGDEBUG, PDCP_TX, \
"Data Before ciphering (last 4 bytes only)", \
{"LineNum","ue_index","LcId","Value", "",\
"","",\
FNAME,"",\
}},
{PDCP_DATA_AFTER_DECIPHERING, LOGDEBUG, PDCP_TX, \
"Data after deciphering (last 4 bytes only)", \
{"LineNum","ue_index","LcId","Value", "",\
"","",\
FNAME,"",\
}},
{PDCP_DATA_PDU_FROM_RLC, LOGDEBUG, PDCP_RX,\
"Receiving data PDU from RLC->", \
{"LineNum","ue_index","LcId","", "",\
"","",\
FNAME,"",\
}},
{PDCP_CONTROL_PDU_FROM_RLC, LOGDEBUG, PDCP_RX,\
"Receiving Control PDU from RLC->", \
{"LineNum","ue_index","LcId","", "",\
"","",\
FNAME,"",\
}},
{PDCP_DUPLICATE_PACKET_RECEIVED_FROM_RLC, LOGWARNING, PDCP_RX,\
"Duplicate packet received from RLC->", \
{"LineNum","ue_index","LcId","Count", "",\
"","",\
FNAME,"",\
}},
{PDCP_SDU_SIZE_GREATER_THAN_LIMIT, LOGWARNING, PDCP_RX,\
"Dropping PDCP SDU as size > 8188 octets->", \
{"LineNum","ue_index","LcId","SduSize", "Count",\
"","",\
FNAME,"",\
}},
{PDCP_SDU_SENT_UPPER_LAYER, LOGDEBUG, PDCP_RX,\
"Sending PDCP SDU to RRC/GTP-U->", \
{"LineNum","ue_index","LcId","Count", "", \
"","",\
FNAME,"",\
}},
{PDCP_RLC_STATUS_IND_RECVD, LOGDEBUG, PDCP_TX, \
"Receiving status ind from RLC-> ", \
{"LineNum","ue_index","LcId","Trans_Id","DeliveryStatus",\
"", "", \
FNAME,"",\
}},
{PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_TX, \
"Received Data Pointer is NULL", \
{"LineNum","ue_index","LcId","Count", "",\
"","",\
FNAME,EVENT, \
}},
{PDCP_MSG_ALLOC_FAILED, LOGERROR, L2_SYS_FAIL, \
"MsgAlloc Failed", \
{"LineNum","Size","ue_index","LcId", "Count",\
"","",\
FNAME,EVENT, \
}},
{PDCP_MSG_EXTRACT_FAILED, LOGERROR, L2_SYS_FAIL, \
"MsgExtract Failed", \
{"LineNum","Size","ue_index","LcId", "Count",\
"","",\
FNAME,EVENT, \
}},
{PDCP_MSG_INSERT_FAILED, LOGERROR, L2_SYS_FAIL, \
"MsgInsert Failed", \
{"LineNum","Size","ue_index","LcId", "Count",\
"","",\
FNAME,EVENT, \
}},
{PDCP_MSG_CLONE_FAILED, LOGERROR, L2_SYS_FAIL, \
"MsgClone Failed", \
{"LineNum","Size","ue_index","LcId", "Count",\
"","",\
FNAME,"",\
}},
{PDCP_MSG_REMOVE_FAILED, LOGERROR, L2_SYS_FAIL, \
"MsgRemove Failed", \
{"LineNum","ue_index","LcId","Size to be removed", "Count",\
"","",\
FNAME,EVENT, \
}},
{PDCP_MSG_RESERVE_FAILED, LOGERROR, L2_SYS_FAIL, \
"MsgReserve Failed", \
{"LineNum","Size","ue_index","LcId", "Count",\
"","",\
FNAME,EVENT, \
}},
{PDCP_GET_MEM_FROM_POOL_FAILED, LOGERROR, L2_SYS_FAIL, \
"getMemFromPool Failed", \
{"LineNum","Size","ue_index","LcId","Count", \
"","",\
FNAME,EVENT, \
}},
{PDCP_MSG_JOIN_FAILED, LOGERROR, L2_SYS_FAIL, \
"MsgJoin Failed", \
{"ue_index","LcId","Size of 1st arg","Size of 2nd arg", "Count", \
"","",\
FNAME,EVENT, \
}},
{PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, \
"Invalid TLV Message Length from OAM/RRC", \
{"LineNum","RecvdLength","ReqdLength","","", \
"","",\
FNAME,EVENT, \
}},
{PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, \
"RRC fails", \
{"LineNum","RetVal","ue_index","","", \
"","",\
FNAME,EVENT, \
}},
{PDCP_OAM_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, \
"OAM fails", \
{"LineNum","RetVal","","","",\
"","",\
FNAME,EVENT, \
}},
{PDCP_PARAMS_TO_INTEGRITY_ENGINE, LOGDEBUG, (PDCP_RX | PDCP_TX | PDCP_RRCOAM), \
"Integrity protection with parameters",
{"LineNum", "LcId","Count", "Direction", "AlgorithmID", \
"", "", \
FNAME,"",\
}},
{PDCP_CALC_MAC_I, LOGDEBUG, (PDCP_TX | PDCP_RRCOAM), \
"Calculated MAC-I", \
{"LineNum","MACI[0]","MACI[1]","MACI[2]","MACI[3]",\
"","",\
FNAME,"",\
}},
{PDCP_RECV_MAC_I, LOGDEBUG, PDCP_RX,\
"Received MAC-I", \
{"LineNum","MACI[0]","MACI[1]","MACI[2]","MACI[3]",\
"","",\
FNAME,"",\
}},
{PDCP_RRC_REQUEST_FOR_UE, LOGDEBUG, PDCP_RRCOAM, \
"PDCP receives request from RRC for UE", \
{"LineNum","ue_index","","", "",\
"","",\
FNAME,"",\
}},
{PDCP_HEXDUMP_OAM_RRC_MSG, LOGDEBUG, PDCP_RRCOAM, \
"Length and Hexdump of messages received by PDCP", \
{"LineNum","RecvdLength","Hexdump","", "",\
"","",\
FNAME,"",\
}},
{PDCP_SRC_ID_INCORRECT, LOGWARNING, PDCP_RRCOAM, \
"OAM-RRC: Incorrect Source module id", \
{"LineNum","SrcModuleId","","","", \
"","",\
FNAME,"",\
}},
{PDCP_DEST_ID_INCORRECT, LOGWARNING, PDCP_RRCOAM, \
"OAM-RRC: Incorrect Destination module id", \
{"LineNum","DestModuleId","","","", \
"","",\
FNAME,"",\
}},
{PDCP_INVALID_OPTIONALPARAM_LEN, LOGWARNING, PDCP_RRCOAM, \
"OAM-RRC: Invalid optional parameters length", \
{"LineNum","TagId", "OptionalParamLen","","", \
"","",\
FNAME,"",\
}},
{PDCP_INVALID_DATA_SIZE, LOGWARNING, PDCP_RX,\
"Invalid data size", \
{"LineNum","DataSize","Count","ue_index", "LcId",\
"","",\
FNAME,EVENT, \
}},
{PDCP_INVALID_UE, LOGWARNING, PDCP_RRCOAM, \
"Invalid UE", \
{"LineNum","ue_index","isPdcpContextInitialized","","", \
"","",\
FNAME,"",\
}},
{PDCP_INVALID_TAG_ID, LOGWARNING, PDCP_RRCOAM, \
"OAM-RRC: Tag ID is not supported", \
{"LineNum","RecvdTagId","","","", \
"","",\
FNAME,"",\
}},
/*MULTI_SECTOR*/
{PDCP_TAG_ID_NOT_CHKED, LOGWARNING, PDCP_RRCOAM, \
"OAM-RRC: Tag ID is not checked", \
{"LineNum","RecvdTagId","","","", \
"","",\
FNAME,"",\
}},
{PDCP_RRC_CELL_INDEX_ERR, LOGWARNING, PDCP_RRCOAM, \
"RRC cellIndex is different from self cellIndex", \
{"LineNum","RRC cell_index","Selfcell_index","","", \
"","",\
FNAME,"",\
}},
{PDCP_CONTEXT_PARAMS, LOGINFO, PDCP_RRCOAM, \
"OAM-RRC: PDCP context parameters->", \
{"LineNum","ue_index","LcId","Direction","rlc_mode",\
"", "",\
FNAME,"",\
}},
{PDCP_INTEGRITY_PARAMS, LOGINFO, PDCP_RRCOAM, \
"OAM-RRC: Integrity parameters->", \
{"LineNum","ue_index","AlgoId","", "",\
"","",\
FNAME,"Key",\
}},
{PDCP_SRB_CIPHERING_PARAMS, LOGINFO, PDCP_RRCOAM, \
"OAM-RRC: SRB ciphering parameters->", \
{"LineNum","ue_index","AlgoId","", "",\
"","",\
FNAME,"Key",\
}},
{PDCP_DRB_CIPHERING_PARAMS, LOGINFO, PDCP_RRCOAM, \
"OAM-RRC: DRB ciphering parameters->", \
{"LineNum","ue_index","AlgoId","","", \
"","",\
FNAME,"Key",\
}},
{PDCP_CONF_DIS_TIMER, LOGINFO, PDCP_RRCOAM, \
"OAM-RRC: Configuring Discard Timer parameters->", \
{"LineNum","ue_index","LcId","Direction","DiscardTimer",\
"","", \
FNAME,"",\
}},
{PDCP_CONF_SN_SIZE, LOGINFO, PDCP_RRCOAM, \
"OAM-RRC: Configuring SN SIZE parameters->", \
{"LineNum","ue_index","LcId","Direction","SnSize",\
"","", \
FNAME,"",\
}},
{PDCP_CONF_STATUS_REPRT, LOGINFO, PDCP_RRCOAM, \
"OAM-RRC: Configuring Status Report parameters->", \
{"LineNum","ue_index","LcId","Direction", "StatusReport",\
"","", \
FNAME,"",\
}},
{PDCP_CONF_ROHC, LOGINFO, PDCP_RRCOAM, \
"OAM-RRC: Configuring ROHC parameters->", \
{"LineNum","ue_index","LcId","ProfileId", "MaxCid",\
"","",\
FNAME,"",\
}},
{PDCP_OAM_API_NS, LOGWARNING, PDCP_RRCOAM, \
"OAM FAIL: API Id not supported->", \
{"LineNum","ApiId","","", "",\
"","",\
FNAME,"",\
}},
{PDCP_RRC_API_NS, LOGWARNING, PDCP_RRCOAM, \
"RRC FAIL: API Id not supported->", \
{"LineNum","ApiId","","","", \
"","",\
FNAME,"",\
}},
{PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, \
"UeId or LcId is not valid ->", \
{"LineNum","LcId","ue_index","", "",\
"","",\
FNAME,"",\
}},
{PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM, \
"Direction is not valid ->", \
{"LineNum","Direction","ue_index","LcId","", \
"","",\
FNAME,"",\
}},
{PDCP_STATE_VARS_RESET_TX, LOGDEBUG, PDCP_RRCOAM, \
"PDCP state variables at TX side", \
{"LineNum", "ue_index","LcId", "SN", "HFN", \
"", "", \
FNAME, "", \
}},
{PDCP_CONTROL_PDU_NOT_HANDLED_PROPERLY, LOGWARNING, PDCP_TX, \
"Failure to Handle Feedback Packet", \
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{PDCP_DISCARD_TIMER_EXPIRE, LOGWARNING, PDCP_TX, \
"Discarding PDCP SDU fro timer expiry", \
{"LineNum","ue_index","LcId","Count","TickAssociated", \
"","",\
FNAME,"",\
}},
{PDCP_NULL_HEADER, LOGFATAL, PDCP_TX, \
"PDCP Header is NULL", \
{"LineNum","ue_index","LcId","Count", "SnSize",\
"","",\
FNAME,"",\
}},
{PDCP_UNKNOWN_ALGO_ID, LOGWARNING, (PDCP_TX|PDCP_RRCOAM), \
"Unknown AlgorithmID used for integrity/ciphering", \
{"LineNum","AlgoId","LcId","ue_index", "",\
"","",\
FNAME,EVENT, \
}},
{PDCP_DATA_TO_INTEGRITY_ENGINE, LOGDEBUG, PDCP_TX, \
"PDCP sending data to integrity engine", \
{"LineNum","ue_index","LcId","Count", "",\
"","",\
FNAME,"",\
}},
{PDCP_DATA_TO_CIPHERING_ENGINE, LOGDEBUG, PDCP_TX, \
"PDCP sending data to ciphering engine->", \
{"LineNum","ue_index","LcId","Count", "",\
"","",\
FNAME,"",\
}},
{PDCP_DATA_TO_ROHC_ENGINE, LOGDEBUG, PDCP_RX,\
"PDCP sending data to ROHC engine", \
{"LineNum","ue_index","LcId","Count", "",\
"","",\
FNAME,"",\
}},
{PDCP_WRONG_SN_SIZE, LOGWARNING, (PDCP_TX|PDCP_RX), \
"Wrong snSize for SRB/DRB", \
{"LineNum","SnSize","ue_index","LcId","Count",\
"","", \
FNAME,EVENT, \
}},
{PDCP_SIGNALING_PDU_FROM_RLC, LOGDEBUG, PDCP_RX,\
"Receiving signaling PDU from RLC->", \
{"LineNum","ue_index","LcId", "", "", \
"", "",\
FNAME,"",\
}},
{PDCP_STATUS_IND_GTPU, LOGINFO, PDCP_TX, \
"Sending status ind from PDCP to GTP-U-> ", \
{"LineNum","ue_index","LcId","Count","DeliveryStatus",\
"", "", \
FNAME,"",\
}},
{PDCP_STATUS_IND_RRC, LOGINFO, PDCP_TX, \
"Sending status ind from PDCP to RRC-> ", \
{"LineNum","ue_index","LcId","SrbServiceRequested","DeliveryStatus",\
"", "", \
FNAME,"",\
}},
{PDCP_INVALID_RLC_MODE, LOGWARNING, PDCP_RRCOAM, \
"PDCP received invalid rlcMode ", \
{"LineNum","rlc_mode","ue_index","LcId","",\
"", "", \
FNAME,"",\
}},
{PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, \
"RRC-OAM: Processing of optional tag fails->", \
{"LineNum","Error","","","",\
"", "", \
FNAME,EVENT, \
}},
{PDCP_INVALID_DISCARD_TIMER_VALUE, LOGWARNING, PDCP_RRCOAM, \
"RRC-OAM: Invalid value of discard timer->", \
{"LineNum","ue_index","LcId","DiscardTimerVal","",\
"", "", \
FNAME,EVENT, \
}},
{PDCP_INVALID_ROHC_PROFILE_VALUE, LOGWARNING, PDCP_RRCOAM, \
"RRC-OAM: Invalid profileId value of RoHC->", \
{"LineNum","ue_index","LcId","ProfileId","",\
"", "", \
FNAME,EVENT, \
}},
{PDCP_INVALID_STATUS_REPORT_VALUE, LOGWARNING, PDCP_RRCOAM, \
"RRC-OAM: Invalid status report reqd value->", \
{"LineNum","ue_index","LcId","StatusReportReqdVal","",\
"", "", \
FNAME,EVENT, \
}},
{PDCP_ROHC_CONTEXT_ALREADY_INITIALIZED, LOGWARNING, PDCP_RRCOAM, \
"ROHC context is already initialised", \
{"ue_index","LcId","Direction","ProfileIdMask","MaxCid", \
"", "", \
FNAME,EVENT, \
}},
{PDCP_INVALID_MODULE_ID, LOGFATAL, PDCP_RRCOAM, \
"Worng Module Id", \
{"LineNum","ModuleId","","","",\
"", "", \
"FileName",FNAME,\
}},
{PDCP_INVALID_CRNTI, LOGWARNING, PDCP_RRCOAM, \
"CRNTI value receive not in range", \
{"LineNum","ue_index","crnti","StartRange","EndRange",\
"", "", \
FNAME,"",\
}},
{PDCP_INVALID_OLD_CRNTI, LOGWARNING, PDCP_RRCOAM, \
"Old CRNTI value do not match with existing", \
{"LineNum","ue_index","OldCRNTIValueRecvd","ExistingCrntivalue","",\
"", "", \
FNAME,"",\
}},
{PDCP_INVALID_UE_CONTEXT, LOGWARNING, ( PDCP_TX | PDCP_RRCOAM ), \
"UE context not initialized", \
{"LineNum","ue_index","isPdcpContextInitialized","","",\
"", "", \
FNAME,"",\
}},
{PDCP_STORAGE_NULL, LOGWARNING, (PDCP_TX|PDCP_RX), \
"Storage Tree is NULL", \
{"LineNum","","","","",\
"", "", \
FNAME,"",\
}}, 
{PDCP_DL_DATA_FWD_FAILS, LOGWARNING, PDCP_TX, \
"DL Data cannot be forwarded", \
{"LineNum","ue_index","LcId","SN","DataSize",\
"", "", \
FNAME,"",\
}}, 
{PDCP_UL_DATA_FWD_FAILS, LOGWARNING, PDCP_RX,\
"UL Data cannot be forwarded", \
{"LineNum","ue_index","LcId","SN","DataSize",\
"", "", \
FNAME,"",\
}},
{PDCP_OUT_WINDOW_DISCARD, LOGWARNING, PDCP_RX,\
"Discarding as packet recvd out of window", \
{"LineNum","ue_index","LcId","Count","", \
"", "", \
FNAME,"",\
}},
{PDCP_MSG_SEG_NEXT_FAILED, LOGWARNING, ( PDCP_TX | PDCP_RX ),\
"MsgSegnext fails", \
{"LineNum","ue_index","LcId","","", \
"", "", \
FNAME,"",\
}},
{PDCP_SDU_NOT_SENT_UPPER_LAYER, LOGWARNING, PDCP_RX,\
"PDCP SDU cannot sent to GTP-U as cavity there in UL", \
{"LineNum", "ue_index","LcId","SN","LastSubmittedSN", \
"", "", \
FNAME, "", \
}},
{PDCP_NULL_NODE_Q, LOGWARNING, (PDCP_TX | PDCP_RX | PDCP_RRCOAM), \
"Node in PDCP Q found to be NULL", \
{"LineNum", "ue_index","LcId", "", "", \
"", "", \
FNAME, "QName", \
}},
{PDCP_NO_UL_BITMAP, LOGINFO, (PDCP_RX | PDCP_RRCOAM), \
"No UL OOO packet received from RLC", \
{"LineNum","ue_index","LcId", "", "", \
"","",\
FNAME,"",\
}},
{PDCP_NO_UL_BITMAP_SENT, LOGINFO, PDCP_RRCOAM, \
"Not sending PDCP UL bitmap", \
{"LineNum","ue_index","LcId", "StatusReportReqd", "", \
"","",\
FNAME,"",\
}},
{PDCP_SDU_FROM_PACKET_RELAY_TO_PDCP, LOGDEBUG, ( PDCP_TX | PDCP_RX ), \
"Enqueueing SDU from Packet Relay to PDCP->", \
{"LineNum","ue_index","LcId","SNRecvd", "",\
"","",\
FNAME,"QName", \
}},
{PDCP_SDU_ENQUEUED_FROM_RRC, LOGINFO, PDCP_RRCOAM, \
"Enqueuing PDCP SDU from RRC->", \
{"LineNum","ue_index","LcId","ServiceReqested", "",\
"","",\
FNAME,"",\
}},
{PDCP_UE_CTX_ALREADY_CREATED, LOGWARNING, PDCP_RRCOAM, \
"PDCP context already created with", \
{"LineNum","ue_index","", "", "", \
"","",\
FNAME,"QName", \
}},
{PDCP_RESUME_REQ, LOGINFO, PDCP_RRCOAM, \
"Resume request recvd for", \
{"LineNum","ue_index","LcId", "Direction", "", \
"","",\
FNAME,"",\
}},
{PDCP_RE_ESTABLISH_REQ, LOGINFO, PDCP_RRCOAM, \
"Re-establish request recvd for", \
{"LineNum","ue_index","LcId", "", "", \
"","",\
FNAME,"",\
}}, 
{PDCP_SN_HFN_STATUS_REQ_RECVD, LOGINFO, PDCP_RRCOAM, \
"PDCP SN-HFN request received at source PDCP", \
{"LineNum","ue_index","SnHfnReqd", "", "", \
"","",\
FNAME,"",\
}},
{PDCP_Q_EMPTY, LOGDEBUG, PDCP_RRCOAM, \
"PDCP Q made empty", \
{"LineNum", "ue_index","LcId", "", "", \
"", "", \
FNAME, "QName", \
}},
{PDCP_CHANGE_CRNTI_REQ_RECVD, LOGINFO, PDCP_RRCOAM, \
"Change CRNTI req recvd", \
{"LineNum", "ue_index","OldCrnti", "NewCrnti", "", \
"", "", \
FNAME, "", \
}}, 
{PDCP_INVALID_EVENT_RCVD, LOGDEBUG, (PDCP_TX|PDCP_RX), \
"Invalid event received", \
{"LineNum", "ue_index","LcId", "State", "Event", \
"", "", \
FNAME, "", \
}},
{PDCP_EVENT_RCVD, LOGDEBUG, ( PDCP_TX | PDCP_RX  ), \
"PDCP event recvd at data path", \
{"LineNum", "ue_index","LcId", "State", "Event", \
"", "", \
FNAME, "", \
}},
{PDCP_RX_EVENT_RCVD, LOGINFO, PDCP_RX, \
"PDCP event recvd at data path", \
{"LineNum", "ue_index","OldState", " Event", " NewState", \
" LcId", "", \
FNAME, " FSM:", \
}},
{PDCP_TX_EVENT_RCVD, LOGINFO,  PDCP_TX, \
"PDCP event recvd at data path", \
{"LineNum", "ue_index","OldState", " Event", " NewState", \
" LcId", "", \
FNAME, " FSM:", \
}},
{PDCP_STATE_VARS_RESET_RX, LOGDEBUG, PDCP_RRCOAM, \
"PDCP state variables at RX side", \
{"ue_index","LcId", "SN", "HFN", "LastSubmittedSn", \
"", "", \
FNAME, "RlcMode", \
}}, 
{PDCP_FRESH_SDU_NOT_PROCESSED, LOGDEBUG, PDCP_TX, \
"Fresh SDU cannot be processed as forwarding queue unempty",\
{"LineNum", "ue_index","", "", "", \
"", "", \
FNAME, "", \
}},
{PDCP_INVALID_STATE, LOGWARNING, PDCP_RRCOAM, \
"Invalid RRC request received at State", \
{"ue_index", "LcId", "SRB1State","SRB2DRBState","Event",\
"", "", \
FNAME,"",\
}},
{PDCP_UE_CANNOT_DELETE, LOGWARNING, PDCP_RRCOAM, \
"UE cannot be deleted as EMI not recvd for LC when in HO state", \
{"LineNum","ue_index","LcId","","",\
"", "", \
FNAME,"",\
}},
{PDCP_SDU_FWDED_TO_PACKET_RELAY, LOGDEBUG, ( PDCP_TX | PDCP_RX ), \
"Forwarding data packets",\
{"LineNum","ue_index","LcId", "SN", "DataSize", \
"", "", \
FNAME, "", \
}}, 
{PDCP_EMI_RECVD, LOGDEBUG, ( PDCP_TX | PDCP_RX ), \
"EMI recvd from packet relay", \
{"LineNum","ue_index","LcId", "", "", \
"", "", \
FNAME, "", \
}},
{PDCP_DELETE_RB_AFTER_EMI, LOGDEBUG, ( PDCP_TX | PDCP_RX ), \
"Delete LC after EMI reception", \
{"LineNum","ue_index","LcId", "", "", \
"", "", \
FNAME, "", \
}},
{PDCP_RECEIVED_MAX_SDU, LOGWARNING, PDCP_TX, \
"PDCP Storage Queue full",\
{"LineNum","ue_index","LcId","PdcpSduCount","CountDropped", \
"","",\
FNAME,"",\
}}, 
{PDCP_RETRANSMIT_PDU_FOR_RLC_Q_FULL, LOGDEBUG, PDCP_TX, \
"RLC Q FULL Indication Received, Retransmitting PDU", \
{"LineNum", "ue_index", "LcId", "Count", "", \
"", "", \
FNAME, "", \
}}, 
{PDCP_LC_DELETE_FAILURE, LOGWARNING, PDCP_RRCOAM, \
"LC Id cannot be deleted", \
{"LineNum", "ue_index", "LcId", "RetVal", "", \
"", "", \
FNAME, "", \
}}, 
{PDCP_SEC_INIT_FAILS, LOGWARNING, PDCP_RRCOAM, \
"Security engine cannot be initialized", \
{"LineNum", "RspCode", "", "", "", \
"", "", \
FNAME,EVENT, \
}},
{PDCP_SEC_CONTEXT_CREATE, LOGDEBUG, PDCP_RRCOAM,\
"Security engine Context created",\
{"LineNum", "LcId", "Direction", "", "",\
"", "",\
FNAME, "",\
}},
{PDCP_SEC_INIT, LOGDEBUG, PDCP_RRCOAM,\
"Security engine initialized",\
{"LineNum", "Working Mode", "", "", "",\
"", "",\
FNAME, "",\
}}, 
{PDCP_SEC_CONTEXT_DELETE, LOGDEBUG, PDCP_RRCOAM,\
"Security engine Context Deleted",\
{"LineNum", "LcId", "Direction", "", "",\
"", "",\
FNAME, "",\
}},
{PDCP_SEC_POLL_JOB_RING, LOGDEBUG, (PDCP_TX|PDCP_RX),\
"Polling of output queue received",\
{"LineNum", "MaxProcessedPktsToBeSent", "", "", "",\
"", "",\
FNAME,EVENT,\
}},
{PDCP_SEC_CALL_CALLBACK_FUNCTION, LOGDEBUG, (PDCP_TX|PDCP_RX),\
"Security calls callback function to send back processed packets",\
{"LineNum", "BearerType", "Direction", "Status", "ErrInfo",\
"", "",\
FNAME, "",\
}},  
{PDCP_INVALID_RB, LOGWARNING, PDCP_RRCOAM,\
"PDCP received invalid RB entity",\
{"LineNum", "BearerId", "ue_index", "", "",\
"","",\
FNAME, "",\
}},
{PDCP_INVALID_QCI, LOGWARNING, PDCP_RRCOAM,\
"PDCP received invalid QCI from RRC",\
{"LineNum", "QCI", "ue_index", "LcId", "",\
"", "",\
FNAME, "",\
}}, 
{PDCP_DISCARD_TIMER_SDU_NOT_DELETED,  LOGWARNING, PDCP_TX,\
"Discard timer expired, but PDCP SDU cannot be deleted",\
{"LineNum", "ue_index", "LcId", "Count", "TickAssociated"
"", "",\
FNAME, "", \
}},
{PDCP_TX_SDU_Q_FULL, LOGWARNING, PDCP_TX,\
"Dropping SDU as PDCP SDU Q full",\
{"LineNum", "ue_index", "LcId", "PdcpSduCount", ""
"", "",\
FNAME, "", \
}},
{ PDCP_MULTIPLE_SPS_SETUP_NOT_VALID, LOGWARNING, PDCP_RRCOAM,\
"PDCP receives SPS setup after setup from L3",\
{"LineNum", "ue_index", "RecvdReqType", "ConfiguredReqType", "",\
"", "",\
FNAME, "", \
}},
{PDCP_UE_SPS_STATE, LOGINFO, PDCP_RX,\
"Current UE level SPS state", \
{"LineNum","ue_index","OldState"," Event"," NewState",\
" LcId","", \
FNAME," FSM:",\
}},
{PDCP_RRM_INVALID_MSG_ID, LOGWARNING, PDCP_RRM_INT,\
"PDCP RRM invalid api id", \
{"LineNum","MsgId","","","",\
"","", \
FNAME,"PDCP_RRM_API_ID",\
}},
{PDCP_RRM_INVALID_MODULE_ID, LOGWARNING, PDCP_RRM_INT,\
"PDCP RRM invalid module id", \
{"LineNum","SrcModuleId","DestMouleId","","",\
"","", \
FNAME,"PDCP_RRM_MSG_PROCESS",\
}},
{PDCP_RRM_MSG_SENT_FAIL, LOGWARNING, PDCP_RRM_INT,\
"PDCP message sent to RRM fail", \
{"LineNum","","","","",\
"","", \
FNAME,"PDCP_RRM_ECN_CN_MSG_SENT_FAIL",\
}},
{PDCP_DRB_ECN_CN_MSG, LOGDEBUG, PDCP_RRM_INT,\
"PDCP DRB ECN config Message", \
{"LineNum","ue_index","NumOfLc","","",\
"","", \
FNAME,"",\
}},
{PDCP_RRM_INVALID_MSG_LENGTH, LOGWARNING, PDCP_RRM_INT,\
"PDCP RRM invalid Msg Length ", \
{"LineNum","","","","",\
"","", \
FNAME,"PDCP_RRM_INVALID_BYTES_READ",\
}},
{PDCP_EXCEED_MAX_UE, LOGWARNING, PDCP_RRM_INT,\
"PDCP RRM exceed Max Ue", \
{"LineNum","ue_index","NumOfLc","","",\
"","", \
FNAME,"PDCP_RRM_DRB_ECN_CE_REQ",\
}},
{PDCP_EXCEED_MAX_LC, LOGWARNING, PDCP_RRM_INT,\
"PDCP RRM Exceed Max Lc", \
{"LineNum","LcId","","","",\
"","", \
FNAME,"PDCP_RRM_DRB_ECN_CE_REQ",\
}},
{PDCP_INVALID_ECN_FEATURE, LOGWARNING, PDCP_RRM_INT,\
"PDCP RRM Invalid ECN Feature", \
{"LineNum","FeatureStatus","EcnTrigger","","",\
"","", \
FNAME,"PDCP_RRM_ECN_CONFIG_REQ",\
}},
{PDCP_INVALID_WATERMARK_VALUE, LOGWARNING, PDCP_RRM_INT,\
"PDCP RRM watermark value is out of range", \
{"LineNum","lowWaterMarkValue","HighWaterMarkValue","","",\
"","", \
FNAME,"PDCP_RRM_ECN_CONFIG_REQ",\
}},
{PDCP_INVALID_CONGESTION_DIRECTION, LOGWARNING, PDCP_RRM_INT,\
"PDCP RRM Congestion Direction", \
{"LineNum","EcnDirection","congestionStatus","","",\
"","", \
FNAME,"PDCP_RRM_DRB_ECN_CE_REQ",\
}},
{PDCP_SPS_INTERVAL_PROFILE_IS_NULL, LOGWARNING, (PDCP_RX|PDCP_RRCOAM),\
"SPS Interval Profile cannot be found at PDCP", \
{"LineNum","ue_index","ulSpsInterval","","",\
"","", \
FNAME,"",\
}},
/* SPR 8196 fix start*/
{PDCP_OAM_INIT_IND,LOGDEBUG,PDCP_RRCOAM,\
"PDCP send OAM init indication",\
{"ApiId","SrcModuleId","DestModuleId","","",\
"","",\
FNAME,EVENT,\
}},
/* SPR 8196 fix end*/
{PDCP_CANNOT_ADD_UL_BITMAP, LOGWARNING, (PDCP_RX|PDCP_RRCOAM),\
"PDCP cannot add packet to UL bitmap", \
{"LineNum","ue_index","LcId","lastSubmittedCount","PacketCount",\
"","", \
FNAME,"",\
}},
{PDCP_INTRA_HO_FWD_FAIL, LOGWARNING, PDCP_TX,\
"PDCP cannot handle Intra-HO forwarding request", \
{"LineNum","oldUeId","NewUeId","","",\
"","", \
FNAME,"",\
}},
{PDCP_RECV_PSR, LOGINFO, PDCP_TX,\
"PDCP receives PSR", \
{"LineNum","ue_index","lcId","ueState","Size",\
"","", \
FNAME,"",\
}},
{PDCP_PRINT_VERSION_NUM, LOGFATAL, PDCP_TX,\
"version:",\
{"LineNum", "", "", "", "",\
"", "",\
FNAME, EVENT, \
}},
{PDCP_TRAFFIC_NOT_ALLOWED, LOGWARNING, PDCP_TX,\
"Dropping SDU as PDCP traffic not alloed on Lc",\
{"LineNum", "ue_index", "LcId", "PdcpSduCount", ""
"", "",\
FNAME, "", \
}},
/*RLC LOGS*/
{LTE_RLC_SEND_DEL_IND_FAILED, LOGERROR, (RLC_TX|RLC_AM),\
"Delivery Indication to PDCP failed",\
{"LineNum","ue_index","LcId","vtA","vtMS",\
"","",\
FNAME,"",\
}},
{LTE_RLC_RX_AM_SDU,LOGINFO,RLC_TX,\
"LTE_RLC_RX_AM_SDU",\
{"ue_index","LcId", "lcQLoad", "lcQLoadretxnPduSize",\
"lcQLoadctrlQLoad","ReTxQSize","SDU Size",\
FNAME,"",\
}},
{LTE_RLC_RX_AM_SDU_ERR,LOGDEBUG,RLC_TX,\
"LTE_RLC_RX_AM_SDU_ERR",\
{"ue_index","LcId", "TxQSize", "RlcQSize",\
"","","",\
FNAME,"",\
}},
{LTE_RLC_NULL_TID_NODE_IN_DEL_IND, LOGERROR, (RLC_TX|RLC_AM),\
"NULL Tid node in ReTx pdu during Del Ind",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_CASE_IN_DEL_IND, LOGERROR, (RLC_TX|RLC_AM),\
"Mismatch in the Tid node",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_SN_IN_DEL_IND, LOGWARNING, (RLC_TX|RLC_AM),\
"Invalid index in ReTx buffer during Del Ind",\
{"LineNum","ue_index","LcId","SN","Index",\
"","",\
FNAME,"",\
}},
{LTE_RLC_FREE_SDU_TID_FAILED, LOGERROR, (RLC_TX|RLC_AM),\
"Unable to free TidInfo from ReTx buffer",\
{"LineNum","ue_index","LcId","SN","RetVal",\
"","",\
FNAME,"",\
}},
{LTE_RLC_NULL_RETX_PDU_IN_DEL_IND, LOGWARNING, (RLC_TX|RLC_AM),\
"NULL PDU in ReTx buffer ",\
{"LineNum","ue_index","LcId","SN","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_NO_TID_PRESENT_FOR_DEL_IND, LOGERROR, (RLC_TX|RLC_AM),\
"No Tid present in TidQ for Del Ind",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_FREE_RETX_BUFFER_FAILED, LOGERROR, (RLC_TX|RLC_AM),\
"Unable to free ReTx buffer",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_SEND_DEL_IND, LOGDEBUG, (RLC_TX|RLC_AM),\
"Sending Del Ind to PDCP",\
{"LineNum","ue_index","LcId","No of TiDs","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_STATE_VARS_ON_DEL_IND, LOGDEBUG, (RLC_TX|RLC_AM),\
"State Variables after sending Del Ind",\
{"ue_index","LcId","vtA","vtS","vtMS",\
"","",\
FNAME,"",\
}},
{LTE_RLC_NON_ZERO_DC_CPT_IN_CONTROL_PDU, LOGERROR, (RLC_TX|RLC_AM),\
"D/C & CPT fields should be Zero in status pdu",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_OUT_OF_RANGE_ACK_RECVD_1, LOGERROR, (RLC_TX|RLC_AM),\
"Received ACK is out of range: 1",\
{"LineNum","ue_index","LcId","ACK received","RetVal",\
"vtA","vtS",\
FNAME,"",\
}},
{LTE_RLC_OUT_OF_RANGE_ACK_RECVD_2, LOGERROR, (RLC_TX|RLC_AM),\
"Received ACK is out of range: 2",\
{"LineNum","ue_index","LcId","vtA","vtS",\
"","",\
FNAME,"",\
}},
{LTE_RLC_ACK_OUT_OF_WINDOW_1, LOGERROR, (RLC_TX|RLC_AM),\
"Received ACK is out of window: 1",\
{"LineNum","ue_index","LcId","ACK received","RetVal",\
"vtA","vtMS",\
FNAME,"",\
}},
{LTE_RLC_ACK_OUT_OF_WINDOW_2, LOGERROR, (RLC_TX|RLC_AM),\
"Received ACK is out of window : 2",\
{"LineNum","ue_index","LcId","vtA","vtMS",\
"","",\
FNAME,"",\
}},
{LTE_RLC_EMPTY_NACK_Q_IN_ENCODING, LOGERROR, (RLC_RX|RLC_AM),\
"NackQ is empty: Atleast ACK should be present",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_STATUS_PDU_SIZE_CALCULATION, LOGDEBUG, (RLC_RX|RLC_AM),\
"Calculated Status PDU size",\
{"LineNum","ue_index","LcId","PduSize","Opp recvd",\
"","",\
FNAME,"",\
}},
{LTE_RLC_NULL_RETX_PDU_POINTER, LOGERROR, (RLC_TX|RLC_AM),\
"NULL PDU in ReTx buffer",\
{"LineNum","ue_index","LcId","SN","RetVal",\
"","",\
FNAME,"",\
}},
{LTE_RLC_QLOAD_CALCULATION, LOGDEBUG, (RLC_TX|RLC_AM),\
"QLoad for the entity",\
{"ue_index","LcId","CtrlQLoad","ReTxQLoad","FreshQLoad",\
"","",\
FNAME,"",\
}},
/*******************************************************************************************/
/*need to remove existing logIds later*/
/*******************************************************************************************/

/******************************lteRlcRrcInterface.c***********************/

{ LTE_RLC_REPORT_ERROR_TO_RRC, LOGWARNING, (RLC_RRC),\
"REPORT_ERROR_TO_RRC",\
{"LineNum","ApiId","MsgLen","RetVal","","",\
""\
FNAME,"",\
}},

{ LTE_RLC_HANDLE_SOCKET_MSG_ERR, LOGFATAL, (RLC_RRC),\
"Failed :- allocated mem pool for RX msg from RRC & OAM",\
{"LineNum","RetVal","","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_HANDLE_SOCKET_MSG_ERR_1, LOGWARNING, (RLC_RRC),\
" Read from socket err",\
{"LineNum","RetVal","","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_HANDLE_SOCKET_MSG_ERR_2, LOGWARNING, (RLC_RRC),\
" bytesRead < 10",\
{"LineNum","bytesRead","RetVal","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_HANDLE_RECV_SOCKET_MSG_ERR, LOGWARNING, (RLC_RRC),\
"Invalid DestModule Id != 6 ",\
{"LineNum","SrcModId","DestModId","ApiId","RetVal",\
"","",\
FNAME,"",\
}},
{ LTE_RLC_HANDLE_RECV_SOCKET_MSG_ERR_1, LOGWARNING, (RLC_RRC),\
"Rlc Layer not initialized(Drop RRC Msg) ",\
{"LineNum","SrcModId","DestModId","ApiId","RetVal",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_HANDLE_RECV_SOCKET_MSG_ERR_2, LOGWARNING, (RLC_RRC),\
"Invalid SrcModule Id != 3 or 1",\
{"LineNum","SrcModId","DestModId","ApiId","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_HANDLE_RLC_DELETE_ENTITY, LOGWARNING, RLC_RRC,\
"DELETE_ENTITY: Invalid UE Index",\
{"LineNum","ue_index","Trans_Id", "RetVal","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DOWNLINK_COMM_CH_EXIST, LOGFATAL, RLC_RRC,\
"DL Common CH entity already exists",\
{"LineNum","CommonChType","EntityType", "RetVal","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_UPLINK_COMM_CH_EXIST, LOGFATAL, RLC_RRC,\
"UL Common CH entity already exists",\
{"LineNum","CommonChType","EntityType","RetVal","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_CREATE_UE_CTX_ERR, LOGWARNING, RLC_RRC,\
"CREATE_UE_CTX UE Ctx does not exist",\
{"LineNum","ue_index","Rnti","LcId","RetVal", \
"","",\
FNAME,"",\
}},

{LTE_RLC_CREATE_UE_CTX_ERR_1, LOGWARNING, RLC_RRC,\
"LCID already exist",\
{"LineNum","ue_index","Rnti","LcId","RetVal", \
"","",\
FNAME,"",\
}},

/*need correction*/
{LTE_RLC_CREATE_UE_CTX_ERR_2, LOGWARNING, RLC_RRC,\
"UL Common Channel entity already exists",\
{"LineNum","ue_index","Rnti","LcId","EMode", \
"","",\
FNAME,"",\
}},

{LTE_RLC_CREATE_UE_CTX_ERR_3, LOGWARNING, RLC_RRC,\
"Uplink Common Channel entity is already exists",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_CREATE_UE_CTX_ERR_4, LOGFATAL, RLC_RRC,\
"Mem allocation failed",\
{"LineNum","ue_index","","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_INVALID_ENTITY, LOGWARNING, RLC_RRC,\
"RLC_INVALID_ENTITY[NULL]",\
{"LineNum","LcId","RetVal","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_INVALID_ENTITY_1, LOGWARNING, RLC_RRC,\
"RRC RLC Entity does not exist",\
{"LineNum","ue_index","LcId","RetVal","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_INVALID_ENTITY_2, LOGWARNING, RLC_RRC,\
"INVALID_ENTITY RLC Entity does not exist",\
{"LineNum","ue_index","LcId","RetVal","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_INVALID_UECONTEXT, LOGWARNING, RLC_RRC,\
"INVALID_UECTX UeContext does not exists",\
{"LineNum","ue_index","LcId","RetVal","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_INVALID_UECONTEXT_1, LOGWARNING, RLC_RRC,\
"INVLD_UECTX UeContext does not exists",\
{"LineNum","ue_index","LcId","RetVal","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_INVALID_UECONTEXT_2, LOGWARNING, RLC_RRC,\
"INVALID_RRC_UECTX UeContext does not exists",\
{"LineNum","ue_index","LcId","RetVal","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_RECONFIG_UE_ENTITY_1, LOGINFO, RLC_RRC,\
"RLC Entity reconfigured,new param values 1: ",\
{"ue_index","LcId","t_reordering","tStatusProhibit","tPollRetransmit", \
"","",\
FNAME,"",\
}},

{LTE_RLC_RECONFIG_UE_ENTITY_2, LOGINFO, RLC_RRC,\
"RLC Entity reconfigured,new param values 2: ",\
{"ue_index","LcId","PollPdu","PollByte","MaxReTxThreshold", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ENTITY_MARKED_IN_PENDING_DEL_QUEUE, LOGWARNING, RLC_RRC,\
"Entity already marked as pending deletion ",\
{"LineNum","RetVal","","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ENTITY_IN_PENDING_DEL_QUEUE, LOGWARNING, RLC_RRC,\
"common CH is already in pending deletion queue",\
{"LineNum","ComChType","RetVal","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_REESTAB_UM_TX_ENTITY, LOGINFO, (RLC_RRC),\
"UM-TX Re-Esatablishment:",\
{"LineNum","ue_index","LcId","TxQSize","US", \
"","",\
FNAME,"",\
}},
/*MULTI_SECTOR changes starts*/
{LTE_RLC_PARSE_RRC_CELL_INDEX_ERR, LOGWARNING, (RLC_RRC),\
"RRC API cellIndex is different from selfCellIndex",\
{"LineNum","Trans_Id","RRC cell_index","Selfcell_index","", \
"","",\
FNAME,"",\
}},
/*MULTI_SECTOR changes ends*/

/** RLC Rel 3.0 Changes Start **/
{LTE_RLC_REESTAB_UM_RX_ENTITY, LOGINFO, (RLC_RRC),\
"UM-RX entity Re-Establishment:",\
{"ue_index","LcId","SDUSize","UR","UH", \
"","",\
FNAME,"",\
}},

/** RLC Rel 3.0 Changes Start **/
{LTE_RLC_REESTAB_UM_TXRX_ENTITY, LOGINFO, (RLC_RRC),\
"UM-TX RX entity Re-Establishment:",\
{"ue_index","LcId","SDUSize","UR","UH", \
"","",\
FNAME,"",\
}},
/* SPR 3894 change start */
/* Change for SPR 2203 Start */
{LTE_RLC_DELIVERY_STATUS_IND, LOGINFO, RLC_AM,\
"Sending RLC delivery Indication to PDCP",\
{"LineNum","ue_index","LcId","StatusBufferCount","", \
"","",\
FNAME,"",\
}},

/* Change for SPR 2203 Start */
{LTE_RLC_DELIVERY_STATUS_IND_1,LOGDEBUG, RLC_AM,\
"Sending RLC delivery Indication to PDCP_1",\
{"LineNum","ue_index","LcId","StatusBufferCount","", \
"","",\
FNAME,"",\
}},
/* SPR 3894 change End */
/** RLC Rel 3.0 Changes Start **/
{LTE_RLC_REESTAB_AM_RX_ENTITY_1,LOGINFO, RLC_RRC,\
"AM-RX Re-Establishment_1:",\
{"ue_index","LcId","vrR","vrH","vrMR", \
"","",\
FNAME,"", \
}},

{LTE_RLC_ADD_DFE_TO_AM_PDU, LOGWARNING, (RLC_TX|RLC_AM),\
"Available size less than 2bytes",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ADD_DFE_TO_AM_PDU_1,LOGWARNING, (RLC_TX|RLC_AM),\
"MsgAlloc Failed",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
/* + SPR 16766 */
{LTE_RLC_ADD_DFE_TO_AM_PDU_2,LOGWARNING, (RLC_TX|RLC_AM|MAC_L1_INF),\
"MsgSplit failed.",\
{"LineNum","ue_index","Rnti","","", \
"","",\
FNAME,"",\
}},
/* - SPR 16766 */

{LTE_RLC_ENCODE_AMD_PDU_HEADER, LOGERROR, (RLC_TX|RLC_AM),\
"No. of Dfe = 0",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},


/*****************lteRlcOamInterface.c*************/

{ LTE_RLC_OAM_INTERFACE_CLEANUP_ERR, LOGWARNING, (RLC_OAM),\
"Rlc not Initilaized-Cleanup Invalid ",\
{"LineNum","SentBufId","MsgLen","RetVal","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_OAM_INTERFACE_CLEANUP_ERR_1, LOGWARNING, (RLC_OAM),\
"Invalid RlcCleanup Req Len != 10",\
{"LineNum","SentBufId","MsgLen","RetVal","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_OAM_GET_STATS_ERR, LOGWARNING, (RLC_OAM),\
" Invalid Rlc GetStats ReqLen != 4",\
{"LineNum","Trans_Id","MsgLen","RetVal","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_OAM_GET_STATS_ERR_1, LOGFATAL, (RLC_OAM),\
" RlcGetStats RespBuff MemAlloc failed",\
{"LineNum","Trans_Id","MsgLen","RespLen","RetVal",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_OAM_GET_STATS_ERR_2, LOGWARNING, (RLC_OAM),\
"GetStats Req Err-RLC not Initialized",\
{"LineNum","Trans_Id","MsgLen","RespLen","RetVal",\
"","",\
FNAME,"",\
}},

{ FILL_UM_STATUS_ERR, LOGWARNING, (RLC_OAM),\
"Not an UM Tx Entity",\
{"LineNum","ue_index","UeIdFlag","LcId","",\
"","",\
FNAME,"",\
}},

{ FILL_UM_STATUS_ERR_1, LOGWARNING, (RLC_OAM),\
"Not an UM Rx Entity",\
{"LineNum","ue_index","UeIdFlag","LcId","",\
"","",\
FNAME,"",\
}},

{ FILL_AM_STATUS_ERR, LOGWARNING, (RLC_OAM),\
"Not an AM Tx Entity",\
{"LineNum","ue_index","UeIdFlag","LcId","",\
"","",\
FNAME,"",\
}},

{ FILL_AM_STATUS_ERR_1, LOGWARNING, (RLC_OAM),\
"Not an AM Rx Entity",\
{"LineNum","ue_index","UeIdFlag","LcId","",\
"","",\
FNAME,"",\
}},

{ FILL_UE_STATUS_ERR, LOGWARNING, (RLC_OAM),\
"FillUeStatus Invalid mode",\
{"LineNum","ue_index","UeIdFlag","Mode","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_OAM_STATUS_ERR, LOGWARNING, (RLC_OAM),\
" Validation failed for UE index",\
{"LineNum","Trans_Id","UeIdStart","RetVal","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_OAM_STATUS_ERR_1, LOGFATAL, (RLC_OAM),\
"GetStatus RespBuff mem alloc failed ",\
{"LineNum","Trans_Id","UeIdStart","RespLen","RetVal",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_OAM_STATUS_ERR_2, LOGWARNING, (RLC_OAM),\
"GetStatus Req Err-RLC not Initialized ",\
{"LineNum","Trans_Id","UeIdStart","RespLen","RetVal",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_OAM_STATUS_ERR_3, LOGWARNING, (RLC_OAM),\
"Invalid GetStatus Req Len < 4 ",\
{"LineNum","Trans_Id","UeIdStart","MsgLen","RetVal",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_CONFIG_INIT_ERR, LOGWARNING, (RLC_OAM),\
"Invalid RntiRange in RlcOamInitReq",\
{"LineNum","RntiStartRange","rntiEndRange","RetVal","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_CONFIG_INIT_ERR_1, LOGWARNING, (RLC_OAM),\
"Invalid numOfUEs in RlcOamInitReq ",\
{"LineNum","NumOfUe","RetVal","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_CONFIG_INIT_ERR_2, LOGWARNING, (RLC_OAM),\
"Init RLCThreads and Intfce Failed ",\
{"LineNum","NumOfUe","RntiStartRange","rntiEndRange","RetVal",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_HANDLE_RLC_OAM_INIT_LAYER_ERR, LOGWARNING, (RLC_OAM),\
"Rlc Layer already initialized ",\
{"LineNum","RetVal","","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_HANDLE_RLC_OAM_INIT_LAYER_ERR_1, LOGWARNING, (RLC_OAM),\
"Invalid parameters in RLC Init req ",\
{"LineNum","RetVal","","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_HANDLE_RLC_OAM_INIT_LAYER_ERR_2, LOGWARNING, (RLC_OAM),\
"Invalid Rlc Oam Init Req msglen ",\
{"LineNum","RetVal","","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_OAM_RESET_STATS_ERR, LOGWARNING, (RLC_OAM),\
"Stats reset err-RLC not initialized ",\
{"LineNum","SendInd","MsgLen","RetVal","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_OAM_RESET_STATS_ERR_1, LOGWARNING, (RLC_OAM),\
"Invalid Stats Reset msgLen != 10",\
{"LineNum","SendInd","MsgLen","RetVal","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_HANDLE_OAM_REQ_ERR, LOGFATAL, (RLC_OAM),\
"Response buffer Mem Alloc failed ",\
{"LineNum","Trans_Id","ApiId","MsgLen","RetVal",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_HANDLE_OAM_REQ_ERR_1, LOGWARNING, (RLC_OAM),\
"Invalid OAM req ApiId ",\
{"LineNum","Trans_Id","ApiId","MsgLen","RetVal",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_HANDLE_OAM_REQ, LOGINFO, (RLC_OAM),\
"OAM req msg recvd",\
{"LineNum","Trans_Id","ApiId","MsgLen","RetVal",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_BUILD_CONFIGURATION_ERR, LOGWARNING, (RLC_OAM),\
"RlcbuildStamp file does not exist",\
{"LineNum","RetVal","","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_BUILD_CONFIGURATION_ERR_1, LOGWARNING, (RLC_OAM),\
" Incorrect Rlc-Oam RlcbuildStamp file",\
{"LineNum","RetVal","","","",\
"","",\
FNAME,"",\
}},

/**************"lteRlcMacInterface.c"***************************/

{LTE_RLC_INVALID_UE_INDEX_OR_LC, LOGWARNING, RLC_MAC,\
"Invalid Ue index or NumberOfLC or RLC Layer not init",\
{"LineNum","ue_index","NumOfLC","RlcLayerInitialized_g","", \
"","",\
FNAME,"",\
}},

/********lteRlcRrcParseUtil.c *********/

{ LTE_RLC_PARSE_RRC_MSG_ERR, LOGWARNING, (RLC_RRC),\
"Not able to parseRRC Msg (bytesRead != msgLen)",\
{"LineNum","Trans_Id","BytesRead","MsgLen","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RRC_MSG_ERR_1, LOGWARNING, (RLC_RRC),\
"Invalid messageId received from RRC",\
{"LineNum","Trans_Id","BytesRead","MsgLen","MsgId",\
"","",\
FNAME,"",\
}},

{LTE_RLC_SEND_COMM_CH_DATA_FAILED, LOGWARNING, RLC_RRC,\
"Fail to send the common channel data to MAC",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_COMM_CH_RECONFIG_FAILED, LOGWARNING, RLC_RRC,\
"Fail to reconfig Common Channel",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_PARSE_UE_IDX_ERR, LOGWARNING, RLC_RRC,\
"Failed Parse UE Idx (msgLen < 2)",\
{"LineNum","MsgLen","","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_PARSE_UE_IDX_ERR_1, LOGWARNING, RLC_RRC,\
"Invalid UeIndx > MAX_UE_ID(300)",\
{"LineNum","ue_index","MsgLen","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_PARSE_UE_IDX_ERR_2, LOGWARNING, RLC_RRC,\
"Ue Id does not exist to Reconfig ",\
{"LineNum","ue_index","","","", \
"","",\
FNAME,"",\
}},


{LTE_RLC_INVALID_RNTI, LOGWARNING, RLC_RRC,\
"Invalid RNTI for Broadcast Channel data",\
{"LineNum","Rnti","","","", \
"","",\
FNAME,EVENT, \
}},

{LTE_RLC_INVALID_RNTI_1, LOGWARNING, RLC_RRC,\
"Invalid RNTI for Paging Channel data",\
{"LineNum","Rnti","","","", \
"","",\
FNAME,EVENT, \
}},

{LTE_RLC_INVALID_RNTI_2, LOGWARNING, RLC_RRC,\
"Invalid RNTI for common Channel data",\
{"LineNum","Rnti","","","", \
"","",\
FNAME,EVENT, \
}},

{ LTE_RLC_PARSE_CREATE_MSG_ERR, LOGWARNING, (RLC_RRC),\
"Invalid meglength received < 4 ",\
{"LineNum","MsgLen","","","",
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_CREATE_MSG_ERR_1, LOGWARNING, (RLC_RRC),\
"Invalid UeId received in UE Create request",\
{"LineNum","MsgLen","ue_index","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_CREATE_MSG_ERR_2, LOGWARNING, (RLC_RRC),\
"UE Ctx already exist",\
{"LineNum","MsgLen","ue_index","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_CREATE_MSG_ERR_3, LOGWARNING, (RLC_RRC),\
"Invalid Rnti received in UE create request",\
{"LineNum","Rnti","StartRnti","EndRnti","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_CREATE_MSG_ERR_4, LOGWARNING, (RLC_RRC),\
"Invalid lcCount received in in UE create request ",\
{"LineNum","Rnti","StartRnti","EndRnti","lcCount",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_RRC_HANDLE_CREATE_UE_ENTITY_REQ_ERR, LOGWARNING, (RLC_RRC),\
"Max active UE reached ",\
{"LineNum","ApiId","ActiveUEs","SupportedUEs","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_RRC_HANDLE_CREATE_UE_ENTITY_REQ, LOGINFO, (RLC_RRC),\
"Create UE entity request received from RRC ",\
{"LineNum","ApiId", "ActiveUEs","RspCode","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_RLC_DELETE_ENTITY, LOGINFO, RLC_RRC,\
"Req for Deleting RLC Entityo",\
{"LineNum","ue_index","RspCode","MsgLen","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_RLC_RECONFIG_ERR, LOGWARNING, RLC_RRC,\
"Fail to reconfig - parsing failed ",\
{"LineNum","ue_index","MsgLen","RetVal","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_RLC_RECONFIG_ERR_1, LOGWARNING, RLC_RRC,\
"Reconfig Partial Success",\
{"LineNum","ue_index","MsgLen","NoOfFailures","RetVal", \
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_RLC_RE_ESTABLISH_ERR, LOGWARNING, RLC_RRC,\
"RE_ESTABLISHMENT Parsing Error",\
{"LineNum","ue_index","RetVal","MsgLen","NoOfFailures", \
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_RLC_RE_ESTABLISH_ERR_1, LOGWARNING, RLC_RRC,\
"Parsing Error in RE_ESTABLISHMENT",\
{"LineNum","ue_index","RetVal","MsgLen","", \
"","",\
FNAME,"",\
}},

{ LTE_RLC_RRC_HANDLE_UE_ENTITY_REQ_ERR, LOGWARNING, (RLC_RRC),\
"Invalid UE entity request received from RRC",\
{"LineNum","MsgId","","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_RRC_HANDLE_UE_ENTITY_REQ, LOGINFO, (RLC_RRC),\
"UE entity request received from RRC ",\
{"LineNum","MsgId","RetVal","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_SEND_CNF_MSG_TO_RRC, LOGINFO, (RLC_RRC),\
"Send cnf msg to RRC ",\
{"LineNum","MsgId","opCode","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_COMM_CH_CREATION_FAILED, LOGWARNING, RLC_RRC,\
"Invalid common channel entity for creation",\
{"LineNum","SubTag","","","", \
"","",\
FNAME,"",\
}},

{LOG_RLC_COMM_CH_INVALID_OPERATION, LOGWARNING, RLC_RRC,\
"Invalid operation for common channel",\
{"LineNum","Tag","","","", \
"","",\
FNAME,"",\
}},

{ LTE_RLC_CREATE_TX_UM_RLC_ERR, LOGWARNING, (RLC_RRC),\
"Invalid TagLen != 7 in create TX UM request",\
{"LineNum","TagLen","","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_CREATE_TX_UM_RLC_ERR_1, LOGWARNING, (RLC_RRC),\
"CREATE_TX_UM Invalid LcID >= 11",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_CREATE_TX_UM_RLC_ERR_2, LOGWARNING, (RLC_RRC),\
"Invalid snFieldLen ! 5 or !10 in create TX UM request",\
{"LineNum","TagLen","LcId","SnFieldLen","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_CREATE_TX_UM_RLC_ENTITY, LOGINFO, (RLC_RRC),\
"Create TX UM RLC entity request received from RRC ",\
{"LineNum","LcId","SnFieldLen","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_RX_UM_RLC_ERR, LOGWARNING, (RLC_RRC),\
"Invalid Tag Len in create RX UM request ",\
{"LineNum","TagLen","","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_RX_UM_RLC_ERR_1, LOGWARNING, (RLC_RRC),\
"Invalid LcID in create RX UM request ",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_RX_UM_RLC_ERR_2, LOGWARNING, (RLC_RRC),\
"Invalid snFieldLen ! 5 or !10 in create RX UM request ",\
{"LineNum","TagLen","LcId","SnFieldLen","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_RX_UM_RLC_ERR_3, LOGWARNING, (RLC_RRC),\
"Invalid tReordering in create RX UM request ",\
{"LineNum","TagLen","LcId","SnFieldLen","t_reordering",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_RX_UM_RLC_ENTITY, LOGINFO, (RLC_RRC),\
"Create RX UM  entity request received from RRC ",\
{"LineNum","LcId","SnFieldLen","tReordering","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_TX_RX_UM_ERR, LOGWARNING, (RLC_RRC),\
"Invalid Tag Len != 11",\
{"LineNum","TagLen","","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_TX_RX_UM_ERR_1, LOGWARNING, (RLC_RRC),\
"CREATE_TX_RX_UM Invalid LcId >= 11 ",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_TX_RX_UM_ERR_2, LOGWARNING, (RLC_RRC),\
"Invalid snFieldLen ! 5 or !10 in TX RX UM request ",\
{"LineNum","TagLen","LcId","SnFieldLen_tx","SnFieldLen_rx",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_TX_RX_UM_ERR_3, LOGWARNING, (RLC_RRC),\
"Invalid tReordering( 0 < or > 300 ) ",\
{"LineNum","TagLen","LcId","t_reordering","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_TX_RX_UM_ENTITY, LOGINFO, (RLC_RRC),\
"Create TX RX UM  entity request received from RRC ",\
{"LineNum","LcId","SnFieldLen_tx","SnFieldLen_rx","tReordering",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change start */
{ LTE_RLC_CREATE_RX_TX_ENTITY, LOGINFO, (RLC_RRC),\
"Create TX RX AM  entity request received from RRC ",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},
{ LTE_RLC_CREATE_RX_TX_ENTITY_1, LOGINFO, (RLC_RRC),\
"Create TX RX AM  entity req received from RRC ",\
{"LineNum","LcId","t_poll_retransmit","poll_pdu","poll_byte",\
"","",\
FNAME,"",\
}},/*682*/
/* SPR 3894 change End */
//not in code
{ LTE_RLC_CREATE_RX_TX_ENTITY_2, LOGINFO, (RLC_RRC),\
"Create TX_RX_AM  entity request received from RRC ",\
{"LineNum","LcId","t_status_prohibit","max_retx_threshold","t_reordering",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_CREATE_RX_TX_ERR, LOGWARNING, (RLC_RRC),\
"Invalid tagLen != 17",\
{"LineNum","TagLen","","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_RX_TX_ERR_1, LOGWARNING, (RLC_RRC),\
"CREATE_RX_TX Invalid lcId >= 11 ",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_RX_TX_ERR_2, LOGWARNING, (RLC_RRC),\
"Invalid tPollRetransmit < 5 or > 300 ",\
{"LineNum","TagLen","LcId","t_poll_retransmit","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_RX_TX_ERR_3, LOGWARNING, (RLC_RRC),\
"Invalid pollPDU < 4 or > 385",\
{"LineNum","TagLen","LcId","t_poll_retransmit","poll_pdu",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_RX_TX_ERR_4, LOGWARNING, (RLC_RRC),\
"Invalid pollByte < 25 or > 3001 or not a valid no. (36.331)",\
{"LineNum","TagLen","LcId","poll_byte","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_RX_TX_ERR_5, LOGWARNING, (RLC_RRC),\
"Invalid maxRetxThreshold < 1 or > 32 or not a valid no.(36.331)",\
{"LineNum","TagLen","LcId","max_retx_threshold","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_CREATE_RX_TX_ERR_6, LOGWARNING, (RLC_RRC),\
" Invalid tReordering < 0 or > 200 or not a valid no.(36.331)",\
{"LineNum","LcId","t_poll_retransmit","t_reordering","",\
"","",\
FNAME,"",\
}},/*690*/


{ LTE_RLC_CREATE_RX_TX_ERR_7, LOGWARNING, (RLC_RRC),\
"Invalid tStatusProhibit < 0 or > 500 or not a valid no.(36.331) ",\
{"LineNum","LcId","t_poll_retransmit","t_reordering","t_status_prohibit",\
"","",\
FNAME,"",\
}},



{ LTE_RLC_PARSE_DEL_TX_UM_ERR, LOGWARNING, (RLC_RRC),\
"PARSE_DEL_TX_UM Invalid TagLen != 5 ",\
{"LineNum","TagLen","","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_DEL_TX_UM_ERR_1, LOGWARNING, (RLC_RRC),\
"PARSE_DEL_TX_UM Invalid lcId >= 11",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_DEL_TX_UM_ENTITY, LOGINFO, (RLC_RRC),\
"Deleting Tx UM Rlc Entity",\
{"LineNum","LcId","","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_DEL_RX_UM_ERR, LOGWARNING, (RLC_RRC),\
"PARSE_DEL_RX_UM Invalid TagLen != 5",\
{"LineNum","TagLen","","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_DEL_RX_UM_ERR_1, LOGWARNING, (RLC_RRC),\
"PARSE_DEL_RX_UM Invalid lcId >= 11",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_DEL_RX_UM_ENTITY, LOGINFO, (RLC_RRC),\
"Deleting Rx UM Rlc Entity",\
{"LineNum","LcId","","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_DEL_TX_RX_UM_ERR, LOGWARNING, (RLC_RRC),\
"DEL_TX_RX_UM Invalid TagLen != 5",\
{"LineNum","TagLen","","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_DEL_TX_RX_UM_ERR_1, LOGWARNING, (RLC_RRC),\
"Nvalid lcId >= 11",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_DEL_TX_RX_UM_ENTITY, LOGINFO, (RLC_RRC),\
"Deleting TxRX UM Rlc Entity",\
{"LineNum","LcId","","","",\
"","",\
FNAME,"",\
}},/*700*/


{ LTE_RLC_PARSE_DEL_TX_RX_AM_ERR, LOGWARNING, (RLC_RRC),\
"DEL_TX_RX_AM Invalid TagLen != 5",\
{"LineNum","TagLen","","","",\
" ",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_DEL_TX_RX_AM_ERR_1, LOGWARNING, (RLC_RRC),\
"PARSE_DEL_TX_RX_AM Invalid lcId >= 11",\
{"LineNum","TagLen","LcId","","",\
" ",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_DEL_TX_RX_AM_ENTITY, LOGINFO, (RLC_RRC),\
"Deleting AM Rlc Entity",\
{"LineNum","LcId","","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_RECONFIG_TX_UM_ERR, LOGWARNING, (RLC_OAM),\
"RECONFIG_TX_UM Invalid TagLen != 5",\
{"LineNum","TagLen","","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_RECONFIG_TX_UM_ERR_1, LOGWARNING, (RLC_OAM),\
"PARSE_RECONFIG_TX_UM Invalid lcId >= 11 ",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},

/*Does not exist*/
{ LTE_RLC_PARSE_RECONFIG_TX_UM_ERR_2, LOGWARNING, (RLC_OAM),\
"RLC_PARSE_RECONFIG_TX_UM_ERR_2",\
{"LineNum","ue_index","LcId","PduType","MACOpprtunity",\
"","",\
FNAME,"",\
}},/*706*/

{ LTE_RLC_PARSE_RECONFIG_TX_UM_ENTITY, LOGINFO, (RLC_OAM),\
"Reconfigure UM TX Entity",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_RECONFIG_RX_UM_ERR, LOGWARNING, (RLC_OAM),\
"RECONFIG_RX_UM Invalid TagLen != 5 ",\
{"LineNum","TagLen","","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_RECONFIG_RX_UM_ERR_1, LOGWARNING, (RLC_OAM),\
"PARSE_RECNFG_RX_UM Invalid lcId >= 11",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_RECONFIG_RX_UM_ERR_2, LOGWARNING, (RLC_OAM),\
"Invalid tReordering < 0 or > 200 or a invalid no.(36.331)",\
{"LineNum","TagLen","LcId","t_reordering","",\
"","",\
FNAME,"",\
}},/*710*/


{ LTE_RLC_PARSE_RECONFIG_RX_UM_ENTITY, LOGINFO, (RLC_OAM),\
"Reconfigure UM RX Entity ",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_RECONFIG_RX_TX_UM_ERR, LOGWARNING, (RLC_OAM),\
"RECONFIG_RX_TX_UM Invalid TagLen != 5",\
{"LineNum","TagLen","","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_RECONFIG_RX_TX_UM_ERR_1, LOGWARNING, (RLC_OAM),\
"RECONFIG_RX_TX_UM Invalid lcId >= 11",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_RECONFIG_RX_TX_UM_ERR_2, LOGWARNING, (RLC_OAM),\
"Invalid tReordering < 0 or > 200 or a invalid Rfr.(36.331)",\
{"LineNum","TagLen","LcId","t_reordering","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_RECONFIG_RX_TX_UM_ENTITY, LOGINFO, (RLC_OAM),\
" Reconfigure UM TX Entity",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_PARSE_RECONFIG_AM_ERR, LOGWARNING, (RLC_OAM),\
"Invalid TagLen != 17 ",\
{"LineNum","TagLen","","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RECONFIG_AM_ERR_1, LOGWARNING, (RLC_OAM),\
"PARSE_RECONFIG_AM Invalid lcId >= 11 ",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RECONFIG_AM_ERR_2, LOGWARNING, (RLC_OAM),\
"Invalid tPollRetransmit < 5 or > 300 or a invalid no.(36.331)",\
{"LineNum","ue_index","LcId","PduType","MACOpprtunity",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RECONFIG_AM_ERR_3, LOGWARNING, (RLC_OAM),\
"Invalid pollPDU < 4 or > 385 or a invalid no.(36.331)",\
{"LineNum","TagLen","LcId","t_poll_retransmit","poll_pdu",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RECONFIG_AM_ERR_4, LOGWARNING, (RLC_OAM),\
"Invalid pollByte < 25 or > 3001 or a invalid no.(36.331)",\
{"LineNum","TagLen","LcId","poll_pdu","poll_byte",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RECONFIG_AM_ERR_5, LOGWARNING, (RLC_OAM),\
"Invalid maxRetxThreshold < 1 or > 32 or a invalid no.(36.331)",\
{"LineNum","TagLen","LcId","max_retx_threshold","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RECONFIG_AM_ERR_6, LOGWARNING, (RLC_OAM),\
"RECONFIG_AM Invalid tReordering < 0 or > 200 or a invalid no.(36.331)",\
{"LineNum","TagLen","LcId","t_reordering","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RECONFIG_AM_ERR_7, LOGWARNING, (RLC_OAM),\
"Invalid tStatusProhibit < 0 or > 500 or a invalid no.(36.331)",\
{"LineNum","TagLen","LcId","t_reordering","t_status_prohibit",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RECONFIG_AM_ENTITY, LOGINFO, (RLC_OAM),\
"Parse Reconfig AM entity",\
{"LineNum","TagLen","LcId","tPollRetransmit","PollPdu",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RECONFIG_AM_ENTITY_1, LOGINFO, (RLC_OAM),\
"Reconfig AM entity",\
{"LineNum","LcId","PollByte","MaxReTxThreshold","tReordering",\
"t_status_prohibit","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RE_ESTABLISH_ERR, LOGWARNING, (RLC_RRC),\
"RE_ESTABLISH Invalid TagLen != 5",\
{"LineNum","TagLen","","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RE_ESTABLISH_ERR_1, LOGWARNING, (RLC_RRC),\
"PARSE_RE_ESTABLISH Invalid lcId >= 11",\
{"LineNum","TagLen","LcId","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_RE_ESTABLISH, LOGINFO   , (RLC_OAM),\
"Reestablishing Rlc Entity",\
{"LineNum","LcId","","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_RRC_PARSE_UE_ENTITY_INFO_ERR, LOGWARNING, (RLC_OAM),\
"Invalid taglen (tagLen > msgLen) ",\
{"LineNum","Tag","TagLen","MsgLen","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_UE_ENTITY_INFO, LOGWARNING, (RLC_OAM),\
"Invalid TAG",\
{"LineNum","Tag","TagLen","MsgLen","",\
"","",\
FNAME,"",\
}}, /*730*/
/*doesn't exist in code*/
/* SPR 3894 change start */
{ LTE_RLC_RRC_PARSE_UE_ENTITY_INFO, LOGWARNING, (RLC_RRC),\
"RLC_RRC_PARSE_UE_ENTITY_INFO",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change End */
{LTE_RLC_STATE_MACHINE_INFO, LOGINFO, (RLC_RRC),\
"Rlc Process Rrc Oam Msg State: ",\
{"LineNum","cell_index","OldState"," Event"," NewState",\
"","",\
FNAME," FSM:",\
}},
/**********lteRlcRrcExtInterface.c*****************/

{LTE_RLC_SOCKET_CREATION_FAILED, LOGFATAL, (RLC_OAM),\
"SOCKET_CREATION_FAILED",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},
//does not exists in code
/* SPR 3894 change start */
{LTE_RLC_SOCKET_CREATION_FAILED_1, LOGFATAL, (RLC_OAM),\
"RLC_SOCKET_CREATION_FAILED_1 ",\
{"LineNum","ue_index","LcId","PduType","MACOpprtunity",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change End */
{LTE_RLC_SOCKET_ADDRESS_CONVERSION_ERROR, LOGFATAL, RLC_RRC,\
"inet_aton FNAMEtion failed",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_SOCKET_OPEN_ERROR, LOGFATAL, RLC_RRC,\
"Socket creation/open failed",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_SOCKET_BIND_FAILED, LOGFATAL, RLC_RRC,\
"Socket bind failed ",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},

/*********************UM Mode******************/

{LTE_RLC_ADD_DFE_TO_PDU_ERR, LOGWARNING, (RLC_TX|RLC_UM),\
"available size is less than 2 bytes",\
{"LineNum","availableSize","Size","NumDfe","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ADD_DFE_TO_PDU_ERR_1, LOGFATAL, (RLC_TX|RLC_UM),\
"MsgAlloc failed ADD_DFE_TO_PDU",\
{"LineNum","availableSize","Size","NumDfe","", \
"","",\
FNAME,"",\
}},
/* SPR 3894 change start */
{LTE_RLC_ADD_DFE_TO_PDU_ERR_2, LOGWARNING, (RLC_TX|RLC_UM),\
"MsgSplit failed Err",\
{"LineNum","availableSize","Size","NumDfe","", \
"","",\
FNAME,"",\
}},
/* SPR 3894 change End */
{LTE_RLC_ENCODE_PDU_HEADER_ERR, LOGWARNING, RLC_TX,\
"SDU Count in PDU=0, No PDU Heaader EnCoded",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_PDU_HEADER_1, LOGWARNING, (RLC_TX|RLC_UM),\
"MsgReserve failed.",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_UMD_RAW_PDU_ERR, LOGFATAL, (RLC_TX|RLC_UM),\
"Memory allocation failed.",\
{"LineNum","ue_index","LcId","availableSize","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_1, LOGFATAL , (RLC_TX|RLC_UM),\
"MsgAlloc failed for UMD_RAW_PDU",\
{"LineNum","ue_index","LcId","availableSize","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_2, LOGWARNING, (RLC_TX|RLC_UM),\
"SDU Sagmentation failed",\
{"LineNum","ue_index","LcId","availableSize","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_3, LOGWARNING, RLC_TX,\
"SDU dequeue fail from SDU Queue",\
{"LineNum","ue_index","LcId","availableTxOpp","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_4, LOGWARNING, (RLC_TX|RLC_UM),\
"Join SDU to PDU failed",\
{"LineNum","ue_index","LcId","availableSize","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_5, LOGWARNING, (RLC_TX|RLC_UM),\
"Invalid numDfe >= 512",\
{"LineNum","ue_index","LcId","availableSize","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_6, LOGWARNING, (RLC_TX|RLC_UM),\
"last dfe added was greater than 2047",\
{"LineNum","ue_index","LcId","availableSize","NumDfe", \
"last dfe","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_UMD_RAW_PDU_ERR_7, LOGWARNING, RLC_TX,\
"UM PDU Header Encoding failed.",\
{"LineNum","ue_index","LcId","AvailableTxOpp","", \
"","",\
FNAME,"",\
}},/*750*/

{LTE_RLC_ENCODE_UMD_RAW_PDU, LOGDEBUG, RLC_TX,\
"ENCODE_UMD_RAW_PDU detials1",\
{"LineNum","ue_index","LcId","AvailableSize","SzRawPdu", \
"","",\
FNAME,"",\
}},
{LTE_RLC_ENCODE_UMD_RAW_PDU_1, LOGDEBUG, RLC_TX,\
"ENCODE_UMD_RAW_PDU details2",\
{"LineNum","ue_index","LcId","UMHdrSize","UMDPduSize", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_TX_OPP_IND, LOGDEBUG, RLC_TX,\
"UM TX Opp from MAC",\
{"LineNum","ue_index","LcId","TxQSize","opp", \
"","",\
FNAME,"",\
}},



/**************"lteRlcUmTxOperation.c**************/
{LTE_RLC_RX_UM_SDU_ERR, LOGWARNING, RLC_TX,\
"UM_MAX_SDU_REVD",\
{"LineNum","ue_index","LcId","coutSDU","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RX_UM_SDU, LOGINFO, RLC_TX,\
"Receiving UM Sdu From Upper Layer",\
{"LineNum","ue_index","LcId","lcQLoad","coutSDU + 1", \
"TxQSize","SDUSize",\
FNAME,"",\
}},

{LTE_RLC_RX_UM_SDU_DISCARD_ERR, LOGWARNING, RLC_TX,\
"Search or delete SDU from Q failed.",\
{"LineNum","ue_index","LcId","Trans_Id","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_RX_UM_SDU_DISCARD, LOGINFO, RLC_TX,\
"UM SDU discarded from PDCP: Updated QLoad to MAC",\
{"ue_index","LcId","Trans_Id","SduSize","QLoad", \
"","",\
FNAME,"",\
}},

/**********lteRlcUmdDecode.c********/
{LTE_RLC_DECODE_UMD_HEADER_ERR, LOGWARNING, RLC_RX,\
"RLC Recv NULL data Pointer ",\
{"LineNum","SnLen","Size","Data_p","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_UMD_HEADER_ERR_1, LOGWARNING, RLC_RX,\
"Invalid snLen !5 and !10 Recv",\
{"LineNum","SnLength","PduSize","","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_UMD_RAW_PDU_ERR, LOGWARNING, RLC_RX,\
"Decoding of UM PDU Failed, Null data pointer",\
{"LineNum","SN","Fix Header Size","Data_p","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_UMD_RAW_PDU_ERR_1, LOGWARNING, RLC_RX,\
"Decoding of UM PDU Failed,(sz = 0)",\
{"LineNum","SN","HIdx","PduSize","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_UMD_RAW_PDU_ERR_2, LOGWARNING, RLC_RX,\
"Decoding Of UM PDU failed,Fix Header removal failed",\
{"LineNum","SN","HIdx","PduSize","RetVal", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_UMD_RAW_PDU, LOGDEBUG, RLC_RX,\
"Full SDU present in recv PDU",\
{"LineNum","SN","HIdx","PduSize","Fi", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_UMD_RAW_PDU_ERR_4, LOGFATAL, RLC_RX,\
"Memory allocation failed",\
{"LineNum","SN","HIdx","PduSize","RetVal", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_AND_ENQ_SEG_ERR, LOGWARNING, RLC_RX,\
"Invalid segment length",\
{"LineNum","No. of dfe","LI","Dfe size used","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_AND_ENQ_SEG, LOGDEBUG, RLC_RX,\
"SDU Enqueued to Queue",\
{"LineNum","SduCnt","DfeSz","EnqDataSz","RemainDataSz", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_AND_ENQ_LAST_SEGMENT, LOGDEBUG, RLC_RX,\
"Last SDU Enqueued",\
{"LineNum","SduCnt","DfeSz","Fi","", \
"","",\
FNAME,"",\
}},

/*************lteRlcUmRxOperation.c***********/

{LTE_RLC_RECEIVE_UM_PDU_ERR, LOGWARNING, RLC_RX,\
"Handle Um Pdu From Lower Layer failed",\
{"LineNum","ue_index","LcId","SN","RetVal", \
"","",\
FNAME,"",\
}},

{LTE_RLC_IS_VALID_RX_UM_PDU_ERR, LOGWARNING, RLC_RX,\
"Duplicate PDU recv, Discarded",\
{"LineNum","SN","LcId","ue_index","snRingStatus", \
"","",\
FNAME,"",\
}},

{LTE_RLC_IS_VALID_RX_UM_PDU_ERR_1, LOGWARNING, RLC_RX,\
"Out of Window PDU Recv,Discarded",\
{"LineNum","UR","SN","UH","RetVal", \
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_UM_PDU_ERR, LOGWARNING, RLC_RX,\
"Received Null data or Invalid SN Len",\
{"LineNum","ue_index","LcId","Size","RetVal", \
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_UM_PDU_ERR_1, LOGWARNING, RLC_RX,\
"Packet duplicate or out of window",\
{"LineNum","ue_index","LcId","Size","SN", \
"RetVal","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_UM_PDU_ERR_2, LOGWARNING, RLC_RX,\
"Decode Umd Raw Pdu failed.",\
{"LineNum","ue_index","LcId","Size","SN", \
"RetVal","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_UM_PDU_ERR_3, LOGDEBUG, RLC_RX,\
"Sn checked: IN/OUT window ",\
{"LineNum","UH","SN","UH","Mbase", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER, LOGDEBUG, RLC_RX,\
"SeqNum added in list for reassembly",\
{"LineNum","ue_index","LcId","SN","UH", \
"","",\
FNAME,"",\
}},

/*{LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_1, LOGDEBUG, RLC_RX,\
"SN Updated",\
{"LineNum","ue_index","LcId","tempSN","UH", \
"MBase","",\
FNAME,"",\
}},*/
{LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_2, LOGDEBUG, RLC_RX,\
"UR updated: falls outside of the reordering window",\
{"ue_index","LcId","UH","UR","window size" \
"","",\
FNAME,"",\
}},

{LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_3, LOGDEBUG, RLC_RX,\
"State variables updated",\
{"ue_index","LcId","UH","UR","window size", \
"","",\
FNAME,"",\
}},


{LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_4, LOGDEBUG, RLC_RX,\
"SN received is same as UR",\
{"ue_index","LcId","UH","UR","SN", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_5, LOGDEBUG, RLC_RX,\
"UR Updated",\
{"ue_index","LcId","UH","UR","SN received", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_6, LOGDEBUG, RLC_RX,\
"SN added in list for reassembly",\
{"ue_index","LcId","UH","UR","SN", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_7, LOGDEBUG, RLC_RX,\
"UR Updated after reassembly",\
{"ue_index","LcId","UH","UR","SN", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_8, LOGDEBUG, RLC_RX,\
"Out of order PDU received",\
{"ue_index","LcId","UH","UR","SN" \
"","",\
FNAME,"",\
}},

{LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_9, LOGDEBUG, RLC_RX,\
"Stop and reset ReOrdering Timer",\
{"ue_index","LcId","UH","UR","UX", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_10, LOGDEBUG, RLC_RX,\
"Start ReOrdering Timer",\
{"ue_index","LcId","UH","UR","UX", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ACTION_UM_RO_TIMER_EXPIRY_ERR, LOGWARNING, (RLC_RX),\
"Rg_p=NULL",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_ACTION_UM_RO_TIMER_EXPIRY_ERR_1, LOGDEBUG, (RLC_RX),\
"Dropping the SN due to RO Timer Expiry",\
{"LineNum","ue_index","LcId","UX","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ACTION_UM_RO_TIMER_EXPIRY, LOGDEBUG, (RLC_RX),\
"UR updated",\
{"LineNum","ue_index","LcId","UR","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ACTION_UM_RO_TIMER_EXPIRY_1, LOGDEBUG, (RLC_RX),\
"Added SN List",\
{"LineNum","ue_index","LcId","UR","SzUsed", \
"","",\
FNAME,"",\
}},


{LTE_RLC_ACTION_UM_RO_TIMER_EXPIRY_2, LOGDEBUG, (RLC_RX),\
"RO Timer Start",\
{"LineNum","ue_index","LcId","UR","UX", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DELIVER_UM_SDU_DIRECTLY, LOGDEBUG, RLC_RX,\
"RLC SDU delivered to PDCP",\
{"LineNum","ue_index","LcId","SDU size","", \
"","",\
FNAME,"",\
}},

{LTE_RLC_REASSEMBLE_IN_CMP_SDU_ERR, LOGWARNING, RLC_RX,\
"Msg Join Failed!",\
{"ue_index","LcId","SN","Size of fisrt sdu","size of second sdu", \
"","",\
FNAME,"",\
}},

{LTE_RLC_REASSEMBLE_IN_CMP_SDU, LOGDEBUG, RLC_RX,\
"Joined: Incomplete SDU to new one received",\
{"LineNum","ue_index","LcId","SN","SDU status", \
"","",\
FNAME,"",\
}},

{LTE_RLC_REASSEMBLE_IN_CMP_SDU_ERR_1, LOGWARNING, RLC_RX,\
"Failed: Msg Join",\
{"LineNum","ue_index","LcId","SN","SDU status", \
"","",\
FNAME,"",\
}},

{LTE_RLC_REASSEMBLE_IN_CMP_SDU_1, LOGDEBUG, RLC_RX,\
"Joined & reserve Incomplete SDU to new one received",\
{"LineNum","ue_index","LcId","SN","SDU status", \
"","",\
FNAME,"",\
}},

{LTE_RLC_REASSEMBLE_IN_CMP_SDU_2, LOGDEBUG, RLC_RX,\
"Full sdu received :Reassemble & deliver to PDCP",\
{"ue_index","LcId","SN","Size of SDU received","SDU status", \
"","",\
FNAME,"",\
}},

{LTE_RLC_REASSEMBLE_IN_CMP_SDU_ERR_2, LOGWARNING, RLC_RX,\
"Invalid status of SDU received for reassembly",\
{"ue_index","LcId","SN","Size of SDU received","SDU status", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DELIVER_UM_SDU_TO_PDCP, LOGDEBUG, RLC_RX,\
"Reassembly done: complete SDU received",\
{"ue_index","LcId","SDU status","SN","SDU size", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DELIVER_UM_SDU_TO_PDCP_1, LOGDEBUG, RLC_RX,\
"Incomplete SN resereved for reassembly",\
{"ue_index","LcId","SDU status","SN","SDU size", \
"","",\
FNAME,"",\
}},

{LTE_RLC_DELIVER_UM_SDU_TO_PDCP_ERR, LOGWARNING, RLC_RX,\
"Invalid UM SDU Status",\
{"ue_index","LcId","SDU tatus","SN","SDU size", \
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_RLC_REASSEMBLE_SDU, LOGWARNING, RLC_RX,\
"Incomplete PDU discarded: Either out of sequence or PDU portion missing",\
{"ue_index","LcId","SN","Dfe status","Incomplete PDU size", \
"SzUsed","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_RLC_REASSEMBLE_SDU_1, LOGDEBUG, RLC_RX,\
"SDU portion received pending for reassembly",\
{"ue_index","LcId","SN","SDU status","SDU size", \
"","",\
FNAME,"",\
}},


{LTE_RLC_HANDLE_RLC_REASSEMBLE_UM_SDU, LOGDEBUG, RLC_RX,\
"Reassemle UM PDU",\
{"LineNum","ue_index","LcId","No.of PDU reassembled","", \
"","",\
FNAME,"",\
}},

/***********************END of UM Mode*********************/

/*************lteRlcRxProcessor.c******************/

{LTE_RLC_MSG_ALLOC_FAIL, LOGFATAL, (RLC_RX),\
"Msg Alloc failed for common Ch data",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR, LOGWARNING, (RLC_RX),\
"PDU received from MAC is of size zero",\
{"LineNum","ue_index","LcId","PduSize","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_1, LOGWARNING, (RLC_RX),\
"NULL PDU received from MAC",\
{"LineNum","ue_index","LcId","PduSize","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_2, LOGWARNING, (RLC_RX),\
"Message size is not equal to data length of the PDU",\
{"LineNum","ue_index","LcId","PduSize","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_3, LOGWARNING, (RLC_RX),\
"Invalid LcId in Uplink data ",\
{"LineNum","ue_index","LcId","PduSize","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_4, LOGWARNING, (RLC_RX),\
"RLC Entity doesn't exist",\
{"LineNum","ue_index","LcId","PduSize","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_5, LOGWARNING, (RLC_RX),\
"RLC Entity is not in Active Mode",\
{"LineNum","ue_index","LcId","PduSize","RlcEntityState",\
"","",\
FNAME,"",\
}},

{LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_6, LOGWARNING, (RLC_RX),\
"UM Entity or RX UM Entity does not exist",\
{"LineNum","ue_index","LcId","PduSize","RlcEntityMode",\
"","",\
FNAME,"",\
}},

{LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_7, LOGWARNING, (RLC_RX),\
"AM Entity or RX AM Entity does not exist",\
{"LineNum","ue_index","LcId","PduSize","RlcEntityMode",\
"","",\
FNAME,"",\
}},

{LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_8, LOGWARNING, (RLC_RX),\
"Invalid RLC Entity Mode (Neither UM or AM)",\
{"LineNum","ue_index","LcId","PduSize","RlcEntityMode",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_UM_RX_PACKET, LOGDEBUG, (RLC_RX),\
"RLC UM PDU Handled Successfully",\
{"LineNum","ue_index","LcId","DataLength","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_UM_RX_PACKET_ERR, LOGWARNING, (RLC_RX),\
"Unable to handle UM Rx data PDU",\
{"LineNum","ue_index","LcId","DataLength","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_AM_RX_PACKET, LOGDEBUG, (RLC_RX),\
"RLC AM PDU Decode Successfully",\
{"LineNum","ue_index","LcId","DataLength","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_AM_RX_PACKET_ERR , LOGWARNING, (RLC_RX),\
"Decoding Failed For RX AM PDU",\
{"LineNum","ue_index","LcId","DataLength","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_DATA_FROM_MAC_ERR , LOGWARNING, (RLC_RX),\
"NULL UeDataInd pointer received from MAC",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_DATA_FROM_MAC_ERR_1 , LOGWARNING, (RLC_RX),\
"UE Context does not exist for data received from MAC",\
{"LineNum","ue_index","lcCount","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_DATA_FROM_MAC_ERR_2 , LOGWARNING, (RLC_RX),\
"Invalid Rlc Entity for data received from MAC",\
{"LineNum","ue_index","lcCount","RlcEntityMode","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_HEADER_INFO , LOGDEBUG, (RLC_RX),\
"PDU variable header details",\
{"LineNum","DfeCount","LI","PosOfEbit","E",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_HEADER_INFO_1 , LOGDEBUG, (RLC_RX),\
"UM PDU Header Info",\
{"LineNum","HeaderSize","DfeCount","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DB_DELETE_RLC_ENTITY , LOGERROR, (RLC_RX),\
"DELETE_RLC_ENTITY UE Ctx does not exist",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DB_DELETE_RLC_ENTITY_1 , LOGERROR, (RLC_RX),\
"DB_DELETE_RLC_ENTITY UE Ctx does not exist",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_RECEIVE_UM_PDU_ERR_1, LOGWARNING, (RLC_RX),\
"Duplicate or out of window PDU received",\
{"LineNum","ue_index","LcId","SN","RetVal", \
"","",\
FNAME,"",\
}},

{LTE_RLC_RECEIVE_UM_PDU_ERR_2, LOGWARNING, (RLC_RX),\
"Decoding of Recv PDU Failed",\
{"LineNum","ue_index","LcId","PRawPdu.size","SN", \
"RetVal","",\
FNAME,"",\
}},
{LTE_RLC_RECEIVE_UM_PDU, LOGDEBUG, (RLC_RX),\
"Recv PDU from Mac",\
{"LineNum","ue_index","LcId","SN","UR", \
"UH","",\
FNAME,"",\
}},


/**************End of lteRlcRxProcessor.c**********/


/***********Start of lteRlcTxProcessor.c***********/

{LTE_RLC_IS_VALID_UE_DATA_REQ_ERR, LOGWARNING, RLC_TX,\
"Invalid UE data",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_IS_VALID_UE_DATA_REQ_ERR_1, LOGWARNING, RLC_TX,\
"Invalid UE data or Inactive",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_ENTITY_ERR, LOGWARNING, RLC_TX,\
"RLC_GET_ENTITY_ERR",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_ENTITY_ERR_1, LOGWARNING, RLC_TX,\
"ENTITY_NOT_ACTIVE_MODE",\
{"LineNum","ue_index","LcId","RlcEntityState","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_ENTITY_ERR_2, LOGWARNING, RLC_TX,\
"AM_ENTITY_NOT_EXIST",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_ENTITY_ERR_3, LOGWARNING, RLC_TX,\
"UM_ENTITY_NOT_EXIST",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_ENTITY_ERR_4, LOGWARNING, RLC_TX,\
"GET_ENTITY: Invalid Entity Mode",\
{"LineNum","ue_index","LcId","RlcEntityMode","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_DATA_FROM_PDCP_ERR, LOGWARNING, RLC_TX,\
"FROM_PDCP: Invalid Entity Mode",\
{"LineNum","ue_index","LcId","RlcEntityMode","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR, LOGWARNING, RLC_TX,\
"NULL HARQ Pointer",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR_1, LOGWARNING, RLC_TX,\
"Sostart > Soend",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR_2, LOGWARNING, RLC_TX,\
"Invalid UE",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR_3, LOGWARNING, RLC_TX,\
"UE not active or Invalid mode",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR_4, LOGWARNING, RLC_TX,\
"HARQ_FAILURE",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR_5, LOGWARNING, RLC_TX,\
"Invalid Tx PDU",\
{"LineNum","ue_index","LcId","SN","ReTxId",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_HARQ_FAILURE_IND, LOGINFO, RLC_TX,\
"HANDLE_HARQ_FAILURE_IND",\
{"LineNum","ue_index","LcId","SN","RetxnPduSize",\
"ctrlQLoad","lcQueueLoad",\
FNAME,"",\
}},

{LTE_RLC_UM_TX_OPP_ERR, LOGINFO, RLC_TX,\
"UMD_PDU Encoded",\
{"LineNum","ue_index","LcId","TxQSize","US",\
"TxOppSize","",\
FNAME,"",\
}},

{LTE_RLC_UM_TX_OPP_ERR_1, LOGWARNING, RLC_TX,\
"UMD PDU Encoding fail",\
{"LineNum","ue_index","LcId","TxQSize","TxOppSize",\
"","",\
FNAME,"",\
}},

{LTE_RLC_AM_TX_OPP_ERR, LOGWARNING, RLC_TX,\
"RLC_AM_TX_OPP_ERR",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_TX_OPP_VALIDATE_1, LOGWARNING, RLC_TX,\
"INVALID LCID in Tx Opp",\
{"LineNum","Rnti","LcId","TxOpp","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_TX_OPP_VALIDATE_2, LOGWARNING, RLC_TX,\
"LC Entity is not Active or exists",\
{"LineNum","Rnti","LcId","TxOpp","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_TX_OPP_VALIDATE_3, LOGWARNING, RLC_TX,\
"Invalid Opp from MAC ( 1 >= TxOpp )",\
{"LineNum","Rnti","LcId","TxOpp","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_TX_OPP_VALIDATE_4, LOGWARNING, RLC_TX,\
"TX_OPP: INVALID_TX_ENTITY in Tx Opp",\
{"LineNum","Rnti","LcId","TxOpp","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_TX_OPP_VALIDATE_5, LOGWARNING, RLC_TX,\
"INVALID_TX_ENTITY in Tx Opp",\
{"LineNum","Rnti","LcId","TxOpp","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_TX_OPP_1, LOGWARNING, RLC_TX,\
"UE_OPP_IND_NULL",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_TX_LD_CHANGE, LOGWARNING, RLC_TX,\
"Overload level changed.",\
/*+ Dyn UEs To Be SchPerTTI +*/
{"LineNum","currTickOverLoad","MacOverLoadLevel","DlUeSchdInLastTick",\
"ulUeSchdInLastTick",\
/*- Dyn UEs To Be SchPerTTI -*/
"","",\
FNAME,"",\
}},

{LTE_RLC_TX_OPP_2, LOGWARNING, RLC_TX,\
"TX_CTX_NOT_FOUND",\
{"LineNum","ue_index","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_TX_OPP_3, LOGWARNING, RLC_TX,\
"TX_OPP: Invalid Entity mode",\
{"LineNum","Rnti","LcId","TxOpp","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_TX_OPP_4, LOGWARNING, RLC_TX,\
"AM_TX_OPP_ERR",\
{"LineNum","Rnti","lcCount","","",\
"","",\
FNAME,"",\
}},
/***********End of lteRlcTxProcessor.c***********/
/**************************Start RLC Timer lteRlcTimer.c ***********/

{LTE_RLC_START_SP_TIMER_ERR , LOGWARNING, RLC_TIMER,\
"SP -> RX AM Entity Ptr is NULL",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_START_SP_TIMER_ERR_1 , LOGWARNING, RLC_TIMER,\
" SP Timer is already Running",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_START_SP_TIMER_ERR_2 , LOGWARNING, RLC_TIMER,\
"AM SP START TIMER Get Mem Pool Failed for Timer Node",\
{"LineNum","ue_index","LcId","RetVal","ExpiryIndex",\
"","",\
FNAME,"",\
}},
{
LTE_RLC_START_SP_TIMER , LOGINFO, RLC_TIMER,\
"Status Prbht Timer started",\
{"LineNum","ue_index","LcId","StartTick","Duration",\
"ExpiryIdx","",\
FNAME,"",\
}
},

{LTE_RLC_STOP_SP_TIMER_ERR , LOGWARNING, RLC_TIMER,\
"STOP SP RX AM Entity Ptr is NULL",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_STOP_SP_TIMER_ERR_1, LOGWARNING, RLC_TIMER,\
"Status Prbht timer is not running",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{ LTE_RLC_STOP_SP_TIMER_ERR_2, LOGWARNING, RLC_TIMER,\
"Status Prbht timer:- expirtyIdx is invalid",\
{"LineNum","ue_index","LcId","SpDuration","SpExpiryIdx",\
"","",\
FNAME,"",\
}},
{ LTE_RLC_STOP_SP_TIMER, LOGINFO, RLC_TIMER,\
"Stop the Status Prbht Timer",\
{"LineNum","ue_index","LcId","StopTick","SpDuration",\
"SpExpiryIdx","",\
FNAME,"",\
}},
{ LTE_RLC_EXPIRE_SP_TIMER_ERR, LOGWARNING, RLC_TIMER,\
"Aready Stop SP timer, so, no need to take any action for expiry",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_EXPIRE_SP_TIMER_ERR_1, LOGWARNING, RLC_TIMER,\
"During SP Expiry: UE Ctx does not exist",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_EXPIRE_SP_TIMER_ERR_2, LOGWARNING, RLC_TIMER,\
"EXPIRE_SP_TIMER: RLC Entity does not exit",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change start */
{LTE_RLC_EXPIRE_SP_TIMER_ERR_3, LOGWARNING, RLC_TIMER,\
" RLC Entity is not in active mode or not in AM Mode EXPIRE_SP_TIMER",\
{"LineNum","ue_index","LcId","RlcEntityState","RlcEntityMode",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change End */
{ LTE_RLC_EXPIRE_SP_TIMER, LOGINFO, RLC_TIMER,\
"Expired the Status Prbht timer",\
{"LineNum","ue_index","LcId","ExpiryTime"," spDuration",\
" spExpiryIdx","",\
FNAME,"",\
}},

{ LTE_RLC_RESTART_SP_TIMER_ERR, LOGWARNING, RLC_TIMER,\
"RX AM entitiy ptr is null",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},
{ LTE_RLC_RESTART_SP_TIMER, LOGINFO, RLC_TIMER,\
"In Restart SP Timer..After Stop the SP Timer",\
{"LineNum","ue_index","LcId","StopSPTime","SpDuration",\
"SpExpiryIdx","RetVal",\
FNAME,"",\
}},
{ LTE_RLC_RESTART_SP_TIMER_1, LOGINFO, RLC_TIMER,\
"In Restart SP Timer..After Start the SP Timer",\
{"LineNum","ue_index","LcId","StartSPTimer","SpDuration",\
"SpExpiryIdx","RetVal",\
FNAME,"",\
}},

/******Start UM RO Timer ********************/
{LTE_RLC_START_UM_RO_TIMER_ERR, LOGWARNING, RLC_TIMER,\
"START_RO_TIMER RX UM Entity Ptr is NULL",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_START_UM_RO_TIMER_ERR_1, LOGWARNING, RLC_TIMER,\
"UM RO Timer is already Running",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},
{ LTE_RLC_START_UM_RO_TIMER_ERR_2, LOGWARNING, RLC_TIMER,\
"UM RO START Get Mem Pool Failed for Timer Node",\
{"LineNum","ue_index","LcId","RetVal","ExpiryIndex",\
"","",\
FNAME,"",\
}},
{ LTE_RLC_START_UM_RO_TIMER , LOGINFO, RLC_TIMER,\
"UM RO Timer started",\
{"LineNum","ue_index","LcId","StartTick","Duration",\
"ExpiryIdx","",\
FNAME,"",\
}},
{ LTE_RLC_STOP_UM_RO_TIMER_ERR , LOGWARNING, RLC_TIMER,\
"STOP_RO_TIMER RX UM Entity Ptr is NULL",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},
{ LTE_RLC_STOP_UM_RO_TIMER_ERR_1, LOGWARNING, RLC_TIMER,\
"UM ReOrdering timer is not running",\
{"LineNum","ue_index","LcId","Timer Duration","Timer Expiry",\
"","",\
FNAME,"",\
}},
{ LTE_RLC_STOP_UM_RO_TIMER_ERR_2 , LOGWARNING, RLC_TIMER,\
"UM ReOrdering timer: expirtyIdx is invalid",\
{"LineNum","ue_index","LcId","Timer Duration","Expiry Index",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_STOP_UM_RO_TIMER, LOGINFO, RLC_TIMER,\
"Stop the UM ReOrdering Timer",\
{"ue_index","LcId","StopTick","Timer Duration","Expiry Index",\
"","",\
FNAME,"",\
}},


{LTE_RLC_EXPIRE_UM_RO_TIMER_ERR , LOGWARNING, RLC_TIMER,\
"Aready Stop UM RO timer, so, no need to take any action for expiry",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_EXPIRE_UM_RO_TIMER_ERR_1 , LOGWARNING, RLC_TIMER,\
"During UM RO Expiry: UE Ctx does not exist",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},


{ LTE_RLC_EXPIRE_UM_RO_TIMER_ERR_2, LOGWARNING, RLC_TIMER,\
"UM_RO_TIMER: RLC Entity does not exit",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_EXPIRE_UM_RO_TIMER_ERR_3, LOGWARNING, RLC_TIMER,\
" RLC Entity is not in active mode or not in UM Mode",\
{"LineNum","ue_index","LcId","RlcEntityState","RlcEntityMode",\
"","",\
FNAME,"",\
}},

{LTE_RLC_EXPIRE_UM_RO_TIMER, LOGINFO, RLC_TIMER,\
"Expired the UM RO timer",\
{"LineNum","ue_index","LcId","ExpiryTick"," roDuration",\
" roExpiryIdx","",\
FNAME,"",\
}},

{LTE_RLC_RESTART_UM_RO_TIMER_ERR , LOGWARNING, RLC_TIMER,\
"RX UM entitiy ptr is null",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_RESTART_UM_RO_TIMER, LOGINFO, RLC_TIMER,\
"In Restart UM RO Timer..After Stop the UM RO Timer",\
{"LineNum","ue_index","LcId","StopROTick","RoDuration",\
"RoExpiryIdx","RetVal",\
FNAME,"",\
}},

{ LTE_RLC_RESTART_UM_RO_TIMER_1, LOGINFO, RLC_TIMER,\
"In Restart UM RO Timer..After Start the UM RO Timer",\
{"LineNum","ue_index","LcId","StartROTick","RoDuration",\
"RoExpiryIdx","RetVal",\
FNAME,"",\
}},

/***************Start AM RO Timer ********************/
{ LTE_RLC_START_AM_RO_TIMER_ERR , LOGWARNING, RLC_TIMER,\
"START AM RO Timer RX AM Entity Ptr is NULL",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_START_AM_RO_TIMER_ERR_1 , LOGWARNING, RLC_TIMER,\
" AM RO Timer is already Running",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_START_AM_RO_TIMER_ERR_2 , LOGWARNING, RLC_TIMER,\
"AM RO START Get Mem Pool Failed for Timer Node",\
{"LineNum","ue_index","LcId","RetVal","ExpiryIndex",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_START_AM_RO_TIMER , LOGINFO, RLC_TIMER,\
"AM RO Timer started",\
{"LineNum","ue_index","LcId","StartTick","Duration",\
"ExpiryIdx","",\
FNAME,"",\
}},
{LTE_RLC_STOP_AM_RO_TIMER_ERR , LOGWARNING, RLC_TIMER,\
"STOP AM RO TIMER RX AM Entity Ptr is NULL",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_STOP_AM_RO_TIMER_ERR_1, LOGWARNING, RLC_TIMER,\
"AM RO timer is not running",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_STOP_AM_RO_TIMER_ERR_2, LOGWARNING, RLC_TIMER,\
"AM RO timer:- expirtyIdx is invalid",\
{"LineNum","ue_index","LcId","RoDuration","roExpiryIdx",\
"","",\
FNAME,"",\
}},

{LTE_RLC_STOP_AM_RO_TIMER, LOGINFO, RLC_TIMER,\
"Stop the AM RO Timer",\
{"LineNum","ue_index","LcId","StopTick","RoDuration",\
"RoExpiryIdx","",\
FNAME,"",\
}},

{LTE_RLC_EXPIRE_AM_RO_TIMER_ERR , LOGWARNING, RLC_TIMER,\
"Aready Stop AM RO timer, so, no need to take any action for expiry",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

/* SPR 3894 change start */
{ LTE_RLC_EXPIRE_AM_RO_TIMER_ERR_1, LOGWARNING, RLC_TIMER,\
"AM RO Expiry: UE Ctx does not exist",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change End */

{ LTE_RLC_EXPIRE_AM_RO_TIMER_ERR_2, LOGWARNING, RLC_TIMER,\
"AM_RO_TIMER: RLC Entity does not exit",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change start */
{ LTE_RLC_EXPIRE_AM_RO_TIMER_ERR_3, LOGWARNING, RLC_TIMER,\
" RLC Entity is not in active mode or not in AM Mode EXPIRE_AM_RO_TIMER",\
{"LineNum","ue_index","LcId","RlcEntityState","RlcEntityMode",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change End */

{ LTE_RLC_EXPIRE_AM_RO_TIMER, LOGINFO, RLC_TIMER,\
"Expired the AM RO timer",\
{"LineNum","ue_index","LcId","ExpiryTick"," roDuration",\
" roExpiryIdx","",\
FNAME,"",\
}},

{LTE_RLC_RESTART_AM_RO_TIMER_ERR, LOGWARNING, RLC_TIMER,\
"RESTART AM RO RX AM Entity Ptr is NULL",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_RESTART_AM_RO_TIMER , LOGINFO, RLC_TIMER,\
"In Restart AM RO Timer..After Stop the AM RO Timer",\
{"LineNum","ue_index","LcId","StopROTick","RoDuration",\
"RoExpiryIdx","RetVal",\
FNAME,"",\
}},

{LTE_RLC_RESTART_AM_RO_TIMER_1 , LOGINFO, RLC_TIMER,\
"In Restart AM RO Timer..After Start the AM RO Timer",\
{"LineNum","ue_index","LcId","StartROTick","RoDuration",\
"RoExpiryIdx","RetVal",\
FNAME,"",\
}},

/****************** START PR Timer *****************/
{LTE_RLC_START_AM_PR_TIMER_ERR, LOGWARNING, RLC_TIMER,\
"START_AM_PR TX AM Entity Ptr is NULL",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_START_AM_PR_TIMER_ERR_1, LOGWARNING, RLC_TIMER,\
" PR Timer is already Running",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_START_AM_PR_TIMER_ERR_2 , LOGWARNING, RLC_TIMER,\
"AM PR START Get Mem Pool Failed for Timer Node",\
{"LineNum","ue_index","LcId","RetVal","ExpiryIndex",\
"","",\
FNAME,"",\
}},

{LTE_RLC_START_AM_PR_TIMER, LOGINFO, RLC_TIMER,\
"PR Timer started",\
{"LineNum","ue_index","LcId","StartTick","PrDuration",\
"ExpiryIdx","",\
FNAME,"",\
}},

{LTE_RLC_STOP_AM_PR_TIMER_ERR, LOGWARNING, RLC_TIMER,\
"STOP_AM_PR TX AM Entity Ptr is NULL",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_STOP_AM_PR_TIMER_ERR_1 , LOGWARNING, RLC_TIMER,\
"PR timer is not running",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_STOP_AM_PR_TIMER_ERR_2, LOGWARNING, RLC_TIMER,\
"PR timer:- expirtyIdx is invalid",\
{"LineNum","ue_index","LcId","SpDuration","SpExpiryIdx",\
"","",\
FNAME,"",\
}},

{LTE_RLC_STOP_AM_PR_TIMER , LOGINFO, RLC_TIMER,\
"Stop the PR Timer",\
{"LineNum","ue_index","LcId","StopTick","PrDuration",\
"PrExpiryIdx","",\
FNAME,"",\
}},

{ LTE_RLC_EXPIRE_AM_PR_TIMER_ERR, LOGWARNING, RLC_TIMER,\
"Aready Stop PR timer, so, no need to take any action for expiry",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

/* SPR 3894 change start */
{ LTE_RLC_EXPIRE_AM_PR_TIMER_ERR_1, LOGWARNING, RLC_TIMER,\
"During AM RO Expiry: UE Ctx does not exist",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change End */

{ LTE_RLC_EXPIRE_AM_PR_TIMER_ERR_2, LOGWARNING, RLC_TIMER,\
"AM_PR_TIMER: RLC Entity does not exit",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

/* SPR 3894 change start */
{ LTE_RLC_EXPIRE_AM_PR_TIMER_ERR_3, LOGWARNING, RLC_TIMER,\
" RLC Entity is not in active mode or not in AM Mode EXPIRE_AM_PR_TIMER",\
{"LineNum","ue_index","LcId","RlcEntityState","RlcEntityMode",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change End */

{ LTE_RLC_EXPIRE_AM_PR_TIMER, LOGINFO, RLC_TIMER,\
"Expired the PR timer",\
{"LineNum","ue_index","LcId","ExpiryTick","PrDuration",\
"PrExpiryIdx","",\
FNAME,"",\
}},

{ LTE_RLC_RESTART_AM_PR_TIMER_ERR, LOGWARNING, RLC_TIMER,\
"RESTART_PR_TIMER TX AM Entity Ptr is NULL",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_RESTART_AM_PR_TIMER, LOGINFO, RLC_TIMER,\
"In Restart PR Timer..After Stop the PR Timer",\
{"LineNum","ue_index","LcId","StopPRTick","PrDuration",\
"PrExpiryIdx","RetVal",\
FNAME,"",\
}},

{ LTE_RLC_RESTART_AM_PR_TIMER_1, LOGINFO, RLC_TIMER,\
"In Restart PR Timer..After Start the PR Timer",\
{"LineNum","ue_index","LcId","StartPRTick","PrDuration",\
"PrExpiryIdx","RetVal",\
FNAME,"",\
}},


/*****************End RLC Timer **********************/
{LTE_RLC_PDU_TYPE, LOGDEBUG, (RLC_TX|RLC_AM),\
"MAC Provided Opportunity to RLC",\
{"LineNum","ue_index","LcId","PduType","MACOpprtunity",\
"","",\
FNAME,"",\
}},
{LTE_RLC_POLLING_INFO, LOGDEBUG, ((RLC_TX|RLC_AM)),\
"PDU Polling Info", \
{"ue_index","LcId","PollBit","ByteWithoutPoll","PduWithoutPoll", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SETTING_POLL_BIT_1, LOGDEBUG,(RLC_TX|RLC_AM),\
"LTE_RLC_SETTING_POLL_BIT_1", \
{"LineNum","ue_index","LcId","TxQSize","ReTxQSize",\
"TxPduIn.pRawPdu.size","", \
FNAME,"",\
}},
{LTE_RLC_SETTING_POLL_BIT_2, LOGDEBUG,(RLC_TX|RLC_AM),\
"LTE_RLC_SETTING_POLL_BIT_2", \
{"LineNum","ue_index","LcId","poll_pdu","poll_byte", \
"","",\
FNAME,"",\
}},
{LTE_RLC_POLL_BIT_NEW_TRANSMISSION, LOGDEBUG,(RLC_TX|RLC_AM),\
"PollBit for new transmission", \
{"LineNum","ue_index","LcId","PollBit","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SUCCESS_IND_TO_PDCP, LOGDEBUG, (RLC_TX|RLC_AM),\
"Success indication to PDCP ", \
{"LineNum","ue_index","LcId","Trans_Id","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_PDU_SUCCESS_IND_TO_PDCP, LOGDEBUG, (RLC_TX|RLC_AM),\
"Sending successful delivery indication of AMD PDU to PDCP", \
{"LineNum","ue_index","LcId","SN","TxIdCount", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SETTING_POLL_BIT_FOR_MORE_TX, LOGDEBUG, (RLC_TX|RLC_AM),\
"Setting Poll bit For Retransmission ", \
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SN_OUTSIDE_TX_WINDOW, LOGDEBUG, (RLC_TX|RLC_AM),\
"SN falls outside of the transmitting window  ", \
{"ue_index","LcId","vtA","LastAckReceived","vtMS", \
"","",\
FNAME,"",\
}},
{LTE_RLC_POLL_TIMER_EXPIRE, LOGDEBUG, (RLC_TX|RLC_AM),\
"NACK list should empty on expiry of poll retransmission ", \
{"LineNum","ue_index","LcId","NackListCount","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NO_NACK_IN_STATUS_REPORT, LOGDEBUG, (RLC_TX|RLC_AM),\
"No NACK received in the status report.", \
{"LineNum","ue_index","LcId","PointerSize","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_ENQUEUEING_NACK_LIST, LOGDEBUG, (RLC_TX|RLC_AM),\
"ENQUEUE nack list ", \
{"LineNum","ue_index","LcId","SN","SoStart", \
"SoEnd","",\
FNAME,"",\
}},
{LTE_RLC_NO_PDU_FOR_TRANSMISSION, LOGDEBUG, (RLC_TX|RLC_AM),\
"No new RLC data PDU can be transmitted ", \
{"LineNum","ue_index","LcId","Nack","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_WRONG_INDEX_FOR_RE_TX, LOGFATAL, (RLC_TX|RLC_AM),\
"Getting wrong index for retransmission", \
{"LineNum","ue_index","LcId","ReTxId","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RECEIVED_LAST_ACK, LOGDEBUG, (RLC_TX|RLC_AM),\
"Last Ack Received : Stopping poll-timer   ", \
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NACK_ON_POLL_TIMER_EXPIRY, LOGDEBUG, (RLC_TX|RLC_AM),\
"Poll Timer Expired" ,\
{"LineNum","ue_index","LcId","vtS","NackSn", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RE_TX_AM_SDU, LOGDEBUG, (RLC_TX|RLC_AM),\
"Retransmission of AM PDU with SN = VT(S) - 1", \
{"ue_index","LcId","SN","SoStart","SoEnd", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SN_NOT_RE_TX, LOGDEBUG, (RLC_TX|RLC_AM),\
"AMD PDU which has not been acknowledged for retransmission ", \
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_WRONG_INDEX_FOR_SUCCESS_IND, LOGWARNING, (RLC_TX|RLC_AM),\
"Received wrong idx : ReTx QueueId is more than window size ", \
{"LineNum","ue_index","LcId","ReTxQIdx","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_DECODE_STATUS_PDU, LOGDEBUG, (RLC_TX|RLC_AM),\
"Receiving Wrong NACK, Unable to retransmit PDU", \
{"LineNum","ue_index","LcId","Nack","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_OUT_OF_RANGE_SN_IN_DEL_IND, LOGWARNING, (RLC_TX|RLC_AM),\
"Can't free out of range during Del Ind",\
{"ue_index","LcId","SN","vtA","vtS",\
"","",\
FNAME,"",\
}},
{LTE_RLC_MAC_BO_REQUEST, LOGDEBUG, (RLC_TX),\
"RLC update the qload to MAC", \
{"LineNum","ue_index","LcId","QLoad","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_POLL_BIT_FOR_RE_TX, LOGDEBUG, (RLC_TX|RLC_AM),\
"Including poll bit in the RLC data PDU ", \
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NO_TID_PRESENT, LOGWARNING, (RLC_TX|RLC_AM),\
"No transactionId found ", \
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NO_TX_BUFFER, LOGWARNING, (RLC_TX|RLC_AM),\
"No transmission buffer present ", \
{"ue_index","LcId","RCount","LastAckReceived","vtA", \
"","",\
FNAME,"",\
}},
{LTE_RLC_ACK_OUTSIDE_TX_WINDOW, LOGWARNING, (RLC_TX|RLC_AM),\
"Received ACK is out of transmission window ", \
{"LineNum","ue_index","LcId","ACK","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RING_SLOT_NOT_FREE_1, LOGDEBUG,(RLC_TX|RLC_AM),\
"Status PDU NACK for segment", \
{"ue_index","LcId","NACK","Sostart","SoEnd", \
"","",\
FNAME,"",\
}},

{LTE_RLC_RING_SLOT_FREE, LOGDEBUG,(RLC_TX|RLC_AM),\
"Status PDU NACK complete", \
{"LineNum","ue_index","LcId","NACK","ACK", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODING_FAILED_FOR_CONTROL_PDU, LOGINFO, (RLC_TX|RLC_AM),\
"Handling Transmission opp for control PDU: Encoding Failed  ", \
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_ENCODING_STATUS_PDU, LOGDEBUG, (RLC_TX|RLC_AM),\
"Encoding the Status PDU ", \
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_DECODING_AM_HEADER, LOGWARNING, (RLC_RX|RLC_AM),\
"Data pointer for Received AM PDU is null ", \
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_FIX_HEADER_DECODED_1, LOGDEBUG, (RLC_RX|RLC_AM),\
"Header Elements of received PDU 1", \
{"LineNum","DC","RF","P","FI", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NO_RE_TX_PDU, LOGINFO, (RLC_RX|RLC_AM),\
"No RE-TX PDU for seqNum", \
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_FIX_HEADER_DECODED_2, LOGDEBUG, (RLC_RX|RLC_AM),\
"Header Elements of received PDU 2", \
{"LineNum","SN","E","LSF","SO", \
"","",\
FNAME,"",\
}},
{LTE_RLC_DECODING_COMPLETED, LOGDEBUG, (RLC_RX|RLC_AM),\
"Rceived PDU header Information", \
{"LineNum","DC","RF","P","FI", \
"","",\
FNAME,"",\
}},
{LTE_RLC_REASSEMBLY, LOGDEBUG, (RLC_RX|RLC_AM),\
"RLC Reassembly ", \
{"LineNum","LastSoEnd","SoStart","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_DECODING_AM_SEGMENT_HEADER, LOGDEBUG, (RLC_RX|RLC_AM),\
"Amd PDU segment header decoded ", \
{"LineNum","E","LI","PDU Header","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_FAILED_TO_POPULATE_REC_BUFFER, LOGINFO, (RLC_RX|RLC_AM),\
"Unable to populate reception buffer ", \
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_SEGMENT_PROCESS_FAILED, LOGWARNING, (RLC_RX|RLC_AM),\
"Not Able to handle Segment PDU ", \
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_PROCESS_AM_SEGMENT_PDU, LOGDEBUG, (RLC_RX|RLC_AM),\
"AM segment PDU from segment queue ", \
{"LineNum","AMSegment","SoStart","SoEnd","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_DECODING_FAILED, LOGWARNING, (RLC_RX|RLC_AM),\
"RLC Header Decoding failed: Data pointer for SDU could not be removed ", \
{"LineNum","SN","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_MSG_REMOVE_FAIL, LOGFATAL, (RLC_RX),\
"Msg Removal failure", \
{"LineNum","SN","","","", \
"","",\
FNAME,EVENT, \
}},
{LTE_RLC_SN_RECEIVED, LOGDEBUG, (RLC_RX|RLC_AM),\
"Received PDU with number of SDUs ", \
{"LineNum","SN","NumberofSDUs","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NULL_POINTER_RECEIVED_FOR_SDU, LOGFATAL, (RLC_RX|RLC_AM),\
"AM PDU Decode:Data pointer for SDU is null ", \
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_MEM_ALLOC_FAILURE, LOGERROR, L2_SYS_FAIL,\
"RLC Memory Allocation Failure ", \
{"LineNum","Size","","","", \
"","",\
FNAME,EVENT, \
}},
{LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGERROR, L2_SYS_FAIL,\
"RLC Get Mem from pool Failure ", \
{"LineNum","Size","ue_index","LcId","", \
"","",\
FNAME,EVENT, \
}},
{LTE_RLC_AM_PDU_RECEIVED_FOR_DECODING, LOGDEBUG, (RLC_RX|RLC_AM),\
"PDU Received ", \
{"LineNum","FI","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_LAST_AM_SDU_RECEIVED, LOGDEBUG, (RLC_RX|RLC_AM),\
"Last AM SDU of received PDU ", \
{"LineNum","FI","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_COUNT_DFE, LOGDEBUG, (RLC_RX|RLC_AM),\
"Received PDU have SDUs", \
{"LineNum","SN","NumberofSDUs","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_CURRUPTED_AM_PDU_RECEIVED, LOGWARNING, (RLC_RX|RLC_AM),\
"Received corrupted AMD PDU ", \
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_REQUESTED_SIZE_LESS_THAN_FIXED_HEADER, LOGWARNING, (RLC_TX|RLC_AM),\
"Data size requested by MAC is less than or equal to fixed header size ", \
{"LineNum","ue_index","LcId","TxOpp","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NO_DATA_FOR_AM_ENTITY, LOGWARNING, (RLC_TX|RLC_AM),\
"No data for RLC AM entity ", \
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SN_OUTSIDE_TX_WINDOW_FOR_ENCODING, LOGWARNING, (RLC_TX|RLC_AM),\
"SN falls out of the transmitting window ", \
{"ue_index","LcId","vtA","vtS","vtMs", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RECEIVED_PDU_FOR_ENCODING, LOGDEBUG, (RLC_RX|RLC_AM),\
"LTE_RLC_RECEIVED_PDU_FOR_ENCODING", \
{"LineNum","ue_index","LcId","Data_Size","PDUSize", \
"tIdCurrentNode","SnTidCount",\
FNAME,"",\
}},
{LTE_RLC_REQUEST_FOR_QUEUE_SIZE, LOGDEBUG, (RLC_RX|RLC_AM),\
"LTE_RLC_REQUEST_FOR_QUEUE_SIZE", \
{"LineNum","ue_index","LcId","AM SDU Size","QSize", \
"","",\
FNAME,"",\
}},
{LTE_RLC_ENCODING_COMPLETE_FOR_FRESH_PDU_1, LOGDEBUG, (RLC_TX|RLC_AM),\
"After Encoding Fresh PDU Header fields 1:", \
{"ue_index","LcId","DC","RF","P", \
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODING_COMPLETE_FOR_FRESH_PDU_2, LOGDEBUG, (RLC_TX|RLC_AM),\
"After Encoding Fresh PDU Header fields 2:", \
{"LineNum","FI","E","SN","PduFormedSize", \
"","",\
FNAME,"",\
}},

{LTE_RLC_TX_STATE_VARS, LOGDEBUG, (RLC_TX|RLC_AM),\
"TX PDU State vars:", \
{"LineNum","ue_index","LcId","vtA","vtS", \
"vtMS","",\
FNAME,EVENT, \
}},
{LTE_RLC_ENCODE_RE_TX_PDU, LOGDEBUG, (RLC_AM|RLC_TX),\
"PDU Retransmitted ", \
{"LineNum","ue_index","LcId","SN","Pdu_Size", \
"","",\
FNAME,"",\
}},

{LTE_RLC_MAX_RE_TX_IND_PDCP, LOGWARNING,(RLC_TX|RLC_AM),\
"RLC unable to retransmit.Sending MaxReTx Threshold Ind to PDCP", \
{"LineNum","ue_index","LcId","SN","Retransmission count", \
"","",\
FNAME,"",\
}},
{LTE_RLC_MAX_RE_TX_IND_RRC, LOGWARNING,(RLC_TX|RLC_AM),\
"RLC unable to retransmit AMD PDU.Sending MaxReTx Threshold Ind to PDCP", \
{"LineNum","ue_index","LcId","SN","Retransmission count", \
"","",\
FNAME,"",\
}},
{LTE_RLC_MAX_RE_TX_IND, LOGDEBUG,(RLC_TX|RLC_AM),\
"Sending Max ReTxThreshold Indication for retransmission of  AMD PDU",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_TID_INFO_NOT_PRESENT, LOGWARNING, (RLC_TX|RLC_AM),\
"Sdu transmission id info must present",\
{"LineNum","ue_index","LcId","Queue_Count","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NO_RETRANSMISSION, LOGDEBUG, (RLC_TX|RLC_AM),\
"No PDU for retransmission",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_GET_QUEUEL_LOAD, LOGDEBUG,(RLC_TX|RLC_AM),\
"After calculating variable size header",\
{"LineNum","ue_index","LcId","QLoad","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RECEIVED_MAX_SDU, LOGWARNING,(RLC_TX|RLC_AM),\
"Maximum number of SDU Received:RLC Queue full",\
{"LineNum","ue_index","LcId","AmSDUCount","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RECEIVE_AM_SDU, LOGDEBUG,(RLC_TX|RLC_AM),\
"Received AM SDU From PDCP",\
{"LineNum","ue_index","LcId","QSize","PDCPCount", \
"","",\
FNAME,"",\
}},
{LTE_RLC_FETCH_QUEUE_LOAD, LOGDEBUG,(RLC_TX|RLC_AM),\
" ",\
{"LineNum","ue_index","LcId","QSize","ReTxQSize", \
"RxStatusPduSize","TxQSize",\
FNAME,"",\
}},
{LTE_RLC_GET_PDU_TYPE, LOGDEBUG, (RLC_TX|RLC_AM),\
"Getting transmission type of the PDU",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_DISCARD_AM_SDU, LOGDEBUG,(RLC_TX|RLC_AM),\
"Received AM SDU discard From PDCP",\
{"LineNum","ue_index","LcId","Trans_Id","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_STATUS_PDU_ENCODING, LOGDEBUG, (RLC_TX|RLC_AM),\
"Encoding the Status PDU for transmission opportunity  ",\
{"LineNum","ue_index","LcId","ACKSN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NEW_AM_PDU_ENCODING, LOGDEBUG, (RLC_TX|RLC_AM),\
"Encoding the New AM PDU ",\
{"LineNum","ue_index","LcId","SN","RecvOpportunity", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NEW_AM_PDU_ENCODING_COMPLETE, LOGDEBUG, (RLC_TX|RLC_AM),\
"After Encoding New AM PDU ",\
{"ue_index","LcId","SN","TxSize","RemainingDataSize", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NEW_AM_PDU_ENCODING_FAILED, LOGWARNING, (RLC_TX|RLC_AM),\
"Encoding of new AMD PDU Failed, ",\
{"LineNum","ue_index","LcId","QSize","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RE_TX_AM_PDU_ENCODING, LOGDEBUG,(RLC_TX|RLC_AM),\
"Encoding the retransmission AM PDU",\
{"LineNum","ue_index","LcId","RequestedSize","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RE_TX_AM_PDU_ENCODING_COMPLETED, LOGDEBUG, (RLC_TX|RLC_AM),\
"Encoding of retransmission AM PDU completed ",\
{"LineNum","ue_index","LcId","SN","PduSize", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RE_TX_AM_PDU_ENCODING_FAILED, LOGWARNING, (RLC_TX|RLC_AM),\
"Encoding of retransmission AM PDU Failed ",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_POLL_TIMER_START, LOGDEBUG, (RLC_TX|RLC_AM),\
"POLL Timer started",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_STATUS_PROHIBIT_TIMER_START, LOGDEBUG, (RLC_TX|RLC_AM),\
"StatusProhibit Timer Started ",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_REMAINING_QUEUE_LOAD, LOGDEBUG, (RLC_TX|RLC_AM),\
"Remaining Queue Load at RLC",\
{"LineNum","ue_index","LcId","RemainingQLoad","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_ENCODING_FAILED, LOGWARNING, (RLC_TX|RLC_AM),\
"Encoding Failed send zero indication to MAC",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_FIXED_HEADER_DECODING_FAILED, LOGWARNING, (RLC_RX|RLC_AM),\
"Decoding of AM PDU Fixed header failed",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_FIXED_HEADER_DECODING_COMPLETE, LOGDEBUG, (RLC_RX|RLC_AM),\
"Header decoded Successfully",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_CONTROL_PDU_MEM_ALLOC_FAILED, LOGERROR, L2_SYS_FAIL,\
"Memory allocation fails for RLC ControlPdu_p",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_ENQUEUE_STATUS_PDU, LOGDEBUG, (RLC_RX|RLC_AM),\
"Status PDU received and Enqueue in control queue ",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_PDU_RECEIVED_FROM_MAC,LOGDEBUG,(RLC_RX|RLC_AM),\
"Handle AM PDU received From MAC",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_PDU_DISCARDED, LOGWARNING, (RLC_RX|RLC_AM),\
"Recieved AM PDU discarded",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_MAC_OPPORTUNITY,LOGDEBUG,(RLC_TX|RLC_UM),\
"MAC Provided opportunity to RLC ",\
{"LineNum","ue_index","LcId","MACOpportunity","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_MSG_JOIN_FAIL, LOGFATAL, L2_SYS_FAIL,\
"Msg Join Fail",\
{"LineNum","ue_index","LcId","Size of 1st arg","Size of 2nd arg", \
"","",\
FNAME,EVENT, \
}},
{LTE_RLC_RECEIVED_DUPLICATE_PKT, LOGWARNING, (RLC_RX|RLC_AM),\
"Duplicate Pkt recvd,Discarded",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SN_RECEIVED_FIRST_TIME, LOGDEBUG, (RLC_RX|RLC_AM),\
"PDU received, store in reception buffer",\
{"ue_index","LcId","SN","vrR","vrMR", \
"","",\
FNAME,"",\
}},
{LTE_RLC_POLL_TIMER_RESTART,LOGINFO, (RLC_TX|RLC_AM),\
"POLL Timer ReStarted",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_PDU_DECODING_FAILED, LOGWARNING, (RLC_RX|RLC_AM),\
"Decoding failed for AM PDU",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_POLL_BIT_RECEIVED, LOGDEBUG, (RLC_RX|RLC_AM),\
"Received PDU with Poll bit set",\
{"SN","vrMS","vrR","vrMR","P" \
"","",\
FNAME,"",\
}},
{LTE_RLC_MSG_EXTRACT_FAIL, LOGFATAL, (RLC_RX | RLC_AM),\
"Msg Extract Fail",\
{"LineNum","Size","ue_index","LcId","", \
"","",\
FNAME,EVENT, \
}},
/*{LTE_RLC_STATUS_PROHIBIT_TIMER_START, LOGDEBUG, (RLC_TX|RLC_AM),\
"Status Prohibit Timer Started",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},*/
{LTE_RLC_RING_SLOT_NOT_FREE, LOGERROR, (RLC_RX|RLC_AM),\
"Ring Slot not free while placing SN in recp buffer",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_PDU_IN_RECEPTION_BUFFER, LOGDEBUG, (RLC_RX|RLC_AM),\
"Action on Received PDU,In Reception Buffer",\
{"ue_index","LcId","SN","vrMR_p","vrMR",\
"","",\
FNAME,"",\
}},
{LTE_RLC_RING_SLOT_IN_USE, LOGDEBUG, (RLC_RX|RLC_AM),\
"Ring status in use",\
{"LineNum","ue_index","LcId","SN","RCount", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UPDATING_VRMS, LOGDEBUG, (RLC_RX|RLC_AM),\
"In Reception Buffer : Before updating vrMS",\
{"LineNum","ue_index","LcId","RCount","vrH", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UPDATED_VRMS, LOGDEBUG, (RLC_RX|RLC_AM),\
"In Reception Buffer:",\
{"LineNum","ue_index","LcId","vrMS","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UPDATED_STATE_VARS, LOGINFO, (RLC_RX|RLC_AM),\
"In Reception Buffer: Updated state variables",\
{"LineNum","ue_index","LcId","vrR","vrMR", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SN_LIST_FORMATION, LOGDEBUG, (RLC_RX|RLC_AM),\
"In Reception Buffer: Forming SN List",\
{"LineNum","ue_index","LcId","RCount","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UPDATED_RCOUNT, LOGDEBUG, (RLC_RX|RLC_AM),\
"In Reception Buffer: Updated rCount for PDU_AM_COMPLETE",\
{"LineNum","ue_index","LcId","RCount","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NO_REVERSE_SN_LIST_FOUND, LOGDEBUG, (RLC_RX|RLC_AM),\
"In Reception Buffer: No reverse SN list",\
{"ue_index","LcId","RCount","vrR","vrMR", \
"","",\
FNAME,"",\
}},
{LTE_RLC_BEFORE_REASSM_RLC_STATE_VARS_1, LOGINFO, (RLC_RX|RLC_AM),\
"Before Reassembly:RLC State Variables 1:",\
{"LineNum","ue_index","LcId","SN","vrR", \
"","",\
FNAME,"",\
}},
{LTE_RLC_BEFORE_REASSM_RLC_STATE_VARS_2, LOGINFO, (RLC_RX|RLC_AM),\
"Before Reassembly:RLC State Variables 2:",\
{"LineNum","vrMR","vrX","vrMS","vrH", \
"","",\
FNAME,"",\
}},
{LTE_RLC_TRIGGERING_STATUS_REPORT, LOGDEBUG, (RLC_RX|RLC_AM),\
"Delay triggering STATUS report until x<VR(MS) or x>=VR(MR)",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_REORDERING_TIMER_RUNNING, LOGDEBUG, (RLC_RX|RLC_AM),\
"RLC AM reOrdering Timer is running",\
{"LineNum","ue_index","LcId","vrX","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_TIMER_STOP, LOGDEBUG, (RLC_RX|RLC_AM),\
"Timer Stop for RLC AM",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_TIMER_START, LOGDEBUG, (RLC_RX|RLC_AM),\
"VRH greater than VRR, Timer Start",\
{"ue_index","LcId","SN","vrX","RxAMEntity->vrX",\
"","",\
FNAME,"",\
}},
/** SPR 1753 Changes Start **/
{LTE_RLC_MULTI_DATA_QUEUE,LOGWARNING,(RLC_RX),\
"RLC Multi Buffer Queue is empty",\
{"LineNum","QSize","Multi_Buff_Flag","","", \
"","",\
FNAME,"", \
}},
{LTE_RLC_MULTI_BUFF_RECV,LOGDEBUG, (RLC_RX|RLC_AM),\
"Multi Buffer Receive From Lowr Layer",\
{"LineNum","ue_index","LcId","BuffCount","MultiBuffFlag", \
"","",\
FNAME,"", \
}},
/** SPR 1753 Chanegs End **/
{LTE_RLC_REORDERING_TIMER_EXPIRE, LOGINFO, (RLC_RX|RLC_AM),\
"t-Reordering Timer Expired",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
/** RLC Rel 3.0 Changes End **/
{LTE_RLC_DELIVER_SDU_TO_PDCP, LOGDEBUG, (RLC_RX|RLC_AM),\
"RLC Delivered SDUs to PDCP",\
{"LineNum","ue_index","LcId","Size","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_DELIVERING_AM_SDU_TO_QUEUE, LOGDEBUG, (RLC_RX|RLC_AM),\
"ReAssembling, concatenating & delivering AM SDU to Queue",\
{"LineNum","ue_index","LcId","Size","SN", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SN_PORTION_RESERVED, LOGDEBUG, (RLC_RX|RLC_AM),\
"ReAssembly: Incomplete SDU of PDU is reserved for next reassembly",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SN_PORTION_DISCARDED, LOGWARNING, (RLC_RX|RLC_AM),\
"ReAssembly: FULL SDU received discard last portion of PDU",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_DELIVER_AM_SDU_TO_PDCP, LOGDEBUG, (RLC_RX|RLC_AM),\
"ReAssembly: Delivered AM SDU to PDCP",\
{"ue_index","LcId","Status","SN","Size", \
"","",\
FNAME,EVENT,\
}},
{LTE_RLC_SDU_START_PORTION_DISCARDED, LOGWARNING, (RLC_RX|RLC_AM),\
"ReAssembly: Corrupted PDU was received,START Portion of SDU discarded",\
{"LineNum","ue_index","LcId","Status","SN", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SDU_PORTION_DISCARDED, LOGWARNING, (RLC_RX|RLC_AM),\
"ReAssembly: Portion of PDU discarded",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RECEIVED_CURRUPTED_AM_PDU, LOGWARNING, (RLC_RX|RLC_AM),\
"ReAssembly: Corrupted AM PDU Received",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_STATUS_REPORT_DELAY, LOGDEBUG, (RLC_RX|RLC_AM),\
"Status Prohibit Timer is running so delayed to trigger status report",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_TRIGGER_STATUS_PDU, LOGDEBUG, (RLC_RX|RLC_AM),\
"Trigger the status PDU",\
{"LineNum","ue_index","LcId","StatusPduLen","ACK", \
"","",\
FNAME,"",\
}},
{LTE_RLC_MEMORY_ALLOC_FAILURE, LOGERROR, L2_SYS_FAIL,\
"Trigger status report: Memory allocation fails for rlcControlPdu_p",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_STATUS_PROHIBIT_TIMER_EXPIRE, LOGINFO, (RLC_RX|RLC_AM),\
"Expiring Status Prohibit Timer reset the respective flags",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_DECODING_COMPLETE, LOGDEBUG, (RLC_RX|RLC_UM),\
"After Decoding, UM PDU header elements:",\
{"LineNum","FI","SN","E","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NULL_DATA_POINTER_RECEIVED, LOGWARNING, (RLC_RX|RLC_UM),\
"Data pointer for UM RawPdu received to decode header is null",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_PARTIALLY_DECODING_FAILED, LOGWARNING, (RLC_RX|RLC_UM),\
"Partial decoding failed for UM RawPdu",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
/** SPR 2444 Changes Start **/
{LTE_RLC_UM_PDU_RECEIVED_FOR_DECODING, LOGDEBUG, (RLC_RX|RLC_UM),\
"First SDU of received PDU ",\
{"LineNum","SduStatus","","","",  /* fi not present in one log*/ \
"","",\
FNAME,"",\
}},
{LTE_RLC_LAST_UM_SDU_RECEIVED, LOGDEBUG, (RLC_RX|RLC_UM),\
"Last UM SDU of received PDU ",\
{"LineNum","SduStatus","","","", \
"","",\
FNAME,"",\
}},
/** SPR 2444 Changes End **/
{LTE_RLC_CURRUPTED_UM_PDU_RECEIVED, LOGWARNING, (RLC_RX|RLC_UM),\
"Extracting next SDU(s) & data field of UMD PDU :Received corrupted",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_MEMORY_ALLOCATION_FAILURE, LOGERROR, L2_SYS_FAIL,\
"Memory Allocation Failure",\
{"LineNum","","","","", \
"","",\
FNAME,EVENT, \
}},
{LTE_RLC_MEMORY_REMOVAL_FAILURE, LOGERROR, L2_SYS_FAIL,\
"Memory remove Failure ",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_PDU_ENCODING_1, LOGDEBUG, (RLC_TX|RLC_UM),\
"Encoding UM PDU 1",\
{"FI","E","SnLen","SN","SDUCount", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_PDU_ENCODING_2, LOGDEBUG, (RLC_TX|RLC_UM),\
"Encoding UM PDU 2",\
{"SegmentLength","FixHeaderSize","DataSize","PduSize","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_MEM_ALLOCATION_FAILURE, LOGERROR, L2_SYS_FAIL,\
"RLC Mem Allocation failure",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_SDU_RECEIVED, LOGDEBUG, (RLC_TX|RLC_UM),\
"Received UM SDU From PDCP",\
{"LineNum","ue_index","LcId","QSize","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_SDU_DISCARDED_PDCP, LOGWARNING, (RLC_TX|RLC_UM),\
"Received UM SDU discard From PDCP",\
{"LineNum","ue_index","LcId","Trans_Id","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_PDU_DISCARDED, LOGWARNING, (RLC_RX|RLC_UM),\
"UM PDU discarded received from Lower layer",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_HEADER_DECODING_FAILED, LOGWARNING, (RLC_RX|RLC_UM),\
"UM PDU Header partial decoding failed",\
{"LineNum","ue_index","LcId","UR","UH", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_PDU_DECODING_FAILED, LOGWARNING, (RLC_RX|RLC_UM),\
"Complete decoding failed for UM PDU",\
{"LineNum","ue_index","LcId","SN","UR","UH" \
"","",\
FNAME,"",\
}},
{LTE_RLC_SN_OUTSIDE_REORDERING_WINDOW, LOGDEBUG, (RLC_RX|RLC_UM),\
"SN falls outside of the reordering window: RLC UM State Variables",\
{"ue_index","LcId","SN","UR","UH" \
"","",\
FNAME,"",\
}},
{LTE_RLC_OUT_OF_ORDER_UM_PDU_RECEIVED, LOGDEBUG, (RLC_RX|RLC_UM),\
"Reception buffer contains an out of order UMD PDU ",\
{"ue_index","LcId","SN","UR","UH" \
"","",\
FNAME,"",\
}},
{LTE_RLC_REORDERING_TIMER_STOP, LOGDEBUG, (RLC_RX|RLC_UM),\
"Re-ordering Timer Stopped, RLC UM State Variables",\
{"ue_index","LcId","SN","UR","UH",\
"","",\
FNAME,"",\
}},
/** RLC Rel 3.0 Changes Start **/
/** RLC Rel 3.0 Changes End **/
{LTE_RLC_REORDERING_TIMER_START, LOGDEBUG, (RLC_RX|RLC_UM),\
"UH greater than UR, Reordering Timer Start",\
{"ue_index","LcId","UR","UH","UX", \
"","",\
FNAME,"",\
}},
{LTE_RLC_NULL_RING_POINTER_RECEIVED, LOGERROR, (RLC_RX|RLC_UM),\
"Ring Pointer is NULL",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_TIMER_EXPIRE, LOGINFO, (RLC_RX|RLC_UM),\
"UM Timer Expire at State variables",\
{"ue_index","LcId","UR","UH","UX" \
"","",\
FNAME,"",\
}},
{LTE_RLC_MOVE_UM_SDU_DIRECTLY, LOGDEBUG, (RLC_RX|RLC_UM),\
"Enqueueing UM Raw SDU",\
{"LineNum","ue_index","LcId","SDU_SIZE","SN", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_SDU_PORTION_RESERVED, LOGDEBUG, (RLC_RX|RLC_UM),\
"Incomplete SDU of current PDU is reserved for next reassembly",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_SDU_FULL_PORTION_DISCARDED, LOGWARNING, (RLC_RX|RLC_UM),\
"Complete SDU of current PDU discarded",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_SDU_START_PORTION_DISCARDED, LOGWARNING, (RLC_RX|RLC_UM),\
"First SDU of PDU discarded",\
{"LineNum","ue_index","LcId","SN","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_CHANNEL_TYPE, LOGWARNING, RLC_MAC,\
"Invalid Common Channel Type or RLC Layer not initialized ",\
{"LineNum","ComChType","RlcLayerInitialized_g","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_TM_TX_ENTITY_NOT_PRESENT, LOGWARNING, RLC_MAC,\
"TM TX Entity not present",\
{"LineNum","Rnti","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_TM_RX_ENTITY_NOT_PRESENT, LOGWARNING, RLC_MAC,\
"TM RX Entity not present",\
{"LineNum","Rnti","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SENDING_HARQ_INDICATION, LOGINFO, RLC_MAC,\
"MAC give Harq Failure Indication to RLC",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_OAM_SOCKET_CREATION_FAILED, LOGFATAL, RLC_RRC,\
"OAM TX socket creation failed",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_BUILD_CONFIGURATION, LOGWARNING, RLC_OAM,\
"Product Build Version File Not Found",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_THREAD_INTERFACE_CLEANUP, LOGWARNING, RLC_OAM,\
"RLC thread interface cleanup: RLC layer not initialized",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_LOGFILE_CREATION, LOGFATAL, RLC_OAM,\
"Unable to open file for RLC log details",\
{"LineNum","","","","", \
"","",\
FNAME,"FileName", \
}},
{LTE_RLC_RECEIVE_MSG_FROM_SOCKET, LOGFATAL, RLC_RRC,\
"Error while receiving message on socket",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RRC_INTERFACE_THREAD_INIT_FAILED, LOGWARNING, RLC_RRC,\
"Unable to initialize RRC Interface thread",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_REESTAB_AM_RX_ENTITY_2,LOGINFO, RLC_RRC,\
"AM-RX Re-Establishment_2:",\
{"vrX","vrMS","isRTimerRunning","isSPTimerRunning","" \
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_DEST_MODULE_ID, LOGWARNING, RLC_RRC,\
"Invalid destination module id",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
/** RLC Rel 3.0 Changes Start **/
{LTE_RLC_REESTAB_AM_TX_ENTITY_1, LOGINFO, (RLC_RRC),\
"AM-TX Re-Establishment_1:",\
{"ue_index","LcId","TxQSize","ReTxQSize","vtA" \
"","",\
FNAME,"", \
}},
{LTE_RLC_REESTAB_AM_TX_ENTITY_2, LOGINFO, (RLC_RRC),\
"AM-TX Re-Establishment_2:",\
{"PduWithoutPoll","istRetransmitTimerRunning","qLoad","StatusReportAck","vtMS" \
"","",\
FNAME,"", \
}},
{LTE_RLC_REESTAB_INVALID_REQUEST, LOGINFO, (RLC_RRC),\
"Invalid LcId received for reEstablishment",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
/** RLC Rel 3.0 Changes End **/
/* Change for SPR 2203 End */
{LTE_RLC_MEM_ALLOC_FAILED_RRC_DATA_REQ, LOGERROR, L2_SYS_FAIL,\
"Memory allocation failure",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RECV_DATA_PKT,LOGDEBUG, (RLC_UM|RLC_RX),\
"RLC Receive UM Data Pkt:",\
{"ue_index","LcId","VRUR","VRUH","VRUX", \
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_UEIDX_LCID, LOGWARNING, RLC_PDCP,\
"Invalid UeId or lcId or UninitRLCLayer",\
{"LineNum","ue_index","LcId","RlcLayerInitialized_g","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_MEM_ALLOC_FAILED_MAC_UE_DATA_IND, LOGERROR, L2_SYS_FAIL,\
"Memory allocation fails for MAC UE Data Indication",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RRC_MSG_PARSING_FAILED, LOGWARNING, RLC_RRC,\
"Bytes read are not equal to msgLen to parse RRC massage",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_MSG_DROPPED, LOGFATAL, RLC_RRC,\
"Incorrect MsgId received to parse RRC message, dropping message",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_UE_CONTEXT, LOGWARNING, RLC_RRC,\
"Invalid UE context",\
{"LineNum","ue_index","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_UE_INDEX, LOGWARNING, RLC_RRC,\
"Invalid UE Index",\
{"LineNum","ue_index","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_CREATE_UE_REQUEST, LOGDEBUG, RLC_RRC,\
"RLC CREATE UE REQ recieved",\
{"LineNum","MsgId","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_DELETE_UE_REQUEST, LOGDEBUG, RLC_RRC,\
"RLC DELETE UE REQ recieved",\
{"LineNum","MsgId","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_RECONFIG_UE_REQUEST, LOGDEBUG, RLC_RRC,\
"RLC RECONFIG UE REQ received",\
{"LineNum","MsgId","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_REESTAB_UE_REQUEST, LOGDEBUG, RLC_RRC,\
"RLC REESTAB UE REQ recieved ",\
{"LineNum","MsgId","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_COMM_CH_DELETION_FAILED, LOGWARNING, RLC_RRC,\
" Failed to delete the common channel entity",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_COMM_CH_REESTAB_FAILED, LOGWARNING, RLC_RRC,\
"Failed to re-establish the common channel entity",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_SN_LENGTH, LOGWARNING, RLC_RRC,\
"Invalid SN field length of UM entity",\
{"LineNum","ue_index","LcId","SnSize","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_POLL_BYTE, LOGWARNING, RLC_RRC,\
"Invalide pollByte value",\
{"LineNum","ue_index","LcId","poll_byte","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_POLL_PDU, LOGWARNING, RLC_RRC,\
"Invalide pollPDU value",\
{"LineNum","ue_index","LcId","poll_pdu","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_LCID_OUT_OF_RANGE, LOGWARNING, RLC_RRC,\
"LCID is outOfRange",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_LAYER_NOT_INITIALIZED, LOGWARNING, RLC_OAM,\
"RLC Layer not initialized",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_MEMORY_ALLOCATION_FAILED, LOGERROR, L2_SYS_FAIL,\
"RLC Memory Allocation failure",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_INIT_DB, LOGDEBUG,(RLC_RX),\
"UE context DB initialized",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_DE_INIT_DB, LOGDEBUG,(RLC_RX),\
"UE context DB de-initialized",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_UEINDEX_UECONTEXT_MATCH, LOGWARNING,(RLC_RX),\
"UE context does not exist",\
{"LineNum","ue_index","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_ENQUEUE_RX_TIMER_EXPIRY, LOGDEBUG,(RLC_RX),\
"Enqueue in RX TIMER Expiry Queue",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_ENQUEUE_TX_TIMER_EXPIRY, LOGDEBUG,(RLC_RX),\
"Enqueue in TX TIMER Expiry Queue",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_DATA_PKT_DROP,LOGDEBUG, (RLC_RX|RLC_AM),\
"RLC data pck dropped",\
{"LineNum","ue_index","LcId","SN","DataPduDropCount",\
"","",\
FNAME,"",\
}},
{LTE_RLC_INIT_RX_THREAD_FAILED, LOGFATAL,(RLC_RX),\
"Unable to initialize the Rx thread",\
{"LineNum","","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_ENTITY_INVALID, LOGWARNING,(RLC_RX),\
"Invalid RLC Entity ",\
{"LineNum","ue_index","LcId","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_GET_RING_LOAD, LOGFATAL,(RLC_RX),\
"Number of Active Ring",\
{"LineNum","ue_index","LcId","ActiveRing","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_SDU_RECEIVED_RRC, LOGDEBUG, (RLC_TX|RLC_TM),\
"Receiving SDU From RRC ",\
{"LineNum","ComChType","QSize","RlcEntityState","", \
"","",\
FNAME,"",\
}}, 
{LTE_RLC_PDU_RECEIVED_MAC, LOGDEBUG, (RLC_TX|RLC_TM),\
"Receiving PDU From MAC",\
{"LineNum","ComChType","","","", \
"","",\
FNAME,"",\
}},
{LTE_RLC_UECONTEXT_NOT_EXIST, LOGWARNING,(RLC_TX),\
"UE Context not exist ",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_ENTITY_NOT_EXIST, LOGWARNING,(RLC_TX),\
"No RLC Entity exist",\
{"LineNum","ue_index","LcId","Entity state","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_TX_ENTITY_NOT_EXIST, LOGWARNING,(RLC_TX|RLC_UM),\
"No TX UM RLC Entity exist",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_SDU_DISCARDED, LOGDEBUG,(RLC_TX|RLC_UM),\
"PDCP->RLC:Received UM SDU discarded",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_TX_ENTITY_NOT_EXIST, LOGWARNING,(RLC_TX|RLC_AM),\
"No TX AM RLC Entity exist",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_SDU_DISCARDED, LOGDEBUG,(RLC_TX|RLC_AM),\
"Received AM SDU discarded",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_QUEUE_LOAD, LOGDEBUG,(RLC_TX|RLC_UM),\
"RLC Queue Load",\
{"LineNum","ue_index","LcId","QLoad","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_UEINDEX_LCID, LOGWARNING,(RLC_TX|RLC_AM),\
"UE Context not found",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_RETX_QUEUE_LOAD, LOGDEBUG,(RLC_TX|RLC_AM),\
"RLC Retransmission Queue Load",\
{"LineNum","ue_index","LcId","ReTxQSize","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_STATUS_PDU_SIZE, LOGDEBUG,(RLC_RX|RLC_AM),\
"RLC Status PDU size",\
{"LineNum","ue_index","LcId","Size","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HARQ_FAILURE_IND, LOGDEBUG,(RLC_TX|RLC_AM),\
"Harq Failure Info 1:",\
{"ue_index","LcId","SN","Sostart","Soend",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HARQ_FAILURE_IND_2, LOGDEBUG,(RLC_TX|RLC_AM),\
"Harq Failure Info 2:",\
{"LineNum","ue_index","LcId","Sostart","Soend",\
"","",\
FNAME,"",\
}},
{LTE_RLC_WRONG_HARQ_FAILURE_REPORTING, LOGERROR,(RLC_TX|RLC_AM),\
"Wrong Reporting of Harq Failure by MAC",\
{"LineNum","ue_index","LcId","SN","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HARQ_IND, LOGDEBUG,(RLC_TX|RLC_AM),\
"RLC Handle HARQ Failure Indication by MAC",\
{"LineNum","ue_index","LcId","SN","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_STATUS_PDU_DROP,LOGDEBUG,(RLC_RX|RLC_AM),\
"RLC Status PDU drop",\
{"LineNum","ue_index","LcId","StatusPduDropCount","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_WRONG_LCID_FOR_TX_OPP, LOGWARNING,(RLC_TX|RLC_AM),\
"Wrong lcId ",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_UNABLE_TO_HANDLE_TX_OPP, LOGWARNING,(RLC_TX|RLC_AM),\
"Unable to perform TX opp in same Tick  ",\
{"LineNum","ReqTick","CurTick","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_STATUS_REPORT_RECEIVED, LOGDEBUG,(RLC_TX|RLC_AM),\
"Action on Received Status Report",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_TRANSMIT_STATUS_REPORT, LOGDEBUG,(RLC_TX|RLC_AM),\
"Transmitting Status-Report",\
{"LineNum","ue_index","LcId","QLoad","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_PKT_DROP_WRONG_RANGE_FROM_OAM, LOGWARNING, (RLC_RX|RLC_AM),\
"Wrong range received from oam for Packet drop",\
{"LineNum","ue_index","LcId","StartSN","EndSN",\
"","",\
FNAME,"",\
}},
{LTE_RLC_QLOAD_NEW,LOGDEBUG,(RLC_TX|RLC_AM),\
"New queue load",\
{"LineNum","ue_index","LcId","RequestedDataSize","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_RECV_STATUS_REPORT, LOGDEBUG, (RLC_TX|RLC_AM),\
"Received Status Report",\
{"LineNum","ue_index","LcId","ACKSN","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_RECV_NACK_SN, LOGDEBUG, (RLC_TX|RLC_AM),\
"Received NACK SN",\
{"LineNum","ue_index","LcId","NACK","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_ENCODE_STATUS_PDU, LOGERROR, (RLC_TX|RLC_AM),\
"Encoding of STATUS PDU Failed",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_ENCODE_RETX_PDU, LOGERROR, (RLC_TX|RLC_AM),\
"Encoding of RETX PDU Failed",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},
/*SPR 15251 Fix*/
{LTE_RLC_ENCODE_NEW_PDU, LOGWARNING, (RLC_TX|RLC_AM),\
/*SPR 15251 Fix*/
"Encoding of NEW PDU Failed",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_TX_OPP, LOGERROR, RLC_TX,\
"ueOppInd_p is null",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_UM_TX_OPP, LOGERROR, RLC_TX,\
"UM encoding ",\
{"LineNum","ue_index","LcId","QSize","PduSize",\
"","",\
FNAME,"",\
}},
{LTE_RLC_AM_TX_OPP, LOGERROR, RLC_TX,\
"AM encoding",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change start */
{LTE_RLC_INVALID_ENTITY_ERR_1, LOGWARNING, RLC_RRC,\
"RLC_INVALID_ENTITY_ERR_1",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_INVALID_ENTITY_ERR, LOGWARNING, RLC_RRC,\
"RLC_INVALID_ENTITY_ERR",\
{"LineNum","LcId","RetVal","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_INVALID_ENTITY_ERR_2, LOGWARNING, RLC_RRC,\
"RLC_INVALID_ENTITY_ERR_2",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change End */
{LTE_RLC_INVALID_ENTITY_ERR_3, LOGWARNING, RLC_RRC,\
"Invalid entity type rlcReconfigUEEntity",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_INVALID_UECONTEXT_3, LOGWARNING, RLC_RRC,\
"Invalid UE Context.lcId does not exists",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_INVALID_ENTITY_TYPE, LOGWARNING, RLC_RRC,\
"Invalid UE Context in rlcDeleteUEEntity",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENTITY_IN_PENDING_DEL_QUEUE_1, LOGINFO, RLC_RRC,\
"RLC Entity is in deletion Queue",\
{"LineNum","ue_index","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DEL_UE_CONTEXT, LOGINFO, RLC_RRC,\
"RLC UE Context is deleted",\
{"LineNum","ue_index","Trans_Id","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DEL_UE_ENTITY, LOGINFO, RLC_RRC,\
"RLC UE entity is deleted",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DEL_UE_CONTEXT_ERR, LOGWARNING, RLC_RRC,\
"Error in permanent deletion of UE Context",\
{"LineNum","ue_index","Trans_Id","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DEL_UE_ENTITY_ERR, LOGWARNING, RLC_RRC,\
"Error in permanent deletion of UE Entity",\
{"LineNum","ue_index","Trans_Id","LcId","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DEL_UE_RX_ENTITY, LOGINFO, RLC_RRC,\
"RLC UE RX entity is deleted",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},


{LTE_RLC_DEL_UE_TX_ENTITY, LOGINFO, RLC_RRC,\
"RLC UE TX entity is deleted",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DEL_UE_COMMON_CH, LOGINFO, RLC_RRC,\
"RLC UE Common Channal is deleted",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_REST_UE_ENTITY_ERR, LOGINFO, RLC_RRC,\
"RLC context does not exist",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_REST_UE_ENTITY_ERR_1, LOGINFO, RLC_RRC,\
"REST_UE_ENTITY RLC Entity does not exist",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_REST_UE_ENTITY_ERR_2, LOGINFO, RLC_RRC,\
"REST_UE_ENTITY: Invalid entity mode",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_REST_UE_ENTITY_ERR_3, LOGINFO, RLC_RRC,\
"UE Entity Re-established",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_REST_UE_ENTITY_ERR_4, LOGINFO, RLC_RRC,\
"UE Common CH Entity Re-established",\
{"LineNum","ue_index","LcId","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_DATA_FROM_PDCP_ERR_1, LOGWARNING, RLC_TX,\
"Could not get RLC Entity",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_RECV_SDU_ERR, LOGWARNING, RLC_TX,\
"RECV_SDU: maximum number of SDU received",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_DATA_FROM_MAC_ERR_3, LOGWARNING, (RLC_RX),\
"Maximum number of SDU received",\
{"LineNum","ue_index","lcCount","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_IS_VALID_RX_UM_PDU, LOGINFO, (RLC_RX),\
"check for PDU accept/discard",\
{"LineNum","SN","LcId","ue_index","SnStatus",\
"","",\
FNAME,"",\
}},

{LTE_RLC_VALIDATE_AND_GET_ENTITY, LOGDEBUG, (RLC_RX),\
"Fetch RLC Entity from Q",\
{"LineNum","Rnti","LcId","PduSize","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HANDLE_DATA_FROM_MAC, LOGDEBUG, (RLC_RX),\
"Fetch RLC UE context from Q",\
{"LineNum","ue_index","lcCount","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_UMD_HEADER, LOGINFO, (RLC_RX),\
"DECODE_UMD_HEADER: RLC Header decoding",\
{"SnLen","Size","FI","E","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_UMD_RAW_PDU_ERR_5, LOGINFO, (RLC_RX),\
"DECODE_UMD_RAW_PDU: RLC Header decoding",\
{"LineNum","SN","HIdx","PduSize","RetVal",\
"","",\
FNAME,"",\
}},

{ LTE_RLC_PARSE_UE_ENTITY_INFO_1, LOGDEBUG, (RLC_OAM),\
"Valid TAG",\
{"LineNum","Tag","TagLen","MsgLen","RetVal",\
"","",\
FNAME,"",\
}},


/***********************AM new Logs**************/
{LTE_RLC_TX_OPP_5, LOGWARNING, RLC_TX,\
"TX_OPP: LC Entity is not Active or exists",\
{"LineNum","ue_index","LcId","TxOpp","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_RETX_PDU_INFO, LOGDEBUG, (RLC_TX|RLC_AM),\
"PDU retransmission",\
{"LineNum","ue_index","LcId","SN","ReTxCount",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_RETX_PDU_ERR, LOGWARNING, (RLC_TX|RLC_AM),\
"Requested Data Size <= fixed size(2)",\
{"LineNum","Rnti","LcId","ReqDataSize","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_RETX_PDU_ERR_1, LOGWARNING, (RLC_TX|RLC_AM),\
"ENCODE_RETX_PDU Invalid ReTx PDU",\
{"LineNum","Rnti","LcId","SN","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_RETX_PDU_ERR_2, LOGWARNING, (RLC_TX|RLC_AM),\
"ENCODE_RETX_PDU msgAlloc Failed!",\
{"LineNum","Rnti","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_RETX_PDU_ERR_3, LOGWARNING, (RLC_TX|RLC_AM),\
"Create segment header failed!",\
{"LineNum","Rnti","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_RETX_PDU_ERR_4, LOGWARNING, (RLC_TX|RLC_AM),\
"Invalid SO",\
{"LineNum","Rnti","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_RETX_PDU_ERR_5, LOGWARNING, (RLC_TX|RLC_AM),\
"Invalid soStart",\
{"LineNum","Rnti","LcId","SO","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_DATA_SEG_ERR, LOGWARNING, (RLC_TX|RLC_AM),\
"Invalid PDU Data pointer",\
{"LineNum","RetVal","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_DATA_SEG_ERR_1, LOGWARNING, (RLC_TX|RLC_AM),\
"GET_DATA_SEG msgAlloc Failed!",\
{"LineNum","RetVal","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_DATA_SEG_ERR_2, LOGWARNING, (RLC_TX|RLC_AM),\
"GET_DATA_SEG msgSplit Failed!",\
{"LineNum", "SplitPos", "RetVal","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_DATA_SEG_ERR_3, LOGWARNING, (RLC_TX|RLC_AM),\
"GET_DATA_SEGMNT msgAlloc Failed!",\
{"LineNum","RetVal","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_DATA_SEG_ERR_4, LOGWARNING, (RLC_TX|RLC_AM),\
"MsgSplit Failed!",\
{"LineNum","SplitPos", "RetVal","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_RETX_PDU_ERR_6, LOGWARNING, (RLC_TX|RLC_AM),\
"Get data segment failed!",\
{"LineNum","Rnti","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_RETX_PDU_ERR_7, LOGWARNING, (RLC_TX|RLC_AM),\
"MsgJoinAndFree Failed!",\
{"LineNum","Rnti","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_RETX_PDU_ERR_8, LOGWARNING, (RLC_TX|RLC_AM),\
"Invalid NACK list pointer",\
{"LineNum","Rnti","LcId","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_RETX_PDU_ERR_9, LOGWARNING, (RLC_TX|RLC_AM),\
"Invalid ReTx PDU",\
{"LineNum","Rnti","LcId", "SN","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_SEND_MAX_THRESHHOLD_ERR, LOGWARNING, (RLC_TX|RLC_AM),\
"Invalid TIDInfo Pointer",\
{"LineNum","Rnti","LcId", "SN","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_DATA_PDU_ERR, LOGWARNING, (RLC_TX|RLC_AM),\
"Invalid rlcAmRawSdu size",\
{"LineNum","Rnti","LcId", "Size","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_DATA_PDU_ERR_1, LOGWARNING, (RLC_TX|RLC_AM),\
"Amd Validation Failed!",\
{"LineNum","Rnti","LcId", "RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_DATA_PDU_ERR_2, LOGWARNING, (RLC_TX|RLC_AM),\
"ENCODE_DATA_PDU: Failed to allocate Mem from pool",\
{"LineNum","Rnti","LcId", "RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_DATA_PDU_ERR_3, LOGWARNING, (RLC_TX|RLC_AM),\
"Failed: allocate Mem from pool",\
{"LineNum","Rnti","LcId", "RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_DATA_PDU_ERR_4, LOGWARNING, (RLC_TX|RLC_AM),\
"ENCODE_PDU msgAlloc Failed!",\
{"LineNum","Rnti","LcId", "RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_DATA_PDU_ERR_5, LOGWARNING, (RLC_TX|RLC_AM),\
"Failed to allocate Mem from pool",\
{"LineNum","Rnti","LcId", "RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_DATA_PDU_ERR_6, LOGWARNING, (RLC_TX|RLC_AM),\
"RlcAddSduSegmentToAmdPdu failed!",\
{"LineNum","Rnti","LcId", "RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_DATA_PDU_ERR_7, LOGWARNING, (RLC_TX|RLC_AM),\
"Failed to encode SDU lost",\
{"LineNum","Rnti","LcId", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ENCODE_DATA_PDU_ERR_8, LOGWARNING, (RLC_TX|RLC_AM),\
"Reached MAX no of DFEi(512)",\
{"LineNum","Rnti","LcId", "NumDfe","",\
"","",\
FNAME,"",\
}},

/*****************AM RX New Logs****************/


{LTE_RLC_DECODE_AMD_HEADER_ERR, LOGWARNING, (RLC_RX),\
"DECODE_AMD_HEADER Invalid Data pointer",\
{"LineNum","RetVal","", "","",\
"","",\
FNAME,"",\
}},

/* + SPR 16766 */
{LTE_RLC_HDL_PDU_ERR, LOGWARNING, (RLC_RX),\
"Amd Decode failed!",\
{"LineNum","RetVal","ue", "lc","",\
"","",\
FNAME,"",\
}},
/* - SPR 16766 */

{LTE_RLC_RECV_STATUS_PDU_ERR, LOGWARNING, (RLC_RX),\
"RECV_STATUS_PDU Invalid Data pointer",\
{"LineNum","ue_index","ue_index", "RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HDL_PDU_ERR_1, LOGWARNING, (RLC_RX),\
"Discard PDU: Out of Window",\
{"LineNum","vrR","SN", "vrMR","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HDL_PDU_ERR_2, LOGWARNING, (RLC_RX),\
"Duplicate PDU received",\
{"LineNum","SnStatus","amdPdu_p->status", "RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_ERR, LOGWARNING, (RLC_RX),\
"Invalid base pointer",\
{"LineNum","RetVal","", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_ERR_1, LOGWARNING, (RLC_RX),\
"RlcDecodeAmdSegmentHeader failed",\
{"LineNum","RetVal","", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_CHECK_STATUS_REP_BUFF_ERR, LOGWARNING, (RLC_RX),\
"Invalid SO Start in resp buff ",\
{"LineNum","RetVal","", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_CHECK_STATUS_REP_BUFF_ERR_1, LOGWARNING, (RLC_RX),\
"Invalid PDU seg pointer in getFirstQueueNode",\
{"LineNum","RetVal","", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_CHECK_STATUS_REP_BUFF_ERR_2, LOGWARNING, (RLC_RX),\
"Invalid SO Start",\
{"LineNum","RetVal","", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_PROCESS_AMD_SEGMENT_ERR_1, LOGWARNING, (RLC_RX),\
"MsgRemove failed!",\
{"LineNum","","", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_PROCESS_AMD_SEGMENT_ERR_2, LOGWARNING, (RLC_RX),\
"RlcProcessAmdSegment failed!",\
{"LineNum","","", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_PROCESS_AMD_SEGMENT_ERR_3, LOGWARNING, (RLC_RX),\
"Failed to alloc Mem from pool",\
{"LineNum","RetVal","", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_GET_HEADER_INFO_ERR, LOGWARNING, (RLC_RX),\
"Invalid eliStart",\
{"LineNum","RetVal","", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_PROCESS_AMD_SEGMENT_ERR_4, LOGWARNING, (RLC_RX),\
"Invalid dfeCount rlcGetHeaderInfo failed.",\
{"LineNum","DfeCount","RetVal", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_ENQUE_SEG_ERR, LOGWARNING, (RLC_RX),\
"li > pDFE.size",\
{"LineNum","li","PDFE.size", "RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_ENQUE_SEG_ERR_1, LOGWARNING, (RLC_RX),\
"DECODE_ENQUE_SEG msgAlloc Failed!",\
{"LineNum","RetVal","", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_PROCESS_AMD_SEGMENT_ERR_5, LOGWARNING, (RLC_RX),\
"DecodeAndEnqueueAMSegment failed",\
{"LineNum","RetVal","", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HDL_PDU_ERR_3, LOGWARNING, (RLC_RX),\
"RlcDecodeAmdRawPdu/handleRlcAmdPduSegment failed.",\
{"LineNum","","", "","",\
"","",\
FNAME,"",\
}},
/* + SPR 16766 */
{LTE_RLC_RECV_DATA_ERR, LOGWARNING, (RLC_RX),\
"Error in handle PDU from lower layer",\
{"LineNum","RetVal","ue", "lc","",\
"","",\
FNAME,"",\
}},
/* - SPR 16766 */
{LTE_RLC_CHECK_RX_SN_ERR, LOGWARNING, (RLC_RX),\
"SnStatus = 0",\
{"LineNum","vrMS","vrR", "SN","SnStatus",\
"","",\
FNAME,"",\
}},

{LTE_RLC_CHECK_RX_SN_ERR_1, LOGWARNING, (RLC_RX),\
"Not all bytes of PDU/PDU seg recvd OR SN missed",\
{"LineNum","vrMS","vrR", "SN","SnStatus",\
"","",\
FNAME,"",\
}},

{LTE_RLC_REASSEMBLE_ERR, LOGWARNING, (RLC_RX),\
"MsgJoin Failed!",\
{"LineNum","","", "","",\
"","",\
FNAME,"",\
}},
/*
{LTE_RLC_DECODE_ENQUE_SEG_ERR_2, LOGWARNING, (RLC_RX),\
"MsgAlloc Failed!",\
{"LineNum","RetVal","", "","",\
"","",\
FNAME,"",\
}},*/

{LTE_RLC_REESTAB_UM_RX_ENTITY_1, LOGINFO, RLC_RRC,\
"Entity Reestablished",\
{"LineNum","Rnti","LcId", "Size used","UR",\
"UH","",\
FNAME,"",\
}},




{LTE_RLC_DECODE_SEG_HDR_ERR, LOGWARNING, (RLC_RX),\
"DECODE_SEG_HDR msgSplit failed!",\
{"LineNum","SplitIndex","RetVal", "","",\
"UH","",\
FNAME,"",\
}},

{LTE_RLC_AM_STATUS_PDU, LOGINFO, (RLC_TX|RLC_AM),\
"Control PDU Encoding successful.",\
{"LineNum","Rnti","LcId", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_AM_STATUS_PDU_1, LOGINFO, (RLC_TX|RLC_AM),\
"ReTx PDU Encoding successful.",\
{"LineNum","Rnti","LcId", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_AM_STATUS_PDU_2, LOGINFO, (RLC_TX|RLC_AM),\
"New PDU Encoding successful.",\
{"LineNum","Rnti","LcId", "","",\
"","",\
FNAME,"",\
}},

/* SPR 4391 changes start*/
{LTE_RLC_AM_STATUS_PDU_3, LOGINFO, RLC_TX,\
"RlcHandleTxOppurtunity successfully executed.",\
{"LineNum","","", "","",\
"","",\
FNAME,"",\
}},
/* SPR 4391 changes end*/


/*******************New Logs 6/03/12 ***************/
{LTE_RLC_DECODE_AMD_HEADER_ERR_1, LOGINFO, (RLC_RX),\
"Decode Amd Header Successful_1",\
{"LineNum","DC","RF", "P","FI",\
"","",\
FNAME,"",\
}},

{LTE_RLC_DECODE_AMD_HEADER_ERR_2, LOGINFO, (RLC_RX),\
"Decode Amd Header Successful_2",\
{"LineNum","E","SN", "LSF","SO",\
"","",\
FNAME,"",\
}},

{LTE_RLC_ACK_OUT_OF_WINDOW_3, LOGINFO, (RLC_TX|RLC_AM),\
"Stopping SP timer",\
{"LineNum","ue_index","LcId","ACK received","PollSN",\
"","",\
FNAME,"",\
}},

{LTE_RLC_FETCH_RETX_PDU_QLOAD, LOGINFO, (RLC_TX|RLC_AM),\
"FETCH_RETX_PDU_QLOAD",\
{"LineNum","ReTxQLoad","ReTxQSize","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_FETCH_RETX_PDU_QLOAD_1, LOGINFO, (RLC_TX|RLC_AM),\
"FETCH_RETX_PDU_QLOAD after adding fixed header size",\
{"LineNum","QLoad","ReTxQSize","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_FETCH_RETX_PDU_QLOAD_2, LOGINFO, (RLC_TX|RLC_AM),\
"Fetch Fresh Data Qload",\
{"ue_index", "LcId","QLoad","txSduIn.rlcAmRawSdu.size","vrHeaderSize",\
"","",\
FNAME,"",\
}},

{LTE_RLC_NACK_RECEIVE_STATUS, LOGINFO, (RLC_TX|RLC_AM),\
"Action On Nack Receive Status Report",\
{"ue_index", "LcId","vtA","vtMS","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_NACK_RECEIVE_STATUS_1, LOGWARNING, (RLC_TX|RLC_AM),\
"Null pointer amReTxPdu_p",\
{"ue_index", "LcId","ReTxId", "vtA","NackSn_t",\
"vtMS_t","",\
FNAME,"",\
}},

{LTE_RLC_NACK_RECEIVE_STATUS_2, LOGWARNING, (RLC_TX|RLC_AM),\
"Action On Receive Status Report-No nack present",\
{"LineNum","ue_index", "LcId","lastAckReceived","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_SEND_DEL_IND_INFO,LOGINFO, (RLC_TX|RLC_AM),\
"RLC Action On Receive Status Report Information",\
{"LineNum","ue_index","LcId","vtA","vtMS","", \
"",\
FNAME,"", \
}},

{LTE_RLC_PROCESS_AMD_SEGMENT_ERR_6, LOGINFO, (RLC_RX),\
"Decode Amd Raw Pdu Information E=0",\
{"LineNum","E","FI", "PDFE.size","SzHeader",\
"","",\
FNAME,"",\
}},

{LTE_RLC_PROCESS_AMD_SEGMENT_ERR_7, LOGINFO, (RLC_RX),\
"Decode Amd Raw Pdu Information E=1",\
{"LineNum","E","Dfe_p->status", "amdHdr_p->lsf"," amdHdr_p->rf",\
"","",\
FNAME,"",\
}},

{LTE_RLC_HDL_PDU_ERR_4, LOGINFO, (RLC_RX),\
"RlcHandleAmdPduFromLowerLayer executed",\
{"LineNum","RetVal", "vrR","vrMR","ringSeqNum",\
"","",\
FNAME,"",\
}},

{LTE_RLC_STATUS_REPORT_INFO, LOGINFO, (RLC_RX),\
"Trigger Status Report Information",\
{"LineNum","vrMS", "amRxE_p->vrR","RxStatusPduSize","amTxE_p->lcId",\
"","",\
FNAME,"",\
}},

{LTE_RLC_NULL_RETX_PDU_POINTER_1, LOGINFO, (RLC_TX|RLC_AM),\
"FetchIncmpPduQload Information",\
{"SN","ue_index","LcId","soStart","soEnd",\
"","",\
FNAME,"",\
}},

{LTE_RLC_CHECK_RX_SN_MS_ERR, LOGINFO, (RLC_RX),\
"all bytes SN = VR(MS) are received",\
{"LineNum","SN","vrR", "vrMS","SnStatus",\
"","",\
FNAME,"",\
}},

{LTE_RLC_IN_RECP_BUFFER_INFO, LOGINFO, (RLC_RX),\
"Action in reception buffer information",
{"vrR","vrMS","vrH","RetVal","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_IN_RECP_BUFFER_INFO_1, LOGINFO, (RLC_RX),\
"Next First free seqNum",
{"SN","UH","UR", "RCount","ringSN",\
"","",\
FNAME,"",\
}},

{LTE_RLC_IN_RECP_BUFFER_INFO_2, LOGINFO, (RLC_RX),\
"Update State variables and reassemble SDU",
{"RCount","ringSN","amdPdu_p->status", "vrR","vrMR",\
"","",\
FNAME,"",\
}},

{LTE_RLC_IN_RECP_BUFFER_INFO_3, LOGINFO, (RLC_RX),\
"Reassemble Amd Sdu Info",
{"RCount","SN","ue_index", "LcId","RetVal",\
"","",\
FNAME,"",\
}},

{LTE_RLC_IN_RECP_BUFFER_INFO_4, LOGINFO, (RLC_RX),\
"Update State variables & reassemble SDU",
{"LineNum","RCount","ringSN", "vrR","vrMR",\
"","",\
FNAME,"",\
}},

{LTE_RLC_IN_RECP_BUFFER_INFO_5, LOGINFO, (RLC_RX),\
"State variable updates Status report",
{"vrR","vrMR","vrMS", "triggerStatFlag","SN",\
"","",\
FNAME,"",\
}},

{LTE_RLC_IN_RECP_BUFFER_INFO_6, LOGINFO, (RLC_RX),\
"ReorderingTimer report",
{"UR","UH","UX", "MR","isRTimerRunning",\
"","",\
FNAME,"",\
}},

{LTE_RLC_RECV_DATA_INFO, LOGINFO, (RLC_RX),\
"Amd PDU Processed...",
{"LineNum","SN","RetVal", "","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_NACK_RECEIVE_STATUS_3, LOGINFO, (RLC_TX|RLC_AM),\
"SN falls within transmitting window",\
{"ue_index","LcId","vtA","vtMS","leastNackRcv",\
"","",\
FNAME,"",\
}},

{LTE_RLC_NACK_RECEIVE_STATUS_4, LOGINFO, (RLC_TX|RLC_AM),\
"Header Information Updated",\
{"E1","E2","NackSn","Sostart","Soend",\
"","",\
FNAME,"",\
}},
/* SPR 3894 change start */
{LTE_RLC_OUT_OF_RANGE_SN_IN_DEL_IND_1, LOGWARNING, (RLC_TX|RLC_AM),\
"Can not free out of range during Del Ind",\
{"ue_index","LcId","SN","vtA","vtS",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RLC_DELETE_ENTITY_ERR, LOGERROR,RLC_RRC,\
"Unable to mark Entity pending delete",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_DELETE_ENTITY_ERR_1, LOGWARNING,RLC_RRC,\
"No Node present for deletion ",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_DELETE_ENTITY_ERR_2, LOGWARNING,RLC_RRC,\
"Unable to delete Ue context ",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_DELETE_ENTITY_ERR_3, LOGWARNING,RLC_RRC,\
"Invalid request recived for Delete UE/Entity ",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_DELETE_ENTITY, LOGINFO,RLC_RRC,\
"UE marked as pending delete from RLC UL/DL thread ",\
{"LineNum","ue_index","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RECONFIG_ERR_1, LOGERROR,RLC_RRC,\
"RLC Reconfig Error_1",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RECONFIG_ERR_2, LOGWARNING,RLC_RRC,\
"RLC Reconfig Error_2",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RECONFIG_ERR_3, LOGWARNING,RLC_RRC,\
"RLC Reconfig Error_3",\
{"","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RECONFIG_ERR_4, LOGWARNING,RLC_RRC,\
"UE context not exist for reconfiguration",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RECONFIG_ERR_5, LOGWARNING,RLC_RRC,\
"LC not exist for reconfiguration",\
{"ue_index","LcId","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RECONFIG_ERR_6, LOGWARNING,RLC_RRC,\
"Unable to reconfig entity Error_6",\
{"ue_index","LcId","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RECONFIG_ERR_7, LOGWARNING,RLC_RRC,\
"Unable to reconfig entity Error_7",\
{"ue_index","LcId","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RECONFIG_ERR_8, LOGWARNING,RLC_RRC,\
"Unable to reconfig entity Error_8",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RECONFIG_ERR_9, LOGERROR,RLC_RRC,\
"Unable to reconfigure UE",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RE_ESTABLISH_ERR_1, LOGERROR,RLC_RRC,\
"Unable to reEstablish UE ",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RE_ESTABLISH_ERR_2, LOGWARNING,RLC_RRC,\
"No node found to reEstablish UE",\
{"","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RE_ESTABLISH_ERR_3, LOGWARNING,RLC_RRC,\
"Unable to reEstablish UE entity error_3",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},
{LTE_RLC_HANDLE_RE_ESTABLISH_ERR_4, LOGWARNING,RLC_RRC,\
"Unable to reEstablish UE entity error_4",\
{"ue_index","","","","",\
"","",\
FNAME,"",\
}},

{LTE_RLC_OUT_OF_RANGE_SN_IN_DEL_IND_2, LOGINFO, (RLC_TX|RLC_AM),\
"Free ReTx Buffer During Del Ind",\
{"ue_index","LcId","SN","vtA","vtS",\
"","",\
FNAME,"",\
}},
{ LTE_RLC_MULTIPLE_SPS_SETUP_NOT_VALID, LOGWARNING, RLC_RRC,\
"RLC receives SPS setup after setup from L3",\
{"LineNum", "ue_index", "RecvdReqType", "ConfiguredReqType", "",\
"", "",\
FNAME, "",\
}},
{LTE_RLC_DUPLICATE_STATUS_IND, LOGINFO, (RLC_TX|RLC_AM),\
"Receive Duplicate Status Indication",\
{"LineNum","ue_index","LcId","vtA","leastNackRcv",\
"","",\
FNAME,"",\
}},
{ LTE_RLC_GET_MEM_POOL_FAILED, LOGERROR, L2_SYS_FAIL,\
"RLC ERROR: getMemFromPool Failed",\
{"LineNum", "Size", "", "", "",\
"", "",\
FNAME, "File",\
}},
/* SPR 8196 Fix Start */
{RLC_OAM_INIT_IND,LOGDEBUG,RLC_OAM,\
"SEND_RLC_OAM_INIT",\
{"ApiId","SrcModuleId","DestModuleId","","",\
"","",\
FNAME,EVENT,\
}}, 
{LTE_RLC_ENCODE_NEW_PDU2, LOGINFO, (RLC_TX|RLC_AM),\
"NEW PDU Encoded",\
{"ue_index","LcId","SN","vtA","",\
"","",\
FNAME,"",\
}},
/* SPR 8196 Fix End */
/*PacketRelay LOGS*/  
{PR_RECVD_RECONFIG_UE_ENTITY_REQ, LOGINFO, PR_GTPU_CONTROL,\
"S1U->PR: Received S1U_RECONFIG_UE_ENTITY_REQ from S1U",\
{"LineNum","ue_index","LcId","","",
"","",\
FNAME,"",\
}},
{PR_OAM_INV_LOG_LEV,LOGWARNING, PR_GTPU_OAM,\
"Invalid Log Level received in SET_LOG_LEVEL_REQ from OAM",\
{"LineNum","Min Log Lev INVALID_LOG ","Max Log Lev LOGDEBUG ","Received","",
"","",\
FNAME,"", \
}},
{PR_SEND_CNF_TO_OAM_FAIL,LOGWARNING, PR_GTPU_OAM,\
"Sending Confirmation to OAM failed for mentioned API",\
{"LineNum","","","","",
"","",\
FNAME,"ApiId", \
}},
{PR_S1U_SETUP_SAP_REQ_NO_TUNNEL_INFO, LOGWARNING, PR_GTPU_CONTROL,\
"Received SETUP_SAP_REQ IE with Mandatory TUNNEL_INFO_IE absent from S1U",\
{"LineNum","ue_index","LcId","","",
"","",\
FNAME,"", \
}},
{PR_UL_DATA_NOT_SENT_TO_GTPU, LOGWARNING, PR_GTPU_UL_DATA,\
"Packet Relay could not send UL Data to GTPU",\
{"LineNum","LcId","","","",
"","",\
FNAME,"", \
}},
{PR_UL_DATA_SENT_TO_GTPU, LOGDEBUG, PR_GTPU_UL_DATA,\
"UL Data Sent successfully to GTPU",\
{"LineNum","LcId","","","",
"","",\
FNAME,"", \
}},
{PR_UL_DATA_EMPTY, LOGWARNING, PR_GTPU_UL_DATA,\
"Empty UL Data Req received from Adapter @ Packet Relay",\
{"LineNum","LcId","","","",
"","",\
FNAME,"", \
}},
{PR_DL_DATA_NOT_SENT_TO_PDCP, LOGINFO, PR_GTPU_DL_DATA,\
"DL data received from GTPU, not sent to PDCP by Packet Relay",\
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_DL_DATA_SENT_TO_PDCP, LOGDEBUG, PR_GTPU_DL_DATA,\
"DL Data Sent successfully to GTPU",\
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_DL_DATA_EMPTY, LOGWARNING, PR_GTPU_DL_DATA,\
"Empty DL Data Ind received from GTPU @ Packet Relay",\
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_DL_DATA_MSG_ALLOC_FAIL, LOGWARNING, L2_SYS_FAIL,\
"MsgAlloc Failed in DL Data Ind at Packet Relay/Adapter",\
{"LineNum","","","","",
"","",\
FNAME,"@", \
}},
{PR_DL_DATA_MSG_INSERT_FAIL, LOGFATAL, PR_GTPU_DL_DATA,\
"MsgInsert Failed in DL Data Ind at Packet Relay/Adapter",\
{"LineNum","","","","",
"","",\
FNAME,"@", \
}},
{PR_TEID_INDX_ALREADY_IN_USE_SETUP, LOGWARNING,PR_GTPU_CONTROL,
"TEID index already in Use For SETUP_SAP_REQ",
{"LineNum","RelayUeCnxtMngmntForTunnelId_g","","","",
"","",\
FNAME,"", 
}},
{PR_TEID_NOT_IN_USE_FOR_UE_LCID, LOGWARNING,PR_GTPU_CONTROL,
"Tunnel Id not in Use for mentioned UEId LCId",
{"LineNum","ue_index","LcId","","",
"","",\
FNAME,"", 
}},
{PR_SETUP_SAP_REQ_FAILURE, LOGWARNING, PR_GTPU_CONTROL,
"SETUP_SAP_REQ received with INV parameters or format - FAILURE",
{"LineNum","","","","",
"","",\
FNAME,"", 
}},
{PR_SETUP_SAP_REQ_EMPTY, LOGWARNING, PR_GTPU_CONTROL,
"Empty SETUP_SAP_REQ received from S1AP at packetRelay",
{"LineNum","","","","",
"","",\
FNAME,"", 
}},
{PR_RECVD_S1U_DELETE_UE_ENTITY_REQ, LOGINFO, PR_GTPU_CONTROL,
"S1U->PR: Received S1U_DELETE_UE_ENTITY_REQ",
{"LineNum","ue_index","","","",
"","",\
FNAME,"", 
}},
{PR_TEID_INDX_ALREADY_IN_USE_RELEASE, LOGWARNING, PR_GTPU_CONTROL,
"RELEASE_SAP_REQ @ PacketRelay : TEID index already in Use",
{"LineNum","ue_index","TEID index","","",
"","",
FNAME,"", 
}},
{PR_NO_PROCESS_S1U_DELETE_UE_ENTITY_REQ, LOGWARNING, PR_GTPU_CONTROL,
"RELEASE_SAP_REQ not sent to GTPU from PacketRelay",
{"LineNum","ue_index","","","",
"","",
FNAME,"", 
}},
{PR_SENT_S1U_DELETE_UE_ENTITY_REQ, LOGINFO, PR_GTPU_CONTROL,
"RELEASE_SAP_REQ sent to GTPU from PacketRelay",
{"LineNum","ue_index","","","",
"","",
FNAME,"", 
}},
{PR_SEND_CNF_TO_CP_FAIL,LOGWARNING, PR_GTPU_CONTROL,
"Packet Relay Could not send Indication/Cnf/Resp to CP",
{"LineNum","","","","",
"","",
FNAME,"ApiId", 
}},
{PR_SENT_ERR_IND_TO_CP,LOGINFO, PR_GTPU_CONTROL,
"PR->S1AP:Err Indication sent Successfully",
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{PR_SENT_PATH_FAILURE_IND_TO_CP,LOGINFO, PR_GTPU_CONTROL,
"PR->S1AP:Path Failure Indication sent Successfully",
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{PR_SETUP_SAP_CNF_TEID_FROM_GTPU, LOGINFO, PR_GTPU_CONTROL,
"GTPU->PR, SETUP_SAP_CNF Tunnel Id received",
{"LineNum","TEID Self","","","",
"","",
FNAME,"", 
}},
{PR_SETUP_SAP_CNF_NO_UE_CTXT, LOGWARNING, PR_GTPU_CONTROL,
"GTPU->PR, SETUP_SAP_CNF Received, No UE ctxt found",
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{PR_SENT_RELEASE_SAP_CNF_TO_CP,LOGINFO, PR_GTPU_CONTROL,
"PR->S1AP: RELEASE_SAP_CNF sent successfully",
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{PR_DETACH_TRIGGERED,LOGINFO, PR_GTPU_CONTROL,
"Adapter : Detach triggered for UeId",
{"LineNum","ue_index","","","",
"","",
FNAME,"", 
}},
{PR_MU_ADAPTER_GLUE_SELECT_FAILED,LOGWARNING, PR_GTPU_DL_DATA,
"Adapter : MU GLUE Select Failed",
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{PR_ADAPTER_READY_TO_RECEIVE_TEID_UEID_LCID,LOGINFO, PR_GTPU_DL_DATA,
"Adapter : Ready to receive TEID, UEID and LCID",
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{PR_ADAPTER_UEID_NOT_FOUND_RELEASE,LOGWARNING, PR_GTPU_DL_DATA,
"Adapter : UEId not found in Release DB",
{"LineNum","ue_index","","","",
"","",
FNAME,"", 
}},
{PR_ADAPTER_INV_API_ID,LOGWARNING, PR_GTPU_DL_DATA,
"Adapter : Invalid API Id, Should be - \
    PDCP_ADAPTER_TID_INFO/PDCP_ADAPTER_UE_INFO_FOR_REL",
{"LineNum","ApiId","","","",
"","",
FNAME,"", 
}},
{PR_ADAPTER_LESS_BYTES_RCVD, LOGFATAL, PR_GTPU_DL_DATA,
"Adapter : Less Bytes received from Socket!!!", 
{"LineNum","bytesRead","","","",
"","",
FNAME,"", 
}},
{PR_ADAPTER_INV_UEID_LCID, LOGWARNING, PR_GTPU_UL_DATA,
"Adapter : Inv UeId LcID received from PDCP!!!", 
{"LineNum","ue_index","LcId","","",
"","",
FNAME,"", 
}},
{PR_ADAPTER_EMPTY_UL_DATA, LOGWARNING, PR_GTPU_UL_DATA,\
"Empty UL Data Ind received from PDCP",\
{"LineNum","ue_index","LcId","","",
"","",\
FNAME,"", \
}},
{PR_ADAPTER_UL_DATA_GREATER_SIZE, LOGWARNING, PR_GTPU_UL_DATA,\
"UL Data > BUFFERSIZE received from PDCP",\
{"LineNum","BUFFERSIZE","Received Size","","",
"","",\
FNAME,"", \
}},
{PR_ADAPTER_ALREADY_INITIALIZED, LOGINFO, PR_GTPU_COMMON,\
"Adapter already Initialized",\
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_ADAPTER_NOT_INITIALIZED, LOGWARNING, PR_GTPU_COMMON,\
"Adapter Not yet Initialized",\
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_ADAPTER_GLUE_SOCKET_SUCCESS, LOGINFO, PR_GTPU_COMMON,\
"MU Adapter GLUE Socket Created successfully",\
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_ADAPTER_INV_UEID_RECVD,LOGWARNING,PR_GTPU_DL_DATA,\
"Adapter: Invalid UE Id Received",\
{"LineNum","ue_index","","","",
"","",\
FNAME,"", \
}},
{PR_ADAPTER_INSERT_TEID_UEID_LCID_IN_DB, LOGINFO, PR_GTPU_DL_DATA,\
"Adapter: Inserted rcvd TunnelId, UeId and LcId in DB successfully",\
{"LineNum","TunnelId","ue_index","LcId","",
"","",\
FNAME,"", \
}},
{PR_ADAPTER_DELETE_TEID_UEID_LCID_FROM_DB, LOGINFO, PR_GTPU_DL_DATA,\
"Adapter: Deleted TunnelId, UeId and LcId from DB successfully",\
{"LineNum","TunnelId","ue_index","LcId","",
"","",\
FNAME,"", \
}},
{PR_ADAPTER_ERR_IN_SENDING_PACKET_TO_TG, LOGWARNING, PR_GTPU_UL_DATA,\
"Adapter: Packet Sending Err to TG",\
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_ADAPTER_TUNNEL_ID_ALREADY_RCVD, LOGWARNING, PR_GTPU_DL_DATA,\
"Adapter: TunnelId already received",\
{"LineNum","TunnelId","","","",
"","",\
FNAME,"", \
}},
{PR_WRONG_MOD_ID_RCVD,LOGWARNING,PR_GTPU_CONTROL,\
"Wrong Source Module Id at PacketRelay Control",\
{"LineNum","Received","","","",
"","",\
FNAME,"", \
}},
{PR_PROCESSING_API_FAIL,LOGWARNING,PR_GTPU_CONTROL,\
"PacketRelay : Processing of FNAMEtion for API Id failed",\
{"LineNum","ApiId","","","",
"","",\
FNAME,"", \
}},
{PR_RCVD_S1U_CREATE_UE_ENTITY_REQ,LOGINFO,PR_GTPU_CONTROL,\
"PacketRelay : Rcvd S1U_CREATE_UE_ENTITY_REQ from S1U",\
{"LineNum","ue_index","","","",
"","",\
FNAME,"", \
}},
{PR_SEND_S1U_CREATE_UE_ENTITY_CNF,LOGINFO,PR_GTPU_CONTROL,\
"PR->S1U : Sending S1U_CREATE_UE_ENTITY_CNF ",\
{"LineNum","ue_index","Cause","","",
"","",\
FNAME,"", \
}},
{PR_RCVD_S1U_RECONFIG_UE_ENTITY_REQ,LOGINFO,PR_GTPU_CONTROL,\
"S1U->PR : Rcvd S1U_RECONFIG_UE_ENTITY_REQ from S1U",\
{"LineNum","ue_index","","","",
"","",\
FNAME,"", \
}},
{PR_RCVD_S1U_RECONFIG_INCORRECT_TAG,LOGWARNING, PR_GTPU_CONTROL,\
"PR : Rcvd Incorrect tag in S1U_RECONFIG_REQ",\
{"LineNum","Expected "," or ","Recieved","",
"","",\
FNAME,"", \
}},
{PR_SENT_TEID_UEID_LCID_TO_DLTG,LOGINFO, PR_GTPU_CONTROL,\
"PR->DL_TG: Sent TunnelId, UEId and lcId to DLTG",\
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_WRONG_IP_ADDR_LEN_IN_RELAYCONFIG,LOGWARNING, PR_GTPU_CONTROL,\
"PR: IP address Len of L2 is Zero in Relay Config",\
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_SEND_S1U_RECONFIG_CNF,LOGINFO,PR_GTPU_CONTROL,\
"PR->S1U:Sending S1U_RECONFIG_UE_ENTITY_CNF to S1U",\
{"LineNum","ue_index","Cause","","",
"","",\
FNAME,"", \
}},
{PR_RECVD_S1U_DEL_UE_ENTITY_REQ,LOGINFO,PR_GTPU_CONTROL,\
"S1U->PR:Received S1U_DELETE_UE_ENTITY_REQ",\
{"LineNum","ue_index","","","",
"","",\
FNAME,"", \
}},
{PR_UEID_NOT_FOUND_FOR_REL,LOGWARNING, PR_GTPU_CONTROL,\
"PR:UeId not found in Relay DB for S1U_DELETE_UE_ENTITY_REQ",\
{"LineNum","ue_index","","","",
"","",\
FNAME,"", \
}},
{PR_SENT_UEID_FOR_REL_TO_DLTG,LOGINFO, PR_GTPU_CONTROL,\
"PR->DL_TG :SENT UE-ID for Release to DL-TG",\
{"LineNum","ue_index","","","",
"","",\
FNAME,"", \
}},
{PR_SEND_S1U_DEL_UE_ENTITY_CNF,LOGINFO, PR_GTPU_CONTROL,\
"PR->S1U:Sending S1U_DELETE_UE_ENTITY_CNF to S1U",\
{"LineNum","ue_index","Cause","","",
"","",\
FNAME,"", \
}},
{PR_INIT_DONE,LOGINFO, PR_GTPU_CONTROL,\
"PacketRelay Init Successfully Done!!!",\
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_UNSUPPORTED_API_RCVD, LOGWARNING, PR_GTPU_COMMON,\
"GTPU/OAM/S1AP->PR: Unsupported API received from Source",\
{"LineNum","ApiId","","","",
"","",\
FNAME,"", \
}},
{PR_UNABLE_TO_OPEN_CONF_FILE,LOGWARNING,PR_GTPU_COMMON,\
"PR: Unable to open Config File: [../cfg/lteLayer2PortConfig.cfg]",
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_GTPU_INV_IE_RCVD_FROM_OAM, LOGWARNING, PR_GTPU_OAM,\
"PR: Invalid IE received from OAM in INITDB_REQ",
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_UNABLE_TO_SEND_ERR_REPORT_TO_OAM, LOGWARNING, PR_GTPU_OAM,\
"PR: Unable to Send Err Report to OAM",
{"LineNum","","","","",
"","",\
FNAME,"", \
}},
{PR_GTPU_API_PROCESSING_FAILED, LOGWARNING, PR_GTPU_OAM,\
"PR->GTPU: GTPU failed to process the API",
{"LineNum","","","","",
"","",\
FNAME,"ApiId", \
}},
{PR_SOCKET_OPEN_SUCCESS, LOGINFO, PR_GTPU_COMMON,\
"PR: Successfully Opened Socket for FD",
{"LineNum","Port","","","",
"","",\
FNAME,"FD", \
}},
{PR_SEND_IND_TO_PDCP_FAIL,LOGWARNING, PR_GTPU_CONTROL,
"Packet Relay Could not send Indication/Cnf/Resp to PDCP",
{"LineNum","","","","",
"","",
FNAME,"ApiId", 
}},
{PR_RCVD_EMI_FOR_NONEXIST_RAB,LOGWARNING, PR_GTPU_CONTROL,
"Received End Marker Ind for not existing RAB at PR",
{"LineNum","TransId not inUse","","","",
"","",
FNAME,"", 
}},
{PR_SEND_EMI_TO_CP,LOGINFO, PR_GTPU_CONTROL,
"PR->CP:End Marker Ind sent to S1AP",
{"LineNum","ue_index","LcId","","",
"","",
FNAME,EVENT, 
}},
{PR_SEND_EMI_TO_PDCP,LOGINFO, PR_GTPU_CONTROL,
"PR->PDCP:End Marker Ind sent to PDCP",
{"LineNum","ue_index","LcId","","",
"","",
FNAME,EVENT, 
}},
{PR_RCVD_END_OF_DATA_FWD_IND,LOGINFO, PR_GTPU_CONTROL,
"PDCP->Adapter:End Of Data Forwarding Ind",
{"LineNum","ue_index","LcId","","",
"","",
FNAME,"", 
}},
{PR_RCVD_END_OF_DATA_FWD_IND_INV_RAB,LOGWARNING, PR_GTPU_CONTROL,
"Adapter->PR:End Of Data Forwarding Ind for Invalid RAB",
{"LineNum","ue_index","LcId","","",
"","",
FNAME,"", 
}},
{PR_SETUP_SAP_REQ_SAP_INFO,LOGDEBUG, PR_GTPU_CONTROL,
"S1AP->PR:Received SETUP_SAP_REQ IEs from S1AP",
{"LineNum","ue_index","LcId","Sap flag","",
"","",
FNAME,"",
}},
{PR_SETUP_SAP_REQ_SAP_INFO_INV,LOGWARNING, PR_GTPU_CONTROL,
"S1AP->PR: Received Invalid SAP FLAG Value in SETUP_SAP_REQ",
{"LineNum","ue_index","LcId","Sap flag","",
"","",
FNAME,"",
}},
{PR_SETUP_SAP_RECONFIG_TUNNEL_ID_INUSE,LOGWARNING, PR_GTPU_CONTROL,
"S1AP->PR: SETUP_SAP_RECONFIG Tunnel Id Received from S1AP Already In use",
{"LineNum","ue_index","LcId","In Use Flag","",
"","",
FNAME,"",
}},
{PR_SAP_REL_TUNNEL_ID_INV,LOGWARNING, PR_GTPU_CONTROL,
"S1AP->PR: Received Invalid Tunnel Id in SAP_REL from S1AP ",
{"LineNum","ue_index","Tunnel Id","","",
"","",
FNAME,"",
}},
{PR_GTPU_SAP_REL_FAILURE,LOGWARNING, PR_GTPU_CONTROL,
"PR->GTPU: Sending SETUP_SAP_REL Message failed",
{"LineNum","ue_index","","","",
"","",
FNAME,"",
}},
{PR_HO_SRC_DATA_FWD_IND,LOGINFO, PR_GTPU_HO_DATA,
"PR->PDCP: Calling pdcpSourceDataFwdReq Indication",
{"LineNum","ue_index","lc_count","Start_LcId","ulDataFwdFlag",
"","",
FNAME,"",
}},
{PR_DLTG_TUNNEL_CREATE_IND,LOGDEBUG, PR_GTPU_HO_DATA,
"PR->DLTG: Tunnel Create Indication",
{"LineNum","ue_index|LcId","Peer-eGTPU","ENodeB-eGTPU",
"ENBType|TunnelType","","",
FNAME,"",
}},
{PR_MSG_POOL_HIGH_WATERMARK,LOGINFO, PR_GTPU_DL_DATA,
 "MsgPool high WaterMark Reached",
{"MsgPoolUsage","","","","",
 "","",
FNAME,EVENT,
}},
{PR_MSG_POOL_LOW_WATERMARK,LOGINFO, PR_GTPU_DL_DATA,
 "MsgPool Low WaterMark Reached",
{"MsgPoolUsage","","","","",
 "","",
FNAME,EVENT,
}},
{PR_MEM_POOL_HIGH_WATERMARK,LOGINFO, PR_GTPU_DL_DATA,
 "MemPool high WaterMark Reached",
{"MemPoolUsage","","","","",
 "","",
FNAME,EVENT,
}},
{PR_MEM_POOL_LOW_WATERMARK,LOGINFO, PR_GTPU_DL_DATA,
 "MemPool high WaterMark Reached",
{"MemPoolUsage","","","","",
 "","",
FNAME,EVENT,
}},
{PR_SEC_Q_FULL, LOGINFO, PR_GTPU_DL_DATA,\
"Max number of packets reached in SEC TX queue",\
{"LineNum","QSize","","","",\
"","",\
FNAME,EVENT,\
}},
{PR_SENT_TEID_UEID_LCID_TO_SECONDARY_DLTG, LOGINFO, PR_GTPU_CONTROL,\
"PR_SENT_TEID_UEID_LCID_TO_SECONDARY_DLTG",\
{"LineNum","","","","",\
"","",\
FNAME,"",\
}},
{PR_SECONDARY_TRANSPORT_ADDR, LOGINFO, PR_GTPU_CONTROL,\
"PR_SECONDARY_TRANSPORT_ADDR",\
{"LineNum","TunnelId","","","",\
"","",\
FNAME,"",\
}},
{PR_ATTEMPTED_NON_EXIST_RAB, LOGINFO, PR_GTPU_CONTROL,\
"PR_ATTEMPTED_NON_EXIST_RAB",\
{"LineNum","TunnelId","","","",\
"","",\
FNAME,"",\
}},
{PR_SENT_TO_SECONDARY_TG_GET_FAILED, LOGINFO, PR_GTPU_CONTROL,\
"PR_SENT_TO_SECONDARY_TG_GET_FAILED",\
{"LineNum","TeidSelf","TeidPeer","","",\
"","",\
FNAME,EVENT,\
}},
{PR_SECONDARY_TRANSPORT_ADDR_ALLREADY_EXIST, LOGINFO, PR_GTPU_CONTROL,\
"PR_SECONDARY_TRANSPORT_ADDR_ALLREADY_EXIST",\
{"LineNum","TunnelId","","","",\
"","",\
FNAME,"",\
}},
/* SPR 10894 [Intra-Cell HO] (Bug 604) start*/
{PR_RCVD_S1U_INTRA_ENB_DATA_FWD_REQ,LOGINFO,PR_GTPU_CONTROL,\
"PacketRelay : Rcvd S1U_CREATE_UE_ENTITY_REQ from S1U",\
{"LineNum","Src UeId","Tgt UeId","","",
"","",\
FNAME,"", \
}},
{PR_RCVD_S1U_INTRA_ENB_DATA_FWD_REQ_EMPTY,LOGWARNING,PR_GTPU_CONTROL,\
"PacketRelay : Rcvd S1U_CREATE_UE_ENTITY_REQ from S1U",\
{"LineNum","Src UeId","Tgt UeId","","",
"","",\
FNAME,"", \
}},
{PR_RCVD_S1U_LOCAL_PATH_SWITCH_REQ,LOGINFO,PR_GTPU_CONTROL,\
"PacketRelay : Rcvd S1U_CREATE_UE_ENTITY_REQ from S1U",\
{"LineNum","Src UeId","Tgt UeId","","",
"","",\
FNAME,"", \
}},
{PR_RCVD_S1U_LOCAL_PATH_SWITCH_REQ_EMPTY,LOGWARNING,PR_GTPU_CONTROL,\
"PacketRelay : Rcvd S1U_CREATE_UE_ENTITY_REQ from S1U",\
{"LineNum","Src UeId","Tgt UeId","","",
"","",\
FNAME,"", \
}},
/* SPR 16425 fix start */
{PR_ALL_TRANSID_USED, LOGFATAL, PR_GTPU_CONTROL,\
"PacketRelay used all the transaction Ids for tunnel creation",\
{"LineNum","TotalTransId","","","",
"","",\
"FNAME","", \
}},
/* SPR 16425 fix end */
/* SPR 10894 [Intra-Cell HO] (Bug 604) end*/
/*eGTPU LOGS*/
/*Category PR_GTPU_DL_DATA*/
{GTPU_NO_MEM_TO_CREATE_POOL, LOGFATAL, PR_GTPU_COMMON, \
"Unable to acquire memory for eGTPU pools ", 
{"LineNum","","","","",
"","",
FNAME,"",     
}},
{GTPU_INIT_DONE, LOGINFO, PR_GTPU_COMMON, \
"Egtpu_init called Successfully!! ", 
{"LineNum","","","","",
"","",
FNAME,"",     
}},
{GTPU_UNKNOWN_API, LOGWARNING, PR_GTPU_COMMON, \
"Unknown API received by eGTPU", 
{"LineNum","ApiId","","","",
"","",
FNAME,"",     
}},
{GTPU_INIT_NOT_DONE, LOGWARNING, PR_GTPU_COMMON, 
"EGTPu Stack not Initialized", 
{"LineNum","","","","",
"","",
FNAME,"",     
}},
{GTPU_MSG_LESS_BYTES_RCVD, LOGWARNING, PR_GTPU_DL_DATA, 
"Less Bytes received in Payload than expected", 
{"LineNum","Expected Len ","Received Len","","",
"","",
FNAME,"",     
}},
{GTPU_PAYLOAD_LESS_THAN_HDR_LEN, LOGWARNING, PR_GTPU_DL_DATA, 
"Payload Len < GTP Header Len (8 Bytes)", 
{"LineNum","Expected Len ","Received Len","","",
"","",
FNAME,"",     
}},
{GTPU_VERSION_INCORRECT, LOGDEBUG, PR_GTPU_DL_DATA, 
"Peer EGTPU Version Not Supported", 
{"LineNum","Supported GTP Ver","Peer GTP Ver","","",
"","",
FNAME,"",     
}},
{GTPU_PROTOCOLTYPE_INCORRECT, LOGDEBUG, PR_GTPU_DL_DATA, 
"Peer EGTPU Protocol Type Not Supported", 
{"LineNum","Supported GTP PT","Peer GTP PT","","",
"","",
FNAME,"",     
}},
{GTPU_INSUFFICIENT_HDR_LEN_WITH_SEQ_NUM, LOGWARNING, PR_GTPU_DL_DATA, 
"Insufficient GTP Header Length", 
{"LineNum","Expected","Received","","",
"","",
FNAME,"",    
}},
{GTPU_UNKNOWN_MSG, LOGWARNING, PR_GTPU_DL_DATA,\
"PduType != TPDU/ ECHO_REQ/ ECHO_RESP/ \
    SUPP_EXT_HDR_NOTI/ ERR_IND/ \
    END_MARKER",\
{"LineNum","Recvd pduType","","","",
"","",
FNAME,"", \
}},
{GTPU_TPDU_EXT_HDR_DECODE_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,\
"Extension Header Decoding Failure in the Data PDU received",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_ECHO_REQ_INV_FORMAT, LOGWARNING, PR_GTPU_DL_DATA,     /*10*/
"(S) flag not set in GTP Header of ECHO REQ received from Peer",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_ECHO_RESP_EXT_HDR_DECODE_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,\
"Extension Header Length goes beyond Payload",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_ECHO_RESP_INV_FORMAT, LOGWARNING, PR_GTPU_DL_DATA,\
"(S) flag not set in GTP Header of ECHO RESP to be sent",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_EXT_HDR_NOTI_DECODE_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,\
"Ext Header Len of Extension Header Notification goes beyond Payload",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_EXT_HDR_NOTI_INV_FORMAT, LOGWARNING, PR_GTPU_DL_DATA,\
"(S) flag not set in GTP Ext Header Notification Msg Received from Peer",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_ERR_IND_EXT_HDR_DECODE_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,\
"Ext Header Len of Err Indication goes beyond Payload",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_ERR_IND_INV_FORMAT, LOGWARNING, PR_GTPU_DL_DATA,\
"(S) flag not set in GTP Header of ERR IND Received from Peer",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_END_MARKER_EXT_HDR_DECODE_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,\
"Ext Header Len of END Marker Indication goes beyond Payload",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_NO_RX_TUNNEL, LOGWARNING, PR_GTPU_DL_DATA,\
"Tunnel not configured to receive DL User Data",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
/*MULTI_SECTOR changes start*/
{GTPU_RRC_S1U_CELL_INDEX_ERR, LOGWARNING, PR_GTPU_CONTROL,\
"RRC API cellIndex is different from selfCellIndex",
{"LineNum","cell_index","Selfcell_index","","",
"","",
FNAME,"", \
}},
/*MULTI_SECTOR changes end*/
{GTPU_NO_TORELAY_NO_FWD_SAP_SET, LOGWARNING, PR_GTPU_DL_DATA,\
"Either of the TO_RELAY_SAP ore FWD_SAP must be set to \
    forward the DL Data received from Peer",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_GPDU_SENT_TO_RELAY, LOGDEBUG, PR_GTPU_DL_DATA,   /*20*/
"GTP->PacketRelay : Received G-PDU successfully sent to Relay",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_ECHO_RESP_SENT_TO_PEER, LOGINFO, PR_GTPU_DL_DATA,\
"Echo Response Sent to Peer GTPu Entity",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_ECHO_RESP_RECVD_FROM_PEER, LOGINFO, PR_GTPU_DL_DATA,\
"Echo Response Received from Peer GTPu Entity",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_ECHO_RESP_RECVD_FROM_UNKNOWN_PEER, LOGWARNING, PR_GTPU_DL_DATA,\
"Echo Response Received from Unknown Peer, \
    for which peer context does not exist",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_ERR_IND_SENT_TO_CP, LOGINFO, PR_GTPU_DL_DATA,\
"Received Err Indication Sent to Control Plane",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_ERR_IND_RECVD_FROM_UNKNOWN_PEER, LOGWARNING, PR_GTPU_DL_DATA,\
"Received Err Indication From Unknown Peer",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_SENT_ERR_IND_TO_PEER, LOGINFO, PR_GTPU_DL_DATA,\
"Sent Err Indication to Peer",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_DATA_RCVD_FOR_UNKNOWN_RAB, LOGWARNING , PR_GTPU_DL_DATA,\
"RAB Context not found in eGTPu DB for the DL Data Received",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_END_MARKER_RCVD_FOR_UNKNOWN_RAB, LOGWARNING, PR_GTPU_DL_DATA,\
"RAB Context not found in eGTPu DB for the End Marker Pkt Received",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_END_MARKER_SENT_TO_CP, LOGINFO, PR_GTPU_DL_DATA,\
"End Marker Indication Sent to Control Plane",
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_END_MARKER_SENT_TO_PEER, LOGINFO, PR_GTPU_DL_DATA,     /*30*/
"GTPU->PR :End Marker Indication Sent to Peer GTPu",
{"LineNum","TunnelId Peer","","","",
"","",
FNAME,"", \
}},
/*Category PR_GTPU_OAM - 13*/    
{GTPU_INITDB_REQ_FAIL_DUE_TO_NUM_SRC_IP, LOGWARNING, PR_GTPU_DL_DATA,
"GTP Init failed cause : number of MY_ADDR > 10 or MY_ADDR < 1 \
    received from OAM",
{"LineNum","MAX Supported IP","MIN Supported IP","Recieved","",
"","",
FNAME,"", \
}},
{GTPU_INIT_FAIL_DUE_TO_IP_LEN, LOGWARNING, PR_GTPU_DL_DATA,\
"GTP Init failed cause : INVALID IP address Length Received", 
{"LineNum","IPv4 Len","IPv6 Len","Recieved","",
"","",
FNAME,"", \
}},
{GTPU_INITDB_CNF_SENT_TO_RELAY, LOGDEBUG, PR_GTPU_DL_DATA,\
"GTP Stacke Init Done, INITDB_CNF sent to Relay", 
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_INV_TIMER_TYPE_IN_SET_TMR_REQ, LOGDEBUG, PR_GTPU_DL_DATA,\
"Invalid Timer Type received in Set Timer Req from OAM", 
{"LineNum","Expected EGTPU_ECHO_RSP_TMR","Recieved ","","",
"","",
FNAME,"", \
}},
{GTPU_ECHO_RESP_TIMER_DURATION_RCVD, LOGDEBUG, PR_GTPU_DL_DATA,\
"Received Echo Request Timer Duration in SET_TIMER_REQ from OAM", 
{"LineNum","Recieved Duration","","","",
"","",
FNAME,"", \
}},
{GTPU_SEND_SET_TIMER_CNF, LOGDEBUG, PR_GTPU_DL_DATA,\
"GTP->RELAY : SET_TIMER_CNF Sent", 
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_INV_TIMER_TYPE_IN_GET_TMR_REQ, LOGDEBUG, PR_GTPU_DL_DATA,\
"Invalid Timer Type received in Get Timer Req from OAM", 
{"LineNum","Expected EGTPU_ECHO_RSP_TMR","Recieved ","","",
"","",
FNAME,"", \
}},
{GTPU_SEND_GET_TIMER_CNF, LOGDEBUG, PR_GTPU_DL_DATA,\
"GTP->RELAY : GET_TIMER_CNF Sent", 
{"LineNum","ECHO_RSP_TMR Duration Sent","","","",
"","",
FNAME,"", \
}},
{GTPU_INV_TIMER_TYPE_IN_SET_MAX_RETRY_REQ, LOGDEBUG, PR_GTPU_DL_DATA,\
"Invalid Timer Type received in Set Max Retry Req from OAM", 
{"LineNum","Expected EGTPU_ECHO_RSP_TMR","Recieved ","","",
"","",
FNAME,"", \
}},
{GTPU_SET_MAX_RETRY_REQ, LOGDEBUG, PR_GTPU_DL_DATA,\
"Received Max Retry of ECHO_RESP in SET_MAX_RETRY_REQ from OAM", 
{"LineNum","Recieved max Retries","","","",
"","",
FNAME,"", \
}},
{GTPU_SEND_SET_MAX_RETRY_CNF, LOGDEBUG, PR_GTPU_DL_DATA,\
"GTP->RELAY : SET_MAX_RETRY_CNF Sent", 
{"LineNum","","","","",
"","",
FNAME,"", \
}},
{GTPU_INV_TIMER_TYPE_IN_GET_MAX_RETRY_REQ, LOGDEBUG, PR_GTPU_DL_DATA,\
"Invalid Timer Type received in Get Max Retry Req from OAM", 
{"LineNum","Expected EGTPU_ECHO_RSP_TMR","Recieved ","","",
"","",
FNAME,"", \
}},
{GTPU_SEND_GET_MAX_RETRY_CNF, LOGDEBUG, PR_GTPU_DL_DATA,  
"GTP->RELAY : SET_MAX_RETRY_CNF Sent", 
{"LineNum","Max Retry Val Sent","","","",
"","",
FNAME,"", 
}},
/*Category PR_GTPU_UL_DATA - 7*/    
{GTP_UL_DATA_FOR_UNKNOWN_RAB, LOGWARNING, PR_GTPU_UL_DATA,
"UL Data Received for Unknown TEID, for which RAB ctxt does not exist", 
{"LineNum","Unknown TEID","","","",
"","",
FNAME,"", 
}},
{GTP_UL_TUNNEL_NO_TX, LOGWARNING, PR_GTPU_UL_DATA,
"UL Data for TEID, not configured with SAP flag either \
    FROM_RELAY_SAP/TX_SAP or PEER Transp Addr not Available", 
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{GTP_UL_DATA_SENT_TO_PEER, LOGDEBUG, PR_GTPU_UL_DATA,
"UL Data Sent to Peer", 
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{GTP_RELOC_DATA_REQ_FOR_UNKNOWN_RAB, LOGWARNING, PR_GTPU_UL_DATA,
"Reloc Data Req received for unknown TEID", 
{"LineNum","Unknown TEID","","","",
"","",
FNAME,"", 
}},
{GTP_RELOC_DATA_REQ_NO_FWD, LOGWARNING, PR_GTPU_UL_DATA,
"Reloc Data Req received for Tunnel not configured with either \
    RELOC_DATA_SAP/FWD_SAP or no FWD Addr Available", 
{"LineNum","TEID","","","",
"","",
FNAME,"", 
}},
{GTP_RELOC_DATA_REQ_NO_EXT_HDR_SUPP, LOGWARNING, PR_GTPU_UL_DATA,
"Reloc Data Req received with PDCP Sequence Number but peer does not support \
    Extension Header", 
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{GTP_RELOC_DATA_REQ_PROCESSED, LOGINFO, PR_GTPU_UL_DATA,
"Reloc Data Req is processed successfully", 
{"LineNum","","","","",
"","",
FNAME,"", 
}},
/*Category PR_GTPU_CONTROL - 14*/
{GTP_INV_IP_TRAFFIC_CLASS_IN_SETUP_SAP_REQ, LOGWARNING, PR_GTPU_CONTROL,
"Invalid IP Traffic Class Received in SETUP_SAP_REQ", 
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{GTP_INV_SRCIP_LEN_IN_SETUP_SAP_REQ, LOGWARNING, PR_GTPU_CONTROL,
"Invalid Source IP Address Len received in SETUP_SAP_REQ", 
{"LineNum","Rcvd Src Addr Len","","","",
"","",
FNAME,"", 
}},
{GTP_INV_DESTIP_LEN_IN_SETUP_SAP_REQ, LOGWARNING, PR_GTPU_CONTROL,
"Invalid Destination IP Address Len received in SETUP_SAP_REQ", 
{"LineNum","Rcvd Dest Addr Len","","","",
"","",
FNAME,"", 
}},
{GTP_SRCIP_DESTIP_LEN_MISMATCH, LOGWARNING, PR_GTPU_CONTROL,
"Src IP Len != Dest IP Len, in SETUP_SAP_REQ", 
{"LineNum","Src Addr Len","Dest Addr Len","","",
"","",
FNAME,"", 
}},
{GTP_RAB_CTXT_RELEASED, LOGINFO, PR_GTPU_CONTROL,
"Rab Context Released for RELEASE_SAP_REQ", 
{"LineNum","Rel TEID","","","",
"","",
FNAME,"", 
}},
{GTP_RELEASE_REQ_FOR_NONEXIST_RAB, LOGWARNING, PR_GTPU_CONTROL,
"RELEASE_SAP_REQ received for non existing RAB", 
{"LineNum","Rel TEID","","","",
"","",
FNAME,"", 
}},
{GTPU_SEND_CNF_TO_CP, LOGINFO, PR_GTPU_CONTROL,
"GTP->CP:Confirmation Sent to Control Plane ", 
{"LineNum","ApiId","","","",
"","",
FNAME,"", 
}},
{GTPU_CREATE_SAP_REQ_WITH_FWD_SAP, LOGWARNING, PR_GTPU_CONTROL,
"CREATE_SAP_REQ received FWD Sap flag enabled(incorrect) ", 
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{GTPU_CREATE_SAP_REQ_RSRC_NOT_AVAILABLE, LOGWARNING, PR_GTPU_CONTROL,
"CREATE_SAP_REQ received to create new Rab Ctxt, but resources not available\
    RAB CTXT is freed!!", 
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{GTPU_SRC_IP_NOT_CONFIGURED, LOGWARNING, PR_GTPU_CONTROL,
"CREATE_SAP_REQ received with Source IP not configured during Initialization ", 
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{GTPU_SRC_IP_NOT_PRESENT, LOGWARNING, PR_GTPU_CONTROL,
"CREATE_SAP_REQ received with No Source IP, RAB context freed ", 
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{GTPU_PEER_NOT_PRESENT, LOGWARNING, PR_GTPU_CONTROL,
"CREATE_SAP_REQ received with No Peer Info, RAB context freed ", 
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{GTPU_MOD_SAP_REQ_FOR_NONEXIST_RAB, LOGWARNING, PR_GTPU_CONTROL,
"MODIFY_SAP_REQ received for non existing RAB context, send CNF with cause = \
    CTXT_NOT_FOUND", 
{"LineNum","For TEID ","","","",
"","",
FNAME,"", 
}},
{GTPU_MOD_SAP_REQ_INV_SRC_ADD, LOGWARNING, PR_GTPU_CONTROL,
"MODIFY_SAP_REQ cannot be received with Source Addr, RAB ctxt Released ", 
{"LineNum","","","","",
"","",
FNAME,"", 
}},
{GTPU_SEND_EMI_TO_PEER_REQ_FOR_UNKNOWN_RAB, LOGWARNING, PR_GTPU_CONTROL,
"PR->GTPU: EMI_SEND_TO_PEER_REQ received for unknown RAB", 
{"LineNum","teIdSelf","","","",
"","",
FNAME,"", 
}},
{GTPU_DL_DATA_DISCARDED_AFTER_EMI, LOGINFO, PR_GTPU_DL_DATA,
"DL Data Discarded after End Marker Ind",
{"LineNum","","","","",
"","",
FNAME,"",
}},
/* + coverity 32080 */
{GTPU_SEND_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,
"Egtpu send from gtpu failure",
{"LineNum","","","","",
"","",
FNAME,"",
}},
/* - coverity 32080 */
/* + coverity 32078 */
/* SPR 17747 FIX START */
{GTPU_ECHO_TIMER_START_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,
"Unable to start the timer",
{"LineNum","","","","",
"","",
"FunctionName","TimerType",
}},

{GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
"GTPU reports error",
{"LineNum","error","PduType","","",
"","",
"FunctionName","Module",
}}
/* SPR 17747 FIX END */

/*{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },
{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },
{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },
{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },*/
/* compilation warning 10-JUN-2014 fix start */
//{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ },{ }
/* compilation warning 10-JUN-2014 fix end */
};

/*
 ********************************
 * Log Level Strings
 ********************************
 */
Char8* logLevelStrValues[]={""    , "FATAL","ERROR","","WARN","","","",\
                            "INFO", "",     "",     "","",    "","","",\
                            "BRIEF","",     "",     "","",    "","","",\
                            "",     "",     "",     "","",    "","","",\
                            "DEBUG","",     "",     "","",    "","","",\
                            "",     "",     "",     "","",    "","","",\
                            "",     "",     "",     "","",    "","","",\
                            "",     "",     "",     "","",    "","","",\
                            "DEBUGTTI" };


Char8* logModuleStrValues[]={"","MAC","PDCP","","RLC","","","",\
                              "GTPUPR","","","","","","","",\
                              "SYNC"
                              };   
/*****************************************************************************
 * Function Name  : initLogCategoryStrValues
 * Inputs         :  
 * Outputs        : None 
 * Returns        : None
 * Description    :   
 *****************************************************************************/
const Char8 *getLogCategoryStrValues(UInt64 logCategory)
{
    const Char8 *strValue="";
    switch(logCategory) {
    case L2_SYS_FAIL:
           strValue = "L2_SYS_FAIL";
           break;
    case MAC_L1_INF:
           strValue = "MAC_L1_INF";
           break;
    case MAC_UL_HARQ:
           strValue = "MAC_UL_HARQ";
           break;
    case MAC_RACH:
           strValue = "MAC_RACH";
           break;
    case MAC_RRC_INF:
           strValue = "MAC_RRC_INF";
           break;
    case MAC_OAM_INF:
           strValue = "MAC_OAM_INF";
           break;
/* ICIC changes start */
    case MAC_RRM_INF:
           strValue = "MAC_RRM_INF";
           break;
/* ICIC changes end */

    case MAC_DL_Strategy:
           strValue = "MAC_DL_Strategy";
           break;
    case MAC_UL_Strategy:
           strValue = "MAC_UL_Strategy";
           break;
    case MAC_PDSCH:
           strValue = "MAC_PDSCH";
           break;
    case MAC_MUX:
           strValue = "MAC_MUX";
           break;
    case MAC_PDCCH:
           strValue = "MAC_PDCCH";
           break;
    case MAC_PUSCH:
           strValue = "MAC_PUSCH";
           break;
    case MAC_PUCCH:
           strValue = "MAC_PUCCH";
           break;
    case MAC_BCCH:
           strValue = "MAC_BCCH";
           break;
    case MAC_PCCH:
           strValue = "MAC_PCCH";
           break;
    case MAC_CCCH:
           strValue = "MAC_CCCH";
           break;
    case MAC_DL_HARQ:
           strValue = "MAC_DL_HARQ";
           break;
    case MAC_CQI:
           strValue = "MAC_CQI";
           break;
    case MAC_DL_SPS_STRATEGY:
          strValue = "MAC_DL_SPS_STRATEGY";
          break;
    case MAC_UL_SPS_STRATEGY:
          strValue = "MAC_UL_SPS_STRATEGY";
          break;
    /* + SPR_10291 */
    case MAC_DL_PC:
          strValue = "MAC_DL_PC";
          break;
    case MAC_UL_PC:
          strValue = "MAC_UL_PC";
          break;
    case MAC_TB_DL:
          strValue = "MAC_TB_DL";
          break;
    case MAC_TB_UL:
          strValue = "MAC_TB_UL";
          break;
    case MAC_SRS:
          strValue = "MAC_SRS";
          break;
    case MAC_TA:
          strValue = "MAC_TA";
          break;
    case MAC_MEAS_GAP:
          strValue = "MAC_MEAS_GAP";
          break;
    case MAC_UL_CSI:
          strValue = "MAC_UL_CSI";
          break;
    case DL_RESOURCE_MGR:
          strValue = "DL_RESOURCE_MGR";
          break;
    /* -SPR_10291 */
    /* ATB changes logs */
    case MAC_ATB:
         strValue = "MAC_ATB";
         break;
    case MAC_SCHEDULER:
    {
        strValue = "MAC_SCHEDULER";
        break;
    }
    case RLC_TX:
           strValue = "RLC_TX";
           break;
    case RLC_RX:
           strValue = "RLC_RX";
           break;
    case RLC_UM:
           strValue = "RLC_UM";
           break;
    case RLC_AM:
           strValue = "RLC_AM";
           break;
    case RLC_TM:
           strValue = "RLC_TM";
           break;
    case (RLC_TX|RLC_UM):
           strValue = "RLC_UM_TX";
           break;
    case (RLC_RX|RLC_UM):
           strValue = "RLC_UM_RX";
           break;
    case (RLC_TX|RLC_AM):
           strValue = "RLC_AM_TX";
           break;
    case (RLC_RX|RLC_AM):
           strValue = "RLC_AM_RX";
           break;
    case (RLC_TX|RLC_TM):
           strValue = "RLC_TM_TX";
           break;
    case (RLC_RX|RLC_TM):
           strValue = "RLC_TM_RX";
           break;
    case RLC_MAC:
           strValue = "RLC_MAC";
           break;
    case RLC_PDCP:
           strValue = "RLC_PDCP";
           break;
    case RLC_RRC:
           strValue = "RLC_RRC";
           break;
    case RLC_OAM:
           strValue = "RLC_OAM";
           break;
    case RLC_TIMER:
           strValue = "RLC_TIMER";
           break;
    case PDCP_RRCOAM:
           strValue = "PDCP_RRCOAM";
           break;
    case PDCP_TX:
           strValue = "PDCP_TX";
           break;
    case PDCP_RX:
           strValue = "PDCP_RX";
           break; 
    case (PDCP_TX | PDCP_RRCOAM):
           strValue = "PDCP_TX_RRC_OAM";
           break;
    case (PDCP_RX | PDCP_RRCOAM):
           strValue = "PDCP_RX_RRC_OAM";
           break;
    case (PDCP_TX|PDCP_RX):
           strValue = "PDCP_RX_TX";
           break;         
    case PDCP_RRM_INT:
           strValue = "PDCP_RRM_INT";
           break;         
    /*SPR 2030 fix Start*/
    case PR_GTPU_CONTROL:
           strValue = "PR_GTPU_CONTROL";
           break;
    case PR_GTPU_OAM:
           strValue = "PR_GTPU_OAM";
           break;
    case PR_GTPU_DL_DATA:
           strValue = "PR_GTPU_DL_DATA";
           break;
    case PR_GTPU_UL_DATA:
           strValue = "PR_GTPU_UL_DATA";
           break;
    case PR_GTPU_COMMON:
           strValue = "PR_GTPU_COMMON";
           break;
    case PR_GTPU_HO_DATA:
           strValue = "PR_GTPU_HO_DATA";
           break;
           /*SPR 2030 fix End*/
    case MAC_DRX:
           strValue = "MAC_DRX_MGR";
           break;
    case MAC_MEAS_HANDLER:
           strValue = "MAC_MEAS_HANDLER";
           break; 
#ifdef LTE_EMBMS_SUPPORTED           
    case L2_EMBMS_CAT:
           strValue = "L2_EMBMS_CAT";
           break;
#endif          
#ifdef ENDC_ENABLED
		   /*Dual Connectivity SeNB changes +*/
	case X2U_RELAY:
		   strValue = "X2U_RELAY";
           break;
		   /*Dual Connectivity SeNB changes -*/
#endif

    default:
           break;
    }
    return strValue;
}

/*****************************************************************************
 * Function Name  : initLogArray
 * Inputs         :  
 * Outputs        : None 
 * Returns        : None
 * Description    :   
 *****************************************************************************/
void verifyLogArray()
{
   UInt32 logArrayIndex,logIndex;

   for(logArrayIndex=0;logArrayIndex <= MAC_CURR_MAX_LOG_ID;logArrayIndex++)
   {

       if(logArrayIndex != logCommValues[logArrayIndex].logId)
       {
           printf("IN MAC LOGINDEX %d LOGID %d\n",logArrayIndex,logCommValues[logArrayIndex].logId);
           
           printf("IN MAC LOGINDEX %d STR %s\n",logArrayIndex,logCommValues[logArrayIndex].log_str);
           
           printf("IN MAC LOGID %d STR %s\n",logCommValues[logArrayIndex].logId,
                                logCommValues[logCommValues[logArrayIndex].logId].log_str);
          
       }
   }
   for(logArrayIndex=PDCP_CONTEXT_ALREADY_INITIALIZED;\
           logArrayIndex <= PDCP_CURR_MAX_LOG_ID;logArrayIndex++)
   {
       if(logArrayIndex != logCommValues[logArrayIndex].logId)
       {
           printf("IN PDCP LOGINDEX %d LOGID %d\n",logArrayIndex,logCommValues[logArrayIndex].logId);
       }

   }
   for(logArrayIndex=LTE_RLC_SEND_DEL_IND_FAILED;\
           logArrayIndex <= RLC_CURR_MAX_LOG_ID;logArrayIndex++)
   {
       if(logArrayIndex != logCommValues[logArrayIndex].logId)
       {
           printf("IN RLC LOGINDEX %d LOGID %d\n",logArrayIndex,logCommValues[logArrayIndex].logId);
       }

   }
   for(logArrayIndex=PR_RECVD_RECONFIG_UE_ENTITY_REQ;\
           logArrayIndex <= PR_MAX_LOG_ID; logArrayIndex++)
   {
       if(logArrayIndex != logCommValues[logArrayIndex].logId)
       {
           printf("IN PR LOGINDEX %d LOGID %d\n",logArrayIndex,logCommValues[logArrayIndex].logId);
       }
   }
   for(logArrayIndex = GTPU_NO_MEM_TO_CREATE_POOL;
           logArrayIndex <= GTPU_MAX_LOG_ID ; logArrayIndex++)
   {
       if(logArrayIndex != logCommValues[logArrayIndex].logId)
       {
           printf("IN GTPU LOGINDEX %d LOGID %d\n",logArrayIndex,logCommValues[logArrayIndex].logId);
       }
   }
  
   for(logArrayIndex=0;logArrayIndex <= (MAC_CURR_MAX_LOG_ID -1);logArrayIndex++)
   {
       for(logIndex=logArrayIndex + 1;logIndex <= MAC_CURR_MAX_LOG_ID;logIndex++)
       {
           if(!(strcmp((const char *) logCommValues[logArrayIndex].log_str,
                           (const char *)logCommValues[logIndex].log_str)))
           {
               printf("IN MAC STR COMP LOGINDEX1 %d LOGINDEX2 %d %s\n",logArrayIndex,logIndex,\
                       logCommValues[logArrayIndex].log_str);
           }
       }
   }
   for(logArrayIndex=PDCP_CONTEXT_ALREADY_INITIALIZED;\
           logArrayIndex <= (PDCP_CURR_MAX_LOG_ID -1);logArrayIndex++)
   {
       for(logIndex=logArrayIndex + 1;logIndex <= PDCP_CURR_MAX_LOG_ID;logIndex++)
       {
           if(!(strcmp((const char *) logCommValues[logArrayIndex].log_str,
               (const char *)logCommValues[logIndex].log_str)))
           {
               printf("IN PDCP STR COMP LOGINDEX1 %d LOGINDEX2 %d %s\n",logArrayIndex,logIndex,\
                       logCommValues[logArrayIndex].log_str);
           }
       }

   }
   /*for(logArrayIndex=LOG_RLC_LOG_ID1;\*/
   /*for(logArrayIndex=LTE_RLC_PDU_TYPE;\(*/
   for(logArrayIndex=LTE_RLC_SEND_DEL_IND_FAILED;\
           logArrayIndex <= (RLC_CURR_MAX_LOG_ID - 1);logArrayIndex++)
   {
       for(logIndex=logArrayIndex + 1;logIndex <= RLC_CURR_MAX_LOG_ID;logIndex++)
       {
           if(!(strcmp((const char *) logCommValues[logArrayIndex].log_str,
               (const char *)logCommValues[logIndex].log_str)))
           {
               printf("IN RLC STR COMP LOGINDEX1 %d LOGINDEX2 %d %s\n",logArrayIndex,logIndex,\
                       logCommValues[logArrayIndex].log_str);
           }
       }

   }
 
}    
                          
/*****************************************************************************
 * Function Name  : console_print
 * Inputs         :  
 * Outputs        : None 
 * Returns        : None
 * Description    : A variadic function called by any thread for logging.  
 *****************************************************************************/
/* SPR 15909 fix start */
/* +- SPR 17777 */
void console_print(UInt32 logId, UInt32 logLevel,UInt64 logCategory,\
        tickType_t globalTick,UInt32 intValOne,UInt32 intValTwo,\
        UInt32 intValThree,UInt32 intValFour,UInt32 intValFive,\
        const Char8 *strValOne,const Char8 *strValTwo)
/* +- SPR 17777 */
{
    /* SPR 15909 fix end */
    UInt32 i;
    LogParamValues paramValues;
    const Char8 *strValues,*logStr,*loglevel,*logarea;

    paramValues.logId = logId;
    /*paramValues.logLevel = logLevel;
    paramValues.logCategory = logCategory;*/
    paramValues.globalTick = globalTick;
    paramValues.intValues[0] = intValOne;
    paramValues.intValues[1] = intValTwo;
    paramValues.intValues[2] = intValThree;
    paramValues.intValues[3] = intValFour;
    paramValues.intValues[4] = intValFive;
    /*paramValues.flValues[0]  = floatValOne;
    paramValues.flValues[1] = floatValTwo;*/

    if(logId != INVALID_LOG_ID)
    {
        if(MAC_PHY_SF_IND_ID == logId)
        {
            printf ("**************************************************************"\
                    "**************************************************************\n");
        }
        logStr=logCommValues[logId].log_str;
        loglevel= logLevelStrValues[logLevel];
        logarea = getLogCategoryStrValues(logCategory);

        /* SPR 15909 fix start */
        printf ("%010llu %-7s %-15s %s ",paramValues.globalTick,loglevel,logarea,logStr);
        /* SPR 15909 fix end */
        for(i=0;i<MAX_LOG_INT_VALUES;i++)
        {
            strValues=logCommValues[logId].strValues[i];
            if(strlen(strValues))
            {
                printf ("%s=%d ",\
                        logCommValues[logId].strValues[i],\
                        paramValues.intValues[i]);
            }
            else
            {
                break;
            }
        }
        /*for(i=0;i<MAX_LOG_FLOAT_VALUES;i++)
        {
            strValues=logCommValues[logId].strValues[i + MAX_LOG_INT_VALUES];
            if(strlen(strValues))
            {
                printf ("%s=%-.2f ",\
                        logCommValues[logId].strValues[i + MAX_LOG_INT_VALUES],\
                        paramValues.flValues[i]);
            }
            else
            {
                break;
            }
        }*/
        i=0;
        strValues=\
                  logCommValues[logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES];
        if(strlen(strValues))
        {
            printf ("%s=%s ",\
                    logCommValues[logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES],
                    strValOne);
        }
        i=1;
        strValues=\
                  logCommValues[logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES];
        if(strlen(strValues))
        {
            printf ("%s=%s ",\
                    logCommValues[logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES],
                    strValTwo);
        }
        printf ("\n");
    }

}

/* MAC_TTI_LOG Start */
void  interpet_tti_logs(UInt32 logId,void *log_ptr,FILE *fptr) 
{

/* Coverity_20474_fix Start */
	       ttiDciLoggingInfo *dci_prnt_ptr = PNULL; 
	       ttiULLoggingInfo *ul_conf_prnt_ptr = PNULL; 
/* Coverity_20474_fix End */

     static unsigned int dlCount = 0 , ulCount = 0;
#define L2_MAC_TTI_LOG_COUNT 5000

 
     switch(logId)
     {
         case MAC_TTI_DCI_LOG_INFO :
	       dci_prnt_ptr = (ttiDciLoggingInfo *)log_ptr; 
               if(0 == dlCount % L2_MAC_TTI_LOG_COUNT) /*Printing this log after DL instances of 5000*/
               {
fprintf(fptr,"LogID\t,SFN\t,SF\t,Rnti\t,UEID\t,No Of RB ,DCI\t,HarqID\t,RATPolicy ,RBStart ,TPC\t,TransmissionScheme ,RntiTYPE ,Layer's\t,"
" TB'S \t,TBSIZ1\t,MCS1\t,RV1\t,TransmissionType ,TBSIZ2\t,MCS2\t,RV2\t,TransmissionType2 ,CCE\t,Agg\t,dlQLd\t,ulBSR \n\n");
               }
               dlCount++;
fprintf(fptr,"MAC_TTI_DLSCH_LOG_INFO\n%4d \t,%3d \t,%d \t,%3d \t,%3d \t,%5d \t,%s \t,%3d \t,%s \t,%5d \t,%3d \t,%s ,%s ,%5d \t,%3d \t,%5d \t,"
"%3d \t,%3d \t,%s \t,%5d \t,%3d \t,%3d \t,%s \t,%d \t,%d \t,%3d \t,%3d ",dci_prnt_ptr->logid,dci_prnt_ptr->systemFrame,dci_prnt_ptr->subframe,dci_prnt_ptr->rnti,dci_prnt_ptr->ueID,dci_prnt_ptr->numOfRBs,dciStr[dci_prnt_ptr->dciFormat],dci_prnt_ptr->harqID,RATPolicyType[dci_prnt_ptr->ratPolicy],dci_prnt_ptr->rbCoding,dci_prnt_ptr->tpc,TransmissionScheme[dci_prnt_ptr->transmissionScheme],RNTIType[dci_prnt_ptr->rntiType],dci_prnt_ptr->numOfLayers,dci_prnt_ptr->numOfTB,dci_prnt_ptr->tbSize1,dci_prnt_ptr->mcsIndex1,dci_prnt_ptr->rv1,TransType[dci_prnt_ptr->ndi1],dci_prnt_ptr->tbSize2,dci_prnt_ptr->mcsIndex2,dci_prnt_ptr->rv2,TransType[dci_prnt_ptr->ndi2],dci_prnt_ptr->cceIndex,dci_prnt_ptr->aggregationLevel,dci_prnt_ptr->dlQueueLoad,dci_prnt_ptr->ulBSR);
         break;


         case MAC_TTI_UL_CONF_LOG_INFO :
	       ul_conf_prnt_ptr = (ttiULLoggingInfo *)log_ptr; 
               if(0 == ulCount % L2_MAC_TTI_LOG_COUNT)   /*Printing this log after UL instances of 5000*/
               {
		      fprintf(fptr,"LogID \t,SFN \t,SF \t,RNTI \t,UEID \t,No of RB \t,RBStart \t,pduType \t,TBSIZE ,MCS \t,ModulationType"
		      ",RV,TransmissionType \n\n");
               }
               ulCount++;
	       fprintf(fptr,"MAC_TTI_UL_LOG_INFO\n%4d \t,%3d \t,%d \t,%3d \t,%3d \t,%7d \t,%6d \t,%s ,%d \t,%2d \t,%s ,%d \t,%d"
       ,ul_conf_prnt_ptr->logid,ul_conf_prnt_ptr->systemFrame,ul_conf_prnt_ptr->subframe,ul_conf_prnt_ptr->rnti,ul_conf_prnt_ptr->UEID,
       ul_conf_prnt_ptr->numOfRBs,ul_conf_prnt_ptr->RBStart,pduTypeString[ul_conf_prnt_ptr->pduType],ul_conf_prnt_ptr->tbSize,
       ul_conf_prnt_ptr->mcsIndex,modulationString[ul_conf_prnt_ptr->modType],ul_conf_prnt_ptr->rv,ul_conf_prnt_ptr->txIndicator);

         break;

     }
     fprintf(fptr,"\n");

}
/* FAPI LOGS */
void interpret_fapi_log(Log_FAPI_st *pfapi_logs, FILE *fptr)
{
    /* +- SPR 17777 */
    Char8 *logStr,*loglevel;
    const Char8 *logarea;
    struct tm timestamp;
    
	if (!pfapi_logs) {
		fprintf(fptr, "Invalid parameter, pfapi_logs is NULL\n");
		return;
	}

	/* check the logID to avoid arrays out of bound */
    if(pfapi_logs->logId != INVALID_LOG_ID)
    {
        if(NULL == localtime_r(((time_t*)&pfapi_logs->timestampSec), &timestamp))
        {
            fprintf(fptr, "timestamp decoding failed\n");
        }
        logStr=logCommValues[pfapi_logs->logId].log_str;
        loglevel= logLevelStrValues[logCommValues[pfapi_logs->logId].logLevel];
        logarea = getLogCategoryStrValues(logCommValues[pfapi_logs->logId].logCategory);
        fprintf (fptr, "[%04d-%02d-%02d/%02d:%02d:%02d.%03d]",
                timestamp.tm_year+1900, (timestamp.tm_mon + 1), timestamp.tm_mday,
                timestamp.tm_hour, timestamp.tm_min, timestamp.tm_sec,
                pfapi_logs->timestampUsec/1000);
        fprintf (fptr, "[%4d-%02d][%5s] ", (pfapi_logs->globalTick/10)%1024,
                pfapi_logs->globalTick%10, loglevel);
        fprintf (fptr, "[%41s] ", pfapi_logs->strFuncName);
        fprintf (fptr, "[%4d] [%5s] [%s] ", pfapi_logs->logId, logarea,logStr);
        fprintf (fptr,"\n");
        decode_fapi_message(pfapi_logs->strMessage, fptr);
        fprintf (fptr,"\n");
    }
    else
    {
        fprintf(fptr, "Invalid log\n");
    }
}


/* MAC_TTI_LOG End */

/* L2_FSM_LOG Start */

const Char8 *getMACStates(UInt32 val)
{
    const Char8 *strValue="";
    switch(val) {
    case 0:
           strValue = "SPS_IDLE";
           break;
    case 1:
           strValue = "SPS_CONFIGURED";
           break;
    case 2:
           strValue = "SPS_RES_RSV_REQ_INITIATED";
           break;
    case 3:
           strValue = "SPS_ACTIVATION_INITIATED";
           break;
    case 4:
           strValue = "SPS_ACTIVATED";
           break;
    case 5:
           strValue = "SPS_DEACTIVATION_INITIATED";
           break;
    case 6:
           strValue = "SPS_DEACTIVATED";
           break;
    case 7:
           strValue = "SPS_REACTIVATION_INITIATED";
           break;
    case 8:
           strValue = "UE_NOT_READY";
           break;
    case 9:
           strValue = "INVALID_STATE";
           break;
    case 10:
           strValue = "MAX_SPS_STATE";
           break;
    default:
           strValue = "Inv_State_Value";
    }

    return strValue;
}


const Char8 *getRLCStates(UInt32 val)
{
    const Char8 *strValue="";
    switch(val) {
    case 0:
           strValue = "RLC_CTRL_IDLE_ST";
           break;
    case 1:
           strValue = "WAITING_FOR_HP_RLC_DELETE_UE_CONTEXT_RES_ST";
           break;
    case 2:
           strValue = "WAITING_FOR_HP_RLC_DELETE_ENTITY_RES_ST";
           break;
    case 3:
           strValue = "WAITING_FOR_HP_RLC_RECONFIG_ENTITY_RES_ST";
           break;
    case 4:
           strValue = "WAITING_FOR_HP_RLC_REESTAB_ENTITY_RES_ST";
           break;
    case 5:
           strValue = "RLC_CTRL_MAX_ST";
           break;
    default:
           strValue = "Inv_State_Value";
    }
    return strValue;
}

const Char8 *getPDCPSpsStates(UInt32 val)
{
    const Char8 *strValue="";
    switch(val) {
    case 0:
           strValue = "PDCP_SPS_IDLE";
           break;
    case 1:
           strValue = "PDCP_SPS_CONFIGURED";
           break; 
    case 2:
           strValue = "PDCP_SPS_ACTIVATED_SILENCE_NOT_DETECTED";
           break;
    case 3:
           strValue = "PDCP_SPS_ACTIVATED_SILENCE_DETECTED";
           break;
    case 4:
           strValue = "PDCP_SPS_DEACTIVATED_VOICE_NOT_DETECTED";
           break;
    case 5:
           strValue = "PDCP_SPS_DEACTIVATED_VOICE_DETECTED";
           break;
    case 6:
           strValue = "PDCP_MAX_SPS_RX_STATE";
           break;
    case 7:
           strValue = "PDCP_INVALID_SPS_RX_STATE";
           break;
    default:
           strValue = "Inv_State_Value";
    }
    return strValue;
}

const Char8 *getPDCPTxStates(UInt32 val)
{
    const Char8 *strValue="";
    switch(val) {
    case 0:
           strValue = "PDCP_TX_STATE_IDLE";
           break;
    case 1:
           strValue = "PDCP_TX_STATE_CONNECTED";
           break;
    case 2:
           strValue = "PDCP_TX_STATE_SUSPENDED";
           break;
    case 3:
           strValue = "PDCP_TX_STATE_RESUME";
           break;
    case 4:
           strValue = "PDCP_TX_STATE_HO";
           break;
    case 5:
           strValue = "PDCP_TX_STATE_AWAITED";
           break;
    case 6:
           strValue = "PDCP_TX_MAX_STATE";
           break;
    case 7:
           strValue = "PDCP_TX_INVALID_STATE";
           break;
    default:
           strValue = "Inv_State_Value";
    }
    return strValue;
}

const Char8 *getPDCPRxStates(UInt32 val)
{
    const Char8 *strValue="";
    switch(val) {
    case 0:
           strValue = "PDCP_RX_STATE_IDLE";
           break;
    case 1:
           strValue = "PDCP_RX_STATE_CONNECTED";
           break;
    case 2:
           strValue = "PDCP_RX_STATE_HO";
           break;
    case 3:
           strValue = "PDCP_RX_MAX_STATE";
           break;
    case 4:
           strValue = "PDCP_RX_INVALID_STATE";
           break;
    default:
           strValue = "Inv_State_Value";
    }
    return strValue;
}

const Char8 *getMACEvents(UInt32 val)
{
    const Char8 *strValue="";
    switch(val) {
    case 0:
           strValue = "UE_SYNC_LOSS_ON";
           break;
    case 1:
           strValue = "UE_SYNC_LOSS_OFF";
           break;
    case 2:
           strValue = "SPS_CONFIG_RECVD";
           break;
    case 3:
           strValue = "FIRST_NON_ZERO_QUEUE_LOAD";
           break;
    case 4:
           strValue = "VOICE_PACKET";
           break;
    case 5:
           strValue = "SPS_PDSCH_PUSCH_SUCCESS";
           break;
    case 6:
           strValue = "SPS_PDSCH_PUSCH_FAILURE";
           break;
    case 7:
           strValue = "FIRST_ACK_NACK";
           break;
    case 8:
           strValue = "PDCCH_CCE_ALLOC_FAILURE";
           break;
    case 9:
           strValue = "HARQ_TIMER_EXPIRED";
           break;
    case 10:
           strValue = "SILENCE_DETECTED";
           break;
    case 11:
           strValue = "MCS_CHANGED";
           break;
    case 12:
           strValue = "BLER_THRESHOLD";
           break;
    case 13:
           strValue = "PDU_SEG_IND";
           break;
    case 14:
           strValue = "MAX_DCI_ATTEMPT";
           break;
    case 15:
           strValue = "MAX_EMPTY_TX";
           break;
    case 16:
           strValue = "MAX_SPS_HARQ_RETX";
           break;
    case 17:
           strValue = "TTI_BUNDLING_CONFIG_CHANGED";
           break;
    case 18:
           strValue = "MAX_SPS_CAUSE";
           break;
    case 19:
           strValue = "SDL_PROCESSED";
           break;
    case 20:
           strValue = "OCC_PROCESSED";
           break;
    case 21:
           strValue = "INVALID_CAUSE";
           break;
    default:
           strValue = "Inv_Ev_Value";
    }
    return strValue;
}

const Char8 *getRLCEvents(UInt32 val)
{
    const Char8 *strValue="";
    switch(val) {
    case 0:
           strValue = "RLC_CTRL_NULL_EV";
           break;
    case 1:
           strValue = "RLC_RECV_MSG_FROM_RRCOAM_EV";
           break;
    case 2:
           strValue = "RLC_UE_CONTEXT_NOT_EXIST_EV";
           break;
    case 3:
           strValue = "RLC_DELETE_UE_CONTEXT_EV";
           break;
    case 4:
           strValue = "RLC_DELETE_TX_ENTITY_EV";
           break;
    case 5:
           strValue = "RLC_DELETE_RX_ENTITY_EV";
           break;
    case 6:
           strValue = "RLC_DELETE_TX_RX_ENTITY_EV";
           break;
    case 7:
           strValue = "RLC_RECONFIG_TX_ENTITY_EV";
           break;
    case 8:
           strValue = "RLC_RECONFIG_RX_ENTITY_EV";
           break;
    case 9:
           strValue = "RLC_REESTAB_ENTITY_EV";
           break;
    case 10:
           strValue = "RLC_CTRL_MAX_EV";
           break;
    default:
           strValue = "Inv_Ev_Value";
    }
    return strValue;
}

const Char8 *getPDCPSpsEvents(UInt32 val)
{
    const Char8 *strValue="";
    switch(val) {
    case 0:
           strValue = "PDCP_SPS_RRC_CONFIG";
           break;
    case 1:
           strValue = "PDCP_SPS_CONFIGURE";
           break;
    case 2:
           strValue = "PDCP_SPS_ACTIVATED";
           break;
    case 3:
           strValue = "PDCP_SPS_DEACTIVATED";
           break;
    case 4:
           strValue = "PDCP_MAX_SPS_RX_EVENT";
           break;
    case 5:
           strValue = "PDCP_INVALID_SPS_RX_EVENT";
           break;
    default:
           strValue = "Inv_Ev_Value";
    }
    return strValue;
}

const Char8 *getPDCPRxEvents(UInt32 val)
{
    const Char8 *strValue="";
    switch(val) {
    case 0:
           strValue = "PDCP_SRB_RX";
           break;
    case 1:
           strValue = "PDCP_DRB_RX";
           break;
    case 2:
           strValue = "PDCP_MAX_RX_EVENT";
           break;
    case 3:
           strValue = "PDCP_INVALID_RX_EVENT";
           break;
    default:
           strValue = "Inv_Ev_Value";
    }
    return strValue;
}

const Char8 *getPDCPTxEvents(UInt32 val)
{
    const Char8 *strValue="";
    switch(val) {
    case 0:
           strValue = "PDCP_SRB_TX";
           break;
    case 1:
           strValue = "PDCP_DRB_TX";
           break;
    case 2:
           strValue = "PDCP_SOURCE_DRB_FWD_TX";
           break;
    case 3:
           strValue = "PDCP_TARGET_DRB_FWD_TX";
           break;
    case 4:
           strValue = "PDCP_MAX_TX_EVENT";
           break;
    case 5:
           strValue = "PDCP_INVALID_TX_EVENT";
           break;
    default:
           strValue = "Inv_Ev_Value";
    }
    return strValue;
}

const Char8 *getPDCPRrcTxEvents(UInt32 val)
{
    const Char8 *strValue="";
    switch(val) {
    case 0:
           strValue = "PDCP_CONNECTED";
           break;
    case 1:
           strValue = "PDCP_SUSPENDED";
           break;
    case 2:
           strValue = "PDCP_RESUME";
           break;
    case 3:
           strValue = "PDCP_HO";
           break;
    case 4:
           strValue = "PDCP_AWAITED";
           break;
    case 5:
           strValue = "PDCP_MAX_RRC_TX_EVENT";
           break;
    case 6:
           strValue = "PDCP_INVALID_RRC_TX_EVENT";
           break;
    default:
           strValue = "Inv_Ev_Value";
    }
    return strValue;
}


void  interpet_fsm_logs(UInt32 logId,LogParamValues *pParamValues,FILE *fptr)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(logId)
    /* +- SPR 17777 */
    UInt32 i;
    Char8 *strValues,*logStr,*loglevel;
    const Char8 *logarea;
    struct tm timestamp;
    const Char8 *fsm;

    if(NULL == localtime_r(((time_t*)&pParamValues->tm), &timestamp))
    {
        fprintf(fptr, "timestamp decoding failed\n");
    }

    logStr=logCommValues[pParamValues->logId].log_str;
    loglevel= logLevelStrValues[logCommValues[pParamValues->logId].logLevel];
    logarea = getLogCategoryStrValues(logCommValues[pParamValues->logId].logCategory);

    fprintf (fptr, "[%04d-%02d-%02d/%02d:%02d:%02d.%03d]",
            timestamp.tm_year+1900, (timestamp.tm_mon + 1), timestamp.tm_mday,
            timestamp.tm_hour, timestamp.tm_min, timestamp.tm_sec,
            pParamValues->timestampUsec/1000);

    /* +- SPR 17777 */
    fprintf (fptr, "[%5u-%04u][%5s] ", (UInt32)((pParamValues->globalTick/10240)),
            (UInt32)(pParamValues->globalTick%1000), loglevel);
    i=0;
    strValues=\
              logCommValues[pParamValues->logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES];
    if(strlen(strValues))
    {
        fprintf (fptr,"[%41s] ", pParamValues->params.strValues[0]);  /*41 for MAX_LOG_STR_LEN*/
    }

    /* +- SPR 17777 */
    fprintf (fptr, "[%4d][%5s] %s ", pParamValues->logId, logarea,logStr);

    for(i=0;i<MAX_LOG_INT_VALUES;i++)
    {
        strValues=logCommValues[pParamValues->logId].strValues[i];
        if(strlen(strValues))
        {
            if(i == 2 || i == 4)
            {
                if(pParamValues->logId == MAC_DL_SPS_CURR_STATE || pParamValues->logId == MAC_UL_SPS_CURR_STATE)
                {
                    fsm = getMACStates(pParamValues->intValues[i]);
                }
                else if(pParamValues->logId == LTE_RLC_STATE_MACHINE_INFO)
                {
                    fsm = getRLCStates(pParamValues->intValues[i]);
                }
                else if(pParamValues->logId == PDCP_UE_SPS_STATE)
                {
                    fsm = getPDCPSpsStates(pParamValues->intValues[i]);
                }
                else if(pParamValues->logId == PDCP_RX_EVENT_RCVD)
                {
                    fsm = getPDCPRxStates(pParamValues->intValues[i]);
                }
                else if(pParamValues->logId == PDCP_UE_CONTEXT_STATE_TX
                         || pParamValues->logId == PDCP_TX_EVENT_RCVD)
                {
                    fsm = getPDCPTxStates(pParamValues->intValues[i]);
                }
                else
                {
                    fsm = "INVSTATE";
                }
                fprintf (fptr,"%s=%s",\
                        logCommValues[pParamValues->logId].strValues[i],\
                        fsm);
            }
            else if(i == 3)
            {
                if(pParamValues->logId == MAC_DL_SPS_CURR_STATE || pParamValues->logId == MAC_UL_SPS_CURR_STATE)
                {
                    fsm = getMACEvents(pParamValues->intValues[i]);
                }
                else if(pParamValues->logId == LTE_RLC_STATE_MACHINE_INFO)
                {
                    fsm = getRLCEvents(pParamValues->intValues[i]);
                }
                else if(pParamValues->logId == PDCP_UE_SPS_STATE)
                {
                    fsm = getPDCPSpsEvents(pParamValues->intValues[i]);
                }
                else if(pParamValues->logId == PDCP_RX_EVENT_RCVD)
                {
                    fsm = getPDCPRxEvents(pParamValues->intValues[i]);
                }
                else if(pParamValues->logId == PDCP_TX_EVENT_RCVD)
                {
                    fsm = getPDCPTxEvents(pParamValues->intValues[i]);
                }
                else if(pParamValues->logId == PDCP_UE_CONTEXT_STATE_TX)
                {
                    fsm = getPDCPRrcTxEvents(pParamValues->intValues[i]);
                }
                else
                {
                    fsm = "INVEVENT";
                }
                fprintf (fptr,"%s=%s ",\
                        logCommValues[pParamValues->logId].strValues[i],\
                        fsm);
            }
            else
            {
                fprintf (fptr,"%s=%d ",\
                        logCommValues[pParamValues->logId].strValues[i],\
                        pParamValues->intValues[i]);
            }
        }
        else
        {
            break;
        }
    }
    /*
       i=0;
       strValues=\
       logCommValues[pParamValues->logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES];
       if(strlen(strValues))
       {
       fprintf (fptr,"%s=%s ",\
       logCommValues[pParamValues->logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES],
       pParamValues->strValues[0]);
       }
     */


    i=1;
    strValues=\
              logCommValues[pParamValues->logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES];
    if(strlen(strValues))
    {
        fprintf (fptr,"%s=%s ",\
                logCommValues[pParamValues->logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES],
                pParamValues->params.strValues[1]);
    }
    fprintf (fptr,"\n");
}

/* L2_FSM_LOG End */

/*****************************************************************************
 * Function Name  : interpret_log
 * Inputs         :  
 * Outputs        : None 
 * Returns        : None
 * Description    :   
 *****************************************************************************/
void interpret_log(LogParamValues *pParamValues, FILE *fptr)
{
    UInt32 i;
    Char8 *strValues,*logStr,*loglevel;
    const Char8 *logarea;
    struct tm timestamp;

	if (PNULL == pParamValues) {
		fprintf(fptr, "paramter is NULL\n");
		return;
	}

    if((pParamValues->logId != INVALID_LOG_ID) && (pParamValues->logId <= LOG_MAX_ID))
    {
        /* MAC_TTI_LOG Start*/
        if(pParamValues->logId == MAC_TTI_DCI_LOG_INFO || pParamValues->logId == MAC_TTI_UL_CONF_LOG_INFO )
        {
            interpet_tti_logs(pParamValues->logId, pParamValues,fptr);
            return;
        }
        /* MAC_TTI_LOG End */

        /* L2 FSM logs Start*/
        if(pParamValues->logId == MAC_DL_SPS_CURR_STATE || pParamValues->logId == MAC_UL_SPS_CURR_STATE
             || pParamValues->logId == LTE_RLC_STATE_MACHINE_INFO
             || pParamValues->logId == PDCP_UE_SPS_STATE || pParamValues->logId == PDCP_UE_CONTEXT_STATE_TX
             || pParamValues->logId == PDCP_RX_EVENT_RCVD || pParamValues->logId == PDCP_TX_EVENT_RCVD)
        {
            interpet_fsm_logs(pParamValues->logId, pParamValues,fptr);
            return;
        }

        /* L2 FSM logs Ends*/

        if(MAC_PHY_SF_IND_ID == pParamValues->logId)
        {
            fprintf (fptr,"**************************************************************"\
                    "**************************************************************\n");
        }
        if(NULL == localtime_r(((time_t*)&pParamValues->tm), &timestamp))
        {
            fprintf(fptr, "timestamp decoding failed\n");
        }
        logStr=logCommValues[pParamValues->logId].log_str;
        loglevel= logLevelStrValues[logCommValues[pParamValues->logId].logLevel];
        logarea = getLogCategoryStrValues(logCommValues[pParamValues->logId].logCategory);

        /*fprintf (fptr,"%010d %-7s %-15s %s ",pParamValues->globalTick,logStr);*/
        /* SPR 6802 changes start */
        fprintf (fptr, "[%04d-%02d-%02d/%02d:%02d:%02d.%03d]",
                timestamp.tm_year+1900, (timestamp.tm_mon + 1), timestamp.tm_mday,
                timestamp.tm_hour, timestamp.tm_min, timestamp.tm_sec,
                pParamValues->timestampUsec/1000);
        /* SPR 6802 changes end */
        /* fprintf (fptr, "[% 4d-%02d][%5s][% 4d][%5s] %s ", (pParamValues->globalTick/10)%1024,
           pParamValues->globalTick%10, loglevel, pParamValues->logId, logarea,
           logStr);*/
        /* SPR 15909 fix start */
        fprintf (fptr, "[%4d, % 4d-%02d][%5s] ", pParamValues->globalTick/10240, ((UInt32)((pParamValues->globalTick/10)%1024)),
                ((UInt32)(pParamValues->globalTick%10)), loglevel);
        /* SPR 15909 fix end */

		/* add format print logs for debug usage */
		if (pParamValues->logId == LOG_MAC_FORMAT) {
			char *str_p, *log_format_p, log_format[5];
			UInt32 *p_params=(UInt32*)(&pParamValues->intValues[0]);

			str_p = &(pParamValues->params.strValues[0]);
			while(*str_p) {
			if(*str_p == '%') {
				/*Creating log format*/
				log_format_p=&log_format[0];
				*log_format_p = *str_p;
				log_format_p++;
				str_p++;

				while(isdigit(*str_p))
					str_p++;

				while(*str_p == 'l') {
					*log_format_p=*str_p;
					log_format_p++;
					str_p++;
				}
				*log_format_p=*str_p;
				log_format_p++;
				str_p++;
				*log_format_p='\0';
				/* log format creation is completed. Now print log based on log_format*/
				if(!strcmp(log_format,"%ld") ||
					!strcmp(log_format,"%lx") ||
					!strcmp(log_format,"%lu") ||
					!strcmp(log_format,"%d") ||
					!strcmp(log_format,"%x") ||
					!strcmp(log_format,"%c") ||
					!strcmp(log_format,"%f") ||
					!strcmp(log_format,"%u") ||
					!strcmp(log_format,"%p")) {

						fprintf(fptr,log_format,*((UInt32*)p_params));
						p_params += 1;

					}
				} else {
					fprintf(fptr,"%c",*str_p);
					str_p++;
				}
			}

			return;
		}

        i=0;
        strValues=\
                  logCommValues[pParamValues->logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES];
        if(strlen(strValues))
        {
            fprintf (fptr,"[%41s] ", pParamValues->params.strValues[0]);  /*41 for MAX_LOG_STR_LEN*/
        }

        fprintf (fptr, "[% 4d][%5s] %s ", pParamValues->logId, logarea,logStr);

        for(i=0;i<MAX_LOG_INT_VALUES;i++)
        {
            strValues=logCommValues[pParamValues->logId].strValues[i];
            if(strlen(strValues))
            {
                fprintf (fptr,"%s=%d ",\
                        logCommValues[pParamValues->logId].strValues[i],\
                        pParamValues->intValues[i]);
            }
            else
            {
                break;
            }
        }
        /*
           i=0;
           strValues=\
           logCommValues[pParamValues->logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES];
           if(strlen(strValues))
           {
           fprintf (fptr,"%s=%s ",\
           logCommValues[pParamValues->logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES],
           pParamValues->strValues[0]);
           }*/
        i=1;
        strValues=\
                  logCommValues[pParamValues->logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES];
        if(strlen(strValues))
        {
            fprintf (fptr,"%s=%s ",\
                    logCommValues[pParamValues->logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES],
                    pParamValues->params.strValues[1]);
        }
        fprintf (fptr,"\n");
    }
    else
    {
        fprintf(fptr, "Invalid log\n");
    }
}

/*****************************************************************************
 * Function Name  : file_print
 * Inputs         :  
 * Outputs        : None 
 * Returns        : None
 * Description    :   
 *****************************************************************************/
/* SPR 15909 fix start */
/* +- SPR 17777 */
void file_print(UInt32 logId, UInt32 logLevel,UInt64 logCategory,\
tickType_t globalTick,UInt32 intValOne,UInt32 intValTwo,\
UInt32 intValThree,UInt32 intValFour,UInt32 intValFive,\
const Char8 *strValOne,const Char8 *strValTwo)
/* +- SPR 17777 */
{
    /* SPR 15909 fix end */
    UInt32 i;
    LogParamValues paramValues;
    const Char8 *strValues,*logStr,*loglevel,*logarea;
    FILE *fptr = NULL;
    
    paramValues.logId = logId;
    /*paramValues.logLevel = logLevel;
    paramValues.logCategory = logCategory;*/
    paramValues.globalTick = globalTick;
    paramValues.intValues[0] = intValOne;
    paramValues.intValues[1] = intValTwo;
    paramValues.intValues[2] = intValThree;
    paramValues.intValues[3] = intValFour;
    paramValues.intValues[4] = intValFive;
    /*paramValues.flValues[0]  = floatValOne;
    paramValues.flValues[1] = floatValTwo;*/

    fptr = stdout;

if(logId != INVALID_LOG_ID)
{
    
    if(MAC_PHY_SF_IND_ID == logId)
    {
        fprintf (fptr,"**************************************************************"\
                      "**************************************************************\n");
    }
    logStr=logCommValues[logId].log_str;
    loglevel= logLevelStrValues[logLevel];
    logarea = getLogCategoryStrValues(logCategory);

    /* SPR 15909 fix start */
    fprintf (fptr,"%010llu %-7s %-15s %s ",paramValues.globalTick,loglevel,logarea,logStr);
    /* SPR 15909 fix end */
    for(i=0;i<MAX_LOG_INT_VALUES;i++)
    {
        strValues=logCommValues[logId].strValues[i];
        if(strlen(strValues))
        {
            fprintf (fptr,"%s=%d ",\
            logCommValues[logId].strValues[i],\
            paramValues.intValues[i]);
        }
        else
        {
            break;
        }
    }
    /*for(i=0;i<MAX_LOG_FLOAT_VALUES;i++)
    {
        strValues=logCommValues[logId].strValues[i + MAX_LOG_INT_VALUES];
        if(strlen(strValues))
        {
            fprintf (fptr,"%s=%-.2f ",\
            logCommValues[logId].strValues[i + MAX_LOG_INT_VALUES],\
            paramValues.flValues[i]);
        }
        else
        {
            break;
        }
    }*/
    i=0;
    strValues=\
              logCommValues[logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES];
    if(strlen(strValues))
    {
        fprintf (fptr,"%s=%s ",\
                logCommValues[logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES],
                strValOne);
    }
    i=1;
    strValues=\
              logCommValues[logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES];
    if(strlen(strValues))
    {
        fprintf (fptr,"%s=%s ",\
                logCommValues[logId].strValues[i + MAX_LOG_INT_VALUES + MAX_LOG_FLOAT_VALUES],
                strValTwo);
    }
    fprintf (fptr,"\n");
}
}
