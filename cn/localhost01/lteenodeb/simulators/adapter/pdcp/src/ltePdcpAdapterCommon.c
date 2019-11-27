/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpAdapterCommon.c,v 1.1.14.1 2010/09/21 15:53:12 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Common Funtions Implementation
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpAdapterCommon.c,v $
 * Revision 1.1.14.1  2010/09/21 15:53:12  gur20491
 * FAPI changes
 *
 * Revision 1.1  2010/04/15 10:33:25  gur20619
 * .
 *
 * Revision 1.1  2010/04/15 09:10:33  gur20619
 * pdcp adapter changes for moving the relay functionality
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
//#include "lteRelayCommon.h"
#include "lteRelay.h"
#include "lteRelayS1Sap.h"
#include "lteRelayGtpuSap.h"
//#include "lteRelayOamSap.h"
#include "lteTypes.h"
//#include "cspl.h"
/******************************************************************************
 * Project Includes
 *****************************************************************************/

/******************************************************************************
  Private Definitions
 *****************************************************************************/
static void appendApiHeader
(
    UInt16 moduleId,
    UInt16 apiId,
    UInt16 transId,
    UInt16 msgLen,
    UInt8 *respBuff_p
);
/******************************************************************************
  Private Types
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
    
void* udpSendToSocket(char fd, SockAddr *toAddr,char *buff_p, int len)
{
    if ( -1 ==
        sendto_wrapper(fd,&buff_p[0],len,0,toAddr,sizeof(SockAddr)))
    {
        pError("");
        printf("Not able to send UDP Packet\n");
    }
    return PNULL;
}
                                                                                                   
/****************************************************************************
 *  * Function Name  : appendApiHeader
 *   * Inputs         : moduleId,apiId,transId,msgLen,respBuff_p
 *    * Outputs        : None
 *     * Returns        : NULL.
 *      * Description    : This funtion append API Header in front of message.
 *       ****************************************************************************/

static void appendApiHeader
(
    UInt16 moduleId,
    UInt16 apiId,
    UInt16 transId,
    UInt16 msgLen,
    UInt8 *respBuff_p
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
 *        Relay so it is hard coded bolew */
    RELAY_GTPU_SET_U16BIT(&respBuff_p[startVal],apiId);
    startVal +=2;
    /* For each message, Msg Length will include whole packet length. Calling
 *        Function will take care for this. */
    RELAY_GTPU_SET_U16BIT(&respBuff_p[startVal],msgLen);
    startVal +=2;
}

/****************************************************************************
 * Function Name  : processRelaySetupSapCnf
 * Inputs         : inMsg_p,transId,totalLen
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to S1-AP.
 ****************************************************************************/

UInt32 processRelaySetupSapCnf
(
    UInt8 *inMsg_p,
    UInt16 transId
)
{
    UInt8 outMsg[MAX_API_LEN + API_HDR] = {0};
    UeContextForRsp *respMsg_p = (UeContextForRsp*)inMsg_p;
    UInt16 current = API_HDR, startVal = 0;
    if (PNULL == inMsg_p)
    {
        fprintf(stderr,"UE Context is NULL\n");
        return RELAY_GTPU_FAILURE;
    }
    /* Filling Reqd Info to out Buffer */
    RELAY_GTPU_SET_U16BIT(&outMsg[current],respMsg_p->ueIndex);
    printf("processRelaySetupSapCnf>>ueIndex is [%d]\n",respMsg_p->ueIndex);
    current += 2;
    outMsg[current++] = respMsg_p->lcCount;
    /* For each lc Id, filling the details to out Buffer */
    for (startVal = 0; startVal < respMsg_p->lcCount; startVal++)
    {
        outMsg[current++] = respMsg_p->ueContext[startVal].lcId;
        outMsg[current++] = respMsg_p->ueContext[startVal].cause;
        if (respMsg_p->ueContext[startVal].bitMask)
        {
            /* To check, whether tunnel id needs to be filled or not, by using
            first bit of bitMask if set */
            if (RELAY_TUNNEL_ID_PRESENT
                 == (respMsg_p->ueContext[startVal].bitMask & RELAY_TUNNEL_ID_PRESENT))
            {
                outMsg[current++] = RELAY_GTPU_IE_TEID_DATA_I;
                RELAY_GTPU_SET_U32BIT(&outMsg[current],respMsg_p->ueContext[startVal].tunnelId);
                current += 4;
            }
            /* To check, whether seq no needs to be filled or not, by using 
               second bit of bitMask, if set. if Seq no is present, then both 
               uplink and downlink seq no will present */
            if (RELAY_SEQ_NO_PRESENT ==
                (respMsg_p->ueContext[startVal].bitMask & RELAY_SEQ_NO_PRESENT))
            {
                 /* Appending Tag to Message for UL-SEQ No*/
                 outMsg[current++] = RELAY_GTPU_IE_SEQ_NUM;
                 RELAY_GTPU_SET_U16BIT(&outMsg[current],respMsg_p->ueContext[startVal].ulSeqNo);
                 current += 2;
                 /* Appending Tag to Message for DL-SEQ No*/
                 outMsg[current++] = RELAY_GTPU_IE_SEQ_NUM;
                 RELAY_GTPU_SET_U16BIT(&outMsg[current],respMsg_p->ueContext[startVal].dlSeqNo);
                 current += 2;
            }
        }
    }
    /* Append Header in front of whole message. transaction id sent is same
       as came during request. */
    appendApiHeader(RELAY_S1AP_MODULE_ID,RELAY_GTPU_CP_SETUP_SAP_CNF, \
        respMsg_p->tid,current,outMsg);

     /* send to S1 Module */
    //udpSendToSocket(relayConfigInfo_g.oamS1ApRxFd,&relayConfigInfo_g.s1ApToAddr,outMsg,current);
    udpSendToSocket(relayConfigInfo_g.S1ApRxFd,&relayConfigInfo_g.s1ApToAddr,outMsg,current);
    printf("processRelaySetupSapCnf>> udpSendToSocket success ");
    return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : processRelayReleaseSapCnf
 * Inputs         : respMsg_p
 * Outputs        : None
 * Returns        : NULL.
 * Description    : This funtion send confirmation to S1-AP.
 ****************************************************************************/

UInt32 processRelayReleaseSapCnf
(
    UInt8 *inMsg_p, 
    UInt16 transId
)
{
    UInt8 outMsg[MAX_API_LEN + API_HDR] = {0};
    UeContextForRsp *respMsg_p = (UeContextForRsp*)inMsg_p;
    UInt16 current = API_HDR, startVal = 0;
    UInt8 cause = 0;
    if (PNULL == inMsg_p)
    {
        fprintf(stderr,"UE Context is NULL\n");
        return RELAY_GTPU_FAILURE;
    }
    /* Filling Reqd Info to out Buffer */
    RELAY_GTPU_SET_U16BIT(&outMsg[current],respMsg_p->ueIndex);
    current += 2;
    outMsg[current++] = respMsg_p->lcCount;
    /* For each lc Id, filling the details to out Buffer */
    for (startVal = 0; startVal < respMsg_p->lcCount; startVal++)
    {
        outMsg[current++] = respMsg_p->ueContext[startVal].lcId;
        cause = outMsg[current++] = respMsg_p->ueContext[startVal].cause;
        if ((respMsg_p->ueContext[startVal].bitMask))
        {
            /* To check, whether tunnel id needs to be filled or not, by using
            first bit of bitMask if set */
            if ( cause && (RELAY_TUNNEL_ID_PRESENT
                 == (respMsg_p->ueContext[startVal].bitMask & RELAY_TUNNEL_ID_PRESENT)))
            {
                outMsg[current++] = RELAY_GTPU_IE_TEID_DATA_I;
                RELAY_GTPU_SET_U32BIT(&outMsg[current],respMsg_p->ueContext[startVal].tunnelId);
                current += 4;
            }
            /* To check, whether seq no needs to be filled or not, by using
               second bit of bitMask, if set. if Seq no is present, then both
               uplink and downlink seq no will present */
            if ((!cause) && (RELAY_SEQ_NO_PRESENT ==
                (respMsg_p->ueContext[startVal].bitMask & RELAY_SEQ_NO_PRESENT)))
            {
                 /* Appending Tag to Message for UL-SEQ No*/
                 outMsg[current++] = RELAY_GTPU_IE_SEQ_NUM;
                 RELAY_GTPU_SET_U16BIT(&outMsg[current],respMsg_p->ueContext[startVal].ulSeqNo);
                 current += 2;
                 /* Appending Tag to Message for DL-SEQ No*/
                 outMsg[current++] = RELAY_GTPU_IE_SEQ_NUM;
                 RELAY_GTPU_SET_U16BIT(&outMsg[current],respMsg_p->ueContext[startVal].dlSeqNo);
                 current += 2;
            }
        }
    }
    /* Append Header in front of whole message. transaction id sent is same
       as came during request. */
    appendApiHeader(RELAY_S1AP_MODULE_ID,RELAY_GTPU_CP_RELEASE_SAP_CNF, \
        respMsg_p->tid,current,outMsg);
     udpSendToSocket(relayConfigInfo_g.S1ApRxFd,&relayConfigInfo_g.s1ApToAddr,outMsg,current);
    /* send to RRC Module */
    return RELAY_GTPU_SUCCESS;
}
