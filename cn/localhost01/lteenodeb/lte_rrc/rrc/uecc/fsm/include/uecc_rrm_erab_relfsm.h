/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2010 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_rrm_erab_relfsm.h,v 1.2 2010/10/13 07:03:23 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : UECC RRM E-RAB Release Request procedure definitions
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_rrm_erab_relfsm.h,v $
 * Revision 1.2  2010/10/13 07:03:23  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.1.2.1  2010/07/21 08:34:02  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.1  2010/06/21 14:14:20  gur21231
 * E-RAB Release Command FSM
 *
 *
 * UECC - Initial RRM E-RAB Release Request procedure moved to 
 *        uecc_rrm_erab_relfsm.*
 *
 ****************************************************************************/

#ifndef UECC_RRM_ERAB_RELFSM_H
#define UECC_RRM_ERAB_RELFSM_H

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


extern uecc_fsm_procedure_t uecc_fsm_ue_rrm_erb_release_procedure;

#ifdef ENDC_ENABLED
extern uecc_fsm_procedure_t uecc_fsm_ue_endc_erb_release_procedure;
#endif

/* State UECC_ERB_RELEASE_UE_RECONFIGURE_ONGOING */
void uecc_fsm_erb_release_ue_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

void uecc_fsm_erb_release_rrc_connection_reconfiguration_complete_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

void uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

void uecc_fsm_erb_release_llim_reconfigure_ue_resp_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

rrc_return_t uecc_fsm_erb_release_process_rrm_erb_release_resp_data(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

void uecc_fsm_erb_release_ue_reconfigure_ongoing_timer_expiry_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

void uecc_fsm_erb_release_ongoing_radio_link_failure_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

void uecc_fsm_erb_release_ongoing_re_establish_req_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

void uecc_fsm_erb_release_s1ap_reset_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

void uecc_fsm_erb_release_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

void
uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_re_establish_req_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

void uecc_fsm_rrm_resource_deletion_ongoing_s1ap_reset_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

void uecc_fsm_rrm_resource_deletion_ongoing_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

void uecc_fsm_rrm_resource_deletion_ongoing_radio_link_failure_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

void uecc_fsm_erb_release_rrm_resource_deletion_ongoing_re_establish_req_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

void uecc_fsm_erb_release_rrm_resource_deletion_ongoing_timer_expiry_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

void uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_timer_expiry_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);
#endif /* UECC_RRM_ERAB_RELFSM_H */

