
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2RLCInterface.h,v 1.1.1.1 2010/02/11 04:51:22 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations for the RLC Interface
 *                     with the LTE MAC Layer.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLayer2RLCInterface.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3  2009/07/14 14:45:02  gur20439
 * rrc interface code.
 *
 * Revision 1.2  2009/05/26 07:31:54  gur20439
 * modified for mac release 1.2
 *
 * Revision 1.1  2009/03/30 10:20:28  gur19836
 * Initial Version
 *
 * Revision 1.15  2009/03/23 08:32:30  gur19140
 * Integration changes first waves
 *
 * Revision 1.14  2009/03/13 08:00:01  gur19836
 * Modified for OAM and RLC Interface Changes
 *
 * Revision 1.13  2009/01/23 13:00:09  gur18569
 * changes in name for RLC interface
 *
 * Revision 1.12  2009/01/19 15:45:04  gur18569
 * name changes for RLC interfaces
 *
 * Revision 1.11  2009/01/16 14:25:33  gur18569
 * added num of lcs field in sendArqIndication
 *
 * Revision 1.10  2009/01/08 19:02:08  gur18569
 * Review comments incorporated
 *
 * Revision 1.9  2009/01/05 13:00:50  gur18569
 * added requestFailure argument in receiveRLCData
 *
 * Revision 1.8  2008/12/08 12:55:50  gur12905
 * Modified during Phase - 2 merging
 *
 * Revision 1.7  2008/12/05 13:24:59  gur18569
 * multicore changes
 *
 * Revision 1.6  2008/10/14 05:40:11  gur11912
 * Interface modified for ARQ
 *
 * Revision 1.5  2008/09/26 07:23:27  gur11912
 * UT Defect Fixed
 *
 * Revision 1.4  2008/09/19 07:15:18  gur11912
 * review comments incorporated
 *
 * Revision 1.2  2008/09/10 04:51:42  gur11974
 * Updated for RLC simulator
 *
 * Revision 1.1  2008/09/09 05:13:56  gur11974
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTELAYER2_RLC_INTERFACE_H
#define LTELAYER2_RLC_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteLayer2UEContext.h"
#include "lteLayer2UplinkUEContext.h"
#include "lteLayer2MacRlcInterface.h"
#include "lteLayer2MacPhyInterface.h"

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
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

MacRetType getRLCData (UInt16 ueId, SInt8 lcId, UInt32 size, RLCDataBuffer* rlcDataBuffer_p);


UInt32 getUELogicalChannelBO (UInt16 ueId, SInt8 lcId);


// To be used from RLC Simulator.

/*new functions for Phase-2*/
UInt8* receiveUlData(RLCUplinkData* dataToRlc,UInt16 ueIdx);
extern RNTIInfo *getRntiInfofromRNTIMap (UInt16 rnti, UInt8 cellIndex );


#endif /* Included LTELAYER2_RLC_INTERFACE_H */
