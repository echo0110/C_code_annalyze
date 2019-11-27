/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLSpsScheduler.h,v  $
 *
 ****************************************************************************
 *
 *  File Description : The file contains the definitions for SPS scheduler.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/
#ifndef _LIB_MAC_DL_SPS_PDSCH_DEF_
#define _LIB_MAC_DL_SPS_PDSCH_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacDLSpsScheduler.h"
#include "lteMacDLResourceInfoMgr.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_DL_RES_RSV_REQ_PROCESS 1

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define SPS_RESERVE 1
#define SPS_RELEASE 0


/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* Following structure defines the Sorted SPS Interval TTI Map with 
 * numDlSpsOccasionPerTti as key*/
typedef struct SpsStrIntervalTTISortedT
{
    /* Index number of SPS TTIContext */
    UInt8 index;
    /* Counter maintained by RM for traversing indexes as per least loaded TTI 
       It shall be taken as a key for quick sort of RM */
    UInt8 numSpsUeinRm;
} *LP_SpsStrIntervalTTISorted, SpsStrIntervalTTISorted;

extern SpsStrIntervalTTISorted sortedArr_g[MAX_NUM_CELL][SPS_INTERVAL_40];
extern DLResourceAllocationInfo *dlRAInfo_gp[MAX_NUM_CELL];

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : spsAllocateStatusDataReq
 * Inputs         : ueIndex - Index of UE
 *                  response - Strategy receives following type of responses 
 *                       from PDSCH or HARQ module:
 *                   1. Pdsch res reserv Success
 *                   2. Pdsch res reserv Failure (Mcs Poor)
 *                   3. Harq First Ack / NACK for Activation
 *                   4. HARQ DTX or timer expiry Information
 *                  requestType - Type of request for Reserve or release 
 *                                resources
 *                  spsOpp - Latest Opportunity provided to SPS LC
 *                  numRbs - RBs occupied
 * Outputs        : None.
 * Returns        : LP_PdcpDataReq pointer on Status request structure.
 * Description    : To allocate and init LP_MacDlSpsStatus
 ****************************************************************************/
/* + coverity 54165 */
LP_MacDlSpsStatus spsDLAllocateStatusDataReq( UInt16 ueIndex, SpsCause response,
        UInt8 requestType, UInt32 spsOpp, UInt16 numRbs);
/* - coverity 54165 */

/******************************************************************************
 * Function Name  : initSpsRAT2LocalizedInfo
 * Inputs : 1)rat2Localized_p - This holds pointer to
 *            RAT2LocalizedInfo structure
 *          2)dlRBs - This holds the downlink bandwidth in terms of RBs
 * Outputs        : None
 * Returns        : None
 * Description    : This will intialize the SPS RAT 2 Localized Information
 ******************************************************************************/
void initSpsRAT2LocalizedInfo(
        RAT2LocalizedInfo*rat2Localized_p,
        UInt32 dlRBs);

/******************************************************************************
 * Function Name  : dlSpsResourceReserveHandler
 * Inputs         : qCount - Number of nodes in a queue.
 * Outputs        : None
 * Returns        : None
 * Description    : This function will invoke SPS PDSCH for resource reserve 
 *                  request.
 ******************************************************************************/
void dlSpsResourceReserveHandler( UInt32 qCount ,InternalCellIndex internalCellIndex);

/******************************************************************************
 * Function Name  : dlSpsResourceReleaseHandler
 * Inputs         : qCount - Number of nodes in a queue.
 * Outputs        : None
 * Returns        : None
 * Description    : This function will invoke SPS PDSCH for resource release 
 *                  request.
 ******************************************************************************/
void dlSpsResourceReleaseHandler( UInt32 qCount,InternalCellIndex internalCellIndex );

/******************************************************************************
 * Function Name  : initDLSpsTTIContextMap
 * Inputs         : rmSize - rmSize to be allocated
 * Outputs        : None
 * Returns        : None
 * Description    : This function will initailize RM at the init time for SPS. 
 ******************************************************************************/
void initDLSpsTTIContextMap(UInt32 rmSize
         ,InternalCellIndex internalCellIndex);

/******************************************************************************
 * Function Name  : processResourceReleaseRequest
 * Inputs         : ueContext_p - Pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : This function will free the RM resources.
 ******************************************************************************/
void processDLResourceReleaseRequest(DLUEContext *ueContext_p,InternalCellIndex internalCellIndex);

/****************************************************************************
 * Function Name  : spsSort
 * Inputs         : sortedArray - Array to be sorted
 *                  startingIndex
 *                  lastIndex
 * Outputs        : None.
 * Returns        : None
 * Description    : Sorts the array in ascending order using insertion sort.
 ****************************************************************************/
void spsSort( SpsStrIntervalTTISorted sortedArray[],
        UInt8 startingIndex, UInt8 lastIndex,
        InternalCellIndex internalCellIndex);

#endif

