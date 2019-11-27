/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 ****************************************************************************
 *
 *  File Description : UECC ERB RELEASE FSM functions implementation
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/


/****************************************************************************
* Header File Includes
***************************************************************************/
#include "rrc_rrm_intf.h"
#include "uecc_logger.h"

#include "uecc_common.h"
#include "uecc_ue_timer_utils.h"
#include "uecc_rrm.h"
#include "uecc_llim.h"
#include "uecc_s1ap.h"
#include "uecc_defaults.h"
#include "uecc_fsm_engine.h"
#include "uecc_erab_delfsm.h"
#include "rrc_shared_stats.h"
#include "rrc_mac_intf.h"
#ifdef ENDC_ENABLED
/*EN-DC_changes_start*/
#include "uecc_x2ap.h"
#include "uecc_mainfsm.h"
/*EN-DC_changes_end*/
#endif

#include "uecc_rrm_erab_relfsm.h"
/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_ERB_RELEASE_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,\
        p_ue_context->ue_index,\
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
        ##__VA_ARGS__)

/* SPR:5471 start */
/******************************************************************************
 Private Definitions
 *****************************************************************************/
#define RRC_IS_GBR_BEARER(qci) ((qci >= 1 && qci <= 4) ? 1 :0)
/* SPR:5471 stop */
/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static void uecc_fsm_ue_erb_release_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_erb_release_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_erb_release_procedure_finish_fail(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_release_csc_delete_all_ue_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_release_rrm_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_release_ue_context_mod_req_discard_handler(
    uecc_ue_context_t*  p_ue_context,         /* UECC UE context */
    void*               p_api                 /* Input API buffer */ 
);
static void uecc_fsm_erb_release_erab_rel_ind_discard_handler(
    uecc_ue_context_t*  p_ue_context,         /* UECC UE context */
    void*               p_api                 /* Input API buffer */ 
);
/*SPR 18758 Fix Start*/
/*Code Deleted*/
/*SPR 18758 Fix Start*/
static void update_erab_release_list_with_cause(
    uecc_ue_context_t* p_ue_context,           /* UECC UE context */
    void*              p_cause                /* Input API buffer */
);
static void append_erab_release_list_to_erab_failed_to_release_list(
    uecc_ue_context_t* p_ue_context             /* UECC UE context */
);
static void update_curr_proc_data_with_response_code(
    uecc_ue_context_t* p_ue_context,           /* UECC UE context */
    void*              p_response_code            /* Input API buffer */
);

/* State UECC_ERB_RELEASE_RRM_RESOURCE_DELETION_ONGOING */
static void uecc_fsm_erb_release_rrm_resource_deletion_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_release_rrm_erb_release_resp_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

/* SPR:5471 start */
rrc_return_t uecc_fsm_erb_release_is_atleast_one_non_gbr_bearer_present(
    uecc_ue_context_t*   p_uecc_ue_context,
    erab_list_t*         p_erab_list
    );
/* SPR:5471 stop */
static rrc_return_t uecc_fsm_erb_release_process_e_rab_release_command_msg(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_erb_release_ongoing_ho_required_handler(
        uecc_ue_context_t*  p_ue_context,
          void*               p_api
        );

#ifdef ENDC_ENABLED
/*EN-DC_changes_start*/
static void uecc_fsm_sgnb_rel_req_handler(
   uecc_ue_context_t              *p_ue_context,
   void                           *p_api);
static void uecc_fsm_sgnb_release_ack_handler(
   uecc_ue_context_t              *p_ue_context,
   void                           *p_api);
static void uecc_fsm_sgnb_rel_ongoing_timer_expiry_handler(
   uecc_ue_context_t              *p_ue_context,
   void                           *p_api);
/*EN-DC_changes_end*/
/*Error_Handling_changes*/
static void uecc_fsm_sgnb_rel_error_handler(
   uecc_ue_context_t              *p_ue_context,
   void                           *p_api);
/*Error_Handling_changes*/
#endif

/****************************************************************************
 Private Variables
****************************************************************************/
static uecc_fsm_handler_record_t
    uecc_fsm_ue_erb_release_procedure_mht[] =
{
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND        },
            uecc_fsm_erb_release_rrm_ue_release_req_handler },
    /*SPR 18758 Fix Start*/
    /* Code Deleted */
    /*SPR 18758 Fix Stop*/
    { { UECC_EV_RRM_ERAB_RELEASE_IND          },
            uecc_fsm_erb_release_erab_rel_ind_discard_handler },
    { { UECC_EV_S1AP_UE_CONTEXT_MOD_REQ        },
            uecc_fsm_erb_release_ue_context_mod_req_discard_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ          },
            uecc_fsm_erb_release_csc_delete_all_ue_req_handler },
    { { UECC_EV_RRM_HO_REQUIRED},
     uecc_fsm_erb_release_ongoing_ho_required_handler},
    { { UECC_LAST_ID                      }, PNULL    }
};

static uecc_fsm_handler_record_t 
       uecc_fsm_erb_release_rrm_resource_deletion_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
            uecc_fsm_erb_release_rrm_resource_deletion_ongoing_init_handler  },
    { { UECC_EV_RRM_ERB_RELEASE_RESP   },
            uecc_fsm_erb_release_rrm_erb_release_resp_handler                },
    { { UECC_EV_S1AP_RESET_MSG_IND              },
            uecc_fsm_rrm_resource_deletion_ongoing_s1ap_reset_handler},
    { { UECC_EV_UE_RELEASE_REQ            },
            uecc_fsm_rrm_resource_deletion_ongoing_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND            },
            uecc_fsm_rrm_resource_deletion_ongoing_radio_link_failure_handler },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
  uecc_fsm_erb_release_rrm_resource_deletion_ongoing_re_establish_req_handler},
    { { UECC_EV_TIMER_EXPIRY            },
      uecc_fsm_erb_release_rrm_resource_deletion_ongoing_timer_expiry_handler },
    { { UECC_LAST_ID                    },  PNULL                       }
};

static uecc_fsm_handler_record_t 
       uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                  },
            uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_init_handler   },
    { { UECC_EV_LLIM_RECONFIGURE_UE_RESP  },
            uecc_fsm_erb_release_llim_reconfigure_ue_resp_handler           },
    { { UECC_EV_S1AP_RESET_MSG_IND              },
            uecc_fsm_erb_release_s1ap_reset_handler },
   { { UECC_EV_UE_RELEASE_REQ            },
            uecc_fsm_erb_release_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND            },
      uecc_fsm_erb_release_ongoing_radio_link_failure_handler },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
    uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_re_establish_req_handler},
    { { UECC_EV_TIMER_EXPIRY                },
      uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_timer_expiry_handler },
    { { UECC_LAST_ID                        },  PNULL                   }
};

static uecc_fsm_handler_record_t
        uecc_fsm_erb_release_ue_reconfigure_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
            uecc_fsm_erb_release_ue_reconfigure_ongoing_init_handler   },
    { { UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE   },
      uecc_fsm_erb_release_rrc_connection_reconfiguration_complete_handler  },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
      uecc_fsm_erb_release_ongoing_re_establish_req_handler },
    { { UECC_EV_S1AP_RESET_MSG_IND              },
            uecc_fsm_erb_release_s1ap_reset_handler },
    { { UECC_EV_UE_RELEASE_REQ            },
            uecc_fsm_erb_release_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND            },
      uecc_fsm_erb_release_ongoing_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY                    },
      uecc_fsm_erb_release_ue_reconfigure_ongoing_timer_expiry_handler },
    { { UECC_LAST_ID                            },  PNULL                   }
};

/* Activity states */
static uecc_fsm_activity_state_t 
       uecc_fsm_erb_release_rrm_resource_deletion_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_RRM_ERB_RELEASE_RESOURCE_DELETION_ONGOING",
   /* .s_id           =  */ (S8*)"ERAB_REL_RRM",
   /* .is_need_init   =  */ RRC_TRUE,
 /* .p_mht     =  */ uecc_fsm_erb_release_rrm_resource_deletion_ongoing_mht
};

static uecc_fsm_activity_state_t 
       uecc_fsm_erb_release_llim_reconfigure_ue_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_LLIM_ERB_RELEASE_RECONFIGURE_UE_ONGOING",
   /* .s_id           =  */ (S8*)"ERAB_REL_LLIM",
   /* .is_need_init   =  */ RRC_TRUE,
 /* .p_mht          =  */ uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_erb_release_ue_reconfigure_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_UE_ERB_RELEASE_RECONFIGURE_ONGOING",
   /* .s_id           =  */ (S8*)"ERAB_REL_RRC",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_erb_release_ue_reconfigure_ongoing_mht
};


/* Activity */
static uecc_fsm_activity_t uecc_fsm_erb_release_activity =
{
  /*  .s_name             = */(S8*)"RRC Connection Reconfiguration",
   /* .s_id               = */(S8*)"UE_RCRECONF_A",
/* .p_first_state      = */&uecc_fsm_erb_release_rrm_resource_deletion_ongoing
};

static uecc_fsm_activity_t*
                    uecc_fsm_ue_erb_release_procedure_activity_list[] =
{
    &uecc_fsm_erb_release_activity,
    PNULL
};

/* Procedure */
uecc_fsm_procedure_t uecc_fsm_ue_erb_release_procedure =
{
    /*.s_name             = */(S8*)"ERAB Release CMD",
    /*.s_id               = */(S8*)"ERAB_REL_CMD_P",
    /*.constructor_f      = */uecc_fsm_ue_erb_release_procedure_constructor,
    /*.finish_success_f   = */uecc_fsm_ue_erb_release_procedure_finish_success,
    /*.finish_fail_f      = */uecc_fsm_ue_erb_release_procedure_finish_fail,
    /*.activity_list      = */uecc_fsm_ue_erb_release_procedure_activity_list,
    /*.p_mht              = */uecc_fsm_ue_erb_release_procedure_mht
};

#ifdef ENDC_ENABLED
/*EN-DC_changes_start*/
static uecc_fsm_handler_record_t uecc_fsm_sgnb_release_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT               },
        uecc_fsm_sgnb_rel_req_handler           },
    { { UECC_EV_X2AP_SGNB_RELEASE_REL_ACK      },
        uecc_fsm_sgnb_release_ack_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND            },
        uecc_fsm_erb_release_ongoing_radio_link_failure_handler },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
        uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_re_establish_req_handler},
    { { UECC_EV_TIMER_EXPIRY                },
        uecc_fsm_sgnb_rel_ongoing_timer_expiry_handler   },
    { { UECC_EV_X2AP_ERROR_INDICATION},
        uecc_fsm_sgnb_rel_error_handler     },
    { { UECC_LAST_ID                                    },
        PNULL                                           }
};
static uecc_fsm_activity_state_t uecc_fsm_sgnb_release_ongoing =
{
    /* .s_name         = */  (S8*)"UECC_SGNB_RELEASE_ONGOING",
    /* .s_id           = */  (S8*)"SGNB_REL",
    /* .is_need_init   = */  RRC_TRUE,
    /* .p_mht          = */  uecc_fsm_sgnb_release_ongoing_mht
};
/*EN-DC_changes_end*/
#endif

/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/
/****************************************************************************
* Function Name  : storeLlimInfo_InUeContext_processErabReleaseReq
*
* Description    : store erab release request parameter in ue context 
* recieved from LLIM message and process the rab release request
* Returns        : RRC_FAILURE/RRC_SUCCESS
*
****************************************************************************/
static rrc_return_t storeLlimInfo_InUeContext_processErabReleaseReq(
        s1ap_Cause    *cause,
        uecc_ue_context_t    *p_ue_context,
        rrc_uecc_llim_delete_lc_resp_t  *p_rrc_uecc_llim_delete_lc_resp
)
{
    rrc_return_t   result = RRC_FAILURE;
    U8 counter = 0;
    U8 index = 0;
    U8 lc_counter = 0;
    U8 resp_counter = RRC_NULL;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rrc_uecc_llim_delete_lc_resp);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Check whether UECC_EV_LLIM_RECONFIGURE_UE_RESP
     * received with success or failure or partial success */
    if (SUCCESS == p_rrc_uecc_llim_delete_lc_resp->
            response_code)
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_DELETE_LC_RESP] Response Code =%i",
                p_rrc_uecc_llim_delete_lc_resp->response_code);
        /* SPS Start */
        if ((p_ue_context->m.is_sps_recv_frm_rrm) &&
                /* Check if Setup SPS config was received */
                (!p_ue_context->m.is_sps_setup_or_rel_recv))
        {    
            if (p_rrc_uecc_llim_delete_lc_resp->bitmask &
                    UECC_LLIM_DELETE_LC_RESP_SPS_CRNTI_PRESENT)
            {
                /* Copy SPS CRNTI into UE Context */
                rrc_pack_U16(p_ue_context->p_sps_config->semi_presist_sched_c_rnti,
                        &p_rrc_uecc_llim_delete_lc_resp->sps_crnti);
                p_ue_context->p_sps_config->bitmask |= RRC_SPS_CONFIG_C_RNTI_PRESENT;
            }
            else
            {
                p_ue_context->m.sps_config_updated = RRC_NULL; /* Reset to 0 */
                if (p_ue_context->p_sps_config != PNULL)
                {
                    rrc_mem_free(p_ue_context->p_sps_config);
                    p_ue_context->p_sps_config = PNULL;
                    p_ue_context->m.sps_config_present = 0;
                }
            }

        }
        /* SPS Stop */

        /*CR 101 changes start*/
        for (counter = 0; counter < MAX_ERAB_COUNT; counter++)
        {
            if (PNULL != p_ue_context->p_e_rab_list[counter])
            {
                for (resp_counter = 0;
                        resp_counter < p_rrc_uecc_llim_delete_lc_resp->
                        delete_drb_entity_resp_counter;
                        resp_counter++)
                {
                    if (p_ue_context->p_e_rab_list[counter]->
                            drb_config.logical_channel_identity ==
                            p_rrc_uecc_llim_delete_lc_resp->
                            delete_drb_entity_resp[resp_counter].lc_id)
                    {
                        for (lc_counter = 0; lc_counter < p_rrc_uecc_llim_delete_lc_resp->
                                delete_lc_status_counter; lc_counter++)
                        {
                            if (p_rrc_uecc_llim_delete_lc_resp->
                                    delete_drb_entity_resp[resp_counter].lc_id ==
                                    p_rrc_uecc_llim_delete_lc_resp->
                                    delete_lc_status[lc_counter].lch_id)
                            {
                                if (MAC_DELETE_LC_ACTIVE == p_rrc_uecc_llim_delete_lc_resp->
                                        delete_lc_status[lc_counter].status )
                                {
                                    for (index = 0; index < p_ue_context->p_curr_proc_data->u.
                                            erab_release_command_data.erab_release_list.counter;
                                            index++)
                                    {
                                        if (p_ue_context->p_curr_proc_data->u.
                                                erab_release_command_data.erab_release_list.
                                                erab_item[index].e_RAB_ID == p_ue_context->
                                                p_e_rab_list[counter]->e_RAB_ID)
                                        {
                                            p_ue_context->p_curr_proc_data->u.
                                                erab_release_command_data.erab_release_list.
                                                erab_item[index].lc_status = MAC_DELETE_LC_ACTIVE;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        /*CR 101 changes stop*/

        /* Switch  state */
        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                &uecc_fsm_erb_release_ue_reconfigure_ongoing, PNULL);

        result = RRC_SUCCESS;
    }
    else if (PARTIAL_SUCCESS == 
            p_rrc_uecc_llim_delete_lc_resp->response_code)
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_DELETE_LC_RESP] Response Code =%i",
                p_rrc_uecc_llim_delete_lc_resp->response_code);
        /* SPS Start */
        if (p_ue_context->m.is_sps_recv_frm_rrm)
        {
            /* Check if Setup SPS config was received */
            if (!p_ue_context->m.is_sps_setup_or_rel_recv)
            {
                if (p_rrc_uecc_llim_delete_lc_resp->bitmask &
                        UECC_LLIM_DELETE_LC_RESP_SPS_CRNTI_PRESENT)
                {
                    /* Copy SPS CRNTI into UE Context */
                    rrc_pack_U16(p_ue_context->p_sps_config->semi_presist_sched_c_rnti,
                            &p_rrc_uecc_llim_delete_lc_resp->sps_crnti);
                    p_ue_context->p_sps_config->bitmask |= RRC_SPS_CONFIG_C_RNTI_PRESENT;
                }
                else
                {
                    p_ue_context->m.sps_config_updated = RRC_NULL; /* Reset to 0 */
                }
            }
        }
        /* SPS Stop */

        /*update curr proc data with rrc_uecc_llim_delete_lc_resp */
        update_curr_proc_data_with_response_code(p_ue_context,
                p_rrc_uecc_llim_delete_lc_resp);

        /* Switch  state */
        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                &uecc_fsm_erb_release_ue_reconfigure_ongoing, PNULL);
        result = RRC_SUCCESS;
    }
    else if (FAILURE ==  
            p_rrc_uecc_llim_delete_lc_resp->response_code)
    {   
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_DELETE_LC_RESP] Response Code =%i",
                p_rrc_uecc_llim_delete_lc_resp->response_code);

        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_ERAB_RELEASE_CMD_GUARD_TIMER, 
                &p_ue_context->timers_data);

        /*update curr proc data with rrc_uecc_llim_delete_lc_resp */
        update_curr_proc_data_with_response_code(p_ue_context,
                p_rrc_uecc_llim_delete_lc_resp);

        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                    p_ue_context, FAILURE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                    "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }

        /* append erab_release_list to erab_failed_to_release_list
         * in curr proc data and erab_release_list empty */
        append_erab_release_list_to_erab_failed_to_release_list(
                p_ue_context);

        /* Build and Send S1AP_ERAB_RELEASE_RESPONSE */
        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                    RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                    "[s1ap_ERABReleaseResponse] delivery failure");
        }

        /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
                    cause))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                    "[s1ap_UeContextReleaseRequest] delivery failure");
        }
    }
    else if (L3_FATAL ==
            p_rrc_uecc_llim_delete_lc_resp->response_code)
    {

        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[UECC_EV_LLIM_DELETE_LC_RESP]"
                "Response code  [%i]",
                p_rrc_uecc_llim_delete_lc_resp->response_code);

        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_ERAB_RELEASE_CMD_GUARD_TIMER, 
                &p_ue_context->timers_data);

        /*update curr proc data with rrc_uecc_llim_delete_lc_resp */
        update_curr_proc_data_with_response_code(p_ue_context,
                p_rrc_uecc_llim_delete_lc_resp);

        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                    p_ue_context, FAILURE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                    "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }

        /* append erab_release_list to erab_failed_to_release_list
         * in curr proc data and erab_release_list empty */
        append_erab_release_list_to_erab_failed_to_release_list(
                p_ue_context);

        /* Build and Send S1AP_ERAB_RELEASE_RESPONSE */
        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                    RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                    "[s1ap_ERABReleaseResponse] delivery failure");
        }

        /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
                    cause))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                    "[s1ap_UeContextReleaseRequest] delivery failure");
        }

    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_erb_release_procedure_constructor
 *
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : Constructor for UE E-RAB Release procedure(MME Initiated)
 *
 ****************************************************************************/
void uecc_fsm_ue_erb_release_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_return_t    result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

#ifdef RRC_UECC_TR_ID_CONST
        uecc_ue_ctx_set_op_transction_id(p_ue_context, 1); 
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_INFO,
                "ERAB Release Command: Transaction Id updated");
#else
        uecc_ue_ctx_set_op_transction_id(p_ue_context,
            uecc_ue_ctx_get_next_transction_id(p_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/
    /* Allocate memory for the curr_proc_data to be used during 
     * E-RAB Management Procedures*/
     p_ue_context->p_curr_proc_data = 
         (curr_proc_data_t*)rrc_mem_get(sizeof(curr_proc_data_t));

    if (PNULL == p_ue_context->p_curr_proc_data)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,p_ue_context->
                p_gb_context->facility_name,RRC_BRIEF,
            "[CURR_PROC_DATA] Memory Alocation failed(E-RAB Release Command)");
    }
    else 
    {
        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */

        do {
               /* Start timer*/
               if (RRC_SUCCESS != uecc_ue_timer_start(
                            p_ue_context->p_gb_context, 
                            UECC_ERAB_RELEASE_CMD_GUARD_TIMER,
                            &p_ue_context->p_gb_context->ue_timers_data,
                            &p_ue_context->timers_data,
                            p_ue_context->ue_index))
               {
                   break;
               }

               if (RRC_SUCCESS !=
                   uecc_fsm_erb_release_process_e_rab_release_command_msg
                   (p_ue_context, p_api))
               {
                   break;
               }

               RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO, 
                   "eRAB number to be released [%u]",
                  p_ue_context->p_curr_proc_data->u.
                  erab_release_command_data.erab_to_be_released_list.counter);
             
               result = RRC_SUCCESS;

        } while(0);
    }
    if (RRC_SUCCESS != result)
    {
        /* stop timer if it has been started */
        if (PNULL !=
            p_ue_context->timers_data.timers_ids[UECC_ERAB_RELEASE_CMD_GUARD_TIMER])
        {
            /* Stop timer */
            uecc_ue_timer_stop(
                    p_ue_context->p_gb_context, 
                    UECC_ERAB_RELEASE_CMD_GUARD_TIMER,
                &p_ue_context->timers_data);
        }
        /* Process failure*/
        uecc_fsm_finish_activity_failure(
          p_ue_context, (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_VALIDATION_FAILURE);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_erb_release_procedure_finish_success
 *
 * Input          :  uecc_ue_context_t*  p_ue_context,
 *                   void*               p_api
 *
 * 
 * Output         : None
 *
 * Returns        : None
 * Description    : Success handler for UE E-RAB Release procedure
 *                  (MME Initiated)
 *                  Calls on procedure's finishing
 *
 ****************************************************************************/
void uecc_fsm_ue_erb_release_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    erab_list_t*   p_erab_list = PNULL;
    U8    free_erab_index = RRC_NULL;   

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Clear update configuration IEs flags */
    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.sps_config_updated = 0;
    p_ue_context->m.mac_config_updated = 0;
    p_ue_context->m.srb1_config_updated = 0;
    p_ue_context->m.srb2_config_updated = 0;
    /*SPR_19066_START*/
    p_ue_context->m.ue_inactive_time_updated = RRC_FALSE;
    /*SPR_19066_END*/

#ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    /* Clear the share data in UE Context */
    if (p_ue_context->share_data.p_uecc_rm_data != PNULL)
    {
        rrc_mem_free(p_ue_context->share_data.p_uecc_rm_data);
        p_ue_context->share_data.p_uecc_rm_data = PNULL;
    }
    /* MENB CHANGES - END */
#endif

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        p_ue_context->m.ue_agg_max_bit_rate_updated = 0;
        asn1Free_s1ap_NAS_PDU(&p_ue_context->s1ap_asn1_ctx,
                &p_ue_context->p_curr_proc_data->u.erab_release_command_data.
                nAS_PDU);
        p_ue_context->p_curr_proc_data->u.erab_release_command_data.
            m.nAS_PDUPresent = 0;

        /* point to erab_release_list in erab_release_command_data */
        p_erab_list = &p_ue_context->p_curr_proc_data->u.
            erab_release_command_data.erab_release_list;

        /* Memory freed for Deleted E-RAB's from Ue Context */
        for (free_erab_index = 0; 
                free_erab_index < p_ue_context->p_curr_proc_data->u.
                erab_release_command_data.erab_release_list.counter;
                free_erab_index++)
        {
            if (PNULL != 
                    p_ue_context->p_e_rab_list
                    [p_erab_list->erab_item[free_erab_index].e_RAB_ID])
            {
                /*SPR 20737 Start*/
                /*Code Deleted*/
                /*SPR 20737 Stop*/
                rrc_mem_free(p_ue_context->p_e_rab_list
                        [p_erab_list->erab_item[free_erab_index].e_RAB_ID]);     
                p_ue_context->p_e_rab_list
                    [p_erab_list->erab_item[free_erab_index].e_RAB_ID] = PNULL;
            }
            /* Updates the ERAB Count in UE Context */
            p_ue_context->e_rab_list_count--;
        }

        /* Memory freed previously allocated for E-RAB Release Command */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }
    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                "p_ue_context->p_curr_proc_data is NULL");
    }
    /* SPS Start */
    if (p_ue_context->m.is_sps_recv_frm_rrm)
    {
        if (p_ue_context->m.is_sps_setup_or_rel_recv)
        {
            if (p_ue_context->p_sps_config != PNULL)
            {
                rrc_mem_free(p_ue_context->p_sps_config);
                p_ue_context->m.sps_config_present = 0;
                p_ue_context->p_sps_config = PNULL;
            }
        }
        /* Send to MAC - RRC_MAC_RECONFIG_COMPLETE_IND */
        uecc_llim_build_and_send_mac_reconfig_complete_ind(
                p_ue_context->p_gb_context,
                p_ue_context,
                RRC_SUCCESS);
    }

    p_ue_context->m.sps_config_updated = 0;
    p_ue_context->m.is_sps_recv_frm_rrm = 0;
    p_ue_context->m.is_sps_setup_or_rel_recv = 0;

    /* SPS Stop */


    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
            UECC_EV_S1AP_ERAB_RELEASE_COMMAND_SUCCESS_INT,
            p_ue_context,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name : uecc_fsm_ue_erb_release_procedure_finish_fail
 *
 * Input         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * Output        : None
 *
 * Returns       : None
 * Description   : Failure handler for UE E-RAB Release procedure
 *                 (MME Initiated)
 *                 Calls on procedure's finishing with failure
 *
 ****************************************************************************/
void uecc_fsm_ue_erb_release_procedure_finish_fail(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
/*SPR 20636 Changes Start*/
    uecc_fsm_event_et     failure_ev = (uecc_fsm_event_et)((U32_64_Bit)p_api);
/*SPR 20636 Changes End*/

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* SPR 20383 Fix Start */
    /* Code Moved at the end of function */
    /* SPR 20383 Fix End */
    /* SPS Start */
    if (p_ue_context->m.is_sps_recv_frm_rrm)
    {
        if (p_ue_context->p_sps_config)
        {
            rrc_mem_free(p_ue_context->p_sps_config);
            p_ue_context->p_sps_config = PNULL;
            p_ue_context->m.sps_config_present = 0;
        }
        /* Send to MAC - RRC_MAC_RECONFIG_COMPLETE_IND */
        uecc_llim_build_and_send_mac_reconfig_complete_ind(
                p_ue_context->p_gb_context,
                p_ue_context,
                RRC_FAILURE);
    }
    /* SPS Stop */

    p_ue_context->m.ue_agg_max_bit_rate_updated = 0;

    /*SPR_19066_START*/
    p_ue_context->m.ue_inactive_time_updated = RRC_FALSE;
    /*SPR_19066_END*/

#ifdef ENDC_ENABLED
    /*EN-DC_changes_start*/
    /* Clear the share data in UE Context */
    if (p_ue_context->share_data.p_uecc_rm_data != PNULL)
    {
        rrc_mem_free(p_ue_context->share_data.p_uecc_rm_data);
        p_ue_context->share_data.p_uecc_rm_data = PNULL;
    }
    /*EN-DC_changes_stop*/
#endif

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
    /* Deallocate memory previously allocated for E-RAB Release Command */
    rrc_mem_free(p_ue_context->p_curr_proc_data);
    p_ue_context->p_curr_proc_data = PNULL;
    }
    /* SPR 20383 Fix Start */
    if ((UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT == failure_ev)
            || (UECC_EV_S1AP_ERAB_RELEASE_COMMAND_VALIDATION_FAILURE == failure_ev)) 

    {
        uecc_event_queue_push_event(
                failure_ev,
                p_ue_context,
                p_api);

    }
    /* SPR 20383 Fix End */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_ue_release_req_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_UE_RELEASE_REQ
 *
 ****************************************************************************/
void uecc_fsm_erb_release_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_s1ap_ue_context_release_command_t* p_ue_context_release_command =
        (rrc_s1ap_ue_context_release_command_t*)p_api;
    U32    error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Stop timer */
    uecc_ue_timer_stop(
            p_ue_context->p_gb_context, 
            UECC_ERAB_RELEASE_CMD_GUARD_TIMER, &p_ue_context->timers_data);

    /* Store data in UE context for future processing */
    p_ue_context->share_data.uecc_rcr_data.u.cause =
        p_ue_context_release_command->cause;

    /* update cause of each ERAB in erab_release_list
     * with cause which is received in Ue Context Release Command */
    update_erab_release_list_with_cause(p_ue_context,
        &p_ue_context_release_command->cause);

    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_INTERACTION_WITH_OTHER_PROCEDURE;

    /* update error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
        &error_code);

    /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
        p_ue_context, FAILURE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
    }

    /* Process failure handler for procedure */
    uecc_fsm_finish_activity_failure(
        p_ue_context, 
        (void*)UECC_EV_UE_RELEASE_REQ_INT);

    uecc_event_queue_push_event(
                 UECC_EV_UE_RELEASE_REQ_INT,
                 p_ue_context,
                 p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_s1ap_reset_handler 
 *
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 *
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_S1AP_RESET_MSG_IND
 *
 ****************************************************************************/
void uecc_fsm_erb_release_s1ap_reset_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_S1AP_RESET_MSG_IND");

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_CMD_GUARD_TIMER,
            &p_ue_context->timers_data);

    /* set value of cause due to RLF during ERAB Mgnt Procedures */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_interaction_with_other_procedure;

    /* update cause of each ERAB in erab_release_list
     * with type "T_s1ap_Cause_radioNetwork" and
     * value "s1ap_interaction_with_other_procedure" */
    update_erab_release_list_with_cause(p_ue_context,
        &cause);
    error_code |= RRC_MODULE_ID << 16; 
    error_code |= RRC_INTERACTION_WITH_OTHER_PROCEDURE; 

    /* update error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
        &error_code);

    /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
        p_ue_context, FAILURE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
    }

    /* Process failure handler for procedure */
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_S1AP_RESET_MSG_IND_INT);

    uecc_event_queue_push_event(
                 UECC_EV_S1AP_RESET_MSG_IND_INT,
                 p_ue_context,
                 p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_resource_deletion_ongoing_s1ap_reset_handler 
 *
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_S1AP_RESET_MSG_IND while waiting resp from 
 *                  rrm in rrm_resource_deletion_ongoing state
 *
 ****************************************************************************/
void uecc_fsm_rrm_resource_deletion_ongoing_s1ap_reset_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    U32       error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_CMD_GUARD_TIMER, 
            &p_ue_context->timers_data);

    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_INTERACTION_WITH_OTHER_PROCEDURE;

    /* update error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
        &error_code);

    /*  Since Resp from RRM is not received so in RRC_RRM_ERB_RELEASE_CNF
     ** contain only erab id's which are previously send in RRC_RRM_ERB_RELEASE
     ** REQ so that inside function uecc _rrm_build_and_send_erb_release_conf
     ** don't send erab's(Unknown ERAB IDs) those are not recognized by eNB */

    p_ue_context->p_curr_proc_data->u.erab_release_command_data.
            counter_after_rrm_resp = p_ue_context->p_curr_proc_data->u.
            erab_release_command_data.erab_failed_to_release_list.counter;

    /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                            p_ue_context, FAILURE))
    {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                            "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
    }

    /* Process failure*/
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_S1AP_RESET_MSG_IND_INT);

    uecc_event_queue_push_event(
                 UECC_EV_S1AP_RESET_MSG_IND_INT,
                 p_ue_context,
                 p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_resource_deletion_ongoing_ue_release_
 *                  req_handler
 *
 * Input          : uecc_ue_context_t*   p_ue_context
 *                  void*                p_api
 *
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_UE_RELEASE_REQ while waiting resp from
 *                  rrm in rrm_resource_deletion_ongoing state
 *
 ****************************************************************************/
void uecc_fsm_rrm_resource_deletion_ongoing_ue_release_req_handler(
    uecc_ue_context_t*   p_ue_context,
    void*                p_api
)
{
    U32      error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_CMD_GUARD_TIMER, 
            &p_ue_context->timers_data);

    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_INTERACTION_WITH_OTHER_PROCEDURE;

    /* update error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
        &error_code);

    /*  Since Resp from RRM is not received so in RRC_RRM_ERB_RELEASE_CNF
     ** contain only erab id's which are previously send in RRC_RRM_ERB_RELEASE
     ** REQ so that inside function uecc _rrm_build_and_send_erb_release_conf
     ** don't send erab's(Unknown ERAB IDs) those are not recognized by eNB */

    p_ue_context->p_curr_proc_data->u.erab_release_command_data.
            counter_after_rrm_resp = p_ue_context->p_curr_proc_data->u.
            erab_release_command_data.erab_failed_to_release_list.counter;

    /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                            p_ue_context, FAILURE))
    {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                            "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
    }

    /* Process failure*/
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_UE_RELEASE_REQ_INT);

    uecc_event_queue_push_event(
                 UECC_EV_UE_RELEASE_REQ_INT,
                 p_ue_context,
                 p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_resource_deletion_ongoing_radio_link_
 *                  failure_handler
 *                 
 * Input          : uecc_ue_context_t*    p_ue_context,
 *                  void*                 p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_RADIO_LINK_FAILURE_IND while waiting resp from
 *                  rrm in rrm_resource_deletion_ongoing state
 ****************************************************************************/
void uecc_fsm_rrm_resource_deletion_ongoing_radio_link_failure_handler(
    uecc_ue_context_t*    p_ue_context,
    void*                 p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_RADIO_LINK_FAILURE_IND");

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_CMD_GUARD_TIMER, 
            &p_ue_context->timers_data);
    
    /* set value of cause due to RLF during ERAB Mgnt Procedures */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost;    

    /* update cause of each ERAB in erab_release_list
     * with type "T_s1ap_Cause_radioNetwork" and 
     * value "s1ap_failure_in_radio_interface_procedure" */
    update_erab_release_list_with_cause(p_ue_context,
        &cause);
    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_RADIO_LINK_FAILURE_TRIGGERED;
    
    /* update error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
                &error_code);

    /* Since Resp from RRM is not received so in RRC_RRM_ERB_RELEASE_CNF
     * contain only erab id's which are previously send in RRC_RRM_ERB_RELEASE
     * REQ so that inside function uecc _rrm_build_and_send_erb_release_conf
     * don't send erab's those are not recognized by eNB */
    p_ue_context->p_curr_proc_data->u.erab_release_command_data.
        counter_after_rrm_resp = p_ue_context->p_curr_proc_data->u.
        erab_release_command_data.erab_failed_to_release_list.counter;

    /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                            p_ue_context, FAILURE))
    {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                            "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
    }

    /* append erab_release_list to erab_failed_to_release_list
     * in curr proc data and erab_release_list empty */
    append_erab_release_list_to_erab_failed_to_release_list(
        p_ue_context);

    /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_ERABReleaseResponse] delivery failure");
    }
    /* SPR 20383 Fix Start */
    cause.u.radioNetwork = s1ap_interaction_with_other_procedure;
     /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
                    &cause))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                    "[s1ap_UeContextReleaseRequest] delivery failure");
        }

    /* Process failure*/
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);
    /* Code Deleted */
   /* SPR 20383 Fix End */
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_rrm_resource_deletion_ongoing_
 *                  re_establish_req_handler 
 *                 
 * Input          : uecc_ue_context_t*    p_ue_context,
 *                  void*                 p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ while waiting resp from
 *                  rrm in rrm_resource_deletion_ongoing state
 ****************************************************************************/
void uecc_fsm_erb_release_rrm_resource_deletion_ongoing_re_establish_req_handler( 
    uecc_ue_context_t*    p_ue_context,
    void*                 p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_RRC_RE_ESTABLISH_REQ");

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_CMD_GUARD_TIMER, 
            &p_ue_context->timers_data);
    
    /* set value of cause due to REESTABLISH during ERAB Mgnt Procedures */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_interaction_with_other_procedure; 

    /* update cause of each ERAB in erab_release_list
     * with type "T_s1ap_Cause_radioNetwork" and 
     * value "s1ap_interaction_with_other_procedure" */
    update_erab_release_list_with_cause(p_ue_context,
        &cause);
    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_REESTABLISHMENT_TRIGGERED; 
    
    /* update error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
                &error_code);

    /* Since Resp from RRM is not received so in RRC_RRM_ERB_RELEASE_CNF
     * contain only erab id's which are previously send in RRC_RRM_ERB_RELEASE
     * REQ so that inside function uecc _rrm_build_and_send_erb_release_conf
     * don't send erab's those are not recognized by eNB */
    p_ue_context->p_curr_proc_data->u.erab_release_command_data.
        counter_after_rrm_resp = p_ue_context->p_curr_proc_data->u.
        erab_release_command_data.erab_failed_to_release_list.counter;

    /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                            p_ue_context, FAILURE))
    {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                            "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
    }

    /* append erab_release_list to erab_failed_to_release_list
     * in curr proc data and erab_release_list empty */
    append_erab_release_list_to_erab_failed_to_release_list(
        p_ue_context);

    /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_ERABReleaseResponse] delivery failure");
    }
    /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
        &cause))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_UeContextReleaseRequest] delivery failure");
    }

    if (RRC_SUCCESS !=
           uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
           p_ue_context->p_gb_context, p_ue_context->crnti,
           p_ue_context->reestablishmentCause,
           p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                "Fail to send the RRCConnectionReestablishmentReject to UE");
    }

    /* Set This Flag to RRC_TRUE, so that ue connection release notification
     * not sent to UE (DCCH no more exist) */
    p_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = RRC_TRUE;

    /* Process failure*/
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_csc_delete_all_ue_req_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_CSC_DELETE_ALL_UE_REQ
 *
***************************************************************************/
void uecc_fsm_erb_release_csc_delete_all_ue_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_CSC_DELETE_ALL_UE_REQ");

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_CMD_GUARD_TIMER, 
            &p_ue_context->timers_data);

    /* Process failure*/
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_CSC_CELL_DELETE_REQ_INT);

    uecc_event_queue_push_event(
            UECC_EV_CSC_CELL_DELETE_REQ_INT,
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_rrm_ue_release_req_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND
 *
 ****************************************************************************/
void uecc_fsm_erb_release_rrm_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause cause;
    rrc_rrm_ue_connection_release_ind_t *p_ue_connection_release_ind = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    p_ue_connection_release_ind = (rrc_rrm_ue_connection_release_ind_t *)p_api;

    if (RRC_SUCCESS == uecc_s1ap_build_s1ap_cause(
        &cause, &p_ue_connection_release_ind->release_cause,p_ue_context))
    {
        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_CMD_GUARD_TIMER,
            &p_ue_context->timers_data);

        /* update erab_release_list in curr proc data with 
         * received cause in rrm ue connection release ind */
        update_erab_release_list_with_cause(p_ue_context, 
            &cause);
        /* append erab_release_list to erab_failed_to_release_list
         * in curr proc data and erab_release_list empty */
        append_erab_release_list_to_erab_failed_to_release_list(
            p_ue_context);

        /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
        if (RRC_SUCCESS !=
            uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[s1ap_ERABReleaseResponse] delivery failure");
        }

        uecc_fsm_finish_activity_failure(
            p_ue_context,
            (void*)UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT);

        uecc_event_queue_push_event(
            UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT,
            p_ue_context,
            p_api);
    }
    else
    {
        /* Drop The Received API with Warning Message */
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,p_ue_context->
                p_gb_context->facility_name,RRC_WARNING, "[RRC_RRM_UE_CONNECTION_RELEASE_IND] "
            "invalid cause value [%u][%u]",
            p_ue_connection_release_ind->release_cause.type,
            p_ue_connection_release_ind->release_cause.value);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR 18758 Fix Start*/
/*Code Deleted*/
/*SPR 18758 Fix Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_erab_rel_ind_discard_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_ERAB_RELEASE_IND
******************************************************************************/
void uecc_fsm_erb_release_erab_rel_ind_discard_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* In this function No action is taken for simply discard 
      the received message */

     RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,
        "[UECC_EV_RRM_ERAB_RELEASE_IND]" 
        "MME Initiated E-RAB Release Procedure Ongoing"
        "Discarding E-RAB REL IND Received from RRM");

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_ue_context_mod_req_discard_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_S1AP_UE_CONTEXT_MOD_REQ
******************************************************************************/
void uecc_fsm_erb_release_ue_context_mod_req_discard_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* In this function No action is taken for simply discard 
      the received message */
     RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_WARNING,
        "[UECC_EV_S1AP_UE_CONTEXT_MOD_REQ]" 
        "MME Initiated E-RAB Release Procedure Ongoing"
        "Discarding UE Context Modify request Received from MME");

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_rrm_resource_deletion_ongoing
 *                  _timer_expiry_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_TIMER_EXPIRY while waiting for resp from
 *                  RRM.
 *
 ****************************************************************************/
void uecc_fsm_erb_release_rrm_resource_deletion_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* set cause at timer expiry for ERAB Mgnt Procedures */
    cause.t = T_s1ap_Cause_radioNetwork;
    /* SPR 15473 Fix Start */
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;
    /* SPR 15473 Fix Stop */

    /* set cause of each ERAB in erab_release_list */
    update_erab_release_list_with_cause(p_ue_context,
        &cause);

    /* append erab_release_list to erab_failed_to_release_list
     * in curr proc data and erab_release_list empty */
    append_erab_release_list_to_erab_failed_to_release_list(
        p_ue_context);

    /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_ERABReleaseResponse] delivery failure");
    }

    /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
        &cause))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_UeContextReleaseRequest] delivery failure");
    }

    uecc_fsm_finish_activity_failure(
        p_ue_context,
        (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_timer_expiry_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_TIMER_EXPIRY
 *
 ****************************************************************************/
void uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "LLIM Reconfigure Ue Ongoing: UECC_EV_TIMER_EXPIRY");

    /* set cause at timer expiry for ERAB Mgnt Procedures */
    cause.t = T_s1ap_Cause_radioNetwork;
    /* SPR 15473 Fix Start */
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;
    /* SPR 15473 Fix Stop */

    /* set cause of each ERAB in erab_release_list */
    update_erab_release_list_with_cause(p_ue_context,
        &cause);

    /* Update list with this error code */
    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_TIMER_EXPIRY;

    /* set error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
        &error_code);

    /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
        p_ue_context, FAILURE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
    }

    /* append erab_release_list to erab_failed_to_release_list
     * in curr proc data and erab_release_list empty */
    append_erab_release_list_to_erab_failed_to_release_list(
        p_ue_context);

    /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_ERABReleaseResponse] delivery failure");
    }

    /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
        &cause))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_UeContextReleaseRequest] delivery failure");
    }

    uecc_fsm_finish_activity_failure(
        p_ue_context,
        (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_ue_reconfigure_ongoing 
 *                  _timer_expiry_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_TIMER_EXPIRY
 *
 ****************************************************************************/
void uecc_fsm_erb_release_ue_reconfigure_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "Ue Reconfiguraion Ongoing: UECC_EV_TIMER_EXPIRY");

    /* set cause at timer expiry for ERAB Mgnt Procedures */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;

    /* set cause of each ERAB in erab_release_list */
    update_erab_release_list_with_cause(p_ue_context,
        &cause);

    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_TIMER_EXPIRY;

    /* set error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
        &error_code);

    /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
        p_ue_context, FAILURE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
    }

    /* append erab_release_list to erab_failed_to_release_list
     * in curr proc data and erab_release_list empty */
    append_erab_release_list_to_erab_failed_to_release_list(
        p_ue_context);

    /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_ERABReleaseResponse] delivery failure");
    }

    /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
        &cause))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_UeContextReleaseRequest] delivery failure");
    }

    uecc_fsm_finish_activity_failure(
        p_ue_context,
        (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_ongoing_radio_link_failure_handler 
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_RADIO_LINK_FAILURE_IND
 *
 ****************************************************************************/
void uecc_fsm_erb_release_ongoing_radio_link_failure_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;
#ifdef ENDC_ENABLED
   /*EN-DC_changes_start*/
    U16             erab_idx = RRC_ZERO;
    U16             e_rab_id = RRC_ZERO;
   /*EN-DC_changes_end*/
#endif

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_RADIO_LINK_FAILURE_IND");

#ifdef ENDC_ENABLED
    /*EN-DC_changes_start*/
    for(erab_idx = 0;
            erab_idx < p_ue_context->p_curr_proc_data->u.erab_release_command_data.
            erab_release_list.counter;
            erab_idx++)
    {
        e_rab_id = p_ue_context->p_curr_proc_data->u.erab_release_command_data.
            erab_release_list.erab_item[erab_idx].e_RAB_ID;
        if ( RRC_NULL != p_ue_context->p_e_rab_list[e_rab_id]
                &&
                RRC_ONE == p_ue_context->p_e_rab_list[e_rab_id]->m.DC_BearerTypePresent
                &&
                (SN_TERMINATED_SCG_BEARER == p_ue_context->p_e_rab_list[e_rab_id]->dc_bearer_type ||
                 SN_TERMINATED_SPLIT_BEARER == p_ue_context->p_e_rab_list[e_rab_id]->dc_bearer_type)
           )
        {
            if(RRC_SUCCESS != uecc_fsm_ue_context_release_handler(p_ue_context))
            {
                RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_WARNING,
                        "[%s]X2AP ue_context_release"
                        " failed !!",__FUNCTION__);
            }
            break;
        }
    }
    /*EN-DC_changes_stop*/
#endif
    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_CMD_GUARD_TIMER,
            &p_ue_context->timers_data);

    /* set value of cause due to RLF during ERAB Mgnt Procedures */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost;

    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_RADIO_LINK_FAILURE_TRIGGERED;

    /* update cause of each ERAB in erab_release_list
     * with type "T_s1ap_Cause_radioNetwork" and
     * value "s1ap_failure_in_radio_interface_procedure" */
    update_erab_release_list_with_cause(p_ue_context,
        &cause);

    /* update error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
        &error_code);

    /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
        p_ue_context, FAILURE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
    }

    /* append erab_release_list to erab_failed_to_release_list
     * in curr proc data and erab_release_list empty */
    append_erab_release_list_to_erab_failed_to_release_list(
        p_ue_context);

    /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_ERABReleaseResponse] delivery failure");
    }
    /* SPR 20383 Fix Start */
    cause.u.radioNetwork = s1ap_interaction_with_other_procedure;
    /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
    if (RRC_SUCCESS !=
            uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
                &cause))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[s1ap_UeContextReleaseRequest] delivery failure");
    }

    /* Process failure handler for procedure */
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);
    /* Code Deleted */
   /* SPR 20383 Fix End */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_re
 *                  _establish_req_handler 
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ
 *
 ****************************************************************************/
void uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_re_establish_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32  error_code = RRC_NULL;
#ifdef ENDC_ENABLED
   /*EN-DC_changes_start*/
    U16             erab_idx = RRC_ZERO;
    U16             e_rab_id = RRC_ZERO;
   /*EN-DC_changes_end*/
#endif

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_RRC_RE_ESTABLISH_REQ");
#ifdef ENDC_ENABLED
    /*EN-DC_changes_start*/
    for(erab_idx = 0;
            erab_idx < p_ue_context->p_curr_proc_data->u.erab_release_command_data.
            erab_release_list.counter;
            erab_idx++)
    {
        e_rab_id = p_ue_context->p_curr_proc_data->u.erab_release_command_data.
            erab_release_list.erab_item[erab_idx].e_RAB_ID;
        if ( RRC_NULL != p_ue_context->p_e_rab_list[e_rab_id]
                &&
                RRC_ONE == p_ue_context->p_e_rab_list[e_rab_id]->m.DC_BearerTypePresent
                &&
                (SN_TERMINATED_SCG_BEARER == p_ue_context->p_e_rab_list[e_rab_id]->dc_bearer_type ||
                 SN_TERMINATED_SPLIT_BEARER == p_ue_context->p_e_rab_list[e_rab_id]->dc_bearer_type)
           )
        {
            if(RRC_SUCCESS != uecc_fsm_ue_context_release_handler(p_ue_context))
            {
                RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_WARNING,
                        "[%s]X2AP ue_context_release"
                        " failed !!",__FUNCTION__);
            }
            break;
        }
    }
    /*EN-DC_changes_stop*/
#endif

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_CMD_GUARD_TIMER, 
            &p_ue_context->timers_data);

    /* set value of cause due to RLF during ERAB Mgnt Procedures */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_interaction_with_other_procedure; 

    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_REESTABLISHMENT_TRIGGERED; 

    /* update cause of each ERAB in erab_release_list
     * with type "T_s1ap_Cause_radioNetwork" and
     * value "s1ap_failure_in_radio_interface_procedure" */
    update_erab_release_list_with_cause(p_ue_context,
        &cause);

    /* update error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
        &error_code);

    /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
        p_ue_context, FAILURE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
    }

    /* append erab_release_list to erab_failed_to_release_list
     * in curr proc data and erab_release_list empty */
    append_erab_release_list_to_erab_failed_to_release_list(
        p_ue_context);

    /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_ERABReleaseResponse] delivery failure");
    }

    /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
        &cause))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_UeContextReleaseRequest] delivery failure");
    }

    if (RRC_SUCCESS !=
           uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
           p_ue_context->p_gb_context, p_ue_context->crnti,
           p_ue_context->reestablishmentCause,
           p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                "Fail to send the RRCConnectionReestablishmentReject to UE");
    }

    /* Set This Flag to RRC_TRUE, so that ue connection release notification
     * not sent to UE (DCCH no more exist) */
    p_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = RRC_TRUE;


    /* Process failure handler for procedure */
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_ongoing_re_establish_req_handler 
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ    
 *
 ****************************************************************************/
void uecc_fsm_erb_release_ongoing_re_establish_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_RRC_RE_ESTABLISH_REQ;Unused variable p_api=%p",p_api);/*SPR 17777 +-*/
  
        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_CMD_GUARD_TIMER,
                &p_ue_context->timers_data);

        /* set cause due to RRC Connection Re-establishment
         * (reconfiguration failure) for ERAB Mgnt Procedures */
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;

        /* set cause of each ERAB in erab_release_list */
        update_erab_release_list_with_cause(p_ue_context,
            &cause);

        error_code |= RRC_MODULE_ID << 16;
        error_code |= RRC_REESTABLISHMENT_TRIGGERED;

        /* set error code of each ERAB in erab_release_list */
        update_erab_release_list_with_error_code(p_ue_context,
            &error_code);

        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
            p_ue_context, FAILURE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }

        /* append erab_release_list to erab_failed_to_release_list
         * in curr proc data and erab_release_list empty */
        append_erab_release_list_to_erab_failed_to_release_list(
            p_ue_context);

        /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
        if (RRC_SUCCESS !=
            uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[s1ap_ERABReleaseResponse] delivery failure");
        }

        /* SPR 18724 Fix Start */
        /* Process failure handler for procedure */
        uecc_fsm_finish_activity_failure(
           p_ue_context, (void*)UECC_EV_RRC_RE_ESTABLISH_REQ_INT);

        uecc_event_queue_push_event(
            UECC_EV_RRC_RE_ESTABLISH_REQ_INT,
            p_ue_context,
            p_api);
        /* SPR 18724 Fix Stop */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* SPR:5471 start */
/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_is_atleast_one_non_gbr_bearer_present
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                   erab_list_t*      - p_erab_list
 * Outputs        : None
 * Returns        : rrc_return_t
 * Description    : This returns RRC_SUCCESS if atleast one non GBR bearer
                    is present.
 ****************************************************************************/
rrc_return_t uecc_fsm_erb_release_is_atleast_one_non_gbr_bearer_present(
    uecc_ue_context_t*    p_uecc_ue_context,
    erab_list_t*          p_erab_list
    )
{
    rrc_return_t result = RRC_FAILURE;
    U8 erab_count = RRC_NULL;
    U8 list       = RRC_NULL;
    U8 index      = RRC_NULL;
    U8 counter = RRC_NULL;
    s1ap_E_RAB_ID erab_id_list[MAX_ERAB_COUNT] = {RRC_NULL};

    /*12072_fix_start*/
    U8 non_gbr_counter = RRC_NULL;
    erab_list_t*   p_erab_failed_list = PNULL;
    p_erab_failed_list = &p_uecc_ue_context->p_curr_proc_data->u.
        erab_release_command_data.erab_failed_to_release_list;
    /*12072_fix_stop*/

    /* According to 
       [TS 23.203 Table 6.1.7: Standardized QCI characteristics] 
       A GBR Bearer has QCI values from 1 to 4 and
       a non GBR Bearer has QCI values from 5 to 9*/

    for (erab_count = 0; erab_count < MAX_ERAB_COUNT; erab_count++)
    {
        if (p_uecc_ue_context->p_e_rab_list[erab_count] &&
                !RRC_IS_GBR_BEARER(p_uecc_ue_context->p_e_rab_list[erab_count]->
                    e_RABlevelQoSParameters.qCI))
        {
            // result = RRC_SUCCESS;
            erab_id_list[list] = p_uecc_ue_context->p_e_rab_list[erab_count]->e_RAB_ID;
            list++;
            counter++;
            /*12072_fix_start*/
            non_gbr_counter++;
            /*12072_fix_stop*/
        }
    }
    for (erab_count = 0; erab_count < p_erab_list->counter; erab_count++)
    {
        for (index=0; index < list; index++)
        {
        if (p_erab_list->erab_item[erab_count].e_RAB_ID == erab_id_list[index])
        {
            counter--;
        }
        }
    }
    if(counter>0)
    {
        result = RRC_SUCCESS;
    }
    /*12072_fix_start*/
    else
    {
        p_erab_failed_list->erab_item[p_erab_failed_list->counter].
            e_RAB_ID = erab_id_list[0]; 
        p_erab_failed_list->erab_item[p_erab_failed_list->counter].
            cause.t = T_s1ap_Cause_radioNetwork ;
        p_erab_failed_list->erab_item[p_erab_failed_list->counter].
            cause.u.radioNetwork = s1ap_unspecified_2 ;
        p_erab_failed_list->counter++;

        for (index = 0; index < p_erab_list->counter; index++)
        {
            if (p_erab_list->erab_item[index].e_RAB_ID == 
                    erab_id_list[0])
            {
                for (erab_count = index;erab_count < (p_erab_list->counter - 1);
                        erab_count++)
                {
                    p_erab_list->erab_item[erab_count].e_RAB_ID =
                        p_erab_list->erab_item[erab_count + 1].e_RAB_ID;
                }
                p_erab_list->counter--;
                break;
            }
        }
        if (1 < non_gbr_counter)
        {
            result = RRC_SUCCESS;
        }
    }
    /*12072_fix_stop*/

    return result;
}
/* SPR:5471 stop */

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_process_e_rab_release_command_msg
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : RRC_SUCCESS/RRC_FAILURE 
 *
 * Description    : This is the function processes S1AP_ERAB_RELEASE_COMMAND 
 *                  message:
 *                  - check message content;
 *                  - store IEs in UE context;
 *
 ****************************************************************************/
rrc_return_t uecc_fsm_erb_release_process_e_rab_release_command_msg(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    OSRTDListNode* p_node                   = PNULL;
    erab_item_t*   p_erab_item              = PNULL;
    erab_list_t*   p_erab_list              = PNULL;
    erab_list_t*   p_erab_failed_list       = PNULL;
    rrc_s1ap_erab_release_command_t* p_msg = PNULL;
        
    s1ap_E_RABList_element*  p_erab_elem    = PNULL;
    s1ap_E_RABItem*         
        p_e_rab_to_be_released_list[MAX_MME_ERAB_LIST_COUNT]= { NULL };

    U8             e_rab_index          = RRC_NULL;
    U8             indx                 = RRC_NULL;
    U16  e_rab_to_be_released_list_count = RRC_NULL;
    rrc_return_t   result               = RRC_FAILURE;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    p_msg = (rrc_s1ap_erab_release_command_t*)p_api;

    do {
           /* Check message */
           if (
         RRC_S1AP_ERAB_RELEASE_COMMAND_UE_AGGREGATE_MAXIMUM_BIT_RATE_PRESENT &
             p_msg->bitmask)
           {
               /* Store message data in UE context for future processing */
               p_ue_context->ue_aggregate_maximum_bitrate.
                   uEaggregateMaximumBitRateDL = 
                   p_msg->ue_aggregate_maximum_bitrate.
                   uEaggregateMaximumBitRateDL;
               p_ue_context->ue_aggregate_maximum_bitrate.
                   uEaggregateMaximumBitRateUL = 
                   p_msg->ue_aggregate_maximum_bitrate.
                   uEaggregateMaximumBitRateUL;
               p_ue_context->m.ue_agg_max_bit_rate_updated = 1;
           }

            /* multi E-ERAB supporting */
           for (p_node = p_msg->e_rab_to_be_released_list.head;
                (PNULL != p_node) && 
                    (e_rab_to_be_released_list_count < MAX_ERAB_COUNT);
                p_node = p_node->next, e_rab_to_be_released_list_count++)
           {
               p_erab_elem = (s1ap_E_RABList_element*)p_node->data;
               if ((p_erab_elem != PNULL) && (p_erab_elem->value.u._E_RABItemIEs_1 != PNULL))
               {
                   /* check for valid IDs was performed before when decoding */
                   p_e_rab_to_be_released_list[e_rab_to_be_released_list_count] =
                       p_erab_elem->value.u._E_RABItemIEs_1;
               }
           }

            /* points to erab_to_be_released_list */
           p_erab_list = &p_ue_context->p_curr_proc_data->u.
               erab_release_command_data.erab_to_be_released_list;
           p_erab_list->counter = e_rab_to_be_released_list_count;

           /* points to erab_failed_to_release_list */
           p_erab_failed_list = &p_ue_context->p_curr_proc_data->u.
               erab_release_command_data.erab_failed_to_release_list;
    
           /* Copy E-RAB ID and cause received in E-RAB Release Command into 
            * erab_to_be_released_list of erab_release_command_data */
           for (e_rab_index = 0; e_rab_index < e_rab_to_be_released_list_count;
                e_rab_index++)
           {
               if (PNULL != p_e_rab_to_be_released_list[e_rab_index])
               {
                 if (16 <= p_e_rab_to_be_released_list[e_rab_index]->e_RAB_ID)
                 {
                   p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                       e_RAB_ID =
                       p_e_rab_to_be_released_list[e_rab_index]->e_RAB_ID;

                   p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                       cause.t = T_s1ap_Cause_radioNetwork ;

                   p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                       cause.u.radioNetwork = s1ap_unknown_E_RAB_ID ;
                   p_erab_failed_list->counter++;
                 }
                 else
                 {
                     p_erab_list->erab_item[e_rab_index].e_RAB_ID = 
                         p_e_rab_to_be_released_list[e_rab_index]->e_RAB_ID;
                     p_erab_list->erab_item[e_rab_index].cause = 
                         p_e_rab_to_be_released_list[e_rab_index]->cause;
#ifdef  ENDC_ENABLED
                     /*EN-DC_changes_start*/
                     if( RRC_NULL != p_ue_context->p_e_rab_list[p_erab_list->erab_item[e_rab_index].e_RAB_ID] &&
                             (p_ue_context->p_e_rab_list[p_erab_list->erab_item[e_rab_index].e_RAB_ID]->m.DC_BearerTypePresent) &&
                             ((SN_TERMINATED_SCG_BEARER ==
                              p_ue_context->p_e_rab_list[p_erab_list->erab_item[e_rab_index].e_RAB_ID]->dc_bearer_type) || 
                              (SN_TERMINATED_SPLIT_BEARER ==
                                p_ue_context->p_e_rab_list[p_erab_list->erab_item[e_rab_index].e_RAB_ID]->dc_bearer_type)))
                     {
                         /* Since p_ue_context->share_data.p_uecc_rm_data is allocated only for ICS
                          * Procedure and is free'd at the end of ICS Procedure (Success/Failure),
                          * Thus need to be checked and allocated again if not present.
                          * and should be free'd in the end of the Success/failure of current procedure.
                          */
                         if(PNULL == p_ue_context->share_data.p_uecc_rm_data)
                         {
                             p_ue_context->share_data.p_uecc_rm_data =
                                 (uecc_rm_data_t*)
                                 rrc_mem_get(sizeof(uecc_rm_data_t));
                             if ( p_ue_context->share_data.p_uecc_rm_data  ==  PNULL)
                             {
                                 RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                         p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                                         "Memory Allocation Failure!!");
                                 break;

                             }

                         }

                         p_ue_context->share_data.p_uecc_rm_data->cause = 
                             p_e_rab_to_be_released_list[e_rab_index]->cause;
                     }
                     /*EN-DC_changes_stop*/
#endif
                 }
               }
           }

           /* points to e_rab_item of erab_to_be_released_list */
           p_erab_item = p_ue_context->p_curr_proc_data->u.
               erab_release_command_data.erab_to_be_released_list.erab_item;

           /* points to erab_release_list */
           p_erab_list = &p_ue_context->p_curr_proc_data->u.
               erab_release_command_data.erab_release_list;

           /* Attention: Abnormal check performed for Unknown E-RAB ID
            * or Multiple E-RAB ID instances in E-RAB RELEASE COMMAND 
            * message */
 
           for (e_rab_index = 0; e_rab_index < e_rab_to_be_released_list_count;
                e_rab_index++)
           {
               /* check for Unknown E-RAB ID instance*/
               if (PNULL == p_ue_context->
                   p_e_rab_list[(p_erab_item + e_rab_index)->e_RAB_ID])
               {
                   p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                       e_RAB_ID = (p_erab_item + e_rab_index)->e_RAB_ID;

                   p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                       cause.t = T_s1ap_Cause_radioNetwork ;

                   p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                       cause.u.radioNetwork = s1ap_unknown_E_RAB_ID ;
                   p_erab_failed_list->counter++;
               }
               else
               {
                  /* check for Multiple E-RAB ID instances */
                   for (indx = 0;
                        indx < p_erab_list->counter;
                        indx++)
                   {
                       if ((p_erab_item + e_rab_index)->e_RAB_ID == 
                           p_erab_list->erab_item[indx].e_RAB_ID)
                       break;
                   }
                   /* Store in erab_release_list */
                   if (indx == p_erab_list->counter)
                   {
                       p_erab_list->erab_item[p_erab_list->counter].e_RAB_ID = 
                           (p_erab_item + e_rab_index)->e_RAB_ID;
                       p_erab_list->erab_item[p_erab_list->counter].cause = 
                           (p_erab_item + e_rab_index)->cause;
                       p_erab_list->counter++;
                   }
               }
            
           }    

           if (RRC_S1AP_ERAB_RELEASE_COMMAND_NAS_PDU_PRESENT & 
               p_msg->bitmask)
           {
               asn1Copy_s1ap_NAS_PDU(&p_ue_context->s1ap_asn1_ctx,
                   &p_msg->nas_pdu,
                   &p_ue_context->p_curr_proc_data->u.erab_release_command_data
                       .nAS_PDU);

               p_ue_context->p_curr_proc_data->u.erab_release_command_data.
                   m.nAS_PDUPresent = 1;
           }
           /* SPR:5471 start */
           if (RRC_SUCCESS != 
              uecc_fsm_erb_release_is_atleast_one_non_gbr_bearer_present(
                  p_ue_context,p_erab_list))
               {
                   RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                   "[s1ap_ERABReleaseResponse] unable to release NGBR");
               /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
                   if (RRC_SUCCESS !=
                      uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
                   {
                      RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                      "[s1ap_ERABReleaseResponse] delivery failure");
                   }
                   break;
               }
           /* SPR:5471 stop */
           /* ALL ERAB ID in S1AP ERAB REL CMD are UnKnown */
           if (RRC_NULL == p_erab_list->counter)
           {
               /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
               if (RRC_SUCCESS !=
               uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
               {
                   RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                   "[s1ap_ERABReleaseResponse] delivery failure");
               }
               break;
           }
        result = RRC_SUCCESS;
    } while(0);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

#ifdef ENDC_ENABLED
/****************************************************************************
 * Function Name  : uecc_fsm_check_partial_sgnb_rab_release 
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 * 
 * Output         : None
 *
 * Returns        : RRC_FAILURE :  if all the SCG bearers are present in
 *                                 erab release request
 *                  RRC_SUCCESS :  if some of the SCG bearers are present in
 *                                 erab release request
 *
 * Description    : This function checks how many SCG beareres are present in 
 *                  erab release request.
 *
 ****************************************************************************/
rrc_return_t uecc_fsm_check_partial_sgnb_rab_release
(
        uecc_ue_context_t                       *p_ue_context
)
{
    U8            drb_release_count       = RRC_ZERO;
    U8            erab_release_id         = RRC_ZERO;
    U8            sgnb_rab_release_count  = RRC_ZERO;
    U8            erab_index              = RRC_ZERO;
    rrc_return_t  ret_val                 = RRC_FAILURE;
    
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if ( PNULL != p_ue_context->p_curr_proc_data )
    {
        drb_release_count =
            p_ue_context->p_curr_proc_data->u.erab_release_command_data.erab_release_list.counter;

        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,
                "Total number of ERABs to be released [%d] \n",drb_release_count);

        for (erab_index = RRC_NULL;
                (erab_index < drb_release_count);
                erab_index++)
        {
            erab_release_id = p_ue_context->p_curr_proc_data->u.erab_release_command_data. \
                              erab_release_list.erab_item[erab_index].e_RAB_ID;
            /*check the bearer type of the erab to be deleted*/
            if(PNULL != p_ue_context->p_e_rab_list[erab_release_id])
            {
                if((p_ue_context->p_e_rab_list[erab_release_id]->m.DC_BearerTypePresent) &&
                        (
                         SN_TERMINATED_SCG_BEARER ==
                         p_ue_context->p_e_rab_list[erab_release_id]->dc_bearer_type
                         ||
                         SN_TERMINATED_SPLIT_BEARER ==
                         p_ue_context->p_e_rab_list[erab_release_id]->dc_bearer_type
                        )
                  )
                {
                    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,
                            "ERAB to be released [%d] is SGNB ERAB [%d]\n",erab_release_id);
                    sgnb_rab_release_count++;
                }
            }
        }
        if ( sgnb_rab_release_count != RRC_NULL )
        {
            if(sgnb_rab_release_count == rrc_uecc_count_scg_bearer(p_ue_context) )
            {
                RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,
                        "All the SGNB RABS are in Erab release list\n");
            }
            else
            {
                RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,
                        "Partial SGNB RABS are in Erab release list\n");
                ret_val = RRC_SUCCESS;
            }
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return ret_val;
}
#endif

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_rrm_resource_deletion_
 *                  ongoing_init_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 *
 ****************************************************************************/
void uecc_fsm_erb_release_rrm_resource_deletion_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    /* SPR_21909 Fix - Start */
    s1ap_Cause cause;
    /* SPR_21909 Fix - Stop */
#ifdef ENDC_ENABLED
    rrc_bool_et partial_sgnb_rab_release = RRC_FALSE;
#endif
    rrc_bool_et failed_to_send_to_rrm    = RRC_FALSE;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

#ifdef ENDC_ENABLED
    p_ue_context->partial_sgnb_rab_release = RRC_FALSE;
    if (RRC_SUCCESS == uecc_fsm_check_partial_sgnb_rab_release(p_ue_context))
    {
        partial_sgnb_rab_release = RRC_TRUE;
        p_ue_context->partial_sgnb_rab_release = partial_sgnb_rab_release;
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_release_due_to_eutran_generated_reason;
    }
    else 
    {
#endif
    /* Build and sent the RRC_RRM_ERB_RELEASE_REQ to RRM */
      if ( RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_req(p_ue_context))
      {
        failed_to_send_to_rrm = RRC_TRUE;
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;
      }
#ifdef ENDC_ENABLED
    }
#endif
    
    if( 
#ifdef ENDC_ENABLED
        (partial_sgnb_rab_release == RRC_TRUE ) || (failed_to_send_to_rrm == RRC_TRUE)
#else
        (failed_to_send_to_rrm == RRC_TRUE)
#endif
      )
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "Build and Send ERB Release Req: Failed");
        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_ERAB_RELEASE_CMD_GUARD_TIMER, &p_ue_context->timers_data);
        /* SPR_21909 Fix - Start */
        /* set cause of each ERAB in erab_release_list */
        update_erab_release_list_with_cause(p_ue_context, &cause);
        /* append erab_release_list to erab_failed_to_release_list
         * in curr proc data and erab_release_list empty */
        append_erab_release_list_to_erab_failed_to_release_list(
            p_ue_context);

        #ifdef ENDC_ENABLED
        if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
        {
            /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
            if (RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                        RRC_NULL, RRC_NULL, PNULL, RRC_TRUE))
            {
                RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                        "[s1ap_ERABReleaseResponse] delivery failure");
            }
        }
        #else
        /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
        if (RRC_SUCCESS !=
            uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                RRC_NULL, RRC_NULL, PNULL, RRC_TRUE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[s1ap_ERABReleaseResponse] delivery failure");
        }
        #endif

        /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
        if (RRC_SUCCESS !=
            uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
                &cause))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[s1ap_UeContextReleaseRequest] delivery failure");
        }
        /* SPR_21909 Fix - Stop */
        #ifdef ENDC_ENABLED
        if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
        {
            /* Process failure*/
            uecc_fsm_finish_activity_failure(
                    p_ue_context, (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);
        }
        #else
        /* Process failure*/
        uecc_fsm_finish_activity_failure(
          p_ue_context, (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);
        #endif
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

#ifdef ENDC_ENABLED
/*EN-DC_changes_start*/
/****************************************************************************
 * Function Name  : rrc_uecc_check_sgnb_erab_list_to_release 
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  rrc_rrm_erb_release_resp_t* p_rrc_rrm_erb_release_resp 
 * Output         : None
 * Returns        : None
 * Description    : 
 *
 ****************************************************************************/
rrc_bool_et rrc_uecc_check_sgnb_erab_list_to_release(
        uecc_ue_context_t*          p_ue_context,
        rrc_rrm_erb_release_resp_t* p_rrc_rrm_erb_release_resp)
{
    U8  erab_index          = RRC_NULL;
    U8  drb_release_count   = RRC_NULL;
    U8  erab_release_id     = RRC_NULL;
    rrc_bool_et result      = RRC_FALSE;

    drb_release_count = 
        p_rrc_rrm_erb_release_resp->drb_release_item_list.num_of_list;

    for(erab_index = RRC_NULL; erab_index < drb_release_count; erab_index++)
    {
        erab_release_id = p_rrc_rrm_erb_release_resp->drb_release_item_list.\
                          drb_release_item[erab_index].erab_id;

        /*check the bearer type of the erab to be deleted*/
        if(PNULL != p_ue_context->p_e_rab_list[erab_release_id])
        {
            if((p_ue_context->p_e_rab_list[erab_release_id]->m.DC_BearerTypePresent) &&
/* OPTION3X Changes Start */
                    (
                     SN_TERMINATED_SCG_BEARER ==
                     p_ue_context->p_e_rab_list[erab_release_id]->dc_bearer_type
                     ||
                     SN_TERMINATED_SPLIT_BEARER ==
                     p_ue_context->p_e_rab_list[erab_release_id]->dc_bearer_type
                     )
/* OPTION3X Changes End */
              )
            {
                result = RRC_TRUE;
                break;
            }
            else
            {
                /*since the erab is not SN_TERMINATED_SCG_BEARER/SN_TERMINATED_SPLIT_BEARER, 
                 * break the loop and return*/
/* OPTION3X Changes Start */
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                        p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                        "[X2AP SgNB Release Req] ERAB ID[%d] is not SN_TERMINATED_SCG_BEARER/SN_TERMINATED_SPLIT_BEARER",
                        erab_release_id);
/* OPTION3X Changes End */

                result = RRC_FALSE;
            }
        }
    }
    return result;
}
/*EN-DC_changes_stop*/
#endif

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_rrm_erb_release_resp_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api_data
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_ERB_RELEASE_RESP from a RRM.
 *
 ****************************************************************************/
void uecc_fsm_erb_release_rrm_erb_release_resp_handler(
    uecc_ue_context_t*   p_ue_context,
    void*                p_api_data
)
{
    rrc_rrm_erb_release_resp_t*       p_rrc_rrm_erb_release_resp = PNULL;
    s1ap_Cause                        cause;
    rrc_return_t                      result = RRC_FAILURE;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_rrm_erb_release_resp = (rrc_rrm_erb_release_resp_t*)p_api_data;

    switch (p_rrc_rrm_erb_release_resp->response)
    {
        case SUCCESS:
#ifdef ENDC_ENABLED
           /*EN-DC_changes_start*/
           if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
           {
               if((RRC_TRUE == rrc_uecc_check_sgnb_erab_list_to_release(p_ue_context,
                               p_rrc_rrm_erb_release_resp)) &&
                       (RRC_NULL < rrc_uecc_count_scg_bearer(p_ue_context))
                       && (p_ue_context->x2apUecontextReleased != RRC_TRUE))
               {
                   fill_sgnb_release_cause(&p_ue_context->sgnb_cause,&p_ue_context->share_data.p_uecc_rm_data->cause);
                   UECC_FSM_SET_CHILD_STATE(p_ue_context, &uecc_fsm_sgnb_release_ongoing, PNULL);
               }
               else
                   /*EN-DC_changes_end*/
#endif
               {
                   UECC_FSM_SET_CHILD_STATE(p_ue_context,
                           &uecc_fsm_erb_release_llim_reconfigure_ue_ongoing, 
                           PNULL);
               }
#ifdef ENDC_ENABLED
           }
#endif

           /* SPS Start */
           if (p_rrc_rrm_erb_release_resp->bitmask &
                   RRC_RRM_ERB_RELEASE_RESP_SPS_CONFIG_PRESENT)
           {
               if (RRC_FAILURE == uecc_rrm_parse_and_update_sps_config(
                           p_ue_context,
                           &p_rrc_rrm_erb_release_resp->sps_config))
               {
                  RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                          p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                         "[RRC_RRM_ERB_RELEASE_RESP] SPS config updation failed");
               }
           }

           /* SPS Stop */

           /*SPR_19066_START*/
           if (RRM_RRM_ERB_RELEASE_RESP_UE_INACTIVE_TIME_PRESENT
                   & p_rrc_rrm_erb_release_resp->bitmask)
           {
               p_ue_context->m.ue_inactive_time_updated = RRC_TRUE;
               p_ue_context->ue_inactive_time = 
                   p_rrc_rrm_erb_release_resp->ue_inactive_time_config;
           }
           /*SPR_19066_END*/

           result = RRC_SUCCESS;
            break;

        case FAILURE:
           /* RRM Fail to release ERAB list should be present */

           /* Stop timer */
           uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                   UECC_ERAB_RELEASE_CMD_GUARD_TIMER, 
               &p_ue_context->timers_data);
       
           result = uecc_fsm_erb_release_process_rrm_erb_release_resp_data(
                        p_ue_context,
                        p_rrc_rrm_erb_release_resp);

          if (result == RRC_SUCCESS)
          {
#ifdef ENDC_ENABLED
              /*EN-DC_changes_start*/
              if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
              {
#endif
                  /* cause will be used in ue ctxt release req */
                  if (RRC_FAILURE == uecc_s1ap_build_s1ap_cause(
                              &cause, &p_rrc_rrm_erb_release_resp->
                              drb_failed_to_release_item_list.
                              drb_failed_to_release_item[0].cause,
                              p_ue_context))
                  {
                      RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                              p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                              "[RRC_RRM_ERB_RELEASE_RESP] Invalid cause type");
                  }

                  /* Build and send the RRC_S1AP_E_RAB_RELEASE_RESPONSE msg 
                   * to S1AP */
                  if (RRC_SUCCESS !=
                          uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                              RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
                  {
                      RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                              "[s1ap_ERABReleaseResponse] delivery failure");
                  }
                  /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
                  if (RRC_SUCCESS !=
                          uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
                              &cause))
                  {
                      RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                              "[s1ap_UeContextReleaseRequest] delivery failure");
                  }
#ifdef ENDC_ENABLED
                  /*EN-DC_changes_start*/
              }
#endif
          }
            break;

        case PARTIAL_SUCCESS:
           /* SPS Start */
           if (p_rrc_rrm_erb_release_resp->bitmask &
                   RRC_RRM_ERB_RELEASE_RESP_SPS_CONFIG_PRESENT)
           {
               if (RRC_FAILURE == uecc_rrm_parse_and_update_sps_config(
                           p_ue_context,
                           &p_rrc_rrm_erb_release_resp->sps_config))
               {
                  RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                          p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                          "[RRC_RRM_ERB_RELEASE_RESP] SPS config updation failed");
               }
           }

           /* SPS Stop */
           /* RRM Release and Fail to release ERAB list should be present */
           result = uecc_fsm_erb_release_process_rrm_erb_release_resp_data(
                        p_ue_context,
                        p_rrc_rrm_erb_release_resp);

                if (RRC_SUCCESS == result)
                {
                    /*SPR_19066_START*/
                    if (RRM_RRM_ERB_RELEASE_RESP_UE_INACTIVE_TIME_PRESENT
                            & p_rrc_rrm_erb_release_resp->bitmask)
                    {
                        p_ue_context->m.ue_inactive_time_updated = RRC_TRUE;
                        p_ue_context->ue_inactive_time = 
                            p_rrc_rrm_erb_release_resp->ue_inactive_time_config;
                    }
                    /*SPR_19066_END*/
#ifdef ENDC_ENABLED
                    /*EN-DC_changes_start*/
                    if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
                    {
                        if((RRC_TRUE == rrc_uecc_check_sgnb_erab_list_to_release(p_ue_context,
                                        p_rrc_rrm_erb_release_resp)) &&
                                (RRC_NULL < rrc_uecc_count_scg_bearer(p_ue_context)))
                        {
                            fill_sgnb_release_cause(&p_ue_context->sgnb_cause,&p_ue_context->share_data.p_uecc_rm_data->cause);
                            UECC_FSM_SET_CHILD_STATE(p_ue_context, &uecc_fsm_sgnb_release_ongoing, PNULL);
                        }
                        else
                        {
                            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                                    &uecc_fsm_erb_release_llim_reconfigure_ue_ongoing, 
                                    PNULL);
                        }
                        /*EN-DC_changes_end*/
                    }
#else
                    UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_erb_release_llim_reconfigure_ue_ongoing,
                        PNULL);
#endif
                    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                    return;
                }
            break;

        default:
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_ERB_RELEASE_RESP] Invalid Response=%i",
                p_rrc_rrm_erb_release_resp->response);
            break;
    }

#ifdef ENDC_ENABLED
    /*EN-DC_changes_start*/
    if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
    {
#endif
        /* If response from RRC_FAILURE */
        if (RRC_SUCCESS == result && SUCCESS != 
                p_rrc_rrm_erb_release_resp->response)
        {
            /* Process failure*/
            uecc_fsm_finish_activity_failure(
                    p_ue_context, (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);

        }

        /* If response code is RRC_FAILURE or RRC_PARTIAL_SUCCCESS */
        if (RRC_SUCCESS != result && SUCCESS != 
                p_rrc_rrm_erb_release_resp->response)
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_ERB_RELEASE_RESP] Mandatory List Absent, Response=%i",
                    p_rrc_rrm_erb_release_resp->response);
            /* Process failure*/

            uecc_fsm_finish_activity_failure(
                    p_ue_context, (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);
        }
#ifdef ENDC_ENABLED
    }
#endif

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_
 *                  init_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 *
 ****************************************************************************/
void uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


    /* Send Delete LC request to LLIM */
    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_delete_lc_req(p_ue_context))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[Build_and_send_delete_lc_req] delivery failure");

        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_ERAB_RELEASE_CMD_GUARD_TIMER, &p_ue_context->timers_data);

        /* set cause due to build and send for ERAB Mgnt Procedures */
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_unspecified_2;

        /* set cause of each ERAB in erab_release_list */
        update_erab_release_list_with_cause(p_ue_context,
            &cause);

        /* Error Code by which list is updated */
        error_code |= RRC_MODULE_ID << 16;
        error_code |= RRC_MEMORY_ALLOCATION_FAILURE;

        /* set error code of each ERAB in erab release list */
        update_erab_release_list_with_error_code(p_ue_context,
            &error_code);

        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
            p_ue_context, FAILURE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }

        /* append erab_release_list to erab_failed_to_release_list
         * in curr proc data and erab_release_list empty */
        append_erab_release_list_to_erab_failed_to_release_list(
            p_ue_context);

        /* Build and Send S1AP_ERAB_RELEASE_RESPONSE */
        if (RRC_SUCCESS !=
            uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[s1ap_ERABReleaseResponse] delivery failure");
        }

        /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
        if (RRC_SUCCESS !=
            uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
            &cause))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[s1ap_UeContextReleaseRequest] delivery failure");
        }

        uecc_fsm_finish_activity_failure(
            p_ue_context,
            (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);

    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_llim_reconfigure_ue_resp_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_RECONFIGURE_UE_RESP from LLIM
 *
 ****************************************************************************/
void uecc_fsm_erb_release_llim_reconfigure_ue_resp_handler(
    uecc_ue_context_t*    p_ue_context,
    void*                 p_api
)
{
    rrc_uecc_llim_delete_lc_resp_t*  p_rrc_uecc_llim_delete_lc_resp = PNULL;
    s1ap_Cause    cause;
    rrc_return_t   result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_LLIM_RECONFIGURE_UE_RESP");

    p_rrc_uecc_llim_delete_lc_resp = (rrc_uecc_llim_delete_lc_resp_t*)p_api;

    /* Set s1ap_Cause use to send S1AP */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_unspecified_2;

    do{
        result = storeLlimInfo_InUeContext_processErabReleaseReq(&cause, 
                        p_ue_context, p_rrc_uecc_llim_delete_lc_resp);
    } while(0);

    /* Process failure */
    if (RRC_SUCCESS != result)
    {
        uecc_fsm_finish_activity_failure(
            p_ue_context,
            (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_ue_reconfigure_ongoing_init_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_erb_release_ue_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and Send RRCConnectionReconfiguration */
    #ifdef ENDC_ENABLED
    if ( ERAB_REL == p_ue_context->release_type )
    {
        if (PNULL == p_ue_context->p_curr_proc_data)
        {
            p_ue_context->p_curr_proc_data = 
                (curr_proc_data_t*)rrc_mem_get(sizeof(curr_proc_data_t));

            if (PNULL == p_ue_context->p_curr_proc_data)
            {
                RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_BRIEF,
                        "[CURR_PROC_DATA] Memory Alocation failed(E-RAB Release ENDC )");
            }
            else 
            {
                RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,
                        "[CURR_PROC_DATA] UECC_ERAB_RELEASE_PROC ");
                p_ue_context->p_curr_proc_data->t = UECC_ERAB_RELEASE_PROC;
            }
        }
        else 
        {
            p_ue_context->p_curr_proc_data->t = UECC_ERAB_RELEASE_PROC;
        }
    }
    #endif

    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_rrc_connection_reconfiguration(p_ue_context))
    {
        #ifdef ENDC_ENABLED
        if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
        {
        #endif
        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_ERAB_RELEASE_CMD_GUARD_TIMER, &p_ue_context->timers_data);
        #ifdef ENDC_ENABLED
        }
        #endif

        /* set cause due to build and send for ERAB Mgnt Procedures */
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_unspecified_2;

        /* set cause of each ERAB in erab_release_list */
        update_erab_release_list_with_cause(p_ue_context,
            &cause);

        error_code |= RRC_MODULE_ID << 16;
        error_code |= RRC_MEMORY_ALLOCATION_FAILURE; 

        /* set error code of each ERAB in erab_release_list */
        update_erab_release_list_with_error_code(p_ue_context,
            &error_code);
        /*coverity fix 277496*/
        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
        /* coverity_277496_fix_start */
        if ( PNULL != p_ue_context->p_curr_proc_data )
        /* coverity_277496_fix_end */
        {
            if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                        p_ue_context, FAILURE))
            {
                RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                        "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
            }
        }
        /*coverity fix 277496*/
        /* append erab_release_list to erab_failed_to_release_list
         * in curr proc data and erab_release_list empty */
        append_erab_release_list_to_erab_failed_to_release_list(
            p_ue_context);

        #ifdef ENDC_ENABLED
        if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
        {
#endif
            /* Build and Send S1AP_ERAB_RELEASE_RESPONSE */
            if (RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                        RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
            {
                RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                        "[s1ap_ERABReleaseResponse] delivery failure");
            }
#ifdef ENDC_ENABLED
        }
        #endif

        /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
        if (RRC_SUCCESS !=
            uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
            &cause))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[s1ap_UeContextReleaseRequest] delivery failure");
        }

        #ifdef ENDC_ENABLED
        if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
        {
        uecc_fsm_finish_activity_failure(
            p_ue_context,
            (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);
        }
        else
        {
        uecc_fsm_finish_activity_failure(
            p_ue_context,
            (void*)UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT);
        }
        #else
        uecc_fsm_finish_activity_failure(
            p_ue_context,
            (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_FAILURE_INT);
        #endif
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_rrc_connection_
 *                  reconfiguration_complete_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE 
 *
 ****************************************************************************/
void uecc_fsm_erb_release_rrc_connection_reconfiguration_complete_handler(
    uecc_ue_context_t*   p_ue_context,
    void*                p_api
)
{
    erab_list_t*    p_failed_erab_list = PNULL; 
    U16              resp_counter = RRC_NULL;
#ifdef ENDC_ENABLED
   /*EN-DC_changes_start*/
    U16             erab_idx = RRC_ZERO;
    U16             e_rab_id = RRC_ZERO;
   /*EN-DC_changes_end*/
#endif
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE");
#ifdef ENDC_ENABLED
    /*EN-DC_changes_start*/
    for(erab_idx = 0;
            erab_idx < p_ue_context->p_curr_proc_data->u.erab_release_command_data.
            erab_release_list.counter;
            erab_idx++)
    {
        e_rab_id = p_ue_context->p_curr_proc_data->u.erab_release_command_data.
            erab_release_list.erab_item[erab_idx].e_RAB_ID;
        if ( (RRC_NULL != p_ue_context->p_e_rab_list[e_rab_id] )
                &&
                (RRC_ONE == p_ue_context->p_e_rab_list[e_rab_id]->m.DC_BearerTypePresent)
                &&
                (((SN_TERMINATED_SCG_BEARER == p_ue_context->p_e_rab_list[e_rab_id]->dc_bearer_type) ||
                 /*OPTION3X Changes Start*/
                (SN_TERMINATED_SPLIT_BEARER == p_ue_context->p_e_rab_list[e_rab_id]->dc_bearer_type)) &&
                 /*OPTION3X Changes Stop*/
                (RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag))
           )
        {
            if(RRC_SUCCESS != uecc_fsm_ue_context_release_handler(p_ue_context))
            {
                RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_WARNING,
                        "[%s]X2AP ue_context_release"
                        " failed !!",__FUNCTION__);
            }
            break;
        }
    }
    /*EN-DC_changes_end*/
#endif
    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_ERAB_RELEASE_CMD_GUARD_TIMER, &p_ue_context->timers_data);

    #ifdef ENDC_ENABLED
    if ( RRC_TRUE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
    {
        uecc_fsm_x2ap_ue_context_release_handler(p_ue_context);
        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_SUCCESS to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                    p_ue_context, SUCCESS))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                    "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }
    }
    else
    {
    #endif

    p_failed_erab_list = &p_ue_context->p_curr_proc_data->u.
        erab_release_command_data.erab_failed_to_release_list;
    resp_counter = p_ue_context->p_curr_proc_data->u.
        erab_release_command_data.counter_after_rrm_resp;

    if (resp_counter == p_failed_erab_list->counter)
    {
        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_SUCCESS to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
            p_ue_context, SUCCESS))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }
    }
    else
    {
        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
            p_ue_context, PARTIAL_SUCCESS))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }

    }
#ifdef ENDC_ENABLED
    }
#endif


    #ifdef ENDC_ENABLED
    if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
    {
        /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                    RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                    "[s1ap_ERABReleaseResponse] delivery failure");
        }
    }
    #else
    /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                       RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
            "[s1ap_ERABReleaseResponse] delivery failure");
    }
    #endif

    /* Call success handler for procedure */
    uecc_fsm_finish_activity_success(p_ue_context, PNULL);

    uecc_llim_check_and_send_rrc_ue_information(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_process_rrm_erb_release_resp_data
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 *
 * Description    : Handling of RRC_RRM_ERB_RELEASE_RESP message data
 *
 ****************************************************************************/
rrc_return_t uecc_fsm_erb_release_process_rrm_erb_release_resp_data(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    erab_list_t*                           p_erab_list = PNULL;
    rrc_rrm_erb_release_resp_t* p_rrm_erb_release_resp = PNULL;
    U16*                p_resp_count = PNULL;
    rrc_return_t result = RRC_SUCCESS;
    U8 counter = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrm_erb_release_resp = (rrc_rrm_erb_release_resp_t*)p_api;

    /* Append E-RAB Rlease List to failed list if present */
    if (FAILURE == p_rrm_erb_release_resp->response)
    {
        if (RRC_RRM_DRB_FAILED_TO_RELEASE_ITEM_LIST_PRESENT &
            p_rrm_erb_release_resp->bitmask)
        {
            /* point to erab_failed_to_release_list in 
             * erab_release_command_data */
            p_erab_list = &p_ue_context->p_curr_proc_data->u.
                erab_release_command_data.erab_failed_to_release_list;

            for (counter = 0;
                 counter < p_rrm_erb_release_resp->
                 drb_failed_to_release_item_list.num_of_list;
                 counter++, p_erab_list->counter++)
            {
                p_erab_list->erab_item[p_erab_list->counter].e_RAB_ID =
                    p_rrm_erb_release_resp->drb_failed_to_release_item_list.
                    drb_failed_to_release_item[counter].erab_id;

                /* rrm_cause_t to s1ap_cause type */
                if (RRC_FAILURE == uecc_s1ap_build_s1ap_cause(
                    &p_erab_list->erab_item[p_erab_list->counter].
                    cause, &p_rrm_erb_release_resp->
                    drb_failed_to_release_item_list.
                    drb_failed_to_release_item[counter].cause,
                    p_ue_context))
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                            p_ue_context->p_gb_context->facility_name ,RRC_WARNING,
                        "[RRC_RRM_ERB_RELEASE_RESP] Invalid cause type");
                }
            }
            p_erab_list = &p_ue_context->p_curr_proc_data->u.
                erab_release_command_data.erab_release_list;
            /* empty erab_release_list */
            p_erab_list->counter = RRC_NULL;
        }
        else
            result = RRC_FAILURE;
    }

    if (p_rrm_erb_release_resp->response == PARTIAL_SUCCESS)
    {
        if (RRC_RRM_DRB_RELEASE_ITEM_LIST_PRESENT &
            p_rrm_erb_release_resp->bitmask)
        {
            /* point to erab_release_list in 
             * erab_release_command_data */
            p_erab_list = &p_ue_context->p_curr_proc_data->u.
                erab_release_command_data.erab_release_list;

            memset_wrapper(p_erab_list, 0, sizeof(erab_list_t));

            p_erab_list->counter = 
                p_rrm_erb_release_resp->drb_release_item_list.
                num_of_list;

             for (counter = 0; 
                  counter < p_rrm_erb_release_resp->drb_release_item_list
                  .num_of_list;
                  counter++)
             {
                 p_erab_list->erab_item[counter].e_RAB_ID = 
                     p_rrm_erb_release_resp->drb_release_item_list.
                     drb_release_item[counter].erab_id;
             }
         }
         else
             result = RRC_FAILURE;

         if (RRC_RRM_DRB_FAILED_TO_RELEASE_ITEM_LIST_PRESENT &
             p_rrm_erb_release_resp->bitmask)
         {
             /* point to erab_failed_to_release_list in 
              * erab_release_command_data */
             p_erab_list = &p_ue_context->p_curr_proc_data->u.
                 erab_release_command_data.erab_failed_to_release_list;

             for (counter = 0; 
                  counter < p_rrm_erb_release_resp->
                  drb_failed_to_release_item_list.num_of_list;
                  counter++, p_erab_list->counter++)
             {
                 p_erab_list->erab_item[p_erab_list->counter].e_RAB_ID = 
                     p_rrm_erb_release_resp->drb_failed_to_release_item_list.
                     drb_failed_to_release_item[counter].erab_id;

                 /* rrm_cause_t to s1ap_cause type */
                 if (RRC_FAILURE == uecc_s1ap_build_s1ap_cause( 
                     &p_erab_list->erab_item[p_erab_list->counter].
                      cause, 
                      &p_rrm_erb_release_resp->drb_failed_to_release_item_list.
                      drb_failed_to_release_item[counter].cause,
                      p_ue_context))
                 {
                     RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                             p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                         "[RRC_RRM_ERB_RELEASE_RESP] Invalid cause type");
                 }
             }
             p_resp_count = &p_ue_context->p_curr_proc_data->u.
                 erab_release_command_data.counter_after_rrm_resp;
             *p_resp_count = p_erab_list->counter;
         }
         else
         result = RRC_FAILURE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}
/* Local Functions For General Use */
/****************************************************************************
 * Function Name  : update_erab_release_list_with_cause
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_cause
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This function update erab_release_list in curr
 *                  proc data with received casue (p_cause)
 *
 ****************************************************************************/
void update_erab_release_list_with_cause(
    uecc_ue_context_t* p_ue_context,     
    void*              p_cause
)
{
    erab_list_t* p_erab_list = PNULL;
    U8 erab_counter = RRC_NULL;
    s1ap_Cause   cause;
    
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_cause);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /* SPR 20383 Fix Start */
    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO, "UPDATE ERAB_RELEASE_LIST: "
            "WITH S1AP CAUSE");
    /* SPR 20383 Fix End */

    /* update cause of each ERAB in erab_release_list
     * with received s1ap_Cause as parameter */
    p_erab_list = &p_ue_context->p_curr_proc_data->u.
        erab_release_command_data.erab_release_list;

    /* update by this s1ap_Cause */
    cause = *(s1ap_Cause*)p_cause;

    /* SPR_20915 - Fix - Starts */
    for (erab_counter = 0; 
        ((erab_counter < p_erab_list->counter) && (MAX_ERAB_COUNT > erab_counter)); 
        erab_counter++)
    /* SPR_20915 - Fix - Ends */
    {
        p_erab_list->erab_item[erab_counter].cause = cause;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : update_erab_release_list_with_error_code
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_error_code
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This function update erab_release_list in curr
 *                  proc data with received error code
 *
 ****************************************************************************/
void update_erab_release_list_with_error_code(
    uecc_ue_context_t* p_ue_context,
    void*              p_error_code
)
{
    erab_list_t* p_erab_list = PNULL;
    U8 erab_counter = RRC_NULL;
    U32      error_code;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_error_code);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO, "UPDATE ERAB_RELEASE_LIST:" 
            "WITH ERROR CODE");

    /* update cause of each ERAB in erab_release_list
     * with received s1ap_Cause as parameter */
    p_erab_list = &p_ue_context->p_curr_proc_data->u.
        erab_release_command_data.erab_release_list;

    /* update by this s1ap_Cause */
    error_code = *(U32*)p_error_code;

    /* SPR_20915 - Fix - Starts */
    for (erab_counter = 0; 
        ((erab_counter < p_erab_list->counter) && (MAX_ERAB_COUNT > erab_counter)); 
        erab_counter++)
    /* SPR_20915 - Fix - Ends */
    {
        p_erab_list->erab_item[erab_counter].error_code = error_code;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : append_erab_release_list_to_erab_failed_to_release_list
 *
 * Input          : uecc_ue_context_t* p_ue_context
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This function append erab_release_list to erab_failed_to_release_list
 *                  in curr proc data in ue context
 *
 ****************************************************************************/
void append_erab_release_list_to_erab_failed_to_release_list(
    uecc_ue_context_t* p_ue_context
)
{
    erab_list_t* p_erab_list = PNULL;
    erab_list_t* p_erab_failed_release_list = PNULL;
    
    U8 erab_counter = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO, "ERAB_RELEASE_LIST: APPEND");
    /* point to erab_release_list in curr proc data */
    p_erab_list = &p_ue_context->p_curr_proc_data->u.
        erab_release_command_data.erab_release_list;
    /* point to erab_failed_to_release_list in curr proc data */
    p_erab_failed_release_list = &p_ue_context->p_curr_proc_data->u.
        erab_release_command_data.erab_failed_to_release_list;

    for (erab_counter = 0;
        erab_counter < p_erab_list->counter;
        erab_counter++, p_erab_failed_release_list->counter++)
    {
        p_erab_failed_release_list->
        erab_item[p_erab_failed_release_list->counter].e_RAB_ID = 
        p_erab_list->erab_item[erab_counter].e_RAB_ID;

        p_erab_failed_release_list->
        erab_item[p_erab_failed_release_list->counter].cause = 
        p_erab_list->erab_item[erab_counter].cause;
    }
    /* erab_release_list empty */
    p_erab_list->counter = RRC_NULL;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : update_curr_proc_data_with_response_code
 *
 * Input          : uecc_ue_context_t* p_ue_context,
 *                  void*              p_response_code
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This function update curr proc data with received 
 *                  rrc_uecc_llim_delete_lc_resp from LLIM.
 ****************************************************************************/
void update_curr_proc_data_with_response_code(
    uecc_ue_context_t* p_ue_context,
    void*              p_response_code
)
{
    erab_list_t*    p_erab_list = PNULL;
    erab_list_t*    p_erab_failed_list = PNULL;
    rrc_uecc_llim_delete_lc_resp_t*    p_rrc_uecc_llim_delete_lc_resp = PNULL;
    s1ap_Cause      cause;
    U8              resp_counter = RRC_NULL;
    U8              erab_counter = RRC_NULL;
    /*CR 101 changes start*/
    U8 lc_counter = 0;
    /*CR 101 changes stop*/

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_response_code);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO, "[CURR PROC DATA UPDATED]:"
        "By DELETE LC RESP");

    p_rrc_uecc_llim_delete_lc_resp = 
        (rrc_uecc_llim_delete_lc_resp_t*)p_response_code;
    p_erab_list = &p_ue_context->p_curr_proc_data->u.
        erab_release_command_data.erab_release_list;
    p_erab_list->counter = RRC_NULL;

    p_erab_failed_list = &p_ue_context->p_curr_proc_data->u.
        erab_release_command_data.erab_failed_to_release_list;

    /* Set s1ap_Cause use to send S1AP */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_unspecified_2;

    for (resp_counter = 0;
        resp_counter < p_rrc_uecc_llim_delete_lc_resp->
            delete_drb_entity_resp_counter;
        resp_counter++)
    {
        for (erab_counter = 0; erab_counter < MAX_ERAB_COUNT && 
                p_erab_list->counter < MAX_ERAB_COUNT;
             erab_counter++)
        {
            if ((PNULL != p_ue_context->p_e_rab_list[erab_counter]) &&
                    (MAX_ERAB_COUNT > p_erab_list->counter))
            {

                if (p_ue_context->p_e_rab_list[erab_counter]->
                        drb_config.logical_channel_identity ==
                        p_rrc_uecc_llim_delete_lc_resp->
                        delete_drb_entity_resp[resp_counter].lc_id)
                {
                    if (PARTIAL_SUCCESS ==
                        p_rrc_uecc_llim_delete_lc_resp->response_code)
                    {
                        if (SUCCESS == 
                            p_rrc_uecc_llim_delete_lc_resp->
                            delete_drb_entity_resp[resp_counter].
                            response)
                        {
                            p_erab_list->erab_item[p_erab_list->counter].
                                e_RAB_ID = 
                                p_ue_context->p_e_rab_list[erab_counter]->
                                e_RAB_ID;

                            /* CR 101 changes start*/
                            for (lc_counter = 0; lc_counter < p_rrc_uecc_llim_delete_lc_resp->
                                    delete_lc_status_counter; lc_counter++)
                            {
                                if (p_rrc_uecc_llim_delete_lc_resp->
                                        delete_drb_entity_resp[resp_counter].lc_id == 
                                        p_rrc_uecc_llim_delete_lc_resp->
                                        delete_lc_status[lc_counter].lch_id)
                                {
                                    if (p_rrc_uecc_llim_delete_lc_resp->delete_lc_status[lc_counter].
                                            status == MAC_DELETE_LC_ACTIVE)
                                    {
                                        p_erab_list->erab_item[p_erab_list->counter].
                                            lc_status = MAC_DELETE_LC_ACTIVE;
                                    }
                                }
                            }
                            /* CR 101 changes stop*/
                            p_erab_list->counter++;
                            break;
                        }
                        else 
                        {
                            p_erab_failed_list->
                                erab_item[p_erab_failed_list->counter].
                                e_RAB_ID = 
                                p_ue_context->p_e_rab_list[erab_counter]->
                                e_RAB_ID;

                            p_erab_failed_list->
                                erab_item[p_erab_failed_list->counter].cause = 
                                cause;
                            p_erab_failed_list->
                                erab_item[p_erab_failed_list->counter].
                                error_code = 
                                p_rrc_uecc_llim_delete_lc_resp->
                                delete_drb_entity_resp[resp_counter].response; 
                            p_erab_failed_list->counter++;
                            break; 
                        }
                    }
                    else /* If response code is either 
                          * ERAB_FATAL or ERAB_FAILURE */
                    {
                        
                        p_erab_list->erab_item[p_erab_list->counter].
                            e_RAB_ID = p_ue_context->p_e_rab_list[erab_counter]->
                            e_RAB_ID;

                        p_erab_list->erab_item[p_erab_list->counter].cause =
                            cause;

                        p_erab_list->erab_item[p_erab_list->counter].
                            error_code = p_rrc_uecc_llim_delete_lc_resp->
                            delete_drb_entity_resp[resp_counter].response;

                        p_erab_list->counter++;
                        break;
                    }
                }
            }
        }
        if (erab_counter == MAX_ERAB_COUNT)
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_DELETE_LC_RESP]:Invalid LC Id [%i]",
                p_rrc_uecc_llim_delete_lc_resp->
                delete_drb_entity_resp[resp_counter].lc_id);
        }
                  
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_ongoing_ho_required_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api
 * 
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_HO_REQUIRED
 ******************************************************************************/
void uecc_fsm_erb_release_ongoing_ho_required_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* In this function HO failure indication is sent to RRM */

   RRC_UECC_ERB_RELEASE_FSM_TRACE (RRC_INFO,
            "[UECC_EV_RRM_HO_REQUIRED]" 
            "MME Initiated E-RAB Release Procedure Ongoing"
            "Cancel the HO triggered by RRM");

    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->ho_info.cause.u.radioNetwork = s1ap_interaction_with_other_procedure;
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_failure( p_ue_context))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                "Fail to send the handover failure indication to RRM");
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

    


#ifdef ENDC_ENABLED
/*EN-DC_changes_start*/
/****************************************************************************
 * Function Name  : uecc_fsm_sgnb_rel_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_X2AP_SGNB_REL_REQ message.
 ****************************************************************************/
static
void uecc_fsm_sgnb_rel_req_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*                         api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
            "Send SGNB RELEASE REQUEST to SgNB \n");
    if (RRC_SUCCESS != 
            build_and_send_sgnb_release_request(p_ue_context
                ))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_WARNING,
                "[%s]uecc_x2ap_build_and_send_ue_context_release failed !!");
    }


}
/* MENB CHANGES - START */
/****************************************************************************
 * Function Name  : uecc_fsm_sgnb_release_ack_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_X2AP_SGNB_RELEASE_ACK message.
 ****************************************************************************/
static
void uecc_fsm_sgnb_release_ack_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    U8     release_type = RRC_NULL;
/* OPTION3X Changes Start */
    rrc_bool_t  llim_reconfigure_required = RRC_FALSE;
    U16         erab_idx                  = RRC_NULL;
    uecc_drb_ctx_t      *p_erab           = RRC_NULL;
/* OPTION3X Changes End */
    rrc_x2ap_sgnb_release_request_ack_t * 
        p_sgnb_release_request_ack = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
            "Received SGNB RELEASE REQUEST ACK\n");

    p_sgnb_release_request_ack = 
        (rrc_x2ap_sgnb_release_request_ack_t *)p_api;
    if(p_sgnb_release_request_ack != NULL)
    {
        /*parse menb_ue_id and sgnb_ue_id*/
        if ( ( p_ue_context->x2ap_context.menb_ue_x2ap_id !=
                   p_sgnb_release_request_ack->menb_ue_x2ap_id ) ||
             ( p_ue_context->x2ap_context.sgnb_ue_x2ap_id !=
                   p_sgnb_release_request_ack->sgnb_ue_x2ap_id ) )
        {
        	RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                    " SgNB Release Request Ack "
                    "MeNB_UE_X2AP_ID Mismatch");
        }
    }
/* OPTION3X Changes Start */
    /* 
     * Since only one erab is supported on SgNB 
     * and if only 1 ERAb release is triggred, that too can be a SN_TERMINATED_SPLIT_BEARER
     * thus need to check if LLIM Reconfig (for MN-BEARER/ SN_TERMINATED_SPLIT_BEARER) is required.
     */
    if (RRC_NULL != p_ue_context->p_curr_proc_data)
    {
        for(erab_idx = 0;
                erab_idx < p_ue_context->p_curr_proc_data->u.
                erab_release_command_data.erab_release_list.counter
                &&
                erab_idx < MAX_ERAB_COUNT;
                erab_idx++)
        {
            release_type = rrc_check_erab_for_bearer_or_ue_release_trigger(p_ue_context);
            p_ue_context->release_type = release_type;

            p_erab = p_ue_context->p_e_rab_list[
                p_ue_context->p_curr_proc_data->u.
                erab_release_command_data.erab_release_list.erab_item[erab_idx].e_RAB_ID];
            if( RRC_NULL != p_erab &&
                    !(RRC_ONE == p_erab->m.DC_BearerTypePresent &&
                        SN_TERMINATED_SCG_BEARER == p_erab->dc_bearer_type)
              )
            {
                llim_reconfigure_required = RRC_TRUE;
                break;
            }
        }

        if( RRC_TRUE == llim_reconfigure_required)
        {
            /* Switch  state */
            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                    &uecc_fsm_erb_release_llim_reconfigure_ue_ongoing, 
                    PNULL);
        }
        else
        {
            /* Switch  state */
            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                    &uecc_fsm_erb_release_ue_reconfigure_ongoing, PNULL);
        }

    }
/* OPTION3X Changes End */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_context_release_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called for sgnb UE context release
 ****************************************************************************/
rrc_return_t  uecc_fsm_ue_context_release_handler(
        uecc_ue_context_t*  p_ue_context
)
{
    rrc_return_t result = RRC_SUCCESS;
    s1ap_Cause cause;
    /* Send UE CONTEXT RELEASE to SgNB */
    if(RRC_FALSE == p_ue_context->x2apUecontextReleased)
    {
        if (RRC_SUCCESS != 
                uecc_x2ap_build_and_send_ue_context_release(p_ue_context
                    ))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_WARNING,
                    "[%s]uecc_x2ap_build_and_send_ue_context_release"
                    " failed !!",__FUNCTION__);
            result = RRC_FAILURE;
        }
        /*NR_DC Code Changes Start*/
        else
        {
            /* Send X2AP UE Context Released True */
            p_ue_context->x2apUecontextReleased = RRC_TRUE;
        }
    }
    /*NR_DC Code Changes End*/
    if(RRC_TRUE == p_ue_context->rel_proc_fail)
    {
        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_ERAB_RELEASE_CMD_GUARD_TIMER, &p_ue_context->timers_data);

        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                    p_ue_context, FAILURE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                    "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }
    
        /* append erab_release_list to erab_failed_to_release_list
         * in curr proc data and erab_release_list empty */
        append_erab_release_list_to_erab_failed_to_release_list(
                p_ue_context);

        /* Build and Send S1AP_ERAB_RELESE_RESPONSE */
        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_e_rab_release_response(p_ue_context,
                    RRC_NULL,RRC_NULL,PNULL,RRC_TRUE))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                    "[s1ap_ERABReleaseResponse] delivery failure");
            result = RRC_FAILURE;
        }

        /* cause will be used in ue ctxt release req */
        /*currently cause filling as type to: radioNetwork, value to: unspecified in*/
        if (RRC_FAILURE == uecc_s1ap_build_s1ap_cause(
                    &cause, NULL,
                    p_ue_context))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                    p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                    "[RRC_RRM_ERB_RELEASE_RESP] default type to: radioNetwork, value to: unspecified");
        }

        /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
                    &cause))
        {
            RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                    "[s1ap_UeContextReleaseRequest] delivery failure");
            result = RRC_FAILURE;
        }
        uecc_fsm_finish_activity_success(p_ue_context, PNULL);
    }
    /* Call success handler for procedure */


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}
/****************************************************************************
 * Function Name  : uecc_fsm_sgnb_rel_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_X2AP_SGNB_RELEASE_ACK TIMER EXPIRY message.
 ****************************************************************************/
static
void uecc_fsm_sgnb_rel_ongoing_timer_expiry_handler
(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api

)
{
    s1ap_Cause    cause;
    U32  error_code = RRC_NULL;
    
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
            "SGNB RELEASE REQUEST ACK TIMER_EXPIRY\n");

    /* set cause at timer expiry for ERAB Mgnt Procedures */
    cause.t = T_s1ap_Cause_radioNetwork;
    /* SPR 15473 Fix Start */
    cause.u.radioNetwork = s1ap_unspecified_2;
    /* SPR 15473 Fix Stop */

    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_TIMER_EXPIRY; 
    
    /* set cause of each ERAB in erab_release_list */
    update_erab_release_list_with_cause(p_ue_context,
        &cause);
    
    /* update error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
        &error_code);

    /* coverity fix 254846 */
    //if(PNULL != p_ue_context)
    //{
    p_ue_context->rel_proc_fail = RRC_TRUE;
    //}
    /* coverity fix 254846 */
    /*Trigger sgnb UE context release after timer expiry*/
    /* coverity fix 277501 */
    if( RRC_FAILURE == uecc_fsm_ue_context_release_handler(p_ue_context))
    {
        RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
                "uecc_fsm_ue_context_release_handler Failed \n");
    }
    /* coverity fix 277501 */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
} 

/****************************************************************************
 * Function Name  : uecc_fsm_sgnb_rel_error_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_X2AP_ERROR_INDICATION message.
 ****************************************************************************/
static
void uecc_fsm_sgnb_rel_error_handler( 
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    U8     release_type = RRC_NULL;
/* OPTION3X Changes Start */
    rrc_bool_t  llim_reconfigure_required = RRC_FALSE;
    U16         erab_idx                  = RRC_NULL;
    uecc_drb_ctx_t      *p_erab           = RRC_NULL;
/* OPTION3X Changes End */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_RELEASE_FSM_TRACE(RRC_INFO,
            "Received Error Ind in rleease, so complete the release as SgNB has released the same");
/* OPTION3X Changes Start */
    /* 
     * Since only one erab is supported on SgNB 
     * and if only 1 ERAb release is triggred, that too can be a SN_TERMINATED_SPLIT_BEARER
     * thus need to check if LLIM Reconfig (for MN-BEARER/ SN_TERMINATED_SPLIT_BEARER) is required.
     */
    if (RRC_NULL != p_ue_context->p_curr_proc_data)
    {
        for(erab_idx = 0;
                erab_idx < p_ue_context->p_curr_proc_data->u.
                erab_release_command_data.erab_release_list.counter
                &&
                erab_idx < MAX_ERAB_COUNT;
                erab_idx++)
        {
            release_type = rrc_check_erab_for_bearer_or_ue_release_trigger(p_ue_context);
            p_ue_context->release_type = release_type;
        
            /* Set X2AP UE Context Released True */
            p_ue_context->x2apUecontextReleased = RRC_TRUE;

            p_erab = p_ue_context->p_e_rab_list[
                p_ue_context->p_curr_proc_data->u.
                erab_release_command_data.erab_release_list.erab_item[erab_idx].e_RAB_ID];
            if( RRC_NULL != p_erab &&
                    !(RRC_ONE == p_erab->m.DC_BearerTypePresent &&
                        SN_TERMINATED_SCG_BEARER == p_erab->dc_bearer_type)
              )
            {
                llim_reconfigure_required = RRC_TRUE;
                break;
            }
        }

        if( RRC_TRUE == llim_reconfigure_required)
        {
            /* Switch  state */
            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                    &uecc_fsm_erb_release_llim_reconfigure_ue_ongoing, 
                    PNULL);
        }
        else
        {
            /* Switch  state */
            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                    &uecc_fsm_erb_release_ue_reconfigure_ongoing, PNULL);
        }
    }
/* OPTION3X Changes End */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*EN-DC_changes_end*/
#endif    
