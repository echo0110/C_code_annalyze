/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_anr_fsm.c $
 *
 *******************************************************************************
 *
 * File Description:This file contains the functions for handling ANR level 
 *        messages and functions for manifest structure of this module.
 *******************************************************************************
* Revision Details
 *----------------------
 *   DATE            AUTHOR       REFERENCE       REASON
 *   Feb,2012       Komal/Shekhar                 Initial
 *   May,2012       Komal/Shekhar                 Release 1.1 Changes
 *   Aug,2012       Komal/Shekhar                 GERAN support added
 *   May, 2014      Shilpi                        Coverity Fixes  
 *   July, 2014     Shilpi                        SPR 10331 Fix
 *   Aug, 2014      Shilpi                        SPR 13251 Fix
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_anr_cell_fsm.h>
#include <son_anr_utils.h>
#include <son_utils.h>
#include <son_anr_ctxt_mgr.h>
#include <son_rrm_intf.h>
#include <rrm_son_parser.h>
#include <son_rrm_composer.h>
#include <son_rrm_parser.h>
#include <son_anr_enb_ctxt_mgr.h>
#include <son_anr_table.h>
#include <son_x2_intf.h>
#include <son_es_intf.h>
#include <son_anr_enb_ctxt_mgr.h>
#include <son_mlb_ctxt_mgr.h>

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern const son_8 *p_son_anr_facility_name;
extern const son_s8 *p_son_log_level_names[];
/* Rel 3.0 Cell delete support changes Start */
extern pending_res_t g_anr_pending_response;
/* Rel 3.0 Cell delete support changes Stop */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_START */

/*****************************************************************************
 * Function Name  : anr_get_cell_id_from_rrm_msgs
 * Inputs         : p_msg    - pointer to the message
 *                : api      - procedure code of the message
 *                : from     - Source Module id
 * Outputs        : None
 * Returns        : son_intra_rat_global_cell_id_t* - pointer to the cell id
 * Description    : This function determines the cell_id in the message received
 *                : from RRM
 ****************************************************************************/
static son_intra_rat_global_cell_id_t *
anr_get_cell_id_from_rrm_msgs
(
 son_u8 *p_msg,
 son_u16 api,
 son_u16 from 
)
{
    son_intra_rat_global_cell_id_t *p_cell_id = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_msg)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR, 
                "NULL message pointer passed");

        SON_UT_TRACE_EXIT();
        return SON_PNULL; 
    }

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

        case RRM_SON_MEAS_CONFIG_RESP:
        {
            p_cell_id = (son_intra_rat_global_cell_id_t *)
                &(((son_rrm_meas_config_res_t *)p_msg)->config_res.meas_config_res.
                        cell_id);
            break;
        }

        case RRM_SON_SET_ATTRIBUTE_RESP:
        {
            p_cell_id =
                (son_intra_rat_global_cell_id_t *)
                &((son_rrm_set_attribute_res_t *)p_msg)->
                res.set_attr_resp.id.object_id.cell_id;
            break;
        }

        case RRM_SON_MEAS_RESULTS_IND:
        {
            p_cell_id =
                (son_intra_rat_global_cell_id_t *)
                &((son_rrm_meas_results_ind_t *)p_msg)->cell_id;
            break;
        }

        case RRM_SON_HO_REPORT:
        {
            p_cell_id =
                (son_intra_rat_global_cell_id_t *)
                &((son_rrm_ho_report_t *)p_msg)->src_cgi;
            break;
        }

        case RRM_SON_HO_ATTEMPT_IND:
        {
            p_cell_id = (son_intra_rat_global_cell_id_t *)
                &((son_rrm_ho_attempt_ind_t *)p_msg)->src_cgi;
            break;
        }

        case RRM_SON_HO_FAILURE_REPORT:
        {
            p_cell_id = (son_intra_rat_global_cell_id_t *)
                        &((son_ho_failure_report_t *)p_msg)->src_cgi;
            break;
        }

        case RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP:
        {
            p_cell_id = (son_intra_rat_global_cell_id_t *)
                &(((son_rrm_meas_config_obj_remove_res_t *)p_msg)->remove_res.meas_config_obj_remove_res.
                        cell_id);
            break;
        }

        default:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR, 
                    "Invalid api[%d] received"
                " from[%d]\n", api, from);
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
    return p_cell_id;
} /* anr_get_cell_id_from_rrm_msgs */
 
/*****************************************************************************
 * Function Name  : anr_get_cell_id_from_mif_msgs
 * Inputs         : p_msg    - pointer to the message
 *                : api      - procedure code of the message
 *                : from     - source module id
 * Outputs        : None
 * Returns        : son_intra_rat_global_cell_id_t* - Pointer to the cell id
 * Description    : This function determines the cell_id in the message received
 *                  recieved from MIF
 ****************************************************************************/
static son_intra_rat_global_cell_id_t *
anr_get_cell_id_from_mif_msgs
(
son_u8 *p_msg,
son_u16 api,
son_u16 from
)
{
    son_intra_rat_global_cell_id_t *p_cell_id = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_msg)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
              SON_ERROR,
              "anr_get_cell_id_from_mif_msgs:NULL message pointer passed\n");

      SON_UT_TRACE_EXIT();
      return SON_PNULL;
  }

    switch (api)
    {
        case SONANR_ENABLE_REQ:
        {
            if (SON_SRC_CGI_PRESENT &
                ((son_anr_enable_req_t *)p_msg)->enable_req.bitmask)
            {
                p_cell_id =
                    &((son_anr_enable_req_t *)p_msg)->enable_req.src_cgi[0];
            }

            break;
        }

        case SONANR_DISABLE_REQ:
        {
            if (SON_ANR_DISABLE_SRC_CGI_PRESENT &
                ((son_anr_disable_req_t *)p_msg)->disable_req.bitmask)
            {
                p_cell_id =
                    &((son_anr_disable_req_t *)p_msg)->disable_req.src_cgi[0];
            }

            break;
        }

        case SONANR_NR_ADD_REQ:
        {
            p_cell_id =
                &((son_anr_nr_add_req_t *)p_msg)->add_req.src_cgi;
            break;
        }

        case SONANR_NR_DELETE_REQ:
        {
            p_cell_id =
                &((son_anr_nr_delete_req_t *)p_msg)->del_req.src_cgi;
            break;
        }

        case SONANR_NR_UPDATE_REQ:
        {
            p_cell_id =
                &((son_anr_nr_update_req_t *)p_msg)->upd_req.src_cgi;
            break;
        }

        case SONANR_NR_RETRIEVE_REQ:
        {
            p_cell_id =
                &((son_anr_nr_retrieve_req_t *)p_msg)->rtrv_req.src_cgi;
            break;
        }

        case SONANR_MEAS_CONFIG_REQ:
        {
            p_cell_id =
                &((son_anr_meas_config_req_t *)p_msg)->config_req.src_cgi;
            break;
        }

        case SONANR_MODIFY_UE_COUNT_REQ:
        {
           p_cell_id =
               &((son_anr_modify_ue_count_req_t *)p_msg)->
               modify_ue_cnt_req.src_cgi;
           break;
        }

        case SONANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ:
        {
           p_cell_id =
               &((son_anr_modify_meas_reporting_interval_req_t *)p_msg)
               ->modify_interval_req.src_cgi;
           break;
        }

        case SONANR_NR_DELETE_FROM_REMOVE_LIST_REQ:
        {
            p_cell_id =
                &((son_anr_nr_delete_from_remove_list_req_t *)p_msg)->
               delete_from_remove_list_req.src_cgi;
           break;
        }

        case SONANR_MODIFY_REMOVAL_ATTRIBUTES_REQ:
        {
            p_cell_id =
                &((son_anr_modify_removal_attributes_req_t *)p_msg)->
               modify_removal_attributes_req.src_cgi;
           break;
        }

        case SONANR_MODIFY_PCI_CONFUSION_CFG_REQ:
        {
            p_cell_id =
                &((son_anr_modify_pci_confusion_cfg_req_t *)p_msg)->
               modify_pci_confusion_cfg_req.src_cgi;
           break;
        }

        case SONANR_ENABLE_DISABLE_LIMITED_MODE_REQ:
        {
            p_cell_id =
                &((son_anr_enable_disable_limited_mode_req_t *)p_msg)->
               enable_disable_limited_mode_req.src_cgi;
           break;
        }

        case SONANR_MEAS_CONFIG_OBJ_REMOVE_REQ:
        {
           p_cell_id =
               &((son_anr_meas_config_obj_remove_req_t*)p_msg)->remove_req.src_cgi;
           break;
        }
        
        /* SPR 20653 Fix Start */
        case SONANR_MODIFY_ATTRIBUTE_REQ:
        {
           p_cell_id =
               &((son_anr_modify_attribute_req_t *)p_msg)->
               modify_attribute_req.src_cgi;
           break;
        }
        /* SPR 20653 Fix End */

        default:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                   SON_ERROR,
                   "anr_get_cell_id_from_mif_msgs: Invalid api[%d] received"
                   " from [%d]\n", api, from);
           break;
   }
    } /* switch */

    SON_UT_TRACE_EXIT();
    return p_cell_id;
} /* anr_get_cell_id_from_mif_msgs */
 
/*****************************************************************************
 * Function Name  : anr_get_cell_id_from_msg
 * Inputs         : from     - Source Module Id
 *                : api      - procedure code of the message
 *                : p_msg    - pointer to the message
 * Outputs        : None
 * Returns        : son_intra_rat_global_cell_id_t*  - pointer to the cell id
 * Description    : This function determines the cell_id in the message received
 *                  from any module(internal/external) by ANR.
 ****************************************************************************/
static son_intra_rat_global_cell_id_t *
anr_get_cell_id_from_msg
(
son_u16 from,
son_u16 api,
son_u8 *p_msg
)
{
    son_intra_rat_global_cell_id_t *p_cell_id = SON_PNULL;


    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_msg)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_get_cell_id_from_msg:"
                "NULL message pointer recieved\n");

        SON_UT_TRACE_EXIT();
        return SON_PNULL;
    }
    
    if (SON_MIF_MODULE_ID == from)
    {
        p_cell_id = anr_get_cell_id_from_mif_msgs(p_msg, api, from);
    }
    else if (SON_RRM_MODULE_ID == from)
    {
        p_cell_id = anr_get_cell_id_from_rrm_msgs(p_msg, api, from);
    }
    else if (SON_X2_MODULE_ID == from && SONX2AP_HO_REPORT_IND == api)
    {
        p_cell_id =  (son_intra_rat_global_cell_id_t *)
                    &((son_ho_failure_report_t *)p_msg)->src_cgi;
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_get_cell_id_from_msg: Message received [%d]"
                       " from unknown module: [%d]\n", api, from);
    }

    SON_UT_TRACE_EXIT();
    return p_cell_id;
} /* anr_get_cell_id_from_msg */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_STOP */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_START */

/*****************************************************************************
 * Function Name  : anr_calculte_event_id_from_rrm_msgs
 * Inputs         : from     - Source Module id
 *                : api      - procedure code of the message
 * Outputs        : None
 * Returns        : son_anr_index_et - Index Id in the FSM table corresponding
 *                                     to the message recieved from RRM.
 * Description    : This function computes the index id in the FSM table
 *                  corresponding to the message recieved from RRM.
 ****************************************************************************/
static son_anr_index_et
anr_calculte_event_id_from_rrm_msgs
(
    son_u16               from,
    son_procedure_code_et api
)
{
   son_anr_index_et ret = IDX_NOT_AVAILABLE;


    SON_UT_TRACE_ENTER();

    switch ((rrm_son_message_resp_et)api)
    {
        case RRM_SON_REGISTER_RESP:
        {
           ret = IDX_RRM_REGISTER_RES;
           break;
        }

        case RRM_SON_DEREGISTER_RESP:
        {
           ret = IDX_RRM_DEREGISTER_RES;
           break;
        }

        case RRM_SON_CELL_STATE_CHANGE_IND:
        {
           ret = IDX_RRM_CELL_STATE_CHANGE_IND;
           break;
        }

        case RRM_SON_MEAS_CONFIG_RESP:
        {
           ret = IDX_RRM_SON_MEAS_CONFIG_RESP;
           break;
        }

        case RRM_SON_SET_ATTRIBUTE_RESP:
        {
           ret = IDX_RRM_SONANR_SET_ATTRIBUTE_RESP;
           break;
        }

        case RRM_SON_MEAS_RESULTS_IND:
        {
           ret = IDX_RRM_MEAS_RESULT_IND;
           break;
        }

        case RRM_SON_HO_REPORT:
        {
           ret = IDX_RRM_HO_REPORT;
           break;
        }

        case RRM_SON_HO_ATTEMPT_IND:
        {
           ret = IDX_RRM_HO_ATTEMPTS;
           break;
        }

        case RRM_SON_HO_FAILURE_REPORT:
        {
           ret = IDX_RRM_HO_FAILURE_REPORT;
           break;
        }

        case RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP:
        {
           ret = IDX_RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP;
           break;
        }

        default:
        {
           SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                   SON_ERROR,
                   "anr_calculte_event_id_from_rrm_msgs: Invalid api [%d] received from"
                " [%d]\n", api, from);
           break;
   }
    } /* switch */

  SON_UT_TRACE_EXIT(); 
  return ret;  
} /* anr_calculte_event_id_from_rrm_msgs */
 
/*****************************************************************************
 * Function Name  : anr_calculte_event_id_from_mif_msgs
 * Inputs         : from    - Source module id
 *                : api     -  procedure code of the message
 * Outputs        : None
 * Returns        : son_anr_index_et - Index id in  the FSM table corresponding to
 *                  the message recieved from MIF.
 * Description    : This function computes the index Id in  the FSM table 
 *                  corresponding to the message recieved from MIF.
 ****************************************************************************/
 static son_anr_index_et
anr_calculte_event_id_from_mif_msgs
(
 son_u16               from,
 son_procedure_code_et api
)
{
    son_anr_index_et ret = IDX_NOT_AVAILABLE;


    SON_UT_TRACE_ENTER();

    switch (api)
    {
        case SONANR_ENABLE_REQ:
        {
            ret = IDX_SONANR_ENABLE_REQ;
            break;
        }

        case SONANR_DISABLE_REQ:
        {
            ret = IDX_SONANR_DISABLE_REQ;
            break;
        }

        case SONANR_NR_ADD_REQ:
        {
            ret = IDX_SONANR_NR_ADD_REQ;
            break;
        }

        case SONANR_NR_DELETE_REQ:
        {
            ret = IDX_SONANR_NR_DELETE_REQ;
            break;
        }

        case SONANR_NR_UPDATE_REQ:
        {
            ret = IDX_SONANR_NR_UPDATE_REQ;
            break;
        }

        case SONANR_NR_RETRIEVE_REQ:
        {
            ret = IDX_SONANR_NR_RETRIEVE_REQ;
            break;
        }

        case SONANR_MEAS_CONFIG_REQ:
        {
            ret = IDX_SON_ANR_RRM_MEAS_CONFIG_REQ;
            break;
        }

        case SONANR_MODIFY_UE_COUNT_REQ:
        {
            ret = IDX_SONANR_MODIFY_UE_COUNT_REQ;
            break;
        }

        case SONANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ:
        {
            ret = IDX_SONANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ;
            break;
        }

        case SONANR_NR_DELETE_FROM_REMOVE_LIST_REQ:
        {
            ret = IDX_SONANR_NR_DELETE_FROM_REMOVE_LIST_REQ;
            break;
        }

        case SONANR_MODIFY_REMOVAL_ATTRIBUTES_REQ:
        {
            ret = IDX_SONANR_MODIFY_REMOVAL_ATTRIBUTES_REQ;
            break;
        }

        case SONANR_MODIFY_PCI_CONFUSION_CFG_REQ:
        {
            ret = IDX_SONANR_MODIFY_PCI_CONFUSION_CFG_REQ;
            break;
        }

        case SONANR_ENABLE_DISABLE_LIMITED_MODE_REQ:
        {
            ret = IDX_SONANR_ENABLE_DISABLE_LIMITED_MODE_REQ;
            break;
        }

        case SONANR_MEAS_CONFIG_OBJ_REMOVE_REQ:
        {
            ret = IDX_SON_ANR_RRM_MEAS_CONFIG_OBJ_REMOVE_REQ;
            break;
        }

        /* SPR 20653 Fix Start */
        case SONANR_MODIFY_ATTRIBUTE_REQ:
        {
            ret = IDX_SONANR_MODIFY_ATTRIBUTE_REQ;
            break;
        }
        /* SPR 20653 Fix End */

        default:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_calculte_event_id_from_mif_msgs: Invalid api [%d] received from"
                    " [%d]\n", api, from);
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
    return ret;    
} /* anr_calculte_event_id_from_mif_msgs */

/*****************************************************************************
 * Function Name  : anr_calculate_event_id
 * Inputs         : from  -Source Module Id
 *                : api   -procedure code of the message
 * Returns        : son_anr_index_et - Index Id in the FSM table corresponding 
 *                                     to the message recieved.
 * Description    : This function computes the index Id in the FSM table 
 *                  corresponding to the message recieved.
 ****************************************************************************/
static son_anr_index_et
anr_calculate_event_id
(
    son_u16               from,
	son_procedure_code_et api
)
 {
     son_anr_index_et ret = IDX_NOT_AVAILABLE;


    SON_UT_TRACE_ENTER();
    if (SON_RRM_MODULE_ID == from)
    {
        ret = anr_calculte_event_id_from_rrm_msgs(from, api);
    }
    else if (SON_MIF_MODULE_ID == from)
    {
        ret = anr_calculte_event_id_from_mif_msgs(from, api );
    }
    else if (SON_X2_MODULE_ID == from &&  SONX2AP_HO_REPORT_IND == api)
     {
        ret = IDX_X2_HO_FALIURE_REPORT; 
     }
     else
     {
         SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                 SON_ERROR,
                 "anr_calculate_event_id: Unexpected Msg [%d] received from"
            " [%d] undefined module", from, api);
     }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_calculate_event_id */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_STOP */

/*****************************************************************************
 * Function Name  : anr_log_handler
 * Inputs         : p_buf   Pointer to the message
 *                                : msg_id  message id
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
 *                  RELEASE_SON_BUFFER - Encapsulating shell should release
 *                                        the buffer
 *                  or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                                      the buffer
 * Description    : This is the message handler for ANR module and is called
 *                  when SMIF_INIT_CONFIG_REQ or SMIF_LOG_ENABLE_DISABLE_CMD or
 *                  SMIF_SET_LOG_LEVEL_CMD message is received
 ******************************************************************************/
static son_buf_retain_status_et
anr_log_handler
(
 son_u8  *p_buf,
 son_u16  msg_id
 )
{
    SON_UT_TRACE_ENTER();
    if (SON_PNULL ==  p_buf)
    {
        SON_LOG(SON_OAM_LOG_ON, p_son_anr_facility_name, SON_ERROR,
                "anr_log_handler:NULL message pointer recieved\n");

        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER; 
    }

    switch (msg_id)
    {
        case SMIF_INIT_CONFIG_REQ:
            {
            smif_init_config_req_t *p_msg = (smif_init_config_req_t *)p_buf;
                anr_set_log_mode(p_msg->log_enable);

            /* Print the current logging status(enable/disable)
             *  SON_TRUE: always print log,even if it is disabled. */
                SON_LOG(SON_OAM_LOG_ON, p_son_anr_facility_name, SON_BRIEF,
                            "Log enable/disable = %u",
                            p_msg->log_enable);
                if (SON_OAM_LOG_ON == anr_get_log_mode())
                {
                    /* SPR-13251 Fix Starts */
                    anr_set_log_level(p_msg->log_level);
                    /* SPR-13251 Fix Ends */

                    SET_MODULE_LOG_LEVEL(p_msg->log_level);
                }
                else
                {
                    /* SPR-13251 Fix Starts */
                    anr_set_log_level(SON_LOG_LEVEL_NONE);
                    /* SPR-13251 Fix Ends */


                    /* Set log level to NONE  */
                    SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
                }

            break;
        }

        case SMIF_LOG_ENABLE_DISABLE_CMD:
            {
                smif_log_enable_disable_cmd_t *p_msg = 
                    (smif_log_enable_disable_cmd_t *)p_buf;
                /* Coverity Fix: 19167 */
                anr_set_log_mode((son_oam_log_on_off_et)p_msg->log_enable);

            /* Print the current logging status(enable/disable)
             *  SON_TRUE: always print log, even if it is disabled. */
                SON_LOG(SON_OAM_LOG_ON, p_son_anr_facility_name, SON_BRIEF,
                        "anr_log_handler: Log enable/disable = %u",
                        p_msg->log_enable);

                if (SON_OAM_LOG_ON == anr_get_log_mode())
                {
                if (p_msg->bitmask & SMIF_MODULE_LOG_LEVEL_PRESENT)
                {
                    /* SPR-13251 Fix Starts */
                    anr_set_log_level(p_msg->log_level);
                    /* SPR-13251 Fix Ends */
                    SET_MODULE_LOG_LEVEL(p_msg->log_level);
                }
                }
                else
                {
                    /* SPR-13251 Fix Starts */
                    anr_set_log_level(SON_LOG_LEVEL_NONE);
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
                if (SON_OAM_LOG_ON == anr_get_log_mode())
                {
                    SON_LOG(SON_OAM_LOG_ON, p_son_anr_facility_name,
                            SON_BRIEF,
                            "anr_log_handler: Log level changed through msg: "
                            "SMIF_SET_LOG_LEVEL_CMD \n");
                    /* SPR-13251 Fix Starts */
                    anr_set_log_level(p_msg->log_level);
                    /* SPR-13251 Fix Ends */
                    SET_MODULE_LOG_LEVEL(p_msg->log_level);
                }

            break;
        }

        default:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_log_handler: Invalid message id [%d]\n", msg_id);
            break;
        }
    } /* switch */

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_log_handler */

/*****************************************************************************
 * Function Name  : anr_cell_info_ind_handler
 * Inputs         : p_cspl_hdr - Pointer to the the CSPL header information p_msg
 *                : p_buf      - Pointer to the message data received
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
 *                  RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ANR module and is called when
 *                  SMIF_CELL_INFO_IND message is received.
 ******************************************************************************/
static  son_buf_retain_status_et
anr_cell_info_ind_handler
(
     STACKAPIHDR *p_cspl_hdr,
	son_u8 *p_buf
)
{
    son_cell_info_ind_t          *p_msg                  = SON_PNULL;
    anr_cell_context_node_t  *p_cell_ctxt    = SON_PNULL;
    son_u8					idx 			= 0;
    son_cell_status_t 		cell_sts;
    son_anr_enable_req_t     enable_req;
    son_oam_log_on_off_et    log_mode        = anr_get_log_mode();
    /* CID Fix 109493 +*/
    /*SPR_19279_START*/
    son_u8 cell_index = 0;
    /*SPR_19279_END*/
    /* CID Fix 109493 -*/

    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_cspl_hdr || SON_PNULL == p_buf)
    {
        SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
                "anr_cell_info_ind_handler:NULL CSPL header"
                "pointer or message pointer recieved\n");
        SON_UT_TRACE_EXIT(); 
        return RELEASE_SON_BUFFER; 
    }
    /* CID Fix 109493 +*/
    /*SPR_19279_START*/
    cell_index = *((son_u8 *)(p_buf + sizeof(son_cell_info_ind_t)));
    /*SPR_19279_END*/
    /* CID Fix 109493 -*/

    p_msg             = (son_cell_info_ind_t *)p_buf;

    SON_MEMSET(&enable_req, 0, sizeof(enable_req));
    SON_MEMSET(&cell_sts, 0, sizeof(son_cell_status_t));

    if (!p_msg->cell_info_size)
    {
        SON_LOG(log_mode, p_son_anr_facility_name, SON_BRIEF,
                "anr_cell_info_ind_handler: Cell list is empty\n");
    }
    else
    {
        for (idx = 0; idx < p_msg->cell_info_size; idx++)
        {
            /*SPR_19279_START*/
            p_cell_ctxt = anr_add_new_cell_in_context(&p_msg->serving_cell[idx], cell_index);
            /*SPR_19279_END*/

            if (SON_PNULL != p_cell_ctxt)
            {
                if (SON_ENABLED == anr_get_global_mode())
                {
                    if (!anr_get_pending_res_expected_count())
                    {
                        anr_init_pending_res(son_generate_txn_id(),
                                SMIF_CELL_INFO_IND);
                    }

                    enable_req.transaction_id 	 = 
                        anr_get_pending_res_transaction_id();
                    enable_req.enable_req.bitmask |= SON_SRC_CGI_PRESENT;
                    enable_req.enable_req.bitmask |= SON_UE_CNT_PRESENT;
                    enable_req.enable_req.bitmask |= SON_REPORTNG_INTERVAL_PRESENT;
                    enable_req.enable_req.reporting_interval = SON_ANR_DEFAULT_REPORTING_INTERVAL;
                    enable_req.enable_req.ue_cnt = SON_ANR_DEFAULT_UE_COUNT;
                    enable_req.enable_req.src_cgi_list_size = 1;

                    SON_MEMCPY(&enable_req.enable_req.src_cgi[0],
                            &p_msg->serving_cell[idx].src_cgi,
                            sizeof(son_intra_rat_global_cell_id_t));

                    cell_sts.error_code 			= SON_ERR_TIMER_EXPIRED;

                    SON_MEMCPY(&cell_sts.cgi,
                            &p_msg->serving_cell[idx].src_cgi,
                            sizeof(son_intra_rat_global_cell_id_t));

                    if (SON_TRUE == anr_add_cell_in_pending_res(&cell_sts))
                    {
                        anr_cell_process_msg(p_cspl_hdr, IDX_SONANR_ENABLE_REQ,
                                &p_cell_ctxt->data,
                                (son_u8 *)&enable_req);

                        anr_set_context_state(ANR_STATE_ENABLING);
                    }
                    else
                    {
                        SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
                                "anr_cell_info_ind_handler: Cell [Cell Id: 0x%x] could not "
                                "be added to "
                                "pending response list\n",
                                son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->data.cell_id.cell_identity));
                    }
                }
                else
                {
                    /*ENB CONFIG CHANGE REQ SPR:START*/
                    son_x2_nrt_update_ind_t nrt_upd_ind     = {0};
                    son_anr_feature_state_change_ind_t state_change_ind; /*Alongwith SPR-8950*/
                    nrt_upd_ind.is_local_update = SON_FALSE;
                    anr_populate_send_x2_nrt_update_ind(&nrt_upd_ind,
                            &p_cell_ctxt->data.cell_id, SON_NRT_OP_ADD);

                    /*Alongwith SPR-8950-Start*/
                    SON_MEMSET(&state_change_ind, 0, sizeof(son_oam_feature_state_change_ind_t));
                    SON_MEMCPY(&state_change_ind.srv_cgi,
                            &p_cell_ctxt->data.cell_id,
                            sizeof(son_intra_rat_global_cell_id_t));

                    state_change_ind.module_id = SON_ANR_MODULE_ID;
                    state_change_ind.state = SON_STATE_DISABLED;
                    state_change_ind.cause = SON_ANR_MODE_DISABLED;

                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_BRIEF,
                            "Sending cell state change indication for cell"
                            "[Cell Id: %d]\n",
                            son_convert_char_cell_id_to_int_cell_id(state_change_ind.srv_cgi.cell_identity));

                    son_create_send_buffer((son_u8 *)&state_change_ind,
                            SON_ANR_MODULE_ID,
                            SON_MIF_MODULE_ID,
                            SONANR_FEATURE_STATE_CHANGE_IND,
                            sizeof(state_change_ind));
                    /*Alongwith SPR-8950-End*/

                    /*ENB CONFIG CHANGE REQ SPR:STOP*/
                    /* SPR 19838 : coverity 12772 start */
                    /* Code Deleted */
                    /* SPR 19838 : coverity 12772 end */
                }
            }
        }
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER; 
} /* anr_cell_info_ind_handler */

/*****************************************************************************
 * Function Name  : anr_global_enable_req_handler
 * Inputs         : p_cspl_hdr - Pointer to the the CSPL header information
 *                : p_msg      - Pointer to the message received
*                 : event_id   - Value of the event id
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
*                   invoked when SONANR_ENABLE_REQ message with
*                   no cell id is received.(global enable)
 *****************************************************************************/
static son_buf_retain_status_et
anr_global_enable_req_handler
(
 STACKAPIHDR *p_cspl_hdr,
 son_u8 *p_msg,
 son_anr_index_et event_id
 )
{
    /*Coverity CID-55058 Fix Starts*/
    son_bool_et             send_g_enable_res   = SON_TRUE;
    son_anr_enable_req_t    *p_buf              = (son_anr_enable_req_t  *)p_msg;
    anr_cell_context_t      *p_cell_ctxt        = SON_PNULL;
    anr_cell_context_node_t *p_cell_ctxt_node   = SON_PNULL;
    son_anr_enable_req_t     enable_req;
    son_anr_enable_res_t     g_enable_resp;
    son_cell_status_t        cell_sts;
    son_oam_log_on_off_et    log_mode        = anr_get_log_mode();
    anr_fsm_state_et         curr_fsm_state = anr_get_context_state();
    son_return_et            result = SON_FAILURE;


    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_cspl_hdr || SON_PNULL == p_msg)
    {
        SON_LOG(log_mode, p_son_anr_facility_name,
                SON_ERROR,
                "anr_global_enable_req_handler:NULL CSPL header pointer or"
                "message pointer recieved");

        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER; 
    }

    SON_MEMSET(&enable_req, 0, sizeof(enable_req));
    SON_MEMSET(&g_enable_resp, 0, sizeof(g_enable_resp));
    SON_MEMSET(&cell_sts.cgi, 0, sizeof(son_intra_rat_global_cell_id_t));
    cell_sts.error_code         = SON_ERR_ENABLE_IN_PROGRESS;

    anr_set_global_mode(SON_ENABLED);

    if (ANR_STATE_ENABLING != curr_fsm_state)
    {
        if (ANR_STATE_DISABLING == curr_fsm_state)
        {
            anr_reset_pending_res();
        }

        /*Pointer to the first context node*/
        p_cell_ctxt_node = anr_get_first_cell_ctxt();
        while (SON_PNULL != p_cell_ctxt_node)
        {
            /*Get the data part of the node*/
            p_cell_ctxt = &p_cell_ctxt_node->data;

            if (ANR_CELL_STATE_DISABLED  ==
                    p_cell_ctxt->current_cell_fsm_state ||
                    ANR_CELL_STATE_DISABLING ==
                    p_cell_ctxt->current_cell_fsm_state)
            {
                if (!anr_get_pending_res_expected_count())
                {
                    anr_init_pending_res(p_buf->transaction_id,
                            SONANR_ENABLE_RES);
                }

                enable_req.transaction_id =
                    anr_get_pending_res_transaction_id();
                enable_req.enable_req.bitmask |= SON_SRC_CGI_PRESENT;
                enable_req.enable_req.src_cgi_list_size = 1;

                SON_MEMCPY(&enable_req.enable_req.src_cgi[0],
                        &p_cell_ctxt->cell_id,
                        sizeof(son_intra_rat_global_cell_id_t));

                if (SON_UE_CNT_PRESENT & p_buf->enable_req.bitmask)
                {
                    enable_req.enable_req.ue_cnt = p_buf->enable_req.ue_cnt;
                }
                else
                {
                    enable_req.enable_req.bitmask |= SON_UE_CNT_PRESENT;
                    enable_req.enable_req.ue_cnt = SON_ANR_DEFAULT_UE_COUNT;
                }

                if (SON_REPORTNG_INTERVAL_PRESENT & p_buf->enable_req.bitmask)
                {
                    enable_req.enable_req.reporting_interval =
                        p_buf->enable_req.reporting_interval;
                }
                else
                {
                    enable_req.enable_req.bitmask |= SON_REPORTNG_INTERVAL_PRESENT;
                    enable_req.enable_req.reporting_interval = SON_ANR_DEFAULT_REPORTING_INTERVAL;
                }

                if (SON_LIMITED_MODE_PRESENT & p_buf->enable_req.bitmask)
                {
                    enable_req.enable_req.limited_mode =
                        p_buf->enable_req.limited_mode;
                }

                if ((SON_REMOVAL_CRITERIA_PRESENT & p_buf->enable_req.bitmask) &&
                        p_buf->enable_req.removal_criteria.bitmask)
                {
                    SON_MEMCPY(&enable_req.enable_req.removal_criteria,
                            &p_buf->enable_req.removal_criteria,
                            sizeof(enable_req.enable_req.removal_criteria));
                }

                if ((SON_PCI_CONFUSION_CFG_PRESENT & p_buf->enable_req.bitmask) &&
                        p_buf->enable_req.pci_confusion_cfg.bitmask)
                {
                    SON_MEMCPY(&enable_req.enable_req.pci_confusion_cfg,
                            &p_buf->enable_req.pci_confusion_cfg,
                            sizeof(enable_req.enable_req.pci_confusion_cfg));
                }
                /* SPR 20653 Fix Start */
                if (SON_UE_THROUGHPUT_DL_ANR_THRESHOLD_PRESENT & p_buf->enable_req.bitmask)
                {
                    enable_req.enable_req.ue_throughput_dl_anr_threshold = p_buf->enable_req.ue_throughput_dl_anr_threshold;
                }
                else
                {
                    enable_req.enable_req.bitmask |= SON_UE_THROUGHPUT_DL_ANR_THRESHOLD_PRESENT;
                    enable_req.enable_req.ue_throughput_dl_anr_threshold = SON_DEFAULT_UE_THROUGHPUT_DL_ANR_THRESHOLD;
                }

                if (SON_UE_THROUGHPUT_UL_ANR_THRESHOLD_PRESENT & p_buf->enable_req.bitmask)
                {
                    enable_req.enable_req.ue_throughput_ul_anr_threshold = p_buf->enable_req.ue_throughput_ul_anr_threshold;
                }
                else
                {
                    enable_req.enable_req.bitmask |= SON_UE_THROUGHPUT_UL_ANR_THRESHOLD_PRESENT;
                    enable_req.enable_req.ue_throughput_ul_anr_threshold = SON_DEFAULT_UE_THROUGHPUT_UL_ANR_THRESHOLD;
                }
                /* SPR 20653 Fix End */

                cell_sts.error_code = SON_ERR_TIMER_EXPIRED;
                result = SON_FAILURE;

                SON_MEMCPY(&cell_sts.cgi,
                        &p_cell_ctxt->cell_id,
                        sizeof(son_intra_rat_global_cell_id_t));

                if (SON_TRUE == anr_add_cell_in_pending_res(&cell_sts))
                {
                    anr_cell_process_msg(p_cspl_hdr, event_id,
                            p_cell_ctxt,
                            (son_u8 *)&enable_req);
                    if (SON_TRUE == send_g_enable_res)
                    {
                        send_g_enable_res = SON_FALSE;
                        anr_set_context_state(ANR_STATE_ENABLING);
                    }
                }
                else
                {
                    SON_LOG(log_mode, p_son_anr_facility_name,
                            SON_ERROR,
                            "anr_global_enable_req_handler: Cell [Cell Id: 0x%x] "
                            "could not be added to "
                            "pending response list\n",
                            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
                }
            }
            else
            {
                SON_LOG(log_mode, p_son_anr_facility_name,
                        SON_BRIEF,
                        "Cell with [Cell Id:  0x%x]"
                        "is in %s state\n",
                        son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                        SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->
                        current_cell_fsm_state]);
            }

            p_cell_ctxt_node = anr_get_next_node(p_cell_ctxt_node);
        }

        if (SON_TRUE == send_g_enable_res)
        {
            cell_sts.error_code = SON_NO_ERROR;
            result     = SON_SUCCESS;
            if (SON_PNULL == anr_get_first_cell_ctxt())
            {
                SON_LOG(log_mode, p_son_anr_facility_name,
                        SON_BRIEF,
                        "Global Enable Request"
                        " received but No Cell is Configured\n");
            }
            else
            {
                SON_LOG(log_mode, p_son_anr_facility_name,
                        SON_BRIEF,
                        "Global Enable Request"
                        " received but All Configured Cells are already Enabled/Enabling\n");
            }
        }
    }
    else
    {
        SON_LOG(log_mode, p_son_anr_facility_name,
                SON_BRIEF,
                "ANR Global enable is "
                "in progress\n");
    }

    if (SON_TRUE == send_g_enable_res)
    {
        /*Send the Enable Response*/
        g_enable_resp.transaction_id  = p_buf->transaction_id;
        g_enable_resp.enable_res.error_code = cell_sts.error_code;
        g_enable_resp.enable_res.result = result;
        /* Coverity Fix: 19173,19165 */
        son_create_send_buffer((son_u8 *)&g_enable_resp, SON_ANR_MODULE_ID,
                SON_MIF_MODULE_ID, SONANR_ENABLE_RES,
                sizeof(g_enable_resp));
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
    /*Coverity CID-55058 Fix Ends*/
} /* anr_global_enable_req_handler */

/*****************************************************************************
 * Function Name  : anr_global_disable_req_handler
 * Inputs         : p_cspl_hdr - Pointer to the the CSPL header information
 * Inputs         : p_msg      - Pointer to the message received
*                 : event_id   - Value of the event id
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
*                   invoked when SONANR_DISABLE_REQ message with
*                   no cell id is received(global disable)
 *****************************************************************************/
static son_buf_retain_status_et
anr_global_disable_req_handler
(
 STACKAPIHDR *p_cspl_hdr,
 son_u8 *p_msg,
 son_anr_index_et event_id
 )
{
    son_bool_et              send_g_disable_resp  = SON_TRUE;
    son_anr_disable_req_t    *p_buf                 =
                                            (son_anr_disable_req_t  *)p_msg;
    anr_cell_context_t       *p_cell_ctxt        = SON_PNULL;
    anr_cell_context_node_t  *p_cell_ctxt_node   = SON_PNULL;
    son_oam_log_on_off_et    log_mode            = anr_get_log_mode();
    anr_fsm_state_et         curr_fsm_state      = anr_get_context_state();
    son_cell_status_t        cell_sts;
    son_anr_disable_res_t    g_disable_resp;
    son_anr_disable_req_t    disable_req;
    son_return_et            result = SON_FAILURE;


    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_cspl_hdr || SON_PNULL == p_msg)
    {
        SON_LOG(log_mode, p_son_anr_facility_name,
                SON_ERROR,
                "anr_global_disable_req_handler:NULL CSPL header pointer or "
                "message pointer received");
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    SON_MEMSET(&g_disable_resp, 0, sizeof(g_disable_resp));
    SON_MEMSET(&disable_req, 0, sizeof(disable_req));
    SON_MEMSET(&cell_sts.cgi, 0, sizeof(son_intra_rat_global_cell_id_t));

    cell_sts.error_code     = SON_ERR_DISABLE_IN_PROGRESS;

    anr_set_global_mode(SON_DISABLED);

    if (ANR_STATE_DISABLING != curr_fsm_state)
    {
        if (ANR_STATE_ENABLING == curr_fsm_state)
        {
            anr_reset_pending_res();
        }

       /*Pointer to the anchor of first context node*/
        p_cell_ctxt_node = anr_get_first_cell_ctxt();
        while (SON_PNULL != p_cell_ctxt_node)
        {
            /*Get the data part of the node*/
            p_cell_ctxt = &p_cell_ctxt_node->data;

            if (ANR_CELL_STATE_ENABLED  ==
                            p_cell_ctxt->current_cell_fsm_state ||
                ANR_CELL_STATE_ENABLING ==
                            p_cell_ctxt->current_cell_fsm_state)
            {
                if (!anr_get_pending_res_expected_count())
                {
                    anr_init_pending_res(p_buf->transaction_id,
                                         SONANR_DISABLE_RES);
                }

                disable_req.transaction_id =
                                    anr_get_pending_res_transaction_id();
                disable_req.disable_req.bitmask = SON_ANR_DISABLE_SRC_CGI_PRESENT;

                 disable_req.disable_req.src_cgi_list_size = 1;

                 SON_MEMCPY(&disable_req.disable_req.src_cgi[0],
                             &p_cell_ctxt->cell_id,
                    sizeof(son_intra_rat_global_cell_id_t));

                 cell_sts.error_code = SON_ERR_DEREGISTERATION_FAILURE;
                 result = SON_FAILURE;

                 SON_MEMCPY(&cell_sts.cgi,
                            &p_cell_ctxt->cell_id,
                    sizeof(son_intra_rat_global_cell_id_t));

                if (SON_TRUE == anr_add_cell_in_pending_res(&cell_sts))
                 {
                    anr_cell_process_msg(p_cspl_hdr, event_id,
                        p_cell_ctxt,
                        (son_u8 *)&disable_req);
                    if (SON_TRUE == send_g_disable_resp)
                     {
                         send_g_disable_resp = SON_FALSE;
                         anr_set_context_state(ANR_STATE_DISABLING);
                     }
                 }
                 else
                 {
                     SON_LOG(log_mode, p_son_anr_facility_name,
                             SON_ERROR,
                             "anr_global_disable_req_handler: Cell [Cell Id: 0x%x] "
                             "could not be added to "
                             "pending response list\n",
                             son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
                 }
            }
            else
            {
                SON_LOG(log_mode, p_son_anr_facility_name,
                        SON_BRIEF,
                        "Cell with [Cell Id: 0x%x]"
                        "is in %s state\n",
                        son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                        SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->
                        current_cell_fsm_state]);
            }

            p_cell_ctxt_node = anr_get_next_node(p_cell_ctxt_node);
        }

        if (SON_TRUE == send_g_disable_resp)
        {
            cell_sts.error_code = SON_NO_ERROR;
            result = SON_SUCCESS;
            if (SON_PNULL == anr_get_first_cell_ctxt())
            {
                SON_LOG(log_mode, p_son_anr_facility_name,
                        SON_BRIEF,
                        "Global Disable Request"
                        " received but No Cell is Configured\n");
            }
            else
            {
                SON_LOG(log_mode, p_son_anr_facility_name,
                        SON_BRIEF,
                        "Global Disable Request"
                        " received but All Configured Cells are already Disabled/Disabling\n");
            }
        }
    }
    else
    {
        SON_LOG(log_mode, p_son_anr_facility_name,
                SON_BRIEF,
                "Global disable is "
                "in progress\n");
    }

    if (SON_TRUE == send_g_disable_resp)
    {
        /*Send the Disable Response*/
        g_disable_resp.transaction_id  = p_buf->transaction_id;
        g_disable_resp.disable_res.error_code = cell_sts.error_code;
        g_disable_resp.disable_res.result = result;
        
        /* Coverity Fix: 19172,19164 */
        son_create_send_buffer((son_u8 *)&g_disable_resp, SON_ANR_MODULE_ID,
            SON_MIF_MODULE_ID, SONANR_DISABLE_RES,
            sizeof(g_disable_resp));
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_global_disable_req_handler */

/*****************************************************************************
 * Function Name  : get_nr_from_eutran_scan_res
 * Inputs         : p_data       - Pointer to the EUTRAN scan data
 *                : earfcn       - EARFCN value
 *                : rssi         - RSSI value
 * Outputs        : p_nr         - Pointer to intra rat hash record
 * Returns        : son_bool_et - 
 *                  SON_TRUE - The neighbor record is  available
 *                             from the data received from EUTRAN scan.
 *                  or
 *                  SON_FALSE - The neighbor record is not available
 *                              from the data received from EUTRAN scan.
 * Description    : This is the function to populate eutran neighbors
 *                  from NMM Scan response for the EUTRAN frequencies.
******************************************************************************/
static son_bool_et
get_nr_from_eutran_scan_res
(
eutran_cell_num_t *p_data,
son_u32	earfcn,
son_u8 rssi,
anr_intra_rat_hash_rec_t *p_nr
)
{
    son_float  rsrq_in_db    = 0.0;
    son_float  meas_bw_in_db = 0.0;
    son_u8     plmn_index = SON_NULL;


    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_data || SON_PNULL == p_nr)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "get_nr_from_eutran_scan_res: Invalid input parameters. either p_data or p_nr is NULL!\n");
        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    SON_MEMSET(p_nr, 0, sizeof(anr_intra_rat_hash_rec_t));

    SON_MEMCPY(p_nr->neighbor_info.nbr_cgi.cell_identity,
            p_data->cell_Id, SON_CELL_ID_OCTET_SIZE);

    if (SON_MAX_NUM_PLMN >= p_data->num_plmn_id && 1 <= p_data->num_plmn_id)
    {
        SON_MEMCPY(&p_nr->neighbor_info.nbr_cgi.plmn_id,
                &p_data->plmn_identity_info[0].plmn_id, sizeof(plmn_id_t));

        if (1 < p_data->num_plmn_id)
        {
            p_nr->neighbor_info.bitmask   |= SON_PLMN_ID_PRESENT;

            p_nr->neighbor_info.num_plmn_id = p_data->num_plmn_id - 1;
            for (plmn_index = SON_NULL; plmn_index < (p_data->num_plmn_id - 1);
                    plmn_index++)
            {
                SON_MEMCPY( &p_nr->neighbor_info.plmn_id[plmn_index],
                        &p_data->plmn_identity_info[plmn_index + 1].plmn_id,
                        sizeof(son_cell_plmn_info_t));
            }
        }
    }

    SON_MEMCPY( p_nr->neighbor_info.tac, p_data->tac, SON_TAC_OCTET_SIZE);

    meas_bw_in_db   =  convert_meas_bw_in_db(p_data->allowed_meas_bandwidth);
    rsrq_in_db      =  meas_bw_in_db + (p_data->rsrp - 144) - (rssi - 144);

    /* Perform RSRQ measurement report mapping.
     * Possible range of rsrq is 0 to 34. */
    CONVERT_RSRQ_DB_TO_REPORTED_VALUE(rsrq_in_db, p_nr->neighbor_info.rsrq);

    p_nr->neighbor_info.pci	    = p_data->pci_id;
    p_nr->neighbor_info.earfcn	= earfcn;
    CONVERT_RSRP_TO_3GPP_REPORTED_VALUE(p_data->rsrp, p_nr->neighbor_info.rsrp);
    p_nr->neighbor_info.bitmask   |= SON_TAC_PRESENT;
    p_nr->neighbor_info.bitmask   |= SON_PCI_PRESENT;
    p_nr->neighbor_info.bitmask   |= SON_EARFCN_PRESENT;
    p_nr->neighbor_info.bitmask   |= SON_RSRP_PRESENT;
    p_nr->neighbor_info.bitmask   |= SON_RSRQ_PRESENT;
    p_nr->neighbor_info.bitmask   |= SON_ENABLE_NR_STATUS_PRESENT;
    p_nr->neighbor_info.bitmask   |= SON_ENABLE_HO_STATUS_PRESENT;
    p_nr->neighbor_info.nr_status = SON_NR_UNLOCKED; 
    p_nr->neighbor_info.ho_status = SON_HO_ALLOWED; 

    if (SON_TRUE == p_data->csg_indication && 0 == son_convert_char_cell_id_to_int_cell_id(p_data->csg_identity))
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "get_nr_from_eutran_scan_res: Invalid input parameters! cell_id is Zero\n");
        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    } 

    /*SPR-10230-Start-Access mode setting code has been removed as it will be set on pci range basis*/

    if (SON_TRUE == p_data->csg_indication && 0 != son_convert_char_cell_id_to_int_cell_id(p_data->csg_identity))
    {
        p_nr->neighbor_info.bitmask  |= SON_INTRA_CSG_ID_PRESENT;
        SON_MEMCPY( p_nr->neighbor_info.csg_identity,
                p_data->csg_identity,
                SON_CSG_ID_OCTET_SIZE );
        /*SPR 15799 Fix Start:Re-addding the access mode setting code*/
        p_nr->neighbor_info.access_mode = SON_CLOSED;
        p_nr->neighbor_info.bitmask  |= SON_ACCESS_MODE_PRESENT;
        /*SPR 15799 Fix Stop*/
    }

    if (SON_FALSE == p_data->csg_indication && 0 != son_convert_char_cell_id_to_int_cell_id(p_data->csg_identity))
    {
        p_nr->neighbor_info.bitmask  |= SON_INTRA_CSG_ID_PRESENT;
        SON_MEMCPY( p_nr->neighbor_info.csg_identity,
                p_data->csg_identity,
                SON_CSG_ID_OCTET_SIZE );

        /*SPR 15799 Fix Start*/
        p_nr->neighbor_info.access_mode = SON_HYBRID;
        p_nr->neighbor_info.bitmask  |= SON_ACCESS_MODE_PRESENT;
        /*SPR 15799 Fix Stop*/
    }

    /*SPR-10230-End*/
    /*SPR 15799 Fix Start*/
    anr_determine_enb_id(p_nr, &p_nr->enb_id);
    /*SPR 15799 Fix Stop*/

    /*Neighbors nbrs*/
    if ((p_data->presence_bitmask & SIB_TYPE_4_PRES_FLAG) && 0 < p_data->num_intra_freq_neighbor)
    {
        p_nr->num_intra_freq_neighbor = p_data->num_intra_freq_neighbor;
        SON_MEMCPY(p_nr->intra_freq_neighbor_list, 
                p_data->intra_freq_neighbor_list,
                sizeof(intra_freq_neighbor_info_t) * 
                SON_MAX_INTRA_FREQ_NEIGHBORS_IN_SIB4);
    }
    anr_set_the_ul_earfcn_dl_earfcn_of_eutran_neighbor(p_nr);

    SON_UT_TRACE_EXIT();
    return SON_TRUE;
} /* get_nr_from_eutran_scan_res */

/*****************************************************************************
 * Function Name  : get_nr_from_utran_scan_res
 * Inputs         : p_data       - Pointer to UTRAN scan data
 *                : uarfcn       - UARFCN value
 * Outputs        : p_nr         - Pointer to Inter rat hash record
 * Returns        : son_bool_et -
 *                  SON_TRUE - The neighbor record is available
 *                             from the UTRAN scan.
 *                  or
 *                  SON_FALSE - The neighbor record is not available
 *                               from the UTRAN scan.
 * Description    : This is the function to populate inter rat hash record  
 *                : from NMM Scan response for UTRAN frequencies.
******************************************************************************/
static
son_bool_et
get_nr_from_utran_scan_res
(
    utran_cell_num_t *p_data,
  U16     uarfcn,
  son_inter_rat_neighbor_info_t *p_nr
) 
{
    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_data || SON_PNULL == p_nr)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "get_nr_from_utran_scan_res: Invalid input parameters! Either p_data or p_nr is NULL\n");
        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    SON_MEMSET(p_nr, 0, sizeof(son_inter_rat_neighbor_info_t));

    p_nr->bitmask  =  SON_INTER_UTRAN_NEIGHBOR;

    SON_MEMCPY(p_nr->utran_neighbor.nbr_cgi.cell_identity,
        p_data->cell_Id, SON_CELL_ID_OCTET_SIZE );
    if (SON_MAX_NUM_PLMN >= p_data->num_plmn_id && 1 <= p_data->num_plmn_id)
    {
        SON_MEMCPY(&p_nr->utran_neighbor.nbr_cgi.plmn_id,
            &p_data->plmn_id[0], sizeof(plmn_id_t));

        if (1 < p_data->num_plmn_id)
        {
            p_nr->utran_neighbor.bitmask |= SON_UTRAN_PLMN_ID_PRESENT;
            p_nr->utran_neighbor.num_plmn_id = p_data->num_plmn_id - 1;

            SON_MEMCPY( p_nr->utran_neighbor.plmn_id, &p_data->plmn_id[1],
                    sizeof(p_nr->utran_neighbor.plmn_id));
        }
    }

    SON_MEMCPY(p_nr->utran_neighbor.lac, p_data->lac, SON_LAC_OCTET_SIZE );
    p_nr->utran_neighbor.rac = p_data->rac;
    p_nr->utran_neighbor.uarfcn = uarfcn;

    SON_MEMCPY(&p_nr->utran_neighbor.pci, &p_data->pci_id,
        sizeof(son_utran_physical_cell_id_t));

    if (SON_TRUE == p_data->csg_indication && 0 == son_convert_char_cell_id_to_int_cell_id(p_data->csg_identity))
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "get_nr_from_utran_scan_res: Invalid input parameters! cell_id is Zero\n");
        SON_UT_TRACE_EXIT();
        return SON_FALSE;
    }

    if (SON_TRUE == p_data->csg_indication && 0 != son_convert_char_cell_id_to_int_cell_id(p_data->csg_identity))
    {
        p_nr->utran_neighbor.bitmask  |= SON_UTRAN_CSG_ID_PRESENT | SON_UTRAN_ACCESS_MODE_PRESENT;

        SON_MEMCPY( p_nr->utran_neighbor.csg_identity, p_data->csg_identity,
                SON_CSG_ID_OCTET_SIZE );
        p_nr->utran_neighbor.access_mode = SON_CLOSED;
    }

    if (SON_FALSE == p_data->csg_indication && 0 != son_convert_char_cell_id_to_int_cell_id(p_data->csg_identity))
    {
        p_nr->utran_neighbor.bitmask  |= SON_UTRAN_CSG_ID_PRESENT | SON_UTRAN_ACCESS_MODE_PRESENT;

        SON_MEMCPY( p_nr->utran_neighbor.csg_identity, p_data->csg_identity,
                SON_CSG_ID_OCTET_SIZE );

        p_nr->utran_neighbor.access_mode = SON_HYBRID;        
    }

    if (SON_FALSE == p_data->csg_indication && 0 == son_convert_char_cell_id_to_int_cell_id(p_data->csg_identity))
    {
        p_nr->utran_neighbor.bitmask  |= SON_UTRAN_ACCESS_MODE_PRESENT;
        p_nr->utran_neighbor.access_mode  = SON_OPEN;
    }

    p_nr->utran_neighbor.rscp = p_data->rscp;
    p_nr->utran_neighbor.ecno = p_data->ecno;

    p_nr->utran_neighbor.nr_status = SON_NR_UNLOCKED;
    p_nr->utran_neighbor.ho_status = SON_HO_ALLOWED;

    p_nr->utran_neighbor.bitmask |= SON_UTRAN_PCI_PRESENT;
    p_nr->utran_neighbor.bitmask |= SON_UTRAN_LAC_PRESENT;
    p_nr->utran_neighbor.bitmask |= SON_UTRAN_RAC_PRESENT;
    p_nr->utran_neighbor.bitmask |= SON_UTRAN_UARFCN_PRESENT;
    p_nr->utran_neighbor.bitmask |= SON_UTRAN_RSCP_PRESENT;
    p_nr->utran_neighbor.bitmask |= SON_UTRAN_ECNO_PRESENT;
    p_nr->utran_neighbor.bitmask |= SON_UTRAN_ENABLE_NR_STATUS_PRESENT;
    p_nr->utran_neighbor.bitmask |= SON_UTRAN_ENABLE_HO_STATUS_PRESENT;

    SON_UT_TRACE_EXIT();
    return SON_TRUE;
} /* get_nr_from_utran_scan_res */

/*****************************************************************************
 * Function Name  : anr_process_eutran_scan_res_list
 * Inputs         : p_cell_ctxt         - Pointer to the cell context
 *                : p_eurtan_res_list   - Pointer to eutran scan response list
 * Outputs        : p_add_notification  - Pointer to the add notification
 *                                        structure to be sent to OAM,containing 
 *                                        the neighbors added in the NRT.
 *                : p_update_notification - Pointer to the update notification
 *                                          structure to be sent to OAM,
 *                                          containing the neighbors which are
 *                                          updated in NRT.
 *                : p_rrm_notif        - Pointer to the notification strcture 
 *                                       to RRM, containing the neighbors to
 *                                       be notified to RRM.
 * Returns        : son_bool_et - 
 *                  SON_TRUE -If any EUTRAN neighbor is added, or if PCI/EARFCN
 *                            changes of an existing neighbor.
 *                  or
 *                  SON_FALSE - For other remaining cases.
 * Description    : This function creates the EUTRAN NRs ,updates or add neighbors
 *                  in the intra rat NRT table and populate the notifcation messages
******************************************************************************/
static
son_bool_et
anr_process_eutran_scan_res_list
(
 anr_cell_context_t   *p_cell_ctxt,
 nmm_scan_res_list_t  *p_eurtan_res_list,
 son_anr_updated_nrt_info_t *p_add_notification,
 son_anr_updated_nrt_info_t *p_update_notification,
 son_anr_rrm_updated_nrt_info_t *p_rrm_notif
 )
{
    son_u16                         index           = 0;
    son_error_et                    error_code      = SON_NO_ERROR;
    start_scan_res_eutran_data_t    *p_res_node     = SON_PNULL;
    eutran_channel_num_t            *p_res_data     = SON_PNULL;  
    SON_LIST_NODE                   *p_node         = SON_PNULL;
    anr_intra_rat_hash_rec_t        nr_entry;
    son_global_cell_id_t            gl_cell_id;
    son_bool_et                     ret = SON_FALSE;
    anr_intra_rat_hash_rec_t       *p_hash_rec     = SON_PNULL;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&gl_cell_id, 0, sizeof(son_global_cell_id_t));
    gl_cell_id.bitmask |= SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;

    p_node = get_first_list_node(p_eurtan_res_list);
    while (SON_PNULL != p_node)
    {
        p_res_node = YMEMBEROF(start_scan_res_eutran_data_t, s_node, p_node);

        p_res_data =  &p_res_node->scan_res_data;
        for (index = 0; index < p_res_data->num_of_cells; index++)
        {
            if (p_res_data->cell[index].presence_bitmask & (SIB_TYPE_1_PRES_FLAG | SIB_TYPE_2_PRES_FLAG))
            {
                son_bool_et ret_valz = SON_FALSE;
                error_code = SON_NO_ERROR;

                ret_valz =   get_nr_from_eutran_scan_res(&p_res_data->cell[index],
                        p_res_data->earfcn, 
                        p_res_data->rssi, &nr_entry);

                if (SON_TRUE == ret_valz)
                {
		    /*SPR 15438 Lines deleted*/

                    /*SPR 15438 - Start */

                    anr_intra_rat_find_nr(&p_cell_ctxt->intra_rat_neighbor_table,
                            &nr_entry.neighbor_info.nbr_cgi,
                            &error_code);
                    /*SPR 15438 - End*/

		    if (SON_NO_ERROR == error_code) /* SPR 15438: if condition modified */
                    {
                        p_hash_rec =  anr_intra_rat_modify_nr(p_cell_ctxt,
                                &nr_entry,
                                SON_FALSE,
                                &error_code,
                                p_add_notification,
                                p_update_notification,
                                p_rrm_notif,
                                SON_TRUE,
                                SON_FALSE);
                        if (SON_PNULL != p_hash_rec &&
                            ((p_hash_rec->neighbor_info.pci != nr_entry.neighbor_info.pci) ||
                             (p_hash_rec->neighbor_info.earfcn != nr_entry.neighbor_info.earfcn)||
			       /*SPR 16523 fix start*/
			      (p_hash_rec->neighbor_info.tac != nr_entry.neighbor_info.tac )))
			       /*SPR 16523 fix stop*/
                        {
                            ret = SON_TRUE;
                        }
                    }
		    /*SPR 15438 - Start*/
                    /*Report the detected neighbors to OAM*/
                    else
                    {
                        if (p_add_notification)
                        {
                            /*SPR 15799 Fix Start*/
                            son_anr_set_x2_status_for_new_neighbor(p_cell_ctxt,&nr_entry,SON_PNULL);
                            /*SPR 15799 Fix Stop*/
                            p_add_notification->nrt_op = SON_NRT_OP_ADD;
                            anr_construct_oam_updated_ncl(&p_add_notification->nr_list,
                                    &nr_entry.neighbor_info,
                                    SON_FALSE);
                        }
                    }
                    /*SPR 15438 - End*/

                }
            }
        }

        p_node = get_next_list_node(&p_res_node->s_node);
    }

    /* Spr 17753 Changes Start */ 
     anr_find_conflict_in_nrt_n_send_conflict_ind(p_cell_ctxt);
    /* Spr 17753 Changes End */
    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_process_eutran_scan_res_list */

/*****************************************************************************
 * Function Name  : anr_process_utran_scan_res_list
 * Inputs         : p_cell_ctxt          - Pointer to the cell context
 *                : p_urtan_res_list     - Pointer to utran response list
 * Outputs        : p_add_notification   - Pointer to the add notification
 *                                          structure to be sent to OAM, if any 
 *                                          UTRAN neighbor is added in the NRT.
 * Outputs        : p_update_notification - Pointer to the update notification
 *                                          structure to be sent to OAM if any 
 *                                           UTRAN neighbor is updated in NRT
 *                : p_rrm_notif          - Pointer to RRM notification structure,
 *                                         containing the UTRAN neighbors to be
 *                                         notified to RRM
 * Return         : son_void_t
 * Description    : This function creates the UTRAN NRs,add or update the UTRAN 
 *                  neighbors in the inter rat NRT table and populate
 *                  the notification  messages
******************************************************************************/
static
son_void_t
anr_process_utran_scan_res_list
(
  anr_cell_context_t   *p_cell_ctxt,
  nmm_scan_res_list_t  *p_urtan_res_list,
  son_anr_updated_nrt_info_t *p_add_notification,
  son_anr_updated_nrt_info_t *p_update_notification,
  son_anr_rrm_updated_nrt_info_t *p_rrm_notif
)
{
    son_u16                       index = 0;
    son_error_et                  error_code = SON_NO_ERROR;
    start_scan_res_utran_data_t   *p_res_node          = SON_PNULL;
    utran_channel_num_t           *p_res_data          = SON_PNULL;  
    SON_LIST_NODE                 *p_node            = SON_PNULL;
    anr_inter_rat_hash_rec_t      nr_entry;
    son_inter_rat_neighbor_info_t inter_rat_nbr_info;


    SON_UT_TRACE_ENTER();

    p_node = get_first_list_node(p_urtan_res_list);
    while (SON_PNULL != p_node)
    {
        p_res_node = YMEMBEROF(start_scan_res_utran_data_t, s_node, p_node);
        p_res_data =  &p_res_node->scan_res_data;
        for (index = 0; index < p_res_data->num_of_cells; index++)
        {
            son_bool_et ret_valz = SON_FALSE;
            ret_valz =  get_nr_from_utran_scan_res( &p_res_data->cell[index],
                p_res_data->uarfcn, &inter_rat_nbr_info);

            if (SON_TRUE == ret_valz)
            {
                anr_convert_inter_rat_nr_to_inter_rat_hash_rec(&inter_rat_nbr_info, &nr_entry);

                if (SON_INTER_UTRAN_NEIGHBOR != inter_rat_nbr_info.bitmask)
                {
                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR, "anr_process_utran_scan_res_list:Scanned Cell"
                            "belongs to different RAT.Scanning for the Inter RATs other"
                            " than UTRAN is not supported.\n");
                    continue;
                }

                error_code = SON_NO_ERROR;

                anr_inter_rat_insert_nr(
                        p_cell_ctxt,
                    &nr_entry, SON_TRUE, &error_code, p_add_notification, p_rrm_notif);

                if (SON_ERR_ANR_DUPLICATE_NR == error_code)
                {
                    /*SPR 17777 +-*/ 
                    anr_inter_rat_modify_nr(
                            p_cell_ctxt,
                            &nr_entry, SON_FALSE, &error_code, p_update_notification, p_rrm_notif);
                }
            }
        }

        p_node = get_next_list_node(&p_res_node->s_node);
    }

    SON_UT_TRACE_EXIT();
    return;
} /* anr_process_utran_scan_res_list */

/*****************************************************************************
 * Function Name  : anr_scan_res_handler
 * Inputs         : p_msg        - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
 *                  RELEASE_SON_BUFFER - Encapsulating shell should release
 *                                        the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                                     the buffer.
 * Description    : This is the message handler for ANR module and is invoked
 *                  when SONNMM_START_SCAN_RES message is received.
******************************************************************************/
static son_buf_retain_status_et
anr_scan_res_handler
(
 son_u8 *p_msg
)
{
    son_anr_scan_res_t		   	res					 = {0};
    anr_cell_context_node_t 	*p_cell_ctxt_node 	 = SON_PNULL;
    anr_cell_context_t          *p_cell_ctxt      	 = SON_PNULL;
    son_oam_log_on_off_et       log_mode             = anr_get_log_mode();
    son_anr_updated_nrt_info_t    *p_update_notification = SON_PNULL;
    son_anr_updated_nrt_info_t    *p_add_notification    = SON_PNULL;
    son_anr_rrm_updated_nrt_info_t    *p_rrm_notification = SON_PNULL;
    son_bool_et                  ret = SON_FALSE;
    son_x2_nrt_update_ind_t        nrt_upd_ind         = {0};


    SON_UT_TRACE_ENTER();
    SON_MEMSET(&nrt_upd_ind, 0, sizeof(son_x2_nrt_update_ind_t));

    anr_decrement_pending_scan_res_count();

    son_nmm_start_scan_res_t *p_scan_res   = (son_nmm_start_scan_res_t *)p_msg;
    if (SON_PNULL == p_msg)
    {
        SON_LOG(log_mode, p_son_anr_facility_name,
            SON_ERROR, "anr_scan_res_handler:NULL message pointer received\n");

        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    if (SON_FAILURE == p_scan_res->start_scan_res.son_resp)
    {
        anr_set_pending_scan_res_error_code(p_scan_res->start_scan_res.son_error);
    }

    if (!anr_get_pending_scan_res_count())
    {
        res.transaction_id      = p_scan_res->transaction_id;
        if (SON_NO_ERROR != (res.scan_res.error_code = anr_get_pending_scan_res_error_code()))
        {
            res.scan_res.result = SON_FAILURE;
        }
        else
        {
            res.scan_res.result = SON_SUCCESS;
            res.scan_res.error_code = SON_NO_ERROR;
        }

        son_create_send_buffer((son_u8 *)&res,
            SON_ANR_MODULE_ID, SON_MIF_MODULE_ID,
            SONANR_NR_SCAN_RES, sizeof(res));
    }

    if (SON_FAILURE == p_scan_res->start_scan_res.son_resp)
    {
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    if (SON_PNULL == p_scan_res->start_scan_res.p_eutran_scan_res_list &&
        SON_PNULL == p_scan_res->start_scan_res.p_utran_scan_res_list)
    {
        SON_LOG(log_mode, p_son_anr_facility_name,
            SON_BRIEF, "No neighbors found!\n");
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    p_update_notification = (son_anr_updated_nrt_info_t *)
        son_mem_get(sizeof(son_anr_updated_nrt_info_t));
    if (SON_PNULL == p_update_notification)
    {
        SON_LOG(log_mode, p_son_anr_facility_name,
            SON_BRIEF, "Memory Allocation failure\n");
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    p_add_notification = (son_anr_updated_nrt_info_t *)
        son_mem_get(sizeof(son_anr_updated_nrt_info_t));
    if (SON_PNULL == p_add_notification)
    {
        if (SON_PNULL != p_update_notification)
        {
            son_mem_free(p_update_notification);
        }

        SON_LOG(log_mode, p_son_anr_facility_name,
            SON_BRIEF, "Memory Allocation failure\n");
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    p_rrm_notification = (son_anr_rrm_updated_nrt_info_t *)son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));
    if (SON_PNULL == p_rrm_notification)
    {
        if (SON_PNULL != p_update_notification)
        {
            son_mem_free(p_update_notification);
        }

        if (SON_PNULL != p_add_notification)
        {
            son_mem_free(p_add_notification);
        }

        SON_LOG(log_mode, p_son_anr_facility_name,
            SON_BRIEF, "Memory Allocation failure\n");
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    p_cell_ctxt_node = anr_get_first_cell_ctxt();

    while (SON_PNULL != p_cell_ctxt_node)
    {
        /*Get the data part of the context node*/
        p_cell_ctxt = &p_cell_ctxt_node->data;

        if (ANR_CELL_STATE_ENABLED == p_cell_ctxt->current_cell_fsm_state)
        {
            SON_MEMSET(p_update_notification, 0, sizeof(son_anr_updated_nrt_info_t));
            SON_MEMSET(p_add_notification, 0, sizeof(son_anr_updated_nrt_info_t));
            SON_MEMSET(p_rrm_notification, 0, sizeof(son_anr_rrm_updated_nrt_info_t));
            SON_MEMCPY(&p_update_notification->src_cgi, &p_cell_ctxt->cell_id,
                sizeof(son_intra_rat_global_cell_id_t));
            SON_MEMCPY(&p_add_notification->src_cgi, &p_cell_ctxt->cell_id,
                sizeof(son_intra_rat_global_cell_id_t));
            SON_MEMCPY(&p_rrm_notification->src_cgi, &p_cell_ctxt->cell_id,
                sizeof(son_intra_rat_global_cell_id_t));

            /*Intra rat scanning list processing*/
            if (SON_PNULL !=  p_scan_res->start_scan_res.p_eutran_scan_res_list)
            {
                ret  = anr_process_eutran_scan_res_list( p_cell_ctxt,
                        p_scan_res->start_scan_res.p_eutran_scan_res_list,
                        p_add_notification,
                        p_update_notification,
                        p_rrm_notification);          
            }
            else  /*Inter rat scanning list processing*/
            {
                anr_process_utran_scan_res_list( p_cell_ctxt,
                        p_scan_res->start_scan_res.p_utran_scan_res_list,
                        p_add_notification,
                        p_update_notification,
                        p_rrm_notification);
            }  
            /*+SPR 18853 FIX*/
            p_add_notification->nrt_op = SON_NRT_NMM_OP_ADD;
            /*-SPR 18853 FIX*/

            /* Check if update notification is to be sent to OAM */
            anr_send_updated_ncl_to_oam(p_add_notification);
            anr_send_updated_ncl_to_oam(p_update_notification);
            anr_send_updated_ncl_to_rrm(p_rrm_notification,
                    son_convert_char_cell_id_to_int_cell_id((p_rrm_notification->src_cgi.
                            cell_identity)));

            if (SON_TRUE == ret)
            {
                nrt_upd_ind.is_local_update = SON_FALSE;
                anr_populate_x2_nrt_update_ind(&nrt_upd_ind,
                    &p_cell_ctxt->cell_id, SON_NRT_OP_UPDATE);

                ret = SON_FALSE;
            }
        }
        else
        {
            SON_LOG(log_mode, p_son_anr_facility_name,
                    SON_BRIEF,
                    "Cell [Cell Id: %d]"
                    "is in %s state, no neighbors added\n",
                    son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                    SON_ANR_CELL_FSM_STATES_NAMES[p_cell_ctxt->
                    current_cell_fsm_state]);
        }

        p_cell_ctxt_node = anr_get_next_node(p_cell_ctxt_node);
    }

    if (nrt_upd_ind.num_srv_cells_list_size && SON_TRUE == is_x2_connection_available())
    {
        son_create_send_buffer((son_u8 *)&nrt_upd_ind, SON_ANR_MODULE_ID,
            SON_X2_MODULE_ID, SONX2_NRT_UPDATE_IND,
                sizeof(nrt_upd_ind));

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_BRIEF, 
                "SONX2 NRT UPDATE sent to X2 ");
    }

    if (SON_PNULL != p_update_notification)
    {
        son_mem_free(p_update_notification);
    }

    if (SON_PNULL != p_add_notification)
    {
        son_mem_free(p_add_notification);
    }

    if (SON_PNULL != p_rrm_notification)
    {
        son_mem_free(p_rrm_notification);    
    }
    
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_scan_res_handler */

/*****************************************************************************
 * Function Name  : anr_pci_in_use_list_req_handler
 * Inputs         : p_msg        - Pointer to the message received
 * Outputs        : None
 * Returns        : son_buf_retain_status_et-
 *                  RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for SONANR_PCI_IN_USE_LIST_REQ
 *                  request message
 *                : message.Depending on the pci search level, the PCI values
 *                  will collected.
 ******************************************************************************/
static son_buf_retain_status_et
anr_pci_in_use_list_req_handler
(
 son_u8 *p_msg
)
{
    son_buf_retain_status_et ret = RELEASE_SON_BUFFER;   
    /* Spr 17753 Changes Start*/
    son_anr_pci_in_use_list_res_t *p_res = SON_NULL;
    /* Spr 17753 Changes End*/
    anr_cell_context_node_t   *p_ctxt_node = SON_PNULL;  
    son_oam_log_on_off_et   log_mode =  anr_get_log_mode();
    son_anr_pci_in_use_list_req_t *p_req = (son_anr_pci_in_use_list_req_t *)p_msg;
    anr_intra_rat_hash_nr_tuple_t  *p_intra_rat_nbr_hash_tpl = SON_PNULL;
    /* Spr 17753 Changes Start*/
    /* Code Removed */
    /* Spr 17753 Changes End*/
    son_bool_et is_data_present = SON_FALSE;


    SON_UT_TRACE_ENTER();
  

    do
    {
        /* Spr 17753 Changes Start*/
        p_res = (son_anr_pci_in_use_list_res_t *)son_mem_get(sizeof(son_anr_pci_in_use_list_res_t));
        if( SON_PNULL == p_res )
        {
            /* unable to allocate the memory */
            SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
                    "Unable to alloacte the memory to p_res\n" ); 
            break; 
        }
        /* - coverity 98780 */
        SON_MEMSET(p_res, 0, sizeof(son_anr_pci_in_use_list_res_t));
        p_res->transaction_id = *(son_u16 *)p_msg;
        p_res->result     = SON_FAILURE;
        p_res->error_code = SON_ERR_EMPTY_PCI_LIST;
        SON_MEMCPY(&p_res->src_cgi, &p_req->src_cgi,
                sizeof(son_intra_rat_global_cell_id_t));
        /* Spr 17753 Changes End*/

        if (SON_PCI_DISABLE == anr_get_pci_module_mode())
        {
            SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
                    "anr_pci_in_use_list_req_handler:"
                    "PCI module is not registered with ANR\n" );
            break;
        }

        p_ctxt_node  = anr_get_first_cell_ctxt(); 
        if (SON_PNULL == p_ctxt_node)
        {
            SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
                    "anr_pci_in_use_list_req_handler:" 
                    "Cell context list is empty\n");
            break; 
        }

        if (SON_FAILURE == anr_check_fsm_state(ANR_CELL_STATE_ENABLED))
        {
            /* Spr 17753 Changes Start*/
            p_res->error_code = SON_ERR_ANR_IS_DISABLED;
            /* Spr 17753 Changes End*/
            SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
                    "anr_pci_in_use_list_req_handler:"
                    "ANR is not enabled for atleast one cell\n");
            break;
        }

        SON_LOG(log_mode, p_son_anr_facility_name, SON_BRIEF,
            "Requested PCI search level: [%d]\n", p_req->level);

        while (SON_PNULL != p_ctxt_node)
        {
            if (ANR_CELL_STATE_ENABLED ==  p_ctxt_node->data.current_cell_fsm_state)
            {
                if (p_req->earfcn == anr_get_earfcn(&p_ctxt_node->data))
                {
                    /* Spr 17753 Changes Start*/
                    /* filing serving cell pci */
                    p_res->in_use_pci_list.served_cell_pci = p_ctxt_node->data.pci_value;
                    /* Spr 17753 Changes End*/
                    is_data_present = SON_TRUE;
                }

                p_intra_rat_nbr_hash_tpl = anr_get_first_nr_from_intra_rat_hash_table(
                        &p_ctxt_node->data.intra_rat_neighbor_table);
                while (SON_PNULL != p_intra_rat_nbr_hash_tpl)
                {
                    if (p_intra_rat_nbr_hash_tpl->data.neighbor_info.earfcn == p_req->earfcn)
                    {
                        /* Spr 17753 Changes Start*/
                        p_res->in_use_pci_list.
                            pci_list_level_1[p_res->in_use_pci_list.num_pci_in_use_w_first_degree++] = 
                            p_intra_rat_nbr_hash_tpl->data.neighbor_info.pci;

                        is_data_present = SON_TRUE;

                        SON_LOG(log_mode, p_son_anr_facility_name, SON_BRIEF,
                                "pci in use with first degree CellID[0x%x]: %d",
                                son_convert_char_cell_id_to_int_cell_id
                                (p_intra_rat_nbr_hash_tpl->data.neighbor_info.nbr_cgi.cell_identity),
                                p_intra_rat_nbr_hash_tpl->data.neighbor_info.pci);
                        /* Spr 17753 Changes End*/
                    }

                    if (SON_LEVEL_2 == p_req->level  && list_count(
                            &p_intra_rat_nbr_hash_tpl->data.anr_nr_nbr_list))
                    {
                        anr_nr_nbr_list_node_t *p_node =  anr_get_first_nr_nbr(
                            &p_intra_rat_nbr_hash_tpl->data.anr_nr_nbr_list);

                        while (p_node != SON_PNULL)
                        {
                            if (p_node->nr_nbr_data.earfcn ==  p_req->earfcn)
                            {
                                /* Spr 17753 Changes Start*/
                                p_res->in_use_pci_list.
                                    pci_list_level_2[p_node->nr_nbr_data.pci] = SON_TRUE;

                                SON_LOG(log_mode, p_son_anr_facility_name, SON_BRIEF,
                                        "pci inuse with second degree nbr : %d"
                                        " of first degree neighbor CellId[0x%x]",
                                         p_node->nr_nbr_data.pci,
                                        son_convert_char_cell_id_to_int_cell_id
                                        (p_intra_rat_nbr_hash_tpl->data.neighbor_info.nbr_cgi.cell_identity));
                                /* Spr 17753 Changes End*/
                                is_data_present = SON_TRUE;
                            }

                            p_node = anr_get_next_nr_nbr(p_node);
                        }
                    }

                    p_intra_rat_nbr_hash_tpl = anr_get_next_nr_from_intra_rat_hash_table(
                            &p_ctxt_node->data.intra_rat_neighbor_table,
                            &p_intra_rat_nbr_hash_tpl->anchor);
                }
            }

            p_ctxt_node = anr_get_next_node(p_ctxt_node);
        }
    } while (0);

    if (SON_TRUE == is_data_present)
    {
        /* Spr 17753 Changes Start*/
        p_res->result     =  SON_SUCCESS;
        p_res->error_code =  SON_NO_ERROR;
        /* Spr 17753 Changes End*/
    }

    /* Spr 17753 Changes Start*/
    son_create_send_buffer((son_void_t *)p_res, SON_ANR_MODULE_ID,
        SON_APPS_MODULE_ID, SONANR_PCI_IN_USE_LIST_RES,
        sizeof(son_anr_pci_in_use_list_res_t));

    /* Free the allocated buffer */
    son_mem_free(p_res);
    /* Spr 17753 Changes End*/

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_pci_in_use_list_req_handler */

/*****************************************************************************
 * Function Name  : anr_handle_pci_reg_dereg_req
 * Inputs         : p_msg        -Pointer to the message received
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
 *                  RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for SONANR_PCI_REG_DEREG_REQ
 *                : message
 ******************************************************************************/
 static son_buf_retain_status_et
 anr_pci_reg_dereg_req_handler
 (
  son_u8 *p_msg
 )
{
    son_buf_retain_status_et ret = RELEASE_SON_BUFFER;
    son_anr_pci_reg_dereg_req_t *p_req
        = (son_anr_pci_reg_dereg_req_t *)p_msg;
    son_anr_pci_reg_dereg_res_t  res; 

    son_pci_mode_et crnt_pci_mode = SON_PCI_DISABLE;


    SON_UT_TRACE_ENTER();

    res.transaction_id  = p_req->transaction_id;
    res.result = SON_SUCCESS;
    res.error_code = SON_NO_ERROR;
    crnt_pci_mode = anr_get_pci_module_mode();

    if (SON_PCI_ENABLE == p_req->pci_mode)
    {
        if (SON_PCI_ENABLE == crnt_pci_mode)
        {
            res.result = SON_FAILURE;
            res.error_code = SON_ERR_ALREADY_REGISTERED;
        }
        else
        {
            anr_set_pci_module_mode(SON_PCI_ENABLE);
        }             
    }
    else
    {
        if (SON_PCI_DISABLE == crnt_pci_mode)
        {
            res.result = SON_FAILURE;
            res.error_code = SON_ERR_ALREADY_DEREGISTERED;
        } 
        else
        {
            anr_set_pci_module_mode(SON_PCI_DISABLE);
        }
    } 

    res.pci_mode = p_req->pci_mode;

    if (SON_SUCCESS == res.result)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_BRIEF,
         "Pci mode value is changed from  [%d]  to [%d] ",
            crnt_pci_mode, p_req->pci_mode );
    }
   else
   {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
         " anr_pci_reg_dereg_req_handler : Pci mode value is not changed. It is still [%d] ",
           crnt_pci_mode );
   }

    son_create_send_buffer((son_void_t *)&res, SON_ANR_MODULE_ID,
        SON_APPS_MODULE_ID, SONANR_PCI_REG_DEREG_RES,
            sizeof(res)); 

    SON_UT_TRACE_EXIT();

    return ret; 
} /* anr_pci_reg_dereg_req_handler */

/*****************************************************************************
 * Function Name  : anr_process_apps_msgs
 * Inputs         : p_msg       - Pointer to the message received
 *                : msg_id      - procedure code
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
 *                  RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ANR module handling APPS(PCI)
 *                : messages
 ******************************************************************************/
 static son_buf_retain_status_et
 anr_process_apps_msgs
 (
   son_u8 *p_msg,
   son_u16 msg_id
 )
{
    SON_UT_TRACE_ENTER();

    son_buf_retain_status_et ret = RELEASE_SON_BUFFER;
    if (SON_PNULL == p_msg)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_process_apps_msgs: NULL message pointer received\n");

        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    switch (msg_id)
    {
        case SONANR_PCI_REG_DEREG_REQ:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                    "APPS->ANR:SONANR_PCI_REG_DEREG_REQ");
            ret  = anr_pci_reg_dereg_req_handler(p_msg);
            break;
        }

        case SONANR_PCI_IN_USE_LIST_REQ:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                    "APPS->ANR:SONANR_PCI_IN_USE_LIST_REQ");
            ret = anr_pci_in_use_list_req_handler(p_msg); 
            break;
        }

        default:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "Invalid api [%d] received\n",
                    msg_id);
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_process_apps_msgs */

/*****************************************************************************
 * Function Name  : anr_process_nmm_msgs
 * Inputs         : p_msg       - Pointer to the message received
 *                : msg_id      - procedure code
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
 *                  RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ANR module and is
 *                  used to handle the NMM messages
 ******************************************************************************/
static son_buf_retain_status_et
anr_process_nmm_msgs
(
 son_u8 *p_msg,
 son_u16 msg_id
 )
{
   son_buf_retain_status_et ret = RELEASE_SON_BUFFER;


   SON_UT_TRACE_ENTER();

    if (SONNMM_START_SCAN_RES  == msg_id)
   {
       ret = anr_scan_res_handler(p_msg);
   }
   else
   {
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
   		"anr_process_nmm_msgs: Unexpected Message [%d] Received from module\n",
  	    		msg_id);
   }

   SON_UT_TRACE_EXIT();
   return ret;
} /* anr_process_nmm_msgs */

/* LTE_SON_KLOCWORK_WARN_6JULY_2012_START */

/*****************************************************************************
* Function Name  : anr_create_enable_disable_req_nack
* Inputs         : p_msg      -   Pointer to the message received
*                  api        -   API Id
*                  length     -   length of the enable/disable response message  
*                  error_code -   Error code to be sent
* Outputs        : None
* Returns        : son_u8 *   - Pointer to the enable/disable NACK reponse.
 *                              Address of the first byte of the response
* Description    : This is the function to create the enable/disable 
*                  NACK response
******************************************************************************/
static son_u8 *
anr_create_enable_disable_req_nack
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
        if (SONANR_ENABLE_RES == api)
        {
            SON_MEMSET(p_resp, 0, sizeof(son_anr_enable_res_t));
            ((son_anr_enable_res_t *)p_resp)->transaction_id =
                *((son_u16 *)p_msg);

            /* Filling the response structure */
            ((son_anr_enable_res_t *)p_resp)->enable_res.result = SON_FAILURE;
            ((son_anr_enable_res_t *)p_resp)->enable_res.error_code = error_code;
        }
        
        /* Creating nack response for DISABLE REQ */
        else if (SONANR_DISABLE_RES == api)
        {
            SON_MEMSET(p_resp, 0, sizeof(son_anr_disable_res_t));
            ((son_anr_disable_res_t *)p_resp)->transaction_id =
                *((son_u16 *)p_msg);

            /* Filling the response structure */
            ((son_anr_disable_res_t *)p_resp)->disable_res.result = SON_FAILURE;
            ((son_anr_disable_res_t *)p_resp)->disable_res.error_code = error_code;
        }
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_enable_disable_req_nack */

/*****************************************************************************
* Function Name  : anr_create_meas_config_req_nack
* Inputs         : p_msg       - Pointer to the message received
*                  api         - API Id
*                  length      - length of the meas config response message  
*                  error_code  - Error code to be sent
* Outputs        : None
* Returns        : son_u8 *    -Pointer to the meas config NACK response.
 *                              Address of the first byte of the NACK reponse.
* Description    : This is the function to create the measurement config 
*                  NACK response
******************************************************************************/
static son_u8 *
anr_create_meas_config_req_nack
(
    son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
    son_error_et            error_code
)
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(sizeof(son_anr_meas_config_res_t));
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_meas_config_res_t));
        ((son_anr_meas_config_res_t *)p_resp)->transaction_id =
            *((son_u16 *)p_msg);
        ((son_anr_meas_config_res_t *)p_resp)->config_res.result = 
            SON_FAILURE;
        ((son_anr_meas_config_res_t *)p_resp)->config_res.
            error_code = error_code;
        SON_MEMCPY(&((son_anr_meas_config_res_t *)p_resp)->
                config_res.src_cgi,
                &((son_anr_meas_config_req_t *)p_msg)->config_req.
                src_cgi, sizeof(son_intra_rat_global_cell_id_t));
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_meas_config_req_nack */

/*****************************************************************************
* Function Name  : anr_create_add_req_nack
* Inputs         : p_msg       - Pointer to the message received
*                  api         -  API Id
*                  length      -  length of the NR add response message  
*                  error_code  -  Error code to be sent
* Outputs        : None.
* Returns        : son_u8 * -Pointer to the SONANR_NR_ADD_RES message.
 *                           The Address of the first byte of the response
 *                           is returned.
* Description    : This is the function to create the SONANR_NR_ADD_RES message
 *                 for NACK
******************************************************************************/
static son_u8 *
anr_create_add_req_nack
(
    son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
    son_u32                 length,
    son_error_et            error_code
)
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_nr_add_res_t));
        SON_MEMCPY(&((son_anr_nr_add_res_t *)p_resp)->add_res.src_cgi,
            &((son_anr_nr_add_req_t *)p_msg)->add_req.src_cgi,
                sizeof(son_intra_rat_global_cell_id_t));                
        ((son_anr_nr_add_res_t *)p_resp)->transaction_id = *((son_u16 *)p_msg);
        ((son_anr_nr_add_res_t *)p_resp)->add_res.result  = SON_FAILURE;
        ((son_anr_nr_add_res_t *)p_resp)->add_res.error_code = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_add_req_nack */

/*****************************************************************************
* Function Name  : anr_create_delete_req_nack
* Inputs         : p_msg        - Pointer to the message received
*                  api          - API Id
*                  length       - length of the NR delete response message  
*                  error_code   - Error code to be sent
* Outputs        : None.
* Returns        : son_u8 * -Pointer to the SONANR_NR_DELETE_RES.
 *                           Address of the first byte of the response is
 *                           returned.
* Description    : This is the function to create the SONANR_NR_DELETE_RES for
 *                 NACK
******************************************************************************/
static son_u8 *
anr_create_delete_req_nack
(
    son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
    son_u32                 length,
    son_error_et            error_code
)
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_nr_delete_res_t));
        SON_MEMCPY(&((son_anr_nr_delete_res_t *)p_resp)->del_res.src_cgi,
            &((son_anr_nr_delete_req_t *)p_msg)->del_req.src_cgi,
                sizeof(son_intra_rat_global_cell_id_t));

        ((son_anr_nr_delete_res_t *)p_resp)->transaction_id = *((son_u16 *)p_msg);
        ((son_anr_nr_delete_res_t *)p_resp)->del_res.result  = SON_FAILURE;
        ((son_anr_nr_delete_res_t *)p_resp)->del_res.error_code = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_delete_req_nack */

/*****************************************************************************
 * Function Name  : anr_create_delete_from_remove_list_req_nack
 * Inputs         : p_msg      -  Pointer to the message received
 *                  api        -  API Id
 *                  length     -  length of the NR delete response message
 *                  error_code -  Error code to be sent
 * Outputs        : None.
 * Returns        : son_u8 *   -  Pointer to the 
 *                                SONANR_NR_DELETE_FROM_REMOVE_LIST_RES message.
 *                                Address of the first byte of the response is
 *                                returned.
 * Description    : This is the function to create the 
 *                  SONANR_NR_DELETE_FROM_REMOVE_LIST_RES response to NACK
 * ******************************************************************************/
static son_u8 *
anr_create_delete_from_remove_list_req_nack
(
 son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
 son_u32                 length,
 son_error_et            error_code
 )
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof
                (son_anr_nr_delete_from_remove_list_res_t));
        SON_MEMCPY(&((son_anr_nr_delete_from_remove_list_res_t *)p_resp)->
                delete_from_remove_list_res.src_cgi,
            &((son_anr_nr_delete_from_remove_list_req_t *)p_msg)->
                delete_from_remove_list_req.src_cgi,
                sizeof(son_intra_rat_global_cell_id_t));

        ((son_anr_nr_delete_from_remove_list_res_t *)p_resp)->
        transaction_id = *((son_u16 *)p_msg);
        ((son_anr_nr_delete_from_remove_list_res_t *)p_resp)->
            delete_from_remove_list_res.result  = SON_FAILURE;
        ((son_anr_nr_delete_from_remove_list_res_t *)p_resp)->
            delete_from_remove_list_res.error_code = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_delete_from_remove_list_req_nack */

/*****************************************************************************
* Function Name  : anr_create_update_req_nack
* Inputs         : p_msg       - Pointer to the message received
*                  api         - API Id
*                  length      - length of the NR update response message  
*                  error_code  - Error code to be sent
* Outputs        : None
* Returns        : son_u8 * - Pointer to the SONANR_NR_UPDATE_RES message.
 *                            Address of the first byte of the response is
 *                            returned.
* Description    : This is the function to create the SONANR_NR_UPDATE_RES to 
 *                 NACK
******************************************************************************/
static son_u8 *
anr_create_update_req_nack
(
    son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
    son_u32                 length,
    son_error_et            error_code
)
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_nr_update_res_t));
        SON_MEMCPY(&((son_anr_nr_update_res_t *)p_resp)->upd_res.src_cgi,
            &((son_anr_nr_update_req_t *)p_msg)->upd_req.src_cgi,
                sizeof(son_intra_rat_global_cell_id_t));
        ((son_anr_nr_update_res_t *)p_resp)->transaction_id = *((son_u16 *)p_msg);
        ((son_anr_nr_update_res_t *)p_resp)->upd_res.result  = SON_FAILURE;
        ((son_anr_nr_update_res_t *)p_resp)->upd_res.error_code = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_update_req_nack */

/*****************************************************************************
* Function Name  : anr_create_retreive_req_nack
* Inputs         : p_msg       - Pointer to the message received
*                  api         - API Id
*                  length      - length of the NR retreive response message  
*                  error_code  - Error code to be sent
* Outputs        : None.
* Returns        : son_u8*     - Pointer to the SONANR_NR_RETRIEVE_RES message.
 *                              Address of the first byte of the response is
 *                              returned.
* Description    : This is the function to create the SONANR_NR_RETRIEVE_RES to 
 *                 NACK
******************************************************************************/
static son_u8 *
anr_create_retreive_req_nack
(
    son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
    son_u32                 length,
    son_error_et            error_code
)
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_nr_retrieve_res_t));
        SON_MEMCPY(&((son_anr_nr_retrieve_res_t *)p_resp)->rtrv_res.src_cgi,
            &((son_anr_nr_retrieve_req_t *)p_msg)->rtrv_req.src_cgi,
            sizeof(son_intra_rat_global_cell_id_t));
        ((son_anr_nr_retrieve_res_t *)p_resp)->transaction_id =
            *((son_u16 *)p_msg);
        ((son_anr_nr_retrieve_res_t *)p_resp)->rtrv_res.result  =
            SON_FAILURE;
        ((son_anr_nr_retrieve_res_t *)p_resp)->rtrv_res.error_code =
            error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_retreive_req_nack */

/*****************************************************************************
* Function Name  : anr_create_nr_scan_req_nack
* Inputs         : p_msg       -  Pointer to the message received
*                  api         -  API Id
*                  length      -  length of the scan response message  
*                  error_code  -  Error code to be sent
* Outputs        : None.
* Returns        : son_u8 *    - Pointer to the SONANR_NR_SCAN_RES message.
 *                               Address of the first byte of the response
 *                               is returned.
* Description    : This is the function to create SONANR_NR_SCAN_RES message
 *                 to NACK.
******************************************************************************/
static son_u8 *
anr_create_nr_scan_req_nack
(
    son_u8                  *p_msg,
    /*SPR 17777+-*/
    son_u32                 length,
    son_error_et            error_code
)
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_scan_res_t));
        son_anr_scan_req_t *p_req = (son_anr_scan_req_t *)p_msg;
        if (p_req->eutran_scan_req.p_start_scan_req != SON_PNULL)
        {
            ((son_anr_scan_res_t *)p_resp)->transaction_id =
                p_req->eutran_scan_req.transaction_id;
        }
        else 
        {
            ((son_anr_scan_res_t *)p_resp)->transaction_id =
                p_req->utran_scan_req.transaction_id;
        }

        ((son_anr_scan_res_t *)p_resp)->scan_res.result  = SON_FAILURE;
        ((son_anr_scan_res_t *)p_resp)->scan_res.error_code  =
            error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_nr_scan_req_nack */

/*****************************************************************************
 * Function Name  : anr_create_modify_removal_attributes_req_nack
 * Inputs         : p_msg       - Pointer to the message received
 *                  api         - API Id
 *                  length      - length of the Modify Removal Attributes response 
 *                               message
 *                  error_code  - Error code to be sent
 * Outputs        : None.
 * Returns        : son_u8 *    - Pointer to the SONANR_MODIFY_REMOVAL_ATTRIBUTES_RES
 *                                message.Address of the first byte of the response
 *                                is returned.
 * Description    : This is the function to create the 
 *                  SONANR_MODIFY_REMOVAL_ATTRIBUTES_RES message to NACK.
 * ******************************************************************************/
static son_u8 *
anr_create_modify_removal_attributes_req_nack
(
 son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
 son_u32                 length,
 son_error_et            error_code
 )
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_modify_removal_attributes_res_t));

        SON_MEMCPY(&((son_anr_modify_removal_attributes_res_t *)p_resp)->
                modify_removal_attributes_res.src_cgi,
            &((son_anr_modify_removal_attributes_req_t *)p_msg)->
                modify_removal_attributes_req.src_cgi,
                sizeof(son_intra_rat_global_cell_id_t));

        ((son_anr_modify_removal_attributes_res_t *)p_resp)->
        transaction_id = *((son_u16 *)p_msg);

        ((son_anr_modify_removal_attributes_res_t *)p_resp)->
            modify_removal_attributes_res.result  = SON_FAILURE;

        ((son_anr_modify_removal_attributes_res_t *)p_resp)->
            modify_removal_attributes_res.error_code = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_modify_removal_attributes_req_nack */

/*****************************************************************************
 * Function Name  : anr_create_modify_pci_confusion_cfg_req_nack
 * Inputs         : p_msg       - Pointer to the message received
 *                  api         - API Id
 *                  length      - length of the Modify PCI confusion cfg 
 *                               response message
 *                  error_code  - Error code to be sent
 * Outputs        : None.
 * Returns        : son_u8 *    - Pointer to the SONANR_MODIFY_PCI_CONFUSION_CFG_RES
 *                                message.Address of the first byte is returned.
 * Description    : This is the function to create SONANR_MODIFY_PCI_CONFUSION_CFG_RES
 *                  to NACK
 * ******************************************************************************/
static son_u8 *
anr_create_modify_pci_confusion_cfg_req_nack
(
 son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
 son_u32                 length,
 son_error_et            error_code
 )
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_modify_pci_confusion_cfg_res_t));
        SON_MEMCPY(&((son_anr_modify_pci_confusion_cfg_res_t *)p_resp)->
                modify_pci_confusion_cfg_res.src_cgi,
            &((son_anr_modify_pci_confusion_cfg_req_t *)p_msg)->
                modify_pci_confusion_cfg_req.src_cgi,
                sizeof(son_intra_rat_global_cell_id_t));

        ((son_anr_modify_pci_confusion_cfg_res_t *)p_resp)->
        transaction_id = *((son_u16 *)p_msg);

        ((son_anr_modify_pci_confusion_cfg_res_t *)p_resp)->
            modify_pci_confusion_cfg_res.result  = SON_FAILURE;

        ((son_anr_modify_pci_confusion_cfg_res_t *)p_resp)->
            modify_pci_confusion_cfg_res.error_code = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_modify_pci_confusion_cfg_req_nack */

/* SPR 20653 Fix Start */
/*****************************************************************************
 * Function Name  : anr_create_modify_attribute_req_nack
 * Inputs         : p_msg       - Pointer to the message received
 *                  api         - API Id
 *                  length      - length of the Modify Attribute 
 *                               response message
 *                  error_code  - Error code to be sent
 * Outputs        : None.
 * Returns        : son_u8 *    - Pointer to the SONANR_MODIFY_ATTRIBUTE_RES
 *                                message.Address of the first byte is returned.
 * Description    : This is the function to create SONANR_MODIFY_ATTRIBUTE_RES
 *                  to NACK
 * ******************************************************************************/
static son_u8 *
anr_create_modify_attribute_req_nack
(
 son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
 son_u32                 length,
 son_error_et            error_code
 )
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_modify_attribute_res_t));
        SON_MEMCPY(&((son_anr_modify_attribute_res_t *)p_resp)->
                modify_attribute_res.src_cgi,
            &((son_anr_modify_attribute_req_t *)p_msg)->
                modify_attribute_req.src_cgi,
                sizeof(son_intra_rat_global_cell_id_t));

        ((son_anr_modify_attribute_res_t *)p_resp)->
        transaction_id = *((son_u16 *)p_msg);

        ((son_anr_modify_attribute_res_t *)p_resp)->
            modify_attribute_res.result  = SON_FAILURE;

        ((son_anr_modify_attribute_res_t *)p_resp)->
            modify_attribute_res.error_code = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_modify_attribute_req_nack */
/* SPR 20653 Fix End */

/*****************************************************************************
 * Function Name  : anr_create_enable_disable_limited_mode_req_nack
 * Inputs         : p_msg       - Pointer to the message received
 *                  api         - API Id
 *                  length      - length of the Enable/Disable Limited mode 
 *                               response message
 *                  error_code  - Error code to be sent
 * Outputs        : None.
 * Returns        : son_u8 *    - Pointer to the 
 *                                SONANR_ENABLE_DISABLE_LIMITED_MODE_RES message.
 *                                Address of the first byte of the response is
 *                                returned.
 * Description    : This is the function to create 
 *                  SONANR_ENABLE_DISABLE_LIMITED_MODE_RES response to NACK.
 * ******************************************************************************/
static son_u8 *
anr_create_enable_disable_limited_mode_req_nack
(
 son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
 son_u32                 length,
 son_error_et            error_code
 )
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(
                    son_anr_enable_disable_limited_mode_res_t));
        SON_MEMCPY(&((son_anr_enable_disable_limited_mode_res_t *)p_resp)->
                enable_disable_limited_mode_res.src_cgi,
            &((son_anr_enable_disable_limited_mode_req_t *)p_msg)->
                enable_disable_limited_mode_req.src_cgi,
                sizeof(son_intra_rat_global_cell_id_t));

        ((son_anr_enable_disable_limited_mode_res_t *)p_resp)->
        transaction_id = *((son_u16 *)p_msg);

        ((son_anr_enable_disable_limited_mode_res_t *)p_resp)->
            enable_disable_limited_mode_res.result  = SON_FAILURE;

        ((son_anr_enable_disable_limited_mode_res_t *)p_resp)->
            enable_disable_limited_mode_res.error_code = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_enable_disable_limited_mode_req_nack */

/*****************************************************************************
 * Function Name  : anr_create_enable_disable_X2_req_nack
 * Inputs         : p_msg      -  Pointer to the message received
 *                  api        -  API Id
 *                  length     -  length of the Enable/Disable Limited mode 
 *                               response message
 *                  error_code -  Error code to be sent
 * Outputs        : None.
 * Returns        : son_u8 *   -  Pointer to the SONANR_ENABLE_DISABLE_X2_RES
 *                                 message.Address of the first byte of the
 *                                 response is returned.
 * Description    : This is the function to create SONANR_ENABLE_DISABLE_X2_RES 
 *                  response to NACK.
 * ******************************************************************************/
static son_u8 *
anr_create_enable_disable_x2_req_nack
(
 son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
 son_u32                 length,
 son_error_et            error_code
 )
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(
                    son_anr_enable_disable_x2_res_t));

        ((son_anr_enable_disable_x2_res_t *)p_resp)->
        transaction_id = ((son_anr_enable_disable_x2_req_t *)p_msg)->transaction_id;

        ((son_anr_enable_disable_x2_res_t *)p_resp)->
            enable_disable_x2_res.result  = SON_FAILURE;

        ((son_anr_enable_disable_x2_res_t *)p_resp)->
            enable_disable_x2_res.error_code = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_enable_disable_x2_req_nack */

/*****************************************************************************
* Function Name  : anr_create_enb_add_req_nack
* Inputs         : p_msg       -  Pointer to the message received
*                  api         -  API Id
*                  length      -  length of the NR enb add response message  
*                  error_code  -  Error code to be sent
* Outputs        : None.
* Returns        : son_u8 *    - Pointer to the SONANR_NR_ENB_ADD_RESP message.
 *                               Address of the first byte of the response is
 *                               returned.
* Description    : This is the function to create SONANR_NR_ENB_ADD_RESP 
 *                 message to NACK.
******************************************************************************/
static son_u8 *
anr_create_enb_add_req_nack
(
    son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
    son_u32                 length,
    son_error_et            error_code
)
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_enb_add_resp_t));

        ((son_anr_enb_add_resp_t *)p_resp)->transaction_id = ((son_anr_enb_add_req_t *)p_msg)->transaction_id;
        ((son_anr_enb_add_resp_t *)p_resp)->enb_add_res.result  = SON_FAILURE;
        ((son_anr_enb_add_resp_t *)p_resp)->enb_add_res.error_code = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_enb_add_req_nack */

/*****************************************************************************
* Function Name  : anr_create_enb_update_req_nack
* Inputs         : p_msg      - Pointer to the message received
*                  api        - API Id
*                  length     - length of the NR enb update response message  
*                  error_code - Error code to be sent
* Outputs        : None.
* Returns        : son_u8 *   - Pointer to the SONANR_NR_ENB_UPDATE_RESP message.
 *                              Address of the first byte of the response
 *                              message is returned.
* Description    : This is the function to create SONANR_NR_ENB_UPDATE_RESP 
*                  response to NACK.
******************************************************************************/
static son_u8 *
anr_create_enb_update_req_nack
(
    son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
    son_u32                 length,
    son_error_et            error_code
)
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_enb_update_resp_t));
        
        ((son_anr_enb_update_resp_t *)p_resp)->transaction_id = ((son_anr_enb_update_req_t *)p_msg)->transaction_id;
        ((son_anr_enb_update_resp_t *)p_resp)->enb_update_res.result  = SON_FAILURE;
        ((son_anr_enb_update_resp_t *)p_resp)->enb_update_res.error_code  = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_enb_update_req_nack */

/*****************************************************************************
* Function Name  : anr_create_enb_delete_req_nack
* Inputs         : p_msg      -  Pointer to the message received
*                  api        -  API Id
*                  length     -  length of the NR enb delete response message  
*                  error_code -  Error code to be sent
* Outputs        : None.
* Returns        : son_u8 *   - Pointer to the SONANR_NR_ENB_DELETE_RESP message.
 *                              Address of the first byte of the response is
 *                              returned.
* Description    : This is the function to create SONANR_NR_ENB_DELETE_RESP 
*                  message to NACK.
******************************************************************************/
static son_u8 *
anr_create_enb_delete_req_nack
(
    son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
    son_u32                 length,
    son_error_et            error_code
)
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_enb_delete_resp_t));
        
        ((son_anr_enb_delete_resp_t *)p_resp)->transaction_id = ((son_anr_enb_delete_req_t *)p_msg)->transaction_id;
        ((son_anr_enb_delete_resp_t *)p_resp)->enb_del_res.result  = SON_FAILURE;

        ((son_anr_enb_delete_resp_t *)p_resp)->enb_del_res.error_code = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_enb_delete_req_nack */

/*****************************************************************************
* Function Name  : anr_create_enb_retrieve_req_nack
* Inputs         : p_msg       - Pointer to the message received
*                  api         - API Id
*                  length      - length of the NR enb_retrieve response message  
*                  error_code  - Error code to be sent
* Outputs        : None.
* Returns        : son_u8 *    - Pointer to the SONANR_NR_ENB_RETRIEVE_RESP 
 *                               message.Address of the first byte of the response
 *                               is returned.
* Description    : This is the function to create the SONANR_NR_ENB_RETRIEVE_RESP 
*                  message to NACK.
******************************************************************************/
static son_u8 *
anr_create_enb_retrieve_req_nack
(
    son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
    son_u32                 length,
    son_error_et            error_code
)
{
    son_u8 *p_resp = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_resp = (son_u8 *)son_mem_get(length);
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_enb_retrieve_resp_t));
        
        ((son_anr_enb_retrieve_resp_t *)p_resp)->transaction_id = ((son_anr_enb_retrieve_req_t *)p_msg)->transaction_id;
        ((son_anr_enb_retrieve_resp_t *)p_resp)->enb_retrieve_res.result  = SON_FAILURE;
        ((son_anr_enb_retrieve_resp_t *)p_resp)->
            enb_retrieve_res.error_code = error_code;
    }

    SON_UT_TRACE_EXIT();
    return p_resp;
} /* anr_create_enb_retrieve_req_nack */


/*****************************************************************************
* Function Name  : anr_create_meas_config_obj_remove_req_nack
* Inputs         : p_msg        Pointer to the message received
*                  api          API Id
*                  length       length of the meas object removal response msg
*                  error_code   Error code to be sent
* Outputs        : p_resp       Pointer to the response message created.
* Returns        : son_void_t
* Description    : This is the function to create the measurement object removal
*                  NACK response
******************************************************************************/
static son_u8 *
anr_create_meas_config_obj_remove_req_nack
(
    son_u8                  *p_msg,
        /*SPR 17777 +-*/ 
    son_error_et            error_code
)
{
    son_u8 *p_resp = SON_PNULL;
    SON_UT_TRACE_ENTER();

    p_resp = (son_u8*)son_mem_get(sizeof(son_anr_meas_config_obj_remove_res_t));
    if (SON_PNULL != p_resp)
    {
        SON_MEMSET(p_resp, 0, sizeof(son_anr_meas_config_obj_remove_res_t));
        ((son_anr_meas_config_obj_remove_res_t *)p_resp)->transaction_id =
            *((son_u16*)p_msg);
        ((son_anr_meas_config_obj_remove_res_t *)p_resp)->remove_res.result =
            SON_FAILURE;
        ((son_anr_meas_config_obj_remove_res_t *)p_resp)->remove_res.
            error_code = error_code;
        SON_MEMCPY(&((son_anr_meas_config_obj_remove_res_t *)p_resp)->
                remove_res.src_cgi,
                &((son_anr_meas_config_obj_remove_req_t *)p_msg)->remove_req.
                src_cgi, sizeof(son_intra_rat_global_cell_id_t));
    }
    SON_UT_TRACE_EXIT();
    return p_resp;
}

/*****************************************************************************
* Function Name  : anr_create_send_generic_nack_to_mif
* Inputs         : p_msg        - Pointer to the message received
*                : p_cspl_hdr - Pointer to the the CSPL header information
*                : error_code   - Error code to be sent
* Outputs        : None
* Returns        : son_void_t
* Description    : This is the function to send NACK response to SMIF
******************************************************************************/
static son_void_t
anr_create_send_generic_nack_to_mif
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_error_et     error_code
)
{
    son_bool_et                     send_res    = SON_TRUE;
    /* Coverity Fix: 19176 */
    son_procedure_code_et           api         = SON_PROCEDURE_UNDEFINED;
    son_u32                         length      = SON_NULL;
    son_u8                          *p_resp     = SON_PNULL;
    son_intra_rat_global_cell_id_t  *p_cell_id  = SON_PNULL;


    SON_UT_TRACE_ENTER();

    switch (p_cspl_hdr->api)
    {
        case SONANR_ENABLE_REQ:  /* Intentional Break Left */
        case SONANR_DISABLE_REQ: /* Enable and Disable are same structures */
            {
            api = (SONANR_ENABLE_REQ == p_cspl_hdr->api) ? SONANR_ENABLE_RES
                  : SONANR_DISABLE_RES;
                length = sizeof(son_anr_enable_res_t);
                p_resp = anr_create_enable_disable_req_nack(p_msg, api, length, 
                    error_code);
            break;
        }

        case SONANR_NR_ADD_REQ:
        {
            api = SONANR_NR_ADD_RES;
            length = sizeof(son_anr_nr_add_res_t);
            /*SPR 17777 +-*/ 
            p_resp = anr_create_add_req_nack(p_msg, length, error_code);
            break;
        }

        case SONANR_NR_DELETE_REQ:
        {
            api = SONANR_NR_DELETE_RES;
            length = sizeof(son_anr_nr_delete_res_t);
            /*SPR 17777 +-*/ 
            p_resp = anr_create_delete_req_nack(p_msg,length, error_code);
            break;
        }

        case SONANR_NR_UPDATE_REQ:
        {
            api = SONANR_NR_UPDATE_RES;
            length = sizeof(son_anr_nr_update_res_t);
            /*SPR 17777 +-*/ 
            p_resp = anr_create_update_req_nack(p_msg,length, error_code);
            break;
        }

        case SONANR_NR_RETRIEVE_REQ:
        {
            api = SONANR_NR_RETRIEVE_RES;
            length = sizeof(son_anr_nr_retrieve_res_t);
            /*SPR 17777 +-*/ 
            p_resp = anr_create_retreive_req_nack(p_msg,length, 
                error_code);
            break;
        }

        case SONANR_NR_SCAN_REQ:
        {
            api = SONANR_NR_SCAN_RES;
            length = sizeof(son_anr_scan_res_t);
            p_resp = anr_create_nr_scan_req_nack(p_msg,length, 
                error_code);/*SPR 17777 +-*/
            break;
        }

        case SONANR_MEAS_CONFIG_REQ:
            {
                api = SONANR_MEAS_CONFIG_RES;
                length = sizeof(son_anr_meas_config_res_t);
            /*SPR 17777 +-*/ 
                p_resp = anr_create_meas_config_req_nack(p_msg,
                    error_code);
            break;
        }

        case SONANR_MODIFY_UE_COUNT_REQ:  
            {
                api = SONANR_MODIFY_UE_COUNT_RES;
                p_cell_id = &(((son_anr_modify_ue_count_req_t *)p_msg)->
                        modify_ue_cnt_req.src_cgi);
            }

            /* Break left intentionally */
        case SONANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ: 
            {
                /* Send Generic Response message */
                length = sizeof(son_generic_response_with_cell_id_t);
            p_resp = (son_u8 *)son_mem_get(length);

                if (SON_PNULL == p_cell_id)
                {
                    p_cell_id = 
                        &(((son_anr_modify_meas_reporting_interval_req_t *)
                                    p_msg)->modify_interval_req.src_cgi);
                    api = SONANR_MODIFY_MEAS_REPORTING_INTERVAL_RES;
                }

                if (SON_PNULL != p_resp)
                {
                    SON_MEMSET(p_resp, 0, 
                        sizeof(son_generic_response_with_cell_id_t));
                    SON_MEMCPY(&((son_generic_response_with_cell_id_t *)p_resp)
                            ->generic_res.cell_id, p_cell_id,
                    sizeof(son_intra_rat_global_cell_id_t));
                ((son_generic_response_with_cell_id_t *)p_resp)->
                transaction_id = *((son_u16 *)p_msg);
                ((son_generic_response_with_cell_id_t *)p_resp)->
                        generic_res.result = SON_FAILURE;
                ((son_generic_response_with_cell_id_t *)p_resp)->
                        generic_res.error_code = error_code;
                }

            break;
        }

        case SONANR_NR_DELETE_FROM_REMOVE_LIST_REQ:
        {
            api = SONANR_NR_DELETE_FROM_REMOVE_LIST_RES;
            length = sizeof(son_anr_nr_delete_from_remove_list_res_t);
            /*SPR 17777 +-*/ 
            p_resp = anr_create_delete_from_remove_list_req_nack(p_msg, length, error_code);
            break;
        }

        case SONANR_MODIFY_REMOVAL_ATTRIBUTES_REQ:
        {
            api = SONANR_MODIFY_REMOVAL_ATTRIBUTES_RES;
            length = sizeof(son_anr_modify_removal_attributes_res_t);
            p_resp = anr_create_modify_removal_attributes_req_nack(p_msg, length, error_code);
            break;
        }

        case SONANR_MODIFY_PCI_CONFUSION_CFG_REQ:
        {
            api = SONANR_MODIFY_PCI_CONFUSION_CFG_RES;
            length = sizeof(son_anr_modify_pci_confusion_cfg_res_t);
            p_resp = anr_create_modify_pci_confusion_cfg_req_nack(p_msg,length, error_code);
            break;
        }

        case SONANR_ENABLE_DISABLE_LIMITED_MODE_REQ:
        {
            api = SONANR_ENABLE_DISABLE_LIMITED_MODE_RES;
            length = sizeof(son_anr_enable_disable_limited_mode_res_t);
            p_resp = anr_create_enable_disable_limited_mode_req_nack(p_msg,length, error_code);
            break;
        }

        case SONANR_ENABLE_DISABLE_X2_REQ:
        {
            api = SONANR_ENABLE_DISABLE_X2_RES;
            length = sizeof(son_anr_enable_disable_x2_res_t);
            p_resp = anr_create_enable_disable_x2_req_nack(p_msg,length, error_code);
            break;
        }

        case SONANR_NR_ENB_ADD_REQ:
        {
            api = SONANR_NR_ENB_ADD_RESP;
            length = sizeof(son_anr_enb_add_resp_t);
            p_resp = anr_create_enb_add_req_nack(p_msg,length, error_code);
            break;
        }

        case SONANR_NR_ENB_UPDATE_REQ:
        {
            api = SONANR_NR_ENB_UPDATE_RESP;
            length = sizeof(son_anr_enb_update_resp_t);
            p_resp = anr_create_enb_update_req_nack(p_msg, length, error_code);
            break;
        }

        case SONANR_NR_ENB_DELETE_REQ:
        {
            api = SONANR_NR_ENB_DELETE_RESP;
            length = sizeof(son_anr_enb_delete_resp_t);
            p_resp = anr_create_enb_delete_req_nack(p_msg, length, error_code);
            break;
        }

        case SONANR_NR_ENB_RETRIEVE_REQ:
        {
            api = SONANR_NR_ENB_RETRIEVE_RESP;
            length = sizeof(son_anr_enb_retrieve_resp_t);
            p_resp = anr_create_enb_retrieve_req_nack(p_msg,length, error_code);
            break;
        }

        case SONANR_MEAS_CONFIG_OBJ_REMOVE_REQ:
        {
            api = SONANR_MEAS_CONFIG_OBJ_REMOVE_RES;
            length = sizeof(son_anr_meas_config_obj_remove_res_t);
            p_resp = anr_create_meas_config_obj_remove_req_nack(p_msg,
                    error_code);
            /*SPR 17777 +-*/ 
            break;
        }
        /* SPR 20653 Fix Start */
        case SONANR_MODIFY_ATTRIBUTE_REQ: 
            {
                api = SONANR_MODIFY_ATTRIBUTE_RES;
                length = sizeof(son_anr_modify_attribute_res_t);
                p_resp = anr_create_modify_attribute_req_nack(p_msg,length, error_code);
                break;
        }

        /* SPR 20653 Fix End */

        default:
        {
            SON_LOG(anr_get_log_mode(),
                    p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_create_send_generic_nack_to_mif: "
                    "Unexpected Message [%d] Received from SMIF\n",
                    p_cspl_hdr->api);
            send_res = SON_FALSE;
            break;
    }
    } /* switch */

    if (send_res && p_resp)
    {
        /* Coverity Fix: 19174,19168 */
        son_create_send_buffer(p_resp, SON_ANR_MODULE_ID,
                SON_MIF_MODULE_ID, api,
                length );

        son_mem_free(p_resp);
    }

    SON_UT_TRACE_EXIT();
} /* anr_create_send_generic_nack_to_mif */
        
/*****************************************************************************
* Function Name  : anr_create_send_generic_nack_to_apps
* Inputs         : p_msg       - Pointer to the message received
*                : p_cspl_hdr - Pointer to the the CSPL header information
*                : error_code   Error code to be sent
* Outputs        : None
* Returns        : son_void_t
* Description    : This is the function to send NACK response to SONApps
******************************************************************************/
static son_void_t
anr_create_send_generic_nack_to_apps
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_error_et     error_code
)
{
    son_bool_et                     send_res    = SON_TRUE;
    /* Coverity Fix: 19176 */
    son_procedure_code_et           api         = SON_PROCEDURE_UNDEFINED;
    son_u32                         length      = SON_NULL;
    son_u8                          *p_resp     = SON_PNULL;


    SON_UT_TRACE_ENTER();

    switch (p_cspl_hdr->api)
    {
        case SONANR_PCI_REG_DEREG_REQ:
        {
            api = SONANR_PCI_REG_DEREG_RES; 
            length = sizeof(son_anr_pci_reg_dereg_res_t);
            p_resp = (son_u8 *)son_mem_get(length);
            if (SON_PNULL != p_resp)
            {
                SON_MEMSET(p_resp, 0, sizeof(son_anr_pci_reg_dereg_res_t));
                ((son_anr_pci_reg_dereg_res_t *) p_resp)->transaction_id =
                    ((son_anr_pci_reg_dereg_req_t *)p_msg)->transaction_id;
                ((son_anr_pci_reg_dereg_res_t *) p_resp)->result = SON_FAILURE;
                ((son_anr_pci_reg_dereg_res_t *) p_resp)->error_code = 
                    error_code;                   
            }

            break;
        }

        case SONANR_PCI_IN_USE_LIST_REQ:
        {
            api = SONANR_PCI_IN_USE_LIST_RES;
            length = sizeof(son_anr_pci_in_use_list_res_t);
            p_resp = (son_u8 *)son_mem_get(length);
            if (SON_PNULL != p_resp)
            {
                SON_MEMSET(p_resp, 0, sizeof(son_anr_pci_in_use_list_res_t));
                ((son_anr_pci_in_use_list_res_t *)p_resp)->transaction_id =
                    ((son_anr_pci_in_use_list_req_t *)p_msg)->transaction_id;
                ((son_anr_pci_in_use_list_res_t *)p_resp)->result = SON_FAILURE;
                ((son_anr_pci_in_use_list_res_t *) p_resp)->error_code = 
                    error_code;
            }

            break;
        }

        default:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
                    "anr_create_send_generic_nack_to_apps: "
                    "Unexpected Message [%d] Received from SONApps\n",
                    p_cspl_hdr->api);
            send_res = SON_FALSE;
            break;
    }
    } /* switch */

    if (send_res && p_resp)
    {
        /* Coverity Fix: 19168 */
        son_create_send_buffer(p_resp, SON_ANR_MODULE_ID,
                SON_APPS_MODULE_ID, api,
                length );

        son_mem_free(p_resp);
    }

    SON_UT_TRACE_EXIT();
} /* anr_create_send_generic_nack_to_apps */
     
/*****************************************************************************
* Function Name  : anr_create_send_generic_nack
* Inputs         : p_msg      - Pointer to the message received
*                : p_cspl_hdr - Pointer to the the CSPL header information
*                  error_code - Error code to be sent
* Outputs        : None
* Returns        : son_void_t
* Description    : This is the function to send NACK response to the Requesting
 *                 Module.
******************************************************************************/
static son_void_t
anr_create_send_generic_nack
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_error_et     error_code
)
{
    SON_UT_TRACE_ENTER();

    if (SON_MIF_MODULE_ID == p_cspl_hdr->from)
    {
        anr_create_send_generic_nack_to_mif(p_msg, p_cspl_hdr, error_code);
    }
    else if (SON_APPS_MODULE_ID == p_cspl_hdr->from)
    {
        anr_create_send_generic_nack_to_apps(p_msg, p_cspl_hdr, error_code);
    }  
    else
    {
        SON_LOG(anr_get_log_mode(),
                p_son_anr_facility_name,
                SON_ERROR,
                "anr_create_send_generic_nack: "
                "Unexpected Message [%d] Received from unknown module [%d]\n",
                p_cspl_hdr->api, p_cspl_hdr->from);
    }

    SON_UT_TRACE_EXIT();
} /* anr_create_send_generic_nack */

/* LTE_SON_KLOCWORK_WARN_6JULY_2012_END */

/*****************************************************************************
 * Function Name  : anr_scan_req_handler
 * Inputs         : p_cspl_hdr - Pointer to the the CSPL header information
 *                : p_msg      - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - 
 *                  RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ANR module and is
 *                : invoked when  SONANR_SCAN_REQ message is received.
****************************************************************************/
static son_buf_retain_status_et
anr_scan_req_handler
(
STACKAPIHDR *p_cspl_hdr,
son_u8 *p_msg
)
{
    son_error_et            error_code      = SON_ERR_NO_CELL_CONFIGURED;
    son_bool_et             start_scan      = SON_FALSE;
    anr_cell_context_node_t *p_ctxt_node    = SON_PNULL;
    son_anr_scan_req_t      *p_req          = (son_anr_scan_req_t *)p_msg;


    SON_UT_TRACE_ENTER();
    do
    {
        if (anr_get_context_cell_count() == 0)
        {
            error_code = SON_ERR_NO_CELL_CONFIGURED;
            SON_LOG(anr_get_log_mode(),
                    p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_scan_req_handler: "
                    "NR Scan could not be initiated, "
                    "as no cells are configured\n");

            break;
        }

        p_ctxt_node = anr_get_first_cell_ctxt();
        while (SON_PNULL != p_ctxt_node)
        {
            if (ANR_CELL_STATE_ENABLED ==
                    p_ctxt_node->data.current_cell_fsm_state)
            {
                start_scan = SON_TRUE;
                break;
            }

            p_ctxt_node = anr_get_next_node(p_ctxt_node);
        }

        if (SON_TRUE != start_scan)
        {
            error_code = SON_ERR_ALREADY_DISABLED;
            SON_LOG(anr_get_log_mode(),
                    p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_scan_req_handler: "
                    "NR Scan could not be initiated, "
                    "as no cells are enabled\n");

            break;
        }

        if (SON_PNULL != p_req->eutran_scan_req.p_start_scan_req)
        {
            son_create_send_buffer((son_u8 *)&(p_req->eutran_scan_req), 
                    SON_ANR_MODULE_ID,
                SON_NMM_MODULE_ID, SONNMM_START_SCAN_REQ,
                    sizeof(son_nmm_start_scan_req_t));
            anr_increment_pending_scan_res_count();
        }

        if (SON_PNULL != p_req->utran_scan_req.p_start_scan_req)
        {
            son_create_send_buffer((son_u8 *)&(p_req->utran_scan_req),
                    SON_ANR_MODULE_ID,
                SON_NMM_MODULE_ID, SONNMM_START_SCAN_REQ,
                    sizeof(son_nmm_start_scan_req_t));
            anr_increment_pending_scan_res_count();
        }

        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    } while (0);

    anr_create_send_generic_nack(p_msg,
            p_cspl_hdr,
            error_code);

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_scan_req_handler */

/* Rel1.2 Changes START */

/*****************************************************************************
 * Function Name  : anr_enable_disable_x2_req_handler
 * Inputs         : p_cspl_hdr - Pointer to the the CSPL header information
 *                : p_msg      -  Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when  SONANR_ENABLE_DISABLE_X2_REQ message is 
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_enable_disable_x2_req_handler
(
            /*SPR 17777 +-*/ 
 son_u8 *p_msg
 )
{
    son_anr_enable_disable_x2_req_t *p_req = 
        (son_anr_enable_disable_x2_req_t *)p_msg;
    son_anr_enable_disable_x2_res_t  res;


    SON_UT_TRACE_ENTER();

    /* Filling the response structure */
    res.transaction_id = p_req->transaction_id;
    res.enable_disable_x2_res.error_code = SON_NO_ERROR;
    res.enable_disable_x2_res.result = SON_SUCCESS;

    anr_set_x2_enabled(p_req->enable_disable_x2_req.x2_enabled);

    /* Coverity Fix: 19169,19165 */
    son_create_send_buffer((son_u8 *)&res, SON_ANR_MODULE_ID,
            SON_MIF_MODULE_ID, SONANR_ENABLE_DISABLE_X2_RES,
            sizeof(res));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_enable_disable_x2_req_handler */

/*****************************************************************************
 * Function Name  : anr_nr_enb_add_req_handler 
 * Inputs         : p_cspl_hdr - Pointer to the the CSPL header information
 *                : p_msg      - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when  SONANR_NR_ENB_ADD_REQ message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_nr_enb_add_req_handler
(
 STACKAPIHDR *p_cspl_hdr,
 son_u8 *p_msg
 )
{
    son_anr_enb_add_req_t                       *p_req                          = (son_anr_enb_add_req_t *)p_msg;
    anr_enb_pending_res_data_t 		*p_pending_resp 	= SON_PNULL;
    anr_enb_pending_res_data_t 		*p_existing_pending_resp 	= SON_PNULL;
    anr_pending_nbr_enb_info_data_t	*p_pending_resp_enb_data = SON_PNULL;
    son_nbr_enb_hash_rec_t			*p_enb_rec			= SON_PNULL;
    son_bool_et             		process_request 	= SON_FALSE;
    son_bool_et             		send_tnl_req 		= SON_FALSE;
    son_error_et            		error_code      	= SON_NO_ERROR;
    son_u8							iterator 			= 0;


    SON_UT_TRACE_ENTER();

    do
    {
        /* If X2 is disabled, reject the request */
        if (SON_FALSE == son_get_x2_enabled())
        {
            SON_LOG(anr_get_log_mode(),
                    p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_nr_enb_add_req_handler: "
                    "Neighbor eNBs can not be added, "
                    "as X2 is not enabled\n");
            error_code = SON_ERR_X2_DISABLED;
            break;
        }

        if (anr_get_context_cell_count() == 0)
        {
            SON_LOG(anr_get_log_mode(),
                    p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_nr_enb_add_req_handler: "
                    "Neighbor eNBs can not be added, "
                    "no cells are configured\n");
            error_code = SON_ERR_NO_CELL_CONFIGURED;
            break;
        }

        if (SON_FAILURE == anr_check_fsm_state(ANR_CELL_STATE_ENABLED))
        {
            SON_LOG(anr_get_log_mode(),
                    p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_nr_enb_add_req_handler: "
                    "Neighbor eNBs can not be added, "
                    " ANR is not enabled for even a single cell\n");
            error_code = SON_ERR_ALREADY_DISABLED;
            break;
        }

        p_pending_resp = anr_add_x2_pending_resp_list(p_req->transaction_id,
                SONANR_NR_ENB_ADD_RESP);

        if (SON_PNULL == p_pending_resp)
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_nr_enb_add_req_handler: Memory allocation failure!\n");
            error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;
            break;
        }

        process_request = SON_TRUE;

        for (iterator = 0; iterator < p_req->enb_add_req.nbr_enb_list_size; iterator++)
        {
            p_enb_rec = SON_PNULL;
            p_pending_resp_enb_data = SON_PNULL;
            /* Rel 3.0 Multi Sector code changes Start */
            son_global_enb_id_t own_enb_id = anr_get_own_enb_id();
            /* Rel 3.0 Multi Sector code changes Stop */
            send_tnl_req = SON_FALSE;

            /* Rel 3.0 Multi Sector code changes Start */
            if (!SON_MEMCMP(&own_enb_id, 
                        &p_req->enb_add_req.nbr_enb_list[iterator].enb_id,
                        sizeof(son_global_enb_id_t)))
            {
                error_code = SON_ERR_LOCAL_NBR_CELL_NOT_ALLOWED_IN_X2_SETUP; 
            }
            else
            {
                error_code = SON_NO_ERROR;

                p_enb_rec = x2_insert_nbr_enb(x2_get_enb_nrt(),
                        &p_req->enb_add_req.nbr_enb_list[iterator],
                        &error_code);
                if (SON_ERR_TNL_DISCOVERY_FAILURE == error_code)
                {
                    send_tnl_req = SON_TRUE;
                    if (SON_PNULL != (p_pending_resp_enb_data =
                                anr_find_enb_x2_pending_resp_list(
                                    &p_req->enb_add_req.
                                    nbr_enb_list[iterator].enb_id,
                                    &p_existing_pending_resp)))
                    {
                        send_tnl_req = SON_FALSE;
                        error_code = p_pending_resp_enb_data->error_code;
                        if (SON_ERR_TNL_DISCOVERY_FAILURE == error_code)
                        {
                            error_code = SON_ERR_TNL_DISCOVERY_IN_PROGRESS;
                        }
                    }
                }
            }
            /* Rel 3.0 Multi Sector code changes Stop */

            p_pending_resp_enb_data = anr_add_enb_x2_pending_resp_elem(p_pending_resp,
                    &p_req->enb_add_req.nbr_enb_list[iterator],
                    p_enb_rec,
                    error_code);

            /* Send TNL Discovery Request to RRM */
            if (SON_TRUE == send_tnl_req)
            {
                anr_construct_send_message_to_rrm(p_pending_resp_enb_data, p_pending_resp);
            }
        }

        /* Send message to X2 */
        anr_construct_send_message_to_x2(p_pending_resp);

        if (!p_pending_resp->expected_res_count)
        {
            anr_construct_send_message_to_oam(p_pending_resp);
        }
    } while (0);

    if (SON_FALSE == process_request)
    {
        anr_create_send_generic_nack(p_msg,
                p_cspl_hdr,
                error_code);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_nr_enb_add_req_handler */

/*****************************************************************************
 * Function Name  : anr_nr_enb_update_req_handler
 * Inputs         : p_cspl_hdr   -Pointer to the CSPL Header
 *                : p_msg        -Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et - 
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when  SONANR_NR_ENB_UPDATE_REQ message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_nr_enb_update_req_handler
(
 STACKAPIHDR *p_cspl_hdr,
 son_u8 *p_msg
 )
{
    son_anr_enb_update_req_t        *p_req = (son_anr_enb_update_req_t *)p_msg;
    anr_enb_pending_res_data_t 		*p_pending_resp = SON_PNULL;
    son_nbr_enb_hash_rec_t			*p_enb_rec		= SON_PNULL;
    son_bool_et             		process_request = SON_FALSE;
    son_error_et            		error_code      = SON_NO_ERROR;
    son_u8							iterator 		= 0;


    SON_UT_TRACE_ENTER();

    do
    {
        p_pending_resp = anr_add_x2_pending_resp_list(p_req->transaction_id,
        											  SONANR_NR_ENB_UPDATE_RESP);

        if (SON_PNULL == p_pending_resp)
        {
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_nr_enb_update_req_handler: Memory allocation failure!\n");
        	error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;
        	break;
        }

        process_request = SON_TRUE;

        for (iterator = 0; iterator < p_req->enb_update_req.nbr_enb_list_size; iterator++)
        {
        	error_code = SON_NO_ERROR;
        	p_enb_rec = SON_PNULL;

        	p_enb_rec = x2_modify_nbr_enb(x2_get_enb_nrt(),
        						  	  	  &p_req->enb_update_req.nbr_enb_list[iterator],
        						  	  	  &error_code);
            /*Coverity fixes.removal of p_pending_resp_enb*/

        	 anr_add_enb_x2_pending_resp_elem(p_pending_resp,
			        					  &p_req->enb_update_req.nbr_enb_list[iterator],
			        					  p_enb_rec,
			        					  error_code);
        }

        /* Send message to X2 */
        anr_construct_send_message_to_x2(p_pending_resp);

        if (!p_pending_resp->expected_res_count)
        {
        	anr_construct_send_message_to_oam(p_pending_resp);
        }
    } while (0);

    if (SON_FALSE == process_request)
    {
        anr_create_send_generic_nack(p_msg,
                p_cspl_hdr,
                error_code);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_nr_enb_update_req_handler */

/*****************************************************************************
 * Function Name  : anr_nr_enb_delete_req_handler
 * Inputs         : p_cspl_hdr  - Pointer to the CSPL Header
 *                : p_msg       - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*
 * Description    : This is the message handler for ANR module and is
 *                  invoked when  SONANR_NR_ENB_DELETE_REQ message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_nr_enb_delete_req_handler
(
 STACKAPIHDR *p_cspl_hdr,
 son_u8 *p_msg
 )
{
    son_anr_enb_delete_req_t        *p_req = (son_anr_enb_delete_req_t *)p_msg;
    anr_enb_pending_res_data_t 		*p_pending_resp = SON_PNULL;
    son_nbr_enb_hash_tuple_t        *p_enb_rec		= SON_PNULL;
    son_nbr_enb_hash_rec_t          *p_enb_data      = SON_PNULL;
    son_bool_et             		process_request = SON_FALSE;
    son_error_et            		error_code      = SON_NO_ERROR;
    son_u8							iterator 		= 0;
    son_neighbor_enb_info_t         enb_info;


    SON_UT_TRACE_ENTER();

    do
    {
        p_pending_resp = anr_add_x2_pending_resp_list(p_req->transaction_id,
        											  SONANR_NR_ENB_DELETE_RESP);
        if (SON_PNULL == p_pending_resp)
        {
			SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_nr_enb_delete_req_handler: Memory allocation failure!\n");

        	error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE;
        	break;
        }

        process_request = SON_TRUE;

        for (iterator = 0; iterator < p_req->enb_del_req.nbr_enb_id_list_size; iterator++)
        {
        	error_code = SON_NO_ERROR;
        	p_enb_rec = SON_PNULL;
            p_enb_data = SON_PNULL;
            SON_MEMSET(&enb_info, 0, sizeof(enb_info));
            SON_MEMCPY(&enb_info.enb_id, &p_req->enb_del_req.nbr_enb_id_list[iterator],
                       sizeof(son_global_enb_id_t));

        	p_enb_rec = x2_find_nbr_enb(x2_get_enb_nrt(),
        						  	  	&p_req->enb_del_req.nbr_enb_id_list[iterator],
        						  	  	&error_code);

            if (p_enb_rec && (X2_ENB_STATE_DISCONNECTED == p_enb_rec->data.enb_curr_fsm_state))
        	{
        		/* Delete eNB and neighbor cells belonging to this eNB from NRT */
        		x2_del_nbr_enb(x2_get_enb_nrt(),
        					  &p_req->enb_del_req.nbr_enb_id_list[iterator],
        					  &error_code);
        		p_enb_rec = SON_PNULL;
                continue;
        	}
        	else
            {
                enb_info.bitmask |= SON_ANR_ENB_CUR_X2_CONN_PRESENT;
                enb_info.x2_connection_status = SON_X2_DISCONNECT;
                if (SON_PNULL != p_enb_rec)
                {
                    p_enb_data = &p_enb_rec->data;
                }
            }

        	 /*Coverity fix, removal of p_pending_resp_enb */
            anr_add_enb_x2_pending_resp_elem(p_pending_resp,
			        					  &enb_info,
			        					  p_enb_data,
			        					  error_code);
        }

        /* Send message to X2 */
        anr_construct_send_message_to_x2(p_pending_resp);

        if (!p_pending_resp->expected_res_count)
        {
        	anr_construct_send_message_to_oam(p_pending_resp);
        }
    } while (0);

    if (SON_FALSE == process_request)
    {
        anr_create_send_generic_nack(p_msg,
                p_cspl_hdr,
                error_code);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_nr_enb_delete_req_handler */

/*****************************************************************************
 * Function Name  : is_enb_matches_x2_connection_status
 * Inputs         : p_hash_rec     - Pointer to enb hash record data
 *                : x2_conn_status - X2 connection status
 * Outputs        : None
 * Returns        : son_bool_et -
*                   SON_TRUE  - The x2 connection status matches
*                   or
*                   SON_FALSE - The x2 connection status does not matches
 * Description    : This is invoked from anr_nr_enb_retrieve_req_handler to
 *                  match the x2 connection status of the enb with the value
*                   received in  retrieve req
 *****************************************************************************/
son_bool_et
is_enb_matches_x2_connection_status
(
 son_nbr_enb_hash_rec_t         *p_hash_rec,
 son_x2_connection_status_et    x2_conn_status
 )
{
    if (SON_PNULL == p_hash_rec || 0 == x2_conn_status)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "is_enb_matches_x2_connection_status: "
                "Invalid params. p_hash_rec is NULL or x2_conn_status is Zero\n");

        return SON_FALSE;
    }
    
    /* Validating the x2 connection status for retrieve req */
    switch (x2_conn_status)
    {
        case SON_X2_CONNECTED:
        {
            if (X2_ENB_STATE_CONNECTED == p_hash_rec->enb_curr_fsm_state)
            {
                return SON_TRUE;
            }

            break;
        }

        case SON_X2_CONNECTED_WHITELISTED:
        {
            if (X2_ENB_STATE_CONNECTED == p_hash_rec->enb_curr_fsm_state &&
                    SON_NR_LOCKED == p_hash_rec->nr_status)
            {
                return SON_TRUE;
            }

            break;
        }

        case SON_X2_DISCONNECTED:
        {
            if (X2_ENB_STATE_DISCONNECTED == p_hash_rec->enb_curr_fsm_state)
            {
                return SON_TRUE;
            }

            break;
        }

        case SON_X2_DISCONNECTED_BLACKLISTED:
        {
            if (X2_ENB_STATE_DISCONNECTED == p_hash_rec->enb_curr_fsm_state &&
                    SON_NR_LOCKED == p_hash_rec->nr_status)
            {
                return SON_TRUE;
            }

            break;
        }

        default:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "is_enb_matches_x2_connection_status: "
                    "Incorrect X2 Connection Status");
            break;
    }
    } /* switch */
    
    return SON_FALSE;
} /* is_enb_matches_x2_connection_status */

/*****************************************************************************
 * Function Name  : anr_construct_enb_retrieve_res
 * Inputs         : p_enb_tuple - Pointer to enb hash tuple 
 * Outputs        : p_res      -   Pointer to the enb retrieve response
 * Returns        : son_return_et -
*                   SON_SUCCESS - If a peer eNB record is added in the
*                                 retrieve repsonse.
*                   or
*                   SON_FAILURE - For other conditions
 * Description    : This is invoked from anr_nr_enb_retrieve_req_handler to
*                   populate the enb retrieve response.
 *****************************************************************************/
son_return_et
anr_construct_enb_retrieve_res
( 
 son_nbr_enb_hash_tuple_t       *p_enb_tuple,
 son_anr_enb_retrieve_resp_t    *p_res
 )
{
    son_u8  list_size = p_res->enb_retrieve_res.nbr_enb_list_size;


    SON_UT_TRACE_ENTER();

    if (p_enb_tuple == SON_PNULL)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                "anr_construct_enb_retrieve_res: "
                "Invalid params. p_enb_tuple is NULL\n");

        p_res->enb_retrieve_res.result     = SON_FAILURE;
        p_res->enb_retrieve_res.error_code = SON_ERR_INVALID_PARAMS;
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    /* Populating response structure */
    p_res->enb_retrieve_res.result     =  SON_SUCCESS;
    p_res->enb_retrieve_res.error_code =  SON_NO_ERROR;

    /*Klockwork fix*/
    if (list_size < SON_MAX_NO_NEIGHBOUR_ENBS)
    {
        SON_MEMCPY(&p_res->enb_retrieve_res.nbr_enb_list[list_size].enb_id,
                &p_enb_tuple->data.enb_id,
                sizeof(son_global_enb_id_t));

        p_res->enb_retrieve_res.nbr_enb_list[list_size].enb_tnl_address_list_size =
            p_enb_tuple->data.enb_tnl_address_list_size;

        /*Klockwork fix*/
        SON_MEMCPY(&p_res->enb_retrieve_res.nbr_enb_list[list_size].
                enb_tnl_address_list,
                &p_enb_tuple->data.enb_tnl_address_list,
            (sizeof(son_enb_tnl_address_t) * SON_MAX_NO_ENB_X2_TLAS));

        p_res->enb_retrieve_res.nbr_enb_list[list_size].x2_status = p_enb_tuple->data.x2_status;

        /* BugId - 21401 start */
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_DETAILED,"anr_construct_enb_retrieve_res -p_enb_tuple->data.enb_curr_fsm_state is %d nr_status is %d \n",p_enb_tuple->data.enb_curr_fsm_state,p_enb_tuple->data.nr_status);
        /* BugId - 21401 end */

        /* Setting x2 connection status */
        if (p_enb_tuple->data.enb_curr_fsm_state == X2_ENB_STATE_CONNECTED)
        {
            if (p_enb_tuple->data.nr_status == SON_NR_LOCKED)
            {
                p_res->enb_retrieve_res.nbr_enb_list[list_size].x2_connection_status =
                    SON_X2_CONNECTED_WHITELISTED; 
            }
            else
            {
                p_res->enb_retrieve_res.nbr_enb_list[list_size].x2_connection_status =
                    SON_X2_CONNECTED;
            }
        } 
        else if (p_enb_tuple->data.enb_curr_fsm_state == X2_ENB_STATE_DISCONNECTED)
        {
            if (p_enb_tuple->data.nr_status == SON_NR_LOCKED)
            {
                p_res->enb_retrieve_res.nbr_enb_list[list_size].x2_connection_status =
                    SON_X2_DISCONNECTED_BLACKLISTED;
            }
            else
            {
                p_res->enb_retrieve_res.nbr_enb_list[list_size].x2_connection_status =
                    SON_X2_DISCONNECTED;
            }
        }
        else if (p_enb_tuple->data.enb_curr_fsm_state == X2_ENB_STATE_CONNECTING)
        {
            p_res->enb_retrieve_res.nbr_enb_list[list_size].x2_connection_status =
                SON_X2_CONNECTING;
        }
        else if (p_enb_tuple->data.enb_curr_fsm_state == X2_ENB_STATE_DISCONNECTING)
        {
            p_res->enb_retrieve_res.nbr_enb_list[list_size].x2_connection_status =
                SON_X2_DISCONNECTING;
        }

        if (p_enb_tuple->data.tac_list_size)
        {
            SON_MEMCPY(p_res->enb_retrieve_res.nbr_enb_list[list_size].tac_list,
                    p_enb_tuple->data.tac_list,
                    sizeof(p_res->enb_retrieve_res.nbr_enb_list[list_size].tac_list));

            p_res->enb_retrieve_res.nbr_enb_list[list_size].tac_list_size = p_enb_tuple->data.tac_list_size;

            p_res->enb_retrieve_res.nbr_enb_list[list_size].bitmask |= SON_ANR_ENB_TAC_PRESENT;
        }

        p_res->enb_retrieve_res.nbr_enb_list[list_size].bitmask |=
            SON_ANR_ENB_TNL_ADD_LIST_PRESENT |
            SON_ANR_ENB_X2_HO_STATUS_PRESENT |
            SON_ANR_ENB_CUR_X2_CONN_PRESENT;

        p_res->enb_retrieve_res.nbr_enb_list_size++;

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_BRIEF,
                "Retrieve request succesful for eNB id [ENB ID: 0x%x]",
                son_convert_char_cell_id_to_int_cell_id(p_enb_tuple->data.enb_id.enb_id));
    }
    else
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_BRIEF,
                "List size exceeds");
    }

    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* anr_construct_enb_retrieve_res */

/*****************************************************************************
 * Function Name  : anr_nr_enb_retrieve_req_handler
 * Inputs         : p_cspl_hdr  - Pointer to the CSPL Header information
 *                : p_msg       - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when  SONANR_NR_ENB_RETRIEVE_REQ message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_nr_enb_retrieve_req_handler
(
            /*SPR 17777 +-*/ 
 son_u8 *p_msg
 )
{
    son_anr_enb_retrieve_req_t  *p_req  = (son_anr_enb_retrieve_req_t *)p_msg;
    son_u32                     bitmask = p_req->enb_retrieve_req.bitmask;
    son_nbr_enb_hash_tuple_t    *p_enb_tuple    = SON_PNULL;
    son_error_et                error_code      = SON_NO_ERROR;
    son_anr_enb_retrieve_resp_t	res;


    SON_UT_TRACE_ENTER();

    SON_MEMSET(&res, 0, sizeof(res));
    res.transaction_id = p_req->transaction_id;
    res.enb_retrieve_res.result = SON_SUCCESS;
    res.enb_retrieve_res.error_code = error_code;

    do
    {
        /* Checking if none or more than one parameters are present in retrieve req */
        if (!(bitmask == SON_RTRV_NBR_ENB_ID_PRESENT ||
                    bitmask == SON_RTRV_X2_HO_STATUS_PRESENT ||
                    bitmask == SON_RTRV_X2_CON_STATUS_PRESENT ||
              bitmask == 0))
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_nr_enb_retrieve_req_handler: "
                    "Invalid params. None or more than one parameters are present in retrieve req\n");

            res.enb_retrieve_res.result     = SON_FAILURE;
            res.enb_retrieve_res.error_code = SON_ERR_INVALID_PARAMS;
            break;
        }

        /* Fetching the first record from neighbor enb NRT */
        p_enb_tuple = x2_get_first_nbr_enb_from_nrt(x2_get_enb_nrt());

        /* Checking if the neighbor enb NRT is empty */
        if (SON_PNULL == p_enb_tuple)
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_nr_enb_retrieve_req_handler: "
                    "Empty neighbor eNB NRT\n");

            res.enb_retrieve_res.result      = SON_FAILURE;
            res.enb_retrieve_res.error_code  = SON_ERR_ANR_NR_NOT_FOUND;
            break;
        }

        /* Retrieving info w.r.t enb id */ 
        if (bitmask == SON_RTRV_NBR_ENB_ID_PRESENT)
        {
            p_enb_tuple = x2_find_nbr_enb(x2_get_enb_nrt(),
                    &p_req->enb_retrieve_req.nbr_enb_id,
                    &error_code);

            if (SON_PNULL == p_enb_tuple)
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_nr_enb_retrieve_req_handler: "
                        "Invalid NR eNB id [ENB Id: 0x%x]\n",
                        son_convert_char_cell_id_to_int_cell_id(p_req->enb_retrieve_req.nbr_enb_id.enb_id));

                res.enb_retrieve_res.result      = SON_FAILURE;
                res.enb_retrieve_res.error_code  = error_code;
            }
            else
            {
                anr_construct_enb_retrieve_res(p_enb_tuple, &res);
            }
        }

        /* Retrieving info w.r.t x2 status */
        else if (bitmask == SON_RTRV_X2_HO_STATUS_PRESENT)
        {
            while (SON_PNULL != p_enb_tuple)
            {
                if (p_enb_tuple->data.x2_status == p_req->enb_retrieve_req.x2_status)
                {
                    anr_construct_enb_retrieve_res(p_enb_tuple, &res);
                }

                p_enb_tuple = x2_get_next_nbr_enb_from_nrt(x2_get_enb_nrt(),
                    &p_enb_tuple->anchor);
            }

            if (!res.enb_retrieve_res.nbr_enb_list_size)
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_nr_enb_retrieve_req_handler: "
                        "No nbr ENB id available with the X2 HO Status as [%d]\n",
                        p_req->enb_retrieve_req.x2_status);

                res.enb_retrieve_res.result      = SON_FAILURE;
                res.enb_retrieve_res.error_code  = SON_ERR_ANR_NR_NOT_FOUND;                
            }
        }

        /* Retrieving info w.r.t x2 connection status */ 
        else if (bitmask == SON_RTRV_X2_CON_STATUS_PRESENT)
        {
            if (!(p_req->enb_retrieve_req.x2_connection_status == SON_X2_CONNECTED ||
                        p_req->enb_retrieve_req.x2_connection_status == 
                        SON_X2_CONNECTED_WHITELISTED ||
                        p_req->enb_retrieve_req.x2_connection_status == 
                        SON_X2_DISCONNECTED ||
                        p_req->enb_retrieve_req.x2_connection_status == 
                        SON_X2_DISCONNECTED_BLACKLISTED))
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_nr_enb_retrieve_req_handler: "
                        "X2 connection status is not Connected or "
                        "Disconnected\n");

                res.enb_retrieve_res.result      = SON_FAILURE;
                res.enb_retrieve_res.error_code  = SON_ERR_INVALID_PARAMS;
            }
            else
            {
                while (SON_PNULL != p_enb_tuple)
                {
                    if (SON_TRUE == is_enb_matches_x2_connection_status
                            (&p_enb_tuple->data,
                             p_req->enb_retrieve_req.x2_connection_status))
                    {
                        anr_construct_enb_retrieve_res(p_enb_tuple, &res);
                    }

                    p_enb_tuple = x2_get_next_nbr_enb_from_nrt(x2_get_enb_nrt(),
                            &p_enb_tuple->anchor);
                }

                if (!res.enb_retrieve_res.nbr_enb_list_size)
                {
                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                            SON_ERROR,
                            "anr_nr_enb_retrieve_req_handler: "
                            "No nbr ENB id available with the X2 Conn Status as [%d]\n",
                            p_req->enb_retrieve_req.x2_connection_status);

                    res.enb_retrieve_res.result      = SON_FAILURE;
                    res.enb_retrieve_res.error_code  = SON_ERR_ANR_NR_NOT_FOUND;
                }
            }     
        }

        /* Retrieving info for all the enb neighbours, without any criteria */
        else
        {
            while (SON_PNULL != p_enb_tuple)
            {
                anr_construct_enb_retrieve_res(p_enb_tuple, &res);

                p_enb_tuple = x2_get_next_nbr_enb_from_nrt(x2_get_enb_nrt(),
                        &p_enb_tuple->anchor);
            }
        }

        /*Klocwork Fix Start*/
    } while (0); /*Klocwork Fix End*/
    
    /* Coverity Fix: 19170,19175 */
    son_create_send_buffer((son_u8 *)&res, SON_ANR_MODULE_ID,
            SON_MIF_MODULE_ID, SONANR_NR_ENB_RETRIEVE_RESP,
            sizeof(res));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_nr_enb_retrieve_req_handler */

/*****************************************************************************
 * Function Name  : anr_x2_add_enb_res_handler
 * Inputs         : p_cspl_hdr   - Pointer to the CSPL Header Information
 *                : p_msg        - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when  SONX2_ADD_ENB_RES message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_x2_add_enb_res_handler
(
            /*SPR 17777 +-*/ 
 son_u8 *p_msg
 )
{
    son_x2_add_enb_res_t            *p_res = (son_x2_add_enb_res_t *)p_msg;
	anr_enb_pending_res_node_t		*p_pending_res = SON_PNULL;
	anr_pending_nbr_enb_info_data_t	*p_pending_res_enb = SON_PNULL;
	SON_LIST_NODE					*p_list_node = SON_PNULL;
	son_enb_add_status_node_t		*p_enb_sts = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_pending_res = anr_find_x2_pending_resp_list(p_res->transaction_id);

    if (SON_PNULL != p_pending_res)
    {
    	p_list_node = list_pop_node(&p_res->enb_list);

        while (SON_PNULL != p_list_node)
    	{
    		p_enb_sts = YMEMBEROF(son_enb_add_status_node_t,
             				  anchor, p_list_node);

            if (SON_PNULL != p_enb_sts && SON_PNULL != p_enb_sts->data.p_enb_hash_rec)
            {
                p_pending_res_enb = anr_find_enb_x2_pending_resp_elem(&p_pending_res->data,
                        &p_enb_sts->data.p_enb_hash_rec->enb_id);

                /* SPR 11340 Fix Starts */
                if (SON_PNULL == p_pending_res_enb)
                {
                    SON_LOG(
                            anr_get_log_mode(), p_son_anr_facility_name,
                            SON_BRIEF,
                            "No entry found in X2 Pending response list for eNB [ENB Id: 0x%x]\n",
                            son_convert_char_cell_id_to_int_cell_id(p_enb_sts->data.p_enb_hash_rec->enb_id.enb_id));
                }
                else
                {
                    if (SON_SUCCESS == p_enb_sts->data.result)
                    {
                        SONANR_SET_ENB_PENDING_RES_ERROR_CODE(p_pending_res_enb,
                                SON_NO_ERROR,
                                p_pending_res->data.api_id,
                                p_pending_res->data.transaction_id);
                    }

                    SONANR_DECR_X2_PENDING_RES_COUNT((&p_pending_res->data));
                }
                /* SPR 11340 Fix Ends */
            }

    		/* Free the memory occupied by the node */
    		son_mem_free(p_enb_sts);

    		p_list_node = list_pop_node(&p_res->enb_list);
    	}

        if (!p_pending_res->data.expected_res_count)
		{
			anr_construct_send_message_to_oam(&p_pending_res->data);
		}
    }
    else
    {
    	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
    	   	    SON_BRIEF,
    	   	    "No entry found in X2 Pending response list for Trans Id: [%d]\n",
    	   	    p_res->transaction_id);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_x2_add_enb_res_handler */

/*****************************************************************************
 * Function Name  : anr_x2_link_up_ind_handler
 * Inputs         : p_cspl_hdr   - Pointer to the CSPL Header Information
 *                : p_msg        - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when  SONX2_LINK_UP_IND message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_x2_link_up_ind_handler
(
 STACKAPIHDR *p_cspl_hdr,
 son_u8 *p_msg
 )
{
    son_x2_link_up_ind_t        *p_res = (son_x2_link_up_ind_t *)p_msg;
    /* SPR 11340 Fix Starts */
    /* Lines deleted */
    /* SPR 11340 Fix Ends */

    SON_UT_TRACE_ENTER();

    if (SON_PNULL != p_res->p_enb_rec)
    {
        /* SPR-10331 Fix Starts */
        anr_send_enb_config_update_ind_to_rrm(p_cspl_hdr, 
                &p_res->p_enb_rec->enb_id, p_res);
        /* SPR-10331 Fix Ends */

        /* SPR 11340 Fix Starts */
        /* Lines deleted */
        /* SPR 11340 Fix Ends */

        /*ENB CONFIG CHANGE REQ SPR:START*/
        anr_process_x2_link_up_ind_for_nrt_updation(p_res->p_served_cell_info,
                p_res->num_served_cell,
                &p_res->p_enb_rec->enb_id,
            /*SPR 17777 +-*/ 
                p_res->p_enb_rec
                /* SPR 11340 Fix Starts */
                );
            /*SPR 17777 +-*/ 
        /* SPR 11340 Fix Ends */
        /*ENB CONFIG CHANGE REQ SPR:STOP*/
    }
    else
    {
        /* SPR 11340 Fix Starts */
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_BRIEF,
                "No eNB record is present in SONX2_LINK_UP_IND received from X2!\n");
        /* SPR 11340 Fix Ends */
    }

    /*SPR 17006 Fix Start*/
    /*Code removed*/
    /*SPR 17006 Fix End*/

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_x2_link_up_ind_handler */

/*****************************************************************************
 * Function Name  : anr_x2_link_down_ind_handler
 * Inputs         : p_cspl_hdr  - Pointer to the CSPL Header Information
 *                : p_msg       - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when  SONX2_LINK_DOWN_IND message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_x2_link_down_ind_handler
(
            /*SPR 17777 +-*/ 
 son_u8 *p_msg
 )
{
    son_x2_link_down_ind_t      *p_res = (son_x2_link_down_ind_t *)p_msg;
    /* SPR 11340 Fix Starts */
    /* Lines deleted */
    /* SPR 11340 Fix Ends */


    SON_UT_TRACE_ENTER();

    if (SON_PNULL != p_res->p_enb_rec)
    {
        /* SPR 11340 Fix Starts */
        /* Lines deleted */
        /* SPR 11340 Fix Ends */
        anr_update_x2_status_nbr_cells_nrt(&p_res->p_enb_rec->enb_id);
    }
	else
	{
        /* SPR 11340 Fix Starts */
		SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
				SON_BRIEF,
				"No eNB record is present in SONX2_LINK_DOWN_IND received from X2!\n");
        /* SPR 11340 Fix Ends */
	}

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_x2_link_down_ind_handler */

/*****************************************************************************
 * Function Name  : anr_x2_enb_config_update_ind_handler
 * Inputs         : p_cspl_hdr  - Pointer to the CSPL Header Information
 *                : p_msg_rcvd  - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when  SONX2_ENB_CONFIG_UPDATE_IND message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_x2_enb_config_update_ind_handler
(
 STACKAPIHDR *p_cspl_hdr,
 son_u8 *p_msg_rcvd
 )
{
	anr_cell_context_node_t   		*p_cell_ctxt_tpl        = SON_PNULL;
    anr_cell_context_t        		*p_cell_ctxt            = SON_PNULL;
    /* SPR_19279_START/END Code Deleted */
    son_bool_et               		is_anr_cell_updated     = SON_FALSE;
    son_anr_rrm_updated_nrt_info_t  *p_notification_msg     = SON_PNULL;	
    son_x2_enb_config_update_ind_t  *p_ind = 
        (son_x2_enb_config_update_ind_t *)p_msg_rcvd;
    son_x2_nrt_update_ind_t        nrt_upd_ind         = {0};
    son_bool_et                    ret                 = SON_FALSE;
    /*SPR 17777 +-*/

    son_anr_updated_nrt_info_t     *p_oam_update_notification_for_add_op = SON_PNULL;
    son_anr_updated_nrt_info_t     *p_oam_update_notification_for_update_op = SON_PNULL;
    son_anr_updated_nrt_info_t     *p_oam_update_notification_for_delete_op = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_ind)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_x2_enb_config_update_ind_handler:"
                "Invalid parameters.p_ind is NULL\n");
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    /* SPR_19279_START */
    if(SON_PNULL == p_ind->p_update_ind)
    /* SPR_19279_END */
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_x2_enb_config_update_ind_handler:"
                "Invalid parameters. No matching eNB found\n");
        /* SPR 21119 Fix Start */
        /* Code Deleted */
        /* SPR 21119 Fix End */
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    /* SPR-10331 Fix Starts */
    anr_send_enb_config_update_ind_to_rrm(p_cspl_hdr, 
            /* SPR_19279_START */
            &p_ind->new_eNB_id, p_ind);
            //&p_ind->old_eNB_id, p_ind);
            /* SPR_19279_END */
    /* SPR-10331 Fix Ends */

    p_notification_msg = (son_anr_rrm_updated_nrt_info_t *)son_mem_get(sizeof(son_anr_rrm_updated_nrt_info_t));
    if (SON_PNULL == p_notification_msg)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_x2_enb_config_update_ind_handler:"
                "Memory Allocation failure\n");
        /*SPR 15935 Fix Start*/
        if(SON_PNULL != p_ind->p_update_ind)
        {
            son_mem_free(p_ind->p_update_ind);
        }
        /*SPR 15935 Fix Stop*/
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

    p_oam_update_notification_for_add_op = (son_anr_updated_nrt_info_t *)
                                  son_mem_get(sizeof(son_anr_updated_nrt_info_t)); 
    if (SON_PNULL == p_oam_update_notification_for_add_op)
     {
        if (SON_PNULL != p_notification_msg)
        {
            son_mem_free(p_notification_msg);
        }
        /*SPR 15935 Fix Start*/
        if(SON_PNULL != p_ind->p_update_ind)
        {
            son_mem_free(p_ind->p_update_ind);
        }
        /*SPR 15935 Fix Stop*/

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_x2_enb_config_update_ind_handler:"
                       "Memory Allocation failure\n");
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

   p_oam_update_notification_for_update_op = (son_anr_updated_nrt_info_t *)
                                        son_mem_get(sizeof(son_anr_updated_nrt_info_t));
    if (SON_PNULL == p_oam_update_notification_for_update_op)
    {
        if (SON_PNULL != p_notification_msg)
        {
            son_mem_free(p_notification_msg);
        }

        if (SON_PNULL != p_oam_update_notification_for_add_op)
        {
            son_mem_free(p_oam_update_notification_for_add_op);
        }
        /*SPR 15935 Fix Start*/
        if(SON_PNULL != p_ind->p_update_ind)
        {
            son_mem_free(p_ind->p_update_ind);
        }
        /*SPR 15935 Fix Stop*/
        
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_x2_enb_config_update_ind_handler:"
                "Memory Allocation failure\n");
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }

   p_oam_update_notification_for_delete_op = (son_anr_updated_nrt_info_t *)
                                          son_mem_get(sizeof(son_anr_updated_nrt_info_t));
    if (SON_PNULL == p_oam_update_notification_for_delete_op)
   {
        if (SON_PNULL != p_notification_msg)
        {
            son_mem_free(p_notification_msg);
        }

        if (SON_PNULL != p_oam_update_notification_for_add_op)
        {
            son_mem_free(p_oam_update_notification_for_add_op);
        }

        if (SON_PNULL != p_oam_update_notification_for_update_op)
        {
            son_mem_free(p_oam_update_notification_for_update_op);
        }
        /*SPR 15935 Fix Start*/
        if(SON_PNULL != p_ind->p_update_ind)
        {
            son_mem_free(p_ind->p_update_ind);
        }
        /*SPR 15935 Fix Stop*/
       
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_x2_enb_config_update_ind_handler:"
                "Memory Allocation failure\n");
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
   }
    /*SPR 16523 fix start*/	
    /*nrt_upd_ind.p_enb_rec_to_exclude =     p_ind->old_enb_id;*/ 
    /*SPR 16523 fix stop*/	
    
    /* SPR_19279_START */
    /* Code Deleted */
    /* SPR_19279_END */
    p_cell_ctxt_tpl =  anr_get_first_cell_ctxt();

    /*SPR 17777 +-*/
    while (SON_PNULL != p_cell_ctxt_tpl)
    {
        p_cell_ctxt = &p_cell_ctxt_tpl->data;

        if (ANR_CELL_STATE_ENABLED == p_cell_ctxt->current_cell_fsm_state)
        {
            /*Set all the notifications msg to various entites*/
            SON_MEMSET(p_oam_update_notification_for_add_op, 0,
                    sizeof(son_anr_updated_nrt_info_t));
            SON_MEMSET(p_oam_update_notification_for_update_op, 0,
                    sizeof(son_anr_updated_nrt_info_t));
            SON_MEMSET(p_oam_update_notification_for_delete_op, 0,
                    sizeof(son_anr_updated_nrt_info_t));

            is_anr_cell_updated = SON_TRUE;

            SON_MEMSET(p_notification_msg, 0, sizeof(son_anr_rrm_updated_nrt_info_t));

            SON_MEMCPY(&p_notification_msg->src_cgi, &p_cell_ctxt->cell_id,
                    sizeof(son_rrm_intra_rat_global_cell_id_t));

            SON_MEMCPY(&p_oam_update_notification_for_add_op->src_cgi,
                &p_cell_ctxt->cell_id, sizeof(son_intra_rat_global_cell_id_t));
            SON_MEMCPY(&p_oam_update_notification_for_update_op->src_cgi,
                &p_cell_ctxt->cell_id, sizeof(son_intra_rat_global_cell_id_t));
            SON_MEMCPY(&p_oam_update_notification_for_delete_op->src_cgi,
                &p_cell_ctxt->cell_id, sizeof(son_intra_rat_global_cell_id_t));

            /*traverse all the cell ids in the del list*/
            ret |=  anr_traverse_enb_config_update_ind_del_list(p_cell_ctxt,
                    &p_ind->p_update_ind->del_served_cell,
                    /* SPR_19279_START */
                    &p_ind->old_eNB_id,
                    /* SPR_19279_END */
                    p_oam_update_notification_for_delete_op,
                    p_notification_msg);

            /*traverse all the cell ids in the modify list*/
            ret |=  anr_traverse_enb_config_update_ind_modify_list(p_cell_ctxt,
                    &p_ind->p_update_ind->mod_served_cell,
                    /*SPR 17777 +-*/
                    /* SPR_19279_START */
                    &p_ind->old_eNB_id,
                    &p_ind->new_eNB_id,
                    /* SPR_19279_END */
                    p_oam_update_notification_for_add_op,
                    p_oam_update_notification_for_update_op,
                    p_oam_update_notification_for_delete_op,
                    p_notification_msg);

            /*traverse all the cell ids in the add list*/
            ret |=  anr_traverse_enb_config_update_add_list(p_cell_ctxt,
                    &p_ind->p_update_ind->add_served_cell,
                    /*SPR 17777 +-*/
                    /* SPR_19279_START */
                    &p_ind->new_eNB_id,
                    /* SPR_19279_END */
                    p_oam_update_notification_for_add_op,
                    p_oam_update_notification_for_update_op,
                    p_notification_msg);
	    /*SPR 16523 fix start*/		
            if (SON_TRUE == ret)
            {
                anr_populate_x2_nrt_update_ind(&nrt_upd_ind,
                    &p_cell_ctxt->cell_id, SON_NRT_OP_UPDATE);
	    } 
            ret = SON_FALSE; 
	    /*SPR 16523 fix stop*/		

	    /* Spr 17753 Changes Start */ 
            /* Send update notification to OAM with operation performed as ADD if add
             * operation is done in NRT */
            
            anr_send_updated_ncl_to_oam(p_oam_update_notification_for_delete_op);
            anr_send_updated_ncl_to_oam(p_oam_update_notification_for_update_op);
            anr_find_conflict_in_nrt_n_send_conflict_ind(p_cell_ctxt);
            anr_send_updated_ncl_to_oam(p_oam_update_notification_for_add_op);
            /* Start NR add timer if no timer is running */
            if(0 < p_oam_update_notification_for_add_op->nr_list.intra_rat_nr_list_size)
            {
                if((p_cell_ctxt->nr_add_wait_timer_val)&&(!p_cell_ctxt->son_nr_add_timer_id))
                {
                    p_cell_ctxt->son_nr_add_timer_id = 
                        son_start_timer(p_cell_ctxt->nr_add_wait_timer_val * SON_MILLISECONDS_IN_A_SECOND, 
                                SON_PNULL,
                                0,
                                 SON_FALSE);

                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_BRIEF, "NR add timer [Id: %p] started to wait for NR add req from OAM",
                    p_cell_ctxt->son_nr_add_timer_id);
                }
                else
                {
                    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_BRIEF, "either nr add timer is already running"
                        " or timer val is zero"
                        " not starting timer again");
                } 
            }
            /* Spr 17753 Changes End */
            /* Send updated NRT info to MLB for neighbor deletion */
            if (MLB_CELL_STATE_ENABLED == mlb_get_cell_mode(&p_cell_ctxt->cell_id))
            {
                anr_send_updated_ncl_to_mlb(p_oam_update_notification_for_delete_op);
            }

            /*Send RRM notification to RRM */
            anr_send_updated_ncl_to_rrm(p_notification_msg,
                    son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
        }

        anr_print_cell_context(p_cell_ctxt);
        p_cell_ctxt_tpl = anr_get_next_node(p_cell_ctxt_tpl);
    }

    if (nrt_upd_ind.num_srv_cells_list_size)
   {
        son_create_send_buffer((son_u8 *)&nrt_upd_ind, SON_ANR_MODULE_ID,
            SON_X2_MODULE_ID, SONX2_NRT_UPDATE_IND,
               sizeof(nrt_upd_ind));

       SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_BRIEF, 
               "SONX2 NRT UPDATE sent to X2 ");
   }

    if (SON_FALSE == is_anr_cell_updated)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "anr_x2_enb_config_update_ind_handler:"
                "NO Srving cell in ANR ENABLED state");
    }

    if (SON_PNULL != p_notification_msg)
    {
        son_mem_free(p_notification_msg);
    }

    if (SON_PNULL != p_oam_update_notification_for_add_op)
    {
        son_mem_free(p_oam_update_notification_for_add_op);
    }

    if (SON_PNULL != p_oam_update_notification_for_update_op)
    {
        son_mem_free(p_oam_update_notification_for_update_op);
    }

    if (SON_PNULL != p_oam_update_notification_for_delete_op)
    {
        son_mem_free(p_oam_update_notification_for_delete_op);
    }

   /* SPR 21119 Fix Start */
     /* SPR 17045 Reverted */
    if(SON_PNULL != p_ind->p_update_ind)
    {
        son_mem_free(p_ind->p_update_ind);
    }
    /* SPR 21119 Fix End */ 

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_x2_enb_config_update_ind_handler */


/*****************************************************************************
 * Function Name  : anr_x2_delete_enb_res_handler
 * Inputs         : p_cspl_hdr  - Pointer to the CSPL Header Information
 *                : p_msg       - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when  SONX2_DELETE_ENB_RES message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_x2_delete_enb_res_handler
(
            /*SPR 17777 +-*/ 
 son_u8 *p_msg
 )
{
    son_x2_delete_enb_res_t             *p_res = (son_x2_delete_enb_res_t *)p_msg;
	anr_enb_pending_res_node_t		*p_pending_res = SON_PNULL;
	anr_pending_nbr_enb_info_data_t	*p_pending_res_enb = SON_PNULL;


    SON_UT_TRACE_ENTER();

    if (p_res->p_enb_data_list)
    {
		p_pending_res = anr_find_x2_pending_resp_list(p_res->transaction_id);

        if (SON_PNULL != p_pending_res)
		{
			p_pending_res_enb = anr_find_enb_x2_pending_resp_elem(&p_pending_res->data,
																  &p_res->p_enb_data_list->enb_id);

            /* SPR 11340 Fix Starts */
            if (p_pending_res_enb)
			{
                /* Lines deleted */
                /* SPR 11340 Fix Ends */

				SONANR_DECR_X2_PENDING_RES_COUNT((&p_pending_res->data));

                if (SON_SUCCESS == p_res->result)
				{
					son_error_et error_code = SON_NO_ERROR;
                    if (SONANR_NR_ENB_DELETE_RESP == p_pending_res->data.api_id)
					{
						x2_del_nbr_enb(x2_get_enb_nrt(),
									   &p_res->p_enb_data_list->enb_id,
									   &error_code);
					} 
                    else if (SONANR_NR_ENB_UPDATE_RESP == p_pending_res->data.api_id)
                    {
                         anr_update_x2_status_nbr_cells_nrt(&p_res->p_enb_data_list->enb_id);
                    }

					SONANR_SET_ENB_PENDING_RES_ERROR_CODE(p_pending_res_enb,
														  error_code,
														  p_pending_res->data.api_id,
														  p_pending_res->data.transaction_id);

                    if (SON_X2_DISCONNECT_BLACKLIST == p_pending_res_enb->nb_enb.x2_connection_status)
					{
						p_res->p_enb_data_list->nr_status = SON_NR_LOCKED;
					}

                    if (SON_X2_DISCONNECT == p_pending_res_enb->nb_enb.x2_connection_status)
					{
						p_res->p_enb_data_list->nr_status = SON_NR_UNLOCKED;
					}
				}

                if (!p_pending_res->data.expected_res_count)
				{
					anr_construct_send_message_to_oam(&p_pending_res->data);
				}
			}
			else
			{
                SON_LOG(
                    anr_get_log_mode(), p_son_anr_facility_name,
						SON_BRIEF,
						"No entry found in X2 Pending response list for eNB Id: [0x%x] in API [API Id: %d] with Trans Id [Trans Id:%d]\n",
                    son_convert_char_cell_id_to_int_cell_id(
                        p_res->p_enb_data_list->enb_id.enb_id), SONX2_DELETE_ENB_RES, p_res->transaction_id);
			}
		}
		else
		{
            SON_LOG(
                anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_x2_delete_enb_res_handler: No entry found in X2 Pending response list for API [API Id: %d] and Trans Id [Trans Id: %d]\n",
					SONX2_DELETE_ENB_RES, p_res->transaction_id);
		}
    }
	else
	{
        /* SPR 11340 Fix Starts */
        SON_LOG(
            anr_get_log_mode(), p_son_anr_facility_name,
				SON_BRIEF,
				"No eNB Data present in SONX2_DELETE_ENB_RES with Trans Id [Trans Id: %d] received from X2!\n",
				p_res->transaction_id);
        /* SPR 11340 Fix Ends */
	}

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_x2_delete_enb_res_handler */

/*****************************************************************************
 * Function Name  : anr_tnl_discovery_res_handler
 * Inputs         : p_cspl_hdr  - Pointer to the CSPL Header Information
 *                : p_msg       - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when  SON_RRM_TNL_DISCOVERY_RES message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_tnl_discovery_res_handler
(
            /*SPR 17777 +-*/ 
 son_u8 *p_msg
 )
{
	son_rrm_tnl_discovery_res_t *p_res = (son_rrm_tnl_discovery_res_t *)p_msg;
	anr_enb_pending_res_node_t	*p_pending_res = SON_PNULL;
	anr_pending_nbr_enb_info_data_t	*p_pending_res_enb = SON_PNULL;
	son_nbr_enb_hash_rec_t *p_enb_rec = SON_PNULL;
	son_error_et error_code = SON_NO_ERROR;
    son_u8 idx = 0;


    SON_UT_TRACE_ENTER();

	p_pending_res = anr_find_x2_pending_resp_list(p_res->transaction_id);

    if (SON_PNULL != p_pending_res)
	{
		p_pending_res_enb = anr_find_enb_x2_pending_resp_elem(&p_pending_res->data,
            (son_global_enb_id_t *)&p_res->tnl_res.target_enb_id);

        if (p_pending_res_enb && p_pending_res_enb->expiry_time)
		{
            /* Coverity Fix: 17275 */
            if (RRM_SUCCESS == p_res->tnl_res.result && p_res->tnl_res.enb_tnl_address_list_size)
			{
				p_pending_res_enb->nb_enb.bitmask |= SON_ANR_ENB_TNL_ADD_LIST_PRESENT;

				p_pending_res_enb->nb_enb.enb_tnl_address_list_size =
														p_res->tnl_res.enb_tnl_address_list_size;
                for (idx = 0; idx < p_res->tnl_res.enb_tnl_address_list_size; idx++)
                {
                    if (p_res->tnl_res.enb_tnl_address_list[idx].bitmask &  RRM_IPV4_ADD_PRESENT)
                    {
                        p_pending_res_enb->nb_enb.enb_tnl_address_list[idx].bitmask |= IPV4_ADD_PRESENT;
    			    	SON_MEMCPY(p_pending_res_enb->nb_enb.enb_tnl_address_list[idx].ipv4_add,
					        	   p_res->tnl_res.enb_tnl_address_list[idx].ipv4_add,
						           sizeof(p_pending_res_enb->nb_enb.enb_tnl_address_list[idx].ipv4_add));
                    }

                    if (p_res->tnl_res.enb_tnl_address_list[idx].bitmask &  RRM_IPV6_ADD_PRESENT)
                    {
                        p_pending_res_enb->nb_enb.enb_tnl_address_list[idx].bitmask |= IPV6_ADD_PRESENT;
                        SON_MEMCPY(p_pending_res_enb->nb_enb.enb_tnl_address_list[idx].ipv6_add,
                                   p_res->tnl_res.enb_tnl_address_list[idx].ipv6_add,
                                   sizeof(p_pending_res_enb->nb_enb.enb_tnl_address_list[idx].ipv6_add));
                    }
                }

				p_enb_rec = x2_insert_nbr_enb(x2_get_enb_nrt(),
								  &p_pending_res_enb->nb_enb,
								  &error_code);

				p_pending_res_enb->p_enb_rec = p_enb_rec;

				SONANR_SET_ENB_PENDING_RES_ERROR_CODE(p_pending_res_enb,
													  error_code,
													  p_pending_res->data.api_id,
													  p_pending_res->data.transaction_id);

				SONANR_SET_ENB_PENDING_RES_EXPIRY_TIME(p_pending_res_enb,
													   0,
													   p_pending_res->data.api_id,
													   p_pending_res->data.transaction_id);
			}
		    
            SONANR_DECR_X2_PENDING_RES_COUNT((&p_pending_res->data));

			/* Send message to X2 */
			anr_construct_send_message_to_x2(&p_pending_res->data);

            if (!p_pending_res->data.expected_res_count)
			{
				anr_construct_send_message_to_oam(&p_pending_res->data);
			}
		}
		else
		{
            SON_LOG(
                anr_get_log_mode(), p_son_anr_facility_name,
					SON_ERROR,
					"anr_tnl_discovery_res_handler:"
                    " No entry found in X2 Pending response list for eNB Id: [0x%x] in API [API Id: %d] with Trans Id [Trans Id:%d]\n",
                son_convert_char_cell_id_to_int_cell_id(
                    p_res->tnl_res.target_enb_id.eNb_id), RRM_SON_TNL_DISCOVERY_RESP, p_res->transaction_id);
		}
	}
    else
    {
        SON_LOG(
            anr_get_log_mode(), p_son_anr_facility_name,
				SON_ERROR,
				"anr_tnl_discovery_res_handler: No entry found in X2 Pending response list for API [API Id: %d] and Trans Id [Trans Id: %d]\n",
				SONX2_DELETE_ENB_RES, p_res->transaction_id);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_tnl_discovery_res_handler */

/*****************************************************************************
 * Function Name  : anr_cell_switch_on_for_x2_setup_res_handler
 * Inputs         : p_cspl_hdr  - Pointer to the CSPL Header Information
 *                : p_msg       - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when SONES_CELL_SWITCH_ON_REQ_FOR_X2_SETUP_RES message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_cell_switch_on_for_x2_setup_res_handler
(
            /*SPR 17777 +-*/ 
 son_u8 *p_msg
 )
{
	son_es_cell_switchon_for_x2_setup_res_t *p_res =
        (son_es_cell_switchon_for_x2_setup_res_t *)p_msg;
	anr_enb_pending_res_node_t	*p_pending_res = SON_PNULL;
	anr_pending_nbr_enb_info_node_t *p_pending_res_enb = SON_PNULL;
    son_bool_et             is_es_msg_rcvd = SON_FALSE;


    SON_UT_TRACE_ENTER();

    p_pending_res = anr_find_x2_pending_resp_list(p_res->transaction_id);
    if (SON_PNULL != p_pending_res)
    {
		p_pending_res_enb = anr_get_first_enb_x2_pending_resp_elem(&p_pending_res->data);
        while (SON_PNULL != p_pending_res_enb)
		{
            if (SON_ERR_ES_CELL_SWITCHED_OFF == p_pending_res_enb->data.error_code)
			{
                if (SON_FALSE == is_es_msg_rcvd)
                {
				    SONANR_DECR_X2_PENDING_RES_COUNT((&p_pending_res->data));
                    is_es_msg_rcvd = SON_TRUE;
                }    

                if (SON_SUCCESS == p_res->result)
				{
					SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
							SON_DETAILED,
							"Cell switch On for X2 Setup is complete and "
							"successful for API [API Id: %d] with Trans Id [Trans Id: %d]\n",
                        p_pending_res->data.api_id, p_pending_res->data.transaction_id);

					SONANR_SET_ENB_PENDING_RES_ERROR_CODE((&p_pending_res_enb->data),
														  SON_NO_ERROR,
														  p_pending_res->data.api_id,
														  p_pending_res->data.transaction_id);
				}
			}

			p_pending_res_enb = anr_get_next_enb_x2_pending_resp_elem(p_pending_res_enb);
		}

		p_pending_res->data.switch_on_resp_received = SON_TRUE;
		anr_construct_send_message_to_x2(&p_pending_res->data);

        if (!p_pending_res->data.expected_res_count)
		{
			anr_construct_send_message_to_oam(&p_pending_res->data);
		}
    }
    else
    {
    	SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
    	   	    SON_ERROR,
    	   	    "anr_cell_switch_on_for_x2_setup_res_handler: No entry found in X2 Pending response list for Trans Id: [%d]\n",
    	   	    p_res->transaction_id);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_cell_switch_on_for_x2_setup_res_handler */

/*****************************************************************************
 * Function Name  : anr_process_x2_msgs
 * Inputs         : p_cspl_info - pointer to the CSPL heasder information.
 *                : p_msg       - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
 *                  RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ANR module and is
 *                   called for any message received from X2 module
 ******************************************************************************/
static son_buf_retain_status_et
anr_process_x2_msgs
(
 STACKAPIHDR *p_cspl_info,
 son_u8 *p_msg
 )
{
    son_buf_retain_status_et 	ret = RELEASE_SON_BUFFER;


    SON_UT_TRACE_ENTER();
    switch (p_cspl_info->api)
	{
    case SONX2_ADD_ENB_RES:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                    "X2->ANR:SONX2_ADD_ENB_RES");
            /*SPR 17777 +-*/ 
            ret = anr_x2_add_enb_res_handler(p_msg);
            break;
        }

    case SONX2_LINK_UP_IND:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                    "X2->ANR:SONX2_LINK_UP_IND");
            ret = anr_x2_link_up_ind_handler(p_cspl_info, p_msg);
            break;
        }

    case SONX2_LINK_DOWN_IND:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                    "X2->ANR:SONX2_LINK_DOWN_IND");
            ret = anr_x2_link_down_ind_handler(p_msg);
            break;
        }

    case SONX2_DELETE_ENB_RES:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                    "X2->ANR:SONX2_DELETE_ENB_RES");
            ret = anr_x2_delete_enb_res_handler(p_msg);
            /*SPR 17777 +-*/ 
            break;
        }

    case SONX2_ENB_CONFIG_UPDATE_IND:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                    "X2->ANR:SONX2_ENB_CONFIG_UPDATE_IND");
            ret = anr_x2_enb_config_update_ind_handler(p_cspl_info, p_msg);
            break;
        }


        default:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
                "Unexpected Message [%d] Received from "
                "entity : [%d]\n", p_cspl_info->api, p_cspl_info->from);
        ret = RELEASE_SON_BUFFER;

        break;
	}
    } /* switch */

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_process_x2_msgs */

/* Rel1.2 Changes STOP */

/*****************************************************************************
 * Function Name  : anr_process_smif_msgs
 * Inputs         : p_cspl_hdr  - Pointer to the CSPL Header Information.
 *                : p_msg       - Pointer to the incoming message.
 *                : p_cell_id   - Pointer to the cell id
 *                : event_id    - Value of event id
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
 *                  RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ANR module and is
 *                  called for any message received from MIF module
 ******************************************************************************/
static son_buf_retain_status_et
anr_process_smif_msgs
(
 STACKAPIHDR *p_cspl_hdr,
 son_u8 *p_msg,
 son_intra_rat_global_cell_id_t  *p_cell_id,
 son_anr_index_et event_id
 )
{
    son_buf_retain_status_et 	ret         		= RETAIN_SON_BUFFER;
    anr_cell_context_t 			*p_cell_ctxt		= SON_PNULL;


    SON_UT_TRACE_ENTER();

    switch (p_cspl_hdr->api)
    {
        case SONANR_ENABLE_REQ:
        {
            if (!(SON_SRC_CGI_PRESENT &
                  ((son_anr_enable_req_t *)p_msg)->enable_req.bitmask))
            {
                /*Buffer gets released so ret is RELEASE_SON_BUFFER*/
                ret = anr_global_enable_req_handler(p_cspl_hdr, p_msg, event_id);
            }

            break;
        }

        case SONANR_DISABLE_REQ:
        {
            if (!(SON_ANR_DISABLE_SRC_CGI_PRESENT &
                  ((son_anr_disable_req_t *)p_msg)->disable_req.bitmask))
            {
                /*Buffer gets released so ret is RELEASE_SON_BUFFER*/
                ret = anr_global_disable_req_handler(p_cspl_hdr, p_msg, event_id);
            }

            break;
        }

        default:
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_BRIEF,
                    "anr_process_smif_msgs: Cell specific message received\n");
            break;
    }
    } /* switch */

    if (RETAIN_SON_BUFFER == ret)
    {
        p_cell_ctxt = anr_get_cell_context_from_global_context(p_cell_id);

        if (SON_PNULL == p_cell_ctxt)
        {
            if (p_cspl_hdr->api >= SONANR_REQ_BASE_API_ID &&
            p_cspl_hdr->api <= SONANR_REQ_MAX_API)
            {
                anr_create_send_generic_nack(p_msg, p_cspl_hdr,
                        SON_ERR_CELL_UNCONFIGURED);
            }

            ret  = RELEASE_SON_BUFFER;
        }
        else
        {
            ret = anr_cell_process_msg(p_cspl_hdr, event_id, p_cell_ctxt, p_msg);
        }
    }

	SON_UT_TRACE_EXIT();
    return ret;
} /* anr_process_smif_msgs */

/*****************************************************************************
 * Function Name  : anr_process_rrm_msgs
 * Inputs         : p_cspl_hdr - Pointer to the the CSPL header information
 * Inputs         : p_msg      - Pointer to the message received
*                                 : p_cell_id  - Pointer to cell id
*                                 : event_id   - event id
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This function handle messages from RRM module.
 *****************************************************************************/
static son_buf_retain_status_et
anr_process_rrm_msgs
(
STACKAPIHDR     *p_cspl_hdr,
son_u8  *p_msg,
son_intra_rat_global_cell_id_t  *p_cell_id,
son_anr_index_et event_id
 )
{
    anr_cell_context_t                  *p_cell_ctxt      = SON_PNULL;
    son_buf_retain_status_et ret               = RELEASE_SON_BUFFER;


    SON_UT_TRACE_ENTER();

    /* Rel 3.0 Cell delete support changes Start */
    if (RRM_SON_DEREGISTER_RESP == p_cspl_hdr->api)
    {
        son_rrm_deregistration_res_t *p_res =
            (son_rrm_deregistration_res_t *)p_msg;

        /* Find the transaction Id received in De-register response in 
           temporary transaction Id list stored during cell deletion.
           If found remove it from temporary transaction Id list and 
           do not process the corresponding De-register response as the
           cell is already deleted */
        if (SON_SUCCESS == 
                son_find_txn_id_in_temp_txn_id_list(p_res->transaction_id))
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_BRIEF,
                    "RRM_SON_DEREGISTER_RESP "
                    "received for the cell [Cell Id: 0x%x] already deleted "
                    "at ANR due to cell delete request from OAM so ignore it.",
                    son_convert_char_cell_id_to_int_cell_id(
                        p_res->res.deregistration_resp.id.object_id.
                        cell_id.cell_identity));
            SON_UT_TRACE_EXIT();
            return RELEASE_SON_BUFFER;
        }

    }
    /* Rel 3.0 Cell delete support changes Stop */

    /*Call Wrapper function to unpack it*/

    p_cell_ctxt = anr_get_cell_context_from_global_context(p_cell_id);

    if (!p_cell_ctxt)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
                "anr_process_rrm_msgs: Cell context not found!\n");
    }
    else
    {
        ret = anr_cell_process_msg(p_cspl_hdr, event_id, p_cell_ctxt, p_msg);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_process_rrm_msgs */

/*****************************************************************************
 * Function Name  : anr_process_cell_specific_x2_msgs
 * Inputs         : p_cspl_hdr - Pointer to the the CSPL header information
 *                : p_msg        Pointer to the message received
*                                 : p_cell_id    Pointer to cell id
*                                 : event_id     event id
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This function handle messages from X2 module.
 *****************************************************************************/
static son_buf_retain_status_et
anr_process_cell_specific_x2_msgs
(
 STACKAPIHDR     *p_cspl_hdr,
 son_u8  *p_msg,
 son_intra_rat_global_cell_id_t  *p_cell_id,
 son_anr_index_et event_id
)
{
    anr_cell_context_t			*p_cell_ctxt      = SON_PNULL;
    son_buf_retain_status_et 	ret         	  = RELEASE_SON_BUFFER;


    SON_UT_TRACE_ENTER();

    /*Call Wrapper function to unpack it*/

    p_cell_ctxt = anr_get_cell_context_from_global_context(p_cell_id);

    if (!p_cell_ctxt)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
                "anr_process_cell_specific_x2_msgs: Cell context not found!\n");
    }
    else
    {
        ret = anr_cell_process_msg(p_cspl_hdr, event_id, p_cell_ctxt, p_msg);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* anr_process_cell_specific_x2_msgs */

/*****************************************************************************
 * Function Name  : anr_shutdown_handler
 * Inputs         : p_msg       - Pointer to the message received
*                                       p_cspl_info - Pointer to the the CSPL header information
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
*                   RELEASE_SON_BUFFER - Encapsulating shell should release
*                   the buffer or
*                   RETAIN_SON_BUFFER - Encapsulating shell should retain
*                   the buffer
 * Description    : This is the message handler for ANR module and is
*                   called when SMIF_SHUTDOWN_REQ message is received.
*****************************************************************************/
son_buf_retain_status_et
anr_shutdown_handler
(
 son_u8 *p_msg
            /*SPR 17777 +-*/ 
 )
{
    anr_cell_context_node_t *p_ctxt_node        	= SON_PNULL;
    smif_shutdown_res_t shut_down_res 		= {0};


    SON_UT_TRACE_ENTER();

    p_ctxt_node = anr_get_first_cell_ctxt();
    while (SON_PNULL != p_ctxt_node)
    {
        if (ANR_CELL_STATE_ENABLING == 
                p_ctxt_node->data.current_cell_fsm_state ||
                ANR_CELL_STATE_ENABLED == 
                p_ctxt_node->data.current_cell_fsm_state)
        {
            if (SON_FAILURE == anr_construct_send_rrm_deregister_req(
                        *(son_u16 *)(p_msg),
                        &p_ctxt_node->data.cell_id))
            {
                SON_LOG(anr_get_log_mode(),
                        p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_shutdown_handler: "
                        "anr_construct_send_rrm_deregister_req failed "
                        "for cell [Cell Id: 0x%x]\n",
                        son_convert_char_cell_id_to_int_cell_id(p_ctxt_node->data.cell_id.cell_identity));
            }
        }

        p_ctxt_node = anr_get_next_node(p_ctxt_node);
    }

           /*Clean up memory of each Cell Context */
       anr_clear_context_list();

        /* Clean up X2 Pending Response list */
        anr_clear_x2_pending_resp_list();

        /* Clean up ENABLE/DISABLE Pending Response list */
        anr_clear_pending_res_list();

        /*SPR-9574-Start*/
        /*Clean up Inter and Intra frequency lists*/
        anr_clear_intra_and_inter_freq_list();
        /*SPR-9574-End*/

        /* Stop ANR Timer */
        anr_stop_timer();

    anr_set_context_state(ANR_STATE_SHUTDOWN);
    anr_clear_db();

    /* Send SMIF_SHUTDOWN_RES to SONMgmtIfH*/
    shut_down_res.transaction_id = *(son_u16 *)(p_msg);
    shut_down_res.result         = SON_SUCCESS;
    shut_down_res.error_code     = SON_NO_ERROR;

    /* Coverity Fix: 19171 */
    son_create_send_buffer((son_u8 *)&shut_down_res, SON_ANR_MODULE_ID,
            SON_MIF_MODULE_ID,
        SMIF_SHUTDOWN_RES, sizeof(shut_down_res));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_shutdown_handler */

/*****************************************************************************
 * Function Name  : anr_init_state_handler
 * Inputs         : p_msg       - Pointer to the message received
 *                : p_cspl_hdr  - Pointer to the the CSPL header information.
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
 *                  RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ANR module and is called for
 *                  any message received in INIT State
 ******************************************************************************/
son_buf_retain_status_et
anr_init_state_handler
(
 son_u8 *p_msg,
 STACKAPIHDR *p_cspl_hdr
 )
{
    smif_init_config_res_t            init_config_res = {0};
    son_u8                            length  = 0;
    son_procedure_code_et             api     = SMIF_GENERIC_RES;
   son_buf_retain_status_et 	ret 			= RETAIN_SON_BUFFER;


	SON_UT_TRACE_ENTER();

    if (SMIF_INIT_CONFIG_REQ == (son_procedure_code_et)p_cspl_hdr->api)
    {
        /*Call log level handler*/
        ret = anr_log_handler(p_msg, p_cspl_hdr->api);

        anr_set_context_state(ANR_STATE_ACTIVE);

        api     								= SMIF_INIT_CONFIG_RES;
        init_config_res.transaction_id = *((son_u16 *)p_msg);
        init_config_res.result = SON_SUCCESS;
        init_config_res.error_code = SON_NO_ERROR;
        length = sizeof(smif_init_config_res_t);

        /* Coverity Fix: 19163 */
        son_create_send_buffer((son_u8 *)&init_config_res,
                                SON_ANR_MODULE_ID,
                                SON_MIF_MODULE_ID, api,
                                length );
    }
    else
    {
        if (SONX2_ADD_ENB_RES == p_cspl_hdr->api)
        {
        	x2_clear_eNB_rec_list(&((son_x2_add_enb_res_t *)p_msg)->enb_list);
        }

        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_ERROR,
                "Unexpected Message [%d] "
                "Received from module [%d]",
            p_cspl_hdr->api, p_cspl_hdr->from);

        if (((son_procedure_code_et)p_cspl_hdr->api >= SONANR_REQ_BASE_API_ID &&
                    (son_procedure_code_et) p_cspl_hdr->api <= SONANR_REQ_MAX_API) ||    
                ((son_anr_rrm_message_et)p_cspl_hdr->api >= SON_ANR_RRM_REQ_API_BASE &&  
                 (son_anr_rrm_message_et)p_cspl_hdr->api < SON_ANR_RRM_MAX_REQ_API))
        {
            anr_create_send_generic_nack(p_msg, p_cspl_hdr,
                    SON_ERR_UNEXPECTED_MSG);
        }
        ret = RELEASE_SON_BUFFER;
    }

	SON_UT_TRACE_EXIT();
    return ret;
} /* anr_init_state_handler */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_START */

/*****************************************************************************
 * Function Name  : anr_parse_rrm_messages
 * Inputs         : p_msg       - Pointer to the message received
 *                : api_id      - api_id
 * Outputs        : None
 * Returns        : son_u8*    -Pointer to the RRM message after parsing.
 *                              Address of the first byte is returned.
 * Description    : This is function to parse the messages received from RRM
 ******************************************************************************/
static son_u8 *
anr_parse_rrm_messages
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
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                        "RRM->ANR:RRM_SON_REGISTER_RESP");
                p_parsed_msg = son_mem_get(sizeof(son_rrm_registration_res_t));
                if (SON_PNULL != p_parsed_msg &&
                        RRM_FAILURE == rrm_parse_rrm_son_registration_res(
                            &((son_rrm_registration_res_t *)p_parsed_msg)->res,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                            &length_read))
                {
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                    txn_id_available = SON_FALSE;
                }

                break;
            }

        case RRM_SON_DEREGISTER_RESP:
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                        "RRM->ANR:RRM_SON_DEREGISTER_RESP");
                p_parsed_msg = son_mem_get(sizeof(son_rrm_deregistration_res_t));
                if (SON_PNULL != p_parsed_msg &&
                        RRM_FAILURE == rrm_parse_rrm_son_deregistration_res(
                            &((son_rrm_deregistration_res_t *)p_parsed_msg)->res,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                            &length_read))
                {
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                    txn_id_available = SON_FALSE;
                }

                break;
            }

        case RRM_SON_CELL_STATE_CHANGE_IND:
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                        "RRM->ANR:RRM_SON_CELL_STATE_CHANGE_IND");
                txn_id_available = SON_FALSE;
                p_parsed_msg = son_mem_get(
                        sizeof(son_cell_state_change_indication_t));
                if (SON_PNULL != p_parsed_msg &&
                        RRM_FAILURE == rrm_parse_rrm_son_cell_state_change_ind(
                            (son_cell_state_change_indication_t *)p_parsed_msg,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                            &length_read))
                {
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                }

                break;
            }

        case RRM_SON_MEAS_CONFIG_RESP:
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                        "RRM->ANR:RRM_SON_MEAS_CONFIG_RESP");
                p_parsed_msg = son_mem_get(sizeof(son_rrm_meas_config_res_t));
                if (SON_PNULL != p_parsed_msg && 
                        RRM_FAILURE == rrm_parse_rrm_son_meas_config_res(
                            &((son_rrm_meas_config_res_t *)p_parsed_msg)->
                            config_res,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                            &length_read))
                {
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                    txn_id_available = SON_FALSE;
                }

                break;
            }

        case RRM_SON_SET_ATTRIBUTE_RESP:
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                        "RRM->ANR:RRM_SON_SET_ATTRIBUTE_RESP");
                p_parsed_msg = son_mem_get(sizeof(son_rrm_set_attribute_res_t));
                if (SON_PNULL != p_parsed_msg &&
                        RRM_FAILURE == rrm_parse_rrm_son_set_attr_res(
                            &((son_rrm_set_attribute_res_t *)p_parsed_msg)->
                            res,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                            &length_read))
                {
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                    txn_id_available = SON_FALSE;
                }

                break;
            }

        case RRM_SON_MEAS_RESULTS_IND:
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                        "RRM->ANR:RRM_SON_MEAS_RESULTS_IND");
                txn_id_available = SON_FALSE;
                p_parsed_msg = son_mem_get(sizeof(son_rrm_meas_results_ind_t));
                if (SON_PNULL != p_parsed_msg && 
                        RRM_FAILURE == rrm_parse_rrm_son_meas_results_ind(
                            (son_rrm_meas_results_ind_t *)
                            p_parsed_msg,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                            &length_read))
                {
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                }

                break;
            }

        case RRM_SON_TNL_DISCOVERY_RESP:
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                        "RRM->ANR:RRM_SON_TNL_DISCOVERY_RESP");
                p_parsed_msg = son_mem_get(sizeof(son_rrm_tnl_discovery_res_t));
                if (SON_PNULL != p_parsed_msg &&
                        RRM_FAILURE == rrm_parse_rrm_son_tnl_discovery_res(
                            &((son_rrm_tnl_discovery_res_t *)p_parsed_msg)->
                            tnl_res,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                            &length_read))
                {
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                    txn_id_available = SON_FALSE;
                }

                break;
            }

        case RRM_SON_HO_REPORT:
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                        "RRM->ANR:RRM_SON_HO_REPORT");
                txn_id_available = SON_FALSE;
                p_parsed_msg = son_mem_get(sizeof(son_rrm_ho_report_t));
                if (SON_PNULL != p_parsed_msg &&
                        RRM_FAILURE == rrm_parse_rrm_son_ho_report(
                            (son_rrm_ho_report_t *)p_parsed_msg,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                            &length_read))
                {
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                }

                break;
            }

        case RRM_SON_HO_ATTEMPT_IND:
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                        "RRM->ANR:RRM_SON_HO_ATTEMPT_IND");
                txn_id_available = SON_FALSE;
                p_parsed_msg = son_mem_get(sizeof(son_rrm_ho_attempt_ind_t));
                if (SON_PNULL != p_parsed_msg &&
                        RRM_FAILURE == rrm_parse_rrm_son_ho_attempt_ind(
                            (son_rrm_ho_attempt_ind_t *)p_parsed_msg,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE, &length_read))
                {
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                }

                break;
            }

        case RRM_SON_HO_FAILURE_REPORT:
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                        "RRM->ANR:RRM_SON_HO_FAILURE_REPORT");
                txn_id_available = SON_FALSE;
                p_parsed_msg = son_mem_get(sizeof(son_ho_failure_report_t));
                if (SON_PNULL != p_parsed_msg &&
                        RRM_FAILURE ==  rrm_parse_rrm_son_ho_failure_report(
                            (son_ho_failure_report_t *)p_parsed_msg,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE, &length_read))
                {
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                }

                break;
            }

        case RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP:
            {
                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name, SON_INFO,
                        "RRM->ANR:RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP");
                p_parsed_msg = son_mem_get(sizeof(son_rrm_meas_config_obj_remove_res_t));
                if (SON_PNULL != p_parsed_msg &&
                        RRM_FAILURE == rrm_parse_rrm_son_meas_config_obj_remove_res(
                            &((son_rrm_meas_config_obj_remove_res_t *)p_parsed_msg)->
                            remove_res,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                            &length_read) )
                {
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                    txn_id_available = SON_FALSE;
                }
                break;
            }

#ifdef LTE_EMBMS_SUPPORTED
        case RRM_SON_MBMS_SF_INFO_IND:
            {
                p_parsed_msg = son_mem_get(sizeof(rrm_son_mbms_sf_info_ind_t));
                if (SON_PNULL != p_parsed_msg &&
                        RRM_FAILURE == rrm_parse_rrm_son_mbms_sf_info_ind (
                            ((rrm_son_mbms_sf_info_ind_t *)p_parsed_msg)
                            ,
                            (p_msg + EXT_MSG_API_HDR_SIZE),
                            buf_len - EXT_MSG_API_HDR_SIZE,
                            &length_read) )
                {
                    son_mem_free(p_parsed_msg);
                    p_parsed_msg = SON_PNULL;
                    txn_id_available = SON_FALSE;
                }
                break;
            }
#endif
        default:
            {
                SON_LOG(anr_get_log_mode(),
                        p_son_anr_facility_name, SON_ERROR,
                        "Unexpected Message [%d]"
                        "received from RRM\n", api_id);
                break;
            }
    } /* switch */

#ifdef LTE_EMBMS_SUPPORTED
    if (SON_PNULL != p_parsed_msg && SON_TRUE == txn_id_available && 
        (api_id != RRM_SON_MBMS_SF_INFO_IND))
#else
    if (SON_PNULL != p_parsed_msg && SON_TRUE == txn_id_available)
#endif
    {
        *(son_u16 *)p_parsed_msg = son_get_word_from_header(
                &p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
    }

    return p_parsed_msg;
} /* anr_parse_rrm_messages */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_STOP */

/* Rel 3.0 Cell delete support changes Start */
/*****************************************************************************
 * Function Name  : anr_cell_delete_req_handler
 * Inputs         : p_cspl_hdr   Pointer to the CSPL Header
 *                : p_msg        Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for ANR module and is
 *                  invoked when SMIF_CELL_DELETE_REQ message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_cell_delete_req_handler
(
            /*SPR 17777 +-*/ 
 son_u8 *p_buf
 )
{
    smif_cell_delete_req_t   *p_msg          = (smif_cell_delete_req_t*)p_buf;
    anr_cell_context_t      *p_cell_ctxt    = SON_PNULL;
    son_u8                  idx             = 0;
    son_u8 pend_res_cnt                     = 0;
    son_x2_nrt_update_ind_t nrt_upd_ind;
    son_u16 txn_id =  son_generate_txn_id();
    smif_cell_delete_resp_t  cell_del_res;
    cell_status_list_data_t * p_cell_status_list_node = SON_PNULL;
    son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&nrt_upd_ind, 0, sizeof(nrt_upd_ind));
    SON_MEMSET(&cell_del_res, 0, sizeof(cell_del_res));

    /* Populate the response structure */
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
        p_cell_ctxt = anr_get_cell_context_from_global_context(
                &p_msg->cell_delete_list[idx]);

        if (SON_PNULL != p_cell_ctxt)
        {
            /* Send deregister request to RRM if cell is ENABLING/ENABLED */
            if ((ANR_CELL_STATE_ENABLING == 
                        p_cell_ctxt->current_cell_fsm_state) ||
                    (ANR_CELL_STATE_ENABLED == 
                     p_cell_ctxt->current_cell_fsm_state))
            {
                /* Start Coverity Fix: 60108 */
                if (SON_FAILURE == anr_construct_send_rrm_deregister_req(
                            txn_id,
                            &p_cell_ctxt->cell_id ))
                {
                    cell_del_res.cell_delete_status_list[idx].result = 
                        SON_PARTIAL_SUCCESS;
                    SON_LOG(anr_get_log_mode(),
                            p_son_anr_facility_name,
                            SON_ERROR,
                            "anr_cell_delete_req_handler: "
                            "anr_construct_send_rrm_deregister_req failed "
                            "for cell [Cell Id: 0x%x]\n",
                            son_convert_char_cell_id_to_int_cell_id(
                                p_cell_ctxt->cell_id.cell_identity));
                }
                /* End Coverity Fix: 60108 */
                else
                {
                    /* Store the transaction Id with which De-register
                       request is sent to RRM. This shall be used later when
                       De-register response is received for this request */
                    son_insert_txn_id_in_temp_txn_id_list(txn_id);
                }
            }
            /* Check the pending resp list for this cell */
            if (anr_get_pending_res_expected_count())
            {
                p_cell_status_list_node = 
                    anr_find_cell_in_pending_res(&p_msg->cell_delete_list[idx]);
                if (SON_PNULL != p_cell_status_list_node)
                {
                    /* Error code SON_ERR_TIMER_EXPIRED indicates that
                       the pending response is not received for this cell 
                       so decrement expected response count since the 
                       cell is being deleted and no response should be
                       expected for this cell. */
                    if (SON_ERR_TIMER_EXPIRED == 
                            p_cell_status_list_node->cell_status.error_code)
                    {
                        anr_decrement_pending_res_expected_count();
                    }
                    api = anr_get_pending_res_api_id();
                    if (SMIF_CELL_INFO_IND == api)
                    {
                        /* Delete the cell as in case of cell deletion
                           no feature state change indication shall be send
                           and only cell delete response shall be send to
                           SMIF */
                        list_delete_node(&g_anr_pending_response.cell_sts_list,
                                &p_cell_status_list_node->cell_status_node);
                        son_mem_free(p_cell_status_list_node);
                        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                                SON_BRIEF,
                                "Deleted Cell %x from Pending Response List\n",
                                son_convert_char_cell_id_to_int_cell_id(
                                p_msg->cell_delete_list[idx].cell_identity));
                    }
                    else
                    {
                        /* Update Cell in ANR Pending Response List with error 
                           code SON_ERR_CELL_UNCONFIGURED */
                        p_cell_status_list_node->cell_status.error_code = 
                            SON_ERR_CELL_UNCONFIGURED;
                    }
                }

                if (!(pend_res_cnt = anr_get_pending_res_expected_count()) ||
                        ( pend_res_cnt &&
                          anr_get_pending_res_ts_diff_from_curr_ts() >
                          ANR_TIMEOUT_FOR_PENDING_RES))
                {
                    anr_pending_res_timer_handler();
                    anr_reset_pending_res();
                    anr_set_context_state(ANR_STATE_ACTIVE);
                }
            }

            /* Delete the cell from context */ 
            if (SON_TRUE == anr_delete_cell_from_context
                    (&p_msg->cell_delete_list[idx]))
            {
                if (SON_PARTIAL_SUCCESS != 
                        cell_del_res.cell_delete_status_list[idx].result)
                {
                    cell_del_res.cell_delete_status_list[idx].result = 
                        SON_SUCCESS;
                }

                /* Rel 3.0 Multi Sector code changes Start */
                anr_delete_serving_cell_as_nbr_autonomously
                    (&p_msg->cell_delete_list[idx]);
                /* Rel 3.0 Multi Sector code changes Stop */

                /* Send x2 nrt update indicaion */
                anr_populate_send_x2_nrt_update_ind(&nrt_upd_ind,
                        &p_cell_ctxt->cell_id, SON_NRT_OP_DELETE);

                SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                        SON_BRIEF,
                        "Cell [Cell Id: 0x%x] deleted successfully\n",
                        son_convert_char_cell_id_to_int_cell_id
                        (p_msg->cell_delete_list[idx].cell_identity));
            }
        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_ERROR,
                    "anr_cell_delete_req_handler: "
                    "Cell [Cell Id: 0x%x] does not exists\n",
                    son_convert_char_cell_id_to_int_cell_id
                    (p_msg->cell_delete_list[idx].cell_identity));
        }
    }

    /* Send cell delete resp to mif */
    son_create_send_buffer((son_u8 *)&cell_del_res,
            SON_ANR_MODULE_ID,
            SON_MIF_MODULE_ID,
            SMIF_CELL_DELETE_RESP,
            sizeof(cell_del_res));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}

/* Rel 3.0 Cell delete support changes Stop */

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : anr_get_log_level_req_handler
 * Inputs         : p_cspl_hdr   Pointer to the CSPL Header
 *                : p_buf        Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for ANR module and is
 *                  invoked when SMIF_GET_LOG_LEVEL_REQ message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
anr_get_log_level_req_handler
(
            /*SPR 17777 +-*/ 
 son_u8 *p_buf
 )
{
    smif_get_log_level_req_t *p_msg = (smif_get_log_level_req_t*)p_buf;
    smif_get_log_level_res_t res;
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&res, 0, sizeof(res));
    res.transaction_id = p_msg->transaction_id;
    res.log_level = anr_get_log_level();
    
    /* Send cell delete resp to mif */
    son_create_send_buffer((son_u8 *)&res,
            SON_ANR_MODULE_ID,
            SON_MIF_MODULE_ID,
            SMIF_GET_LOG_LEVEL_RESP,
            sizeof(res));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}
/* SPR-13251 Fix Ends */
/* eICIC feature related code starts */
/*****************************************************************************
 * Function Name  : anr_neighbor_default_config_req_handler
 * Inputs         : p_cspl_hdr   Pointer to the CSPL Header
 *                : p_buf        Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for for ANR default neighbor 
 *                  config request. This API shall send the default value of 
 *                  certain algorithm parameter with default value for neighbor
 *****************************************************************************/
static son_buf_retain_status_et
anr_neighbor_default_config_req_handler
(
            /*SPR 17777 +-*/ 
    son_u8      *p_buf
)
{
    son_anr_neighbor_default_config_req_t *p_msg = (son_anr_neighbor_default_config_req_t *)p_buf;
    smif_neighbor_default_config_res_t  res;
    SON_UT_TRACE_ENTER();
    SON_MEMSET(&res, 0, sizeof(smif_neighbor_default_config_res_t));
    if ( p_msg->son_anr_default_req.bitmask & SON_OAM_ANR_CSG_DEF_SCHEME_PRESENT )
    {
        if (SON_ACTIVATE_ICIC == p_msg->son_anr_default_req.csg_cell_intf_scheme)
        {
            anr_set_intf_csg_scheme(p_msg->son_anr_default_req.csg_cell_intf_scheme);
        }
        else if ( SON_ACTIVATE_EICIC == p_msg->son_anr_default_req.csg_cell_intf_scheme )
        {
            anr_set_intf_csg_scheme(p_msg->son_anr_default_req.csg_cell_intf_scheme);
        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_DETAILED,
                    "OAM configured default scheme commoting in global context");
            anr_set_intf_csg_scheme(SON_NO_INTF_SCHEME);
        }
    }
    if ( p_msg->son_anr_default_req.bitmask & SON_OAM_ANR_PICO_DEF_SCHEME_PRESENT )
    {
        if (SON_ACTIVATE_ICIC == p_msg->son_anr_default_req.pico_cell_intf_scheme)
        {
            anr_set_intf_pico_scheme(p_msg->son_anr_default_req.pico_cell_intf_scheme);
        }
        else if ( SON_ACTIVATE_EICIC == p_msg->son_anr_default_req.pico_cell_intf_scheme )
        {
            anr_set_intf_pico_scheme(p_msg->son_anr_default_req.pico_cell_intf_scheme);
        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_DETAILED,
                    "OAM configured default scheme commoting in global context");
            anr_set_intf_pico_scheme(SON_NO_INTF_SCHEME);
        }
    }


    if ( p_msg->son_anr_default_req.bitmask & SON_OAM_ANR_MACRO_DEF_SCHEME_PRESENT)
    {
        if (SON_ACTIVATE_ICIC == p_msg->son_anr_default_req.macro_cell_intf_scheme)
        {
            anr_set_intf_macro_scheme(p_msg->son_anr_default_req.macro_cell_intf_scheme);
        }
        else if ( SON_ACTIVATE_EICIC == p_msg->son_anr_default_req.macro_cell_intf_scheme )
        {
            anr_set_intf_macro_scheme(p_msg->son_anr_default_req.macro_cell_intf_scheme);
        }
        else
        {
            SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                    SON_DETAILED,
                    "OAM configured default scheme commoting in global context");
            anr_set_intf_macro_scheme(SON_NO_INTF_SCHEME);
        }
    }
    res.transaction_id = p_msg->transaction_id;
    res.default_resp.result = SON_SUCCESS;
    res.default_resp.error_code = SON_NO_ERROR;
    /* Send anr default config res to MIF*/
    son_create_send_buffer((son_u8 *)&res,
            SON_ANR_MODULE_ID,
            SON_MIF_MODULE_ID,
            SONANR_NEIGHBOR_DEFAULT_CONFIG_RESP,
            sizeof(res));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}
/* eICIC feature related code ends */

/* Rel 3.0 Multi Sector code changes Start */
/*****************************************************************************
 * Function Name  : anr_cell_switch_on_off_ind_handler
 * Inputs         : p_cspl_hdr  - Pointer to the CSPL Header Information
 *                : p_msg       - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
 *                   RELEASE_SON_BUFFER - Encapsulating shell should release
 *                   the buffer or
 *                   RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when SONANR_CELL_SWITCH_ON_OFF_IND message is
 *                  received from SON ES.
 *****************************************************************************/
static son_buf_retain_status_et
anr_cell_switch_on_off_ind_handler
(
            /*SPR 17777 +-*/ 
 son_u8 *p_msg
 )
{
    sonanr_cell_switch_on_off_ind_t *p_ind =
        (sonanr_cell_switch_on_off_ind_t *)p_msg;
    SON_UT_TRACE_ENTER();

    if (ES_CELL_SWITCHED_OFF == p_ind->cell_on_off_status)
    {
        anr_delete_serving_cell_as_nbr_autonomously(&p_ind->src_cgi);
    }
    else 
    {
        anr_add_serving_cell_as_nbr_autonomously(&p_ind->src_cgi);
    }
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* anr_cell_switch_on_off_ind_handler */
/* Rel 3.0 Multi Sector code changes Stop */

/*****************************************************************************
 * Function Name  : anr_active_state_handler
 * Inputs         : p_msg       - Pointer to the message received
 *                                : p_cspl_info - Pointer to the the CSPL header information
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
 *                  RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ANR module and is
 *                  called for any message received in state
 *                  other than INIT and SHUTDOWN.
 ******************************************************************************/
son_buf_retain_status_et
anr_active_state_handler
(
 son_u8 *p_msg,
 STACKAPIHDR *p_cspl_info
 )
{
   son_buf_retain_status_et 	ret 			= RETAIN_SON_BUFFER;
   smif_init_config_res_t  init_config_res = {0};
   son_u8                  *p_buf          = SON_PNULL;
   son_u8                  *p_parsed_msg   = SON_PNULL;
   son_anr_index_et				event_id		= IDX_NOT_AVAILABLE;
   son_intra_rat_global_cell_id_t 	*p_cell_id 		= SON_PNULL;
   son_oam_log_on_off_et        log_mode        = anr_get_log_mode();
   son_u8                  length   = 0;
   /* Coverity Fix: 19177 */
   son_procedure_code_et   api      = SON_PROCEDURE_UNDEFINED;
   /* Coverity Fix: 16606 */
   anr_fsm_state_et        curr_fsm_state = anr_get_context_state();


   SON_UT_TRACE_ENTER();

    /* Handle Global ANR Module State Related Messages */

    if (SON_MIF_MODULE_ID == p_cspl_info->from)
   {
        switch (p_cspl_info->api)
       {
           case SMIF_CELL_INFO_IND:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "MIF->ANR:SMIF_CELL_INFO_IND");
                ret  =  anr_cell_info_ind_handler(p_cspl_info, p_msg);
                break;
            }

            case SMIF_LOG_ENABLE_DISABLE_CMD:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "MIF->ANR:SMIF_LOG_ENABLE_DISABLE_CMD");
                ret  =  anr_log_handler(p_msg, p_cspl_info->api);
                break;
            }
            case SMIF_SET_LOG_LEVEL_CMD:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "MIF->ANR:SMIF_SET_LOG_LEVEL_CMD");
                ret  =  anr_log_handler(p_msg, p_cspl_info->api);
                break;
            }

            case SMIF_INIT_CONFIG_REQ:
            {
                /* Coverity Fix: 16606 */
                SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
                           "anr_active_state_handler: Unexpected Message "
                           "SMIF_INIT_CONFIG_REQ Received from SMIF in %s\n",
                            SON_ANR_FSM_STATES_NAMES[curr_fsm_state]);
                   api    = SMIF_INIT_CONFIG_RES;
                init_config_res.transaction_id = *((son_u16 *)p_msg);
                   init_config_res.result = SON_FAILURE;
                   init_config_res.error_code = SON_ERR_UNEXPECTED_MSG;
                   length = sizeof(smif_init_config_res_t);
                   p_buf = (son_u8 *)&init_config_res;
                   ret = RELEASE_SON_BUFFER;
                break;
            }

            case SONANR_NR_SCAN_REQ:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "MIF->ANR:SONANR_NR_SCAN_REQ");
                ret = anr_scan_req_handler(p_cspl_info, p_msg);
                break;
            }

            case SONANR_ENABLE_DISABLE_X2_REQ:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "MIF->ANR:SONANR_ENABLE_DISABLE_X2_REQ");
            /*SPR 17777 +-*/ 
                ret = anr_enable_disable_x2_req_handler(p_msg);
                break;
            }

            case SONANR_NR_ENB_ADD_REQ:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "MIF->ANR:SONANR_NR_ENB_ADD_REQ");
                ret = anr_nr_enb_add_req_handler(p_cspl_info, p_msg);
                break;
            }

            case SONANR_NR_ENB_UPDATE_REQ:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "MIF->ANR:SONANR_NR_ENB_UPDATE_REQ");
                ret = anr_nr_enb_update_req_handler(p_cspl_info, p_msg);
                break;
            }

            case SONANR_NR_ENB_DELETE_REQ:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "MIF->ANR:SONANR_NR_ENB_DELETE_REQ");
                ret = anr_nr_enb_delete_req_handler(p_cspl_info, p_msg);
                break;
            }

            case SONANR_NR_ENB_RETRIEVE_REQ:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "MIF->ANR:SONANR_NR_ENB_RETRIEVE_REQ");
                /*SPR 17777 +-*/ 
                ret = anr_nr_enb_retrieve_req_handler(p_msg);
                break;
            }

            /* Rel 3.0 Cell delete support changes Start */
            case SMIF_CELL_DELETE_REQ:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "MIF->ANR:SMIF_CELL_DELETE_REQ");
                ret = anr_cell_delete_req_handler(p_msg);
                break;
            }
            /* Rel 3.0 Cell delete support changes Stop */          

            /* SPR-13251 Fix Starts */
            case SMIF_GET_LOG_LEVEL_REQ:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "MIF->ANR:SMIF_GET_LOG_LEVEL_REQ");
                ret = anr_get_log_level_req_handler(p_msg);
                break;
            }
            /* SPR-13251 Fix Ends */

            /* eICIC feature related code starts */
            case SONANR_NEIGHBOR_DEFAULT_CONFIG_REQ:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "MIF->ANR:SONANR_NEIGHBOR_DEFAULT_CONFIG_REQ");
                ret = anr_neighbor_default_config_req_handler(p_msg);
                /*SPR 17777 +-*/ 
                break;
            }
            /* eICIC feature related code Ends */

            default:
            {
                /* Intentionally left empty
                 * Other SMIF messages are processed below */
                break;
            }
        } /* switch */
    }
    else if (SON_ES_MODULE_ID == p_cspl_info->from)
    {
        switch (p_cspl_info->api)
	  {
		  case  SONES_CELL_SWITCH_ON_FOR_X2_SETUP_RES:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "ES->ANR:SONES_CELL_SWITCH_ON_FOR_X2_SETUP_RES");
                /*SPR 17777 +-*/ 
                ret = anr_cell_switch_on_for_x2_setup_res_handler( p_msg);
                break;
            }
            /* Rel 3.0 Multi Sector code changes Start */
          case SONANR_CELL_SWITCH_ON_OFF_IND:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
                        "ES->ANR:SONANR_CELL_SWITCH_ON_OFF_IND");
                ret = anr_cell_switch_on_off_ind_handler(p_msg);
                /*SPR 17777 +-*/ 
                break;
            }
            /* Rel 3.0 Multi Sector code changes Stop */
          default:
            {
                SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
	                   "Unexpected Message [%d] Received from "
                    "entity : [%d]\n", p_cspl_info->api, p_cspl_info->from);
	           ret = RELEASE_SON_BUFFER;
			  break;
	  }
   }
    }
    else if (SON_X2_MODULE_ID == p_cspl_info->from)
    {
        if (SONX2AP_HO_REPORT_IND != p_cspl_info->api)
        {
            ret = anr_process_x2_msgs(p_cspl_info, p_msg);
       }
   }
   else if (SON_NMM_MODULE_ID == p_cspl_info->from)
   {
       SON_LOG(log_mode, p_son_anr_facility_name, SON_INFO,
               "X2->ANR:SONX2_ENB_CONFIG_UPDATE_IND");
       ret = anr_process_nmm_msgs(p_msg,
               p_cspl_info->api);
   }
   else if (SON_APPS_MODULE_ID == p_cspl_info->from)
   {
        ret = anr_process_apps_msgs(p_msg, p_cspl_info->api);
   }
   else if (SON_RRM_MODULE_ID == p_cspl_info->from)
   {
        /* Coverity Fix: 17273 */
        if (RRM_SON_TNL_DISCOVERY_RESP == (rrm_son_message_resp_et)p_cspl_info->api)
    	{
    		 /* Invoke Pup Tool */
			 p_parsed_msg = anr_parse_rrm_messages(p_msg,
												   p_cspl_info->api);

            if (SON_PNULL == p_parsed_msg)
			 {
				 SON_LOG(log_mode, p_son_anr_facility_name,
						 SON_ERROR,
						 "anr_active_state_handler: Parsing failed for received"
						 " message: [%d] \n", p_cspl_info->api);
				 ret = RELEASE_SON_BUFFER;
			 }
			 else
             {
                /*SPR 17777 +-*/ 
                ret = anr_tnl_discovery_res_handler(p_parsed_msg);
             }
    	}
#ifdef LTE_EMBMS_SUPPORTED
        if (RRM_SON_MBMS_SF_INFO_IND == (rrm_son_message_resp_et)p_cspl_info->api)
        {
            p_parsed_msg = anr_parse_rrm_messages(p_msg,
                    p_cspl_info->api);
            if (SON_PNULL == p_parsed_msg)
            {
                SON_LOG(log_mode, p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_active_state_handler: Parsing failed for received"
                        " message: %d \n", p_cspl_info->api);
                ret = RELEASE_SON_BUFFER;
            }
            else
            {
                ret = anr_mbms_sf_info_ind_handler(p_cspl_info, p_parsed_msg);
            }
        }
#endif
   }
   else
   {
        SON_LOG(log_mode, p_son_anr_facility_name, SON_ERROR,
               "anr_active_state_handler: Message [%d] Received from unknown "
            "entity : [%d]\n", p_cspl_info->api, p_cspl_info->from);
       ret = RELEASE_SON_BUFFER;
   }

    /* Handle Cell Specific State Machine Related Messages */
    if (RETAIN_SON_BUFFER == ret)
	{
            /* Coverity Fix: 19162 */
            event_id = anr_calculate_event_id(p_cspl_info->from,
                            (son_procedure_code_et)p_cspl_info->api);
            /* SPR 20653 Fix Start */
        if (event_id >= IDX_SONANR_MAX)
            /* SPR 20653 Fix End */
            {
                SON_LOG(log_mode, p_son_anr_facility_name,
                        SON_ERROR,
                        "anr_active_state_handler: Invalid event Id[%d] "
                        "for message [%d] received from [%d]\n",
                        event_id, p_cspl_info->api, p_cspl_info->from);
                SON_UT_TRACE_EXIT();
                return RELEASE_SON_BUFFER;
            }
                                       
        switch (p_cspl_info->from)
        {
            case SON_RRM_MODULE_ID:
                {
                    /* Invoke Pup Tool */
                    p_parsed_msg = anr_parse_rrm_messages(p_msg,
                            p_cspl_info->api);
                    if (SON_PNULL != p_parsed_msg)
                    {
                        p_cell_id = anr_get_cell_id_from_msg(p_cspl_info->from,
                                p_cspl_info->api,
                                p_parsed_msg);

                        ret = anr_process_rrm_msgs(p_cspl_info,
                                p_parsed_msg,
                                p_cell_id,
                                event_id);

                        son_mem_free(p_parsed_msg);
                    }
                    else
                    {
                        SON_LOG(log_mode, p_son_anr_facility_name,
                                SON_ERROR,
                                "Parsing failed for received"
                                " message: [%d] \n", p_cspl_info->api);
                        ret = RELEASE_SON_BUFFER;
                    }

                    break;
                }

            case SON_MIF_MODULE_ID:
                {
                    p_cell_id = anr_get_cell_id_from_msg(p_cspl_info->from,
                            p_cspl_info->api,
                            p_msg);

                    ret = anr_process_smif_msgs(p_cspl_info,
                            p_msg,
                            p_cell_id,
                            event_id);
                    break;
                }

            case SON_X2_MODULE_ID:
                {
                    p_cell_id = anr_get_cell_id_from_msg(p_cspl_info->from, p_cspl_info->api, p_msg);
                    ret = anr_process_cell_specific_x2_msgs(p_cspl_info, p_msg, p_cell_id, event_id);
                    break;
                }

            default:
                {
                    SON_LOG(log_mode, p_son_anr_facility_name,
                            SON_ERROR,
                            "Unexpected Message "
                            "[%d] Received from undefined module [%d]\n",
                            p_cspl_info->api, p_cspl_info->from);
                    ret = RELEASE_SON_BUFFER;
                    break;
                }
        } /* switch */
    }

    if (p_buf)
    {
        son_create_send_buffer(p_buf, SON_ANR_MODULE_ID,
                                (son_module_id_et)p_cspl_info->from, api, 
                                length );
    }
    
	SON_UT_TRACE_EXIT();
	return ret;
} /* anr_active_state_handler */
#ifdef LTE_EMBMS_SUPPORTED
son_void_t
anr_mbms_fill_mbsfn_info_in_cell_cntxt
(
 son_mbms_configuration_t           *p_cell_mbms_config,
 rrm_mbsfn_subframe_config_list_t   *p_new_mbms_config 
)
{
    son_u8 count = 0;
    SON_UT_TRACE_ENTER();

    p_cell_mbms_config->num_mbsfn = p_new_mbms_config->count;

    for( count = 0; count < p_cell_mbms_config->num_mbsfn;
            count++)
    {
        p_cell_mbms_config->mbsfn_info[count].radioframe_alloc_period =
            (rrm_frame_allocation_period_et) p_new_mbms_config->
            mbsfn_subframe_config_info[count].radio_frame_allocation_period;
        p_cell_mbms_config->mbsfn_info[count].radioframe_alloc_offset =
            p_new_mbms_config->mbsfn_subframe_config_info[count].
            radio_frame_allocation_offset;

            if( 0 == p_new_mbms_config->mbsfn_subframe_config_info[count].
                    subframe_allocation.sf_alloc_choice)
            {
                p_cell_mbms_config->mbsfn_info[count].
                    subframe_alloc.bitmask |= SON_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT ;

               SON_MEMCPY( p_cell_mbms_config->mbsfn_info[count].subframe_alloc.oneframe, 
                           p_new_mbms_config->mbsfn_subframe_config_info[count].
                            subframe_allocation.subframe_allocation,
                             SON_SUBFRAME_ALLOC_ONE_FRAME_OCTET_SIZE);
            }
            else  
            {
                p_cell_mbms_config->mbsfn_info[count].
                    subframe_alloc.bitmask |= SON_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT; 

               SON_MEMCPY( p_cell_mbms_config->mbsfn_info[count].subframe_alloc.fourframe, 
                           p_new_mbms_config->mbsfn_subframe_config_info[count].
                            subframe_allocation.subframe_allocation,
                             SON_SUBFRAME_ALLOC_FOUR_FRAME_OCTET_SIZE);
            }
    }

    SON_UT_TRACE_EXIT();
}

/*CID 97682:start */
/*****************************************************************************
 * Function Name  :anr_mbms_sf_info_ind_handler 
 * Inputs         : p_cspl_hdr  - Pointer to the CSPL Header Information
 *                : p_msg       - Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et -
 *                   RELEASE_SON_BUFFER - Encapsulating shell should release
 *                   the buffer or
 *                   RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                   the buffer
 * Description    : This is the message handler for ANR module and is
 *                  invoked when RRM_SON_MBMS_SF_INFO_IND message is
 *                  received.
 *****************************************************************************/
son_buf_retain_status_et
anr_mbms_sf_info_ind_handler
(
 STACKAPIHDR *p_cspl_hdr,
 son_u8 *p_msg
 )
{
    rrm_son_mbms_sf_info_ind_t *p_mbms_sf_info_ind = (rrm_son_mbms_sf_info_ind_t *)p_msg;
    anr_cell_context_t      *p_cell_ctxt        = SON_PNULL;
    anr_cell_context_node_t *p_cell_ctxt_node   = SON_PNULL;
    son_x2_nrt_update_ind_t        nrt_upd_ind         = {0};
    son_u8                        cell_count = 0;
    SON_UT_TRACE_ENTER();
    if (SON_PNULL == p_cspl_hdr || SON_PNULL == p_msg)
    {
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_ERROR,
                ":NULL CSPL header pointer or"
                "message pointer recieved");

        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER; 
    }

    SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
            SON_ERROR, "MBSF INFO COUNT After Parsing [%d]", 
            p_mbms_sf_info_ind->count);
    for(cell_count = 0; cell_count < p_mbms_sf_info_ind->count;
            cell_count++)
    {
        /*Pointer to the first context node*/
        p_cell_ctxt_node = anr_get_first_cell_ctxt();
        while (SON_PNULL != p_cell_ctxt_node)
        {
            /*Get the data part of the node*/
            p_cell_ctxt = &p_cell_ctxt_node->data;
            if( !SON_MEMCMP( &(p_cell_ctxt->cell_id),
                        &(p_mbms_sf_info_ind->cell_sib2_mbsfn_data[cell_count].cgi),
                        sizeof(son_intra_rat_global_cell_id_t)))
            {
                if(RRM_SON_MBMS_CONFIG_APPLY_PRESENT & 
                        p_mbms_sf_info_ind->cell_sib2_mbsfn_data[cell_count].bitmask)
                {
                    p_cell_ctxt->is_srv_mbms_available = SON_TRUE;  
                    anr_mbms_fill_mbsfn_info_in_cell_cntxt(&(p_cell_ctxt->mbms_config),
                            &(p_mbms_sf_info_ind->cell_sib2_mbsfn_data[cell_count].
                                rrm_mbsfn_subframe_config_list));
                }
                else
                {
                    p_cell_ctxt->is_srv_mbms_available = SON_FALSE;  
                }

                anr_populate_x2_nrt_update_ind(&nrt_upd_ind,
                        &p_cell_ctxt->cell_id, SON_NRT_OP_UPDATE);
                break;
            } 
            p_cell_ctxt_node = anr_get_next_node(p_cell_ctxt_node);
        }
    }
    if (nrt_upd_ind.num_srv_cells_list_size)
    {
        nrt_upd_ind.is_local_update = SON_FALSE;
        son_create_send_buffer((son_u8 *)&nrt_upd_ind, SON_ANR_MODULE_ID,
                SON_X2_MODULE_ID, SONX2_NRT_UPDATE_IND,
                sizeof(nrt_upd_ind));
        SON_LOG(anr_get_log_mode(), p_son_anr_facility_name,
                SON_BRIEF, "anr_x2_enb_config_update_ind_handler:"
                "SONX2 NRT UPDATE sent to X2 ");
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} 
/*CID 97682:end */
#endif

