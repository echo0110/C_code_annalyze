/****************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 * $Id: son_nmmctl_fsm.c $
 *
 ****************************************************************************
 *
 * File Description: Processes NMM controller states and events. It contains
 *                   the definition of various event handlers for NMM
 *                   controller module.
 *
 ****************************************************************************
 * Revision Details
 * ----------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   July, 2012      Sandip Samal                Initial code
 *   Aug, 2014       Shilpi                      SPR 13251 Fix
 ****************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_nmm.h>
#include <son_nmm_apps_intf.h>
#include <rrm_son_intf.h>
#include <rrm_son_parser.h>
#include <rrm_son_composer.h>

/****************************************************************************
 *  Private Definitions
 ****************************************************************************/

/****************************************************************************
 *                              global variables
 *****************************************************************************/
static nmmctl_periodic_nw_scan_req_t   *p_periodic_scan_data = SON_PNULL;
static start_scan_req_t           *p_last_scan_data     = SON_PNULL;
static nmmctl_start_scan_req_t    *p_running_start_scan = SON_PNULL;
static nmmctl_stop_scan_req_t     *p_running_stop_scan  = SON_PNULL;
static smif_shutdown_req_t        *p_shutdown_req       = SON_PNULL;
/* Start: CSR 00070093 */
static son_timer_t               g_nmmctl_periodic_scan_timer_id  = SON_PNULL;
/*End: CSR 00070093 */
static son_timer_t               g_nmmctl_scan_resp_timer_id      = SON_NULL;
static nmmctl_start_scan_res_t   g_last_scan_resp_data            = {SON_NULL};
static son_nmm_time_stamp_t g_last_scan_timestamp            = {SON_NULL};

/*
**   Global definitions
*/
const S8 *NMMCTL_FSM_STATES_NAMES[] =
{
    (const S8 *)"NMMCTL_STATE_INIT",
    (const S8 *)"NMMCTL_STATE_ACTIVE",
    (const S8 *)"NMMCTL_STATE_W_PREPARE_RES",
    (const S8 *)"NMMCTL_STATE_W_SCAN_RES",
    (const S8 *)"NMMCTL_STATE_W_COMPLT_RES",
    (const S8 *)"NMMCTL_STATE_W_STOP_RES",
    (const S8 *)"NMMCTL_STATE_W_SHTDWN_RES",
    (const S8 *)"NMMCTL_MAX_STATE"
};

#ifdef SON_DEBUG
static const S8 *NMMCTL_FSM_EVENTS_NAMES[] =
{
    (const S8 *)"NMMCTL_SMIF_INIT_CONFIG_REQ_EV",
    (const S8 *)"NMMCTL_SMIF_LOG_ENABLE_DISABLE_CMD_EV",
    (const S8 *)"NMMCTL_SMIF_LOG_ENABLE_DISABLE_CMD_EV",
    (const S8 *)"NMMCTL_START_SCAN_REQ_EV",
    (const S8 *)"NMMCTL_PERIODIC_SCAN_REQ_EV",
    (const S8 *)"NMMCTL_SMIF_SET_NW_SCAN_CONFIG_CMD_EV",
    (const S8 *)"NMMCTL_START_SCAN_RES_EV",
    (const S8 *)"NMMCTL_STOP_SCAN_REQ_EV",
    (const S8 *)"NMMCTL_STOP_SCAN_RES_EV",
    (const S8 *)"NMMCTL_SCAN_PREPARE_RES_EV",
    (const S8 *)"NMMCTL_SCAN_COMPLETE_RES_EV",
    (const S8 *)"NMMCTL_SMIF_SHUTDOWN_REQ_EV"
};
#endif /* ifdef SON_DEBUG */

/****************************************************************************
 * Exported variables
 ****************************************************************************/

/***************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Macro definitions
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void
nmmctl_fsm_periodic_nmm_scan_req
(
     void            *p_api_buf,
     nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
nmmctl_fsm_init_config_req
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
nmmctl_fsm_set_log_level_cmd
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
nmmctl_fsm_log_enable_disable_cmd
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);
 
static void
nmmctl_fsm_nmm_scan_req
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
nmmctl_fsm_enqueue_nmm_scan_req
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
nmmctl_fsm_nmm_prepare_res
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
nmmctl_fsm_start_scan_res
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
nmmctl_fsm_scan_complete_res
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
nmmctl_fsm_stop_scan_req
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
nmmctl_fsm_shutdown_req
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
nmmctl_fsm_stop_scan_res
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
nmmctl_fsm_invalid_event_handler
(
    void            *p_api_buf, 
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
nmmctl_stop_scan_handler
(
    /*SPR 17777 +-*/
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static int
nmmctl_scan_data_compare
(
    const void     *p_key1,
    const void     *p_key2
);

static const void *
nmmctl_curr_scan_object_id_keyof
(
    const YLNODE *p_ylnode
);

static nmmctl_event_code_et
nmmctl_convert_internal_apitoevent
(
    son_procedure_code_et son_api_id
);

static nmmctl_event_code_et
nmmctl_convert_external_apitoevent
(
    rrm_son_message_resp_et son_api_id
);

static void
nmmctl_prepare_send_external_msg
(
    son_procedure_code_et api_id
);

static void *
nmmctl_get_external_msg
(
    U8                    *p_api_buf,
    rrm_son_message_resp_et api_id
);

static void
scan_response_from_scaned_data
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

static U8
is_scan_and_scaned_data_same
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);
static void
nmmctl_fsm_free_last_scan_data
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

static void
nmmctl_fsm_set_scan_config_cmd
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);

static void
create_and_send_last_scan_timestamp_ind
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/* SPR-13251 Fix Starts */
static void
nmmctl_fsm_get_log_level_req_handler
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
);
/* SPR-13251 Fix Ends */

/****************************************************************************
 *                      Variable initialization
 ****************************************************************************/
                                            
/***********************************************************************
 * FSM Entry Point Table
 **********************************************************************/
typedef void (*nmmctl_fsm_evt_handler_t)
(
    void *p_api_buf,
    nmm_gl_ctx_t *p_nmm_gl_ctx
);
static nmmctl_fsm_evt_handler_t nmmctl_fsm_table[NMMCTL_MAX_STATE][NMMCTL_MAX_EVENT] =
{
    /* State NMMCTL_STATE_INIT */
    {
        nmmctl_fsm_init_config_req,             /* SMIF_INIT_CONFIG_REQ */
        nmmctl_fsm_invalid_event_handler,       /* SMIF_LOG_ENABLE_DISABLE_CMD */
        nmmctl_fsm_invalid_event_handler,       /* SMIF_SET_LOG_LEVEL_CMD */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_START_SCAN_REQ */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_PERIODIC_SCAN_REQ */
        nmmctl_fsm_invalid_event_handler,        /* SMIF_SET_NW_SCAN_CONFIG_CMD */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_START_SCAN_RES */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_STOP_SCAN_REQ */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_STOP_SCAN_RES */
        nmmctl_fsm_invalid_event_handler,       /* RRM_SON_NMM_PREPARE_RESP */
        nmmctl_fsm_invalid_event_handler,       /* RRM_SON_NMM_COMPLETE_RESP */
        nmmctl_fsm_shutdown_req,                 /* SMIF_SHUTDOWN_REQ */
        /* SPR-13251 Fix Starts */
        nmmctl_fsm_invalid_event_handler,       /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State NMMCTL_STATE_ACTIVE */
    {
        nmmctl_fsm_invalid_event_handler,       /* SMIF_INIT_CONFIG_REQ */
        nmmctl_fsm_log_enable_disable_cmd,      /* SMIF_LOG_ENABLE_DISABLE_CMD */
        nmmctl_fsm_set_log_level_cmd,           /* SMIF_SET_LOG_LEVEL_CMD */
        nmmctl_fsm_nmm_scan_req,                /* SONNMM_START_SCAN_REQ */
        nmmctl_fsm_periodic_nmm_scan_req,       /* SONNMM_PERIODIC_SCAN_REQ */
        nmmctl_fsm_set_scan_config_cmd,         /* SMIF_SET_NW_SCAN_CONFIG_CMD */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_START_SCAN_RES */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_STOP_SCAN_REQ */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_STOP_SCAN_RES */
        nmmctl_fsm_invalid_event_handler,       /* RRM_SON_NMM_PREPARE_RESP */
        nmmctl_fsm_invalid_event_handler,       /* RRM_SON_NMM_COMPLETE_RESP */
        nmmctl_fsm_shutdown_req,                /* SMIF_SHUTDOWN_REQ */    
        /* SPR-13251 Fix Starts */
        nmmctl_fsm_get_log_level_req_handler,   /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State NMMCTL_STATE_W_PREPARE_RES */
    {
        nmmctl_fsm_invalid_event_handler,       /* SMIF_INIT_CONFIG_REQ */
        nmmctl_fsm_log_enable_disable_cmd,      /* SMIF_LOG_ENABLE_DISABLE_CMD */
        nmmctl_fsm_set_log_level_cmd,           /* SMIF_SET_LOG_LEVEL_CMD */
        nmmctl_fsm_enqueue_nmm_scan_req,        /* SONNMM_START_SCAN_REQ */
        nmmctl_fsm_periodic_nmm_scan_req,       /* SONNMM_PERIODIC_SCAN_REQ */
        nmmctl_fsm_set_scan_config_cmd,         /* SMIF_SET_NW_SCAN_CONFIG_CMD */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_START_SCAN_RES */
        nmmctl_fsm_stop_scan_req,               /* SONNMM_STOP_SCAN_REQ */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_STOP_SCAN_RES */
        nmmctl_fsm_nmm_prepare_res,             /* RRM_SON_NMM_PREPARE_RESP */
        nmmctl_fsm_invalid_event_handler,       /* RRM_SON_NMM_COMPLETE_RESP */
        nmmctl_fsm_shutdown_req,                /* SMIF_SHUTDOWN_REQ */
        /* SPR-13251 Fix Starts */
        nmmctl_fsm_get_log_level_req_handler,   /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State NMMCTL_STATE_W_SCAN_RES */
    {
        nmmctl_fsm_invalid_event_handler,       /* SMIF_INIT_CONFIG_REQ */
        nmmctl_fsm_log_enable_disable_cmd,      /* SMIF_LOG_ENABLE_DISABLE_CMD */
        nmmctl_fsm_set_log_level_cmd,           /* SMIF_SET_LOG_LEVEL_CMD */
        nmmctl_fsm_enqueue_nmm_scan_req,        /* SONNMM_START_SCAN_REQ */
        nmmctl_fsm_periodic_nmm_scan_req,       /* SONNMM_PERIODIC_SCAN_REQ */
        nmmctl_fsm_set_scan_config_cmd,         /* SMIF_SET_NW_SCAN_CONFIG_CMD */        
        nmmctl_fsm_start_scan_res,              /* SONNMM_START_SCAN_RES */
        nmmctl_fsm_stop_scan_req,               /* SONNMM_STOP_SCAN_REQ */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_STOP_SCAN_RES */
        nmmctl_fsm_invalid_event_handler,       /* RRM_SON_NMM_PREPARE_RESP */
        nmmctl_fsm_invalid_event_handler,       /* RRM_SON_NMM_COMPLETE_RESP */
        nmmctl_fsm_shutdown_req,                /* SMIF_SHUTDOWN_REQ */ 
        /* SPR-13251 Fix Starts */
        nmmctl_fsm_get_log_level_req_handler,   /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State NMMCTL_STATE_W_COMPLT_RES */
    {
        nmmctl_fsm_invalid_event_handler,       /* SMIF_INIT_CONFIG_REQ */
        nmmctl_fsm_log_enable_disable_cmd,      /* SMIF_LOG_ENABLE_DISABLE_CMD */
        nmmctl_fsm_set_log_level_cmd,           /* SMIF_SET_LOG_LEVEL_CMD */
        nmmctl_fsm_enqueue_nmm_scan_req,        /* SONNMM_START_SCAN_REQ */
        nmmctl_fsm_periodic_nmm_scan_req,       /* SONNMM_PERIODIC_SCAN_REQ */
        nmmctl_fsm_set_scan_config_cmd,         /* SMIF_SET_NW_SCAN_CONFIG_CMD */        
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_START_SCAN_RES */
        nmmctl_fsm_stop_scan_req,               /* SONNMM_STOP_SCAN_REQ */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_STOP_SCAN_RES */
        nmmctl_fsm_invalid_event_handler,       /* RRM_SON_NMM_PREPARE_RESP */
        nmmctl_fsm_scan_complete_res,           /* RRM_SON_NMM_COMPLETE_RESP */
        nmmctl_fsm_shutdown_req,                /* SMIF_SHUTDOWN_REQ */ 
        /* SPR-13251 Fix Starts */
        nmmctl_fsm_get_log_level_req_handler,   /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State NMMCTL_STATE_W_STOP_RES */
    {
        nmmctl_fsm_invalid_event_handler,       /* SMIF_INIT_CONFIG_REQ */
        nmmctl_fsm_log_enable_disable_cmd,      /* SMIF_LOG_ENABLE_DISABLE_CMD */
        nmmctl_fsm_set_log_level_cmd,           /* SMIF_SET_LOG_LEVEL_CMD */
        nmmctl_fsm_enqueue_nmm_scan_req,        /* SONNMM_START_SCAN_REQ */
        nmmctl_fsm_periodic_nmm_scan_req,       /* SONNMM_PERIODIC_SCAN_REQ */
        nmmctl_fsm_set_scan_config_cmd,         /* SMIF_SET_NW_SCAN_CONFIG_CMD */        
        nmmctl_fsm_start_scan_res,              /* SONNMM_START_SCAN_RES */
        nmmctl_fsm_stop_scan_req,               /* SONNMM_STOP_SCAN_REQ */
        nmmctl_fsm_stop_scan_res,               /* SONNMM_STOP_SCAN_RES */
        nmmctl_fsm_invalid_event_handler,       /* RRM_SON_NMM_PREPARE_RESP */
        nmmctl_fsm_invalid_event_handler,       /* RRM_SON_NMM_COMPLETE_RESP */
        nmmctl_fsm_shutdown_req,                /* SMIF_SHUTDOWN_REQ */ 
        /* SPR-13251 Fix Starts */
        nmmctl_fsm_get_log_level_req_handler,   /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State NMMCTL_STATE_W_SHTDWN_RES */
    {
        nmmctl_fsm_invalid_event_handler,       /* SMIF_INIT_CONFIG_REQ */
        nmmctl_fsm_invalid_event_handler,       /* SMIF_LOG_ENABLE_DISABLE_CMD */
        nmmctl_fsm_invalid_event_handler,       /* SMIF_SET_LOG_LEVEL_CMD */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_START_SCAN_REQ */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_PERIODIC_SCAN_REQ */
        nmmctl_fsm_invalid_event_handler,        /* SONNMM_SET_SCAN_CONFIG_CMD */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_START_SCAN_RES */
        nmmctl_fsm_invalid_event_handler,       /* SONNMM_STOP_SCAN_REQ */
        nmmctl_fsm_stop_scan_res,               /* SONNMM_STOP_SCAN_RES */
        nmmctl_fsm_nmm_prepare_res,             /* RRM_SON_NMM_PREPARE_RESP */
        nmmctl_fsm_scan_complete_res,           /* RRM_SON_NMM_COMPLETE_RESP */
        nmmctl_fsm_invalid_event_handler,       /* SMIF_SHUTDOWN_REQ */
        /* SPR-13251 Fix Starts */
        nmmctl_fsm_invalid_event_handler,       /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    }
};

/****************************************************************************
 *                      Functions forward declarations
 *****************************************************************************/

/****************************************************************************
 *                      Functions implementations
 *****************************************************************************/

/*****************************************************************************
 * Function Name  : son_nmmctl_fsm_process_msg
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function calls the appropriate event handler as per the
 *                  incoming event and current state of NMM controller.
 ****************************************************************************/
void
son_nmmctl_fsm_process_msg
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    nmmctl_ctx_t          *p_nmmctl_ctx = SON_PNULL;
    nmmctl_fsm_state_et   current_state = NMMCTL_MAX_STATE;
    nmmctl_event_code_et  event         = NMMCTL_MAX_EVENT;
    STACKAPIHDR cspl_hdr = {0};

    SON_UT_TRACE_ENTER();

    son_parse_cspl_header((U8 *)p_api_buf, &cspl_hdr);
    
    /* Get the NMM controller global context pointer */
    p_nmmctl_ctx = p_nmm_gl_ctx->p_nmmctl_ctx;

    /* Get current state of NMM controller */
    current_state = NMMCTL_FSM_GET_CURRENT_STATE(p_nmmctl_ctx);

    if (cspl_hdr.from == SON_RRM_MODULE_ID)
    {
        /* Call the external API ID to event ID conversion function */
        event = nmmctl_convert_external_apitoevent((rrm_son_message_resp_et)cspl_hdr.api);
    }
    else
    {
        /* Call the Internal API ID to event ID conversion function */
        event = nmmctl_convert_internal_apitoevent((son_procedure_code_et)cspl_hdr.api);
    }

    if ((current_state < NMMCTL_MAX_STATE) && (event < NMMCTL_MAX_EVENT))
    {
        #ifdef SON_DEBUG
        SON_FSM_EVENT_TRACE("NMM controller FSM",
                            NMMCTL_FSM_EVENTS_NAMES[event],
                            NMMCTL_FSM_STATES_NAMES[current_state]);
        #endif
        nmmctl_fsm_table[current_state][event](p_api_buf, p_nmm_gl_ctx);
    }
    else
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
        "son_nmmctl_fsm_process_msg: unexpected state"
        "%u or event %u", current_state, event);
    }

    SON_UT_TRACE_EXIT();
} /* son_nmmctl_fsm_process_msg */

/*****************************************************************************
 * Function Name  : create_and_send_last_scan_timestamp_ind 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends the timestamp indication to MIF 
 ****************************************************************************/
static void
create_and_send_last_scan_timestamp_ind
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    son_nmm_last_scan_timestamp_ind_t  timestamp_resp = {SON_NULL};
    

    SON_UT_TRACE_ENTER();

    SON_LOG(SON_OAM_LOG_ON, p_son_nmm_facility_name, SON_BRIEF,
            " Last scan timestamp");

    /* Populate Timestamp RES message data */
    SON_MEMCPY(&timestamp_resp.time_stamp,
                      &g_last_scan_timestamp,
                      sizeof (son_nmm_time_stamp_t));
    son_create_send_buffer((U8 *)&timestamp_resp,
                           SON_NMM_MODULE_ID,
                           SON_MIF_MODULE_ID,
                           SONNMM_LAST_SCAN_TIMESTAMP_IND,
                           sizeof(son_nmm_last_scan_timestamp_ind_t));
    
    SON_UT_TRACE_EXIT();

    return;
} /* create_and_send_last_scan_timestamp_ind */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_init_config_req
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when SMIF_INIT_CONFIG_REQ message
 *                  is received from MIF.
 ****************************************************************************/
static void
nmmctl_fsm_init_config_req
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    nmmctl_ctx_t          *p_nmmctl_ctx = SON_PNULL;
    smif_init_config_res_t smif_init_config_res;
    smif_init_config_req_t *p_smif_init_config_req = SON_PNULL;


    SON_UT_TRACE_ENTER();

    /* Fetch the NMM controller specific global context data pointer */
    p_nmmctl_ctx = p_nmm_gl_ctx->p_nmmctl_ctx;

    p_smif_init_config_req = (smif_init_config_req_t *)(p_api_buf + SON_API_HEADER_SIZE);

    /* 
    ** Enable/Disable the NMM log and set the log level to the values
    ** provided in the SMIF_INIT_CONFIG_REQ.
    */
    g_nmm_log_enable = p_smif_init_config_req->log_enable;

    /* 
    ** Print the info about the current logging status(enable/disable)
    ** Pass first argument as true such that this info is printed
    ** even if logging is disabled.
    */
    SON_LOG(SON_OAM_LOG_ON, p_son_nmm_facility_name, SON_BRIEF,
            "Log enable/disable = %u",
            g_nmm_log_enable);

    /* Set module level logging */
    if (SON_OAM_LOG_ON == g_nmm_log_enable)
    {
        /* SPR-13251 Fix Starts */
        g_nmm_log_level = p_smif_init_config_req->log_level;
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(p_smif_init_config_req->log_level);
    }
    else
    {
        /* SPR-13251 Fix Starts */
        g_nmm_log_level = SON_LOG_LEVEL_NONE;
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
    }

    /* Populate SMIF_INIT_CONFIG_RES message data */
    smif_init_config_res.transaction_id =
      p_smif_init_config_req->transaction_id;
    smif_init_config_res.result = SON_SUCCESS;
    smif_init_config_res.error_code = SON_NO_ERROR;

    /* Set the NMMCTL FSM state to ACTIVE */
    NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_ACTIVE);

    /*
    ** Call the prepare and send message handler
    ** to send the stop Scan response to SONApps
    */
    son_create_send_buffer((U8 *)&smif_init_config_res,
                           SON_NMM_MODULE_ID,
                           SON_MIF_MODULE_ID,
                           SMIF_INIT_CONFIG_RES,
                           sizeof(smif_init_config_res_t));
    
    SON_UT_TRACE_EXIT();

    return;
} /* nmmctl_fsm_init_config_req */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_log_enable_disable_cmd
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SMIF_LOG_ENABLE_DISABLE_CMD
 *                  message received from SONMgmtIfH.
 ****************************************************************************/
static void
nmmctl_fsm_log_enable_disable_cmd
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    smif_log_enable_disable_cmd_t *p_enable_disable_cmd = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_enable_disable_cmd = 
     (smif_log_enable_disable_cmd_t *)(p_api_buf + SON_API_HEADER_SIZE);

    g_nmm_log_enable = p_enable_disable_cmd->log_enable;

    /* 
    ** Print the info about the current logging status(enable/disable)
    ** Pass first argument as true such that this info is printed
    ** even if logging is disabled.
    */
    SON_LOG(SON_OAM_LOG_ON, p_son_nmm_facility_name, SON_BRIEF,
    /*SPR 17777 +-*/
        "nmm_log_enable_cmd: Log enable/disable = %u %u",
        g_nmm_log_enable,sizeof(p_nmm_gl_ctx));

    /* 
    ** Enable/Disable the NMM log and set the log level to the values
    ** provided in the SMIF_LOG_ENABLE_DISABLE_CMD.
    */
    if (SON_OAM_LOG_ON == g_nmm_log_enable)
    {
        if (p_enable_disable_cmd->bitmask & SMIF_MODULE_LOG_LEVEL_PRESENT)
        {
            /* SPR-13251 Fix Starts */
            g_nmm_log_level = p_enable_disable_cmd->log_level;
            /* SPR-13251 Fix Ends */
            SET_MODULE_LOG_LEVEL(p_enable_disable_cmd->log_level);
        }
    }
    else
    {
        /* SPR-13251 Fix Starts */
        g_nmm_log_level = SON_LOG_LEVEL_NONE;
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
    }

    SON_UT_TRACE_EXIT();

    return;
} /* nmmctl_fsm_log_enable_disable_cmd */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_set_log_level_cmd
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SMIF_SET_LOG_LEVEL_CMD
 *                  message received from SONMgmtIfH.
 ****************************************************************************/
static void
nmmctl_fsm_set_log_level_cmd
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    /*SPR 17777 +-*/
    (void)p_nmm_gl_ctx;
    smif_set_log_level_cmd_t *p_smif_set_log_level_cmd = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_smif_set_log_level_cmd = 
     (smif_set_log_level_cmd_t *)(p_api_buf + SON_API_HEADER_SIZE);

    /* Set the log level as provided in the SMIF_SET_LOG_LEVEL_CMD. */
    if (SON_OAM_LOG_ON == g_nmm_log_enable)
    {
        /* SPR-13251 Fix Starts */
        g_nmm_log_level = p_smif_set_log_level_cmd->log_level;
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(p_smif_set_log_level_cmd->log_level);
    }

    SON_UT_TRACE_EXIT();

    return;
} /* nmmctl_fsm_set_log_level_cmd */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_nmm_scan_req
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when SONNMM_START_SCAN_REQ message
 *                  is received to start a scan.
 ****************************************************************************/
static void
nmmctl_fsm_nmm_scan_req
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    nmmctl_ctx_t    *p_nmmctl_ctx = SON_PNULL;


    SON_UT_TRACE_ENTER();

    /* Fetch the NMM controller specific global context data pointer */
    p_nmmctl_ctx = p_nmm_gl_ctx->p_nmmctl_ctx;

    /* Save the message received from SONAPPS/ANR
     * Get memory to save the received scan data */
    p_running_start_scan =
        (nmmctl_start_scan_req_t *)son_mem_get(sizeof(nmmctl_start_scan_req_t));

    if (SON_PNULL == p_running_start_scan)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                "nmmctl_fsm_nmm_scan_req: Mem allocation fail");
        /*SPR-12970-Start*/
        return;
        /*SPR-12970-End*/
    }
    else
    {
        /* Parse CSPL HDR */
        son_parse_cspl_header((U8 *)p_api_buf, (STACKAPIHDR *) &(p_running_start_scan->hdr));
        /* Copy the received scan data */
        SON_MEMCPY((void *) &(p_running_start_scan->nmmctl_start_scan_req),
                (p_api_buf + SON_API_HEADER_SIZE),
                sizeof(son_nmm_start_scan_req_t));

        /* Verify that, is the requested scan data same as last scanned data */
        if ((p_nmmctl_ctx->nmm_scan_config_param.force_scan == SON_FALSE) &&
            ((g_last_scan_resp_data.scan_resp_data_valid == SON_TRUE) && 
            (SON_TRUE == is_scan_and_scaned_data_same())))
        {
            /* Send the scan response from the saved last scan result */
            scan_response_from_scaned_data();
        }
        else
        {    
            /* Call the prepare and send message handler to send RRM prepare message */
            nmmctl_prepare_send_external_msg((son_procedure_code_et)RRM_SON_NMM_PREPARE_REQ);
            /* Change NMM controller state to NMMCTL_STATE_W_PREPARE_RES */
            NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_PREPARE_RES);
        }
    }

    SON_UT_TRACE_EXIT();

    return;
} /* nmmctl_fsm_nmm_scan_req */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_periodic_nmm_scan_req
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when SONNMM_PERIODIC_SCAN_REQ message
 *                  is received to start a scan.
 ****************************************************************************/
static void
nmmctl_fsm_periodic_nmm_scan_req
(
 void            *p_api_buf,
 nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    U8              periodic_scan_flag = SON_NULL;
    nmmctl_ctx_t    *p_nmmctl_ctx      = SON_PNULL;
    void            *p_msg             = SON_PNULL;


    SON_UT_TRACE_ENTER();

    /* Fetch the NMM controller specific global context data pointer */
    p_nmmctl_ctx = p_nmm_gl_ctx->p_nmmctl_ctx;

    if (p_periodic_scan_data == SON_PNULL)
    {
        /* Save the message received from SONAPPS
         * Get memory to save the received scan data */
        p_periodic_scan_data =
            (nmmctl_periodic_nw_scan_req_t *)son_mem_get(sizeof(nmmctl_periodic_nw_scan_req_t));
        if (SON_PNULL == p_periodic_scan_data)
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "nmmctl_fsm_periodic_nmm_scan_req: Mem allocation fail");
            /*SPR-12970-Start*/
            return;
            /*SPR-12970-End*/
        }
        else
        {
             SON_MEMSET(p_periodic_scan_data, SON_NULL, sizeof(nmmctl_periodic_nw_scan_req_t));
        }
    }

    if (p_periodic_scan_data != SON_PNULL)
    {
        /* Fetch the scan ON/OFF status from the requested scan data */
        periodic_scan_flag = ((son_nmm_periodic_nw_scan_req_t *)(p_api_buf + SON_API_HEADER_SIZE))->scan_on_off;

        if ((PERIODIC_SCAN_OFF == p_periodic_scan_data->nmmctl_periodic_nw_scan_req.scan_on_off)
            && (PERIODIC_SCAN_ON == periodic_scan_flag))
        {
            /* Parse CSPL HDR */
            son_parse_cspl_header((U8 *)p_api_buf, (STACKAPIHDR *) &(p_periodic_scan_data->hdr));
            /* Copy the received scan data */
            SON_MEMCPY((void *) &(p_periodic_scan_data->nmmctl_periodic_nw_scan_req),
                       (p_api_buf + SON_API_HEADER_SIZE),
                        sizeof(son_nmm_periodic_nw_scan_req_t));
            /*start timer*/
            g_nmmctl_periodic_scan_timer_id =
            /*SPR_16636 Fix Start*/
            /* SPR 19834 Fix Start */
                son_start_timer(p_periodic_scan_data->nmmctl_periodic_nw_scan_req.scan_time_interval,
            /* SPR 19834 Fix End */
            /*SPR_16636 Fix End*/
                        &p_nmmctl_ctx->nmmctl_periodic_scan_timer_buf,
                        sizeof(p_nmmctl_ctx->nmmctl_periodic_scan_timer_buf),
                        SON_TRUE);
        }
        else if ((PERIODIC_SCAN_ON == p_periodic_scan_data->nmmctl_periodic_nw_scan_req.scan_on_off)
                 && (PERIODIC_SCAN_ON == periodic_scan_flag))
        {
            /* Stop the Timer */
            son_stop_timer(g_nmmctl_periodic_scan_timer_id);
            /* Start: CSR 00070093 */
            g_nmmctl_periodic_scan_timer_id = SON_PNULL;
            /*End: CSR 00070093 */
        
            /* Parse CSPL HDR */
            son_parse_cspl_header((U8 *)p_api_buf, (STACKAPIHDR *) &(p_periodic_scan_data->hdr));
            /* Copy the received scan data */
            SON_MEMCPY((void *) &(p_periodic_scan_data->nmmctl_periodic_nw_scan_req),
                       (p_api_buf + SON_API_HEADER_SIZE),
                       sizeof(son_nmm_periodic_nw_scan_req_t));
        
            /*start timer*/
            g_nmmctl_periodic_scan_timer_id = 
            /*SPR_16636 Fix Start*/
                son_start_timer(p_periodic_scan_data->nmmctl_periodic_nw_scan_req.scan_time_interval * SON_MILLISECONDS_IN_A_SECOND,
            /*SPR_16636 Fix End*/
                        &p_nmmctl_ctx->nmmctl_periodic_scan_timer_buf,
                        sizeof(p_nmmctl_ctx->nmmctl_periodic_scan_timer_buf),
                        SON_TRUE);
        }
        else if ((PERIODIC_SCAN_ON == p_periodic_scan_data->nmmctl_periodic_nw_scan_req.scan_on_off)
                && (PERIODIC_SCAN_OFF == periodic_scan_flag))
        {
            /* Stop the Timer */
            son_stop_timer(g_nmmctl_periodic_scan_timer_id);
            /* Start: CSR 00070093 */
            g_nmmctl_periodic_scan_timer_id = SON_PNULL;
            /*End: CSR 00070093 */
            SON_MEMSET(p_periodic_scan_data,
                       SON_NULL,
                       sizeof(nmmctl_periodic_nw_scan_req_t));
        }
        else
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "nmmctl_fsm_periodic_nmm_scan_req: Unexpexted Message received");
        }

        if (PERIODIC_SCAN_ON == p_periodic_scan_data->nmmctl_periodic_nw_scan_req.scan_on_off)
        {    
            /* Prepare the scan data */
            p_msg = nmmctl_prepare_periodic_scan_data();
            if (p_msg == SON_PNULL)
	    {
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                         "memory allocation failed");
		return;
	    }

            /* Trigger the scan request if NMM controller is in NMMCTL_STATE_ACTIVE state */
            if (p_nmmctl_ctx->current_state == NMMCTL_STATE_ACTIVE)
            {
                nmmctl_fsm_nmm_scan_req(p_msg, p_nmm_gl_ctx);    
            }
            else if ((p_nmmctl_ctx->current_state == NMMCTL_STATE_W_PREPARE_RES) ||
                     (p_nmmctl_ctx->current_state == NMMCTL_STATE_W_SCAN_RES)   ||
                     (p_nmmctl_ctx->current_state == NMMCTL_STATE_W_COMPLT_RES) ||
                     (p_nmmctl_ctx->current_state == NMMCTL_STATE_W_STOP_RES))
            {
                nmmctl_fsm_enqueue_nmm_scan_req(p_msg, p_nmm_gl_ctx);    
            }
            else
            {
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                        "nmmctl_fsm_periodic_nmm_scan_req: Message received in unexpected state");
            }

            /* Free the allocated memory */
            son_mem_free(p_msg);
        }
    }    

    SON_UT_TRACE_EXIT();
    return;
} /* nmmctl_fsm_periodic_nmm_scan_req */

/*****************************************************************************
 * Function Name  : nmmctl_prepare_periodic_scan_data
 * Outputs        : None
 * Returns        : None
 * Description    : This function will prepare the network scan structure from
 *                  the periodic scan data.
 ****************************************************************************/
void *
nmmctl_prepare_periodic_scan_data
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    U32                       msg_size = SON_NULL;
    son_nmm_start_scan_req_t  periodic_scan_data;
    void    *p_buf = SON_NULL;

    msg_size = sizeof (son_nmm_start_scan_req_t);
    p_buf  =  son_mem_get(CV_HDRSIZE + msg_size);
    
    if (SON_PNULL == p_buf)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                "nmmctl_prepare_periodic_scan_data: memory allocation failed");
        /*SPR-12970-Start*/
        return SON_PNULL;
        /*SPR-12970-End*/
    }
    else
    {
        son_construct_cspl_header(p_buf, SON_VERSION_ID, p_periodic_scan_data->hdr.from,
                SON_NMM_MODULE_ID, SONNMM_START_SCAN_REQ, SON_API_PRIORITY, msg_size);
        periodic_scan_data.transaction_id   = p_periodic_scan_data->nmmctl_periodic_nw_scan_req.transaction_id;
        periodic_scan_data.originator_module_id =
            p_periodic_scan_data->nmmctl_periodic_nw_scan_req.originator_module_id;
        periodic_scan_data.p_start_scan_req = p_periodic_scan_data->nmmctl_periodic_nw_scan_req.p_start_scan_req;

        /* Check if data is present. */
        if (SON_NULL != msg_size)
        {
            /* Copy the data in the message memory */
            SON_MEMCPY(p_buf + CV_HDRSIZE, (void *) &periodic_scan_data, msg_size);
        }
    }

    return p_buf;
} /* nmmctl_prepare_periodic_scan_data */

/*****************************************************************************
 * Function Name  : son_nmmctl_time_out_handler
 * Inputs         : timer_id        - Identity of the expired timer
 *                  p_nmm_gl_ctx    - Pointer to the NMM global context data
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes NMM controller timer message.
 ****************************************************************************/
void
son_nmmctl_time_out_handler
(
 son_timer_t         timer_id,
    /*SPR 17777 +-*/
 nmm_gl_ctx_t        *p_nmm_gl_ctx
 )
{
    void    *p_api_buf = SON_NULL;


    SON_UT_TRACE_ENTER();
    if (timer_id == g_nmmctl_periodic_scan_timer_id)
    {   
        /*SPR_16636 Fix Start*/
        /*When periodic nw scan timeout enable force_scan to scan the network forcefully 
        else same data will get return from stored db*/
        /* SPR_17377_Warning_fix + */
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "son_nmmctl_time_out_handler called for periodic nw scan req ,timer Id = %p",timer_id);
        /* SPR_17377_Warning_fix - */
        p_nmm_gl_ctx->p_nmmctl_ctx->nmm_scan_config_param.force_scan = SON_TRUE;
        /*SPR_16636 Fix End*/
        
        
        p_api_buf = nmmctl_prepare_periodic_scan_data();
        if (SON_PNULL != p_api_buf)
        {
            son_nmmctl_fsm_process_msg(p_api_buf, p_nmm_gl_ctx);
            /* Free the allocated memory */
            son_mem_free(p_api_buf);
        }
        else
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "son_nmmctl_time_out_handler: Failed to prepare periodic "
                    "scan data");
        }
    }
    else if (timer_id == g_nmmctl_scan_resp_timer_id)
    {
        /* Free the saved scan data */
        nmmctl_fsm_free_last_scan_data();
    }    

    SON_UT_TRACE_EXIT();

    return;
} /* son_nmmctl_time_out_handler */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_enqueue_nmm_scan_req
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when SONNMM_START_SCAN_REQ message
 *                  is received and NMM controller is processing a scan request.
 ****************************************************************************/
static void
nmmctl_fsm_enqueue_nmm_scan_req
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    STACKAPIHDR cspl_hdr = {0};
    nmmctl_scan_record_t *p_scaning_record = SON_PNULL;
    nmmctl_ctx_t         *p_nmmctl_ctx     = SON_PNULL;
    nmmctl_scan_queue_t  *p_scaning_queue        = SON_PNULL;
    son_nmm_start_scan_res_t  start_scan_res_data;

    
    SON_UT_TRACE_ENTER();
    
    /* Fetch the NMM controller specific global context data pointer */
    p_nmmctl_ctx = p_nmm_gl_ctx->p_nmmctl_ctx;
    p_scaning_queue = &(p_nmmctl_ctx->scaning_queue);

    /* Parse CSPL header from the received message buffer */
    SON_MEMSET(&cspl_hdr, 0, sizeof(STACKAPIHDR));
    son_parse_cspl_header((U8 *)p_api_buf, &cspl_hdr);

    if (p_running_start_scan == SON_PNULL)
    {
        /* Save the message received from SONAPPS/ANR
         * Get memory to save the received scan data */
        p_running_start_scan =
            (nmmctl_start_scan_req_t *)son_mem_get(sizeof(nmmctl_start_scan_req_t));
        if (SON_PNULL == p_running_start_scan)
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "nmmctl_fsm_enqueue_nmm_scan_req: Mem allocation fail");
            /*SPR-12970-Start*/
            return;
            /*SPR-12970-End*/
        }
        else
        {
            /* Copy the parsed CSPL HDR into p_running_start_scan */
            SON_MEMCPY(&(p_running_start_scan->hdr), &cspl_hdr, sizeof(STACKAPIHDR));
            /* Copy the received scan data */
            SON_MEMCPY(&(p_running_start_scan->nmmctl_start_scan_req),
                       (p_api_buf + SON_API_HEADER_SIZE),
                       sizeof(son_nmm_start_scan_req_t));
        }
    }
    else
    {
        /* 
        ** Enqueue the SCAN request, so that it can be process once the ongoing 
        ** scanning is over.
        */
        if (MAX_ALLOWED_SCAN == SCANING_Q_GET_COUNT(p_scaning_queue))
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "nmmctl_fsm_enqueue_nmm_scan_req: Maximum number of allowed scan reached");

            /* Prepare the scan response message */
            start_scan_res_data.transaction_id = 
                ((son_nmm_start_scan_req_t *)(p_api_buf + SON_API_HEADER_SIZE))->transaction_id;
            start_scan_res_data.originator_module_id = 
                ((son_nmm_start_scan_req_t *)(p_api_buf + SON_API_HEADER_SIZE))->originator_module_id;
            start_scan_res_data.start_scan_res.son_resp  = SON_FAILURE;
            start_scan_res_data.start_scan_res.son_error = SON_ERR_NMM_MAX_ALLOWED_SCAN_REACHED;

            /* Send the scan response data */
            son_create_send_buffer((U8 *)&start_scan_res_data,
                                   SON_NMM_MODULE_ID,
                                   (son_module_id_et)cspl_hdr.from,
                                   SONNMM_START_SCAN_RES,
                                   sizeof(son_nmm_start_scan_res_t));
        }
        else
        {
            do
            {
               /* Get memory to save the start scan data */
               p_scaning_record =
                    (nmmctl_scan_record_t *)son_mem_get(sizeof(nmmctl_scan_record_t));

               if (SON_PNULL == p_scaning_record)
               {
                   SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                           "nmmctl_fsm_enqueue_nmm_scan_req: Mem allocation fail");
                   /*SPR-12970-Start*/
                   return;
                   /*SPR-12970-End*/
               }
               else
               {
                   /* Copy the parsed CSPL HDR into p_running_start_scan */
                   SON_MEMCPY(&(p_scaning_record->scan_data.hdr), &cspl_hdr,
                              sizeof(STACKAPIHDR));
                   /* Copy the received scan data */
                   SON_MEMCPY(&p_scaning_record->scan_data.nmmctl_start_scan_req,
                              (p_api_buf + SON_API_HEADER_SIZE),
                              sizeof(son_nmm_start_scan_req_t));

                   /* Enqueue the scan data */
                   SCANING_Q_ENQUEUE(p_scaning_queue, p_scaning_record);
               }
            } while (SON_NULL);
        }
    }

    SON_UT_TRACE_EXIT();

    return;
} /* nmmctl_fsm_enqueue_nmm_scan_req */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_nmm_prepare_res
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when RRM_SON_NMM_PREPARE_RESP message
 *                  is received from RRM.
 ****************************************************************************/
static void
nmmctl_fsm_nmm_prepare_res
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    nmmctl_ctx_t              *p_nmmctl_ctx              = SON_PNULL;
    nmmctl_scan_record_t      *p_scaning_record          = SON_PNULL;
    nmmctl_scan_queue_t       *p_scaning_queue           = SON_PNULL;
    rrm_son_nmm_prepare_res_t *p_scan_prepare_res        = SON_PNULL;
    nmmctl_fsm_state_et       current_state              = NMMCTL_MAX_STATE;
    U32                       msg_size                   = SON_NULL;
    son_module_id_et          origin_module_id           = SON_ALL_INT_MODULES;
    son_nmm_start_scan_res_t  start_scan_res_data;
    smif_shutdown_res_t       shutdown_res_data;
    nmm_start_scan_data_t     start_scan_data;


    SON_UT_TRACE_ENTER();

    /* Get the NMM controller global context pointer */
    p_nmmctl_ctx    = p_nmm_gl_ctx->p_nmmctl_ctx;
    p_scaning_queue = &(p_nmmctl_ctx->scaning_queue);

    /* Get current state of NMM controller */
    current_state = NMMCTL_FSM_GET_CURRENT_STATE(p_nmmctl_ctx);

    /* Verify the response message received from RRM */
    p_scan_prepare_res = (rrm_son_nmm_prepare_res_t *)
                   nmmctl_get_external_msg(p_api_buf,  RRM_SON_NMM_PREPARE_RESP);
    
    if (p_scan_prepare_res != SON_PNULL)
    {
        if (p_scan_prepare_res->prepare_res.result == RRM_SUCCESS)
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF,
            "nmmctl_fsm_nmm_prepare_res: RRM preparation successful");

            if (p_running_start_scan == SON_PNULL)
            {
                if (current_state == NMMCTL_STATE_W_SHTDWN_RES)
                {
                    /* 
                    ** Call the prepare and send message handler to send
                    ** the Scan complete message to RRM.
                    */
                    nmmctl_prepare_send_external_msg((son_procedure_code_et)RRM_SON_NMM_COMPLETE_REQ);
                }
                else
                {
                    /* 
                    ** Call the prepare and send message handler to send
                    ** the Scan complete message to RRM.
                    */
                    nmmctl_prepare_send_external_msg((son_procedure_code_et)RRM_SON_NMM_COMPLETE_REQ);

                    /* Change NMM controller state to NMMCTL_STATE_W_COMPLT_RES */
                    NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_COMPLT_RES);
                }
            }
            else
            {
                /* 
                ** Send SCAN request to NMM:
                ** 1) Send message to NMM.
                ** 2) Change NMM controller state to NMMCTL_STATE_W_SCAN_RES.
                */
                msg_size = 
                GET_PARAMLEN_FROM_CSPL_HDR(p_running_start_scan) - TRANSACTION_ID_SIZE;
                start_scan_data.p_start_scan_req = p_running_start_scan->
                                     nmmctl_start_scan_req.p_start_scan_req;
                son_create_send_buffer((U8 *)&start_scan_data,
                                       SON_NMM_MODULE_ID,
                                       SON_NMM_MODULE_ID,
                                       SONNMM_START_SCAN_REQ,
                                       msg_size);
                NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_SCAN_RES);
            }
        }
        else
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "nmmctl_fsm_nmm_prepare_res: RRM preparation failure, "
                    "NMM cannot proceed for the scan");
            if (p_running_start_scan == SON_PNULL)
            {
                if (current_state == NMMCTL_STATE_W_SHTDWN_RES)
                {
                    /* Prepare shutdown response */
                    shutdown_res_data.transaction_id = p_shutdown_req->transaction_id;
                    shutdown_res_data.result         = SON_SUCCESS;
                    shutdown_res_data.error_code     = SON_ERR_NMM_SCAN_PREPAIR_FAILURE;
                    /* Send the shutdown response */
                    son_create_send_buffer((U8 *)&shutdown_res_data,
                                           SON_NMM_MODULE_ID,
                                           SON_MIF_MODULE_ID,
                                           SMIF_SHUTDOWN_RES,
                                           sizeof(smif_shutdown_res_t));
                    NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_INIT);
                }
                else
                {
                    NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_ACTIVE);
                }
            }
            else
            {
                /* Get the module ID of the requester, requested the start scan */
                origin_module_id = (son_module_id_et) p_running_start_scan->hdr.from;

                /* Prepare the scan response message */
                start_scan_res_data.transaction_id = 
                 p_running_start_scan->nmmctl_start_scan_req.transaction_id;
                start_scan_res_data.originator_module_id = 
                 p_running_start_scan->nmmctl_start_scan_req.originator_module_id;
                start_scan_res_data.start_scan_res.son_resp  = SON_FAILURE;
                start_scan_res_data.start_scan_res.son_error = SON_ERR_NMM_SCAN_PREPAIR_FAILURE;

                /* Send the scan response data */
                son_create_send_buffer((U8 *)&start_scan_res_data,
                                       SON_NMM_MODULE_ID,
                                       origin_module_id,
                                       SONNMM_START_SCAN_RES,
                                       sizeof(son_nmm_start_scan_res_t));
                /* Free the memory allocated to store the currently running scan data */
                son_mem_free(p_running_start_scan);
                p_running_start_scan = SON_PNULL;

                /* 
                ** Check whether any pending scan message is present
                ** in the NMM controller local queue.
                ** If (yes)
                **     send faliur message to SONApps/ANR for all the scan data
                **     Free all the memory reserved for the scan data
                ** endif
                */
                /* Check for all the pending scan requests */
                while (EMPTY_LIST != SCANING_Q_GET_COUNT(p_scaning_queue))
                {
                    /* Dequeue the start scan data */
                    SCANING_Q_DEQUEUE(p_scaning_queue, p_scaning_record);
                    if (p_scaning_record != SON_PNULL)
                    {
                        /* Prepare the scan response message
                         * Only fill transaction id as result and error code already filled */
                        start_scan_res_data.transaction_id = 
                        p_scaning_record->scan_data.nmmctl_start_scan_req.transaction_id;
                        start_scan_res_data.originator_module_id = 
                        p_scaning_record->scan_data.nmmctl_start_scan_req.originator_module_id;

                        /* 
                        ** Call the prepare and send message handler
                        ** to send Scan response to SONApps for all the pending requests.
                        */
                        son_create_send_buffer((U8 *)&start_scan_res_data,
                                               SON_NMM_MODULE_ID,
                                               origin_module_id,
                                               SONNMM_START_SCAN_RES,
                                               sizeof(son_nmm_start_scan_res_t));
                        /* Free the memory */
                        son_mem_free(p_scaning_record);
                        p_scaning_record = SON_PNULL;
                    }
                }

                /* Change NMM controller state to NMMCTL_STATE_ACTIVE */
                NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_ACTIVE);
            }
        }

        son_mem_free(p_scan_prepare_res);
    }

    SON_UT_TRACE_EXIT();
} /* nmmctl_fsm_nmm_prepare_res */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_start_scan_res
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when SONNMM_START_SCAN_RES message
 *                  is received  from NMM.
 ****************************************************************************/
static void
nmmctl_fsm_start_scan_res
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    STACKAPIHDR cspl_hdr = {0};


    son_parse_cspl_header((U8 *)p_api_buf, &cspl_hdr);
    nmmctl_ctx_t             *p_nmmctl_ctx          = SON_PNULL;
    nmmctl_scan_queue_t      *p_scaning_queue       = SON_PNULL;
    nmmctl_scan_record_t     *p_scaning_record      = SON_PNULL;
    start_scan_res_eutran_data_t *p_start_scan_res_eutran_data = SON_PNULL;
    start_scan_res_utran_data_t  *p_start_scan_res_utran_data  = SON_PNULL;
    U32                      msg_size               = SON_NULL;
    nmmctl_fsm_state_et      current_state          = NMMCTL_MAX_STATE;
    son_module_id_et         origin_module_id       =  SON_ALL_INT_MODULES;
    U8                       valid_scan_data        = SON_NULL;
    son_nmm_start_scan_res_t start_scan_res_data;
    nmm_start_scan_data_t    start_scan_data;

    SON_UT_TRACE_ENTER();

    /* Get the NMM controller global context pointer */
    p_nmmctl_ctx    = p_nmm_gl_ctx->p_nmmctl_ctx;
    p_scaning_queue = &(p_nmmctl_ctx->scaning_queue);

    /* Get current state of NMM controller */
    current_state = NMMCTL_FSM_GET_CURRENT_STATE(p_nmmctl_ctx);

    /* Prepare Scan response data */
    if (current_state == NMMCTL_STATE_W_STOP_RES)
    {
        /* Get the module ID of the requester, requested the start scan */
        origin_module_id = (son_module_id_et)p_running_stop_scan->hdr.from;
        start_scan_res_data.transaction_id =
                    p_running_stop_scan->nmmctl_stop_scan_req.transaction_id;
    }
    else
    {
        /* Get the module ID of the requester, requested the start scan */
        origin_module_id = (son_module_id_et) p_running_start_scan->hdr.from;
        start_scan_res_data.transaction_id =
                    p_running_start_scan->nmmctl_start_scan_req.transaction_id;
        start_scan_res_data.originator_module_id =
                    p_running_start_scan->nmmctl_start_scan_req.originator_module_id;
    }

    SON_MEMCPY(&start_scan_res_data.start_scan_res, 
               (p_api_buf + SON_API_HEADER_SIZE),
               sizeof(nmm_start_scan_res_t));
    
    if (SON_SUCCESS == start_scan_res_data.start_scan_res.son_resp)
    {
        son_nmm_fill_time_stamp(&g_last_scan_timestamp);
        create_and_send_last_scan_timestamp_ind();
    }    

    /* Send the scan response */
    son_create_send_buffer((U8 *)&start_scan_res_data,
                           SON_NMM_MODULE_ID,
                           origin_module_id,
                           SONNMM_START_SCAN_RES,
                           sizeof(son_nmm_start_scan_res_t));

    /* 
    ** Check whether any pending scan message is present in
    ** the NMM controller local queue
    */
    if (current_state != NMMCTL_STATE_W_STOP_RES)
    {
        if ((start_scan_res_data.start_scan_res.son_resp == SON_SUCCESS) &&
            (p_nmmctl_ctx->nmm_scan_config_param.force_scan == SON_FALSE))
        {
            /* SPR NO:10540 changes start */ 
            /* Save the requested input data */
            if (p_last_scan_data == SON_PNULL)
            {
                if (SON_PNULL == (p_last_scan_data= (start_scan_req_t * )son_mem_get(sizeof(start_scan_req_t))))
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_ERROR, \
                            "Mem allocation fail");
                    /*SPR-12970-Start*/
                    return;
                    /*SPR-12970-End*/
                }

               SON_MEMCPY( p_last_scan_data ,p_running_start_scan->nmmctl_start_scan_req.p_start_scan_req,sizeof(start_scan_req_t));
            }
            else
            {
                son_mem_free(p_last_scan_data);
                p_last_scan_data = SON_PNULL;
                if (SON_PNULL == (p_last_scan_data= (start_scan_req_t * )son_mem_get(sizeof(start_scan_req_t))))
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_ERROR, \
                            "Mem allocation fail");
                    /*SPR-12970-Start*/
                    return;
                    /*SPR-12970-End*/
                }


                /* Copy the scan data pointer */
                SON_MEMCPY(p_last_scan_data , p_running_start_scan->nmmctl_start_scan_req.p_start_scan_req,sizeof(start_scan_req_t));
            } 
            /* SPR NO:10540 changes end */ 

            /* Save the response data */
            if (start_scan_res_data.start_scan_res.p_eutran_scan_res_list != SON_PNULL)
            {
                /* Save EUTRAN data */
            if (g_last_scan_resp_data.start_scan_res_data.p_eutran_scan_res_list == SON_PNULL)
                {
                    /* Copy the scan resp data */
                    SON_MEMCPY((void *) &g_last_scan_resp_data.start_scan_res_data,
                               (p_api_buf + SON_API_HEADER_SIZE),
                               sizeof(start_scan_res_t));
                g_last_scan_resp_data.scan_resp_data_valid = SON_TRUE;
            }    
            else
            {
                    while (g_last_scan_resp_data.start_scan_res_data.p_eutran_scan_res_list->count != EMPTY_LIST)
                {
                    /* Get data from the nmm start scan resp list */
                        SCAN_RESP_LIST_DEQUEUE(start_scan_res_eutran_data_t,
                            g_last_scan_resp_data.start_scan_res_data.p_eutran_scan_res_list,
                                               p_start_scan_res_eutran_data);

                    /* Delete the node from nmm_scan_resp_list_t */
                    son_mem_free(p_start_scan_res_eutran_data);
                    p_start_scan_res_eutran_data = SON_PNULL;
                }    

                    /* Save the new data */
                    SON_MEMCPY((void *)&g_last_scan_resp_data.start_scan_res_data,
                        (p_api_buf + SON_API_HEADER_SIZE),
                        sizeof(start_scan_res_t));
                g_last_scan_resp_data.scan_resp_data_valid = SON_TRUE;
            }
        }
            else if (start_scan_res_data.start_scan_res.p_utran_scan_res_list != SON_PNULL)
            {
                /* Save UTRAN data */
                if (g_last_scan_resp_data.start_scan_res_data.p_utran_scan_res_list == SON_PNULL)
                {
                    /* Copy the scan resp data */
                    SON_MEMCPY((void *) &g_last_scan_resp_data.start_scan_res_data,
                               (p_api_buf + SON_API_HEADER_SIZE),
                               sizeof(start_scan_res_t));
                    g_last_scan_resp_data.scan_resp_data_valid = SON_TRUE;
                }
                else
                {
                    while (g_last_scan_resp_data.start_scan_res_data.p_utran_scan_res_list->count != EMPTY_LIST)
                    {
                        /* Get data from the nmm start scan resp list */
                        SCAN_RESP_LIST_DEQUEUE(start_scan_res_utran_data_t,
                            g_last_scan_resp_data.start_scan_res_data.p_utran_scan_res_list,
                                               p_start_scan_res_utran_data);

                        /* Delete the node from nmm_scan_resp_list_t */
                        son_mem_free(p_start_scan_res_utran_data);
                        p_start_scan_res_utran_data = SON_PNULL;
                    }    

                    /* Save the new data */
                    SON_MEMCPY((void *)&g_last_scan_resp_data.start_scan_res_data,
                               (p_api_buf + SON_API_HEADER_SIZE),
                               sizeof(start_scan_res_t));
                    g_last_scan_resp_data.scan_resp_data_valid = SON_TRUE;
                }            
            }
            else
            {
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                           "nmmctl_fsm_start_scan_res: Invalid data received[p_eutran_scan_res_list is NULL]");            
            }
        }
        
        while ((EMPTY_LIST != SCANING_Q_GET_COUNT(p_scaning_queue)) && (valid_scan_data == SON_FALSE))
        {
            /* Dequeue the start scan data */
            SCANING_Q_DEQUEUE(p_scaning_queue, p_scaning_record);
            if (SON_PNULL != p_scaning_record)
            {
                /* Copy the message to the gobal pointer */
                SON_MEMCPY(p_running_start_scan, 
                    &p_scaning_record->scan_data,
                    sizeof(nmmctl_start_scan_req_t));
            }

            /* Free the p_scaning_record */
            son_mem_free(p_scaning_record);
            p_scaning_record = SON_PNULL;

            /* Compair the new scan data with the old scan data that has already been performed */
            if ((p_nmmctl_ctx->nmm_scan_config_param.force_scan == SON_FALSE) &&
                ((g_last_scan_resp_data.scan_resp_data_valid == SON_TRUE) &&
                 (SON_TRUE == is_scan_and_scaned_data_same())))
            {
                /* Send the scan response from the saved last scan result */
                scan_response_from_scaned_data();
            }
            else
            {    
                /* Send SCAN request to NMM */
                msg_size = 
                    GET_PARAMLEN_FROM_CSPL_HDR(p_running_start_scan) - TRANSACTION_ID_SIZE;

                start_scan_data.p_start_scan_req = p_running_start_scan->
                    nmmctl_start_scan_req.p_start_scan_req;
                son_create_send_buffer((U8 *)&start_scan_data,
                        SON_NMM_MODULE_ID,
                        SON_NMM_MODULE_ID,
                        SONNMM_START_SCAN_REQ,
                        msg_size); 
                valid_scan_data = SON_TRUE;
            }
        }
    }
    else
    {
        son_mem_free(p_running_start_scan->nmmctl_start_scan_req.p_start_scan_req);
    }    

    if ((EMPTY_LIST == SCANING_Q_GET_COUNT(p_scaning_queue)) &&
        (valid_scan_data == SON_FALSE))
    {
        /* 
         ** Call the prepare and send message handler
         ** to send the Scan complete message to RRM
         */
        nmmctl_prepare_send_external_msg((son_procedure_code_et)RRM_SON_NMM_COMPLETE_REQ);

        /* Change NMM controller state to NMMCTL_STATE_W_COMPLT_RES */
        NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_COMPLT_RES);

        /* Free the running start scan pointer */
        son_mem_free(p_running_start_scan);
        p_running_start_scan = SON_PNULL;
    }
   
    if ((start_scan_res_data.start_scan_res.son_resp == SON_SUCCESS) &&
        ((p_nmmctl_ctx->nmm_scan_config_param.force_scan == SON_FALSE) &&
         (g_last_scan_resp_data.scan_resp_data_valid == SON_TRUE)))
    {
        if (g_nmmctl_scan_resp_timer_id != SON_NULL)
    {
            /* Stop the scan response timer */
            son_stop_timer(g_nmmctl_scan_resp_timer_id);
            /* Start: CSR 00070093 */
            g_nmmctl_scan_resp_timer_id = SON_PNULL;
            /*End: CSR 00070093 */
        }    

        /*start the scan response timer*/
        g_nmmctl_scan_resp_timer_id = 
        son_start_timer(p_nmmctl_ctx->nmm_scan_config_param.scan_time_interval,
                        &p_nmmctl_ctx->nmmctl_scan_resp_timer_buf,
                        sizeof(p_nmmctl_ctx->nmmctl_scan_resp_timer_buf),
                        SON_FALSE);
    }

    SON_UT_TRACE_EXIT();
} /* nmmctl_fsm_start_scan_res */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_scan_complete_res
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when RRM_SON_NMM_COMPLETE_RESP is
 *                  received from RRM.
 ****************************************************************************/
static void
nmmctl_fsm_scan_complete_res
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    SON_UT_TRACE_ENTER();
    nmmctl_ctx_t               *p_nmmctl_ctx               = SON_PNULL;
    rrm_son_nmm_complete_res_t *p_scan_complete_res        = SON_PNULL;
    nmmctl_scan_queue_t        *p_scaning_queue            = SON_PNULL;
    nmmctl_scan_record_t       *p_scaning_record           = SON_PNULL;
    nmmctl_fsm_state_et        current_state               = NMMCTL_STATE_INIT;
    U8                         valid_scan_data             = SON_FALSE;
    smif_shutdown_res_t        shutdown_res_data;

    SON_UT_TRACE_ENTER();

    /* Get the NMM controller global context pointer */
    p_nmmctl_ctx = p_nmm_gl_ctx->p_nmmctl_ctx;
    p_scaning_queue = &(p_nmmctl_ctx->scaning_queue);

    /* Get current state of NMM controller */
    current_state = NMMCTL_FSM_GET_CURRENT_STATE(p_nmmctl_ctx);

    /* Get the response message received from RRM */
    p_scan_complete_res = (rrm_son_nmm_complete_res_t *)
    nmmctl_get_external_msg(p_api_buf,  RRM_SON_NMM_COMPLETE_RESP);

    if (p_scan_complete_res != SON_PNULL)
    {
        /* Check whether the message is received in NMMCTL_STATE_W_SHTDWN_RES state */
        if (current_state == NMMCTL_STATE_W_SHTDWN_RES)
        {
            SON_MEMSET(&shutdown_res_data, 0, sizeof(smif_shutdown_res_t));
            shutdown_res_data.transaction_id = p_shutdown_req->transaction_id;
            shutdown_res_data.result         = SON_SUCCESS;

            /* Send the shutdown response to SONMgmtIfH */
            son_create_send_buffer((U8 *)&shutdown_res_data,
                                   SON_NMM_MODULE_ID,
                                   SON_MIF_MODULE_ID,
                                   SMIF_SHUTDOWN_RES,
                                   sizeof(smif_shutdown_res_t));

            /* Free the memory reserved for shutdown data */
            son_mem_free(p_shutdown_req);
            p_shutdown_req = SON_PNULL;

            /* Set NMM controller FSM state to NMMCTL_STATE_INIT */
            NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_INIT);
        }
        else
        {
            if (p_running_start_scan != SON_PNULL)
            {
                /* Verify that, is the requested scan data same as last scanned data */
                if ((p_nmmctl_ctx->nmm_scan_config_param.force_scan == SON_FALSE) &&
                    ((g_last_scan_resp_data.scan_resp_data_valid == SON_TRUE) &&
                    (SON_TRUE == is_scan_and_scaned_data_same())))
                {
                    /* Send the scan response from the last save scaned result */
                    scan_response_from_scaned_data();
                }
                else
                {
                    /* Call the prepare and send message handler to send RRM prepare message */
                    nmmctl_prepare_send_external_msg((son_procedure_code_et)RRM_SON_NMM_PREPARE_REQ);
                    /* Change NMM controller state to NMMCTL_STATE_W_PREPARE_RES */
                    NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_PREPARE_RES);
                    valid_scan_data = SON_TRUE;
                }    
            }
            /* Coverity ID 72439 Starts */
            else
            {

                /* Get memory for running start scan */
                p_running_start_scan =
                    (nmmctl_start_scan_req_t *)son_mem_get(sizeof(nmmctl_start_scan_req_t));

                if (SON_PNULL == p_running_start_scan)
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                            "nmmctl_fsm_nmm_scan_req: Mem allocation fail");
                    /* Coverity ID 72438 Starts */
                    son_mem_free(p_scan_complete_res);
                    /* Coverity ID 72438 Ends */
                    return;
                }
            }
            /* Coverity ID 72439 Starts */
            while ((EMPTY_LIST != SCANING_Q_GET_COUNT(p_scaning_queue)) && 
                   (valid_scan_data == SON_FALSE))
            {
                /* Dequeue the start scan data */
                SCANING_Q_DEQUEUE(p_scaning_queue, p_scaning_record);

                /* Get memory for running start scan */
                /* Coverity ID 72439 Starts */
                if (SON_PNULL == p_scaning_record)
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                            "nmmctl_fsm_nmm_scan_req: Mem allocation fail");
                    /* Coverity ID 72438,72439 Starts */
                    son_mem_free(p_scan_complete_res);
                    son_mem_free(p_running_start_scan);
                    /* Coverity ID 72438, 72439 Ends */
                    /*SPR-12970-Start*/
                    return;
                    /*SPR-12970-End*/
                }
                else
                {
                    /* Copy the message to the gobal pointer */
                    SON_MEMCPY(p_running_start_scan,
                            &p_scaning_record->scan_data,
                            sizeof(nmmctl_start_scan_req_t));

                    /* Free the p_scaning_record */
                    son_mem_free(p_scaning_record);
                    p_scaning_record = SON_PNULL;

                    /* Verify that, is the requested scan data same as last scanned data */
                    if ((p_nmmctl_ctx->nmm_scan_config_param.force_scan == SON_FALSE) &&
                        ((g_last_scan_resp_data.scan_resp_data_valid == SON_TRUE) &&
                        (SON_TRUE == is_scan_and_scaned_data_same())))
                    {
                        /* Send the scan response from the saved last scan result */
                        scan_response_from_scaned_data();
                    }
                    else
                    {
                        /* Call the prepare and send message handler to send RRM prepare message */
                        nmmctl_prepare_send_external_msg((son_procedure_code_et ) RRM_SON_NMM_PREPARE_REQ);
                        /* Change NMM controller state to NMMCTL_STATE_W_PREPARE_RES */
                        NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_PREPARE_RES);
                        valid_scan_data = SON_TRUE;
                    }
                }    
            }

            if (valid_scan_data == SON_FALSE)
            {
                /* Change NMM controller state to NMMCTL_STATE_ACTIVE */
                NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_ACTIVE);
            }
        }

        son_mem_free(p_scan_complete_res);
    }

    SON_UT_TRACE_EXIT();
} /* nmmctl_fsm_scan_complete_res */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_stop_scan_req
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when SONNMM_STOP_SCAN_REQ message
 *                  is received to stop a scan request
 ****************************************************************************/
static void
nmmctl_fsm_stop_scan_req
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    STACKAPIHDR cspl_hdr = {0};
    nmmctl_ctx_t         *p_nmmctl_ctx            = SON_PNULL;
    nmmctl_scan_record_t *p_scaning_record        = SON_PNULL;
    nmmctl_scan_queue_t  *p_scaning_queue         = SON_PNULL;
    U16                  scan_data_compare_result = SON_NULL;
    U16                  running_stop_scan_id     = SON_NULL;
    U16                  req_stop_scan_id         = SON_NULL;
    son_nmm_stop_scan_res_t stop_scan_res_data;
    U8                   start_scan_found_for_stop_scan = SON_FALSE; 


    SON_UT_TRACE_ENTER();
    /* Parse the CSPL header from the received buffer */
    son_parse_cspl_header((U8 *)p_api_buf, &cspl_hdr);

    /* Get the NMM controller global context pointer */
    p_nmmctl_ctx    = p_nmm_gl_ctx->p_nmmctl_ctx;
    p_scaning_queue = &(p_nmmctl_ctx->scaning_queue);

    if (p_running_stop_scan == SON_PNULL)
    {
        /* Save the message received from SONAPPS/ANR
         * Get memory to save the received stop scan data */
        p_running_stop_scan = (nmmctl_stop_scan_req_t *)
                              son_mem_get(sizeof(nmmctl_stop_scan_req_t));

       if (SON_PNULL == p_running_stop_scan)
       {
           SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                   "nmmctl_fsm_stop_scan_req: Mem allocation fail");
           /*SPR-12970-Start*/
           return;
           /*SPR-12970-End*/
       }
       else
       {
            /* Parse CSPL HDR */
            son_parse_cspl_header((U8 *)p_api_buf, (STACKAPIHDR *) &(p_running_stop_scan->hdr));
            /* Copy the received scan data */
            SON_MEMCPY((void *) &(p_running_stop_scan->nmmctl_stop_scan_req),
                       (p_api_buf + SON_API_HEADER_SIZE),
                       sizeof(son_nmm_stop_scan_req_t));
        }
    }

    if (p_running_stop_scan != SON_PNULL)
    {
        /* Get the transaction ID of running stop scan */
        running_stop_scan_id = p_running_stop_scan->nmmctl_stop_scan_req.transaction_id;

        /* Get the transaction ID of the received stop scan */
        req_stop_scan_id = *(U16 *)(p_api_buf + SON_API_HEADER_SIZE);

        if (p_nmmctl_ctx->current_state != NMMCTL_STATE_W_STOP_RES)
        {
            if ((p_running_start_scan->nmmctl_start_scan_req.transaction_id) ==
                (running_stop_scan_id))
            {
                /* Call the stop scan handler */
                /*SPR 17777 +-*/
                nmmctl_stop_scan_handler( p_nmm_gl_ctx);
                /* Set the start scan found for corresponsing stop scan flag */
                start_scan_found_for_stop_scan = SON_TRUE;
               
            }
        }
        else
        {
            scan_data_compare_result = nmmctl_scan_data_compare(&req_stop_scan_id,
                                                            &running_stop_scan_id);

            if (scan_data_compare_result == SON_NULL)
            {
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "nmmctl_fsm_stop_scan_req: Stop scan request with Transction ID %d"
                    "is already in progress", req_stop_scan_id);

                /* Prepare the stop scan response data */
                stop_scan_res_data.transaction_id         = req_stop_scan_id;
                stop_scan_res_data.stop_scan_res.son_resp = SON_FAILURE;
                stop_scan_res_data.stop_scan_res.son_error = SON_ERR_NMM_STOP_SCAN_RUNNING;

                /* Send the stop Scan response to SONApps/ANR */
                son_create_send_buffer((U8 *)&stop_scan_res_data,
                                       SON_NMM_MODULE_ID,
                                        (son_module_id_et)cspl_hdr.from,
                                       SONNMM_STOP_SCAN_RES,
                                       sizeof(son_nmm_stop_scan_res_t));

                /* Set the start scan found for corresponsing stop scan flag */
                start_scan_found_for_stop_scan = SON_TRUE;
            }
        }
    }

    /* If stop scan request not corresponds with the running start scan,
     * find it in pending queue */
    if (start_scan_found_for_stop_scan == SON_FALSE)
    {
        /* 
        ** Find the respective start scan request in the NMM controller
        ** local scan queue.
        */
        p_scaning_record = (nmmctl_scan_record_t *)
                           ylFind(p_scaning_queue,
                                  &req_stop_scan_id, 
                                  nmmctl_curr_scan_object_id_keyof, 
                                  nmmctl_scan_data_compare);

        if (p_scaning_record != SON_PNULL)
        {
            /* 
            ** Delete the start scan request from the NMM controller
            ** local scan queue
            */
            ylDelete(p_scaning_queue,
                     (YLNODE *)p_scaning_record);

            /* Free the memory allocated to store the start scan request */
            son_mem_free(p_scaning_record);

            /* Prepare the stop scan response data */
            stop_scan_res_data.transaction_id         = req_stop_scan_id;
            stop_scan_res_data.stop_scan_res.son_resp = SON_SUCCESS;

            /* Send the stop Scan response to SONApps/ANR */
            son_create_send_buffer((U8 *)&stop_scan_res_data,
                                   SON_NMM_MODULE_ID,
                                   (son_module_id_et)cspl_hdr.from,
                                   SONNMM_STOP_SCAN_RES,
                                   sizeof(son_nmm_stop_scan_res_t));
        }
        else
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "nmmctl_fsm_stop_scan_req: Start Scan request"
                    "not found for respective Stop Scan");

            /* Prepare the stop scan response */
            stop_scan_res_data.transaction_id         = req_stop_scan_id;
            stop_scan_res_data.stop_scan_res.son_resp = SON_FAILURE;
            stop_scan_res_data.stop_scan_res.son_error = SON_ERR_NMM_START_SCAN_NOT_FOUND;

            /* Send the stop Scan response to SONApps/ANR */
            son_create_send_buffer((U8 *)&stop_scan_res_data,
                                   SON_NMM_MODULE_ID,
                                   (son_module_id_et)cspl_hdr.from,
                                   SONNMM_STOP_SCAN_RES,
                                   sizeof(son_nmm_stop_scan_res_t));
        }

        /* Free the running stop scan pointer */
        if (p_nmmctl_ctx->current_state != NMMCTL_STATE_W_STOP_RES)
        {
            son_mem_free(p_running_stop_scan);
            p_running_stop_scan = SON_PNULL;
        }
    }

    /* Coverity ID 63017 Starts */
    if(SON_PNULL != p_running_stop_scan)
    {
        son_mem_free(p_running_stop_scan);
        p_running_stop_scan = SON_PNULL;
    }
    /* Coverity ID 63017 Ends */
    SON_UT_TRACE_EXIT();
} /* nmmctl_fsm_stop_scan_req */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_shutdown_req
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when SMIF_SHUTDOWN_REQ message is
 *                  received from MIF.
 ****************************************************************************/
static void
nmmctl_fsm_shutdown_req
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    nmmctl_ctx_t             *p_nmmctl_ctx          = SON_PNULL;
    nmmctl_scan_queue_t      *p_scaning_queue       = SON_PNULL;
    nmmctl_scan_record_t     *p_scaning_record      = SON_PNULL;
    nmmctl_fsm_state_et      current_state          = NMMCTL_STATE_INIT;
    smif_shutdown_res_t      shutdown_res_data;


    SON_UT_TRACE_ENTER();

    /* Get the NMM controller global context pointer */
    p_nmmctl_ctx = p_nmm_gl_ctx->p_nmmctl_ctx;
    p_scaning_queue = &(p_nmmctl_ctx->scaning_queue);

    /* Get current state of NMM controller */
    current_state = NMMCTL_FSM_GET_CURRENT_STATE(p_nmmctl_ctx);

    /* Free the saved scan datas */
    if (g_last_scan_resp_data.scan_resp_data_valid == SON_TRUE)
    {
        /* Stop the Timer */
        son_stop_timer(g_nmmctl_scan_resp_timer_id);
        /* Start: CSR 00070093 */
        g_nmmctl_scan_resp_timer_id = SON_PNULL;
        /*End: CSR 00070093 */

        nmmctl_fsm_free_last_scan_data();
    }    

    if (p_periodic_scan_data != SON_PNULL)
    {
        /* Stop the Timer */
        son_stop_timer(g_nmmctl_periodic_scan_timer_id);
        /* Start: CSR 00070093 */
        g_nmmctl_periodic_scan_timer_id = SON_PNULL;
        /*End: CSR 00070093 */

    /* Free the periodic scan data */
    son_mem_free(p_periodic_scan_data->nmmctl_periodic_nw_scan_req.p_start_scan_req);
    p_periodic_scan_data->nmmctl_periodic_nw_scan_req.p_start_scan_req = SON_PNULL;
    son_mem_free(p_periodic_scan_data);
    p_periodic_scan_data = SON_PNULL;
    }    
    
    /* 
    ** If current state is NMMCTL_STATE_INIT/NMMCTL_STATE_ACTIVE,
    ** response imediatly as we are not doing any thing.
    */
    if ((current_state == NMMCTL_STATE_INIT) ||
        (current_state == NMMCTL_STATE_ACTIVE))
    {
        SON_MEMSET(&shutdown_res_data, 0, sizeof(smif_shutdown_res_t));
        shutdown_res_data.transaction_id = *((U16 *)(p_api_buf + SON_API_HEADER_SIZE));
        shutdown_res_data.result         = SON_SUCCESS;

        /* Send the shutdown response to SONMgmtIfH */
        son_create_send_buffer((U8 *)&shutdown_res_data,
                               SON_NMM_MODULE_ID,
                               SON_MIF_MODULE_ID,
                               SMIF_SHUTDOWN_RES,
                               sizeof(smif_shutdown_res_t));
        /* Move to NMMCTL_STATE_INIT if current state is NMMCTL_STATE_ACTIVE */
        if (current_state == NMMCTL_STATE_ACTIVE)
        {
             NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_INIT);
        }
    }
    else
    {
        /* Get memory to save the start scan data */
        p_shutdown_req = (smif_shutdown_req_t *)son_mem_get(sizeof(smif_shutdown_req_t));
        if (SON_PNULL == p_shutdown_req)
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "nmmctl_fsm_shutdown_req: Mem allocation fail");
            /*SPR-12970-Start*/
            return;
            /*SPR-12970-End*/
        }
        else
        {
            p_shutdown_req->transaction_id =  
                                         *((U16 *)(p_api_buf + SON_API_HEADER_SIZE));

            /*
            ** Send the shutdown response to SONMgmtIfH with respect to
            ** the current state of NMM controller.
            */
            switch (current_state)
            {
                /* State is NMMCTL_STATE_W_PREPARE_RES */
                case NMMCTL_STATE_W_PREPARE_RES:
                {
                    /* Clean running scan pointer and scan queue */
                    if (p_running_start_scan != SON_PNULL)
                    {
                        son_mem_free(p_running_start_scan);
                        p_running_start_scan = SON_PNULL;
                    }

                    while (EMPTY_LIST != SCANING_Q_GET_COUNT(p_scaning_queue))
                    {
                        /* Dequeue the start scan data */
                        SCANING_Q_DEQUEUE(p_scaning_queue, p_scaning_record);
                        /* Free the memory */
                        son_mem_free(p_scaning_record);
                        p_scaning_record = SON_PNULL;
                    }

                    /* Wait for the RRM response
                     * Change the state to NMMCTL_STATE_W_SHTDWN_RES */
                    NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_SHTDWN_RES);
                    break;
                }

                /* State is NMMCTL_STATE_W_SCAN_RES */
                case NMMCTL_STATE_W_SCAN_RES:
                {
                    /*
                    **  Prepare and send stopscan request to NMM:
                    **  1) Send stop scan request to NMM.
                    **  2) Remove all entries from scan queue.
                    **  3) Change the state to NMMCTL_STATE_W_SHTDWN_RES.
                    */
                    son_create_send_buffer(SON_PNULL,
                                           SON_NMM_MODULE_ID,
                                           SON_NMM_MODULE_ID,
                                           SONNMM_STOP_SCAN_REQ,
                                           SON_NULL);
                    if (p_running_start_scan != SON_PNULL)
                    {
                        /* Clean running scan pointer and scan queue */
                        son_mem_free(p_running_start_scan);
                        p_running_start_scan = SON_PNULL;
                    }

                    while (EMPTY_LIST != SCANING_Q_GET_COUNT(p_scaning_queue))
                    {
                        /* Dequeue the start scan data */
                        SCANING_Q_DEQUEUE(p_scaning_queue, p_scaning_record);
                        /* Free the memory */
                        son_mem_free(p_scaning_record);
                        p_scaning_record = SON_PNULL;
                    }

                    /* Wait for the RRM response
                     * Change the state to NMMCTL_STATE_W_SHTDWN_RES */
                    NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_SHTDWN_RES);
                    break;
                }

                /* State is NMMCTL_STATE_W_COMPLT_RES */
                case NMMCTL_STATE_W_COMPLT_RES:
                {
                    /* Clean running scan pointer and scan queue */
                    if (p_running_start_scan != SON_PNULL)
                    {
                        son_mem_free(p_running_start_scan);
                        p_running_start_scan = SON_PNULL;
                    }

                    while (EMPTY_LIST != SCANING_Q_GET_COUNT(p_scaning_queue))
                    {
                        /* Dequeue the start scan data */
                        SCANING_Q_DEQUEUE(p_scaning_queue, p_scaning_record);
                        /* Free the memory */
                        son_mem_free(p_scaning_record);
                        p_scaning_record = SON_PNULL;
                    }

                    /* Wait for the RRM response
                     * Change the state to NMMCTL_STATE_W_SHTDWN_RES */
                    NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_SHTDWN_RES);
                    break;
                }

                /* State is NMMCTL_STATE_W_STOP_RES */
                case NMMCTL_STATE_W_STOP_RES:
                {
                    /* Clean running scan pointer and scan queue */
                    if (p_running_start_scan != SON_PNULL)
                    {
                        son_mem_free(p_running_start_scan);
                        p_running_start_scan = SON_PNULL;
                    }

                    while (EMPTY_LIST != SCANING_Q_GET_COUNT(p_scaning_queue))
                    {
                        /* Dequeue the start scan data */
                        SCANING_Q_DEQUEUE(p_scaning_queue, p_scaning_record);
                        /* Free the memory */
                        son_mem_free(p_scaning_record);
                        p_scaning_record = SON_PNULL;
                    }

                    /* Wait for the RRM response
                     * Change the state to NMMCTL_STATE_W_SHTDWN_RES */
                    NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_SHTDWN_RES);
                    break;
                }

                 default:
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "nmmctl_fsm_shutdown_req: unexpected state %u", current_state);
                    break;
            }
            } /* switch */
    }
    }

    SON_UT_TRACE_EXIT();
} /* nmmctl_fsm_shutdown_req */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_stop_scan_res
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when SONNMM_STOP_SCAN_RES message is
 *                  received from NMM.
 ****************************************************************************/
static void
nmmctl_fsm_stop_scan_res
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    STACKAPIHDR cspl_hdr = {0};
    nmmctl_ctx_t         *p_nmmctl_ctx          = SON_PNULL;
    nmmctl_scan_record_t *p_scaning_record      = SON_PNULL;
    U32                  msg_size               = SON_NULL;
    nmmctl_fsm_state_et  current_state          = NMMCTL_MAX_STATE;
    son_module_id_et     origin_module_id       = SON_ALL_INT_MODULES;
    nmmctl_scan_queue_t  *p_scaning_queue       = SON_PNULL;
    U8                   valid_scan_data        = SON_FALSE;
    son_nmm_stop_scan_res_t stop_scan_res_data;
    nmm_start_scan_data_t     start_scan_data;


    SON_UT_TRACE_ENTER();

    /* Parse the CSPL header */
    son_parse_cspl_header((U8 *)p_api_buf, &cspl_hdr);

    /* Get the NMM controller global context pointer */
    p_nmmctl_ctx = p_nmm_gl_ctx->p_nmmctl_ctx;
    p_scaning_queue = &(p_nmmctl_ctx->scaning_queue);
    /* Get current state of NMM controller */
    current_state = NMMCTL_FSM_GET_CURRENT_STATE(p_nmmctl_ctx);

    if (current_state == NMMCTL_STATE_W_SHTDWN_RES)
    {
        /* Free the running stop scan pointer */
        if (p_running_stop_scan != SON_PNULL)
        {
            son_mem_free(p_running_stop_scan);
            p_running_stop_scan = SON_PNULL;
        }

        /* 
         ** Call the prepare and send message handler
         ** to send the Scan complete message to RRM
         */
        nmmctl_prepare_send_external_msg((son_procedure_code_et)RRM_SON_NMM_COMPLETE_REQ);
    }
    else
    {
        /* Get the module ID of the requester, requested the stop scan */
        origin_module_id = (son_module_id_et)p_running_stop_scan->hdr.from;

        /* prepare stop scan response data */
        stop_scan_res_data.transaction_id =
            p_running_stop_scan->nmmctl_stop_scan_req.transaction_id;
        SON_MEMCPY(&stop_scan_res_data.stop_scan_res, 
                (p_api_buf + SON_API_HEADER_SIZE),
                sizeof(stop_scan_res_t));

        /* Send the stop scan response */
        son_create_send_buffer((U8 *)&stop_scan_res_data, SON_NMM_MODULE_ID,
                origin_module_id, SONNMM_STOP_SCAN_RES,
                sizeof(son_nmm_stop_scan_res_t));

        /* Free the running stop scan pointer */
        son_mem_free(p_running_stop_scan);
        p_running_stop_scan = SON_PNULL;

        /* 
         ** Check whether any pending scan message is present in
         ** the NMM controller local queue
         */
        if (p_running_start_scan == SON_NULL)
        {    
            while ((EMPTY_LIST != SCANING_Q_GET_COUNT(p_scaning_queue))
                    && (valid_scan_data == SON_FALSE))
            {
                /*Coverity 94774 Fix Start*/
                /* Get memory for running start scan */
                p_running_start_scan =
                    (nmmctl_start_scan_req_t *)son_mem_get(sizeof(nmmctl_start_scan_req_t));

                if (SON_PNULL == p_running_start_scan)
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                            "nmmctl_fsm_nmm_scan_req: Mem allocation fail");
                    /*SPR-12970-Start*/
                    return;
                    /*SPR-12970-End*/
                }
                /*Coverity 94774 Fix End*/
                /* Dequeue the start scan data */
                SCANING_Q_DEQUEUE(p_scaning_queue, p_scaning_record);

                /*Coverity ID 72730 Starts */
                /* Get memory for running start scan */
                if (SON_PNULL == p_scaning_record)
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                            "nmmctl_fsm_nmm_scan_req: Mem allocation fail");
                    son_mem_free(p_running_start_scan);
                    return;
                }
                /*Coverity ID 72730 Ends */
                /* Copy the message to the gobal pointer */
                SON_MEMCPY(p_running_start_scan, 
                        &p_scaning_record->scan_data,
                        sizeof(nmmctl_start_scan_req_t));

                /* Free the p_scaning_record */
                son_mem_free(p_scaning_record);
                p_scaning_record = SON_PNULL;

                /* Verify that, is the requested scan data same as last scanned data */
                if ((p_nmmctl_ctx->nmm_scan_config_param.force_scan == SON_FALSE) &&
                        ((g_last_scan_resp_data.scan_resp_data_valid == SON_TRUE) &&
                         (SON_TRUE == is_scan_and_scaned_data_same())))
                {
                    /* Send the scan response from the saved last scan result */
                    scan_response_from_scaned_data();
                    /*SPR 15935 Fix Start*/
                    son_mem_free(p_running_start_scan);
                    p_running_start_scan = SON_PNULL;
                    /*SPR 15935 Fix Stop*/
                }
                else
                {    
                    /* Send SCAN request to NMM */
                    msg_size = 
                        GET_PARAMLEN_FROM_CSPL_HDR(p_running_start_scan) - TRANSACTION_ID_SIZE;
                    start_scan_data.p_start_scan_req = p_running_start_scan->
                        nmmctl_start_scan_req.p_start_scan_req;
                    son_create_send_buffer((U8 *)&start_scan_data,
                            SON_NMM_MODULE_ID,
                            SON_NMM_MODULE_ID,
                            SONNMM_START_SCAN_REQ,
                            msg_size);

                    /* set NMM controller FSM state to NMMCTL_STATE_W_SCAN_RES */
                    NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_SCAN_RES);

                    valid_scan_data = SON_TRUE;
                }
            }

            if ((EMPTY_LIST == SCANING_Q_GET_COUNT(p_scaning_queue)) && (SON_FALSE == valid_scan_data))
            {
                /*
                 ** Call the prepare and send message handler
                 ** to send the Scan complete message to RRM
                 */
                nmmctl_prepare_send_external_msg((son_procedure_code_et)RRM_SON_NMM_COMPLETE_REQ);

                /* Change NMM controller state to NMMCTL_STATE_W_COMPLT_RES */
                NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_COMPLT_RES);
            }
        }
    }

    SON_UT_TRACE_EXIT();
} /* nmmctl_fsm_stop_scan_res */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_invalid_event_handler
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This is the invalid event handler.It will be invocked
 *                  whenever an unexpected event is received with respect
 *                  to the current state of the FSM.
 ****************************************************************************/
static void
nmmctl_fsm_invalid_event_handler
(
 void            *p_api_buf,
 nmm_gl_ctx_t    *p_nmm_gl_ctx
 )
{
    SON_UT_TRACE_ENTER();

    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
            "nmmctl_fsm_invalid_event_handler: unexpected event received with respect to"
    /*SPR 17777 +-*/
            "the current NMM controller state %u %u",sizeof(p_api_buf),sizeof(p_nmm_gl_ctx));

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : nmmctl_stop_scan_handler
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is the handler to stop a ongoing scan request.
 ****************************************************************************/
static void
nmmctl_stop_scan_handler
(
    /*SPR 17777 +-*/
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    nmmctl_ctx_t        *p_nmmctl_ctx = SON_PNULL;
    nmmctl_fsm_state_et current_state = NMMCTL_MAX_STATE;
    son_nmm_stop_scan_res_t stop_scan_res_data;
    nmmctl_scan_queue_t  *p_scaning_queue        = SON_PNULL;
     nmmctl_scan_record_t *p_scaning_record = SON_PNULL;


    SON_UT_TRACE_ENTER();

    /* Get the NMM controller global context pointer */
    p_nmmctl_ctx = p_nmm_gl_ctx->p_nmmctl_ctx;
    p_scaning_queue = &(p_nmmctl_ctx->scaning_queue);
    /* Get current state of NMM controller */
    current_state = NMMCTL_FSM_GET_CURRENT_STATE(p_nmmctl_ctx);

    /* 
    ** Send the stop scan req to NMM with respect to 
    ** the current state of NMM controller.
    */
    switch (current_state)
    {
    /* 
    ** If current state is NMMCTL_STATE_ACTIVE, NMMCTL_STATE_W_STOP_RES,
    ** NMMCTL_STATE_W_SHTDWN_RES, invalid handler will be called from FSM
    */

    /* Waiting for RRM prepare response message */
    case NMMCTL_STATE_W_PREPARE_RES:
        {
         /* prepare and send stop scan response */
         stop_scan_res_data.transaction_id = p_running_stop_scan->
                                             nmmctl_stop_scan_req.transaction_id;
         stop_scan_res_data.stop_scan_res.son_resp  = SON_SUCCESS;

         /*
         ** Call the prepare and send message handler
         ** to send the stop Scan response to SONApps/ANR.
         */
            son_create_send_buffer((U8 *)&stop_scan_res_data,
                                SON_NMM_MODULE_ID,
                                (son_module_id_et)p_running_stop_scan->hdr.from,
                                SONNMM_STOP_SCAN_RES,
                                sizeof(son_nmm_stop_scan_res_t));

        /* Free the running start/stop scan pointer */
        son_mem_free(p_running_stop_scan);
        p_running_stop_scan = SON_PNULL;

        /* Read queue head and add its entry to running_scan pointer */
            if (EMPTY_LIST != SCANING_Q_GET_COUNT(p_scaning_queue))
        {
            /* Dequeue the start scan data */
            SCANING_Q_DEQUEUE(p_scaning_queue, p_scaning_record);
                if (SON_PNULL != p_scaning_record)
            {
                /* Copy the message to the gobal pointer */
                SON_MEMCPY(p_running_start_scan, 
                       &p_scaning_record->scan_data,
                       sizeof(nmmctl_start_scan_req_t));
            }

            /* Free the p_scaning_record */
            son_mem_free(p_scaning_record);
        }
        else
        {
            son_mem_free(p_running_start_scan);
            p_running_start_scan = SON_PNULL;
            }

        break;
        }

    /* Waiting for scan response from NMM */
    case NMMCTL_STATE_W_SCAN_RES:
        {
        /*
        **  1) Send stopscan request to NMM.
        **  2) Change NMM controller state to NMMCTL_STATE_W_STOP_RES.
        */
            son_create_send_buffer((U8 *)SON_PNULL,
                               SON_NMM_MODULE_ID,
                               SON_NMM_MODULE_ID,
                               SONNMM_STOP_SCAN_REQ,
                               SON_NULL);

        /* Change the state to NMMCTL_STATE_W_STOP_RES */ 
        NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_STOP_RES);

        /* Free running start scan pointer */
        son_mem_free(p_running_start_scan);
        p_running_start_scan = SON_PNULL;
        break;
        }

       
        /* Waiting for scan complete response from RRM */
    case NMMCTL_STATE_W_COMPLT_RES:
        {
            /* Wait for the RRM response
             * Change the state to NMMCTL_STATE_W_STOP_RES */
            NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_W_STOP_RES);
            break;
        }

        default:
        {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                "nmmctl_stop_scan_handler: unexpected state %u", current_state);
        break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
} /* nmmctl_stop_scan_handler */

/*****************************************************************************
 * Function Name  : nmmctl_scan_data_compare
 * Inputs         : p_key1 - Pointer to the data1 to be compaired
 *                  p_key2 - Pointer to the data2 to be compaired
 * Outputs        : None
 * Returns        : 0 if data1 is same as data2
 *                  1 if data1 is not same as data2
 * Description    : This function compares the key passed to retrieve correct
 *                  node from linked list.
 ****************************************************************************/
static int
nmmctl_scan_data_compare
(
    const void *p_key1, 
    const void *p_key2
)
{
    if (*(U16 *)p_key1 == *(U16 *)p_key2)
    {
        return (0);
    }
    else
    {
        return (1);
    }
}

/*****************************************************************************
 * Function Name  : nmmctl_curr_scan_object_id_keyof
 * Inputs         : p_ylnode - Pointer to scan data.
 * Outputs        : None
 * Returns        : transaction id
 * Description    : This function is the keyof function for the nmmctl_scan_record_t
 *                  linked list.
 ****************************************************************************/
static const void *
nmmctl_curr_scan_object_id_keyof
(
    const YLNODE *p_ylnode
)
{
    return &(((nmmctl_scan_record_t *)p_ylnode)->
                scan_data.nmmctl_start_scan_req.transaction_id);
}

/*****************************************************************************
 * Function Name  : nmmctl_convert_internal_apitoevent
 * Inputs         : son_api_id - SON message ID
 * Outputs        : None
 * Returns        : NMM controller event ID.
 * Description    : This function converts the SON api to NMM controller event ID.
 ****************************************************************************/
static nmmctl_event_code_et
nmmctl_convert_internal_apitoevent
(
     son_procedure_code_et son_api_id
)
{
    nmmctl_event_code_et event = NMMCTL_MAX_EVENT;


    SON_UT_TRACE_ENTER();

    /* Convert the son api to nmm controller event ID */
    switch (son_api_id)
    {
        /* Init configure request has been received from MIF */
        case SMIF_INIT_CONFIG_REQ:
            {
                event = NMMCTL_SMIF_INIT_CONFIG_REQ_EV;
                break;
            }

            /* Log enable/disable request has been received from MIF */
        case SMIF_LOG_ENABLE_DISABLE_CMD:
            {
                event = NMMCTL_SMIF_LOG_ENABLE_DISABLE_CMD_EV;
                break;
            }

            /* Set log level request has been received from MIF */
        case SMIF_SET_LOG_LEVEL_CMD:
            {
                event = NMMCTL_SMIF_SET_LOG_LEVEL_CMD_EV;
                break;
            }

            /* Start scan request has been received from SON apps/ANR */
        case SONNMM_START_SCAN_REQ:
            {
                event = NMMCTL_START_SCAN_REQ_EV; 
                break;
            }

            /* Start scan response has been received from NMM */
        case SONNMM_START_SCAN_RES:
            {
                event = NMMCTL_START_SCAN_RES_EV; 
                break;
            }

            /* Stop scan request has been received from SON apps/ANR */
        case SONNMM_STOP_SCAN_REQ:
            {
                event = NMMCTL_STOP_SCAN_REQ_EV;
                break;
            }

        case SONNMM_PERIODIC_SCAN_REQ:
            {
                event = NMMCTL_PERIODIC_SCAN_REQ_EV;
                break;
            }

            /* Stop scan response has been received from NMM */
        case SONNMM_STOP_SCAN_RES:
            {
                event = NMMCTL_STOP_SCAN_RES_EV;
                break;
            }

        case SMIF_SET_SCAN_CONFIG_CMD:
            {
                event = NMMCTL_SMIF_SET_NW_SCAN_CONFIG_CMD_EV;
                break;
            }

            /* Shutdown request has been received from SON management interface */
        case SMIF_SHUTDOWN_REQ:
            {
                event = NMMCTL_SMIF_SHUTDOWN_REQ_EV;
                break;
            }

            /* SPR-13251 Fix Starts */
        case SMIF_GET_LOG_LEVEL_REQ:
            {
                event = NMMCTL_SMIF_GET_LOG_LEVEL_REQ_EV;
                break;
            }
            /* SPR-13251 Fix Ends */
        default:
            {
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                        "nmmctl_convert_internal_apitoevent: unexpected API %d", son_api_id);
                break;
            }
    } /* switch */

    SON_UT_TRACE_EXIT();

    return event;
} /* nmmctl_convert_internal_apitoevent */

/*****************************************************************************
 * Function Name  : nmmctl_convert_external_apitoevent
 * Inputs         : son_api_id - SON message ID
 * Outputs        : None
 * Returns        : NMM controller event ID.
 * Description    : This function converts the SON api to NMM controller event ID.
 ****************************************************************************/
static nmmctl_event_code_et
nmmctl_convert_external_apitoevent
(
     rrm_son_message_resp_et son_api_id
)
{
    nmmctl_event_code_et event = NMMCTL_MAX_EVENT;


    SON_UT_TRACE_ENTER();

    /* Convert the son api to nmm controller event ID */
    switch (son_api_id)
    {
    /* Scan prepare response has been received from RRM */
    case RRM_SON_NMM_PREPARE_RESP:
        {
        event = NMMCTL_SCAN_PREPARE_RES_EV;
        break;
        }

    /* Scan complete response has been received from RRM */
    case RRM_SON_NMM_COMPLETE_RESP:
        {
        event = NMMCTL_SCAN_COMPLETE_RES_EV;
        break;
        }

    default:
        {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                "nmmctl_convert_external_apitoevent: unexpected API %d", son_api_id);
        break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();

    return event;
} /* nmmctl_convert_external_apitoevent */

/*****************************************************************************
 * Function Name  : nmmctl_prepare_send_external_msg
 * Inputs         : api_id - procedure ID
 * Outputs        : None
 * Returns        : None
 * Description    : This function will prepare the message and send it to the
 *                  external module(RRM).
 ****************************************************************************/
static void
nmmctl_prepare_send_external_msg
(
 son_procedure_code_et api_id 
 )
{
    U16 msg_length = SON_NULL;
    U16 msg_api_length = SON_NULL;
    U8     *p_msg          = SON_PNULL;
    U8     *p_son_msg      = SON_PNULL;


    msg_api_length =  SON_API_HEADER_SIZE + EXT_MSG_API_HDR_SIZE;

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length);

    if (p_msg == SON_PNULL)
    {
        /* Not enough memory */
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                "nmmctl_prepare_send_external_msg: Mem allocation fail");
    }
    else
    {
        SON_MEMSET(p_msg, 0, msg_api_length);
        p_son_msg = p_msg;

        /* Fill CSPL header */
        son_construct_cspl_header(p_son_msg, SON_VERSION_ID, SON_NMM_MODULE_ID,
            SON_RRM_MODULE_ID, api_id, SON_NULL, msg_api_length);

        /* Fill interface header */
        p_son_msg = p_son_msg + SON_API_HEADER_SIZE;
        son_construct_interface_api_header(p_son_msg, SON_NULL, SON_NMM_MODULE_ID,
               SON_RRM_MODULE_ID, api_id, msg_length, SON_NULL);

        /* Send message to RRM*/
        son_send_msg(p_msg, SON_RRM_MODULE_ID);
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF,
                "nmmctl_prepare_send_external_msg: Api ID %d sent to RRM", api_id);
    }

    return;
} /* nmmctl_prepare_send_external_msg */

/*****************************************************************************
 * Function Name  : nmmctl_get_external_msg
 * Inputs         : p_api_buf - pointer to received buffer
 *                  api_id    - message ID
 * Outputs        : None
 * Returns        : Pointer to the data part of the received message
 * Description    : This function will fetch the data part of the message
 *                  received from external entity.
 ****************************************************************************/
static void *
nmmctl_get_external_msg
(
    U8                   *p_api_buf,
    rrm_son_message_resp_et api_id
)
{
    S32 length_read = SON_NULL;
    S32 payload_len = son_ntohl(*((S32 *)(p_api_buf + 
                    SON_API_HEADER_SIZE + 4)));
    U8  *p_src_data = p_api_buf + SON_API_HEADER_SIZE + EXT_MSG_API_HDR_SIZE;
    void  *p_return = SON_PNULL;


    /* Scan prepare response has been received RRM */
    if (api_id == RRM_SON_NMM_PREPARE_RESP)
    {
        rrm_son_nmm_prepare_res_t *p_scan_prepare_res = 
            (rrm_son_nmm_prepare_res_t *)son_mem_get(sizeof(rrm_son_nmm_prepare_res_t));

        if (SON_PNULL == p_scan_prepare_res)
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "nmmctl_get_external_msg: Mem allocation fail");
            /*SPR-12970-Start*/
            return SON_PNULL;
            /*SPR-12970-End*/
        }
        else
        {
            /* Fetch the scan prepare response data received from RRM */
            rrm_parse_rrm_son_nmm_prepare_res(p_scan_prepare_res,
                                              p_src_data,
                                              payload_len,
                                              &length_read);
            p_return = (void *)p_scan_prepare_res;
        }
    }
    /* Scan complete response has been received from RRM */
    else
    {
        rrm_son_nmm_complete_res_t *p_scan_complete_res =
            (rrm_son_nmm_complete_res_t *)son_mem_get(sizeof(rrm_son_nmm_complete_res_t));

        if (SON_PNULL == p_scan_complete_res)
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                    "nmmctl_get_external_msg: Mem allocation fail");
            /*SPR-12970-Start*/
            return SON_PNULL;
            /*SPR-12970-End*/
        }
        else
        {
            /* Fetch the scan complete response data received from RRM */
            rrm_parse_rrm_son_nmm_complete_res(p_scan_complete_res,
                                               p_src_data,
                                               payload_len,
                                               &length_read);
            p_return = (void *)p_scan_complete_res;
        }
    }

    return p_return;
} /* nmmctl_get_external_msg */

/*****************************************************************************
 * Function Name  : scan_response_from_scaned_data
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function will send the network scan response by reading
 *                  the last scan response.
 ****************************************************************************/
static void
scan_response_from_scaned_data
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    son_module_id_et origin_module_id  =  SON_ALL_INT_MODULES;
    son_nmm_start_scan_res_t start_scan_res_data;


    SON_UT_TRACE_ENTER();

    /* Get the module ID of the requester, requested the start scan */
    origin_module_id = (son_module_id_et)p_running_start_scan->hdr.from;
    start_scan_res_data.transaction_id =
        p_running_start_scan->nmmctl_start_scan_req.transaction_id;
    start_scan_res_data.originator_module_id =
        p_running_start_scan->nmmctl_start_scan_req.originator_module_id;

    /* Copy the scan resp data */
    start_scan_res_data.start_scan_res = g_last_scan_resp_data.start_scan_res_data;

    /* Send the scan response */
    son_create_send_buffer((U8 *)&start_scan_res_data,
            SON_NMM_MODULE_ID,
            origin_module_id,
            SONNMM_START_SCAN_RES,
            sizeof(son_nmm_start_scan_res_t));

    SON_UT_TRACE_EXIT();
} /* scan_response_from_scaned_data */

/*****************************************************************************
 * Function Name  : is_scan_and_scaned_data_same
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_TRUE/SON_FALSE 
 * Description    : This function will check the current requested scanned data with 
 *                  previously requested scanned data. 
 ****************************************************************************/
static U8
is_scan_and_scaned_data_same
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    start_scan_req_t *p_current_scan_data = SON_PNULL;
    U8               is_data_same         = SON_TRUE;
    son_s32          compare_result       = SON_NULL;
    son_s32          index                = SON_NULL ;
    son_s32          e_index              = SON_NULL;


    SON_UT_TRACE_ENTER();

    /* Fetch the data to be compaired */
    p_current_scan_data = p_running_start_scan->nmmctl_start_scan_req.p_start_scan_req;

    /* Compair the current scan data with last scan data */
    if ((p_current_scan_data->rat_type != p_last_scan_data->rat_type) ||
            (p_current_scan_data->freq_band_id != p_last_scan_data->freq_band_id) ||
            (p_current_scan_data->num_earfcn != p_last_scan_data->num_earfcn))
    {
        is_data_same = SON_FALSE;
    }

    if (is_data_same != SON_FALSE)
    {
        if (p_current_scan_data->is_meas_bandwidth_valid == p_last_scan_data->is_meas_bandwidth_valid)
        {
            if (p_current_scan_data->is_meas_bandwidth_valid == SON_TRUE)
            {
                if (p_current_scan_data->meas_bandwidth != p_last_scan_data->meas_bandwidth)
                {
                    is_data_same = SON_FALSE;
                }
            }
            else
            {
                /* SPR NO:10540 changes start */
                for(index =0 ; index < p_current_scan_data->num_earfcn;index ++)
                {

                    if (p_current_scan_data->earfcn_pci_info[index]. meas_bandwidth_per_earfcn== 
                            p_last_scan_data->earfcn_pci_info[index].meas_bandwidth_per_earfcn)
                    {
                        compare_result++;
                    }
                    else
                    {
                        compare_result = SON_NULL;
                        break;
                    }
                }

                if (compare_result != SON_NULL)
                {
                    is_data_same = SON_TRUE;
                }
            }
        }
        else
        {
            is_data_same = SON_FALSE;
        }
    }

    if (is_data_same != SON_FALSE)
    {
        for(index =0 ; index < p_current_scan_data->num_earfcn;index ++)
        {

            if (p_current_scan_data->earfcn_pci_info[index].earfcn == p_last_scan_data->earfcn_pci_info[index].earfcn)
            {
                compare_result++;
            }
            else
            {
                compare_result = SON_NULL;
                break;
            }
        }
        if (compare_result != SON_NULL)
        {
            is_data_same = SON_TRUE;
        }
    }

    if (is_data_same != SON_FALSE)
    {
        /* Start: CSR 00053250 */
        for(e_index =0 ;e_index <p_current_scan_data->num_earfcn;e_index++)
        {
            for(index =0 ; index < p_current_scan_data->earfcn_pci_info[e_index].num_pci;index ++)
            {

                if (p_current_scan_data->earfcn_pci_info[e_index].pci[index] == 
                        p_last_scan_data->earfcn_pci_info[e_index].pci[index])
                {
                    compare_result++;
                }
                else
                {
                    compare_result = SON_NULL;
                    break;
                }
            }
        }
        if (compare_result != SON_NULL)
        {
            is_data_same = SON_TRUE;
        }
    }




    /*End: CSR 00053250 end */
    SON_UT_TRACE_EXIT();
 /* SPR:10540 changes end */


    return is_data_same;
} /* is_scan_and_scaned_data_same */


/*****************************************************************************
 * Function Name  : nmmctl_fsm_set_scan_config_cmd
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when  message SMIF_SET_NW_SCAN_CONFIG_CMD
 *                  is received from MIF.
 ****************************************************************************/
static void
nmmctl_fsm_set_scan_config_cmd
(
 void            *p_api_buf,
 nmm_gl_ctx_t    *p_nmm_gl_ctx
 )
{
    nmmctl_ctx_t                  *p_nmmctl_ctx               = SON_PNULL;
    smif_set_nw_scan_config_cmd_t *p_smif_set_scan_config_cmd = SON_PNULL;


    SON_UT_TRACE_ENTER();

    /* Fetch the NMM controller specific global context data pointer */
    p_nmmctl_ctx = p_nmm_gl_ctx->p_nmmctl_ctx;

    p_smif_set_scan_config_cmd = (smif_set_nw_scan_config_cmd_t *)(p_api_buf + SON_API_HEADER_SIZE);

    /* Set the scan time interval between two scan and force scan option */ 
    p_nmmctl_ctx->nmm_scan_config_param.force_scan = p_smif_set_scan_config_cmd->force_scan_on_off;

    /* Fetch the scan interval only if force scan is off */
    if (p_nmmctl_ctx->nmm_scan_config_param.force_scan == SON_FALSE)
    {    
        p_nmmctl_ctx->nmm_scan_config_param.scan_time_interval = p_smif_set_scan_config_cmd->scan_time_interval;

        if (g_last_scan_resp_data.scan_resp_data_valid == SON_TRUE)
        {    
        /* Stop the Timer */
        son_stop_timer(g_nmmctl_scan_resp_timer_id);
        /* Start: CSR 00070093 */
        g_nmmctl_scan_resp_timer_id = SON_PNULL;
        /*End: CSR 00070093 */

        /*start the scan response timer with the new value */
        g_nmmctl_scan_resp_timer_id = son_start_timer(p_nmmctl_ctx->nmm_scan_config_param.scan_time_interval,
                                                    &p_nmmctl_ctx->nmmctl_scan_resp_timer_buf,
                                                    sizeof(p_nmmctl_ctx->nmmctl_scan_resp_timer_buf),
                                                        SON_FALSE);
        }    
    }
    else
    {
        if (g_last_scan_resp_data.scan_resp_data_valid == SON_TRUE)
        {
            /* Stop the Timer */
            son_stop_timer(g_nmmctl_scan_resp_timer_id);
            /* Start: CSR 00070093 */
            g_nmmctl_scan_resp_timer_id = SON_PNULL;
            /*End: CSR 00070093 */
            /* Free the last scan data */
            nmmctl_fsm_free_last_scan_data();
        }    
    }

    SON_UT_TRACE_EXIT();

    return;
} /* nmmctl_fsm_set_scan_config_cmd */

/*****************************************************************************
 * Function Name  : nmmctl_fsm_free_last_scan_data
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function frees all the scanned data. 
 ****************************************************************************/
void
nmmctl_fsm_free_last_scan_data
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
    start_scan_res_eutran_data_t *p_start_scan_res_eutran_data = SON_PNULL;
    start_scan_res_utran_data_t  *p_start_scan_res_utran_data  = SON_PNULL;


    /* Free the saved scan input data */
    son_mem_free(p_last_scan_data);
    p_last_scan_data = SON_PNULL;

    g_last_scan_resp_data.scan_resp_data_valid = SON_FALSE;
    /* Free the saved scan response data */
    if (g_last_scan_resp_data.start_scan_res_data.p_eutran_scan_res_list != SON_PNULL)
    {
        while (g_last_scan_resp_data.start_scan_res_data.p_eutran_scan_res_list->count != EMPTY_LIST)
        {
            /* Get data from the nmm start scan resp list */
            SCAN_RESP_LIST_DEQUEUE(start_scan_res_eutran_data_t,
                g_last_scan_resp_data.start_scan_res_data.p_eutran_scan_res_list,
                                   p_start_scan_res_eutran_data);

            /* Delete the node from nmm_scan_resp_list_t */
            son_mem_free(p_start_scan_res_eutran_data);
            p_start_scan_res_eutran_data = SON_PNULL;
        }
    }
    else if (g_last_scan_resp_data.start_scan_res_data.p_utran_scan_res_list != SON_PNULL)
    {
        while (g_last_scan_resp_data.start_scan_res_data.p_utran_scan_res_list->count != EMPTY_LIST)
        {
            /* Get data from the nmm start scan resp list */
            SCAN_RESP_LIST_DEQUEUE(start_scan_res_utran_data_t,
                g_last_scan_resp_data.start_scan_res_data.p_utran_scan_res_list,
                                   p_start_scan_res_utran_data);

            /* Delete the node from nmm_scan_resp_list_t */
            son_mem_free(p_start_scan_res_utran_data);
            p_start_scan_res_utran_data = SON_PNULL;
        }
    }
    else
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
                "nmmctl_fsm_free_last_scan_data: NO Valid data present");
    }

    return;
} /* nmmctl_fsm_free_last_scan_data */

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : nmmctl_fsm_get_log_level_req_handler
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SMIF_GET_LOG_LEVEL_REQ
 *                  message received from SONMgmtIfH.
 ****************************************************************************/
static void
nmmctl_fsm_get_log_level_req_handler
(
    void            *p_api_buf,
    nmm_gl_ctx_t    *p_nmm_gl_ctx
)
{
    /*SPR 17777 +-*/
    (void)p_api_buf;
    (void)p_nmm_gl_ctx;
    /*SPR 17777 +-*/
    smif_get_log_level_req_t *p_msg = 
        (smif_get_log_level_req_t *)(p_api_buf + SON_API_HEADER_SIZE);
    smif_get_log_level_res_t res;
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&res, 0, sizeof(res));
    res.transaction_id = p_msg->transaction_id;
    res.log_level = g_nmm_log_level;
    
    /* Send cell delete resp to mif */
    son_create_send_buffer((son_u8 *)&res,
            SON_NMM_MODULE_ID,
            SON_MIF_MODULE_ID,
            SMIF_GET_LOG_LEVEL_RESP,
            sizeof(res));

    SON_UT_TRACE_EXIT();
} /* nmmctl_fsm_get_log_level_req_handler */
/* SPR-13251 Fix Ends */
