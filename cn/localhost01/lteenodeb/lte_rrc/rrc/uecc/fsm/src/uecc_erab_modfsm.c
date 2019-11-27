/*******************************************************************************
 *
 *  FILE NAME: uecc_erab_modfsm.c
 *
 *  DESCRIPTION : UECC E-RAB MODIFY FSM implementation functions
 *
 *  REVISION HISTORY :
 *
 *  DATE        AUTHOR      REF                          REASON
 *  --------    -------     ---                          ----------
 *  25 May 2010     TSinha  LTE_L3_DD_Rel_1_2_Rev_3_1    Initial
 *
 *  Copyright (c) 2010, Aricent Inc.
 ******************************************************************************/

/*******************************************************************************
* Header File Includes
*******************************************************************************/
#include "rrc_rrm_intf.h"
#include "uecc_logger.h"
#include "uecc_common.h"
#include "uecc_ue_timer_utils.h"
#include "uecc_rrm.h"
#include "uecc_llim.h"
#include "uecc_s1ap.h"
#include "uecc_defaults.h"
#include "uecc_fsm_engine.h"
#include "uecc_s1ap.h"
#include "rrc_msg_mgmt.h"
#include "rrc_asn_enc_dec.h"
#include "uecc_utils.h"
#include "rrc_intrl_api.h"
#include "rrc_s1ap_asn_intrl_dec_3gpp.h"
#include "rrc_shared_stats.h"


/******************************************************************************
 * Local definitions
 ******************************************************************************/
#define RRC_UECC_ERB_MODIFY_FSM_TRACE(log_level, format, ...)              \
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,    \
        p_ue_context->ue_index,                                            \
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,        \
        ##__VA_ARGS__)

#define RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(err_code_)  \
    err_code_|=(RRC_MODULE_ID << 16)

#define SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context)\
    (p_ue_context->share_data.uecc_rcr_data.\
     rrc_connection_release_not_required_flag = RRC_TRUE)


/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static void uecc_fsm_erb_modify_p_constructor(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_modify_p_success_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_modify_p_failure_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/* State UECC_ERB_MODIFY_RRM_RESOURCE_MODIFICATION_ONGOING */
static void uecc_fsm_erb_modify_rrm_resource_modification_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_modify_rrm_erb_modify_resp_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

static void 
    uecc_fsm_erb_modify_rrm_resource_modify_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

static void  uecc_fsm_erb_modify_rrm_ho_required_indication_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);
static void uecc_fsm_erb_modify_rrc_reestablish_request_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);
/* State UECC_ERB_MODIFY_LLIM_RECONFIGURE_UE_ONGOING */
static void uecc_fsm_erb_modify_llim_reconfigure_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_modify_llim_reconfigure_ue_resp_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

static void
uecc_fsm_erb_modify_llim_reconf_ue_ongoing_ho_required_ind_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
);

static void
uecc_fsm_erb_modify_llim_radio_link_failure_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
);
static void 
uecc_fsm_erb_modify_llim_reconf_ue_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

static void 
uecc_fsm_erb_modify_llim_reconfig_ue_rrc_reestablish_request_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);
/* State UECC_ERB_MODIFY_LLIM_RECONFIGURE_UE_ONGOING */
static void uecc_fsm_erb_modify_llim_rollback_ongoing_init_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

static void uecc_fsm_erb_modify_llim_modify_rollback_resp_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

static void 
uecc_fsm_erb_modify_ue_reconfigure_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

static void uecc_fsm_erb_modify_ue_radio_link_failure_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
);

/* State UECC_ERB_MODIFY_UE_RECONFIGURE_ONGOING */
static void uecc_fsm_erb_modify_ue_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_modify_ue_rrc_conn_reconf_complete_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_modify_ue_rrc_reestablish_request_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
);

static void 
uecc_fsm_erb_modify_ue_rrm_ho_required_indication_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
);
static void 
uecc_fsm_erb_modify_llim_rollback_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
static void
uecc_fsm_erb_modify_rollback_radio_link_failure_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
);

static void 
uecc_fsm_erb_modify_rrc_re_establish_request_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
);

static void 
uecc_fsm_erb_modify_rrm_ho_required_ind_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
);

static void 
uecc_fsm_erb_modify_radio_link_failure_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_modify_ue_rel_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_erb_modify_s1ap_reset_msg_ind_handler
(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_modify_rrm_ue_release_req_handler(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_erb_modify_csc_cell_delete_req_handler(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
); 

static void uecc_fsm_erb_modify_rrm_ho_prep_timer_expiry_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		);  

/*SPR 18758 Fix Start*/
/* Code Deleted */
/*SPR 18758 Fix Stop*/

/*SPR_18241_START*/
/* Code Removed */
/*SPR_18241_END*/

/*SPR 18758 Fix Start*/
/* Code Deleted */
/*SPR 18758 Fix Stop*/

static void uecc_fsm_erb_modify_erab_release_ind_handler(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
);

rrc_return_t uecc_fsm_process_s1ap_erab_modify_msg(
    uecc_ue_context_t*                 p_ue_context,
    rrc_s1ap_erab_modify_request_t*    p_msg
);

rrc_return_t uecc_fsm_erab_modify_process_rrm_erab_modify_resp_data(
    uecc_ue_context_t*  p_ue_context,
    rrc_rrm_erab_modify_resp_t* p_rrm_erb_modify_resp
);

/* Failure processing functions */
void uecc_fsm_erb_modify_fail_action_send_rrm_cnf(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
);

void uecc_fsm_erb_mod_fail_action_send_fail_resp_to_s1ap(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
);

void uecc_fsm_erb_mod_fail_action_send_ue_context_release_req
(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
);

void uecc_fsm_erb_mod_fail_action_ue_context_release_last(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
);

void uecc_fsm_erb_mod_fail_action_last(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
);

rrc_return_t  uecc_update_to_be_modified_list_from_mme
(
    uecc_ue_context_t *p_ue_context,
    rrc_s1ap_erab_modify_request_t *p_msg
);

void uecc_update_ue_context(
    uecc_ue_context_t*  p_ue_context
);
/*SPR_16942_START */
void uecc_update_mac_config_in_ue_context(
        uecc_ue_context_t*  p_ue_context,
        rrm_mac_config_t*   p_mac_config
        );
/*SPR_16942_END*/
void uecc_update_drb_config_in_ue_context(
    uecc_ue_context_t*  p_ue_context,
    erab_modify_list_t *p_mod_list 
);
rrc_return_t  uecc_validate_and_fill_erab_list
(
    uecc_ue_context_t                      *p_ue_context,
    s1ap_E_RABToBeModifiedItemBearerModReq *p_e_rab_list[],
    erab_to_be_modified_list_t             *p_to_modify_list,
    erab_failed_to_modify_list_t           *p_failed_modify_list,
    U16                                    count
);

rrc_bool_et uecc_is_failed_list_updated(
     erab_failed_to_modify_list_t           *p_failed_modify_list,
     s1ap_E_RAB_ID                          erab_id
);

void uecc_update_failed_list_from_to_be_modified_list
(
    uecc_ue_context_t*  p_ue_context,
    s1ap_Cause          cause,
    U32                 error
);

void uecc_update_failed_list_from_modified_list
(
    uecc_ue_context_t*  p_ue_context,
    s1ap_Cause          cause
);

void uecc_update_failure_list(
    uecc_ue_context_t*              p_ue_context,
    rrc_rrm_erab_modify_resp_t*     p_rrc_rrm_erb_modify_resp
);
/*SPR 22036 Fix Start*/
void uecc_fsm_erab_modify_ongoing_s1ap_reset_msg_handler(
		uecc_ue_context_t*  p_ue_context,         
		void*               p_api                
		);

void uecc_fsm_erb_modify_llim_del_reconfigure_ue_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );
void uecc_fsm_erb_modify_llim_del_reconfigure_ue_resp_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );
void uecc_fsm_erb_modify_llim_del_reconf_ue_ongoing_timer_expiry_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );
void uecc_fsm_erb_modify_llim_del_radio_link_failure_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );
void uecc_fsm_erb_modify_llim_del_ue_rel_req_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );
void uecc_fsm_erb_modify_llim_del_reconfig_ue_rrc_reestablish_request_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        );

void uecc_fsm_erabmod_update_erab_release_list_with_error_code(
		uecc_ue_context_t* p_ue_context,
		void*              p_error_code
		);

void uecc_fsm_erbmod_fail_action_mark_all_erab_failed(
    uecc_ue_context_t*  p_ue_context,
    U32                 error_code
);

/*SPR 22036 Fix Stop*/

/****************************************************************************
 Private Variables
****************************************************************************/
static uecc_fsm_handler_record_t 
    uecc_fsm_erb_modify_rrm_resource_modification_ongoing_mht[] =
{
    {{ UECC_EV_STATE_INIT                          },
       uecc_fsm_erb_modify_rrm_resource_modification_ongoing_init_handler  },
    {{ UECC_EV_RRM_ERAB_MODIFY_RESP                },
       uecc_fsm_erb_modify_rrm_erb_modify_resp_handler                     },
    {{ UECC_EV_TIMER_EXPIRY                        },
       uecc_fsm_erb_modify_rrm_resource_modify_ongoing_timer_expiry_handler},
    {{ UECC_EV_RRM_HO_REQUIRED                 },
       uecc_fsm_erb_modify_rrm_ho_required_indication_handler              },
    {{ UECC_EV_LLIM_RADIO_LINK_FAILURE_IND         },
       uecc_fsm_erb_modify_radio_link_failure_handler                      },
    { { UECC_EV_UE_RELEASE_REQ                     },
            uecc_fsm_erb_modify_ue_rel_req_handler                         },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_erb_modify_s1ap_reset_msg_ind_handler                 },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ                      },
            uecc_fsm_erb_modify_rrc_reestablish_request_handler         },
    {{ UECC_LAST_ID                    },  PNULL                           }
};

static uecc_fsm_handler_record_t 
    uecc_fsm_erb_modify_llim_reconfigure_ue_ongoing_mht[] =
{
    {{ UECC_EV_STATE_INIT                           },
       uecc_fsm_erb_modify_llim_reconfigure_ue_ongoing_init_handler        },
    {{ UECC_EV_LLIM_RECONFIGURE_UE_RESP             },
       uecc_fsm_erb_modify_llim_reconfigure_ue_resp_handler                },
    {{ UECC_EV_TIMER_EXPIRY                         },
       uecc_fsm_erb_modify_llim_reconf_ue_ongoing_timer_expiry_handler     },
    {{ UECC_EV_RRM_HO_REQUIRED                  },
       uecc_fsm_erb_modify_llim_reconf_ue_ongoing_ho_required_ind_handler  },
    {{ UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
       uecc_fsm_erb_modify_llim_radio_link_failure_handler                 },
    { { UECC_EV_UE_RELEASE_REQ                     },
            uecc_fsm_erb_modify_ue_rel_req_handler                         },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_erb_modify_s1ap_reset_msg_ind_handler                 },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ                      },
       uecc_fsm_erb_modify_llim_reconfig_ue_rrc_reestablish_request_handler},
    {{ UECC_LAST_ID                        },  PNULL                       }
};
/*SPR 22036 Fix Start*/

static uecc_fsm_handler_record_t
    uecc_fsm_rm_llim_erabmod_del_reconfigure_ue_ongoing_mht[] =
{
    {{ UECC_EV_STATE_INIT                           },
       uecc_fsm_erb_modify_llim_del_reconfigure_ue_ongoing_init_handler        },
    {{ UECC_EV_LLIM_RECONFIGURE_UE_RESP             },
       uecc_fsm_erb_modify_llim_del_reconfigure_ue_resp_handler                },
    {{ UECC_EV_TIMER_EXPIRY                         },
       uecc_fsm_erb_modify_llim_del_reconf_ue_ongoing_timer_expiry_handler     },
    {{ UECC_EV_LLIM_RADIO_LINK_FAILURE_IND          },
       uecc_fsm_erb_modify_llim_del_radio_link_failure_handler                 },
    { { UECC_EV_UE_RELEASE_REQ                      },
       uecc_fsm_erb_modify_llim_del_ue_rel_req_handler                          },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ                },
       uecc_fsm_erb_modify_llim_del_reconfig_ue_rrc_reestablish_request_handler },
    { { UECC_EV_S1AP_RESET_MSG_IND                  },
       uecc_fsm_erab_modify_ongoing_s1ap_reset_msg_handler                      },   
    {{ UECC_LAST_ID                                 },  PNULL                       }
};
/*SPR 22036 Fix Stop*/

static uecc_fsm_handler_record_t
        uecc_fsm_erb_modify_ue_reconfigure_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                                },
            uecc_fsm_erb_modify_ue_reconfigure_ongoing_init_handler        },
    { { UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE   },
            uecc_fsm_erb_modify_ue_rrc_conn_reconf_complete_handler        },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ                      },
            uecc_fsm_erb_modify_ue_rrc_reestablish_request_handler         },
    { { UECC_EV_RRM_HO_REQUIRED                       },
            uecc_fsm_erb_modify_ue_rrm_ho_required_indication_handler      },
    { { UECC_EV_TIMER_EXPIRY                              },
            uecc_fsm_erb_modify_ue_reconfigure_ongoing_timer_expiry_handler},
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND               },
            uecc_fsm_erb_modify_ue_radio_link_failure_handler              },
    { { UECC_EV_UE_RELEASE_REQ                     },
            uecc_fsm_erb_modify_ue_rel_req_handler                         },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_erb_modify_s1ap_reset_msg_ind_handler                 },
    { { UECC_LAST_ID                            },  PNULL                  }
};

static uecc_fsm_handler_record_t
        uecc_fsm_erb_modify_llim_rollback_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                             },
            uecc_fsm_erb_modify_llim_rollback_ongoing_init_handler         },
    { { UECC_EV_LLIM_MODIFY_ROLLBACK_RESP              },
            uecc_fsm_erb_modify_llim_modify_rollback_resp_handler          },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ                   },
            uecc_fsm_erb_modify_rrc_re_establish_request_handler           },
    { { UECC_EV_RRM_HO_REQUIRED                    },
            uecc_fsm_erb_modify_rrm_ho_required_ind_handler                },
    { { UECC_EV_TIMER_EXPIRY                           },
            uecc_fsm_erb_modify_llim_rollback_ongoing_timer_expiry_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND            },
            uecc_fsm_erb_modify_rollback_radio_link_failure_handler        },
    { { UECC_EV_UE_RELEASE_REQ                     },
            uecc_fsm_erb_modify_ue_rel_req_handler                         },
    { { UECC_EV_S1AP_RESET_MSG_IND                 },
            uecc_fsm_erb_modify_s1ap_reset_msg_ind_handler                 },
    { { UECC_LAST_ID                            },  PNULL                  }
};

static uecc_fsm_handler_record_t uecc_fsm_erb_modify_p_mht[] =
{
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND },
            uecc_fsm_erb_modify_rrm_ue_release_req_handler       },
     { { UECC_EV_CSC_DELETE_ALL_UE_REQ        },
            uecc_fsm_erb_modify_csc_cell_delete_req_handler      },
    { { UECC_EV_HO_PREP_TIMER_EXPIRY          },
            uecc_fsm_erb_modify_rrm_ho_prep_timer_expiry_handler },
    /*SPR 18758 Fix Start*/
    /* Code Deleted */
    /*SPR 18758 Fix Stop*/
    { { UECC_EV_RRM_ERAB_RELEASE_IND          },
            uecc_fsm_erb_modify_erab_release_ind_handler         },
    /*SPR_18241_START*/
    /* Code Removed */
    /*SPR_18241_END*/
    { { UECC_LAST_ID                          }, PNULL           }
};

/* Activity states */
static uecc_fsm_activity_state_t 
    uecc_fsm_erb_modify_rrm_resource_modification_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_ERB_MODIFY_RRM_RESOURCE_MODIFICATION_ONGOING",
   /* .s_id           =  */ (S8*)"ERAB_MOD_RRM",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ 
    uecc_fsm_erb_modify_rrm_resource_modification_ongoing_mht
};

/*SPR 22036 Fix Start*/
static uecc_fsm_activity_state_t uecc_fsm_rm_llim_erabmod_del_reconfigure_ue_ongoing =
{
   /* .s_name         = */  (S8*)"UECC_LLIM_ERBMOD_DEL_RECONFIGURE_UE_ONGOING",
   /* .s_id           = */  (S8*)"ERAB_MOD_RRM",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */ uecc_fsm_rm_llim_erabmod_del_reconfigure_ue_ongoing_mht
};
/*SPR 22036 Fix Stop*/

static uecc_fsm_activity_state_t 
    uecc_fsm_erb_modify_llim_reconfigure_ue_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_ERB_MODIFY_LLIM_RECONFIGURE_UE_ONGOING",
   /* .s_id           =  */ (S8*)"ERAB_MOD_LLIM",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ 
    uecc_fsm_erb_modify_llim_reconfigure_ue_ongoing_mht
};

static uecc_fsm_activity_state_t 
    uecc_fsm_erb_modify_ue_reconfigure_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_ERB_MODIFY_UE_RECONFIGURE_ONGOING",
   /* .s_id           =  */ (S8*)"ERAB_MOD_RRC",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ 
    uecc_fsm_erb_modify_ue_reconfigure_ongoing_mht
};

static uecc_fsm_activity_state_t 
    uecc_fsm_erb_modify_llim_rollback_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_ERB_MODIFY_LLIM_ROLLBACK_ONGOING",
   /* .s_id           =  */ (S8*)"ERAB_MOD_RRC",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ 
    uecc_fsm_erb_modify_llim_rollback_ongoing_mht
};

/* Activity */
static uecc_fsm_activity_t uecc_fsm_erb_modify_activity =
{
  /*  .s_name            = */(S8*)"E-RAB MODIFY",
  /* .s_id               = */(S8*)"UE_ERAB_MODIFY_A",
  /* .p_first_state      = */
   &uecc_fsm_erb_modify_rrm_resource_modification_ongoing
};

static uecc_fsm_activity_t*
                    uecc_fsm_erb_modify_p_activity_list[] =
{
    &uecc_fsm_erb_modify_activity,
    PNULL
};

/* Procedure */
uecc_fsm_procedure_t uecc_fsm_erb_modify_procedure =
{
    /*.s_name             = */(S8*)"E-RAB Modify",
    /*.s_id               = */(S8*)"UE_ERABMOD_P",
    /*.constructor_f      = */uecc_fsm_erb_modify_p_constructor,
    /*.finish_success_f   = */uecc_fsm_erb_modify_p_success_handler,
    /*.finish_fail_f      = */uecc_fsm_erb_modify_p_failure_handler,
    /*.activity_list      = */uecc_fsm_erb_modify_p_activity_list,
    /*.p_mht              = */uecc_fsm_erb_modify_p_mht
};

/* Fail actions */
/* ERAB Modify failure actions lists */

static const uecc_fsm_fail_handler_f uecc_fsm_erb_modify_fail_type1_actions[] =
{
    uecc_fsm_erb_mod_fail_action_send_fail_resp_to_s1ap,
    uecc_fsm_erb_mod_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_erb_modify_fail_type2_actions[] =
{
    uecc_fsm_erb_modify_fail_action_send_rrm_cnf,
    uecc_fsm_erb_mod_fail_action_send_fail_resp_to_s1ap,
    uecc_fsm_erb_mod_fail_action_send_ue_context_release_req,
    uecc_fsm_erb_mod_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_erb_modify_fail_type3_actions[] =
{
    uecc_fsm_erb_modify_fail_action_send_rrm_cnf,
    uecc_fsm_erb_mod_fail_action_send_fail_resp_to_s1ap,
    uecc_fsm_erb_mod_fail_action_last,
    PNULL
};


/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/
/*SPR 22036 Fix Start*/

/****************************************************************************
 * Function Name  :
 *      uecc_fsm_erab_modify_ongoing_s1ap_reset_msg_handler
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                       UECC_EV_S1AP_RESET_MSG_IND
 ****************************************************************************/
void uecc_fsm_erab_modify_ongoing_s1ap_reset_msg_handler(
		uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
		void*               p_api                  /* Input API buffer */
		)
{
	s1ap_Cause cause;
	U32 error_code;

	RRC_ASSERT(PNULL != p_ue_context);    
	RRC_ASSERT(PNULL != p_api);

	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context, 
			UECC_ERAB_MODIFY_GUARD_TIMER,
			&p_ue_context->timers_data);


	cause.t = T_s1ap_Cause_transport;
	cause.u.radioNetwork = s1ap_transport_resource_unavailable;

	/* SPR 22310 CID 115954 Fix Start */
	error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
	RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);
	/* SPR 22310 CID 115954 Fix End */

	/* Update erab release list to erab failed to release list */
	uecc_fsm_erabmod_update_erab_release_list_with_error_code(
			p_ue_context,
			&error_code);

	uecc_update_failed_list_from_to_be_modified_list(
			p_ue_context, 
			cause,
			error_code);

	/* Build and send RRC_RRM_ERB_MODIFY_CNF with success to RRM */
	if(RRC_FAILURE == uecc_rrm_build_and_send_erb_modify_conf(
				p_ue_context, 
				FAILURE,
				error_code))
	{
		RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
				"[RRC_RRM_ERB_MODIFY_CNF] Message Delivery failure");
	}

	/* Send event to UE Main FSM */
	uecc_event_queue_push_event(
			UECC_EV_S1AP_RESET_MSG_IND_INT,
			p_ue_context,
			p_api);

	if (PNULL != p_ue_context->p_curr_proc_data)
	{
		/* Free the data for current procedure in the UE Context */
		rrc_mem_free(p_ue_context->p_curr_proc_data);
		p_ue_context->p_curr_proc_data = PNULL;
	}

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
 * Function Name  : uecc_fsm_erabmod_erab_release_fail_handler
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
void uecc_fsm_erabmod_erab_release_fail_handler(
		uecc_ue_context_t*    p_ue_context,
		U32                   err_code,
		uecc_fsm_fail_handler_f const*  p_fail_actions_list
		)
{
    U32      error_code = RRC_NULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    error_code = RRC_MODULE_ID << 16;
    error_code |= err_code;

    /* Update erab release list to erab failed to release list */
    uecc_fsm_erabmod_update_erab_release_list_with_error_code(
            p_ue_context,
            &error_code);

    /* Put all E_RAB's in failed list from ERAB Modify List */
    uecc_fsm_erbmod_fail_action_mark_all_erab_failed(
            p_ue_context,
            err_code);

    /* Start failure processing */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            p_fail_actions_list);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * Function Name  :uecc_fsm_erabmod_update_erab_release_list_with_error_code
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
void uecc_fsm_erabmod_update_erab_release_list_with_error_code(
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
	memset_wrapper(&cause, RRC_NULL, sizeof(s1ap_Cause));

	p_erab_rel_list = &p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
		erab_release_command_data.erab_release_list;

	p_erab_rel_failed_to_list = &p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
		erab_release_command_data.erab_failed_to_release_list;

	/* update by this s1ap_Cause */
	error_code = *(U32*)p_error_code;

    uecc_llim_build_s1ap_cause(p_ue_context->p_gb_context ,
	&cause,
	(rrc_response_t)error_code);

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
 * :uecc_fsm_erb_modify_llim_del_reconfig_ue_rrc_reestablish_request_handler
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
void uecc_fsm_erb_modify_llim_del_reconfig_ue_rrc_reestablish_request_handler(
		uecc_ue_context_t*    p_ue_context,
		void*                 p_api
		)
{

	U32      error_code = RRC_NULL;
	uecc_fsm_fail_handler_f const*  p_fail_actions_list =
		uecc_fsm_erb_modify_fail_type2_actions;

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


	error_code = RRC_MODULE_ID << 16;
	error_code |= RRC_REESTABLISHMENT_TRIGGERED;

	/* Update erab release list to erab failed to release list */
    uecc_fsm_erabmod_update_erab_release_list_with_error_code(
            p_ue_context,
            &error_code);

	/* Put all E_RAB's in failed list from ERAB Modify List */
    uecc_fsm_erbmod_fail_action_mark_all_erab_failed(
            p_ue_context,
            RRC_REESTABLISHMENT_TRIGGERED);

	if (RRC_SUCCESS !=
			uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
				p_ue_context->p_gb_context, p_ue_context->crnti,
				p_ue_context->reestablishmentCause,
				p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
	{
		RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
				"Fail to send the RRCConnectionReestablishmentReject to UE;Unused variable p_api=%p"
                ,p_api);
	}
    /* enabling flag */
    SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);


	/* Start failure processing */
	uecc_fsm_start_failure_processing(
			p_ue_context,
			p_fail_actions_list);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_erb_modify_llim_del_ue_rel_req_handler 
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
void uecc_fsm_erb_modify_llim_del_ue_rel_req_handler(
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

    error_code = RRC_MODULE_ID << 16;
    error_code |= RRC_INTERACTION_WITH_OTHER_PROCEDURE;

    /* Update erab release list to erab failed to release list */
    uecc_fsm_erabmod_update_erab_release_list_with_error_code(
            p_ue_context,
            &error_code);

    /* Put all E_RAB's in failed list from ERAB Modify List */
    uecc_fsm_erbmod_fail_action_mark_all_erab_failed(
            p_ue_context,
            RRC_INTERACTION_WITH_OTHER_PROCEDURE);

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,
            UECC_ERAB_MODIFY_GUARD_TIMER,
            &p_ue_context->timers_data);


    /* Send RRC_RRM_ERB_MODIFY_CNF(FAILURE) to RRM */
    if (RRC_SUCCESS !=uecc_rrm_build_and_send_erb_modify_conf 
            (p_ue_context,
             FAILURE,error_code))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,
                "FAILURE: Send RRC_RRM_ERB_SETUP_CNF (Fail)");
    }

    /* Send ASN.1 Encoded ERAB MODIFY Response to MME */
    if(RRC_SUCCESS !=
            uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL,
                "[s1ap_E_RABSetup] delivery failure");
    }

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Free the data for current procedure in the UE Context */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }
    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    uecc_event_queue_push_event(
            UECC_EV_UE_RELEASE_REQ_INT,
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name: uecc_fsm_erb_modify_llim_del_radio_link_failure_handler
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
void uecc_fsm_erb_modify_llim_del_radio_link_failure_handler(
		uecc_ue_context_t*    p_ue_context,
		void*                 p_api
		)
{
    U32             error_code = RRC_NULL;
    uecc_fsm_fail_handler_f const*  p_fail_actions_list =
        uecc_fsm_erb_modify_fail_type2_actions;

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,
            "UECC_EV_RADIO_LINK_FAILURE_IND");

    error_code = RRC_MODULE_ID << 16;
    error_code |= RRC_RADIO_LINK_FAILURE_TRIGGERED;

    /* Update erab release list to erab failed to release list */
    uecc_fsm_erabmod_update_erab_release_list_with_error_code(
            p_ue_context,
            &error_code);

    /* Put all E_RAB's in failed list from ERAB Modify List */
    uecc_fsm_erbmod_fail_action_mark_all_erab_failed(
            p_ue_context,
            RRC_RADIO_LINK_FAILURE_TRIGGERED);

    /* Start failure processing */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            p_fail_actions_list);

    /* enabling flag */
    SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * FUNCTION NAME :
 * uecc_fsm_erb_modify_llim_del_reconf_ue_ongoing_timer_expiry_handler 
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   :    This is the handler function for the message
*                     UECC_EV_TIMER_EXPIRY on a time out from LLIM.
 ****************************************************************************/
void 
uecc_fsm_erb_modify_llim_del_reconf_ue_ongoing_timer_expiry_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
	U32      error_code = RRC_NULL;

    uecc_fsm_fail_handler_f const*  p_fail_actions_list =
                                  uecc_fsm_erb_modify_fail_type2_actions;

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	error_code = RRC_MODULE_ID << 16;
	error_code |= RRC_TIMER_EXPIRY;

	/* Update erab release list to erab failed to release list */
    uecc_fsm_erabmod_update_erab_release_list_with_error_code(
            p_ue_context,
            &error_code);

    /* Put all E_RAB's in failed list from ERAB Modify List */
    uecc_fsm_erbmod_fail_action_mark_all_erab_failed(
            p_ue_context,
            RRC_TIMER_EXPIRY);

/* Start failure processing */
	uecc_fsm_start_failure_processing(
			p_ue_context,
			p_fail_actions_list);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : update_curr_proc_erbmod_data_with_response_code
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
void update_curr_proc_erbmod_data_with_response_code(
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

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO, "[CURR PROC DATA UPDATED]:"
            "By DELETE LC RESP");

    p_rrc_uecc_llim_delete_lc_resp =
        (rrc_uecc_llim_delete_lc_resp_t*)p_response_code;
    p_erab_list = &p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
        erab_release_command_data.erab_release_list;
    p_erab_list->counter = RRC_NULL;

    p_erab_failed_list = &p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
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
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                    "[UECC_EV_LLIM_DELETE_LC_RESP]:Invalid LC Id [%i]",
                    p_rrc_uecc_llim_delete_lc_resp->
                    delete_drb_entity_resp[resp_counter].lc_id);
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_erab_modify_deallocate_erab
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  U8 erab_id - E_RAB ID
 * Outputs        : None
 * Returns        : None
 * Description    : This dellocates E_RAB from UE Context
 ****************************************************************************/
void uecc_fsm_ue_erab_modify_deallocate_erab(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    U8                  erab_id                 /* E_RAB ID */
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (p_ue_context->p_e_rab_list[erab_id] != PNULL)
    {
        rrc_mem_free(p_ue_context->p_e_rab_list[erab_id]);
        p_ue_context->p_e_rab_list[erab_id] = PNULL;
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,
                "E_RAB ID : %u context deleted from UE Context", erab_id);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_erbmod_fail_action_mark_all_erab_failed
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  U32                 error_code
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion populate failed list from success list
 ****************************************************************************/
void uecc_fsm_erbmod_fail_action_mark_all_erab_failed(
    uecc_ue_context_t*  p_ue_context,
    U32                 error_code
)
{
    U8 fail_list_count = 0;
    U8 succ_count = 0;
    U8 succ_list_counter = 0;

    erab_modify_list_t            *p_erab_modify_list =
        &p_ue_context->p_curr_proc_data->u.erab_modify_request_data.erab_modify_list;

    erab_failed_to_modify_list_t  *p_erab_failed_to_modify_list =
       &p_ue_context->p_curr_proc_data->u.erab_modify_request_data.erab_failed_to_modify_list;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    succ_count = p_erab_modify_list->erab_modify_list_counter;

    fail_list_count = p_erab_failed_to_modify_list->erab_failed_to_modify_list_counter;

    /* ADD RRC Module ID in error code */
    RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);

    for (succ_list_counter = 0;
         succ_list_counter < succ_count;
         succ_list_counter++)
    {
            /* Set status to error_code in E_RAB Modify List */
            p_erab_modify_list->erab_modify_item[succ_list_counter].status =
                RRC_FAILURE;

            /* Copy E_RAB ID to Failed List */
            p_erab_failed_to_modify_list->erab_failed_to_modify_item[fail_list_count]
                .e_RAB_ID = p_erab_modify_list->erab_modify_item[succ_list_counter].drb_config.erab_id;

            p_erab_failed_to_modify_list->erab_failed_to_modify_item[fail_list_count].failed_at = FAILED_AT_LL;
            p_erab_failed_to_modify_list->erab_failed_to_modify_item[fail_list_count].error_code = error_code; 
            /* Map Cause in Fail List */
            uecc_llim_build_s1ap_cause(
                p_ue_context->p_gb_context,
                &p_erab_failed_to_modify_list->erab_failed_to_modify_item
                [fail_list_count].cause,
                (rrc_response_t)error_code);

            /* Delete Failed ERAB from UE CONTEXT */
            uecc_fsm_ue_erab_modify_deallocate_erab(
                p_ue_context,
                p_erab_modify_list->erab_modify_item[succ_list_counter].drb_config.erab_id);

            fail_list_count++;
    }

    /* Copy Incremented fail list count */
    p_erab_failed_to_modify_list->erab_failed_to_modify_list_counter = fail_list_count;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  :uecc_fsm_erb_modify_llim_del_reconfigure_ue_resp_handler
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
void uecc_fsm_erb_modify_llim_del_reconfigure_ue_resp_handler(
        uecc_ue_context_t*    p_ue_context,
        void*                 p_api
        )
{
    rrc_uecc_llim_delete_lc_resp_t*  p_rrc_uecc_llim_delete_lc_resp = PNULL;
    rrc_return_t   result = RRC_FAILURE;
    uecc_fsm_fail_handler_f const*  p_fail_actions_list =
                                  uecc_fsm_erb_modify_fail_type2_actions;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,
            "UECC_EV_LLIM_RECONFIGURE_UE_RESP");

    p_rrc_uecc_llim_delete_lc_resp = (rrc_uecc_llim_delete_lc_resp_t*)p_api;

    do{
        /* Check whether UECC_EV_LLIM_RECONFIGURE_UE_RESP
         * received with success or failure or partial success */
        if (SUCCESS == p_rrc_uecc_llim_delete_lc_resp->
                response_code)
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,
                    "[UECC_EV_LLIM_DELETE_LC_RESP] Response Code =%i",
                    p_rrc_uecc_llim_delete_lc_resp->response_code);

            /* Switch  state */
            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                    &uecc_fsm_erb_modify_llim_reconfigure_ue_ongoing, PNULL);
            result = RRC_SUCCESS;
        }
        else if ((PARTIAL_SUCCESS ==
                    p_rrc_uecc_llim_delete_lc_resp->response_code) ||
                (FAILURE ==
                 p_rrc_uecc_llim_delete_lc_resp->response_code) ||
                L3_FATAL ==
                p_rrc_uecc_llim_delete_lc_resp->response_code)
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,
                    "[UECC_EV_LLIM_DELETE_LC_RESP] Response Code =%i",
                    p_rrc_uecc_llim_delete_lc_resp->response_code);

            /*update curr proc data with rrc_uecc_llim_delete_lc_resp */
            update_curr_proc_erbmod_data_with_response_code(p_ue_context,
                    p_rrc_uecc_llim_delete_lc_resp);


            /* Put all E_RAB's in failed list from ERAB Modify List */
            uecc_fsm_erbmod_fail_action_mark_all_erab_failed(
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
* Function Name  : fill_erab_release_list_arp_change
* Returns        : RRC_SUCCESS/RRC_FAILURE
* Description    : This action routine is called when FSM receives the
*                  UECC_EV_STATE_INIT message.
****************************************************************************/
static rrc_return_t fill_erab_release_list_arp_change_in_mod(
        uecc_ue_context_t  *p_ue_context,
        rrc_rrm_erab_modify_resp_t *p_rrc_rrm_erb_modify_resp
        )
{
    U8          e_rab_index  = RRC_NULL;
    U8          erab_rel_list_count = RRC_NULL;
    U8          erab_rel_fail_list_count = RRC_NULL;
    erab_list_t *p_erab_list = PNULL;
    erab_list_t *p_erab_failed_list = PNULL;
    rrc_return_t result = RRC_SUCCESS;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rrc_rrm_erb_modify_resp);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (RRM_ERB_MODIFY_RESP_API_ERAB_TO_BE_RELEASED_ITEM_LIST_PRESENT &
            p_rrc_rrm_erb_modify_resp->bitmask)
    {
        do {
            /* points to erab_to_be_released_list in erab_release_command_data */
            p_erab_list = &p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
                erab_release_command_data.erab_release_list;
            p_erab_list->counter = p_rrc_rrm_erb_modify_resp->erab_to_be_released_item_list.
                count;

            /* points to erab_failed_to_release_list */
            p_erab_failed_list = &p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
                erab_release_command_data.erab_failed_to_release_list;

            for (e_rab_index = 0;
                    (e_rab_index < p_rrc_rrm_erb_modify_resp->erab_to_be_released_item_list.count) && 
                    (e_rab_index < MAX_ERAB_COUNT);
                    e_rab_index++)
            {
                if ((p_rrc_rrm_erb_modify_resp->erab_to_be_released_item_list.
                         erab_to_be_release_item[e_rab_index].erab_id < MAX_ERAB_COUNT) &&
                         (PNULL != p_ue_context->p_e_rab_list[p_rrc_rrm_erb_modify_resp->erab_to_be_released_item_list.
                        erab_to_be_release_item[e_rab_index].erab_id]))
		{
			/* SPR 22310 CID 115900 Fix Start */
			if(RRC_SUCCESS == uecc_s1ap_build_s1ap_cause(
						&p_erab_list->erab_item[erab_rel_list_count].cause,
						&p_rrc_rrm_erb_modify_resp->erab_to_be_released_item_list.
						erab_to_be_release_item[e_rab_index].cause,
						p_ue_context))
			{
				/* Ignoring return value */  
			}
			p_erab_list->erab_item[erab_rel_list_count++].e_RAB_ID =
				p_rrc_rrm_erb_modify_resp->erab_to_be_released_item_list.
				erab_to_be_release_item[e_rab_index].erab_id;
			/* SPR 22310 CID 115900 Fix End */
		}
                else
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,
                            RRC_WARNING,"Unknown E-RAB ID = [%u]",
                            p_rrc_rrm_erb_modify_resp->erab_to_be_released_item_list.
                            erab_to_be_release_item[e_rab_index].erab_id);
                    p_erab_failed_list->erab_item[erab_rel_fail_list_count].
                        e_RAB_ID = p_rrc_rrm_erb_modify_resp->erab_to_be_released_item_list.
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
                p_ue_context->p_curr_proc_data->u.erab_modify_request_data.is_erab_del_ongoing =
                    RRC_TRUE;
            }
            result = RRC_SUCCESS;
        } while(0);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}
/*SPR 22036 Fix Stop*/

/****************************************************************************
 * FUNCTION NAME : fill_erabModify_sps_meas_timer_info 
 *
 * DESCRIPTION   : fill sps, measuremnt, timer and
 *                  constant info in modify request list and ue context
 *                  from the rrm_erab_modify_response message
 * RETURNS       :  RRC_FAILURE/RRC_SUCCESS
 *
 ****************************************************************************/
rrc_return_t fill_erabModify_sps_meas_timer_info( 
        uecc_ue_context_t*  p_ue_context,
        rrc_rrm_erab_modify_resp_t* p_rrm_erb_modify_resp,
        erab_modify_request_data_t   *p_mod_req_data 
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rrm_erb_modify_resp);
    RRC_ASSERT(PNULL != p_mod_req_data);
    
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    /*** TDD changes start ***/
    /* Store sps_config if present */
    if (RRM_ERAB_MODIFY_RADIO_RES_CONFIG_SPS_CONFIG_PRESENT &
            p_rrm_erb_modify_resp->radio_rsource_config.bitmask )
    {
        /* SPS Start */
        if (RRC_FAILURE == uecc_rrm_parse_and_update_sps_config(
                    p_ue_context,
                    &p_rrm_erb_modify_resp->radio_rsource_config.sps_config))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[RRC_RRM_ERB_MODIFY_RESP] SPS config updation failed");
        }
        else
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[RRC_RRM_ERB_MODIFY_RESP] SPS config updation successful");
        }
    /* SPS Stop */
    }
    /*** TDD changes stop ***/



    /* Store rrm_meas_config_t if present */
    if (RRM_ERAB_MODIFY_RADIO_RES_CONFIG_MEAS_CONFIG_PRESENT &
            p_rrm_erb_modify_resp->radio_rsource_config.bitmask )
    {
        p_mod_req_data->m.meas_config_updated = 1;
        p_mod_req_data->meas_config =
            p_rrm_erb_modify_resp->radio_rsource_config.meas_config;

    }
    /* CR_220 : 
       if rlf_timers_and_constants_r9 were not received Previously by UE,
       then, Store RLF Timers & Constants from rrc_rrm_erb_modify_resp data
       from RRM into UE Context */

    if (RRM_ERAB_MODIFY_RADIO_RES_CONFIG_RLF_TIMERS_AND_CONSTANTS_PRESENT &
            p_rrm_erb_modify_resp->radio_rsource_config.bitmask )
    {

        p_ue_context->m.rlf_timers_and_constants_r9_present = 1;

	    if(RRC_NULL == p_ue_context->p_rlf_timers_and_constants_r9) 
        {
            p_ue_context->p_rlf_timers_and_constants_r9 =
                (rlf_timers_and_constants_r9_t *)rrc_mem_get(sizeof(rlf_timers_and_constants_r9_t)); 
        }
        if (PNULL == p_ue_context->p_rlf_timers_and_constants_r9)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_ERROR,"Unable to allocate Memory");
            return RRC_FAILURE;
        }

        if( RLF_TIMERS_AND_CONSTANTS_SETUP_PRESENT & p_rrm_erb_modify_resp->radio_rsource_config.rlf_timers_and_constants_r9.bitmask)
        {
            p_ue_context->p_rlf_timers_and_constants_r9->bitmask = RLF_TIMERS_AND_CONSTANTS_SETUP_PRESENT;


            p_ue_context->p_rlf_timers_and_constants_r9->setup.t301_r9 =
                p_rrm_erb_modify_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.t301_r9;

            p_ue_context->p_rlf_timers_and_constants_r9->setup.t310_r9 =
                p_rrm_erb_modify_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.t310_r9;

            p_ue_context->p_rlf_timers_and_constants_r9->setup.n310_r9 =
                p_rrm_erb_modify_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.n310_r9;

            p_ue_context->p_rlf_timers_and_constants_r9->setup.t311_r9 =
                p_rrm_erb_modify_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.t311_r9;

            p_ue_context->p_rlf_timers_and_constants_r9->setup.n311_r9 =
                p_rrm_erb_modify_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.n311_r9;
        }
		else
		{
			p_ue_context->p_rlf_timers_and_constants_r9->bitmask = 0;
		}
    }  
    /* Copy Proximity Configuration */

    if (p_rrm_erb_modify_resp->radio_rsource_config.bitmask &
            RRM_ERAB_MODIFY_RADIO_RES_CONFIG_PROXIMITY_PRESENT )
    {
        p_mod_req_data->m.proximity_config_present = 1;
        l3_memcpy_wrapper(&(p_mod_req_data->proximity_config),
                &(p_rrm_erb_modify_resp->radio_rsource_config.proximity_config),
                sizeof(rrm_report_proximity_config_r9_t));

    }
    else
    {
        p_mod_req_data->m.proximity_config_present = 0;
        memset_wrapper(&(p_mod_req_data->proximity_config), RRC_NULL,
                sizeof(rrm_report_proximity_config_r9_t));
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}


/****************************************************************************
 * FUNCTION NAME : prepare_erab_modify_list 
 *
 * DESCRIPTION   : prepare erab modify and delete-failed list
 *                  from the rrm_erab_modify_response message
 * RETURNS       :  RRC_FAILURE/RRC_SUCCESS
 *
 ****************************************************************************/
static rrc_return_t prepare_erab_modify_list( uecc_ue_context_t  *p_ue_context,
        rrc_rrm_erab_modify_resp_t      *p_rrm_erb_modify_resp,
        erab_to_be_modified_list_t      *p_erab_to_be_mod_list ,
        erab_failed_to_modify_list_t    *p_failed_list ,
	    s1ap_Cause   			*cause,
        U32                             *count
)
{
    U32          sub_frame_assignment = RRC_NULL;
    U8           drb_index = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rrm_erb_modify_resp);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Save the failure cases in the failure list, if any  */
    if (RRM_ERAB_MODIFY_RADIO_RES_CONFIG_DRB_FAILED_TO_MODIFY_LIST_PRESENT &
            p_rrm_erb_modify_resp->radio_rsource_config.bitmask)
    { 
        if (p_rrm_erb_modify_resp->radio_rsource_config.
                drb_failed_to_modify_list.count != 0)
        {
            if (p_rrm_erb_modify_resp->response == SUCCESS)
            {
                return RRC_FAILURE;
            }
            else if (p_rrm_erb_modify_resp->response == PARTIAL_SUCCESS)
            {
                (*count) +=  p_rrm_erb_modify_resp->radio_rsource_config.
                    drb_failed_to_modify_list.count; 
                if ((*count) != p_erab_to_be_mod_list->erab_to_be_modified_list_counter)
                {
                    return RRC_FAILURE;
                }
            }
            for (drb_index =0;
                    ((drb_index < p_rrm_erb_modify_resp->radio_rsource_config.
                      drb_failed_to_modify_list.count) 
                     && (drb_index < MAX_ERAB_COUNT));
                    drb_index++)
            {
                p_failed_list->erab_failed_to_modify_item[p_failed_list->erab_failed_to_modify_list_counter].failed_at = FAILED_AT_RRM;
                p_failed_list->erab_failed_to_modify_item[p_failed_list->
                    erab_failed_to_modify_list_counter++].e_RAB_ID =
                    p_rrm_erb_modify_resp->radio_rsource_config.
                    drb_failed_to_modify_list.drb_failed_to_add[drb_index].erab_id;

                if (RRC_SUCCESS == uecc_s1ap_build_s1ap_cause(
                            cause,
                            &(p_rrm_erb_modify_resp->radio_rsource_config.
                                drb_failed_to_modify_list.
                                drb_failed_to_add[drb_index].cause),
                            p_ue_context))
                {
                    l3_memcpy_wrapper(&(p_failed_list->erab_failed_to_modify_item[drb_index].cause),
                            cause,
                            sizeof(s1ap_Cause));
                }
            }
        }
    }
    else if ((p_rrm_erb_modify_resp->response == PARTIAL_SUCCESS) &&
            (!(RRM_ERAB_MODIFY_RADIO_RES_CONFIG_DRB_FAILED_TO_MODIFY_LIST_PRESENT &
               p_rrm_erb_modify_resp->radio_rsource_config.bitmask)))
    {
        return RRC_FAILURE;
    }
    /* Store mac_main_config_t if present */
    if (RRM_ERAB_MODIFY_RADIO_RES_CONFIG_MAC_CONFIG_PRESENT &
            p_rrm_erb_modify_resp->radio_rsource_config.bitmask )
    {
        if (p_rrm_erb_modify_resp->radio_rsource_config.mac_config.bitmask &
                RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT )
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,p_ue_context->
                    p_gb_context->facility_name, RRC_INFO,
                    "ERAB Modify :mac_main_config received from RRM");

            if (PNULL != p_ue_context->p_gb_context->p_p_csc_context)
            {
                if (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                        [p_ue_context->cell_index])
                {
                    if (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                            [p_ue_context->cell_index]->p_csc_init_setup_ind)
                    {
                        /*** TDD changes start ***/
                        if(DUPLEXING_MODE_TDD == p_ue_context->p_gb_context->
                                p_p_csc_context[p_ue_context->cell_index]->
                                p_csc_init_setup_ind->duplexing_mode)
                        {
                            sub_frame_assignment = p_ue_context->p_gb_context->
                                p_p_csc_context[p_ue_context->cell_index]->p_csc_init_setup_ind->
                                sib_type_1_Info.tdd_config.
                                sub_frame_assignment;

                            if (RRC_TRUE == p_rrm_erb_modify_resp->radio_rsource_config.
                                    mac_config.mac_main_config.ul_sch_config.
                                    tti_bundling)
                            {
                                /* TTI bundling can be enabled for FDD and for TDD
                                 * only for configurations 0, 1 and 6 */
                                if (sa_0 != sub_frame_assignment &&
                                        sa_1 != sub_frame_assignment &&
                                        sa_6 != sub_frame_assignment)
                                {
                                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                                            p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                                            "ERAB modify: TTI bundling can be enabled for FDD " \
                                            "and for TDD only for configurations 0, 1 and 6");

                                    return RRC_FAILURE;
                                }
                                if (RRM_ERAB_MODIFY_RADIO_RES_CONFIG_SPS_CONFIG_PRESENT &
                                        p_rrm_erb_modify_resp->radio_rsource_config.bitmask )
                                {
                                    /*For TDD, E-UTRAN does not simultaneously enable TTI
                                     * bundling and semi-persistent scheduling */
                                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                                            p_ue_context->p_gb_context->facility_name,
                                            RRC_WARNING,"ERAB modify: For TDD," \
                                            " E-UTRAN does not simultaneously enable TTI"\
                                            " bundling and semi-persistent scheduling");
                                    return RRC_FAILURE;
                                }
                            }
                        }
                        /*** TDD changes stop ***/
                    }
                }
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_p_constructor
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : Constructor for UE E-RAB Modify procedure
 *                 -Updates the UE context with information received in the 
 *                  E-RAB MODFIY REQUEST message from S1AP.
 *                 -Starts Procedural Timer
 *                 -Activates UE Activity
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_p_constructor(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{

    rrc_s1ap_erab_modify_request_t *p_msg = 
                    (rrc_s1ap_erab_modify_request_t*)p_api;
    
    rrc_return_t    result = RRC_FAILURE;

    /*SPR 18851 FIX START */
    U8 count = RRC_NULL;   
    /*SPR 18851 FIX END */
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

#ifdef RRC_UECC_TR_ID_CONST
        uecc_ue_ctx_set_op_transction_id(p_ue_context, 1); 
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name, 
                RRC_INFO,"ERAB Modify: Transaction Id updated");
#else
        uecc_ue_ctx_set_op_transction_id(p_ue_context,
                uecc_ue_ctx_get_next_transction_id(p_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/
    /* Allocate memory for the p_curr_proc_data for the E-RAB procedure */
    p_ue_context->p_curr_proc_data = (curr_proc_data_t *) 
                                     rrc_mem_get(sizeof(curr_proc_data_t));
    
    if (p_ue_context->p_curr_proc_data == PNULL)
    {
        /* Memory Allocation Failed */
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR, 
                "Memory Allocation Failure");     
        RRC_ASSERT(PNULL != p_ue_context->p_curr_proc_data);
        return;
    }

    /* SES-508 Fix Start */
    /* Code Removed */
    /* SES-508 Fix End */

    /*SPR 18851 FIX START */
    for(count = 0; count < MAX_ERAB_COUNT; count++)
    {    
	    p_ue_context->p_curr_proc_data->u.erab_modify_request_data.erab_to_be_modified_list.
		    erab_to_be_modified_item[count].e_RAB_ID = INVALID_ERAB_ID;
    }    
    /*SPR 18851 FIX END */

    /* Update the UE Context */
    do 
    {
        if (RRC_SUCCESS == uecc_fsm_process_s1ap_erab_modify_msg(
                                                      p_ue_context, 
                                                      p_msg))
        {
            /* Start timer*/
            if ( RRC_SUCCESS != uecc_ue_timer_start(
                                    p_ue_context->p_gb_context, 
                                    UECC_ERAB_MODIFY_GUARD_TIMER,
                                    &p_ue_context->p_gb_context->ue_timers_data,
                                    &p_ue_context->timers_data,
                                    (rrc_ue_index_t)p_ue_context->ue_index))
            {
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                                          "[UECC_ERAB_MODIFY_GUARD_TIMER]"
                                          "Timer Start Failed");
                break;
            }

            result = RRC_SUCCESS;
            /* Activate the first Activity now*/
        }
    } while (0);

    if (RRC_SUCCESS != result)
    {
        /* Build and send the RRC_S1AP_ERAB_MODIFY_RESP msg to S1AP */
        if (RRC_SUCCESS !=
              uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                    "[s1ap_E_RABModifyResponse] Delivery failure");
        }
        /* Process failure */
        uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_S1AP_ERAB_MODIFY_RESP_FAILURE_INT);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_p_success_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 
 * DESCRIPTION   :  Success handler for UE E-RAB Modify procedure
 *
  ****************************************************************************/
void uecc_fsm_erb_modify_p_success_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* Clear update configuration IEs flags */
    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.sps_config_updated = 0;
    p_ue_context->m.mac_config_updated = 0;
    p_ue_context->m.srb1_config_updated = 0;
    p_ue_context->m.srb2_config_updated = 0;
  
    /*SPR_19066_START*/
    p_ue_context->m.ue_inactive_time_updated = RRC_FALSE;
    /*SPR_19066_END*/

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
    /*SPR 22036 Fix Start*/
    if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        p_ue_context->p_curr_proc_data->u.erab_modify_request_data.is_erab_del_ongoing = RRC_FALSE;
    }
    /*SPR 22036 Fix Stop*/


    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_S1AP_ERAB_MODIFY_RESP_SUCCESS_INT,
        p_ue_context,
        p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_p_failure_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   :  Failure handler for UE E-RAB Modify procedure
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_p_failure_handler(
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
    /* SPS Start */
    if (p_ue_context->m.is_sps_recv_frm_rrm)
    {
        if (p_ue_context->p_sps_config != PNULL)
        {
            rrc_mem_free(p_ue_context->p_sps_config);
            p_ue_context->p_sps_config = PNULL;
        }
        p_ue_context->m.sps_config_present = 0;
        p_ue_context->m.sps_config_updated = 0;
        /* Send to MAC - RRC_MAC_RECONFIG_COMPLETE_IND */
        uecc_llim_build_and_send_mac_reconfig_complete_ind(
                p_ue_context->p_gb_context,
                p_ue_context,
                RRC_FAILURE);
    }
       
    /*SPR 22036 Fix Start*/
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
                is_erab_del_ongoing == RRC_TRUE)
        {
            p_ue_context->p_curr_proc_data->u.erab_modify_request_data.is_erab_del_ongoing = RRC_FALSE;
        }
    }
    /*SPR 22036 Fix Stop*/


    /* SPS Stop */
       
    /*SPR_19066_START*/
    p_ue_context->m.ue_inactive_time_updated = RRC_FALSE;
    /*SPR_19066_END*/

    if ((UECC_EV_S1AP_ERAB_MODIFY_RESP_FAILURE_INT == failure_ev)
        || (UECC_EV_S1AP_ERAB_MODIFY_FAILURE_REL_UE_CTX_INT == failure_ev)
         || (UECC_EV_RRC_RE_ESTABLISH_REQ_INT == failure_ev))
    {
        /* Post event to UE FSM for future processing */
        uecc_event_queue_push_event( failure_ev, p_ue_context, p_api);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_rrm_resource_modification_ongoing_init_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This action routine is called when FSM receives the
 *                 UECC_EV_STATE_INIT message. This function sends the
 *                 RRC_RRM_ERB_MODIFY_REQ to RRM.
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_rrm_resource_modification_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send the RRC_RRM_ERB_MODIFY_REQ to RRM */
    if (RRC_SUCCESS !=uecc_rrm_build_and_send_erb_modify_req(p_ue_context))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[RRC_RRM_ERB_MODIFY_REQ] Message Delivery failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_rrm_erb_modify_resp_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api_data
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_RRM_ERAB_MODIFY_RESP from a RRM.
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_rrm_erb_modify_resp_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    rrc_rrm_erab_modify_resp_t*     p_rrc_rrm_erb_modify_resp = PNULL;

    uecc_fsm_fail_handler_f const*  p_failure_actions_list =
                                    uecc_fsm_erb_modify_fail_type1_actions;

    rrc_return_t                    result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_rrm_erb_modify_resp = (rrc_rrm_erab_modify_resp_t*)p_api_data;

    switch (p_rrc_rrm_erb_modify_resp->response)
    {
        case SUCCESS:
        case PARTIAL_SUCCESS:

            /*SPR 22036 Fix Start*/
            (void)fill_erab_release_list_arp_change_in_mod(p_ue_context,
                    p_rrc_rrm_erb_modify_resp);


            /* Process RRC_RRM_ERB_MODIFY_RESP message data */
            result = uecc_fsm_erab_modify_process_rrm_erab_modify_resp_data(
                        p_ue_context,
                        p_rrc_rrm_erb_modify_resp);

            /*SPR 22036 Fix Stop*/
            break;

        case FAILURE:
            /* RRM Fail to Modify ERAB */
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_ERB_MODIFY_RESP] Failed to Modify E-RABs");
            uecc_update_failure_list(p_ue_context, p_rrc_rrm_erb_modify_resp);
            break;

        default:
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_ERB_MODIFY_RESP] Invalid Response=%i",
                p_rrc_rrm_erb_modify_resp->response);
            break;
    }


    /* Process failure */
    if (RRC_SUCCESS != result)
    {
        /* Stop Procedure Timer */
        uecc_ue_timer_stop(
                        p_ue_context->p_gb_context, 
                        UECC_ERAB_MODIFY_GUARD_TIMER, 
                        &p_ue_context->timers_data);       

        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                                          p_failure_actions_list);
    }
    /*SPR 22036 Fix Start*/
    /* Change Child FSM state */
    else
    {
        if (RRC_TRUE ==
                p_ue_context->p_curr_proc_data->u.erab_modify_request_data.is_erab_del_ongoing)
        {
            UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_rm_llim_erabmod_del_reconfigure_ue_ongoing,
                    PNULL);
        }
        else
        {
            UECC_FSM_SET_CHILD_STATE(p_ue_context,
                    &uecc_fsm_erb_modify_llim_reconfigure_ue_ongoing, PNULL);
        }
    }
    /*SPR 22036 Fix Stop*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * FUNCTION NAME : 
 *        uecc_fsm_erb_modify_rrc_reestablish_request_handler 
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_RRC_RE_ESTABLISH_REQ
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_rrc_reestablish_request_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    s1ap_Cause cause;
    rrc_return_t rrc_response = RRC_SUCCESS;
    U32 error_code = RRC_NO_ERROR;
    U16 len        = RRC_NULL; 

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    
    /*
    ** Stop Procedure Timer
    ** Send the ASN. 1 E-RAB MODIFY RESPONSE message containing 
    ** all failures to MME.
    */
      
    /* Stop Procedure Timer */
    uecc_ue_timer_stop(
                        p_ue_context->p_gb_context,
                        UECC_ERAB_MODIFY_GUARD_TIMER, 
                        &p_ue_context->timers_data);


    if (PNULL == p_ue_context->p_curr_proc_data)
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                "curr_proc_data can't be NULL");
        return;
    }

    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_interaction_with_other_procedure; 
    error_code = RRC_REESTABLISHMENT_TRIGGERED; 
    
    RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);
  
    uecc_update_failed_list_from_to_be_modified_list(
        p_ue_context, 
        cause,
        error_code);

   
    /* Build and send RRC_RRM_ERB_MODIFY_CNF with success to RRM */
    rrc_response = uecc_rrm_build_and_send_erb_modify_conf(
                             p_ue_context, 
                             FAILURE,
                             error_code);

    if (rrc_response != RRC_SUCCESS)
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
            "[RRC_RRM_ERB_MODIFY_CNF] Delivery failure");            
    }

    /* Send the ASN. 1 E-RAB MODIFY RESPONSE message containing 
     * all failures to MME.*/
    if (RRC_SUCCESS !=
          uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[s1ap_E_RABModifyResponse] Delivery failure");
    }


    /* Buffered Received Re-establishment Msg */
    len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
                rrc_mem_get(len)))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL,
                "Memory Allocation Failed to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* Storing re_establishment contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
            p_api,len);

    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RE_ESTABLISH_MSG;

    /* Send event to UE Main FSM and pass API data to FSM. */
    uecc_event_queue_push_event(
        UECC_EV_RRC_RE_ESTABLISH_REQ_INT,
        p_ue_context,
        p_ue_context->p_curr_proc_data->msg_bfr_rcvd); 
    
    /* Free the data for current procedure in the UE Context */

    p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
    rrc_mem_free(p_ue_context->p_curr_proc_data);
    p_ue_context->p_curr_proc_data = PNULL;


    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : 
 *         uecc_fsm_erb_modify_rrm_resource_modify_ongoing_timer_expiry_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_TIMER_EXPIRY.
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_rrm_resource_modify_ongoing_timer_expiry_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    s1ap_Cause cause;
    U32        error_code;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_BRIEF, "Time out.");
    
    cause.t = T_s1ap_Cause_radioNetwork;
    /* SPR 15473 Fix Start */
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;   
    /* SPR 15473 Fix Stop */
    error_code = RRC_TIMER_EXPIRY;
    
    RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);
    
    uecc_update_failed_list_from_to_be_modified_list(
            p_ue_context, 
            cause,
            error_code);


    /* Build and send RRC_RRM_ERB_MODIFY_CNF with success to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_modify_conf(
                             p_ue_context, 
                             FAILURE,
                             error_code))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
            "[RRC_RRM_ERB_MODIFY_CNF] Message Delivery failure");
    }
    
    /* Send the ASN. 1 E-RAB MODIFY RESPONSE message containing 
     * all failures to MME.*/
    if (RRC_SUCCESS !=
          uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[s1ap_E_RABModifyResponse] Delivery failure");
    }    
    
    /* Process failure according to selected failure actions list */
    uecc_fsm_finish_activity_failure(
          p_ue_context, (void*)UECC_EV_S1AP_ERAB_MODIFY_FAILURE_REL_UE_CTX_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_rrm_ho_required_indication_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
*                       UECC_EV_RRM_HO_REQUIRED
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_rrm_ho_required_indication_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    s1ap_Cause cause;
    U32        error_code;

    /* SPR 2532 Start */
    U16     len = RRC_NULL;
    /* SPR 2532 Stop */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* If RRC_RRM_HO_REQUIRED is received from RRM while waiting 
     ** for RRC_RRM_ERB_MODIFY_RESP, stop the erab modify procedure 
     ** and start HO PREP TIMER and initiate the following failure
     ** activity actions:
     **
     ** Send ASN encoded E-RAB MODIFY RESPONSE msg to MME 
     ** containing all failure responses.
     ** Pass the UECC_EV_ERB_ONGOING_HO_REQUIRED_INT event to main 
     ** fsm for Handover Procedure.
     */

    /* Stop Procedure Timer */
    uecc_ue_timer_stop(
            p_ue_context->p_gb_context,
            UECC_ERAB_MODIFY_GUARD_TIMER, 
            &p_ue_context->timers_data);

    cause.t = T_s1ap_Cause_radioNetwork;

    /* SPR 2532 Start */

    /* Buffer the HO REQUIRED message in UE Context. */
    len = (U16)sizeof(rrc_rrm_ho_required_t);

    if(PNULL != p_ue_context->p_curr_proc_data)
    {
        if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
                    rrc_mem_get(len)))
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL,
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
        /* SPR 24021/SES-452 Coverity 209450 Fix Start */
        if(PNULL != p_api)
        {
            l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
                    p_api, len);

            /* store received buffer type */
            p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_HO_REQUIRED_MSG;
        }
        /* SPR 24021/SES-452 Coverity 209450 Fix End */
    }
    /* SPR 2532 Stop */

    /* SPR 24021/SES-452 Coverity 209450 Fix Start */
    if(PNULL != p_api)
    {
        if ((HANDOVER_TYPE_INTRA_LTE_S1 ==
                    ((rrc_rrm_ho_required_t*)p_api)->ho_type) ||
                (HANDOVER_TYPE_INTRA_LTE_X2 ==
                 ((rrc_rrm_ho_required_t*)p_api)->ho_type) ||
                /*BUG 604 changes start*/
                (HANDOVER_TYPE_INTER_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type) ||
                (HANDOVER_TYPE_INTRA_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type))
            /*BUG 604 changes stop*/
        {
            cause.u.radioNetwork =
                s1ap_s1_intra_system_handover_triggered;
        }
        else
        {
            cause.u.radioNetwork =
                s1ap_s1_inter_system_handover_triggered;
        }

        error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;

        RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);

        uecc_update_failed_list_from_to_be_modified_list(
                p_ue_context, 
                cause,
                error_code);

        /* Start HO Prep Timer*/
        /*x2_ho_timer*/
        if (HANDOVER_TYPE_INTRA_LTE_X2 ==
                ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = X2_HO;
            if ( RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context, 
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] HO Prep Timer start Failure");
            }
        }
        else if(HANDOVER_TYPE_INTER_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = INTER_CELL_HO;
            if ( RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context, 
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] HO Prep Timer start Failure");
            }
        }
        /*BUG 604 changes start*/
        else if(HANDOVER_TYPE_INTRA_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = INTRA_CELL_HO;
            if ( RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context, 
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] HO Prep Timer start Failure");
            }
        }
        /*BUG 604 changes stop*/
        else
        {
            p_ue_context->ho_info.s1_or_x2_handover = S1_HO;
            if ( RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context, 
                        UECC_UE_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_HO_PREP_TIMER] HO Prep Timer start Failure");
            }
        }

        /* Build and send RRC_RRM_ERB_MODIFY_CNF with success to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_modify_conf(
                    p_ue_context, 
                    FAILURE,
                    error_code))
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                    "[RRC_RRM_ERB_MODIFY_CNF] Message Delivery failure");
        }

        /* Send the ASN. 1 E-RAB MODIFY RESPONSE message containing 
         * all failures to MME.*/
        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                    "[s1ap_E_RABModifyResponse] Delivery failure");
        }

        /* Send event to UE Main FSM and pass API data to FSM. */
        if (PNULL !=  p_ue_context->p_curr_proc_data)
        {
            uecc_event_queue_push_event(
                    UECC_EV_ERB_ONGOING_HO_REQUIRED_INT,
                    p_ue_context,
                    p_ue_context->p_curr_proc_data->msg_bfr_rcvd); /* SPR 2532 Start */

            /* Free the data for current procedure in the UE Context */
            rrc_mem_free(p_ue_context->p_curr_proc_data);  
            p_ue_context->p_curr_proc_data = PNULL;
        }

        /* Stop Procedure */
        uecc_fsm_finish_active_procedure(p_ue_context);    
        /* SPR 24021/SES-452 Fix Start */
        rrc_mem_free(p_api);
        p_api = PNULL;
        /* SPR 24021/SES-452 Fix End */
    }
    /* SPR 24021/SES-452 Coverity 209450 Fix End */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_radio_link_failure_handler
 *
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
*                       UECC_EV_RADIO_LINK_FAILURE_IND
 ****************************************************************************/
void uecc_fsm_erb_modify_radio_link_failure_handler
(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
)
{
    s1ap_Cause cause;
    rrc_return_t rrc_response = RRC_SUCCESS;
    U32 error_code;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    
    /*
    ** Stop Procedure Timer
    ** Send the ASN. 1 E-RAB MODIFY RESPONSE message containing 
    ** all failures to MME.
    */

      
    /* Stop Procedure Timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,
                        UECC_ERAB_MODIFY_GUARD_TIMER, 
                        &p_ue_context->timers_data);


    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost;
    error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
    
    RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);
  
    uecc_update_failed_list_from_to_be_modified_list(
            p_ue_context, 
            cause,
            error_code);

   
    /* Build and send RRC_RRM_ERB_MODIFY_CNF with success to RRM */
    rrc_response = uecc_rrm_build_and_send_erb_modify_conf(
                             p_ue_context, 
                             FAILURE,
                             error_code);

    if (rrc_response != RRC_SUCCESS)
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
            "[RRC_RRM_ERB_MODIFY_CNF] Delivery failure");            
    }

    /* Send the ASN. 1 E-RAB MODIFY RESPONSE message containing 
     * all failures to MME.*/
    if (RRC_SUCCESS !=
          uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[s1ap_E_RABModifyResponse] Delivery failure");
    }


    /* Send event to UE Main FSM and pass API data to FSM. */
    uecc_event_queue_push_event(
        UECC_EV_RADIO_LINK_FAILURE_INT,
        p_ue_context,
        p_api);
    
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Free the data for current procedure in the UE Context */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


 /****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_ue_rel_req_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This function Stops Procedure With Failure
 *                 -- Stop procedural timer
 *                 -- Send internal event to UE Main FSM
 *                 UECC_EV_UE_RELEASE_REQ_INT
 *                 -- Stop procedure
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_ue_rel_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
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
                        UECC_ERAB_MODIFY_GUARD_TIMER, 
                        &p_ue_context->timers_data);

    /* Process failure
    ** Procedure needs to be terminated.
    */
    cause = p_ue_context_rel_command->cause;
  
    /*SPR 22036 Fix Start*/
    if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_unspecified_2;   

        /* SPR 22310 CID 115957 Fix Start */
        error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
        /* SPR 22310 CID 115957 Fix End */

        /* Update erab release list to erab failed to release list */
        uecc_fsm_erabmod_update_erab_release_list_with_error_code(
                p_ue_context,
                &error_code);
    }
    else
    {
        error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
    }
    /*SPR 22036 Fix Stop*/
    /* SPR 22310 CID 115957 Fix Start */
    RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);
    /* SPR 22310 CID 115957 Fix End */


    uecc_update_failed_list_from_to_be_modified_list(
            p_ue_context,
            cause,
            error_code);

    /* Build and send RRC_RRM_ERB_MODIFY_CNF with success to RRM */
    if (RRC_FAILURE == uecc_rrm_build_and_send_erb_modify_conf(
                             p_ue_context, 
                             FAILURE,
                             error_code))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[RRC_RRM_ERB_MODIFY_CNF] Message Delivery failure");
    }

    /* Send the ASN. 1 E-RAB MODIFY RESPONSE message containing 
     * all failures to MME.*/
    if (RRC_SUCCESS !=
          uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[s1ap_E_RABModifyResponse] Message Delivery failure");
    }
    
    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
            UECC_EV_UE_RELEASE_REQ_INT,
            p_ue_context,
            p_api);

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Free the data for current procedure in the UE Context */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data=PNULL;
    }

    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_s1ap_reset_msg_ind_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This function handles the erb_modify_s1ap_reset_msg_ind 
 *
 ****************************************************************************/

void uecc_fsm_erb_modify_s1ap_reset_msg_ind_handler
(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
) 
{
    s1ap_Cause cause;
    U32 error_code;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,
                        UECC_ERAB_MODIFY_GUARD_TIMER, 
                        &p_ue_context->timers_data);
    
    cause.t = T_s1ap_Cause_transport;
    cause.u.radioNetwork = s1ap_transport_resource_unavailable;

    
  
    /*SPR 22036 Fix Start*/
    if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        /* SPR 22310 CID 115956 Fix Start */
        error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
        /* SPR 22310 CID 115956 Fix End */

        /* Update erab release list to erab failed to release list */
        uecc_fsm_erabmod_update_erab_release_list_with_error_code(
                p_ue_context,
                &error_code);
    }
    else
    {
        error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
    }
    /* SPR 22310 CID 115956 Fix Start */
    RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);
    /* SPR 22310 CID 115956 Fix End */
    /*SPR 22036 Fix Stop*/

    uecc_update_failed_list_from_to_be_modified_list(
            p_ue_context, 
            cause,
            error_code);

    /* Build and send RRC_RRM_ERB_MODIFY_CNF with success to RRM */
    if(RRC_FAILURE == uecc_rrm_build_and_send_erb_modify_conf(
                             p_ue_context, 
                             FAILURE,
                             error_code))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[RRC_RRM_ERB_MODIFY_CNF] Message Delivery failure");
    }

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
            UECC_EV_S1AP_RESET_MSG_IND_INT,
            p_ue_context,
            p_api);
    
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Free the data for current procedure in the UE Context */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }
    
    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * FUNCTION NAME :
 *        uecc_fsm_erb_modify_llim_reconfig_ue_rrc_reestablish_request_handler 
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This function handles UECC_EV_RRC_RE_ESTABLISH_REQ when 
 *                 ERAB Modification is ongoing at LLIM.
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_llim_reconfig_ue_rrc_reestablish_request_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    U16     len = RRC_NULL;
    /*SPR 22036 Fix Start*/
    U32      error_code = RRC_NULL;
    uecc_fsm_fail_handler_f const*  p_fail_actions_list =
        uecc_fsm_erb_modify_fail_type2_actions;
    /*SPR 22036 Fix Stop*/

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*SPR 22036 Fix Start*/
    if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        error_code = RRC_MODULE_ID << 16;
        error_code |= RRC_REESTABLISHMENT_TRIGGERED;

        /* Update erab release list to erab failed to release list */
        uecc_fsm_erabmod_update_erab_release_list_with_error_code(
                p_ue_context,
                &error_code);

        /* Put all E_RAB's in failed list from ERAB Modify List */
        uecc_fsm_erbmod_fail_action_mark_all_erab_failed(
                p_ue_context,
                RRC_REESTABLISHMENT_TRIGGERED);

        /* Send RRC-Connection Reject to UE */
        if (RRC_SUCCESS !=
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                    p_ue_context->p_gb_context, p_ue_context->crnti,
                    p_ue_context->reestablishmentCause,
                    p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                    "Fail to send the RRCConnectionReestablishmentReject to UE");
        }

        /* Start failure processing */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                p_fail_actions_list);

        /* enabling flag */
        SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    /*SPr 22036 Fix Stop*/

    /* Buffered Received Re-establishment Msg */
    len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
        rrc_mem_get(len)))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL,
            "Memory Allocation Failed to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* Storing re_establishment contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
        p_api,len);

    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RE_ESTABLISH_MSG;

    /*
    ** Change the state to UECC_LLIM_ERB_MODIFY_ROLLBACK_RECONFIGURE_ONGOING
    */

    /* Switch state */
    UECC_FSM_SET_CHILD_STATE(
    p_ue_context,
    &uecc_fsm_erb_modify_llim_rollback_ongoing,
    PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR 22036 Fix Start*/
/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_erbmod_delete_lc_req
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
void uecc_llim_build_erbmod_delete_lc_req(
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

    /* Fill message: cell index */
    p_rrc_uecc_llim_erbsp_delete_lc_req->cell_index = p_uecc_ue_context->cell_index;

    #ifdef ENDC_ENABLED
    rrc_drb_to_modify_list_t    *p_drb_list = PNULL;
#ifdef ENDC_ENABLED
    /*Logical_Error*/
    U8 delet_lc_count = 0;
    /*Logical_Error*/
#endif

    if (p_uecc_ue_context->p_curr_proc_data  //add for bugid 117724 
		&&(UECC_RRM_DC_BEARER_CHANGE_PROC == p_uecc_ue_context->p_curr_proc_data->t) )
    {
        /*
         * point to drb_list
         */
        p_drb_list = &p_uecc_ue_context->p_curr_proc_data->u.
            rrm_dc_bearer_change_request_data.drb_to_modify_list;

        /*
         * Fill LC info fields
         */
        p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity_counter =
            p_drb_list->drb_count;
    }
    else
    {
        /* point to erab_release_list in erab_release_command_data */
        p_erab_list = &p_uecc_ue_context->p_curr_proc_data->u.erab_modify_request_data.
            erab_release_command_data.erab_release_list;

        /* Fill LC info fields */
        p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity_counter =
            p_erab_list->counter;
    }
    #else
    /* point to erab_release_list in erab_release_command_data */
    p_erab_list = &p_uecc_ue_context->p_curr_proc_data->u.erab_modify_request_data.
        erab_release_command_data.erab_release_list;

    /* Fill LC info fields */
    p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity_counter =
        p_erab_list->counter;
    #endif

    for(drb_index = 0;
            drb_index < p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity_counter;
            drb_index++)
    {
        #ifdef ENDC_ENABLED
    /*Logical_Error*/
        if ( p_uecc_ue_context->p_curr_proc_data  //add for bugid 117724
			&&(UECC_RRM_DC_BEARER_CHANGE_PROC == p_uecc_ue_context->p_curr_proc_data->t))
        {
        	if((RRC_TRUE == p_drb_list->drb_config[drb_index].erab_success) &&
        			(RRC_FALSE == p_drb_list->drb_config[drb_index].erab_failed))
    /*Logical_Error*/
        {
            p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity[drb_index].
                lc_id = p_uecc_ue_context->p_e_rab_list[p_drb_list->
                drb_config[drb_index].erab_id]->
                drb_config.logical_channel_identity;

            if (RRC_RLC_CONFIG_AM_CONFIG_PRESENT &
                    p_uecc_ue_context->
                    p_e_rab_list[p_drb_list->drb_config[drb_index].erab_id]->
                    drb_config.rlc_config_enb.rlc_config.bitmask)
            {
                p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity[drb_index].
                    rlc_mode = RLC_CONFIG_AM_MODE;
            }
            if (RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT &
                    p_uecc_ue_context->
                    p_e_rab_list[p_drb_list->drb_config[drb_index].erab_id]->
                    drb_config.rlc_config_enb.rlc_config.bitmask)
            {
                p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity[drb_index].
                    rlc_mode = RLC_CONFIG_UM_BI_DIR_MODE;
            }
            if (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT &
                    p_uecc_ue_context->
                    p_e_rab_list[p_drb_list->drb_config[drb_index].erab_id]->
                    drb_config.rlc_config_enb.rlc_config.bitmask)
            {
                p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity[drb_index].
                    rlc_mode = RLC_CONFIG_UM_UL_UNI_DIR_MODE;
            }
            if (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT &
                    p_uecc_ue_context->
                    p_e_rab_list[p_drb_list->drb_config[drb_index].erab_id]->
                    drb_config.rlc_config_enb.rlc_config.bitmask)
            {
                p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity[drb_index].
                    rlc_mode = RLC_CONFIG_UM_DL_UNI_DIR_MODE;
            }
    			/*Logical_Error*/
            	delet_lc_count++;
		       /*Logical_Error*/
        	}
        }
        else
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
        #else
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
        #endif
    }
#ifdef ENDC_ENABLED
    /*Logical_Error*/
                    if (p_uecc_ue_context->p_curr_proc_data && //add for bugid 117724
						(UECC_RRM_DC_BEARER_CHANGE_PROC == p_uecc_ue_context->p_curr_proc_data->t))
                    {
                    	p_rrc_uecc_llim_erbsp_delete_lc_req->delete_drb_entity_counter = delet_lc_count;
                    }
    /*Logical_Error*/
#endif
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

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
        uecc_llim_build_erbmod_delete_lc_req(p_uecc_ue_context,
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
void uecc_fsm_erb_modify_llim_del_reconfigure_ue_ongoing_init_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Send Delete LC request to LLIM */
    uecc_llim_build_and_send_erbmod_delete_lc_req(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR 22036 Fix Stop*/

/****************************************************************************
 * FUNCTION NAME : 
 *         uecc_fsm_erb_modify_llim_reconfigure_ue_ongoing_init_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This function sends the UECC_LLIM_MODIFY_LC_REQ to LLIM.
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_llim_reconfigure_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send the UECC_LLIM_MODIFY_LC_REQ msg to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_modify_lc_req(p_ue_context))
    {
        /* Process failure according to selected failure actions list */
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
        "[UECC_LLIM_MODIFY_LC_REQ] Delivery Failure");
    }
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : 
 *         uecc_fsm_erb_modify_llim_reconfigure_ue_resp_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_LLIM_RECONFIGURE_UE_RESP from LLIM
 *                 If Success(i.e. at least one drb is modified):
 *
  ****************************************************************************/
void uecc_fsm_erb_modify_llim_reconfigure_ue_resp_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    rrc_return_t    result            = RRC_FAILURE;
    U16             drb_index         = RRC_NULL;
    rrc_counter_t   failed_list_index = RRC_NULL;

    rrc_bool_et  fill_cnf_list  = RRC_FALSE;
    rrc_bool_et  fill_fail_list = RRC_FALSE;

    s1ap_Cause    cause;
    
    erab_modify_list_t           *p_mod_list    = PNULL;
    erab_failed_to_modify_list_t *p_failed_list = PNULL;

    rrc_uecc_llim_modify_lc_resp_t*    p_rrc_uecc_llim_modify_lc_resp = PNULL;
    uecc_fsm_fail_handler_f const*     p_fail_actions_list = 
                                       uecc_fsm_erb_modify_fail_type2_actions;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* GDB fix - compiler warning 'cause.t' may be used uninitialized in this function */
    memset_wrapper( &cause, 0, sizeof(cause));

    p_rrc_uecc_llim_modify_lc_resp = (rrc_uecc_llim_modify_lc_resp_t*)p_api;

    p_mod_list = 
        &(p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
                erab_modify_list);
    
    p_failed_list = 
        &(p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
                        erab_failed_to_modify_list);
    do
    {
        /* Check whether UECC_EV_LLIM_RECONFIGURE_UE_RESP
        * received with success, partial_success or failure */
        switch (p_rrc_uecc_llim_modify_lc_resp->response_code)
        {
            case SUCCESS:
                RRC_UECC_ERB_MODIFY_FSM_TRACE (RRC_INFO,
                "LLIM has modified radio resources successfully");
                /* Update the Failed list */
                fill_cnf_list = RRC_TRUE;
                result = RRC_SUCCESS;

                /* SPS Start */
                if (p_ue_context->m.is_sps_recv_frm_rrm)
                {
                    /* Check if Setup SPS config  was received */
                    if (!p_ue_context->m.is_sps_setup_or_rel_recv)
                    {
                        if (p_rrc_uecc_llim_modify_lc_resp->bitmask &
                                UECC_LLIM_MODIFY_LC_RESP_SPS_CRNTI_PRESENT)
                        {
                            /* Copy SPS CRNTI into UE Context */
                            rrc_pack_U16(p_ue_context->p_sps_config->semi_presist_sched_c_rnti,
                                    &p_rrc_uecc_llim_modify_lc_resp->sps_crnti);
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
                }
                /* SPS Stop */

                break;               

            case L3_FATAL:
                RRC_UECC_ERB_MODIFY_FSM_TRACE (RRC_ERROR,
                "Radio resources modification unsuccessful at LLIM");
                /* Stop Procedure Timer */
                uecc_ue_timer_stop(p_ue_context->p_gb_context,
                                UECC_ERAB_MODIFY_GUARD_TIMER, 
                                &p_ue_context->timers_data);
                fill_fail_list = RRC_TRUE;
                cause.t = T_s1ap_Cause_radioNetwork;
                cause.u.radioNetwork = s1ap_unspecified_2;

                /* unrecoverable error from LLIM: Release UE Context */
                p_fail_actions_list =
                uecc_fsm_erb_modify_fail_type2_actions;
                break;

            case FAILURE:    
                RRC_UECC_ERB_MODIFY_FSM_TRACE (RRC_ERROR,
                "Radio resources modification unsuccessful at LLIM");
                /* Stop Procedure Timer */
                uecc_ue_timer_stop(p_ue_context->p_gb_context,
                                UECC_ERAB_MODIFY_GUARD_TIMER, 
                                &p_ue_context->timers_data);
                fill_fail_list = RRC_TRUE;
                cause.t = T_s1ap_Cause_radioNetwork;
                cause.u.radioNetwork = s1ap_unspecified_2;
                /*SPR 22036 Fix Start*/
                if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
                        is_erab_del_ongoing == RRC_TRUE)
                {
                    p_fail_actions_list =
                        uecc_fsm_erb_modify_fail_type2_actions;
                }
                else
                {
                p_fail_actions_list =
                uecc_fsm_erb_modify_fail_type3_actions;
                }
                /*SPR 22036 Fix Stop*/
                break;
                
            case PARTIAL_SUCCESS:
                RRC_UECC_ERB_MODIFY_FSM_TRACE (RRC_INFO,
                "LLIM has modified radio resources partially");
                fill_cnf_list = RRC_TRUE;
                fill_fail_list = RRC_TRUE;
                cause.t = T_s1ap_Cause_radioNetwork;
                cause.u.radioNetwork = s1ap_unspecified_2;
                result = RRC_SUCCESS;
                /* SPS Start */
                if (p_ue_context->m.is_sps_recv_frm_rrm)
                {
                    /* Check if Setup SPS config  was received */
                    if (!p_ue_context->m.is_sps_setup_or_rel_recv)
                    {
                        if (p_rrc_uecc_llim_modify_lc_resp->bitmask &
                                UECC_LLIM_ADD_LC_RESP_SPS_CRNTI_PRESENT)
                        {
                            /* Copy SPS CRNTI into UE Context */
                            rrc_pack_U16(p_ue_context->p_sps_config->semi_presist_sched_c_rnti,
                                    &p_rrc_uecc_llim_modify_lc_resp->sps_crnti);
                            p_ue_context->p_sps_config->bitmask |= RRC_SPS_CONFIG_C_RNTI_PRESENT;
                        }
                        else
                        {
                            p_ue_context->m.sps_config_updated = RRC_NULL; /* Reset to 0 */
                        }
                    }
                }
                /* SPS Stop */
                break;
                
            default:
                 RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL,
                "[UECC_EV_LLIM_RECONFIGURE_UE_RESP] Invalid Response=%u",
                    p_rrc_uecc_llim_modify_lc_resp->response_code);
                /* Unrecoverable error from LLIM: Release UE Context */
                p_fail_actions_list =
                    uecc_fsm_erb_modify_fail_type2_actions;
            break;
        }

        if (fill_cnf_list == RRC_TRUE)
        {
            for (drb_index = 0; 
                 drb_index <
                 p_rrc_uecc_llim_modify_lc_resp->modify_drb_status_counter; 
                 drb_index++)
            {
                if (p_rrc_uecc_llim_modify_lc_resp->
                        modify_drb_status[drb_index].response == RRC_NO_ERROR)
                {
                    /* Initialize Status for each DRB in the Modify list */
                    p_mod_list->erab_modify_item[drb_index].status = RRC_SUCCESS;
                }
            }
        }
    
        if (fill_fail_list == RRC_TRUE)
        {
            /* Get the index of the failed list */
            failed_list_index = (U16)
                    (p_failed_list->erab_failed_to_modify_list_counter);
            /* Update the Failed list */
            for (drb_index = 0; 
                 drb_index < 
                 p_rrc_uecc_llim_modify_lc_resp->modify_drb_status_counter; 
                 drb_index++)
            {
                    
                if (p_rrc_uecc_llim_modify_lc_resp->
                        modify_drb_status[drb_index].response != RRC_NO_ERROR)
                {
                    p_failed_list->erab_failed_to_modify_list_counter++;


                    if (failed_list_index < MAX_ERAB_COUNT)
                    {
                        
                    /* Append the failed DRBs to the failed list */
                        
                        p_failed_list->erab_failed_to_modify_item
                            [failed_list_index].e_RAB_ID =
                        p_mod_list->erab_modify_item[drb_index].drb_config.erab_id;
    
                        p_failed_list->
                            erab_failed_to_modify_item[failed_list_index].
                            error_code =
                        p_rrc_uecc_llim_modify_lc_resp->
                            modify_drb_status[drb_index].response;

                        p_failed_list->erab_failed_to_modify_item
                             [failed_list_index].cause = cause;
                        /* 
                        ** Update the  status as Failed in the modify list. 
                        */
                        p_mod_list->erab_modify_item[drb_index].status = 
                        RRC_FAILURE;
                        failed_list_index++;
                    }
                    else
                    {
                        result = RRC_FAILURE;
                        /* Process failure according to selected failure actions list */
                        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_LLIM_MODIFY_LC_RESP] Processing Failure");
                    }
                }
            }
        }
    } while (0);

    if ( RRC_SUCCESS == result)
    {
        UECC_FSM_SET_CHILD_STATE(p_ue_context,
                             &uecc_fsm_erb_modify_ue_reconfigure_ongoing,
                             PNULL);
    }
    else if (RRC_SUCCESS != result )
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
            p_ue_context,
            p_fail_actions_list);
        
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_llim_reconf_ue_ongoing_ho_required_ind_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 *
 * DESCRIPTION   : This is the handler function for the event 
 *                   UECC_EV_RRM_HO_REQUIRED
 ****************************************************************************/
void  uecc_fsm_erb_modify_llim_reconf_ue_ongoing_ho_required_ind_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*SPR 22036 Fix Start*/
    if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,
                "[UECC_EV_RRM_HO_REQUIRED]"
                "RRM Initiated E-RAB Release Procedure Ongoing"
                "Cancel the HO triggered by RRM");
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_interaction_with_other_procedure;
        if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_failure( p_ue_context))
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
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
    /*SPR 22036 Fix Stop*/

    /*
    ** Start the HO PREP TIMER
    ** Change the state toUECC_LLIM_ERB_MODIFY_ROLLBACK_RECONFIGURE_ONGOING
    */
    
    /* Buffer the HO REQUIRED message in UE Context. */
    len = (U16)sizeof(rrc_rrm_ho_required_t);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
           rrc_mem_get(len))) 
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL,
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
    /* SPR 24021/SES-452 Coverity 209463 Fix Start */
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
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] X2_HO Prep Timer start Failure");
            }
            else
            {
                /* Switch state */
                UECC_FSM_SET_CHILD_STATE(
                        p_ue_context,
                        &uecc_fsm_erb_modify_llim_rollback_ongoing,
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
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] INTER_CELL_HO Prep Timer start Failure");
            }
            else
            {
                /* Switch state */
                UECC_FSM_SET_CHILD_STATE(
                        p_ue_context,
                        &uecc_fsm_erb_modify_llim_rollback_ongoing,
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
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] INTRA_CELL_HO Prep Timer start Failure");
            }
            else
            {
                /* Switch state */
                UECC_FSM_SET_CHILD_STATE(
                        p_ue_context,
                        &uecc_fsm_erb_modify_llim_rollback_ongoing,
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
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_HO_PREP_TIMER] HO Prep Timer start Failure");
            }
            else
            {
                /* Switch state */
                UECC_FSM_SET_CHILD_STATE(
                        p_ue_context,
                        &uecc_fsm_erb_modify_llim_rollback_ongoing,
                        PNULL);
            }
        }

        /* SPR 24021/SES-452 Fix Start */
        rrc_mem_free(p_api);
        p_api = PNULL;
        /* SPR 24021/SES-452 Fix End */
    }
    /* SPR 24021/SES-452 Coverity 209463 Fix End */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : 
 *  uecc_fsm_erb_modify_llim_radio_link_failure_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                 UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE
 ****************************************************************************/
void uecc_fsm_erb_modify_llim_radio_link_failure_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    U16   len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*SPR 22036 Fix Start*/
    if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        uecc_fsm_erabmod_erab_release_fail_handler(
                p_ue_context,
                RRC_RADIO_LINK_FAILURE_TRIGGERED,
                uecc_fsm_erb_modify_fail_type2_actions);

        /* enabling flag */
        SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    /*SPR 22036 Fix Stop*/


    /*
     ** Stop Procedure Timer
     ** Change the state to UECC_LLIM_ERB_MODIFY_ROLLBACK_RECONFIGURE_ONGOING
     */

    /* Store RLF in UE Context */
    len = (U16)sizeof(rrc_uecc_llim_radio_link_failure_ind_t);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
        rrc_mem_get(len)))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL,
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

    /* Switch state */
    UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_erb_modify_llim_rollback_ongoing,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME :
 * uecc_fsm_erb_modify_llim_reconf_ue_ongoing_timer_expiry_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   :    This is the handler function for the message
 *                     UECC_EV_TIMER_EXPIRY on a time out from LLIM.
 ****************************************************************************/
void 
uecc_fsm_erb_modify_llim_reconf_ue_ongoing_timer_expiry_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    rrc_return_t rrc_response = RRC_SUCCESS;
    s1ap_Cause cause;
    U32        error_code;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_BRIEF, "Time out.");

    cause.t = T_s1ap_Cause_radioNetwork;
    /* SPR 15473 Fix Start */
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;   
    /* SPR 15473 Fix Stop */
    error_code = RRC_TIMER_EXPIRY;

    RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);

    /*SPR 22036 Fix Start*/
    if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        error_code = RRC_MODULE_ID << 16;
        error_code |= RRC_TIMER_EXPIRY;

        /* Update erab release list to erab failed to release list */
        uecc_fsm_erabmod_update_erab_release_list_with_error_code(
                p_ue_context,
                &error_code);
    }
    /*SPR 22036 Fix Stop*/

    uecc_update_failed_list_from_modified_list(
            p_ue_context, 
            cause);

    /* Build and send RRC_RRM_ERB_MODIFY_CNF with failure to RRM */
    rrc_response = uecc_rrm_build_and_send_erb_modify_conf(
            p_ue_context, 
            FAILURE,
            error_code);
    if (rrc_response != RRC_SUCCESS)
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[RRC_RRM_ERB_MODIFY_CNF] Delivery failure");            
    }

    if (RRC_SUCCESS !=
          uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
            "[s1ap_E_RABModifyResponse] Delivery failure");
    }

    uecc_fsm_finish_activity_failure(
          p_ue_context, (void*)UECC_EV_S1AP_ERAB_MODIFY_FAILURE_REL_UE_CTX_INT);  
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_ue_reconfigure_ongoing_init_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This action routine is called when FSM receives the
 *                 UECC_EV_STATE_INIT message.
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_ue_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* Build and send RRCConnectionReconfiguration */
    if (RRC_SUCCESS !=
            uecc_llim_build_and_send_erb_modify_rrc_connection_reconfiguration(
                p_ue_context))
    {
        /* Send the ASN. 1 E-RAB MODIFY RESPONSE message containing 
         * all failures to MME.*/
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_unspecified_2;

        uecc_update_failed_list_from_modified_list(p_ue_context, cause);

        uecc_fsm_start_failure_processing(p_ue_context,
            uecc_fsm_erb_modify_fail_type2_actions); 
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : 
 *  uecc_fsm_erb_modify_ue_rrc_conn_reconf_complete_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                  UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_ue_rrc_conn_reconf_complete_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    erab_modify_list_t *p_mod_list = PNULL;

    rrc_return_et     rrc_response = RRC_FAILURE;
    rrc_counter_t     erab_count   = RRC_NULL;
    U32               error_code;
    /*SPR 22036 Fix Start*/
    erab_list_t      *p_erab_list = PNULL;
    U8                drb_index = RRC_NULL; 
    /*SPR 22036 Fix Stop*/

    s1ap_Cause    cause;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    p_mod_list = &(p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
        erab_modify_list);

    erab_count = p_mod_list->erab_modify_list_counter;  
    /* Bug_10698 Fix Start*/
    /* SPR 13690 Fix Start */ 
    if ((p_ue_context->curr_meas_config.is_meas_config_valid ==
             RRM_RRC_MC_VALIDATE_SUCCESS) && (PNULL != p_ue_context->p_meas_config))/* SPR 13690 Fix Stop */
    {
        /* Measurment Re-config START */
        /* coverity_fix_64333_start */ 
        if (PNULL == p_ue_context->p_meas_config)
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,"meas_config received is NULL.\n");
            return;
        }
        /* coverity_fix_64333_stop */ 
        if (RRC_SUCCESS != uecc_rrc_update_CurrMeasConfigList(
                    p_ue_context->p_meas_config,
                    &p_ue_context->curr_meas_config,
                    p_ue_context->p_gb_context))
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
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

    /* Stop Procedure Timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_ERAB_MODIFY_GUARD_TIMER, 
            &p_ue_context->timers_data);

    if (erab_count > 0)
    {

        error_code = RRC_NO_ERROR;

        /* Build and send RRC_RRM_ERB_MODIFY_CNF with success to RRM */
        rrc_response = uecc_rrm_build_and_send_erb_modify_conf(
                p_ue_context, 
                SUCCESS,
                error_code);
        if (RRC_SUCCESS !=
              uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                    "[s1ap_E_RABModifyResponse] Delivery failure");
        }

        /*SPR 22036 Fix Start*/
        if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
                is_erab_del_ongoing == RRC_TRUE)
        {
            /* Build and send the RRC_S1AP_E_RAB_RELEASE_INDICATION msg to S1AP */
            if (RRC_SUCCESS != uecc_s1ap_build_and_send_e_rab_release_indication_arp(
                        p_ue_context))
            {
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[s1ap_E_RABSetup] Delivery failure E_RAB Release Indication");
            }
            p_ue_context->p_curr_proc_data->u.erab_modify_request_data.is_erab_del_ongoing = 
                RRC_FALSE;

            p_erab_list = &p_ue_context->p_curr_proc_data->u.erab_modify_request_data.erab_release_command_data.erab_release_list;

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
                    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,
                            "E-RAB ID %u context deleted from UE Context",
                            p_erab_list->erab_item[drb_index].e_RAB_ID);
                }
            }
        }
        /*SPR 22036 Fix Stop*/

        if (rrc_response == RRC_SUCCESS)
        {
            /* 
             ** Update the UE Context  as atleast one E-RAB was 
             ** successfully modified
             */
            uecc_update_ue_context(p_ue_context);
            uecc_fsm_finish_activity_success(p_ue_context, PNULL);
        }
    }

    if (rrc_response != RRC_SUCCESS)
    {
        /* Process failure according to selected failure actions list */
        /* The same as failure processing of LLIM error */

        /* Send the ASN. 1 E-RAB MODIFY RESPONSE message containing 
         * all failures to MME.*/
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_unspecified_2;

        uecc_update_failed_list_from_modified_list(p_ue_context, cause);

        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_erb_modify_fail_type2_actions);
    }
    uecc_llim_check_and_send_rrc_ue_information(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : 
 * uecc_fsm_erb_modify_ue_reconfigure_ongoing_timer_expiry_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   :   This is the handler function for the message
 *                       UECC_EV_TIMER_EXPIRY
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_ue_reconfigure_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    s1ap_Cause   cause;
    U32          error_code;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_BRIEF, "Time out: No response from UE.");

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
            s1ap_unspecified_2;

        /* Send the HO Failure Message to RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);

        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            /* Discard the HO message*/
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL; 
        }
    }
    else
    {
        /* Release any buffered message */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd=PNULL;
        }
    }

    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;

    /*SPR 22036 Fix Start*/
    if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        error_code = RRC_MODULE_ID << 16;
        error_code |= RRC_TIMER_EXPIRY;

        /* Update erab release list to erab failed to release list */
        uecc_fsm_erabmod_update_erab_release_list_with_error_code(
                p_ue_context,
                &error_code);
    }
    /*SPR 22036 Fix Stop*/
    error_code =  RRC_TIMER_EXPIRY;

    uecc_update_failed_list_from_modified_list(p_ue_context, cause);

    RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);

    /* Build and send RRC_RRM_ERB_MODIFY_CNF with success to RRM */
    if(RRC_SUCCESS != uecc_rrm_build_and_send_erb_modify_conf(
                p_ue_context, 
                FAILURE,
                error_code))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
            "[uecc_rrm_build_and_send_erb_modify_conf] sending failure");
    }

    if (RRC_SUCCESS !=
          uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
            "[s1ap_E_RABModifyResponse] Delivery failure");
    }    

    uecc_fsm_finish_activity_failure(
          p_ue_context, (void*)UECC_EV_S1AP_ERAB_MODIFY_FAILURE_REL_UE_CTX_INT);
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_ue_rrc_reestablish_request_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   :   This is the handler function for the message
 *                       UECC_EV_RRC_RE_ESTABLISH_REQ
 ****************************************************************************/
void uecc_fsm_erb_modify_ue_rrc_reestablish_request_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
    {
        /*x2_ho_timer*/
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

        /* Send the HO Failure Message to RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);

        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            /* Discard the HO message*/
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
    }
    else
    {
        /* Release any buffered message */
        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
    }

    /*SPR 22036 Fix Start*/
    if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        uecc_fsm_erabmod_erab_release_fail_handler(
                p_ue_context,
                RRC_REESTABLISHMENT_TRIGGERED,
                uecc_fsm_erb_modify_fail_type2_actions);

        /* Send RRC-Connection Reject to UE */
        if (RRC_SUCCESS !=
                uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
                    p_ue_context->p_gb_context, p_ue_context->crnti,
                    p_ue_context->reestablishmentCause,
                    p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                    "Fail to send the RRCConnectionReestablishmentReject to UE");
        }

        /* enabling flag */
        SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    /*SPR 22036 Fix Stop*/


    /* Store the Re-establish message in the UE Context */
    len = (U16)sizeof(RRCConnectionReestablishmentRequest_r8_IEs);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
        rrc_mem_get(len)))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL,
            "Memory Allocation Failed "
            "to store Reestablishment Req msg");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    /* storing re_establishment contents into msg_bfr_rcvd */
    l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
        p_api, len);

    p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_RE_ESTABLISH_MSG;

    /* Switch state */
    UECC_FSM_SET_CHILD_STATE(p_ue_context,
            &uecc_fsm_erb_modify_llim_rollback_ongoing,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_ue_rrm_ho_required_indication_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                  UECC_EV_RRM_HO_REQUIRED
 *
 ****************************************************************************/
void 
uecc_fsm_erb_modify_ue_rrm_ho_required_indication_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    U16     len = RRC_NULL;

       
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context); 

    /*SPR 22036 Fix Start*/
    if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,
                "[UECC_EV_RRM_HO_REQUIRED]"
                "RRM Initiated E-RAB Release Procedure Ongoing"
                "Cancel the HO triggered by RRM");
        p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
        p_ue_context->ho_info.cause.u.radioNetwork = s1ap_interaction_with_other_procedure;
        if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_failure( p_ue_context))
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
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
    /*SPR 22036 Fix Stop*/

    /* Buffer the HO REQUIRED message in UE Context. */
    len = (U16)sizeof(rrc_rrm_ho_required_t);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
           rrc_mem_get(len))) 
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL,
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
    /* SPR 24021/SES-452 Coverity 209469 Fix Start */
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
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER]X2_HO Prep Timer start Failure");
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
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER]INTER_CELL_HO Prep Timer start Failure");
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
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER]INTRA_CELL_HO Prep Timer start Failure");
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
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_HO_PREP_TIMER] HO Prep Timer start Failure");
            }
        }

        /* SPR 24021/SES-452 Fix Start */
        rrc_mem_free(p_api);
        p_api = PNULL;
        /* SPR 24021/SES-452 Fix End */
    }
    /* SPR 24021/SES-452 Coverity 209469 Fix End */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_ue_radio_link_failure_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the Radio Link Failure 
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_ue_radio_link_failure_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    U16  len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
    {
        /*x2_ho_timer*/
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

        /* Send the HO Failure Message to RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);

        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            /* Memory  freed for RRC_HO_REQUIRED_MSG */
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

    /*SPR 22036 Fix Start*/
    if (p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
            is_erab_del_ongoing == RRC_TRUE)
    {
        uecc_fsm_erabmod_erab_release_fail_handler(
                p_ue_context,
                RRC_RADIO_LINK_FAILURE_TRIGGERED,
                uecc_fsm_erb_modify_fail_type2_actions);

        /* enabling flag */
        SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }
    /*SPR 22036 Fix Stop*/

    /* Buffer the RLF message in UE Context. */
    len = (U16)sizeof(rrc_uecc_llim_radio_link_failure_ind_t);
    if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd =
        rrc_mem_get(len)))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL,
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

    /* Switch state */
    UECC_FSM_SET_CHILD_STATE(
        p_ue_context,
        &uecc_fsm_erb_modify_llim_rollback_ongoing,
        PNULL);
   
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_llim_rollback_ongoing_init_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                  UECC_EV_STATE_INIT                      
 *
 ****************************************************************************/
void 
uecc_fsm_erb_modify_llim_rollback_ongoing_init_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    s1ap_Cause    cause;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send the UECC_LLIM_MODIFY_ROLLBACK_REQ msg to LLIM */
    if (RRC_SUCCESS != 
            uecc_llim_build_and_send_modify_rollback_req(p_ue_context))
    {
        /* Here, the UE context must be released!! */
        /* Process failure according to selected failure actions list */

        /* Send the ASN. 1 E-RAB MODIFY RESPONSE message containing 
         * all failures to MME.*/
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_unspecified_2;

        uecc_update_failed_list_from_modified_list(p_ue_context, cause);        
        uecc_fsm_start_failure_processing(p_ue_context,
                uecc_fsm_erb_modify_fail_type2_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_llim_modify_rollback_resp_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                       UECC_LLIM_MODIFY_ROLLBACK_RESP                         
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_llim_modify_rollback_resp_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{

    rrc_return_et rrc_response = RRC_FAILURE;
    U32           error_code   = RRC_NO_ERROR;
    s1ap_Cause    cause;

    rrc_uecc_llim_modify_rollback_resp_t* p_rrc_uecc_llim_modify_rollback_resp;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    rrc_response = RRC_FAILURE;

    p_rrc_uecc_llim_modify_rollback_resp = PNULL;
    p_rrc_uecc_llim_modify_rollback_resp = 
        (rrc_uecc_llim_modify_rollback_resp_t*)p_api;

    memset_wrapper(&cause, 0, sizeof(s1ap_Cause));    

    /* Stop Procedure Timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_ERAB_MODIFY_GUARD_TIMER, 
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
                        error_code = RRC_REESTABLISHMENT_TRIGGERED;
                        cause.t = T_s1ap_Cause_radioNetwork;
                        /* SPR 13133 Fix Start */
                        cause.u.radioNetwork = 
                            s1ap_failure_in_radio_interface_procedure;
                        /* SPR 13133 Fix Stop */
                        break;
                    case RRC_HO_REQUIRED_MSG:
                        error_code = RRC_HANDOVER_TRIGGERED;
                        cause.t = T_s1ap_Cause_radioNetwork;
                        if ( (HANDOVER_TYPE_INTRA_LTE_S1 ==
                                    ((rrc_rrm_ho_required_t*)p_ue_context->
                                     p_curr_proc_data->msg_bfr_rcvd)->ho_type) ||
                                (HANDOVER_TYPE_INTRA_LTE_X2 ==
                                 ((rrc_rrm_ho_required_t*)p_ue_context->
                                  p_curr_proc_data->msg_bfr_rcvd)->ho_type) ||
                                (HANDOVER_TYPE_INTER_CELL == ((rrc_rrm_ho_required_t*)p_ue_context->
                                                              /*BUG 604 changes start*/
                                                              p_curr_proc_data->msg_bfr_rcvd)->ho_type) ||
                                (HANDOVER_TYPE_INTRA_CELL == ((rrc_rrm_ho_required_t*)p_ue_context->
                                                              p_curr_proc_data->msg_bfr_rcvd)->ho_type))
                            /*BUG 604 changes stop*/
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
                        error_code = RRC_RADIO_LINK_FAILURE_TRIGGERED;
                        cause.t = T_s1ap_Cause_radioNetwork;
                        cause.u.radioNetwork = s1ap_radio_connection_with_ue_lost;
                        break;
                    default:
                        /* Error*/
                        error_code = RRC_INTERNAL_ERROR;
                        rrc_response = RRC_FAILURE;
                        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO, 
                                "Unrecognized Message Encountered");
                        break;
                }
            }
            break;

        case RRC_FAILURE:
            switch (p_ue_context->p_curr_proc_data->msg_bfr_type)
            {
                case RRC_HO_REQUIRED_MSG:
                    error_code = RRC_HANDOVER_ROLLBACK_TRIGGERED_FAILURE;
                    cause.t = T_s1ap_Cause_radioNetwork;
                    cause.u.radioNetwork = 
                        s1ap_interaction_with_other_procedure;

                    /*x2_ho_timer*/
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
                        s1ap_unspecified_2;

                    /* Send the HO Failure Message to RRM */
                    uecc_rrm_build_and_send_ho_failure(p_ue_context);

                    break;
                case RRC_RE_ESTABLISH_MSG:
                    error_code = 
                        RRC_REESTABLISHMENT_TRIGGERED_ROLLBACK_FAILURE;
                    cause.t = T_s1ap_Cause_radioNetwork;
                    cause.u.radioNetwork = 
                        s1ap_interaction_with_other_procedure;
                    break;
                case RRC_RLF_MSG:
                    error_code =
                        RRC_RADIO_LINK_FAILURE_TRIGGERED_ROLLBACK_FAILURE;
                    cause.t = T_s1ap_Cause_radioNetwork;
                    cause.u.radioNetwork = 
                        s1ap_radio_connection_with_ue_lost;
                    break;
                default:
                    error_code = RRC_INTERNAL_ERROR;
                    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                        "Invalid Error Code Received");
                    break;
            }
            break;
        default:
            /* Error*/
            error_code = RRC_INTERNAL_ERROR;
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_BRIEF, 
                    "Unrecognized Response Received");
            break;
    }

    /* Send the ASN. 1 E-RAB MODIFY RESPONSE message containing 
     * all failures to MME.*/

    uecc_update_failed_list_from_modified_list(p_ue_context, cause);

    /* Send message to RRM RRC_RRM_ERB_MODIFY_CNF(FAILURE) */
    RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_modify_conf(
                p_ue_context, FAILURE, error_code))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[RRC_RRM_ERB_MODIFY_CNF] Delivery failure");
    }

    if (RRC_SUCCESS != uecc_s1ap_build_and_send_erab_modify_response(
                p_ue_context))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[s1ap_E_RABModifyResponse] Delivery failure");
    }

    if (rrc_response == RRC_SUCCESS)
    {
        /* Notify procedure about failure */
        uecc_fsm_finish_activity_failure(
             p_ue_context, (void *) UECC_EV_S1AP_ERAB_MODIFY_RESP_FAILURE_INT);
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
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "Fail to send the RRCConnectionReestablishmentReject to UE");
            }
            else
            {
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,
                        "RRCConnectionReestablishmentReject Send to UE Successfully");
            }
            /* enabling flag */
            SET_RRC_CONNECTION_RELEASE_NOT_REQ_FLAG(p_ue_context);

        }


        /* Notify procedure about failure */
        uecc_fsm_finish_activity_failure(
             p_ue_context, (void *)UECC_EV_S1AP_ERAB_MODIFY_FAILURE_REL_UE_CTX_INT );
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_rrc_re_establish_request_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                       UECC_EV_RRC_RE_ESTABLISH_REQ                         
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_rrc_re_establish_request_handler
(
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
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL, 
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
 * FUNCTION NAME : uecc_fsm_erb_modify_rrm_ho_required_ind_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                       UECC_EV_RRM_HO_REQUIRED                         
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_rrm_ho_required_ind_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    U16     len = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_NO_MSG)
    {
        len = (U16)sizeof(rrc_rrm_ho_required_t);
        if (PNULL == (p_ue_context->p_curr_proc_data->msg_bfr_rcvd = 
            rrc_mem_get(len)))
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL, 
                    "memory allocation failed to "
                    "store received RRC_RRM_HO_REQUIRED");
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
        l3_memcpy_wrapper(p_ue_context->p_curr_proc_data->msg_bfr_rcvd,
            p_api,len);

        p_ue_context->p_curr_proc_data->msg_bfr_type = RRC_HO_REQUIRED_MSG;

        /*x2_ho_timer*/
        if (HANDOVER_TYPE_INTRA_LTE_X2 ==
                ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = X2_HO;
            /* Start the HO PREP TIMER */
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] HO Prep Timer start Failure");
            }
        }
        else if(HANDOVER_TYPE_INTER_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = INTER_CELL_HO;
            /* Start the HO PREP TIMER */
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] INTER_CELL_HO Timer start Failure");
            }
        }
        /*BUG 604 changes start*/
        else if(HANDOVER_TYPE_INTRA_CELL == ((rrc_rrm_ho_required_t*)p_api)->ho_type)
        {
            p_ue_context->ho_info.s1_or_x2_handover = INTRA_CELL_HO;
            /* Start the HO PREP TIMER */
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_X2_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_X2_HO_PREP_TIMER] INTRA_CELL_HO Timer start Failure");
            }
        }
        /*BUG 604 changes stop*/
        else
        {
            p_ue_context->ho_info.s1_or_x2_handover = S1_HO;
            /* Start the HO PREP TIMER */
            if (RRC_SUCCESS != uecc_ue_timer_start(
                        p_ue_context->p_gb_context,
                        UECC_UE_HO_PREP_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index))
            {
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "[UECC_UE_HO_PREP_TIMER] HO Prep Timer start Failure");
            }
        }
    }
    else
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
 * uecc_fsm_erb_modify_llim_rollback_ongoing_timer_expiry_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                       UECC_EV_TIMER_EXPIRY                         
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_llim_rollback_ongoing_timer_expiry_handler
(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api
)
{
    s1ap_Cause    cause;
    U32          error_code;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
    {
        /*x2_ho_timer*/
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

        /* Send the HO Failure Message to RRM */
        uecc_rrm_build_and_send_ho_failure(p_ue_context);

        if (PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd)
        {
            /* Discard the HO message*/
            rrc_mem_free(p_ue_context->p_curr_proc_data->msg_bfr_rcvd);
            p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
        }
    }
    /* Process failure according to selected failure actions list */
    /* Send the ASN. 1 E-RAB MODIFY RESPONSE message containing 
     * all failures to MME.*/
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_unspecified_2;
    error_code =  RRC_TIMER_EXPIRY;

    uecc_update_failed_list_from_modified_list(p_ue_context, cause);

    RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);

    /* Build and send RRC_RRM_ERB_MODIFY_CNF with failure to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_modify_conf(
                p_ue_context, 
                FAILURE,
                error_code))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
            "[uecc_rrm_build_and_send_erb_modify_conf] Sending failed");
    }

    if (RRC_SUCCESS !=
          uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
            "[s1ap_E_RABModifyResponse] Delivery failure");
    }

    uecc_fsm_finish_activity_failure(
          p_ue_context, (void*)UECC_EV_S1AP_ERAB_MODIFY_FAILURE_REL_UE_CTX_INT);  

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_rollback_radio_link_failure_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                      for Radio LinkFailure                         
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_rollback_radio_link_failure_handler
(
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
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,
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
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL,
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

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_process_s1ap_erab_modify_msg
 *
 * INPUT         : uecc_ue_context_t*                   p_ue_context,
 *                 rrc_s1ap_erab_modify_request_t*      p_msg
 *
 * OUTPUT        : None
 *
 * RETURNS       : rrc_return_t
 *
 * DESCRIPTION   : This is the function processes UECC_EV_S1AP_ERAB_MODIFY_REQ
 *                  message.
 *
 ****************************************************************************/
rrc_return_t uecc_fsm_process_s1ap_erab_modify_msg(
    uecc_ue_context_t*                   p_ue_context,
    rrc_s1ap_erab_modify_request_t*      p_msg
)
{
    rrc_return_t    result = RRC_SUCCESS;

    erab_modify_request_data_t   *p_mod_req_data = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_msg);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_mod_req_data =
        &(p_ue_context->p_curr_proc_data->u.erab_modify_request_data);

    /* Store message data in UE context for future processing */
    if (RRC_S1AP_ERAB_MODIFY_UE_AGGREGATE_MAX_BITRATE_PRESENT
            & p_msg->bitmask)
    {
        p_mod_req_data->m.ue_ambr_present = 1;

        p_mod_req_data->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateDL
            = p_msg->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateDL;
        p_mod_req_data->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateUL
            = p_msg->ue_aggregate_maximum_bitrate.uEaggregateMaximumBitRateUL;
    }
    /* 
     ** Update the to_be_modified list and failed_list as per the e_rab_id(s)
     ** received from MME.
     */
    if (RRC_FAILURE == uecc_update_to_be_modified_list_from_mme(
                p_ue_context, 
                p_msg))
    {
        result = RRC_FAILURE; 
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_S1AP_ERAB_MODIFY_REQ] Processing failed");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * FUNCTION NAME : uecc_update_to_be_modified_list_from_mme
 *
 *
 * INPUT         : uecc_ue_context_t*               p_ue_context,
 *                 rrc_s1ap_erab_modify_request_t  *p_msg
 *
 * OUTPUT        : None
 *
 * RETURNS       : rrc_return_t
 *
 * DESCRIPTION   : This function validates the data received from the MME (via 
 *               S1AP) and updates the to_be_modified and failed_list 
 *               accordingly.
 ****************************************************************************/
rrc_return_t  uecc_update_to_be_modified_list_from_mme
(
    uecc_ue_context_t               *p_ue_context,
    rrc_s1ap_erab_modify_request_t  *p_msg
)
{
    U16             e_rab_list_count      = RRC_NULL;
    rrc_return_t    result                = RRC_SUCCESS;

    s1ap_E_RABToBeModifiedItemBearerModReq 
        *p_e_rab_list[MAX_MME_ERAB_LIST_COUNT] = { NULL };

    OSRTDListNode                                  *p_node      = PNULL;
    s1ap_E_RABToBeModifiedListBearerModReq_element *p_erab_elem = PNULL;

    erab_to_be_modified_list_t    *p_to_modify_list     = PNULL;
    erab_failed_to_modify_list_t  *p_failed_modify_list = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_msg);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_to_modify_list = &(p_ue_context->p_curr_proc_data->u.
        erab_modify_request_data.erab_to_be_modified_list);

    p_failed_modify_list = &(p_ue_context->p_curr_proc_data->u.
        erab_modify_request_data.erab_failed_to_modify_list);
    
    /* 
     ** Abnormal checks for ERAB Modify Procedure
     ** =========================================
     ** For each ERAB ID received in ERAB to be modified List IE 
     ** verify the following:
     ** If ERAB ID value does not match any of the ERAB ID
     ** present in UE context, mark the ERAB ID in ERAB failed 
     ** to modify list IE present in UE context with 
     ** failure cause s1ap_unknown_E_RAB_ID.
     **
     ** If ERAB ID value matches the ERAB ID in ERAB Modify List IE 
     ** present in UE context, mark the ERAB ID in ERAB failed to Modify list IE
     ** with failure cause s1ap_multiple_E_RAB_ID_instances. 
     ** Delete the same ERAB ID value from ERAB Modify List IE
     ** present in UE context.
     **
     ** In ERAB to be modified list IE, if E-RAB Level QoS Parameters IE 
     ** contain QCI IE indicating a GBR bearer but does not contain the 
     ** GBR QoS Information IE, mark the ERAB ID in ERAB failed to 
     ** modified list IE
     ** present in UE context with failure cause s1ap_invalid_qos_combination.
     **
     */

    for (p_node = p_msg->e_rab_to_be_modified_list.head;
            (PNULL!=p_node) && (e_rab_list_count < MAX_MME_ERAB_LIST_COUNT); 
            p_node = p_node->next,e_rab_list_count++)
    {
        p_erab_elem=
            (s1ap_E_RABToBeModifiedListBearerModReq_element*)p_node->data;
        if ((p_erab_elem != PNULL) && (p_erab_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1 != PNULL))
        {
            /* check for valid IDs was performed before when decoding */
            p_e_rab_list[e_rab_list_count] = p_erab_elem->value.u._E_RABToBeModifiedItemBearerModReqIEs_1;
        }
    }

    /*Coverity Fix 30410*/ 
    if ( RRC_NULL != e_rab_list_count )
    {
        if (RRC_SUCCESS != uecc_validate_and_fill_erab_list(p_ue_context,
                    p_e_rab_list, 
                    p_to_modify_list, 
                    p_failed_modify_list, 
                    e_rab_list_count))
        {
            result = RRC_FAILURE;
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                    "No E-RAB to be processed for Modification ");
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * FUNCTION NAME : uecc_validate_and_fill_erab_list
 *
 * INPUT         : uecc_ue_context_t                      *p_ue_context,
 *                 s1ap_E_RABToBeModifiedItemBearerModReq *p_e_rab_list[],
 *                 erab_to_be_modified_list_t             *p_to_modify_list,
 *                 erab_failed_to_modify_list_t           *p_failed_modify_list,
 *                 U16                                    count
 *
 * OUTPUT        : None
 *
 * RETURNS       : rrc_return_t
 *
 * DESCRIPTION   : This function validates the E-RAB list and populates the 
 *                 Modify List and Falied to Modify List. 
 *
 ****************************************************************************/
rrc_return_t uecc_validate_and_fill_erab_list
(
    uecc_ue_context_t                      *p_ue_context,
    s1ap_E_RABToBeModifiedItemBearerModReq *p_e_rab_list[],
    erab_to_be_modified_list_t             *p_to_modify_list,
    erab_failed_to_modify_list_t           *p_failed_modify_list,
    U16                                    count
)
{   
    rrc_return_t result = RRC_SUCCESS;

    U16 e_rab_index = RRC_NULL;
    U16 list_index  = RRC_NULL;   
    U8  mod_index   = RRC_NULL;   
    U8  fail_index  = RRC_NULL;   

    U8 local_array[MAX_ERAB_COUNT] = {0};

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (PNULL != p_ue_context->p_gb_context)
    {
	    if (p_ue_context->p_gb_context->p_p_csc_context == PNULL || 
	            p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index] == NULL)
	    {
	        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, p_ue_context->
                    p_gb_context->facility_name, RRC_WARNING,"CELL Context is NULL");

	    }
    }

    for (e_rab_index = 0; (e_rab_index < count && 
            count < MAX_MME_ERAB_LIST_COUNT && 
            fail_index < MAX_SUPPORTED_E_RAB_ID); 
         e_rab_index++)
    {
        if (p_e_rab_list[e_rab_index]->e_RAB_ID > MAX_SUPPORTED_E_RAB_ID)
        {
            /* Save the e_rab_id in the failed list */
            p_failed_modify_list->erab_failed_to_modify_item
                    [fail_index].e_RAB_ID =  
            p_e_rab_list[e_rab_index]->e_RAB_ID;
            
            /* Update the cause. */
            p_failed_modify_list->erab_failed_to_modify_item
                [fail_index].cause.t = T_s1ap_Cause_radioNetwork;
            p_failed_modify_list->erab_failed_to_modify_item[fail_index].
                cause.u.radioNetwork = s1ap_unknown_E_RAB_ID;                      

            p_failed_modify_list->erab_failed_to_modify_item
                [fail_index].error_code = RRC_ERROR_CODE_LAST;

            /* Increment the counters for failed to modify list*/
            fail_index++;
            p_failed_modify_list->erab_failed_to_modify_list_counter++; 
        }
        else
        {
            list_index = (U16)p_e_rab_list[e_rab_index]->e_RAB_ID;
            local_array[list_index]++;
        }
    }

    for (e_rab_index = 0; 
            ((e_rab_index < MAX_MME_ERAB_LIST_COUNT) &&
             (e_rab_index < count) &&
             (fail_index < MAX_SUPPORTED_E_RAB_ID)&&
             (mod_index < MAX_SUPPORTED_E_RAB_ID)); 
            e_rab_index++)
    {
        if(p_e_rab_list[e_rab_index]->e_RAB_ID > MAX_SUPPORTED_E_RAB_ID)
        {
            continue;
        }
        else
        {
            list_index = (U16)p_e_rab_list[e_rab_index]->e_RAB_ID;

            if (PNULL != p_ue_context->p_gb_context && PNULL != g_rrc_stats.p_rrc_stats)
            {
                /* SPR 13441 Fix Start */
                if ( (RRC_MIN_QCI_GBR_LIMIT <= p_e_rab_list[e_rab_index]->
                         e_RABLevelQoSParameters.qCI) && (RRC_MAX_QCI_NON_GBR_LIMIT >= 
                             p_e_rab_list[e_rab_index]->e_RABLevelQoSParameters.qCI) )
                {
                    /* Update the ERAB Modification Attempts */
                    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context, 
                            g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].
                            rrc_erab_related_stats.erab_mod_stats.
                            erab_modQosAttNbr_qci[RRC_UPDATED_QCI_INDEX(p_e_rab_list[e_rab_index]->e_RABLevelQoSParameters.qCI)],
                            RRC_UPDATED_QCI_INDEX(p_e_rab_list[e_rab_index]->e_RABLevelQoSParameters.qCI),
                            RRC_MAX_QCI_INDEX);

                    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context, 
                            g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_erab_related_stats.erab_mod_stats.
                            erab_modQosAttNbr_sum, 
                            RRC_UPDATED_QCI_INDEX(p_e_rab_list[e_rab_index]->e_RABLevelQoSParameters.qCI),
                            RRC_MAX_QCI_INDEX);
                }
                /* SPR 13441 Fix End */

            }
            if ( local_array[list_index] == 1) 
            {
                if (p_ue_context->p_e_rab_list[list_index] != PNULL )
                {
                    /* Save it in the to be modified list to be sent to RRM*/
                    p_to_modify_list->erab_to_be_modified_item[mod_index].
                        e_RAB_ID =
                            p_e_rab_list[e_rab_index]->e_RAB_ID;  
                    /* Save it in the to be modified list to be sent to RRM*/
                    p_to_modify_list->erab_to_be_modified_item[mod_index].
                        e_RAB_ID = p_e_rab_list[e_rab_index]->e_RAB_ID;

                    asn1Copy_s1ap_E_RABLevelQoSParameters(
                        &p_ue_context->s1ap_asn1_ctx,
                                &p_e_rab_list[e_rab_index]->e_RABLevelQoSParameters,
                        &p_to_modify_list->
                        erab_to_be_modified_item[mod_index].
                        e_RABlevelQoSParameters);
                    
                    asn1Copy_s1ap_NAS_PDU(&p_ue_context->s1ap_asn1_ctx,
                            &p_e_rab_list[e_rab_index]->nAS_PDU,
                            &p_to_modify_list->erab_to_be_modified_item
                            [mod_index].nAS_PDU);

                    /* Update the count of E-RABs to be modified 
                     * in the UE Context */
                    mod_index++;
                    p_to_modify_list->erab_to_be_modified_list_counter++;
                    //                }
            }
            else
            {
                /* Save the e_rab_id in the failed list */
                p_failed_modify_list->erab_failed_to_modify_item
                    [fail_index].e_RAB_ID =  
                            p_e_rab_list[e_rab_index]->e_RAB_ID;

                /* Update the cause. */
                p_failed_modify_list->erab_failed_to_modify_item
                    [fail_index].cause.t =
                    T_s1ap_Cause_radioNetwork;
                p_failed_modify_list->erab_failed_to_modify_item[fail_index].
                    cause.u.radioNetwork = s1ap_unknown_E_RAB_ID;              

                p_failed_modify_list->erab_failed_to_modify_item
                    [fail_index].error_code = RRC_ERROR_CODE_LAST;

                /* Increment the counter */
                fail_index++;
                p_failed_modify_list->erab_failed_to_modify_list_counter++;

            }
        }
        else if ( local_array[list_index] > 1)
        {
            if (p_ue_context->p_e_rab_list[list_index] != PNULL)
            {
                if (RRC_TRUE == 
                        uecc_is_failed_list_updated(p_failed_modify_list, 
                            (s1ap_E_RAB_ID) list_index))
                {
                    /* Do Nothing */
                }
                else
                {
                    /* Find Multiple Occurances, if any */
                    /* Save the e_rab_id in the failed list */
                    p_failed_modify_list->erab_failed_to_modify_item
                        [fail_index].e_RAB_ID =  
                        p_e_rab_list[e_rab_index]->e_RAB_ID;

                    /* Update the cause. */
                    p_failed_modify_list->erab_failed_to_modify_item
                        [fail_index].cause.t =
                        T_s1ap_Cause_radioNetwork;

                    p_failed_modify_list->erab_failed_to_modify_item[fail_index].
                        cause.u.radioNetwork = s1ap_multiple_E_RAB_ID_instances;

                    p_failed_modify_list->erab_failed_to_modify_item
                        [fail_index].error_code = RRC_ERROR_CODE_LAST;

                    /* Increment the counters */
                    fail_index++;
                    p_failed_modify_list->erab_failed_to_modify_list_counter++;
                }
            }
            else
            {
                /* Save the e_rab_id in the failed list */
                p_failed_modify_list->erab_failed_to_modify_item
                    [fail_index].e_RAB_ID =  
                    p_e_rab_list[e_rab_index]->e_RAB_ID;
                /* Update the cause. */
                p_failed_modify_list->erab_failed_to_modify_item
                    [fail_index].cause.t =
                    T_s1ap_Cause_radioNetwork;
                p_failed_modify_list->erab_failed_to_modify_item[fail_index].
                    cause.u.radioNetwork = s1ap_unknown_E_RAB_ID;              

                p_failed_modify_list->erab_failed_to_modify_item
                    [fail_index].error_code = RRC_ERROR_CODE_LAST;

                /* Increment the counters */
                fail_index++;
                p_failed_modify_list->erab_failed_to_modify_list_counter++;
            }
        }      
    }
    }

    if (p_to_modify_list->erab_to_be_modified_list_counter == RRC_NULL)
    {
        result = RRC_FAILURE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * FUNCTION NAME : uecc_is_failed_list_updated
 *
 * INPUT         : erab_failed_to_modify_list_t           *p_failed_modify_list,
 *                 s1ap_E_RAB_ID                          erab_id
 *
 *
 * OUTPUT        : None
 *
 * RETURNS       : ret
 *
 * DESCRIPTION   : This function check if the erab_id is already present in the 
 *                          failed list.
 *
 ****************************************************************************/
rrc_bool_et uecc_is_failed_list_updated(
     erab_failed_to_modify_list_t           *p_failed_modify_list,
     s1ap_E_RAB_ID                          erab_id
)
{
    U16 fail_idx = RRC_NULL;
    rrc_bool_et ret = RRC_FALSE;

    for (fail_idx = 0;
        fail_idx < p_failed_modify_list->erab_failed_to_modify_list_counter; 
        fail_idx++)
    {
        if (erab_id == 
                p_failed_modify_list->erab_failed_to_modify_item[fail_idx].e_RAB_ID)
        {
            ret = RRC_TRUE;            
        }
    }

    return ret;

}

/****************************************************************************
 * FUNCTION NAME : uecc_update_ue_context
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,

 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This function updates the UE Context.
 *
 ****************************************************************************/
void uecc_update_ue_context(
    uecc_ue_context_t*  p_ue_context
)
{


    erab_modify_list_t *p_mod_list = PNULL;
    RRC_ASSERT(PNULL != p_ue_context);
    /*SPR_16942_START*/
    rrm_mac_config_t *p_mac_config = PNULL;
    /*SPR_16942_END*/    
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_mod_list = &(p_ue_context->p_curr_proc_data->u.
            erab_modify_request_data.erab_modify_list);

    /* Update the UE AMBR */

    p_ue_context->ue_aggregate_maximum_bitrate =
        p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
        ue_aggregate_maximum_bitrate;

    /* Update the DRB config in the UE Context as the procedure is now complete.*/
    uecc_update_drb_config_in_ue_context(p_ue_context, p_mod_list);
    /*SPR_16942_START*/
    if (p_ue_context->p_curr_proc_data->
        u.erab_modify_request_data.m.mac_config_updated == 1)
    {
        p_mac_config =   &(p_ue_context->p_curr_proc_data->u.erab_modify_request_data.mac_config);
        uecc_update_mac_config_in_ue_context(p_ue_context,p_mac_config);
    }
    /* Code Removed */    
    /*SPR_16942_END*/
    if (p_ue_context->p_curr_proc_data->
        u.erab_modify_request_data.m.sps_config_updated == 1)
    {

        if( RRC_SPS_CONFIG_SPS_CONFIG_DL_PRESENT &
            p_ue_context->p_curr_proc_data->
            u.erab_modify_request_data.sps_config.bitmask)
        {
            p_ue_context->p_sps_config->sps_config_dl = 
                p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
                sps_config.sps_config_dl;
        }       

        if( RRC_SPS_CONFIG_SPS_CONFIG_UL_PRESENT &
            p_ue_context->p_curr_proc_data->
            u.erab_modify_request_data.sps_config.bitmask)
        {
            p_ue_context->p_sps_config->sps_config_ul = 
                p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
                sps_config.sps_config_ul;
        }  
    }
    /* SPS Stop */
    /* Copy Proximity Configuration */
    p_ue_context->m.proximity_config_present = 
        p_ue_context->p_curr_proc_data->u.erab_modify_request_data.m.proximity_config_present;

    l3_memcpy_wrapper(&(p_ue_context->proximity_config), &( p_ue_context->
                p_curr_proc_data->u.erab_modify_request_data.proximity_config),
            sizeof(rrm_report_proximity_config_r9_t));


    /* Meas Config is already updated into Curr Meas on receiving the
     * RRC Reconfiguration Complete Message from UE */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return;

}
/*SPR_16942_START*/
/****************************************************************************
 * FUNCTION NAME : uecc_update_mac_config_in_ue_context
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 rrm_mac_config_t*   p_mac_config_t            
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This function updates the MAC_CONFIG in UE Context.
 *
 ****************************************************************************/
void uecc_update_mac_config_in_ue_context(
        uecc_ue_context_t*  p_ue_context,
        rrm_mac_config_t*   p_mac_config
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* MAC configuration */
    if( RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT &
            p_mac_config->bitmask)
    {
        /* Updating UL_SCH Configuration in UE Context */ 
        /*SPR_17969_Start*/
        if(p_mac_config->mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_UL_SCH_CONFIG_PRESENT)
        {
            p_ue_context->mac_config.mac_main_config.bitmask |= RRC_MAC_MAIN_CONFIG_UL_SCH_CONFIG_PRESENT;
            if(RRC_UL_SCH_CONFIG_MAX_HARQ_TX_PRESENT & p_mac_config->mac_main_config.ul_sch_config.bitmask)
            {
                p_ue_context->mac_config.mac_main_config.ul_sch_config.bitmask 
                    |= RRC_UL_SCH_CONFIG_MAX_HARQ_TX_PRESENT;
                p_ue_context->mac_config.mac_main_config.ul_sch_config.max_harq_tx
                    = p_mac_config->mac_main_config.ul_sch_config.max_harq_tx;
            }
            if(RRC_UL_SCH_CONFIG_PERIODIC_BSR_TIMER_PRESENT & p_mac_config->mac_main_config.ul_sch_config.bitmask)
            {
                p_ue_context->mac_config.mac_main_config.ul_sch_config.bitmask 
                    |= RRC_UL_SCH_CONFIG_PERIODIC_BSR_TIMER_PRESENT;
                p_ue_context->mac_config.mac_main_config.ul_sch_config.periodic_bsr_timer
                    = p_mac_config->mac_main_config.ul_sch_config.periodic_bsr_timer;
            }
            p_ue_context->mac_config.mac_main_config.ul_sch_config.retx_bsr_timer
                = p_mac_config->mac_main_config.ul_sch_config.retx_bsr_timer;
            p_ue_context->mac_config.mac_main_config.ul_sch_config.tti_bundling
                = p_mac_config->mac_main_config.ul_sch_config.tti_bundling;
        }
        /*SPR_17969_End*/

        /* Updating DRX Configuration in UE Context */
        if ( RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT &
                p_mac_config->mac_main_config.bitmask)
        {
            if((p_ue_context->mac_config.bitmask & RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT) &&
                    (p_ue_context->mac_config.mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT))
            {
                if(p_mac_config->mac_main_config.drx_config.drx_config_type == RRC_DRX_CONFIG_TYPE_REL)
                {
                    /* Reset DRX Config Bitmask and Remove DRX Config from UE context */
                    p_ue_context->mac_config.mac_main_config.bitmask &= ~(RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT);
                }
                else
                {
                    p_ue_context->mac_config.mac_main_config.drx_config = p_mac_config->mac_main_config.drx_config;
                }
            }
            else
            {
                if((p_mac_config->mac_main_config.drx_config.drx_config_type == RRC_DRX_CONFIG_TYPE_SETUP))
                { 
                    p_ue_context->mac_config.mac_main_config.bitmask |= RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT;
                    p_ue_context->mac_config.mac_main_config.drx_config = p_mac_config->mac_main_config.drx_config;
                }
                else
                {
                    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,"Invalid DRX CONFIG TYPE %u Recived",p_mac_config->mac_main_config.drx_config.drx_config_type); 
                }
            }
        }   

        /* Updating Time alignment in UE Context */
        p_ue_context->mac_config.mac_main_config.time_alignment_timer_dedicated = p_mac_config->mac_main_config.time_alignment_timer_dedicated;

        /* Updating PHR Configuration in UE Context */
        if ( RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT &
                p_mac_config->mac_main_config.bitmask)
        {
            if( (p_ue_context->mac_config.bitmask & RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT) &&
                (p_ue_context->mac_config.mac_main_config.bitmask & RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT))
            {
                if(!(p_mac_config->mac_main_config.phr_config.bitmask & RRC_PHR_CONFIG_PARAM_PRESENT))
                {
                    /* Reset PHR Config Bitmask and Remove PHR Config from UE context */
                    p_ue_context->mac_config.mac_main_config.bitmask &= ~(RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT);
                }
                else
                {
                    p_ue_context->mac_config.mac_main_config.phr_config = p_mac_config->mac_main_config.phr_config;
                }
            }  
            else
            {
                if((p_mac_config->mac_main_config.phr_config.bitmask & RRC_PHR_CONFIG_PARAM_PRESENT))
                { 
                    p_ue_context->mac_config.mac_main_config.bitmask |= RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT;
                    p_ue_context->mac_config.mac_main_config.phr_config = p_mac_config->mac_main_config.phr_config;
                }
                else
                {
                    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,"Invalid PHR CONFIG TYPE Recived"); 
                }
            }
        }

        /* Updating SR_PROHIBIT_TIMER_R9 in UE Context */
        if ( RRC_MAC_MAIN_CONFIG_SR_PROHIBIT_TIMER_R9_PRESENT &
                p_mac_config->mac_main_config.bitmask)
        {
            p_ue_context->mac_config.mac_main_config.bitmask |= RRC_MAC_MAIN_CONFIG_SR_PROHIBIT_TIMER_R9_PRESENT;
            p_ue_context->mac_config.mac_main_config.sr_prohibit_timer_r9 = p_mac_config->mac_main_config.sr_prohibit_timer_r9;
        }

        /*SPR_17969_Start*/
        /*Code Removed*/
        /*SPR_17969_End*/
        p_ue_context->m.mac_main_config_present = 1;
        p_ue_context->mac_config.bitmask |= RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT;

        /* SES-469 Changes - Starts */
        /* Updating MAC_MAIN_CONFIG_V1020 in UE Context */
        if ( RRC_MAC_MAIN_CONFIG_V1020_PRESENT & p_mac_config->mac_main_config.bitmask)
        {
            p_ue_context->mac_config.mac_main_config.bitmask |= RRC_MAC_MAIN_CONFIG_V1020_PRESENT;
            p_ue_context->mac_config.mac_main_config.mac_main_config_v1020 = p_mac_config->mac_main_config.mac_main_config_v1020;
        }
        /* SES-469 Changes - Ends */
    }
    /* Updating MAC_MAIN_CONFIG_EXTENSION in UE Context */
    p_ue_context->mac_config.mac_main_config_extensions =  p_mac_config->mac_main_config_extensions;
    p_ue_context->m.mac_config_present = 1;
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return;
}
/*SPR_16942_END*/

/****************************************************************************
 * FUNCTION NAME : uecc_update_drb_config_in_ue_context
 *
 * INPUT         :uecc_ue_context_t*  p_ue_context,
 *                erab_modify_list_t *p_mod_list
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This function updates DRB configuration in UE Context.
 * 
 ****************************************************************************/
void uecc_update_drb_config_in_ue_context(
    uecc_ue_context_t*  p_ue_context,
    erab_modify_list_t *p_mod_list 
)
{
    U8 erab_index = RRC_NULL;
    U8 drb_index  = RRC_NULL;

    rrc_counter_t  erab_count = RRC_NULL;

    /* Get the counter */
    erab_count = p_mod_list->erab_modify_list_counter;

    for (drb_index = 0; drb_index < erab_count; drb_index++)
    {
        erab_index = p_mod_list->erab_modify_item
            [drb_index].drb_config.erab_id;
        if (p_mod_list->erab_modify_item[drb_index].status == RRC_SUCCESS)
        {

            p_ue_context->p_e_rab_list[erab_index]->m.nAS_PDUPresent  = 1;
            p_ue_context->p_e_rab_list[erab_index]->nAS_PDU = 
                p_mod_list->erab_modify_item[drb_index].nAS_PDU;

            if (RRM_DRB_CONFIG_BEARED_ID_PRESENT & 
                p_mod_list->erab_modify_item[drb_index].drb_config.bitmask)
            {
                p_ue_context->p_e_rab_list[erab_index]->drb_config.erab_id = 
                    p_mod_list->erab_modify_item[drb_index].drb_config.erab_id;
            }

            if (RRM_DRB_CONFIG_LOGICAL_CHANNEL_IDENTITY_PRESENT & 
                p_mod_list->erab_modify_item[drb_index].drb_config.bitmask)
            {
                p_ue_context->p_e_rab_list[erab_index]->
                    drb_config.logical_channel_identity = 
                    p_mod_list->erab_modify_item[drb_index].
                    drb_config.logical_channel_identity;
            }

            if (RRM_DRB_CONFIG_PDCP_CONFIG_PRESENT & 
                    p_mod_list->erab_modify_item[drb_index].drb_config.bitmask)
            {
                if (RRC_PDCP_CONFIG_DISCARD_TIMER_PRESENT & 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.pdcp_config.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->
                        drb_config.pdcp_config.discard_timer = 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.pdcp_config.discard_timer;
                }

                if (RRC_PDCP_CONFIG_RLC_AM_STATUS_REPORT_REQ_PRESENT & 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.pdcp_config.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->
                        drb_config.pdcp_config.rlc_am_status_report_required = 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.pdcp_config.rlc_am_status_report_required;
                }
                /* PSR Start */
                if (RRC_PDCP_CONFIG_RLC_AM_ENB_STATUS_REPORT_REQ_PRESENT & 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.pdcp_config.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->
                        drb_config.pdcp_config.rlc_am_enb_status_report_required = 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.pdcp_config.rlc_am_enb_status_report_required;
                }
                /* PSR End */
                if (RRC_PDCP_CONFIG_RLC_UM_PDCP_SN_SIZE_PRESENT & 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.pdcp_config.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->
                        drb_config.pdcp_config.rlc_um_pdcp_sn_size = 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.pdcp_config.rlc_um_pdcp_sn_size;
                }

                if (RRC_PDCP_HEADER_COMPRESSION_ROHC_CONFIG_PRESENT & 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.pdcp_config.header_compression.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->
                        drb_config.pdcp_config.header_compression.rohc_config = 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.pdcp_config.header_compression.rohc_config;
                }

            }


            if (RRM_DRB_CONFIG_RLC_CONFIG_PRESENT & 
                    p_mod_list->erab_modify_item[drb_index].drb_config.bitmask)
            {
                if (RRC_RLC_CONFIG_AM_CONFIG_PRESENT & 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.rlc_config_enb.rlc_config.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->
                        drb_config.rlc_config_enb.rlc_config.am_config = 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.rlc_config_enb.rlc_config.am_config;
                }      

                if (RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT & 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.rlc_config_enb.rlc_config.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->drb_config.
                        rlc_config_enb.rlc_config.um_bi_directional_config = 
                        p_mod_list->erab_modify_item[drb_index].drb_config.
                        rlc_config_enb.rlc_config.um_bi_directional_config;
                }             

                if (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT & 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.rlc_config_enb.rlc_config.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->drb_config.
                        rlc_config_enb.rlc_config.um_uni_directional_ul_config = 
                        p_mod_list->erab_modify_item[drb_index].drb_config.
                        rlc_config_enb.rlc_config.um_uni_directional_ul_config;
                }             

                if (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT & 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.rlc_config_enb.rlc_config.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->drb_config.
                        rlc_config_enb.rlc_config.um_uni_directional_dl_config = 
                        p_mod_list->erab_modify_item[drb_index].drb_config.
                        rlc_config_enb.rlc_config.um_uni_directional_dl_config;
                }  
            }

            if ( RRM_DRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT & 
                    p_mod_list->erab_modify_item[drb_index].drb_config.bitmask)
            {
                if (RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_PRESENT & 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.logical_channel_config.
                        ul_specific_parameters_explicit.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->
                        drb_config.logical_channel_config.
                        ul_specific_parameters_explicit.logical_channel_group = 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.logical_channel_config.
                        ul_specific_parameters_explicit.logical_channel_group;
                } 

                p_ue_context->p_e_rab_list[erab_index]->
                    drb_config.logical_channel_config.
                    ul_specific_parameters_explicit.priority = 
                    p_mod_list->erab_modify_item[drb_index].
                    drb_config.logical_channel_config.
                    ul_specific_parameters_explicit.priority;

                p_ue_context->p_e_rab_list[erab_index]->
                    drb_config.logical_channel_config.
                    ul_specific_parameters_explicit.prioritized_bit_rate = 
                    p_mod_list->erab_modify_item[drb_index].
                    drb_config.logical_channel_config.
                    ul_specific_parameters_explicit.prioritized_bit_rate;

                p_ue_context->p_e_rab_list[erab_index]->
                    drb_config.logical_channel_config.
                    ul_specific_parameters_explicit.bucket_size_duration = 
                    p_mod_list->erab_modify_item[drb_index].
                    drb_config.logical_channel_config.
                    ul_specific_parameters_explicit.bucket_size_duration;

                /* lc_sr_mask_r9 */ 
                if (RRM_UL_SPECIFIC_PARAMETERS_LC_SR_MASK_R9_PRESENT &
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.logical_channel_config.bitmask)
                {
                    if (RRM_UL_SPECIFIC_PARAMETERS_EXPLICIT_PRESENT & 
                            p_mod_list->erab_modify_item[drb_index].
                            drb_config.logical_channel_config.bitmask)
                    {
                        p_ue_context->p_e_rab_list[erab_index]->drb_config.
                            logical_channel_config.lc_sr_mask_r9 =
                            p_mod_list->erab_modify_item[drb_index].drb_config.
                            logical_channel_config.lc_sr_mask_r9;
                    }
                    else
                    {
                        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                             "p_mod_list->erab_modify_item[drb_index]."
                             "drb_config.logical_channel_config."
                             "ul_specific_parameters_explicit");
                       
                    }
                } 
            }

            if ( RRM_DRB_CONFIG_S1U_CONFIG_PRESENT & 
                    p_mod_list->erab_modify_item[drb_index].drb_config.bitmask)
            {
                p_ue_context->p_e_rab_list[erab_index]->
                    drb_config.s1u_config = 
                    p_mod_list->erab_modify_item[drb_index].drb_config.s1u_config;
            }

            if ( RRM_DRB_CONFIG_MAC_LC_CONFIG_ENB_PRESENT & 
                    p_mod_list->erab_modify_item[drb_index].drb_config.bitmask)
            {
                if (RRM_MAC_LC_CONFIG_UL_LC_G_ID_PRESENT  &
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.mac_lc_config_enb.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->
                        drb_config.mac_lc_config_enb.ul_lc_g_id = 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.mac_lc_config_enb.ul_lc_g_id;
                }
                if (RRM_MAC_LC_CONFIG_DL_PRIORITY_PRESENT &
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.mac_lc_config_enb.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->
                        drb_config.mac_lc_config_enb.dl_lch_priority = 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.mac_lc_config_enb.dl_lch_priority;
                }
            }

            if ( RRM_DRB_CONFIG_RLC_CONFIG_UE_PRESENT & 
                    p_mod_list->erab_modify_item[drb_index].drb_config.bitmask)
            {
                if (RRC_RLC_CONFIG_AM_CONFIG_PRESENT & 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.rlc_config_ue.bitmask)
                {

                    p_ue_context->p_e_rab_list[erab_index]->
                        drb_config.rlc_config_ue.am_config = 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.rlc_config_ue.am_config;
                }

                if (RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT  &
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.rlc_config_ue.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->drb_config.
                        rlc_config_ue.um_bi_directional_config = 
                        p_mod_list->erab_modify_item[drb_index].drb_config.
                        rlc_config_ue.um_bi_directional_config;
                }
                if (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT & 
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.rlc_config_ue.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->drb_config.
                        rlc_config_ue.um_uni_directional_ul_config =
                        p_mod_list->erab_modify_item[drb_index].drb_config.
                        rlc_config_ue.um_uni_directional_ul_config;
                }            
                if (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT &
                        p_mod_list->erab_modify_item[drb_index].
                        drb_config.rlc_config_ue.bitmask)
                {
                    p_ue_context->p_e_rab_list[erab_index]->drb_config.
                        rlc_config_ue.um_uni_directional_dl_config =
                        p_mod_list->erab_modify_item[drb_index].drb_config.
                        rlc_config_ue.um_uni_directional_dl_config;
                } 
            }
        }
    }
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erab_modify_process_rrm_erab_modify_resp_data
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 rrc_rrm_erab_modify_resp_t* p_rrm_erb_modify_resp 
 *
 * OUTPUT        : NONE
 *
 * RETURNS       : rrc_return_t
 *
 * DESCRIPTION   : This is the function Handles RRC_RRM_ERB_MODIFY_RESP message 
 * 
 ****************************************************************************/
rrc_return_t uecc_fsm_erab_modify_process_rrm_erab_modify_resp_data(
    uecc_ue_context_t*  p_ue_context,
    rrc_rrm_erab_modify_resp_t* p_rrm_erb_modify_resp
)
{
    rrc_return_t result    = RRC_SUCCESS;
    U8           idx       = RRC_NULL;
    U8           drb_index = RRC_NULL;
    s1ap_Cause   cause;
    U32          count     = RRC_NULL;
    U32          error_code;


    erab_modify_request_data_t   *p_mod_req_data  = PNULL;
    erab_to_be_modified_list_t   *p_erab_to_be_mod_list = PNULL;
    erab_failed_to_modify_list_t *p_failed_list   = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_rrm_erb_modify_resp);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(p_ue_context->p_gb_context,
                p_ue_context->cell_index))
    {

        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,p_ue_context->
                p_gb_context->facility_name ,RRC_WARNING,"Cell not valid or not active ");

        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_cell_not_available;
        error_code = RRC_INTERNAL_ERROR;

        RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);

        uecc_update_failed_list_from_to_be_modified_list(
                p_ue_context, 
                cause, 
                error_code);
        result = RRC_FAILURE;

        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return result;
    }
    do
    {
        p_mod_req_data =
            &(p_ue_context->p_curr_proc_data->u.erab_modify_request_data);

        p_failed_list = &(p_mod_req_data->erab_failed_to_modify_list);

        p_erab_to_be_mod_list = &(p_mod_req_data->erab_to_be_modified_list);

        /* Store UE parameters in curr_proc_data. */

        /* Store E-RAB Modify data in the modify list */
        if (RRM_ERAB_MODIFY_RADIO_RES_CONFIG_DRB_TO_MODIFY_LIST_PRESENT &
                p_rrm_erb_modify_resp->radio_rsource_config.bitmask)
        { 
            if (p_rrm_erb_modify_resp->response == SUCCESS)
            {
                /* Bug_10698 Fix Start */
                p_ue_context->p_meas_config = (rrm_meas_config_t*) 
                    rrc_mem_get (sizeof (rrm_meas_config_t));
                if (PNULL == p_ue_context->p_meas_config)
                {
                    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_FATAL,
                            "memory allocation failed to store received "
                            "MEAS_CONFIG in RRC_RRM_ERB_MODIFY_RESP");
                    return RRC_FAILURE;
                }
                //p_ue_context->p_meas_config = &p_rrm_erb_modify_resp->radio_rsource_config.meas_config;
                l3_memcpy_wrapper(p_ue_context->p_meas_config,
                        &p_rrm_erb_modify_resp->radio_rsource_config.meas_config,
                        sizeof(rrm_meas_config_t));
                /* Bug_10698 Fix Stop */
                if (p_rrm_erb_modify_resp->radio_rsource_config.
                        drb_to_modify_list.drb_count !=
                        p_erab_to_be_mod_list->erab_to_be_modified_list_counter)
                {
                    result = RRC_FAILURE;
                    break;
                }
            }
            else if (p_rrm_erb_modify_resp->response == PARTIAL_SUCCESS)
            {
                count +=  p_rrm_erb_modify_resp->radio_rsource_config.
                    drb_to_modify_list.drb_count; 
            }

            /* Update the Modify List count */
            p_mod_req_data->erab_modify_list.erab_modify_list_counter =
                (rrc_counter_t)p_rrm_erb_modify_resp->radio_rsource_config.
                drb_to_modify_list.drb_count;

            for (drb_index = 0;
                    drb_index < p_mod_req_data->erab_modify_list.
                    erab_modify_list_counter;
                    drb_index++)
            {

                l3_memcpy_wrapper(
                        &(p_mod_req_data->erab_modify_list.
                            erab_modify_item[drb_index].drb_config),
                        &p_rrm_erb_modify_resp->radio_rsource_config.
                        drb_to_modify_list.drb_config[drb_index],
                        sizeof(rrm_drb_config_t));
            }

            for (drb_index = 0;
                    drb_index < p_rrm_erb_modify_resp->radio_rsource_config.
                    drb_to_modify_list.drb_count;
                    drb_index++)
            {
                for (idx = 0;
                        idx < p_erab_to_be_mod_list->erab_to_be_modified_list_counter;
                        idx++)
                {
                    if(p_erab_to_be_mod_list->erab_to_be_modified_item[idx].
                            e_RAB_ID ==
                            p_mod_req_data->erab_modify_list.erab_modify_item
                            [drb_index].drb_config.erab_id)
                    {
                        l3_memcpy_wrapper(
                                &(p_mod_req_data->erab_modify_list.
                                    erab_modify_item[drb_index].nAS_PDU),
                                &(p_erab_to_be_mod_list->erab_to_be_modified_item[idx].
                                    nAS_PDU),
                                sizeof(s1ap_NAS_PDU));
                    }
                }
            }   

        }
        else
        {
            result = RRC_FAILURE;
            break;
        }

        result =  prepare_erab_modify_list(p_ue_context,
                p_rrm_erb_modify_resp,
                p_erab_to_be_mod_list,
                p_failed_list,
                &cause,
                &count);
        if (RRC_FAILURE == result)
        {
            break;
        }

        /*SPR_16942_START*/
        if (RRM_ERAB_MODIFY_RADIO_RES_CONFIG_MAC_CONFIG_PRESENT &
                p_rrm_erb_modify_resp->radio_rsource_config.bitmask)
        {
            p_mod_req_data->m.mac_config_updated = 1;
            p_mod_req_data->mac_config =
                p_rrm_erb_modify_resp->radio_rsource_config.mac_config;
        }
        /*SPR_16942_END*/      
        result =  fill_erabModify_sps_meas_timer_info(p_ue_context,
                p_rrm_erb_modify_resp,
                p_mod_req_data );

        if (RRC_FAILURE == result)
        {
            break;
        }

        /*SPR_19066_START*/
        if (RRM_ERB_MODIFY_RESP_API_UE_INACTIVE_TIME_PRESENT
                & p_rrm_erb_modify_resp->bitmask)
        {
            p_ue_context->m.ue_inactive_time_updated = RRC_TRUE;
            p_ue_context->ue_inactive_time = 
                p_rrm_erb_modify_resp->ue_inactive_time_config;
        }
        /*SPR_19066_END*/

    }while(0);

    if (result == RRC_FAILURE)
    {
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_unspecified_2;
        error_code = RRC_INTERNAL_ERROR;

        RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);

        uecc_update_failed_list_from_to_be_modified_list(
                p_ue_context, 
                cause, 
                error_code);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}


/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_rrm_ue_release_req_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,         
                   void*               p_api  
 *
 * OUTPUT        : None
 * 
 * RETURNS       : None
 *
 * DESCRIPTION   : This function handles UECC_EV_RRM_UE_CONNECTION_REL_IND 
 *                 event.
 ****************************************************************************/

void uecc_fsm_erb_modify_rrm_ue_release_req_handler
(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
)
{
    s1ap_Cause cause;
    U32 error_code = RRC_NO_ERROR;

    rrc_rrm_ue_connection_release_ind_t *p_ue_connection_release_ind = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_ue_connection_release_ind = (rrc_rrm_ue_connection_release_ind_t *)p_api;

    /* Stop ERAB Modify procedural timer. */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_ERAB_MODIFY_GUARD_TIMER,
            &p_ue_context->timers_data);


    if (RRC_SUCCESS == uecc_s1ap_build_s1ap_cause(
            &cause, &p_ue_connection_release_ind->release_cause,
            p_ue_context))
    {
        /* Update the failed list */
        error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;
        RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);
        uecc_update_failed_list_from_to_be_modified_list(
                p_ue_context, 
                cause,
                error_code);
    }
    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index ,
                p_ue_context->p_gb_context->facility_name, 
                RRC_WARNING, "[RRC_RRM_UE_CONNECTION_RELEASE_IND] "
                "invalid cause value [%u][%u]",
                p_ue_connection_release_ind->release_cause.type,
                p_ue_connection_release_ind->release_cause.value);
    }

    /* Build and send RRC_RRM_ERB_MODIFY_CNF with failure to RRM */
    if(RRC_FAILURE == uecc_rrm_build_and_send_erb_modify_conf(
                p_ue_context, 
                FAILURE,
                error_code))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[RRC_RRM_ERB_MODIFY_CNF] Message Delivery failure");
    }

    /* Build and send the RRC_S1AP_ERAB_MODIFY_RESP to S1AP */
    if (RRC_SUCCESS !=
          uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[s1ap_E_RABModifyResponse] Delivery failure");
    }
    /* Push internal event to UE Main FSM 
     ** UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT along with decoded payload.
     */
    uecc_event_queue_push_event(
            UECC_EV_RRM_UE_CONNECTION_RELEASE_IND_INT,
            p_ue_context,
            p_api);

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Free the data for current procedure in the UE Context */
        rrc_mem_free(p_ue_context->p_curr_proc_data);  
        p_ue_context->p_curr_proc_data = PNULL;
    }

    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_csc_cell_delete_req_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This is the handler function for the message
 *                  UECC_EV_CSC_CELL_DELETE_REQ_INT.
 *
 ****************************************************************************/

void uecc_fsm_erb_modify_csc_cell_delete_req_handler
(
        uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
        void*               p_api          /* Input API buffer */
) 
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Stop Procedure Timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context,
            UECC_ERAB_MODIFY_GUARD_TIMER, 
            &p_ue_context->timers_data);

    uecc_event_queue_push_event(
            UECC_EV_CSC_CELL_DELETE_REQ_INT,
            p_ue_context,
            p_api);

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Free the data for current procedure in the UE Context */
        rrc_mem_free(p_ue_context->p_curr_proc_data);  
        p_ue_context->p_curr_proc_data = PNULL;
    }

    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_rrm_ho_prep_timer_expiry_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 * DESCRIPTION   : This function handles the UECC_EV_HO_PREP_TIMER_EXPIRY event.
 *
 ****************************************************************************/

void uecc_fsm_erb_modify_rrm_ho_prep_timer_expiry_handler
(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
)  
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        /* Check if HO message is present in the curr_proc_data */
        if (p_ue_context->p_curr_proc_data->msg_bfr_type == RRC_HO_REQUIRED_MSG)
        {
            if (  PNULL != p_ue_context->p_curr_proc_data->msg_bfr_rcvd )
            {
                p_ue_context->p_curr_proc_data->msg_bfr_rcvd = PNULL;
            }

            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO, 
                    "[UECC_EV_HO_PREP_TIMER_EXPIRY]"
                    "HO Prep Timer Expired");
            p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
            p_ue_context->ho_info.cause.u.radioNetwork = s1ap_tS1relocprep_expiry;

            /* Send HO Failure to the RRM */
            if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_failure( p_ue_context))
            {
                RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_ERROR,
                        "Fail to send the handover failure indication to RRM");
            }
        }
        else
        {
            RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO,
                    "[UECC_EV_HO_PREP_TIMER_EXPIRY]: Invalid Event Received");
        }


        /* Free the data for current procedure in the UE Context */
        rrc_mem_free(p_ue_context->p_curr_proc_data);  
        p_ue_context->p_curr_proc_data = PNULL;
    }

    /* Stop Procedure */
    uecc_fsm_finish_active_procedure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}

/*SPR 18758 Fix Start*/
/* Code Deleted */
/*SPR 18758 Fix Stop*/

/*SPR_18241_START*/
/* Code Removed */
/*SPR_18241_END*/

/*SPR 18758 Fix Start*/
/* Code Deleted */
/*SPR 18758 Fix Stop*/
/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_erab_release_ind_handler
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 void*               p_api
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This function discards the message and send the failure 
 *                 response (RRC_RRM_ERB_RELEASE_IND_CNF) message containing 
 *                 failure causes to MME.
 *
 ****************************************************************************/

void uecc_fsm_erb_modify_erab_release_ind_handler
(
        uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
        void*               p_api                  /* Input API buffer */
)
{

    U32 error_code = RRC_NO_ERROR;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    /* Build and send RRC_RRM_ERAB_RELEASE_IND_RESP to RRM
     ** with response code as FAILURE
     ** with error_code for each ERAB_ID mapped to 
     ** Interaction with other procedure
     */
    error_code = RRC_INTERACTION_WITH_OTHER_PROCEDURE;

    RRC_UECC_ERB_MODIFY_UPDATE_ERROR_CODE(error_code);

    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_ind_fail_resp(
                p_api,
                p_ue_context,
                FAILURE,
                error_code))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[RRC_RRM_ERB_RELEASE_IND_CNF] Delivery failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_modify_fail_action_send_rrm_cnf
 *
 * INPUT         :uecc_ue_context_t*  p_ue_context,
 *                rrc_bool_et*        p_break
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : Fail action.
 *
 ****************************************************************************/
void uecc_fsm_erb_modify_fail_action_send_rrm_cnf(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    U32 error_code;

    RRC_ASSERT( PNULL != p_ue_context );
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO, 
            "FAILURE: Send RRC_RRM_ERB_MODIFY_CNF.");

    error_code = RRC_NO_ERROR;
    /* Send message to RRM RRC_RRM_ERB_MODIFY_CNF(FAILURE) */
    if(RRC_SUCCESS != uecc_rrm_build_and_send_erb_modify_conf(p_ue_context, 
                FAILURE, error_code))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[RRC_RRM_ERB_MODIFY_CNF] Delivery failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_mod_fail_action_send_fail_resp_to_s1ap
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 rrc_bool_et*        p_break
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : Fail action.
 * 
 ****************************************************************************/
void uecc_fsm_erb_mod_fail_action_send_fail_resp_to_s1ap(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT( PNULL != p_ue_context );
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    /* Send the ASN. 1 E-RAB MODIFY RESPONSE message 
     * containing all failures to MME.*/
    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO, 
            "FAILURE: Send RRC_S1AP_ERAB_MODIFY_RESP.");

    /* Build and send the RRC_S1AP_ERAB_MODIFY_RESP msg to S1AP */
    if (RRC_SUCCESS !=
          uecc_s1ap_build_and_send_erab_modify_response(p_ue_context))
    {
        RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_WARNING,
                "[s1ap_E_RABModifyResponse] Delivery failure");
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_fsm_erb_mod_fail_action_send_ue_context_release_req
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 rrc_bool_et*        p_break
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : Sends the ASN.1 encoded UE Context Release request
 *
 ****************************************************************************/
void uecc_fsm_erb_mod_fail_action_send_ue_context_release_req
(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    s1ap_Cause cause; 
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO, "FAILURE: END");
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
 * FUNCTION NAME : uecc_fsm_erb_mod_fail_action_last
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 rrc_bool_et*        p_break
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : Handling of UECC_ERB_MODIFY_FAIL_ACTION_LAST
 *
 ****************************************************************************/
void uecc_fsm_erb_mod_fail_action_last(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_ERB_MODIFY_FSM_TRACE(RRC_INFO, "FAILURE: END");

    /* This function relists the UE Context as the lower layers 
     * are completely out of sync now */
    /* The UE context data should be removed from all the layers */

    /* Notify procedure about failure */
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void *) UECC_EV_S1AP_ERAB_MODIFY_RESP_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_update_failed_list_from_to_be_modified_list
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 s1ap_Cause          cause,
 *                 U32                 error
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This function updates the failed list from to be 
 *                 the modified  list.
 *
 ****************************************************************************/
void uecc_update_failed_list_from_to_be_modified_list
(
    uecc_ue_context_t*  p_ue_context,
    s1ap_Cause          cause,
    U32                 error
)
{
    erab_failed_to_modify_list_t *p_failed_list    = PNULL;    
    erab_modify_list_t           *p_mod_list       = PNULL;

    rrc_counter_t drb_idx  = RRC_NULL;
    rrc_counter_t fail_idx = RRC_NULL;
    rrc_counter_t mod_idx  = RRC_NULL;
    rrc_counter_t idx      = RRC_NULL;

    rrc_bool_et   update_list = RRC_TRUE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Map the cause received in RRC_RRM_UE_CONNECTION_REL_IND
     ** to S1AP cause for each ERAB in  erab_modify_list in
     ** current proc data 
     */

    p_failed_list = 
        &(p_ue_context->p_curr_proc_data->u.
                erab_modify_request_data.erab_failed_to_modify_list);

    p_mod_list =    &(p_ue_context->p_curr_proc_data->u.
            erab_modify_request_data.erab_modify_list);
    /*SPR 22036 Fix Start */
    mod_idx  = p_mod_list->erab_modify_list_counter;
    fail_idx = p_failed_list->erab_failed_to_modify_list_counter;

    /* Append this list to erab_failed to modify list in 
     ** current proc data. Then clear the erab_modify_list 
     ** in current proc data.
     */
    for (drb_idx = 0; 
         drb_idx < mod_idx && fail_idx < MAX_ERAB_COUNT; 
         drb_idx++)
    {
        for (idx=0; idx < fail_idx; idx++)
        {
            if (p_failed_list->erab_failed_to_modify_item[fail_idx].e_RAB_ID ==
                    p_mod_list->erab_modify_item[drb_idx].drb_config.erab_id)
            {
                update_list = RRC_FALSE;
                break;
            }
        }
        if (update_list == RRC_TRUE)
        {
            p_failed_list->erab_failed_to_modify_item[fail_idx].e_RAB_ID = 
                p_mod_list->erab_modify_item[drb_idx].drb_config.erab_id;
            /*SPR 22036 Test Fix Stop Case7*/

            p_failed_list->erab_failed_to_modify_item[fail_idx].cause = cause;
            /* This field will be ignored by RRM*/
            p_failed_list->erab_failed_to_modify_item[fail_idx].error_code =
                error;

            fail_idx++;
        }
        update_list = RRC_TRUE;
    }
    p_failed_list->erab_failed_to_modify_list_counter = fail_idx;
    /* Set the E-RAB Modify Procedure Data to 0x00. */
    memset_wrapper(p_mod_list, 0x00, sizeof(erab_modify_list_t));    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * FUNCTION NAME : uecc_update_failed_list_from_modified_list
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context,
 *                 s1ap_Cause          cause
 *
 * OUTPUT        : None
 *
 * RETURNS       : NONE
 *
 * DESCRIPTION   : This function updates the failed list from the modify list.
 *
 ****************************************************************************/
void uecc_update_failed_list_from_modified_list
(
    uecc_ue_context_t*  p_ue_context,
    s1ap_Cause          cause
)
{
    erab_failed_to_modify_list_t *p_failed_list    = PNULL;    
    erab_modify_list_t   *p_mod_list = PNULL;

    rrc_counter_t drb_idx  = RRC_NULL;
    rrc_counter_t fail_idx = RRC_NULL;
    rrc_counter_t mod_idx  = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_failed_list = 
        &(p_ue_context->p_curr_proc_data->u.
                erab_modify_request_data.erab_failed_to_modify_list);

    p_mod_list =    &(p_ue_context->p_curr_proc_data->u.
        erab_modify_request_data.erab_modify_list);

    mod_idx  = p_mod_list->erab_modify_list_counter;
    fail_idx = p_failed_list->erab_failed_to_modify_list_counter;

    for (drb_idx = 0; 
        drb_idx < mod_idx && fail_idx < MAX_ERAB_COUNT; 
        drb_idx++)
    {
        p_failed_list->erab_failed_to_modify_item[fail_idx].e_RAB_ID = 
            p_mod_list->erab_modify_item[drb_idx].drb_config.erab_id;

        p_failed_list->erab_failed_to_modify_item[fail_idx].cause = 
            cause;

        /* This field will be ignored by RRM*/
        p_failed_list->erab_failed_to_modify_item[fail_idx].error_code =
            (U32)RRC_NO_ERROR;

        fail_idx++;
    }
    p_failed_list->erab_failed_to_modify_list_counter = fail_idx;
    /* Set the E-RAB Modify Procedure Data to 0x00. */
    memset_wrapper(p_mod_list, 0x00, sizeof(erab_modify_list_t));    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * FUNCTION NAME : uecc_update_failure_list
 *
 *
 * INPUT         : uecc_ue_context_t*              p_ue_context,
 *                 rrc_rrm_erab_modify_resp_t*     p_rrc_rrm_erb_modify_resp
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 *
 * DESCRIPTION   : This function updates the failed list from the 
 *                 response received from RRM.
 ****************************************************************************/
void uecc_update_failure_list(
    uecc_ue_context_t*              p_ue_context,
    rrc_rrm_erab_modify_resp_t*     p_rrc_rrm_erb_modify_resp
)
{
    erab_modify_request_data_t*     p_mod_req_data = PNULL;
    erab_failed_to_modify_list_t*   p_failed_list  = PNULL;

    U8           drb_index = RRC_NULL;
    s1ap_Cause   cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rrc_rrm_erb_modify_resp);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_mod_req_data =
        &(p_ue_context->p_curr_proc_data->u.erab_modify_request_data);

    p_failed_list = &(p_mod_req_data->erab_failed_to_modify_list);   

    /* Save the failure cases in the failure list, if any  */
    if (RRM_ERAB_MODIFY_RADIO_RES_CONFIG_DRB_FAILED_TO_MODIFY_LIST_PRESENT &
            p_rrc_rrm_erb_modify_resp->radio_rsource_config.bitmask)
    { 
        if (p_rrc_rrm_erb_modify_resp->radio_rsource_config.
                drb_failed_to_modify_list.count)
        {
            for (drb_index = 0; 
                    ((drb_index < p_rrc_rrm_erb_modify_resp->radio_rsource_config.
                      drb_failed_to_modify_list.count) 
                     && (drb_index < MAX_ERAB_COUNT));
                    drb_index++)
            {
                p_failed_list->erab_failed_to_modify_item
                    [p_failed_list->erab_failed_to_modify_list_counter++].
                    e_RAB_ID =
                        p_rrc_rrm_erb_modify_resp->radio_rsource_config.
                        drb_failed_to_modify_list.drb_failed_to_add[drb_index].
                        erab_id;

                if (RRC_SUCCESS == uecc_s1ap_build_s1ap_cause(
                            &cause,
                            &(p_rrc_rrm_erb_modify_resp->radio_rsource_config.
                                drb_failed_to_modify_list.
                                drb_failed_to_add[drb_index].cause),
                            p_ue_context))
                {
                    p_failed_list->erab_failed_to_modify_item[drb_index].cause 
                        = cause;
                }
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return;
}

