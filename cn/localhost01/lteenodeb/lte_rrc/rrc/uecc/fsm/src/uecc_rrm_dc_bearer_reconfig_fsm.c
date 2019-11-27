#ifdef ENDC_ENABLED
/******************************************************************************
*
*   FILE NAME:
*       uecc_rrm_dc_bearer_reconfig_fsm.c
*
*   DESCRIPTION:
*       UECC DC BEARER CHANGE procedure implementation.
*
*
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/


/****************************************************************************
 * Header File Includes
 ***************************************************************************/

#include "uecc_rrm_dc_bearer_reconfig_fsm.h"
#include "uecc_ue_ctx.h"
#include "uecc_common.h"
#include "uecc_llim.h"
#include "uecc_rrm.h"
#include "uecc_s1ap.h"
#include "uecc_x2ap.h"
#include "uecc_logger.h"
#include "uecc_ed.h"
#include "uecc_mainfsm.h"
#include "uecc_fsm_engine.h"
#include "uecc_defaults.h"
#include "rrc_msg_mgmt.h"
#include "rrc_phy_intf.h"
#include "rrc_s1u_intf.h"
#include "uecc_utils.h"
#include "uecc_erab_modfsm.h"
/* OPTION3X Changes Start */
#include "uecc_llim_utils.h"
#include "uecc_erab_delfsm.h"
/* OPTION3X Changes End */

/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_DC_BEARER_CHANGE_UPDATE_ERROR_CODE(err_code_)  \
    err_code_|=(RRC_MODULE_ID << 16)

#define RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(log_level, format, ...)            \
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,    \
        p_ue_context->ue_index,                                            \
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,        \
        ##__VA_ARGS__)

static void
uecc_fsm_rm_dc_bearer_change_s1u_reconfigure_delete_lc_timer_expiry
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_rm_dc_bearer_change_sgnb_addition_timer_exp_sgnb_rel_ack_rcvd
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/******************************************************************************
 *   FUNCTION NAME: rrc_validate_peer_cell_info_and_get_peer_gnb_context 
 *   INPUT        : uecc_ue_context_t*              p_ue_context
 *                  rrc_rrm_dc_bearer_change_req_t* p_dc_bearer_change_req
 *                  U8                              *peer_enodeb_id
 *   OUTPUT       : None               
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE 
 *   DESCRIPTION  : This function validate the ecgi for the target cell of peer
 *                  gNB, received in RRC_RRM_DC_BEARER_CHANGE_REQ 
 *******************************************************************************/
static 
rrc_return_et
rrc_validate_peer_cell_info_and_get_peer_gnb_context
(
    uecc_ue_context_t*                 p_ue_context,
    rrc_rrm_dc_bearer_change_req_t*    p_dc_bearer_change_req,
    U8                                 *peer_enodeb_id
);
static void
uecc_fsm_s1u_reconfig_for_dc_bearer_change_init_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/****************************************************************************
 * Function Name  : uecc_dc_bearer_change_fail_build_and_send_sgnb_release
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Sends SgNB Release to discard SgNBAdditionRequest send 
 *                  earlier.
 ****************************************************************************/
void uecc_dc_bearer_change_fail_build_and_send_sgnb_release
(
    uecc_ue_context_t*  p_ue_context
);

/**************************************************************************************
 * FUNCTION NAME : uecc_fsm_sgnb_add_failure_rrc_reestablish_request_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_RRC_RE_ESTABLISH_REQ 
 *                 event.
 *************************************************************************************/
void uecc_fsm_sgnb_add_failure_rrc_reestablish_request_handler 
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
);

static void
uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_cnf_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_sgnb_add_req_reject 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_X2AP_SGNB_ADDITION_REQ_REJECT
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_sgnb_add_req_reject(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_ongoing_rlf_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_LLIM_RADIO_LINK_FAILURE_IND
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_ongoing_rlf_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_s1ap_reset_msg_ind_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_S1AP_RESET_MSG_IND 
 *                 event.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_s1ap_reset_msg_ind_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
);

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_x2ap_reset_msg_ind_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_X2AP_RESET_MSG_IND 
 *                 event.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_x2ap_reset_msg_ind_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
);

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_after_sgnb_add_s1ap_reset_msg_ind_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_S1AP_RESET_MSG_IND 
 *                 event.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_after_sgnb_add_s1ap_reset_msg_ind_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
);

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_ue_rel_req_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_UE_RELEASE_REQ 
 *                 event.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_ue_rel_req_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
);

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_dc_bearer_change_after_sgnb_add_ongoing_ho_required_ind_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_RRM_HO_REQUIRED 
 *                 event.
 ****************************************************************************/
void uecc_fsm_dc_bearer_change_after_sgnb_add_ongoing_ho_required_ind_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
);

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_chnage_ue_reconfig_init_handler 
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void
uecc_fsm_rrm_dc_bearer_chnage_ue_reconfig_init_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/****************************************************************************
 * FUNCTION NAME : uecc_dc_bearer_change_update_ue_context
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function updates the UE Context after completion 
 *                 of DC bearer change procedure.
 ****************************************************************************/
rrc_return_t
uecc_dc_bearer_change_update_ue_context
(
    uecc_ue_context_t*    p_ue_context
);

static void
uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_ongoing_timer_expiry_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_init_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_ack_rcvd
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_timer_expiry
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_llim_s1u_reconfig_after_sgnb_add_cnf_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_llim_reconfig_timer_exp_after_second_s1u_reconfigure_cnf_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_llim_reconfig_timer_exp_after_sgnb_add_cnf_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_rm_llim_reconfig_after_second_s1u_reconfigure_init_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_llim_reconfig_resp_after_second_s1u_reconfigure_cnf_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_rrm_dc_bearer_change_p_constructor
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_rrm_dc_bearer_change_p_success_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_rrm_dc_bearer_change_p_failure_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static rrc_return_t
uecc_fsm_process_rrm_dc_bearer_change_req
(
    uecc_ue_context_t*                p_ue_context,
    rrc_rrm_dc_bearer_change_req_t*   p_msg
);

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_rrm_ue_release_req_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND 
 ****************************************************************************/
void
uecc_fsm_rrm_dc_bearer_change_rrm_ue_release_req_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
);

/****************************************************************************
 * Function Name  : uecc_fsm_llim_reconfig_for_dc_bearer_change_init_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_STATE_INIT 
 ****************************************************************************/
void
uecc_fsm_llim_reconfig_for_dc_bearer_change_init_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
);

/****************************************************************************
 * Function Name  : uecc_fsm_llim_reconfig_resp_after_sgnb_reconfig_complete_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_RECONFIGURE_UE_RESP 
 ****************************************************************************/
void
uecc_fsm_llim_reconfig_resp_after_sgnb_reconfig_complete_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
);

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_erab_release_command_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function processes the received ERAB RELEASE REQUEST
 *                 from MME. It will simply discard the received message.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_erab_release_command_handler
(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_erab_modify_req_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function processes the received ERAB MODIFY REQUEST
 *                 from MME. It will simply discard the received message.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_erab_modify_req_handler
(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_erab_release_ind_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function processes the received ERAB RELEASE IND from
 *                 RRM. It will discards the received message and send failure
 *                 as response in RRC_RRM_ERB_RELEASE_IND_CNF message and error
 *                 code for each ERAB Id set to "Interaction with other
 *                 procedure".
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_erab_release_ind_handler
(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);

/****************************************************************************
 * Function Name  : uecc_fsm_s1ap_erab_modification_cnf_after_sgnb_reconfig_complete_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_S1AP_ERAB_MODIFICATION_CNF 
 ****************************************************************************/
void
uecc_fsm_s1ap_erab_modification_cnf_after_sgnb_reconfig_complete_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
);

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_ue_reconfig_resp_handler 
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE
 ****************************************************************************/
void
uecc_fsm_rrm_dc_bearer_change_ue_reconfig_resp_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
);

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_erab_setup_req_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function processes the received ERAB SETUP REQUEST 
 *                 from MME. It will simply discards the received message.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_erab_setup_req_handler
(
        uecc_ue_context_t*  p_ue_context,   
        void*               p_api
);
  
static void
uecc_fsm_llim_reconfig_timer_exp_after_sgnb_reconfig_complete_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_rrm_dc_bearer_change_ue_reconfig_timer_exp_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_csc_cell_delete_req_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_CSC_CELL_DELETE_REQ_INT.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_csc_cell_delete_req_handler
(
        uecc_ue_context_t*  p_ue_context,  
        void*               p_api
);
 
/**************************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_rrc_reestablish_request_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_RRC_RE_ESTABLISH_REQ 
 *                 event.
 *************************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_rrc_reestablish_request_handler 
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
);

/****************************************************************************
 * Function Name  : uecc_fsm_llim_reestab_cnf_after_second_s1u_reconfigure_cnf_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*              - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP message.
 ****************************************************************************/
void
uecc_fsm_llim_reestab_cnf_after_second_s1u_reconfigure_cnf_handler
(
    uecc_ue_context_t  *p_ue_context,
    void               *p_api
);

static void
uecc_fsm_rm_dc_bearer_change_fail_action_rollback_ue_context
(
 uecc_ue_context_t*  p_ue_context,
 rrc_bool_et         p_break
 );

/****************************************************************************
* FUNCTION NAME : uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_fail_handler 
* INPUT         : uecc_ue_context_t*  p_ue_context,
*                 void*               p_api
* OUTPUT        : None
* RETURNS       : None
* DESCRIPTION   : Handles S1U_RECONFIG_RESP for UE DC bearer change procedure
*                 from LLIM
****************************************************************************/
void
uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_fail_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_x2ap_error_ind 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_X2AP_ERROR_INDICATION
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_x2ap_error_ind(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
uecc_fsm_rm_dc_bearer_change_s1u_reconfigure_delete_lc_init_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static uecc_fsm_handler_record_t
        uecc_fsm_rrm_dc_bearer_change_s1u_reconfigure_ue_ongoing_mht[] =
{
    {{ UECC_EV_STATE_INIT                              },
       uecc_fsm_s1u_reconfig_for_dc_bearer_change_init_handler                    },
    {{ UECC_EV_LLIM_S1U_RECONFIG_CNF                   },
       uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_cnf_handler            },
    {{ UECC_EV_TIMER_EXPIRY                            },
       uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_ongoing_timer_expiry_handler            },
    {{ UECC_EV_RRM_HO_REQUIRED                         },
       uecc_fsm_dc_bearer_change_after_sgnb_add_ongoing_ho_required_ind_handler         },
    {{ UECC_EV_S1AP_RESET_MSG_IND                      },
       uecc_fsm_rrm_dc_bearer_change_s1ap_reset_msg_ind_handler      },
    {{ UECC_EV_UE_RELEASE_REQ                          },
       uecc_fsm_rrm_dc_bearer_change_ue_rel_req_handler              },
    {{ UECC_EV_LLIM_RADIO_LINK_FAILURE_IND             },
       uecc_fsm_rrm_dc_bearer_change_ongoing_rlf_handler },
    {{ UECC_EV_X2AP_RESET_MSG_IND                      },
       uecc_fsm_rrm_dc_bearer_change_x2ap_reset_msg_ind_handler      },
    {{ UECC_LAST_ID                                    },  
       PNULL                                                                            }
};

static uecc_fsm_handler_record_t
    uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_mht[] = 
{
    { { UECC_EV_STATE_INIT                              },
            uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_init_handler         },
    { { UECC_EV_X2AP_SGNB_ADDITION_REQ_ACK              },
            uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_ack_rcvd             },
    { { UECC_EV_X2AP_SGNB_ADDITION_REQ_REJECT           },
            uecc_fsm_rrm_dc_bearer_change_sgnb_add_req_reject            },
    { { UECC_EV_TIMER_EXPIRY                            },
            uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_timer_expiry         },
    { { UECC_EV_RRM_HO_REQUIRED                         },
            uecc_fsm_dc_bearer_change_after_sgnb_add_ongoing_ho_required_ind_handler         },
    { { UECC_EV_S1AP_RESET_MSG_IND                      },
            uecc_fsm_rrm_dc_bearer_change_s1ap_reset_msg_ind_handler      },   
    { { UECC_EV_UE_RELEASE_REQ                          },
            uecc_fsm_rrm_dc_bearer_change_ue_rel_req_handler             },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ                    },
            uecc_fsm_sgnb_add_failure_rrc_reestablish_request_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND             },
            uecc_fsm_rrm_dc_bearer_change_ongoing_rlf_handler                 },
    { { UECC_EV_X2AP_ERROR_INDICATION                   },
            uecc_fsm_rrm_dc_bearer_change_x2ap_error_ind                 },
    { { UECC_EV_X2AP_RESET_MSG_IND                      },
            uecc_fsm_rrm_dc_bearer_change_x2ap_reset_msg_ind_handler      },
    { { UECC_LAST_ID                                    },
        PNULL                   }
};

static uecc_fsm_handler_record_t
    uecc_fsm_rm_dc_bearer_change_s1u_reconfigure_delete_lc_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                              },
            uecc_fsm_rm_dc_bearer_change_s1u_reconfigure_delete_lc_init_handler         },
    { { UECC_EV_LLIM_S1U_RECONFIG_CNF                   },
            uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_fail_handler            },
    { { UECC_EV_TIMER_EXPIRY                            },
            uecc_fsm_rm_dc_bearer_change_s1u_reconfigure_delete_lc_timer_expiry         },
    { { UECC_EV_X2AP_SGNB_RELEASE_REL_ACK              },
            uecc_fsm_rm_dc_bearer_change_sgnb_addition_timer_exp_sgnb_rel_ack_rcvd             },
    { { UECC_EV_RRM_HO_REQUIRED                         },
            uecc_fsm_dc_bearer_change_after_sgnb_add_ongoing_ho_required_ind_handler         },
    { { UECC_EV_S1AP_RESET_MSG_IND                      },
            uecc_fsm_rrm_dc_bearer_change_s1ap_reset_msg_ind_handler      },   
    { { UECC_EV_UE_RELEASE_REQ                          },
            uecc_fsm_rrm_dc_bearer_change_ue_rel_req_handler             },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ                    },
            uecc_fsm_sgnb_add_failure_rrc_reestablish_request_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND             },
            uecc_fsm_rrm_dc_bearer_change_ongoing_rlf_handler                 },
    { { UECC_EV_X2AP_ERROR_INDICATION                   },
            uecc_fsm_rrm_dc_bearer_change_x2ap_error_ind                 },
    { { UECC_EV_X2AP_RESET_MSG_IND                      },
            uecc_fsm_rrm_dc_bearer_change_x2ap_reset_msg_ind_handler      },
    { { UECC_LAST_ID                                    },
        PNULL                   }
};

static uecc_fsm_handler_record_t
        uecc_fsm_rm_llim_reconfig_after_sgnb_add_ongoing_mht[] =
{
    {{ UECC_EV_STATE_INIT                               },
       uecc_fsm_s1u_reconfig_for_dc_bearer_change_init_handler          },
    {{ UECC_EV_LLIM_S1U_RECONFIG_CNF                    },
       uecc_fsm_llim_s1u_reconfig_after_sgnb_add_cnf_handler            },
    {{ UECC_EV_TIMER_EXPIRY                             },
       uecc_fsm_llim_reconfig_timer_exp_after_sgnb_add_cnf_handler      },
    {{ UECC_EV_RRM_HO_REQUIRED                         },
       uecc_fsm_dc_bearer_change_after_sgnb_add_ongoing_ho_required_ind_handler         },
    {{ UECC_EV_S1AP_RESET_MSG_IND                      },
       uecc_fsm_rrm_dc_bearer_change_after_sgnb_add_s1ap_reset_msg_ind_handler      },
    {{ UECC_EV_UE_RELEASE_REQ                          },
       uecc_fsm_rrm_dc_bearer_change_ue_rel_req_handler              },
    {{ UECC_EV_RRC_RE_ESTABLISH_REQ                    },
       uecc_fsm_rrm_dc_bearer_change_rrc_reestablish_request_handler },
    {{ UECC_EV_LLIM_RADIO_LINK_FAILURE_IND             },
       uecc_fsm_rrm_dc_bearer_change_ongoing_rlf_handler },
    {{ UECC_EV_X2AP_RESET_MSG_IND                      },
       uecc_fsm_rrm_dc_bearer_change_x2ap_reset_msg_ind_handler      },
    {{ UECC_LAST_ID                                    },  
       PNULL                                                                            }
};

static uecc_fsm_handler_record_t
        uecc_fsm_rm_llim_reconfig_after_second_s1u_reconfigure_ongoing_mht[] =
{
    {{ UECC_EV_STATE_INIT                               },
        uecc_fsm_rm_llim_reconfig_after_second_s1u_reconfigure_init_handler          },
    {{ UECC_EV_LLIM_RECONFIGURE_UE_RESP                 },
       uecc_fsm_llim_reconfig_resp_after_second_s1u_reconfigure_cnf_handler           },
    {{ UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP                 },
       uecc_fsm_llim_reestab_cnf_after_second_s1u_reconfigure_cnf_handler           },
    {{ UECC_EV_TIMER_EXPIRY                             },
       uecc_fsm_llim_reconfig_timer_exp_after_second_s1u_reconfigure_cnf_handler      },
    {{ UECC_EV_RRM_HO_REQUIRED                         },
       uecc_fsm_dc_bearer_change_after_sgnb_add_ongoing_ho_required_ind_handler         },
    {{ UECC_EV_S1AP_RESET_MSG_IND                      },
       uecc_fsm_rrm_dc_bearer_change_after_sgnb_add_s1ap_reset_msg_ind_handler      },
    {{ UECC_EV_UE_RELEASE_REQ                          },
       uecc_fsm_rrm_dc_bearer_change_ue_rel_req_handler              },
    {{ UECC_EV_RRC_RE_ESTABLISH_REQ                    },
       uecc_fsm_rrm_dc_bearer_change_rrc_reestablish_request_handler },
    {{ UECC_EV_LLIM_RADIO_LINK_FAILURE_IND             },
       uecc_fsm_rrm_dc_bearer_change_ongoing_rlf_handler },
    {{ UECC_EV_X2AP_RESET_MSG_IND                      },
       uecc_fsm_rrm_dc_bearer_change_x2ap_reset_msg_ind_handler      },
    {{ UECC_LAST_ID                                    },  
       PNULL                                                                            }
};

static uecc_fsm_handler_record_t
        uecc_fsm_rm_rrm_dc_bearer_change_ue_reconfigure_ongoing_mht[] =
{
    {{ UECC_EV_STATE_INIT                               },
       uecc_fsm_rrm_dc_bearer_chnage_ue_reconfig_init_handler          },
    {{ UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE                    },
       uecc_fsm_rrm_dc_bearer_change_ue_reconfig_resp_handler            },
    {{ UECC_EV_TIMER_EXPIRY                             },
       uecc_fsm_rrm_dc_bearer_change_ue_reconfig_timer_exp_handler      },
    {{ UECC_EV_RRM_HO_REQUIRED                         },
       uecc_fsm_dc_bearer_change_after_sgnb_add_ongoing_ho_required_ind_handler         },
    {{ UECC_EV_S1AP_RESET_MSG_IND                      },
       uecc_fsm_rrm_dc_bearer_change_after_sgnb_add_s1ap_reset_msg_ind_handler      },
    {{ UECC_EV_UE_RELEASE_REQ                          },
       uecc_fsm_rrm_dc_bearer_change_ue_rel_req_handler              },
    {{ UECC_EV_RRC_RE_ESTABLISH_REQ                    },
       uecc_fsm_rrm_dc_bearer_change_rrc_reestablish_request_handler },
    {{ UECC_EV_LLIM_RADIO_LINK_FAILURE_IND             },
       uecc_fsm_rrm_dc_bearer_change_ongoing_rlf_handler },
    {{ UECC_EV_X2AP_RESET_MSG_IND                      },
       uecc_fsm_rrm_dc_bearer_change_x2ap_reset_msg_ind_handler      },
    {{ UECC_LAST_ID                                    },  
       PNULL                                                                            }
};

static uecc_fsm_handler_record_t
        uecc_fsm_rm_rrm_dc_bearer_change_llim_reconfigure_ongoing_mht[] =
{
    {{ UECC_EV_STATE_INIT                               },
       uecc_fsm_llim_reconfig_for_dc_bearer_change_init_handler          },
    {{ UECC_EV_TIMER_EXPIRY                             },
       uecc_fsm_llim_reconfig_timer_exp_after_sgnb_reconfig_complete_handler      },
    {{ UECC_EV_LLIM_RECONFIGURE_UE_RESP                 },
       uecc_fsm_llim_reconfig_resp_after_sgnb_reconfig_complete_handler           },
    {{ UECC_EV_S1AP_ERAB_MODIFICATION_CNF                 },
       uecc_fsm_s1ap_erab_modification_cnf_after_sgnb_reconfig_complete_handler           },
    {{ UECC_EV_RRM_HO_REQUIRED                         },
       uecc_fsm_dc_bearer_change_after_sgnb_add_ongoing_ho_required_ind_handler         },
    {{ UECC_EV_S1AP_RESET_MSG_IND                      },
       uecc_fsm_rrm_dc_bearer_change_after_sgnb_add_s1ap_reset_msg_ind_handler      },
    {{ UECC_EV_UE_RELEASE_REQ                          },
       uecc_fsm_rrm_dc_bearer_change_ue_rel_req_handler              },
    {{ UECC_EV_RRC_RE_ESTABLISH_REQ                    },
       uecc_fsm_rrm_dc_bearer_change_rrc_reestablish_request_handler },
    {{ UECC_EV_LLIM_RADIO_LINK_FAILURE_IND             },
       uecc_fsm_rrm_dc_bearer_change_ongoing_rlf_handler },
    {{ UECC_EV_X2AP_RESET_MSG_IND                      },
       uecc_fsm_rrm_dc_bearer_change_x2ap_reset_msg_ind_handler      },
    {{ UECC_LAST_ID                                    },  
       PNULL                                                                            }
};

static uecc_fsm_activity_state_t
    uecc_fsm_rm_dc_bearer_change_fail_s1u_reconfig_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_RRM_DC_BEARER_CHANGE_FAIL_S1U_RECONFIG_ONGOING",
   /* .s_id           = */  (S8*)"RRM_DC_BEARER_CHANGE_FAIL_S1U_RECONFIG",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_dc_bearer_change_s1u_reconfigure_delete_lc_ongoing_mht
};

/* LLIM s1u reconfig ongoing state */
static uecc_fsm_activity_state_t
    uecc_fsm_rrm_dc_bearer_change_s1u_reconfigure_ue_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_RRM_DC_BEARER_CHANGE_S1U_RECONFIG_ONGOING",
   /* .s_id           =  */ (S8*)"RRM_DC_BEARER_CHANGE_S1U_RECONFIG",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_rrm_dc_bearer_change_s1u_reconfigure_ue_ongoing_mht
};

static uecc_fsm_activity_state_t
    uecc_fsm_rm_llim_reconfig_after_second_s1u_reconfigure_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_RRM_DC_BEARER_CHANGE_ADD_LC_ONGOING",
   /* .s_id           =  */ (S8*)"RRM_DC_BEARER_CHANGE_ADD_LC",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_rm_llim_reconfig_after_second_s1u_reconfigure_ongoing_mht
};

static uecc_fsm_activity_state_t
    uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_X2AP_SGNB_ADDITION_ONGOING",
   /* .s_id           = */  (S8*)"SGNB_ADD",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_mht
};

static uecc_fsm_activity_state_t
uecc_fsm_rm_llim_reconfig_after_sgnb_add_ongoing = 
{
   /* .s_name         = */  (S8*)"UECC_RRM_LLIM_RECONFIG_AFTER_SGNB_ADD_ONGOING",
   /* .s_id           = */  (S8*)"DC_BEARER_CHANGE_LLIM_RECONFIG_AFTER_SGNB_ADD",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_llim_reconfig_after_sgnb_add_ongoing_mht
};

static uecc_fsm_activity_state_t
uecc_fsm_rm_dc_bearer_change_ue_reconfigure_ongoing = 
{
   /* .s_name         = */  (S8*)"UECC_RRM_DC_BEARER_CHANGE_UE_RECONFIGURE_ONGOING",
   /* .s_id           = */  (S8*)"DC_BEARER_CHANGE_UE_RECONFIG_AFTER_LLIM_REESTAB_CNF",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_rrm_dc_bearer_change_ue_reconfigure_ongoing_mht
};

static uecc_fsm_activity_state_t
uecc_fsm_rm_llim_reconfig_after_sgnb_reconfig_complete_ongoing = 
{
   /* .s_name         = */  (S8*)"UECC_RRM_DC_BEARER_CHANGE_LLIM_RECONFIGURE_ONGOING",
   /* .s_id           = */  (S8*)"DC_BEARER_CHANGE_LLIM_RECONFIG_AFTER_SGNB_RECONFIG_COMPLETE_CNF",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_rrm_dc_bearer_change_llim_reconfigure_ongoing_mht
};
/* First activity of the DC bearer change procedure activity list */
static uecc_fsm_activity_t uecc_fsm_rrm_dc_bearer_change_activity =
{
  /* .s_name             = */(S8*)"RRM DC BEARER CHANGE",
  /* .s_id               = */(S8*)"UE_RRM_DC_BEARER_CHANGE_A",
  /* .p_first_state      = */&uecc_fsm_rrm_dc_bearer_change_s1u_reconfigure_ue_ongoing
};

/* DC bearer change procedure activity list */
static uecc_fsm_activity_t* uecc_fsm_rrm_dc_bearer_change_p_activity_list[] =
{
    &uecc_fsm_rrm_dc_bearer_change_activity,
    PNULL
};

/* DC bearer change procedure mht */
static uecc_fsm_handler_record_t
uecc_fsm_rrm_dc_bearer_change_p_mht[] =
{
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
            uecc_fsm_rrm_dc_bearer_change_rrm_ue_release_req_handler       },
     { { UECC_EV_CSC_DELETE_ALL_UE_REQ        },
            uecc_fsm_rrm_dc_bearer_change_csc_cell_delete_req_handler      },
    { { UECC_EV_S1AP_ERAB_SETUP_REQ           },
            uecc_fsm_rrm_dc_bearer_change_erab_setup_req_handler           },
    { { UECC_EV_S1AP_ERAB_MODIFY_REQ          },
            uecc_fsm_rrm_dc_bearer_change_erab_modify_req_handler          },
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND     },
            uecc_fsm_rrm_dc_bearer_change_erab_release_command_handler     },
    { { UECC_EV_RRM_ERAB_RELEASE_IND          },
            uecc_fsm_rrm_dc_bearer_change_erab_release_ind_handler         },
    { { UECC_LAST_ID                          }, 
            PNULL                                                      }
};

/* DC bearer change Procedure */
uecc_fsm_procedure_t uecc_fsm_rrm_dc_bearer_change_procedure =
{
    /*.s_name             = */(S8*)"RRM DC Bearer Change",
    /*.s_id               = */(S8*)"RRM_UE_DC_BEARER_CHANGE_P",
    /*.constructor_f      = */uecc_fsm_rrm_dc_bearer_change_p_constructor,
    /*.finish_success_f   = */uecc_fsm_rrm_dc_bearer_change_p_success_handler,
    /*.finish_fail_f      = */uecc_fsm_rrm_dc_bearer_change_p_failure_handler,
    /*.activity_list      = */uecc_fsm_rrm_dc_bearer_change_p_activity_list,
    /*.p_mht              = */uecc_fsm_rrm_dc_bearer_change_p_mht
};

static void
uecc_fsm_rm_dc_bearer_change_s1u_reconfigure_delete_lc_timer_expiry
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Fill SgNB cause for RLF/Reestablishment Scenario,
     * SCGFailureInformationNR will already have the cause filled
     * at the time of Parsing the message at LLIM.
     */
    if (RRC_ONE != p_ue_context->m.sgnbCausePresent) 
    {
        if( PNULL != p_ue_context->p_curr_proc_data &&
                RRC_RLF_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
        {
            /* For RLF Scenario */
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;

            p_ue_context->m.sgnbCausePresent = RRC_ONE;
        }
        else if( PNULL != p_ue_context->p_curr_proc_data &&
                RRC_RE_ESTABLISH_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
        {
            /* For Reestablishment Scenario */
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_procedure_cancelled;

            p_ue_context->m.sgnbCausePresent = RRC_ONE;
        }
        else
        {
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;

            p_ue_context->m.sgnbCausePresent = RRC_ONE;
        }

    }

    /* Reset SgNBAddRequired Flag in UE_CTX*/
    p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    p_ue_context->x2apUecontextReleased = RRC_TRUE;
    p_ue_context->x2ap_context.menb_ue_x2ap_id = RRC_INVALID_UE_INDEX;

    /*
     * Process failure
     */
    uecc_fsm_finish_activity_failure(p_ue_context, 
            //(void*)RRM_DC_BEARER_CHANGE_FAIL_ENB_UE_REL);
            (void*)RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

static void
uecc_fsm_rm_dc_bearer_change_s1u_reconfigure_delete_lc_init_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*
     * Build and sent the S1U Reconfig request to LLIM
     */
    if ( RRC_SUCCESS != uecc_llim_build_and_send_delete_new_lc_s1u_reconfig_req(
        p_ue_context) )
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
                "FAILURE: Send UECC_LLIM_S1U_RECONFIG_REQ to LLIM");

        uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void
uecc_fsm_rm_dc_bearer_change_sgnb_addition_timer_exp_sgnb_rel_ack_rcvd
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_x2ap_sgnb_release_request_ack_t * 
        p_sgnb_release_request_ack = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_INFO,
            "Received SGNB RELEASE REQUEST ACK\n");

    p_sgnb_release_request_ack = 
        (rrc_x2ap_sgnb_release_request_ack_t *)p_api;

    if(p_sgnb_release_request_ack != NULL)
    {
        /*parse menb_ue_id and sgnb_ue_id*/
    }

    uecc_fsm_x2ap_ue_context_release_handler(p_ue_context);

    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/******************************************************************************
 *   FUNCTION NAME: rrc_validate_peer_cell_info_and_get_peer_gnb_context 
 *   INPUT        : uecc_ue_context_t*              p_ue_context
 *                  rrc_rrm_dc_bearer_change_req_t* p_dc_bearer_change_req
 *                  U8                              *peer_enodeb_id
 *   OUTPUT       : None               
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE 
 *   DESCRIPTION  : This function validate the ecgi for the target cell of peer
 *                  gNB, received in RRC_RRM_DC_BEARER_CHANGE_REQ 
 *******************************************************************************/
static 
rrc_return_et
rrc_validate_peer_cell_info_and_get_peer_gnb_context
(
    uecc_ue_context_t*                 p_ue_context,
    rrc_rrm_dc_bearer_change_req_t*    p_dc_bearer_change_req,
    U8                                 *peer_enodeb_id
)
{
    U8 o_c = RRC_NULL;

    rrc_return_et retVal = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_dc_bearer_change_req);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    for ( o_c = RRC_NULL; o_c < MAX_PEER_ENB; o_c++ )
    {
        if ( RRC_TRUE == 
                p_ue_context->p_gb_context->x2ap_ctx.x2ap_link_info[o_c].
                connection_status )
        {
            if ( !memcmp_wrapper(p_dc_bearer_change_req->sgnb_add_param.gnb_id.
                        x2_en_gnb_id.x2_gnb_id,
                        p_ue_context->p_gb_context->x2ap_ctx.x2ap_link_info[o_c].
                        peer_gb_gnb_id.x2_en_gnb_id.x2_gnb_id,
                        EN_GNB_ID_SIZE))
            {
                *peer_enodeb_id = o_c;
                retVal = RRC_SUCCESS;
            }

            if ( retVal == RRC_SUCCESS )
            {
                break;
            }
        }
    }

    if ( MAX_PEER_ENB == o_c )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_ERROR,"No NR serving cell info present in peer eNodeB Context"
                "GNB ID value received is invalid");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return retVal;

}
/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_p_constructor
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : Constructor for UE RRM DC Bearer change procedure
 *                 -Updates the UE context with information received in the 
 *                  DC BEARER CHANGE REQUEST message from RRM.
 *                 -Starts Procedural Timer
 *                 -Activates UE Activity
 *
 ****************************************************************************/
void
uecc_fsm_rrm_dc_bearer_change_p_constructor
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_rrm_dc_bearer_change_req_t*  p_msg  = RRC_PNULL;
    rrc_return_t                     result = RRC_FAILURE;
    U8                               count  = RRC_NULL;   
    dc_bearer_change_request_data_t* p_dc_bearer_change_req_data = RRC_PNULL;
    U8                               peer_enodeb_id;
    U8                               index = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_msg = (rrc_rrm_dc_bearer_change_req_t*)p_api;

#ifdef RRC_UECC_TR_ID_CONST
    uecc_ue_ctx_set_op_transction_id(p_ue_context, RRC_ONE); 

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(
            RRC_INFO,"DC Bearer Change: Transaction Id updated");
#else
    uecc_ue_ctx_set_op_transction_id(
            p_ue_context,
            uecc_ue_ctx_get_next_transction_id(p_ue_context));
#endif 

    /*
     * Allocate memory for the p_curr_proc_data for the DC bearer
     * change procedure
     */
    p_ue_context->p_curr_proc_data = (curr_proc_data_t *) rrc_mem_get(
            sizeof(curr_proc_data_t));

    if (  PNULL == p_ue_context->p_curr_proc_data )
    {
        /*
         * Memory Allocation Failed
         */
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR, 
                "[CURR_PROC_DATA] Memory Alocation failed(DC Bearer Change)");   

        /*
         * Process failure
         */
        uecc_fsm_finish_activity_failure(p_ue_context, 
                (void*)RRM_DC_BEARER_CHANGE_FAIL);
        return;
    }

    /*
     * Reset the DC Bearer Change Procedure Data
     */
    memset_wrapper(p_ue_context->p_curr_proc_data, 0x00, sizeof(curr_proc_data_t));

    do 
    {
        /*
         * Fetch pointer to DC Bearer Change request data
         */
        p_dc_bearer_change_req_data = &p_ue_context->p_curr_proc_data->u.
            rrm_dc_bearer_change_request_data;

        /*
         * Since 0 is valid erab Id, explicity set the erab Id of each 
         * erab to invalid value
         */
        for ( count = RRC_NULL; count < MAX_ERAB_COUNT; count++ )
        {    
            p_dc_bearer_change_req_data->drb_to_modify_list.
                drb_config[count].erab_id = INVALID_ERAB_ID;
        }    

        /*
         * Set current procedure to RRM Initiated DC bearer change
         */
        p_ue_context->p_curr_proc_data->t = UECC_RRM_DC_BEARER_CHANGE_PROC;

        if ( RRM_DC_BEARER_CHANGE_REQ_SGNB_ADD_PARAMS_PRESENT &
            p_msg->bitmask )
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED, 
                    "[DC_BEARER_CHANGE] SgNB addition params present ");

            if ( RRC_SGNB_ADD_PARAM_GNB_ID_PRESENT &
                    p_msg->sgnb_add_param.bitmask )
            {
                RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED, 
                        "[DC_BEARER_CHANGE] GNB ID present "); 

                if(RRC_SUCCESS == rrc_validate_peer_cell_info_and_get_peer_gnb_context(
                            p_ue_context,
                            p_msg,
                            &peer_enodeb_id))
                {
                    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED, 
                            "[DC_BEARER_CHANGE] Validated peer cell info and "
                            " peer gNB context got with peer_enodeb_id %d",peer_enodeb_id); 

                    /* Store peer_enodeb_id in X2AP UE context */
                    p_ue_context->x2ap_context.peer_enodeb_id
                        = peer_enodeb_id;
                }
                else
                {
                    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED, 
                            "[DC_BEARER_CHANGE] Peer gNodeB validation failed");   
                    result = RRC_FAILURE;
                    break;
                }
            }
            else
            {
                /* Store default SgNB peer_enodeb_id in X2AP UE context */
                for ( index = RRC_NULL; index < MAX_PEER_ENB; index++ )
                {
                    if ( X2_ENB_COMM_INFO_DEFAULT_SGNB_PRESENT &
                            p_ue_context->p_gb_context->x2ap_ctx.
                            x2ap_link_info[index].enb_comm_info.bitmask )
                    {
                        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED, 
                                "[DC_BEARER_CHANGE] Default SgNB is present ");  

                        p_ue_context->x2ap_context.peer_enodeb_id =
                            index;
                        break;
                    }
                }

                if ( MAX_PEER_ENB == index )
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,
                            RRC_DETAILED,"No NR serving cell info present in peer eNodeB Context"
                            "with deafult GNB value received ");
                    result = RRC_FAILURE;
                    break;
                }

            }
        }
        /*
         * Store the received information in curr proc data in UE Context
         */
        result = uecc_fsm_process_rrm_dc_bearer_change_req(p_ue_context, 
                p_msg);
    } 
    while(RRC_NULL);

    if ( RRC_SUCCESS != result )
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_DC_BEARER_CHANGE_REQ] Failed ," 
                "Finish procedure with generic fail cause");

        /*
         * Process failure
         */
        uecc_fsm_finish_activity_failure(p_ue_context, 
                (void*)RRM_DC_BEARER_CHANGE_FAIL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_p_failure_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   :  Failure handler for UE DC bearer change procedure
 ****************************************************************************/
void
uecc_fsm_rrm_dc_bearer_change_p_failure_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    rrm_dc_bearer_change_fail_cause_et failure_cause 
                = (rrm_dc_bearer_change_fail_cause_et) ((S32)p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*
     * Stop all the running timers
     */
    if ( PNULL !=
            p_ue_context->timers_data.timers_ids[UECC_DC_BEARER_CHANGE_GUARD_TIMER] )
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                           UECC_DC_BEARER_CHANGE_GUARD_TIMER,
                           &p_ue_context->timers_data);
    }

    if (PNULL !=
            p_ue_context->timers_data.timers_ids[UECC_DC_PREP_TIMER])
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                           UECC_DC_PREP_TIMER,
                           &p_ue_context->timers_data);
    }

    /*
     * Build and send the RRC_RRM_DC_BEARER_CHANGE_CNF msg to RRM
     */
    if ( RRC_SUCCESS != uecc_rrm_build_and_send_dc_bearer_change_cnf(
                            p_ue_context,
                            RRC_FAILURE) )
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_WARNING,
                "Failed to send RRC_RRM_DC_BEARER_CHANGE_RESP to RRM");
    }

    /*call the rollback function for DC_BEARER_CHANGE_PROC*/
    uecc_fsm_rm_dc_bearer_change_fail_action_rollback_ue_context(
            p_ue_context, RRC_TRUE);

    /*
     * Free the data for current procedure in the UE Context
     */
    if ( PNULL != p_ue_context->p_curr_proc_data )
    {
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    /*
     * Send event to main FSM based on the failure cause
     */
    if (RRM_DC_BEARER_CHANGE_FAIL_ENB_UE_REL == failure_cause || 
            RRM_DC_BEARER_CHANGE_FAIL_UE_REL == failure_cause)
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_WARNING,
                  "DC bearer change failed, triggering UE release");

        /* Send event to main FSM to trigger UE release */
        uecc_event_queue_push_event(
                UECC_EV_RRM_DC_BEARER_CHANGE_CNF_FAILURE_UE_REL,
                p_ue_context,
                PNULL);
    }
    else
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_WARNING,
                  "DC bearer change failed, finishing DC BEARER Change FSM");

        /*
         * Send event to main FSM to indicate DC bearer change 
         * complete
         */
        uecc_event_queue_push_event(
                UECC_EV_RRM_DC_BEARER_CHANGE_CNF,
                p_ue_context,
                PNULL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_parse_dc_bearer_change_drb_to_modify_list 
 * INPUT         : rrc_drb_to_modify_list_t*    p_drb_to_mod_list
 *                 rrm_drb_to_add_info_list_t*  p_src_mod_list 
 *                 uecc_ue_context_t*           p_ue_context
 * OUTPUT        : None
 * RETURNS       : rrc_return_t
 * DESCRIPTION   : This is the function processes RRC_RRM_DC_BEARER_CHANGE_REQ
 *                 event received from RRM and store the required information
 ****************************************************************************/
rrc_return_t
uecc_fsm_parse_dc_bearer_change_drb_to_modify_list
(
 rrc_drb_to_modify_list_t*      p_dst_mod_list,
 rrm_drb_to_add_info_list_t*    p_src_mod_list,
 uecc_ue_context_t*             p_ue_context 
)
{
    U8                  count           = RRC_ZERO;               
    rrc_return_t        result          = RRC_SUCCESS;
    uecc_drb_ctx_t      *p_erab_ctx     = PNULL;
    rrc_bool_et         is_scg_bearer_already_present = RRC_FALSE;

    RRC_ASSERT(PNULL != p_src_mod_list);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_dst_mod_list->drb_count = p_src_mod_list->drb_count;

    /*Firstly, check if there is any other rab previosly present in ue-context*/
    if(RRC_NULL <
            rrc_uecc_count_scg_bearer(p_ue_context))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,
                "[DC_BEARER_CHANGE_PROC] SCG Bearer is already present!");
        is_scg_bearer_already_present = RRC_TRUE;
    }

    for(count = RRC_ZERO; count < p_dst_mod_list->drb_count; count++)
    {
        /*coverity fix 277497*/
        p_erab_ctx = p_ue_context->p_e_rab_list[
            p_src_mod_list->drb_config[count].erab_id];

        /*check if the erab_id received from RRM is present in ue context or not*/
        if(!p_erab_ctx)
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(
                    RRC_ERROR, "erab_context corresponding to "
                    " erab id [%d] received in DC_BEARER_CHANGE_PROC is missing in"
                    " rrc ue context", 
                    p_src_mod_list->drb_config[count].erab_id);

            result = RRC_FAILURE;
            return result;
        }
        /*coverity fix 277497*/

        if(RRM_DRB_CONFIG_BEARED_ID_PRESENT &
                p_src_mod_list->drb_config[count].bitmask)
        {

            /*update erab_id*/
            p_dst_mod_list->drb_config[count].erab_id =
                p_src_mod_list->drb_config[count].erab_id;
        }

        /*update new drb id allocated by RRM in curr proc*/
        p_dst_mod_list->drb_config[count].new_drb_id = 
            p_src_mod_list->drb_config[count].drb_id;
        
        /*update the old drb id stored in the ue context of rrc*/
        p_dst_mod_list->drb_config[count].old_drb_id = 
            p_erab_ctx->drb_config.drb_id;

        /*update the new lc id allocated by RRM in current proc*/
        if(RRM_DRB_CONFIG_LOGICAL_CHANNEL_IDENTITY_PRESENT &
                p_src_mod_list->drb_config[count].bitmask)
        {
            p_dst_mod_list->drb_config[count].new_lc_id =
                p_src_mod_list->drb_config[count].logical_channel_identity;
        }

        /*update the old lc id stored in the ue context of rrc*/
        p_dst_mod_list->drb_config[count].old_lc_id = 
            p_erab_ctx->drb_config.logical_channel_identity;

        /*update dc_bearer_type*/
        if(RRM_DRB_CONFIG_DC_BEARER_TYPE_PRESENT &
                p_src_mod_list->drb_config[count].bitmask)
        {
            p_dst_mod_list->drb_config[count].dc_bearer_type =
                p_src_mod_list->drb_config[count].dc_bearer_type;

            /*We also need to update this information in p_erab_ctx,
             *  * which shall be reverted in case of fallback procedure*/
            p_erab_ctx->dc_bearer_type =
                p_src_mod_list->drb_config[count].dc_bearer_type;

            p_erab_ctx->m.DC_BearerTypePresent = RRC_ONE;
            /*We also need to update this information in p_erab_ctx->drb_config
             *which shall be reverted in case of fallback procedure*/
            p_erab_ctx->drb_config.dc_bearer_type =
                p_src_mod_list->drb_config[count].dc_bearer_type;

            p_erab_ctx->drb_config.bitmask |=
                RRM_DRB_CONFIG_DC_BEARER_TYPE_PRESENT;
        }

        /* qos_id */
        p_dst_mod_list->drb_config[count].qos_id = 
            p_erab_ctx->drb_config.s1u_config.qos_id;

        p_dst_mod_list->drb_config[count].qos_profile.
            allocation_retention_priority =
            p_erab_ctx->drb_config.s1u_config.qos_profile.
            allocation_retention_priority;

        p_dst_mod_list->drb_config[count].qos_profile.
            qos_profile_data_size = 
            p_erab_ctx->drb_config.s1u_config.qos_profile.
            qos_profile_data_size;

        l3_memcpy_wrapper(
                &p_dst_mod_list->drb_config[count].qos_profile.qos_profile_data,
                &p_erab_ctx->drb_config.s1u_config.qos_profile.qos_profile_data,
                p_erab_ctx->drb_config.s1u_config.qos_profile.qos_profile_data_size);


        /*check if scg was already present, then do not update is_sgnb_add_reqd
         * is ue context*/
        if(RRC_FALSE == is_scg_bearer_already_present)
        {
            p_ue_context->is_sgnb_add_reqd = RRC_TRUE;
        }
        else
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(
                    RRC_ERROR, "Already SCG bearer exist "
                    " for this ue ");
            result = RRC_FAILURE;
        }
    }
    return result;
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_process_rrm_dc_bearer_change_req
 * INPUT         : uecc_ue_context_t*                   p_ue_context,
 *                 rrc_rrm_dc_bearer_change_req_t*      p_msg
 * OUTPUT        : None
 * RETURNS       : rrc_return_t
 * DESCRIPTION   : This is the function processes RRC_RRM_DC_BEARER_CHANGE_REQ
 *                 event received from RRM and store the required information
 *                 in current procedure data.
 ****************************************************************************/
rrc_return_t
uecc_fsm_process_rrm_dc_bearer_change_req
(
    uecc_ue_context_t*                p_ue_context,
    rrc_rrm_dc_bearer_change_req_t*   p_msg
)
{
    rrc_return_t                   result              = RRC_SUCCESS;
    dc_bearer_change_request_data_t*  p_dc_bearer_change_req_data = PNULL;
	U8                             index               = 0;
	U8                             erab_id             = 0;
    
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_msg);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Fetch pointer to dc bearer change req data */
    p_dc_bearer_change_req_data = &(p_ue_context->p_curr_proc_data->u.
                                rrm_dc_bearer_change_request_data);

    /* Store message data in UE context for future processing */
	for (index = 0; index < p_msg->drb_to_modify_list.drb_count; index++)
	{
		erab_id = p_msg->drb_to_modify_list.drb_config[index].erab_id;
		if ( p_msg->drb_to_modify_list.drb_config[index].
			erab_id == p_ue_context->p_e_rab_list[erab_id]->e_RAB_ID  )
		{
			continue;
		}
		else
		{
    		RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
				"Invalid Erab Id received for DC bearer change req from RRM:[%d]",
				p_msg->drb_to_modify_list.drb_config[index].erab_id);
			result = RRC_FAILURE;
			return result;
		}
	}

    if(RRC_SUCCESS == result)
    {
        if(RRM_DC_BEARER_CHANGE_REQ_DRB_TO_MODIFY_LIST_PRESENT &
                p_msg->bitmask)
        {
            result = uecc_fsm_parse_dc_bearer_change_drb_to_modify_list(
                    &p_dc_bearer_change_req_data->drb_to_modify_list,
                    &p_msg->drb_to_modify_list,
                    p_ue_context);

            if(RRC_SUCCESS == result)
            {
                p_dc_bearer_change_req_data->m.drb_config_updated = RRC_ONE;
            }
            else
            {
                RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                        "SCG bearer already exist for DC bearer change req UE from RRM");
                result = RRC_FAILURE;
                return result;
            }
        }

        if(RRM_DC_BEARER_CHANGE_REQ_SGNB_ADD_PARAMS_PRESENT &
                p_msg->bitmask)
        {
             p_dc_bearer_change_req_data->m.sgnb_add_config_updated =
                 RRC_ONE;

            /*update the ue_agg_max_bit_rate_sgnb received from RRM in 
             * DC_BEARER_CHANGE_PROC req*/
            p_dc_bearer_change_req_data->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                ue_agg_max_bit_rate_ul =
                p_msg->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                ue_agg_max_bit_rate_ul;

            p_dc_bearer_change_req_data->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                ue_agg_max_bit_rate_dl =
                p_msg->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                ue_agg_max_bit_rate_dl;

            p_ue_context->ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_ul =
                p_msg->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                ue_agg_max_bit_rate_ul;

            p_ue_context->ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_dl =
                p_msg->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                ue_agg_max_bit_rate_dl;

            if ( RRC_RRM_EXT_UE_AGG_MAX_BITRATE_PRESENT &
                p_dc_bearer_change_req_data->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                    bitmask )
            {
                p_dc_bearer_change_req_data->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                    bitmask |=
                    RRC_RRM_EXT_UE_AGG_MAX_BITRATE_PRESENT;

                p_dc_bearer_change_req_data->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                    ue_agg_max_bit_rate_ext.ext_ue_agg_max_bit_rate_ul =
                    p_msg->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                    ue_agg_max_bit_rate_ext.ext_ue_agg_max_bit_rate_ul;

                p_dc_bearer_change_req_data->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                    ue_agg_max_bit_rate_ext.ext_ue_agg_max_bit_rate_dl =
                    p_msg->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                    ue_agg_max_bit_rate_ext.ext_ue_agg_max_bit_rate_dl;

                p_ue_context->ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_ext.
                    ext_ue_agg_max_bit_rate_ul =
                    p_msg->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                    ue_agg_max_bit_rate_ext.ext_ue_agg_max_bit_rate_ul;

                p_ue_context->ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_ext.
                    ext_ue_agg_max_bit_rate_dl =
                    p_msg->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                    ue_agg_max_bit_rate_ext.ext_ue_agg_max_bit_rate_dl;
            }
        }

        /*
         * Store SgNB Addition params, if present
         */
        if ( RRM_DC_BEARER_CHANGE_REQ_SGNB_ADD_PARAMS_PRESENT & p_msg->bitmask)
        {
            p_dc_bearer_change_req_data->m.sgnb_add_config_updated = RRC_ONE;

            l3_memcpy_wrapper(&p_dc_bearer_change_req_data->sgnb_add_param,
                    &p_msg->sgnb_add_param,
                    sizeof(rrc_rrm_sgnb_add_param_t));
        }

        if ( RRM_DC_BEARER_REQ_MEAS_CONFIG_PRESENT &
            p_msg->bitmask )
        {
            p_dc_bearer_change_req_data->m.endc_meas_config_updated = RRC_ONE;

            l3_memcpy_wrapper(&p_dc_bearer_change_req_data->endc_meas_config,
                    &p_msg->rrc_rrm_endc_meas_config,
                    sizeof(rrc_rrm_endc_meas_config_t));
        }
    }
   
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
* FUNCTION NAME : uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_ongoing_timer_expiry_handler 
* INPUT         : uecc_ue_context_t*  p_ue_context,
*                 void*               p_api
* OUTPUT        : None
* RETURNS       : None
* DESCRIPTION   : Timer Expiry handler for UE DC bearer change procedure
****************************************************************************/
void
uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_ongoing_timer_expiry_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    U8      count = RRC_ZERO;

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    
    /*
     * set the DC bearer conversion failure flag for all the RABs
     */
    for(count = 0; count < p_ue_context->p_curr_proc_data->u.
            rrm_dc_bearer_change_request_data.drb_to_modify_list.drb_count;
        count++)
    {
        p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
            drb_to_modify_list.drb_config[count].erab_failed = RRC_TRUE;
    }

    uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)RRM_DC_BEARER_CHANGE_FAIL_RRC_RECONFIG);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_dc_bearer_change_update_ue_context
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function updates the UE Context after completion 
 *                 of DC bearer change procedure.
 ****************************************************************************/
rrc_return_t
uecc_dc_bearer_change_update_ue_context
(
    uecc_ue_context_t*    p_ue_context
)
{
    dc_bearer_change_request_data_t*  p_dc_bearer_change_req = PNULL;
    U8                             erab_count     = RRC_NULL;
    U8                             erab_id        = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*
     * Fetch the list of ERABs which are successfully configured
     */
    p_dc_bearer_change_req = &(p_ue_context->p_curr_proc_data->u.
                         rrm_dc_bearer_change_request_data);

    /*
     * Update the DRB config in the UE Context as the procedure is now complete
     */
    for ( erab_count = RRC_NULL; 
         erab_count < p_dc_bearer_change_req->drb_to_modify_list.drb_count;
         erab_count++ )
    {
        if(RRC_FALSE == 
                p_dc_bearer_change_req->drb_to_modify_list.
                drb_config[erab_count].erab_failed)
        {
            erab_id = p_dc_bearer_change_req->drb_to_modify_list.drb_config[erab_count].
                erab_id;

            if(RRC_PNULL != p_ue_context->p_e_rab_list[erab_id])
            {
                /*update the dc_bearer_rype in drb_config and p_e_rab_list*/
                p_ue_context->p_e_rab_list[erab_id]->dc_bearer_type =
                    SN_TERMINATED_SPLIT_BEARER;

                p_ue_context->p_e_rab_list[erab_id]->drb_config.dc_bearer_type =
                    SN_TERMINATED_SPLIT_BEARER;

                p_ue_context->p_e_rab_list[erab_id]->drb_config.bitmask |=
                    RRM_DRB_CONFIG_LOGICAL_CHANNEL_IDENTITY_PRESENT;

                p_ue_context->p_e_rab_list[erab_id]->drb_config.logical_channel_identity =
                    p_dc_bearer_change_req->drb_to_modify_list.drb_config[erab_count].
                    new_lc_id;

                p_ue_context->p_e_rab_list[erab_id]->drb_config.drb_id =
                    p_dc_bearer_change_req->drb_to_modify_list.drb_config[erab_count].
                    new_drb_id;

                /*update the teid and transport layer information in uecc_drb_ctx_t*/
                if( RRC_ONE == 
                        p_dc_bearer_change_req->drb_to_modify_list.        
                        drb_config[erab_count].m.rrc_x2u_dl_teid_self_updated)
                {
                    p_ue_context->p_e_rab_list[erab_id]->dl_x2u_teid_self =
                        p_dc_bearer_change_req->drb_to_modify_list.
                        drb_config[erab_count].dl_x2u_teid_self;
                }

                if( RRC_ONE ==
                        p_dc_bearer_change_req->drb_to_modify_list.                      
                        drb_config[erab_count].m.rrc_x2u_dl_trasport_addr_self_updated)
                {
                    p_ue_context->p_e_rab_list[erab_id]->dl_x2u_transport_layer_addr_self.
                        data_length = 
                        p_dc_bearer_change_req->drb_to_modify_list.drb_config[erab_count].
                        dl_x2u_transport_layer_addr_self.data_length;

                    l3_memcpy_wrapper(
                            p_ue_context->p_e_rab_list[erab_id]->\
                            dl_x2u_transport_layer_addr_self.data,
                            p_dc_bearer_change_req->drb_to_modify_list.\
                            drb_config[erab_count].dl_x2u_transport_layer_addr_self.data,
                            p_ue_context->p_e_rab_list[erab_id]->\
                            dl_x2u_transport_layer_addr_self.data_length);
                }

                if(RRC_ONE ==
                        p_dc_bearer_change_req->drb_to_modify_list.
                        drb_config[erab_count].m.rrc_x2u_ul_teid_peer_updated)
                {
                    p_ue_context->p_e_rab_list[erab_id]->ul_x2u_teid_peer =
                        p_dc_bearer_change_req->drb_to_modify_list.
                        drb_config[erab_count].ul_x2u_teid_peer;
                }

                if(RRC_ONE == p_dc_bearer_change_req->drb_to_modify_list.
                        drb_config[erab_count].m.rrc_x2u_ul_trasport_addr_updated)
                {
                    /* coverity fix 277491 */
                   if ( RRC_SUCCESS != rrc_copy_transportlayer_address(
                            &p_ue_context->p_e_rab_list[erab_id]->
                            ul_x2u_transport_layer_addr,
                            &p_dc_bearer_change_req->drb_to_modify_list.
                            drb_config[erab_count].ul_x2u_transport_layer_addr,
                            p_ue_context) )
                   {
                       RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                                        "FAILED: Copy of the Trasport Layer Address from Source to Target");
                       return RRC_FAILURE;

                   }
                   /* coverity fix 277491 */
                }

                /*Reset the pdcp_config container as the pdcp has been removed from MeNB*/ 
                p_ue_context->p_e_rab_list[erab_id]->drb_config.bitmask = 
                    (rrc_bitmask_t)(p_ue_context->p_e_rab_list[erab_id]->drb_config.bitmask &
                    ~RRM_DRB_CONFIG_PDCP_CONFIG_PRESENT);

                memset_wrapper(&p_ue_context->p_e_rab_list[erab_id]->drb_config.pdcp_config, 
                        0x00, sizeof(rrc_pdcp_config_t));
            }
            else
            {
                RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                        "Invalid Erab Id received for DC bearer change:[%d]",
                        erab_id);
                return RRC_FAILURE;
            }
        }
    }
    /*TODO if required, need to clear, teid_self and
     * transportLayerAddress_self from uecc_drb_ctx_t in p_ue_context*/ 

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_p_success_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : Success handler for UE DC bearer change procedure
  ****************************************************************************/
void
uecc_fsm_rrm_dc_bearer_change_p_success_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*
     * Stop all the running timers
     */
    if ( PNULL !=
            p_ue_context->timers_data.timers_ids[UECC_DC_BEARER_CHANGE_GUARD_TIMER] )
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_DC_BEARER_CHANGE_GUARD_TIMER,
                &p_ue_context->timers_data);
    }

    if ( PNULL !=
            p_ue_context->timers_data.timers_ids[UECC_DC_PREP_TIMER] )
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_DC_PREP_TIMER,
                &p_ue_context->timers_data);
    }

    /* Build and send the RRC_RRM_DC_BEARER_CHANGE_CNF msg to RRM */
    if ( RRC_SUCCESS !=
            uecc_rrm_build_and_send_dc_bearer_change_cnf(p_ue_context, RRC_SUCCESS) )
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_WARNING,
                "[RRC_RRM_DC_BEARER_CHANGE_RESP] Delivery failure");
    }

    /*
     * Commit data in UE CONTEXT
     */
    uecc_dc_bearer_change_update_ue_context(p_ue_context);

    /*
     * Free the data for current procedure in the UE Context
     */
    if ( PNULL != p_ue_context->p_curr_proc_data )
    {
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    /* Send event to main FSM to indicate successful completion of
     * RRM triggered DC bearer change procedure
     */
    uecc_event_queue_push_event(
                UECC_EV_RRM_DC_BEARER_CHANGE_CNF,
                p_ue_context,
                PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_s1u_reconfig_for_dc_bearer_change_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void
uecc_fsm_s1u_reconfig_for_dc_bearer_change_init_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    if ( RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context,
            UECC_DC_BEARER_CHANGE_GUARD_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(
                RRC_WARNING,
                "[UECC_DC_BEARER_CHANGE_GUARD_TIMER]"
                "Timer Start Failed");
    }
    /*
     * Build and sent the S1U Reconfig request to LLIM
     */
    if ( RRC_SUCCESS != uecc_llim_build_and_send_s1u_reconfig_req(p_ue_context) )
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
                "FAILURE: Send UECC_LLIM_S1U_RECONFIG_REQ to LLIM");

        if ( RRC_SUCCESS != uecc_rrm_build_and_send_dc_bearer_change_cnf( 
                    p_ue_context, RRC_FAILURE) )
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                    "Failed to send the DC bearer change response to RRM");
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
* FUNCTION NAME : uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_cnf_handler 
* INPUT         : uecc_ue_context_t*  p_ue_context,
*                 void*               p_api
* OUTPUT        : None
* RETURNS       : None
* DESCRIPTION   : Handles S1U_RECONFIG_RESP for UE DC bearer change procedure
*                 from LLIM
****************************************************************************/
void
uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_cnf_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_s1u_reconfigure_cnf_t
        *p_s1u_reconfig_cnf = RRC_PNULL;
    dc_bearer_change_request_data_t
        *p_dc_bearer_change_req_data = RRC_PNULL; 
    U8          drb_count = RRC_ZERO;
    U8          s1u_cnf_drb_count = RRC_ZERO;
    U8          tunnel_count = RRC_ZERO;
    rrc_bool_t  self_tunnel_details_updated = RRC_FALSE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_s1u_reconfig_cnf = (rrc_uecc_llim_s1u_reconfigure_cnf_t *)
        p_api;

    do
    {
        if ( SUCCESS == p_s1u_reconfig_cnf->response_code)
        {
            /*Parse S1U Reconfig Cnf  and store the new self TeID*/
            if( RRC_UECC_LLIM_S1U_SETUP_SAP_CNF_PRESENT &
                    p_s1u_reconfig_cnf->bitmask)
            {
                p_dc_bearer_change_req_data = &p_ue_context->p_curr_proc_data->u.
                    rrm_dc_bearer_change_request_data;

                for(s1u_cnf_drb_count = RRC_ZERO;
                        s1u_cnf_drb_count < p_s1u_reconfig_cnf->setup_sap_cnf_list.num_setup_sap_cnf;
                        s1u_cnf_drb_count++)
                {
                    self_tunnel_details_updated = RRC_FALSE;
                    for(drb_count = RRC_ZERO;
                            drb_count < p_dc_bearer_change_req_data->drb_to_modify_list.drb_count
                            &&
                            RRC_FALSE == self_tunnel_details_updated;
                            drb_count++)
                    {
                        if ( p_dc_bearer_change_req_data->drb_to_modify_list.
                                drb_config[drb_count].new_lc_id == p_s1u_reconfig_cnf->setup_sap_cnf_list.
                                setup_sap_cnf[s1u_cnf_drb_count].lc_id
                                &&
                                S1U_SUCCESS == p_s1u_reconfig_cnf->setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].
                                cause
                                &&
                                (UECC_LLIM_S1U_TUNNEL_INFO_CNF_PRESENT & 
                                 p_s1u_reconfig_cnf->setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].bitmask)
                           )
                        {
                            /* Update Tunnel Info in UECC Curr Proc Data*/
                            for(tunnel_count = RRC_ZERO;
                                    tunnel_count <  p_s1u_reconfig_cnf->setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].
                                        tunnel_info_cnf_count;
                                    tunnel_count++)
                            {
                                if ( S1U_SUCCESS == p_s1u_reconfig_cnf->setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].
                                        tunnel_info_cnf[tunnel_count].cause)
                                {
                                    /**Fill Self TeID in curr Proc **/
                                    if( RRC_UECC_LLIM_S1U_TUNNEL_INFO_CNF_TEID_SELF_PRESENT & p_s1u_reconfig_cnf->\
                                        setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].tunnel_info_cnf[tunnel_count].bitmask)
                                    {
                                        p_dc_bearer_change_req_data->drb_to_modify_list.
                                            drb_config[drb_count].dl_x2u_teid_self =
                                            p_s1u_reconfig_cnf->setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].
                                            tunnel_info_cnf[tunnel_count].teid_self;

                                        p_dc_bearer_change_req_data->drb_to_modify_list.
                                            drb_config[drb_count].m.rrc_x2u_dl_teid_self_updated = RRC_ONE;
                                    }

                                    /** Fill TransporLayerAddress in curr proc **/
                                    if( RRC_UECC_LLIM_S1U_TUNNEL_INFO_CNF_TRANSPORT_ADDRESS_PRESENT & p_s1u_reconfig_cnf->\
                                        setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].tunnel_info_cnf[tunnel_count].bitmask)
                                    {
                                        p_dc_bearer_change_req_data->drb_to_modify_list.
                                            drb_config[drb_count].dl_x2u_transport_layer_addr_self.data_length =
                                            p_s1u_reconfig_cnf->setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].
                                            tunnel_info_cnf[tunnel_count].transport_addr.data_length;

                                        l3_memcpy_wrapper(
                                                p_dc_bearer_change_req_data->drb_to_modify_list.
                                                drb_config[drb_count].dl_x2u_transport_layer_addr_self.data,
                                                p_s1u_reconfig_cnf->setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].
                                                tunnel_info_cnf[tunnel_count].transport_addr.data,
                                                 p_dc_bearer_change_req_data->drb_to_modify_list.drb_config[drb_count].
                                                 dl_x2u_transport_layer_addr_self.data_length);

                                        p_dc_bearer_change_req_data->drb_to_modify_list.
                                            drb_config[drb_count].m.rrc_x2u_dl_trasport_addr_self_updated = RRC_ONE;
                                    }
                                    /* Should there be any logic for this to be true?
                                     * i.e. both TeID and Transport layer adderess  need to be updated
                                     */
                                    self_tunnel_details_updated = RRC_TRUE;
                                    break; /* Storing only one TeID*/
                                }
                                else
                                {
                                    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                                            "[RRC_S1U_RECOFNIG_UE_CNF] cause Failed ," 
                                            "Finish procedure with generic fail cause");

                                    /*
                                     * Process failure
                                     */
                                    uecc_fsm_finish_activity_failure(p_ue_context, 
                                            (void*)RRM_DC_BEARER_CHANGE_FAIL_RRC_RECONFIG);
                                }
                            }
                        }
                    }
                }
            }

            UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing,
                    PNULL);
        }
        else   
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                    "[RRC_S1U_RECOFNIG_UE_CNF] Failed ," 
                    "Finish procedure with generic fail cause");

            /*
             * Process failure
             */
            uecc_fsm_finish_activity_failure(p_ue_context, 
                    (void*)RRM_DC_BEARER_CHANGE_FAIL_RRC_RECONFIG);
        }
    } while(RRC_NULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void
uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_init_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*Build and send SgNB Addition Request*/
    if (RRC_SUCCESS != 
            uecc_x2ap_build_and_send_sgnb_addtion_request(p_ue_context))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(
                RRC_ERROR,
                "Error while sending the SGNB_ADD_REQ");

        /*
         * Process failure
         */
		uecc_fsm_finish_activity_failure(
				p_ue_context,
				(void*)RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD);
    }
    else
    {
        /* increment the sk_counter needs to be incremented every time
         * a SgNBAddition Request is send.
         */
        p_ue_context->sk_counter_r15++;

        /* Stopping UECC_DC_BEARER_CHANGE_GUARD_TIMER as initiating an outside 
         * network call for SgNB Addtition. The timer will be reinitiated
         * once the Acknowledgement for the SgNB Addition is received. 
         */
        if ( PNULL !=
            p_ue_context->timers_data.timers_ids[UECC_DC_BEARER_CHANGE_GUARD_TIMER] )
        {
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_DC_BEARER_CHANGE_GUARD_TIMER,
                &p_ue_context->timers_data);  
        }

        /* Start UECC_DC_PREP_TIMER for SgNBAddition Procedure.
         * This timer should be stopped once the 
         * SgNBAdditionRequestAcknowledgement is received successfully.
         */
        if (RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context, 
                    UECC_DC_PREP_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                    "UECC_DC_PREP_TIMER IS NOT Started");
            /*
             * Process failure
             */
            uecc_fsm_finish_activity_failure(
                    p_ue_context,
                    (void*)RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_ack_rcvd 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_X2AP_SGNB_ADDITION_REQ_ACK from X2AP
 ****************************************************************************/
void uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_ack_rcvd(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Stop DC_PREP_TIMER after SgNBAdditionRequestAck is parsed successfully
     * else wait for DC_PREP_TIMER Expiry event. */
    /*Process and store SgNB Addition Request Ack Message Params*/
    if (RRC_SUCCESS != 
            uecc_x2ap_handle_sgnb_addition_ack_msg_rcvd(p_ue_context, p_api))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(
                RRC_ERROR,
                "Error while handling SgNBAdditionRequestAck"
                " message. Start Local Error Handling");
        /*Logical_Error*/
        uecc_fsm_process_event(
                p_ue_context,
                UECC_EV_X2AP_ERROR_INDICATION,
                p_api);
        /*Logical_Error*/

        /*
         * Process failure
         */
        uecc_fsm_finish_activity_failure(p_ue_context, 
                (void*)RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD);
    }
    else
    {
        /* Stopping UECC_DC_PREP_TIMER on reception of SgNBAddReqAck */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_DC_PREP_TIMER,
                &p_ue_context->timers_data);  

        /*Go for Second S1u Reconfig procedure*/
        UECC_FSM_SET_CHILD_STATE(
                p_ue_context,
                &uecc_fsm_rm_llim_reconfig_after_sgnb_add_ongoing,
                PNULL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_timer_expiry
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void
uecc_fsm_rm_dc_bearer_change_sgnb_addition_ongoing_timer_expiry
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);

    U8 count = RRC_ZERO;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*
     * set the DC bearer conversion failure flag for all the RABs
     */
    for(count = 0; count < p_ue_context->p_curr_proc_data->u.
            rrm_dc_bearer_change_request_data.drb_to_modify_list.drb_count;
        count++)
    {
        p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
            drb_to_modify_list.drb_config[count].erab_failed = RRC_TRUE;
    }

  	/*release the x2ap context for this perticular UE*/
    uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);

    /*reset is_sgnb_add_reqd in p_ue_context*/
    p_ue_context->is_sgnb_add_reqd = RRC_FALSE;

    /*Logical_Error*/
    if ( RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context,
            UECC_DC_BEARER_CHANGE_GUARD_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(
                RRC_WARNING,
                "[UECC_DC_BEARER_CHANGE_GUARD_TIMER]"
                "Timer Start Failed");
    }

    /* Change state to uecc_fsm_rm_dc_bearer_change_fail_s1u_reconfig_ongoing */
    UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_rm_dc_bearer_change_fail_s1u_reconfig_ongoing,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
* FUNCTION NAME : uecc_fsm_llim_s1u_reconfig_after_sgnb_add_cnf_handler 
* INPUT         : uecc_ue_context_t*  p_ue_context,
*                 void*               p_api
* OUTPUT        : None
* RETURNS       : None
* DESCRIPTION   : Handles S1U_RECONFIG_RESP for UE DC bearer change procedure
*                 from LLIM
****************************************************************************/
void
uecc_fsm_llim_s1u_reconfig_after_sgnb_add_cnf_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_s1u_reconfigure_cnf_t
        *p_s1u_reconfig_cnf = RRC_PNULL;
    dc_bearer_change_request_data_t
        *p_dc_bearer_change_req_data = RRC_PNULL; 
    U8          drb_count = RRC_ZERO;
    U8          s1u_cnf_drb_count = RRC_ZERO;
    U8          tunnel_count = RRC_ZERO;
    rrc_bool_t  self_tunnel_details_updated = RRC_FALSE;
    rrc_bool_t  s1ap_cnf_is_valid = RRC_FALSE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_s1u_reconfig_cnf = (rrc_uecc_llim_s1u_reconfigure_cnf_t *)
        p_api;

    do
    {
        if ( RRC_SUCCESS == p_s1u_reconfig_cnf->response_code)
        {
            if( RRC_UECC_LLIM_S1U_SETUP_SAP_CNF_PRESENT &
                    p_s1u_reconfig_cnf->bitmask)
            {
                p_dc_bearer_change_req_data = &p_ue_context->p_curr_proc_data->u.
                    rrm_dc_bearer_change_request_data;

                for(s1u_cnf_drb_count = RRC_ZERO;
                        s1u_cnf_drb_count < p_s1u_reconfig_cnf->setup_sap_cnf_list.num_setup_sap_cnf;
                        s1u_cnf_drb_count++)
                {
                    s1ap_cnf_is_valid = RRC_FALSE;
                    for(drb_count = RRC_ZERO;
                            drb_count < p_dc_bearer_change_req_data->drb_to_modify_list.drb_count &&
                            RRC_FALSE == self_tunnel_details_updated;
                            drb_count++)
                    {
                        if ( p_dc_bearer_change_req_data->drb_to_modify_list.
                                drb_config[drb_count].new_lc_id == p_s1u_reconfig_cnf->setup_sap_cnf_list.
                                setup_sap_cnf[s1u_cnf_drb_count].lc_id
                                &&
                                S1U_SUCCESS == 
                                p_s1u_reconfig_cnf->setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].cause)
                        {
                            s1ap_cnf_is_valid = RRC_TRUE;
                            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(
                                    RRC_INFO,"S1U Reconfig Success for LC ID[%d]",
                                    p_s1u_reconfig_cnf->setup_sap_cnf_list.
                                    setup_sap_cnf[s1u_cnf_drb_count].lc_id
                                    );
                            /* Update Tunnel Info in UECC Curr Proc Data*/
                            for(tunnel_count = RRC_ZERO;
                                    tunnel_count <  p_s1u_reconfig_cnf->setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].
                                        tunnel_info_cnf_count;
                                    tunnel_count++)
                            {
                                /**Fill Self TeID in curr Proc **/
                                if( RRC_UECC_LLIM_S1U_TUNNEL_INFO_CNF_TEID_SELF_PRESENT & p_s1u_reconfig_cnf->\
                                        setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].tunnel_info_cnf[tunnel_count].bitmask)
                                {
                                    p_dc_bearer_change_req_data->drb_to_modify_list.
                                        drb_config[drb_count].dl_x2u_teid_self =
                                        p_s1u_reconfig_cnf->setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].
                                        tunnel_info_cnf[tunnel_count].teid_self;

                                    p_dc_bearer_change_req_data->drb_to_modify_list.
                                        drb_config[drb_count].m.rrc_x2u_dl_teid_self_updated = RRC_ONE;
                                }

                                /** Fill TransporLayerAddress in curr proc **/
                                if( RRC_UECC_LLIM_S1U_TUNNEL_INFO_CNF_TRANSPORT_ADDRESS_PRESENT & p_s1u_reconfig_cnf->\
                                        setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].tunnel_info_cnf[tunnel_count].bitmask)
                                {
                                    p_dc_bearer_change_req_data->drb_to_modify_list.
                                        drb_config[drb_count].dl_x2u_transport_layer_addr_self.data_length =
                                        p_s1u_reconfig_cnf->setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].
                                        tunnel_info_cnf[tunnel_count].transport_addr.data_length;

                                    l3_memcpy_wrapper(
                                            p_dc_bearer_change_req_data->drb_to_modify_list.
                                            drb_config[drb_count].dl_x2u_transport_layer_addr_self.data,
                                            p_s1u_reconfig_cnf->setup_sap_cnf_list.setup_sap_cnf[s1u_cnf_drb_count].
                                            tunnel_info_cnf[tunnel_count].transport_addr.data,
                                            p_dc_bearer_change_req_data->drb_to_modify_list.drb_config[drb_count].
                                            dl_x2u_transport_layer_addr_self.data_length);

                                    p_dc_bearer_change_req_data->drb_to_modify_list.
                                        drb_config[drb_count].m.rrc_x2u_dl_trasport_addr_self_updated = RRC_ONE;
                                }
                                /* Should there be any logic for this to be true?
                                 * i.e. both TeID and Transport layer adderess  need to be updated
                                 */
                                self_tunnel_details_updated = RRC_TRUE;
                                /* coverity fix 277531 */
                                //break; /* Storing only one TeID*/
                                /* coverity fix 277531 */
                            }
                        }
                    }
                    if(RRC_FALSE == s1ap_cnf_is_valid)
                    {
                        /*
                         * Process failure
                         */
                        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
                                "self_tunnel_details_updated2 =%d \n", self_tunnel_details_updated); 
                        uecc_fsm_finish_activity_failure(p_ue_context, 
                                (void*)RRM_DC_BEARER_CHANGE_FAIL_RRC_RECONFIG);
                        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                        return;
                    }
                }
            }
            if ( RRC_UECC_LLIM_S1U_REL_SAP_CNF_PRESENT &
                    p_s1u_reconfig_cnf->bitmask)
            {
                if  ( p_s1u_reconfig_cnf->rel_sap_cnf_list.num_rel_sap_cnf )
                {
                    if ( RRC_SUCCESS != p_s1u_reconfig_cnf->response_code)
                    {
                        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                                "[RRC_S1U_RECOFNIG_UE_CNF] Failed ," 
                                "Finish procedure with generic fail cause");

                        /*
                         * Process failure
                         */
                        uecc_fsm_finish_activity_failure(p_ue_context, 
                                (void*)RRM_DC_BEARER_CHANGE_FAIL_ENB_UE_REL);
                    }
                    else
                    {
                    	/*Logical_Error*/
                    	/*This is not failure procedure*/
     					/*UECC_FSM_SET_CHILD_STATE(
                    			p_ue_context,
                    			&uecc_fsm_rm_llim_reconfig_after_sgnb_add_ongoing,
                    			PNULL);
                        uecc_fsm_finish_activity_failure(
                                p_ue_context, (void*)RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD);
                        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);*/
                    	/*Logical_Error*/
                        return;
                    }
                }

            }

            /* Change state to uecc_fsm_rm_llim_reconfig_after_second_s1u_reconfigure_ongoing */
            UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_rm_llim_reconfig_after_second_s1u_reconfigure_ongoing,
                    PNULL);
        }
        else
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                    "[RRC_S1U_RECOFNIG_UE_CNF] Failed ," 
                    "Finish procedure with generic fail cause");
            /* Fill SgNB cause for RLF/Reestablishment Scenario,
             * SCGFailureInformationNR will already have the cause filled
             * at the time of Parsing the message at LLIM.
             */
            if (RRC_ONE != p_ue_context->m.sgnbCausePresent) 
            {
                if( PNULL != p_ue_context->p_curr_proc_data &&
                        RRC_RLF_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
                {
                    /* For RLF Scenario */
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;

                    p_ue_context->m.sgnbCausePresent = RRC_ONE;
                }
                else if( PNULL != p_ue_context->p_curr_proc_data &&
                        RRC_RE_ESTABLISH_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
                {
                    /* For Reestablishment Scenario */
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_procedure_cancelled;

                    p_ue_context->m.sgnbCausePresent = RRC_ONE;
                }
                else
                {
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;

                    p_ue_context->m.sgnbCausePresent = RRC_ONE;
                }

            }
            /* Send SgNBReconfigurationComplete with failure */ 
            if (RRC_SUCCESS != 
                    uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                        p_ue_context
                        ))
            {
                RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR, 
                        "[%s],uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure"
                        " failed!", __FUNCTION__);
            }

            /* Reset SgNBAddRequired Flag in UE_CTX*/
            p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
            p_ue_context->x2apUecontextReleased = RRC_TRUE;
            p_ue_context->x2ap_context.menb_ue_x2ap_id = RRC_INVALID_UE_INDEX;

            /*
             * Process failure
             */
            uecc_fsm_finish_activity_failure(p_ue_context, 
                    (void*)RRM_DC_BEARER_CHANGE_FAIL_RRC_RECONFIG);
        }
    } while(RRC_NULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_llim_reconfig_after_second_s1u_reconfigure_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*              - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
static void
uecc_fsm_rm_llim_reconfig_after_second_s1u_reconfigure_init_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*Build and send the UECC_LLIM_ADD_LC_REQ msg to LLIM
     * for the new lc id allocated by RRM in DC_BEARER_CHANGE_PROC*/
    if (RRC_SUCCESS != uecc_llim_build_and_send_add_lc_req(p_ue_context))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_FATAL,
                "FAILURE: UNABLE TO SEND UECC_LLIM_ADD_LC_REQ");

        /*
         * Process failure
         */
        uecc_fsm_finish_activity_failure(p_ue_context, 
                (void*)RRM_DC_BEARER_CHANGE_FAIL_RRC_RECONFIG);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_llim_reconfig_resp_after_second_s1u_reconfigure_cnf_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*              - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_RECONFIGURE_UE_RESP message.
 ****************************************************************************/
static void
uecc_fsm_llim_reconfig_resp_after_second_s1u_reconfigure_cnf_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_add_lc_resp_t  *p_add_lc_resp = RRC_PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_add_lc_resp = (rrc_uecc_llim_add_lc_resp_t *)p_api;

    do
    {
        if ( RRC_SUCCESS == p_add_lc_resp->response_code )
        {
            /*
             * Send PDCP Stop Traffic Ind
             */
            uecc_llim_build_and_send_pdcp_lwa_traffic_state_update_ind(
                    p_ue_context,
                    STOP_LWA_TRAFFIC);

            /*
             * Build and sent the ReEstablishment request to LLIM for old LC ID
             */
            if ( RRC_SUCCESS != uecc_llim_build_and_send_re_est_req(p_ue_context) )
            {
                uecc_fsm_finish_activity_failure(
                        p_ue_context, 
                        (void *)RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD);
            }
        }
        else
        {
            /* Fill SgNB cause for RLF/Reestablishment Scenario,
             * SCGFailureInformationNR will already have the cause filled
             * at the time of Parsing the message at LLIM.
             */
            if (RRC_ONE != p_ue_context->m.sgnbCausePresent) 
            {
                if( PNULL != p_ue_context->p_curr_proc_data &&
                        RRC_RLF_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
                {
                    /* For RLF Scenario */
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;

                    p_ue_context->m.sgnbCausePresent = RRC_ONE;
                }
                else if( PNULL != p_ue_context->p_curr_proc_data &&
                        RRC_RE_ESTABLISH_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
                {
                    /* For Reestablishment Scenario */
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_procedure_cancelled;

                    p_ue_context->m.sgnbCausePresent = RRC_ONE;
                }
                else
                {
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;

                    p_ue_context->m.sgnbCausePresent = RRC_ONE;
                }
            }
            /* Send SgNBReconfigurationComplete with failure */ 
            if (RRC_SUCCESS != 
                    uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                        p_ue_context
                        ))
            {
                RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR, 
                        "[%s],uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure"
                        " failed!", __FUNCTION__);
            }

            /* Reset SgNBAddRequired Flag in UE_CTX*/
            p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
            p_ue_context->x2apUecontextReleased = RRC_TRUE;

            /*
             * Finish procedure and trigger UE release
             */
            uecc_fsm_finish_activity_failure(
                    p_ue_context, 
                    (void *)RRM_DC_BEARER_CHANGE_FAIL_ENB_UE_REL);
        }
    }while(RRC_NULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_llim_reestab_cnf_after_second_s1u_reconfigure_cnf_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*              - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP message.
 ****************************************************************************/
void
uecc_fsm_llim_reestab_cnf_after_second_s1u_reconfigure_cnf_handler
(
    uecc_ue_context_t  *p_ue_context,
    void               *p_api
)
{
    rrc_uecc_llim_re_establish_ue_entity_resp_t
        *p_re_establish_ue_entity_rest = 
        (rrc_uecc_llim_re_establish_ue_entity_resp_t *)p_api;
    rrc_return_t                    result              = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    do
    {
        /*
         * Check whether UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP 
         * received with success or failure or partial success
         */
        switch ( p_re_establish_ue_entity_rest->response_code )
        {
            case RRC_SUCCESS:
            {
                RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
                        "LLIM has modified the requested ERABs to SN terminated Split bearer");

                UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_rm_dc_bearer_change_ue_reconfigure_ongoing, PNULL);

                result = RRC_SUCCESS;
                break;
            }

            case RRC_FAILURE:
            default:
            {
                RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                        "[UECC_LLIM_RE_ESTABLISHMENT_RESP]"
                        " Invalid Response Code = [%u]. Start failure processing",
                        p_re_establish_ue_entity_rest->response_code);
                break;
            }
        }
    }while(RRC_NULL);

    if (RRC_SUCCESS != result)
    {
        /* Fill SgNB cause for RLF/Reestablishment Scenario,
         * SCGFailureInformationNR will already have the cause filled
         * at the time of Parsing the message at LLIM.
         */
        if (RRC_ONE != p_ue_context->m.sgnbCausePresent) 
        {
            if( PNULL != p_ue_context->p_curr_proc_data &&
                    RRC_RLF_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
            {
                /* For RLF Scenario */
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;

                p_ue_context->m.sgnbCausePresent = RRC_ONE;
            }
            else if( PNULL != p_ue_context->p_curr_proc_data &&
                    RRC_RE_ESTABLISH_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
            {
                /* For Reestablishment Scenario */
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_procedure_cancelled;

                p_ue_context->m.sgnbCausePresent = RRC_ONE;
            }
            else
            {
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;

                p_ue_context->m.sgnbCausePresent = RRC_ONE;
            }
        }
        /* Send SgNBReconfigurationComplete with failure */ 
        if (RRC_SUCCESS != 
                uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                    p_ue_context
                    ))
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR, 
                    "[%s],uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure"
                    " failed!", __FUNCTION__);
        }

        /* Reset SgNBAddRequired Flag in UE_CTX*/
        p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
        p_ue_context->x2apUecontextReleased = RRC_TRUE;

        /* Finish procedure and trigger UE release */
        uecc_fsm_finish_activity_failure(
                p_ue_context, 
                (void *)RRM_DC_BEARER_CHANGE_FAIL_ENB_UE_REL);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
* FUNCTION NAME : uecc_fsm_llim_reconfig_timer_exp_after_sgnb_add_cnf_handler 
* INPUT         : uecc_ue_context_t*  p_ue_context,
*                 void*               p_api
* OUTPUT        : None
* RETURNS       : None
* DESCRIPTION   : Timer Expiry handler for UE DC bearer change procedure
****************************************************************************/
static void
uecc_fsm_llim_reconfig_timer_exp_after_sgnb_add_cnf_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    U8 count = RRC_ZERO; 
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


    /*
     * set the DC bearer conversion failure flag for all the RABs
     */
    /* coverity fix 277500 */
     if( PNULL != p_ue_context->p_curr_proc_data)
     {
         for(count = 0; count < p_ue_context->p_curr_proc_data->u.
                 rrm_dc_bearer_change_request_data.drb_to_modify_list.drb_count;
                 count++)
         {
             p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                 drb_to_modify_list.drb_config[count].erab_failed = RRC_TRUE;
         }
     }
    /* coverity fix 277500 */ 

    if ( RRC_SUCCESS != uecc_ue_timer_start(
                p_ue_context->p_gb_context,
                UECC_DC_BEARER_CHANGE_GUARD_TIMER,
                &p_ue_context->p_gb_context->ue_timers_data,
                &p_ue_context->timers_data,
                p_ue_context->ue_index))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(
                RRC_WARNING,
                "[UECC_DC_BEARER_CHANGE_GUARD_TIMER]"
                "Timer Start Failed");
    }

    /*
     * Build and sent the S1U Reconfig request to LLIM
     */
    if ( RRC_SUCCESS != uecc_llim_build_and_send_delete_new_lc_s1u_reconfig_req(p_ue_context) )
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
                "FAILURE: Send UECC_LLIM_S1U_RECONFIG_REQ to LLIM");

        uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
* FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_ue_reconfig_timer_exp_handler 
* INPUT         : uecc_ue_context_t*  p_ue_context,
*                 void*               p_api
* OUTPUT        : None
* RETURNS       : None
* DESCRIPTION   : Timer Expiry handler for UE DC bearer change procedure
****************************************************************************/
static void
uecc_fsm_rrm_dc_bearer_change_ue_reconfig_timer_exp_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Fill SgNB cause for RLF/Reestablishment Scenario,
     * SCGFailureInformationNR will already have the cause filled
     * at the time of Parsing the message at LLIM.
     */
    if (RRC_ONE != p_ue_context->m.sgnbCausePresent) 
    {
        if( PNULL != p_ue_context->p_curr_proc_data &&
                RRC_RLF_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
        {
            /* For RLF Scenario */
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;

            p_ue_context->m.sgnbCausePresent = RRC_ONE;
        }
        else if( PNULL != p_ue_context->p_curr_proc_data &&
                RRC_RE_ESTABLISH_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
        {
            /* For Reestablishment Scenario */
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_procedure_cancelled;

            p_ue_context->m.sgnbCausePresent = RRC_ONE;
        }
        else
        {
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;

            p_ue_context->m.sgnbCausePresent = RRC_ONE;
        }
    }
    /* Send SgNBReconfigurationComplete with failure */ 
    if (RRC_SUCCESS != 
            uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                p_ue_context
                ))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR, 
                "[%s],uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure"
                " failed!", __FUNCTION__);
    }

    /* Reset SgNBAddRequired Flag in UE_CTX*/
    p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    p_ue_context->x2apUecontextReleased = RRC_TRUE;

    uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)RRM_DC_BEARER_CHANGE_FAIL_ENB_UE_REL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
* FUNCTION NAME : uecc_fsm_llim_reconfig_timer_exp_after_sgnb_reconfig_complete_handler 
* INPUT         : uecc_ue_context_t*  p_ue_context,
*                 void*               p_api
* OUTPUT        : None
* RETURNS       : None
* DESCRIPTION   : Timer Expiry handler for UE DC bearer change procedure
****************************************************************************/
static void
uecc_fsm_llim_reconfig_timer_exp_after_sgnb_reconfig_complete_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Fill SgNB cause for RLF/Reestablishment Scenario,
     * SCGFailureInformationNR will already have the cause filled
     * at the time of Parsing the message at LLIM.
     */
    if (RRC_ONE != p_ue_context->m.sgnbCausePresent) 
    {
        if( PNULL != p_ue_context->p_curr_proc_data &&
                RRC_RLF_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
        {
            /* For RLF Scenario */
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;

            p_ue_context->m.sgnbCausePresent = RRC_ONE;
        }
        else if( PNULL != p_ue_context->p_curr_proc_data &&
                RRC_RE_ESTABLISH_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
        {
            /* For Reestablishment Scenario */
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_procedure_cancelled;

            p_ue_context->m.sgnbCausePresent = RRC_ONE;
        }
        else
        {
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;

            p_ue_context->m.sgnbCausePresent = RRC_ONE;
        }
    }
    /* Send SgNBReconfigurationComplete with failure */ 
    if (RRC_SUCCESS != 
            uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                p_ue_context
                ))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR, 
                "[%s],uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure"
                " failed!", __FUNCTION__);
    }

    /* Reset SgNBAddRequired Flag in UE_CTX*/
    p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    p_ue_context->x2apUecontextReleased = RRC_TRUE;

    uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)RRM_DC_BEARER_CHANGE_FAIL_ENB_UE_REL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_chnage_ue_reconfig_init_handler 
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void
uecc_fsm_rrm_dc_bearer_chnage_ue_reconfig_init_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",     
            p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_ue_context);

    /*
     * Build and send RRCConnectionReconfiguration
     */
    if (RRC_SUCCESS !=
            uecc_llim_build_and_send_erb_setup_rrc_connection_reconfiguration(
                                    p_ue_context))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_WARNING,
                        "[UECC_DC_BEARER_CHANGE_RRC_RECONFIG]"
                        "Unable to send RRC connection Reconfiguration to UE");

        /* Finish the current activity */
        uecc_fsm_finish_activity_failure(
                        p_ue_context, 
                        (void *)RRM_DC_BEARER_CHANGE_FAIL_ENB_UE_REL);
    }

    /*reset is_sgnb_add_reqd in ue_context*/
    p_ue_context->is_sgnb_add_reqd = RRC_FALSE;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_ue_reconfig_resp_handler 
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE
 ****************************************************************************/
void
uecc_fsm_rrm_dc_bearer_change_ue_reconfig_resp_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)
{
    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",     
            p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE");

    /*
     * Send ASN.1 Encoded SgNBReconfigurationComplete to SgNB
     */
    if ( RRC_SUCCESS !=
            uecc_x2ap_build_and_send_sgnb_reconfiguration_complete(p_ue_context) )
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_FATAL,
                "[x2ap_SgNBReconfigurationComplete] delivery failure");

        /*
         * Process failure
         */
        uecc_fsm_finish_activity_failure(
                p_ue_context, 
                (void*)RRM_DC_BEARER_CHANGE_FAIL_SGNB_RECONFIG_COMPLETE);
    }

    UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_rm_llim_reconfig_after_sgnb_reconfig_complete_ongoing,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_llim_reconfig_for_dc_bearer_change_init_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_STATE_INIT 
 ****************************************************************************/
void
uecc_fsm_llim_reconfig_for_dc_bearer_change_init_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)
{
    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",     
            p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_ue_context);

    /*
     * Build and send the UECC_LLIM_DELETE_LC_REQ msg to LLIM
     * for the old lc id
     */
    if ( RRC_SUCCESS != uecc_llim_build_and_send_erbmod_delete_lc_req(p_ue_context) )
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_FATAL,
                "FAILURE: UNABLE TO SEND UECC_LLIM_DELETE_LC_REQ");

        /*
         * Send event to main FSM to trigger UE Release
         */
        uecc_fsm_finish_activity_failure(
                        p_ue_context, 
                        (void *)RRM_DC_BEARER_CHANGE_FAIL_UE_REL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_llim_reconfig_resp_after_sgnb_reconfig_complete_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_RECONFIGURE_UE_RESP 
 ****************************************************************************/
void
uecc_fsm_llim_reconfig_resp_after_sgnb_reconfig_complete_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)
{
    rrc_uecc_llim_delete_lc_resp_t*  p_rrc_uecc_llim_delete_lc_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_LLIM_RECONFIGURE_UE_RESP");

    p_rrc_uecc_llim_delete_lc_resp = (rrc_uecc_llim_delete_lc_resp_t*)p_api;

    do
    {
        /*
         * Check whether UECC_EV_LLIM_RECONFIGURE_UE_RESP
         * received with success or failure or partial success
         */
        if ( SUCCESS == p_rrc_uecc_llim_delete_lc_resp->
                response_code )
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
                    "[UECC_EV_LLIM_DELETE_LC_RESP] Response Code =%i",
                    p_rrc_uecc_llim_delete_lc_resp->response_code);
            
            /*
             * Build and send the S1AP_ERAB_MODIFICATION_IND msg to S1AP
             */
            if ( RRC_SUCCESS != uecc_s1ap_build_and_send_erab_modification_ind(p_ue_context) )
            {
                /*
                 * Process failure
                 */
                uecc_fsm_finish_activity_failure(
                        p_ue_context, 
                        (void *)RRM_DC_BEARER_CHANGE_FAIL_ERAB_MODIFIC_IND);
            }
        }
        else if ( (PARTIAL_SUCCESS ==
                    p_rrc_uecc_llim_delete_lc_resp->response_code) ||
                (FAILURE ==
                 p_rrc_uecc_llim_delete_lc_resp->response_code) ||
                (L3_FATAL ==
                p_rrc_uecc_llim_delete_lc_resp->response_code) )
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
                    "[UECC_EV_LLIM_DELETE_LC_RESP] Response Code =%i",
                    p_rrc_uecc_llim_delete_lc_resp->response_code);
            /* Fill SgNB cause for RLF/Reestablishment Scenario,
             * SCGFailureInformationNR will already have the cause filled
             * at the time of Parsing the message at LLIM.
             */
            if (RRC_ONE != p_ue_context->m.sgnbCausePresent) 
            {
                if( PNULL != p_ue_context->p_curr_proc_data &&
                        RRC_RLF_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
                {
                    /* For RLF Scenario */
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;

                    p_ue_context->m.sgnbCausePresent = RRC_ONE;
                }
                else if( PNULL != p_ue_context->p_curr_proc_data &&
                        RRC_RE_ESTABLISH_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
                {
                    /* For Reestablishment Scenario */
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_procedure_cancelled;

                    p_ue_context->m.sgnbCausePresent = RRC_ONE;
                }
                else
                {
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;

                    p_ue_context->m.sgnbCausePresent = RRC_ONE;
                }
            }
            /* Send SgNBReconfigurationComplete with failure */ 
            if (RRC_SUCCESS != 
                    uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                        p_ue_context
                        ))
            {
                RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR, 
                        "[%s],uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure"
                        " failed!", __FUNCTION__);
            }

            /* Reset SgNBAddRequired Flag in UE_CTX*/
            p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
            p_ue_context->x2apUecontextReleased = RRC_TRUE;

            /*
             * Process failure
             */
            uecc_fsm_finish_activity_failure(
                    p_ue_context, 
                    (void *)RRM_DC_BEARER_CHANGE_FAIL_UE_REL);
        }
    }while(RRC_NULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_s1ap_erab_modification_cnf_after_sgnb_reconfig_complete_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_S1AP_ERAB_MODIFICATION_CNF 
 ****************************************************************************/
void
uecc_fsm_s1ap_erab_modification_cnf_after_sgnb_reconfig_complete_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*
     * Stop UECC_DC_BEARER_CHANGE_GUARD_TIMER 
     */
    if ( PNULL !=
            p_ue_context->timers_data.timers_ids[UECC_DC_BEARER_CHANGE_GUARD_TIMER] )
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_DC_BEARER_CHANGE_GUARD_TIMER,
                &p_ue_context->timers_data); 
    }

    uecc_fsm_finish_activity_success(p_ue_context, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_rrm_ue_release_req_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND 
 ****************************************************************************/
void
uecc_fsm_rrm_dc_bearer_change_rrm_ue_release_req_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*
     * Finish the current procedure
     */
    uecc_fsm_finish_activity_failure(
                    p_ue_context, 
                    (void *)RRM_DC_BEARER_CHANGE_FAIL_UE_REL);

    /*
     * Forward the received internal event to UE Main FSM along with 
     * decoded payload.
     */
    uecc_event_queue_push_event(
                    UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT,
                    p_ue_context,
                    p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_csc_cell_delete_req_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_CSC_CELL_DELETE_REQ_INT.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_csc_cell_delete_req_handler
(
    uecc_ue_context_t*  p_ue_context,  
    void*               p_api
) 
{
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    uecc_fsm_finish_activity_failure(
                p_ue_context, 
                (void *)RRM_DC_BEARER_CHANGE_FAIL_CELL_DEL);

    uecc_event_queue_push_event(
                UECC_EV_CSC_CELL_DELETE_REQ_INT,
                p_ue_context,
                p_api);
     
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_erab_setup_req_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function processes the received ERAB SETUP REQUEST 
 *                 from MME. It will simply discards the received message.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_erab_setup_req_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)  
{
    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",     
            p_api);

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,
            "[UECC_EV_S1AP_ERAB_SETUP_REQ]" 
            "DC bearer change Procedure Ongoing"
            "Discarding ERAB SetUp Request Received from MME");

    rrc_s1ap_erab_setup_request_t    *p_msg = 
         (rrc_s1ap_erab_setup_request_t *)p_api;
    OSRTDListNode* p_node = PNULL;
    U8 fail_list_count = RRC_NULL;        
    s1ap_E_RABToBeSetupListBearerSUReq *p_e_rab_to_be_setup_list = 
            &p_msg->e_rab_to_be_setup_list;
    s1ap_E_RABToBeSetupListBearerSUReq_element *p_erab_elem = PNULL;    
    uecc_erab_failed_to_setup_list_t           *p_failed_erab_data = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Memory allocated dynamically for curr_proc_data */
    p_ue_context->p_failed_curr_proc_data = (failed_curr_proc_data_t *)rrc_mem_get(sizeof(
                                     failed_curr_proc_data_t));
    do
    {
        if (PNULL == p_ue_context->p_failed_curr_proc_data)
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_FATAL,
                "Memory Allocation failed for Failed Current PROC data.");
                break;
        }
        memset_wrapper(p_ue_context->p_failed_curr_proc_data,
                   0,
                   sizeof(failed_curr_proc_data_t));
        p_failed_erab_data = &p_ue_context->p_failed_curr_proc_data->u.
                             erab_setup_request_data.failed_erab_data;

        /* Find the mutiple occurences of ERAB IDs in temp_erab_list array */
        for (p_node = p_e_rab_to_be_setup_list->head;
            (PNULL != p_node) && (p_e_rab_to_be_setup_list->count < MAX_MME_ERAB_LIST_COUNT);
            p_node=p_node->next )
        {
            p_erab_elem=(s1ap_E_RABToBeSetupListBearerSUReq_element*)p_node->data;
            if ((p_erab_elem == PNULL) || ( p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1 == PNULL))
            {
                continue;
            }
        
            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].
                        e_RAB_ID = p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID;
            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count].cause.t 
                        =  T_s1ap_Cause_radioNetwork;
            p_failed_erab_data->erab_failed_to_setup_item[fail_list_count++].cause.u.
                        radioNetwork = s1ap_interaction_with_other_procedure;
            
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                    "E_RAB ID Put in Failed List : %u",
                    p_erab_elem->value.u._E_RABToBeSetupItemBearerSUReqIEs_1->e_RAB_ID);
        } 
        p_failed_erab_data->count = fail_list_count;

        /* Send ASN.1 Encoded ERAB SETUP Response to MME */
        if(RRC_SUCCESS !=
           uecc_s1ap_build_and_send_erab_setup_response(p_ue_context))
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_FATAL,
                "[s1ap_E_RABSetupResponse] delivery failure");
        }

        /* Free memory for failed_curr_proc_data_t */
        if ( PNULL != p_ue_context->p_failed_curr_proc_data )
        {
            rrc_mem_free(p_ue_context->p_failed_curr_proc_data);
            p_ue_context->p_failed_curr_proc_data = PNULL;
        }
    }while(0);
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_erab_modify_req_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function processes the received ERAB MODIFY REQUEST
 *                 from MME. It will simply discard the received message.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_erab_modify_req_handler
(
    uecc_ue_context_t*  p_ue_context,          
    void*               p_api
)
{
    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",     
            p_api);

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,
            "[UECC_EV_S1AP_ERAB_RECONFIG_REQ]" 
            "DC bearer change Procedure Ongoing"
            "Discarding ERAB Reconfig Request Received from MME");

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_erab_release_command_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function processes the received ERAB RELEASE REQUEST 
 *                 from MME. It will simply discard the received message.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_erab_release_command_handler
(
    uecc_ue_context_t*  p_ue_context,          
    void*               p_api
)  
{
    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",     
            p_api);

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*
     * Do Nothing Here
     */
    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,
            "[UECC_EV_S1AP_ERAB_RELEASE_COMMAND]" 
            "DC bearer change Procedure Ongoing"
            "Discarding ERAB Release Command Received from MME");
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_erab_release_ind_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function processes the received ERAB RELEASE IND from 
 *                 RRM. It will discards the received message and send failure 
 *                 as response in RRC_RRM_ERB_RELEASE_IND_CNF message and error 
 *                 code for each ERAB Id set to "Interaction with other 
 *                 procedure".
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_erab_release_ind_handler
(
    uecc_ue_context_t*  p_ue_context,          
    void*               p_api
)
{
    U32 error_code = RRC_NO_ERROR;

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*
     * Build and send RRC_RRM_ERAB_RELEASE_IND_RESP to RRM
     * with response code as FAILURE and error_code for each 
     * ERAB_ID set to Interaction with other procedure
     */
    error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
    
    RRC_UECC_DC_BEARER_CHANGE_UPDATE_ERROR_CODE(error_code);

    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_ind_fail_resp(
                            p_api,
                            p_ue_context,
                            FAILURE,
                            error_code))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_WARNING,
                "[RRC_RRM_ERB_RELEASE_IND_CNF] Delivery failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_dc_bearer_change_after_sgnb_add_ongoing_ho_required_ind_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_RRM_HO_REQUIRED 
 *                 event.
 ****************************************************************************/
void uecc_fsm_dc_bearer_change_after_sgnb_add_ongoing_ho_required_ind_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_WARNING,
            "Ignoring the HO Required Request.Continue with DC bearer change Proc");

    /*
     * SEND HO FAILURE TO RRM
     */
    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->ho_info.cause.u.radioNetwork =
              s1ap_interaction_with_other_procedure;

    /*
     * Send the HO Failure Message to RRM
     */
    uecc_rrm_build_and_send_ho_failure(p_ue_context);
    if(PNULL != p_api)
    {
        rrc_mem_free(p_api);
        p_api = PNULL;
    }
 
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_s1ap_reset_msg_ind_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_S1AP_RESET_MSG_IND 
 *                 event.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_s1ap_reset_msg_ind_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void *)RRM_DC_BEARER_CHANGE_FAIL_S1AP_RESET);

    /*
     * Process failure
     */
    uecc_fsm_finish_activity_failure(p_ue_context, 
            (void*)RRM_DC_BEARER_CHANGE_FAIL_ENB_UE_REL);
    /*
     * Push internal event to UE Main FSM 
     * UECC_EV_S1AP_RESET_MSG_IND along with decoded payload.
     */
    uecc_event_queue_push_event(
            UECC_EV_S1AP_RESET_MSG_IND,
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_ue_rel_req_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_UE_RELEASE_REQ 
 *                 event.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_ue_rel_req_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
   
    /*
     * Finish the current procedure and trigger UE Release 
     */ 
    uecc_fsm_finish_activity_failure(
                p_ue_context, 
                (void *)RRM_DC_BEARER_CHANGE_FAIL_UE_REL);

    /*
     * Push internal event to UE Main FSM 
     * UECC_EV_UE_RELEASE_REQ along with decoded payload.
     */
    uecc_event_queue_push_event(
                UECC_EV_UE_RELEASE_REQ,
                p_ue_context,
                p_api);
 
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/**************************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_rrc_reestablish_request_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_RRC_RE_ESTABLISH_REQ 
 *                 event.
 *************************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_rrc_reestablish_request_handler 
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_RRC_RE_ESTABLISH_REQ");

    /* Send Re-establishment Reject */
    if (RRC_SUCCESS !=
            uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                p_ue_context->p_gb_context, p_ue_context->crnti, 0,
                p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                "Fail to send the RRCConnectionReestablishmentReject to UE");
    }
    else
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
                "[uecc_fsm_wait_for_s1ap_ue_ctx_rel_cmd_re_establish_req_handler]:"
                " RE-EST REJECT MSG SEND SUCCESSFULLY");
    }

    /* For Reestablishment Scenario */
    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_procedure_cancelled;

    p_ue_context->m.sgnbCausePresent = RRC_ONE;

    /* Send SgNBReconfigurationComplete with failure */
    if (RRC_SUCCESS !=
            uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                p_ue_context
                ))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                "[%s],uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure"
                " failed!", __FUNCTION__);
    }

    /* Reset SgNBAddRequired Flag in UE_CTX*/
    p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    p_ue_context->x2apUecontextReleased = RRC_TRUE;

    /*
     * Send event to main FSM to trigger UE Release
     */
    uecc_fsm_finish_activity_failure(
            p_ue_context, 
            (void *)RRM_DC_BEARER_CHANGE_FAIL_UE_REL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_ongoing_rlf_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_LLIM_RADIO_LINK_FAILURE_IND
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_ongoing_rlf_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    U16       len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    len = (U16)sizeof(rrc_uecc_llim_radio_link_failure_ind_t);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
        rrc_mem_get(len)))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_FATAL,
            "memory allocation failed to store "
            "received RLF msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* storing radio link failure contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
        p_api, len);

    /* Store type Radio Link Failure msg in UE Context */
    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RLF_MSG;

    uecc_fsm_finish_activity_failure(
         p_ue_context, 
         (void *)RRM_DC_BEARER_CHANGE_FAIL_RLF);
 
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*Error_Handling_changes*/
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_x2ap_error_ind 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_X2AP_ERROR_INDICATION
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_x2ap_error_ind(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
/*Logical_Error*/
	U8 count = 0;
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*stop the DC_PREP_TIMER as the error ind has been received from the peer sgnb*/
    uecc_ue_timer_stop(p_ue_context->p_gb_context,
            UECC_DC_PREP_TIMER,
            &p_ue_context->timers_data);  

    /*release the x2ap context for this perticular UE*/
  	if(RRC_SUCCESS != uecc_fsm_ue_context_release_handler(p_ue_context))
  	{
  	  	RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_FATAL,
  	      	  	"uecc_fsm_ue_context_release_handler failure");
  	}

    /*reset is_sgnb_add_reqd in p_ue_context*/
    p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    
     /* set the DC bearer conversion failure flag for all the RABs*/
    for(count = 0; count < p_ue_context->p_curr_proc_data->u.
            rrm_dc_bearer_change_request_data.drb_to_modify_list.drb_count;
        count++)
    {
        p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
            drb_to_modify_list.drb_config[count].erab_failed = RRC_TRUE;
    }
    
    /* Build and send the S1U Reconfig request to LLIM*/
    if ( RRC_SUCCESS != uecc_llim_build_and_send_delete_new_lc_s1u_reconfig_req(p_ue_context) )
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
                "FAILURE: Send UECC_LLIM_S1U_RECONFIG_REQ to LLIM");

        uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD);
    }
  
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*Error_Handling_changes*/

/*Logical_Error*/
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_dc_bearer_change_sgnb_add_req_reject 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_X2AP_SGNB_ADDITION_REQ_REJECT
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_sgnb_add_req_reject(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
	U8 count = 0;
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*stop the DC_PREP_TIMER as the error ind has been received from the peer sgnb*/
    uecc_ue_timer_stop(p_ue_context->p_gb_context,
            UECC_DC_PREP_TIMER,
            &p_ue_context->timers_data);  

    /*release the x2ap context for this perticular UE*/
    uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);

    /*reset is_sgnb_add_reqd in p_ue_context*/
    p_ue_context->is_sgnb_add_reqd = RRC_FALSE;

    /* set the DC bearer conversion failure flag for all the RABs*/
    for(count = 0; count < p_ue_context->p_curr_proc_data->u.
            rrm_dc_bearer_change_request_data.drb_to_modify_list.drb_count;
        count++)
    {
        p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
            drb_to_modify_list.drb_config[count].erab_failed = RRC_TRUE;
    }

    /* Build and send the S1U Reconfig request to LLIM*/
    if ( RRC_SUCCESS != uecc_llim_build_and_send_delete_new_lc_s1u_reconfig_req(p_ue_context) )
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
                "FAILURE: Send UECC_LLIM_S1U_RECONFIG_REQ to LLIM");

        uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

/*Logical_Error*/
}

/****************************************************************************
 * Function Name  : uecc_dc_bearer_change_fail_build_and_send_sgnb_release
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Sends SgNB Release to discard SgNBAdditionRequest send 
 *                  earlier.
 ****************************************************************************/
void uecc_dc_bearer_change_fail_build_and_send_sgnb_release
(
    uecc_ue_context_t*  p_ue_context
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    if( UECC_RRM_DC_BEARER_CHANGE_PROC == p_ue_context->p_curr_proc_data->t ) 
    {
        p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->sgnb_cause.u.radioNetwork = x2ap_tDCprep_expiry;

        /*
         * Send SgNB RELEASE to SgNB
         */ 
        if ( RRC_SUCCESS != 
                build_and_send_sgnb_release_request(p_ue_context) )
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR, 
                    "[%s],Unable to process SgNB Release Request", __FUNCTION__);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
* FUNCTION NAME : uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_fail_handler 
* INPUT         : uecc_ue_context_t*  p_ue_context,
*                 void*               p_api
* OUTPUT        : None
* RETURNS       : None
* DESCRIPTION   : Handles S1U_RECONFIG_RESP for UE DC bearer change procedure
*                 from LLIM
****************************************************************************/
void
uecc_fsm_llim_s1u_reconfig_for_dc_bearer_change_fail_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_s1u_reconfigure_cnf_t
        *p_s1u_reconfig_cnf = RRC_PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_s1u_reconfig_cnf = (rrc_uecc_llim_s1u_reconfigure_cnf_t *)
        p_api;

    do
    {
        if ( RRC_SUCCESS != p_s1u_reconfig_cnf->response_code)
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                    "[RRC_S1U_RECOFNIG_UE_CNF] Failed ," 
                    "Finish procedure with generic fail cause");

            /*
             * Process failure
             */
            uecc_fsm_finish_activity_failure(p_ue_context, 
                    (void*)RRM_DC_BEARER_CHANGE_FAIL_ENB_UE_REL);
        }
        else
        {
            /*
             * Send SgNB release to SgNB
             */
            uecc_dc_bearer_change_fail_build_and_send_sgnb_release(p_ue_context);
            p_ue_context->msg_type = 0;
        }
    } while(RRC_NULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/**************************************************************************************
 * FUNCTION NAME : uecc_fsm_sgnb_add_failure_rrc_reestablish_request_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_RRC_RE_ESTABLISH_REQ 
 *                 event.
 *************************************************************************************/
void uecc_fsm_sgnb_add_failure_rrc_reestablish_request_handler 
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)
{
	U16     len     = RRC_NULL;
    U8      count   = RRC_ZERO; 

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
            "UECC_EV_RRC_RE_ESTABLISH_REQ");

    /*
     * Stop DC Prep Timer
     */
    if ( PNULL !=
            p_ue_context->timers_data.timers_ids[UECC_DC_PREP_TIMER] )
    {
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                           UECC_DC_PREP_TIMER,
                           &p_ue_context->timers_data);
    }

    /*
     * set the DC bearer conversion failure flag for all the RABs
     */
    for(count = 0; count < p_ue_context->p_curr_proc_data->u.
            rrm_dc_bearer_change_request_data.drb_to_modify_list.drb_count;
        count++)
    {
        p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
            drb_to_modify_list.drb_config[count].erab_failed = RRC_TRUE;
    }

    /*
     * Send SgNB elease to SgNB
     */
    uecc_dc_bearer_change_fail_build_and_send_sgnb_release(p_ue_context);

    /*
     * Start DC bearer change Guard timer
     */
    if ( RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context,
            UECC_DC_BEARER_CHANGE_GUARD_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(
                RRC_WARNING,
                "[UECC_DC_BEARER_CHANGE_GUARD_TIMER]"
                "Timer Start Failed");
    }

    /*
     * Build and sent the S1U Reconfig request to LLIM
     */
    if ( RRC_SUCCESS != uecc_llim_build_and_send_delete_new_lc_s1u_reconfig_req(p_ue_context) )
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_INFO,
                "FAILURE: Send UECC_LLIM_S1U_RECONFIG_REQ to LLIM");

        uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)RRM_DC_BEARER_CHANGE_FAIL_SGNB_ADD);
    }

	/*
     * Store the Re-establish message in the UE Context
     */
	len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);
	if ( PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = rrc_mem_get(len)) )
	{
		RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_FATAL,
				"Memory Allocation Failed "
				"to store Reestablishment Req msg");
		RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
		return;
	}

	/*
     * storing re_establishment contents into msg_bfr_rcvd
     */
	l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
			p_api, len);

	p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RE_ESTABLISH_MSG;
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
* FUNCTION NAME : uecc_fsm_llim_reconfig_timer_exp_after_second_s1u_reconfigure_cnf_handler 
* INPUT         : uecc_ue_context_t*  p_ue_context,
*                 void*               p_api
* OUTPUT        : None
* RETURNS       : None
* DESCRIPTION   : Timer Expiry handler for UE DC bearer change procedure
****************************************************************************/
static void
uecc_fsm_llim_reconfig_timer_exp_after_second_s1u_reconfigure_cnf_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Fill SgNB cause for RLF/Reestablishment Scenario,
     * SCGFailureInformationNR will already have the cause filled
     * at the time of Parsing the message at LLIM.
     */
    if (RRC_ONE != p_ue_context->m.sgnbCausePresent) 
    {
        if( PNULL != p_ue_context->p_curr_proc_data &&
                RRC_RLF_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
        {
            /* For RLF Scenario */
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;

            p_ue_context->m.sgnbCausePresent = RRC_ONE;
        }
        else if( PNULL != p_ue_context->p_curr_proc_data &&
                RRC_RE_ESTABLISH_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
        {
            /* For Reestablishment Scenario */
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_procedure_cancelled;

            p_ue_context->m.sgnbCausePresent = RRC_ONE;
        }
        else
        {
            /* For Timer Expiry Scenario */
            p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
            p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;

            p_ue_context->m.sgnbCausePresent = RRC_ONE;

        }
    }
    /* Send SgNBReconfigurationComplete with failure */ 
    if (RRC_SUCCESS != 
            uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                p_ue_context
                ))
    {
        RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR, 
                "[%s],uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure"
                " failed!", __FUNCTION__);
    }

    /* Reset SgNBAddRequired Flag in UE_CTX*/
    p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    p_ue_context->x2apUecontextReleased = RRC_TRUE;

    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)RRM_DC_BEARER_CHANGE_FAIL_ENB_UE_REL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
* FUNCTION NAME : uecc_fsm_rm_dc_bearer_change_fail_action_rollback_ue_context 
* INPUT         : uecc_ue_context_t* - p_ue_context,
*                 rrc_bool_et        - p_break
* OUTPUT        : None
* RETURNS       : None
* DESCRIPTION   : Timer Expiry handler for UE DC bearer change procedure
****************************************************************************/
static void
uecc_fsm_rm_dc_bearer_change_fail_action_rollback_ue_context
(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et         p_break
)
{
    rrc_drb_to_modify_list_t    *p_drb_to_modify_list   = PNULL;
    uecc_drb_ctx_t              *p_erab_ctx             = PNULL;
    U8                          erab_idx                = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_break);

    /* Fetch pointer to rrc_drb_to_modify_list_t */
    p_drb_to_modify_list = &(p_ue_context->p_curr_proc_data->u.
                                rrm_dc_bearer_change_request_data.drb_to_modify_list);

    for(erab_idx = RRC_NULL;
            erab_idx < p_drb_to_modify_list->drb_count;
            erab_idx++)
    {
        p_erab_ctx = p_ue_context->p_e_rab_list[
            p_drb_to_modify_list->drb_config[erab_idx].erab_id];

        if(PNULL == p_erab_ctx)
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_ERROR,
                    "ERAB[%d] Context is not Present in UECC_CTXT",
                    p_drb_to_modify_list->drb_config[erab_idx].erab_id
                    );
            return;
        }

        /* Rollback All the data for the Failed ERABs, erab_failed needs to be set
         * beforehand
         */
        if(RRC_TRUE == p_drb_to_modify_list->drb_config[erab_idx].erab_failed)
        {
            /* Rollback LogicalChannelId to old LCID*/
            p_erab_ctx->drb_config.logical_channel_identity =
                p_drb_to_modify_list->drb_config[erab_idx].old_lc_id;

            /* Rollback DC_bearer_type in uecc_erab_ctxt*/
            p_erab_ctx->dc_bearer_type = RRC_ZERO;
            p_erab_ctx->m.DC_BearerTypePresent = RRC_ZERO;

            /* Rollback DC_bearer_type in uecc_erab_ctxt->drb_config*/
            p_erab_ctx->drb_config.dc_bearer_type = RRC_ZERO;
            p_erab_ctx->drb_config.bitmask ^=
                RRM_DRB_CONFIG_DC_BEARER_TYPE_PRESENT;

            /*RollBack sgnb_s1ap_gtp_id in uecc_erab_context*/
            memset_wrapper(
                    &p_erab_ctx->sgnb_s1ap_gtp_id,
                    RRC_ZERO,
                    sizeof(x2ap_GTP_TEI));

            /*RollBack sgnb_s1ap_transportLayerAddress in uecc_erab_context*/
            memset_wrapper(
                    &p_erab_ctx->sgnb_s1ap_transportLayerAddress,
                    RRC_ZERO,
                    sizeof(x2ap_TransportLayerAddress));
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_x2ap_reset_msg_ind_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_X2AP_RESET_MSG_IND 
 *                 event.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_x2ap_reset_msg_ind_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void *)RRM_DC_BEARER_CHANGE_FAIL_X2AP_RESET);

    /*
     * Push internal event to UE Main FSM 
     * UECC_EV_X2AP_RESET_MSG_IND along with decoded payload.
     */
    uecc_event_queue_push_event(
            UECC_EV_X2AP_RESET_MSG_IND,
            p_ue_context,
            p_api);

     RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rrm_dc_bearer_change_after_sgnb_add_s1ap_reset_msg_ind_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
 *                 void*               p_api  
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function handles UECC_EV_S1AP_RESET_MSG_IND 
 *                 event.
 ****************************************************************************/
void uecc_fsm_rrm_dc_bearer_change_after_sgnb_add_s1ap_reset_msg_ind_handler
(
    uecc_ue_context_t*  p_ue_context,   
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void *)RRM_DC_BEARER_CHANGE_FAIL_S1AP_RESET);

    /* Send ASN.1 Encoded SgNBReconfigurationComplete to SgNB if SgNB Addition was required*/
    if ( RRC_TRUE == p_ue_context->is_sgnb_add_reqd &&
            (RRC_NULL <
             rrc_uecc_count_scg_bearer(p_ue_context))
       )
    {
        if(RRC_SUCCESS !=
                uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                    p_ue_context))
        {
            RRC_UECC_RRM_DC_BEARER_RECONFIG_FSM_TRACE(RRC_FATAL,
                    "[x2ap_SgNBReconfigurationComplete] delivery failure");
        }

        /*reset is_sgnb_add_reqd in p_ue_context*/
        p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    }

    /*
     * Push internal event to UE Main FSM 
     * UECC_EV_S1AP_RESET_MSG_IND along with decoded payload.
     */
    uecc_event_queue_push_event(
            UECC_EV_S1AP_RESET_MSG_IND,
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
#endif
