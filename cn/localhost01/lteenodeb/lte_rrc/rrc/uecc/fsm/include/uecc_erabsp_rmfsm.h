/**************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************/
#ifndef INCLUDED_UECC_ERABSP_RM_FSM_H
#define INCLUDED_UECC_ERABSP_RM_FSM_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/

#include "uecc_rmfsm.h"
#include "s1ap_asn_enc_dec_3gpp.h"

/****************************************************************************
 * External data types
 ****************************************************************************/
struct _uecc_ue_context_t;

/****************************************************************************
 * Public functions definitions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_build_fail_list_with_radio_cause
 * Inputs         : s1ap_E_RAB_ID e_rab_ID - ERAB ID
 *                : s1ap_CauseRadioNetwork radioCause - Radio Network Cause
 * Outputs        : None
 * Returns        : None
 * Description    : Put all erabs to fail in failed list
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_build_fail_list_with_radio_cause(
    struct _uecc_ue_context_t* p_ue_context,    /* UECC UE context */
    s1ap_E_RAB_ID e_rab_ID,                     /* ERAB ID */
    s1ap_CauseRadioNetwork radioCause           /* Radio Network Cause */
);

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_erbsp_delete_lc_req
*
*   DESCRIPTION:
*       Build internal message UECC_LLIM_DELETE_LC_REQ
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_build_erbsp_delete_lc_req(
    struct _uecc_ue_context_t*             p_uecc_ue_context,
    rrc_uecc_llim_delete_lc_req_t* p_rrc_uecc_llim_erbsp_delete_lc_req
);

/* SPR 13448 Fix Start */
/****************************************************************************
 * Function Name  : uecc_fsm_rm_fail_action_mark_all_erab_fail_with_rrm_cause
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrm_cause_t*       - pointer to RRM Cause
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion populate failed list from success list
 ****************************************************************************/
void uecc_fsm_rm_fail_action_mark_all_erab_fail_with_rrm_cause(
    struct _uecc_ue_context_t* p_ue_context,    /* UECC UE context */
    rrm_cause_t*        p_rrm_cause
);
/* SPR 13448 Fix Stop */

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Macro definitions
 ****************************************************************************/

/****************************************************************************
 * Enumeration Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

extern uecc_fsm_activity_t uecc_fsm_erab_setup_rrc_connection_reconfiguration_activity;

#endif


