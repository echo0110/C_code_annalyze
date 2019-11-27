/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpExecutionLegs.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : The PDCP layer Execution Legs definitions
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpExecutionLegs.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.3  2009/07/20 08:21:52  gur19479
 * Changes done for SRB handling
 *
 * Revision 1.1.2.2  2009/07/14 05:44:32  gur20435
 * PDCP Optimizations
 *
 * Revision 1.1.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
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

#ifndef _LIB_EXECUTION_LEGS_DEF_
#define _LIB_EXECUTION_LEGS_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>
#include "ltePdcpContext.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef UInt32 (*pdcpHandlerFuncT) (UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, void *context_p);

#ifndef UE_SIM_TESTING
typedef UInt32 (*pdcpSpsHandlerFuncT) (UInt16 ueIndex, UInt8 lcId,
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p, 
        LP_PdcpSpsIntervalProfile spsIntProf_p, 
        InternalCellIndex intCellIdx );
#endif

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#ifdef UE_SIM_TESTING
#ifdef UESIM_PDCP_THREAD
#define MAX_PACKET_TRANSFER_RX       120
#else
#define MAX_PACKET_TRANSFER_RX       20
#endif
#else
#define MAX_PACKET_TRANSFER_RX       20
#endif
#define MAX_FWDED_PACKET_TRANSFER_TX 100
#define MAX_PACKET_TRANSFER_TX       100

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern pdcpHandlerFuncT pdcpRxStateMachine[PDCP_RX_MAX_STATE]
                                          [PDCP_MAX_RX_EVENT];

extern pdcpHandlerFuncT pdcpTxStateMachine[PDCP_TX_MAX_STATE]
                                          [PDCP_MAX_TX_EVENT];

#ifndef UE_SIM_TESTING
extern pdcpSpsHandlerFuncT pdcpSpsRxStateMachine[PDCP_MAX_SPS_RX_STATE]
                                             [PDCP_MAX_SPS_RX_EVENT]
                                             [PDCP_MAX_SPS_PKT_TYPE];
#endif

#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
extern LTE_CIRCQUEUE pdcpRlcUeDataReqQ_g;
extern LTE_CIRCQUEUE pdcpRlcUeDataDiscardReqQ_g;
#endif

#ifdef UE_SIM_TESTING
extern LTE_CIRCQUEUE rlcRxUePendingDelQ_g;
#endif
                                          
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpElTxProcessSrbMessage 
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : It takes one message form the PDCP TX queue, finds 
 *                  corresponding PDCP entity and processes it according to 
 *                  rules which are defined for SRB entity.
  ****************************************************************************/
/* + SPR 17439 */
UInt32 pdcpElTxProcessSrbMessage(void);

/****************************************************************************
 * Function Name  : pdcpElTxProcessFwdedMessage  
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : It takes one message form the PDCP TX Fwded queue, finds 
 *                  corresponding PDCP entity and processes it according to
 *                  rules which are defined for DRB entity. This function is 
 *                  only available at target PDCP during HO
 ****************************************************************************/
UInt32 pdcpElTxProcessFwdedMessage(void);

/****************************************************************************
 * Function Name  : pdcpElTxProcessDrbMessage  
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : It takes one message form the PDCP TX queue, finds 
 *                  corresponding PDCP entity and processes it according to
 *                  rules which are defined for DRB entity.
 ****************************************************************************/
UInt32 pdcpElTxProcessDrbMessage(void);
/* - SPR 17439 */
/****************************************************************************
 * Function Name  : pdcpElTxProcessDeliveryInd  
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Process one RLC data delivery indication (PDCP PDU) message from 
                    PDCP delivery indication TX queue
  ****************************************************************************/
void pdcpElTxProcessDeliveryInd(void);

/****************************************************************************
 * Function Name  : pdcpElTxProcessDiscardTimers  
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Processing queues with timers
  ****************************************************************************/
void pdcpElTxProcessDiscardTimers(void);

/****************************************************************************
 * Function Name  : pdcpElTxProcessControlPduInd
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Process all available PDCP Control PDU indications from 
 *                  PDCP Control PDU TX queue.
 ****************************************************************************/
void pdcpElTxProcessControlPduInd(UInt64 count);

/****************************************************************************
 * Function Name  : pdcpElRxProcessMessage  
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : It takes one message form the PDCP RX queue, finds corresponding 
                    PDCP entity and processes it according to rules which are defined for
                    selected PDCP entity type.
  ****************************************************************************/
/* + SPR 17439 */
void pdcpElRxProcessMessage(void);
/* - SPR 17439 */
#ifdef PDCP_ASYNC_INTERFACE
/****************************************************************************
 * Function Name  : pdcpElRxProcessDecipheredPdu
 * Inputs         : count - number of nodes in deciphered queue
 * Outputs        : None
 * Returns        : None
 * Description    : Process all available PDCP Data PDUs that are deciphered 
 *                  by security engine but decompression yet to be done. This
 *                  EL will decipher the PDUs and form SDUs and will send it
 *                  to upper layer (packet relay)
 ****************************************************************************/
void pdcpElRxProcessDecipheredPdu(UInt64 count);

/****************************************************************************
 * Function Name  : pdcpElPollSecurityEngine
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This EL will poll the security engine to retrive back the 
 *                  processed packets by security engine.
 ****************************************************************************/
void pdcpElPollSecurityEngine( void );
#endif

/* + SPR 19066 */
/****************************************************************************
 * Function Name  : pdcpElAuditSendInactiveUE
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This EL will audit for inactive UEs in all the connected 
 *                  UEs in the system.
 ****************************************************************************/
void pdcpElAuditSendInactiveUE( InternalCellIndex cellIndex );
/* - SPR 19066 */
#endif /* _LIB_EXECUTION_LEGS_DEF_ */
