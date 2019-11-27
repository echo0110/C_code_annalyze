/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_isa_a.c,v 1.10 2010/10/13 07:03:33 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : UECC Initial security activation activity implementation
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_isa_a.c,v $
 * Revision 1.10  2010/10/13 07:03:33  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.3.4.3  2010/10/05 12:54:11  gur04518
 * Merged SPR 692
 *
 * Revision 1.3.4.2  2010/09/13 13:03:58  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.3.4.1  2010/07/21 08:34:41  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.3  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.4.4  2009/12/22 16:39:44  gur18569
 * commented field name initialization in fsm activity structures
 *
 * Revision 1.1.4.3  2009/12/08 21:20:22  ukr15916
 * AK_18 fixed
 *
 * Revision 1.1.4.2  2009/12/08 16:05:18  ukr15916
 * LG_14 fixed
 *
 * Revision 1.1.4.1  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.5  2009/11/24 12:12:50  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.2.4  2009/11/24 11:32:35  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.2.2.2.3  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.2.2.2.2  2009/11/20 19:14:40  ukr16032
 * UECC ICS failure processing updated
 *
 * Revision 1.1.2.2.2.1  2009/11/20 15:38:45  ukr16032
 * UECC FSMs updated
 *
 * Revision 1.1.2.2  2009/11/18 10:02:49  ukr16032
 * UECC IAS procedure implemented
 *
 * Revision 1.1.2.1  2009/11/16 18:32:53  ukr16032
 * UECC ISA activity implementation started
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Header File Includes
 ***************************************************************************/

#include "uecc_ue_ctx.h"
#include "uecc_isa_a.h"
#include "uecc_common.h"
#include "uecc_logger.h"
#include "uecc_fsm_engine.h"
#include "uecc_llim.h"
#include "uecc_s1ap.h"
#include "uecc_icsp.h"

/****************************************************************************
 * Local definitions
 ****************************************************************************/

#define RRC_UECC_ISA_FSM_TRACE(log_level, format, ...)\
    RRC_LOG(rrc_uecc_log_facility_name,log_level,"[UE:%u] [%s] [%s] "format,\
        p_ue_context->ue_index,\
        p_ue_context->uecc_fsm.proc_descriptor.p_active_proc->s_id,\
        p_ue_context->uecc_fsm.proc_descriptor.p_activity->s_id,\
        ##__VA_ARGS__)

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
static void uecc_fsm_isa_llim_configure_integrity_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_isa_llim_configure_integrity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_isa_ue_security_setup_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_isa_ue_security_setup_delivery_status_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);


static void uecc_fsm_isa_llim_configure_ciphering_dl_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_isa_llim_configure_ciphering_dl_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_isa_llim_configure_ciphering_security_mode_complete_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_isa_security_mode_failure_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_isa_ue_wait_for_security_setup_complete_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void
 uecc_fsm_isa_ue_wait_for_security_setup_complete_security_mode_complete_handler
(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_isa_llim_configure_ciphering_ul_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

static void uecc_fsm_isa_llim_configure_ciphering_ul_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);
static void uecc_fsm_isa_failure_llim_switch_off_sequrity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);



static void uecc_fsm_isa_fail_action_switch_security_off(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
);

static void uecc_fsm_isa_fail_action_last(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
);

static void uecc_fsm_isa_ue_security_setup_ongoing_security_mode_complete_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

/* SPR 4118 Start */

static void uecc_fsm_isa_notify_integrity_failure_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api);

/* SPR 16895 Fix Start */
extern void uecc_fsm_rrm_ue_information_req_start_timer(
    uecc_ue_context_t   *p_ue_context
);

extern void uecc_rrm_ue_information_req_stop_timer(
    uecc_ue_context_t   *p_ue_context
);
/* SPR 16895 Fix Stop */

/****************************************************************************
 Private Variables
****************************************************************************/

/**************************************
 * States definition
 **************************************/
static uecc_fsm_handler_record_t
    uecc_fsm_isa_llim_configure_integrity_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
        uecc_fsm_isa_llim_configure_integrity_ongoing_init_handler   },
    { { UECC_EV_LLIM_CONFIGURE_SECURITY_RESP   },
        uecc_fsm_isa_llim_configure_integrity_resp_handler },
    /* SPR 4118 Start */
    { { UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_isa_notify_integrity_failure_handler },
    /* SPR 4118 Stop */
    { { UECC_LAST_ID                            },  PNULL }
};

static uecc_fsm_handler_record_t uecc_fsm_isa_ue_security_setup_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
        uecc_fsm_isa_ue_security_setup_ongoing_init_handler   },
    { { UECC_EV_LLIM_SRB_DATA_STATUS_IND },
        uecc_fsm_isa_ue_security_setup_delivery_status_handler },
    { { UECC_EV_TIMER_EXPIRY },
        uecc_fsm_isa_security_mode_failure_handler },
    { { UECC_EV_RRC_SECURITY_MODE_COMPLETE },
        uecc_fsm_isa_ue_security_setup_ongoing_security_mode_complete_handler },
    { { UECC_EV_RRC_SECURITY_MODE_FAILURE },
        uecc_fsm_isa_security_mode_failure_handler },
    /* SPR 4118 Start */
    { { UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_isa_notify_integrity_failure_handler },
    /* SPR 4118 Stop */
    { { UECC_LAST_ID                            },  PNULL }
};


static uecc_fsm_handler_record_t
    uecc_fsm_isa_llim_configure_ciphering_dl_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
        uecc_fsm_isa_llim_configure_ciphering_dl_ongoing_init_handler   },
    { { UECC_EV_LLIM_CONFIGURE_SECURITY_RESP },
        uecc_fsm_isa_llim_configure_ciphering_dl_resp_handler },
    { { UECC_EV_RRC_SECURITY_MODE_COMPLETE },
        uecc_fsm_isa_llim_configure_ciphering_security_mode_complete_handler },
    { { UECC_EV_RRC_SECURITY_MODE_FAILURE },
        uecc_fsm_isa_security_mode_failure_handler },
    { { UECC_EV_TIMER_EXPIRY },
        uecc_fsm_isa_security_mode_failure_handler },
    /* SPR 4118 Start */
    { { UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_isa_notify_integrity_failure_handler },
    /* SPR 4118 Stop */
    { { UECC_LAST_ID                            },  PNULL }
};

static uecc_fsm_handler_record_t
    uecc_fsm_isa_ue_wait_for_security_setup_complete_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
        uecc_fsm_isa_ue_wait_for_security_setup_complete_init_handler   },
    { { UECC_EV_RRC_SECURITY_MODE_COMPLETE },
uecc_fsm_isa_ue_wait_for_security_setup_complete_security_mode_complete_handler
    },
    { { UECC_EV_RRC_SECURITY_MODE_FAILURE },
        uecc_fsm_isa_security_mode_failure_handler },
    { { UECC_EV_TIMER_EXPIRY },
        uecc_fsm_isa_security_mode_failure_handler },
    /* SPR 4118 Start */
    { { UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_isa_notify_integrity_failure_handler },
    /* SPR 4118 Stop */
    { { UECC_LAST_ID                            },  PNULL }
};

static uecc_fsm_handler_record_t
    uecc_fsm_isa_llim_configure_ciphering_ul_ongoing_mht[] =
{
    { { UECC_EV_STATE_INIT                      },
        uecc_fsm_isa_llim_configure_ciphering_ul_ongoing_init_handler   },
    { { UECC_EV_LLIM_CONFIGURE_SECURITY_RESP },
        uecc_fsm_isa_llim_configure_ciphering_ul_resp_handler },
    /* SPR 4118 Start */
    { { UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_isa_notify_integrity_failure_handler },
    /* SPR 4118 Stop */
    { { UECC_LAST_ID                            },  PNULL }
};

static uecc_fsm_handler_record_t
    uecc_fsm_isa_failure_llim_switch_off_sequrity_mht[] =
{
    { { UECC_EV_LLIM_CONFIGURE_SECURITY_RESP   },
        uecc_fsm_isa_failure_llim_switch_off_sequrity_resp_handler },
    /* SPR 4118 Start */
    { { UECC_EV_NOTIFY_INTEGRITY_FAILURE   },
        uecc_fsm_isa_notify_integrity_failure_handler },
    /* SPR 4118 Stop */
    { { UECC_LAST_ID                            },  PNULL }
};

static uecc_fsm_activity_state_t
    uecc_fsm_isa_llim_configure_integrity_ongoing_state =
{
    /*.s_name         = */  (S8*)"UECC_CONFIGURE_INTEGRITY_ONGOING",
    /*.s_id           = */  (S8*)"ISA_CONFIGURE_INTEGRITY",
    /*.is_need_init   =  */ RRC_TRUE,
    /*.p_mht          =  */ uecc_fsm_isa_llim_configure_integrity_ongoing_mht
};

static uecc_fsm_activity_state_t uecc_fsm_isa_ue_security_setup_ongoing_state =
{
    /*.s_name         = */  (S8*)"UECC_UE_SECURITY_SETUP_ONGOING",
    /*.s_id           = */  (S8*)"ISA_UE_SECURITY_SETUP",
    /*.is_need_init   = */  RRC_TRUE,
    /*.p_mht          = */  uecc_fsm_isa_ue_security_setup_ongoing_mht
};


static uecc_fsm_activity_state_t
    uecc_fsm_isa_llim_configure_ciphering_dl_ongoing_state =
{
    /*.s_name         = */  (S8*)"UECC_CONFIGURE_CIPHERING_DL_ONGOING",
    /*.s_id           = */  (S8*)"ISA_CONFIGURE_CIPHERING_DL",
    /*.is_need_init   = */  RRC_TRUE,
    /*.p_mht          = */  uecc_fsm_isa_llim_configure_ciphering_dl_ongoing_mht
};

static uecc_fsm_activity_state_t
    uecc_fsm_isa_ue_wait_for_security_setup_complete_state =
{
   /* .s_name         = */  (S8*)"UECC_UE_WAIT_SECURITY_SETUP_COMPLETE",
   /* .s_id           = */  (S8*)"ISA_UE_WAIT_SECURITY_SETUP_COMPLETE",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_isa_ue_wait_for_security_setup_complete_mht
};

static uecc_fsm_activity_state_t
    uecc_fsm_isa_llim_configure_ciphering_ul_ongoing_state =
{
   /* .s_name         = */  (S8*)"UECC_CONFIGURE_CIPHERING_UL_ONGOING",
   /* .s_id           = */  (S8*)"ISA_CONFIGURE_CIPHERING_UL",
   /* .is_need_init   = */  RRC_TRUE,
   /* .p_mht          = */  uecc_fsm_isa_llim_configure_ciphering_ul_ongoing_mht
};

static uecc_fsm_activity_state_t
    uecc_fsm_isa_failure_llim_switch_off_sequrity_state =
{
   /* .s_name         = */  (S8*)"UECC_SWITCH_OFF_SECURITY_ONGOING",
   /* .s_id           = */  (S8*)"ISA_SWITCH_OFF_SECURITY",
   /* .is_need_init   = */  RRC_FALSE,
   /* .p_mht          = */  uecc_fsm_isa_failure_llim_switch_off_sequrity_mht
};
/**************************************
 * Activity definition
 **************************************/
/* Activity */
uecc_fsm_activity_t uecc_fsm_initial_security_activation_activity =
{
   /* .s_name             =*/ (S8*)"Initial security activation",
   /* .s_id               =*/ (S8*)"UE_ISA_A",
   /* .p_first_state      =*/ &uecc_fsm_isa_llim_configure_integrity_ongoing_state
};

/**************************************
 * Failure actions definition
 **************************************/
static const uecc_fsm_fail_handler_f uecc_fsm_isa_failure_actions[] =
{
    uecc_fsm_isa_fail_action_switch_security_off,
    uecc_fsm_isa_fail_action_last,
    PNULL
};

/****************************************************************************
 * Local Functions Implementation
 ****************************************************************************/

/****************************************************************************
 * Function Name  : uecc_fsm_isa_llim_configure_integrity_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_isa_llim_configure_integrity_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ISA_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm:%p",p_api);

    /* Configure integrity protection on LL*/
    if (RRC_SUCCESS !=
            uecc_llim_build_and_send_configure_security_req(
                p_ue_context,
                RRC_TRUE,
                RRC_FALSE,
                (rrc_rb_direction_et)RRC_NULL, /*not used for integrity config*/
                RRC_FALSE
                ))
    {
        /* Process failure */
        /* Security configuration has't been applied -
         * it is safety to return in wait s1ap connection state*/
        uecc_fsm_finish_activity_failure(
               p_ue_context, (void*)UECC_EV_S1AP_INITIAL_CTX_SETUP_FAILURE_INT);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_isa_llim_configure_integrity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_CONFIGURE_SECURITY_RESP message.
 ****************************************************************************/
void uecc_fsm_isa_llim_configure_integrity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_configure_security_resp_t *p_configure_security_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_configure_security_resp = (rrc_uecc_llim_configure_security_resp_t*)p_api;

    if (RRC_SUCCESS == p_configure_security_resp->response_code)
    {
        /* Configure UE security */
        UECC_FSM_SET_CHILD_STATE(
                p_ue_context,
                &uecc_fsm_isa_ue_security_setup_ongoing_state, PNULL);
    }
    else
    {
        /* Process failure */
        /* Security configuration has't been applied -
         * it is safety to return in wait s1ap connection state */
        uecc_fsm_finish_activity_failure(
               p_ue_context, (void*)UECC_EV_S1AP_INITIAL_CTX_SETUP_FAILURE_INT);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_isa_ue_security_setup_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_isa_ue_security_setup_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ISA_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm:%p",p_api);

    p_ue_context->share_data.p_uecc_rm_data->
        security_mode_complete_received = RRC_FALSE;

    p_ue_context->share_data.p_uecc_rm_data->
        security_mode_failure_received = RRC_FALSE;

    /* Configure UE security mode */
    if (RRC_SUCCESS !=
        uecc_llim_build_and_send_rrc_security_mode_command(p_ue_context))
    {
        /* Process failure */
        /* The security should be disabled */
        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_isa_failure_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_isa_ue_security_setup_delivery_status_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_SRB_DATA_STATUS_IND message.
 ****************************************************************************/
void uecc_fsm_isa_ue_security_setup_delivery_status_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_srb_data_status_ind_t *p_srb_status = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_srb_status = (rrc_uecc_llim_srb_data_status_ind_t*)p_api;

    if (RRC_SUCCESS == p_srb_status->response_code)
    {
        /* Configure DL ciphering */
        UECC_FSM_SET_CHILD_STATE(
                p_ue_context,
                &uecc_fsm_isa_llim_configure_ciphering_dl_ongoing_state, PNULL);
    }
    else
    {
        /* Process failure */
        /* The security should be disabled */
        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_isa_failure_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/****************************************************************************
 * Function Name  : uecc_fsm_isa_llim_configure_ciphering_dl_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_isa_llim_configure_ciphering_dl_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ISA_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm:%p",p_api);

    p_ue_context->share_data.p_uecc_rm_data->
        security_configure_ciphering_dl_failure = RRC_FALSE;

    /* If security mode complete is received already */
    if (RRC_TRUE == p_ue_context->share_data.
            p_uecc_rm_data->security_mode_complete_received)
    {
        /* Configure DL and UL ciphering on LL*/
        if (RRC_SUCCESS !=
                uecc_llim_build_and_send_configure_security_req(
                    p_ue_context,
                    RRC_FALSE,
                    RRC_TRUE,
                    RRC_PDCP_RB_DIR_BOTH,
                    RRC_FALSE))
        {
            /* The worst case - if security is configured successfully
             * on UE,
             * but DL ciphering configuration is failed on
             * eNB*/
            uecc_fsm_finish_activity_failure(
                    p_ue_context,
                    (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
        }
        else
        {
            p_ue_context->share_data.p_uecc_rm_data->
                is_ul_dl_ciphering_configured = RRC_TRUE;
        }
    }
    /* Configure DL ciphering on LL*/
    else 
    {
        if (RRC_SUCCESS !=
                uecc_llim_build_and_send_configure_security_req(
                    p_ue_context,
                    RRC_FALSE,
                    RRC_TRUE,
                    RRC_PDCP_RB_DIR_TX,
                    RRC_FALSE))
        {
            /* Process failure */
            /* Set failure flag and wait for response from UE */
            p_ue_context->share_data.p_uecc_rm_data->
                security_configure_ciphering_dl_failure = RRC_TRUE;

            /* Wait response from UE */
            UECC_FSM_SET_CHILD_STATE(
                p_ue_context,
                &uecc_fsm_isa_ue_wait_for_security_setup_complete_state,
                PNULL);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_isa_llim_configure_ciphering_dl_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_CONFIGURE_SECURITY_RESP message.
 ****************************************************************************/
void uecc_fsm_isa_llim_configure_ciphering_dl_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_configure_security_resp_t *p_configure_security_resp = PNULL;
    /* SPR 16895 Fix Start */
    rrc_rrm_ue_information_req_t ue_info_req;
    /* SPR 16895 Fix Stop */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_configure_security_resp = (rrc_uecc_llim_configure_security_resp_t*)p_api;

    do
    {
        if (RRC_TRUE == 
                p_ue_context->share_data.p_uecc_rm_data->
                is_ul_dl_ciphering_configured)
        {
            /*
             * X2AP: The security algo received during attach procedure are used in 
             * X2AP HO so the memory context should not be freed.
             */

            if (RRC_SUCCESS == p_configure_security_resp->response_code)
            {
                /* SPR 16895 Fix Start */
                if (1 == p_ue_context->rlf_proc_flags.m.rrc_conn_setup_reconfigure_rlf_info_present)
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
                /* SPR 16895 Fix Stop */
                /*SPR_17121_START*/
                if(p_configure_security_resp->bitmask & 
                        UECC_LLIM_CONFIGURE_SECURITY_MACI_RESP_PRESENT)
                {
                    p_ue_context->mac_i = p_configure_security_resp->mac_i.mac_i; 
                }
                else
                {
                    RRC_UECC_ISA_FSM_TRACE(RRC_ERROR,"MACI not recieved ");
                     uecc_fsm_finish_activity_failure(
                             p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
                     break;
                }
                /*SPR_17121_END*/
                /* Finish ISA activity */
                uecc_fsm_finish_activity_success(p_ue_context, PNULL);
            }
            else
            {
                /* Process failure */
                uecc_fsm_finish_activity_failure(
                    p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
            }
            break;
        }
    
        if (RRC_SUCCESS != p_configure_security_resp->response_code)
        {
            /* Set failure flag and wait for response from UE */
            p_ue_context->share_data.p_uecc_rm_data->
                security_configure_ciphering_dl_failure = RRC_TRUE;
    
        }
    
        /* Wait response from UE */
        UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
        &uecc_fsm_isa_ue_wait_for_security_setup_complete_state,
            PNULL);

    }while(0);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : 
 *  uecc_fsm_isa_llim_configure_ciphering_security_mode_complete_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRC_SECURITY_MODE_COMPLETE message.
 ****************************************************************************/
void uecc_fsm_isa_llim_configure_ciphering_security_mode_complete_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ISA_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm:%p",p_api);

    p_ue_context->share_data.
        p_uecc_rm_data->security_mode_complete_received = RRC_TRUE;

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_isa_ue_security_setup_ongoing_security_mode_complete_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRC_SECURITY_MODE_COMPLETE message.
 ****************************************************************************/
void uecc_fsm_isa_ue_security_setup_ongoing_security_mode_complete_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ISA_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm:%p",p_api);

    p_ue_context->share_data.
        p_uecc_rm_data->security_mode_complete_received = RRC_TRUE;

    /* Configure DL ciphering */
    UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
            &uecc_fsm_isa_llim_configure_ciphering_dl_ongoing_state, PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_isa_security_mode_failure_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRC_SECURITY_MODE_FAILURE message.
 ****************************************************************************/
void uecc_fsm_isa_security_mode_failure_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ISA_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm:%p",p_api);

    /* Process failure */
    /* The security should be disabled */
    uecc_fsm_start_failure_processing(
        p_ue_context,
        uecc_fsm_isa_failure_actions);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_isa_ue_wait_for_security_setup_complete_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_isa_ue_wait_for_security_setup_complete_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ISA_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm:%p",p_api);

    /* Check if response on SecurityModeCommand has been received already */
    if ( RRC_TRUE ==
      p_ue_context->share_data.p_uecc_rm_data->security_mode_complete_received)
    {
        if (RRC_FALSE ==
            p_ue_context->share_data.p_uecc_rm_data->
            security_configure_ciphering_dl_failure)
    {
        /* Configure UL ciphering */
        UECC_FSM_SET_CHILD_STATE(
            p_ue_context,
                    &uecc_fsm_isa_llim_configure_ciphering_ul_ongoing_state,
                    PNULL);
        }
        else
        {
            /* The worst case - if security is configured successfully on UE,
             * but DL ciphering configuration is failed on eNB*/
            uecc_fsm_finish_activity_failure(
                p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
        }
    }
    else if (RRC_TRUE ==
        p_ue_context->share_data.p_uecc_rm_data->security_mode_failure_received)
    {
        /* The security should be disabled */
        uecc_fsm_start_failure_processing(
            p_ue_context,
            uecc_fsm_isa_failure_actions);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  :
 *  uecc_fsm_isa_ue_wait_for_security_setup_complete_security_mode_complete_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_RRC_SECURITY_MODE_COMPLETE message.
 ****************************************************************************/
void
uecc_fsm_isa_ue_wait_for_security_setup_complete_security_mode_complete_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ISA_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm:%p",p_api);

    if (RRC_FALSE == p_ue_context->share_data.p_uecc_rm_data->
                    security_configure_ciphering_dl_failure)
    {
    /* Configure UL ciphering */
    UECC_FSM_SET_CHILD_STATE(
        p_ue_context,
        &uecc_fsm_isa_llim_configure_ciphering_ul_ongoing_state, PNULL);
    }
    else
    {
        /* The worst case - if security is configured successfully on UE,
         * but DL ciphering configuration is failed on eNB*/
        uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_isa_llim_configure_ciphering_ul_ongoing_init_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_STATE_INIT message.
 ****************************************************************************/
void uecc_fsm_isa_llim_configure_ciphering_ul_ongoing_init_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ISA_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm:%p",p_api);

    /* Configure UL ciphering on LL*/
    if (RRC_SUCCESS !=
            uecc_llim_build_and_send_configure_security_req(
                p_ue_context,
                RRC_FALSE,
                RRC_TRUE,
                RRC_PDCP_RB_DIR_RX,
                RRC_FALSE))
    {
        /* Process failure */
        /* Security is not needed to be switched off - DL configuration is
         * the same on UE and eNB*/
        uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_isa_llim_configure_ciphering_ul_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_CONFIGURE_SECURITY_RESP message.
 ****************************************************************************/
void uecc_fsm_isa_llim_configure_ciphering_ul_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_configure_security_resp_t *p_configure_security_resp = PNULL;
    /* SPR 16895 Fix Start */
    rrc_rrm_ue_information_req_t ue_info_req;
    /* SPR 16895 Fix Stop */


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_configure_security_resp = (rrc_uecc_llim_configure_security_resp_t*)p_api;

    
    /*
     * X2AP: The security algo received during attach procedure are used in 
     * X2AP HO so the memory context should not be freed.
     */

    
    if (RRC_SUCCESS == p_configure_security_resp->response_code)
    {
        /* SPR 16895 Fix Start */
        if (1 == p_ue_context->rlf_proc_flags.m.rrc_conn_setup_reconfigure_rlf_info_present)
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
        /* SPR 16895 Fix Stop */
        /*SPR_17121_START*/
        if(p_configure_security_resp->bitmask & 
                UECC_LLIM_CONFIGURE_SECURITY_MACI_RESP_PRESENT)
        {
            p_ue_context->mac_i = p_configure_security_resp->mac_i.mac_i; 
        }
        else
        {
            RRC_UECC_ISA_FSM_TRACE(RRC_ERROR,"MACI not recieved ");
            uecc_fsm_finish_activity_failure(
                    p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
            return;
        }
        /*SPR_17121_END*/
        /* Finish ISA activity */
        uecc_fsm_finish_activity_success(p_ue_context, PNULL);
    }
    else
    {
        /* Process failure */
        uecc_fsm_finish_activity_failure(
            p_ue_context, (void*)UECC_EV_WAIT_FOR_RELEASE_REQ_IND_INT);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/****************************************************************************
 * Function Name  : uecc_fsm_isa_failure_llim_switch_off_sequrity_resp_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is called when FSM receives the
 *                  UECC_EV_LLIM_CONFIGURE_SECURITY_RESP message.
 ****************************************************************************/
void uecc_fsm_isa_failure_llim_switch_off_sequrity_resp_handler(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
)
{
    rrc_uecc_llim_configure_security_resp_t *p_configure_security_resp = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_configure_security_resp = (rrc_uecc_llim_configure_security_resp_t*)p_api;

    if (RRC_SUCCESS != p_configure_security_resp->response_code)
    {
        RRC_UECC_ISA_FSM_TRACE(RRC_WARNING,
                "Switch off security request failed in LLIM");
    }

    /* Continue failure processing */
    uecc_fsm_process_failure(p_ue_context);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_isa_fail_action_last
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et* - p_break      
 * Outputs        : None
 * Returns        : None
 * Description    : Last ISA failure step handling
 ****************************************************************************/
void uecc_fsm_isa_fail_action_last(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ISA_FSM_TRACE(RRC_DETAILED,"Unused variable in fsm:%p",p_break);

    RRC_UECC_ISA_FSM_TRACE(RRC_INFO, "FAILURE: END");
    /* SPR 21308 Fix Start */
    /* notify procedure about failure */
    uecc_fsm_finish_activity_failure(
        p_ue_context, (void*)UECC_EV_S1AP_INITIAL_CTX_SETUP_FAILURE_INT);
    /* SPR 21308 Fix End */
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_fsm_isa_fail_action_switch_security_off
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  rrc_bool_et* - p_break      
 * Outputs        : None
 * Returns        : None
 * Description    : Switch security off for SRB1 on LL due to ISA failure
 ****************************************************************************/
void uecc_fsm_isa_fail_action_switch_security_off(
    uecc_ue_context_t*  p_ue_context,
    rrc_bool_et*        p_break
)
{
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_ISA_FSM_TRACE(RRC_INFO, "FAILURE: Switch security off");

    *p_break = RRC_FALSE;

    /* Switch off security on LL*/
    if (RRC_SUCCESS ==
            uecc_llim_build_and_send_configure_security_req(
                p_ue_context,
                RRC_FALSE,
                RRC_FALSE,
                RRC_PDCP_RB_DIR_BOTH,
                RRC_TRUE))
    {
        /* Wait for LLIM response */
        *p_break = RRC_TRUE;

        UECC_FSM_SET_CHILD_STATE(
                p_ue_context,
                &uecc_fsm_isa_failure_llim_switch_off_sequrity_state,
                PNULL);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/* SPR 4118 Start */
/****************************************************************************
 * Function Name  : uecc_fsm_isa_notify_integrity_failure_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void - api data
 * Outputs        : None
 * Returns        : None
 * Description    : handler function for integrity failure
 ****************************************************************************/
void uecc_fsm_isa_notify_integrity_failure_handler(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api)
{
    (void)p_api;/*SPR 17777 +-*/
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_api);

    uecc_fsm_isa_fail_action_last(
        p_ue_context,
        PNULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}


/* SPR 4118 Stop */
