/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2UplinkTransmitter.h,v 1.1.1.1 2010/02/11 04:51:22 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : this file contains the function,data structure to be 
 *                      used in lteLayer2UplinkTransmitter.c
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLayer2UplinkTransmitter.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3  2009/07/14 14:45:02  gur20439
 * rrc interface code.
 *
 * Revision 1.2  2009/06/15 13:52:57  gur20439
 * modifed for release 1.2.
 *
 * Revision 1.1  2009/03/30 10:20:28  gur19836
 * Initial Version
 *
 * Revision 1.10  2009/01/29 17:55:18  gur03939
 * Changes for message queue interface with PHY SIM
 *
 * Revision 1.9  2009/01/29 12:33:34  gur18569
 * putting shared mem code in SHM_INTF
 *
 * Revision 1.8  2009/01/29 10:02:42  gur18569
 * removed shMsgBuf struct from this file
 *
 * Revision 1.7  2009/01/28 12:16:46  gur03939
 * Changes to avoid memcpy with shared memory interfaces
 *
 * Revision 1.6  2009/01/20 07:54:00  gur19836
 * Changes done for interface with RRC Layer
 *
 * Revision 1.5  2009/01/20 07:19:32  gur19413
 * updated
 *
 * Revision 1.4  2009/01/17 20:27:27  gur11447
 * Changes for 1 msec
 *
 * Revision 1.3  2008/12/31 06:43:31  gur20052
 * changes for AllocTBSize are added
 *
 * Revision 1.2  2008/12/10 10:37:06  gur12905
 * Updated for transmitter thread creation
 *
 * Revision 1.1  2008/12/08 12:56:25  gur12905
 * Initial Version phase - 2
 *
 *
 *
 ****************************************************************************/

#ifndef UPLINK_TRANSMITTER_H
#define UPLINK_TRANSMITTER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#if 0
#include "lteLayer2PhysInterface.h"
#include "lteLayer2UplinkContainer.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define UL_RESOURCE_GRANT 0x2F

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/


/****************************************************************************
 * Exported Functions
 ****************************************************************************/
void *initUplinkTransmitter(void *arg);
void buildAndSendULOutGoingMessage(ulDispatcherToPhy *ptr_p,UInt32 type);
void writeDataToPhySHM(shDLMessageQBuf* msg_p,UInt32 len,UInt32 type);
#endif

#endif  /* INCLUDED_FILENAME_H */

