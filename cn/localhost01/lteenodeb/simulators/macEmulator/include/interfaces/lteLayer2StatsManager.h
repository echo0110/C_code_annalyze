/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2StatsManager.h,v 1.1.1.1 2010/02/11 04:51:22 cm_intel Exp $
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
 * $Log: lteLayer2StatsManager.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2  2009/07/14 14:45:02  gur20439
 * rrc interface code.
 *
 * Revision 1.1  2009/03/30 10:20:28  gur19836
 * Initial Version
 *
 * Revision 1.9  2009/02/17 06:46:10  gur18569
 * added mcsIndex field for stats
 *
 * Revision 1.8  2009/01/29 13:26:41  gur20052
 * Added Changed related to the Stats MAnager of Uplink
 *
 * Revision 1.7  2009/01/22 11:16:13  gur19836
 * Option added for enabling/disabling stats for UL Scheduler
 *
 * Revision 1.6  2009/01/15 05:09:58  gur20052
 * Added support for the UL Stats Manager
 *
 * Revision 1.5  2009/01/09 05:44:01  gur18569
 * added rbBitMap field in DL stats
 *
 * Revision 1.4  2008/12/19 12:59:36  gur20052
 * Changes Incorporated for the UL Stats
 *
 * Revision 1.3  2008/12/05 13:24:37  gur18569
 * multicore changes
 *
 * Revision 1.2  2008/11/13 11:18:13  gur11912
 * Updated for Performance
 *
 * Revision 1.1  2008/10/20 07:00:44  gur18569
 * initial version
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_STATS_MANAGER_H
#define INCLUDED_STATS_MANAGER_H
#if 0
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2Types.h"
#include "lteLayer2TBSize.h"
#include "lteLayer2UEContext.h"
#include "lteLayer2UplinkUEContext.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_STATS_TICKS 1000
#define MAX_STATS_TICKS_ARRAYS 4


/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct UePRBTTIInfoT
{
    UInt32 lastScheduledTick;        
    UInt32 currentTick;          
    UInt32 rbBitMap;          
    UInt32 ueQueueLoad;            
    UInt16 ueId;                   
    UInt8  numOfRb;
    UInt8  modulationScheme;       
    UInt8  uePriority;   
    UInt8  typeOfAllocation; 
}UePRBTTIInfo;

typedef struct StatsInfoT
{
    UInt8 currentRBIndex;
    UePRBTTIInfo rbAllocPerTTI[MAX_RESOURCE_BLOCK];
}StatsInfo;

typedef struct statsFileQueueNodeT
{
    LTE_SNODE statsFileQueueNodeAnchor;
    UInt8     statsArrIndex;
    UInt32     statsTickIndex;
}statsFileQueueNode;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern LTE_SQUEUE         statFileQueue_g;
extern LTE_SQUEUE         statULFileQueue_g;
extern LTE_SEM               statsFileSem_g;
extern StatsInfo          statsInfoArr[MAX_STATS_TICKS_ARRAYS][MAX_STATS_TICKS];
extern UInt8              enableStats_g;
extern UInt64  SMThID_g;
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
UInt8 fillStatsInfo(DLUEContext* ueContext_p,UInt8 numOfRb,UInt32 rbBitMap
                ,SInt8 mcsIndex);
UInt8 fillULStatsInfo(ueContextForUplink* ueContext_p,UInt8 numOfRb,
      UInt32 aggregatedBSRval,UInt8 tempTypeOfAllocation);
void *statsManagerThread(void* param);
void resetStatsInfo(UInt8 flag, UInt8 fromScheduler);
void incrementStatsTickIndex();
void incrementULStatsTickIndex();
#endif
#endif  /* INCLUDED_STATS_MANAGER_H */
