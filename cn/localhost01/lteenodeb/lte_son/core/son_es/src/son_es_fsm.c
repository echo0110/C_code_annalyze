/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_es_fsm.c $
 *
 *******************************************************************************
 *
 * File Description:This file contains the functions for handling ES level
 *        messages and functions for manifest structure of this module.
 *
 *******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Feb,2012        Atul/Reetesh		         Initial
 *   Aug,2012        Rashi	                 Changes done due to changes in
 *                                           structures suggested during GERAN
 *   Sep,2012        Atul		            Changes Done to support Release 1.2
 *   July, 2014      Shilpi                  SPR 12719 Fix
 *   Aug, 2014       Shilpi                  SPR 13251 Fix
 ******************************************************************************/
#include <son_es_cell_fsm.h>
#include <son_es_fsm.h>
#include <son_utils.h>
#include <rrm_son_parser.h>
#include <son_anr_ctxt_mgr.h>
extern const son_8 *p_son_es_facility_name;
extern switch_on_pending_res_t g_es_switch_on_pending_res;
extern const QWAIT son_waitstruct;
/* AUTONOMOUS SWITCH OFF START */
extern es_global_t g_es_context;
/* Rel 3.0 Cell delete support changes Start */
extern pending_res_t g_es_pending_response;
/* Rel 3.0 Cell delete support changes Stop */
/* AUTONOMOUS SWITCH OFF END */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_START */


/*****************************************************************************
 * Function Name  : es_get_cell_id_from_mif_msgs
 * Inputs         : from     Source module id
 *                : api      Procedure code
 *                : p_msg    Pointer to the message
 * Outputs        : None
 * Returns        : son_intra_rat_global_cell_id_t*
 * Description    : This function determines the cell_id in the mif  message
 ****************************************************************************/
static son_intra_rat_global_cell_id_t *
es_get_cell_id_from_mif_msgs
(
 son_u16 from,
 son_u16 api,
 son_u8 *p_msg
 )
{
	son_intra_rat_global_cell_id_t *p_cell_id = SON_PNULL;

	switch (api)
	{
		case SONES_ENABLE_REQ:
			{
				if (SON_ES_ENABLE_SRV_CGI_PRESENT ==
						((son_es_enable_req_t *)p_msg)->
						enable_req.bitmask)
				{
					p_cell_id =
						&((son_es_enable_req_t *)p_msg)->
						enable_req.srv_cgi;
				}

				break;
			}

		case SONES_DISABLE_REQ:
			{
				if (SON_ES_ENABLE_SRV_CGI_PRESENT ==
						((son_es_disable_req_t *)p_msg)->
						disable_req.bitmask)
				{
					p_cell_id =
						&((son_es_disable_req_t *)p_msg)->
						disable_req.srv_cgi;
				}

				break;
				break;
			}

		case SONES_CELL_SWITCH_ON_REQ:
			{
				p_cell_id =
					&((son_es_cell_switch_on_req_t *)p_msg)->
					switch_on_req.srv_cgi;
				break;
			}

		case SONES_CELL_SWITCH_OFF_REQ:
			{
				p_cell_id =  &((son_es_cell_switch_off_req_t *)p_msg)->
					switch_off_req.srv_cgi;
				break;
			}
/* SPR_19619 start */

		case SONES_PEER_CELL_ACTIVATION_REQ:
            {

                /* SPR_19619 : Coverity_109491 Fix Start */
                /* CID : 109698 Fix Start */
                p_cell_id =  &((son_es_peer_cell_activation_req_t *)p_msg)->
                    peer_cell_activation_req.served_cell_list[0];

                /* CID : 109698 Fix End */
                /* SPR_19619 : Coverity_109491 Fix Stop */
                break;

            }

/* SPR_19619 stop */

		case SONES_MODIFY_UE_COUNT_INTERVAL_REQ:
			{
				p_cell_id = &((son_es_modify_ue_count_interval_req_t *)
						p_msg)->modify_interval_req.srv_cgi;
				break;
			}

		case SONES_MODIFY_UE_COUNT_THRESHOLD_REQ:
			{
				p_cell_id = &((son_es_modify_ue_count_threshold_req_t *)
						p_msg)->modify_threshold_req.srv_cgi;
				break;
			}

		case  SONES_SWITCHED_OFF_CELL_INFO_REQ:
			{
				break; /* Intentionally left blank */
			}

			/* AUTONOMOUS SWITCH OFF START */
		case  SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ:
			{
				break; /* Intentionally left blank */
			}

			/* AUTONOMOUS SWITCH OFF END */
		default:
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name,
						SON_ERROR,
						"Invalid api[%d] received"
						" from [%d]", api, from);
				break;
			}
	} /* switch */

	return p_cell_id;
} /* es_get_cell_id_from_mif_msgs */

/*****************************************************************************
 * Function Name  : es_get_cell_id_from_rrm_msgs
 * Inputs         : from     source module id
 *                : api      procedure code of the message
 *                : p_msg    pointer to the message
 * Outputs        : None
 * Returns        : son_intra_rat_global_cell_id_t*
 * Description    : This function determines the cell_id in the rrm messages
 ****************************************************************************/
static son_intra_rat_global_cell_id_t *
es_get_cell_id_from_rrm_msgs
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

		case RRM_SON_SET_ATTRIBUTE_RESP:
			{
				p_cell_id =
					(son_intra_rat_global_cell_id_t *)
					&((son_rrm_set_attribute_res_t *)p_msg)->
					res.set_attr_resp.id.object_id.cell_id;
				break;
			}

		case RRM_SON_CELL_SWITCH_ON_RESP:
			{
				p_cell_id = &((son_es_cell_switch_on_res_t *)p_msg)->
					generic_res.cell_id;
				break;
			}

		case RRM_SON_CELL_SWITCH_OFF_RESP:
			{
				p_cell_id =  &((son_es_cell_switch_off_res_t *)p_msg)->
					generic_res.cell_id;
				break;
			}

		case RRM_SON_NUM_ACTIVE_UE_REPORT:
			{
				p_cell_id = (son_intra_rat_global_cell_id_t *)
					&((son_es_active_ue_cnt_report_t *)p_msg)->
					cell_id;
				break;
			}

		case RRM_UE_COUNT_THRESHOLD_REACHED_IND:
			{
				p_cell_id = (son_intra_rat_global_cell_id_t *)
					&((son_es_threshold_hit_ind_t *)p_msg)->cell_id;
				break;
			}

			/* AUTONOMOUS SWITCH OFF START */
		case RRM_SON_LOAD_REPORT_IND:
			{
				p_cell_id =
					(son_intra_rat_global_cell_id_t *)
					&(((son_es_load_report_ind_t *)p_msg)->serv_cell_load_info.g_cell_id);
				break;
			}

			/* AUTONOMOUS SWITCH OFF END */
		default:
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name,
						SON_ERROR,
						"es_get_cell_id_from_rrm_msgs: Invalid api[%d] received"
						" from [%d]", api, from);
				break;
			}
	} /* switch */

	return p_cell_id;
} /* es_get_cell_id_from_rrm_msgs */

/*****************************************************************************
 * Function Name  : es_get_cell_id_from_msg
 * Inputs         : from     source module id
 *                : api      procedure code of the message
 *                : p_msg    pointer to the message
 * Outputs        : None
 * Returns        : son_intra_rat_global_cell_id_t*
 * Description    : This function determines the cell_id in the message received
 ****************************************************************************/
static son_intra_rat_global_cell_id_t *
es_get_cell_id_from_msg
(
 son_u16 from,
 son_u16 api,
 son_u8 *p_msg
 )
{
	son_intra_rat_global_cell_id_t *p_cell_id = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SON_MIF_MODULE_ID == from)
	{
		p_cell_id = es_get_cell_id_from_mif_msgs( from, api, p_msg);
        if (SON_NULL != p_cell_id)
        {

            SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
                    "Cell 0x%x received in Msg %u from MIF",
                    son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity), api);
        }
	}
	else if (SON_RRM_MODULE_ID == from)
	{
		p_cell_id = es_get_cell_id_from_rrm_msgs( from, api, p_msg );
        if (SON_NULL != p_cell_id)
        {

            SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
                    "Cell 0x%x received in Msg %u from RRM",
                    son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity), api);
        }
	}
	else
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_ERROR, "Message received"
				" from unknown module %d", from);
	}

	SON_UT_TRACE_EXIT();
	return p_cell_id;
} /* es_get_cell_id_from_msg */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_STOP */

/*****************************************************************************
 * Function Name  : es_create_send_generic_nack
 * Inputs         : p_msg           Pointer to the message received
 *                  p_cspl_hdr      Pointer to cspl stack header
 *					p_cell_id	    Pointer to Cell ID
 *                  error_code      son_error_et
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the function to send NACK response to sender
 ******************************************************************************/
static son_void_t
es_create_send_generic_nack
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_intra_rat_global_cell_id_t  *p_cell_id,
 son_error_et error_code
 )
{
	son_u32 length                   = SON_NULL;
	son_bool_et send_res  = SON_TRUE;
	son_u8          *p_resp = SON_PNULL;

	SON_UT_TRACE_ENTER();

	switch (p_cspl_hdr->api)
	{
		case SONES_ENABLE_REQ:  /* Intentional Break Left */
		case SONES_DISABLE_REQ:
			{                                /* Enable and Disable are same structures */
				length = sizeof(son_es_enable_res_t);

				p_resp = (son_u8 *)son_mem_get(length);
				if (SON_PNULL != p_resp)
				{
					SON_MEMSET(p_resp, 0, length);
					((son_es_enable_res_t *)p_resp)->transaction_id =
						*((son_u16 *)p_msg);
					((son_es_enable_res_t *)p_resp)->enable_res.result = SON_FAILURE;

					if (SON_ERR_UNEXPECTED_MSG == error_code)
					{
						((son_es_enable_res_t *)p_resp)->enable_res.error_code = error_code;
					}
					else
					{
						if (SON_PNULL != p_cell_id)
						{
							((son_es_enable_res_t *)p_resp)->enable_res.cell_status_list_size
								= 1;
							((son_es_enable_res_t *)p_resp)->enable_res.cell_status_list[0].
								error_code = error_code;

							SON_MEMCPY(&((son_es_enable_res_t *)p_resp)->
									enable_res.cell_status_list[0].cgi,
									p_cell_id, sizeof(son_intra_rat_global_cell_id_t));
						}
						else
						{
							((son_es_enable_res_t *)p_resp)->enable_res.error_code = error_code;
						}
					}
				}

				break;
			}

		case SONES_MODIFY_UE_COUNT_INTERVAL_REQ:  /*Intentional Break Left*/
		case SONES_MODIFY_UE_COUNT_THRESHOLD_REQ: /*Genrck Resp structure */
		case SONES_CELL_SWITCH_ON_REQ:
		case SONES_CELL_SWITCH_OFF_REQ:
			{
				length = sizeof(son_generic_response_with_cell_id_t);
				p_resp = (son_u8 *)son_mem_get(length);
				if (SON_PNULL != p_resp)
				{
					if (SON_PNULL != p_cell_id)
					{
						SON_MEMCPY(&(
									(son_generic_response_with_cell_id_t *)p_resp)->
								generic_res.cell_id, p_cell_id,
								sizeof(son_intra_rat_global_cell_id_t));
					}

					((son_generic_response_with_cell_id_t *)p_resp)->transaction_id =
						*((son_u16 *)p_msg);
					((son_generic_response_with_cell_id_t *)p_resp)->
						generic_res.result
						= SON_FAILURE;
					((son_generic_response_with_cell_id_t *)p_resp)->
						generic_res.error_code = error_code;
				}

				break;
			}

		case SONES_SWITCHED_OFF_CELL_INFO_REQ:
			{
				length = sizeof(son_es_switched_off_cell_info_res_t);
				p_resp = (son_u8 *)son_mem_get(length);
				if (SON_PNULL != p_resp)
				{
					((son_es_switched_off_cell_info_res_t *)p_resp)->transaction_id =
						*((son_u16 *)p_msg);
					((son_es_switched_off_cell_info_res_t *)p_resp)->
						switched_off_cell_info.result = SON_FAILURE;
					((son_es_switched_off_cell_info_res_t *)p_resp)->
						switched_off_cell_info.error_code = error_code;
					((son_es_switched_off_cell_info_res_t *)p_resp)->
						switched_off_cell_info.num_switched_off_cells = 0;
				}

				break;
			}

			/* ---------------ES 1.2 Changes Start ------------------*/
		case SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ:
			{
				length = sizeof(son_es_cell_switchon_for_x2_setup_res_t);
				p_resp = (son_u8 *)son_mem_get(length);
				if (SON_PNULL != p_resp)
				{
					((son_es_cell_switchon_for_x2_setup_res_t *)p_resp)->
						transaction_id = *((son_u16 *)p_msg);
					((son_es_cell_switchon_for_x2_setup_res_t *)p_resp)->
						result = SON_FAILURE;
					((son_es_cell_switchon_for_x2_setup_res_t *)p_resp)->
						error_code = error_code;
				}

				break;
			}

			/* ---------------ES 1.2 Changes Finished ------------------
			 * AUTONOMOUS SWITCH OFF START */
		case SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ:
			{
				length = sizeof(son_es_autonomous_switch_off_config_resp_t);
				p_resp = (son_u8 *)son_mem_get(length);
				if (SON_PNULL != p_resp)
				{
					SON_MEMSET(p_resp, 0, length);
					((son_es_autonomous_switch_off_config_resp_t *)p_resp)->transaction_id =
						*((son_u16 *)p_msg);
					((son_es_autonomous_switch_off_config_resp_t *)p_resp)->
						autonomous_switch_off_config_resp.result = SON_FAILURE;
					((son_es_autonomous_switch_off_config_resp_t *)p_resp)->
						autonomous_switch_off_config_resp.error_code = error_code;
					((son_es_autonomous_switch_off_config_resp_t *)p_resp)->
						autonomous_switch_off_config_resp.cell_status_list_size = SON_NULL;
				}

				break;
			}
			/* SPR 11689 start*/
		case SONES_CELL_ACTIVATION_REQ:
			{
				length = sizeof(son_es_cell_activation_resp_t);
				p_resp = (son_u8 *)son_mem_get(length);
				if (SON_PNULL != p_resp)
				{
					SON_MEMSET(p_resp, 0, length);
					((son_es_cell_activation_resp_t *)p_resp)->
						enbid_1 =  ((son_es_cell_activation_req_t *)p_msg)->enbid_1; 

					((son_es_cell_activation_resp_t *)p_resp)->
						enbid_2 =  ((son_es_cell_activation_req_t *)p_msg)->enbid_2; 

					((son_es_cell_activation_resp_t *)p_resp)->
						result = SON_FAILURE;
					((son_es_cell_activation_resp_t *)p_resp)->
						error_code = error_code;
				} 
				break; 

			}
			/*SPR 11689 changes end */

			/* AUTONOMOUS SWITCH OFF END */
		default:
			{
				SON_LOG(es_get_log_mode(),
						p_son_es_facility_name,
						SON_ERROR,
						"Unexpected Message [%d] Received from module [%d]",
						p_cspl_hdr->api, p_cspl_hdr->from);
				send_res = SON_FALSE;
				break;
			}
	} /* switch */

	if (send_res)
	{
		SON_LOG(es_get_log_mode(),
				p_son_es_facility_name,
				SON_ERROR,
				"Message [%d] sent from module [%d]",
				p_cspl_hdr->api, p_cspl_hdr->from);

		son_create_send_buffer(p_resp, SON_ES_MODULE_ID,
				SON_MIF_MODULE_ID,
				(son_procedure_code_et)(p_cspl_hdr->api + (
						SONES_RES_BASE_API_ID - SONES_REQ_BASE_API_ID)),
				length );
	}

	if (p_resp)
	{
		son_mem_free(p_resp);
	}

	SON_UT_TRACE_EXIT();
} /* es_create_send_generic_nack */

/*****************************************************************************
 * Function Name  : es_calculate_event_id
 * Inputs         : p_cspl_hdr         Pointer to CSPL Header
 * Outputs        : None
 * Returns        : son_es_index_et
 * Description    : This function computes the index in the FSM for the given
 *                : procedure code
 ****************************************************************************/
static son_es_index_et
es_calculate_event_id
(
 STACKAPIHDR *p_cspl_hdr
 )
{
	son_es_index_et ret = IDX_NOT_AVAILABLE;

	SON_UT_TRACE_ENTER();

	if (SON_MIF_MODULE_ID == p_cspl_hdr->from)
	{
		switch (p_cspl_hdr->api)
		{
			case SONES_ENABLE_REQ:
				{
                    SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
                            "MIF->ES:SONES_ENABLE_REQ");
					ret = IDX_SONES_ENABLE_REQ;
					break;
				}

			case SONES_DISABLE_REQ:
				{
                    SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
                            "MIF->ES:SONES_DISABLE_REQ");
					ret = IDX_SONES_DISABLE_REQ;
					break;
				}

			case SONES_CELL_SWITCH_ON_REQ:
				{
                    SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
                            "MIF->ES:SONES_CELL_SWITCH_ON_REQ");
					ret = IDX_SONES_CELL_SWITCH_ON_REQ;
					break;
				}

			case SONES_CELL_SWITCH_OFF_REQ:
				{
                    SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
                            "MIF->ES:SONES_CELL_SWITCH_OFF_REQ");
					ret = IDX_SONES_CELL_SWITCH_OFF_REQ;
					break;
				}

			case SONES_MODIFY_UE_COUNT_INTERVAL_REQ:
				{
                    SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
                            "MIF->ES:SONES_MODIFY_UE_COUNT_INTERVAL_REQ");
					ret = IDX_SONES_MODIFY_UE_COUNT_INTERVAL_REQ;
					break;
				}

			case SONES_MODIFY_UE_COUNT_THRESHOLD_REQ:
				{
                    SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
                            "MIF->ES:SONES_MODIFY_UE_COUNT_THRESHOLD_REQ");
					ret = IDX_SONES_MODIFY_UE_COUNT_THRESHOLD_REQ;
					break;
				}

			default:
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name,
							SON_WARNING,
							"Invalid API = [%u] received from SMIF", p_cspl_hdr->api);
					break;
				}
		} /* switch */
	}
	else if (SON_RRM_MODULE_ID == p_cspl_hdr->from)
	{
		switch (p_cspl_hdr->api)
		{
			case RRM_SON_REGISTER_RESP:
				{
					ret = IDX_RRM_SON_REGISTER_RESP;
					break;
				}

			case RRM_SON_DEREGISTER_RESP:
				{
					ret = IDX_RRM_SON_DEREGISTER_RESP;
					break;
				}

			case RRM_SON_SET_ATTRIBUTE_RESP:
				{
					ret = IDX_RRM_SON_SET_ATTRIBUTE_RESP;
					break;
				}

			case RRM_SON_CELL_SWITCH_ON_RESP:
				{
					ret = IDX_RRM_SON_CELL_SWITCH_ON_RESP;
					break;
				}

			case RRM_SON_CELL_SWITCH_OFF_RESP:
				{
					ret = IDX_RRM_SON_CELL_SWITCH_OFF_RESP;
					break;
				}

			case RRM_SON_NUM_ACTIVE_UE_REPORT:
				{
					ret = IDX_RRM_SON_NUM_ACTIVE_UE_REPORT;
					break;
				}

			case RRM_UE_COUNT_THRESHOLD_REACHED_IND:
				{
					ret = IDX_RRM_UE_COUNT_THRESHOLD_REACHED_IND;
					break;
				}

				/*case RRM_S1_MSG_IND:
				 * ret = IDX_RRM_S1_MSG_IND;
				 * break;
				 */
			case RRM_SON_CELL_STATE_CHANGE_IND:
				{
					ret = IDX_RRM_SON_CELL_CHNAGE_IND;
					break;
				}

			case RRM_SON_LOAD_REPORT_IND:
				{
					ret = IDX_RRM_SON_LOAD_REPORT;
					break;
				}

			default:
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name,
							SON_WARNING,
							"Invalid API = [%u] received from RRM", p_cspl_hdr->api);
					break;
				}
		} /* switch */
	}
	else if (SON_X2_MODULE_ID == p_cspl_hdr->from)
	{
		switch (p_cspl_hdr->api)
		{
			case SONES_CELL_ACTIVATION_REQ:
				{
					ret = IDX_SONES_CELL_ACTIVATION_REQ;
					break;
				}

			default:
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name,
							SON_WARNING,
							"es_calculate_event_id: "
							"Invalid API = [%u] received from SON-X2", p_cspl_hdr->api);
					break;
				}
		} /* switch */
	}
	else
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_WARNING,
				"invalid api = [%u] received from [%u]", p_cspl_hdr->api,
				p_cspl_hdr->from);
	}

	return ret;
} /* es_calculate_event_id */

/*****************************************************************************
 * Function Name  : es_log_handler
 * Inputs         : p_cspl_hdr  Pointer to the CSPL header
 *				  : p_buf       Pointer to the message received
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ES module and is called
 *                  when SMIF_INIT_CONFIG_REQ or SMIF_LOG_ENABLE_DISABLE_CMD or
 *                  SMIF_SET_LOG_LEVEL_CMD message is received
 ******************************************************************************/
static son_buf_retain_status_et
es_log_handler
(
 STACKAPIHDR    *p_cspl_hdr,
 son_u8             *p_buf
 )
{
	SON_UT_TRACE_ENTER();
	switch (p_cspl_hdr->api)
	{
		case SMIF_INIT_CONFIG_REQ:
			{
				smif_init_config_req_t *p_msg = (smif_init_config_req_t *)p_buf;
				es_set_log_mode(p_msg->log_enable);

				/* Print the current logging status(enable/disable)
				 * SON_OAM_LOG_ON: always print log,even if it is disabled. */
				SON_LOG(SON_OAM_LOG_ON, p_son_es_facility_name, SON_BRIEF,
						"Log enable/disable = [%u]",
						p_msg->log_enable);
				if (SON_OAM_LOG_ON == es_get_log_mode())
				{
					/* SPR-13251 Fix Starts */
					es_set_log_level(p_msg->log_level);
					/* SPR-13251 Fix Ends */
					SET_MODULE_LOG_LEVEL(p_msg->log_level);
				}
				else
				{
					/* SPR-13251 Fix Starts */
					es_set_log_level(SON_LOG_LEVEL_NONE);
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
				es_set_log_mode((son_oam_log_on_off_et)(p_msg->log_enable));

				/* Print the current logging status(enable/disable)
				 *      SON_OAM_LOG_ON: always print log,even if it is disabled. */
				SON_LOG(SON_OAM_LOG_ON, p_son_es_facility_name, SON_BRIEF,
						"Log enable/disable = [%u]",
						p_msg->log_enable);

				if (SON_OAM_LOG_ON == es_get_log_mode())
				{
					if (p_msg->bitmask & SMIF_MODULE_LOG_LEVEL_PRESENT)
					{
						/* SPR-13251 Fix Starts */
						es_set_log_level(p_msg->log_level);
						/* SPR-13251 Fix Ends */
						SET_MODULE_LOG_LEVEL(p_msg->log_level);
					}
					else
					{
						SON_LOG(SON_OAM_LOG_ON, p_son_es_facility_name, SON_BRIEF,
								"BitMask SMIF_MODULE_LOG_LEVEL_PRESENT not present");
					}
				}
				else
				{
					/* SPR-13251 Fix Starts */
					es_set_log_level(SON_LOG_LEVEL_NONE);
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

				if (SON_OAM_LOG_ON == es_get_log_mode())
				{
					SON_LOG(SON_OAM_LOG_ON, p_son_es_facility_name,
							SON_BRIEF,
							"Log level changed through msg: "
							"SMIF_SET_LOG_LEVEL_CMD");
					/* SPR-13251 Fix Starts */
					es_set_log_level(p_msg->log_level);
					/* SPR-13251 Fix Ends */
					SET_MODULE_LOG_LEVEL(p_msg->log_level);
				}

				break;
			}

		default:
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
						"Invalid message [%d] received from [%d]",
                        p_cspl_hdr->api, p_cspl_hdr->from);
				break;
			}
	} /* switch */

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_log_handler */

/*****************************************************************************
 * Function Name  : es_cell_info_ind_handler
 * Inputs         : p_cspl_hdr  Pointer to the CSPL header
 *				  : p_buf       Pointer to the message received
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ES module and is called when
 *                  SMIF_CELL_INFO_IND message is received.
 ******************************************************************************/
static son_buf_retain_status_et
es_cell_info_ind_handler
(
 STACKAPIHDR    *p_cspl_hdr,
 son_u8 *p_buf
 )
{
	son_cell_info_ind_t         *p_msg                  = (son_cell_info_ind_t *)p_buf;
	son_es_enable_req_t enable_req          = {0};
	es_cell_context_node_t  *p_cell_ctxt    = SON_PNULL;
	son_u8 idx                     = 0;
	son_cell_status_t cell_sts;
	son_oam_log_on_off_et log_mode       = es_get_log_mode();
	son_feature_state_change_cause_et cause = SON_STATE_CHANGE_CAUSE_UNDEFINED;
	son_feature_state_et state = SON_STATE_UNDEFINED;
    /*SPR_19279_START */
    son_u8 cell_index = *((son_u8 *)(p_buf + sizeof(son_cell_info_ind_t)));
    /*SPR_19279_END */

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&cell_sts, 0, sizeof(son_cell_status_t));
	if (0 == p_msg->cell_info_size)
	{
		SON_LOG(log_mode, p_son_es_facility_name, SON_ERROR,
				"Cell list is empty");
	}
	else
	{
		for (idx = 0; idx < p_msg->cell_info_size; idx++)
		{
			/*SPR-11554-Start*/
			p_cell_ctxt = es_add_new_cell_in_context(
					&p_msg->serving_cell[idx],
           /*SPR_19279_START */
					&state, &cause,
                    cell_index);
           /*SPR_19279_END */
			/*SPR-11554-End*/

			if (SON_DISABLED == es_get_global_mode() ||
					SON_PNULL == p_cell_ctxt)
			{
				/* Send SONES_FEATURE_STATE_CHANGE_IND */
				es_send_feature_state_change_ind(
						&p_msg->serving_cell[idx].src_cgi,
						state, cause);
			}

			if (SON_PNULL != p_cell_ctxt && SON_ENABLED == es_get_global_mode())
			{
				if (!es_get_pending_res_expected_count())
				{
					es_init_pending_res(son_generate_txn_id(), SMIF_CELL_INFO_IND);
				}

				enable_req.transaction_id = es_get_pending_res_transaction_id();
				enable_req.enable_req.bitmask = SON_ES_ENABLE_SRV_CGI_PRESENT;

				SON_MEMCPY(&enable_req.enable_req.srv_cgi,
						&p_msg->serving_cell[idx].src_cgi,
						sizeof(son_intra_rat_global_cell_id_t));

				/* Fix 867 Start*/
				enable_req.enable_req.interval = g_es_context.ue_report_interval;
				enable_req.enable_req.threshold_value = g_es_context.ue_count_threshold;
				/* Fix 867 End */

				/* Rel 3.0 Cell delete support changes Start */
				cell_sts.error_code = SON_ERR_TIMER_EXPIRED;
				/* Rel 3.0 Cell delete support changes Stop */
				SON_MEMCPY(&cell_sts.cgi,
						&p_msg->serving_cell[idx].src_cgi,
						sizeof(son_intra_rat_global_cell_id_t));

				if (SON_TRUE == es_add_cell_in_pending_res(&cell_sts))
				{
					es_cell_process_msg(IDX_SONES_ENABLE_REQ,
							&p_cell_ctxt->data,
							p_cspl_hdr,
							(son_u8 *)&enable_req);
					es_set_context_state(ES_STATE_ENABLING);
				}
				else
				{
					SON_LOG(log_mode, p_son_es_facility_name, SON_ERROR,
							"Cell [0x%x] could not be added "
							"to pending response list",
							son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->data.cell_id.cell_identity));
				}
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* es_cell_info_ind_handler */

/*****************************************************************************
 * Function Name  : es_global_enable_req_handler
 * Inputs         : p_cspl_hdr  Pointer to the CSPL header
 *                   p_msg        Pointer to the message received
 *                   event_id     event id
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ES module and is
 *					invoked when SONES_ENABLE_REQ message with
 *					no cell id is received.
 *****************************************************************************/
static son_buf_retain_status_et
es_global_enable_req_handler
(
 STACKAPIHDR        *p_cspl_hdr,
 son_u8                     *p_msg,
 son_es_index_et event_id
 )
{
	son_bool_et send_g_enable_res  = SON_TRUE;
	son_es_enable_res_t g_enable_resp           = {0};
	son_es_enable_req_t         *p_buf  = (son_es_enable_req_t  *)p_msg;
	es_cell_context_t           *p_cell_ctxt     = SON_PNULL;
	es_cell_context_node_t *p_cell_ctxt_node = SON_PNULL;
	son_es_enable_req_t enable_req          = {0};
	son_cell_status_t cell_sts;
	son_oam_log_on_off_et log_mode       = es_get_log_mode();
	es_fsm_state_et curr_fsm_state = es_get_context_state();
	son_return_et result = SON_FAILURE;

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&cell_sts.cgi, 0, sizeof(son_intra_rat_global_cell_id_t));
	cell_sts.error_code                 = SON_ERR_ENABLE_IN_PROGRESS;

	es_set_global_mode(SON_ENABLED);

	if (ES_STATE_ENABLING != curr_fsm_state)
	{
		if (ES_STATE_DISABLING == curr_fsm_state)
		{
			es_reset_pending_res();
		}

		/* Fix 867 Start*/
		g_es_context.ue_report_interval = p_buf->enable_req.interval;
		g_es_context.ue_count_threshold = p_buf->enable_req.threshold_value;
		/* Fix 867 End */

		/*Pointer to the first context node*/
		p_cell_ctxt_node = es_get_first_cell_ctxt();

		while (SON_PNULL != p_cell_ctxt_node)
		{
			/*Get the data part of the node*/
			p_cell_ctxt = &p_cell_ctxt_node->data;

			if (ES_CELL_STATE_DISABLED  ==
					p_cell_ctxt->current_cell_fsm_state ||
					ES_CELL_STATE_DISABLING ==
					p_cell_ctxt->current_cell_fsm_state)
			{
				if (!es_get_pending_res_expected_count())
				{
					es_init_pending_res(p_buf->transaction_id, SONES_ENABLE_RES);
				}

				enable_req.transaction_id =
					es_get_pending_res_transaction_id();
				enable_req.enable_req.bitmask = SON_ES_ENABLE_SRV_CGI_PRESENT;
				enable_req.enable_req.interval = p_buf->enable_req.interval;
				enable_req.enable_req.threshold_value = p_buf->
					enable_req.threshold_value;

				SON_MEMCPY(&enable_req.enable_req.srv_cgi,
						&p_cell_ctxt->cell_id,
						sizeof(son_intra_rat_global_cell_id_t));

				/* Rel 3.0 Cell delete support changes Start */
				cell_sts.error_code = SON_ERR_TIMER_EXPIRED;
				/* Rel 3.0 Cell delete support changes Stop */
				result = SON_FAILURE;

				SON_MEMCPY(&cell_sts.cgi,
						&p_cell_ctxt->cell_id,
						sizeof(son_intra_rat_global_cell_id_t));

				if (SON_TRUE == es_add_cell_in_pending_res(&cell_sts))
				{
					es_cell_process_msg(event_id,
							p_cell_ctxt,
							p_cspl_hdr,
							(son_u8 *)&enable_req);

					if (SON_TRUE == send_g_enable_res)
					{
						send_g_enable_res = SON_FALSE;
						es_set_context_state(ES_STATE_ENABLING);
					}
				}
				else
				{
					SON_LOG(log_mode, p_son_es_facility_name,
							SON_ERROR,
							"Cell [0x%x] could not be added to "
							"pending response list",
							son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
				}
			}
			else
			{
				SON_LOG(log_mode, p_son_es_facility_name,
						SON_ERROR,
						"Cell with cells_id [0x%x] is in %s state",
						son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
						SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->
						current_cell_fsm_state]);
			}

			p_cell_ctxt_node = es_get_next_node(p_cell_ctxt_node);
		}

		if (SON_TRUE == send_g_enable_res)
		{
			cell_sts.error_code = SON_NO_ERROR;
			result = SON_SUCCESS;
			if (SON_PNULL == es_get_first_cell_ctxt())
			{
				SON_LOG(log_mode, p_son_es_facility_name,
						SON_BRIEF,
						"Global Enable Request"
						" received but No Cell is Configured");
			}
			else
			{
				SON_LOG(log_mode, p_son_es_facility_name,
						SON_BRIEF,
						"Global Enable Request"
						" received but All Configured Cells are already "
						"Enabled or Enabling");
			}
		}
	}
	else
	{
		SON_LOG(log_mode, p_son_es_facility_name,
				SON_ERROR,
				"ES Global enable is "
				"in progress");
	}

	if (SON_TRUE == send_g_enable_res)
	{
		/*Send the Enable Response*/
		g_enable_resp.transaction_id  = p_buf->transaction_id;
		g_enable_resp.enable_res.error_code =
			cell_sts.error_code;
		g_enable_resp.enable_res.result = result;
		son_create_send_buffer((son_u8 *)&g_enable_resp, SON_ES_MODULE_ID,
				SON_MIF_MODULE_ID, SONES_ENABLE_RES,
				sizeof(g_enable_resp));
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* es_global_enable_req_handler */

/*****************************************************************************
 * Function Name  : es_global_disable_req_handler
 * Inputs         : p_cspl_hdr  Pointer to the CSPL header
 *                   p_msg        Pointer to the message received
 *                   event_id     event id
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ES module and is
 *					invoked when SONES_DISABLE_REQ message with
 *					no cell id is received
 *****************************************************************************/
static son_buf_retain_status_et
es_global_disable_req_handler
(
 STACKAPIHDR        *p_cspl_hdr,
 son_u8                     *p_msg,
 son_es_index_et event_id
 )
{
	son_bool_et send_g_disable_res  = SON_TRUE;
	son_es_disable_res_t g_disable_resp          = {0};
	son_es_disable_req_t        *p_buf       = (son_es_disable_req_t  *)p_msg;
	es_cell_context_t           *p_cell_ctxt        = SON_PNULL;
	es_cell_context_node_t      *p_cell_ctxt_node    = SON_PNULL;
	son_es_disable_req_t disable_req                     = {0};
	son_cell_status_t cell_sts;
	son_oam_log_on_off_et log_mode       = es_get_log_mode();
	es_fsm_state_et curr_fsm_state = es_get_context_state();
	son_return_et result = SON_FAILURE;

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&cell_sts.cgi, 0, sizeof(son_intra_rat_global_cell_id_t));
	cell_sts.error_code         = SON_ERR_DISABLE_IN_PROGRESS;

	es_set_global_mode(SON_DISABLED);

	if (ES_STATE_UNDEFINED == curr_fsm_state)
	{
		SON_LOG(log_mode, p_son_es_facility_name,
				SON_ERROR,
				"Current global FSM state is ES_STATE_UNDEFINED");
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	if (ES_STATE_DISABLING != curr_fsm_state)
	{
		if (ES_STATE_ENABLING == curr_fsm_state)
		{
			es_reset_pending_res();
		}

		/*Pointer to the first context node*/
		p_cell_ctxt_node = es_get_first_cell_ctxt();

		while (SON_PNULL != p_cell_ctxt_node)
		{
			/*Get the data part of the node*/
			p_cell_ctxt = &p_cell_ctxt_node->data;

			if ((ES_CELL_STATE_DISABLED !=
						p_cell_ctxt->current_cell_fsm_state)
					&& (ES_CELL_STATE_DISABLING !=
						p_cell_ctxt->current_cell_fsm_state))
			{
				if (!es_get_pending_res_expected_count())
				{
					es_init_pending_res(p_buf->transaction_id,
							SONES_DISABLE_RES);
				}

				disable_req.transaction_id =
					es_get_pending_res_transaction_id();
				disable_req.disable_req.bitmask = SON_ES_ENABLE_SRV_CGI_PRESENT;

				SON_MEMCPY(&disable_req.disable_req.srv_cgi,
						&p_cell_ctxt->cell_id,
						sizeof(son_intra_rat_global_cell_id_t));
				cell_sts.error_code = SON_ERR_DEREGISTERATION_FAILURE;
				result = SON_FAILURE;

				SON_MEMCPY(&cell_sts.cgi,
						&p_cell_ctxt->cell_id,
						sizeof(son_intra_rat_global_cell_id_t));

				if (SON_TRUE == es_add_cell_in_pending_res(&cell_sts))
				{
					es_cell_process_msg(event_id,
							p_cell_ctxt,
							p_cspl_hdr,
							(son_u8 *)&disable_req);
					if (SON_TRUE == send_g_disable_res)
					{
						send_g_disable_res = SON_FALSE;
						es_set_context_state(ES_STATE_DISABLING);
					}
				}
				else
				{
					SON_LOG(log_mode, p_son_es_facility_name,
							SON_ERROR,
							"Cell [0x%x] "
							"could not be added to "
							"pending response list",
							son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
				}
			}
			else
			{
				SON_LOG(log_mode, p_son_es_facility_name,
						SON_ERROR,
						"Cell with cells_id [0x%x]"
						" is in %s state",
						son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
						SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->
						current_cell_fsm_state]);
			}

			p_cell_ctxt_node = es_get_next_node(p_cell_ctxt_node);
		}

		if (SON_TRUE == send_g_disable_res)
		{
			cell_sts.error_code = SON_NO_ERROR;
			result = SON_SUCCESS;

			if (SON_PNULL == es_get_first_cell_ctxt())
			{
				SON_LOG(log_mode, p_son_es_facility_name,
						SON_BRIEF,
						"Global Disable Request"
						" received but No Cell is Configured");
			}
			else
			{
				SON_LOG(log_mode, p_son_es_facility_name,
						SON_BRIEF,
						"Global Disable Request"
						" received but All Configured Cells are already "
						"Disabled/Disabling");
			}
		}
	}
	else
	{
		SON_LOG(log_mode, p_son_es_facility_name,
				SON_ERROR,
				"ES Global disable is "
				"in progress");
	}

	if (SON_TRUE == send_g_disable_res)
	{
		/*Send the Disable Response*/
		g_disable_resp.transaction_id  = p_buf->transaction_id;

		g_disable_resp.enable_res.error_code = cell_sts.error_code;
		g_disable_resp.enable_res.result = result;

		son_create_send_buffer((son_u8 *)&g_disable_resp, SON_ES_MODULE_ID,
				SON_MIF_MODULE_ID, SONES_DISABLE_RES,
				sizeof(g_disable_resp));
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* es_global_disable_req_handler */

/*****************************************************************************
 * Function Name  : handle_switched_off_cell_info
 * Inputs         : p_cspl_hdr  Pointer to the CSPL header
 *                  p_msg        Pointer to the message received
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for
 *                  SONES_SWITCHED_OFF_CELL_INFO_REQ message. It finds all the
 *                  the switched off cells Ids and sends them in the
 *                  SONES_SWITCHED_OFF_CELL_INFO_RES message to SONMgmtIfH.
 ******************************************************************************/
static son_buf_retain_status_et
handle_switched_off_cell_info
(
 STACKAPIHDR    *p_cspl_hdr,
 son_u8          *p_msg
 )
{
	son_u8 cell_id_count = 0;
	son_es_switched_off_cell_info_req_t    *p_cell_info_req = SON_PNULL;
	son_es_switched_off_cell_info_res_t cell_info_res = {0};

	p_cell_info_req = (son_es_switched_off_cell_info_req_t *)p_msg;
	es_cell_context_node_t *p_cxt_node = SON_PNULL;

	SON_UT_TRACE_ENTER();
	cell_info_res.transaction_id =  p_cell_info_req->transaction_id;
	cell_info_res.switched_off_cell_info.result = SON_SUCCESS;
	cell_info_res.switched_off_cell_info.error_code = SON_NO_ERROR;

	/* Check if the context list has any nodes present */
	p_cxt_node = es_get_first_cell_ctxt();
	if (SON_PNULL == p_cxt_node)
	{
		/* Set cell_id_count to 0 as no nodes are present in the list */
		cell_id_count = 0;
	}
	else
	{
		/* Iterate through all the nodes of the list */
		do
		{
			/* SPR-12719 Fix Starts */
			/*  Check if the node has entry for cell which is in
			 **  switched off state. If yes then increment switched 
			 **  off cell count and store its cell id. */
			if (ES_CELL_STATE_SWITCHED_OFF == p_cxt_node->data.
					current_cell_fsm_state)
				/* SPR-12719 Fix Ends */
			{
				SON_MEMCPY(&(cell_info_res.switched_off_cell_info.
							switched_off_cell_info_list[cell_id_count]),
						&p_cxt_node->data.cell_id,
						sizeof(son_intra_rat_global_cell_id_t));
				cell_id_count++;
			}
		} while (SON_PNULL != (p_cxt_node = es_get_next_node(p_cxt_node)));
	}

	cell_info_res.switched_off_cell_info.num_switched_off_cells = cell_id_count;

     /*SPR 17777 +-*/
	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
			"Total Number of Switched OFF Cells %d", cell_id_count);
	LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
     /*SPR 17777 +-*/

	son_create_send_buffer((son_u8 *)&cell_info_res, SON_ES_MODULE_ID,
			SON_MIF_MODULE_ID, SONES_SWITCHED_OFF_CELL_INFO_RES,
			sizeof(son_es_switched_off_cell_info_res_t));

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* handle_switched_off_cell_info */

/* -------------------ES 1.2 Changes Start ------------------------*/

/*****************************************************************************
 * Function Name  : handle_cell_switchon_for_x2_setup_req
 * Inputs         : p_cspl_hdr  Pointer to the CSPL header
 *                  p_msg        Pointer to the message received
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for
 *                  SONES_CELL_SWITCHON_FOR_X2_SETUP_REQ message. It shall call
 *                  all the Cell FSMs which are not switched on and also update
 *                  the Pending Switchedon Response structure.
 ******************************************************************************/
static son_buf_retain_status_et
handle_cell_switchon_for_x2_setup_req
(
 STACKAPIHDR    *p_cspl_hdr,
 son_u8          *p_msg
 )
{
	son_es_cell_switchon_for_x2_setup_req_t    *p_cell_switchon_x2_req =
		SON_PNULL;
	son_es_cell_switchon_for_x2_setup_res_t cell_switchon_x2_res = {0};
	son_cell_switchon_status_t cell_sts;
	son_return_et result = SON_SUCCESS;
	son_error_et error_code = SON_NO_ERROR;
	son_bool_et send_x2_switchon_res  = SON_FALSE;
	SON_LIST_NODE           *p_node             = SON_PNULL;
	cell_switchon_status_list_data_t *p_list_node = SON_PNULL;
	son_es_cell_switch_on_req_t req = {0};
	es_cell_context_t       *p_cell_ctxt     = SON_PNULL;
	es_cell_context_node_t  *p_cxt_node = SON_PNULL;
	son_time_t current_time;
	son_oam_log_on_off_et log_mode        = es_get_log_mode();

	p_cell_switchon_x2_req = (son_es_cell_switchon_for_x2_setup_req_t *)p_msg;

	SON_UT_TRACE_ENTER();

	/* Pointer to the first context node*/
	p_cxt_node = es_get_first_cell_ctxt();

	if (SON_TRUE == es_switchon_pending_res_has_entry())
	{
		send_x2_switchon_res = SON_TRUE;
		result = SON_FAILURE;
		error_code = SON_ERR_ES_CELL_SWITCHON_FOR_X2_IN_PROGRESS;

		SON_LOG(log_mode, p_son_es_facility_name,
				SON_WARNING,
				"Cell Switch On request from ANR is in progress");
	}
	else
	{
		/* Check if the context list has any nodes present */
		if (SON_PNULL == p_cxt_node)
		{
			send_x2_switchon_res = SON_TRUE;
			result = SON_FAILURE;
			error_code = SON_ERR_NO_CELL_CONFIGURED;
			SON_LOG(log_mode, p_son_es_facility_name,
					SON_INFO,
					" No cell is confured at ES");
		}
		else
		{
			es_init_switch_on_pending_res(
					p_cell_switchon_x2_req->transaction_id,
					(son_procedure_code_et)(p_cspl_hdr->api));

			/* Iterate through all the nodes of the list */
			do
			{
				/*Get the data part of the node*/
				p_cell_ctxt = &p_cxt_node->data;

				/*  Check if the node has entry for cell which is in
				 **  switching off/switching on/switched on state.
				 */
				if (ES_CELL_STATE_SWITCHING_OFF ==
						p_cell_ctxt->current_cell_fsm_state ||
						ES_CELL_STATE_SWITCHED_OFF ==
						p_cell_ctxt->current_cell_fsm_state ||
						ES_CELL_STATE_SWITCHING_ON ==
						p_cell_ctxt->current_cell_fsm_state)
				{
					SON_MEMSET(&cell_sts, 0, sizeof(son_cell_switchon_status_t));
					SON_MEMCPY(&cell_sts.cgi, &p_cell_ctxt->cell_id,
							sizeof(son_intra_rat_global_cell_id_t));

					cell_sts.result = SON_FAILURE;
					/* Rel 3.0 Cell delete support changes Start */
					cell_sts.error_code = SON_ERR_TIMER_EXPIRED;
					/* Rel 3.0 Cell delete support changes Stop */
					cell_sts.oam_ind_required =  SON_FALSE;
					cell_sts.deactivation_indication =
						X2_CELL_DEACTIVATION_IND_NULL;

					if (ES_CELL_STATE_SWITCHING_OFF ==
							p_cell_ctxt->current_cell_fsm_state)
					{
						cell_sts.cell_power_state =
							ES_CELL_POWER_STATE_SWITCHING_OFF;
					}
					else if (ES_CELL_STATE_SWITCHING_ON ==
							p_cell_ctxt->current_cell_fsm_state)
					{
						cell_sts.cell_power_state =
							ES_CELL_POWER_STATE_SWITCHING_ON;
					}
					else if (ES_CELL_STATE_SWITCHED_OFF ==
							p_cell_ctxt->current_cell_fsm_state)
					{
						cell_sts.cell_power_state =
							ES_CELL_POWER_STATE_SWITCHED_OFF;
					}

					if (SON_TRUE == es_add_cell_in_switchon_pending_res(
								&cell_sts))
					{
						send_x2_switchon_res = SON_FALSE;
					}
					else
					{
						SON_LOG(log_mode, p_son_es_facility_name,
								SON_ERROR,
								"Cell [0x%x] could not be added to pending "
								"switchon response list due to add failure",
								son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->
									cell_id.cell_identity));

						result = SON_FAILURE;
						error_code = SON_ERR_ES_GENERIC_ERROR;
						break;
					}
				}
			} while (SON_PNULL != (p_cxt_node = es_get_next_node(p_cxt_node)));

			if (es_switchon_pending_res_has_entry())
			{
				if ((SON_FAILURE == result) &&
						(SON_ERR_ES_GENERIC_ERROR == error_code))
				{
					/*
					 *   Atleast adding one entry to pending response structure
					 *   is failed hence send ANR FAILURE response and clean
					 *   Pending switch on structe
					 */
					send_x2_switchon_res = SON_TRUE;
					es_reset_switch_on_pending_res();
				}
				else
				{
					/*
					 *   All the entries have been added succesfully and ES
					 *   shall send switch on request for all those cells whose
					 *   state is switched off otherwise we will just wait for
					 *   existing responses intiated by OAM and whose entries
					 *   we have already added in pending switch on structure
					 */

					p_node = get_first_list_node(
							&g_es_switch_on_pending_res.
							cell_switchon_status_list);

					while (SON_PNULL != p_node)
					{
						p_list_node =  YMEMBEROF(
								cell_switchon_status_list_data_t,
								cell_switchon_status_node,
								p_node);
						p_cell_ctxt = SON_PNULL;
						p_cell_ctxt = es_get_cell_context_from_global_context(
								&p_list_node->cell_switchon_status.cgi);

						if (SON_PNULL != p_cell_ctxt)
						{
							if (ES_CELL_STATE_SWITCHED_OFF ==
									p_cell_ctxt->current_cell_fsm_state)
							{
								req.transaction_id =
									g_es_switch_on_pending_res.transaction_id;

								SON_MEMCPY(&req.switch_on_req.srv_cgi,
										&p_list_node->cell_switchon_status.cgi,
										sizeof(
											son_intra_rat_global_cell_id_t));
								es_cell_process_msg(
										IDX_SONES_CELL_SWITCH_ON_REQ,
										p_cell_ctxt,
										p_cspl_hdr,
										(son_u8 *)&req
										);
							}
						}

						p_node = get_next_list_node(p_node);
					}

					/* Get current time */
					son_waitstruct.walltime(&current_time);

					es_set_switchon_expiry_time(
							current_time, DEFAULT_SON_ES_SWITCH_ON_WAIT);
				}
			}
			else
			{
				/*
				 *  This block means-
				 *  1) Either all the cells are already Switched On(either state
				 *  is ES_DISABLED/ES_DISABLING/ES_ENABLING/ES_ENABLED i.e not
				 *  switched off), OR
				 *  2) Atleast one cell is switched off/switching off or
				 *  switchng on but we are not able to add a single entry in
				 *  Pending Switch on structure
				 */

				if ((SON_FAILURE == result) &&
						(SON_ERR_ES_GENERIC_ERROR == error_code))
				{
					/*
					 *   Adding entry to pending response structure
					 *   is failed hence send ANR FAILURE response and clean
					 *   Pending switch on structe
					 */
					send_x2_switchon_res = SON_TRUE;
					result = SON_FAILURE;
					error_code = SON_ERR_ES_GENERIC_ERROR;

					es_reset_switch_on_pending_res();
				}
				else
				{
					/*
					 * All the Cells are already switched on
					 */
					send_x2_switchon_res = SON_TRUE;
					result = SON_SUCCESS;
					error_code = SON_NO_ERROR;

					es_reset_switch_on_pending_res();
				}
			}
		}
	}

	if (send_x2_switchon_res == SON_TRUE)
	{
		cell_switchon_x2_res.transaction_id =
			p_cell_switchon_x2_req->transaction_id;
		cell_switchon_x2_res.result = result;
		cell_switchon_x2_res.error_code = error_code;
		son_create_send_buffer((son_u8 *)&cell_switchon_x2_res,
				SON_ES_MODULE_ID,
				SON_ANR_MODULE_ID, SONES_CELL_SWITCH_ON_FOR_X2_SETUP_RES,
				sizeof(cell_switchon_x2_res));
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* handle_cell_switchon_for_x2_setup_req */

/* -------------------ES 1.2 Changes Finished ------------------------*/

/*SPR 11689 changes start */
/*****************************************************************************
 * Function Name  :handle_cell_activation_req 
 * Inputs         : p_cspl_hdr  Pointer to the CSPL header
 *                  p_msg        Pointer to the message received
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for
 *                  SONES_CELL_ACTIVATION_REQ message. It shall call
 *                  all the Cell FSMs which are not switched on and also update
 *                  the Pending Switchedon Response structure.
 ******************************************************************************/
static son_buf_retain_status_et
handle_cell_activation_req
(
 STACKAPIHDR    *p_cspl_hdr,
 son_u8          *p_msg
 )
{
	son_es_cell_activation_req_t    *p_cell_activation_req = SON_PNULL;
	son_u32 i =0;
	son_return_et result = SON_FAILURE;
	son_error_et error_code = SON_NO_ERROR;
	es_cell_context_t       *p_cell_ctxt     = SON_PNULL;
	son_oam_log_on_off_et log_mode        = es_get_log_mode();
	son_buf_retain_status_et ret             = RETAIN_SON_BUFFER;
	p_cell_activation_req = (son_es_cell_activation_req_t *)p_msg;
	son_es_cell_activation_resp_t    cell_activation_resp;
	son_cell_switchon_status_t cell_sts;
	/* SPR_19619 start */
	son_time_t current_time;
	/* SPR_19619 stop */


	SON_UT_TRACE_ENTER();

	SON_MEMSET(&cell_activation_resp,0,sizeof(son_es_cell_activation_resp_t));

	if(g_es_context.peer_cell_activation_enabled == SON_TRUE)
	{
		/*traverse the serving cell list needs to be switched on*/
		while(i < p_cell_activation_req->cell_activation_info.num_served_cell)
		{

			p_cell_ctxt = es_get_cell_context_from_global_context(&p_cell_activation_req->
					cell_activation_info.served_cell_list[i]);

			if (SON_PNULL == p_cell_ctxt)
			{
				SON_LOG(log_mode, p_son_es_facility_name,
						SON_ERROR,
						" Invalid cell state[p_cell_ctxt is NULL]");
				i++;
				continue;
			}
			/*check if the cell is switched off*/
			if (ES_CELL_STATE_SWITCHED_OFF ==
					p_cell_ctxt->current_cell_fsm_state )
			{

				if (!es_get_switchon_pending_res_expected_count())
				{
					es_init_switch_on_pending_res(son_generate_txn_id(),
							SONES_CELL_ACTIVATION_REQ);
				}
				SON_MEMSET(&cell_sts, 0, sizeof(son_cell_switchon_status_t));
				SON_MEMCPY(&cell_sts.cgi, &p_cell_ctxt->cell_id,
						sizeof(son_intra_rat_global_cell_id_t));
				cell_sts.error_code = SON_NO_ERROR;
				cell_sts.cell_power_state = ES_CELL_POWER_STATE_SWITCHED_OFF;
				cell_sts.oam_ind_required = SON_TRUE;
				cell_sts.result = SON_SUCCESS;

				if (SON_TRUE == es_add_cell_in_switchon_pending_res(&cell_sts))
				{
					p_cell_ctxt->cell_activation_req_flag = SON_TRUE;
					/*save target enb Id*/
					SON_MEMCPY(&p_cell_ctxt->enbid_1,
							&p_cell_activation_req->enbid_1,
							sizeof(son_global_enb_id_t));
					SON_MEMCPY(&p_cell_ctxt->enbid_2,
							&p_cell_activation_req->enbid_2,
							sizeof(son_global_enb_id_t));

					result = SON_SUCCESS;
					es_cell_process_msg(
							IDX_SONES_CELL_ACTIVATION_REQ,
							p_cell_ctxt,
							p_cspl_hdr,
							(son_u8 *)&p_msg
							);
				}

			}
			/*if any other state return failure from here*/
			else
			{                    
				SON_LOG(log_mode, p_son_es_facility_name,
						SON_ERROR,
						" Invalid cell state [%d]", ES_CELL_STATE_SWITCHED_OFF);

			}    

			i++;
		}/* while end */
	}
	else
	{
		SON_LOG(log_mode, p_son_es_facility_name,
				SON_ERROR,
				" cell activation from peer is disabled by OAM");
	}

	/*if none of the cells are in switched-off state, send failure response*/
	if((SON_FAILURE == result)) 
	{
		cell_activation_resp.enbid_1 = p_cell_activation_req->enbid_1;
        /* SPR 19838 : Coverity 12095 fix start */
		cell_activation_resp.enbid_2 = p_cell_activation_req->enbid_2;
        /* SPR 19838 : Coverity 12095 fix end */
		cell_activation_resp.result = result;
		cell_activation_resp.error_code = error_code;
		son_create_send_buffer((son_u8 *)&cell_activation_resp,
				SON_ES_MODULE_ID,
				SON_X2_MODULE_ID,SONES_CELL_ACTIVATION_RESP ,
				sizeof(cell_activation_resp));
	}
	/* SPR_19619 start */
	/* Get current time */
	son_waitstruct.walltime(&current_time);

	es_set_switchon_expiry_time(
			current_time, 7);

	/* SPR_19619 stop */

	return ret;

} /* handle_cell_activation_req */

/* SPR_19619 start */

/*****************************************************************************
 * Function Name  : handle_peer_cell_activation_resp 
 * Inputs         : p_cspl_hdr  Pointer to the CSPL header
 *                  p_msg        Pointer to the message received
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for
 *                  SONX2_PEER_CELL_ACTIVATION_RESP message.
 ******************************************************************************/
static son_buf_retain_status_et
handle_peer_cell_activation_resp
(
 STACKAPIHDR    *p_cspl_hdr,
 son_u8          *p_msg
 )
{
    son_es_cell_act_resp_t *p_resp = (son_es_cell_act_resp_t *)p_msg;
    son_es_peer_cell_activation_resp_t p_cell_activation_response = {0};

    SON_UT_TRACE_ENTER();


    if (p_resp->result== 1)
    {
        p_cell_activation_response.peer_cell_activation_resp.result  = SON_SUCCESS;
        p_cell_activation_response.peer_cell_activation_resp.error_code = SON_NO_ERROR;
    }

    else
    {
        p_cell_activation_response.peer_cell_activation_resp.result  = SON_FAILURE;
        p_cell_activation_response.peer_cell_activation_resp.error_code = p_resp->error_code;
    }
    p_cell_activation_response.transaction_id = p_resp->transaction_id;
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);

/* coverity_109484 start */
    son_create_send_buffer((son_u8 *)&p_cell_activation_response, SON_ES_MODULE_ID,
            SON_MIF_MODULE_ID, SMIF_PEER_CELL_ACTIVATION_RESP,
            sizeof(son_es_peer_cell_activation_resp_t));
/* coverity_109484 stop */

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* handle_peer_cell_activation_resp */


/*****************************************************************************
 * Function Name  : es_peer_cell_activation_req_handler
 * Inputs         : p_cspl_hdr  Pointer to the CSPL header
 *                   p_msg        Pointer to the message received
 *                   event_id     event id
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ES module and is
 *					invoked when SONES_PEER_CELL_ACTIVATION_REQ message is received.
 *****************************************************************************/
static son_buf_retain_status_et
es_peer_cell_activation_req_handler
(
 STACKAPIHDR        *p_cspl_hdr,
 son_u8             *p_msg
 )
{
    son_x2_peer_cell_activation_req_t   *p_peer_cell_activation_req = (son_x2_peer_cell_activation_req_t  *)p_msg;
    smif_peer_cell_activation_res_t     cell_activation_res; 
    son_u32 index = 0;
    es_cell_context_node_t              *p_ctxt_node         = SON_PNULL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&cell_activation_res, 0, sizeof(cell_activation_res));

    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);

    /* Coverity 109699 + */
    if (es_get_context_cell_count())
    {
         p_ctxt_node = es_get_first_cell_ctxt();

         /*traverse the serving cell list */
         for(index =0;index< p_peer_cell_activation_req->peer_cell_activation_req.num_served_cell;index++)   //CID 109699
         {
    /* Coverity 109699 - */
            if (SON_PNULL != p_ctxt_node)
            {
                /* Check if the FSM State of Cell is ENABLED */
                if (ES_CELL_STATE_ENABLED ==
                        p_ctxt_node->data.current_cell_fsm_state )
                {
                    son_create_send_buffer((son_u8 *)p_peer_cell_activation_req, 
                            SON_ES_MODULE_ID, SON_X2_MODULE_ID, SONX2_PEER_CELL_ACTIVATION_REQ,
                            sizeof(son_x2_peer_cell_activation_req_t));

                }
                else
                {
                    SON_LOG(es_get_log_mode(), p_son_es_facility_name,
                            SON_INFO,
                            "Cell FSM State is not ENABLED");

                    cell_activation_res.transaction_id = p_peer_cell_activation_req->transaction_id;
                    cell_activation_res.result = SON_FAILURE;
                    cell_activation_res.error_code = SON_ERR_CELL_ACTIVATION_FAILED;

                    /* Send cell activation failure resp to mif */
                    son_create_send_buffer((son_u8 *)&cell_activation_res,
                            SON_ES_MODULE_ID,
                            SON_MIF_MODULE_ID,
                            SMIF_PEER_CELL_ACTIVATION_RESP,
                            sizeof(cell_activation_res));
                }
                p_ctxt_node = es_get_next_node(p_ctxt_node);
            }
        }
    }
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /*es_peer_cell_activation_req_handler*/

/* SPR_19619 stop */



/*SPR11689 changes end */
/*****************************************************************************
 * Function Name  : es_process_smif_msgs
 * Inputs         : p_cspl_hdr  Pointer to the CSPL header
 *                  p_msg        Pointer to the message received
 *					p_cell_id	 Pointer to Cell ID
 *					event_id	 event id
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ES module and is
 *					called for any message received in state other than ACTIVE.
 ******************************************************************************/
static son_buf_retain_status_et
es_process_smif_msgs
(
 STACKAPIHDR                     *p_cspl_hdr,
 son_u8                                  *p_msg,
 son_intra_rat_global_cell_id_t  *p_cell_id,
 son_es_index_et event_id
 )
{
	son_buf_retain_status_et ret             = RETAIN_SON_BUFFER;
	es_cell_context_t               *p_cell_ctxt        = SON_PNULL;

	SON_UT_TRACE_ENTER();

	switch (p_cspl_hdr->api)
	{
		case SONES_ENABLE_REQ:
			{
				if (SON_ES_ENABLE_SRV_CGI_PRESENT !=
						((son_es_enable_req_t *)p_msg)->enable_req.bitmask)
				{
					/*Buffer gets released so ret is RELEASE_SON_BUFFER*/
					ret = es_global_enable_req_handler(p_cspl_hdr, p_msg, event_id);
				}

				break;
			}

		case SONES_DISABLE_REQ:
			{
				if (SON_ES_DISABLE_SRV_CGI_PRESENT !=
						((son_es_disable_req_t *)p_msg)->disable_req.bitmask)
				{
					/*Buffer gets released so ret is RELEASE_SON_BUFFER*/
					ret = es_global_disable_req_handler(p_cspl_hdr, p_msg, event_id);
				}

				break;
			}

		case SONES_CELL_SWITCH_ON_REQ:
			{
				/*
				 *   If cell switch on is already initiated by ANR or X2 then
				 *   shall respond OAM with error code as
				 *   Switch_on_alreay_initiated_by_anr_or_x2
				 */
				son_es_cell_switch_on_req_t *p_req =
					(son_es_cell_switch_on_req_t *)p_msg;
				if (son_get_x2_enabled() &&
						(SON_PNULL != es_find_cell_in_switchon_pending_res(
												   &p_req->switch_on_req.srv_cgi)))
				{
					son_create_and_send_cell_switch_on_resp_to_oam(
							p_req->transaction_id,
							&p_req->switch_on_req.srv_cgi,
							SON_FAILURE,
							SON_ERR_ES_CELL_SWITCHON_FOR_X2_IN_PROGRESS);
					return RELEASE_SON_BUFFER;
				}

				break;
			}

		default:
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
						"Cell specific message received");
				break;
			}
	} /* switch */

	if (RETAIN_SON_BUFFER == ret)
	{
		p_cell_ctxt = es_get_cell_context_from_global_context(p_cell_id);

		if (!p_cell_ctxt)
		{
			SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
					"Cell context not found! for Msg:"
					"%s", SON_ES_CELL_FSM_EVENT_NAMES[event_id] );

			es_create_send_generic_nack(
					p_msg,
					p_cspl_hdr,
					p_cell_id,
					SON_ERR_CELL_UNCONFIGURED);
			ret  = RELEASE_SON_BUFFER;
		}
		else
		{
			ret = es_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, p_msg);
		}
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* es_process_smif_msgs */

/*****************************************************************************
 * Function Name  : es_process_rrm_msgs
 * Inputs         : p_msg       Pointer to the message received
 *					p_cell_id	Pointer to Cell ID
 *                   p_cspl_hdr  Pointer to the CSPL header
 *					event_id	event id
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This function handle messages from RRM module.
 *****************************************************************************/
static son_buf_retain_status_et
es_process_rrm_msgs
(
 son_u8                                              *p_msg,
 son_intra_rat_global_cell_id_t  *p_cell_id,
 STACKAPIHDR                     *p_cspl_hdr,
 son_es_index_et event_id
 )
{
	es_cell_context_t                   *p_cell_ctxt      = SON_PNULL;
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
			SON_LOG(es_get_log_mode(), p_son_es_facility_name,
					SON_INFO,
					"RRM_SON_DEREGISTER_RESP "
					"received for the Cell 0x%x already deleted "
					"at ES due to cell delete request from OAM  so ignore it.",
					son_convert_char_cell_id_to_int_cell_id(
						p_res->res.deregistration_resp.id.object_id.
						cell_id.cell_identity));
			SON_UT_TRACE_EXIT();
			return RELEASE_SON_BUFFER;
		}

	}
	/* Rel 3.0 Cell delete support changes Stop */

	p_cell_ctxt = es_get_cell_context_from_global_context(p_cell_id);
	if (p_cell_ctxt)
	{
		ret = es_cell_process_msg(event_id, p_cell_ctxt, p_cspl_hdr, p_msg);
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* es_process_rrm_msgs */

/*****************************************************************************
 * Function Name  : es_shutdown_handler
 * Inputs         : p_msg       Pointer to the message received
 *                                       p_cspl_hdr Pointer to cspl stack header
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ES module and is
 *                   called when SMIF_SHUTDOWN_REQ message is received.
 *****************************************************************************/
son_buf_retain_status_et
es_shutdown_handler
(
 son_u8                 *p_msg,
 STACKAPIHDR    *p_cspl_hdr
 )
{
	es_cell_context_node_t *p_ctxt_node         = SON_PNULL;
	smif_shutdown_res_t shut_down_res           = {0};

	SON_UT_TRACE_ENTER();

	SON_LOG(es_get_log_mode(), p_son_es_facility_name,
			SON_INFO, "MIF->ES:SMIF_SHUTDOWN_REQ");
	if (es_get_context_cell_count())
	{
		p_ctxt_node = es_get_first_cell_ctxt();

		while (SON_PNULL != p_ctxt_node)
		{
			if (ES_CELL_STATE_ENABLING ==
					p_ctxt_node->data.current_cell_fsm_state ||
					ES_CELL_STATE_ENABLED ==
					p_ctxt_node->data.current_cell_fsm_state)
			{
				if (SON_FAILURE == es_construct_send_rrm_deregister_req((
								(smif_shutdown_req_t *)p_msg)->transaction_id,
							&p_ctxt_node->data.cell_id))
				{
					SON_LOG(es_get_log_mode(),
							p_son_es_facility_name,
							SON_DETAILED,
							"Failed to send RRM_DEREGISTER_REQ"
							"for Cell 0x%x",
							son_convert_char_cell_id_to_int_cell_id(p_ctxt_node->data.cell_id.cell_identity));
					LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                            /*SPR 17777 +-*/
				}
			}

			p_ctxt_node = es_get_next_node(p_ctxt_node);
		}

		/*Clean up memory of each Cell Context */
		es_clear_context_list();

		es_clear_pending_res_list();
		/* -------------------ES 1.2 Changes Start ------------------------*/
		es_reset_switch_on_pending_res();
		/* -------------------ES 1.2 Changes End ------------------------*/
	}

	es_set_context_state(ES_STATE_SHUTDOWN);

	/* Send SMIF_SHUTDOWN_RES to SONMgmtIfH*/

	shut_down_res.transaction_id = *((son_u16 *)(p_msg));
	shut_down_res.result         = SON_SUCCESS;
	shut_down_res.error_code     = SON_NO_ERROR;

	son_create_send_buffer((son_u8 *)&shut_down_res, SON_ES_MODULE_ID,
			SON_MIF_MODULE_ID, SMIF_SHUTDOWN_RES,
			sizeof(shut_down_res));
	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* es_shutdown_handler */

/* AUTONOMOUS SWITCH OFF START */

/*****************************************************************************
 * Function Name  : son_es_send_autonomous_switch_off_config_res_to_mif
 * Inputs         : p_out_msg    Pointer to the message to be sent to MIF
 *                : transaction_id Transaction_id recvd in Req from MIF
 *                : result       Result of the request
 *                : error_code   Error code in case of Failure
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends the SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_RESP
 *                  message to MIF in resp to SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ
 ******************************************************************************/
static void
son_es_send_autonomous_switch_off_config_res_to_mif
(
 son_es_autonomous_switch_off_config_resp_t  *p_out_msg,
 son_u16 transaction_id,
 son_return_et result,
 son_error_et error_code
 )
{
	SON_UT_TRACE_ENTER();

	p_out_msg->transaction_id = transaction_id;
	p_out_msg->autonomous_switch_off_config_resp.result = result;
	p_out_msg->autonomous_switch_off_config_resp.error_code = error_code;

	son_create_send_buffer((son_u8 *)p_out_msg,
			SON_ES_MODULE_ID,
			SON_MIF_MODULE_ID,
			SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_RESP,
			sizeof(son_es_autonomous_switch_off_config_resp_t));

	SON_UT_TRACE_EXIT();
}

/* AUTONOMOUS SWITCH OFF END */

/*****************************************************************************
 * Function Name  : es_init_state_handler
 * Inputs         : p_msg        Pointer to the message received
 *				   p_cspl_hdr Pointer to cspl stack header
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ES module and is called for
 *				   any message received in INIT State
 ******************************************************************************/
son_buf_retain_status_et
es_init_state_handler
(
 son_u8                  *p_msg,
 STACKAPIHDR             *p_cspl_hdr
 )
{
	smif_init_config_res_t init_config_res = {0};
	son_u8 length  = 0;
	son_procedure_code_et api     = SON_PROCEDURE_UNDEFINED;
	son_intra_rat_global_cell_id_t      *p_cell_id = SON_PNULL;

	SON_UT_TRACE_ENTER();
	if (SMIF_INIT_CONFIG_REQ == p_cspl_hdr->api)
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
				"MIF->ES:SMIF_INIT_CONFIG_REQ");

		/*Call log level handler*/
		if (RELEASE_SON_BUFFER != es_log_handler(p_cspl_hdr, p_msg))
		{
			SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
					"es_log_handler requested not to release the memory");
		}
		es_set_context_state(ES_STATE_ACTIVE);

		api     = SMIF_INIT_CONFIG_RES;
		init_config_res.transaction_id = *((son_u16 *)p_msg);
		init_config_res.result = SON_SUCCESS;
		init_config_res.error_code = SON_NO_ERROR;
		length = sizeof(smif_init_config_res_t);

		son_create_send_buffer((son_u8 *)&init_config_res, SON_ES_MODULE_ID,
				(son_module_id_et)(p_cspl_hdr->from), api,
				length );
	}
	else if (((p_cspl_hdr->api >= SONES_REQ_BASE_API_ID) &&
				(p_cspl_hdr->api <= SONES_REQ_MAX_API)))
	{
		p_cell_id = es_get_cell_id_from_msg(p_cspl_hdr->from,
				p_cspl_hdr->api,
				p_msg);
		es_create_send_generic_nack(p_msg, p_cspl_hdr, p_cell_id,
				SON_ERR_UNEXPECTED_MSG);
	}
	else if (((p_cspl_hdr->api >= SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ) &&
				(p_cspl_hdr->api <= SONES_X2_REQ_MAX_API)))
	{
		es_create_send_generic_nack(p_msg, p_cspl_hdr, SON_PNULL,
				SON_ERR_UNEXPECTED_MSG);
	}

	else
	{
		/*
		 * **Handle all the cases where SONES do not send any response,
		 ** just log error and release buffer.
		 */
		SON_LOG(es_get_log_mode(),
				p_son_es_facility_name,
				SON_ERROR,
				"Unexpected Message [%d] Received from module [%d]",
				p_cspl_hdr->api, p_cspl_hdr->from);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* es_init_state_handler */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_START */

/*****************************************************************************
 * Function Name  : es_parse_rrm_messages
 * Inputs         : p_msg        Pointer to the message received
 *                : api_id       api_id
 * Outputs        : None
 * Returns        : son_u8* - pointer to the parsed message
 * Description    : This is function to parse the messages received from RRM
 ******************************************************************************/
static son_u8 *
es_parse_rrm_messages
(
 son_u8 *p_msg,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	son_s32 buf_len = 0;
	son_bool_et txn_id_available = SON_TRUE;
	S32 length_read = 0;

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
				if (SON_PNULL != p_parsed_msg &&
						RRM_FAILURE == rrm_parse_rrm_son_registration_res(
							&((son_rrm_registration_res_t *)p_parsed_msg)->
							res,
							(p_msg + EXT_MSG_API_HDR_SIZE),
							buf_len - EXT_MSG_API_HDR_SIZE,
							&length_read))
				{
                    SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
                            "RRM->ES:RRM_SON_REGISTER_RESP parsing failed");
					son_mem_free(p_parsed_msg);
					p_parsed_msg = SON_PNULL;
					txn_id_available = SON_FALSE;
				}
                else
                {
                    SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
                            "RRM->ES:RRM_SON_REGISTER_RESP");
                }

				break;
			}

		case RRM_SON_DEREGISTER_RESP:
			{
				p_parsed_msg =
					son_mem_get(sizeof(son_rrm_deregistration_res_t));
				if (SON_PNULL != p_parsed_msg &&
						RRM_FAILURE == rrm_parse_rrm_son_deregistration_res(
							&((son_rrm_deregistration_res_t *)p_parsed_msg)->res,
							(p_msg + EXT_MSG_API_HDR_SIZE),
							buf_len - EXT_MSG_API_HDR_SIZE,
							&length_read))
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
							"RRM->ES:RRM_SON_DEREGISTER_RESP parsing failed");
					son_mem_free(p_parsed_msg);
					p_parsed_msg = SON_PNULL;
					txn_id_available = SON_FALSE;
				}
				else
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"RRM->ES:RRM_SON_DEREGISTER_RESP");
				}

				break;
			}

		case RRM_SON_SET_ATTRIBUTE_RESP:
			{
				p_parsed_msg = son_mem_get(sizeof(son_rrm_set_attribute_res_t));
				if (SON_PNULL != p_parsed_msg &&
						RRM_FAILURE == rrm_parse_rrm_son_set_attr_res(
							&((son_rrm_set_attribute_res_t *)p_parsed_msg)->
							res,
							(p_msg + EXT_MSG_API_HDR_SIZE),
							buf_len - EXT_MSG_API_HDR_SIZE,
							&length_read))
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
							"RRM->ES:RRM_SON_SET_ATTRIBUTE_RESP parsing failed");
					son_mem_free(p_parsed_msg);
					p_parsed_msg = SON_PNULL;
					txn_id_available = SON_FALSE;
				}
				else
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"RRM->ES:RRM_SON_SET_ATTRIBUTE_RESP");
				}

				break;
			}

		case RRM_SON_CELL_STATE_CHANGE_IND:
			{
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
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
							"RRM->ES:RRM_SON_CELL_STATE_CHANGE_IND parsing failed");
					son_mem_free(p_parsed_msg);
					p_parsed_msg = SON_PNULL;
				}
				else
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"RRM->ES:RRM_SON_CELL_STATE_CHANGE_IND");
				}

				break;
			}

		case RRM_SON_CELL_SWITCH_ON_RESP:
			{
				p_parsed_msg = son_mem_get(sizeof(son_es_cell_switch_on_res_t));
				if (SON_PNULL != p_parsed_msg &&
						RRM_FAILURE == rrm_parse_rrm_son_cell_switch_on_res(
							(rrm_son_cell_switch_on_res_t *)
							&((son_es_cell_switch_on_res_t *)p_parsed_msg)->
							generic_res,
							(p_msg + EXT_MSG_API_HDR_SIZE),
							buf_len - EXT_MSG_API_HDR_SIZE,
							&length_read))
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
							"RRM->ES:RRM_SON_CELL_SWITCH_ON_RESP parsing failed");
					son_mem_free(p_parsed_msg);
					p_parsed_msg = SON_PNULL;
					txn_id_available = SON_FALSE;
				}
				else
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"RRM->ES:RRM_SON_CELL_SWITCH_ON_RESP");
				}

				break;
			}

		case RRM_SON_CELL_SWITCH_OFF_RESP:
			{
				p_parsed_msg = son_mem_get(sizeof(son_es_cell_switch_off_res_t));
				if (SON_PNULL != p_parsed_msg  &&
						RRM_FAILURE == rrm_parse_rrm_son_cell_switch_off_res(
							(rrm_son_cell_switch_off_res_t *)
							&((son_es_cell_switch_off_res_t *)p_parsed_msg)->
							generic_res,
							(p_msg + EXT_MSG_API_HDR_SIZE),
							buf_len - EXT_MSG_API_HDR_SIZE,
							&length_read))
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
							"RRM->ES:RRM_SON_REGISTER_RESP parsing failed");
					son_mem_free(p_parsed_msg);
					p_parsed_msg = SON_PNULL;
					txn_id_available = SON_FALSE;
				}
				else
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"RRM->ES:RRM_SON_REGISTER_RESP");
				}

				break;
			}

		case RRM_SON_NUM_ACTIVE_UE_REPORT:
			{
				txn_id_available = SON_FALSE;
				p_parsed_msg = son_mem_get(
						sizeof(son_es_active_ue_cnt_report_t));
				if (SON_PNULL != p_parsed_msg &&
						RRM_FAILURE == rrm_parse_rrm_son_active_ue_cnt_report(
							(son_es_active_ue_cnt_report_t *)p_parsed_msg,
							(p_msg + EXT_MSG_API_HDR_SIZE),
							buf_len - EXT_MSG_API_HDR_SIZE,
							&length_read))
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
							"RRM->ES:RRM_SON_NUM_ACTIVE_UE_REPORT parsing failed");
					son_mem_free(p_parsed_msg);
					p_parsed_msg = SON_PNULL;
				}
				else
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"RRM->ES:RRM_SON_NUM_ACTIVE_UE_REPORT");
				}

				break;
			}

		case RRM_UE_COUNT_THRESHOLD_REACHED_IND:
			{
				txn_id_available = SON_FALSE;
				p_parsed_msg = son_mem_get(
						sizeof(son_es_threshold_hit_ind_t));
				if (SON_PNULL != p_parsed_msg &&
						RRM_FAILURE == rrm_parse_rrm_ue_count_threshold_hit_ind(
							(son_es_threshold_hit_ind_t *)p_parsed_msg,
							(p_msg + EXT_MSG_API_HDR_SIZE),
							buf_len - EXT_MSG_API_HDR_SIZE,
							&length_read))
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
							"RRM->ES:RRM_UE_COUNT_THRESHOLD_REACHED_IND parsing failed");
					son_mem_free(p_parsed_msg);
					p_parsed_msg = SON_PNULL;
				}
				else
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"RRM->ES:RRM_UE_COUNT_THRESHOLD_REACHED_IND");
				}

				break;
			}

			/* AUTONOMOUS SWITCH OFF START */
		case RRM_SON_LOAD_REPORT_IND:
			{
				txn_id_available = SON_FALSE;
				p_parsed_msg = son_mem_get(sizeof(son_es_load_report_ind_t));
				if (SON_PNULL != p_parsed_msg &&
						(RRM_FAILURE == rrm_parse_rrm_son_load_report_ind(
												  (son_es_load_report_ind_t *)p_parsed_msg,
												  (p_msg + EXT_MSG_API_HDR_SIZE),
												  buf_len - EXT_MSG_API_HDR_SIZE,
												  &length_read)))
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
							"RRM->ES:RRM_SON_LOAD_REPORT_IND parsing failed");
					son_mem_free(p_parsed_msg);
					p_parsed_msg = SON_PNULL;
				}
				else
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
							"RRM->ES:RRM_SON_LOAD_REPORT_IND");
				}

				break;
			}

			/* AUTONOMOUS SWITCH OFF END */
		default:
			{
				SON_LOG(es_get_log_mode(),
						p_son_es_facility_name, SON_ERROR,
						"Unexpected Message [%u]"
						" received from RRM", api_id);
				break;
			}
	} /* switch */

	if (SON_PNULL != p_parsed_msg && SON_TRUE == txn_id_available)
	{
		*(son_u16 *)p_parsed_msg = son_get_word_from_header(
				&p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
	}

	return p_parsed_msg;
} /* es_parse_rrm_messages */

/* AUTONOMOUS SWITCH OFF START */

/*****************************************************************************
 * Function Name  : son_es_set_global_config_for_autonomous_switch_off
 * Inputs         : p_autonomous_switch_off_config_req
 *                           Pointer to the autonomous_switch_off_config_req
 *                           received from MIF
 * Outputs        : None
 * Returns        : None
 * Description    : This function sets the configuration received in
 *                  Autonomous Switch OFF Config Req from MIF in the ES Global context.
 ******************************************************************************/
static void
son_es_set_global_config_for_autonomous_switch_off
(
 son_oam_es_autonomous_switch_off_config_req_t   *p_autonomous_switch_off_config_req
 )
{
	SON_UT_TRACE_ENTER();

	g_es_context.g_autonomous_switch_off_info = SON_NULL;
	g_es_context.g_load_level_threshold = SON_NULL;

	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
			"Setting GLOBAL configuration for Autonomous Switch Off");

	g_es_context.g_autonomous_switch_off_info |= SON_ES_AUTONOMOUS_SWITCH_OFF_ENABLED;

	/* Start: SPR 8724 */
	g_es_context.autonomous_switch_off_timer_val =
		p_autonomous_switch_off_config_req->timer_duration;
	/* End: SPR 8724 */

	/* Low Load Level is the criteria for Autonomous cell switch Off */
	if (p_autonomous_switch_off_config_req->bitmask
			& SON_OAM_AUTO_SWITCH_OFF_LOAD_LEVEL)
	{
		g_es_context.g_autonomous_switch_off_info |= SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_LOAD_LEVEL;
		g_es_context.g_load_level_threshold = p_autonomous_switch_off_config_req->load_level_threshold;

		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
				"Set bitmask for checking Load Level and set threshold value = [%d]",
				g_es_context.g_load_level_threshold);
	}

	/* Less Active UEs is the criteria for Autonomous cell switch Off */
	if (p_autonomous_switch_off_config_req->bitmask
			& SON_OAM_AUTO_SWITCH_OFF_UE_COUNT)
	{
		g_es_context.g_autonomous_switch_off_info |= SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_UE_CNT;
		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
				"Set bitmask for checking Active UE count");
	}
	es_print_global_context("son_es_set_global_config_for_autonomous_switch_off");

	SON_UT_TRACE_EXIT();
} /* son_es_set_global_config_for_autonomous_switch_off */

/*****************************************************************************
 * Function Name  : son_es_set_config_for_autonomous_switch_off
 * Inputs         : p_cell_ctxt     Pointer to the cell ctxt
 *                : p_autonomous_switch_off_config_req
 *                                  Pointer to the autonomous_switch_off_config_req
 *                                  received from MIF
 * Outputs        : None
 * Returns        : None
 * Description    : This function sets the configuration received in
 *                  Autonomous Switch OFF Config Req from MIF in the cell context.
 ******************************************************************************/
static void
son_es_set_config_for_autonomous_switch_off
(
 es_cell_context_t                               *p_cell_ctxt,
 son_oam_es_autonomous_switch_off_config_req_t   *p_autonomous_switch_off_config_req
 )
{
	SON_UT_TRACE_ENTER();

	p_cell_ctxt->autonomous_switch_off_info = SON_NULL;
	p_cell_ctxt->load_level_threshold = SON_NULL;

	p_cell_ctxt->autonomous_switch_off_info |= SON_ES_AUTONOMOUS_SWITCH_OFF_ENABLED;

	/* Low Load Level is the criteria for Autonomous cell switch Off */
	if (p_autonomous_switch_off_config_req->bitmask
			& SON_OAM_AUTO_SWITCH_OFF_LOAD_LEVEL)
	{
		p_cell_ctxt->autonomous_switch_off_info |= SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_LOAD_LEVEL;
		p_cell_ctxt->load_level_threshold = p_autonomous_switch_off_config_req->load_level_threshold;

		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
				"Set bitmask for checking Load Level and set threshold value = [%d]",
				p_cell_ctxt->load_level_threshold);
	}

	/* Less Active UEs is the criteria for Autonomous cell switch Off */
	if (p_autonomous_switch_off_config_req->bitmask
			& SON_OAM_AUTO_SWITCH_OFF_UE_COUNT)
	{
		p_cell_ctxt->autonomous_switch_off_info |= SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_UE_CNT;
		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
				"Set bitmask for checking Active UE count");
	}
	es_print_cell_context(p_cell_ctxt);


	SON_UT_TRACE_EXIT();
} /* son_es_set_config_for_autonomous_switch_off */

/*****************************************************************************
 * Function Name  : son_es_create_autonomous_switch_off_resp_error_list
 * Inputs         : p_cell_id     Pointer to the cell ID
 *                : error_code    Error Code for the failure
 *                : p_out_msg       Pointer to the autonomous_switch_off_config_resp
 *                                  to be sent to MIF
 * Outputs        : p_out_msg       Pointer to the autonomous_switch_off_config_resp
 *                                  to be sent to MIF
 * Returns        : None
 * Description    : This function fills the Autonomous Switch OFF Config Res
 *                  message error list
 ******************************************************************************/
static son_void_t
son_es_create_autonomous_switch_off_resp_error_list
(
 son_intra_rat_global_cell_id_t                  *p_cell_id,
 son_error_et error_code,
 son_es_autonomous_switch_off_config_resp_t      *p_out_msg
 )
{
	SON_UT_TRACE_ENTER();

	/* Add cell in error list */
	SON_MEMCPY(&p_out_msg->autonomous_switch_off_config_resp.cell_status_list
			[p_out_msg->autonomous_switch_off_config_resp.cell_status_list_size].cgi,
			p_cell_id, sizeof(son_intra_rat_global_cell_id_t));

	p_out_msg->autonomous_switch_off_config_resp.cell_status_list
		[p_out_msg->autonomous_switch_off_config_resp.cell_status_list_size].error_code =
		error_code;

	p_out_msg->autonomous_switch_off_config_resp.cell_status_list_size++;

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_es_autonomous_switch_off_config_req_handler
 * Inputs         : p_cspl_hdr Pointer to cspl stack header
 *                : p_msg        Pointer to the message received
 * Outputs        : None
 * Returns        : None
 * Description    : This is the message handler for Autonomous Switch Off Config
 *                  Request reveived by ES from MIF in ES states ES_STATE_ACTIVE
 *                  or ES_STATE_ENABLING
 *
 ******************************************************************************/
static void
son_es_autonomous_switch_off_config_req_handler
(
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_msg
 )
{
	son_return_et result     = SON_SUCCESS;
	son_error_et error_code = SON_NO_ERROR;
	son_u16 cell_count = SON_NULL;
	son_u16 cell_index = SON_NULL;
	es_cell_context_t       *p_cell_ctxt      = SON_PNULL;
	es_cell_context_node_t  *p_cell_ctxt_node = SON_PNULL;
	son_es_autonomous_switch_off_config_resp_t out_msg = {0};

	SON_UT_TRACE_ENTER();

	son_es_autonomous_switch_off_config_req_t   *p_autonomous_switch_off_config_req =
		(son_es_autonomous_switch_off_config_req_t *)p_msg;

	cell_count = p_autonomous_switch_off_config_req->autonomous_switch_off_config_req.
		src_cgi_list_size;

	/* At least 1 criteria needs to be specified Load Level or active UE count */
	if ((!(SON_OAM_AUTO_SWITCH_OFF_LOAD_LEVEL &
					p_autonomous_switch_off_config_req->autonomous_switch_off_config_req.bitmask))
			&& (!(SON_OAM_AUTO_SWITCH_OFF_UE_COUNT &
					p_autonomous_switch_off_config_req->autonomous_switch_off_config_req.bitmask)))
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
				"No Criteria specified for triggering Autonomous Cell Switch Off."
				"Sending FAILURE resp to MIF;p_cspl_hdr=%p",p_cspl_hdr);
                /*SPR 17777 +-*/
		result = SON_FAILURE;
		error_code = SON_ERR_INVALID_PARAMS;
	}
	/* Global configuration received */
	else if (SON_NULL == cell_count)
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
				"GLOBAL Req for Autonomous Cell Switch Off");

		/*Pointer to the first context node*/
		p_cell_ctxt_node = es_get_first_cell_ctxt();

		if (SON_PNULL == p_cell_ctxt_node)
		{
			SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
					"No cell configured");
		}

		while (SON_PNULL != p_cell_ctxt_node)
		{
			/*Get the data part of the node*/
			p_cell_ctxt = &p_cell_ctxt_node->data;

			/* Set Config */
			son_es_set_config_for_autonomous_switch_off(p_cell_ctxt,
					&p_autonomous_switch_off_config_req->autonomous_switch_off_config_req);
			p_cell_ctxt_node = es_get_next_node(p_cell_ctxt_node);
		}

		/* Set autonomous switch off configuration in GLOBAL Context */
		son_es_set_global_config_for_autonomous_switch_off(
				&p_autonomous_switch_off_config_req->autonomous_switch_off_config_req);
	}
	else
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
				"Autonomous Switch Off Req received for %u cells", cell_count);

		/* Process request for each cell specified in the cell list */
		for (cell_index = SON_NULL; ((cell_index < cell_count)
					&& (cell_index < SON_MAX_CELL_SUPPORTED_AT_ENB)); 
				cell_index++)
		{
			p_cell_ctxt = es_get_cell_context_from_global_context(&p_autonomous_switch_off_config_req->
					autonomous_switch_off_config_req.cell_id[cell_index]);

			/* If cell not found in the global context list */
			if (SON_PNULL == p_cell_ctxt)
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
						"NO cell configured with Cell Id 0x%x "
						"Adding Cell in Error list of Autonomous Switch Off Resp",
						son_convert_char_cell_id_to_int_cell_id(p_autonomous_switch_off_config_req->
							autonomous_switch_off_config_req.cell_id[cell_index].cell_identity));

				son_es_create_autonomous_switch_off_resp_error_list(&p_autonomous_switch_off_config_req->
						autonomous_switch_off_config_req.cell_id[cell_index], SON_ERR_CELL_UNCONFIGURED, &out_msg);

				result = SON_FAILURE;
			}
			/* If Cell is found, set autonomous switch off configuration */
			else
			{
				/* Set Config */
				son_es_set_config_for_autonomous_switch_off(p_cell_ctxt,
						&p_autonomous_switch_off_config_req->autonomous_switch_off_config_req);
				/* Start: SPR 8724 */
				g_es_context.autonomous_switch_off_timer_val = 
					p_autonomous_switch_off_config_req->
					autonomous_switch_off_config_req.timer_duration;
				/* End: SPR 8724 */
			}
		}
	}

	/* Send consolidated resp to MIF. out_msg contains empty cell list in case of Success
	 *  or global Failure and Non-empty in case of cell specific Failures */
	son_es_send_autonomous_switch_off_config_res_to_mif(&out_msg,
			p_autonomous_switch_off_config_req->transaction_id, result, error_code);

	SON_UT_TRACE_EXIT();
} /* son_es_autonomous_switch_off_config_req_handler */

/* AUTONOMOUS SWITCH OFF END */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_STOP */

/* Rel 3.0 Cell delete support changes Start */

/******************************************************************************
 * Function Name  : es_cell_delete_req_handler
 * Inputs         : p_cspl_hdr   Pointer to the CSPL Header
 *                : p_msg        Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for ES module and is
 *                  invoked when SMIF_CELL_DELETE_REQ message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
es_cell_delete_req_handler
(
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_buf
 )
{
	smif_cell_delete_req_t   *p_msg          = (smif_cell_delete_req_t *)p_buf;
	es_cell_context_t       *p_cell_ctxt    = SON_PNULL;
	son_u8                  idx             = 0;
	smif_cell_delete_resp_t  cell_del_res;
	son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;
	cell_switchon_status_list_data_t *p_node = SON_PNULL;
	cell_status_list_data_t *p_cell_status_list_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

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
		p_cell_ctxt = es_get_cell_context_from_global_context(
				&p_msg->cell_delete_list[idx]);

		if (SON_PNULL != p_cell_ctxt)
		{
			/* Send deregister request to RRM if cell is ENABLING/ENABLED */
			if (!((ES_CELL_STATE_DISABLING == 
							p_cell_ctxt->current_cell_fsm_state) ||
						(ES_CELL_STATE_DISABLED == 
						 p_cell_ctxt->current_cell_fsm_state)))
			{
				son_u16 txn_id =  son_generate_txn_id();

				/* Start Coverity Fix: 60111 */
				if (SON_FAILURE == es_construct_send_rrm_deregister_req(
							txn_id,
							&p_cell_ctxt->cell_id ))
				{
					cell_del_res.cell_delete_status_list[idx].result = 
						SON_PARTIAL_SUCCESS;
					SON_LOG(es_get_log_mode(),
							p_son_es_facility_name,
							SON_ERROR,
							"es_construct_send_rrm_deregister_req failed "
							"for Cell 0x%x",
							son_convert_char_cell_id_to_int_cell_id(
								p_cell_ctxt->cell_id.cell_identity));
				}
				/* End Coverity Fix: 60111 */
				else
				{
					/* Store the transaction Id with which De-register
					   request is sent to RRM. This shall be used later when
					   De-register response is received for this request */
					son_insert_txn_id_in_temp_txn_id_list(txn_id);
				}
			}

			if (son_get_x2_enabled() &&
					SON_TRUE == es_switchon_pending_res_has_entry())
			{
				/*  Check if the cell entry exists in 
				    g_es_switch_on_pending_res. If yes then delete the entry 
				    from pending list as this cell is being deleted. */
				p_node = es_find_cell_in_switchon_pending_res(
						&p_cell_ctxt->cell_id);
				if (SON_PNULL != p_node)
				{
					/* Error code SON_ERR_TIMER_EXPIRED indicates that
					   the pending response is not received for this cell 
					   so decrement expected response count since the 
					   cell is being deleted and no response should be
					   expected for this cell. */
					if (SON_ERR_TIMER_EXPIRED == 
							p_node->cell_switchon_status.error_code)
					{
						es_decrement_switchon_pending_res_expected_count();
					}
					list_delete_node(&g_es_switch_on_pending_res.
							cell_switchon_status_list,
							&p_node->cell_switchon_status_node);
					son_mem_free(p_node);
				}

				/* If after deletion shall check if there is no more
				   pending entries from RRM and can send the response 
				   back to ANR plus if required to OAM and X2 module too. */
				if (MIN_PENDING_RES_COUNT ==
						es_get_switchon_pending_res_expected_count())
				{ 
					/* Check if there is any cell entry in pending 
					   response after expected response count becomes 
					   zero, if there is no entry, that means nothing 
					   to send to X2 or OAM but we must send response
					   to ANR for its request with SUCCESS. 
					   If there is entry in pending response after expected 
					   response count becomes zero then send a response to 
					   ANR, X2 or OAM. */
					if (es_switchon_pending_res_has_entry())
					{
						/*  
						 *   1.  Send Switch on indication to X2 module as X2 
						 *       support is enabled
						 */
						son_create_and_send_switch_on_ind_to_x2( 
								SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ,
								SON_PNULL);

						/*  
						 *   2. Send Switch off indication to OAM module 
						 *      for all the other cells whose oam_ind = true
						 */
						son_create_and_send_cell_switch_on_off_ind_to_oam();
					}

					son_create_and_send_x2_res_to_anr(
							es_get_switchon_pending_res_transaction_id(),
							SON_SUCCESS,
							SON_NO_ERROR);
					/*
					 *   Clear the pending response list
					 */
					es_clear_switchon_pending_res_list();
				}
			}

			/* Check the pending resp list for this cell */
			if (es_get_pending_res_expected_count())
			{
				p_cell_status_list_node = es_find_cell_in_pending_res(&p_msg->cell_delete_list[idx]);
				if (SON_PNULL != p_cell_status_list_node)
				{
					/* Error code SON_ERR_TIMER_EXPIRED indicates that
					   the pending response is not received for this cell 
					   so decrement expected response count since the 
					   cell is being deleted and no response should be
					   expected for this cell. */
					if (SON_ERR_TIMER_EXPIRED == p_cell_status_list_node->cell_status.error_code)
					{
						es_decrement_pending_res_expected_count();
					}
					api = es_get_pending_res_api_id();
					if (SMIF_CELL_INFO_IND == api)
					{
						/* Delete the cell as in case of cell deletion
						   no feature state change indication shall be send
						   and only cell delete response shall be send to
						   SMIF */
						list_delete_node(&g_es_pending_response.cell_sts_list,
								&p_cell_status_list_node->cell_status_node);
						son_mem_free(p_cell_status_list_node);
						SON_LOG(g_es_context.log_enable, p_son_es_facility_name,
								SON_BRIEF,
								"Deleted Cell 0x%x from Pending Response List",
								son_convert_char_cell_id_to_int_cell_id(p_msg->cell_delete_list[idx].cell_identity));
					}
					else
					{
						/* Update Cell in ES Pending Response List with error code
						   SON_ERR_CELL_UNCONFIGURED */
						p_cell_status_list_node->cell_status.error_code = SON_ERR_CELL_UNCONFIGURED;
					}
				}
				if (0 == es_get_pending_res_expected_count())
				{
					es_pending_res_handler();
					es_reset_pending_res();
					es_set_context_state(ES_STATE_ACTIVE);
				}
			}
			/* Delete the cell from the context */
			if (SON_TRUE == es_delete_cell_from_context
					(&p_msg->cell_delete_list[idx]))
			{
				if (SON_PARTIAL_SUCCESS != 
						cell_del_res.cell_delete_status_list[idx].result)
				{
					cell_del_res.cell_delete_status_list[idx].result = 
						SON_SUCCESS;
				}
				SON_LOG(es_get_log_mode(), p_son_es_facility_name,
						SON_INFO,
						"Cell 0x%x deleted successfully",
						son_convert_char_cell_id_to_int_cell_id
						(p_msg->cell_delete_list[idx].cell_identity));
			}
		}
		else
		{
            /*SPR 17777 +-*/
			SON_LOG(es_get_log_mode(), p_son_es_facility_name,
					SON_ERROR,
					"Cell 0x%x does not exists",
					son_convert_char_cell_id_to_int_cell_id
					(p_msg->cell_delete_list[idx].cell_identity));

			LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
            /*SPR 17777 +-*/
		}
	}
	/* Send cell delete resp to mif */
	son_create_send_buffer((son_u8 *)&cell_del_res,
			SON_ES_MODULE_ID,
			SON_MIF_MODULE_ID,
			SMIF_CELL_DELETE_RESP,
			sizeof(cell_del_res));

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
}

/* Rel 3.0 Cell delete support changes Stop */

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : es_get_log_level_req_handler
 * Inputs         : p_cspl_hdr   Pointer to the CSPL Header
 *                : p_buf        Pointer to the incoming message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : This is the message handler for ES module and is
 *                  invoked when SMIF_GET_LOG_LEVEL_REQ message is
 *                  received.
 *****************************************************************************/
static son_buf_retain_status_et
es_get_log_level_req_handler
(
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_buf
 )
{
	smif_get_log_level_req_t *p_msg = (smif_get_log_level_req_t*)p_buf;
	smif_get_log_level_res_t res;
	SON_UT_TRACE_ENTER();

	SON_MEMSET(&res, 0, sizeof(res));
	res.transaction_id = p_msg->transaction_id;
	res.log_level = es_get_log_level();

	/* Send cell delete resp to mif */
	son_create_send_buffer((son_u8 *)&res,
			SON_ES_MODULE_ID,
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

/*****************************************************************************
 * Function Name  : es_active_state_handler
 * Inputs         : p_msg        Pointer to the message received
 *				    p_cspl_hdr Pointer to cspl stack header
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the message handler for ES module and is
 *					called for any message received in state other than INIT and SHUTDOWN.
 ******************************************************************************/
son_buf_retain_status_et
es_active_state_handler
(
 son_u8                 *p_msg,
 STACKAPIHDR    *p_cspl_hdr
 )
{
	son_buf_retain_status_et ret                        = RETAIN_SON_BUFFER;
	smif_init_config_res_t init_config_res = {0};
	son_u8                  *p_buf          = SON_PNULL;
	son_u8                  *p_parsed_msg   = SON_PNULL;
	son_es_index_et event_id        = IDX_NOT_AVAILABLE;
	son_intra_rat_global_cell_id_t      *p_cell_id = SON_PNULL;
	son_oam_log_on_off_et log_mode        = es_get_log_mode();
	son_u8 length          = 0;
	son_procedure_code_et api             = SON_PROCEDURE_UNDEFINED;
	es_fsm_state_et current_es_fsm_state = ES_STATE_INIT;

	SON_UT_TRACE_ENTER();

	/* Handle Global ES Module State Related Messages */

	if (SON_MIF_MODULE_ID == p_cspl_hdr->from)
	{
		switch (p_cspl_hdr->api)
		{
			case SMIF_CELL_INFO_IND:
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"MIF->ES:SMIF_CELL_INFO_IND");
					ret  =  es_cell_info_ind_handler(p_cspl_hdr, p_msg);
					break;
				}

			case SMIF_LOG_ENABLE_DISABLE_CMD: /*same as SMIF_SET_LOG_LEVEL_CMD*/
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"MIF->ES:SMIF_LOG_ENABLE_DISABLE_CMD");
					ret  =  es_log_handler(p_cspl_hdr, p_msg);
					break;
				}
			case SMIF_SET_LOG_LEVEL_CMD:
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"MIF->ES:SMIF_SET_LOG_LEVEL_CMD");
					ret  =  es_log_handler(p_cspl_hdr, p_msg);
					break;
				}

			case SMIF_INIT_CONFIG_REQ:
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"MIF->ES:SMIF_INIT_CONFIG_REQ");
					current_es_fsm_state = es_get_context_state();
					if (ES_STATE_INIT != current_es_fsm_state)
					{
						SON_LOG(log_mode, p_son_es_facility_name, SON_ERROR,
								"Unexpected Message"
								" SMIF_INIT_CONFIG_REQ received in %s",
								SON_ES_FSM_STATES_NAMES[current_es_fsm_state]);
					}

					api = SMIF_INIT_CONFIG_RES;
					init_config_res.transaction_id = *((son_u16 *)p_msg);
					init_config_res.result = SON_FAILURE;
					init_config_res.error_code = SON_ERR_UNEXPECTED_MSG;
					length = sizeof(smif_init_config_res_t);
					p_buf = (son_u8 *)&init_config_res;
					ret = RELEASE_SON_BUFFER;
					break;
				}

			case SONES_SWITCHED_OFF_CELL_INFO_REQ:
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"MIF->ES:SONES_SWITCHED_OFF_CELL_INFO_REQ");
					/*Buffer gets released so ret is RELEASE_SON_BUFFER*/
					ret = handle_switched_off_cell_info(p_cspl_hdr, p_msg);
					break;
				}

				/* AUTONOMOUS SWITCH OFF START */
			case SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ:
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"MIF->ES:SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ");
					/* Check state should be ES_STATE_ACTIVE or ES_STATE_ENABLING*/
					current_es_fsm_state = es_get_context_state();
					if (ES_STATE_DISABLING != current_es_fsm_state)
					{
						son_es_autonomous_switch_off_config_req_handler(p_cspl_hdr, p_msg);
					}
					else
					{
						SON_LOG(log_mode, p_son_es_facility_name, SON_ERROR,
								"Unexpected Message"
								" SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ received in %s",
								SON_ES_FSM_STATES_NAMES[current_es_fsm_state]);

						/* Send global error */
						son_es_autonomous_switch_off_config_resp_t out_msg = {0};

						son_es_send_autonomous_switch_off_config_res_to_mif(&out_msg,
								((son_es_autonomous_switch_off_config_req_t *)p_msg)->transaction_id,
								SON_FAILURE, SON_ERR_DISABLE_IN_PROGRESS);
					}

					ret = RELEASE_SON_BUFFER;
					break;
				}
				/* AUTONOMOUS SWITCH OFF END */

				/* Rel 3.0 Cell delete support changes Start */
			case SMIF_CELL_DELETE_REQ:
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"MIF->ES:SMIF_CELL_DELETE_REQ");
					/*Buffer gets released so ret is RELEASE_SON_BUFFER*/
					ret = es_cell_delete_req_handler(p_cspl_hdr, p_msg);
					break;
				}
				/* Rel 3.0 Cell delete support changes Stop */

				/* SPR-13251 Fix Starts */
			case SMIF_GET_LOG_LEVEL_REQ:
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"MIF->ES:SMIF_GET_LOG_LEVEL_REQ");
					ret = es_get_log_level_req_handler(p_cspl_hdr, p_msg);
					break;
				}
				/* SPR-13251 Fix Ends */

            /* SPR_19619 start */

			case SONES_PEER_CELL_ACTIVATION_REQ:
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"MIF->ES:SONES_PEER_CELL_ACTIVATION_REQ");
					ret = es_peer_cell_activation_req_handler(p_cspl_hdr, p_msg );
					break;
				}

            /* SPR_19619 stop */

			default:
				{
					/* Intentionally left empty
					 * Other SMIF messages are processed below */
					break;
				}
		} /* switch */
	}
	/* ----------------ES 1.2 Changes Start ------------------------*/
	else if (SON_ANR_MODULE_ID == p_cspl_hdr->from)
	{
		switch (p_cspl_hdr->api)
		{
			case SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ:
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"ANR->ES:SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ");
					/*Buffer gets released so ret is RELEASE_SON_BUFFER*/
					ret = handle_cell_switchon_for_x2_setup_req(p_cspl_hdr, p_msg);
					break;
				}

			default:
				{
					SON_LOG(log_mode, p_son_es_facility_name, SON_ERROR,
							"Unknown Message [%d] Received "
							"from ANR", p_cspl_hdr->api);
					ret = RELEASE_SON_BUFFER;
					break;
				}
		} /* switch */
	}
	/* SPR 11689 changes start */
	else if (SON_X2_MODULE_ID == p_cspl_hdr->from)
	{
		switch (p_cspl_hdr->api)
		{
			case SONES_CELL_ACTIVATION_REQ :
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"SONX2->ES:SONES_CELL_ACTIVATION_REQ");
					ret = handle_cell_activation_req(p_cspl_hdr, p_msg);
					break;
				}

/* SPR_19619 start */

			case SONX2_PEER_CELL_ACTIVATION_RESP :
				{
					SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_INFO,
							"SONX2->ES:SONX2_PEER_CELL_ACTIVATION_RESP");
					ret = handle_peer_cell_activation_resp(p_cspl_hdr, p_msg);
					break;
				}

/* SPR_19619 stop */

			default:
				{
					SON_LOG(log_mode, p_son_es_facility_name, SON_ERROR,
							"Unknown Message [%d] Received "
							"from SONX2", p_cspl_hdr->api);
					ret = RELEASE_SON_BUFFER;
					break;
				}
		} /* switch */
	}
	/* SPR 11689 changes end */
	/* -------------------ES 1.2 Changes Finished -------------------*/
	else if (SON_RRM_MODULE_ID != p_cspl_hdr->from)
	{
		SON_LOG(log_mode, p_son_es_facility_name, SON_ERROR,
				"Message Received from unknown "
				"module %u", p_cspl_hdr->from);
		ret = RELEASE_SON_BUFFER;
	}


	/* Handle Cell Specific State Machine Related Messages */
	if (RETAIN_SON_BUFFER == ret)
	{
		event_id = es_calculate_event_id(p_cspl_hdr);

		if (event_id > IDX_SONES_MAX)
		{
			SON_LOG(log_mode, p_son_es_facility_name,
					SON_ERROR,
					"Invalid event id [%u] received",
					event_id);
			SON_UT_TRACE_EXIT();
			return RELEASE_SON_BUFFER;
		}

		switch (p_cspl_hdr->from)
		{
			case SON_RRM_MODULE_ID:
				{
					/* Invoke Pup Tool to parse the message received from RRM*/
					p_parsed_msg = es_parse_rrm_messages(p_msg,
							p_cspl_hdr->api);
					if (SON_PNULL != p_parsed_msg)
					{
						p_cell_id = es_get_cell_id_from_msg(p_cspl_hdr->from,
								p_cspl_hdr->api,
								p_parsed_msg);

						ret = es_process_rrm_msgs(p_parsed_msg,
								p_cell_id,
								p_cspl_hdr,
								event_id);
						/* Free the Memory Allocated in PUP Tool parsing */
						son_mem_free(p_parsed_msg);
					}
					else
					{
						ret = RELEASE_SON_BUFFER;
					}

					break;
				}

			case SON_MIF_MODULE_ID:
				{
					p_cell_id = es_get_cell_id_from_msg(p_cspl_hdr->from,
							p_cspl_hdr->api,
							p_msg);
					ret = es_process_smif_msgs( p_cspl_hdr,
							p_msg,
							p_cell_id,
							event_id);
					break;
				}

			case SON_X2_MODULE_ID:
				{
					SON_LOG(log_mode, p_son_es_facility_name, SON_INFO,
							"Message [%d]"
							" Received from SON-X2 module",
							p_cspl_hdr->api);
					break;
				}
			default:
				{
					SON_LOG(log_mode, p_son_es_facility_name, SON_ERROR,
							"Unexpected Message [%d]"
							"Received from undefined module",
							p_cspl_hdr->api);
					ret = RELEASE_SON_BUFFER;
					break;
				}
		} /* switch */
	}

	if (p_buf)
	{
		son_create_send_buffer(p_buf, SON_ES_MODULE_ID,
				(son_module_id_et)p_cspl_hdr->from, api,
				length );
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* es_active_state_handler */
