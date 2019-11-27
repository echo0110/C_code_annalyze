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
 *  File Description : UECC RRM ERB RELEASE FSM functions implementation
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

#include "uecc_rrm_erab_relfsm.h"
/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,\
        p_ue_context->ue_index,\
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
        ##__VA_ARGS__)

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static void uecc_fsm_ue_rrm_erb_release_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
/* x2ap_drop2_rw_start - remove static declaration */
void uecc_fsm_ue_rrm_erb_release_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

void uecc_fsm_ue_rrm_erb_release_procedure_finish_fail(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

void uecc_fsm_rrm_erb_release_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
void uecc_fsm_rrm_erb_release_s1ap_reset_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
void uecc_fsm_rrm_erb_release_csc_delete_all_ue_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

void uecc_fsm_rrm_erb_release_rrm_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/*static void uecc_fsm_rrm_erb_release_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,         
    void*               p_api                 
);*/
void uecc_fsm_rrm_erb_release_erab_setup_req_discard_handler(
    uecc_ue_context_t*  p_ue_context,         /* UECC UE context */
    void*               p_api                 /* Input API buffer */ 
);
void uecc_fsm_rrm_erb_release_erab_modify_req_discard_handler(
    uecc_ue_context_t*  p_ue_context,         /* UECC UE context */
    void*               p_api                 /* Input API buffer */ 
);
void uecc_fsm_rrm_erb_release_erab_rel_cmd_discard_handler(
    uecc_ue_context_t*  p_ue_context,         /* UECC UE context */
    void*               p_api                 /* Input API buffer */ 
);
void uecc_fsm_rrm_erb_release_erab_rel_ind_discard_handler(
    uecc_ue_context_t*  p_ue_context,         /* UECC UE context */
    void*               p_api                 /* Input API buffer */ 
);

/*SPR_18241_START*/
/* Code Removed */
/*SPR_18241_END*/

/* State UECC_RRM_ERB_RELEASE_UE_RECONFIGURE_ONGOING */
static void uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static
void uecc_fsm_rrm_erb_release_rrc_connection_reconfiguration_complete_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

static 
void uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing_timer_expiry_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

/* State UECC_RRM_ERB_RELEASE_LLIM_RECONFIGURE_UE_ONGOING */
static void uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rrm_erb_release_llim_reconfigure_ue_resp_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);
static void
uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_reestablish_req_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

static
void uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_timer_expiry_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);


void uecc_fsm_rrm_erb_release_ongoing_radio_link_failure_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);
static void uecc_fsm_rrm_erb_release_ongoing_re_establish_req_handler(
    uecc_ue_context_t*   p_ue_context,          /* UECC UE context */
    void*                p_api_data             /* Input API buffer */
);

static 
rrc_return_t uecc_fsm_rrm_erb_release_process_rrm_e_rab_release_ind_msg(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/* Failure processing functions */
static void update_erab_release_list_with_cause(
    uecc_ue_context_t* p_ue_context,           /* UECC UE context */
    void*              p_cause                /* Input API buffer */
);
static void update_curr_proc_data_with_response_code(
        uecc_ue_context_t* p_ue_context,           /* UECC UE context */
        void*              p_response_code            /* Input API*/
);
void uecc_fsm_rrm_erb_release_ongoing_ho_required_handler(
        uecc_ue_context_t*  p_ue_context,
          void*               p_api
        );

static void process_delete_drb_resp_erab_rel(
        uecc_ue_context_t*    p_ue_context,
        rrc_uecc_llim_delete_lc_resp_t*   p_rrc_uecc_llim_delete_lc_resp,
        U8 counter );

#ifdef ENDC_ENABLED
static uecc_fsm_handler_record_t
        uecc_fsm_erb_release_ue_reconfigure_ongoing_endc_mht[] =
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

static uecc_fsm_handler_record_t 
       uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_endc_mht[] =
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

static uecc_fsm_activity_state_t 
       uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_endc =
{
   /* .s_name         =  */ (S8*)"UECC_LLIM_ERB_RELEASE_RECONFIGURE_UE_ONGOING",
   /* .s_id           =  */ (S8*)"ERAB_REL_LLIM",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_endc_mht
};

static uecc_fsm_activity_state_t uecc_fsm_erb_release_ue_reconfigure_ongoing_endc =
{
   /* .s_name         =  */ (S8*)"UECC_UE_ERB_RELEASE_RECONFIGURE_ONGOING",
   /* .s_id           =  */ (S8*)"ERAB_REL_RRC",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_erb_release_ue_reconfigure_ongoing_endc_mht
};

/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_endc_rrm_erb_release_resp_handler
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api_data
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_ERB_RELEASE_RESP from a RRM.
 ****************************************************************************/
void uecc_fsm_erb_release_endc_rrm_erb_release_resp_handler
(
    uecc_ue_context_t*   p_ue_context,
    void*                p_api_data
);

/****************************************************************************
 * Function Name  : uecc_fsm_ue_endc_erb_release_procedure_constructor
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : Constructor for UE E-RAB Release Procedure(eNB Initiated)
 ****************************************************************************/
void uecc_fsm_ue_endc_erb_release_procedure_constructor
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/****************************************************************************
 * Function Name  : uecc_fsm_ue_endc_erb_release_procedure_finish_success
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : Success handler for UE E-RAB Release procedure
 *                  (eNB Initiated) Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_endc_erb_release_procedure_finish_success
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/****************************************************************************
 * Function Name  : uecc_fsm_ue_endc_erb_release_procedure_finish_fail
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : Failure handler for UE E-RAB Release procedure
 *                  (ENB Initiated) Calls on procedure's finishing with failure
 ****************************************************************************/
void uecc_fsm_ue_endc_erb_release_procedure_finish_fail
(
    uecc_ue_context_t*    p_ue_context,
    void*                 p_api
);

#endif

/****************************************************************************
  Private Variables
 ****************************************************************************/
uecc_fsm_handler_record_t
uecc_fsm_ue_rrm_erb_release_procedure_mht[] =
{
    { { UECC_EV_UE_RELEASE_REQ            },
            uecc_fsm_rrm_erb_release_ue_release_req_handler },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND        },
            uecc_fsm_rrm_erb_release_rrm_ue_release_req_handler },
    { { UECC_EV_S1AP_RESET_MSG_IND              },
            uecc_fsm_rrm_erb_release_s1ap_reset_handler },
    { { UECC_EV_S1AP_ERAB_SETUP_REQ              },
            uecc_fsm_rrm_erb_release_erab_setup_req_discard_handler },
    { { UECC_EV_S1AP_ERAB_MODIFY_REQ            },
            uecc_fsm_rrm_erb_release_erab_modify_req_discard_handler },
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND      },
            uecc_fsm_rrm_erb_release_erab_rel_cmd_discard_handler },
    { { UECC_EV_RRM_ERAB_RELEASE_IND          },
            uecc_fsm_rrm_erb_release_erab_rel_ind_discard_handler },
    /*SPR_18241_START*/
    /* Code Removed */
    /*SPR_18241_END*/
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ          },
            uecc_fsm_rrm_erb_release_csc_delete_all_ue_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND            },
      uecc_fsm_rrm_erb_release_ongoing_radio_link_failure_handler },
    { { UECC_EV_RRM_HO_REQUIRED},
     uecc_fsm_rrm_erb_release_ongoing_ho_required_handler},
    { { UECC_LAST_ID                      }, PNULL    }
};

static uecc_fsm_handler_record_t
       uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                  },
       uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_init_handler },
    { { UECC_EV_LLIM_RECONFIGURE_UE_RESP  },
            uecc_fsm_rrm_erb_release_llim_reconfigure_ue_resp_handler    },
    { { UECC_EV_TIMER_EXPIRY                },
   uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_timer_expiry_handler },
    { { UECC_EV_RRC_RE_ESTABLISH_REQ               },
  uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_reestablish_req_handler},
    { { UECC_LAST_ID                        },  PNULL                   }
};

static uecc_fsm_handler_record_t
        uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
            uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing_init_handler   },
    { { UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE   },
    uecc_fsm_rrm_erb_release_rrc_connection_reconfiguration_complete_handler},
    { { UECC_EV_TIMER_EXPIRY                    },
      uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing_timer_expiry_handler },
   { { UECC_EV_RRC_RE_ESTABLISH_REQ            },
      uecc_fsm_rrm_erb_release_ongoing_re_establish_req_handler },
    { { UECC_LAST_ID                            },  PNULL                   }
};

/* Activity states */
static uecc_fsm_activity_state_t
       uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing =
{
   /* .s_name       =  */ (S8*)"UECC_LLIM_ERB_RELEASE_IND_RECONFIGURE_UE_ONGOING",
   /* .s_id         =  */ (S8*)"ERAB_REL_LLIM",
   /* .is_need_init =  */ RRC_TRUE,
 /* .p_mht       =  */ uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_mht
};

static uecc_fsm_activity_state_t 
       uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_UE_ERB_RELEASE_IND_RECONFIGURE_ONGOING",
   /* .s_id           =  */ (S8*)"ERAB_REL_RRC",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing_mht
};

/* Activity */
static uecc_fsm_activity_t uecc_fsm_rrm_erb_release_activity =
{
  /*  .s_name     = */(S8*)"RRC Connection Reconfiguration",
   /* .s_id       = */(S8*)"UE_RCRECONF_A",
/* .p_first_state = */&uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing
};

uecc_fsm_activity_t*
                    uecc_fsm_ue_rrm_erb_release_procedure_activity_list[] =
{
    &uecc_fsm_rrm_erb_release_activity,
    PNULL
};

/* Procedure */
uecc_fsm_procedure_t uecc_fsm_ue_rrm_erb_release_procedure =
{
    /*.s_name          = */(S8*)"RRM ERAB Release",
    /*.s_id            = */(S8*)"UE_RRM_ERABREL_P",
    /*.constructor_f   = */uecc_fsm_ue_rrm_erb_release_procedure_constructor,
  /*.finish_success_f = */uecc_fsm_ue_rrm_erb_release_procedure_finish_success,
    /*.finish_fail_f   = */uecc_fsm_ue_rrm_erb_release_procedure_finish_fail,
    /*.activity_list   = */uecc_fsm_ue_rrm_erb_release_procedure_activity_list,
    /*.p_mht           = */uecc_fsm_ue_rrm_erb_release_procedure_mht
};
/* x2ap_drop2_rw_end - remove static declaration */

#ifdef ENDC_ENABLED
static uecc_fsm_handler_record_t 
       uecc_fsm_erb_release_endc_rrm_resource_deletion_ongoing_mht[] =
{
    { { UECC_EV_RRM_ERB_RELEASE_RESP   },
            uecc_fsm_erb_release_endc_rrm_erb_release_resp_handler                },
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

/* Activity states */
static uecc_fsm_activity_state_t 
       uecc_fsm_erb_release_endc_rrm_resource_deletion_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_RRM_ERB_RELEASE_RESOURCE_DELETION_ONGOING",
   /* .s_id           =  */ (S8*)"ERAB_REL_RRM",
   /* .is_need_init   =  */ RRC_FALSE, /*bug fixed in code review*/
   /* .p_mht     =  */ uecc_fsm_erb_release_endc_rrm_resource_deletion_ongoing_mht
};

/* Activity */
static uecc_fsm_activity_t uecc_fsm_endc_erb_release_activity =
{
  /*  .s_name             = */(S8*)"RRC Connection Reconfiguration",
  /* .s_id               = */(S8*)"UE_RCRECONF_A",
  /* .p_first_state      = */&uecc_fsm_erb_release_endc_rrm_resource_deletion_ongoing
};

uecc_fsm_activity_t*
                    uecc_fsm_ue_endc_erb_release_procedure_activity_list[] =
{
    &uecc_fsm_endc_erb_release_activity,
    PNULL
};

/****************************************************************************
  Private Variables
 ****************************************************************************/
uecc_fsm_handler_record_t
uecc_fsm_ue_endc_erb_release_procedure_mht[] =
{
    { { UECC_EV_UE_RELEASE_REQ            },
            uecc_fsm_rrm_erb_release_ue_release_req_handler },
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND        },
            uecc_fsm_rrm_erb_release_rrm_ue_release_req_handler },
    { { UECC_EV_S1AP_RESET_MSG_IND              },
            uecc_fsm_rrm_erb_release_s1ap_reset_handler },
    { { UECC_EV_S1AP_ERAB_SETUP_REQ              },
            uecc_fsm_rrm_erb_release_erab_setup_req_discard_handler },
    { { UECC_EV_S1AP_ERAB_MODIFY_REQ            },
            uecc_fsm_rrm_erb_release_erab_modify_req_discard_handler },
    { { UECC_EV_S1AP_ERAB_RELEASE_COMMAND      },
            uecc_fsm_rrm_erb_release_erab_rel_cmd_discard_handler },
    { { UECC_EV_RRM_ERAB_RELEASE_IND          },
            uecc_fsm_rrm_erb_release_erab_rel_ind_discard_handler },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ          },
            uecc_fsm_rrm_erb_release_csc_delete_all_ue_req_handler },
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND            },
      uecc_fsm_rrm_erb_release_ongoing_radio_link_failure_handler },
    { { UECC_EV_RRM_HO_REQUIRED},
     uecc_fsm_rrm_erb_release_ongoing_ho_required_handler},
    { { UECC_LAST_ID                      }, PNULL    }
};

/* Procedure */
uecc_fsm_procedure_t uecc_fsm_ue_endc_erb_release_procedure =
{
    /*.s_name          = */(S8*)"ENDC ERAB Release",
    /*.s_id            = */(S8*)"UE_ERABREL_P",
    /*.constructor_f   = */uecc_fsm_ue_endc_erb_release_procedure_constructor,
    /*.finish_success_f = */uecc_fsm_ue_endc_erb_release_procedure_finish_success,
    /*.finish_fail_f   = */uecc_fsm_ue_endc_erb_release_procedure_finish_fail,
    /*.activity_list   = */uecc_fsm_ue_endc_erb_release_procedure_activity_list,
    /*.p_mht           = */uecc_fsm_ue_endc_erb_release_procedure_mht
};
#endif

/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/
/****************************************************************************
 * * Function Name  : process_delete_drb_resp_erab_rel
 * *
 * * Description    :
 * *     process delete DRBrequest in case of ERAB release
 * *
 * * Returns        : None
 * *
 * ****************************************************************************/
static void process_delete_drb_resp_erab_rel(
        uecc_ue_context_t*    p_ue_context,
        rrc_uecc_llim_delete_lc_resp_t*   p_rrc_uecc_llim_delete_lc_resp,
        U8 counter )
{
    U8 lc_counter = 0;
    U8 index = 0;
    U8 resp_counter = RRC_NULL;
    
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
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
                    if ( MAC_DELETE_LC_ACTIVE == p_rrc_uecc_llim_delete_lc_resp->
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

    /*SPR_19066_START*/
    p_ue_context->m.ue_inactive_time_updated = RRC_FALSE;
    /*SPR_19066_END*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_erb_release_procedure_constructor
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : Constructor for UE E-RAB Release Procedure(eNB Initiated)
 ****************************************************************************/
void uecc_fsm_ue_rrm_erb_release_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_return_t    result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Allocate memory for the curr_proc_data to be used during
     * E-RAB Management Procedures*/
     p_ue_context->p_curr_proc_data =
         (curr_proc_data_t*)rrc_mem_get(sizeof(curr_proc_data_t));

    if (PNULL == p_ue_context->p_curr_proc_data)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_BRIEF,
                "[CURR_PROC_DATA] Memory Alocation failed(E-RAB Release Ind)");
    }
    else
    {
        memset_wrapper(p_ue_context->p_curr_proc_data, 0,
                       sizeof(curr_proc_data_t));
        do {
               /* Start timer*/
               if (RRC_SUCCESS != 
                   uecc_ue_timer_start(
                   p_ue_context->p_gb_context,     
                   UECC_ERAB_RELEASE_IND_GUARD_TIMER,
                   &p_ue_context->p_gb_context->ue_timers_data,
                   &p_ue_context->timers_data,
                   p_ue_context->ue_index))
               {
                   break;
               }

               if (RRC_SUCCESS !=
               uecc_fsm_rrm_erb_release_process_rrm_e_rab_release_ind_msg
                   (p_ue_context, p_api))
               {
                   break;
               }

               RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
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
            p_ue_context->timers_data.timers_ids[UECC_ERAB_RELEASE_IND_GUARD_TIMER])
        {
            /* Stop timer */
            uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_IND_GUARD_TIMER, 
                &p_ue_context->timers_data);
        }

        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_SUCCESS to RRM */
        if ((PNULL == p_ue_context->p_curr_proc_data) || 
                (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
            p_ue_context, FAILURE)))
        {
            RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }

        /* Process failure*/
        uecc_fsm_finish_activity_failure(
          p_ue_context, (void*)UECC_EV_S1AP_ERAB_RELEASE_COMMAND_VALIDATION_FAILURE);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_erb_release_procedure_finish_success
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : Success handler for UE E-RAB Release procedure
 *                  (eNB Initiated) Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_rrm_erb_release_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    erab_list_t*   p_erab_list = PNULL;
    U8    free_erab_index = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


    /* Clear update configuration IEs flags */
    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.sps_config_updated = 0;
    p_ue_context->m.mac_config_updated = 0;
    p_ue_context->m.srb1_config_updated = 0;
    p_ue_context->m.srb2_config_updated = 0;

    /*SPR_17382_START*/
    p_ue_context->ho_info.is_x2_ho_erab_rel_reqd = RRC_FALSE;
    /*SPR_17382_END*/

    if (PNULL != p_ue_context->p_curr_proc_data)
    {
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
                rrc_mem_free(p_ue_context->p_e_rab_list
                        [p_erab_list->erab_item[free_erab_index].e_RAB_ID]);
                p_ue_context->p_e_rab_list
                    [p_erab_list->erab_item[free_erab_index].e_RAB_ID] = PNULL;

                /* Updates the ERAB Count in UE Context */
                p_ue_context->e_rab_list_count--;
            }
        }

        /* Deallocate memory previously allocated for 
         * RRM E-RAB Release Indication */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;

        /* Send event to UE Main FSM */
        uecc_event_queue_push_event(
                UECC_EV_RRM_ERAB_RELEASE_IND_SUCCESS_INT,
                p_ue_context,
                PNULL);
    }
    else
    {

        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_FATAL,"p_ue_context is NULL");
    }

#ifdef MEM_PROFILE_ON
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name, RRC_INFO,
    "\n *****Sizeof UE CONTEXT after RRM initiated ERAB RELEASE is successful %d ",
    sizeof(uecc_ue_context_t));

    qvPoolStats();
#endif

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_erb_release_procedure_finish_fail
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : Failure handler for UE E-RAB Release procedure
 *                  (ENB Initiated) Calls on procedure's finishing with failure
 ****************************************************************************/
void uecc_fsm_ue_rrm_erb_release_procedure_finish_fail(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
/*SPR 20636 Changes Start*/
    uecc_fsm_event_et     failure_ev = (uecc_fsm_event_et)((U32_64_Bit)p_api);
/*SPR 20636 Changes End*/

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /*SPR_17382_START*/
    p_ue_context->ho_info.is_x2_ho_erab_rel_reqd = RRC_FALSE;
    /*SPR_17382_END*/

    if ((UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT == failure_ev) ||
        (UECC_EV_S1AP_ERAB_RELEASE_COMMAND_VALIDATION_FAILURE == failure_ev))
    {
        /* Notify UE FSM about failure */
        /* Post event to UE FSM for future processing */
        uecc_event_queue_push_event(
            failure_ev,
            p_ue_context,
            p_api);
    }

    /*SPR_19066_START*/
    p_ue_context->m.ue_inactive_time_updated = RRC_FALSE;
    /*SPR_19066_END*/

    /* Deallocate memory previously allocated for E-RAB Release */
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_ue_release_req_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_UE_RELEASE_REQ
 ****************************************************************************/
void uecc_fsm_rrm_erb_release_ue_release_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_s1ap_ue_context_release_command_t *p_ue_context_release_command 
        = PNULL;
    U32    error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_UE_RELEASE_REQ");

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_ERAB_RELEASE_IND_GUARD_TIMER, &p_ue_context->timers_data);

    p_ue_context_release_command = (rrc_s1ap_ue_context_release_command_t*)
        p_api;

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
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
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
 * Function Name  : uecc_fsm_rrm_erb_release_rrm_ue_release_req_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND
 ****************************************************************************/
void uecc_fsm_rrm_erb_release_rrm_ue_release_req_handler(
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

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_RRM_UE_CONNECTION_RELEASE_IND");

    if (RRC_SUCCESS == uecc_s1ap_build_s1ap_cause(
        &cause, &p_ue_connection_release_ind->release_cause,
        p_ue_context))
    {
        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_IND_GUARD_TIMER,
            &p_ue_context->timers_data);

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
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_WARNING, 
            "[RRC_RRM_UE_CONNECTION_RELEASE_IND]"
            " invalid cause value [%u][%u]",
            p_ue_connection_release_ind->release_cause.type,
            p_ue_connection_release_ind->release_cause.value);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_timer_expiry_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 *
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_TIMER_EXPIRY
 *
 *
 ****************************************************************************/
/*void uecc_fsm_rrm_erb_release_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_UECC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_ue_context);

    p_api = p_api;

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_TIMER_EXPIRY");

    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);

    RRC_UECC_UT_TRACE_EXIT();
}*/
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_erab_setup_req_discard_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                      UECC_EV_S1AP_ERAB_SETUP_REQ
 *                      UECC_EV_S1AP_ERAB_MODIFY_REQ
 *                      UECC_EV_S1AP_ERAB_RELEASE_COMMAND
 *                      UECC_EV_RRM_ERAB_RELEASE_IND
******************************************************************************/
void uecc_fsm_rrm_erb_release_erab_setup_req_discard_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,
        "[UECC_EV_S1AP_ERAB_SETUP_REQ]" 
        "RRM Initiated E-RAB Release Procedure Ongoing"
        "Discarding E-RAB SetUp Req Received from MME");


    /* In this function No action is taken for simply discard
      the received message */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_discard_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                      UECC_EV_S1AP_ERAB_MODIFY_REQ
******************************************************************************/
void uecc_fsm_rrm_erb_release_erab_modify_req_discard_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* In this function No action is taken for simply discard 
      the received message */

     RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,
        "[UECC_EV_S1AP_ERAB_MODIFY_REQ]" 
        "RRM Initiated E-RAB Release Procedure Ongoing"
        "Discarding E-RAB Modify Req Received from MME");

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_discard_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                      UECC_EV_S1AP_ERAB_RELEASE_COMMAND
******************************************************************************/
void uecc_fsm_rrm_erb_release_erab_rel_cmd_discard_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* In this function No action is taken for simply discard 
      the received message */

     RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,
        "[UECC_EV_S1AP_ERAB_RELEASE_COMMAND]" 
        "RRM Initiated E-RAB Release Procedure Ongoing"
        "Discarding E-RAB Release Cmd Received from MME");
     
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_erab_rel_ind_discard_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                      UECC_EV_RRM_ERAB_RELEASE_IND
******************************************************************************/
void uecc_fsm_rrm_erb_release_erab_rel_ind_discard_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* In this function No action is taken for simply discard 
      the received message */

     RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,
        "[UECC_EV_RRM_ERAB_RELEASE_IND]" 
        "RRM Initiated E-RAB Release Procedure Ongoing"
        "Discarding E-RAB REL IND Received from RRM");

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR_18241_START*/
/* Code Removed */
/*SPR_18241_END*/

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_s1ap_reset_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_S1AP_RESET_MSG_IND
 ****************************************************************************/
void uecc_fsm_rrm_erb_release_s1ap_reset_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_S1AP_RESET_MSG_IND");

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_ERAB_RELEASE_IND_GUARD_TIMER, &p_ue_context->timers_data);

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
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
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
 * Function Name  : uecc_fsm_rrm_erb_release_csc_delete_all_ue_req_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_CSC_DELETE_ALL_UE_REQ
***************************************************************************/
void uecc_fsm_rrm_erb_release_csc_delete_all_ue_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_CSC_DELETE_ALL_UE_REQ");

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_ERAB_RELEASE_IND_GUARD_TIMER, &p_ue_context->timers_data);

    /* Process failure*/
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_CSC_CELL_DELETE_REQ_INT);

    uecc_event_queue_push_event(
        UECC_EV_CSC_CELL_DELETE_REQ_INT,
        p_ue_context,
        p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_process_rrm_e_rab_release_ind_msg 
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This is the function processes RRM_ERAB_RELEASE_IND
 *                  message:
 *                  - check message content;
 *                  - store IEs in UE context;
 ****************************************************************************/
rrc_return_t uecc_fsm_rrm_erb_release_process_rrm_e_rab_release_ind_msg(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    erab_item_t*   p_erab_item = PNULL;
    erab_list_t*   p_erab_list = PNULL;
    rrc_rrm_erb_release_ind_t*  p_msg = PNULL;
    U8           e_rab_index          = RRC_NULL;
    U8           wrong_cause = RRC_NULL;


    erab_list_t*   p_erab_failed_list       = PNULL;
    U8 indx  = 0;

    rrc_return_t   result = RRC_FAILURE;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_msg = (rrc_rrm_erb_release_ind_t*)p_api;
    
    do {
           /* points to erab_to_be_released_list in erab_release_command_data */
           p_erab_list = &p_ue_context->p_curr_proc_data->u.
               erab_release_command_data.erab_to_be_released_list;
           p_erab_list->counter = p_msg->erab_to_be_released_item_list.
               count;
          
           /* points to erab_failed_to_release_list */
           p_erab_failed_list = &p_ue_context->p_curr_proc_data->u.
               erab_release_command_data.erab_failed_to_release_list;


           for (e_rab_index = 0;
                e_rab_index < p_msg->erab_to_be_released_item_list.count;
                e_rab_index++)
           {
               p_erab_list->erab_item[e_rab_index].e_RAB_ID = 
                   p_msg->erab_to_be_released_item_list.
                   erab_to_be_release_item[e_rab_index].erab_id;  

               /* Convert received rrm cause into s1ap cause 
                * in received list from RRM */
               if (RRC_SUCCESS != uecc_s1ap_build_s1ap_cause(
                   &p_erab_list->erab_item[e_rab_index].cause,
                   &p_msg->erab_to_be_released_item_list.
                   erab_to_be_release_item[e_rab_index].cause,
                   p_ue_context))  
               {
                   RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                           p_ue_context->p_gb_context->facility_name,RRC_WARNING, 
                           "[RRC_RRM_ERB_RELEASE_IND]"
                           "invalid cause value [%u][%u]",
                           p_msg->erab_to_be_released_item_list.
                           erab_to_be_release_item[e_rab_index].cause.type,
                           p_msg->erab_to_be_released_item_list.
                           erab_to_be_release_item[e_rab_index].cause.value);
                   wrong_cause = 1;
                   break;
               }

           }
           if (wrong_cause == 1)
               break;

           /* points to e_rab_item of erab_to_be_released_list 
            * in erab_release_command_data */
           p_erab_item = p_ue_context->p_curr_proc_data->u.
               erab_release_command_data.erab_to_be_released_list.erab_item;

           /* points to erab_release_list */
           p_erab_list = &p_ue_context->p_curr_proc_data->u.
               erab_release_command_data.erab_release_list;

           /* Simply copy the structure to make in_sync with 
            * MME initiated E-RAB Release Procedure. 
            * Attention: Consider RRM will send correct E-RAB ID list to
            * be released so that checking for "Unknown E-RAB ID" and 
            * "Multiple Instance" is skipped */

           for (e_rab_index = 0;
                e_rab_index < p_msg->erab_to_be_released_item_list.count;
                e_rab_index++)
           {
               /* check for Unknown E-RAB ID instance*/
               if (PNULL == p_ue_context->
                   p_e_rab_list[(p_erab_item + e_rab_index)->e_RAB_ID])
               {
                   RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                           p_ue_context->p_gb_context->facility_name,
                           RRC_WARNING,"Unknown E-RAB ID = [%u]",
                           (p_erab_item + e_rab_index)->e_RAB_ID);
                   p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                       e_RAB_ID = (p_erab_item + e_rab_index)->e_RAB_ID;

                   p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                       error_code |= RRC_RRM_MODULE_ID << 16;

                   p_erab_failed_list->erab_item[p_erab_failed_list->counter].
                       error_code |= RRC_UNKNOWN_ERAB_ID; /* Yet to add reason */

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
                       {
                           RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                   p_ue_context->p_gb_context->facility_name,
                                   RRC_WARNING,"Multiple E-RAB ID = [%u]",
                                   (p_erab_item + e_rab_index)->e_RAB_ID);
                           break;
                       }
                   }
                   /* Store in erab_release_list */
                   if ((indx == p_erab_list->counter) && 
                           (MAX_ERAB_COUNT > p_erab_list->counter))
                   {
                       p_erab_list->erab_item[p_erab_list->counter].e_RAB_ID =
                           (p_erab_item + e_rab_index)->e_RAB_ID;
                       p_erab_list->erab_item[p_erab_list->counter++].cause =
                           (p_erab_item + e_rab_index)->cause;
                   }
               }
           }

           if (0 == p_erab_list->counter)
           {
               RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                       p_ue_context->p_gb_context->facility_name,
                       RRC_WARNING,"E-RAB Release List is empty"
                       " after validation. Indicates Failure");
               break;
           }

        /*SPR_19066_START*/
        if (RRM_ERB_RELEASE_IND_API_UE_INACTIVE_TIME_PRESENT
                & p_msg->bitmask)
        {
            p_ue_context->m.ue_inactive_time_updated = RRC_TRUE;
            p_ue_context->ue_inactive_time = 
                p_msg->ue_inactive_time_config;
        }
        /*SPR_19066_END*/
    
        result = RRC_SUCCESS;
    } while(0);
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);    
    return result;
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing_init_handler 
 *
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause       cause;
    U32              error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send RRCConnectionReconfiguration */
    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_rrc_connection_reconfiguration(p_ue_context))
    {
        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_ERAB_RELEASE_IND_GUARD_TIMER, &p_ue_context->timers_data);

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

        /*SPR_17382_START*/
        /*
         * L3 does not have to send ERAB Release CNF to RRM for ERABs
         * recevied in ERAB to be released list in Path Switch Ack.
         */
        if(RRC_FALSE == 
                p_ue_context->ho_info.is_x2_ho_erab_rel_reqd)
        {
            /* Build and send RRC_RRM_ERB_RELEASE_CNF with FAILURE to RRM */
            if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                        p_ue_context, FAILURE))
            {
                RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                        "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
            }
        }
        /*SPR_17382_END*/

        uecc_fsm_finish_activity_failure(
            p_ue_context,
            (void*)UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_rrc_connection_
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 *                   reconfiguration_complete_handler
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE
 *
 ****************************************************************************/
void uecc_fsm_rrm_erb_release_rrc_connection_reconfiguration_complete_handler(
    uecc_ue_context_t*   p_ue_context,
    void*                p_api
)
{
    erab_list_t*    p_failed_erab_list = PNULL;
    U16             resp_counter = RRC_NULL;
    /*SPR_17382_START*/
    erab_list_t*    p_erab_list = PNULL;
    U8              free_erab_index = RRC_NULL;
    /*SPR_17382_END*/
    rrc_return_t  rrc_response = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE");

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_ERAB_RELEASE_IND_GUARD_TIMER, &p_ue_context->timers_data);

    p_failed_erab_list = &p_ue_context->p_curr_proc_data->u.
        erab_release_command_data.erab_failed_to_release_list;
    resp_counter = p_ue_context->p_curr_proc_data->u.
        erab_release_command_data.counter_after_rrm_resp;

/*SPR_17382_START*/
    /*
     * L3 does not have to send ERAB Release CNF to RRM or 
     * ERAB Release Ind to MME for ERABs recevied in 
     * ERAB to be released list in Path Switch Ack.
     */
    if(RRC_FALSE == 
            p_ue_context->ho_info.is_x2_ho_erab_rel_reqd)
    {
        if (resp_counter == p_failed_erab_list->counter)
        {
            /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_SUCCESS to RRM */
            if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                        p_ue_context, SUCCESS))
            {
                RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                        "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
            }
        }
        else
        {
            /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_PARTAIL_SUCCESS to RRM */
            if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                        p_ue_context, PARTIAL_SUCCESS))
            {
                RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                        "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
            }
        }

        /* Build and send the RRC_S1AP_E_RAB_RELEASE_INDICATION msg to S1AP */
        if (RRC_SUCCESS !=
                (rrc_response =
                 uecc_s1ap_build_and_send_e_rab_release_indication(
                     p_ue_context)))
        {
            RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                    "[s1ap_ERABReleaseIndication] Delivery failure");
        }
    }
    else
    {
        /* Coverity_Fix 94885 +- */
            /* point to erab_release_list in erab_release_command_data */
            p_erab_list = &p_ue_context->p_curr_proc_data->u.
                erab_release_command_data.erab_release_list;

            /* 
             * Memory freed for ERAB Failed in 
             * Path Switch Req Ack from Ue Context 
             */
            /* Coverity_Fix 94889 ++ */
            while(free_erab_index < p_erab_list->counter)
                /* Coverity_Fix 94889 -- */
            {
                if ((PNULL != p_ue_context->p_e_rab_list
                            [p_erab_list->erab_item[free_erab_index].e_RAB_ID]) &&
                        (RRC_ERAB_FAILURE_PATH_SWITCH_REQ_ACK == p_ue_context->p_e_rab_list
                         [p_erab_list->erab_item[free_erab_index].e_RAB_ID]
                         ->uecc_x2ap_drb_ctx.status))
                {
                    rrc_mem_free(p_ue_context->p_e_rab_list
                            [p_erab_list->erab_item[free_erab_index].e_RAB_ID]);
                    p_ue_context->p_e_rab_list
                        [p_erab_list->erab_item[free_erab_index].e_RAB_ID] = PNULL;

                    /* Updates the ERAB Count in UE Context */
                    p_ue_context->e_rab_list_count--;

                    p_erab_list->erab_item[free_erab_index] = 
                        p_erab_list->erab_item[p_erab_list->counter - 1];

                    p_erab_list->counter--;
                    /* Coverity_Fix 94889 ++ */
                }
                else
                {
                    free_erab_index++;
                }
                /* Coverity_Fix 94889 -- */
            }
            /* Coverity_Fix 94885 +- */

        if(RRC_NULL != p_erab_list->counter)
        {
            /* Build and send the RRC_S1AP_E_RAB_RELEASE_INDICATION msg to S1AP */
            if (RRC_SUCCESS !=
                    (rrc_response =
                     uecc_s1ap_build_and_send_e_rab_release_indication(
                         p_ue_context)))
            {
                RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                        "[s1ap_ERABReleaseIndication] Delivery failure");
            }
        }
        else
            rrc_response = RRC_SUCCESS;
    }
/*SPR_17382_END*/

    if (RRC_SUCCESS == rrc_response)
    {
        /* Call success handler for procedure */
        uecc_fsm_finish_activity_success(p_ue_context, PNULL);
    }
    else
    {
        uecc_fsm_finish_activity_failure(
            p_ue_context,
            (void*)UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT);
    }
    uecc_llim_check_and_send_rrc_ue_information(p_ue_context);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_
 *                  init_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Send Delete LC request to LLIM */
    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_delete_lc_req(p_ue_context))
    {
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                "[Build_and_send_delete_lc_req] delivery failure");

        /* Stop timer */
        uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                UECC_ERAB_RELEASE_IND_GUARD_TIMER, &p_ue_context->timers_data);

        /* set cause due to build and send for ERAB Mgnt Procedures */
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_unspecified_2;

        /* set cause of each ERAB in erab_release_list */
        update_erab_release_list_with_cause(p_ue_context,
            &cause);

        error_code |= RRC_MODULE_ID << 16;
        error_code |= RRC_MEMORY_ALLOCATION_FAILURE; 

        /* set error code of each ERAB in erab release list */
        update_erab_release_list_with_error_code(p_ue_context,
            &error_code);

        /*SPR_17382_START*/
        /*
         * L3 does not have to send ERAB Release CNF to RRM for ERABs
         * recevied in ERAB to be released list in Path Switch Ack.
         */
        if(RRC_FALSE == 
                p_ue_context->ho_info.is_x2_ho_erab_rel_reqd)
        {
            /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
            if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                        p_ue_context, FAILURE))
            {
                RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                        "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
            }
        }
        /*SPR_17382_END*/
        /* SPR_21935 Fix - Start */ 
        /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
        if (RRC_SUCCESS !=
                uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
                    &cause))
        {
            RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                    "[s1ap_UeContextReleaseRequest] delivery failure");
        }
        /* SPR_21935 Fix - Stop */ 

        uecc_fsm_finish_activity_failure(
            p_ue_context,
            (void*)UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT);

    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_llim_reconfigure_ue_resp_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_RECONFIGURE_UE_RESP from LLIM
 ****************************************************************************/
void uecc_fsm_rrm_erb_release_llim_reconfigure_ue_resp_handler(
    uecc_ue_context_t*    p_ue_context,
    void*                 p_api
)
{
    rrc_uecc_llim_delete_lc_resp_t*   p_rrc_uecc_llim_delete_lc_resp = PNULL;
    s1ap_Cause      cause;
    U8 counter = 0;
    rrc_return_t  result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_LLIM_RECONFIGURE_UE_RESP");

    p_rrc_uecc_llim_delete_lc_resp = (rrc_uecc_llim_delete_lc_resp_t*)p_api;
    /* set value of cause */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_unspecified_2;

    do{
          /* Check whether UECC_EV_LLIM_RECONFIGURE_UE_RESP
           * received with success or failure or partial success */
          if (SUCCESS == p_rrc_uecc_llim_delete_lc_resp->
                   response_code)
          {
              RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
                  "[UECC_EV_LLIM_DELETE_LC_RESP] Response=%i",
                  p_rrc_uecc_llim_delete_lc_resp->response_code);

              /*CR 101 changes start*/
              for (counter = 0; counter < MAX_ERAB_COUNT; counter++)
              {
                  if (PNULL != p_ue_context->p_e_rab_list[counter])
                  {
                      process_delete_drb_resp_erab_rel(p_ue_context,
                            p_rrc_uecc_llim_delete_lc_resp,counter);
                  }
              } 
              /*CR 101 changes stop*/

              /* Switch  state */
              UECC_FSM_SET_CHILD_STATE(p_ue_context,
                  &uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing, PNULL);

              result = RRC_SUCCESS;
          }
          else if (PARTIAL_SUCCESS ==
                   p_rrc_uecc_llim_delete_lc_resp->response_code)
          {
              RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
                  "[UECC_EV_LLIM_DELETE_LC_RESP] Response=%i",
                  p_rrc_uecc_llim_delete_lc_resp->response_code);

              /*update curr proc data with
               * rrc_uecc_llim_delete_lc_resp */
              update_curr_proc_data_with_response_code(p_ue_context,
                  p_rrc_uecc_llim_delete_lc_resp);

              /* Switch  state */
              UECC_FSM_SET_CHILD_STATE(p_ue_context,
                  &uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing, PNULL);
              result = RRC_SUCCESS;
          }
          else if (FAILURE ==
                   p_rrc_uecc_llim_delete_lc_resp->response_code)
          {
              RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
                  "[UECC_EV_LLIM_DELETE_LC_RESP] Response=%i",
                  p_rrc_uecc_llim_delete_lc_resp->response_code);

              /* Stop timer */
              uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_IND_GUARD_TIMER,
                  &p_ue_context->timers_data);

              /*update curr proc data with
               * rrc_uecc_llim_delete_lc_resp */
              update_curr_proc_data_with_response_code(p_ue_context,
                  p_rrc_uecc_llim_delete_lc_resp);

              /*SPR_17382_START*/
              /*
               * L3 does not have to send ERAB Release CNF to RRM for ERABs
               * recevied in ERAB to be released list in Path Switch Ack.
               */
              if(RRC_FALSE == 
                      p_ue_context->ho_info.is_x2_ho_erab_rel_reqd)
              {
                  /* Build and send RRC_RRM_ERB_RELEASE_CNF with FAILURE to RRM */
                  if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                              p_ue_context, FAILURE))
                  {
                      RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                              "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
                  }
              }
              /*SPR_17382_END*/

             /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
             if (RRC_SUCCESS !=
                     uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
                         &cause))
             {
                 RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                         "[s1ap_UeContextReleaseRequest] delivery failure");
             }
          }
          else if (L3_FATAL ==
                   p_rrc_uecc_llim_delete_lc_resp->response_code)
          {

              RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                  "[UECC_EV_LLIM_DELETE_LC_RESP]: Response code=[%i]",
                  p_rrc_uecc_llim_delete_lc_resp->response_code);

              /* Stop timer */
              uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_IND_GUARD_TIMER,
                  &p_ue_context->timers_data);

              /*update curr proc data with
               * rrc_uecc_llim_delete_lc_resp */
              update_curr_proc_data_with_response_code(p_ue_context,
                  p_rrc_uecc_llim_delete_lc_resp);

              /*SPR_17382_START*/
              /*
               * L3 does not have to send ERAB Release CNF to RRM for ERABs
               * recevied in ERAB to be released list in Path Switch Ack.
               */
              if(RRC_FALSE == 
                      p_ue_context->ho_info.is_x2_ho_erab_rel_reqd)
              {
                  /* Build and send RRC_RRM_ERB_RELEASE_CNF with FAILURE to RRM */
                  if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                              p_ue_context, FAILURE))
                  {
                      RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                              "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
                  }
              }
              /*SPR_17382_END*/

             /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
             if (RRC_SUCCESS !=
                     uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
                         &cause))
             {
                 RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                         "[s1ap_UeContextReleaseRequest] delivery failure");
             }
          }
    } while(0);

    /* Process failure */
    if (RRC_SUCCESS != result)
    {
        uecc_fsm_finish_activity_failure(
            p_ue_context,
            (void*)UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  :uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_reestablish_req_handler 
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ over CCCH. 
 *
 ****************************************************************************/
void 
uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_reestablish_req_handler(
    uecc_ue_context_t*    p_ue_context,
    void*                 p_api
)
{
    s1ap_Cause  cause;
    U32  error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_RRC_RE_ESTABLISH_REQ;Unused variable p_api=%p",p_api);/*SPR 17777 +-*/

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, UECC_ERAB_RELEASE_IND_GUARD_TIMER,
            &p_ue_context->timers_data);

    /* set value of cause */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_interaction_with_other_procedure; 

    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_REESTABLISHMENT_TRIGGERED; 

    /* update error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
        &error_code);

    /*SPR_17382_START*/
    /*
     * L3 does not have to send ERAB Release CNF to RRM for ERABs
     * recevied in ERAB to be released list in Path Switch Ack.
     */
    if(RRC_FALSE == 
            p_ue_context->ho_info.is_x2_ho_erab_rel_reqd)
    {
        /* Build and send RRC_RRM_ERB_RELEASE_CNF with FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                    p_ue_context, FAILURE))
        {
            RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }
    }
    /*SPR_17382_END*/

    /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
        &cause))
    {
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
        "[s1ap_UeContextReleaseRequest] delivery failure");
    }
    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
        p_ue_context->p_gb_context, p_ue_context->crnti,
       p_ue_context->reestablishmentCause,
        p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
    {
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
        "Fail to send the RRCConnectionReestablishmentReject to UE");
    }

    /* Set This Flag to RRC_TRUE, so that ue connection release notification
     * not sent to UE (DCCH no more exist) */
    p_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = RRC_TRUE;

    uecc_fsm_finish_activity_failure(
        p_ue_context,
        (void*)UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing_timer_expiry_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    :This is the handler function for the message
 *                 UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rrm_erb_release_ue_reconfigure_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "Ue Reconfiguraion Ongoing: UECC_EV_TIMER_EXPIRY");

    /* set cause at timer expiry for ERAB Mgnt Procedures */
    cause.t = T_s1ap_Cause_radioNetwork;
    /* SPR 15473 Fix Start */
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;
    /* SPR 15473 Fix Stop */

    /* set cause of each ERAB in erab_release_list */
    update_erab_release_list_with_cause(p_ue_context,
        &cause);

    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_TIMER_EXPIRY;

    /* set error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
        &error_code);

    /*SPR_17382_START*/
    /*
     * L3 does not have to send ERAB Release CNF to RRM for ERABs
     * recevied in ERAB to be released list in Path Switch Ack.
     */
    if(RRC_FALSE == 
            p_ue_context->ho_info.is_x2_ho_erab_rel_reqd)
    {
        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                    p_ue_context, FAILURE))
        {
            RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }
    }
    /*SPR_17382_END*/

    /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
        &cause))
    {
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
            "[s1ap_UeContextReleaseRequest] delivery failure");
    }

    uecc_fsm_finish_activity_failure(
        p_ue_context,
        (void*)UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing
 *                  _timer_expiry_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rrm_erb_release_llim_reconfigure_ue_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "LLIM Reconfigure Ue Ongoing: UECC_EV_TIMER_EXPIRY");

    /* set cause at timer expiry for ERAB Mgnt Procedures */
    cause.t = T_s1ap_Cause_radioNetwork;
    /* SPR 15473 Fix Start */
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;
    /* SPR 15473 Fix Stop */

    /* set cause of each ERAB in erab_release_list */
    update_erab_release_list_with_cause(p_ue_context,
        &cause);

    error_code |= RRC_MODULE_ID << 16;
    error_code |= RRC_TIMER_EXPIRY;

    /* set error code of each ERAB in erab_release_list */
    update_erab_release_list_with_error_code(p_ue_context,
        &error_code);

    /*SPR_17382_START*/
    /*
     * L3 does not have to send ERAB Release CNF to RRM for ERABs
     * recevied in ERAB to be released list in Path Switch Ack.
     */
    if(RRC_FALSE == 
            p_ue_context->ho_info.is_x2_ho_erab_rel_reqd)
    {
        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                    p_ue_context, FAILURE))
        {
            RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }
    }
    /*SPR_17382_END*/

    /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
        &cause))
    {
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
            "[s1ap_UeContextReleaseRequest] delivery failure");
    }

    uecc_fsm_finish_activity_failure(
        p_ue_context,
        (void*)UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_ongoing_radio_link_failure_handler
 *
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RADIO_LINK_FAILURE_IND
 ****************************************************************************/
void uecc_fsm_rrm_erb_release_ongoing_radio_link_failure_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause      cause;
    U32             error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_RADIO_LINK_FAILURE_IND");

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_ERAB_RELEASE_IND_GUARD_TIMER, &p_ue_context->timers_data);

    /* set value of cause due to RLF during ERAB Mgnt Procedures */
    cause.t = T_s1ap_Cause_radioNetwork;
    cause.u.radioNetwork = s1ap_failure_in_radio_interface_procedure;

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

    /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
        p_ue_context, FAILURE))
    {
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
            "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
    }

    /* Process failure handler for procedure */
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_RADIO_LINK_FAILURE_INT);

    uecc_event_queue_push_event(
        UECC_EV_RADIO_LINK_FAILURE_INT,
        p_ue_context,
        p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_ongoing_re_establish_req_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRC_RE_ESTABLISH_REQ
 ****************************************************************************/
void uecc_fsm_rrm_erb_release_ongoing_re_establish_req_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause    cause;
    U32           error_code = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
        "UECC_EV_RRC_RE_ESTABLISH_REQ");

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_ERAB_RELEASE_IND_GUARD_TIMER, &p_ue_context->timers_data);

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

    /*SPR_17382_START*/
    /*
     * L3 does not have to send ERAB Release CNF to RRM for ERABs
     * recevied in ERAB to be released list in Path Switch Ack.
     */
    if(RRC_FALSE == 
            p_ue_context->ho_info.is_x2_ho_erab_rel_reqd)
    {
        /* Build and send RRC_RRM_ERB_RELEASE_CNF with RRC_FAILURE to RRM */
        if (RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_conf(
                    p_ue_context, FAILURE))
        {
            RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_ERB_RELEASE_CNF] delivery failure");
        }
    }
    /*SPR_17382_END*/
    /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
        &cause))
    {
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
            "[s1ap_UeContextReleaseRequest] delivery failure");
    }

    if (RRC_SUCCESS !=
            uecc_llim_build_and_send_rrc_connection_reestablishment_reject(
            p_ue_context->p_gb_context, p_ue_context->crnti,
            p_ue_context->reestablishmentCause,
            p_ue_context->cell_index, RRC_UECCMD_MODULE_ID))
    {
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
            "Fail to send the RRCConnectionReestablishmentReject to UE");
    }

    /* Set This Flag to RRC_TRUE, so that ue connection release notification
     * not sent to UE (DCCH no more exist) */
    p_ue_context->share_data.uecc_rcr_data.
        rrc_connection_release_not_required_flag = RRC_TRUE;

    /* Call failure handler for procedure */
    uecc_fsm_finish_activity_failure(
        p_ue_context,
        (void*)UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : update_erab_release_list_with_cause
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_cause
 * Output         : None
 * Returns        : None
 * Description    : This function update erab_release_list in curr
 *                  proc data with received casue (p_cause)
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

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO, "UPDATE ERAB_RELEASE_LIST:"
            "WITH S1AP CASUSE");

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
* Function Name  : update_curr_proc_data_with_response_code
* Input          : uecc_ue_context_t*  p_ue_context
*                  void*               p_response_code
* Output         : None
* Returns        : None
* Description    : This function update curr proc data with received
*                  rrc_uecc_llim_delete_lc_resp from LLIM.
****************************************************************************/
void update_curr_proc_data_with_response_code(
    uecc_ue_context_t* p_ue_context,
        void*              p_response_code)
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

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO, "[CURR PROC DATA UPDATED]:"
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
        for (erab_counter = 0; erab_counter < MAX_ERAB_COUNT;
                erab_counter++)
        {
            if ((PNULL != p_ue_context->p_e_rab_list[erab_counter]) &&
                     (MAX_ERAB_COUNT > p_erab_list->counter))
            {

                if(p_ue_context->p_e_rab_list[erab_counter]->
                        drb_config.logical_channel_identity == 
                        p_rrc_uecc_llim_delete_lc_resp->
                        delete_drb_entity_resp[resp_counter].lc_id
                        && p_erab_list->counter < MAX_ERAB_COUNT)
                {
                    if(PARTIAL_SUCCESS ==
                            p_rrc_uecc_llim_delete_lc_resp->response_code)
                    {
                        if (SUCCESS ==
                                p_rrc_uecc_llim_delete_lc_resp->
                                delete_drb_entity_resp[resp_counter].
                                response)
                        {
                            p_erab_list->erab_item[p_erab_list->counter].
                                e_RAB_ID
                                =
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
                            p_erab_list->counter++;
                            break;
                        }
                        else
                        {
                            p_erab_failed_list->
                                erab_item[p_erab_failed_list->counter].
                                e_RAB_ID
                                =
                                p_ue_context->p_e_rab_list[erab_counter]->
                                e_RAB_ID;

                            p_erab_failed_list->
                                erab_item[p_erab_failed_list->counter].cause
                                =
                                cause;
                            p_erab_failed_list->
                                erab_item[p_erab_failed_list->counter].
                                error_code
                                =
                                p_rrc_uecc_llim_delete_lc_resp->
                                delete_drb_entity_resp[resp_counter].response;
                            p_erab_failed_list->counter++;
                            break;
                        }
                    }
                    else /* If response code is either
                          * L3_FATAL or FAILURE */
                    {

                        p_erab_list->erab_item[p_erab_list->counter].
                            e_RAB_ID
                            =
                            p_ue_context->p_e_rab_list[erab_counter]->
                            e_RAB_ID;

                        p_erab_list->erab_item[p_erab_list->counter].cause
                            =
                            cause;

                        p_erab_list->erab_item[p_erab_list->counter].
                            error_code
                            =
                            p_rrc_uecc_llim_delete_lc_resp->
                            delete_drb_entity_resp[resp_counter].response;

                        p_erab_list->counter++;
                        break;
                    }
                }
            }
        }
        if (erab_counter == MAX_ERAB_COUNT)
        {
            RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_WARNING,
                    "[UECC_EV_LLIM_DELETE_LC_RESP]:Invalid LC Id [%i]",
                    p_rrc_uecc_llim_delete_lc_resp->
                    delete_drb_entity_resp[resp_counter].lc_id);
        }

    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_rrm_erb_release_ongoing_ho_required_handler
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                      UECC_EV_RRM_HO_REQUIRED
 ******************************************************************************/
void uecc_fsm_rrm_erb_release_ongoing_ho_required_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* In this function HO failure indication is sent to RRM */

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_INFO,
            "[UECC_EV_RRM_HO_REQUIRED]" 
            "RRM Initiated E-RAB Release Procedure Ongoing"
            "Cancel the HO triggered by RRM");

    p_ue_context->ho_info.cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->ho_info.cause.u.radioNetwork = s1ap_interaction_with_other_procedure;
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ho_failure( p_ue_context))
    {
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
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
/****************************************************************************
 * Function Name  : uecc_fsm_ue_endc_erb_release_procedure_finish_success
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : Success handler for UE E-RAB Release procedure
 *                  (eNB Initiated) Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_endc_erb_release_procedure_finish_success
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    U8             free_erab_index = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*
     * Clear update configuration IEs flags
     */
    p_ue_context->m.physical_config_dedicated_updated = RRC_NULL;
    p_ue_context->m.sps_config_updated = RRC_NULL;
    p_ue_context->m.mac_config_updated = RRC_NULL;
    p_ue_context->m.srb1_config_updated = RRC_NULL;
    p_ue_context->m.srb2_config_updated = RRC_NULL;

    p_ue_context->ho_info.is_x2_ho_erab_rel_reqd = RRC_FALSE;

    /*
     * Build and send the RRC_S1AP_E_RAB_RELEASE_INDICATION msg to S1AP
     */
    if ( RRC_SUCCESS != uecc_s1ap_build_and_send_e_rab_release_indication_endc(
                p_ue_context) )
    {
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                "[s1ap_E_RABSetup] Delivery failure E_RAB Release Indication");
    }
    if ( PNULL != p_ue_context->p_curr_proc_data )
    {
        /*
         * Memory freed for Deleted E-RAB's from Ue Context
         */
        for (free_erab_index = RRC_NULL;
                free_erab_index < MAX_ERAB_COUNT;
                free_erab_index++)
        {
            if (PNULL != 
                    p_ue_context->p_e_rab_list
                    [free_erab_index])
            {
                if ( ( p_ue_context->p_e_rab_list[free_erab_index]->m.DC_BearerTypePresent ) &&
                        ( SN_TERMINATED_SPLIT_BEARER == p_ue_context->
                          p_e_rab_list[free_erab_index]->dc_bearer_type )) 
                {
                    rrc_mem_free(p_ue_context->p_e_rab_list[free_erab_index]);
                    p_ue_context->p_e_rab_list[free_erab_index] = PNULL;

                    /*
                     * Updates the ERAB Count in UE Context
                     */
                    p_ue_context->e_rab_list_count--;
                }
                if ( ( p_ue_context->p_e_rab_list[free_erab_index]->m.DC_BearerTypePresent ) &&
                        ( SN_TERMINATED_SCG_BEARER == p_ue_context->
                          p_e_rab_list[free_erab_index]->dc_bearer_type )) 
                {
                    p_ue_context->ue_status.ue_num_of_drb--;
                }

            }
        }

        /*
         * Deallocate memory previously allocated for 
         * RRM E-RAB Release
         */
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;

        /*
         * Send event to UE Main FSM
         */
        uecc_event_queue_push_event(
                UECC_EV_RRM_ERAB_RELEASE_IND_SUCCESS_INT,
                p_ue_context,
                PNULL);
    }
    else
    {

        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_FATAL,"p_ue_context is NULL");
    }

#ifdef MEM_PROFILE_ON
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name, RRC_INFO,
            "\n *****Sizeof UE CONTEXT after RRM initiated ERAB RELEASE is successful %d ",
            sizeof(uecc_ue_context_t));

    qvPoolStats();
#endif

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_endc_erb_release_procedure_finish_fail
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : Failure handler for UE E-RAB Release procedure
 *                  (ENB Initiated) Calls on procedure's finishing with failure
 ****************************************************************************/
void uecc_fsm_ue_endc_erb_release_procedure_finish_fail
(
    uecc_ue_context_t*    p_ue_context,
    void*                 p_api
)
{
    uecc_fsm_event_et     failure_ev = (uecc_fsm_event_et)((U32_64_Bit)p_api);

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_ue_context->ho_info.is_x2_ho_erab_rel_reqd = RRC_FALSE;

    if ( UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT == failure_ev )
    {
        /*
         * Notify UE FSM about failure
         */
        /*
         * Post event to UE FSM for future processing
         */
        uecc_event_queue_push_event(
            failure_ev,
            p_ue_context,
            p_api);
    }

    p_ue_context->m.ue_inactive_time_updated = RRC_FALSE;

    /*
     * Deallocate memory previously allocated for E-RAB Release
     */
    if (PNULL != p_ue_context->p_curr_proc_data)
    {
        rrc_mem_free(p_ue_context->p_curr_proc_data);
        p_ue_context->p_curr_proc_data = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_endc_erb_release_procedure_constructor
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  void*               p_api
 * Output         : None
 * Returns        : None
 * Description    : Constructor for UE E-RAB Release Procedure(eNB Initiated)
 ****************************************************************************/
void uecc_fsm_ue_endc_erb_release_procedure_constructor
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    s1ap_Cause                        cause;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Allocate memory for the curr_proc_data to be used during
     * E-RAB Management Procedures*/
     p_ue_context->p_curr_proc_data =
         (curr_proc_data_t*)rrc_mem_get(sizeof(curr_proc_data_t));

    if (PNULL == p_ue_context->p_curr_proc_data)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_BRIEF,
                "[CURR_PROC_DATA] Memory Alocation failed(E-RAB Release)");
    }
    else
    {
        memset_wrapper(p_ue_context->p_curr_proc_data, 0,
                sizeof(curr_proc_data_t));
        do {
            /*
             * Start timer
             */
            if ( RRC_SUCCESS != 
                    uecc_ue_timer_start(
                        p_ue_context->p_gb_context,     
                        UECC_ERAB_RELEASE_ENDC_GUARD_TIMER,
                        &p_ue_context->p_gb_context->ue_timers_data,
                        &p_ue_context->timers_data,
                        p_ue_context->ue_index) )
            {
                break;
            }

            /*
             * Build and sent the RRC_RRM_ERB_RELEASE_REQ to RRM
             */
            if ( RRC_SUCCESS != uecc_rrm_build_and_send_erb_release_req(p_ue_context) )
            {
                RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                        "Build and Send ERB Release Req: Failed");

                /*
                 * Stop timer
                 */
                if (PNULL != 
                        p_ue_context->timers_data.timers_ids[UECC_ERAB_RELEASE_ENDC_GUARD_TIMER])
                {
                    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                            UECC_ERAB_RELEASE_ENDC_GUARD_TIMER, &p_ue_context->timers_data);
                }

                /* set value of cause due to RLF during ERAB Mgnt Procedures */
                cause.t = T_s1ap_Cause_radioNetwork;
                cause.u.radioNetwork = s1ap_release_due_to_eutran_generated_reason;

                /*
                 * Build and send the UE_CONTEXT_RELEASE_REQ msg to S1AP
                 */
                if (RRC_SUCCESS !=
                        uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
                            &cause))
                {
                    RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                            "[s1ap_UeContextReleaseRequest] delivery failure");
                }

                /*
                 * Process failure
                 */
                uecc_fsm_finish_activity_failure(
                        p_ue_context, (void*)UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT);
            }

        } while(RRC_NULL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : rrc_uecc_check_sgnb_erab_list_to_release_for_split
 * Input          : uecc_ue_context_t*  p_ue_context
 *                  rrc_rrm_erb_release_resp_t* p_rrc_rrm_erb_release_resp
 * Output         : None
 * Returns        : result - TRUE/FALSE
 * Description    : This function checks if any split bearer is present or not
 ****************************************************************************/
rrc_bool_et
rrc_uecc_check_sgnb_erab_list_to_release_for_split
(
    uecc_ue_context_t*          p_ue_context,
    rrc_rrm_erb_release_resp_t* p_rrc_rrm_erb_release_resp
)
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
                ( SN_TERMINATED_SPLIT_BEARER ==
                     p_ue_context->p_e_rab_list[erab_release_id]->dc_bearer_type ))
            {
                result = RRC_TRUE;
                break;
            }
            else
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                        p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                        "[X2AP SgNB Release Req] ERAB ID[%d] is not SN_TERMINATED_SPLIT_BEARER",
                        erab_release_id);

            }
        }
    }

    return result;
}
/****************************************************************************
 * Function Name  : uecc_fsm_erb_release_endc_rrm_erb_release_resp_handler
 * Input          : uecc_ue_context_t*  p_ue_context,
 *                  void*               p_api_data
 * Output         : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_ERB_RELEASE_RESP from a RRM.
 ****************************************************************************/
void uecc_fsm_erb_release_endc_rrm_erb_release_resp_handler
(
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
            if((RRC_TRUE == rrc_uecc_check_sgnb_erab_list_to_release_for_split(p_ue_context,
                               p_rrc_rrm_erb_release_resp)) &&
                       (RRC_NULL < rrc_uecc_count_scg_bearer(p_ue_context)))
            {
                UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_endc, 
                        PNULL);
            }
            else
            {
                /*
                 * Switch state
                 */
                UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_erb_release_ue_reconfigure_ongoing_endc, PNULL);
            }

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

           if (RRM_RRM_ERB_RELEASE_RESP_UE_INACTIVE_TIME_PRESENT
                   & p_rrc_rrm_erb_release_resp->bitmask)
           {
               p_ue_context->m.ue_inactive_time_updated = RRC_TRUE;
               p_ue_context->ue_inactive_time = 
                   p_rrc_rrm_erb_release_resp->ue_inactive_time_config;
           }

           result = RRC_SUCCESS;
           break;

        case FAILURE:
           /* RRM Fail to release ERAB list should be present */

           /* Stop timer */
           uecc_ue_timer_stop(p_ue_context->p_gb_context, 
                   UECC_ERAB_RELEASE_ENDC_GUARD_TIMER, 
               &p_ue_context->timers_data);
       
           result = uecc_fsm_erb_release_process_rrm_erb_release_resp_data(
                        p_ue_context,
                        p_rrc_rrm_erb_release_resp);

          if (result == RRC_SUCCESS)
          {
             /* coverity_277530_fix_start */
             /* set cause due to build and send for ERAB Mgnt Procedures */
              cause.t = T_s1ap_Cause_radioNetwork;
              cause.u.radioNetwork = s1ap_unspecified_2; 
             /* coverity_277530_fix_end */
              /* Build and send the UE_CONTEXT_RELEASE_REQ  msg to S1AP */
              if (RRC_SUCCESS !=
                  uecc_s1ap_build_and_send_enb_ue_ctx_release_req(p_ue_context,
                  &cause))
              {
                  RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_FATAL,
                      "[s1ap_UeContextReleaseRequest] delivery failure");
              }
          }
            break;

        case PARTIAL_SUCCESS:
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

           /* RRM Release and Fail to release ERAB list should be present */
           result = uecc_fsm_erb_release_process_rrm_erb_release_resp_data(
                        p_ue_context,
                        p_rrc_rrm_erb_release_resp);

           if (RRC_SUCCESS == result)
           {
               if (RRM_RRM_ERB_RELEASE_RESP_UE_INACTIVE_TIME_PRESENT
                       & p_rrc_rrm_erb_release_resp->bitmask)
               {
                   p_ue_context->m.ue_inactive_time_updated = RRC_TRUE;
                   p_ue_context->ue_inactive_time = 
                       p_rrc_rrm_erb_release_resp->ue_inactive_time_config;
               }

               if((RRC_TRUE == rrc_uecc_check_sgnb_erab_list_to_release_for_split(p_ue_context,
                               p_rrc_rrm_erb_release_resp)) &&
                       (RRC_NULL < rrc_uecc_count_scg_bearer(p_ue_context)))
               {
                   UECC_FSM_SET_CHILD_STATE(p_ue_context,
                           &uecc_fsm_erb_release_llim_reconfigure_ue_ongoing_endc, 
                           PNULL);
               }
               else
               {
                   /*
                    * Switch state
                    */
                   UECC_FSM_SET_CHILD_STATE(p_ue_context,
                           &uecc_fsm_erb_release_ue_reconfigure_ongoing_endc, PNULL);
               }
               RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
               return;
           }
            break;

        default:
            RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_ERB_RELEASE_RESP] Invalid Response=%i",
                p_rrc_rrm_erb_release_resp->response);
            break;
    }

    /* If response from RRC_FAILURE */
    if (RRC_SUCCESS == result && SUCCESS != 
        p_rrc_rrm_erb_release_resp->response)
    {
        /* Process failure*/
        uecc_fsm_finish_activity_failure(
           p_ue_context, (void*)UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT);

    }

    /* If response code is RRC_FAILURE or RRC_PARTIAL_SUCCCESS */
    if (RRC_SUCCESS != result && SUCCESS != 
        p_rrc_rrm_erb_release_resp->response)
    {
        RRC_UECC_RRM_ERB_RELEASE_FSM_TRACE(RRC_ERROR,
            "[RRC_RRM_ERB_RELEASE_RESP] Mandatory List Absent, Response=%i",
             p_rrc_rrm_erb_release_resp->response);
        /* Process failure*/

        uecc_fsm_finish_activity_failure(
           p_ue_context, (void*)UECC_EV_RRM_ERAB_RELEASE_IND_FAILURE_INT);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
#endif
