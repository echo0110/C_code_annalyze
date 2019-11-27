/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_mlb_fsm.c $
 *
 *******************************************************************************
 *
 * File Description:This file contains the functions for handling MLB level
 *        messages and functions for manifest structure of this module.
 *
 *******************************************************************************
* Revision Details
 *----------------------
 *   DATE            AUTHOR       REFERENCE       REASON
 *   Dec,2012       Sajida/Vinod  Initial         Initial
 *   Aug, 2014      Shilpi                        SPR 13251 Fix
 ******************************************************************************/

#include <son_mlb_cell_fsm.h>
#include <son_mlb_fsm.h>
#include <son_utils.h>
#include <son_mlb_ctxt_mgr.h>
#include <son_rrm_intf.h>
#include <rrm_son_parser.h>
#include <son_rrm_composer.h>
#include <son_rrm_parser.h>
#include <son_anr_intf.h>
#include <son_defines.h>
#include <rrm_son_intf.h> 

extern mlb_global_t  g_mlb_context;
extern const son_8 *p_son_mlb_facility_name;
/* Rel 3.0 Cell delete support changes Start */
extern pending_res_t g_mlb_pending_response;
/* Rel 3.0 Cell delete support changes Stop */

/*****************************************************************************
 * Function Name  : son_mlb_get_cell_event_id
 * Inputs         : api  - API Id of message received
 * Outputs        : None
 * Returns        : son_mlb_index_et - Event Id internal to MLB module
 * Description    : This is the event id geneartor for OAM APIs
 ******************************************************************************/
son_mlb_index_et 
son_mlb_get_cell_event_id
(
    son_u16 api
)
{
    son_mlb_index_et ret;
    /*coverty_CID_21516 fix start*/
    son_procedure_code_et proc_code = (son_procedure_code_et)api;

    switch (proc_code)
    /*coverty_CID_21516 fix end*/
    {
        case SONMLB_ENABLE_REQ:
        {
            ret =  CELL_SONMLB_ENABLE_REQ;
            break;
        }

        case SONMLB_DISABLE_REQ:
        {
            ret =  CELL_SONMLB_DISABLE_REQ;
            break;
        }

        case SONMLB_MODIFY_ATTRIBUTES_REQ:
        {
            ret =  CELL_SONMLB_MODIFY_REQ;
            break;
        }

        case SONANR_DISABLE_IND:
        {
            ret =  CELL_SON_ANR_DISABLE_IND;
            break;
        }

        case SONMLB_MOBILITY_CHANGE_REQ:
        {
            ret =  CELL_MOBILITY_CHANGE_REQ;
            break;
        }

        case SONMLB_MOBILITY_CHANGE_ACK:
        {
            ret =  CELL_MOBILITY_CHANGE_ACK;
            break;
        }

        case SONMLB_MOBILITY_CHANGE_FAILURE:
        {
            ret =  CELL_MOBILITY_CHANGE_FAILURE;
            break;
        }

/*Rel 3.0 Multi Sector code changes Start*/
        case SONMLB_INTERNAL_MOBILITY_CHANGE_REQ:
        {
            ret =  CELL_INTERNAL_MOBILITY_CHANGE_REQ;
            break;
        }

        case SONMLB_INTERNAL_MOBILITY_CHANGE_ACK:
        {
            ret =  CELL_INTERNAL_MOBILITY_CHANGE_ACK;
            break;
        }

        case SONMLB_INTERNAL_MOBILITY_CHANGE_FAILURE:
        {
            ret =  CELL_INTERNAL_MOBILITY_CHANGE_FAILURE;
            break;
        }

/*Rel 3.0 Multi Sector code changes Stop*/

        case SONANR_UPDATED_NRT_INFO:
        {
            ret =  CELL_SON_MLB_NBR_DELETE_IND;
            break;
        }

        case SONMLB_OFFSET_CHANGE_REQ:
        {
            ret =  CELL_SON_MLB_OFFSET_CHANGE_REQ;
            break;
        }

        case SONMLB_PEER_OFFSET_CHANGE_RESP:
        {
            ret =  CELL_SON_MLB_PEER_OFFSET_CHANGE_RESP;
            break;
        }

        default:
        {
            ret = CELL_SON_INVALID_EVENT;
            break;
        }
    } /* switch */

    return ret;
} /* son_mlb_get_cell_event_id */

/*****************************************************************************
 * Function Name  : rrm_get_cell_event_id
 * Inputs         : api    - API Id of message received
 * Outputs        : None
 * Returns        : son_mlb_index_et - Event Id internal to MLB module
 * Description    : This is the event id geneartor for rrm APIs
 ******************************************************************************/
son_mlb_index_et 
rrm_get_cell_event_id
(
    son_u16 api
)
{
    son_mlb_index_et ret;
    rrm_son_message_resp_et rrm_proc_code = (rrm_son_message_resp_et)api;
    
    switch (rrm_proc_code)
    {
            case RRM_SON_REGISTER_RESP:
        {
                ret =  CELL_RRM_REGISTER_RES;
                break;
        }

        case RRM_SON_DEREGISTER_RESP:
        {
                ret =  CELL_RRM_DEREGISTER_RES;
                break;
        }

        case RRM_SON_HO_REPORT:
        {
                ret =  CELL_RRM_SON_HO_REPORT;
                break;
        }

        case RRM_SON_LOAD_REPORT_IND:
        {
                ret =  CELL_RRM_SON_LOAD_REPORT;
                break;
        }

        case RRM_SON_CELL_STATE_CHANGE_IND:
        {
                ret =  CELL_SON_MLB_STATE_CHANGE_IND;
                break;
        }
        /* eICIC feature code Starts */
        case RRM_SON_EICIC_ENABLE_DISABLE_IND:
        {
            ret = CELL_SON_EICIC_ENABLE_DISABLE_IND;
            break;
        }
        /* eICIC feature code Ends */

        default:
        {
            ret = CELL_SON_INVALID_EVENT;
            break;
        }
    } /* switch */

    return ret;
} /* rrm_get_cell_event_id */

/*****************************************************************************
 * Function Name  : mlb_shutdown_handler
 * Inputs         : p_msg     -   Pointer to the message received
 *                : p_cspl_info - Pointer to STACKAPIHDR structure
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is called for
 * handling SHUT DOWN msg in any state 
 ******************************************************************************/
son_buf_retain_status_et
mlb_shutdown_handler
(
 son_u8 *p_msg,
 STACKAPIHDR *p_cspl_info
 )
{
    mlb_cell_context_node_t *p_ctxt_node    = SON_PNULL;
    smif_shutdown_res_t shut_down_res       = {0};

    SON_UT_TRACE_ENTER();

    p_ctxt_node = mlb_get_first_cell_ctxt();
    while (SON_PNULL != p_ctxt_node)
    {
        if (MLB_CELL_STATE_ENABLING == 
                p_ctxt_node->data.current_cell_fsm_state ||
                MLB_CELL_STATE_ENABLED == 
                p_ctxt_node->data.current_cell_fsm_state)
        {
            if (SON_FAILURE == mlb_construct_send_rrm_deregister_req(
                        *(son_u16 *)(p_msg),
                        &p_ctxt_node->data.cell_id))
            {
                SON_LOG(mlb_get_log_mode(),
                        p_son_mlb_facility_name,
                        SON_DETAILED,
                        "mlb_construct_send_rrm_deregister_req failed "
                        "for Cell 0x%x]",
                        son_convert_char_cell_id_to_int_cell_id(p_ctxt_node->data.cell_id.cell_identity));
            }
        }

        p_ctxt_node = mlb_get_next_node(p_ctxt_node);
    }

    /*Clean up memory of each Cell Context */
    mlb_clear_context_list();

    /* Clean up ENABLE/DISABLE Pending Response list */
    mlb_clear_pending_res_list();

    mlb_set_context_state(MLB_STATE_SHUTDOWN);

    /* Send SMIF_SHUTDOWN_RES to SONMgmtIfH*/
    shut_down_res.transaction_id = *(son_u16 *)(p_msg);
    shut_down_res.result         = SON_SUCCESS;
    shut_down_res.error_code     = SON_NO_ERROR;

    son_create_send_buffer((son_u8 *)&shut_down_res, SON_MLB_MODULE_ID,
            (son_module_id_et)p_cspl_info->from,
        SMIF_SHUTDOWN_RES, sizeof(shut_down_res));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_shutdown_handler */

/*****************************************************************************
 * Function Name  : mlb_init_state_handler
 * Inputs         : p_msg   -     Pointer to the message received
 *                : p_cspl_hdr  - Pointer to STACKAPIHDR structure
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the SMIF_INIT_CONFIG_REQ message handler for MLB module
 ******************************************************************************/
son_buf_retain_status_et
mlb_init_state_handler
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr
 )
{
    smif_init_config_res_t            init_config_res = {0};
    son_u8                            length  = 0;
    son_procedure_code_et             api     = SMIF_GENERIC_RES;

    SON_UT_TRACE_ENTER();

    smif_init_config_req_t *msg = (smif_init_config_req_t *)p_msg;
    mlb_set_log_mode((son_oam_log_on_off_et)(msg->log_enable)); /*coverity_CID_21645 fix start-end*/

    /* Print the current logging status(enable/disable)
     * SON_TRUE: always print log,even if it is disabled. */

    SON_LOG(SON_OAM_LOG_ON, p_son_mlb_facility_name, SON_BRIEF,
            "mlb_log_handler: Log enable/disable = %u",
            msg->log_enable); 
    if (SON_OAM_LOG_ON == mlb_get_log_mode())
    {
        /* SPR-13251 Fix Starts */
        mlb_set_log_level(msg->log_level);
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(msg->log_level);
    }
    else
    {
        /* SPR-13251 Fix Starts */
        mlb_set_log_level(SON_LOG_LEVEL_NONE);
        /* SPR-13251 Fix Ends */
        /* Set log level to NONE  */
        SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
    }

    mlb_set_context_state(MLB_STATE_ACTIVE);

    api = SMIF_INIT_CONFIG_RES;
    init_config_res.transaction_id = *((son_u16 *)p_msg);
    init_config_res.result = SON_SUCCESS;
    init_config_res.error_code = SON_NO_ERROR;
    length = sizeof(smif_init_config_res_t);
    son_create_send_buffer((son_u8 *)&init_config_res,
            SON_MLB_MODULE_ID,
            (son_module_id_et)p_cspl_hdr->from, api,
            length );

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_init_state_handler */

/*****************************************************************************
 * Function Name  : mlb_log_handler
 * Inputs         : p_buf -  Pointer to the message
 *                : p_cspl_hdr - Pointer to STACKAPIHDR structure
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is called
 *                  when SMIF_INIT_CONFIG_REQ or SMIF_LOG_ENABLE_DISABLE_CMD or
 *                  SMIF_SET_LOG_LEVEL_CMD message is received 
 ******************************************************************************/
son_buf_retain_status_et
mlb_log_handler
(
    son_u8          *p_buf,
    STACKAPIHDR     *p_cspl_hdr
 )
{
    SON_UT_TRACE_ENTER();
    switch (p_cspl_hdr->api)
    {
        case SMIF_LOG_ENABLE_DISABLE_CMD:
            {
                smif_log_enable_disable_cmd_t *p_msg = 
                    (smif_log_enable_disable_cmd_t *)p_buf;
                mlb_set_log_mode((son_oam_log_on_off_et)(p_msg->log_enable)); /*coverity_CID_21640 fix start-end*/

            /* Print the current logging status(enable/disable)
             * SON_TRUE: always print log, even if it is disabled. */
                SON_LOG(SON_OAM_LOG_ON, p_son_mlb_facility_name, SON_BRIEF,
                        "Log enable/disable = %u",
                        p_msg->log_enable);

                if (SON_OAM_LOG_ON == mlb_get_log_mode())
                {
                if (p_msg->bitmask & SMIF_MODULE_LOG_LEVEL_PRESENT)
                {
                    /* SPR-13251 Fix Starts */
                    mlb_set_log_level(p_msg->log_level);
                    /* SPR-13251 Fix Ends */
                    SET_MODULE_LOG_LEVEL(p_msg->log_level);
                }
            }
            else
            {
                /* SPR-13251 Fix Starts */
                mlb_set_log_level(SON_LOG_LEVEL_NONE);
                /* SPR-13251 Fix Ends */
                /* Set log level to NONE  */
                SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
            }

            break;
        }

        case SMIF_SET_LOG_LEVEL_CMD:
            {
                smif_set_log_level_cmd_t *p_msg =
                (smif_set_log_level_cmd_t *)p_buf;
                if (SON_OAM_LOG_ON == mlb_get_log_mode())
                {
                    SON_LOG(SON_OAM_LOG_ON, p_son_mlb_facility_name,
                            SON_BRIEF,
                            "Log level changed through msg: "
                            "SMIF_SET_LOG_LEVEL_CMD");
                    /* SPR-13251 Fix Starts */
                    mlb_set_log_level(p_msg->log_level);
                    /* SPR-13251 Fix Ends */
                    SET_MODULE_LOG_LEVEL(p_msg->log_level);
                }

            break;
        }

        default:
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                SON_ERROR,
                "Invalid message id %d", p_cspl_hdr->api);
            break;
        }
    } /* switch */

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_log_handler */

/*****************************************************************************
 * Function Name  : mlb_create_send_generic_nack
 * Inputs         : p_msg      -    Pointer to the message received
 *                  p_cspl_hdr -    Pointer to cspl stack header
 *                  error_code -    Error code to be sent
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the function to send NACK response to sender
 ******************************************************************************/
son_void_t
mlb_create_send_generic_nack
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_error_et     error_code
 )
{
    SON_UT_TRACE_ENTER();

    if (SON_MIF_MODULE_ID == p_cspl_hdr->from)
    {
        mlb_create_send_generic_nack_to_mif(p_msg, p_cspl_hdr, error_code);
    }
    else
    {
        SON_LOG(mlb_get_log_mode(),
                p_son_mlb_facility_name,
                SON_ERROR,
                "Unexpected Message %d Received from unknown module %d",
                p_cspl_hdr->api, p_cspl_hdr->from);
    }

    SON_UT_TRACE_EXIT();
} /* mlb_create_send_generic_nack */

/*****************************************************************************
 * Function Name  : mlb_create_send_generic_nack_to_mif
 * Inputs         : p_msg       -   Pointer to the message received
 *                  p_cspl_hdr  -   Pointer to cspl stack header
 *                  error_code  -   Error code to be sent
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the function to send NACK response to SMIF
 ******************************************************************************/
son_void_t
mlb_create_send_generic_nack_to_mif
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_error_et     error_code
 )
{
    son_bool_et                     send_res    = SON_TRUE;
    son_procedure_code_et           api         = (son_procedure_code_et)SON_UNDEFINED; /*coverity_CID_21646 fix start-end*/
    son_u32                         length      = SON_NULL;
    son_u8                          *p_resp     = SON_PNULL;

    SON_UT_TRACE_ENTER();

    switch (p_cspl_hdr->api)
    {
        case SONMLB_ENABLE_REQ:  /* Intentional Break Left */
        case SONMLB_DISABLE_REQ: /* Enable and Disable are same structures */
            {
            api = (SONMLB_ENABLE_REQ == p_cspl_hdr->api) ? SONMLB_ENABLE_RES
                  : SONMLB_DISABLE_RES;
                length = sizeof(son_mlb_enable_res_t);
                p_resp = mlb_create_enable_disable_req_nack(p_msg, api, length, 
                        error_code);
            break;
        }

        default:
        {
            SON_LOG(mlb_get_log_mode(),
                    p_son_mlb_facility_name,
                    SON_ERROR,
                    "Unexpected Message %d Received from SMIF",
                    p_cspl_hdr->api);
            send_res = SON_FALSE;
            break;
    }
    } /* switch */

    if (send_res && p_resp)
    {
        /*Coverity CID 79204 Fix Start*/
        son_create_send_buffer(p_resp, SON_MLB_MODULE_ID,
                SON_MIF_MODULE_ID, api,
                length );
        /*son_create_send_buffer(p_resp, (son_module_id_et)p_cspl_hdr->to,
              (son_module_id_et)p_cspl_hdr->from, api,
                length );*/
        /*Coverity CID 79204 Fix End*/
        son_mem_free(p_resp);
    }

    SON_UT_TRACE_EXIT();
} /* mlb_create_send_generic_nack_to_mif */

/*****************************************************************************
 * Function Name  : mlb_create_enable_disable_req_nack
 * Inputs         : p_msg    -    Pointer to the message received
 *                  api     -     API Id
 *                  length   -    length of the enable/disable response message  
 *                  error_code -  Error code to be sent
 * Outputs        : p_resp       Pointer to the response message created.
 * Returns        : son_u8*
 * Description    : This is the function to create the enable/disable 
 *                  NACK response
 ******************************************************************************/
son_u8 *
mlb_create_enable_disable_req_nack
(
    son_u8                  *p_msg,
    son_procedure_code_et   api, 
    son_u32                 length,
    son_error_et            error_code
 )
{
    son_u8 *p_resp = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);

    if (SON_PNULL != p_resp)
    {
        /* Creating nack response for ENABLE REQ */
        if (SONMLB_ENABLE_RES == api)
        {
            SON_MEMSET(p_resp, 0, sizeof(son_mlb_enable_req_t));
            ((son_mlb_enable_req_t *)p_resp)->transaction_id =
                *((son_u16 *)p_msg);

            /* Filling the response structure */
            ((son_mlb_enable_res_t *)p_resp)->enable_res.result = SON_FAILURE;
            ((son_mlb_enable_res_t *)p_resp)->enable_res.error_code = SON_NO_ERROR;
            ((son_mlb_enable_res_t *)p_resp)->enable_res.cell_status_list_size = 1;
            SON_MEMCPY(&(((son_mlb_enable_res_t *)p_resp)->enable_res.cell_status_list[0].cgi),
                    &(((son_mlb_enable_req_t *)p_msg)->enable_req.src_cgi),
                    sizeof(son_intra_rat_global_cell_id_t));
            ((son_mlb_enable_res_t *)p_resp)->enable_res.cell_status_list[0].error_code = error_code;
        }

        /* Creating nack response for DISABLE REQ */
        else if (SONMLB_DISABLE_RES == api)
        {
            SON_MEMSET(p_resp, 0, sizeof(son_mlb_disable_res_t));
            ((son_mlb_disable_res_t *)p_resp)->transaction_id =
                *((son_u16 *)p_msg);

            /* Filling the response structure */
            ((son_mlb_disable_res_t *)p_resp)->disable_res.result = SON_FAILURE;
            ((son_mlb_disable_res_t *)p_resp)->disable_res.error_code = SON_NO_ERROR;
            ((son_mlb_disable_res_t *)p_resp)->disable_res.cell_status_list_size = 1;
            SON_MEMCPY(&(((son_mlb_disable_res_t *)p_resp)->disable_res.cell_status_list[0].cgi),
                    &(((son_mlb_disable_req_t *)p_msg)->disable_req.src_cgi),
                    sizeof(son_intra_rat_global_cell_id_t));
            ((son_mlb_disable_res_t *)p_resp)->disable_res.cell_status_list[0].error_code = error_code;
        }
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* mlb_create_enable_disable_req_nack */

/*****************************************************************************
 * Function Name  : mlb_fsm_nack_enable_req_msg
 * Inputs         : p_msg   -     Pointer to the message received
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the function to handle unexpected messages in a MLB state
 ******************************************************************************/
son_buf_retain_status_et
mlb_fsm_nack_enable_req_msg
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr
 )
{
    SON_UT_TRACE_ENTER();
    son_mlb_enable_res_t  enable_resp = {0};
    son_mlb_enable_req_t  *p_req = (son_mlb_enable_req_t *)p_msg;
    son_intra_rat_global_cell_id_t *p_cell_id = SON_PNULL;

    enable_resp.enable_res.cell_status_list_size = 1;
    enable_resp.enable_res.result = SON_FAILURE;
    enable_resp.enable_res.error_code = SON_NO_ERROR;
    enable_resp.enable_res.cell_status_list[0].error_code = 
        SON_ERR_ALREADY_ENABLED;

    if (MLB_STATE_INIT == g_mlb_context.current_fsm_state)
    {
        enable_resp.enable_res.cell_status_list[0].error_code = 
            SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE;
    }
    else if (MLB_STATE_ENABLING == g_mlb_context.current_fsm_state)
    {
        enable_resp.enable_res.cell_status_list[0].error_code = 
            SON_ERR_ENABLE_IN_PROGRESS;
    }
    else if (MLB_STATE_DISABLING == g_mlb_context.current_fsm_state)
    {
        enable_resp.enable_res.cell_status_list[0].error_code = 
            SON_ERR_DISABLE_IN_PROGRESS;
    }

    enable_resp.transaction_id = 
        p_req->transaction_id;
    SON_MEMCPY(&(enable_resp.enable_res.cell_status_list[0].cgi), 
            &p_req->enable_req.src_cgi, 
        sizeof(son_intra_rat_global_cell_id_t));

    p_cell_id = mlb_get_cell_id_from_mif_msgs(p_cspl_hdr->from, p_cspl_hdr->api, p_msg);
    /*coverty_CID_24841 fix start*/
    if (SON_PNULL != p_cell_id)
    {
        /*coverty_CID_24841 fix end*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Unexpected Msg  %d received for "
            "cell %x in %s state from %d. SONMLB_ENABLE_RES "
            "Msg sent to %d with Error Code: %d ",
            p_cspl_hdr->api,
            son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity),
            SON_MLB_FSM_STATES_NAMES[g_mlb_context.current_fsm_state],
            p_cspl_hdr->from, p_cspl_hdr->to,
            enable_resp.enable_res.cell_status_list[0].error_code);
    /*coverty_CID_24841 fix start*/
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Cell is not configured for received msg");
    }

    /*coverty_CID_24841 fix end*/

    son_create_send_buffer((son_u8 *)&enable_resp, SON_MLB_MODULE_ID,
            (son_module_id_et)p_cspl_hdr->from, SONMLB_ENABLE_RES,
            sizeof(son_mlb_enable_res_t));
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_nack_enable_req_msg */

/*****************************************************************************
 * Function Name  : mlb_fsm_nack_disable_req_msg
 * Inputs         : p_msg   -     Pointer to the message received
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the function to handle unexpected messages in a MLB state
 ******************************************************************************/
son_buf_retain_status_et
mlb_fsm_nack_disable_req_msg
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr
 )
{
    SON_UT_TRACE_ENTER();
    son_mlb_disable_res_t disable_resp = {0};
    son_mlb_disable_req_t *p_req = (son_mlb_disable_req_t *)p_msg;
    son_intra_rat_global_cell_id_t *p_cell_id = SON_PNULL;

    disable_resp.disable_res.cell_status_list_size = 1;
    disable_resp.disable_res.result = SON_FAILURE;
    disable_resp.disable_res.error_code = SON_NO_ERROR;
    disable_resp.disable_res.cell_status_list[0].error_code = 
        SON_ERR_ALREADY_DISABLED;

    if (MLB_STATE_INIT == g_mlb_context.current_fsm_state)
    {
        disable_resp.disable_res.cell_status_list[0].error_code = 
            SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE;
    }
    else if (MLB_STATE_ENABLING == g_mlb_context.current_fsm_state)
    {
        disable_resp.disable_res.cell_status_list[0].error_code = 
            SON_ERR_ENABLE_IN_PROGRESS;
    }
    else if (MLB_STATE_DISABLING == g_mlb_context.current_fsm_state)
    {
        disable_resp.disable_res.cell_status_list[0].error_code = 
            SON_ERR_DISABLE_IN_PROGRESS;
    }

    disable_resp.transaction_id = p_req->transaction_id;
    SON_MEMCPY(&(disable_resp.disable_res.cell_status_list[0].cgi), 
            &p_req->disable_req.src_cgi, 
        sizeof(son_intra_rat_global_cell_id_t));

    p_cell_id = mlb_get_cell_id_from_mif_msgs(p_cspl_hdr->from, p_cspl_hdr->api, p_msg);
    /*coverty_CID_24840 fix start*/
    if (SON_PNULL != p_cell_id)
    {
        /*coverty_CID_24840 fix end*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Unexpected Msg  %d received for "
            "cell %x in %s state from %d. SONMLB_DISABLE_RES"
            "Msg sent to %d with Error Code: %d ",
            p_cspl_hdr->api,
            son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity),
            SON_MLB_FSM_STATES_NAMES[g_mlb_context.current_fsm_state],
            p_cspl_hdr->from, p_cspl_hdr->to,
            disable_resp.disable_res.cell_status_list[0].error_code);
    /*coverty_CID_24840 fix start*/
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Cell is not configured for received msg");
    }

    /*coverty_CID_24840 fix end*/

    son_create_send_buffer((son_u8 *)&disable_resp, SON_MLB_MODULE_ID,
            (son_module_id_et)p_cspl_hdr->from, SONMLB_DISABLE_RES,  /*coverity_CID_24962 fix start-end*/
            sizeof(son_mlb_disable_res_t));
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_nack_disable_req_msg */

/*****************************************************************************
 * Function Name  : mlb_fsm_nack_init_config_req_msg
 * Inputs         : p_msg   -     Pointer to the message received
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the function to handle unexpected messages in a MLB state
 ******************************************************************************/
son_buf_retain_status_et
mlb_fsm_nack_init_config_req_msg
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr
 )
{
    SON_UT_TRACE_ENTER();
    smif_init_config_res_t init_resp = {0};
    smif_init_config_req_t *p_req = (smif_init_config_req_t *)p_msg;

    init_resp.result = SON_FAILURE;
    init_resp.error_code = SON_ERR_UNEXPECTED_MSG;
    init_resp.transaction_id = p_req->transaction_id;

    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Unexpected Msg  %d received "
            "in %s state from %d. SON_OAM_INIT_CONFIG_RESP"
            "Msg sent to %d with Error Code: %d",
            p_cspl_hdr->api,
            SON_MLB_FSM_STATES_NAMES[g_mlb_context.current_fsm_state],
            p_cspl_hdr->from, p_cspl_hdr->to,
            init_resp.error_code);

    son_create_send_buffer((son_u8 *)&init_resp, SON_MLB_MODULE_ID,
            (son_module_id_et)p_cspl_hdr->from, SON_OAM_INIT_CONFIG_RESP,   /*coverity_CID_24961 fix start-end*/
            sizeof(smif_init_config_res_t));
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_nack_init_config_req_msg */

/*****************************************************************************
 * Function Name  : mlb_fsm_nack_modify_req_msg
 * Inputs         : p_msg  -      Pointer to the message received
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the function to handle unexpected messages in a MLB state
 ******************************************************************************/
son_buf_retain_status_et
mlb_fsm_nack_modify_req_msg
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr
 )
{
    SON_UT_TRACE_ENTER();
    son_mlb_modify_res_t modify_resp = {0};
    son_mlb_modify_req_t *p_req = (son_mlb_modify_req_t *)p_msg;
    son_intra_rat_global_cell_id_t *p_cell_id = SON_PNULL;

    modify_resp.modify_attributes_res.result = SON_FAILURE;

    if (MLB_STATE_INIT == g_mlb_context.current_fsm_state)
    {
        modify_resp.modify_attributes_res.error_code = 
            SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE;
    }
    else
    {
        modify_resp.modify_attributes_res.error_code = 
            SON_ERR_UNEXPECTED_MSG;
    }

    modify_resp.transaction_id = p_req->transaction_id;
    SON_MEMCPY(&(modify_resp.modify_attributes_res.srv_cgi), 
            &p_req->modify_attributes_req.srv_cgi, 
        sizeof(son_intra_rat_global_cell_id_t));
    p_cell_id = mlb_get_cell_id_from_mif_msgs(p_cspl_hdr->from, p_cspl_hdr->api, p_msg);
    /*coverty_CID_24843 fix start*/
    if (SON_PNULL != p_cell_id)
    {
        /*coverty_CID_24843 fix end*/
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
                "Unexpected Msg  %d received for "
                "cell %x in %s state from %d. SONMLB_MODIFY_ATTRIBUTES_RES"
                "Msg sent to %d with Error Code: %d",
                p_cspl_hdr->api,
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity),
                SON_MLB_FSM_STATES_NAMES[g_mlb_context.current_fsm_state],
                p_cspl_hdr->from, p_cspl_hdr->to,
                modify_resp.modify_attributes_res.error_code);
    /*coverty_CID_24843 fix start*/
    }
    else
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
                "Cell is not configured for received msg");
    }

        /*coverty_CID_24843 fix end*/

    son_create_send_buffer((son_u8 *)&modify_resp, SON_MLB_MODULE_ID,
            (son_module_id_et)p_cspl_hdr->from, SONMLB_MODIFY_ATTRIBUTES_RES,
            sizeof(son_mlb_modify_res_t));
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_fsm_nack_modify_req_msg */

/*****************************************************************************
 * Function Name  : mlb_fsm_ignore_msg
 * Inputs         : p_msg    -    Pointer to the message received
 *                  p_cspl_hdr  - Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the function to handle unexpected messages in a MLB state
 ******************************************************************************/
son_buf_retain_status_et
mlb_fsm_ignore_msg
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr
 )
{
    /* Rel 3.0 Cell delete support changes Start */
    son_rrm_deregistration_res_t *p_res = SON_PNULL;
    /* Rel 3.0 Cell delete support changes Stop */
    SON_UT_TRACE_ENTER();

    /* Rel 3.0 Cell delete support changes Start */
    if (RRM_SON_DEREGISTER_RESP == p_cspl_hdr->api)
    {
        p_res = (son_rrm_deregistration_res_t *)p_msg;

        /* Find the transaction Id received in De-register response in
           temporary transaction Id list stored during cell deletion.
           If found remove it from temporary transaction Id list and
           do not process the corresponding De-register response as the
           cell is already deleted */
        if (SON_SUCCESS == 
                son_find_txn_id_in_temp_txn_id_list(p_res->transaction_id))
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, 
                    SON_INFO,
                    "RRM_SON_DEREGISTER_RESP "
                    "received for the Cell 0x%x already deleted "
                    "at MLB due to cell delete request from OAM so ignore it",
                    son_convert_char_cell_id_to_int_cell_id(
                        p_res->res.deregistration_resp.id.object_id.
                        cell_id.cell_identity));
            SON_UT_TRACE_EXIT();
            return RELEASE_SON_BUFFER;
        }
    }
    /* Rel 3.0 Cell delete support changes Stop */
    
    SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_WARNING,
            "Unexpected Message %d "
            "Received from module %d in MLB global FSM state %s",
            p_cspl_hdr->api, p_cspl_hdr->from,
            SON_MLB_FSM_STATES_NAMES[g_mlb_context.current_fsm_state]);
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : mlb_parse_rrm_messages
 * Inputs         : p_msg    -    Pointer to the message received
 *                  api_id    -   api_id
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is function to parse the messages received from RRM
 ******************************************************************************/
static son_u8 *
mlb_parse_rrm_messages
(
    son_u8 *p_msg,
    son_u16 api_id
 )
{
    son_u8 *p_parsed_msg = SON_PNULL;
    son_s32 buf_len = 0;
    S32 length_read = 0;
    son_bool_et txn_id_available = SON_TRUE;

    if (SON_PNULL == p_msg)
    {
        return p_parsed_msg;
    }

    buf_len = son_get_word_from_header(&p_msg[EXT_MSG_BUF_LEN_OFFSET]);
    switch (api_id)
    {
        case RRM_SON_REGISTER_RESP:
            {
                p_parsed_msg = son_mem_get(sizeof(son_rrm_registration_res_t));
                if (SON_PNULL != p_parsed_msg)
                {
                    if (RRM_FAILURE == rrm_parse_rrm_son_registration_res(
                    &((son_rrm_registration_res_t *)p_parsed_msg)->res,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                    &length_read))
                {
                        SON_LOG(mlb_get_log_mode(),
                                p_son_mlb_facility_name, SON_ERROR,
                                "RRM->MLB:RRM_SON_REGISTER_RESP parsing failed");
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                    txn_id_available = SON_FALSE;
                }
                    else
                    {

                        SON_LOG(mlb_get_log_mode(),
                                p_son_mlb_facility_name, SON_INFO,
                                "RRM->MLB:RRM_SON_REGISTER_RESP response %u, error_code %u",
                                ((son_rrm_registration_res_t *)p_parsed_msg)->res.registration_resp.result,
                                ((son_rrm_registration_res_t *)p_parsed_msg)->res.registration_resp.error_code);
                    }
                }
                else
                {
                    SON_LOG(mlb_get_log_mode(),
                            p_son_mlb_facility_name, SON_ERROR,
                            "Memory allocation failed for p_parsed_msg."
                            " RRM_SON_REGISTER_RESP cannot be decoded");
                }
            break;
        }

        case RRM_SON_DEREGISTER_RESP:
            {
                p_parsed_msg = son_mem_get(sizeof(son_rrm_deregistration_res_t));
                if (SON_PNULL != p_parsed_msg)
                {
                    if (RRM_FAILURE == rrm_parse_rrm_son_deregistration_res(
                    &((son_rrm_deregistration_res_t *)p_parsed_msg)->res,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                    &length_read))
                {
                        SON_LOG(mlb_get_log_mode(),
                                p_son_mlb_facility_name, SON_ERROR,
                                "RRM->MLB:RRM_SON_DEREGISTER_RESP parsing failed");
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                    txn_id_available = SON_FALSE;
                }
                    else
                    {
                        SON_LOG(mlb_get_log_mode(),
                                p_son_mlb_facility_name, SON_INFO,
                                "RRM->MLB:RRM_SON_DEREGISTER_RESP response %u, error_code %u",
                                ((son_rrm_deregistration_res_t *)p_parsed_msg)->res.deregistration_resp.result,
                                ((son_rrm_deregistration_res_t *)p_parsed_msg)->res.deregistration_resp.error_code);
                    }
                }
                else
                {
                    SON_LOG(mlb_get_log_mode(),
                            p_son_mlb_facility_name, SON_ERROR,
                            "Memory allocation failed for p_parsed_msg."
                            " RRM_SON_DEREGISTER_RESP cannot be decoded");
                }
            break;
        }

        case RRM_SON_CELL_STATE_CHANGE_IND:
            {
                txn_id_available = SON_FALSE;
                p_parsed_msg = son_mem_get(
                        sizeof(son_cell_state_change_indication_t));
                if (SON_PNULL != p_parsed_msg)
                {
                    if (RRM_FAILURE == rrm_parse_rrm_son_cell_state_change_ind(
                            (son_cell_state_change_indication_t *)p_parsed_msg,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                    &length_read))
                {
                        SON_LOG(mlb_get_log_mode(),
                                p_son_mlb_facility_name, SON_ERROR,
                                "RRM->MLB:RRM_SON_CELL_STATE_CHANGE_IND parsing failed");
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                }
                    else
                    {
                        SON_LOG(mlb_get_log_mode(),
                                p_son_mlb_facility_name, SON_INFO,
                                "RRM->MLB:RRM_SON_CELL_STATE_CHANGE_IND");
                    }
                }
                else
                {
                    SON_LOG(mlb_get_log_mode(),
                            p_son_mlb_facility_name, SON_ERROR,
                            "Memory allocation failed for p_parsed_msg."
                            " RRM_SON_CELL_STATE_CHANGE_IND cannot be decoded");
                }
            break;
        }

        case RRM_SON_HO_REPORT:
            {
                txn_id_available = SON_FALSE;
                p_parsed_msg = son_mem_get(sizeof(rrm_son_ho_report_t));
                if (SON_PNULL != p_parsed_msg)
                {
                    if (RRM_FAILURE == rrm_parse_rrm_son_ho_report(
                    (rrm_son_ho_report_t *)p_parsed_msg,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                    &length_read))
                {
                        SON_LOG(mlb_get_log_mode(),
                                p_son_mlb_facility_name, SON_ERROR,
                                "RRM->MLB:RRM_SON_HO_REPORT parsing failed");
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                }
                    else
                    {
                        SON_LOG(mlb_get_log_mode(),
                                p_son_mlb_facility_name, SON_INFO,
                                "RRM->MLB:RRM_SON_HO_REPORT");
                    }
                }
                else
                {
                    SON_LOG(mlb_get_log_mode(),
                            p_son_mlb_facility_name, SON_ERROR,
                            "Memory allocation failed for p_parsed_msg."
                            " RRM_SON_HO_REPORT cannot be decoded");
                }
            break;
        }

        case RRM_SON_LOAD_REPORT_IND:
            {
                txn_id_available = SON_FALSE;
                p_parsed_msg = son_mem_get(sizeof(rrm_son_load_report_ind_t));
                if (SON_PNULL != p_parsed_msg)
                {
                    if (RRM_FAILURE == rrm_parse_rrm_son_load_report_ind(
                     (rrm_son_load_report_ind_t *)p_parsed_msg,
                                                                      (p_msg + EXT_MSG_API_HDR_SIZE),
                                                                      buf_len - EXT_MSG_API_HDR_SIZE,
                                &length_read))
                {
                        SON_LOG(mlb_get_log_mode(),
                                p_son_mlb_facility_name, SON_ERROR,
                                "RRM->MLB:RRM_SON_LOAD_REPORT_IND parsing failed");
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                }
                    else
                    {
                        SON_LOG(mlb_get_log_mode(),
                                p_son_mlb_facility_name, SON_INFO,
                                "RRM->MLB:RRM_SON_LOAD_REPORT_IND");
                    }
                }
                else
                {
                    SON_LOG(mlb_get_log_mode(),
                            p_son_mlb_facility_name, SON_ERROR,
                            "Memory allocation failed for p_parsed_msg."
                            " RRM_SON_LOAD_REPORT_IND cannot be decoded");
                }
            break;
        }
/* eICIC feature code Starts */
        case RRM_SON_EICIC_ENABLE_DISABLE_IND:
            {   
                txn_id_available = SON_FALSE;
                p_parsed_msg = son_mem_get(sizeof(rrm_son_eicic_enable_disable_ind_t));
                if(SON_PNULL != p_parsed_msg)
                {
                    if(RRM_FAILURE == rrm_parse_rrm_son_eicic_enable_disable_ind (
                                        (rrm_son_eicic_enable_disable_ind_t *)p_parsed_msg,
                                        (p_msg + EXT_MSG_API_HDR_SIZE),
                                        buf_len - EXT_MSG_API_HDR_SIZE,
                                &length_read))
                {
                        SON_LOG(mlb_get_log_mode(),
                                p_son_mlb_facility_name, SON_ERROR,
                                "RRM->MLB:RRM_SON_EICIC_ENABLE_DISABLE_IND parsing failed");
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                }
                    else
                    {
                        SON_LOG(mlb_get_log_mode(),
                                p_son_mlb_facility_name, SON_INFO,
                                "RRM->MLB:RRM_SON_EICIC_ENABLE_DISABLE_IND");
                    }
                }
                else
                {
                    SON_LOG(mlb_get_log_mode(),
                            p_son_mlb_facility_name, SON_ERROR,
                            "Memory allocation failed for p_parsed_msg."
                            " RRM_SON_EICIC_ENABLE_DISABLE_IND cannot be decoded");
                }
                break;
            }
/* eICIC feature code Ends */

        default:
        {
            SON_LOG(mlb_get_log_mode(),
                p_son_mlb_facility_name, SON_ERROR,
                        "Unexpected Message %d"
                        "received from RRM", api_id);
            break;
    }
    } /* switch */

    if (SON_PNULL != p_parsed_msg && SON_TRUE == txn_id_available)
    {
        *(son_u16 *)p_parsed_msg = son_get_word_from_header(
                &p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
    }

    return p_parsed_msg;
} /* mlb_parse_rrm_messages */

/*****************************************************************************
 * Function Name  : mlb_process_rrm_msgs
 * Inputs         : p_msg   -     Pointer to the message received
 *                : p_cell_id  -  Pointer to Cell ID
 *                : p_cspl_hdr -  Pointer to cspl stack header
 *                : event_id   -  event id
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This function handle messages from RRM module.
 *****************************************************************************/
static son_buf_retain_status_et
mlb_process_rrm_msgs
(
    son_u8                          *p_msg,
    son_intra_rat_global_cell_id_t  *p_cell_id,
    STACKAPIHDR                     *p_cspl_hdr,
    son_mlb_index_et                event_id
 )
{
    mlb_cell_context_t          *p_cell_ctxt      = SON_PNULL;
    son_buf_retain_status_et    ret               = RELEASE_SON_BUFFER;

    SON_UT_TRACE_ENTER();

    /* Rel 3.0 Cell delete support changes Start */
    /* Find the transaction Id received in De-register response in
       temporary transaction Id list stored during cell deletion.
       If found remove it from temporary transaction Id list and
       do not process the corresponding De-register response as the
       cell is already deleted */
    if (RRM_SON_DEREGISTER_RESP == p_cspl_hdr->api)
    {
        son_rrm_deregistration_res_t *p_res = (son_rrm_deregistration_res_t *)p_msg;
        if (SON_SUCCESS == 
                son_find_txn_id_in_temp_txn_id_list(p_res->transaction_id))
        {
            SON_LOG(mlb_get_log_mode(),
                    p_son_mlb_facility_name, SON_BRIEF,
                    "RRM_SON_DEREGISTER_RESP "
                    "received for the Cell 0x%x already deleted "
                    "at MLB due to cell delete request from OAM so ignore it",
                    son_convert_char_cell_id_to_int_cell_id(
                        p_res->res.deregistration_resp.id.object_id.
                        cell_id.cell_identity));
            SON_UT_TRACE_EXIT();
            return ret;
        }
    }
    /* Rel 3.0 Cell delete support changes Stop */

    p_cell_ctxt = mlb_get_cell_context_from_global_context(p_cell_id);
    if (!p_cell_ctxt)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
                "Cell context not found! for Msg : "
            "%s", SON_MLB_CELL_FSM_EVENT_NAMES[event_id] );
    }
    else
    {
        ret = mlb_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, p_msg);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_process_rrm_msgs */

/*****************************************************************************
 * Function Name  : mlb_get_cell_id_from_rrm_msgs
 * Inputs         : from -    source module id
 *                : api  -    procedure code of the message
 *                : p_msg -   pointer to the message
 * Returns        : son_intra_rat_global_cell_id_t*
 * Description    : This function determines the cell_id in the rrm messages
 ****************************************************************************/
static son_intra_rat_global_cell_id_t *
mlb_get_cell_id_from_rrm_msgs
(
    son_u16 from,
    son_u16 api,
    son_u8 *p_msg
 )
{
    son_intra_rat_global_cell_id_t *p_cell_id = SON_PNULL;

    switch (api)
    {
        case RRM_SON_REGISTER_RESP:
        {
            p_cell_id =
                (son_intra_rat_global_cell_id_t *)
                &((son_rrm_registration_res_t *)p_msg)->
                res.registration_resp.id.object_id.cell_id;
            break;
        }

        case RRM_SON_DEREGISTER_RESP:
        {
            p_cell_id =
                (son_intra_rat_global_cell_id_t *)
                &((son_rrm_deregistration_res_t *)p_msg)->
                res.deregistration_resp.id.object_id.cell_id;
            break;
        }

        case RRM_SON_CELL_STATE_CHANGE_IND:
        {
            p_cell_id =
                (son_intra_rat_global_cell_id_t *)
                &((son_cell_state_change_indication_t *)p_msg)->cell_id;
            break;
        }

        case RRM_SON_HO_REPORT:
        {
            p_cell_id =
                (son_intra_rat_global_cell_id_t *)
                &((rrm_son_ho_report_t *)p_msg)->src_cgi;
            break;
        }

        case RRM_SON_LOAD_REPORT_IND:
        {
            p_cell_id =
                (son_intra_rat_global_cell_id_t *)
                &(((rrm_son_load_report_ind_t *)p_msg)->serv_cell_load_info.g_cell_id);
            break;
        }
        /** eICIC feature code Starts */
        case RRM_SON_EICIC_ENABLE_DISABLE_IND:
        {
            p_cell_id =
                (son_intra_rat_global_cell_id_t *)
                &(((rrm_son_eicic_enable_disable_ind_t*)p_msg)->cell_id);
            break;
        }
        /** eICIC feature code Ends */

        default:
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_ERROR,
                    "Invalid api[%d] received"
                    " from %d", api, from);
            break;
    }
    } /* switch */

    return p_cell_id;
} /* mlb_get_cell_id_from_rrm_msgs */

/*****************************************************************************
 * Function Name  : mlb_handle_rrm_messages
 * Inputs         : p_msg   -     Pointer to the message received
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the RRM message handler for MLB module
 ******************************************************************************/
son_buf_retain_status_et
mlb_handle_rrm_messages
(
    son_u8         *p_msg,
    STACKAPIHDR    *p_cspl_hdr
 )
{
    son_buf_retain_status_et        ret = RELEASE_SON_BUFFER;
    son_u8                         *p_parsed_msg = SON_PNULL;
    son_mlb_index_et               event_id = CELL_SON_INVALID_EVENT;
    son_intra_rat_global_cell_id_t *p_cell_id = SON_PNULL;
    son_oam_log_on_off_et          log_mode = mlb_get_log_mode();

    SON_UT_TRACE_ENTER();

    event_id = rrm_get_cell_event_id(p_cspl_hdr->api);
    p_parsed_msg = mlb_parse_rrm_messages(p_msg,
            p_cspl_hdr->api);
    if (SON_PNULL != p_parsed_msg)
    {
        p_cell_id = mlb_get_cell_id_from_rrm_msgs(p_cspl_hdr->from,
                p_cspl_hdr->api,
                p_parsed_msg);

        ret = mlb_process_rrm_msgs(p_parsed_msg,
                p_cell_id,
                p_cspl_hdr,
                event_id);
        /* Free the Memory Allocated in PUP Tool parsing */
        son_mem_free(p_parsed_msg);
    }
    else
    {
        SON_LOG(log_mode, p_son_mlb_facility_name,
                SON_ERROR,   /*coveriyt_CID_21643 fix start-end*/
                "Parsing failed for "
                "received message: %d", p_cspl_hdr->api);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_handle_rrm_messages */

/*****************************************************************************
 * Function Name  : mlb_cell_info_ind_handler
 * Inputs         : p_buf  -      Pointer to the message receiveda
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLb module and is called when
 * SMIF_CELL_INFO_IND message is received.
 ******************************************************************************/
static son_buf_retain_status_et
mlb_cell_info_ind_handler
(
    son_u8      *p_buf,
    STACKAPIHDR *p_cspl_hdr
 )
{
    son_cell_info_ind_t      *p_msg = (son_cell_info_ind_t *)p_buf;
    son_mlb_enable_req_t     enable_req = {0};
    mlb_cell_context_node_t  *p_cell_ctxt = SON_PNULL;
    son_u8                   idx = 0;
    son_cell_status_t        cell_sts;
    son_feature_state_change_cause_et cause = SON_STATE_CHANGE_CAUSE_UNDEFINED;
    son_feature_state_et state = SON_STATE_UNDEFINED;
    son_oam_log_on_off_et    log_mode = mlb_get_log_mode();
    /*SPR_19279_START */
    son_u8 cell_index = *((son_u8 *)(p_buf + sizeof(son_cell_info_ind_t)));
    /*SPR_19279_END */

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&cell_sts, 0, sizeof(son_cell_status_t));
    if (0 == p_msg->cell_info_size)
    {
        SON_LOG(log_mode, p_son_mlb_facility_name, SON_ERROR,
                "Cell list is empty");
    }
    else    
    {  
        /* Check MLB Global Mode */
        if (SON_ENABLED == g_mlb_context.mlb_global_mode)
        {
            mlb_init_pending_res(son_generate_txn_id(),
                    SMIF_CELL_INFO_IND);

            for (idx = 0; idx < p_msg->cell_info_size; idx++)
            {
                /*SPR-11554-Start*/
                p_cell_ctxt = mlb_add_new_cell_in_context(
                        &p_msg->serving_cell[idx],
                /*SPR_19279_START */
                        &state, &cause, cell_index);
                /*SPR_19279_END */
                /*SPR-11554-End*/

                if (SON_PNULL != p_cell_ctxt)
                {
                    cell_sts.error_code = SON_ERR_TIMER_EXPIRED;
                    SON_MEMCPY(&cell_sts.cgi,
                            &p_msg->serving_cell[idx].src_cgi,
                        sizeof(son_intra_rat_global_cell_id_t));

                    if (SON_TRUE == mlb_add_cell_in_pending_res(&cell_sts))
                    {
                        enable_req.transaction_id = 
                            mlb_get_pending_res_transaction_id();
                        enable_req.enable_req.bitmask |= 
                            SON_OAM_MLB_ENABLE_SRC_CGI_PRESENT;

                        SON_MEMCPY(&enable_req.enable_req.src_cgi,
                                &p_msg->serving_cell[idx].src_cgi,
                            sizeof(son_intra_rat_global_cell_id_t));

                        mlb_cell_process_msg(CELL_SONMLB_ENABLE_REQ,
                                &p_cell_ctxt->data,
                                p_cspl_hdr,
                            (son_u8 *)&enable_req);
                    }
                    else
                    {
                        SON_LOG(log_mode, p_son_mlb_facility_name, SON_ERROR,
                                "Cell 0x%x could not "
                                "be added to pending response list",
                                son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->data.cell_id.
                                cell_identity));
                    }
                }
                else
                {
                    /* Send SONMLB_FEATURE_STATE_CHANGE_IND */
                    mlb_send_feature_state_change_ind(
                            &p_msg->serving_cell[idx].src_cgi,
                            state, cause);
                }
            }

            if (0 == mlb_get_pending_res_expected_count())
            {
                mlb_reset_pending_resp_list();
            }
            else
            {
                /* Set MLB global FSM state to MLB_STATE_ENABLING
                 * as at least 1 registration request has been sent */
                mlb_set_context_state(MLB_STATE_ENABLING);
                g_mlb_context.mlb_prending_res_timer_id = son_start_timer(
                        MLB_DEFULT_PENDING_RESPONSE_TIMER, SON_PNULL, SON_NULL,
                        SON_FALSE);
            }
        }
        else
        {
            /* Add cell in MLB cell configuration list if number of current
             * configured cells is below maximum number of configured cells
             * allowed at MLB and the cell is not already configured.
             * Cell must be added with MLB mode as disabled by default. */
            for (idx = 0; idx < p_msg->cell_info_size; idx++)
            {
                /*coverity_CID_25299 fix start-end */
                /*SPR-11554-Start*/
                mlb_add_new_cell_in_context(
                        &p_msg->serving_cell[idx], 
                /*SPR_19279_START */
                        &state, &cause,cell_index);
                /*SPR_19279_END */
                /*SPR-11554-End*/
                /* Send MLB_FEATURE_STATE_CHANGE_IND */
                mlb_send_feature_state_change_ind(
                        &p_msg->serving_cell[idx].src_cgi,
                        state, cause);
            }
        }
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_cell_info_ind_handler */

/******************************************************************************
 * Function Name  : mlb_anr_nbr_update_notification 
 * Inputs         : p_buf  -      Pointer to the message received
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is called when
 *                  nbr is deleted at the ANR and SONANR_UPDATE_NOTIFICATION message 
 *                  is received. 
 ******************************************************************************/
son_buf_retain_status_et
mlb_anr_nbr_update_notification
(
    son_u8         *p_buf,
    STACKAPIHDR    *p_cspl_hdr
 )
{
    son_buf_retain_status_et ret = RELEASE_SON_BUFFER;
    son_anr_updated_nrt_info_t   *p_msg = (son_anr_updated_nrt_info_t *)p_buf;
    mlb_cell_context_t           *p_cell_ctxt = SON_PNULL;
    son_mlb_index_et              event_id;

    SON_UT_TRACE_ENTER();

    p_cell_ctxt = mlb_get_cell_context_from_global_context(&p_msg->src_cgi);
    event_id = son_mlb_get_cell_event_id(p_cspl_hdr->api);
    if (!p_cell_ctxt)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
                "Cell context not found! for Msg : "
            "%s", SON_MLB_CELL_FSM_EVENT_NAMES[event_id] );
    }
    else
    {
        ret = mlb_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, (son_u8 *)p_msg);
    }

    SON_UT_TRACE_EXIT();
    return ret;  /*coverity_CID_21915 fix start-end*/
} /* mlb_anr_nbr_update_notification */

/******************************************************************************
 * Function Name  : mlb_global_sonanr_disable_ind_handler
 * Inputs         : p_buf   -     Pointer to the message received
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is called when
 *                  SONANR_DISABLE_IND message is received. 
 ******************************************************************************/
son_buf_retain_status_et
mlb_global_sonanr_disable_ind_handler
(
    son_u8         *p_buf,
    STACKAPIHDR    *p_cspl_hdr
 )
{
    son_buf_retain_status_et ret = RELEASE_SON_BUFFER;
    son_anr_disable_ind_t        *p_msg = (son_anr_disable_ind_t *)p_buf;
    mlb_cell_context_t           *p_cell_ctxt = SON_PNULL;
    son_mlb_index_et              event_id;

    SON_UT_TRACE_ENTER();

    p_cell_ctxt = mlb_get_cell_context_from_global_context(&p_msg->src_cgi);
    event_id = son_mlb_get_cell_event_id(p_cspl_hdr->api);
    if (!p_cell_ctxt)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
                "Cell context not found! for Msg : "
            "%s", SON_MLB_CELL_FSM_EVENT_NAMES[event_id] );
    }
    else
    {
        ret = mlb_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, (son_u8 *)p_msg);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_global_sonanr_disable_ind_handler */

/*****************************************************************************
 * Function Name  : mlb_sonmlb_modify_req
 * Inputs         : p_buf   -     Pointer to the message received
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is called when
 *                  SONMLB_MODIFY_REQ message is received from OAM. 
 ******************************************************************************/
son_buf_retain_status_et
mlb_sonmlb_modify_req
(
    son_u8         *p_buf,
    STACKAPIHDR    *p_cspl_hdr
 )
{
    son_buf_retain_status_et ret = RELEASE_SON_BUFFER;
    son_mlb_modify_req_t      *p_msg = (son_mlb_modify_req_t *)p_buf;
    mlb_cell_context_t        *p_cell_ctxt = SON_PNULL;

    son_mlb_index_et          event_id;

    SON_UT_TRACE_ENTER();

    event_id = son_mlb_get_cell_event_id(p_cspl_hdr->api);
    p_cell_ctxt = mlb_get_cell_context_from_global_context(&p_msg->modify_attributes_req.srv_cgi);

    if (!p_cell_ctxt)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Cell context not found! for Msg : "
            "%s", SON_MLB_CELL_FSM_EVENT_NAMES[event_id] );

        son_mlb_modify_res_t config_params_res;
        son_size_t config_params_res_len =
            sizeof(son_mlb_modify_res_t);
        SON_MEMSET(&config_params_res, 0, config_params_res_len);
        config_params_res.transaction_id = p_msg->transaction_id;
        SON_MEMCPY(&config_params_res.modify_attributes_res.srv_cgi,
                &p_msg->modify_attributes_req.srv_cgi,
                sizeof(son_intra_rat_global_cell_id_t));
        config_params_res.modify_attributes_res.result = SON_FAILURE;
        config_params_res.modify_attributes_res.error_code =
            SON_ERR_CELL_UNCONFIGURED;
        son_create_send_buffer((son_u8 *)&config_params_res,
                SON_MLB_MODULE_ID, SON_MIF_MODULE_ID,
                SONMLB_MODIFY_ATTRIBUTES_RES, config_params_res_len);
    }
    else
    {
        /*SONMLB-- Centralized support start*/
        if (SON_OAM_MLB_MODE_PRESENT & p_msg->modify_attributes_req.bitmask)
        {
            g_mlb_context.mlb_config_param.mlb_mode = p_msg->modify_attributes_req.mlb_mode;
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_BRIEF,
                "SON is working in [%d] mode"
                , g_mlb_context.mlb_config_param.mlb_mode);
        }

        /*SONMLB-- Centralized support end*/
        ret = mlb_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, (son_u8 *)p_msg);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_sonmlb_modify_req */

/*****************************************************************************
 * Function Name  : mlb_get_cell_id_from_mif_msgs
 * Inputs         : from  -   Source module id
 *                : api   -   Procedure code
 *                : p_msg  -  Pointer to the message
 * Returns        : son_intra_rat_global_cell_id_t*
 * Description    : This function determines the cell_id in the mif  message
 ****************************************************************************/
son_intra_rat_global_cell_id_t *
mlb_get_cell_id_from_mif_msgs
(
    son_u16 from,
    son_u16 api,
    son_u8 *p_msg
 )
{
    son_intra_rat_global_cell_id_t *p_cell_id = SON_PNULL;

    switch (api)
    {
        case SONMLB_ENABLE_REQ:
            {
                if (SON_OAM_MLB_ENABLE_SRC_CGI_PRESENT == 
                        ((son_mlb_enable_req_t *)p_msg)->
                        enable_req.bitmask)
                {
                    p_cell_id =
                        &((son_mlb_enable_req_t *)p_msg)->
                        enable_req.src_cgi;
                }

            break;
        }

        case SONMLB_DISABLE_REQ:
            {
                if (SON_OAM_MLB_DISABLE_SRC_CGI_PRESENT == 
                        ((son_mlb_disable_req_t *)p_msg)->
                        disable_req.bitmask)
                {
                    p_cell_id =
                        &((son_mlb_disable_req_t *)p_msg)->
                        disable_req.src_cgi;
                }

            break;
        }

        case SONMLB_MOBILITY_CHANGE_REQ:
        {
            p_cell_id =
                &((mlb_mobility_change_req_t *)p_msg)->
                    src_cgi2;
            break;
        }

        case SONMLB_MOBILITY_CHANGE_ACK:
        {
            p_cell_id =
                &((sonmlb_mobility_change_ack_t *)p_msg)->
                src_cgi1;
            break;
        }

        case SONMLB_MOBILITY_CHANGE_FAILURE:
        {
            p_cell_id =
                &((sonmlb_mobility_change_failure_t *)p_msg)->
                src_cgi1;
            break;
        }

/*Rel 3.0 Multi Sector code changes Start*/
        case SONMLB_INTERNAL_MOBILITY_CHANGE_REQ:
        {
            p_cell_id =
                &((mlb_mobility_change_req_t *)p_msg)->
                    src_cgi2;
            break;
        }

        case SONMLB_INTERNAL_MOBILITY_CHANGE_ACK:
        {
            p_cell_id =
                &((sonmlb_mobility_change_ack_t *)p_msg)->
                src_cgi1;
            break;
        }

        case SONMLB_INTERNAL_MOBILITY_CHANGE_FAILURE:
        {
            p_cell_id =
                &((sonmlb_mobility_change_failure_t *)p_msg)->
                src_cgi1;
            break;
        }
/*Rel 3.0 Multi Sector code changes Stop*/

        case SONMLB_FEATURE_STATE_CHANGE_IND: 
            {
                p_cell_id =
                &((son_mlb_feature_state_change_ind_t *)p_msg)->
                    srv_cgi;
            break;
        }

        case SONMLB_MODIFY_ATTRIBUTES_REQ:
        {
            p_cell_id =
                &((son_mlb_modify_req_t *)p_msg)->
                    modify_attributes_req.srv_cgi;
            break;
        }

        case SONMLB_MODIFY_ATTRIBUTES_RES:
            {
                p_cell_id =
                &((son_mlb_modify_res_t *)p_msg)->
                    modify_attributes_res.srv_cgi;
            break;
        }

        default:
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name,
                    SON_ERROR,
                    "Invalid api[%d] received"
                    " from %d", api, from);
            break;
    }
    } /* switch */

    return p_cell_id; 
} /* mlb_get_cell_id_from_mif_msgs */

/*****************************************************************************
 * Function Name  : mlb_mobility_change_req_handler 
 * Inputs         : p_buf   -     Pointer to the message received
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is called when
 *                  SONMLB_MOBILITY_CHANGE_REQ and SONMLB_INTERNAL_MOBILITY_CHANGE_REQ 
 *                  message is received from X2 and MLB of other cell. 
 ******************************************************************************/
son_buf_retain_status_et
mlb_mobility_change_req_handler
(
    son_u8         *p_msg,
    STACKAPIHDR    *p_cspl_hdr
 )
{
    son_buf_retain_status_et        ret = RELEASE_SON_BUFFER;
    mlb_cell_context_t              *p_cell_ctxt = SON_PNULL;

    son_mlb_index_et                 event_id = CELL_SON_INVALID_EVENT;
    son_intra_rat_global_cell_id_t   p_cell_id;

    SON_UT_TRACE_ENTER();

    mlb_mobility_change_req_t  *req = (mlb_mobility_change_req_t *)p_msg;

    SON_MEMCPY(&p_cell_id, &req->src_cgi2, sizeof(son_intra_rat_global_cell_id_t));
    event_id = son_mlb_get_cell_event_id(p_cspl_hdr->api);
    p_cell_ctxt = mlb_get_cell_context_from_global_context(&p_cell_id);

    if (!p_cell_ctxt)
        {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
                    "Cell context not found! for Msg : "
            "%s", SON_MLB_CELL_FSM_EVENT_NAMES[event_id] );

            mlb_create_send_generic_nack(
                    p_msg, 
                    p_cspl_hdr,
                    SON_ERR_CELL_UNCONFIGURED);
            ret  = RELEASE_SON_BUFFER;
        }
        else
        {
        ret = mlb_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, p_msg);
        }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_mobility_change_req_handler */

/*****************************************************************************
 * Function Name  : mlb_mobility_change_failure_handler 
 * Inputs         : p_buf   -     Pointer to the message received
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is called when
 *                  SONMLB_MOBILITY_CHANGE_FAILURE message is received from X2. 
 ******************************************************************************/
son_buf_retain_status_et
mlb_mobility_change_failure_handler
(
    son_u8         *p_msg,
    STACKAPIHDR    *p_cspl_hdr
 )
{
    son_buf_retain_status_et        ret = RELEASE_SON_BUFFER;
    mlb_cell_context_t              *p_cell_ctxt = SON_PNULL;

    son_mlb_index_et                event_id = CELL_SON_INVALID_EVENT;
    son_intra_rat_global_cell_id_t     p_cell_id;

    sonmlb_mobility_change_failure_t *req = (sonmlb_mobility_change_failure_t *)p_msg;

    SON_MEMCPY(&p_cell_id, &req->src_cgi1, sizeof(son_intra_rat_global_cell_id_t));

    SON_UT_TRACE_ENTER();

    event_id = son_mlb_get_cell_event_id(p_cspl_hdr->api);
    p_cell_ctxt = mlb_get_cell_context_from_global_context(&p_cell_id);

    if (!p_cell_ctxt)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Cell context not found! for Msg : "
            "%s", SON_MLB_CELL_FSM_EVENT_NAMES[event_id] );

        mlb_create_send_generic_nack(
            p_msg,
            p_cspl_hdr,
            SON_ERR_CELL_UNCONFIGURED);
        ret  = RELEASE_SON_BUFFER;
    }
    else
    {
        ret = mlb_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, p_msg);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_mobility_change_failure_handler */

/*****************************************************************************
 * Function Name  : mlb_mobility_change_ack_handler 
 * Inputs         : p_buf  -      Pointer to the message received
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is called when
 *                  SONMLB_MOBILITY_CHANGE_ACK message is received from X2. 
 ******************************************************************************/
son_buf_retain_status_et
mlb_mobility_change_ack_handler
(
    son_u8         *p_msg,
    STACKAPIHDR    *p_cspl_hdr
 )
{
    son_buf_retain_status_et    ret = RELEASE_SON_BUFFER;
    mlb_cell_context_t             *p_cell_ctxt = SON_PNULL;

    son_mlb_index_et            event_id = CELL_SON_INVALID_EVENT;

    son_intra_rat_global_cell_id_t     p_cell_id;
    sonmlb_mobility_change_ack_t *req = (sonmlb_mobility_change_ack_t *)p_msg;

    SON_MEMSET(&p_cell_id, 0, sizeof(son_intra_rat_global_cell_id_t));
    SON_MEMCPY(&p_cell_id, &req->src_cgi1, sizeof(son_intra_rat_global_cell_id_t));
    SON_UT_TRACE_ENTER();

    event_id = son_mlb_get_cell_event_id(p_cspl_hdr->api);
    p_cell_ctxt = mlb_get_cell_context_from_global_context(&p_cell_id);

    if (!p_cell_ctxt)
        {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
                    "Cell context not found! for Msg : "
            "%s", SON_MLB_CELL_FSM_EVENT_NAMES[event_id] );

            mlb_create_send_generic_nack(
                    p_msg, 
                    p_cspl_hdr,
                    SON_ERR_CELL_UNCONFIGURED);
            ret  = RELEASE_SON_BUFFER;
        }
        else
        {
        ret = mlb_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, p_msg);
        }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_mobility_change_ack_handler */

/*****************************************************************************
 * Function Name  : mlb_enable_req_handler
 * Inputs         : p_buf   -     Pointer to the message received
 *                  p_cspl_hdr  - Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is called when
 *                  SONMLB_ENABLE_REQ message is received from OAM. 
 ******************************************************************************/
son_buf_retain_status_et
mlb_enable_req_handler
(
    son_u8         *p_msg,
    STACKAPIHDR    *p_cspl_hdr
 )
{
    son_buf_retain_status_et    ret = RELEASE_SON_BUFFER;
    mlb_cell_context_t          *p_cell_ctxt = SON_PNULL;

    son_mlb_index_et            event_id = CELL_SON_INVALID_EVENT;
    son_intra_rat_global_cell_id_t     *p_cell_id = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_cell_id = mlb_get_cell_id_from_mif_msgs(p_cspl_hdr->from, p_cspl_hdr->api, p_msg);
    event_id = son_mlb_get_cell_event_id(p_cspl_hdr->api);
    if (p_cell_id == SON_PNULL)
    {
        /*Buffer gets released so ret is RELEASE_SON_BUFFER*/
        ret = mlb_global_enable_req_handler(p_cspl_hdr, p_msg, event_id);
    }
    else
    {
        p_cell_ctxt = mlb_get_cell_context_from_global_context(p_cell_id);

        if (!p_cell_ctxt)
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
                    "Cell context not found! for Msg : %s",
                    SON_MLB_CELL_FSM_EVENT_NAMES[event_id] );

            mlb_create_send_generic_nack(
                    p_msg, 
                    p_cspl_hdr,
                    SON_ERR_CELL_UNCONFIGURED);
            ret  = RELEASE_SON_BUFFER;
        }
        else
        {
            ret = mlb_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, p_msg);
        }
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_enable_req_handler */

/*****************************************************************************
 * Function Name  : mlb_global_enable_req_handler
 * Inputs         : p_cspl_hdr -  Pointer to cspl stack header
 *                  p_msg     -   Pointer to the message received     
 *                  event_id  -   event id
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is
 *                  invoked when SONMLB_ENABLE_REQ message with 
 *                  no cell id is received.
 *****************************************************************************/
son_buf_retain_status_et
mlb_global_enable_req_handler
(
    STACKAPIHDR        *p_cspl_hdr,
    son_u8             *p_msg,
    son_mlb_index_et    event_id
 )
{
    son_mlb_enable_res_t enable_res;
    son_size_t enable_res_len = sizeof(son_mlb_enable_res_t);
    son_mlb_enable_req_t        *p_buf    = (son_mlb_enable_req_t  *)p_msg;
    mlb_cell_context_t          *p_cell_ctxt = SON_PNULL;
    mlb_cell_context_node_t *p_cell_ctxt_node = SON_PNULL;
    son_mlb_enable_req_t        enable_req = {0};
    son_cell_status_t           cell_sts;
    son_oam_log_on_off_et       log_mode = mlb_get_log_mode();

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&enable_res, 0, enable_res_len);
    SON_MEMSET(&cell_sts, 0, sizeof(son_cell_status_t));
    mlb_reset_pending_resp_list();
    mlb_set_global_mode(SON_ENABLED);

    /* Find from the configure cell list whether there is at least one
     * cell with cell state = MLB_CELL_STATE_DISABLED or
     * MLB_CELL_STATE_DISABLING. */
    if (SON_TRUE == mlb_is_any_cell_disabled())
    {
        mlb_init_pending_res(p_buf->transaction_id, SONMLB_ENABLE_RES);
        /*Pointer to the first context node*/
        p_cell_ctxt_node = mlb_get_first_cell_ctxt();

        while (SON_PNULL != p_cell_ctxt_node)
        {
            /*Get the data part of the node*/
            p_cell_ctxt = &p_cell_ctxt_node->data;

            if (MLB_CELL_STATE_DISABLED  ==  
                    p_cell_ctxt->current_cell_fsm_state)
            {
                cell_sts.error_code = SON_ERR_TIMER_EXPIRED;
                SON_MEMCPY(&cell_sts.cgi,
                        &p_cell_ctxt->cell_id,
                    sizeof(son_intra_rat_global_cell_id_t));

                if (SON_TRUE == mlb_add_cell_in_pending_res(&cell_sts))
                {
                    enable_req.transaction_id = p_buf->transaction_id; 
                    enable_req.enable_req.bitmask = 
                        SON_OAM_MLB_ENABLE_SRC_CGI_PRESENT;
                    SON_MEMCPY(&enable_req.enable_req.src_cgi,
                            &p_cell_ctxt->cell_id,
                        sizeof(son_intra_rat_global_cell_id_t));

                    mlb_cell_process_msg(event_id,
                            p_cell_ctxt,
                            p_cspl_hdr,
                        (son_u8 *)&enable_req);
                }
                else
                {
                    SON_LOG(log_mode, p_son_mlb_facility_name,
                            SON_ERROR,
                            "Cell 0x%x "
                            "could not be added to "
                            "pending response list",
                            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
                }
            }
            else
            {
                SON_LOG(log_mode, p_son_mlb_facility_name,
                        SON_ERROR,
                        "Cell with cells_0x%x"
                        " is in %s state",
                        son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                        SON_MLB_CELL_FSM_STATES_NAMES[p_cell_ctxt->
                        current_cell_fsm_state]);
            }

            p_cell_ctxt_node = mlb_get_next_node(p_cell_ctxt_node);
        }

        if (0 == mlb_get_pending_res_expected_count())
        {
            /* Construct and send SONMLB_ENABLE_RES with all the cells in
             * Pending Response List as per the error status of the
             * cell. */
            enable_res.transaction_id = p_buf->transaction_id;
            enable_res.enable_res.result = SON_FAILURE;
            enable_res.enable_res.error_code = SON_NO_ERROR;
            mlb_populate_enable_disable_res_failure_data_from_pending_list(
                    &enable_res.enable_res.cell_status_list_size,
                    enable_res.enable_res.cell_status_list);

            son_create_send_buffer((son_u8 *)&enable_res,
                    SON_MLB_MODULE_ID, SON_MIF_MODULE_ID, SONMLB_ENABLE_RES,
                    enable_res_len);

            mlb_reset_pending_resp_list();
        }
        else
        {
            /* Set MLB global FSM state to MLB_STATE_ENABLING
             * as at least 1 registration request has been sent */
            SONMLB_FSM_SET_STATE(g_mlb_context.current_fsm_state,
                    MLB_STATE_ENABLING);
            g_mlb_context.mlb_prending_res_timer_id = son_start_timer(
                    MLB_DEFULT_PENDING_RESPONSE_TIMER, SON_PNULL, SON_NULL,
                    SON_FALSE);
        }
    }
    else
    {
        /* Send SONMLB_ENABLE_RES to SONMgmtIfH with result as
         * SON_SUCCESS */
        enable_res.transaction_id = p_buf->transaction_id;
        enable_res.enable_res.result = SON_SUCCESS;
        enable_res.enable_res.error_code = SON_NO_ERROR;
        son_create_send_buffer((son_u8 *)&enable_res,
                SON_MLB_MODULE_ID, SON_MIF_MODULE_ID, SONMLB_ENABLE_RES,
                enable_res_len);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_global_enable_req_handler */

/*****************************************************************************
 * Function Name  : mlb_disable_req_handler
 * Inputs         : p_buf   -     Pointer to the message received
 *                  p_cspl_hdr -  Pointer to cspl stack header
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is called when
 *                  SONMLB_DISABLE_REQ message is received from OAM. 
 ******************************************************************************/
son_buf_retain_status_et
mlb_disable_req_handler
(
    son_u8         *p_msg,
    STACKAPIHDR    *p_cspl_hdr
 )
{
    son_buf_retain_status_et    ret = RELEASE_SON_BUFFER;
    mlb_cell_context_t          *p_cell_ctxt = SON_PNULL;
    son_mlb_index_et            event_id = CELL_SON_INVALID_EVENT;
    son_intra_rat_global_cell_id_t     *p_cell_id = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_cell_id = mlb_get_cell_id_from_mif_msgs(p_cspl_hdr->from,
        p_cspl_hdr->api, p_msg);
    event_id = son_mlb_get_cell_event_id(p_cspl_hdr->api);
    if (p_cell_id == SON_PNULL)
    {
        /*Buffer gets released so ret is RELEASE_SON_BUFFER*/
        ret = mlb_global_disable_req_handler(p_cspl_hdr, p_msg, event_id);
    }
    else
    {
        p_cell_ctxt = mlb_get_cell_context_from_global_context(p_cell_id);

        if (!p_cell_ctxt)
        {
            SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
                    "Cell context not found! for Msg :%s",
                    SON_MLB_CELL_FSM_EVENT_NAMES[event_id] );

            mlb_create_send_generic_nack(
                    p_msg, 
                    p_cspl_hdr,
                    SON_ERR_CELL_UNCONFIGURED);
            ret  = RELEASE_SON_BUFFER;
        }
        else
        {
            ret = mlb_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, p_msg);
        }
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_disable_req_handler */

/*****************************************************************************
 * Function Name  : mlb_global_disable_req_handler
 * Inputs         : p_cspl_hdr -  Pointer to cspl stack header
 *                  p_msg    -    Pointer to the message received
 *                  event_id  -   event id
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is 
 *                  invoked when SONMLB_DISABLE_REQ message with 
 *                  no cell id is received
 *****************************************************************************/
son_buf_retain_status_et
mlb_global_disable_req_handler
(
    STACKAPIHDR        *p_cspl_hdr,
    son_u8             *p_msg,
    son_mlb_index_et   event_id
 )
{
    son_mlb_disable_res_t disable_res;
    son_size_t disable_res_len = sizeof(son_mlb_disable_res_t);
    son_mlb_disable_req_t   *p_buf = (son_mlb_disable_req_t  *)p_msg;
    mlb_cell_context_t      *p_cell_ctxt = SON_PNULL;
    mlb_cell_context_node_t *p_cell_ctxt_node = SON_PNULL;
    son_mlb_disable_req_t   disable_req = {0};
    son_cell_status_t       cell_sts;
    son_oam_log_on_off_et   log_mode = mlb_get_log_mode();

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&disable_res, 0, disable_res_len);
    SON_MEMSET(&cell_sts, 0, sizeof(son_cell_status_t));
    mlb_set_global_mode(SON_DISABLED);
    mlb_reset_pending_resp_list();

    /* Find from the configure cell list whether there is at least one
     * cell with cell state =  MLB_CELL_STATE_ENABLED or
     * MLB_CELL_STATE_ENABLING state */
    if (SON_TRUE == mlb_is_any_cell_enabled())
    {
        /* If such cells exists */
        mlb_init_pending_res(p_buf->transaction_id,
                SONMLB_DISABLE_RES);
        /*Pointer to the first context node*/
        p_cell_ctxt_node = mlb_get_first_cell_ctxt();

        while (SON_PNULL != p_cell_ctxt_node)
        {
            /*Get the data part of the node*/
            p_cell_ctxt = &p_cell_ctxt_node->data;
            if ((MLB_CELL_STATE_ENABLED ==
                        p_cell_ctxt->current_cell_fsm_state))
            {
                cell_sts.error_code = SON_ERR_TIMER_EXPIRED;
                SON_MEMCPY(&cell_sts.cgi,
                        &p_cell_ctxt->cell_id,
                    sizeof(son_intra_rat_global_cell_id_t));

                if (SON_TRUE == mlb_add_cell_in_pending_res(&cell_sts))
                {
                    disable_req.transaction_id = p_buf->transaction_id;
                    disable_req.disable_req.bitmask = 
                        SON_OAM_MLB_ENABLE_SRC_CGI_PRESENT;

                    SON_MEMCPY(&disable_req.disable_req.src_cgi,
                            &p_cell_ctxt->cell_id,
                        sizeof(son_intra_rat_global_cell_id_t));
                    mlb_cell_process_msg(event_id,
                            p_cell_ctxt,
                            p_cspl_hdr,
                        (son_u8 *)&disable_req);
                }
                else
                {
                    SON_LOG(log_mode, p_son_mlb_facility_name,
                            SON_ERROR,
                            "Cell 0x%x "
                            "could not be added to "
                            "pending response list",
                            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
                }
            }
            else
            {
                SON_LOG(log_mode, p_son_mlb_facility_name,
                        SON_ERROR,
                        "Cell with cells_id 0x%x"
                        " is in %s state",
                        son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                        SON_MLB_CELL_FSM_STATES_NAMES[p_cell_ctxt->
                        current_cell_fsm_state]);
            }

            p_cell_ctxt_node = mlb_get_next_node(p_cell_ctxt_node);
        }

        if (0 == mlb_get_pending_res_expected_count())
        {
            /* Construct and send SONMLB_DISABLE_RES with all the cells in
             * Pending Response List as per the error status of the
             * cell. */
            disable_res.transaction_id = p_buf->transaction_id;
            disable_res.disable_res.result = SON_FAILURE;
            disable_res.disable_res.error_code = SON_NO_ERROR;
            mlb_populate_enable_disable_res_failure_data_from_pending_list(
                    &disable_res.disable_res.cell_status_list_size,
                    disable_res.disable_res.cell_status_list);

            son_create_send_buffer((son_u8 *)&disable_res,
                    SON_MLB_MODULE_ID, SON_MIF_MODULE_ID,
                    SONMLB_DISABLE_RES, disable_res_len);

            mlb_reset_pending_resp_list();
        }
        else
        {
            /* Set MLB global FSM state to MLB_STATE_DISABLING */
            SONMLB_FSM_SET_STATE(g_mlb_context.current_fsm_state,
                    MLB_STATE_DISABLING);
            g_mlb_context.mlb_prending_res_timer_id = son_start_timer(
                    MLB_DEFULT_PENDING_RESPONSE_TIMER, SON_PNULL, SON_NULL,
                    SON_FALSE);
        }
    }
    else
    {
        /* Send SONMLB_DISABLE_RES to SONMgmtIfH with result as
         * SON_SUCCESS */
        disable_res.transaction_id = p_buf->transaction_id;
        disable_res.disable_res.result = SON_SUCCESS;
        disable_res.disable_res.error_code = SON_NO_ERROR;
        son_create_send_buffer((son_u8 *)&disable_res,
                SON_MLB_MODULE_ID, SON_MIF_MODULE_ID, SONMLB_DISABLE_RES,
                disable_res_len);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mlb_global_disable_req_handler */

/*SONMLB-- Centralized support start*/

/*****************************************************************************
 * Function Name  : mlb_offset_change_req_handler
 * Inputs         : p_msg        Pointer to the message received
 *                : p_cspl_hdr
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is called when
 * SONMLB_OFFSET_CHANGE_REQ message is received from SON-MIF.
 ******************************************************************************/
son_buf_retain_status_et
mlb_offset_change_req_handler
(
    son_u8         *p_msg,
    STACKAPIHDR    *p_cspl_hdr
)
{
    son_buf_retain_status_et ret = RELEASE_SON_BUFFER;
    mlb_cell_context_t              *p_cell_ctxt = SON_PNULL;

    son_mlb_index_et event_id = CELL_SON_INVALID_EVENT;
    son_intra_rat_global_cell_id_t p_cell_id;

    SON_UT_TRACE_ENTER();

    son_mlb_offset_change_req_t  *req = (son_mlb_offset_change_req_t *)p_msg;

    SON_MEMCPY(&p_cell_id, &req->offset_change_req.srv_cgi, sizeof(son_intra_rat_global_cell_id_t));
    event_id = son_mlb_get_cell_event_id(p_cspl_hdr->api);
    p_cell_ctxt = mlb_get_cell_context_from_global_context(&p_cell_id);

    if (!p_cell_ctxt)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Cell context not found! for Msg : %s", SON_MLB_CELL_FSM_EVENT_NAMES[event_id] );

        son_mlb_offset_change_resp_t offset_change_resp;
        son_size_t offset_change_resp_len =
            sizeof(son_mlb_offset_change_resp_t);
        SON_MEMSET(&offset_change_resp, 0, offset_change_resp_len);
        offset_change_resp.transaction_id = req->transaction_id;
        SON_MEMCPY(&offset_change_resp.offset_change_resp.srv_cgi,
            &req->offset_change_req.srv_cgi,
            sizeof(son_intra_rat_global_cell_id_t));
        offset_change_resp.offset_change_resp.mlb_status = SON_FAILURE;
        offset_change_resp.offset_change_resp.error_code =
            SON_ERR_CELL_UNCONFIGURED;
        son_create_send_buffer((son_u8 *)&offset_change_resp,
            SON_MLB_MODULE_ID, SON_MIF_MODULE_ID,
            SONMLB_OFFSET_CHANGE_RESP, offset_change_resp_len);
    }
    else if (SON_MODE_CENTRALIZED != g_mlb_context.mlb_config_param.mlb_mode)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Unexpected msg in MLB mode [%d]"
            , g_mlb_context.mlb_config_param.mlb_mode);
    }
    else
    {
        ret = mlb_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, p_msg);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_offset_change_req_handler */

/*****************************************************************************
 * Function Name  : mlb_peer_offset_change_resp_handler
 * Inputs         : p_msg        Pointer to the message received
 *                : p_cspl_hdr
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for MLB module and is called when
 * SONMLB_PEER_OFFSET_CHANGE_RESP message is received from SON-MIF.
 ******************************************************************************/
son_buf_retain_status_et
mlb_peer_offset_change_resp_handler
(
    son_u8         *p_msg,
    STACKAPIHDR    *p_cspl_hdr
)
{
    son_buf_retain_status_et ret = RELEASE_SON_BUFFER;
    mlb_cell_context_t              *p_cell_ctxt = SON_PNULL;

    son_mlb_index_et event_id = CELL_SON_INVALID_EVENT;
    son_intra_rat_global_cell_id_t p_cell_id;

    SON_UT_TRACE_ENTER();

    son_mlb_peer_offset_change_resp_t  *req = (son_mlb_peer_offset_change_resp_t *)p_msg;

    SON_MEMCPY(&p_cell_id, &req->peer_offset_change_resp.trgt_cgi, sizeof(son_intra_rat_global_cell_id_t));
    event_id = son_mlb_get_cell_event_id(p_cspl_hdr->api);
    p_cell_ctxt = mlb_get_cell_context_from_global_context(&p_cell_id);

    if (!p_cell_ctxt)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Cell context not found! for Msg : %s", SON_MLB_CELL_FSM_EVENT_NAMES[event_id] );
    }
    else if (SON_MODE_CENTRALIZED != g_mlb_context.mlb_config_param.mlb_mode)
    {
        SON_LOG(mlb_get_log_mode(), p_son_mlb_facility_name, SON_ERROR,
            "Unexpected msg in MLB mode [%d]"
            , g_mlb_context.mlb_config_param.mlb_mode);
    }
    else
    {
        ret = mlb_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, p_msg);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mlb_peer_offset_change_resp_handler */

/*SONMLB-- Centralized support end*/

/* Rel 3.0 Cell delete support changes Start */
/*****************************************************************************
 * Function Name  : mlb_fsm_cell_delete_req_handler
 * Inputs         : p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  event_id - Value of the event Id internal to MLB module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SMIF_CELL_DELETE_REQ message received from MIF
 ****************************************************************************/
static son_buf_retain_status_et
mlb_fsm_cell_delete_req_handler
(
    son_u8          *p_buf,
    STACKAPIHDR     *p_cspl_hdr
)
{
    smif_cell_delete_req_t   *p_msg          = (smif_cell_delete_req_t *)p_buf;
    mlb_cell_context_t      *p_cell_ctxt    = SON_PNULL;
    son_u8                  idx             = 0;
    smif_cell_delete_resp_t  cell_del_res;
    son_oam_log_on_off_et   log_mode = mlb_get_log_mode();
    cell_status_list_data_t *p_list_node = SON_PNULL;
    son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&cell_del_res, 0, sizeof(cell_del_res));

    cell_del_res.cell_delete_status_list_size =
        p_msg->cell_delete_list_size;

    /* Traverse the cell delete list */
    for (idx = 0; idx < p_msg->cell_delete_list_size; idx ++)
    {
        /* Populate the response structure */
        SON_MEMCPY(&cell_del_res.cell_delete_status_list[idx].cgi,
                &p_msg->cell_delete_list[idx],
                sizeof(son_intra_rat_global_cell_id_t));

        cell_del_res.cell_delete_status_list[idx].result = SON_FAILURE;

        /* Find the cell in global context */
        p_cell_ctxt = mlb_get_cell_context_from_global_context(
                &p_msg->cell_delete_list[idx]);

        if (SON_PNULL != p_cell_ctxt)
        {
            /* Send deregister request to RRM if cell is ENABLING/ENABLED */
            if ((MLB_CELL_STATE_ENABLING == 
                        p_cell_ctxt->current_cell_fsm_state) ||
                    (MLB_CELL_STATE_ENABLED == 
                     p_cell_ctxt->current_cell_fsm_state))
            {
                son_u16 txn_id =  son_generate_txn_id();

                /* Start Coverity Fix: 60110 */
                if (SON_FAILURE == mlb_construct_send_rrm_deregister_req(
                            txn_id,
                            &p_cell_ctxt->cell_id ))
                {
                    cell_del_res.cell_delete_status_list[idx].result = 
                        SON_PARTIAL_SUCCESS;
                    SON_LOG(mlb_get_log_mode(),
                            p_son_mlb_facility_name,
                            SON_ERROR,
                            "mlb_construct_send_rrm_deregister_req failed "
                            "for Cell 0x%x]",
                            son_convert_char_cell_id_to_int_cell_id(
                                p_cell_ctxt->cell_id.cell_identity));
                }
                /* End Coverity Fix: 60110 */
                else
                {
                    /* Store the transaction Id with which De-register
                       request is sent to RRM. This shall be used later when
                       De-register response is received for this request */
                    son_insert_txn_id_in_temp_txn_id_list(txn_id);
                }
            }

            /* Check the pending resp list for this cell */
            if (mlb_get_pending_res_expected_count())
            {
                /* Search for the cell Id in pending response structure. */
                p_list_node = mlb_find_cell_in_pending_res(&p_cell_ctxt->cell_id);

                if (SON_PNULL != p_list_node)
                {
                    /* Error code SON_ERR_TIMER_EXPIRED indicates that
                       the pending response is not received for this cell 
                       so decrement expected response count since the 
                       cell is being deleted and no response should be
                       expected for this cell. */
                    if (SON_ERR_TIMER_EXPIRED == p_list_node->cell_status.error_code)
                    {
                        mlb_decrement_pending_res_expected_count();
                    }
                    api = mlb_get_pending_res_api_id();
                    if (SMIF_CELL_INFO_IND == api)
                    {
                        /* Delete the cell as in case of cell deletion
                           no feature state change indication shall be send
                           and only cell delete response shall be send to 
                           SMIF */
                        list_delete_node(&g_mlb_pending_response.cell_sts_list,
                                &p_list_node->cell_status_node);
                        son_mem_free(p_list_node);
                        SON_LOG(g_mlb_context.log_enable, p_son_mlb_facility_name,
                                SON_BRIEF,
                                "Deleted Cell 0x%x from Pending Response List",
                                son_convert_char_cell_id_to_int_cell_id(
                                p_cell_ctxt->cell_id.cell_identity));
                    }
                    else
                    {
                        /* Update Cell in MLB Pending Response List with error code
                           SON_ERR_CELL_UNCONFIGURED */
                        p_list_node->cell_status.error_code = SON_ERR_CELL_UNCONFIGURED;
                    }
                }

                /* Start Coverity Fix: 60121 */
                if (0 == mlb_get_pending_res_expected_count())
                {
                    /* End Coverity Fix: 60121 */
                    mlb_pending_res_timer_handler();
                    /* Set MLB global FSM state to ACTIVE */
                    SONMLB_FSM_SET_STATE(g_mlb_context.current_fsm_state,
                            MLB_STATE_ACTIVE);
                }
            }

            /* Delete the cell from the context */
            if (SON_TRUE == mlb_delete_cell_from_context
                    (&p_msg->cell_delete_list[idx]))
            {
                if (SON_PARTIAL_SUCCESS != 
                        cell_del_res.cell_delete_status_list[idx].result)
                {
                    cell_del_res.cell_delete_status_list[idx].result = 
                        SON_SUCCESS;
                }

                SON_LOG(log_mode, p_son_mlb_facility_name,
                        SON_INFO,
                        "Cell 0x%x deleted successfully",
                        son_convert_char_cell_id_to_int_cell_id
                        (p_msg->cell_delete_list[idx].cell_identity));
            }
        }
        else
        {
                    /*SPR 17777 +-*/
            SON_LOG(log_mode, p_son_mlb_facility_name,
                    SON_WARNING,
                    "Cell 0x%x does not exists",
                    son_convert_char_cell_id_to_int_cell_id
                    (p_msg->cell_delete_list[idx].cell_identity));
            LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
        }
    }

    /* Send cell delete resp to mif */
    son_create_send_buffer((son_u8 *)&cell_del_res,
            SON_MLB_MODULE_ID,
            SON_MIF_MODULE_ID,
            SMIF_CELL_DELETE_RESP,
            sizeof(cell_del_res));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}
/* Rel 3.0 Cell delete support changes Stop */

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : mlb_fsm_get_log_level_req_handler
 * Inputs         : p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  event_id - Value of the event Id internal to MLB module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SMIF_GET_LOG_LEVEL_REQ message received from MIF
 ****************************************************************************/
static son_buf_retain_status_et
mlb_fsm_get_log_level_req_handler
(
    son_u8          *p_buf,
    STACKAPIHDR     *p_cspl_hdr
)
{
    smif_get_log_level_req_t *p_msg = (smif_get_log_level_req_t*)p_buf;
    smif_get_log_level_res_t res;
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&res, 0, sizeof(res));
    res.transaction_id = p_msg->transaction_id;
    res.log_level = mlb_get_log_level();
    
    /* Send cell delete resp to mif */
    son_create_send_buffer((son_u8 *)&res,
            SON_MLB_MODULE_ID,
            SON_MIF_MODULE_ID,
            SMIF_GET_LOG_LEVEL_RESP,
            sizeof(res));
                    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}
/* SPR-13251 Fix Ends */

/*******************************************************************************
 * Array : mlb_fsm_table
 *
 * DESCRIPTION: Function pointer table for UE FSM. It contains the list of event
 * handlers which will perform the corresponding actions.
 ******************************************************************************/
mlb_fsm_fn_ptr_tbl
mlb_fsm_table[MLB_MAX_FSM_STATE][MLB_MAX_FSM_EVENT] = 
{
    /*
     * 0: MLB_STATE_INIT 
     */
    {
        mlb_init_state_handler,
        /*SONMLB_INIT_CONFIG_REQ*/ 
        mlb_fsm_nack_enable_req_msg,
        /*SONMLB_ENABLE_REQ*/
        mlb_fsm_nack_disable_req_msg,
        /*SONMLB_DISABLE_REQ*/
        mlb_fsm_ignore_msg, 
        /*SMIF_CELL_INFO_IND*/
        mlb_fsm_ignore_msg, 
        /* SMIF_LOG_ENABLE_DISABLE_CMD*/
        mlb_fsm_ignore_msg,
        /*SMIF_SET_LOG_LEVEL_CMD*/
        mlb_fsm_ignore_msg, 
        /*RRM_SON_REGISTER_RES*/
        mlb_fsm_ignore_msg, 
        /*RRM_SON_DEREGISTER_RES*/
        mlb_fsm_ignore_msg,
        /*RRM_SON_HO_REPORT*/
        mlb_fsm_ignore_msg,
        /*RRM_SON_LOAD_REPORT*/
        mlb_fsm_ignore_msg,
        /*RRM_SON_CELL_STATE_CHANGE*/
        mlb_fsm_ignore_msg, 
        /*SONANR_DISABLE_IND*/
        mlb_fsm_nack_modify_req_msg,
        /*SONMLB_MODIFY_REQ*/
        mlb_shutdown_handler, 
        /*SMIF_SHUTDOWN_REQ*/
        mlb_fsm_ignore_msg, 
        /*SON_MOBILITY_CHANGE_REQ*/
        mlb_fsm_ignore_msg,   
        /*SON_MOBILITY_CHANGE_FAILURE*/
        mlb_fsm_ignore_msg,
        /*SON_MOBILITY_CHANGE_ACK */
/*Rel 3.0 Multi Sector code changes Start*/
        mlb_fsm_ignore_msg,
        /*SON_INTERNAL_MOBILITY_CHANGE_REQ*/
        mlb_fsm_ignore_msg,
        /*SON_INTERNAL_MOBILITY_CHANGE_FAILURE*/
        mlb_fsm_ignore_msg,
        /*SON_INTERNAL_MOBILITY_CHANGE_ACK */ 
/*Rel 3.0 Multi Sector code changes Stop*/
        mlb_fsm_ignore_msg,
        /*SONANR_NBR_DELETE_IND */
        mlb_fsm_ignore_msg,
        /*SONMLB_OFFSET_CHANGE_REQ */
        mlb_fsm_ignore_msg,
        /*SONMLB_PEER_OFFSET_CHANGE_RESP */
/*MULTISECTOR CHANGE*/
        /* Rel 3.0 Cell delete support changes Start */
        mlb_fsm_ignore_msg,
        /*SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        mlb_fsm_ignore_msg,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
/* eICIC feature code Starts */
       mlb_fsm_ignore_msg,
      /*RRM_SON_EICIC_ENABLE_DISABLE_IND*/
/* eICIC feature code Ends */
    },

    /*
     * 1: MLB_STATE_ACTIVE 
     */
    {
        mlb_fsm_nack_init_config_req_msg, 
        /*SONMLB_INIT_CONFIG_REQ*/ 
        mlb_enable_req_handler,
        /*SONMLB_ENABLE_REQ*/
        mlb_disable_req_handler,
        /*SONMLB_DISABLE_REQ*/
        mlb_cell_info_ind_handler,
        /*SMIF_CELL_INFO_IND*/
        mlb_log_handler,
        /* SMIF_LOG_ENABLE_DISABLE_CMD*/
        mlb_log_handler,
        /*SMIF_SET_LOG_LEVEL_CMD*/
        mlb_handle_rrm_messages,
        /*RRM_SON_REGISTER_RES*/
        mlb_handle_rrm_messages,
        /*RRM_SON_DEREGISTER_RES*/
        mlb_handle_rrm_messages,
        /*RRM_SON_HO_REPORT*/
        mlb_handle_rrm_messages,
        /*RRM_SON_LOAD_REPORT*/
        mlb_handle_rrm_messages, 
        /*RRM_SON_CELL_STATE_CHANGE*/
        mlb_global_sonanr_disable_ind_handler,
        /*SONANR_DISABLE_IND**/
        mlb_sonmlb_modify_req,
        /*SONMLB_MODIFY_REQ*/
        mlb_shutdown_handler,
        /*SMIF_SHUTDOWN_REQ*/
        mlb_mobility_change_req_handler, 
        /*SON_MOBILITY_CHANGE_REQ*/
        mlb_mobility_change_failure_handler,   
        /*SON_MOBILITY_CHANGE_FAILURE*/
        mlb_mobility_change_ack_handler,    
        /*SON_MOBILITY_CHANGE_ACK */
/*Rel 3.0 Multi Sector code changes Start*/
        mlb_mobility_change_req_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_REQ*/
        mlb_mobility_change_failure_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_FAILURE*/
        mlb_mobility_change_ack_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_ACK */
/*Rel 3.0 Multi Sector code changes Stop*/ 
        mlb_anr_nbr_update_notification,
        /* SONANR_NBR_DELETE_IND*/
        mlb_offset_change_req_handler,
        /*SONMLB_OFFSET_CHANGE_REQ */
        mlb_peer_offset_change_resp_handler,
        /*SONMLB_PEER_OFFSET_CHANGE_RESP */
        /* Rel 3.0 Cell delete support changes Start */
        mlb_fsm_cell_delete_req_handler,
        /*SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        mlb_fsm_get_log_level_req_handler,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
/* eICIC feature code Starts */
        mlb_handle_rrm_messages,
       /*RRM_SON_EICIC_ENABLE_DISABLE_IND*/
/* eICIC feature code Ends */
    },

    /*
     * 2: MLB_STATE_ENABLING 
     */
    {
        mlb_fsm_nack_init_config_req_msg, 
        /*SONMLB_INIT_CONFIG_REQ*/
        mlb_fsm_nack_enable_req_msg, 
        /*SONMLB_ENABLE_REQ*/
        mlb_fsm_nack_disable_req_msg, 
        /*SONMLB_DISABLE_REQ*/
        mlb_fsm_ignore_msg,  
        /*SMIF_CELL_INFO_IND*/
        mlb_log_handler,
        /* SMIF_LOG_ENABLE_DISABLE_CMD*/
        mlb_log_handler,
        /*SMIF_SET_LOG_LEVEL_CMD*/
        mlb_handle_rrm_messages, 
        /*RRM_SON_REGISTER_RES*/
        mlb_fsm_ignore_msg, 
        /*RRM_SON_DEREGISTER_RES*/
        mlb_handle_rrm_messages, 
        /*RRM_SON_HO_REPORT*/
        mlb_handle_rrm_messages, 
        /*RRM_SON_LOAD_REPORT*/
        mlb_handle_rrm_messages,
        /*RRM_SON_CELL_STATE_CHANGE*/
        mlb_global_sonanr_disable_ind_handler, 
        /*SONANR_DISABLE_IND*/
        mlb_sonmlb_modify_req, 
        /*SONMLB_MODIFY_REQ*/
        mlb_shutdown_handler, 
        /*SMIF_SHUTDOWN_REQ*/
        mlb_mobility_change_req_handler, 
        /*SON_MOBILITY_CHANGE_REQ*/
        mlb_mobility_change_failure_handler,
        /*SON_MOBILITY_CHANGE_FAILURE*/
        mlb_mobility_change_ack_handler,
        /*SON_MOBILITY_CHANGE_ACK */
/*Rel 3.0 Multi Sector code changes Start*/
        mlb_mobility_change_req_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_REQ*/
        mlb_mobility_change_failure_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_FAILURE*/
        mlb_mobility_change_ack_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_ACK */
/*Rel 3.0 Multi Sector code changes Stop*/
        mlb_fsm_ignore_msg,
        /*SONANR_NBR_DELETE_IND*/
        mlb_fsm_ignore_msg,
        /*SONMLB_OFFSET_CHANGE_REQ */
        mlb_fsm_ignore_msg,
        /*SONMLB_PEER_OFFSET_CHANGE_RESP */
        /* Rel 3.0 Cell delete support changes Start */
        mlb_fsm_cell_delete_req_handler,
        /*SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        mlb_fsm_get_log_level_req_handler,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
/* eICIC feature code Starts */
       mlb_fsm_ignore_msg,
      /*RRM_SON_EICIC_ENABLE_DISABLE_IND*/
/* eICIC feature code Ends */
    },

    /*
     * 3: MLB_STATE_DISABLING 
     */
    {
        mlb_fsm_nack_init_config_req_msg, 
        /*SONMLB_INIT_CONFIG_REQ*/
        mlb_fsm_nack_enable_req_msg,
        /*SONMLB_ENABLE_REQ*/
        mlb_fsm_nack_disable_req_msg,
        /*SONMLB_DISABLE_REQ*/
        mlb_fsm_ignore_msg,  
        /*SMIF_CELL_INFO_IND*/
        mlb_log_handler,
        /* SMIF_LOG_ENABLE_DISABLE_CMD*/
        mlb_log_handler,
        /*SMIF_SET_LOG_LEVEL_CMD*/
        mlb_fsm_ignore_msg,
        /*RRM_SON_REGISTER_RES*/
        mlb_handle_rrm_messages,
        /*RRM_SON_DEREGISTER_RES*/
        mlb_handle_rrm_messages, 
        /*RRM_SON_HO_REPORT*/
        mlb_handle_rrm_messages,
        /*RRM_SON_LOAD_REPORT*/
        mlb_handle_rrm_messages,
        /*RRM_SON_CELL_STATE_CHANGE*/
        mlb_global_sonanr_disable_ind_handler, 
        /*SONANR_DISABLE_IND*/
        mlb_sonmlb_modify_req, 
        /*SONMLB_MODIFY_REQ*/
        mlb_shutdown_handler, 
        /*SMIF_SHUTDOWN_REQ*/
        mlb_mobility_change_req_handler,
        /*SON_MOBILITY_CHANGE_REQ*/
        mlb_mobility_change_failure_handler,
        /*SON_MOBILITY_CHANGE_FAILURE*/
        mlb_mobility_change_ack_handler,
        /*SON_MOBILITY_CHANGE_ACK */
/*Rel 3.0 Multi Sector code changes Start*/
        mlb_mobility_change_req_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_REQ*/
        mlb_mobility_change_failure_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_FAILURE*/
        mlb_mobility_change_ack_handler,
        /*SON_INTERNAL_MOBILITY_CHANGE_ACK */
/*Rel 3.0 Multi Sector code changes Stop*/
        mlb_fsm_ignore_msg,
        /*SONANR_NBR_DELETE_IND */
        mlb_fsm_ignore_msg,
        /*SONMLB_OFFSET_CHANGE_REQ */
        mlb_fsm_ignore_msg,
        /*SONMLB_PEER_OFFSET_CHANGE_RESP */
        /* Rel 3.0 Cell delete support changes Start */
        mlb_fsm_cell_delete_req_handler,
        /*SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        mlb_fsm_get_log_level_req_handler,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
/* eICIC feature code Starts */
       mlb_fsm_ignore_msg,
      /*RRM_SON_EICIC_ENABLE_DISABLE_IND*/
/* eICIC feature code Ends */
    },

    /*
     * 4: MLB_STATE_SHUTDOWN 
     */
    {
        mlb_fsm_ignore_msg,
        /*SONMLB_INIT_CONFIG_REQ*/
        mlb_fsm_ignore_msg,  
        /*SONMLB_ENABLE_REQ*/
        mlb_fsm_ignore_msg, 
        /*SONMLB_DISABLE_REQ*/
        mlb_fsm_ignore_msg, 
        /*SMIF_CELL_INFO_IND*/
        mlb_fsm_ignore_msg, 
        /*SMIF_LOG_ENABLE_DISABLE_CMD*/
        mlb_fsm_ignore_msg, 
        /*SMIF_SET_LOG_LEVEL_CMD*/
        mlb_fsm_ignore_msg, 
        /*RRM_SON_REGISTER_RES*/
        mlb_handle_rrm_messages,
        /*RRM_SON_DEREGISTER_RES*/
        mlb_fsm_ignore_msg, 
        /*RRM_SON_HO_REPORT*/
        mlb_fsm_ignore_msg, 
        /*RRM_SON_LOAD_REPORT*/
        mlb_fsm_ignore_msg,
        /*RRM_SON_CELL_STATE_CHANGE*/
        mlb_fsm_ignore_msg, 
        /*SONANR_DISABLE_IND*/
        mlb_fsm_ignore_msg, 
        /*SONMLB_MODIFY_REQ*/
        mlb_fsm_ignore_msg,
        /*SMIF_SHUTDOWN_REQ*/
        mlb_fsm_ignore_msg,
        /*SON_MOBILITY_CHANGE_REQ*/
        mlb_fsm_ignore_msg, 
        /*SON_MOBILITY_CHANGE_FAILURE*/
        mlb_fsm_ignore_msg,
        /*SON_MOBILITY_CHANGE_ACK */
/*Rel 3.0 Multi Sector code changes Start*/
        mlb_fsm_ignore_msg,
        /*SON_INTERNAL_MOBILITY_CHANGE_REQ*/
        mlb_fsm_ignore_msg,
        /*SON_INTERNAL_MOBILITY_CHANGE_FAILURE*/
        mlb_fsm_ignore_msg,
        /*SON_INTERNAL_MOBILITY_CHANGE_ACK */
/*Rel 3.0 Multi Sector code changes Stop*/
        mlb_fsm_ignore_msg,
        /*SONANR_NBR_DELETE_IND */
        mlb_fsm_ignore_msg,
        /*SONMLB_OFFSET_CHANGE_REQ */
        mlb_fsm_ignore_msg,
        /*SONMLB_PEER_OFFSET_CHANGE_RESP */
        /* Rel 3.0 Cell delete support changes Start */
        mlb_fsm_ignore_msg,
        /*SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        mlb_fsm_ignore_msg,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
/* eICIC feature code Starts */
       mlb_fsm_ignore_msg,
      /*RRM_SON_EICIC_ENABLE_DISABLE_IND*/
/* eICIC feature code Ends */
    }
};
