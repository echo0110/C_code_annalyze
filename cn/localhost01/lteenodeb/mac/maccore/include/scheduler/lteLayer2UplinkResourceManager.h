/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2UplinkResourceManager.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description :It contains macro declarations , function declarations , 
 *			sturucture declarations
 *             		required by lteLayer2UplinkResourceManager.c 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLayer2UplinkResourceManager.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.6  2009/05/29 05:51:15  gur18569
 * removed compilation errors
 *
 * Revision 1.5  2009/02/26 06:05:25  gur10445
 *  UL Resource manager Fix
 *
 * Revision 1.4  2009/02/24 13:01:25  gur10445
 * Store free indx in RB allocation
 *
 * Revision 1.3  2009/01/15 05:12:37  gur20052
 * Review Comments of Vandana are incorporated
 *
 * Revision 1.2  2009/01/01 06:41:05  gur20052
 * Using rrcConfig_g.uplkAvailableRBs_g instead of MAX_NUM_RBS
 *
 * Revision 1.1  2008/12/08 12:54:29  gur12905
 * Initial Version phase -2
 *
 *
 *
 ****************************************************************************/
#if 0   //ysl remove unused code 2019.10.29
#ifndef _LTE_MACCORE_UPLINK_RESOURCE_MANAGER_H
#define _LTE_MACCORE_UPLINK_RESOURCE_MANAGER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2UplinkUEContext.h"
#include "lteLayer2UplinkContainer.h"
#include "lteLayer2UplinkScheduler.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define upperBoundRBNum_g 110
#define lowerBoundRBNum_g 10
#define MAX_NUM_RBS (upperBoundRBNum_g - lowerBoundRBNum_g +1)
/* (41)*(0.9))=4RBs minimum required for supporting 1 RA */
#define MAX_NUM_RB_AFTER_RA     (((rrcConfig_g.uplkAvailableRBs_g ) > (41)) ? ((rrcConfig_g.uplkAvailableRBs_g)*(0.9)):(2))
#define NUM_OF_RB_MAP   8
#define RB_DEFINETELY_REQD 0x01
#define RB_REQUIRED 0x00

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/*struct of the node for Resource manager/
  (or rar res allocation) for response to res allocation req*/



/*struct of the resource Map fpr resource allocation*/
typedef struct rbMapNodeT{
    UInt8 *rbSubMap; /*array of available RBs in a tick in UL*/
    UInt16 freeRBCounter;
    UInt16 freeRBIndx; /* keeps starting index for free locations*/
    UInt8 allocationFlag; /*RBs (in Sub_rbmap)with the same value
                            as this (Rb_free_lag_ind )are free in the
                            resource map*/
    UInt8 resetFlag; 
}rbMapNode;




/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/*CA Changes start  */
void initializeResourceMap(InternalCellIndex cellIndex);
/*CA Changes end  */
void freeResourceFromResourceManager(UInt16 length,UInt8 index ,UInt8 tti);
MacRetType getResourceFromResourceManager(UInt16 length,UInt16 tti,
        allocatedResInfo *ptr,UInt8 reqTypeFlag);
void resetRBsAtLocation(UInt8 tti,
        InternalCellIndex internalCellIndex);
UInt16 getFreeRbNumber(UInt8   tti);
MacRetType allocatNACKResourcesFromResourceManager(UInt8 startIndex,
                                                UInt8 length,UInt16 tti);

#endif /*endif of _LTE_MACCORE_UPLINK_SCHEDULER_H_*/
#endif