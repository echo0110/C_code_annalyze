/******************************************************************************
*  Filename :  uecc_rcrfsm.h
*
*  DESCRIPTION   :  This file contains the function prototypes for UECC RCR FSM
*
*
*  DATE        AUTHOR      REF             REASON
*  --------    -------     ---             ----------
*  20-May-09   Shashank    DDv0.2          Initial
*
*     Copyright (C) 2009 Aricent Inc . All Rights Reserved
******************************************************************************/

#ifndef INCLUDED_UECC_RCR_FSM_H
#define INCLUDED_UECC_RCR_FSM_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/

#include "uecc_fsm_common.h"

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
extern uecc_fsm_procedure_t uecc_fsm_ue_connection_release_procedure;


extern uecc_fsm_procedure_t
    uecc_fsm_ue_connection_release_setup_failure_procedure;

extern uecc_fsm_procedure_t
    uecc_fsm_overload_ue_connection_release_setup_failure_procedure;

extern uecc_fsm_procedure_t uecc_fsm_ue_connection_release_csc_cel_del_procedure;

extern uecc_fsm_procedure_t
    uecc_fsm_ue_rrm_connection_release_procedure;

extern uecc_fsm_procedure_t
    uecc_fsm_ue_rrm_connection_release_no_mme_id_procedure;

#define RRC_UECC_UPDATE_UE_CTX_DEL_STATUS(_del_flag_) \
    _del_flag_.is_rrc_resp_expected = 0
#endif /* INCLUDED_UECC_RCS_FSM_H */
