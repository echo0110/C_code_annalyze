/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2017 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRelayX2uSap.h
 *
 ****************************************************************************
 *
 *  File Description : This file contains variable declarations for X2-U 
 ****************************************************************************/

#ifndef LTE_RELAY_X2U_SAP_H
#define LTE_RELAY_X2U_SAP_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRelayGtpuSap.h"
#include "lteTypes.h"
#include "lteRelay.h"
#include "lteList.h"
#include "lteQueue.h"
#include "lteRelayAdapter.h"
#include "lteRelayCommon.h"


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
 * Function Name  : processX2uSuspendUEEntityRequest
 * Inputs         : suspendUeEntityReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process X2U_SUSPEND_UE_ENTITY_REQ request.
 ****************************************************************************/
UInt32 processX2uSuspendUEEntityRequest(UInt8 *suspendUeEntityReq_p, UInt16 msgLen);

/****************************************************************************
 * Function Name  : processX2uLcReleaseInd
 * Inputs         : lcReleaseInd_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process X2U_LC_RELEASE_IND
 ****************************************************************************/
UInt32 processX2uLcReleaseInd(UInt8 *lcReleaseInd_p, UInt16 msgLen);
#endif
