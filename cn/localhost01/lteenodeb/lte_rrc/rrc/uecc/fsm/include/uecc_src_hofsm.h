/****************************************************************************
  *
  *  ARICENT -
  *
  *  Copyright (c) 2010 Aricent.
  *
  ****************************************************************************
  * File Details
  * ------------
  *  $Id: uecc_src_hofsm.h,v 1.2 2010/10/13 07:05:31 gur18569 Exp $
  ****************************************************************************
  *
  *  File Description : This file contains function prototypes for UECC SRC HO 
                        FSM 
  *
  ****************************************************************************
  *
  * Revision Details
  * ----------------
  *
  * $Log: uecc_src_hofsm.h,v $
  * Revision 1.2  2010/10/13 07:05:31  gur18569
  * Merge from LTE_L3_Rel_2_0_2 tag tag
  *
  * Revision 1.1.2.2  2010/07/21 11:54:43  gur18569
  * incorporated code comments
  *
  * Revision 1.1.2.1  2010/06/28 12:47:06  gur16768
  * Added uecc_src_hofsm.h
  *
  *
  *
  ****************************************************************************/

#ifndef INCLUDED_UECC_SRC_HO_FSM_H
#define INCLUDED_UECC_SRC_HO_FSM_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/
  
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Enumeration Definitions
 ****************************************************************************/
#define MAX_NUM_TARGET_CELL             32

enum UECC_LLIM_FAILURE_RESPONSE_CODE
{
    RE_EST_TRIGGERED = 0,
    RADIO_LINK_FAILURE,
    INVALID_S1AP_HO_CMD_RECEIVED,
    HO_PREP_FAIL_RECEIVED,
    HO_CMD_ONGOING_OVERALL_TIMER_EXPIRY,
    ENB_STATUS_TRANSFER_ONGOING_OVERALL_TIMER_EXPIRY,
    ENB_STATUS_RESPONSE_FAILURE,
    S1U_RECONFIGURE_FOR_FWD_FAILURE,
    WAIT_FOR_SRB_DATA_STATUS_IND_OVERALL_TIMER_EXPIRY,
    CSC_DELETE_UE_ENTITY_REQ,
    S1AP_RESET_REQ_RECEIVED
};   
/****************************************************************************
 * Public Variables
 ****************************************************************************/
extern uecc_fsm_procedure_t uecc_fsm_source_handover_procedure;
/*SPR_17348_START*/
/*SPR_19066_START*/
/*Code Deleted*/
/*SPR_19066_END*/
/*SPR_17348_END*/

/* SPR_7888_fix */
rrc_return_t convert_rrm_cause_to_s1ap_cause(rrm_cause_t *p_rrmCause, s1ap_Cause *p_cause);
/* SPR_7888_fix */
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/*SPR_17284_START*/
void map_x2ap_cause_to_s1ap_cause(x2ap_Cause x2apCause, s1ap_Cause *p_cause);
/*SPR_17284_END*/
/*SPR_17348_START*/
void uecc_fsm_src_ho_release_ho_info(
        uecc_ue_context_t*  p_ue_context
        );
/*SPR_17348_END*/


#endif  /* INCLUDED_UECC_SRC_HO_FSM_H */
