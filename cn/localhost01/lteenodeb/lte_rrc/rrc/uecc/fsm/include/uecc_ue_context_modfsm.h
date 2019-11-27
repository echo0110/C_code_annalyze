/*******************************************************************************  
*  FILE :  uecc_ue_context_modfsm.h
*
*  DESCRIPTION   :  This file contains the function prototypes for UECC UE  
*                   CONTEXT MODIFY FSM
*
*
*  DATE        AUTHOR      REF             REASON
*  --------    -------     ---             ----------
*  16-Aug-10   Komal      -----            Initial
*
*     Copyright (C) 2009 Aricent Inc . All Rights Reserved
******************************************************************************/

#ifndef INCLUDED_UECC_UE_CONTEXT_MODFSM_H
#define INCLUDED_UECC_UE_CONTEXT_MODFSM_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/

#include "uecc_fsm_common.h"
#include "rrc_rrm_il_composer.h"

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
extern uecc_fsm_procedure_t uecc_fsm_ue_context_mod_procedure;
/*SPR_18241_START*/
extern uecc_fsm_procedure_t uecc_fsm_queued_ue_context_mod_procedure;
/*SPR_18241_END*/
/*securitykey*/
extern uecc_fsm_procedure_t uecc_fsm_ue_context_mod_after_security_rekeying_procedure;

void uecc_fsm_ue_ctx_mod_fail_action_send_rrm_cnf(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        );

void uecc_fsm_ue_ctx_mod_fail_action_send_fail_resp_to_s1ap(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
);

/*securitykey*/
#endif /* INCLUDED_UECC_UE_CONTEXT_MODFSM_H */

