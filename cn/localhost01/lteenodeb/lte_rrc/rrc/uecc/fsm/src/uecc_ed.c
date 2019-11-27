/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_ed.c,v 1.20 2010/10/13 07:03:25 gur18569 Exp $
 ****************************************************************************
 * DESCRIPTION :This file have entry point function to uecc module and
 *              calling handler function of various fsm's
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_ed.c,v $
 * Revision 1.20  2010/10/13 07:03:25  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.6.4.14  2010/10/07 06:47:33  gur04518
 * corrected the boundry check over modules_index
 *
 * Revision 1.6.4.13  2010/10/04 13:53:58  gur04518
 * Removed TABS
 *
 * Revision 1.6.4.12  2010/09/23 06:45:11  gur25381
 * Reverting Back the Klocwork Changes
 *
 * Revision 1.6.4.11  2010/09/22 14:47:37  gur25381
 * klocwork warning solved after merging
 *
 * Revision 1.6.4.10  2010/09/13 13:03:52  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.6.4.9  2010/09/07 15:02:29  gur16768
 * UECC_UE_LL_GUARD_TIMER timer handling was missing
 *
 * Revision 1.6.4.8  2010/08/30 12:22:10  gur16768
 * modified for src ho
 *
 * Revision 1.6.4.7  2010/08/20 14:57:30  gur18569
 * code fix for trg ho
 *
 * Revision 1.6.4.6  2010/08/05 08:07:13  gur15697
 * merging round 2
 *
 * Revision 1.6.4.5  2010/07/21 08:34:41  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.6.4.4  2010/07/16 05:18:28  gur22047
 * review comments incorporation
 *
 * Revision 1.6.4.3  2010/07/15 06:24:28  gur22047
 * code review comments incorporate
 *
 * Revision 1.6.4.2  2010/06/29 10:03:02  gur22047
 * RRM interface API from RRM to UECC handling
 *
 * Revision 1.6.4.1  2010/06/24 12:49:34  gur22047
 * internal source and s1ap code
 *
 * Revision 1.6  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.5  2010/02/23 14:11:14  gur22133
 * after_lokesh_ue_cap_code_review_23feb
 *
 * Revision 1.4  2010/02/18 04:19:02  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.3  2010/01/11 11:46:43  gur10248
 * *** empty log message ***
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.31  2009/12/07 15:28:44  ukr16032
 * UECC - inspection log items LG_1, LG_2, LG_3, LG_5, LG_8, LG_9, LG_10, LG_12, LG_13, LG_15, LG_16, LG_17 fixed
 *
 * Revision 1.1.2.30  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.28.2.18  2009/11/24 12:12:50  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.2.28.2.17  2009/11/24 11:32:35  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.2.28.2.16.2.1  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.2.28.2.16  2009/11/19 09:52:18  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.2.28.2.15  2009/11/16 18:32:53  ukr16032
 * UECC ISA activity implementation started
 *
 * Revision 1.1.2.28.2.14  2009/11/16 16:39:48  ukr18880
 * release from RRM handling
 *
 * Revision 1.1.2.28.2.13  2009/11/16 14:45:02  ukr18877
 * S1-AP moved to internal interfaces
 *
 * Revision 1.1.2.28.2.12  2009/11/16 13:37:03  ukr16018
 * S1AP is internal module now 1.
 *
 * Revision 1.1.2.28.2.11  2009/11/15 10:02:59  ukr15916
 * beautification
 *
 * Revision 1.1.2.28.2.10  2009/11/12 01:38:08  ukr15916
 * initial_context_setup_request and initial_context_setup_response updated
 *  for ASN S1AP
 *
 * Revision 1.1.2.28.2.9  2009/11/11 17:47:41  ukr16032
 * UECC
 *  -- S1AP UL NAS message generation updated
 *  -- FSM - review
 *  -- FSM - reset and rrm_release handling added to wait_for_s1_connection and
 * ue_connected states
 *  -- S1AP_RESET_MSG_IND message added
 *
 * Revision 1.1.2.28.2.8  2009/11/10 20:02:17  ukr16032
 * UECC S1AP_STATUS_MSG_IND handling added
 *
 * Revision 1.1.2.28.2.7  2009/11/10 11:48:15  ukr15916
 * downlink_nas_transport and nas_non_delivery_indication updated for ASN S1AP
 *
 * Revision 1.1.2.28.2.6  2009/11/06 09:52:12  ukr15916
 * UECC ue_context_release_command message updated for ASN S1AP
 *
 * Revision 1.1.2.28.2.5  2009/10/30 15:21:08  ukr15916
 * Define RRC_UECC_N_UE is removed. Dynamic UE number supporting improved.
 *
 * Revision 1.1.2.28.2.4  2009/10/30 12:47:44  ukr15916
 * UECC-OAMH interface -- Get Cell Status and Get UE Status
 * requests handling added
 *
 *
 ****************************************************************************/

#include "uecc_ed.h"
#include "uecc_common.h"
#include "rrc_defines.h"
#include "rrc_ext_api.h"
#include "rrc_intrl_api.h"

#include "rrc_msg_mgmt.h"
#include "uecc_oamh.h"
#include "uecc_csc.h"
#include "uecc_llim.h"
#include "uecc_s1ap.h"
#include "uecc_rrm.h"

#include "uecc_event_queue.h"
#include "uecc_utils.h"

#include "uecc_logger.h"
#include "uecc_ue_timer_utils.h"

#include "uecc_fsm_common.h"
#include "uecc_fsm_engine.h"
#include "uecc_global_ctx.h"
#include "rrc_handlers.h"
#include "uecc_x2ap.h"
/*SPR_17893_START*/
#include "uecc_ct_a.h"
/*SPR_17893_END*/


/****************************************************************************
 * Private Function definitions
 ****************************************************************************/
static rrc_return_et uecc_check_csc_message
(
    uecc_gb_context_t *p_uecc_gb_context    /* UECC global context */
);

static rrc_return_et uecc_check_rrm_message
(
    uecc_gb_context_t *p_uecc_gb_context    /* UECC global context */
);

static rrc_return_et uecc_check_llim_message
(
    uecc_gb_context_t *p_uecc_gb_context    /* UECC global context */
);

static rrc_return_et uecc_check_s1ap_message
(
    uecc_gb_context_t *p_uecc_gb_context    /* UECC global context */
);


/* SPR 21803 Fix Start */
rrc_return_et uecc_check_x2ap_message
/* SPR 21803 Fix Stop */
(
    uecc_gb_context_t *p_uecc_gb_context    /* UECC global context */
);

static void uecc_ue_process_timer_message
(
    uecc_gb_context_t*      p_uecc_gb_context,      /* UECC global context */
    uecc_timer_buffer_t*    p_uecc_timer_buffer    /* Timer buffer */
);
/* Measurment Re-config START */
static void uecc_ue_process_meas_config_guard_timer_message
(
    uecc_gb_context_t*      p_uecc_gb_context,      /* UECC global context */
    uecc_timer_buffer_t*    p_uecc_timer_buffer    /* Timer buffer */
);


static void uecc_ue_process_counter_check_guard_timer_message
(
    uecc_gb_context_t*      p_uecc_gb_context,      /* UECC global context */
    uecc_timer_buffer_t*    p_uecc_timer_buffer    /* Timer buffer */
);
/* Measurment Re-config END */

static void uecc_ue_process_store_ue_context_timer_message
(
    uecc_gb_context_t*      p_uecc_gb_context,      /* UECC global context */
    uecc_timer_buffer_t*    p_uecc_timer_buffer    /* Timer buffer */
);

static void uecc_process_queue
(
    uecc_gb_context_t*      p_uecc_gb_context,
    uecc_event_queue_t*     p_uecc_event_queue  /* Pointer to UECC queue */
);

void rrc_uecc_process_health_monitor_chk
(
    uecc_gb_context_t   *p_uecc_gb_context
);
/*SPR_17893_START*/
void rrc_uecc_process_ue_capability_enq_timer_exp_handler
(
    uecc_gb_context_t   *p_uecc_gb_context,
    uecc_timer_buffer_t*    p_uecc_timer_buffer
);
/*SPR_17893_END*/
/*SPR_19066_START*/
void uecc_llim_process_ho_prep_info_resp
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);
/*SPR_19066_END*/

/****************************************************************************
 * Global variables declarations
 ****************************************************************************/
/* OAMH messages handlers */
static uecc_intf_handlers_table_t uecc_intf_oamh_handlers_table[]=
{
    { { UECC_OAMH_GET_CELL_STATUS_REQ   },
            uecc_oamh_get_cell_status_req_processs_msg },
    { { UECC_OAMH_GET_UE_STATUS_REQ     },
            uecc_oamh_get_ue_status_req_processs_msg },
    { { UECC_OAMH_CELL_TRAFFIC_TRACE_START  },
            uecc_oamh_cell_traffic_trace_start_processs_msg },
    { { UECC_OAMH_CELL_TRAFFIC_TRACE_STOP  },
            uecc_oamh_cell_traffic_trace_stop_processs_msg },
    { { UECC_OAMH_TRACE_FILE_TRANSFER_COMPLETE_IND  },
            uecc_oamh_trace_file_transfer_complete_ind_processs_msg },
    { { UECC_LAST_ID                    }, PNULL }
};

/* UECCMD messages handlers */
static uecc_intf_handlers_table_t uecc_intf_ueccmd_handlers_table[]=
{
    { { UECC_OAMH_PROV_REQ              },
            uecc_oamh_prov_req_processs_msg },
    { { UECC_OAMH_CLEANUP_REQ           },
            uecc_oamh_clear_req_processs_msg    },
    /* lipa start */
    { { UECC_OAMH_ADD_DEL_LGW_IND           },
            uecc_oamh_add_del_lgw_ind_processs_msg    },
    /* lipa end */
    { { UECC_LAST_ID                    }, PNULL }
};

/* CSC messages handlers */
static uecc_intf_handlers_table_t uecc_intf_csc_handlers_table[]=
{
    { { CSC_UECC_INIT_SETUP_IND }, uecc_csc_process_init_setup_ind  },

    { { CSC_UECC_DELETE_ALL_UE_REQ }, uecc_csc_process_del_all_ue_req },
    { { CSC_UECC_CELL_RECONFIG_IND }, uecc_csc_process_cell_reconfig_ind },
    { { CSC_UECC_CELL_START_IND }, uecc_csc_process_cell_start_ind },
    { { CSC_UECC_CELL_STOP_IND }, uecc_csc_process_cell_stop_ind },
    { { UECC_LAST_ID            }, PNULL                            }
    
};

/* RRM messages handlers */
static uecc_intf_handlers_table_t uecc_intf_rrm_handlers_table[]=
{
    { { RRC_RRM_UE_ADMISSION_RESP           },
            uecc_rrm_process_ue_admission_resp          },
    { { RRC_RRM_UE_RELEASE_RESP             },
            uecc_rrm_process_ue_release_resp            },
    { { RRC_RRM_ERB_SETUP_RESP              },
            uecc_rrm_process_erb_setup_resp             },
    { { RRC_RRM_UE_CONNECTION_RELEASE_IND   },
            uecc_rrm_process_ue_connection_release_ind  },
    { { RRC_RRM_UE_CAPABILITY_ENQUIRY_REQ   },
            uecc_rrm_process_ue_capability_enquiry      },
    { { RRC_RRM_UE_CONTEXT_MOD_RESP   },
            uecc_rrm_process_ue_context_modification_resp },
    { { RRC_RRM_ERB_MODIFY_RESP              },
            uecc_rrm_process_erb_modify_resp             },
    /* UE RECONFIGURATION REQ*/
    { { RRC_RRM_UE_RECONFIG_REQ   },
            uecc_rrm_process_ue_reconfig_request  },
    /* ERB RELEASE COMMAND START */
    { { RRC_RRM_ERB_RELEASE_RESP   },
            uecc_rrm_process_erb_release_resp  },
    /* ERB RELEASE COMMAND END */
    { { RRC_RRM_ERB_RELEASE_IND   },
            uecc_rrm_process_erb_release_ind  },

/*REL 1.2:HANDOVER PROCEDURE START*/
    { { RRC_RRM_HO_REQUIRED                 },
            uecc_rrm_process_ho_required                },
    { { RRC_RRM_UE_HO_ADM_RESP              },
            uecc_rrm_process_ue_ho_adm_resp             },
    { { RRC_RRM_HO_CANCEL_REQ               },
            uecc_rrm_process_ho_cancel_req              },
    { { RRC_RRM_UE_HO_CMD_RESP              },
            uecc_rrm_process_ue_ho_cmd_resp             },
    { { RRC_RRM_MEAS_CONFIG_REQ              },
            uecc_rrm_process_meas_config_req            },
    { { RRC_RRM_CSFB_PARAMETERS_RESP_CDMA2000              },
            uecc_rrm_process_csfb_parameters_resp_cdma2000 },
    { { RRC_RRM_COUNTER_CHECK_REQ              },
            uecc_rrm_process_counter_check_req            },
    { { RRC_RRM_UE_INFORMATION_REQ              },
            uecc_rrm_process_ue_information_req            },
/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
    { { RRC_RRM_DC_BEARER_CHANGE_REQ             },
            uecc_rrm_process_dc_bearer_change_req          },
#endif
/* OPTION3X Changes Start */
    { { UECC_LAST_ID                        }, PNULL    }
};

/* LLIM messages handlers */
static uecc_intf_handlers_table_t uecc_intf_llim_handlers_table[]=
{
    { { UECC_LLIM_CREATE_UE_ENTITY_RESP },
            uecc_llim_process_create_ue_entity_resp },
    { { UECC_LLIM_DELETE_UE_ENTITY_RESP },
            uecc_llim_process_delete_ue_entity_resp },
    { { UECC_LLIM_CCCH_DATA_IND         },
            uecc_llim_process_ccch_data_ind         },
    { { UECC_LLIM_SRB_DATA_IND          },
            uecc_llim_process_srb_data_ind          },

    { { UECC_LLIM_ADD_LC_RESP           },
            uecc_llim_process_add_lc_resp           },
    { { UECC_LLIM_SRB_DATA_STATUS_IND   },
            uecc_llim_process_srb_data_status_ind   },
    { { UECC_LLIM_CONFIGURE_SECURITY_RESP   },
            uecc_llim_process_configure_security_resp   },
    { { UECC_LLIM_S1U_ERROR_IND         },
            uecc_llim_process_s1u_error_ind },
    { { UECC_LLIM_S1U_PATH_FAILURE_IND  },
            uecc_llim_process_s1u_path_failure_ind },
    { { UECC_LLIM_S1U_PATH_SUCCESS_IND  },
            uecc_llim_process_s1u_path_success_ind },
    /* SPR 15644 fix start */
    { { UECC_LLIM_RADIO_LINK_FAILURE_IND },
	    uecc_llim_process_sync_status_ind  },
    /* SPR 15644 fix end */
    { { UECC_LLIM_MODIFY_LC_RESP           },
	    uecc_llim_process_modify_lc_resp           },
    { { UECC_LLIM_ADD_ROLLBACK_RESP     },
            uecc_llim_process_add_rollback_resp        },
    { { UECC_LLIM_MODIFY_ROLLBACK_RESP     },
            uecc_llim_process_modify_rollback_resp        },
    { { UECC_LLIM_DELETE_LC_RESP           },
            uecc_llim_process_delete_lc_resp           },
    { { UECC_LLIM_SUSPEND_UE_ENTITY_RESP },
            uecc_llim_suspend_ue_entity_resp  },
    { { UECC_LLIM_MAC_I_RESP            },
            uecc_llim_process_mac_i_resp },
    { { UECC_LLIM_SN_HFN_STATUS_RESP    },
            uecc_llim_process_sn_hfn_status_resp },
    { { UECC_LLIM_UE_INACTIVE_TIME_RESP },
            uecc_llim_process_ue_inactive_time_resp },
    { { UECC_LLIM_HO_RACH_RESOURCE_RESP },
            uecc_llim_process_ho_rach_resource_resp },
    { { UECC_LLIM_S1U_RECONFIGURE_CNF   },
            uecc_llim_process_s1u_reconfigure_cnf   },
    { { UECC_LLIM_S1U_END_MARKER_IND   },
            uecc_llim_process_s1u_end_marker_ind   },
    { { UECC_LLIM_RE_ESTABLISH_UE_ENTITY_RESP   },
            uecc_llim_process_re_establish_ue_entity_resp },
    { { UECC_LLIM_RECONFIG_SECURITY_RESP   },
            uecc_llim_process_reconfig_security_resp },
    { { UECC_LLIM_RESUME_UE_ENTITY_RESP   },
            uecc_llim_process_resume_ue_entity_resp },
    { { UECC_LLIM_CHANGE_CRNTI_RESP   },
            uecc_llim_process_change_crnti_resp },
    { { UECC_LLIM_MEAS_CONFIG_RESP   },
            uecc_llim_process_meas_config_resp },
    { { UECC_LLIM_INACTIVE_UES_IND   },
            uecc_llim_process_inactive_ues_ind },
    { { UECC_LLIM_COUNT_WRAPAROUND_IND   },
            uecc_llim_process_count_wraparound_ind },
    { { UECC_LLIM_RESET_UE_ENTITY_CNF   },
            uecc_llim_process_reset_ue_entity_cnf },
    /* SPR 4118 Start */
    { { UECC_LLIM_NOTIFY_INTEGRITY_FAILURE   },
            uecc_llim_process_notify_integrity_failure },
    /* SPR 4118 Stop */
    { { UECC_LLIM_DRB_COUNT_MSB_RESP   },
            uecc_llim_process_drb_count_msb_resp },
    /*SPR:6462 start*/
    { { UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF   },
            uecc_llim_process_reestab_comp_cnf },
    /*SPR:6462 stop*/
/*BUG 604 changes start*/
    { { UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_CNF   },
            uecc_llim_process_intra_enb_data_fwd_cnf },
    { { UECC_LLIM_S1U_LOCAL_PATH_SWITCH_CNF   },
            uecc_llim_process_local_path_switch_cnf },
/*BUG 604 changes stop*/
 /*Bug 357 Fix Start*/
    { { UECC_LLIM_PDCP_RE_ESTABLISH_UE_ENTITY_RESP   },
            uecc_llim_process_re_establish_ue_entity_resp },
 /*Bug 357 Fix Stop*/
    /*SPR_19066_START*/
    { { UECC_LLIM_HO_PREP_INFO_RESP },
            uecc_llim_process_ho_prep_info_resp },
    /*SPR_19066_END*/
    { { UECC_LAST_ID                    }, PNULL    }
};

/* S1AP messages handlers */
static uecc_intf_handlers_table_t uecc_intf_s1ap_handlers_table[]=
{
    { { S1AP_STATUS_MSG_IND             },
            uecc_s1ap_process_status_msg_ind },
    { { S1AP_UE_ASSOCIATED_SIG_MSG_IND  },
            uecc_s1ap_process_ue_associated_sig_msg_ind},
    { { S1AP_RESET_MSG_IND              },
            uecc_s1ap_process_reset_msg_ind  },
    { { S1AP_DEL_CONTEXT_MSG_IND        },
            uecc_s1ap_process_del_ctx_msg_ind },
    { { S1AP_OVERLOAD_MSG_IND           },
            uecc_s1ap_process_s1ap_overload_msg_ind },
    { { S1AP_UE_ASSOCIATED_MME_UPDATE_STATUS_IND },
            uecc_s1ap_process_ue_associated_mme_update_status_ind },
    { { S1AP_UE_REL_REQ_INT           },
            uecc_s1ap_process_ue_rel_req_int },
    { { S1AP_ALLOCATE_MME_RESP           },
            uecc_s1ap_process_alloc_mme_resp },
    { { UECC_LAST_ID                    }, PNULL       }
};

/* X2AP messages handlers */
static uecc_intf_handlers_table_t uecc_intf_x2ap_handlers_table[]=
{
    { { X2AP_STATUS_MSG_IND             },
            uecc_x2ap_process_status_msg_ind },
    { { X2AP_UE_ASSOCIATED_SIG_MSG_IND  },
            uecc_x2ap_process_ue_associated_sig_msg_ind },
    { { X2AP_RESET_MSG_IND              },
            uecc_x2ap_process_reset_msg_ind },
    { { X2AP_UE_RLF_MSG_IND             },
            uecc_x2ap_process_rlf_msg_ind },
    /* Bug 4691 Changes Start */
    { { X2AP_UPDATE_NBOUR_INFO},
            uecc_x2ap_process_update_nbour_info },
    /* Bug 4691 Changes End */
    { { X2AP_UPDATE_NR_NBOUR_INFO},
            uecc_x2ap_process_update_nr_nbour_info },
    { { UECC_LAST_ID                    }, PNULL       }
};


/* Table with RRC modules and handlers tables */
static uecc_intf_modules_table_t uecc_intf_modules_table[] =
{
    { { RRC_OAMH_MODULE_ID  }, PNULL,
            uecc_intf_oamh_handlers_table   },
    { { RRC_UECCMD_MODULE_ID  }, PNULL,
            uecc_intf_ueccmd_handlers_table   },
    { { RRC_CSC_MODULE_ID   }, uecc_check_csc_message,
            uecc_intf_csc_handlers_table    },
    { { RRC_RRM_MODULE_ID   }, uecc_check_rrm_message,
            uecc_intf_rrm_handlers_table    },
    { { RRC_LLIM_MODULE_ID  }, uecc_check_llim_message,
            uecc_intf_llim_handlers_table   },
    { { RRC_S1AP_MODULE_ID  }, uecc_check_s1ap_message,
            uecc_intf_s1ap_handlers_table   },
    /* SPR 21803 Fix Start */
    { { RRC_X2AP_MODULE_ID  }, PNULL,
    /* SPR 21803 Fix Stop */
            uecc_intf_x2ap_handlers_table   },
    { { UECC_LAST_ID        }, PNULL                  , PNULL   }
};

/****************************************************************************
 * Function implementation
 ****************************************************************************/

/********************************************************************
 *
 *  FUNCTION NAME : rrc_uecc_process_msg
 *
 *
 *  INPUT         : void *p_api
 *                  void *p_context
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : It returns the success or failure status.
 *
 *  DESCRIPTION   : This is the entry point function of uecc module.It
 *                  processes the received api and deliver it to the
 *                  respective state.
 *******************************************************************/

S32 rrc_uecc_process_msg
(
    void *p_api,        /* Input API buffer */
    void *p_context      /* UECC global context */
)
{
    uecc_gb_context_t      *p_gb_context = PNULL;
    uecc_event_queue_t     event_queue;
    p_uecc_intf_handler_t   message_handler = PNULL;
    U16                     modules_index = UECC_LAST_ID;
    U16                     message_index = UECC_LAST_ID;


    /* SPR 22964 Fix Start */
    SET_UE_INDEX(0xFFFF); 
    /* SPR 22964 Fix End */

    /* RRC_UECC_TRACE(RRC_INFO, RRC_MSG_RECVD); */

    /* Precondition check */
    /* Check general message integrity too */
    if ( (PNULL == p_api) || (PNULL == p_context) )
    {
        RRC_TRACE(RRC_ERROR,
            "rrc_uecc_process_msg: somebody call us in wrong context");

        /* Encapsulating shell should release the buffer */
        return RRC_BUFFER_SHOULD_BE_RELEASED;
    }
    p_gb_context = (uecc_gb_context_t *)p_context;
    RRC_UECC_UT_TRACE_ENTER(p_gb_context);

    if (RRC_SUCCESS != rrc_check_cspl_header(p_api))
    {
        RRC_UECC_TRACE(p_gb_context->context_index,
                p_gb_context->facility_name,RRC_WARNING,
                "rrc_uecc_process_msg: bad message header - ignore msg");
        RRC_UECC_UT_TRACE_EXIT(p_gb_context);

        /* Encapsulating shell should release the buffer */
        return RRC_BUFFER_SHOULD_BE_RELEASED;
    }


    /* Init UECC FSM Event queue */
    uecc_event_queue_init(&event_queue);
    p_gb_context->p_event_queue = &event_queue;

    do
    {
        /* find module id */
        modules_index = uecc_find_index_by_id(
                        rrc_get_src_module_id(p_api),
                        (uecc_id_t*)uecc_intf_modules_table,
                        sizeof(uecc_intf_modules_table_t));

        if (!( modules_index <= (RRC_UECCMD_MODULE_ID - RRC_MIN_INT_MODULE_ID)))
        {
            /* source module id is not supported -- drop it */
            RRC_UECC_TRACE(p_gb_context->context_index,p_gb_context->
                    facility_name,RRC_WARNING,
                "rrc_uecc_process_msg: "
                "Received API from unexpected module [0x%02X]",
                rrc_get_src_module_id(p_api) );
            break;
        }
        /* check is message valid in current context */
        if(PNULL!=uecc_intf_modules_table[modules_index].p_uecc_intf_check_func)
        {
            if (RRC_SUCCESS !=
                uecc_intf_modules_table[modules_index].p_uecc_intf_check_func(
                    p_gb_context))
            {
                /* drop message */
                RRC_UECC_TRACE(p_gb_context->context_index,p_gb_context->
                        facility_name,RRC_WARNING,
                    "rrc_uecc_process_msg: check message validity failed");
                break;
            }
        }

        RRC_ASSERT(PNULL !=
            uecc_intf_modules_table[modules_index].p_uecc_handlers_table);

        /* find API id */
        message_index = uecc_find_index_by_id(
                        rrc_get_api_id(p_api),
                        (uecc_id_t*)uecc_intf_modules_table[modules_index].
                            p_uecc_handlers_table,
                        sizeof(uecc_intf_handlers_table_t));

        message_handler = uecc_intf_modules_table[modules_index].
            p_uecc_handlers_table[message_index].p_uecc_intf_handler;

        if (PNULL == message_handler)
        {
            RRC_UECC_TRACE(p_gb_context->context_index,p_gb_context->
                    facility_name,RRC_WARNING,
                "rrc_uecc_process_msg: "
                "No event handler for received API [0x%04X]",
                rrc_get_api_id(p_api));
            break;
        }

        /* run message handler */
        message_handler(p_api, p_gb_context);
    }
    while(0);

    /* Process queued events*/
    uecc_process_queue(p_gb_context, &event_queue);

    RRC_UECC_UT_TRACE_EXIT(p_gb_context);

    return RRC_BUFFER_SHOULD_BE_RELEASED;
}

/********************************************************************
 *
 *  FUNCTION NAME : uecc_check_csc_message
 *
 *  INPUT         : uecc_gb_context_t *p_uecc_gb_context
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : RRC_SUCCESS/RRC_FAILURE
 *
 *  DESCRIPTION   : This function checks validity of incoming
 *                  CSC message
 *******************************************************************/
rrc_return_et uecc_check_csc_message
(
    uecc_gb_context_t *p_uecc_gb_context    /* UECC global context */
)
{
    rrc_return_et    result = RRC_SUCCESS;
    RRC_ASSERT(p_uecc_gb_context != PNULL);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    /* Check if UECC have been initialized by OAMH */
    if (UECC_READY_OAMH_FLAG !=
        (p_uecc_gb_context->ready_condition & UECC_READY_OAMH_FLAG) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name,RRC_WARNING,
            "uecc_check_csc_message: "
            "message dropped - UECC isn't initialized by OAMH");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        result = RRC_FAILURE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/********************************************************************
 *
 *  FUNCTION NAME : uecc_check_rrm_message
 *
 *  INPUT         : uecc_gb_context_t *p_uecc_gb_context
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : RRC_SUCCESS/RRC_FAILURE
 *
 *  DESCRIPTION   : This function checks validity of incoming
 *                  RRM message
 r
 *******************************************************************/
rrc_return_et uecc_check_rrm_message
(
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    rrc_return_et    result = RRC_SUCCESS;
    rrc_bool_et     is_any_cell_active = RRC_FALSE;
    U8              index = RRC_NULL;
    RRC_ASSERT(p_uecc_gb_context != PNULL);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    
    if (UECC_READY_OAMH_FLAG !=
            (p_uecc_gb_context->ready_condition & UECC_READY_OAMH_FLAG) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name,RRC_WARNING,
            "uecc_check RRm message when cell delete procedure is triggered: "
            "message dropped - UECC isn't initialized by OAMH");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        result = RRC_FAILURE;
    }
    else if(UECC_READY_OAMH_FLAG ==
            (p_uecc_gb_context->ready_condition & UECC_READY_OAMH_FLAG) )
    {
        if ( PNULL != p_uecc_gb_context->p_p_csc_context )
        {
            for (index = 0; index < MAX_NUM_CELLS; index++)
            {
                if (PNULL != p_uecc_gb_context->p_p_csc_context[index])
                {
                    if (PNULL != p_uecc_gb_context->p_p_csc_context[index]->
                            p_csc_init_setup_ind)
                    {
                        if ( RRC_CSC_CELL_ACTIVE == 
                                p_uecc_gb_context->p_p_csc_context[index]->
                                p_csc_init_setup_ind->cell_state )
                        {
                            is_any_cell_active = RRC_TRUE;
                            break;
                        }
                    }
                }
            }

        }
        if ( RRC_FALSE == is_any_cell_active )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name,RRC_WARNING,
                    "uecc_check RRm message dropped - "
                    "UECC isn't initialized by CSC or OAMH");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            result = RRC_FAILURE;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/********************************************************************
 *
 *  FUNCTION NAME : uecc_check_llim_message
 *
 *  INPUT         : uecc_gb_context_t *p_uecc_gb_context
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : RRC_SUCCESS/RRC_FAILURE
 *
 *  DESCRIPTION   : This function checks validity of incoming
 *                  LLIM message
 *
 *******************************************************************/
rrc_return_et uecc_check_llim_message
(
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    rrc_return_et    result = RRC_SUCCESS;
    rrc_bool_et     is_any_cell_active = RRC_FALSE;
    U8              index = RRC_NULL;

    RRC_ASSERT(p_uecc_gb_context != PNULL);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    
    if (UECC_READY_OAMH_FLAG !=
            (p_uecc_gb_context->ready_condition & UECC_READY_OAMH_FLAG) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name,RRC_WARNING,
                "uecc_check LLIM message when cell delete procedure is"
                "triggered: "
                "message dropped - UECC isn't initialized by OAMH");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        result = RRC_FAILURE;
    }
    else if(UECC_READY_OAMH_FLAG ==
            (p_uecc_gb_context->ready_condition & UECC_READY_OAMH_FLAG) )
    {
        if (PNULL != p_uecc_gb_context->p_p_csc_context)
        {
            for (index = 0; index < MAX_NUM_CELLS; index++)
            {
                if (PNULL != p_uecc_gb_context->p_p_csc_context[index])
                {
                    if (PNULL != p_uecc_gb_context->p_p_csc_context[index]->
                            p_csc_init_setup_ind)
                    {
                        if ((RRC_CSC_CELL_ACTIVE == 
                                p_uecc_gb_context->p_p_csc_context[index]->p_csc_init_setup_ind
                                ->cell_state) ||
                             (1 == p_uecc_gb_context->p_p_csc_context[index]->del_cell_ongoing))
                        {   
                            is_any_cell_active = RRC_TRUE;
                            break;
                        }
                    }
                }
            }
        }

        if (RRC_FALSE == is_any_cell_active)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name,RRC_WARNING,
                    "uecc_check LLIM message dropped - "
                    "UECC isn't initialized by oAMH or CSC");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            result = RRC_FAILURE;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/********************************************************************
 *
 *  FUNCTION NAME : uecc_check_s1ap_message
 *
 *  INPUT         : uecc_gb_context_t *p_uecc_gb_context
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : RRC_SUCCESS/RRC_FAILURE
 *
 *  DESCRIPTION   : This function checks validity of incoming
 *                  S1AP message
 *
 *******************************************************************/
rrc_return_et uecc_check_s1ap_message
(
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    rrc_return_et    result = RRC_SUCCESS;

    RRC_ASSERT(p_uecc_gb_context != PNULL);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    
    if (UECC_READY_OAMH_FLAG !=
            (p_uecc_gb_context->ready_condition & UECC_READY_OAMH_FLAG) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name,RRC_WARNING,
            "uecc_check S1AP message when cell delete procedure is triggered: "
            "message dropped - UECC isn't initialized by OAMH");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        result = RRC_FAILURE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}


/********************************************************************
 *
 *  FUNCTION NAME : uecc_check_x2ap_message
 *
 *  INPUT         : uecc_gb_context_t *p_uecc_gb_context
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : RRC_SUCCESS/RRC_FAILURE
 *
 *  DESCRIPTION   : This function checks validity of incoming
 *                  X2AP message
 *
 *******************************************************************/
rrc_return_et uecc_check_x2ap_message
(
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
)
{
    rrc_return_et    result = RRC_SUCCESS;
    rrc_bool_et     is_any_cell_active = RRC_FALSE;
    U8              index = RRC_NULL;

    RRC_ASSERT(p_uecc_gb_context != PNULL);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    
    if (UECC_READY_OAMH_FLAG !=
            (p_uecc_gb_context->ready_condition & UECC_READY_OAMH_FLAG) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name,RRC_WARNING,
            "uecc_check X2AP message when cell delete procedure is triggered: "
            "message dropped - UECC isn't initialized by OAMH");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        result = RRC_FAILURE;
    }
    else if(UECC_READY_OAMH_FLAG ==
            (p_uecc_gb_context->ready_condition & UECC_READY_OAMH_FLAG) )
    {
        if (PNULL != p_uecc_gb_context->p_p_csc_context)
        {
            for (index = 0; index < MAX_NUM_CELLS; index++)
            {
                if (PNULL != p_uecc_gb_context->p_p_csc_context[index])
                {
                    if (PNULL != p_uecc_gb_context->p_p_csc_context[index]->
                            p_csc_init_setup_ind)
                    {
                        if (RRC_CSC_CELL_ACTIVE == 
                                p_uecc_gb_context->p_p_csc_context[index]->p_csc_init_setup_ind->
                                cell_state)
                        {   
                            is_any_cell_active = RRC_TRUE;
                            break;
                        }
                    }
                }
            }
        }
        if (RRC_FALSE == is_any_cell_active)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name,RRC_WARNING,
                    "uecc_check X2AP message dropped -"
                    "UECC isn't initialized by OAMH or CSC");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            result = RRC_FAILURE;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/********************************************************************
 *
 *  FUNCTION NAME : uecc_ue_process_timer_message
 *
 *  INPUT         : uecc_gb_context_t *p_uecc_gb_context
 *                  uecc_timer_buffer_t*    p_uecc_timer_buffer
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : None
 *
 *  DESCRIPTION   : This function processes timers for UECC FSMs
 *
 *******************************************************************/
void uecc_ue_process_timer_message
(
    uecc_gb_context_t*      p_uecc_gb_context,      /* UECC global context */
    uecc_timer_buffer_t*    p_uecc_timer_buffer    /* Timer buffer */
)
{
    uecc_ue_context_t* p_ue_context = PNULL;

    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_uecc_timer_buffer);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
/* spr 22224 fix start */
    p_ue_context =
        uecc_ue_ctx_get(p_uecc_gb_context, p_uecc_timer_buffer->ue_index);

    if (( PNULL == p_ue_context )
        )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_ERROR, "The UE context isn't found.");
        return;
    }

    
    if(RRC_FALSE == uecc_ue_timers_descriptors[ p_uecc_timer_buffer->id].is_repeated)
    {
    }
    {
    uecc_ue_timer_mark_as_expired(
            p_uecc_gb_context, 
            p_uecc_timer_buffer->id,
            &p_ue_context->timers_data);

    uecc_event_queue_push_event(
        p_uecc_timer_buffer->fsm_event_id,
        p_ue_context,
        PNULL);
    }
/* spr 22224 fix end */
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/* Measurment Re-config START */
/********************************************************************
 *
 *  FUNCTION NAME : uecc_ue_process_meas_config_guard_timer_message
 *
 *  INPUT         : uecc_gb_context_t*      p_uecc_gb_context
 *                  uecc_timer_buffer_t*    p_uecc_timer_buffer
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : None
 *
 *  DESCRIPTION   : This function processes expiry of meas_config_guard_timer 
 *
 *******************************************************************/
void uecc_ue_process_meas_config_guard_timer_message
(
    uecc_gb_context_t*      p_uecc_gb_context,      /* UECC global context */
    uecc_timer_buffer_t*    p_uecc_timer_buffer    /* Timer buffer */
)
{
	/* coverity_fix_54730_start */
	uecc_ue_context_t* p_ue_context = PNULL;
	s1ap_Cause cause;

	RRC_ASSERT(PNULL != p_uecc_gb_context);
	RRC_ASSERT(PNULL != p_uecc_timer_buffer);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

	p_ue_context =
		uecc_ue_ctx_get(p_uecc_gb_context, p_uecc_timer_buffer->ue_index);
	if ( PNULL == p_ue_context )
	{
		RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
				facility_name, RRC_ERROR, "The UE context isn't found.");
		return;
	}

	RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
			facility_name, RRC_INFO,
			"UECC_UE_MEAS_CONFIG_GUARD_TIMER Timer Expired.");

	uecc_ue_timer_mark_as_expired(
			p_uecc_gb_context, 
			p_uecc_timer_buffer->id,
			&p_ue_context->timers_data);

	/* Timer expired while waiting for RRC Connection Reconfiguration 
	 * Complete */
	if (RRC_TRUE == p_ue_context->is_meas_transaction_id_valid)
	{
		/* Build and send the RRC_RRM_MEAS_CONFIG_RESP(failure) to RRM. */
		if (RRC_SUCCESS != uecc_rrm_build_and_send_meas_config_resp(
					p_ue_context, RRC_FAILURE, RRC_TIMER_EXPIRY))
		{
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name,RRC_WARNING,
					"Failure to build and send meas config resp to RRM.");
		}

		/* Build and send UE Release Req to MME with following cause */
		cause.t = T_s1ap_Cause_radioNetwork;
		cause.u.radioNetwork = s1ap_release_due_to_eutran_generated_reason;
		/* Now build and send UE CONTEXT RELEASE REQ to MME */
        /* SPR 18911 Fix Start */
        /* Will not send the UE CONTEXT RELEASE REQ to MME if release is already
         * ongoing for the same UE */
        if (p_ue_context->uecc_fsm.p_current_state->s_name != PNULL)
        {
            if ((strcmp_wrapper(p_ue_context->uecc_fsm.p_current_state->s_name,
                                        (const S8 *)("UECC_UE_WAIT_FOR_S1AP_UE_CTX_REL_CMD_ONGOING"))))
            {
		        if (RRC_SUCCESS != uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
				        	p_ue_context,
					        &cause))
		        {
			        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
					    p_uecc_gb_context->facility_name,
					    RRC_WARNING, "failure to send UE CONTEXT RELEASE REQ to MME.");
		        }
            }
        }
        /* SPR 18911 Fix Stop */
	}
	/* Timer got expired while waiting response from LLIM */
	else 
	{
		/* Build and send the RRC_RRM_MEAS_CONFIG_RESP(failure) to RRM. */
		if (RRC_SUCCESS != uecc_rrm_build_and_send_meas_config_resp(
					p_ue_context, RRC_FAILURE, RRC_NO_ERROR))
		{
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name,RRC_WARNING,
					"Failure to build and send meas config resp to RRM.");
		}
	}
	/* Mark this flag as false */
	p_ue_context->is_meas_transaction_id_valid = RRC_FALSE;

	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/* coverity_fix_54730_stop */
/* Measurment Re-config END */

/*Counter Check Start */
/********************************************************************
 *
 *  FUNCTION NAME : uecc_ue_process_counter_check_guard_timer_message
 *
 *  INPUT         : uecc_gb_context_t*      p_uecc_gb_context
 *                  uecc_timer_buffer_t*    p_uecc_timer_buffer
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : None
 *
 *  DESCRIPTION   : This function processes expiry of counter_check_guard_timer 
 *
 *******************************************************************/
void uecc_ue_process_counter_check_guard_timer_message
(
    uecc_gb_context_t*      p_uecc_gb_context,      /* UECC global context */
    uecc_timer_buffer_t*    p_uecc_timer_buffer    /* Timer buffer */
)
{
    uecc_ue_context_t* p_ue_context = PNULL;

    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_uecc_timer_buffer);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    p_ue_context =
        uecc_ue_ctx_get(p_uecc_gb_context, p_uecc_timer_buffer->ue_index);
    if ( PNULL == p_ue_context )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_ERROR, "The UE context isn't found.");
        return;
    }
    
    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_INFO,
                "UECC_UE_COUNTER_CHECK_GUARD_TIMER Timer Expired.");

    uecc_ue_timer_mark_as_expired(
            p_uecc_gb_context, 
            p_uecc_timer_buffer->id,
            &p_ue_context->timers_data);

    /* Build and send the RRC_RRM_COUNTER_CHECK_RESP(failure) to RRM. */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_counter_check_failure(
                p_ue_context, RRM_RRC_TIMER_EXPIRED))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name,RRC_WARNING,
                "Failure during uecc_rrm_build_and_send_counter_check_failur message.");
    }


    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/*Counter Check End */

/********************************************************************
 *
 *  FUNCTION NAME : uecc_process_queue
 *
 *  INPUT         : uecc_gb_context_t*      p_uecc_gb_context
 *                  uecc_event_queue_t*     p_uecc_event_queue
 *
 *  OUTPUT        : None
 *
 *  RETURNS       : None
 *
 *  DESCRIPTION   : This function processes UECC event queue
 *
 *******************************************************************/
void uecc_process_queue
(
    uecc_gb_context_t*      p_uecc_gb_context,
    uecc_event_queue_t*     p_uecc_event_queue  /* Pointer to UECC queue */
)
{
    uecc_event_t*   p_event = PNULL;

    RRC_ASSERT(PNULL != p_uecc_event_queue);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable p_uecc_gb_context=%p"
                       ,p_uecc_gb_context);/*SPR 17777 +-*/
    while ( PNULL!=(p_event = uecc_event_queue_pop_event(p_uecc_event_queue)) )
    {
        if(p_event->p_ue_context != PNULL)
        {
        /* Send event to corresponding FSM */
        uecc_fsm_process_event(p_event->p_ue_context,
            p_event->event_id, p_event->p_msg_info);
        }


        /* Free event */
        rrc_mem_free(p_event);
        p_event = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/********************************************************************
 * FUNCTION NAME : uecc_ue_process_store_ue_context_timer_message
 *
 * INPUT         : uecc_gb_context_t*      p_uecc_gb_context
 *                 uecc_timer_buffer_t*    p_uecc_timer_buffer
 *
 * OUTPUT        : None
 *
 * RETURNS       : None
 * DESCRIPTION   : This function processes expiry of store_ue_context_timer
 * *****************************************************************/
void uecc_ue_process_store_ue_context_timer_message
(
    uecc_gb_context_t*      p_uecc_gb_context,      /* UECC global context */
    uecc_timer_buffer_t*    p_uecc_timer_buffer    /* Timer buffer */
)
{
	uecc_ue_context_t* p_ue_context = PNULL;
	RRC_ASSERT(PNULL != p_uecc_gb_context);
	RRC_ASSERT(PNULL != p_uecc_timer_buffer);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

	p_ue_context = uecc_ue_ctx_get(p_uecc_gb_context, p_uecc_timer_buffer->ue_index);
	if ( PNULL == p_ue_context )
	{
		RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
				facility_name, RRC_ERROR, "The UE context isn't found.");
		return;
	}

	uecc_ue_timer_mark_as_expired(
			p_uecc_gb_context,
			p_uecc_timer_buffer->id,
			&p_ue_context->timers_data);

/*SPR_18059_START*/
  /*code removed */
/*SPR_18059_END*/

	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/****************************************************************************
 * Function Name  : rrc_uecc_process_timer_msg
 * Inputs         : rrc_timer_t timer_id
 *                  void        *p_timer_buf
 *                  void        *p_context
 * Outputs        : none
 * Returns        : none
 * Description    : This is the entry function for the timer expiries.
 ****************************************************************************/
void rrc_uecc_process_timer_msg
(
    rrc_timer_t timer_id,       /* Identity of the expired timer */
    void        *p_timer_buf,   /* Timer buffer */
    void        *p_context      /* UECC global context */
)
{
    uecc_timer_buffer_t    *p_uecc_timer_buffer = PNULL;
    uecc_gb_context_t      *p_gb_context = PNULL;
    uecc_event_queue_t     event_queue;



    /* Precondition check */
    if ( (PNULL == p_timer_buf) || (PNULL == p_context) )
    {
        RRC_TRACE(RRC_ERROR,
            "rrc_uecc_process_timer_msg: called in wrong context");
        return;
    }

    p_gb_context = (uecc_gb_context_t *)p_context;
    p_uecc_timer_buffer = (uecc_timer_buffer_t *)p_timer_buf;

    RRC_UECC_UT_TRACE_ENTER(p_gb_context);
    
    RRC_UECC_TRACE(p_gb_context->context_index,
                   p_gb_context-> facility_name, 
                   RRC_INFO, "Timer event processing.");

    /* Init UECC FSM Event queue */
    uecc_event_queue_init(&event_queue);
    p_gb_context->p_event_queue = &event_queue;

    RRC_UECC_TRACE(p_gb_context->context_index,
                   p_gb_context-> facility_name, 
                   RRC_INFO, "rrc_uecc_process_timer_msg : %s timer id expired",
                   uecc_ue_timers_descriptors[ p_uecc_timer_buffer->id].str_id);

    if (UECC_TIMER_TYPE_CONTROL == p_uecc_timer_buffer->type)
    {
        uecc_oamh_process_timer_msg(timer_id,
            p_gb_context);
    }
    else
    {
        switch (p_uecc_timer_buffer->id)
        {
        case UECC_UE_RCS_TIMER:
        case UECC_UE_ICS_TIMER:
        case UECC_UE_RCR_TIMER:
        case UECC_UE_MME_GUARD_TIMER:
        case UECC_RLF_W_FOR_REEST_TIMER:
            /*SPR_17893_START*/
            /*Case Moved*/
            /*SPR_17893_END*/
        case UECC_UE_CONTXT_MOD_TIMER:
        case UECC_ERAB_SETUP_GUARD_TIMER:        
        case UECC_ERAB_MODIFY_GUARD_TIMER:
        case UECC_RLF_UE_SUSPEND_GUARD_TIMER:
        case UECC_ERAB_RELEASE_CMD_GUARD_TIMER:
        case UECC_ERAB_RELEASE_IND_GUARD_TIMER:
        case UECC_UE_HO_PREP_TIMER:
        case UECC_UE_HO_OVERALL_TIMER:
        case UECC_UE_TRG_HO_GUARD_TIMER:
        case UECC_UE_LL_GUARD_TIMER:
        case UECC_UE_CRE_TIMER:
        /*x2_ho_timer :start*/
        case UECC_UE_X2_HO_PREP_TIMER:
        case UECC_UE_X2_HO_OVERALL_TIMER:
        /*x2_ho_timer :end*/
        case UECC_UE_INTRA_CELL_HO_GUARD_TIMER:
        case UECC_RRM_UE_RECONFIG_GUARD_TIMER:
        /* SPR 4118 Start */
        case UECC_UE_INTEGRITY_FAILURE_TIMER:
        /* SPR 4118 Stop */
        case UECC_UE_CCO_GUARD_TIMER:
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
        case UECC_DC_PREP_TIMER:
        case UECC_DC_BEARER_CHANGE_GUARD_TIMER:
/*NR_DC Code Change Stop*/
#endif
            uecc_ue_process_timer_message(p_gb_context,
                p_uecc_timer_buffer);
             break;
        case UECC_UE_MEAS_CONFIG_GUARD_TIMER:
        uecc_ue_process_meas_config_guard_timer_message(p_gb_context,
                p_uecc_timer_buffer);
            break;
        case UECC_UE_COUNTER_CHECK_GUARD_TIMER:
        uecc_ue_process_counter_check_guard_timer_message(p_gb_context,
                p_uecc_timer_buffer);
            break;
        case UECC_UE_INFORMATION_GUARD_TIMER:
            uecc_rrm_ue_information_req_timer_expiry_handler(p_gb_context,
                    p_uecc_timer_buffer);
            break;

        case UECC_STORE_UE_CONTEXT_TIMER:
            uecc_ue_process_store_ue_context_timer_message(
                    p_gb_context, p_uecc_timer_buffer);
            break;

        case UECC_HEALTH_MONITOR_TIMER:
            RRC_UECC_TRACE(p_gb_context->context_index, p_gb_context->
                    facility_name,RRC_INFO,
                "rrc_uecc_process_timer_msg: "
                "processing health monitor timer");
            rrc_uecc_process_health_monitor_chk(p_gb_context);
            break;

            /*SPR_17893_START*/
        case UECC_UE_CAPABILITY_ENQ_TIMER:
            RRC_UECC_TRACE(p_gb_context->context_index, p_gb_context->
                    facility_name,RRC_INFO,
                "rrc_uecc_process_timer_msg: "
                "processing ue capability timer expiry");
            rrc_uecc_process_ue_capability_enq_timer_exp_handler(p_gb_context,
                    p_uecc_timer_buffer);
            break;
            /*SPR_17893_END*/

        default:
            RRC_UECC_TRACE(p_gb_context->context_index, p_gb_context->
                    facility_name,RRC_ERROR,
                "rrc_uecc_process_timer_msg: "
                "timer event received for unexpected submodule");
            break;
        }
    }

    /* Process queued events*/
    uecc_process_queue(p_gb_context, &event_queue);

    if (RRC_TRUE == p_uecc_timer_buffer->auto_delete)
    {
        if (PNULL != p_timer_buf)
        {
            rrc_mem_free(p_timer_buf);
	    /* + Coverity 81419 */
            //p_timer_buf = PNULL;
	    /* - Coverity 81419 */
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_gb_context);
}

/****************************************************************************
 * Function Name  : rrc_uecc_process_health_monitor_chk
 * Inputs         : p_uecc_gb_context - pointer to UECC global context data
 * Outputs        : None
 * Returns        : void
 * Description    : This function checks the UECC Module ID and update the 
 *                  the global health monitor variable for the
 *                  corresponding UECC module.
 ****************************************************************************/

void rrc_uecc_process_health_monitor_chk
(
    uecc_gb_context_t   *p_uecc_gb_context
)
{
    RRC_ASSERT(PNULL != p_uecc_gb_context);

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    switch (p_uecc_gb_context->uecc_module_id)
    {
        case  RRC_UECC_1_MODULE_ID:
            rrc_update_health_monitor_variable(UECC1_HEALTH_MONITOR_EXPIRY_EVENT);
            break;
        case  RRC_UECC_2_MODULE_ID:
            rrc_update_health_monitor_variable(UECC2_HEALTH_MONITOR_EXPIRY_EVENT);
            break;
        case  RRC_UECC_3_MODULE_ID:
            rrc_update_health_monitor_variable(UECC3_HEALTH_MONITOR_EXPIRY_EVENT);
            break;
        case  RRC_UECC_4_MODULE_ID:
            rrc_update_health_monitor_variable(UECC4_HEALTH_MONITOR_EXPIRY_EVENT);
            break;
        case  RRC_UECC_5_MODULE_ID:
            rrc_update_health_monitor_variable(UECC5_HEALTH_MONITOR_EXPIRY_EVENT);
            break;
        case  RRC_UECC_6_MODULE_ID:
            rrc_update_health_monitor_variable(UECC6_HEALTH_MONITOR_EXPIRY_EVENT);
            break;
        case  RRC_UECC_7_MODULE_ID:
            rrc_update_health_monitor_variable(UECC7_HEALTH_MONITOR_EXPIRY_EVENT);
            break;
        case  RRC_UECC_8_MODULE_ID:
            rrc_update_health_monitor_variable(UECC8_HEALTH_MONITOR_EXPIRY_EVENT);
            break;
        default:
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name,RRC_WARNING,
                    "Wrong Module Id");
            break;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/*SPR_17893_START*/
/****************************************************************************
 * Function Name  : rrc_uecc_process_ue_capability_enq_timer_exp_handler
 * Inputs         : p_uecc_gb_context - pointer to UECC global context data
 * Outputs        : None
 * Returns        : void
 * Description    : This function processes ue capability timer expiry
 ****************************************************************************/

void rrc_uecc_process_ue_capability_enq_timer_exp_handler
(
    uecc_gb_context_t   *p_uecc_gb_context,
    uecc_timer_buffer_t*    p_uecc_timer_buffer
)
{
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_uecc_timer_buffer);

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    uecc_ue_context_t* p_ue_context = PNULL;

    p_ue_context =
        uecc_ue_ctx_get(p_uecc_gb_context, p_uecc_timer_buffer->ue_index);
    if ( PNULL == p_ue_context )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_ERROR, "The UE context isn't found.");
        return;
    }

    if(RRC_FALSE == p_ue_context->rrm_ue_capability_enq_flag_after_attach )
    {
        uecc_ue_process_timer_message(p_uecc_gb_context,
                p_uecc_timer_buffer);
    }
    else
    {
        uecc_ue_timer_mark_as_expired(
                p_uecc_gb_context, 
                p_uecc_timer_buffer->id,
                &p_ue_context->timers_data);

        uecc_ue_capability_timer_expiry_handler(p_ue_context,PNULL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/*SPR_17893_END*/
