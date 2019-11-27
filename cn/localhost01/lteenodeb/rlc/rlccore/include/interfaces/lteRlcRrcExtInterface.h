/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcRrcExtInterface.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:48 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations for APIs to receive 
 *                     request from RRC and send a response back.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcRrcExtInterface.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:48  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:30  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.13  2009/04/15 16:04:22  gur19140
 * rlc latency + logging related changes
 *
 * Revision 1.12  2009/03/23 08:37:17  gur19140
 * Integration changes first waves
 *
 * Revision 1.11  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.10  2009/03/05 07:56:19  gur20294
 * updated port number
 *
 * Revision 1.9  2009/03/03 11:31:58  gur20294
 * Updated for MAX_DATA_SIZE macro name to MAX_DATASIZE
 *
 * Revision 1.8  2009/03/02 05:34:08  gur19836
 * Updated for OAM Enhancement
 *
 * Revision 1.7  2009/02/28 15:30:49  gur20294
 * changed the values of READPORT and WRITEPORT
 *
 * Revision 1.6  2009/02/12 03:53:02  gur19461
 * Corrected
 *
 * Revision 1.5  2009/02/12 03:50:33  gur19461
 * Corrected value of UX to RLC_NULL
 *
 * Revision 1.4  2009/02/05 11:01:01  gur19836
 * CR comments fixed
 *
 * Revision 1.3  2009/02/02 07:23:09  gur20294
 * added function to set port number
 *
 * Revision 1.2  2009/01/30 13:23:20  gur19140
 * removing waring
 *
 * Revision 1.1  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_RRCEXT_INTERFACE_H
#define INCLUDED_RRCEXT_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteMemPool.h"
#include "lteMisc.h"
#include "lteTypes.h"
#include "lteRlcTypes.h"
#include "lteLayer2CommanTypes.h"
#include "alarm.h"

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define TXPORT_OAM RLC_TX_OAM_PORT
#define READPORT RLC_RX_OAM_RRC_PORT
#define WRITEPORT RLC_TX_RRC_PORT

#define RRCIP "127.0.0.1"

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern SInt32 readSockFD_g;
extern SInt32 writeSockFD_g;
extern SockAddr writeAddr_g;
extern SInt32 writeOAMSockFD_g;
extern SockAddr writeOAMAddr_g;

/* SPR 5178 changes starts here */
/* SPR 5178 changes ends here */

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*Coverity 97476 Fix Start*/
#define SOCKET_ERROR (-1)
/*Coverity 97476 Fix End*/
 /* read the RRC data from socket */
 /* SPR 6949 Fix Start */
#define READ_FROM_SOCKET(ui8_MsgBuf_p,sockAddr_rrcAddr,ui32_addrLen)\
  recvfrom_wrapper(readSockFD_g, (ui8_MsgBuf_p), MAX_REQ_DATASIZE,0,PNULL,0) 
/* SPR 6949 Fix End */
/* SPR 5178 changes starts here */
/* SPR 5178 changes ends here */
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
 /* This APIs to create the send and receive socket to communicate with RRC.
    This API is called at the time of initializing the RRC interface */
/* SPR 5178 changes starts here */
void setReadWritePort(UInt16 readPort, UInt16 rrcWritePort, UInt16 oamWritePort);
/* SPR 5178 changes ends here */
/* + SPR 17439 */
void createRLCSockets(void);
/* - SPR 17439 */
#endif  /* INCLUDED_RRCEXT_INTERFACE_H */

