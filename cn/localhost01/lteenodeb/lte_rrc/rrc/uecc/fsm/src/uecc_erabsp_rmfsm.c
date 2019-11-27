/******************************************************************************
*
*   FILE NAME:
*       uecc_erabsp_rmfsm.c
*
*   DESCRIPTION:
*       UECC ERAB SETUP procedure implementation.
*
*
*   DATE            AUTHOR      REFERENCE       REASON
*   28 May 2010     Ajay       ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/


/****************************************************************************
 * Header File Includes
 ***************************************************************************/

#include "uecc_erabsp_rmfsm.h"
#include "uecc_common.h"
#include "uecc_llim.h"
#include "uecc_rrm.h"
#include "uecc_s1ap.h"
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
#include "uecc_x2ap.h"
/*NR_DC Code Change Stop*/
#include "uecc_erab_delfsm.h"
#endif
#include "uecc_logger.h"
#include "uecc_ed.h"
#include "uecc_mainfsm.h"
#include "uecc_fsm_engine.h"
#include "uecc_defaults.h"
#include "rrc_msg_mgmt.h"
#include "rrc_phy_intf.h"
/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,\
        p_ue_context->ue_index,\
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
        ##__VA_ARGS__)


#define SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context)\
    (p_ue_context->share_data.uecc_rcr_data.\
     rrc_connection_release_not_required_flag = RRC_TRUE)


extern rrc_return_t uecc_fsm_rm_process_rrm_erab_setup_resp_data(
           uecc_ue_context_t*  p_ue_context,
           rrc_rrm_erb_setup_resp_t* p_rrm_erb_setup_resp);

/* GDB Fix - compiler warning */
extern rrc_return_et uecc_llim_build_and_send_erb_setup_erab_del_rrc_connection_reconfiguration( uecc_ue_context_t*  p_uecc_ue_context );

/*SPR_16942_START*/
extern void uecc_update_mac_config_in_ue_context(
        uecc_ue_context_t*  p_ue_context,
        rrm_mac_config_t*   p_mac_config
        );
/*SPR_16942_END*/

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
extern rrc_return_t uecc_fsm_chk_if_sgnb_addition_reqd(
        uecc_ue_context_t*  p_ue_context,
        rrm_drb_to_add_info_list_t*  p_drb_to_add_list
        );
extern rrc_return_et uecc_x2ap_handle_sgnb_addition_ack_msg_rcvd(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);
/*NR_DC Code Change Stop*/
#endif

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/

/* State UECC_RRM_ERB_SETUP_RESOURCE_ADMISSION_ONGOING */
static void uecc_fsm_rm_rrm_erab_setup_resource_admission_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_rrm_erab_setup_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_rrm_erab_setup_resource_admission_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_rrm_erab_setup_ongoing_rrc_conn_reestab_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
    
static void uecc_fsm_rm_rrm_erab_setup_ongoing_ho_required_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_rrm_erab_setup_ongoing_rlf_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);


static void uecc_fsm_rm_rrm_erab_setup_ongoing_s1ap_reset_msg_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);


/* State UECC_LLIM_ERB_SETUP_RECONFIGURE_UE_ONGOING */
static void uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_llim_erab_setup_reconfigure_ue_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_rm_llim_erab_setup_ongoing_rrc_conn_reestab_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_rm_llim_erab_setup_ongoing_ho_required_handler(
 uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_llim_erab_setup_ongoing_rlf_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_erab_setup_ongoing_ue_rel_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_llim_erab_setup_ongoing_s1ap_reset_msg_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);


/* State UECC_LLIM_ERB_SETUP_ROLLBACK_ONGOING */
static void uecc_fsm_rm_llim_erab_setup_rollback_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_llim_erab_setup_rollback_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_llim_erab_setup_rollback_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_llim_erab_setup_rollback_ongoing_rrc_conn_reestab_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_rm_llim_erab_setup_rollback_ongoing_ho_required_handler(
 uecc_ue_context_t*  p_ue_context,             /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_llim_erab_setup_rollback_ongoing_rlf_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_llim_erab_setup_rollback_ongoing_s1ap_reset_msg_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);



/* State UECC_UE_ERB_SETUP_RECONFIGURE_ONGOING */
static void uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_erab_setup_rrc_connection_reconfiguration_complete_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);


static void uecc_fsm_rm_ue_erab_setup_ongoing_rrc_conn_reestab_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
    
static void uecc_fsm_rm_ue_erab_setup_ongoing_ho_required_handler(
 uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_ue_erab_setup_ongoing_rlf_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_ue_erab_setup_ongoing_s1ap_reset_msg_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);


/* Failure processing functions */
static void uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);


static void uecc_fsm_rm_erab_setup_fail_action_delete_drb_info_from_ctx(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);


static void uecc_fsm_rm_erab_setup_fail_action_last(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);


static void uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

static void uecc_fsm_rm_erab_setup_fail_fatal_error_action_last(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);


static void uecc_fsm_rm_erab_setup_fail_stop_proc_timer(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

void uecc_fsm_rm_erab_setup_build_s1ap_ho_type(
    U32 *s1ap_ho_type,
    U8  ho_type  /* HO TYPE */ 
);

static rrc_return_t uecc_fsm_rm_erab_setup_read_op_rrm_params(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_rrm_erb_setup_resp_t*  p_rrc_rrm_erb_setup_resp, /* ERAB SETUP RESP */
    uecc_rm_erab_setup_op_data_t*     p_erab_op_params     
);

static void uecc_fsm_rm_erab_setup_action_append_erab_to_fail_list(
    uecc_ue_context_t*  p_ue_context,
    rrc_response_t     responseCode
);

static void uecc_fsm_rm_erab_setup_fail_action_send_ue_ctx_rel_req(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

static void uecc_fsm_rm_fail_action_mark_all_erab_failed_with_cause(
        uecc_ue_context_t*  p_ue_context,
        U32                 error_code,
        s1ap_Cause          cause
);
void uecc_fsm_rm_llim_erab_setup_ho_prep_timer_expiry_handler 
(
 uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
 void*               p_api                  /* Input API buffer */
 ); 

static void uecc_fsm_rm_erab_setup_rollback_last_fail_action(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

void uecc_llim_build_erbsp_delete_lc_req(
    uecc_ue_context_t*             p_uecc_ue_context,
    rrc_uecc_llim_delete_lc_req_t* p_rrc_uecc_llim_erbsp_delete_lc_req
);
/* ARP CHANGES BEGIN */

/* State UECC_LLIM_ERBSP_DEL_RECONFIGURE_UE_ONGOING */
static void uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

static void uecc_rm_erabsp_update_erab_release_list_with_error_code(
        uecc_ue_context_t* p_ue_context,
        void*              p_error_code
);

static void uecc_fsm_rm_llim_erapsp_del_reconfigure_ue_ongoing_reestablishment_handler(
    uecc_ue_context_t*    p_ue_context,
    void*                 p_api
);


static void uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_rlf_handler(
    uecc_ue_context_t*    p_ue_context,
    void*                 p_api
);

static void uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_ho_required_handler(
    uecc_ue_context_t*    p_ue_context,
    void*                 p_api
);


static void uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_ue_rel_req_handler(
    uecc_ue_context_t*    p_ue_context,
    void*                 p_api
);


static void uecc_fsm_rm_erabsp_erab_release_fail_handler(
        uecc_ue_context_t*    p_ue_context,
        U32                   err_code,
        uecc_fsm_fail_handler_f const*  p_fail_actions_list
);

static void uecc_fsm_rm_ue_erabsp_erab_del_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_erabsp_erab_del_rrc_connection_reconfiguration_complete_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
/* ARP CHANGES END*/

/* SPR 13775 Fix Start */
extern void updateMacConfigInUeCtx(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        rrm_mac_config_t*   p_mac_config_dest, /*desc MAC config*/
        rrm_mac_config_t*   p_mac_config_src /*src MAC config*/
);

extern void updatePhyConfigDedInUeCtx(
        uecc_ue_context_t*  p_ue_context,   /* UECC UE context */
        rrc_phy_physical_config_dedicated_t*  p_phy_config_dest, /*dest PHY config*/
        rrc_phy_physical_config_dedicated_t*  p_phy_config_src /*src PHY config*/
);
/* SPR 13775 Fix Stop */

#ifdef ENDC_ENABLED
/*NR_DC Code Changes Start*/
static void uecc_fsm_rm_erabsp_sgnb_addition_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_erabsp_sgnb_addition_ongoing_ack_rcvd(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_erabsp_sgnb_addition_ongoing_timer_expiry(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_erabsp_sgnb_addition_ongoing_sgnb_add_req_reject(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_erabsp_sgnb_addition_ongoing_x2ap_error_ind(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_rrc_conn_reestab_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_ho_required_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_rlf_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_erabsp_sgnb_addition_ongoing_ue_rel_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_s1ap_reset_msg_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_llim_erabsp_sgnb_addition_ho_prep_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_erab_setup_fail_action_send_sgnb_reconfig_complete(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

static void uecc_fsm_rm_erab_setup_fail_action_send_sgnb_reconfig_complete_guard_timer_exp(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

static void uecc_fsm_rm_erab_setup_fail_action_send_sgnb_release(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

static void uecc_fsm_rm_erab_setup_fail_rlf_reestab_action_send_sgnb_release(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

static void uecc_fsm_rm_erab_setup_scg_failure_information_nr_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_x2ap_reset_msg_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_erab_setup_fail_action_s1_reset_send_sgnb_release(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

/*NR_DC Code Changes Stop*/
#endif

/**************************************
 * States definition
 **************************************/
static uecc_fsm_handler_record_t
                    uecc_fsm_rm_rrm_erab_setup_resource_admission_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
            uecc_fsm_rm_rrm_erab_setup_resource_admission_ongoing_init_handler         },
    { { UECC_EV_RRM_ERB_SETUP_RESP              },
            uecc_fsm_rm_rrm_erab_setup_resp_handler                          },
    { { UECC_EV_TIMER_EXPIRY                    },
            uecc_fsm_rm_rrm_erab_setup_resource_admission_ongoing_timer_expiry_handler },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
            uecc_fsm_rm_rrm_erab_setup_ongoing_rrc_conn_reestab_req_handler},
    { { UECC_EV_RRM_HO_REQUIRED            },
            uecc_fsm_rm_rrm_erab_setup_ongoing_ho_required_handler},
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND         },
            uecc_fsm_rm_rrm_erab_setup_ongoing_rlf_handler},
    { { UECC_EV_UE_RELEASE_REQ                  },
            uecc_fsm_rm_erab_setup_ongoing_ue_rel_req_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND               },
            uecc_fsm_rm_rrm_erab_setup_ongoing_s1ap_reset_msg_handler},   
    { { UECC_EV_HO_PREP_TIMER_EXPIRY          },
            uecc_fsm_rm_llim_erab_setup_ho_prep_timer_expiry_handler},
    { { UECC_LAST_ID                            },  PNULL                   }
};

static uecc_fsm_handler_record_t
                        uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
            uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing_init_handler            },
    { { UECC_EV_LLIM_RECONFIGURE_UE_RESP        },
            uecc_fsm_rm_llim_erab_setup_reconfigure_ue_resp_handler                    },
    { { UECC_EV_TIMER_EXPIRY                    },
            uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing_timer_expiry_handler    },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
            uecc_fsm_rm_llim_erab_setup_ongoing_rrc_conn_reestab_req_handler},
    { { UECC_EV_RRM_HO_REQUIRED            },
            uecc_fsm_rm_llim_erab_setup_ongoing_ho_required_handler},
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND         },
            uecc_fsm_rm_llim_erab_setup_ongoing_rlf_handler},
    { { UECC_EV_UE_RELEASE_REQ                  },
            uecc_fsm_rm_erab_setup_ongoing_ue_rel_req_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND               },
            uecc_fsm_rm_llim_erab_setup_ongoing_s1ap_reset_msg_handler},   
    { { UECC_EV_HO_PREP_TIMER_EXPIRY          },
            uecc_fsm_rm_llim_erab_setup_ho_prep_timer_expiry_handler},
    { { UECC_LAST_ID                            },  PNULL                   }
};

/*ARP Changes begin */

static uecc_fsm_handler_record_t
                        uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
            uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_init_handler            },
    { { UECC_EV_LLIM_RECONFIGURE_UE_RESP        },
            uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_resp_handler                    },
    { { UECC_EV_TIMER_EXPIRY                    },
            uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_timer_expiry_handler    },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
            uecc_fsm_rm_llim_erapsp_del_reconfigure_ue_ongoing_reestablishment_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND     },
            uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_rlf_handler             },
    { { UECC_EV_RRM_HO_REQUIRED                 },
            uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_ho_required_handler     },
    { { UECC_EV_UE_RELEASE_REQ                  },
            uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_ue_rel_req_handler      },
    { { UECC_LAST_ID                            },  PNULL                   }
};

/*ARP Changes end*/

static uecc_fsm_handler_record_t
                        uecc_fsm_rm_llim_erab_setup_rollback_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
            uecc_fsm_rm_llim_erab_setup_rollback_ongoing_init_handler            },
    { { UECC_EV_LLIM_ADD_ROLLBACK_RESP        },
            uecc_fsm_rm_llim_erab_setup_rollback_resp_handler                    },
    { { UECC_EV_TIMER_EXPIRY                    },
            uecc_fsm_rm_llim_erab_setup_rollback_ongoing_timer_expiry_handler    },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
            uecc_fsm_rm_llim_erab_setup_rollback_ongoing_rrc_conn_reestab_req_handler},
    { { UECC_EV_RRM_HO_REQUIRED            },
            uecc_fsm_rm_llim_erab_setup_rollback_ongoing_ho_required_handler},
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND         },
            uecc_fsm_rm_llim_erab_setup_rollback_ongoing_rlf_handler},
    { { UECC_EV_UE_RELEASE_REQ                  },
            uecc_fsm_rm_erab_setup_ongoing_ue_rel_req_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND               },
            uecc_fsm_rm_llim_erab_setup_rollback_ongoing_s1ap_reset_msg_handler},   
    { { UECC_EV_HO_PREP_TIMER_EXPIRY          },
            uecc_fsm_rm_llim_erab_setup_ho_prep_timer_expiry_handler},
    { { UECC_LAST_ID                            },  PNULL                   }
};



static uecc_fsm_handler_record_t uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                              },
            uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing_init_handler             },
    { { UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE },
            uecc_fsm_rm_erab_setup_rrc_connection_reconfiguration_complete_handler },
    { { UECC_EV_TIMER_EXPIRY                            },
            uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing_timer_expiry_handler     },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
            uecc_fsm_rm_ue_erab_setup_ongoing_rrc_conn_reestab_req_handler},
    { { UECC_EV_RRM_HO_REQUIRED            },
            uecc_fsm_rm_ue_erab_setup_ongoing_ho_required_handler},
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND         },
            uecc_fsm_rm_ue_erab_setup_ongoing_rlf_handler},
    { { UECC_EV_UE_RELEASE_REQ                  },
            uecc_fsm_rm_erab_setup_ongoing_ue_rel_req_handler},
    { { UECC_EV_S1AP_RESET_MSG_IND               },
            uecc_fsm_rm_ue_erab_setup_ongoing_s1ap_reset_msg_handler},   
    { { UECC_EV_HO_PREP_TIMER_EXPIRY          },
            uecc_fsm_rm_llim_erab_setup_ho_prep_timer_expiry_handler},
#ifdef ENDC_ENABLED
/*NR_DC Code Change START*/
    { { UECC_EV_SCG_FAILURE_INFORMATION_NR              },
            uecc_fsm_rm_erab_setup_scg_failure_information_nr_handler           },
/*NR_DC Code Change Stop*/
#endif

    { { UECC_LAST_ID                                    },  PNULL       }
};

/* ARP changes begin */
static uecc_fsm_handler_record_t uecc_fsm_rm_ue_erabsp_del_reconfigure_ongoing_mht[]=
{
    { { UECC_EV_STATE_INIT                              },
            uecc_fsm_rm_ue_erabsp_erab_del_reconfigure_ongoing_init_handler             },
    { { UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE },
            uecc_fsm_rm_erabsp_erab_del_rrc_connection_reconfiguration_complete_handler },
    { { UECC_EV_TIMER_EXPIRY                            },
            uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing_timer_expiry_handler     },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
            uecc_fsm_rm_llim_erapsp_del_reconfigure_ue_ongoing_reestablishment_handler },
    { { UECC_EV_RRM_HO_REQUIRED                 },
            uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_ho_required_handler     },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND     },
            uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_rlf_handler             },
    { { UECC_EV_UE_RELEASE_REQ                  },
            uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_ue_rel_req_handler      },
    { { UECC_LAST_ID                                    },  PNULL       }
};

/* ARP changes end */

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
static uecc_fsm_handler_record_t uecc_fsm_rm_erabsp_sgnb_addition_ongoing_mht[] = 
{
    { { UECC_EV_STATE_INIT                              },
            uecc_fsm_rm_erabsp_sgnb_addition_ongoing_init_handler                   },
    { { UECC_EV_X2AP_SGNB_ADDITION_REQ_ACK              },
            uecc_fsm_rm_erabsp_sgnb_addition_ongoing_ack_rcvd                       },
    { { UECC_EV_TIMER_EXPIRY                            },
            uecc_fsm_rm_erabsp_sgnb_addition_ongoing_timer_expiry                   },
    { { UECC_EV_X2AP_SGNB_ADDITION_REQ_REJECT           },
            uecc_fsm_rm_erabsp_sgnb_addition_ongoing_sgnb_add_req_reject            },
    { { UECC_EV_X2AP_ERROR_INDICATION                   },
            uecc_fsm_rm_erabsp_sgnb_addition_ongoing_x2ap_error_ind                 },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ                    },
            uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_rrc_conn_reestab_req_handler},
    { { UECC_EV_RRM_HO_REQUIRED                         },
            uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_ho_required_handler         },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND             },
            uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_rlf_handler                 },
    { { UECC_EV_UE_RELEASE_REQ                          },
            uecc_fsm_rm_erabsp_sgnb_addition_ongoing_ue_rel_req_handler             },
    { { UECC_EV_S1AP_RESET_MSG_IND                      },
            uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_s1ap_reset_msg_handler      },   
    { { UECC_EV_HO_PREP_TIMER_EXPIRY                    },
            uecc_fsm_rm_llim_erabsp_sgnb_addition_ho_prep_timer_expiry_handler      },
    { { UECC_EV_X2AP_RESET_MSG_IND                      },
            uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_x2ap_reset_msg_handler      },   
    { { UECC_LAST_ID                                    },  PNULL                   }
};
/*NR_DC Code Change Stop*/
#endif

static uecc_fsm_activity_state_t uecc_fsm_rm_rrm_erab_setup_resource_admission_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_RRM_ERB_SETUP_RESOURCE_ADMISSION_ONGOING",
   /* .s_id           = */  (S8*)"RM_RRM",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_rrm_erab_setup_resource_admission_ongoing_mht
};
/* ARP CHANGES BEGIN */

static uecc_fsm_activity_state_t uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_LLIM_ERBSP_DEL_RECONFIGURE_UE_ONGOING",
   /* .s_id           = */  (S8*)"RM_LLIM",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */ uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_mht 
};

/* ARP CHANGES END */ 

static uecc_fsm_activity_state_t uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing =
{   /* .s_name         = */  (S8*)"UECC_LLIM_ERB_SETUP_RECONFIGURE_UE_ONGOING",
   /* .s_id           = */  (S8*)"RM_LLIM",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing_mht
};


static uecc_fsm_activity_state_t uecc_fsm_rm_llim_erab_setup_rollback_ue_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_LLIM_ERB_SETUP_ROLLBACK_ONGOING",
   /* .s_id           = */  (S8*)"RM_LLIM",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_llim_erab_setup_rollback_ongoing_mht
};


static uecc_fsm_activity_state_t uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_UE_ERB_SETUP_RECONFIGURE_ONGOING",
   /* .s_id           = */  (S8*)"RM_UE",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing_mht
};

/* ARP changes begin */
static uecc_fsm_activity_state_t uecc_fsm_rm_ue_erabsp_del_reconfigure_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_UE_ERB_SETUP_RECONFIGURE_ONGOING",
   /* .s_id           = */  (S8*)"RM_UE",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_ue_erabsp_del_reconfigure_ongoing_mht
};
/* ARP changes end */
 
 
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
static uecc_fsm_activity_state_t uecc_fsm_rm_erabsp_sgnb_addition_ongoing = 
{
   /* .s_name         = */  (S8*)"UECC_X2AP_SGNB_ADDITION_ONGOING",
   /* .s_id           = */  (S8*)"SGNB_ADD",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_erabsp_sgnb_addition_ongoing_mht
};
/*NR_DC Code Change Stop*/
#endif

/**************************************
 * Activity definition
 **************************************/
/* Activity */
uecc_fsm_activity_t uecc_fsm_erab_setup_rrc_connection_reconfiguration_activity =
{
   /* .s_name             =*/ (S8*)"RRC Connection Reconfiguration",
   /* .s_id               =*/ (S8*)"UE_RCRECONF_A",
   /* .p_first_state      =*/ &uecc_fsm_rm_rrm_erab_setup_resource_admission_ongoing
};



/**************************************
 * Failure actions definition
 **************************************/

/* RM failure actions lists */
static const 
    uecc_fsm_fail_handler_f uecc_fsm_rm_erab_setup_fail_wait_rrm_actions[] =
{
    uecc_fsm_rm_erab_setup_fail_stop_proc_timer,
    /* SPR 13448 Fix Start */
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    /* SPR 13448 Fix Stop */
    uecc_fsm_rm_erab_setup_fail_action_last,
    PNULL
};

static const 
    uecc_fsm_fail_handler_f uecc_fsm_rm_erab_setup_fail_wrong_info_from_rrm[] =
{
    uecc_fsm_rm_erab_setup_fail_stop_proc_timer,
    uecc_fsm_rm_erab_setup_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f 
    uecc_fsm_rm_erab_setup_fail_timer_expiry_while_wait_from_rrm[] =
{
    uecc_fsm_rm_erab_setup_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_fatal_error_action_last,
    PNULL
};


static const uecc_fsm_fail_handler_f 
    uecc_fsm_rm_erab_setup_fail_all_failure_from_llim[] =
{
    uecc_fsm_rm_erab_setup_fail_stop_proc_timer,
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f 
    uecc_fsm_rm_erab_setup_fail_fatal_error_from_llim[] =
{
    uecc_fsm_rm_erab_setup_fail_stop_proc_timer,
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_fatal_error_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f
    uecc_fsm_rm_erab_setup_fail_timer_expiry_while_wait_from_llim[] =
{
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_fatal_error_action_last,
    PNULL
};




static const uecc_fsm_fail_handler_f 
    uecc_fsm_rm_erab_setup_fail_rollback_failure_from_llim[] =
{
    uecc_fsm_rm_erab_setup_fail_stop_proc_timer,
    uecc_fsm_rm_erab_setup_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_action_send_ue_ctx_rel_req,
    uecc_fsm_rm_erab_setup_rollback_last_fail_action,
    PNULL
};

static const uecc_fsm_fail_handler_f 
    uecc_fsm_rm_erab_setup_fail_other_rrm_actions[] =
{
    uecc_fsm_rm_erab_setup_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_action_last,
    PNULL
};
static const uecc_fsm_fail_handler_f 
    uecc_fsm_rm_erab_setup_fail_timer_expiry_wait_for_ue_reconfig[] =
{
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    uecc_fsm_rm_erab_setup_fail_action_send_sgnb_reconfig_complete_guard_timer_exp,
/*NR_DC Code Change Stop*/
#endif
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_action_last,
    PNULL
};

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
static const uecc_fsm_fail_handler_f
    uecc_fsm_rm_erab_setup_fail_s1_reset_wait_from_sgnb[] =
{
    uecc_fsm_rm_erab_setup_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_erab_setup_fail_action_s1_reset_send_sgnb_release,
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f
    uecc_fsm_rm_erab_setup_fail_x2_reset_wait_from_sgnb[] =
{
    uecc_fsm_rm_erab_setup_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f
    uecc_fsm_rm_erab_setup_fail_timer_expiry_while_wait_from_sgnb[] =
{
    uecc_fsm_rm_erab_setup_fail_action_send_sgnb_release,
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f
    uecc_fsm_rm_erab_setup_fail_rlf_reestablishment_wait_from_sgnb[] =
{
    uecc_fsm_rm_erab_setup_fail_rlf_reestab_action_send_sgnb_release,
    uecc_fsm_rm_erab_setup_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f 
    uecc_fsm_rm_erab_setup_fail_wait_for_ue_reconfig[] =
{
    uecc_fsm_rm_erab_setup_fail_stop_proc_timer,
    uecc_fsm_rm_erab_setup_fail_action_send_sgnb_reconfig_complete,
    uecc_fsm_rm_erab_setup_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_action_last,
    PNULL
};
/*NR_DC Code Change Stop*/
/*Error_Handling_changes*/
static const uecc_fsm_fail_handler_f
    uecc_fsm_rm_erab_setup_error_ind_fail_1_actions[] =
{
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp,
    uecc_fsm_rm_erab_setup_fail_action_last,
    PNULL
};
/*Error_Handling_changes*/

#endif


/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/
/****************************************************************************
* Function Name  : fill_erab_release_list_arp_change
* Returns        : RRC_SUCCESS/RRC_FAILURE
* Description    : This action routine is called when FSM receives the
*                  UECC_EV_STATE_INIT message.
****************************************************************************/
static rrc_return_t fill_erab_release_list_arp_change(
        uecc_ue_context_t  *p_ue_context,
        rrc_rrm_erb_setup_resp_t  *p_rrc_rrm_erb_setup_resp 
        )
{
    /* ARP changes  begin */
    U8          e_rab_index  = RRC_NULL;
    U8          erab_rel_list_count = RRC_NULL;
    U8          erab_rel_fail_list_count = RRC_NULL;
    erab_list_t *p_erab_list = PNULL;
    erab_list_t *p_erab_failed_list = PNULL;
    rrc_return_t result = RRC_SUCCESS;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rrc_rrm_erb_setup_resp);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /* changes for ARP start */

    if (RRM_ERB_SETUP_RESP_API_ERAB_TO_BE_RELEASED_ITEM_LIST_PRESENT &
            p_rrc_rrm_erb_setup_resp->bitmask)
    {
        do {
            /* points to erab_to_be_released_list in erab_release_command_data */
            p_erab_list = &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                erab_release_command_data.erab_release_list;
            p_erab_list->counter = p_rrc_rrm_erb_setup_resp->erab_to_be_released_item_list.
                count;

            /* points to erab_failed_to_release_list */
            p_erab_failed_list = &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                erab_release_command_data.erab_failed_to_release_list;


            for (e_rab_index = 0;
                    e_rab_index < p_rrc_rrm_erb_setup_resp->erab_to_be_released_item_list.count;
                    e_rab_index++)
            {
                if (PNULL != p_ue_context->
                        p_e_rab_list[p_rrc_rrm_erb_setup_resp->erab_to_be_released_item_list.
                        erab_to_be_release_item[e_rab_index].erab_id])
                {

                    /* Convert received rrm cause into s1ap cause 
                     * in received list from RRM */
                    if (RRC_SUCCESS != uecc_s1ap_build_s1ap_cause(
                                &p_erab_list->erab_item[erab_rel_list_count].cause,
                                &p_rrc_rrm_erb_setup_resp->erab_to_be_released_item_list.
                                erab_to_be_release_item[e_rab_index].cause,
                                p_ue_context))  
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,
                                RRC_WARNING, 
                                "[RRC_RRM_ERB_SETUP_RESP]"
                                "invalid cause value [%u][%u]",
                                p_rrc_rrm_erb_setup_resp->erab_to_be_released_item_list.
                                erab_to_be_release_item[e_rab_index].cause.type,
                                p_rrc_rrm_erb_setup_resp->erab_to_be_released_item_list.
                                erab_to_be_release_item[e_rab_index].cause.value);

                        p_erab_failed_list->erab_item[erab_rel_fail_list_count].
                            e_RAB_ID = p_rrc_rrm_erb_setup_resp->erab_to_be_released_item_list.
                            erab_to_be_release_item[e_rab_index].erab_id;

                        p_erab_failed_list->erab_item[erab_rel_fail_list_count].
                            error_code |= RRC_RRM_MODULE_ID << 16;

                        p_erab_failed_list->erab_item[erab_rel_fail_list_count++].
                            error_code |= RRC_UNKNOWN_ERAB_ID; /* Yet to add reason */

                        p_erab_failed_list->counter++;

                        continue;
                    }

                    p_erab_list->erab_item[erab_rel_list_count++].e_RAB_ID = 
                        p_rrc_rrm_erb_setup_resp->erab_to_be_released_item_list.
                        erab_to_be_release_item[e_rab_index].erab_id;  
                }
                else
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,
                            RRC_WARNING,"Unknown E-RAB ID = [%u]",
                            p_rrc_rrm_erb_setup_resp->erab_to_be_released_item_list.
                            erab_to_be_release_item[e_rab_index].erab_id);
                    p_erab_failed_list->erab_item[erab_rel_fail_list_count].
                        e_RAB_ID = p_rrc_rrm_erb_setup_resp->erab_to_be_released_item_list.
                        erab_to_be_release_item[e_rab_index].erab_id;

                    p_erab_failed_list->erab_item[erab_rel_fail_list_count].
                        error_code |= RRC_RRM_MODULE_ID << 16;

                    p_erab_failed_list->erab_item[erab_rel_fail_list_count++].
                        error_code |= RRC_UNKNOWN_ERAB_ID; /* Yet to add reason */

                    p_erab_failed_list->counter++;

                }

            }

            p_erab_list->counter = erab_rel_list_count;

            if (RRC_NULL == p_erab_list->counter)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,
                        RRC_WARNING,"E-RAB Release List is empty"
                        " after validation. Indicates Failure");
            }
            else
            {
                p_ue_context->p_curr_proc_data->u.erab_setup_request_data.is_erab_del_ongoing = 
                    RRC_TRUE;
            }


            result = RRC_SUCCESS;
        } while(0);

    }
    /* ARP changes End */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
* Function Name  : fill_erab_release_list_tdd_change
* Returns        : RRC_SUCCESS/RRC_FAILURE
* Description    : This action routine is called when FSM receives the
*                  UECC_EV_STATE_INIT message.
****************************************************************************/

static rrc_return_t fill_erab_release_list_tdd_change(
        uecc_ue_context_t  *p_ue_context,
        rrc_rrm_erb_setup_resp_t  *p_rrc_rrm_erb_setup_resp 
        )
{
    U8              sub_frame_assignment = RRC_NULL;
    rrc_return_t    result = RRC_SUCCESS;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rrc_rrm_erb_setup_resp);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (0 == (RRM_ERB_SETUP_RESP_API_ADM_RADIO_RES_PRESENT
                & p_rrc_rrm_erb_setup_resp->bitmask))
    {
        /* It should be present in case of success
         * See LTE RRC API Reference Manual RRC_RRM_ERB_SETUP_RESP */
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_ERB_SETUP_RESP] radio_rsource_config absent");
    }
    else
    {
        if ((PNULL != p_ue_context->p_gb_context->p_p_csc_context) &&
                (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                 [p_ue_context->cell_index]) &&
                (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                 [p_ue_context->cell_index]->p_csc_init_setup_ind))
        {
            /*** TDD changes start ***/
            if(DUPLEXING_MODE_TDD == p_ue_context->p_gb_context->p_p_csc_context[
                    p_ue_context->cell_index]->p_csc_init_setup_ind->
                    duplexing_mode)
            {
                sub_frame_assignment = p_ue_context->p_gb_context->p_p_csc_context[
                    p_ue_context->cell_index]->p_csc_init_setup_ind->
                    sib_type_1_Info.tdd_config.sub_frame_assignment;

                if ((p_rrc_rrm_erb_setup_resp->radio_rsource_config.bitmask &
                            RRM_ERAB_SETUP_RADIO_RES_CONFIG_MAC_CONFIG_PRESENT) &&
                        (p_rrc_rrm_erb_setup_resp->radio_rsource_config.mac_config.bitmask &
                         RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT ) &&
                        (RRC_TRUE == p_rrc_rrm_erb_setup_resp->radio_rsource_config.
                         mac_config.mac_main_config.ul_sch_config.tti_bundling) )
                {
                    /* TTI bundling can be enabled for FDD and for TDD only for
                     * configurations 0, 1 and 6 */
                    if (sa_0 != sub_frame_assignment &&
                            sa_1 != sub_frame_assignment &&
                            sa_6 != sub_frame_assignment)
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,
                                RRC_WARNING,"ERAB setup: TTI bundling can be"\
                                " enabled for FDD and for TDD only for"
                                " configurations 0, 1 and 6");
                        result = RRC_FAILURE;
                    }
                    if (RRM_ERAB_SETUP_RADIO_RES_CONFIG_SPS_CONFIG_PRESENT &
                            p_rrc_rrm_erb_setup_resp->radio_rsource_config.bitmask )
                    {
                        /*For TDD, E-UTRAN does not simultaneously enable TTI
                         *bundling and semi-persistent scheduling */
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,
                                RRC_WARNING,"ERAB setup: For TDD, E-UTRAN does not"
                                " simultaneously enable TTI bundling and"
                                " semi-persistent scheduling");
                        result = RRC_FAILURE;
                    }

                }
            }
            if (RRC_SUCCESS == result )
            {
                /* Process Radio Resource Dedicated Info from RRM */
                result = uecc_fsm_rm_process_rrm_erab_radio_res_dedicated(
                        p_ue_context,
                        p_rrc_rrm_erb_setup_resp);
                RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
                        "[RRC_RRM_ERB_SETUP_RESP] :[%u]", result);
            }
            /*** TDD changes stop ***/
        }


        /* cr_220 :
           if rlf_timers_and_constants_r9 were not received Previously by UE,
           then, Store RLF Timers & Constants from rrc_rrm_erb_setup_resp data
           from RRM into UE Context */

        if (RRM_ERAB_SETUP_RADIO_RES_CONFIG_RLF_TIMERS_AND_CONSTANTS_PRESENT &
                p_rrc_rrm_erb_setup_resp->radio_rsource_config.bitmask )
        {

            p_ue_context->m.rlf_timers_and_constants_r9_present = 1;

            if(RLF_TIMERS_AND_CONSTANTS_SETUP_PRESENT & p_rrc_rrm_erb_setup_resp->radio_rsource_config.rlf_timers_and_constants_r9.bitmask)
            {


                if(RRC_NULL == p_ue_context->p_rlf_timers_and_constants_r9) 
                {
                    p_ue_context->p_rlf_timers_and_constants_r9 =
                        (rlf_timers_and_constants_r9_t *)rrc_mem_get(sizeof(rlf_timers_and_constants_r9_t)); 
                }
                if (PNULL != p_ue_context->p_rlf_timers_and_constants_r9)
                {

                    p_ue_context->p_rlf_timers_and_constants_r9->bitmask = RLF_TIMERS_AND_CONSTANTS_SETUP_PRESENT;

                    p_ue_context->p_rlf_timers_and_constants_r9->setup.t301_r9 =
                        p_rrc_rrm_erb_setup_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.t301_r9;

                    p_ue_context->p_rlf_timers_and_constants_r9->setup.t310_r9 =
                        p_rrc_rrm_erb_setup_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.t310_r9;

                    p_ue_context->p_rlf_timers_and_constants_r9->setup.n310_r9 =
                        p_rrc_rrm_erb_setup_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.n310_r9;

                    p_ue_context->p_rlf_timers_and_constants_r9->setup.t311_r9 =
                        p_rrc_rrm_erb_setup_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.t311_r9;

                    p_ue_context->p_rlf_timers_and_constants_r9->setup.n311_r9 =
                        p_rrc_rrm_erb_setup_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.n311_r9;
                }
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_rrm_erab_setup_resource_admission_ongoing_init_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rm_rrm_erab_setup_resource_admission_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send the RRC_RRM_ERB_SETUP_REQ msg to RRM */
    if(RRC_SUCCESS != uecc_rrm_build_and_send_erb_setup_req(p_ue_context))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
            "FAILURE: UNABLE TO SEND TO RRC_RRM_ERB_SETUP_REQ");
        
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_rrm_erab_setup_resp_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context 
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRM_ERB_SETUP_RESP message from RRM.
 ****************************************************************************/
void uecc_fsm_rm_rrm_erab_setup_resp_handler(
         uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
         void*               p_api                  /* Input API buffer */
)
{
	rrc_rrm_erb_setup_resp_t*  p_rrc_rrm_erb_setup_resp = PNULL;
	/*** TDD changes start***/
	rrc_return_t               result = RRC_SUCCESS;
	/*** TDD changes start***/

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(p_ue_context->p_gb_context,
				p_ue_context->cell_index))
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
				p_ue_context->p_gb_context->facility_name, 
				RRC_WARNING,"cell not active");
		/* Process failure according to selected failure actions list */
		uecc_fsm_start_failure_processing(
				p_ue_context,
				uecc_fsm_rm_erab_setup_fail_wrong_info_from_rrm);
		RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
		return;
	}

	p_rrc_rrm_erb_setup_resp = (rrc_rrm_erb_setup_resp_t*)p_api;

	/* Check whether RRC_RRM_ERB_SETUP_RESP received with success or failure */
	if (PARTIAL_SUCCESS >= p_rrc_rrm_erb_setup_resp->response)
	{
		/* changes for ARP start */
		/* Coverity ID 81455 Starts */
		(void)fill_erab_release_list_arp_change(p_ue_context, 
				p_rrc_rrm_erb_setup_resp);
		/* Coverity ID 81455 Ends */
		/*** TDD changes ***/
		result = fill_erab_release_list_tdd_change(p_ue_context, 
				p_rrc_rrm_erb_setup_resp);
	}

	do
	{
		if (result == RRC_FAILURE) /* Error While processing Resp from RRM */
		{
			/* Process failure according to selected failure actions list */
			uecc_fsm_start_failure_processing(
					p_ue_context,
					uecc_fsm_rm_erab_setup_fail_wrong_info_from_rrm);
		}
		else if (SUCCESS == p_rrc_rrm_erb_setup_resp->response || 
				PARTIAL_SUCCESS == p_rrc_rrm_erb_setup_resp->response)
		{

			/* Read Optional Parameter if present */   
			/* Allocate Memory to Store Optinal Parameters if present */
			if ((p_rrc_rrm_erb_setup_resp->radio_rsource_config.bitmask &
						RRM_ERAB_SETUP_RADIO_RES_CONFIG_MAC_CONFIG_PRESENT) ||
					(p_rrc_rrm_erb_setup_resp->radio_rsource_config.bitmask &
					 RRM_ERAB_SETUP_RADIO_RES_CONFIG_SPS_CONFIG_PRESENT) ||             
					(p_rrc_rrm_erb_setup_resp->radio_rsource_config.bitmask &
					 RRM_ERAB_SETUP_RADIO_RES_CONFIG_PHY_CONFIG_DED_PRESENT) ||
					(p_rrc_rrm_erb_setup_resp->radio_rsource_config.bitmask &
					 RRM_ERAB_SETUP_RADIO_RES_CONFIG_MEAS_CONFIG_PRESENT))
			{   
				/* Read Optinal Parameters */
				if(RRC_FAILURE == uecc_fsm_rm_erab_setup_read_op_rrm_params(
							p_ue_context,           
							p_rrc_rrm_erb_setup_resp,
							&p_ue_context->p_curr_proc_data->u.
							erab_setup_request_data.erab_op_params))
				{

					/* Process failure according to selected failure actions list */
					uecc_fsm_start_failure_processing(
							p_ue_context,
							uecc_fsm_rm_erab_setup_fail_wrong_info_from_rrm);
					break;
				}

			}

            /*SPR_19066_START*/
            if (RRM_ERB_SETUP_RESP_API_UE_INACTIVE_TIME_PRESENT
                    & p_rrc_rrm_erb_setup_resp->bitmask)
            {
                p_ue_context->m.ue_inactive_time_updated = RRC_TRUE;
                p_ue_context->ue_inactive_time = 
                    p_rrc_rrm_erb_setup_resp->ue_inactive_time_config;
            }
            /*SPR_19066_END*/

#ifdef ENDC_ENABLED
        /*NR_DC Code Change Start*/
        /*check if the rab will be admitted on the MeNB or the SgNB UE
         * Addition is required for this*/
            if( RRC_SUCCESS ==
                    uecc_fsm_chk_if_sgnb_addition_reqd(
                        p_ue_context,
                        &p_rrc_rrm_erb_setup_resp->radio_rsource_config.
                        drb_to_add_list))
            {
                /*Store UE Aggregated Max Bit Rate for DL and UL*/
                p_ue_context->ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_ul =
                    p_rrc_rrm_erb_setup_resp->sgnb_add_param.ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_ul;
                p_ue_context->ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_dl =
                    p_rrc_rrm_erb_setup_resp->sgnb_add_param.ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_dl;

                if ( RRC_RRM_EXT_UE_AGG_MAX_BITRATE_PRESENT &
                    p_rrc_rrm_erb_setup_resp->sgnb_add_param.ue_agg_max_bit_rate_sgnb.bitmask )
                {
                    p_ue_context->ue_agg_max_bit_rate_sgnb.bitmask |=
                        RRC_RRM_EXT_UE_AGG_MAX_BITRATE_PRESENT;

                    /*Store UE Aggregated Max Bit Rate Ext for DL and UL*/
                    p_ue_context->ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_ext.
                        ext_ue_agg_max_bit_rate_ul =
                            p_rrc_rrm_erb_setup_resp->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                                ue_agg_max_bit_rate_ext.ext_ue_agg_max_bit_rate_ul;
                    p_ue_context->ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_ext.
                        ext_ue_agg_max_bit_rate_dl =
                            p_rrc_rrm_erb_setup_resp->sgnb_add_param.ue_agg_max_bit_rate_sgnb.
                                ue_agg_max_bit_rate_ext.ext_ue_agg_max_bit_rate_dl;
                }


                if((RRC_TRUE == p_ue_context->is_sgnb_add_reqd)&&
                        (RRM_ERB_SETUP_RESP_API_SGNB_ADD_PARAM_PRESENT &
                         p_rrc_rrm_erb_setup_resp->bitmask))
                {
                    l3_memcpy_wrapper(&(p_ue_context->p_curr_proc_data->
                                u.erab_setup_request_data.uecc_cg_config_info),
                            &(p_rrc_rrm_erb_setup_resp->sgnb_add_param.cg_config_info),
                            sizeof(cg_config_info_t));
                }
            }
           /*NR_DC Code Change Stop*/
#endif

			/* ARP changes begin */
			/* Change Child FSM state */
			if (RRC_TRUE == 
					p_ue_context->p_curr_proc_data->u.erab_setup_request_data.is_erab_del_ongoing)
			{
				UECC_FSM_SET_CHILD_STATE(
						p_ue_context,
						&uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing,
						PNULL);
			}
			else
			{

#ifdef ENDC_ENABLED
			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_WARNING,
					"[RRC_RRM_ERB_SETUP_RESP] is_sgnb_add_reqd[%d] rrc_uecc_count_scg_bearer[%d]",
					p_ue_context->is_sgnb_add_reqd, rrc_uecc_count_scg_bearer(p_ue_context));
                /*NR_DC Code Change Start*/
                /*check if SGNB Addition is required for the UE*/
                if (RRC_TRUE == p_ue_context->is_sgnb_add_reqd &&
                        (RRC_NULL <
                         rrc_uecc_count_scg_bearer(p_ue_context))) 
                {
                    UECC_FSM_SET_CHILD_STATE(
                            p_ue_context,
                            &uecc_fsm_rm_erabsp_sgnb_addition_ongoing,
                            PNULL);
                }
                else
                {
				    UECC_FSM_SET_CHILD_STATE(
					    	p_ue_context,
						    &uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing,
						    PNULL);
                }
                /*NR_DC Code Change Stop*/
#else
				UECC_FSM_SET_CHILD_STATE(
						p_ue_context,
						&uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing,
						PNULL);
#endif
			}
			/* ARP changes end */
		}
		else if(FAILURE == p_rrc_rrm_erb_setup_resp->response)
		{
			/* SPR 13448 Fix Start */
			if (RRM_ERB_SETUP_RESP_API_FAIL_CAUSE_PRESENT &
					p_rrc_rrm_erb_setup_resp->bitmask)
			{
				uecc_fsm_rm_fail_action_mark_all_erab_fail_with_rrm_cause(
						p_ue_context,
						&p_rrc_rrm_erb_setup_resp->fail_cause);
			}
			else
			{
				uecc_fsm_rm_fail_action_mark_all_erab_failed(
						p_ue_context,
						RRC_INVALID_RRM_MESSAGE_RECV);
			}

			p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
				erab_info_data.count = 0;
			/* SPR 13448 Fix Stop */

			/* Process failure according to selected failure actions list */
			uecc_fsm_start_failure_processing(
					p_ue_context,
					uecc_fsm_rm_erab_setup_fail_wait_rrm_actions);
		}
		else /* Invaild response code received */
		{
			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
					"[RRC_RRM_ERB_SETUP_RESP] Invalid response code : %u",
					p_rrc_rrm_erb_setup_resp->response);
		}
	}while(0);
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_rrm_erab_setup_resource_admission_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rm_rrm_erab_setup_resource_admission_ongoing_timer_expiry_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_TIMER_EXPIRY);

    /* Start Failure Processing With Failure Actions */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rm_erab_setup_fail_timer_expiry_while_wait_from_rrm);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* This scenario will not occur */
/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_rrm_erab_setup_ongoing_rrc_conn_reestab_req_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_RRC_RE_ESTABLISH_REQ
 ****************************************************************************/
void uecc_fsm_rm_rrm_erab_setup_ongoing_rrc_conn_reestab_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
        rrc_mem_get(len)))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL, 
            "Memory Allocation Failed to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    
    /* storing re_establishment contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
            p_api,len);

    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RE_ESTABLISH_MSG;


    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_REESTABLISHMENT_TRIGGERED);

    /* Send Failure Confirmation to RRM */
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf(
            p_ue_context,
            PNULL);

    /* Start Failure Processing With Failure Actions */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rm_erab_setup_fail_wait_rrm_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_rrm_erab_setup_ongoing_ho_required_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_RRM_HO_REQUIRED
 ****************************************************************************/
void uecc_fsm_rm_rrm_erab_setup_ongoing_ho_required_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    U16    len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    len = (U16)sizeof(rrc_rrm_ho_required_t);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
        rrc_mem_get(len)))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL, 
            "memory allocation failed to store received RRC_RRM_HO_REQUIRED");
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
   
    /* storing re_establishment contents into msg_bfr_rcvd */
    /* SPR 24021/SES-452 Coverity 209457 Fix Start */
    if(PNULL != p_api)
    {
        l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
                p_api, len);

        /* Store the HO REQUIRED msg in UE Context */
        p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_HO_REQUIRED_MSG;

        /* START HO PREP TIMER */
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
                RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                        "ERROR in Starting X2_HO PREP TIMER");
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
                RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                        "ERROR in Starting INTER_CELL_HO PREP TIMER");
            }
        }
        /*BUG 604 changes start*/
        else if(HANDOVER_TYPE_INTRA_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = INTRA_CELL_HO;
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context, 
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                        "ERROR in Starting INTRA_CELL_HO PREP TIMER");
            }
        }
        /*BUG 604 changes stop*/
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
                RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                        "ERROR in Starting HO PREP TIMER");
            }
        }

        /* Mark ALL E-RAB's Failed */
        uecc_fsm_rm_erab_setup_action_append_erab_to_fail_list(
                p_ue_context,
                RRC_SUCCESS);

        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_rm_erab_setup_fail_other_rrm_actions);

        /* SPR 24021/SES-452 Fix Start */
        rrc_mem_free(p_api);
        p_api = PNULL;
        /* SPR 24021/SES-452 Fix End */
    }
    /* SPR 24021/SES-452 Coverity 209457 Fix End */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_rrm_erab_setup_ongoing_rlf_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_RADIO_LINK_FAILURE_IND
 ****************************************************************************/
void uecc_fsm_rm_rrm_erab_setup_ongoing_rlf_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    U16       len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_RADIO_LINK_FAILURE_TRIGGERED);

    len = (U16)sizeof(rrc_uecc_llim_radio_link_failure_ind_t);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
        rrc_mem_get(len)))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
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

    /* SPR 13448 Fix Start */
    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_RADIO_LINK_FAILURE_TRIGGERED);
    /* SPR 13448 Fix Stop */

    /* Send Failure Confirmation to RRM */
    uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf(
            p_ue_context,
            PNULL);

    /* Start Failure Processing With Failure Actions */    
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rm_erab_setup_fail_wait_rrm_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  : uecc_fsm_rm_rrm_erab_setup_reconfigure_ue_ongoing_init_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send the UECC_LLIM_ADD_LC_REQ msg to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_add_lc_req(p_ue_context))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL, 
            "FAILURE: UNABLE TO SEND UECC_LLIM_ADD_LC_REQ");
        
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  : uecc_fsm_rm_llim_erab_setup_reconfigure_ue_resp_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_LLIM_RECONFIGURE_UE_RESP from LLIM
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_reconfigure_ue_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    rrc_return_t    result = RRC_FAILURE;

    rrc_uecc_llim_add_lc_resp_t*    p_rrc_uecc_llim_add_lc_resp = PNULL;
    uecc_fsm_fail_handler_f const*  p_fail_actions_list =
                                        uecc_fsm_rm_erab_setup_fail_all_failure_from_llim;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_uecc_llim_add_lc_resp = (rrc_uecc_llim_add_lc_resp_t*)p_api;

    do
    {
        /* Map Response to ERAB Setup List Response */
        if (SUCCESS == p_rrc_uecc_llim_add_lc_resp->response_code)
        {
            p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                erab_info_data.outerResponse = SUCCESS;
        }
        else if (PARTIAL_SUCCESS == p_rrc_uecc_llim_add_lc_resp->response_code)
        {
            p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
               erab_info_data.outerResponse = PARTIAL_SUCCESS;        
        }
        else if (L3_FATAL == p_rrc_uecc_llim_add_lc_resp->response_code ||
                 FAILURE == p_rrc_uecc_llim_add_lc_resp->response_code)
        {
            p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                erab_info_data.outerResponse = FAILURE;
        }
        else /* INVALID RESPONSE */
        {
            /* Put all E_RAB's in failed list from ERAB Setup List */
            uecc_fsm_rm_fail_action_mark_all_erab_failed(
                p_ue_context,
                RRC_INVALID_RESPONSE_RECEIVED);

            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RECONFIGURE_UE_RESP] INVALID RESPONSE [%u]",
                 p_rrc_uecc_llim_add_lc_resp->response_code);
            /* failure at LLIM - INVALID RESPONSE */
            p_fail_actions_list =
                uecc_fsm_rm_erab_setup_fail_fatal_error_from_llim;
            result = RRC_FAILURE;
            break;

        } 
                /* SPS Start */
        if (p_ue_context->m.is_sps_recv_frm_rrm)
        {
            /* Check if Setup SPS config was received */
            if (!p_ue_context->m.is_sps_setup_or_rel_recv)
            {
                if (p_rrc_uecc_llim_add_lc_resp->bitmask &
                        UECC_LLIM_ADD_LC_RESP_SPS_CRNTI_PRESENT)
                {
                    /* Copy SPS CRNTI into UE Context */
                    rrc_pack_U16(p_ue_context->p_sps_config->semi_presist_sched_c_rnti,
                            &p_rrc_uecc_llim_add_lc_resp->sps_crnti);
                    p_ue_context->p_sps_config->bitmask |= RRC_SPS_CONFIG_C_RNTI_PRESENT;
                }
                else
                {
                    p_ue_context->m.sps_config_updated = RRC_NULL; /* Reset to 0 */
                    if (p_ue_context->p_sps_config != PNULL)
                    {
                        rrc_mem_free(p_ue_context->p_sps_config);
                        p_ue_context->p_sps_config = PNULL;
                        p_ue_context->m.sps_config_present = RRC_NULL;
                    }
                }
            }
        }
        /* SPS Stop */

        
        result = uecc_fsm_rm_process_llim_add_lc_res(
            p_ue_context,
            p_rrc_uecc_llim_add_lc_resp);
        /* Coverity_fix_81322_start*/
        if(RRC_FAILURE == result)
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,
                "uecc_fsm_rm_process_llim_add_lc_res returned failure");
        }
         /* Coverity_fix_81322_end*/

        if (SUCCESS == p_rrc_uecc_llim_add_lc_resp->response_code || 
            PARTIAL_SUCCESS ==  p_rrc_uecc_llim_add_lc_resp->
            response_code)
        {

            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,
                "LLIM has allocated radio resources successfully for new ERAB");

            /* Switch state */
            UECC_FSM_SET_CHILD_STATE(
                p_ue_context,
                &uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing,
                PNULL);

            result = RRC_SUCCESS;
        }
        else if (FAILURE == p_rrc_uecc_llim_add_lc_resp->response_code)
        { 
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RECONFIGURE_UE_RESP] failed [%u]",
                 p_rrc_uecc_llim_add_lc_resp->response_code);
            /* ARP changes begin */
            if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                is_erab_del_ongoing == RRC_TRUE)
            {

                /* Switch state */
                UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_rm_ue_erabsp_del_reconfigure_ongoing,
                    PNULL);
                result = RRC_SUCCESS;
            }
            else
            {
            /* failure at LLIM - LLIM has deleted LCs on LL by itself*/
            p_fail_actions_list =
                uecc_fsm_rm_erab_setup_fail_all_failure_from_llim;
            result = RRC_FAILURE;       

            /* eMTC changes start */
            /* SPR 22915 fix start */
                /* Put all E_RAB's in failed list from ERAB Setup List */
                uecc_fsm_rm_fail_action_mark_all_erab_failed(
                        p_ue_context,
                        RRC_LLIM_RECONFIG_FAILURE);
            /* SPR 22915 fix stop */
            /* eMTC changes stop */
            }
        }
        else /* Response is L3_FATAL ERROR */ 
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RECONFIGURE_UE_RESP] L3_FATAL ERROR [%u]",
                 p_rrc_uecc_llim_add_lc_resp->response_code);
            /* failure at LLIM - L3_FATAL ERROR */
            p_fail_actions_list =
                uecc_fsm_rm_erab_setup_fail_fatal_error_from_llim;
            result = RRC_FAILURE;
        }
    } while (0);



    if (RRC_SUCCESS != result )
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
            p_ue_context,
            p_fail_actions_list);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_llim_erab_setup_resource_ue_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    /* ARP changes begin */
    U32 error_code = RRC_NULL;
    /* ARP changes end */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


    /* ARP changes begin */
    if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        error_code |= RRC_MODULE_ID << 16;
        error_code |= RRC_TIMER_EXPIRY;

        /* Update erab release list to erab failed to release list */
        uecc_rm_erabsp_update_erab_release_list_with_error_code(
                p_ue_context,
                &error_code);
    }
    /* ARP changes start */

    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_TIMER_EXPIRY);

    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rm_erab_setup_fail_timer_expiry_while_wait_from_llim);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_llim_erab_setup_ongoing_rrc_conn_reestab_req_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_RRC_RE_ESTABLISH_REQ
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_ongoing_rrc_conn_reestab_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{  
    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* ARP changes begin */
    if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        uecc_fsm_rm_erabsp_erab_release_fail_handler(
                p_ue_context,
                RRC_REESTABLISHMENT_TRIGGERED,
                uecc_fsm_rm_erab_setup_fail_fatal_error_from_llim);

        /* Send RRC-Connection Reject to UE */
        if (RRC_SUCCESS !=
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                    p_ue_context->p_gb_context, p_ue_context->crnti,
                    p_ue_context->reestablishmentCause,
                    p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                    "Fail to send the RRCConnectionReestablishmentReject to UE");
        }

        /* enabling flag */
        SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    /* ARP changes end */

    /* Buffered Received Re-establishment Msg */
    len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = rrc_mem_get(len)))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL, 
            "Memory Allocation Failed to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    
    /* storing re_establishment contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
        p_api,len);

    /* store received message type */
    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RE_ESTABLISH_MSG;

    /* Now change the state to ERAB SETUP ROLLBACK STATE */
    /* Switch state */
    UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_rm_llim_erab_setup_rollback_ue_ongoing,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_llim_erab_setup_ongoing_ho_required_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_RRM_HO_REQUIRED
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_ongoing_ho_required_handler(
 uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api               /* Input API buffer */
)
{
    U16       len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* ARP changes begin */
    if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
                "[UECC_EV_RRM_HO_REQUIRED]"
                "RRM Initiated E-RAB Release Procedure Ongoing"
                "Cancel the HO triggered by RRM");
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_interaction_with_other_procedure;
        if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_failure( p_ue_context))
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
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
        return;
    }

    /* ARP changes end */
    len = (U16)sizeof(rrc_rrm_ho_required_t);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
        rrc_mem_get(len)))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
            "memory allocation failed to store received RRC_RRM_HO_REQUIRED");
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

    /* storing RRC_HO_REQUIRED_MSG contents into msg_bfr_rcvd */
    /* SPR 24021/SES-452 Coverity 209455 Fix Start */
    if(PNULL != p_api)
    {
        l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
                p_api, len);

        /* Store the HO REQUIRED msg in UE Context */
        p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_HO_REQUIRED_MSG;

        /* START HO PREP TIMER */
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
                RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                        "ERROR in Starting X2_HO PREP TIMER");
            }
            else
            {
                /* Now change the state to ERAB SETUP ROLLBACK STATE */
                /* Switch state */
                UECC_FSM_SET_CHILD_STATE(
                        p_ue_context,
                        &uecc_fsm_rm_llim_erab_setup_rollback_ue_ongoing,
                        PNULL);
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
                RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                        "ERROR in Starting INTER_CELL_HO PREP TIMER");
            }
            else
            {
                /* Now change the state to ERAB SETUP ROLLBACK STATE */
                /* Switch state */
                UECC_FSM_SET_CHILD_STATE(
                        p_ue_context,
                        &uecc_fsm_rm_llim_erab_setup_rollback_ue_ongoing,
                        PNULL);
            }
        }
        /*BUG 604 changes start*/
        else if(HANDOVER_TYPE_INTRA_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = INTRA_CELL_HO;
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                        "ERROR in Starting INTRA_CELL_HO PREP TIMER");
            }
            else
            {
                /* Now change the state to ERAB SETUP ROLLBACK STATE */
                /* Switch state */
                UECC_FSM_SET_CHILD_STATE(
                        p_ue_context,
                        &uecc_fsm_rm_llim_erab_setup_rollback_ue_ongoing,
                        PNULL);
            }
        }
        /*BUG 604 changes stop*/
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
                RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                        "ERROR in Starting HO PREP TIMER");
            }
            else
            {
                /* Now change the state to ERAB SETUP ROLLBACK STATE */
                /* Switch state */
                UECC_FSM_SET_CHILD_STATE(
                        p_ue_context,
                        &uecc_fsm_rm_llim_erab_setup_rollback_ue_ongoing,
                        PNULL);
            }
        }

        /* SPR 24021/SES-452 Fix Start */
        rrc_mem_free(p_api);
        p_api = PNULL;
        /* SPR 24021/SES-452 Fix End */
    }
    /* SPR 24021/SES-452 Coverity 209455 Fix End */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_llim_erab_setup_ongoing_rlf_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_RADIO_LINK_FAILURE_IND
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_ongoing_rlf_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* ARP changes begin */
    if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        uecc_fsm_rm_erabsp_erab_release_fail_handler(
                p_ue_context,
                RRC_RADIO_LINK_FAILURE_TRIGGERED,
                uecc_fsm_rm_erab_setup_fail_fatal_error_from_llim);

        /* enabling flag */
        SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    /* ARP changes end */


    len = (U16)sizeof(rrc_uecc_llim_radio_link_failure_ind_t);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
        rrc_mem_get(len)))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
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

    /* Now change the state to ERAB SETUP ROLLBACK STATE */
    /* Switch state */
    UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_rm_llim_erab_setup_rollback_ue_ongoing,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_llim_erab_setup_rollback_ongoing_init_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_rollback_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send the UECC_LLIM_ADD_ROLLBACK_REQ msg to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_add_rollback_req(p_ue_context))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO, 
            "FAILURE: UNABLE TO SEND UECC_LLIM_ADD_ROLLBACK_REQ");    
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_llim_erab_setup_rollback_resp_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_LLIM_RECONFIGURE_UE_RESP from LLIM
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_rollback_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{

    rrc_uecc_llim_add_rollback_resp_t *p_add_rollback_resp =
        (rrc_uecc_llim_add_rollback_resp_t*)p_api;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    do
    {
    if(p_add_rollback_resp->response > SUCCESS) /* INVALID RESPONSE */
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_WARNING,
            "[UECC_EV_LLIM_RECONFIGURE_UE_RESP] INVALID RESPONSE [%u]",
                p_add_rollback_resp->response);
            break;
    }

    /* Append E_RAB's to fail list and set cause and error codes */
    uecc_fsm_rm_erab_setup_action_append_erab_to_fail_list(
        p_ue_context,
        p_add_rollback_resp->response);

        if(p_add_rollback_resp->response == SUCCESS)
    {

        /* FAILURE ACTIONS */
        uecc_fsm_start_failure_processing(
            p_ue_context,
                uecc_fsm_rm_erab_setup_fail_all_failure_from_llim);
    }
    else
    {

        /* FAILURE ACTIONS */
        uecc_fsm_start_failure_processing(
            p_ue_context,
                uecc_fsm_rm_erab_setup_fail_rollback_failure_from_llim);
    }
    }while(0);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_llim_erab_setup_rollback_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_rollback_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing_timer_expiry_handler(
        p_ue_context,
        p_api);
}

/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_llim_erab_setup_rollback_ongoing_rrc_conn_reestab_req_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_RRC_RE_ESTABLISH_REQ
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_rollback_ongoing_rrc_conn_reestab_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    U16     len = RRC_NULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Send HO Failure to RRM if HO msg is buffered */
    if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
    {
        /* Stop Procedure Timer */
        if ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                /*BUG 604 changes start*/
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ) 
            /*BUG 604 changes stop*/
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
            s1ap_radio_connection_with_ue_lost;

        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            /* Being Freed Buffered RRM_HO_REQUIRED MSG */
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }

        /* Build and Send RRM_HO_FAILRE */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);
    }
    else if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_RLF_MSG)
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED, 
            "Freed Buffered RLF msg since Re-establishment Req msg is"
            "received");

        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            /* release buffered RLF msg */
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

    /* Buffered Received Re-establishment Msg */
    len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = rrc_mem_get(len)))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL, 
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
 * Function Name  :
 *      uecc_fsm_rm_llim_erab_setup_rollback_ongoing_ho_required_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_RRM_HO_REQUIRED
 ****************************************************************************/    
void uecc_fsm_rm_llim_erab_setup_rollback_ongoing_ho_required_handler(
 uecc_ue_context_t*  p_ue_context,             /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* STORE MSG IN UE CONTEXT */
    /* IF ANY MSG PRESENT IN UE CONTEXT DON'T STORE HO MSG AND DISCARD HO MSG*/

    if (RRC_HO_REQUIRED_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
    {
        /* Discard this HO message, already have a message in context */ 
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
            "HO MSG Recv: Discarding b/c HO Msg already present in UE Ctx;Unused variable p_api=%p"
            ,p_api);/*SPR 17777 +-*/
    }
    else if ((RRC_RE_ESTABLISH_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type) ||
             (RRC_RLF_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type))
    {
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_protocol;
        p_ue_context->ho_info.cause.u.radioNetwork =
            s1ap_message_not_compatible_with_receiver_state;

        /* SEND HO FAILURE TO RRM */
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
 * Function Name  :
 *      uecc_fsm_rm_llim_erab_setup_rollback_ongoing_rlf_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_RADIO_LINK_FAILURE_IND
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_rollback_ongoing_rlf_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    U16        len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Send HO Failure to RRM if HO msg is buffered */
    if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
    {
        /* Stop Procedure Timer */
        if ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
/*BUG 604 changes start*/
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ) 
/*BUG 604 changes stop*/
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
 
        /* Assign handover failure cause */
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_protocol;
        p_ue_context->ho_info.cause.u.radioNetwork =
            s1ap_radio_connection_with_ue_lost;

        /* SEND HO FAILURE TO RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);

        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            /* Freed store RRC_RRM_HO_MSG */
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
        
    }
    else if (p_ue_context->p_curr_proc_data->msg_bfr_type ==
        RRC_RE_ESTABLISH_MSG)
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,
            "Release buffered Reestablishment Req msg"
            "since RLF msg is received");

        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            /* Freed Buffered Re-establishment Req msg */
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
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
          rrc_mem_get(len)))
    {
      RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
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

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing_init_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* build and send RRCConnectionReconfiguration */
    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_erb_setup_rrc_connection_reconfiguration(p_ue_context))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL, 
            "FAILURE: UNABLE TO SEND RRCCONNECTIONRECONFIGURATION");
        
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_rrc_connection_reconfiguration_complete_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE from UE
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_rrc_connection_reconfiguration_complete_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
	rrc_return_et rrc_response = RRC_FAILURE;

	/* ARP changes begin */
	erab_list_t*   p_erab_list = PNULL;
	U8 drb_index               = RRC_NULL;
	/* ARP changes end */

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	/* Bug_10698 Fix Start*/
	/* spr_12829_fix_start */
    /*LWA_Legacy_Bug_Fix : 21690 +*/
	if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.erab_op_params.m.meas_config_present)
		/*LWA_Legacy_Bug_Fix : 21690 -*/
	{
		/* spr_12829_fix_stop */
		/* Measurment Re-config START */
		/*LWA_Legacy_Bug_Fix : 21690 +*/
		if (RRC_SUCCESS != uecc_rrc_update_CurrMeasConfigList(
                    &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.erab_op_params.meas_config,
					&p_ue_context->curr_meas_config,
					p_ue_context->p_gb_context))
			/*LWA_Legacy_Bug_Fix : 21690 -*/
		{
			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_WARNING,
					"Failure to update CurrMeasConfigList.\n!!");
		}
		if (p_ue_context->p_meas_config != PNULL)
		{
			/* Memory Optimization: Free the meas_config data.*/
			rrc_mem_free(p_ue_context->p_meas_config);
			p_ue_context->p_meas_config = PNULL;
		}
	}
	/* Bug_10698 Fix Stop */


	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context,
			UECC_ERAB_SETUP_GUARD_TIMER,
			&p_ue_context->timers_data);  

	/* build and send RRC_RRM_ERB_SETUP_CNF with success to RRM */
	rrc_response = uecc_rrm_build_and_send_erb_setup_conf(
			p_ue_context);

	if (RRC_SUCCESS == rrc_response)
	{

#ifdef ENDC_ENABLED
        /*NR_DC Code Change Start*/
        /* Send ASN.1 Encoded SgNBReconfigurationComplete to SgNB if SgNB Addition was required*/
        if ( RRC_TRUE == p_ue_context->is_sgnb_add_reqd &&
                (RRC_NULL <
                 rrc_uecc_count_scg_bearer(p_ue_context))
           )
        {
            if(RRC_SUCCESS !=
                    uecc_x2ap_build_and_send_sgnb_reconfiguration_complete(p_ue_context))
            {
                RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
                        "[x2ap_SgNBReconfigurationComplete] delivery failure");
            }

            /*reset is_sgnb_add_reqd in p_ue_context*/
            p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
        }
        /*NR_DC Code Change Stop*/
#endif

        /* Send ASN.1 Encoded ERAB SETUP Response to MME */
        if(RRC_SUCCESS !=
                uecc_s1ap_build_and_send_erab_setup_response(p_ue_context))
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
                    "[s1ap_E_RABSetup] delivery failure");
        }

		/* ARP changes begin */
		if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
				is_erab_del_ongoing == RRC_TRUE)
		{
			/* Build and send the RRC_S1AP_E_RAB_RELEASE_INDICATION msg to S1AP */
			if (RRC_SUCCESS != uecc_s1ap_build_and_send_e_rab_release_indication_arp(
						p_ue_context))
			{
				RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
						"[s1ap_E_RABSetup] Delivery failure E_RAB Release Indication");
			}
			p_ue_context->p_curr_proc_data->u.erab_setup_request_data.is_erab_del_ongoing = 
				RRC_FALSE;

			p_erab_list = &p_ue_context->p_curr_proc_data->u.
				erab_setup_request_data.erab_release_command_data.erab_release_list;

			for (drb_index =0;
					drb_index < p_erab_list->counter;
					drb_index++)
			{
				if (PNULL !=
						p_ue_context->p_e_rab_list
						[p_erab_list->erab_item[drb_index].e_RAB_ID])
				{
					rrc_mem_free(p_ue_context->p_e_rab_list
							[p_erab_list->erab_item[drb_index].e_RAB_ID]);
					p_ue_context->p_e_rab_list
						[p_erab_list->erab_item[drb_index].e_RAB_ID] = PNULL;
					/* Updates the ERAB Count in UE Context */
					p_ue_context->e_rab_list_count--;
					RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
							"E-RAB ID %u context deleted from UE Context",
							p_erab_list->erab_item[drb_index].e_RAB_ID);
				}
			}
		}
		/* ARP changes end */

		/* ICS STOP */
		uecc_fsm_finish_activity_success(
				p_ue_context, 
				(void *)UECC_EV_S1AP_ERAB_SETUP_SUCCESS_INT);
	}
	else
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
				"[RRC_RRM_ERB_SETUP_CNF] delivery failure");

	}

	uecc_llim_check_and_send_rrc_ue_information(p_ue_context);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing_timer_expiry_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		)
{
	/* ARP changes begin */
	U32      error_code = RRC_NULL;
	/* ARP changes end */

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	/* ARP changes begin */
	if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
			is_erab_del_ongoing == RRC_TRUE)
	{
		error_code |= RRC_MODULE_ID << 16;
		error_code |= RRC_TIMER_EXPIRY;

		/* Update erab release list to erab failed to release list */
		uecc_rm_erabsp_update_erab_release_list_with_error_code(
				p_ue_context,
				&error_code);
	}
	/* ARP changes start */

	/* Put all E_RAB's in failed list from ERAB Setup List */
	uecc_fsm_rm_fail_action_mark_all_erab_failed(
			p_ue_context,
			RRC_TIMER_EXPIRY);
	uecc_fsm_start_failure_processing(
			p_ue_context,
/*NR_DC Code Change Start*/
			uecc_fsm_rm_erab_setup_fail_timer_expiry_wait_for_ue_reconfig);
/*NR_DC Code Change Stop*/

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_ue_erab_setup_ongoing_rrc_conn_reestab_req_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_RRC_RE_ESTABLISH_REQ
 ****************************************************************************/
void uecc_fsm_rm_ue_erab_setup_ongoing_rrc_conn_reestab_req_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		)
{
	U16     len = RRC_NULL;

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


	/* Send HO Failure to RRM if HO msg is buffered */
	if ( p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
	{
		/* Stop Procedure Timer */
		if ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
				/*BUG 604 changes start*/
				(INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
				(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ) 
			/*BUG 604 changes stop*/
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

		/* SEND HO FAILURE TO RRM */
		uecc_rrm_build_and_send_ho_failure(p_ue_context);

		if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
		{
			/* Free Memory */
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

	/* ARP changes begin */
	if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
			is_erab_del_ongoing == RRC_TRUE)
	{
		uecc_fsm_rm_erabsp_erab_release_fail_handler(
				p_ue_context,
				RRC_REESTABLISHMENT_TRIGGERED,
                uecc_fsm_rm_erab_setup_fail_fatal_error_from_llim);

		/* Send RRC-Connection Reject to UE */
		if (RRC_SUCCESS !=
				uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
					p_ue_context->p_gb_context, p_ue_context->crnti,
					p_ue_context->reestablishmentCause,
					p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
		{
			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
					"Fail to send the RRCConnectionReestablishmentReject to UE");
		}

		/* enabling flag */
		SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

		RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
		return;
	}
	/* ARP changes end */

	/* Buffered Received Re-establishment Msg */
	len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);
	if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
				rrc_mem_get(len)))
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL, 
				"Memory Allocation Failed to store Reestablishment Req msg");

		RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
		return;
	}

	/* storing re_establishment contents into msg_bfr_rcvd */
	l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
			p_api,len);
	p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RE_ESTABLISH_MSG;

#ifdef ENDC_ENABLED
/*NR_DC Code Change START*/
    /*If SgNB Addition was triggerd then go for SgNBReconfigComplete failure*/
    if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd)
    {
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_rm_erab_setup_fail_wait_for_ue_reconfig);
    }
    else
    {
/*NR_DC Code Change Stop*/
#endif
	/* Now change the state to ERAB SETUP ROLLBACK STATE */
	/* Switch state */
        UECC_FSM_SET_CHILD_STATE(
                p_ue_context,
                &uecc_fsm_rm_llim_erab_setup_rollback_ue_ongoing,
                PNULL);
#ifdef ENDC_ENABLED
/*NR_DC Code Change START*/
    }
/*NR_DC Code Change Stop*/
#endif
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_ue_erab_setup_ongoing_ho_required_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_RRM_HO_REQUIRED
 ****************************************************************************/      
void uecc_fsm_rm_ue_erab_setup_ongoing_ho_required_handler(
		uecc_ue_context_t*  p_ue_context,       /* UECC UE context */
		void*               p_api               /* Input API buffer */
		)
{
	U16      len = RRC_NULL;

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);


	/* ARP changes begin */
	if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
			is_erab_del_ongoing == RRC_TRUE)
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
				"[UECC_EV_RRM_HO_REQUIRED]"
				"RRM Initiated E-RAB Release Procedure Ongoing"
				"Cancel the HO triggered by RRM");
		p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
		p_ue_context->ho_info.cause.u.radioNetwork = s1ap_interaction_with_other_procedure;
		if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_failure( p_ue_context))
		{
			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
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
		return;
	}
	/* ARP changes end */



	/* STORE MSG IN UE CONTEXT */
	/* IF ANY MSG PRESENT IN UE CONTEXT DON'T STORE HO MSG AND DISCARD HO MSG*/
	if (RRC_HO_REQUIRED_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
	{
		/* Discard this HO message, already have a message in context */ 
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
				"HO MSG Recv: Discarding b/c HO Msg already present in UE Ctx");
	}
	else if (PNULL == p_ue_context->p_curr_proc_data->msg_bfr_rcvd )
	{
		len = (U16)sizeof(rrc_rrm_ho_required_t);
		if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
					rrc_mem_get(len)))
		{
			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
					"memory allocation failed to store received RRC_RRM_HO_REQUIRED");
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

		/* storing re_establishment contents into msg_bfr_rcvd */
		l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
				p_api, len);

		/* Store the HO REQUIRED msg in UE Context */
		p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_HO_REQUIRED_MSG;

		/* START HO PREP TIMER */
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
				RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
						"ERROR in Starting X2_HO PREP TIMER");
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
				RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
						"ERROR in Starting INTER_CELL_HO PREP TIMER");
			}
		}
		/*BUG 604 changes start*/
		else if(HANDOVER_TYPE_INTRA_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
		{
			p_ue_context->ho_info.s1_or_x2_handover = INTRA_CELL_HO;
			if (RRC_SUCCESS != uecc_ue_timer_start(
						p_ue_context->p_gb_context,
						UECC_UE_X2_HO_PREP_TIMER,
						&p_ue_context->p_gb_context->ue_timers_data,
						&p_ue_context->timers_data,
						p_ue_context->ue_index))
			{
				RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
						"ERROR in Starting INTRA_CELL_HO PREP TIMER");
			}
		}
		/*BUG 604 changes stop*/
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
				RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
						"ERROR in Starting HO PREP TIMER");
			}

		}
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
 * Function Name  :
 *      uecc_fsm_rm_ue_erab_setup_ongoing_rlf_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_RADIO_LINK_FAILURE_IND
 ****************************************************************************/
void uecc_fsm_rm_ue_erab_setup_ongoing_rlf_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		)
{
	U16          len = RRC_NULL;
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


	/* Send HO Failure to RRM if HO msg is buffered */
	if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
	{
		/* Stop Procedure Timer */
		if ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
				/*BUG 604 changes start*/
				(INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
				(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ) 
			/*BUG 604 changes stop*/
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

		/* SEND HO FAILURE TO RRM */
		uecc_rrm_build_and_send_ho_failure(p_ue_context);

		if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
		{
			/* Freed stored buffer */
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

	/* ARP changes begin */
	if (p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
			is_erab_del_ongoing == RRC_TRUE)
	{
		uecc_fsm_rm_erabsp_erab_release_fail_handler(
				p_ue_context,
				RRC_RADIO_LINK_FAILURE_TRIGGERED,
				uecc_fsm_rm_erab_setup_fail_fatal_error_from_llim);

		/* enabling flag */
		SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

		RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
		return;
	}
	/* ARP changes end */

	len = (U16)sizeof(rrc_uecc_llim_radio_link_failure_ind_t);
	if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
				rrc_mem_get(len)))
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
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

#ifdef ENDC_ENABLED
/*NR_DC Code Change START*/
    /*If SgNB Addition was triggerd then go for SgNBReconfigComplete failure*/
    if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd)
    {
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_rm_erab_setup_fail_wait_for_ue_reconfig);
    }
    else
    {
        /*NR_DC Code Change Stop*/
#endif
        /* Now change the state to ERAB SETUP ROLLBACK STATE */
        /* Switch state */
        UECC_FSM_SET_CHILD_STATE(
                p_ue_context,
                &uecc_fsm_rm_llim_erab_setup_rollback_ue_ongoing,
                PNULL);
#ifdef ENDC_ENABLED
/*NR_DC Code Change START*/
    }
/*NR_DC Code Change Stop*/
#endif
	/* Now change the state to ERAB SETUP ROLLBACK STATE */
	/* Switch state */
	UECC_FSM_SET_CHILD_STATE(
			p_ue_context,
			&uecc_fsm_rm_llim_erab_setup_rollback_ue_ongoing,
			PNULL);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of UECC_RM_FAIL_ACTION_SEND_RRC_RRM_ERB_SETUP_CNF,
 *                  RRC_RRM_ERB_SETUP_CNF fail message to RRM
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf(
		uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
		rrc_bool_et*        p_break
		)
{

	uecc_rm_erab_setup_info_t    *p_erab_info_list = 
		&p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
		erab_info_data;

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_break);

	/* Set the ERAB SETUP LIST outerResponse TO FAILURE */
	p_erab_info_list->outerResponse = FAILURE;

	if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_setup_conf
			(p_ue_context))
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
				"FAILURE: Send RRC_RRM_ERB_SETUP_CNF (Fail)");
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_fail_action_send_erb_setup_resp
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  rrc_bool_et*        p_break
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Send ASN1 Encoded ERAB SETUP RESPONSE to MME
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_fail_action_send_erab_setup_resp(
		uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
		rrc_bool_et*        p_break
		)
{


	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_break);

	RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
			"FAILURE: Send ASN1 Encoded ERAB SETUP RESPONSE(Fail)");

	/* Send ASN.1 Encoded ERAB SETUP Response to MME */
	if(RRC_SUCCESS !=
			uecc_s1ap_build_and_send_erab_setup_response(p_ue_context))
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
				"[s1ap_E_RABSetup] delivery failure");
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_fail_action_delete_drb_info_from_ctx
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  rrc_bool_et*        p_break
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of UECC_RM_FAIL_ACTION_DELETE_DRB_INFO_FROM_CTX.
 *                      Delete DRB info from UE context
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_fail_action_delete_drb_info_from_ctx(
		uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
		rrc_bool_et*        p_break
		)
{

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_break);

	RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
			"FAILURE: Delete DRB info from UE context");

	uecc_fsm_rm_fail_action_mark_all_erab_failed(
			p_ue_context,
			0);
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_fail_action_last
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  rrc_bool_et*        p_break
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of UECC_EV_S1AP_ERAB_SETUP_FAILURE_INT event.
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_fail_action_last(
		uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
		rrc_bool_et*        p_break
		)
{

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_break);

	RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO, "FAILURE: END");

	/* notify procedure about failure */
	uecc_fsm_finish_activity_failure(
			p_ue_context, (void*)UECC_EV_S1AP_ERAB_SETUP_FAILURE_INT);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_fatal_error_action_last
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  rrc_bool_et*        p_break
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Call failure handler with event
 UECC_EV_S1AP_ERAB_SETUP_PROC_TIMER_EXPIRY_INT
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_fail_fatal_error_action_last(
		uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
		rrc_bool_et*        p_break
		)
{

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_break);

	RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO, "FAILURE: FATAL ERROR");

	/* notify procedure about failure */
	uecc_fsm_finish_activity_failure(
			p_ue_context, (void*)UECC_EV_S1AP_ERAB_SETUP_PROC_TIMER_EXPIRY_INT);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}



/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_build_s1ap_ho_type
 * Inputs         : U32 *s1ap_ho_type - pointer to S1AP HO Type
 *                : U8 ho_type - HO Type received from RRM
 * Outputs        : None
 * Returns        : None
 * Description    : updates the value of  s1ap_ho_type. 
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_build_s1ap_ho_type(
		U32 *s1ap_ho_type, /* S1AP HO Type */ 
		U8  ho_type        /* HO Type from RRM */ 
		)
{
	switch( ho_type )
	{
		case 0:
			*s1ap_ho_type = s1ap_s1_intra_system_handover_triggered;
			break;
		default:
			*s1ap_ho_type = s1ap_s1_inter_system_handover_triggered; 
			break;
	}
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_build_fail_list_with_radio_cause
 * Inputs         : uecc_ue_context_t - Pointer to UE Context
 *                : s1ap_E_RAB_ID e_rab_ID - ERAB ID
 *                : s1ap_CauseRadioNetwork radioCause - Radio Network Cause
 * Outputs        : None
 * Returns        : None
 * Description    : Put all erabs to fail in failed list with radio cause 
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_build_fail_list_with_radio_cause(
		uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
		s1ap_E_RAB_ID e_rab_ID,                     /* ERAB ID */
		s1ap_CauseRadioNetwork radioCause           /* Radio Network Cause */
		)
{
	rrc_counter_t       fail_list_count = 0;

	uecc_erab_failed_to_setup_list_t* p_erab_fail_list = &p_ue_context->
		p_curr_proc_data->u.erab_setup_request_data.failed_erab_data;

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	fail_list_count =  p_erab_fail_list->count; 

	p_erab_fail_list->erab_failed_to_setup_item[fail_list_count].e_RAB_ID = 
		e_rab_ID; 

	p_erab_fail_list->erab_failed_to_setup_item[fail_list_count].cause.t = 
		T_s1ap_Cause_radioNetwork;

	p_erab_fail_list->erab_failed_to_setup_item[fail_list_count].cause.u.
		radioNetwork = radioCause;

	p_erab_fail_list->count++;

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_rrm_erab_setup_ongoing_s1ap_reset_msg_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_S1AP_RESET_MSG_IND
 ****************************************************************************/
void uecc_fsm_rm_rrm_erab_setup_ongoing_s1ap_reset_msg_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		)
{
	RRC_ASSERT(PNULL != p_ue_context);    
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context,
			UECC_ERAB_SETUP_GUARD_TIMER,
			&p_ue_context->timers_data);

	uecc_fsm_rm_fail_action_mark_all_erab_failed(
			p_ue_context,
			RRC_INTERACTION_WITH_OTHER_PROCEDURE);
	/* Send event to UE Main FSM */
	uecc_event_queue_push_event(
			UECC_EV_S1AP_RESET_MSG_IND_INT,
			p_ue_context,
			p_api);
	uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf(
			p_ue_context,
			PNULL);

	/* Stop Procedure */
	uecc_fsm_finish_active_procedure(p_ue_context);


	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_erab_setup_ongoing_ue_rel_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_UE_RELEASE_REQ
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_ongoing_ue_rel_req_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		)
{
	rrc_s1ap_ue_context_release_command_t *p_ue_context_rel_command; 

	U32 error_code;
	s1ap_Cause cause;

	RRC_ASSERT(PNULL != p_ue_context);    
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	p_ue_context_rel_command = (rrc_s1ap_ue_context_release_command_t*)p_api;

	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context,
			UECC_ERAB_SETUP_GUARD_TIMER,
			&p_ue_context->timers_data);

	/* Process failure
	 ** Procedure needs to be terminated.
	 */
	cause = p_ue_context_rel_command->cause;
	error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
	RRC_UECC_ERB_SETUP_UPDATE_ERROR_CODE(error_code);


	/* Make All Success E_RAB Fail and put in fail list */
	uecc_fsm_rm_fail_action_mark_all_erab_failed_with_cause(
			p_ue_context,
			error_code,
			cause);


	/* Build and send RRC_RRM_ERB_MODIFY_CNF with success to RRM */
	uecc_fsm_rm_erab_setup_fail_action_send_rrm_erb_setup_cnf(
			p_ue_context, 
			PNULL);    

	/* Send the ASN. 1 E-RAB SETUP RESPONSE message containing 
	 * all failures to MME.*/
	if (RRC_SUCCESS !=
			uecc_s1ap_build_and_send_erab_setup_response(p_ue_context))
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_WARNING,
				"[s1ap_E_RABSetupResponse] Delivery failure");
	}    

	if (PNULL != p_ue_context->p_curr_proc_data)
	{
		/* Free the data for current procedure in the UE Context */
		rrc_mem_free(p_ue_context->p_curr_proc_data);
		p_ue_context->p_curr_proc_data = PNULL;
	}

	/* Stop Procedure */
	uecc_fsm_finish_active_procedure(p_ue_context);


	/* Send event to UE Main FSM */
	uecc_event_queue_push_event(
			UECC_EV_UE_RELEASE_REQ_INT,
			p_ue_context,
			p_api);


	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_llim_erab_setup_ongoing_s1ap_reset_msg_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_S1AP_RESET_MSG_IND
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_ongoing_s1ap_reset_msg_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		)
{
	RRC_ASSERT(PNULL != p_ue_context);    
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context, 
			UECC_ERAB_SETUP_GUARD_TIMER,
			&p_ue_context->timers_data);

	/* Here Update failure cause = s1ap_interaction_with_other_procedure 
	 * in ERAB Setup List.
	 * Send Failure Confimation to RRM.
	 */


	/* Send event to UE Main FSM */
	uecc_event_queue_push_event(
			UECC_EV_S1AP_RESET_MSG_IND_INT,
			p_ue_context,
			p_api);

	/* Stop Procedure */
	uecc_fsm_finish_active_procedure(p_ue_context);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_llim_erab_setup_rollback_ongoing_s1ap_reset_msg_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_S1AP_RESET_MSG_IND
 ****************************************************************************/
void uecc_fsm_rm_llim_erab_setup_rollback_ongoing_s1ap_reset_msg_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		)
{
	RRC_ASSERT(PNULL != p_ue_context);    
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	uecc_fsm_rm_llim_erab_setup_ongoing_s1ap_reset_msg_handler(
			p_ue_context,
			p_api);


	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);


}


/****************************************************************************
 * Function Name  :
 *      uecc_fsm_rm_ue_erab_setup_ongoing_s1ap_reset_msg_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_S1AP_RESET_MSG_IND
 ****************************************************************************/
void uecc_fsm_rm_ue_erab_setup_ongoing_s1ap_reset_msg_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		)
{
    RRC_ASSERT(PNULL != p_ue_context);    
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

#ifdef ENDC_ENABLED
    /*NR_DC Code Change Start*/
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
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
                    "[x2ap_SgNBReconfigurationComplete] delivery failure");
        }

        /*reset is_sgnb_add_reqd in p_ue_context*/
        p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    }
    /*NR_DC Code Change Stop*/
#endif
    uecc_fsm_rm_llim_erab_setup_ongoing_s1ap_reset_msg_handler(
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);


}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_ue_erab_setup_read_op_rrm_params
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_rrm_erb_setup_resp_t* - pointer to RRM ERAB SETUP RSP
 *                  uecc_rm_erab_setup_op_data_t* - pointer to temp structure 
 *                  for storing ooptional parameters
 * Outputs        : RRC_SUCCESS/RRC_FAILURE
 * Returns        : None
 * Description    : This reads optional parameters received from RRM in 
 *                  RRC_RRM_ERB_SETUP_RESP
 ****************************************************************************/
rrc_return_t uecc_fsm_rm_erab_setup_read_op_rrm_params(
		uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
		rrc_rrm_erb_setup_resp_t*  p_rrm_erb_setup_resp, /* RRM ERB SETUP RSP */ 
		uecc_rm_erab_setup_op_data_t*     p_erab_op_params         
		)
{

	RRC_ASSERT(PNULL != p_ue_context);    
	RRC_ASSERT(PNULL != p_rrm_erb_setup_resp);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	/* Store UE parameters in temporary structure */

	/* Store physical_config_dedicated if present */
	if (RRM_ERAB_SETUP_RADIO_RES_CONFIG_PHY_CONFIG_DED_PRESENT &
			p_rrm_erb_setup_resp->radio_rsource_config.bitmask )
	{
		p_erab_op_params->m.physical_config_dedicated_present = 1;
		p_erab_op_params->physical_config_dedicated =
			p_rrm_erb_setup_resp->radio_rsource_config.
			physical_config_dedicated;
                /*SPR_16942_START*/ 
                /*Code Removed */
                /*SPR_16942_END*/ 

		/* cr_311 sr_ConfigIndex range check */ 
		if (p_ue_context->m.access_stratum_release)
		{
			if (((p_rrm_erb_setup_resp->radio_rsource_config.physical_config_dedicated.
							scheduling_request_config.scheduling_request_config_param.
							sr_configuration_index == 156) ||
						(p_rrm_erb_setup_resp->radio_rsource_config.physical_config_dedicated.
						 scheduling_request_config.scheduling_request_config_param.
						 sr_configuration_index == 157)) &&
					((p_ue_context->access_stratum_release == rrm_rel8)))
			{
				RRC_UECC_TRACE (p_ue_context->p_gb_context->context_index,
						p_ue_context->p_gb_context->facility_name,
						RRC_INFO,"Release 8 do not support"
						" Values 156 or 157 for sr_ConfigIndex");
			}
		}     
/*SPR_16942_START*/
		if(RRC_PHY_ANTENNA_INFORMATION_PRESENT &
				p_erab_op_params->physical_config_dedicated.bitmask)
		{
			if(!(RRC_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT &
						p_erab_op_params->physical_config_dedicated
						.antenna_information.bitmask))
			{   /* Default values shall be used */
				RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
						p_ue_context->p_gb_context->facility_name,RRC_INFO,
						"[RRC_RRM_ERB_SETUP_RESP] In PHY Dedicated Info, "
						"Antenna info is not present.");
			}
			else
			{
				if (RRC_FAILURE ==
						uecc_fsm_validate_antenna_codebook_subset_restriction(
							&p_erab_op_params->physical_config_dedicated.
							antenna_information.antenna_information_explicit_value,
							p_ue_context))
				{
					RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
							p_ue_context->p_gb_context->facility_name,RRC_INFO,
							"[RRC_RRM_ERB_SETUP_RESP] Code Book Subset Restriction"
							" Validation Failed");
					return RRC_FAILURE;
				}
			}
		}

		/* CR 316 : verifying antenna_info_v920 along with TM 8 presence */
		if (p_ue_context->physical_config_dedicated.antenna_info_v920.bitmask &
				RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT)
		{    
			if((RRC_PHY_ANTENNA_INFORMATION_PRESENT &
						p_erab_op_params->physical_config_dedicated.bitmask) && 
					(p_erab_op_params->physical_config_dedicated.antenna_information.
					 antenna_information_explicit_value.transmission_mode ==
					 TRANSMISSION_MODE_TM8_V920))
			{
				RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
						p_ue_context->p_gb_context->facility_name,RRC_INFO,
						"Transmission Mode is set to --> 8");
			}
			else
			{
				RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
						p_ue_context->p_gb_context->facility_name,RRC_INFO,
						"[RRC_RRM_ERB_SETUP_RESP] In PHY Dedicated Info, "
						"Antenna info is not present."
						" or Transmission mode != 8");
				p_erab_op_params->physical_config_dedicated.bitmask ^=
					RRC_PHY_ANTENNA_INFO_V920_PRESENT;
			}
		}    
		/*Bug_12240_fix_start*/
		if ((p_erab_op_params->physical_config_dedicated.bitmask &
					RRC_PHY_ANTENNA_INFO_R10_PRESENT) &&
				(p_erab_op_params->physical_config_dedicated.antenna_info_r10.
				 bitmask & RRC_ANTENNA_INFO_R10_EXPLICIT_VALUE_PRESENT))
		{
			if (RRC_FAILURE ==
					uecc_fsm_validate_antenna_codebook_subset_restriction_r10(
						&p_erab_op_params->physical_config_dedicated.
						antenna_info_r10.explicit_value,p_ue_context))
			{
				RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
						p_ue_context->p_gb_context->facility_name,RRC_INFO,
						"[RRC_RRM_ERB_SETUP_RESP] Code Book Subset"
						" Restriction for r10 Validation Failed");
				return RRC_FAILURE;
			}
		}
		/*Bug_12240_fix_stop*/
		/* CR 247: Added cqi_report_config_v920 */
		if (!(p_erab_op_params->physical_config_dedicated.bitmask &
					RRC_PHY_CQI_REPORTING_PRESENT))
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
					p_ue_context->p_gb_context->facility_name,RRC_INFO,
					"CQI_REPORTING Config is not present");
			/*Bug_12721_start*/
			if (p_erab_op_params->physical_config_dedicated.bitmask &
					RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT)
			{
				p_erab_op_params->physical_config_dedicated.bitmask ^=
					RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT;
			}
			/*Bug_12721_stop*/
		}
		else
		{    
			/*Bug_12721_start*/
			if (p_erab_op_params->physical_config_dedicated.bitmask &
					RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT)
				/*Bug_12721_stop*/
			{
				/* Varifying the conditional presence of CQI_MASK_R9 */
				if (p_erab_op_params->physical_config_dedicated.cqi_report_config_v920.bitmask &
						RRC_PHY_CQI_MASK_R9_PRESENT) 
				{
					if (p_erab_op_params->physical_config_dedicated.cqi_reporting.
							cqi_reporting_periodic.bitmask & 
							RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT)
					{
						RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
								p_ue_context->p_gb_context->facility_name,RRC_INFO,
								"CQI_MASK_R9 ::  Field cqi-ReportPeriodic is present"
								" and set to SETUP");
					}    
					else
					{
						RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
								p_ue_context->p_gb_context->facility_name,RRC_INFO,
								"[RRC_RRM_ERB_SETUP_RESP] In PHY Dedicated Info, "
								" Field cqi-ReportPeriodic is set to RELEASE");
						p_erab_op_params->physical_config_dedicated.cqi_report_config_v920.
							bitmask ^= RRC_PHY_CQI_MASK_R9_PRESENT;

						if (!(p_erab_op_params->physical_config_dedicated.
									cqi_report_config_v920.bitmask & RRC_PHY_PMI_RI_REPORT_R9_PRESENT))
						{
							p_erab_op_params->physical_config_dedicated.bitmask ^=
								RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT;
						}    
					}   
				}
				/* Varifying the conditional presence of pmi-RI-Report-r9 */
				if (p_erab_op_params->physical_config_dedicated.cqi_report_config_v920.bitmask &
						RRC_PHY_PMI_RI_REPORT_R9_PRESENT)
				{      
					if ((p_erab_op_params->physical_config_dedicated.cqi_reporting.
								cqi_reporting_periodic.bitmask &
								RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT) &&
							(p_erab_op_params->physical_config_dedicated.cqi_reporting.bitmask &
							 RRC_PHY_CQI_REPORTING_MODE_APERIODIC_PRESENT))
					{   
						RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
								p_ue_context->p_gb_context->facility_name,RRC_INFO,
								"PMI_RI_REPORT_R9 :: Field cqi-ReportPeriodic is present and set to SETUP " 
								" and cqi-ReportModeAperiodic is also Present");
					}
					else
					{
						RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
								p_ue_context->p_gb_context->facility_name,RRC_INFO,
								"[RRC_RRM_ERB_SETUP_RESP] In PHY Dedicated Info, "
								"PMI_RI_REPORT_R9 ::  Field cqi-ReportPeriodic is set to RELEASE"
								" or APERIODIC is not present");
						p_erab_op_params->physical_config_dedicated.cqi_report_config_v920.
							bitmask ^= RRC_PHY_PMI_RI_REPORT_R9_PRESENT;
						if (!(p_erab_op_params->physical_config_dedicated.cqi_report_config_v920.
									bitmask & RRC_PHY_CQI_MASK_R9_PRESENT))
						{
							p_erab_op_params->physical_config_dedicated.bitmask ^=
								RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT;
						}    
					}
				}
			}
		}    
	}
/*SPR_16942_END*/    
	/* SPS Start */
	/* Store sps_config if present */
	if (RRM_ERAB_SETUP_RADIO_RES_CONFIG_SPS_CONFIG_PRESENT &
			p_rrm_erb_setup_resp->radio_rsource_config.bitmask )
	{
		if (RRC_FAILURE == uecc_rrm_parse_and_update_sps_config(
					p_ue_context,
					&p_rrm_erb_setup_resp->radio_rsource_config.sps_config))
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
					p_ue_context->p_gb_context->facility_name,
					RRC_WARNING,"[RRC_RRM_ERB_SETUP_RESP] SPS config updation failed");
		}

	}
	/* SPS Stop */

	/* Store mac_main_config_t if present */
	if (RRM_ERAB_SETUP_RADIO_RES_CONFIG_MAC_CONFIG_PRESENT &
			p_rrm_erb_setup_resp->radio_rsource_config.bitmask )
	{
		p_erab_op_params->m.mac_config_present = 1;

		/* Update MAC Config in UE Context */
                /*SPR_16942_START*/
                /*Code Removed*/
                /*SPR_16942_END*/

		if ( RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT & 
				p_rrm_erb_setup_resp->radio_rsource_config.mac_config.bitmask)
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
					p_ue_context->p_gb_context->facility_name,RRC_INFO,
					"ERAB setup: mac_main_config present in RRM erb setup resp");
			p_erab_op_params->m.mac_main_config_present = 1;
			p_erab_op_params->mac_config.mac_main_config =
				p_rrm_erb_setup_resp->radio_rsource_config.mac_config.mac_main_config;

/*SPR_16942_START*/
                }
                p_erab_op_params->mac_config.mac_main_config_extensions =
                    p_rrm_erb_setup_resp->radio_rsource_config.mac_config.mac_main_config_extensions;
/*SPR_16942_END*/
	}

	/* Store rrm_meas_config_t if present */
	if (RRM_ERAB_SETUP_RADIO_RES_CONFIG_MEAS_CONFIG_PRESENT &
			p_rrm_erb_setup_resp->radio_rsource_config.bitmask )
	{
        /*LWA_Legacy_Bug_Fix : 21690 +*/
		p_erab_op_params->m.meas_config_present = 1;
        /*LWA_Legacy_Bug_Fix : 21690 -*/
		p_ue_context->m.meas_config_present = 1;

		/*** TDD changes start ***/
		p_ue_context->m.meas_config_updated = 1;
		/*** TDD changes stop ***/

		p_erab_op_params->meas_config =
			p_rrm_erb_setup_resp->radio_rsource_config.meas_config;
		/* Copy the config into UE Context so that the config is
		 * correctly updated into Curr Meas on receiving the RRC Reconf Complete
		 * in uecc_llim_process_dcch_rrcConnectionReconfigurationComplete() */
	}

	/* Copy Proximity Configuration */
	if (p_rrm_erb_setup_resp->radio_rsource_config.bitmask &
			RRM_ERAB_SETUP_RADIO_RES_CONFIG_PROXIMITY_PRESENT )
	{
		p_erab_op_params->m.proximity_config_present = 1;
		l3_memcpy_wrapper(&(p_erab_op_params->proximity_config),
				&(p_rrm_erb_setup_resp->radio_rsource_config.proximity_config),
				sizeof(rrm_report_proximity_config_r9_t));
/*SPR_16942_START*/
        }
        else
        {
                p_erab_op_params->m.proximity_config_present = 0;
                memset_wrapper(&(p_erab_op_params->proximity_config), RRC_NULL,
                                sizeof(rrm_report_proximity_config_r9_t));
/*SPR_16942_END*/                

	}
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
	return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_fail_stop_proc_timer
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Stop ERAB SETUP Procedural Timer
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_fail_stop_proc_timer(
		uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
		rrc_bool_et*        p_break
		)
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_break);

	RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO, "FAILURE: Stop Timer");

	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context, 
			UECC_ERAB_SETUP_GUARD_TIMER,
			&p_ue_context->timers_data);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}




/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_action_append_erab_to_fail_list
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_response_et    - Response from LLIM in case of rollback 
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion populate failed list from success list and 
 Set status in ERAB List to error code which will sent to 
 RRM.
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_action_append_erab_to_fail_list(
		uecc_ue_context_t*  p_ue_context,
		rrc_response_t     responseCode
		)
{
	U8 fail_list_count   =  0;
	U8 succ_list_count   =  0;
	U8 succ_list_counter =  0;
	U32 error_code       =  0;

	uecc_rm_erab_setup_info_t *erab_setup_list = &p_ue_context->
		p_curr_proc_data->u.erab_setup_request_data.erab_info_data;

	uecc_erab_failed_to_setup_list_t *fail_erab_list = &p_ue_context->
		p_curr_proc_data->u.erab_setup_request_data.failed_erab_data;


	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	succ_list_count = erab_setup_list->count; 

	fail_list_count = fail_erab_list->count;

	for (succ_list_counter = 0; 
			succ_list_counter < succ_list_count;
			succ_list_counter++)
	{

		/* SPR 2511 Start */
		if (RRC_NO_ERROR == erab_setup_list->erab_info[succ_list_counter].
				status)
		{
			/* Copy E_RAB ID from success E_RAB List to Fail List */ 
			fail_erab_list->erab_failed_to_setup_item[fail_list_count].e_RAB_ID = 
				erab_setup_list->erab_info[succ_list_counter].erab_id;

			/* Assign S1AP Cause Type in Fail List */
			fail_erab_list->erab_failed_to_setup_item[fail_list_count].cause.t =  
				T_s1ap_Cause_radioNetwork;

			/* If responseCode is FAILURE i.e Rollback is failed at LLIM */
			if (responseCode == FAILURE)
			{
				fail_erab_list->erab_failed_to_setup_item[fail_list_count].cause.u.
					radioNetwork = s1ap_radio_connection_with_ue_lost;

				if (RRC_RE_ESTABLISH_MSG == 
						p_ue_context->p_curr_proc_data->msg_bfr_type)
				{ 
					error_code = RRC_REESTABLISHMENT_TRIGGERED_ROLLBACK_FAILURE;
					RRC_UECC_ERB_SETUP_UPDATE_ERROR_CODE(error_code);
					erab_setup_list->erab_info[succ_list_counter].status = 
						error_code;
				}
				else if (RRC_HO_REQUIRED_MSG == 
						p_ue_context->p_curr_proc_data->msg_bfr_type)
				{ 
					if((((rrc_rrm_ho_required_t*)p_ue_context->
									p_curr_proc_data->msg_bfr_rcvd)->ho_type == 
								HANDOVER_TYPE_INTRA_LTE_S1 ||
								((rrc_rrm_ho_required_t*)p_ue_context->
								 p_curr_proc_data->msg_bfr_rcvd)->ho_type == 
								HANDOVER_TYPE_INTRA_LTE_X2) ||
							((rrc_rrm_ho_required_t*)p_ue_context->
							 p_curr_proc_data->msg_bfr_rcvd)->ho_type == 
							/*BUG 604 changes start*/
							HANDOVER_TYPE_INTER_CELL ||
							((rrc_rrm_ho_required_t*)p_ue_context->
							 p_curr_proc_data->msg_bfr_rcvd)->ho_type == 
							HANDOVER_TYPE_INTRA_CELL )
						/*BUG 604 changes stop*/
					{ 
						fail_erab_list->erab_failed_to_setup_item[fail_list_count].cause.u.
							radioNetwork = s1ap_s1_intra_system_handover_triggered;
					}
					else
					{
						fail_erab_list->erab_failed_to_setup_item[fail_list_count].cause.u.
							radioNetwork = s1ap_s1_inter_system_handover_triggered;
					}
					error_code = RRC_HANDOVER_ROLLBACK_TRIGGERED_FAILURE;
					RRC_UECC_ERB_SETUP_UPDATE_ERROR_CODE(error_code);
					erab_setup_list->erab_info[succ_list_counter].status = 
						error_code;
				}
				else if (RRC_RLF_MSG == 
						p_ue_context->p_curr_proc_data->msg_bfr_type)
				{ 
					error_code = RRC_RADIO_LINK_FAILURE_TRIGGERED_ROLLBACK_FAILURE;
					RRC_UECC_ERB_SETUP_UPDATE_ERROR_CODE(error_code);
					erab_setup_list->erab_info[succ_list_counter].status = 
						error_code;
				}
			}
			else
			{
				if (RRC_RE_ESTABLISH_MSG == 
						p_ue_context->p_curr_proc_data->msg_bfr_type)  
				{
					/* SPR 13369 Fix Start */
					fail_erab_list->erab_failed_to_setup_item[fail_list_count].cause.u.
						radioNetwork = s1ap_failure_in_radio_interface_procedure;
					/* SPR 13369 Fix Stop */
					error_code = RRC_REESTABLISHMENT_TRIGGERED;
					RRC_UECC_ERB_SETUP_UPDATE_ERROR_CODE(error_code);
					erab_setup_list->erab_info[succ_list_counter].status = 
						error_code;
				}
				else if (RRC_HO_REQUIRED_MSG == 
						p_ue_context->p_curr_proc_data->msg_bfr_type)
				{
					if(((rrc_rrm_ho_required_t*)p_ue_context->
								p_curr_proc_data->msg_bfr_rcvd)->ho_type == HANDOVER_TYPE_INTRA_LTE_S1)
					{ 
						fail_erab_list->erab_failed_to_setup_item[fail_list_count].cause.u.
							radioNetwork = s1ap_s1_intra_system_handover_triggered;
					}
					else
					{
						fail_erab_list->erab_failed_to_setup_item[fail_list_count].cause.u.
							radioNetwork = s1ap_s1_inter_system_handover_triggered;
					}
					error_code = RRC_HANDOVER_TRIGGERED;
					RRC_UECC_ERB_SETUP_UPDATE_ERROR_CODE(error_code);
					erab_setup_list->erab_info[succ_list_counter].status = 
						error_code; 
				}
				else if (RRC_RLF_MSG == 
						p_ue_context->p_curr_proc_data->msg_bfr_type)
				{
					fail_erab_list->erab_failed_to_setup_item[fail_list_count].cause.u.
						radioNetwork = s1ap_radio_connection_with_ue_lost;
					error_code = RRC_RADIO_LINK_FAILURE_TRIGGERED;
					RRC_UECC_ERB_SETUP_UPDATE_ERROR_CODE(error_code);
					erab_setup_list->erab_info[succ_list_counter].status = 
						error_code;
				}
			}

			/* Deallocate E_RAB From E_RAB List in UE Context */
			uecc_fsm_rm_erab_setup_deallocate_erab(
					p_ue_context,           
					erab_setup_list->erab_info[succ_list_counter].erab_id);

			fail_list_count++;

		}
		/* SPR 2511 Stop */
	}


	/* Copy Failed Count to Fail List */
	fail_erab_list->count = fail_list_count;

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_fail_action_ue_ctx_rel_req
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Send UE Context Release Request
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_fail_action_send_ue_ctx_rel_req(
		uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
		rrc_bool_et*        p_break
		)
{
	s1ap_Cause cause;

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_break);

	RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO, "FAILURE: Send UE Ctx Release");

	/* Fill Cause */
	cause.t = T_s1ap_Cause_radioNetwork;
	cause.u.radioNetwork = s1ap_release_due_to_eutran_generated_reason; 

	/* Now build and send UE CONTEXT RELEASE REQ to MME */
	if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
				p_ue_context,
				&cause))
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
				p_ue_context->p_gb_context->facility_name,
				RRC_WARNING,
				"[%s]failed to send UE CONTEXT RELEASE REQ to MME",__FUNCTION__);
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}



/****************************************************************************
 * Function Name  : uecc_fsm_rm_fail_action_mark_all_erab_failed_with_cause
 * Inputs         : uecc_ue_context_t*  p_ue_context,
 *                  U32                 error_code,
 *                  s1ap_Cause          cause
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion populate failed list from success list
 ****************************************************************************/
void uecc_fsm_rm_fail_action_mark_all_erab_failed_with_cause(
		uecc_ue_context_t*  p_ue_context,
		U32                 error_code,
		s1ap_Cause          cause
		)
{
	U8 fail_list_count = 0;
	U8 succ_count = 0;
	U8 succ_list_counter = 0;

	uecc_rm_erab_setup_info_t*         p_erab_setup_list = 
		&p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
		erab_info_data;
	uecc_erab_failed_to_setup_list_t*  p_erab_fail_list = 
		&p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
		failed_erab_data;

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	succ_count = p_erab_setup_list->count; 

	fail_list_count =
		p_erab_fail_list->count;

	for (succ_list_counter = 0; 
			succ_list_counter < succ_count; 
			succ_list_counter++)
	{
		if (RRC_NO_ERROR ==
				p_erab_setup_list->erab_info[succ_list_counter].
				status)
		{
			/* Set status to error_code in E_RAB Setup List */
			p_erab_setup_list->erab_info[succ_list_counter].status
				= error_code;

			/* Copy E_RAB ID to Failed List */
			p_erab_fail_list->erab_failed_to_setup_item[fail_list_count]
				.e_RAB_ID = p_erab_setup_list->erab_info[succ_list_counter].
				erab_id;

			/* Copy Cause in Fail List */                             
			p_erab_fail_list->erab_failed_to_setup_item[fail_list_count]
				.cause = cause;

			/* Delete Failed ERAB from UE CONTEXT */
			uecc_fsm_rm_erab_setup_deallocate_erab(
					p_ue_context,           
					p_erab_setup_list->erab_info[succ_list_counter].erab_id);

			fail_list_count++;
		}
	}

	/* Copy Incremented fail list count */
	p_erab_fail_list->count =
		fail_list_count;

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_rm_llim_erab_setup_ho_prep_timer_expiry_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This function sends the RRC_RRM_HO_FAILURE message to RRM.
 *
 ****************************************************************************/

void uecc_fsm_rm_llim_erab_setup_ho_prep_timer_expiry_handler 
(
 uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
 void*               p_api                  /* Input API buffer */
 )  
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);


	if (PNULL != p_ue_context->p_curr_proc_data)
	{
		/* Check if HO message is present in the curr_proc_data */
		if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
		{
			if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
			{
				/* Free Buffered RRC_HO_REQUIRED_MSG */
				rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
				p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
			}

			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO, 
					"[UECC_EV_HO_PREP_TIMER_EXPIRY]"
					"HO Prep Timer Expired");
			p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
			p_ue_context->ho_info.cause.u.radioNetwork = s1ap_tS1relocprep_expiry;

			/* Send HO Failure to the RRM */
			if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_failure( p_ue_context))
			{
				RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
						"Fail to send the handover failure indication to RRM");
			}
		}
		else
		{
			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
					"[UECC_EV_HO_PREP_TIMER_EXPIRY]: Invalid Event Received");
		}

		/* Free the data for current procedure in the UE Context */
		rrc_mem_free(p_ue_context->p_curr_proc_data);  
		p_ue_context->p_curr_proc_data = PNULL;
	}
	else
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
				p_ue_context->p_gb_context->facility_name, RRC_WARNING,
				"p_ue_context->p_curr_proc_data is NULL");
	}

	/* Stop Procedure */
	uecc_fsm_finish_active_procedure(p_ue_context);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_rollback_last_fail_action
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*  
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion sends RRC-Connection Re-establishment Reject
 *                  in case of rollback is due to Re-establishment and clears
 *                  re-establishment buffer in UE Context
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_rollback_last_fail_action(
		uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
		rrc_bool_et*        p_break
		)
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_break);

	do
	{
		if(p_ue_context->p_curr_proc_data->msg_bfr_type ==
				RRC_RE_ESTABLISH_MSG)
		{
			/* Send RRC-Connection Reject to UE */
			if (RRC_SUCCESS !=
					uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
						p_ue_context->p_gb_context, p_ue_context->crnti, 
						p_ue_context->reestablishmentCause,
						p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
			{
				RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
						"Fail to send the RRCConnectionReestablishmentReject to UE");
			}
			else
			{
				RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
						"RRCConnectionReestablishmentReject Send to UE Successfully");
			}

			/* Clears RRC-Connection Re-establishment Buffer in UE Context*/
			if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
			{
				rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
				p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
			}
			p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_NO_MSG;

			/* enabling flag */
			SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

			/* notify procedure about failure */
			uecc_fsm_finish_activity_failure(
					p_ue_context, (void*)UECC_EV_S1AP_ERAB_SETUP_ROLLBACK_FAILURE_REL_UE_CTX_INT);
		}
		else
		{
			/* notify procedure about failure */
			uecc_fsm_finish_activity_failure(
					p_ue_context, (void*)UECC_EV_S1AP_ERAB_SETUP_FAILURE_INT);
		}
	}while(0);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* ARP CHANGES BEGIN */

/****************************************************************************
 * Function Name  : update_curr_proc_erbsp_data_with_response_code
 *   
 * Input          : uecc_ue_context_t* p_ue_context
 *                  void*              p_response_code 
 *
 * Output         : None
 *
 * Returns        : None
 *
 * Description    :
 *     This function update curr proc data with received 
 *     rrc_uecc_llim_delete_lc_resp from LLIM.
 *
 ****************************************************************************/
void update_curr_proc_erbsp_data_with_response_code(
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

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_response_code);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO, "[CURR PROC DATA UPDATED]:"
			"By DELETE LC RESP");

	p_rrc_uecc_llim_delete_lc_resp = 
		(rrc_uecc_llim_delete_lc_resp_t*)p_response_code;
	p_erab_list = &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
		erab_release_command_data.erab_release_list;
	p_erab_list->counter = RRC_NULL;

	p_erab_failed_list = &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
		erab_release_command_data.erab_failed_to_release_list;

	/* Set s1ap_Cause use to send S1AP */
	cause.t = T_s1ap_Cause_radioNetwork;
	cause.u.radioNetwork = s1ap_unspecified_2;

	for (resp_counter = 0;
			resp_counter < p_rrc_uecc_llim_delete_lc_resp->
			delete_drb_entity_resp_counter;
			resp_counter++)
	{
		for (erab_counter = 0; erab_counter < MAX_ERAB_COUNT;
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

						p_erab_failed_list->erab_item[p_erab_failed_list->counter].
							e_RAB_ID = p_ue_context->p_e_rab_list[erab_counter]->
							e_RAB_ID;

						p_erab_failed_list->erab_item[p_erab_failed_list->counter].cause =
							cause;

						p_erab_failed_list->erab_item[p_erab_failed_list->counter].
							error_code = p_rrc_uecc_llim_delete_lc_resp->
							delete_drb_entity_resp[resp_counter].response;

						p_erab_failed_list->counter++;
						break;
					}
				}
			}
		}
		if (erab_counter == MAX_ERAB_COUNT)
		{
			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_WARNING,
					"[UECC_EV_LLIM_DELETE_LC_RESP]:Invalid LC Id [%i]",
					p_rrc_uecc_llim_delete_lc_resp->
					delete_drb_entity_resp[resp_counter].lc_id);
		}

	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_and_send_erbsp_delete_lc_req
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
rrc_return_et uecc_llim_build_and_send_erbsp_delete_lc_req
(
 uecc_ue_context_t* p_uecc_ue_context
 )
{
	U8*             p_msg = PNULL;
	rrc_return_et   rrc_response = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	p_msg = uecc_alloc_intrl_msg(RRC_LLIM_MODULE_ID,
			UECC_LLIM_DELETE_LC_REQ,
			sizeof(rrc_uecc_llim_delete_lc_req_t));

	if (PNULL != p_msg)
	{
		uecc_llim_build_erbsp_delete_lc_req(p_uecc_ue_context,
				(rrc_uecc_llim_delete_lc_req_t*)(p_msg + RRC_API_HEADER_SIZE));

		/* Send the built API to LLIM  module*/
		rrc_send_message(p_msg, RRC_LLIM_MODULE_ID);
		rrc_response = RRC_SUCCESS;
	}
	else
	{
		RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
				p_uecc_ue_context->p_gb_context->facility_name,
				RRC_FATAL,
				"Build and Send [UECC_LLIM_DELETE_LC_REQ]:Memory Allocation Failed.");
		rrc_response = RRC_FAILURE;
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return rrc_response;
}
/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_erbsp_delete_lc_req
 *
 *   INPUT:        uecc_ue_context_t*             p_uecc_ue_context
 *                 rrc_uecc_llim_delete_lc_req_t* p_rrc_uecc_llim_erbsp_delete_lc_req
 *
 *   OUTPUT:       None
 *
 *   RETURNS:      None
 *
 *   DESCRIPTION:  Build internal message UECC_LLIM_DELETE_LC_REQ
 ******************************************************************************/
void uecc_llim_build_erbsp_delete_lc_req(
		uecc_ue_context_t*             p_uecc_ue_context,
		rrc_uecc_llim_delete_lc_req_t* p_rrc_uecc_llim_erbsp_delete_lc_req
		)
{
	erab_list_t*  p_erab_list = PNULL;
	rrc_counter_t drb_index = RRC_NULL;

	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_rrc_uecc_llim_erbsp_delete_lc_req);

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	/* Fill message */
	/* Fill message: transaction_id */
	p_rrc_uecc_llim_erbsp_delete_lc_req->transaction_id =
		uecc_ue_ctx_get_op_transction_id(p_uecc_ue_context);
	/* Fill message: ue_index */
	p_rrc_uecc_llim_erbsp_delete_lc_req->ue_index = p_uecc_ue_context->ue_index;

	/* spr_12532_fix_start */
	/* Fill message: cell index */
	p_rrc_uecc_llim_erbsp_delete_lc_req->cell_index = p_uecc_ue_context->cell_index;
	/* spr_12532_fix_stop */

	/* point to erab_release_list in erab_release_command_data */
	p_erab_list = &p_uecc_ue_context->p_curr_proc_data->u.erab_setup_request_data.
		erab_release_command_data.erab_release_list;

	/* Fill LC info fields */
	p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity_counter =
		p_erab_list->counter; 

	for(drb_index = 0;
			drb_index < p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity_counter;
			drb_index++)
	{

		p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity[drb_index].
			lc_id = p_uecc_ue_context->p_e_rab_list[p_erab_list->
			erab_item[drb_index].e_RAB_ID]->
			drb_config.logical_channel_identity;

		if (RRC_RLC_CONFIG_AM_CONFIG_PRESENT &
				p_uecc_ue_context->
				p_e_rab_list[p_erab_list->erab_item[drb_index].e_RAB_ID]->
				drb_config.rlc_config_enb.rlc_config.bitmask)
		{
			p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity[drb_index].
				rlc_mode = RLC_CONFIG_AM_MODE; 
		}
		if (RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT &
				p_uecc_ue_context->
				p_e_rab_list[p_erab_list->erab_item[drb_index].e_RAB_ID]->
				drb_config.rlc_config_enb.rlc_config.bitmask)
		{
			p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity[drb_index].
				rlc_mode = RLC_CONFIG_UM_BI_DIR_MODE;
		}
		if (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT &
				p_uecc_ue_context->
				p_e_rab_list[p_erab_list->erab_item[drb_index].e_RAB_ID]->
				drb_config.rlc_config_enb.rlc_config.bitmask)
		{
			p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity[drb_index].
				rlc_mode = RLC_CONFIG_UM_UL_UNI_DIR_MODE;
		}
		if (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT &
				p_uecc_ue_context->
				p_e_rab_list[p_erab_list->erab_item[drb_index].e_RAB_ID]->
				drb_config.rlc_config_enb.rlc_config.bitmask)
		{
			p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity[drb_index].
				rlc_mode = RLC_CONFIG_UM_DL_UNI_DIR_MODE;
		}

	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  :uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_init_handler 
 *                 
 * Input          :uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 *
 * Output         :None
 *
 * Returns        : None
 *
 * Description    :This action routine is called when FSM receives the
 *                 UECC_EV_STATE_INIT message.
 *
 ****************************************************************************/
void uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_init_handler(
		uecc_ue_context_t*  p_ue_context,
		void*               p_api
		)
{

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
   RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	/* Send Delete LC request to LLIM */
	if (RRC_SUCCESS !=
			uecc_llim_build_and_send_erbsp_delete_lc_req(p_ue_context))
	{
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  :uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_resp_handler 
 *
 * Input          :uecc_ue_context_t*    p_ue_context
 *                 void*                 p_api
 *
 * Output         : none
 * 
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_RECONFIGURE_UE_RESP from LLIM
 *
 ****************************************************************************/
void uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_resp_handler(
		uecc_ue_context_t*    p_ue_context,
		void*                 p_api
		)
{
	rrc_uecc_llim_delete_lc_resp_t*  p_rrc_uecc_llim_delete_lc_resp = PNULL;
	rrc_return_t   result = RRC_FAILURE;
	uecc_fsm_fail_handler_f const*  p_fail_actions_list =
		uecc_fsm_rm_erab_setup_fail_fatal_error_from_llim;

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
			"UECC_EV_LLIM_RECONFIGURE_UE_RESP");

	p_rrc_uecc_llim_delete_lc_resp = (rrc_uecc_llim_delete_lc_resp_t*)p_api;

	do{
		/* Check whether UECC_EV_LLIM_RECONFIGURE_UE_RESP
		 * received with success or failure or partial success */
		if (SUCCESS == p_rrc_uecc_llim_delete_lc_resp->
				response_code)
		{
			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
					"[UECC_EV_LLIM_DELETE_LC_RESP] Response Code =%i",
					p_rrc_uecc_llim_delete_lc_resp->response_code);

			/* Switch  state */
			UECC_FSM_SET_CHILD_STATE(p_ue_context,
					&uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing, PNULL);
			result = RRC_SUCCESS;
		}
		else if ((PARTIAL_SUCCESS == 
					p_rrc_uecc_llim_delete_lc_resp->response_code) ||
				(FAILURE == 
				 p_rrc_uecc_llim_delete_lc_resp->response_code) ||
				L3_FATAL == 
				p_rrc_uecc_llim_delete_lc_resp->response_code)
		{
			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
					"[UECC_EV_LLIM_DELETE_LC_RESP] Response Code =%i",
					p_rrc_uecc_llim_delete_lc_resp->response_code);

			/*update curr proc data with rrc_uecc_llim_delete_lc_resp */
			update_curr_proc_erbsp_data_with_response_code(p_ue_context,
					p_rrc_uecc_llim_delete_lc_resp);

			/* Put all E_RAB's in failed list from ERAB Setup List */
			uecc_fsm_rm_fail_action_mark_all_erab_failed(
					p_ue_context,
					RRC_UECC_ERAB_RELEASE_FAILED);

		}
	} while(0);

	/* Process failure */
	if (RRC_SUCCESS != result)
	{
		/* Start failure processing */
		uecc_fsm_start_failure_processing(
				p_ue_context,
				p_fail_actions_list);
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name
 * :uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_timer_expiry_handler
 *
 * Input          : uecc_ue_context_t*    p_ue_context
 *                  void*                 p_api
 *                  
 * Output         : None
 * 
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  RRC_TIMER_EXPIRY from LLIM
 *
 ****************************************************************************/
void uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_timer_expiry_handler(
		uecc_ue_context_t*    p_ue_context,
		void*                 p_api
		)
{
	U32      error_code = RRC_NULL;
	uecc_fsm_fail_handler_f const*  p_fail_actions_list =
		uecc_fsm_rm_erab_setup_fail_timer_expiry_while_wait_from_llim;
    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILEDALL,"Unused variable in FSM:%p",
			p_api);/*SPR 17777 +-*/

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	error_code |= RRC_MODULE_ID << 16;
	error_code |= RRC_TIMER_EXPIRY;

	/* Update erab release list to erab failed to release list */
	uecc_rm_erabsp_update_erab_release_list_with_error_code(
			p_ue_context,
			&error_code);

	/* Put all E_RAB's in failed list from ERAB Setup List */
	uecc_fsm_rm_fail_action_mark_all_erab_failed(
			p_ue_context,
			RRC_TIMER_EXPIRY);


	/* Start failure processing */
	uecc_fsm_start_failure_processing(
			p_ue_context,
			p_fail_actions_list);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  :uecc_rm_erabsp_update_erab_release_list_with_error_code
 *
 * Input          :uecc_ue_context_t* p_ue_context,
 *                 void*              p_error_code
 *
 * Output         :None
 *
 *
 * Returns        : None
 *
 * Description    :This is the update function for erab release with 
 *                 error code.
 ****************************************************************************/
void uecc_rm_erabsp_update_erab_release_list_with_error_code(
		uecc_ue_context_t* p_ue_context,
		void*              p_error_code
		)
{
	erab_list_t* p_erab_rel_list = PNULL;
	erab_list_t* p_erab_rel_failed_to_list = PNULL;

	U8 erab_counter = RRC_NULL;

	U32      error_code;
	s1ap_Cause    cause;

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_error_code);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	/*  set cause at timer expiry for ERAB Mgnt Procedures */
	/* SPR 15473 Fix Start */
	memset_wrapper(&cause, RRC_NULL, sizeof(s1ap_Cause));
	/* SPR 15473 Fix Stop */

	p_erab_rel_list = &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
		erab_release_command_data.erab_release_list;

	p_erab_rel_failed_to_list = &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
		erab_release_command_data.erab_failed_to_release_list;

	/* update by this s1ap_Cause */
	error_code = *(U32*)p_error_code;

    /* SPR 15473 Fix Start */
    uecc_llim_build_s1ap_cause(p_ue_context->p_gb_context ,
	&cause,
	(rrc_response_t)error_code);
    /* SPR 15473 Fix Stop */

	for (erab_counter = 0;
			erab_counter < p_erab_rel_list->counter;
			erab_counter++)
	{
		p_erab_rel_failed_to_list->erab_item[p_erab_rel_failed_to_list->counter].e_RAB_ID = 
			p_erab_rel_list->erab_item[erab_counter].e_RAB_ID;

		p_erab_rel_failed_to_list->erab_item[p_erab_rel_failed_to_list->counter].cause
			= cause;

		p_erab_rel_failed_to_list->erab_item[p_erab_rel_failed_to_list->counter++].error_code = 
			error_code;

	}

	/* Clear the Release List */
	memset_wrapper(p_erab_rel_list, RRC_NULL, sizeof(erab_list_t));

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name
 * :uecc_fsm_rm_llim_erapsp_del_reconfigure_ue_ongoing_reestablishment_handler
 *
 * Input          : uecc_ue_context_t*    p_ue_context,
 *                  void*                 p_api
 *
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ from LLIM
 *
 ****************************************************************************/
void uecc_fsm_rm_llim_erapsp_del_reconfigure_ue_ongoing_reestablishment_handler(
		uecc_ue_context_t*    p_ue_context,
		void*                 p_api
		)
{

	U32      error_code = RRC_NULL;
	uecc_fsm_fail_handler_f const*  p_fail_actions_list =
		uecc_fsm_rm_erab_setup_fail_fatal_error_from_llim;

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


	error_code |= RRC_MODULE_ID << 16;
	error_code |= RRC_REESTABLISHMENT_TRIGGERED;

	/* Update erab release list to erab failed to release list */
	uecc_rm_erabsp_update_erab_release_list_with_error_code(
			p_ue_context,
			&error_code);

	/* Put all E_RAB's in failed list from ERAB Setup List */
	uecc_fsm_rm_fail_action_mark_all_erab_failed(
			p_ue_context,
			RRC_REESTABLISHMENT_TRIGGERED);


	if (RRC_SUCCESS !=
			uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
				p_ue_context->p_gb_context, p_ue_context->crnti,
				p_ue_context->reestablishmentCause,
				p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
				"Fail to send the RRCConnectionReestablishmentReject to UE;Unused variable p_api=%p"
                ,p_api);/*SPR 17777 +-*/
	}


	/* Start failure processing */
	uecc_fsm_start_failure_processing(
			p_ue_context,
			p_fail_actions_list);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name
 * :uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_rlf_handler
 *
 * Input          : uecc_ue_context_t*    p_ue_context
 *                  void*                 p_api
 *
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_RADIO_LINK_FAILURE_IND from LLIM
 ****************************************************************************/
void uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_rlf_handler(
		uecc_ue_context_t*    p_ue_context,
		void*                 p_api
		)
{
	U32             error_code = RRC_NULL;
	uecc_fsm_fail_handler_f const*  p_fail_actions_list =
		uecc_fsm_rm_erab_setup_fail_fatal_error_from_llim;

	RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
			"UECC_EV_RADIO_LINK_FAILURE_IND");

	error_code |= RRC_MODULE_ID << 16;
	error_code |= RRC_RADIO_LINK_FAILURE_TRIGGERED;

	/* Update erab release list to erab failed to release list */
	uecc_rm_erabsp_update_erab_release_list_with_error_code(
			p_ue_context,
			&error_code);

	/* Put all E_RAB's in failed list from ERAB Setup List */
	uecc_fsm_rm_fail_action_mark_all_erab_failed(
			p_ue_context,
			RRC_RADIO_LINK_FAILURE_TRIGGERED);

	/* Start failure processing */
	uecc_fsm_start_failure_processing(
			p_ue_context,
			p_fail_actions_list);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name
 * :uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_ho_required_handler
 *
 * Input          : uecc_ue_context_t*    p_ue_context,
 *                  void*                 p_api
 *
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_HO_REQUIRED from LLIM
 *
 ****************************************************************************/
void uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_ho_required_handler(
		uecc_ue_context_t*    p_ue_context,
		void*                 p_api
		)
{

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
			"[UECC_EV_RRM_HO_REQUIRED]"
			"RRM Initiated E-RAB Release Procedure Ongoing"
			"Cancel the HO triggered by RRM");

	p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
	p_ue_context->ho_info.cause.u.radioNetwork = s1ap_interaction_with_other_procedure;
	if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_failure( p_ue_context))
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
				"Fail to send the handover failure indication to RRM;Unused variable p_api=%p"
                ,p_api);/*SPR 17777 +-*/
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
 * Function Name
 * :uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_ue_rel_req_handler
 *
 * Input          : uecc_ue_context_t*    p_ue_context
 *                  void*                 p_api
 *
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for the message
 *                  UECC_EV_UE_RELEASE_REQ
 ****************************************************************************/
void uecc_fsm_rm_llim_erabsp_del_reconfigure_ue_ongoing_ue_rel_req_handler(
		uecc_ue_context_t*    p_ue_context,
		void*                 p_api
		)
{
	rrc_s1ap_ue_context_release_command_t *p_ue_context_release_command = PNULL;
	U32    error_code = RRC_NULL;

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	p_ue_context_release_command = (rrc_s1ap_ue_context_release_command_t*)p_api;

	/* Store data in UE context for future processing */
	p_ue_context->share_data.uecc_rcr_data.u.cause =
		p_ue_context_release_command->cause;

	error_code |= RRC_MODULE_ID << 16;
	error_code |= RRC_INTERACTION_WITH_OTHER_PROCEDURE;

	/* Update erab release list to erab failed to release list */
	uecc_rm_erabsp_update_erab_release_list_with_error_code(
			p_ue_context,
			&error_code);

	/* Put all E_RAB's in failed list from ERAB Setup List */
	uecc_fsm_rm_fail_action_mark_all_erab_failed(
			p_ue_context,
			error_code);

	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context,
			UECC_ERAB_SETUP_GUARD_TIMER,
			&p_ue_context->timers_data);


	/* Send RRC_RRM_ERB_SETUP_CNF(FAILURE) to RRM */
	if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_setup_conf
			(p_ue_context))
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
				"FAILURE: Send RRC_RRM_ERB_SETUP_CNF (Fail)");
	}

	/* Send ASN.1 Encoded ERAB SETUP Response to MME */
	if(RRC_SUCCESS !=
			uecc_s1ap_build_and_send_erab_setup_response(p_ue_context))
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
				"[s1ap_E_RABSetup] delivery failure");
	}

	if (PNULL != p_ue_context->p_curr_proc_data)
	{
		/* Free the data for current procedure in the UE Context */
		rrc_mem_free(p_ue_context->p_curr_proc_data);
		p_ue_context->p_curr_proc_data = PNULL;
	}

	
        /* SPR 7106 Fix Start */
        /* Stop Procedure */
        uecc_fsm_finish_active_procedure(p_ue_context);
        /* SPR 7106 Fix Stop */

	uecc_event_queue_push_event(
			UECC_EV_UE_RELEASE_REQ_INT,
			p_ue_context,
			p_api);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erabsp_erab_release_fail_handler
 *
 * Input          : uecc_ue_context_t*    p_ue_context
 *                  U32                   err_code
 *                  uecc_fsm_fail_handler_f const* p_fail_actions_list
 *
 * Output         : None
 *
 * Returns        : None
 *
 * Description    : This is the handler function for erab release failure
 ****************************************************************************/
void uecc_fsm_rm_erabsp_erab_release_fail_handler(
		uecc_ue_context_t*    p_ue_context,
		U32                   err_code,
		uecc_fsm_fail_handler_f const*  p_fail_actions_list
		)
{
	U32      error_code = RRC_NULL;


	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	error_code |= RRC_MODULE_ID << 16;
	error_code |= err_code;

	/* Update erab release list to erab failed to release list */
	uecc_rm_erabsp_update_erab_release_list_with_error_code(
			p_ue_context,
			&error_code);

	/* Put all E_RAB's in failed list from ERAB Setup List */
	uecc_fsm_rm_fail_action_mark_all_erab_failed(
			p_ue_context,
			err_code);

	/* Start failure processing */
	uecc_fsm_start_failure_processing(
			p_ue_context,
			p_fail_actions_list);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rm_ue_erabsp_erab_del_reconfigure_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rm_ue_erabsp_erab_del_reconfigure_ongoing_init_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		)
{

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
	RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	/* build and send RRCConnectionReconfiguration */
	if (RRC_SUCCESS !=
			uecc_llim_build_and_send_erb_setup_erab_del_rrc_connection_reconfiguration(p_ue_context))
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL, 
				"FAILURE: UNABLE TO SEND RRCCONNECTIONRECONFIGURATION");

	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erabsp_erab_del_rrc_connection_reconfiguration_complete_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE from UE
 ****************************************************************************/
void uecc_fsm_rm_erabsp_erab_del_rrc_connection_reconfiguration_complete_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		)
{
	rrc_return_et rrc_response = RRC_FAILURE;

	/* ARP changes begin */
	erab_list_t*   p_erab_list = PNULL;
	U8 drb_index               = RRC_NULL;
	/* ARP changes end */

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
	RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_api);

	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context,
			UECC_ERAB_SETUP_GUARD_TIMER,
			&p_ue_context->timers_data);  

	/* build and send RRC_RRM_ERB_SETUP_CNF with success to RRM */
	rrc_response = uecc_rrm_build_and_send_erb_setup_conf(
			p_ue_context);

	if (RRC_SUCCESS == rrc_response)
	{
		/* Send ASN.1 Encoded ERAB SETUP Response to MME */
		if(RRC_SUCCESS !=
				uecc_s1ap_build_and_send_erab_setup_response(p_ue_context))
		{
			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
					"[s1ap_E_RABSetup] delivery failure");
		}

		/* Build and send the RRC_S1AP_E_RAB_RELEASE_INDICATION msg to S1AP */
		if (RRC_SUCCESS != uecc_s1ap_build_and_send_e_rab_release_indication(
					p_ue_context))
		{
			RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
					"[s1ap_E_RABSetup] Delivery failure E_RAB Release Indication");
		}
		p_ue_context->p_curr_proc_data->u.erab_setup_request_data.is_erab_del_ongoing = 
			RRC_FALSE;

		p_erab_list = &p_ue_context->p_curr_proc_data->u.
			erab_setup_request_data.erab_release_command_data.erab_release_list;

		for (drb_index =0;
				drb_index < p_erab_list->counter;
				drb_index++)
		{
			if (PNULL !=
					p_ue_context->p_e_rab_list
					[p_erab_list->erab_item[drb_index].e_RAB_ID])
			{
				rrc_mem_free(p_ue_context->p_e_rab_list
						[p_erab_list->erab_item[drb_index].e_RAB_ID]);
				p_ue_context->p_e_rab_list
					[p_erab_list->erab_item[drb_index].e_RAB_ID] = PNULL;
				/* Updates the ERAB Count in UE Context */
				p_ue_context->e_rab_list_count--;
				RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,
						"E-RAB ID %u context deleted from UE Context",
						p_erab_list->erab_item[drb_index].e_RAB_ID);
			}
		}
		/* ARP changes end */

		/* ICS STOP */
		uecc_fsm_finish_activity_failure(
				p_ue_context, 
				(void *)UECC_EV_S1AP_ERAB_SETUP_FAILURE_INT);
	}
	else
	{
		RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
				"[RRC_RRM_ERB_SETUP_CNF] delivery failure");

	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}
/* ARP CHANGES END */

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
    uecc_ue_context_t*  p_ue_context,
    rrm_cause_t*        p_rrm_cause
)
{
    U8 fail_list_count = 0;
    U8 succ_count = 0;
    U8 succ_list_counter = 0;

    uecc_rm_erab_setup_info_t*         p_erab_setup_list = 
        &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        erab_info_data;
    uecc_erab_failed_to_setup_list_t*  p_erab_fail_list = 
        &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        failed_erab_data;
	/*SPR 14184 Fix Start*/
    rrc_bool_et        is_drb_already_failed = RRC_FALSE;
    U8 fail_list_counter = 0;
	/*SPR 14184 Fix Stop*/

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rrm_cause);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    succ_count = p_erab_setup_list->count; 

    fail_list_count = p_erab_fail_list->count;

    for (succ_list_counter = 0; 
         succ_list_counter < succ_count; 
         succ_list_counter++)
    {
        is_drb_already_failed = RRC_FALSE;
	/*SPR 14184 Fix Start*/
        for (fail_list_counter = 0;
                fail_list_counter < fail_list_count;
                     fail_list_counter++)
        {
            if (p_erab_setup_list->erab_info[succ_list_counter].
                 erab_id == p_erab_fail_list->
                  erab_failed_to_setup_item[fail_list_counter].e_RAB_ID)
            {
               is_drb_already_failed = RRC_TRUE;
            } 
        }

        if ( (RRC_NO_ERROR == p_erab_setup_list->erab_info[succ_list_counter].
            status) && (RRC_FALSE == is_drb_already_failed) )
	/*SPR 14184 Fix Stop*/
        {
            /* Set status to error_code in E_RAB Setup List */
            p_erab_setup_list->erab_info[succ_list_counter].status = 0; 

            /* Copy E_RAB ID to Failed List */
            p_erab_fail_list->erab_failed_to_setup_item[fail_list_count]
                .e_RAB_ID = p_erab_setup_list->erab_info[succ_list_counter].
                                erab_id;

            /* Map Cause in Fail List */                     
            /*Coverity_CID_69682_FIX_START*/
            if(RRC_SUCCESS != uecc_s1ap_build_s1ap_cause(
                &p_erab_fail_list->erab_failed_to_setup_item
                [fail_list_count].cause,
                p_rrm_cause,
                p_ue_context))
            {
                RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                        "Error in uecc_s1ap_build_s1ap_cause");
            }
            /*Coverity_CID_69682_FIX_END*/

            /* Delete Failed ERAB from UE CONTEXT */
            uecc_fsm_rm_erab_setup_deallocate_erab(
                p_ue_context,           
                p_erab_setup_list->erab_info[succ_list_counter].erab_id);

            fail_list_count++;
        }
    }

    /* Copy Incremented fail list count */
    p_erab_fail_list->count = fail_list_count;
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* SPR 13448 Fix Stop */

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erabsp_sgnb_addition_ongoing_init_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rm_erabsp_sgnb_addition_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*Build and send SgNB Addition Request*/
    if (RRC_SUCCESS != 
            uecc_x2ap_build_and_send_sgnb_addtion_request(p_ue_context))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(
                RRC_ERROR,
                "Error while sending the SGNB_ADD_REQ"
                );

		uecc_fsm_start_failure_processing(
				p_ue_context,
				uecc_fsm_rm_erab_setup_fail_wrong_info_from_rrm);
    }
    else
    {
        /* increment the sk_counter needs to be incremented every time
         * a SgNBAddition Request is send.
         */
        p_ue_context->sk_counter_r15++;

        /* Stopping UECC_ERAB_SETUP_GUARD_TIMER as initiating an outside 
         * network call for SgNB Addtition. The timer will be reinitiated
         * once the Acknowledgement for the SgNB Addition is received. 
         */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_ERAB_SETUP_GUARD_TIMER,
                &p_ue_context->timers_data);  

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
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                    "UECC_DC_PREP_TIMER IS NOT Started");
            /* Stop Procedure */
            uecc_fsm_finish_active_procedure(p_ue_context);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erabsp_sgnb_addition_ongoing_ack_rcvd 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_X2AP_SGNB_ADDITION_REQ_ACK from X2AP
 ****************************************************************************/
void uecc_fsm_rm_erabsp_sgnb_addition_ongoing_ack_rcvd(
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
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(
                RRC_ERROR,
                "Error while handling SgNBAdditionRequestAck"
                " message. Start Local Error Handling");
        /*Logical_Error*/
        uecc_fsm_process_event(
                p_ue_context,
                UECC_EV_X2AP_ERROR_INDICATION,
                p_api);
        /*Logical_Error*/
    }
    else
    {
        /* Stopping UECC_DC_PREP_TIMER on reception of SgNBAddReqAck */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_DC_PREP_TIMER,
                &p_ue_context->timers_data);  

        /*
         * Starting UECC_ERAB_SETUP_GUARD_TIMER again as it was stopped
         * when SgNBAddition was initiated. This timer will be stopped
         * on ERAB Setup Procedure completion.
         */
        if (RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context, 
                    UECC_ERAB_SETUP_GUARD_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                    "UECC_ERAB_SETUP_GUARD_TIMER IS NOT Started");
            /* Stop Procedure */
            uecc_fsm_finish_active_procedure(p_ue_context);
        }
        else
        {
            /*update the outerResponse*/
            p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                erab_info_data.outerResponse = SUCCESS;

            /*Go for UE RRCConnectionReconfiguration procedure*/
            UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_rm_ue_erab_setup_reconfigure_ongoing,
                    PNULL);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erabsp_sgnb_addition_ongoing_timer_expiry 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the event
*                       UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rm_erabsp_sgnb_addition_ongoing_timer_expiry(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_TIMER_EXPIRY);
        /*Logical_Error*/
    /*release the x2ap context for this perticular UE*/
    uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);

    /*reset is_sgnb_add_reqd in p_ue_context*/
    p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
        /*Logical_Error*/

    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rm_erab_setup_fail_timer_expiry_while_wait_from_sgnb);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*Logical_Error*/
/****************************************************************************
 * Function Name  : uecc_fsm_rm_erabsp_sgnb_addition_ongoing_sgnb_add_req_reject 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_X2AP_SGNB_ADDITION_REQ_REJECT
 ****************************************************************************/
void uecc_fsm_rm_erabsp_sgnb_addition_ongoing_sgnb_add_req_reject(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    U8    fail_list_index =  RRC_NULL;
    rrm_cause_t     cause;
    U8 mn_mcg_present = RRC_FALSE;
    U8 erab_index = 0;
    U8 erab_id = 0;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*stop the DC_PREP_TIMER as the error ind has been received from the peer sgnb*/
    uecc_ue_timer_stop(p_ue_context->p_gb_context,
            UECC_DC_PREP_TIMER,
            &p_ue_context->timers_data);  

    /*release the x2ap context for this perticular UE*/
    uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);

    /*reset is_sgnb_add_reqd in p_ue_context*/
    p_ue_context->is_sgnb_add_reqd = RRC_FALSE;

    /*mark the RABs selected for SCG as failure*/

    uecc_erab_failed_to_setup_list_t*  p_erab_fail_list =
        &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                failed_erab_data);
    uecc_rm_erab_setup_info_t*  p_erab_setup_list =
        &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                erab_info_data);

    for(erab_index = RRC_NULL; erab_index < p_erab_setup_list->count; erab_index++)
    {
    	erab_id = p_erab_setup_list->erab_info[erab_index].erab_id;
        /*check if p_e_rab_list is not null*/
        if ( ( PNULL != p_ue_context->p_e_rab_list[erab_id] ) &&
                 ( p_ue_context->p_e_rab_list[erab_id]->m.DC_BearerTypePresent ))
        {
        	if(RRC_NO_ERROR == p_erab_setup_list->erab_info[erab_index].status)
        	{
            	p_erab_fail_list->erab_failed_to_setup_item[p_erab_fail_list->count].e_RAB_ID =
                	p_ue_context->p_e_rab_list[erab_id]->e_RAB_ID;

            	cause.type = T_s1ap_Cause_radioNetwork;
            	cause.value = s1ap_release_due_to_eutran_generated_reason;
            	p_erab_setup_list->erab_info[erab_index].status = RRC_X2AP_ERROR_IND;
            	/* MAP CAUSE VALUE HERE AND STORE IN FAILED LIST */
            	if ( RRC_SUCCESS != uecc_s1ap_build_s1ap_cause(
                        	&p_erab_fail_list->erab_failed_to_setup_item
                        	[p_erab_fail_list->count].cause, &cause,
                        	p_ue_context))
            	{
                	return;
            	}
            	/* Delete Failed ERAB from UE CONTEXT */
            	uecc_fsm_rm_erab_setup_deallocate_erab(
                		p_ue_context,           
                		p_ue_context->p_e_rab_list[erab_id]->e_RAB_ID);

            	p_erab_fail_list->erab_failed_to_setup_item[p_erab_fail_list->count].dc_bearer_type = 1;

            	p_erab_fail_list->count++;
            }
        }
        else if(PNULL != p_ue_context->p_e_rab_list[erab_id])
        {
            		mn_mcg_present = RRC_TRUE;
        }
    }

    for(fail_list_index = RRC_NULL; fail_list_index < p_erab_fail_list->count;
            fail_list_index++)
    {
        for(erab_index = RRC_NULL; erab_index < MAX_ERAB_COUNT; erab_index++)
        {
            if ( p_erab_setup_list->erab_info[erab_index].erab_id ==
                    p_erab_fail_list->erab_failed_to_setup_item[p_erab_fail_list->count].e_RAB_ID )
            {
                p_erab_setup_list->erab_info[erab_index].status = RRC_INTERNAL_ERROR;
                p_erab_setup_list->outerResponse = PARTIAL_SUCCESS;
            }
        }
    }



    /*check if all the rabs on the system are SCG Rabs*/
    if(RRC_FALSE == mn_mcg_present)
    {
        /*start the failure handling for the error ind received 
         * at time of erab setup - trigger erab setup failure 
         * towrads MME and RRM*/
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_rm_erab_setup_error_ind_fail_1_actions);
    }
    else
    {
        /*Start the UECC_ERAB_SETUP_GUARD_TIMER for establishing remaining RABs
         * which was stopped at time of sending SgNb Addition Req*/

        if(RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context,
                    UECC_ERAB_SETUP_GUARD_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                    "UECC_ERAB_SETUP_GUARD_TIMER IS NOT Started");
            /* Stop Procedure */
            uecc_fsm_finish_active_procedure(p_ue_context);
        }
        else
        {
            /*send the llim reconfig request to the lower layers for oter 
             * eRabs which are marked as MN Terminated MCG Bearer*/
            UECC_FSM_SET_CHILD_STATE(   
                    p_ue_context,
                    &uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing,
                    PNULL);
        }


    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}
/*Logical_Error*/

/*Error_Handling_changes*/
/****************************************************************************
 * Function Name  : uecc_fsm_rm_erabsp_sgnb_addition_ongoing_x2ap_error_ind 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_X2AP_ERROR_INDICATION
 ****************************************************************************/
/*Logical_Error*/
void uecc_fsm_rm_erabsp_sgnb_addition_ongoing_x2ap_error_ind(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{

    U8    fail_list_index =  RRC_NULL;
    rrm_cause_t     cause;
    U8 mn_mcg_present = RRC_FALSE;
    U8 erab_index = 0;
    U8 erab_id = 0;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*stop the DC_PREP_TIMER as the error ind has been received from the peer sgnb*/
    uecc_ue_timer_stop(p_ue_context->p_gb_context,
            UECC_DC_PREP_TIMER,
            &p_ue_context->timers_data);  

    /*release the x2ap context for this perticular UE*/
  	if(RRC_SUCCESS != uecc_fsm_ue_context_release_handler(p_ue_context))
  	{
  	  	RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
  	      	  	"uecc_fsm_ue_context_release_handler failure");
  	}
    /*reset is_sgnb_add_reqd in p_ue_context*/
    p_ue_context->is_sgnb_add_reqd = RRC_FALSE;

    /*mark the RABs selected for SCG as failure*/
    uecc_erab_failed_to_setup_list_t*  p_erab_fail_list =
            &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                    failed_erab_data);
    uecc_rm_erab_setup_info_t*  p_erab_setup_list =
            &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                    erab_info_data);

    for(erab_index = RRC_NULL; erab_index < p_erab_setup_list->count; erab_index++)
    {
        erab_id = p_erab_setup_list->erab_info[erab_index].erab_id;
        /*check if p_e_rab_list is not null*/
        if ( ( PNULL != p_ue_context->p_e_rab_list[erab_id] ) &&
                ( p_ue_context->p_e_rab_list[erab_id]->m.DC_BearerTypePresent ))
        {
            if(RRC_NO_ERROR == p_erab_setup_list->erab_info[erab_index].status)
            {
                p_erab_fail_list->erab_failed_to_setup_item[p_erab_fail_list->count].e_RAB_ID =
                    p_ue_context->p_e_rab_list[erab_id]->e_RAB_ID;

                cause.type = T_s1ap_Cause_radioNetwork;
                cause.value = s1ap_release_due_to_eutran_generated_reason;
                p_erab_setup_list->erab_info[erab_index].status = RRC_X2AP_ERROR_IND;
                /* MAP CAUSE VALUE HERE AND STORE IN FAILED LIST */
                if ( RRC_SUCCESS != uecc_s1ap_build_s1ap_cause(
                            &p_erab_fail_list->erab_failed_to_setup_item
                            [p_erab_fail_list->count].cause, &cause,
                            p_ue_context))
                {
                    return;
                }
                /* Delete Failed ERAB from UE CONTEXT */
                uecc_fsm_rm_erab_setup_deallocate_erab(
                        p_ue_context,           
                        p_ue_context->p_e_rab_list[erab_id]->e_RAB_ID);

                p_erab_fail_list->erab_failed_to_setup_item[p_erab_fail_list->count].dc_bearer_type = 1;

                p_erab_fail_list->count++;
            }
        }
        else if(PNULL != p_ue_context->p_e_rab_list[erab_id])
        {
            mn_mcg_present = RRC_TRUE;
        }
    }

    for(fail_list_index = RRC_NULL; fail_list_index < p_erab_fail_list->count;
                fail_list_index++)
    {
        for(erab_index = RRC_NULL; erab_index < MAX_ERAB_COUNT; erab_index++)
        {
            if ( p_erab_setup_list->erab_info[erab_index].erab_id ==
                    p_erab_fail_list->erab_failed_to_setup_item[p_erab_fail_list->count].e_RAB_ID )
            {
                p_erab_setup_list->erab_info[erab_index].status = RRC_INTERNAL_ERROR;
                p_erab_setup_list->outerResponse = PARTIAL_SUCCESS;
            }
        }
    }

	/*check if all the rabs on the system are SCG Rabs*/
    if(RRC_FALSE == mn_mcg_present)
    {
        /*start the failure handling for the error ind received 
         * at time of erab setup - trigger erab setup failure 
         * towrads MME and RRM*/
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_rm_erab_setup_error_ind_fail_1_actions);
    }
    else
    {
        /*Start the UECC_ERAB_SETUP_GUARD_TIMER for establishing remaining RABs
         * which was stopped at time of sending SgNb Addition Req*/

        if(RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context,
                    UECC_ERAB_SETUP_GUARD_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR,
                    "UECC_ERAB_SETUP_GUARD_TIMER IS NOT Started");
            /* Stop Procedure */
            uecc_fsm_finish_active_procedure(p_ue_context);
        }
        else
        {
            /*send the llim reconfig request to the lower layers for oter 
             * eRabs which are marked as MN Terminated MCG Bearer*/
            UECC_FSM_SET_CHILD_STATE(   
                    p_ue_context,
                    &uecc_fsm_rm_llim_erab_setup_reconfigure_ue_ongoing,
                    PNULL);
        }


    }
  
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*Logical_Error*/
/*Error_Handling_changes*/


/****************************************************************************
 * Function Name  : uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_rrc_conn_reestab_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_RRC_RE_ESTABLISH_REQ
 ****************************************************************************/
void uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_rrc_conn_reestab_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
        rrc_mem_get(len)))
    {
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL, 
            "Memory Allocation Failed to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
     
    /* storing re_establishment contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
            p_api,len);

    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RE_ESTABLISH_MSG;


    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_REESTABLISHMENT_TRIGGERED);

	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context, 
			UECC_DC_PREP_TIMER,
			&p_ue_context->timers_data);

    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rm_erab_setup_fail_rlf_reestablishment_wait_from_sgnb);
  
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_ho_required_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_RRM_HO_REQUIRED
 ****************************************************************************/
void uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_ho_required_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,"Event: UECC_EV_RRM_HO_REQUIRED received. "
            " Ignoring and waiting for TDCprep Timer Expiry.");
  
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_rlf_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_LLIM_RADIO_LINK_FAILURE_IND
 ****************************************************************************/
void uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_rlf_handler(
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
        RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_FATAL,
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

    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_RADIO_LINK_FAILURE_TRIGGERED);

	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context, 
			UECC_DC_PREP_TIMER,
			&p_ue_context->timers_data);

    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rm_erab_setup_fail_rlf_reestablishment_wait_from_sgnb);
 
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rm_erabsp_sgnb_addition_ongoing_ue_rel_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_UE_RELEASE_REQ
 ****************************************************************************/
void uecc_fsm_rm_erabsp_sgnb_addition_ongoing_ue_rel_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,"Event: UECC_EV_UE_RELEASE_REQ received. "
            " Ignoring and waiting for TDCprep Timer Expiry.");
  
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_s1ap_reset_msg_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_S1AP_RESET_MSG_IND
 ****************************************************************************/
void uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_s1ap_reset_msg_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,"Event: UECC_EV_S1AP_RESET_MSG_IND received. ");
  
    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_INTERNAL_ERROR);

	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context, 
			UECC_DC_PREP_TIMER,
			&p_ue_context->timers_data);

    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rm_erab_setup_fail_s1_reset_wait_from_sgnb);

	/* Send event to UE Main FSM */
	uecc_event_queue_push_event(
			UECC_EV_S1AP_RESET_MSG_IND_INT,
			p_ue_context,
			p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_x2ap_reset_msg_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_S1AP_RESET_MSG_IND
 ****************************************************************************/
void uecc_fsm_rm_ue_erabsp_sgnb_addition_ongoing_x2ap_reset_msg_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,"Event: UECC_EV_X2AP_RESET_MSG_IND received.");

    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_INTERNAL_ERROR);

	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context, 
			UECC_DC_PREP_TIMER,
			&p_ue_context->timers_data);

    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rm_erab_setup_fail_x2_reset_wait_from_sgnb);
  
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rm_llim_erabsp_sgnb_addition_ho_prep_timer_expiry_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_HO_PREP_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rm_llim_erabsp_sgnb_addition_ho_prep_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_INFO,"Event: UECC_EV_HO_PREP_TIMER_EXPIRY received. "
            " Ignoring and waiting for TDCprep Timer Expiry.");
  
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : 
 *              uecc_fsm_rm_erab_setup_fail_action_send_sgnb_reconfig_complete_guard_timer_exp
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*       - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Sends SgNBReconfigurationComplete with failure to release
 *                  the resources at SgNB
 *                  
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_fail_action_send_sgnb_reconfig_complete_guard_timer_exp(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
    {
        /* Fill SgNB cause for GUARD TIMER EXPIRY*/
        if (RRC_ONE != p_ue_context->m.sgnbCausePresent) 
        {
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_failure_in_the_radio_interface_procedure;

                p_ue_context->m.sgnbCausePresent = RRC_ONE;
        }
        /* Send SgNBReconfigurationComplete with failure */ 
        if (RRC_SUCCESS != 
               uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                   p_ue_context
                   ))
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR, 
                    "[%s],uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure"
                    " failed!", __FUNCTION__);
        }

        /* Reset SgNBAddRequired Flag in UE_CTX*/
        p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : 
 *              uecc_fsm_rm_erab_setup_fail_action_send_sgnb_reconfig_complete
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*       - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Sends SgNBReconfigurationComplete with failure to release
 *                  the resources at SgNB
 *                  
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_fail_action_send_sgnb_reconfig_complete(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
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
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_tDCprep_expiry;

                p_ue_context->m.sgnbCausePresent = RRC_ONE;
            }
        }
        /* Send SgNBReconfigurationComplete with failure */ 
        if (RRC_SUCCESS != 
               uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                   p_ue_context
                   ))
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR, 
                    "[%s],uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure"
                    " failed!", __FUNCTION__);
        }

        /* Reset SgNBAddRequired Flag in UE_CTX*/
        p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_fail_action_send_sgnb_release
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*       - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Sends SgNB Release to discard SgNBAdditionRequest send 
 *                  earlier.
 *                  
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_fail_action_send_sgnb_release(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
    {
        p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->sgnb_cause.u.radioNetwork = x2ap_tDCprep_expiry;

        /* Send SgNB RELEASE to SgNB */ 
        if (RRC_SUCCESS != 
                build_and_send_sgnb_release_request(p_ue_context))
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR, 
                    "[%s],Unable to process SgNB Release Request", __FUNCTION__);
        }
        
        /* Reset SgNBAddRequired Flag in UE_CTX*/
        p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_fail_rlf_reestab_action_send_sgnb_release
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*       - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Sends SgNB Release to discard SgNBAdditionRequest send 
 *                  earlier.
 *                  
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_fail_rlf_reestab_action_send_sgnb_release(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
    {
        /* Fill SgNB cause for RLF/Reestablishment Scenario*/
        {
            if( PNULL != p_ue_context->p_curr_proc_data &&
                    RRC_RLF_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
            {
                /* For RLF Scenario */
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_radio_connection_with_UE_lost;
            }
            else if( PNULL != p_ue_context->p_curr_proc_data &&
                    RRC_RE_ESTABLISH_MSG == p_ue_context->p_curr_proc_data->msg_bfr_type )
            {
                /* For Reestablishment Scenario */
                p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                p_ue_context->sgnb_cause.u.radioNetwork = x2ap_procedure_cancelled;
            }
        }

        /* Send SgNB RELEASE to SgNB */ 
        if (RRC_SUCCESS != 
                build_and_send_sgnb_release_request(p_ue_context))
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR, 
                    "[%s],Unable to process SgNB Release Request", __FUNCTION__);
        }
        
        /* Reset SgNBAddRequired Flag in UE_CTX*/
        p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_scg_failure_information_nr_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_SCG_FAILURE_INFORMATION_NR
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_scg_failure_information_nr_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

	uecc_fsm_start_failure_processing(
			p_ue_context,
			uecc_fsm_rm_erab_setup_fail_wait_for_ue_reconfig);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_erab_setup_fail_action_s1_reset_send_sgnb_release
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*       - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Sends SgNB Release to discard SgNBAdditionRequest send 
 *                  earlier.
 *                  
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_fail_action_s1_reset_send_sgnb_release(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
    {
        p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->sgnb_cause.u.radioNetwork = x2ap_unspecified_2;

        /* Send SgNB RELEASE to SgNB */ 
        if (RRC_SUCCESS != 
                build_and_send_sgnb_release_request(p_ue_context))
        {
            RRC_UECC_ERAB_SETUP_RM_FSM_TRACE(RRC_ERROR, 
                    "[%s],Unable to process SgNB Release Request", __FUNCTION__);
        }
        
        /* Reset SgNBAddRequired Flag in UE_CTX*/
        p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*NR_DC Code Change Stop*/
#endif
