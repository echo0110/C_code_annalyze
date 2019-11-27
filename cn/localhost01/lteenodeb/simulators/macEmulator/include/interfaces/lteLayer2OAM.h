/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2OAM.h,v 1.1.1.1.16.1 2010/09/21 15:53:23 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations for the Operation, 
 *                     Administration and Maintaince Interface of the LTE 
 *                     MAC Layer.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLayer2OAM.h,v $
 * Revision 1.1.1.1.16.1  2010/09/21 15:53:23  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2  2009/07/14 14:45:02  gur20439
 * rrc interface code.
 *
 * Revision 1.1  2009/03/30 10:20:27  gur19836
 * Initial Version
 *
 * Revision 1.8  2009/03/13 08:00:01  gur19836
 * Modified for OAM and RLC Interface Changes
 *
 * Revision 1.7  2009/01/29 11:47:33  gur03939
 * Code changed for uplink and downlink throughput
 *
 * Revision 1.6  2009/01/27 12:29:17  gur19413
 * throughput function added
 *
 * Revision 1.5  2009/01/22 11:15:52  gur19836
 * Option added for enabling/disabling stats for UL Scheduler
 *
 * Revision 1.4  2009/01/20 07:54:00  gur19836
 * Changes done for interface with RRC Layer
 *
 * Revision 1.3  2008/10/20 07:10:34  gur18569
 *  added prototype of enableStats()
 *
 * Revision 1.2  2008/09/19 07:15:18  gur11912
 * review comments incorporated
 *
 * Revision 1.1  2008/09/09 05:13:56  gur11974
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTELAYER2_OAM_H
#define LTELAYER2_OAM_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2UEContext.h"
#include "lteLayer2RRCInterface.h"

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
#define MEGA_BITS_PER_SEC (1000*1000)

/* Error codes */
#define MAC_LAYER_NOT_INITIALIZED 14
#define MAC_LAYER_ALREADY_INITIALIZED 15


/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/


void setLogLevel(UInt8 level);

void enableLog( UInt8 flag);



void displayMemPoolStats(void);


#endif /* Included LTELAYER2_OAM_H */
