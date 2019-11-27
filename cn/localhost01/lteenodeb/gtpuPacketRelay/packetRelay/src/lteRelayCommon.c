/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteRelayCommon.c,v 1.3.6.2 2010/08/02 08:57:15 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Common Funtions Implementation
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRelayCommon.c,v $
 * Revision 1.3.6.2  2010/08/02 08:57:15  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.3  2010/02/22 11:55:52  gur19479
 * updated to put a #define for throughput display
 *
 * Revision 1.2  2010/02/18 07:47:03  gur19479
 * updated for throughput
 *
 * Revision 1.1.2.2  2010/01/20 22:33:20  gur19479
 * updated for Peer GTP Packet handing
 *
 * Revision 1.1.2.1  2009/11/25 09:57:08  cm_lteenodeb
 * code checkin
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
#include "lteRelayCommon.h"
#include "lteRelay.h"

#include "lteLog.h"

#include "lteRelayS1Sap.h"
#include "lteRelayGtpuSap.h"
#include "lteRelayOamSap.h"
#include "lteTypes.h"
/* + SPR 19022 */
#include "egtpu_msgtypes.h"
/* - SPR 19022 */
#ifdef LAYER2_DLTG_THREAD
#include "lteLayer2DLTG.h"
#endif
#ifdef ENDC_ENABLED
#include "lteX2uRelay.h"
#include "egtpu_cdb.h"
#include "lteRelayAdapter.h"
#endif
/******************************************************************************
 * Project Includes
 *****************************************************************************/

/******************************************************************************
  Private Definitions
 *****************************************************************************/
/*
void appendApiHeader
(
    UInt16 moduleId,
    UInt16 apiId,
    UInt16 transId,
    UInt16 msgLen,
    UInt8 *respBuff_p
);*/
/******************************************************************************
  Private Types
 *****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern relayConfigInfo relayConfigInfo_g;
/*SPR 20554 Fix Start*/
extern SInt8 s1uIpv4SockRx_g;
/*SPR 3587 Fix Start*/
extern SockAddr sockAddrRx_g;
/*SPR 20554 Fix End*/
/*SPR 3587 Fix End*/
/*IPV6_CHANGE - START*/
extern SInt8 ipv6SockRx_g;
extern sockAddrIn6 sockAddrRx6_g;
extern UInt8 sockAddrType;
/*IPV6_CHANGE - END*/
/* warning removed*/
extern UInt64 uplinkRelayEgressDataSize ;
extern UInt64 uplinkRelayEgressDataNum;
extern UInt16 getRrcUeIdxFromUeIdx(UInt16 rrcUeIdx);
extern UInt16 getUeIdxFromRrcUeIdx(UInt16 rrcUeIdx);
#ifdef LAYER2_DLTG_THREAD
void lteMacprocessULPacketInDLTG (UInt8 *data_p,
                UInt32 payLoadLen,
                UInt32 teid);
void addRecordToTable(UInt32 tid_self, UInt32 tunnelId,UInt16 ueId,UInt8 lcId);
#endif

#ifdef LINUX_PC_TEST
#define MAX_PRINT_BUFF_LEN 200
char g_print_buff[MAX_PRINT_BUFF_LEN];
char* change_data_to_str(UInt16 len, UInt8* data)
{
	UInt16 cnt, curlen = 0;
	if (len * 2 + 1 > MAX_PRINT_BUFF_LEN)
	{
		len = 64;
	}

	for (cnt = 0; cnt < len; cnt++)
	{
		sprintf(g_print_buff + curlen, "%02X", data[cnt]);
		curlen += 2;
	}
	g_print_buff[curlen] = 0;
	return g_print_buff;
}
#endif
/****************************************************************************
 * Functions implementation
 *****************************************************************************/

/****************************************************************************
 * Function Name  : udpSendToAdapter
 * Inputs         : buff_p - pointer to the buffer to be sent,
 *                  len - length of buffer
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion sends Information to Adapter.
 ****************************************************************************/
void* udpSendToAdapter(Char8 *buff_p, Int len)
{
    if ( -1 ==
        sendto_wrapper(relayConfigInfo_g.adapterRxFd,&buff_p[0],len,0,&relayConfigInfo_g.adapterToAddr,sizeof(SockAddr)))
    {
        LTE_LOG(LOG_INFO,"Not able to send UDP Packet\n");
    }
    return PNULL;
}


/****************************************************************************
 * Function Name  : udpSendToOam
 * Inputs         : buff_p - pointer to the buffer to be sent,
 *                  len - length of buffer
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion sends Information to OAM.
 ****************************************************************************/
void* udpSendToOam(Char8 *buff_p, Int len)
{
     if ( -1 ==
         sendto_wrapper(relayConfigInfo_g.oamS1ApRxFd,&buff_p[0],len,0,&relayConfigInfo_g.oamToAddr,sizeof(SockAddr)))
     {
         LTE_LOG(LOG_INFO,"Not able to send UDP Packet\n");
     }
     return PNULL;
}

/****************************************************************************
 * Function Name  : udpSendToS1ap
 * Inputs         : buff_p - pointer to the buffer to be sent,
 *                  len - length of buffer
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion sends Information to S1-AP.
 ****************************************************************************/
void* udpSendToS1ap(Char8 *buff_p, Int len)
{
     if ( -1 ==
         sendto_wrapper(relayConfigInfo_g.oamS1ApRxFd,&buff_p[0],len,0,&relayConfigInfo_g.s1ApToAddr,sizeof(SockAddr)))
     {
         LTE_LOG(LOG_INFO,"Not able to send UDP Packet\n");
     }
     return PNULL;
}
/*SPR 21188 +*/
#if defined PDCP_GTPU_INTF
/****************************************************************************
 * Function Name  : udpSendToPdcp
 * Inputs         : buff_p - pointer to the buffer to be sent,
 *                  len - length of buffer
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion sends Information to S1-AP.
 ****************************************************************************/
void* udpSendToPdcp(char *buff_p, int len, UInt8 instanceId)
{
     if ( -1 ==
         sendto_wrapper(relayConfigInfo_g.gtpu_Pdcp_Ip_port[instanceId].pdcpRxFd,&buff_p[0],len,0,\
             &relayConfigInfo_g.gtpu_Pdcp_Ip_port[instanceId].pdcpRxAddr,sizeof(SockAddr)))
     {
         LOG_GTPUPR_ERROR( PR_GTPU_COMMON, "Not able to send UDP Packet" );
     }
     return PNULL;
}
#endif
/*SPR 21188 +*/
#ifndef EPC_SETUP
/****************************************************************************
 * Function Name  : udpSendToTG
 * Inputs         : respMsg - pointer to the response message buffer,
 *                  current,
 *                  interfaceType
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS
 * Description    : This funtion sends Information to DL-TG.
 ****************************************************************************/
UInt8 udpSendToTG(UInt8 *respMsg, UInt16 current, Char8 interfaceType,
                  /*SPR 20646 fix start*/
                  UInt16 Addr_len
                  /*SPR 20646 fix end*/)
{

    if ((IPV4_ADDR_PRESENT & interfaceType)
        /*SPR 20646 fix start*/
        && (Addr_len==4)
        /*SPR 20646 fix end*/)
    {
        if( -1 == sendto_wrapper(glueTx_g, respMsg, current, 0, &tgGlueAddress_g, sizeof(SockAddr)) )
        {
            LOG_GTPU_MSG(PR_SENT_TEID_UEID_LCID_TO_DLTG,LOGINFO,
                    PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0,0,0, 0,0,__func__,"");
        }
    }
    if ((IPV6_ADDR_PRESENT & interfaceType)
        /*SPR 20646 fix start*/
        && (Addr_len==16)
        /*SPR 20646 fix end*/)
    {
        if( -1 == sendto_wrapper(glueTx6_g, respMsg, current, 0, (SockAddr *)&tgGlueAddress6_g, sizeof(sockAddrIn6)) )
        {
            LOG_GTPU_MSG(PR_SENT_TEID_UEID_LCID_TO_DLTG,LOGINFO,
                    PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0,0,0, 0,0,__func__,"");
        }
    }
    return RELAY_GTPU_SUCCESS;
}

/* LIPA changes start */
/****************************************************************************
 * Function Name  : udpSendToSecondaryTG
 * Inputs         : respMsg - pointer to the response message buffer,
 *                  current,
 *                  interfaceType
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion sends Secondary Tunnel Information to DL-TG.
 ****************************************************************************/
UInt8 udpSendToSecondaryTG(UInt8 *respMsg, UInt16 current, Char8 interfaceType)
{
    if (IPV4_ADDR_PRESENT & interfaceType)
    {
        if( -1 == sendto_wrapper(secondaryTG_g, respMsg, current, 0, &secondaryTGAddress_g, sizeof(SockAddr)))
        {
            LOG_GTPU_MSG(PR_SENT_TEID_UEID_LCID_TO_SECONDARY_DLTG, LOGINFO,
                    PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0,0,0, 0,0,__func__,"");
            return RELAY_GTPU_FAILURE;
        }
    }
    else if (IPV6_ADDR_PRESENT & interfaceType)
    {
        /* SPR 8745 Fix Start */
        if( -1 == sendto_wrapper(secondaryTG6_g, respMsg, current, 0, (SockAddr *)&secondaryTGAddress6_g, 
                                                                                  sizeof(sockAddrIn6)))
        /* SPR 8745 Fix End */
        {
            LOG_GTPU_MSG(PR_SENT_TEID_UEID_LCID_TO_SECONDARY_DLTG, LOGINFO,
                    PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0,0,0, 0,0,__func__,"");
            return RELAY_GTPU_FAILURE;
        }
    }
    return RELAY_GTPU_SUCCESS;
}
/* LIPA changes end */
#endif

/****************************************************************************
 * Function Name  : processRelayInitDbCnf
 * Inputs         : respMsg_p - pointer to the response message buffer,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelayInitDbCnf
(
    UInt8 *respMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
   /* Check for incoming message and length of that message should be greater 
      than 10 */
   if (respMsg_p && (API_HDR <= totalLen))
   {
       /* Append header in front of message, TransId will be same as was sent
          in request message */ 
       appendApiHeader(RELAY_OAM_MODULE_ID,LM_INITDB_CNF,\
           transId,totalLen,respMsg_p, transIdMappingForLM_g[LM_INITDB_CNF].cellId);
       /* send to OAM */
       LTE_LOG(LOG_INFO,"[SENDING] INIT_DB_CNF to OAM\n");
  
  udpSendToOam((Char8*)&respMsg_p[0],totalLen);
       return RELAY_GTPU_SUCCESS;
   }
   return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : sendPacketToPeerGtpu
 * Inputs         : data_p
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion send packets to Peer GTPU.
 ****************************************************************************/
void sendPacketToPeerGtpu(UInt8 *data_p)
{
    UInt32 startPayLoadLen = 0;
    UInt32 endPayLoadLen = 0;
    UInt32 payLoadLen = 0;
    UInt32 peerIpAddr = 0;
    UInt8  dscp;
    SInt8 sockfd;
    UInt8  addr_len;
    sockAddrIn6 sockaddr6;
#ifndef LAYER2_DLTG_THREAD    
    void *pSockAddr = NULL;
    UInt16 sockAddrLen = 0;
#endif 
    startPayLoadLen = RELAY_GTPU_GET_U16BIT(&data_p[CSPL_HDR - 4]);   
    endPayLoadLen = RELAY_GTPU_GET_U16BIT(&data_p[CSPL_HDR - 6]);  
    payLoadLen = endPayLoadLen - startPayLoadLen +1;  
    
    /* + SPR 19022 */
    UInt16 peerPort = RELAY_GTPU_GET_U16BIT(&data_p[CSPL_HDR + 1]);
    /* - SPR 19022 */

    /* Extract the IP address of SGW*/
    addr_len = RELAY_GTPU_GET_U16BIT(&data_p[CSPL_HDR + 4]);
    if (RELAY_ADDR_LEN_IPV4 == addr_len)
    {
        peerIpAddr = RELAY_GTPU_GET_U32BIT(&data_p[CSPL_HDR + 6]);
        relayConfigInfo_g.gtpuToAddr.sin_addr.s_addr = htonl_wrapper(peerIpAddr);
        relayConfigInfo_g.gtpuToAddr.sin_family = AF_INET;
        relayConfigInfo_g.gtpuToAddr.sin_port = htons_wrapper(peerPort);
        /*Qos Id sent from GTPu Stack is the DSCP value*/
        dscp = (UInt8)RELAY_GTPU_GET_U32BIT(&data_p[CSPL_HDR + 10]);
        /* SPR 4047 Fix Start */
        if(dscp >= MAX_DSCP_VALUES)
        {
            /* SPR 4047 Fix End */
            /*SPR 20554 Fix Start*/
            sockfd = s1uIpv4SockRx_g;
            /*SPR 20554 Fix End*/
        }
        else
        {
            /* Coverity Fix 111604 +*/
            sockfd = relayDscpToSockFdMap_g[dscp].sockfd;

            /*If DSCP unrecognized, use Best Effort Class to Deliver
             * RFC - 2474*/
            if(RELAY_INVALID_FD == sockfd) 
                sockfd = s1uIpv4SockRx_g;
    }

#ifndef LAYER2_DLTG_THREAD    
        pSockAddr = &relayConfigInfo_g.gtpuToAddr;
        sockAddrLen = sizeof(relayConfigInfo_g.gtpuToAddr);
#endif
    }
    else if (RELAY_ADDR_LEN_IPV6 == addr_len)
    {
        memSet(&sockaddr6,0,sizeof(sockaddr6));
        memCpy(sockaddr6.sin6_addr.s6_addr, &data_p[CSPL_HDR + 6], addr_len);
        sockaddr6.sin6_family = AF_INET6;
        sockaddr6.sin6_port = htons_wrapper(peerPort);
	/*Qos Id sent from GTPu Stack is the DSCP value*/
	dscp = (UInt8)RELAY_GTPU_GET_U32BIT(&data_p[CSPL_HDR + 22]);
	/* SPR 4047 Fix Start */
	if(dscp >= MAX_DSCP_VALUES)
	{
        /*SPR 20554 Fix Start*/
        sockfd = ipv6SockRx_g;
    }
    else
    {
        sockfd = relayDscpToSockFdMap6_g[dscp].sockfd;

        /*If IPv6 traffic class unrecognized, use Best Effort Class to Deliver
         * RFC - 2474*/
        if(RELAY_INVALID_FD == sockfd) 
            sockfd = ipv6SockRx_g;
        /*SPR 20554 Fix End*/
	}
#ifndef LAYER2_DLTG_THREAD    
        pSockAddr = &sockaddr6;
        sockAddrLen = sizeof(sockaddr6);
#endif
    }
    else
    {
        LTE_LOG(LOG_INFO,"\n[ERROR] Incorrect length for ipaddress received - %d.\n",addr_len);
        return;
    }

#ifdef LINUX_PC_TEST
	SEND_TO_TEST(2152,&data_p[startPayLoadLen],payLoadLen);	
	printf("SEND_TO_TEST %uBytes:%s\n",
			payLoadLen,change_data_to_str(payLoadLen, (UInt8*)&data_p[startPayLoadLen]));
	return;	
#endif

#ifdef LAYER2_DLTG_THREAD
    UInt32 teid = 0;
    teid = RELAY_GTPU_GET_U32BIT(&data_p[startPayLoadLen+4]);
    lteMacprocessULPacketInDLTG (&data_p[startPayLoadLen],
                                 payLoadLen,
				 teid);
#else
	uplinkRelayEgressDataSize += payLoadLen;
	uplinkRelayEgressDataNum ++;
    if ( -1 ==
        sendto_wrapper(sockfd,&data_p[startPayLoadLen],payLoadLen,
                0, pSockAddr,sockAddrLen ))
    {
	    LTE_LOG(LOG_INFO,"\n[ERROR] Not able to send data to DL-TG\n");
    }
#endif
}

/****************************************************************************
 * Function Name  : processRelaySetTimerCnf
 * Inputs         : respMsg_p - pointer to the response message buffer,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send set timer confirmation to OAM.
 ****************************************************************************/

UInt32 processRelaySetTimerCnf
(
    UInt8 *respMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    /* Check for incoming message and length of that message should be greater
      than 10 for safety sake*/
    if (respMsg_p && (API_HDR <= totalLen))
    {
      /* Append header in front of message, TransId will be same as was sent
          in request message */
    /*  + Layer2 NON CA Changes */  
       appendApiHeader(RELAY_OAM_MODULE_ID,LM_SET_TIMER_CNF,\
           transId,totalLen,respMsg_p,transIdMappingForLM_g[LM_SET_TIMER_CNF].cellId);
    /*  - Layer2 NON CA Changes */  

       /* send to OAM */ 
        udpSendToOam((Char8*)&respMsg_p[0],totalLen);
       return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processRelayGetTimerCnf
 * Inputs         : respMsg_p - pointer to the response message buffer,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send get timer confirmation to OAM.
 ****************************************************************************/

UInt32 processRelayGetTimerCnf
(
    UInt8 *respMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    if (respMsg_p && (API_HDR <= totalLen))
    {
      /* Append header in front of message, TransId will be same as was sent
          in request message */ 
    /*  + Layer2 NON CA Changes */  
       appendApiHeader(RELAY_OAM_MODULE_ID,LM_GET_TIMER_CNF,\
           transId,totalLen,respMsg_p,transIdMappingForLM_g[LM_GET_TIMER_CNF].cellId);
    /*  - Layer2 NON CA Changes */  
       /* send to OAM */ 
        udpSendToOam((Char8*)&respMsg_p[0],totalLen);
       return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processRelayGetMaxRetriesCnf
 * Inputs         : respMsg_p - pointer to the response message buffer,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelayGetMaxRetriesCnf
(
    UInt8 *respMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    /* Check for incoming message and length of that message should be greater
      than 10 for safety sake*/
    if (respMsg_p && (API_HDR <= totalLen))
    {
        /* Append header in front of message, TransId will be same as was sent
          in request message */
    /*  + Layer2 NON CA Changes */  
        appendApiHeader(RELAY_OAM_MODULE_ID,LM_GET_MAX_RETIRES_CNF,\
            transId,totalLen,respMsg_p,transIdMappingForLM_g[LM_GET_MAX_RETIRES_CNF].cellId);
    /*  - Layer2 NON CA Changes */  
        /* send to OAM */
        udpSendToOam((Char8*)&respMsg_p[0],totalLen);
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processRelaySetMaxRetriesCnf
 * Inputs         : respMsg_p - pointer to the response message buffer,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelaySetMaxRetriesCnf
(
    UInt8 *respMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    /* Check for incoming message and length of that message should be greater
      than 10 for safety sake*/
    if (respMsg_p && (API_HDR <= totalLen))
    {
        /* Append header in front of message, TransId will be same as was sent
          in request message */
    /*  + Layer2 NON CA Changes */  
        appendApiHeader(RELAY_OAM_MODULE_ID,LM_SET_MAX_RETIRES_CNF,\
            transId,totalLen,respMsg_p,transIdMappingForLM_g[LM_SET_MAX_RETIRES_CNF].cellId);
    /*  - Layer2 NON CA Changes */  
        /* send to OAM */
        udpSendToOam((Char8*)&respMsg_p[0],totalLen);
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processRelayInitTraceLvlCnf
 * Inputs         : respMsg_p - pointer to the response message buffer,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelayInitTraceLvlCnf
(
    UInt8 *respMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    /* Check for incoming message and length of that message should be greater
      than 10 for safety sake*/
    if (respMsg_p && (API_HDR <= totalLen))
    {
        /* Append header in front of message, TransId will be same as was sent
          in request message */
    /*  + Layer2 NON CA Changes */  
        appendApiHeader(RELAY_OAM_MODULE_ID,LM_INIT_TRACE_LEVEL_CNF,\
            transId,totalLen,respMsg_p,transIdMappingForLM_g[LM_INIT_TRACE_LEVEL_CNF].cellId);
    /*  - Layer2 NON CA Changes */  
        /* send to OAM */
        udpSendToOam((Char8*)&respMsg_p[0],totalLen);
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processRelaySetTraceLvlCnf
 * Inputs         : respMsg_p - pointer to the response message buffer,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelaySetTraceLvlCnf
(
    UInt8 *respMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    /* Check for incoming message and length of that message should be greater
      than 10 for safety sake*/
    if (respMsg_p && (API_HDR <= totalLen))
    {
        /* Append header in front of message, TransId will be same as was sent
          in request message */
    /*  + Layer2 NON CA Changes */  
        appendApiHeader(RELAY_OAM_MODULE_ID,LM_SET_TRACE_LEVEL_CNF,\
            transId, totalLen,respMsg_p,transIdMappingForLM_g[LM_SET_TRACE_LEVEL_CNF].cellId);
    /*  - Layer2 NON CA Changes */  
        /* send to OAM */
        udpSendToOam((Char8*)&respMsg_p[0],totalLen);
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processRelayGetTraceLvlCnf
 * Inputs         : respMsg_p - pointer to the response message buffer,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/

UInt32 processRelayGetTraceLvlCnf
(
    UInt8 *respMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    /* Check for incoming message and length of that message should be greater
      than 10 for safety sake*/
    if (respMsg_p && (API_HDR <= totalLen))
    {
       /* Append header in front of message, TransId will be same as was sent
          in request message */
    /*  + Layer2 NON CA Changes */  
       appendApiHeader(RELAY_OAM_MODULE_ID,LM_GET_TRACE_LEVEL_CNF,\
        transId,totalLen,respMsg_p,
        transIdMappingForLM_g[LM_GET_TRACE_LEVEL_CNF].cellId);
    /*  - Layer2 NON CA Changes */  

       /* send to OAM */
       udpSendToOam((Char8*)&respMsg_p[0],totalLen);
       return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;

}

/*SPR 22474 start*/
/****************************************************************************
 * Function Name  : processRelayGetDebugInfoResp
 * Inputs         : respMsg_p - pointer to the response message buffer,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send confirmation to OAM.
 ****************************************************************************/
UInt32 processRelayGetDebugInfoResp
(
    UInt8 *respMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    /* Check for incoming message and length of that message should be greater
         * than header length for safety sake*/
    if (respMsg_p && (API_HDR <= totalLen))
    {
       /* Append header in front of message, TransId will be same as was sent
                in request message */
       appendApiHeader(RELAY_OAM_MODULE_ID,GTPU_OAM_GET_DEBUG_INFO_RESP,
        transId,totalLen,respMsg_p,
        transIdMappingForLM_g[GTPU_OAM_GET_DEBUG_INFO_RESP].cellId);

       /* send to OAM */
       udpSendToOam((Char8*)&respMsg_p[0],totalLen);
       return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;

}
/*SPR 22474 end*/

#ifndef EPC_SETUP
/****************************************************************************
 * Function Name  : sendMessageToDlTg
 * Inputs         : ueIndex - UE ID,
 *                  lc_id - Logical channel ID,
 *                  teid_self - Self TEID,
 *                  teid_peer - Peer's TEID,
 *                  outMsgTG_p - Pointer to Out message,
 *                  currentTG - Current TG,
 *                  tunnelType - Type of tunnel,
 *                  enbType - eNodeB type
 * Outputs        : None
 * Returns        : resp
 * Description    : This funtion sends message to DL-TG.
 ****************************************************************************/
UInt8 sendMessageToDlTg (UInt16 ueIndex,UInt8 lc_id,UInt32 teid_self, 
        UInt32 teid_peer,UInt8 * outMsgTG_p,UInt16 currentTG, UInt8 tunnelType, UInt8 enbType, 
        /*SPR 20646 fix start*/
        UInt16 Addr_len
        /*SPR 20646 fix end*/
        )
{
#ifdef LINUX_PC_TEST
	return 0;
#endif

    /* SPR 4047 Fix Start */
    SInt16 resp = 0;
    /* SPR 4047 Fix End */
    RELAY_GTPU_SET_U32BIT((outMsgTG_p+currentTG), teid_self);
    currentTG +=4;
    RELAY_GTPU_SET_U32BIT((outMsgTG_p+currentTG), teid_peer);
    currentTG +=4;
    RELAY_GTPU_SET_U16BIT((outMsgTG_p+currentTG), ueIndex);
    currentTG +=2;
    *(outMsgTG_p+currentTG) = lc_id;
    currentTG ++;
    *(outMsgTG_p+currentTG) = tunnelType;
    currentTG ++;
    *(outMsgTG_p+currentTG) = enbType;
    currentTG ++;


    /*  + Layer2 NON CA Changes */  
    appendApiHeader(RELAY_S1AP_MODULE_ID, S1U_RECONFIGURE_UE_ENTITY_CNF, 0, 
    currentTG, outMsgTG_p,s1uUEReconfigCnfInfo_g[ueIndex].cellIndex);
    /*  - Layer2 NON CA Changes */  

    /* SPR 4047 Fix Start */
	/*IPV6_CHANGE - START*/
    /* SPR 5599_128826 Fix Start */
    /* +GCC 4.7 FIX */
    resp = udpSendToTG(outMsgTG_p , currentTG , relayConfigInfo_g.peerTgInfAddrType,
                       /*SPR 20646 fix start*/
                       Addr_len
                       /*SPR 20646 fix end*/
                      );
    /* -GCC 4.7 FIX */
	/* SPR 5599_128826 Fix End */
    /*IPV6_CHANGE - End*/
    return resp;
}    
/*SPR #2404 fix End*/
#ifdef ENDC_ENABLED
/****************************************************************************
 * Function Name  : sendTunnelInfoToDlTg
 * Inputs         : ueIndex - UE ID,
 *                  lc_id - Logical channel ID,
 *                  teid_self - Self TEID,
 *                  teid_peer - Peer's TEID,
 *                  tunnelType - Type of tunnel,
 *                  enbType - eNodeB type
                    transportAddr - peer trasport addr
 * Outputs        : None
 * Returns        : resp
 * Description    : This funtion sends message to DL-TG.
 ****************************************************************************/
UInt8 sendTunnelInfoToDlTg (UInt16 ueIndex,UInt8 lc_id,UInt32 teid_self, 
        UInt32 teid_peer,UInt8 tunnelType, UInt8 snSize, 
        UInt16 Addr_len,
        UInt8 *transportAddr
        )
{
	UInt16 currentTG  = API_HDR;
	UInt8  outMsgTG_p [MAX_API_LEN]; 

    SInt16 resp = 0;
    RELAY_GTPU_SET_U32BIT((outMsgTG_p+currentTG), teid_self);
    currentTG +=4;
    RELAY_GTPU_SET_U32BIT((outMsgTG_p+currentTG), teid_peer);
    currentTG +=4;
    RELAY_GTPU_SET_U16BIT((outMsgTG_p+currentTG), ueIndex);
    currentTG +=2;
    *(outMsgTG_p+currentTG) = lc_id;
    currentTG ++;
    *(outMsgTG_p+currentTG) = tunnelType;
    currentTG ++;
    *(outMsgTG_p+currentTG) = snSize;
    currentTG ++;
    
    if(PNULL != transportAddr)
    {
    	outMsgTG_p[currentTG++] = *(transportAddr +3);
    	outMsgTG_p[currentTG++] = *(transportAddr +2);
    	outMsgTG_p[currentTG++] = *(transportAddr +1);
    	outMsgTG_p[currentTG++] = *(transportAddr +0);
    }

    appendApiHeader(RELAY_S1AP_MODULE_ID, S1U_RECONFIGURE_UE_ENTITY_CNF, 0, 
    currentTG, (UInt8*)&outMsgTG_p,s1uUEReconfigCnfInfo_g[ueIndex].cellIndex);

    resp = udpSendToTG((UInt8*)&outMsgTG_p , currentTG , relayConfigInfo_g.peerTgInfAddrType,
                       Addr_len
                      );
    return resp;
}
#endif
#endif

/****************************************************************************
 * Function Name  : getTlvLenAndComposeTunnelInfoCnf
 * Inputs         : outMsg - Pointer to out message,
 *                  current,
 *                  TunnelInfo - Tunnel Info,
 *                  lcIndex - Logical channel index,
 *                  enbType - eNodeB type
 * Outputs        : Tlv Length for Tunnel Info in Setup Sap Cnf
 * Returns        : length
 * Description    : This funtion calculates the TLV Length and composes 
 *                  Tunnel Info IE.
 ****************************************************************************/
UInt16 getTlvLenAndComposeTunnelInfoCnf(UInt8 *outMsg, UInt16 current,
        relayTunnelInfo *tunnelInfo, UInt8 lcIndex, UInt8 enbType)
{
    UInt16 length = 0, ueIndex =0;
    UInt16 tlvLenPos = 0;
    UInt8 i;
#ifndef EPC_SETUP
#ifndef LAYER2_DLTG_THREAD
    UInt32 transId = 0;
#endif
    /*Coverity 55830 Start*/
#ifndef EPC_SETUP
    UInt8 *outMsgTG = PNULL;
    outMsgTG = (UInt8 *)getMemFromPool((MAX_API_LEN + API_HDR), PNULL);
#endif
    /*Coverity 55830 End*/
    /*  + Layer2 NON CA Changes */  
    UInt16 currentTG = API_HDR;
    /*  - Layer2 NON CA Changes */  
#endif 
    RELAY_GTPU_SET_U16BIT(&outMsg[current],S1U_IE_TUNNEL_INFO_CNF_TAG);
    length += RELAY_SIZE_OF_TAG;
    current += RELAY_SIZE_OF_TAG;
 
    tlvLenPos = current;
    current += RELAY_SIZE_OF_LEN;
    length += RELAY_SIZE_OF_LEN;
   
    /*Fill Cause & increment length*/
    outMsg[current++] = tunnelInfo->cause;
    length ++;

    /*Fill Rb direction & increment length*/
    RELAY_GTPU_SET_U16BIT(&outMsg[current],tunnelInfo->rbDirection);
    current += 2;
    length += 2;

    if(tunnelInfo->cause == GTPU_CAUSE_SUCCESS)
    {
            RELAY_GTPU_SET_U16BIT(&outMsg[current],S1U_IE_TEID_SELF_TAG);
            current += RELAY_SIZE_OF_TAG;
            RELAY_GTPU_SET_U16BIT(&outMsg[current],RELAY_SIZE_OF_TAG + 
                    RELAY_SIZE_OF_LEN+ RELAY_SIZE_OF_TUNNELID );
            current += RELAY_SIZE_OF_LEN;
            RELAY_GTPU_SET_U32BIT(&outMsg[current],tunnelInfo->teidSelf);

            current += RELAY_SIZE_OF_TUNNELID; 
            length += RELAY_SIZE_OF_TAG + RELAY_SIZE_OF_LEN + RELAY_SIZE_OF_TUNNELID;
             
            /*Source Trasnport Address present only when cause = SUCCESS*/
            RELAY_GTPU_SET_U16BIT(&outMsg[current],S1U_IE_GSN_ADDR_TAG);
            current += RELAY_SIZE_OF_TAG;
            RELAY_GTPU_SET_U16BIT(&outMsg[current],RELAY_SIZE_OF_TAG + 
                    RELAY_SIZE_OF_LEN + tunnelInfo->addrLen);
            current += RELAY_SIZE_OF_LEN;
            for(i=0;i<tunnelInfo->addrLen;i++)
            {
                outMsg[current++] = tunnelInfo->transportAddr[i];
            }
            length += RELAY_SIZE_OF_TAG + RELAY_SIZE_OF_LEN + tunnelInfo->addrLen;
            /*Send Info to DL TG*/

            /* SPR 4558 Fix Start */
            ueIndex = getUeIdxFromRrcUeIdx(s1ReconfigSapCnf_g.ueIndex);
#ifdef ENDC_ENABLED
             relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelAddLen = tunnelInfo->addrLen;
#endif
            /* SPR 4558 Fix End */
            if((s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].isNormalTunnelPresentFlag)
#ifdef ENDC_ENABLED
            ||(s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].isScgTunnelPresentFlag)
#endif
            	)
            {
                /*teIdSelf received from GTP stack is peer for DL-Tg/SGW*/
                s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_peer =
                tunnelInfo->teidSelf;
                s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].isNormalTunnelPresentFlag = 0;
#ifdef ENDC_ENABLED
                s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].isScgTunnelPresentFlag = 0;
#endif
                tunnelInfo->bitMask = 0;

#ifdef LAYER2_DLTG_THREAD
    addRecordToTable(
                    s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_self,
                    s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_peer,
                    ueIndex,
                    s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].lc_id);

#else
/*SPR #2404 fix Start*/
            /* mask 4MSB bits from ueIndex in case of MULTI_SECTOR*/
            /* SPR 4558 Fix Start */
#ifndef EPC_SETUP
            UInt16 rrcUeIndex = getUeIdxFromRrcUeIdx(s1ReconfigSapCnf_g.ueIndex);
            /* SPR 4558 Fix End */

            /* Here it requires to decide whether the Tunnel is created on SGW or LGW 
             * In case the secondary tunnel exists, it means this request is for LGW
             * else the tunnel will be created with SGW 
             */
            transId = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].transId;
            if ((relayMapBasedOnTransIdForSecTunnel_g[transId].inUseFlag == RELAY_GTPU_IN_USE) &&
                (relayMapBasedOnTransIdForSecTunnel_g[transId].sapFlags == S1U_SECONDARY_TUNNEL_SAP_FLAG))
	    {
		    /* Sending Secondary Tunnel Information to DL-TG*/
		    sendMessageToDlTgForSecondaryTunnel (
				    rrcUeIndex,
			     	    s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].lc_id,
				    s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_self,
				    s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_peer,
				    NORMAL_BEARER);
	    }
            else
            {
    /*Coverity 55830 Start*/
                if(outMsgTG)
                {
#ifdef ENDC_ENABLED
            if(s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_self != 0)
#endif
                    sendMessageToDlTg(
                            rrcUeIndex,/*s1ReconfigSapCnf_g.ueIndex & RRC_MK_VELID_UE_ID),*/
                        s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].lc_id,
                        s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_self,
                        s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_peer,
                        outMsgTG,currentTG,NORMAL_BEARER, (enbType<<4)|NORMAL_BEARER,
                        /*SPR 20646 fix start*/
                        tunnelInfo->addrLen
                        /*SPR 20646 fix end*/
                        );
                }
    /*Coverity 55830 End*/
           }
/*SPR #2404 fix End*/
#endif
#endif
            }
            else if(s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].isDlTunnelPresentFlag)
            {
                /*teIdSelf received from GTP stack is peer for DL-Tg/SGW*/
                s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_peerDlFwd =
                tunnelInfo->teidSelf;
                s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].isDlTunnelPresentFlag = 0;
                tunnelInfo->bitMask = 0;
/*SPR #2404 fix Start*/
            /* mask 4MSB bits from ueIndex in case of MULTI_SECTOR*/
            /* SPR 4558 Fix Start */
#ifndef EPC_SETUP
            UInt16 rrcUeIndex = getUeIdxFromRrcUeIdx(s1ReconfigSapCnf_g.ueIndex);
            /* SPR 4558 Fix End */
    /*Coverity 55830 Start*/
                if(outMsgTG)
                {
                    sendMessageToDlTg(
                            rrcUeIndex,
                            s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].lc_id,
                            s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_selfDlFwd,
                            s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_peerDlFwd,
                            outMsgTG,currentTG,DL_FORWARDING, (enbType<<4)|DL_FORWARDING,
                            /*SPR 20646 fix start*/
                            tunnelInfo->addrLen
                            /*SPR 20646 fix end*/
                            );
                }
    /*Coverity 55830 End*/
/*SPR #2404 fix End*/
#endif
            }
            else if(s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].isUlTunnelPresentFlag)
            {
                /*teIdSelf received from GTP stack is peer for DL-Tg/SGW*/
                s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_peerUlFwd =
                tunnelInfo->teidSelf;
                s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].isUlTunnelPresentFlag = 0;
                tunnelInfo->bitMask = 0;
/*SPR #2404 fix Start*/
            /* mask 4MSB bits from ueIndex in case of MULTI_SECTOR*/
            /* SPR 4558 Fix Start */
#ifndef EPC_SETUP
            UInt16 rrcUeIndex = getUeIdxFromRrcUeIdx(s1ReconfigSapCnf_g.ueIndex);
            /* SPR 4558 Fix End */
    /*Coverity 55830 Start*/
                if(outMsgTG)
                {
                    sendMessageToDlTg(
                            rrcUeIndex,
                            s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].lc_id,
                            s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_selfUlFwd,
                            s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_peerUlFwd,
                            outMsgTG,currentTG,UL_FORWARDING, (enbType<<4)|UL_FORWARDING,
                            /*SPR 20646 fix start*/
                            tunnelInfo->addrLen
                            /*SPR 20646 fix end*/
                            );
                }
    /*Coverity 55830 End*/
/*SPR #2404 fix End*/
#endif
            }
            /* SPR 7844 changes start */
            if ( TARGET_ENB_VALUE == enbType )
            {
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].isTargetENB = LTE_TRUE;
            }
            /* SPR 7844 changes end */
    }

    /*Populate correct length value of the tag S1U_IE_TUNNEL_INFO_CNF_TAG*/
    RELAY_GTPU_SET_U16BIT(&outMsg[tlvLenPos],length);
    /*Coverity 55830 Start*/
#ifndef EPC_SETUP
    /* SPR 21804 Fix Start */
    if(outMsgTG)
    {
        freeMemPool(outMsgTG);
    }
    /* SPR 21804 Fix End */
#endif
    /*Coverity 55830 End*/

    return length;
}

/****************************************************************************
 * Function Name  : getTlvLenAndComposeSetupSapCnf
 * Inputs         : outMsg - Pointer to Out message,
 *                  current,
 *                  setupSapCnf - Pointer to relaySetupCnf,
 *                  enbType - eNodeB type,
 *                  recvCause
 * Outputs        : Tlv Length
 * Returns        : length
 * Description    : This funtion calculates the TLV Length and composes 
 *                  Setup Sap Cnf IE.
 ****************************************************************************/

UInt16 getTlvLenAndComposeSetupSapCnf(UInt8 *outMsg, UInt16 current, 
                                      relaySetupCnf *setupSapCnf,UInt8 enbType,UInt8 *recvCause)
{
    UInt16 length = 0, loop2;
    UInt16 tlvLenPos = 0, tunnelInfoCnfLen = 0, lcId = 0;
    UInt8 cause=0, lcIndex = 0;
    
        /*Get the correct index of lcId to populate IEs*/
        if(setupSapCnf->bitMask)
        {
           /*Fill the IEs for S1U_IE_RELAY_SETUP_SAP_CNF_TAG*/    
          RELAY_GTPU_SET_U16BIT(&outMsg[current],S1U_IE_RELAY_SETUP_SAP_CNF_TAG);
          current += RELAY_SIZE_OF_TAG;
          tlvLenPos = current;
          current += RELAY_SIZE_OF_LEN;
          length += RELAY_SIZE_OF_TAG + RELAY_SIZE_OF_LEN;
            /*Populate lcId & increment length*/
            outMsg[current++] = 
                setupSapCnf->lcId;
            length ++;
            /*Populate DB to be sent to TG*/
            lcId =
               setupSapCnf->lcId;

            /*Get the correct lcIndex*/
            lcIndex = lcId - 3;

            /* SPR 4558 Fix Start */
            s1uUEReconfigCnfInfo_g[(getUeIdxFromRrcUeIdx(s1ReconfigSapCnf_g.ueIndex))].rbInfo[lcIndex].lc_id = lcId;
            /* SPR 4558 Fix End */

            cause = setupSapCnf->cause;
            *recvCause |= cause;
            /*Populate cause & increment length*/
            outMsg[current++] = cause;
            length ++;

            for(loop2 = 0; loop2 < MAX_NUM_TUNNEL_PER_LC; 
                    loop2++)
            {
                /*Get the index where valid tunnel information is stored*/
                if(setupSapCnf->tunnelInfo[loop2].bitMask)
                {
                    tunnelInfoCnfLen = getTlvLenAndComposeTunnelInfoCnf
                          (outMsg, current,
                           &setupSapCnf->tunnelInfo[loop2],
                           lcIndex,enbType);
                    length+= tunnelInfoCnfLen;
                    current+= tunnelInfoCnfLen;
                }
            }/*loop2*/

            if(GTPU_CAUSE_SUCCESS == cause)
            {
                RELAY_GTPU_SET_U16BIT(&outMsg[current],S1U_IE_SEQ_NUM_TAG);
                current += RELAY_SIZE_OF_TAG;
                /*Uplink and Downlink Sequence Number*/
                RELAY_GTPU_SET_U16BIT(&outMsg[current],2*RELAY_SIZE_OF_SEQ_NUM + 
                        RELAY_SIZE_OF_TAG + RELAY_SIZE_OF_LEN);
                current += RELAY_SIZE_OF_LEN;
                RELAY_GTPU_SET_U16BIT(&outMsg[current],
                        setupSapCnf->seqNumUplink);
                current += RELAY_SIZE_OF_SEQ_NUM;

                RELAY_GTPU_SET_U16BIT(&outMsg[current],
                        setupSapCnf->seqNumDnlink);
                current += RELAY_SIZE_OF_SEQ_NUM;

                length += RELAY_SIZE_OF_TAG + RELAY_SIZE_OF_LEN + 2*RELAY_SIZE_OF_SEQ_NUM;

            }
        }


    RELAY_GTPU_SET_U16BIT(&outMsg[tlvLenPos],length);


    return length ;
   
}

/****************************************************************************
 * Function Name  : processRelaySetupSapCnf
 * Inputs         : inMsg_p,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send confirmation to S1-AP.
 ****************************************************************************/

UInt32 processRelaySetupSapCnf
(
    UInt8 *inMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    UInt8  cause = 0; 
    UInt16 ueIndex = 0;
    /*  + Layer2 NON CA Changes */  
    UInt16 current = API_HDR;
    /*  - Layer2 NON CA Changes */  
    UInt8 enbType = SOURCE_ENB_VALUE;
    UInt16 loop1;

    /* + coverity 52877 */
    UInt8* outMsg_p = PNULL;
    outMsg_p = (UInt8 *)getMemFromPool(MAX_API_LEN + API_HDR,NULL);
    if(PNULL == outMsg_p)
    {
       lteWarning("In %s, MEMORY ALLOC FAILURE\n",__func__);
       return RELAY_GTPU_FAILURE;
    }
    /* SPR 11109 Fix start */ 
    memSet(outMsg_p, 0, MAX_API_LEN + API_HDR);
    /* SPR 11109 Fix end */ 

    /* - coverity 52877 */
    
    /* Filling Reqd Info to out Buffer */
    RELAY_GTPU_SET_U16BIT((outMsg_p + current),s1ReconfigSapCnf_g.ueIndex);
            /* SPR 4558 Fix Start */
    ueIndex = getUeIdxFromRrcUeIdx(s1ReconfigSapCnf_g.ueIndex);
            /* SPR 4558 Fix End */
    current += 2;

    /*If ApiIdForRsp is S1U_CREATE_UE_ENTITY_CNF, then 
     * there is a responseCode to be filled at the top level*/
     if (S1U_CREATE_UE_ENTITY_CNF == 
             s1ReconfigSapCnf_g.apiIdForRsp)
    {
    	/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
		/*INVALID LC RESP*/
		if(0 != totalLen && 4 == totalLen)
		{
		    *(outMsg_p + current++) = totalLen; /*Using this variable to fill cause*/
		}
		else
#endif
	    /* OPTION3X Changes End */

        *(outMsg_p + current++) = GTPU_CAUSE_SUCCESS; 
    }

     /* Coverity 73424 Fix Start*/
     if( transId >= RELAY_GTPU_MAX_TRANS_ID )
     {
         freeMemPool(outMsg_p);
         return RELAY_GTPU_FAILURE;
     }
     /* Coverity 73424 Fix End*/

    /*Get the eNodeB type from the Sap flag values stored*/
/*INTEL_T3300 +*/
/* Coverity 73424/73402 Fix Start */
    if(transId < RELAY_GTPU_MAX_TRANS_ID)
    {
     if((relayMapBasedOnTransId_g[transId].sapFlags & PR_EGTPU_RX_SAP) &&
        (relayMapBasedOnTransId_g[transId].sapFlags & PR_EGTPU_FWD_SAP))
        {
          enbType = TARGET_ENB_VALUE;
        }
    }  
 /* Coverity 73424/73402 Fix End */

    for(loop1 = 0; loop1 < MAX_RAB_ID; loop1++)
   {
    current += getTlvLenAndComposeSetupSapCnf(outMsg_p,current,
            &s1ReconfigSapCnf_g.setupSapCnf[loop1],enbType,&cause);
   }

    /*Log the message to be sent*/
    if (S1U_RECONFIGURE_UE_ENTITY_CNF == s1ReconfigSapCnf_g.apiIdForRsp)
    {
        LOG_GTPU_MSG(PR_SEND_S1U_RECONFIG_CNF,LOGINFO,PR_GTPU_CONTROL,
        /* GCC_4_7_WARNING Fix Start */
                relayGtpuCurrentTick_g,__LINE__,ueIndex,
       /* GCC_4_7_WARNING Fix End */
                cause,0,0, 0,0, __func__, "");
    }
    else if (S1U_CREATE_UE_ENTITY_CNF == s1ReconfigSapCnf_g.apiIdForRsp)
    {
        LOG_GTPU_MSG(PR_SEND_S1U_CREATE_UE_ENTITY_CNF,LOGINFO,PR_GTPU_CONTROL,
        /* GCC_4_7_WARNING Fix Start */
                relayGtpuCurrentTick_g,__LINE__,ueIndex,
       /* GCC_4_7_WARNING Fix End */
                cause,totalLen,sizeof(inMsg_p), 0,0, __func__, "");
        /* +- SPR 17777 */
    }
    /*  + Layer2 NON CA Changes */  
    appendApiHeader(RELAY_S1AP_MODULE_ID, s1ReconfigSapCnf_g.apiIdForRsp, 
    s1ReconfigSapCnf_g.transIdForRsp, current, outMsg_p,s1uUEReconfigCnfInfo_g[ueIndex].cellIndex);
    /*  - Layer2 NON CA Changes */  

    /* Sending S1U_RECONFIGURE_UE_ENTITY_CNF to S1AP Module */
    udpSendToS1ap((Char8*)outMsg_p,current);

    /* + coverity 52877 */
    freeMemPool(outMsg_p);
    /* - coverity 52877 */

    return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : getTlvLenAndComposeRelTunnelInfoCnf
 * Inputs         : outMsg,
 *                  current,
 *                  tunnelInfo - Pointer to relayRelTunnelInfo,
 *                  lcIndex - Logical channel index
 * Outputs        : Tlv Length for Release Tunnel Info
 * Returns        : length
 * Description    : This funtion calculates the TLV Length and composes 
 *                  Release Tunnel Info IE.
 ****************************************************************************/

UInt16 getTlvLenAndComposeRelTunnelInfoCnf(UInt8 *outMsg, UInt16 current,
        relayRelTunnelInfo *tunnelInfo, UInt8 lcIndex)
{
    UInt16 length = 0;
#ifndef EPC_SETUP 
    UInt16 ueIndex =0;
    UInt8 outMsgTG[MAX_API_LEN + API_HDR] = {0};
    /*  + Layer2 NON CA Changes */  
    UInt16 currentTG = API_HDR;
    /*  - Layer2 NON CA Changes */  
    /* +- SPR 17777 */
#else
  LTE_GCC_UNUSED_PARAM(lcIndex)
    /* +- SPR 17777 */
#endif
    /* SPR 4790 Fix Start */
    UInt16 tlvLenPos = 0;
    /* SPR 4790 Fix End */

    RELAY_GTPU_SET_U16BIT(&outMsg[current],S1U_IE_RELEASE_TUNNEL_INFO);
    length += RELAY_SIZE_OF_TAG;
    current += RELAY_SIZE_OF_TAG;
 
            RELAY_GTPU_SET_U16BIT(&outMsg[current],RELAY_SIZE_OF_TAG + 
                    RELAY_SIZE_OF_LEN+ RELAY_SIZE_OF_TUNNELID );
            current += RELAY_SIZE_OF_LEN;
            RELAY_GTPU_SET_U32BIT(&outMsg[current],tunnelInfo->teidSelf);

            current += RELAY_SIZE_OF_TUNNELID; 
            length += RELAY_SIZE_OF_LEN + RELAY_SIZE_OF_TUNNELID;
/* SPR #2404 fix Start*/
            
            /*Send Info to DL TG*/
            /* mask 4MSB bits from ueIndex in case of MULTI_SECTOR*/
            /* SPR 4558 Fix Start */
#ifndef EPC_SETUP
            ueIndex = getUeIdxFromRrcUeIdx(s1ReconfigSapCnf_g.ueIndex);
            /* SPR 4558 Fix End */

    if(S1U_RECONFIGURE_UE_ENTITY_CNF == s1ReconfigSapCnf_g.apiIdForRsp)
       {
            sendMessageToDlTg(
                ueIndex,
                lcIndex + 3,
                INVALID_TUNNEL_ID,
                tunnelInfo->teidSelf,
                outMsgTG,currentTG,NORMAL_BEARER, 0,
                /*SPR 20646 fix start*/
                tunnelInfo->addrLen
                /*SPR 20646 fix end*/
                );
      }       
#endif
/*SPR #2404 fix End*/

    /*Populate correct length value of the tag S1U_IE_TUNNEL_INFO_CNF_TAG*/
    RELAY_GTPU_SET_U16BIT(&outMsg[tlvLenPos],length);

    return length;
}

/****************************************************************************
 * Function Name  : getTlvLenAndComposeReleaseSapCnf
 * Inputs         : outMsg,
 *                  current,
 *                  releaseSapCnf - Pointer to relayReleaseCnf,
 *                  recvCause
 * Outputs        : Tlv Length
 * Returns        : length
 * Description    : This funtion calculates the TLV Length and composes 
 *                  Release Sap Cnf IE.
 ****************************************************************************/
UInt16 getTlvLenAndComposeReleaseSapCnf(UInt8 *outMsg, UInt16 current,
                                        relayReleaseCnf *releaseSapCnf, UInt8 *recvCause)
{
    UInt16 length = 0,loop2;
    UInt16 tlvLenPos = 0, tunnelInfoCnfLen = 0, lcId = 0;
    UInt8 cause=0, lcIndex = 0;
    
      if(releaseSapCnf->bitMask)
      {
       cause = releaseSapCnf->cause;

          /*Fill the IEs for S1U_IE_RELAY_RELEASE_SAP_CNF_TAG*/    
          RELAY_GTPU_SET_U16BIT(&outMsg[current],S1U_IE_RELAY_REL_SAP_CNF_TAG);
          current += RELAY_SIZE_OF_TAG;
          tlvLenPos = current;
          current += RELAY_SIZE_OF_LEN;
          length += RELAY_SIZE_OF_TAG + RELAY_SIZE_OF_LEN;

            /*Populate lcId & increment length*/
            lcId =
                releaseSapCnf->lcId;

            outMsg[current++] = lcId; 
            length ++;

          /*Populate Cause and increment length*/ 
            outMsg[current++] = cause;
            length ++;
            *recvCause |= cause;

            /*Populate DB to be sent to TG*/
            /*Get the correct lcIndex*/
            lcIndex = lcId - 3;
            /* mask 4MSB bits from ueIndex in case of MULTI_SECTOR*/
            /* SPR 4558 Fix Start */
            s1uUEReconfigCnfInfo_g[(getUeIdxFromRrcUeIdx(s1ReconfigSapCnf_g.ueIndex))].rbInfo[lcIndex].lc_id = lcId;
            /* SPR 4558 Fix End */


            /*Populate Sequence Numbers & increment length*/
            if(GTPU_CAUSE_SUCCESS == cause)
            {
                RELAY_GTPU_SET_U16BIT(&outMsg[current],S1U_IE_SEQ_NUM_TAG);
                current += RELAY_SIZE_OF_TAG;
                /*Uplink and Downlink Sequence Number*/
                RELAY_GTPU_SET_U16BIT(&outMsg[current],2*RELAY_SIZE_OF_SEQ_NUM + 
                        RELAY_SIZE_OF_TAG + RELAY_SIZE_OF_LEN);
                current += RELAY_SIZE_OF_LEN;
                RELAY_GTPU_SET_U16BIT(&outMsg[current],
                        releaseSapCnf->seqNumUplink);
                current += RELAY_SIZE_OF_SEQ_NUM;

                RELAY_GTPU_SET_U16BIT(&outMsg[current],
                        releaseSapCnf->seqNumDnlink);
                current += RELAY_SIZE_OF_SEQ_NUM;

                length += RELAY_SIZE_OF_TAG + RELAY_SIZE_OF_LEN + 2*RELAY_SIZE_OF_SEQ_NUM;

            }

            for(loop2 = 0; loop2 < MAX_NUM_TUNNEL_PER_LC; 
                    loop2++)
            {
                /*Get the index where valid tunnel information is stored*/
                if(releaseSapCnf->relTunnelInfo[loop2].bitMask)
                {
                    tunnelInfoCnfLen = getTlvLenAndComposeRelTunnelInfoCnf
                          (outMsg, current,
                           &releaseSapCnf->relTunnelInfo[loop2],
                           lcIndex);
                    length+= tunnelInfoCnfLen;
                    current+= tunnelInfoCnfLen;
                }
            }/*loop2*/
       }/*If valid bitMask present for the lcId*/

    RELAY_GTPU_SET_U16BIT(&outMsg[tlvLenPos],length);


    return length ;
   
}
/****************************************************************************
 * Function Name  : processRelayReleaseSapCnf
 * Inputs         : inMsg_p,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS
 * Description    : This funtion send confirmation to S1-AP.
 ****************************************************************************/
UInt32 processRelayReleaseSapCnf
(
    UInt8 *inMsg_p, 
    UInt16 transId,
    UInt16 totalLen
)
{
    /* Coverity 55830 Start */
    UInt8 * outMsg_p = PNULL;
    outMsg_p = (UInt8 *)getMemFromPool(sizeof(UInt8) * (MAX_API_LEN + API_HDR), PNULL);
    if(PNULL == outMsg_p)
    {
        lteWarning("In %s, MEMORY ALLOC FAILURE\n",__func__);
        return RELAY_GTPU_FAILURE;
    }
    memSet(outMsg_p,0,sizeof(UInt8) *(MAX_API_LEN + API_HDR));
    /* Coverity 55830 End */
    /* SPR 4790 Fix Start */
    /*  + Layer2 NON CA Changes */  
    UInt16 current = API_HDR;
    /*  - Layer2 NON CA Changes */  
    /* SPR 4790 Fix End */
    UInt8 loop1, cause = 0;
            /* mask 4MSB bits from ueIndex in case of MULTI_SECTOR*/
            /* SPR 4558 Fix Start */

    /*  + Layer2 NON CA Changes */
    UInt16 ueIndex =  getUeIdxFromRrcUeIdx(s1ReconfigSapCnf_g.ueIndex);
    /*  - Layer2 NON CA Changes */  
            /* SPR 4558 Fix End */

    /* Filling Reqd Info to out Buffer */
    /* SPR :4790 Fix Start */
    RELAY_GTPU_SET_U16BIT(&outMsg_p[current],s1ReconfigSapCnf_g.ueIndex);
    /* SPR :4790 Fix End */
    current += 2;

    if (S1U_RECONFIGURE_UE_ENTITY_CNF ==
                s1ReconfigSapCnf_g.apiIdForRsp)
    {
        for(loop1 = 0; loop1 < MAX_RAB_ID; loop1++)
      {
         current += getTlvLenAndComposeReleaseSapCnf(outMsg_p,current, 
                    &s1ReconfigSapCnf_g.releaseSapCnf[loop1],&cause);
      }
       /*Log the message to be sent*/
        /* +- SPR 17777 */
        LOG_GTPU_MSG(PR_SEND_S1U_RECONFIG_CNF,LOGINFO,PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g,__LINE__,ueIndex,
                cause,sizeof(inMsg_p),totalLen,transId,0, __func__, "");
        /* +- SPR 17777 */
    /* SPR 3085 Fix Start */
    /* Append Header in front of whole message. transaction id sent is same
       as came during request. */

    /*  + Layer2 NON CA Changes */  
        appendApiHeader(RELAY_S1AP_MODULE_ID,s1ReconfigSapCnf_g.apiIdForRsp,
             s1ReconfigSapCnf_g.transIdForRsp,current,outMsg_p,s1uUEReconfigCnfInfo_g[ueIndex].cellIndex);
    /*  - Layer2 NON CA Changes */  

        udpSendToS1ap((Char8*)outMsg_p,current);

    /* SPR 3085 Fix End */
    }
    /* Coverity 55830 Start */
    freeMemPool(outMsg_p);
    /* Coverity 55830 End */
           
/* SPR 3277 Fix End */
 
    return RELAY_GTPU_SUCCESS;
}
/****************************************************************************
 * Function Name  : processRelayCpErrorSapInd
 * Inputs         : inMsg_p,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send confirmation to S1-AP.
 ****************************************************************************/

UInt32 processRelayCpErrorSapInd
(
    UInt8 *inMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    UInt8 outMsg[MAX_API_LEN + API_HDR] = {0};
    UInt16 startVal = API_HDR;

    /* SPR_9416_Changes Start */
    UInt32 relayTransId = 0; 
    UInt16 ueIndex = 0; 
    UInt16 rrcUeIndex = 0; 
    UInt8  lcId = 0; 
    /* SPR_9416_Changes End */
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(totalLen)
    /* +- SPR 17777 */

    if (inMsg_p)
    {

        /* SPR_9416 Changes start */
        relayTransId = *(UInt32 *)inMsg_p;
        /*+- Coverity 88175 Fix deleted*/
        /* Coverity_73477/73069 Fix Start*/
        if(relayTransId < RELAY_GTPU_MAX_TRANS_ID)
        {
            ueIndex = relayMapBasedOnTransId_g[relayTransId].ueIndex;
        }    
        else
        {
            return RELAY_GTPU_FAILURE;
        }
        /* Coverity_73477/73069 Fix End */
        rrcUeIndex = getRrcUeIdxFromUeIdx(ueIndex);
        lcId = relayMapBasedOnTransId_g[relayTransId].lcId;

        RELAY_GTPU_SET_U16BIT(&outMsg[startVal],rrcUeIndex);
        startVal +=2;

        outMsg[startVal++] = lcId;

        /* SPR_9416 Changes end */
    /*  + Layer2 NON CA Changes */  
	/* SPR 13378 fix Start  */
        appendApiHeader(RELAY_S1AP_MODULE_ID,S1U_ERROR_IND,\
            transId,startVal,outMsg,s1uUEReconfigCnfInfo_g[ueIndex].cellIndex);
	/* SPR 13378 fix end  */
    /*  - Layer2 NON CA Changes */  
        /* send message to S1-AP */
        udpSendToS1ap((Char8*)outMsg,startVal);
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processRelayPathFailureInd
 * Inputs         : respMsg_p,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion PATH FAILURE IND to S1-AP.
 ****************************************************************************/

UInt32 processRelayPathFailureInd
(
    UInt8 *respMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    if (respMsg_p && (API_HDR < totalLen))
    {
        /* Append API Header in front of message for sending to S1-AP */
        /*+SPR #3456*/
         /*
          * This message is not based on cell.
          * Hence cell field in  Header is filled
          * with API_HEADER_CELL_ID_NIL
          */
        appendApiHeader(RELAY_S1AP_MODULE_ID,S1U_PATH_FAILURE_IND,\
            transId,totalLen,respMsg_p,API_HEADER_CELL_ID_NIL); 
        /*-SPR #3456*/
        /* Send to S1-AP */
        udpSendToS1ap((Char8*)&respMsg_p[0],totalLen);
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/*+SPR #3456*/
/****************************************************************************
 * Function Name  : processRelayPathSuccessInd
 * Inputs         : respMsg_p,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send PATH SUCCESS IND to S1-AP.
 ****************************************************************************/

UInt32 processRelayPathSuccessInd
(
    UInt8 *respMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    if (respMsg_p && (API_HDR < totalLen))
    {
        /* Append API Header in front of message for sending to S1-AP */
    /*  + Layer2 NON CA Changes */  
         /*
          * This message is not based on cell.
          * Hence cell field in  Header is filled 
          * with API_HEADER_CELL_ID_NIL
          */
        appendApiHeader(RELAY_S1AP_MODULE_ID,S1U_PATH_SUCCESS_IND,\
            transId,totalLen,respMsg_p,API_HEADER_CELL_ID_NIL);
    /*  - Layer2 NON CA Changes */  
        /* Send to S1-AP */
        udpSendToS1ap((Char8*)&respMsg_p[0],totalLen);
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}
/*-SPR #3456*/

#ifdef DISPLAY_THROUGHPUT
/****************************************************************************
 * Function Name  : diffInMilliSecPhy
 * Inputs         : start - Pointer to Start TimeSpec,
 *                  end - Pointer to end TimeSpec
 * Outputs        : None
 * Returns        : diffInMicroseconds
 * Description    : This funtion send PATH SUCCESS IND to S1-AP.
 ****************************************************************************/
static UInt32 diffInMilliSecPhy(TimeSpec *  start, TimeSpec *  end)
{
  UInt32 diffInMicroseconds = 0;
  if (end->tv_sec  > start->tv_sec )
  {
    diffInMicroseconds = (end->tv_sec - start->tv_sec ) * 1000000;
  }
  if (start->tv_nsec >  end->tv_nsec)
  {
    diffInMicroseconds -= (start->tv_nsec -  end->tv_nsec)/1000;
  }
  else
  {
    diffInMicroseconds += (end->tv_nsec -  start->tv_nsec)/1000;
  }
  return (diffInMicroseconds/1000);
}
#endif


/****************************************************************************
 * Function Name  : processRelayDataInd
 * Inputs         : inMsg_p,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion send data to Pdcp Apapter.
 ****************************************************************************/

UInt32 processRelayDataInd
(
    UInt8 *inMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
#ifdef DISPLAY_THROUGHPUT
    static TimeSpec lasttime;
    static TimeSpec currtime;
    static unsigned long  datavolume = 0;
    UInt32 diff = 0;
#endif
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(transId)
    /* +- SPR 17777 */
    if (inMsg_p && (API_HDR < totalLen))
    {
#ifdef DISPLAY_THROUGHPUT
      clock_gettime_real_time(&currtime);
      datavolume += totalLen;
      if ( (diff = diffInMilliSecPhy(&lasttime,&currtime)) > 5000 )
      {
        printf("Throughput=%u\n",(datavolume*8*1000)/diff);
        datavolume =0;
        clock_gettime_real_time(&lasttime);
      }
#endif
        /* Now send data to PDCP Adapter */
        udpSendToAdapter((Char8*)&inMsg_p[0],totalLen);
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : appendCsplHeader
 * Inputs         : moduleId,
 *                  apiId,
 *                  totalLen,
 *                  msgBuff_p
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion append CSPL Header.
 ****************************************************************************/

void appendCsplHeader
(
    UInt16 moduleId,
    UInt16 apiId,
    UInt16 totalLen,
    UInt8* msgBuff_p
)
{
    UInt16 current = 0;
    if (msgBuff_p)
    {
        msgBuff_p[current++] = GTPU_API_VERSION_NUMBER; 
        msgBuff_p[current++] = GTPU_DEF_PRIORITY; 
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],moduleId);
        current +=2;
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],GTPU_MODULE_ID);
        current +=2;
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],apiId);
        current +=2;
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],totalLen);
        current +=2;
        if (GTPU_DATA_REQ == apiId || GTPU_UNITDATA_IND == apiId || GTPU_RELOC_DATA_REQ ==apiId 
#ifdef ENDC_ENABLED
            /*Dual Connectivity SeNB changes +*/
            || (GTPU_X2U_DELIVERY_STATUS == apiId)
            /*Dual Connectivity SeNB changes -*/
#endif
            )
        {
            RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],totalLen - 1);
            current +=2;
            if (GTPU_DATA_REQ == apiId || GTPU_RELOC_DATA_REQ == apiId 
#ifdef ENDC_ENABLED
                    || (GTPU_X2U_DELIVERY_STATUS == apiId)
#endif
               ) 
            {
                RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],START_PAYLOAD);
            }
            else 
            {
                RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],CSPL_HDR);
            }
            current +=2;
        }
        else
        {
            RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],0);
            current +=2;
            RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],0);
            current +=2;
        }

        if ( GTPU_RELOC_DATA_REQ ==apiId )
        {
            RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],24); /*API Header(16) + teidSelf(5) + pdcp_num(3)*/
        }
        else
        {
            RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],totalLen-1);
        }
        current +=2;
    } 
}
/*SPR 3587 Fix Start*/
/****************************************************************************
 * Function Name  : appendCsplHeaderForDL
 * Inputs         : moduleId,
 *                  apiId,
 *                  totalLen,
 *                  msgBuff_p
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion append CSPL Header for Downlink.
 ****************************************************************************/
void appendCsplHeaderForDL
(
    UInt16 moduleId,
    UInt16 apiId,
    UInt16 totalLen,
    UInt8* msgBuff_p
)
{
    UInt32 iterator = 0;
    UInt16 unusedBytes = 0;
    UInt16 current = 0;
    if (msgBuff_p)
    {
        msgBuff_p[current++] = GTPU_API_VERSION_NUMBER;
        msgBuff_p[current++] = GTPU_DEF_PRIORITY; 
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],moduleId);
        current +=2;
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],GTPU_MODULE_ID);
        current +=2;
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],apiId);
        current +=2;
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],totalLen);
        current +=2;
        if (GTPU_DATA_REQ == apiId || GTPU_UNITDATA_IND == apiId || GTPU_RELOC_DATA_REQ ==apiId )
        {
            RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],totalLen - 1);
            current +=2;
            /* SPR 4357 Fix Start */  
          //  if (GTPU_DATA_REQ == apiId || GTPU_RELOC_DATA_REQ == apiId) 
          //  {
                RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],START_PAYLOAD);
          //  }
          //  else 
          //  {
                //RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],CSPL_HDR);
            //    RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],64);
           // }
            /* SPR 4357 Fix End */  
            current +=2;
        }
        else
        {
            RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],0);
            current +=2;
            RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],0);
            current +=2;
        }

        if ( GTPU_RELOC_DATA_REQ ==apiId )
        {
            RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],24); /*API Header(16) + teidSelf(5) + pdcp_num(3)*/
        }
        else
        {
            RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],totalLen-1);
        }
        current +=2;
        /*Encode Source UDP Port and Source IPv4 Address*/
        /*Note: IPv6 is currently not supported in Packet Relay, this needs to be implemented in future releases*/
        /*Type value for UDP Port number element*/
        RELAY_GTPU_SET_VALUE(&msgBuff_p[current],EGTPU_IE_PORT_NUM);
        current +=1;
        /*UDP Port Number*/
        /* + SPR 19022 */
        if( EGTPU_MSGT_ECHO_REQUEST == msgBuff_p[START_PAYLOAD + 1] ) 
        {
            RELAY_SET_PORT_NUM_IE(&msgBuff_p[current],ntohs_wrapper(relayConfigInfo_g.sgwToAddr.sin_port));
        }
        else
        {
            RELAY_SET_PORT_NUM_IE(&msgBuff_p[current],ntohs_wrapper(EGTPU_UDP_PORT));
        }
        /* - SPR 19022 */
        current +=2;
        /*Type for Transport address element:IPv4*/
        RELAY_GTPU_SET_VALUE(&msgBuff_p[current],EGTPU_IE_GSN_ADDR);
        current +=1;
        /*IPV6_CHANGE - START*/
#ifdef LAYER2_DLTG_THREAD
        /*Encoding Length of IP address:Currently it is IPv4:4 only*/
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],4);
        current +=2;
        /* SPR 6949 Fix Start */
        RELAY_SET_ENTITY_ADDR_IE(&msgBuff_p[current],ntohl_wrapper(relayConfigInfo_g.sgwToAddr.sin_addr.s_addr));
        /* SPR 6949 Fix End */
        current +=4;
#else
        if(IPV4_ADDR_PRESENT == sockAddrType)
        {
            /*Encoding Length of IPv4 address*/
            RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],RELAY_ADDR_LEN_IPV4);
            current +=2;
            /* SPR 6949 Fix Start */
            RELAY_SET_ENTITY_ADDR_IE(&msgBuff_p[current],ntohl_wrapper(relayConfigInfo_g.sgwToAddr.sin_addr.s_addr));
            /* SPR 6949 Fix End */
            current +=RELAY_ADDR_LEN_IPV4;
        }
        else
        {
            /*Encoding Length of IPv6 address*/
            RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],RELAY_ADDR_LEN_IPV6);
            current +=2;
            /* SPR 6949 Fix Start */
            memCpy(&msgBuff_p[current], relayConfigInfo_g.sgwToAddrIn6.sin6_addr.s6_addr, RELAY_ADDR_LEN_IPV6);
            /* SPR 6949 Fix End */
            current +=RELAY_ADDR_LEN_IPV6;
        }
#endif
        /*IPV6_CHANGE - END*/
        /*Creating void on remaining bytes of 64 bytes*/
        if(current <= START_PAYLOAD)
        {
            unusedBytes = (START_PAYLOAD - current);
            for(iterator=0; iterator<unusedBytes; ++iterator)
            {
                msgBuff_p[current] = 0x00;
                current++;
            }
        }
        else
        {
            //Error
        }
    }
}
/*SPR 3587 Fix End*/
/****************************************************************************
 * Function Name  : appendApiHeader
 * Inputs         : moduleId,
 *                  apiId,
 *                  transId,
 *                  msgLen,
 *                  respBuff_p
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion append API Header in front of message.
 ****************************************************************************/

void appendApiHeader
(
    UInt16 moduleId,
    UInt16 apiId,
    UInt16 transId,
    UInt16 msgLen,
    UInt8 *respBuff_p,
    /*  + Layer2 NON CA Changes */  
    RrcCellIndex cellId
    /*  - Layer2 NON CA Changes */  
)
{
    UInt8 startVal = 0;
    RELAY_GTPU_SET_U16BIT(&respBuff_p[startVal],transId);
    startVal +=2;
    RELAY_GTPU_SET_U16BIT(&respBuff_p[startVal],RELAY_MODULE_ID);
    startVal +=2;
    RELAY_GTPU_SET_U16BIT(&respBuff_p[startVal],moduleId);
    startVal +=2;
    /* For each message sent outside, Source Module always will be Packet
       Relay so it is hard coded bolew */
    RELAY_GTPU_SET_U16BIT(&respBuff_p[startVal],apiId);
    startVal +=2;
    /* For each message, Msg Length will include whole packet length. Calling
       Function will take care for this. */
    RELAY_GTPU_SET_U16BIT(&respBuff_p[startVal],msgLen);

 /* + Layer2 NON CA Changes */
    respBuff_p[API_HEADER_CELL_INDEX_OFFSET] = cellId;
 /* - Layer2 NON CA Changes */

}

/****************************************************************************
 * Function Name  : prepareAndSendErrorReportToLM
 * Inputs         : transId,
 *                  apiId
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE(with error code)
 * Description    : This funtion send faliure Rsp to LM.
 ****************************************************************************/

UInt32 prepareAndSendErrorReportToLM
(
    UInt16 transId,
    UInt16 apiId
)
{
    UInt8 errorReport[MAX_API_LEN + OAM_L2_API_HDR] = {0};
    UInt16 current = 0;
    /* Cpoy Transaction Identifier */
    RELAY_GTPU_SET_U16BIT(&errorReport[current],transId);
    current += 2;
    /* Copy Source Module identifier */ 
    RELAY_GTPU_SET_U16BIT(&errorReport[current],RELAY_MODULE_ID);
    current += 2;
    /* Copy Destination Module identifier */ 
    RELAY_GTPU_SET_U16BIT(&errorReport[current],RELAY_OAM_MODULE_ID);
    current += 2;
    /* Copy API identifier */ 
    RELAY_GTPU_SET_U16BIT(&errorReport[current],apiId);
    current += 2;
    /* Append Total length */
    RELAY_GTPU_SET_U16BIT(&errorReport[current],OAM_L2_API_HDR +1);
    current += 2;
 /* + Layer2 NON CA Changes */ 
    errorReport[current++] = transIdMappingForLM_g[apiId].cellId;
 /* - Layer2 NON CA Changes */ 
    /* Append Error Code */
    errorReport[current++] = RELAY_GTPU_PERR_UNEXPECTED_MSG;
    udpSendToOam((Char8*)errorReport,current);
    /* Send to OAM */
    return RELAY_GTPU_SUCCESS;
}
/* SPR 2030 fix Start */

/****************************************************************************
 * Function Name  : processRelayGetLogLevelResp
 * Inputs         : respMsg_pi
 *                  transId
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function sends PR_GTPU_GET_LOG_LEVEL_RESP to OAM.
 ****************************************************************************/
UInt32 processRelayGetLogLevelResp(UInt8 *respMsg_p,UInt16 transId,
        UInt16 totalLen)
{
   /* Check for incoming message and length of that message should be greater 
      than API_HDR length */
   if (respMsg_p && (API_HDR <= totalLen))
   {
       /* Append header in front of message, TransId will be same as was sent
          in request message */ 
 /* + Layer2 NON CA Changes */
       appendApiHeader(RELAY_OAM_MODULE_ID,PR_GTPU_GET_LOG_LEVEL_RESP,\
         transId,totalLen,respMsg_p,respMsg_p[API_HEADER_CELL_INDEX_OFFSET]);
 /* - Layer2 NON CA Changes */

       /* send to OAM */
       udpSendToOam((Char8*)&respMsg_p[0],totalLen);
       return RELAY_GTPU_SUCCESS;
   }
   return RELAY_GTPU_FAILURE;
}


/****************************************************************************
 * Function Name  : processRelayGetLogCategoryResp
 * Inputs         : respMsg_p,
 *                  transId
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function sends PR_GTPU_GET_LOG_CATEGORY_RESP to OAM.
 ****************************************************************************/
UInt32 processRelayGetLogCategoryResp(UInt8 *respMsg_p,UInt16 transId,
        UInt16 totalLen)
{
   if (respMsg_p && (API_HDR <= totalLen))
   {
       /* Append header in front of message, TransId will be same as was sent
          in request message */ 
 /* + Layer2 NON CA Changes */
       appendApiHeader(RELAY_OAM_MODULE_ID,PR_GTPU_GET_LOG_CATEGORY_RESP,
           transId,totalLen,respMsg_p,respMsg_p[API_HEADER_CELL_INDEX_OFFSET]);
 /* - Layer2 NON CA Changes */


       /* send to OAM */
       udpSendToOam((Char8*)&respMsg_p[0],totalLen);
       return RELAY_GTPU_SUCCESS;
   }
   return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : pdcpFirstSubFrameInd
 * Inputs         : sysFrameNum,
 *                  subFrameNum
 * Outputs        : None
 * Returns        : None
 * Description    : This function 
 ****************************************************************************/
void pdcpFirstSubFrameInd(UInt32 sysFrameNum, UInt32 subFrameNum)
{
    relayGtpuCurrentTick_g = sysFrameNum*10 + subFrameNum;
}

/* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
/****************************************************************************
 * Function Name  : pdcpFirstSubFrameIndFrmL2Tick
 * Inputs         : currentTickFrmL2Tick
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates gtpu tick with current tick. 
 ****************************************************************************/
/* SPR 15909 fix start */ 
void pdcpFirstSubFrameIndFrmL2Tick(tickType_t currentTickFrmL2Tick)
/* SPR 15909 fix end */
{
    relayGtpuCurrentTick_g = currentTickFrmL2Tick;
}
#endif
/* FAPI L2 AS MASTER END*/

/****************************************************************************
 * Function Name  : pdcpSubFrameInd
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function 
 ****************************************************************************/
/* + SPR 17439 */
void pdcpSubFrameInd(void)
/* - SPR 17439 */    
{

    relayGtpuCurrentTick_g++;

    /*+SPR #3456*/
    /*
     * This func is moved from
     * UL_Thread to Main_Thread.
     * To reduce overload on UL Thread
     */
    /* SPR_9432_Fix Start */
    // egtpu_check_echo_timer_expiry();
    /* SPR_9432_Fix End */

    /*-SPR #3456*/
}

/* SPR 2030 fix End */
/****************************************************************************
 * Function Name  : processRelayCpEndMarkerInd
 * Inputs         : inMsg_p,i
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS
 * Description    : This funtion send End Marker Ind to S1-AP.
 ****************************************************************************/

UInt32 processRelayCpEndMarkerInd
(
    UInt8 *inMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    /*  + Layer2 NON CA Changes */  
    UInt8 outMsg[MAX_API_LEN + API_HDR] = {0};
    /* +- Coverity 88172 Fix code delted*/
 /* Coverity_73402/73424 Fix Start */
    UeIndexLcIdTunnelIdMap *respMsg_p = PNULL;
 /* Coverity_73402/73424 Fix End */

    UInt16 current = API_HDR;

 /* Coverity_73402/73424 Fix Start */
    if(transId < RELAY_GTPU_MAX_TRANS_ID)
    { 
       respMsg_p = &relayMapBasedOnTransId_g[transId];
    }
    else
    {
        return RELAY_GTPU_FAILURE;
    }
 /* Coverity_73402/73424 Fix End */

    /*  - Layer2 NON CA Changes */  

    /* Filling Reqd Info to out Buffer */
    /* SPR 18819 fix start */
    UInt16 rrcUeIdx = 0;
    rrcUeIdx = getRrcUeIdxFromUeIdx(respMsg_p->ueIndex);

    RELAY_GTPU_SET_U16BIT(&outMsg[current],rrcUeIdx);
    /* SPR 18819 fix end */
    current += 2;
    outMsg[current++] = respMsg_p->lcId;

    RELAY_GTPU_SET_U32BIT(&outMsg[current],respMsg_p->tunnelId);
    current+=4;
    /*Set API Header*/
    /*  + Layer2 NON CA Changes */  
    appendApiHeader(RELAY_S1AP_MODULE_ID, S1U_END_MARKER_IND,
            transId, current, outMsg,s1uUEReconfigCnfInfo_g[respMsg_p->ueIndex].cellIndex);
    /*  - Layer2 NON CA Changes */  

    /* send message to S1-AP */
    udpSendToS1ap((Char8*)outMsg,current);

    /* +- SPR 17777 */
    LOG_GTPU_MSG(PR_SEND_EMI_TO_CP,LOGINFO, PR_GTPU_CONTROL,
             relayGtpuCurrentTick_g, __LINE__,
             respMsg_p->ueIndex,respMsg_p->lcId,sizeof(inMsg_p),totalLen, 0,0,
             __func__,"RELAY_GTPU_CP_END_MARKER_IND"); 

    return RELAY_GTPU_SUCCESS;

}

/****************************************************************************
 * Function Name  : processRelayPdcpEndMarkerInd
 * Inputs         : inMsg_p,
 *                  transId,
 *                  totalLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS
 * Description    : This funtion send End Marker Ind to S1-AP.
 ****************************************************************************/
UInt32 processRelayPdcpEndMarkerInd
(
    UInt8 *inMsg_p,
    UInt16 transId,
    UInt16 totalLen
)
{
    UInt8 outMsg[MAX_API_LEN + API_HDR] = {0};
    /*+- Coverity 88171 Fix Code Deleted*/
    /* Coverity 73424/73402 Fix Start */
    UeIndexLcIdTunnelIdMap *respMsg_p = PNULL;

    if(transId < RELAY_GTPU_MAX_TRANS_ID)
    { 
        respMsg_p = &relayMapBasedOnTransId_g[transId];
    }
    else
    {
        return RELAY_GTPU_FAILURE;
    }
    /* Coverity 73424/73402 Fix End */
   

    UInt16 current = API_HDR;

    /* Filling Reqd Info to out Buffer */
    RELAY_GTPU_SET_U16BIT(&outMsg[current],respMsg_p->ueIndex);
    current += 2;
    outMsg[current++] = respMsg_p->lcId;
    /*Set API Header*/
 /* + Layer2 NON CA Changes */
         /*
          * This message is not based on cell.
          * Hence cell field in  Header is filled 
          * with API_HEADER_CELL_ID_NIL
          */
    appendApiHeader(RELAY_ADAPTER_MODULE_ID, RELAY_GTPU_PDCP_END_MARKER_IND,
            0, current, outMsg,API_HEADER_CELL_ID_NIL);
 /* - Layer2 NON CA Changes */

        /* send message to PDCP */
    /* +- SPR 17777 */
    LOG_GTPU_MSG(PR_SEND_EMI_TO_PDCP,LOGINFO, PR_GTPU_CONTROL,
            relayGtpuCurrentTick_g, __LINE__,
            respMsg_p->ueIndex,respMsg_p->lcId,sizeof(inMsg_p),totalLen, 0,0,
            __func__,"RELAY_GTPU_CP_END_MARKER_IND");

    /* +- SPR 17777 */
    lteRelayMsgHandler(outMsg);
    /* +- SPR 17777 */

    return RELAY_GTPU_SUCCESS;
}

/* LIPA changes start */
/****************************************************************************
 * Function Name  : sendMessageToDlTgForSecondaryTunnel
 * Inputs         : ueIndex
 *                  lc_id
 *                  teid_self
 *                  teid_peer
 *                  tunnelType
 * Outputs        : Resp
 * Returns        : Resp
 * Description    : This funtion sends the secondary tunnel info to DL-TG
 *                  about teidSelf and teid-Peer.
 ****************************************************************************/
#ifndef EPC_SETUP
UInt8 sendMessageToDlTgForSecondaryTunnel (UInt16 ueIndex,
                                           UInt8 lc_id,
                                           UInt32 teid_self, 
                                           UInt32 teid_peer,
                                           UInt8 tunnelType)
{
    UInt8 outMsgTG[MAX_API_LEN + API_HDR] = {0};
    /*  + Layer2 NON CA Changes */  
    UInt16 currentTG = API_HDR;
    /*  - Layer2 NON CA Changes */  

    RELAY_GTPU_SET_U32BIT((outMsgTG+currentTG), teid_self);
    currentTG +=4;
    RELAY_GTPU_SET_U32BIT((outMsgTG+currentTG), teid_peer);
    currentTG +=4;
    RELAY_GTPU_SET_U16BIT((outMsgTG+currentTG), ueIndex);
    currentTG +=2;
    *(outMsgTG+currentTG) = lc_id;
    currentTG ++;
    *(outMsgTG+currentTG) = tunnelType;
    currentTG ++;

    /*  + Layer2 NON CA Changes */  
    appendApiHeader(RELAY_S1AP_MODULE_ID, S1U_RECONFIGURE_UE_ENTITY_CNF, 0, 
    currentTG, outMsgTG,s1uUEReconfigCnfInfo_g[ueIndex].cellIndex);
    /*  - Layer2 NON CA Changes */  

    if(RELAY_GTPU_SUCCESS !=  udpSendToSecondaryTG(outMsgTG , currentTG , relayConfigInfo_g.peerTgInfAddrType))
    {
        LOG_GTPU_MSG(PR_SENT_TO_SECONDARY_TG_GET_FAILED,LOGERROR, PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g, __LINE__,
                teid_self,teid_peer,0,0, 0,0,
                __func__,"PR_Sentto to Secondary TG get Failed");
        return RELAY_GTPU_FAILURE;
    }
    return RELAY_GTPU_SUCCESS;
}
#endif

/* SPR 7085 fix start */
/**************************************************************
 *  Function Name  : processRelayGetStatsResp
 *  Inputs         : respMsg_p,
 *                   transId,
 *                   totalLen
 *  Outputs        : None
 *  Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 *  Description    : This funtion send Stats Resp to OAM.
 **************************************************************/

UInt32 processRelayGetStatsResp(UInt8 *respMsg_p,
                                UInt16 transId,
                                UInt16 totalLen)
{
    /* Check for incoming message and length of that message should be greater
     *       than 10 */
    if (respMsg_p && (API_HDR <= totalLen))
    {
        /* Append header in front of message, TransId will be same as was sent
         *           in request message */
    /*  + Layer2 NON CA Changes */  
        appendApiHeader(RELAY_OAM_MODULE_ID,PR_GTPU_GET_STATS_RESP,
             transId,totalLen,respMsg_p,
             transIdMappingForLM_g[PR_GTPU_GET_STATS_RESP].cellId);
    /*  - Layer2 NON CA Changes */  
        /* send to OAM */
        LTE_LOG(LOG_INFO,"[SENDING] PR_GTPU_GET_STATS_RESP to OAM\n");
        udpSendToOam((Char8*)&respMsg_p[0],totalLen);
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}
#if defined (ENDC_ENABLED) && defined(PDCP_GTPU_INTF)
extern UInt8 gtpu_Pdcp_Instance_g[MAX_RRC_CELL_INDEX];
extern void sendFreetoPdcp ( void *q, void *buf_p );
/**************************************************************
 *  Function Name  : sendDatatoRlc
 *  Description    : This funtion send DL PDCP data to RLC
 *  Returns        : None
 **************************************************************/
void sendDatatoRlc(
        UInt16 lcId,
        UInt16 ueIndex,
        UInt16 dataLen,
        UInt16 transLen,
        UInt16 serviceRequest,
        UInt8  transactionPresent,
        void   *data_p,
        void   *transId,
        tickType_t pktArrTime)
{
    UInt8  *sendBuffer_p = PNULL;
    UInt8  instanceId = 0;
    UInt16 size = API_HDR;
    egtpu_rab_ctxt_t *p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(p_global,
    relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].tunnelId);
    if(PNULL == p_rab_ctxt)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,
                "RAB Context is null for given  \
                ueIndex [%d] lcId [%d] \n",ueIndex,lcId);

        return;
    }
    instanceId = gtpu_Pdcp_Instance_g[p_rab_ctxt->teid_cell.cellIndex];

    sendBuffer_p = (UInt8 *)getMemFromPool(BUFFERSIZE, PNULL);

    if(PNULL == sendBuffer_p)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,
                "Memory Allocation failed for buffer to be sent to RLC \
                ueIndex [%d] lcId [%d] \n",ueIndex,lcId);
        return;
    }
    ueIndex = getRrcUeIdxFromUeIdx(ueIndex);

    RELAY_GTPU_SET_U16BIT(&sendBuffer_p[size], ueIndex);
    size += 2;
    RELAY_GTPU_SET_U16BIT(&sendBuffer_p[size], lcId);
    size += 2;
    RELAY_GTPU_SET_U16BIT(&sendBuffer_p[size], dataLen);
    size += 2;
    *(sendBuffer_p +size) = transactionPresent;
    size++;
    RELAY_GTPU_SET_U16BIT(&sendBuffer_p[size], transLen);
    size += 2;
    RELAY_GTPU_SET_U64BIT(&sendBuffer_p[size], pktArrTime);
    size += 8;
    RELAY_GTPU_SET_U16BIT(&sendBuffer_p[size], serviceRequest);
    size += 2;

    msgExtract(data_p, 0, &sendBuffer_p[size], dataLen);
    size += dataLen;

    msgExtract(transId, 0, &sendBuffer_p[size], transLen);
    size += transLen;

    appendApiHeader(RLC_MODULE_ID,X2U_RLC_DL_DATA_REQ,
            0xFF,size,sendBuffer_p,
            API_HEADER_CELL_ID_NIL);
     if ( -1 ==
         sendto_wrapper(relayConfigInfo_g.x2uRlc_Ip_Port[instanceId].rlcRxFd,&sendBuffer_p[0],size,0,\
             &relayConfigInfo_g.x2uRlc_Ip_Port[instanceId].rlcRxAddr,sizeof(SockAddr)))
     {
         LOG_GTPUPR_ERROR( PR_GTPU_COMMON, "Not able to send UDP Packet" );
     }
     freeMemPool(sendBuffer_p);
}
/**************************************************************
 *  Function Name  : sendMessageToX2uRelay
 *  Description    : This funtion send RLC message to X2U Relay
 *  Returns        : None
 **************************************************************/
void sendMessageToX2uRelay(UInt8 *msgBuf_p, SInt32 bytesread)
{
    void        *ulData_p = PNULL;
    UInt8       packetForReEstablishment;
    UInt8       *tempBuff_p = PNULL;
    UInt16      ulDataLen = 0;
    UInt16      msgLen = 0;
    UInt16      current = 0;
    UInt16      msgId = 0;
    UInt16      ueIndex = 0;
    UInt16      lcId = 0;
    UInt32      count = 0;
    tickType_t  headPktTS = 0;
    tickType_t  pktTimeStamp = 0;
    RlcStatusBuffer       *deliveryStatus_p = PNULL;


    current += 6;
    msgId = LTE_GET_U16BIT(msgBuf_p + current);
    current += 2;
    msgLen = LTE_GET_U16BIT(msgBuf_p + current);
    current += 4;

    if ( (UInt16)bytesread != msgLen )
    {
        LOG_GTPUPR_WARNING( PR_GTPU_COMMON, "msgLength[%d] not "
                "same as byteRead[%d] msgId[%d]", msgLen, bytesread,msgId );

        return;
    }
    tempBuff_p = msgBuf_p + INTERFACE_API_HEADER_LEN;
    current = INTERFACE_API_HEADER_LEN;
    if(RLC_X2U_DL_DATA_DELIVERY_STATUS == msgId)
    {
        ueIndex = RELAY_GTPU_GET_U16BIT(&tempBuff_p[current]);
        current += 2;
        
        lcId    = *(tempBuff_p + current);
        current ++;
        
        count   = RELAY_GTPU_GET_U32BIT(&tempBuff_p[current]);
        current += 4;
        
        deliveryStatus_p = (RlcStatusBuffer *)&tempBuff_p[current];

        ueIndex = getUeIdxFromRrcUeIdx( ueIndex );

        if(!rlcX2uDlDataDeliveryStatus(ueIndex,lcId,count, deliveryStatus_p))
        {
            LOG_GTPUPR_WARNING( PR_GTPU_COMMON, " DL Data Delivery Status processing failed" 
                    "ueIndex:%u lcId:%u",ueIndex, lcId);

        }
        freeMemPool(msgBuf_p);
        msgBuf_p = PNULL;

    }
    else if(RLC_X2U_UL_DATA_IND == msgId)
    {
        ueIndex = RELAY_GTPU_GET_U16BIT(&tempBuff_p[current]);
        current += 2;
        
        lcId    = *(tempBuff_p + current);
        current ++;

        packetForReEstablishment = *(tempBuff_p + current);
        current++;

        ulDataLen    = RELAY_GTPU_GET_U16BIT(&tempBuff_p[current]);
        current += 2;

        headPktTS = RELAY_GTPU_GET_U64BIT(&tempBuff_p[current]);
        current += 8;

        pktTimeStamp = RELAY_GTPU_GET_U64BIT(&tempBuff_p[current]);
        current += 8;
        ulData_p = msgAlloc(PNULL, 0, 0, 0);

        if (PNULL == ulData_p)
        {
            sendFreetoPdcp( 0, msgBuf_p);
            ltePanic("msgAlloc failed in %s", __FUNCTION__);
        }
        if (0 == qvMsgInsertExternal(ulData_p, 0, &tempBuff_p[current],
                            ulDataLen, sendFreetoPdcp, (void *)tempBuff_p))
        {
            msgFree( ulData_p );
            ulData_p = PNULL;
            sendFreetoPdcp(0,msgBuf_p);
            ltePanic("qvMsgInsertExternal failed in %s",__func__);
        }
        
        ueIndex = getUeIdxFromRrcUeIdx( ueIndex );

        if(RELAY_GTPU_FAILURE == rlcX2uUlDataInd(ueIndex,
                        lcId,
                        packetForReEstablishment,
                        ulDataLen,
                        headPktTS,
                        pktTimeStamp,
                        ulData_p
                        ))
        {
            LOG_GTPUPR_WARNING( PR_GTPU_COMMON, "UL data processing failed at X2U "
                    "msgId[%d] msgLen[%d]", msgId, msgLen );
        }
    }
    else
    {
        LOG_GTPUPR_WARNING( PR_GTPU_COMMON, "invalid API received from RLC "
                "msgId[%d] msgLen[%d]", msgId, msgLen );

    }
}
/**************************************************************
 *  Function Name  : sendRlcDiscardReq
 *  Description    : This funtion send RLC Data discard req message
 *  Returns        : None
 **************************************************************/
UInt16 sendRlcDiscardReq(UInt16 ueIndex, UInt16 lcId, void *transactionId, UInt16 transIdLen)
{
    UInt8  *sendBuffer_p = PNULL;
    UInt8  instanceId = 0;
    UInt16 size = API_HDR;
    egtpu_rab_ctxt_t *p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(p_global,
    relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].tunnelId);
    if(PNULL == p_rab_ctxt)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,
                "RAB Context is null for given  \
                ueIndex [%d] lcId [%d] \n",ueIndex,lcId);

        return RELAY_GTPU_FAILURE;
    }
    instanceId = gtpu_Pdcp_Instance_g[p_rab_ctxt->teid_cell.cellIndex];

    sendBuffer_p = (UInt8 *)getMemFromPool(BUFFERSIZE, PNULL);

    if(PNULL == sendBuffer_p)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,
                "Memory Allocation failed for buffer to be sent to RLC \
                ueIndex [%d] lcId [%d] \n",ueIndex,lcId);
        return RELAY_GTPU_FAILURE;
    }
    ueIndex = getRrcUeIdxFromUeIdx(ueIndex);

    RELAY_GTPU_SET_U16BIT(&sendBuffer_p[size], ueIndex);
    size += 2;

    RELAY_GTPU_SET_U16BIT(&sendBuffer_p[size], lcId);
    size += 2;
    
    RELAY_GTPU_SET_U16BIT(&sendBuffer_p[size], transIdLen);
    size += 2;

    msgExtract(transactionId, 0, &sendBuffer_p[size], transIdLen);
    size += transIdLen;

    appendApiHeader(RLC_MODULE_ID,X2U_RLC_DATA_DISCARD_REQ,
            0xFF,size,sendBuffer_p,
            API_HEADER_CELL_ID_NIL);
     if ( -1 ==
         sendto_wrapper(relayConfigInfo_g.x2uRlc_Ip_Port[instanceId].rlcRxFd,&sendBuffer_p[0],size,0,\
             &relayConfigInfo_g.x2uRlc_Ip_Port[instanceId].rlcRxAddr,sizeof(SockAddr)))
     {
         LOG_GTPUPR_ERROR( PR_GTPU_COMMON, "Not able to send UDP Packet" );
     }
     freeMemPool(sendBuffer_p);
     return RELAY_GTPU_SUCCESS;
}
#endif
