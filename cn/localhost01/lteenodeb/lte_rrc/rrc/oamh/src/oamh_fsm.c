/*****************************************************************************
**
**  FILE NAME     :  oamh_fsm.c
**
**  DESCRIPTION   :  This file comtains the OAMH FSM
**
**
**  DATE        AUTHOR      REF             REASON
**  --------    -------     ---             ----------
**  13-May-09   Shashank    DDv0.2          Initial
**  21-May-09   Shashank    DDv0.2          Review comments incorporated.
**  27-May-09   Shashank    DDv0.2      Formal review comments incorporated.
**  14-Oct-09   OleksandrM  DDv1.41     Global redesign
**
**     Copyright (C) 2009 Aricent Inc . All Rights Reserved
******************************************************************************/

/****************************************************************************
* Header File Includes
***************************************************************************/
#include "oamh_fsm.h"
#include "rrc_oam_il_composer.h"
#include "oamh_logger.h"
#include "oamh_timers.h"
#include "uecc_common.h"
#include "rrc_shared_stats.h"
#include "rrc_logging.h"
#if (LOGGER_TYPE == RRC_ADV_LOGGER)
#include "advlogging.h"
#include <lteLogger.h>
#endif
#include <lteMisc.h>  

/*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL
QLOCK rrc_lock;
#endif
/*Lockless Implementation stop*/

extern U8 num_active_threads;
/****************************************************************************
 Local definitions
 ****************************************************************************/

#define OAMH_OAM_INIT_IND_TIMER_DURATION 1000

/****************************************************************************
 Static functions
****************************************************************************/

static rrc_bool_et establish_all_connections(
    rrc_oam_communication_info_resp_t *,
    comm_info_t *,
    rrc_oam_communication_info_req_t *);
/* SPR 20908 Start */
static rrc_bool_et establish_all_connections_l2(
    rrc_oamh_communication_info_resp_t *,
    comm_info_t *,
    rrc_oamh_communication_info_ind_t *);
/* SPR 20908 End */
static void oamh_fsm_trace_timer_configuration(U8, rrc_oam_timer_info_t*);
static void oamh_trace_algorithms(const U8*, U8, const S8 **description);
/* lipa start */
static rrc_return_et process_and_validate_add_lgw_req
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    rrc_oam_add_lgw_req_t *p_lgw_req,
    ueccmd_oamh_add_del_lgw_ind_t *p_ueccmd_oamh_add_del_lgw_ind
);
static void oamh_oam_discard_add_lgw_req_handler(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
);
static void oamh_oam_discard_del_lgw_req_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
);
static void oamh_oam_del_lgw_req_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
);
static void oamh_oam_add_lgw_req_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
);
/* lipa end */
/****************************************************************************
 Private Variables
****************************************************************************/
const S8* OAMH_FSM_STATES_NAMES[] =
{
    (const S8*)"OAMH_INIT",
    (const S8*)"OAMH_STATE_W_FOR_COMM_PROV_REQ",
    (const S8*)"OAMH_STATE_W_FOR_CLEANUP_RESP",
    (const S8*)"OAMH_STATE_ACTIVE",
    (const S8*)"OAMH_NUM_OF_STATES"
};

const S8* OAMH_FSM_EVENTS_NAMES[] =
{
    (const S8*)"OAMH_EVENT_CSC_INIT_IND",
    (const S8*)"OAMH_EVENT_LLIM_INIT_IND",
    (const S8*)"OAMH_EVENT_UECCMD_INIT_IND",
    (const S8*)"OAMH_EVENT_OAM_COMM_INFO_REQ",
    (const S8*)"OAMH_EVENT_OAM_PROV_REQ",
    (const S8*)"OAMH_EVENT_CSC_PROV_RESP",
    (const S8*)"OAMH_EVENT_LLIM_PROV_RESP",
    (const S8*)"OAMH_EVENT_UECCMD_PROV_RESP",
    (const S8*)"OAMH_EVENT_OAM_CLEANUP_REQ",
    (const S8*)"OAMH_EVENT_CSC_CLEANUP_RESP",
    (const S8*)"OAMH_EVENT_LLIM_CLEANUP_RESP",
    (const S8*)"OAMH_EVENT_UECCMD_CLEANUP_RESP",
    (const S8*)"OAMH_EVENT_OAM_ADD_LGW_REQ", /*lipa start*/
    (const S8*)"OAMH_EVENT_OAM_DEL_LGW_REQ",/*lipa end*/
    (const S8*)"OAMH_NUM_OF_EVENTS",
    (const S8*)"OAMH_INVALID_EVENT"
};

const S8* OAMH_COMM_MODULES_NAMES[] =
{
    (const S8*)"OAM_INVALID_MODULE_ID",
    (const S8*)"OAM_MODULE_ID",
    (const S8*)"OAM_MODULE_ID_RRM",
    (const S8*)"OAM_MODULE_ID_RRC",
    (const S8*)"OAM_MODULE_ID_S1U",
    (const S8*)"OAM_MODULE_ID_PDCP",
    (const S8*)"OAM_MODULE_ID_RLC",
    (const S8*)"OAM_MODULE_ID_MAC",
    (const S8*)"OAM_MODULE_ID_PHY",
    (const S8*)"OAM_MAX_MODULE_ID"
};

const S8* OAMH_TIMERS_NAMES[] =
{
    (const S8*)"CSC_CELL_SETUP_TIMER",
    (const S8*)"LLIM_CELL_SETUP_TIMER",
    (const S8*)"UECC_RRC_CONN_SETUP_TIMER",
    (const S8*)"LLIM_CREATE_UE_ENTITY_TIMER",
    (const S8*)"UECC_RRC_CONN_RECONFIG_TIMER",
    (const S8*)"LLIM_RECONFIG_UE_ENTITY_TIMER",
    (const S8*)"UECC_RRC_CONN_RELEASE_TIMER",
    (const S8*)"LLIM_DELETE_UE_ENTITY_TIMER",
    (const S8*)"LLIM_CONFIG_UE_SECURITY_TIMER",
    (const S8*)"UECC_RRC_MME_GUARD_TIMER",
    (const S8*)"UECC_RRM_UE_CAPABILITY_ENQ_TIMER",
    (const S8*)"CSC_CELL_RECONFIG_TIMER",
    (const S8*)"LLIM_CELL_RECONFIG_TIMER",
    (const S8*)"CSC_CELL_DELETE_TIMER",
    (const S8*)"UECC_RRC_UE_CONTXT_MOD_TIMER",
    (const S8*)"UECC_RRC_ERAB_SETUP_GUARD_TIMER",
    (const S8*)"UECC_RRC_ERAB_MODIFY_GUARD_TIMER",
    (const S8*)"UECC_ERAB_RELEASE_CMD_TIMER",  /* 17 */
    (const S8*)"UECC_ERAB_RELEASE_IND_TIMER",  /* 18 */
    (const S8*)"UECC_RLF_WAIT_FOR_REEST_TIMER",      /* 19 */
    (const S8*)"UECC_RLF_UE_SUSPEND_TIMER",         /* 20 */

/*REL 1.2:HANDOVER PROCEDURE START*/
    (const S8*)"UECC_RRC_HO_PREP_TIMER", /* 21 */
    (const S8*)"UECC_RRC_HO_OVERALL_TIMER", /* 22 */
    (const S8*)"UECC_LL_GUARD_TIMER", /* 23 */
    (const S8*)"UECC_RRC_OFFSET_TIMER",  /* 24 */
    (const S8*)"UECC_MEAS_CONFIG_GUARD_TIMER", /* 25 */
    (const S8*)"UECC_RRC_TRG_HO_GUARD_TIMER",  /*26 */
/*REL 1.2:HANDOVER PROCEDURE END*/ 
    (const S8*)"LLIM_SUSPEND_UE_ENTITY_TIMER",      /* 27 */
    (const S8*)"LLIM_REESTABLISH_UE_ENTITY_TIMER", /* 28 */
    (const S8*)"LLIM_RECONFIG_SECURITY_UE_ENTITY_TIMER", /* 29 */
    (const S8*)"LLIM_RESUME_UE_ENTITY_TIMER",             /* 30 */
    (const S8*)"UECC_CRE_GUARD_TIMER",             /* 31 */
    (const S8*)"LLIM_CHANGE_CRNTI_TIMER",          /* 32*/
    (const S8*)"CSC_PWS_TIMER",                    /* 33*/
    (const S8*)"CSC_KILL_TIMER",		/* 34 */
    (const S8*)"CSC_CELL_START_TIMER",        /* 35 */
    (const S8*)"LLIM_CELL_START_TIMER",       /* 36 */
    (const S8*)"CSC_CELL_STOP_TIMER",         /* 37 */
    (const S8*)"LLIM_CELL_STOP_TIMER",         /* 38 */
    /*x2_ho_timer: missing in 3.4 release*/
    (const S8*)"LLIM_CELL_DELETE_TIMER",         /* 39 */
    /*x2_ho_timer*/
    (const S8*)"UECC_RRC_X2_HO_PREP_TIMER",  /* 40 */
    (const S8*)"UECC_RRC_X2_HO_OVERALL_TIMER",  /* 41 */
    (const S8*)"UECC_RRC_INTRA_CELL_HO_GUARD_TIMER",  /* 42 */
    (const S8*)"UECC_RRM_UE_INFORMATION_REQ_TIMER"  /* 43 */
};

const S8* OAMH_LOG_LEVEL_NAMES[] =
{
    (const S8*)"OAM_LOG_LEVEL_FATAL",
    (const S8*)"OAM_LOG_LEVEL_ERROR",
    (const S8*)"OAM_LOG_LEVEL_WARNING",
    (const S8*)"OAM_LOG_LEVEL_INFO",
    (const S8*)"OAM_LOG_LEVEL_BRIEF",
    (const S8*)"OAM_LOG_LEVEL_DETAILED",
    (const S8*)"OAM_LOG_LEVEL_DETAILEDALL"
};

const S8* OAMH_INT_ALGORITHM_NAMES[] =
{
    (const S8*)"RRC_PDCP_INT_ALG_EIA0",
    (const S8*)"RRC_PDCP_INT_ALG_EIA1",
    (const S8*)"RRC_PDCP_INT_ALG_EIA2"
};

const S8* OAMH_CIPH_ALGORITHM_NAMES[] =
{
    (const S8*)"RRC_PDCP_CIPH_ALG_EEA0",
    (const S8*)"RRC_PDCP_CIPH_ALG_EEA1",
    (const S8*)"RRC_PDCP_CIPH_ALG_EEA2"
};

const S8* THREAD_NAMES[] =
{
    (const S8*)"rrc_msg_dstbtr",
    (const S8*)"rrc_main",
    (const S8*)"rrc_cdma",
    (const S8*)"rrc_uecc_1",
    (const S8*)"rrc_logger",
    (const S8*)"rrc_uecc_3",
    (const S8*)"rrc_uecc_4",
    (const S8*)"rrc_uecc_5",
    (const S8*)"rrc_uecc_6",
    (const S8*)"rrc_uecc_7",
    (const S8*)"rrc_uecc_8",
    (const S8*)"rrc_uecc_9"
};

const S8* SHARED_MEMORY_NAMES[] =
{
    (const S8*)"L3_SHARED_MEMORY_1",
    (const S8*)"L3_SHARED_MEMORY_2",
    (const S8*)"L3_SHARED_MEMORY_3",
    (const S8*)"L3_SHARED_MEMORY_4",
    (const S8*)"L3_SHARED_MEMORY_5",
    (const S8*)"L3_SHARED_MEMORY_6",
    (const S8*)"L3_SHARED_MEMORY_7",
    (const S8*)"L3_SHARED_MEMORY_8",
    (const S8*)"L3_SHARED_MEMORY_9",
    (const S8*)"L3_SHARED_MEMORY_10",
    (const S8*)"L3_SHARED_MEMORY_11",
    (const S8*)"L3_SHARED_MEMORY_12"
};

void *health_monitoring (void)/*SPR 17777 +-*/
{
    /* function to be used for health monitoring */
    rrc_update_health_monitor_variable(ADV_LOGGER_HEALTH_MONITOR_EXPIRY_EVENT);
    return RRC_PNULL;
}

/******************************************************************************
*   FUNCTION NAME: rrc_oamh_send_internal_msg
*
*   INPUTS        : rrc_module_id_t   dst_module_id
*                   U16               api_id
*                   U16               msg_size
*                   void              *p_msg
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function sends message to RRC internal modules
*
*   RETURNS:
*       RRC_SUCCESS - message was send successfully
*       RRC_FAILURE - failed to send message
******************************************************************************/
rrc_return_t rrc_oamh_send_internal_msg
(
    rrc_module_id_t   dst_module_id,  /* Destination module identifier */
    U16               api_id,         /* API Id */
    U16               msg_size,       /* Size of message (payload) */
    void              *p_msg          /* Message specific info (payload) */
)
{
    U8 *p_api          = PNULL;
    U16 msg_api_length = (U16)(msg_size + RRC_API_HEADER_SIZE);

    RRC_OAMH_UT_TRACE_ENTER();

    /* Allocate buffer */
    p_api = rrc_msg_mem_get(msg_api_length);

    if (PNULL == p_api)
    {
        return RRC_FAILURE;
    }

    memset_wrapper(p_api, 0, msg_api_length);

    rrc_construct_api_header(p_api, RRC_VERSION_ID, RRC_OAMH_MODULE_ID,
                             dst_module_id, api_id, msg_api_length);

    if((0 != msg_size) &&  (PNULL != p_msg))
    {
        l3_memcpy_wrapper(p_api + RRC_API_HEADER_SIZE, p_msg, msg_size);
    }

    /* Send message to */
    rrc_send_message(p_api, dst_module_id);

    RRC_OAMH_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oamh_reset_context
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function set default values for OAMH global context
*
*   RETURNS:
*       None
******************************************************************************/
void oamh_reset_context
(
    rrc_oamh_gb_context_t *p_oamh_gb_context /*OAMH global context*/
)
{

/*valgrind_fix start*/
    U8 cell_index = RRC_NULL;
/*valgrind_fix stop*/
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);

    p_oamh_gb_context->init_ind              = 0;
    p_oamh_gb_context->comm_prov_info_flag   = 0;
    p_oamh_gb_context->prov_status           = 0;
    p_oamh_gb_context->cleanup_status        = 0;
    p_oamh_gb_context->prov_req_trans_id     = 0;
    p_oamh_gb_context->comm_req_trans_id     = 0;
    p_oamh_gb_context->cleanup_req_trans_id  = 0;
    p_oamh_gb_context->oam_init_ind_duration = OAMH_OAM_INIT_IND_TIMER_DURATION;
    /*lipa start */
    p_oamh_gb_context->lgw_req_trans_id = RRC_NULL;
    p_oamh_gb_context->lgw_configured_status = RRC_NULL;
    /*lipa end*/
    
    memset_wrapper (&p_oamh_gb_context->rrc_oamh_stats_status, 0,
            sizeof(rrc_oamh_stats_t));
    memset_wrapper (&p_oamh_gb_context->timers_data, 0,
            sizeof(oamh_timer_data_t));

    if(PNULL != p_oamh_gb_context->oam_init_ind_timer)
    {
        rrc_stop_timer(p_oamh_gb_context->oam_init_ind_timer);
        p_oamh_gb_context->oam_init_ind_timer = PNULL;
    }

    /*p_oamh_gb_context->p_comm_info_int - leave as is*/

#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    /* Cleanup file logging if exist and switch to console logging */
    if (RRC_LOG_CTXT_CONSOLE < g_advl_ctxt_rrc)
    {
        deinit_log(g_advl_ctxt_rrc);
        g_advl_ctxt_rrc = RRC_LOG_CTXT_CONSOLE;
    }
#endif /* (LOGGER_TYPE == RRC_ADV_LOGGER) */
#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    oamh_change_logging_parameters(p_oamh_gb_context, RRC_BRIEF, RRC_TRUE);
#endif
/*valgrind_fix start*/
    if(PNULL != (p_oamh_gb_context->p_p_csc_cell_ctx))
    {
	    if(PNULL != (*(p_oamh_gb_context->p_p_csc_cell_ctx)))
	    {
		    RRC_OAMH_TRACE(RRC_INFO, "p_oamh_gb_context->p_p_csc_cell_ctx[cell_index]=[%p] freed",
				    p_oamh_gb_context->p_p_csc_cell_ctx[cell_index]);
		    rrc_mem_free(p_oamh_gb_context->p_p_csc_cell_ctx[cell_index]);
		    p_oamh_gb_context->p_p_csc_cell_ctx[cell_index] = PNULL;

		    rrc_mem_free(p_oamh_gb_context->p_p_csc_cell_ctx);
		    p_oamh_gb_context->p_p_csc_cell_ctx = PNULL;
	    }
    }
/*valgrind_fix stop*/
   
    /*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL
    if(PNULL != rrc_lock)
    {
        qvDestroyLock(rrc_lock);
        rrc_lock = PNULL;
    }
#endif
    /*Lockless Implementation stop*/
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: check_for_oamh_active_state
*  
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function checks preconditions for switching FSM to
*       OAMH_STATE_ACTIVE state and perform state's switching
*       if it necessary
*
*   RETURNS:
*       None
******************************************************************************/
static void check_for_oamh_active_state
(
    rrc_oamh_gb_context_t*  p_oamh_gb_context
)
{
    U8 cell_index = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT( PNULL != p_oamh_gb_context );

    /* If both Communication and provision responses have been sent */
    if ((p_oamh_gb_context->comm_prov_info_flag & OAMH_COMM_RECD)
            && (p_oamh_gb_context->comm_prov_info_flag & OAMH_PROV_RECD))
    {
        /*Reset current state's flags*/
        p_oamh_gb_context->comm_prov_info_flag = 0;
        p_oamh_gb_context->prov_status = 0;
        if(PNULL != p_oamh_gb_context->p_p_csc_cell_ctx)
        {
            for(cell_index = 0;cell_index < MAX_NUM_CELLS;cell_index++)
            {
                if(PNULL != p_oamh_gb_context->p_p_csc_cell_ctx[cell_index])
                {
                    p_oamh_gb_context->p_p_csc_cell_ctx[cell_index]->
                        ready_condition = RRC_TRUE;   
                }
            }
        }
        /*Change state*/
        OAMH_SET_FSM_STATE(p_oamh_gb_context,OAMH_STATE_ACTIVE);
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: check_for_oamh_comm_prov_state
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function checks preconditions for switching FSM to
*       OAMH_STATE_W_FOR_COMM_PROV_REQ state and perform state's switching
*       if it necessary
*
*   RETURNS:
*       None
******************************************************************************/
static void check_for_oamh_comm_prov_state
(
    rrc_oamh_gb_context_t*  p_oamh_gb_context
)
{
    RRC_OAMH_UT_TRACE_ENTER();
    /*valgrind_fix start*/
    oamh_timer_buf_t repeated_timer_buf;

    repeated_timer_buf.timer_type = OAMH_INIT_TIMER ;
    repeated_timer_buf.auto_delete = RRC_FALSE;
    repeated_timer_buf.target_submodule = OAMH_SUBMODULE;
    /*valgrind_fix stop*/

    RRC_ASSERT( PNULL != p_oamh_gb_context );
    /* Check if init_indication messages received from all RRC modules */
    if ((p_oamh_gb_context->init_ind & OAMH_CSC_INIT_RECD)
         && (p_oamh_gb_context->init_ind & OAMH_LLIM_INIT_RECD)
         && (p_oamh_gb_context->init_ind & OAMH_UECCMD_INIT_RECD))
    {
        /* Start repeated timer for sending CSC_OAMH_INIT_IND */
        if(PNULL == p_oamh_gb_context->oam_init_ind_timer)
        {
	    /*valgrind_fix start*/
            p_oamh_gb_context->oam_init_ind_timer =
                rrc_start_timer(p_oamh_gb_context->oam_init_ind_duration,
                &repeated_timer_buf,
                sizeof (oamh_timer_buf_t),
                RRC_TRUE);
	    /*valgrind_fix stop*/
        }
        else
        {
            RRC_OAMH_TRACE(RRC_WARNING, "oam_init_ind_timer already exist!");
        }

        /*Reset current state's flags*/
        p_oamh_gb_context->init_ind = 0;
        /*Change state*/
        OAMH_SET_FSM_STATE(p_oamh_gb_context,OAMH_STATE_W_FOR_COMM_PROV_REQ);
    }
    RRC_OAMH_UT_TRACE_EXIT();
}
/* SPR 20908 Start */
/* code removed */
/* SPR 20908 End */

/******************************************************************************
*   FUNCTION NAME: oamh_oam_comm_req_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION: This function handles the communicaton req
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_comm_req_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    rrc_oam_communication_info_resp_t rrc_oam_communication_info_resp;
    rrc_oam_communication_info_req_t *p_rrc_oam_communication_info_req = PNULL;
    /* SPR 20908 Start */
    U8       result = RRC_SUCCESS;
    /* SPR 20908 End */

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);
    
    memset_wrapper(&rrc_oam_communication_info_resp, RRC_NULL, 
            sizeof(rrc_oam_communication_info_resp_t));

    p_rrc_oam_communication_info_req =
                   (rrc_oam_communication_info_req_t *)(p_msg);

    /* Stop oam_init_ind timer if exist */
    if(PNULL != p_oamh_gb_context->oam_init_ind_timer)
    {
        rrc_stop_timer(p_oamh_gb_context->oam_init_ind_timer);
        p_oamh_gb_context->oam_init_ind_timer = PNULL;
    }

    /* SPR 20908 Start */
    if (result != RRC_FAILURE)
    {
        /* SPR 20908 End */
        if(RRC_TRUE == establish_all_connections(
                    &rrc_oam_communication_info_resp,
                    p_oamh_gb_context->p_comm_info_int,
                    p_rrc_oam_communication_info_req))
        {
            p_oamh_gb_context->comm_prov_info_flag |= OAMH_COMM_RECD;
            /*Update CSC_Cell status index***********/
        }
        else
        {
            RRC_OAMH_TRACE(RRC_WARNING, "Not all connections are established!");
        }
    }
    /* Send back RRC_OAM_COMMUNICATION_INFO_RESP */
    rrc_oam_il_send_rrc_oam_communication_info_resp(
                        &rrc_oam_communication_info_resp,
                        RRC_OAMH_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        p_oamh_gb_context->comm_req_trans_id,
                        RRC_NULL);

    check_for_oamh_active_state(p_oamh_gb_context);

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_dump_oam_provision_req
*   INPUTS       : rrc_oam_provision_req_t  *p_rrc_oam_prov_req
*   OUTPUTS      : None
*
*   DESCRIPTION: This function puts detailed information about
*                RRC_OAM_PROVISION_REQ into LOG
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_dump_oam_provision_req
(
    rrc_oam_provision_req_t  *p_rrc_oam_prov_req
)
{
    /* SPR 20908 Start */
    rrc_oam_integrity_algorithms_t* p_int_algs  = PNULL;
    rrc_oam_ciphering_algorithms_t* p_ciph_algs = PNULL;
    /* SPR 20908 End */
/*SPR 21653 changes start*/
    U8      cell_index = RRC_NULL;
    U8      core_index = RRC_NULL;
/*SPR 21653 changes end*/

    RRC_ASSERT(PNULL != p_rrc_oam_prov_req);

/*SPR 21653 changes start*/
    for (cell_index=0; cell_index<p_rrc_oam_prov_req->sector_info_list.count;
            cell_index++)
    {
        p_int_algs =
            &(p_rrc_oam_prov_req->sector_info_list.sector_info[cell_index].
                    supported_security_algorithms.integrity_algorithms);
        p_ciph_algs =
            &(p_rrc_oam_prov_req->sector_info_list.sector_info[cell_index].
                    supported_security_algorithms.ciphering_algorithms);

        RRC_OAMH_TRACE(RRC_INFO, "INTEGRITY ALGORITHMS for Cell Id: %u", 
                p_rrc_oam_prov_req->sector_info_list.sector_info[cell_index].cell_index);
        oamh_trace_algorithms(p_int_algs->algorithms,
                p_int_algs->num_algorithms,
                OAMH_INT_ALGORITHM_NAMES);

        RRC_OAMH_TRACE(RRC_INFO, "CIPHERING ALGORITHMS for Cell Id: %u", 
                p_rrc_oam_prov_req->sector_info_list.sector_info[cell_index].cell_index);
        oamh_trace_algorithms(p_ciph_algs->algorithms,
                p_ciph_algs->num_algorithms,
                OAMH_CIPH_ALGORITHM_NAMES);

        RRC_OAMH_TRACE(RRC_INFO, "Maximum number of supported UEs: %u for Cell Id: %u",
                p_rrc_oam_prov_req->sector_info_list.sector_info[cell_index].
                max_num_supported_ue, p_rrc_oam_prov_req->sector_info_list.sector_info[cell_index].cell_index);
    }

    RRC_OAMH_TRACE(RRC_INFO, "Number of UECC Instances: %u",
            p_rrc_oam_prov_req->no_of_uecc_instances);
    for (core_index = 0; core_index < p_rrc_oam_prov_req->no_of_uecc_instances; core_index++)
    {
        RRC_OAMH_TRACE(RRC_INFO, "Core No. of %u UECC Instances: %u",
                           core_index, p_rrc_oam_prov_req->core_no[core_index]);
    }
    RRC_OAMH_TRACE(RRC_INFO, "Core No. of Rest of L3 Modules: %u",
                             p_rrc_oam_prov_req->rest_rrc_core_no);
/*SPR 21653 changes end*/

    RRC_OAMH_TRACE(RRC_INFO, "Log level: %s",
        OAMH_LOG_LEVEL_NAMES[p_rrc_oam_prov_req->log_level]);

    RRC_OAMH_TRACE(RRC_INFO, "phy_sync_mode: %u",
        p_rrc_oam_prov_req->phy_sync_mode);


    RRC_OAMH_TRACE(RRC_INFO, "UECC timers configuration:");
    oamh_fsm_trace_timer_configuration(
            p_rrc_oam_prov_req->uecc_timer_info.num_of_timers,
            p_rrc_oam_prov_req->uecc_timer_info.timer_info
            );
    RRC_OAMH_TRACE(RRC_INFO, "LLIM timers configuration:");
    oamh_fsm_trace_timer_configuration(
            p_rrc_oam_prov_req->llim_timer_info.num_of_timers,
            p_rrc_oam_prov_req->llim_timer_info.timer_info
            );
    RRC_OAMH_TRACE(RRC_INFO, "CSC timers configuration:");
    oamh_fsm_trace_timer_configuration(
            p_rrc_oam_prov_req->csc_timer_info.num_of_timers,
            p_rrc_oam_prov_req->csc_timer_info.timer_info
            );

    RRC_ASSERT(sizeof(OAMH_INT_ALGORITHM_NAMES)/sizeof(S8*)
                == RRC_MAX_INTEGRITY_ALGORITHMS);
    RRC_ASSERT(sizeof(OAMH_CIPH_ALGORITHM_NAMES)/sizeof(S8*)
                == RRC_MAX_CIPHERING_ALGORITHMS);

    RRC_OAMH_TRACE(RRC_INFO," Maximum retry count for csc reconfiguration : %u",
       p_rrc_oam_prov_req->max_cell_reconfig_retry_count );      

#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    if(RRC_OAM_SHARED_MEM_NAME_PRESENT & p_rrc_oam_prov_req->bitmask)
    {
        RRC_OAMH_TRACE(RRC_INFO, "Maximum Shared memory size is : %u",
                   p_rrc_oam_prov_req->shared_mem_logging_config.SharedMemSize);
        RRC_OAMH_TRACE(RRC_INFO, "Maximum Log file size is : %u",
                   p_rrc_oam_prov_req->shared_mem_logging_config.MaxLogFileSize);
        RRC_OAMH_TRACE(RRC_INFO, "Bitmask value : %u",
                   p_rrc_oam_prov_req->shared_mem_logging_config.bitmask);
        RRC_OAMH_TRACE(RRC_INFO, "Log file path : %s",
                   p_rrc_oam_prov_req->shared_mem_logging_config.LogFilePath);
        RRC_OAMH_TRACE(RRC_INFO, "Core number   : %u",
                   p_rrc_oam_prov_req->shared_mem_logging_config.core_no);
        RRC_OAMH_TRACE(RRC_INFO, "Maximum Log File count  : %u",
                   p_rrc_oam_prov_req->shared_mem_logging_config.MaxLogFileCount);
    }
#endif
}
/* SPR 20908 Start */
/******************************************************************************
*   FUNCTION NAME: oamh_csc_comm_info_ind_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION: This function handles the communicaton req from CSC
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_csc_comm_info_ind_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    rrc_oamh_communication_info_resp_t rrc_oamh_communication_info_resp;
    rrc_oamh_communication_info_ind_t *p_rrc_oamh_communication_info_ind = PNULL;
    
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);
    
    memset_wrapper(&rrc_oamh_communication_info_resp, RRC_NULL, 
            sizeof(rrc_oamh_communication_info_resp_t));

    p_rrc_oamh_communication_info_ind =
	    (rrc_oamh_communication_info_ind_t *)((U8*)p_msg + RRC_API_HEADER_SIZE);


    if(RRC_TRUE == establish_all_connections_l2(
			    &rrc_oamh_communication_info_resp,
			    p_oamh_gb_context->p_comm_info_int,
			    p_rrc_oamh_communication_info_ind))
    {
	    p_oamh_gb_context->comm_prov_info_flag |= OAMH_COMM_RECD;
	    /*Update CSC_Cell status index***********/
    }
    else
    {
	    RRC_OAMH_TRACE(RRC_WARNING, "Not all connections are established!");
    }
/* SPR 20908 Start */
    /* Send back RRC_OAMH_COMMUNICATION_INFO_RESP */
    rrc_oamh_send_internal_msg(RRC_CSC_MODULE_ID,
		    CSC_OAMH_OAM_COMMUNICATION_INFO_RESP,
		    sizeof(rrc_oamh_communication_info_resp_t),
		    &rrc_oamh_communication_info_resp);
/* SPR 20908 End */

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_module_timers_handler
*
*   DESCRIPTION: This function updates oam module timers during provisiong
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_module_timers_handler(
        rrc_oamh_gb_context_t       *p_oamh_gb_context,
        rrc_oam_provision_req_t     *p_rrc_oam_prov_req,
        rrc_ueccmd_oamh_prov_req_t  *ueccmd_oamh_prov_req)
{
    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_rrc_oam_prov_req);
    RRC_ASSERT(PNULL != ueccmd_oamh_prov_req);
	if (RRC_OAM_MODULE_TIMERS_INFO & p_rrc_oam_prov_req->bitmask)
	{
		ueccmd_oamh_prov_req->uecc_oamh_prov_req.bitmask |=
			RRC_UECC_TIMERS_INFO;

		if (RRC_OAM_UECC_INTRA_CELL_HO_GUARD_TIMER_PRESENT & 
				p_rrc_oam_prov_req->module_timers_info.bitmask)
		{
			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.bitmask |=
				RRC_UECC_INTRA_CELL_HO_GUARD_TIMER_PRESENT;

			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.
				uecc_intra_cell_ho_guard_timer = p_rrc_oam_prov_req->
				module_timers_info.uecc_intra_cell_ho_guard_timer;
		}

		if (RRC_OAM_UECC_UE_RECONFIG_TIMER_PRESENT & 
				p_rrc_oam_prov_req->module_timers_info.bitmask)
		{
			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.bitmask |=
				RRC_UECC_UE_RECONFIG_TIMER_PRESENT;

			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.
				uecc_ue_reconfig_timer = p_rrc_oam_prov_req->
				module_timers_info.uecc_ue_reconfig_timer;
		}
		if (p_rrc_oam_prov_req->module_timers_info.bitmask &
				RRC_OAM_UECC_INTEGRITY_THRESHOLD_TIMER_PRESENT)
		{
			ueccmd_oamh_prov_req->uecc_oamh_prov_req.bitmask |=
				RRC_UECC_INTEGRITY_TIMER_VALUE_PRESENT;
			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_integrity_timer_value =
				p_rrc_oam_prov_req-> module_timers_info.uecc_integrity_threshold_timer_value;
		}
		if (RRC_OAM_UECC_CCO_GUARD_TIMER_PRESENT &
				p_rrc_oam_prov_req->module_timers_info.bitmask)
		{
			ueccmd_oamh_prov_req->uecc_oamh_prov_req.
				uecc_timers_info.bitmask |= RRC_UECC_CCO_GUARD_TIMER_PRESENT;

			ueccmd_oamh_prov_req->uecc_oamh_prov_req.
				uecc_timers_info.uecc_cco_guard_timer = 
				p_rrc_oam_prov_req->module_timers_info.uecc_cco_guard_timer_value;
		}
		if (RRC_OAM_UECC_UE_COUNTER_CHECK_TIMER_PRESENT & 
				p_rrc_oam_prov_req->module_timers_info.bitmask)
		{
			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.bitmask |=
				RRC_UECC_COUNTER_CHECK_TIMER_PRESENT;

			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.
				uecc_counter_check_timer = p_rrc_oam_prov_req->
				module_timers_info.uecc_ue_counter_check_timer;
		}
		if (RRC_OAM_UECC_UE_INFORMATION_TIMER_PRESENT & 
				p_rrc_oam_prov_req->module_timers_info.bitmask)
		{
			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.bitmask |=
				RRC_UECC_UE_INFORMATION_TIMER_PRESENT;

			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.
				uecc_ue_information_timer = p_rrc_oam_prov_req->
				module_timers_info.uecc_ue_information_timer;
		}
		if (RRC_OAM_UECC_STORE_UE_CONTEXT_TIMER_PRESENT &
				p_rrc_oam_prov_req->module_timers_info.bitmask)
		{
			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.bitmask |=
				RRC_UECC_STORE_UE_CONTEXT_TIMER_PRESENT;

			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.
				uecc_store_ue_context_timer = p_rrc_oam_prov_req->
				module_timers_info.uecc_store_ue_context_timer;
		}
                if (RRC_OAM_UECC_HEALTH_MONITORING_TIMER_PRESENT &
                        p_rrc_oam_prov_req->module_timers_info.bitmask)
                {
                    start_oamh_health_monitor_timer(p_oamh_gb_context,
                        ((p_rrc_oam_prov_req->module_timers_info.rrc_health_mon_time_int * 1000)/2));
                    ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.bitmask |=
                        RRC_UECC_HEALTH_MONITORING_TIMER_PRESENT;

                    ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.
                        uecc_health_monitor_timer_value = p_rrc_oam_prov_req->
                        module_timers_info.rrc_health_mon_time_int;
                }
#ifdef ENDC_ENABLED
        /* MENB_CHANGES_START */
		if (RRC_OAM_UECC_DC_PREP_TIMER_PRESENT &
				p_rrc_oam_prov_req->module_timers_info.bitmask)
		{
			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.bitmask |=
				RRC_UECC_DC_PREP_TIMER_PRESENT;

			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.
				uecc_dc_prep_timer = p_rrc_oam_prov_req->
				module_timers_info.uecc_dc_prep_timer;
		}
        /* MENB_CHANGES_END */
/* OPTION3X Changes Start */
		if (RRC_OAM_UECC_DC_BEARER_CHANGE_GUARD_TIMER &
				p_rrc_oam_prov_req->module_timers_info.bitmask)
		{
			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.bitmask |=
				RRC_UECC_DC_BEARER_CHANGE_GUARD_PRESENT;

			ueccmd_oamh_prov_req->uecc_oamh_prov_req.uecc_timers_info.
				uecc_dc_bearer_change_guard_timer = p_rrc_oam_prov_req->
				module_timers_info.uecc_dc_bearer_change_guard_timer;
		}
/* OPTION3X Changes End */
#endif
	}
  
    RRC_OAMH_UT_TRACE_EXIT();
}

#if (LOGGER_TYPE == RRC_ADV_LOGGER)
/******************************************************************************
*   FUNCTION NAME: oamh_oam_prov_req_hdlr_rrc_adv_logger
*
*   DESCRIPTION: This function updates oam provisiong info when
*                LOGGER_TYPE == RRC_ADV_LOGGER
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_prov_req_hdlr_rrc_adv_logger(
        rrc_oam_provision_req_t     *p_rrc_oam_prov_req,
        rrc_ueccmd_oamh_prov_req_t  *ueccmd_oamh_prov_req,
        rrc_bool_et                 *data_is_correct,
        rrc_bool_et                 *is_shared_mem_log)
{
    U16                         log_index = RRC_NULL;
    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_rrc_oam_prov_req);
    RRC_ASSERT(PNULL != ueccmd_oamh_prov_req);
    /* logging to file */
    if (p_rrc_oam_prov_req->bitmask & RRC_OAM_CONFIG_OPTION_PRESENT)
    {
        if (p_rrc_oam_prov_req->advance_logging_config_option == RRC_OAM_LOGGER_OPTION_FILE)
        {
            if((p_rrc_oam_prov_req->bitmask & RRC_OAM_LOG_FILE_NAME_PRESENT))
            {
                U16 loop                         = RRC_NULL;
                rrc_bool_et file_name_is_correct = RRC_FALSE;

                for(loop = 0; loop < sizeof(p_rrc_oam_prov_req->log_file_name); loop++)
                {
                    if ('\0' == p_rrc_oam_prov_req->log_file_name[loop])
                    { 
                        file_name_is_correct = RRC_TRUE;
                        break;
                    }
                }

                if (RRC_TRUE == file_name_is_correct)
                {
                    /* If file context exists - deinit it */
                    if (RRC_LOG_CTXT_CONSOLE < g_advl_ctxt_rrc)
                    {
                        deinit_log(g_advl_ctxt_rrc);
                        g_advl_ctxt_rrc = RRC_LOG_CTXT_CONSOLE;
                    }
                    if (LogSuccess == init_log(LogTarget_File, (char *)p_rrc_oam_prov_req
                                ->log_file_name, &log_index))
                    {
                        g_advl_ctxt_rrc = log_index;

                        ueccmd_oamh_prov_req->uecc_oamh_prov_req.bitmask |=  
                            RRC_UECC_LOG_FILE_NAME_PRESENT;

                        l3_memcpy_wrapper(ueccmd_oamh_prov_req->uecc_oamh_prov_req.log_file_name,
                                p_rrc_oam_prov_req->log_file_name,
                                sizeof(p_rrc_oam_prov_req->log_file_name));

                    }
                    else
                    {
                        RRC_OAMH_TRACE(RRC_WARNING, "Couldn't init file logging!");
                        *data_is_correct = RRC_FALSE;
                    }
                }
                else
                {
                    RRC_OAMH_TRACE(RRC_WARNING, "Non zero terminated name of file "
                            "passed!");
                    *data_is_correct = RRC_FALSE;
                }
            } 
            else 
            {
                /* If file context exists - deinit it */
                if (RRC_LOG_CTXT_CONSOLE < g_advl_ctxt_rrc)
                {
                    deinit_log(g_advl_ctxt_rrc);
                    g_advl_ctxt_rrc = RRC_LOG_CTXT_CONSOLE;
                }
                RRC_OAMH_TRACE(RRC_WARNING, "RRC_LOG_FILE_NAME not present");
                *data_is_correct = RRC_FALSE;
            }
        } 
        else if (p_rrc_oam_prov_req->advance_logging_config_option == RRC_OAM_LOGGER_OPTION_NETWORK)
        {
            /* logging to file */
            if (p_rrc_oam_prov_req->bitmask & RRC_OAM_NETWORK_IP_PORT_PRESENT)
            {
                U16 loop                         = RRC_NULL;
                rrc_bool_et network_ip_port_is_correct = RRC_FALSE;

                for(loop = 0; loop < sizeof(p_rrc_oam_prov_req->network_ip_port); loop++)
                {
                    if ('\0' == p_rrc_oam_prov_req->network_ip_port[loop])
                    {
                        network_ip_port_is_correct = RRC_TRUE;
                        break;
                    }
                } 

                if (RRC_TRUE == network_ip_port_is_correct)
                {
                    if (LogSuccess == init_log(LogTarget_Network, (char *)p_rrc_oam_prov_req
                                ->network_ip_port, &log_index))
                    {
                        g_advl_ctxt_rrc = log_index;
                        
                        ueccmd_oamh_prov_req->uecc_oamh_prov_req.bitmask |=  
                            RRC_UECC_NETWORK_INDEX_PRESENT;

                        ueccmd_oamh_prov_req->uecc_oamh_prov_req.network_index =
                                  log_index;
                    }
                    else
                    {
                        RRC_OAMH_TRACE(RRC_WARNING, "Couldn't init Network logging!");
                        *data_is_correct = RRC_FALSE;
                    }
                }
                else
                {
                    RRC_OAMH_TRACE(RRC_WARNING, "Non zero terminated Network IP and Port "
                            "passed!");
                    *data_is_correct = RRC_FALSE;
                }
            }
            else
            {
                /* If file context exists - deinit it */
                if (RRC_LOG_CTXT_CONSOLE < g_advl_ctxt_rrc)
                {
                    deinit_log(g_advl_ctxt_rrc);
                    g_advl_ctxt_rrc = RRC_LOG_CTXT_CONSOLE;
                }
                RRC_OAMH_TRACE(RRC_WARNING, "RRC_NETWORK_IP_PORT not present");
                *data_is_correct = RRC_FALSE;
            }
        }
        else if(p_rrc_oam_prov_req->advance_logging_config_option ==
                RRC_OAM_LOGGER_OPTION_SHARED_MEM)
        {
            /* Providing shared memory to each module */
            if (p_rrc_oam_prov_req->bitmask & RRC_OAM_SHARED_MEM_NAME_PRESENT)
            {
                *is_shared_mem_log = RRC_TRUE;
            }
        }
        else
        {
            /* If file context exists - deinit it */
            if (RRC_LOG_CTXT_CONSOLE < g_advl_ctxt_rrc)
            {
                deinit_log(g_advl_ctxt_rrc);
                g_advl_ctxt_rrc = RRC_LOG_CTXT_CONSOLE;
            }
        }
    }
    RRC_OAMH_UT_TRACE_EXIT();
}
#endif

/******************************************************************************
*   FUNCTION NAME: oamh_oam_prov_req_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION: This is the handler RRC_OAM_PROVISION_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_oam_prov_req_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    rrc_oam_provision_req_t     *p_rrc_oam_prov_req = PNULL;
    rrc_ueccmd_oamh_prov_req_t   ueccmd_oamh_prov_req;
    rrc_csc_oamh_prov_req_t     csc_oamh_prov_req;
    rrc_llim_oamh_prov_req_t    llim_oamh_prov_req;
    rrc_bool_et                 data_is_correct = RRC_TRUE;
    U8 loop_index   = 0;
/*SPR 21653 changes start*/
    U8 sector_index = 0;
/*Val Grind fix */
    U8 sector_index_1 = 0;
/*Val Grind fix */
/*SPR 21653 changes end*/
    U8 core_index = 0;
    rrc_bool_et                 nullIntegritySupport[MAX_NUM_CELLS];
    rrc_bool_et                 nullCipheringSupport[MAX_NUM_CELLS];
    /*SPR 21096 Start*/
    U8         csc_cell_index_status = RRC_ZERO;
    /*SPR 21096 End*/
    U16        total_ue_supported = RRC_NULL;

#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    S8                          *p_log_file_path;
    S8                          dir[]="Log_Files";
    U16                         log_index = RRC_NULL;
    rrc_bool_et      is_shared_mem_log = RRC_FALSE;
#endif
    U8               rest_rrc_core_no;
    U8               no_of_uecc_instances = RRC_NULL;
    U8               core_no[MAX_UECC_INSTANCES];

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    p_rrc_oam_prov_req = (rrc_oam_provision_req_t *)(p_msg);

    memset_wrapper (nullIntegritySupport, RRC_FALSE, sizeof(nullIntegritySupport));
    memset_wrapper (nullCipheringSupport, RRC_FALSE, sizeof(nullCipheringSupport));
    memset_wrapper(&ueccmd_oamh_prov_req, RRC_NULL, sizeof(ueccmd_oamh_prov_req));

    /*only one provision request can be proccessed.*/
    if( p_oamh_gb_context->prov_status & OAMH_OAM_PROV_REQ_RECD )
    {
        RRC_OAMH_TRACE(RRC_WARNING, "Message ignored."
            " RRC_OAM_PROV_REQ already received.");
        RRC_OAMH_UT_TRACE_EXIT();
        return;
    }
    /* set flag to indicate the reception of RRC_OAM_PROV_REQ msg */
    p_oamh_gb_context->prov_status |= OAMH_OAM_PROV_REQ_RECD;

    /* print detailed information about RRC_OAM_PROV_REQ */
    oamh_dump_oam_provision_req(p_rrc_oam_prov_req);

    if (p_rrc_oam_prov_req->bitmask & RRC_OAM_UE_CONTEXT_LIST_SIZE_PRESENT)
    {
        ueccmd_oamh_prov_req.uecc_oamh_prov_req.bitmask |= 
            RRC_UECC_UE_CONTEXT_LIST_SIZE_PRESENT;
        ueccmd_oamh_prov_req.uecc_oamh_prov_req.ue_context_list_size =
            p_rrc_oam_prov_req->ue_context_list_size;
        RRC_OAMH_TRACE(RRC_INFO, "ue_context_list_size:%d",
                p_rrc_oam_prov_req->ue_context_list_size);
    }
#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    oamh_oam_prov_req_hdlr_rrc_adv_logger(p_rrc_oam_prov_req, 
        &ueccmd_oamh_prov_req, &data_is_correct, &is_shared_mem_log);
#endif /* (LOGGER_TYPE == RRC_ADV_LOGGER) */

#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    oamh_change_logging_parameters(p_oamh_gb_context,
      oamh_get_rrc_log_level_from_oam_log_level(p_rrc_oam_prov_req->log_level),
      p_oamh_gb_context->log_enabled);
#endif
/* (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */

/*SPR 21653 changes start*/
    /* SPR 1452 Start */
    for (sector_index = 0; sector_index < p_rrc_oam_prov_req->sector_info_list.count; 
            sector_index++)
    {
        for (loop_index = 0; 
                loop_index < p_rrc_oam_prov_req->sector_info_list.sector_info[sector_index].
                supported_security_algorithms.integrity_algorithms.num_algorithms;
                loop_index++)
        {
            if ((p_rrc_oam_prov_req->sector_info_list.sector_info[sector_index].
                        supported_security_algorithms.integrity_algorithms.
                        algorithms[loop_index]) == RRC_PDCP_INT_ALG_EIA0)
            {
                nullIntegritySupport[p_rrc_oam_prov_req->sector_info_list.
                    sector_info[sector_index].cell_index] = RRC_TRUE;
                break;
            }
        }
        /* SPR 1452 Stop */
        /* SPR 1455 Start */
        for (loop_index = 0; 
                loop_index < p_rrc_oam_prov_req->sector_info_list.sector_info[sector_index].
                supported_security_algorithms.ciphering_algorithms.num_algorithms;
                loop_index++)
        {
            if ((p_rrc_oam_prov_req->sector_info_list.sector_info[sector_index].
                        supported_security_algorithms.ciphering_algorithms.
                        algorithms[loop_index]) == RRC_PDCP_CIPH_ALG_EEA0)
            {
                nullCipheringSupport[p_rrc_oam_prov_req->sector_info_list.
                    sector_info[sector_index].cell_index] = RRC_TRUE;
                break;
            }
        }

        if ( (RRC_TRUE == nullIntegritySupport[p_rrc_oam_prov_req->sector_info_list.
                    sector_info[sector_index].cell_index]) &&
                (RRC_FALSE == nullCipheringSupport[p_rrc_oam_prov_req->sector_info_list.
                 sector_info[sector_index].cell_index]) )
        {
            RRC_OAMH_TRACE(RRC_ERROR, "Invalid Security Algo Configuration received.");
            data_is_correct = RRC_FALSE;
            break;
        }
        /* Bitmap indicates the cells for which Provisioning Info received */  
        if (((csc_cell_index_status) & (1 << p_rrc_oam_prov_req->sector_info_list.
                    sector_info[sector_index].cell_index)))
        {
            RRC_OAMH_TRACE(RRC_ERROR, "Duplicate Provisioning Info received for Cell Index: %d",
                    p_rrc_oam_prov_req->sector_info_list.sector_info[sector_index].cell_index);
            data_is_correct = RRC_FALSE;
            break;
        }
        csc_cell_index_status |= (U8)(1 << (p_rrc_oam_prov_req->sector_info_list.
                    sector_info[sector_index].cell_index));
	/*SPR 5759 start*/
	if (p_rrc_oam_prov_req->sector_info_list.sector_info[sector_index].max_num_supported_ue <= 
		RRC_MAX_NUM_SUPPORTED_UE)
	{
            total_ue_supported = (U16)(total_ue_supported + p_rrc_oam_prov_req->sector_info_list.
                sector_info[sector_index].max_num_supported_ue);
	}
	else
	{
	    RRC_OAMH_TRACE(RRC_ERROR, "Invalid number of UE's supported :%d received for Cell Index: %d",
		p_rrc_oam_prov_req->sector_info_list.sector_info[sector_index].max_num_supported_ue,
		p_rrc_oam_prov_req->sector_info_list.sector_info[sector_index].cell_index);
	    data_is_correct = RRC_FALSE;
	    break;	
	}
	/*SPR 5759 end*/
        /* SPR 1455 Stop */

	
        /* SPR 1455 Stop */
    }
/*SPR 21653 changes end*/


/*SPR 21653 changes start*/
    no_of_uecc_instances = p_rrc_oam_prov_req->no_of_uecc_instances;
    rest_rrc_core_no     = p_rrc_oam_prov_req->rest_rrc_core_no;
/*SPR 21653 changes end*/

    for (core_index = 0; core_index < no_of_uecc_instances; core_index++)
    {
/*SPR 21653 changes start*/
        core_no[core_index] = p_rrc_oam_prov_req->core_no[core_index];
/*SPR 21653 changes end*/
    }
    RRC_OAMH_TRACE(RRC_INFO, "Creating UECC Instances.");
    if((RRC_FALSE == data_is_correct) || 
       (RRC_FAILURE == rrc_create_uecc_instances(no_of_uecc_instances,
                                                rest_rrc_core_no,
                                                core_no)))
    {
        data_is_correct = RRC_FALSE;
    }

#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    /* SPR-7639_fix */
    if (RRC_TRUE == is_shared_mem_log)
    {
        if(p_rrc_oam_prov_req->shared_mem_logging_config.bitmask &
                                            RRC_OAM_SHARED_MEM_LOG_FILE_PATH_PRESENT)
        {
            p_log_file_path = p_rrc_oam_prov_req->shared_mem_logging_config.LogFilePath;
        }
        else
        {
            if( 0 == RRC_MKDIR((char*)dir,  S_IRWXU | S_IRWXO | S_IRWXG))
            {
                p_log_file_path = dir;
            }
            else
            {
                RRC_OAMH_TRACE(RRC_ERROR, "Logging Directory creation failed !!");
                is_shared_mem_log = RRC_FALSE;
            }
        }
    }
    /* SPR-7639_fix */

    if((RRC_TRUE == is_shared_mem_log) && (RRC_TRUE == data_is_correct))
    {
        /* Thread for Reader application */
        RRC_OAMH_TRACE(RRC_INFO, "Creating Logger Thread.");
        rrc_create_logger_thread(
                &p_rrc_oam_prov_req->shared_mem_logging_config,p_rrc_oam_prov_req->module_timers_info.rrc_health_mon_time_int/2);
        p_oamh_gb_context->adv_logger_hm_chk = RRC_TRUE;
/*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL
        rrc_lock = qvNewLock(); 
#endif
/*Lockless Implementation stop*/

        if (LogSuccess == init_log(
                    LogTarget_Shared_Mem, "LTE_SHARED_MEMORY", &log_index)) 
        {
            lgr_shm_logging_config_t thd_cfg[num_active_threads];
            for(loop_index = 0; loop_index < num_active_threads; loop_index++)
            {
                set_names(thd_cfg,loop_index,&THREAD_NAMES[loop_index],&SHARED_MEMORY_NAMES[loop_index]);
                thd_cfg[loop_index].p_call_back = (call_back_pointer)health_monitoring;
                thd_cfg[loop_index].p_cbk_arg = RRC_NULL; 
                thd_cfg[loop_index].shm_size = p_rrc_oam_prov_req->shared_mem_logging_config.SharedMemSize; 
                thd_cfg[loop_index].log_file_sz = p_rrc_oam_prov_req->shared_mem_logging_config.MaxLogFileSize;
                thd_cfg[loop_index].log_chunk_cnt = MAX_CHUNK_COUNT;
                if(p_rrc_oam_prov_req->shared_mem_logging_config.bitmask & RRC_OAM_SHARED_MEM_LOG_FILE_COUNT_PRESENT)
                {
                    thd_cfg[loop_index].file_cnt = p_rrc_oam_prov_req->shared_mem_logging_config.MaxLogFileCount;
                }
                else
                {
                    thd_cfg[loop_index].file_cnt = MAX_LOG_FILE_NUMBER;
                }
            }
            if(LGR_SUCCESS == lgr_in_mem_logging_init(num_active_threads,
                        thd_cfg,
                        (char*)p_log_file_path))
            {
                RRC_OAMH_TRACE(RRC_INFO, "Created SharedMemory");
                g_advl_ctxt_rrc = log_index;
                ueccmd_oamh_prov_req.uecc_oamh_prov_req.bitmask |= 
                    RRC_UECC_SHARED_MEM_INDEX_PRESENT;
                ueccmd_oamh_prov_req.uecc_oamh_prov_req.shared_mem_index =
                    log_index;
                oamh_change_logging_parameters(p_oamh_gb_context,
                    oamh_get_rrc_log_level_from_oam_log_level(p_rrc_oam_prov_req->log_level),
                    p_oamh_gb_context->log_enabled);
            }
            else
            {
                RRC_OAMH_TRACE(RRC_WARNING,
                    "Couldn't init Shared Memory logging!");
                data_is_correct = RRC_FALSE;
            }
        }
        else
        {
            RRC_OAMH_TRACE(RRC_WARNING,
                    "Couldn't init Shared Memory logging!");
            data_is_correct = RRC_FALSE;
        }
    }
#endif
    /* SPR 20908 Start */
    p_oamh_gb_context->csc_cell_index_status = csc_cell_index_status;
    /* code removed */
    /* SPR 20908 End */
    if (RRC_TRUE == data_is_correct)
    {
        /*Initialize Cell context db */
        if ( (PNULL == (p_oamh_gb_context->p_p_csc_cell_ctx = (struct _rrc_oamh_cell_ctx_t * *)
                           rrc_mem_get((MAX_NUM_CELLS) * sizeof(struct _rrc_oamh_cell_ctx_t *))) ))
        {
            /* Not enough memory */
            RRC_OAMH_TRACE(RRC_ERROR, "Not Enough Memory.");
            RRC_OAMH_UT_TRACE_EXIT();
            return;
        }
        memset_wrapper(p_oamh_gb_context->p_p_csc_cell_ctx, RRC_NULL,
                MAX_NUM_CELLS*sizeof(struct _rrc_oamh_cell_ctx_t *));
/*SPR 21653 changes start*/ 
        for (sector_index = 0; sector_index < p_rrc_oam_prov_req->sector_info_list.count; 
                sector_index++)
        {
            if (PNULL == (p_oamh_gb_context->p_p_csc_cell_ctx[p_rrc_oam_prov_req->sector_info_list.
                        sector_info[sector_index].cell_index] = (struct _rrc_oamh_cell_ctx_t  *)
                        rrc_mem_get(sizeof(struct _rrc_oamh_cell_ctx_t ))))
            {
                /* Not enough memory */
                RRC_OAMH_TRACE(RRC_ERROR, "Not Enough Memory.");
		    /*Val Grind fix */
		    for ( sector_index_1 = 0; sector_index_1 < sector_index; sector_index_1++ )
		    {
                            if ( p_oamh_gb_context->p_p_csc_cell_ctx[p_rrc_oam_prov_req->sector_info_list.
                                sector_info[sector_index_1].cell_index] )
			    {
				    RRC_OAMH_TRACE(RRC_INFO, "p_oamh_gb_context->p_p_csc_cell_ctx[p_rrc_oam_prov_req->sector_info_list."
						    "sector_info[sector_index_1].cell_index]=[%p] freed",
						    p_oamh_gb_context->p_p_csc_cell_ctx[p_rrc_oam_prov_req->sector_info_list.
						    sector_info[sector_index_1].cell_index]);
				    rrc_mem_free(p_oamh_gb_context->p_p_csc_cell_ctx[p_rrc_oam_prov_req->sector_info_list.
						    sector_info[sector_index_1].cell_index]);
				    p_oamh_gb_context->p_p_csc_cell_ctx[p_rrc_oam_prov_req->sector_info_list.
					    sector_info[sector_index_1].cell_index]= PNULL;
			    }
		    }
                    rrc_mem_free(p_oamh_gb_context->p_p_csc_cell_ctx);
                    p_oamh_gb_context->p_p_csc_cell_ctx = PNULL;
		    /*Val Grind fix */
                RRC_OAMH_UT_TRACE_EXIT();
                return;
            }
            memset_wrapper (p_oamh_gb_context->p_p_csc_cell_ctx[p_rrc_oam_prov_req->sector_info_list.
                    sector_info[sector_index].cell_index], RRC_NULL, sizeof(struct _rrc_oamh_cell_ctx_t ));
        }

/*SPR 21653 changes end*/ 

        /* Stop oam_init_ind timer if exist */
        if(PNULL != p_oamh_gb_context->oam_init_ind_timer)
        {
            rrc_stop_timer(p_oamh_gb_context->oam_init_ind_timer);
            p_oamh_gb_context->oam_init_ind_timer = PNULL;
        }

        /* Build and send provision message to UECC MD */
/*SPR 21653 changes start*/ 
        ueccmd_oamh_prov_req.no_of_uecc_instances = 
                           p_rrc_oam_prov_req->no_of_uecc_instances;
        p_oamh_gb_context->no_of_uecc_instances = 
                           p_rrc_oam_prov_req->no_of_uecc_instances;

        ueccmd_oamh_prov_req.rest_rrc_core_no = p_rrc_oam_prov_req->rest_rrc_core_no;
/*SPR 21653 changes end*/ 

        ueccmd_oamh_prov_req.uecc_oamh_prov_req.total_ue_supported = 
                           total_ue_supported;

        for (core_index = 0; core_index < p_oamh_gb_context->no_of_uecc_instances;
               core_index++)
        {
/*SPR 21653 changes start*/ 
            ueccmd_oamh_prov_req.core_no[core_index] =
                  p_rrc_oam_prov_req->core_no[core_index];
/*SPR 21653 changes end*/ 
        }
        
/*SPR 21653 changes start*/ 
        ueccmd_oamh_prov_req.uecc_oamh_prov_req.supported_security_algo_per_cell.
            num_cells = p_rrc_oam_prov_req->sector_info_list.count;

        for (sector_index=0; sector_index<p_rrc_oam_prov_req->
                sector_info_list.count; sector_index++)
        {
            ueccmd_oamh_prov_req.uecc_oamh_prov_req.supported_security_algo_per_cell.
                    supported_security_algorithms[sector_index].cell_index =
                    p_rrc_oam_prov_req->sector_info_list.sector_info[sector_index].cell_index;

            ueccmd_oamh_prov_req.uecc_oamh_prov_req.supported_security_algo_per_cell.
                supported_security_algorithms[sector_index].integrity_algorithms =
                p_rrc_oam_prov_req->sector_info_list.sector_info[sector_index].
                supported_security_algorithms.integrity_algorithms;

            ueccmd_oamh_prov_req.uecc_oamh_prov_req.supported_security_algo_per_cell.
                supported_security_algorithms[sector_index].ciphering_algorithms =
                p_rrc_oam_prov_req->sector_info_list.sector_info[sector_index].
                supported_security_algorithms.ciphering_algorithms;

        }
/*SPR 21653 changes end*/ 
        ueccmd_oamh_prov_req.uecc_oamh_prov_req.timer_info =
                    p_rrc_oam_prov_req->uecc_timer_info;

        ueccmd_oamh_prov_req.uecc_oamh_prov_req.len_vendor_name = 
                    p_rrc_oam_prov_req->len_vendor_name;


        l3_memcpy_wrapper(ueccmd_oamh_prov_req.uecc_oamh_prov_req.vendor_name, 
                   p_rrc_oam_prov_req->vendor_name,
                   sizeof(p_rrc_oam_prov_req->vendor_name));

        /* SPR 4118 Start */

        if (RRC_OAM_INTEGRITY_THRESHOLD_COUNTER_PRESENT &
                p_rrc_oam_prov_req->bitmask)
        {
            ueccmd_oamh_prov_req.uecc_oamh_prov_req.bitmask |=
                RRC_UECC_INTEGRITY_THRESHOLD_COUNTER_PRESENT;
            ueccmd_oamh_prov_req.uecc_oamh_prov_req.integrity_threshold_counter = 
                p_rrc_oam_prov_req->pdcp_integrity_threshold_counter;
        }

        /* SPR 4118 Stop */
        oamh_oam_module_timers_handler(p_oamh_gb_context, 
                     p_rrc_oam_prov_req, &ueccmd_oamh_prov_req);

        rrc_oamh_send_internal_msg(RRC_UECCMD_MODULE_ID,
                                   UECCMD_OAMH_PROV_REQ,
                                   sizeof(ueccmd_oamh_prov_req),
                                   &ueccmd_oamh_prov_req);

        /* Build and send provision message to CSC */
        memset_wrapper(&csc_oamh_prov_req, 0, sizeof(csc_oamh_prov_req));

        csc_oamh_prov_req.oam_csc_timer_info =
            p_rrc_oam_prov_req->csc_timer_info;
        csc_oamh_prov_req.sfn_offset = 
            p_rrc_oam_prov_req->non_broadcast_sfn_offset;
        csc_oamh_prov_req.max_cell_reconfig_retry_count = 
            p_rrc_oam_prov_req->max_cell_reconfig_retry_count;

        csc_oamh_prov_req.csc_cell_index_status = p_oamh_gb_context->csc_cell_index_status;

/*SPR 21653 changes start*/ 
        csc_oamh_prov_req.max_num_supported_cells = 
            p_rrc_oam_prov_req->sector_info_list.count;

        csc_oamh_prov_req.no_of_uecc_instances = p_rrc_oam_prov_req->no_of_uecc_instances;

        for (sector_index=0; sector_index<p_rrc_oam_prov_req->
                sector_info_list.count; sector_index++)
        {
             csc_oamh_prov_req.max_num_supported_ue_per_cell[p_rrc_oam_prov_req->
                sector_info_list.sector_info[sector_index].cell_index] = p_rrc_oam_prov_req->
                sector_info_list.sector_info[sector_index].max_num_supported_ue;
        }
        
        for ( loop_index = 0; loop_index < MAX_NUM_CELLS; loop_index++)
        {
            csc_oamh_prov_req.null_integrity_support[loop_index] = nullIntegritySupport[loop_index];
        }
/*SPR 21653 changes end*/ 
        if (RRC_OAM_UECC_HEALTH_MONITORING_TIMER_PRESENT &
                p_rrc_oam_prov_req->module_timers_info.bitmask)
        {
            csc_oamh_prov_req.bitmask |= RRC_CSC_HEALTH_MONITORING_TIMER_PRESENT;
            csc_oamh_prov_req.csc_health_monitor_timer_value = 0;
        }

        rrc_oamh_send_internal_msg(RRC_CSC_MODULE_ID,
                                   CSC_OAMH_PROV_REQ,
                                   sizeof(csc_oamh_prov_req),
                                   &csc_oamh_prov_req);

        /* Build and send provision message to LLIM */
        memset_wrapper(&llim_oamh_prov_req, 0, sizeof(llim_oamh_prov_req));
        
/*SPR 21653 changes start*/ 
        llim_oamh_prov_req.no_of_uecc_instances = 
                           p_rrc_oam_prov_req->no_of_uecc_instances;
        for (sector_index=0; sector_index<p_rrc_oam_prov_req->
                sector_info_list.count; sector_index++)
        {
             llim_oamh_prov_req.max_num_supported_ue_per_cell[p_rrc_oam_prov_req->
                sector_info_list.sector_info[sector_index].cell_index] = p_rrc_oam_prov_req->
                sector_info_list.sector_info[sector_index].max_num_supported_ue;
        }
        /*SPR 21653 changes end*/ 

        llim_oamh_prov_req.timer_info =
                    p_rrc_oam_prov_req->llim_timer_info;
        if (RRC_OAM_UECC_HEALTH_MONITORING_TIMER_PRESENT &
                p_rrc_oam_prov_req->module_timers_info.bitmask)
        {
            llim_oamh_prov_req.bitmask |= RRC_LLIM_HEALTH_MONITORING_TIMER_PRESENT;
            llim_oamh_prov_req.llim_health_monitor_timer_value = 0;
        }

        rrc_oamh_send_internal_msg(RRC_LLIM_MODULE_ID,
                                   LLIM_OAMH_PROV_REQ,
                                   sizeof(llim_oamh_prov_req),
                                   &llim_oamh_prov_req);
    }
    else
    {
        rrc_oam_provision_resp_t rrc_oam_prov_resp;

        memset_wrapper(&rrc_oam_prov_resp, 0, sizeof(rrc_oam_prov_resp));
        rrc_oam_prov_resp.response = RRC_FAILURE;

        /* Encode and send the provisioning response to OAM */
        rrc_oam_il_send_rrc_oam_provision_resp(
            &rrc_oam_prov_resp,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            p_oamh_gb_context->prov_req_trans_id,
            RRC_NULL);
    }

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_invalid_event_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION: This is the handler function for any invalid event
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_invalid_event_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    oamh_state_et state = (oamh_state_et)(OAMH_GET_FSM_STATE(p_oamh_gb_context));

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_OAMH_TRACE(RRC_DETAILED, 
            "Unused variable in fsm p_msg=%p", p_msg);      

    RRC_OAMH_TRACE(RRC_WARNING,"Invalid Event, state=%i",state);

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_csc_init_ind_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION: This is the handler for CSC_OAMH_INIT_IND msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_csc_init_ind_handler
(
    rrc_oamh_gb_context_t   *p_oamh_gb_context,
    void                    *p_msg
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_DETAILED, 
            "Unused variable in fsm p_msg=%p", p_msg);       

    rrc_oamh_send_internal_msg(RRC_CSC_MODULE_ID,
                               CSC_OAMH_INIT_CNF,
                               0,
                               PNULL);

    p_oamh_gb_context->init_ind |= OAMH_CSC_INIT_RECD;
    check_for_oamh_comm_prov_state(p_oamh_gb_context);
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_llim_init_ind_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION: This is the handler for LLIM_OAMH_INIT_IND msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_llim_init_ind_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_DETAILED, 
            "Unused variable in fsm p_msg=%p", p_msg);       
    rrc_oamh_send_internal_msg(RRC_LLIM_MODULE_ID,
                               LLIM_OAMH_INIT_CNF,
                               0,
                               PNULL);
    p_oamh_gb_context->init_ind |= OAMH_LLIM_INIT_RECD;
    check_for_oamh_comm_prov_state(p_oamh_gb_context);

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_ueccmd_init_ind_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION: This is the handler for UECCMD_OAMH_INIT_IND msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_ueccmd_init_ind_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_DETAILED, 
            "Unused variable in fsm p_msg=%p", p_msg);       
    rrc_oamh_send_internal_msg(RRC_UECCMD_MODULE_ID,
                               UECCMD_OAMH_INIT_CNF,
                               0,
                               PNULL);
    p_oamh_gb_context->init_ind |= OAMH_UECCMD_INIT_RECD;
    check_for_oamh_comm_prov_state(p_oamh_gb_context);

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: check_for_rrc_oam_prov_resp
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*           This function checks preconditions for sending
*           RRC_OAM_PROVISION_RESP messsage and sends it if all is OK.
*
*   RETURNS:
*       RRC_TRUE - if RRC_OAM_PROVISION_RESP has been sent
*       RRC_FALSE - in other case
******************************************************************************/
static rrc_bool_et check_for_rrc_oam_prov_resp
(
    rrc_oamh_gb_context_t*  p_oamh_gb_context
)
{
    rrc_bool_et result = RRC_FALSE;

    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    /* Check if prov_response messages received from all RRC modules */
    if ((p_oamh_gb_context->prov_status & OAMH_CSC_PROV_RECD)
         && (p_oamh_gb_context->prov_status & OAMH_LLIM_PROV_RECD)
         && (p_oamh_gb_context->prov_status & OAMH_UECCMD_PROV_RECD))

    {
        rrc_oam_provision_resp_t rrc_oam_prov_resp;

        memset_wrapper(&rrc_oam_prov_resp, 0, sizeof(rrc_oam_prov_resp));
        p_oamh_gb_context->comm_prov_info_flag |= OAMH_PROV_RECD;

        if ((p_oamh_gb_context->prov_status & OAMH_CSC_PROV_SUCCESS)
            && (p_oamh_gb_context->prov_status & OAMH_LLIM_PROV_SUCCESS)
            && (p_oamh_gb_context->prov_status & OAMH_UECCMD_PROV_SUCCESS))
        {
            rrc_oam_prov_resp.response = RRC_SUCCESS;
        }
        else
        {
            rrc_oam_prov_resp.response = RRC_FAILURE;
        }

        /* Encode and send the provisioning response to OAM */
        rrc_oam_il_send_rrc_oam_provision_resp(
            &rrc_oam_prov_resp,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            p_oamh_gb_context->prov_req_trans_id,
            RRC_NULL);

        if(RRC_FAILURE == rrc_oam_prov_resp.response)
        {
            ALARM_MSG_L3(RRC_MODULE_ID, RRC_PROVISIONING_FAILURE_ALARM_ID,
                    MAJOR_ALARM,INVALID_ERR_CAUSE_ID);
        }
        

        result = RRC_TRUE;
    }
    RRC_OAMH_UT_TRACE_EXIT();
    return result;
}

/******************************************************************************
*   FUNCTION NAME: check_for_rrc_oam_cleanup_cnf
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*   OUTPUTS      : None
*   DESCRIPTION:
*           This function checks preconditions for sending
*           RRC_OAM_CLEANUP_RESP messsage and sends it if all is OK.
*
*   RETURNS:
*           None
******************************************************************************/
static void check_for_rrc_oam_cleanup_cnf
(
    rrc_oamh_gb_context_t*  p_oamh_gb_context
)
{
    U8 index = RRC_NULL;
    U8 uecc_index = RRC_NULL;
    RRC_OAMH_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    /* Check if cleanup response messages received from all RRC modules */
    if ((p_oamh_gb_context->cleanup_status & OAMH_CSC_CLEANUP_RECD)
         && (p_oamh_gb_context->cleanup_status & OAMH_LLIM_CLEANUP_RECD)
         && (p_oamh_gb_context->cleanup_status & OAMH_UECCMD_CLEANUP_RECD))
    {
        rrc_oam_cleanup_resp_t rrc_oam_cleanup_resp;
        rrc_oam_cleanup_resp.response = RRC_SUCCESS;

        /* Encode and send the cleanup response to OAM */
        rrc_oam_il_send_rrc_oam_cleanup_resp(
            &rrc_oam_cleanup_resp,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            p_oamh_gb_context->cleanup_req_trans_id,
            RRC_NULL);
        
        if (PNULL != g_rrc_stats.p_rrc_stats)
        {
            /* Resetting the shared memory */
            for (uecc_index = 0; uecc_index < g_rrc_stats.p_rrc_stats->
                    rrc_uecc_stats.count && uecc_index < MAX_UECC_INSTANCES;
                    uecc_index++)
            {
                for (index = 0; index < MAX_NUM_CELLS; index++)
                {
                    memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[index].rrc_connection_related_stats, 0,
                            sizeof(rrc_oam_connection_related_stats_t));

                    memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[index].rrc_erab_related_stats, 0,
                            sizeof(rrc_oam_erab_related_stats_t));

                    memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[index].rrc_handover_related_stats, 0,
                            sizeof(rrc_oam_handover_related_stats_t));

                    memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                            rrc_uecc_instance_stats[uecc_index].
                            rrc_uecc_cell_stats[index].rrc_ue_assoc_s1_conn_related_stats, 0,
                            sizeof(rrc_uecc_oam_stat_ue_assoc_s1_connection_t));

                    g_rrc_stats.p_rrc_stats->rrc_uecc_stats.
                        rrc_uecc_instance_stats[uecc_index].
                        rrc_uecc_cell_stats[index].rrc_IratIncMobility_LAI =  0;

                    memset_wrapper(&g_rrc_stats.p_rrc_stats->rrc_csc_stats.
                            rrc_csc_cell_stats[index].rrc_paging_related_stats,0, 
                            sizeof(rrc_csc_oam_stat_paging_t));
                }
            }
        }

        /* Reset context */
        oamh_reset_context(p_oamh_gb_context);
        /* Change state */
        OAMH_SET_FSM_STATE(p_oamh_gb_context,OAMH_STATE_W_FOR_COMM_PROV_REQ);
    }
    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_csc_prov_resp_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION:
*           This is the handler for CSC_OAMH_PROV_RESP msg
*
*   RETURNS:
*           None
******************************************************************************/
static void oamh_csc_prov_resp_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    rrc_csc_oamh_prov_resp_t  *p_csc_oamh_prov_resp = PNULL;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    p_csc_oamh_prov_resp = (rrc_csc_oamh_prov_resp_t *)
                                ((U8*)p_msg + RRC_API_HEADER_SIZE);
    p_oamh_gb_context->prov_status |= OAMH_CSC_PROV_RECD;
    if(RRC_SUCCESS == p_csc_oamh_prov_resp->result)
    {
        p_oamh_gb_context->prov_status |= OAMH_CSC_PROV_SUCCESS;
    }

    if (RRC_TRUE == check_for_rrc_oam_prov_resp(p_oamh_gb_context))
    {
        check_for_oamh_active_state(p_oamh_gb_context);
    }

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_llim_prov_resp_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION:
*           This is the handler for LLIM_OAMH_PROV_RESP msg
*
*   RETURNS:
*           None
******************************************************************************/
static void oamh_llim_prov_resp_handler
(
    rrc_oamh_gb_context_t *p_oamh_gb_context,
    void                  *p_msg
)
{
    rrc_llim_oamh_prov_resp_t *p_llim_oamh_prov_resp = PNULL;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    p_llim_oamh_prov_resp = (rrc_llim_oamh_prov_resp_t *)
                                ((U8*)p_msg + RRC_API_HEADER_SIZE);
    p_oamh_gb_context->prov_status |= OAMH_LLIM_PROV_RECD;
    if(RRC_SUCCESS == p_llim_oamh_prov_resp->result)
    {
        p_oamh_gb_context->prov_status |= OAMH_LLIM_PROV_SUCCESS;
    }

    if (RRC_TRUE == check_for_rrc_oam_prov_resp(p_oamh_gb_context))
    {
        check_for_oamh_active_state(p_oamh_gb_context);
    }

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_ueccmd_prov_resp_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION:
*           This is the handler for UECCMD_OAMH_PROV_RESP msg
*
*   RETURNS:
*           None
******************************************************************************/
static void oamh_ueccmd_prov_resp_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    rrc_ueccmd_oamh_prov_resp_t  *p_ueccmd_oamh_prov_resp = PNULL;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);

    p_ueccmd_oamh_prov_resp = (rrc_ueccmd_oamh_prov_resp_t *)
                                ((U8*)p_msg + RRC_API_HEADER_SIZE);
    p_oamh_gb_context->prov_status |= OAMH_UECCMD_PROV_RECD;
    if(RRC_SUCCESS == p_ueccmd_oamh_prov_resp->result)
    {
        p_oamh_gb_context->prov_status |= OAMH_UECCMD_PROV_SUCCESS;
    }

    if (RRC_TRUE == check_for_rrc_oam_prov_resp(p_oamh_gb_context))
    {
        check_for_oamh_active_state(p_oamh_gb_context);
    }

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_cleanup_req_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION:
*           This is the handler for RRC_OAM_CLEANUP_REQ msg
*
*   RETURNS:
*           None
******************************************************************************/
static void oamh_oam_cleanup_req_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);
    /*coverity fix start*/
    /*coverity fix stop*/

    RRC_OAMH_TRACE(RRC_DETAILED, 
        "Unused variable in fsm p_msg=%p", p_msg);       
    /* Stop oam_init_ind timer if exist */
    if(PNULL != p_oamh_gb_context->oam_init_ind_timer)
    {
        rrc_stop_timer(p_oamh_gb_context->oam_init_ind_timer);
        p_oamh_gb_context->oam_init_ind_timer = PNULL;
    }
   /*valgrind_fix start*/ 
    if(PNULL !=  p_oamh_gb_context->timers_data.
               timers_ids[OAMH_HEALTH_MONITOR_TIMER])
    {
        rrc_stop_timer(p_oamh_gb_context->timers_data.
                  timers_ids[OAMH_HEALTH_MONITOR_TIMER]);
        p_oamh_gb_context->timers_data.timers_ids[OAMH_HEALTH_MONITOR_TIMER] = PNULL;
    }
   /*valgrind_fix stop*/

    rrc_oamh_send_internal_msg(RRC_UECCMD_MODULE_ID,
                               UECCMD_OAMH_CLEANUP_REQ,
                               0,
                               PNULL);

    rrc_oamh_send_internal_msg(RRC_CSC_MODULE_ID,
                               CSC_OAMH_CLEANUP_REQ,
                               0,
                               PNULL);

    rrc_oamh_send_internal_msg(RRC_LLIM_MODULE_ID,
                               LLIM_OAMH_CLEANUP_REQ,
                               0,
                               PNULL);
    /*Change state*/
    OAMH_SET_FSM_STATE(p_oamh_gb_context,OAMH_STATE_W_FOR_CLEANUP_RESP);

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_second_cleanup_req_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION:
*       This is handler for repeated RRC_OAM_CLEANUP_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
static void oamh_second_cleanup_req_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    RRC_OAMH_UT_TRACE_ENTER();
    /*coverity fix start*/
    /*coverity fix stop*/

    RRC_OAMH_TRACE(RRC_DETAILED, 
        "Unused variable in fsm p_msg=%p and gb_context=%p", p_msg, p_oamh_gb_context);          

    RRC_OAMH_TRACE(RRC_WARNING,"Handling of RRC_OAM_CLEANUP_REQ is already in "
        "progress.");

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_csc_cleanup_resp_handler
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*
*   DESCRIPTION:
*           This is the handler for CSC_OAMH_CLEANUP_RESP msg
*
*   RETURNS:
*           None
******************************************************************************/
static void oamh_csc_cleanup_resp_handler
(
    rrc_oamh_gb_context_t *p_oamh_gb_context,
    void                  *p_msg
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_DETAILED, 
            "Unused variable in fsm p_msg=%p", p_msg);       

    p_oamh_gb_context->cleanup_status |= OAMH_CSC_CLEANUP_RECD;

    check_for_rrc_oam_cleanup_cnf(p_oamh_gb_context);

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_llim_cleanup_resp_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION:
*           This is the handler for LLIM_OAMH_CLEANUP_RESP msg
*
*   RETURNS:
*           None
******************************************************************************/
static void oamh_llim_cleanup_resp_handler
(
    rrc_oamh_gb_context_t *p_oamh_gb_context,
    void                  *p_msg
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_DETAILED, 
            "Unused variable in fsm p_msg=%p", p_msg);       
    p_oamh_gb_context->cleanup_status |= OAMH_LLIM_CLEANUP_RECD;

    check_for_rrc_oam_cleanup_cnf(p_oamh_gb_context);

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_ueccmd_cleanup_resp_handler
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                   *p_msg
*   OUTPUTS      : None
*   DESCRIPTION:
*           This is the handler for UECCMD_OAMH_CLEANUP_RESP msg
*
*   RETURNS:
*           None
******************************************************************************/
static void oamh_ueccmd_cleanup_resp_handler
(
    rrc_oamh_gb_context_t *p_oamh_gb_context,
    void                  *p_msg
)
{
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_oamh_gb_context);

    RRC_OAMH_TRACE(RRC_DETAILED, 
            "Unused variable in fsm p_msg=%p", p_msg);       

    p_oamh_gb_context->cleanup_status |= OAMH_UECCMD_CLEANUP_RECD;

    check_for_rrc_oam_cleanup_cnf(p_oamh_gb_context);

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: update_communication_info
*
*   INPUTS        :rrc_oam_ext_comm_info_t           *ext_comm_info
*                  rrc_oam_ext_connection_status_t   *ext_connection_status
*                  S8                  p_comm_info_int[MAX_LEN_COMM_INFO]
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function update internal communication info for Upper and Lower 
*       Layer from RRC_OAM_COMMUNICATION_INFO_REQ
*
*   RETURNS:
*       RRC_TRUE  - if all connections are OK
*       RRC_FALSE - if we have an invalid connection
******************************************************************************/
static rrc_bool_et  update_communication_info
(
    rrc_oam_ext_comm_info_t           *ext_comm_info, /* Source */
    rrc_oam_ext_connection_status_t   *ext_connection_status, /* Destination */
    /* internal communication info used in driver */
    S8                  p_comm_info_int[MAX_LEN_COMM_INFO] /* Destination */
)
{
    rrc_bool_et             result  = RRC_TRUE;

    /* SPR 1664 Start */
#ifndef RRC_UNIT_TEST_FRAMEWORK
    U8 min_ip_value[IPV4_ADDRESS_LENGTH] = {RRC_NULL};
    U8 max_ip_value[IPV4_ADDRESS_LENGTH] = {255};
    /* SPR 1664 Stop */
#endif
    RRC_OAMH_UT_TRACE_ENTER();

    ext_connection_status->ext_module_id =ext_comm_info->ext_module_id;

    /*spr 1893 stop*/
    if (OAM_TRANSPORT_MODE_UDP == ext_comm_info->transport_mode)
    {
        /* Valid connection data */
        ext_connection_status->connection_status = RRC_OAM_CONNECTED;

#ifndef RRC_UNIT_TEST_FRAMEWORK
        if (ext_comm_info->bitmask & 
                RRC_OAM_IP_ADDR_PRESENT)
        {
            /* SPR 1664 Start */
            if ((RRC_NULL ==
                        memcmp_wrapper(min_ip_value,
                            ext_comm_info->ip_addr,
                            IPV4_ADDRESS_LENGTH)) ||
                    (RRC_NULL ==
                     memcmp_wrapper(max_ip_value,
                         ext_comm_info->ip_addr,
                         IPV4_ADDRESS_LENGTH)))

            {
                RRC_OAMH_TRACE(RRC_ERROR,"Invalid IP in COMMUNICATION INFO");
                /* Invalid connection data */
                ext_connection_status->connection_status = 
                    RRC_OAM_NOT_CONNECTED;
                result = RRC_FALSE;
            }
            else
            {
                ext_connection_status->connection_status = RRC_OAM_CONNECTED;
            }
            /* SPR 1664 Stop */
            snprintf_wrapper((Char8 *)&(p_comm_info_int)[0],
                    MAX_LEN_COMM_INFO,
                    (const Char8 *)"%u.%u.%u.%u:%u",
                    ext_comm_info->ip_addr[0],
                    ext_comm_info->ip_addr[1],
                    ext_comm_info->ip_addr[2],
                    ext_comm_info->ip_addr[3],
                    ext_comm_info->port);
        }
        else
        {
            snprintf_wrapper((Char8 *)&(p_comm_info_int)[0],
                    MAX_LEN_COMM_INFO,
                    (const Char8*)"%u",
                    ext_comm_info->port);

            /* Valid connection data */
            ext_connection_status->connection_status = RRC_OAM_CONNECTED;
        }
#endif
    }
    else
    {
        /* Invalid connection data */
        ext_connection_status->connection_status = RRC_OAM_NOT_CONNECTED;
        result = RRC_FALSE;
    }

    RRC_OAMH_TRACE(RRC_INFO, "\t [MODULE:%s] [INFO:%s] [MODE:%s]",
            OAMH_COMM_MODULES_NAMES[ext_comm_info->ext_module_id],
            &(p_comm_info_int)[0],
            (OAM_TRANSPORT_MODE_TCP == ext_comm_info->transport_mode)?
            "OAM_TRANSPORT_MODE_TCP":"OAM_TRANSPORT_MODE_UDP");

    return result;

    RRC_OAMH_UT_TRACE_EXIT();

}
#ifndef RRC_UNIT_TEST_FRAMEWORK
/* SPR 20908 Start */
/******************************************************************************
*   FUNCTION NAME: update_communication_info_l2
*
*   INPUTS        :rrc_oam_ext_comm_info_t           *ext_comm_info
*                  S8                  p_comm_info_int[MAX_LEN_COMM_INFO]
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function update internal communication info for Lower 
*       Layer from RRC_OAMH_COMMUNICATION_INFO_IND from CSC
*
*   RETURNS:
*       RRC_TRUE  - if all connections are OK
*       RRC_FALSE - if we have an invalid connection
******************************************************************************/
static rrc_bool_et  update_communication_info_l2
(
    rrc_oam_ext_comm_info_t           *ext_comm_info, /* Source */
    /* internal communication info used in driver */
    S8                  p_comm_info_int[MAX_LEN_COMM_INFO] /* Destination */
)
{
	rrc_bool_et             result  = RRC_TRUE;

	/* SPR 1664 Start */
	U8 min_ip_value[IPV4_ADDRESS_LENGTH] = {RRC_NULL};
	U8 max_ip_value[IPV4_ADDRESS_LENGTH] = {255};
	/* SPR 1664 Stop */

	RRC_OAMH_UT_TRACE_ENTER();

	/*spr 1893 stop*/
	if (OAM_TRANSPORT_MODE_UDP == ext_comm_info->transport_mode)
	{
		/* SPR 1664 Start */
		if ((RRC_NULL == memcmp_wrapper(min_ip_value,ext_comm_info->ip_addr,
						IPV4_ADDRESS_LENGTH)) || (RRC_NULL ==  memcmp_wrapper(max_ip_value,
							ext_comm_info->ip_addr, IPV4_ADDRESS_LENGTH)))
		{
			RRC_OAMH_TRACE(RRC_ERROR,"Invalid IP in COMMUNICATION INFO");
			result = RRC_FALSE;
		}
		/* SPR 1664 Stop */
		snprintf_wrapper((Char8 *)&(p_comm_info_int)[0],
				MAX_LEN_COMM_INFO,
				(const Char8 *)"%u.%u.%u.%u:%u",
				ext_comm_info->ip_addr[0],
				ext_comm_info->ip_addr[1],
				ext_comm_info->ip_addr[2],
				ext_comm_info->ip_addr[3],
				ext_comm_info->port);

	}
	else
	{
		result = RRC_FALSE;
	}

	RRC_OAMH_TRACE(RRC_INFO, "\t [MODULE:%s] [INFO:%s] [MODE:%s]",
			OAMH_COMM_MODULES_NAMES[ext_comm_info->ext_module_id],
			&(p_comm_info_int)[0],
			(OAM_TRANSPORT_MODE_TCP == ext_comm_info->transport_mode)?
			"OAM_TRANSPORT_MODE_TCP":"OAM_TRANSPORT_MODE_UDP");

	RRC_OAMH_UT_TRACE_EXIT();
	return result;
}

/*SPR 21653 changes start*/
/******************************************************************************
*   FUNCTION NAME: check_and_update_lower_layer_comm_info
*
*   INPUTS       : rrc_oamh_communication_info_ind_t  
*                                               *p_rrc_oamh_communication_info_ind
*                  rrc_oamh_communication_info_resp_t
*                                               *p_rrc_oamh_communication_info_resp
*                  comm_info_t                       *p_comm_info_int
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function update internal communication info for Lower Layer
*       from RRC_OAM_COMMUNICATION_INFO_REQ
*
*   RETURNS:
*       RRC_TRUE  - if all connections are OK
*       RRC_FALSE - if we have an invalid connection
******************************************************************************/
static rrc_bool_et check_and_update_lower_layer_comm_info
(
 rrc_oamh_communication_info_ind_t  *p_rrc_oamh_communication_info_ind,
 rrc_oamh_communication_info_resp_t  *p_rrc_oamh_communication_info_resp, 
 comm_info_t                       *p_comm_info_int
 )
{
    rrc_bool_et             result  = RRC_TRUE;
    U8                      index   = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();

    p_comm_info_int->sector_comm_info_list.sector_comm_info[p_rrc_oamh_communication_info_ind->l2_instance_id].cell_index =
        p_rrc_oamh_communication_info_ind->cell_index;

    RRC_OAMH_TRACE(RRC_INFO,"Handling Lower Layer Communication Info for [Instance: %d]",
            p_rrc_oamh_communication_info_ind->l2_instance_id);




    for (index = 0;	index < RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES;index++)
    {
        /*If checks Passes for Comm Info then update global context and
         * prepare comm Info response */
        if (RRC_FALSE ==  update_communication_info_l2(&(p_rrc_oamh_communication_info_ind->ext_comm_info_lower_layer[index]),
                    p_comm_info_int->sector_comm_info_list.sector_comm_info[p_rrc_oamh_communication_info_ind->l2_instance_id].
                    comm_info_lower_layer[((p_rrc_oamh_communication_info_ind
                            ->ext_comm_info_lower_layer[index].ext_module_id) - 
                        (RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES - RRC_MIN_EXT_MODULE_ID)) ]))
        {
            result = RRC_FALSE;
        }
    }

    RRC_OAMH_UT_TRACE_EXIT();

    return result;
}
#endif
/* SPR 20908 End */
/******************************************************************************
*   FUNCTION NAME: check_and_update_upper_layer_comm_info
*
*   INPUTS        : rrc_oam_communication_info_req_
*                                          *p_rrc_oam_communication_info_req
*                   rrc_oam_communication_info_resp_t
*                                          *p_rrc_oam_communication_info_resp
*                   comm_info_t                       *p_comm_info_int
*                   U8                                *ext_module_id_bitmask
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function update internal communication info for Upper Layer
*       from RRC_OAM_COMMUNICATION_INFO_REQ
*
*   RETURNS:
*       RRC_TRUE  - if all connections are OK
*       RRC_FALSE - if we have an invalid connection
******************************************************************************/
static rrc_bool_et check_and_update_upper_layer_comm_info
(
    rrc_oam_communication_info_req_t  *p_rrc_oam_communication_info_req,
    rrc_oam_communication_info_resp_t *p_rrc_oam_communication_info_resp, 
    comm_info_t                       *p_comm_info_int,
    U8                                *ext_module_id_bitmask
 )
{
    rrc_bool_et             result  = RRC_TRUE;
    U8                      index   = RRC_NULL;
    U8                      ext_module_id_bit      = RRC_NULL;
    U8                      ext_module_id      = RRC_NULL;

    RRC_OAMH_UT_TRACE_ENTER();

    for (index = 0; 
            index < RRC_OAM_MAX_EXT_UPPER_LAYER_MODULES; 
            index++)
    {
        /* SPR 1893 start*/
        ext_module_id_bit = (U8)(1 << (( p_rrc_oam_communication_info_req->
                        ext_comm_info_upper_layer[index].
                        ext_module_id )- 1 ));
        if(*ext_module_id_bitmask & ext_module_id_bit)
        {
            RRC_OAMH_TRACE(RRC_WARNING,"Multiple communication informations received for module %s",
                    OAMH_COMM_MODULES_NAMES[p_rrc_oam_communication_info_req->
                    ext_comm_info_upper_layer[index].ext_module_id]);
            result = RRC_FALSE;
            continue;
        }
        *ext_module_id_bitmask |= ext_module_id_bit;

        ext_module_id = p_rrc_oam_communication_info_req->ext_comm_info_upper_layer[index].
            ext_module_id;

        if (ext_module_id !=RRC_OAM_MODULE_ID &&
                ext_module_id != RRC_RRM_MODULE_ID &&
                 ext_module_id != RRC_MODULE_ID)
        {
            RRC_OAMH_TRACE(RRC_WARNING,"Wrong Entity Id: %d received, Only Upper Layer"
                    "Entities should be present",
                    p_rrc_oam_communication_info_req->ext_comm_info_upper_layer[index].
                    ext_module_id);

            result = RRC_FALSE;
            continue;
        }

        if (RRC_FALSE == update_communication_info(&(p_rrc_oam_communication_info_req->
                ext_comm_info_upper_layer[index]), &(p_rrc_oam_communication_info_resp->
                ext_connection_status_upper_layer[(p_rrc_oam_communication_info_req->
                ext_comm_info_upper_layer[index].ext_module_id) - RRC_MIN_EXT_MODULE_ID]),
                p_comm_info_int->comm_info_upper_layer[p_rrc_oam_communication_info_req->
                ext_comm_info_upper_layer[index].ext_module_id - RRC_MIN_EXT_MODULE_ID]))
        {
            result = RRC_FALSE;
        }
    }

    RRC_OAMH_UT_TRACE_EXIT();

    return result;
}
/*SPR 21653 changes end*/

/******************************************************************************
*   FUNCTION NAME: establish_all_connections
*
*   INPUTS        : rrc_oam_communication_info_resp_t
*                                               *p_rrc_oam_communication_info_resp
*                   comm_info_t                  *p_comm_info_int
*                   rrc_oam_communication_info_req_
*                                          *p_rrc_oam_communication_info_req 
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function update internal communication info according to data in
*       RRC_OAM_COMMUNICATION_INFO_REQ and prepare
*       RRC_OAM_COMMUNICATION_INFO_RESP.
*       This function shouldn't be executed in parallel with driver's
*       s_send function
*
*   RETURNS:
*       RRC_TRUE  - if all connections are OK
*       RRC_FALSE - if we have an invalid connection
******************************************************************************/
static rrc_bool_et establish_all_connections
(
    /* RRC_OAM_COMMUNICATION_INFO_RESP */
    rrc_oam_communication_info_resp_t *p_rrc_oam_communication_info_resp,
    /* internal communication info used in driver */
    comm_info_t                  *p_comm_info_int,
    /* source - RRC_OAM_COMMUNICATION_INFO_REQ */
    rrc_oam_communication_info_req_t  *p_rrc_oam_communication_info_req
)
{
    rrc_bool_et             result  = RRC_TRUE;
/*SPR 21653 changes start*/
/* SPR 20908 Start */
    rrc_bool_et             result_upper_layer   = RRC_TRUE;
/* SPR 20908 End */
/*SPR 21653 changes end*/
    U8                      index   = RRC_NULL;
    U8                      ext_module_id_bitmask  = RRC_NULL; 

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_rrc_oam_communication_info_resp);
    RRC_ASSERT(PNULL != p_comm_info_int);
    RRC_ASSERT(PNULL != p_rrc_oam_communication_info_req);

    RRC_OAMH_TRACE(RRC_INFO,"Updating communication info.");
   
/*SPR 21653 changes start*/
/* SPR 20908 Start */
    result_upper_layer = check_and_update_upper_layer_comm_info(
            p_rrc_oam_communication_info_req,
            p_rrc_oam_communication_info_resp, p_comm_info_int,
            &ext_module_id_bitmask);

    result = result_upper_layer;

    p_rrc_oam_communication_info_resp->bitmask =
                   RRC_NULL;

    p_rrc_oam_communication_info_resp->response = result;

/*SPR 21653 changes end*/

/* SPR 20908 End */
    /* SPR 1893 */
    if(RRC_FALSE == result)
    {
        p_rrc_oam_communication_info_resp->bitmask = 
            RRC_OAM_COMM_INFO_FAIL_CAUSE_PRESENT;

        p_rrc_oam_communication_info_resp->cause = RRC_OAM_COMM_INFO_FAILURE;
        p_rrc_oam_communication_info_resp->response = RRC_FAILURE;

        for(index = 0; 
                index < RRC_OAM_MAX_EXT_UPPER_LAYER_MODULES; 
                index++)
        {
            if(RRC_OAM_CONNECTED != p_rrc_oam_communication_info_resp->
                    ext_connection_status_upper_layer[index].
                connection_status)
            {
                p_rrc_oam_communication_info_resp->ext_connection_status_upper_layer[index].
                   connection_status = RRC_OAM_NOT_CONNECTED;
                p_rrc_oam_communication_info_resp->ext_connection_status_upper_layer[index].
                   ext_module_id = p_rrc_oam_communication_info_req->
                   ext_comm_info_upper_layer[index].ext_module_id;
            }

        }
/* SPR 20908 Start */
/*SPR 21653 changes start*/
/* SPR 20908 End */
/*SPR 21653 changes end*/
    }

    /*spr 1893 stop*/
    RRC_OAMH_UT_TRACE_EXIT();
    return result;
}
/* SPR 20908 Start */
/******************************************************************************
*   FUNCTION NAME: establish_all_connections_l2
*
*   INPUTS        : rrc_oamh_communication_info_resp_t
*                                               *rrc_oamh_communication_info_resp
*                   comm_info_t                  *p_comm_info_int
*                   rrc_oamh_communication_info_ind_t
*                                          *p_rrc_oamh_communication_info_ind 
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function update internal communication info according to data in
*       CSC_OAMH_OAM_COMMUNICATION_INFO_IND and prepare
*       CSC_OAMH_OAM_COMMUNICATION_INFO_RESP.
*
*   RETURNS:
*       RRC_TRUE  - if all connections are OK
*       RRC_FALSE - if we have an invalid connection
******************************************************************************/
static rrc_bool_et establish_all_connections_l2
(
    /* RRC_OAMH_COMMUNICATION_INFO_RESP */
    rrc_oamh_communication_info_resp_t *p_rrc_oamh_communication_info_resp,
    /* internal communication info used in driver */
    comm_info_t                  *p_comm_info_int,
    /* source - RRC_OAMH_COMMUNICATION_INFO_IND */
    rrc_oamh_communication_info_ind_t  *p_rrc_oamh_communication_info_ind
)
{
    rrc_bool_et             result;

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_rrc_oamh_communication_info_resp);
    RRC_ASSERT(PNULL != p_comm_info_int);
    RRC_ASSERT(PNULL != p_rrc_oamh_communication_info_ind);

    RRC_OAMH_TRACE(RRC_INFO,"Updating Layer_2 communication info.");
   
#ifndef RRC_UNIT_TEST_FRAMEWORK
    result = check_and_update_lower_layer_comm_info(p_rrc_oamh_communication_info_ind,
            p_rrc_oamh_communication_info_resp, p_comm_info_int);
#else
    result = RRC_TRUE;
#endif
	/*SPR 21096 Start*/
        p_comm_info_int->sector_comm_info_list.count = RRC_THREE;
	/*SPR 21096 End*/
    p_rrc_oamh_communication_info_resp->cell_index = p_rrc_oamh_communication_info_ind->cell_index;
    p_rrc_oamh_communication_info_resp->result = result;

    RRC_OAMH_UT_TRACE_EXIT();
    return result;
}
/* SPR 20908 End */


/******************************************************************************
*   FUNCTION NAME: oamh_trace_algorithms
*   INPUTS        : const U8   *data
*                   U8         data_size
*                   const S8 **description
*   OUTPUTS      : None
*
*   DESCRIPTION:
*           This is function puts detailed info about alogorithms into LOG
*
*   RETURNS:
*           None
******************************************************************************/
void oamh_trace_algorithms
(
    const U8   *data,
    U8         data_size,
    const S8 **description
)
{
    U8 index  = RRC_NULL;
    U8 alg_id = RRC_NULL;

    RRC_OAMH_TRACE(RRC_INFO, "\tNumber of Algorithms [%u]", data_size);
    RRC_ASSERT(PNULL != data);

    for(index = 0; index < data_size; index++)
    {
        alg_id = data[index];
        RRC_OAMH_TRACE(RRC_INFO,"\t Algorithm ID:%s", description[alg_id]);
    }
}
/* SPR 20908 End */
/******************************************************************************
*   FUNCTION NAME: oamh_fsm_trace_timer_configuration
*
*   INPUTS        : U8                    num_of_timers
*                   U8         data_size
*   OUTPUTS      : None
*   DESCRIPTION:
*           This is function puts detailed info about timers into LOG
*
*   RETURNS:
*           None
******************************************************************************/
void oamh_fsm_trace_timer_configuration
(
    U8                    num_of_timers,
    rrc_oam_timer_info_t  *p_timer_info
)
{
    U8 i = RRC_NULL;

    RRC_ASSERT(PNULL != p_timer_info);
    RRC_OAMH_TRACE(RRC_INFO, "\tNumber of timers [%u]", num_of_timers);

    for (i = 0; i < num_of_timers; i++)
    {
        RRC_OAMH_TRACE(RRC_INFO, "\t[TIMER_ID:%s] [VALUE:%u ms]",
                OAMH_TIMERS_NAMES[p_timer_info[i].timer_id],
                p_timer_info[i].timer_val);
    }
}


/* OAMH FSM handlers mapping */
static oamh_fsm_ret oamh_fsm[OAMH_NUM_OF_STATES][OAMH_NUM_OF_EVENTS] =
{
    /* State OAMH_INIT */
    {
        oamh_csc_init_ind_handler,    /* OAMH_EVENT_CSC_INIT_IND      */
        oamh_llim_init_ind_handler,   /* OAMH_EVENT_LLIM_INIT_IND     */
        oamh_ueccmd_init_ind_handler, /* OAMH_EVENT_UECCMD_INIT_IND     */
        oamh_invalid_event_handler,   /* OAMH_EVENT_OAM_COMM_INFO_REQ */
        oamh_invalid_event_handler,   /* OAMH_EVENT_OAM_PROV_REQ      */
        oamh_invalid_event_handler,   /* OAMH_EVENT_CSC_PROV_RESP     */
        oamh_invalid_event_handler,   /* OAMH_EVENT_LLIM_PROV_RESP    */
        oamh_invalid_event_handler,   /* OAMH_EVENT_UECCMD_PROV_RESP    */
        oamh_invalid_event_handler,   /* OAMH_EVENT_OAM_CLEANUP_REQ   */
        oamh_invalid_event_handler,   /* OAMH_EVENT_CSC_CLEANUP_RESP  */
        oamh_invalid_event_handler,   /* OAMH_EVENT_LLIM_CLEANUP_RESP */
        oamh_invalid_event_handler,    /* OAMH_EVENT_UECCMD_CLEANUP_RESP */
        /*lipa start*/
        oamh_oam_discard_add_lgw_req_handler,    /* OAMH_EVENT_OAM_ADD_LGW_REQ */
        oamh_oam_discard_del_lgw_req_handler    /* OAMH_EVENT_OAM_DEL_LGW_REQ */
        /*lipa end*/
    },
    /* State OAMH_STATE_W_FOR_COMM_PROV_REQ */
    {
        oamh_invalid_event_handler,    /* OAMH_EVENT_CSC_INIT_IND      */
        oamh_invalid_event_handler,    /* OAMH_EVENT_LLIM_INIT_IND     */
        oamh_invalid_event_handler,    /* OAMH_EVENT_UECCMD_INIT_IND     */
        oamh_oam_comm_req_handler,     /* OAMH_EVENT_OAM_COMM_INFO_REQ */
        oamh_oam_prov_req_handler,     /* OAMH_EVENT_OAM_PROV_REQ      */
        oamh_csc_prov_resp_handler,    /* OAMH_EVENT_CSC_PROV_RESP     */
        oamh_llim_prov_resp_handler,   /* OAMH_EVENT_LLIM_PROV_RESP    */
        oamh_ueccmd_prov_resp_handler, /* OAMH_EVENT_UECCMD_PROV_RESP    */
        oamh_oam_cleanup_req_handler,  /* OAMH_EVENT_OAM_CLEANUP_REQ   */
        oamh_invalid_event_handler,    /* OAMH_EVENT_CSC_CLEANUP_RESP  */
        oamh_invalid_event_handler,    /* OAMH_EVENT_LLIM_CLEANUP_RESP */
        oamh_invalid_event_handler,     /* OAMH_EVENT_UECCMD_CLEANUP_RESP */
        /*lipa start*/
        oamh_oam_discard_add_lgw_req_handler,    /* OAMH_EVENT_OAM_ADD_LGW_REQ */
        oamh_oam_discard_del_lgw_req_handler    /* OAMH_EVENT_OAM_DEL_LGW_REQ */
        /*lipa end*/
    },
    /* State OAMH_STATE_W_FOR_CLEANUP_RESP */
    {
        oamh_invalid_event_handler,      /* OAMH_EVENT_CSC_INIT_IND      */
        oamh_invalid_event_handler,      /* OAMH_EVENT_LLIM_INIT_IND     */
        oamh_invalid_event_handler,      /* OAMH_EVENT_UECCMD_INIT_IND     */
        oamh_invalid_event_handler,      /* OAMH_EVENT_OAM_COMM_INFO_REQ */
        oamh_invalid_event_handler,      /* OAMH_EVENT_OAM_PROV_REQ      */
        oamh_invalid_event_handler,      /* OAMH_EVENT_CSC_PROV_RESP     */
        oamh_invalid_event_handler,      /* OAMH_EVENT_LLIM_PROV_RESP    */
        oamh_invalid_event_handler,      /* OAMH_EVENT_UECCMD_PROV_RESP    */
        oamh_second_cleanup_req_handler, /* OAMH_EVENT_OAM_CLEANUP_REQ   */
        oamh_csc_cleanup_resp_handler,   /* OAMH_EVENT_CSC_CLEANUP_RESP  */
        oamh_llim_cleanup_resp_handler,  /* OAMH_EVENT_LLIM_CLEANUP_RESP */
        oamh_ueccmd_cleanup_resp_handler, /* OAMH_EVENT_UECCMD_CLEANUP_RESP */
        /*lipa start*/
        oamh_oam_discard_add_lgw_req_handler,    /* OAMH_EVENT_OAM_ADD_LGW_REQ */
        oamh_oam_discard_del_lgw_req_handler    /* OAMH_EVENT_OAM_DEL_LGW_REQ */
        /*lipa end*/
    },
    /* State OAMH_STATE_ACTIVE */
    {
        oamh_invalid_event_handler,   /* OAMH_EVENT_CSC_INIT_IND      */
        oamh_invalid_event_handler,   /* OAMH_EVENT_LLIM_INIT_IND     */
        oamh_invalid_event_handler,   /* OAMH_EVENT_UECCMD_INIT_IND     */
        oamh_invalid_event_handler,   /* OAMH_EVENT_OAM_COMM_INFO_REQ */
        oamh_invalid_event_handler,   /* OAMH_EVENT_OAM_PROV_REQ      */
        oamh_invalid_event_handler,   /* OAMH_EVENT_CSC_PROV_RESP     */
        oamh_invalid_event_handler,   /* OAMH_EVENT_LLIM_PROV_RESP    */
        oamh_invalid_event_handler,   /* OAMH_EVENT_UECCMD_PROV_RESP    */
        oamh_oam_cleanup_req_handler, /* OAMH_EVENT_OAM_CLEANUP_REQ   */
        oamh_invalid_event_handler,   /* OAMH_EVENT_CSC_CLEANUP_RESP  */
        oamh_invalid_event_handler,   /* OAMH_EVENT_LLIM_CLEANUP_RESP */
        oamh_invalid_event_handler,    /* OAMH_EVENT_UECCMD_CLEANUP_RESP */
        /*lipa start*/
        oamh_oam_add_lgw_req_handler,   /* OAMH_EVENT_OAM_ADD_LGW_REQ */
        oamh_oam_del_lgw_req_handler, /* OAMH_EVENT_OAM_DEL_LGW_REQ */
        /*lipa end*/
        /* SPR 20908 Start */
        oamh_csc_comm_info_ind_handler     /* OAMH_EVENT_CSC_COMMUNICATION_INFO_IND */
        /* SPR 20908 End */
    }
};

/******************************************************************************
*   FUNCTION NAME: oamh_fsm_process_event
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   oamh_event_et         event
*                   U16                   api_id
*                   void                  *p_msg
*   OUTPUTS      : None
*   DESCRIPTION:
*           This is entry point function for OAMH FSM. This function
*           calls appropriate function handler based on state and event
*           combination.
*
*   RETURNS:
*           None
******************************************************************************/
void oamh_fsm_process_event
(
    rrc_oamh_gb_context_t *p_oamh_gb_context,
    oamh_event_et         event,
    U16                   api_id,
    void                  *p_msg
)
{
    oamh_state_et current_state = (oamh_state_et)(OAMH_GET_FSM_STATE(p_oamh_gb_context));

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_OAMH_TRACE(RRC_DETAILEDALL, 
            "Unused variable in fsm p_msg=%d", api_id);
    if ((current_state < OAMH_NUM_OF_STATES) && (event < OAMH_NUM_OF_EVENTS))
    {
        RRC_OAMH_FSM_EVENT_TRACE(RRC_OAMH_FSM_NAME,
                                 OAMH_FSM_EVENTS_NAMES[event],
                                 OAMH_FSM_STATES_NAMES[current_state]);

        /* Invoke appropriate event handler */
        (*oamh_fsm[current_state][event])(p_oamh_gb_context, p_msg);
    }
    else
    {
        RRC_OAMH_TRACE(RRC_ERROR, "oamh_fsm_process_event: unexpected state %u"
            " or event %u", current_state, event);
        RRC_ASSERT(0);
    }

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: process_and_validate_add_lgw_req 
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   
*   OUTPUTS      : ueccmd_oamh_add_del_lgw_ind_t *p_msg
*   DESCRIPTION:
*           This function processes RRC_OAM_ADD_LGW_REQ message and validates
*           the IP address configuration received for LGW. If correct 
*           configuration is received, then populates ueccmd_oamh_add_del_lgw_ind
*           with the configuration received for LGW. 
*
*   RETURNS:
*          RRC_SUCCESS/RRC_FAILURE 
******************************************************************************/
static rrc_return_et process_and_validate_add_lgw_req
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    rrc_oam_add_lgw_req_t *p_lgw_req,
    ueccmd_oamh_add_del_lgw_ind_t *p_ueccmd_oamh_add_del_lgw_ind
)
{
    rrc_return_et result  = RRC_SUCCESS;

    U8 index = 0;
    U8 index2 = 0;
    S8 number_of_char = 0;
    U8 min_ip_value[RRC_MAX_IP_ADDR_LEN_IPV6] = {RRC_NULL};
    U8 max_ip_value[RRC_MAX_IP_ADDR_LEN_IPV6] = {255};
    U8 temp_buffer[MAX_LEN_COMM_INFO] = {RRC_NULL};
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT( PNULL != p_lgw_req );
    RRC_ASSERT( PNULL != p_ueccmd_oamh_add_del_lgw_ind );

    for ( index = 0 ; 
            index< RRC_MAX_IP_ADDR_LEN_IPV6;
            index++ )
    {
        max_ip_value[index] =  255;
    }    
    /*If both IPV4 and IPV6 LGW IP Addresses are configured OR
      None of them is configured, then return failure*/
    if ( ((p_lgw_req->bitmask & RRC_OAM_ADD_LGW_IPV4_ADDR_PRESENT) 
                &&
                (p_lgw_req->bitmask & RRC_OAM_ADD_LGW_IPV6_ADDR_PRESENT))
            ||
            ( p_lgw_req->bitmask == RRC_NULL ) )
    {
        RRC_OAMH_TRACE(RRC_ERROR, "Invalid bitmask received from OAM");
        result = RRC_FAILURE;
    }
    else
    {
        if (p_lgw_req->bitmask & RRC_OAM_ADD_LGW_IPV4_ADDR_PRESENT)
        {
            if ( (RRC_NULL == memcmp_wrapper(min_ip_value,
                                   p_lgw_req->ip_addr,
                                  RRC_MAX_IP_ADDR_LEN ))
                          ||
                 (RRC_NULL == memcmp_wrapper(max_ip_value,
                                        p_lgw_req->ip_addr,
                                        RRC_MAX_IP_ADDR_LEN)))
            {
                RRC_OAMH_TRACE(RRC_ERROR, "Invalid IPv4 address received");
                result = RRC_FAILURE;
            }
            else
            {
                /* Comparing that the IPv4 address received for LGW is not the
                 * same as already configured for some other module at L3
                 * As the addresses received in oam_communication_info_req are
                 * only IPv4 addresses , here comparison is only done when IPv4
                 * address is received for LGW 
                 * First of all, writing the IP address received in octet
                 * string to the string format for comparison*/

                number_of_char =  snprintf_wrapper((Char8 *)(temp_buffer),
                        MAX_LEN_COMM_INFO,
                        (const Char8*)"%u.%u.%u.%u:",
                        p_lgw_req->ip_addr[0],
                        p_lgw_req->ip_addr[1],
                        p_lgw_req->ip_addr[2],
                        p_lgw_req->ip_addr[3]);

                /* Comparing the IP address with the upper layer OAM, RRM */
                for (index = 0 ; index < RRC_OAM_MAX_EXT_SON_UPPER_LAYER_MODULES ; index++)
                {
                    if (RRC_NULL == memcmp_wrapper(
                                p_oamh_gb_context->p_comm_info_int->comm_info_upper_layer[index],
                                temp_buffer,
                                number_of_char))
                    {
                        RRC_OAMH_TRACE(RRC_ERROR, "IPv4 address received is configured for some other module");
                        result = RRC_FAILURE;
                        break;
                    }
                }
                /* Comparing the IP address with the lower layers S1U, MAC,
                 * PDCP, RLC, PHY*/
                if (RRC_FAILURE != result) 
                {

                    for (index = 0 ; index < p_oamh_gb_context->p_comm_info_int->sector_comm_info_list.count ; index++) 
                    {
                        for (index2 = 0 ; index2 < RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES; index2++ )
                        {
                            if (RRC_NULL == memcmp_wrapper(
                                        p_oamh_gb_context->p_comm_info_int->sector_comm_info_list.sector_comm_info[index].comm_info_lower_layer[index2],
                                        temp_buffer,
                                        number_of_char))
                            {
                                RRC_OAMH_TRACE(RRC_ERROR, "IPv4 address received is configured for some other module");
                                result = RRC_FAILURE;
                                break;

                            }
                        }
                        if ( RRC_FAILURE == result ) 
                        {
                            break;
                        }
                 
                    }              
                }

                if (RRC_FAILURE != result) 
                {
                    p_ueccmd_oamh_add_del_lgw_ind->bitmask |= 
                        RRC_OAM_ADD_LGW_ADDR_PRESENT;

                    p_ueccmd_oamh_add_del_lgw_ind->data_length = 
                        RRC_MAX_IP_ADDR_LEN;

                    l3_memcpy_wrapper((void *)p_ueccmd_oamh_add_del_lgw_ind->lgw_addr,
                            (void *)p_lgw_req->ip_addr,
                            p_ueccmd_oamh_add_del_lgw_ind->data_length);
                }
            }
        }
        else 
        {
            if ( (RRC_NULL == memcmp_wrapper(min_ip_value,
                                   p_lgw_req->ipv6_addr,
                                  RRC_MAX_IP_ADDR_LEN_IPV6))
                          ||
                 (RRC_NULL == memcmp_wrapper(max_ip_value,
                                        p_lgw_req->ipv6_addr,
                                        RRC_MAX_IP_ADDR_LEN_IPV6)))
            {
                RRC_OAMH_TRACE(RRC_ERROR, "Invalid IPv6 address received");
                result = RRC_FAILURE;
            }

            p_ueccmd_oamh_add_del_lgw_ind->bitmask |= 
                  RRC_OAM_ADD_LGW_ADDR_PRESENT;

            p_ueccmd_oamh_add_del_lgw_ind->data_length = 
                RRC_MAX_IP_ADDR_LEN_IPV6;
           
            l3_memcpy_wrapper((void *)p_ueccmd_oamh_add_del_lgw_ind->lgw_addr,
                (void *)p_lgw_req->ipv6_addr,
                p_ueccmd_oamh_add_del_lgw_ind->data_length);
        } 
        
    } 

    RRC_OAMH_UT_TRACE_EXIT();
    return result;
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_add_lgw_req_handler 
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                  *p_msg
*   OUTPUTS      :  None 
*   DESCRIPTION:
*           This function handles RRC_OAM_ADD_LGW_REQ message and validates
*           the IP address configuration received for LGW. If correct 
*           configuration is received, then sends ueccmd_oamh_add_del_lgw_ind
*           with the configuration received for LGW to UECCMD. It also sends 
*           RRC_OAM_ADD_LGW_RESP to OAM with response code Sucess/Failure as 
*           the case may be. 
*
*   RETURNS:
*          None 
******************************************************************************/
static void oamh_oam_add_lgw_req_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    rrc_oam_add_lgw_resp_t rrc_oam_add_lgw_resp;
    rrc_oam_add_lgw_req_t *p_rrc_oam_add_lgw_req   = PNULL ;
    ueccmd_oamh_add_del_lgw_ind_t ueccmd_oamh_add_del_lgw_ind;
    
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);
	
    p_rrc_oam_add_lgw_req = (rrc_oam_add_lgw_req_t *)(p_msg);
    
    memset_wrapper(&rrc_oam_add_lgw_resp, RRC_NULL, sizeof(rrc_oam_add_lgw_resp_t));

    /* Check that LGW is not already configured */ 
    if ( p_oamh_gb_context->lgw_configured_status & OAMH_LGW_CONFIGURED ) 	
    {
        RRC_OAMH_TRACE(RRC_ERROR, "LGW is already Configured");
        rrc_oam_add_lgw_resp.bitmask |= RRC_OAM_ADD_LGW_RESP_FAILURE_CAUSE_PRESENT;
     	rrc_oam_add_lgw_resp.response = RRC_FAILURE;
	    rrc_oam_add_lgw_resp.cause =  LGW_ALREADY_CONFIGURED;   	
    } 
    else
    {
        memset_wrapper(&ueccmd_oamh_add_del_lgw_ind, RRC_NULL,
                sizeof(ueccmd_oamh_add_del_lgw_ind_t));

        if ( RRC_FAILURE == process_and_validate_add_lgw_req(p_oamh_gb_context, p_rrc_oam_add_lgw_req,
                    &ueccmd_oamh_add_del_lgw_ind) )
        {
            RRC_OAMH_TRACE(RRC_ERROR, "RRC_OAM_ADD_LGW_REQ Unsuccessful, sending FAILURE to OAM");
            rrc_oam_add_lgw_resp.bitmask |= RRC_OAM_ADD_LGW_RESP_FAILURE_CAUSE_PRESENT;
            rrc_oam_add_lgw_resp.response = RRC_FAILURE;
            rrc_oam_add_lgw_resp.cause = INVALID_LGW_IP_ADDR_CONFIGURED;   	
        }
        else 
        {
            /* Store IP address of LGW in oamh_gb_context */
            rrc_oam_add_lgw_resp.bitmask = RRC_NULL;
            rrc_oam_add_lgw_resp.response = RRC_SUCCESS;

            p_oamh_gb_context->lgw_configured_status = OAMH_LGW_CONFIGURED;

            /*send indication to ueccmd  for informing the lgw address*/ 
            rrc_oamh_send_internal_msg(RRC_UECCMD_MODULE_ID,
                    UECCMD_OAMH_ADD_DEL_LGW_IND,
                    sizeof(ueccmd_oamh_add_del_lgw_ind),
                    &ueccmd_oamh_add_del_lgw_ind);
            RRC_OAMH_TRACE(RRC_INFO, "RRC_OAM_ADD_LGW_REQ successful, LGW IP Addr sent to UECC MD");

            RRC_OAMH_TRACE(RRC_INFO, "RRC_OAM_ADD_LGW_REQ successful, sending SUCCESS to OAM");
        }
    }

    /* Send RRC_OAM_ADD_LGW_RESPONSE  to OAM*/
    if (RRC_SUCCESS == rrc_oam_il_send_rrc_oam_add_lgw_resp(
            &rrc_oam_add_lgw_resp,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            p_oamh_gb_context->lgw_req_trans_id,
            RRC_NULL))
   {
     RRC_OAMH_TRACE(RRC_INFO, "RRC_OAM_ADD_LGW_RESP sent successfully to OAM");
   }

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME:  oamh_oam_del_lgw_req_handler 
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                  *p_msg
*   OUTPUTS      :  None 
*   DESCRIPTION:
*           This function handles RRC_OAM_DELETE_LGW_REQ , deletes the
*           configuration of LGW. It sends ueccmd_oamh_add_del_lgw_ind
*           to UECCMD indicating LGW deletion.It also sends 
*           RRC_OAM_DELETE_LGW_RESP to OAM with response code Sucess/Failure  
*           depending on the conditions. 
*
*   RETURNS:
*          None 
******************************************************************************/
static void oamh_oam_del_lgw_req_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    rrc_oam_delete_lgw_resp_t rrc_oam_delete_lgw_resp;
    ueccmd_oamh_add_del_lgw_ind_t ueccmd_oamh_add_del_lgw_ind;
    

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);
    
    RRC_OAMH_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_msg);         
    memset_wrapper(&rrc_oam_delete_lgw_resp, RRC_NULL, sizeof (rrc_oam_delete_lgw_resp_t));
 
    if (!( p_oamh_gb_context->lgw_configured_status & OAMH_LGW_CONFIGURED )) 	
    {
	   /* LGW is not  configured */
        RRC_OAMH_TRACE(RRC_ERROR, "Can't delete LGW as It's NOT Configured");
        rrc_oam_delete_lgw_resp.bitmask |= RRC_OAM_DELETE_LGW_RESP_FAILURE_CAUSE_PRESENT;
    	rrc_oam_delete_lgw_resp.response = RRC_FAILURE;
        rrc_oam_delete_lgw_resp.cause = LGW_NOT_CONFIGURED ;   	
    } 
    else
    {
        rrc_oam_delete_lgw_resp.bitmask = RRC_NULL;
        rrc_oam_delete_lgw_resp.response = RRC_SUCCESS;
        p_oamh_gb_context->lgw_configured_status = RRC_NULL;

        /* send indication to uecc-md for deleting the lgw */ 
        memset_wrapper(&ueccmd_oamh_add_del_lgw_ind, RRC_NULL,
                sizeof (ueccmd_oamh_add_del_lgw_ind_t));
        rrc_oamh_send_internal_msg(RRC_UECCMD_MODULE_ID,
                UECCMD_OAMH_ADD_DEL_LGW_IND,
                sizeof(ueccmd_oamh_add_del_lgw_ind_t),
                &ueccmd_oamh_add_del_lgw_ind);
        RRC_OAMH_TRACE(RRC_INFO, "Delete LGW Req sent to UECC MD");
        RRC_OAMH_TRACE(RRC_INFO, "Sending Delete LGW Resp to OAM with SUCCESS");
    }	

    /* Send RRC_OAM_ADD_LGW_RESPONSE  to OAM*/
    rrc_oam_il_send_rrc_oam_delete_lgw_resp(
            &rrc_oam_delete_lgw_resp,
            RRC_OAMH_MODULE_ID,
            RRC_OAM_MODULE_ID,
            p_oamh_gb_context->lgw_req_trans_id,
            RRC_NULL);

    RRC_OAMH_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: oamh_oam_discard_add_lgw_req_handler 
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                  *p_msg
*   OUTPUTS      :  None 
*   DESCRIPTION:
*           This function discards the RRC_OAM_ADD_LGW_REQ message received 
*           from OAM. It sends RRC_OAM_ADD_LGW_RESP to OAM with response as 
*           FAILURE and cause as "received in invalid L3 state". 
*
*   RETURNS:
*          None 
******************************************************************************/
static void oamh_oam_discard_add_lgw_req_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    rrc_oam_add_lgw_resp_t rrc_oam_add_lgw_resp;
    
    
    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);
	
    RRC_OAMH_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_msg);         
    
    memset_wrapper(&rrc_oam_add_lgw_resp, RRC_NULL, sizeof(rrc_oam_add_lgw_resp_t));
    
    /*LGW received in Invalid STATE */
    RRC_OAMH_TRACE(RRC_ERROR, "RRC_OAM_ADD_LGW_REQ received in Invalid STATE[%u]", p_oamh_gb_context->current_state);
    rrc_oam_add_lgw_resp.bitmask |= RRC_OAM_ADD_LGW_RESP_FAILURE_CAUSE_PRESENT;
    rrc_oam_add_lgw_resp.response = RRC_FAILURE;
    rrc_oam_add_lgw_resp.cause = ADD_LGW_REQ_RECVD_IN_INVALID_L3_STATE;
    
    /* Send RRC_OAM_ADD_LGW_RESPONSE  to OAM*/
    rrc_oam_il_send_rrc_oam_add_lgw_resp(
                        &rrc_oam_add_lgw_resp,
                        RRC_OAMH_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        p_oamh_gb_context->lgw_req_trans_id,
                        RRC_NULL);
    RRC_OAMH_TRACE(RRC_INFO, "RRC_OAM_ADD_LGW_REQ Discarded. FAILURE sent to OAM");

    RRC_OAMH_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: oamh_oam_discard_del_lgw_req_handler 
*
*   INPUTS        : rrc_oamh_gb_context_t *p_oamh_gb_context
*                   void                  *p_msg
*   OUTPUTS      :  None 
*   DESCRIPTION:
*           This function discards the RRC_OAM_DELETE_LGW_REQ message received 
*           from OAM. It sends RRC_OAM_DELETE_LGW_RESP to OAM with response as 
*           FAILURE and cause as "received in invalid L3 state". 
*
*   RETURNS:
*          None 
******************************************************************************/
static void oamh_oam_discard_del_lgw_req_handler
(
    rrc_oamh_gb_context_t  *p_oamh_gb_context,
    void                   *p_msg
)
{
    rrc_oam_delete_lgw_resp_t rrc_oam_delete_lgw_resp;
    

    RRC_OAMH_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_msg);
    RRC_ASSERT(PNULL != p_oamh_gb_context);
    RRC_OAMH_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_msg);         
	
    memset_wrapper(&rrc_oam_delete_lgw_resp, RRC_NULL, sizeof(rrc_oam_delete_lgw_resp_t));
    
    /* RRC_OAM_DELETE_LGW_REQ received in Invalid State */
    RRC_OAMH_TRACE(RRC_ERROR, "RRC_OAM_DELETE_LGW_REQ received in Invalid STATE[%u]", p_oamh_gb_context->current_state);
    rrc_oam_delete_lgw_resp.bitmask |= RRC_OAM_DELETE_LGW_RESP_FAILURE_CAUSE_PRESENT;
    rrc_oam_delete_lgw_resp.response = RRC_FAILURE;
    rrc_oam_delete_lgw_resp.cause = DEL_LGW_REQ_RECVD_IN_INVALID_L3_STATE;   	
    
    /* Send RRC_OAM_ADD_LGW_RESPONSE  to OAM*/
    rrc_oam_il_send_rrc_oam_delete_lgw_resp(
                        &rrc_oam_delete_lgw_resp,
                        RRC_OAMH_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        p_oamh_gb_context->lgw_req_trans_id,
                        RRC_NULL);
    RRC_OAMH_TRACE(RRC_INFO, "DELETE LGW RESP sent to OAM with FAILURE");

    RRC_OAMH_UT_TRACE_EXIT();
}
/* lipa end */
