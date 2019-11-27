/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2Uplink.h,v 1.1.1.1 2010/02/11 04:51:22 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations for the Partial Uplink
 *                     logic / Receiver thread logic for the LTE MAC layer
 *                     from the Physical Layer.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLayer2Uplink.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3  2009/07/14 14:45:02  gur20439
 * rrc interface code.
 *
 * Revision 1.2  2009/06/15 13:52:33  gur20439
 * modifed for release 1.2.
 *
 * Revision 1.1  2009/03/30 10:20:28  gur19836
 * Initial Version
 *
 * Revision 1.15  2009/03/23 08:32:30  gur19140
 * Integration changes first waves
 *
 * Revision 1.14  2009/02/20 14:34:02  gur18569
 * increased BUFFLEN to 10000
 *
 * Revision 1.13  2009/01/28 12:17:22  gur03939
 * Changes to avoid memcpy with shared memory interfaces
 *
 * Revision 1.12  2009/01/27 12:29:17  gur19413
 * throughput function added
 *
 * Revision 1.11  2009/01/17 20:26:29  gur11447
 * Changes for 1 msec
 *
 * Revision 1.10  2008/12/26 15:14:34  gur12905
 * define NUM_MSG_BUFF added
 *
 * Revision 1.9  2008/12/11 14:40:42  gur12905
 * BUFFLEN is modified to 512 bytes
 *
 * Revision 1.8  2008/12/09 11:51:19  gur19836
 * Updated during linking
 *
 * Revision 1.7  2008/12/08 12:55:50  gur12905
 * Modified during Phase - 2 merging
 *
 * Revision 1.6  2008/12/05 13:25:11  gur18569
 * multicore changes
 *
 * Revision 1.5  2008/10/17 09:24:00  gur19836
 * Enhancement: cleanupMACLayer functionality added
 *
 * Revision 1.4  2008/09/26 07:22:46  gur11912
 * UT Defect Fixed
 *
 * Revision 1.3  2008/09/19 07:15:18  gur11912
 * review comments incorporated
 *
 * Revision 1.2  2008/09/09 16:10:12  gur11974
 * Updating for integration
 *
 * Revision 1.1  2008/09/09 05:13:56  gur11974
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTELAYER2_UPLINK_H
#define LTELAYER2_UPLINK_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

//#include "lteLayer2WorkerThread.h"
//#include "lteLayer2HARQProcess.h"
#include "lteLayer2PhysInterface.h"
#include "lteLayer2CommanTypes.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define RECVPORT    MAC_RX_PHY_PORT 
#define BUFFLEN 10000
#define NUM_MSG_BUFF 200
#define INVALID_BSR_INDEX 65
#define UL_FLAG 0x02
#define LAST_BYTE 14

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#if 0
/* Structure for defining the MsgType
typedef enum MsgTypeT 
{
    NACK = 0,
    ACK,
    DATA
}MsgType;*/

/* Structure for storing the Message Info */
typedef struct MesgInfoT
{
    UplinkMsgType  mesgtype;
    UInt16   ueIndex;
    SInt8    harqId;
}MesgInfo;

/* Structure for Received SDUs from eNodeB MAC */
typedef struct PduInfoT
{
    UInt8 lcId;
    UInt16 length;
} PduInfo;    

/* Structure for sockaddr_in */
//typedef struct sockaddr_in SockAddr;

/* Socket FD for receiving messages from Physical Layer */
extern SInt32 recvSockFD_g;

extern SInt32 shMsgQULId_g;



/****************************************************************************
 * Exported Functions
 ****************************************************************************/
void *initReceiverThread(void* args);
	
void decodeUplink(UInt8* buffer_p, MesgInfo* msgInfo_p);
    
MacRetType updateHARQ(MesgInfo* msgInfo_p);

void readMsgFromSocket(void);

/*Function for Phase -2 */
//ulMessage * decodeIncomingDataFromPhy(UInt8 *ptr);
//UInt8 *fillAperiodicCqiMode20Info(ulMessage *ul_msg_ptr,UInt8 *inPtr);

//UInt8 *fillAperiodicCqiMode30Info(ulMessage *ul_msg_ptr,UInt8 *inPtr);

//UInt8 *fillPeriodicCqiMode10Info(ulMessage *ul_msg_ptr,UInt8 *inPtr);

//UInt8 *fillPeriodicCqiMode20Info(ulMessage *ul_msg_ptr,UInt8 *inPtr);
void putEntryInAckNackQueue( UInt16 ueIndex, SInt8 harqProcId,
        UplinkMsgType operation);

//extern void distributeIncomingData(ulMessage *ptr);

void displayUplkThroughPut();
#endif
#endif /* Included LTELAYER2_UPLINK_H */
