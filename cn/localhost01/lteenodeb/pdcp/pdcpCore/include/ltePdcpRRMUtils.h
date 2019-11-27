/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: ltePdcpRRMUtils.h
 *
 *************************************************************************
 *
 * File Description : This file contains the common structure/ functions
 *                    used by PDCP and RRM module.   
 *************************************************************************/

/**************************************************************************
 *  Project Includes
 **************************************************************************/
#ifndef INCLUDED_LTE_PDCP_RRM_UTILS_H
#define INCLUDED_LTE_PDCP_RRM_UTILS_H

#include "ltePdcpRRMInterface.h"

/* Parameter range check error*/
#define  PDCP_RRM_INVALID_API_LENGTH 4
#define  PDCP_RRM_OUT_OF_RANGE_DATA  5 

#define PDCP_RECV_ERROR         -1
#define PDCP_SEND_ERROR         -1
/* Maximum length of IPV4 header with extension bytes */
#define IP_HEADER_MODIFIED_LENGTH      60
#define IP_HEADER_LENGTH_WO_OPT_BYTES  20
#define CHECKSUM_BYTE                  10
#define ONE_OCTET                       8
#define TWO_OCTETS                     16

#define IPV4_VER    4
#define IPV6_VER    6
#define IPV6_HEADER_LENGTH_TILL_ECN  2
typedef enum EcnCeBitT
{
    SET_ECN_CE_BIT_IPV4_HDR = 0x03,
    SET_ECN_CE_BIT_IPV6_HDR = 0x30
}EcnCeBit;
/********************************************************************************
 * Description      : This function calls init function to create message queue
                      for sending and receiving message. 
 ********************************************************************************/
/* + SPR 17439 */
UInt8 initPdcpRRMIpcInterface(void);
/* - SPR 17439 */
/********************************************************************************
 * Description      : This function will be used to call the send function to send 
                      data to RRM
 ********************************************************************************/
SInt8 sendPdcpMsgToRRM(UInt8 *buffer,UInt16 length
        );

/********************************************************************************
 * Description      : This api will be used to call the send function to send 
                      data to RRM
 ********************************************************************************/
SInt16 recvPdcpMsgFromRRM(UInt8 *buffer);

/****************************************************************************
 * Function Name  : sendEcnDrbUeIndicationToRRM
 * Inputs         : 
 * Returns        : None
 * Description    : This API sends the Drb ue indication to RRM 
 ****************************************************************************/
void sendEcnDrbUeIndicationToRRM( RrmPdcpDrbEcnCeInd ecnCeInd);

/* SPR 21583 Code Removed */

/****************************************************************************
 * Description    : This API receives message, parse the msgId and call
 *                  the corresponding API to deal with the message.
 ****************************************************************************/
void processPdcpRRMMsg (UInt8 * buffer, SInt16 bytesRead);

#endif
