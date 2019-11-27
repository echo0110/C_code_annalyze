/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpDiscardTimer.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Definitions for Discard Timer Interface
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpDiscardTimer.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.2  2009/05/22 09:20:48  gur19836
 * ROHC Changes merged
 *
 * Revision 1.1  2009/05/20 13:22:17  gur20548
 * Changed header file name
 *
 * Revision 1.3  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef __DISCARDTIMER_H__
#define __DISCARDTIMER_H__

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"
#include "lteList.h"
#include "lteTime.h"
#include "ltePdcpTxQueues.h"

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
#define DELETE_UE_TIMER 200

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpProcessDiscardTimers  
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Processing queues with timers
  ****************************************************************************/
void pdcpProcessDiscardTimers(void);

/* SRP 3608 changes start */
/****************************************************************************
 * Function Name  : pdcpProcessPSRTimers
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Processing PSR with timer
 ****************************************************************************/
void pdcpProcessPSRTimers(void);
/* SPR 3608 changes end */

/****************************************************************************
 * Function Name  : pdcpProcessDeleteUeTimers  
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Checks if we need to delete the UE after EMI recvd from
 *                  packet relay
 ****************************************************************************/
void pdcpProcessDeleteUeTimers(void);

/* SPR 5178 changes starts here */
/* PDCP_RLC_SPLIT changes*/
/****************************************************************************
 * Function Name  : sendDataDiscardReqToRlc
 * Inputs         : ueIndex - UE Identifier of a UE
 *                    lcId - Logical Channel ID
 *                    transactionId - Transaction ID
 * Outputs        :
 * Returns        : RLC_SUCCESS or FAILURE
 * Variables      :
 * Description    : This API is used by PDCP to discard any PDU already sent
 *                      to RLC Layer. RLC Layer will delete the referenced PDU only
 *                      if any portion of it is not transmitted to MAC yet.
 *******************************************************************************/
UInt16 sendDataDiscardReqToRlc (
                UInt16 ueIndex,
                UInt8 lcId,
                void *transactionId
                );

/* SPR 5178 changes ends here */


#endif /* __DISCARDTIMER_H__ */
