/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_rcrfsm.c,v 1.11 2010/10/13 07:03:35 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description :
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_rcrfsm.c,v $
 * Revision 1.11  2010/10/13 07:03:35  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.6.4.5  2010/09/13 13:04:00  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.6.4.4  2010/09/07 12:31:18  gur18569
 * incorporated review comments related to rrm rel not required flag
 *
 * Revision 1.6.4.3  2010/09/07 04:13:05  gur18569
 *  added a flag in RCR Fsm which would decide to interact with RRM or not
 *
 * Revision 1.6.4.2  2010/08/10 10:02:57  gur18569
 * incorported review comments for ho
 *
 * Revision 1.6.4.1  2010/07/21 08:34:41  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.6  2010/04/07 15:32:59  gur21006
 * warnings removed
 *
 * Revision 1.5  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.4  2010/02/24 04:26:29  gur10248
 * comments_dpisposed_cell_del_SRB2_1_1
 *
 * Revision 1.3  2010/01/11 11:42:30  gur10248
 * *** empty log message ***
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.30  2009/12/22 16:39:44  gur18569
 * commented field name initialization in fsm activity structures
 *
 * Revision 1.1.2.29  2009/12/11 11:27:34  ukr16032
 * UECC SSIT - LLIM response codes processing updated for RM and RCR
 *
 * Revision 1.1.2.28  2009/12/08 11:22:51  ukr16032
 * UECC - inspection log items LG_20, LG_23, LG_24, LG_26, LG_27, LG_28, LG_29, LG_34, LG_40, LG_41 fixed
 *
 * Revision 1.1.2.27  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.25.2.13  2009/11/24 12:12:50  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.2.25.2.12  2009/11/24 11:32:35  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.2.25.2.11.2.2  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.2.25.2.11.2.1  2009/11/20 12:16:24  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.25.2.11  2009/11/19 09:52:18  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.2.25.2.10  2009/11/16 11:02:49  ukr18880
 * 80 lines
 *
 * Revision 1.1.2.25.2.9  2009/11/14 16:29:54  ukr16032
 * UECC UE Capability transfer implemented
 * UECC ICSP - security capability checking implemented
 *
 * Revision 1.1.2.25.2.8  2009/11/11 17:47:41  ukr16032
 * UECC
 *  -- S1AP UL NAS message generation updated
 *  -- FSM - review
 *  -- FSM - reset and rrm_release handling added to wait_for_s1_connection and
 * ue_connected states
 *  -- S1AP_RESET_MSG_IND message added
 *
 * Revision 1.1.2.25.2.6  2009/11/10 20:02:17  ukr16032
 * UECC S1AP_STATUS_MSG_IND handling added
 *
 * Revision 1.1.2.25.2.5  2009/11/10 17:11:00  ukr15916
 * wrong rcr cause.t=0 removed
 *
 * Revision 1.1.2.25.2.4  2009/11/08 00:27:16  ukr15916
 * s1ap-related code updated in UECC
 *
 * Revision 1.1.2.25.2.3  2009/11/03 11:43:37  ukr18880
 * UECC FSM engine was updated
 *
 * Revision 1.1.2.25.2.2  2009/10/27 12:10:39  ukr15916
 * header fixed
 *
 * Revision 1.1.2.25.6.9  2009/10/26 13:34:05  ukr18880
 * UECC FSM engine modification
 *
 * Revision 1.1.2.25.6.8  2009/10/26 12:58:54  ukr18880
 * UECC FSM update
 *
 * Revision 1.1.2.25.6.7  2009/10/26 12:51:19  ukr15916
 * UECC-OAMH interface updated.
 *
 * Revision 1.1.2.25.6.6  2009/10/23 14:13:44  ukr18880
 * UECC FSM update
 *
 * Revision 1.1.2.25.6.5  2009/10/23 12:35:41  ukr18880
 * update of UECC FSM
 *
 * Revision 1.1.2.25.6.4  2009/10/23 08:55:47  ukr18880
 * UECC RCR FSM is now worked on new engine
 *
 * Revision 1.1.2.25.6.3  2009/10/22 14:01:24  ukr18880
 * unnecessary functions(timers handling) and #includes were erased
 *
 * Revision 1.1.2.25.6.2  2009/10/22 07:03:19  ukr18880
 * New UECC FSM initial creation
 *20.10.2009    AndriyT -   initial creation
 *
 ****************************************************************************/


/****************************************************************************
* Header File Includes
***************************************************************************/

#include "rrc_asn_enc_dec.h"
#include "uecc_logger.h"

#include "uecc_rcrfsm.h"
#include "uecc_trg_hofsm.h"

#include "uecc_llim.h"
#include "uecc_rrm.h"
#include "uecc_s1ap.h"

#include "uecc_ue_timer_utils.h"
#include "uecc_common.h"
#include "uecc_fsm_engine.h"
#include "rrc_s1ap_asn_intrl_dec_3gpp.h"
/*CR 101 changes start*/
#include "rrc_shared_stats.h"
#include "rrc_mac_intf.h"
/*CR 101 changes stop*/
#ifdef ENDC_ENABLED
/*EN-DC_changes_start*/
#include "uecc_x2ap.h"
#include "uecc_mainfsm.h"
/*EN-DC_changes_end*/
#endif
#include "uecc_erab_delfsm.h"

/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_RCR_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,\
        p_ue_context->ue_index,\
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
        ##__VA_ARGS__)


/*SPR 20901 Start*/
/* Code Removed */
/*SPR 20901 Stop*/
/* SPR 18552 Fix Start */
#define RRC_SET_UE_CTX_DEL_STATUS(_del_flag_) \
    _del_flag_.is_rrc_resp_expected = 1
/* SPR 18552 Fix Stop */

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static void uecc_fsm_ue_connection_release_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void*               p_api          /* Input API buffer */
);

static void uecc_fsm_ue_connection_release_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void*               p_api          /* Input API buffer */
);

static
 void uecc_fsm_ue_connection_release_setup_failure_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void*               p_api          /* Input API buffer */
);
static 
 void uecc_fsm_overload_ue_conn_rel_setup_failure_procedure_finish_success(
         uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
         void*               p_api          /* Input API buffer */
);

static void uecc_fsm_ue_connection_release_csc_cel_del_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/* State UECC_RCR_UE_RELEASE_NOTIFICATION_ONGOING */
static void uecc_fsm_rcr_ue_release_notification_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void*               p_api          /* Input API buffer */
);

static void uecc_fsm_rcr_llim_srb_data_status_ind_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);

static void uecc_fsm_rcr_ue_release_notification_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);

/*RLF_fix Start*/
static void uecc_fsm_rcr_ue_release_notification_ongoing_rlf_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);
/*RLF_fix Stop*/

/*SPR_17062_START*/
static void uecc_fsm_rcr_ue_release_notification_rrm_ue_conn_rel_ind_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);

static void uecc_fsm_rcr_rrm_ue_conn_rel_ind_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);
/*SPR_17062_END*/

/* State UECC_RCR_RRM_RELEASE_NOTIFICATION_ONGOING */
static void uecc_fsm_rcr_rrm_release_notification_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void*               p_api          /* Input API buffer */
);

static void uecc_fsm_rcr_rrm_ue_release_resp_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);

static void uecc_fsm_rcr_rrm_release_notification_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);

/* State UECC_RCR_LLIM_DELETE_UE_ONGOING */
static void uecc_fsm_rcr_llim_delete_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void*               p_api          /* Input API buffer */
);

static void uecc_fsm_rcr_llim_delete_ue_entity_resp_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);

static void uecc_fsm_rcr_llim_delete_ue_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);

static void uecc_fsm_rcr_ue_release_wait_for_s1ap_init_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);

static void uecc_fsm_rcr_rrm_ue_release_req_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);

static void uecc_fsm_rcr_rrm_ue_release_req_wait_for_s1ap_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);

static void uecc_fsm_rcr_ue_release_wait_for_s1ap_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context, /* UECC UE context */
    void                *p_api_data    /* Input API buffer */
);

static void uecc_fsm_ue_rrm_connection_release_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void*               p_api          /* Input API buffer */
);

static
 void uecc_fsm_ue_rrm_connection_release_setup_failure_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void*               p_api          /* Input API buffer */
);

void uecc_fsm_ue_rrm_connection_release_mme_guard_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);

static void uecc_fsm_ue_rrm_connection_release_no_mme_id_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,  /* UECC UE context */
    void*               p_api          /* Input API buffer */
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
/*Error_Handling_changes*/
static void uecc_fsm_sgnb_rel_error_handler(
   uecc_ue_context_t              *p_ue_context,
   void                           *p_api);
/*Error_Handling_changes*/

/*EN-DC_changes_end*/
#endif

/****************************************************************************
 Private Variables
****************************************************************************/

/**************************************
 * States definition
 **************************************/
static uecc_fsm_handler_record_t
            uecc_fsm_rcr_ue_release_notification_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                  },
        uecc_fsm_rcr_ue_release_notification_ongoing_init_handler           },
    { { UECC_EV_LLIM_SRB_DATA_STATUS_IND    },
        uecc_fsm_rcr_llim_srb_data_status_ind_handler                       },
    { { UECC_EV_TIMER_EXPIRY                },
        uecc_fsm_rcr_ue_release_notification_ongoing_timer_expiry_handler   },
/*RLF_fix Start*/
    { { UECC_EV_LLIM_RADIO_LINK_FAILURE_IND                },
        uecc_fsm_rcr_ue_release_notification_ongoing_rlf_handler   },
/*RLF_fix Stop*/
    /*SPR_17062_START*/
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND   },
        uecc_fsm_rcr_ue_release_notification_rrm_ue_conn_rel_ind_handler   },
    /*SPR_17062_END*/
    { { UECC_LAST_ID                        },  PNULL                       }
};

static uecc_fsm_handler_record_t
            uecc_fsm_rcr_rrm_release_notification_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                  },
        uecc_fsm_rcr_rrm_release_notification_ongoing_init_handler          },
    { { UECC_EV_RRM_UE_RELEASE_RESP         },
        uecc_fsm_rcr_rrm_ue_release_resp_handler                            },
    { { UECC_EV_TIMER_EXPIRY                },
        uecc_fsm_rcr_rrm_release_notification_ongoing_timer_expiry_handler  },
    /*SPR_17062_START*/
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND   },
        uecc_fsm_rcr_rrm_ue_conn_rel_ind_handler   },
    /*SPR_17062_END*/
    { { UECC_LAST_ID                        },  PNULL                       }
};

static uecc_fsm_handler_record_t uecc_fsm_rcr_llim_delete_ue_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                  },
        uecc_fsm_rcr_llim_delete_ue_ongoing_init_handler            },
    { { UECC_EV_LLIM_DELETE_UE_ENTITY_RESP  },
        uecc_fsm_rcr_llim_delete_ue_entity_resp_handler             },
    { { UECC_EV_TIMER_EXPIRY                },
        uecc_fsm_rcr_llim_delete_ue_ongoing_timer_expiry_handler    },
    { { UECC_LAST_ID                        },  PNULL               }
};

static uecc_fsm_handler_record_t
    uecc_fsm_rcr_ue_release_wait_for_s1ap_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                  },
        uecc_fsm_rcr_ue_release_wait_for_s1ap_init_handler              },
    { { UECC_EV_UE_RELEASE_REQ},
        uecc_fsm_rcr_rrm_ue_release_req_wait_for_s1ap_handler           },
    { { UECC_EV_MME_GUARD_TIMER_EXPIRY              },
        uecc_fsm_rcr_ue_release_wait_for_s1ap_timer_expiry_handler      },
    { { UECC_LAST_ID                        },  PNULL                   }
};

static uecc_fsm_activity_state_t uecc_fsm_rcr_ue_release_notification_ongoing =
{
   /* .s_name         =*/   (S8*)"UECC_RCR_UE_RELEASE_NOTIFICATION_ONGOING",
   /* .s_id           =*/   (S8*)"RCR_UE",
   /* .is_need_init   =*/   RRC_TRUE,
   /* .p_mht          =*/   uecc_fsm_rcr_ue_release_notification_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_rcr_rrm_release_notification_ongoing =
{
   /* .s_name         =*/   (S8*)"UECC_RCR_RRM_RELEASE_NOTIFICATION_ONGOING",
   /* .s_id           =*/   (S8*)"RCR_RRM",
   /* .is_need_init   =*/   RRC_TRUE,
   /* .p_mht          =*/   uecc_fsm_rcr_rrm_release_notification_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_rcr_llim_delete_ue_ongoing =
{
   /* .s_name         =*/   (S8*)"UECC_RCR_LLIM_DELETE_UE_ONGOING",
   /* .s_id           =*/   (S8*)"RCR_LLIM",
   /* .is_need_init   =*/   RRC_TRUE,
   /* .p_mht          =*/   uecc_fsm_rcr_llim_delete_ue_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_rcr_ue_release_wait_for_s1ap_ongoing =
{
   /* .s_name         =*/   (S8*)"UECC_RCR_UE_RELEASE_WAIT_FOR_S1AP_ONGOING",
   /* .s_id           =*/   (S8*)"RCR_WAIT_S1AP",
   /* .is_need_init   =*/   RRC_TRUE,
   /* .p_mht          =*/   uecc_fsm_rcr_ue_release_wait_for_s1ap_ongoing_mht
};

/**************************************
 * Activity definition
 **************************************/

/*  RRC Connection Release activity */
static uecc_fsm_activity_t uecc_fsm_rrc_connection_release_activity =
{
  /*  .s_name             =*/ (S8*)"RRC Connection Release",
  /*  .s_id               =*/ (S8*)"UE_RCR_A",
  /*  .p_first_state      =*/ &uecc_fsm_rcr_ue_release_notification_ongoing
};

/*  RRC RRM Connection Release Wait for S1AP activity */
static uecc_fsm_activity_t
    uecc_fsm_rrc_rrm_connection_release_wait_for_s1ap_activity =
{
  /*  .s_name             =*/ (S8*)"RRC RRM Connection Release Wait For S1AP",
  /*  .s_id               =*/ (S8*)"UE_RCR_RRM_W_S1_A",
  /*  .p_first_state      =*/ &uecc_fsm_rcr_ue_release_wait_for_s1ap_ongoing
};
#ifdef ENDC_ENABLED
/*EN-DC_changes_start*/
static uecc_fsm_handler_record_t uecc_fsm_sgnb_release_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT               },
        uecc_fsm_sgnb_rel_req_handler           },
    { { UECC_EV_X2AP_SGNB_RELEASE_REL_ACK      },
        uecc_fsm_sgnb_release_ack_handler },
    { { UECC_EV_TIMER_EXPIRY                },
        uecc_fsm_sgnb_rel_ongoing_timer_expiry_handler   },
    { { UECC_EV_X2AP_ERROR_INDICATION },
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
/**************************************
 * Procedures definition
 **************************************/

static uecc_fsm_activity_t*
    uecc_fsm_ue_connection_release_procedure_activity_list[] =
{
    &uecc_fsm_rrc_connection_release_activity,
    PNULL
};

static uecc_fsm_activity_t*
    uecc_fsm_ue_rrm_connection_release_procedure_activity_list[] =
{
    &uecc_fsm_rrc_connection_release_activity,
    &uecc_fsm_rrc_rrm_connection_release_wait_for_s1ap_activity,
    PNULL
};

/* S1AP initiated Connection Release Procedure */
uecc_fsm_procedure_t uecc_fsm_ue_connection_release_procedure =
{
  /*  .s_name           =*/ (S8*)"UE Connection Release procedure",
  /*  .s_id             =*/ (S8*)"UE_CR_P",
  /*  .constructor_f    =*/
      uecc_fsm_ue_connection_release_procedure_constructor,
  /*  .finish_success_f =*/
      uecc_fsm_ue_connection_release_procedure_finish_success,
  /*  .finish_fail_f    =*/ PNULL,
  /*  .activity_list    =*/
      uecc_fsm_ue_connection_release_procedure_activity_list,
  /*  .p_mht            =*/ PNULL
};

/* eNB initiated Connection Release Procedure due to Connection Setup Failure */
uecc_fsm_procedure_t uecc_fsm_ue_connection_release_setup_failure_procedure =
{
  /*  .s_name           =*/ (S8*)"UCR due to RCS failure",
  /*  .s_id             =*/ (S8*)"UE_CR_RCSF_P",
  /*  .constructor_f    =*/
      uecc_fsm_ue_connection_release_procedure_constructor,
  /*  .finish_success_f =*/
      uecc_fsm_ue_connection_release_setup_failure_procedure_finish_success,
  /*  .finish_fail_f    = */ PNULL,
  /*  .activity_list    = */
      uecc_fsm_ue_connection_release_procedure_activity_list,
  /*  .p_mht            = */ PNULL
};

/* eNB initiated Connection Release Procedure due to MME Overload*/
uecc_fsm_procedure_t uecc_fsm_overload_ue_connection_release_setup_failure_procedure =
{
  /*  .s_name           =*/ (S8*)"UCR due to MME OVERLOAD",
  /*  .s_id             =*/ (S8*)"UE_CR_RCSF_P",
  /*  .constructor_f    =*/
      uecc_fsm_ue_connection_release_procedure_constructor,
  /*  .finish_success_f =*/
      uecc_fsm_overload_ue_conn_rel_setup_failure_procedure_finish_success,
  /*  .finish_fail_f    = */ PNULL,
  /*  .activity_list    = */
      uecc_fsm_ue_connection_release_procedure_activity_list,
  /*  .p_mht            = */ PNULL
};

static uecc_fsm_handler_record_t
    uecc_fsm_ue_rrm_connection_release_procedure_mht[] =
{
    { { UECC_EV_MME_GUARD_TIMER_EXPIRY                },
        uecc_fsm_ue_rrm_connection_release_mme_guard_timer_expiry_handler    },
    { { UECC_EV_UE_RELEASE_REQ},
        uecc_fsm_rcr_rrm_ue_release_req_handler             },
    { { UECC_LAST_ID                      },    PNULL       }
};

/* RRM initiated Connection Release Procedure (unoptimized handover) */
uecc_fsm_procedure_t uecc_fsm_ue_rrm_connection_release_procedure =
{
  /*  .s_name           =*/ (S8*)"UCR due to RRM",
  /*  .s_id             =*/ (S8*)"UE_CR_RRM_P",
  /*  .constructor_f    =*/
      uecc_fsm_ue_rrm_connection_release_procedure_constructor,
  /*  .finish_success_f =*/
      uecc_fsm_ue_rrm_connection_release_setup_failure_procedure_finish_success,
  /*  .finish_fail_f    = */ PNULL,
  /*  .activity_list    = */
      uecc_fsm_ue_rrm_connection_release_procedure_activity_list,
  /*  .p_mht            = */ uecc_fsm_ue_rrm_connection_release_procedure_mht
};

/* RRM initiated Connection Release Procedure due to no mme id */
uecc_fsm_procedure_t uecc_fsm_ue_rrm_connection_release_no_mme_id_procedure =
{
  /*  .s_name           =*/ (S8*)"UCR due to no mme id",
  /*  .s_id             =*/ (S8*)"UE_CR_NO_MME_P",
  /*  .constructor_f    =*/
      uecc_fsm_ue_rrm_connection_release_no_mme_id_procedure_constructor,
  /*  .finish_success_f =*/
      uecc_fsm_ue_connection_release_setup_failure_procedure_finish_success,
  /*  .finish_fail_f    = */ PNULL,
  /*  .activity_list    = */
      uecc_fsm_ue_connection_release_procedure_activity_list,
  /*  .p_mht            = */ PNULL
};
uecc_fsm_procedure_t uecc_fsm_ue_connection_release_csc_cel_del_procedure =
{
   /* .s_name           = */(S8*)"UCR due to CSC CEL DEL",
    /*.s_id             = */(S8*)"UE_CR_CSC_DEL_P",
    /*.constructor_f    = */uecc_fsm_ue_connection_release_procedure_constructor,
    /*.finish_success_f =*/
        uecc_fsm_ue_connection_release_csc_cel_del_procedure_finish_success,
   /* .finish_fail_f    =*/ PNULL,
   /* .activity_list    =*/ uecc_fsm_ue_connection_release_procedure_activity_list,
    /*.p_mht            =*/ PNULL
};

/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/
/****************************************************************************
 * Function Name  : uecc_fsm_ue_connection_release_procedure_constructor
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for UE Connection Release procedure
 ****************************************************************************/
void uecc_fsm_ue_connection_release_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

/* Store transaction id and use it for Release operation */
#ifdef RRC_UECC_TR_ID_CONST
    uecc_ue_ctx_set_op_transction_id(p_ue_context, 1); 
#else
    uecc_ue_ctx_set_op_transction_id(p_ue_context,
                    uecc_ue_ctx_get_next_transction_id(p_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/
    /* Start timer */
    /* SPR 18552 Fix Start */
    RRC_SET_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
    /* SPR 18552 Fix Stop */ 

    if ( RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context, 
            UECC_UE_RCR_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index) )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[UECC_UE_RCR_TIMER]"
                "Timer Start Failed");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_connection_release_procedure_finish_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Success handler for UE Connection Release procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_connection_release_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    U32 enb_ue_s1ap_id ;
    rrc_bool_et mme_release_required ;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_RCR_TIMER, &p_ue_context->timers_data);

    /* BUG 12722 Fix Start */
    if (RRC_TRUE == p_ue_context->share_data.uecc_rcr_data.
                               rrm_rel_req_not_required_flag)
    {
        p_ue_context->share_data.uecc_rcr_data.reset_towards_mme_required =
                   RRC_FALSE;
    }
    /* BUG 12722 Fix Stop */

    if ( p_ue_context->
            share_data.uecc_rcr_data.reset_towards_mme_required == RRC_TRUE )
    {
        enb_ue_s1ap_id = p_ue_context->s1ap_ue_id ;
        mme_release_required = RRC_TRUE ;
        if (RRC_SUCCESS != uecc_s1ap_build_and_send_ue_release_msg_req(
                    enb_ue_s1ap_id,
                    mme_release_required,p_ue_context ))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[S1AP_UE_RELEASE_MSG_REQ] Delivery failed");

        }
    }
    /* SPR 16750 Fix Start */
    else if (RRC_TRUE == 
                p_ue_context->share_data.uecc_rcr_data.s1_error_ind_recv)
    {
        if(RRC_FAILURE == uecc_s1ap_build_and_send_ue_release_ind(p_ue_context))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                    "[%s] Failed to send UE Release Indication to S1AP "
                    ,__FUNCTION__);
        }
    }
    /* SPR 16750 Fix Stop */
    /* Send RRC_S1AP_UE_CTX_RELEASE_COMP_RESP message to S1AP */
    else 
    {
        if (RRC_FALSE == p_ue_context->share_data.uecc_rcr_data.
                            ue_context_rel_comp_not_req_flag)
        {
            if (RRC_SUCCESS !=
                    uecc_s1ap_build_and_send_ue_ctx_release_comp_resp( p_ue_context ))
            {
                RRC_UECC_RCR_FSM_TRACE(RRC_ERROR,
                        "[S1AP_UE_CTX_RELEASE_COMP_RESP] Delivery failure");
            }
        }
    }

#if 0
    if ( p_ue_context->sgnbInitiatedSgnbReleaseFlag )
    {
        RRC_UECC_RCR_FSM_TRACE(RRC_WARNING,
                "RRC will delete UE conetxt after sending UE context release to SgNB");
    }
    else
    {
    /* SPR 20704 Fix Start */
    RRC_UECC_UPDATE_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
    /* SPR 20704 Fix End */
    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_UE_RELEASED_INT,
        p_ue_context,
        PNULL);
    }
#endif
    /* SPR 20704 Fix Start */
    RRC_UECC_UPDATE_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
    /* SPR 20704 Fix End */
    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_UE_RELEASED_INT,
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_connection_release_setup_failure_procedure_finish_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Success handler for UE Connection Release procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_connection_release_setup_failure_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_RCR_TIMER, &p_ue_context->timers_data);

    RRC_UECC_UPDATE_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_UE_RELEASED_INT,
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_overload_ue_conn_rel_setup_failure_procedure_finish_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Success handler for UE Connection Release procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_overload_ue_conn_rel_setup_failure_procedure_finish_success(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_RCR_TIMER, &p_ue_context->timers_data);
   /* SPR 20704 Fix Start */
    RRC_UECC_UPDATE_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
   /* SPR 20704 Fix End */
    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
            UECC_EV_UE_RELEASED_INT,
            p_ue_context,
            PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_connection_release_csc_cel_del_procedure_finish_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Success handler for UE Connection Release procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_connection_release_csc_cel_del_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_RCR_TIMER, &p_ue_context->timers_data);
	/* SPR 20704 Fix Start */
	RRC_UECC_UPDATE_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
	/* SPR 20704 Fix End */

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_UE_RELEASED_INT,
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_rcr_ue_release_notification_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rcr_ue_release_notification_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* SPR 21458 Start */
    if ((p_ue_context->
       share_data.uecc_rcr_data.rrc_connection_release_not_required_flag !=
       RRC_TRUE) && (p_ue_context->p_gb_context->l2_instance_reset_flag != RRC_TRUE))
    /* SPR 21458 End */
    {
        /* Send RRC Connection Release to UE */
        if (RRC_SUCCESS !=
                uecc_llim_build_and_send_rrc_connection_release( p_ue_context ))
        {
            RRC_UECC_RCR_FSM_TRACE(RRC_ERROR,
                    "RRCConnectionRelease delivery fail.");
            /* Switch to the next step
             *  - simulate receiving of UECC_LLIM_SRB_DATA_STATUS_IND message
             * Post event to RCR FSM for future processing */

            uecc_event_queue_push_event(
                    UECC_EV_LLIM_SRB_DATA_STATUS_IND,
                    p_ue_context,
                    PNULL);
#ifdef MEM_PROFILE_ON
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING,"\n The Memory usage for RRC Connection Release");
            qvPoolStats();
#endif

        }
        /*Bug_11395_fix_start*/
        p_ue_context->ue_ctx_del_status.is_rrc_resp_expected = RRC_TRUE;
        /*Bug_11395_fix_stop*/
    }
    else
    { 
        if (p_ue_context->
                share_data.uecc_rcr_data.rrm_rel_req_not_required_flag !=
                RRC_TRUE)
        {
            UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_rcr_rrm_release_notification_ongoing, PNULL);
        }
        else
        {
#ifdef ENDC_ENABLED
            /*EN-DC_changes_start*/
            if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
            {
                fill_sgnb_release_cause(&p_ue_context->sgnb_cause,&p_ue_context->share_data.uecc_rcr_data.u.cause);
                if(RRC_NULL < 
                        rrc_uecc_count_scg_bearer(p_ue_context)
                        && (p_ue_context->x2apUecontextReleased != RRC_TRUE))
                {
                    UECC_FSM_SET_CHILD_STATE(
                            p_ue_context,
                            &uecc_fsm_sgnb_release_ongoing, PNULL);
                }
            }
            else
            /*EN-DC_changes_end*/
#endif
            {
                /* Switch to the LLIM wait response state */
                UECC_FSM_SET_CHILD_STATE(
                        p_ue_context,
                        &uecc_fsm_rcr_llim_delete_ue_ongoing, PNULL);
            }
        }
    }


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_llim_srb_data_status_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_SRB_DATA_STATUS_IND from LLIM
 ****************************************************************************/
void uecc_fsm_rcr_llim_srb_data_status_ind_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    rrc_uecc_llim_srb_data_status_ind_t* p_llim_srb_data_status_ind = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Get pointer on message */
    if (PNULL != p_api_data)
    {
        p_llim_srb_data_status_ind =
            (rrc_uecc_llim_srb_data_status_ind_t*)p_api_data;

        if (    (UECC_LTE_RRC_SRB1_LC_ID != p_llim_srb_data_status_ind->lc_id)
             || (RRC_SUCCESS != p_llim_srb_data_status_ind->response_code) )
        {
            RRC_UECC_RCR_FSM_TRACE(RRC_WARNING,
                    "RRCConnectionRelease delivery fail.");
        }
    }

    /* Switch to the RRM wait response state */
    if (p_ue_context->
            share_data.uecc_rcr_data.rrm_rel_req_not_required_flag !=
            RRC_TRUE)
    {
        #ifdef ENDC_ENABLED
        /*EN-DC_changes_start*/
        if ( RRC_TRUE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
        {
            uecc_fsm_x2ap_ue_context_release_handler(p_ue_context);
        }
        /*EN-DC_changes_stop*/
        #endif

        UECC_FSM_SET_CHILD_STATE(
                p_ue_context,
                &uecc_fsm_rcr_rrm_release_notification_ongoing, PNULL);
    }
    else
    {
#ifdef ENDC_ENABLED
        /*EN-DC_changes_start*/
        if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
        {
            fill_sgnb_release_cause(&p_ue_context->sgnb_cause,&p_ue_context->share_data.uecc_rcr_data.u.cause);
            if(RRC_NULL < 
                    rrc_uecc_count_scg_bearer(p_ue_context))
            {
                UECC_FSM_SET_CHILD_STATE(
                        p_ue_context,
                        &uecc_fsm_sgnb_release_ongoing, PNULL);
            }
            else
            {
                /* Switch to the LLIM wait response state */
                UECC_FSM_SET_CHILD_STATE(
                        p_ue_context,
                        &uecc_fsm_rcr_llim_delete_ue_ongoing, PNULL);
            }
        }
        else
        {
            /* Switch to the LLIM wait response state */
            UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_rcr_llim_delete_ue_ongoing, PNULL);
        }
        /*EN-DC_changes_end*/
#else
        {
            /* Switch to the LLIM wait response state */
            UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_rcr_llim_delete_ue_ongoing, PNULL);
        }
#endif
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_ue_release_notification_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rcr_ue_release_notification_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);


    RRC_UECC_RCR_FSM_TRACE(RRC_BRIEF, "Time out.");

    /* Restart the Procedural timer */
    if ( RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context,
            UECC_UE_RCR_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index) )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[UECC_UE_RCR_TIMER]"
                "Timer Start Failed");
    }

    /* Post event to RCR FSM for future processing */
    uecc_event_queue_push_event(
        UECC_EV_LLIM_SRB_DATA_STATUS_IND,
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*SPR_17062_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_rcr_ue_release_notification_rrm_ue_conn_rel_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND
 ****************************************************************************/
void uecc_fsm_rcr_ue_release_notification_rrm_ue_conn_rel_ind_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);        

    p_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag
        = RRC_TRUE;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_rrm_ue_conn_rel_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_UE_CONNECTION_RELEASE_IND from RRM.
 *                  In current state, this event shall be handled as
 *                  RRM_UE_RELEASE_RESP.
 ****************************************************************************/
void uecc_fsm_rcr_rrm_ue_conn_rel_ind_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);        

    /* Switch to the LLIM wait response state */
    UECC_FSM_SET_CHILD_STATE(
        p_ue_context,
        &uecc_fsm_rcr_llim_delete_ue_ongoing, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR_17062_END*/

/*RLF_fix Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_rcr_ue_release_notification_ongoing_rlf_handler 
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_LLIM_RADIO_LINK_FAILURE_IND
 ****************************************************************************/
void uecc_fsm_rcr_ue_release_notification_ongoing_rlf_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_INFO, "Radio Link Failure Triggered.");
    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED, "p_api_data = %p", p_api_data);        

    /* Post event to RCR FSM for future processing */
    uecc_event_queue_push_event(
        UECC_EV_LLIM_SRB_DATA_STATUS_IND,
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*RLF_fix Stop*/

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_rrm_release_notification_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rcr_rrm_release_notification_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    ue_context_info_list_node_t *p_ue_context_info_list_node = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Send RRC_RRM_UE_RELEASE_REQ request to RRM */
    if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_release_req( p_ue_context ))
    {
        RRC_UECC_RCR_FSM_TRACE(RRC_ERROR,
                "RRC_RRM_UE_RELEASE_REQ delivery fail.");
        /* Switch to the next step
         *  - simulate receiving of UECC_RCR_EVENT_RRM_UE_RELEASE_RESP message
         * Post event to RCR FSM for future processing */

        uecc_event_queue_push_event(
                UECC_EV_RRM_UE_RELEASE_RESP,
                p_ue_context,
                PNULL);

    }

    /* Setting the ue_context_release flag to TRUE for the UE released */
    p_ue_context_info_list_node = (ue_context_info_list_node_t*)
        list_first_node(&p_ue_context->p_gb_context->ue_context_info_list);

    while(p_ue_context_info_list_node)
    {
        if(p_ue_context_info_list_node->ue_ctx_info.ue_index ==
                    p_ue_context->ue_index)
        {
            if(RRC_FALSE == p_ue_context_info_list_node->
                                ue_ctx_info.ue_context_released)
            {
                p_ue_context_info_list_node->ue_ctx_info.
                        ue_context_released = RRC_TRUE;
                RRC_UECC_RCR_FSM_TRACE(RRC_INFO,
                     "ue_context_released flag set to TRUE for ue_index:%d",
                     p_ue_context->ue_index);
                break;
            }
        }
        p_ue_context_info_list_node = (ue_context_info_list_node_t*)
              list_next_node((const list_node_t *)&(p_ue_context_info_list_node->node));
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_rrm_ue_release_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_RRM_UE_RELEASE_RESP from RRM
 ****************************************************************************/
void uecc_fsm_rcr_rrm_ue_release_resp_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    rrc_rrm_ue_release_resp_t *p_rrc_rrm_ue_release_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Get pointer on message */
    if (PNULL != p_api_data)
    {
        p_rrc_rrm_ue_release_resp = (rrc_rrm_ue_release_resp_t*)p_api_data;

        if ( RRC_SUCCESS != p_rrc_rrm_ue_release_resp->response)
        {
            RRC_UECC_RCR_FSM_TRACE(RRC_WARNING,
                "UECC_RCR_EVENT_RRM_UE_RELEASE_RESP failed.");
        }
    }

#ifdef ENDC_ENABLED
    /*EN-DC_changes_start*/
    if ( RRC_FALSE == p_ue_context->sgnbInitiatedSgnbReleaseFlag )
    {
        fill_sgnb_release_cause(&p_ue_context->sgnb_cause,&p_ue_context->share_data.uecc_rcr_data.u.cause);
        if((RRC_NULL < 
                    rrc_uecc_count_scg_bearer(p_ue_context)) 
                && (p_ue_context->x2apUecontextReleased != RRC_TRUE))
        {
            p_ue_context->partial_sgnb_rab_release = RRC_TRUE;
            UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_sgnb_release_ongoing, PNULL);
        }
        else
        {
            /* Switch to the LLIM wait response state */
            UECC_FSM_SET_CHILD_STATE(
                    p_ue_context,
                    &uecc_fsm_rcr_llim_delete_ue_ongoing, PNULL);
        }
    }
    else
    {
    /* Switch to the LLIM wait response state */
    UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_rcr_llim_delete_ue_ongoing, PNULL);
    }
    /*EN-DC_changes_end*/
#else
    /* Switch to the LLIM wait response state */
    UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_rcr_llim_delete_ue_ongoing, PNULL);
#endif

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_rrm_release_notification_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rcr_rrm_release_notification_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_RCR_FSM_TRACE(RRC_BRIEF, "Time out.");

    /* Restart the Procedural timer */
    if ( RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context,
            UECC_UE_RCR_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index) )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[UECC_UE_RCR_TIMER]"
                "Timer Start Failed");
    }

    /* Post event to RCR FSM for future processing */
    uecc_event_queue_push_event(
        UECC_EV_RRM_UE_RELEASE_RESP,
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_llim_delete_ue_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rcr_llim_delete_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Send UE entity delete request to LLIM */
    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_delete_ue_entity_req( p_ue_context ))
    {
        RRC_UECC_RCR_FSM_TRACE(RRC_ERROR,
            "UECC_LLIM_DELETE_UE_ENTITY_REQ delivery fail.");
        /* Switch to the next step
         *  - simulate receiving of UECC_RCR_EVENT_LLIM_DELETE_UE_RESP message
         * Post event to RCR FSM for future processing */

        uecc_event_queue_push_event(
            UECC_EV_LLIM_DELETE_UE_ENTITY_RESP,
            p_ue_context,
            PNULL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_llim_delete_ue_entity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                        UECC_EV_LLIM_DELETE_UE_ENTITY_RESP from LLIM
 ****************************************************************************/
void uecc_fsm_rcr_llim_delete_ue_entity_resp_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    /* CR 101 changes start*/
    U8 counter = RRC_NULL;
    U8 qci_value = RRC_NULL;
    /*SPR 21160  start*/
    U16 resp_counter = RRC_NULL;
    /*SPR 21160   stop*/
    s1ap_Cause  s1apCause;
    /* SPR 23518/SES-342 Fix Start */
    rrc_bool_et is_cause_valid = RRC_TRUE;
    /* SPR 23518/SES-342 Fix End */
    /* CR 101 changes stop*/

    rrc_uecc_llim_delete_ue_entity_resp_t* p_llim_delete_ue_entity_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Get pointer on message */
    if (PNULL != p_api_data)
    {
        p_llim_delete_ue_entity_resp =
            (rrc_uecc_llim_delete_ue_entity_resp_t*)p_api_data;

        if ( RRC_LLIM_RESPONSE_CODE_LAST <=
                p_llim_delete_ue_entity_resp->response_code )
        {
            RRC_UECC_RCR_FSM_TRACE(RRC_WARNING,
                "[UECC_RCR_EVENT_LLIM_DELETE_UE_RESP] Invalid Response=%u",
                p_llim_delete_ue_entity_resp->response_code);
        }
        else if ( RRC_LLIM_SUCCESS != p_llim_delete_ue_entity_resp->response_code )
        {
            RRC_UECC_RCR_FSM_TRACE(RRC_WARNING,
                "[UECC_RCR_EVENT_LLIM_DELETE_UE_RESP] failed [%u]",
                    p_llim_delete_ue_entity_resp->response_code);
        }
    }
    
    /* CR 101 changes start*/
    memset_wrapper(&s1apCause, 0, sizeof(s1apCause));

    if (p_ue_context->ue_ctx_rel_req_cause.t != RRC_NULL)
    {
        s1apCause = p_ue_context->ue_ctx_rel_req_cause;
    }
    else
    {
        s1apCause = p_ue_context->share_data.uecc_rcr_data.u.cause;
    }

    for (counter = 0; counter < MAX_ERAB_COUNT; counter++)
    {
        if (PNULL != p_ue_context->p_e_rab_list[counter])
        {
            qci_value = p_ue_context->p_e_rab_list[counter]->
                e_RABlevelQoSParameters.qCI;

           /* SPR_7613 Start */ 
            if(p_llim_delete_ue_entity_resp!=PNULL)
            {
            for (resp_counter = 0;
                    resp_counter < p_llim_delete_ue_entity_resp->
                    delete_lc_status_counter;
                    resp_counter++)
            {
                if (p_ue_context->p_e_rab_list[counter]->
                        drb_config.logical_channel_identity ==
                        p_llim_delete_ue_entity_resp->
                        delete_lc_status[resp_counter].lch_id)
                {
                    if (MAC_DELETE_LC_ACTIVE == p_llim_delete_ue_entity_resp->
                            delete_lc_status[resp_counter].status )
                    {
                        /* checking the cause for ue_ctx_rel_req if ue release is enb
                         * initiated otherwise cause for ue_ctx_rel_cmd is checked and
                         * counter is updated for active erabs(only) with appropriate cause or if
                         * RESET is received or sent then cause is T_s1ap_Cause_transport
                         * is sent so counter will be updated then also*/
                        /* SPR 23518/SES-342 Fix Start */
                        if(p_ue_context->ue_ctx_rel_req_cause.t != RRC_NULL)
                        {
                            if (((s1apCause.t == T_s1ap_Cause_radioNetwork) &&
                                        ((s1apCause.u.radioNetwork == s1ap_user_inactivity) ||
                                         (s1apCause.u.radioNetwork == s1ap_cs_fallback_triggered) ||
                                         (s1apCause.u.radioNetwork == s1ap_ue_not_available_for_ps_service) ||
                                         (s1apCause.u.radioNetwork == s1ap_redirection_towards_1xRTT) ||
                                         (s1apCause.u.radioNetwork == s1ap_interrat_redirection))) ||
                                    ((s1apCause.t == T_s1ap_Cause_nas) &&
                                     (s1apCause.u.nas == s1ap_csg_subscription_expiry)))
                            {
                                is_cause_valid = RRC_FALSE;
                                RRC_UECC_RCR_FSM_TRACE(RRC_INFO,
                                        "[UECC_RCR_EVENT_LLIM_DELETE_UE_RESP] eNB initiated UE context Release ongoing,"
                                        "is_cause_valid set to %d", is_cause_valid);
                            }
                        }
                        else
                        {
                            if( ((s1apCause.t == T_s1ap_Cause_nas) &&
                                        ((s1apCause.u.nas == s1ap_normal_release) ||
                                         (s1apCause.u.nas == s1ap_detach))) ||
                                    ((s1apCause.t == T_s1ap_Cause_radioNetwork) &&
                                     ((s1apCause.u.radioNetwork == s1ap_handover_cancelled) ||
                                      (s1apCause.u.radioNetwork == s1ap_redirection_towards_1xRTT) ||
                                      (s1apCause.u.radioNetwork == s1ap_successful_handover) ||
                                      (s1apCause.u.radioNetwork == s1ap_interrat_redirection) ||
                                      (s1apCause.u.radioNetwork == s1ap_s1_intra_system_handover_triggered))))
                            {
                                is_cause_valid = RRC_FALSE;
                                RRC_UECC_RCR_FSM_TRACE(RRC_INFO,
                                        "[UECC_RCR_EVENT_LLIM_DELETE_UE_RESP] MME initiated UE context Release ongoing,"
                                        "is_cause_valid set to %d", is_cause_valid);
                            }
                        }
                        /* SPR 23518/SES-342 Fix End */

                        if (is_cause_valid == RRC_TRUE)
                        {
                            /* SPR 13441 Fix Start */
                            if ( (RRC_MIN_QCI_GBR_LIMIT <= qci_value) &&
                                    (RRC_MAX_QCI_NON_GBR_LIMIT >= qci_value) )
                            {
                            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                                    rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                                    rrc_erab_related_stats.erab_release_stats.
                                    erab_relActNbr_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                                    RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

                            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                                    rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                                    rrc_erab_related_stats.erab_release_stats.
                                    erab_relActNbr_sum, RRC_UPDATED_QCI_INDEX(qci_value), 
                                    RRC_MAX_QCI_INDEX);
                            }
                            /* SPR 13441 Fix End */
                        }
                    }
                }
            }
          }
          /* SPR_7613 Stop*/
        }
    }
    /* CR 101 changes stop*/

    /* Switch off RCR FSM with success always */
    uecc_fsm_finish_activity_success(p_ue_context, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_llim_delete_ue_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rcr_llim_delete_ue_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_RCR_FSM_TRACE(RRC_BRIEF, "Time out.");

    /* Post event to RCR FSM for future processing */
    uecc_event_queue_push_event(
        UECC_EV_LLIM_DELETE_UE_ENTITY_RESP,
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_ue_release_wait_for_s1ap_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rcr_ue_release_wait_for_s1ap_init_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    /* SPR 20627 Fix Start */
    p_ue_context->ue_ctx_del_status.is_rrc_resp_expected = RRC_NULL;
    /* SPR 20627 Fix Stop */
    /*Stop ReleaseTimer because in this moment LowLeyer release is done*/
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_RCR_TIMER, &p_ue_context->timers_data);

    if(RRC_TRUE == p_ue_context->share_data.uecc_rcr_data.release_performed)
    {
        /* SPR 21052 Fix Start */
        /*Lines Deleted*/
        /* SPR 21052 Fix End */
        uecc_fsm_finish_activity_success(p_ue_context, PNULL);
    }
    /* SPR 16750 Fix Start */
    else if (RRC_TRUE == 
                p_ue_context->share_data.uecc_rcr_data.s1_error_ind_recv)
    {
        uecc_fsm_finish_activity_success(p_ue_context, PNULL);
    }
    /* SPR 16750 Fix Stop */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_rrm_ue_release_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_UE_RELEASE_REQ
 ****************************************************************************/
void uecc_fsm_rcr_rrm_ue_release_req_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    /* RRM FIX Start */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_MME_GUARD_TIMER, &p_ue_context->timers_data);
    /* RRM FIX Stop */
    
    /* BUG_8169_start */
    RRC_UECC_RCR_FSM_TRACE(RRC_ERROR,
            "Sending RRC_S1AP_UE_CTX_RELEASE_COMP_RESP to MME");
    /* Send RRC_S1AP_UE_CTX_RELEASE_COMP_RESP message to S1AP */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_ue_ctx_release_comp_resp( p_ue_context ))
    {
        RRC_UECC_RCR_FSM_TRACE(RRC_ERROR,
            "[S1AP_UE_CTX_RELEASE_COMP_RESP] Delivery failure");
    }
    
    /* BUG_8169_stop */

    /* Set flag */
    p_ue_context->share_data.uecc_rcr_data.release_performed = TRUE;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_rrm_ue_release_req_wait_for_s1ap_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_UE_RELEASE_REQ
 ****************************************************************************/
void uecc_fsm_rcr_rrm_ue_release_req_wait_for_s1ap_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    /* RRM FIX Start */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_MME_GUARD_TIMER, &p_ue_context->timers_data);
    /* RRM FIX Stop */
/* Send RRC_S1AP_UE_CTX_RELEASE_COMP_RESP message to S1AP */
    if (RRC_SUCCESS !=
        uecc_s1ap_build_and_send_ue_ctx_release_comp_resp( p_ue_context ))
    {
        RRC_UECC_RCR_FSM_TRACE(RRC_ERROR,
            "[S1AP_UE_CTX_RELEASE_COMP_RESP] Delivery failure");
    }

    /* Switch off RCR FSM with success always */
    uecc_fsm_finish_activity_success(p_ue_context, PNULL);
    /*SPR 21947 Start */ 
    if (PNULL != p_api_data)
    {
        rrc_mem_free(p_api_data);
        p_api_data = PNULL;
    }
    /*SPR 21947 Stop */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcr_ue_release_wait_for_s1ap_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rcr_ue_release_wait_for_s1ap_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    /* SPR 20627 Fix Start */
    /* Code Removed */
    /* SPR 20627 Fix End */



    /* Send ue_release_msg */
    if (RRC_SUCCESS != uecc_s1ap_build_and_send_ue_release_msg_req(
            p_ue_context->s1ap_ue_id,
            RRC_TRUE,
            p_ue_context))
    {
        RRC_UECC_RCR_FSM_TRACE(RRC_WARNING,
            "ue_release_msg delivery failure");
    }
/* Switch off RCR FSM with success always */
    uecc_fsm_finish_activity_success(p_ue_context, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_connection_release_procedure_constructor
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for UE RRM Connection Release procedure
 ****************************************************************************/
void uecc_fsm_ue_rrm_connection_release_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Init flag */
    p_ue_context->share_data.uecc_rcr_data.release_performed = RRC_FALSE;

    /*FIXES*/
    p_ue_context->share_data.uecc_rcr_data.rrm_rel_req_not_required_flag =
                RRC_TRUE;    

    /* SPR 18552 Fix Start */
    RRC_SET_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
    /* SPR 18552 Fix Stop */ 

    /* Start timer */
    if ( RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context,
            UECC_UE_RCR_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index) )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[UECC_UE_RCR_TIMER]"
                "Timer Start Failed");
    }

    /* Start timer */
    if ( RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context,
            UECC_UE_MME_GUARD_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index) )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[UECC_UE_MME_GUARD_TIMER]"
                "Timer Start Failed");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_connection_release_setup_failure_procedure_finish_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Success handler for UE RRM Connection Release procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_rrm_connection_release_setup_failure_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
    /* Stop timers */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_RCR_TIMER, &p_ue_context->timers_data);
  /* SPR 20704 Fix Start */
    RRC_UECC_UPDATE_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
    /* SPR 20704 Fix End */
    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_UE_RELEASED_INT,
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_connection_release_mme_guard_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                   UECC_EV_MME_GUARD_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_ue_rrm_connection_release_mme_guard_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    
    /*SPR 20901 Start*/
    /* Code Removed */
    /*SPR 20901 Stop*/

    /* Send ue_release_msg */
    if (RRC_SUCCESS != uecc_s1ap_build_and_send_ue_release_msg_req(
            p_ue_context->s1ap_ue_id,
            RRC_TRUE,
            p_ue_context))
    {
        RRC_UECC_RCR_FSM_TRACE(RRC_WARNING,
            "ue_release_msg delivery failure");
    }

    /* Set flag */
    p_ue_context->share_data.uecc_rcr_data.release_performed = RRC_TRUE;

    RRC_UECC_RCR_FSM_TRACE(RRC_WARNING,
        "%s:MME_GUARD_TIMER expired before RELEASE_TIMER",
        __FUNCTION__);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_rrm_connection_release_no_mme_id_procedure_constructor
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for UE Connection_no_mme_id_Release procedure
 ****************************************************************************/
void uecc_fsm_ue_rrm_connection_release_no_mme_id_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);     

    /* SPR 18552 Fix Start */
    RRC_SET_UE_CTX_DEL_STATUS(p_ue_context->ue_ctx_del_status);
    /* SPR 18552 Fix Stop */ 

    /* Send ue_release_msg */
    if (RRC_SUCCESS != uecc_s1ap_build_and_send_ue_release_msg_req(
            p_ue_context->s1ap_ue_id,
            RRC_TRUE,
            p_ue_context))
    {
        RRC_UECC_RCR_FSM_TRACE(RRC_WARNING,
            "ue_release_msg delivery failure");
    }

    /* Start timer */
    if ( RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context,
            UECC_UE_RCR_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index) )
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,RRC_WARNING,
                "[UECC_UE_RCR_TIMER]"
                "Timer Start Failed");
    }

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

    RRC_UECC_RCR_FSM_TRACE(RRC_INFO,
            "Send SGNB RELEASE REQUEST to SgNB \n");

    if ( ( ( p_ue_context->p_gb_context->
        p_p_csc_context[p_ue_context->cell_index]->del_cell_ongoing)) ||
       ( p_ue_context->p_gb_context->
        p_p_csc_context[p_ue_context->cell_index]->is_cell_stop_ongoing))
    {
        /* Send UE CONTEXT RELEASE to SgNB */
        if (RRC_SUCCESS !=
                uecc_x2ap_build_and_send_ue_context_release(p_ue_context
                    ))
        {
            RRC_UECC_RCR_FSM_TRACE(RRC_WARNING,
                    "[%s]uecc_x2ap_build_and_send_ue_context_release"
                    " failed !!",__FUNCTION__);
        }
        /* Send X2AP UE Context Released True */
        p_ue_context->x2apUecontextReleased = RRC_TRUE;
    }
    else
    {
        if (RRC_SUCCESS != 
                build_and_send_sgnb_release_request(p_ue_context
                    ))
        {
            RRC_UECC_RCR_FSM_TRACE(RRC_WARNING,
                    "[%s]build_and_send_sgnb_release_request failed !!");
        }

    }


}
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
void uecc_fsm_sgnb_release_ack_handler
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

    RRC_UECC_RCR_FSM_TRACE(RRC_INFO,
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

    do {
        /* Send UE CONTEXT RELEASE to SgNB */ 
        if (RRC_SUCCESS != 
                uecc_x2ap_build_and_send_ue_context_release(p_ue_context
                    ))
        {
            RRC_UECC_RCR_FSM_TRACE(RRC_WARNING,
                    "[%s]uecc_x2ap_build_and_send_ue_context_release"
                    " failed !!",__FUNCTION__);
            break;
        }

        /* Send X2AP UE Context Released True */
        p_ue_context->x2apUecontextReleased = RRC_TRUE;
    }while(0);
    
    /* Switch to the LLIM wait response state */
    UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_rcr_llim_delete_ue_ongoing, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
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
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_INFO,
            "SGNB RELEASE REQUEST ACK TIMER_EXPIRY\n");
        p_ue_context->rel_proc_fail = RRC_FALSE;

    /*Trigger sgnb UE context release after timer expiry*/
    /* coverity_277501_fix_start */
    if( RRC_FAILURE == uecc_fsm_ue_context_release_handler(p_ue_context))
    {
        RRC_UECC_RCR_FSM_TRACE(RRC_INFO,
                "uecc_fsm_ue_context_release_handler failed\n");
    }
    /* coverity_277501_fix_end */

    uecc_fsm_finish_activity_success(p_ue_context, PNULL);

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
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCR_FSM_TRACE(RRC_INFO,
            "Received Error Ind in UE Release, go ahead with release\n");

    /* Set X2AP UE Context Released True */
    p_ue_context->x2apUecontextReleased = RRC_TRUE;
    
    /* Switch to the LLIM wait response state */
    UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_rcr_llim_delete_ue_ongoing, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*EN-DC_changes_end*/
#endif
