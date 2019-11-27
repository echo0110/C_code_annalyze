/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_rmfsm.c,v 1.41 2010/12/01 13:03:00 gur19827 Exp $
 ****************************************************************************
 *
 *  File Description : UECC RM FSM functions implementation
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_rmfsm.c,v $
 * Revision 1.41  2010/12/01 13:03:00  gur19827
 * Merged Inter RAT HO
 *
 * Revision 1.40  2010/10/19 03:18:48  gur11120
 * SPR 708 fix
 *
 * Revision 1.39  2010/10/13 07:03:38  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.10.4.8  2010/10/10 13:34:38  gur04336
 * made change to set pointer as NULL after memfree
 *
 * Revision 1.10.4.7  2010/10/08 12:36:40  gur21006
 * TDD changes merged
 *
 * Revision 1.10.4.6  2010/10/05 12:36:38  gur22047
 * compilation warning removed
 *
 * Revision 1.10.4.5  2010/10/04 13:53:58  gur04518
 * Removed TABS
 *
 * Revision 1.10.4.4  2010/09/13 13:04:03  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.10.4.3  2010/08/05 08:07:14  gur15697
 * merging round 2
 *
 * Revision 1.10.4.2  2010/07/21 08:34:41  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.10.4.1  2010/06/22 05:31:59  gur22047
 * Internal Interface update
 *
 * Revision 1.10  2010/05/14 09:36:58  gur19827
 * checklist,warning_removal,dst mod id,tdd,phy cell id added
 *
 * Revision 1.9  2010/04/19 09:17:44  gur20470
 * Added check for srb2 info present in erb setup resp
 *
 * Revision 1.8  2010/04/13 11:23:58  gur10248
 * SRB2 changes
 *
 * Revision 1.7  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.6  2010/03/17 12:50:56  gur10248
 * CR201_changes
 *
 * Revision 1.5  2010/02/24 04:26:37  gur10248
 * comments_dpisposed_cell_del_SRB2_1_1
 *
 * Revision 1.4  2010/02/18 04:19:02  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.3  2010/01/11 11:42:42  gur10248
 * *** empty log message ***
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.66  2009/12/22 16:39:44  gur18569
 * commented field name initialization in fsm activity structures
 *
 * Revision 1.1.2.65  2009/12/11 11:27:34  ukr16032
 * UECC SSIT - LLIM response codes processing updated for RM and RCR
 *
 * Revision 1.1.2.64  2009/12/09 15:36:27  ukr16032
 * UECC - inspection log items PN_4, SG_1, SG_6, AK_1, AK_12, AK_25, AK_27 fixed
 *
 * Revision 1.1.2.63  2009/12/08 16:05:18  ukr15916
 * LG_14 fixed
 *
 * Revision 1.1.2.62  2009/12/08 12:06:15  ukr16032
 * UECC - inspection log items LG_21 fixed
 *
 * Revision 1.1.2.61  2009/12/08 11:22:51  ukr16032
 * UECC - inspection log items LG_20, LG_23, LG_24, LG_26, LG_27, LG_28, LG_29, LG_34, LG_40, LG_41 fixed
 *
 * Revision 1.1.2.60  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.58.2.21  2009/11/24 12:12:51  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.2.58.2.20  2009/11/24 11:32:35  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.2.58.2.19.2.8  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.2.58.2.19.2.7  2009/11/20 19:14:40  ukr16032
 * UECC ICS failure processing updated
 *
 * Revision 1.1.2.58.2.19.2.6  2009/11/20 15:38:45  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.58.2.19.2.5  2009/11/20 13:17:29  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.58.2.19.2.4  2009/11/20 12:57:30  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.58.2.19.2.3  2009/11/20 12:16:23  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.58.2.19.2.2  2009/11/20 10:37:47  ukr15916
 * RRM_ERAB_SETUP_RADIO_RES_CONFIG_DRB_TO_ADD_LIST_PRESENT is used
 *
 * Revision 1.1.2.58.2.19.2.1  2009/11/19 18:56:34  ukr16032
 * UECC RRM updated according to the last interface changes.
 *
 * Revision 1.1.2.58.2.19  2009/11/19 10:24:57  ukr15916
 * using memset_wrapper,l3_memcpy_wrapper,memcmp_wrapper
 *
 * Revision 1.1.2.58.2.18  2009/11/19 09:52:17  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.2.58.2.17  2009/11/17 09:42:25  ukr18880
 * uecc_ue_ctx_get_mme_ue_id(p_ue_context) function is now used instead of
 * directly using mme_ue_id from UE context, in all cases except it initializing
 *
 * Revision 1.1.2.58.2.16  2009/11/16 18:32:52  ukr16032
 * UECC ISA activity implementation started
 *
 * Revision 1.1.2.58.2.15  2009/11/16 11:02:49  ukr18880
 * 80 lines
 *
 * Revision 1.1.2.58.2.14  2009/11/14 16:29:54  ukr16032
 * UECC UE Capability transfer implemented
 * UECC ICSP - security capability checking implemented
 *
 * Revision 1.1.2.58.2.13  2009/11/13 13:14:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.2.58.2.12  2009/11/13 12:36:55  ukr18880
 * *** empty log message ***
 *
 * Revision 1.1.2.58.2.11  2009/11/13 12:23:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.2.58.2.10  2009/11/12 14:36:33  ukr16032
 * UECC - Initial UE context setup procedure moved to uecc_icsp.*
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Header File Includes
 ***************************************************************************/

#include "uecc_rmfsm.h"
#include "uecc_common.h"
#include "uecc_llim.h"
#include "uecc_rrm.h"
#include "uecc_s1ap.h"
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
#include "uecc_x2ap.h"
/*NR_DC Code Change Stop*/
#endif
#include "uecc_logger.h"
#include "uecc_ed.h"
#include "uecc_mainfsm.h"
#include "uecc_fsm_engine.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "uecc_defaults.h"
#include "rrc_phy_intf.h"
#include <lteMisc.h>
/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_RM_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,\
        p_ue_context->ue_index,\
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
        ##__VA_ARGS__)

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/

/* State UECC_RRM_RESOURCE_ADMISSION_ONGOING */
static void uecc_fsm_rm_rrm_resource_admission_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_rrm_erb_setup_resp_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

#ifdef RRC_UT_FRAMEWORK
void uecc_fsm_rm_rrm_resource_admission_ongoing_timer_expiry_handler(
#else
static void uecc_fsm_rm_rrm_resource_admission_ongoing_timer_expiry_handler(
#endif
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/* State UECC_LLIM_RECONFIGURE_UE_ONGOING */
static void uecc_fsm_rm_llim_reconfigure_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

#ifdef RRC_UT_FRAMEWORK
void uecc_fsm_rm_llim_reconfigure_ue_resp_handler(
#else
static void uecc_fsm_rm_llim_reconfigure_ue_resp_handler(
#endif
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

#ifdef RRC_UT_FRAMEWORK
void uecc_fsm_rm_llim_reconfigure_ue_ongoing_timer_expiry_handler(
#else
static void uecc_fsm_rm_llim_reconfigure_ue_ongoing_timer_expiry_handler(
#endif
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/* State UECC_UE_RECONFIGURE_ONGOING */
static void uecc_fsm_rm_ue_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_rrc_connection_reconfiguration_complete_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rm_ue_reconfigure_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

/* Failure processing functions */
static void uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

/*SPR 18693 Fix Start*/
/*Code Removed*/
/*SPR 18693 Fix Stop*/

static void uecc_fsm_rm_fail_action_delete_drb_info_from_ctx(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

/*SPR 18693 Fix Start*/
/*Code Removed*/
/*SPR 18693 Fix Stop*/

static void uecc_fsm_rm_fail_action_last(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

static rrc_return_t uecc_fsm_rm_process_rrm_erab_setup_resp_data(
    uecc_ue_context_t*  p_ue_context,
    rrc_rrm_erb_setup_resp_t* p_rrm_erb_setup_resp
);

static rrc_return_t uecc_fsm_rm_read_all_success_drb(
    uecc_ue_context_t*  p_ue_context,    
    rrm_drb_to_add_info_list_t*  p_drb_to_add_list
);

static void uecc_fsm_rm_read_all_failed_drb(
    uecc_ue_context_t*  p_ue_context,    
    rrm_drb_failed_to_add_info_list_t*  p_drb_failed_to_add_list
);

static rrc_return_t uecc_fsm_fetch_failed_drb_from_sucess_list(
    uecc_ue_context_t*  p_ue_context,    
    rrm_drb_to_add_info_list_t*  p_drb_to_add_list
);

static rrc_return_t uecc_fsm_chk_at_least_one_non_gbr_rab(
    uecc_ue_context_t*  p_ue_context
);

rrc_return_et uecc_is_default_bearer_established
(
    uecc_ue_context_t*  p_ue_context
);

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

#ifdef ENDC_ENABLED
/*NR_DC Code Changes Start*/
/* Failure processing functions */
/*coverity_<278332>_fix_start*/
#if 0
static void uecc_fsm_rm_fail_action_last_endc(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);
#endif
/*coverity_<278332>_fix_end*/
/* coverity_<277510>_fix_start */
#if 0
static void uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf_endc(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);
#endif
/* coverity_<277510>_fix_end*/
static void uecc_fsm_rm_fail_stop_proc_timer(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

static void uecc_fsm_rm_sgnb_addition_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_sgnb_addition_ongoing_addition_ack_rcvd(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_sgnb_addition_ongoing_timer_expiry(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_sgnb_addition_ongoing_sgnb_add_req_reject(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_sgnb_addition_ongoing_x2ap_error_ind(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

static void uecc_fsm_rm_fail_action_send_sgnb_reconfig_complete(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);
/* coverity_<280379>_fix_start*/
#if 0
static void uecc_fsm_rm_fail_action_send_sgnb_release(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);
#endif
/* coverity_<280379>_fix_end*/
static void uecc_fsm_rm_scg_failure_information_nr_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
    );

/****************************************************************************
 * Function Name  : uecc_fsm_chk_if_sgnb_addition_reqd
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrm_drb_to_add_info_list_t*   - DRB to be added list
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion checks if any of the DRB is 
 *                  SN_TERMINATED_SCG_BEARER, if yes the same is updated in the 
 *                  ue context
 ****************************************************************************/
rrc_return_t uecc_fsm_chk_if_sgnb_addition_reqd(
        uecc_ue_context_t*  p_ue_context,
        rrm_drb_to_add_info_list_t*  p_drb_to_add_list
);
/*NR_DC Code Change Stop*/
#endif


/* SPR 13775 Fix Stop */
/****************************************************************************
 Private Variables
****************************************************************************/

/**************************************
 * States definition
 **************************************/
static uecc_fsm_handler_record_t
                    uecc_fsm_rm_rrm_resource_admission_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
            uecc_fsm_rm_rrm_resource_admission_ongoing_init_handler         },
    { { UECC_EV_RRM_ERB_SETUP_RESP              },
            uecc_fsm_rm_rrm_erb_setup_resp_handler                          },
    { { UECC_EV_TIMER_EXPIRY                    },
            uecc_fsm_rm_rrm_resource_admission_ongoing_timer_expiry_handler },
    { { UECC_LAST_ID                            },  PNULL                   }
};

static uecc_fsm_handler_record_t
                        uecc_fsm_rm_llim_reconfigure_ue_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
            uecc_fsm_rm_llim_reconfigure_ue_ongoing_init_handler            },
    { { UECC_EV_LLIM_RECONFIGURE_UE_RESP        },
            uecc_fsm_rm_llim_reconfigure_ue_resp_handler                    },
    { { UECC_EV_TIMER_EXPIRY                    },
            uecc_fsm_rm_llim_reconfigure_ue_ongoing_timer_expiry_handler    },
    { { UECC_LAST_ID                            },  PNULL                   }
};

static uecc_fsm_handler_record_t uecc_fsm_rm_ue_reconfigure_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                              },
            uecc_fsm_rm_ue_reconfigure_ongoing_init_handler             },
    { { UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE },
            uecc_fsm_rm_rrc_connection_reconfiguration_complete_handler },
    { { UECC_EV_TIMER_EXPIRY                            },
            uecc_fsm_rm_ue_reconfigure_ongoing_timer_expiry_handler     },
#ifdef ENDC_ENABLED
/*NR_DC Code Change START*/
    { { UECC_EV_SCG_FAILURE_INFORMATION_NR              },
            uecc_fsm_rm_scg_failure_information_nr_handler              },
/*NR_DC Code Change Stop*/
#endif
    { { UECC_LAST_ID                                    },  PNULL       }
};

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
static uecc_fsm_handler_record_t uecc_fsm_rm_sgnb_addition_ongoing_mht[] = 
{
    { { UECC_EV_STATE_INIT                              },
            uecc_fsm_rm_sgnb_addition_ongoing_init_handler              },
    { { UECC_EV_X2AP_SGNB_ADDITION_REQ_ACK},
            uecc_fsm_rm_sgnb_addition_ongoing_addition_ack_rcvd         },
    { { UECC_EV_TIMER_EXPIRY                            },
            uecc_fsm_rm_sgnb_addition_ongoing_timer_expiry              },
    { { UECC_EV_X2AP_SGNB_ADDITION_REQ_REJECT           },
            uecc_fsm_rm_sgnb_addition_ongoing_sgnb_add_req_reject       },
    { { UECC_EV_X2AP_ERROR_INDICATION                   },
            uecc_fsm_rm_sgnb_addition_ongoing_x2ap_error_ind            },
    { { UECC_LAST_ID                                    },  PNULL       }
};
/*NR_DC Code Change Stop*/
#endif

#ifdef RRC_UT_FRAMEWORK
uecc_fsm_activity_state_t uecc_fsm_rm_rrm_resource_admission_ongoing =
#else
static uecc_fsm_activity_state_t uecc_fsm_rm_rrm_resource_admission_ongoing =
#endif
{
   /* .s_name         = */  (S8*)"UECC_RRM_RESOURCE_ADMISSION_ONGOING",
   /* .s_id           = */  (S8*)"RM_RRM",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_rrm_resource_admission_ongoing_mht
};

#ifdef RRC_UT_FRAMEWORK
uecc_fsm_activity_state_t uecc_fsm_rm_llim_reconfigure_ue_ongoing =
#else
static uecc_fsm_activity_state_t uecc_fsm_rm_llim_reconfigure_ue_ongoing =
#endif
{
   /* .s_name         = */  (S8*)"UECC_LLIM_RECONFIGURE_UE_ONGOING",
   /* .s_id           = */  (S8*)"RM_LLIM",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_llim_reconfigure_ue_ongoing_mht
};

#ifdef RRC_UT_FRAMEWORK
uecc_fsm_activity_state_t uecc_fsm_rm_ue_reconfigure_ongoing =
#else
static uecc_fsm_activity_state_t uecc_fsm_rm_ue_reconfigure_ongoing =
#endif
{
   /* .s_name         = */  (S8*)"UECC_UE_RECONFIGURE_ONGOING",
   /* .s_id           = */  (S8*)"RM_UE",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_ue_reconfigure_ongoing_mht
};

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
static uecc_fsm_activity_state_t uecc_fsm_rm_sgnb_addition_ongoing = 
{
   /* .s_name         = */  (S8*)"UECC_X2AP_SGNB_ADDITION_ONGOING",
   /* .s_id           = */  (S8*)"SGNB_ADD",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_rm_sgnb_addition_ongoing_mht
};
/*NR_DC Code Change Stop*/
#endif

/**************************************
 * Activity definition
 **************************************/
/* Activity */
uecc_fsm_activity_t uecc_fsm_rrc_connection_reconfiguration_activity =
{
   /* .s_name             =*/ (S8*)"RRC Connection Reconfiguration",
   /* .s_id               =*/ (S8*)"UE_RCRECONF_A",
   /* .p_first_state      =*/ &uecc_fsm_rm_rrm_resource_admission_ongoing
};

/**************************************
 * Failure actions definition
 **************************************/

/* RM failure actions lists */
static const uecc_fsm_fail_handler_f uecc_fsm_rm_fail_wait_rrm_actions[] =
{
    uecc_fsm_rm_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_fail_action_last,
    PNULL
};

static const
    uecc_fsm_fail_handler_f uecc_fsm_rm_fail_wait_rrm_actions_no_response[] =
{
    uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_rm_fail_wait_llim_ue_actions[] =
{
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    uecc_fsm_rm_fail_stop_proc_timer,
    uecc_fsm_rm_fail_action_send_sgnb_reconfig_complete,
/*NR_DC Code Change Stop*/
#endif
    uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf,
/*SPR 18693 Fix Start*/
/*Code Removed*/
/*SPR 18693 Fix Stop*/
    uecc_fsm_rm_fail_action_delete_drb_info_from_ctx,
/*SPR 18693 Fix Start*/
/*Code Removed*/
/*SPR 18693 Fix Stop*/
    uecc_fsm_rm_fail_action_last,
    PNULL
};

/* fail actions when LLIM is unrecoverable */
static const uecc_fsm_fail_handler_f
                            uecc_fsm_rm_fail_wait_llim_unrecov_actions[] =
{
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    uecc_fsm_rm_fail_stop_proc_timer,
    uecc_fsm_rm_fail_action_send_sgnb_reconfig_complete,
/*NR_DC Code Change Stop*/
#endif
    uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf,
/*SPR 18693 Fix Start*/
/*Code Removed*/
/*SPR 18693 Fix Stop*/
    uecc_fsm_rm_fail_action_delete_drb_info_from_ctx,
/*SPR 18693 Fix Start*/
/*Code Removed*/
/*SPR 18693 Fix Stop*/
    uecc_fsm_rm_fail_action_last,
    PNULL
};
/* coverity_<277520>_fix_start */
/*static const uecc_fsm_fail_handler_f 
                            uecc_fsm_rm_fail_timer_expiry_wait_llim_ue_actions[] =
{
#ifdef ENDC_ENABLED
//NR_DC Code Change Start
    uecc_fsm_rm_fail_action_send_sgnb_reconfig_complete,
//NR_DC Code Change Stop
#endif
    uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_fail_action_last,
    PNULL
};*/
/* coverity_<277520>_fix_end */

static const uecc_fsm_fail_handler_f 
                            uecc_fsm_rm_fail_timer_expiry_ue_reconfig_actions[] =
{
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
    uecc_fsm_rm_fail_action_send_sgnb_reconfig_complete,
/*NR_DC Code Change Stop*/
#endif
    uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_fail_action_last,
    PNULL
};

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
static const uecc_fsm_fail_handler_f 
                            uecc_fsm_rm_fail_sgnb_addition_request_fail[] =
{
    uecc_fsm_rm_fail_stop_proc_timer,
    uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_fail_action_last,
    PNULL
};


static const uecc_fsm_fail_handler_f 
                            uecc_fsm_rm_fail_wait_sgnb_actions[] =
{
    uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f
                            uecc_fsm_rm_fail_wait_ue_reconfig_actions[] =
{
    uecc_fsm_rm_fail_stop_proc_timer,
    uecc_fsm_rm_fail_action_send_sgnb_reconfig_complete,
    uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_fail_action_last,
    PNULL
};
/* coverity_<277510>_fix_start */
/*static const uecc_fsm_fail_handler_f 
                            uecc_fsm_rm_fail_wait_sgnb_actions_partial[] =
{
    uecc_fsm_rm_fail_action_send_sgnb_release,
    uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf_endc,
    uecc_fsm_rm_fail_action_last_endc,
    PNULL
};*/
/* coverity_<277510>_fix_start */
/*NR_DC Code Change Stop*/

/*Error_Handling_changes*/
static const uecc_fsm_fail_handler_f 
                            uecc_fsm_rm_ics_proc_error_ind_fail_1_actions[] =
{
    uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf,
    uecc_fsm_rm_fail_action_delete_drb_info_from_ctx,
    uecc_fsm_rm_fail_action_last,
    PNULL
};
/*Error_Handling_changes*/
#endif




/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/
/****************************************************************************
* Function Name  : store_macMeasConfig_ueContext 
* Inputs         : uecc_ue_context_t* - pointer to UE context
* Returns        : RRC_FAILURE/RRC_SUCCESS
* Description    : storing mac config of ERAB to be setup and mac measurement
*                  info from RRM_ERAB_SETUP_RES msg into UE context
****************************************************************************/
static rrc_return_t store_macMeasConfig_ueContext(
        uecc_ue_context_t *p_ue_context,
        rrc_rrm_erb_setup_resp_t *p_rrm_erb_setup_resp
        )
{
    U16 sub_frame_assignment = RRC_NULL;
    U8 index = 0;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rrm_erb_setup_resp);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if( (PNULL != p_ue_context->p_gb_context->p_p_csc_context) &&
        (PNULL != p_ue_context->p_gb_context->p_p_csc_context
            [p_ue_context->cell_index]) &&
        (PNULL != p_ue_context->p_gb_context->p_p_csc_context
            [p_ue_context->cell_index]->p_csc_init_setup_ind) &&
            /* Store mac_main_config_t if present */
        (RRM_ERAB_SETUP_RADIO_RES_CONFIG_MAC_CONFIG_PRESENT &
            p_rrm_erb_setup_resp->radio_rsource_config.bitmask ))
    {
        p_ue_context->m.mac_config_updated = 1;

        if(DUPLEXING_MODE_FDD == p_ue_context->p_gb_context->p_p_csc_context[
                p_ue_context->cell_index]->p_csc_init_setup_ind->
                duplexing_mode)
        {
            if ((p_rrm_erb_setup_resp->radio_rsource_config.mac_config.
                        mac_main_config_extensions.dl_num_harq_process <
                        RRC_RRM_FDD_DL_HARQ_PROCESS_MIN) ||
                    (p_rrm_erb_setup_resp->radio_rsource_config.mac_config.
                     mac_main_config_extensions.dl_num_harq_process > RRC_RRM_FDD_DL_HARQ_PROCESS_MAX))
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,RRC_ERROR,
                        "[RRC_RRM_ERB_SETUP_ADMISSION_RESP]"
                        "Duplexing Mode = FDD "
                        "and dl_num_harq_process = %u",
                        p_rrm_erb_setup_resp->radio_rsource_config.mac_config.
                        mac_main_config_extensions.dl_num_harq_process);
                return RRC_FAILURE;
            }
        }

        if( RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT &
                p_rrm_erb_setup_resp->radio_rsource_config.mac_config.bitmask)
        {
            /*** TDD changes start ***/
            if(DUPLEXING_MODE_TDD == p_ue_context->p_gb_context->p_p_csc_context[
                    p_ue_context->cell_index]->p_csc_init_setup_ind->
                    duplexing_mode)
            {
                sub_frame_assignment = p_ue_context->p_gb_context->p_p_csc_context[
                    p_ue_context->cell_index]->p_csc_init_setup_ind->
                    sib_type_1_Info.tdd_config.sub_frame_assignment;

                if (RRC_TRUE == p_rrm_erb_setup_resp->radio_rsource_config.
                        mac_config.mac_main_config.ul_sch_config.tti_bundling)
                {
                    /* TTI bundling can be enabled for FDD and for TDD only for
                     *                      * configurations 0, 1 and 6 */
                    if (sa_0 != sub_frame_assignment &&
                            sa_1 != sub_frame_assignment &&
                            sa_6 != sub_frame_assignment)
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,
                                RRC_WARNING,"ICS: TTI bundling can be"\
                                " enabled for FDD and for TDD only for"
                                " configurations 0, 1 and 6");
                        return RRC_FAILURE;
                    }
                    if (RRM_ERAB_SETUP_RADIO_RES_CONFIG_SPS_CONFIG_PRESENT &
                            p_rrm_erb_setup_resp->radio_rsource_config.bitmask )
                    {
                        /*For TDD, E-UTRAN does not simultaneously enable TTI
                         *                          * bundling and semi-persistent scheduling */
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,
                                RRC_WARNING,"ICS: For TDD, E-UTRAN does not"
                                " simultaneously enable TTI bundling and"
                                " semi-persistent scheduling");
                        return RRC_FAILURE;
                    }

                }
            }
            /*** TDD changes stop ***/


            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_DETAILED,"ICS: mac_main_config present in RRM ERB setup resp");
            p_ue_context->m.mac_main_config_present = 1;

  	    /* SPR 13775 Fix Start */
  	    /* SPR 13775 Fix Stop */
        }
  	    /* SPR 13775 Fix Start *
	    updateMacConfigInUeCtx(p_ue_context, &p_ue_context->mac_config ,
		&p_rrm_erb_setup_resp->radio_rsource_config.mac_config);
  	    /* SPR 13775 Fix Stop */
        memcpy(&p_ue_context->mac_config, &p_rrm_erb_setup_resp->radio_rsource_config.mac_config, sizeof(p_ue_context->mac_config));
    }

    /*** TDD changes start ***/
    /* Store sps_config if present */
    if (RRM_ERAB_SETUP_RADIO_RES_CONFIG_SPS_CONFIG_PRESENT &
            p_rrm_erb_setup_resp->radio_rsource_config.bitmask )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_INFO,"SPS present");
        /* SPS Start */
        if (RRC_FAILURE == uecc_rrm_parse_and_update_sps_config(
                    p_ue_context,
                    &p_rrm_erb_setup_resp->radio_rsource_config.sps_config))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING,"[RRC_RRM_ERB_SETUP_RESP] SPS config updation failed");
        }
        /* SPS Stop */
        p_ue_context->m.sps_config_updated = 1;
    }
    /*** TDD changes stop ***/

    /* Store rrm_meas_config_t if present */
    if (RRM_ERAB_SETUP_RADIO_RES_CONFIG_MEAS_CONFIG_PRESENT &
            p_rrm_erb_setup_resp->radio_rsource_config.bitmask )
    {
        if (p_rrm_erb_setup_resp->radio_rsource_config.meas_config.bitmask & 
                UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT)
        {            
            for (index = 0; index < p_rrm_erb_setup_resp->radio_rsource_config.
                    meas_config.meas_object_to_add_mod_list.count; index++)
            {
                if (p_rrm_erb_setup_resp->radio_rsource_config.meas_config.
                        meas_object_to_add_mod_list.meas_object_to_add_mod[index].
                        meas_object.bitmask & MEAS_OBJECT_TO_ADD_EUTRA_PRESENT)
                {
                    p_ue_context->m.measObjectEutraPresent = RRC_TRUE;
                }
            }
        }
        if (p_ue_context->m.measObjectEutraPresent != RRC_TRUE)
        {
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_ERB_SETUP_RESP] measurment Object for EUTRA not present:CR201");
            return RRC_FAILURE;
        }

        p_ue_context->m.meas_config_updated = 1;
        p_ue_context->m.meas_config_present = 1;

        /*Memory Optimization */
        /*valgrind_fix_start*/
        if(PNULL ==  p_ue_context->p_meas_config)
        {
            p_ue_context->p_meas_config = (rrm_meas_config_t*) 
                rrc_mem_get (sizeof (rrm_meas_config_t));

            if (p_ue_context->p_meas_config == PNULL)
            {
                RRC_UECC_RM_FSM_TRACE(RRC_FATAL,
                        "[RRC_RRM_ERB_SETUP_RESP] Memory Allocation Failure");
                return RRC_FAILURE;
            }
            memset_wrapper (p_ue_context->p_meas_config, 0, sizeof (rrm_meas_config_t));
        }
        /*valgrind_fix_stop*/
        l3_memcpy_wrapper(p_ue_context->p_meas_config,
                &p_rrm_erb_setup_resp->radio_rsource_config.meas_config,
                sizeof(rrm_meas_config_t));

    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : store_phyConfigDedinfo_UeContext 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Returns        : RRC_FAILURE/RRC_SUCCESS
 * Description    : storing physical config dedicated info of ERAB to be setup
 *                  from RRM_ERAB_SETUP_RES msg into UE context
 ****************************************************************************/
static rrc_return_t store_phyConfigDedinfo_ueContext(uecc_ue_context_t *p_ue_context,
        rrc_rrm_erb_setup_resp_t *p_rrm_erb_setup_resp
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rrm_erb_setup_resp);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /* Store physical_config_dedicated if present */
    if (RRM_ERAB_SETUP_RADIO_RES_CONFIG_PHY_CONFIG_DED_PRESENT &
            p_rrm_erb_setup_resp->radio_rsource_config.bitmask )
    {
        p_ue_context->m.physical_config_dedicated_updated = 1;

        /* cr_311 sr_ConfigIndex range check */
        if ((p_ue_context->m.access_stratum_release) && 
                (((p_rrm_erb_setup_resp->radio_rsource_config.physical_config_dedicated.
                   scheduling_request_config.scheduling_request_config_param.
                   sr_configuration_index == 156) ||
                  (p_rrm_erb_setup_resp->radio_rsource_config.physical_config_dedicated.
                   scheduling_request_config.scheduling_request_config_param.
                   sr_configuration_index == 157)) &&
                 ((p_ue_context->access_stratum_release == RRC_UE_REL_8))) )
        {
            RRC_UECC_TRACE (p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_INFO,"Release 8 do not support"
                    " Values 156 or 157 for sr_ConfigIndex");
        }

  	/* SPR 13775 Fix Start *
	updatePhyConfigDedInUeCtx(p_ue_context, 
	    &p_ue_context->physical_config_dedicated,
	    &p_rrm_erb_setup_resp->radio_rsource_config.physical_config_dedicated);
  	/* SPR 13775 Fix Stop */
    memcpy(&p_ue_context->physical_config_dedicated, &p_rrm_erb_setup_resp->radio_rsource_config.physical_config_dedicated, sizeof(p_ue_context->physical_config_dedicated));

        /* CBSR Changes Start */

        if(RRC_PHY_ANTENNA_INFORMATION_PRESENT &
                p_ue_context->physical_config_dedicated.bitmask)
        {
            if(!(RRC_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT &
                        p_ue_context->physical_config_dedicated
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
                            &p_ue_context->physical_config_dedicated.
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


        /* CBSR Changes Stop */
        /* cr_316 : verifying antenna_info_v920 along with TM 8 presence */
        if (p_ue_context->physical_config_dedicated.antenna_info_v920.bitmask &
                RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT)
        {
            if((RRC_PHY_ANTENNA_INFORMATION_PRESENT &
                        p_ue_context->physical_config_dedicated.bitmask) &&
                    (p_ue_context->physical_config_dedicated.antenna_information.
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
                        " or Transmission Mode != 8");
                p_ue_context->physical_config_dedicated.bitmask ^=
                    RRC_PHY_ANTENNA_INFO_V920_PRESENT;
            }
        }

        /*Bug_12240_fix_start*/
        if ((p_ue_context->physical_config_dedicated.bitmask &
                    RRC_PHY_ANTENNA_INFO_R10_PRESENT) &&
                (p_ue_context->physical_config_dedicated.antenna_info_r10.
                 bitmask & RRC_ANTENNA_INFO_R10_EXPLICIT_VALUE_PRESENT))
        {    
            if (RRC_FAILURE ==
                    uecc_fsm_validate_antenna_codebook_subset_restriction_r10(
                        &p_ue_context->physical_config_dedicated.
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
        if (!(p_ue_context->physical_config_dedicated.bitmask &
                    RRC_PHY_CQI_REPORTING_PRESENT))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_INFO,
                    "CQI_REPORTING Config is not present");
            /*Bug_11255_start*/
            if (p_ue_context->physical_config_dedicated.bitmask &
                    RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT)
            {
                p_ue_context->physical_config_dedicated.bitmask ^=
                    RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT;
            }
            /*Bug_11255_stop*/
        }    
        else
        {    
            /*Bug_11255_start*/
            if (p_ue_context->physical_config_dedicated.bitmask &
                    RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT)
                /*Bug_11255_stop*/
            {
                /* Varifying the conditional presence of CQI_MASK_R9 */
                if (p_ue_context->physical_config_dedicated.cqi_report_config_v920.
                        bitmask & RRC_PHY_CQI_MASK_R9_PRESENT)
                {
                    if (p_ue_context->physical_config_dedicated.cqi_reporting.
                            cqi_reporting_periodic.bitmask &
                            RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT)
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,RRC_INFO,
                                "CQI_MASK_R9 :: Field cqi-ReportPeriodic is present"
                                " and set to SETUP");
                    }
                    else
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,RRC_INFO,
                                "[RRC_RRM_ERAB_SETUP_RESP] In PHY Dedicated Info, "
                                " Field cqi-ReportPeriodic is set to RELEASE");
                        p_ue_context->physical_config_dedicated.cqi_report_config_v920.
                            bitmask ^= RRC_PHY_CQI_MASK_R9_PRESENT;
                        if (!(p_ue_context->physical_config_dedicated.
                                    cqi_report_config_v920.bitmask & RRC_PHY_PMI_RI_REPORT_R9_PRESENT))
                        {
                            p_ue_context->physical_config_dedicated.bitmask ^=
                                RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT;
                        }    
                    }
                }
                /* Varifying the conditional presence of pmi-RI-Report-r9 */
                if (p_ue_context->physical_config_dedicated.cqi_report_config_v920.
                        bitmask & RRC_PHY_PMI_RI_REPORT_R9_PRESENT)
                {
                    /*tm7-8 fix*/
                    if ((p_ue_context->physical_config_dedicated.cqi_reporting.
                                cqi_reporting_periodic.bitmask &
                                RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT) ||
                            (p_ue_context->physical_config_dedicated.cqi_reporting.bitmask &
                             RRC_PHY_CQI_REPORTING_MODE_APERIODIC_PRESENT))
                        /*tm7-8 fix*/
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,RRC_INFO,
                                "PMI_RI_REPORT_R9 :: Field cqi-ReportPeriodic is present and" 
                                "set to SETUP and cqi-ReportModeAperiodic is also Present");
                    }
                    else
                    {    
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name,RRC_INFO,
                                "[RRC_RRM_ERAB_SETUP_RESP] In PHY Dedicated Info, "
                                " Field cqi-ReportPeriodic is set to RELEASE"
                                " or APERIODIC is not present");
                        /*p_ue_context->physical_config_dedicated.cqi_report_config_v920.
                          bitmask ^= RRC_PHY_PMI_RI_REPORT_R9_PRESENT; */
                        if (!(p_ue_context->physical_config_dedicated.cqi_report_config_v920.
                                    bitmask & RRC_PHY_CQI_MASK_R9_PRESENT))
                        {
                            p_ue_context->physical_config_dedicated.bitmask ^=
                                RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT;
                        }     
                    }
                }
            }
        }        
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_rrm_resource_admission_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rm_rrm_resource_admission_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send the RRC_RRM_ERB_SETUP_REQ msg to RRM */
    if(RRC_SUCCESS != uecc_rrm_build_and_send_erb_setup_req(p_ue_context))
    {
        /* Process failure */
        uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}




/****************************************************************************
 * Function Name  : uecc_fsm_rm_rrm_erb_setup_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_RRM_ERB_SETUP_RESP from a RRM.
 ****************************************************************************/
void uecc_fsm_rm_rrm_erb_setup_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_rrm_erb_setup_resp_t*  p_rrc_rrm_erb_setup_resp = PNULL;
    rrc_return_t               result = RRC_FAILURE;
    rrm_cause_t                rrm_fail_cause;
    U8                         index = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_rrm_erb_setup_resp = (rrc_rrm_erb_setup_resp_t*)p_api;

    do  {
        /* Check whether RRC_RRM_ERB_SETUP_RESP received with success or failure */
        if(SUCCESS == p_rrc_rrm_erb_setup_resp->response || 
                PARTIAL_SUCCESS == p_rrc_rrm_erb_setup_resp->response)
        {
            if (0 == (RRM_ERB_SETUP_RESP_API_ADM_RADIO_RES_PRESENT
                        & p_rrc_rrm_erb_setup_resp->bitmask))
            {
            /* It should be present in case of success
             * See LTE RRC API Reference Manual RRC_RRM_ERB_SETUP_RESP */
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_ERB_SETUP_RESP] radio_rsource_config absent");
        }
        /*SPR_19066_START*/
        else if(!(RRM_ERB_SETUP_RESP_API_UE_INACTIVE_TIME_PRESENT &
                    p_rrc_rrm_erb_setup_resp->bitmask))
        {
            /* It should be present in case of success for first ERB.
             * See LTE RRC API Reference Manual RRC_RRM_ERB_SETUP_RESP */
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_ERB_SETUP_RESP] UE Inactive Time config is absent");
        }
        else
        {
                /* Store message data in UE context for future processing */
                /* Process RRC_RRM_ERB_SETUP_RESP message data */
                if (RRC_SUCCESS == uecc_fsm_rm_process_rrm_erab_setup_resp_data(
                            p_ue_context,p_rrc_rrm_erb_setup_resp))
                {
                    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,
                            "RRM has allocated radio resources successfully for new ERAB");
#ifdef RRC_UT_FRAMEWORK
                    p_ue_context->uecc_fsm.p_current_state= &uecc_fsm_rm_llim_reconfigure_ue_ongoing;
#endif

#ifdef ENDC_ENABLED
                    /*NR_DC Code Change Start*/
                    /*check if SGNB Addition is required for the UE*/
                    if((RRC_TRUE == p_ue_context->is_sgnb_add_reqd)&&
                            RRC_NULL < 
                            rrc_uecc_count_scg_bearer(p_ue_context))
                    {
                        RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,
                                "SgNB Addition is required. Initiating SgNB Addition Procedure ");

                        /* Store default SgNB peer_enodeb_id in X2AP UE context */
                        for ( index = RRC_NULL; index < MAX_PEER_ENB; index++ )
                        {
                            if ( X2_ENB_COMM_INFO_DEFAULT_SGNB_PRESENT &
                                    p_ue_context->p_gb_context->x2ap_ctx.
                                    x2ap_link_info[index].enb_comm_info.bitmask )
                            {
                                RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,
                                        "[DC_BEARER_CHANGE] Default SgNB with peer_enodeb_id "
                                        "%d is present ",index);

                                p_ue_context->x2ap_context.peer_enodeb_id =
                                    index;
                                break;
                            }
                        }

                        if ( MAX_PEER_ENB == index )
                        {
                            RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,
                                    "No NR serving cell info present in peer eNodeB Context "
                                    "with deafult GNB value received ");
                            result = RRC_FAILURE;
                            break;
                        }

                        UECC_FSM_SET_CHILD_STATE(
                                p_ue_context,
                                &uecc_fsm_rm_sgnb_addition_ongoing,
                                PNULL);
                    }
                    else
                    {
                        /*NR_DC Code Change Stop*/
#endif
                        UECC_FSM_SET_CHILD_STATE(
                                p_ue_context,
                                &uecc_fsm_rm_llim_reconfigure_ue_ongoing,
                                PNULL);
#ifdef ENDC_ENABLED
                        /*NR_DC Code Change Start*/
                    }
                    /*NR_DC Code Change Stop*/
#endif

                    p_ue_context->m.ue_inactive_time_updated = RRC_TRUE;
                    p_ue_context->ue_inactive_time = 
                        p_rrc_rrm_erb_setup_resp->ue_inactive_time_config;
                    result = RRC_SUCCESS;
                }
        }
        /*SPR_19066_END*/
    }
    }while(RRC_NULL);

    if (RRC_SUCCESS != result)
    {

        if (RRM_ERB_SETUP_RESP_API_FAIL_CAUSE_PRESENT &
                p_rrc_rrm_erb_setup_resp->bitmask)
        {

            /* SPR 13468 Fix Start */
            p_ue_context->m.erb_setup_resp_fail_cause_present = RRC_TRUE;	   
            /* SPR 13468 Fix Stop */

            /* Process failure according to selected failure actions list */
            memset_wrapper(&rrm_fail_cause, RRC_NULL, sizeof(rrm_fail_cause));

            rrm_fail_cause = p_rrc_rrm_erb_setup_resp->fail_cause;

            if (RRC_SUCCESS != uecc_s1ap_build_s1ap_cause(
                    &p_ue_context->share_data.p_uecc_rm_data->cause,
                    &rrm_fail_cause,
                    p_ue_context))
            {
                RRC_UECC_RM_FSM_TRACE(RRC_WARNING, "Invalid cause type.");
            }

        }

        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_rm_fail_wait_rrm_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_rrm_resource_admission_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rm_rrm_resource_admission_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    p_ue_context->share_data.p_uecc_rm_data->cause.t = T_s1ap_Cause_radioNetwork;
    p_ue_context->share_data.p_uecc_rm_data->cause.u.radioNetwork = s1ap_release_due_to_eutran_generated_reason;
    
    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_TIMER_EXPIRY);

    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rm_fail_wait_rrm_actions_no_response);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_llim_reconfigure_ue_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rm_llim_reconfigure_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Build and send the UECC_LLIM_ADD_LC_REQ msg to LLIM */
    if (RRC_SUCCESS != uecc_llim_build_and_send_add_lc_req(p_ue_context))
    {
        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_rm_fail_wait_rrm_actions_no_response);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_llim_reconfigure_ue_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_LLIM_RECONFIGURE_UE_RESP from LLIM
 ****************************************************************************/
void uecc_fsm_rm_llim_reconfigure_ue_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_return_t    result = RRC_FAILURE;

    rrc_uecc_llim_add_lc_resp_t*    p_rrc_uecc_llim_add_lc_resp = PNULL;
    uecc_fsm_fail_handler_f const*  p_fail_actions_list =
                                        uecc_fsm_rm_fail_wait_llim_ue_actions;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_uecc_llim_add_lc_resp = (rrc_uecc_llim_add_lc_resp_t*)p_api;

    do
    {
        /* Check for Invalid Response */
        if (L3_FATAL <  p_rrc_uecc_llim_add_lc_resp->response_code)
        {
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                "Invalid response recv from LLIM : %u",
                p_rrc_uecc_llim_add_lc_resp->response_code);
            break;
        }

        result = uecc_fsm_rm_process_llim_add_lc_res(
                 p_ue_context,
                 p_rrc_uecc_llim_add_lc_resp);

        if (SUCCESS == p_rrc_uecc_llim_add_lc_resp->response_code || 
            PARTIAL_SUCCESS ==  p_rrc_uecc_llim_add_lc_resp->
            response_code)
        {
            /*Qos changes START*/
            if (RRC_SUCCESS == 
                uecc_fsm_chk_at_least_one_non_gbr_rab (p_ue_context))
            {
                /*Qos changes END*/
                RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,
                "LLIM has allocated radio resources successfully for new ERAB");
            
                /* Switch state */
                UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_rm_ue_reconfigure_ongoing,
                    PNULL);

                result = RRC_SUCCESS;

                /* Map Response to ERAB Setup List Response */            
                p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                    erab_info_data.outerResponse = 
                    p_rrc_uecc_llim_add_lc_resp->response_code;
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
                        /* TEST FIX */
                        if (p_ue_context->p_sps_config != NULL)
                        {
                            rrc_mem_free(p_ue_context->p_sps_config);
                            p_ue_context->p_sps_config = PNULL;
                            p_ue_context->m.sps_config_present = RRC_NULL;
                        }
                    }
                }
            }
                /* SPS Stop */
            }
            else
            {
                result = RRC_FAILURE;
            }
        }
        else if (FAILURE == p_rrc_uecc_llim_add_lc_resp->response_code)
        {
            RRC_UECC_RM_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RECONFIGURE_UE_RESP] failed [%u]",
                 p_rrc_uecc_llim_add_lc_resp->response_code);
            /* failure at LLIM - LLIM has deleted LCs on LL by itself*/
            p_fail_actions_list =
                uecc_fsm_rm_fail_wait_rrm_actions_no_response;
 
            result = RRC_FAILURE;
  
            /* Map Response to ERAB Setup List Response */            
            p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                erab_info_data.outerResponse = FAILURE;

        }
        /* Response is L3_FATAL ERROR */ 
        else if (L3_FATAL == p_rrc_uecc_llim_add_lc_resp->response_code)
        {
            RRC_UECC_RM_FSM_TRACE(RRC_WARNING,
                "[UECC_EV_LLIM_RECONFIGURE_UE_RESP] L3_FATAL ERROR [%u]",
                 p_rrc_uecc_llim_add_lc_resp->response_code);
            /* failure at LLIM - L3_FATAL ERROR */
            p_fail_actions_list =
                uecc_fsm_rm_fail_wait_llim_unrecov_actions;
            result = RRC_FAILURE;

            /* Map Response to ERAB Setup List Response */            
            p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                erab_info_data.outerResponse = FAILURE;
        }

    if (RRC_SUCCESS != result )
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
            p_ue_context,
            p_fail_actions_list);
    }

    } while (0);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_llim_reconfigure_ue_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rm_llim_reconfigure_ue_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_TIMER_EXPIRY);

    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rm_fail_wait_llim_ue_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_ue_reconfigure_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rm_ue_reconfigure_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* build and send RRCConnectionReconfiguration */
    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_ics_rrc_connection_reconfiguration(p_ue_context))
    {
        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_rm_fail_wait_llim_ue_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_rrc_connection_reconfiguration_complete_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_RRC_CONNECTION_RECONFIGURATION_COMPLETE from UE
 ****************************************************************************/
void uecc_fsm_rm_rrc_connection_reconfiguration_complete_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_return_et rrc_response = RRC_FAILURE;
    rrc_rrm_ue_information_req_t ue_info_req;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Measurment Re-config START */
    /* SPR 13690 Fix Start */
    if ((RRM_RRC_MC_VALIDATE_SUCCESS == 
              p_ue_context->curr_meas_config.is_meas_config_valid) && 
			    (PNULL != p_ue_context->p_meas_config))/* SPR 13690 Fix Stop */
    {
        uecc_update_MeasConfig_in_ics_complete(
            p_ue_context->p_meas_config,
            &p_ue_context->curr_meas_config,
            p_ue_context->p_gb_context);
    }
    /* Measurment Re-config END */

    if (RRC_FAILURE == uecc_is_default_bearer_established(p_ue_context))
    {
        RRC_UECC_RM_FSM_TRACE (RRC_INFO,
                     "Default Non-GBR bearer not established");
    }
    else
    {
        /* build and send RRC_RRM_ERB_SETUP_CNF with success to RRM */
        rrc_response = uecc_rrm_build_and_send_erb_setup_conf(
            p_ue_context);
    }

    if(p_ue_context->rlf_proc_flags.m.rrc_conn_setup_reconfigure_rlf_info_present == 1)
    {
        memset_wrapper(&ue_info_req, 0, sizeof(rrc_rrm_ue_information_req_t));
        ue_info_req.ue_Index   = p_ue_context->ue_index;
        ue_info_req.rach_report_req = RRC_FALSE;
        ue_info_req.rlf_report_req  = RRC_TRUE;
        /* Start timer */
        uecc_fsm_rrm_ue_information_req_start_timer(p_ue_context);

        /* Set UE Information intrl procedure flag in ue context */
        p_ue_context->ue_info_proc_running = RRC_TRUE;

        p_ue_context->rlf_proc_flags.ue_info_proc_setup_reconfig_rlf = RRC_TRUE;


        /* UE INFORMATION REQ */
        if (RRC_SUCCESS != uecc_llim_build_and_send_rrc_ue_information(
                            p_ue_context, &ue_info_req))
        {
            uecc_rrm_ue_information_req_stop_timer(p_ue_context);
        }
    }

    if (RRC_SUCCESS == rrc_response)
    {
#ifdef ENDC_ENABLED
        /*NR_DC Code Change Start*/
        /* Send ASN.1 Encoded SgNBReconfigurationComplete to SgNB if SgNB addition was required */
        uecc_erab_failed_to_setup_list_t*  p_erab_fail_list =
            &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                    failed_erab_data);
        if((RRC_TRUE == p_ue_context->is_sgnb_add_reqd)&&
                (RRC_NULL <
                rrc_uecc_count_scg_bearer(p_ue_context))  )
        {
            if ( RRC_NULL == p_erab_fail_list->count )
            {
                if(RRC_SUCCESS !=
                        uecc_x2ap_build_and_send_sgnb_reconfiguration_complete(p_ue_context))
                {
                    RRC_UECC_RM_FSM_TRACE(RRC_FATAL,
                            "[x2ap_SgNBReconfigurationComplete] delivery failure");
                }
            }
            else
            {
                if(RRC_SUCCESS !=
                        uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(p_ue_context))
                {
                    RRC_UECC_RM_FSM_TRACE(RRC_FATAL,
                            "[x2ap_SgNBReconfigurationComplete] delivery failure");
                }
            }
            /*reset is_sgnb_add_reqd in p_ue_context*/
            p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
        }
        else
        {
            if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd)
            {
                U8 count = 0;
                for ( count = 0; count < p_erab_fail_list->count; count++ )
                {
                    if ( p_erab_fail_list->erab_failed_to_setup_item[count].dc_bearer_type == 1 )
                    {
                        if(RRC_SUCCESS !=
                                uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(p_ue_context))
                        {
                            RRC_UECC_RM_FSM_TRACE(RRC_FATAL,
                                    "[x2ap_SgNBReconfigurationComplete] delivery failure");
                        }
                        /*reset is_sgnb_add_reqd in p_ue_context*/
                        p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
                    }
                }
            }
        }
        /*NR_DC Code Change Stop*/
#endif
        uecc_fsm_finish_activity_success(p_ue_context, PNULL);
    }
    else
    {
        /* Process failure according to selected failure actions list */
        /* The same as failure processing of LLIM error */
        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_rm_fail_wait_llim_ue_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_ue_reconfigure_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rm_ue_reconfigure_ongoing_timer_expiry_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* SPR 14196 Fix Start */
    p_ue_context->share_data.p_uecc_rm_data->cause.t =
            T_s1ap_Cause_radioNetwork;
    p_ue_context->share_data.p_uecc_rm_data->cause.u.radioNetwork =
            s1ap_failure_in_radio_interface_procedure;
    /* SPR 14196 Fix Stop */
    /* Put all E_RAB's in failed list from ERAB Setup List */
    uecc_fsm_rm_fail_action_mark_all_erab_failed(
        p_ue_context,
        RRC_TIMER_EXPIRY);

    uecc_fsm_start_failure_processing(
        p_ue_context,
/*NR_DC Code Change Start*/
        uecc_fsm_rm_fail_timer_expiry_ue_reconfig_actions);
/*NR_DC Code Change Stop*/

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* Failure processing functions */

/****************************************************************************
 * Function Name  : uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*       - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of UECC_RM_FAIL_ACTION_SEND_RRC_RRM_ERB_SETUP_CNF,
 *                  RRC_RRM_ERB_SETUP_CNF fail message to RRM
 ****************************************************************************/
void uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    uecc_rm_erab_setup_info_t    *p_erab_info_list = 
        &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        erab_info_data;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_RM_FSM_TRACE(RRC_INFO,
        "FAILURE: Send RRC_RRM_ERB_SETUP_CNF (Fail)");

    /* Set the ERAB SETUP LIST outerResponse TO FAILURE */
    p_erab_info_list->outerResponse = FAILURE;
    if(RRC_SUCCESS != 
            uecc_rrm_build_and_send_erb_setup_conf(p_ue_context))
    {
        RRC_UECC_RM_FSM_TRACE(RRC_ERROR, 
                "[%s],Unable to send Erab Setup Confirm", __FUNCTION__);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR 18693 Fix Start*/
/*Code Removed*/
/*SPR 18693 Fix Stop*/

/****************************************************************************
 * Function Name  : uecc_fsm_rm_fail_action_delete_drb_info_from_ctx
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of UECC_RM_FAIL_ACTION_DELETE_DRB_INFO_FROM_CTX.
 *                      Delete DRB info from UE context
 ****************************************************************************/
void uecc_fsm_rm_fail_action_delete_drb_info_from_ctx(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    U8 id_count = 0;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_RM_FSM_TRACE(RRC_INFO, "FAILURE: Delete DRB info from UE context");

    /* ICS START */
    for ( id_count = 0; id_count < MAX_ERAB_COUNT; id_count++)
    {
        if (p_ue_context->p_e_rab_list[id_count] != PNULL)
        {
            RRC_UECC_RM_FSM_TRACE(RRC_INFO, 
                "ERAB ID : %u context deleted from UE Context ",
                 p_ue_context->p_e_rab_list[id_count]->e_RAB_ID);
            rrc_mem_free(p_ue_context->p_e_rab_list[id_count]);
            p_ue_context->p_e_rab_list[id_count] = PNULL;
        }
    }
    p_ue_context->e_rab_list_count = 0;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR 18693 Fix Start*/
/*Code Removed*/
/*SPR 18693 Fix Stop*/

/****************************************************************************
 * Function Name  : uecc_fsm_rm_fail_action_last
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of UECC_RM_FAIL_ACTION_LAST
 ****************************************************************************/
void uecc_fsm_rm_fail_action_last(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);
    RRC_UECC_RM_FSM_TRACE(RRC_INFO, "FAILURE: END");

    /* notify procedure about failure */
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_rm_process_rrm_erab_setup_resp_data
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_rrm_erb_setup_resp_t* p_rrm_erb_setup_resp
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of RRC_RRM_ERB_SETUP_RESP message data
 ****************************************************************************/
rrc_return_t uecc_fsm_rm_process_rrm_erab_setup_resp_data(
    uecc_ue_context_t*  p_ue_context,
    rrc_rrm_erb_setup_resp_t* p_rrm_erb_setup_resp
)
{
    rrc_return_t result = RRC_FAILURE;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);

    /* Store UE parameters in UE context and apply default values for them
     * if its needed.*/
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(p_ue_context->p_gb_context,
                p_ue_context->cell_index))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_WARNING,"cell not active");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == store_phyConfigDedinfo_ueContext(p_ue_context, p_rrm_erb_setup_resp))
    { 
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    if (RRC_FAILURE == store_macMeasConfig_ueContext(p_ue_context, p_rrm_erb_setup_resp))
    { 
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    if ((p_ue_context->m.srb2_config_present) &&
        /* SRB2 information received in UE_ADMISSION_RESP */
        /* overwrite the old SRB2 configuration in UE context */
        /* with ERB_SETUP_RESP SRB2 configuration */
        /* Store SRB2 information also */
        (RRM_ERAB_SETUP_RADIO_RES_CONFIG_SRB_INFO_PRESENT &
            p_rrm_erb_setup_resp->radio_rsource_config.bitmask ) &&
        (RRM_SRB_INFO_SRB2_CONFIG_PRESENT &
            p_rrm_erb_setup_resp->radio_rsource_config.srb_info.bitmask)) 
            {
                p_ue_context->srb2_config =
                    p_rrm_erb_setup_resp->radio_rsource_config.srb_info.srb2_config;
                p_ue_context->m.srb2_config_present = 1;
                p_ue_context->m.srb2_config_updated = 1;
                /* Apply default configuration */
                if ((RRM_SRB_CONFIG_AM_CONFIG_PRESENT &
                p_ue_context->srb2_config.bitmask) &&
                (!(RRM_SRB_AM_CONFIG_EXPLICIT_PRESENT
                & p_ue_context->srb2_config.srb_am_config.bitmask)))
                {
                    RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                        "[RRC_RRM_ERB_SETUP_RESP] In SRB AM Config, "
                        "explicit AM config is not present."
                        " Applying default values for it.");
                    p_ue_context->srb2_config.bitmask =
                        (rrc_bitmask_t)(p_ue_context->srb2_config.bitmask &
                         ~RRM_SRB_CONFIG_AM_CONFIG_PRESENT);
                }

                if((RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT &
                p_ue_context->srb2_config.bitmask) &&
                (!(RRM_UL_SPECIFIC_PARAMETERS_EXPLICIT_PRESENT
                & p_ue_context->srb2_config.logical_channel_config.bitmask)))
                {
                    RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                        "[RRC_RRM_ERB_SETUP_RESP] In SRB UL Specific "
                        "parameters, explicit SRB UL specific parameters"
                        " are not present. Applying default values for it.");
                    p_ue_context->srb2_config.bitmask =
                    (rrc_bitmask_t)(p_ue_context->srb2_config.bitmask &
                    ~RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT);
                }

                /* SPR 14984 Fix Start */
                /* Apply default configuration */
                uecc_defaults_apply_srb_config(
                    &p_ue_context->srb2_config, &srb2_config_default, 
                     p_ue_context, RRC_SRB2_LC_ID);
                /* SPR 14984 Fix Stop */
    }
    else if (RRM_ERAB_SETUP_RADIO_RES_CONFIG_SRB_INFO_PRESENT &
    p_rrm_erb_setup_resp->radio_rsource_config.bitmask )
    {
        if ((RRM_SRB_INFO_SRB2_CONFIG_PRESENT &
        p_rrm_erb_setup_resp->radio_rsource_config.srb_info.bitmask))
        {
            p_ue_context->srb2_config =
            p_rrm_erb_setup_resp->radio_rsource_config.srb_info.srb2_config;

            p_ue_context->m.srb2_config_present = 1;
            p_ue_context->m.srb2_config_updated = 1;
            /* Apply default configuration */
            if ((RRM_SRB_CONFIG_AM_CONFIG_PRESENT &
            p_ue_context->srb2_config.bitmask) &&
            (!(RRM_SRB_AM_CONFIG_EXPLICIT_PRESENT
            & p_ue_context->srb2_config.srb_am_config.bitmask)))
            {
                RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_ERB_SETUP_RESP] In SRB AM Config, "
                    "explicit AM config is not present."
                    " Applying default values for it.");
                p_ue_context->srb2_config.bitmask =
                    (rrc_bitmask_t)(p_ue_context->srb2_config.bitmask &
                    ~RRM_SRB_CONFIG_AM_CONFIG_PRESENT);
            }

            if ((RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT &
            p_ue_context->srb2_config.bitmask) &&
            (!(RRM_UL_SPECIFIC_PARAMETERS_EXPLICIT_PRESENT
            & p_ue_context->srb2_config.logical_channel_config.bitmask)))
            {
                RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_ERB_SETUP_RESP] In SRB UL Specific "
                    "parameters, explicit SRB UL specific parameters"
                    " are not present. Applying default values for it.");
                p_ue_context->srb2_config.bitmask =
                    (rrc_bitmask_t)(p_ue_context->srb2_config.bitmask &
                    ~RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT);
            }

            /* SPR 14984 Fix Start */
            /* Apply default configuration */
            uecc_defaults_apply_srb_config(
                &p_ue_context->srb2_config, &srb2_config_default, 
                 p_ue_context, RRC_SRB2_LC_ID);
            /* SPR 14984 Fix Stop */
        }
        else
        {
            /* SRB2 Configuration is received neither in ADMISSION RESP nor in
            * ERB_SETUP_RESP but
            * RRM_ERAB_SETUP_RADIO_RES_CONFIG_SRB_INFO_PRESENT bitmask is set */
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_ERB_SETUP_RESP] srb2_info absent");
            return RRC_FAILURE;
        } 
    }
    else
    {
        /* SRB2 Configuration is not received neither in ADMISSION RESP */
        /* nor in ERB_SETUP_RESP*/
        RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
            "[RRC_RRM_ERB_SETUP_RESP] srb2_info absent");
        return RRC_FAILURE;
    } 

    /*cr_220: Store RLF Timers & Constants from ERB_Setup_Resp data from RRM into UE ctx */
    if (RRM_ERAB_SETUP_RADIO_RES_CONFIG_RLF_TIMERS_AND_CONSTANTS_PRESENT &
            p_rrm_erb_setup_resp->radio_rsource_config.bitmask )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_INFO,"RLF Timers & Constants present");
        p_ue_context->m.rlf_timers_and_constants_r9_present = 1;
 
        if( RLF_TIMERS_AND_CONSTANTS_SETUP_PRESENT & p_rrm_erb_setup_resp->radio_rsource_config.rlf_timers_and_constants_r9.bitmask)
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
                p_rrm_erb_setup_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.t301_r9;

            p_ue_context->p_rlf_timers_and_constants_r9->setup.t310_r9 =
                p_rrm_erb_setup_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.t310_r9;

            p_ue_context->p_rlf_timers_and_constants_r9->setup.n310_r9 =
                p_rrm_erb_setup_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.n310_r9;

            p_ue_context->p_rlf_timers_and_constants_r9->setup.t311_r9 =
                p_rrm_erb_setup_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.t311_r9;

            p_ue_context->p_rlf_timers_and_constants_r9->setup.n311_r9 =
                p_rrm_erb_setup_resp->radio_rsource_config.rlf_timers_and_constants_r9.setup.n311_r9;
          }
        }
    }

    /* Copy Proximity Configuration */
    if (p_rrm_erb_setup_resp->radio_rsource_config.bitmask &
            RRM_ERAB_SETUP_RADIO_RES_CONFIG_PROXIMITY_PRESENT )
    {
         p_ue_context->m.proximity_config_present = 1;
        l3_memcpy_wrapper(&(p_ue_context->proximity_config),
                &(p_rrm_erb_setup_resp->radio_rsource_config.proximity_config),
                sizeof(rrm_report_proximity_config_r9_t));

    }

    /* Bug 3961 Fix Start */
    if(RRM_ERAB_SETUP_RADIO_RES_CONFIG_MEAS_SUBFRAME_PATTERN_PCELL_R10_PRESENT & 
            p_rrm_erb_setup_resp->radio_rsource_config.bitmask)
    {
        p_ue_context->m.meas_subframe_pattern_pcell_present = 1;
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_DETAILED," Copying MEAS Subframe Pattern PCell to UECC UE Context ");

        /*SPR 21302 Fix Start*/
        /*if condition removed for handling both setup and release*/
        /*SPR 21302 Fix End*/
        p_ue_context->rrc_meas_subframe_pattern_pcell_r10 = p_rrm_erb_setup_resp->radio_rsource_config.rrc_meas_subframe_pattern_pcell_r10;
    }
    /* Bug 3961 Fix End */

    /* ICS START */
    /* Process Radio Resource Config Dedicated if response is not FAILURE */
    if (p_rrm_erb_setup_resp->response != FAILURE)
    {
        result = uecc_fsm_rm_process_rrm_erab_radio_res_dedicated(
            p_ue_context,
            p_rrm_erb_setup_resp);
    }

    /* ICS STOP */

#ifdef ENDC_ENABLED
    /*NR_DC Code Change Start*/
    if ( RRC_SUCCESS == result )
    {
        /*check if the rab will be admitted on the MeNB or the SgNB UE
         * Addition is required for this*/
        //todo list chng (review comment)
        if ( RRM_ERB_SETUP_RESP_API_SGNB_ADD_PARAM_PRESENT &
            p_rrm_erb_setup_resp->bitmask )
        {
            if( RRC_SUCCESS ==
                    uecc_fsm_chk_if_sgnb_addition_reqd(
                        p_ue_context,
                        &p_rrm_erb_setup_resp->radio_rsource_config.
                        drb_to_add_list))
            {
                /*Store UE Aggregated Max Bit Rate for DL and UL*/
                p_ue_context->ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_ul =
                    p_rrm_erb_setup_resp->sgnb_add_param.ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_ul;
                p_ue_context->ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_dl =
                    p_rrm_erb_setup_resp->sgnb_add_param.ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_dl;

                    l3_memcpy_wrapper(&(p_ue_context->p_curr_proc_data->
                                u.erab_setup_request_data.uecc_cg_config_info),
                            &(p_rrm_erb_setup_resp->sgnb_add_param.cg_config_info),
                            sizeof(cg_config_info_t));
            }
            else
            {
                result = RRC_FAILURE;
            }
        }
    }
    /*NR_DC Code Change Stop*/
#endif
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}



/* ICS START */

/****************************************************************************
 * Function Name  : uecc_fsm_rm_fail_action_mark_all_erab_failed
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  U32                 error_code
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion populate failed list from success list
 ****************************************************************************/
void uecc_fsm_rm_fail_action_mark_all_erab_failed(
    uecc_ue_context_t*  p_ue_context,
    U32                 error_code
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

    fail_list_count = p_erab_fail_list->count;

    /* ADD RRC Module ID in error code */
    RRC_UECC_ERB_SETUP_UPDATE_ERROR_CODE(error_code);

    for (succ_list_counter = 0; 
         succ_list_counter < succ_count; 
         succ_list_counter++)
    {
        if (RRC_NO_ERROR == p_erab_setup_list->erab_info[succ_list_counter].
            status)
        {
            /* Set status to error_code in E_RAB Setup List */
            p_erab_setup_list->erab_info[succ_list_counter].status = 
                error_code;

            /* Copy E_RAB ID to Failed List */
            p_erab_fail_list->erab_failed_to_setup_item[fail_list_count]
                .e_RAB_ID = p_erab_setup_list->erab_info[succ_list_counter].
                                erab_id;

            /* Map Cause in Fail List */                             
            uecc_llim_build_s1ap_cause(
                p_ue_context->p_gb_context, 
                &p_erab_fail_list->erab_failed_to_setup_item
                [fail_list_count].cause,
                (rrc_response_t)error_code);

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

/******************************************************************************
 *   FUNCTION NAME: uecc_fill_response_code
 *   INPUT        : U32      *p_response_code
 *                  U16       module_id
 *                  U16       response
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function fills the response code failed at RRC and to be send to
 *                  RRM
 * ******************************************************************************/
void uecc_fill_response_code
(
  U32      *p_response_code,
  U16       module_id,
  U16       response
)
{
    *p_response_code = RRC_NULL;
    *p_response_code |= (U32)(module_id << 16);
    *p_response_code |= response ;
}

/* qos changes start*/
/****************************************************************************
 * Function Name  : uecc_fsm_chk_at_least_one_non_gbr_rab
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion check atleast 1 NON GBR bearer is present in 
 *                  erab_setup_resp coming from RRM else add all Rabs to 
 *                  failed list. Its valid during ICS only.
 ****************************************************************************/
static rrc_return_t uecc_fsm_chk_at_least_one_non_gbr_rab(
    uecc_ue_context_t*  p_ue_context    
)
{
    rrc_return_t result = RRC_FAILURE;
    U8 drb_index = 0;
    U8 rab_id = 0;
    U8 fail_list_count = 0;
    rrm_cause_t   cause ;
    uecc_rm_erab_setup_info_t    *p_erab_info_list = PNULL; 
    uecc_erab_failed_to_setup_list_t*  p_failed_erab_list = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if ((p_ue_context->p_curr_proc_data->t != UECC_ICS_PROC) || 
        (!(strncmp_wrapper((const S8 *)
          (p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name),
          (const S8 *)"UECC_RRM_ERB_SETUP_RESOURCE_ADMISSION_ONGOING",
          strlen_wrapper((const Char8*)
          p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name)))))
    {
       return RRC_SUCCESS;        
    }
    /*The sucesslist of DRBs updated after receiving eRab_setup_resp from RRM*/
    p_erab_info_list = 
        &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        erab_info_data);

    p_failed_erab_list = 
        &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        failed_erab_data);

    for (drb_index = 0; drb_index < p_erab_info_list->count; drb_index++)
    {
        rab_id = p_erab_info_list->erab_info[drb_index].erab_id;
        if ((p_ue_context->p_e_rab_list[rab_id] != PNULL) && 
            (rab_id == p_ue_context->p_e_rab_list[rab_id]->e_RAB_ID) &&
            (BEARER_TYPE_NON_GBR == 
             p_ue_context->p_e_rab_list[rab_id]->drb_config.bearer_type))
        {
            result = RRC_SUCCESS;
            break;
        }
    }

    if ((RRC_FAILURE == result) &&
        ( !(strncmp_wrapper((const S8 *)
          (p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name),
          (const S8 *)("UECC_LLIM_RECONFIGURE_UE_ONGOING"),
          strlen_wrapper((const Char8*)
          p_ue_context->uecc_fsm.proc_descriptor.p_current_state->s_name)))))
    {
        RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                "No non gbr bearer is present during ICS so failing the ICS");
        memset_wrapper (&cause, 0, sizeof(cause));
        fail_list_count = p_failed_erab_list->count;

        for (drb_index = 0; drb_index < p_erab_info_list->count; drb_index++)
        {
            if (RRC_NO_ERROR == p_erab_info_list->erab_info[drb_index].status)
            {
                rab_id = p_erab_info_list->erab_info[drb_index].erab_id;
                uecc_fill_response_code (
                        &p_erab_info_list->erab_info[drb_index].status,
                        RRC_MODULE_ID, RRC_UECC_NON_GBR_RAB_NOT_PRESENT);

                /* Put Fail Erab ID recv from RRM in Fail List */
                p_failed_erab_list->erab_failed_to_setup_item[fail_list_count].
                        e_RAB_ID = rab_id;

                /* Increment Fail List Count */
                fail_list_count++;

                /* Deallocate Failed E_RAB From UE Context */
                uecc_fsm_rm_erab_setup_deallocate_erab (p_ue_context, rab_id);
            }
        }
        p_failed_erab_list->count = fail_list_count;
    }
    return result;
}

/****************************************************************************
 * Function Name  : uecc_fsm_fetch_failed_drb_from_sucess_list
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrm_drb_to_add_info_list_t*   - Success DRB's
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion check bearer_type for all the success drb, 
 *                  if bearer type coming from RRM is not matching with qos 
 *                  parameter coming from MME,then add to failed list
 ****************************************************************************/
static rrc_return_t uecc_fsm_fetch_failed_drb_from_sucess_list(
    uecc_ue_context_t*  p_ue_context,    
    rrm_drb_to_add_info_list_t*  p_drb_to_add_list
)
{
    U8 drb_index = 0;
    U8 erb_id_rrm = 0;
    U8 fail_list_count = 0;
    rrm_cause_t   cause ;
    uecc_erab_failed_to_setup_list_t*  p_failed_erab_list = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_drb_to_add_list); 
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_failed_erab_list = 
        &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        failed_erab_data);
    
    memset_wrapper (&cause, 0, sizeof(cause));
    fail_list_count = p_failed_erab_list->count;

    /* Copy DRB Config recv from RRM in UE Context */
    for ( drb_index = 0; 
          ((drb_index < p_drb_to_add_list->drb_count) && 
          (drb_index < MAX_LC_COUNT));
          drb_index++ )
    {
        /* Qos changes start*/
        /*Fetch the eRAB_ID from received drb_config*/
        erb_id_rrm = p_drb_to_add_list->drb_config[drb_index].erab_id;

        /* Check if E_RAB ID present in UE Context */
        if (p_ue_context->p_e_rab_list[erb_id_rrm] == PNULL)
        {
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_ERB_SETUP_RESP] Unknown E_RAB ID Received : %u", 
                 p_drb_to_add_list->drb_config[drb_index].erab_id);
            return RRC_FAILURE;
        } 

        /*If the eRAB_ID matches then validate the bearer_type received from RRM
         * with qos parameters for eRAB_ID received from MME in ICS i.e stored 
         * in ue_context*/
        
        if (erb_id_rrm == p_ue_context->p_e_rab_list[erb_id_rrm]->e_RAB_ID)
        {
            /* If bearer_type received from RRM is GBR and the GBR_QOS_INFO
             * is not received from MME (in ICS) for that eRAB_ID then add the 
             * eRAB to failed list and raise warning */
            if ((BEARER_TYPE_GBR == p_drb_to_add_list->drb_config[drb_index].
                 bearer_type) &&
                !(p_ue_context->p_e_rab_list[erb_id_rrm]->e_RABlevelQoSParameters.
                 m.gbrQosInformationPresent))
            {
                cause.value = s1ap_invalid_qos_combination;
                cause.type = T_ranap_Cause_radioNetwork; 
                
                /* Put Fail Erab ID recv from RRM in Fail List */
                p_failed_erab_list->erab_failed_to_setup_item[fail_list_count].
                    e_RAB_ID = erb_id_rrm;

                /* Updating the qos_status for failed RAB*/
                p_failed_erab_list->erab_failed_to_setup_item[fail_list_count].
                    qos_status = BEARER_TYPE_QOS_FAILURE;

                /* MAP CAUSE VALUE HERE AND STORE IN FAILED LIST */
                if ( RRC_SUCCESS != uecc_s1ap_build_s1ap_cause(
                    &p_failed_erab_list->erab_failed_to_setup_item
                    [fail_list_count].cause, &cause,
                    p_ue_context))
                {
                    return RRC_FAILURE;
                }
       
                /* Increment Fail List Count */
                fail_list_count++;

                /* Deallocate Failed E_RAB From UE Context */
                uecc_fsm_rm_erab_setup_deallocate_erab (p_ue_context, 
                                                        erb_id_rrm);
            }
        }
    }
    /* SPR:9975 start */
    /*spr:8314 start*/
    /* to check if all the success drb got failed or not */    
    if((fail_list_count - p_failed_erab_list->count) == p_drb_to_add_list->drb_count)
    {
        RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_ERB_SETUP_RESP]RAB requested has failed"); 
        /* Update Fail List Counter */
        p_failed_erab_list->count = fail_list_count;
        return RRC_FAILURE;
    } 
    /*spr:8314 stop*/
    /* Update Fail List Counter */
    p_failed_erab_list->count = fail_list_count;
    /* SPR:9975 stop */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}
/* qos changes end*/

/****************************************************************************
 * Function Name  : uecc_fsm_rm_read_all_success_drb
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrm_drb_to_add_info_list_t*   - Success DRB's
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion read success DRB list and put in share data
 ****************************************************************************/
rrc_return_t uecc_fsm_rm_read_all_success_drb(
    uecc_ue_context_t*  p_ue_context,    
    rrm_drb_to_add_info_list_t*  p_drb_to_add_list
)
{
    U8 drb_index = 0;
    U8 erb_id_rrm = 0;
    U8 sucess_list_count = 0;
    uecc_rm_erab_setup_info_t    *p_erab_info_list = PNULL; 
    /*SPR 3438 start*/
    U8 count = 0;
    U8 temp_idx = 0;
    uecc_erab_failed_to_setup_list_t  failed_rab_list;
    /*SPR 3438 end*/
    /* SPR 17022 Fix Start */
    U8 index = 0;
    rrc_temp_erb_info_t	*p_temp_erab_info = PNULL;
    /* SPR 17022 Fix Stop */
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_drb_to_add_list); 

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    memset_wrapper (&failed_rab_list, RRC_NULL, sizeof(failed_rab_list));
    p_erab_info_list = 
        &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        erab_info_data);
	
    /* SPR 17022 Fix Start */
    p_temp_erab_info = &p_ue_context->erab_info_temp;
    /* SPR 17022 Fix Stop */

    /* SPR 17022 Fix Start */
    for ( index = 0 ; index < p_temp_erab_info->count; index++)
    {
        /* Copy DRB Config recv from RRM in UE Context */
        /* Coverity_Fix 95163 ++ */
        for ( drb_index = 0; 
                drb_index < p_drb_to_add_list->drb_count && 
                drb_index < MAX_LC_COUNT;
                drb_index++)
        {
            if (p_temp_erab_info->erab_info[index].erab_id == 
                    p_drb_to_add_list->drb_config[drb_index].erab_id)
            {

                /* Coverity_Fix 95163 -- */ 
                /* Qos changes start*/
                /*Fetch the eRAB_ID from received drb_config*/
                erb_id_rrm = p_drb_to_add_list->drb_config[drb_index].erab_id;

                /* Check if E_RAB ID present in UE Context */
                if (p_ue_context->p_e_rab_list[erb_id_rrm] == PNULL)
                {
                    RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                            "[RRC_RRM_ERB_SETUP_RESP] Unknown E_RAB ID Received : %u", 
                            p_drb_to_add_list->drb_config[drb_index].erab_id);
                    return RRC_FAILURE;
                } 
                /*If the eRAB_ID matches then validate the bearer_type received from RRM
                 * with qos parameters for eRAB_ID received from MME in ICS i.e stored 
                 * in ue_context*/

                if (erb_id_rrm == p_ue_context->p_e_rab_list[erb_id_rrm]->e_RAB_ID)
                {
                    p_temp_erab_info->erab_info[index].drb_to_add_item_flag = RRC_TRUE;

                    /* If bearer_type received from RRM is GBR then check that
                     * GBR_QOS_INFO(Received in ICS from MME)should present in ue_context
                     * Else if beerer_type received from RRM is Non-GBR then ignore the
                     * GBR_QOS_INFO check*/
                    if ((BEARER_TYPE_NON_GBR == p_drb_to_add_list->drb_config[drb_index].
                                bearer_type) ||
                            ((BEARER_TYPE_GBR == p_drb_to_add_list->drb_config[drb_index].
                              bearer_type) &&
                             (p_ue_context->p_e_rab_list[erb_id_rrm]->e_RABlevelQoSParameters.
                              m.gbrQosInformationPresent)))
                    {
                        /* Qos changes end*/
                        /* Copy E_RAB ID in Success List */
                        p_erab_info_list->erab_info[index].erab_id = 
                            p_drb_to_add_list->drb_config[drb_index].erab_id;

                        /* Copy LC_ID in Success List */
                        p_erab_info_list->erab_info[index].lc_id = 
                            p_drb_to_add_list->drb_config[drb_index].
                            logical_channel_identity;        

                        /* Copy drb_config recv from RRM in ERAB List in UE Context */
                        p_ue_context->p_e_rab_list[erb_id_rrm]->drb_config = 
                            p_drb_to_add_list->drb_config[drb_index];
                        /*Increase the sucessful rabs counter*/
                        sucess_list_count ++;
                        /* Qos changes start*/
                    }
                    /*SPR 3438 start*/
                    else
                    {
                        failed_rab_list.erab_failed_to_setup_item[count].e_RAB_ID = 
                            p_drb_to_add_list->drb_config[drb_index].erab_id;
                        failed_rab_list.count ++;

                    }
                    /*SPR 3438 end*/
                }
            }
            /* Qos changes end*/
        }
    }

    /* SPR 17022 Fix Stop */
    /*SPR 3438 start*/
    /*Check whether any E-RAB has been discarded after receving the
     * E_RABSetupresp from RRM in comparission to the E_RAB list received in
     * S1AP_E_RABSetupReq and update the current proc data accordingly*/
    if (p_erab_info_list->count != sucess_list_count)
    {
        for (count = 0; count < failed_rab_list.count; count++)
        {
            for (drb_index = 0; drb_index < p_erab_info_list->count; drb_index++)
            {
                if (failed_rab_list.erab_failed_to_setup_item[count].e_RAB_ID ==
                    p_erab_info_list->erab_info[drb_index].erab_id)
                {
                    for (temp_idx = drb_index; 
                         (temp_idx + 1) < p_erab_info_list->count && 
                         (temp_idx + 1)< MAX_ERAB_COUNT; 
                         temp_idx++)
                    {
                        /*Copy the next element data to the current */
                        p_erab_info_list->erab_info[temp_idx].erab_id = 
                           p_erab_info_list->erab_info[temp_idx + 1].erab_id;
                        p_erab_info_list->erab_info[temp_idx].lc_id = 
                           p_erab_info_list->erab_info[temp_idx + 1].lc_id;
                        p_erab_info_list->erab_info[temp_idx].status = 
                          p_erab_info_list->erab_info[temp_idx + 1].status;
                    }
                }
            }
        }
    }
    /*SPR 3438 end*/
    /* SPR 17022 Fix Start */
    for ( index = 0; index < p_temp_erab_info->count; index++)
    {
	if (RRC_FALSE == p_temp_erab_info->erab_info[index].drb_to_add_item_flag)
	{
            for (drb_index = 0; drb_index < p_erab_info_list->count; drb_index++)
            {
                if (p_temp_erab_info->erab_info[index].erab_id ==
                    p_erab_info_list->erab_info[drb_index].erab_id)
                {
                    for (temp_idx = drb_index; 
                         (temp_idx + 1) < p_erab_info_list->count && 
                         (temp_idx + 1)< MAX_ERAB_COUNT; 
                         temp_idx++)
                    {
                        /*Copy the next element data to the current */
                        p_erab_info_list->erab_info[temp_idx].erab_id = 
                           p_erab_info_list->erab_info[temp_idx + 1].erab_id;
                        p_erab_info_list->erab_info[temp_idx].lc_id = 
                           p_erab_info_list->erab_info[temp_idx + 1].lc_id;
                        p_erab_info_list->erab_info[temp_idx].status = 
                          p_erab_info_list->erab_info[temp_idx + 1].status;
                    }
		}
            }
        }
    } 
    /* SPR 17022 Fix Stop */
    
    /* update the sucessful eRAB list count in the current proc*/
    p_erab_info_list->count = sucess_list_count;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_read_all_failed_drb
 * Inputs         : uecc_ue_context_t* -  p_ue_context
 *                  rrm_drb_failed_to_add_info_list_t* - Failed DRB's
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion read failed DRB list and put in share data
 ****************************************************************************/
void uecc_fsm_rm_read_all_failed_drb(
    uecc_ue_context_t*  p_ue_context,    
    rrm_drb_failed_to_add_info_list_t*  p_drb_failed_to_add_list
)
{
    U8 fail_drb_index  = 0;
    U8 fail_list_count = 0;
    uecc_erab_failed_to_setup_list_t*  p_failed_erab_list = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_drb_failed_to_add_list);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
          
    p_failed_erab_list = 
        &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        failed_erab_data);

    fail_list_count = p_failed_erab_list->count;

    /* Append Failed E_RAB ID's recv from RRM to Failed List */
    for ( fail_drb_index = 0; 
          fail_drb_index < p_drb_failed_to_add_list->count && 
          fail_drb_index < MAX_ERAB_COUNT;
          fail_drb_index++ )
    {
                
        /* Put Fail Erab ID recv from RRM in Fail List */
        p_failed_erab_list->erab_failed_to_setup_item[fail_list_count].
            e_RAB_ID = p_drb_failed_to_add_list->drb_failed_to_add
                       [fail_drb_index].erab_id;

        /* MAP CAUSE VALUE HERE AND STORE IN FAILED LIST */
        if (RRC_SUCCESS != uecc_s1ap_build_s1ap_cause(
            &p_failed_erab_list->erab_failed_to_setup_item[fail_list_count].
            cause,
            &p_drb_failed_to_add_list->drb_failed_to_add[fail_drb_index].cause,
            p_ue_context))
        {
            RRC_UECC_RM_FSM_TRACE(RRC_WARNING, "Invalid cause type.");
        }
       
        /* Increment Fail List Count */
        fail_list_count++;

        /* Deallocate Failed E_RAB From UE Context */
        uecc_fsm_rm_erab_setup_deallocate_erab(
            p_ue_context,           
            p_drb_failed_to_add_list->drb_failed_to_add
                       [fail_drb_index].erab_id);
    }

    /* Update Fail List Counter */
    p_failed_erab_list->count = fail_list_count;
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_ue_erab_setup_deallocate_erab
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  U8 erab_id - E_RAB ID
 * Outputs        : None
 * Returns        : None
 * Description    : This dellocates E_RAB from UE Context
 ****************************************************************************/
void uecc_fsm_rm_erab_setup_deallocate_erab(
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
        RRC_UECC_RM_FSM_TRACE(RRC_INFO,
            "E_RAB ID : %u context deleted from UE Context", erab_id);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

#ifdef ENDC_ENABLED
/****************************************************************************
 * Function Name  : uecc_fsm_rm_fail_action_last_endc
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of UECC_RM_FAIL_ACTION_LAST
 ****************************************************************************/
/*coverity_<278332>_fix_start*/
#if 0
void uecc_fsm_rm_fail_action_last_endc
(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    uecc_fsm_finish_activity_success(
        p_ue_context, (void*)UECC_EV_S1AP_INITIAL_CTX_SETUP_SUCCESS_INT);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
#endif
/*coverity_<278332>_fix_end*/
/****************************************************************************
 * Function Name  : uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf_endc
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*       - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Handling of UECC_RM_FAIL_ACTION_SEND_RRC_RRM_ERB_SETUP_CNF,
 *                  RRC_RRM_ERB_SETUP_CNF partial success message to RRM
 ****************************************************************************/
/*coverity_<278331>_fix_start*/
#if 0
void uecc_fsm_rm_fail_action_send_rrm_erb_setup_cnf_endc
(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_RM_FSM_TRACE(RRC_INFO,
        "Send RRC_RRM_ERB_SETUP_CNF (partial success)");

    /* Set the ERAB SETUP LIST outerResponse TO FAILURE */
    if(RRC_SUCCESS != 
            uecc_rrm_build_and_send_erb_setup_conf(p_ue_context))
    {
        RRC_UECC_RM_FSM_TRACE(RRC_ERROR, 
                "[%s],Unable to send Erab Setup Confirm", __FUNCTION__);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
#endif
/*coverity_<278331>_fix_end*/
/*NR_DC Code Change Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_chk_if_sgnb_addition_reqd
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrm_drb_to_add_info_list_t*   - DRB to be added list
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion checks if any of the DRB is 
 *                  SN_TERMINATED_SCG_BEARER, if yes the same is updated in the 
 *                  ue context
 ****************************************************************************/
rrc_return_t uecc_fsm_chk_if_sgnb_addition_reqd(
        uecc_ue_context_t*  p_ue_context,
        rrm_drb_to_add_info_list_t*  p_drb_to_add_list
        )
{
    U8              drb_index   = 0;
    U8              erab_id     = 0;
    rrc_bool_et     is_scg_bearer_already_present = RRC_FALSE;
    rrc_return_et   result = RRC_FAILURE;

    if ( RRC_TRUE == p_ue_context->is_sgnb_add_reqd )
    {
        RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_ERB_SETUP_RESP] SCG bearer already exists "
                "for this Ue [%d]",
                p_ue_context->ue_index);
        return RRC_FAILURE;
    }

    /*check if the drb_type is SN_TERMINATED_SCG_BEARER, if yes update 
     * the same in ue context*/
    for (drb_index = 0;
            drb_index < p_drb_to_add_list->drb_count &&
            drb_index < MAX_LC_COUNT;
            drb_index++)
    {
        if ((RRM_DRB_CONFIG_DC_BEARER_TYPE_PRESENT & 
                    p_drb_to_add_list->drb_config[drb_index].bitmask) &&
                (SN_TERMINATED_SCG_BEARER == 
                 p_drb_to_add_list->drb_config[drb_index].dc_bearer_type))
        {
            /*Extract the erab_id of the SN_TERMINATED_SCG_BEARER
             * from received drb_config from RRM*/
            erab_id = p_drb_to_add_list->drb_config[drb_index].erab_id;

            /*Check if the erab_id is present in the UE Context*/
            if (PNULL == p_ue_context->p_e_rab_list[erab_id])
            {
                RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                        "[RRC_RRM_ERB_SETUP_RESP] Unknown E_RAB ID Received : [%u]",
                        erab_id);
                return RRC_FAILURE;
            }

            /*validate if the erab_id recieved matches from the erab_id stored
             * in the UE Context*/
            if (erab_id != p_ue_context->p_e_rab_list[erab_id]->e_RAB_ID)
            {
                RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                        "[RRC_RRM_ERB_SETUP_RESP] Invalid E_RAB ID Rcvd: [%u]"
                        "Ue Context Erab Id: [%u]",
                        erab_id,
                        p_ue_context->p_e_rab_list[erab_id]->e_RAB_ID);
                return RRC_FAILURE;
            }

            /*Set the dc_bearer_type for this erab_id in UE Context*/
            p_ue_context->p_e_rab_list[erab_id]->m.DC_BearerTypePresent = RRC_TRUE;

            p_ue_context->p_e_rab_list[erab_id]->dc_bearer_type = 
                p_drb_to_add_list->drb_config[drb_index].dc_bearer_type;

            /*check if scg was already present, then do not update is_sgnb_add_reqd
             * is ue context*/
            if(RRC_FALSE == is_scg_bearer_already_present)
            {
                p_ue_context->is_sgnb_add_reqd = RRC_TRUE;
                is_scg_bearer_already_present = RRC_TRUE;
                result = RRC_SUCCESS;
            }
        }
    }
    return result;
}
/*NR_DC Code Change Stop*/
#endif
/****************************************************************************
 * Function Name  : uecc_fsm_rm_process_rrm_erab_radio_res_dedicated
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_rrm_erb_setup_resp_t* - Pointer to ERB SETUP RESP from 
 *                  RRM
 * Outputs        : None 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This Function process radio resource config dedicated 
 *                  received from RRM in RRC_RRM_ERB_SETUP_RESP
 ****************************************************************************/
rrc_return_t uecc_fsm_rm_process_rrm_erab_radio_res_dedicated(
    uecc_ue_context_t*  p_ue_context,
    rrc_rrm_erb_setup_resp_t*   p_rrc_rrm_erb_setup_resp   
)
{
    rrc_return_t result = RRC_FAILURE;
    uecc_rm_erab_setup_info_t    *p_erab_setup_list = 
        &p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
        erab_info_data;


    RRC_ASSERT(PNULL != p_ue_context);     
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
   
    do
    {

        if ((p_rrc_rrm_erb_setup_resp->radio_rsource_config.drb_to_add_list.
             drb_count +
             p_rrc_rrm_erb_setup_resp->radio_rsource_config.drb_failed_to_add_list.
             count) != p_erab_setup_list->count)
        {
            RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,
                "No of ERAB counts mismatched in response recevied from RRM");
        }
        /* Check whether RRC_RRM_ERB_SETUP_RESP received with success or failure */
        else if (SUCCESS == p_rrc_rrm_erb_setup_resp->response)
        {
            /* Process RRC_RRM_ERB_SETUP_RESP message data */
            /* Read Successful DRB's */
            if (RRC_SUCCESS == uecc_fsm_rm_read_all_success_drb(
                               p_ue_context,    
                               &p_rrc_rrm_erb_setup_resp->radio_rsource_config.
                               drb_to_add_list))
            {   
                RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,
                 "RRM has allocated radio resources successfully for new ERAB");

            /*Qos changes SRART*/
                /*Check that at least 1 Non-GBR bearer is present during ICS*/
                if (RRC_SUCCESS == 
                    uecc_fsm_chk_at_least_one_non_gbr_rab (p_ue_context))
                {
                    /* Check bearer_type for all the success drb, if bearer type
                     * coming from RRM is not matching with qos parameter coming
                     * from MME,then add to failed list*/
                    if (RRC_SUCCESS == uecc_fsm_fetch_failed_drb_from_sucess_list(
                                    p_ue_context, 
                                    &p_rrc_rrm_erb_setup_resp->radio_rsource_config.
                                    drb_to_add_list))
                    {
                        result = RRC_SUCCESS;
                    }
                }
                /*Qos changes END*/

            }   
        }
        else if (PARTIAL_SUCCESS == p_rrc_rrm_erb_setup_resp->response )
        {
            if ((p_rrc_rrm_erb_setup_resp->radio_rsource_config.bitmask &
                RRM_ERAB_SETUP_RADIO_RES_CONFIG_DRB_TO_ADD_LIST_PRESENT) && 
                (p_rrc_rrm_erb_setup_resp->radio_rsource_config.bitmask &
                RRM_ERAB_SETUP_RADIO_RES_CONFIG_DRB_FAILED_TO_ADD_LIST_PRESENT))
            {
                /* Process RRC_RRM_ERB_SETUP_RESP message data */
                /* Read Successful DRB's */ 
                if (RRC_SUCCESS == uecc_fsm_rm_read_all_success_drb(
                                   p_ue_context,    
                                   &p_rrc_rrm_erb_setup_resp->radio_rsource_config.
                                   drb_to_add_list))
                {   
                    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,
                    "RRM has allocated radio resources successfully for new ERAB");
                }
                /* Read Failed E_RAB's */ 
                uecc_fsm_rm_read_all_failed_drb(
                    p_ue_context,    
                    &p_rrc_rrm_erb_setup_resp->radio_rsource_config.
                    drb_failed_to_add_list);

                /*Qos changes SRART*/
                /*Check that at least 1 Non-GBR bearer is present during ICS*/
                if (RRC_SUCCESS == 
                    uecc_fsm_chk_at_least_one_non_gbr_rab (p_ue_context))
                {
                    /* Check bearer_type for all the success drb, if bearer type
                     * coming from RRM is not matching with qos parameter coming
                     * from MME,then add to failed list*/
                    result = uecc_fsm_fetch_failed_drb_from_sucess_list(
                             p_ue_context, 
                             &p_rrc_rrm_erb_setup_resp->radio_rsource_config.
                             drb_to_add_list);
                    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,
                    "RRM has failed to allocat radio resources for some ERAB's");
                }
                else
                {
                    result = RRC_FAILURE;
                }
                /*Qos changes END*/
           }
           else
           {
                /* Error : Parameters are missing */
                RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,
                    "ADD_LIST or FAILED_TO_ADD List are missing");
                break;
            } 
        }
        else if (FAILURE == p_rrc_rrm_erb_setup_resp->response)
        {
            /* Read Failed E_RAB's */ 
            uecc_fsm_rm_read_all_failed_drb(
                p_ue_context,    
                &p_rrc_rrm_erb_setup_resp->radio_rsource_config.
                drb_failed_to_add_list);
            
            /* SPR 13448 Fix Start */
            result = RRC_SUCCESS;
            /* SPR 13448 Fix Stop */
        }
        else
        {
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
             "Wrong response code from RRM in RRC_RRM_ERB_SETUP_RESP : %d",
              p_rrc_rrm_erb_setup_resp->response);
        } 
    }while(0);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);    

    return result;
}
/****************************************************************************
 * Function Name  : uecc_fsm_rm_process_llim_add_lc_res
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_uecc_llim_add_lc_resp_t* - Pointer to ADD LC RESP from 
 *                  LLIM
 * Outputs        : None 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This Function process ADD LC RESP from LLIM
 ****************************************************************************/
rrc_return_t uecc_fsm_rm_process_llim_add_lc_res(
    uecc_ue_context_t*  p_ue_context,
    rrc_uecc_llim_add_lc_resp_t*    p_rrc_uecc_llim_add_lc_resp
)
{
    U8 fail_list_count = 0;
    U8 lc_list_count = 0;
    U8 erab_id = 0;    
    U8 lc_counter = 0;
    U8 erab_count = 0;
    U8 drb_index = 0;
    rrc_return_t result = RRC_SUCCESS;
    /* SPR_20915 - Fix - Starts */
    U8 lc_matched = RRC_FALSE;
    /* SPR_20915 - Fix - Ends */
    uecc_rm_erab_setup_info_t*        p_erab_setup_list = &p_ue_context->
        p_curr_proc_data->u.erab_setup_request_data.erab_info_data;
    uecc_erab_failed_to_setup_list_t* p_erab_fail_list = &p_ue_context->
        p_curr_proc_data->u.erab_setup_request_data.failed_erab_data;
 
    RRC_ASSERT(PNULL != p_ue_context); 
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    fail_list_count = p_erab_fail_list->count; 
    lc_list_count = (U8)p_rrc_uecc_llim_add_lc_resp->create_drb_status_counter;        
    erab_count = p_erab_setup_list->count;  

    do
    {
        /* SPR_20915 - Fix - Starts */
        for (lc_counter = 0; 
            ((lc_counter < lc_list_count) && (RRC_UECC_LLIM_MAX_DRB > lc_counter));
            lc_counter++)
        {
            lc_matched = RRC_FALSE; 
            for (drb_index = 0; 
                ((drb_index < erab_count) && (MAX_ERAB_COUNT > drb_index));
                drb_index++)
            {
                if (p_rrc_uecc_llim_add_lc_resp->create_drb_status[lc_counter].lc_id == 
                        p_erab_setup_list->erab_info[drb_index].lc_id)
                {
                    erab_id = p_erab_setup_list->erab_info[drb_index].erab_id;
                    lc_matched = RRC_TRUE;
                    break;
                }         
            }  
            if(RRC_TRUE == lc_matched)
            {
                /* eMTC changes start */
                /* SPR fix 22915 start */
                if ((RRC_NO_ERROR == p_rrc_uecc_llim_add_lc_resp->
                    create_drb_status[lc_counter].response) &&
                    ( RRC_FAILURE != p_rrc_uecc_llim_add_lc_resp->response_code))
                /* SPR fix 22915 stop */
                /* eMTC changes stop */
                {
                    /* COPY SELF TEID HERE */                 
                    p_ue_context->p_e_rab_list[erab_id]->teid_self =
                        p_rrc_uecc_llim_add_lc_resp->create_drb_status[lc_counter].
                        s1u_response.tunnel_info_cnf[0].teid_self; 
                    p_ue_context->p_e_rab_list[erab_id]->
                        self_transport_address_length =
                        (U8)p_rrc_uecc_llim_add_lc_resp->create_drb_status[lc_counter].
                        s1u_response.tunnel_info_cnf[0].transport_addr.data_length;

                    /* COPY SELF Transport Address */                 
                    l3_memcpy_wrapper(p_ue_context->p_e_rab_list[erab_id]->
                    self_transport_address,
                    p_rrc_uecc_llim_add_lc_resp->create_drb_status[lc_counter].
                    s1u_response.tunnel_info_cnf[0].transport_addr.data,
                    p_rrc_uecc_llim_add_lc_resp->create_drb_status[lc_counter].
                    s1u_response.tunnel_info_cnf[0].transport_addr.data_length); 

                }
                /* eMTC changes start */
                /* SPR fix 22915 start */
                else if ( RRC_FAILURE == p_rrc_uecc_llim_add_lc_resp->response_code)
                {
                    RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                            "Dediacted Erabs creation failed at LL");
                }
                /* SPR fix 22915 stop */
                /* eMTC changes stop */
                else
                {
                    /* Set status to Response in Share Data in ERAB List */
                    p_erab_setup_list->erab_info[drb_index].status =  
                        p_rrc_uecc_llim_add_lc_resp->create_drb_status[lc_counter].response;

                    /* Copy E_RAB in failed list */
                    p_erab_fail_list->erab_failed_to_setup_item[fail_list_count].
                        e_RAB_ID = erab_id;

                    /* Map Cause in Fail List */                             
                    uecc_llim_build_s1ap_cause(
                        p_ue_context->p_gb_context, 
                        &p_erab_fail_list->erab_failed_to_setup_item[fail_list_count].cause,
                        (rrc_response_t)(p_rrc_uecc_llim_add_lc_resp->create_drb_status[lc_counter].response));

                    /* Delete Failed ERAB from UE CONTEXT */
                    uecc_fsm_rm_erab_setup_deallocate_erab(
                        p_ue_context,           
                        erab_id);

                    /* Increment Fail List Count */ 
                    fail_list_count++;
                }
            }
        }
        /* SPR_20915 - Fix - Ends */
    }while(0);

    /* Copy fail list count to Fail List */
    p_erab_fail_list->count = fail_list_count;
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * Function Name  : uecc_is_default_bearer_established
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This Function checks if default bearer is established.
 ****************************************************************************/
rrc_return_et uecc_is_default_bearer_established
(
    uecc_ue_context_t*  p_ue_context
)
{
    rrc_return_et rrc_response = RRC_FAILURE;
    U16           erab_id      = RRC_NULL;

    for (erab_id = 0;
         erab_id < MAX_ERAB_COUNT;
         erab_id++)
    {
        if (p_ue_context->p_e_rab_list[erab_id] != PNULL) 
        {
            /* Verify that atleast one established bearer is Non-GBR */
            if ((p_ue_context->p_e_rab_list[erab_id]->e_RABlevelQoSParameters.qCI >= 
                                                    RRC_MIN_QCI_NON_GBR_LIMIT) &&
                (p_ue_context->p_e_rab_list[erab_id]->e_RABlevelQoSParameters.qCI <= 
                                                    RRC_MAX_QCI_NON_GBR_LIMIT))
            {
                rrc_response = RRC_SUCCESS;
                
                RRC_UECC_RM_FSM_TRACE (RRC_INFO, 
                     "Default Non-GBR bearer Established");
                break;
            }
        }
    }

    return rrc_response;
}

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_rm_fail_stop_proc_timer
 * Inputs         : uecc_ue_context_t*  p_ue_context
 *                  rrc_bool_et*        p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Stop ICS Procedural Timer
 ****************************************************************************/
void uecc_fsm_rm_fail_stop_proc_timer(
		uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
		rrc_bool_et*        p_break
		)
{
	RRC_ASSERT(PNULL != p_ue_context);
	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
			p_break);

	RRC_UECC_RM_FSM_TRACE(RRC_INFO, "FAILURE: Stop Timer");

	/* Stop timer */
	uecc_ue_timer_stop(p_ue_context->p_gb_context, 
			UECC_UE_ICS_TIMER,
			&p_ue_context->timers_data);

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

}
/****************************************************************************
 * Function Name  : uecc_fsm_rm_sgnb_addition_ongoing_init_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                          UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rm_sgnb_addition_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /*Build and send SgNB Addition Request*/
    if (RRC_SUCCESS != 
            uecc_x2ap_build_and_send_sgnb_addtion_request(p_ue_context))
    {
        RRC_UECC_RM_FSM_TRACE(RRC_ERROR,"Error while sending the SGNB_ADD_REQ");

        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_rm_fail_sgnb_addition_request_fail);
    }
    else
    {
        /* increment the sk_counter needs to be incremented every time
         * a SgNBAddition Request is send.*/
        p_ue_context->sk_counter_r15++;

        /* Stopping UECC_UE_ICS_TIMER as initiating an outside 
         * network call for SgNB Addtition. The timer will be reinitiated
         * once the Acknowledgement for the SgNB Addition is received.*/
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_UE_ICS_TIMER,
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
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                    "UECC_DC_PREP_TIMER IS NOT Started");
            /* Stop Procedure */
            uecc_fsm_finish_active_procedure(p_ue_context);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_sgnb_addition_ongoing_addition_ack_rcvd 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_X2AP_SGNB_ADDITION_REQ_ACK from X2AP
 ****************************************************************************/
void uecc_fsm_rm_sgnb_addition_ongoing_addition_ack_rcvd(
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
    if (RRC_SUCCESS == 
            uecc_x2ap_handle_sgnb_addition_ack_msg_rcvd(p_ue_context, p_api))
    {
        /* Stopping UECC_DC_PREP_TIMER on reception of SgNBAddReqAck */
        uecc_ue_timer_stop(p_ue_context->p_gb_context,
                UECC_DC_PREP_TIMER,
                &p_ue_context->timers_data);  

        /*
         * Starting UECC_UE_ICS_TIMER again as it was stopped
         * when SgNBAddition was initiated. This timer will be stopped
         * on ERAB Setup Procedure completion.
         */
        if (RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context, 
                    UECC_UE_ICS_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                    "UECC_UE_ICS_TIMER IS NOT Started");

            /* Stop Procedure */
            uecc_fsm_finish_active_procedure(p_ue_context);
        }
        else
        {
            /*send the llim reconfig request to the lower layers for
             * SRB and other eRabs*/
            UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_rm_llim_reconfigure_ue_ongoing,
                    PNULL);
        }
    }
    else
    {
        RRC_UECC_RM_FSM_TRACE(RRC_ERROR,"Error while handling SgNBAdditionRequestAck"
                " message. Start Local Error Handling");
        /*Logical_Error*/
        uecc_fsm_process_event(
                p_ue_context,
                UECC_EV_X2AP_ERROR_INDICATION,
                p_api);
        /*Logical_Error*/

    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_sgnb_addition_ongoing_timer_expiry 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rm_sgnb_addition_ongoing_timer_expiry(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    U8    erab_index    = RRC_NULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /*Logical_Error*/
  /*release the x2ap context for this perticular UE*/
   uecc_x2ap_build_and_send_ue_release_ind(p_ue_context);
   /*reset is_sgnb_add_reqd in p_ue_context*/
   p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    /*Logical_Error*/
    if ( p_ue_context->e_rab_list_count ==
          (rrc_uecc_count_scg_bearer(p_ue_context)) )
    {
        /* Put all E_RAB's in failed list from ERAB Setup List */
        uecc_fsm_rm_fail_action_mark_all_erab_failed(
                p_ue_context,
                RRC_TIMER_EXPIRY);
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_rm_fail_wait_sgnb_actions);
    }
    else
    {
        /*
         * Starting UECC_UE_ICS_TIMER again as it was stopped
         * when SgNBAddition was initiated. This timer will be stopped
         * on ERAB Setup Procedure completion.
         */
        if (RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context, 
                    UECC_UE_ICS_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                    "UECC_UE_ICS_TIMER IS NOT Started");

            /* Stop Procedure */
            uecc_fsm_finish_active_procedure(p_ue_context);
        }
        else
        {
            /*send the llim reconfig request to the lower layers for
             * SRB and other eRabs*/
            UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_rm_llim_reconfigure_ue_ongoing,
                    PNULL);
        }
        uecc_erab_failed_to_setup_list_t*  p_erab_fail_list =
            &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
            failed_erab_data);
        uecc_rm_erab_setup_info_t*  p_erab_setup_list =
            &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
            erab_info_data);

        p_erab_fail_list->count = RRC_NULL;

        U8    fail_list_index =  RRC_NULL;
        rrm_cause_t     cause;
        for(erab_index = RRC_NULL; erab_index < MAX_ERAB_COUNT; erab_index++)
        {
            /*check if p_e_rab_list is not null*/
            if (( PNULL != p_ue_context->p_e_rab_list[erab_index] ) &&
                    ( ( p_ue_context->p_e_rab_list[erab_index]->m.DC_BearerTypePresent ) ))
            {
                p_erab_fail_list->erab_failed_to_setup_item[p_erab_fail_list->count].e_RAB_ID =
                    p_ue_context->p_e_rab_list[erab_index]->e_RAB_ID;

                cause.type = T_s1ap_Cause_radioNetwork;
                cause.value = s1ap_release_due_to_eutran_generated_reason;
                /* MAP CAUSE VALUE HERE AND STORE IN FAILED LIST */
                if ( RRC_SUCCESS != uecc_s1ap_build_s1ap_cause(
                            &p_erab_fail_list->erab_failed_to_setup_item
                            [p_erab_fail_list->count].cause, &cause,
                            p_ue_context))
                {
                    return;
                }
                p_erab_fail_list->erab_failed_to_setup_item[p_erab_fail_list->count].dc_bearer_type = 1;

                p_erab_fail_list->count++;
                /* Fill SgNB cause for GUARD TIMER EXPIRY*/
                if (RRC_ONE != p_ue_context->m.sgnbCausePresent)
                {
                    p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
                    p_ue_context->sgnb_cause.u.radioNetwork = x2ap_failure_in_the_radio_interface_procedure;

                    p_ue_context->m.sgnbCausePresent = RRC_ONE;
                }
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

    }


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_sgnb_addition_ongoing_sgnb_add_req_reject 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_X2AP_SGNB_ADDITION_REQ_REJECT
 ****************************************************************************/
 /*Logical_Error*/
void uecc_fsm_rm_sgnb_addition_ongoing_sgnb_add_req_reject(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    U8    fail_list_index =  RRC_NULL;
    rrm_cause_t     cause;
    U8 mn_mcg_present = RRC_FALSE;
	U8 erab_index = 0;
    U8 erab_id = 0;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
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
                uecc_fsm_rm_ics_proc_error_ind_fail_1_actions);
    }
    else
    {
        /*Start the UECC_UE_ICS_TIMER for establishing remaining RABs
         * which was stopped at time of sending SgNb Addition Req*/

        if(RRC_SUCCESS != uecc_ue_timer_start(
                    p_ue_context->p_gb_context,
                    UECC_UE_ICS_TIMER,
                    &p_ue_context->p_gb_context->ue_timers_data,
                    &p_ue_context->timers_data,
                    p_ue_context->ue_index))
        {
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
                    "UECC_UE_ICS_TIMER IS NOT Started");
            /* Stop Procedure */
            uecc_fsm_finish_active_procedure(p_ue_context);
        }
        else
        {
            /*send the llim reconfig request to the lower layers for oter 
             * eRabs which are marked as MN Terminated MCG Bearer*/
            UECC_FSM_SET_CHILD_STATE(   
                    p_ue_context,
                    &uecc_fsm_rm_llim_reconfigure_ue_ongoing,
                    PNULL);
        }


    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

 /*Logical_Error*/
}

/****************************************************************************
 * Function Name  : uecc_fsm_rm_sgnb_addition_ongoing_x2ap_error_ind 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_X2AP_ERROR_INDICATION
 ****************************************************************************/
void uecc_fsm_rm_sgnb_addition_ongoing_x2ap_error_ind(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
 /*Logical_Error*/
	RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    U8    fail_list_index =  RRC_NULL;
    rrm_cause_t     cause;
    U8 mn_mcg_present = RRC_FALSE;
    U8 erab_index = 0;
    U8 erab_id = 0;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
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
                uecc_fsm_rm_ics_proc_error_ind_fail_1_actions);
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
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR,
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
                    &uecc_fsm_rm_llim_reconfigure_ue_ongoing,
                    PNULL);
        }


    }
  
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
 /*Logical_Error*/
}



/****************************************************************************
 * Function Name  : uecc_fsm_rm_fail_action_send_sgnb_reconfig_complete 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*       - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Sends SgNBReconfigurationComplete with failure to release
 *                  the resources at SgNB
 ****************************************************************************/
void uecc_fsm_rm_fail_action_send_sgnb_reconfig_complete(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
    {
        fill_sgnb_cause_for_scgFailureInformationNR_failure_type(
                p_ue_context,
                PNULL
                );

        /* Send SgNBReconfigurationComplete with failure */ 
        if (RRC_SUCCESS != 
                uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
                    p_ue_context
                    ))
        {
            RRC_UECC_RM_FSM_TRACE(RRC_WARNING,
                    "[%s],uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure"
                    " failed !!",__FUNCTION__);
        }

        /* Reset SgNBAddRequired Flag in UE_CTX*/
        p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_rm_fail_action_send_sgnb_release
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et*       - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Sends SgNB Release to discard SgNBAdditionRequest send 
 *                  earlier.
 *                  
 ****************************************************************************/
/* coverity_<280379>_fix_start*/
#if 0
void uecc_fsm_rm_fail_action_send_sgnb_release(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    if(RRC_TRUE == p_ue_context->is_sgnb_add_reqd) 
    {
        p_ue_context->sgnb_cause.t = T_x2ap_Cause_radioNetwork;
        p_ue_context->sgnb_cause.u.radioNetwork = x2ap_tDCprep_expiry;

        /* Send SgNB RELEASE to SgNB */ 
        if (RRC_SUCCESS != 
                build_and_send_sgnb_release_request(p_ue_context))
        {
            RRC_UECC_RM_FSM_TRACE(RRC_ERROR, 
                    "[%s],Unable to process SgNB Release Request", __FUNCTION__);
        }

        /* Reset SgNBAddRequired Flag in UE_CTX*/
        p_ue_context->is_sgnb_add_reqd = RRC_FALSE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
#endif
/* coverity_<280379>_fix_end*/
/****************************************************************************
 * Function Name  : uecc_fsm_rm_scg_failure_information_nr_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the Event
 *                       UECC_EV_SCG_FAILURE_INFORMATION_NR
 ****************************************************************************/
void uecc_fsm_rm_scg_failure_information_nr_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RM_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

	/* Put all E_RAB's in failed list from ERAB Setup List */
	uecc_fsm_rm_fail_action_mark_all_erab_failed(
			p_ue_context,
			RRC_NULL);

	uecc_fsm_start_failure_processing(
			p_ue_context,
			uecc_fsm_rm_fail_wait_ue_reconfig_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*NR_DC Code Change Stop*/
#endif
