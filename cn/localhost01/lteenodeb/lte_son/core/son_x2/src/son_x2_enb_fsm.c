/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_x2_enb_fsm.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains X2 eNB FSM functions
 *
 *******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Sep, 2012       Shilpi/Atul Initial         Initial
 *   July, 2014      Shilpi                      SPR 10331 Fix
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_x2_enb_fsm.h>
/* SPR Fix Start */                    
#include <son_anr_table.h>
#include <son_anr_enb_ctxt_mgr.h>
/* SPR Fix End */                    
#include <rrc_x2apCommon_composer.h>
#include <son_mlb_intf.h>
#include <son_x2_ctxt_mgr.h>

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern const son_8 *p_son_x2_facility_name;
extern const QWAIT  son_waitstruct;
extern const son_s8 *SON_X2_ENB_FSM_EVENT_NAMES[];
/* SPR Fix Start */                    
extern x2_global_context_t g_x2_gb_ctxt;
/* SPR Fix End */                    
#define x2ap_Cause_radioNetwork       1
#define x2ap_load_balancing           22
#define x2ap_value_out_of_allowed_range           24

/*SPR_17451_START*/
#define x2ap_unspecified              21
/*SPR_17451_END*/


/******************************************************************************
 * Function Name  : sonx2_enb_fsm_invalid_event_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on 
 *                                  the eNB FSM state)
 * Description    : Event Handler for logging invalid event it is invoked when
 *                  an unexpected event occurs..
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_invalid_event_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	SON_UT_TRACE_ENTER();
	SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_WARNING,
			"Unexpected "
			"%u->SONX2:%s in %s state for eNB 0x%x",
                /*SPR 17777 +-*/
			p_cspl_hdr->from,
			SON_X2_ENB_FSM_EVENT_NAMES[event_id],
			SON_X2_ENB_FSM_STATES_NAMES[p_enb_ctxt->enb_curr_fsm_state],
			son_convert_char_cell_id_to_int_cell_id(p_enb_ctxt->enb_id.enb_id));
			LTE_GCC_UNUSED_PARAM(p_msg);
                /*SPR 17777 +-*/

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_UNEXPECTED;
}

/******************************************************************************
 * Function Name  : sonx2_enb_fsm_add_enb_req_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for new eNB connection Add Request from ANR.
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_add_enb_req_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	SON_UT_TRACE_ENTER();
	SONX2_ENB_FSM_SET_STATE(p_enb_ctxt,
			p_enb_ctxt->enb_prev_fsm_state,
			p_enb_ctxt->enb_curr_fsm_state,
			X2_ENB_STATE_CONNECTING);

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_add_enb_req_handler */

/******************************************************************************
 * Function Name  : sonx2_enb_fsm_link_up_ind_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for Link Up event from L3
 *****************************************************************************/
static son_x2_result_et 
sonx2_enb_fsm_link_up_ind_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	SON_UT_TRACE_ENTER();
	SONX2_ENB_FSM_SET_STATE(p_enb_ctxt,
			p_enb_ctxt->enb_prev_fsm_state,
			p_enb_ctxt->enb_curr_fsm_state,
			X2_ENB_STATE_CONNECTED);

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED; 
} /* sonx2_enb_fsm_link_up_ind_handler */

/******************************************************************************
 * Function Name  : sonx2_enb_fsm_add_enb_res_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_result        Pointer to the result received 
 *                                  from L3 X2AP               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for eNB connection Add Response from L3
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_add_enb_res_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_result
 )
{
	son_x2_oam_link_down_ind_t oam_link_down_ind; 


	SON_UT_TRACE_ENTER();
	SON_MEMSET(&oam_link_down_ind, 0, sizeof(oam_link_down_ind));

	if (SON_SUCCESS == (*(son_return_et *)p_result))
	{
		/* New State will be CONNECTING but it is already in CONENCTING 
		 *  state, earlier waiting for ADD_ENB response
		 * and now it shall be waiting for Link UP indication so nothing needs
		 * to be done here */
	}
	else
	{
		SONX2_ENB_FSM_SET_STATE(p_enb_ctxt,
				p_enb_ctxt->enb_prev_fsm_state,
				p_enb_ctxt->enb_curr_fsm_state,
				X2_ENB_STATE_DISCONNECTED);

		/* Send link down indication to OAM */
		SON_MEMCPY(&oam_link_down_ind.nbr_enb_id,
				&p_enb_ctxt->enb_id,
				sizeof(son_global_enb_id_t));

		son_create_send_buffer((son_u8 *)&oam_link_down_ind,
				SON_X2_MODULE_ID, SON_MIF_MODULE_ID,
				SONX2_LINK_DOWN_IND, sizeof(son_x2_oam_link_down_ind_t));
	}

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_add_enb_res_handler */

/******************************************************************************
 * Function Name  : sonx2_enb_fsm_link_down_ind_handler
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for Link Down indication received from L3
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_link_down_ind_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	SON_UT_TRACE_ENTER();
	/*SPR_16835_START*/
	if(X2_ENB_STATE_DISCONNECTING == p_enb_ctxt->enb_curr_fsm_state &&
			p_enb_ctxt->txn_id_for_del_req  < SON_INVALID_TXN_ID &&
			SON_FALSE == p_enb_ctxt->is_self_del_enb_req_triggered)
	{
		son_x2_delete_enb_res_t delete_res = {0};
		delete_res.result = SON_SUCCESS;
		delete_res.transaction_id = p_enb_ctxt->txn_id_for_del_req;
		delete_res.p_enb_data_list = p_enb_ctxt;
		p_enb_ctxt->txn_id_for_del_req = SON_INVALID_TXN_ID;
		son_create_send_buffer((son_u8 *)&delete_res,
				SON_X2_MODULE_ID, SON_ANR_MODULE_ID,
				SONX2_DELETE_ENB_RES,
				sizeof(son_x2_delete_enb_res_t));
	}
	/*SPR_16835_END*/

	SONX2_ENB_FSM_SET_STATE(p_enb_ctxt,
			p_enb_ctxt->enb_prev_fsm_state,
			p_enb_ctxt->enb_curr_fsm_state,
			X2_ENB_STATE_DISCONNECTED);
        /*SPR_16835_START*/
        x2_clear_ecu_gu_group_id_data(&p_enb_ctxt->
                pending_gu_group_id_data);
        x2_clear_ecu_gu_group_id_data(&p_enb_ctxt->
                sent_gu_group_id_data);
	x2_delete_enb_config_cell_list(&p_enb_ctxt->
		enb_config_cell_info);
	p_enb_ctxt->time_d_wait_expiry.s = 0;
	p_enb_ctxt->time_d_wait_expiry.us = 0;
        p_enb_ctxt->is_enb_config_update_res_awaited =  SON_FALSE;
	p_enb_ctxt->is_self_del_enb_req_triggered    =  SON_FALSE;
        /*SPR_16835_END*/

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_link_down_ind_handler */

/*SPR_17670_START*/
/******************************************************************************
 * Function Name  : sonx2_enb_fsm_delete_enb_req_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for eNB Conenction Delete Request received 
 *                  from ANR
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_delete_enb_req_handler
(
    son_x2_enb_index_et         event_id,
    STACKAPIHDR                 *p_cspl_hdr,
    son_nbr_enb_hash_rec_t      *p_enb_ctxt,
    son_u8                      *p_msg
)
{
    son_rrc_delete_enb_req_t rrc_del_enb_req = {0};
    son_x2_delete_enb_req_t *p_anr_del_enb_req = 
        (son_x2_delete_enb_req_t *)p_msg;


    SON_UT_TRACE_ENTER();

    /* Send delete eNB requests to L3 X2AP */
    rrc_del_enb_req.transaction_id = p_anr_del_enb_req->transaction_id; 
    x2_convert_son_enbid_to_l3_enbid(&p_enb_ctxt->enb_id,
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
                "delete eNB request could not be sent to RRC for "
                "eNB 0x%x", son_convert_char_cell_id_to_int_cell_id(p_enb_ctxt->enb_id.enb_id));
    }
    else 
    {
	/*SPR_16835_START*/
	p_enb_ctxt->txn_id_for_del_req = rrc_del_enb_req.transaction_id; 	
	/*SPR_16835_END*/	
    }

    SONX2_ENB_FSM_SET_STATE(p_enb_ctxt,
                p_enb_ctxt->enb_prev_fsm_state,
                p_enb_ctxt->enb_curr_fsm_state,
                X2_ENB_STATE_DISCONNECTING);

    SON_UT_TRACE_EXIT();
    return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_delete_enb_req_handler */
/*SPR_17670_END*/

/******************************************************************************
 * Function Name  : sonx2_enb_fsm_resetting_state_delete_enb_req_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for eNB Conenction Delete Request received 
 *                  from ANR when eNB Connection's state is RESETTING 
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_resetting_state_delete_enb_req_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	son_x2_delete_enb_req_t *p_anr_del_enb_req = 
		(son_x2_delete_enb_req_t *)p_msg;
	son_rrc_delete_enb_req_t rrc_del_enb_req = {0};


	SON_UT_TRACE_ENTER();

	SONX2_ENB_FSM_SET_STATE(p_enb_ctxt,
			p_enb_ctxt->enb_prev_fsm_state,
			p_enb_ctxt->enb_curr_fsm_state,
			X2_ENB_STATE_DISCONNECTING);

	/* Send delete eNB requests to L3 X2AP */
	rrc_del_enb_req.transaction_id = p_anr_del_enb_req->transaction_id; 
	x2_convert_son_enbid_to_l3_enbid(&p_enb_ctxt->enb_id,
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
				"[eNB Id: 0x%x]", son_convert_char_cell_id_to_int_cell_id(p_enb_ctxt->enb_id.enb_id));
	}

	/* SPR-11340 Fix Starts */
	/* Lines Deleted */
	/* SPR-11340 Fix Ends */

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_resetting_state_delete_enb_req_handler */

/******************************************************************************
 * Function Name  : sonx2_enb_fsm_delete_enb_res_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for eNB Conenction Delete Response received 
 *                  from L3
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_delete_enb_res_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	son_rrc_delete_enb_res_t *p_res = 
		(son_rrc_delete_enb_res_t *)p_msg;
	son_x2_delete_enb_res_t delete_res = {0};


	SON_UT_TRACE_ENTER();
	/* If Success is received from L3 X2AP */
	if (1 == p_res->del_enb_res.response)
	{
		delete_res.result = SON_SUCCESS;
		SONX2_ENB_FSM_SET_STATE(p_enb_ctxt,
				p_enb_ctxt->enb_prev_fsm_state,
				p_enb_ctxt->enb_curr_fsm_state,
				X2_ENB_STATE_DISCONNECTED);
		/* SPR-10331 Fix Starts */
		x2_clear_ecu_gu_group_id_data(&p_enb_ctxt->
				pending_gu_group_id_data);
		x2_clear_ecu_gu_group_id_data(&p_enb_ctxt->
				sent_gu_group_id_data);
		/* SPR-10331 Fix Ends */
		if (SON_TRUE ==  p_enb_ctxt->is_self_del_enb_req_triggered)
		{
			son_x2_oam_link_down_ind_t oam_link_down_ind;

			SON_MEMSET(&oam_link_down_ind, 0, sizeof(son_x2_oam_link_down_ind_t));
			/* Send link down indication to OAM */
			SON_MEMCPY(&oam_link_down_ind.nbr_enb_id,
					&p_enb_ctxt->enb_id,
					sizeof(son_global_enb_id_t));

			son_create_send_buffer((son_u8 *)&oam_link_down_ind,
					SON_X2_MODULE_ID, SON_MIF_MODULE_ID,
					SONX2_LINK_DOWN_IND,
					sizeof(son_x2_oam_link_down_ind_t));
		}
	}
	else if (0 == p_res->del_enb_res.response)
	{
		delete_res.result = SON_FAILURE;
		/* In case delete eNB request was triggered from Resetting state
		 * i.e. self triggered delete eNB request and now delete eNB
		 * response is received with failure, wait
		 * for the Reset response(request was sent when resetting state
		 * was set). */
		if (SON_TRUE ==  p_enb_ctxt->is_self_del_enb_req_triggered)
		{
			/* SPR 11340 Fix Starts */
			/* Lines deleted */
			/* Reset response has not been received till now otherwise 
			   the state would not have been disconnecting */
			/* SPR 11340 Fix Ends */
			SONX2_ENB_FSM_SET_STATE(p_enb_ctxt,
					p_enb_ctxt->enb_prev_fsm_state,
					p_enb_ctxt->enb_curr_fsm_state,
					X2_ENB_STATE_RESETTING);
		}
		else 
		{
			SONX2_ENB_FSM_SET_STATE(p_enb_ctxt,
					p_enb_ctxt->enb_prev_fsm_state,
					p_enb_ctxt->enb_curr_fsm_state,
					X2_ENB_STATE_CONNECTED);
			/* SPR-10331 Fix Starts */
			if (!(SON_TRUE == p_enb_ctxt->is_enb_config_update_res_awaited ||
						p_enb_ctxt->time_d_wait_expiry.s ||
						p_enb_ctxt->time_d_wait_expiry.us))
			{
				/* If pending data is found to be send in eNB config 
				   update req */
				if (list_count(&p_enb_ctxt->enb_config_cell_info) ||
						list_count(&p_enb_ctxt->pending_gu_group_id_data.
							add_gu_id_list) ||
						list_count(&p_enb_ctxt->pending_gu_group_id_data.
							delete_gu_id_list))
				{
					/* Create eNB config request with the pending info */
					x2_create_send_enb_config_update_req_msg(
							&p_enb_ctxt->enb_config_cell_info,
							p_enb_ctxt);
					x2_print_pending_enb_config_update_req_data(
							&p_enb_ctxt->enb_config_cell_info);
					x2_print_pending_gu_group_id_data(p_enb_ctxt);
					x2_print_sent_gu_group_id_data(p_enb_ctxt);
				}
			}
			/* SPR-10331 Fix Starts */
		}
	}

	if (SON_FALSE ==  p_enb_ctxt->is_self_del_enb_req_triggered)
	{
		/* Send delete eNB response to ANR */
		delete_res.transaction_id = p_res->transaction_id;
		delete_res.p_enb_data_list = p_enb_ctxt;
		son_create_send_buffer((son_u8 *)&delete_res,
				SON_X2_MODULE_ID, SON_ANR_MODULE_ID,
				SONX2_DELETE_ENB_RES,
				sizeof(son_x2_delete_enb_res_t));
	}
	/*SPR_16835_START*/
	p_enb_ctxt->is_self_del_enb_req_triggered = SON_FALSE;
	p_enb_ctxt->txn_id_for_del_req = SON_INVALID_TXN_ID; 
	/*SPR_16835_END*/

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_delete_enb_res_handler */

/******************************************************************************
 * Function Name  : x2_create_send_es_triggerd_enb_config_update_req 
 * Inputs         : p_req           Pointer to the switch on/off indication 
 *                                  message received from SON ES
 *                : p_enb_rec       Pointer to the peer eNB record to which eNB
 *                                  config update request message is to be sent.
 * Outputs        : None
 * Returns        : None
 * Description    : This function creates and send eNB config update request 
 *                  message when cell switch on/off indication is received 
 *                  from SON ES.
 *****************************************************************************/
son_void_t
x2_create_send_es_triggerd_enb_config_update_req
(
 son_x2_cell_switch_on_off_ind_t *p_req,
 son_nbr_enb_hash_rec_t          *p_enb_rec
 )
{
	son_rrc_enb_config_update_req_t *p_update_req = SON_PNULL;
	son_u16 index = 0;
	/* SPR 11340 Fix Starts */
	/* Line deleted */
	/* SPR 11340 Fix Ends */
	son_size_t update_req_size = sizeof(son_rrc_enb_config_update_req_t);


	SON_UT_TRACE_ENTER();

	p_update_req = (son_rrc_enb_config_update_req_t *)
		son_mem_get(update_req_size);

	if (SON_PNULL == p_update_req)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
				"Memory allocation failed for p_update_req. Can't not send eNB Config Update"
				" request message to eNB 0x%x",
				son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.enb_id));
	}
	else
	{
		SON_MEMSET(p_update_req, 0, update_req_size);
		list_count(&p_enb_rec->enb_config_cell_info);
		/* Create eNB config update request message */
		p_update_req->transaction_id = son_generate_txn_id();
		p_update_req->enb_config_update_req.num_peer_enb = 1;
		x2_convert_son_enbid_to_l3_enbid(&p_enb_rec->enb_id, 
				&p_update_req->enb_config_update_req.enb_info[0]);

		for (; index < p_req->num_srv_cells_list_size && 
				index < MAX_SERVED_CELLS; index++) 
		{
			p_update_req->enb_config_update_req.bitmask |= 
				X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT;
			x2_populate_cells_to_modify_list_info(
					&p_req->cell_switch_on_off_data_list[index].src_cgi,
					p_req->cell_switch_on_off_data_list[index].
					deactivation_indication,
					&p_update_req->enb_config_update_req.mod_served_cell.
					cell_list[p_update_req->enb_config_update_req.
					mod_served_cell.num_served_cell]);
			p_update_req->enb_config_update_req.mod_served_cell.
				num_served_cell++;
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
					"x2_create_send_es_triggerd_enb_config_update_req: "
					"eNB config update request could not be sent to RRC for"
					" enb [eNB Id: 0x%x]", 
					son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.enb_id));
		}
		else
		{
			/* SPR 11340 Fix Starts */
			/* Lines deleted */
			p_enb_rec->is_enb_config_update_res_awaited = SON_TRUE;
			/* SPR 11340 Fix Ends */
		}

		/* Free the memory allocated on heap for the update request */
		son_mem_free(p_update_req);
	}

	SON_UT_TRACE_EXIT();
} /* x2_create_send_es_triggerd_enb_config_update_req */

/* SPR_19619 start */

/******************************************************************************
 * Function Name  : x2_create_send_es_peer_cell_activation_req 
 * Inputs         : p_req           Pointer to the switch on/off indication 
 *                                  message received from SON ES
 *                : p_enb_rec       Pointer to the peer eNB record to which eNB
 *                                  config update request message is to be sent.
 * Outputs        : None
 * Returns        : None
 * Description    : This function creates and send eNB peer cell activation request 
 *                  message when it is received 
 *                  from SON ES.
 *****************************************************************************/
son_void_t
x2_create_send_es_peer_cell_activation_req
(
 son_x2_peer_cell_activation_req_t  *p_req,
 son_nbr_enb_hash_rec_t             *p_enb_rec
 )
{
    son_rrc_peer_cell_activation_req_t  rrc_cell_activation_req ;
    son_x2_peer_cell_activation_req_t *p_cell_activation_req =  (son_x2_peer_cell_activation_req_t *)p_req ;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&rrc_cell_activation_req, 0, sizeof(son_rrc_peer_cell_activation_req_t));

    rrc_cell_activation_req.transaction_id = p_cell_activation_req->transaction_id;
    rrc_cell_activation_req.activation_req.peer_cell_activation_info.num_served_cell = p_cell_activation_req->peer_cell_activation_req.num_served_cell;


    x2_convert_son_ecgi_to_l3_ecgi(&p_cell_activation_req->peer_cell_activation_req.served_cell_list[0],
            &rrc_cell_activation_req.activation_req.peer_cell_activation_info.served_cell_list[0]);

    x2_convert_son_enbid_to_l3_enbid(&p_enb_rec->enb_id,&rrc_cell_activation_req.activation_req.gb_enb_id);

    /* Call pup tool to send update req to RRC */
    if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_peer_cell_activation_req(
                &rrc_cell_activation_req.activation_req,
                SON_MODULE_ID,
                SON_RRC_MODULE_ID,
                rrc_cell_activation_req.transaction_id,
                SON_NULL))
    {
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
                SON_ERROR,
                "Peer Cell Activation request could not be sent to RRC for"
                " eNB 0x%x", 
                son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.enb_id));
    }

    SON_UT_TRACE_EXIT();
} /* x2_create_send_es_peer_cell_activation_req */

/* SPR_19619 stop */

/* SPR Fix Start */

/******************************************************************************
 * Function Name  : x2_populate_temp_nbr_ecgi_enb_id_map 
 * Inputs         : p_src_cgi   Pointer to the source cell ECGI
 * Outputs        : None
 * Returns        : None
 * Description    : This function populates the nbr_ecgi_enb_id_map with 
 *                  all the neighbor cells ECGI and eNB IDs to send in eNB 
 *                  Config update request message to X2AP. This map is 
 *                  populated for the all the neighbors of all the serving cells
 *                  which needs to be informed through eNB config update 
 *                  request message and ensures that neighbors are not \
 *                  duplicate in the message.
 *****************************************************************************/
son_void_t
x2_populate_temp_nbr_ecgi_enb_id_map
(
 son_intra_rat_global_cell_id_t      *p_src_cgi
 )
{
	anr_cell_context_t      *p_cell_ctxt        = SON_PNULL;
	anr_intra_rat_hash_nr_tuple_t *p_intra_tpl = SON_PNULL;
	son_nbr_ecgi_enb_id_hash_rec_t hash_rec;
	son_size_t hash_rec_len = sizeof(son_nbr_ecgi_enb_id_hash_rec_t);


	SON_UT_TRACE_ENTER();

	/* Search for the given cell Id in the cell contexts 
	 *  maintained by ANR */
	p_cell_ctxt = anr_get_cell_context_from_global_context(p_src_cgi);

	if (SON_PNULL != p_cell_ctxt) 
	{
		p_intra_tpl = anr_get_first_nr_from_intra_rat_hash_table(
				&p_cell_ctxt->intra_rat_neighbor_table);

		while (SON_PNULL != p_intra_tpl)
		{
			/* Populate the neighbor info in tempory map nbr_ecgi_enb_id_map 
			   after validation */
			if (SON_TRUE == anr_validate_eutran_neighbor_for_l3(
						&p_intra_tpl->data))
			{

				SON_MEMSET(&hash_rec, 0, hash_rec_len);
				SON_MEMCPY(&hash_rec.nbr_cell_id, 
						&p_intra_tpl->data.neighbor_info.nbr_cgi,
						sizeof(son_intra_rat_global_cell_id_t));
				anr_determine_enb_id(&p_intra_tpl->data, &hash_rec.nbr_enb_id);
				x2_insert_entry_in_nbr_ecgi_enb_id_map(
						&g_x2_gb_ctxt.nbr_ecgi_enb_id_map, 
						&hash_rec);
			}
			p_intra_tpl =  anr_get_next_nr_from_intra_rat_hash_table(
					&p_cell_ctxt->intra_rat_neighbor_table, 
					&p_intra_tpl->anchor);
		}
	}

	SON_UT_TRACE_EXIT();
} /* x2_populate_temp_nbr_ecgi_enb_id_map */

/******************************************************************************
 * Function Name  : x2_populate_neigh_ecgi_enb_id_map 
 * Inputs         : None
 * Outputs        : Pointer to the strucuting storing neighbor cells ECGI and 
 *                  eNB Ids
 * Returns        : None
 * Description    : This function populates the neighbor cells ECGI and eNB IDs 
 *                  in the enb config update request message to be send to X2AP.
 *****************************************************************************/
son_void_t
x2_populate_neigh_ecgi_enb_id_map
(
 x2ap_neigh_ecgi_enb_id_map_list_t   *p_neigh_ecgi_enb_id_map_list
 )
{
	son_nbr_ecgi_enb_id_hash_tuple_t *p_intra_tpl = SON_PNULL;


	SON_UT_TRACE_ENTER();

	p_intra_tpl = x2_get_first_nbr_info_from_nbr_ecgi_enb_id_map(
			&g_x2_gb_ctxt.nbr_ecgi_enb_id_map);
	while (SON_PNULL != p_intra_tpl && 
			MAX_NEIGHBOURS > p_neigh_ecgi_enb_id_map_list->neigh_info_count)
	{
		x2_convert_son_ecgi_to_l3_ecgi(
				&p_intra_tpl->data.nbr_cell_id, 
				&p_neigh_ecgi_enb_id_map_list->
				neigh_ecgi_enb_id_info[
				p_neigh_ecgi_enb_id_map_list->neigh_info_count].ecgi);
		x2_convert_son_enbid_to_l3_enbid(
				&p_intra_tpl->data.nbr_enb_id, 
				&p_neigh_ecgi_enb_id_map_list->
				neigh_ecgi_enb_id_info[
				p_neigh_ecgi_enb_id_map_list->neigh_info_count].enb_id);
		p_neigh_ecgi_enb_id_map_list->neigh_info_count++;
		p_intra_tpl =  x2_get_next_nbr_info_from_nbr_ecgi_enb_id_map(
				&g_x2_gb_ctxt.nbr_ecgi_enb_id_map, &p_intra_tpl->anchor);
	}

	x2_clear_nbr_ecgi_enb_id_map(&g_x2_gb_ctxt.nbr_ecgi_enb_id_map); 
	SON_UT_TRACE_EXIT();
} /* x2_populate_neigh_ecgi_enb_id_map */

/* SPR Fix End */

/******************************************************************************
 * Function Name  : x2_create_send_anr_triggerd_enb_config_update_req 
 * Inputs         : p_req           Pointer to the cell or its NRT update 
 *                                  indication message received from SON ANR
 *                : p_enb_rec       Pointer to the peer eNB record to which eNB
 *                                  config update request message is to be sent.
 * Outputs        : None
 * Returns        : None
 * Description    : This function creates and send eNB config update request 
 *                  message when serving cell's or its neighbor information 
 *                  change indication is received from SON ANR.
 *****************************************************************************/
son_void_t
x2_create_send_anr_triggerd_enb_config_update_req
(
 son_x2_nrt_update_ind_t *p_req,
 son_nbr_enb_hash_rec_t  *p_enb_rec
 )
{
	son_rrc_enb_config_update_req_t *p_update_req = SON_PNULL;
	x2ap_enb_config_update_req_t *p_enb_config_update_req = SON_PNULL;
	son_u16 index = 0;
	/* SPR 11340 Fix Starts */
	/* Line deleted */
	/* SPR 11340 Fix Ends */
	son_size_t update_req_size = sizeof(son_rrc_enb_config_update_req_t);


	SON_UT_TRACE_ENTER();

	p_update_req = (son_rrc_enb_config_update_req_t *)
		son_mem_get(update_req_size);

	if (SON_PNULL == p_update_req)
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
				"Memory allocation failed for p_update_req. Can't not send eNB Config Update"
				" request message to eNB 0x%x",
				son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.enb_id));
	}
	else
	{
		SON_MEMSET(p_update_req, 0, update_req_size);
		p_enb_config_update_req = &p_update_req->enb_config_update_req;

		/* Create eNB config update request message */
		p_update_req->transaction_id = son_generate_txn_id();
		p_enb_config_update_req->num_peer_enb = 1;
		x2_convert_son_enbid_to_l3_enbid(&p_enb_rec->enb_id, 
				&p_enb_config_update_req->enb_info[0]);
		for (; index < p_req->num_srv_cells_list_size &&
				index < MAX_SERVED_CELLS; index++) 
		{
			if (SON_NRT_OP_UPDATE == p_req->srv_cell_updated_info_list[index].
					nrt_op)
			{
				p_enb_config_update_req->bitmask |= 
					X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT;
				x2_populate_cells_to_modify_list_info(
						&p_req->srv_cell_updated_info_list[index].src_cgi,
						X2_CELL_DEACTIVATION_FALSE,
						&p_enb_config_update_req->mod_served_cell.cell_list[
						p_enb_config_update_req->mod_served_cell.
						num_served_cell]);
				p_enb_config_update_req->mod_served_cell.num_served_cell++;
			}
			else if (SON_NRT_OP_ADD == p_req->srv_cell_updated_info_list[index].
					nrt_op)
			{
				p_enb_config_update_req->bitmask |= 
					X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT;
				x2_populate_srv_cell_info(
						&p_req->srv_cell_updated_info_list[index].src_cgi,
						&p_enb_config_update_req->add_served_cell.cell_list[
						p_enb_config_update_req->add_served_cell.
						num_served_cell]);
				p_enb_config_update_req->add_served_cell.num_served_cell++;
			}
			else if (SON_NRT_OP_DELETE == p_req->srv_cell_updated_info_list[
					index].nrt_op)
			{
				p_enb_config_update_req->bitmask |= 
					X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT;
				x2_populate_cell_id_for_l3(
						p_req->srv_cell_updated_info_list[index].src_cgi.
						cell_identity,
						&(p_enb_config_update_req->del_served_cell.cell_id_list[
							p_enb_config_update_req->del_served_cell.
							num_served_cell]));
				p_enb_config_update_req->del_served_cell.num_served_cell++;
			}

			/* SPR Fix Start */            
			x2_populate_temp_nbr_ecgi_enb_id_map(
					&p_req->srv_cell_updated_info_list[index].src_cgi);
			/* SPR Fix End */                    
		}

		/* SPR Fix Start */            
		x2_populate_neigh_ecgi_enb_id_map(
				&p_enb_config_update_req->neigh_ecgi_enb_id_map);
		if (0 < p_enb_config_update_req->neigh_ecgi_enb_id_map.neigh_info_count)
		{
			p_enb_config_update_req->bitmask |= 
				X2AP_NEIGH_ECGI_ENB_ID_MAPPING_PRESENT;
		}

		/* SPR Fix End */                    

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
					"eNB config update request could not be sent to RRC "
					"for enb [eNB Id: 0x%x]", 
					son_convert_char_cell_id_to_int_cell_id(p_enb_rec->enb_id.enb_id));
		}
		else
		{
			/* SPR 11340 Fix Starts */
			/* Lines deleted */
			p_enb_rec->is_enb_config_update_res_awaited = SON_TRUE;
			/* SPR 11340 Fix Ends */
		}

		/* Free the memory allocated on heap for the update request */
		son_mem_free(p_update_req);
	}

	SON_UT_TRACE_EXIT();
} /* x2_create_send_anr_triggerd_enb_config_update_req */

/******************************************************************************
 * Function Name  : x2_add_enb_config_req_data_in_enb_config_cell_list 
 * Inputs         : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : None
 * Description    : Function to add the received eNB config update request data
 *                  to the pending eNB config cell list since eNB config update
 *                  req can't be triggered either due to already triggered
 *                  eNB config update request or due to time d wait or 
 *                  reset timer running.
 *****************************************************************************/
static son_void_t
x2_add_enb_config_req_data_in_enb_config_cell_list
(
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	son_u16 index = 0;
	/* SPR-10331 Fix Starts */
	x2ap_gu_group_id_t x2_gu_grp_id;
	/* SPR-10331 Fix Ends */


	SON_UT_TRACE_ENTER();
	if (SON_ES_MODULE_ID == p_cspl_hdr->from)
	{
		son_x2_cell_switch_on_off_ind_t *p_ind =
			(son_x2_cell_switch_on_off_ind_t *)p_msg;

		for (index = 0; 
				index < p_ind->num_srv_cells_list_size; 
				index++)
		{
			x2_insert_cell_in_enb_cfg(p_enb_ctxt,
					&p_ind->cell_switch_on_off_data_list[index].src_cgi,
					SON_NRT_OP_UPDATE,
					p_ind->cell_switch_on_off_data_list[index].
					deactivation_indication);
			x2_print_pending_enb_config_update_req_data(&p_enb_ctxt->enb_config_cell_info);
		}
	}
	else if (SON_ANR_MODULE_ID == p_cspl_hdr->from)
	{
		son_x2_nrt_update_ind_t *p_ind = 
			(son_x2_nrt_update_ind_t *)p_msg;
		for (index = 0; 
				index < p_ind->num_srv_cells_list_size; 
				index++)
		{
			x2_insert_cell_in_enb_cfg(p_enb_ctxt,
					&p_ind->srv_cell_updated_info_list[index].src_cgi,
					p_ind->srv_cell_updated_info_list[index].nrt_op,
					X2_CELL_DEACTIVATION_IND_NULL);
			x2_print_pending_enb_config_update_req_data(&p_enb_ctxt->enb_config_cell_info);
		}
	}
	/* SPR-10331 Fix Starts */
	else if (SON_MIF_MODULE_ID == p_cspl_hdr->from)
	{
		son_oam_anr_gu_group_id_config_req_t *p_req = 
			(son_oam_anr_gu_group_id_config_req_t *)p_msg;
		if (SON_OAM_GU_GROUP_ID_TO_ADD_LIST_PRESENT & p_req->bitmask)
		{
			for (index = 0;
					index < p_req->gu_group_id_to_add_list.num_gu_group_id;
					index++)
			{
				SON_MEMSET(&x2_gu_grp_id, 0, sizeof(x2ap_gu_group_id_t));
				x2_convert_son_gu_group_id_to_l3_gu_group_id(
						&p_req->gu_group_id_to_add_list.gu_id_list[index], 
						&x2_gu_grp_id);
				/* If node is successfully deleted then it means:
				   Node was stored in pending delete list and now
				   it has been added again so nothing needs to be kept in 
				   pending for this GU Group Id as deletion and later addition 
				   has no impact on GU Group Id list.
				   In case it is not found in pending delete list then add it 
				   in pending add list */
				if (SON_FAILURE == 
						x2_delete_gu_grp_id_from_list(&p_enb_ctxt->
							pending_gu_group_id_data.delete_gu_id_list,
							&x2_gu_grp_id))
				{
					/* Node is not found in pending delete list so add it in 
					   pending add list */
					x2_insert_gu_grp_id_in_list(&p_enb_ctxt->
							pending_gu_group_id_data.add_gu_id_list, 
							&x2_gu_grp_id);
				}
			}
		}
		if (SON_OAM_GU_GROUP_ID_TO_DELETE_LIST_PRESENT & p_req->bitmask)
		{
			for (index = 0;
					index < p_req->gu_group_id_to_delete_list.num_gu_group_id;
					index++)
			{
				SON_MEMSET(&x2_gu_grp_id, 0, sizeof(x2ap_gu_group_id_t));
				x2_convert_son_gu_group_id_to_l3_gu_group_id(
						&p_req->gu_group_id_to_delete_list.gu_id_list[index], 
						&x2_gu_grp_id);
				/* If node is successfully deleted then it means:
				   Node was stored in pending add list and now
				   it has been deleted so nothing needs to be kept in 
				   pending for this GU Group Id as addition and later deletion 
				   has no impact on GU Group Id list.
				   In case it is not found in pending add list then add it 
				   from pending delete list */
				if (SON_FAILURE == 
						x2_delete_gu_grp_id_from_list(&p_enb_ctxt->
							pending_gu_group_id_data.add_gu_id_list,
							&x2_gu_grp_id))
				{
					/* Node is not found in pending add list so add it in 
					   pending delete list */
					x2_insert_gu_grp_id_in_list(&p_enb_ctxt->
							pending_gu_group_id_data.delete_gu_id_list, 
							&x2_gu_grp_id);
				}
			}
		}
		x2_print_pending_gu_group_id_data(p_enb_ctxt);
	}
	/* SPR-10331 Fix Ends */

	SON_UT_TRACE_EXIT();
} /* x2_add_enb_config_req_data_in_enb_config_cell_list */

/******************************************************************************
 * Function Name  : sonx2_enb_fsm_connecetd_state_enb_config_update_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for eNB Config Update request received 
 *                  from ES/ANR when eNB Conenction's state is CONNECETD.
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_connecetd_state_enb_config_update_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	SON_UT_TRACE_ENTER();

	/* eNB Config Update message shall not change the current state of 
	 * eNB Connection FSM. */

	/* Add the request to the pending requests:
	 *  1. If there is one eNB config update response awaited (indicated by
	 *     is_enb_config_update_res_awaited flag set to TRUE)
	 *  2. Peer has configured time_d_wait expiry value
	 *  3. Reset response is awaited */
	/* SPR 11340 Fix Starts */
	if (SON_TRUE == p_enb_ctxt->is_enb_config_update_res_awaited ||
			p_enb_ctxt->time_d_wait_expiry.s ||
			p_enb_ctxt->time_d_wait_expiry.us ||
			X2_ENB_STATE_RESETTING == p_enb_ctxt->enb_curr_fsm_state)
		/* SPR 11340 Fix Ends */
	{
		x2_add_enb_config_req_data_in_enb_config_cell_list(
				p_cspl_hdr, p_enb_ctxt, p_msg);

		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
				"Request added to the pending eNB Config Update requests "
				"either due to already triggered request or due to timed "
				"wait or reset timer running.");
	}
	/* Create eNB config update request */
	else if (SON_ES_MODULE_ID == p_cspl_hdr->from)
	{
		x2_create_send_es_triggerd_enb_config_update_req(
				(son_x2_cell_switch_on_off_ind_t *)p_msg, p_enb_ctxt);
	}
	else if (SON_ANR_MODULE_ID == p_cspl_hdr->from)
	{
		x2_create_send_anr_triggerd_enb_config_update_req(
				(son_x2_nrt_update_ind_t *)p_msg, p_enb_ctxt);
	}

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_connecetd_state_enb_config_update_handler */

/******************************************************************************
 * Function Name  : sonx2_enb_fsm_resetting_state_enb_config_update_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for eNB Config Update request received 
 *                  from ES/ANR when eNB Conenction's state is RESETTING.
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_resetting_state_enb_config_update_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	SON_UT_TRACE_ENTER();

	/* eNB Config Update message shall not change the current state of
	 * eNB Connection FSM. */

	/* Insert entry in pending list */
	SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
			"Request added to the pending eNB Config Update requests "
			"since X2 connection reset is in progress.");
	x2_add_enb_config_req_data_in_enb_config_cell_list(
			p_cspl_hdr, p_enb_ctxt, p_msg);

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_resetting_state_enb_config_update_handler */

/* SPR-10331 Fix Starts */
/******************************************************************************
 * Function Name  : sonx2_enb_fsm_resetting_state_gu_grp_id_config_req_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for GU Group Id config request received 
 *                  from SMIF when eNB Conenction's state is RESETTING.
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_resetting_state_gu_grp_id_config_req_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	SON_UT_TRACE_ENTER();
	/* This message shall not change the current state of eNB Connection FSM. */

	/* Insert entry in pending list */
	SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
			"Request added to the pending eNB Config Update requests "
			"since X2 connection reset is in progress.");
	x2_add_enb_config_req_data_in_enb_config_cell_list(
			p_cspl_hdr, p_enb_ctxt, p_msg);

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_resetting_state_gu_grp_id_config_req_handler */
/* SPR-10331 Fix Ends */

/******************************************************************************
 * Function Name  : sonx2_enb_fsm_enb_config_update_res_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for eNB Config Update response received 
 *                  from L3
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_enb_config_update_res_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	SON_UT_TRACE_ENTER();

	/* eNB Config Update Response shall not change the current state of
	 * eNB FSM. */

	/* SPR 11340 Fix Starts */
	/* Reset is_enb_config_update_res_awaited flag to FALSE as the awaited 
	   eNB config update response is received */
	p_enb_ctxt->is_enb_config_update_res_awaited = SON_FALSE;

	/* SPR 11340 Fix Ends */

	if (X2AP_ENB_CONFIG_UPDATE_RES == p_cspl_hdr->api)
	{
		son_rrc_enb_config_update_res_t *p_update_res =
			(son_rrc_enb_config_update_res_t *)p_msg;
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
				"X2_ENB_EVENT_RRC_ENB_CONFIG_UPDATE_RES "
				"Response eNB 0x%x "
				"Response Result %u",
				son_convert_char_cell_id_to_int_cell_id(p_enb_ctxt->enb_id.enb_id),
				(son_u16)p_update_res->enb_config_update_res.response);

		/* SPR 11340 Fix Starts */
		if (0 == p_update_res->enb_config_update_res.response)
		{
			son_rrc_reset_req_t reset_req = {0};
			/* Initate X2 reset request since eNB config update
			   response message is received with failure */
			reset_req.transaction_id = son_generate_txn_id();
			x2_convert_son_enbid_to_l3_enbid(
					&p_enb_ctxt->enb_id,
					&reset_req.reset_req.gb_enb_id);
			/*SPR_17451_START*/
			reset_req.reset_req.cause.type = x2ap_Cause_radioNetwork;
			reset_req.reset_req.cause.value = x2ap_unspecified;
			/*SPR_17451_END*/

			/* Cell pup tool to send reset req to L3 X2AP */
			if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_reset_req(
						&reset_req.reset_req,
						SON_MODULE_ID,
						SON_RRC_MODULE_ID,
						reset_req.transaction_id,
						SON_NULL))
			{
				SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
						SON_ERROR,
						"sonx2_enb_fsm_enb_config_update_res_handler: Reset "
						"request with transaction id = [%u] could not be sent "
						"to RRC", reset_req.transaction_id);
			}
			else
			{
				SONX2_ENB_FSM_SET_STATE(p_enb_ctxt,
						(p_enb_ctxt->enb_prev_fsm_state),
						(p_enb_ctxt->enb_curr_fsm_state),
						X2_ENB_STATE_RESETTING);
			}
		}
		else
		{
			/* SPR-10331 Fix Starts */
			x2_clear_ecu_gu_group_id_data(
					&p_enb_ctxt->sent_gu_group_id_data);
			/* If pending data is found to be send in eNB config update req */
			if (list_count(&p_enb_ctxt->enb_config_cell_info) ||
					list_count(&p_enb_ctxt->pending_gu_group_id_data.
						add_gu_id_list) ||
					list_count(&p_enb_ctxt->pending_gu_group_id_data.
						delete_gu_id_list))
				/* SPR-10331 Fix Ends */
			{
				/* Create eNB config request with the pending info */
				x2_create_send_enb_config_update_req_msg(
						&p_enb_ctxt->enb_config_cell_info,
						p_enb_ctxt);
				x2_print_pending_enb_config_update_req_data(
						&p_enb_ctxt->enb_config_cell_info);
				x2_print_pending_gu_group_id_data(p_enb_ctxt);
				x2_print_sent_gu_group_id_data(p_enb_ctxt);
			}
		}
		/* SPR 11340 Fix Ends */
	}
	else if (X2AP_ENB_CONFIG_UPDATE_WAIT_IND == p_cspl_hdr->api)
	{
		son_x2_enb_config_update_wait_ind *p_update_wait_ind =
			(son_x2_enb_config_update_wait_ind *)p_msg;

		/* Update the time_d_wait to the time when time_d_wait will 
		 *  expire i.e (current time + the value received) */
		if (0 != p_update_wait_ind->time_to_wait)
		{
			son_time_t current_time;
			/* Get current time */
			son_waitstruct.walltime(&current_time);
			p_enb_ctxt->time_d_wait_expiry.s = current_time.s +
				p_update_wait_ind->time_to_wait;
			SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
					"Time d wait expiry is set to current time + "
					"time_d_wait (current time: %lu sec and %lu usec, "
					"Time d wait: [%u] sec) in eNB [eNB Id: 0x%x] context",
					current_time.s,
					current_time.us,
					p_update_wait_ind->time_to_wait,
					son_convert_char_cell_id_to_int_cell_id(p_enb_ctxt->enb_id.enb_id));
		}
		else 
		{
			SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
					"X2_ENB_EVENT_RRC_ENB_CONFIG_UPDATE_WAIT_IND "
                /*SPR 17777 +-*/
					"received with time to wait value as 0");
			LTE_GCC_UNUSED_PARAM(event_id);
		}
	}

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_enb_config_update_res_handler */


/* SPR_19619 start */

/******************************************************************************
 * Function Name  : sonx2_enb_fsm_peer_cell_activation_req_handler 
 * Inputs         : p_req           Pointer to the switch on/off indication 
 *                                  message received from SON ES
 *                : p_enb_rec       Pointer to the peer eNB record to which eNB
 *                                  config update request message is to be sent.
 * Outputs        : None
 * Returns        : None
 * Description    : This function creates and send peer cell activation request 
 *                  message  to RRC when peer cell indication request is received 
 *                  from SON ES.
 *****************************************************************************/

static son_x2_result_et
sonx2_enb_fsm_peer_cell_activation_req_handler
(
    son_x2_enb_index_et         event_id,
    STACKAPIHDR                 *p_cspl_hdr,
    son_nbr_enb_hash_rec_t      *p_enb_ctxt,
    son_u8                      *p_msg
)

{
    SON_UT_TRACE_ENTER();

	/* Create Peer Cell Activation request */
		x2_create_send_es_peer_cell_activation_req(
				(son_x2_peer_cell_activation_req_t *)p_msg, p_enb_ctxt);

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_peer_cell_activation_req_handler */

/* SPR_19619 stop */

/******************************************************************************
 * Function Name  : sonx2_enb_fsm_disconnecting_state_reset_resp_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for Reset response received 
 *                  from L3 when eNB connection's state is DISCONNECTING
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_disconnecting_state_reset_resp_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	SON_UT_TRACE_ENTER();
	if (X2_ENB_STATE_RESETTING == p_enb_ctxt->enb_prev_fsm_state)
	{
		SONX2_FSM_SET_STATE(p_enb_ctxt->enb_prev_fsm_state, X2_ENB_STATE_CONNECTED);
	}
	else
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_BRIEF,
				"Previous state is %s",
				SON_X2_ENB_FSM_STATES_NAMES[p_enb_ctxt->enb_prev_fsm_state]);
	}

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_disconnecting_state_reset_resp_handler */

/******************************************************************************
 * Function Name  : sonx2_enb_fsm_resetting_state_reset_resp_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for Reset response received 
 *                  from L3 when eNB connection's state is RESETTING 
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_resetting_state_reset_resp_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	son_rrc_reset_res_t *p_reset_res = (son_rrc_reset_res_t *)p_msg;
	son_x2_delete_enb_req_t del_enb_req = {0};


	SON_UT_TRACE_ENTER();

	/* SPR-11340 Fix Starts */
	/* Lines Deleted */
	/* SPR-11340 Fix Ends */

	/* If success response is received */
	if (1 == p_reset_res->reset_res.response)
	{
		SONX2_ENB_FSM_SET_STATE(p_enb_ctxt,
				p_enb_ctxt->enb_prev_fsm_state,
				p_enb_ctxt->enb_curr_fsm_state,
				X2_ENB_STATE_CONNECTED);

		/* Check if pending data is found to be send in eNB config update req */
		/* SPR-10331 Fix Starts */
		if (list_count(&p_enb_ctxt->enb_config_cell_info) ||
				list_count(&p_enb_ctxt->pending_gu_group_id_data.
					add_gu_id_list) ||
				list_count(&p_enb_ctxt->pending_gu_group_id_data.
					delete_gu_id_list))
			/* SPR-10331 Fix Ends */
		{
			/* Create eNB config request with the pending info */
			x2_create_send_enb_config_update_req_msg(
					&p_enb_ctxt->enb_config_cell_info,
					p_enb_ctxt);
			x2_print_pending_enb_config_update_req_data(
					&p_enb_ctxt->enb_config_cell_info);
			x2_print_pending_gu_group_id_data(p_enb_ctxt);
			x2_print_sent_gu_group_id_data(p_enb_ctxt);
		}
	}
	/* Failure is received in reset response */
	else 
	{
		x2_enb_fsm_state_et prev_state = p_enb_ctxt->enb_prev_fsm_state;
		SONX2_ENB_FSM_SET_STATE(p_enb_ctxt,
				p_enb_ctxt->enb_prev_fsm_state,
				p_enb_ctxt->enb_curr_fsm_state, prev_state);

		/* Initiate delete eNB procedure by sending delete eNB request */
		del_enb_req.transaction_id = son_generate_txn_id();
		x2_insert_eNB_in_list(&del_enb_req.enb_list, p_enb_ctxt);

		p_cspl_hdr->from = SON_X2_MODULE_ID;
		p_cspl_hdr->api = SONX2_DELETE_ENB_REQ;
		p_cspl_hdr->paramlen = sizeof(son_x2_delete_enb_req_t);
		/*SPR_17670_START*/
		sonx2_enb_fsm_delete_enb_req_handler(
				/*SPR_17670_END*/
				X2_ENB_IDX_SONANR_DELETE_ENB_REQ, p_cspl_hdr, 
				p_enb_ctxt, (son_u8 *)&del_enb_req);
		/* Set flag for self triggered delete eNB request */
		p_enb_ctxt->is_self_del_enb_req_triggered = SON_TRUE;
	}

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_resetting_state_reset_resp_handler */

/*****************************************************************************
 * Function Name  : sonx2_enb_fsm_mob_change_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 *                  p_enb_ctxt - Pointer to eNB context 
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONMLB_MOBILITY_CHANGE_REQ,
 *                  message received from SON MLB.
 ****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_mob_change_req_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	mlb_mobility_change_req_t *p_req = (mlb_mobility_change_req_t *)p_msg;
	son_rrc_mobility_change_req_t mob_req;


	SON_UT_TRACE_ENTER();

	SON_MEMSET(&mob_req, 0, sizeof(son_rrc_mobility_change_req_t));
	mob_req.transaction_id = son_generate_txn_id();
	x2_convert_son_enbid_to_l3_enbid(&p_req->trgt_enbid, &mob_req.mob_change_req.gb_enb_id);
	x2_convert_son_ecgi_to_l3_ecgi(&p_req->src_cgi1, &mob_req.mob_change_req.mobility_change_info.enb1_cell_id);
	x2_convert_son_ecgi_to_l3_ecgi(&p_req->src_cgi2, &mob_req.mob_change_req.mobility_change_info.enb2_cell_id);
	mob_req.mob_change_req.mobility_change_info.enb2_mob_param.handover_trigger_change =
		p_req->proposed_mobility_params;
	mob_req.mob_change_req.mobility_change_info.cause.type = x2ap_Cause_radioNetwork;
	mob_req.mob_change_req.mobility_change_info.cause.value = x2ap_load_balancing;

	if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_mobility_change_req(
				&mob_req.mob_change_req,
				SON_MODULE_ID,
				SON_RRC_MODULE_ID,
				mob_req.transaction_id,
				SON_NULL))
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_ERROR,
				"X2AP_MOBILITY_CHANGE_REQ could not be sent for eNB 0x%x",
				son_convert_char_cell_id_to_int_cell_id(p_enb_ctxt->enb_id.enb_id));
	}

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_mob_change_req_handler */

/*****************************************************************************
 * Function Name  : sonx2_enb_fsm_mob_change_ack_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 *                  p_enb_ctxt - Pointer to eNB context 
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONMLB_MOBILITY_CHANGE_ACK,
 *                  message received from SON MLB.
 ****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_mob_change_ack_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	sonmlb_mobility_change_ack_t *p_ack = (sonmlb_mobility_change_ack_t *)p_msg;
	son_rrc_mobility_setting_change_resp_t mob_resp;


	SON_UT_TRACE_ENTER();

	SON_MEMSET(&mob_resp, 0, sizeof(son_rrc_mobility_setting_change_resp_t));
	mob_resp.transaction_id = son_generate_txn_id();
	x2_convert_son_enbid_to_l3_enbid(&p_ack->src_enbid, 
			&mob_resp.mob_setting_change_res.mobility_setting_change_resp.enb2_gb_id);
	x2_convert_son_enbid_to_l3_enbid(&p_ack->trgt_enbid, 
			&mob_resp.mob_setting_change_res.mobility_setting_change_resp.enb1_gb_id);
	x2_convert_son_ecgi_to_l3_ecgi(&p_ack->src_cgi1, 
			&mob_resp.mob_setting_change_res.mobility_setting_change_resp.enb2_cell_id);
	x2_convert_son_ecgi_to_l3_ecgi(&p_ack->src_cgi2, 
			&mob_resp.mob_setting_change_res.mobility_setting_change_resp.enb1_cell_id);
	mob_resp.mob_setting_change_res.mobility_setting_change_resp.response = 1; /* 1 is success at L3 X2AP */

	if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_mobility_setting_change_resp(
				&mob_resp.mob_setting_change_res,
				SON_MODULE_ID,
				SON_RRC_MODULE_ID,
				mob_resp.transaction_id,
				SON_NULL))
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_ERROR,
				" X2AP_MOBILITY_SETTING_CHANGE_RESP could not be sent for eNB 0x%x",
				son_convert_char_cell_id_to_int_cell_id(p_enb_ctxt->enb_id.enb_id));
	}

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_mob_change_ack_handler */

/*****************************************************************************
 * Function Name  : sonx2_enb_fsm_mob_change_fail_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 *                  p_enb_ctxt - Pointer to eNB context 
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONMLB_MOBILITY_CHANGE_FAILURE,
 *                  message received from SON MLB.
 ****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_mob_change_fail_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	sonmlb_mobility_change_failure_t *p_fail = (sonmlb_mobility_change_failure_t *)p_msg;
	son_rrc_mobility_setting_change_resp_t mob_resp;


	SON_UT_TRACE_ENTER();

	SON_MEMSET(&mob_resp, 0, sizeof(son_rrc_mobility_setting_change_resp_t));
	mob_resp.transaction_id = son_generate_txn_id();
	x2_convert_son_enbid_to_l3_enbid(&p_fail->src_enbid, 
			&mob_resp.mob_setting_change_res.mobility_setting_change_resp.enb2_gb_id);
	x2_convert_son_enbid_to_l3_enbid(&p_fail->trgt_enbid, 
			&mob_resp.mob_setting_change_res.mobility_setting_change_resp.enb1_gb_id);
	x2_convert_son_ecgi_to_l3_ecgi(&p_fail->src_cgi1, 
			&mob_resp.mob_setting_change_res.mobility_setting_change_resp.enb2_cell_id);
	x2_convert_son_ecgi_to_l3_ecgi(&p_fail->src_cgi2, 
			&mob_resp.mob_setting_change_res.mobility_setting_change_resp.enb1_cell_id);
	mob_resp.mob_setting_change_res.mobility_setting_change_resp.response = 0; /* 1 is failure at L3 X2AP */
	/*fill cause value*/
	mob_resp.mob_setting_change_res.mobility_setting_change_resp.bitmask |= 
		X2AP_MOBILITY_SETTING_CAUSE_PRESENT;
	mob_resp.mob_setting_change_res.mobility_setting_change_resp.cause.type = x2ap_Cause_radioNetwork;
    mob_resp.mob_setting_change_res.mobility_setting_change_resp.cause.value = x2ap_value_out_of_allowed_range;
    /*SPR 20523 START*/
    SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
            SON_BRIEF,
            "sonx2_enb_fsm_mob_change_fail_handler: "
            "p_fail->bitmask %d ",p_fail->bitmask);

    if (MOBILITY_PARAM_RANGE_PRESENT & p_fail->bitmask)
    /*SPR 20523 END*/
	{
		mob_resp.mob_setting_change_res.mobility_setting_change_resp.bitmask |= 
			X2AP_MOBILITY_PARAM_MOD_RANGE_PRESENT;
		mob_resp.mob_setting_change_res.mobility_setting_change_resp.enb2_mob_param_mod_range.
			ho_trigger_change_lower_limit = p_fail->mob_param_range.lower_limit;
		mob_resp.mob_setting_change_res.mobility_setting_change_resp.enb2_mob_param_mod_range.
			ho_trigger_change_upper_limit = p_fail->mob_param_range.upper_limit;
	}

	if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_mobility_setting_change_resp(
				&mob_resp.mob_setting_change_res,
				SON_MODULE_ID,
				SON_RRC_MODULE_ID,
				mob_resp.transaction_id,
				SON_NULL))
	{
		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name,
				SON_ERROR,
				"X2AP_MOBILITY_SETTING_CHANGE_RESP could not be sent for eNB 0x%x",
				son_convert_char_cell_id_to_int_cell_id(p_enb_ctxt->enb_id.enb_id));
	}

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_mob_change_fail_handler */

/* SPR-10331 Fix Starts */
/*****************************************************************************
 * Function Name  : sonx2_enb_fsm_gu_group_id_config_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to X2 module
 *                  p_enb_ctxt - Pointer to eNB context 
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  SONX2_GU_GROUP_ID_CONFIG_REQ from SMIF
 ****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_gu_group_id_config_req_handler
(
 son_x2_enb_index_et         event_id,
 STACKAPIHDR                 *p_cspl_hdr,
 son_nbr_enb_hash_rec_t      *p_enb_ctxt,
 son_u8                      *p_msg
 )
{
	son_oam_anr_gu_group_id_config_req_t *p_req = 
		(son_oam_anr_gu_group_id_config_req_t *)p_msg;
	x2ap_enb_config_update_req_t *p_enb_config_update_req = SON_PNULL;
	son_rrc_enb_config_update_req_t *p_update_req = SON_PNULL;
	son_size_t update_req_size = sizeof(son_rrc_enb_config_update_req_t);
	son_u16 index = 0;
	SON_UT_TRACE_ENTER();

	/* GU Group Id message shall not change the current state of 
	 * eNB Connection FSM. */

	/* Add the request to the pending requests:
	 *  1. If there is one eNB config update response awaited (indicated by
	 *     is_enb_config_update_res_awaited flag set to TRUE)
	 *  2. Peer has configured time_d_wait expiry value */
	if (SON_TRUE == p_enb_ctxt->is_enb_config_update_res_awaited ||
			p_enb_ctxt->time_d_wait_expiry.s ||
			p_enb_ctxt->time_d_wait_expiry.us)
	{
		x2_add_enb_config_req_data_in_enb_config_cell_list(
				p_cspl_hdr, p_enb_ctxt, p_msg);

		SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
				"Request added to the pending eNB Config Update requests "
				"either due to already triggered request or due to timed "
				"wait timer running.");
	}
	else
	{
		p_update_req = (son_rrc_enb_config_update_req_t *)
			son_mem_get(update_req_size);

		if (SON_PNULL == p_update_req)
		{
			SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_ERROR,
					"Memory allocation failed to p_update_req.Can't not send eNB Config "
					"Update request message to X2AP");
		}
		else
		{
			/* Create eNB config update request message */
			SON_MEMSET(p_update_req, 0, update_req_size);
			p_update_req->transaction_id = son_generate_txn_id();
			p_enb_config_update_req = &p_update_req->enb_config_update_req;
			p_enb_config_update_req->num_peer_enb = 1;
			x2_convert_son_enbid_to_l3_enbid(&p_enb_ctxt->enb_id, 
					&p_enb_config_update_req->enb_info[0]);

			if (SON_OAM_GU_GROUP_ID_TO_ADD_LIST_PRESENT & 
					p_req->bitmask)
			{
				p_enb_config_update_req->bitmask |= 
					X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT;
				p_enb_config_update_req->add_gu_id.num_gu_group_id = 
					p_req->gu_group_id_to_add_list.
					num_gu_group_id;
				for (index = 0; index < 
						p_enb_config_update_req->add_gu_id.num_gu_group_id;
						index++)
				{
					x2_convert_son_gu_group_id_to_l3_gu_group_id(
							&p_req->gu_group_id_to_add_list.gu_id_list[index],
							&p_enb_config_update_req->add_gu_id.
							gu_id_list[index]);
				}
			}
			if (SON_OAM_GU_GROUP_ID_TO_DELETE_LIST_PRESENT & 
					p_req->bitmask)
			{
				p_enb_config_update_req->bitmask |= 
					X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT;
				p_enb_config_update_req->delete_gu_id.num_gu_group_id = 
					p_req->gu_group_id_to_delete_list.
					num_gu_group_id;
				for (index = 0; index <
						p_enb_config_update_req->delete_gu_id.num_gu_group_id;
						index++)
				{
					x2_convert_son_gu_group_id_to_l3_gu_group_id(
							&p_req->gu_group_id_to_delete_list.gu_id_list[index],
							&p_enb_config_update_req->delete_gu_id.
							gu_id_list[index]);
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
						"eNB config update request could not be sent to X2AP");
			}
			else
			{
				/* SPR 11340 Fix Starts */
				p_enb_ctxt->is_enb_config_update_res_awaited = SON_TRUE;
				/* SPR 11340 Fix Ends */

				/* Store the data in last sent eNB config update request 
				   data which shall be used to re-initiate the 
				   X2AP_ENB_CONFIG_UPDATE_REQ in case eNB Config 
				   Update failure Response is received from peer ENB. */
				for (index = 0; index < p_enb_config_update_req->add_gu_id.
						num_gu_group_id; index++)
				{
					x2_insert_gu_grp_id_in_list(
							&p_enb_ctxt->sent_gu_group_id_data.add_gu_id_list,
							&p_enb_config_update_req->add_gu_id.
							gu_id_list[index]);
				}
				for (index = 0; index < p_enb_config_update_req->delete_gu_id.
						num_gu_group_id; index++)
				{
					x2_insert_gu_grp_id_in_list(
							&p_enb_ctxt->sent_gu_group_id_data.
							delete_gu_id_list,
							&p_enb_config_update_req->delete_gu_id.
							gu_id_list[index]);
				}

			}
			/* Free the memory allocated on heap for the update request */
			son_mem_free(p_update_req);
		}
		x2_print_pending_gu_group_id_data(p_enb_ctxt);
		x2_print_sent_gu_group_id_data(p_enb_ctxt);
	}

	SON_UT_TRACE_EXIT();
	return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_gu_group_id_config_req_handler */
/* SPR-10331 Fix Ends */

/*SPR_16835_START*/
/******************************************************************************
 * Function Name  : sonx2_enb_fsm_disconnecting_state_enb_config_update_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for eNB Config Update request received 
 *                  from ES/ANR when eNB Conenction's state is DISCONNECTING.
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_disconnecting_state_enb_config_update_handler
(
    son_x2_enb_index_et         event_id,
    STACKAPIHDR                 *p_cspl_hdr,
    son_nbr_enb_hash_rec_t      *p_enb_ctxt,
    son_u8                      *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /* eNB Config Update message shall not change the current state of
     * eNB Connection FSM. */

    /* Insert entry in pending list */
    SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
            "Request added to the pending eNB Config Update requests "
            "since X2 link disconnection is in progress.");
    x2_add_enb_config_req_data_in_enb_config_cell_list(
        p_cspl_hdr, p_enb_ctxt, p_msg);

    SON_UT_TRACE_EXIT();
    return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_disconnecting_state_enb_config_update_handler */
/******************************************************************************
 * Function Name  : sonx2_enb_fsm_disconnecting_state_gu_grp_id_config_req_handler 
 * Inputs         : event_id        event identity
 *                : p_csl_hdr       Pointer to cspl stack header
 *                : p_enb_ctxt      pointer to son_nbr_enb_hash_rec_t       
 *                : p_msg           Pointer to the message               
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : Event Handler for GU Group Id config request received 
 *                  from SMIF when eNB Conenction's state is DISCONNECTING.
 *****************************************************************************/
static son_x2_result_et
sonx2_enb_fsm_disconnecting_state_gu_grp_id_config_req_handler
(
    son_x2_enb_index_et         event_id,
    STACKAPIHDR                 *p_cspl_hdr,
    son_nbr_enb_hash_rec_t      *p_enb_ctxt,
    son_u8                      *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /* This message shall not change the current state of eNB Connection FSM. */

    /* Insert entry in pending list */
    SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
            "Request added to the pending eNB Config Update requests "
            "since X2 link disconnection is in progress.");
    x2_add_enb_config_req_data_in_enb_config_cell_list(
        p_cspl_hdr, p_enb_ctxt, p_msg);

    SON_UT_TRACE_EXIT();
    return X2_ENB_MSG_EXPECTED;
} /* sonx2_enb_fsm_disconnecting_state_gu_grp_id_config_req_handler */
/*SPR_16835_END*/
/************************* X2 eNB Specific State Machine*********************/
typedef son_x2_result_et (*x2_enb_fsm_evt_handler_t)
	(
	 son_x2_enb_index_et event_id,
	 STACKAPIHDR *p_cspl_hdr,
	 son_nbr_enb_hash_rec_t *p_enb_ctxt,
	 son_u8 *p_msg
	);

	/************************************************************************
	 * Static per ENB X2 FSM
	 ***********************************************************************/
	static x2_enb_fsm_evt_handler_t
	x2_enb_fsm_table[X2_ENB_NUM_OF_STATES][X2_ENB_NUM_OF_EVENTS] = 
{
	/* State X2_ENB_STATE_DISCONNECTED */
	{
		sonx2_enb_fsm_add_enb_req_handler,
		/* SONX2_EVENT_ADD_ENB_REQ */
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_ADD_ENB_RES */
		sonx2_enb_fsm_link_up_ind_handler,
		/* SONX2_EVENT_LINK_UP_IND */
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_LINK_DOWN_IND */ 
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_DELETE_ENB_REQ */
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_DELETE_ENB_RES */
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_MSG_ENB_CONFIG_UPDATE */
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_RES */
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_ENB_RESET_RES */
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_REQ */	
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_ACK */	
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_FAILURE */	
		/* SPR-10331 Fix Starts */
        /* SPR_19619 start */
		sonx2_enb_fsm_invalid_event_handler,
		/* X2_ENB_EVENT_GU_GROUP_ID_CONFIG_REQ */
		/* SPR-10331 Fix Ends */
		sonx2_enb_fsm_invalid_event_handler
		/* X2_ENB_EVENT_PEER_CELL_ACTIVATION_REQ */
        /* SPR_19619 stop */

	},
	/* State X2_ENB_STATE_CONNECTING */
	{
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_ADD_ENB_REQ */
		sonx2_enb_fsm_add_enb_res_handler,
		/* SONX2_EVENT_ADD_ENB_RES */
		sonx2_enb_fsm_link_up_ind_handler,
		/* SONX2_EVENT_LINK_UP_IND */
		sonx2_enb_fsm_link_down_ind_handler,
		/* SONX2_EVENT_LINK_DOWN_IND */
		/*SPR_17670_START*/
		sonx2_enb_fsm_delete_enb_req_handler,
		/*SPR_17670_END*/
		/* SONX2_EVENT_DELETE_ENB_REQ */
		sonx2_enb_fsm_invalid_event_handler,       
		/* SONX2_EVENT_DELETE_ENB_RES */
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_MSG_ENB_CONFIG_UPDATE */
		sonx2_enb_fsm_invalid_event_handler,     
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_RES */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_ENB_RESET_RES */
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_REQ */	
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_ACK */	
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_FAILURE */	
		/* SPR-10331 Fix Starts */
        /* SPR_19619 start */
		sonx2_enb_fsm_invalid_event_handler,
		/* X2_ENB_EVENT_GU_GROUP_ID_CONFIG_REQ */
		/* SPR-10331 Fix Ends */
		sonx2_enb_fsm_invalid_event_handler
		/* X2_ENB_EVENT_PEER_CELL_ACTIVATION_REQ */
        /* SPR_19619 stop */

	},
	/* State X2_ENB_STATE_CONNECTED */
	{
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_ADD_ENB_REQ */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_ADD_ENB_RES */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_LINK_UP_IND */
		sonx2_enb_fsm_link_down_ind_handler,
		/* SONX2_EVENT_LINK_DOWN_IND */
		/*SPR_17670_START*/
		sonx2_enb_fsm_delete_enb_req_handler,
		/*SPR_17670_END*/
		/* SONX2_EVENT_DELETE_ENB_REQ */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_DELETE_ENB_RES */
		sonx2_enb_fsm_connecetd_state_enb_config_update_handler,
		/* SONX2_EVENT_MSG_ENB_CONFIG_UPDATE */
		sonx2_enb_fsm_enb_config_update_res_handler, 
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_RES */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_ENB_RESET_RES */
		sonx2_enb_fsm_mob_change_req_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_REQ */	
		sonx2_enb_fsm_mob_change_ack_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_ACK */	
		sonx2_enb_fsm_mob_change_fail_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_FAILURE */		
		/* SPR-10331 Fix Starts */
		/* SPR_19619 start */
		sonx2_enb_fsm_gu_group_id_config_req_handler,
		/* X2_ENB_EVENT_GU_GROUP_ID_CONFIG_REQ */
		sonx2_enb_fsm_peer_cell_activation_req_handler
		/* X2_ENB_EVENT_PEER_CELL_ACTIVATION_REQ */
		/* SPR_19619 stop */
		/* SPR-10331 Fix Ends */
	},
	/* State X2_ENB_STATE_DISCONNECTING */
	{
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_ADD_ENB_REQ */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_ADD_ENB_RES */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_LINK_UP_IND */
		sonx2_enb_fsm_link_down_ind_handler,
		/* SONX2_EVENT_LINK_DOWN_IND */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_DELETE_ENB_REQ */
		sonx2_enb_fsm_delete_enb_res_handler,
		/* SONX2_EVENT_DELETE_ENB_RES */
		/*SPR_16835_START*/
		sonx2_enb_fsm_disconnecting_state_enb_config_update_handler, 
		/*SPR_16835_END*/
		/* SONX2_EVENT_MSG_ENB_CONFIG_UPDATE */
		/* SPR 11340 Fix Starts */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SPR 11340 Fix Ends */
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_RES */
		sonx2_enb_fsm_disconnecting_state_reset_resp_handler,
		/* SONX2_EVENT_ENB_RESET_RES */
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_REQ */	
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_ACK */	
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_FAILURE */	
		/* SPR_16835_START */
        /* SPR_19619 start */
		sonx2_enb_fsm_disconnecting_state_gu_grp_id_config_req_handler, 
		/* X2_ENB_EVENT_GU_GROUP_ID_CONFIG_REQ */
		/* SPR_16835_END */
		sonx2_enb_fsm_invalid_event_handler
		/* X2_ENB_EVENT_PEER_CELL_ACTIVATION_REQ */
        /* SPR_19619 stop */


    },
	/* State X2_ENB_STATE_RESETTING */
	{
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_ADD_ENB_REQ */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_ADD_ENB_RES */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_LINK_UP_IND */
		sonx2_enb_fsm_link_down_ind_handler,
		/* SONX2_EVENT_LINK_DOWN_IND */
		sonx2_enb_fsm_resetting_state_delete_enb_req_handler,
		/* SONX2_EVENT_DELETE_ENB_REQ */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_DELETE_ENB_RES */
		sonx2_enb_fsm_resetting_state_enb_config_update_handler,
		/* SONX2_EVENT_MSG_ENB_CONFIG_UPDATE */
		sonx2_enb_fsm_invalid_event_handler, 
		/* SONX2_EVENT_ENB_CONFIG_UPDATE_RES */
		sonx2_enb_fsm_resetting_state_reset_resp_handler,
		/* SONX2_EVENT_ENB_RESET_RES */
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_REQ */	
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_ACK */	
		sonx2_enb_fsm_invalid_event_handler,
		/* SONX2_EVENT_SON_MOBILITY_CHANGE_FAILURE */	
		/* SPR-10331 Fix Starts */
        /* SPR_19619 start */
		sonx2_enb_fsm_resetting_state_gu_grp_id_config_req_handler,
			/* X2_ENB_EVENT_GU_GROUP_ID_CONFIG_REQ */
			/* SPR-10331 Fix Ends */
		sonx2_enb_fsm_invalid_event_handler 
		/* X2_ENB_EVENT_PEER_CELL_ACTIVATION_REQ */
        /* SPR_19619 stop */

    }
};

/*****************************************************************************
 * Function Name  : x2_enb_calculate_msg_index
 * Inputs         : p_cspl_hdr - Pointer to the the CSPL header information
 * Outputs        : None
 * Returns        : Index in SONX2 eNB FSM
 * Description    : This function computes the index in the X2 eNB FSM for the
 *                  given procedure code
 ****************************************************************************/
static son_x2_enb_index_et
x2_enb_calculate_msg_index
(
 STACKAPIHDR                  *p_cspl_hdr
 )
{
	son_x2_enb_index_et ret = X2_ENB_IDX_NOT_AVAILABLE;


	SON_UT_TRACE_ENTER();

	switch (p_cspl_hdr->from)
	{
		case SON_RRC_MODULE_ID:
			{
				switch (p_cspl_hdr->api)
				{
					case X2AP_ADD_ENB_RES:
						{
							ret = X2_ENB_IDX_RRC_ADD_ENB_RES;
							break;
						}

					case X2AP_ENB_LINK_UP_IND:
						{
							ret = X2_ENB_IDX_RRC_LINK_UP_IND;
							break;
						}

					case X2AP_ENB_LINK_DOWN_IND:
						{
							ret = X2_ENB_IDX_RRC_LINK_DOWN_IND;
							break;
						}

					case X2AP_DEL_ENB_RES:
						{
							ret = X2_ENB_IDX_RRC_DELETE_ENB_RES;
							break;
						}

					case X2AP_ENB_CONFIG_UPDATE_RES:
						;
						/* break is not used intentionally as both eNB 
						   config update response and eNB config update 
						   wait indication are handled using same handler 
						   function */
					case X2AP_ENB_CONFIG_UPDATE_WAIT_IND:
						{
							ret = X2_ENB_IDX_RRC_ENB_CONFIG_UPDATE_RES;
							break;
						}

					case X2AP_RESET_RES:
						{
							ret = X2_ENB_IDX_RRC_RESET_RES;
							break;
						}
					default:
						{
							SON_LOG(X2_GET_LOG_MODE,
									p_son_x2_facility_name, SON_WARNING,
									"Invalid API = [%u] received from RRC", p_cspl_hdr->api);
							break;
						}
				}
			}
			break;
		case SON_MIF_MODULE_ID:
			{
				switch (p_cspl_hdr->api)
				{
					case SONX2_GU_GROUP_ID_CONFIG_REQ:
						{
							ret = X2_ENB_IDX_GU_GROUP_ID_CONFIG_REQ;
							break;
						}
					default:
						{
							SON_LOG(X2_GET_LOG_MODE,
									p_son_x2_facility_name, SON_WARNING,
									"Invalid API = [%u] received from SMIF", 
									p_cspl_hdr->api);
						}
				}
				break;
			}
		default:
			{
				switch (p_cspl_hdr->api)
				{
					case SONX2_ADD_ENB_REQ:
						{
							ret = X2_ENB_IDX_SONANR_ADD_ENB_REQ;
							break;
						}

					case SONX2_DELETE_ENB_REQ:
						{
							ret = X2_ENB_IDX_SONANR_DELETE_ENB_REQ;
							break;
						}

					case SONX2_SWITCH_ON_OFF_IND:

						/* Break is missing as both switch on off indication from ES
						 *  and NRT update indication requires same handling at eNB
						 *  level which is to trigger eNB Config Update */
					case SONX2_NRT_UPDATE_IND:    
						{
							ret = X2_ENB_IDX_MSG_ENB_CONFIG_UPDATE;
							break;
						}

					case SONX2_MOBILITY_CHANGE_REQ:
					case SONMLB_MOBILITY_CHANGE_REQ:
						{
							ret = X2_ENB_IDX_SONMLB_MOBILITY_CHANGE_REQ;
							break;
						}

					case SONX2_MOBILITY_CHANGE_ACK:
					case SONMLB_MOBILITY_CHANGE_ACK:
						{
							ret = X2_ENB_IDX_SONMLB_MOBILITY_CHANGE_ACK;
							break;
						}

					case SONX2_MOBILITY_CHANGE_FAILURE:
					case SONMLB_MOBILITY_CHANGE_FAILURE:
						{
							ret = X2_ENB_IDX_SONMLB_MOBILITY_CHANGE_FAILURE;
							break;
						}

			/* SPR_19619 start */
           
					case SONX2_PEER_CELL_ACTIVATION_REQ:
						{
							ret = X2_ENB_IDX_PEER_CELL_ACTIVATION_REQ;
							break;
						}
			/* SPR_19619 stop */

                    default:
						{
							SON_LOG(X2_GET_LOG_MODE,
									p_son_x2_facility_name, SON_WARNING,
									"Invalid API = [%u] received", p_cspl_hdr->api);
							break;
						}
				}
			} 
	} /* from switch */

	SON_UT_TRACE_EXIT();
	return ret;
} /* x2_enb_calculate_msg_index */

/*****************************************************************************
 * Function Name  : x2_enb_process_msg
 * Inputs         : p_cspl_hdr         Pointer to CSPL Header
 *                  p_enb_ctxt         Pointer to eNB context
 *                  p_x2_msg           Pointer to incoming message
 * Outputs        : None
 * Returns        : son_x2_result_et Indicator for global X2 to take decision
 *                                  on further handling of the message.
 *                                  (Expected/Unexpected message based on
 *                                  the eNB FSM state)
 * Description    : This is the entry point to the X2 eNB FSM.
 *                  This function takes the API message received, CSPL header,
 *                  eNB context and calls the required event handler function.
 ****************************************************************************/
son_x2_result_et
x2_enb_process_msg
(
 STACKAPIHDR             *p_cspl_hdr,
 son_nbr_enb_hash_rec_t  *p_enb_ctxt,
 son_u8                  *p_x2_msg
 )
{
	x2_enb_fsm_state_et curr_enb_fsm_state = p_enb_ctxt->enb_curr_fsm_state;
	son_x2_result_et ret = X2_ENB_MSG_UNEXPECTED;
	son_x2_enb_index_et event_id = X2_ENB_IDX_NOT_AVAILABLE;


	SON_UT_TRACE_ENTER();

	/* Call the appropriate message handler based on the API Id and
	 *     source module id */
	event_id = x2_enb_calculate_msg_index(p_cspl_hdr);

	/* Validate the index of x2_enb_fsm_table array*/
	if (X2_ENB_NUM_OF_EVENTS <= event_id)
	{
		/* No need to print the log here for invalid API Id as it is
		 * already printed in x2_enb_calculate_msg_index function */
		/* Do nothing */
	}
	/* Validate the eNB FSM state */
	else if (X2_NUM_OF_STATES <= (x2_global_fsm_state_et)curr_enb_fsm_state)
	{
		SON_LOG(X2_GET_LOG_MODE,
				p_son_x2_facility_name, SON_WARNING,
				"API %u received in "
				"invalid state %u", p_cspl_hdr->api,
				curr_enb_fsm_state);
	}
	else 
	{
        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
              "### eNB_FSM: eNB 0x%x Processing State %s Event %s ###",
               son_convert_char_cell_id_to_int_cell_id(p_enb_ctxt->enb_id.enb_id),
               SON_X2_ENB_FSM_STATES_NAMES[curr_enb_fsm_state], SON_X2_ENB_FSM_EVENT_NAMES[event_id]);

		ret = (*x2_enb_fsm_table[curr_enb_fsm_state][event_id])
			(event_id, p_cspl_hdr, p_enb_ctxt, p_x2_msg);

        SON_LOG(X2_GET_LOG_MODE, p_son_x2_facility_name, SON_INFO,
              "### eNB_FSM exit ###");
    }

	SON_UT_TRACE_EXIT();
	return ret;
} /* x2_enb_process_msg */
