/**************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 *  $Id:ltePdcpRRMInterface.c 
 *
 * ***********************************************************************
 *
 *  File Description : This file contains wrapper function to invoke IPC functions
 *                     for init, send and received. 
 *                    
 *************************************************************************/
/************************************************************************
 * Standard Library Includes
 ***********************************************************************/
/*************************************************************************
 * Project Includes
 *************************************************************************/
#include "lteMessageQIntf.h"
#include "ltePdcpErrors.h"
#include "ltePdcpRRMUtils.h"
#include "lteMisc.h"
#include "lteLog.h"
#include "ltePdcpSockTools.h"
#include "lteLayer2CommanTypes.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
#define QNAME_SIZE 20

/****************************************************************************
 * Global types
 *****************************************************************************/
MQD_T fdForSendingPdcpMsgToRRM_g;
MQD_T fdForReceivingPdcpMsgFromRRM_g;
/* CA changes start */
/*SPR 21545 Start*/
extern UInt8 instanceL2_g;
/*SPR 21545 End*/
/* CA changes end */
/*SPR 20863 FIX*/
#ifndef UE_SIM_TESTING
extern SInt32 txRRMSockFD_g;
extern SockAddr txRRM_addr_g;
#endif
/*SPR 20863 FIX*/
#ifdef MAC_AUT_TEST
UInt8 *pdcpRrmPhyBuff_g;
UInt32 pdcpRrmMsgLen_g;
extern UInt8 macAutFlag ;
#endif
/********************************************************************************
 * Function Name    : initPdcpRRMIpcInterface
 * Inputs           : None
 * Returns          : PDCP_SUCCESS or Error code
 * Description      : This function calls init function to create message queue
                      for sending and receiving message. 
 ********************************************************************************/
/* + SPR 17439 */
UInt8 initPdcpRRMIpcInterface(void)
/* - SPR 17439 */    
{   
#ifndef UE_SIM_TESTING
    /* SPR 5443 */
    char qName[QNAME_SIZE] = {0};
    /* SPR 5443 */

    snprintf_wrapper(qName,sizeof(qName),"/PdcpToRrm%d",instanceL2_g);
    /* Initializing message queue for sending */
    fdForSendingPdcpMsgToRRM_g = lteInitMessageQTx(qName);
    if (fdForSendingPdcpMsgToRRM_g < 0)
    {
/* SPR 23260 + */
        /* SPR 7385 Changes Starts */
        ltePanic("mq_open failed for Pdcp to Rrm");
        /* SPR 7385 Changes Ends */
        return PDCP_FAIL;
/* SPR 23260 - */
    }

    snprintf_wrapper(qName,sizeof(qName),"/RrmToPdcp%d",instanceL2_g);
    /* Initializing message queue for receiving */
    fdForReceivingPdcpMsgFromRRM_g = lteInitMessageQRx(qName);
    if (fdForReceivingPdcpMsgFromRRM_g < 0)
    {
        /* SPR 7385 Changes Starts */
        ltePanic("mq_open failed for Rrm to Pdcp");
        /* SPR 7385 Changes Ends */
        return PDCP_FAIL;
    }
#endif
    return PDCP_SUCCESS;
}



/********************************************************************************
 * Function Name    : sendPdcpMsgToRRM
 * Inputs           : buffer -> Message buffer
 *                    length -> length of buffer
 * Returns          : Value returned by sendUsingMessageQ
 * Description      : This function will be used to call the send function to send 
                      data to RRM
 ********************************************************************************/
SInt8 sendPdcpMsgToRRM( UInt8 *buffer, UInt16 length
        )
{
    SInt8 retVal = 0;
#ifndef UE_SIM_TESTING
#ifdef MAC_AUT_TEST
    if (macAutFlag)
    {
       /** CA Changes Start **/ 
        pdcpRrmPhyBuff_g = MEM_ALLOC_AUT(length+1);

        MEM_COPY_AUT(pdcpRrmPhyBuff_g,buffer,length);
       /** CA Changes End **/ 
        pdcpRrmMsgLen_g = length;
        return pdcpRrmMsgLen_g;
    }
    else
#endif
    {
        /*SPR 20863 +*/
        UInt8 moduleId = LTE_GET_U16BIT(buffer + 4);
        WriteToSocket(buffer, length,moduleId);
        /*SPR 20863 -*/
    }
#endif	
    return retVal;

}

/********************************************************************************
 * Function Name    : recvPdcpMsgFromRRM 
 * Inputs           : buffer -> Message buffer 
 *                    msgSize
 * Returns          : Value returned by recvUsingMessageQ 
 * Description      : This api will be used to call the send function to send 
                      data to RRM
 ********************************************************************************/
SInt16 recvPdcpMsgFromRRM(UInt8 *buffer)
{    
    SInt16 retVal = 0;
#ifndef UE_SIM_TESTING
    retVal = lteMessageQRecv(fdForReceivingPdcpMsgFromRRM_g, buffer);
#endif
    return retVal;
}
