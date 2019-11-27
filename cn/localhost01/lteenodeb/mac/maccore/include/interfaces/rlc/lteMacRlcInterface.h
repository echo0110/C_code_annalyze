/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacRlcInterface.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacRlcInterface.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.4  2009/07/31 09:02:38  gur18569
 * resolved header file conflicts
 *
 * Revision 1.12  2009/07/15 12:02:20  gur18550
 * Bug solved
 *
 * Revision 1.11  2009/07/15 06:43:41  gur18550
 * Added Temp
 *
 * Revision 1.9  2009/06/08 05:12:12  gur20470
 * updated for mac 1.2 release
 *
 * Revision 1.8  2009/06/07 09:53:23  gur20470
 * initial creation
 *
 * Revision 1.6  2009/05/21 05:23:58  gur18550
 * forTB replaced with tbIdentifier
 *
 * Revision 1.5  2009/05/20 16:18:07  gur20470
 * added update rlc sdu size
 *
 * Revision 1.4  2009/05/10 13:10:46  gur18569
 * lteMacRLCInterface.h
 *
 * Revision 1.3  2009/05/08 13:20:48  gur15697
 * Updated
 *
 * Revision 1.2  2009/05/08 08:50:48  gur19413
 * add RLCDataBuffer ARQInfo LCQueueLoad structures
 *
 * Revision 1.1  2009/05/05 11:16:17  gur19413
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_RLC_INTERFACE_H
#define LTE_MAC_RLC_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteLayer2MacRlcInterface.h"
#include "lteMacTypes.h"
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define IS_UE_DELETION_IN_PROGRESS(ueContext_p)\
            (!ueContext_p || ueContext_p->deletionInProgress) ? 1 : 0


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* IP Packet type received by MAC */
typedef enum IpPktTypeT
{
    IP_NOT_DETERMINED = 0,
    IPV4_TYPE         = 1,
    IPV6_TYPE         = 2
} IpPktType;


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* SPR 609 changes start */  
/* +- SPR 17777 */
void lteMacUpdateStatsRlcHarqFailure(RLCHarqFailureInfo* pRlcHarqFailureInfo);
void lteMacUpdateStatsRlcRx(RLCDataBuffer * pRlcBuffer); 
/* +- SPR 17777 */
/* SPR 609 changes end */  
/* 32.425 Events code changes start*/
#ifdef KPI_STATS
void lteMacUpdateKpiStatsIPThpDlOnAdd(UInt16 ueIndex, RLCDataBuffer * pRlcBuffer, UInt8 lcCount);
void lteMacUpdateKpiStatsIPThpDlOnDelete(UInt8 lcId, UInt16 ueIndex);
/* 32.425 Events code changes end*/
#endif  
#endif  /* LTE_MAC_RLC_INTERFACE_H */



