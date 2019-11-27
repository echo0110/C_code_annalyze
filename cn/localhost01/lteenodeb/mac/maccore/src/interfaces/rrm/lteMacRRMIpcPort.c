/**************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 *  $Id:lteMacRRMIpcPort.c 
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
#include "lteMacTypes.h"
#include "lteMessageQIntf.h"
#include "lteMacComPorting.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
/*#define QNAME_SIZE 20 */
/****************************************************************************
 * Global types
 *****************************************************************************/
extern UInt8 instanceL2_g;
/*SPR 21545 End*/
extern Char8 *macRRMBuff_gp;
#ifdef MAC_AUT_TEST
extern UInt8 macAutFlag ;
#endif

/* SPR 16766 ++ */
#ifdef LOG_PRINT_ENABLED
extern tickType_t getCurrentTick(void);
#endif
/* SPR 16766 -- */
/* SPR 21661 + */
/*SPR 20863 +*/
Char8 rrmIPAddress_g[HOSTNAME_MAX_LEN];
SInt16 txPortRRM_g = -1;
SInt32 txRRMSockFD_g;
SockAddr txRRM_addr_g;
/*SPR 20863 -*/

/********************************************************************************
 * Function Name    : sendMsgToRRM
 * Inputs           : buffer - Pointer to buffer to be send to RRM
 *                    length - length of buffer
 * Returns          : MAC_FAILURE or Value returned by sendUsingMessageQ
 * Description      : This function will be used to call the send function to send 
                      data to RRM.
 ********************************************************************************/
SInt32 sendMsgToRRM( UInt8 *buffer,UInt16 length
        )
{
    SInt32 retVal = 0;
#ifdef MAC_AUT_TEST
    if(macAutFlag )
    memCpy(macRRMBuff_gp,buffer,length);
    else
#endif
    retVal = sendto_wrapper(txRRMSockFD_g, buffer, length, 0,
            (const SockAddr *)&txRRM_addr_g, sizeof (txRRM_addr_g));
    return retVal;

}
