/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteRelayAdapter.c,v 1.2 2010/02/18 07:46:43 gur19479 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definitions of Data APIs
 *                   : between Relay and Pdcp Adapter.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRelayAdapter.c,v $
 * Revision 1.2  2010/02/18 07:46:43  gur19479
 * updated for optimization
 *
 * Revision 1.1.2.2  2010/01/20 22:31:49  gur19479
 * updated for optimization
 *
 * Revision 1.1.2.1  2009/11/25 09:57:08  cm_lteenodeb
 * code checkin
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/


/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteRelay.h"
#include "lteRelayAdapter.h"
#include "lteRelayS1Sap.h"
#include "lteRelayGtpuSap.h"
#include "lteRelayCommon.h"
#include "lteMemPool.h"
#ifdef LTE_EMBMS_SUPPORTED
#include "lteRelayM2Sap.h"
#include "lteRelayM1uTeidSearchTree.h"
#include "lteEmbmsSyncGtpuInterface.h"
#endif
#include "lteMisc.h"
#include "lteMessageQIntf.h"
#include "lteLayer2CommanTypes.h"
#ifdef LAYER2_DLTG_THREAD
#include "lteLayer2DLTG.h"
#endif

#ifdef L2_FAST_PATH
#include "egtpu_glob.h"
#include "egtpu_msgtypes.h"
#include "egtpu_ietypes.h"
#include "egtpu_glob_struct.h"
#include "egtpu_apihdr.h"
#include "egtpu_cdb.h"
/*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
/*SPR 21188 -*/
#include "ltePdcpContext.h"
#include "ltePdcpPdcpUserSap.h"
#ifdef PDCP_ASYNC_INTERFACE
#include "ltePdcpSecEngine.h"
#endif
#else
#include "lteCommonStatsManager.h"
#endif
#endif
/*SPR 21188 +*/
#include "egtpu_msgtypes.h"
#include "egtpu_apihdr.h"

#include "egtpu_cdb.h"
#ifdef PDCP_GTPU_INTF
#include "lteCommonStatsManager.h"
#endif
/*SPR 21188 -*/
#include "lteConfigFileHdlr.h"
#ifdef ENDC_ENABLED
#include "lteX2uRelay.h"
#endif

/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

#define PDCP_HEADER_SPACE 2
#define PDCP_TRAILER_SPACE 4
#define SOCKET_ERROR (-1) /*cov 95475 +-*/


#define SOCKET_BUFFER_SIZE	(4*1024)
/******************************************************************************
  Private Definitions
 *****************************************************************************/
#ifdef L2_FAST_PATH
extern egtpu_global_t *p_global;
extern MSGPOOL MsgPool_g;
/*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
/*SPR 21188 -*/
extern PdcpContext pdcpContext;
#endif
#endif
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

/* Start of change for VARIBLE Declaration */
Char8 trafficGeneratorIP[20];
Char8 adapterIP[20];
UInt16 trafficGeneratorPort;
UInt16 pdcpAdapterRxPort;
/* End of change for VARIBLE Declaration */


/* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
void processEmbmsData( UInt8 *recvData_p, SInt16 bytesRead ,HeaderInfo *headerInfo );
/* SPR 18458 Fix Start */
extern UInt8 mcchModificationPeriodCounter(UInt8 areaIndex);
/* SPR 18458 Fix End */
#endif
/* eMBMS Code Changes End */

UInt8 ReadConfigFile(Char8* configFileName,Char8 * trafficGeneratorIP_p,UInt16* pdcpAdapterMode_g,UInt16* pdcpAdapterDelay_g);
/******************************************************************************
  Private Types
 *****************************************************************************/
#ifndef LAYER2_DLTG_THREAD 
UInt8 * dlTempMsgPtr = PNULL;
#else
  extern UInt8 * dlTempMsgPtr;
#endif
#ifdef PDCP_ASYNC_INTERFACE
#define MAX_PACKETS_TX_SEC_Q 500
#endif

#define START_OFFSET_UL_GTP 16
/*SPR 3587 Fix Start*/
#define START_OFFSET_DL_GTP 64
/*SPR 3587 Fix End*/

#define PDCP_ADAPTER_BUFFER_SIZE        2048
#define PDCP_ADAPTER_MAX_BUFFERS        5000
#define MSG_POOL_HIGH_WATERMARK         70
#define MSG_POOL_LOW_WATERMARK          60
#define MEM_POOL_HIGH_WATERMARK         70
#define MEM_POOL_LOW_WATERMARK          60
/* Fix for SPR 2008 start */
#define MAX_PACKETS_TO_SERVE             400
#define MAX_QUEUE_LOAD_TO_SERVE        40000
/* Fix for SPR 2008 end */


static ULong32 recvThId_g;
/* + Coverity 32409 */
/* static UInt8 ueContext_g[MAX_UE+1][MAX_LC+1]; */
/* - Coverity 32409 */

/*SPR 3587 Fix Start*/
SockAddr sockAddrRx_g;
/*SPR 3587 Fix End*/

/*IPV6_CHANGE - START*/
sockAddrIn6 sockAddrRx6_g;
UInt8 sockAddrType;
/*IPV6_CHANGE - End*/

/* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
SockAddr sockAddrRxEmbms_g;
sockAddrIn6 sockAddrRx6Embms_g;
SInt32 sockRxEmbms_g;
SInt32 sockRx6Embms_g;
extern TEIDSearchTree teidTree_g;
#endif
/* eMBMS Code Changes End */

static SInt32 sockTx_g;
/* SPR 14327 fix start */
/*SPR 20554 Fix Start*/
SInt32 s1uIpv4SockRx_g;
SInt32 ipv6SockRx_g;
/*SPR 20554 Fix End*/
/* SPR 14327 fix end */

LP_ConfigFileFormat configData = PNULL;

SInt32 sockRxTid_g = -1;/* Socket Descriptor for Tunnel ID reception*/

/* Table for storing the tunnel id corresponding to ue id */
adapterDataBase_t adapterDb_g[ADAPTER_MAX_UE];
SInt16 lastUeInSystem_g = -1;

/* SPR 2241 changes start */
extern Int pdcpSocketRx;
extern relayConfigInfo relayConfigInfo_g;
extern void pdcpElTxProcessSocket(UInt8 *buf,UInt32 length);
/* SPR 2241 changes end */

extern UInt8 initIndTimer_g ;

/*****************************************************************************
 * * Private Function Prototypes
 * ****************************************************************************/
static void setConfig(LP_ConfigFileFormat);
/* SPR 14327 fix start */
/* + SPR 19022 */
static UInt8 initSocketAddress(SockAddr *s,SInt32 *sockfd,char *ipAddr,UInt16 port);
static UInt8 initSocketAddress6(sockAddrIn6 *s,SInt32 *sockfd,UInt8 *ipAddr, UInt16 port);
/* - SPR 19022 */

/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

void writeToSocket(SInt8 fd, void*msgPtr, UInt16 msgLen,
        Char8 *hostname_p, UInt16 port);

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern UInt32 g_transId;
/*SPR 21366 +*/
extern UInt8 gtpu_Pdcp_Instance_g[MAX_RRC_CELL_INDEX];
/*SPR 21366 +*/
extern UInt32 processPeerGtpuDataInd(UInt8 *data_p, UInt32 trans_id, UInt32 totalLen);
UInt8 adapterInitialized_g = 0;
UInt16 pdcpAdapterMode_g = 1;
UInt16 pdcpAdapterDelay_g = 5;

extern UInt32 ReadFromSocket(UInt8 *buf, UInt16 moduleId);
extern void pdcpDataReq( UInt16 ueIndex, UInt16 lcId, UInt32  transactionId,
		void* data_p, 
        /*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
        /*SPR 21188 -*/
        void *msgBuffer_p,
#endif
        UInt16 size
        /*SPR 21366 +*/
        ,UInt8 instanceId
        /*SPR 21366 -*/
        );
extern void pdcpTargetDlDataRelocReq(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
		void *data_p, 
        /*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
        /*SPR 21188 -*/
        void *msgBuffer_p,
#endif
        UInt16 size 
        /*SPR 21366 +*/
        ,UInt8 instanceId
        /*SPR 21366 -*/
        );
extern void pdcpTargetUlDataRelocReq(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
		void *data_p, 
        /*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
        /*SPR 21188 -*/
        void *msgBuffer_p,
#endif
        UInt16 size
        /*SPR 21366 +*/
        ,UInt8 instanceId
        /*SPR 21366 -*/
        );
extern void pdcpEndMarkerInd( UInt16 ueIndex, UInt8 lcId
        /*SPR 21366 +*/
        ,UInt8 instanceId
        /*SPR 21366 -*/

        );

#if  defined (L2_FAST_PATH)
/*SPR 6837 Fix start*/
void lteUserPlaneFastPath(UInt8 *recvData_p, UInt32 bytesRead);
#endif
/*SPR 6837 Fix end*/

extern MSGPOOL MsgPool_g;

/* SPR 3085 Fix Start*/
extern UInt32 isDelayReq_g ;
/* SPR 3085 Fix End */

extern UInt16 getRrcUeIdxFromUeIdx(UInt16 ueIdx);
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

#ifndef LAYER2_DLTG_THREAD 
static UInt8 msgPoolOverFlow = 1 ;
static UInt8 memPoolOverFlow = 1;

#endif

#ifdef LTE_EMBMS_SUPPORTED
void processPeerGtpuEmbmsData(UInt8 *recvData_p,SInt16 bytesRead,HeaderInfo *headerInfo);
#endif

/* ECN changes start */
extern MQD_T fdForReceivingPdcpMsgFromRRM_g;
/* ECN changes end */


UInt32 msgPoolUsage = 0;
UInt32 memPoolUsage = 0;

/****************************************************************************
 * Functions implementation
 *****************************************************************************/

/****************************************************************************
 * Function Name  : handleRelayAdapterDataReq 
 * Inputs         : ueIndex - UE Index for Unique Identification of UE,
 *                  lcId - Logical Channel Identifier,
 *                  payloadLen - Length of payload,
 *                  gtpuMsg_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion handle Data Req from Adapter.
 ****************************************************************************/

UInt32 handleRelayAdapterDataReq
(
 UInt16 ueIndex, 
 UInt8 lcId,
 UInt16 payloadLen,
 UInt8 *gtpuMsg_p
 )
{
	UInt32 current = CSPL_HDR;
	UInt32 tunnelId = 0;
	UInt8 lcIndex = lcId - 3;
	/* To check, whether tunnel Id is created for this UE Index and Lc-Id 
	   Before accessing the actual value, it check the inUseFlag. if this
	   Flag is set, then only it will access the tunnel ID. */
	/* + Coverity 73069 */
	if ( ueIndex < RELAY_GTPU_MAX_UE && lcIndex < RELAY_GTPU_MAX_LC )
	{
		if (RELAY_GTPU_IN_USE ==
				relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].inUseFlag)
		{
			tunnelId = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId;
			gtpuMsg_p[current++] = RELAY_GTPU_IE_TEID_DATA_I;
			RELAY_GTPU_SET_U32BIT(&gtpuMsg_p[current],tunnelId);
			current = START_PAYLOAD;
			appendCsplHeader(EI_RELAY_MODULE_ID,GTPU_DATA_REQ,current+payloadLen,gtpuMsg_p);
			egtpu_process_msg(p_global, gtpuMsg_p);
			return RELAY_GTPU_SUCCESS;
		}
		else
		{
			lteWarning("Not Valid Tunnel Id is found for UE Index [%d] \
					and lcId [%d]\n",ueIndex,lcId);
		}
		return RELAY_GTPU_FAILURE;
	}
	else
	{
		return RELAY_GTPU_FAILURE;
	}
	/* - Coverity 73069 */
}

/*****************************************************************************
 * Function Name  : lteFreeEgtpuBuff
 * Inputs         : ptr1,
 *                  ptr2
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion free's GTPU Buffer.
 ****************************************************************************/
void lteFreeEgtpuBuff(void * ptr1, void * ptr2)
{
/* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(ptr1)
/* +- SPR 17777 */
    freeMemPool(ptr2);
}
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

/****************************************************************************
 * Function Name  : handleRelayAdapterDataInd
 * Inpuits        : transId - Transaction Identifier,
 *                  pdcpNo - number of PDCP,
 *                  payloadLen - Length of payload,
 *                  gtpuMsg_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion handle Data Indication Req from GTP-U.
 ****************************************************************************/
UInt32 handleRelayAdapterDataInd
(
 UInt32 transId,
 UInt16 pdcpNo,
 UInt16 payloadLen,
 UInt8 *gtpuMsg_p
 )
{
	/* Before sending packets to PDCP Adapter, it finds the UE-Index and Lc-ID
	   for this tunnel Id. For Safely sake, first it checks the inUseFlag to 
	   varify whether this structure is in use or not. */
	/* + Coverity 73069/73176 */
	if ( transId < RELAY_GTPU_MAX_TRANS_ID )
	{
		if (RELAY_GTPU_IN_USE == relayMapBasedOnTransId_g[transId].inUseFlag)
		{
#ifdef ENDC_ENABLED
                /*in case of split bearer call prX2UdlDataReq*/
                if((relayMapBasedOnTransId_g[transId].tunnelType == SCG_SPLIT_BEARER) && 
                	(relayMapBasedOnUeIndexLcId_g[relayMapBasedOnTransId_g[transId].ueIndex][relayMapBasedOnTransId_g[transId].lcId - 3].
                    dcBearerType == SN_TERMINATED_SCG_SPLIT_BEARER))
                {
                    if(RELAY_GTPU_FAILURE == prX2UdlDataReq(relayMapBasedOnTransId_g[transId].ueIndex,
                                relayMapBasedOnTransId_g[transId].lcId,
                                gtpuMsg_p,
                                payloadLen))
                    {
                        LOG_GTPUPR_WARNING(PR_GTPU_COMMON," X2U DL data Req failed");
                    }
					dlTempMsgPtr = PNULL;
					return RELAY_GTPU_SUCCESS;
                }
#endif

#if  !defined(PDCP_GTPU_INTF)
			/* Now Filling the ueIndex and LC-ID field to packet */
			UInt8* data_p = (UInt8 *)msgAlloc(PNULL, 0, 0, 0);        
			if ( PNULL == data_p ) 
			{
				/* SPR 2030 fix Start*/
				LOG_GTPU_MSG(PR_DL_DATA_MSG_ALLOC_FAIL, LOGWARNING, PR_GTPU_DL_DATA,
						relayGtpuCurrentTick_g, __LINE__, 
						0,0,0,0, 0,0, __func__,"");
				/* SPR 2030 fix End*/
				return RELAY_GTPU_FAILURE;
			}
/* +- SPR 17486 */
/*SPR 21653 changes start*/
            qvMsgInsertExternal( data_p, 0, gtpuMsg_p, payloadLen, 
                    lteFreeEgtpuBuff, dlTempMsgPtr
#ifdef CSPL_LEAK_DEBUG_LITE
                    , __func__, __LINE__
#endif
                    );
/*SPR 21653 changes end*/
#endif

            /*SPR 21366 +*/
            egtpu_rab_ctxt_t *p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(p_global,
                    relayMapBasedOnUeIndexLcId_g[relayMapBasedOnTransId_g[transId].ueIndex][relayMapBasedOnTransId_g[transId].lcId - 3].tunnelId);
            /*SPR 21366 -*/
            if(PNULL != p_rab_ctxt)
            {
			/* If tunnel type is Normal Bearer Tunnel then call pdcpDataReq API */
			if(relayMapBasedOnTransId_g[transId].tunnelType == NORMAL_BEARER)
            {
                pdcpDataReq(relayMapBasedOnTransId_g[transId].ueIndex,
                        relayMapBasedOnTransId_g[transId].lcId,
                        g_transId,
                        /*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
                        /* Actual Payload Pointer */
                        gtpuMsg_p,
                        /*SPR 21188 -*/
                        /* Pointer received from the fast socket */
                        dlTempMsgPtr,
#else
                        data_p,
#endif
                        payloadLen
                        /*SPR 21366 +*/
                        ,gtpu_Pdcp_Instance_g[p_rab_ctxt->teid_cell.cellIndex]
                        /*SPR 21366 -*/
                        );
            }
            /* If tunnel type is DL Forwarding tunnel then call pdcpTargetDlDataRelocReq API*/
            /*SPR #2716 fix Start*/
            else
				/*SPR #2716 fix End*/
				if(relayMapBasedOnTransId_g[transId].tunnelType == DL_FORWARDING)
                {

                    pdcpTargetDlDataRelocReq( relayMapBasedOnTransId_g[transId].ueIndex,
                            relayMapBasedOnTransId_g[transId].lcId,
                            pdcpNo,
                            /*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
                            /* Actual Payload Pointer */
                            gtpuMsg_p,
                            /*SPR 21188 -*/
                            /* Pointer received from the fast socket */
                            dlTempMsgPtr,
#else
                            data_p,
#endif
                            payloadLen
                           /*SPR 21366 +*/
                        ,gtpu_Pdcp_Instance_g[p_rab_ctxt->teid_cell.cellIndex]
                          /*SPR 21366 -*/

                            );

                }

            /* If tunnel type is UL Forwarding tunnel then call pdcpTargetUlDataRelocReq API*/

            /*SPR #2716 fix Start*/
                else
					/*SPR #2716 fix End*/
					if(relayMapBasedOnTransId_g[transId].tunnelType == UL_FORWARDING)
                    {

                        pdcpTargetUlDataRelocReq( relayMapBasedOnTransId_g[transId].ueIndex,
                                relayMapBasedOnTransId_g[transId].lcId,
                                pdcpNo,
                                /*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
                                /* Actual Payload Pointer */
                                gtpuMsg_p,
                                /*SPR 21188 -*/
                                /* Pointer received from the fast socket */
                                dlTempMsgPtr,
#else
                                data_p,
#endif
                                payloadLen
                           /*SPR 21366 +*/
                        ,gtpu_Pdcp_Instance_g[p_rab_ctxt->teid_cell.cellIndex]
                          /*SPR 21366 -*/

                                );
                    }


			/*SPR #2716 fix Start*/
					else
                    {
#if   PDCP_GTPU_INTF
                        freeMemPool( dlTempMsgPtr );
#else
                        msgFree(data_p);
#endif
                    }
			dlTempMsgPtr = PNULL;
            }
			/*SPR #2716 fix End*/

			return RELAY_GTPU_SUCCESS;
		}
	}
	return RELAY_GTPU_FAILURE;
	/* - Coverity 73069/73176 */
}

/****************************************************************************
 * Function Name  : processGtpuAdapterDataInd
 * Inputs         : gtpuDataReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process DATA Ind from GTP-U.
 ****************************************************************************/
UInt32 processGtpuAdapterDataInd
(
 UInt8 *gtpuDataInd_p,
 UInt16 msgLen
 )
{
	UInt32 transId  = 0;
	/* Coverity 54671 */
	UInt16 current = CSPL_HDR, pdcpNo = INVALID_PDCP_SN;
	UInt16 startPayloadLen = 0, endPayloadLen = 0, payloadLen = 0;

	if (PNULL != gtpuDataInd_p)
	{
		startPayloadLen = (gtpuDataInd_p[CSPL_HDR - 4])<<8;
		startPayloadLen |= gtpuDataInd_p[CSPL_HDR - 3];
		endPayloadLen = (gtpuDataInd_p[CSPL_HDR - 6])<<8;
		endPayloadLen |= gtpuDataInd_p[CSPL_HDR - 5];
		payloadLen = endPayloadLen - startPayloadLen+1;

		/* Coverity 54671 */
		if (RELAY_GTPU_IE_TRANSACTION_ID == gtpuDataInd_p[current])
		{ 
			current++;
			transId =  RELAY_GTPU_GET_U32BIT(&gtpuDataInd_p[current]);
            /*coverity 73176 fix start*/
            if(RELAY_GTPU_MAX_TRANS_ID < transId)
            {
                /* +- SPR 17777 */
                LOG_GTPU_MSG(PR_DL_DATA_NOT_SENT_TO_PDCP, LOGWARNING, PR_GTPU_DL_DATA,
                        relayGtpuCurrentTick_g, __LINE__, 
                        msgLen,0,0,0, 0,0, __func__,"");
                /* +- SPR 17777 */
                return RELAY_GTPU_FAILURE;
            }
            /*coverity 73176 fix end*/
			current +=4;
		}
		if (RELAY_GTPU_IE_SEQ_NUM == gtpuDataInd_p[current])
		{
			current++;
			current +=2;
		}
		if (RELAY_GTPU_IE_NPDU_NUM == gtpuDataInd_p[current])
		{
			current++;
			current++;
		}
		if (RELAY_GTPU_IE_PDCP_NUM == gtpuDataInd_p[current])
		{
			current++;
			pdcpNo = RELAY_GTPU_GET_U16BIT(&gtpuDataInd_p[current]);
			current +=2;
		}
		/* Call handleRelayAdapterDataInd fucntion for further handling */
		if ( RELAY_GTPU_SUCCESS != 
				handleRelayAdapterDataInd(transId,pdcpNo,payloadLen,&gtpuDataInd_p[startPayloadLen]))
		{
			/* SPR 2030 fix Start*/
			LOG_GTPU_MSG(PR_DL_DATA_NOT_SENT_TO_PDCP, LOGWARNING, PR_GTPU_DL_DATA,
					relayGtpuCurrentTick_g, __LINE__, 
					0,0,0,0, 0,0, __func__,"");
			/* SPR 2030 fix End*/
			return RELAY_GTPU_FAILURE;
		}
		/* SPR 2030 fix Start*/
		LOG_UT(PR_DL_DATA_SENT_TO_PDCP, LOGINFO, PR_GTPU_DL_DATA,
				relayGtpuCurrentTick_g, __LINE__, 
				0,0,0,0, 0,0, __func__,"");
		/* SPR 2030 fix End*/
		return RELAY_GTPU_SUCCESS;
	}
	/* SPR 2030 fix Start*/
	LOG_GTPU_MSG(PR_DL_DATA_EMPTY, LOGWARNING, PR_GTPU_DL_DATA,
			relayGtpuCurrentTick_g, __LINE__, 
			0,0,0,0, 0,0, __func__,"");
	/* SPR 2030 fix End*/
	return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processGtpuAdapterDataReq
 * Inputs         : gtpuDataReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process DATA Req from Adapter.
 ****************************************************************************/

UInt32 processGtpuAdapterDataReq 
(
 UInt8 *gtpuDataReq_p,
 UInt16 msgLen
 )
{
	UInt16 ueIndex = 0;
	UInt16 current = START_OFFSET_UL;
	UInt8 lcId = 0;
	UInt16 payloadLen = 0;

	if (PNULL != gtpuDataReq_p)
	{
		ueIndex = RELAY_GTPU_GET_U16BIT(&gtpuDataReq_p[current]);
		current +=2;
		lcId = gtpuDataReq_p[current++];
		payloadLen = RELAY_GTPU_GET_U16BIT(&gtpuDataReq_p[current]);
		current +=2;

		/* Call handleRelayAdapterDataReq fucntion for further handling */
		if ( RELAY_GTPU_SUCCESS != 
				handleRelayAdapterDataReq(ueIndex,lcId,payloadLen,gtpuDataReq_p))
		{

			/* SPR 2030 fix Start*/
            /* +- SPR 17777 */
			LOG_GTPU_MSG(PR_UL_DATA_NOT_SENT_TO_GTPU, LOGWARNING, PR_GTPU_UL_DATA,
					relayGtpuCurrentTick_g, __LINE__, 
					lcId,msgLen,0,0, 0,0, __func__,"");
            /* +- SPR 17777 */
			/* SPR 2030 fix End*/

			return RELAY_GTPU_FAILURE;
		}
		/* SPR 2030 fix Start*/
		LOG_UT(PR_UL_DATA_SENT_TO_GTPU, LOGDEBUG, PR_GTPU_UL_DATA,
				relayGtpuCurrentTick_g, __LINE__, 
				lcId,0,0,0, 0,0, __func__,"");
		/* SPR 2030 fix End*/
		return RELAY_GTPU_SUCCESS;
	}
	/* SPR 2030 fix Start*/
	LOG_GTPU_MSG(PR_UL_DATA_EMPTY, LOGWARNING, PR_GTPU_UL_DATA,
			relayGtpuCurrentTick_g, __LINE__, 
			lcId,0,0,0, 0,0, __func__,"");
	/* SPR 2030 fix End*/
	return RELAY_GTPU_FAILURE;
}


/****************************************************************************
 * Function Name  : handleRelayAdapterDlDataRelocReq 
 * Inputs         : ueIndex - UE Index for Unique Identification of UE,
 *                  lcId - Logical Channel Identifier,
 *                  payloadLen - Length of payload,
 *                  gtpuMsg_p,
 *                  pdcpSeqNo - PDCP Sequence No
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion handle Data Req from Adapter.
 ****************************************************************************/

UInt32 handleRelayAdapterDlDataRelocReq
(
 UInt16 ueIndex, 
 UInt8 lcId,
 UInt16 payloadLen,
 UInt8 *gtpuMsg_p,
 UInt16 pdcpSeqNo
 )
{
	UInt32 current = CSPL_HDR;
	UInt32 tunnelId = 0;
	UInt8 lcIndex = lcId - 3;
	/* To check, whether DL FWD tunnel Id is created for this UE Index 
	 * amd LcId Before accessing the actual value. 
	 */
	if (RELAY_GTPU_IN_USE ==
			relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].inUseFlag && 
			relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId != INVALID_TUNNEL_ID)
	{
		/* Fill DL FWD tunnel id */
		tunnelId = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId;
		gtpuMsg_p[current++] = RELAY_GTPU_IE_TEID_DATA_I;
		RELAY_GTPU_SET_U32BIT(&gtpuMsg_p[current],tunnelId);
		current += 4; 

		/* If PDCP sequence no is valid then fill sequence no */
		if( pdcpSeqNo != INVALID_PDCP_SN )
		{
			gtpuMsg_p[current++] = RELAY_GTPU_IE_PDCP_NUM;
			RELAY_GTPU_SET_U16BIT(&gtpuMsg_p[current],pdcpSeqNo);
		}
		else
		{
			gtpuMsg_p[current++] = 0;
			RELAY_GTPU_SET_U16BIT(&gtpuMsg_p[current],0);
			current += 2; 
		}

		/* Fill payload part */
		current = START_PAYLOAD;
		appendCsplHeader(EI_RELAY_MODULE_ID,GTPU_RELOC_DATA_REQ,current+payloadLen,gtpuMsg_p);
		egtpu_process_msg(p_global, gtpuMsg_p);
		return RELAY_GTPU_SUCCESS;
	}
	else
	{
		LOG_GTPU_MSG(PR_DL_DATA_NOT_SENT_TO_PDCP, LOGINFO, PR_GTPU_DL_DATA,
				relayGtpuCurrentTick_g, __LINE__, 
				ueIndex,lcId,0,0, 0,0, __func__,"");
	}
	return RELAY_GTPU_FAILURE;
}


/****************************************************************************
 * Function Name  : processGtpuAdapterDlDataRelocReq
 * Inputs         : gtpuDataReq_p,
 *                  pdcpSeqNo - PDCP Sequence No
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process DL DATA RELOC Req from Adapter.
 ****************************************************************************/

UInt32 processGtpuAdapterDlDataRelocReq 
(
 UInt8 *gtpuDataReq_p,
 UInt16 pdcpSeqNo
 )
{
	UInt16 ueIndex = 0;
	UInt16 current = START_OFFSET_UL;
	UInt8 lcId = 0;
	UInt16 payloadLen = 0;

	if (PNULL != gtpuDataReq_p)
	{
		ueIndex = RELAY_GTPU_GET_U16BIT(&gtpuDataReq_p[current]);
		current +=2;
		lcId = gtpuDataReq_p[current++];
		payloadLen = RELAY_GTPU_GET_U16BIT(&gtpuDataReq_p[current]);
		current +=2;

		/* Call handleRelayAdapterDlDataRelocReq function for further handling */
		if ( RELAY_GTPU_SUCCESS != 
				handleRelayAdapterDlDataRelocReq(ueIndex,lcId,payloadLen,gtpuDataReq_p,pdcpSeqNo))
		{

			LOG_GTPU_MSG(PR_UL_DATA_NOT_SENT_TO_GTPU, LOGWARNING, PR_GTPU_UL_DATA,
					relayGtpuCurrentTick_g, __LINE__, 
					lcId,0,0,0, 0,0, __func__,"");

			return RELAY_GTPU_FAILURE;
		}
		
		LOG_GTPU_MSG(PR_UL_DATA_SENT_TO_GTPU, LOGINFO, PR_GTPU_UL_DATA,
				relayGtpuCurrentTick_g, __LINE__,
				lcId,0,0,0, 0,0, __func__,"");
		return RELAY_GTPU_SUCCESS;
	}
	LOG_GTPU_MSG(PR_UL_DATA_EMPTY, LOGWARNING, PR_GTPU_UL_DATA,
			relayGtpuCurrentTick_g, __LINE__, 
			lcId,0,0,0, 0,0, __func__,"");

	return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : handleRelayAdapterUlDataRelocReq 
 * Inputs         : ueIndex - UE Index for Unique Identification of UE,
 *                  lcId - Logical Channel Identifier,
 *                  payloadLen - Length of payload,
 *                  gtpuMsg_p,
 *                  pdcpSeqNo - PDCP Sequence No
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion handle UL RELOC Data Req from Adapter.
 ****************************************************************************/

UInt32 handleRelayAdapterUlDataRelocReq
(
 UInt16 ueIndex, 
 UInt8 lcId,
 UInt16 payloadLen,
 UInt8 *gtpuMsg_p,
 UInt16 pdcpSeqNo
 )
{
	UInt32 current = CSPL_HDR;
	UInt32 tunnelId = 0;
	UInt8 lcIndex = lcId - 3;
	/* To check, whether UL FWD tunnel Id is created for this UE Index 
	 * amd LcId Before accessing the actual value. 
	 */
    /* Coverity 73187 Fix Start*/
    if(ueIndex >= RELAY_GTPU_MAX_UE)
    {
        return RELAY_GTPU_FAILURE;
    }
    /* Coverity 73187 Fix End*/
	if (RELAY_GTPU_IN_USE ==
			relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].inUseFlag && 
			relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelUlFwdId != INVALID_TUNNEL_ID)
	{
		/* Fill UL FWD tunnel id */
		tunnelId = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelUlFwdId;
		gtpuMsg_p[current++] = RELAY_GTPU_IE_TEID_DATA_I;
		RELAY_GTPU_SET_U32BIT(&gtpuMsg_p[current],tunnelId);
		current += 4;

		/* If PDCP sequence no is valid then fill sequence no */
		if( pdcpSeqNo != INVALID_PDCP_SN )
		{
			gtpuMsg_p[current++] = RELAY_GTPU_IE_PDCP_NUM;
			RELAY_GTPU_SET_U16BIT(&gtpuMsg_p[current],pdcpSeqNo);
		}
		else
		{
			gtpuMsg_p[current++] = 0;
			RELAY_GTPU_SET_U16BIT(&gtpuMsg_p[current],0);
			current += 2;
		}


		/* Fill payload part */
		current = START_PAYLOAD;
		appendCsplHeader(EI_RELAY_MODULE_ID,GTPU_RELOC_DATA_REQ,current+payloadLen,gtpuMsg_p);
		egtpu_process_msg(p_global, gtpuMsg_p);
		return RELAY_GTPU_SUCCESS;
	}
	else
	{
		lteWarning("Not Valid UL FWD Tunnel Id is found for UE Index [%d] \
				and lcId [%d]\n",ueIndex,lcId);
	}
	return RELAY_GTPU_FAILURE;
}


/****************************************************************************
 * Function Name  : processGtpuAdapterUlDataRelocReq
 * Inputs         : gtpuDataReq_p,
 *                  pdcpSeqNo - PDCP Sequence No
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process UL DATA RELOC Req from Adapter.
 ****************************************************************************/

UInt32 processGtpuAdapterUlDataRelocReq 
(
 UInt8 *gtpuDataReq_p,
 UInt16 pdcpSeqNo
 )
{
	UInt16 ueIndex = 0;
	UInt16 current = START_OFFSET_UL;
	UInt8 lcId = 0;
	UInt16 payloadLen = 0;

	if (PNULL != gtpuDataReq_p)
	{
		ueIndex = RELAY_GTPU_GET_U16BIT(&gtpuDataReq_p[current]);
		current +=2;
		lcId = gtpuDataReq_p[current++];
		payloadLen = RELAY_GTPU_GET_U16BIT(&gtpuDataReq_p[current]);
		current +=2;

		/* Call handleRelayAdapterUlDataRelocReq function for further handling */
		if ( RELAY_GTPU_SUCCESS != 
				handleRelayAdapterUlDataRelocReq(ueIndex,lcId,payloadLen,gtpuDataReq_p,pdcpSeqNo))
		{

			LOG_GTPU_MSG(PR_UL_DATA_NOT_SENT_TO_GTPU, LOGWARNING, PR_GTPU_UL_DATA,
					relayGtpuCurrentTick_g, __LINE__, 
					lcId,0,0,0, 0,0, __func__,"");

			return RELAY_GTPU_FAILURE;
		}
		LOG_GTPU_MSG(PR_UL_DATA_SENT_TO_GTPU, LOGINFO, PR_GTPU_UL_DATA,
				relayGtpuCurrentTick_g, __LINE__, 
				lcId,0,0,0, 0,0, __func__,"");

		return RELAY_GTPU_SUCCESS;
	}
	LOG_GTPU_MSG(PR_UL_DATA_EMPTY, LOGWARNING, PR_GTPU_UL_DATA,
			relayGtpuCurrentTick_g, __LINE__, 
			lcId,0,0,0, 0,0, __func__,"");

	return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processGtpuAdapterEndMarkerInd
 * Inputs         : gtpuEndMarkerInd_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion sends End Marker Ind to PDCP.
 ****************************************************************************/
UInt32 processGtpuAdapterEndMarkerInd 
(
 UInt8 *gtpuEndMarkerInd_p,
 UInt16 msgLen
 )
{
	UInt16 ueIndex = 0;
	UInt16 current = API_HDR;
	UInt8 lcId = 0;
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(msgLen)
    /* +- SPR 17777 */

	if (PNULL != gtpuEndMarkerInd_p)
    {
        ueIndex = RELAY_GTPU_GET_U16BIT(&gtpuEndMarkerInd_p[current]);
        current +=2;
        lcId = gtpuEndMarkerInd_p[current++];
        /*SPR 21366 +*/
        if(MAX_UE_SUPPORTED > ueIndex )
        {
            egtpu_rab_ctxt_t *p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(p_global,
                    relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].tunnelId);
            /*SPR 21366 -*/

            /* Send EMI to PDCP */
            if(PNULL != p_rab_ctxt)
            {
                pdcpEndMarkerInd(ueIndex,lcId
                        /*SPR 21366 +*/
                        ,gtpu_Pdcp_Instance_g[p_rab_ctxt->teid_cell.cellIndex]
                        /*SPR 21366 -*/

                        );
            }
        }
        return RELAY_GTPU_SUCCESS;
    }

	lteWarning("Empty End Marker Ind Msg to PDCP");        
	return RELAY_GTPU_FAILURE;
}


/****************************************************************************
 * Function Name  : processAdapterEndOfDataFwdInd
 * Inputs         : ueIndex,
 *                  lcId,
 *                  isEndMarkerIndRecvd
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion receives EndOfDataFwdInd from PDCP Adapter,
 *                  and sends the End Marker Packet to Peer eGTPu stack.
 *                  It creates the GTP Header for End Marker Packet.
 ****************************************************************************/

void processAdapterEndOfDataFwdInd 
(
 UInt16 ueIndex,
 UInt8  lcId,
 /* SPR 3085 Fix Start */
 UInt8 isEndMarkerIndRecvd
 /* SPR 3085 Fix End */
 )
{
	UInt8 *gtpuEmiReq = PNULL;
	gtpuEmiReq = qvMsgAllocBySize (MAX_API_LEN + CSPL_HDR);
	UInt32 current = CSPL_HDR;
	UInt8 lcIndex = lcId - 3;
    /* SPR 15974 fix start */
    UInt8 lcCtr = 0;
    UInt8 deleteUeFlag = LTE_TRUE;
    /* SPR 15974 fix end */
	/* +COVERITY 32127 */
	if (gtpuEmiReq)
	{
		if(RELAY_GTPU_IN_USE ==
				relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].inUseFlag)
		{
			/* SPR 6758 changes start */
			relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].isLocalEndMarkerIndRecvd = LTE_TRUE;
			/* SPR 6758 changes end */
			/* SPR 3085 Fix Start */
			if (1 == isEndMarkerIndRecvd )
			{
				/* SPR 3085 Fix End */
				RELAY_GTPU_SET_U32BIT(&gtpuEmiReq[current],
						relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId);
				current += 4;

				LOG_GTPU_MSG(PR_RCVD_END_OF_DATA_FWD_IND, LOGINFO, PR_GTPU_CONTROL,
						relayGtpuCurrentTick_g, __LINE__,ueIndex,lcId,0,0, 0,0, 
						__func__,"");

				/*Send API to get TEID-Peer+ Peer Transport Address Info from eGTPu*/
				appendCsplHeader(EI_RELAY_MODULE_ID,GTPU_SEND_EMI_TO_PEER_REQ,
						current,gtpuEmiReq);
				egtpu_process_msg(p_global, gtpuEmiReq);
				/* SPR 3085 Fix Start */
			}
			/* SPR 5481 Fix Start */
            /* Check if delete Request has been received from Layer3 and DL 
             * forwarding tunnels are created on source GTP */
			if (relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].EmiFlag)
            { 
                /* Reset the flags to default value since tunnels are going to 
                 * be deleted */
                /* SPR 16001 fix start */
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].isLocalEndMarkerIndRecvd = 
                    LTE_FALSE;
                /* SPR 16001 fix end */
                /* +- SPR 15974 */
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].EmiFlag = 
                    LTE_FALSE;
            }
            /* SPR 15974 fix start */
            else
            {
                /* Since delete request is not yet received hence set this flag
                 * to FALSE so that UE is not deleted */
                deleteUeFlag = LTE_FALSE;
            }
            for (lcCtr = 0; lcCtr < RELAY_GTPU_MAX_LC; lcCtr++)
            {
                if( LTE_TRUE == 
                        relayMapBasedOnUeIndexLcId_g[ueIndex][lcCtr].EmiFlag )
                {
                    /* Local End marker is not yet received from PDCP for 
                     * All LCs of the UE */
                     deleteUeFlag = LTE_FALSE;
                     break;
                }
            }
            /* trigger DeleteUE only when for all the LCs local EMI is 
             * received */
            if( LTE_TRUE == deleteUeFlag )
            {
                triggerDeleteUeEntityReq(ueIndex);
            }
            /* SPR 15974 fix end */
			/* SPR 5481 Fix End */
			/* SPR 3085 Fix End */
		}
		else
		{
			LOG_GTPU_MSG(PR_RCVD_END_OF_DATA_FWD_IND_INV_RAB, LOGWARNING, PR_GTPU_CONTROL,
					relayGtpuCurrentTick_g, __LINE__,ueIndex,lcId,0,0, 0,0, 
					__func__,"");
		}
		msgFree( gtpuEmiReq );
	}
	/* +COVERITY 32127 */
}

/* SPR 3085 Fix Start */
/**************************************************************************************
 * Function Name  : triggerDeleteUeEntityReq
 * Inputs         : ueIndex
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This Function will Trigger Delete UE-Entity Req based on ueIndex 
 *                  and lcIndex and Update UE Context Management Based upon Tunnel-ID.
 **************************************************************************************/
/* SPR 5481 Fix Start */
int triggerDeleteUeEntityReq(UInt16 ueIndex)
	/* SPR 5481 Fix End */
{
	relayS1RelSapReq releaseReq = {0};
	UInt8 respMsg[20] = {0}, cause = GTPU_CAUSE_SUCCESS;
	UInt8 lcIndex =0;
	/* SPR 5481 Fix Start */
	UInt32 transIdForRsp = 0;
	/* SPR 5481 Fix End */
	UInt16 current = 0;
	UInt16 startVal = 0;
	UInt16 apiIdForRsp = S1U_DELETE_UE_ENTITY_CNF;

	/** SPR 11111 Changes Start **/
	/** Fetch the rrcUeIndex From Local UeLc Table because at this point of time, 
	  ueIdxToRrcUeIdx table might have been updated and marked invalid by other layers. 
	  it is assumed that LCID=3 is always present as the ue index is being fetched here 
	  otherwise run the loop ang get the value where the inUseFlag is set **/

	UInt16 rrcUeIndex = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].rrcUeIndex;
	/** SPR 11111 Changes End **/

	for (lcIndex = 0; lcIndex < MAX_RAB_ID; lcIndex++)
	{
		if(INVALID_TUNNEL_ID !=
				relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId)
			releaseReq.teidCount++;
		if(INVALID_TUNNEL_ID !=
				relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId)
		{
			releaseReq.teidCount++;
			/* SPR 5481 Fix Start */
			transIdForRsp = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].transIdRsp;
			/* SPR 5481 Fix End */
		}
		if(INVALID_TUNNEL_ID !=
				relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelUlFwdId)
			releaseReq.teidCount++;
		relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].inUseFlag =
			RELAY_GTPU_NOT_IN_USE;
		for (startVal = 0; startVal < releaseReq.teidCount; startVal++)
		{
			if (RELAY_GTPU_FAILURE ==
					updateRelayUeCntxMngmntBasedOnTunnelId(
						&releaseReq.relSapReq[startVal],
						lcIndex,transIdForRsp,apiIdForRsp,
						ueIndex,releaseReq.teidCount,startVal))
			{
				LOG_GTPU_MSG(PR_GTPU_SAP_REL_FAILURE, LOGWARNING,
						PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__,
						ueIndex, 0,0,0,0,0,__func__,"");
				isDelayReq_g = RELAY_GTPU_IN_USE;
				return RELAY_GTPU_FAILURE;
			}
		}
		releaseReq.teidCount = 0;
	}
	/* SPR 5978 Fix Start */
	current = API_HDR; 
	RELAY_GTPU_SET_U16BIT(&respMsg[current], rrcUeIndex);
	/* SPR 5978 Fix End */
	current += 2;
    /*CID 94088 Fix satrt*/
	respMsg[current++] = cause;  /* Cause */
    /*CID 94088 Fix end*/

    /* + SPR 12979 */
    /* UE is deleted, hence free from the global map */
    if( 0 == setUeIdxInvalid( ueIndex, UE_IDX_MAPPING_FOR_S1U) )
    {
        freeUeIdx( ueIndex );
    }
    /* - SPR 12979 */
	LOG_GTPU_MSG(PR_SEND_S1U_DEL_UE_ENTITY_CNF,LOGINFO,PR_GTPU_CONTROL,
			relayGtpuCurrentTick_g,__LINE__,ueIndex,
			cause,
			0,0, 0,0, __func__, "");

	appendApiHeader(RELAY_S1AP_MODULE_ID, apiIdForRsp, transIdForRsp, 
			current, respMsg,s1uUEReconfigCnfInfo_g[ueIndex].cellIndex);

	udpSendToS1ap((char*)respMsg, current);

	return RELAY_GTPU_SUCCESS;
}
/* SPR 3085 Fix End */


#ifndef LAYER2_DLTG_THREAD

/**************************************************************************************
 * Function Name  : processPeerDataInd
 * Inputs         : recvBuff_p - pointer to the buffer received,
 *                  sockByteRead - number of bytes read on socket
 *                  transactionId - Transaction Id. It will be same as
 *                                  received in data request for which
 *                                  delivery status need to be sent.
 * Outputs        : None
 * Returns        : None
 * Description    : This Function will Process Peer Data.
 *************************************************************************************/
void processPeerDataInd(UInt8 * recvBuff_p, UInt16 sockByteRead
#ifndef L2_FAST_PATH 
		,UInt32 transactionId 
#endif
		)
{

#ifdef L2_FAST_PATH /* Undefine this */
	lteUserPlaneFastPath(recvBuff_p, sockByteRead + START_OFFSET_DL_GTP);
    /* +- SPR 15194 */
#else
	/*Review comments incorporated*/
	processPeerGtpuDataInd(recvBuff_p,
			transactionId++,
			sockByteRead+START_OFFSET_DL_GTP);
#endif
}
/*SPR 20554 Fix Start*/
/**************************************************************************************
 * Function Name  : checkAndReturnDataRead 
 * Inputs         : recvBuff_p - pointer to the buffer received,
 *                  bytesRead - number of bytes read on socket
 * Outputs        : None
 * Returns        : None
 * Description    : This Function will return data pointer if received data 
 *                  else free the pointer and retunr NULL
 *************************************************************************************/
UInt8 * checkAndReturnDataRead(UInt8 *recvData_p,SInt32 bytesRead)
{
    if ( -1 == bytesRead )
    {
        if ( errno != EAGAIN )
        {
            /*SPR 2030 fix Start*/
            LOG_GTPU_MSG(L2_SOCKET_RECVFROM_FAIL_ID,LOGWARNING, L2_SYS_FAIL,
                    relayGtpuCurrentTick_g,__LINE__,0,0,0,0,
                    0,0,__func__,"");
            /*SPR 2030 fix End*/
            freeMemPool(recvData_p);
            /* SPR 9749 Fix Start */
            dlTempMsgPtr = PNULL;
            /* SPR 9749 Fix End */
            ltePanic ("Error in receive message.\n");
        }
        else
        {
            freeMemPool(recvData_p);
            /* SPR 9749 Fix Start */
            dlTempMsgPtr = PNULL;
            /* SPR 9749 Fix End */
            return PNULL;
        }
    } 
    /*Memory usage has hit high water mark, drop the packet*/
    if ((1 == msgPoolOverFlow) || (1 == memPoolOverFlow)
       )
    {
        /* LOG_GTPU_MSG(PR_MSG_POOL_HIGH_WATERMARK, LOGERROR,PR_GTPU_DL_DATA,
           getCurrentTick() ,msgPoolUsage,0,0,0,0,
           0,0,__func__,"Stopping traffic"); */
        lteLogDropedPkt("Dropping packets watermark = %d\n", msgPoolUsage);
        freeMemPool(recvData_p);
        dlTempMsgPtr = PNULL;
        return PNULL;
    }
    if (bytesRead > 0)
    {
        dlTempMsgPtr = recvData_p;
        return recvData_p;
    }
    else
    {
        LOG_GTPU_MSG(PR_ADAPTER_LESS_BYTES_RCVD, LOGFATAL, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__,
                bytesRead, 0,0,0,0,0, __func__,
                "" );
        freeMemPool(recvData_p);
        dlTempMsgPtr = PNULL;
    }
	return PNULL;
}
/*SPR 20554 Fix End*/

/******************************************************************************
 * Function Name  : receiveUserData
 * Inputs         : pdcpTxData 
 *                  pdcpTxByteRead 
 *                  relaySockData 
 *                  relaySockByteRead,
 *                  sockByteRead,
 *                  pdcpRrmData,
 *                  pdcpRrmDataSize
 * Outputs        : None
 * Returns        : pointer to the buffer received 
 * Description    : This Function will  receive the USER data 
 ****************************************************************************/
/*SPR 20554 Fix Start*/
UInt8 * receiveUserData(UInt8 *pdcpTxData, SInt16 *pdcpTxByteRead, 
		UInt8 *relaySockData,SInt16 *relaySockByteRead, UInt8 *isIpv4Ipv6DataReceived,
		UInt8 *pdcpRrmData,SInt16 * pdcpRrmDataSize
#ifndef L2_FAST_PATH 
                                ,UInt32  *transactionId
#endif

/* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
        ,SInt16 *eMbmsBytesRead,
        HeaderInfo *hdrInfo
#endif
                                )
/* eMBMS Code Changes End */
{
    /*SPR 20554 Fix End*/
    /* To recv data from socklet */
    SInt32 bytesRead = -1;
    UInt8 *recvData_p = PNULL;
    /*SPR 20554 Fix Start*/
    UInt8 *recvIpv4Data_p = PNULL;
    UInt8 *recvIpv6Data_p = PNULL;
    /*SPR 20554 Fix End*/

    SInt32          selectReturn;
    fd_Set          fdSock;
    SInt32          highFd = 0;
#ifdef LTE_EMBMS_SUPPORTED
    UInt8 *eMbmsSockData=PNULL;
/* SPR 18458 Fix Start */
    UInt8 areaCount=0;
    UInt8 nodeCount=0;
    mcchMapAreaIdNode *mcchMapAreaIdNode_p = PNULL;
/* SPR 18458 Fix End */
#endif
/*SPR 21653 changes start*/
    /* + SPR 19022 */
    /*SPR 20554 Fix Start*/
    SockAddr* sgwToIpv4Addr_p = PNULL;
    SInt32 sgwToIpv4Addr_len = 0;
    SInt32 sgwToIpv6Addr_len = 0;
#ifndef L2_FAST_PATH 
    UInt32 localTransId=*transactionId;
#endif
    SockAddr*  sgwToIpv6Addr_p = PNULL;
    /*SPR 20554 Fix End*/
    /* - SPR 19022 */
/*SPR 21653 changes end*/
    timeVal    wTime;
#ifdef PDCP_ASYNC_INTERFACE
    /* SPR 14546 fix start */
    UInt32 sQCount = QCOUNT_PDCP_SEC_INPUT_TX_Q();
    /* SPR 14546 fix end */
    if ( MAX_PACKETS_TX_SEC_Q < sQCount )
    {
        LOG_GTPU_MSG(PR_SEC_Q_FULL, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__, sQCount, 0,0,0, 0,0,
                __func__,"Stopping traffic");
        lteLogDropedPkt("Number of packets in SEC TX queue = %d\n", sQCount);
        return PNULL;
    }
#endif

/*SPR 21653 changes start*/
    /* FD for listening the Tid Info */
    lteFD_ZERO( &fdSock );
    /* + SPR 19022 */
    /*SPR 20554 Fix Start*/
    if (0 < s1uIpv4SockRx_g)
    {
        lteFD_SET(s1uIpv4SockRx_g, &fdSock );
        if (s1uIpv4SockRx_g > highFd) highFd = s1uIpv4SockRx_g;
        sgwToIpv4Addr_p = &relayConfigInfo_g.sgwToAddr;
        sgwToIpv4Addr_len = sizeof(relayConfigInfo_g.sgwToAddr);
    }
    if (0 < ipv6SockRx_g)
    {
        lteFD_SET(ipv6SockRx_g, &fdSock);
        if (ipv6SockRx_g > highFd) highFd = ipv6SockRx_g;
        sgwToIpv6Addr_p = (SockAddr *)&relayConfigInfo_g.sgwToAddrIn6;
        sgwToIpv6Addr_len = sizeof(relayConfigInfo_g.sgwToAddrIn6);
    }
    /* - SPR 19022 */
/*SPR 21653 changes end*/
    if(0 == initIndTimer_g)
    {
       /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
        /*SPR 21188 -*/
        lteFD_SET(pdcpSocketRx,&fdSock);if (pdcpSocketRx > highFd) highFd = pdcpSocketRx;
#endif
        lteFD_SET(relayConfigInfo_g.s1ApTxFd,&fdSock);
        if (relayConfigInfo_g.s1ApTxFd > highFd) highFd = relayConfigInfo_g.s1ApTxFd;
    }


   /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
        /*SPR 21188 -*/
    if (fdForReceivingPdcpMsgFromRRM_g > 0)
    {
        lteFD_SET(fdForReceivingPdcpMsgFromRRM_g,&fdSock);
        if(fdForReceivingPdcpMsgFromRRM_g > highFd) 
        {
            highFd = fdForReceivingPdcpMsgFromRRM_g;
        }
    } 
#else

if (relayConfigInfo_g.gtpuRxFd > 0)
{
	 lteFD_SET(relayConfigInfo_g.gtpuRxFd,&fdSock);

	 if (relayConfigInfo_g.gtpuRxFd > highFd)
	 {
	 	highFd = relayConfigInfo_g.gtpuRxFd;
	 }
}
#endif
        /* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
    if (0 < sockRxEmbms_g)
    {
	lteFD_SET(sockRxEmbms_g, &fdSock );
	if (sockRxEmbms_g > highFd) 
	    highFd = sockRxEmbms_g;
    }

    if (0 < sockRx6Embms_g)
    {
       lteFD_SET(sockRx6Embms_g, &fdSock);
       if (sockRx6Embms_g > highFd) 
	    highFd = sockRx6Embms_g;
    }
#endif
        /* eMBMS Code Changes End */


    /* Time Out value 1 micro second */
    wTime.tv_sec  = 0;

    wTime.tv_usec = 200;

        /* SPR 18739 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
        /* SPR 18458 Fix Start */
        for(areaCount=0; areaCount<MAX_NUM_EMBMS_AREA_INDEX; areaCount++)
        {

            UInt8  mcchupdateTime =  mcchModificationPeriodCounter(areaCount);
            LTE_QUEUE* mcchMapAreaIdQueue_p=&(relayMcchMapIndex_g[areaCount][mcchupdateTime].mcchMapAreaIdQueue);
            nodeCount=queueCount(mcchMapAreaIdQueue_p);
            if(nodeCount)
            {
                deleteM1uAreaLcNode(areaCount);
            }    
            while(nodeCount)
            {
                mcchMapAreaIdNode_p = (mcchMapAreaIdNode *)DEQUEUE_AREA_NODE(mcchMapAreaIdQueue_p);
                if (PNULL == mcchMapAreaIdNode_p)
                {
                    /*No data is left on index to schedule */
                    break;
                }
                if(mcchMapAreaIdNode_p->lcId != MAX_EMBMS_LCID)
                {    
                    addM1uAreaLcNode(areaCount,mcchMapAreaIdNode_p);
                }
                freeMemPool(mcchMapAreaIdNode_p);
                mcchMapAreaIdNode_p= PNULL;
                nodeCount--;
            }
        }
        /* SPR 18458 Fix End */
#endif	
        /* SPR 18739 Fix End */

    while( (selectReturn = select_wrapper( highFd + 1, &fdSock, 0, 0, &wTime )) < 0)
    {
        if (errno == EINTR)
            continue;
        LOG_GTPU_MSG(PR_MU_ADAPTER_GLUE_SELECT_FAILED,LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g,__LINE__,0,0,0,0,0,0,__func__,"");
        pError("MU_Adapter Glue Select Failed!");
        return PNULL;
    }

    if(!selectReturn)
    {
        return PNULL;
    }
    /*SPR 20554 Fix Start*/
/*SPR 21653 changes start*/
    /*SPR 20554 Fix End*/
    /*SPR 20554 Fix End*/
        /* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
     if (lteFD_ISSET(sockRxEmbms_g,&fdSock))
     {
	 eMbmsSockData = (UInt8 *)getMemFromPool(BUFFERSIZE,PNULL);
	 if ((1 == msgPoolOverFlow) || (1 == memPoolOverFlow))

	 {
	    lteLogDropedPkt("Dropping packets watermark = %d\n", msgPoolUsage);
	    freeMemPool(eMbmsSockData);
	    return PNULL;
	 }
	 if(PNULL == eMbmsSockData)
	 {
	    ltePanic("%s: can't allocate buffer",__FUNCTION__);
	 }
	 *eMbmsBytesRead = recvMsg_wrapper(sockRxEmbms_g,&eMbmsSockData[START_OFFSET_DL_GTP],(BUFFERSIZE - START_OFFSET_DL_GTP ),hdrInfo,IPV4_FLAG);
	 lteFD_CLR(sockRxEmbms_g,&fdSock);
	 if((*eMbmsBytesRead) > 0)
	 {
	    return eMbmsSockData;
	 }
	 else
	 {
    	    freeMemPool(eMbmsSockData);
	    return PNULL;
	    /* Coverity 93757 Fix */
	 }
    }

    if(lteFD_ISSET(sockRx6Embms_g,&fdSock))
    {
	eMbmsSockData = (UInt8 *)getMemFromPool(BUFFERSIZE,PNULL);
	if(PNULL == eMbmsSockData)
	{
	   ltePanic("%s: can't allocate buffer",__FUNCTION__);
	}
	*eMbmsBytesRead = recvMsg_wrapper(sockRx6Embms_g,&eMbmsSockData[START_OFFSET_DL_GTP], ( BUFFERSIZE - START_OFFSET_DL_GTP ),hdrInfo,IPV6_FLAG);
	lteFD_CLR(sockRx6Embms_g,&fdSock);
	if((*eMbmsBytesRead) > 0)
	{
	   return eMbmsSockData;
	}
	else
	{
	   freeMemPool(eMbmsSockData);
	   return PNULL;
	   /* Coverity 93757 Fix */
	}
    }
#endif
        /* eMBMS Code Changes End */

    if (dlTempMsgPtr != PNULL)
    {
        freeMemPool(dlTempMsgPtr);
        dlTempMsgPtr = PNULL;
    }

    recvData_p = (UInt8 *)getMemFromPool(BUFFERSIZE,PNULL);
    if(PNULL == recvData_p)
    {
        ltePanic("%s: can't allocate buffer",__FUNCTION__);
    }
    dlTempMsgPtr = recvData_p;
/*SPR 21653 changes end*/

    if(0 == initIndTimer_g)
    {
        /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
        /*SPR 21188 -*/
        /* SPR 2241 changes start */
        if (lteFD_ISSET(pdcpSocketRx,&fdSock))
        {
            *pdcpTxByteRead = ReadFromSocket(pdcpTxData, PDCP_MODULE_ID);
            lteFD_CLR(pdcpSocketRx,&fdSock);
        }
#endif
        if (lteFD_ISSET(relayConfigInfo_g.s1ApTxFd,&fdSock))
        {
            /* SPR 15497 fix start */
            /* Clear the Recv buffer pointer to avoid garbage values */
            memSet(relaySockData, 0, MAX_RECV_BUFF_SIZE);
            /* SPR 15497 fix end */
            *relaySockByteRead = recvFromRelaySockHandler( relaySockData,
                    MAX_RECV_BUFF_SIZE,RELAY_S1AP_MODULE_ID, PNULL );
            lteFD_CLR(relayConfigInfo_g.s1ApTxFd,&fdSock);
        }
    }
 /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
        /*SPR 21188 -*/
    if (lteFD_ISSET(fdForReceivingPdcpMsgFromRRM_g,&fdSock))
    {  
        /* Function call for receiving message from RRM, 
         * parse the msgId and call the corresponding API to deal with the message.*/
        *pdcpRrmDataSize = recvPdcpMsgFromRRM(pdcpRrmData);
        lteFD_CLR(fdForReceivingPdcpMsgFromRRM_g,&fdSock);
    }
#else
	
	if (relayConfigInfo_g.gtpuRxFd > 0)
	{
		 if (lteFD_ISSET(relayConfigInfo_g.gtpuRxFd,&fdSock))
		 {
		 	mbRecvDataPktsFrmPdcp();
		 }
	}

#endif

/*SPR 21653 changes start*/
    SInt32 sockFd = 0;
/*SPR 21653 changes end*/

/*SPR 21653 changes start*/
        /*SPR 20554 Fix Start*/
        if( lteFD_ISSET(s1uIpv4SockRx_g, &fdSock) ) {
            sockFd = s1uIpv4SockRx_g;
            sockAddrType |= IPV4_ADDR_PRESENT;
        }
    if (lteFD_ISSET(ipv6SockRx_g, &fdSock) ) {
        sockFd = ipv6SockRx_g;
        sockAddrType |= IPV6_ADDR_PRESENT;
    }

    if (0 != sockFd)
/*SPR 21653 changes end*/
    {
        RDTSC_AND_CLOCK_START_SOCKET;

/*SPR 21653 changes start*/
        /*SPR 6949 Fix Start*/
        if( IPV4_ADDR_PRESENT & sockAddrType )
        {
            if (dlTempMsgPtr != PNULL)
            {
                freeMemPool(dlTempMsgPtr);
                dlTempMsgPtr = PNULL;
            }

            recvIpv4Data_p = (UInt8 *)getMemFromPool(BUFFERSIZE,PNULL);
            if(PNULL == recvIpv4Data_p)
            {
                /*SPR 20554 Fix Start*/
                /*SPR 20636 Changes Start*/
                ltePanic("%s: can't allocate buffer unused[%x]",__FUNCTION__,(unsigned int*)recvData_p);
                /*SPR 20636 Changes End*/
                /*SPR 20554 Fix End*/
            }
            bytesRead = recvfrom_wrapper(s1uIpv4SockRx_g, &recvIpv4Data_p[START_OFFSET_DL_GTP], BUFFERSIZE - START_OFFSET_DL_GTP,0,
                    sgwToIpv4Addr_p,sgwToIpv4Addr_len);
            relayConfigInfo_g.sgwToAddr = *sgwToIpv4Addr_p ;
#ifndef TDD_CONFIG
            RDTSC_AND_CLOCK_END_SOCKET(0);
#endif 
            if( PNULL != checkAndReturnDataRead(recvIpv4Data_p,bytesRead))
            {
                if(bytesRead > 0)
                {
                    processPeerDataInd( recvIpv4Data_p,bytesRead
#ifndef L2_FAST_PATH 
                            , localTransId++
#endif
                            );
                }
                *isIpv4Ipv6DataReceived = 1;

            }
            else
            {
                *isIpv4Ipv6DataReceived = 0;
            }
        }

        if( IPV6_ADDR_PRESENT & sockAddrType )
        {
            if (dlTempMsgPtr != PNULL)
            {
                freeMemPool(dlTempMsgPtr);
                dlTempMsgPtr = PNULL;
            }

            recvIpv6Data_p = (UInt8 *)getMemFromPool(BUFFERSIZE,PNULL);
            if(PNULL == recvIpv6Data_p)
            {
                ltePanic("%s: can't allocate buffer",__FUNCTION__);
            }
            bytesRead = recvfrom_wrapper(ipv6SockRx_g, &recvIpv6Data_p[START_OFFSET_DL_GTP], BUFFERSIZE - START_OFFSET_DL_GTP,0,
                    sgwToIpv6Addr_p,sgwToIpv6Addr_len);
            relayConfigInfo_g.sgwToAddr = *sgwToIpv6Addr_p ;
#ifndef TDD_CONFIG
            RDTSC_AND_CLOCK_END_SOCKET(0);
#endif
            if( PNULL != checkAndReturnDataRead(recvIpv6Data_p,bytesRead))
            {
                if(bytesRead > 0)
                {
                    processPeerDataInd( recvIpv6Data_p,bytesRead
#ifndef L2_FAST_PATH 
                            , localTransId++ 
#endif
                            );
                }
                *isIpv4Ipv6DataReceived = 1;
            }
            else
            {
                *isIpv4Ipv6DataReceived = 0;
            }
        }
#ifndef L2_FAST_PATH 
        *transactionId = localTransId;
#endif
/*SPR 21653 changes end*/
#ifndef TDD_CONFIG
        RDTSC_AND_CLOCK_END_SOCKET(0);
#endif
    }
    return PNULL;
}

/**************************************************************************************
 * Function Name  : msgAndMemPoolUsage
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This Function will get Message and Memory Pool 
 **************************************************************************************/
/* + SPR 17439 */
void msgAndMemPoolUsage (void)
/* - SPR 17439 */
{
#ifndef GTPU_UT_FRWK

	msgPoolUsage = qvMsgPoolGetUsagePercentage();
	if ((MSG_POOL_HIGH_WATERMARK < msgPoolUsage) && (msgPoolOverFlow == 0))
	{
		msgPoolOverFlow = 1;
		LOG_UT(PR_MSG_POOL_HIGH_WATERMARK, LOGDEBUG,PR_GTPU_DL_DATA,
				relayGtpuCurrentTick_g ,msgPoolUsage,0,0,0,0,
				0,0,__func__,"Stopping traffic");
	}
	else if ((MSG_POOL_LOW_WATERMARK > msgPoolUsage) && (msgPoolOverFlow == 1))
	{
		msgPoolOverFlow = 0;
		LOG_UT(PR_MSG_POOL_LOW_WATERMARK, LOGDEBUG, PR_GTPU_DL_DATA,
				relayGtpuCurrentTick_g ,msgPoolUsage,0,0,0,0,
				0,0,__func__,"Restarting traffic");

	}
	memPoolUsage = qvMemPoolGetUsagePercentage();
	if ((MEM_POOL_HIGH_WATERMARK < memPoolUsage) && (memPoolOverFlow == 0))
	{
		memPoolOverFlow = 1;
		LOG_UT(PR_MEM_POOL_HIGH_WATERMARK, LOGDEBUG, PR_GTPU_DL_DATA,
				relayGtpuCurrentTick_g ,memPoolUsage,0,0,0,0,
				0,0,__func__,"Stopping traffic");

	}
	else if ((MEM_POOL_LOW_WATERMARK > memPoolUsage) && (memPoolOverFlow == 1))
	{
		memPoolOverFlow = 0;
		LOG_UT(PR_MEM_POOL_LOW_WATERMARK, LOGDEBUG, PR_GTPU_DL_DATA,
				relayGtpuCurrentTick_g ,memPoolUsage,0,0,0,0,
				0,0,__func__,"Restrating traffic");

	}
#endif 
}

/**************************************************************************************
 * Function Name  : handleUserData
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This Function will handle the user data.
 *************************************************************************************/
/* + SPR 17439 */
void handleUserData (void)
/* - SPR 17439 */    
{
    TimeSpec tp1;
	UInt8 *pdcpTxData = PNULL;
	UInt8 *pdcpRrmData = PNULL;
	UInt8 *relaySockData = PNULL;
	/*SPR 20554 Fix Start*/
	UInt8 isIpv4Ipv6DataReceived =0;
	/*SPR 20554 Fix End*/
#ifdef LTE_EMBMS_SUPPORTED
	UInt8 *recvBuff_p = PNULL;
#endif

        /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
	/* Coverity_31224 Fix */
	pdcpTxData = (UInt8 *)getMemFromPool(SOCKET_BUFFER_SIZE, PNULL);
	if(PNULL == pdcpTxData)
	{
		lteWarning("In %s, MEMORY ALLOC FAIL\n",__func__);
		return;
	}
	pdcpRrmData = (UInt8 *)getMemFromPool(MAX_PDCP_RRM_MSG_SZ, PNULL);
	if(PNULL == pdcpRrmData)
	{
		freeMemPool(pdcpTxData);
		lteWarning("In %s, MEMORY ALLOC FAIL\n",__func__);
		return;
	}
#endif
	relaySockData = (UInt8 *)getMemFromPool(MAX_RECV_BUFF_SIZE, PNULL);
	if(PNULL == relaySockData)
	{
#if  !defined(PDCP_GTPU_INTF)
		freeMemPool(pdcpTxData);
		freeMemPool(pdcpRrmData);
#endif
		lteWarning("In %s, MEMORY ALLOC FAIL\n",__func__);
		return;
	}
	/* Coverity_31224 Fix */
	SInt16 pdcpRrmDataSize;
	/*SPR 20554 Fix Start*/
	SInt16 pdcpTxByteRead = 0, relaySockByteRead = 0;
	/*SPR 20554 Fix End*/
	/* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
	SInt16 eMbmsBytesRead = -1;
	HeaderInfo *headerInfo_p=(HeaderInfo *)getMemFromPool(sizeof(HeaderInfo), PNULL);
	if(PNULL == headerInfo_p)
	{
		freeMemPool(pdcpRrmData);
		freeMemPool(pdcpTxData);
		freeMemPool(relaySockData);
		lteWarning("In %s, MEMORY ALLOC FAIL\n",__func__);
		return;
	}

#endif
	/* eMBMS Code Changes End */
#ifndef L2_FAST_PATH
	UInt32 transactionId = 0;
#endif
#if  !defined(PDCP_GTPU_INTF)
	while (1)
#endif
	{
		pdcpTxByteRead = 0; relaySockByteRead = 0; pdcpRrmDataSize = 0;
#ifdef LTE_EMBMS_SUPPORTED
		eMbmsBytesRead = -1;
#endif

        clock_gettime_real_time( &tp1);
		msgAndMemPoolUsage ();

		isIpv4Ipv6DataReceived = 0;	/* SPR21135 +- */
		/*SPR 20554 Fix Start*/
#ifdef LTE_EMBMS_SUPPORTED
	recvBuff_p = receiveUserData(pdcpTxData,&pdcpTxByteRead,relaySockData,
				/*SPR 20554 Fix End*/
				&relaySockByteRead, &isIpv4Ipv6DataReceived,
				pdcpRrmData,&pdcpRrmDataSize
#ifndef L2_FAST_PATH 
				, &transactionId
#endif
				,&eMbmsBytesRead
				,headerInfo_p
				);
#else
	(void)receiveUserData(pdcpTxData,&pdcpTxByteRead,relaySockData,
				/*SPR 20554 Fix End*/
				&relaySockByteRead, &isIpv4Ipv6DataReceived,
				pdcpRrmData,&pdcpRrmDataSize
#ifndef L2_FAST_PATH 
				, &transactionId
#endif

				);
#endif
		/*SPR 20554 Fix End*/

        /*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
		if(pdcpTxByteRead > 0)
		{
			pdcpElTxProcessSocket(pdcpTxData,pdcpTxByteRead);
		}
		if(pdcpRrmDataSize > 0)
		{
			processPdcpRRMMsg(pdcpRrmData,pdcpRrmDataSize);
		}
#endif
		if(relaySockByteRead > 0)
		{
			lteRelaySockHandler(relaySockData,relaySockByteRead);
		}
		/* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
		if(eMbmsBytesRead > 0)
		{
#ifdef L2_FAST_PATH
			/* Function will process the data received from BMSC */
			processEmbmsData(recvBuff_p,( eMbmsBytesRead + START_OFFSET_DL_GTP ),headerInfo_p);
#else
			processPeerGtpuEmbmsData(recvBuff_p,( eMbmsBytesRead + START_OFFSET_DL_GTP ),headerInfo_p);
#endif
		}
#endif
		/* eMBMS Code Changes End */
#if  !defined(PDCP_GTPU_INTF)
		if( !isIpv4Ipv6DataReceived )
		{
			/*SPR 20554 Fix End*/
			/*SPR 21188 +*/
#ifndef PDCP_GTPU_INTF
			break;
#endif
			/*SPR 21188 +*/
		}
#endif
#ifdef ENDC_ENABLED
    triggerDeliveryStatusReport();
#endif
    }
/*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
    /*SPR 21188 -*/
    freeMemPool(pdcpTxData);
    freeMemPool(pdcpRrmData);
#endif	
	freeMemPool(relaySockData);
#ifdef LTE_EMBMS_SUPPORTED
	freeMemPool(headerInfo_p);
#endif

}
#endif

/****************************************************************************
 * * FAST PATH Implementation
 * *
 * ****************************************************************************/
#ifdef L2_FAST_PATH
/****************************************************************************
 * Function Name  : freeMemPool2Args
 * Inputs         : Pointer to be freed
 * Outputs        : None
 * Returns        : None
 * Description    : This function free's the pointer. New fuction required to
 *                  support two aruguments required by CSPL.
 *****************************************************************************/
void freeMemPool2Args (void *p , void *q)
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(p)
    /* +- SPR 17777 */
    /* SPR 22402 Fix start */
    freeMemPool(q);
    /* SPR 22402 Fix end */
}
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

/**********************************************************************************
 * Function Name  : lteUserPlaneFastPath
 * Inputs         : recvData_p - Pointer to GTPU message received from Peer GTPU,
 *                  bytesRead
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes GTPU Header and identifies T-PDUs to
 *                  be processed on a fast path.
 **********************************************************************************/
/* + SPR 20276 */
void lteUserPlaneFastPath(UInt8 *recvData_p, UInt32 bytesRead)
{
    egtpu_rab_ctxt_t    *p_rab_ctxt = PNULL;
#if  !defined PDCP_GTPU_INTF
    LP_PdcpUeContext     p_pdcp_ctxt = PNULL;
    LP_PdcpEnityContext ctx_p = PNULL;
    UInt8* data_p = PNULL;
#endif
    UInt8 gtpHeaderLen = 0;
    UInt32 size = 0;
    static UInt32 transactionId = 0;
    UInt8 offset = START_OFFSET_DL_GTP; 
    UInt8 error_flag = LTE_FALSE;
    UInt16 msgType =  EGTPU_GET_MSG_TYPE(&recvData_p[offset]);

    do
    {
        /* + SPR 19022 */

        /* - SPR 19022 */

        /*
         * Any one of the optional field - E or PN are set in GTP
         * header, then do not process on Fast Path.
         * Since GTPU SN is not supported hence, pass it through slow 
         * path only.
         */
        if(!(recvData_p[offset] & GTP_HDR_ALL_OPT_FIELD_SET))
        {
            /* Check if GTP SN is present in GTP header to decide about
             * the GTP header length */
            if( recvData_p[offset] & GTP_HDR_SEQ_NUM_BIT )
            {
                gtpHeaderLen = GTP_MAX_HDR_LEN;
            }
            else
            {
                gtpHeaderLen = GTP_MIN_HDR_LEN;
            }
            /*
             * Check if the size of the packet received has -
             * Minium 12 Bytes for CSPL Header
             * Minimum 8 Bytes for GTPU Header
             * Total = 20 Bytes
             */
            if( (bytesRead - offset) >= gtpHeaderLen )
            {
                size = bytesRead - (gtpHeaderLen + offset);
                /*
                 * Check the GTPU Message header. 
                 * Check for Message Type in GPTU Header.
                 * If the Message type is not T-PDU, then follow existing code flow.
                 */
                if (EGTPU_MSGT_TPDU ==  msgType)
                {
                    /*
                     * 1. Get Tunnel ID for the GTPU Message
                     * 2. Fetch the RAB Context corresponding to the RAB Context
                     */
                    p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(
                            p_global, EGTPU_GET_TEID((&recvData_p[offset])));
                    if ((PNULL != p_rab_ctxt) && (0 < size))
                    {
#if  defined PDCP_GTPU_INTF
                        /* Send the data packet to PDCP over mailbox */
                        pdcpDataReq(
                                relayMapBasedOnTransId_g[p_rab_ctxt->transaction_id].ueIndex,
                                relayMapBasedOnTransId_g[p_rab_ctxt->transaction_id].lcId,
                                p_rab_ctxt->transaction_id,
                                &recvData_p[offset + gtpHeaderLen], recvData_p, size,0);
                        /* Set dlTempMsgPtr so that BRCM memory is not freed */
                        dlTempMsgPtr = PNULL;
#else
                        /*
                         * 1. Get UE index and Logical Channel ID based on Transaction ID
                         * 2. Fetch the PDCP Context.
                         */
                        p_pdcp_ctxt = 
                            pdcpContext.ueContexts[relayMapBasedOnTransId_g[p_rab_ctxt->transaction_id].ueIndex];
                        if( PNULL != p_pdcp_ctxt)
                        {
                            /*
                             * Check -
                             * 1. The PDCP State for DRB is PDCP_TX_STATE_CONNECTED
                             * 2. DL-Forwarding Queue contents
                             */
                            if((PDCP_TX_STATE_CONNECTED == p_pdcp_ctxt->pdcpTxSrb2DrbState) && 
                                    (0 == QCOUNT_PDCP_DATA_FWDING_DL_Q(p_pdcp_ctxt)))
                            {
                                /*
                                 * Fast Path Execution for PDCP 
                                 */
                                /* Convert data received to ZCB and send it to PDCPi Layer */
                                data_p = (UInt8 *)msgAlloc(PNULL, 0, 0, 0);
                                if ( PNULL == data_p ) 
                                {
                                    LOG_GTPU_MSG(PR_DL_DATA_MSG_ALLOC_FAIL, LOGFATAL, PR_GTPU_DL_DATA,
                                            relayGtpuCurrentTick_g, __LINE__, 
                                            size, 0,0,0,0,0, __func__,
                                            " ADAPTER" );
                                    /*SPR 12884 FIX start*/
                                    error_flag = LTE_TRUE;
                                    break;
                                    /*SPR 12884 FIX End */
                                }
/*SPR 21653 changes start*/


                                if (0 == qvMsgInsertExternal(data_p, 0, &recvData_p[offset + gtpHeaderLen], size,
                                            lteFreeEgtpuBuff,
                                            (void *)recvData_p
#ifdef CSPL_LEAK_DEBUG_LITE
                                            ,__func__,__LINE__
#endif
                                            ))
                                    {
                                        LOG_GTPU_MSG(PR_DL_DATA_MSG_INSERT_FAIL, LOGFATAL, PR_GTPU_DL_DATA,
                                                relayGtpuCurrentTick_g, __LINE__,
                                                size, 0,0,0,0,0, __func__,
                                                " ADAPTER" );
                                        msgFree( data_p );
                                        error_flag = LTE_TRUE;
                                        break;
                                    }
/*SPR 21653 changes end*/
                                dlTempMsgPtr = PNULL;
                                ctx_p = pdcpGetEntityContext(
                                        relayMapBasedOnTransId_g[p_rab_ctxt->transaction_id].ueIndex,
                                        relayMapBasedOnTransId_g[p_rab_ctxt->transaction_id].lcId,
                                        PDCP_ENTITY_RB_TX);
                                if (PNULL == ctx_p )
                                {
                                    LOG_GTPU_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                                            relayGtpuCurrentTick_g, __LINE__, 
                                            relayMapBasedOnTransId_g[p_rab_ctxt->transaction_id].ueIndex, 
                                            relayMapBasedOnTransId_g[p_rab_ctxt->transaction_id].lcId, 0, 0, 0,
                                            0,__func__,"ADAPTER");
                                    msgFree( data_p );
                                    break;
                                }

                                if (!QCOUNT_PDCP_TX_DATA_REQ_Q( ctx_p ))
                                {
                                    pdcpDataFpReq(
                                            relayMapBasedOnTransId_g[p_rab_ctxt->transaction_id].ueIndex,
                                            relayMapBasedOnTransId_g[p_rab_ctxt->transaction_id].lcId,
                                            ctx_p, p_rab_ctxt->transaction_id,
                                            data_p, size);
                                }
                                else
                                {
                                    pdcpDataReq(
                                            relayMapBasedOnTransId_g[p_rab_ctxt->transaction_id].ueIndex,
                                            relayMapBasedOnTransId_g[p_rab_ctxt->transaction_id].lcId,
                                            p_rab_ctxt->transaction_id,
                                            data_p, size,0);
                                }
                            }
                            else
                            {
                                /*
                                 * Follow legacy Processing
                                 */
                                /*SPR 21108 Fix Start */
                                if( RELAY_GTPU_FAILURE == processPeerGtpuDataInd(
                                            recvData_p, transactionId++,
                                            bytesRead ) )
                                {
                                    error_flag = LTE_TRUE;
                                }
                                /*SPR 21108 Fix End */
                            }
                        }
                        else
                        {
                            error_flag = LTE_TRUE;
                        }
#endif
                    }
                    else
                    {
                        error_flag = LTE_TRUE;
                    }
                }
                else
                {
                    /*
                     * Follow legacy Processing
                     */
                    /*SPR 21108 Fix Start */
                    if( RELAY_GTPU_FAILURE == processPeerGtpuDataInd(
                                recvData_p, transactionId++,
                                bytesRead ) )
                    {
                        error_flag = LTE_TRUE;
                    }
                    /*SPR 21108 Fix End */
                }
            }
            else
            {
                error_flag = LTE_TRUE;
            }
        }
        else
        {
            /*
             * Follow legacy Processing
             */
            if( RELAY_GTPU_FAILURE == processPeerGtpuDataInd( recvData_p,
                        transactionId++, bytesRead ) )
            {
                error_flag = LTE_TRUE;
            }
        }
    }while (0);

    if(error_flag == LTE_TRUE)
    {
        /* Cannot process the data hence, free the memory */
        lteFreeEgtpuBuff( 0,(void *)recvData_p );
        dlTempMsgPtr = PNULL;
    }
}
/* - SPR 20276 */
#endif

/*SPR 21653 changes start*/
/* code removed */
/*SPR 21653 changes end*/

/* + SPR 19022 */
/* - SPR 19022 */
/********************************************************************************
 * Function Name  : initSocketAddress
 * Inputs         : s - sockaddr_in,
 *                  sockfd - socketFileDescriptor,
 *                  port - Port Number
 * Outputs        : None
 * Returns        : status(0-Failure, 1-Successful)
 * Description    : To intialize the Socket address with specific socket file
 *                  descriptor and specific port.
 ********************************************************************************/
/* SPR 14327 fix start */
static UInt8 initSocketAddress(SockAddr *s,SInt32 *sockfd,Char8 *ipAddr, UInt16 port)
{
    /*As per SPR 713, change end*/
    if((*sockfd = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("creating socket error\n");
        return 0;
    }
    (*s).sin_family = AF_INET;
    (*s).sin_port = htons_wrapper(port);
    if (Inet_aton_wrapper((ipAddr),&((*s).sin_addr)) == 0)
    {
        ltePanic ("Error while converting the address.");
    }
    return 1;
}


/* Intefaces */
/****************************************************************************
 * Function Name  : pdcpDataInd
 * Inputs         : ueIndex - UE Id,
 *                  lcId - Logical channel Id,
 *                  data_p - pointer to PDCP SDU data in Zero-copy
 *                           buffering format,
 *                  size - size of data
 * Outputs        : None
 * Returns        : None
 * Description    : The PDCP layer application has to implement this function.
 *                  It's used for passing SDU form the PDCP layer to the PDCP
 *                  user (upper layers).
 ****************************************************************************/
void  pdcpDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p, UInt16 size)
{
    UInt8 *msgPtr = PNULL;
    UInt16 msgLen = 0;

    if(!data_p) {
        LOG_GTPU_MSG(PR_ADAPTER_EMPTY_UL_DATA, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                ueIndex, lcId,0,0,0,0, __func__,
                "" );
        return ;
    }    
/*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
/*SPR 21188 -*/
#ifdef KPI_STATS
    UInt32 activeIndexForPdcpThpStats = (gMacStats.
            pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats);
    LtePdcpKPIThpStats *pLtePdcpKPIThpStats = &(gMacStats.pStats->lteCommonUeKPIStats.
            ltePdcpKPIThpStats[activeIndexForPdcpThpStats]);
    pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[ueIndex][lcId - 3] += size;
#endif
#endif

    /* We receive SDU from the PDCP layer, so pass it to upper layer in some way */
    if(BUFFERSIZE < size )
    {
        msgFree(data_p);
        data_p = PNULL;
        LOG_GTPU_MSG(PR_ADAPTER_UL_DATA_GREATER_SIZE, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                BUFFERSIZE, size,0,0,0,0, __func__,
                "" );
        return ;
    }
#ifdef L2_FAST_PATH
    /*
     * Get memory for GTP Header + Payload received from PDCP
     */
    msgLen = size+GTP_MIN_HDR_LEN;
    msgPtr = msgReserve(data_p, 0, GTP_MIN_HDR_LEN);
    if(PNULL == msgPtr)
    {
        ltePanic("%s: can't allocate buffer",__FUNCTION__);
        return;
    }
#else
    msgLen = size+64;
    msgPtr = (UInt8 *)getMemFromPool(msgLen, PNULL);
    if(PNULL == msgPtr)
    {
        /*SPR fix 2030 Start */
        LOG_GTPU_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                relayGtpuCurrentTick_g, __LINE__, 
                msgLen, 0,0,0,0,0, __func__,
                "ADAPTER" );
        /*SPR fix 2030 End */
        return;
    }
    msgExtract(data_p, 0, msgPtr+64, size);

    /* not freeing memory here as ZCB used */
    msgFree(data_p);
    data_p = PNULL;
#endif

#ifdef L2_FAST_PATH
    egtpu_rab_ctxt_t    *p_rab_ctxt = NULL;
    /* SPR_3736 start */
    p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(
            p_global, relayMapBasedOnUeIndexLcId_g[ueIndex][lcId-3].tunnelId);
    if(NULL != p_rab_ctxt)
    {
        /*
         *  * Populate the GTPU Header of 8 Bytes
         *   */
        UInt8 gtp_hdr_indx = 0;

        /* Octet 1
         ** Version - 1
         ** Protocol Type - 1
         ** All other fields are unset
         **/
        msgPtr[gtp_hdr_indx++]= 0x30;
        /*
         ** Octet 2
         **
         ** Message Type = T-PDU = 255
         **/
        msgPtr[gtp_hdr_indx++]= 0xff;

        /*
         ** Octet 3 & 4
         **
         ** Size of Payload
         **/
        msgPtr[gtp_hdr_indx++]= (size & 0xFF00)>>8;
        msgPtr[gtp_hdr_indx++]= (size & 0x00FF)>>0;


        /*
         ** Octet 5, 6, 7 & 8
         **
         ** Tunnel ID
         **/
        /* SPR 10787 fix start */
        LTE_SET_U32BIT(&msgPtr[gtp_hdr_indx],p_rab_ctxt->teid_cell.teid_peer);
        /* SPR 10787 fix end */
    }
    else
    {
        LTE_LOG(LOG_INFO,"\n[ERROR] NULL rab ctxt - error in Relay Map\n");
        msgFree(data_p);
        data_p = NULL;
        return;
    }
    /*SPR_3736 change end */
#endif

#ifndef L2_FAST_PATH
    LTE_SET_U16BIT(&msgPtr[59],ueIndex);
    msgPtr[61] = lcId;
    LTE_SET_U16BIT(&msgPtr[62], (msgLen-64));
#endif

#ifdef L2_FAST_PATH
    UInt8  addr_len;
    SInt8               socketfd = 0; 
    sockAddrIn6 sockaddr6;
    void *pSockAddr = NULL;
    UInt16 sockAddrLen = 0;
    /*
     * Fetch the RAB Context on the Basis of the Tunnel ID
     */
    /*SPR_3736 change start*/
    /*    p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(
          p_global, relayMapBasedOnUeIndexLcId_g[ueIndex][lcId-3].tunnelId);

          if ((NULL != p_rab_ctxt) &&
          (NULL != p_rab_ctxt->p_transport_addr))
          */
    if (NULL != p_rab_ctxt->p_transport_addr)
        /*SPR_3736 change end */
    {
        /*IPV6_CHANGE - START*/
        addr_len = RELAY_GTPU_GET_U16BIT(&(p_rab_ctxt->p_transport_addr->entity_addr[0]));
        /* Coverity 83206 Fix Start*/
        if(addr_len > RELAY_ADDR_LEN_IPV6)
        {
            msgFree(data_p);
            data_p = NULL;
            return;
        }
        /* Coverity 83206 Fix End*/
        if (RELAY_ADDR_LEN_IPV4 == addr_len)
        {
            /*
             * Populate the information for sending the packet to Peer GTPU Entity
             */
            relayConfigInfo_g.gtpuToAddr.sin_family = AF_INET;
            relayConfigInfo_g.gtpuToAddr.sin_port = htons_wrapper(EGTPU_UDP_PORT);
            relayConfigInfo_g.gtpuToAddr.sin_addr.s_addr 
                = htonl_wrapper(RELAY_GTPU_GET_U32BIT(&(p_rab_ctxt->p_transport_addr->entity_addr[2])));        
            /* COVERITY 10446 */ 
            if (p_rab_ctxt->qos_id >= MAX_DSCP_VALUES)
                /* COVERITY 10446 */ 
            {
                /*SPR 20554 Fix Start*/
                socketfd = s1uIpv4SockRx_g;
                /*SPR 20554 Fix End*/
            } 
            else
            {
                socketfd = relayDscpToSockFdMap_g[p_rab_ctxt->qos_id].sockfd;
                if (socketfd == RELAY_INVALID_FD)
                {
                    /*SPR 20554 Fix Start*/
                    socketfd = s1uIpv4SockRx_g;
                    /*SPR 20554 Fix End*/
                }
            }
            pSockAddr = &relayConfigInfo_g.gtpuToAddr;
            sockAddrLen = sizeof(relayConfigInfo_g.gtpuToAddr);
        }
        else
        {
            memSet(&sockaddr6,0,sizeof(sockaddr6));
            memCpy(sockaddr6.sin6_addr.s6_addr, &(p_rab_ctxt->p_transport_addr->entity_addr[2]), addr_len);
            sockaddr6.sin6_family = AF_INET6;
            sockaddr6.sin6_port = htons_wrapper(EGTPU_UDP_PORT);

            if (p_rab_ctxt->qos_id >= MAX_DSCP_VALUES)
            {
                /*SPR 20554 Fix Start*/
                socketfd = ipv6SockRx_g;
                /*SPR 20554 Fix End*/
            } 
            else
            {
                socketfd = relayDscpToSockFdMap6_g[p_rab_ctxt->qos_id].sockfd;
                if (socketfd == RELAY_INVALID_FD)
                {
                    /*SPR 20554 Fix Start*/
                    socketfd = ipv6SockRx_g;
                    /*SPR 20554 Fix End*/
                }
            }
            pSockAddr = &sockaddr6;
            sockAddrLen = sizeof(sockaddr6); 
        }
        /*IPV6_CHANGE - END*/
#ifdef LAYER2_DLTG_THREAD
        /*
         * Pass the GTPu Payload to DLTG 
         */
        /* SPR 10787 fix start */
        lteMacprocessULPacketInDLTG (data_p,size + GTP_MIN_HDR_LEN,
                p_rab_ctxt->teid_cell.teid_peer);
        /* SPR 10787 fix end */
#else
        /*
         * Flatten buffer before writing to socket
         */
        UInt8 *flatbuff = PNULL;
        flatbuff = (UInt8 *)getMemFromPool(msgLen, PNULL);
        if(PNULL == flatbuff)
        {
            ltePanic("%s: can't allocate buffer",__FUNCTION__);
            return;
        }
        msgExtract(data_p, 0, flatbuff, msgLen);
        /*SPR 6837 Fix start*/
        if ( -1 ==
                sendto_wrapper(socketfd,&flatbuff[0],msgLen,
                    0,pSockAddr,sockAddrLen ))
            /*SPR 6837 Fix end*/
        {
            LTE_LOG(LOG_INFO,"\n[ERROR] Not able to send data to DL-TG\n");
        }

        freeMemPool(flatbuff);
        flatbuff = NULL;
#endif
    }
#else
    processGtpuAdapterDataReq(msgPtr, msgLen);
#endif
#ifdef L2_FAST_PATH
    /* freeing memory here, not freed earlier ZCB used */
    msgFree(data_p);
    data_p = NULL;
#else
    freeMemPool(msgPtr);
    /* coverity 72863 Fix +- */
#endif
}

/****************************************************************************
 * Function Name  : writeToSocket
 * Inputs         : fd - Socket fd,
 *                  msgPtr - Ptr to message to be sent,
 *                  msgLen -  Message Length,
 *                  hostname_p - Hostname of machine,
 *                  port - Port on which data needs to be sent
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to write data on socket for
 *                  traffic generator.
 * ****************************************************************************/
void writeToSocket(SInt8 fd, void *msgPtr, UInt16 msgLen,
        Char8 *hostname_p, UInt16 port)
{
    Int res_len = -1;
    /* write To Socket */
    struct hostent *host_p = PNULL;
    SockAddr toSockAddr;
    host_p= (struct hostent *) gethostbyname_wrapper(hostname_p);
    /*As per SPR 713, change start*/
    if(((struct hostent *)0) == host_p)
    {
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(L2_GET_HOSTBYNAME_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                relayGtpuCurrentTick_g,__LINE__,
                0,0 ,
                0,0, 0,0, __func__, "");
        /* SPR 2030 fix End*/
        return;
    }
    /*As per SPR 713, change end*/
    toSockAddr.sin_family = AF_INET;
    toSockAddr.sin_port = htons_wrapper(port);
    toSockAddr.sin_addr = *((struct in_addr *)host_p->h_addr);
#ifndef TDD_CONFIG
    RDTSC_AND_CLOCK_START_SOCKET;
#endif
    res_len = sendto_wrapper(fd,(UChar8*)msgPtr,msgLen,0,
            &toSockAddr,sizeof(SockAddr));
#ifndef TDD_CONFIG
    RDTSC_AND_CLOCK_END_SOCKET(0);
#endif

    if(res_len == msgLen)
    {
    }
    else
        if (res_len == -1)
        {
            LOG_GTPU_MSG(PR_ADAPTER_ERR_IN_SENDING_PACKET_TO_TG, LOGWARNING, PR_GTPU_UL_DATA,
                    relayGtpuCurrentTick_g,__LINE__,0,0,0,0,
                    0,0,__func__,"");
        }
}

/****************************************************************************
 *  Function Name  : pdcpDataStatusInd
 *  Inputs         : ueIndex - UE Id,
 *                   lcId    - Logical channel Id,
 *                   deliveryStatus - SDU delivery status
 *  Outputs        : None
 *  Returns        : None
 *  Description    : The PDCP layer application has to implement this function.
 *                   It used for sending SDU delivery indictaion to the PDCP
 *                   user (upper layers) in case of Discard_timer expires for
 *                   SDU or PDU delivery is failed (for RLC AM).
 * ****************************************************************************/
void pdcpDataStatusInd( UInt16 ueIndex, UInt16 lcId, UInt16 deliveryStatus )
{
    /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(ueIndex)
        LTE_GCC_UNUSED_PARAM(lcId)
        LTE_GCC_UNUSED_PARAM(deliveryStatus)
        /* +- SPR 17777 */
}  

/****************************************************************************
 * Function Name  : pdcpSourceDlDataRelocInd
 * Inputs         : ueIndex - UE Id,
 *                  lcId - Logical channel Id,
 *                  sn - Sequence Number,
 *                  data_p - pointer to PDCP SDU data in Zero-copy
 *                           buffering format,
 *                  size - size of payload
 * Outputs        : None
 * Returns        : None
 * Description    : The PDCP layer calls this API to forward Downlink SDUs 
 *                  to upper layer at source eNodeB in case of handover.
 ****************************************************************************/

UInt16 pdcpSourceDlDataRelocInd(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
        void *data_p, UInt32 size)
{

    UInt8 *msgPtr = PNULL;
    if(!data_p) 
    {
        LOG_GTPU_MSG(PR_ADAPTER_EMPTY_UL_DATA, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                ueIndex, lcId,0,0,0,0, __func__,
                "" );
        /* SPR 16770 fix start */
        return 0; 
        /* SPR 16770 fix end */
    }    
/*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
/*SPR 21188 -*/
#ifdef KPI_STATS
	UInt32 activeIndexForPdcpThpStats = (gMacStats.
			pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats);
	LtePdcpKPIThpStats *pLtePdcpKPIThpStats = &(gMacStats.pStats->lteCommonUeKPIStats.
			ltePdcpKPIThpStats[activeIndexForPdcpThpStats]);

    /*SPR 21291 Start*/
    if(pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId - 3] >= size)
    {
	pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId - 3] -= size;
    }
    else
    {
        pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId - 3] = 0;
    }
    /*SPR 21291 End*/

#endif
#endif

    /* We receive SDU from the PDCP layer, so forward it to upper layer */
    if(BUFFERSIZE < size )
    {
        LOG_GTPU_MSG(PR_ADAPTER_UL_DATA_GREATER_SIZE, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                BUFFERSIZE, size,0,0,0,0, __func__,
                "" );
        return 0;
    }

    UInt16 msgLen = size+64;
	msgPtr = (UInt8 *)getMemFromPool(msgLen, PNULL);
	if(PNULL == msgPtr)
	{
		LOG_GTPU_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
				relayGtpuCurrentTick_g, __LINE__, 
				msgLen, 0,0,0,0,0, __func__,
				"ADAPTER" );
		return 0;
	}
	msgExtract(data_p, 0, msgPtr+64, size);

    LTE_SET_U16BIT(&msgPtr[59],ueIndex);
    msgPtr[61] = lcId;
    LTE_SET_U16BIT(&msgPtr[62], (msgLen-64));

    /*Forward DL PDCP SDUs to packet Relay*/
    processGtpuAdapterDlDataRelocReq(msgPtr,sn);

	freeMemPool(msgPtr);
	return 1;
}

/****************************************************************************
 * Function Name  : pdcpSourceUlDataRelocInd
 * Inputs         : ueIndex - UE Id,
 *                  lcId - Logical channel Id,
 *                  sn - Sequence Number,
 *                  data_p - pointer to PDCP SDU data in Zero-copy
 *                           buffering format,
 *                  size - size of payload
 *                  internalCellIndex - internal cell Id 
 * Outputs        : None
 * Returns        : None
 * Description    : The PDCP layer calls this API to forward uplink SDUs 
 *                  to upper layer at source eNodeB in case of handover.
 ****************************************************************************/
UInt16 pdcpSourceUlDataRelocInd(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
        void *data_p, UInt32 size
#ifdef KPI_STATS
        , InternalCellIndex internalCellIndex
#endif
        )
{
    UInt8 *msgPtr = PNULL;
    if(!data_p) 
    {
        LOG_GTPU_MSG(PR_ADAPTER_EMPTY_UL_DATA, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                ueIndex, lcId,0,0,0,0, __func__,
                "" );
        /* SPR 16770 fix start */
        return 0;
        /* SPR 16770 fix end */
    }    
    /* We receive SDU from the PDCP layer, so forward it to upper layer */
    if(BUFFERSIZE < size )
    {
        LOG_GTPU_MSG(PR_ADAPTER_UL_DATA_GREATER_SIZE, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                BUFFERSIZE, size,0,0,0,0, __func__,
                "" );
        return 0;
    }
    /* + SPR 17415 */
/*SPR 21188 +*/
#if  !defined(PDCP_GTPU_INTF)
/*SPR 21188 -*/
#ifdef KPI_STATS
    /* SPR 20831 Changes Start */
    UInt32 activeIndexForPdcpThpStats = gMacStats.pStats->
        lteCommonUeKPIStats.activeIndexForPdcpThpStats;
    LtePdcpKPIThpStats *pLtePdcpKPIThpStats = &(gMacStats.pStats->lteCommonUeKPIStats.
            ltePdcpKPIThpStats[activeIndexForPdcpThpStats]);
    /* SPR 20831 Changes End */

    /* UlSduBitRate needs to be calculated at source eNB and will not be 
     * incremented at target, hence incrementing the stats here for HO 
     * (Reference: Section 4.4.1.2 of 32.425) */
    pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[ueIndex][lcId - MIN_DRB_LC] +=
        size;
#endif
#endif
    /* - SPR 17415 */
	UInt16 msgLen = size+64;
	msgPtr = (UInt8 *)getMemFromPool(msgLen, PNULL);
	if(PNULL == msgPtr)
	{
		LOG_GTPU_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
				relayGtpuCurrentTick_g, __LINE__, 
				msgLen, 0,0,0,0,0, __func__,
				"ADAPTER" );
		return 0;
	}
	msgExtract(data_p, 0, msgPtr+64, size);

	LTE_SET_U16BIT(&msgPtr[59],ueIndex);
	msgPtr[61] = lcId;
	LTE_SET_U16BIT(&msgPtr[62], (msgLen-64));

	/*Forward DL PDCP SDUs to packet Relay*/
	processGtpuAdapterUlDataRelocReq(msgPtr,sn);

	freeMemPool(msgPtr);
	return 1;
}

/****************************************************************************
 * Function Name  : pdcpEndOfDataForwardingInd
 * Inputs         : ueIndex - UE Id,
 *                  lcId    - Logical channel Id,
 *                  isEndMarkerIndRecvd - Flag for EMI Received
 * Outputs        : None
 * Returns        : None
 * Description    : The PDCP layer indicates end of forwarding SDUs in HO
 *                  Scenario.
 ****************************************************************************/
    /* SPR 3085 Fix Start*/
void pdcpEndOfDataForwardingInd(UInt16 ueIndex, UInt8 lcId,UInt8 isEndMarkerIndRecvd)
    /* SPR 3085 Fix End*/
{
    if((ueIndex < ADAPTER_MAX_UE) && (lcId <= MAX_DRB_LC) && (lcId >= MIN_DRB_LC))
    {
        /* SPR 3085 Fix Start */
        processAdapterEndOfDataFwdInd(ueIndex,lcId,isEndMarkerIndRecvd);
        /* SPR 3085 Fix End */
    }
    else
    {
        LOG_GTPU_MSG(PR_ADAPTER_INV_UEID_LCID, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__,
                ueIndex, lcId,0,0,0,0, __func__,"" );
    }
}

/****************************************************************************
 * Function Name  : setConfig
 * Inputs         : configData - Initialization Configuration Data 
 * Outputs        : None
 * Returns        : None
 * Description    : To set the default config values in config structre.
 ****************************************************************************/
static void  setConfig(LP_ConfigFileFormat configData)
{
    configData->trafficGeneratorPort= trafficGeneratorPort;
    strNCpy(configData->trafficGeneratorIP,trafficGeneratorIP,sizeof(configData->trafficGeneratorIP));
    strNCpy(configData->adapterIP,adapterIP,sizeof(configData->adapterIP));
    configData->addrPresenceSt = IPV4_ADDR_PRESENT;
    configData->adapterMode= pdcpAdapterMode_g;
    configData->adapterDelay= pdcpAdapterDelay_g;
    configData->adapterRxPort = pdcpAdapterRxPort;
}
/****************************************************************************
 * Function Name  : initPdcpAdapter
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function is the initialization function for PDCP 
 *                  Adapter.
 ****************************************************************************/
/* + SPR 17439 */
void initPdcpAdapter(void)
/* - SPR 17439 */
{
	if(adapterInitialized_g == 1) {
		LOG_GTPU_MSG(PR_ADAPTER_ALREADY_INITIALIZED, LOGINFO, PR_GTPU_COMMON,
				relayGtpuCurrentTick_g,__LINE__,
				0,0 ,
				0,0, 0,0, __func__, "");
		return;
	}
#if  !defined(PDCP_GTPU_INTF)
    createMemPool(BUFFERSIZE, 15000);
#else
    createMemPool(BUFFERSIZE, 150);
#endif
	/* Start 128 UE: Memory Changes */
	createMemPool(sizeof(ConfigFileFormat), 1);
#ifdef LAYER2_DLTG_THREAD
#ifndef PDCP_GTPU_INTF
    createMemPool(sizeof(dlTgDataPkt), MAX_PKTS_IN_QUE + 35000);
#else
	createMemPool(sizeof(dlTgDataPkt), MAX_PKTS_IN_QUE + 1600000);
#endif
#endif
    /* End 128 UE: Memory Changes */

    Char8 * adapterConfigFileName = "../ltePdcpAdapterConfig.cfg";
    configData = 
        (LP_ConfigFileFormat)getMemFromPool(sizeof(ConfigFileFormat),PNULL);
    if(configData == PNULL){
        /*SPR fix 2030 Start */
        LOG_GTPU_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                relayGtpuCurrentTick_g, __LINE__, 
                sizeof(ConfigFileFormat), 0,0,0,0,0, __func__,
                "ADAPTER" );
        ALARM_MSG(PACKET_RELAY_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        /*SPR fix 2030 End */
        return;
    }
    /* create msg pool to be used */
    /*pdcpAdapterMsgPool_g =
      msgPoolCreate(PDCP_ADAPTER_BUFFER_SIZE, PDCP_ADAPTER_MAX_BUFFERS);*/
    ReadConfigFile(adapterConfigFileName,configData->trafficGeneratorIP,&pdcpAdapterMode_g,&pdcpAdapterDelay_g);
    /* Set Config Parameters into config structure */
    setConfig(configData);
    /* Select a mode, normal or loopback, in which PDCP Adapter will be running 
       based on the config param, adapterMode,  read from config file */
    switch(configData->adapterMode)
    {
        case NORMAL_MODE:
        break;
        default:
        /* Set to NORMAL_MODE by default*/
        configData->adapterMode = NORMAL_MODE;
    }

    adapterInitialized_g = 1;
}

/****************************************************************************
 * Function Name  : cleanupPdcpAdapter
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function will cleanup the PDCP Adapter.
 ****************************************************************************/
/* + SPR 17439 */
void cleanupPdcpAdapter(void)
    /* - SPR 17439 */    
{
    if(adapterInitialized_g == 0){
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(PR_ADAPTER_NOT_INITIALIZED, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g,__LINE__,
                0,0,0,0, 0,0, __func__, "");
        /* SPR 2030 fix End*/
        return;
    }
    /*+COVERITY 5.3.0 - 32086*/
    (void)threadCancel(recvThId_g);
    /*-COVERITY 5.3.0 - 32086*/
    /*cov 95475 +*/
    /*SPR 20554 Fix Start*/
    if(SOCKET_ERROR == close_wrapper(s1uIpv4SockRx_g))
    {
        LOG_MSG(L2_CLOSE_SOCKET_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                relayGtpuCurrentTick_g,\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
    }
    s1uIpv4SockRx_g = 0;
    /*SPR 20554 Fix End*/
    if(SOCKET_ERROR == close_wrapper(sockTx_g))
    {
        LOG_MSG(L2_CLOSE_SOCKET_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                relayGtpuCurrentTick_g,\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
        /*SPR 18490 +-*/
    }
    /*cov 95475 -*/
    sockTx_g = 0;
    adapterInitialized_g = 0;
}

/* + SPR 19022 */
/* - SPR 19022 */
/****************************************************************************
 * Function Name  : initSocketAddress6
 * Inputs         : s - sockaddr_in,
 *                  sockfd - socketFileDescriptor,
 *                  ipAddr - IP Address,
 *                  port - Port Number
 * Outputs        : None
 * Returns        : status (0-Failure,1-Successful)
 * Description    : To intialize the Socket address with specific socket file
 *                  descriptor and specific port for IPv6 socket.
 ****************************************************************************/
    /* SPR 14327 fix start */
static UInt8 initSocketAddress6(sockAddrIn6 *s,SInt32 *sockfd,UInt8 *ipAddr, UInt16 port)
    /* SPR 14327 fix end */
{
    if((*sockfd = socket_wrapper(AF_INET6, SOCK_DGRAM, 0)) == -1)
    {
        ltePanic("creating socket error\n");
        return 0;
    }
    (*s).sin6_family = AF_INET6;
    (*s).sin6_port = htons_wrapper(port);
    memCpy((*s).sin6_addr.s6_addr, ipAddr, IPV6_ADDR_LEN);
    (*s).sin6_flowinfo = 0;
    (*s).sin6_scope_id = 0;

    return 1;
}
/******************************************************************************
 *  Function Name  : addTunnelIdentier
 *  Inputs         : tunnelId - Tunnel ID,
 *                   ueId - UE ID,
 *                   lcId - Logical channel ID
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This function Stores the Tunnel Id, Ue Id and Lc Id 
 *                   In a Table.
 * ***************************************************************************/
void addTunnelIdentier(UInt32 tunnelId,UInt16 ueId,UInt8 lcId)
{
    if(ueId >= ADAPTER_MAX_UE)
    {
        /*SPR 2030 fix Start*/
        LOG_GTPU_MSG(PR_ADAPTER_INV_UEID_RECVD,LOGWARNING,PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g,__LINE__,
                ueId,0,0,0,0,0,__func__,"");
        /*SPR 2030 fix End*/
        return;
    }
    if(adapterDb_g[ueId].flag)
    {
        /*SPR 2030 fix Start*/
        LOG_GTPU_MSG(PR_ADAPTER_TUNNEL_ID_ALREADY_RCVD, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g,__LINE__,
                tunnelId,0,0,0,0,0,__func__,"");
        /*SPR 2030 fix End*/
        return;
    }
    if(lastUeInSystem_g < ueId)
    {
        lastUeInSystem_g = ueId;
    }

    /* Inserting the values into the Table */
    /*SPR 2030 fix Start*/
    LOG_GTPU_MSG(PR_ADAPTER_INSERT_TEID_UEID_LCID_IN_DB, LOGINFO, PR_GTPU_DL_DATA,
            relayGtpuCurrentTick_g,__LINE__,
            tunnelId,ueId,lcId,0,0,0,__func__,"");
    /*SPR 2030 fix End*/
    adapterDb_g[ueId].ueId = ueId;
    adapterDb_g[ueId].lcId = lcId;
    adapterDb_g[ueId].tId = tunnelId;

    /* Setting the flag on */
    adapterDb_g[ueId].flag = 1;
}

/****************************************************************************
 * Function Name  : initInternetSocket
 * Inputs         : configData - Initialization Configuration Data
 * Outputs        : None
 * Returns        : None
 * Description    : This function is the initialization function for opening 
 *                  Internet Sockets.
 ****************************************************************************/
void initInternetSocket(LP_ConfigFileFormat configData)
{
    /* SPR 23690 fix + */
    /* SPR 22248 MBMS Changes Start */
    SInt32 enabled = 1; 
    /* SPR 22248 MBMS Changes Stop */
    /* SPR 23690 fix - */
    /* Initialize Rx and Tx Sockets to communicate with traffic generator */
    /*IPV6_CHANGE - END*/
    if (IPV4_ADDR_PRESENT & configData->addrPresenceSt)
    {
                if(initSocketAddress(&sockAddrRx_g,&s1uIpv4SockRx_g,configData->adapterIP,configData->adapterRxPort) == 0)
                {
                    /* SPR 2030 fix Start*/
                    LOG_GTPU_MSG(L2_CREATE_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                            relayGtpuCurrentTick_g,__LINE__,
                            0,0 ,
                            0,0, 0,0, __func__, "");
                    /* SPR 2030 fix End*/
                }
        sockAddrRx_g.sin_addr.s_addr = inet_addr_wrapper(configData->adapterIP);
        /* SPR 23690 fix + */
        /* SPR 22248 MBMS Changes Start */
        if(setsockopt_wrapper(s1uIpv4SockRx_g, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled))<0)
        {
            ltePanic("setsockopt_wrapper SO_REUSEADDR failed");
        }
        /* SPR 22248 MBMS Changes Stop */
        /* SPR 23690 fix - */
/*SPR 21653 changes start*/
        /*SPR 20554 Fix Start*/
		if(bind_wrapper(s1uIpv4SockRx_g,&sockAddrRx_g,sizeof(SockAddr))== -1)
        /*SPR 20554 Fix End*/
		{
			/* SPR 2030 fix Start*/
            /* +- SPR 17486 */
			LOG_GTPU_MSG(L2_BIND_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
					relayGtpuCurrentTick_g,__LINE__,
					0,0 ,
					0,0, 0,0, __func__, "");
            /* +- SPR 17486 */
			/* SPR 2030 fix End*/
		}
/*SPR 21653 changes end*/
        /*SPR 20554 Fix Start*/
        if(fcntl3_wrapper(s1uIpv4SockRx_g, F_SETFL, O_NONBLOCK) < 0)
        /*SPR 20554 Fix End*/
        {
            ltePanic("fcntl()");
        }

    }
    if (IPV6_ADDR_PRESENT & configData->addrPresenceSt)
    {
         if(initSocketAddress6(&sockAddrRx6_g,&ipv6SockRx_g,configData->adapterIP6,configData->adapterRxPort) == 0)
        /*SPR 20554 Fix End*/
                {
                    LOG_GTPU_MSG(L2_CREATE_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                            relayGtpuCurrentTick_g,__LINE__,
                            0,0 ,
                            0,0, 0,0, __func__, "");
                }
/*SPR 21653 changes start*/
        /*NTL IPV6 changes start*/
            /*NTL IPV6 changes end*/
        /*SPR 20554 Fix Start*/
            if(bind_wrapper_addr6(ipv6SockRx_g, &sockAddrRx6_g, sizeof(sockAddrIn6))== -1)
        /*SPR 20554 Fix End*/
            {
                LOG_GTPU_MSG(L2_BIND_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                        relayGtpuCurrentTick_g,__LINE__,
                        0,0 ,
                        0,0, 0,0, __func__, "");
            }
/*SPR 21653 changes end*/
        /*SPR 20554 Fix Start*/
        if(fcntl3_wrapper(ipv6SockRx_g, F_SETFL, O_NONBLOCK) < 0)
        /*SPR 20554 Fix End*/
        {
            ltePanic("fcntl()");
        }
    }
}

/* - SPR 19022 */

/* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
/****************************************************************************
 * Function Name  : initInternetSocketForEmbms
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function is the initialization function for opening 
 *                  Internet Sockets For receiving eMBMS data.
 ****************************************************************************/
void initInternetSocketForEmbms(void)
{
    /*SPR 17973 fix start*/
    SInt32 enabled=1;
    /*SPR 17973 fix end*/
    /* Initialize Rx and Tx Sockets to communicate with traffic generator */

    /*IPV6_CHANGE - START*/
    if (IPV4_ADDR_PRESENT & configData->addrPresenceSt)
    {
        /* Need to change the port and adapter IP for EMBMS 
        GTP, on receiving TNL info shall create a GTP  Multicast tunnel with
        port 2152
        As per TG 29.061 section 20.3.1 " The destination UDP port for IP multicast transport shall be fixed as port number 927"
        */ 
		if(initSocketAddress(&sockAddrRxEmbms_g,&sockRxEmbms_g,configData->adapterIP,configData->adapterRxPort) == 0)
		{
			/* SPR 2030 fix Start*/
			LOG_MSG(L2_CREATE_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
					relayGtpuCurrentTick_g,__LINE__,
					0,0 ,
					0,0, 0,0, __func__, "");
			/* SPR 2030 fix End*/
		}
        sockAddrRxEmbms_g.sin_addr.s_addr = htonl_wrapper(INADDR_ANY);
    /*SPR 17973 fix start*/
        if(setsockopt_wrapper(sockRxEmbms_g, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled))<0)
        {
            ltePanic("setsockopt_wrapper SO_REUSEADDR failed");
        }
    /*SPR 17973 fix end*/
        //		LTE_LOG(LOG_INFO,PNULL," configData->adapterIP = %s port = %d\n",configData->adapterIP,configData->adapterRxPort);
     /*SPR 22975 Fix Start*/
        /*MULTISECTOR MACRO removed*/
     /*SPR 22975 Fix Stop*/
		if(bind_wrapper(sockRxEmbms_g,&sockAddrRxEmbms_g,sizeof(SockAddr))== -1)
		{
            ltePanic("bind_wrapper failed");
			/* SPR 2030 fix Start*/
			LOG_MSG(L2_BIND_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
					relayGtpuCurrentTick_g,__LINE__,
					0,0 ,
					0,0, 0,0, __func__, "");

            /* SPR 2030 fix End*/
        }
        if (setsockopt_wrapper(sockRxEmbms_g, IPPROTO_IP, IP_PKTINFO,&enabled, sizeof(enabled)) < 0)
        {    
            ltePanic("setsockopt_wrapper IP_PKTINFO failed");
        }          

     /*SPR 22975 Fix Start*/
        /*MULTISECTOR MACRO removed*/
     /*SPR 22975 Fix Stop*/
        if(fcntl3_wrapper(sockRxEmbms_g, F_SETFL, O_NONBLOCK) < 0)
        {
            ltePanic("fcntl()");
        }
    }

    if (IPV6_ADDR_PRESENT & configData->addrPresenceSt)
    {
        if(initSocketAddress6(&sockAddrRx6Embms_g,&sockRx6Embms_g,configData->adapterIP6,configData->adapterRxPort) == 0)
        {
            LOG_MSG(L2_CREATE_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0 ,
                    0,0, 0,0, __func__, "");
        }
        sockAddrRx6Embms_g.sin6_addr=in6addr_any;
     /*SPR 22975 Fix Start*/
        /*MULTISECTOR MACRO removed*/
     /*SPR 22975 Fix Stop*/
        if(setsockopt_wrapper(sockRx6Embms_g, IPPROTO_IPV6, IPV6_V6ONLY, &enabled, sizeof(enabled)))
        {
            ltePanic("setsockopt_wrapper IPV6_V6ONLY failed");
        }
        if(setsockopt_wrapper(sockRx6Embms_g, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled))<0)
        {
            ltePanic("setsockopt_wrapper SO_REUSEADDR failed");
        }

        if(bind_wrapper_addr6(sockRx6Embms_g, &sockAddrRx6Embms_g, sizeof(sockAddrIn6))== -1)
        {
            LOG_MSG(L2_BIND_SOCKET_FAIL_ID, LOGWARNING, L2_SYS_FAIL,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0 ,
                    0,0, 0,0, __func__, "");
            ltePanic("Binding failed");

        }
        if (setsockopt_wrapper(sockRx6Embms_g, IPPROTO_IPV6, IPV6_RECVPKTINFO,&enabled, sizeof(enabled)) < 0)
        {    
            ltePanic("setsockopt_wrapper IPV6_RECVPKTINFO failed");
		}
     /*SPR 22975 Fix Start*/
        /*MULTISECTOR MACRO removed*/
     /*SPR 22975 Fix Stop*/
        if(fcntl3_wrapper(sockRx6Embms_g, F_SETFL, O_NONBLOCK) < 0)
        {
			ltePanic("fcntl()");
		}
	}
	/*IPV6_CHANGE - END*/

}
/****************************************************************************
 * Function Name  : processEmbmsDataAndSendToSync 
 * Inputs         : recvBuffer - buffer received from BMSC
 * 		            bytesRead
 * Outputs        : 
 * Returns        :
 * Description    : This function handles BMSC data and after validation sends
 *                  the same to sync entity.
 ****************************************************************************/
void processEmbmsDataAndSendToSync(UInt8 *recvData_p,SInt16 bytesRead,HeaderInfo *headerInfo)
{
    UInt8 gtpHeaderLen = 0;
    egtpu_teid_t teid = 0;
    UInt32 size = 0;
    UInt8 offset = START_OFFSET_DL_GTP; 
    TEIDAreaLcTreeInfo *teidTreeNode_p = PNULL; 
    TEIDInfo *teidInfoNode_p = PNULL;
    UInt8 *pduData_p[MAX_AREA_LIST_COUNT];
    UInt8 listCount = 0;
    UInt8 nodeFound=LTE_FALSE; 
    UInt16 msgType =  EGTPU_GET_MSG_TYPE(&recvData_p[offset]);
    UInt8 ret=RELAY_GTPU_SUCCESS;
    gtpHeaderLen = GTP_MIN_HDR_LEN;
    size = bytesRead - (GTP_MIN_HDR_LEN + offset);
    if(bytesRead >= 20)
    {
        /* BMSC needs to send the TPDU msgType for eMBMS data packets */
        if (EGTPU_MSGT_TPDU ==  msgType)
        {
            /* Fetch the TEID from the GTPU header and 
             * find the node in the TEID tree maintained 
             * In case the data is received for the TEID which is not prsent in the tree, 
             * that data will not be handled
             * */
            teid = EGTPU_GET_TEID(&recvData_p[offset]);
            teidTreeNode_p = (TEIDAreaLcTreeInfo *)teidFindTreeNode(&teidTree_g,
                    &teid);

            if((teidTreeNode_p != PNULL))
            {
                teidInfoNode_p=(TEIDInfo *)getFirstListNode(&(teidTreeNode_p->teidInfoList));
                if(PNULL == teidInfoNode_p)
                {
                    //logging
                                LOG_MSG(PR_M1U_SEND_DATA_TO_SYNC_FAIL, LOGFATAL, L2_EMBMS_CAT,
                                        relayGtpuCurrentTick_g, __LINE__, 
                                        size, 0,0,0,0,0, __func__,
                                        "Get First Node from Tree Node Failed" );
                    freeMemPool(recvData_p);
                    return;

                }
                while(teidInfoNode_p)
                {

                    if((!memCmp(teidInfoNode_p->mIp,headerInfo->destIp,IPV6_ADDR_SIZE)) &&
                            (!memCmp(teidInfoNode_p->sIp,headerInfo->srcIp,IPV6_ADDR_SIZE)))//memCmp between mip and sip in tree and recieved in data header
                    {
                        if(teidInfoNode_p->listCount)
                        { 
                            nodeFound=LTE_TRUE; 
                            UInt8* data_p = (UInt8 *)msgAlloc(PNULL, 0, 0, 0);
                            if ( PNULL == data_p ) 
                            {
                                LOG_MSG(PR_DL_DATA_MSG_ALLOC_FAIL, LOGFATAL, L2_EMBMS_CAT,
                                        relayGtpuCurrentTick_g, __LINE__, 
                                        size, 0,0,0,0,0, __func__,
                                        " ADAPTER" );
                                return;
                            }
                            if (0 == qvMsgInsertExternal(data_p, 0, &recvData_p[offset + gtpHeaderLen], size,
#ifdef L2_FAST_PATH
                                        freeMemPool2Args,
#else
                                        lteFreeEgtpuBuff,
#endif

                                        (void *)recvData_p))
                            {
                                LOG_MSG(PR_DL_DATA_MSG_INSERT_FAIL, LOGFATAL, L2_EMBMS_CAT,
                                        relayGtpuCurrentTick_g, __LINE__,
                                        size, 0,0,0,0,0, __func__,
                                        " ADAPTER" );
                                /* Coverity 93753 Fix */
                                msgFree(data_p);
                                return;
                            }
                            //crashcheck();
                            pduData_p[0] = data_p;
                            for(listCount = 0; listCount < teidInfoNode_p->listCount ; listCount++)
                            {
                                if(listCount != 0)
                                {
                                    UInt8* tempData_p = getMemFromPool(BUFFERSIZE,PNULL);
                                    if (tempData_p != PNULL)
                                    {
                                        memCpy(tempData_p, recvData_p, bytesRead);
                                        pduData_p[listCount] = (UInt8 *)msgAlloc(PNULL, 0, 0, 0);
                                        if (0 == qvMsgInsertExternal(pduData_p[listCount], 0, &tempData_p[offset + gtpHeaderLen], size,
#ifdef L2_FAST_PATH
                                                    freeMemPool2Args,
#else
                                                    lteFreeEgtpuBuff,
#endif

                                                    (void *)tempData_p))
                                        {
                                            LOG_MSG(PR_DL_DATA_MSG_INSERT_FAIL, LOGFATAL, L2_EMBMS_CAT,
                                                    relayGtpuCurrentTick_g, __LINE__,
                                                    size, 0,0,0,0,0, __func__,
                                                    " ADAPTER" );
                                            /* Coverity 93753 Fix */
                                            msgFree(pduData_p[listCount]);
                                            freeMemPool(tempData_p);
                                            return;
                                        }
                                    }
                                }
                            }    
                            for(listCount = 0; listCount < teidInfoNode_p->listCount ; listCount++)
                            {
                                /* Cloning of the data PDU is required only when the list count
                                 * is more than 1 for a particulat TEID */
                                /*
                                if(listCount != 0)
                                {
                                    pduData_p = msgClone(data_p);
                                }
                                 */                                
                                pRelaySynceMbmsData *pRelaySynceMbmsData_p = PNULL;
                                pRelaySynceMbmsData_p = (pRelaySynceMbmsData *)
                                    getMemFromPool(sizeof(pRelaySynceMbmsData), PNULL);
                                if(PNULL == pRelaySynceMbmsData_p)
                                {
                                    LOG_MSG(PR_M1U_SEND_DATA_TO_SYNC_FAIL, LOGFATAL, L2_EMBMS_CAT,
                                            relayGtpuCurrentTick_g, __LINE__, 
                                            size, 0,0,0,0,0, __func__,
                                            "getMemFromPool Failed" );
                                    msgFree(pduData_p[listCount]);
                                    return;
                                }
                                /* Enqueue the data to SYNC entity for each (Area Id, LcId) tuple present 
                                 * corresponding to TEID received in the data from BMSC */
                                pRelaySynceMbmsData_p->areaId = teidInfoNode_p->areaLcList[listCount].areaId;
                                pRelaySynceMbmsData_p->lcId = teidInfoNode_p->areaLcList[listCount].lcId;
                                pRelaySynceMbmsData_p->data_p = pduData_p[listCount];
                                pRelaySynceMbmsData_p->dataSize = size;
                                if(( QUEUE_COUNT_SYNC_INTERFACE_Q(&pRelaySynceMbmsDataQueue_g)) < MAX_NODES )
                                {
                                    ENQUEUE_SYNC_PRELAY_NODE(pRelaySynceMbmsData_p);
                                }
                                else
                                {
                                    freeMemPool(pRelaySynceMbmsData_p);
                                    msgFree(pduData_p[listCount]);
				    LOG_GTPUPR_ERROR(L2_EMBMS_CAT ,"BMSC data dropped due to Sync Q full");
                                    return;
                                }
                            }
                        }
                        else
                        {
                            //logging
                        }
                        break;
                    }
                    else
                    {
                        teidInfoNode_p = ( TEIDInfo *)getNextListNode(&(teidInfoNode_p->teidAnchor));

                    }
                } 
                if(LTE_TRUE != nodeFound)
                {
                    LOG_MSG(PR_M1U_SEND_DATA_TO_SYNC_FAIL, LOGFATAL,
                            L2_EMBMS_CAT, relayGtpuCurrentTick_g, __LINE__,
                            0, 0,0,0,0,0,__func__,"No session running for this Info");
                    ret = RELAY_GTPU_FAILURE;
                }
            }

            else
            {    /*no node with this teid*/
                LOG_MSG(PR_M1U_SEND_DATA_TO_SYNC_FAIL, LOGFATAL,
                        L2_EMBMS_CAT, relayGtpuCurrentTick_g, __LINE__,
                        0, 0,0,0,0,0,__func__,"Incorrect Teid recieved");
                    //logging
                    ret = RELAY_GTPU_FAILURE;
            }
        }
        else
        {
            LOG_MSG(PR_M1U_SEND_DATA_TO_SYNC_FAIL, LOGFATAL,
                    L2_EMBMS_CAT, relayGtpuCurrentTick_g, __LINE__,
                    0, 0,0,0,0,0,__func__,"Incorrect msgType");
                    ret = RELAY_GTPU_FAILURE;
            /* Incorrect msgType PDU */
        }
    }
    else
    {
        LOG_MSG(PR_M1U_SEND_DATA_TO_SYNC_FAIL, LOGFATAL,
                L2_EMBMS_CAT, relayGtpuCurrentTick_g, __LINE__,
                0, 0,0,0,0,0,__func__,"Bytes Read less than 20");
                    ret = RELAY_GTPU_FAILURE;
    }
    if(RELAY_GTPU_SUCCESS != ret)
    {
        freeMemPool(recvData_p);
    }

}
#ifdef L2_FAST_PATH
/****************************************************************************
 * Function Name  : processEmbmsData 
 * Inputs         : recvBuffer - buffer received from BMSC
 * 		            bytesRead
 * Outputs        : 
 * Returns        :
 * Description    : This function process the data received from BMSC GW 
 * 		            and sends the same to Sync module if the TEID received 
 * 		            in the data is valid.
 ****************************************************************************/
void processEmbmsData( UInt8 *recvData_p, SInt16 bytesRead ,HeaderInfo *headerInfo)
{
    UInt8 offset = START_OFFSET_DL_GTP; 
    if(!(recvData_p[offset] & GTP_HDR_ALL_OPT_FIELD_SET))
    {
        processEmbmsDataAndSendToSync(recvData_p, bytesRead,headerInfo);
    }
    else
    {
        processPeerGtpuEmbmsData(recvData_p,bytesRead,headerInfo);
    }
}
#endif
/*******************************************************************************
 * Function Name  : dec_extn_hdr_embms_validation
 * Inputs         : p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion returns failure when -
 *                  extn header goes beyond the payload
 *                  not supported and comprehension reqd extn hdr type recd
 *                  else it is success
 *
 *                  in case of failure, error shall be logged, buffer is freed
 *                  in case of success, pdcp_num will be read
 *                  nd EGTPU_IE_PDCP_NUM_FLAG is set in ie_presence flag.
 *******************************************************************************/
static egtpu_return_t dec_extn_hdr_embms_validation( U8bit *p_start, U8bit *p_end )
{
    U8bit ext_hdr, *p_tmp = p_start + EGTPU_HDR_LEN_WITH_SEQ_NUM;

    do {
        if ((ext_hdr = *(p_tmp - 1))) /* Check the Next Extn Hdr Type value */
		{
			/** Verify the length in the extn header.
			**  Length is in multiple of 4 octets */
            if (!(*p_tmp) || ((p_tmp + 4 * (*p_tmp)) > (p_end + 1)))
			{
                return EGTPU_FAILURE;
            }

            if (ext_hdr == EGTPU_PDCP_PDU_EXT_HDR)
			{
                if (*p_tmp != EGTPU_LEN_OF_PDCP_PDU_EXT_HDR)
				{
                    return EGTPU_FAILURE;
                }

            }
			else if (ext_hdr & EGTPU_EXT_HDR_COMPREHENSION_REQD)
			{ /* comprehension reqd */
                return EGTPU_FAILURE;
            }
            p_tmp += (4 * (*p_tmp)); /** length is in multiple of 4 octets */
        }
		else
            break;
    } while (EGTPU_TRUE);

    return EGTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : validateGTPHeaderForEmbmsData 
 * Inputs         : gtpuDataMsg_p - buffer received from BMSC
 * Outputs        : 
 * Returns        : Validation success or Failure
 * Description    : This function handles and parses the BMSC data for 
 *                  GTP validation
 ****************************************************************************/
egtpu_return_t validateGTPHeaderForEmbmsData(UInt8 *gtpuDataMsg_p)
{
    U8bit    *p_buffer = gtpuDataMsg_p;
    U8bit    *p_start = NULL;/*EGTPU_API_GET_PAYLOAD_STRT_PTR (p_buffer),*/
    U8bit    *p_end = NULL;/*EGTPU_API_GET_PAYLOAD_END_PTR (p_buffer);*/
    U16bit    start = EGTPU_API_GET_START_PAYLOAD (p_buffer);
    U16bit    end = EGTPU_API_GET_END_PAYLOAD (p_buffer);
    U16bit   pdu_len, payload_len;
    p_start = (U8bit *)(p_buffer + start);
    p_end   = (U8bit *)(p_buffer + end);

    pdu_len = EGTPU_GET_MSG_LEN (p_start) + GTP_MSG_HDR_MIN_LEN;
    /*payload_len = (U16bit) ((*p_end) - (*p_start) + 1);*/
    payload_len = (end - start) + 1;

    if (pdu_len > payload_len)
    { /* less bytes have been rcvd */
        /* log error and discard the packet */
        LOG_MSG(GTPU_MSG_LESS_BYTES_RCVD, LOGWARNING, L2_EMBMS_CAT,
                relayGtpuCurrentTick_g, __LINE__, 
                pdu_len,payload_len,0,0, 0,0, __func__,"");

        if (payload_len < GTP_MSG_HDR_MIN_LEN)
        {
            LOG_MSG(GTPU_PAYLOAD_LESS_THAN_HDR_LEN, LOGWARNING, L2_EMBMS_CAT,
                    relayGtpuCurrentTick_g, __LINE__, 
                    GTP_MSG_HDR_MIN_LEN,payload_len,0,0, 0,0, __func__,"");

        }
        return EGTPU_FAILURE;
    }
    else if (pdu_len < payload_len)
    {
        p_end = p_start + pdu_len - 1; /* discard extra bytes rcvd */

        /* set end of payload as end of PDU, fix msg length too */
        /* peer is mis-behaving */
        EGTPU_API_SET_END_PAYLOAD (p_buffer, (p_end - p_buffer));
        EGTPU_API_SET_MSG_LEN (p_buffer, (p_end - p_buffer + 1));
    }

    /* protocol header validation */
    /* payload length shall atleast be GTP_MSG_HDR_MIN_LEN */
    if ((S32bit) (p_end - p_start + 1) < GTP_MSG_HDR_MIN_LEN)
    {
        LOG_MSG(GTPU_PAYLOAD_LESS_THAN_HDR_LEN, LOGDEBUG, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                GTP_MSG_HDR_MIN_LEN,(S32bit) (p_end - p_start + 1),0,0, 0,0, __func__,
                "");
        return EGTPU_FAILURE;
    }

    /* Version Number: shall be v1 */
    if (EGTPU_GET_VER_NUM(p_start) != GTP_VERSION)
    {
        LOG_MSG(GTPU_VERSION_INCORRECT, LOGDEBUG, L2_EMBMS_CAT,
                relayGtpuCurrentTick_g, __LINE__, 
                GTP_VERSION,EGTPU_GET_VER_NUM(p_start),0,0, 0,0, __func__,"");
        return EGTPU_FAILURE;
    }

    /* PT FLag: shall be 1 */
    if (EGTPU_GET_PT_FLAG (p_start) != EGTPU_PROTOCOL_TYPE)
    {
        LOG_MSG(GTPU_PROTOCOLTYPE_INCORRECT, LOGDEBUG, L2_EMBMS_CAT,
                relayGtpuCurrentTick_g, __LINE__, 
                EGTPU_PROTOCOL_TYPE,EGTPU_GET_PT_FLAG (p_start),0,0, 0,0, __func__,"");
        return EGTPU_FAILURE;
    }

    /* hdr-len is length of entire egtpu hdr */
    /* hdr-len will atleast be 12 when either of E, S, PN flag is set */
    if (*p_start & 0x07)
    {
        if ((S32bit)(p_end - p_start + 1) < EGTPU_HDR_LEN_WITH_SEQ_NUM)
        {
            LOG_MSG(GTPU_INSUFFICIENT_HDR_LEN_WITH_SEQ_NUM, LOGWARNING, 
                    PR_GTPU_DL_DATA,relayGtpuCurrentTick_g, __LINE__,
                    EGTPU_HDR_LEN_WITH_SEQ_NUM,(S32bit)(p_end - p_start + 1),
                    0,0, 0,0, __func__,""); 
            return EGTPU_FAILURE;
        }
    }
    if (EGTPU_GET_EXT_FLAG (p_start) &&
            dec_extn_hdr_embms_validation(p_start, p_end))
    {
        /* SPR 2030 fix Start*/
        LOG_MSG(GTPU_TPDU_EXT_HDR_DECODE_FAILURE, LOGWARNING, L2_EMBMS_CAT,
                relayGtpuCurrentTick_g, __LINE__,0,0,0,0,
                0,0, __func__,""); 
        /* SPR 2030 fix End*/
        return EGTPU_FAILURE;  /* error has been logged */
    }

    return EGTPU_SUCCESS;

}
/****************************************************************************
 * Function Name  : processPeerGtpuEmbmsData 
 * Inputs         : recvBuffer - buffer received from BMSC
 * 		            bytesRead
 * Outputs        : 
 * Returns        :
 * Description    : This function handles BMSC data through normal GTPU 
 *                  processing
 ****************************************************************************/
void processPeerGtpuEmbmsData(UInt8 *recvData_p,SInt16 bytesRead,HeaderInfo *headerInfo)
{
    egtpu_return_t ret = EGTPU_SUCCESS;
    /* Appending CSPL header for starting 64 bytes */
    appendCsplHeaderForDL(EI_UDP_MODULE_ID,GTPU_UNITDATA_IND,bytesRead,recvData_p);
    /* Validate GTP Header of eMBMS data */
    ret = validateGTPHeaderForEmbmsData(recvData_p);
    if(EGTPU_SUCCESS == ret)
    {
        processEmbmsDataAndSendToSync(recvData_p, bytesRead,headerInfo);
    }
    else
    {
        freeMemPool(recvData_p);
        /* Coverity 93579 Fix */
    }
}
#endif
/* eMBMS Code Changes End */
