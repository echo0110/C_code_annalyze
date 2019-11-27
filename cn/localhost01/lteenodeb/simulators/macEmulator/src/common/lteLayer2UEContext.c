/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2UEContext.c,v 1.1.1.1.16.1 2010/09/21 15:53:28 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file defines functions like getUEContext, 
 *                     getLogicalChannel, deleteUELogicalChannels etc that 
 *                     are required by other modules.   
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLayer2UEContext.c,v $
 * Revision 1.1.1.1.16.1  2010/09/21 15:53:28  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2  2009/07/14 14:50:03  gur20439
 * rrc interface code.
 *
 * Revision 1.1  2009/03/30 10:20:34  gur19836
 * Initial Version
 *
 * Revision 1.20  2009/03/25 07:18:41  gur20439
 * LTE_LOG macro called modified as per the new macro.
 *
 * Revision 1.19  2009/03/02 10:40:08  gur18569
 * resetting assignedRBQueueLoad value before update the Queueload
 *
 * Revision 1.18  2009/02/20 14:13:05  gur18569
 * performance changes
 *
 * Revision 1.17  2009/01/31 18:43:40  gur18569
 * moved code of ue deletion from nonzerolist from scheduler.c
 *
 * Revision 1.16  2009/01/29 22:40:49  gur18569
 * added code to reset delay whn ue is being added into non zerolists
 *
 * Revision 1.15  2009/01/10 15:02:39  gur18569
 * added logs for lc deletion
 *
 * Revision 1.14  2009/01/09 16:08:08  gur18569
 * added logs
 *
 * Revision 1.13  2009/01/08 18:56:50  gur18569
 * Review comments incorporated
 *
 * Revision 1.12  2009/01/06 12:16:39  gur11912
 * Updated Queue Load in Bits
 *
 * Revision 1.11  2009/01/05 13:06:15  gur18569
 * added function updateLCQueueLoad
 *
 * Revision 1.10  2009/01/04 14:31:35  gur18569
 *  converting qload from rlc in bits in updateUeQueueLoad
 *
 * Revision 1.9  2008/12/30 08:42:56  gur18569
 * added logs for UT
 *
 * Revision 1.8  2008/12/23 05:14:50  gur11912
 * code review comments incorporated for Phase2
 *
 * Revision 1.7  2008/12/11 14:16:43  gur11912
 * Downlink Phase 2 changes
 *
 * Revision 1.6  2008/12/05 13:46:06  gur18569
 *  multi core changes
 *
 * Revision 1.5  2008/10/22 12:30:18  gur11912
 * fixed for performance changes
 *
 * Revision 1.4  2008/10/17 09:24:00  gur19836
 * Enhancement: cleanupMACLayer functionality added
 *
 * Revision 1.3  2008/10/14 05:42:38  gur11912
 * UT Defect Fixed
 *
 * Revision 1.2  2008/09/19 12:35:18  gur11912
 * Updated for Integration
 *
 * Revision 1.1  2008/09/19 07:12:04  gur11912
 * INITIAL VERSION
 *
 * Revision 1.3  2008/09/09 07:26:39  gur11912
 * printf replaced with lteLog
 *
 * Revision 1.2  2008/09/09 05:07:44  gur11974
 * Updated for integration changes.
 *
 * Revision 1.1  2008/09/04 06:07:16  gur11912
 * Initial version of Layer 2 MAC HARQ Files. Commited by RPT.
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2UEContext.h"
/****************************************************************************
 Private Definitions
 ****************************************************************************/
/* +CA_NA */
extern UInt32 stoppedProcessing_g[MAX_NUM_CELLS];
/* -CA_NA */


/****************************************************************************
 Private Types
 ****************************************************************************/

 /****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

DLUEContext * ueDLContextArr_g[MAX_UE_SUPPORTED];

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/


/*****************************************************************************
 * Function Name  : deleteUELogicalChannels 
 * Inputs         : ueContext_p - pointer to UE Context
 *                  lcBitMask - logical channel to be deleted  
 * Outputs        : Sets fields of LogicalChannelInfo structure to invalid
 *                  values indicating Invalid logical channel.   
 * Returns        : None
 * Variables      : 
 * Description    : This function sets 'isDeleted' field of LogicalChannelInfo 
 *                  structure if corresponding LCID bitmask is set.
 *****************************************************************************/
void deleteUELogicalChannels(DLUEContext *ueContext_p, UInt32 lcBitMask)
{
    UInt8 count = 0;
    for(count = 0; count < MAX_NUMBER_OF_LOGICAL_CHANNEL; count++)
    {
        if(lcBitMask & (1 << count))
        {
            ueContext_p->logicalChannel[count + 1].logicalChannelId = -1;
            ueContext_p->logicalChannel[count + 1].queueLoad = 0;
            ueContext_p->logicalChannel[count + 1].logicalChannelPriority = 0;
            ueContext_p->logicalChannel[count + 1].lastScheduledTick = 0;

            //decrement the count of active logical channels too.
            //ueContext_p->numLogicalChannels--;
        }
    }
}
