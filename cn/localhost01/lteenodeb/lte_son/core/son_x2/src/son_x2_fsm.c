/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_x2_fsm.c $
 *
 *******************************************************************************
 *
 * File Description:This file contains the functions for handling X2 global
 *      level messages and functions for manifest structure of this module.
 *
 *******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR       REFERENCE       REASON
 *   Sep, 2012       Shilpi/Atul  Initial         Initial
 *   July, 2014      Shilpi                       SPR 11340 Fix
 *   July, 2014      Shilpi                       SPR 10331 Fix
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_x2_ctxt_mgr.h>
#include <son_anr_ctxt_mgr.h>
#include <son_x2_fsm.h>
#include <son_x2_enb_fsm.h>
#include <rrc_x2apCommon_composer.h>
#include <rrc_x2apCommon_parser.h>
#include <son_mro_intf.h>
#include <son_mlb_intf.h>
#include <son_es_intf.h>
/* SPR_19279_START */
#include <son_anr_table.h>
/* SPR_19279_END */

/****************************************************************************
 * Global Variables
 ****************************************************************************/
x2_global_context_t g_x2_gb_ctxt;
extern const son_8 *p_son_x2_facility_name;
extern const son_s8 *SON_X2_FSM_EVENT_NAMES[];
extern const son_s8 *SON_X2_FSM_STATES_NAMES[];

/*****************************************************************************
 * Function Name  : sonx2_fsm_init_config_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_INIT_CONFIG_REQ
 *                  message received from SONMgmtIfH in X2_STATE_INIT state.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_init_config_req
(
 son_u8          *p_buf,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	smif_init_config_req_t *p_msg = (smif_init_config_req_t *)p_buf;
	smif_init_config_res_t init_config_res = {0};

	SON_UT_TRACE_ENTER();

                /*SPR 17777 +-*/
    SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILEDALL, 
            "Log for unused variabless "
            "evnt_id[%u] p_cspl_hdr[%p]",
            evnt_id,p_cspl_hdr);
                /*SPR 17777 +-*/
	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_DETAILED,
			"SMIF_INIT_CONFIG_REQ received, "
			"trans Id: [%u] , log enable: [%u] , log level: [%u]",
			p_msg->transaction_id,
			(son_u16)p_msg->log_enable,
			(son_u16)p_msg->log_level);

	if (SON_OAM_LOG_ON == X2_GET_LOG_MODE)
	{
		SET_MODULE_LOG_LEVEL(p_msg->log_level);
	}
	else
	{
		/* Logging is disabled so set log level to NONE  */
		SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
	}

	init_config_res.transaction_id = p_msg->transaction_id;
	init_config_res.result = SON_SUCCESS;
	init_config_res.error_code = SON_NO_ERROR;
	son_create_send_buffer((son_u8 *)&init_config_res,
			SON_X2_MODULE_ID,
			SON_MIF_MODULE_ID, SMIF_INIT_CONFIG_RES,
			sizeof(smif_init_config_res_t));

	/* Set X2 global FSM state to ACTIVE */
	SONX2_FSM_SET_STATE(g_x2_gb_ctxt.x2_global_fsm_state,
			X2_STATE_ACTIVE);

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_init_config_req */

/*****************************************************************************
 * Function Name  : sonx2_fsm_nack_init_config_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_INIT_CONFIG_REQ
 *                  message received from SONMgmtIfH in any state other than
 *                  X2_STATE_INIT state.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_nack_init_config_req
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
                /*SPR 17777 +-*/
    SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILEDALL, 
            "Log for unused variabless "
            "evnt_id[%u] p_cspl_hdr[%p]",
            evnt_id,p_cspl_hdr);
                /*SPR 17777 +-*/
	smif_init_config_res_t smif_init_config_res = {0};
	smif_init_config_req_t *p_smif_init_config_req =
		(smif_init_config_req_t *)p_msg;

	SON_UT_TRACE_ENTER();

	/* Populate the init config response structure */
	smif_init_config_res.transaction_id =
		p_smif_init_config_req->transaction_id;
	smif_init_config_res.result = SON_FAILURE;
	smif_init_config_res.error_code = SON_ERR_UNEXPECTED_MSG;

	/* Sends SMIF_INIT_CONFIG_RES to SONMgmtIfh */
	son_create_send_buffer((son_u8 *)&smif_init_config_res,
			SON_X2_MODULE_ID, SON_MIF_MODULE_ID, SMIF_INIT_CONFIG_RES,
			sizeof(smif_init_config_res_t));

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_nack_init_config_req */

/*****************************************************************************
 * Function Name  : sonx2_fsm_invalid_event_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for the messages received
 *                  in invalid states for which nothing needs to be done.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_invalid_event_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	SON_UT_TRACE_ENTER();

                /*SPR 17777 +-*/
	SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_WARNING,
			"Invalid Event [%s] occured "
			"in [%s] state. Received Message [%u] from [%u] pmsg [%u]",
			SON_X2_FSM_EVENT_NAMES[evnt_id],
			SON_X2_FSM_STATES_NAMES[g_x2_gb_ctxt.x2_global_fsm_state],
			p_cspl_hdr->api, p_cspl_hdr->from,*p_msg);
                /*SPR 17777 +-*/

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : sonx2_fsm_shutdown_state_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for the messages received
 *                  in X2_STATE_SHUTDOWN state.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_shutdown_state_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	SON_UT_TRACE_ENTER();

	SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_BRIEF,
			"Message %p received"
			" in shutdown state from [%u]", SON_X2_FSM_EVENT_NAMES[evnt_id],
			p_cspl_hdr->from);

	/* Clean up the memory dynamically allocated for the received message */
	if (X2_IDX_SONANR_EVENT_ADD_ENB_REQ == evnt_id)
	{
		x2_clear_eNB_rec_list(&((son_x2_add_enb_req_t *)p_msg)->enb_list);
	}
	else if (X2_IDX_SONANR_EVENT_DELETE_ENB_REQ == evnt_id)
	{
		x2_clear_eNB_rec_list(&((son_x2_delete_enb_req_t *)p_msg)
				->enb_list);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_shutdown_state_handler */

/*****************************************************************************
 * Function Name  : sonx2_fsm_set_log_level_cmd_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SET_LOG_LEVEL_CMD message received from
 *                  SONMgmtIfH in state other than X2_STATE_INIT and
 *                  X2_STATE_SHUTDOWN state.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_set_log_level_cmd_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	smif_set_log_level_cmd_t *p_cmd =
		(smif_set_log_level_cmd_t *)p_msg;

	SON_UT_TRACE_ENTER();

	if (SON_OAM_LOG_ON == X2_GET_LOG_MODE)
	{
		SON_LOG(SON_OAM_LOG_ON, p_son_x2_facility_name,
				SON_BRIEF,
				"Log level "
                /*SPR 17777 +-*/
				"changed to [%u] through msg: SMIF_SET_LOG_LEVEL_CMD"
                "evnt_id[%u] p_cspl_hdr[%p]",
				(son_u8)p_cmd->log_level,evnt_id,p_cspl_hdr);
                /*SPR 17777 +-*/
		SET_MODULE_LOG_LEVEL(p_cmd->log_level);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_set_log_level_cmd_handler */

/*****************************************************************************
 * Function Name  : sonx2_send_add_enb_failure_res
 * Inputs         : p_req - Pointer to the add eNB req message data received
 *                  (after the CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  error_code - Error cause
 * Outputs        : None
 * Returns        : None
 * Description    : This is the function to send add eNB failure response to
 *                  the module which requested to add eNB.
 ****************************************************************************/
static son_bool_et
sonx2_send_add_enb_failure_res
(
 son_x2_add_enb_req_t    *p_req,
 STACKAPIHDR             *p_cspl_hdr,
 son_error_et error_code
 )
{
	son_x2_add_enb_res_t res;
	son_enb_add_status_t enb_add_status;
	son_size_t enb_add_status_len = sizeof(son_enb_add_status_t);
	son_nbr_enb_hash_node_t *p_nbr_enb_hash_node = SON_PNULL;
	son_bool_et ret = SON_TRUE;
	SON_LIST_NODE *p_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	res.transaction_id = p_req->transaction_id;
	list_init(&res.enb_list);

	/* Read node from the request list */
	while ((p_node = list_pop_node(&p_req->enb_list)))
	{
		p_nbr_enb_hash_node = YMEMBEROF(
				son_nbr_enb_hash_node_t, anchor, p_node);
		if (SON_PNULL != p_nbr_enb_hash_node)
		{
			/* Insert node into the response list */
			SON_MEMSET(&enb_add_status, 0, enb_add_status_len);
			enb_add_status.p_enb_hash_rec = p_nbr_enb_hash_node->data;
			enb_add_status.result = SON_FAILURE;
			if (SON_PNULL == (x2_insert_add_res_node(&res.enb_list,
							&enb_add_status)))
			{
				ret = SON_FALSE;
			}
		}
	}

	/* Send Add eNB response to ANR */
	son_create_send_buffer((son_u8 *)&res,
			SON_X2_MODULE_ID,
			SON_ANR_MODULE_ID, SONX2_ADD_ENB_RES,
			sizeof(son_x2_add_enb_res_t));

	SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_WARNING,
			"Failure Add eNB response "
                /*SPR 17777 +-*/
			"sent to ANR error_code [%u]",error_code);
	LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/

	SON_UT_TRACE_EXIT();
	return ret;
} /* sonx2_send_add_enb_failure_res */

/*****************************************************************************
 * Function Name  : sonx2_fsm_nack_add_enb_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONANR_ADD_ENB_REQ
 *                  message received from SON ANR in X2_STATE_INIT state.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_nack_add_enb_req
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_x2_add_enb_req_t *p_req = (son_x2_add_enb_req_t *)p_msg;

	SON_UT_TRACE_ENTER();

	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SONANR_ADD_ENB_REQ received:"
                /*SPR 17777 +-*/
			"transaction Id: [%u]\tNumber of eNB to be added: [%u] evnt_id [%u]",
			p_req->transaction_id,
			list_count(&p_req->enb_list),evnt_id);
                /*SPR 17777 +-*/

	sonx2_send_add_enb_failure_res(p_req, p_cspl_hdr,
			SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE);

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_nack_add_enb_req */

/*****************************************************************************
 * Function Name  : sonx2_fsm_shutdown_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_SHUTDOWN_REQ
 *                  message received from SONMgmtIfH in any state other than
 *                  X2_STATE_SHUTDOWN state.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_shutdown_req
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	smif_shutdown_res_t shut_down_res       = {0};

	SON_UT_TRACE_ENTER();

	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SMIF_SHUTDOWN_REQ received:"
                /*SPR 17777 +-*/
			"transaction Id: [%u] evnt_id [%u]",
			*(son_u16 *)(p_msg),evnt_id);
	LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/

	/* Clean up the eNB global context */
	x2_clear_global_context();

	/* Set X2 Global FSM state to SHUTDOWN */
	SONX2_FSM_SET_STATE(g_x2_gb_ctxt.x2_global_fsm_state,
			X2_STATE_SHUTDOWN);

	/* Send SMIF_SHUTDOWN_RES to SONMgmtIfH*/
	shut_down_res.transaction_id = *(son_u16 *)(p_msg);
	shut_down_res.result         = SON_SUCCESS;
	shut_down_res.error_code     = SON_NO_ERROR;

	son_create_send_buffer((son_u8 *)&shut_down_res, SON_X2_MODULE_ID,
			SON_MIF_MODULE_ID, SMIF_SHUTDOWN_RES, sizeof(shut_down_res));

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_shutdown_req */

/*****************************************************************************
 * Function Name  : sonx2_fsm_nack_delete_enb_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the nack function for SONANR_DELETE_ENB_REQ
 *                  message received from SON ANR.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_nack_delete_enb_req
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_x2_delete_enb_req_t *p_req = (son_x2_delete_enb_req_t *)p_msg;
	son_x2_delete_enb_res_t res = {0};
	SON_LIST_NODE *p_node = SON_PNULL;
	son_nbr_enb_hash_node_t *p_nbr_enb_hash_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SONANR_DELETE_ENB_REQ received:"
                /*SPR 17777 +-*/
			"transaction Id: [%u] Number of eNB to be deleted: [%u]"
			"evnt_id [%u]",
			p_req->transaction_id,
			list_count(&p_req->enb_list),
			evnt_id);
	LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/

	res.transaction_id = p_req->transaction_id;
	res.result = SON_FAILURE;

	/* Read node from the request list */
	while ((p_node = list_pop_node(&p_req->enb_list)))
	{
		p_nbr_enb_hash_node = YMEMBEROF(
				son_nbr_enb_hash_node_t, anchor, p_node);
		if (SON_PNULL != p_nbr_enb_hash_node && p_nbr_enb_hash_node->data)
		{
			/* Send negative response to ANR for this eNB */
			res.p_enb_data_list = p_nbr_enb_hash_node->data;
			son_create_send_buffer((son_u8 *)&res, SON_X2_MODULE_ID,
					SON_ANR_MODULE_ID, SONX2_DELETE_ENB_RES,
					sizeof(son_x2_delete_enb_res_t));
		}
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_nack_delete_enb_req */

/*****************************************************************************
 * Function Name  : sonx2_populate_rrc_add_enb_req
 * Inputs         : p_req - Pointer to the eNB request message data received
 *                  (after the CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_rrc_req - Pointer to the eNB request message to be sent
 *                  to L3 X2AP
 * Outputs        : None
 * Returns        : SON_SUCCESS/SON_FAILURE
 * Description    : This is the function which is used to find if any eNB
 *                  received in add eNB request needs X2 set up and if
 *                  required add the relavent data in the message pointer
 *                  to be sent to L3 X2AP.
 ****************************************************************************/
son_return_et
sonx2_populate_rrc_add_enb_req
(
 son_x2_add_enb_req_t    *p_req,
 STACKAPIHDR             *p_cspl_hdr,
 son_rrc_add_enb_req_t   *p_rrc_req
 )
{
	son_u8 ip_addr_index = 0;
	SON_LIST_NODE *p_node = SON_PNULL;
	son_nbr_enb_hash_node_t *p_nbr_enb_hash_node = SON_PNULL;
	son_return_et ret = SON_FAILURE;
	/* SPR 7528 IPV6 Support Start */
	x2_enb_comm_info_t *p_enb_comm_info = SON_PNULL;
	/* SPR 7528 IPV6 Support End */

	SON_UT_TRACE_ENTER();

	if (SON_PNULL == p_req || SON_PNULL == p_cspl_hdr ||
			SON_PNULL == p_rrc_req)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_WARNING,
				"Failed to create add eNB request towards L3 X2AP, p_req or "
				"p_cspl_hdr or p_rrc_req is NULL");
		SON_UT_TRACE_EXIT();
		return SON_FAILURE;
	}

	/* Read node from the request list */
	while ((p_node = list_pop_node(&p_req->enb_list)))
	{
		p_nbr_enb_hash_node = YMEMBEROF(
				son_nbr_enb_hash_node_t, anchor, p_node);
		if (p_nbr_enb_hash_node && p_nbr_enb_hash_node->data &&
				(X2_ENB_STATE_DISCONNECTED ==
				 p_nbr_enb_hash_node->data->enb_curr_fsm_state))
		{
			/* Call eNB FSM */
			x2_enb_process_msg(p_cspl_hdr,
					p_nbr_enb_hash_node->data,
					(son_u8 *)p_req);

			/* SPR 7528 IPV6 Support Start */
			p_enb_comm_info = &p_rrc_req->add_enb_req.enb_info[
				p_rrc_req->add_enb_req.num_peer_enb];

			/* Add IP address details to L3 X2AP add eNB request */
			for (ip_addr_index = 0;
					ip_addr_index <
					p_nbr_enb_hash_node->data->enb_tnl_address_list_size;
					ip_addr_index++)
			{
				son_enb_tnl_address_t *p_addr =
					&(p_nbr_enb_hash_node->data->
							enb_tnl_address_list[ip_addr_index]);

				if (IPV4_ADD_PRESENT & p_addr->bitmask)
				{
					/*Klockwork Fix Starts*/
					if(p_enb_comm_info->num_ip_addr < 3)
					{
						x2_convert_son_ip_addr_to_l3_ip_addr(
								p_addr->ipv4_add,
								p_enb_comm_info->ip_addr[p_enb_comm_info->num_ip_addr].ip_addr);
                        /* spr 20633 changes start*/
						p_enb_comm_info->bitmask |= X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT;
						p_enb_comm_info->num_ip_addr++;
						p_enb_comm_info->bitmask |= X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT;
                        /* spr 20633 changes end*/
						ret = SON_SUCCESS;
					}
					/*Klockwork Fix Ends*/
				}
				else if (IPV6_ADD_PRESENT & p_addr->bitmask)
				{
					if(p_enb_comm_info->num_ipv6_addr < 3)
					{
						if(SON_SUCCESS == x2_convert_son_ipv6_addr_to_l3_ipv6_addr(
									p_addr->ipv6_add,
									p_enb_comm_info->ipv6_addr[p_enb_comm_info->num_ipv6_addr].ipv6_addr))
						{
							p_enb_comm_info->bitmask |= X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT;

							p_enb_comm_info->num_ipv6_addr++;
							p_enb_comm_info->bitmask |= X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT;

							/* Below statement is added just to prevent failing of msg composing
							   as IPV4 add is Mandatory at RRC. So filling a random value to lie
							   in b/w valid range i.e. 1-3 */
							/* SPR 10122 Fix Start */
							p_enb_comm_info->num_ip_addr = SON_NULL;
							/* SPR 10122 Fix End */
							ret = SON_SUCCESS;
						}
					}
				}
				else
				{
					/* IP Addr missing error*/
					SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
							SON_WARNING,
							" Invalid IP address information is received in "
							"add eNB request from ANR.(bitmaks not set for "
							"both IPv4 and IPv6)");
				}
			}

			/* If any of the IPv4/IPv6 address is retreived for the eNB */
			if((SON_NULL < p_enb_comm_info->num_ip_addr) || 
					(SON_NULL < p_enb_comm_info->num_ipv6_addr))
			{
				/* Add port detail to L3 X2AP add eNB request */
				p_rrc_req->add_enb_req.enb_info[
					p_rrc_req->add_enb_req.num_peer_enb].port =
					SON_X2_SCTP_DEFAULT_PORT;
				p_rrc_req->add_enb_req.num_peer_enb++;
			}
			/* SPR 7528 IPV6 Support End */
            /* SPR 16119 Fix Start */
            son_mem_free(p_nbr_enb_hash_node);
            /* SPR 16119 Fix End */
		}
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* sonx2_populate_rrc_add_enb_req */

/*****************************************************************************
 * Function Name  : sonx2_fsm_active_state_add_enb_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONANR_ADD_ENB_REQ
 *                  message received from SON ANR.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_active_state_add_enb_req_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_x2_add_enb_req_t *p_req = (son_x2_add_enb_req_t *)p_msg;
	son_rrc_add_enb_req_t *p_rrc_req = SON_PNULL;
	son_size_t rrc_req_len = sizeof(son_rrc_add_enb_req_t);
	son_x2_add_enb_res_t add_res = {0};
	son_buf_retain_status_et buf_ret_status = RELEASE_SON_BUFFER;
	son_error_et error_code = SON_ERR_NO_ENB_REQUIRES_X2_SETUP;
	son_return_et ret = SON_SUCCESS;

	SON_UT_TRACE_ENTER();

	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SONANR_ADD_ENB_REQ"
                /*SPR 17777 +-*/
			"received, trans Id: [%u] , Number of eNB to be added: [%u] "
            "evnt_id [%u]",
			p_req->transaction_id,
			list_count(&p_req->enb_list),evnt_id);
                /*SPR 17777 +-*/

	if (0 == list_count(&p_req->enb_list))
	{
		add_res.transaction_id = p_req->transaction_id;
		list_init(&p_req->enb_list);
		son_create_send_buffer((son_u8 *)&add_res,
				SON_X2_MODULE_ID, SON_ANR_MODULE_ID,
				SONX2_ADD_ENB_RES,
				sizeof(son_x2_add_enb_res_t));
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_WARNING,
				" Add eNB request received without any eNB details");
		SON_UT_TRACE_EXIT();
		return buf_ret_status;
	}

	p_rrc_req = (son_rrc_add_enb_req_t *)son_mem_get(rrc_req_len);
	if (SON_PNULL == p_rrc_req)
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_ERROR,
				"Failed to get memory for "
				"add eNB request message to L3 X2AP");
		/* SEND ERROR MESSAGE to ANR */
		sonx2_send_add_enb_failure_res(p_req, p_cspl_hdr,
				SON_ERR_SYS_MEM_ALLOC_FAILURE);
		SON_UT_TRACE_EXIT();
		return buf_ret_status;
	}

	SON_MEMSET(p_rrc_req, 0, rrc_req_len);
	p_rrc_req->transaction_id = p_req->transaction_id;

	ret = sonx2_populate_rrc_add_enb_req(p_req, p_cspl_hdr, p_rrc_req);

	if (SON_FAILURE == ret)
	{
		error_code = SON_ERR_INVALID_PARAMS;
	}

	if (0 == p_rrc_req->add_enb_req.num_peer_enb)
	{
		/* SEND ERROR MESSAGE to ANR. (No ENB is configured.) */
		sonx2_send_add_enb_failure_res(p_req, p_cspl_hdr, error_code);
	}
	else
	{
		/* Set X2 Global FSM state to CONNECTING as request is sent for X2
		 *  set up to L3 X2AP */
		SONX2_FSM_SET_STATE(g_x2_gb_ctxt.x2_global_fsm_state,
				X2_STATE_CONNECTING);

		/* Invoke PUP tool to send eNB ADD Request to L3 X2AP */
		if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_add_enb_req(
					&p_rrc_req->add_enb_req,
					SON_MODULE_ID,
					SON_RRC_MODULE_ID,
					p_rrc_req->transaction_id,
					SON_NULL))
		{
			SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
					SON_ERROR,
					"Add eNB request with transaction id = [%u] could not be "
					"sent to RRC", p_rrc_req->transaction_id);
		}
	}

	son_mem_free(p_rrc_req);

	SON_UT_TRACE_EXIT();
	return buf_ret_status;
} /* sonx2_fsm_active_state_add_enb_req_handler */

/*****************************************************************************
 * Function Name  : sonx2_fsm_connecting_connected_state_add_enb_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONANR_ADD_ENB_REQ
 *                  message received from SON ANR in X2_STATE_CONNECTED state.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_connecting_connected_state_add_enb_req_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_x2_add_enb_req_t *p_req = (son_x2_add_enb_req_t *)p_msg;
	son_rrc_add_enb_req_t *p_rrc_req = SON_PNULL;
	son_size_t rrc_req_len = sizeof(son_rrc_add_enb_req_t);
	son_x2_add_enb_res_t add_res = {0};
	son_buf_retain_status_et buf_ret_status = RELEASE_SON_BUFFER;
	son_error_et error_code = SON_ERR_NO_ENB_REQUIRES_X2_SETUP;
	son_return_et ret = SON_SUCCESS;

	SON_UT_TRACE_ENTER();

	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SONANR_ADD_ENB_REQ received:"
                /*SPR 17777 +-*/
			"transaction Id: [%u]\tNumber of eNB to be added: [%u] evnt_id [%u]",
			p_req->transaction_id,
			list_count(&p_req->enb_list),evnt_id);
                /*SPR 17777 +-*/

	if (0 == list_count(&p_req->enb_list))
	{
		add_res.transaction_id = p_req->transaction_id;
		list_init(&p_req->enb_list);
		son_create_send_buffer((son_u8 *)&add_res,
				SON_X2_MODULE_ID, SON_ANR_MODULE_ID,
				SONX2_ADD_ENB_RES,
				sizeof(son_x2_add_enb_res_t));
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_WARNING,
				" Add eNB request received without any eNB details");
		SON_UT_TRACE_EXIT();
		return buf_ret_status;
	}

	p_rrc_req = (son_rrc_add_enb_req_t *)son_mem_get(rrc_req_len);
	if (SON_PNULL == p_rrc_req)
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_ERROR,
				"Memory allocation failed for p_rrc_req");
		/* SEND ERROR MESSAGE to ANR */
		sonx2_send_add_enb_failure_res(p_req, p_cspl_hdr,
				SON_ERR_SYS_MEM_ALLOC_FAILURE);
		SON_UT_TRACE_EXIT();
		return buf_ret_status;
	}

	SON_MEMSET(p_rrc_req, 0, rrc_req_len);
	/* SPR 11340 Fix Starts */
	p_rrc_req->transaction_id = p_req->transaction_id;
	/* SPR 11340 Fix Ends */
	ret = sonx2_populate_rrc_add_enb_req(p_req, p_cspl_hdr, p_rrc_req);

	if (SON_FAILURE == ret)
	{
		error_code = SON_ERR_INVALID_PARAMS;
	}

	if (0 == p_rrc_req->add_enb_req.num_peer_enb)
	{
		/* SEND ERROR MESSAGE to ANR. (No eNB is configured.) */
		sonx2_send_add_enb_failure_res(p_req, p_cspl_hdr, error_code);
	}
	else
	{
		/* Invoke PUP tool call to send eNB ADD Request to L3 X2AP */
		if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_add_enb_req(
					&p_rrc_req->add_enb_req,
					SON_MODULE_ID,
					SON_RRC_MODULE_ID,
					p_rrc_req->transaction_id,
					SON_NULL))
		{
			SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
					SON_ERROR,
					"Add eNB request with transaction id = [%u] could not be "
					"sent to RRC", p_rrc_req->transaction_id);
		}
	}

	son_mem_free(p_rrc_req);

	SON_UT_TRACE_EXIT();
	return buf_ret_status;
} /* sonx2_fsm_connecting_connected_state_add_enb_req_handler */

/*****************************************************************************
 * Function Name  : sonx2_fsm_delete_enb_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONANR_DELETE_ENB_REQ
 *                  message received from SON ANR.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_delete_enb_req
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_x2_delete_enb_req_t *p_req = (son_x2_delete_enb_req_t *)p_msg;
	son_buf_retain_status_et buf_ret_status = RELEASE_SON_BUFFER;
	son_x2_delete_enb_res_t delete_res = {0};
	SON_LIST_NODE *p_node = SON_PNULL;
	son_nbr_enb_hash_node_t *p_nbr_enb_hash_node = SON_PNULL;

	SON_UT_TRACE_ENTER();

	if (0 == list_count(&p_req->enb_list))
	{
		delete_res.transaction_id = p_req->transaction_id;
		delete_res.result = SON_FAILURE;
		son_create_send_buffer((son_u8 *)&delete_res,
				SON_X2_MODULE_ID, SON_ANR_MODULE_ID,
				SONX2_DELETE_ENB_RES,
				sizeof(son_x2_delete_enb_res_t));
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_WARNING,
				"Delete eNB request received"
				" without any eNB details");
		SON_UT_TRACE_EXIT();
		return buf_ret_status;
	}

	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SONANR_DELETE_ENB_REQ received:\n"
                /*SPR 17777 +-*/
			"transaction Id: [%u]\tNumber of eNB to be deleted: [%u] evnt_id: [%u]",
			p_req->transaction_id,
			list_count(&p_req->enb_list),evnt_id);
                /*SPR 17777 +-*/

	/* Read node from the request list */
	while ((p_node = list_pop_node(&p_req->enb_list)))
	{
		p_nbr_enb_hash_node = YMEMBEROF(
				son_nbr_enb_hash_node_t, anchor, p_node);
		if (SON_PNULL != p_nbr_enb_hash_node && p_nbr_enb_hash_node->data)
		{
			/* Call eNB FSM */
			x2_enb_process_msg(p_cspl_hdr,
					p_nbr_enb_hash_node->data,
					p_msg);
		}
        /* SPR 16119 Fix Start */
        son_mem_free(p_nbr_enb_hash_node);
        /* SPR 16119 Fix End */
	}

	SON_UT_TRACE_EXIT();
	return buf_ret_status;
} /* sonx2_fsm_delete_enb_req */

/*****************************************************************************
 * Function Name  : x2_add_peer_enb
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for RRC_LINK_UP_IND
 *                  message received from L3 X2AP.
 ****************************************************************************/
static son_nbr_enb_hash_rec_t *
x2_add_peer_enb
(
 son_rrc_link_up_ind_t *p_ind,
 son_global_enb_id_t *p_son_enbid
 )
{
	son_enb_tnl_address_t enb_tnl_addr[SON_MAX_NO_ENB_X2_TLAS];
	son_u8 index = 0;
	son_u8 son_index = 0;
	son_u8 num_ip_addr = 0;
	son_neighbor_enb_info_t enb_info;
	son_nbr_enb_hash_rec_t *p_hash_rec = SON_PNULL;
	son_nbr_enb_ip_addr_hash_rec_t ip_addr_hash_rec;
	son_error_et error_code = SON_NO_ERROR;

	SON_UT_TRACE_ENTER();

	SON_MEMSET(enb_tnl_addr, 0,
			SON_MAX_NO_ENB_X2_TLAS * sizeof(son_enb_tnl_address_t));
	SON_MEMSET(&enb_info, 0, sizeof(son_neighbor_enb_info_t));
	SON_MEMSET(&ip_addr_hash_rec, 0, sizeof(son_nbr_enb_ip_addr_hash_rec_t));
	SON_MEMCPY(&ip_addr_hash_rec.enb_id, p_son_enbid,
			sizeof(son_global_enb_id_t));

	/* SPR 7528 IPV6 Support Start */
	if(((X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT & p_ind->enb_comm_info.bitmask) &&
				!(X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT & p_ind->enb_comm_info.bitmask)) ||
			(!(X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT & p_ind->enb_comm_info.bitmask) &&
			 (X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT & p_ind->enb_comm_info.bitmask)))
	{
		error_code = SON_ERR_INVALID_PARAMS;        
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_WARNING,
				"IPV6 address details missing");

	}
	else if((X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT & p_ind->enb_comm_info.bitmask) &&
			(X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT & p_ind->enb_comm_info.bitmask))
	{
		num_ip_addr = p_ind->enb_comm_info.num_ipv6_addr >= 3 ? 2 :
			p_ind->enb_comm_info.num_ipv6_addr;

		for (index = 0, son_index = 0; index < num_ip_addr; index++)
		{
			if(SON_SUCCESS == x2_convert_l3_ipv6_addr_to_son_ipv6_addr(
						p_ind->enb_comm_info.ipv6_addr[index].ipv6_addr,
						enb_tnl_addr[son_index].ipv6_add))
			{
				enb_tnl_addr[son_index].bitmask |= IPV6_ADD_PRESENT;
				son_index++;
			}
		}
	}
	else
	{
		num_ip_addr = p_ind->enb_comm_info.num_ip_addr >= 3 ? 2 :
			p_ind->enb_comm_info.num_ip_addr;

		for (index = 0, son_index = 0; index < num_ip_addr; index++)
		{
			x2_convert_l3_ip_addr_to_son_ip_addr(
					p_ind->enb_comm_info.ip_addr[index].ip_addr,
					enb_tnl_addr[son_index].ipv4_add);
			enb_tnl_addr[son_index].bitmask |= IPV4_ADD_PRESENT;
			son_index++;
		}

	}    
	/* If at least 1 valid IP address is received and added */
	if(SON_NULL < son_index)
	{
		/* SPR 7528 IPV6 Support End */

		x2_init_enb_neighbor_info(&enb_info, p_son_enbid,
				num_ip_addr, enb_tnl_addr, p_ind->served_cell_info[0].cell_info.tac);

		p_hash_rec = x2_insert_nbr_enb(&g_x2_gb_ctxt.nbr_enb_context_table,
				&enb_info, &error_code);

		if (SON_PNULL == p_hash_rec)
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_WARNING,
					"Failed to "
					"insert the entry for eNB 0x%x "
					"in neighbor eNB hash table.",
					son_convert_char_cell_id_to_int_cell_id(enb_info.enb_id.enb_id));
			/*SPR_17670_START*/
			if(SON_ERR_DUPLICATE_IP_IN_LINK_UP == error_code)
			{
				son_rrc_delete_enb_req_t rrc_del_enb_req = {0};
				rrc_del_enb_req.transaction_id = son_generate_txn_id();

				SON_LOG(X2_GET_LOG_MODE,
						p_son_x2_facility_name, SON_WARNING,
						"Delete eNB req is being trigerred "
						"for eNB recieved in LINK UP with duplicate IP");

				x2_convert_son_enbid_to_l3_enbid(&enb_info.enb_id,
						&rrc_del_enb_req.del_enb_req.gb_enb_id);
				/* Call pup tool to send delete eNB request to L3 X2AP */
				if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_delete_enb_req(
							&rrc_del_enb_req.del_enb_req,
							SON_MODULE_ID,
							SON_RRC_MODULE_ID,
							rrc_del_enb_req.transaction_id,
                            SON_NULL))
				{
					SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
							SON_ERROR,
							"delete eNB request could not be sent to RRC for enb "
							"0x%x", son_convert_char_cell_id_to_int_cell_id(enb_info.enb_id.enb_id));
				}

			}
			/*SPR_17670_END*/
		}
		else
		{
			x2_print_enb_table();
			SONX2_ENB_FSM_SET_STATE(p_hash_rec,
					p_hash_rec->enb_prev_fsm_state,
					p_hash_rec->enb_curr_fsm_state,
					X2_ENB_STATE_CONNECTING);
		}
	}
	SON_UT_TRACE_EXIT();
	return p_hash_rec;
} /* x2_add_peer_enb */

/* SPR-10331 Fix Starts */
/* Lined Deleted */
/* SPR-10331 Fix Ends */

/*****************************************************************************
 * Function Name  : sonx2_fsm_link_up_ind
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for RRC_LINK_UP_IND
 *                  message received from L3 X2AP.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_link_up_ind
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_rrc_link_up_ind_t *p_ind = (son_rrc_link_up_ind_t *)p_msg;
	son_x2_link_up_ind_t anr_link_up_ind = {0};
	son_nbr_enb_hash_tuple_t *p_hash_tpl = SON_PNULL;
	son_nbr_enb_hash_rec_t *p_hash_rec = SON_PNULL;
	son_global_enb_id_t son_enbid;
	son_error_et error = SON_NO_ERROR;
	son_x2_oam_link_up_ind_t *p_oam_link_up_ind = SON_PNULL;
	son_size_t oam_link_up_ind_len = sizeof(son_x2_oam_link_up_ind_t);
	son_size_t cell_info_arr_len = sizeof(x2ap_served_cell_info_arr_t);
	/* SPR 11340 Fix Starts */
	son_bool_et is_x2_conn_triggered_by_oam = SON_TRUE;
	/* SPR 11340 Fix Ends */

	/*SPR_16728_START*/
	son_u8 index = 0;
	son_u8 son_index = 0;
	son_u8 num_ip_addr = 0;
	son_error_et error_code = SON_NO_ERROR;
	son_enb_tnl_address_t enb_tnl_addr[SON_MAX_NO_ENB_X2_TLAS];
	son_nbr_enb_ip_addr_hash_rec_t ip_addr_hash_rec;
	/*SPR_16728_END*/
	SON_UT_TRACE_ENTER();

	SON_MEMSET(&son_enbid, 0, sizeof(son_global_enb_id_t));
	son_enbid.enb_type = SON_ENB_TYPE_UNDEFINED;
	x2_convert_l3_enbid_to_son_enbid(&p_ind->enb2_gb_id,
			&son_enbid);

	p_hash_tpl = x2_find_nbr_enb(&g_x2_gb_ctxt.nbr_enb_context_table,
			&son_enbid, &error);

	/* Neighbor eNB not already added depicts X2 set up initiated by peer
	 *  eNB so add entry in eNB hash table */
	if (SON_PNULL == p_hash_tpl)
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_BRIEF,
				"sonx2_fsm_link_up_ind: Link Up indication is received"
				" for the X2 connection established by peer eNB with "
                /*SPR 17777 +-*/
				"eNB [eNB Id: 0x%x] evnt_id[%u]",
				son_convert_char_cell_id_to_int_cell_id(son_enbid.enb_id),evnt_id);
                /*SPR 17777 +-*/
		/* SPR 11340 Fix Starts */
		is_x2_conn_triggered_by_oam = SON_FALSE;
		/* SPR 11340 Fix Ends */

		p_hash_rec = x2_add_peer_enb(p_ind, &son_enbid);
		if (SON_PNULL == p_hash_rec)
		{
			SON_UT_TRACE_EXIT();
			return RELEASE_SON_BUFFER;
		}
	}
	else
	{
		p_hash_rec = &p_hash_tpl->data;
		/*SPR 16523 fix start*/
		if(X2_ENB_STATE_DISCONNECTED == p_hash_tpl->data.enb_curr_fsm_state)
		{
			is_x2_conn_triggered_by_oam = SON_FALSE;
			/*SPR_16728_START*/

			/*Prepare the local IP Address Array Containing the IP Address provided
			 *in the Link Up Indication */
			SON_MEMSET(enb_tnl_addr, 0,SON_MAX_NO_ENB_X2_TLAS * sizeof(son_enb_tnl_address_t));

			if(((X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT & p_ind->enb_comm_info.bitmask) &&
						!(X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT & p_ind->enb_comm_info.bitmask)) ||
					(!(X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT & p_ind->enb_comm_info.bitmask) &&
					 (X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT & p_ind->enb_comm_info.bitmask)))
			{
				SON_LOG(X2_GET_LOG_MODE,
						p_son_x2_facility_name, SON_WARNING,
						"IPV6 address details missing");

			}
			else if((X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT & p_ind->enb_comm_info.bitmask) &&
					(X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT & p_ind->enb_comm_info.bitmask))
			{
				num_ip_addr = p_ind->enb_comm_info.num_ipv6_addr >= 3 ? 2 :
					p_ind->enb_comm_info.num_ipv6_addr;
				for (index = 0, son_index = 0; index < num_ip_addr; index++)
				{
					if(SON_SUCCESS == x2_convert_l3_ipv6_addr_to_son_ipv6_addr(
								p_ind->enb_comm_info.ipv6_addr[index].ipv6_addr,
								enb_tnl_addr[son_index].ipv6_add))
					{
						enb_tnl_addr[son_index].bitmask |= IPV6_ADD_PRESENT;
						son_index++;
					}
				}
			}
			else
			{
				num_ip_addr = p_ind->enb_comm_info.num_ip_addr >= 3 ? 2 :
					p_ind->enb_comm_info.num_ip_addr;
				for (index = 0, son_index = 0; index < num_ip_addr; index++)
				{
					x2_convert_l3_ip_addr_to_son_ip_addr(
							p_ind->enb_comm_info.ip_addr[index].ip_addr,
							enb_tnl_addr[son_index].ipv4_add);
					enb_tnl_addr[son_index].bitmask |= IPV4_ADD_PRESENT;
					son_index++;
				}
			}    

			/*This is kind of safe check as it is not possible that no IP is provided by L3*/	
			if(son_index)
			{
				/*Delete the previous list of IP_Addr-Hash Rec from the IP_ADDR-eNB Hash Table*/
				for(index = 0;	index< p_hash_rec->enb_tnl_address_list_size  ;index++)
				{
					x2_delete_entry_from_nbr_enb_ip_addr_map(&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
							&p_hash_rec->enb_tnl_address_list[index],
							&error_code);
				}

				/*Add the new entries in the IP_ADDR-eNB Hash Table*/
				SON_MEMSET(&ip_addr_hash_rec, 0, sizeof(son_nbr_enb_ip_addr_hash_rec_t));
				ip_addr_hash_rec.enb_id = p_hash_rec->enb_id;	
				for(index = 0; index <son_index  ;index++)
				{
					ip_addr_hash_rec.enb_tnl_address = enb_tnl_addr[index];
					x2_insert_entry_in_nbr_enb_ip_addr_map(&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
							&ip_addr_hash_rec,
							&error_code);
				}

				/*Update the eNB Context available in the eNB Table*/
				SON_MEMCPY(p_hash_rec->enb_tnl_address_list,
						enb_tnl_addr,SON_MAX_NO_ENB_X2_TLAS * sizeof(son_enb_tnl_address_t));
				p_hash_rec->enb_tnl_address_list_size = son_index;
			}
			else
			{
				SON_LOG(X2_GET_LOG_MODE,
						p_son_x2_facility_name, SON_WARNING,
						"IP Address List is not populated");
			}
			/*SPR_16728_END*/	
		}
		/*SPR 16523 fix stop*/
	}

	/* Call eNB FSM */
	x2_enb_process_msg(p_cspl_hdr,
			p_hash_rec,
			p_msg);

	/* Set X2 Global FSM state to CONNECTED */
	SONX2_FSM_SET_STATE(g_x2_gb_ctxt.x2_global_fsm_state,
			X2_STATE_CONNECTED);

	/* Send link up indication to ANR */
	anr_link_up_ind.p_enb_rec = p_hash_rec;
	anr_link_up_ind.num_served_cell = p_ind->num_served_cell;
	anr_link_up_ind.p_served_cell_info =
		(x2ap_served_cell_info_arr_t *)son_mem_get(
				cell_info_arr_len * anr_link_up_ind.num_served_cell);
	/* SPR 11340 Fix Starts */
	anr_link_up_ind.is_x2_conn_triggered_by_oam = is_x2_conn_triggered_by_oam;
	/* SPR 11340 Fix Ends */

	if (SON_PNULL != anr_link_up_ind.p_served_cell_info)
	{
		SON_MEMCPY(anr_link_up_ind.p_served_cell_info,
				p_ind->served_cell_info,
				anr_link_up_ind.num_served_cell *
				cell_info_arr_len);
		/* SPR-10331 Fix Starts */
		if (X2AP_GU_GROUP_ID_LIST_PRESENT & p_ind->bitmask)
		{
			SON_MEMCPY(&anr_link_up_ind.gu_group_id,
					&p_ind->gu_group_id,
					sizeof(x2ap_gu_group_id_list_t));
		}
		/* SPR-10331 Fix Ends */
		son_create_send_buffer((son_u8 *)&anr_link_up_ind, SON_X2_MODULE_ID,
				SON_ANR_MODULE_ID, SONX2_LINK_UP_IND,
				sizeof(son_x2_link_up_ind_t));
        /*coverity 72661 fix start*/
        son_mem_free(anr_link_up_ind.p_served_cell_info);
        /*coverity 72661 fix end*/
	}
	else
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_ERROR,
				"Memory allocation failed for p_served_cell_info. "
				"Link up indication cannot be sent to ANR for eNB 0x%x",
				son_convert_char_cell_id_to_int_cell_id(p_hash_rec->enb_id.enb_id));
	}

	/* Send link up indication to OAM */
	p_oam_link_up_ind = (son_x2_oam_link_up_ind_t *)son_mem_get(
			oam_link_up_ind_len);
	if (SON_PNULL != p_oam_link_up_ind)
	{
		SON_MEMSET(p_oam_link_up_ind, 0, oam_link_up_ind_len);
		SON_MEMCPY(&p_oam_link_up_ind->nbr_enb_id,
				&p_hash_rec->enb_id,
				sizeof(son_global_enb_id_t));

		/* SPR-10331 Fix Starts */
		/* Line deleted */
		/* SPR-10331 Fix Ends */

		son_create_send_buffer((son_u8 *)p_oam_link_up_ind, SON_X2_MODULE_ID,
				SON_MIF_MODULE_ID, SONX2_LINK_UP_IND,
				oam_link_up_ind_len);
		son_mem_free(p_oam_link_up_ind);
	}
	else
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_ERROR,
				"Memory allocation failed for p_oam_link_up_ind. "
				"Link up indication cannot be sent to OAM for eNB 0x%x",
				son_convert_char_cell_id_to_int_cell_id(p_hash_rec->enb_id.enb_id));
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_link_up_ind */

/*****************************************************************************
 * function name  : x2_process_add_enb_response
 * inputs         : p_enb_list - pointer to the enb data received
 *                  p_cspl_hdr - pointer to the the cspl header information
 *                  result - result for the enb data received
 * outputs          p_add_res - pointer to the add enb response to be sent to
 *                  anr
 * returns        : SUCCESS/FAILURE
 * description    : This is the helper function to process the success/failure
 *                  lists received in rrc_add_enb_res message received from
 *                  l3 x2ap.
 ****************************************************************************/
static son_return_et
x2_process_add_enb_response
(
 x2ap_enb_comm_info_list_t   *p_enb_list,
 STACKAPIHDR                 *p_cspl_hdr,
 son_return_et result,
 son_x2_add_enb_res_t        *p_add_res
 )
{
	son_global_enb_id_t *p_son_enbid = SON_PNULL;
	son_u8 index = 0, ip_add_index = 0;
	son_enb_add_status_t enb_add_status;
	son_return_et ret = SON_SUCCESS;
	son_x2_result_et x2_result;
	son_nbr_enb_hash_tuple_t *p_hash_tpl = SON_PNULL;
	son_error_et error = SON_NO_ERROR;
	son_size_t enb_add_status_len = sizeof(son_enb_add_status_t);
	son_u8 num_ip_address = SON_NULL;

	SON_UT_TRACE_ENTER();

	for (; index < p_enb_list->num_peer_enb; index++)
	{
		/* SPR 7528 IPV6 Support Start */
		if(X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT &
				p_enb_list->enb_comm_info[index].bitmask)
		{
			num_ip_address = p_enb_list->enb_comm_info[index].num_ipv6_addr;
		}
		else
		{
			num_ip_address = p_enb_list->enb_comm_info[index].num_ip_addr;
		}

		for (ip_add_index = 0;
				ip_add_index < num_ip_address;
				ip_add_index++)
		{
			son_enb_tnl_address_t tnl_addr = {0};

			if(X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT &
					p_enb_list->enb_comm_info[index].bitmask)
			{
				if(SON_SUCCESS == x2_convert_l3_ipv6_addr_to_son_ipv6_addr(
							p_enb_list->enb_comm_info[index].ipv6_addr[ip_add_index].ipv6_addr,
							tnl_addr.ipv6_add))
				{
					tnl_addr.bitmask |= IPV6_ADD_PRESENT;
				}
			}
			else
			{    
				tnl_addr.bitmask |= IPV4_ADD_PRESENT;
				x2_convert_l3_ip_addr_to_son_ip_addr(
						p_enb_list->enb_comm_info[index].ip_addr[ip_add_index].ip_addr,
						tnl_addr.ipv4_add);
			}
			/* If a valid IP address is retrieved */
			if((IPV4_ADD_PRESENT & tnl_addr.bitmask) || (IPV6_ADD_PRESENT & tnl_addr.bitmask))
			{
				p_son_enbid = x2_get_enb_id_from_nbr_enb_ip_addr_map(
						&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
						&tnl_addr,
						&error);
			}
			/* SPR 7528 IPV6 Support End */

			if (SON_PNULL != p_son_enbid)
			{
				/* Search for the received eNB Id in the neighbor eNB hash
				 *  table */
				p_hash_tpl = x2_find_nbr_enb(
						&g_x2_gb_ctxt.nbr_enb_context_table,
						p_son_enbid, &error);

				if (SON_PNULL == p_hash_tpl)
				{
					SON_LOG(X2_GET_LOG_MODE,
							p_son_x2_facility_name, SON_WARNING,
							"Add eNB response "
							"contains eNB 0x%x not configured at "
							"SON X2 module.",
							son_convert_char_cell_id_to_int_cell_id(p_son_enbid->enb_id));
				}
				else
				{
					/* Call eNB FSM. Here instead of passing the pointer to the
					 * message just pass the SUCCESS or FAILURE received to set
					 * eNB FSM states */
					x2_result = x2_enb_process_msg(p_cspl_hdr,
							&p_hash_tpl->data,
							(son_u8 *)&result);

					if (X2_ENB_MSG_EXPECTED == x2_result)
					{
						/* Insert node into the response list to be sent to
						 *  ANR */
						SON_MEMSET(&enb_add_status, 0, enb_add_status_len);
						enb_add_status.p_enb_hash_rec = &p_hash_tpl->data;
						enb_add_status.result = result;
						if (SON_PNULL == x2_insert_add_res_node(
									&p_add_res->enb_list, &enb_add_status))
						{
							SON_LOG(X2_GET_LOG_MODE,
									p_son_x2_facility_name, SON_WARNING,
									"Failed to "
									"insert a node with eNB 0x%x in "
									"the add eNB response message",
									son_convert_char_cell_id_to_int_cell_id(p_son_enbid->enb_id));
							ret = SON_FAILURE;
						}
					}
				}

				break;
			}
		}
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* x2_process_add_enb_response */

/*****************************************************************************
 * Function Name  : x2_set_global_state
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This is the function which decides X2 new global fsm
 *                  states based on all neighbors X2 connection state.
 ****************************************************************************/
static son_void_t
x2_set_global_state
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
)
{
	son_nbr_enb_hash_tuple_t *p_nbr_enb_hash_tpl = SON_PNULL;
	x2_global_fsm_state_et temp_state = X2_STATE_ACTIVE;

	/* Decide the new global fsm states based on all nbr
	 * eNB states
	 * 1.   If any eNB is still in CONNECTED/RESETTING state
	 * global new state = CONNECTED
	 * 2.   If any eNB is in CONNECTING state global new state
	 * = previous state
	 * 3.   Otherwise eNB add response with failure is received
	 * from last eNB with which X2 connection was being
	 * established so global new state = ACTIVE */
	p_nbr_enb_hash_tpl = x2_get_first_nbr_enb_from_nrt(
			&g_x2_gb_ctxt.nbr_enb_context_table);

	while (SON_PNULL != p_nbr_enb_hash_tpl)
	{
		if (X2_ENB_STATE_CONNECTING ==
				p_nbr_enb_hash_tpl->data.enb_curr_fsm_state)
		{
			temp_state = X2_STATE_CONNECTING;
			p_nbr_enb_hash_tpl = x2_get_next_nbr_enb_from_nrt(
					&g_x2_gb_ctxt.nbr_enb_context_table,
					&p_nbr_enb_hash_tpl->anchor);
			continue;
		}
		/*SPR_16835_START*/
		else if ((X2_ENB_STATE_CONNECTED ==
					p_nbr_enb_hash_tpl->data.enb_curr_fsm_state) ||
				(X2_ENB_STATE_RESETTING ==
				 p_nbr_enb_hash_tpl->data.enb_curr_fsm_state) ||
				(X2_ENB_STATE_DISCONNECTING ==
				 p_nbr_enb_hash_tpl->data.enb_curr_fsm_state))
			/*SPR_16835_END*/
		{
			temp_state = X2_STATE_CONNECTED;
			break;
		}

		p_nbr_enb_hash_tpl = x2_get_next_nbr_enb_from_nrt(
				&g_x2_gb_ctxt.nbr_enb_context_table,
				&p_nbr_enb_hash_tpl->anchor);
	}

	/* Set X2 Global FSM state to the state found above */
	SONX2_FSM_SET_STATE(g_x2_gb_ctxt.x2_global_fsm_state,
			temp_state);
} /* x2_set_global_state */

/*****************************************************************************
 * Function Name  : sonx2_fsm_add_enb_res
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for RRC_ADD_ENB_RES
 *                  message received from L3 X2AP in X2_STATE_CONNECTING.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_add_enb_res
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_rrc_add_enb_res_t *p_res = (son_rrc_add_enb_res_t *)p_msg;
	son_x2_add_enb_res_t add_res = {0};

	SON_UT_TRACE_ENTER();

	add_res.transaction_id = p_res->transaction_id;

	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"X2_ADD_ENB_RES received, "
                /*SPR 17777 +-*/
			"trans Id: [%u] evnt_id [%u]",
			p_res->transaction_id,evnt_id);
                /*SPR 17777 +-*/

	if (X2AP_SUCCESS_ENB_ADD_LIST_PRESENT &
			p_res->add_enb_res.bitmask)
	{
		x2_process_add_enb_response(
				&p_res->add_enb_res.success_enb_list,
				p_cspl_hdr,
				SON_SUCCESS,
				&add_res);
	}
	/* SPR 11340 Fix Starts */
	if (X2AP_FAILURE_ENB_ADD_LIST_PRESENT &
			p_res->add_enb_res.bitmask)
		/* SPR 11340 Fix Ends */
	{
		x2_process_add_enb_response(
				&p_res->add_enb_res.failure_enb_list,
				p_cspl_hdr,
				SON_FAILURE,
				&add_res);
	}

	/* Set X2 global FSM state based on the states of all the eNB FSM states */
	x2_set_global_state();

	if (0 != list_count(&add_res.enb_list))
	{
		son_create_send_buffer((son_u8 *)&add_res,
				SON_X2_MODULE_ID,
				SON_ANR_MODULE_ID, SONX2_ADD_ENB_RES,
				sizeof(son_x2_add_enb_res_t));
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_add_enb_res */

/*****************************************************************************
 * Function Name  : sonx2_fsm_link_down_ind
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for RRC_LINK_DOWN_IND
 *                  message received from L3 X2AP.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_link_down_ind
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_rrc_link_down_ind_t *p_ind = (son_rrc_link_down_ind_t *)p_msg;
	son_x2_link_down_ind_t anr_link_down_ind = {0};
	son_nbr_enb_hash_tuple_t *p_hash_tpl = SON_PNULL;
	son_global_enb_id_t son_enbid;
	son_error_et error = SON_NO_ERROR;
	son_x2_oam_link_down_ind_t oam_link_down_ind;
	son_u16 enb_id_bitmask = X2AP_PEER_ENB_ID_INFO_PRESENT & p_ind->bitmask;

	/* SPR4811 changes start */
	son_u16 common_info_bitmask = X2AP_PEER_ENB_COMM_INFO_PRESENT &
		p_ind->bitmask;

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&son_enbid, 0, sizeof(son_global_enb_id_t));
	son_enbid.enb_type = SON_ENB_TYPE_UNDEFINED;
	/* Validate link down indication message */
	if (!enb_id_bitmask && !common_info_bitmask)
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_WARNING,
				"Link down indication received "
				"without the peer eNB information. evnt_id %u",evnt_id);
                /*SPR 17777 +-*/
		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}

	/*  SPR4811 changes ends */

	SON_MEMSET(&oam_link_down_ind, 0, sizeof(son_x2_oam_link_down_ind_t));
	if (enb_id_bitmask)
	{
		/* Link down indication is received due to any of the following reasons:
		 *  o   SCTP link goes down at run time
		 *  o   X2 link is terminated by the peer eNB */
		x2_convert_l3_enbid_to_son_enbid(&p_ind->enb2_gb_id,
				&son_enbid);

		/* Search for the received eNB Id in the neighbor eNB hash table */
		p_hash_tpl = x2_find_nbr_enb(&g_x2_gb_ctxt.nbr_enb_context_table,
				&son_enbid, &error);
		if (SON_PNULL == p_hash_tpl)
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_WARNING,
					"Link down indication received "
					"from the eNB 0x%x not configured at SON X2 ",
					son_convert_char_cell_id_to_int_cell_id(son_enbid.enb_id));
		}
	}

	/* SPR4811 changes start */
	else if (common_info_bitmask)
	{
		/* Link down indication received due to X2 setup failure
		 *  with peer eNB. In this case peer eNB Id will not be
		 *  present so it is extracted from IP address information
		 *  of the peer */
		son_u8 ip_add_index = 0;
		/* SPR 7528 IPV6 Support Start */
		son_u8 num_ip_address = SON_NULL;

		if(X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT &
				p_ind->enb_comm_info.bitmask)
		{
			num_ip_address = p_ind->enb_comm_info.num_ipv6_addr; 
		}
		else
		{
			num_ip_address = p_ind->enb_comm_info.num_ip_addr;
		}

		for (ip_add_index = 0;
				ip_add_index < num_ip_address;
				ip_add_index++)
		{
			son_enb_tnl_address_t tnl_addr = {0};
			son_global_enb_id_t *p_son_enbid = SON_PNULL;
			if(X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT &
					p_ind->enb_comm_info.bitmask)
			{
				if(SON_SUCCESS == x2_convert_l3_ipv6_addr_to_son_ipv6_addr(
							p_ind->enb_comm_info.ipv6_addr[ip_add_index].ipv6_addr,
							tnl_addr.ipv6_add))
				{
					tnl_addr.bitmask |= IPV6_ADD_PRESENT;
				}
			}
			else
			{
				tnl_addr.bitmask |= IPV4_ADD_PRESENT;
				x2_convert_l3_ip_addr_to_son_ip_addr(
						p_ind->enb_comm_info.ip_addr[ip_add_index].ip_addr,
						tnl_addr.ipv4_add);
			}
			if((IPV4_ADD_PRESENT & tnl_addr.bitmask) || (IPV6_ADD_PRESENT & tnl_addr.bitmask))
			{
				p_son_enbid = x2_get_enb_id_from_nbr_enb_ip_addr_map(
						&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
						&tnl_addr,
						&error);
			}
			/* SPR 7528 IPV6 Support End */

			if (SON_PNULL != p_son_enbid)
			{
				/* Search for the received eNB Id in the neighbor eNB hash
				 *  table */
				p_hash_tpl = x2_find_nbr_enb(
						&g_x2_gb_ctxt.nbr_enb_context_table,
						p_son_enbid, &error);
				if (SON_PNULL == p_hash_tpl)
				{
					SON_LOG(X2_GET_LOG_MODE,
							p_son_x2_facility_name, SON_WARNING,
							"Link down indication"
							" received from eNB 0x%x not "
							"configured at SON X2",
							son_convert_char_cell_id_to_int_cell_id(p_son_enbid->enb_id));
				}

				break;
			}
			else
			{
				SON_LOG(X2_GET_LOG_MODE,
						p_son_x2_facility_name, SON_WARNING,
						"Cound not find eNB record"
						" corresponding to the IP address "
						"information received in link down indication.");
			}
		}
	}

	/* SPR4811 changes end */

	if (SON_PNULL != p_hash_tpl)
	{
		/* Call eNB FSM */
		x2_enb_process_msg(p_cspl_hdr,
				&p_hash_tpl->data,
				p_msg);

		/* Set X2 global FSM state based on the states of all the eNB
		 * FSM states */
		x2_set_global_state();

		/* Send link down indication to ANR */
		anr_link_down_ind.p_enb_rec = &p_hash_tpl->data;

		son_create_send_buffer((son_u8 *)&anr_link_down_ind,
				SON_X2_MODULE_ID,
				SON_ANR_MODULE_ID, SONX2_LINK_DOWN_IND,
				sizeof(son_x2_link_down_ind_t));

		/* Send link down indication to OAM */
		SON_MEMCPY(&oam_link_down_ind.nbr_enb_id,
				&p_hash_tpl->data.enb_id,
				sizeof(son_global_enb_id_t));

		son_create_send_buffer((son_u8 *)&oam_link_down_ind,
				SON_X2_MODULE_ID, SON_MIF_MODULE_ID,
				SONX2_LINK_DOWN_IND,
				sizeof(son_x2_oam_link_down_ind_t));
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_link_down_ind */

#ifdef SON_MRO_FEATURE

/*****************************************************************************
 * Function Name  : sonx2_fsm_ho_report_ind_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for X2AP_HO_REPORT_IND
 *                  message received from L3 X2AP.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_ho_report_ind_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
                /*SPR 17777 +-*/
    SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_DETAILEDALL, 
            "Log for unused variabless "
            "evnt_id[%u] p_cspl_hdr[%p]",
            evnt_id,p_cspl_hdr);
                /*SPR 17777 +-*/
	son_rrc_ho_report_ind_t *p_ind = (son_rrc_ho_report_ind_t *)p_msg;
	son_ho_failure_report_t ho_report_ind;
	son_size_t ho_report_ind_len = sizeof(son_ho_failure_report_t);

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&ho_report_ind, 0, ho_report_ind_len);
	x2_convert_l3_ecgi_to_son_ecgi(&p_ind->src_ecgi,
			(son_intra_rat_global_cell_id_t *)&ho_report_ind.src_cgi);
	x2_convert_l3_ecgi_to_son_ecgi(&p_ind->fail_cell_ecgi,
			(son_intra_rat_global_cell_id_t *)&ho_report_ind.trgt_cgi);
	if (X2AP_HO_REPORT_REEST_CELL_ECGI_PRESENT & p_ind->bitmask)
	{
		ho_report_ind.bitmask |= HO_FAILURE_REPORT_RE_EST_CGI_PRESENT;
		x2_convert_l3_ecgi_to_son_ecgi(&p_ind->re_est_cell_ecgi,
				(son_intra_rat_global_cell_id_t *)&ho_report_ind.
				restablishment_cgi);
	}

	x2_map_ho_report_type_enum((son_ul32 *)&p_ind->ho_report_type, &ho_report_ind.ho_cause);
	/* Send HO Report indication to MRO */
	son_create_send_buffer((son_u8 *)&ho_report_ind,
			SON_X2_MODULE_ID,
			SON_MRO_MODULE_ID, SONX2_HO_REPORT_IND,
			ho_report_ind_len);

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_ho_report_ind_handler */

/*****************************************************************************
 * Function Name  : sonx2_fsm_mob_setting_change_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for X2AP_MOBILITY_SETTING_CHANGE_REQ
 *                  message received from L3 X2AP.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_mob_setting_change_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
                /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(evnt_id);
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/
	son_rrc_mobility_settings_change_req_t *p_req = (son_rrc_mobility_settings_change_req_t *)p_msg;
	mlb_mobility_change_req_t mobility_change_req;
	son_size_t len = sizeof(mlb_mobility_change_req_t);

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&mobility_change_req, 0, len);
	x2_convert_l3_ecgi_to_son_ecgi(&p_req->mob_setting_change_req.mobility_change_info.enb1_cell_id,
			(son_intra_rat_global_cell_id_t *)&mobility_change_req.src_cgi1);
	x2_convert_l3_ecgi_to_son_ecgi(&p_req->mob_setting_change_req.mobility_change_info.enb2_cell_id,
			(son_intra_rat_global_cell_id_t *)&mobility_change_req.src_cgi2);
	mobility_change_req.proposed_mobility_params =
		p_req->mob_setting_change_req.mobility_change_info.enb2_mob_param.handover_trigger_change;
	mobility_change_req.cause.value = p_req->mob_setting_change_req.mobility_change_info.cause.value;
	mobility_change_req.cause.type = p_req->mob_setting_change_req.mobility_change_info.cause.type;
	/* Send MOBILITY_SETTINGS_CHANGE_REQ to MLB */
	son_create_send_buffer((son_u8 *)&mobility_change_req,
			SON_X2_MODULE_ID,
			SON_MLB_MODULE_ID, SONMLB_MOBILITY_CHANGE_REQ,
			len);

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_mob_setting_change_handler */

/*****************************************************************************
 * Function Name  : sonx2_fsm_mob_change_resp_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for X2AP_MOBILITY_CHANGE_RESP
 *                  message received from L3 X2AP.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_mob_change_resp_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_rrc_mobility_change_resp_t *p_resp = (son_rrc_mobility_change_resp_t *)p_msg;
	sonmlb_mobility_change_ack_t ack;
	sonmlb_mobility_change_failure_t *p_failure = SON_PNULL;
	son_size_t len = 0;

	SON_UT_TRACE_ENTER();

	if (1 == p_resp->mob_change_res.mobility_change_resp.response)
	{
		len = sizeof(sonmlb_mobility_change_ack_t);
		SON_MEMSET(&ack, 0, len);

		x2_convert_l3_ecgi_to_son_ecgi(&p_resp->mob_change_res.mobility_change_resp.enb1_cell_id,
				&ack.src_cgi1);
		x2_convert_l3_ecgi_to_son_ecgi(&p_resp->mob_change_res.mobility_change_resp.enb2_cell_id,
				&ack.src_cgi2);
		/* Send MOBILITY_CHANGE_ACK to MLB */
		son_create_send_buffer((son_u8 *)&ack,
				SON_X2_MODULE_ID,
				SON_MLB_MODULE_ID, SONMLB_MOBILITY_CHANGE_ACK,
				len);
	}
	else
	{
		len = sizeof(sonmlb_mobility_change_failure_t);
		p_failure = (sonmlb_mobility_change_failure_t *)son_mem_get(len);
		if (SON_PNULL == p_failure)
		{
			SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
					"Memory allocation failed. Cannot send X2AP_MOBILITY_CHANGE_RESP");
                /*SPR 17777 +-*/
			LTE_GCC_UNUSED_PARAM(evnt_id);
			LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/
		}
		else
		{
			SON_MEMSET(p_failure, 0, len);

			x2_convert_l3_ecgi_to_son_ecgi(&p_resp->mob_change_res.
					mobility_change_resp.enb1_cell_id,
					&p_failure->src_cgi1);
			x2_convert_l3_ecgi_to_son_ecgi(&p_resp->mob_change_res.
					mobility_change_resp.enb2_cell_id,
					&p_failure->src_cgi2);

			if (X2AP_MOBILITY_SETTING_CAUSE_PRESENT & p_resp->mob_change_res.
					mobility_change_resp.bitmask)
			{
				p_failure->bitmask |= X2AP_MOBILITY_SETTING_CAUSE_PRESENT;
				p_failure->cause.value = p_resp->mob_change_res.
					mobility_change_resp.cause.value;
				p_failure->cause.type = p_resp->mob_change_res.
					mobility_change_resp.cause.type;
			}

			if (X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT &
					p_resp->mob_change_res.mobility_change_resp.bitmask)
			{
				p_failure->bitmask |= MOBILITY_PARAM_RANGE_PRESENT;
				p_failure->mob_param_range.lower_limit =
					p_resp->mob_change_res.mobility_change_resp.
					enb2_mob_param_mod_range.ho_trigger_change_lower_limit;
				p_failure->mob_param_range.upper_limit =
					p_resp->mob_change_res.mobility_change_resp.
					enb2_mob_param_mod_range.ho_trigger_change_upper_limit;
			}

			/* Send MOBILITY_CHANGE_FAILURE to MLB */
			son_create_send_buffer((son_u8 *)p_failure,
					SON_X2_MODULE_ID,
					SON_MLB_MODULE_ID, SONMLB_MOBILITY_CHANGE_FAILURE,
					len);
			son_mem_free(p_failure);
		}
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_mob_change_resp_handler */

#endif /* ifdef SON_MRO_FEATURE */

/*****************************************************************************
 * Function Name  : sonx2_fsm_son_mlb_mob_change_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONMLB_MOBILITY_CHANGE_REQ,
 *                  message received from SON MLB.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_sonmlb_mob_change_req_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	mlb_mobility_change_req_t *p_req = (mlb_mobility_change_req_t *)p_msg;
	son_nbr_enb_hash_tuple_t *p_hash_tpl = SON_PNULL;
	son_error_et error = SON_NO_ERROR;

	SON_UT_TRACE_ENTER();

	p_hash_tpl = x2_find_nbr_enb(&g_x2_gb_ctxt.nbr_enb_context_table,
			&p_req->trgt_enbid, &error);

	if (SON_PNULL != p_hash_tpl)
	{
		if (X2_ENB_STATE_CONNECTED ==
				p_hash_tpl->data.enb_curr_fsm_state)
		{
			/* Call eNB FSM */
			x2_enb_process_msg(p_cspl_hdr,
					&p_hash_tpl->data,
					(son_u8 *)p_req);
		}
	}
	else
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_ERROR,
				"mobility change req "
				" received from the MLB [eNB 0x%x] not configured at SON",
                /*SPR 17777 +-*/
				son_convert_char_cell_id_to_int_cell_id(p_req->trgt_enbid.enb_id));
		LTE_GCC_UNUSED_PARAM(evnt_id);
                /*SPR 17777 +-*/
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_sonmlb_mob_change_req_handler */

/*****************************************************************************
 * Function Name  : sonx2_fsm_sonmlb_mob_change_ack_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONMLB_MOBILITY_CHANGE_ACK,
 *                  message received from SON MLB.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_sonmlb_mob_change_ack_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	sonmlb_mobility_change_ack_t *p_ack = (sonmlb_mobility_change_ack_t *)p_msg;
	son_nbr_enb_hash_tuple_t *p_hash_tpl = SON_PNULL;

	SON_UT_TRACE_ENTER();

	son_error_et error = SON_NO_ERROR;

	p_hash_tpl = x2_find_nbr_enb(&g_x2_gb_ctxt.nbr_enb_context_table,
			&p_ack->trgt_enbid, &error);

	if (SON_PNULL != p_hash_tpl)
	{
		if (X2_ENB_STATE_CONNECTED ==
				p_hash_tpl->data.enb_curr_fsm_state)
		{
			/* Call eNB FSM */
			x2_enb_process_msg(p_cspl_hdr,
					&p_hash_tpl->data,
					(son_u8 *)p_ack);
		}
	}
	else
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_ERROR,
				"mobility change ack "
				" received from the MLB [eNB Id: 0x%x] not configured at SON",
                /*SPR 17777 +-*/
				son_convert_char_cell_id_to_int_cell_id(p_ack->trgt_enbid.enb_id));
		LTE_GCC_UNUSED_PARAM(evnt_id);
                /*SPR 17777 +-*/
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_sonmlb_mob_change_ack_handler */

/*****************************************************************************
 * Function Name  : sonx2_fsm_sonmlb_mob_change_fail_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONMLB_MOBILITY_CHANGE_FAILURE,
 *                  message received from SON MLB.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_sonmlb_mob_change_fail_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	sonmlb_mobility_change_failure_t *p_failure = (sonmlb_mobility_change_failure_t *)p_msg;
	son_nbr_enb_hash_tuple_t *p_hash_tpl = SON_PNULL;
	son_error_et error = SON_NO_ERROR;

	SON_UT_TRACE_ENTER();

	p_hash_tpl = x2_find_nbr_enb(&g_x2_gb_ctxt.nbr_enb_context_table,
			&p_failure->trgt_enbid, &error);

	if (SON_PNULL != p_hash_tpl)
	{
		if (X2_ENB_STATE_CONNECTED ==
				p_hash_tpl->data.enb_curr_fsm_state)
		{
			/* Call eNB FSM */
			x2_enb_process_msg(p_cspl_hdr,
					&p_hash_tpl->data,
					(son_u8 *)p_failure);
		}
	}
	else
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_ERROR,
				"mobility change failure "
				" received from the MLB [eNB 0x%x] not configured at SON",
                /*SPR 17777 +-*/
				son_convert_char_cell_id_to_int_cell_id( p_failure->trgt_enbid.enb_id));
		LTE_GCC_UNUSED_PARAM(evnt_id);
                /*SPR 17777 +-*/
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_sonmlb_mob_change_fail_handler */

/*****************************************************************************
 * Function Name  : sonx2_fsm_switch_on_off_ind
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SONES_CELL_SWITCH_ON_OFF_IND message received from SON ES.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_switch_on_off_ind
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_x2_cell_switch_on_off_ind_t *p_ind =
		(son_x2_cell_switch_on_off_ind_t *)p_msg;
	son_nbr_enb_hash_tuple_t *p_nbr_enb_hash_tpl = SON_PNULL;

	SON_UT_TRACE_ENTER();

                /*SPR 17777 +-*/
	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SONES_CELL_SWITCH_ON_OFF_IND received:"
			"Number of served cells for eNB Config Update Request: [%u] evnt_id[%u]",
			p_ind->num_srv_cells_list_size,evnt_id);
                /*SPR 17777 +-*/

	/* Find eNBs in connected state to sent eNB config update by iterating
	 *  through all the eNB entires in neighbor eNB table */
	p_nbr_enb_hash_tpl = x2_get_first_nbr_enb_from_nrt(
			&g_x2_gb_ctxt.nbr_enb_context_table);

	while (SON_PNULL != p_nbr_enb_hash_tpl)
	{
		/*SPR_16835_START*/
		if (X2_ENB_STATE_CONNECTED ==
				p_nbr_enb_hash_tpl->data.enb_curr_fsm_state ||
				X2_ENB_STATE_RESETTING ==
				p_nbr_enb_hash_tpl->data.enb_curr_fsm_state ||
				X2_ENB_STATE_DISCONNECTING == 
				p_nbr_enb_hash_tpl->data.enb_curr_fsm_state
				/*SPR_16835_END*/
		   )			
		{
			/* Call eNB FSM */
			x2_enb_process_msg(p_cspl_hdr,
					&p_nbr_enb_hash_tpl->data,
					(son_u8 *)p_ind);
		}

		p_nbr_enb_hash_tpl = x2_get_next_nbr_enb_from_nrt(
				&g_x2_gb_ctxt.nbr_enb_context_table,
				&p_nbr_enb_hash_tpl->anchor);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_switch_on_off_ind */

/* SPR Fix Start */

/*****************************************************************************
 * Function Name  : sonx2_send_local_enb_update_req
 * Inputs         : p_ind - Pointer to the NRT update indication message
 *                  data received (after the CSPL header)
 * Outputs        :  None
 * Returns        : None
 * Description    : This is the function to send local eNB config update req
 *                  L3 X2AP.
 ****************************************************************************/
static son_void_t
sonx2_send_local_enb_update_req
(
 son_x2_nrt_update_ind_t *p_ind
 )
{
	x2ap_enb_config_update_req_t *p_enb_config_update_req = SON_PNULL;
	son_u16 index = 0;
	son_rrc_enb_config_update_req_t *p_update_req = SON_PNULL;
	son_size_t update_req_size = sizeof(son_rrc_enb_config_update_req_t);

	SON_UT_TRACE_ENTER();

	p_update_req = (son_rrc_enb_config_update_req_t *)
		son_mem_get(update_req_size);

	if (SON_PNULL == p_update_req)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
				"Memory allocation failed for p_update_req."
				" Cannot send local eNB Config "
				"Update request message to X2AP");
	}
	else
	{
		SON_MEMSET(p_update_req, 0, update_req_size);
		p_enb_config_update_req = &p_update_req->enb_config_update_req;

		/* Create eNB config update request message */
		p_update_req->transaction_id = son_generate_txn_id();
		p_enb_config_update_req->num_peer_enb = 0;
		for (; index < p_ind->num_srv_cells_list_size &&
				index < MAX_SERVED_CELLS; index++)
		{
			if (SON_NRT_OP_UPDATE == p_ind->srv_cell_updated_info_list[
					index].nrt_op)
			{
				p_enb_config_update_req->bitmask |=
					X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT;
				x2_populate_cells_to_modify_list_info(
						&p_ind->srv_cell_updated_info_list[index].src_cgi,
						X2_CELL_DEACTIVATION_FALSE,
						&p_enb_config_update_req->mod_served_cell.cell_list[
						p_enb_config_update_req->mod_served_cell.
						num_served_cell]);
				p_enb_config_update_req->mod_served_cell.num_served_cell++;
			}
			else if (SON_NRT_OP_ADD == p_ind->srv_cell_updated_info_list[
					index].nrt_op)
			{
				p_enb_config_update_req->bitmask |=
					X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT;
				x2_populate_srv_cell_info(
						&p_ind->srv_cell_updated_info_list[index].src_cgi,
						&p_enb_config_update_req->add_served_cell.cell_list[
						p_enb_config_update_req->add_served_cell.
						num_served_cell]);
				p_enb_config_update_req->add_served_cell.num_served_cell++;
			}
			else if (SON_NRT_OP_DELETE == p_ind->srv_cell_updated_info_list[
					index].nrt_op)
			{
				p_enb_config_update_req->bitmask |=
					X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT;
				x2_populate_cell_id_for_l3(
						p_ind->srv_cell_updated_info_list[index].src_cgi.
						cell_identity,
						&(p_enb_config_update_req->del_served_cell.
							cell_id_list[
							p_enb_config_update_req->del_served_cell.
							num_served_cell]));
				p_enb_config_update_req->del_served_cell.num_served_cell++;
			}

			x2_populate_temp_nbr_ecgi_enb_id_map(
					&p_ind->srv_cell_updated_info_list[index].src_cgi);
		}

		x2_populate_neigh_ecgi_enb_id_map(
				&p_enb_config_update_req->neigh_ecgi_enb_id_map);
		if (0 < p_enb_config_update_req->neigh_ecgi_enb_id_map.
				neigh_info_count)
		{
			p_enb_config_update_req->bitmask |=
				X2AP_NEIGH_ECGI_ENB_ID_MAPPING_PRESENT;
		}

		/* Call pup tool to send update req to RRC */
		if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_enb_config_update_req(
					&p_update_req->enb_config_update_req,
					SON_MODULE_ID,
					SON_RRC_MODULE_ID,
					p_update_req->transaction_id,
					SON_NULL))
		{
			SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
					SON_ERROR,
					"eNB config update request could not be sent to X2AP");
		}
		/* Free the memory allocated on heap for the update request */
		son_mem_free(p_update_req);
	}

	SON_UT_TRACE_EXIT();
} /* sonx2_send_local_enb_update_req */

/* SPR Fix End */

/*****************************************************************************
 * Function Name  : sonx2_fsm_nrt_update_ind
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONANR_NRT_UPDATE_IND
 *                  message received from SON ANR.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_nrt_update_ind
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_x2_nrt_update_ind_t *p_ind =
		(son_x2_nrt_update_ind_t *)p_msg;
	son_nbr_enb_hash_tuple_t *p_nbr_enb_hash_tpl = SON_PNULL;
	son_bool_et is_local_update_reqd = SON_TRUE;

	SON_UT_TRACE_ENTER();

	/* SPR Fix Start */
	if (SON_TRUE == p_ind->is_local_update)
	{
                /*SPR 17777 +-*/
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_BRIEF,
				"SONANR_NRT_UPDATE_IND received for "
				"local updation of X2AP NRT, "
				"Number of served cells for eNB Config Update Request: [%u] evnt_id[%u]",
				p_ind->num_srv_cells_list_size,evnt_id);
                /*SPR 17777 +-*/
		sonx2_send_local_enb_update_req(p_ind);
	}
	/*SPR 16523 fix start*/
	else if(SON_PNULL != p_ind->p_enb_rec)
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_INFO,
				"SONANR_NRT_UPDATE_IND received for "
				"specific eNB 0x%x. "
				"No. of served cells for eNB Config Update Request: %u",
				son_convert_char_cell_id_to_int_cell_id(p_ind->p_enb_rec->enb_id.enb_id),
				p_ind->num_srv_cells_list_size);

		/*SPR_16835_START*/	
		if ((X2_ENB_STATE_CONNECTED ==
					p_ind->p_enb_rec->enb_curr_fsm_state) ||
				(X2_ENB_STATE_RESETTING ==
				 p_ind->p_enb_rec->enb_curr_fsm_state) ||
				(X2_ENB_STATE_DISCONNECTING ==
				 p_ind->p_enb_rec->enb_curr_fsm_state))
			/*SPR_16835_END*/	
		{
			is_local_update_reqd = SON_FALSE;
			/* Call eNB FSM */
			x2_enb_process_msg(p_cspl_hdr,
					p_ind->p_enb_rec,
					(son_u8 *)p_ind);
		}

		if (SON_TRUE == is_local_update_reqd)
		{
			sonx2_send_local_enb_update_req(p_ind);
		}
	}	
	/*SPR 16523 fix stop*/
	else
	{
		/* SPR Fix End */
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_INFO,
				"SONANR_NRT_UPDATE_IND received:"
				" Number of served cells for eNB Config Update Request:"
				" [%u]", p_ind->num_srv_cells_list_size);

		/* Find eNBs in connected state to sent eNB config update by iterating
		 * through all the eNB entires in neighbor eNB table */
		p_nbr_enb_hash_tpl = x2_get_first_nbr_enb_from_nrt(
				&g_x2_gb_ctxt.nbr_enb_context_table);

		while (SON_PNULL != p_nbr_enb_hash_tpl)
		{
			/* Check if the eNB Config Update request message needs
			 * to be send to this peer eNB */
			/*SPR 16523 fix start*/	
			/* code removed */
			/*SPR 16523 fix stop*/	
			/*SPR_16835_START*/	
			if (X2_ENB_STATE_CONNECTED ==
					p_nbr_enb_hash_tpl->data.enb_curr_fsm_state ||
					X2_ENB_STATE_RESETTING ==
					p_nbr_enb_hash_tpl->data.enb_curr_fsm_state ||
					(X2_ENB_STATE_DISCONNECTING ==
					 p_nbr_enb_hash_tpl->data.enb_curr_fsm_state))
				/*SPR_16835_END*/	
			{
				/* SPR Fix Start */
				is_local_update_reqd = SON_FALSE;

				/* SPR Fix End
				 * Call eNB FSM */
				x2_enb_process_msg(p_cspl_hdr,
						&p_nbr_enb_hash_tpl->data,
						(son_u8 *)p_ind);
			}

			p_nbr_enb_hash_tpl = x2_get_next_nbr_enb_from_nrt(
					&g_x2_gb_ctxt.nbr_enb_context_table,
					&p_nbr_enb_hash_tpl->anchor);
		}

		/* SPR Fix Start */
		if (SON_TRUE == is_local_update_reqd)
		{
			sonx2_send_local_enb_update_req(p_ind);
		}

		/* SPR Fix End */
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_nrt_update_ind */

/* SPR 11689 changes start */

/*****************************************************************************
 * Function Name  : sonx2_fsm_cell_activation_res
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONES_CELL_ACTIVATION_RES
 *                  message received from SON ES in X2_STATE_CONNECTED state.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_cell_activation_res
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_rrc_cell_activation_resp_t cell_act_resp; 
	x2ap_cell_activation_resp_t *p_cell_act_res;
	son_es_cell_activation_resp_t *p_cell_activation_res = (son_es_cell_activation_resp_t *)p_msg;
	son_size_t len = sizeof(son_rrc_cell_activation_resp_t);
	son_u32 i;


	SON_UT_TRACE_ENTER();

	SON_MEMSET(&cell_act_resp, 0, len);
	p_cell_act_res = &cell_act_resp.cell_activation_res.cell_activation_resp;

	cell_act_resp.transaction_id = son_generate_txn_id();
	x2_convert_son_enbid_to_l3_enbid(&p_cell_activation_res->enbid_1,
			&p_cell_act_res->enb1_gb_id);

	x2_convert_son_enbid_to_l3_enbid(&p_cell_activation_res->enbid_2,
			&p_cell_act_res->enb2_gb_id);

	p_cell_act_res->response = p_cell_activation_res->result;

	if(p_cell_activation_res->cell_activation_info.num_served_cell != 0)
	{ 
		p_cell_act_res->bitmask |=
			X2AP_CELL_ACTIVATION_SERVED_CELL_LIST_PRESENT;
		p_cell_act_res->num_served_cell = p_cell_activation_res->cell_activation_info.num_served_cell;

		for (i=0;i<p_cell_activation_res->cell_activation_info.num_served_cell;i++)
		{
			x2_convert_son_ecgi_to_l3_ecgi((son_intra_rat_global_cell_id_t *)&p_cell_activation_res->cell_activation_info.
					served_cell_list[i],&p_cell_act_res->served_cell_list[i]);
		}
	}
	/*If result is failure fill failure cause*/
	if(SON_FAILURE == p_cell_activation_res->result) 
	{
		p_cell_act_res->bitmask |=
			X2AP_CELL_ACTIVATION_CAUSE_PRESENT;
		p_cell_act_res->cause.type = 1;
		p_cell_act_res->cause.value = 1;
	}

	if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_cell_activation_response(
				&cell_act_resp.cell_activation_res,
				SON_MODULE_ID,
				SON_RRC_MODULE_ID,
				cell_act_resp.transaction_id,
				SON_NULL))
	{
                /*SPR 17777 +-*/
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_ERROR,
				" CELL ACTIVATION RESP could not be sent"
                " evnt_id [%u] p_cspl_hdr[%p]",
                evnt_id,p_cspl_hdr);
                /*SPR 17777 +-*/
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
}
/* SPR 11689 changes end */

/*****************************************************************************
 * Function Name  : sonx2_fsm_enb_config_update_res
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for RRC_ENB_CONFIG_UPDATE_RES
 *                  message received from L3 X2AP.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_enb_config_update_res
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_global_enb_id_t son_enbid;
	son_nbr_enb_hash_tuple_t *p_hash_tpl = SON_PNULL;
	son_error_et error = SON_NO_ERROR;

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&son_enbid, 0, sizeof(son_global_enb_id_t));
	son_enbid.enb_type = SON_ENB_TYPE_UNDEFINED;
	if (X2AP_ENB_CONFIG_UPDATE_RES == p_cspl_hdr->api)
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_BRIEF,
				"SONX2_ENB_CONFIG_UPDATE_RES "
				"received: transaction Id: [%u]",
				((son_rrc_enb_config_update_res_t *)p_msg)->transaction_id);

		x2_convert_l3_enbid_to_son_enbid(
				&((son_rrc_enb_config_update_res_t *)p_msg)->
				enb_config_update_res.enb2_gb_id,
				&son_enbid);
	}
	else if (X2AP_ENB_CONFIG_UPDATE_WAIT_IND == p_cspl_hdr->api)
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_BRIEF,
				"SONX2_ENB_CONFIG_UPDATE_WAIT_IND "
				"received");

		x2_convert_l3_enbid_to_son_enbid(
				&((son_x2_enb_config_update_wait_ind *)p_msg)->enb2_gb_id,
				&son_enbid);
	}

	/* Search for the eNB Id in the neighbor eNB table. */
	p_hash_tpl = x2_find_nbr_enb(&g_x2_gb_ctxt.nbr_enb_context_table,
			&son_enbid, &error);
	if (SON_PNULL == p_hash_tpl)
	{
                /*SPR 17777 +-*/
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_WARNING,
				"eNB config update "
				"received from eNB 0x%x not configured at SON",
				son_convert_char_cell_id_to_int_cell_id(son_enbid.enb_id));
		LTE_GCC_UNUSED_PARAM(evnt_id);
                /*SPR 17777 +-*/
	}
	else
	{
		/* Call eNB FSM */
		x2_enb_process_msg(p_cspl_hdr,
				&p_hash_tpl->data,
				(son_u8 *)p_msg);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_enb_config_update_res */

/*****************************************************************************
 * Function Name  : sonx2_fsm_enb_config_update_ind_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for RRC_ENB_CONFIG_UPDATE_IND
 *                  message received from L3 X2AP.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_enb_config_update_ind_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_rrc_enb_config_update_ind_t *p_ind =
		(son_rrc_enb_config_update_ind_t *)p_msg;
	son_global_enb_id_t son_enbid;
    /* SPR_19279_START */
    /* Code Removed */
    /* SPR_19279_END */
	son_nbr_enb_hash_tuple_t *p_hash_tpl = SON_PNULL;
	son_error_et error = SON_NO_ERROR;
    /* SPR_19279_START */
    son_x2_enb_config_update_ind_t update_ind;
    son_cell_plmn_info_t son_plmn_id;
    /*CID 115114 fix +*/
    son_nbr_enb_ip_addr_hash_rec_t ip_addr_hash_rec;
    son_u16 idx = 0;
    /*CID 115114 fix -*/
    /* SPR_19279_END */

	SON_UT_TRACE_ENTER();

	SON_MEMSET(&son_enbid, 0, sizeof(son_global_enb_id_t));
	/*SPR 22001 Start*/
    SON_MEMSET(&son_plmn_id, 0, sizeof(son_cell_plmn_info_t));
	SON_MEMSET(&update_ind, 0, sizeof(son_x2_enb_config_update_ind_t));
	/*SPR 22001 End*/
	son_enbid.enb_type = SON_ENB_TYPE_UNDEFINED;
	x2_convert_l3_enbid_to_son_enbid(&p_ind->enb1_gb_id,
			&son_enbid);

	p_hash_tpl = x2_find_nbr_enb(&g_x2_gb_ctxt.nbr_enb_context_table,
			&son_enbid, &error);
	if (SON_PNULL == p_hash_tpl)
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_WARNING,
				"eNB config update"
				" received from the eNB 0x%x not configured at SON",
				son_convert_char_cell_id_to_int_cell_id(son_enbid.enb_id));
	}
	else if (X2_ENB_STATE_CONNECTED == p_hash_tpl->data.enb_curr_fsm_state)
	{
		son_size_t update_ind_len = sizeof(x2ap_enb_config_update_ind_t);
		/* Send eNB config update indication to SON ANR */
        /* SPR_19279_START */
        /* Code Removed */
        /* SPR_19279_END */
		update_ind.p_update_ind = (x2ap_enb_config_update_ind_t *)
			son_mem_get(update_ind_len);
		if (SON_PNULL != update_ind.p_update_ind)
		{
			SON_MEMCPY(update_ind.p_update_ind, p_ind, update_ind_len);

            /* SPR_19279_START */  
            SON_MEMCPY(&(update_ind.old_eNB_id.enb_id),
                    &(son_enbid.enb_id),
                    SON_ENB_ID_OCTET_SIZE);

            SON_MEMCPY(&(update_ind.new_eNB_id.enb_id),
                    &(son_enbid.enb_id),
                    SON_ENB_ID_OCTET_SIZE);

            SON_MEMCPY(&(update_ind.old_eNB_id.enb_type),
                    &(son_enbid.enb_type),
                    sizeof(son_enb_type_et));

            SON_MEMCPY(&(update_ind.new_eNB_id.enb_type),
                    &(son_enbid.enb_type),
                    sizeof(son_enb_type_et));


            SON_MEMCPY(&update_ind.old_eNB_id.plmn_id,
                    &(son_enbid.plmn_id),
                    sizeof(son_cell_plmn_info_t));
          /* SPR 21119 Fix Start */
            if(X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT & update_ind.p_update_ind->bitmask)
            {
            x2_convert_l3_plmn_id_to_son_plmn_id(
                    &(update_ind.p_update_ind->mod_served_cell.cell_list[0].served_cell_info.
                        cell_info.bcast_plmn_info.plmn_identity[0]),
                    &son_plmn_id);

            SON_MEMCPY(&(update_ind.new_eNB_id.plmn_id),
                    &(son_plmn_id),
                    sizeof(son_cell_plmn_info_t));
            }
            else
            {    SON_MEMCPY(&(update_ind.new_eNB_id.plmn_id),
                    &(update_ind.old_eNB_id.plmn_id),
                    sizeof(son_cell_plmn_info_t));

            }    
          /* SPR 21119 Fix End */
/* CID 115051 fix start */

            if(0 != SON_MEMCMP(&(update_ind.new_eNB_id.plmn_id),
                        &(update_ind.old_eNB_id.plmn_id),
                        sizeof(son_cell_plmn_info_t)))
            {    
                SON_HASH *p_hash = x2_get_enb_nrt();
                son_nbr_enb_hash_rec_t hash_data;
                SON_HASH_NODE  *p_hash_node = SON_PNULL;
	            SON_MEMSET(&hash_data, 0, sizeof(son_nbr_enb_hash_rec_t));
                SON_MEMCPY(&hash_data.enb_id, &son_enbid, sizeof(son_global_enb_id_t));
	            son_nbr_enb_hash_tuple_t *p_tpl = SON_PNULL;

                if (SON_PNULL != (p_hash_node = hash_find(p_hash, (son_void_t *)&hash_data)))
                {    
		             p_tpl = (son_nbr_enb_hash_tuple_t *)
			                 YMEMBEROF(son_nbr_enb_hash_tuple_t, anchor, p_hash_node);
                    /*Delete the Old IP addresses*/
                    for (idx = 0; idx < p_hash_tpl->data.enb_tnl_address_list_size; idx++)
                    {
                        x2_delete_entry_from_nbr_enb_ip_addr_map(&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
                                &p_hash_tpl->data.enb_tnl_address_list[idx],
                                &error);
                    }
                    ANR_FREE_HASH_REC(x2_get_enb_hash_rec_pool(), p_hash, p_tpl,
                            p_hash_node);
                    SON_LOG(X2_GET_LOG_MODE,
                            p_son_x2_facility_name, SON_INFO,
                            "Neighbor eNB 0x%x"
                            " is successfully deleted from the table",
                            son_convert_char_cell_id_to_int_cell_id(update_ind.old_eNB_id.enb_id));
                }
                else
                {
                    SON_LOG(X2_GET_LOG_MODE,
                            p_son_x2_facility_name, SON_WARNING,
                            "Neighbor eNB 0x%x, p_hash_node %p"
                           " not found in the table",
                           son_convert_char_cell_id_to_int_cell_id(update_ind.old_eNB_id.enb_id),
                           p_hash_node);
	                return RELEASE_SON_BUFFER;
                }   

                son_nbr_enb_hash_tuple_t *p_temp_hash_tpl = SON_PNULL;
                SON_MEMCPY(&hash_data.enb_id, &update_ind.new_eNB_id, sizeof(son_global_enb_id_t)); 

                if (SON_PNULL != hash_find(p_hash, (son_void_t *)&hash_data))
                {
                    SON_LOG(X2_GET_LOG_MODE,
                            p_son_x2_facility_name, SON_ERROR,
                            " Neighbor eNB 0x%x Already Present."
                            " Duplicate Entry",
                            son_convert_char_cell_id_to_int_cell_id(hash_data.enb_id.enb_id));

                    SON_UT_TRACE_EXIT();
	                return RELEASE_SON_BUFFER;
                }  
                else
                {
                     SON_LOG(X2_GET_LOG_MODE,p_son_x2_facility_name, SON_DETAILED,"hash_find Successfully");
                }    

                ANR_GET_HASH_REC(x2_get_enb_hash_rec_pool(), p_temp_hash_tpl);
                if (SON_PNULL == p_temp_hash_tpl)
                {
                    SON_LOG(X2_GET_LOG_MODE,
                            p_son_x2_facility_name, SON_ERROR,
                            "Memory allocation failed for p_temp_hash_tpl. Neighbor eNB 0x%x"
                            " could not be added",
                            son_convert_char_cell_id_to_int_cell_id(hash_data.enb_id.enb_id));

                    SON_UT_TRACE_EXIT();
	                return RELEASE_SON_BUFFER;
                }
                else
                {
                     SON_LOG(X2_GET_LOG_MODE,p_son_x2_facility_name, SON_DETAILED,"ANR_GET_HASH_REC Successfully");
                }     

                SON_MEMSET(&ip_addr_hash_rec, 0, sizeof(son_nbr_enb_ip_addr_hash_rec_t));
                ip_addr_hash_rec.enb_id = update_ind.new_eNB_id;
                /*Add the latest IP addresses. Assuming that insertion ndoes not fails*/
                for (idx = 0; idx < p_hash_tpl->data.enb_tnl_address_list_size; idx++)
                {
                    SON_MEMCPY(&ip_addr_hash_rec.enb_tnl_address,
                          &p_hash_tpl->data.enb_tnl_address_list[idx],
                          sizeof(ip_addr_hash_rec.enb_tnl_address));

                    x2_insert_entry_in_nbr_enb_ip_addr_map(&g_x2_gb_ctxt.nbr_enb_ip_addr_map,
                            (son_void_t *)&ip_addr_hash_rec,
                            &error);
                }
                SON_MEMCPY(&p_temp_hash_tpl->data, &p_hash_tpl->data,
                            sizeof(son_nbr_enb_ip_addr_hash_rec_t));
                hash_insert(p_hash, &p_temp_hash_tpl->anchor);
            }
            /* SPR_19279_END */

			son_create_send_buffer((son_u8 *)&update_ind,
					SON_X2_MODULE_ID,
					SON_ANR_MODULE_ID, SONX2_ENB_CONFIG_UPDATE_IND,
					sizeof(son_x2_enb_config_update_ind_t));
            
            /* SPR 21119 Fix Start */
            /* Please refrain from doing mem_free here, the same is already handled in  
             * anr_x2_enb_config_update_ind_handler
             */
            /* SPR 21119 Fix End */
		}
		else
		{
                /*SPR 17777 +-*/
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_ERROR,
                    "Memory allocation failed for p_update_ind"
                    "Cannot send update to ANR for eNB 0x%x",
                    son_convert_char_cell_id_to_int_cell_id(p_hash_tpl->data.enb_id.enb_id));
            LTE_GCC_UNUSED_PARAM(evnt_id);
            LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/
        }
    }

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
/* CID 115051 fix start */
/* CID 115051 fix end */

} /* sonx2_fsm_enb_config_update_ind_handler */

/*****************************************************************************
 * Function Name  : sonx2_fsm_enb_reset_resp
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for RRC_ENB_CONFIG_UPDATE_IND
 *                  message received from L3 X2AP.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_enb_reset_resp
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_rrc_reset_res_t *p_res = (son_rrc_reset_res_t *)p_msg;
	son_global_enb_id_t son_enbid;
	son_nbr_enb_hash_tuple_t *p_hash_tpl = SON_PNULL;
	son_error_et error = SON_NO_ERROR;

	SON_UT_TRACE_ENTER();

                /*SPR 17777 +-*/
	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SONX2_RESET_RES received:"
			"transaction Id: [%u] evnt_id[%u]",
			p_res->transaction_id,evnt_id);
                /*SPR 17777 +-*/

	/*SPR 11154 : START*/
	SON_MEMSET(&son_enbid,0,sizeof(son_global_enb_id_t));
	/*SPR 11154 : END*/
	x2_convert_l3_enbid_to_son_enbid(&p_res->reset_res.enb2_gb_id,
			&son_enbid);

	p_hash_tpl = x2_find_nbr_enb(&g_x2_gb_ctxt.nbr_enb_context_table,
			&son_enbid, &error);

	if (SON_PNULL == p_hash_tpl)
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_WARNING,
				"eNB reset response "
				" received from the eNB 0x%x not configured at SON",
				son_convert_char_cell_id_to_int_cell_id(son_enbid.enb_id));
	}
	else
	{
		/* Call eNB FSM */
		x2_enb_process_msg(p_cspl_hdr,
				&p_hash_tpl->data,
				p_msg);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_enb_reset_resp */

/* SPR 11689 changes start */
/*****************************************************************************
 * Function Name  : sonx2_fsm_cell_activation_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONES_CELL_ACTIVATION_REQ
 *                  message received from X2 to  SON_ES.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_cell_activation_req
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_rrc_cell_activation_req_t *p_req = (son_rrc_cell_activation_req_t *)p_msg;
	son_es_cell_activation_req_t es_cell_activation_req;
	son_size_t len = sizeof(son_es_cell_activation_req_t);
	son_u32 i;

	SON_UT_TRACE_ENTER();

                /*SPR 17777 +-*/
	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SONES_CELL_ACTIVATION_REQ  received:\n"
			"transaction Id: [%u] evnt_id [%u] p_cspl_hdr [%p]",
			p_req->transaction_id,evnt_id,p_cspl_hdr);
                /*SPR 17777 +-*/

	SON_MEMSET(&es_cell_activation_req, 0, len);
	es_cell_activation_req.cell_activation_info.num_served_cell = 
		p_req->cell_activation_req.cell_activation_info.num_served_cell;
	for(i=0 ; i < p_req->cell_activation_req.cell_activation_info.num_served_cell ;i++)
	{

		x2_convert_l3_ecgi_to_son_ecgi(&p_req->cell_activation_req.cell_activation_info.served_cell_list[i],
				(son_intra_rat_global_cell_id_t *)&es_cell_activation_req.cell_activation_info.served_cell_list[i]);

	}     

	x2_convert_l3_enbid_to_son_enbid(&p_req->cell_activation_req.enb2_gb_id,
			&es_cell_activation_req.enbid_1);
	x2_convert_l3_enbid_to_son_enbid(&p_req->cell_activation_req.enb1_gb_id,
			&es_cell_activation_req.enbid_2);



	son_create_send_buffer((son_u8 *)&es_cell_activation_req,
			SON_X2_MODULE_ID,
			SON_ES_MODULE_ID, SONES_CELL_ACTIVATION_REQ,
			sizeof(son_es_cell_activation_req_t));

	SON_UT_TRACE_EXIT();
	/* Release the buffer memory as it is no longer needed. */
	return RELEASE_SON_BUFFER;
}

/*SPR 11689 changes end */ 
/*****************************************************************************
 * Function Name  : sonx2_fsm_delete_enb_res
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for RRC_DELETE_ENB_RES
 *                  message received from L3 X2AP.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_delete_enb_res
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_rrc_delete_enb_res_t *p_res = (son_rrc_delete_enb_res_t *)p_msg;
	son_global_enb_id_t son_enbid;
	son_error_et error = SON_NO_ERROR;
	son_nbr_enb_hash_tuple_t *p_hash_tpl = SON_PNULL;

	SON_UT_TRACE_ENTER();

	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SONX2_DELETE_ENB_RES received:"
			"transaction Id: [%u]",
			p_res->transaction_id);
	SON_MEMSET(&son_enbid, 0, sizeof(son_global_enb_id_t));
	son_enbid.enb_type = SON_ENB_TYPE_UNDEFINED;
	x2_convert_l3_enbid_to_son_enbid(&p_res->del_enb_res.enb2_gb_id,
			&son_enbid);

	p_hash_tpl = x2_find_nbr_enb(&g_x2_gb_ctxt.nbr_enb_context_table,
			&son_enbid, &error);

	if (SON_PNULL == p_hash_tpl)
	{
                /*SPR 17777 +-*/
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_WARNING,
				"delete enb response contains "
				" received from eNB 0x%x not configured at SON",
				son_convert_char_cell_id_to_int_cell_id(son_enbid.enb_id));
		LTE_GCC_UNUSED_PARAM(evnt_id);
                /*SPR 17777 +-*/
	}
	else
	{
		/* Call eNB FSM */
		x2_enb_process_msg(p_cspl_hdr,
				&p_hash_tpl->data,
				p_msg);

		/* If Success is received from L3 X2AP, Set X2 global FSM state
		 *  based on the states of all the eNB FSM states. Otherwise the global
		 *  state need not change. */
		if (1 == p_res->del_enb_res.response)
		{
			x2_set_global_state();
		}
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
} /* sonx2_fsm_delete_enb_res */

/* SPR-10331 Fix Starts */
/*****************************************************************************
 * Function Name  : sonx2_fsm_nack_gu_group_id_config_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  SONX2_GU_GROUP_ID_CONFIG_REQ message received from OAM
 *                  in state X2_STATE_INIT.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_nack_gu_group_id_config_req
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_x2_gu_group_id_config_req_t *p_req = 
		(son_x2_gu_group_id_config_req_t *)p_msg;
	son_x2_gu_group_id_config_resp_t res = {0};
	SON_UT_TRACE_ENTER();

                /*SPR 17777 +-*/
	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SON_OAM_ANR_GU_GROUP_ID_CONFIG_REQ received:"
			"transaction Id: [%u] Bitmask: [%u]",
			p_req->transaction_id,
			p_req->gu_group_id_config_req.bitmask);
	LTE_GCC_UNUSED_PARAM(evnt_id);
	LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/

	res.transaction_id = p_req->transaction_id;
	res.gu_group_id_config_resp.result = SON_FAILURE;
	res.gu_group_id_config_resp.error_code = 
		SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE;

	son_create_send_buffer((son_u8 *)&res, SON_X2_MODULE_ID,
			SON_MIF_MODULE_ID, SONX2_GU_GROUP_ID_CONFIG_RES,
			sizeof(son_x2_gu_group_id_config_resp_t));

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : sonx2_fsm_gu_group_id_config_req_in_active_connecting_state
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  SONX2_GU_GROUP_ID_CONFIG_REQ message received from OAM
 *                  in states X2_STATE_ACTIVE and X2_STATE_CONNECTING.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_gu_group_id_config_req_in_active_connecting_state
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_x2_gu_group_id_config_req_t *p_req = 
		(son_x2_gu_group_id_config_req_t *)p_msg;
	son_x2_gu_group_id_config_resp_t res = {0};
	x2ap_enb_config_update_req_t *p_enb_config_update_req = SON_PNULL;
	son_rrc_enb_config_update_req_t *p_update_req = SON_PNULL;
	son_size_t update_req_size = sizeof(son_rrc_enb_config_update_req_t);
	son_u16 idx = 0;
	SON_UT_TRACE_ENTER();

	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SON_OAM_ANR_GU_GROUP_ID_CONFIG_REQ received, "
			"trans Id: [%u] , Bitmask: [%u]",
			p_req->transaction_id,
			p_req->gu_group_id_config_req.bitmask);



	/* Send SONX2_GU_GROUP_ID_CONFIG_RES to SMIF */
	res.transaction_id = p_req->transaction_id;
	if (!((SON_OAM_GU_GROUP_ID_TO_ADD_LIST_PRESENT & 
					p_req->gu_group_id_config_req.bitmask) ||
				(SON_OAM_GU_GROUP_ID_TO_DELETE_LIST_PRESENT & 
				 p_req->gu_group_id_config_req.bitmask)))
	{
		res.gu_group_id_config_resp.result = SON_FAILURE;
		res.gu_group_id_config_resp.error_code = 
			SON_ERR_INVALID_PARAMS;

		son_create_send_buffer((son_u8 *)&res, SON_X2_MODULE_ID,
				SON_MIF_MODULE_ID, SONX2_GU_GROUP_ID_CONFIG_RES,
				sizeof(son_x2_gu_group_id_config_resp_t));

		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}
	else
	{
		res.gu_group_id_config_resp.result = SON_SUCCESS;
		res.gu_group_id_config_resp.error_code = 
			SON_NO_ERROR;

		son_create_send_buffer((son_u8 *)&res, SON_X2_MODULE_ID,
				SON_MIF_MODULE_ID, SONX2_GU_GROUP_ID_CONFIG_RES,
				sizeof(son_x2_gu_group_id_config_resp_t));
	}

	p_update_req = (son_rrc_enb_config_update_req_t *)
		son_mem_get(update_req_size);

	if (SON_PNULL == p_update_req)
	{
                /*SPR 17777 +-*/
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
				"Memory allocation failed. Cannot send local eNB Config "
				"Update request message to X2AP");
		LTE_GCC_UNUSED_PARAM(evnt_id);
		LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/
	}
	else
	{
		SON_MEMSET(p_update_req, 0, update_req_size);
		p_enb_config_update_req = &p_update_req->enb_config_update_req;

		/* Create eNB config update request message */
		p_update_req->transaction_id = p_req->transaction_id;
		p_enb_config_update_req->num_peer_enb = 0;

		if (SON_OAM_GU_GROUP_ID_TO_ADD_LIST_PRESENT & 
				p_req->gu_group_id_config_req.bitmask)
		{
			p_enb_config_update_req->bitmask |= 
				X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT;
			p_enb_config_update_req->add_gu_id.num_gu_group_id = 
				p_req->gu_group_id_config_req.gu_group_id_to_add_list.
				num_gu_group_id;
			for (idx = 0; 
					idx < p_enb_config_update_req->add_gu_id.num_gu_group_id;
					idx++)
			{
				x2_convert_son_gu_group_id_to_l3_gu_group_id(
						&p_req->gu_group_id_config_req.
						gu_group_id_to_add_list.gu_id_list[idx],
						&p_enb_config_update_req->add_gu_id.gu_id_list[idx]);
			}

		}
		if (SON_OAM_GU_GROUP_ID_TO_DELETE_LIST_PRESENT & 
				p_req->gu_group_id_config_req.bitmask)
		{
			p_enb_config_update_req->bitmask |= 
				X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT;
			p_enb_config_update_req->delete_gu_id.num_gu_group_id = 
				p_req->gu_group_id_config_req.gu_group_id_to_delete_list.
				num_gu_group_id;
			for (idx = 0; 
					idx < p_enb_config_update_req->delete_gu_id.num_gu_group_id;
					idx++)
			{
				x2_convert_son_gu_group_id_to_l3_gu_group_id(
						&p_req->gu_group_id_config_req.
						gu_group_id_to_delete_list.gu_id_list[idx],
						&p_enb_config_update_req->delete_gu_id.gu_id_list[idx]);
			}
		}

		/* Call pup tool to send update req to RRC */
		if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_enb_config_update_req(
					&p_update_req->enb_config_update_req,
					SON_MODULE_ID,
					SON_RRC_MODULE_ID,
					p_update_req->transaction_id,
					SON_NULL))
		{
			SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
					SON_ERROR,
					"sonx2_send_local_enb_update_req: "
					"Local eNB config update request could not be sent to X2AP");
		}

		/* Free the memory allocated on heap for the update request */
		son_mem_free(p_update_req);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : sonx2_fsm_gu_group_id_config_req_in_connected_state 
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  SONX2_GU_GROUP_ID_CONFIG_REQ message received from OAM
 *                  in state X2_STATE_CONNECTED.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_gu_group_id_config_req_in_connected_state
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
	son_x2_gu_group_id_config_req_t *p_req = 
		(son_x2_gu_group_id_config_req_t *)p_msg;
	son_x2_gu_group_id_config_resp_t res = {0};

	son_nbr_enb_hash_tuple_t *p_nbr_enb_hash_tpl = SON_PNULL;

	SON_UT_TRACE_ENTER();

                /*SPR 17777 +-*/
	SON_LOG(X2_GET_LOG_MODE,
			p_son_x2_facility_name, SON_BRIEF,
			"SON_OAM_ANR_GU_GROUP_ID_CONFIG_REQ received:"
			"transaction Id: [%u] Bitmask: [%u]",
			p_req->transaction_id,
			p_req->gu_group_id_config_req.bitmask);
	LTE_GCC_UNUSED_PARAM(evnt_id);
                /*SPR 17777 +-*/

	/* Send SONX2_GU_GROUP_ID_CONFIG_RES to SMIF */
	res.transaction_id = p_req->transaction_id;
	if (!((SON_OAM_GU_GROUP_ID_TO_ADD_LIST_PRESENT & 
					p_req->gu_group_id_config_req.bitmask) ||
				(SON_OAM_GU_GROUP_ID_TO_DELETE_LIST_PRESENT & 
				 p_req->gu_group_id_config_req.bitmask)))
	{
		res.gu_group_id_config_resp.result = SON_FAILURE;
		res.gu_group_id_config_resp.error_code = 
			SON_ERR_INVALID_PARAMS;

		son_create_send_buffer((son_u8 *)&res, SON_X2_MODULE_ID,
				SON_MIF_MODULE_ID, SONX2_GU_GROUP_ID_CONFIG_RES,
				sizeof(son_x2_gu_group_id_config_resp_t));

		SON_UT_TRACE_EXIT();
		return RELEASE_SON_BUFFER;
	}
	else
	{
		res.gu_group_id_config_resp.result = SON_SUCCESS;
		res.gu_group_id_config_resp.error_code = 
			SON_NO_ERROR;

		son_create_send_buffer((son_u8 *)&res, SON_X2_MODULE_ID,
				SON_MIF_MODULE_ID, SONX2_GU_GROUP_ID_CONFIG_RES,
				sizeof(son_x2_gu_group_id_config_resp_t));
	}

	p_nbr_enb_hash_tpl = x2_get_first_nbr_enb_from_nrt(
			&g_x2_gb_ctxt.nbr_enb_context_table);

	while (SON_PNULL != p_nbr_enb_hash_tpl)
	{
		/* Call eNB FSM */
		x2_enb_process_msg(p_cspl_hdr,
				&(p_nbr_enb_hash_tpl->data),
				(son_u8 *)&(p_req->gu_group_id_config_req));
		p_nbr_enb_hash_tpl = x2_get_next_nbr_enb_from_nrt(
				&g_x2_gb_ctxt.nbr_enb_context_table,
				&p_nbr_enb_hash_tpl->anchor);
	}

	SON_UT_TRACE_EXIT();
	return RELEASE_SON_BUFFER;
}
/* SPR-10331 Fix Ends */

/* SPR_19619 start */

/*****************************************************************************
 * Function Name  : sonx2_fsm_peer_cell_activation_req 
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  SONX2_PEER_CELL_ACTIVATION_REQ message received from SON ES
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_fsm_peer_cell_activation_req
(
 son_u8          *p_buf,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
    son_x2_peer_cell_activation_req_t *p_req = (son_x2_peer_cell_activation_req_t *)p_buf;
    son_nbr_enb_hash_tuple_t *p_nbr_enb_hash_tpl = SON_PNULL;


    SON_UT_TRACE_ENTER();

    LTE_GCC_UNUSED_PARAM(evnt_id);
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);

    SON_LOG(X2_GET_LOG_MODE,
            p_son_x2_facility_name, SON_DETAILED,
            "Cell activation request "
            "transaction Id: [%u] Bitmask: [%u]",
            p_req->transaction_id,
            p_req->peer_cell_activation_req.bitmask);
    LTE_GCC_UNUSED_PARAM(evnt_id);

    p_nbr_enb_hash_tpl = x2_get_first_nbr_enb_from_nrt(
            &g_x2_gb_ctxt.nbr_enb_context_table);

    while (SON_PNULL != p_nbr_enb_hash_tpl)

    {    
        if (X2_ENB_STATE_CONNECTED ==
                p_nbr_enb_hash_tpl->data.enb_curr_fsm_state )                 

            x2_enb_process_msg(p_cspl_hdr,
                    &p_nbr_enb_hash_tpl->data,
                    (son_u8 *)p_req);


        p_nbr_enb_hash_tpl = x2_get_next_nbr_enb_from_nrt(
                &g_x2_gb_ctxt.nbr_enb_context_table,
                &p_nbr_enb_hash_tpl->anchor);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER ;
}

/*****************************************************************************
 * Function Name  : sonx2_peer_cell_activation_resp_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for X2AP_PEER_CELL_ACTIVATION_RESP
 *                  message received from L3 X2AP.
 ****************************************************************************/
static son_buf_retain_status_et
sonx2_peer_cell_activation_resp_handler
(
 son_u8          *p_msg,
 STACKAPIHDR     *p_cspl_hdr,
 son_x2_index_et evnt_id
 )
{
    son_rrc_peer_cell_activation_resp_t *p_resp = (son_rrc_peer_cell_activation_resp_t *)p_msg;
    smif_peer_cell_activation_res_t es_cell_activation_resp;
    son_global_enb_id_t son_enbid;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&son_enbid, 0, sizeof(son_global_enb_id_t));
    son_enbid.enb_type = SON_HOME_ENB;
    es_cell_activation_resp.result = (son_return_et)p_resp->activation_res.peer_cell_activation_resp.response;
    es_cell_activation_resp.transaction_id = p_resp->transaction_id;
        /* Send Peer Cell Activation Response  to SON ES*/
        son_create_send_buffer((son_u8 *)&es_cell_activation_resp,
                SON_X2_MODULE_ID,
                SON_ES_MODULE_ID, SONX2_PEER_CELL_ACTIVATION_RESP,
                sizeof (smif_peer_cell_activation_res_t));

        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_BRIEF,
                " Successfully send peer cell activation response from SON X2 to SON ES");



    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* sonx2_peer_cell_activation_resp_handler */


/*****************************************************************************
 * Function Name  : x2_get_msg_idx_from_rrc_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SONX2 FSM
 * Description    : This function computes the index in the X2 FSM for the
 *                  messages from RRC.
 ****************************************************************************/
static son_x2_index_et
x2_get_msg_idx_from_rrc_msgs
(
 son_u16 from,
 son_u16 api
 )
{
	son_x2_index_et ret = X2_IDX_NOT_AVAILABLE;

	switch (api)
	{
		case X2AP_ADD_ENB_RES:
			{
				ret = X2_IDX_RRC_EVENT_ADD_ENB_RES;
				break;
			}

		case X2AP_ENB_LINK_UP_IND:
			{
				ret = X2_IDX_RRC_EVENT_LINK_UP_IND;
				break;
			}

		case X2AP_ENB_LINK_DOWN_IND:
			{
				ret = X2_IDX_RRC_EVENT_LINK_DOWN_IND;
				break;
			}

		case X2AP_DEL_ENB_RES:
			{
				ret = X2_IDX_RRC_EVENT_DELETE_ENB_RES;
				break;
			}

		case X2AP_ENB_CONFIG_UPDATE_RES:

			/* break is not used intentionally as both eNB config update
			 *  response and eNB config update wait indication are handled
			 *  using same handler function */
		case X2AP_ENB_CONFIG_UPDATE_WAIT_IND:
			{
				ret = X2_IDX_RRC_EVENT_ENB_CONFIG_UPDATE_RES;
				break;
			}

		case X2AP_ENB_CONFIG_UPDATE_IND:
			{
				ret = X2_IDX_RRC_EVENT_ENB_CONFIG_UPDATE_IND;
				break;
			}

		case X2AP_RESET_RES:
			{
				ret = X2_IDX_RRC_EVENT_ENB_RESET_RES;
				break;
			}

#ifdef SON_MRO_FEATURE
		case X2AP_HO_REPORT_IND:
			{
				ret = X2_IDX_RRC_EVENT_HO_REPORT_IND;
				break;
			}

#endif
		case X2AP_MOBILITY_SETTING_CHANGE_REQ:
			{
				ret = X2_IDX_SONMLB_EVENT_MOB_SETT_CHANGE_REQ;
				break;
			}

		case X2AP_MOBILITY_CHANGE_RESP:
			{
				ret = X2_IDX_SONMLB_EVENT_MOB_CHANGE_RESP;
				break;
			}

		case X2AP_CELL_ACTIVATION_REQ:
			{
				ret = X2_IDX_RRC_EVENT_CELL_ACTIVATION_REQ;
				break;
			}

/* SPR_19619 start */

		case X2AP_PEER_CELL_ACTIVATION_RESP:
			{
				ret = X2_IDX_RRC_EVENT_CELL_ACTIVATION_RESP;
				break;
			}
/* SPR_19619 stop */
		default:
			{
				SON_LOG(X2_GET_LOG_MODE,
						p_son_x2_facility_name, SON_WARNING,
						"Invalid API = [%u] received from %d", api, from);
				break;
			}
	} /* switch */

	return ret;
} /* x2_get_msg_idx_from_rrc_msgs */

/*****************************************************************************
 * Function Name  : x2_get_msg_idx_from_es_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SONX2 FSM
 * Description    : This function computes the index in the X2 FSM for the
 *                  messages from ES.
 ****************************************************************************/
static son_x2_index_et
x2_get_msg_idx_from_es_msgs
(
 son_u16 from,
 son_u16 api
 )
{
	son_x2_index_et ret = X2_IDX_NOT_AVAILABLE;

	switch (api)
	{
		case SONX2_SWITCH_ON_OFF_IND:
			{
				ret = X2_IDX_SONES_EVENT_CELL_SWITCH_ON_OFF_IND;
				break;
			}

		case SONES_CELL_ACTIVATION_RESP:
			{
				ret = X2_IDX_SONES_EVENT_CELL_ACTIVATION_RES;
				break;
			}

     /* SPR_19619 start */

        case SONX2_PEER_CELL_ACTIVATION_REQ:
        {	
            ret =  X2_IDX_SONES_EVENT_PEER_CELL_ACTIVATION_REQ;
            break;
        }

        /* SPR_19619 stop */

		default:
			{
				SON_LOG(X2_GET_LOG_MODE,
						p_son_x2_facility_name, SON_WARNING,
						"Invalid API = [%u] received from [%u]", api, from);
				break;
			}
	}

	return ret;
} /* x2_get_msg_idx_from_es_msgs */

/*****************************************************************************
 * Function Name  : x2_get_msg_idx_from_mlb_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SONX2 FSM
 * Description    : This function computes the index in the X2 FSM for the
 *                  messages from ES.
 ****************************************************************************/
static son_x2_index_et
x2_get_msg_idx_from_mlb_msgs
(
 son_u16 from,
 son_u16 api
 )
{
	son_x2_index_et ret = X2_IDX_NOT_AVAILABLE;

	switch (api)
	{
		case SONMLB_MOBILITY_CHANGE_REQ:
			{
				ret = X2_IDX_SONMLB_EVENT_MOB_CHANGE_REQ;
				break;
			}

		case SONMLB_MOBILITY_CHANGE_ACK:
			{
				ret = X2_IDX_SONMLB_EVENT_MOB_CHANGE_ACK;
				break;
			}

		case SONMLB_MOBILITY_CHANGE_FAILURE:
			{
				ret = X2_IDX_SONMLB_EVENT_MOB_CHANGE_FAIL;
				break;
			}

		default:
			{
				SON_LOG(X2_GET_LOG_MODE,
						p_son_x2_facility_name, SON_WARNING,
						"Invalid API = [%u] received from [%u]", api, from);
				break;
			}
	} /* switch */

	return ret;
} /* x2_get_msg_idx_from_mlb_msgs */

/*****************************************************************************
 * Function Name  : x2_get_msg_idx_from_anr_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SONX2 FSM
 * Description    : This function computes the index in the X2 FSM for the
 *                  messages from ANR.
 ****************************************************************************/
static son_x2_index_et
x2_get_msg_idx_from_anr_msgs
(
 son_u16 from,
 son_u16 api
 )
{
	son_x2_index_et ret = X2_IDX_NOT_AVAILABLE;

	switch (api)
	{
		case SONX2_ADD_ENB_REQ:
			{
				ret = X2_IDX_SONANR_EVENT_ADD_ENB_REQ;
				break;
			}

		case SONX2_DELETE_ENB_REQ:
			{
				ret = X2_IDX_SONANR_EVENT_DELETE_ENB_REQ;
				break;
			}

		case SONX2_NRT_UPDATE_IND:
			{
				ret = X2_IDX_SONANR_EVENT_NRT_UPDATE_IND;
				break;
			}

		default:
			{
				SON_LOG(X2_GET_LOG_MODE,
						p_son_x2_facility_name, SON_WARNING,
						"Invalid API = [%u] received from [%u]", api, from);
				break;
			}
	} /* switch */

	return ret;
} /* x2_get_msg_idx_from_anr_msgs */

/*****************************************************************************
 * Function Name  : x2_get_msg_idx_from_mif_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SONApps FSM
 * Description    : This function computes the index in the Apps FSM for the
 *                  messages from MIF
 ****************************************************************************/
static son_x2_index_et
x2_get_msg_idx_from_mif_msgs
(
 son_u16 from,
 son_u16 api
 )
{
	son_x2_index_et ret = X2_IDX_NOT_AVAILABLE;

	switch (api)
	{
		case SMIF_INIT_CONFIG_REQ:
			{
				ret = X2_IDX_SMIF_EVENT_INIT_CONFIG_REQ;
				break;
			}

		case SMIF_SET_LOG_LEVEL_CMD:
			{
				ret = X2_IDX_SMIF_EVENT_SET_LOG_LEVEL_CMD;
				break;
			}

		case SMIF_SHUTDOWN_REQ:
			{
				ret = X2_IDX_SMIF_EVENT_SHUTDOWN_REQ;
				break;
			}

		case SONX2_GU_GROUP_ID_CONFIG_REQ:
			ret = X2_IDX_SMIF_EVENT_GU_GROUP_ID_CONFIG_REQ;
			break;

		default:
			{
				SON_LOG(X2_GET_LOG_MODE,
						p_son_x2_facility_name, SON_WARNING,
						"Invalid API = [%u] received from [%u]", api, from);
				break;
			}
	} /* switch */

	return ret;
} /* x2_get_msg_idx_from_mif_msgs */

/*****************************************************************************
 * Function Name  : x2_calculate_msg_index
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SONX2 FSM
 * Description    : This function computes the index in the X2 FSM for the
 *                  given procedure code
 ****************************************************************************/
static son_x2_index_et
x2_calculate_msg_index
(
 son_u16 from,
 son_u16 api
 )
{
	son_x2_index_et ret = X2_IDX_NOT_AVAILABLE;

	SON_UT_TRACE_ENTER();

	if (SON_MIF_MODULE_ID == from)
	{
		ret = x2_get_msg_idx_from_mif_msgs(from, api);
	}
	else if (SON_ANR_MODULE_ID == from)
	{
		ret = x2_get_msg_idx_from_anr_msgs(from, api);
	}
	else if (SON_ES_MODULE_ID == from)
	{
		ret = x2_get_msg_idx_from_es_msgs(from, api);
	}
	else if (SON_MLB_MODULE_ID == from)
	{
		ret = x2_get_msg_idx_from_mlb_msgs(from, api);
	}
	else if (SON_RRC_MODULE_ID == from)
	{
		ret = x2_get_msg_idx_from_rrc_msgs(from, api);
	}
	else
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_WARNING,
				"Message with API Id = [%u] received "
				"from unknown entity : [%u]", api, from);
	}

	SON_UT_TRACE_EXIT();
	return ret;
} /* x2_calculate_msg_index */

/************************* X2 Global State Machine*********************/
typedef son_buf_retain_status_et (*p_x2_fsm_evt_handler_t)
	(
	 son_u8 *p_msg,
	 STACKAPIHDR *p_cspl_hdr,
	 son_x2_index_et evnt_id
	);

	/***********************************************************************
	 * X2 Global FSM Entry Point Table
	 **********************************************************************/
	static p_x2_fsm_evt_handler_t
	son_x2_fsm_table[X2_NUM_OF_STATES][X2_NUM_OF_EVENTS] =
{
	/* State INIT */
	{
		sonx2_fsm_init_config_req,
		/* SMIF_EVENT_INIT_CONFIG_REQ */
		sonx2_fsm_invalid_event_handler,
		/*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
		sonx2_fsm_shutdown_req,
		/* SMIF_EVENT_SHUTDOWN_REQ */
		sonx2_fsm_nack_add_enb_req,
		/* SONX2_EVENT_ADD_ENB_REQ */
		sonx2_fsm_nack_delete_enb_req,
		/* SONX2_EVENT_DELETE_ENB_REQ */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_NRT_UPDATE_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_CELL_SWITCH_ON_OFF_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_CELL_ACTIVATION_RES*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_ADD_ENB_RES */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_LINK_UP_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_LINK_DOWN_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_DELETE_ENB_RES */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_CELL_ACTIVATION_REQ*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_RES */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_ENB_RESET_RES */
#ifdef SON_MRO_FEATURE
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_HO_REPORT_IND */
#endif
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_MOBILITY_SETTING_CHANGE_REQ,L3-X2AP->SON*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_MOBILITY_CHANGE_RESP,L3-X2AP->SON*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_REQ,SON->L3-X2AP*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_ACK,SON->L3-X2AP*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_FAILURE,SON->L3-X2AP*/
		/* SPR-10331 Fix Starts */
        /* SPR_19619 start */
		sonx2_fsm_nack_gu_group_id_config_req,
		/* X2_SMIF_EVENT_GU_GROUP_ID_CONFIG_REQ */
		/* SPR-10331 Fix Ends */
        sonx2_fsm_invalid_event_handler,
        /* SONX2_EVENT_PEER_CELL_ACTIVATION_REQ */
        sonx2_fsm_invalid_event_handler
        /* X2AP_PEER_CELL_ACTIVATION_RESP*/
        /* SPR_19619 stop */
    },
	/* State ACTIVE */
	{
		sonx2_fsm_nack_init_config_req,
		/* SMIF_EVENT_INIT_CONFIG_REQ */
		sonx2_fsm_set_log_level_cmd_handler,
		/*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
		sonx2_fsm_shutdown_req,
		/* SMIF_EVENT_SHUTDOWN_REQ */
		sonx2_fsm_active_state_add_enb_req_handler,
		/* SONX2_EVENT_ADD_ENB_REQ */
		sonx2_fsm_nack_delete_enb_req,
		/* SONX2_EVENT_DELETE_ENB_REQ */
		sonx2_fsm_nrt_update_ind,
		/* SONX2_EVENT_NRT_UPDATE_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_CELL_SWITCH_ON_OFF_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_CELL_ACTIVATION_RES*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_ADD_ENB_RES */
		sonx2_fsm_link_up_ind,
		/* SONX2_EVENT_LINK_UP_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_LINK_DOWN_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_DELETE_ENB_RES */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_CELL_ACTIVATION_REQ*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_RES */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_ENB_RESET_RES */
#ifdef SON_MRO_FEATURE
		sonx2_fsm_ho_report_ind_handler,
		/* SONX2_EVENT_HO_REPORT_IND */
#endif
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_MOBILITY_SETTING_CHANGE_REQ,L3-X2AP->SON*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_MOBILITY_CHANGE_RESP,L3-X2AP->SON*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_REQ,SON->L3-X2AP*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_ACK,SON->L3-X2AP*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_FAILURE,SON->L3-X2AP*/
		/* SPR-10331 Fix Starts */
        /* SPR_19619 start */
		sonx2_fsm_gu_group_id_config_req_in_active_connecting_state,
		/* X2_SMIF_EVENT_GU_GROUP_ID_CONFIG_REQ */
		/* SPR-10331 Fix Ends */
        sonx2_fsm_peer_cell_activation_req,
        /* SONX2_EVENT_PEER_CELL_ACTIVATION_REQ */
        sonx2_fsm_invalid_event_handler
        /* X2AP_PEER_CELL_ACTIVATION_RESP*/
        /* SPR_19619 stop */
	},
	/* State CONNECTING */
	{
		sonx2_fsm_nack_init_config_req,
		/* SMIF_EVENT_INIT_CONFIG_REQ */
		sonx2_fsm_set_log_level_cmd_handler,
		/*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
		sonx2_fsm_shutdown_req,
		/* SMIF_EVENT_SHUTDOWN_REQ */
		sonx2_fsm_connecting_connected_state_add_enb_req_handler,
		/* SONX2_EVENT_ADD_ENB_REQ */
		/*SPR_17670_START*/
		sonx2_fsm_delete_enb_req,
		/*SPR_17670_END*/
		/* SONX2_EVENT_DELETE_ENB_REQ */
		sonx2_fsm_nrt_update_ind,
		/* SONX2_EVENT_NRT_UPDATE_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_CELL_SWITCH_ON_OFF_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_CELL_ACTIVATION_RES*/
		sonx2_fsm_add_enb_res,
		/* SONX2_EVENT_ADD_ENB_RES */
		sonx2_fsm_link_up_ind,
		/* SONX2_EVENT_LINK_UP_IND */
		sonx2_fsm_link_down_ind,
		/* SONX2_EVENT_LINK_DOWN_IND */
		/*SPR_17670_START*/
		sonx2_fsm_delete_enb_res,
		/*SPR_17670_END*/
		/* SONX2_EVENT_DELETE_ENB_RES */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_CELL_ACTIVATION_REQ*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_RES */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_IND */
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_ENB_RESET_RES */
#ifdef SON_MRO_FEATURE
		sonx2_fsm_ho_report_ind_handler,
		/* SONX2_EVENT_HO_REPORT_IND */
#endif
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_MOBILITY_SETTING_CHANGE_REQ,L3-X2AP->SON*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_MOBILITY_CHANGE_RESP,L3-X2AP->SON*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_REQ,SON->L3-X2AP*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_ACK,SON->L3-X2AP*/
		sonx2_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_FAILURE,SON->L3-X2AP*/
		/* SPR-10331 Fix Starts */
        /* SPR_19619 start */
		sonx2_fsm_gu_group_id_config_req_in_active_connecting_state,
		/* X2_SMIF_EVENT_GU_GROUP_ID_CONFIG_REQ */
		/* SPR-10331 Fix Ends */
        sonx2_fsm_invalid_event_handler,
        /* SONX2_EVENT_PEER_CELL_ACTIVATION_REQ */
        sonx2_fsm_invalid_event_handler
        /* X2AP_PEER_CELL_ACTIVATION_RESP*/
        /* SPR_19619 stop */
	},
	/* State CONNECTED */
	{
		sonx2_fsm_nack_init_config_req,
		/* SMIF_EVENT_INIT_CONFIG_REQ */
		sonx2_fsm_set_log_level_cmd_handler,
		/*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
		sonx2_fsm_shutdown_req,
		/* SMIF_EVENT_SHUTDOWN_REQ */
		sonx2_fsm_connecting_connected_state_add_enb_req_handler,
		/* SONX2_EVENT_ADD_ENB_REQ */
		sonx2_fsm_delete_enb_req,
		/* SONX2_EVENT_DELETE_ENB_REQ */
		sonx2_fsm_nrt_update_ind,
		/* SONX2_EVENT_NRT_UPDATE_IND */
		sonx2_fsm_switch_on_off_ind,
		/* SONX2_EVENT_CELL_SWITCH_ON_OFF_IND */
		sonx2_fsm_cell_activation_res,
		/* SONX2_EVENT_CELL_ACTIVATION_RES*/
		sonx2_fsm_add_enb_res,
		/* SONX2_EVENT_ADD_ENB_RES */
		sonx2_fsm_link_up_ind,
		/* SONX2_EVENT_LINK_UP_IND */
		sonx2_fsm_link_down_ind,
		/* SONX2_EVENT_LINK_DOWN_IND */
		sonx2_fsm_delete_enb_res,
		/* SONX2_EVENT_DELETE_ENB_RES */
		sonx2_fsm_cell_activation_req,
		/* SONX2_EVENT_CELL_ACTIVATION_REQ*/
		sonx2_fsm_enb_config_update_res,
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_RES */
		sonx2_fsm_enb_config_update_ind_handler,
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_IND */
		sonx2_fsm_enb_reset_resp,
		/* SONX2_EVENT_ENB_RESET_RES */
#ifdef SON_MRO_FEATURE
		sonx2_fsm_ho_report_ind_handler,
		/* SONX2_EVENT_HO_REPORT_IND */
		sonx2_fsm_mob_setting_change_handler,
		/* SONX2_EVENT_MOBILITY_SETTING_CHANGE_REQ,L3-X2AP->SON*/
		sonx2_fsm_mob_change_resp_handler,
#endif
		/* SONX2_EVENT_MOBILITY_CHANGE_RESP,L3-X2AP->SON*/
		sonx2_fsm_sonmlb_mob_change_req_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_REQ,SON->L3-X2AP*/
		sonx2_fsm_sonmlb_mob_change_ack_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_ACK,SON->L3-X2AP*/
		sonx2_fsm_sonmlb_mob_change_fail_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_FAILURE,SON->L3-X2AP*/
		/* SPR-10331 Fix Starts */
        /* SPR_19619 start */
		sonx2_fsm_gu_group_id_config_req_in_connected_state,
		/* X2_SMIF_EVENT_GU_GROUP_ID_CONFIG_REQ */
		/* SPR-10331 Fix Ends */
        sonx2_fsm_peer_cell_activation_req,
        /* SONX2_EVENT_PEER_CELL_ACTIVATION_REQ */
         sonx2_peer_cell_activation_resp_handler
        /* X2AP_PEER_CELL_ACTIVATION_RESP*/
        /* SPR_19619 start */
	},
	/* State SHUTDOWN */
	{
		sonx2_fsm_shutdown_state_handler,
		/* SMIF_EVENT_INIT_CONFIG_REQ */
		sonx2_fsm_shutdown_state_handler,
		/*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
		sonx2_fsm_shutdown_state_handler,
		/* SMIF_EVENT_SHUTDOWN_REQ */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_ADD_ENB_REQ */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_DELETE_ENB_REQ */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_NRT_UPDATE_IND */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_CELL_SWITCH_ON_OFF_IND */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_CELL_ACTIVATION_RES*/
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_ADD_ENB_RES */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_LINK_UP_IND */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_LINK_DOWN_IND */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_DELETE_ENB_RES */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_CELL_ACTIVATION_REQ*/
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_RES */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_IND */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_ENB_RESET_RES */
#ifdef SON_MRO_FEATURE
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_HO_REPORT_IND */
#endif
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_MOBILITY_SETTING_CHANGE_REQ,L3-X2AP->SON */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_MOBILITY_CHANGE_RESP,L3-X2AP->SON */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_REQ,SON->L3-X2AP */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_ACK,SON->L3-X2AP */
		sonx2_fsm_shutdown_state_handler,
		/* SONX2_EVENT_SON_MLB_MOBILTY_CHANGE_FAILURE`,SON->L3-X2AP */
		/* SPR-10331 Fix Starts */
       /* SPR_19619 start */
		sonx2_fsm_shutdown_state_handler,
	    /* X2_SMIF_EVENT_GU_GROUP_ID_CONFIG_REQ */
		/* SPR-10331 Fix Ends */
        sonx2_fsm_invalid_event_handler,
        /* SONX2_EVENT_PEER_CELL_ACTIVATION_REQ */
        sonx2_fsm_invalid_event_handler
        /* X2AP_PEER_CELL_ACTIVATION_RESP*/
        /* SPR_19619 stop */
	}
};

/****************************************************************************
 * Function Name  : x2_parse_add_enb_res
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the add eNB response
 *                  message received from RRC
 ****************************************************************************/
static son_u8 *
x2_parse_add_enb_res
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	S32 length_read = 0;

	SON_UT_TRACE_ENTER();

	p_parsed_msg = son_mem_get(
			sizeof(son_rrc_add_enb_res_t));
	if (SON_PNULL != p_parsed_msg)
	{
		if (SON_FAILURE == (son_return_et) rrc_parse_x2ap_add_enb_res(
					&((son_rrc_add_enb_res_t *)p_parsed_msg)->
					add_enb_res,
					(p_msg + EXT_MSG_API_HDR_SIZE),
					buf_len - EXT_MSG_API_HDR_SIZE,
					&length_read))
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_ERROR,
					"RRC->SONX2:X2AP_ADD_ENB_RES parsing "
					"failed");
			/* Free the memory when parsing is failed */
			son_mem_free(p_parsed_msg);
			p_parsed_msg = SON_PNULL;
		}
		else
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_INFO,
					"RRC->SONX2:X2AP_ADD_ENB_RES");

			*(son_u16 *)p_parsed_msg = son_get_word_from_header(
					&p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
		}
	}

	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_add_enb_res */

/****************************************************************************
 * Function Name  : x2_parse_delete_enb_res
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the delete eNB response
 *                  message received from RRC
 ****************************************************************************/
static son_u8 *
x2_parse_delete_enb_res
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	S32 length_read = 0;

	SON_UT_TRACE_ENTER();

	p_parsed_msg = son_mem_get(
			sizeof(son_rrc_delete_enb_res_t));
	if (SON_PNULL != p_parsed_msg)
	{
		if (SON_FAILURE == (son_return_et)rrc_parse_x2ap_delete_enb_res(
					&((son_rrc_delete_enb_res_t *)p_parsed_msg)->
					del_enb_res,
					(p_msg + EXT_MSG_API_HDR_SIZE),
					buf_len - EXT_MSG_API_HDR_SIZE,
					&length_read))
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_ERROR,
					"RRC->SONX2:X2AP_DEL_ENB_RES parsing failed");
			/* Free the memory when parsing is failed */
			son_mem_free(p_parsed_msg);
			p_parsed_msg = SON_PNULL;
		}
		else
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_INFO,
					"RRC->SONX2:X2AP_DEL_ENB_RES response %u",
					((son_rrc_delete_enb_res_t *)p_parsed_msg)->del_enb_res.response);
			*(son_u16 *)p_parsed_msg = son_get_word_from_header(
					&p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
		}
	}

	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_delete_enb_res */

/****************************************************************************
 * Function Name  : x2_parse_link_up_ind
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the link up indication
 *                  message received from RRC
 ****************************************************************************/
static son_u8 *
x2_parse_link_up_ind
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	S32 length_read = 0;

	SON_UT_TRACE_ENTER();

	p_parsed_msg = son_mem_get(sizeof(son_rrc_link_up_ind_t));
	if (SON_PNULL != p_parsed_msg &&
			SON_FAILURE == (son_return_et) rrc_parse_x2ap_enb_link_up_ind(
				(son_rrc_link_up_ind_t *)p_parsed_msg,
				(p_msg + EXT_MSG_API_HDR_SIZE),
				buf_len - EXT_MSG_API_HDR_SIZE,
				&length_read))
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_ERROR,
				"RRC->SONX2:X2AP_ENB_LINK_UP_IND parsing failed");
		/* Free the memory when parsing is failed */
		son_mem_free(p_parsed_msg);
		p_parsed_msg = SON_PNULL;
	}
    else
    {
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_INFO,
				"RRC->SONX2:X2AP_ENB_LINK_UP_IND");
    }

	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_link_up_ind */
/****************************************************************************
 * Function Name  : x2_parse_cell_activation_req
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the cell activation request
 *                  message received from RRC
 ****************************************************************************/
static son_u8 *
x2_parse_cell_activation_req
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
	son_u8  *p_parsed_msg = SON_PNULL;
	S32 length_read = 0;


	SON_UT_TRACE_ENTER();

	p_parsed_msg = son_mem_get(sizeof(son_rrc_cell_activation_req_t));
	if (SON_PNULL != p_parsed_msg )
	{ 
		if (SON_FAILURE == (son_return_et)rrc_parse_x2ap_cell_activation_req(
					&((son_rrc_cell_activation_req_t *)
						p_parsed_msg)->cell_activation_req,
					(p_msg + EXT_MSG_API_HDR_SIZE),
					buf_len - EXT_MSG_API_HDR_SIZE,
					&length_read))

		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_ERROR,
					"RRC->SONX2:X2AP_CELL_ACTIVATION_REQ parsing failed");
			/* Free the memory when parsing is failed */
			son_mem_free(p_parsed_msg);
			p_parsed_msg = SON_PNULL;

		}
		else
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_INFO,
					"RRC->SONX2:X2AP_CELL_ACTIVATION_REQ");
			*(son_u16 *)p_parsed_msg = son_get_word_from_header(
					&p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
		}
	}
	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_cell_activation_req */


/****************************************************************************
 * Function Name  : x2_parse_link_down_ind
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the link down indication
 *                  message received from RRC
 ****************************************************************************/
static son_u8 *
x2_parse_link_down_ind
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	S32 length_read = 0;

	SON_UT_TRACE_ENTER();

	p_parsed_msg = son_mem_get(sizeof(son_rrc_link_down_ind_t));
	if (SON_PNULL != p_parsed_msg &&
			SON_FAILURE == (son_return_et) rrc_parse_x2ap_enb_link_down_ind(
				(son_rrc_link_down_ind_t *)p_parsed_msg,
				(p_msg + EXT_MSG_API_HDR_SIZE),
				buf_len - EXT_MSG_API_HDR_SIZE,
				&length_read))
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_ERROR,
				"RRC->SONX2:X2AP_ENB_LINK_DOWN_IND parsing failed");
		/* Free the memory when parsing is failed */
		son_mem_free(p_parsed_msg);
		p_parsed_msg = SON_PNULL;
	}
    else
    {
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_INFO,
				"RRC->SONX2:X2AP_ENB_LINK_DOWN_IND");
    }

	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_link_down_ind */

/****************************************************************************
 * Function Name  : x2_parse_enb_config_update_res
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the eNB config update
 *                  response message received from RRC
 ****************************************************************************/
static son_u8 *
x2_parse_enb_config_update_res
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	S32 length_read = 0;

	SON_UT_TRACE_ENTER();

	p_parsed_msg = son_mem_get(
			sizeof(son_rrc_enb_config_update_res_t));
	if (SON_PNULL != p_parsed_msg)
	{
		if (SON_FAILURE == (son_return_et) rrc_parse_x2ap_enb_config_update_resp(
					&((son_rrc_enb_config_update_res_t *)
						p_parsed_msg)->
					enb_config_update_res,
					(p_msg + EXT_MSG_API_HDR_SIZE),
					buf_len - EXT_MSG_API_HDR_SIZE,
					&length_read))
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_ERROR,
					"RRC->SONX2:X2AP_ENB_CONFIG_UPDATE_RES parsing failed");
			/* Free the memory when parsing is failed */
			son_mem_free(p_parsed_msg);
			p_parsed_msg = SON_PNULL;
		}
		else
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_INFO,
					"RRC->SONX2:X2AP_ENB_CONFIG_UPDATE_RES response %u, Cause %u",
					((son_rrc_enb_config_update_res_t *)p_parsed_msg)->enb_config_update_res.response,
					((son_rrc_enb_config_update_res_t *)p_parsed_msg)->enb_config_update_res.cause);
			*(son_u16 *)p_parsed_msg = son_get_word_from_header(
					&p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
		}
	}

	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_enb_config_update_res */

/*****************************************************************************
 * Function Name  : x2_parse_enb_config_update_wait_ind
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the eNB config update
 *                  wait indication message received from RRC
 ****************************************************************************/
static son_u8 *
x2_parse_enb_config_update_wait_ind
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	S32 length_read = 0;

	SON_UT_TRACE_ENTER();

	p_parsed_msg = son_mem_get(
			sizeof(son_x2_enb_config_update_wait_ind));
	if (SON_PNULL != p_parsed_msg)
	{
		if (SON_FAILURE == (son_return_et) rrc_parse_x2ap_enb_config_update_wait_ind(
				(son_x2_enb_config_update_wait_ind *)
				p_parsed_msg,
				(p_msg + EXT_MSG_API_HDR_SIZE),
				buf_len - EXT_MSG_API_HDR_SIZE,
				&length_read))
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_ERROR,
					"RRC->SONX2:X2AP_ENB_CONFIG_UPDATE_WAIT_IND parsing failed");
		/* Free the memory when parsing is failed */
		son_mem_free(p_parsed_msg);
		p_parsed_msg = SON_PNULL;
	}
		else
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_INFO,
					"RRC->SONX2:X2AP_ENB_CONFIG_UPDATE_WAIT_IND time_to_wait %u",
					((son_x2_enb_config_update_wait_ind *)p_parsed_msg)->time_to_wait);
		}
	}
    else
    {
        SON_LOG(X2_GET_LOG_MODE,
                p_son_x2_facility_name, SON_ERROR,
                "Memory allocation failed for p_parsed_msg."
                " X2AP_ENB_CONFIG_UPDATE_WAIT_IND cannot be decoded");
    }


	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_enb_config_update_wait_ind */

/*****************************************************************************
 * Function Name  : x2_parse_enb_config_update_ind
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the eNB config update
 *                  indication message received from RRC
 ****************************************************************************/
static son_u8 *
x2_parse_enb_config_update_ind
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	S32 length_read = 0;

	SON_UT_TRACE_ENTER();

	p_parsed_msg = son_mem_get(
			sizeof(son_rrc_enb_config_update_ind_t));
	if (SON_PNULL != p_parsed_msg)
	{
		if (SON_FAILURE == (son_return_et) rrc_parse_x2ap_enb_config_update_ind(
				(son_rrc_enb_config_update_ind_t *)
				p_parsed_msg,
				(p_msg + EXT_MSG_API_HDR_SIZE),
				buf_len - EXT_MSG_API_HDR_SIZE,
				&length_read))
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_ERROR,
					"RRC->SONX2:X2AP_ENB_CONFIG_UPDATE_IND parsing failed");
		/* Free the memory when parsing is failed */
		son_mem_free(p_parsed_msg);
		p_parsed_msg = SON_PNULL;
	}
		else
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_INFO,
					"RRC->SONX2:X2AP_ENB_CONFIG_UPDATE_IND");
		}
	}
    else
    {
        SON_LOG(X2_GET_LOG_MODE,
                p_son_x2_facility_name, SON_ERROR,
                "Memory allocation failed for p_parsed_msg."
                " X2AP_ENB_CONFIG_UPDATE_IND cannot be decoded");
    }

	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_enb_config_update_ind */

#ifdef SON_MRO_FEATURE

/*****************************************************************************
 * Function Name  : x2_parse_ho_report_ind
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the HO report indication
 *                  message received from RRC
 ****************************************************************************/
static son_u8 *
x2_parse_ho_report_ind
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	S32 length_read = 0;

	SON_UT_TRACE_ENTER();
	p_parsed_msg = son_mem_get(sizeof(son_rrc_ho_report_ind_t));
	if (SON_PNULL != p_parsed_msg)
	{
		if (SON_FAILURE == (son_return_et) rrc_parse_x2ap_ho_report_ind(
				(son_rrc_ho_report_ind_t *)p_parsed_msg,
				(p_msg + EXT_MSG_API_HDR_SIZE),
				buf_len - EXT_MSG_API_HDR_SIZE,
				&length_read))
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_ERROR,
					"RRC->SON:X2AP_HO_REPORT_IND parsing failed");
		/* Free the memory when parsing is failed */
		son_mem_free(p_parsed_msg);
		p_parsed_msg = SON_PNULL;
	}
		else
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_INFO,
					"RRC->SON_X2:X2AP_HO_REPORT_IND");
		}
	}
    else
    {
        SON_LOG(X2_GET_LOG_MODE,
                p_son_x2_facility_name, SON_ERROR,
                "Memory allocation failed for p_parsed_msg."
                " X2AP_HO_REPORT_IND cannot be decoded");
    }

	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_ho_report_ind */

#endif /* ifdef SON_MRO_FEATURE */

/*****************************************************************************
 * Function Name  : x2_parse_reset_res
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the reset response message
 *                  received from RRC
 ****************************************************************************/
static son_u8 *
x2_parse_reset_res
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	S32 length_read = 0;

	SON_UT_TRACE_ENTER();

	p_parsed_msg = son_mem_get(
			sizeof(son_rrc_reset_res_t));
	if (SON_PNULL != p_parsed_msg)
	{
		if (SON_FAILURE == (son_return_et) rrc_parse_x2ap_reset_resp(
					&((son_rrc_reset_res_t *)
						p_parsed_msg)->reset_res,
					(p_msg + EXT_MSG_API_HDR_SIZE),
					buf_len - EXT_MSG_API_HDR_SIZE,
					&length_read))
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_ERROR,
					"RRC->SON:X2AP_RESET_RES parsing failed");
			/* Free the memory when parsing is failed */
			son_mem_free(p_parsed_msg);
			p_parsed_msg = SON_PNULL;
		}
		else
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_INFO,
					"RRC->SON_X2:X2AP_RESET_RES response %u",
                    ((son_rrc_reset_res_t *) p_parsed_msg)->reset_res.response);

			*(son_u16 *)p_parsed_msg = son_get_word_from_header(
					&p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
		}
	}
    else
    {
        SON_LOG(X2_GET_LOG_MODE,
                p_son_x2_facility_name, SON_ERROR,
                "Memory allocation failed for p_parsed_msg."
                " X2AP_RESET_RES cannot be decoded");
    }

	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_reset_res */

#ifdef SON_MRO_FEATURE

/*****************************************************************************
 * Function Name  : x2_parse_mobility_setting_change_req
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the
 *                  X2AP_MOBILITY_SETTING_CHANGE_REQ message received from RRC
 ****************************************************************************/
static son_u8 *
x2_parse_mobility_setting_change_req
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	S32 length_read = 0;

	SON_UT_TRACE_ENTER();

	p_parsed_msg = son_mem_get(
			sizeof(son_rrc_mobility_settings_change_req_t));
	if (SON_PNULL != p_parsed_msg)
	{
		if (SON_FAILURE == (son_return_et)rrc_parse_x2ap_mobility_setting_change_req(
					&((son_rrc_mobility_settings_change_req_t *)
						p_parsed_msg)->mob_setting_change_req,
					(p_msg + EXT_MSG_API_HDR_SIZE),
					buf_len - EXT_MSG_API_HDR_SIZE,
					&length_read))
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_ERROR,
					"RRC->SONX2:X2AP_MOBILITY_SETTING_CHANGE_REQ parsing failed");
			/* Free the memory when parsing is failed */
			son_mem_free(p_parsed_msg);
			p_parsed_msg = SON_PNULL;
		}
		else
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_INFO,
					"RRC->SONX2:X2AP_MOBILITY_SETTING_CHANGE_REQ");
			*(son_u16 *)p_parsed_msg = son_get_word_from_header(
					&p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
		}
	}
    else
    {
        SON_LOG(X2_GET_LOG_MODE,
                p_son_x2_facility_name, SON_ERROR,
                "Memory allocation failed for p_parsed_msg."
                " X2AP_MOBILITY_SETTING_CHANGE_REQ cannot be decoded");
    }

	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_mobility_setting_change_req */

/*****************************************************************************
 * Function Name  : x2_parse_mobility_change_resp
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the
 *                  X2AP_MOBILITY_CHANGE_RESP message received from RRC
 ****************************************************************************/
static son_u8 *
x2_parse_mobility_change_resp
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	S32 length_read = 0;

	SON_UT_TRACE_ENTER();

	p_parsed_msg = son_mem_get(
			sizeof(son_rrc_mobility_change_resp_t));
	if (SON_PNULL != p_parsed_msg)
	{
		if (SON_FAILURE == (son_return_et) rrc_parse_x2ap_mobility_change_resp(
					&((son_rrc_mobility_change_resp_t *)
						p_parsed_msg)->mob_change_res,
					(p_msg + EXT_MSG_API_HDR_SIZE),
					buf_len - EXT_MSG_API_HDR_SIZE,
					&length_read))
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_ERROR,
					"RRC->SONX2:X2AP_MOBILITY_CHANGE_RESP parsing failed");
			/* Free the memory when parsing is failed */
			son_mem_free(p_parsed_msg);
			p_parsed_msg = SON_PNULL;
		}
		else
		{
			SON_LOG(X2_GET_LOG_MODE,
					p_son_x2_facility_name, SON_INFO,
					"RRC->SONX2:X2AP_MOBILITY_CHANGE_RESP response %u, cause %u",
					((son_rrc_mobility_change_resp_t *)p_parsed_msg)->mob_change_res.mobility_change_resp.response,
					((son_rrc_mobility_change_resp_t *)p_parsed_msg)->mob_change_res.mobility_change_resp.cause);

			*(son_u16 *)p_parsed_msg = son_get_word_from_header(
					&p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
		}
	}
    else
    {
        SON_LOG(X2_GET_LOG_MODE,
                p_son_x2_facility_name, SON_ERROR,
                "Memory allocation failed for p_parsed_msg."
                " X2AP_MOBILITY_CHANGE_RESP cannot be decoded");
    }

	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_mobility_change_resp */

#endif /* ifdef SON_MRO_FEATURE */

/* SPR_19619 start */

/*****************************************************************************
 * Function Name  : x2_parse_peer_cell_activation_resp
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the
 *                  X2AP_PEER_CELL_ACTIVATION_RESP message received from RRC
 ****************************************************************************/

static son_u8 *
x2_parse_peer_cell_activation_resp
(
 son_u8  *p_msg,
 son_u16 buf_len,
 son_u16 api_id
 )
{
    son_u8 *p_parsed_msg = SON_PNULL;
    S32 length_read = 0;

    SON_UT_TRACE_ENTER();

    p_parsed_msg = son_mem_get(
            sizeof(son_rrc_peer_cell_activation_resp_t));
    if (SON_PNULL != p_parsed_msg)
    {
        if (SON_FAILURE == (son_return_et)rrc_parse_x2ap_peer_cell_activation_resp(
                    &((son_rrc_peer_cell_activation_resp_t *)
                        p_parsed_msg)->activation_res,
                    (p_msg + EXT_MSG_API_HDR_SIZE),
                    buf_len - EXT_MSG_API_HDR_SIZE,
                    &length_read))
        {
            SON_LOG(X2_GET_LOG_MODE,
                    p_son_x2_facility_name, SON_ERROR,
                    "RRC->SONX2:X2AP_PEER_CELL_ACTIVATION_RESP parsing failed");
            /* Free the memory when parsing is failed */
            son_mem_free(p_parsed_msg);
            p_parsed_msg = SON_PNULL;
        }
        else
        {
            SON_LOG(X2_GET_LOG_MODE,
                    p_son_x2_facility_name, SON_INFO,
                    "RRC->SONX2:X2AP_PEER_CELL_ACTIVATION_RESP response %u, cause %u",
                    ((son_rrc_peer_cell_activation_resp_t *)p_parsed_msg)->activation_res.peer_cell_activation_resp.response,
                    ((son_rrc_peer_cell_activation_resp_t *)p_parsed_msg)->activation_res.peer_cell_activation_resp.cause);

            *(son_u16 *)p_parsed_msg = son_get_word_from_header(
                    &p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
        }
    }
    else
    {
        SON_LOG(X2_GET_LOG_MODE,
                p_son_x2_facility_name, SON_ERROR,
                "Memory allocation failed for p_parsed_msg."
                " X2AP_PEER_CELL_ACTIVATION_RESP cannot be decoded");
    }

    SON_UT_TRACE_EXIT();
    return p_parsed_msg;
} /* x2_parse_peer_cell_activation_resp */
/* SPR_19619 stop */

/*****************************************************************************
 * Function Name  : x2_parse_rrc_msgs
 * Inputs         : p_msg  - Pointer to the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the messages received from
 *                  RRC
 ****************************************************************************/
static son_u8 *
x2_parse_rrc_msgs
(
 son_u8 *p_msg,
 son_u16 api_id
 )
{
	son_u8 *p_parsed_msg = SON_PNULL;
	son_u16 buf_len = 0;

	SON_UT_TRACE_ENTER();

	buf_len = son_get_word_from_header(&p_msg[EXT_MSG_BUF_LEN_OFFSET]);

	switch (api_id)
	{
		case X2AP_ADD_ENB_RES:
			{
				p_parsed_msg = x2_parse_add_enb_res(p_msg, buf_len, api_id);
				break;
			}

		case X2AP_DEL_ENB_RES:
			{
				p_parsed_msg = x2_parse_delete_enb_res(p_msg, buf_len, api_id);
				break;
			}

		case X2AP_ENB_LINK_UP_IND:
			{
				p_parsed_msg = x2_parse_link_up_ind(p_msg, buf_len, api_id);
				break;
			}

		case X2AP_ENB_LINK_DOWN_IND:
			{
				p_parsed_msg = x2_parse_link_down_ind(p_msg, buf_len, api_id);
				break;
			}

		case X2AP_ENB_CONFIG_UPDATE_RES:
			{
				p_parsed_msg = x2_parse_enb_config_update_res(p_msg, buf_len, api_id);
				break;
			}

		case X2AP_ENB_CONFIG_UPDATE_WAIT_IND:
			{
				p_parsed_msg = x2_parse_enb_config_update_wait_ind(p_msg, buf_len, api_id);
				break;
			}

		case X2AP_ENB_CONFIG_UPDATE_IND:
			{
				p_parsed_msg = x2_parse_enb_config_update_ind(p_msg, buf_len, api_id);
				break;
			}

		case X2AP_RESET_RES:
			{
				p_parsed_msg = x2_parse_reset_res(p_msg, buf_len, api_id);
				break;
			}
		case X2AP_CELL_ACTIVATION_REQ:
			{
				p_parsed_msg = x2_parse_cell_activation_req(p_msg,buf_len,api_id);
				break;
			}


#ifdef SON_MRO_FEATURE
		case X2AP_HO_REPORT_IND:
			{
				p_parsed_msg = x2_parse_ho_report_ind(p_msg, buf_len, api_id);
				break;
			}

		case X2AP_MOBILITY_SETTING_CHANGE_REQ:
			{
				p_parsed_msg = x2_parse_mobility_setting_change_req(p_msg, buf_len, api_id);
				break;
			}

		case X2AP_MOBILITY_CHANGE_RESP:
			{
				p_parsed_msg = x2_parse_mobility_change_resp(p_msg, buf_len, api_id);
				break;
			}
#endif /* ifdef SON_MRO_FEATURE */

      /* SPR_19619 start */
      
    	case X2AP_PEER_CELL_ACTIVATION_RESP:
			{
				p_parsed_msg = x2_parse_peer_cell_activation_resp(p_msg, buf_len, api_id);
				break;
			}
      /* SPR_19619 stop */
		
		default:
			{
				SON_LOG(X2_GET_LOG_MODE,
						p_son_x2_facility_name, SON_ERROR,
						"x2_parse_rrc_msgs: Unexpected Message: [%u] "
						"received from RRC", api_id);
				break;
			}
	} /* switch */

	SON_UT_TRACE_EXIT();
	return p_parsed_msg;
} /* x2_parse_rrc_msgs */

/*****************************************************************************
 * Function Name  : son_x2_process_event
 * Inputs         : p_msg - Pointer to the message received
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This function is used to invoke the appropriate event
 *                  handler for the messages received by SON X2. It
 *                  calculates the API Id, finds current FSM state and based
 *                  on this access the son_x2_fsm_table to call the
 *                  appropriate handler.
 ****************************************************************************/
son_buf_retain_status_et
son_x2_process_event
(
 son_void_t *p_msg
 )
{
	son_buf_retain_status_et buf_retain_status = RELEASE_SON_BUFFER;
	son_x2_index_et idx = X2_IDX_NOT_AVAILABLE;
	STACKAPIHDR cspl_hdr = {0};
	son_u8 *p_parsed_msg = SON_PNULL;

	SON_UT_TRACE_ENTER();

	/* Parse the cspl header */
	son_parse_cspl_header(p_msg, &cspl_hdr);

	/* Call the appropriate message handler based on the API Id and
	 * source module id */
	idx = x2_calculate_msg_index(cspl_hdr.from, cspl_hdr.api);

	/* Validate the index of son_x2_fsm_table array*/
	if (X2_NUM_OF_EVENTS <= idx)
	{
		/* No need to print the log here for invalid API Id as it is
		 * already printed in x2_calculate_msg_index function */
		/* Do nothing */
	}
	else if (X2_NUM_OF_STATES <=
			g_x2_gb_ctxt.x2_global_fsm_state)
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_WARNING,
				"Message with API Id = [%u] received in "
				"invalid state = [%u]", cspl_hdr.api,
				g_x2_gb_ctxt.x2_global_fsm_state);
	}
	else
	{
		/* Move the pointer such that it points to the message data after the
		 * CSPL header */
		p_msg += CV_HDRSIZE;

		/* Parse the incoming message received from L3 X2AP */
		if (SON_RRC_MODULE_ID == cspl_hdr.from)
		{
			/* Parse message received from L3 X2AP */
			p_parsed_msg = x2_parse_rrc_msgs(p_msg,
					cspl_hdr.api);

			if (SON_PNULL != p_parsed_msg)
			{
				p_msg = p_parsed_msg;
			}
			else
			{
				SON_LOG(X2_GET_LOG_MODE,
						p_son_x2_facility_name, SON_DETAILED,
						"Parsing failed for received "
						"message: [%u] in state [%s]", cspl_hdr.api,
						SON_X2_FSM_STATES_NAMES[g_x2_gb_ctxt.x2_global_fsm_state]);

				SON_UT_TRACE_EXIT();
				return buf_retain_status;
			}
		}

		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_INFO,
				"### X2_FSM: Processing API %u in state %s ###", cspl_hdr.api,
				SON_X2_FSM_STATES_NAMES[g_x2_gb_ctxt.x2_global_fsm_state]);

		buf_retain_status =
			(*son_x2_fsm_table[g_x2_gb_ctxt.x2_global_fsm_state][idx])
			(p_msg, &cspl_hdr, idx);

		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_INFO,
				"### X2_FSM: Processing Completed ###");

		/* Free the memory taken for message received from L3 X2AP */
		if (SON_PNULL != p_parsed_msg)
		{
			son_mem_free(p_parsed_msg);
		}
	}

	SON_UT_TRACE_EXIT();
	return buf_retain_status;
} /* son_x2_process_event */
