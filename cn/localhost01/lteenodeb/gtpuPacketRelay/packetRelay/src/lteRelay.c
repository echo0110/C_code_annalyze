/***************************************************************************
*
* ARICENT -
*
* Copyright (C) 2014 Aricent Inc . All Rights Reserved.
*
****************************************************************************
*
* \$Id: lteRelay.c
*
****************************************************************************
*
* File Description: This file contains Data Structures and entry-point
*                   APIs for LTE Packet Relay functionality.
*
****************************************************************************
* Revision Details
* ----------------------
*
***************************************************************************/

#include "lteRelay.h"
#include "lteRelayOamSap.h"
#include "lteRelayS1Sap.h"
#include "lteMisc.h"
#include "lteRelayCommon.h"
#include "lteRelayAdapter.h"
#include "lteLayer2PdcpGtpuInterface.h"
#ifdef LTE_EMBMS_SUPPORTED
#include "lteRelayM2Sap.h"
#endif

#ifdef ENDC_ENABLED
#include "lteRelayX2uSap.h"
#include "lteRelay.h"
#include "lteX2uRelay.h"
#endif
relayConfigInfo relayConfigInfo_g;

/* Start of change for VARIBLE Declaration */
UInt8 myAddr[RELAY_ADDR_LEN_IPV6];
UInt8 myAddr4[RELAY_ADDR_LEN_IPV4];
UInt8 myAddr6[RELAY_ADDR_LEN_IPV6];
UInt8 myAddrType;
S1uUEReconfigCnfInfo s1uUEReconfigCnfInfo_g[RELAY_GTPU_MAX_UE];
relayS1ReconfigSapCnf s1ReconfigSapCnf_g;
transIdMappingForLM transIdMappingForLM_g[RELAY_MAX_LM_API];
SockAddr tgGlueAddress_g;
SInt32 glueTx_g;
sockAddrIn6 tgGlueAddress6_g;
SInt32 glueTx6_g;
/* 255 UE changes start */
TunnelIdMap relayMapBasedOnUeIndexLcId_g [RELAY_GTPU_MAX_UE][RELAY_GTPU_MAX_LC];
/* 255 UE changes end */
UeIndexLcIdTunnelIdMap relayMapBasedOnTransId_g[RELAY_GTPU_MAX_TRANS_ID];
/* LIPA changes start */
/* This is mapping Table for Secondary Tunnel Based upon Trans Id  */
UeIndexLcIdTunnelIdMap relayMapBasedOnTransIdForSecTunnel_g[RELAY_GTPU_MAX_TRANS_ID];
/* LIPA changes end */
relayDscpSockInfo relayDscpToSockFdMap_g[MAX_DSCP_VALUES];
relayDscpSockInfo relayDscpToSockFdMap6_g[MAX_DSCP_VALUES];
UInt32 g_transId;
/* End of change for VARIBLE Declaration */

/* LIPA changes start */
SInt32 secondaryTG_g;
SInt32 secondaryTG6_g;
SockAddr secondaryTGAddress_g;
sockAddrIn6 secondaryTGAddress6_g;
/* LIPA changes end */
/*SPR 21366 +*/
UInt8 gtpu_Pdcp_Instance_g[MAX_RRC_CELL_INDEX] = {0};
/*SPR 21366 -*/
extern egtpu_void_t *egtpu_malloc (U32bit p);

extern egtpu_void_t egtpu_sysfree (egtpu_void_t *p);

/*  + Layer2 NON CA Changes */  
extern void appendApiHeader(UInt16 moduleId, UInt16 apiId, UInt16 transId,
                            UInt16 msgLen, UInt8 *respBuff_p,RrcCellIndex cellId);
/*  - Layer2 NON CA Changes */  

/* Start of change for VARIBLE Declaration */
UInt32 (*processRelayMessage[RELAY_MAX_ADAPTER_API])(UInt8 *buffer_p, UInt16 msgLen);

UInt32 (*sendConfirmationRsp[RELAY_MAX_ADAPTER_API])
    (UInt8 *buffer_p,UInt16 transId, UInt16 totalLen);
/* End of change for VARIBLE Declaration */
extern void* udpSendToAdapter(Char8 *buff_p, Int len);
extern void* udpSendToS1ap(Char8 *buff_p, Int len);
extern void* udpSendToOam(Char8 *buff_p, Int len);

//extern void* udpSendToSocket(Char8 fd, SockAddr *toAddr,Char8 *buff_p, int len);
extern UInt8 udpSendToTG(UInt8 *respMsg, UInt16 current, Char8 interfaceType);
/* SPR 2030 fix Start */
 /* SPR 15909 fix start */
tickType_t relayGtpuCurrentTick_g = 0;
 /* SPR 15909 fix end */
/* SPR 2030 fix End */

/* warning removed*/
extern UInt16 getUeIdxFromRrcUeIdx(UInt16 rrcUeIdx);
/* Start of change for VARIBLE Declaration */
/*SPR 21545 Start*/
extern UInt8 instanceL2_g;
/*SPR 21545 End*/
/* Start of change for VARIBLE Declaration */
/* SPR 4506 Fix Start */
UInt8 egtpuLayerState_g = 0;
/* SPR 4506 Fix End */
#define MAX_STR_LENGTH 128
UInt8 Dscp[MAX_VALID_DSCP] =
{CS0,
 CS1,
 AF11,
 AF12,
 AF13,
 CS2,
 AF21,
 AF22,
 AF23,
 CS3,
 AF31,
 AF32,
 AF33,
 CS4,
 AF41,
 AF42,
 AF43,
/* SPR 19288 change start */
 CS5,
 EF,
 CS6,
 CS7
/* SPR 19288 change end */
};


/****************************************************************************
 * Function Name  : initGlueSockets
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion check and return vaid API identifier.
 ****************************************************************************/
/*SPR #2404 fix Start*/
/* + SPR 17439 */
void initGlueSockets(void)
/* - SPR 17439 */    
{
    /*IPV6_CHANGE - START*/
    if (IPV4_ADDR_PRESENT & relayConfigInfo_g.peerTgInfAddrType)
    {
        if( (glueTx_g = socket_wrapper(AF_INET, SOCK_DGRAM, 0 )) < 0 )
        {
            /*SPR 2030 fix Start*/
            LOG_GTPU_MSG(L2_CREATE_SOCKET_FAIL_ID,LOGWARNING,PR_GTPU_COMMON,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0,0,0,0,0,__func__,"[Error]DL-TG Rx fd Socket Opening");
            /*SPR 2030 fix End*/
        }
 
        /* Filling the Address Info for DL-TG */
        tgGlueAddress_g.sin_family = AF_INET;
        tgGlueAddress_g.sin_addr.s_addr = inet_addr_wrapper(relayConfigInfo_g.peerTgInf);
        tgGlueAddress_g.sin_port = htons_wrapper(TG_GLUE_PORT);
    }
    if (IPV6_ADDR_PRESENT & relayConfigInfo_g.peerTgInfAddrType)
    {
        if( (glueTx6_g = socket_wrapper(AF_INET6, SOCK_DGRAM, 0 )) < 0 )
        {
            /*SPR 2030 fix Start*/
            LOG_GTPU_MSG(L2_CREATE_SOCKET_FAIL_ID,LOGWARNING,PR_GTPU_COMMON,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0,0,0,0,0,__func__,"[Error]DL-TG Rx fd Socket Opening: IPV6");
            /*SPR 2030 fix End*/
        }
       
        /* Filling the Address Info for DL-TG */
        memSet(&tgGlueAddress6_g, 0, sizeof(tgGlueAddress6_g));
        tgGlueAddress6_g.sin6_family = AF_INET6;
        /*coverity 87491 fix start*/
        (void)inet_pton_wrapper(AF_INET6, relayConfigInfo_g.peerTgInfV6, &tgGlueAddress6_g.sin6_addr);
        /*coverity 87491 fix end*/
        tgGlueAddress6_g.sin6_port = htons_wrapper(TG_GLUE_PORT);
    }
       /*IPV6_CHANGE - END*/ 
    /* LIPA changes start */
    if (IPV4_ADDR_PRESENT & relayConfigInfo_g.secPeerTgInfAddrType)
    {
        /* IPV4 socket creation for Secondary DL-TG in case of LIPA Support */
        if( (secondaryTG_g = socket_wrapper(AF_INET, SOCK_DGRAM, 0 )) < 0 )
        {
            LOG_GTPU_MSG(L2_CREATE_SOCKET_FAIL_ID,LOGWARNING,PR_GTPU_COMMON,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0,0,0,0,0,__func__,"[Error]Secondary DL-TG Rx fd Socket Opening");
        }
        /* Filling the Address Info for Secondary peer DL-TG  for LIPA Support */
        secondaryTGAddress_g.sin_family = AF_INET;
        secondaryTGAddress_g.sin_addr.s_addr = inet_addr_wrapper(relayConfigInfo_g.secPeerTgInf);
        secondaryTGAddress_g.sin_port = htons_wrapper(SECONDARY_TG_PORT);
    }
    if (IPV6_ADDR_PRESENT & relayConfigInfo_g.secPeerTgInfAddrType)
    {
        /* IPV6 socket creation for Secondary DL-TG in case of LIPA Support */
        if( (secondaryTG6_g = socket_wrapper(AF_INET6, SOCK_DGRAM, 0 )) < 0 )
        {
            /*SPR 2030 fix Start*/
            LOG_GTPU_MSG(L2_CREATE_SOCKET_FAIL_ID,LOGWARNING,PR_GTPU_COMMON,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0,0,0,0,0,__func__,"[Error]Secondary DL-TG Rx fd Socket Opening: IPV6");
            /*SPR 2030 fix End*/
        }
       
        /* Filling the Address Info for Secondary peer DL-TG for LIPA Support  */
        memSet(&secondaryTGAddress6_g, 0, sizeof(secondaryTGAddress6_g));
        secondaryTGAddress6_g.sin6_family = AF_INET6;
        /*coverity 87491 fix start*/
        (void)inet_pton_wrapper(AF_INET6, relayConfigInfo_g.secPeerTgInfV6, &secondaryTGAddress6_g.sin6_addr);
        /*coverity 87491 fix end*/
        secondaryTGAddress6_g.sin6_port = htons_wrapper(SECONDARY_TG_PORT);
     }
     /* LIPA changes end */

}
/*SPR #2404 fix End*/

/****************************************************************************
 * Function Name  : checkAndMapApiValidity
 * Inputs         : apiId
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion check and return vaid API identifier.
 ****************************************************************************/

static UInt32 checkAndMapApiValidity(UInt16 apiId)
{
    /* Check for Control Plane APIs */
    if ((GTPU_CP_OUT_API_BASE <= apiId) && (GTPU_CP_LAST_OUT_API >= apiId))
    {
        if (GTPU_CP_SETUP_SAP_CNF == apiId)
            return RELAY_GTPU_CP_SETUP_SAP_CNF;
        else if (GTPU_CP_RELEASE_SAP_CNF == apiId)
            return RELAY_GTPU_CP_RELEASE_SAP_CNF;
        else if (GTPU_CP_ERROR_IND == apiId)
            return RELAY_GTPU_CP_ERROR_IND;
        else if (GTPU_CP_PATH_FAILURE_IND == apiId)
            return RELAY_GTPU_CP_PATH_FAILURE_IND;
        /*+SPR #3456*/
        else if (GTPU_CP_PATH_SUCCESS_IND == apiId)
            return RELAY_GTPU_CP_PATH_SUCCESS_IND;
        /*-SPR #3456*/
        /* +COVERITY 10295 */
        else 
           return RELAY_GTPU_CP_END_MARKER_IND;
        /* -COVERITY 10295 */
    } 
    /* Check for Relay APIs */
    else if ((GTPU_RELAY_OUT_API_BASE <= apiId) && (GTPU_RELAY_LAST_OUT_API >= apiId) && (GTPU_RELAY_DATA_IND == apiId))
    {
            return RELAY_GTPU_DATA_IND;
	/* coverity fix 10040 +- */
    }
    /* Check for Layer Management APIs */
    else if ((GTPU_LM_OUT_API_BASE <= apiId) && ( GTPU_LM_LAST_OUT_API >= apiId))
    {
        if (GTPU_LM_INITDB_CNF == apiId)
            return LM_INITDB_CNF;
        else if (GTPU_LM_SET_TIMER_CNF == apiId)
            return LM_SET_TIMER_CNF;
        else if (GTPU_LM_GET_TIMER_CNF == apiId)
            return LM_GET_TIMER_CNF;
        else if (GTPU_LM_GET_MAX_RETRIES_CNF == apiId)
            return LM_GET_MAX_RETIRES_CNF;
        else if (GTPU_LM_SET_MAX_RETRIES_CNF == apiId)
            return LM_SET_MAX_RETIRES_CNF;
        else if (GTPU_LM_INIT_TRACE_LEVEL_CNF == apiId)
            return LM_INIT_TRACE_LEVEL_CNF;
        else if (GTPU_LM_SET_TRACE_LEVEL_CNF == apiId)
            return LM_SET_TRACE_LEVEL_CNF;
        else if (GTPU_LM_GET_TRACE_LEVEL_CNF == apiId)
            return LM_GET_TRACE_LEVEL_CNF;
/* SPR 7085 Fix Start */
        else if (GTPU_LM_GET_STATS_CNF == apiId)
/* SPR 8933 Fix Start */
            return LM_GET_STATS_CNF;
        else if (GTPU_LM_GET_INIT_STATS_CNF == apiId)
            return LM_INIT_GET_STATS_CNF;
/* SPR 8933 Fix End */
/* SPR 7085 Fix End */
        else 
            return RELAY_GTPU_FAILURE;
    }
    else
    {
        return RELAY_GTPU_FAILURE;
    }
    /* SPR 4047 Fix Start */
    /* return RELAY_GTPU_FAILURE; */
    /* SPR 4047 Fix End */
}

/****************************************************************************
 * Function Name  : lteRelayMsgHandler
 * Inputs         : buf: Acutal buffer that needs to be processed
 *                  data
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion is handling data from GTP-U. This function is
 *                : entry point for all messages received by GTP-U Stack. 
 ****************************************************************************/
/* +- SPR 17777 */
void lteRelayMsgHandler(void *buf)
/* +- SPR 17777 */
{
   UInt8 * pMsg       =   (UInt8 *)buf;
   UInt16 apiID        =   0;
   UInt16 sourceModId  =   0;
   UInt32 validApiId   =   0;
   UInt32 size         =   0;
   sourceModId = RELAY_GTPU_GET_U16BIT(&pMsg[2]);
   apiID = RELAY_GTPU_GET_U16BIT(&pMsg[RELAY_API_ID_POS]);
   size = RELAY_GTPU_GET_U16BIT(&pMsg[RELAY_MSG_LEN_POS]);
   /* Coverty 73495 Fix Start*/
   if(size > EGTPU_MAX_MSG_SIZE)
   {
       return;
   }
   /* Coverty 73495 Fix End*/

   if (GTPU_RELAY_UNITDATA_REQ == apiID)
   {
      sendPacketToPeerGtpu(pMsg);
      return;
   }
   if (GTPU_RELAY_DATA_IND == apiID)
   {
      processGtpuAdapterDataInd(pMsg,size);
      return;
   }
   if(RELAY_GTPU_PDCP_END_MARKER_IND == apiID)
   {
       processGtpuAdapterEndMarkerInd(pMsg,size);
       return;
   }

   if (GTPU_MODULE_ID == sourceModId)
   {
       if ((validApiId = checkAndMapApiValidity(apiID))) {
           apiID = validApiId;
       }
       else
       {
           /*SPR 2030 fix Start*/
           LOG_GTPU_MSG(PR_UNSUPPORTED_API_RCVD, LOGWARNING, PR_GTPU_COMMON,
                   relayGtpuCurrentTick_g,__LINE__,apiID,
                   0,0,0, 0,0, __func__,"SrcModId:GTPU_MODULE_ID");
           /*SPR 2030 fix End*/
           
           return;
       }
   } 
   else if (RELAY_OAM_MODULE_ID == sourceModId)
   {
       if (RELAY_REQ_API_BASE < apiID || LM_GET_TRACE_LEVEL_REQ > apiID){ 
           /*SPR 2030 fix Start*/
           LOG_GTPU_MSG(PR_UNSUPPORTED_API_RCVD, LOGWARNING, PR_GTPU_COMMON,
                   relayGtpuCurrentTick_g,__LINE__,apiID,0,0,0, 0,0,
                   __func__,"SrcModId:RELAY_OAM_MODULE_ID");
           /*SPR 2030 fix End*/
           return;
       }
   }
   else if (RELAY_S1AP_MODULE_ID == sourceModId)
   {
       if (RELAY_GTPU_SETUP_SAP_REQ != apiID && RELAY_GTPU_RELEASE_SAP_REQ != apiID) 
       {  
           /*SPR 2030 fix Start*/
           LOG_GTPU_MSG(PR_UNSUPPORTED_API_RCVD, LOGWARNING, PR_GTPU_COMMON,
                   relayGtpuCurrentTick_g,__LINE__,apiID,0,0,0, 0,0, 
                   __func__,"SrcModId:RELAY_S1AP_MODULE_ID");
           /*SPR 2030 fix End*/
           return;
       }
   }
   else if (RELAY_ADAPTER_MODULE_ID == sourceModId)
   {
       if (RELAY_GTPU_DATA_REQ != apiID)
       {
           /*SPR 2030 fix Start*/
           LOG_GTPU_MSG(PR_UNSUPPORTED_API_RCVD, LOGWARNING, PR_GTPU_COMMON,
                   relayGtpuCurrentTick_g,__LINE__,apiID,0,0,0, 0,0,
                   __func__,"SrcModId:RELAY_ADAPTER_MODULE_ID");
           /*SPR 2030 fix End*/
           return;
      }
   }
   else 
   {
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(PR_WRONG_MOD_ID_RCVD,LOGWARNING,PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g,__LINE__,sourceModId,
                0,0,0, 0,0, __func__, "");
        /* SPR 2030 fix End*/
       return;
   } 
   /* Now Handling the request */
   if (RELAY_GTPU_SUCCESS != processRelayMessage[apiID](pMsg,size))
   {
       LOG_GTPUPR_ERROR(PR_GTPU_CONTROL,"API ID :%u received from GTP-U is not correct", apiID);
   }
}


/******************************************************************************
 * Function Name  : recvFromRelaySockHandler 
 * Inputs         : rcvBuffer,
 *                  rcvBuffer_len,
 *                  moduleId
 *                  sockAddr - socket Address from which data packet received
 * Outputs        : None
 * Returns        : bytesRead
 * Description    : This funtion calls appropriate function handler. This 
 *                : function is entry point for OAM and S1-AP Interfaces.
 *****************************************************************************/
SInt32 recvFromRelaySockHandler(UInt8 *rcvBuffer,UInt32 rcvBuffer_len, 
        UInt32 moduleId, SockAddr *sockAddr )
{
    SInt32 bytesRead = 0;
    if (RELAY_S1AP_MODULE_ID == moduleId)
    {
        /* SPR 6949 Fix Start */
        bytesRead = recvfrom_wrapper(relayConfigInfo_g.s1ApTxFd,rcvBuffer,rcvBuffer_len,0,PNULL,0);
        /* SPR 6949 Fix Start */
    }
    /*SPR 21188 +*/
#if defined PDCP_GTPU_INTF
    else if((PDCP_MODULE_ID == moduleId))
    {
        /*SPR 21366 +*/
        bytesRead = recvfrom_wrapper(relayConfigInfo_g.gtpuRxFd,rcvBuffer,rcvBuffer_len,0,sockAddr,sizeof(SockAddr));
        /*SPR 21366 -*/
    }
#endif
    /*SPR 21188 -*/
    else
    {
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(PR_WRONG_MOD_ID_RCVD,LOGWARNING,PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g,__LINE__,moduleId,
                0,0,0, 0,0, __func__, "Expected:RELAY_S1AP_MODULE_ID");
        /* SPR 2030 fix End*/
    }
    return bytesRead;
}


/******************************************************************************
 * Function Name  : lteRelaySockHandler 
 * Inputs         : rcvBuffer,
 *                  bytesRead
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion calls appropriate function handler. This 
 *                : function is entry point for OAM and S1-AP Interfaces.
 *****************************************************************************/
void lteRelaySockHandler(UInt8 *rcvBuffer,SInt32 bytesRead)
{
    UInt16 apiId = 0, current = 0, msgLen = 0;
    UInt8 *recvBuff = rcvBuffer;
    /* + Layer2 NON CA Changes */ 
    RrcCellIndex      cellId = INVALID_CELL_INDEX; // External Cell Index from L3
    /* - Layer2 NON CA Changes */ 
    /*Coverity 10310 Fix Start*/
    if ( -1 != bytesRead && rcvBuffer != PNULL)
    {
    /*Coverity 10310 Fix End*/
        apiId = RELAY_GTPU_GET_U16BIT(&recvBuff[current + RELAY_API_ID_POS]);
        msgLen = RELAY_GTPU_GET_U16BIT(&recvBuff[current + RELAY_MSG_LEN_POS]);

        /* + Layer2 NON CA Changes */ 
        cellId = recvBuff[API_HEADER_CELL_INDEX_OFFSET]; // External Cell Index from L3
        /* Since PR is running as a seperate process, update the Layer2 
         * internalCellIndex to RRC-OAM CellIndex mapping */
        /* Validate the cell index for range */
#ifdef LTE_EMBMS_SUPPORTED
        if((apiId < M1U_RELAY_MIN_API_ID) || (apiId > M1U_RELAY_MAX_API_ID))
	{
#endif
		if (MAX_RRC_CELL_INDEX < cellId)
		{
			LOG_GTPUPR_ERROR(PR_GTPU_CONTROL,"Cell ID :%u received from L3+ is not correct", cellId);
			return;
		}
#ifdef LTE_EMBMS_SUPPORTED
	}
#endif
        /*SPR 18132 fix end*/
      if (RELAY_GTPU_SUCCESS != processRelayMessage[apiId](recvBuff,msgLen))
      {
        LOG_GTPUPR_ERROR(PR_GTPU_CONTROL,"API ID :%u received from L3+ is not correct", apiId);

        return;
      }
      /* SPR 4506 Fix Start */
      egtpuLayerState_g = 1;
      /* SPR 4506 Fix End */
    }
    else
    {
        return;
    }
}

/******************************************************************************
 * Function Name  : udpBindRxSocket 
 * Inputs         : fd - File descriptor,
 *                  sockAddr_p - Pointer to SockAddr,
 *                  port
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion bind UDP socket.
 *****************************************************************************/
UInt32 udpBindRxSocket(UInt8 fd, SockAddr *sockAddr_p, UInt32 port)
{
    sockAddr_p->sin_addr.s_addr = inet_addr_wrapper((Char8 *)relayConfigInfo_g.adapterSocketInf);
/*INTEL_T3300 -*/
    sockAddr_p->sin_family = AF_INET;
    sockAddr_p->sin_port = htons_wrapper(port);
    if (SOCKET_BIND_FAIL == bind_wrapper(fd,sockAddr_p,sizeof(SockAddr)))
    {
        /*SPR 2030 fix Start*/
        LOG_GTPU_MSG(L2_BIND_SOCKET_FAIL_ID,LOGWARNING,L2_SYS_FAIL,
                relayGtpuCurrentTick_g,__LINE__,
                0,0,0,0, 0,0, __func__,"");
        /*SPR 2030 fix End*/
        return RELAY_GTPU_FAILURE;
    }
    return RELAY_GTPU_SUCCESS;
}

/******************************************************************************
 * Function Name  : prepareSocketForSndRcv 
 * Inputs         : None
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion open socket interfaces.
 *****************************************************************************/

UInt32 prepareSocketForSndRcv(void)
{
    struct hostent *host_p = PNULL; 
    Int flag_non_blocking;
    Int ipv6_tos = 0;
    UInt8 i,dscpIndex = 0,tos = 0;
    /* SPR 4047 Fix Start */
    SInt8 resp = 0;
    /* SPR 4047 Fix End */
    SInt8 sockfd = RELAY_INVALID_FD;

    if (RELAY_INVALID_FD != 
        (relayConfigInfo_g.oamS1ApRxFd = socket_wrapper(AF_INET,SOCK_DGRAM,0)))
    {
        relayConfigInfo_g.oamToAddr.sin_family = AF_INET;
        relayConfigInfo_g.oamToAddr.sin_port = htons_wrapper(relayConfigInfo_g.oamRxPort);
        host_p = (struct hostent *)gethostbyname_wrapper((Char8*)relayConfigInfo_g.oamSocketInf);
        relayConfigInfo_g.oamToAddr.sin_addr = *(struct in_addr *)host_p->h_addr;
        host_p = PNULL;

        /*SPR 2030 fix Start*/
        LOG_GTPU_MSG(PR_SOCKET_OPEN_SUCCESS, LOGINFO, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g,__LINE__,
                relayConfigInfo_g.oamRxPort,0,0,0,
                0,0,__func__,"OAM-RX-FD");
        /*SPR 2030 fix End*/

        relayConfigInfo_g.s1ApToAddr.sin_family = AF_INET;
        relayConfigInfo_g.s1ApToAddr.sin_port = htons_wrapper(relayConfigInfo_g.s1ApRxPort);
        host_p = (struct hostent *)gethostbyname_wrapper((Char8*)relayConfigInfo_g.s1ApSocketInf);
        relayConfigInfo_g.s1ApToAddr.sin_addr = *(struct in_addr *)host_p->h_addr;
	host_p = PNULL;
	/*SPR 2030 fix Start*/
        LOG_GTPU_MSG(PR_SOCKET_OPEN_SUCCESS, LOGINFO, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g,__LINE__,
                relayConfigInfo_g.s1ApRxPort,0,0,0,
                0,0,__func__,"S1AP-RX-FD");
        /*SPR 2030 fix End*/
    }
    if ( RELAY_INVALID_FD !=
        (relayConfigInfo_g.s1ApTxFd = socket_wrapper(AF_INET,SOCK_DGRAM,0)))
    {
        /* + coverity 32085 */
        if((flag_non_blocking = fcntl2_wrapper(relayConfigInfo_g.s1ApTxFd, F_GETFL)) < 0) 
        {
            LTE_LOG(LOG_INFO,"Not able to get flags for S1 Tx file des");
            return RELAY_GTPU_FAILURE;
        }
        /* - coverity 32085 */
  
        flag_non_blocking |= O_NONBLOCK;   
        if ( RELAY_GTPU_SUCCESS == 
           udpBindRxSocket(relayConfigInfo_g.s1ApTxFd,&relayConfigInfo_g.s1ApFromAddr,
               relayConfigInfo_g.s1ApTxPort))
        {
            /*SPR 2030 fix Start*/
            LOG_GTPU_MSG(PR_SOCKET_OPEN_SUCCESS, LOGINFO, PR_GTPU_COMMON,
                    relayGtpuCurrentTick_g,__LINE__,
                    relayConfigInfo_g.s1ApTxPort,0,0,0,
                    0,0,__func__," OAM-S1AP-TX-FD");
            /*SPR 2030 fix End*/
            /* COVERITY 10290*/
            if(-1 == fcntl3_wrapper(relayConfigInfo_g.s1ApTxFd, F_SETFL, flag_non_blocking))
            {
              ltePanic("Unable to perform F_SETFL\n"); 
            }
            /* COVERITY 10290*/
        }
    }
#if defined PDCP_GTPU_INTF
    if ( RELAY_INVALID_FD !=
        (relayConfigInfo_g.gtpuRxFd = socket_wrapper(AF_INET,SOCK_DGRAM,0)))
    {
        if((flag_non_blocking = fcntl2_wrapper(relayConfigInfo_g.gtpuRxFd, F_GETFL)) < 0) 
        {
            LOG_GTPUPR_ERROR( PR_GTPU_COMMON, "Not able to get flags for S1 Tx file des");
            return RELAY_GTPU_FAILURE;
        }

        flag_non_blocking |= O_NONBLOCK;   
        if ( RELAY_GTPU_SUCCESS == 
                udpBindRxSocket(relayConfigInfo_g.gtpuRxFd,&relayConfigInfo_g.gtpuFromAddr,
                    relayConfigInfo_g.gtpuRxPort))
        {
            LOG_GTPU_MSG(PR_SOCKET_OPEN_SUCCESS, LOGINFO, PR_GTPU_COMMON,
                    relayGtpuCurrentTick_g,__LINE__,
                    relayConfigInfo_g.s1ApTxPort,0,0,0,
                    0,0,__func__," OAM-S1AP-TX-FD");
            if(-1 == fcntl3_wrapper(relayConfigInfo_g.gtpuRxFd, F_SETFL, flag_non_blocking))
            {
                ltePanic("Unable to perform F_SETFL\n"); 
            }
        }
    }
#endif
   for(i = 0; i< MAX_VALID_DSCP; i++) 
   {
       dscpIndex = Dscp[i];  

       if(RELAY_INVALID_FD != 
               (relayDscpToSockFdMap_g[dscpIndex].sockfd = socket_wrapper(AF_INET,SOCK_DGRAM, 0)))
/*INTEL_T3300 -*/
           {
               sockfd = relayDscpToSockFdMap_g[dscpIndex].sockfd;


               /*Set the proper TOS value for the socket, 
                *DSCP is first 6 bits of 8 bit TOS field in IP header*/ 
               tos = dscpIndex << 2; /*dscpIndex itself is DSCP field*/
          /* SPR 4047 Fix Start */
               resp = setsockopt_wrapper(sockfd,IPPROTO_IP,IP_TOS,&tos,sizeof(tos));
               if(0 == resp)
               {
                   LOG_GTPU_MSG(PR_SOCKET_OPEN_SUCCESS, LOGINFO, PR_GTPU_COMMON,
                           relayGtpuCurrentTick_g,__LINE__,
                           sockfd,0,0,0,
                           0,0,__func__," QCI_SOCKET_FD");
               }
               else
               {
                   LTE_LOG(LOG_INFO,"SetSocket return failure ");
                   return RELAY_GTPU_FAILURE;
               }
          /* SPR 4047 Fix End */
           }
           else
           {
               LOG_GTPU_MSG(L2_CREATE_SOCKET_FAIL_ID,LOGWARNING,PR_GTPU_COMMON,
                       relayGtpuCurrentTick_g,__LINE__,
                       0,0,0,0,0,0,__func__,"[Error]DSCP fd Socket Opening");
           }
       /*IPV6_CHANGE - START*/
       if(RELAY_INVALID_FD != 
               (relayDscpToSockFdMap6_g[dscpIndex].sockfd = socket_wrapper(AF_INET6,SOCK_DGRAM, 0)))
       {
           sockfd = relayDscpToSockFdMap6_g[dscpIndex].sockfd;

           ipv6_tos = dscpIndex << 2; /*dscpIndex itself is DSCP field*/
           resp = setsockopt_wrapper(sockfd,IPPROTO_IPV6,IPV6_TCLASS,&ipv6_tos,sizeof(ipv6_tos));
           if(0 == resp)
           {
               LOG_GTPU_MSG(PR_SOCKET_OPEN_SUCCESS, LOGINFO, PR_GTPU_COMMON,
                       relayGtpuCurrentTick_g,__LINE__,
                       sockfd,0,0,0,
                       0,0,__func__," QCI_SOCKET_FD");
           }
           else
           {
               LTE_LOG(LOG_INFO,"SetSocket return failure for IPv6 TClass.");
               return RELAY_GTPU_FAILURE;
           }
       }
       else
       {
           LOG_GTPU_MSG(L2_CREATE_SOCKET_FAIL_ID,LOGWARNING,PR_GTPU_COMMON,
                   relayGtpuCurrentTick_g,__LINE__,
                   0,0,0,0,0,0,__func__,"[Error]DSCP fd Socket Opening");
       }
      /*IPV6_CHANGE - END*/
   }

/* SPR #2294 fix Start*/
/* SPR #2294 fix End*/
        
   return RELAY_GTPU_SUCCESS;
}

/******************************************************************************
 * Function Name  : udpSocketSetBlocking 
 * Inputs         : fd - file descriptor,
 *                  blockmode (1 for yes)
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion set a blocking/nonblocking socket.
 *****************************************************************************/
UInt32 udpSocketSetBlocking(UInt8 fd, UInt32 blockMode)
{
    SInt32 flags = 0;
    /* + SPR 17439 */
    SInt32 newFlags = 0;
    /* - SPR 17439 */
    if((flags = fcntl2_wrapper(fd,F_GETFL)) < 0)
    {
        LTE_LOG(LOG_INFO,"Not able to get flags for file des");
        return RELAY_GTPU_FAILURE;
    }  
    if(blockMode)
        newFlags = flags & ~O_NONBLOCK;
    else
        newFlags = flags | O_NONBLOCK;
   
    if (flags  != newFlags)
    {
        if (fcntl3_wrapper(fd,F_SETFL,newFlags) < 0)
        {
           LTE_LOG(LOG_INFO,"Not able to set flags for file des ");
            return RELAY_GTPU_FAILURE;
        }
    }
    return RELAY_GTPU_SUCCESS;
}

/******************************************************************************
 * Function Name  : initPacketRelayOamFunPtr
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion initalise function pointers for OAM Interface.
 *****************************************************************************/
void initPacketRelayOamFunPtr(void)
{
    processRelayMessage[LM_INITDB_REQ] = processGtpuInitDbReq;
    processRelayMessage[LM_SET_TIMER_REQ] = processGtpuSetTimerReq;
    processRelayMessage[LM_GET_TIMER_REQ] = processGtpuGetTimerReq;
    processRelayMessage[LM_GET_MAX_RETIRES_REQ] = processGtpuGetMaxRetriesReq;
    processRelayMessage[LM_SET_MAX_RETIRES_REQ] = processGtpuSetMaxRetriesReq;
    processRelayMessage[LM_INIT_TRACE_LEVEL_REQ] = processGtpuInitTraceLvlReq;
    processRelayMessage[LM_SET_TRACE_LEVEL_REQ] = processGtpuSetTraceLvlReq;
    processRelayMessage[LM_GET_TRACE_LEVEL_REQ] = processGtpuGetTraceLvlReq;

    processRelayMessage[LM_INITDB_CNF] = processGtpuInitDbCnf;
    processRelayMessage[LM_SET_TIMER_CNF] = processGtpuSetTimerCnf;
    processRelayMessage[LM_GET_TIMER_CNF] = processGtpuGetTimerCnf;
    processRelayMessage[LM_GET_MAX_RETIRES_CNF] = processGtpuGetMaxRetriesCnf;
    processRelayMessage[LM_SET_MAX_RETIRES_CNF] = processGtpuSetMaxRetriesCnf;
    /* SPR 7085 fix Start */
    processRelayMessage[LM_GET_STATS_CNF] = processGtpuGetStatsResp;
    processRelayMessage[LM_INIT_GET_STATS_CNF] = processGtpuGetStatsResp;
    /* SPR 7085 fix End */
    processRelayMessage[LM_INIT_TRACE_LEVEL_CNF] = processGtpuInitTraceLvlCnf;
    processRelayMessage[LM_SET_TRACE_LEVEL_CNF] = processGtpuSetTraceLvlCnf;
    processRelayMessage[LM_GET_TRACE_LEVEL_CNF] = processGtpuGetTraceLvlCnf;
    /* SPR 2030 fix Start */
    processRelayMessage[PR_GTPU_SET_LOG_LEVEL_REQ] = processGtpuSetLogLevelReq;
    processRelayMessage[PR_GTPU_GET_LOG_LEVEL_REQ] = processGtpuGetLogLevelReq;
    processRelayMessage[PR_GTPU_ENABLE_LOG_CATEGORY_REQ] = 
        processGtpuEnableLogCategoryReq;
    processRelayMessage[PR_GTPU_DISABLE_LOG_CATEGORY_REQ] = 
        processGtpuDisableLogCategoryReq;
    processRelayMessage[PR_GTPU_GET_LOG_CATEGORY_REQ] = 
        processGtpuGetLogCategoryReq;
    /*HSM_CHG_START*/
    processRelayMessage[PR_GTPU_PROC_SUP_REQ] =
        processGtpuGetHealthMonitoringReq;
    /*HSM_CHG_END*/
    /* SPR 2030 fix End */
    /* SPR 7085 fix Start */
    processRelayMessage[PR_GTPU_GET_STATS_REQ] = processGtpuGetStatsReq;
    /* SPR 7085 fix End */
    /* Alarm Fix for Path Fail Start */
    /* Update it after consult with OAM because existing API requires changes */
    //processRelayMessage[PR_GTPU_GET_STATS_REQ] = processGtpuAlarmNotif;
    /* Alarm Fix for Path Fail End */
	/* SPR 13765 fix Start */
    processRelayMessage[PR_GTPU_ERROR_IND] = processGtpuErrorInd;
	/* SPR 13765 fix END */
	/*spr 22474 start*/
	processRelayMessage[OAM_GTPU_GET_DEBUG_INFO_REQ] = processGtpuGetDebugInfoReq;
    /*spr 22474 end*/
    return;
}

/******************************************************************************
 * Function Name  : initPacketRelayS1ApFunPtr 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion initalise function pointers for S1-AP Interface. 
 ******************************************************************************/

void initPacketRelayS1ApFunPtr(void)
{
    processRelayMessage[S1U_RECONFIGURE_UE_ENTITY_REQ]
        = processGtpuSetupSapReq;
    processRelayMessage[S1U_CREATE_UE_ENTITY_REQ]
        = processCreateUeRequest;
    processRelayMessage[S1U_DELETE_UE_ENTITY_REQ]
        = processGtpuDeleteUeReq;
    processRelayMessage[RELAY_GTPU_CP_ERROR_IND]
        = processGtpuCpErrorInd;
    processRelayMessage[RELAY_GTPU_CP_PATH_FAILURE_IND]
        = processGtpuCpPathFailureInd;
    /*+SPR #3456*/
    processRelayMessage[RELAY_GTPU_CP_PATH_SUCCESS_IND]
        = processGtpuCpPathSuccessInd;
    /*-SPR #3456*/
    processRelayMessage[RELAY_GTPU_CP_SETUP_SAP_CNF]
        = processGtpuSetupSapCnf;
    processRelayMessage[RELAY_GTPU_CP_RELEASE_SAP_CNF] 
        = processGtpuReleaseSapCnf;

    processRelayMessage[RELAY_GTPU_CP_END_MARKER_IND]
        = processGtpuCpEndMarkerInd;
    /* Intra-eNB HO Changes Start*/
    processRelayMessage[RRC_S1U_INTRA_ENB_DATA_FWD_REQ]
        = processS1uIntraEnbDataFwdReq;
    processRelayMessage[RRC_S1U_LOCAL_PATH_SWITCH_REQ]
        = processS1uLocalPathSwitchReq;
    /* Intra-eNB HO Changes End*/
    /* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
    processRelayMessage[M1U_SESSION_START]
        = processM1uSessionStartRequest;
    processRelayMessage[M1U_SESSION_STOP]
        = processM1uSessionStopRequest;
    processRelayMessage[M1U_SESSION_UPDATE]
        = processM1uSessionUpdateRequest;
    processRelayMessage[M1U_CREATE_AREA_ENTITY_REQ]
        = processM1uCreateAreaEntityReq;
    processRelayMessage[M1U_DELETE_AREA_ENTITY_REQ]
        = processM1uDeleteAreaEntityReq;
#endif
    /* eMBMS Code Changes End */

#ifdef ENDC_ENABLED
    processRelayMessage[X2U_LC_RELEASE_IND] = processX2uLcReleaseInd;
    processRelayMessage[X2U_SUSPEND_UE_ENTITY_REQ] = processX2uSuspendUEEntityRequest;
#endif

}

/****************************************************************************
 * Function Name  : initPacketRelayAdapterFunPtr
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion initalise function pointers for Adapter.
 ****************************************************************************/

void initPacketRelayAdapterFunPtr(void)
{
    processRelayMessage[RELAY_GTPU_DATA_REQ] = processGtpuAdapterDataReq;
    processRelayMessage[RELAY_GTPU_DATA_IND] = processGtpuAdapterDataInd;
}

/****************************************************************************
 * Function Name  : initPrepareAndSendRspFunPtr
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion initalise function pointers for prepare and 
 *                  send response to respective module.
 ****************************************************************************/
void initPrepareAndSendRspFunPtr(void)
{
    sendConfirmationRsp[LM_INITDB_CNF]
        = processRelayInitDbCnf;
    sendConfirmationRsp[LM_SET_TIMER_CNF] 
        = processRelaySetTimerCnf;
    sendConfirmationRsp[LM_GET_TIMER_CNF]
        = processRelayGetTimerCnf;
    sendConfirmationRsp[LM_GET_MAX_RETIRES_CNF]
        = processRelayGetMaxRetriesCnf;
    sendConfirmationRsp[LM_SET_MAX_RETIRES_CNF]
        = processRelaySetMaxRetriesCnf;
    sendConfirmationRsp[LM_INIT_TRACE_LEVEL_CNF]
        = processRelayInitTraceLvlCnf;
    sendConfirmationRsp[LM_SET_TRACE_LEVEL_CNF] 
        = processRelaySetTraceLvlCnf;
    sendConfirmationRsp[LM_GET_TRACE_LEVEL_CNF]
        = processRelayGetTraceLvlCnf;
    sendConfirmationRsp[RELAY_GTPU_CP_SETUP_SAP_CNF]
        = processRelaySetupSapCnf;
    sendConfirmationRsp[RELAY_GTPU_CP_RELEASE_SAP_CNF]
        = processRelayReleaseSapCnf;
    sendConfirmationRsp[RELAY_GTPU_CP_ERROR_IND] 
        = processRelayCpErrorSapInd;
    sendConfirmationRsp[RELAY_GTPU_CP_PATH_FAILURE_IND]
        = processRelayPathFailureInd;
    /*+SPR #3456*/
    sendConfirmationRsp[RELAY_GTPU_CP_PATH_SUCCESS_IND]
        = processRelayPathSuccessInd;
    /*-SPR #3456*/
    sendConfirmationRsp[RELAY_GTPU_DATA_IND]
        = processRelayDataInd;

    /* SPR 2030 fix Start*/
    sendConfirmationRsp[PR_GTPU_GET_LOG_LEVEL_RESP]
        = processRelayGetLogLevelResp;
    sendConfirmationRsp[PR_GTPU_GET_LOG_CATEGORY_RESP]
        = processRelayGetLogCategoryResp;
    /* SPR 2030 fix End*/
    
    sendConfirmationRsp[RELAY_GTPU_CP_END_MARKER_IND]
        = processRelayCpEndMarkerInd;
    sendConfirmationRsp[RELAY_GTPU_PDCP_END_MARKER_IND]
        = processRelayPdcpEndMarkerInd;
    /* SPR 7085 fix start */
    sendConfirmationRsp[PR_GTPU_GET_STATS_RESP]
        = processRelayGetStatsResp;
    /* SPR 7085 fix end */
	/*spr 22474 start*/
    sendConfirmationRsp[GTPU_OAM_GET_DEBUG_INFO_RESP]
        = processRelayGetDebugInfoResp;
	/*spr 22474 end*/

}

/****************************************************************************
 * Function Name  : initPacketRelayGlobVariables
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion initalise all global variables.
 ****************************************************************************/
void initPacketRelayGlobVariables(void)
{
    UInt32 startVal = 0, startIndex = 0;
#ifdef LTE_EMBMS_SUPPORTED
    initPacketRelayEmbmsStructures();
#endif
    for (startVal = 0; startVal < RELAY_GTPU_MAX_UE; startVal++)
    {
        for (startIndex = 0; startIndex < RELAY_GTPU_MAX_LC; startIndex++)
        {
            memSet(&relayMapBasedOnUeIndexLcId_g[startVal][startIndex],0,sizeof(TunnelIdMap));
            relayMapBasedOnUeIndexLcId_g[startVal][startIndex].tunnelId = INVALID_TUNNEL_ID;
            /* Initialize DL FWD tunnel and UL FWD tunnel to default value */
            relayMapBasedOnUeIndexLcId_g[startVal][startIndex].tunnelDlFwdId = INVALID_TUNNEL_ID;
            relayMapBasedOnUeIndexLcId_g[startVal][startIndex].tunnelUlFwdId = INVALID_TUNNEL_ID; 
            /* SPR 6758 changes start */
            relayMapBasedOnUeIndexLcId_g[startVal][startIndex].isLocalEndMarkerIndRecvd = LTE_FALSE;
            /* SPR 6758 changes end */
            /* SPR 7844 changes start */
            relayMapBasedOnUeIndexLcId_g[startVal][startIndex].isTargetENB = LTE_FALSE; 
            /* SPR 7844 changes end */
#ifdef ENDC_ENABLED
            relayMapBasedOnUeIndexLcId_g[startVal][startIndex].dcBearerType = INVALID_DC_BEARER; 
            relayMapBasedOnUeIndexLcId_g[startVal][startIndex].dcTunnelId = INVALID_TUNNEL_ID; 
#endif
        }
        memSet(&s1uUEReconfigCnfInfo_g,0,sizeof(s1uUEReconfigCnfInfo_g[startVal]));
    }
    for (startVal = 0; startVal < RELAY_GTPU_MAX_TRANS_ID; startVal++) 
    {
        memSet(&relayMapBasedOnTransId_g[startVal],0,sizeof(UeIndexLcIdTunnelIdMap));
        relayMapBasedOnTransId_g[startVal].tunnelType = INVALID_TUNNEL_TYPE; 
    }
    memSet(&relayConfigInfo_g,0,sizeof(relayConfigInfo_g));

    memSet(&transIdMappingForLM_g,0,sizeof(transIdMappingForLM));

    /*SPR #2431 fix Start*/ 
    /*IPV6_CHANGE - START*/
    memSet(myAddr4,0,RELAY_ADDR_LEN_IPV4);
    memSet(myAddr6,0,RELAY_ADDR_LEN_IPV6);
    myAddrType = 0;
    /*IPV6_CHANGE - END*/
    /*SPR #2431 fix End*/

    memSet(&s1ReconfigSapCnf_g,0,sizeof(relayS1ReconfigSapCnf));
    s1ReconfigSapCnf_g.ueIndex = INVALID_UE_INDEX;

    /*Initialize the socket fds to -1*/
    for(startVal=0; startVal < MAX_DSCP_VALUES; startVal++)
    {
        relayDscpToSockFdMap_g[startVal].sockfd = RELAY_INVALID_FD; 
	    /*IPV6_CHANGE - START*/
        relayDscpToSockFdMap6_g[startVal].sockfd = RELAY_INVALID_FD; 
        /*IPV6_CHANGE - END*/
    }

}

/****************************************************************************
 * Function Name  : initPacketRelay
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion initalise all function pointers.
 ****************************************************************************/
void initPacketRelay(void)
{
    initPacketRelayOamFunPtr();
    initPacketRelayS1ApFunPtr();
    initPacketRelayAdapterFunPtr();
    initPrepareAndSendRspFunPtr();
    /*+SPR_17855*/    
    initPacketRelayGlobVariables();
    /*-SPR_17855*/
    /*All communication with OAM/RRC/RRM if any have been moved to mailbox
      interface, hence socket interface initialisation not needed*/
    getCfgForSocket(NULL,relayConfigInfo_g.oamSocketInf,relayConfigInfo_g.s1ApSocketInf,relayConfigInfo_g.adapterSocketInf,
            &relayConfigInfo_g.oamRxPort,&relayConfigInfo_g.s1ApRxPort,&relayConfigInfo_g.s1ApTxPort,
            relayConfigInfo_g.peerTgInf,relayConfigInfo_g.peerTgInfV6,&relayConfigInfo_g.peerTgInfAddrType,
            relayConfigInfo_g.secPeerTgInf,relayConfigInfo_g.secPeerTgInfV6,
            /*SPR 21064 Start*/
            &relayConfigInfo_g.secPeerTgInfAddrType,0 /*InstanceId Fix*/
            /*SPR 21064 End*/
            /*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
            ,&relayConfigInfo_g.gtpuRxPort
#ifdef ENDC_ENABLED
            ,&relayConfigInfo_g.rlcRxPort
#endif
#endif
            /*SPR 21366 -*/
            );

    prepareSocketForSndRcv();
    /*SPR #2404 fix Start*/
    initGlueSockets();
    /*SPR #2404 fix End*/
#ifdef ENDC_ENABLED
    initX2uRelayContext();
#endif
    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(PR_INIT_DONE,LOGINFO,
            PR_GTPU_CONTROL,
            relayGtpuCurrentTick_g,__LINE__,
            0,0,0,0, 0,0,__func__,"");
    /* SPR 2030 fix End*/
}
/* handleGtpuRegistration func moved in gtpuMain.c */
