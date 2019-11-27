/****************************************************************************
*
* ARICENT -
*
*
* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
* $Id: son_mlb_msg_handler.c $
*
****************************************************************************
*
* File Description: This file contains the definition of manifest structure for
*                   SONMLB module. It also defines the init and message
*                   handler functions for the SONMLB module which are used
*                   for SONMLB initialization and message handling
*                   respectively.
*
****************************************************************************
* Revision Details
*----------------------
*   DATE            AUTHOR      REFERENCE       REASON
*  Dec, 2012      Sajida/Vinod  Initial         Initial
*  Aug, 2014      Shilpi                        SPR 13251 Fix
****************************************************************************/

/****************************************************************************
* Project Include
****************************************************************************/
#include <son_mlb_cell_fsm.h>
#include <son_mlb_fsm.h>
#include <son_utils.h>
#include <son_mlb_ctxt_mgr.h>
#include <ylib.h>
#include <rrm_son_intf.h>

/****************************************************************************
* Global Variables
****************************************************************************/
const const son_8 *p_son_mlb_facility_name = "SON_MLB";
extern mlb_fsm_fn_ptr_tbl mlb_fsm_table[MLB_MAX_FSM_STATE][MLB_MAX_FSM_EVENT];
extern const son_s8 *SON_MLB_FSM_STATES_NAMES[];

/*****************************************************************************
 * Function Name  : son_mlb_fill_mlb_param_default_values
 * Inputs         : p_mlb_config_param - Pointer to SONMLB global config
 *                  params
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : This function initializes the mlb global context
 *                  configuration parameters with default values.
 ****************************************************************************/
son_return_et
son_mlb_fill_mlb_param_default_values
(
    son_mlb_config_param_t *p_mlb_config_param
)
{
    SON_UT_TRACE_ENTER();

    p_mlb_config_param->max_ho_sample = MAX_HO_SAMPLE;
    p_mlb_config_param->q_offset_min = SON_Q_OFFSET_RANGE_DB_24;
    p_mlb_config_param->q_offset_max = SON_Q_OFFSET_RANGE_DB24;
    p_mlb_config_param->eutra_threshold_rsrp = DEFAULT_EUTRA_THRESHOLD_RSRP;
    p_mlb_config_param->small_step_cell_offset = DEFAULT_SMALL_STEP_CELL_OFFSET;
    p_mlb_config_param->eutra_load_threshold_idle = DEFAULT_EUTRA_LOAD_THRESHOLD_IDLE;
    p_mlb_config_param->eutra_load_threshold_conn = DEFAULT_EUTRA_LOAD_THRESHOLD_CONN;
    /*coverity_CID_26327 and 26326 start*/
    p_mlb_config_param->eutra_offset_min_step = SON_Q_OFFSET_RANGE_DB_20; /*2*/
    p_mlb_config_param->eutra_ocn_min_step = SON_Q_OFFSET_RANGE_DB_22; /*1*/
    /*coverity_CID_26327 and 26326 end
     *MLB Enhancement: start*/
    p_mlb_config_param->num_of_mlb_iteration = NUM_OF_MLB_ITERATION;
    p_mlb_config_param->mlb_start_threshold = DEFAULT_MLB_START_THRESHOLD;
    p_mlb_config_param->mlb_stop_threshold = DEFAULT_MLB_STOP_THRESHOLD;
    p_mlb_config_param->min_capacity_gain = DEFAULT_MIN_CAPACITY_GAIN;
    p_mlb_config_param->mlb_mode = SON_MODE_DISTRIBUTED;
    /*MLB Enhancement: End*/
    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* son_mlb_fill_mlb_param_default_values */

/*****************************************************************************
 * Function Name  : son_mlb_init
 * Inputs         : p_son_mlb_init_data - Pointer to SONMLB stack
 *                  initialization data (Unused,  but required as part of CSPL
 *                  signature)
 * Outputs        : None
 * Returns        : son_void_t *
 * Description    : This is the initialization function of SONMLB module.
 *                  It sets the SONMLB FSM state to init and send the INIT IND
 *                  to SONMgmtIfH.
 ****************************************************************************/
son_void_t *
son_mlb_init
(
    /* Pointer to SONMLB stack initialization data. Unused */
    son_void_t *p_son_mlb_init_data
)
{
    SON_UT_TRACE_ENTER();

    /* Init advance logger with default log file */
    SON_INIT_LOG(SON_PNULL);

    mlb_init_global_context();

    /*validate cfg data*/
    if ((g_mlb_context.mlb_config_param.q_offset_min > 
                SON_Q_OFFSET_RANGE_DB24) ||
            (g_mlb_context.mlb_config_param.q_offset_max > 
             SON_Q_OFFSET_RANGE_DB24) ||
            (g_mlb_context.mlb_config_param.q_offset_min >
             g_mlb_context.mlb_config_param.q_offset_max))
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_WARNING,
            "Invalid Qoffset range"
            " received from CFG file.Setting default value[q_offset_min=%d, q_offset_max=%d]",
            g_mlb_context.mlb_config_param.q_offset_min,g_mlb_context.mlb_config_param.q_offset_max);
        g_mlb_context.mlb_config_param.q_offset_min =
            SON_Q_OFFSET_RANGE_DB_24;
        g_mlb_context.mlb_config_param.q_offset_max =
            SON_Q_OFFSET_RANGE_DB24;
    }

    if (g_mlb_context.mlb_config_param.eutra_threshold_rsrp > MAX_EUTRA_THRESHOLD_RSRP)
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_WARNING,
            "Invalid threshold RSRP [%d]"
            " received from CFG file.Setting default value",
            g_mlb_context.mlb_config_param.eutra_threshold_rsrp);
        g_mlb_context.mlb_config_param.eutra_threshold_rsrp = DEFAULT_EUTRA_THRESHOLD_RSRP;
    }

    if (g_mlb_context.mlb_config_param.eutra_load_threshold_idle >= MAX_EUTRA_LOAD_THRESHOLD ||
        g_mlb_context.mlb_config_param.eutra_load_threshold_conn >= MAX_EUTRA_LOAD_THRESHOLD)
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_WARNING,
            "Invalid threshold load [threshold_idle=%d and threshold_conn=%d]"
            " received from CFG file. Setting default value[%d,%d]",
            g_mlb_context.mlb_config_param.eutra_load_threshold_idle,
            g_mlb_context.mlb_config_param.eutra_load_threshold_conn,
            DEFAULT_EUTRA_LOAD_THRESHOLD_IDLE,
            DEFAULT_EUTRA_LOAD_THRESHOLD_CONN);
        g_mlb_context.mlb_config_param.eutra_load_threshold_idle = DEFAULT_EUTRA_LOAD_THRESHOLD_IDLE;
        g_mlb_context.mlb_config_param.eutra_load_threshold_conn = DEFAULT_EUTRA_LOAD_THRESHOLD_CONN;
    }

    if ((g_mlb_context.mlb_config_param.eutra_offset_min_step > SON_Q_OFFSET_RANGE_DB24) ||
        (g_mlb_context.mlb_config_param.eutra_ocn_min_step > SON_Q_OFFSET_RANGE_DB24))
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_INFO,
            "Invalid offset step[offset_min_step=%d and ocn_min_step=%d]"
            " received from CFG file.Setting default value[%d,%d]",
            g_mlb_context.mlb_config_param.eutra_offset_min_step,
            g_mlb_context.mlb_config_param.eutra_ocn_min_step,
            SON_Q_OFFSET_RANGE_DB_20,
            SON_Q_OFFSET_RANGE_DB_22);
        /*coverity_CID_26325 and 26324 start*/
        g_mlb_context.mlb_config_param.eutra_offset_min_step = SON_Q_OFFSET_RANGE_DB_20;
        g_mlb_context.mlb_config_param.eutra_ocn_min_step = SON_Q_OFFSET_RANGE_DB_22;
        /*coverity_CID_26325 and 26324 end*/
    }

    /*MLB Enhancement: start*/
    if (g_mlb_context.mlb_config_param.num_of_mlb_iteration > NUM_OF_MLB_ITERATION ||
        g_mlb_context.mlb_config_param.num_of_mlb_iteration == MIN_NUM_OF_MLB_ITERATION)
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_WARNING,
            "Invalid number of iteration[num_of_mlb_iteration=%d]"
            " received from CFG file.Setting default value[%d]",
            g_mlb_context.mlb_config_param.num_of_mlb_iteration,
            NUM_OF_MLB_ITERATION);

        g_mlb_context.mlb_config_param.num_of_mlb_iteration = NUM_OF_MLB_ITERATION;
    }

    if ((g_mlb_context.mlb_config_param.mlb_stop_threshold > MAX_EUTRA_LOAD_THRESHOLD) ||
        (g_mlb_context.mlb_config_param.mlb_start_threshold > MAX_EUTRA_LOAD_THRESHOLD) ||
        (g_mlb_context.mlb_config_param.mlb_stop_threshold <
         g_mlb_context.mlb_config_param.mlb_start_threshold))
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_WARNING,
            "Invalid start stop threshold"
            " received from CFG file.Setting default value[%d,%d]",
            DEFAULT_MLB_START_THRESHOLD,
            DEFAULT_MLB_STOP_THRESHOLD);

        g_mlb_context.mlb_config_param.mlb_start_threshold = DEFAULT_MLB_START_THRESHOLD;
        g_mlb_context.mlb_config_param.mlb_stop_threshold = DEFAULT_MLB_STOP_THRESHOLD;
    }

    if (g_mlb_context.mlb_config_param.min_capacity_gain > MAX_CAPACITY_GAIN)
    {
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_WARNING,
            "Invalid min capacity gain[%d]"
            " received from CFG file.Setting default value[%d]",
            g_mlb_context.mlb_config_param.min_capacity_gain,
            DEFAULT_MIN_CAPACITY_GAIN);

        g_mlb_context.mlb_config_param.min_capacity_gain = DEFAULT_MIN_CAPACITY_GAIN;
    }

    if (g_mlb_context.mlb_config_param.mlb_mode > SON_MODE_CENTRALIZED)
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_WARNING,
            "Invalid mlb_mode[%d]"
            " received from CFG file.Setting default value[%d] p_son_mlb_init_data=%p",
            g_mlb_context.mlb_config_param.mlb_mode,
            SON_MODE_DISTRIBUTED,
            p_son_mlb_init_data);
                    /*SPR 17777 +-*/

        g_mlb_context.mlb_config_param.mlb_mode = SON_MODE_DISTRIBUTED;
    }

    /*MLB Enhancement: End*/

    mlb_init_pending_res(SON_UNDEFINED, SON_PROCEDURE_UNDEFINED);

    /* Send SMIF_INIT_IND to SONMgmtIfh */
    son_create_send_buffer(SON_PNULL, SON_MLB_MODULE_ID,
        SON_MIF_MODULE_ID, SMIF_INIT_IND,
        0);
    SON_UT_TRACE_EXIT();

    /* Returning pointer to the global context of SONMLB - unused
     * in current approach */
    return SON_PNULL;
} /* son_mlb_init */

/*****************************************************************************
 * Function Name  : son_mlb_timer_handler
 * Inputs         : timer_id - Identity of the expired timer
 *                  p_timer_buf - Pointer to the received message buffer
 *                  p_gl_ctx - Pointer to the global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the expiry of MLB timer
 ****************************************************************************/
static son_void_t
son_mlb_timer_handler
(
    son_timer_t timer_id,     /* Identity of the expired timer */
    son_void_t   *p_timer_buf,  /* Pointer to data associated with the timer */
    son_void_t   *p_gl_ctx      /* Pointer to the global context data */
)
{
    SON_UT_TRACE_ENTER();

    /* Check if MLB_PENDING_RESPONSE_TIMER timer expired */
    if (g_mlb_context.mlb_prending_res_timer_id == timer_id)
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
            SON_DETAILED,
            "son_mlb_timer_handler: MLB pending response timer"
            " expired p_timer_buf=%p p_gl_ctx=%p",p_timer_buf,p_gl_ctx);
                    /*SPR 17777 +-*/
        mlb_pending_res_timer_handler();
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mlb_get_event_id
 * Inputs         : api     -   API Id of message received
 * Outputs        : None
 * Returns        : mlb_fsm_event_et
 * Description    : This is the event id geneartor for OAM APIs
 ******************************************************************************/
mlb_fsm_event_et
son_mlb_get_event_id
(
    son_u16 api
)
{
    mlb_fsm_event_et ret;
    son_oam_log_on_off_et log_mode    = mlb_get_log_mode();
    /*coverty_CID_21517 fix start*/
    son_procedure_code_et proc_code = (son_procedure_code_et)api;

    switch (proc_code)
    /*coverty_CID_21517 fix end*/
    {
        case SMIF_INIT_CONFIG_REQ:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MIF->MLB:SMIF_INIT_CONFIG_REQ");
            ret =  SONMLB_INIT_CONFIG_REQ;
            break;
        }

        case SONMLB_ENABLE_REQ:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MIF->MLB:SONMLB_ENABLE_REQ");
            ret =  SONMLB_ENABLE_REQ_HAND;
            break;
        }

        case SONMLB_DISABLE_REQ:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MIF->MLB:SONMLB_DISABLE_REQ");
            ret =  SONMLB_DISABLE_REQ_HAND;
            break;
        }

        case SONMLB_MODIFY_ATTRIBUTES_REQ:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MIF->MLB:SONMLB_MODIFY_ATTRIBUTES_REQ");
            ret =  SONMLB_MODIFY_REQ;
            break;
        }

        case SMIF_SHUTDOWN_REQ:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MIF->MLB:SMIF_SHUTDOWN_REQ");
            ret =  SMIF_SHUTDOWN_REQ_HAND;
            break;
        }

        case SMIF_CELL_INFO_IND:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MIF->MLB:SMIF_CELL_INFO_IND");
            ret =  SMIF_CELL_INFO_IND_HAND;
            break;
        }

        case SMIF_LOG_ENABLE_DISABLE_CMD:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MIF->MLB:SMIF_LOG_ENABLE_DISABLE_CMD");
            ret =  SMIF_LOG_ENABLE_DISABLE_CMD_HAND;
            break;
        }

        case SMIF_SET_LOG_LEVEL_CMD:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MIF->MLB:SMIF_SET_LOG_LEVEL_CMD");
            ret =  SMIF_SET_LOG_LEVEL_CMD_HAND;
            break;
        }

        case SONANR_DISABLE_IND:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "ANR->MLB:SONANR_DISABLE_IND");
            ret =  SONANR_DISABLE_IND_HAND;
            break;
        }

        case SONMLB_MOBILITY_CHANGE_REQ:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "SONX2->MLB:SONMLB_MOBILITY_CHANGE_REQ");
            ret =  SON_MOBILITY_CHANGE_REQ;
            break;
        }

        case SONMLB_MOBILITY_CHANGE_ACK:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "SONX2->MLB:SONMLB_MOBILITY_CHANGE_ACK");
            ret =  SON_MOBILITY_CHANGE_ACK;
            break;
        }

        case SONMLB_MOBILITY_CHANGE_FAILURE:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "SONX2->MLB:SONMLB_MOBILITY_CHANGE_FAILURE");
            ret =  SON_MOBILITY_CHANGE_FAILURE;
            break;
        }

/*Rel 3.0 Multi Sector code changes Start*/
        case SONMLB_INTERNAL_MOBILITY_CHANGE_REQ:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MLB->MLB:SONMLB_INTERNAL_MOBILITY_CHANGE_REQ");
            ret =  SON_INTERNAL_MOBILITY_CHANGE_REQ;
            break;
        }

        case SONMLB_INTERNAL_MOBILITY_CHANGE_ACK:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MLB->MLB:SONMLB_INTERNAL_MOBILITY_CHANGE_ACK");
            ret =  SON_INTERNAL_MOBILITY_CHANGE_ACK;
            break;
        }

        case SONMLB_INTERNAL_MOBILITY_CHANGE_FAILURE:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MLB->MLB:SONMLB_INTERNAL_MOBILITY_CHANGE_FAILURE");
            ret =  SON_INTERNAL_MOBILITY_CHANGE_FAILURE;
            break;
        }
/*Rel 3.0 Multi Sector code changes Stop*/

        case SONANR_UPDATED_NRT_INFO:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "ANR->MLB:SONANR_UPDATED_NRT_INFO");
            ret =  SONANR_NBR_UPDATE_INFO_HAND;
            break;
        }

        case SONMLB_OFFSET_CHANGE_REQ:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MIF->MLB:SONMLB_OFFSET_CHANGE_REQ");
            ret =  SONMLB_OFFSET_CHANGE_REQ_HAND;
            break;
        }

        case SONMLB_PEER_OFFSET_CHANGE_RESP:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MIF->MLB:SONMLB_PEER_OFFSET_CHANGE_RESP");
            ret =  SONMLB_PEER_OFFSET_CHANGE_RESP_HAND;
            break;
        }
        /* Rel 3.0 Cell delete support changes Start */
        case SMIF_CELL_DELETE_REQ:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MIF->MLB:SMIF_CELL_DELETE_REQ");
            ret =  SMIF_CELL_DELETE_REQ_HAND;
            break;
        }
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        case SMIF_GET_LOG_LEVEL_REQ:
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "MIF->MLB:SMIF_GET_LOG_LEVEL_REQ");
            ret =  SMIF_GET_LOG_LEVEL_REQ_HAND;
            break;
        }
        /* SPR-13251 Fix Ends */
        default:
        {
            ret = MLB_EVENT_UNDEFINED;
            break;
        }
    } /* switch */

    return ret;
} /* son_mlb_get_event_id */

/*****************************************************************************
 * Function Name  : rrm_get_event_id
 * Inputs         : api -      API Id of message received
 * Outputs        : None
 * Returns        : mlb_fsm_event_et
 * Description    : This is the event id geneartor for RRM APIs
 ******************************************************************************/
mlb_fsm_event_et
rrm_get_event_id
(
    son_u16 api
)
{
    mlb_fsm_event_et ret;
    rrm_son_message_resp_et proc_code = (rrm_son_message_resp_et)api;

    switch (proc_code)
    {
        case RRM_SON_REGISTER_RESP:
        {
            ret =  RRM_SON_REGISTER_RES_HAND;
            break;
        }

        case RRM_SON_DEREGISTER_RESP:
        {
            ret =  RRM_SON_DEREGISTER_RES_HAND;
            break;
        }

        case RRM_SON_HO_REPORT:
        {
            ret =  RRM_SON_HO_REPORT_HAND;
            break;
        }

        case RRM_SON_LOAD_REPORT_IND:
        {
            ret =  RRM_SON_LOAD_REPORT_HAND;
            break;
        }

        case RRM_SON_CELL_STATE_CHANGE_IND:
        {
            ret =  RRM_SON_CELL_STATE_CHANGE;
            break;
        }
/* eICIC feature code Starts */
        case RRM_SON_EICIC_ENABLE_DISABLE_IND:
        {
            ret =  RRM_SON_EICIC_ENABLE_DISABLE;
            break;
        }
/* eICIC feature code Ends */

        default:
        {
            ret = MLB_EVENT_UNDEFINED;
            break;
        }
    } /* switch */

    return ret;
} /* rrm_get_event_id */

/*****************************************************************************
 * Function Name  : son_mlb_process_msg
 * Inputs         : p_buf - Pointer to msg buffer
 *                  p_cxt - Pointer to the SONMLB global context data(Unused,
 *                  but required as part of CSPL signature)
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the main entry point of SONMLB module - CSPL
 *                  callback. It receives all the external APIs received
 *                  from external entities and invokes appropriate
 *                  handlers based on the API ID and current state.
 ****************************************************************************/
S32
son_mlb_process_msg
(
    son_void_t *p_buf,
    son_void_t *p_cxt
)
{
    STACKAPIHDR cspl_info = {0};
    son_buf_retain_status_et ret =  RELEASE_SON_BUFFER;
    son_u8                      *p_msg  = (son_u8 *)p_buf;
    son_oam_log_on_off_et log_mode    = mlb_get_log_mode();
    mlb_fsm_state_et curr_state  = MLB_STATE_UNDEFINED;
    mlb_fsm_event_et event_id;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_msg)
    {
                    /*SPR 17777 +-*/
        SON_LOG(log_mode, p_son_mlb_facility_name, SON_ERROR,
                "API pointer received is NULL");
        LTE_GCC_UNUSED_PARAM(p_cxt);
                    /*SPR 17777 +-*/
    }
    else
    {
        /*Parse the cspl header*/
        son_parse_cspl_header(p_msg, &cspl_info);

        /* Offset by CV_HDRSIZE to skip the CSPL Header */
        p_msg = p_msg + CV_HDRSIZE;

        curr_state = mlb_get_context_state();

        /*Klockwork Fix Starts*/
        if (cspl_info.from == SON_RRM_MODULE_ID)
        {
            event_id = rrm_get_event_id(cspl_info.api);
        }
        else
        {
            event_id  = son_mlb_get_event_id(cspl_info.api);
        }
        if(curr_state < MLB_MAX_FSM_STATE && event_id < MLB_MAX_FSM_EVENT)
        {
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "### MLB_FSM: Processing state %s Event %u ###",
                    SON_MLB_FSM_STATES_NAMES[curr_state], event_id);
            ret = mlb_fsm_table[curr_state]
                [event_id](p_msg, &cspl_info);
            SON_LOG(log_mode, p_son_mlb_facility_name, SON_INFO,
                    "### MLB_FSM: Processing Complete ###");
        }
        /*Klockwork Fix Ends*/
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* son_mlb_process_msg */

static const son_ul32 son_mlb_dst_list[] =
{
    SON_MIF_MODULE_ID,
    SON_ANR_MODULE_ID,
    SON_RRM_MODULE_ID,
    SON_X2_MODULE_ID
};

/****************************************************************************
* Initialization of QManifest Structure for SON MLB Module
****************************************************************************/
const QMANIFEST son_manifest_mlb =
{
    "SON_MLB_MODULE_ID",
    SON_MLB_MODULE_ID,

    { 0, son_mlb_init },            /*{ early initialization, late initialization }*/
    { son_mlb_process_msg, son_mlb_timer_handler }, /* { message processor, timer handler }*/
    {0, 0, 0 },                   /* { pack, unpack, alloc}*/
    son_mlb_dst_list                /* destinations*/
};
