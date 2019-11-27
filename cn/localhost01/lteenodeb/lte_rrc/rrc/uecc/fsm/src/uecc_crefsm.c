/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2010 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 ****************************************************************************
 *
 *  File Description : UE Connection Re-establishment FSM functions implementation
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
#include "uecc_crefsm.h"
#include "uecc_rrm.h"
#include "uecc_llim.h"
#include "uecc_s1ap.h"
#include "uecc_defaults.h"
#include "uecc_mainfsm.h"
#include "uecc_fsm_engine.h"
#include "rrc_intrl_api.h"
#include "uecc_x2ap.h"

#include "uecc_trg_hofsm.h"
#include "rrc_shared_stats.h"
#include "rrc_rrm_il_composer.h"
#include "rrc_msg_mgmt.h"

/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_CRE_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,\
        p_ue_context->ue_index,\
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
        ##__VA_ARGS__)


/**********************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static void uecc_fsm_ue_cre_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_cre_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_cre_procedure_finish_fail(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/*SPR_17121_START*/
/*Code Removed*/
/*SPR_17121_END*/

/* State UECC_RCRE_LLIM_CHANGE_CRNTI_ONGOING */
static void uecc_fsm_cre_llim_change_crnti_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_cre_llim_change_crnti_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_cre_llim_change_crnti_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/* State UECC_CRE_LLIM_SUSPEND_UE_ENTITY_ONGOING */
static void uecc_fsm_cre_llim_suspend_ue_entity_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_suspend_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
/*SPR_18186_START*/
static void uecc_fsm_cre_llim_reset_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
/*SPR_18186_END*/
static void uecc_fsm_cre_llim_suspend_ue_entity_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/* State UECC_CRE_LLIM_REESTABLISH_SRB1_ONGOING */
static void uecc_fsm_cre_llim_re_establish_srb1_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_re_establish_srb1_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_re_establish_srb1_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/* State UECC_CRE_LLIM_RECONFIGURE_UE_ENTITY_ONGOING */
static void uecc_fsm_cre_llim_reconfig_ue_entity_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_reconfigure_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_reconfig_ue_entity_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/* State UECC_CRE_LLIM_RESUME_SRB1_ONGOING */
static void uecc_fsm_cre_llim_resume_srb1_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_resume_srb1_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_resume_srb1_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/* State UECC_CRE_W_FOR_REESTABLISH_COMPLETE */
static void uecc_fsm_cre_w_for_re_establish_complete_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_rrc_connection_re_establish_complete_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_w_for_re_establish_complete_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/*SPR 15738 Fix Start*/
static void uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_cnf_msg_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
/*SPR 15738 Fix Stop*/

/* State UECC_CRE_LLIM_REESTABLISH_SRB2_DRBs_ONGOING */
static void uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_re_establish_srb2_drbs_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static 
void uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/*CA_RE_ESTAB_START*/
static void uecc_fsm_cre_llim_scell_lower_layer_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_scell_lower_layer_reconfigure_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
/*CA_RE_ESTAB_STOP*/

/* State UECC_CRE_LLIM_RESUME_UPLINK_SRB2_DRBs_ONGOING */
static void uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static
void uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/* State UECC_CRE_W_FOR_RECONFIG_COMPLETE */
static void uecc_fsm_cre_w_for_reconfig_complete_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_rrc_connection_reconfiguration_complete_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static
void uecc_fsm_cre_w_for_reconfig_complete_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/* State UECC_CRE_LLIM_RESUME_DOWNLINK_SRB2_DRBs_ONGOING */
static void uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static
void uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/* Local Functions prototype */
static 
rrc_return_et uecc_fsm_cre_process_rrc_connection_reestablishment_req_msg(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);
/*SPR_17121_START*/
/*Code Removed*/
/*SPR_17121_END*/

static void uecc_fsm_cre_srb1_cell_delelte_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);
static void uecc_fsm_cre_srb2_drbs_cell_delelte_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);

static void uecc_fsm_cre_srb1_reset_msg_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);
static void uecc_fsm_cre_srb2_drbs_reset_msg_ind_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);

static void uecc_fsm_cre_srb1_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);
static void uecc_fsm_cre_srb1_radio_link_failure_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);

static void uecc_fsm_cre_srb2_drbs_radio_link_failure_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);
static void uecc_fsm_cre_srb2_drbs_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);
/* Failure processing functions prototype */
static void
uecc_fsm_cre_fail_action_build_and_send_rrc_connection_reestablishment_reject(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
);

static void
uecc_fsm_cre_fail_action1_build_and_send_rrc_connection_reestablishment_reject(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
);

static void uecc_fsm_cre_fail_action_last(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
);


static void uecc_fsm_cre_fail_action_post_event2(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
);

static void uecc_fsm_cre_fail_action_post_event4(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
);

static void uecc_fsm_cre_fail_action_build_and_send_reestab_ind(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
);

/* BUG 11253 Fix Start */
static void uecc_fsm_cre_srb1_ue_conn_release_ind_handler(
    uecc_ue_context_t*  p_ue_context,
    void* p_api
);

static void uecc_fsm_cre_srb2_drbs_ue_conn_release_ind_handler(
    uecc_ue_context_t*  p_ue_context,
    void* p_api
);
/* BUG 11253 Fix Stop */


/****************************************************************************
 Private Variables
****************************************************************************/
/*SPR_17121_START*/
/*Code Removed*/
/*SPR_17121_END*/
static
uecc_fsm_handler_record_t uecc_fsm_cre_llim_change_crnti_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
            uecc_fsm_cre_llim_change_crnti_ongoing_init_handler   },
    { { UECC_EV_LLIM_CHANGE_CRNTI_RESP   },
            uecc_fsm_cre_llim_change_crnti_resp_handler                  },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ   },
            uecc_fsm_cre_srb1_cell_delelte_req_handler                  },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_cre_srb1_reset_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ                 },
            uecc_fsm_cre_srb1_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
            uecc_fsm_cre_srb1_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY            },
            uecc_fsm_cre_llim_change_crnti_ongoing_timer_expiry_handler },
     /* BUG 11253 Fix Start */
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
            uecc_fsm_cre_srb1_ue_conn_release_ind_handler },
     /* BUG 11253 Fix Stop */
    { { UECC_LAST_ID                    },  PNULL                       }
};

static 
uecc_fsm_handler_record_t uecc_fsm_cre_llim_suspend_ue_entity_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
            uecc_fsm_cre_llim_suspend_ue_entity_ongoing_init_handler   },
    { { UECC_EV_LLIM_SUSPEND_UE_ENTITY_RESP   },
            uecc_fsm_cre_llim_suspend_ue_entity_resp_handler           },
    /*SPR_18186_START*/
    { { UECC_EV_LLIM_RESET_UE_ENTITY_RESP },
            uecc_fsm_cre_llim_reset_ue_entity_resp_handler           },
    /*SPR_18186_END*/
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ   },
            uecc_fsm_cre_srb1_cell_delelte_req_handler                  },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_cre_srb1_reset_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ                 },
            uecc_fsm_cre_srb1_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
            uecc_fsm_cre_srb1_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY            },
            uecc_fsm_cre_llim_suspend_ue_entity_ongoing_timer_expiry_handler },
      /* BUG 11253 Fix Start */
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
           uecc_fsm_cre_srb1_ue_conn_release_ind_handler            },
     /* BUG 11253 Fix Stop */
    { { UECC_LAST_ID                    },  PNULL                       }
};

static 
uecc_fsm_handler_record_t uecc_fsm_cre_llim_re_establish_srb1_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
            uecc_fsm_cre_llim_re_establish_srb1_ongoing_init_handler   },
    { { UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP   },
            uecc_fsm_cre_llim_re_establish_srb1_ue_entity_resp_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ   },
            uecc_fsm_cre_srb1_cell_delelte_req_handler                  },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_cre_srb1_reset_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ                 },
            uecc_fsm_cre_srb1_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
            uecc_fsm_cre_srb1_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY            },
            uecc_fsm_cre_llim_re_establish_srb1_ongoing_timer_expiry_handler },
      /* BUG 11253 Fix Start */
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
            uecc_fsm_cre_srb1_ue_conn_release_ind_handler  },
     /* BUG 11253 Fix Stop */
    { { UECC_LAST_ID                    },  PNULL                       }
};

static 
uecc_fsm_handler_record_t uecc_fsm_cre_llim_reconfig_ue_entity_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
            uecc_fsm_cre_llim_reconfig_ue_entity_ongoing_init_handler   },
    { { UECC_EV_LLIM_RECONFIGURE_UE_ENTITY_RESP   },
            uecc_fsm_cre_llim_reconfigure_ue_entity_resp_handler      },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ   },
            uecc_fsm_cre_srb1_cell_delelte_req_handler                  },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_cre_srb1_reset_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ                 },
            uecc_fsm_cre_srb1_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
            uecc_fsm_cre_srb1_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY            },
            uecc_fsm_cre_llim_reconfig_ue_entity_ongoing_timer_expiry_handler },
     /* BUG 11253 Fix Start */
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
             uecc_fsm_cre_srb1_ue_conn_release_ind_handler },
     /* BUG 11253 Fix Stop */
    { { UECC_LAST_ID                    },  PNULL                       }
};

static 
uecc_fsm_handler_record_t uecc_fsm_cre_llim_resume_srb1_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
            uecc_fsm_cre_llim_resume_srb1_ongoing_init_handler   },
    { { UECC_EV_LLIM_RESUME_UE_ENTITY_RESP   },
            uecc_fsm_cre_llim_resume_srb1_ue_entity_resp_handler                  }, 
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ   },
            uecc_fsm_cre_srb1_cell_delelte_req_handler                  },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_cre_srb1_reset_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ                 },
            uecc_fsm_cre_srb1_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
            uecc_fsm_cre_srb1_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY            },
            uecc_fsm_cre_llim_resume_srb1_ongoing_timer_expiry_handler },
     /* BUG 11253 Fix Start */
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
            uecc_fsm_cre_srb1_ue_conn_release_ind_handler              },
     /* BUG 11253 Fix Stop */
    { { UECC_LAST_ID                    },  PNULL                       }
};

static 
uecc_fsm_handler_record_t uecc_fsm_cre_w_for_re_establish_complete_mht[] =
{
    { { UECC_EV_STATE_INIT              },
            uecc_fsm_cre_w_for_re_establish_complete_init_handler   },
    { { UECC_EV_RRC_CONNECTION_REESTABLISHMENT_COMPLETE   },
            uecc_fsm_cre_rrc_connection_re_establish_complete_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ   },
            uecc_fsm_cre_srb2_drbs_cell_delelte_req_handler   },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_cre_srb2_drbs_reset_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ                 },
            uecc_fsm_cre_srb2_drbs_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
            uecc_fsm_cre_srb1_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY            },
            uecc_fsm_cre_w_for_re_establish_complete_timer_expiry_handler },
     /* BUG 11253 Fix Start */
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
            uecc_fsm_cre_srb2_drbs_ue_conn_release_ind_handler },                                                           
            /* BUG 11253 Fix Stop */
    { { UECC_LAST_ID                    },  PNULL                       }
};

/*SPR_17121_START*/
/*Code Removed*/
/*SPR_17121_END*/

static uecc_fsm_activity_state_t uecc_fsm_cre_llim_change_crnti_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_RCRE_LLIM_CHANGE_CRNTI_ONGOING",
   /* .s_id           =  */ (S8*)"CRE_LLIM_CHANGE_CRNTI_UE_ENTITY",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_cre_llim_change_crnti_ongoing_mht
};
static uecc_fsm_activity_state_t uecc_fsm_cre_llim_suspend_ue_entity_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_CRE_LLIM_SUSPEND_UE_ENTITY_ONGOING",
   /* .s_id           =  */ (S8*)"CRE_LLIM_SUSPEND_UE_ENTITY",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_cre_llim_suspend_ue_entity_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_cre_llim_re_establish_srb1_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_CRE_LLIM_REESTABLISH_SRB1_ONGOING",
   /* .s_id           =  */ (S8*)"CRE_LLIM_REESTABLISH_SRB1",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_cre_llim_re_establish_srb1_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_cre_llim_reconfig_ue_entity_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_CRE_LLIM_RECONFIGURE_UE_ENTITY_ONGOING",
   /* .s_id           =  */ (S8*)"CRE_LLIM_RECONFIGURE_UE_ENTITY",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_cre_llim_reconfig_ue_entity_ongoing_mht
};
static uecc_fsm_activity_state_t uecc_fsm_cre_llim_resume_srb1_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_CRE_LLIM_RESUME_SRB1_ONGOING",
   /* .s_id           =  */ (S8*)"CRE_LLIM_RESUME_SRB1",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_cre_llim_resume_srb1_ongoing_mht
};
static uecc_fsm_activity_state_t uecc_fsm_cre_w_for_re_establish_complete =
{
   /* .s_name         =  */ (S8*)"UECC_CRE_W_FOR_REESTABLISH_COMPLETE",
   /* .s_id           =  */ (S8*)"CRE_W_FOR_REESTABLISH_COMPLETE",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_cre_w_for_re_establish_complete_mht
};

/* Activity */
static uecc_fsm_activity_t uecc_fsm_ue_cre_activity =
{
  /*  .s_name             = */(S8*)"RRC Connection Reestablishment",
   /* .s_id               = */(S8*)"UE_CRE_A",
/*SPR_17121_START*/
   /* .p_first_state      = */&uecc_fsm_cre_llim_suspend_ue_entity_ongoing
/*SPR_17121_END*/
};
/*CA_RE_ESTAB_START*/
static uecc_fsm_handler_record_t 
uecc_fsm_cre_llim_scell_lower_layer_reconfigure_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
      uecc_fsm_cre_llim_scell_lower_layer_reconfigure_ongoing_init_handler   },
    { { UECC_EV_LLIM_RECONFIGURE_UE_ENTITY_RESP   },
      uecc_fsm_cre_llim_scell_lower_layer_reconfigure_ue_entity_resp_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ   },
            uecc_fsm_cre_srb2_drbs_cell_delelte_req_handler                  },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_cre_srb2_drbs_reset_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ                 },
            uecc_fsm_cre_srb2_drbs_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
            uecc_fsm_cre_srb2_drbs_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY            },
      uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing_timer_expiry_handler },
      /* BUG 11253 Fix Start */
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
            uecc_fsm_cre_srb2_drbs_ue_conn_release_ind_handler },      
     /* BUG 11253 Fix Stop */
   { { UECC_LAST_ID                    },  PNULL                       }
};
/*CA_RE_ESTAB_STOP*/

/*SPR 15738 Fix Start*/
static uecc_fsm_handler_record_t
uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing_mht[]=
{
    { { UECC_EV_STATE_INIT              },
            uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing_init_handler   },
    { { UECC_EV_LLIM_REESTAB_UE_ENTITY_COMP_CNF   },
            uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_cnf_msg_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ   },
            uecc_fsm_cre_srb2_drbs_cell_delelte_req_handler                  },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_cre_srb2_drbs_reset_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ                 },
            uecc_fsm_cre_srb2_drbs_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
            uecc_fsm_cre_srb2_drbs_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY            },
      	    uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing_timer_expiry_handler },
    { { UECC_LAST_ID                    },  PNULL                       }

};
/*SPR 15738 Fix Stop*/
static uecc_fsm_handler_record_t 
uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
      uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing_init_handler   },
    { { UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP   },
      uecc_fsm_cre_llim_re_establish_srb2_drbs_ue_entity_resp_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ   },
            uecc_fsm_cre_srb2_drbs_cell_delelte_req_handler                  },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_cre_srb2_drbs_reset_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ                 },
            uecc_fsm_cre_srb2_drbs_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
            uecc_fsm_cre_srb2_drbs_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY            },
      uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing_timer_expiry_handler },
       /* BUG 11253 Fix Start */
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
            uecc_fsm_cre_srb2_drbs_ue_conn_release_ind_handler },      
       /* BUG 11253 Fix Stop */
  { { UECC_LAST_ID                    },  PNULL                       }
};

static uecc_fsm_handler_record_t 
uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
            uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ongoing_init_handler   },
    { { UECC_EV_LLIM_RESUME_UE_ENTITY_RESP   },
            uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ue_entity_resp_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ   },
            uecc_fsm_cre_srb2_drbs_cell_delelte_req_handler                  },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_cre_srb2_drbs_reset_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ                 },
            uecc_fsm_cre_srb2_drbs_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
            uecc_fsm_cre_srb2_drbs_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY            },
      uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ongoing_timer_expiry_handler },
        /* BUG 11253 Fix Start */
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
            uecc_fsm_cre_srb2_drbs_ue_conn_release_ind_handler }, 
     /* BUG 11253 Fix Stop */
 { { UECC_LAST_ID                    },  PNULL                       }
};

static
uecc_fsm_handler_record_t uecc_fsm_cre_w_for_reconfig_complete_mht[] =
{
    { { UECC_EV_STATE_INIT              },
            uecc_fsm_cre_w_for_reconfig_complete_init_handler   },
    { { UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE   },
            uecc_fsm_cre_rrc_connection_reconfiguration_complete_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ   },
            uecc_fsm_cre_srb2_drbs_cell_delelte_req_handler                  },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_cre_srb2_drbs_reset_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ                 },
            uecc_fsm_cre_srb2_drbs_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
            uecc_fsm_cre_srb2_drbs_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY            },
            uecc_fsm_cre_w_for_reconfig_complete_timer_expiry_handler },
         /* BUG 11253 Fix Start */
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
            uecc_fsm_cre_srb2_drbs_ue_conn_release_ind_handler }, 
     /* BUG 11253 Fix Stop */
{ { UECC_LAST_ID                    },  PNULL                       }
};

static uecc_fsm_handler_record_t
uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT              },
            uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ongoing_init_handler   },
    { { UECC_EV_LLIM_RESUME_UE_ENTITY_RESP   },
            uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ue_entity_resp_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ   },
            uecc_fsm_cre_srb2_drbs_cell_delelte_req_handler                  },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_cre_srb2_drbs_reset_msg_ind_handler    },
    { { UECC_EV_UE_RELEASE_REQ                 },
            uecc_fsm_cre_srb2_drbs_ue_release_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
            uecc_fsm_cre_srb2_drbs_radio_link_failure_handler },
    { { UECC_EV_TIMER_EXPIRY            },
   uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ongoing_timer_expiry_handler },
         /* BUG 11253 Fix Start */
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },

        uecc_fsm_cre_srb2_drbs_ue_conn_release_ind_handler },      
     /* BUG 11253 Fix Stop */
{ { UECC_LAST_ID                    },  PNULL                       }
};

/* Activity states */
/*CA_RE_ESTAB_START*/
static 
uecc_fsm_activity_state_t uecc_fsm_cre_llim_scell_lower_layer_reconfigure_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_CRE_LLIM_SCELL_LOWER_LAYER_RECONFIGURE_ONGOING",
   /* .s_id           =  */ (S8*)"CRE_LLIM_SCELL_LOWER_LAYER_RECONFIGURE",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_cre_llim_scell_lower_layer_reconfigure_ongoing_mht
};
/*CA_RE_ESTAB_STOP*/

static 
uecc_fsm_activity_state_t uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_CRE_LLIM_REESTABLISH_SRB2_DRBs_ONGOING",
   /* .s_id           =  */ (S8*)"CRE_LLIM_REESTABLISH_SRB2_DRBs",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing_mht
};
/*SPR 15738 Fix Start*/
static
uecc_fsm_activity_state_t uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing =
{

   /* .s_name         =  */ (S8*)"UECC_CRE_LLIM_RLC_REESTAB_COMP_ONGOING",
   /* .s_id           =  */ (S8*)"CRE_LLIM_RLC_REESTAB_COMP_ONGOING",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing_mht 
};
/*SPR 15738 Fix Stop*/
static 
uecc_fsm_activity_state_t uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_CRE_LLIM_RESUME_UPLINK_SRB2_DRBs_ONGOING",
   /* .s_id           =  */ (S8*)"CRE_LLIM_RESUME_UPLINK_SRB2_DRBs",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_cre_w_for_reconfig_complete =
{
   /* .s_name         =  */ (S8*)"UECC_CRE_W_FOR_RECONFIG_COMPLETE",
   /* .s_id           =  */ (S8*)"CRE_W_FOR_RECONFIG_COMPLETE",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_cre_w_for_reconfig_complete_mht
};

static 
uecc_fsm_activity_state_t uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_CRE_LLIM_RESUME_DOWNLINK_SRB2_DRBs_ONGOING",
   /* .s_id           =  */ (S8*)"CRE_LLIM_RESUME_DOWNLINK_SRB2_DRBs",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ongoing_mht
};

/* Activity */
static 
uecc_fsm_activity_t uecc_fsm_ue_cre_rrc_connection_reconfiguration_activity =
{
  /*  .s_name             = */
    (S8*)"RRC Connection Reconfiguration After RRC Connection Re-establishment",
   /* .s_id               = */(S8*)"UE_CRE_RECONFIG_A",
   /* .p_first_state      = */&uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing
};

static uecc_fsm_activity_t*
                    uecc_fsm_ue_cre_procedure_activity_list[] =
{
    &uecc_fsm_ue_cre_activity,
    &uecc_fsm_ue_cre_rrc_connection_reconfiguration_activity,
    PNULL
};

/* Procedure */
uecc_fsm_procedure_t uecc_fsm_ue_connection_re_establishment_procedure =
{
    /*.s_name             = */(S8*)"RRC Connection Reestablishment Procedure",
    /*.s_id               = */(S8*)"CRE_P",
    /*.constructor_f      = */uecc_fsm_ue_cre_procedure_constructor,
    /*.finish_success_f   = */uecc_fsm_ue_cre_procedure_finish_success,
    /*.finish_fail_f      = */uecc_fsm_ue_cre_procedure_finish_fail,
    /*.activity_list      = */uecc_fsm_ue_cre_procedure_activity_list,
    /*.p_mht              = */PNULL
};

/* Fail actions */
static const uecc_fsm_fail_handler_f uecc_fsm_cre_fail_type1_actions[] =
{
    uecc_fsm_cre_fail_action_build_and_send_reestab_ind,
    uecc_fsm_cre_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_cre_fail_type2_actions[] =
{
  uecc_fsm_cre_fail_action_build_and_send_rrc_connection_reestablishment_reject,
    uecc_fsm_cre_fail_action_last,
    PNULL
};

/* CSC Cell delete req during srb1 re-establishment */
static const uecc_fsm_fail_handler_f uecc_fsm_cre_fail_type4_actions[] =
{
  uecc_fsm_cre_fail_action_build_and_send_rrc_connection_reestablishment_reject,
    uecc_fsm_cre_fail_action_post_event2,
    PNULL
};

/* S1AP-RESET IND During SRB2 and DRBs re-establishment on going */
static const uecc_fsm_fail_handler_f uecc_fsm_cre_fail_type6_actions[] =
{
    uecc_fsm_cre_fail_action_build_and_send_reestab_ind,
    uecc_fsm_cre_fail_action_post_event4,
    PNULL
};

/* CSC Cell delete req during srb2 and drbs re-establishment */
static const uecc_fsm_fail_handler_f uecc_fsm_cre_fail_type7_actions[] =
{
    uecc_fsm_cre_fail_action_build_and_send_reestab_ind,
    uecc_fsm_cre_fail_action_post_event2,
    PNULL
};

/* S1AP-RESET IND During SRB1 re-establishment on going */
static const uecc_fsm_fail_handler_f uecc_fsm_cre_fail_type8_actions[] =
{
  uecc_fsm_cre_fail_action_build_and_send_rrc_connection_reestablishment_reject,
    uecc_fsm_cre_fail_action_post_event4,
    PNULL
};
/* Send when UE context Release Command received during SRB1 re-establishment
 * ongoing. */
static const uecc_fsm_fail_handler_f uecc_fsm_cre_fail_type9_actions[] =
{
  uecc_fsm_cre_fail_action_build_and_send_rrc_connection_reestablishment_reject,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_cre_fail_type10_actions[] =
{
  uecc_fsm_cre_fail_action1_build_and_send_rrc_connection_reestablishment_reject,
    uecc_fsm_cre_fail_action_last,
    PNULL
};
/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/
/****************************************************************************
 * Function Name  : uecc_fsm_ue_cre_procedure_constructor 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for UE Connection Re-establishment procedure
 ****************************************************************************/
void uecc_fsm_ue_cre_procedure_constructor(
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
            "Ue Connection Reestablishment:Transaction Id updated");
#else
    uecc_ue_ctx_set_op_transction_id(p_ue_context,
                    uecc_ue_ctx_get_next_transction_id(p_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/
    /* SPR 17007 Fix Start */
    /* Deallocate the memory allocated during last Re-establishment Procedure */
    /* if UE Information Procedure has not been triggered */
    if (PNULL != p_ue_context->p_reestab_ue_identity)
    {
        rrc_mem_free(p_ue_context->p_reestab_ue_identity);
        p_ue_context->p_reestab_ue_identity = PNULL;
    }
    /* SPR 17007 Fix Stop */

    /* Allocate memory for the reestab_ue_identity to be used during
     * Ue Connection Reestablishment Procedure*/
     p_ue_context->p_reestab_ue_identity =
         (reestab_ue_identity_t*)rrc_mem_get(sizeof(reestab_ue_identity_t));

    if (PNULL == p_ue_context->p_reestab_ue_identity)
    {
        RRC_UECC_CRE_FSM_TRACE(RRC_FATAL,
            "[UE_CONN_REESTABLISH] "
            "Memory Alocation Failed For(ReestablishUE-Identity)");
    }
    else
    {
        memset_wrapper(p_ue_context->p_reestab_ue_identity, RRC_NULL,
        sizeof(reestab_ue_identity_t));

    do {
        /* Start timer*/
        if (RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context, 
            UECC_UE_CRE_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index))
        {
                    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                        "[UE_CONN_REESTABLISH] "
                        "Fail to Start Timer");
            break;
        }
        uecc_fsm_cre_process_rrc_connection_reestablishment_req_msg
                                                  (p_ue_context, p_api);
        result = RRC_SUCCESS;
    } while(0); 
    }
    /* Set This Flag to RRC_TRUE, so that ue connection release notification
     * not sent to UE (DCCH no more exist) */
    p_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = RRC_TRUE;

    if (RRC_SUCCESS != result)
    {
        /* stop timer if it has been started */
        if (PNULL !=
            p_ue_context->timers_data.timers_ids[UECC_UE_CRE_TIMER])
        {
            /* Stop timer */
            uecc_ue_timer_stop(
                    p_ue_context->p_gb_context, 
                    UECC_UE_CRE_TIMER, 
                    &p_ue_context->timers_data);
        }

        /* Process failure according to selected 
         * failure actions list */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type10_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_cre_procedure_finish_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Success handler for UE Connection Re-establishment 
 *                  procedure, Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_cre_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    /* SPR 16062 Start */
    /* Code Removed */
    /* SPR 16062 Stop */
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);
    /* Stop timer */
    uecc_ue_timer_stop(
            p_ue_context->p_gb_context, 
            UECC_UE_CRE_TIMER, 
            &p_ue_context->timers_data);

    /* Clear update configuration IEs flags */
    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.sps_config_updated = 0;
    p_ue_context->m.mac_config_updated = 0;
    p_ue_context->m.meas_config_updated = 0;
    p_ue_context->m.srb1_config_updated = 0;
    p_ue_context->m.srb2_config_updated = 0;
    p_ue_context->m.ue_agg_max_bit_rate_updated = 0;

    /* SPR 16053 Fix Start */
    if (RRC_FAILURE == uecc_rrm_build_and_send_ue_reestab_complete_ind
                                   (p_ue_context, RRC_SUCCESS))
    {
         RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,p_ue_context->
              p_gb_context->facility_name,RRC_WARNING,
              "Unable to send RRC_RRM_UE_REESTAB_COMPLETE_IND");
     }
    /* SPR 16053 Fix Stop */

    /* Check Re-establishment Received during HO at target eNB side */
    if (RRC_TRUE == p_ue_context->trg_ho_fail_rrc_cre_flag)
    {
        /*SPR_17976_START*/
        /*Code Removed*/
        /*SPR_17976_END*/

        /* BZ_836 Fix Start */
        /*BUG 604 changes start*/
        if ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                /* SPR 16062 Start */
                (S1_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
                /* SPR 16062 Stop */
                (INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) || 
                (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ) 
            /*BUG 604 changes stop*/
        {
            /* Send event to UE Main FSM */
            uecc_event_queue_push_event(
                    /* SPR 16062 Start */
                    UECC_EV_HO_ONGOING_UE_CONNECTION_REESTABLISH_SUCCESS_INT,
                    /* SPR 16062 Stop */
                    p_ue_context,
                    PNULL);
            /* SPR 16149 Fix Start */
            p_ue_context->trg_ho_fail_rrc_cre_flag = RRC_FALSE;
            /* SPR 16149 Fix Stop */
        }
        /* BZ_836 Fix Stop */
    }
    else
    {
      /* Send event to UE Main FSM */
      uecc_event_queue_push_event(
          UECC_EV_UE_CONNECTION_REESTABLISH_SUCCESS_INT,
          p_ue_context,
          PNULL);
    }

#ifdef MEM_PROFILE_ON


      RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,p_ue_context->
              p_gb_context->facility_name,RRC_WARNING,
              "\n ***************Sizeof UE CONTEXT After RRC Connection Reestablsihment ");
      qvPoolStats();


#endif

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_cre_procedure_finish_fail
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Failure handler for UE Connection Re-establishment
 *                  procedure, Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_cre_procedure_finish_fail(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
/*SPR 20636 Changes Start*/
    uecc_fsm_event_et failure_ev = (uecc_fsm_event_et)((U32_64_Bit)p_api);
/*SPR 20636 Changes End*/
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* SPR 16053 Fix Start */
    if (RRC_FAILURE == uecc_rrm_build_and_send_ue_reestab_complete_ind
                                   (p_ue_context, RRC_FAILURE))
    {
         RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,p_ue_context->
              p_gb_context->facility_name,RRC_WARNING,
              "Unable to send RRC_RRM_UE_REESTAB_COMPLETE_IND");
    }
    /* SPR 16053 Fix Stop */

    /* stop timer if it has been started */
    if (PNULL !=
         p_ue_context->timers_data.timers_ids[UECC_UE_CRE_TIMER])
    {
        /* Stop timer */
        uecc_ue_timer_stop(
                p_ue_context->p_gb_context,
                UECC_UE_CRE_TIMER, 
                &p_ue_context->timers_data);
    }

    /* notify UE FSM about failure */
    /* Post event to UE FSM for future processing */
    if ((UECC_EV_UE_CONNECTION_REESTABLISH_FAILURE_INT == failure_ev) ||
       (UECC_EV_CSC_DELETE_ALL_UE_REQ_INT == failure_ev)||
       (UECC_EV_S1AP_RESET_MSG_IND_INT == failure_ev))
    {
        uecc_event_queue_push_event(
                failure_ev,
                p_ue_context,
                PNULL);
    }
    else
    {
        /* Send event to UE Main FSM */
        uecc_event_queue_push_event(
                UECC_EV_UE_CONNECTION_REESTABLISH_SUCCESS_INT,
                p_ue_context,
                PNULL);
    }

    if (RRC_TRUE ==
            p_ue_context->trg_ho_fail_rrc_cre_flag)
    {
        /*SPR_17976_START*/
        if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            if(RRC_TRUE != p_ue_context->ho_info.p_ho_info->
                    p_trg_ho_info->is_path_switch_req_ack_recv)
            {
                /*Release UE context maintained at X2AP module*/
                uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
            }

            if(uecc_trg_is_path_switch_ongoing(p_ue_context))
            {
                if(RRC_FAILURE == uecc_s1ap_build_and_send_ue_release_ind(p_ue_context))
                {
                    RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                            "[%s] Failed to send UE Release Indication to S1AP for UE:%u"
                            ,__FUNCTION__, p_ue_context->ue_index);
                }
            }
        }
        /*SPR_17976_END*/

        /*RRC_RRM_HO_ADM_CNF to RRM*/
        uecc_rrm_build_and_send_ue_ho_adm_cnf(
                p_ue_context,
                RRC_FAILURE);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/*SPR_17121_START*/
/*Code Removed*/
/*SPR_17121_END*/

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_change_crnti_ongoing_init_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_cre_llim_change_crnti_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_change_crnti_req_t rrc_uecc_llim_change_crnti_req;
    rrc_return_t                    result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    /* Reset */
    memset_wrapper(&rrc_uecc_llim_change_crnti_req, RRC_NULL,
            sizeof(rrc_uecc_llim_change_crnti_req_t));

    /* ue_index */
    rrc_uecc_llim_change_crnti_req.ue_index = p_ue_context->ue_index;
    rrc_uecc_llim_change_crnti_req.cell_index = p_ue_context->cell_index;

    /* Transaction ID */
    rrc_uecc_llim_change_crnti_req.transaction_id =
        uecc_ue_ctx_get_op_transction_id(p_ue_context);

    /* Fill Old C-RNTI in msg */
    rrc_uecc_llim_change_crnti_req.old_crnti = p_ue_context->old_crnti;
    
    /* Fill New C-RNTI in msg */
    rrc_uecc_llim_change_crnti_req.new_crnti = p_ue_context->crnti;

    /* Vimal */
    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,"Old C-RNTI=[%u] and New C-RNTI=[%u]",
        rrc_uecc_llim_change_crnti_req.old_crnti, rrc_uecc_llim_change_crnti_req.new_crnti);
    
    result = uecc_rrc_send_internal_msg(RRC_LLIM_MODULE_ID,
            UECC_LLIM_CHANGE_CRNTI_REQ,
            sizeof(rrc_uecc_llim_change_crnti_req_t),
            &rrc_uecc_llim_change_crnti_req);

    if (RRC_SUCCESS != result)
    {
        RRC_UECC_CRE_FSM_TRACE(RRC_ERROR,
        "Send UECC_LLIM_CHANGE_CRNTI_REQ message: Failed");

        /* Process failure according to selected
         * failure actions list */
        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_cre_fail_type2_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_change_crnti_resp_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_CHANGE_CRNTI_RESP message.
 ****************************************************************************/
void uecc_fsm_cre_llim_change_crnti_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_change_crnti_resp_t* 
        p_rrc_uecc_llim_change_crnti_resp = PNULL;
    rrc_return_t       result = RRC_FAILURE;
    
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
        "UECC_EV_LLIM_CHANGE_CRNTI_RESP");

    p_rrc_uecc_llim_change_crnti_resp = (rrc_uecc_llim_change_crnti_resp_t*)p_api;

    switch (p_rrc_uecc_llim_change_crnti_resp->response_code)
    {
        case SUCCESS:
            /* CSR 00058385 Fix Start */
             RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP]"
                " Successful Response");
             UECC_FSM_SET_CHILD_STATE(p_ue_context,
                 &uecc_fsm_cre_llim_reconfig_ue_entity_ongoing, PNULL);
            /* CSR 00058385 Fix Stop */
            result = RRC_SUCCESS;
            break;

        case PARTIAL_SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                    "[UECC_LLIM_CHANGE_CRNTI_RESP]"
                    " RRC_PARTIAL_SUCCESS response received from LLIM");
            break;

        case FAILURE:
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_CHANGE_CRNTI_RESP] FAILURE response received from LLIM");
            break;
        case L3_FATAL:

            RRC_UECC_CRE_FSM_TRACE(RRC_FATAL,
                "[UECC_EV_LLIM_CHANGE_CRNTI_RESP] L3_FATAL response received");
            break;

        default:
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_CHANGE_CRNTI_RESP] Invalid Response=%i",
                    p_rrc_uecc_llim_change_crnti_resp->response_code);
            break;
    }
    /* Process failure */
    if (RRC_SUCCESS != result)
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_cre_fail_type2_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_change_crnti_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message.
 ****************************************************************************/
void uecc_fsm_cre_llim_change_crnti_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_BRIEF, "[LLIM CHANGE CRNTI Ongoing]"
        "Time out.");

    /*SPR_21694_START*/
    p_ue_context->timers_data.timers_ids[UECC_UE_CRE_TIMER] = PNULL;
    /*SPR_21694_END*/
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type2_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_suspend_ue_entity_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_cre_llim_suspend_ue_entity_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    /*SPR_17121_START*/
    rrc_rrm_ue_reestab_config_req_t ue_reestab_config_api;
    /*SPR_17727_START*/
    rrc_uecc_ueccmd_ue_con_re_establish_ongoing_t   
                                    uecc_ueccmd_re_estb_msg;
    rrc_return_t                    result = RRC_FAILURE;
    /*SPR_17727_END*/

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);
    
    /* re-estab changes start */
    memset_wrapper(&ue_reestab_config_api,RRC_NULL,sizeof(rrc_rrm_ue_reestab_config_req_t));                
    ue_reestab_config_api.ue_index   =  p_ue_context->ue_index;
    ue_reestab_config_api.rrc_rrm_re_estab_cause = p_ue_context->reestablishmentCause;         

    rrc_rrm_il_send_rrc_rrm_ue_reestab_config_req(
            &ue_reestab_config_api,
            RRC_UECC_MODULE_ID,
            RRC_RRM_MODULE_ID,
            RRC_TRANSACTION_ID_ABSENT,
            p_ue_context->cell_index);
    /* re-estab changes stop */

    /*SPR_17727_START*/
    /* 
     * Send UECCMD_UE_CON_RE_ESTABLISH_ONGOING to UECCMD so that
     * it can delete its re-establishment context
     */
    uecc_ueccmd_re_estb_msg.cell_index = p_ue_context->cell_index;
    uecc_ueccmd_re_estb_msg.rnti = p_ue_context->crnti;
    result = uecc_rrc_send_internal_msg(RRC_UECCMD_MODULE_ID,
            UECCMD_UE_CON_RE_ESTABLISH_ONGOING,
            sizeof(rrc_uecc_ueccmd_ue_con_re_establish_ongoing_t),
            &uecc_ueccmd_re_estb_msg);
    if (RRC_FAILURE == result)
    {
        RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECCMD_UE_CON_RE_ESTABLISH_ONGOING] "
                "Message Delivery failure");

        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_cre_fail_type2_actions);
    }
    else
    {
    /*SPR_17727_END*/

    /* CSR 00058385 Fix Start */

    /* This flag set to RRC_TRUE (in RLF procedure )when RLF
     * indication is received by UECC from LLIM. */
    if (RRC_TRUE != p_ue_context->share_data.uecc_rcr_data.rlf_detected)
    {
            /*SPR_18186_START*/
            if ((RRC_TRUE ==
                        p_ue_context->trg_ho_fail_rrc_cre_flag) &&
                    (((RRC_TRUE != 
                       p_ue_context->ho_info.p_ho_info->
                       p_trg_ho_info->is_mme_status_transfer_ind_recv) &&
                      (RRC_TRUE != 
                       p_ue_context->ho_info.p_ho_info->
                       p_trg_ho_info->is_sn_status_transfer_ind_recv )) ||
                     (RRC_TRUE != p_ue_context->ho_info.p_ho_info->
                      p_trg_ho_info->is_rrc_conn_reconf_comp_recv)))
            {
                /* If HO was ongoing and rrcConnectionReconfiguration has not bee
                 * received, send UECC_LLIM_RESET_UE_ENTITY_REQ to LLIM 
                 */
                if (RRC_SUCCESS != uecc_llim_build_and_send_reset_ue_entity_req(p_ue_context))
                {
                    RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                            "[UECC_LLIM_RESET_UE_ENTITY_REQ] Message Delivery failure");

                    /* Process failure according to selected failure actions list */
                    uecc_fsm_start_failure_processing(
                            p_ue_context,
                            uecc_fsm_cre_fail_type2_actions);
                }
            }
            else
            {
                /* Build and Send the UECC_LLIM_SUSPEND_UE_ENTITY_REQ Required to LLIM */
        	if (RRC_SUCCESS != uecc_llim_build_and_send_suspend_ue_entity_req(p_ue_context))
                {
                     RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                         "[UECC_LLIM_SUSPEND_UE_ENTITY_REQ] Message Delivery failure");

               /* Process failure according to selected failure actions list */
               uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_cre_fail_type2_actions);
                 }
             }
            /*SPR_18186_END*/
    }
    else
    {
        /* Reset, Due to RLF it was set */
        p_ue_context->share_data.uecc_rcr_data.rlf_detected = RRC_FALSE;
        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                &uecc_fsm_cre_llim_re_establish_srb1_ongoing, PNULL);
    }
    /* CSR 00058385 Fix Stop */
    /*SPR_17727_START*/
    }
    /*SPR_17727_END*/

    /*SPR_17121_END*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_suspend_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_SUSPEND_UE_ENTITY_RESP message.
 ****************************************************************************/
void uecc_fsm_cre_llim_suspend_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_suspend_ue_entity_resp_t*  
                              p_uecc_llim_suspend_ue_entity_resp = PNULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_uecc_llim_suspend_ue_entity_resp = 
            (rrc_uecc_llim_suspend_ue_entity_resp_t*)p_api;

    switch (p_uecc_llim_suspend_ue_entity_resp->response_code)
    {

        case SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_SUSPEND_UE_ENTITY_RESP] Successful Response");

             UECC_FSM_SET_CHILD_STATE(p_ue_context,
                 &uecc_fsm_cre_llim_re_establish_srb1_ongoing, PNULL);

            break ;
        case FAILURE :
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_SUSPEND_UE_ENTITY_RESP] Failure Response");

            break;
        default :
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_SUSPEND_UE_ENTITY_RESP] Invalid Response=%i",
                 p_uecc_llim_suspend_ue_entity_resp->response_code);
    }

    /* Process failure */
    if (RRC_SUCCESS != p_uecc_llim_suspend_ue_entity_resp->response_code)
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_cre_fail_type2_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR_18186_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_reset_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_RESET_UE_ENTITY_RESP message.
 ****************************************************************************/
void uecc_fsm_cre_llim_reset_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_reset_ue_entity_cnf_t*
        p_uecc_llim_reset_ue_entity_cnf = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if ((RRC_TRUE ==
                p_ue_context->trg_ho_fail_rrc_cre_flag) &&
            (((RRC_TRUE != 
               p_ue_context->ho_info.p_ho_info->
               p_trg_ho_info->is_mme_status_transfer_ind_recv) &&
              (RRC_TRUE != 
               p_ue_context->ho_info.p_ho_info->
               p_trg_ho_info->is_sn_status_transfer_ind_recv )) ||
             (RRC_TRUE != p_ue_context->ho_info.p_ho_info->
              p_trg_ho_info->is_rrc_conn_reconf_comp_recv)))
    {
        p_uecc_llim_reset_ue_entity_cnf =
            (rrc_uecc_llim_reset_ue_entity_cnf_t*)p_api;

        switch (p_uecc_llim_reset_ue_entity_cnf->response)
        {
            case SUCCESS:
                RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                        "[UECC_EV_LLIM_RESET_UE_ENTITY_RESP] Successful Response");

                UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_cre_llim_re_establish_srb1_ongoing, PNULL);

                break ;
            case FAILURE :
                RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                        "[UECC_EV_LLIM_RESET_UE_ENTITY_RESP] Failure Response");

                break;
            default :
                RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                        "[UECC_EV_LLIM_RESET_UE_ENTITY_RESP] "
                        "Invalid Response=%u",
                        p_uecc_llim_reset_ue_entity_cnf->response);
        }

        /* Process failure */
        /* + coverity 98802 */
        if ( SUCCESS != p_uecc_llim_reset_ue_entity_cnf->response )
            /* - coverity 98802 */
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(p_ue_context,
                    uecc_fsm_cre_fail_type2_actions);
        }
    }
    else
    {
        RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RESET_UE_ENTITY_RESP] Unexpected message "
                "received for UE:%u. Ignoring this message", 
                p_ue_context->ue_index);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR_18186_END*/

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_suspend_ue_entity_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message.
 ****************************************************************************/
void uecc_fsm_cre_llim_suspend_ue_entity_ongoing_timer_expiry_handler( 
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_BRIEF, "[Suspend Ue Entity Ongoing]"
        "Time out.");

    /*SPR_21694_START*/
    p_ue_context->timers_data.timers_ids[UECC_UE_CRE_TIMER] = PNULL;
    /*SPR_21694_END*/
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type2_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_re_establish_srb1_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_cre_llim_re_establish_srb1_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);
    /* SPR 18926 Fix Start */
    /* Sending rlc reestablish req for SRB1, SRB2 and DRBs and 
     * pdcp reestablish req for SRB1 Only */
    /* Build and Send the UECC_LLIM_REESTABLISH_UE_ENTITY_REQ Required to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_reestablish_ue_entity_req(
           p_ue_context, RRC_SRB1_LC_ID, RRC_TRUE))
    /* SPR 18926 Fix Stop */

    {
        RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_LLIM_REESTABLISH_UE_ENTITY_REQ(SRB1)]"
                "Message Delivery failure");
    
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_cre_fail_type2_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_re_establish_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP message.
 ****************************************************************************/
void uecc_fsm_cre_llim_re_establish_srb1_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_re_establish_ue_entity_resp_t*
                              p_uecc_llim_re_establish_ue_entity_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_uecc_llim_re_establish_ue_entity_resp =
                (rrc_uecc_llim_re_establish_ue_entity_resp_t*)p_api;

    switch (p_uecc_llim_re_establish_ue_entity_resp->response_code)
    {

        case SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP]"
                " Successful Response");
             /* CSR 00058385 Fix Start */
             UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_cre_llim_change_crnti_ongoing, PNULL);
             /* CSR 00058385 Fix Stop */
            break ;
        case FAILURE :
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP] Failure Response");

            break;
        default :
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP] Invalid Response=%i",
                 p_uecc_llim_re_establish_ue_entity_resp->response_code);
    }

    /* Process failure */
    if (SUCCESS != p_uecc_llim_re_establish_ue_entity_resp->response_code)
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_cre_fail_type2_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_re_establish_srb1_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message.
 ****************************************************************************/
void uecc_fsm_cre_llim_re_establish_srb1_ongoing_timer_expiry_handler( 
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_BRIEF, "[Reestablishment of SRB1 Ongoing]"
        "Time out.");

    /*SPR_21694_START*/
    p_ue_context->timers_data.timers_ids[UECC_UE_CRE_TIMER] = PNULL;
    /*SPR_21694_END*/
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type2_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_reconfig_ue_entity_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_cre_llim_reconfig_ue_entity_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{

	/* coverity_fix_64372_start */
	rrc_return_et result   = RRC_FAILURE;
	rrc_bool_et is_found   = RRC_FALSE;
	U8 algo_index          = RRC_NULL;

	HandoverPreparationInformation_r8_IEs *p_ho_prep_info = PNULL;

	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

	if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(p_ue_context->p_gb_context,
				p_ue_context->cell_index))
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, p_ue_context->
				p_gb_context->facility_name,RRC_WARNING,"Cell not valid or not active ");

		/* Process failure according to selected failure actions list */
		uecc_fsm_start_failure_processing(
				p_ue_context,
				uecc_fsm_cre_fail_type2_actions);
		RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
		return;
	}

	do
	{
		/* Coverity fix 97141 start */
                /* SPR_18208 start */
                /* SPR 18799 Fix Start */
		if (PNULL != p_ue_context->ho_info.p_ho_info &&
                PNULL != p_ue_context->ho_info.p_ho_info->p_trg_ho_info)
                /* SPR 18799 Fix End */
		{
			/*SPR_17461_START*/
			if ((RRC_TRUE == p_ue_context->trg_ho_fail_rrc_cre_flag) &&
					(RRC_FALSE == p_ue_context->ho_info.p_ho_info->
					 p_trg_ho_info->is_rrc_conn_reconf_comp_recv))
			{
				/*SPR_17461_END*/
                /* SPR 18799 Fix - Null check removed */


				/* X2AP FIX_HO_4 : Start*/
				/*BUG 604 changes start*/
				if ( (X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
						(INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
						(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) ) 
					/*BUG 604 changes stop*/
				{
					p_ho_prep_info = &p_ue_context->ho_info.p_ho_info->p_trg_ho_info->
						rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container;
				}
				else
				{
					p_ho_prep_info = &p_ue_context->ho_info.p_ho_info->p_trg_ho_info->
						rrc_s1ap_ho_request.source_to_target_transparent_container.
						source_enb_to_target_enb_transparent_container.rRC_Container;
				}

				is_found = RRC_FALSE;


				if (!p_ho_prep_info->m.as_ConfigPresent)
				{
					RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
							"AS CONFIG absent. It must be present");
					break;
				}

				/* Check for Ciphering Algo Support */
				for (algo_index = 0;
						algo_index < p_ue_context->p_gb_context->
						supported_security_algo_per_cell.
						supported_security_algorithms[p_ue_context->cell_index].
						ciphering_algorithms.num_algorithms;
						algo_index++)
				{

					/* Check its presence in MME side */
					if (p_ue_context->p_gb_context->
							supported_security_algo_per_cell.
							supported_security_algorithms[p_ue_context->cell_index].
							ciphering_algorithms.algorithms[algo_index] ==
							p_ho_prep_info->as_Config.sourceSecurityAlgorithmConfig.
							cipheringAlgorithm)
					{
						p_ue_context->configure_ciphering.algorithm_id =
							p_ue_context->p_gb_context->
							supported_security_algo_per_cell.
							supported_security_algorithms[p_ue_context->cell_index].
							ciphering_algorithms.algorithms[algo_index];

						p_ue_context->configure_ciphering_user_plane.algorithm_id =
							p_ue_context->p_gb_context->supported_security_algo_per_cell.
							supported_security_algorithms[p_ue_context->cell_index].
							ciphering_algorithms.algorithms[algo_index];

						is_found = RRC_TRUE;
						break;
					}
				}

				if (RRC_FALSE == is_found)
				{
					RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
							"Ciphering algo [%u] received in src_to_trg_container"
							" is not supported.",
							p_ho_prep_info->
							as_Config.sourceSecurityAlgorithmConfig.cipheringAlgorithm);
					break;
				}

				is_found = RRC_FALSE;


				/* Check for Integrity Algo Support */
				for (algo_index = 0; algo_index < p_ue_context->p_gb_context->
						supported_security_algo_per_cell.
						supported_security_algorithms[p_ue_context->cell_index].
						integrity_algorithms.num_algorithms;algo_index++)
				{
					/* Check its presence in MME side */
					if(p_ue_context->p_gb_context->supported_security_algo_per_cell.
							supported_security_algorithms[p_ue_context->cell_index].
							integrity_algorithms.algorithms[algo_index] ==
							p_ho_prep_info->
							as_Config.sourceSecurityAlgorithmConfig.integrityProtAlgorithm)
					{
						p_ue_context->configure_integrity_protection.algorithm_id =
							p_ue_context->p_gb_context->supported_security_algo_per_cell.
							supported_security_algorithms[p_ue_context->cell_index].
							integrity_algorithms.algorithms[algo_index];

						is_found = RRC_TRUE;
						break;
					}
				}


				if (RRC_FALSE == is_found)
				{
					RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
							"Integrity algo [%u] received in src_to_trg_container"
							" is not supported.",
							p_ho_prep_info->
							as_Config.sourceSecurityAlgorithmConfig.integrityProtAlgorithm);

					break;
				}
			}
		}
                /* SPR_18208 stop */
		/* Coverity Fix97141 End */
		/* Bug 896 Fix Start */
        //else
        //{
        if (PNULL != p_ue_context->p_gb_context->p_p_csc_context)
        {
            if (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index])
            {
                if (PNULL != p_ue_context->p_gb_context->
                        p_p_csc_context[p_ue_context->cell_index]->p_csc_init_setup_ind) 
                {    
                    /* Generate KeNB* */
                    rrc_generate_kenb_star(
                            p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                            p_csc_init_setup_ind->phys_cell_id,
                            p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                            p_csc_init_setup_ind->eutra_dl_carrier_freq,
                            &p_ue_context->security_key.data[0],
                            &p_ue_context->security_key.data[0]
                            );
                }
            }
        }
        //}
        /* Bug 896 Fix Stop */


        /* Generate integrity/security keys */
        rrc_generate_key(
                K_RRC_INT_ALG_P1,
                (U8)p_ue_context->
                configure_integrity_protection.algorithm_id,
                &p_ue_context->configure_integrity_protection.key[0],
                &p_ue_context->security_key.data[0]);

        rrc_generate_key(
                K_RRC_ENC_ALG_P1,
                (U8)p_ue_context->configure_ciphering.algorithm_id,
                &p_ue_context->configure_ciphering.key[0],
                &p_ue_context->security_key.data[0]);

        rrc_generate_key(
                K_UP_ENC_ALG_P1,
                (U8)p_ue_context->configure_ciphering_user_plane.
                algorithm_id,
                &p_ue_context->configure_ciphering_user_plane.key[0],
                &p_ue_context->security_key.data[0]);


        result = RRC_SUCCESS;
        /*SPR_17143_START*/
        RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                "SRB Key Generation Inputs during Re-establishment: "
                "Integrity Algo Id[%u] ciphering algo[%u] "
                "security_key size[%u] key  - "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]",
                p_ue_context->configure_integrity_protection.algorithm_id,
                p_ue_context->configure_ciphering.algorithm_id,
                p_ue_context->security_key.numbits,
                p_ue_context->security_key.data[0],
                p_ue_context->security_key.data[1],
                p_ue_context->security_key.data[2],
                p_ue_context->security_key.data[3],
                p_ue_context->security_key.data[4],
                p_ue_context->security_key.data[5],
                p_ue_context->security_key.data[6],
                p_ue_context->security_key.data[7],
                p_ue_context->security_key.data[8],
                p_ue_context->security_key.data[9],
                p_ue_context->security_key.data[10],
                p_ue_context->security_key.data[11],
                p_ue_context->security_key.data[12],
                p_ue_context->security_key.data[13],
                p_ue_context->security_key.data[14],
                p_ue_context->security_key.data[15],
                p_ue_context->security_key.data[16],
                p_ue_context->security_key.data[17],
                p_ue_context->security_key.data[18],
                p_ue_context->security_key.data[19],
                p_ue_context->security_key.data[20],
                p_ue_context->security_key.data[21],
                p_ue_context->security_key.data[22],
                p_ue_context->security_key.data[23],
                p_ue_context->security_key.data[24],
                p_ue_context->security_key.data[25],
                p_ue_context->security_key.data[26],
                p_ue_context->security_key.data[27],
                p_ue_context->security_key.data[28],
                p_ue_context->security_key.data[29],
                p_ue_context->security_key.data[30],
                p_ue_context->security_key.data[31]);

        RRC_UECC_CRE_FSM_TRACE(RRC_INFO, "SRB Integrity Key Data: "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]", 
                p_ue_context->configure_integrity_protection.key[0],
                p_ue_context->configure_integrity_protection.key[1],
                p_ue_context->configure_integrity_protection.key[2],
                p_ue_context->configure_integrity_protection.key[3],
                p_ue_context->configure_integrity_protection.key[4],
                p_ue_context->configure_integrity_protection.key[5],
                p_ue_context->configure_integrity_protection.key[6],
                p_ue_context->configure_integrity_protection.key[7],
                p_ue_context->configure_integrity_protection.key[8],
                p_ue_context->configure_integrity_protection.key[9],
                p_ue_context->configure_integrity_protection.key[10],
                p_ue_context->configure_integrity_protection.key[11],
                p_ue_context->configure_integrity_protection.key[12],
                p_ue_context->configure_integrity_protection.key[13],
                p_ue_context->configure_integrity_protection.key[14],
                p_ue_context->configure_integrity_protection.key[15]);

        RRC_UECC_CRE_FSM_TRACE(RRC_INFO, "SRB Ciphering Key Data: "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]", 
                p_ue_context->configure_ciphering.key[0],
                p_ue_context->configure_ciphering.key[1],
                p_ue_context->configure_ciphering.key[2],
                p_ue_context->configure_ciphering.key[3],
                p_ue_context->configure_ciphering.key[4],
                p_ue_context->configure_ciphering.key[5],
                p_ue_context->configure_ciphering.key[6],
                p_ue_context->configure_ciphering.key[7],
                p_ue_context->configure_ciphering.key[8],
                p_ue_context->configure_ciphering.key[9],
                p_ue_context->configure_ciphering.key[10],
                p_ue_context->configure_ciphering.key[11],
                p_ue_context->configure_ciphering.key[12],
                p_ue_context->configure_ciphering.key[13],
                p_ue_context->configure_ciphering.key[14],
                p_ue_context->configure_ciphering.key[15]);

        RRC_UECC_CRE_FSM_TRACE(RRC_INFO, "User Plane Ciphering Key Data: "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]", 
                p_ue_context->configure_ciphering_user_plane.key[0],
                p_ue_context->configure_ciphering_user_plane.key[1],
                p_ue_context->configure_ciphering_user_plane.key[2],
                p_ue_context->configure_ciphering_user_plane.key[3],
                p_ue_context->configure_ciphering_user_plane.key[4],
                p_ue_context->configure_ciphering_user_plane.key[5],
                p_ue_context->configure_ciphering_user_plane.key[6],
                p_ue_context->configure_ciphering_user_plane.key[7],
                p_ue_context->configure_ciphering_user_plane.key[8],
                p_ue_context->configure_ciphering_user_plane.key[9],
                p_ue_context->configure_ciphering_user_plane.key[10],
                p_ue_context->configure_ciphering_user_plane.key[11],
                p_ue_context->configure_ciphering_user_plane.key[12],
                p_ue_context->configure_ciphering_user_plane.key[13],
                p_ue_context->configure_ciphering_user_plane.key[14],
                p_ue_context->configure_ciphering_user_plane.key[15]);
        /*SPR_17143_END*/

    }while(0);

    if (result == RRC_SUCCESS)
    {

        /* Build and Send the UECC_LLIM_RECONFIG_SECURITY_REQ Required to LLIM */
        if (RRC_SUCCESS != 
                uecc_llim_build_and_send_reconfig_security_req(p_ue_context))
        {
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_RECONFIG_SECURITY_REQ] Message Delivery failure");

            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_cre_fail_type2_actions);
        }
	}
	else
	{
		/* Process failure according to selected failure actions list */
		uecc_fsm_start_failure_processing(
				p_ue_context,
				uecc_fsm_cre_fail_type2_actions);
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* coverity_fix_64372_stop */
/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_reconfigure_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_RECONFIGURE_UE_ENTITY_RESP message.
 ****************************************************************************/
void uecc_fsm_cre_llim_reconfigure_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_reconfig_security_resp_t* 
        p_uecc_llim_reconfig_security_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    p_uecc_llim_reconfig_security_resp = 
        (rrc_uecc_llim_reconfig_security_resp_t*)p_api;
    
    switch (p_uecc_llim_reconfig_security_resp->response_code)
    {

        case SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_RECONFIGURE_UE_ENTITY_RESP]"
                "Successful Response");

            /*SPR_17121_START*/
            if(p_uecc_llim_reconfig_security_resp->bitmask & 
                    UECC_LLIM_RECONFIGURE_SECURITY_MACI_RESP_PRESENT)
            {
                p_ue_context->mac_i = 
                    p_uecc_llim_reconfig_security_resp->mac_i.mac_i; 

                /*SPR_18186_START*/
                if ((RRC_TRUE ==
                            p_ue_context->trg_ho_fail_rrc_cre_flag) &&
                        (((RRC_TRUE != 
                           p_ue_context->ho_info.p_ho_info->
                           p_trg_ho_info->is_mme_status_transfer_ind_recv) &&
                          (RRC_TRUE != 
                           p_ue_context->ho_info.p_ho_info->
                           p_trg_ho_info->is_sn_status_transfer_ind_recv )) ||
                         (RRC_TRUE != p_ue_context->ho_info.p_ho_info->
                          p_trg_ho_info->is_rrc_conn_reconf_comp_recv)))
                {
                    UECC_FSM_SET_CHILD_STATE(p_ue_context,
                            &uecc_fsm_cre_w_for_re_establish_complete, PNULL);
                }
                else
                {
                UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_cre_llim_resume_srb1_ongoing, PNULL);
                }
                /*SPR_18186_END*/
            }
            else
            {
                RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                        "[UECC_EV_LLIM_RECONFIGURE_UE_ENTITY_RESP]"
                        "MACI not recieved");
                p_uecc_llim_reconfig_security_resp->response_code = FAILURE;
            }
            /*SPR_17121_END*/

            break ;
        case FAILURE :
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RECONFIGURE_UE_ENTITY_RESP]"
                "Failure Response");

            break;
        default :
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RECONFIGURE_UE_ENTITY_RESP]Invalid Response=%i", 
                    p_uecc_llim_reconfig_security_resp->response_code);
    }

    /* Process failure */
    if (SUCCESS != p_uecc_llim_reconfig_security_resp->response_code)
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_cre_fail_type2_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_reconfig_ue_entity_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message.
 ****************************************************************************/
void uecc_fsm_cre_llim_reconfig_ue_entity_ongoing_timer_expiry_handler( 
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_BRIEF, "[Reconfig New Security Keys]Time out.");

    /*SPR_21694_START*/
    p_ue_context->timers_data.timers_ids[UECC_UE_CRE_TIMER] = PNULL;
    /*SPR_21694_END*/
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type2_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_resume_srb1_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_cre_llim_resume_srb1_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    /* Build and Send the UECC_LLIM_RESUME_UE_ENTITY_REQ Required to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_resume_ue_entity_req(
           p_ue_context, RRC_SRB1_LC_ID, RRC_PDCP_RB_DIR_BOTH))
    {
        RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_LLIM_RESUME_UE_ENTITY_REQ] Message Delivery failure");

        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_cre_fail_type2_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_resume_srb1_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_RESUME_UE_ENTITY_RESP message.
 ****************************************************************************/
void uecc_fsm_cre_llim_resume_srb1_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_resume_ue_entity_resp_t* 
        p_uecc_llim_resume_ue_entity_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    p_uecc_llim_resume_ue_entity_resp = 
        (rrc_uecc_llim_resume_ue_entity_resp_t*)p_api;
    
    switch (p_uecc_llim_resume_ue_entity_resp->response_code)
    {

        case SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]"
                "SRB1 Resumes Successfully");

             UECC_FSM_SET_CHILD_STATE(p_ue_context,
                 &uecc_fsm_cre_w_for_re_establish_complete, PNULL);

            break ;
        case FAILURE :
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]"
                "SRB1 Fail to Resume");

            break;
        default :
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]Invalid Response=%i", 
                    p_uecc_llim_resume_ue_entity_resp->response_code);
    }

    /* Process failure */
    if (RRC_SUCCESS != p_uecc_llim_resume_ue_entity_resp->response_code)
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_cre_fail_type2_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_resume_srb1_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message.
 ****************************************************************************/
void uecc_fsm_cre_llim_resume_srb1_ongoing_timer_expiry_handler( 
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_BRIEF, "[Resume SRB1 Ongoing]Time out.");

    /*SPR_21694_START*/
    p_ue_context->timers_data.timers_ids[UECC_UE_CRE_TIMER] = PNULL;
    /*SPR_21694_END*/
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type2_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_w_for_re_establish_complete_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_cre_w_for_re_establish_complete_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_rrc_connection_reestablishment(p_ue_context))
    {

        RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[RRCConnectionReestablishment] Message Delivery failure");

        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_cre_fail_type2_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_rrc_connection_re_establish_complete_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRC_CONNECTION_REESTABLISHMENT_COMPLETE message.
 ****************************************************************************/
void uecc_fsm_cre_rrc_connection_re_establish_complete_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                "RRCConnectionReestablishmentComplete Received");

    /* Set This Flag to RRC_FALSE, so that ue connection release notification
     * can be sent to UE (Now DCCH reestablish) */
    p_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = RRC_FALSE;

    /* start next activity for re-establishment procedure */ 
    uecc_fsm_finish_activity_success(
            p_ue_context,
            (void *)UECC_EV_UE_CONNECTION_REESTABLISH_SUCCESS_INT);

    if ((PNULL != p_ue_context->p_gb_context) && (PNULL != g_rrc_stats.p_rrc_stats))
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context, 
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_connection_related_stats.
                rrc_connection_reestablishment.
                rrc_connReEstabSucc_cause[p_ue_context->reestablishmentCause], 
                p_ue_context->reestablishmentCause, (RRC_REESTABLISHMENT_CAUSE_INDEX-1));

        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_connection_related_stats.
                rrc_connection_reestablishment.
                rrc_connReEstabSucc_sum, p_ue_context->reestablishmentCause,
                (RRC_REESTABLISHMENT_CAUSE_INDEX-1));

        /* SPR 17391 Fix Start */
        if((RRC_TRUE == p_ue_context->trg_ho_fail_rrc_cre_flag) &&
                (RRC_FALSE == p_ue_context->ho_info.p_ho_info->
                /*SPR_20720_START*/
                 p_trg_ho_info->is_rrc_conn_reconf_comp_recv) &&
	    		(RRC_FALSE == p_ue_context->m.rrc_connection_count_updated) &&
                (INTRA_CELL_HO != p_ue_context->ho_info.s1_or_x2_handover))
                /*SPR_20720_END*/
        {
        	RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                    g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                    p_gb_context->offset_uecc_mod_id].
                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                    rrc_connection_related_stats.
                    rrcConnectionNumber, 0, DUMMY_RANGE_CHECK_INDEX);
            p_ue_context->m.rrc_connection_count_updated = RRC_TRUE;
        }    
        /* SPR 17391 Fix Stop */
    }

    /*SPR_18186_START*/
    /* Code Removed */
    /*SPR_18186_END*/
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_w_for_re_establish_complete_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message.
 ****************************************************************************/
void uecc_fsm_cre_w_for_re_establish_complete_timer_expiry_handler( 
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_BRIEF, "[Wait For ReestablishmentComplete]"
        "Time out.");

    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
        /* Update the performace counter */
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context, 
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_connection_related_stats.
                rrc_connection_reestablishment.
                rrc_connReEstabFail_cause[p_ue_context->reestablishmentCause],
                p_ue_context->reestablishmentCause, (RRC_REESTABLISHMENT_CAUSE_INDEX-1));

        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context, 
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_connection_related_stats.
                rrc_connection_reestablishment.
                rrc_connReEstabFail_sum, p_ue_context->reestablishmentCause,
                (RRC_REESTABLISHMENT_CAUSE_INDEX-1));

    }

    /*SPR_21694_START*/
    p_ue_context->timers_data.timers_ids[UECC_UE_CRE_TIMER] = PNULL;
    /*SPR_21694_END*/
    /* Continue failure processing */
    /* We have already sent RRC Connection Re-establishment 
     * But timer get expired while waiting for RRC Connection
     * Re-establishment Complete so don't send RRC Connection Re-establishment
     * Reject send only Ue Release Req to MME. */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type1_actions); 

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*CA_RE_ESTAB_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_scell_lower_layer_reconfigure_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_cre_llim_scell_lower_layer_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context, /*UECC UE Context*/
    void*               p_api /*Input API Buffer*/
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    /* Build and Send the UECC_LLIM_RE_ESTABLISH_UE_ENTITY_REQ Required to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_modify_lc_req(p_ue_context))
    {
        RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_LLIM_MODIFY_LC_REQ] Message Delivery failure");

        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_cre_fail_type1_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*CA_RE_ESTAB_STOP*/

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);
    /* SPR 18926 Fix Start */
    /* Sending pdcp reestablish req for SRB2 and DRBs.
     * As, reestablish req for SRB1, SRB2 and DRBs is already sent for RLC
     * So no need to send it again*/
    /* Build and Send the UECC_LLIM_RE_ESTABLISH_UE_ENTITY_REQ Required to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_reestablish_ue_entity_req(
           p_ue_context, RRC_SRB2_LC_ID, RRC_FALSE))
    /* SPR 18926 Fix Stop */

    {
        RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_LLIM_RE_ESTABLISH_UE_ENTITY_REQ(SRB2 and DRBs)]"
                " Message Delivery failure");

        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_cre_fail_type1_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*CA_RE_ESTAB_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_scell_lower_layer_reconfigure_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP message.
 ****************************************************************************/
void uecc_fsm_cre_llim_scell_lower_layer_reconfigure_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context, /*UECC UE Context*/
    void*               p_api /*Input API Buffer*/
)
{
    rrc_uecc_llim_re_establish_ue_entity_resp_t*
                              p_uecc_llim_re_establish_ue_entity_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    p_uecc_llim_re_establish_ue_entity_resp =
                (rrc_uecc_llim_re_establish_ue_entity_resp_t*)p_api;

    switch (p_uecc_llim_re_establish_ue_entity_resp->response_code)
    {

        case SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP]"
                " Successful Response");
	     /*SPR 15738 Fix Start*/

             UECC_FSM_SET_CHILD_STATE(p_ue_context,
                 &uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing, PNULL);

	     /*SPR 15738 Fix Stop*/
            break ;
        case PARTIAL_SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP]"
                "PARTIAL_SUCCESS response received");

            break;
        case FAILURE:
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP]"
                "FAILURE response received");
            break;
        case L3_FATAL:
            RRC_UECC_CRE_FSM_TRACE(RRC_FATAL,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP]"
                "L3_FATAL response received");

            break;

        default :
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP] Invalid Response=%i",
                 p_uecc_llim_re_establish_ue_entity_resp->response_code);
    }

    /* Process failure */
    if (SUCCESS != p_uecc_llim_re_establish_ue_entity_resp->response_code)
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_cre_fail_type1_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*CA_RE_ESTAB_STOP*/
/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_re_establish_srb2_drbs_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP message.
 ****************************************************************************/
void uecc_fsm_cre_llim_re_establish_srb2_drbs_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_re_establish_ue_entity_resp_t*
                              p_uecc_llim_re_establish_ue_entity_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    p_uecc_llim_re_establish_ue_entity_resp =
                (rrc_uecc_llim_re_establish_ue_entity_resp_t*)p_api;

    switch (p_uecc_llim_re_establish_ue_entity_resp->response_code)
    {

        case SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP]"
                " Successful Response");
             /*CA_RE_ESTAB_START*/
            /*Based on the scell configiruation set the child state*/
            if ((RRC_TRUE == p_ue_context->m.scell_config_present) &&
                    (PNULL != p_ue_context->p_scell_config) &&
                    (p_ue_context->p_scell_config->bitmask &
                     RRM_SCELL_ADD_MOD_LIST_PRESENT) &&
                    (p_ue_context->p_scell_config->scell_add_mod_list.
                     count > 0))
            {
                UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_cre_llim_scell_lower_layer_reconfigure_ongoing, PNULL);
            }
            else
            {
                /*SPR 17116 Fix Start*/

                UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing, PNULL);

                /*SPR 17116 Fix Stop*/
            }
             /*CA_RE_ESTAB_STOP*/
            break ;
        case PARTIAL_SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP]"
                "PARTIAL_SUCCESS response received");

            break;
        case FAILURE:
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP]"
                "FAILURE response received");
            break;
        case L3_FATAL:
            RRC_UECC_CRE_FSM_TRACE(RRC_FATAL,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP]"
                "L3_FATAL response received");

            break;

        default :
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_REESTABLISH_UE_ENTITY_RESP] Invalid Response=%i",
                 p_uecc_llim_re_establish_ue_entity_resp->response_code);
    }

    /* Process failure */
    if (SUCCESS != p_uecc_llim_re_establish_ue_entity_resp->response_code)
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_cre_fail_type1_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_srb2_drbs_radio_link_failure_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_RADIO_LINK_FAILURE_IND message.
 ****************************************************************************/
void uecc_fsm_cre_srb2_drbs_radio_link_failure_handler( 
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_BRIEF, "[Reestablish SRB2 and DRBs Ongoing]"
        "Radio link failure is detected");

    /* Set This Flag to RRC_TRUE, so that ue connection release notification
     * not to be sent to UE (due to radio link failure message can't forward
     * to UE) */
    p_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = RRC_TRUE;

    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type1_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/*SPR 15738 Fix Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing_init_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives successful 
 * 		    llim_re_establish_srb2_drbs_ue_entity_response to initiate
 * 		    RLC_REESTAB_UE_ENTITY_COMP 
 ****************************************************************************/
void uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing_init_handler(
		uecc_ue_context_t*  p_ue_context,
		void*               p_api
		)
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* SPR_17377_Warning_fix + */
	RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);
    /* SPR_17377_Warning_fix - */


	if (RRC_SUCCESS != uecc_llim_build_and_send_rlc_reest_ue_entity_comp_ind(
				p_ue_context))
	{
	    RRC_UECC_CRE_FSM_TRACE(RRC_ERROR,
		"Failed to send the reestablish complete indication to LLIM");
	    /* Process failure according to selected failure actions list */
	    uecc_fsm_start_failure_processing(
			 p_ue_context,
			 uecc_fsm_cre_fail_type1_actions);
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}
/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_cnf_msg_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_REESTAB_UE_ENTITY_COMP_CNF message.
 ****************************************************************************/
void uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_cnf_msg_handler(
		uecc_ue_context_t*  p_ue_context,
		void*               p_api
		)
{

	rrc_uecc_llim_re_establish_ue_entity_complete_cnf_t
		*p_rrc_uecc_llim_reestab_ue_entity_comp_cnf
		  = (rrc_uecc_llim_re_establish_ue_entity_complete_cnf_t*)p_api;
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_ASSERT(PNULL != p_api);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	if(RRC_SUCCESS == 
		p_rrc_uecc_llim_reestab_ue_entity_comp_cnf->response_code)
	{
        /*SPR_18186_START*/
        if ((RRC_TRUE ==
                    p_ue_context->trg_ho_fail_rrc_cre_flag) &&
                (((RRC_TRUE != 
                   p_ue_context->ho_info.p_ho_info->
                   p_trg_ho_info->is_mme_status_transfer_ind_recv) &&
                  (RRC_TRUE != 
                   p_ue_context->ho_info.p_ho_info->
                   p_trg_ho_info->is_sn_status_transfer_ind_recv )) ||
                 (RRC_TRUE != p_ue_context->ho_info.p_ho_info->
                  p_trg_ho_info->is_rrc_conn_reconf_comp_recv)))
        {
            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                    &uecc_fsm_cre_w_for_reconfig_complete,
                    PNULL);
        }
        else
        {
		UECC_FSM_SET_CHILD_STATE(p_ue_context,
			&uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ongoing,
			PNULL);
        }
        /*SPR_18186_END*/

	}
	else
	{
		RRC_UECC_CRE_FSM_TRACE(RRC_ERROR,
			"RLC REESTAB COMPLETE CNF response is NOT SUCCESS");
		/* Process failure according to selected failure actions list */
		uecc_fsm_start_failure_processing(
				p_ue_context,
				uecc_fsm_cre_fail_type1_actions);
	}
	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message.
 ****************************************************************************/


void uecc_fsm_cre_llim_rlc_reestab_ue_entity_comp_ongoing_timer_expiry_handler( 
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* SPR_17377_Warning_fix + */
    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);
    /* SPR_17377_Warning_fix - */

    RRC_UECC_CRE_FSM_TRACE(RRC_BRIEF, "[RLC REESTAB UE ENTITY COMPLETE"
        			" Time out.");

    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type1_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR 15738 Fix Stop*/

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message.
 ****************************************************************************/
void uecc_fsm_cre_llim_re_establish_srb2_drbs_ongoing_timer_expiry_handler( 
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_BRIEF, "[Reestablish SRB2 and DRBs Ongoing]"
        "Time out.");

    /*SPR_21694_START*/
    p_ue_context->timers_data.timers_ids[UECC_UE_CRE_TIMER] = PNULL;
    /*SPR_21694_END*/
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type1_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    /* Build and Send the UECC_LLIM_RESUME_UE_ENTITY_REQ Required to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_resume_ue_entity_req(
           p_ue_context, RRC_SRB2_LC_ID, RRC_PDCP_RB_DIR_RX))
    {
        RRC_UECC_CRE_FSM_TRACE(RRC_WARNING, 
            "[UECC_LLIM_RESUME_UE_ENTITY_REQ(SRB2 and DRBs) Uplink]"
            "Message Delivery Failed");

        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_cre_fail_type1_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_RESUME_UE_ENTITY_RESP message.
 ****************************************************************************/
void uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_resume_ue_entity_resp_t* 
        p_uecc_llim_resume_ue_entity_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    p_uecc_llim_resume_ue_entity_resp = 
        (rrc_uecc_llim_resume_ue_entity_resp_t*)p_api;
    
    switch (p_uecc_llim_resume_ue_entity_resp->response_code)
    {

        case SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]"
                "SRB2 and DRBs Resumes (Uplink Direction) Successfully");

             UECC_FSM_SET_CHILD_STATE(p_ue_context,
                 &uecc_fsm_cre_w_for_reconfig_complete, PNULL);

            break ;
        case PARTIAL_SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]"
                "Failed to Resume SRB2 and DRBs(Uplink Direction)");

            break;

        case FAILURE:
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]"
                "Failed to Resume SRB2 and DRBs(Uplink Direction)");

            break;
        case L3_FATAL:
            RRC_UECC_CRE_FSM_TRACE(RRC_FATAL,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]"
                "L3_FATAL response received"
                " to Resume SRB2 and DRBs(Uplink Direction)");

            break;
        default :
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]Invalid Response=%i", 
                    p_uecc_llim_resume_ue_entity_resp->response_code);
    }

    /* Process failure */
    if (SUCCESS != p_uecc_llim_resume_ue_entity_resp->response_code)
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_cre_fail_type1_actions);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message.
 ****************************************************************************/
void uecc_fsm_cre_llim_resume_uplink_srb2_drbs_ongoing_timer_expiry_handler( 
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_BRIEF, "[Resume SRB2 and DRBs"
        "(Uplink direction) Ongoing] Time out.");

    /*SPR_21694_START*/
    p_ue_context->timers_data.timers_ids[UECC_UE_CRE_TIMER] = PNULL;
    /*SPR_21694_END*/
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type1_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_w_for_reconfig_complete_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_cre_w_for_reconfig_complete_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    /* Build and Send RRCConnectionReconfiguration */
    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_rrc_connection_reconfiguration(p_ue_context))
    {
        RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
            "[RRCConnectionReconfiguration]Message Delivery Failed");

        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_cre_fail_type1_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_rrc_connection_reconfiguration_complete_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE message.
 ****************************************************************************/
void uecc_fsm_cre_rrc_connection_reconfiguration_complete_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_rrm_ue_information_req_t ue_info_req;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
        "RRCConnectionReconfigurationComplete is received");

    if (p_ue_context->rlf_proc_flags.m.rrc_reestab_comp_rlf_info_present == 1 || 
            p_ue_context->rlf_proc_flags.m.rrc_conn_setup_reconfigure_rlf_info_present == 1)
    {
        memset_wrapper(&ue_info_req, 0, sizeof(rrc_rrm_ue_information_req_t));
        ue_info_req.ue_Index   = p_ue_context->ue_index;
        ue_info_req.rach_report_req = 0;
        ue_info_req.rlf_report_req  = 1;
        /* Start timer */
        uecc_fsm_rrm_ue_information_req_start_timer(p_ue_context);

        /* Set UE Information intrl procedure flag in ue context */
        p_ue_context->ue_info_proc_running = RRC_TRUE;

        p_ue_context->rlf_proc_flags.ue_info_proc_intrl = RRC_TRUE;

        if(1 == p_ue_context->rlf_proc_flags.m.rrc_conn_setup_reconfigure_rlf_info_present)
        {
            p_ue_context->rlf_proc_flags.ue_info_proc_setup_reconfig_rlf = RRC_TRUE;
            p_ue_context->rlf_proc_flags.ue_info_proc_intrl = RRC_FALSE;
        }


        /* UE INFORMATION REQ */
        if (RRC_SUCCESS != uecc_llim_build_and_send_rrc_ue_information(
                            p_ue_context, &ue_info_req))
        {
            uecc_rrm_ue_information_req_stop_timer(p_ue_context);
        }
    }

    /*SPR_18186_START*/
    if ((RRC_TRUE ==
                p_ue_context->trg_ho_fail_rrc_cre_flag) &&
            (((RRC_TRUE != 
               p_ue_context->ho_info.p_ho_info->
               p_trg_ho_info->is_mme_status_transfer_ind_recv) &&
              (RRC_TRUE != 
               p_ue_context->ho_info.p_ho_info->
               p_trg_ho_info->is_sn_status_transfer_ind_recv )) ||
             (RRC_TRUE != p_ue_context->ho_info.p_ho_info->
              p_trg_ho_info->is_rrc_conn_reconf_comp_recv)))
    {
        uecc_fsm_finish_activity_success(p_ue_context, PNULL);
    }
    else
    {
    UECC_FSM_SET_CHILD_STATE(p_ue_context,
        &uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ongoing, PNULL);
    }
    /*SPR_18186_END*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_w_for_reconfig_complete_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message.
 ****************************************************************************/
void uecc_fsm_cre_w_for_reconfig_complete_timer_expiry_handler( 
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_BRIEF, 
        "[Wait For RRCConnectionReconfigurationComplete] Time out.");

    /*SPR_21694_START*/
    p_ue_context->timers_data.timers_ids[UECC_UE_CRE_TIMER] = PNULL;
    /*SPR_21694_END*/
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type1_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    /* Build and Send the UECC_LLIM_RESUME_UE_ENTITY_REQ Required to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_resume_ue_entity_req(
           p_ue_context, RRC_SRB2_LC_ID, RRC_PDCP_RB_DIR_BOTH))
    {
        RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
            "[UECC_LLIM_RESUME_UE_ENTITY_REQ(SRB2 and DRBs) (downlink)]"
            "Message Delivery Failed");

        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_cre_fail_type1_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_RESUME_UE_ENTITY_RESP message.
 ****************************************************************************/
void uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ue_entity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_resume_ue_entity_resp_t* 
                                  p_uecc_llim_resume_ue_entity_resp = PNULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    p_uecc_llim_resume_ue_entity_resp = 
        (rrc_uecc_llim_resume_ue_entity_resp_t*)p_api;
    
    switch (p_uecc_llim_resume_ue_entity_resp->response_code)
    {

        case SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]"
                "SRB2 and DRBs Resumes (Downlink Direction) Successfully");
            /*CA_RE_ESTAB_START*/
            /* Build and send RRC_RRM_UE_REESTAB_CONFIG_CNF with SUCCESS to RRM */
            (void)uecc_rrm_build_and_send_ue_reestab_config_cnf(
                    p_ue_context, SUCCESS);
            /*CA_RE_ESTAB_STOP*/

            break ;
        case PARTIAL_SUCCESS:
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]"
                "Failed to Resume SRB2 and DRBs(Downlink Direction)");

            break;
        case FAILURE:
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]"
                "Failed to Resume SRB2 and DRBs(Downlink Direction)");

            break;
        case L3_FATAL:
            RRC_UECC_CRE_FSM_TRACE(RRC_FATAL,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]"
                "Timer expired to Resume SRB2 and DRBs(Downlink Direction)");

            break;
        default :
            RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RESUME_UE_ENTITY_RESP]Invalid Response=%i", 
                    p_uecc_llim_resume_ue_entity_resp->response_code);
    }
    
    /*SPR_17976_START*/
    if(RRC_TRUE != p_ue_context->m.reestb_during_reestb)
    {
        /* In case re-establishment request is received when
         * re-establishment is already ongoing, this child
         * FSM will be terminated by main FSM only.
         */
        if (SUCCESS == p_uecc_llim_resume_ue_entity_resp->response_code)
    {
        uecc_fsm_finish_activity_success(p_ue_context, PNULL);
    }
    else
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_cre_fail_type1_actions);
    }
    }
    /*SPR_17976_END*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_TIMER_EXPIRY message.
 ****************************************************************************/
void uecc_fsm_cre_llim_resume_downlink_srb2_drbs_ongoing_timer_expiry_handler( 
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_BRIEF, "[Resume SRB2 and DRBs"
        "(Downlink direction) Ongoing] Time out.");

    /*SPR_21694_START*/
    p_ue_context->timers_data.timers_ids[UECC_UE_CRE_TIMER] = PNULL;
    /*SPR_21694_END*/
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type1_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/***** Local Function Definitions *****/

/****************************************************************************
 * Function Name  : uecc_fsm_cre_process_rrc_connection_reestablishment_req_msg
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This action routine is called when 
 *                  RRC Connection Reestablishment Request api is processed.
 ****************************************************************************/
rrc_return_et uecc_fsm_cre_process_rrc_connection_reestablishment_req_msg(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRCConnectionReestablishmentRequest_r8_IEs* 
        p_rrcConnectionReeestablishmentRequest_r8 = PNULL;
    rrc_return_et       result = RRC_FAILURE;
    
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    p_rrcConnectionReeestablishmentRequest_r8 = 
        (RRCConnectionReestablishmentRequest_r8_IEs*)p_api;

    /* Store ReestabUeIdentity: shortMAC-I in ue context */
    rrc_unpack_U16(
            &p_ue_context->p_reestab_ue_identity->short_mac_i,
            p_rrcConnectionReeestablishmentRequest_r8->ue_Identity.shortMAC_I.data);

    rrc_unpack_U16(
            &p_ue_context->p_reestab_ue_identity->crnti,
            p_rrcConnectionReeestablishmentRequest_r8->ue_Identity.c_RNTI.data);

    p_ue_context->p_reestab_ue_identity->pci = 
            p_rrcConnectionReeestablishmentRequest_r8->ue_Identity.physCellId;

    result = RRC_SUCCESS;
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/*SPR_17121_START*/
/*Code Removed*/
/*SPR_17121_END*/

/****************************************************************************
 * Function Name  : uecc_fsm_cre_srb1_cell_delelte_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message 
 *                  UECC_EV_CSC_DELETE_ALL_UE_REQ during re-establishing
 *                  SRB1 in first activity of procedure
 ****************************************************************************/
void uecc_fsm_cre_srb1_cell_delelte_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void* p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
        "[UECC_EV_CSC_DELETE_ALL_UE_REQ is Received]");

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type4_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_cre_srb2_drbs_cell_delelte_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message 
 *                  UECC_EV_CSC_DELETE_ALL_UE_REQ during re-establishing
 *                  SRB2 and DRBs in Second Activity of procedure
 ****************************************************************************/
void uecc_fsm_cre_srb2_drbs_cell_delelte_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void* p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
        "[UECC_EV_CSC_DELETE_ALL_UE_REQ is Received]");

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type7_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_cre_ue_release_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_UE_RELEASE_REQ SRB1 re-establishment ongoing.
 ****************************************************************************/
void uecc_fsm_cre_srb1_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void* p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
        "[UECC_EV_UE_RELEASE_REQ is Received]");

    /* SPR 16053 Fix Start */
    if (RRC_FAILURE == uecc_rrm_build_and_send_ue_reestab_complete_ind
                                   (p_ue_context, RRC_FAILURE))
    {
         RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,p_ue_context->
              p_gb_context->facility_name,RRC_WARNING,
              "Unable to send RRC_RRM_UE_REESTAB_COMPLETE_IND");
    }
    /* SPR 16053 Fix Stop */

    /* SPR 16782 Fix Start */
    if (RRC_TRUE ==
            p_ue_context->trg_ho_fail_rrc_cre_flag)
    {
        /*SPR_17976_START*/
        if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            if(RRC_TRUE != p_ue_context->ho_info.p_ho_info->
                    p_trg_ho_info->is_path_switch_req_ack_recv)
            {
                /*Release UE context maintained at X2AP module*/
                uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
            }

            if(uecc_trg_is_path_switch_ongoing(p_ue_context))
            {
                if(RRC_FAILURE == uecc_s1ap_build_and_send_ue_release_ind(p_ue_context))
                {
                    RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                            "[%s] Failed to send UE Release Indication to S1AP for UE:%u"
                            ,__FUNCTION__, p_ue_context->ue_index);
                }
            }
        }
        /*SPR_17976_END*/

        /*RRC_RRM_HO_ADM_CNF to RRM*/
        uecc_rrm_build_and_send_ue_ho_adm_cnf(
                p_ue_context,
                RRC_FAILURE);
    }
    /* SPR 16782 Fix Stop */

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type9_actions);

    /* Stop timer */
    uecc_ue_timer_stop(
            p_ue_context->p_gb_context,
            UECC_UE_CRE_TIMER, 
            &p_ue_context->timers_data);    

    /* push event with received api
     * UE context release command */
    uecc_event_queue_push_event(
        UECC_EV_UE_RELEASE_REQ_INT,
        p_ue_context,
        p_api);

    /* Finish Current procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_srb2_drbs_ue_release_req_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_UE_RELEASE_REQ during SRB2 and DRBs
 *                  Re-establishment ongoing.
 ****************************************************************************/
void uecc_fsm_cre_srb2_drbs_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void* p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
        "[UECC_EV_UE_RELEASE_REQ is Received]");

    /* SPR 16053 Fix Start */
    if (RRC_FAILURE == uecc_rrm_build_and_send_ue_reestab_complete_ind
                                   (p_ue_context, RRC_FAILURE))
    {
         RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,p_ue_context->
              p_gb_context->facility_name,RRC_WARNING,
              "Unable to send RRC_RRM_UE_REESTAB_COMPLETE_IND");
    }
    /* SPR 16053 Fix Stop */

    /* SPR 16782 Fix Start */
    if (RRC_TRUE ==
            p_ue_context->trg_ho_fail_rrc_cre_flag)
    {
        /*SPR_17976_START*/
        if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            if(RRC_TRUE != p_ue_context->ho_info.p_ho_info->
                    p_trg_ho_info->is_path_switch_req_ack_recv)
            {
                /*Release UE context maintained at X2AP module*/
                uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
            }

            if(uecc_trg_is_path_switch_ongoing(p_ue_context))
            {
                if(RRC_FAILURE == uecc_s1ap_build_and_send_ue_release_ind(p_ue_context))
                {
                    RRC_UECC_CRE_FSM_TRACE(RRC_WARNING,
                            "[%s] Failed to send UE Release Indication to S1AP for UE:%u"
                            ,__FUNCTION__, p_ue_context->ue_index);
                }
            }
        }
        /*SPR_17976_END*/

        /*RRC_RRM_HO_ADM_CNF to RRM*/
        uecc_rrm_build_and_send_ue_ho_adm_cnf(
                p_ue_context,
                RRC_FAILURE);
    }
    /* SPR 16782 Fix Stop */

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_UE_CRE_TIMER, 
            &p_ue_context->timers_data);    

    if(RRC_FAILURE == x2ap_build_and_send_reestablishment_ind(
                p_ue_context))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_ERROR,
                "x2ap_build_and_send_reestablishment_ind failed.");
    }

    /*SPR_17976_START*/
    /*Code Removed*/
    /*SPR_17976_END*/

    /* push event with received api
     * UE context release command */
    uecc_event_queue_push_event(
        UECC_EV_UE_RELEASE_REQ_INT,
        p_ue_context,
        p_api);

    /* Finish Current procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_cre_srb1_radio_link_failure_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_RADIO_LINK_FAILURE_IND during srb1 
 *                  re-establishment is ongoing.
 ****************************************************************************/
void uecc_fsm_cre_srb1_radio_link_failure_handler(
    uecc_ue_context_t*  p_ue_context,
    void* p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    /* Ignore Radio link failure indication when re-establishment of SRB1
     * is ongoing */
    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
        "[SRB1 Re-establishment is ongoing]:"
        "UECC_EV_LLIM_RADIO_LINK_FAILURE_IND is Received");

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_srb1_reset_msg_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_S1AP_RESET_MSG_IND during srb1 re-establishment
 *                  Ongoing.
 ****************************************************************************/
void uecc_fsm_cre_srb1_reset_msg_ind_handler(
    uecc_ue_context_t*  p_ue_context,
    void* p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
        "[UECC_EV_S1AP_RESET_MSG_IND is Received]");

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type8_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_srb2_drbs_reset_msg_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_S1AP_RESET_MSG_IND during SRB1 and DRBs 
 *                  Re-establishment ongoing.
 ****************************************************************************/
void uecc_fsm_cre_srb2_drbs_reset_msg_ind_handler(
    uecc_ue_context_t*  p_ue_context,
    void* p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_api);

    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
        "[UECC_EV_S1AP_RESET_MSG_IND is Received]");

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_cre_fail_type6_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* Failure processing functions definitions */
/****************************************************************************
 * Function Name  : 
 * uecc_fsm_cre_fail_action_build_and_send_rrc_connection_reestablishment_reject
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*     -   p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of RRC Connection Reestablishment Reject message to UE
 ****************************************************************************/
void 
uecc_fsm_cre_fail_action_build_and_send_rrc_connection_reestablishment_reject(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_break);

    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
            "Send RRCConnectionReestablishmentReject");

    if (RRC_SUCCESS != 
           uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
           p_ue_context->p_gb_context, p_ue_context->crnti,
           p_ue_context->reestablishmentCause,
           p_ue_context->cell_index, RRC_LLIM_MODULE_ID))
    {
        RRC_UECC_CRE_FSM_TRACE(RRC_ERROR,
                "Fail to send the RRCConnectionReestablishmentReject to UE");
    }
    /*SPR_16680_START*/
    /* When UECC is sending rrcConnectionReestablishmentReject directly 
     * to LLIM, it has to explicitly send message to X2 module for RLF
     * Indication whereas when rrcConnectionReestablimentReject is
     * sent via UECCMD, this is not required because UECCMD sends
     * message to X2 module for RLF. */
    if(RRC_FAILURE == x2ap_build_and_send_reestablishment_ind(
                p_ue_context))
    {    
        RRC_UECC_CRE_FSM_TRACE(RRC_ERROR,
                "x2ap_build_and_send_reestablishment_ind failed.");
    }    
    /*SPR_16680_END*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : 
* uecc_fsm_cre_fail_action1_build_and_send_rrc_connection_reestablishment_reject
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*     -   p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of RRC Connection Reestablishment Reject message to UE
 ****************************************************************************/
void 
uecc_fsm_cre_fail_action1_build_and_send_rrc_connection_reestablishment_reject(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_break);

    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
            "Send RRCConnectionReestablishmentReject");

    if (RRC_SUCCESS != 
           uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
           p_ue_context->p_gb_context, p_ue_context->crnti,
           p_ue_context->reestablishmentCause,
           p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
    {
        RRC_UECC_CRE_FSM_TRACE(RRC_ERROR,
                "Fail to send the RRCConnectionReestablishmentReject to UE");
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_cre_fail_action_last
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*     -   p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Call procedure's fail function
 ****************************************************************************/
void uecc_fsm_cre_fail_action_last(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_break);

    RRC_UECC_CRE_FSM_TRACE(RRC_INFO, "FAILURE: END.");

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_UE_CONNECTION_REESTABLISH_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}


/****************************************************************************
 * Function Name  : uecc_fsm_cre_fail_action_post_event2
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*     -   p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Call procedure's fail function
 ****************************************************************************/
void uecc_fsm_cre_fail_action_post_event2(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_break);

    RRC_UECC_CRE_FSM_TRACE(RRC_INFO, "POSTING EVENT TO MAIN FSM.");

    uecc_fsm_finish_activity_failure(p_ue_context, 
        (void*)UECC_EV_CSC_DELETE_ALL_UE_REQ_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_cre_fail_action_post_event4
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*     -   p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Call procedure's fail function
 ****************************************************************************/
void uecc_fsm_cre_fail_action_post_event4(
        uecc_ue_context_t*  p_ue_context,
        rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_break);

    RRC_UECC_CRE_FSM_TRACE(RRC_INFO, "POSTING EVENT TO MAIN FSM.");

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_S1AP_RESET_MSG_IND_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_fail_action_build_and_send_reestab_ind
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Call procedure's fail function
 * **************************************************************************/
void uecc_fsm_cre_fail_action_build_and_send_reestab_ind(
                       uecc_ue_context_t*  p_ue_context,
                       rrc_bool_et*        p_break)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_CRE_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_break);

/*CA_PHASE3_INTER_CELL_MRO_START*/
    if (RRC_TRUE == p_ue_context->too_late_inter_cell_ho_flg)
    {
        if (RRC_FAILURE == x2ap_build_and_send_rlf_ind(p_ue_context,
                    p_ue_context->p_gb_context))
        {     
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_ERROR,"x2ap_build_and_send_rlf_ind failed.");
        }
    }
/*CA_PHASE3_INTER_CELL_MRO_STOP*/
    else
    {

        if(RRC_FAILURE == x2ap_build_and_send_reestablishment_ind(
                    p_ue_context))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_ERROR, "x2ap_build_and_send_reestablishment_ind failed.");
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* BUG 11253 Fix Start */
/****************************************************************************
 * Function Name  : uecc_fsm_cre_srb1_ue_conn_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND 
 *                  SRB1 re-establishment ongoing.
 ****************************************************************************/
void uecc_fsm_cre_srb1_ue_conn_release_ind_handler(
    uecc_ue_context_t*  p_ue_context,
    void* p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
            "[UECC_EV_RRM_UE_CONNECTION_RELEASE_IND is Received]");

    /* Process failure according to selected failure actions list */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_cre_fail_type9_actions);

     /* Since RRCConnectionReestablishmentReject is sent , 
      * no need to send RRC Connection Release*/
     p_ue_context->share_data.uecc_rcr_data.
      rrc_connection_release_not_required_flag = RRC_TRUE;
     p_ue_context->share_data.uecc_rcr_data.
         rrm_rel_req_not_required_flag = RRC_TRUE;
    /* Stop timer */
    uecc_ue_timer_stop(
            p_ue_context->p_gb_context,
            UECC_UE_CRE_TIMER, 
            &p_ue_context->timers_data);    
    
      /* push event with received api
     * UE context release command */ 
    uecc_event_queue_push_event(
            UECC_EV_RRM_UE_CONNECTION_RELEASE_IND,
            p_ue_context,
            p_api);

    /* Finish Current procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_cre_srb2_drbs_ue_conn_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND during SRB2 and DRBs
 *                  Re-establishment ongoing.
 ****************************************************************************/
void uecc_fsm_cre_srb2_drbs_ue_conn_release_ind_handler(
    uecc_ue_context_t*  p_ue_context,
    void* p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_CRE_FSM_TRACE(RRC_INFO,
        "[UECC_EV_RRM_UE_CONNECTION_RELEASE_IND is Received]");

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,UECC_UE_CRE_TIMER, 
            &p_ue_context->timers_data);    

    if(RRC_FAILURE == x2ap_build_and_send_reestablishment_ind(
                p_ue_context))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_ERROR,
                "x2ap_build_and_send_reestablishment_ind failed.");
    }

    /* Deallocate memory previously allocated for
      RRCConnectionReestablishmentReq */
    if (PNULL != p_ue_context->p_reestab_ue_identity)
    {
        rrc_mem_free(p_ue_context->p_reestab_ue_identity);
        p_ue_context->p_reestab_ue_identity = PNULL;
    }
    
     p_ue_context->share_data.uecc_rcr_data.
         rrm_rel_req_not_required_flag = RRC_TRUE;
    /* push event with received api
     * UE context release command */
    uecc_event_queue_push_event(
        UECC_EV_RRM_UE_CONNECTION_RELEASE_IND,
        p_ue_context,
        p_api);

    /* Finish Current procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* BUG 11253 Fix Stop */

