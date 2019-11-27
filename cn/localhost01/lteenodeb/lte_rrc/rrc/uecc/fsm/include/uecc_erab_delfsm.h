/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2010 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_erab_delfsm.h,v 1.2 2010/10/13 07:03:15 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : UECC E-RAB Release Command procedure definitions
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_erab_delfsm.h,v $
 * Revision 1.2  2010/10/13 07:03:15  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.1.2.1  2010/07/21 08:33:18  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.1  2010/06/21 14:13:59  gur21231
 * E-RAB Release Command FSM
 *
 *
 * UECC - Initial E-RAB Release Command procedure moved to uecc_erab_delfsm.*
 *
 *
 ****************************************************************************/

#ifndef UECC_ERAB_DELFSM_H
#define UECC_ERAB_DELFSM_H

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


extern uecc_fsm_procedure_t uecc_fsm_ue_erb_release_procedure;
void update_erab_release_list_with_error_code(
    uecc_ue_context_t* p_ue_context,
    void*              p_error_code
);

#ifdef ENDC_ENABLED
rrc_return_t  uecc_fsm_ue_context_release_handler(
    uecc_ue_context_t*  p_ue_context
);

/****************************************************************************
 * Function Name  : rrc_check_erab_for_bearer_or_ue_release_trigger
 * Input          : uecc_ue_context_t* - pointer to UE context
 * Output         : None
 * Returns        : release_type - UE RELEASE/ERAB RELEASE
 * Description    : This function checks erab list to trigger either UE release
 *                  or ERAB release procedure
 *                  in UECC_UE_CONNECTED states.
 ****************************************************************************/
U8
rrc_check_erab_for_bearer_or_ue_release_trigger
(
    uecc_ue_context_t   *p_ue_context
);

#endif

#endif /* UECC_ERAB_DELFSM_H */

