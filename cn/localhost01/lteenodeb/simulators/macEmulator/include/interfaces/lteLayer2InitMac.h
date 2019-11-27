/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2InitMac.h,v 1.1.1.1.16.1 2010/09/21 15:53:22 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations for the LTE MAC Layer
 *                     Initialization.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLayer2InitMac.h,v $
 * Revision 1.1.1.1.16.1  2010/09/21 15:53:22  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.5  2009/09/03 10:23:19  gur20439
 * default port in file modified
 *
 * Revision 1.4  2009/07/14 14:45:02  gur20439
 * rrc interface code.
 *
 * Revision 1.3  2009/06/15 13:51:47  gur20439
 * modifed for release 1.2.
 *
 * Revision 1.2  2009/06/01 10:52:27  gur20439
 * changes for mac 1.2.
 *
 * Revision 1.1  2009/03/30 10:20:27  gur19836
 * Initial Version
 *
 * Revision 1.13  2009/03/23 08:32:30  gur19140
 * Integration changes first waves
 *
 * Revision 1.12  2009/03/13 08:00:01  gur19836
 * Modified for OAM and RLC Interface Changes
 *
 * Revision 1.11  2009/01/31 01:20:33  gur20052
 * Added the default value of NUM_DEMUX_THREAD to 1
 *
 * Revision 1.10  2009/01/29 17:54:44  gur03939
 * Modified name of strcuture shULGrantMessageQBuf
 *
 * Revision 1.9  2009/01/29 17:47:12  gur03939
 * Changes for different message queue strcutures for UL Data nd UL Grant
 *
 * Revision 1.8  2009/01/29 10:04:10  gur18569
 * changed SH_MESSAGE_BUF_SIZE to SIZE_OF_UL_GRANT_TO_PHY + 4
 *
 * Revision 1.7  2009/01/28 12:29:59  gur03939
 * reverting back the port changes
 *
 * Revision 1.6  2009/01/28 12:17:51  gur03939
 * Changes to avoid memcpy with shared memory interfaces
 *
 * Revision 1.5  2009/01/27 20:47:52  gur18569
 * making default demux threads to 2
 *
 * Revision 1.4  2009/01/26 14:47:04  gur19413
 * updated
 *
 * Revision 1.3  2009/01/25 20:23:22  gur19413
 * updated
 *
 * Revision 1.2  2009/01/24 10:18:59  gur11912
 * Regression bugs fixed
 *
 * Revision 1.1  2009/01/20 07:50:27  gur19836
 * Initial Version
 *
 *
 ****************************************************************************/

#ifndef LTELAYER2_INIT_MAC_H
#define LTELAYER2_INIT_MAC_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteLayer2Types.h"
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

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern SInt32 rxSockFD_g;
extern SInt32 txSockFD_g[MAX_NUM_CELLS];
extern SockAddr tx_addr_g;


#define HOSTNAME_MAX_LEN 64
/* variable for IP Address */
UChar8 l2ModuleIP[HOSTNAME_MAX_LEN];
//ipv6 change
UChar8 dlTgIP[HOSTNAME_MAX_LEN];
UChar8 oamModuleIP_g[HOSTNAME_MAX_LEN];
UChar8 rrcModuleIP_g[HOSTNAME_MAX_LEN];

/* TA_CHG */
#define TA_CONF_FILE_NAME "../cfg/lteLayer2TAConfig.cfg"
#define MAX_TA_VALUES_SUPPORTED      10

typedef struct TAValuesT
{
    UInt16   taRach;
    UInt16   taUlsch[MAX_TA_VALUES_SUPPORTED]; 
    UInt16   taCqi[MAX_TA_VALUES_SUPPORTED]; 
    UInt16   taSrs[MAX_TA_VALUES_SUPPORTED]; /* SRS_TA */
    UInt8    numTaUlSch;
    UInt8    numTaCqi;
    UInt8    numTaSrs; /* SRS_TA */
}TAValues;
/* TA_CHG */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

MacRetType initMACLayer (UInt8 cellIndex);

void cleanupMACLayer(void);
#endif /* Included LTELAYER2_INIT_MAC_H */
