/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: son_es_cell_fsm.c $
 *
 ******************************************************************************
 *
 * File Description: This file contains ES per cell FSM functions
 *
 ******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Feb, 2012        Atul/Reetesh	             Initial
 *   Sep,2012        Atul                  Changes Done to support Release 1.2
 *   July, 2014      Shilpi                      SPR 12483 Fix 
 *   July, 2014      Shilpi                      SPR 12708 Fix 
 *   Aug, 2014       Shilpi                      SPR 12876 Fix 
 *****************************************************************************/
#include <son_es_cell_fsm.h>
#include <son_utils.h>
#include <rrm_son_composer.h>
#include <son_anr_ctxt_mgr.h>
extern const son_8 *p_son_es_facility_name;
extern switch_on_pending_res_t g_es_switch_on_pending_res;
/* Fix 875 Start */
extern es_global_t g_es_context;
/* Fix 875 End */
/*****************************************************************************
 * Function Name  : es_map_rrm_error_codes
 * Inputs         : son_func_name   Pointer to calling function name
 *                : result          Result received from RRM
 *                : rrm_error_code  Error Code received from RRM
 *                : msg_id          API Id of message received from RRM
 * Outputs        : None
 * Returns        : son_error_et
 * Description    : Maps and returns SON error code for the received RRM
 *                : error code
 ****************************************************************************/
static son_error_et
es_map_rrm_error_codes
(
 const son_8 *son_func_name,
 rrm_return_et result,
 rrm_error_et rrm_error_code,
 rrm_son_message_resp_et msg_id
 )
{
	son_error_et ret_val = SON_NO_ERROR;

	SON_UT_TRACE_ENTER();
	SON_LOG(es_get_log_mode(), p_son_es_facility_name,
			SON_ERROR,
			"[%s]: Message [%d] received from RRM with result: [%d] and error "
			"code: [%d]",
			son_func_name,
			msg_id,
			result,
			rrm_error_code);

	switch (rrm_error_code)
	{
		case RRM_ERR_INVALID_PARAMS:
			{
				ret_val = SON_ERR_INVALID_PARAMS;
				break;
			}

		case RRM_ERR_UNEXPECTED_MSG:
		case RRM_ERR_CELL_ALREADY_REGISTER_FAILURE:
		case RRM_ERR_CELL_ALREADY_DEREGISTER_FAILURE:
		case RRM_ERR_EVENT_NOT_POSSIBLE:
		case RRM_ERR_CELL_SET_ATTR_FAILURE:
		case RRM_ERR_CELL_UNCONFIGURED:
		case RRM_ERR_OBJECT_NON_EXISTENT:
			{
				ret_val = SON_ERR_RRM_FAILURE;
				break;
			}

		case RRM_ERR_EMERGENCY_CALL_ONGOING:
			{
				ret_val = SON_ERR_EMERGENCY_CALL_ONGOING;
				break;
			}

		case RRM_ERR_CELL_ALREADY_SWITCHED_ON:
			{
				ret_val = SON_ERR_ES_CELL_SWITCHED_ON;
				break;
			}

		case RRM_ERR_CELL_ALREADY_SWITCHED_OFF:
			{
				ret_val = SON_ERR_ES_CELL_SWITCHED_OFF;
				break;
			}

		case RRM_ERR_DEREGISTERATION_FAILURE:
			{
				ret_val = SON_ERR_DEREGISTERATION_FAILURE;
				break;
			}

		case RRM_ERR_REGISTERATION_FAILURE:
			{
				ret_val = SON_ERR_REGISTERATION_FAILURE;
				break;
			}

		default:
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name,
						SON_ERROR,
						"[%s]: RRM Error Code [%d] received in message [%d] could not "
						"be mapped",
						son_func_name,
						rrm_error_code,
						msg_id);
				ret_val = SON_ERR_RRM_FAILURE;
				break;
			}
	} /* switch */

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* es_map_rrm_error_codes */

/******************************************************************************
 * Function Name  : es_construct_send_rrm_register_req 
 * Inputs         : transaction_id  Transaction Id
 *					p_cell_id   Pointer to Cell_Id
 * 				  	interval	Interval(s)
 * 				    threshold	Threshold Count (Active UEs)
 * Outputs        : None
 * Returns        :	son_return_et
 * Description    : Constructs RRM_REGISTER_REQ message
 *****************************************************************************/
static son_return_et 
es_construct_send_rrm_register_req
(
 son_u16	transaction_id,
 son_intra_rat_global_cell_id_t  *p_cell_id,
 son_u16 interval,
 son_u8 threshold
 )
{
	son_rrm_register_req_t reg_req = {0};
	son_return_et ret_val      = SON_FAILURE;
	son_u8 attr_list_size = 0;
	son_u8 attr_list_index = 0;


	SON_UT_TRACE_ENTER();

	reg_req.transaction_id = transaction_id;
	reg_req.req.register_req.object.type = RRM_OBJ_CELL;

	SON_MEMCPY(&(reg_req.req.register_req.object.id.object_id.cell_id), 
			p_cell_id, sizeof(son_intra_rat_global_cell_id_t));

	attr_list_size++;

	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		id = RRM_ATTR_CELL_STATE;
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		trgr_list_size = 1;
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		trgr_list[0].id = RRM_TRGR_EVENT;
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		trgr_list[0].bitmask = RRM_TRIGGER_TYPE_EVENT;
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		trgr_list[0].event.enable = SON_TRUE;

	if (0 == interval)
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_BRIEF,
				"Active UE Report Interval is : [%d], hence No Registration "
				"of Active UE Reports for Cell Id : [0x%x]", 
				interval, son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
	}
	else
	{
		attr_list_size++;
		attr_list_index++;

		reg_req.req.register_req.object.p_attr_lst[attr_list_index].
			id = RRM_ATTR_ACTIVE_UE_COUNT_REPORT;
		reg_req.req.register_req.object.p_attr_lst[attr_list_index].
			trgr_list_size = 1;
		reg_req.req.register_req.object.p_attr_lst[attr_list_index].
			trgr_list[0].id = RRM_TRGR_PERIODIC;
		reg_req.req.register_req.object.p_attr_lst[attr_list_index].
			trgr_list[0].bitmask = RRM_TRIGGER_TYPE_PERIODIC;    
		reg_req.req.register_req.object.p_attr_lst[attr_list_index].
			trgr_list[0].periodic.interval = interval;
	}

	attr_list_size++;
	attr_list_index++;

	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		id = RRM_ATTR_ACTIVE_UE_THRESHOLD;
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		trgr_list_size = 1;
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		trgr_list[0].id = RRM_TRGR_THRESHOLD;
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		trgr_list[0].bitmask = RRM_TRIGGER_TYPE_THRESHOLD;
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		trgr_list[0].threshold.count = threshold;

	/* AUTONOMOUS SWITCH OFF START */
	attr_list_size++;
	attr_list_index++;

	/* Register ES for receiving Load Report from RRM */
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		id = RRM_ATTR_SRV_CELL_LOAD_REPORT;
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		trgr_list_size = 1;
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		trgr_list[0].id = RRM_TRGR_PERIODIC;
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		trgr_list[0].bitmask = RRM_TRIGGER_TYPE_PERIODIC;
	reg_req.req.register_req.object.p_attr_lst[attr_list_index].
		trgr_list[0].periodic.interval =
		SON_ES_RRM_LOAD_REPORT_PERIODIC_INTERVAL;

	/* AUTONOMOUS SWITCH OFF END */

	reg_req.req.register_req.object.attr_list_size = attr_list_size;

	/*PUP Tool call to create and send REGISTER Message to RRM */
	if (RRM_SUCCESS == rrm_son_send_rrm_son_register_req(&reg_req.req,
				SON_ES_MODULE_ID, SON_RRM_MODULE_ID,
				transaction_id, SON_NULL))
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_INFO,
				"Message [%u] send from [%u] to [%u] for Cell Id: [0x%x]",
				RRM_SON_REGISTER_REQ,
				SON_ES_MODULE_ID,
				SON_RRM_MODULE_ID,
				son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
		ret_val = SON_SUCCESS;
	}
	else
	{
		SON_LOG(es_get_log_mode(), 
				p_son_es_facility_name,
				SON_ERROR,
				"Message sending failure to RRM for Cell 0x%x",
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* es_construct_send_rrm_register_req */

/******************************************************************************
 * Function Name  : es_construct_send_rrm_deregister_req 
 * Inputs         : transaction_id  Transaction Id
 *					p_cell_id  	Pointer to Cell_Id
 * Outputs        : None
 * Returns        :	son_return_et
 * Description    : Constructs RRM_DEREGISTER_REQ message
 *****************************************************************************/
son_return_et
es_construct_send_rrm_deregister_req
(
 son_u16 transaction_id,
 son_intra_rat_global_cell_id_t  *p_cell_id
 )
{
	son_rrm_deregister_req_t 	dereg_req = {0};
	son_return_et ret_val = SON_FAILURE;


	SON_UT_TRACE_ENTER();

	dereg_req.transaction_id = transaction_id;
	dereg_req.req.deregister_req.object.type = RRM_OBJ_CELL;
	SON_MEMCPY(&(dereg_req.req.deregister_req.object.id.object_id.cell_id), 
			p_cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	/* AUTONOMOUS SWITCH OFF START */
	dereg_req.req.deregister_req.object.attr_list_size = SON_NULL;
	/* 5 : when S1 msg indication supported by RRM
	 * AUTONOMOUS SWITCH OFF END */

	dereg_req.req.deregister_req.object.p_attr_lst[0].id = RRM_ATTR_CELL_STATE;
	dereg_req.req.deregister_req.object.p_attr_lst[0].trgr_list_size = 0;
	dereg_req.req.deregister_req.object.attr_list_size++;

	dereg_req.req.deregister_req.object.p_attr_lst[1].id = 
		RRM_ATTR_ACTIVE_UE_COUNT_REPORT;
	dereg_req.req.deregister_req.object.p_attr_lst[1].trgr_list_size = 0;
	dereg_req.req.deregister_req.object.attr_list_size++;

	dereg_req.req.deregister_req.object.p_attr_lst[2].id = 
		RRM_ATTR_ACTIVE_UE_THRESHOLD;
	dereg_req.req.deregister_req.object.p_attr_lst[2].trgr_list_size = 0;
	dereg_req.req.deregister_req.object.attr_list_size++;

	/* AUTONOMOUS SWITCH OFF START */
	dereg_req.req.deregister_req.object.p_attr_lst[3].id =
		RRM_ATTR_SRV_CELL_LOAD_REPORT;
	dereg_req.req.deregister_req.object.p_attr_lst[3].trgr_list_size = 0;
	dereg_req.req.deregister_req.object.attr_list_size++;
	/* AUTONOMOUS SWITCH OFF END */

	/* p_dereg_req->object.p_attr_lst[3].id = RRM_ATTR_S1_MSG_NOTIFICATION;
	 ** when S1 msg indication supported by RRM */

	/*PUP Tool call to create and send DE-REGISTER Message to RRM */
	if (RRM_SUCCESS == rrm_son_send_rrm_son_deregister_req(
				&dereg_req.req,
				SON_ES_MODULE_ID, SON_RRM_MODULE_ID,
				transaction_id, SON_NULL))
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_BRIEF,
				"Message [%u] send from [%u] to [%u] for Cell: [0x%x]",
				RRM_SON_DEREGISTER_REQ,
				SON_ES_MODULE_ID,
				SON_RRM_MODULE_ID,
				son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));

		ret_val = SON_SUCCESS;
	}
	else
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
				"Message sending failure to RRM");
	}

	SON_UT_TRACE_EXIT();
	return ret_val;
} /* es_construct_send_rrm_deregister_req */

/*****************************************************************************
 * Function Name  : es_construct_set_attribute_req
 * Inputs         : transaction_id  Transaction Id
 *      			p_cell_id   	Pointer to Cell_Id
 * 				  	att_rid			Attribute ID which needs to be modified
 *                  attr_val   		New Value for Attribute
 * Outputs        : None
 * Returns        :	son_void_t
 * Description    : Constructs RRM_SET_ATTRIBUTE_REQ
 ****************************************************************************/
static son_void_t
es_construct_set_attribute_req
(
 son_u16							transaction_id,
 son_intra_rat_global_cell_id_t  *p_cell_id,
 rrm_attr_id_et					attr_id,
 son_u32                    		attr_val
 )
{
	son_rrm_set_attribute_value_req_t set_att_req = {0};


	SON_UT_TRACE_ENTER();
    /* SPR 20653 Fix Start */
	set_att_req.req.object.type = RRM_OBJ_CELL;

	set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].id = attr_id;
	set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list_size = 1;
	SON_MEMCPY(&(set_att_req.req.object.id.object_id.cell_id), p_cell_id, 
			sizeof(son_intra_rat_global_cell_id_t));

	if (RRM_ATTR_ACTIVE_UE_COUNT_REPORT == attr_id)
	{
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list[0].id = RRM_TRGR_PERIODIC;

		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list[0].bitmask = RRM_TRIGGER_TYPE_PERIODIC;
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list[0].
			periodic.interval = attr_val;
	}
	else if (RRM_ATTR_ACTIVE_UE_THRESHOLD == attr_id)
	{
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list[0].id = RRM_TRGR_THRESHOLD;
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list[0].bitmask = RRM_TRIGGER_TYPE_THRESHOLD;
		set_att_req.req.object.p_attr_lst[set_att_req.req.object.attr_list_size].trgr_list[0].threshold.count =
			(son_u16 )attr_val;
	}
    /* SPR 20653 Fix End */

	/*PUP Tool call to create and send SET ATTRIBUTE REQ Message to RRM */
	if (RRM_FAILURE == rrm_son_send_rrm_son_set_attribute_value_req(
				&set_att_req.req,
				SON_ES_MODULE_ID, SON_RRM_MODULE_ID,
				transaction_id, SON_NULL))
	{
		SON_LOG(es_get_log_mode(), 
				p_son_es_facility_name, 
				SON_ERROR,
				"Message sending failure to RRM");
	}
	else
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_BRIEF,
				"Message [%u] [%u]->[%u] for Cell [0x%x]",
				RRM_SON_SET_ATTRIBUTE_REQ,
				SON_ES_MODULE_ID,
				SON_RRM_MODULE_ID,
				son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
	}

	SON_UT_TRACE_EXIT();
} /* es_construct_set_attribute_req */

/******************************************************************************
 * Function Name  : son_create_and_send_switch_on_ind_to_x2 
 * Inputs         : invoking_api      Invoking API ID
 *                  p_src_cgi         Pointer to cell ID
 * Outputs        : None
 * Returns        : son_void_t	
 * Description    : Generic Function to construct and send 
 *                  son_x2_cell_switch_on_off_ind message to X2 Module.
 *****************************************************************************/
son_void_t
son_create_and_send_switch_on_ind_to_x2
(  
 son_procedure_code_et   invoking_api,
 son_intra_rat_global_cell_id_t *p_src_cgi
 )
{
	son_x2_cell_switch_on_off_ind_t  x2_cell_switch_on_off_ind = {0};
	SON_LIST_NODE           *p_node             = SON_PNULL;
	son_u8                  u_cnt                 = 0;
	cell_switchon_status_list_data_t *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	if (SONES_CELL_SWITCH_ON_REQ == invoking_api)
	{
		x2_cell_switch_on_off_ind.num_srv_cells_list_size = 1;

		SON_MEMCPY(&x2_cell_switch_on_off_ind.
				cell_switch_on_off_data_list[0].src_cgi, p_src_cgi,
				sizeof(son_intra_rat_global_cell_id_t));
		x2_cell_switch_on_off_ind.cell_switch_on_off_data_list[0].
			deactivation_indication = X2_CELL_DEACTIVATION_FALSE;
	}
	/*SPR 11689 changes start */
	else if (SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ == invoking_api || SONES_CELL_ACTIVATION_REQ ==invoking_api)
	{
		/*SPR11689 changes end */
		p_node = get_first_list_node(
				&g_es_switch_on_pending_res.cell_switchon_status_list);

		if (SON_PNULL != p_node)
		{
			while (SON_PNULL != p_node)
			{
				p_list_node =  YMEMBEROF( cell_switchon_status_list_data_t,
						cell_switchon_status_node, p_node);

				if (p_list_node->cell_switchon_status.result == SON_SUCCESS)
				{
					SON_MEMCPY(&x2_cell_switch_on_off_ind.
							cell_switch_on_off_data_list[u_cnt].src_cgi,
							&p_list_node->cell_switchon_status.cgi,
							sizeof(son_intra_rat_global_cell_id_t));
					x2_cell_switch_on_off_ind.
						cell_switch_on_off_data_list[u_cnt].
						deactivation_indication = p_list_node->
						cell_switchon_status.deactivation_indication;
					u_cnt++;
				}

				p_node = get_next_list_node(p_node);
			}

			x2_cell_switch_on_off_ind.num_srv_cells_list_size =  u_cnt;
		}
		else
		{
			SON_LOG(es_get_log_mode(),
					p_son_es_facility_name,
					SON_DETAILED,
					"Switch On pending response list is empty");

			SON_UT_TRACE_EXIT();
			return;
		}
	}
	else
	{
		SON_LOG(es_get_log_mode(),
				p_son_es_facility_name,
				SON_ERROR,
				"Incorrect invoking_api");

		SON_UT_TRACE_EXIT();
		return;
	}

	if (x2_cell_switch_on_off_ind.num_srv_cells_list_size > 0)
	{
		SON_LOG(es_get_log_mode(),
				p_son_es_facility_name,
				SON_DETAILED,
				"SWITCH ON OFF Indication sent to X2 for [%d] cell(s)", 
				x2_cell_switch_on_off_ind.num_srv_cells_list_size);

		for (u_cnt = 0; u_cnt <
				x2_cell_switch_on_off_ind.num_srv_cells_list_size; 
				u_cnt++)
		{
			SON_LOG(es_get_log_mode(),
					p_son_es_facility_name, SON_BRIEF,
					"Cell [0x%x] and Deactivation Flag is : [%d]",
					son_convert_char_cell_id_to_int_cell_id(x2_cell_switch_on_off_ind.
						cell_switch_on_off_data_list[u_cnt].src_cgi.
						cell_identity),
					x2_cell_switch_on_off_ind.
					cell_switch_on_off_data_list[u_cnt].
					deactivation_indication);
		}

		son_create_send_buffer((son_u8 *)&x2_cell_switch_on_off_ind, 
				SON_ES_MODULE_ID, SON_X2_MODULE_ID, SONX2_SWITCH_ON_OFF_IND,
				sizeof(son_x2_cell_switch_on_off_ind_t));
	}
	else
	{
		SON_LOG(es_get_log_mode(),
				p_son_es_facility_name,
				SON_INFO,
				"No Message is sent to X2");
	}

	SON_UT_TRACE_EXIT();
} /* son_create_and_send_switch_on_ind_to_x2 */

/******************************************************************************
 * Function Name  : son_create_and_send_switch_off_ind_to_x2 
 * Inputs         : p_src_cgi         Pointer to cell ID
 * Outputs        : None
 * Returns        :	son_void_t
 * Description    : Generic Function to construct and send
 *                  son_x2_cell_switch_on_off_ind to X2 Module
 *****************************************************************************/
son_void_t
son_create_and_send_switch_off_ind_to_x2
(
 son_intra_rat_global_cell_id_t *p_src_cgi
 )
{
	son_x2_cell_switch_on_off_ind_t x2_cell_switch_on_off_ind = {0};
	son_u8 u_cnt = 0;


	x2_cell_switch_on_off_ind.num_srv_cells_list_size = 1;

	SON_UT_TRACE_ENTER();

	SON_MEMCPY(&x2_cell_switch_on_off_ind.cell_switch_on_off_data_list[0].
			src_cgi, p_src_cgi, sizeof(son_intra_rat_global_cell_id_t));

	x2_cell_switch_on_off_ind.cell_switch_on_off_data_list[0].
		deactivation_indication = X2_CELL_DEACTIVATION_TRUE;

	SON_LOG(es_get_log_mode(),
			p_son_es_facility_name, SON_DETAILED,
			"SWICTH ON OFF Indication sent to X2 for [%d] cell(s) "
			"with following details",
			x2_cell_switch_on_off_ind.num_srv_cells_list_size);

	for (u_cnt = 0;
			u_cnt < x2_cell_switch_on_off_ind.num_srv_cells_list_size; 
			u_cnt++)
	{
		SON_LOG(es_get_log_mode(),
				p_son_es_facility_name, SON_BRIEF,
				"Cell ID : [0x%x] and Deactivation Flag is : [%d]",
				son_convert_char_cell_id_to_int_cell_id(x2_cell_switch_on_off_ind.
					cell_switch_on_off_data_list[u_cnt].src_cgi.cell_identity),
				x2_cell_switch_on_off_ind.cell_switch_on_off_data_list[u_cnt].
				deactivation_indication);
	}

	son_create_send_buffer((son_u8 *)&x2_cell_switch_on_off_ind, 
			SON_ES_MODULE_ID, SON_X2_MODULE_ID, SONX2_SWITCH_ON_OFF_IND,
			sizeof(son_x2_cell_switch_on_off_ind_t));

	SON_UT_TRACE_EXIT();
} /* son_create_and_send_switch_off_ind_to_x2 */

/******************************************************************************
 * Function Name  : son_create_and_send_cell_switch_on_resp_to_oam 
 * Inputs         : transaction_id  : Transaction ID
 *                : p_cell_id       : Pointer to cell ID
 *                : result          : Succes/Failure as input
 *                : error_code      : Error Code
 * Outputs        : None
 * Returns        :	son_void_t
 * Description    : Generic function to construct and send cell switch 
 *                  on response to OAM.
 *****************************************************************************/
son_void_t
son_create_and_send_cell_switch_on_resp_to_oam
(
 son_u16 transaction_id,
 son_intra_rat_global_cell_id_t  *p_cell_id,
 son_return_et   result,
 son_error_et    error_code
 )
{
	son_es_cell_switch_on_res_t cell_switch_on_resp = {0};


	SON_UT_TRACE_ENTER();

	SON_MEMCPY(&cell_switch_on_resp.generic_res.cell_id, p_cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	cell_switch_on_resp.transaction_id = transaction_id;
	cell_switch_on_resp.generic_res.error_code = error_code;
	cell_switch_on_resp.generic_res.result = result;

	son_create_send_buffer((son_u8 *)&cell_switch_on_resp
			, SON_ES_MODULE_ID, SON_MIF_MODULE_ID
			, SONES_CELL_SWITCH_ON_RES
			, sizeof(son_es_cell_switch_on_res_t));

	SON_UT_TRACE_EXIT();
} /* son_create_and_send_cell_switch_on_resp_to_oam */

/******************************************************************************
 * Function Name  : son_create_and_send_cell_switch_off_resp_to_oam 
 * Inputs         : transaction_id  : Transaction ID
 *                : p_cell_id       : Pointer to cell ID
 *                : result          : Succes/Failure as input
 *                : error_code      : Error Code
 * Outputs        : None
 * Returns        :	son_void_t
 * Description    : Generic function to construct and send cell switch
 *                  off response to OAM.
 *****************************************************************************/
son_void_t
son_create_and_send_cell_switch_off_resp_to_oam
(
 son_u16 transaction_id,
 son_intra_rat_global_cell_id_t *p_cell_id,
 son_return_et   result,
 son_error_et    error_code
 )
{
	son_es_cell_switch_off_res_t    cell_switch_off_resp = {0}; 


	SON_UT_TRACE_ENTER();

	cell_switch_off_resp.transaction_id = transaction_id;
	cell_switch_off_resp.generic_res.result = result;
	cell_switch_off_resp.generic_res.error_code = error_code;
	SON_MEMCPY(&cell_switch_off_resp.generic_res.cell_id,
			p_cell_id, sizeof(son_intra_rat_global_cell_id_t));

	son_create_send_buffer((son_u8 *)&cell_switch_off_resp
			, SON_ES_MODULE_ID, SON_MIF_MODULE_ID
			, SONES_CELL_SWITCH_OFF_RES
			, sizeof(son_es_cell_switch_off_res_t));

	SON_UT_TRACE_EXIT();
} /* son_create_and_send_cell_switch_off_resp_to_oam */

/******************************************************************************
 * Function Name  : son_create_and_send_x2_res_to_anr
 * Inputs         : transaction_id  : Transaction ID
 *                : result          : Succes/Failure as input
 *                : error_code      : Error Code
 * Outputs        : None
 * Returns        :	son_void_t
 * Description    : Generic function to construct and send cell switch
 *                  on response for x2 setup request to X2 Module.
 *****************************************************************************/
son_void_t
son_create_and_send_x2_res_to_anr
(
 son_u16             transaction_id,
 son_return_et       result,
 son_error_et        error_code
 )
{
	son_es_cell_switchon_for_x2_setup_res_t     cell_switchon_x2_res = {0};
	SON_LIST_NODE           *p_node             = SON_PNULL;
	cell_switchon_status_list_data_t *p_list_node = SON_PNULL;


	SON_UT_TRACE_ENTER();

	cell_switchon_x2_res.transaction_id = transaction_id;

	p_node = get_first_list_node(
			&g_es_switch_on_pending_res.cell_switchon_status_list);

	if (SON_PNULL != p_node)
	{
		while (SON_PNULL != p_node)
		{
			p_list_node =  YMEMBEROF( cell_switchon_status_list_data_t,
					cell_switchon_status_node, p_node);

			if (SON_FAILURE == p_list_node->cell_switchon_status.result)
			{
				SON_LOG(es_get_log_mode(), 
						p_son_es_facility_name,
						SON_BRIEF,
						"Result - SON_FAILURE found in switch on pending response "
						"structure for cell : [0x%x]",
						son_convert_char_cell_id_to_int_cell_id(p_list_node->cell_switchon_status.
							cgi.cell_identity));

				cell_switchon_x2_res.result = SON_FAILURE;
				cell_switchon_x2_res.error_code = p_list_node->
					cell_switchon_status.error_code;

				break;
			}
			else
			{
				cell_switchon_x2_res.result = SON_SUCCESS;
				cell_switchon_x2_res.error_code = SON_NO_ERROR;
			}

			p_node = get_next_list_node(p_node);
		}
	}
	else
	{
		/*
		 *   No entry in pending structure and if it is still called i.e.
		 *   eitehr SUCCESS(if existing switching off failed)/FAILURE(timer
		 *   expiry) needs to be sent
		 */
		cell_switchon_x2_res.result = result;
		cell_switchon_x2_res.error_code = error_code;
	}

	son_create_send_buffer((son_u8 *)&cell_switchon_x2_res, SON_ES_MODULE_ID,
			SON_ANR_MODULE_ID, SONES_CELL_SWITCH_ON_FOR_X2_SETUP_RES,
			sizeof(cell_switchon_x2_res));

	SON_UT_TRACE_EXIT();
} /* son_create_and_send_x2_res_to_anr */

/******************************************************************************
 * Function Name  : son_create_and_send_cell_switch_on_off_ind_to_oam
 * Inputs         : None
 * Outputs        : None
 * Returns        :	son_void_t
 * Description    : Generic function to construct and send cell switch
 *                  on indication to OAM.
 *****************************************************************************/
son_void_t
son_create_and_send_cell_switch_on_off_ind_to_oam
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	son_es_cell_switchon_ind_t          cell_switchon_ind = {0};
	SON_LIST_NODE           *p_node             = SON_PNULL;
	cell_switchon_status_list_data_t *p_list_node = SON_PNULL;
	son_u8  count   = 0;
	es_cell_context_t   *p_cell_ctxt = SON_NULL;


	SON_UT_TRACE_ENTER();

	p_node = get_first_list_node(
			&g_es_switch_on_pending_res.cell_switchon_status_list);

	if (SON_PNULL != p_node)
	{
		while (SON_PNULL != p_node)
		{
			p_list_node =  YMEMBEROF( cell_switchon_status_list_data_t,
					cell_switchon_status_node, p_node);

			SON_LOG(es_get_log_mode(),
					p_son_es_facility_name,
					SON_INFO,
					"OAM_Indication_to_Send for cell [0x%x] in switch "
					"on pending response structure is : [%d]",
					son_convert_char_cell_id_to_int_cell_id(p_list_node->cell_switchon_status.cgi.
						cell_identity),
					p_list_node->cell_switchon_status.oam_ind_required);
			if (SON_TRUE == p_list_node->cell_switchon_status.oam_ind_required)
			{
				SON_MEMCPY(&cell_switchon_ind.cell_on_status_list[count].cgi,
						&p_list_node->cell_switchon_status.cgi,
						sizeof(son_intra_rat_global_cell_id_t));
				/*SPR11689 changes start */
				p_cell_ctxt = es_get_cell_context_from_global_context(
						&p_list_node->cell_switchon_status.cgi);
				if (p_cell_ctxt) /* cov_65752_fix */
				{    
					if(p_cell_ctxt->cell_activation_req_flag == SON_TRUE)
					{
						if(p_cell_ctxt->rrm_cell_activation_response == SON_SUCCESS)
						{
							cell_switchon_ind.cell_on_status_list[count].
								switch_on_off_cause = SON_SWTICH_ON_FROM_PEER_ENB;

							cell_switchon_ind.cell_on_status_list[count].result =
								p_list_node->cell_switchon_status.result;

							cell_switchon_ind.cell_on_status_list[count].error_code =
								p_list_node->cell_switchon_status.error_code;

							count++;
						}

						/* now reset cell activation flag*/
						p_cell_ctxt->cell_activation_req_flag = SON_FALSE;

					} 
					else
					{
						cell_switchon_ind.cell_on_status_list[count].
							switch_on_off_cause = SON_SWITCH_ON_DUE_TO_X2_SET_UP;

						cell_switchon_ind.cell_on_status_list[count].result = 
							p_list_node->cell_switchon_status.result;

						cell_switchon_ind.cell_on_status_list[count].error_code = 
							p_list_node->cell_switchon_status.error_code;

						count++;
					}
				}
				/*SPR11689 changes end */
			}

			p_node = get_next_list_node(p_node);
		}

		if (count > 0)
		{
			cell_switchon_ind.cell_on_status_list_size = count;

			SON_LOG(es_get_log_mode(),
					p_son_es_facility_name,
					SON_INFO,
					" SONES_CELL_SWITCH_ON_OFF_IND message is sent to OAM with"
					" total number of cells succesfull/unsuccesful switched on"
					" are: [%d]",
					cell_switchon_ind.cell_on_status_list_size);

			son_create_send_buffer((son_u8 *)&cell_switchon_ind, 
					SON_ES_MODULE_ID,
					SON_MIF_MODULE_ID, SONES_CELL_SWITCH_ON_OFF_IND,
					sizeof(son_es_cell_switchon_ind_t));
		}
		else
		{
			SON_LOG(es_get_log_mode(),
					p_son_es_facility_name,
					SON_DETAILED,
					"No Switch On  Indication to be send to OAM");
		}
	}

	SON_UT_TRACE_EXIT();
} /* son_create_and_send_cell_switch_on_off_ind_to_oam */

/* AUTONOMOUS SWITCH OFF START */

/******************************************************************************
 * Function Name  : son_create_and_send_cell_switch_on_off_ind_to_oam_for_autonomous_switch_off
 * Inputs         : p_cell_ctxt         Pointer to cell context
 * Outputs        : None
 * Returns        :	son_void_t
 * Description    : Function to construct and send cell switch
 *                  on indication to OAM when Switch off res is received from RRM in response
 *                  to Autonomous cell switch off req triggered by ES
 *****************************************************************************/
static son_void_t
son_create_and_send_cell_switch_on_off_ind_to_oam_for_autonomous_switch_off
(
 es_cell_context_t   *p_cell_ctxt
 )
{
	son_u8 count             = SON_NULL;
	son_es_cell_switchon_ind_t cell_switchon_ind = {0};


	SON_UT_TRACE_ENTER();

	SON_MEMCPY(&cell_switchon_ind.cell_on_status_list[count].cgi,
			&p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	/* Fix 875 Start */    
	if((SON_ES_SWITCH_OFF_LOW_LOAD_COND_HIT & p_cell_ctxt->switch_off_cond_hit_status) &&
			(SON_ES_SWITCH_OFF_LOW_UE_CNT_COND_HIT & p_cell_ctxt->switch_off_cond_hit_status))
	{
		cell_switchon_ind.cell_on_status_list[count].switch_on_off_cause = 
			SON_SWITCH_OFF_DUE_TO_LOW_LOAD_AND_LOW_UE_CNT_COND;
	}
	else if(SON_ES_SWITCH_OFF_LOW_LOAD_COND_HIT & p_cell_ctxt->switch_off_cond_hit_status)
	{
		cell_switchon_ind.cell_on_status_list[count].switch_on_off_cause = 
			SON_SWITCH_OFF_DUE_TO_LOW_LOAD_COND;
	}
	else
	{ 
		cell_switchon_ind.cell_on_status_list[count].switch_on_off_cause = 
			SON_SWITCH_OFF_DUE_TO_LOW_UE_CNT_COND;
	}
	/* Fix 875 End */

	cell_switchon_ind.cell_on_status_list[count].result = SON_SUCCESS;

	cell_switchon_ind.cell_on_status_list[count].error_code = SON_NO_ERROR;

	/* Fix 875 Start */
	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
			"Sending Cell Switch OFF(Autonomous) ind to MIF for the cause : [%d]",
			cell_switchon_ind.cell_on_status_list[count].switch_on_off_cause);
	/* Fix 875 End */

	count++;

	cell_switchon_ind.cell_on_status_list_size = count;

	son_create_send_buffer((son_u8 *)&cell_switchon_ind,
			SON_ES_MODULE_ID,
			SON_MIF_MODULE_ID, SONES_CELL_SWITCH_ON_OFF_IND,
			sizeof(son_es_cell_switchon_ind_t));

	SON_UT_TRACE_EXIT();
} /* son_create_and_send_cell_switch_on_off_ind_to_oam_for_autonomous_switch_off */

/* AUTONOMOUS SWITCH OFF END */

/****************************************************************************
 * Function Name  : es_fsm_enable_req_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for Cell Specific SONES_ENABLE_REQ.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_enable_req_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t  *p_cell_ctxt,
 STACKAPIHDR        *p_cspl_hdr,
 son_u8			   *p_msg
 )
{
	son_es_enable_req_t *p_req 	= (son_es_enable_req_t *)(p_msg);


	SON_UT_TRACE_ENTER();

	if (SON_SUCCESS == es_construct_send_rrm_register_req(
				p_req->transaction_id,
				&p_req->enable_req.srv_cgi,
				p_req->enable_req.interval,
				p_req->enable_req.threshold_value))
	{
		SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ES_CELL_STATE_ENABLING);
        /*SPR 20288 Fix Start*/
        p_cell_ctxt->ue_count_threshold = p_req->enable_req.threshold_value;
        p_cell_ctxt->ue_report_interval = (son_u8)p_req->enable_req.interval;
        /*SPR 20288 Fix Stop*/

	}

     /*SPR 17777 +-*/
    SON_LOG(es_get_log_mode(), p_son_es_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr)
     /*SPR 17777 +-*/
	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_enable_req_handler */

/******************************************************************************
 * Function Name  : es_fsm_disable_req_handler 
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for Cell Specific SONES_DISABLE_REQ.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_disable_req_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_msg
 )
{
	son_es_disable_req_t *p_req = (son_es_disable_req_t *)(p_msg);


	SON_UT_TRACE_ENTER();

	if (SON_SUCCESS == es_construct_send_rrm_deregister_req(
				p_req->transaction_id,
				&p_req->disable_req.srv_cgi))
	{
		SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ES_CELL_STATE_DISABLING);
	}

     /*SPR 17777 +-*/
    SON_LOG(es_get_log_mode(), p_son_es_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr)
     /*SPR 17777 +-*/
	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* es_fsm_disable_req_handler */

/******************************************************************************
 * Function Name  : es_fsm_modify_ue_cnt_intrvl_req_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_MODIFY_UE_COUNT_INTERVAL_REQ.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_modify_ue_cnt_intrvl_req_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_es_modify_ue_count_interval_req_t *p_req =
		(son_es_modify_ue_count_interval_req_t *)(p_msg);

	/* Fix 875 Start */
	/* Store the new requested value to commit when resp from RRM is received */
	p_cell_ctxt->modify_ue_report_interval = p_req->modify_interval_req.interval;
	/* Fix 875 End */

	SON_UT_TRACE_ENTER();
	es_construct_set_attribute_req(p_req->transaction_id, 
			&p_req->modify_interval_req.srv_cgi,
			RRM_ATTR_ACTIVE_UE_COUNT_REPORT,
			p_req->modify_interval_req.interval);
	SON_UT_TRACE_EXIT();
     /*SPR 17777 +-*/
    SON_LOG(es_get_log_mode(), p_son_es_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr)
     /*SPR 17777 +-*/
	return RELEASE_SON_BUFFER;
}

/******************************************************************************
 * Function Name  : es_fsm_modify_ue_cnt_threshold_req_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_MODIFY_UE_COUNT_THRESHOLD_REQ.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_modify_ue_cnt_threshold_req_handler
(
 son_es_index_et     evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_es_modify_ue_count_threshold_req_t	*p_req 		=
		(son_es_modify_ue_count_threshold_req_t *)(p_msg);

	/* Fix 875 Start */
	/* Store the new requested value to commit when resp from RRM is received */
	p_cell_ctxt->modify_ue_count_threshold_val = 
		p_req->modify_threshold_req.threshold_value;
	/* Fix 875 End */

	SON_UT_TRACE_ENTER();	
	es_construct_set_attribute_req(p_req->transaction_id, 
			&(p_req->modify_threshold_req.srv_cgi),
			RRM_ATTR_ACTIVE_UE_THRESHOLD,
			p_req->modify_threshold_req.threshold_value);
     /*SPR 17777 +-*/
    SON_LOG(es_get_log_mode(), p_son_es_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr)
     /*SPR 17777 +-*/
	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : es_process_rrm_register_deregister_res
 * Inputs         : msg_id: API ID of response to be sent to SMIF
 *                  tid:    Transaction ID of response to be sent to SMIF
 *                  p_res:  Pointer to register or deregister response
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : This is the function to handle register_res or deregister_res
 *
 ****************************************************************************/
static son_return_et
es_process_rrm_register_deregister_res
(
 son_procedure_code_et msg_id,
 son_u16 tid,
 rrm_son_registration_based_res_t *p_res
 )
{
	son_es_enable_res_t 			response 		= {0};
	son_u16 						pndg_rs_tid		= 0;
	son_bool_et  					send_res 		= SON_TRUE;
	son_intra_rat_global_cell_id_t 	*p_rcvd_cell_id	
		= (son_intra_rat_global_cell_id_t *)&p_res->id.object_id.cell_id;
	son_return_et                   result = SON_SUCCESS;
	son_error_et                    error_code = SON_NO_ERROR;
	rrm_son_message_resp_et         rrm_response_id = RRM_SON_RESP_MAX_API;
	son_feature_state_et state = SON_STATE_DISABLED;
	son_feature_state_change_cause_et cause = SON_RRM_REGISTRATION_UNSUCCESSFUL;
	son_procedure_code_et api = es_get_pending_res_api_id();


	SON_UT_TRACE_ENTER();

	if (SONES_ENABLE_RES == msg_id)
	{
		rrm_response_id = RRM_SON_REGISTER_RESP;
	}
	else
	{
		rrm_response_id = RRM_SON_DEREGISTER_RESP;
	}

	result = ((p_res->result == RRM_SUCCESS) ? SON_SUCCESS : SON_FAILURE);
	if (SON_FAILURE == result)
	{
		error_code = es_map_rrm_error_codes(__func__,
				(rrm_return_et)result,
				p_res->error_code,
				rrm_response_id);
	}
	else
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_DETAILED,
				"Message [%d] received from RRM with result: [%d] and "
				"error code: [%d]",
				rrm_response_id,
				result,
				error_code);
		state = SON_STATE_ENABLED;
		cause = SON_RRM_REGISTRATION_SUCCESSFUL;
	}

	SON_UT_TRACE_ENTER();

	pndg_rs_tid = es_get_pending_res_transaction_id();

	if (pndg_rs_tid != SON_UNDEFINED)
	{
		if (pndg_rs_tid != tid)
		{
			es_delete_cell_from_pending_res(p_rcvd_cell_id);
		}
		else
		{
			if (SON_TRUE == es_update_cell_in_pending_res(p_rcvd_cell_id,
						error_code))
			{
				send_res = SON_FALSE;
				es_decrement_pending_res_expected_count();
				es_set_pending_res_ts_with_current_timestamp();
				if (SON_FAILURE == result)
				{
					es_set_pending_res_error_rcvd(SON_TRUE);
				}
			}    
		}

		if (!es_get_pending_res_expected_count() ||
				(es_get_pending_res_ts_diff_from_curr_ts() > 
				 ES_TIMEOUT_FOR_PENDING_RES))
		{
			if (SMIF_CELL_INFO_IND != api)
			{
				/* send response for request maintained in pending_response */
				response.transaction_id = pndg_rs_tid;
				es_delete_succesful_cells_from_pending_res();

				if ((response.enable_res.cell_status_list_size =
							es_get_pending_res_cell_count()) > 
						MIN_PENDING_RES_COUNT)
				{
					es_construct_cell_arr_from_pending_res_list(
							response.enable_res.cell_status_list);
				}
				else
				{
					response.enable_res.result =
						SON_SUCCESS;
					response.enable_res.error_code =
						SON_NO_ERROR;
				}

				son_create_send_buffer((son_u8 *)&response, 
						SON_ES_MODULE_ID,
						SON_MIF_MODULE_ID, 
						api,
						sizeof(response));
			}

			if (es_get_pending_res_ts_diff_from_curr_ts() >
					ES_TIMEOUT_FOR_PENDING_RES)
			{
				es_cell_context_node_t *p_ctxt_node = SON_PNULL;
				es_cell_context_t  *p_cell_ctxt = SON_PNULL;
				es_cell_fsm_state_et new_state = ES_CELL_STATE_UNDEFINED;
				p_ctxt_node = es_get_first_cell_ctxt();
				while (SON_PNULL != p_ctxt_node)
				{
					p_cell_ctxt = &p_ctxt_node->data;
					if (es_find_cell_in_pending_res(&p_cell_ctxt->cell_id))
					{
						new_state = p_cell_ctxt->previous_cell_fsm_state;
						SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
								p_cell_ctxt->previous_cell_fsm_state,
								p_cell_ctxt->current_cell_fsm_state,
								new_state);
					}

					if (SMIF_CELL_INFO_IND == api)
					{
						/* Send feature state change indication to SMIF */
						es_send_feature_state_change_ind(&p_cell_ctxt->cell_id,
								SON_STATE_DISABLED, 
								SON_RRM_REGISTRATION_UNSUCCESSFUL);
					}

					p_ctxt_node = es_get_next_node(p_ctxt_node);
				}
			}

			es_set_context_state(ES_STATE_ACTIVE);
			es_reset_pending_res();
		}
	}

	if (RRM_FAILURE == p_res->result &&
			(RRM_ERR_OBJECT_NON_EXISTENT == p_res->error_code ||
			 RRM_ERR_CELL_UNCONFIGURED == p_res->error_code)
			&& msg_id == SONES_ENABLE_RES)
	{
		state = SON_STATE_DELETED;
		cause = SON_RRM_CELL_UNCONFIGURED;
	}

	if (SMIF_CELL_INFO_IND == api)
	{
		/* Send feature state change indication as received
		 * registration response is for the request triggered due
		 * to Cell Info Indication request received by ES */
		es_send_feature_state_change_ind(p_rcvd_cell_id,
				state, cause);
	}

	if (RRM_FAILURE == p_res->result &&
			(RRM_ERR_OBJECT_NON_EXISTENT == p_res->error_code ||
			 RRM_ERR_CELL_UNCONFIGURED == p_res->error_code)
			&& msg_id == SONES_ENABLE_RES)
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_ERROR,
				"Registration failed, cell [0x%x] deleted from ES"
				" Cell Context List",
				son_convert_char_cell_id_to_int_cell_id(p_rcvd_cell_id->cell_identity));
		es_delete_cell_from_context(p_rcvd_cell_id);
	}

	if (send_res == SON_TRUE)
	{
		response.transaction_id = tid;
		if (SON_NO_ERROR != error_code)
		{
			response.enable_res.cell_status_list_size = 1;
			SON_MEMCPY(&response.enable_res.cell_status_list[0].cgi,
					p_rcvd_cell_id, sizeof(son_intra_rat_global_cell_id_t));
			response.enable_res.cell_status_list[0].error_code = error_code;
		}
		else
		{
			response.enable_res.error_code = error_code;
		}

		response.enable_res.result = result;

		son_create_send_buffer((son_u8 *)&response, 
				SON_ES_MODULE_ID,
				SON_MIF_MODULE_ID, msg_id,
				sizeof(response));
	}

	SON_UT_TRACE_EXIT();
	return SON_SUCCESS;
} /* es_process_rrm_register_deregister_res */

/******************************************************************************
 * Function Name  : es_fsm_register_res_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for RRM_REGISTER_RES.It is invoked
 *                  when ES state for the cell is ES_CELL_STATE_ENABLING.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_register_res_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_rrm_registration_res_t *p_res = (son_rrm_registration_res_t *)p_msg;


	SON_UT_TRACE_ENTER();

	if ((p_res->res.registration_resp.result) == RRM_SUCCESS)
	{
		SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ES_CELL_STATE_ENABLED);
	}
	else
	{
		SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ES_CELL_STATE_DISABLED);
        /*SPR 20288 Fix Start*/
        p_cell_ctxt->ue_count_threshold = 0;
        p_cell_ctxt->ue_report_interval = 0;
        /*SPR 20288 Fix Stop*/

	}

	es_process_rrm_register_deregister_res(SONES_ENABLE_RES, 
			p_res->transaction_id,
			&p_res->res.registration_resp);

     /*SPR 17777 +-*/
    SON_LOG(es_get_log_mode(), p_son_es_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr)
     /*SPR 17777 +-*/
	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_register_res_handler */

/*****************************************************************************
 * Function Name  : es_fsm_deregister_res_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for the RRM_DEREGISTER_RES. It is invoked
 *                  when ES state for the cell is ES_CELL_STATE_DISABLING.
 ****************************************************************************/
static son_buf_retain_status_et
es_fsm_deregister_res_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_rrm_deregistration_res_t *p_res 	= 
		(son_rrm_deregistration_res_t *)p_msg;
	son_buf_retain_status_et ret_val = RELEASE_SON_BUFFER;


	SON_UT_TRACE_ENTER();

	if ((p_res->res.deregistration_resp.result) == RRM_SUCCESS)
	{
		SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ES_CELL_STATE_DISABLED);
	}
	else
	{
		es_cell_fsm_state_et new_state = ES_CELL_STATE_ENABLED;
		if (ES_CELL_STATE_ENABLED == p_cell_ctxt->
				previous_cell_fsm_state)
		{
			new_state = ES_CELL_STATE_ENABLED;
		}
		else if (ES_CELL_STATE_SWITCHED_OFF == p_cell_ctxt->
				previous_cell_fsm_state)
		{
			new_state = ES_CELL_STATE_SWITCHED_OFF;
		}

		SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				new_state);
	}

	es_process_rrm_register_deregister_res(SONES_DISABLE_RES,
			p_res->transaction_id,
			&p_res->res.deregistration_resp);
     /*SPR 17777 +-*/
    SON_LOG(es_get_log_mode(), p_son_es_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr)
     /*SPR 17777 +-*/
	SON_UT_TRACE_EXIT();
	return ret_val;
} /* es_fsm_deregister_res_handler */

/******************************************************************************
 * Function Name  : es_fsm_cell_switch_on_req_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_CELL_SWITCH_ON_REQ.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_cell_switch_on_req_handler
(
 son_es_index_et     evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_es_cell_switch_on_req_t *p_cell_switch_on_req = 
		(son_es_cell_switch_on_req_t *)p_msg;


	SON_UT_TRACE_ENTER();

	/*PUP Tool call to create and send CELL SWITCH ON Message to RRM */
	if (RRM_SUCCESS == rrm_son_send_rrm_son_cell_switch_on_req(
				(rrm_son_cell_switch_on_req_t *)
				&p_cell_switch_on_req->switch_on_req,
				SON_ES_MODULE_ID, SON_RRM_MODULE_ID,
				p_cell_switch_on_req->transaction_id, SON_NULL))
	{                                   
		SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ES_CELL_STATE_SWITCHING_ON);
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_BRIEF,
				"Message [%u] send from [%u] to [%u] for Cell: [0x%x]",
				RRM_SON_CELL_SWITCH_ON_REQ,
				SON_ES_MODULE_ID,
				SON_RRM_MODULE_ID,
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
	}
	else
	{
		SON_LOG(es_get_log_mode(), 
				p_son_es_facility_name, 
				SON_ERROR,
				"Message sending failure"
				" to RRM Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr);
        /*SPR 17777 +-*/
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* es_fsm_cell_switch_on_req_handler */


/******************************************************************************
 * Function Name  : es_fsm_cell_activation_req_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_CELL_ACTIVATION_REQ.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_cell_activation_req_handler
(
 son_es_index_et     evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	rrm_son_cell_switch_on_req_t cell_switch_on_req;  
	SON_MEMCPY(&cell_switch_on_req.cell_id,
			&p_cell_ctxt->cell_id,
			sizeof(rrm_oam_eutran_global_cell_id_t));


	SON_UT_TRACE_ENTER();
	son_u16 transaction_id = son_generate_txn_id();

	/*PUP Tool call to create and send CELL SWITCH ON Message to RRM */
	if (RRM_SUCCESS == rrm_son_send_rrm_son_cell_switch_on_req(
				&cell_switch_on_req,
				SON_ES_MODULE_ID, SON_RRM_MODULE_ID,
				transaction_id, SON_NULL))
	{                                   
		SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ES_CELL_STATE_SWITCHING_ON);
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_BRIEF,
				"Message [%u] send from [%u] to [%u] for Cell: [0x%x]",
				RRM_SON_CELL_SWITCH_ON_REQ,
				SON_ES_MODULE_ID,
				SON_RRM_MODULE_ID,
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
	}
	else
	{
		SON_LOG(es_get_log_mode(), 
				p_son_es_facility_name, 
				SON_ERROR,
				"Message sending failure to RRM");
                /*SPR 17777 +-*/
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
		LTE_GCC_UNUSED_PARAM(evnt_id);
		LTE_GCC_UNUSED_PARAM(p_msg);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* es_fsm_cell_activation_req_handler */

/******************************************************************************
 * Function Name  : es_fsm_cell_switch_on_res_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_CELL_SWITCH_ON_RES.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_cell_switch_on_res_handler
(
 son_es_index_et     evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_es_cell_switch_on_res_t *p_cell_switch_on_res = 
		(son_es_cell_switch_on_res_t *)p_msg;
	son_es_cell_switch_on_res_t cell_switch_on_resp = {0};
	cell_switchon_status_list_data_t    *p_list_node_anr = SON_PNULL;
	son_error_et converted_error_code = SON_NO_ERROR;
	SON_UT_TRACE_ENTER();

	rrm_error_et rrm_error_code = 
		((rrm_son_non_registration_based_res_t *)(&p_cell_switch_on_res->generic_res))->error_code;

	/*SPR11689 changes start */
	if(p_cell_ctxt->cell_activation_req_flag == SON_TRUE)
	{

		/*search if the cell is in pending response list*/
		p_list_node_anr = es_find_cell_in_switchon_pending_res(
				&p_cell_switch_on_res->generic_res.cell_id);
		if (SON_PNULL != p_list_node_anr)
		{
			/*save rrm response*/
			p_cell_ctxt->rrm_cell_activation_response = 
				p_cell_switch_on_res->generic_res.result;
			/*Decremenet Expected res count*/
			es_decrement_switchon_pending_res_expected_count();
			if (MIN_PENDING_RES_COUNT ==
					es_get_switchon_pending_res_expected_count())
			{
				/* traverse the list and send cell activation resp to x2*/
				es_construct_and_send_cell_activation_response();

				/*Send Switch on-off indication to OAM module*/
				son_create_and_send_cell_switch_on_off_ind_to_oam();
			}
			if(SON_SUCCESS == p_cell_switch_on_res->generic_res.result)
			{
				/*Send Switch on-off indication to ANR module*/
				es_send_sonanr_cell_switch_on_off_info_ind(
						&p_cell_ctxt->cell_id
						,ES_CELL_SWITCHED_ON);
				/*change cell state as switched-on*/ 
				SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
						p_cell_ctxt->previous_cell_fsm_state,
						p_cell_ctxt->current_cell_fsm_state,
						ES_CELL_STATE_ENABLED);
			}
			else
			{
				/*change cell state as switched-off*/ 
				SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
						p_cell_ctxt->previous_cell_fsm_state,
						p_cell_ctxt->current_cell_fsm_state,
						ES_CELL_STATE_SWITCHED_OFF);
			}
		}
		SON_UT_TRACE_EXIT();

		return RELEASE_SON_BUFFER;
	}
	/*SPR11689 changes end */

	if (!son_get_x2_enabled())
	{
		cell_switch_on_resp.transaction_id = p_cell_switch_on_res->
			transaction_id;
		SON_MEMCPY(&cell_switch_on_resp.generic_res.cell_id,
				&p_cell_switch_on_res->generic_res.cell_id,
				sizeof(son_intra_rat_global_cell_id_t));

		if (SON_SUCCESS == p_cell_switch_on_res->generic_res.result)
		{
			SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
					p_cell_ctxt->previous_cell_fsm_state,
					p_cell_ctxt->current_cell_fsm_state,
					ES_CELL_STATE_ENABLED);
			/* Rel 3.0 Multi Sector code changes Start */
			/* Check if multiple cells exist at eNB and 
			   ANR_AUTO_MAINTAIN_LOCAL_CELL_AS_NBR config param is set to 
			   TRUE such that ANR maintains serving cells as neighbors of  
			   each others in NRTs of the all serving cells and the NRT 
			   needs to be updated based on whether cell is swithed ON or OFF */
			if (1 < anr_get_context_cell_count() && SON_TRUE ==
					anr_get_auto_maintain_local_cell_as_nbr_status())
			{
				es_send_sonanr_cell_switch_on_off_info_ind(
						&p_cell_ctxt->cell_id,
						ES_CELL_SWITCHED_ON);
			}
			/* Rel 3.0 Multi Sector code changes Stop */

            /*SPR 17777 +-*/
			SON_LOG(es_get_log_mode(), p_son_es_facility_name,
					SON_DETAILED,
					"Message [%d] "
					"received from RRM with result: [%d] and error code: [%d]"
                    "Unused variable evnt_id = %d",
					p_cspl_hdr->api,
					p_cell_switch_on_res->generic_res.result,
					p_cell_switch_on_res->generic_res.error_code,
                    evnt_id);
            /*SPR 17777 +-*/

			cell_switch_on_resp.generic_res.result = SON_SUCCESS;
			cell_switch_on_resp.generic_res.error_code = SON_NO_ERROR;
		}
		else
		{
			cell_switch_on_resp.generic_res.result = SON_FAILURE;
			cell_switch_on_resp.generic_res.error_code =
				es_map_rrm_error_codes( __func__,
						(rrm_return_et)p_cell_switch_on_res->generic_res.result,
						(rrm_error_et)(p_cell_switch_on_res->generic_res.error_code),
						(rrm_son_message_resp_et)(p_cspl_hdr->api));
			if (RRM_ERR_CELL_UNCONFIGURED == rrm_error_code)
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name,
						SON_ERROR,
						"Switch ON failed as Cell is not configured at RRM "
						" , cell deleted from ES Cell Context List");

				es_delete_cell_from_context((son_intra_rat_global_cell_id_t *)
						&(p_cell_switch_on_res->generic_res.cell_id));
			}
			else
			{
				SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
						p_cell_ctxt->previous_cell_fsm_state,
						p_cell_ctxt->current_cell_fsm_state,
						ES_CELL_STATE_SWITCHED_OFF);
			}
		}

		son_create_send_buffer((son_u8 *)&cell_switch_on_resp
				, SON_ES_MODULE_ID, SON_MIF_MODULE_ID
				, SONES_CELL_SWITCH_ON_RES
				, sizeof(son_es_cell_switch_on_res_t));
	}
	else
	{
		if (SON_SUCCESS == p_cell_switch_on_res->generic_res.result)
		{

			SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
					p_cell_ctxt->previous_cell_fsm_state,
					p_cell_ctxt->current_cell_fsm_state,
					ES_CELL_STATE_ENABLED);

			/* Rel 3.0 Multi Sector code changes Start */
			/* Check if multiple cells exist at eNB and 
			   ANR_AUTO_MAINTAIN_LOCAL_CELL_AS_NBR config param is set to 
			   TRUE such that ANR maintains serving cells as neighbors of  
			   each others in NRTs of the all serving cells and the NRT 
			   needs to be updated based on whether cell is swithed ON or OFF */
			if (1 < anr_get_context_cell_count() && SON_TRUE == 
					anr_get_auto_maintain_local_cell_as_nbr_status())
			{
				es_send_sonanr_cell_switch_on_off_info_ind(
						&p_cell_ctxt->cell_id,
						ES_CELL_SWITCHED_ON);
			}
			/* Rel 3.0 Multi Sector code changes Stop */

			SON_LOG(es_get_log_mode(), p_son_es_facility_name,
					SON_DETAILED,
					"Message [%d] received " 
					"from RRM with result: [%d] and error code: [%d]",
					p_cspl_hdr->api, p_cell_switch_on_res->generic_res.result,
					p_cell_switch_on_res->generic_res.error_code);

			p_list_node_anr = es_find_cell_in_switchon_pending_res(
					&p_cell_switch_on_res->generic_res.cell_id);

			if (SON_PNULL != p_list_node_anr)
			{
				if (ES_CELL_POWER_STATE_SWITCHED_OFF ==
						p_list_node_anr->cell_switchon_status.cell_power_state)
				{
					/* 
					 *   Cell was switched off and ANR requested to switch on 
					 */

					/*
					 *   Shall not send individual switch on response to OAM as
					 *   switch on was not triggered by OAM
					 */

					/*
					 *   1. Update pending response entry
					 */
					es_update_cell_in_switchon_pending_res( 
							&p_cell_switch_on_res->generic_res.cell_id,
							ES_CELL_POWER_STATE_SWITCHED_ON,
							SON_TRUE,
							X2_CELL_DEACTIVATION_FALSE,
							SON_SUCCESS,
							SON_NO_ERROR);

					/*
					 *   2.  Decremenet Expected res count
					 */
					es_decrement_switchon_pending_res_expected_count();

					/* 
					 *   3.  After decrement, shall check if there is no more
					 *   pending entries from RRM and can send the response 
					 *   back to ANR plus if required to OAM and X2 module too.
					 */
					if (MIN_PENDING_RES_COUNT ==
							es_get_switchon_pending_res_expected_count())
					{
						/*
						 *   1.  Send Switch on indication to X2 module as X2
						 *       support is enabled
						 */
						son_create_and_send_switch_on_ind_to_x2(
								SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ,
								NULL);

						/*
						 *   2. Send Switch off indication to OAM module
						 *      for all the other cells whose oam_ind = true
						 */
						son_create_and_send_cell_switch_on_off_ind_to_oam();

						/*
						 *   3. Send response to ANR for its request.
						 */
						son_create_and_send_x2_res_to_anr(
								es_get_switchon_pending_res_transaction_id(),
								SON_SUCCESS,
								SON_NO_ERROR);

						/*
						 *   4. Clear the pending response list
						 */
						es_clear_switchon_pending_res_list();
					}
				}
				else if (ES_CELL_POWER_STATE_SWITCHING_ON ==
						p_list_node_anr->cell_switchon_status.cell_power_state)
				{
					/* Cell was requested to switch on by OAM before
					 * ANR requested to switch on */

					/*
					 *   1. Cell switch on response shall go to OAM
					 */
					son_create_and_send_cell_switch_on_resp_to_oam(
							p_cell_switch_on_res->transaction_id,
							&p_cell_switch_on_res->generic_res.cell_id,
							SON_SUCCESS,
							SON_NO_ERROR);

					/*
					 *    2. Update the pending entry before sending switch on
					 *       request to RRM because of ANR's request
					 */
					es_update_cell_in_switchon_pending_res( 
							&p_cell_switch_on_res->generic_res.cell_id,
							ES_CELL_POWER_STATE_SWITCHED_ON,
							SON_FALSE,
							X2_CELL_DEACTIVATION_FALSE,
							SON_SUCCESS,
							SON_NO_ERROR);

					/*
					 *   3. Decrement the expected response count
					 */
					es_decrement_switchon_pending_res_expected_count();

					/* 
					 *   4.  After decrement, shall check if there is no more
					 *   pending entries from RRM and can send the response 
					 *   back to ANR plus if required to OAM and X2 module too.
					 */
					if (MIN_PENDING_RES_COUNT ==
							es_get_switchon_pending_res_expected_count())
					{
						/*
						 *   1. Send response to ANR for its request.
						 */
						son_create_and_send_x2_res_to_anr(
								es_get_switchon_pending_res_transaction_id(),
								SON_SUCCESS,
								SON_NO_ERROR);

						/*
						 *   2.  Send Switch on indication to X2 module as X2
						 *       support is enabled
						 */
						son_create_and_send_switch_on_ind_to_x2(
								SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ,
								NULL);

						/*
						 *   3. Send Switch off indication to OAM module
						 *      for all the other cells whose oam_ind = true
						 */
						son_create_and_send_cell_switch_on_off_ind_to_oam();

						/*
						 *   4. Clear the pending response list
						 */
						es_clear_switchon_pending_res_list();
					}
				}        
			}
			else 
			{
				/*
				 *   It means that Switch On request was from OAM 
				 */

				/*  
				 *   1. Switch On response shall go to OAM
				 */
				son_create_and_send_cell_switch_on_resp_to_oam(
						p_cell_switch_on_res->transaction_id,
						&p_cell_switch_on_res->generic_res.cell_id,
						SON_SUCCESS,
						SON_NO_ERROR);

				/*
				 *   2.  Send Switch on indication to X2 module as X2
				 *       support is enabled
				 */
				son_create_and_send_switch_on_ind_to_x2(
						SONES_CELL_SWITCH_ON_REQ,
						&p_cell_switch_on_res->generic_res.cell_id);
			}
		}
		else 
		{
			if (RRM_ERR_CELL_UNCONFIGURED == rrm_error_code)
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name,
						SON_ERROR,
						"Switch ON failed as Cell is not configured at RRM , cell"
						" deleted from ES Cell Context List");

				es_delete_cell_from_context(
						(son_intra_rat_global_cell_id_t *)
						&(p_cell_switch_on_res->generic_res.cell_id));
			}
			else
			{
				SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
						p_cell_ctxt->previous_cell_fsm_state,
						p_cell_ctxt->current_cell_fsm_state,
						ES_CELL_STATE_SWITCHED_OFF);
			}

			converted_error_code = es_map_rrm_error_codes( __func__,
					(rrm_return_et)p_cell_switch_on_res->generic_res.result,
					(rrm_error_et)(p_cell_switch_on_res->generic_res.error_code),
					(rrm_son_message_resp_et)(p_cspl_hdr->api));

			p_list_node_anr = es_find_cell_in_switchon_pending_res(
					&p_cell_switch_on_res->generic_res.cell_id);

			if (SON_PNULL != p_list_node_anr)
			{
				if (ES_CELL_POWER_STATE_SWITCHED_OFF ==
						p_list_node_anr->cell_switchon_status.cell_power_state)
				{
					/* 
					 *   i.e. Cell was switched off and ANR requested to 
					 *   switch on therefore switch on response shall not send
					 *   to OAM.
					 */

					/*
					 *   1. Update pending response entry
					 */
					es_update_cell_in_switchon_pending_res( 
							&p_cell_switch_on_res->generic_res.cell_id,
							ES_CELL_POWER_STATE_SWITCHED_OFF,
							SON_TRUE, X2_CELL_DEACTIVATION_FALSE,
							SON_FAILURE, converted_error_code);

					/*
					 *   2.  Decremenet Expected res count
					 */
					es_decrement_switchon_pending_res_expected_count();

					/*
					 *   3.  After decrement, shall check if there is no more
					 *   pending entries from RRM and can send the response
					 *   back to ANR plus if required to OAM and X2 module too.
					 */
					if (MIN_PENDING_RES_COUNT ==
							es_get_switchon_pending_res_expected_count())
					{
						/*
						 *   1. Send response to ANR for its request.
						 */
						son_create_and_send_x2_res_to_anr(
								es_get_switchon_pending_res_transaction_id(),
								SON_SUCCESS, SON_NO_ERROR);

						/*
						 *   2.  Send Switch on indication to X2 module as X2
						 *       support is enabled
						 */
						son_create_and_send_switch_on_ind_to_x2(
								SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ,
								NULL);

						/*
						 *   3. Send Switch off indication to OAM module
						 *      for all the other cells whose oam_ind = true
						 */
						son_create_and_send_cell_switch_on_off_ind_to_oam();

						/*
						 *   4. Clear the pending response list
						 */

						es_clear_switchon_pending_res_list();
					}
				}
				else if (ES_CELL_POWER_STATE_SWITCHING_ON ==
						p_list_node_anr->cell_switchon_status.cell_power_state)
				{
					/* 
					 *   i.e. Cell was requested to switch on by OAM before
					 *   ANR requested to switch on 
					 */

					/*
					 *   1. Shall Send the switch On response to OAM
					 */ 
					son_create_and_send_cell_switch_on_resp_to_oam(
							p_cell_switch_on_res->transaction_id,
							&p_cell_switch_on_res->generic_res.cell_id,
							SON_FAILURE,
							converted_error_code);

					/*
					 *   2. Update pending response entry
					 */
					es_update_cell_in_switchon_pending_res( 
							&p_cell_switch_on_res->generic_res.cell_id,
							ES_CELL_POWER_STATE_SWITCHED_OFF,
							SON_FALSE,
							X2_CELL_DEACTIVATION_FALSE,
							SON_FAILURE,
							converted_error_code);

					/*
					 *   3.  Decremenet Expected res count
					 */
					es_decrement_switchon_pending_res_expected_count();

					/*
					 *   4.  After decrement, shall check if there is no more
					 *   pending entries from RRM and can send the response
					 *   back to ANR plus if required to OAM and X2 module too.
					 */
					if (MIN_PENDING_RES_COUNT ==
							es_get_switchon_pending_res_expected_count())
					{
						/*
						 *   1. Send response to ANR for its request.
						 */
						son_create_and_send_x2_res_to_anr(
								es_get_switchon_pending_res_transaction_id(),
								SON_SUCCESS,
								SON_NO_ERROR);

						/*
						 *   2.  Send Switch on indication to X2 module as X2
						 *       support is enabled
						 */
						son_create_and_send_switch_on_ind_to_x2(
								SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ,
								NULL);

						/*
						 *   3. Send Switch off indication to OAM module
						 *      for all the other cells whose oam_ind = true
						 */
						son_create_and_send_cell_switch_on_off_ind_to_oam();

						/*
						 *   4. Clear the pending response list
						 */
						es_clear_switchon_pending_res_list();
					}
				}        
			}
			else 
			{   
				/*
				 *   i.e. OAM requested the switch on
				 */

				/*
				 *   1. Send swith off response to OAM
				 */
				son_create_and_send_cell_switch_on_resp_to_oam(
						p_cell_switch_on_res->transaction_id,
						&p_cell_switch_on_res->generic_res.cell_id,
						SON_FAILURE,
						converted_error_code);

				/*
				 *   2. Since switch on is failed no need to send indication to X2
				 */
			}
		}
	} 

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_cell_switch_on_res_handler */


/******************************************************************************
 * Function Name  : es_construct_and_send_cell_activation_response
 * Inputs         : None 
 * Outputs        : None
 * Returns        : None 
 * Description    : This function iconstructs and sends SONES_CELL_ACTIVATION_RESP
 *                  to SONX2.
 *****************************************************************************/
/* + SPR 17439 */
	son_void_t
es_construct_and_send_cell_activation_response(void)
/* - SPR 17439 */
{

	son_es_cell_activation_resp_t cell_activation_res;
	es_cell_context_node_t *p_cell_ctxt_node = SON_PNULL;
	es_cell_context_t      *p_cell_ctxt     = SON_PNULL;

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&cell_activation_res,0,sizeof(son_es_cell_activation_resp_t));
	/*Pointer to the first context node*/
	p_cell_ctxt_node = es_get_first_cell_ctxt();
	while (SON_PNULL != p_cell_ctxt_node)
	{
		/*Get the data part of the node*/
		p_cell_ctxt = &p_cell_ctxt_node->data;
		if(p_cell_ctxt->cell_activation_req_flag == SON_TRUE &&
				p_cell_ctxt->rrm_cell_activation_response == SON_SUCCESS)
		{

			cell_activation_res.result =  SON_SUCCESS;
			cell_activation_res.error_code =  SON_NO_ERROR;

			SON_MEMCPY(&cell_activation_res.cell_activation_info.served_cell_list[
					cell_activation_res.cell_activation_info.num_served_cell],
					&p_cell_ctxt->cell_id,
					sizeof(son_intra_rat_global_cell_id_t));

			cell_activation_res.cell_activation_info.num_served_cell++;

		}
		if(p_cell_ctxt->cell_activation_req_flag == SON_TRUE)
		{
			/*copy target enb Id*/
			SON_MEMCPY(& cell_activation_res.enbid_1,
					&p_cell_ctxt->enbid_1,
					sizeof(son_global_enb_id_t));
			SON_MEMCPY(& cell_activation_res.enbid_2,
					&p_cell_ctxt->enbid_2,
					sizeof(son_global_enb_id_t));

		}
		/*find next node*/
		p_cell_ctxt_node = es_get_next_node(p_cell_ctxt_node);
	}
	if( cell_activation_res.result !=  SON_SUCCESS)
	{
		cell_activation_res.result =  SON_FAILURE;
		cell_activation_res.error_code =  SON_ERR_RRM_FAILURE;
	}

	son_create_send_buffer((son_u8 *)&cell_activation_res, SON_ES_MODULE_ID,
			SON_X2_MODULE_ID, SONES_CELL_ACTIVATION_RESP,
			sizeof(cell_activation_res));


	SON_UT_TRACE_EXIT();
} /* es_construct_and_send_cell_activation_response */


/******************************************************************************
 * Function Name  : es_map_switch_off_mode
 * Inputs         : son_switch_off_mode SON switch off mode enum value
 * Outputs        : None
 * Returns        : RRM Switch off mode enum value 
 * Description    : This function maps SON switch off mode enum value to RRM switch 
 *                  off mode enum value.
 *****************************************************************************/
static rrm_switchoff_et
es_map_switch_off_mode
(
 son_cell_switch_off_mode_et son_switch_off_mode
 )
{
	rrm_switchoff_et rrm_switch_off_mode = RRM_SO_UNDEF;


	SON_UT_TRACE_ENTER();

	switch (son_switch_off_mode)
	{
		case SO_GRACEFUL:
			{
				rrm_switch_off_mode = RRM_SO_GRACEFUL;
				break;
			}

		case SO_FORCEFUL:
			{
				rrm_switch_off_mode = RRM_SO_FORCEFUL;
				break;
			}

		default:
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name,
						SON_ERROR,
						"es_map_switch_off_mode: "
						"Can not map switch off mode for invalid input value [%u]",
						son_switch_off_mode);
				break;
			}
	} /* switch */

	SON_UT_TRACE_EXIT();
	return rrm_switch_off_mode;
} /* es_map_switch_off_mode */

/******************************************************************************
 * Function Name  : es_fsm_cell_switch_off_req_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                  p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_CELL_SWITCH_OFF_REQ.
 *****************************************************************************/
son_buf_retain_status_et
es_fsm_cell_switch_off_req_handler
(
 son_es_index_et     evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	SON_UT_TRACE_ENTER();

	son_es_cell_switch_off_req_t *p_cell_switch_off_req = 
		(son_es_cell_switch_off_req_t *)p_msg;
	rrm_son_cell_switch_off_req_t  switch_off_req;

	/* AUTONOMOUS SWITCH OFF START */
	SON_MEMCPY(&switch_off_req.cell_id, &p_cell_ctxt->cell_id,
			sizeof(son_intra_rat_global_cell_id_t));
	/* AUTONOMOUS SWITCH OFF END */

	switch_off_req.switchoff_type = es_map_switch_off_mode(
			p_cell_switch_off_req->switch_off_req.switch_off_mode);
	switch_off_req.switchoff_time =  ES_CELL_SWITCH_OFF_TIME;

	/*PUP Tool call to create and send CELL SWITCH OFF Message to RRM */
	if (RRM_SUCCESS == rrm_son_send_rrm_son_cell_switch_off_req(
				&switch_off_req,
				SON_ES_MODULE_ID, SON_RRM_MODULE_ID,
				p_cell_switch_off_req->transaction_id, SON_NULL))
	{                                   
		SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ES_CELL_STATE_SWITCHING_OFF);
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_BRIEF,
				"Message [%u] send from [%u] to [%u] for Cell: [0x%x]",
				RRM_SON_CELL_SWITCH_OFF_REQ,
				SON_ES_MODULE_ID,
				SON_RRM_MODULE_ID,
				son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
	}
	else
	{
		SON_LOG(es_get_log_mode(), 
				p_son_es_facility_name, 
				SON_ERROR,
				"Message sending failure to RRM");
                /*SPR 17777 +-*/
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
		LTE_GCC_UNUSED_PARAM(evnt_id);
	}

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_cell_switch_off_req_handler */

/******************************************************************************
 * Function Name  : es_fsm_cell_switch_off_res_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                  p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_CELL_SWITCH_OFF_RES.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_cell_switch_off_res_handler
(
 son_es_index_et     evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_es_cell_switch_off_res_t *p_cell_switch_off_res = 
		(son_es_cell_switch_off_res_t *)p_msg;
	rrm_error_et rrm_error_code = 
		((rrm_son_non_registration_based_res_t *)(&p_cell_switch_off_res->generic_res))->error_code;
	son_es_cell_switch_on_req_t         req          = {0};
	cell_switchon_status_list_data_t    *p_list_node = SON_PNULL;
	son_error_et                        converted_error_code;


	SON_UT_TRACE_ENTER();

	if (SON_SUCCESS == p_cell_switch_off_res->generic_res.result)
	{
		SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ES_CELL_STATE_SWITCHED_OFF);

		/* Rel 3.0 Multi Sector code changes Start */
		/* Check if multiple cells exist at eNB then indicate ANR 
		   such that ANR deletes NRT entry of serving cell maintained 
		   as neighbor in the NRT of other serving cells */
		if (1 < anr_get_context_cell_count())
		{
			es_send_sonanr_cell_switch_on_off_info_ind(
					&p_cell_ctxt->cell_id,
					ES_CELL_SWITCHED_OFF);
		}
		/* Rel 3.0 Multi Sector code changes Stop */

		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_INFO,
				"Message [%d] "
				"received from RRM with result: [%d] and error code: [%d]",
				p_cspl_hdr->api,
				p_cell_switch_off_res->generic_res.result,
				p_cell_switch_off_res->generic_res.error_code);
		if (son_get_x2_enabled())
		{
			p_list_node = es_find_cell_in_switchon_pending_res(
					&p_cell_switch_off_res->generic_res.cell_id);

			if (SON_PNULL != p_list_node)
			{
				/* 
				 *  Before ANR's Cell Switch On request , OAM's Switch off was 
				 *  underprocess and noew OAM's switch off response is recvd 
				 */

				/*
				 *    1. Send this switch off response to OAM 
				 */
				son_create_and_send_cell_switch_off_resp_to_oam(
						p_cell_switch_off_res->transaction_id,
						&p_cell_switch_off_res->generic_res.cell_id,
						SON_SUCCESS,
						SON_NO_ERROR);

				/*
				 *   2.  Send Switch off indication to X2 module as X2
				 *       support is enabled
				 */
				son_create_and_send_switch_off_ind_to_x2(
						&p_cell_switch_off_res->generic_res.cell_id);

				/*
				 *    3. Update the pending entry before sending switch on 
				 *       request to RRM because of ANR's request
				 */
				es_update_cell_in_switchon_pending_res( 
						&p_cell_switch_off_res->generic_res.cell_id,
						ES_CELL_POWER_STATE_SWITCHED_OFF,
						SON_FALSE,
						X2_CELL_DEACTIVATION_TRUE,
						SON_FAILURE,
						SON_ERR_TIMER_EXPIRED);

				/*
				 *    4. Sending switch on request to RRM
				 */
				SON_MEMCPY(&req.switch_on_req.srv_cgi,
						&p_cell_switch_off_res->generic_res.cell_id,
						sizeof(son_intra_rat_global_cell_id_t));
				req.transaction_id = 
					es_get_switchon_pending_res_transaction_id();

				es_cell_process_msg(IDX_SONES_CELL_SWITCH_ON_REQ,
						p_cell_ctxt, p_cspl_hdr, (son_u8 *)&req);
			}
			else   
			{
				/* AUTONOMOUS SWITCH OFF START
				 * If Autonomous cell switch off req was triggerred by ES */
				if (SON_ES_AUTONOMOUS_SWITCH_OFF_REQ_SENT_TO_RRM & p_cell_ctxt->autonomous_switch_off_info)
				{
					/*
					 *   Switch off response received for Autonomous switch off
					 *   request by ES and there is nothing to be done for this
					 *   cell with ANR's perspective
					 */

					/*
					 *    1. Send this switch off ind to OAM
					 */

					son_create_and_send_cell_switch_on_off_ind_to_oam_for_autonomous_switch_off(p_cell_ctxt);

					/* Reset the "Autonomous Req Sent" status */
					p_cell_ctxt->autonomous_switch_off_info ^= SON_ES_AUTONOMOUS_SWITCH_OFF_REQ_SENT_TO_RRM;
					/* Fix 875 Start */
					p_cell_ctxt->switch_off_cond_hit_status = SON_NULL;
					/* Fix 875 End */
				}
				else
				{
					/* AUTONOMOUS SWITCH OFF END */

					/*
					 *   Switch off response received for OAM's switch off
					 *   request and there is nothing to be done for this
					 *   cell with ANR's perspective
					 */

					/*
					 *    1. Send this switch off response to OAM
					 */
					son_create_and_send_cell_switch_off_resp_to_oam(
							p_cell_switch_off_res->transaction_id,
							&p_cell_switch_off_res->generic_res.cell_id,
							SON_SUCCESS,
							SON_NO_ERROR);
				}

				/*
				 *   2.  Send Switch off indication to X2 module as X2
				 *       support is enabled
				 */
				son_create_and_send_switch_off_ind_to_x2(
						&p_cell_switch_off_res->generic_res.cell_id);
			}
		}
		else
		{
			/* AUTONOMOUS SWITCH OFF START
			 * If Autonomous cell switch off req was triggerred by ES */
			if (SON_ES_AUTONOMOUS_SWITCH_OFF_REQ_SENT_TO_RRM & p_cell_ctxt->autonomous_switch_off_info)
			{
				/*
				 *   Switch off response received for Autonomous switch off
				 *   request by ES and there is nothing to be done for this
				 *   cell with ANR's perspective
				 */

				/*
				 *    1. Send switch off ind to OAM
				 */

				son_create_and_send_cell_switch_on_off_ind_to_oam_for_autonomous_switch_off(p_cell_ctxt);

				/* Reset the "Autonomous Req Sent" status */
				p_cell_ctxt->autonomous_switch_off_info ^= SON_ES_AUTONOMOUS_SWITCH_OFF_REQ_SENT_TO_RRM;
				/* Fix 875 Start */
				p_cell_ctxt->switch_off_cond_hit_status = SON_NULL;
				/* Fix 875 End */
			}
			else
			{
				/* 
				 *   Switch off response received for OAM's switch off 
				 *   request and there is nothing to be done for this 
				 *   cell with ANR's perspective as X2 is disabled.
				 */

				/*
				 *    1. Send this switch off response to OAM 
				 */
				son_create_and_send_cell_switch_off_resp_to_oam(
						p_cell_switch_off_res->transaction_id,
						&p_cell_switch_off_res->generic_res.cell_id,
						SON_SUCCESS,
						SON_NO_ERROR);
			}
		}
	}
	else 
	{
		converted_error_code = es_map_rrm_error_codes( __func__,
				(rrm_return_et)p_cell_switch_off_res->generic_res.result,
				(rrm_error_et)(p_cell_switch_off_res->generic_res.error_code),
				(rrm_son_message_resp_et)(p_cspl_hdr->api)); 

		if (RRM_ERR_CELL_UNCONFIGURED == rrm_error_code)
		{
			SON_LOG(es_get_log_mode(), p_son_es_facility_name,
					SON_ERROR,
					"Switch OFF failed as Cell is not configured at RRM ,"
					"cell deleted from ES Cell Context List");

			es_delete_cell_from_context((son_intra_rat_global_cell_id_t *) 
					&(p_cell_switch_off_res->generic_res.cell_id));
		}
		else
		{
			SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
					p_cell_ctxt->previous_cell_fsm_state,
					p_cell_ctxt->current_cell_fsm_state,
					ES_CELL_STATE_ENABLED);
		}   

		/* AUTONOMOUS SWITCH OFF START
		 * If Autonomous cell switch off req was triggerred by ES */
		if (SON_ES_AUTONOMOUS_SWITCH_OFF_REQ_SENT_TO_RRM & p_cell_ctxt->autonomous_switch_off_info)
		{
            /*SPR 17777 +-*/
			SON_LOG(es_get_log_mode(), p_son_es_facility_name,
					SON_WARNING,
					"Message [%d] "
					"received from RRM for AUTONOMOUS Switch Off Req "
					"with (FAILURE) result: [%d] and error code: [%d]",
					p_cspl_hdr->api,
					p_cell_switch_off_res->generic_res.result,
					p_cell_switch_off_res->generic_res.error_code);

                    LTE_GCC_UNUSED_PARAM(evnt_id);
            /*SPR 17777 +-*/

			/* Reset the "Autonomous Req Sent" status */
			p_cell_ctxt->autonomous_switch_off_info ^= SON_ES_AUTONOMOUS_SWITCH_OFF_REQ_SENT_TO_RRM;
			/* Fix 875 Start */
			p_cell_ctxt->switch_off_cond_hit_status = SON_NULL;
			/* Fix 875 End */
		}
		else
		{
			/* AUTONOMOUS SWITCH OFF END */

			if (son_get_x2_enabled())
			{
				p_list_node = es_find_cell_in_switchon_pending_res(
						&p_cell_switch_off_res->generic_res.cell_id);
				if (SON_PNULL !=  p_list_node)
				{
					/*
					 *   Switch off response received for OAM's switch off
					 *   request and there is nothing to be done for this
					 *   cell with ANR's perspective
					 */

					/*
					 *    1. Send this switch off response to OAM
					 */
					son_create_and_send_cell_switch_off_resp_to_oam(
							p_cell_switch_off_res->transaction_id,
							&p_cell_switch_off_res->generic_res.cell_id,
							SON_FAILURE,
							converted_error_code
							);

					if (RRM_ERR_CELL_UNCONFIGURED == rrm_error_code)
					{
						es_update_cell_in_switchon_pending_res(
								&p_cell_switch_off_res->generic_res.cell_id,
								ES_CELL_POWER_STATE_SWITCHED_OFF,
								SON_FALSE,
								X2_CELL_DEACTIVATION_FALSE,
								SON_FAILURE,
								converted_error_code);

						es_decrement_switchon_pending_res_expected_count();
					}
					else
					{
						/*
						 *  2. Delete the entry from pending list as this 
						 *  cell is not switched off so we will consider it
						 *  as already switched on cell.
						 */
						es_delete_cell_from_switchon_pending_res(
								&p_cell_switch_off_res->
								generic_res.cell_id);
					}

					/* 
					 *   3.  After deletion shall check if there is no more
					 *   pending entries from RRM and can send the response 
					 *   back to ANR plus if required to OAM and X2 module too.
					 */
					if (MIN_PENDING_RES_COUNT ==
							es_get_switchon_pending_res_expected_count())
					{ 
						if (!es_switchon_pending_res_has_entry())
						{
							/*
							 *   1. Check if there is any cell entry in pending 
							 *      response after expected response count becomes 
							 *      zero, if there is no entry, that means nothing 
							 *      to send to X2 or OAM but we must send response
							 *      to ANR for its request with SUCCESS.
							 */
							son_create_and_send_x2_res_to_anr(
									es_get_switchon_pending_res_transaction_id(),
									SON_SUCCESS,
									SON_NO_ERROR);

							/*
							 *   2. Clear the pending response list
							 */
							es_clear_switchon_pending_res_list();
						}
						else
						{
							/*
							 *   1. Check if there is any cell entry in pending 
							 *      response after expected response count becomes 
							 *      zero to send a response to ANR,X2 or OAM.
							 */
							son_create_and_send_x2_res_to_anr(
									es_get_switchon_pending_res_transaction_id(),
									SON_SUCCESS,
									SON_NO_ERROR);

							/*  
							 *   2.  Send Switch on indication to X2 module as X2 
							 *       support is enabled
							 */
							son_create_and_send_switch_on_ind_to_x2( 
									SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ,
									&p_cell_switch_off_res->generic_res.cell_id);

							/*  
							 *   3. Send Switch off indication to OAM module 
							 *      for all the other cells whose oam_ind = true
							 */
							son_create_and_send_cell_switch_on_off_ind_to_oam();

							/*
							 *   4. Clear the pending response list
							 */
							es_clear_switchon_pending_res_list();
						}
					}
				}
				else 
				{
					/* 
					 *   Switch off response received for OAM's switch off 
					 *   request and there is nothing to be done for this 
					 *   cell with ANR's perspective
					 */

					/*
					 *    1. Send this switch off response to OAM 
					 */
					son_create_and_send_cell_switch_off_resp_to_oam(
							p_cell_switch_off_res->transaction_id,
							&p_cell_switch_off_res->generic_res.cell_id,
							SON_FAILURE,
							converted_error_code
							);
				}
			}
			else    
			{
				/* 
				 *   Switch off response received for OAM's switch off 
				 *   request and there is nothing to be done for this 
				 *   cell with ANR's perspective
				 */

				/*
				 *    1. Send this switch off response to OAM 
				 */
				son_create_and_send_cell_switch_off_resp_to_oam(
						p_cell_switch_off_res->transaction_id,
						&p_cell_switch_off_res->generic_res.cell_id,
						SON_FAILURE,
						converted_error_code
						);
			}
		}
	}

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_cell_switch_off_res_handler */

/*****************************************************************************
 * Function Name  : es_fsm_set_attribute_value_res_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SON_RRM_SET_ATTRIBUTE_RES.
 ****************************************************************************/
static son_buf_retain_status_et
es_fsm_set_attribute_value_res_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t *p_cell_ctxt,
 STACKAPIHDR    *p_cspl_hdr,
 son_u8			  *p_msg
 )
{
	son_rrm_set_attribute_res_t *p_res 	= (son_rrm_set_attribute_res_t *)p_msg;
	son_generic_response_with_cell_id_t	modify_res = {0};


	SON_UT_TRACE_ENTER();

	son_procedure_code_et api  = SON_PROCEDURE_UNDEFINED;

	if (RRM_ATTR_ACTIVE_UE_THRESHOLD == p_res->res.id) 
	{
		api = SONES_MODIFY_UE_COUNT_THRESHOLD_RES;
		/* Fix 875 Start */
		if(RRM_SUCCESS == p_res->res.set_attr_resp.result)
		{
			SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
					"Modified UE Count Threshold Value. Old: [%d], New: [%d]",
					p_cell_ctxt->ue_count_threshold, p_cell_ctxt->modify_ue_count_threshold_val);

			/* If new threshold value is less than the old one, and Low UE count condition for
			   Auto Switch Off is already hit, need to stop the timer */
			if((SON_PNULL != p_cell_ctxt->es_timer_id) && 
					(p_cell_ctxt->ue_count_threshold > p_cell_ctxt->modify_ue_count_threshold_val))
			{
				son_stop_timer(p_cell_ctxt->es_timer_id);
			}
			p_cell_ctxt->ue_count_threshold = p_cell_ctxt->modify_ue_count_threshold_val;
		}
		p_cell_ctxt->modify_ue_count_threshold_val = SON_NULL;
		/* Fix 875 End */
	}
	else if (RRM_ATTR_ACTIVE_UE_COUNT_REPORT == p_res->res.id)
	{
		api = SONES_MODIFY_UE_COUNT_INTERVAL_RES;
		/* Fix 875 Start */
		if(RRM_SUCCESS == p_res->res.set_attr_resp.result)
		{
			SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
					"Modified UE Report Interval Value. Old: [%d], New: [%d]",
					p_cell_ctxt->ue_report_interval, p_cell_ctxt->modify_ue_report_interval);

			p_cell_ctxt->ue_report_interval = p_cell_ctxt->modify_ue_report_interval;
		}
		p_cell_ctxt->modify_ue_report_interval = SON_NULL;
		/* Fix 875 End */
	}
	else
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
				"Unexpected attribute id [%d] received in set attribute response "
				"form: [%d] for API: [%s]", p_res->res.id, p_cspl_hdr->from,
				SON_ES_CELL_FSM_EVENT_NAMES[evnt_id]); 
	}

	modify_res.transaction_id = p_res->transaction_id;
	SON_MEMCPY(&modify_res.generic_res.cell_id,
			&p_res->res.set_attr_resp.id.object_id.cell_id,
			sizeof(son_intra_rat_global_cell_id_t));

	if (RRM_FAILURE == p_res->res.set_attr_resp.result)
	{
		modify_res.generic_res.result = SON_FAILURE;
		modify_res.generic_res.error_code = es_map_rrm_error_codes(
				__func__,
				p_res->res.set_attr_resp.result, 
				p_res->res.set_attr_resp.error_code,
				(rrm_son_message_resp_et)(p_cspl_hdr->api)
				);

		if (RRM_ERR_OBJECT_NON_EXISTENT == p_res->res.set_attr_resp.error_code ||
				RRM_ERR_CELL_UNCONFIGURED == p_res->res.set_attr_resp.error_code)
		{
			SON_LOG(es_get_log_mode(), p_son_es_facility_name,
					SON_ERROR,
					"Modifying Attribute failed as Cell not configured,"
					" cell [Cell Id: %d] deleted from ES Cell Context List",
					son_convert_char_cell_id_to_int_cell_id(p_res->res.set_attr_resp.id.
						object_id.cell_id.cell_identity));

			es_delete_cell_from_context((
						(son_intra_rat_global_cell_id_t *)&p_res->
						res.set_attr_resp.id.object_id.cell_id));
		}
	}
	else
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_DETAILED,
				"Message [%d] received from RRM with result: [%d] and error code: [%d]",
				p_cspl_hdr->api,
				p_res->res.set_attr_resp.result,
				p_res->res.set_attr_resp.error_code);

		modify_res.generic_res.error_code =  SON_NO_ERROR;
		modify_res.generic_res.result = SON_SUCCESS;
	}

	son_create_send_buffer((son_u8 *)&modify_res, SON_ES_MODULE_ID,
			SON_MIF_MODULE_ID, api,
			sizeof(modify_res));

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_set_attribute_value_res_handler */

/*****************************************************************************
 * Function Name  : es_fsm_active_ue_cnt_rprt_handler 
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for RRM_ACTIVE_UE_COUNT_REPORT. 
 ****************************************************************************/
static son_buf_retain_status_et
es_fsm_active_ue_cnt_rprt_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	SON_UT_TRACE_ENTER();

	/* AUTONOMOUS SWITCH OFF START */
	/* Fix 875 Start */
	son_es_active_ue_cnt_report_t *p_active_ue_cnt_report = (son_es_active_ue_cnt_report_t *)p_msg;

	if ((SON_ES_AUTONOMOUS_SWITCH_OFF_ENABLED & p_cell_ctxt->autonomous_switch_off_info)
			&& (SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_UE_CNT & p_cell_ctxt->autonomous_switch_off_info))
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
				"Auotonomous Switch Off- Active UE Count Criteria ENABLED for the cell.");

		if (p_active_ue_cnt_report->ue_cnt <= p_cell_ctxt->ue_count_threshold)
		{
            /*SPR 17777 +-*/
			SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
					"UE count report received from RRM with active UEs "
					"LESS than or EQUAL to the threshold count [%d]"
                    "Unused variable evnt_id=%d p_cspl_hdr=%p", 
					p_cell_ctxt->ue_count_threshold,evnt_id,p_cspl_hdr);
            /*SPR 17777 +-*/
			/* If monitoring of reports to auto switch off cell is not going on */
			if(SON_PNULL == p_cell_ctxt->es_timer_id)
			{
				p_cell_ctxt->switch_off_cond_hit_status |= SON_ES_SWITCH_OFF_LOW_UE_CNT_COND_HIT;
				/* If Load check criteria is NOT set, OR otherwise the less load condition is already hit */
				if(!(SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_LOAD_LEVEL & p_cell_ctxt->autonomous_switch_off_info) ||
						((SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_LOAD_LEVEL & p_cell_ctxt->autonomous_switch_off_info) &&
						 (SON_ES_SWITCH_OFF_LOW_LOAD_COND_HIT & p_cell_ctxt->switch_off_cond_hit_status)))
				{
					/* Start the timer to monitor the reports */
					/* SPR-12708 Fix Starts */
					p_cell_ctxt->es_timer_id = son_start_timer(
							g_es_context.autonomous_switch_off_timer_val,
							&p_cell_ctxt->cell_id, sizeof(son_intra_rat_global_cell_id_t), SON_FALSE);
					/* SPR-12708 Fix Ends */
				}
			}
			else
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
						"Monitoring of the reports already going on");
			}
		}
		else
		{
			SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
					"UE count report received from RRM with active UEs ABOVE than the threshold count [%d]",
					p_cell_ctxt->ue_count_threshold);

			if(SON_ES_SWITCH_OFF_LOW_UE_CNT_COND_HIT & p_cell_ctxt->switch_off_cond_hit_status)
			{
				p_cell_ctxt->switch_off_cond_hit_status ^= SON_ES_SWITCH_OFF_LOW_UE_CNT_COND_HIT;

				SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
						"Low UE count condition was hit earlier. Status cleared now");

				/* If monitoring of reports to auto switch off cell is going on due to Low load
				   and low UE count both OR low UE count only, stop the timer*/
				if(p_cell_ctxt->es_timer_id)
				{
					son_stop_timer(p_cell_ctxt->es_timer_id);
					p_cell_ctxt->es_timer_id = SON_PNULL;
				}
			}
		}
	}
	else
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
				"Auotonomous Switch Off- Active UE Count Criteria DISABLED for the cell.");
	}
	/* Fix 875 Start */
	/* AUTONOMOUS SWITCH OFF END */

	son_create_send_buffer(p_msg
			, SON_ES_MODULE_ID, SON_MIF_MODULE_ID
			, SONES_ACTIVE_UE_COUNT_REPORT
			, sizeof(son_es_active_ue_cnt_report_t));

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : es_fsm_ue_cnt_threshold_hit_ind_handler 
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for RRM_UE_COUNT_THRESHOLD_HIT_IND.
 ****************************************************************************/
static son_buf_retain_status_et
es_fsm_ue_cnt_threshold_hit_ind_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	SON_UT_TRACE_ENTER();

	/* Fix 875 Start -delete */

	son_create_send_buffer(p_msg
			, SON_ES_MODULE_ID, SON_MIF_MODULE_ID
			, SONES_THRESHOLD_HIT_IND
			, sizeof(son_es_threshold_hit_ind_t));

     /*SPR 17777 +-*/
    SON_LOG(es_get_log_mode(), p_son_es_facility_name,SON_DETAILEDALL,
            "Unused variable evnt_id=%d p_cspl_hdr=%p p_cell_ctxt=%p",evnt_id,p_cspl_hdr,p_cell_ctxt)
     /*SPR 17777 +-*/
	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_ue_cnt_threshold_hit_ind_handler */

/*****************************************************************************
 * Function Name  : es_fsm_s1_msg_ind_handler 
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for RRM_S1_MSG_IND. 
 ****************************************************************************/
static son_buf_retain_status_et
es_fsm_s1_msg_ind_handler
(   
 son_es_index_et    evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_msg
 )
{
	son_es_request_to_switch_on_cell_t req_to_switch_on_cell;


	SON_MEMSET(&req_to_switch_on_cell.srv_cgi, 0,
			sizeof(son_intra_rat_global_cell_id_t));
	req_to_switch_on_cell.cause   = CAUSE_UNDEFINED;

	rrm_s1_msg_ind_t *s1_msg_ind = (rrm_s1_msg_ind_t *)p_msg;

	SON_UT_TRACE_ENTER();

	SON_MEMCPY(&(req_to_switch_on_cell.srv_cgi),
			&s1_msg_ind->cell_id, 
			sizeof(son_intra_rat_global_cell_id_t));
	req_to_switch_on_cell.cause = RECEPTION_OF_S1_MSG;

	son_create_send_buffer((son_u8 *)&req_to_switch_on_cell
			, SON_ES_MODULE_ID, SON_MIF_MODULE_ID
			, SONES_REQUEST_TO_SWITCH_ON_CELL
			, sizeof(son_es_request_to_switch_on_cell_t));
     /*SPR 17777 +-*/
    SON_LOG(es_get_log_mode(), p_son_es_facility_name,SON_DETAILEDALL,
            "Unused variable evnt_id=%d p_cspl_hdr=%p p_cell_ctxt=%p",evnt_id,p_cspl_hdr,p_cell_ctxt)
     /*SPR 17777 +-*/
	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_s1_msg_ind_handler */

/******************************************************************************
 * Function Name  : es_fsm_invalid_event_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for logging invalid event it is invoked when
 * 					an unexpected event occurs.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_invalid_event_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t 	*p_cell_ctxt,
 STACKAPIHDR        *p_cspl_hdr,
 son_u8				*p_msg
 )
{	
	SON_UT_TRACE_ENTER();
     /*SPR 17777 +-*/
	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
			"Unexpected Message [%s] received in "
			"[%s] state for the cell [0x%x] from [%d]",
			SON_ES_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->from);

	LTE_GCC_UNUSED_PARAM(p_msg);
     /*SPR 17777 +-*/

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
}

/******************************************************************************
 * Function Name  : es_fsm_cell_state_change_handler 
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler to handle RRM_CELL_STATE_CHANGE_IND message.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_cell_state_change_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t 	*p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8				*p_msg
 )
{	
	son_cell_state_change_indication_t *p_ind =
		(son_cell_state_change_indication_t *)p_msg;
	son_feature_state_et                state = SON_STATE_UNDEFINED;
	son_feature_state_change_cause_et   cause = 
		SON_STATE_CHANGE_CAUSE_UNDEFINED;


	SON_UT_TRACE_ENTER();

	if (RRM_CELL_DELETED ==  p_ind->cell_state)
	{
		state = SON_STATE_DELETED;
		cause = SON_RRM_CELL_UNCONFIGURED;

		if (es_get_pending_res_expected_count())
		{
			es_delete_cell_from_pending_res
				((son_intra_rat_global_cell_id_t *)&p_ind->cell_id);
		}

		if (SON_TRUE == es_delete_cell_from_context
				((son_intra_rat_global_cell_id_t *)&p_ind->cell_id))
		{
			SON_LOG(es_get_log_mode(), p_son_es_facility_name,
					SON_INFO,
					"Cell 0x%x deleted successfully from ES Cell Context List",
					son_convert_char_cell_id_to_int_cell_id(p_ind->cell_id.cell_identity));
		}
		else
		{
			SON_LOG(es_get_log_mode(), p_son_es_facility_name,
					SON_ERROR,
					"Cell 0x%x could not be deleted from ES Cell Context List",
					son_convert_char_cell_id_to_int_cell_id(p_ind->cell_id.cell_identity));
		}
	}
	else if (RRM_CELL_UNOPERATIONAL ==  p_ind->cell_state)
	{
		/* PR 551 Fix Start */    
		state = SON_STATE_ENABLED;
		/* PR 551 Fix End */    
		cause = SON_RRM_CELL_UNOPERATIONAL;

		if (es_get_pending_res_expected_count())
		{
			es_delete_cell_from_pending_res
				((son_intra_rat_global_cell_id_t *)&p_ind->cell_id);
		}

		/* Rel 3.0 Multi Sector code changes Start */
		if (ES_CELL_STATE_SWITCHED_OFF != 
				p_cell_ctxt->current_cell_fsm_state)
		{
			/* Rel 3.0 Multi Sector code changes Stop */
			SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
					p_cell_ctxt->previous_cell_fsm_state,
					p_cell_ctxt->current_cell_fsm_state,
					ES_CELL_STATE_SWITCHED_OFF);
			/* SPR-12483 Fix Starts */
			/* Lines Deleted */
			p_cell_ctxt->current_cell_state = RRM_CELL_UNOPERATIONAL;
			/* SPR-12483 Fix Ends */
		}
	}
	/* PR 551 Fix Start */    
	else if (RRM_CELL_OPERATIONAL == p_ind->cell_state)
	{
		state = SON_STATE_ENABLED;
		cause = SON_RRM_CELL_OPERATIONAL;

		SONES_CELL_FSM_SET_STATE(p_cell_ctxt,
				p_cell_ctxt->previous_cell_fsm_state,
				p_cell_ctxt->current_cell_fsm_state,
				ES_CELL_STATE_ENABLED);

		/* SPR-12483 Fix Starts */
		p_cell_ctxt->current_cell_state = RRM_CELL_OPERATIONAL;
		/* SPR-12483 Fix Ends */
	}
	/* PR 551 Fix End */    
	else
	{
        /*SPR 17777 +-*/
		SON_LOG(es_get_log_mode(), p_son_es_facility_name,
				SON_ERROR,
				"Invalid Cell state[%d]"
				"received from RRM");
		LTE_GCC_UNUSED_PARAM(p_ind->cell_state);
		LTE_GCC_UNUSED_PARAM(evnt_id);
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
        /*SPR 17777 +-*/
	}

	if (SON_STATE_UNDEFINED != state)
	{
		es_send_feature_state_change_ind(
				(son_intra_rat_global_cell_id_t *)&p_ind->cell_id,
				state, cause);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* es_fsm_cell_state_change_handler */

/* AUTONOMOUS SWITCH OFF START */

/*****************************************************************************
 * Function Name  : es_fsm_load_report_ind_handler
 * Inputs         : p_cell_context    Pointer to ES cell context
 *                : p_msg             Pointer to the message
 * Outputs        : None
 * Returns        : son_buf_retain_status_et
 * Description    : Event Handler for RRM_LOAD_REPORT_IND.
 ****************************************************************************/
static son_buf_retain_status_et
es_fsm_load_report_ind_handler
(
 son_es_index_et evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8                          *p_msg
 )
{
	SON_UT_TRACE_ENTER();

	son_es_load_report_ind_t *p_load_report_ind = (son_es_load_report_ind_t *)p_msg;

	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
			"Load Report received from RRM dl_comp_avl_cap = [%d] ul_comp_avl_cap = [%d]",
			p_load_report_ind->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cap_val,
			p_load_report_ind->serv_cell_load_info.comp_avl_cap_grp.ul_comp_avl_cap.cap_val);

	if ((SON_ES_AUTONOMOUS_SWITCH_OFF_ENABLED & p_cell_ctxt->autonomous_switch_off_info)
			&& (SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_LOAD_LEVEL & p_cell_ctxt->autonomous_switch_off_info))
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
				"Auotonomous Switch Off- Load Level Criteria ENABLED for the cell.");

		/* Fix 875 Start */
		if (((ES_MAX_LOAD - p_load_report_ind->serv_cell_load_info.comp_avl_cap_grp.dl_comp_avl_cap.cap_val) <=
					p_cell_ctxt->load_level_threshold) &&
				((ES_MAX_LOAD - p_load_report_ind->serv_cell_load_info.comp_avl_cap_grp.ul_comp_avl_cap.cap_val) <=
				 p_cell_ctxt->load_level_threshold))
		{
            /*SPR 17777 +-*/
			SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
					"Load Report received from RRM with load level EQUAL/LESS than the threshold = [%d]"
                    "Unused variable evnt_id=%d p_cspl_hdr=%p",
					p_cell_ctxt->load_level_threshold,evnt_id,p_cspl_hdr);
            /*SPR 17777 +-*/

			/* If monitoring of reports to auto switch off cell is not going on */
			if(SON_PNULL == p_cell_ctxt->es_timer_id)
			{
				p_cell_ctxt->switch_off_cond_hit_status |= SON_ES_SWITCH_OFF_LOW_LOAD_COND_HIT;
				/* If UE check criteria is NOT set, OR otherwise the less UE count condition is already hit */
				if(!(SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_UE_CNT & p_cell_ctxt->autonomous_switch_off_info) ||
						((SON_ES_AUTONOMOUS_SWITCH_OFF_CHECK_UE_CNT & p_cell_ctxt->autonomous_switch_off_info) &&
						 (SON_ES_SWITCH_OFF_LOW_UE_CNT_COND_HIT & p_cell_ctxt->switch_off_cond_hit_status)))
				{
					/* Start the timer to monitor the reports */
					/* SPR-12708 Fix Starts */
					p_cell_ctxt->es_timer_id = son_start_timer(
							g_es_context.autonomous_switch_off_timer_val,
							&p_cell_ctxt->cell_id, sizeof(son_intra_rat_global_cell_id_t), SON_FALSE);
					/* SPR-12708 Fix Ends */
				}
			}
			else
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
						"Monitoring of the reports already going on");
			}
		}
		else
		{
			SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
					"Load Report received from RRM with load level ABOVE the threshold value = [%d]",
					p_cell_ctxt->load_level_threshold);

			if(SON_ES_SWITCH_OFF_LOW_LOAD_COND_HIT & p_cell_ctxt->switch_off_cond_hit_status)
			{
				p_cell_ctxt->switch_off_cond_hit_status ^= SON_ES_SWITCH_OFF_LOW_LOAD_COND_HIT;

				SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
						"Low Load was hit earlier. Status cleared now");

				/* If monitoring of reports to auto switch off cell is going on due to Low load 
				   and low UE count both OR low load only, stop the timer*/              
				if(p_cell_ctxt->es_timer_id)
				{
					son_stop_timer(p_cell_ctxt->es_timer_id);
					p_cell_ctxt->es_timer_id = SON_PNULL;
				}
			}
		}
		/* Fix 875 End */
	}
	else
	{
		SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_DETAILED,
				"Auotonomous Switch Off- Load Level Criteria DISABLED for the cell.");
	}

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_load_report_ind_handler */

/* AUTONOMOUS SWITCH OFF END */

/******************************************************************************
 * Function Name  : es_fsm_nack_enable_req_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_ENABLE_REQ, when received in
 * 					unexpected state.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_nack_enable_req_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_es_enable_res_t  enable_resp = {0};
	son_es_enable_req_t  *p_req = (son_es_enable_req_t *)p_msg;


	SON_UT_TRACE_ENTER();

	enable_resp.enable_res.cell_status_list_size = 1;
	enable_resp.enable_res.result = SON_FAILURE;
	enable_resp.enable_res.cell_status_list[0].error_code = 
		SON_ERR_ALREADY_ENABLED;

	if (ES_CELL_STATE_ENABLING == p_cell_ctxt->current_cell_fsm_state)
	{
		enable_resp.enable_res.cell_status_list[0].error_code = 
			SON_ERR_ENABLE_IN_PROGRESS;
	}

	enable_resp.transaction_id = 
		((son_es_enable_req_t *)p_msg)->transaction_id;
	SON_MEMCPY(&(enable_resp.enable_res.cell_status_list[0].cgi), 
			&p_req->enable_req.srv_cgi, 
			sizeof(son_intra_rat_global_cell_id_t));

	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
			"Unexpected Msg [%s] received for "
			"cell [0x%x] in [%s] state from [%d]. SONES_ENABLE_RES "
			"Msg sent to [%d] with Error Code: [%d] ",
			SON_ES_CELL_FSM_EVENT_NAMES[evnt_id],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			p_cspl_hdr->from, p_cspl_hdr->from,
			enable_resp.enable_res.cell_status_list[0].error_code);

	son_create_send_buffer((son_u8 *)&enable_resp, SON_ES_MODULE_ID,
			SON_MIF_MODULE_ID, SONES_ENABLE_RES,
			sizeof(son_es_enable_res_t));

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_nack_enable_req_handler */

/******************************************************************************
 * Function Name  : es_fsm_nack_disable_req_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_DISABLE_REQ, when received in
 * 					unexpected state.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_nack_disable_req_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t  *p_cell_ctxt,
 STACKAPIHDR        *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_es_disable_res_t  disable_resp = {0};
	son_es_disable_req_t  *p_req = (son_es_disable_req_t *)p_msg;


	disable_resp.enable_res.cell_status_list_size = 1;
	disable_resp.enable_res.cell_status_list[0].error_code = 
		SON_ERR_ALREADY_DISABLED;
	disable_resp.enable_res.result = SON_FAILURE;

	SON_UT_TRACE_ENTER();

	if (ES_CELL_STATE_DISABLING == p_cell_ctxt->current_cell_fsm_state)
	{
		disable_resp.enable_res.cell_status_list[0].error_code = 
			SON_ERR_DISABLE_IN_PROGRESS;
	}
	else if (ES_CELL_STATE_SWITCHED_OFF == p_cell_ctxt->current_cell_fsm_state)
	{
		disable_resp.enable_res.cell_status_list[0].error_code =
			SON_ERR_ES_CELL_SWITCHED_OFF;
	}

	disable_resp.transaction_id = 
		((son_es_disable_req_t *)p_msg)->transaction_id;
	SON_MEMCPY(&(disable_resp.enable_res.cell_status_list[0].cgi),
			&p_req->disable_req.srv_cgi, 
			sizeof(son_intra_rat_global_cell_id_t));

	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
			"Unexpected Msg [%s] received in "
			"[%s] state for cell: [0x%x] from [%d]. SONES_DISABLE_RES Msg sent to [%d] "
			"with Error Code: [%d] ",
			SON_ES_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->from, p_cspl_hdr->from,
			disable_resp.enable_res.cell_status_list[0].error_code);

	son_create_send_buffer((son_u8 *)&disable_resp, SON_ES_MODULE_ID,
			SON_MIF_MODULE_ID, SONES_DISABLE_RES,
			sizeof(son_es_disable_res_t));

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* es_fsm_nack_disable_req_handler */

/*****************************************************************************
 * Function Name  : es_fsm_nack_cell_switch_on_req_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_CELL_SWITCH_ON_REQ. It is invoked
 *                   when received in unexpected ES state.
 ****************************************************************************/
static son_buf_retain_status_et
es_fsm_nack_cell_switch_on_req_handler
(  
 son_es_index_et    evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_es_cell_switch_on_res_t cell_switch_on_resp = {0};
	son_es_cell_switch_on_req_t *p_req = (son_es_cell_switch_on_req_t *)p_msg;
	son_error_et error_code = SON_NO_ERROR;


	SON_UT_TRACE_ENTER();

	switch (p_cell_ctxt->current_cell_fsm_state)
	{
		case ES_CELL_STATE_DISABLED:
			{
				error_code = SON_ERR_ALREADY_DISABLED;
				break;
			}

		case ES_CELL_STATE_ENABLING:
			{
				error_code = SON_ERR_ENABLE_IN_PROGRESS;
				break;
			}

		case ES_CELL_STATE_ENABLED:
			{
				error_code = SON_ERR_ES_CELL_SWITCHED_ON;
				break;
			}

		case ES_CELL_STATE_DISABLING:
			{
				error_code = SON_ERR_DISABLE_IN_PROGRESS;
				break;
			}

		case ES_CELL_STATE_SWITCHING_ON:
			{
				error_code = SON_ERR_ES_CELL_SWITCHED_ON_IN_PROGRESS;
				break;
			}

		default:
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name,
						SON_ERROR, "Default Case hit");
				break;
			}
	} /* switch */

	cell_switch_on_resp.transaction_id = p_req->transaction_id;
	SON_MEMCPY(&(cell_switch_on_resp.generic_res.cell_id),
			&p_req->switch_on_req.srv_cgi, 
			sizeof(son_intra_rat_global_cell_id_t));

	cell_switch_on_resp.generic_res.result = SON_FAILURE;
	cell_switch_on_resp.generic_res.error_code = error_code;

	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
			"Unexpected Msg [%s] received"
			" in [%s] state for cell: [0x%x] from [%d]. SONES_CELL_SWITCH_ON_RES Msg "
			"sent to [%d] with Result: [%d] and Error Code: [%d]",
			SON_ES_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->from, p_cspl_hdr->from,
			cell_switch_on_resp.generic_res.result,
			cell_switch_on_resp.generic_res.error_code);

	son_create_send_buffer((son_u8 *)&cell_switch_on_resp, SON_ES_MODULE_ID,
			(son_module_id_et)p_cspl_hdr->from, SONES_CELL_SWITCH_ON_RES,
			sizeof(son_es_cell_switch_on_res_t));

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* es_fsm_nack_cell_switch_on_req_handler */

/******************************************************************************
 * Function Name  : es_fsm_nack_cell_switch_off_req_handler
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_CELL_SWITCH_OFF_REQ, when received
 * 					in unexpected state.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_nack_cell_switch_off_req_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_es_cell_switch_off_res_t cell_switch_off_resp = {0};
	son_es_cell_switch_off_req_t *p_req = (son_es_cell_switch_off_req_t *)p_msg;


	SON_UT_TRACE_ENTER();

	son_error_et error_code = SON_NO_ERROR;
	switch (p_cell_ctxt->current_cell_fsm_state)
	{
		case ES_CELL_STATE_DISABLED:
			{
				error_code = SON_ERR_ALREADY_DISABLED;
				break;
			}

		case ES_CELL_STATE_ENABLING:
			{
				error_code = SON_ERR_ENABLE_IN_PROGRESS;
				break;
			}

		case ES_CELL_STATE_DISABLING:
			{
				error_code = SON_ERR_DISABLE_IN_PROGRESS;
				break;
			}

		case ES_CELL_STATE_SWITCHING_OFF:
			{
				error_code = SON_ERR_ES_CELL_SWITCH_OFF_IN_PROGRESS;
				break;
			}

		case ES_CELL_STATE_SWITCHED_OFF:
			{
				error_code = SON_ERR_ES_CELL_SWITCHED_OFF;
				break;
			}

		default:
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name,
						SON_ERROR,
						"Invalid current state %u", p_cell_ctxt->current_cell_fsm_state);
				break;
			}
	} /* switch */

	cell_switch_off_resp.transaction_id = p_req->transaction_id;
	SON_MEMCPY(&(cell_switch_off_resp.generic_res.cell_id),
			&p_req->switch_off_req.srv_cgi, 
			sizeof(son_intra_rat_global_cell_id_t));
	cell_switch_off_resp.generic_res.result = SON_FAILURE;
	cell_switch_off_resp.generic_res.error_code = error_code;

	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
			"Unexpected Msg [%s] "
			"received in  [%s] state for cell: [0x%x] from [%d]. "
			"SONES_CELL_SWITCH_OFF_RES Msg sent"
			" to [%d] with Result: [%d] and Error Code: [%d] ",
			SON_ES_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->from, p_cspl_hdr->from,
			cell_switch_off_resp.generic_res.result,
			cell_switch_off_resp.generic_res.error_code);

	son_create_send_buffer((son_u8 *)&cell_switch_off_resp, 
			SON_ES_MODULE_ID,
			(son_module_id_et)p_cspl_hdr->from, SONES_CELL_SWITCH_OFF_RES,
			sizeof(son_es_cell_switch_off_res_t));

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_nack_cell_switch_off_req_handler */

/******************************************************************************
 * Function Name  :es_fsm_nack_modify_ue_cnt_intrval_req_handler 
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_MODIFY_UE_COUNT_INTERVAL_REQ.
 *                  It is invoked when received in unexpected ES state.
 *****************************************************************************/
static son_buf_retain_status_et
es_fsm_nack_modify_ue_cnt_intrval_req_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_es_modify_ue_count_interval_res_t modify_ue_count_interval_resp;
	son_es_modify_ue_count_interval_req_t *p_req = 
		(son_es_modify_ue_count_interval_req_t *)p_msg;
	son_error_et error_code = SON_NO_ERROR;


	SON_UT_TRACE_ENTER();

	switch (p_cell_ctxt->current_cell_fsm_state)
	{
		case ES_CELL_STATE_DISABLED:
			{
				error_code = SON_ERR_ALREADY_DISABLED;
				break;
			}

		case ES_CELL_STATE_ENABLING:
			{
				error_code = SON_ERR_ENABLE_IN_PROGRESS;
				break;
			}

		case ES_CELL_STATE_DISABLING:
			{
				error_code = SON_ERR_DISABLE_IN_PROGRESS;
				break;
			}

		default:
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name,
						SON_ERROR,
						"Invalid current state %u", p_cell_ctxt->current_cell_fsm_state);
				break;
			}
	} /* switch */

	modify_ue_count_interval_resp.transaction_id = p_req->transaction_id;
	SON_MEMCPY(&(modify_ue_count_interval_resp.generic_res.cell_id),
			&p_req->modify_interval_req.srv_cgi, 
			sizeof(son_intra_rat_global_cell_id_t));
	modify_ue_count_interval_resp.generic_res.result = SON_FAILURE;
	modify_ue_count_interval_resp.generic_res.error_code = error_code;
	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
			"Unexpected Msg [%s] "
			"received in  [%s] state for cell: [0x%x] from [%d]."
			" SONES_MODIFY_UE_COUNT_INTERVAL_RES Msg sent"
			" to [%d] with Result: [%d] and Error Code: [%d] ",
			SON_ES_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->from, p_cspl_hdr->from,
			modify_ue_count_interval_resp.generic_res.result,
			modify_ue_count_interval_resp.generic_res.error_code);

	son_create_send_buffer((son_u8 *)&modify_ue_count_interval_resp, 
			SON_ES_MODULE_ID, SON_MIF_MODULE_ID, 
			SONES_MODIFY_UE_COUNT_INTERVAL_RES,
			sizeof(son_es_modify_ue_count_interval_res_t));

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_nack_modify_ue_cnt_intrval_req_handler */

/*****************************************************************************
 * Function Name  : es_fsm_nack_modify_ue_cnt_threshold_req_handler 
 * Inputs         : event_id        event id
 *                  p_cell_context  Pointer to ES cell context
 *                  p_cspl_hdr      Pointer to the CSPL header
 *                : p_msg           Pointer to the message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : Event Handler for SONES_MODIFY_UE_COUNT_THRESHOLD_REQ.
 *                  It is invoked when received in unexpected ES state.
 ****************************************************************************/
static son_buf_retain_status_et
es_fsm_nack_modify_ue_cnt_threshold_req_handler
(
 son_es_index_et    evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8			    *p_msg
 )
{
	son_es_modify_ue_count_threshold_res_t modify_ue_count_threshold_resp;
	son_es_modify_ue_count_threshold_req_t *p_req = 
		(son_es_modify_ue_count_threshold_req_t *)p_msg;
	son_error_et error_code = SON_NO_ERROR;


	SON_UT_TRACE_ENTER();

	switch (p_cell_ctxt->current_cell_fsm_state)
	{
		case ES_CELL_STATE_DISABLED:
			{
				error_code = SON_ERR_ALREADY_DISABLED;
				break;
			}

		case ES_CELL_STATE_ENABLING:
			{
				error_code = SON_ERR_ENABLE_IN_PROGRESS;
				break;
			}

		case ES_CELL_STATE_DISABLING:
			{
				error_code = SON_ERR_DISABLE_IN_PROGRESS;
				break;
			}

		default:
			{
				SON_LOG(es_get_log_mode(), p_son_es_facility_name,
						SON_ERROR,
						"Invalid current state %u", p_cell_ctxt->current_cell_fsm_state);
				break;
			}
	} /* switch */

	modify_ue_count_threshold_resp.transaction_id = p_req->transaction_id;
	SON_MEMCPY(&(modify_ue_count_threshold_resp.generic_res.cell_id),
			&p_req->modify_threshold_req.srv_cgi, 
			sizeof(son_intra_rat_global_cell_id_t));
	modify_ue_count_threshold_resp.generic_res.result = SON_FAILURE;
	modify_ue_count_threshold_resp.generic_res.error_code = error_code;
	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_ERROR,
			"Unexpected Msg [%s] "
			"received in  [%s] state for cell:[0x%x] from [%d] "
			"SONES_MODIFY_UE_COUNT_THRESHOLD_RES Msg sent"
			" to [%d] with Result: [%d] and Error Code: [%d] ",
			SON_ES_CELL_FSM_EVENT_NAMES[evnt_id],
			SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
			p_cspl_hdr->from, p_cspl_hdr->from,
			modify_ue_count_threshold_resp.generic_res.result,
			modify_ue_count_threshold_resp.generic_res.error_code);

	son_create_send_buffer((son_u8 *)&modify_ue_count_threshold_resp,
			SON_ES_MODULE_ID, SON_MIF_MODULE_ID, 
			SONES_MODIFY_UE_COUNT_THRESHOLD_RES,
			sizeof(son_es_modify_ue_count_threshold_res_t));

	SON_UT_TRACE_EXIT();

	return RELEASE_SON_BUFFER;
} /* es_fsm_nack_modify_ue_cnt_threshold_req_handler */

/************************* ES Cell Specific State Machine*********************/
typedef son_buf_retain_status_et  (*es_fsm_evt_handler_t) 
	(
	 son_es_index_et evnt_id,
	 es_cell_context_t *p_cell_ctxt,
	 STACKAPIHDR *p_cspl_hdr,
	 son_u8 *p_msg
	);

	/*****************************************************************************
	 * FSM Entry Point Table
	 *****************************************************************************/
	static es_fsm_evt_handler_t 
	es_fsm_table[ES_CELL_NUM_OF_STATES][ES_CELL_NUM_OF_EVENTS] =
{
	/* State DISABLING */
	{
		es_fsm_enable_req_handler,                             	/* SONES_ENABLE_REQ*/
		es_fsm_nack_disable_req_handler,                      	/* SONES_DISABLE_REQ*/
		es_fsm_nack_cell_switch_on_req_handler,        			/* SONES_CELL_SWITCH_ON_REQ*/
		es_fsm_nack_cell_switch_off_req_handler,      			/* SONES_CELL_SWITCH_OFF_REQ*/
		es_fsm_nack_modify_ue_cnt_intrval_req_handler,     		/* SONES_MODIFY_UE_COUNT_INTRVAL_REQ */
		es_fsm_nack_modify_ue_cnt_threshold_req_handler,	  	/* SONES_MODIFY_UE_COUNT_THRESHOLD_REQ */
		es_fsm_invalid_event_handler,                         	/* RRM_REGISTER_RES*/
		es_fsm_deregister_res_handler,                       	/* RRM_DEREGISTER_RES*/
		es_fsm_invalid_event_handler,       					/* RRM_EVENT_SET_ATTRIBUTE_VALUE_RES */
		es_fsm_invalid_event_handler,                   		/* RRM_CELL_SWITCH_ON_RES*/ 
		es_fsm_invalid_event_handler,               			/* RRM_CELL_SWITCH_OFF_RES  */
		es_fsm_invalid_event_handler,                			/* RRM_ACTIVE_UE_COUNT_REPORT*/
		es_fsm_invalid_event_handler,            				/* RRM_UE_COUNT_THRESHOLD_HIT_IND*/
		es_fsm_invalid_event_handler,                          	/* RRM_S1_MSG_IND */
		es_fsm_cell_state_change_handler,                      	/* RRM_SON_CELL_STATE_CHANGE_IND*/
		/* AUTONOMOUS SWITCH OFF START */
		es_fsm_invalid_event_handler,                           /* RRM_SON_LOAD_REPORT_IND */
		/* AUTONOMOUS SWITCH OFF END */
		es_fsm_invalid_event_handler,                           /* SONES_CELL_ACTIVATION_REQ */

	},

	/* State DISABLED */
	{
		es_fsm_enable_req_handler,                             	/* SONES_ENABLE_REQ*/
		es_fsm_nack_disable_req_handler,                     	/* SONES_DISABLE_REQ*/ 
		es_fsm_nack_cell_switch_on_req_handler,                 /* SONES_CELL_SWITCH_ON_REQ */
		es_fsm_nack_cell_switch_off_req_handler,      			/* SONES_CELL_SWITCH_OFF_REQ*/
		es_fsm_nack_modify_ue_cnt_intrval_req_handler,          /* SONES_MODIFY_UE_COUNT_INTRVAL_REQ */
		es_fsm_nack_modify_ue_cnt_threshold_req_handler,        /* SONES_MODIFY_UE_COUNT_THRESHOLD_REQ */
		es_fsm_invalid_event_handler,                       	/* RRM_REGISTER_RES */
		es_fsm_invalid_event_handler,                       	/* RRM_DEREGISTER_RES*/ 
		es_fsm_invalid_event_handler,       					/* RRM_EVENT_SET_ATTRIBUTE_VALUE_RES */ 
		es_fsm_invalid_event_handler,                   		/* RRM_CELL_SWITCH_ON_RES*/
		es_fsm_invalid_event_handler,                   		/* RRM_CELL_SWITCH_OFF_RES*/
		es_fsm_invalid_event_handler,               			/* RRM_ACTIVE_UE_COUNT_REPORT*/ 
		es_fsm_invalid_event_handler,           				/* RRM_UE_COUNT_THRESHOLD_HIT_IND*/ 
		es_fsm_invalid_event_handler,                           /* RRM_S1_MSG_IND */
		es_fsm_cell_state_change_handler,                      	/* RRM_SON_CELL_STATE_CHANGE_IND*/
		/* AUTONOMOUS SWITCH OFF START */
		es_fsm_invalid_event_handler,                           /* RRM_SON_LOAD_REPORT_IND */
		/* AUTONOMOUS SWITCH OFF END */
		es_fsm_invalid_event_handler,                           /* SONES_CELL_ACTIVATION_REQ */

	},

	/* State ENABLING */
	{
		es_fsm_nack_enable_req_handler,                       	/* SONES_ENABLE_REQ*/ 
		es_fsm_disable_req_handler,                          	/* SONES_DISABLE_REQ*/
		es_fsm_nack_cell_switch_on_req_handler,        			/* SONES_CELL_SWITCH_ON_REQ*/
		es_fsm_nack_cell_switch_off_req_handler,      			/* SONES_CELL_SWITCH_OFF_REQ*/
		es_fsm_nack_modify_ue_cnt_intrval_req_handler,          /* SONES_MODIFY_UE_COUNT_INTERVAL_REQ */
		es_fsm_nack_modify_ue_cnt_threshold_req_handler,	    /* SONES_MODIFY_UE_COUNT_THRESHOLD_REQ */
		es_fsm_register_res_handler,                          	/* RRM_REGISTER_RES*/
		es_fsm_invalid_event_handler,                        	/* RRM_DEREGISTER_RES*/
		es_fsm_invalid_event_handler,        					/* RRM_EVENT_SET_ATTRIBUTE_VALUE_RES */
		es_fsm_invalid_event_handler,                    		/* RRM_CELL_SWITCH_ON_RES*/
		es_fsm_invalid_event_handler,                   		/* RRM_CELL_SWITCH_OFF_RES*/
		es_fsm_invalid_event_handler,             				/* RRM_ACTIVE_UE_COUNT_REPORT   */
		es_fsm_invalid_event_handler,           				/* RRM_UE_COUNT_THRESHOLD_HIT_IND*/ 
		es_fsm_invalid_event_handler,                          	/* RRM_S1_MSG_IND */
		es_fsm_cell_state_change_handler,                      	/* RRM_SON_CELL_STATE_CHANGE_IND*/
		/* AUTONOMOUS SWITCH OFF START */
		es_fsm_invalid_event_handler,                           /* RRM_SON_LOAD_REPORT_IND */
		/* AUTONOMOUS SWITCH OFF END */
		es_fsm_invalid_event_handler,                           /* SONES_CELL_ACTIVATION_REQ */
	},

	/* State ENABLED */
	{
		es_fsm_nack_enable_req_handler,                       	/* SONES_ENABLE_REQ*/
		es_fsm_disable_req_handler,                           	/* SONES_DISABLE_REQ*/
		es_fsm_nack_cell_switch_on_req_handler,        			/* SONES_CELL_SWITCH_ON_REQ*/
		es_fsm_cell_switch_off_req_handler,           			/* SONES_CELL_SWITCH_OFF_REQ*/
		es_fsm_modify_ue_cnt_intrvl_req_handler,   	            /* SONES_MODIFY_UE_COUNT_INTERVAL_REQ */
		es_fsm_modify_ue_cnt_threshold_req_handler,			    /* SONES_MODIFY_UE_COUNT_THRESHOLD_REQ */
		es_fsm_invalid_event_handler,                          	/* RRM_REGISTER_RES*/
		es_fsm_invalid_event_handler,                         	/* RRM_DEREGISTER_RES*/
		es_fsm_set_attribute_value_res_handler,	                /* RRM_EVENT_SET_ATTRIBUTE_VALUE_RES */
		es_fsm_invalid_event_handler,                    		/* RRM_CELL_SWITCH_ON_RES*/
		es_fsm_invalid_event_handler,                  			/* RRM_CELL_SWITCH_OFF_RES*/
		es_fsm_active_ue_cnt_rprt_handler,            			/* RRM_ACTIVE_UE_COUNT_REPORT*/
		es_fsm_ue_cnt_threshold_hit_ind_handler, 				/* RRM_UE_COUNT_THRESHOLD_HIT_IND*/
		es_fsm_s1_msg_ind_handler,                              /* RRM_S1_MSG_IND */
		es_fsm_cell_state_change_handler,                      	/* RRM_SON_CELL_STATE_CHANGE_IND*/
		/* AUTONOMOUS SWITCH OFF START */
		es_fsm_load_report_ind_handler,                         /* RRM_SON_LOAD_REPORT_IND */
		/* AUTONOMOUS SWITCH OFF END */
		es_fsm_invalid_event_handler,                           /* SONES_CELL_ACTIVATION_REQ */

	},

	/* State CELL SWITCHING OFF */
	{
		es_fsm_nack_enable_req_handler,                  		/* SONES_ENABLE_REQ*/
		es_fsm_disable_req_handler,                           	/* SONES_DISABLE_REQ*/
		es_fsm_cell_switch_on_req_handler,              		/* SONES_CELL_SWITCH_ON_REQ*/
		es_fsm_nack_cell_switch_off_req_handler,       			/* SONES_CELL_SWITCH_OFF_REQ*/
		es_fsm_modify_ue_cnt_intrvl_req_handler, 				/* SONES_MODIFY_UE_COUNT_INTERVAL_REQ */
		es_fsm_modify_ue_cnt_threshold_req_handler,            /* SONES_MODIFY_UE_COUNT_THRESHOLD_REQ */
		es_fsm_invalid_event_handler,                			/* RRM_REGISTER_RES*/
		es_fsm_invalid_event_handler,                       	/* RRM_DEREGISTER_RES*/
		es_fsm_set_attribute_value_res_handler,                 /* RRM_EVENT_SET_ATTRIBUTE_VALUE_RES */
		es_fsm_invalid_event_handler,          			        /* RRM_CELL_SWITCH_ON_RES*/
		es_fsm_cell_switch_off_res_handler,              		/* RRM_CELL_SWITCH_OFF_RES*/
		es_fsm_invalid_event_handler,                			/* RRM_ACTIVE_UE_COUNT_REPORT */
		es_fsm_invalid_event_handler,           				/* RRM_UE_COUNT_THRESHOLD_HIT_IND*/ 
		es_fsm_invalid_event_handler,                          	/* RRM_S1_MSG_IND */
		es_fsm_cell_state_change_handler,                      	/* RRM_SON_CELL_STATE_CHANGE_IND*/
		/* AUTONOMOUS SWITCH OFF START */
		es_fsm_invalid_event_handler,                           /* RRM_SON_LOAD_REPORT_IND */
		/* AUTONOMOUS SWITCH OFF END */
		es_fsm_invalid_event_handler,                           /* SONES_CELL_ACTIVATION_REQ */

	},

	/* State CELL SWITCHING ON */
	{
		es_fsm_nack_enable_req_handler,                       	/* SONES_ENABLE_REQ*/
		es_fsm_disable_req_handler,                           	/* SONES_DISABLE_REQ*/
		es_fsm_nack_cell_switch_on_req_handler,        			/* SONES_CELL_SWITCH_ON_REQ*/
		es_fsm_cell_switch_off_req_handler,          			/* SONES_CELL_SWITCH_OFF_REQ */
		es_fsm_modify_ue_cnt_intrvl_req_handler,   	            /* SONES_MODIFY_UE_COUNT_INTERVAL_REQ */
		es_fsm_modify_ue_cnt_threshold_req_handler, 			/* SONES_MODIFY_UE_COUNT_THRESHOLD_REQ */
		es_fsm_invalid_event_handler,                         	/* RRM_REGISTER_RES*/ 
		es_fsm_invalid_event_handler,                        	/* RRM_DEREGISTER_RES*/ 
		es_fsm_set_attribute_value_res_handler,					/* RRM_EVENT_SET_ATTRIBUTE_VALUE_RES*/
		es_fsm_cell_switch_on_res_handler,              		/* RRM_CELL_SWITCH_ON_RES*/
		es_fsm_invalid_event_handler,                   		/* RRM_CELL_SWITCH_OFF_RES*/
		es_fsm_invalid_event_handler,               			/* RRM_ACTIVE_UE_COUNT_REPORT*/ 
		es_fsm_invalid_event_handler,           				/* RRM_UE_COUNT_THRESHOLD_HIT_IND*/ 
		es_fsm_invalid_event_handler,                           /* RRM_S1_MSG_IND */
		es_fsm_cell_state_change_handler,                      	/* RRM_SON_CELL_STATE_CHANGE_IND*/
		/* AUTONOMOUS SWITCH OFF START */
		es_fsm_invalid_event_handler,                           /* RRM_SON_LOAD_REPORT_IND */
		/* AUTONOMOUS SWITCH OFF END */
		es_fsm_invalid_event_handler,                           /* SONES_CELL_ACTIVATION_REQ */
	},

	/* State CELL SWITCHED OFF */
	{
		es_fsm_nack_enable_req_handler,                      	/* SONES_ENABLE_REQ*/  
		es_fsm_nack_disable_req_handler,                        /* SONES_DISABLE_REQ*/
		es_fsm_cell_switch_on_req_handler,                      /* SONES_CELL_SWITCH_ON_REQ*/
		es_fsm_nack_cell_switch_off_req_handler,       			/* SONES_CELL_SWITCH_OFF_REQ*/
		es_fsm_modify_ue_cnt_intrvl_req_handler,                /* SONES_MODIFY_UE_COUNT_INTERVAL_REQ*/
		es_fsm_modify_ue_cnt_threshold_req_handler, 			/* SONES_MODIFY_UE_COUNT_THRESHOLD_REQ */
		es_fsm_invalid_event_handler,                         	/* RRM_REGISTER_RES*/ 
		es_fsm_invalid_event_handler,                       	/* RRM_DEREGISTER_RES*/ 
		es_fsm_set_attribute_value_res_handler,					/* RRM_EVENT_SET_ATTRIBUTE_VALUE_RES*/
		es_fsm_invalid_event_handler,                    		/* RRM_CELL_SWITCH_ON_RES*/ 
		es_fsm_invalid_event_handler,                   		/* RRM_CELL_SWITCH_OFF_RES*/
		es_fsm_invalid_event_handler,               			/* RRM_ACTIVE_UE_COUNT_REPORT*/ 
		es_fsm_invalid_event_handler,           				/* RRM_UE_COUNT_THRESHOLD_HIT_IND*/ 
		es_fsm_invalid_event_handler,                          	/* RRM_S1_MSG_IND */
		es_fsm_cell_state_change_handler,                      	/* RRM_SON_CELL_STATE_CHANGE_IND*/
		/* AUTONOMOUS SWITCH OFF START */
		es_fsm_invalid_event_handler,                           /* RRM_SON_LOAD_REPORT_IND */
		/* AUTONOMOUS SWITCH OFF END */
		es_fsm_cell_activation_req_handler,                      /* SONES_CELL_ACTIVATION_REQ */
	}   
};

/*****************************************************************************
 * Function Name  : es_print_cell_context 
 * Inputs         : p_cell_ctxt         Pointer to cell context
 * Outputs        : None
 * Returns        : None 
 * Description    : This function prints the ES cell context
 ****************************************************************************/
son_void_t
es_print_cell_context(
		es_cell_context_t   *p_cell_ctxt
		)
{
	SON_UT_TRACE_ENTER();

	/* Fix 875 Start */
	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
			"ES Cell Context: Previous_cell_fsm_state = [%s],"
			" Current_cell_fsm_state = [%s], Current_cell_state = [%d]",
			SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->previous_cell_fsm_state],
			SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
			p_cell_ctxt->current_cell_state);

	SON_LOG(es_get_log_mode(), p_son_es_facility_name, SON_BRIEF,
			"autonomous_switch_off_info = [%d] load_level_threshold = [%d]"
			" ue_report_interval = [%d] ue_count_threshold = [%d] "
			"es_timer_id = %lu switch_off_cond_hit_status = [%d]",
			p_cell_ctxt->autonomous_switch_off_info,
			p_cell_ctxt->load_level_threshold,
			p_cell_ctxt->ue_report_interval, p_cell_ctxt->ue_count_threshold,
			(son_ul32)p_cell_ctxt->es_timer_id, p_cell_ctxt->switch_off_cond_hit_status);
	/* Fix 875 End */

	SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : es_cell_process_msg
 * Inputs         : evnt_id             event identity
 *                : p_cell_ctxt         Pointer to cell context
 *                : p_es_msg           Pointer to incoming message
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the entry point to the es Cell FSM.
 *                  This function takes the API message received,event_id,cell context
 *                  and calls the required event handler function.
 ****************************************************************************/
son_buf_retain_status_et
es_cell_process_msg
(
 son_es_index_et     evnt_id,
 es_cell_context_t   *p_cell_ctxt,
 STACKAPIHDR         *p_cspl_hdr,
 son_u8              *p_es_msg
 )
{
	son_buf_retain_status_et ret = RELEASE_SON_BUFFER;
	if (p_cell_ctxt) /* cov_65753_fix */
	{    
		es_cell_fsm_state_et  current_state = p_cell_ctxt->current_cell_fsm_state;

		SON_UT_TRACE_ENTER();

		if (current_state < ES_CELL_NUM_OF_STATES && evnt_id <= IDX_SONES_MAX)
		{
			ret = (*es_fsm_table[current_state][evnt_id])(evnt_id, p_cell_ctxt, p_cspl_hdr, p_es_msg);

			/* SPR_18169 Fix Start */
			if (IDX_RRM_SON_CELL_CHNAGE_IND != evnt_id ||
					(IDX_RRM_SON_CELL_CHNAGE_IND == evnt_id &&
					 RRM_CELL_DELETED !=
					 ((son_cell_state_change_indication_t *)p_es_msg)->
					 cell_state))
			{
				/* SPR_18169 Fix End */

				SON_LOG(anr_get_log_mode(), p_son_es_facility_name, SON_INFO,
						"cell_id: 0x%x OldState: %s Event: %s NewState: %s",
						son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
						SON_ES_CELL_FSM_STATES_NAMES[current_state],
						SON_ES_CELL_FSM_EVENT_NAMES[evnt_id],
						SON_ES_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state]); 
				/* SPR_18169 Fix Start */
			}
			/* SPR_18169 Fix End */
			/* SPR-12876 Fix Starts */
			/* Do not call function to print cell context in case cell delete 
			   indication is received from RRM */ 
			if (IDX_RRM_SON_CELL_CHNAGE_IND != evnt_id || 
					(IDX_RRM_SON_CELL_CHNAGE_IND == evnt_id && 
					 RRM_CELL_DELETED != 
					 ((son_cell_state_change_indication_t *)p_es_msg)->
					 cell_state))
			{
				if(p_cell_ctxt)
				{
					es_print_cell_context(p_cell_ctxt);
				}
			}
			/* SPR-12876 Fix Ends */
		}
		else
		{
            SON_LOG(anr_get_log_mode(), p_son_es_facility_name, SON_ERROR,
                  "Unexpected Event or State  cell_index: 0x%x State: %d Event: %d",
                  son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                  current_state, evnt_id);
		}
	}
	SON_UT_TRACE_EXIT();
	return ret;
} /* es_cell_process_msg */
