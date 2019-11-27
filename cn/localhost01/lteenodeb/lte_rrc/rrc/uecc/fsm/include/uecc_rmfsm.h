/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_rmfsm.h,v 1.5 2010/10/13 07:03:22 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : UECC RM FSM declarations
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
  * $Log: uecc_rmfsm.h,v $
  * Revision 1.5  2010/10/13 07:03:22  gur18569
  * Merge from LTE_L3_Rel_2_0_2 tag tag
  *
  * Revision 1.2.10.2  2010/08/05 08:05:54  gur15697
  * merging round 2
  *
  * Revision 1.2.10.1  2010/07/21 08:32:16  gur15697
  * HO ERAB code Merging changes
  *
  * Revision 1.2  2010/01/04 16:10:05  ukr15916
  * no message
  *
  * Revision 1.1.2.19  2009/11/26 18:33:48  ukr16018
  * Merge S1AP and RRC (from branch dev_rel_1_0).
  *
  * Revision 1.1.2.17.2.6  2009/11/24 11:32:34  ukr15916
  * updated for dev_rel_1_0
  *
  * Revision 1.1.2.17.2.5.2.2  2009/11/23 16:43:50  ukr15916
  * beautification
  *
  * Revision 1.1.2.17.2.5.2.1  2009/11/20 19:14:40  ukr16032
  * UECC ICS failure processing updated
  *
  * Revision 1.1.2.17.2.5  2009/11/13 13:14:05  ukr18880
  * comments ordering
  *
  * Revision 1.1.2.17.2.4  2009/11/13 12:36:55  ukr18880
  * *** empty log message ***
  *
  * Revision 1.1.2.17.2.3  2009/11/13 12:23:05  ukr18880
  * comments ordering
  *
  * Revision 1.1.2.17.2.2  2009/11/12 14:36:33  ukr16032
  * UECC - Initial UE context setup procedure moved to uecc_icsp.*
  *
 *
 ****************************************************************************/

#ifndef INCLUDED_UECC_RM_FSM_H
#define INCLUDED_UECC_RM_FSM_H

/****************************************************************************
 * Header File Includes
 ****************************************************************************/

#include "uecc_fsm_common.h"
#include "uecc_x2ap.h"
#include "rrc_uecc_llim_intf.h"
/****************************************************************************
 * External data types
 ****************************************************************************/
struct _uecc_ue_context_t;
struct _rrc_uecc_llim_add_lc_resp_t;
struct rrc_rrm_erb_setup_resp_t;
struct _rrc_rrm_erb_setup_resp_t;
/****************************************************************************
 * Private Variables
 ****************************************************************************/


/****************************************************************************
 * Public functions definitions
 ****************************************************************************/
void uecc_fsm_rm_fail_action_mark_all_erab_failed(
    struct _uecc_ue_context_t*  p_ue_context,
    U32                 error_code
);

rrc_return_t uecc_fsm_rm_process_llim_add_lc_res(
    struct _uecc_ue_context_t*  p_ue_context,
    struct _rrc_uecc_llim_add_lc_resp_t*    p_rrc_uecc_llim_add_lc_resp
);

rrc_return_t uecc_fsm_rm_process_rrm_erab_radio_res_dedicated(
    struct _uecc_ue_context_t*  p_ue_context,
    struct _rrc_rrm_erb_setup_resp_t*   p_rrc_rrm_erb_setup_resp   
);

void uecc_fsm_rm_erab_setup_deallocate_erab(
    struct _uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    U8                  erab_id                 /* E_RAB ID */
);

void uecc_fill_response_code
(
    U32      *p_response_code,
    U16       module_id,
    U16       response
);

/****************************************************************************
 * Exported Macro definitions
 ****************************************************************************/


#define RRC_UECC_ERB_SETUP_UPDATE_ERROR_CODE(_err_code_)  \
    _err_code_|=(RRC_MODULE_ID << 16)


/****************************************************************************
 * Enumeration Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/

extern uecc_fsm_activity_t uecc_fsm_rrc_connection_reconfiguration_activity;

#endif


