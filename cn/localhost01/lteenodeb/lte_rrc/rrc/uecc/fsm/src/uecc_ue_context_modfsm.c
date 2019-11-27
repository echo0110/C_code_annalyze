/*******************************************************************************
 *
 *  FILE NAME: uecc_ue_context_modfsm.c
 *
 *  DESCRIPTION : UECC UE CONTEXT MODIFICATION FSM implementation functions
 *
 *  REVISION HISTORY :
 *
 *  DATE        AUTHOR      REF                          REASON
 *  --------    -------     ---                          ----------
 *  16 Aug 2010     Komal   ---------                    Initial
 *
 *  Copyright (c) 2010, Aricent Inc.
 ******************************************************************************/


/****************************************************************************
* Header File Includes
***************************************************************************/
#include "rrc_rrm_intf.h"
#include "uecc_logger.h"

#include "uecc_common.h"
#include "uecc_ue_timer_utils.h"
#include "uecc_ue_context_modfsm.h"
#include "uecc_rrm.h"
#include "uecc_llim.h"
#include "uecc_s1ap.h"
#include "uecc_defaults.h"
#include "uecc_mainfsm.h"
/*securitykey*/
#include "uecc_icsp.h"
/*securitykey*/

#include "uecc_fsm_engine.h"

/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(log_level, format, ...)              \
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,    \
        p_ue_context->ue_index,                                            \
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,        \
        ##__VA_ARGS__)
#define SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context)\
    (p_ue_context->share_data.uecc_rcr_data.\
     rrc_connection_release_not_required_flag = RRC_TRUE)
/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/

static void uecc_ue_context_mod_mme_p_constructor(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_ue_context_mod_mme_p_success_handler(
     uecc_ue_context_t*  p_ue_context,
     void*               p_api
);

static void uecc_fsm_ue_context_mod_rrm_ongoing_init_handler(
     uecc_ue_context_t*  p_ue_context,
     void*               p_api
);

static void uecc_fsm_ue_context_mod_rrm_resp_handler(
     uecc_ue_context_t   *p_ue_context,
     void                *p_api_data
);

static void uecc_fsm_ue_context_mod_erab_rel_ind_handler(
     uecc_ue_context_t*  p_ue_context,
     void*               p_api
);

static void uecc_fsm_ue_context_mod_rrm_ongoing_timer_expiry_handler(
     uecc_ue_context_t   *p_ue_context,
     void                *p_api_data
);

static void uecc_fsm_ue_context_mod_rrc_re_establish_handler(
     uecc_ue_context_t   *p_ue_context,
     void                *p_api_data
);

static void uecc_fsm_ue_context_mod_ho_required_handler(
     uecc_ue_context_t   *p_ue_context,
     void                *p_api_data
);

static void uecc_fsm_ue_context_mod_radio_link_failure_handler(
     uecc_ue_context_t   *p_ue_context,
     void                *p_api_data
);

/*SPR_18241_START*/
/* Code Removed */
/*SPR_18241_END*/

static void uecc_ue_context_mod_mme_p_failure_handler(
                uecc_ue_context_t*      p_ue_context,
                void*                   p_api
);

static void uecc_fsm_ue_context_mod_ue_release_req_handler(
                uecc_ue_context_t*      p_ue_context,
                void*                   p_api
);

static void uecc_fsm_ue_context_mod_ue_connection_rel_handler(
                uecc_ue_context_t*      p_ue_context,
                void*                   p_api
);

static void uecc_fsm_ue_context_mod_s1ap_reset_msg_ind_handler(
                uecc_ue_context_t*      p_ue_context,
                void*                   p_api
);

static void uecc_fsm_ue_context_mod_csc_delete_all_ue_req_handler(
        uecc_ue_context_t*      p_ue_context,
        void*                   p_api
);

/* Failure Processing functions */
void uecc_fsm_ue_ctx_mod_fail_action_send_rrm_cnf(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
);

void uecc_fsm_ue_ctx_mod_fail_action_send_fail_resp_to_s1ap(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
);

static void uecc_fsm_ue_ctx_mod_fail_action_send_ue_context_release_req(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
);

static void uecc_fsm_ue_ctx_mod_fail_action_last(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
);

/* state UE_CTX_MOD_LLIM_ONGOIING  */
static void uecc_fsm_ue_context_mod_llim_reconfigure_ue_ongoing_init_handler(
        uecc_ue_context_t*      p_ue_context,
        void*                   p_api
);
static void uecc_fsm_ue_context_mod_llim_reconfigure_ue_resp_handler(
        uecc_ue_context_t*      p_ue_context,
        void*                   p_api
);

static void  uecc_fsm_ue_context_mod_llim_reconfig_ue_ongoing_radio_link_failure_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
);

static void uecc_fsm_ue_context_mod_llim_reconfig_ue_ongoing_ho_required_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
);

static void uecc_fsm_ue_context_mod_llim_reconfig_ue_rrc_reestablish_request_handler( 
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
);

/* state UE_CTX_MOD_LLIM_RECONFIG_ROLLBACK_ONGOING */

static void uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_init_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
);

static void uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
);

static void uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_rrc_re_establish_request_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
);
static void uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_rrm_ho_required_ind_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
);

static void uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_timer_expiry_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
);
static void uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_radio_link_failure_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
);
/*securitykey*/
static void  rrc_build_handover_and_invoke_intracell_ho(
        uecc_ue_context_t   *p_ue_context,
        /* CSR#59144 fix start */
        U16                 t304_timer
        /* CSR#59144 fix stop */
);
static void uecc_ue_context_mod_after_security_rekying_p_constructor(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);
    
/*securitykey*/

/****************************************************************************
  Private Variables
 ****************************************************************************/

static const uecc_fsm_fail_handler_f uecc_fsm_ue_ctx_mod_fail_type1_actions[] =
{
    uecc_fsm_ue_ctx_mod_fail_action_send_rrm_cnf,
    uecc_fsm_ue_ctx_mod_fail_action_send_fail_resp_to_s1ap,
    uecc_fsm_ue_ctx_mod_fail_action_last,
    PNULL
};


static const uecc_fsm_fail_handler_f uecc_fsm_ue_ctx_mod_fail_type2_actions[] =
{
    uecc_fsm_ue_ctx_mod_fail_action_send_rrm_cnf,
    uecc_fsm_ue_ctx_mod_fail_action_send_fail_resp_to_s1ap,
    uecc_fsm_ue_ctx_mod_fail_action_send_ue_context_release_req,
    uecc_fsm_ue_ctx_mod_fail_action_last,
    PNULL
};



static uecc_fsm_handler_record_t uecc_fsm_ue_context_mod_rrm_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
        uecc_fsm_ue_context_mod_rrm_ongoing_init_handler},
    { { UECC_EV_RRM_UE_CONTEXT_MOD_RESP   },
        uecc_fsm_ue_context_mod_rrm_resp_handler},
    { { UECC_EV_TIMER_EXPIRY            },
        uecc_fsm_ue_context_mod_rrm_ongoing_timer_expiry_handler},
    { { UECC_EV_RRM_ERAB_RELEASE_IND            },
        uecc_fsm_ue_context_mod_erab_rel_ind_handler},
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND            },
        uecc_fsm_ue_context_mod_radio_link_failure_handler},
    { { UECC_EV_UE_RELEASE_REQ              },
        uecc_fsm_ue_context_mod_ue_release_req_handler },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
        uecc_fsm_ue_context_mod_ue_connection_rel_handler },
    { { UECC_EV_S1AP_RESET_MSG_IND  },
        uecc_fsm_ue_context_mod_s1ap_reset_msg_ind_handler },
    { { UECC_EV_RRM_HO_REQUIRED },
        uecc_fsm_ue_context_mod_ho_required_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ },
        uecc_fsm_ue_context_mod_csc_delete_all_ue_req_handler },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ  },
        uecc_fsm_ue_context_mod_rrc_re_establish_handler },
    { { UECC_LAST_ID                    },  PNULL}
};

static uecc_fsm_handler_record_t uecc_fsm_ue_context_mod_llim_reconfig_ue_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
        uecc_fsm_ue_context_mod_llim_reconfigure_ue_ongoing_init_handler},
    { { UECC_EV_LLIM_RECONFIGURE_UE_RESP   },
        uecc_fsm_ue_context_mod_llim_reconfigure_ue_resp_handler},
    { { UECC_EV_TIMER_EXPIRY            },
        uecc_fsm_ue_context_mod_rrm_ongoing_timer_expiry_handler},
    { { UECC_EV_RRM_ERAB_RELEASE_IND            },
        uecc_fsm_ue_context_mod_erab_rel_ind_handler},
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND            },
        uecc_fsm_ue_context_mod_llim_reconfig_ue_ongoing_radio_link_failure_handler},
    { { UECC_EV_UE_RELEASE_REQ              },
        uecc_fsm_ue_context_mod_ue_release_req_handler },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
        uecc_fsm_ue_context_mod_ue_connection_rel_handler },
    { { UECC_EV_S1AP_RESET_MSG_IND  },
        uecc_fsm_ue_context_mod_s1ap_reset_msg_ind_handler },
    { { UECC_EV_RRM_HO_REQUIRED },
        uecc_fsm_ue_context_mod_llim_reconfig_ue_ongoing_ho_required_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ },
        uecc_fsm_ue_context_mod_csc_delete_all_ue_req_handler },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ  },
        uecc_fsm_ue_context_mod_llim_reconfig_ue_rrc_reestablish_request_handler },
    { { UECC_LAST_ID                    },  PNULL}
};


static uecc_fsm_handler_record_t uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                             },
        uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_init_handler},
    { { UECC_EV_LLIM_MODIFY_ROLLBACK_RESP              },
        uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_resp_handler},
    { { UECC_EV_RRC_RE_ESTABLISH_REQ                   },
        uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_rrc_re_establish_request_handler},
    { { UECC_EV_RRM_HO_REQUIRED                    },
        uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_rrm_ho_required_ind_handler},
    { { UECC_EV_TIMER_EXPIRY                           },
        uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_timer_expiry_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND            },
        uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_radio_link_failure_handler},
    { { UECC_EV_UE_RELEASE_REQ                     },
        uecc_fsm_ue_context_mod_ue_release_req_handler },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
        uecc_fsm_ue_context_mod_s1ap_reset_msg_ind_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ },
        uecc_fsm_ue_context_mod_csc_delete_all_ue_req_handler },
    { { UECC_LAST_ID                            },  PNULL}
};
static uecc_fsm_activity_state_t uecc_ue_context_mod_rrm_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_UE_CONTEXT_MOD_RRM_ONGOING",
    /* .s_id           =  */ (S8*)"CONTXT_MOD_RRM",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ uecc_fsm_ue_context_mod_rrm_ongoing_mht
};

/*SPR_18241_START*/
static uecc_fsm_activity_state_t uecc_queued_ue_context_mod_rrm_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_UE_CONTEXT_MOD_RRM_ONGOING",
    /* .s_id           =  */ (S8*)"CONTXT_MOD_RRM",
    /* .is_need_init   =  */ RRC_FALSE,
    /* .p_mht          =  */ uecc_fsm_ue_context_mod_rrm_ongoing_mht
};
/*SPR_18241_END*/


static uecc_fsm_activity_state_t uecc_fsm_ue_context_mod_llim_reconfigure_ue_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_UE_CONTEXT_MOD_LLIM_RECONFIG_UE_ONGOING",
    /* .s_id           =  */ (S8*)"CONTXT_MOD_LLIM",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ uecc_fsm_ue_context_mod_llim_reconfig_ue_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_ue_context_mod_llim_reconfigure_ue_rollback_ongoing =
{
    /* .s_name         =  */ (S8*)"UECC_UE_CONTEXT_MOD_LLIM_RECONFIG_UE_ROLLBACK_ONGOING",
    /* .s_id           =  */ (S8*)"CONTXT_MOD_ROLLBACK",
    /* .is_need_init   =  */ RRC_TRUE,
    /* .p_mht          =  */ uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_mht
};

/*Activity*/
static uecc_fsm_activity_t uecc_fsm_rrc_ue_contxt_mod_activity =
{
    /*  .s_name             = */(S8*)"RRC_RRM_UE_CONTXT_MOD",
    /* .s_id               = */(S8*)"UE_CONTXT_MOD_A",
    /* .p_first_state      = */&uecc_ue_context_mod_rrm_ongoing
};
static uecc_fsm_activity_t*
                    uecc_fsm_ue_context_mod_mme_procedure_activity_list[] =
{
    &uecc_fsm_rrc_ue_contxt_mod_activity,
    PNULL
};
uecc_fsm_procedure_t uecc_fsm_ue_context_mod_procedure =
{
    /*.s_name             = */(S8*)"UE MOD MME",
    /*.s_id               = */(S8*)"UE_CONTXTMOD_P",
    /*.constructor_f      = */uecc_ue_context_mod_mme_p_constructor,
    /*.finish_success_f   = */uecc_ue_context_mod_mme_p_success_handler,
    /*.finish_fail_f      = */uecc_ue_context_mod_mme_p_failure_handler,
    /*.activity_list      = */uecc_fsm_ue_context_mod_mme_procedure_activity_list,
    /*.p_mht              = */PNULL
};

/*SPR_18241_START*/
/*Activity*/
static uecc_fsm_activity_t uecc_fsm_rrc_queued_ue_contxt_mod_activity =
{
    /*  .s_name             = */(S8*)"RRC_RRM_UE_CONTXT_MOD",
    /* .s_id               = */(S8*)"UE_CONTXT_MOD_A",
    /* .p_first_state      = */&uecc_queued_ue_context_mod_rrm_ongoing
};
static uecc_fsm_activity_t*
                    uecc_fsm_queued_ue_context_mod_mme_procedure_activity_list[] =
{
    &uecc_fsm_rrc_queued_ue_contxt_mod_activity,
    PNULL
};

/* This activity list is called when UE context modification is
 * received during reconfiguration. On receiving the UE context
 * modification, RRC forwards it to RRM but RRM queues it and
 * waits for ongoing procedure to complete. On completion of
 * ongoing procedure, RRM sends UE Context Modificatoin Resp
 * to RRC.
 */
uecc_fsm_procedure_t uecc_fsm_queued_ue_context_mod_procedure =
{
    /*.s_name             = */(S8*)"QUEUED UE MOD MME",
    /*.s_id               = */(S8*)"UE_CONTXTMOD_P",
    /*.constructor_f      = */uecc_ue_context_mod_mme_p_constructor,
    /*.finish_success_f   = */uecc_ue_context_mod_mme_p_success_handler,
    /*.finish_fail_f      = */uecc_ue_context_mod_mme_p_failure_handler,
    /*.activity_list      = */uecc_fsm_queued_ue_context_mod_mme_procedure_activity_list,
    /*.p_mht              = */PNULL
};
/*SPR_18241_END*/

/*securitykey*/
uecc_fsm_procedure_t uecc_fsm_ue_context_mod_after_security_rekeying_procedure =
{
    /*.s_name             = */(S8*)"UE MOD AFTER SECURITY",
    /*.s_id               = */(S8*)"UE_CONTXTMOD_AFTER_SECURITY_P",
    /*.constructor_f      = */uecc_ue_context_mod_after_security_rekying_p_constructor,
    /*.finish_success_f   = */PNULL,
    /*.finish_fail_f      = */uecc_ue_context_mod_mme_p_failure_handler,
    /*.activity_list      = */PNULL,
    /*.p_mht              = */PNULL
};
/*securitykey*/

/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/
/*securitykey*/
/****************************************************************************
 * Function Name  : uecc_ue_context_mod_after_security_rekying_p_constructor
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for UE_Context_Modification after Security
 *                  Rekying
 ****************************************************************************/
void uecc_ue_context_mod_after_security_rekying_p_constructor(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    uecc_fsm_activity_state_t* p_prev_current_state = PNULL;
    rrc_rrm_ue_contxt_mod_cnf_t    rrc_rrm_ue_contxt_mod_cnf;
    rrc_bool_et ue_release_flag = RRC_FALSE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    memset_wrapper(&rrc_rrm_ue_contxt_mod_cnf, 0x00, sizeof(rrc_rrm_ue_contxt_mod_cnf_t));

    /* Store transaction id and use it for current operation */
#ifdef RRC_UECC_TR_ID_CONST
    uecc_ue_ctx_set_op_transction_id(p_ue_context, 1); 
#else
    uecc_ue_ctx_set_op_transction_id(p_ue_context,
            uecc_ue_ctx_get_next_transction_id(p_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/

    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_INFO,
            "UE CONTEXT MOD: Transaction ID updated");

    /* Reset the UE Context Modificatin Security Flag */
    p_ue_context->ue_ctx_mod_security_flag = RRC_FALSE;

    do {
        if( RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context, 
                    UECC_UE_CONTXT_MOD_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_ERROR,
                    "UE CONTEXT MOD TIMER FAILED TO START in constuctor");

            uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

            uecc_event_queue_push_event(
                    UECC_EV_UE_CONTXT_MOD_FAIL_REL_UE_CONTXT_INT,
                    p_ue_context,
                    PNULL);
            break;
        }

        /*If Security Rekeying procedure is successful using Intra-Cell HO
         * Signalling, then send UE Ctx Mod Cnf to RRM and UE Ctx Resp (Success) to MME*/
        if(p_ue_context->ue_ctx_mod_intra_ho_success_flag)
        {
            RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_ERROR,
                    "UE CONTEXT MOD SECURITY COMPLETE");
            rrc_rrm_ue_contxt_mod_cnf.ue_index = p_ue_context->ue_index ;
            rrc_rrm_ue_contxt_mod_cnf.response = RRC_SUCCESS;

            if (RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(&rrc_rrm_ue_contxt_mod_cnf,
                        RRC_UECC_MODULE_ID,
                        RRC_RRM_MODULE_ID,
                        uecc_ue_ctx_get_op_transction_id(p_ue_context),
                        p_ue_context->cell_index))
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                        "[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf] Sending failed");
                ue_release_flag = RRC_TRUE;
            }

            if(RRC_TRUE != ue_release_flag)
            {
                if (RRC_SUCCESS !=
                        uecc_s1ap_build_and_send_ue_context_modification_response(p_ue_context))
                {
                    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                            "[s1ap_UEContextModificationResponse]Delivery failed");
                    ue_release_flag = RRC_TRUE;
                }
            }

            if(RRC_TRUE == ue_release_flag)
            {
                uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

                /*In case some error occurs while sending UE Context
                 * Modification Response to MME, send Failure to main-fsm with an indication to
                 * release UE Context*/
                uecc_event_queue_push_event(
                        UECC_EV_UE_CONTXT_MOD_FAIL_REL_UE_CONTXT_INT,
                        p_ue_context,
                        PNULL);
            }
            else
            {
                /* CSR 00070664 Fix Start */
                /* Stop Procedure Timer */
                uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                        UECC_UE_CONTXT_MOD_TIMER, 
                        &p_ue_context->timers_data);
                /* CSR 00070664 Fix Stop */

                /* Update the UE AMBR in UE Context from current procedure data*/
                if(p_ue_context->p_curr_proc_data->m.AggMaxBitRate)
                {
                    p_ue_context->ue_aggregate_maximum_bitrate =
                        p_ue_context->p_curr_proc_data->ue_aggregate_maximum_bitrate;
                }
                /* Reseting the ambr flag*/
                p_ue_context->p_curr_proc_data->m.AggMaxBitRate = RRC_FALSE;
                
                /* Update UE security capabilities in UE context from current procedure data*/
                if(p_ue_context->p_curr_proc_data->m.SecurityCapability_present)
                {
                    asn1Copy_s1ap_EncryptionAlgorithms(&p_ue_context->s1ap_asn1_ctx,
                            &p_ue_context->p_curr_proc_data->ue_security_capabilities.encryptionAlgorithms,
                            &p_ue_context->ue_security_capabilities.encryptionAlgorithms);

                    asn1Copy_s1ap_IntegrityProtectionAlgorithms(&p_ue_context->s1ap_asn1_ctx,
                            &p_ue_context->p_curr_proc_data->ue_security_capabilities.integrityProtectionAlgorithms,
                            &p_ue_context->ue_security_capabilities.integrityProtectionAlgorithms);
                }
                /* Reseting the UE Security Capability flag*/
                p_ue_context->p_curr_proc_data->m.SecurityCapability_present = RRC_FALSE;
                
                /* Update security key in UE context from current procedure data */
                if(p_ue_context->p_curr_proc_data->m.SecurityKey_present)
                { 
                    asn1Copy_s1ap_SecurityKey(&p_ue_context->s1ap_asn1_ctx,
                            &p_ue_context->p_curr_proc_data->security_key,
                            &p_ue_context->security_key);
                }
                /* Reseting the SecurityKey flag*/
                p_ue_context->p_curr_proc_data->m.SecurityKey_present = RRC_FALSE;
                uecc_fsm_finish_active_procedure(p_ue_context);
                uecc_event_queue_push_event(
                        UECC_EV_UE_CONTXT_MOD_SUCCESS_INT,
                        p_ue_context,
                        PNULL);
            }

            /* Reset the Intra-Cell HO Success Flag as Security Rekeying has
             * successfully done */
            p_ue_context->ue_ctx_mod_intra_ho_success_flag = RRC_FALSE;
        }
        else  /*If Security Rekeying procedure is unsuccessful */
        {
            RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_ERROR,
                    "UE CONTEXT MOD SECURITY FAILED");

            /*IT_FIX*/
            /*rollback for algorithm*/
            p_ue_context->configure_ciphering_user_plane.algorithm_id =
                p_ue_context->p_curr_proc_data->userplane_ciphering_algo_id;

            p_ue_context->configure_ciphering.algorithm_id = 
                p_ue_context->p_curr_proc_data->ciphering_algo_id;

            p_ue_context->configure_integrity_protection.algorithm_id =
                p_ue_context->p_curr_proc_data->integrity_algo_id;
            /*IT_FIX*/

            p_prev_current_state =
                p_ue_context->prev_ongoing_uecc_fsm.proc_descriptor.p_current_state;

            /* Copy Proc Descriptor information */
            l3_memcpy_wrapper(&p_ue_context->uecc_fsm.proc_descriptor,
                    &p_ue_context->prev_ongoing_uecc_fsm.proc_descriptor,
                    sizeof(uecc_fsm_procedure_descriptor_t));

            /*If previous state was LLIM Reconfiguration in case UE Context
             * Modificaiton was also recieved for AMBR.
             * In this case, if Security Rekying procedure is failed, we've to
             * Rollbak AMBR to the previously configured AMBR*/
            if (p_prev_current_state ==
                    &uecc_fsm_ue_context_mod_llim_reconfigure_ue_ongoing)
            {
                p_ue_context->uecc_fsm.proc_descriptor.p_current_state =
                    &uecc_fsm_ue_context_mod_llim_reconfigure_ue_rollback_ongoing; 

                UECC_FSM_SET_CHILD_STATE(
                        p_ue_context,
                        p_ue_context->uecc_fsm.proc_descriptor.p_current_state,
                        PNULL);
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            }
            /*In case UE Context Modificaiton was not recieved for AMBR*/
            else
            {
                uecc_fsm_ue_ctx_mod_fail_action_send_rrm_cnf(p_ue_context, NULL); 
                uecc_fsm_ue_ctx_mod_fail_action_send_fail_resp_to_s1ap(p_ue_context, NULL);

                uecc_fsm_finish_activity_failure(p_ue_context, PNULL);
                uecc_event_queue_push_event(
                        UECC_EV_UE_CONTXT_MOD_FAIL_INT,
                        p_ue_context,
                        PNULL);
            }
        }
    }while(0);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*securitykey*/

/* Coverity CID 99387 Fix Start */
/****************************************************************************
 * Function Name  : uecc_ue_context_mod_mme_p_constructor
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for UE Connection Setup procedure
 ****************************************************************************/
void uecc_ue_context_mod_mme_p_constructor(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    s1ap_Cause cause;
    rrc_return_et result =  RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*SPR_18241_START*/
    if(PNULL == p_ue_context->p_temp_ue_ctx_mod_proc_data)
    {
        /*SPR_18241_END*/
        /* Store transaction id and use it for current operation */
#ifdef RRC_UECC_TR_ID_CONST
        uecc_ue_ctx_set_op_transction_id(p_ue_context, 1); 
#else
        uecc_ue_ctx_set_op_transction_id(p_ue_context,
                uecc_ue_ctx_get_next_transction_id(p_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/

        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_INFO,
                "UE CONTEXT MOD: Transaction ID updated");

        /*Resetting the UE_CTX_MOD_SECURITY & INTRA_HO_SUCCESS Flags */
        p_ue_context->ue_ctx_mod_security_flag = RRC_FALSE;
        p_ue_context->ue_ctx_mod_intra_ho_success_flag = RRC_FALSE;

        do {
            if( RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context, 
                        UECC_UE_CONTXT_MOD_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_ERROR,
                        "UE CONTEXT MOD TIMER FAILED TO START in constuctor");
                break;
            }

            /*SPR_18241_START*/
            /* Allocate memory for the p_curr_proc_data */
            p_ue_context->p_curr_proc_data = (curr_proc_data_t *)
                rrc_mem_get(sizeof(curr_proc_data_t));
            if (p_ue_context->p_curr_proc_data == PNULL)
            {
                /* Memory Allocation Failed */
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_FATAL,
                        "Memory Allocation Failure");
                break;
            }    

            if(RRC_SUCCESS != uecc_fsm_process_ue_ctx_mod_msg(p_ue_context,
                        p_ue_context->p_curr_proc_data, 
                        p_api))
                /*SPR_18241_END*/
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_ERROR,
                        "UE CONTEXT MOD Prcess Msg failed");
                break;
            }

            RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_INFO,
                    "UE CONTEXT MOD Recieved from MME");
            result = RRC_SUCCESS;
        }while(0);

        if(RRC_SUCCESS != result)
        {
            /* stop timer if it has been started */
            if (PNULL !=
                    p_ue_context->timers_data.timers_ids[UECC_UE_CONTXT_MOD_TIMER])
            {
                /* Stop UE Context Modification procedural timer. */
                uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_UE_CONTXT_MOD_TIMER,
                        &p_ue_context->timers_data);
            }
            /*SPR_18241_START*/
            if((PNULL != p_ue_context->p_curr_proc_data) && 
                    (!p_ue_context->p_curr_proc_data->m.SecurityCapability_present))
                /*SPR_18241_END*/
            {
                cause.t = T_s1ap_Cause_radioNetwork;
                cause.u.radioNetwork = s1ap_unspecified_2;
            }
            else
            {
                cause.t = T_s1ap_Cause_radioNetwork;
                cause.u.protocol=s1ap_encryption_and_or_integrity_protection_algorithms_not_supported;
            }

            if (RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_ue_context_modification_failure(
                        p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                        "[s1ap_UEContextModificationFailure]Delivery failed");
            }
            uecc_fsm_finish_activity_failure(p_ue_context, PNULL);
            uecc_event_queue_push_event(
                    UECC_EV_UE_CONTXT_MOD_FAIL_INT,
                    p_ue_context,
                    PNULL);
        }
        /*SPR_18241_START*/
    }
    else
    {
        if( RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context, 
                    UECC_UE_CONTXT_MOD_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_ERROR,
                    "UE CONTEXT MOD TIMER FAILED TO START in constuctor");
            cause.t = T_s1ap_Cause_radioNetwork;
            cause.u.radioNetwork = s1ap_unspecified_2;

            if (RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_ue_context_modification_failure(
                        p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                        "s1ap_UEContextModificationFailure Delivery failed");
            }
            uecc_fsm_finish_activity_failure(p_ue_context, PNULL);
            uecc_event_queue_push_event(
                    UECC_EV_UE_CONTXT_MOD_FAIL_INT,
                    p_ue_context,
                    PNULL);
        }
    }
    /*SPR_18241_END*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* Coverity CID 99387 Fix End */

/****************************************************************************
 * Function Name  : uecc_ue_context_mod_mme_p_success_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Success handler for UE Connection Setup procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_ue_context_mod_mme_p_success_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Updating the UAMBR Value in UE Context */
        if (p_ue_context->p_curr_proc_data->m.AggMaxBitRate)
        {
            p_ue_context->m.AggMaxBitRate = 1;
            p_ue_context->ue_aggregate_maximum_bitrate.
            uEaggregateMaximumBitRateDL =
                p_ue_context->p_curr_proc_data->ue_aggregate_maximum_bitrate.
                uEaggregateMaximumBitRateDL;

            p_ue_context->ue_aggregate_maximum_bitrate.
            uEaggregateMaximumBitRateUL =
                p_ue_context->p_curr_proc_data->ue_aggregate_maximum_bitrate.
                uEaggregateMaximumBitRateUL;
        }
        /* Updating SpId in UE Context */
        if (p_ue_context->p_curr_proc_data->m.SpId)
        {
            p_ue_context->m.SpId = 1;
            p_ue_context->spid = p_ue_context->p_curr_proc_data->spid;
        }
        /* Updating CsfallbackIndicator in UE Context */
        if (p_ue_context->p_curr_proc_data->m.CsfallbackIndicator)
        {
            p_ue_context->m.CsfallbackIndicator = 1;
            p_ue_context->cs_fallback_indicator = 
                p_ue_context->p_curr_proc_data->cs_fallback_indicator;

            /* Reset Variables */
                p_ue_context->p_curr_proc_data->m.CsfallbackIndicator =
                RRC_FALSE;
            
        }
/* Updating CSG in UE Context*/
        if (p_ue_context->p_curr_proc_data->m.CsgMembershipStatus)
        {
            p_ue_context->m.CsgMembershipStatus = 1;
            p_ue_context->csg_membership_status =
                p_ue_context->p_curr_proc_data->csg_membership_status;
        }

        /* Updating Registered LAI in UE Context */
        if ((p_ue_context->p_curr_proc_data->m.registered_lai_present) &&
              (PNULL != p_ue_context->p_curr_proc_data->p_registered_lai))
        {
            //Memory Allocation
            if(PNULL == p_ue_context->p_registered_lai)
            {
                p_ue_context->p_registered_lai = 
                (registered_lai_t *)rrc_mem_get(sizeof(registered_lai_t));
                if (PNULL == p_ue_context->p_registered_lai)
                {
                    /* Memory Allocation Failed */
                    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_FATAL,
                      "Memory Allocation Failure");
                    return;
                }
            }

            p_ue_context->m.registered_lai_present = 1;

            p_ue_context->p_registered_lai->plmn_identity.num = 
                p_ue_context->p_curr_proc_data->p_registered_lai->plmn_identity.num;


            l3_memcpy_wrapper(p_ue_context->p_registered_lai->plmn_identity.data,
                    p_ue_context->p_curr_proc_data->p_registered_lai->plmn_identity.data,
                    p_ue_context->p_curr_proc_data->p_registered_lai->plmn_identity.num);

            p_ue_context->p_registered_lai->lac.num = 
                p_ue_context->p_curr_proc_data->p_registered_lai->lac.num;

            l3_memcpy_wrapper(p_ue_context->p_registered_lai->lac.data,
                    p_ue_context->p_curr_proc_data->p_registered_lai->lac.data,
                    p_ue_context->p_curr_proc_data->p_registered_lai->lac.num);
           
            /* Free the memory taken for registered lai */
            rrc_mem_free(p_ue_context->p_curr_proc_data->p_registered_lai);
            p_ue_context->p_curr_proc_data->p_registered_lai = PNULL;
        }
/*ambr*/
        /*commenting the following code as it's already being handled in mainfsm*/
        /* Free the memory taken for current procedure data */
        /*rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;*/
/*ambr*/
    }    

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_CONTXT_MOD_TIMER,
            &p_ue_context->timers_data);

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
            UECC_EV_UE_CONTXT_MOD_SUCCESS_INT,
            p_ue_context,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_rrm_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_ue_context_mod_rrm_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    s1ap_Cause cause;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /*BUG 604 changes start*/
    /* Invoke Intra cell HO  to RRM  if security is present*/
    if ((p_ue_context->p_curr_proc_data->m.SecurityKey_present) ||
            ( (p_ue_context->p_curr_proc_data->m.SecurityCapability_present) &&
             ((p_ue_context->p_curr_proc_data->userplane_ciphering_algo_id !=
               p_ue_context->configure_ciphering_user_plane.algorithm_id) ||
              (p_ue_context->p_curr_proc_data->ciphering_algo_id !=
               p_ue_context->configure_ciphering.algorithm_id) ||
              (p_ue_context->p_curr_proc_data->integrity_algo_id !=
               p_ue_context->configure_integrity_protection.algorithm_id)) ) )
    {
	    /*SPR_17041_START*/
	    if (p_ue_context->rrc_trg_ho_fwd_tnl_info.drb_tunnel_info_count == 0)
	    {
		    /* Set the flag, which if TURE will indicate that Intra-Cell HO procedure
		     * for Security Rekeying was originally invoked from UE Ctx Mod procedure*/
		    p_ue_context->ue_ctx_mod_security_flag = RRC_TRUE;
		    /* Build and send the RRC_RRM_INTRA_ENB_HO_IND to RRM */
		    if (RRC_SUCCESS != uecc_llim_build_and_send_rrc_rrm_intra_enb_ho_ind(p_ue_context))
		    {
			    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
					    p_ue_context->p_gb_context->facility_name, RRC_WARNING,
					    "[RRC_RRM_INTRA_ENB_HO_IND]message sending failed for UE %u",
					    p_ue_context->ue_index);
		    }
	    }
	    else
	    {
		    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
				    p_ue_context->p_gb_context->facility_name,
				    RRC_WARNING,
				    "[RRC_RRM_INTRA_ENB_HO_IND] Forwarding of previous HO is ongoing for UE %u.",
				    p_ue_context->ue_index);
		    /* Send UE Context Modfication Failure */
		    cause.t = T_s1ap_Cause_radioNetwork;
		    cause.u.radioNetwork = s1ap_interaction_with_other_procedure;

		    if (RRC_SUCCESS !=
				    uecc_s1ap_build_and_send_ue_context_modification_failure(
					    p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
		    {
			    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
					    "[s1ap_UEContextModificationFailure]Delivery failed");
		    }
		    uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

		    uecc_event_queue_push_event(
				    UECC_EV_UE_CONTXT_MOD_FAIL_INT,
				    p_ue_context,
				    PNULL);
	    }
	    /*SPR_17041_END*/
    }
    else
    {
    /*BUG 604 changes stop*/
        /* Build and sent the RRC_RRM_UE_CONTXT_MOD_REQ to RRM */
        if (RRC_SUCCESS !=
            uecc_rrm_build_and_send_ue_context_mod_req(p_ue_context))
        {
            /* Send UE Context Modfication Failure */
            cause.t = T_s1ap_Cause_radioNetwork;
            cause.u.radioNetwork = s1ap_unspecified_2;

            if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_ue_context_modification_failure(
                    p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                    "[s1ap_UEContextModificationFailure]Delivery failed");
            }
            uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

        uecc_event_queue_push_event(
                UECC_EV_UE_CONTXT_MOD_FAIL_INT,
                p_ue_context,
                PNULL);
    }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_rrm_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_UE_CONTEXT_MOD_RESP from a RRM.
 ****************************************************************************/
void uecc_fsm_ue_context_mod_rrm_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
)
{
    s1ap_Cause cause;
    rrc_rrm_ue_contxt_mod_cnf_t    rrc_rrm_ue_contxt_mod_cnf; 
    rrc_rrm_ue_contxt_mod_resp_t*  p_rrc_rrm_ue_contxt_mod_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    p_rrc_rrm_ue_contxt_mod_resp =
        (rrc_rrm_ue_contxt_mod_resp_t*)p_api_data;


    rrc_rrm_ue_contxt_mod_cnf.ue_index = p_rrc_rrm_ue_contxt_mod_resp->ue_index ; 
    rrc_rrm_ue_contxt_mod_cnf.response = RRC_SUCCESS;

    switch (p_rrc_rrm_ue_contxt_mod_resp->response)
    {
        case RRC_SUCCESS:
            /* CSR#59144 fix start */
            /*If T304 was requested but not received from RRM, UCM procedure gets failed*/
            if(RRC_TRUE == p_ue_context->p_curr_proc_data->t304_requested_from_rrm)
            {
                p_ue_context->p_curr_proc_data->t304_requested_from_rrm = RRC_FALSE;
                if(!(UE_CONTEXT_MOD_RESP_T304_PRESENCE_FLAG & p_rrc_rrm_ue_contxt_mod_resp->bitmask))
                {
                    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_ERROR,
                            "T304 not recieved from RRM in UE_CONTEXT_MOD_RESP message, sending failure");
                    /* Send UE Context Modfication Failure with unspecified
                     * cause */
                    cause.t = T_s1ap_Cause_misc;
                    cause.u.misc = s1ap_unspecified;

                    if (RRC_SUCCESS !=
                            uecc_s1ap_build_and_send_ue_context_modification_failure(
                                p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
                    {
                        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                                "[s1ap_UEContextModificationFailure]Delivery failed");
                    }
                    uecc_fsm_finish_activity_failure(p_ue_context, PNULL);
                    uecc_event_queue_push_event(
                            UECC_EV_UE_CONTXT_MOD_FAIL_INT,
                            p_ue_context,
                            PNULL);
                    break;
                }
                else  /*Updated T304 timer value in current procedure data of UE Context*/
                {
                    p_ue_context->p_curr_proc_data->m.T304_timer_rcvd_frm_rrm = RRC_TRUE;
                    p_ue_context->p_curr_proc_data->t304_timer = p_rrc_rrm_ue_contxt_mod_resp->t304_timer;
                }
            }
            /* CSR#59144 fix stop */

            /* If AMBR is present in UE Ctx Mod message, then change state 
               UE Entity reconfiguration at RLC and MAC*/
            if(p_ue_context->p_curr_proc_data->m.AggMaxBitRate)
            {
                UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_ue_context_mod_llim_reconfigure_ue_ongoing,
                        PNULL);
            }

            /* If Security Parameters are present in UE Ctx Mod message, then invoke 
               Intracell HO procedure for doing Security ReKeying */
        /*IT_FIX*/
            /* Trigger Intra Cell HO procedure for Security Rekeying if:
             * 1. SecurityKey is presnet OR
             * 2. UE Security Capability is present and at least one of the
             *    following three algorithms derived from UE Security Capability
             *    is different than already stored Algorithms:
             *    a) User Plane Ciphering Algorithm ID
             *    b) Ciphering Algorithm ID
             *    c) Integrity Protection Algorithm ID*/
            else if((p_ue_context->p_curr_proc_data->m.SecurityKey_present) ||
                    (
                     (p_ue_context->p_curr_proc_data->m.SecurityCapability_present) &&
                     (
                        (p_ue_context->p_curr_proc_data->userplane_ciphering_algo_id !=
                              p_ue_context->configure_ciphering_user_plane.algorithm_id) ||
                        (p_ue_context->p_curr_proc_data->ciphering_algo_id !=
                              p_ue_context->configure_ciphering.algorithm_id) ||
                        (p_ue_context->p_curr_proc_data->integrity_algo_id !=
                              p_ue_context->configure_integrity_protection.algorithm_id)
                      )
                     )
                   )
        /*IT_FIX*/
            {
                /* CSR#59144 fix start */
                rrc_build_handover_and_invoke_intracell_ho(p_ue_context, p_ue_context->p_curr_proc_data->t304_timer);
                /* CSR#59144 fix stop */
                break;
            }
            else
            {
                if (RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(&rrc_rrm_ue_contxt_mod_cnf,
                            RRC_UECC_MODULE_ID,
                            RRC_RRM_MODULE_ID,
                            uecc_ue_ctx_get_op_transction_id(p_ue_context),
                            p_ue_context->cell_index))
                {
                    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                            "[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf] Sending failed");
                }

                if (RRC_SUCCESS !=
                        uecc_s1ap_build_and_send_ue_context_modification_response(p_ue_context))
                {
                    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                            "[s1ap_UEContextModificationResponse]Delivery failed");
                }                   
                uecc_fsm_finish_activity_success(p_ue_context,
                        PNULL);
            }
            break;

        case RRC_FAILURE:

            /* Send UE Context Modfication Failure */

            if (RRC_SUCCESS != uecc_s1ap_build_s1ap_cause(
                        &cause,
                        &(p_rrc_rrm_ue_contxt_mod_resp->cause),
                        p_ue_context))
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                        "[s1ap_UEContextModificationFailure]invalid cause received from RRM");
            }

            if (RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_ue_context_modification_failure(
                        p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                        "[s1ap_UEContextModificationFailure]Delivery failed");
            }
            uecc_fsm_finish_activity_failure(p_ue_context, PNULL);
            uecc_event_queue_push_event(
                    UECC_EV_UE_CONTXT_MOD_FAIL_INT,
                    p_ue_context,
                    PNULL);

              break;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_erab_rel_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : ERAB REL IND handler for UE context modification
 *                  procedure
 *                  
 ****************************************************************************/
void uecc_fsm_ue_context_mod_erab_rel_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{

    rrc_rrm_erb_release_ind_t* rrc_rrm_erb_release_ind = PNULL;
    rrc_rrm_erb_release_cnf_t rrc_rrm_erb_release_cnf;
    U8     erab_index = 0;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /* Send ERAB_REL_CNF with failure to RRM */

    rrc_rrm_erb_release_ind = (rrc_rrm_erb_release_ind_t*)p_api;

    memset_wrapper (&rrc_rrm_erb_release_cnf, RRC_NULL, 
                          sizeof(rrc_rrm_erb_release_cnf_t));

    rrc_rrm_erb_release_cnf.bitmask = RRM_ERB_RELEASE_CNF_ERROR_LIST_PRESENT;
    rrc_rrm_erb_release_cnf.ue_index = rrc_rrm_erb_release_ind->ue_index;
    rrc_rrm_erb_release_cnf.response = RRC_FAILURE;


    rrc_rrm_erb_release_cnf.erab_error_list.erab_count =
           rrc_rrm_erb_release_ind->erab_to_be_released_item_list.count;

    for (erab_index = 0;
         erab_index < rrc_rrm_erb_release_ind->erab_to_be_released_item_list.count;
         erab_index++)
    {
        rrc_rrm_erb_release_cnf.erab_error_list.erab_error_info[erab_index].erab_id =
        rrc_rrm_erb_release_ind->erab_to_be_released_item_list.erab_to_be_release_item[erab_index].erab_id;

        rrc_rrm_erb_release_cnf.erab_error_list.erab_error_info[erab_index].error_code = RRC_NO_ERROR;
    }


    /* PUP encoding */
    if(RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_erb_release_cnf(
                       &rrc_rrm_erb_release_cnf,
                       RRC_UECC_MODULE_ID,
                       RRC_RRM_MODULE_ID,
                       uecc_ue_ctx_get_op_transction_id(p_ue_context),
                       p_ue_context->cell_index))
    {
       RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
       "[rrc_rrm_il_send_rrc_rrm_erb_release_cnf] Sending failed");
    }


    /*uecc_rrm_build_and_send_erb_release_conf(p_ue_context,RRC_FAILURE);*/
       
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_rrm_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_ue_context_mod_rrm_ongoing_timer_expiry_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
)
{
    s1ap_Cause cause;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /* SPR 15473 Fix Start */
    memset_wrapper(&cause, RRC_NULL, sizeof(s1ap_Cause)); 
    /* SPR 15473 Fix Stop */
    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_BRIEF, "Time out.");


    /*BUG 604 changes start*/
    if (RRC_TRUE == p_ue_context->ue_ctx_mod_security_flag)
    {
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_encryption_and_or_integrity_protection_algorithms_not_supported;
    }
    else
    {
    /*Sending UE Ctx Mod Cnf with FAILURE to RRM*/
    uecc_fsm_ue_ctx_mod_fail_action_send_rrm_cnf(p_ue_context, NULL);

    cause.t = T_s1ap_Cause_radioNetwork;
    /* SPR 15473 Fix Start */
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;
    /* SPR 15473 Fix Stop */
    }
    /*BUG 604 changes stop*/

    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_ue_context_modification_failure(
            p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
        "[s1ap_UEContextModificationFailure]Delivery failed");
    }
    uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

    uecc_event_queue_push_event(
                 UECC_EV_UE_CONTXT_MOD_FAIL_REL_UE_CONTXT_INT,
                 p_ue_context,
                 PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_rrc_re_establish_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_RE_ESTABLISH
 ****************************************************************************/
void uecc_fsm_ue_context_mod_rrc_re_establish_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
)
{
    s1ap_Cause cause;
    rrc_rrm_ue_contxt_mod_cnf_t      rrc_rrm_ue_contxt_mod_cnf ;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_BRIEF, 
            "Re-establishment triggered in UE context Modification");

    rrc_rrm_ue_contxt_mod_cnf.ue_index = p_ue_context->ue_index ;
    rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE ;
    
    if(RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(
                    &rrc_rrm_ue_contxt_mod_cnf,
                    RRC_UECC_MODULE_ID,
                    RRC_RRM_MODULE_ID,
                    uecc_ue_ctx_get_op_transction_id(p_ue_context),
                    p_ue_context->cell_index))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
        "[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf]Sending failed");
    }

    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_interaction_with_other_procedure;

    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_ue_context_modification_failure(
            p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
        "[s1ap_UEContextModificationFailure]Delivery failed");
    }

    uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

    uecc_event_queue_push_event(
            UECC_EV_RRC_RE_ESTABLISH_REQ_INT,
            p_ue_context,
            p_api_data);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_ho_required_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_HANDOVER
 ****************************************************************************/
void uecc_fsm_ue_context_mod_ho_required_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    s1ap_Cause cause;
    rrc_rrm_ue_contxt_mod_cnf_t      rrc_rrm_ue_contxt_mod_cnf ;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_BRIEF, 
            "Handover triggered in UE context Modification");


    /*BUG 604 changes start*/
    if ( (HANDOVER_TYPE_INTRA_CELL == ((rrc_rrm_ho_required_t *)p_api_data)->ho_type) &&
         (RRC_TRUE == p_ue_context->ue_ctx_mod_security_flag) )
    { 
        /* stop UE Context Mod Timer */
        if (PNULL !=
                p_ue_context->timers_data.timers_ids[UECC_UE_CONTXT_MOD_TIMER])
        {
            /* Stop UE Context Modification procedural timer. */
            uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_UE_CONTXT_MOD_TIMER,
                    &p_ue_context->timers_data);
        }
    
        /* Finish active procedure descriptor */
        uecc_fsm_finish_active_procedure(p_ue_context); 
    }
    else
    {


    rrc_rrm_ue_contxt_mod_cnf.ue_index = p_ue_context->ue_index ;
    rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE ;

    if(RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(
                    &rrc_rrm_ue_contxt_mod_cnf,
                    RRC_UECC_MODULE_ID,
                    RRC_RRM_MODULE_ID,
                    uecc_ue_ctx_get_op_transction_id(p_ue_context),
                    p_ue_context->cell_index))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
        "[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf] Sending failed");
    }

    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_interaction_with_other_procedure;

    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_ue_context_modification_failure(
            p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
        "[s1ap_UEContextModificationFailure]Delivery failed");
    }

    uecc_fsm_finish_activity_failure(p_ue_context, PNULL);
    }

    uecc_event_queue_push_event(
            /*BUG 604 changes start*/
            UECC_EV_RRM_HO_REQUIRED,
            /*BUG 604 changes stop*/
            p_ue_context,
            p_api_data);
    /*BUG 604 changes stop*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_radio_link_failure_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RADIO_LINK_FAILURE
 ****************************************************************************/
void uecc_fsm_ue_context_mod_radio_link_failure_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    s1ap_Cause cause;
    rrc_rrm_ue_contxt_mod_cnf_t      rrc_rrm_ue_contxt_mod_cnf ;
    rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE ;
   
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_BRIEF, 
            "Radio Link Failure triggered in UE context Modification");


    rrc_rrm_ue_contxt_mod_cnf.ue_index = p_ue_context->ue_index ;
    rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE ;

    if(RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(
			    &rrc_rrm_ue_contxt_mod_cnf,
			    RRC_UECC_MODULE_ID,
			    RRC_RRM_MODULE_ID,
			    uecc_ue_ctx_get_op_transction_id(p_ue_context),
                p_ue_context->cell_index))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
	"[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf]Sending failed ");
    }
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost;

    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_ue_context_modification_failure(
            p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
        "[s1ap_UEContextModificationFailure]Delivery failed");
    }

    uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

    /* Continue failure processing */
    uecc_event_queue_push_event(
            UECC_EV_RADIO_LINK_FAILURE_INT,
            p_ue_context,
            p_api_data);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR_18241_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_process_ue_ctx_mod_msg
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : curr_proc_data_t*   pointer to procedure data
 * Returns        : SUCCESS/FAILURE
 * Description    : Stores UE Context Msg data
 ****************************************************************************/
rrc_return_et uecc_fsm_process_ue_ctx_mod_msg(
        uecc_ue_context_t*  p_ue_context,
        curr_proc_data_t*   p_curr_proc_data,
        void*               p_api
        )
{
    rrc_return_et result = RRC_FAILURE;
    rrc_s1ap_ue_context_modification_request_t *p_msg=
        (rrc_s1ap_ue_context_modification_request_t*)p_api;
    LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH_STRUCT
                                        *p_event_ciph_integrity_algo = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    if(PNULL == p_ue_context->p_gb_context)
    {
        /* uecc_global_context is null */
        return RRC_FAILURE;
    }
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    memset_wrapper((void *) p_curr_proc_data, 0,
               sizeof(curr_proc_data_t));

    /* Store AMBR in UE Context for future processing */
    if(RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_AGG_MAX_BIT_RATE_PRESENT
             &  p_msg->bitmask)
    {    
        p_curr_proc_data->m.AggMaxBitRate = 1;
        p_curr_proc_data->ue_aggregate_maximum_bitrate.
        uEaggregateMaximumBitRateDL =
            p_msg->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateDL;

        p_curr_proc_data->ue_aggregate_maximum_bitrate.
        uEaggregateMaximumBitRateUL =
            p_msg->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateUL;
    }   

    /* Store Security Key in UE Context for future processing */
    if(RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_SECURITY_KEY_PRESENT
            & p_msg->bitmask)
    {
        /*securitykey*/
        p_curr_proc_data->m.SecurityKey_present = 1;
        asn1Copy_s1ap_SecurityKey(&p_ue_context->s1ap_asn1_ctx,
                &p_msg->security_key,
                &p_curr_proc_data->security_key);
       /*securitykey*/
    }
    if(RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_SUBSCRIBER_PROFILE_ID_FOR_RFP_PRESENT
            & p_msg->bitmask)
    {
        p_curr_proc_data->m.SpId = 1;
        p_curr_proc_data->spid = p_msg->subscriber_profile_id_for_rfp;
    }
    if(RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_CS_FALLBACK_INDICATOR_PRESENT
            & p_msg->bitmask)
    {
        p_curr_proc_data->m.CsfallbackIndicator = 1;
        p_curr_proc_data->cs_fallback_indicator =
            p_msg->cs_fallback_indicator;
    }

    /* Store UE Security Capabilities in UE Context for future processing */
    if(RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_UE_SECURITY_CAPABILITIES_PRESENT
            & p_msg->bitmask)
    {
        /*securitykey*/
        p_curr_proc_data->m.SecurityCapability_present = 1;

        /*IT_FIX*/
         /* Taking backup of currently configured ciphering and integrity
          * protection algorithms in current procedure data*/
         p_curr_proc_data->userplane_ciphering_algo_id = 
                        p_ue_context->configure_ciphering_user_plane.algorithm_id; 
         p_curr_proc_data->ciphering_algo_id = 
                        p_ue_context->configure_ciphering.algorithm_id;
         p_curr_proc_data->integrity_algo_id = 
                        p_ue_context->configure_integrity_protection.algorithm_id; 

         RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_BRIEF,
                 "Algorithm IDs Before UCM: [UP_CIPH|CIPH|INT_PROT]=[%d|%d|%d]",
                  p_ue_context->configure_ciphering_user_plane.algorithm_id,
                  p_ue_context->configure_ciphering.algorithm_id,
                  p_ue_context->configure_integrity_protection.algorithm_id);
        /*IT_FIX*/

        /*SPR_17484_START*/
        if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(p_ue_context->p_gb_context,
                    p_ue_context->cell_index))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING,"cell not active");

            p_ue_context->share_data.p_uecc_rm_data->cause.t =
                T_s1ap_Cause_radioNetwork;
            p_ue_context->share_data.p_uecc_rm_data->cause.u.radioNetwork =
                s1ap_cell_not_available;
            result = RRC_FAILURE;
            return result;
        }
        /*SPR_17484_END*/

            /* Check security capabilities */
        if(RRC_SUCCESS != uecc_fsm_icsp_check_and_process_security_capability(
                    p_ue_context,
                    (void*)(&p_msg->ue_security_capabilities)))
        {

            /* Generating Protocol Event
             * LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH*/
            p_event_ciph_integrity_algo = 
                rrc_mem_get(sizeof(LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH_STRUCT));
            if (p_event_ciph_integrity_algo)
            {
                /* SES-508 Fix Start */
                /* Code Removed */
                /* SES-508 Fix End */
                p_event_ciph_integrity_algo->header.length = 
                    sizeof(LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH_STRUCT);
                p_event_ciph_integrity_algo->header.event_id = 
                    LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH;

                if ((PNULL != p_ue_context->p_gb_context->p_p_csc_context) && 
                    (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index])
                    && (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                            p_csc_init_setup_ind))
                {
                    l3_memcpy_wrapper(&p_event_ciph_integrity_algo->header.EVENT_PARAM_CELL_ID, 
                            p_ue_context->p_gb_context->
                            p_p_csc_context[p_ue_context->cell_index]->
                            p_csc_init_setup_ind->sib_type_1_Info.
                            cell_access_related_info.cell_Id, sizeof(U32));
                }
                p_event_ciph_integrity_algo->header.EVENT_PARAM_EUTRANCELL_FROID = 
                    p_event_ciph_integrity_algo->header.EVENT_PARAM_CELL_ID;

                if (p_ue_context->m.mme_ue_id_present)
                {
                    p_event_ciph_integrity_algo->header.EVENT_PARAM_MMES1APID = p_ue_context->mme_ue_id;
                }

                p_event_ciph_integrity_algo->header.EVENT_PARAM_RAC_UE_REF = p_ue_context->s1ap_ue_id;

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_ciph_integrity_algo);
            }
            /*IT_FIX*/
            RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_BRIEF,
                    "Algorithm IDs After UCM: [UP_CIPH|CIPH|INT_PROT]=[%d|%d|%d]",
                    p_ue_context->configure_ciphering_user_plane.algorithm_id,
                    p_ue_context->configure_ciphering.algorithm_id,
                    p_ue_context->configure_integrity_protection.algorithm_id);
            /*IT_FIX*/
            return result;
        }
        /*IT_FIX*/
         RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_BRIEF,
                 "Algorithm IDs After UCM: [UP_CIPH|CIPH|INT_PROT]=[%d|%d|%d]",
                  p_ue_context->configure_ciphering_user_plane.algorithm_id,
                  p_ue_context->configure_ciphering.algorithm_id,
                  p_ue_context->configure_integrity_protection.algorithm_id);
        /*IT_FIX*/
 
        /* Fill UE security capabilities in UE context */
        asn1Copy_s1ap_EncryptionAlgorithms(&p_ue_context->s1ap_asn1_ctx,
                &p_msg->ue_security_capabilities.encryptionAlgorithms,
                &p_curr_proc_data->ue_security_capabilities.encryptionAlgorithms);
        asn1Copy_s1ap_IntegrityProtectionAlgorithms(&p_ue_context->s1ap_asn1_ctx,
                &p_msg->ue_security_capabilities.integrityProtectionAlgorithms,
                &p_curr_proc_data->ue_security_capabilities.integrityProtectionAlgorithms);
    }
    /*securitykey*/

    if(RRC_S1AP_UE_CONTEXT_MODIFICATION_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT
            & p_msg->bitmask)
    {
        p_curr_proc_data->m.CsgMembershipStatus = 1;
        p_curr_proc_data->csg_membership_status =
            p_msg->csg_membership_status;
    }



    /* CR_761_START */

    if(RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_REGISTERED_LAI_PRESENT
            & p_msg->bitmask)
    {
        //Memory Allocation
        p_curr_proc_data->p_registered_lai = 
            (registered_lai_t *)rrc_mem_get(sizeof(registered_lai_t));
        if (PNULL == p_curr_proc_data->p_registered_lai)
        {
            /* Memory Allocation Failed */
            RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_FATAL,
                    "Memory Allocation Failure");
            return result;
        }

        p_curr_proc_data->m.registered_lai_present = 1;

        p_curr_proc_data->p_registered_lai->plmn_identity.num = 
            p_msg->registered_lai.pLMNidentity.numocts;


        l3_memcpy_wrapper(p_curr_proc_data->p_registered_lai->plmn_identity.data,
                    p_msg->registered_lai.pLMNidentity.data,
                    p_msg->registered_lai.pLMNidentity.numocts);

        p_curr_proc_data->p_registered_lai->lac.num = 
             p_msg->registered_lai.lAC.numocts;
  
        l3_memcpy_wrapper(p_curr_proc_data->p_registered_lai->lac.data,
                    p_msg->registered_lai.lAC.data,
                    p_msg->registered_lai.lAC.numocts);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    result = RRC_SUCCESS;
    return result;
}
/*SPR_18241_END*/

/****************************************************************************
 * Function Name  : uecc_ue_context_mod_mme_p_failure_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : failure finish for UE CONTEXT MODIFICATION procedure
 ****************************************************************************/
void uecc_ue_context_mod_mme_p_failure_handler(
                uecc_ue_context_t*      p_ue_context,
                void*                   p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_CONTXT_MOD_TIMER,
            &p_ue_context->timers_data);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_ue_release_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handling for the event UECC_EV_UE_RELEASE_REQ 
 ****************************************************************************/
void uecc_fsm_ue_context_mod_ue_release_req_handler(
                uecc_ue_context_t*      p_ue_context,
                void*                   p_api
)
{
        rrc_rrm_ue_contxt_mod_cnf_t      rrc_rrm_ue_contxt_mod_cnf ;
        RRC_ASSERT(PNULL != p_ue_context);
        RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_UE_CONTXT_MOD_TIMER,
            &p_ue_context->timers_data);

        rrc_rrm_ue_contxt_mod_cnf.ue_index =
         p_ue_context->ue_index ;

        rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE ;

        if(RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(
                    &rrc_rrm_ue_contxt_mod_cnf,
                    RRC_UECC_MODULE_ID,
                    RRC_RRM_MODULE_ID,
                    uecc_ue_ctx_get_op_transction_id(p_ue_context),
                    p_ue_context->cell_index))
        {
            RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
            "rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf] Sending failed ");
        }   
        uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

       
        uecc_event_queue_push_event(
            UECC_EV_UE_RELEASE_REQ_INT,
            p_ue_context,
            p_api);
             
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_s1ap_reset_msg_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handling for the event UECC_EV_S1AP_RESET_MSG_IND 
 ****************************************************************************/
void uecc_fsm_ue_context_mod_s1ap_reset_msg_ind_handler(
                uecc_ue_context_t*      p_ue_context,
                void*                   p_api
)
{
    rrc_rrm_ue_contxt_mod_cnf_t      rrc_rrm_ue_contxt_mod_cnf ;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    rrc_rrm_ue_contxt_mod_cnf.ue_index =
        p_ue_context->ue_index ;

    rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE ;

    if(RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(
                &rrc_rrm_ue_contxt_mod_cnf,
                RRC_UECC_MODULE_ID,
                RRC_RRM_MODULE_ID,
                uecc_ue_ctx_get_op_transction_id(p_ue_context),
                p_ue_context->cell_index))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                "[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf]Sending failed");
    }

    uecc_fsm_finish_activity_failure(p_ue_context, PNULL);
    uecc_event_queue_push_event(
            UECC_EV_S1AP_RESET_MSG_INT,
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_csc_delete_all_ue_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handling for the event UECC_EV_CSC_DELETE_ALL_UE_REQ
 ****************************************************************************/
void uecc_fsm_ue_context_mod_csc_delete_all_ue_req_handler(
                uecc_ue_context_t*      p_ue_context,
                void*                   p_api
)
{

        rrc_rrm_ue_contxt_mod_cnf_t      rrc_rrm_ue_contxt_mod_cnf ;
        RRC_ASSERT(PNULL != p_ue_context);
        RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

        rrc_rrm_ue_contxt_mod_cnf.ue_index =
         p_ue_context->ue_index ;

        rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE ;

        if(RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(
                    &rrc_rrm_ue_contxt_mod_cnf,
                    RRC_UECC_MODULE_ID,
                    RRC_RRM_MODULE_ID,
                    uecc_ue_ctx_get_op_transction_id(p_ue_context),
                    p_ue_context->cell_index))
        {
            RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
            "[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf]Sending failed");
        }

        uecc_fsm_finish_activity_failure(p_ue_context, PNULL);
        uecc_event_queue_push_event(
            UECC_EV_CSC_DELETE_ALL_UE_REQ_INT,
            p_ue_context,
            p_api);

        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);


}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_ue_connection_rel_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handling for the event UECC_EV_RRM_UE_CONNECTION_RELEASE_IND
 ****************************************************************************/
void uecc_fsm_ue_context_mod_ue_connection_rel_handler(
                uecc_ue_context_t*      p_ue_context,
                void*                   p_api
)
{

        rrc_rrm_ue_contxt_mod_cnf_t      rrc_rrm_ue_contxt_mod_cnf ;
        RRC_ASSERT(PNULL != p_ue_context);
        RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

        rrc_rrm_ue_contxt_mod_cnf.ue_index =
         p_ue_context->ue_index ;

        rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE ;

        if(RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(
                    &rrc_rrm_ue_contxt_mod_cnf,
                    RRC_UECC_MODULE_ID,
                    RRC_RRM_MODULE_ID,
                    uecc_ue_ctx_get_op_transction_id(p_ue_context),
                    p_ue_context->cell_index))
        {
            RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
            "[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf]Sending failed");
        }

    /*Send UE CONTEXT MODIFICATION RESP with FAILURE to S1AP*/
    uecc_fsm_ue_ctx_mod_fail_action_send_fail_resp_to_s1ap(p_ue_context, NULL);

    uecc_fsm_finish_activity_failure(p_ue_context, PNULL);
    uecc_event_queue_push_event(
            UECC_EV_RRM_UE_CONNECTION_RELEASE_INT,
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_mod_llim_reconfigure_ue_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends UECC_LLIM_MODIFY_LC_REQ to LLIM for AMBR
 ****************************************************************************/
static void uecc_fsm_ue_context_mod_llim_reconfigure_ue_ongoing_init_handler(
        uecc_ue_context_t*      p_ue_context,
        void*                   p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send the UECC_LLIM_MODIFY_LC_REQ msg to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_modify_lc_req(p_ue_context))
    {
        /* Process failure according to selected failure actions list */
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                "[UECC_LLIM_MODIFY_LC_REQ] Delivery Failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_context_mod_llim_reconfigure_ue_resp_handler 
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_LLIM_RECONFIGURE_UE_RESP from LLIM
 *                 If Success(i.e. at least one drb is modified):
 ****************************************************************************/
static void uecc_fsm_ue_context_mod_llim_reconfigure_ue_resp_handler(
        uecc_ue_context_t*      p_ue_context,
        void*                   p_api
)
{
	/* coverity_fix_54691_start */

	rrc_return_et result = RRC_FAILURE;
	s1ap_Cause    cause;
	rrc_rrm_ue_contxt_mod_cnf_t      rrc_rrm_ue_contxt_mod_cnf; 
	rrc_uecc_llim_modify_lc_resp_t*    p_rrc_uecc_llim_modify_lc_resp = PNULL;
	uecc_fsm_fail_handler_f const*     p_fail_actions_list = 
		uecc_fsm_ue_ctx_mod_fail_type1_actions; 

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	memset_wrapper( &cause, 0, sizeof(cause));
	p_rrc_uecc_llim_modify_lc_resp = (rrc_uecc_llim_modify_lc_resp_t*)p_api;
	memset_wrapper(&rrc_rrm_ue_contxt_mod_cnf, 0x00, sizeof(rrc_rrm_ue_contxt_mod_cnf_t));
	rrc_rrm_ue_contxt_mod_cnf.response = RRC_SUCCESS;
	rrc_rrm_ue_contxt_mod_cnf.ue_index = p_ue_context->ue_index;
	do
	{
		/* Check whether UECC_EV_LLIM_RECONFIGURE_UE_RESP
		 * received with success, partial_success or failure */
		switch (p_rrc_uecc_llim_modify_lc_resp->response_code)
		{
			case SUCCESS:
				/*IT_FIX*/
				/* Trigger Intra Cell HO procedure for Security Rekeying if:
				 * 1. SecurityKey is presnet OR
				 * 2. UE Security Capability is present and at least one of the
				 *    following three algorithms derived from UE Security Capability
				 *    is different than already stored Algorithms:
				 *    a) User Plane Ciphering Algorithm ID
				 *    b) Ciphering Algorithm ID
				 *    c) Integrity Protection Algorithm ID*/
				if((p_ue_context->p_curr_proc_data->m.SecurityKey_present) ||
						(
						 (p_ue_context->p_curr_proc_data->m.SecurityCapability_present) &&
						 (
						  (p_ue_context->p_curr_proc_data->userplane_ciphering_algo_id !=
						   p_ue_context->configure_ciphering_user_plane.algorithm_id) ||
						  (p_ue_context->p_curr_proc_data->ciphering_algo_id !=
						   p_ue_context->configure_ciphering.algorithm_id) ||
						  (p_ue_context->p_curr_proc_data->integrity_algo_id !=
						   p_ue_context->configure_integrity_protection.algorithm_id)
						 )
						)
				  )
					/*IT_FIX*/
				{
					/* CSR#59144 fix start */
					rrc_build_handover_and_invoke_intracell_ho(p_ue_context, p_ue_context->p_curr_proc_data->t304_timer);
					/* CSR#59144 fix stop */
				}
				else
				{
					RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE (RRC_INFO,
							"LLIM has modified radio resources successfully for AMBR");
					result = RRC_SUCCESS;
				}
				break;

			case FAILURE:
			case PARTIAL_SUCCESS:
				RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE (RRC_ERROR,
						"Radio resources modification unsuccessful or partially successful at LLIM");
				/*Stop Procedure Timer */
				uecc_ue_timer_stop(p_ue_context->p_gb_context,
						UECC_UE_CONTXT_MOD_TIMER,
						&p_ue_context->timers_data);
				cause.t = T_s1ap_Cause_radioNetwork;
				cause.u.radioNetwork = s1ap_unspecified_2;
				p_fail_actions_list =
					uecc_fsm_ue_ctx_mod_fail_type1_actions;
				break;

			case L3_FATAL:
			default:
				RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE (RRC_FATAL,
						"[UECC_EV_LLIM_RECONFIGURE_UE_RESP] Invalid or L3_FATAL Response=%u",
						p_rrc_uecc_llim_modify_lc_resp->response_code);

				/* Unrecoverable error from LLIM: Release UE Context */
				p_fail_actions_list =
					uecc_fsm_ue_ctx_mod_fail_type2_actions;
		}
	}while(0);

	if(RRC_SUCCESS == result)
	{
		/* Update the UE AMBR in UE Context*/
		p_ue_context->ue_aggregate_maximum_bitrate =
			p_ue_context->p_curr_proc_data->ue_aggregate_maximum_bitrate;

		/*Send UE Context Modificatin Confirm to RRM with SUCCESS*/
		if (RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(&rrc_rrm_ue_contxt_mod_cnf,
					RRC_UECC_MODULE_ID,
					RRC_RRM_MODULE_ID,
					uecc_ue_ctx_get_op_transction_id(p_ue_context),
					p_ue_context->cell_index))
		{
			RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
					"[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf] Sending failed");
		}

		/*Send UE Context Modificatin Response to MME with SUCCESS*/
		if (RRC_SUCCESS !=
				uecc_s1ap_build_and_send_ue_context_modification_response(p_ue_context))
		{
			RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
					"[s1ap_UEContextModificationResponse]Delivery failed");
		}                   
		uecc_fsm_finish_activity_success(p_ue_context,
				PNULL);

	}
	else if ((RRC_SUCCESS != result ) && 
			(p_ue_context->ue_ctx_mod_security_flag != RRC_TRUE))
	{
		/* Process failure according to selected failure actions list */
		uecc_fsm_start_failure_processing(
				p_ue_context,
				p_fail_actions_list);
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* coverity_fix_54691_stop */

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_ctx_mod_fail_action_send_rrm_cnf 
 * INPUT         :uecc_ue_context_t*  p_ue_context,
 *                rrc_bool_et*        p_break
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : To send FAILURE in UE_CONTEXT_MOD_CNF message to RRM
 ****************************************************************************/
void uecc_fsm_ue_ctx_mod_fail_action_send_rrm_cnf(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
        )
{
    rrc_rrm_ue_contxt_mod_cnf_t rrc_rrm_ue_contxt_mod_cnf;

    RRC_ASSERT( PNULL != p_ue_context );
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    memset_wrapper(&rrc_rrm_ue_contxt_mod_cnf, 0x00, sizeof(rrc_rrm_ue_contxt_mod_cnf_t));
    rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE;
    rrc_rrm_ue_contxt_mod_cnf.ue_index = p_ue_context->ue_index;
    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_INFO, 
            "FAILURE: Send RRC_RRM_UE_CTX_MOD_CNF.");

    /* Send message to RRM RRC_RRM_UE_CTX_MOD_CNF(FAILURE) */
    if (RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(&rrc_rrm_ue_contxt_mod_cnf,
                RRC_UECC_MODULE_ID,
                RRC_RRM_MODULE_ID,
                uecc_ue_ctx_get_op_transction_id(p_ue_context),
                p_ue_context->cell_index))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                "[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf] Sending failed");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_ctx_mod_fail_action_send_fail_resp_to_s1ap 
 * INPUT         :uecc_ue_context_t*  p_ue_context,
 *                rrc_bool_et*        p_break
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : Sending UE CONTEXT MODIFICATION FAILURE TO S1AP when Failure 
 *                 is recieved from LLIM module
 ****************************************************************************/
void uecc_fsm_ue_ctx_mod_fail_action_send_fail_resp_to_s1ap(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
)
{
    s1ap_Cause cause; 
    RRC_ASSERT( PNULL != p_ue_context );
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.transport = s1ap_unspecified_2;
    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    /* Send the ASN. 1 UE_CTX_MOD RESPONSE message 
     * containing all failures to MME.*/
    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_INFO, 
            "FAILURE: Send RRC_S1AP_UE_CTX_MOD_RESP.");

    /* Build and send the RRC_S1AP_UE_CTX_MOD_Failure  msg to S1AP */
    if (RRC_SUCCESS !=
            uecc_s1ap_build_and_send_ue_context_modification_failure(
                p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                "[s1ap_UEContextModificationFailure]Delivery failed");
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_ctx_mod_fail_action_send_ue_context_release_req 
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 rrc_bool_et*        p_break
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : To send UE Ctx Release Req to MME in case FATAL is recd from
 *                 LLIM
 ****************************************************************************/
static void uecc_fsm_ue_ctx_mod_fail_action_send_ue_context_release_req(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
)
{
    s1ap_Cause cause; 
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_INFO, 
                      "FAILURE: Sending UE_CTX_REL_REQ to MME");
    /* Send release request to MME and
     * don't start UE Connection Release procedure */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.transport = s1ap_unspecified_2;

    if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
                p_ue_context,
                &cause ) )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                p_ue_context->p_gb_context->facility_name ,RRC_WARNING,
                "[s1ap_UEContextReleaseRequest] Delivery failed");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_ctx_mod_fail_action_last 
 * INPUT         :uecc_ue_context_t*  p_ue_context,
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This function relists the UE Context as the lower layers 
 *                 are completely out of sync now. UE context data is removed 
 *                 from all the layers
 ****************************************************************************/
static void uecc_fsm_ue_ctx_mod_fail_action_last(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_INFO, "FAILURE: END");

    /* This function relists the UE Context as the lower layers 
     * are completely out of sync now */
    /* The UE context data should be removed from all the layers */

    /* Notify procedure about failure */
    uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

    uecc_event_queue_push_event(
            UECC_EV_UE_CONTXT_MOD_FAIL_INT,
            p_ue_context,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : 
 *  uecc_fsm_ue_context_mod_llim_reconfig_ue_ongoing_radio_link_failure_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_LLIM_RADIO_LINK_FAILURE_IND. Switch to ROLLBACK state
 ****************************************************************************/

static void  uecc_fsm_ue_context_mod_llim_reconfig_ue_ongoing_radio_link_failure_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
)
{
    U16   len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Store RLF in UE Context */
    len = (U16)sizeof(rrc_uecc_llim_radio_link_failure_ind_t);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
                rrc_mem_get(len)))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_FATAL,
                "memory allocation failed to store received"
                "RLF msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* storing RLF contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
            p_api, len);

    /* Store type of received msg in UE Context */
    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RLF_MSG;

    /* Change the state to UECC_FSM_UE_CTX_MOD_ROLLBACK_ONGOING */
    UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_ue_context_mod_llim_reconfigure_ue_rollback_ongoing,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_context_mod_llim_reconfig_ue_ongoing_ho_required_handler
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_RRM_HO_REQUIRED. Buffer the HO REQUIRED 
 *                 message in UE Context. Start the HO PREP TIMER.
 *                 Change the state to UECC_LLIM_UE_CTX_MOD_ROLLBACK_ONGOING
 ****************************************************************************/
static void uecc_fsm_ue_context_mod_llim_reconfig_ue_ongoing_ho_required_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
)
{
    U16     len = RRC_NULL;
    U8      result = RRC_FALSE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* Buffer the HO REQUIRED message in UE Context. */
    len = (U16)sizeof(rrc_rrm_ho_required_t);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
                rrc_mem_get(len))) 
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_FATAL,
                "memory allocation failed to store received "
                "RRC_RRM_HO_REQUIRED");

        /* SPR 24021/SES-452 Fix Start */
        if(PNULL != p_api)
        {
            rrc_mem_free(p_api);
            p_api = PNULL;
        }
        /* SPR 24021/SES-452 Fix End */
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* storing rrc_rrm_ho_required contents into msg_bfr_rcvd */
    /* SPR 24021/SES-452 Coverity 209449 Fix Start */
    if(PNULL != p_api)
    {
        l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
                p_api, len);

        /* store received buffer type */
        p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_HO_REQUIRED_MSG;

        /* Start the HO PREP TIMER */
        /*x2_ho_timer*/
        if (HANDOVER_TYPE_INTRA_LTE_X2 ==
                ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = X2_HO;

            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context, 
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] X2_HO Prep Timer start Failure");
            }
            else
            {
                result = RRC_TRUE;
            }
        }
        else if(HANDOVER_TYPE_INTER_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = INTER_CELL_HO;

            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context, 
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] INTER_CELL_HO Prep Timer start Failure");
            }
            else
            {
                result = RRC_TRUE;
            }
        }
        else
        {
            p_ue_context->ho_info.s1_or_x2_handover = S1_HO;

            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context, 
                        UECC_UE_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_HO_PREP_TIMER] HO Prep Timer start Failure");
            }
            else
            {
                result = RRC_TRUE;
            }
        }

        if(RRC_TRUE == result)
        {
            /* Switch state */
            UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_ue_context_mod_llim_reconfigure_ue_rollback_ongoing,
                    PNULL);
        }
        /* SPR 24021/SES-452 Fix Start */
        rrc_mem_free(p_api);
        p_api = PNULL;
        /* SPR 24021/SES-452 Fix End */
    }
    /* SPR 24021/SES-452 Coverity 209449 Fix End */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME :  uecc_fsm_ue_context_mod_llim_reconfig_ue_rrc_reestablish_request_handler 
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_RRC_RE_ESTABLISH_REQ. Buffered Received
 *                 Re-establishment Msg.  Change the state to
 *                 UECC_LLIM_UE_CTX_MOD_ROLLBACK_ONGOING
 ****************************************************************************/
static void uecc_fsm_ue_context_mod_llim_reconfig_ue_rrc_reestablish_request_handler( 
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
        )
{
    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Buffered Received Re-establishment Msg */
    len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
                rrc_mem_get(len)))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_FATAL,
                "Memory Allocation Failed to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* Storing re_establishment contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
            p_api,len);

    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RE_ESTABLISH_MSG;

    /* Change the state to UECC_LLIM_UE_CTX_MOD_ROLLBACK_ONGOING */
    UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_ue_context_mod_llim_reconfigure_ue_rollback_ongoing,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_init_handler 
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_STATE_INIT                              
 ****************************************************************************/
static void uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_init_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* Build and send the UECC_LLIM_MODIFY_ROLLBACK_REQ msg to LLIM */
    if (RRC_SUCCESS != 
            uecc_llim_build_and_send_modify_rollback_req(p_ue_context))
    {
        /* Here, the UE context must be released!! */
        /* Process failure according to selected failure actions list */

        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_ue_ctx_mod_fail_type2_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_resp_handler 
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_STATE_INIT                              
 ****************************************************************************/
static void uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
        )
{
    rrc_return_et rrc_response = RRC_FAILURE;
    s1ap_Cause    cause;

    rrc_uecc_llim_modify_rollback_resp_t* p_rrc_uecc_llim_modify_rollback_resp;
    rrc_rrm_ue_contxt_mod_cnf_t      rrc_rrm_ue_contxt_mod_cnf ;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    rrc_response = RRC_FAILURE;

    p_rrc_uecc_llim_modify_rollback_resp = PNULL;
    p_rrc_uecc_llim_modify_rollback_resp = 
        (rrc_uecc_llim_modify_rollback_resp_t*)p_api;
    memset_wrapper(&rrc_rrm_ue_contxt_mod_cnf, 0x00, sizeof(rrc_rrm_ue_contxt_mod_cnf_t));
    rrc_rrm_ue_contxt_mod_cnf.ue_index = p_ue_context->ue_index;


    memset_wrapper(&cause, 0, sizeof(s1ap_Cause));    

    /* Stop Procedure Timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_CONTXT_MOD_TIMER, 
            &p_ue_context->timers_data);

    switch (p_rrc_uecc_llim_modify_rollback_resp->response)
    {
        case RRC_SUCCESS:
            rrc_response = RRC_SUCCESS;
            if (p_ue_context->p_curr_proc_data->msg_bfr_type != RRC_NO_MSG)
            {
                /* Start the Procedure as per the buffered Message */
                switch(p_ue_context->p_curr_proc_data->msg_bfr_type)
                {
                    /* Post Internal event as per the buffered message type*/
                    case RRC_RE_ESTABLISH_MSG:
                        cause.t = T_s1ap_Cause_radioNetwork;
                        cause.u.radioNetwork = 
                            s1ap_interaction_with_other_procedure;
                        break;
                    case RRC_HO_REQUIRED_MSG:
                        cause.t = T_s1ap_Cause_radioNetwork;
                        if ( (HANDOVER_TYPE_INTRA_LTE_S1 ==
                                    ((rrc_rrm_ho_required_t*)p_ue_context->
                                     p_curr_proc_data->msg_bfr_rcvd)->ho_type) ||
                                (HANDOVER_TYPE_INTRA_LTE_X2 ==
                                 ((rrc_rrm_ho_required_t*)p_ue_context->
                                  p_curr_proc_data->msg_bfr_rcvd)->ho_type) ||
                                (HANDOVER_TYPE_INTER_CELL == ((rrc_rrm_ho_required_t*)p_ue_context->
                                                              p_curr_proc_data->msg_bfr_rcvd)->ho_type))
                        {
                            cause.u.radioNetwork =
                                s1ap_s1_intra_system_handover_triggered;
                        }
                        else
                        {
                            cause.u.radioNetwork =
                                s1ap_s1_inter_system_handover_triggered;
                        }
                        break;
                    case RRC_RLF_MSG:
                        cause.t = T_s1ap_Cause_radioNetwork;
                        cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost;
                        break;
                    default:
                        /* Error*/
                        rrc_response = RRC_FAILURE;
                        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_INFO, 
                                "Unrecognized Message Encountered");
                        break;
                }
            }
            /*securitykey*/
            else
            {
                cause.t = T_s1ap_Cause_radioNetwork;
                cause.u.radioNetwork = 
                    s1ap_interaction_with_other_procedure;
            }
            /*securitykey*/
            break;

        case RRC_FAILURE:
            if (p_ue_context->p_curr_proc_data->msg_bfr_type != RRC_NO_MSG)
            {
                switch (p_ue_context->p_curr_proc_data->msg_bfr_type)
                {
                    case RRC_HO_REQUIRED_MSG:
                        cause.t = T_s1ap_Cause_radioNetwork;
                        cause.u.radioNetwork = 
                            s1ap_interaction_with_other_procedure;

                        /*x2_ho_timer*/
                        /* Stop Procedure Timer */
                        if ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) )
                        {
                            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                                    UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
                            p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
                        }
                        else
                        {
                            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                                    UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
                            p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
                        }


                        /* SEND HO FAILURE TO RRM */
                        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                        p_ue_context->ho_info.cause.u.radioNetwork =
                            s1ap_unspecified_2;

                        /* Send the HO Failure Message to RRM */
                        uecc_rrm_build_and_send_ho_failure(p_ue_context);

                        break;
                    case RRC_RE_ESTABLISH_MSG:
                        cause.t = T_s1ap_Cause_radioNetwork;
                        cause.u.radioNetwork = 
                            s1ap_interaction_with_other_procedure;
                        break;
                    case RRC_RLF_MSG:
                        cause.t = T_s1ap_Cause_radioNetwork;
                        cause.u.radioNetwork = 
                            s1ap_radio_connection_with_ue_lost;
                        break;
                    default:
                        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                                "Invalid Error Code Received");
                        break;
                }
            }
            /*securitykey*/
            else
            {
                cause.t = T_s1ap_Cause_radioNetwork;
                cause.u.radioNetwork = 
                    s1ap_interaction_with_other_procedure;
            }
            /*securitykey*/
            break;
        default:
            /* Error*/
            RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_BRIEF, 
                    "Unrecognized Response Received");
            break;
    }

    /* Send message to RRM RRC_RRM_UE_CTX_MOD_CNF(FAILURE) */

    rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE;
    if (RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(&rrc_rrm_ue_contxt_mod_cnf,
                RRC_UECC_MODULE_ID,
                RRC_RRM_MODULE_ID,
                uecc_ue_ctx_get_op_transction_id(p_ue_context),
                p_ue_context->cell_index))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                "[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf] Sending failed");
    }
    /* Build and send the RRC_S1AP_UE_CTX_MOD_Failure  msg to S1AP */
    if (RRC_SUCCESS !=
            uecc_s1ap_build_and_send_ue_context_modification_failure(
                p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                "[s1ap_UEContextModificationFailure]Delivery failed");
    }

    if (rrc_response == RRC_SUCCESS)
    {
        /* Notify procedure about failure */
        uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

        uecc_event_queue_push_event(
                UECC_EV_UE_CONTXT_MOD_FAIL_INT,
                p_ue_context,
                p_api);
    }
    else
    {
        if(p_ue_context->p_curr_proc_data->msg_bfr_type ==
                RRC_RE_ESTABLISH_MSG)
        {
            /* Send RRC-Connection Reject to UE
             *              * */
            if (RRC_SUCCESS !=
                    uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                        p_ue_context->p_gb_context , 
                        p_ue_context->crnti, otherFailure,
                        p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_ERROR,
                        "Fail to send the RRCConnectionReestablishmentReject to UE");
            }
            else
            {
                RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_INFO,
                        "RRCConnectionReestablishmentReject Send to UE Successfully");
            }
            /* enabling flag */
            SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);
        }

        /* Notify procedure about failure */
        uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

        uecc_event_queue_push_event(
                UECC_EV_UE_CONTXT_MOD_FAIL_REL_UE_CONTXT_INT,
                p_ue_context,
                p_api);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : 
 uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_rrc_re_establish_request_handler 
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_RRC_RE_ESTABLISH_REQ                             
 ****************************************************************************/
static void 
uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_rrc_re_establish_request_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
        )
{
    U16     len = RRC_NULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (p_ue_context->p_curr_proc_data->msg_bfr_type != RRC_NO_MSG)
    {
        /* Start the Procedure as per the buffered Message */
        switch(p_ue_context->p_curr_proc_data->msg_bfr_type)
        {
            case RRC_HO_REQUIRED_MSG:

                /*x2_ho_timer*/
                /* Stop Procedure Timer */
                if ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                        (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ) 
                {
                    uecc_ue_timer_stop(p_ue_context->p_gb_context,
                            UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
                    p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
                }
                else
                {
                    uecc_ue_timer_stop(p_ue_context->p_gb_context,
                            UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
                    p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
                }

                /* SEND HO FAILURE TO RRM */
                p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
                p_ue_context->ho_info.cause.u.radioNetwork = 
                    s1ap_interaction_with_other_procedure;

                /* Send the HO Failure Message to RRM */
                uecc_rrm_build_and_send_ho_failure(p_ue_context);

                if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
                {
                    /* Free memory for RRC_HO_REQUIRED_MSG msg */
                    rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
                }
                break;
            case RRC_RLF_MSG:

                if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
                {
                    /* Freed buffered RLF message*/
                    rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
                }
                break;
            default:
                if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
                {
                    rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
                }
                break;
        }
    }
    /* Store Re-establishment Req msg to Ue in Context */
    len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);

    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
                rrc_mem_get(len)))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_FATAL, 
                "Memory Allocation Failed to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* storing re_establishment contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
            p_api,len);

    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RE_ESTABLISH_MSG;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME :
 uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_rrm_ho_required_ind_handler 
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_RRM_HO_REQUIRED                             
 ****************************************************************************/
static void 
uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_rrm_ho_required_ind_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
        )
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (RRC_HO_REQUIRED_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type) 
    {
        /* Discard this HO message, already have a message in context */
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_INFO,
                "HO MSG Recv: Discarding b/c HO Msg already present in UE Ctx;Unused variable p_api=%p",p_api);/*SPR 17777 +-*/
    }
    else if ((RRC_RE_ESTABLISH_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type) || 
            (RRC_RLF_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type))
    {
        /* SEND HO FAILURE TO RRM */
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = 
            s1ap_interaction_with_other_procedure;

        /* Send the HO Failure Message to RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);
    }
    /* SPR 24021/SES-452 Fix Start */
    if(PNULL != p_api)
    {
        rrc_mem_free(p_api);
        p_api = PNULL;
    }
    /* SPR 24021/SES-452 Fix End */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME :
 uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_timer_expiry_handler 
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_TIMER_EXPIRY                      
 ****************************************************************************/
static void 
uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_timer_expiry_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
        )
{
    rrc_rrm_ue_contxt_mod_cnf_t      rrc_rrm_ue_contxt_mod_cnf ;
    s1ap_Cause    cause;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    memset_wrapper(&rrc_rrm_ue_contxt_mod_cnf, 0x00, sizeof(rrc_rrm_ue_contxt_mod_cnf_t));
    rrc_rrm_ue_contxt_mod_cnf.ue_index = p_ue_context->ue_index;

    RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
    {
        /*x2_ho_timer*/
        /* Stop Procedure Timer */
        if ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ) 
        {
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
            p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
        }
        else
        {
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
            p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
        }
        /* SEND HO FAILURE TO RRM */
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = 
            s1ap_interaction_with_other_procedure;

        /* Send the HO Failure Message to RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);

        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            /* Discard the HO message*/
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
    }
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_unspecified_2;

    /* Build and send RRC_RRM_UE_CTX_MOD_CNF with failure to RRM */
    rrc_rrm_ue_contxt_mod_cnf.response = RRC_FAILURE;
    if (RRC_SUCCESS != rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf(&rrc_rrm_ue_contxt_mod_cnf,
                RRC_UECC_MODULE_ID,
                RRC_RRM_MODULE_ID,
                uecc_ue_ctx_get_op_transction_id(p_ue_context),
                p_ue_context->cell_index))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                "[rrc_rrm_il_send_rrc_rrm_ue_contxt_mod_cnf] Sending failed");
    }
    /* Build and send the RRC_S1AP_UE_CTX_MOD_Failure  msg to S1AP */
    if (RRC_SUCCESS !=
            uecc_s1ap_build_and_send_ue_context_modification_failure(
                p_ue_context->p_gb_context, p_ue_context,RRC_NULL,RRC_NULL,&cause,PNULL))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_WARNING,
                "[s1ap_UEContextModificationFailure]Delivery failed");
    }

    uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

    uecc_event_queue_push_event(
            UECC_EV_UE_CONTXT_MOD_FAIL_REL_UE_CONTXT_INT,
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME :
 uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_radio_link_failure_handler 
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the handler function for the message
 *                 for Radio LinkFailure                         
 ****************************************************************************/
static void 
uecc_fsm_ue_context_mod_llim_reconfig_ue_rollback_ongoing_radio_link_failure_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api
        )
{
    U16   len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Due to RRC Internal cause rollback in ongoing */
    if (p_ue_context->p_curr_proc_data->msg_bfr_type == 
            RRC_RE_ESTABLISH_MSG)
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_DETAILED,
                "Release buffered Re-establishment Req msg");

        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            /* freed buffered re-establishment req msg */
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
    }
    else if (p_ue_context->p_curr_proc_data->msg_bfr_type == 
            RRC_HO_REQUIRED_MSG)
    {
        /*x2_ho_timer*/
        /* Stop Procedure Timer */
        if ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ) 
        {
            uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                    UECC_UE_X2_HO_PREP_TIMER, &p_ue_context->timers_data);
            p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
        }
        else
        {
            uecc_ue_timer_stop(p_ue_context->p_gb_context,
                    UECC_UE_HO_PREP_TIMER, &p_ue_context->timers_data);
            p_ue_context->ho_info.s1_or_x2_handover = INVALID_HO;
        }
        /* SEND HO FAILURE TO RRM */
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = 
            s1ap_interaction_with_other_procedure;

        /* Send the HO Failure Message to RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);

        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            /* Free Buffered HO message*/
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
    }
    else
    {
        /* Release any buffered msg */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
    }

    len = (U16)sizeof(rrc_uecc_llim_radio_link_failure_ind_t);
    /* Store RRC_RLF_MSG msg to Ue in Context */
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
                rrc_mem_get(len)))
    {
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_FATAL,
                "memory allocation failed to store "
                "received RLF msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* storing RRC_RLF_MSG contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
            p_api,len);

    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RLF_MSG;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/*securitykey*/
/****************************************************************************
 * FUNCTION NAME : rrc_build_handover_and_invoke_intracell_ho
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 * OUTPUT        : None
 * RETURNS       : None
 * DESCRIPTION   : This is the fuction to build rrc_rrm_ho_required and trigger
 *                 INTRA-CELL HO procedure.
 *                 The Intra Cell HO procedure is trigerred by locally building
 *                 HO_REQUIRD message without involving RRM.
 ****************************************************************************/
static void  rrc_build_handover_and_invoke_intracell_ho(
   uecc_ue_context_t   *p_ue_context,
   /* CSR#59144 fix start */
   U16                 t304_timer
   /* CSR#59144 fix stop */
   )
 
{
    rrc_rrm_ho_required_t* p_rrc_rrm_ho_required = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_rrm_ho_required = (rrc_rrm_ho_required_t*)rrc_mem_get(sizeof(rrc_rrm_ho_required_t));

    if (PNULL == p_rrc_rrm_ho_required)
    {
        /* Memory Allocation Failed */
        RRC_UECC_UE_CONTEXT_MOD_FSM_TRACE(RRC_FATAL,
                "Memory Allocation Failure");
        return;
    }

    /* SES-508 Fix Start */
    /* Code Removed */
    /* SES-508 Fix End */

    /* stop timer if it has been started */
    if (PNULL !=
            p_ue_context->timers_data.timers_ids[UECC_UE_CONTXT_MOD_TIMER])
    {
        /* Stop UE Context Modification procedural timer. */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_UE_CONTXT_MOD_TIMER,
                &p_ue_context->timers_data);
    }

    /*Save the ongoing UE FSM Context to restore it back after
     * Intra Cell HO for Security Rekying has completed
     **/
    uecc_fsm_store_ongoing_active_procedure(p_ue_context);

    /* Finish active procedure descriptor */
    uecc_fsm_finish_active_procedure(p_ue_context);

    /* Set the flag, which if TURE will indicate that Intra-Cell HO procedure
     * for Security Rekeying was originally invoked from UE Ctx Mod procedure*/
    p_ue_context->ue_ctx_mod_security_flag = RRC_TRUE;

    /*Build ho_required (Intra-Cell HO) msg. Here RRC-RRM message is populated
     * locally at L3 without involving RRM because RRM has is not affected with
     * Security Rekeying */
    p_rrc_rrm_ho_required->bitmask |= RRC_RRM_UE_HO_ADM_REQ_MOBILITY_CONTROL_INFO_PRESENT;
    p_rrc_rrm_ho_required->ue_index = p_ue_context->ue_index;
    p_rrc_rrm_ho_required->ho_type = HANDOVER_TYPE_INTRA_CELL;
    p_rrc_rrm_ho_required->cause.type = T_s1ap_Cause_radioNetwork;
    p_rrc_rrm_ho_required->cause.value = s1ap_handover_desirable_for_radio_reason;
    /*p_rrc_rrm_ho_required->target_id = */  /*not required in intra-cell HO case*/

    /* CSR#59144 fix start */
    /*p_rrc_rrm_ho_required->mobility_control_info.t304_timer = 0;*/
    p_rrc_rrm_ho_required->mobility_control_info.t304_timer = t304_timer;

    /* Reset the T304 flag as it's no more required */
     p_ue_context->p_curr_proc_data->m.T304_timer_rcvd_frm_rrm = RRC_FALSE;
    /* CSR#59144 fix stop */

    /* Push event to mainfsm to call Intra-Cell HO for security rekeying procedure */  
    uecc_event_queue_push_event(
            UECC_EV_HO_REQUIRED_INT,
            p_ue_context,
            p_rrc_rrm_ho_required);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*securitykey*/



