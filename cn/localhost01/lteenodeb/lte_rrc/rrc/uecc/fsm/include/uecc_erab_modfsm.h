/*******************************************************************************  
*  FILE :  uecc_erab_modfsm.h
*
*  DESCRIPTION   :  This file contains the function prototypes for UECC E-RAB 
*                   MODIFY FSM
*
*
*  DATE        AUTHOR      REF             REASON
*  --------    -------     ---             ----------
*  28-May-10   TSinha                      Initial
*
*     Copyright (C) 2009 Aricent Inc . All Rights Reserved
******************************************************************************/

#ifndef INCLUDED_UECC_ERAB_MODFSM_H
#define INCLUDED_UECC_ERAB_MODFSM_H

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
extern uecc_fsm_procedure_t uecc_fsm_erb_modify_procedure;

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_and_send_erbmod_delete_lc_req
 *
 *   INPUT        : uecc_ue_context_t* p_uecc_ue_context
 *
 *   OUTPUT       : None
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 *   DESCRIPTION  :Send internal message UECC_LLIM_DELETE_LC_REQ to LLIM
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_and_send_erbmod_delete_lc_req
(
    uecc_ue_context_t* p_uecc_ue_context
);
#endif /* INCLUDED_UECC_ERAB_MODFSM_H */

