/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_rcsfsm.c,v 1.26 2010/12/01 13:02:50 gur19827 Exp $
 ****************************************************************************
 *
 *  File Description : UECC RCS FSM functions implementation
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * 21.10.2009   AndriyT -   initial creation
 * $Log: uecc_rcsfsm.c,v $
 * Revision 1.26  2010/12/01 13:02:50  gur19827
 * Merged Inter RAT HO
 *
 * Revision 1.25  2010/10/13 07:03:36  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.7.4.5  2010/10/08 12:36:40  gur21006
 * TDD changes merged
 *
 * Revision 1.7.4.4  2010/09/13 13:04:01  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.7.4.3  2010/08/05 08:07:13  gur15697
 * merging round 2
 *
 * Revision 1.7.4.2  2010/07/21 08:34:41  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.7.4.1  2010/06/22 05:31:46  gur22047
 * Internal Interface update
 *
 * Revision 1.7  2010/05/14 09:36:58  gur19827
 * checklist,warning_removal,dst mod id,tdd,phy cell id added
 *
 * Revision 1.6  2010/04/13 11:23:58  gur10248
 * SRB2 changes
 *
 * Revision 1.5  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.4  2010/03/17 12:51:03  gur10248
 * CR201_changes
 *
 * Revision 1.3  2010/02/18 04:19:02  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.47  2009/12/22 16:39:44  gur18569
 * commented field name initialization in fsm activity structures
 *
 * Revision 1.1.2.46  2009/12/11 10:38:15  ukr16032
 * UECC SSIT - LLIM creating UE response code processing updated
 *
 * Revision 1.1.2.45  2009/12/11 09:33:53  ukr18880
 * stop RCS timer
 *
 * Revision 1.1.2.44  2009/12/09 15:36:26  ukr16032
 * UECC - inspection log items PN_4, SG_1, SG_6, AK_1, AK_12, AK_25, AK_27 fixed
 *
 * Revision 1.1.2.43  2009/12/08 16:18:10  ukr15916
 * LG_11 fixed
 *
 * Revision 1.1.2.42  2009/12/08 16:05:18  ukr15916
 * LG_14 fixed
 *
 * Revision 1.1.2.41  2009/12/07 15:28:44  ukr16032
 * UECC - inspection log items LG_1, LG_2, LG_3, LG_5, LG_8, LG_9, LG_10, LG_12,
 *  LG_13, LG_15, LG_16, LG_17 fixed
 *
 * Revision 1.1.2.40  2009/12/07 11:49:45  ukr18880
 * bugs fixed in UECC
 *
 * Revision 1.1.2.39  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.37.2.20  2009/11/24 12:12:51  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.2.37.2.19  2009/11/24 11:32:35  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.2.37.2.18.2.4  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.2.37.2.18.2.3  2009/11/20 15:38:46  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.37.2.18.2.2  2009/11/20 12:16:24  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.37.2.18.2.1  2009/11/19 18:56:34  ukr16032
 * UECC RRM updated according to the last interface changes.
 *
 * Revision 1.1.2.37.2.18  2009/11/19 09:52:17  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.2.37.2.17  2009/11/16 11:02:49  ukr18880
 * 80 lines
 *
 * Revision 1.1.2.37.2.16  2009/11/14 16:29:54  ukr16032
 * UECC UE Capability transfer implemented
 * UECC ICSP - security capability checking implemented
 *
 * Revision 1.1.2.37.2.15  2009/11/13 13:14:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.2.37.2.14  2009/11/13 12:36:55  ukr18880
 * *** empty log message ***
 *
 * Revision 1.1.2.37.2.13  2009/11/13 12:23:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.2.37.2.12  2009/11/11 17:47:41  ukr16032
 * UECC
 *  -- S1AP UL NAS message generation updated
 *  -- FSM - review
 *  -- FSM - reset and rrm_release handling added to wait_for_s1_connection and
 * ue_connected states
 *  -- S1AP_RESET_MSG_IND message added
 *
 * Revision 1.1.2.37.2.10  2009/11/10 20:31:19  ukr18880
 * UECC statistic was implemented
 *
 * Revision 1.1.2.37.2.9  2009/11/10 20:02:17  ukr16032
 * UECC S1AP_STATUS_MSG_IND handling added
 *
 * Revision 1.1.2.37.2.8  2009/11/09 14:50:50  ukr16032
 * UECC - IS1AP Initial UE message filling reviewed
 *
 * Revision 1.1.2.37.2.7  2009/11/04 12:26:29  ukr15916
 * UECC - UE connection setup procedure updated.
 * New flags added to UE ctx for configuration IEs update necessity notification
 *
 * Revision 1.1.2.37.2.6  2009/11/03 17:37:20  ukr15916
 * UECC - UE connection setup procedure updated.
 *
 * Revision 1.1.2.37.2.5  2009/11/03 11:43:37  ukr18880
 * UECC FSM engine was updated
 *
 * Revision 1.1.2.37.2.4  2009/11/02 15:55:10  ukr15916
 * UECC - UE connection setup procedure updated.
 *
 * Revision 1.1.2.37.2.3  2009/11/02 13:48:46  ukr15916
 * UECC - default values checked.
 * UE connection setup procedure update started.
 *
 * Revision 1.1.2.37.2.2  2009/10/27 13:56:38  ukr18880
 * UECC FSM failure processing was optimized
 *
 * Revision 1.1.2.37.2.1  2009/10/27 09:50:13  ukr15916
 * UECC added to Rel 1.0
 *
 * Revision 1.1.2.37.6.14  2009/10/27 07:59:20  ukr18880
 * UECC FSM failure processing was updated
 *
 * Revision 1.1.2.37.6.13  2009/10/26 16:14:46  ukr18880
 * UECC FSM failure processing engine initial creation
 *
 * Revision 1.1.2.37.6.12  2009/10/26 15:05:28  ukr18880
 * Failure processing in UECC FSM was changed from "switch" to tables
 *
 * Revision 1.1.2.37.6.11  2009/10/26 13:34:05  ukr18880
 * UECC FSM engine modification
 *
 * Revision 1.1.2.37.6.10  2009/10/26 12:58:54  ukr18880
 * UECC FSM update
 *
 * Revision 1.1.2.37.6.9  2009/10/26 12:51:19  ukr15916
 * UECC-OAMH interface updated.
 *
 * Revision 1.1.2.37.6.8  2009/10/23 14:13:44  ukr18880
 * UECC FSM update
 *
 * Revision 1.1.2.37.6.7  2009/10/23 12:35:41  ukr18880
 * update of UECC FSM
 *
 * Revision 1.1.2.37.6.6  2009/10/23 08:55:47  ukr18880
 * UECC RCR FSM is now worked on new engine
 *
 * Revision 1.1.2.37.6.5  2009/10/22 13:48:24  ukr18880
 * UECC timer handling was changed
 *
 * Revision 1.1.2.37.6.4  2009/10/22 13:10:04  ukr18880
 * handlers calls logic was changed in UECC FSM engine
 *
 * Revision 1.1.2.37.6.3  2009/10/22 08:38:56  ukr18880
 * UECC RCS FSM functional tests are passed successfully now
 *
 *
 ****************************************************************************/


/****************************************************************************
* Header File Includes
***************************************************************************/
#include "rrc_rrm_intf.h"
#include "uecc_logger.h"

#include "uecc_common.h"
#include "uecc_ue_timer_utils.h"
#include "uecc_rcsfsm.h"
#include "uecc_rrm.h"
#include "uecc_llim.h"
#include "uecc_s1ap.h"
#include "uecc_defaults.h"
#include "uecc_mainfsm.h"
#include "rrc_phy_intf.h"
#include "uecc_fsm_engine.h"
#include "rrc_shared_stats.h"
#include "rrc_msg_mgmt.h"
#include "uecc_llim.h"
/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define RRC_UECC_RCS_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] "format,\
        p_ue_context->ue_index,\
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
        ##__VA_ARGS__)



/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/


static void uecc_fsm_ue_connection_setup_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_connection_setup_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_ue_connection_setup_procedure_finish_fail(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
/*SPR 8708 Fix Start*/
/*SPR 8708 Fix Stop*/
/*SPR 8708 Fix Start*/
static void uecc_fsm_rcs_ue_conn_s1ap_allocate_mme_resp_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);
/*SPR 8708 Fix Stop*/
static void uecc_fsm_rcs_rrm_ue_admission_resp_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

static void uecc_fsm_rcs_rrm_ue_admission_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

/* State UECC_RCS_LLIM_CREATE_UE_ONGOING */
static void uecc_fsm_rcs_llim_create_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rcs_llim_create_ue_entity_resp_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

static void uecc_fsm_rcs_llim_create_ue_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

/* State UECC_RCS_RRC_CONNECTION_SETUP_ONGOING */
static void uecc_fsm_rcs_rrc_connection_setup_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);

static void uecc_fsm_rcs_rrc_connection_setup_complete_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

static void uecc_fsm_rcs_rrc_connection_setup_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

/* State UECC_RCS_FAIL_W_FOR_RESPONSE_FROM_RRC */
static void uecc_fsm_rcs_llim_delete_ue_entity_response_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

static void uecc_fsm_rcs_fail_w_for_response_from_rrc_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,          /* UECC UE context */
    void                *p_api_data             /* Input API buffer */
);

/* Failure processing functions */

static void uecc_fsm_rcs_fail_action_send_llim_ue_conn_rej_req(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

static void uecc_fsm_rcs_fail_action_send_rrm_cnf(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

static void uecc_fsm_rcs_fail_action_delete_ue(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

static void uecc_fsm_rcs_fail_action_last(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);

static rrc_return_t uecc_fsm_rcs_process_rrm_ue_admission_resp_data(
    uecc_ue_context_t*           p_ue_context,  /* UECC UE context */
    rrc_rrm_ue_admission_resp_t* p_rrc_rrm_ue_admission_resp
);
static void uecc_fsm_ue_connection_setup_csc_cell_delete(
    uecc_ue_context_t*  p_ue_context,          /* UECC UE context */
    void*               p_api                  /* Input API buffer */
);
void uecc_fsm_rcs_s1ap_allocate_mme_resp_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);

void uecc_fsm_rcs_rrc_connection_setup_ongoing_overload_ind_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
/*SPR 13758 Fix Start*/

void uecc_fsm_rcs_s1ap_ue_release_req_handler(
     uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
/*SPR 13758 Fix Stop*/
/*CSR:00053548 changes start*/
void uecc_fsm_rcs_rrm_ue_conn_release_ind_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);

void uecc_fsm_rcs_rrm_ue_conn_release_ind_w_for_del_resp_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
void uecc_fsm_rcs_rrm_ue_conn_release_ind_conn_setup_ongoing_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
static void uecc_fsm_rcs_fail_action_rrc_connection_release(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    rrc_bool_et*        p_break
);
/*CSR:00053548 changes stop*/
/* SPR 18649 Fix Start */
static void uecc_fsm_rcs_rrm_ue_admission_ongoing_reset_msg_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
static void uecc_fsm_rcs_llim_create_ue_ongoing_reset_msg_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
static void uecc_fsm_rcs_rrc_connection_setup_ongoing_reset_msg_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
/* SPR 18649 Fix Stop */
/* SPR 15790 Fix Start */
void uecc_fsm_rcs_csc_cell_del_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
);
void uecc_fsm_rcs_rrm_ue_admission_ongoing_cell_delete_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
void uecc_fsm_rcs_llim_create_ue_ongoing_cell_delete_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
);
/* SPR 15790 Fix Stop */
/* Warning Fix Start */
extern rrc_return_et uecc_csc_build_and_send_delete_all_ue_entity_resp
(
    rrc_return_et     response ,
   uecc_gb_context_t  *p_uecc_gb_context,
   rrc_cell_index_t   cell_index
);
/* Warning Fix Stop */
/****************************************************************************
 Private Variables
****************************************************************************/
static uecc_fsm_handler_record_t uecc_fsm_rcs_rrm_ue_admission_ongoing_mht[] =
{
/*SPR 8708 Fix Start*/
    { { UECC_EV_STATE_INIT              }, PNULL 		 },
    { { UECC_EV_S1AP_ALLOCATE_MME_RESP  },
	    uecc_fsm_rcs_ue_conn_s1ap_allocate_mme_resp_handler  },
/*SPR 8708 Fix Stop*/
    { { UECC_EV_RRM_UE_ADMISSION_RESP   },
            uecc_fsm_rcs_rrm_ue_admission_resp_handler                  },
    { { UECC_EV_TIMER_EXPIRY            },
            uecc_fsm_rcs_rrm_ue_admission_ongoing_timer_expiry_handler  },
/* SPR 15790 Fix Start */
     { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
           uecc_fsm_rcs_rrm_ue_admission_ongoing_cell_delete_handler   },
    /* SPR 15790 Fix Stop */
    /* SPR 18649 Fix Start */
    { { UECC_EV_S1AP_RESET_MSG_IND   },
        uecc_fsm_rcs_rrm_ue_admission_ongoing_reset_msg_handler  },
    /* SPR 18649 Fix Stop */
    { { UECC_LAST_ID                    },  PNULL                       }
};

static uecc_fsm_handler_record_t uecc_fsm_rcs_llim_create_ue_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                  },
            uecc_fsm_rcs_llim_create_ue_ongoing_init_handler            },
    { { UECC_EV_LLIM_CREATE_UE_ENTITY_RESP  },
            uecc_fsm_rcs_llim_create_ue_entity_resp_handler             },
    { { UECC_EV_TIMER_EXPIRY                },
            uecc_fsm_rcs_llim_create_ue_ongoing_timer_expiry_handler    },
/* SPR 15790 Fix Start */
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
            uecc_fsm_rcs_llim_create_ue_ongoing_cell_delete_handler  },
/* SPR 15790 FIX Stop */
/*CSR:00053548 changes start*/
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND   },
            uecc_fsm_rcs_rrm_ue_conn_release_ind_handler   },
/*CSR:00053548 changes stop*/
    /* SPR 18649 Fix Start */
    { { UECC_EV_S1AP_RESET_MSG_IND   },
            uecc_fsm_rcs_llim_create_ue_ongoing_reset_msg_handler  },
    /* SPR 18649 Fix Stop */

    { { UECC_LAST_ID                        },  PNULL                   }
};

static uecc_fsm_handler_record_t
        uecc_fsm_rcs_rrc_connection_setup_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
            uecc_fsm_rcs_rrc_connection_setup_ongoing_init_handler          },
    { { UECC_EV_RRC_CONNECTION_SETUP_COMPLETE   },
            uecc_fsm_rcs_rrc_connection_setup_complete_handler              },
    { { UECC_EV_TIMER_EXPIRY                    },
            uecc_fsm_rcs_rrc_connection_setup_ongoing_timer_expiry_handler  },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
            uecc_fsm_ue_connection_setup_csc_cell_delete  },
    { { UECC_EV_S1AP_ALLOCATE_MME_RESP   },
            uecc_fsm_rcs_s1ap_allocate_mme_resp_handler              },
    { { UECC_EV_S1AP_OVERLOAD_MSG_IND                    },
            uecc_fsm_rcs_rrc_connection_setup_ongoing_overload_ind_handler  },
/*SPR 13758 Fix Start*/
    { { UECC_EV_S1AP_UE_REL_REQ_INT		},
	    uecc_fsm_rcs_s1ap_ue_release_req_handler	},
/*SPR 13758 Fix Stop*/
/*CSR:00053548 changes start*/
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND   },
            uecc_fsm_rcs_rrm_ue_conn_release_ind_conn_setup_ongoing_handler   },
    /*CSR:00053548 changes stop*/
    /* SPR 18649 Fix Start */
    { { UECC_EV_S1AP_RESET_MSG_IND   },
        uecc_fsm_rcs_rrc_connection_setup_ongoing_reset_msg_handler  },
    /* SPR 18649 Fix Stop */
    { { UECC_LAST_ID                            },  PNULL                   }
};

static uecc_fsm_handler_record_t
        uecc_fsm_rcs_fail_w_for_response_from_rrc_mht[] =
{
    { { UECC_EV_LLIM_DELETE_UE_ENTITY_RESP  },
            uecc_fsm_rcs_llim_delete_ue_entity_response_handler             },
    { { UECC_EV_TIMER_EXPIRY                },
            uecc_fsm_rcs_fail_w_for_response_from_rrc_timer_expiry_handler  },
    { { UECC_EV_CSC_DELETE_ALL_UE_REQ            },
            uecc_fsm_ue_connection_setup_csc_cell_delete  },
/*CSR:00053548 changes start*/
    { { UECC_EV_RRM_UE_CONNECTION_RELEASE_IND   },
            uecc_fsm_rcs_rrm_ue_conn_release_ind_w_for_del_resp_handler   },
    /*CSR:00053548 changes stop*/
    /* SPR 18649 Fix Start */
    { { UECC_EV_S1AP_RESET_MSG_IND   },
        uecc_fsm_rcs_rrm_ue_conn_release_ind_w_for_del_resp_handler   },
    /* SPR 18649 Fix Stop */
    { { UECC_LAST_ID                        },  PNULL                       }
};

/* Activity states */
static uecc_fsm_activity_state_t uecc_fsm_rcs_rrm_ue_admission_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_RCS_RRM_UE_ADMISSION_ONGOING",
   /* .s_id           =  */ (S8*)"RCS_RRM",
   /* .is_need_init   =  */ RRC_FALSE,	/*SPR 8708 Fix Start*/
					/*SPR 8708 Fix Stop*/
   /* .p_mht          =  */ uecc_fsm_rcs_rrm_ue_admission_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_rcs_llim_create_ue_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_RCS_LLIM_CREATE_UE_ONGOING",
   /* .s_id           =  */ (S8*)"RCS_LLIM",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_rcs_llim_create_ue_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_rcs_rrc_connection_setup_ongoing =
{
   /* .s_name         =  */ (S8*)"UECC_RCS_RRC_CONNECTION_SETUP_ONGOING",
   /* .s_id           =  */ (S8*)"RCS_RRC",
   /* .is_need_init   =  */ RRC_TRUE,
   /* .p_mht          =  */ uecc_fsm_rcs_rrc_connection_setup_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_rcs_fail_w_for_response_from_rrc =
{
   /* .s_name         =  */ (S8*)"UECC_RCS_FAIL_W_FOR_RESPONSE_FROM_RRC",
   /* .s_id           =  */ (S8*)"RCS_FAIL",
   /* .is_need_init   =  */ RRC_FALSE,
   /* .p_mht          =  */ uecc_fsm_rcs_fail_w_for_response_from_rrc_mht
};

/* Activity */
static uecc_fsm_activity_t uecc_fsm_rrc_connection_setup_activity =
{
  /*  .s_name             = */(S8*)"RRC Connection Setup",
   /* .s_id               = */(S8*)"UE_RCS_A",
   /* .p_first_state      = */&uecc_fsm_rcs_rrm_ue_admission_ongoing
};

static uecc_fsm_activity_t*
                    uecc_fsm_ue_connection_setup_procedure_activity_list[] =
{
    &uecc_fsm_rrc_connection_setup_activity,
    PNULL
};

/* Procedure */
uecc_fsm_procedure_t uecc_fsm_ue_connection_setup_procedure =
{
    /*.s_name             = */(S8*)"UE Connection Setup",
    /*.s_id               = */(S8*)"UE_CS_P",
    /*.constructor_f      = */uecc_fsm_ue_connection_setup_procedure_constructor,
    /*.finish_success_f   = */uecc_fsm_ue_connection_setup_procedure_finish_success,
    /*.finish_fail_f      = */uecc_fsm_ue_connection_setup_procedure_finish_fail,
    /*.activity_list      = */uecc_fsm_ue_connection_setup_procedure_activity_list,
    /*.p_mht              = */PNULL
};


/* Fail actions */
/* RCS failure actions lists */
static const uecc_fsm_fail_handler_f uecc_fsm_rcs_fail_type1_actions[] =
{
    uecc_fsm_rcs_fail_action_send_llim_ue_conn_rej_req,
    uecc_fsm_rcs_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_rcs_fail_type2_actions[] =
{
    uecc_fsm_rcs_fail_action_send_rrm_cnf,
    uecc_fsm_rcs_fail_action_send_llim_ue_conn_rej_req,
    uecc_fsm_rcs_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_rcs_fail_type3_actions[] =
{
    uecc_fsm_rcs_fail_action_delete_ue,
    uecc_fsm_rcs_fail_action_send_rrm_cnf,
    uecc_fsm_rcs_fail_action_send_llim_ue_conn_rej_req,
    uecc_fsm_rcs_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_rcs_fail_type4_actions[] =
{
    uecc_fsm_rcs_fail_action_delete_ue,
    uecc_fsm_rcs_fail_action_send_rrm_cnf,
    uecc_fsm_rcs_fail_action_last,
    PNULL
};

/*CSR:00053548 changes start*/
static const uecc_fsm_fail_handler_f uecc_fsm_rcs_fail_type5_actions[] =
{
    uecc_fsm_rcs_fail_action_rrc_connection_release,
    uecc_fsm_rcs_fail_action_delete_ue,
    uecc_fsm_rcs_fail_action_send_rrm_cnf,
    uecc_fsm_rcs_fail_action_last,
    PNULL
};
/*CSR:00053548 changes stop*/

/* SPR 15790 Fix Start */
static const uecc_fsm_fail_handler_f uecc_fsm_rcs_fail_type6_actions[] =
{
    uecc_fsm_rcs_fail_action_send_rrm_cnf,
    uecc_fsm_rcs_fail_action_send_llim_ue_conn_rej_req,
    uecc_fsm_rcs_csc_cell_del_handler,
    uecc_fsm_rcs_fail_action_last,
    PNULL
};

static const uecc_fsm_fail_handler_f uecc_fsm_rcs_fail_type7_actions[] =
{
    uecc_fsm_rcs_fail_action_delete_ue,
    uecc_fsm_rcs_fail_action_send_rrm_cnf,
    uecc_fsm_rcs_fail_action_send_llim_ue_conn_rej_req,
    uecc_fsm_rcs_csc_cell_del_handler,
    uecc_fsm_rcs_fail_action_last,
    PNULL
};
/* SPR 15790 Fix Stop */

/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/
/****************************************************************************
 * Function Name  : uecc_fsm_ue_connection_setup_procedure_constructor
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Constructor for UE Connection Setup procedure
 ****************************************************************************/
void uecc_fsm_ue_connection_setup_procedure_constructor(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    /*SPR 8708 Fix Start*/
    U8 procedure_code = RRC_NULL;
    /*SPR 8708 Fix Stop*/
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);
#ifdef RRC_UECC_TR_ID_CONST
    uecc_ue_ctx_set_op_transction_id(p_ue_context, 1); 
#else
    uecc_ue_ctx_set_op_transction_id(p_ue_context,
            uecc_ue_ctx_get_next_transction_id(p_ue_context));
#endif /*RRC_UECC_TR_ID_CONST*/

    /* SPR 19417 Fix Start */
    p_ue_context->m.rel_of_mme_present = 0;
    /* SPR 19417 Fix Stop */
   
    /* SPR 18649 Fix Start */
    p_ue_context->is_adm_cnf_sent = RRC_FALSE;
    /* SPR 18649 Fix Stop */
    /* Start timer*/
    if ( RRC_SUCCESS != uecc_ue_timer_start(
                p_ue_context->p_gb_context, 
                UECC_UE_RCS_TIMER,
                &p_ue_context->p_gb_context->ue_timers_data,
                &p_ue_context->timers_data,
                p_ue_context->ue_index))
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_rcs_fail_type1_actions);
    }
    /* SPR 4257 Start */
    if (PNULL != p_ue_context->p_gb_context)
    {
        if (PNULL != p_ue_context->p_gb_context->p_p_csc_context)
        {
            if (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                    [p_ue_context->cell_index])
            {
                if (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                        [p_ue_context->cell_index]->p_rrc_uecc_statistic)
                {
                    /* update number of UE */
                    uecc_status_cell_update_num_of_ue(p_ue_context->p_gb_context, 
                            p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                            p_rrc_uecc_statistic, RRC_TRUE);  
                }
            }
        }
        /*SPR 8708 Fix Start*/
        if ( p_ue_context->rcs_stmsi.bitmask == RRC_INITIAL_UE_IDENTITY_S_TMSI_PRESENT)
        {
            procedure_code = RRC_RrcConnectionReq_proc_code;
            if (RRC_SUCCESS != build_and_send_allocate_mme_req(
                        p_ue_context , procedure_code))
            {
                RRC_UECC_RCS_FSM_TRACE(RRC_WARNING,
                        "[ALLOCATE MME REQ]  failed");

            }

        }
        else
        {
            if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_admission_req(p_ue_context))
            {
                /* Process failure according to selected failure actions list */
                uecc_fsm_start_failure_processing(
                        p_ue_context,
                        uecc_fsm_rcs_fail_type1_actions);
            }

        }
        /* 15048_start */
    }
    /* 15048_end */
    /*SPR 8708 Fix Stop*/
    /* SPR 4257 Stop */

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
* Function Name  : calculate_conn_est_time_and_update_counter 
*
* Inputs         : uecc_ue_context_t* - pointer to UE context
* Returns        : None
*
* Description    : calculate time taken to complete rrc connection setup and 
*                   update RRC performance counter 
*******************************************************************************/
static void calculate_conn_est_time_and_update_counter(
        uecc_ue_context_t   *p_ue_context)
{
    U32     connection_estab_time = RRC_NULL;
    /*Passing a local variable connection_estab_time as a counter 
     * name inorder to calculate connection establishment time .
     * No counter updation takes place */
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_CALCULATE_TIMEDIFF_AND_UPDATE_COUNTER(p_ue_context->p_gb_context,
            p_ue_context->ue_stats.
            ue_timer_stats[RRC_UE_RRC_CONN_SETUP_TIME], 
            connection_estab_time, 0, DUMMY_RANGE_CHECK_INDEX);

    RRC_CALCULATE_TIMEDIFF_AND_UPDATE_COUNTER(p_ue_context->p_gb_context,
            p_ue_context->ue_stats.
            ue_timer_stats[RRC_UE_RRC_CONN_SETUP_TIME], g_rrc_stats.
            p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_connection_related_stats.
            rrc_connEstabTime_cause[p_ue_context->rcs_establishmentCause].
            rrc_connEstabTime_Sum, 0, DUMMY_RANGE_CHECK_INDEX);

    if ((connection_estab_time) > (g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_connection_related_stats.
                rrc_connEstabTime_cause[p_ue_context->rcs_establishmentCause].
                rrc_connEstabTime_Max))
    {
        g_rrc_stats.p_rrc_stats->rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_connection_related_stats.rrc_connEstabTime_cause[p_ue_context->
            rcs_establishmentCause].rrc_connEstabTime_Max = connection_estab_time;
    }

    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
            g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
            rrc_uecc_instance_stats[p_ue_context->
            p_gb_context->offset_uecc_mod_id].
            rrc_uecc_cell_stats[p_ue_context->cell_index].
            rrc_connection_related_stats.
            rrc_connEstabTime_cause[p_ue_context->rcs_establishmentCause].
            count, 0,DUMMY_RANGE_CHECK_INDEX);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : conn_setup_complete_update_performance_counter
 *
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Returns        : None
 *
 * Description    : update RRC performance counter upon rrc connection setup
 *                   complete
 * ******************************************************************************/
static void conn_setup_complete_update_performance_counter(
        uecc_ue_context_t   *p_ue_context)
{
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_ue_assoc_s1_conn_related_stats.
                s1sig_connEstabAtt, 0, DUMMY_RANGE_CHECK_INDEX);

        /* Update statistic for successful RRC Connection establishment */

        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_connection_related_stats.
                rrc_connection_establishment.rrc_connEstabSucc_cause[p_ue_context->
                rcs_establishmentCause], p_ue_context->rcs_establishmentCause, 
                (RRC_ESTABLISHMENT_CAUSE_INDEX - 1));


        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_connection_related_stats.rrc_connection_establishment.
                rrc_connEstabSucc_sum, p_ue_context->rcs_establishmentCause,
                (RRC_ESTABLISHMENT_CAUSE_INDEX - 1));

    	/* SPR 17391 Fix Start */
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].
                rrc_connection_related_stats.
                rrcConnectionNumber, 0, DUMMY_RANGE_CHECK_INDEX);

	    p_ue_context->m.rrc_connection_count_updated = RRC_TRUE;	
    	/* SPR 17391 Fix Stop */
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
* Function Name  : fill_ue_radio_admission_resp
* Inputs         : uecc_ue_context_t* - pointer to UE context
* Returns        : RRC_FAILURE/RRC_SUCCESS
*
* Description    : fill physical config dedicated paramter in ue radio admission
*                   response 
* * ****************************************************************************/

static rrc_return_t fill_ue_radio_admission_resp( 
        uecc_ue_context_t           *p_ue_context,
        rrc_rrm_ue_admission_resp_t *p_rrc_rrm_ue_admission_resp
        )
{
    U8 i = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_rrc_rrm_ue_admission_resp);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /* sps_config */
    if ( RRM_UE_ADM_RADIO_RESP_SPS_CONFIG_PRESENT &
            p_rrc_rrm_ue_admission_resp->radio_res_config.bitmask )
    {
        /* SPS Start */
        if (RRC_FAILURE == uecc_rrm_parse_and_update_sps_config(
                    p_ue_context,
                    &p_rrc_rrm_ue_admission_resp->
                    radio_res_config.sps_config))
        {
            RRC_UECC_RCS_FSM_TRACE(RRC_WARNING,
                    "[RRC_RRM_UE_ADMISSION_RESP] SPS Config updation failed");
        }
        /* SPS Stop */
        RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                "[RRC_RRM_UE_ADMISSION_RESP] sps config present");
    }
    /* Store rrm_meas_config_t if present */
    if ( RRM_UE_ADM_RADIO_RESP_MEAS_CONFIG_PRESENT &
            p_rrc_rrm_ue_admission_resp->radio_res_config.bitmask )
    {
        if (p_rrc_rrm_ue_admission_resp->radio_res_config.meas_config.bitmask & 
                UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT)
        {
            for (i = 0; i < p_rrc_rrm_ue_admission_resp->radio_res_config.
                    meas_config.meas_object_to_add_mod_list.count; i++)
            {
                if (p_rrc_rrm_ue_admission_resp->radio_res_config.
                        meas_config.meas_object_to_add_mod_list.meas_object_to_add_mod[i].
                        meas_object.bitmask & MEAS_OBJECT_TO_ADD_EUTRA_PRESENT)
                {
                    p_ue_context->m.measObjectEutraPresent = RRC_TRUE;
                }
            }
        }
        p_ue_context->m.meas_config_present = 1;
        p_ue_context->m.meas_config_updated = 1;

        /*Memory Optimization */
        p_ue_context->p_meas_config = (rrm_meas_config_t*) 
            rrc_mem_get (sizeof (rrm_meas_config_t));

        if (p_ue_context->p_meas_config == PNULL)
        {
            RRC_UECC_RCS_FSM_TRACE(RRC_FATAL,
                    "[RRC_RRM_UE_ADMISSION_RESP] Memory Allocation Failure");
            return RRC_FAILURE;
        }
        /* SES-508 Fix Start */
        /* Code Removed */
        /* SES-508 Fix End */
        l3_memcpy_wrapper(p_ue_context->p_meas_config,
                &p_rrc_rrm_ue_admission_resp->radio_res_config.meas_config,
                sizeof(rrm_meas_config_t));
    }

    /* SRB configuration */
    if ( RRM_UE_ADM_RADIO_RESP_SRB_INFO_PRESENT &
            p_rrc_rrm_ue_admission_resp->radio_res_config.bitmask )
    {
        if ( !(RRM_SRB_INFO_SRB1_CONFIG_PRESENT &
                    p_rrc_rrm_ue_admission_resp->radio_res_config.srb_info.bitmask))
        {
            RRC_UECC_RCS_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_UE_ADMISSION_RESP] srb1_config absent");
            return RRC_FAILURE;
        }

        p_ue_context->m.srb1_config_present = 1;
        p_ue_context->m.srb1_config_updated = 1;
        /* Store SRB1 configuration if present */
        p_ue_context->srb1_config =
            p_rrc_rrm_ue_admission_resp->radio_res_config.srb_info.srb1_config;

        if ((RRM_SRB_CONFIG_AM_CONFIG_PRESENT & p_ue_context->srb1_config.bitmask) &&
                (!(RRM_SRB_AM_CONFIG_EXPLICIT_PRESENT 
                   & p_ue_context->srb1_config.srb_am_config.bitmask)))
        {
            RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                    "[RRC_RRM_UE_ADMISSION_RESP] In SRB AM Config, "
                    "explicit AM config is not present."
                    " Applying default values for it.");
            p_ue_context->srb1_config.bitmask =
                (rrc_bitmask_t)(p_ue_context->srb1_config.bitmask &
                        ~RRM_SRB_CONFIG_AM_CONFIG_PRESENT);
        }

        if((RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT & 
                    p_ue_context->srb1_config.bitmask) &&
                (!(RRM_UL_SPECIFIC_PARAMETERS_EXPLICIT_PRESENT &
                   p_ue_context->srb1_config.logical_channel_config.bitmask)))
        {
            RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                    "[RRC_RRM_UE_ADMISSION_RESP] In SRB UL Specific "
                    " parameters, explicit SRB UL specific parameters"
                    " are not present. Applying default values for it.");
            p_ue_context->srb1_config.bitmask =
                (rrc_bitmask_t)(p_ue_context->srb1_config.bitmask &
                        ~RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT);
        }

        /* SPR 14984 Fix Start */
        /* Apply default configuration */
        uecc_defaults_apply_srb_config(
           &p_ue_context->srb1_config, &srb1_config_default, 
            p_ue_context, RRC_SRB1_LC_ID);
        /* SPR 14984 Fix Stop */

        if (RRM_SRB_INFO_SRB2_CONFIG_PRESENT &
                p_rrc_rrm_ue_admission_resp->radio_res_config.srb_info.bitmask)
        {
            /* Store SRB2 configuration if present */
            p_ue_context->srb2_config =
                p_rrc_rrm_ue_admission_resp->radio_res_config.srb_info.srb2_config;

            p_ue_context->m.srb2_config_present = 1;
            p_ue_context->m.srb2_config_updated = 1;
            /* Apply default configuration */
            if((RRM_SRB_CONFIG_AM_CONFIG_PRESENT & 
                        p_ue_context->srb2_config.bitmask) &&
                    (!(RRM_SRB_AM_CONFIG_EXPLICIT_PRESENT 
                       & p_ue_context->srb2_config.srb_am_config.bitmask)))
            {
                RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                        "[RRC_RRM_UE_ADMISSION_RESP] In SRB AM Config, "
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
                RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                        "[RRC_RRM_UE_ADMISSION_RESP] In SRB UL Specific "
                        "parameters, explicit SRB UL specific parameters"
                        " are not present. Applying default values for it.");
                p_ue_context->srb2_config.bitmask =
                    (rrc_bitmask_t)(p_ue_context->srb2_config.bitmask &
                            ~RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT);
            }

            /* SPR 14984 Fix Start */
            uecc_defaults_apply_srb_config(
                &p_ue_context->srb2_config, &srb2_config_default, 
                p_ue_context, RRC_SRB2_LC_ID);
            /* SPR 14984 Fix Stop */
        }
    }
    else
    {
        RRC_UECC_RCS_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_UE_ADMISSION_RESP] srb_info absent");
        return RRC_FAILURE;
    }
    /* cr_220 change */

    /* Storing RLF Timers & Constants from rrc_rrm_ue_admission_resp data from RRM into
     * UE Context */
    if(RRM_UE_ADM_RADIO_RESP_RLF_TIMERS_AND_CONSTANTS_PRESENT & 
            p_rrc_rrm_ue_admission_resp->radio_res_config.bitmask )
    {
        p_ue_context->m.rlf_timers_and_constants_r9_present = 1;

        if( RLF_TIMERS_AND_CONSTANTS_SETUP_PRESENT & p_rrc_rrm_ue_admission_resp->radio_res_config.rlf_timers_and_constants_r9.bitmask)
        {
            // Allocating Memory
            p_ue_context->p_rlf_timers_and_constants_r9 =
                (rlf_timers_and_constants_r9_t *)rrc_mem_get(sizeof(rlf_timers_and_constants_r9_t));

            if (PNULL != p_ue_context->p_rlf_timers_and_constants_r9)
            {
                p_ue_context->p_rlf_timers_and_constants_r9->bitmask = RLF_TIMERS_AND_CONSTANTS_SETUP_PRESENT;

                p_ue_context->p_rlf_timers_and_constants_r9->setup.t301_r9 =
                    p_rrc_rrm_ue_admission_resp->radio_res_config.rlf_timers_and_constants_r9.setup.t301_r9;

                p_ue_context->p_rlf_timers_and_constants_r9->setup.t310_r9 =
                    p_rrc_rrm_ue_admission_resp->radio_res_config.rlf_timers_and_constants_r9.setup.t310_r9;

                p_ue_context->p_rlf_timers_and_constants_r9->setup.n310_r9 =
                    p_rrc_rrm_ue_admission_resp->radio_res_config.rlf_timers_and_constants_r9.setup.n310_r9;

                p_ue_context->p_rlf_timers_and_constants_r9->setup.t311_r9 =
                    p_rrc_rrm_ue_admission_resp->radio_res_config.rlf_timers_and_constants_r9.setup.t311_r9;

                p_ue_context->p_rlf_timers_and_constants_r9->setup.n311_r9 =
                    p_rrc_rrm_ue_admission_resp->radio_res_config.rlf_timers_and_constants_r9.setup.n311_r9;
            }
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}


/****************************************************************************
* Function Name  :verify_phy_config_ded_antenna_param 
* Inputs         : uecc_ue_context_t* - pointer to UE context
* Returns        : RRC_FAILURE/RRC_SUCCESS
*
* Description    : verify phycisal config dedicated antenna parameter and set
*                   their bitmask to form ue admission response 
* ****************************************************************************/
static rrc_return_t verify_phy_config_ded_antenna_param(
        uecc_ue_context_t *p_ue_context)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* CBSR Changes Start */
    if(RRC_PHY_ANTENNA_INFORMATION_PRESENT &
            p_ue_context->physical_config_dedicated.bitmask)
    {
        if(!(RRC_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT &
                    p_ue_context->physical_config_dedicated
                    .antenna_information.bitmask))
        {   /* Default values shall be used */
            RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                    "[RRC_RRM_UE_ADMISSION_RESP] In PHY Dedicated Info, "
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
                RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                        "[RRC_RRM_UE_ADMISSION_RESP] Code Book Subset Restriction"
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
            RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                    "Transmission Mode is set to --> 8");
        }
        else
        {
            RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                    "[RRC_RRM_UE_ADMISSION_RESP] In PHY Dedicated Info, "
                    "Antenna info is not present."
                    " or Transmission mode != 8");
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

    /* cr_247: Added cqi_report_config_v920 */
    if (!(p_ue_context->physical_config_dedicated.bitmask &
                RRC_PHY_CQI_REPORTING_PRESENT))
    {
        RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
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
            if (p_ue_context->physical_config_dedicated.cqi_report_config_v920.bitmask &
                    RRC_PHY_CQI_MASK_R9_PRESENT) 
            {
                if (p_ue_context->physical_config_dedicated.cqi_reporting.
                        cqi_reporting_periodic.bitmask & 
                        RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT)
                {
                    RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                            "CQI_MASK_R9 :: Field cqi-ReportPeriodic is present"
                            " and set to SETUP");
                }    
                else
                {
                    RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                            "[RRC_RRM_UE_ADMISSION_RESP] In PHY Dedicated Info, "
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
            if (p_ue_context->physical_config_dedicated.cqi_report_config_v920.bitmask &
                    RRC_PHY_PMI_RI_REPORT_R9_PRESENT)
            {      
                if ((p_ue_context->physical_config_dedicated.cqi_reporting.
                            cqi_reporting_periodic.bitmask &
                            RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT) &&
                        (p_ue_context->physical_config_dedicated.cqi_reporting.bitmask &
                         RRC_PHY_CQI_REPORTING_MODE_APERIODIC_PRESENT))
                {   
                    RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                            "PMI_RI_REPORT_R9 :: Field cqi-ReportPeriodic is present and set to SETUP " 
                            " and cqi-ReportModeAperiodic is also Present");
                }
                else
                {
                    RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                            "[RRC_RRM_UE_ADMISSION_RESP] In PHY Dedicated Info, "
                            "PMI_RI_REPORT_R9 :: Field cqi-ReportPeriodic is set to RELEASE"
                            " or APERIODIC is not present");
                    p_ue_context->physical_config_dedicated.cqi_report_config_v920.
                        bitmask ^= RRC_PHY_PMI_RI_REPORT_R9_PRESENT;
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

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_connection_setup_procedure_finish_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Success handler for UE Connection Setup procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_connection_setup_procedure_finish_success(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_RCS_TIMER, &p_ue_context->timers_data);

    /* Clear update configuration IEs flags */
    p_ue_context->m.physical_config_dedicated_updated = 0;
    p_ue_context->m.sps_config_updated = 0;
    p_ue_context->m.mac_config_updated = 0;
    p_ue_context->m.srb1_config_updated = 0;
    p_ue_context->m.srb2_config_updated = 0;

    /* SPS Start */
    if (p_ue_context->m.is_sps_recv_frm_rrm)
    {

        /* Send to MAC - RRC_MAC_RECONFIG_COMPLETE_IND */
        uecc_llim_build_and_send_mac_reconfig_complete_ind(
            p_ue_context->p_gb_context,
            p_ue_context,
            RRC_SUCCESS);
    }
    p_ue_context->m.sps_config_updated = 0;
    p_ue_context->m.is_sps_recv_frm_rrm = 0;
    /* SPS Stop */


    /* Indicate SRB1 creation */
    p_ue_context->m.srb1_created = 1;

    /* Send event to UE Main FSM */
    uecc_event_queue_push_event(
        UECC_EV_UE_CONNECTION_SETUP_SUCCESS_INT,
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_ue_connection_setup_procedure_finish_fail
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Failure handler for UE Connection Setup procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_connection_setup_procedure_finish_fail(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_RCS_TIMER, &p_ue_context->timers_data);

    /* SPS Start */
    if (p_ue_context->m.is_sps_recv_frm_rrm)
    {
        if (p_ue_context->p_sps_config != PNULL)
        {
            rrc_mem_free(p_ue_context->p_sps_config);
            p_ue_context->p_sps_config = PNULL;
        }
    /* Send to MAC - RRC_MAC_RECONFIG_COMPLETE_IND */
    uecc_llim_build_and_send_mac_reconfig_complete_ind(
            p_ue_context->p_gb_context,
            p_ue_context,
            RRC_SUCCESS);
    }
    /* SPS Stop */

    /*SPR_17633_START*/
    if(RRC_TRUE == p_ue_context->m.rel_of_mme_present)
    {
        /* 
         * Send ue_release_msg to S1AP module 
         */
        if (RRC_SUCCESS != uecc_s1ap_build_and_send_ue_release_msg_req(
                    p_ue_context->s1ap_ue_id,
                    RRC_FALSE,
                    p_ue_context))
        {
            RRC_UECC_RCS_FSM_TRACE(RRC_WARNING,
                    "ue_release_msg delivery failed");
        }
    }
    /*SPR_17633_END*/

    /* notify UE FSM about failure */
    /* Post event to UE FSM for future processing */
    uecc_event_queue_push_event(
            UECC_EV_UE_CONNECTION_SETUP_FAILURE_INT,
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_ue_connection_setup_csc_cell_delete
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Failure handler for UE Connection Setup procedure
 *                  Calls on procedure's finishing
 ****************************************************************************/
void uecc_fsm_ue_connection_setup_csc_cell_delete(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_RCS_TIMER, &p_ue_context->timers_data);

    /* notify UE FSM about csc cell delete */
    /* Post event to UE FSM for future processing */
    uecc_event_queue_push_event(
            UECC_EV_UE_CONNECTION_SETUP_CCSC_CELL_DELETE,
            p_ue_context,
            p_api);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR 8708 Fix Start*/
/*SPR 8708 Fix Stop*/
/*SPR 8708 Fix Start*/
/****************************************************************************
 * Function Name  : uecc_rcs_ue_conn_s1ap_allocate_mme_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 
*                   UECC_EV_S1AP_ALLOCATE_MME_RESP from a RRM.
 ****************************************************************************/

void uecc_fsm_rcs_ue_conn_s1ap_allocate_mme_resp_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    s1ap_allocate_mme_resp_t     *allocate_mme_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api_data);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    allocate_mme_resp = (s1ap_allocate_mme_resp_t*)p_api_data;

    if (RRC_INVALID_MME_ID == allocate_mme_resp->mme_id)
    {

        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_rcs_fail_type1_actions);
    }
    else
    {
        p_ue_context->m.rel_of_mme_present = 1;
        /* 15053_start */
        p_ue_context->rel_of_mme = (rel_info_of_mme_et)allocate_mme_resp->rel_of_mme;
        /* 15053_end */
        /* SPR 18363 Fix Start */
        p_ue_context->mme_id = allocate_mme_resp->mme_id;
        /* SPR 18363 Fix End */

        RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
            "Allocated MME %u of Rel-%u eNB UE S1AP ID %u for UE %u",
            p_ue_context->mme_id, p_ue_context->rel_of_mme + RRC_EIGHT,
            allocate_mme_resp->enb_ue_s1ap_id,
            p_ue_context->ue_index);
        if (RRC_SUCCESS != uecc_rrm_build_and_send_ue_admission_req(p_ue_context))
        {
            /* Process failure according to selected failure actions list */
            uecc_fsm_start_failure_processing(
                    p_ue_context,
                    uecc_fsm_rcs_fail_type1_actions);
        }

    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR 8708 Fix Stop*/
/****************************************************************************
 * Function Name  : uecc_rcs_rrm_ue_admission_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_RCS_EVENT_UE_ADMISSION_RESP from a RRM.
 ****************************************************************************/
void uecc_fsm_rcs_rrm_ue_admission_resp_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    rrc_rrm_ue_admission_resp_t*    p_rrc_rrm_ue_admission_resp = PNULL;
    rrc_return_t                    result = RRC_FAILURE;
    uecc_fsm_fail_handler_f const*  p_failure_actions_list =
                                        uecc_fsm_rcs_fail_type2_actions;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_rrc_rrm_ue_admission_resp = (rrc_rrm_ue_admission_resp_t*)p_api_data;

    switch (p_rrc_rrm_ue_admission_resp->response)
    {
        case RRC_SUCCESS:
            if ( !(RRM_UE_ADM_RESP_API_UE_ADM_RADIO_RESP_PRESENT
                    & p_rrc_rrm_ue_admission_resp->bitmask))
            {
                /* It should be present in case of success
                 * See LTE RRC API Reference Manual RRC_RRM_UE_ADMISSION_RESP */
                RRC_UECC_RCS_FSM_TRACE(RRC_ERROR,
                    "[RRC_RRM_UE_ADMISSION_RESP] radio_res_config absent");
            }
            else
            {
                /* Store message data in UE context for future processing */
                /* Process RRC_RRM_UE_ADMISSION_RESP message data */
                result = uecc_fsm_rcs_process_rrm_ue_admission_resp_data(
                            p_ue_context,
                            p_rrc_rrm_ue_admission_resp );

                if ( RRC_SUCCESS == result)
                {
                    UECC_FSM_SET_CHILD_STATE(p_ue_context,
                        &uecc_fsm_rcs_llim_create_ue_ongoing, PNULL);
                }
            }
            break;

        case RRC_FAILURE:
            /* RRM rejected connection */
            p_ue_context->rcs_reject_cause = RRM_FAILURE; 
	    
	    /*SPR Fix 8739 start*/
	    p_ue_context->wait_time = p_rrc_rrm_ue_admission_resp->wait_time;

	    if (p_rrc_rrm_ue_admission_resp->bitmask & 
			RRM_UE_ADM_RESP_API_UE_ADM_EXTENDED_WAIT_TIME_PRESENT )
	    {
		p_ue_context->extended_wait_time = p_rrc_rrm_ue_admission_resp->extended_wait_time;
	    }
	    /*SPR Fix 8739 stop*/
            
	    p_failure_actions_list = uecc_fsm_rcs_fail_type1_actions;

            /* CR 026 changes start*/
            if (p_rrc_rrm_ue_admission_resp->bitmask & RRM_UE_ADM_RESP_API_FAIL_CAUSE_PRESENT)
            {
                p_ue_context->rrc_conn_estab_rrm_fail_cause = p_rrc_rrm_ue_admission_resp->
                    fail_cause;

                p_ue_context->m.rrc_conn_estab_rrm_fail_cause_present = 1;
            }
            /* CR 026 changes stop*/
            break;

        default:
            RRC_UECC_RCS_FSM_TRACE(RRC_ERROR,
                "[RRC_RRM_UE_ADMISSION_RESP] Invalid Response=%i",
                p_rrc_rrm_ue_admission_resp->response);
            break;
    }

    /* Process failure */
    if (RRC_SUCCESS != result)
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(p_ue_context,
                                          p_failure_actions_list);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_rrm_ue_admission_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rcs_rrm_ue_admission_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_RCS_FSM_TRACE(RRC_BRIEF, "Time out.");

    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rcs_fail_type2_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_llim_create_ue_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rcs_llim_create_ue_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);


    /* Send Create UE request to LLIM */
    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_create_ue_entity_req(p_ue_context))
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_rcs_fail_type2_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_rcs_llim_create_ue_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_LLIM_CREATE_UE_ENTITY_RESP from LLIM
 ****************************************************************************/
void uecc_fsm_rcs_llim_create_ue_entity_resp_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    rrc_return_t                           result = RRC_FAILURE;
    rrc_uecc_llim_create_ue_entity_resp_t* p_llim_create_ue_entity_resp = PNULL;
    uecc_fsm_fail_handler_f const*         p_failure_actions_list =
                                                uecc_fsm_rcs_fail_type3_actions;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_llim_create_ue_entity_resp =
        (rrc_uecc_llim_create_ue_entity_resp_t*)p_api_data;

    if ( RRC_LLIM_RESPONSE_CODE_LAST <=
            p_llim_create_ue_entity_resp->response_code )
    {
        RRC_UECC_RCS_FSM_TRACE(RRC_FATAL,
            "[UECC_EV_LLIM_CREATE_UE_ENTITY_RESP] Invalid Response=%i",
            p_llim_create_ue_entity_resp->response_code);
    }
    else if ( RRC_LLIM_SUCCESS == p_llim_create_ue_entity_resp->response_code )
    {

        /* SPS Start */
        if (p_ue_context->m.is_sps_recv_frm_rrm)
        {
            if (p_llim_create_ue_entity_resp->bitmask &
                    UECC_LLIM_CREATE_SRB_ENTITY_SPS_CRNTI_PRESENT)
            {
                /* Copy SPS CRNTI into UE Context */
                rrc_pack_U16(p_ue_context->p_sps_config->semi_presist_sched_c_rnti,
                        &p_llim_create_ue_entity_resp->sps_crnti);
                p_ue_context->p_sps_config->bitmask |= RRC_SPS_CONFIG_C_RNTI_PRESENT;

            }
            else
            {
                p_ue_context->m.sps_config_updated = RRC_NULL; /* Reset to 0 */
                if (p_ue_context->p_sps_config != NULL)
                {
                    rrc_mem_free(p_ue_context->p_sps_config);
                    p_ue_context->p_sps_config = PNULL;
                    p_ue_context->m.sps_config_present = 0;
                }
            }
        }

        /* SPS Stop */

        /* Switch RCS state */
        UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_rcs_rrc_connection_setup_ongoing, PNULL);
        result = RRC_SUCCESS;
    }
    else
    {
        RRC_UECC_RCS_FSM_TRACE(RRC_WARNING,
            "[UECC_EV_LLIM_CREATE_UE_ENTITY_RESP] Create UE failed at LL [%i]",
            p_llim_create_ue_entity_resp->response_code);
        /* LLIM rejected connection */
        p_failure_actions_list = uecc_fsm_rcs_fail_type2_actions;
    }

    /* Process failure */
    if (RRC_SUCCESS != result)
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
            p_ue_context,
            p_failure_actions_list);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_llim_create_ue_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rcs_llim_create_ue_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_RCS_FSM_TRACE(RRC_BRIEF, "Time out.");

    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rcs_fail_type3_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_rrc_connection_setup_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_rcs_rrc_connection_setup_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api);

    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_rrc_connection_setup(p_ue_context))
    {
        /* Process failure according to selected failure actions list */
        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_rcs_fail_type3_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_rrc_connection_setup_complete_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_LLIM_CREATE_UE_ENTITY_RESP from LLIM
 ****************************************************************************/
void uecc_fsm_rcs_rrc_connection_setup_complete_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    rrc_return_t                result = RRC_FAILURE;
    RRCConnectionSetupComplete* p_setup_complete = PNULL;
    /* SPR 13394 Fix Start */
/* Bug 8198 Fix Start */
    U8 procedure_code = RRC_NULL;
/* Bug 8198 Fix End */
    /* SPR 13394 Fix Stop */

    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api_data);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/

    if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(p_ue_context->p_gb_context,
                p_ue_context->cell_index))

    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_WARNING,"cell not active");
        /* Here the UE connection is active and SRB1 channel is established.
         * Start UE Connection Release control procedure */
        uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)UECC_RCS_FAILURE_RELEASE_REQ);
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return;
    }

    do
    {
        if ((PNULL != p_ue_context->p_gb_context->p_p_csc_context) &&
                (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                 [p_ue_context->cell_index]) &&
                (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                 [p_ue_context->cell_index]->p_csc_init_setup_ind))
                {
                    /* it should be RRCConnectionSetupComplete */
                    p_setup_complete = (RRCConnectionSetupComplete*)p_api_data;

                    /* All pointer should be checked by UECC LLIM interface unit */
                    p_ue_context->selected_plmn_identity =
                        p_setup_complete->criticalExtensions.u.c1->
                        u.rrcConnectionSetupComplete_r8->selectedPLMN_Identity;

                    /* If registred MME information is present store it in
                     * UE context */
                    if(p_setup_complete->criticalExtensions.u.c1->
                            u.rrcConnectionSetupComplete_r8->m.registeredMMEPresent)
                    {
                        p_ue_context->registered_mme_info_present = 1;
                        p_ue_context->registered_mme =
                            p_setup_complete->criticalExtensions.u.c1->
                            u.rrcConnectionSetupComplete_r8->registeredMME;

                        if ((PNULL != g_rrc_stats.p_rrc_stats) && (RRC_NULL == (((p_setup_complete->criticalExtensions.
                                        u.c1->u.rrcConnectionSetupComplete_r8->registeredMME.mmegi.data[1]) 
                                        >> RRC_MMEGI_RIGHT_SHIFT_BITS) & RRC_MMEGI_BITMASK )))
                        {
                            RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                                    g_rrc_stats.p_rrc_stats->
                                    rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                                    p_gb_context->offset_uecc_mod_id].
                                    rrc_uecc_cell_stats[p_ue_context->cell_index].
                                    rrc_IratIncMobility_LAI,
                                    0, DUMMY_RANGE_CHECK_INDEX);
                        }
                    }


                    /* Get selected PLMN-identity and store it in UE context */
                    if (p_ue_context->selected_plmn_identity >
                            p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                            p_csc_init_setup_ind->sib_type_1_Info.
                            cell_access_related_info.plmn_Id_info_list.count)
                    {
                        break;
                    }
				/*SPR 15896 Fix Start*/
                    if (p_ue_context->m.traceActivated)
                    {
				/*SPR 15875 Fix Start*/
			    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
					    (p_ue_context->p_ue_trace_activation_info != PNULL) &&
					    (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & UU) &&
					    ((MEDIUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_trace_activation_info->trace_depth) ||
					     (MEDIUM == p_ue_context->p_ue_trace_activation_info->trace_depth) ||
					     (MINIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_trace_activation_info->trace_depth) ||
					     (MINIMUM == p_ue_context->p_ue_trace_activation_info->trace_depth)))
			    {
				    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), GEN_MME,"\n <msg function=\"Uu\" name=\"RRC CONNECTION SETUP COMPLETE\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
						    difftime_wrapper(p_ue_context->p_ue_trace_activation_info->rawtime));
			    }
			    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
					    (p_ue_context->p_ue_cell_trace_activation_info != PNULL) &&
					    (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & UU) &&
					    ((MEDIUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_cell_trace_activation_info->trace_depth) ||
					     (MEDIUM == p_ue_context->p_ue_cell_trace_activation_info->trace_depth) ||
					     (MINIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_cell_trace_activation_info->trace_depth) ||
					     (MINIMUM == p_ue_context->p_ue_cell_trace_activation_info->trace_depth)))
			    {
				    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), GEN_CELL,"\n <msg function=\"Uu\" name=\"RRC CONNECTION SETUP COMPLETE\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
						    difftime_wrapper(p_ue_context->p_ue_cell_trace_activation_info->rawtime));
			    }
                        	P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), UU,"\n <ie name=\"Selected PLMN IDENTITY\">%d </ie>",p_ue_context->selected_plmn_identity);

                            /* SPR:5907 start */
                        if (1 == p_ue_context->registered_mme_info_present)
                        {
                            P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), UU,"\n <ie name=\"RegisteredMME-MMMgi\">%02x%02x </ie>",
                                                        p_ue_context->registered_mme.mmegi.data[0],p_ue_context->registered_mme.mmegi.data[1]);
                            P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), UU,"\n <ie name=\"RegisteredMME-MMEc\">%02x </ie> ",p_ue_context->registered_mme.mmec.data[0]);
                        }
                        /* SPR:5907 stop */
                        /* SPR:7355 start */    
                        P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), UU," \n </msg>");
                        /* SPR:7355 stop */    
				/*SPR 15896 Fix Stop*/
				/*SPR 15875 Fix Stop*/
                    } 
                        /*SPR 15875 Fix Stop*/

                    p_ue_context->plmn_identity =
                        p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                        cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
                        cell_access_related_info.plmn_Id_info_list.
                        plmn_identity_info[p_ue_context->selected_plmn_identity - 1];

                    /* Build and send RRM_UE_ADMISSION_CNF with success to RRM*/
                    result = uecc_rrm_build_and_send_ue_admission_cnf(
                            p_ue_context, RRC_SUCCESS );

                    if (RRC_SUCCESS != result)
                    {
                        break;
                    }
                    /* SPR 16149 Start */
                    p_ue_context->is_adm_cnf_sent = RRC_TRUE;
                    /* SPR 16149 Stop */

                    /* Build and send initial UE message to S1AP */
/* Bug 8198 Fix Start */
		    /*SPR 8708 Fix Start*/
		    if (p_ue_context->rcs_stmsi.bitmask != RRC_INITIAL_UE_IDENTITY_S_TMSI_PRESENT)
		    {
			
                    	procedure_code = ASN1V_s1ap_id_initialUEMessage; 
                    	result = build_and_send_allocate_mme_req(
                            p_ue_context,procedure_code);
/* Bug 8198 Fix End */

                    	if (RRC_SUCCESS != result)
                    	{
                        	break;
                    	}
		    }	
		    /*SPR 8708 Fix Stop*/

                    asn1Init_s1ap_NAS_PDU(&p_ue_context->nas_pdu);
                    p_ue_context->nas_pdu.numocts =
                        p_setup_complete->criticalExtensions.u.c1->
                        u.rrcConnectionSetupComplete_r8->dedicatedInfoNAS.numocts;
                    p_ue_context->nas_pdu.data = rrc_mem_get( sizeof (U8) * p_ue_context->nas_pdu.numocts);
                    if (p_ue_context->nas_pdu.data == PNULL)
                    {
                        break;
                    }
                    l3_memcpy_wrapper((void *)p_ue_context->nas_pdu.data, (const void *)p_setup_complete->criticalExtensions.u.
                    c1->u.rrcConnectionSetupComplete_r8->dedicatedInfoNAS.data, 
                            sizeof(U8)*(p_ue_context->nas_pdu.numocts));

                    conn_setup_complete_update_performance_counter(p_ue_context);
                    /* Getting the time of receiving rrc connection complete and calculating 
                     * total rrc connection setup time*/
                    RRC_UE_STOP_TIME_PERFORMANCE_COUNTER(p_ue_context,
                            p_ue_context->ue_stats.
                            ue_timer_stats[RRC_UE_RRC_CONN_SETUP_TIME],
                            RRC_UE_RRC_CONN_SETUP_TIME);
                    if ((PNULL != g_rrc_stats.p_rrc_stats) &&
                        ((p_ue_context->rcs_establishmentCause) <= (RRC_ESTABLISHMENT_CAUSE_INDEX - 1)))
                    {
                        calculate_conn_est_time_and_update_counter(p_ue_context);
                    }
/*SPR 8708 Fix Start*/
		    if(p_ue_context->rcs_stmsi.bitmask == RRC_INITIAL_UE_IDENTITY_S_TMSI_PRESENT)
		    {	
			/* Build and send initial UE message to S1AP */
        		result = uecc_s1ap_build_and_send_initial_ue_msg_req(
                		p_ue_context,
                		p_ue_context->rel_of_mme);

        	    	if (  p_ue_context->nas_pdu.data != PNULL)
        	    	{
         			rrc_mem_free ((void*)p_ue_context->nas_pdu.data);
                    	}

        	    	if (RRC_SUCCESS != result)
        	    	{
           			break;
        	    	}
   		   	/* Trace related action : Now plmn_identity has been selected in 
     		   	* RRC Connection Setup Complete msg, so trace reference and trace 
     		   	* recording session reference can be created */

    		   	/* Start MME Guard Timer For Initial UE */
    		   	if( RRC_SUCCESS != uecc_ue_timer_start(
            	   	p_ue_context->p_gb_context, 
            	   	UECC_UE_MME_GUARD_TIMER,
            	   	&p_ue_context->p_gb_context->ue_timers_data,
            	   	&p_ue_context->timers_data,
            	   	p_ue_context->ue_index))
    		  	{
         			RRC_UECC_RCS_FSM_TRACE(RRC_ERROR, "MME Guard Timer For Initial UE failed");
        			result = RRC_FAILURE;
        			break;
    		  	}

    		  	uecc_fsm_finish_activity_success(p_ue_context, PNULL);
		   }



/*SPR 8708 Fix Stop*/
        }
    } while (0);

    /* SPR 13394 Fix Start */
    if ( RRC_FAILURE == result)
    {
        /* Here the UE connection is active and SRB1 channel is established.
         * Start UE Connection Release control procedure */
        uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_RCS_FAILURE_RELEASE_REQ);
    }
    /* SPR 13394 Fix Stop */
   
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_rrc_connection_setup_ongoing_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rcs_rrc_connection_setup_ongoing_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_RCS_FSM_TRACE(RRC_BRIEF, "Time out.");

    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
        /* Update the performance counter */
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_connection_related_stats.
                rrc_connection_establishment.
                rrc_connEstabFail_cause[p_ue_context->rcs_establishmentCause],
                p_ue_context->rcs_establishmentCause, (RRC_ESTABLISHMENT_CAUSE_INDEX-1));

        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_connection_related_stats.
                rrc_connection_establishment.
                rrc_connEstabFail_sum, p_ue_context->rcs_establishmentCause,
                (RRC_ESTABLISHMENT_CAUSE_INDEX-1));

    }
 /* SPR 16149 Fix Start */
    if (RRC_TRUE == p_ue_context->is_adm_cnf_sent)
    {
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_rcs_fail_type5_actions);       
    }    
    else
    {

    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rcs_fail_type4_actions);
    }
    /* SPR 16149 Fix Stop */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_rrc_connection_setup_ongoing_overload_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_S1AP_OVERLOAD_MSG_IND
 ****************************************************************************/
void uecc_fsm_rcs_rrc_connection_setup_ongoing_overload_ind_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
            "Processing OVERLOAD INDICATION");

    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_RCS_FAILURE_RELEASE_REQ);


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR 13758 Fix Start*/
/****************************************************************************
 * Function Name  : uecc_fsm_rcs_s1ap_ue_release_req_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 * 			UECC_EV_S1AP_UE_REL_REQ_INT from LLIM
 ****************************************************************************/
void uecc_fsm_rcs_s1ap_ue_release_req_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data

)
{

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    
    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);
    RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
            "Processing S1AP UE RELEASE REQ");
   
    /*Stop Timer*/ 
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_RCS_TIMER, &p_ue_context->timers_data);

    /*Continue Failure Proccessing */
    uecc_fsm_finish_activity_failure(
	p_ue_context, (void*)UECC_RCS_FAILURE_RELEASE_REQ);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    
}
/*SPR 13758 Fix Stop*/
/****************************************************************************
 * Function Name  : uecc_fsm_rcs_llim_delete_ue_entity_response_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_LLIM_DELETE_UE_ENTITY_RESP from LLIM
 ****************************************************************************/
void uecc_fsm_rcs_llim_delete_ue_entity_response_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    rrc_uecc_llim_delete_ue_entity_resp_t *p_llim_delete_ue_entity_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_llim_delete_ue_entity_resp =
        (rrc_uecc_llim_delete_ue_entity_resp_t*)p_api_data;

    if ( RRC_SUCCESS != p_llim_delete_ue_entity_resp->response_code )
    {
        RRC_UECC_RCS_FSM_TRACE(RRC_WARNING, "Delete UE request failed in LLIM");
    }

    /* Stop timer */
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_RCS_TIMER, &p_ue_context->timers_data);

    /* Continue failure processing */
    uecc_fsm_process_failure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_fail_w_for_response_from_rrc_timer_expiry_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rcs_fail_w_for_response_from_rrc_timer_expiry_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_RCS_FSM_TRACE(RRC_BRIEF, "Time out.");

    uecc_fsm_process_failure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_fail_action_send_llim_ue_conn_rej_req
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et* - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
void uecc_fsm_rcs_fail_action_send_llim_ue_conn_rej_req(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT( PNULL != p_ue_context );
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
        "FAILURE: Send UECC_LLIM_UE_CON_REJ_REQ(rrcConnectionReject).");

    /* send UECC_LLIM_UE_CON_REJ_REQ(rrcConnectionReject) message to LLIM */
    if ( RRC_SUCCESS !=
                uecc_llim_build_and_send_rrc_connection_reject(
                p_ue_context->p_gb_context,
                p_ue_context->crnti,
                p_ue_context->wait_time,
                p_ue_context->rcs_establishmentCause,
                p_ue_context->cell_index,
                p_ue_context->rcs_reject_cause,
                p_ue_context->extended_wait_time,/* CR 026 changes start*/
                p_ue_context))/* CR 026 changes stop*/
    {
        RRC_UECC_RCS_FSM_TRACE(RRC_ERROR,
            "[UECC_LLIM_UE_CON_REJ_REQ] Delivery failure");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_fail_action_send_rrm_cnf
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et* - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
void uecc_fsm_rcs_fail_action_send_rrm_cnf(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT( PNULL != p_ue_context );
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);


    RRC_UECC_RCS_FSM_TRACE(RRC_INFO, "FAILURE: Send RRM_UE_ADMISSION_CNF.");

    /* Build and send RRM_UE_ADMISSION_CNF with failure to RRM*/
    uecc_rrm_build_and_send_ue_admission_cnf( p_ue_context, RRC_FAILURE );

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_fail_action_delete_ue
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et* - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
void uecc_fsm_rcs_fail_action_delete_ue(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT( PNULL != p_ue_context );
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    RRC_UECC_RCS_FSM_TRACE(RRC_INFO, "FAILURE: Delete UE.");

    *p_break = RRC_TRUE;

    /*Stop procedural timer*/
    uecc_ue_timer_stop(p_ue_context->p_gb_context, 
            UECC_UE_RCS_TIMER, &p_ue_context->timers_data);

    /* Start timer for delete UE operation */
    /* Bug 895 Fix Start */
    if ( RRC_SUCCESS != uecc_ue_timer_start(
                            p_ue_context->p_gb_context,
                            UECC_UE_RCR_TIMER,
                            &p_ue_context->p_gb_context->ue_timers_data,
                            &p_ue_context->timers_data,
                            p_ue_context->ue_index))
    /* Bug 895 Fix Stop */
    {
        RRC_UECC_RCS_FSM_TRACE( RRC_ERROR,
            "FAILURE: Can't start Delete UE timer.");
        *p_break = RRC_FALSE;
    }

    /* Build and send UECC_LLIM_DELETE_UE_ENTITY_REQ with failure to LLIM*/
    if (RRC_SUCCESS !=
          uecc_llim_build_and_send_delete_ue_entity_req( p_ue_context ))
    {
        RRC_UECC_RCS_FSM_TRACE( RRC_ERROR,
            "FAILURE: UECC_LLIM_DELETE_UE_ENTITY_REQ delivery fail.");
          *p_break = RRC_FALSE;
    }

    if ( RRC_TRUE == *p_break)
    {
        /* Change state and wait for LLIM response */
        UECC_FSM_SET_CHILD_STATE(
            p_ue_context, &uecc_fsm_rcs_fail_w_for_response_from_rrc, PNULL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_fail_action_last
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et* - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
void uecc_fsm_rcs_fail_action_last(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_break);

    RRC_UECC_RCS_FSM_TRACE(RRC_INFO, "FAILURE: END.");

    uecc_fsm_finish_activity_failure(p_ue_context, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_process_rrm_ue_admission_resp_data
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_rrm_ue_admission_resp_t* - pointer on message data
 * Outputs        : None
 * Returns        : None
 * Description    : Process data from rrm_ue_admission_resp message and
 *                  update UE context.
 ****************************************************************************/
rrc_return_t uecc_fsm_rcs_process_rrm_ue_admission_resp_data(
    uecc_ue_context_t*           p_ue_context,
    rrc_rrm_ue_admission_resp_t* p_rrc_rrm_ue_admission_resp
)
{
    U8 sub_frame_assignment  = RRC_NULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    /* Store UE parameters in UE context and apply default values for them
     * if its needed.*/
    
    if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(p_ue_context->p_gb_context,
                p_ue_context->cell_index))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_WARNING,"cell not active");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    if ((PNULL != p_ue_context->p_gb_context->p_p_csc_context) &&
            (PNULL != p_ue_context->p_gb_context->p_p_csc_context
             [p_ue_context->cell_index]) &&
            (PNULL != p_ue_context->p_gb_context->p_p_csc_context
             [p_ue_context->cell_index]->p_csc_init_setup_ind) )
                {
                    /* physical_config_dedicated */
                    if ( RRM_UE_ADM_RADIO_RESP_PHY_CONFIG_DED_PRESENT &
                            p_rrc_rrm_ue_admission_resp->radio_res_config.bitmask )
                    {
                        RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                                "[RRC_RRM_UE_ADMISSION_RESP] PHY Dedicated Info present");
                        p_ue_context->m.physical_config_dedicated_present = 1;
                        p_ue_context->m.physical_config_dedicated_updated = 1;

                        /* Store physical_config_dedicated if present */
                        p_ue_context->physical_config_dedicated =
                            p_rrc_rrm_ue_admission_resp->radio_res_config.
                            physical_config_dedicated;

                        if (RRC_FAILURE == verify_phy_config_ded_antenna_param(p_ue_context))
                        {
                            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                            return RRC_FAILURE;
                        }

                        if (p_rrc_rrm_ue_admission_resp->radio_res_config.
                                physical_config_dedicated.bitmask &
                                RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT)
                        {
                            if (RRC_PHY_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT &
                                    p_rrc_rrm_ue_admission_resp->radio_res_config.
                                    physical_config_dedicated.
                                    pucch_configuration_dedicated.bitmask )
                            {

                                if (DUPLEXING_MODE_FDD == p_ue_context->p_gb_context->
                                        p_p_csc_context[p_ue_context->cell_index]->
                                        p_csc_init_setup_ind->duplexing_mode)
                                {
                                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                            p_ue_context->p_gb_context->facility_name,
                                            RRC_WARNING,"TDD ack nack feedback mode " \
                                            " must be absent in UE admission resp from RRM" \
                                            "in case of FDD Cell. Ignoring the received"
                                            " TDD ack nack feedback mode.");

                                    p_ue_context->physical_config_dedicated.
                                        pucch_configuration_dedicated.bitmask &= (rrc_bitmask_t)
                                        (~RRC_PHY_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT);

                                }
                            }
                            else if(DUPLEXING_MODE_TDD == p_ue_context->p_gb_context->
                                    p_p_csc_context[p_ue_context->cell_index]->
                                    p_csc_init_setup_ind->duplexing_mode)
                            {
                                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                        p_ue_context->p_gb_context->facility_name,
                                        RRC_WARNING,"TDD ack nack feedback mode " 
                                        " must be present in UE admission resp from RRM" 
                                        "in case of TDD Cell");
                                return RRC_FAILURE;
                            }
                        }
                        else if(DUPLEXING_MODE_TDD == p_ue_context->p_gb_context->
                                p_p_csc_context[p_ue_context->cell_index]->
                                p_csc_init_setup_ind->duplexing_mode)
                        {
                            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                    p_ue_context->p_gb_context->facility_name,
                                    RRC_WARNING,"TDD ack nack feedback mode " 
                                    " must be present in UE admission resp from RRM"
                                    "in case of TDD Cell");
                            return RRC_FAILURE;
                        }


                    }
                    else
                    {
                        RRC_UECC_RCS_FSM_TRACE(RRC_ERROR,
                                "[RRC_RRM_UE_ADMISSION_RESP]"
                                "phy_config_dedicated_configuarion absent");
                        return RRC_FAILURE;
                    }

                    /*** TDD changes start ***/

                    /* MAC configuration */
                    if (RRM_UE_ADM_RADIO_RESP_MAC_CONFIG_PRESENT &
                            p_rrc_rrm_ue_admission_resp->radio_res_config.bitmask)
                    {

                        if(DUPLEXING_MODE_FDD == p_ue_context->p_gb_context->p_p_csc_context[
                                p_ue_context->cell_index]->p_csc_init_setup_ind->
                                duplexing_mode)
                        {
                            if ((p_rrc_rrm_ue_admission_resp->radio_res_config.mac_config.
                                        mac_main_config_extensions.dl_num_harq_process <
                                        RRC_RRM_FDD_DL_HARQ_PROCESS_MIN) || 
                                    (p_rrc_rrm_ue_admission_resp->radio_res_config.mac_config.
                                     mac_main_config_extensions.dl_num_harq_process > RRC_RRM_FDD_DL_HARQ_PROCESS_MAX))
                            {
                                RRC_UECC_RCS_FSM_TRACE(RRC_ERROR,
                                        "[RRC_RRM_UE_ADMISSION_RESP]"
                                        "Duplexing Mode = FDD "
                                        "and dl_num_harq_process = %u",
                                        p_rrc_rrm_ue_admission_resp->radio_res_config.mac_config.
                                        mac_main_config_extensions.dl_num_harq_process);
                                return RRC_FAILURE;
                            }

                        }

                        /* Store rrm_mac_config_t if present */
                        p_ue_context->m.mac_config_present = 1;
                        p_ue_context->m.mac_config_updated = 1;

                        /*mac_main_config has been made mandatory during RCS procedure so that
                         *          * RRC does not fill the default values of mac_main_config in RRC
                         *                   * connection setup to UE*/

                        if(RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT &
                                p_rrc_rrm_ue_admission_resp->radio_res_config.mac_config.bitmask)
                        {

                            RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                                    "[RRC_RRM_UE_ADMISSION_RESP] mac_main_config present");

                            if(DUPLEXING_MODE_TDD == p_ue_context->p_gb_context->p_p_csc_context[
                                    p_ue_context->cell_index]->p_csc_init_setup_ind->
                                    duplexing_mode)
                            {
                                sub_frame_assignment = p_ue_context->p_gb_context->p_p_csc_context[
                                    p_ue_context->cell_index]->p_csc_init_setup_ind->
                                    sib_type_1_Info.tdd_config.sub_frame_assignment;

                                if (RRC_TRUE == p_rrc_rrm_ue_admission_resp->radio_res_config.
                                        mac_config.mac_main_config.ul_sch_config.tti_bundling)
                                {
                                    /* TTI bundling can be enabled for FDD and for TDD only for
                                     *                      * configurations 0, 1 and 6 */
                                    if (sa_0 != sub_frame_assignment &&
                                            sa_1 != sub_frame_assignment &&
                                            sa_6 != sub_frame_assignment)
                                    {
                                        RRC_UECC_RCS_FSM_TRACE(RRC_WARNING,
                                                "[RRC_RRM_UE_ADMISSION_RESP] :"\
                                                "TTI bundling can be enabled for FDD"\
                                                " and for TDD only for"\
                                                " configurations 0, 1 and 6 ");
                                        return RRC_FAILURE;
                                    }
                                    if(RRM_UE_ADM_RADIO_RESP_SPS_CONFIG_PRESENT &
                                            p_rrc_rrm_ue_admission_resp->radio_res_config.
                                            bitmask)
                                    {
                                        /*For TDD, E-UTRAN does not simultaneously enable TTI
                                         *                          * bundling and semi-persistent scheduling */
                                        RRC_UECC_RCS_FSM_TRACE(RRC_WARNING,
                                                "For TDD, E-UTRAN does not simultaneously enable TTI"\
                                                " bundling and semi-persistent scheduling");
                                        return RRC_FAILURE;
                                    }

                                }
                            }
                            p_ue_context->mac_config =
                                p_rrc_rrm_ue_admission_resp->radio_res_config.mac_config;

                            p_ue_context->m.mac_main_config_present = 1;
                        }

                        else
                        {
                            RRC_UECC_RCS_FSM_TRACE(RRC_ERROR,
                                    "[RRC_RRM_UE_ADMISSION_RESP] mac_main_config absent");
                            return RRC_FAILURE;
                        }
                    }
                    else
                    {
                        RRC_UECC_RCS_FSM_TRACE(RRC_ERROR,
                                "[RRC_RRM_UE_ADMISSION_RESP] mac_config absent");
                        return RRC_FAILURE;
                    }
        }
    /*** TDD changes stop ***/

    if (RRC_FAILURE == fill_ue_radio_admission_resp(p_ue_context,
                p_rrc_rrm_ue_admission_resp))
    {
        return RRC_FAILURE;
    }

    /* Store wait time */
    p_ue_context->wait_time = p_rrc_rrm_ue_admission_resp->wait_time;
    
    /* Store extended wait time */

    if ( (RRM_UE_ADM_RESP_API_UE_ADM_EXTENDED_WAIT_TIME_PRESENT
                                    & p_rrc_rrm_ue_admission_resp->bitmask))
    {
        p_ue_context->m.extended_wait_time_present = 1;
        p_ue_context->extended_wait_time = p_rrc_rrm_ue_admission_resp->extended_wait_time;
    }

    

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_s1ap_allocate_mme_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                       UECC_EV_S1AP_ALLOCATE_MME_RESP
****************************************************************************/
void uecc_fsm_rcs_s1ap_allocate_mme_resp_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{

    s1ap_allocate_mme_resp_t     *allocate_mme_resp = PNULL;
    rrc_return_t result = RRC_SUCCESS;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api_data);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    allocate_mme_resp = (s1ap_allocate_mme_resp_t*)p_api_data;

    do
    {
        /* SPR 18363 Fix Start */
        p_ue_context->mme_id = allocate_mme_resp->mme_id;
        /* SPR 18363 Fix End */

        /*SPR_17633_START*/
        if (RRC_INVALID_MME_ID == allocate_mme_resp->mme_id)
        {
            RRC_UECC_RCS_FSM_TRACE(RRC_INFO, "MME ID allocation failed.");
		    result = RRC_FAILURE;
            break;
        }
        /* SPR 18168 Fix Start */
        p_ue_context->m.rel_of_mme_present = 1;
        p_ue_context->rel_of_mme = (rel_info_of_mme_et)allocate_mme_resp->rel_of_mme;
        /* SPR 18168 Fix Stop*/
        /*SPR_17633_END*/

        /* SPR_21459 Fix - Start */
        /* Update received mme_id in UE Context */
        p_ue_context->mme_id = allocate_mme_resp->mme_id;
        /* SPR_21459 Fix - End */

        /* Build and send initial UE message to S1AP */
        result = uecc_s1ap_build_and_send_initial_ue_msg_req(
                p_ue_context,
                allocate_mme_resp->rel_of_mme);

        if (  p_ue_context->nas_pdu.data != PNULL)
        {
            rrc_mem_free ((void*)p_ue_context->nas_pdu.data);
        }

        if (RRC_SUCCESS != result)
        {
            break;
        }

    /* Trace related action : Now plmn_identity has been selected in 
     * RRC Connection Setup Complete msg, so trace reference and trace 
     * recording session reference can be created */

    /* Start MME Guard Timer For Initial UE */
    if( RRC_SUCCESS != uecc_ue_timer_start(
            p_ue_context->p_gb_context, 
            UECC_UE_MME_GUARD_TIMER,
            &p_ue_context->p_gb_context->ue_timers_data,
            &p_ue_context->timers_data,
            p_ue_context->ue_index))
    {
        RRC_UECC_RCS_FSM_TRACE(RRC_ERROR, "MME Guard Timer For Initial UE failed");
        result = RRC_FAILURE;
        break;
    }
    /* SPR 18168 Fix Start */
    /* Code Removed */
    /* SPR 18168 Fix Stop */
    uecc_fsm_finish_activity_success(p_ue_context, PNULL);
} while (0);

if ( RRC_FAILURE == result)
{
    /* Here the UE connection is active and SRB1 channel is established.
     * Start UE Connection Release control procedure */
    uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_RCS_FAILURE_RELEASE_REQ);
}

RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/*CSR:00053548 changes start*/
/****************************************************************************
 * Function Name  : uecc_fsm_rcs_rrm_ue_conn_release_ind_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_RRM_UE_CONNECTION_RELEASE_IND
 ****************************************************************************/
void uecc_fsm_rcs_rrm_ue_conn_release_ind_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED, "Unused variable in fsm:%p",p_api_data);       

    RRC_UECC_RCS_FSM_TRACE(RRC_BRIEF, "RRM_UE_CONNECTION_RELEASE_IND received"
                                       "in create_ue_entity_ongoing state");
    
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rcs_fail_type3_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*CSR:00053548 changes stop*/

/*CSR:00053548 changes start*/
/****************************************************************************
 * Function Name  : uecc_fsm_rcs_rrm_ue_conn_release_ind_w_for_del_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_RRM_UE_CONNECTION_RELEASE_IND
 ****************************************************************************/
void uecc_fsm_rcs_rrm_ue_conn_release_ind_w_for_del_resp_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED, "Unused variable in fsm:%p",p_api_data);       

    /*Ignore Message*/
    RRC_UECC_RCS_FSM_TRACE(RRC_BRIEF, "Ignoring message as UE delete is already ongoing");
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_rcs_rrm_ue_conn_release_ind_conn_setup_ongoing_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_TIMER_EXPIRY
 ****************************************************************************/
void uecc_fsm_rcs_rrm_ue_conn_release_ind_conn_setup_ongoing_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED, "Unused variable in fsm:%p",p_api_data);       

    RRC_UECC_RCS_FSM_TRACE(RRC_BRIEF, "RRM_UE_CONNECTION_RELEASE_IND received"
                                       "in connection_setup_ongoing state");

    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rcs_fail_type5_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_fail_action_rrc_connection_release
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et* - p_break
 * Outputs        : None
 * Returns        : None
 * Description    : Fail action.
 ****************************************************************************/
void uecc_fsm_rcs_fail_action_rrc_connection_release(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT( PNULL != p_ue_context );
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED, "Unused variable in fsm:%p",p_break);      

    RRC_UECC_RCS_FSM_TRACE(RRC_INFO, "FAILURE: Send RRC Connection Release to UE");

    if (RRC_SUCCESS ==
            uecc_llim_build_and_send_rrc_connection_release( p_ue_context ))
    {
        RRC_UECC_RCS_FSM_TRACE(RRC_INFO,
                "RRCConnectionRelease sent to UE");
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*CSR:00053548 changes stop*/

/* SPR 15790 Fix Start */
/****************************************************************************
 * Function Name  : uecc_fsm_rcs_rrm_ue_admission_ongoing_cell_delete_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_CSC_DELETE_ALL_UE_REQ
 ****************************************************************************/
void uecc_fsm_rcs_rrm_ue_admission_ongoing_cell_delete_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_RCS_FSM_TRACE(RRC_BRIEF, "Cell Delete Event Received");

    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rcs_fail_type6_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_llim_create_ue_ongoing_cell_delete_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
*                   UECC_EV_CSC_DELETE_ALL_UE_REQ
 ****************************************************************************/
void uecc_fsm_rcs_llim_create_ue_ongoing_cell_delete_handler(
    uecc_ue_context_t   *p_ue_context,
    void                *p_api_data
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%p",
            p_api_data);

    RRC_UECC_RCS_FSM_TRACE(RRC_BRIEF, "Cell Delete Event Received");

    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_rcs_fail_type7_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_csc_cell_del_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : Handler for CSC delete all UE request.
 ****************************************************************************/
void uecc_fsm_rcs_csc_cell_del_handler(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrc_bool_et*        p_break
)
{
    /* SPR 21412 Fix Start */
    /* SPR 21412 Fix End */
    RRC_ASSERT(PNULL != p_uecc_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->facility_name,p_uecc_ue_context->p_gb_context->facility_name,
            RRC_DETAILEDALL,"Unused variable p_break=%p"
            ,p_break);/*SPR 17777 +-*/
    if ((PNULL != p_uecc_ue_context->p_gb_context) &&
            (PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context) )
    {
        /* SPR 21412 Fix Start */
        /* SPR 21412 Fix End */
        /* decrement the count_use_ue_del_cell_req parameter by 1 
         * This count will be used during UECC_CSC_DEL_ALL_UE_RESP */
        if((PNULL != p_uecc_ue_context->p_gb_context->
             p_p_csc_context[p_uecc_ue_context->cell_index]) &&
                (p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                del_cell_ongoing == 1))
        {
            if(p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    count_use_ue_del_cell_req > 0)
                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    count_use_ue_del_cell_req --;

            if(p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    count_use_ue_del_cell_req == 0)
            {
                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                    del_cell_ongoing = 0;
                uecc_csc_build_and_send_delete_all_ue_entity_resp(RRC_SUCCESS,
                        p_uecc_ue_context->p_gb_context,p_uecc_ue_context->cell_index);
                if (RRC_TRUE == p_uecc_ue_context->p_gb_context->
                        p_p_csc_context[p_uecc_ue_context->cell_index]->is_cell_stop_ongoing)
                {
                    p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->
                        is_cell_stop_ongoing = RRC_FALSE;
                }
                else
                {
                    /* Clean Cell Context */
                    if (p_uecc_ue_context->p_gb_context->p_p_csc_context != PNULL )
                    {
                        if (p_uecc_ue_context->p_gb_context->
                                p_p_csc_context[p_uecc_ue_context->cell_index] != PNULL )
                        {
                            if (p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_csc_init_setup_ind != PNULL)
                            {
                                rrc_mem_free (p_uecc_ue_context->p_gb_context->
                                        p_p_csc_context[p_uecc_ue_context->cell_index]->
                                        p_csc_init_setup_ind);
                                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_csc_init_setup_ind = PNULL;
                            }
                            if (p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_rrc_uecc_statistic != PNULL)
                            {
                                rrc_mem_free (p_uecc_ue_context->p_gb_context->
                                        p_p_csc_context[p_uecc_ue_context->cell_index]->p_rrc_uecc_statistic);
                                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_rrc_uecc_statistic = PNULL;
                            }
                            if (p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_cell_traffic_trace_info != PNULL)
                            {
                                rrc_mem_free (p_uecc_ue_context->p_gb_context->
                                        p_p_csc_context[p_uecc_ue_context->cell_index]->p_cell_traffic_trace_info);
                                p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                                    cell_index]->p_cell_traffic_trace_info = PNULL;
                            }
                            rrc_mem_free(p_uecc_ue_context->p_gb_context->
                                    p_p_csc_context[p_uecc_ue_context->cell_index]);
                            p_uecc_ue_context->p_gb_context->
                                p_p_csc_context[p_uecc_ue_context->cell_index] = PNULL;
                        }
                    }
                }
            }
        }
        /* SPR 21412 Fix Start */
        /* SPR 21412 Fix End */
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}
/* SPR 15790 Fix Stop */
/* SPR 18649 Fix Start */
/****************************************************************************
 * Function Name  : uecc_fsm_rcs_rrm_ue_admission_ongoing_reset_msg_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                   S1AP_RESET_MSG_IND during RRC Connection Establishment
 ****************************************************************************/
void uecc_fsm_rcs_rrm_ue_admission_ongoing_reset_msg_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%x",
            p_api_data);

    RRC_UECC_RCS_FSM_TRACE(RRC_BRIEF, "S1AP_RESET_MSG_IND received.");

    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_rcs_fail_type2_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_rcs_llim_create_ue_ongoing_reset_msg_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  S1AP_RESET_MSG_IND during RRC Connection Establishment
 ****************************************************************************/
void uecc_fsm_rcs_llim_create_ue_ongoing_reset_msg_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%x",
            p_api_data);
    RRC_UECC_RCS_FSM_TRACE(RRC_BRIEF, "S1AP_RESET_MSG_IND received.");

    /* RCS Timer will be stopped while calling delete_ue action */
    /* Continue failure processing */
    uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_rcs_fail_type3_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_rcs_rrc_connection_setup_ongoing_reset_msg_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the message
 *                  S1AP_RESET_MSG_IND during RRC Connection Establishment
 ****************************************************************************/
void uecc_fsm_rcs_rrc_connection_setup_ongoing_reset_msg_handler(
        uecc_ue_context_t   *p_ue_context,
        void                *p_api_data
        )
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_UECC_RCS_FSM_TRACE(RRC_DETAILED,"Unused variable in FSM:%x",
            p_api_data);

    RRC_UECC_RCS_FSM_TRACE(RRC_BRIEF, "S1AP_RESET_MSG_IND received.");

    if (PNULL != g_rrc_stats.p_rrc_stats)
    {
        /* Update the performance counter */
        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_connection_related_stats.
                rrc_connection_establishment.
                rrc_connEstabFail_cause[p_ue_context->rcs_establishmentCause],
                p_ue_context->rcs_establishmentCause, (RRC_ESTABLISHMENT_CAUSE_INDEX-1));

        RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                g_rrc_stats.p_rrc_stats->
                rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->p_gb_context->offset_uecc_mod_id].
                rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_connection_related_stats.
                rrc_connection_establishment.
                rrc_connEstabFail_sum, p_ue_context->rcs_establishmentCause,
                (RRC_ESTABLISHMENT_CAUSE_INDEX-1));

    }

    /* Continue failure processing */
    if (RRC_FALSE == p_ue_context->is_adm_cnf_sent)
    {
        uecc_fsm_start_failure_processing(
                p_ue_context,
                uecc_fsm_rcs_fail_type5_actions);
    }
    else
    {
        uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)UECC_RCS_FAILURE_RELEASE_REQ);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/* SPR 18649 Fix Stop */


