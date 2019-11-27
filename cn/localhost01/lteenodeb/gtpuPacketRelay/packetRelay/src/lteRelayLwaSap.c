/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteRelayLwaSap.c,v 1.2 2016/10/03 09:57:08 gur20882 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definitions of Data APIs
 *                   : between Relay and LWA Adapter.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRelayLwaSap.c,v $
 *
 * Revision 1.1.2.1  2016/10/03 09:57:08  cm_lteenodeb
 * Intial version  
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/


/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteRelay.h"
#include "lteRelayS1Sap.h"
#include "lteRelayGtpuSap.h"
#include "lteRelayCommon.h"
#include "lteMemPool.h"
#include "egtpu_cdb.h"
#include "lteRelayAdapter.h"

#include "lteMisc.h"
#include "lteMessageQIntf.h"
#include "lteLayer2CommanTypes.h"


/******************************************************************************
  Private Definitions
 *****************************************************************************/
extern SInt32 s1uIpv4SockRx_g;
extern SInt32 ipv6SockRx_g;
/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 *****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/


/****************************************************************************
 * Functions implementation
 *****************************************************************************/

/****************************************************************************
 * Function Name  : sendToWT 
 * Inputs         : ueIndex  - UE Index,
 *                  lcId     - Logical Channel Id,
 *                  data_p   - Data buffer,
 *                  msgLen   - Message length
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion sends message to WT.
 ****************************************************************************/
void sendToWT(UInt16 ueIndex, UInt8 lcId, void *data_p, UInt16 msgLen)
{
    egtpu_rab_ctxt_t* p_rab_ctxt = NULL;

    /* Fetch RAB context based on tunnel Id */
    p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(p_global, 
                    relayMapBasedOnUeIndexLcId_g[ueIndex][lcId-3].lwaTunnelId);
    if (NULL != p_rab_ctxt)
    {
        UInt8       addr_len;
        sockAddrIn6 sockaddr6;
        SInt8       socketfd    = 0;
        void*       pSockAddr   = NULL;
        UInt16      sockAddrLen = 0;

        if (NULL != p_rab_ctxt->p_transport_addr)
        {
            /* Fetch length of peer address */
            addr_len = RELAY_GTPU_GET_U16BIT(&(p_rab_ctxt->p_transport_addr->entity_addr[0]));
            if (addr_len > RELAY_ADDR_LEN_IPV6)
            {
                /*START:LWA:DATA:FIX*/
                //msgFree(data_p);
                //data_p = NULL;
                /*END:LWA:DATA:FIX*/
                return;
            }

            /* Handling for IPv4 address */
            if (RELAY_ADDR_LEN_IPV4 == addr_len)
            {
                /* Populate the information for sending the packet to Peer GTPU Entity */
                relayConfigInfo_g.gtpuToWTAddr.sin_family = AF_INET;
                relayConfigInfo_g.gtpuToWTAddr.sin_port   = htons_wrapper(EGTPU_UDP_PORT);
                relayConfigInfo_g.gtpuToWTAddr.sin_addr.s_addr
                        = htonl_wrapper(RELAY_GTPU_GET_U32BIT(&(p_rab_ctxt->p_transport_addr->entity_addr[2])));

                if (p_rab_ctxt->qos_id >= MAX_DSCP_VALUES)
                {
                    socketfd = s1uIpv4SockRx_g;
                }
                else
                {
                    socketfd = relayDscpToSockFdMap_g[p_rab_ctxt->qos_id].sockfd;
                    if (socketfd == RELAY_INVALID_FD)
                    {
                        socketfd = s1uIpv4SockRx_g;
                    }
                }

                pSockAddr   = &relayConfigInfo_g.gtpuToWTAddr;
                sockAddrLen = sizeof(relayConfigInfo_g.gtpuToWTAddr);
            }
            else
            {
                memSet(&sockaddr6,0,sizeof(sockaddr6));

                memCpy(sockaddr6.sin6_addr.s6_addr, &(p_rab_ctxt->p_transport_addr->entity_addr[2]), addr_len);
                sockaddr6.sin6_family = AF_INET6;
                sockaddr6.sin6_port   = htons_wrapper(EGTPU_UDP_PORT);

                if (p_rab_ctxt->qos_id >= MAX_DSCP_VALUES)
                {
                    socketfd = ipv6SockRx_g;
                }
                else
                {
                    socketfd = relayDscpToSockFdMap6_g[p_rab_ctxt->qos_id].sockfd;
                    if (socketfd == RELAY_INVALID_FD)
                    {
                        socketfd = ipv6SockRx_g;
                    }
                }

                pSockAddr   = &sockaddr6;
                sockAddrLen = sizeof(sockaddr6);
            }

            UInt8 *flatbuff = PNULL;
            flatbuff        = (UInt8 *)getMemFromPool(msgLen, PNULL);
            memSet(flatbuff,0,msgLen);
            if (PNULL == flatbuff)
            {
                ltePanic("%s: can't allocate buffer",__FUNCTION__);
                return;
            }

            if (0 == msgExtract(data_p, 0, flatbuff, msgLen))
            {
                ltePanic("%s:Failed to extract bytes",__FUNCTION__);
            }
            /*LWA:Cov:113168*/
            if (-1 == sendto_wrapper(socketfd,(void *)flatbuff, msgLen,
                                     0, pSockAddr, sockAddrLen))
            {
                LTE_LOG(LOG_INFO,"\n[ERROR] Not able to send LWA data to WT\n");
            }

            freeMemPool(flatbuff);
            /*+:LWA:Cov:113144*/
            /*flatbuff = NULL;*/
            /*-:LWA:Cov:113144*/

            /*START:LWA:DATA:FIX*/
            //msgFree(data_p);
            //data_p = NULL;
            /*END:LWA:DATA:FIX*/
        }
    }
}

/****************************************************************************
 * Function Name  : appendCsplHeaderForLwaDlUserData 
 * Inputs         : moduleId  - Source module Id,
 *                  apiId     - API Id,
 *                  totalLen  - Total length of message,
 *                  msgBuff_p - Message buffer
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion append CSPL Header for LWA DL User Data PDU.
 ****************************************************************************/
void appendCsplHeaderForLwaDLUserData(UInt16  moduleId,
                                      UInt16  apiId,
                                      UInt16  totalLen,
                                      UInt8*  msgBuff_p)
{
    UInt16 current = 0;

    if (msgBuff_p)
    {
        /* Set API version */
        msgBuff_p[current++] = GTPU_API_VERSION_NUMBER; 

        /* Set priority */
        msgBuff_p[current++] = GTPU_DEF_PRIORITY; 

        /* Set source module Id */
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],moduleId);
        current +=2;

        /* Set destination module Id */
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],GTPU_MODULE_ID);
        current +=2;

        /* Set API Id */
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],apiId);
        current +=2;

        /* Set total length */
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],totalLen);
        current +=2;

        /* Set End payload */
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],totalLen - 1);
        current +=2;

        /* Set start payload */
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],START_PAYLOAD);
        current +=2;

        /* Set end parameter list */
        RELAY_GTPU_SET_U16BIT(&msgBuff_p[current],totalLen-1);
        current +=2;
    } 
}

/****************************************************************************
 * Function Name  : handleRelayAdapterLwaDataReq 
 * Inputs         : ueIndex    - UE Index,
 *                  lcId       - Logical Channel Identifier,
 *                  payloadLen - Length of payload,
 *                  gtpuMsg_p  - Payload buffer 
 *                  hdrContent - RAN container header content
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion handle LWA Data Req from Adapter.
 ****************************************************************************/
UInt32 handleRelayAdapterLwaDataReq(UInt16  ueIndex, 
                                    UInt8   lcId,
                                    UInt16  payloadLen,
                                    UInt8*  gtpuMsg_p,
                                    UInt32  hdrContent)
{
    UInt32 current  = CSPL_HDR;
    UInt32 tunnelId = 0;
    UInt8  lcIndex  = lcId - 3;

    /* To check, whether tunnel Id is created for this UE Index and Lc-Id 
       Before accessing the actual value, it check the inUseFlag. if this
       Flag is set, then only it will access the tunnel ID. */
    if (ueIndex < RELAY_GTPU_MAX_UE && lcIndex < RELAY_GTPU_MAX_LC)
    {
        if (RELAY_GTPU_IN_USE ==
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].inUseFlag)
        {
            /* Fetch tunnel Id */
            tunnelId = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].lwaTunnelId;

            /* Set LWA tunnel Id in the CSPL header */
            gtpuMsg_p[current++] = RELAY_GTPU_IE_TEID_DATA_I;
            RELAY_GTPU_SET_U32BIT(&gtpuMsg_p[current],tunnelId);
            current += 4;

            /* Set content of RAN container extension header */
			gtpuMsg_p[current++] = EGTPU_IE_RAN_CONTAINER_CONTENT;
			RELAY_GTPU_SET_U32BIT(&gtpuMsg_p[current],hdrContent);

            /* Set current index to start of payload */
            current = START_PAYLOAD;

            /* Add CSPL header before forwarding message to GTPU Stack */
            appendCsplHeaderForLwaDLUserData(EI_RELAY_MODULE_ID, 
                                             GTPU_LWA_DATA_REQ, 
                                             current+payloadLen, 
                                             gtpuMsg_p);

            /* Call the GTPU stack API to process the received message */
            egtpu_process_msg(p_global, gtpuMsg_p);

            return RELAY_GTPU_SUCCESS;
        }

        lteWarning("Not Valid Tunnel Id exists for UE Index [%d] \
                   and lcId [%d]\n", ueIndex, lcId);

        return RELAY_GTPU_FAILURE;
    }
    else
    {
        return RELAY_GTPU_FAILURE;
    }
}

/****************************************************************************
 * Function Name  : processGtpuAdapterLwaDataReq
 * Inputs         : gtpuDataReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process LWA DATA Req from Adapter.
 ****************************************************************************/
UInt32 processGtpuAdapterLwaDataReq(UInt8*  gtpuDataReq_p,
                                    UInt16  msgLen,
                                    UInt32  hdrContent)
{
    UInt16 ueIndex    = 0;
    UInt16 current    = START_OFFSET_UL;
    UInt8  lcId       = 0;
    UInt16 payloadLen = 0;

    if (PNULL != gtpuDataReq_p)
    {
        /* Fetch UE index */
        ueIndex = RELAY_GTPU_GET_U16BIT(&gtpuDataReq_p[current]);
        current +=2;

        /* Fetch LCID */
        lcId = gtpuDataReq_p[current++];

        /* Fetch payload length */
        payloadLen = RELAY_GTPU_GET_U16BIT(&gtpuDataReq_p[current]);
        current +=2;

        /* Call handleRelayAdapterLwaDataReq function for further handling */
        if(RELAY_GTPU_SUCCESS != 
                handleRelayAdapterLwaDataReq(ueIndex,lcId,payloadLen,gtpuDataReq_p, hdrContent))
        {

            LOG_GTPU_MSG(PR_UL_DATA_NOT_SENT_TO_GTPU, LOGWARNING, PR_GTPU_UL_DATA,
                         relayGtpuCurrentTick_g, __LINE__, 
                         lcId,msgLen,0,0, 0,0, __func__,"");

            return RELAY_GTPU_FAILURE;
        }

        LOG_UT(PR_UL_DATA_SENT_TO_GTPU, LOGDEBUG, PR_GTPU_UL_DATA,
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
 * Function Name  : lwaDataInd
 * Inputs         : ueIndex    - UE Index,
 *                  lcId       - logical channel Id,
 *                  data_p     - pointer to LWA SDU data in Zero-copy
 *                               buffering format,
 *                  size       - size of data
 *                  hdrContent - DL USER DATA HDR
 * Outputs        : None
 * Returns        : None
 * Description    : The LWA data plane module has to implement this function.
 *                  It's used for passing SDU from the LWA data plane module 
 *                  to egtpu stack.
 ****************************************************************************/
void lwaDataInd(UInt16 ueIndex, UInt16 lcId, void* data_p, 
                UInt16 size, UInt32 hdrContent)
{
    UInt8* msgPtr = PNULL;
    UInt16 msgLen = 0;

    /* Ensure that we have received valid data buffer */
    if (!data_p) 
    {
        LOG_GTPU_MSG(PR_LWA_ADAPTER_EMPTY_DL_LWA_DATA, LOGWARNING, PR_GTPU_UL_DATA,
                     relayGtpuCurrentTick_g, __LINE__, 
                     ueIndex, lcId,0,0,0,0, __func__, "");
        return;
    }    

    /* Ensure that the size of data buffer is not more than max allowed size */
    if (BUFFERSIZE < size)
    {
        msgFree(data_p);

        LOG_GTPU_MSG(PR_ADAPTER_UL_DATA_GREATER_SIZE, LOGWARNING, PR_GTPU_UL_DATA,
                     relayGtpuCurrentTick_g, __LINE__, 
                     BUFFERSIZE, size,0,0,0,0, __func__, "");
        return;
    }

    /* We receive Xw-U PDU from the LWA Relay layer, so pass it to upper layer 
     * in some way */

#ifdef L2_FAST_PATH

    egtpu_rab_ctxt_t*  p_rab_ctxt = NULL;
    UInt8              index      = 0;

    /* Get memory for GTPU Header + Payload received from PDCP */
    msgLen = size + GTP_HDR_LEN_WITH_RAN_CONTAINER_EXTN_HDR;

    msgPtr = msgReserve(data_p, 0, GTP_HDR_LEN_WITH_RAN_CONTAINER_EXTN_HDR);
    if (PNULL == msgPtr)
    {
        ltePanic("%s: can't allocate buffer",__FUNCTION__);
        return;
    }

    /* Fetch RAB context using tunnel Id corresponding to UE index 
     * and LC Id passed by the caller. */
    p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(p_global, 
                     relayMapBasedOnUeIndexLcId_g[ueIndex][lcId-3].lwaTunnelId);
    if (NULL != p_rab_ctxt)
    {
        /* Set the version, protocol type and extension bit
         * Version - 1
         * Protocol Type - 1
         * Extension bit is set
         * All other fields are unset */
        msgPtr[index++] = 0x34; 

        /* Set the message type.
         * Message Type = T-PDU = 255 */
        msgPtr[index++] = 0xFF;

        /* Set the message size */
        RELAY_GTPU_SET_U16BIT(&msgPtr[index], size+GTP_MAX_HDR_LEN);
        index += 2;
        /* Set tunnel Id */
        RELAY_GTPU_SET_U32BIT(&msgPtr[index], p_rab_ctxt->teid_cell.teid_peer);
        index += 4;

        /* Set the sequence number as 0 */
        RELAY_GTPU_SET_U16BIT(&msgPtr[index], 0);
        index += 2;

        /* Set the N-PDU number as 0 */
        msgPtr[index++] = 0;

        /* Set the next extension hdr type to "RAN Container" */
        //index = 11;
        msgPtr[index++] = 0x81;

        /* Set the RAN container extension hdr length */
        msgPtr[index++] = 0x2;

        /* Set the content of RAN container extension header */
        RELAY_GTPU_SET_U32BIT(&msgPtr[index], hdrContent);
        index += 4;

        /* Set the next two bytes as spare */
        RELAY_GTPU_SET_U16BIT(&msgPtr[index],0);
        index += 2;

        /* Set the next extension hdr type as '0' */
        msgPtr[index] = 0x00; 

        /* Send the message to WT */
        sendToWT(ueIndex, lcId, data_p, msgLen);
    
        /* Freeing memory here, not freed earlier as ZCB used */
        msgFree(data_p);
        /*+:LWA:Cov:113142*/
        /*data_p = NULL;*/
        /*-:LWA:Cov:113142*/
    }

#else

    msgLen = size + 64;
    msgPtr = (UInt8 *)getMemFromPool(msgLen, PNULL);
    if (PNULL == msgPtr)
    {
        LOG_GTPU_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                     relayGtpuCurrentTick_g, __LINE__, 
                     msgLen, 0,0,0,0,0, __func__,
                     "ADAPTER" );
        return;
    }

    msgExtract(data_p, 0, msgPtr+64, size);

    /* Not freeing memory here as ZCB used */
    msgFree(data_p);

    /* Set the UE index */
    LTE_SET_U16BIT(&msgPtr[59], ueIndex);

    /* Set the LC ID */
    msgPtr[61] = lcId;

    /* Set the payload length */
    LTE_SET_U16BIT(&msgPtr[62], (msgLen-64));

    /* Process received data request */
    processGtpuAdapterLwaDataReq(msgPtr, msgLen, hdrContent);

    freeMemPool(msgPtr);

#endif
}


