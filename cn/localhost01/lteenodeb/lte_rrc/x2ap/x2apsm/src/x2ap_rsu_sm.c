/*******************************************************************************
 *
 *  FILE NAME   : x2ap_rsu_sm.c
 *
 *  DESCRIPTION : This file contains the Resource status update state machine 
 *                and the state machine Entry function definition. 
 *
 *  DATE 05-March-2012  NAME Rajneesh Walia
 *                     REFERENCE  36.423_960(3GPP Spec) REASON Initial
 *
 *  Copyright 2010, Aricent Inc.
 *
 *******************************************************************************/
#include "x2ap_rsu_sm.h" 
#include "x2ap_error_indication.h"
#include "asn_dec_wrappers.h"
#include "rrc_logging.h"


/* RSU SM States */
U8* RSU_FSM_STATE_NAMES[]=
{
	(U8*)"X2AP_RSU_STATE_IDLE",           /* Idle state*/
	(U8*)"X2AP_RSU_STATE_SEND_RSU",       /* Sending resource status update */
	(U8*)"X2AP_RSU_STATE_STOP_ONGOING",   /* Stop command ongoing */
	(U8*)"X2AP_RSU_STATE_START_ONGOING",  /* Start command ongoing */
	(U8*)"X2AP_RSU_STATE_LISTEN_FOR_RSU", /* Listening for resource status update */
};

/* RSU SM Events*/
U8* RSU_FSM_EVENT_NAMES[]=
{
	(U8*)"X2AP_RSU_EVENT_X2_CONNECTION_DOWN",             /* X2 connection down */
	(U8*)"X2AP_RSU_EVENT_RESOURCE_STATUS_INFO",           /* Info recvd from RRM */
	(U8*)"X2AP_RSU_EVENT_RESOURCE_STATUS_UPDATE",         /* Update from peer eNB */
	(U8*)"X2AP_RSU_EVENT_RESOURCE_STATUS_FAILURE",        /* Failure from peer eNB */
	(U8*)"X2AP_RSU_EVENT_RESOURCE_STATUS_REQUEST",        /* Request from peer eNB */
	(U8*)"X2AP_RSU_EVENT_RESOURCE_STATUS_RESPONSE",       /* Response from peer eNB */
	(U8*)"X2AP_RSU_EVENT_RESOURCE_STATUS_RRM_STOP_REQ",   /* Stop request   */
	(U8*)"X2AP_RSU_EVENT_RESOURCE_STATUS_RRM_STOP_RES",   /* Stop response  */
	(U8*)"X2AP_RSU_EVENT_RESOURCE_STATUS_RRM_START_REQ",  /* Start request  */
	(U8*)"X2AP_RSU_EVENT_RESOURCE_STATUS_RRM_START_RES",   /* Start response */
	(U8*)"X2AP_RSU_EVENT_PEER_ENB_RESPONSE_TIMER_EXPIRY", /* Peer eNB response Timer */
	(U8*)"X2AP_RSU_EVENT_RRM_RESPONSE_TIMER_EXPIRY"       /* RRM response timer */
};

#define X2AP_RRM_MAX_API_COUNT 9
#define MAX_CELLIN_ENB 256 

/* RSU X2AP-RRM APIs */
U8* X2AP_RRM_RSU_API_NAMES[]=
{
	(U8*)"X2AP_RRM_RSU_RRM_START_REQ",
	(U8*)"X2AP_RRM_RSU_ENB_START_REQ",
	(U8*)"X2AP_RRM_RSU_RRM_STOP_REQ",
	(U8*)"X2AP_RRM_RSU_ENB_STOP_REQ",
	(U8*)"X2AP_RRM_RSU_RRM_RES",
	(U8*)"X2AP_RRM_RSU_ENB_RES",
	(U8*)"X2AP_RRM_RSU_RRM_UPDATE",
	(U8*)"X2AP_RRM_RSU_UPDATE"
};


/*****************************************************************************
 * DATA
 *      DESCRIPTION: This is used for the logging of X2AP RSU States.
 ****************************************************************************/
U8 *X2AP_RSU_FSM_STATES_NAMES[] =
{
	(U8 *)"X2AP_RSU_STATE_IDLE",
	(U8 *)"X2AP_RSU_STATE_START_ONGOING",
	(U8 *)"X2AP_RSU_STATE_STOP_ONGOING",
	(U8 *)"X2AP_RSU_STATE_SEND_RSU",
	(U8 *)"X2AP_RSU_STATE_LISTEN_RSU",
	(U8 *)"X2AP_RSU_MAX_STATE"
};

/******************************************************************************
 * DATA DESCRIPTION : This enum is used for the logging of X2AP RSU Events.
 ******************************************************************************/
U8 *X2AP_RSU_FSM_EVENTS_NAMES[] =
{

	(U8 *)"X2AP_RSU_EVENT_RESOURCE_STATUS_RRM_START_REQ",
	(U8 *)"X2AP_RSU_EVENT_RESOURCE_STATUS_RRM_STOP_REQ",
	(U8 *)"X2AP_RSU_EVENT_RESOURCE_STATUS_RRM_RES",
	(U8 *)"X2AP_RSU_EVENT_RESOURCE_STATUS_REQUEST",
	(U8 *)"X2AP_RSU_EVENT_RESOURCE_STATUS_RESPONSE",
	(U8 *)"X2AP_RSU_EVENT_RESOURCE_STATUS_FAILURE",
	(U8 *)"X2AP_RSU_EVENT_RESOURCE_STATUS_UPDATE",
	(U8 *)"X2AP_RSU_EVENT_RESOURCE_STATUS_INFO",
	(U8 *)"X2AP_RSU_EVENT_RESOURCE_STATUS_ENB_TIMER_EXPIRY",
	(U8 *)"X2AP_RSU_EVENT_RESOURCE_STATUS_RRM_TIMER_EXPIRY" ,  
	(U8 *)"X2AP_RSU_EVENT_RESOURCE_STATUS_RSU_TIMER_EXPIRY",
	(U8 *)"X2AP_RSU_EVENT_X2_CONNECTION_DOWN",
	(U8 *)"X2AP_RSU_MAX_EVENT"    
};



/******************************************************************************
 * Function Name  : x2ap_rsu_res_status_rrm_start_req_evt_hdl
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  transaction_Id - transaction id
 *                  p_rrm_rsu_start_req - pointer to x2ap_rrm_rsu_rrm_start_req_t
 *                  peer_enodeb_id - Peer eNB id
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE 
 * Description    : This function is called to handle the resource status 
 *                  request received from the RRM 
 *******************************************************************************/
	x2ap_return_et 
x2ap_rsu_res_status_rrm_start_req_evt_hdl(x2ap_gb_context_t* p_x2ap_gb_ctx,
		U32 transaction_Id,
		x2ap_rrm_rsu_rrm_start_req_t *p_rrm_rsu_start_req,
		U8 peer_enodeb_id)
{
	x2ap_timer_t timer_id;
	rsu_enb_meas_id_search_node_t *p_insert_node = X2AP_P_NULL;
	x2ap_peer_enb_context_t *p_peer_enb_ctx = X2AP_NULL;
	U32 meas_Id = X2AP_NULL; 
	U8 counter = X2AP_NULL;
	x2ap_return_et retVal = X2AP_FAILURE;

	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);
	X2AP_ASSERT(X2AP_P_NULL != p_rrm_rsu_start_req);

	/* 4. Create Measurement ID and Measurement Context. */
	/* Fetch the measurement ID from the pool */
	meas_Id = x2ap_rsu_get_meas_id(p_x2ap_gb_ctx);
	if ( X2AP_RSU_MEASUREMENT_ID_INVALID == meas_Id)
	{
		X2AP_TRACE(X2AP_WARNING,"\nInValid MeasureMent ID ");
		return X2AP_FAILURE;
	}

	p_insert_node = x2ap_mem_get(sizeof(rsu_enb_meas_id_search_node_t));
	if(X2AP_P_NULL == p_insert_node)
	{
		X2AP_TRACE(X2AP_FATAL,"\nMemeory allocation failed ");
		return X2AP_FAILURE;
	}
	/* SPR 7573-RSU fix */
	X2AP_MEMSET(p_insert_node,0,sizeof(rsu_enb_meas_id_search_node_t));
	/* SPR 7573-RSU fix */
	/* SPR_5966 */
	p_insert_node->x2ap_rsu_ctx.self_meas_id = meas_Id;
	p_insert_node->x2ap_rsu_ctx.src_or_trg_meas_id = SRC_MEAS_ID;
	/* SPR_5966 */
	p_insert_node->x2ap_rsu_ctx.enb1_meas_id = meas_Id;

	/* Set Measurement ID of Peer ENB as Invalid. */
	p_insert_node->x2ap_rsu_ctx.enb2_meas_id = X2AP_RSU_MEASUREMENT_ID_INVALID; 
	p_insert_node->x2ap_rsu_ctx.rrm_transaction_id = transaction_Id; 
	p_insert_node->x2ap_rsu_ctx.peer_enb_id = peer_enodeb_id;


	/* fetch the peer enodeB context using peer enb id as key */
	for(counter = 0 ; counter < MAX_PEER_ENB ; counter++)
	{
		/* SPR 9061 Fix Start */
		if ((p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].peer_enodeb_id ==
					peer_enodeb_id) && 
				(p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].op_status == OPERATIONAL))
			/* SPR 9061 Fix End */
		{
			p_peer_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
			break;

		} 
	}    

	/* Prepare X2AP: RESOURCE STATUS REQUEST (Start)*/
	/* Build and send ASN X2AP: RESOURCE STATUS REQUEST (Start) */
	if(p_rrm_rsu_start_req->bitmask & RRM_RSU_START_PARTIAL_SUCC_IND_IS_PRESENT)
	{
		p_insert_node->x2ap_rsu_ctx.partial_success_indicator_present = 1;
	}
	/*klockwork Fix*/
	if (PNULL != p_peer_enb_ctx)
	{
		retVal = x2ap_rsu_build_and_send_start_req(p_x2ap_gb_ctx,
				p_peer_enb_ctx,
				p_rrm_rsu_start_req,
				p_insert_node);
	}
	/*klockwork Fix*/

	if (X2AP_SUCCESS == retVal)
	{
		/* 5. a.Start X2AP_RSU_ENB_GUARD_TIMER.*/
		if (X2AP_SUCCESS == x2ap_rsu_start_timer(X2AP_RSU_ENB_GUARD_TIMER,
					p_peer_enb_ctx, 
					p_x2ap_gb_ctx->config_param.timer_conf_info.
					x2ap_rsu_enb_guard_tim_val,
					&timer_id,
					meas_Id)) 
		{
			X2AP_TRACE(X2AP_INFO,
					"X2AP_RSU_ENB_GUARD_TIMER Started Successfully");
		}
		else
		{
			/* coverity_fix_63256_start */
			x2ap_mem_free(p_insert_node);
			/* coverity_fix_63256_stop */
			return X2AP_FAILURE;
		} 
		/* Store Timer ID in Measurement context. */
		p_insert_node->x2ap_rsu_ctx.timer_id = timer_id;
		p_insert_node->x2ap_rsu_ctx.timer_type = X2AP_RSU_ENB_GUARD_TIMER;

		/* Move to X2AP_RSU_STATE_START_ONGOING from X2AP_RSU_STATE_IDLE.*/
		p_insert_node->x2ap_rsu_ctx.state = X2AP_RSU_STATE_START_ONGOING; 
   /* SPR 21515 Fix Start */
	X2AP_TRACE(X2AP_INFO,"[%s] Start X2AP RSU Timer:[%d] Timer Value:[%d]",
			__FUNCTION__,p_insert_node->x2ap_rsu_ctx.timer_id, p_insert_node->x2ap_rsu_ctx.timer_type);
   /* SPR 21515 Fix End */
		/* calling function to insert the measurement context in the tree */
		x2ap_st_insert_node(&p_x2ap_gb_ctx->x2ap_rsu_context_tree,&p_insert_node->nodeAnchor);
	}    
	else 
	{
		X2AP_TRACE(X2AP_ERROR, "[%s]: X2AP_RSU_RESOURCE_STATUS(START) send failure",__FUNCTION__);

		/*free the allocated memory */
		x2ap_mem_free(p_insert_node);

		/*Build and send RESOURCE STATUS RESPONSE(START) as FAILURE */
		x2ap_rsu_send_rrm_start_resp(transaction_Id,X2AP_FAILURE,
				X2AP_RSU_MEASUREMENT_ID_INVALID,x2ap_unspecified_2,p_peer_enb_ctx /*x2ap_rsu_gb_enb_id_fix*/
				,X2AP_P_NULL,X2AP_NULL); 
		retVal = X2AP_FAILURE;
	}
	return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_rsu_start_timer
 * Inputs         : timer_type    : Timer Type
 *                  p_enb_ctx - pointer to prrt eNb context 
 *                  timer_value   : Timer Value
 *                  p_timer_id - pointyer to x2ap_timer_t
 *                  means_id : measurement id
 * Outputs        : p_timer_id : Timer Id
 * Returns        : X2AP_FAILURE or  X2AP_SUCCESS 
 * Description    : This start x2ap rsu resource status enb timer 
 *****************************************************************************/
x2ap_return_et 
x2ap_rsu_start_timer
(
 x2ap_timer_id_en               timer_type,
 x2ap_peer_enb_context_t        *p_enb_ctx,
 U32                            timer_value,
 x2ap_timer_t                   *p_timer_id,
 U32                            meas_id
 )
{
	x2ap_return_et ret_val = X2AP_SUCCESS;
	x2ap_timer_buffer_t timer_buf;

	X2AP_UT_TRACE_ENTER();
	X2AP_MEMSET(&timer_buf, 0, sizeof(x2ap_timer_buffer_t));

	timer_buf.timer_type = timer_type;
	timer_buf.auto_delete = X2AP_TRUE;
	timer_buf.sd = p_enb_ctx->sctp_sd;

	/*saving the measurement ID in the timer specific data*/
	/* SPR 21515 Fix Start */
	/*SPR 20636 Changes Start*/
	timer_buf.p_timer_specific_data = x2ap_mem_get(sizeof(U32));
	/* SPR 22310 CID 115907 Fix Start */
	if (X2AP_P_NULL == timer_buf.p_timer_specific_data)
	{
		X2AP_TRACE(X2AP_FATAL,"Memory allocation failed.");
		return X2AP_FAILURE;
	}
	/* SPR 22310 CID 115907 Fix End */
	X2AP_MEMSET(timer_buf.p_timer_specific_data, 0, sizeof(U32));
	*((U32*)timer_buf.p_timer_specific_data) = meas_id;
	/*SPR 20636 Changes End*/

	*p_timer_id = x2ap_start_timer(
			timer_value, &timer_buf,
			sizeof(timer_buf), X2AP_FALSE );

	X2AP_TRACE(X2AP_INFO,"[%s] Start X2AP RSU Timer: %d Timer Value:%d  Timer ID is:[%p] timer specific data is %d",
			__FUNCTION__, timer_type, timer_value, p_timer_id, *((U32 *)(timer_buf.p_timer_specific_data)));
	/* SPR 21515 Fix End */
	X2AP_UT_TRACE_EXIT();
	return ret_val;
}

/****************************************************************************
 * Function Name  : x2ap_rsu_build_and_send_start_req
 * Inputs         : p_x2ap_gb_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 *                  p_rrm_rsu_start_req - pointer to x2ap_rrm_rsu_rrm_start_req_t
 *                  p_insert_node -  pointer to rsu_enb_meas_id_search_node_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called when X2 Association is UP
 *                  received from OAM.This function is invoked per ENB context
 *                  to build and send x2ap resource statuse request.
 *****************************************************************************/
x2ap_return_et x2ap_rsu_build_and_send_start_req(x2ap_gb_context_t  *p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t *p_enb_ctx,
		x2ap_rrm_rsu_rrm_start_req_t *p_rrm_rsu_start_req,
		rsu_enb_meas_id_search_node_t *p_insert_node)
{
	x2ap_return_et        retVal = X2AP_FAILURE;
	U8                    asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
	U16                   asn_msg_len = 0;
	x2ap_timer_buffer_t  x2_setup_rsp_timer_buf;
	rrc_x2ap_rsu_start_req_t x2ap_rsu_start_req;

	x2ap_peer_enb_msg_t  peer_enb_msg;
    /* SPR 16118 Fix Start */
    OSCTXT asn1_ctx;
    /* SPR 16118 Fix End */


	X2AP_UT_TRACE_ENTER();
	/* Received pointer should not be NULL */
	X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);
	X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);
	X2AP_ASSERT(p_rrm_rsu_start_req != X2AP_P_NULL);
	X2AP_ASSERT(p_insert_node != X2AP_P_NULL);

	X2AP_MEMSET (&x2_setup_rsp_timer_buf, X2AP_NULL, sizeof(x2ap_timer_buffer_t));
	X2AP_MEMSET (&x2ap_rsu_start_req, X2AP_NULL, sizeof(rrc_x2ap_rsu_start_req_t));
	X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    /* SPR 16118 Fix Start */
   /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        X2AP_TRACE(X2AP_ERROR,
                "[x2ap_ResourceStatusResponse] "
                "ASN context initialization failed");
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    /* SPR 16118 Fix End */

	/* Build X2AP RSU REQUEST  TO send to eNB */
	if(X2AP_SUCCESS == x2ap_rsu_build_start_req(&x2ap_rsu_start_req,
				p_x2ap_gb_ctx,
				p_enb_ctx,
				p_rrm_rsu_start_req,
				p_insert_node))
	{
		/* ASN Encoding */
		retVal = x2ap_rsu_encode_start_req(
				&(x2ap_rsu_start_req),
				asn_msg_buff,
                        &asn_msg_len,
                        /* SPR 16118 Fix Start */
                        &asn1_ctx
                        /* SPR 16118 Fix End */
                        );
		/* Send Message */
		if (X2AP_SUCCESS == retVal)
        {
            peer_enb_msg.p_msg = asn_msg_buff;
            peer_enb_msg.msg_len = asn_msg_len;
            /* SPR_14436_Fix Start */
            peer_enb_msg.stream_id = p_enb_ctx->stream_info.
                stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
            /* SPR_14436_Fix Stop */

            if (INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd)
            {
                /*Sending the  X2 RSU START REQUEST to Peer eNB via SCTP */
                retVal = x2ap_sctp_sm_entry
                    (X2AP_SCTP_SEND_DATA_EV,(U8*)&peer_enb_msg,p_x2ap_gb_ctx,p_enb_ctx);
            }    
        }     
		else 
		{
			X2AP_TRACE(X2AP_ERROR,
					"[%s]::ASN Encoding of X2AP RSU REQUEST(START) FAILED"
					,__FUNCTION__);
             /* SPR 16118 Fix Start */
             /* SPR 16118 Fix End */
		}
	}
	else
	{
		X2AP_TRACE(X2AP_ERROR,"[%s]: x2ap rsu start request build failed\n",__FUNCTION__);
		X2AP_UT_TRACE_EXIT();
        /* SPR 16118 Fix Start */
        /* SPR 16118 Fix End */
	}
    /* SPR 16118 Fix Start */
    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    /* SPR 16118 Fix End */

	X2AP_UT_TRACE_EXIT();
	return retVal;
}
/****************************************************************************
 * Function Name  : x2ap_rsu_build_start_req
 * Inputs         : p_x2ap_rsu_start_req - pointer to rrc_x2ap_rsu_start_req_t
 *                  p_x2ap_gb_ctx  - pointer to X2AP global context
 *                  p_enb_ctx - pointer to peer eNB context 
 *                  p_rrm_rsu_start_req - pointer to x2ap_rrm_rsu_rrm_start_req_t
 *                  p_insert_node - pointer to rsu_enb_meas_id_search_node_t
 * Outputs        : x2ap_rsu_start_req - x2ap rsu request
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to build x2ap rsu start  request.
 *****************************************************************************/
x2ap_return_et x2ap_rsu_build_start_req(rrc_x2ap_rsu_start_req_t   *p_x2ap_rsu_start_req, x2ap_gb_context_t *p_x2ap_gb_ctx, x2ap_peer_enb_context_t *p_enb_ctx, x2ap_rrm_rsu_rrm_start_req_t* p_rrm_rsu_start_req, rsu_enb_meas_id_search_node_t *p_insert_node)
{
	x2ap_return_et    retVal = X2AP_SUCCESS;
	U16               cell_to_report_count = 0;
	U8                peer_enb_ct = 0;
	U8                peer_enodeb_id = 0;

	X2AP_UT_TRACE_ENTER();
	/* Fetching the peer eNb id from the context */
	if(PNULL != p_enb_ctx) 
	{
		peer_enodeb_id = p_enb_ctx->peer_enodeb_id;
	}

	for (peer_enb_ct = 0; peer_enb_ct<MAX_PEER_ENB; peer_enb_ct++)
	{
		if (peer_enb_ct == peer_enodeb_id && (X2AP_LINK_UP == p_x2ap_gb_ctx->x2ap_link_info[peer_enb_ct].connection_status))
		{
			if(p_insert_node != PNULL)
			{
				/* Fill the eNB1 Measurement Id*/
				p_x2ap_rsu_start_req->eNB1_measurement_Id =  p_insert_node->x2ap_rsu_ctx.enb1_meas_id;
			}

			/* filling the registration required status */
			p_x2ap_rsu_start_req->registration_reqd = X2AP_START;

			/* Udating the bitmask from x2ap global context */
			p_x2ap_rsu_start_req->reportcharacteristics_bitmask = p_rrm_rsu_start_req->report_characteristics;

			/* Getting the number of served cell from the x2ap global context */
			p_x2ap_rsu_start_req->cell_to_report_id.num_served_cell = p_rrm_rsu_start_req->num_cell;

			/*Getting the value of rsu_periodicity*/
			p_x2ap_rsu_start_req->rsu_periodicity = p_rrm_rsu_start_req->report_periodicity;

			if(p_insert_node != NULL && p_insert_node->x2ap_rsu_ctx.partial_success_indicator_present)
			{
				p_x2ap_rsu_start_req->bitmask |=
					X2AP_PARTIAL_SUCCESS_IND_PRESENT; 
				p_x2ap_rsu_start_req->partial_success_indicator = p_rrm_rsu_start_req->partial_success_indicator;
			}

			/* Updating the Cell info for each cell */ 
			for (cell_to_report_count = 0; cell_to_report_count < p_rrm_rsu_start_req->num_cell; cell_to_report_count++ )
			{
				p_x2ap_rsu_start_req->cell_to_report_id.cell_id[cell_to_report_count].cell_info.plmn_identity = p_rrm_rsu_start_req->cell_info[cell_to_report_count].plmn_identity;
				X2AP_MEMCPY(p_x2ap_rsu_start_req->cell_to_report_id.cell_id[cell_to_report_count].cell_info.cell_identity,p_rrm_rsu_start_req->cell_info[cell_to_report_count].cell_identity,HOME_ENB_ID_OCTET_SIZE);
			}
		}  
		if(MAX_PEER_ENB == peer_enodeb_id)
		{
			X2AP_TRACE(X2AP_WARNING,"Peer EnodeB Id received is out of range : %d", peer_enodeb_id);
			retVal = X2AP_FAILURE;
		}
		X2AP_UT_TRACE_EXIT();
	}
	return retVal;
}
/*******************************************************************************
 * Function Name : x2ap_rsu_encode_start_req
 * Inputs        : p_rsu_start_req - pointer to rrc_x2ap_sn_status_transfer_t
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff_size - pointer to size of result buffer
 * Outputs       : p_buff (ASN.1 encoded message),
 *               p_buff_size (size of result buffer)
 * Returns       : X2AP_SUCCESS / X2AP_FAILURE
 * Description   : This function used to encode sn status transfer
 ********************************************************************************/
x2ap_return_et x2ap_rsu_encode_start_req 
(
 rrc_x2ap_rsu_start_req_t  *p_rsu_start_req,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* SPR 16118 Fix Start */
    OSCTXT *p_asn1_ctx
    /* SPR 16118 Fix End */
)
{

	x2ap_return_et           result = X2AP_FAILURE;
    /* SPR 16118 Fix Start */
    /* SPR 16118 Fix End */
	x2ap_X2AP_PDU               x2ap_pdu;
	x2ap_ResourceStatusRequest  *p_x2ap_msg = X2AP_P_NULL;
	x2ap_CellToReport_List cell_to_report_list;
	x2ap_ReportCharacteristics report_characteristics;

	X2AP_UT_TRACE_ENTER();

	X2AP_ASSERT(PNULL != p_buff);
	X2AP_ASSERT(PNULL != p_buff_size);
	X2AP_ASSERT(PNULL != p_rsu_start_req);

    /* SPR 16118 Fix Start */
    /* SPR 16118 Fix End */
	do
	{
		/* Init X2AP PDU */
		asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
		/* Set Pdu type to Initiating message */
		x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        /* SPR 16118 Fix Start */
		x2ap_pdu.u.initiatingMessage =
            rtxMemAllocType(p_asn1_ctx, x2ap_InitiatingMessage);
       /* SPR 16118 Fix End */
		if (PNULL == x2ap_pdu.u.initiatingMessage)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			break;
		}
		asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

		x2ap_pdu.u.initiatingMessage->procedureCode =
			ASN1V_x2ap_id_resourceStatusReportingInitiation; /*Confirm*/

		x2ap_pdu.u.initiatingMessage->criticality = x2ap_reject;

		/* Set the initiating message type to resource status reporting initiation */
		x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__resourceStatusReportingInitiation;

        /* SPR 16118 Fix Start */
		x2ap_pdu.u.initiatingMessage->value.u.resourceStatusReportingInitiation = 
            rtxMemAllocType(p_asn1_ctx,x2ap_ResourceStatusRequest);
        /* SPR 16118 Fix End */
		if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.resourceStatusReportingInitiation)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			break;
		}
		p_x2ap_msg = x2ap_pdu.u.initiatingMessage->value.u.resourceStatusReportingInitiation;

		asn1Init_x2ap_ResourceStatusRequest(p_x2ap_msg);

		/*Encode IE 1*/
        /* SPR 16118 Fix Start */
        if (RT_OK != asn1Append_x2ap_ResourceStatusRequest_protocolIEs_1(p_asn1_ctx,&p_x2ap_msg->protocolIEs,p_rsu_start_req->eNB1_measurement_Id))
		{
			break;
		}
		/*Encode IE 2*/
		/*if (RT_OK != asn1Append_x2ap_ResourceStatusRequest_protocolIEs_2(&asn1_ctx,&p_x2ap_msg->protocolIEs,p_rsu_start_req->eNB2_measurement_Id))
		  {
		  break;
		  }*/
		/*Encode IE 3*/
        if (RT_OK != asn1Append_x2ap_ResourceStatusRequest_protocolIEs_3(p_asn1_ctx,&p_x2ap_msg->protocolIEs,p_rsu_start_req->registration_reqd))
		{
			break;
		}

		/*Encode IE 4*/
		if (X2AP_FAILURE == x2ap_rsu_build_report_characterstics_bitmask(p_rsu_start_req, &report_characteristics))
		{
			break;
		}

        if (RT_OK != asn1Append_x2ap_ResourceStatusRequest_protocolIEs_4(p_asn1_ctx,&p_x2ap_msg->protocolIEs, &report_characteristics))
		{
			break;
		}

		/*Encode IE 5*/
		asn1Init_x2ap_CellToReport_List(&cell_to_report_list);

        if (X2AP_FAILURE == x2ap_rsu_build_cell_report_list(p_asn1_ctx,&cell_to_report_list,p_rsu_start_req))
		{
			break;
		}
        if (RT_OK != asn1Append_x2ap_ResourceStatusRequest_protocolIEs_5(p_asn1_ctx,&p_x2ap_msg->protocolIEs,&cell_to_report_list))
		{
			break;
		}
		/*Encode IE 6*/
        if (RT_OK != asn1Append_x2ap_ResourceStatusRequest_protocolIEs_6(p_asn1_ctx,&p_x2ap_msg->protocolIEs,p_rsu_start_req->rsu_periodicity))
		{
			break;
		}
		/*Encode IE 7*/
		if(p_rsu_start_req->bitmask & X2AP_PARTIAL_SUCCESS_IND_PRESENT)
		{
            if (RT_OK != asn1Append_x2ap_ResourceStatusRequest_protocolIEs_7(p_asn1_ctx,&p_x2ap_msg->protocolIEs,p_rsu_start_req->partial_success_indicator))
			{
				break;
			}
		}
		/* ASN Encode message */
        pu_setBuffer(p_asn1_ctx, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

        if (RT_OK != asn1PE_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu))
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
			break;
		} 

        *p_buff_size = (U16)pe_GetMsgLen(p_asn1_ctx);
        /* SPR 16118 Fix End */

		/* Print the asn encoded message */
		
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : Resource Status"
                "Request Message sent on X2AP Interface ",&x2ap_pdu);
		result=X2AP_SUCCESS;
	} while(0);
	X2AP_UT_TRACE_EXIT();
	return result;
}

/*****************************************************************************
 * Function Name  : x2ap_rsu_build_report_characterstics_bitmask 
 * Inputs         : p_rsu_start_req - pointer to rrc_x2ap_rsu_start_req_t
 *                  p_report_characteristics - pointer to 
 *                                              x2ap_ReportCharacteristics
 * Outputs        : p_report_characteristics-
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function used to build the report characterstics bitmask.
 ********************************************************************************/
	x2ap_return_et 
x2ap_rsu_build_report_characterstics_bitmask(rrc_x2ap_rsu_start_req_t *p_rsu_start_req, x2ap_ReportCharacteristics* p_report_characteristics)
{
	asn1Init_x2ap_ReportCharacteristics(p_report_characteristics);

	/*Packing the value*/
	rrc_cp_pack_U32(p_report_characteristics->data ,&p_rsu_start_req->reportcharacteristics_bitmask ,"reportcharacteristics_bitmask");
	p_report_characteristics->numbits = 32;
	return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_rsu_build_cell_report_list 
 * Inputs         : pctxt - pointer to OSCTXT
 *                  plist - pointer to x2ap_CellToReport_List
 *                  p_rsu_start_req - pointer to rrc_x2ap_rsu_start_req_t
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function used to encode cell to report list to be sent in
 *               RSU start req to peer enb.
 ********************************************************************************/
	x2ap_return_et
x2ap_rsu_build_cell_report_list(OSCTXT* pctxt,x2ap_CellToReport_List* plist,rrc_x2ap_rsu_start_req_t *p_rsu_start_req)
{
	OSRTDListNode* p_node = X2AP_P_NULL;
	x2ap_CellToReport_List_element *p_elem = X2AP_P_NULL;
	U8 cell_counter = 0;
	U16 noOfCell = 0;

	X2AP_UT_TRACE_ENTER();

	X2AP_ASSERT(X2AP_P_NULL != p_rsu_start_req);

	noOfCell = p_rsu_start_req->cell_to_report_id.num_served_cell;

	if (noOfCell >= MAX_SERVED_CELLS)
	{
		return X2AP_FAILURE;
	}

	for (cell_counter = 0; cell_counter < noOfCell ;cell_counter++)
	{

		rtxDListAllocNodeAndData(pctxt,
				x2ap_CellToReport_List_element,
				&p_node,
				&p_elem);
		if (X2AP_P_NULL == p_node)
		{
			X2AP_TRACE(X2AP_ERROR,"Can't allocate p_node");
			X2AP_UT_TRACE_EXIT();
			return X2AP_FAILURE;
		}         


		asn1Init_x2ap_CellToReport_List_element(p_elem);
		asn1Init_x2ap_CellToReport_Item(&p_elem->value);

		/*p_elem->id = ASN1V_x2ap_id_CellToReport;*/
		p_elem->id = ASN1V_x2ap_id_CellToReport_Item;
		p_elem->criticality = x2ap_ignore;

		/* Filling Target Cell ID (M) */

		p_elem->value.cell_ID.pLMN_Identity.numocts = MCC_OCTET_SIZE;


		x2ap_compose_asn_plmn_identity(
				p_elem->value.cell_ID.pLMN_Identity.data,
				&(p_rsu_start_req->cell_to_report_id.cell_id[cell_counter].cell_info.plmn_identity));

		/* copy cell_identity */
		p_elem->value.cell_ID.eUTRANcellIdentifier.numbits = 8*sizeof(p_rsu_start_req->cell_to_report_id.cell_id[cell_counter].cell_info.cell_identity);
		p_elem->value.cell_ID.eUTRANcellIdentifier.numbits =(p_elem->value.cell_ID.eUTRANcellIdentifier.numbits < MAX_CELL_ID_BITS ? p_elem->value.cell_ID.eUTRANcellIdentifier.numbits : MAX_CELL_ID_BITS);

		X2AP_MEMCPY(((void*)p_elem->value.cell_ID.eUTRANcellIdentifier.data),
				((const void*)p_rsu_start_req->cell_to_report_id.cell_id[cell_counter].cell_info.cell_identity),
				p_elem->value.cell_ID.eUTRANcellIdentifier.numbits/8+1);

		/*p_elem->value.extElem1 = 0; Need To confirm*/
		if (0 == rtxDListAppend(pctxt, plist, (void*)p_elem))
		{ 
			return (x2ap_return_et)LOG_RTERR(pctxt, RTERR_NOMEM);
		}

	} 
	return X2AP_SUCCESS;
}

/*******************************************************************************
 * FUNCTION NAME : x2ap_rsu_build_measurement_initiation_result_list
 * 
 * DESCRIPTION : This function used to encode measurement_initiatin_result_list sent in 
 *               Resource Status response to peer enb.
 * 	
 * INPUTS      : p_asn1_ctx (for memory allocation),
 *               pointer to rrc_x2ap_rsu_response_t,
 *               pointer to x2ap_rrm_rsu_measurement_res_t
 *
 * RETURNS     : X2AP_SUCCESS / X2AP_FAILURE    
 *
 ********************************************************************************/
	x2ap_return_et
x2ap_rsu_build_measurement_initiation_result_list(rrc_x2ap_rsu_response_t* p_x2ap_rsu_response,x2ap_rrm_rsu_measurement_res_t *p_x2ap_rrm_rsu_measurement,OSCTXT *p_asn1_ctx)
{
	/* coverity_fix_54717_start */
	OSRTDListNode*       p_node = PNULL;
	OSRTDListNode*       p_node_failurecause_list_elem = PNULL;
	U16 meas_result_ct = 0;
	U8                   counter = 0;
	unsigned long cause = 0;
	x2ap_MeasurementInitiationResult_List_element *p_elem = X2AP_P_NULL;    
	x2ap_MeasurementInitiationResult_List *p_meas_init_result_list;
	x2ap_MeasurementFailureCause_List_element *p_meas_init_failure_list_element;
	x2ap_MeasurementFailureCause_List *p_meas_init_failure_list;

	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_rsu_response);
	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_rrm_rsu_measurement);

	p_meas_init_result_list = &p_x2ap_rsu_response->measurement_initiation_result_list;


	asn1Init_x2ap_MeasurementInitiationResult_List(p_meas_init_result_list);



	for (meas_result_ct = 0; meas_result_ct < p_x2ap_rrm_rsu_measurement->num_cell; meas_result_ct++)
	{
		rtxDListAllocNodeAndData(p_asn1_ctx,
				x2ap_MeasurementInitiationResult_List_element,
				&p_node,
				&p_elem);

		if (X2AP_P_NULL == p_node)
		{
			X2AP_TRACE(X2AP_ERROR,"Can't allocate p_node");

			rtxDListFreeAll(p_asn1_ctx, p_meas_init_result_list);

			X2AP_UT_TRACE_EXIT();
			return X2AP_FAILURE;
		}

		asn1Init_x2ap_MeasurementInitiationResult_List_element(p_elem);
		asn1Init_x2ap_MeasurementInitiationResult_Item(&p_elem->value);

		p_elem->id = ASN1V_x2ap_id_MeasurementInitiationResult_List;
		p_elem->criticality = x2ap_ignore;
		if(p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].bitmask & X2AP_RRM_FAILED_CAUSE_ITEM_PRESENT)
		{
			p_elem->value.m.measurementFailureCause_ListPresent = 1;
		}

		p_elem->value.cell_ID.pLMN_Identity.numocts = MCC_OCTET_SIZE;
		x2ap_compose_asn_plmn_identity(
				p_elem->value.cell_ID.pLMN_Identity.data,
				&(p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].cell_info.plmn_identity));

		p_elem->value.cell_ID.eUTRANcellIdentifier.numbits = 8*sizeof(p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].cell_info.cell_identity);
		p_elem->value.cell_ID.eUTRANcellIdentifier.numbits =(p_elem->value.cell_ID.eUTRANcellIdentifier.numbits < MAX_CELL_ID_BITS ? p_elem->value.cell_ID.eUTRANcellIdentifier.numbits : MAX_CELL_ID_BITS);


		X2AP_MEMCPY(((void*)p_elem->value.cell_ID.eUTRANcellIdentifier.data),
				((const void*)p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].cell_info.cell_identity),
				p_elem->value.cell_ID.eUTRANcellIdentifier.numbits/8+1);

		p_meas_init_failure_list = &p_elem->value.measurementFailureCause_List;

		asn1Init_x2ap_MeasurementFailureCause_List(p_meas_init_failure_list);



		for(counter = 0;counter < p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].num_measurement;counter++)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx,
					x2ap_MeasurementFailureCause_List_element,
					&p_node_failurecause_list_elem,
					&p_meas_init_failure_list_element);
			if (X2AP_P_NULL == p_node_failurecause_list_elem)
			{
				X2AP_TRACE(X2AP_ERROR,"Can't allocate p_node_failurecause_list_elem");

				rtxDListFreeAll(p_asn1_ctx, p_meas_init_failure_list);

				X2AP_UT_TRACE_EXIT();
				return X2AP_FAILURE;
			}

			asn1Init_x2ap_MeasurementFailureCause_List_element(p_meas_init_failure_list_element);    
			asn1Init_x2ap_MeasurementFailureCause_Item(&p_meas_init_failure_list_element->value);    
			p_meas_init_failure_list_element->id = ASN1V_x2ap_id_MeasurementFailureCause_Item;
			p_meas_init_failure_list_element->criticality = x2ap_ignore;

			p_meas_init_failure_list_element->value.measurementFailedReportCharacteristics.numbits = 32;

            /* SPR 20674 Fix Start */
            /* Code Deleted */
	         rrc_cp_pack_U32(p_meas_init_failure_list_element->value.measurementFailedReportCharacteristics.data,
					&p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].
                    measurement_failure_cause_item[counter].report_characteristics, 
                    "measurementFailedReportCharacteristics");
            /* SPR 20674 Fix End */
			cause = p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].measurement_failure_cause_item[counter].rrm_cause.value;
			p_meas_init_failure_list_element->value.cause.t = 1; 
			p_meas_init_failure_list_element->value.cause.u.radioNetwork = cause; 
			if (0 == rtxDListAppend(p_asn1_ctx,p_meas_init_failure_list, (void*)p_meas_init_failure_list_element))
			{ 
				return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
			}
		}


		if (0 == rtxDListAppend(p_asn1_ctx,p_meas_init_result_list, (void*)p_elem))
		{ 
			return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
		}
	}
	return X2AP_SUCCESS;
}
/* coverity_fix_54717_stop */

/*******************************************************************************
 * FUNCTION NAME : x2ap_rsu_complete_failure_cause_measurement_list
 * 
 * DESCRIPTION : This function used to encode complete failure cause list sent in 
 *               Resource Status failure message to peer enb.
 * 	
 * INPUTS      : p_asn1_ctx (for memory allocation),
 *               pointer to p_x2ap_rrm_rsu_measurement,
 *               pointer to rrc_x2ap_rsu_failure_t
 *
 * RETURNS     : X2AP_SUCCESS / X2AP_FAILURE    
 *
 ********************************************************************************/
	x2ap_return_et
x2ap_rsu_complete_failure_cause_measurement_list(rrc_x2ap_rsu_failure_t* p_x2ap_rsu_failure,x2ap_rrm_rsu_measurement_res_t *p_x2ap_rrm_rsu_measurement,OSCTXT *p_asn1_ctx)
{
	/* coverity_fix_54715_start */
	OSRTDListNode*       p_node = PNULL;
	OSRTDListNode*       p_node_failurecause_list_elem = PNULL;
	U16 meas_result_ct = 0;
	U16 counter        = 0;
	unsigned long cause = 0;
	x2ap_CompleteFailureCauseInformation_List_element *p_elem = X2AP_P_NULL;    
	x2ap_CompleteFailureCauseInformation_List *p_comp_failure_cause_inf_list;
	x2ap_MeasurementFailureCause_List_element *p_meas_init_failure_list_element;
	x2ap_MeasurementFailureCause_List *p_meas_init_failure_list;

	p_comp_failure_cause_inf_list = &p_x2ap_rsu_failure->complete_failure_cause_inf_list;


	asn1Init_x2ap_CompleteFailureCauseInformation_List(p_comp_failure_cause_inf_list);


	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_rsu_failure);
	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_rrm_rsu_measurement);

	for (meas_result_ct = 0; meas_result_ct < p_x2ap_rrm_rsu_measurement->num_cell; meas_result_ct++)
	{
		rtxDListAllocNodeAndData(p_asn1_ctx,
				x2ap_CompleteFailureCauseInformation_List_element,
				&p_node,
				&p_elem);

		if (X2AP_P_NULL == p_node)
		{
			X2AP_TRACE(X2AP_ERROR,"Can't allocate p_node");

			rtxDListFreeAll(p_asn1_ctx, p_comp_failure_cause_inf_list);

			X2AP_UT_TRACE_EXIT();
			return X2AP_FAILURE;
		}

		asn1Init_x2ap_CompleteFailureCauseInformation_List_element(p_elem);
		asn1Init_x2ap_CompleteFailureCauseInformation_Item(&p_elem->value);

		p_elem->id = ASN1V_x2ap_id_CompleteFailureCauseInformation_List;
		p_elem->criticality = x2ap_ignore;

		p_elem->value.cell_ID.pLMN_Identity.numocts = MCC_OCTET_SIZE;
		x2ap_compose_asn_plmn_identity(
				p_elem->value.cell_ID.pLMN_Identity.data,
				&(p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].cell_info.plmn_identity));

		p_elem->value.cell_ID.eUTRANcellIdentifier.numbits = 8*sizeof(p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].cell_info.cell_identity);
		p_elem->value.cell_ID.eUTRANcellIdentifier.numbits =(p_elem->value.cell_ID.eUTRANcellIdentifier.numbits < MAX_CELL_ID_BITS ? p_elem->value.cell_ID.eUTRANcellIdentifier.numbits : MAX_CELL_ID_BITS);


		X2AP_MEMCPY(((void*)p_elem->value.cell_ID.eUTRANcellIdentifier.data),
				((const void*)p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].cell_info.cell_identity),
				p_elem->value.cell_ID.eUTRANcellIdentifier.numbits/8+1);

		p_meas_init_failure_list = &p_elem->value.measurementFailureCause_List;

		asn1Init_x2ap_MeasurementFailureCause_List(p_meas_init_failure_list);



		for(counter = 0;counter < p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].num_measurement;counter++)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx,
					x2ap_MeasurementFailureCause_List_element,
					&p_node_failurecause_list_elem,
					&p_meas_init_failure_list_element);
			if (X2AP_P_NULL == p_node_failurecause_list_elem)
			{
				X2AP_TRACE(X2AP_ERROR,"Can't allocate p_node_failurecause_list_elem");

				rtxDListFreeAll(p_asn1_ctx, p_meas_init_failure_list);

				X2AP_UT_TRACE_EXIT();
				return X2AP_FAILURE;
			}

			asn1Init_x2ap_MeasurementFailureCause_List_element(p_meas_init_failure_list_element);    
			asn1Init_x2ap_MeasurementFailureCause_Item(&p_meas_init_failure_list_element->value);    
			p_meas_init_failure_list_element->id = ASN1V_x2ap_id_MeasurementFailureCause_Item;
			p_meas_init_failure_list_element->criticality = x2ap_ignore;

			p_meas_init_failure_list_element->value.measurementFailedReportCharacteristics.numbits = 32;

            /* SPR 20674 Fix Start */
            /* Code Deleted */
	         rrc_cp_pack_U32(p_meas_init_failure_list_element->value.measurementFailedReportCharacteristics.data,
					&p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].
                    measurement_failure_cause_item[counter].report_characteristics, 
                    "measurementFailedReportCharacteristics");
            /* SPR 20674 Fix End */
			cause = p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].measurement_failure_cause_item[counter].rrm_cause.value;
			p_meas_init_failure_list_element->value.cause.t = p_x2ap_rrm_rsu_measurement->rsu_measurement_res_item[meas_result_ct].measurement_failure_cause_item[counter].rrm_cause.type; 
			p_meas_init_failure_list_element->value.cause.u.radioNetwork = cause; 
			if (0 == rtxDListAppend(p_asn1_ctx,p_meas_init_failure_list, (void*)p_meas_init_failure_list_element))
			{ 
				return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
			}
		}


		if (0 == rtxDListAppend(p_asn1_ctx,p_comp_failure_cause_inf_list, (void*)p_elem))
		{ 
			return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
		}
	}
	return X2AP_SUCCESS;
}
/* coverity_fix_54715_stop */

/****************************************************************************
 * Function Name  : x2ap_rsu_process_msg
 * Inputs         : p_msg_ptr -     Pointer to message buffer
 *                : p_x2ap_gb_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the resource status 
 *                  mesaage received from peer enb. 
 ****************************************************************************/                  
void x2ap_rsu_process_msg(U8 *p_msg_ptr, 
		x2ap_gb_context_t  *p_x2ap_gb_ctx, 
		x2ap_peer_enb_context_t *p_enb_ctx)
{
	U8                             *p_asn_msg;
    /* SPR 19768 Fix Start */
	U32                            *p_asn_msg_len;
    /* SPR 19768 Fix Start */
	rrc_x2ap_rsu_start_req_t       x2ap_rsu_start_req;

	rrc_x2ap_rsu_failure_t         x2ap_rsu_fail;

	rrc_x2ap_rsu_response_t        x2ap_rsu_response;

	x2ap_peer_enb_msg_t          *p_peer_enb_msg = X2AP_P_NULL;
	x2ap_X2AP_PDU        x2ap_pdu;
	OSCTXT               asn1_ctx;
	x2ap_ResourceStatusRequest *p_ResourceStatusRequest = X2AP_P_NULL; 

	x2ap_ResourceStatusFailure *p_ResourceStatusFailure = X2AP_P_NULL; 

	x2ap_ResourceStatusResponse *p_ResourceStatusResponse = X2AP_P_NULL;

	X2AP_UT_TRACE_ENTER();

	p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;
	/* Received pointer should not be NULL */
	X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
	X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
	X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);

	X2AP_MEMSET(&x2ap_rsu_start_req,X2AP_NULL, sizeof(rrc_x2ap_rsu_start_req_t));

	p_asn_msg = (p_peer_enb_msg)->p_msg;
    /* SPR 19768 Fix Start */
	p_asn_msg_len = (U32*)&(p_peer_enb_msg->msg_len);
    /* SPR 19768 Fix Stop */

	/* Init ASN1 context */
	if ( 0 != rtInitContext(&asn1_ctx) )
	{
		/* Drop message */
		X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
				"context initialization failed.",__FUNCTION__);
		return;
	}

	/*Set pointer of asn buffer in asn context*/
	pu_setBuffer(&asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

	if (0 != asn1PD_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu) )
	{
		/* Drop message */
		X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
				"decode failed.",__FUNCTION__);
		/* Send Transfer Syntax Error */
		x2ap_build_and_send_err_indication(p_x2ap_gb_ctx, p_enb_ctx, PNULL);
        /* SPR 16118 Fix Start */
        /* Free ASN Context */
        rtFreeContext(&asn1_ctx);
        /* SPR 16118 Fix End */
		return;
	}
	if (T_x2ap_X2AP_PDU_initiatingMessage == x2ap_pdu.t)
	{
		X2AP_TRACE(X2AP_INFO,"X2AP_MSG : Resource Status Request \n");
	
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : Resource Status Request", &x2ap_pdu);
		p_ResourceStatusRequest = x2ap_pdu.u.initiatingMessage->value.u.resourceStatusReportingInitiation;
		if (X2AP_FAILURE == x2ap_rsu_process_start_msg(&asn1_ctx, p_ResourceStatusRequest, &x2ap_rsu_start_req, p_x2ap_gb_ctx, p_enb_ctx))
		{
            /* SPR 16118 Fix Start */
            X2AP_TRACE(X2AP_INFO,"X2AP_MSG : x2ap_rsu_process_start_msg Failure \n");
            /* SPR 16118 Fix End */
		}
	}
	else if (T_x2ap_X2AP_PDU_unsuccessfulOutcome == x2ap_pdu.t)
	{
		X2AP_TRACE(X2AP_INFO,"X2AP_MSG : Resource Status Failure \n");
		
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : Resource Status Request", &x2ap_pdu);
		p_ResourceStatusFailure = x2ap_pdu.u.unsuccessfulOutcome->value.u.resourceStatusReportingInitiation;
		if (X2AP_FAILURE == x2ap_rsu_process_failure_msg(&asn1_ctx, p_ResourceStatusFailure, &x2ap_rsu_fail, p_x2ap_gb_ctx, p_enb_ctx))
		{
            /* SPR 16118 Fix Start */
            X2AP_TRACE(X2AP_INFO,"X2AP_MSG : x2ap_rsu_process_failure_msg Failure \n");
            /* SPR 16118 Fix End */
		}
	}
	else if (T_x2ap_X2AP_PDU_successfulOutcome == x2ap_pdu.t)
	{
		X2AP_TRACE(X2AP_INFO,"X2AP_MSG : Resource Status Response \n");
	
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : Resource Status Request", &x2ap_pdu);
		p_ResourceStatusResponse = x2ap_pdu.u.successfulOutcome->value.u.resourceStatusReportingInitiation;
		if (X2AP_FAILURE == x2ap_rsu_process_response_msg(&asn1_ctx, p_ResourceStatusResponse, &x2ap_rsu_response, p_x2ap_gb_ctx, p_enb_ctx))
		{
            /* SPR 16118 Fix Start */
            X2AP_TRACE(X2AP_INFO,"X2AP_MSG : x2ap_rsu_process_response_msg \n");
            /* SPR 16118 Fix End */
		}
	}
    /* SPR 16118 Fix Start */
    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    /* SPR 16118 Fix End */
	return;
}

/****************************************************************************
 * Function Name  : x2ap_rsu_process_start_msg
 * Inputs           p_asn1_ctx -  Pointer to ASN context
 *                  p_3gpp    -  Pointer to asn message pointer
 *                  p_trgt   -   Pointer to rrc_x2ap_rsu_start_req_t
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 p_enb_ctx -  Pointer to peer_enodeb_context
 * Outputs        : p_trgt
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the resource status 
 *                  start mesaage received from peer enb. 
 ****************************************************************************/                  
x2ap_return_et x2ap_rsu_process_start_msg
(
 OSCTXT      *p_asn1_ctx,
 x2ap_ResourceStatusRequest       *p_3gpp,    /* unpacked src */
 rrc_x2ap_rsu_start_req_t    *p_trgt,    /* dest */
 x2ap_gb_context_t  *p_x2ap_gb_ctx,      /* X2ap global context */
 x2ap_peer_enb_context_t *p_enb_ctx    /* peer EnodeB context */
 )
{

	x2ap_return_et result = X2AP_SUCCESS; 
	U16 cell_counter = X2AP_NULL;
	U16 count = X2AP_NULL;
	U8 fail_cause = X2AP_NULL;
	U8 success_cell = X2AP_NULL;
	U8 is_rsu_periodicity_present = X2AP_FALSE;

	rsu_enb_meas_id_search_node_t *p_rsu_tree_node = X2AP_P_NULL;
	x2ap_rsu_context_t    *p_x2ap_rsu_ctx = X2AP_P_NULL;
	rsu_enb_meas_id_search_node_t *p_temp = X2AP_P_NULL;
	rsu_enb_meas_id_search_node_t *p_next = X2AP_P_NULL;
	rrc_x2ap_error_indication_t error_indication;
	x2ap_rrm_rsu_measurement_res_t x2ap_rsu_measurement_res_list;
	X2AP_MEMSET(&x2ap_rsu_measurement_res_list,X2AP_NULL,
			sizeof(x2ap_rrm_rsu_measurement_res_t));

	p_temp = (rsu_enb_meas_id_search_node_t*)x2ap_st_get_first_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree));
	p_next = p_temp;

	X2AP_UT_TRACE_ENTER();
	X2AP_MEMSET(&error_indication,X2AP_NULL,
			sizeof(rrc_x2ap_error_indication_t));
	if(X2AP_FAILURE == x2ap_rsu_decode_start_msg(p_x2ap_gb_ctx, p_enb_ctx,
				p_asn1_ctx, p_3gpp,
				p_trgt, &is_rsu_periodicity_present,
				&error_indication))
	{
		X2AP_TRACE(X2AP_ERROR,"%s: X2AP: RESOURCE STATUS REQUEST and decode failed.",__FUNCTION__);
		if (error_indication.cause.t == T_x2ap_Cause_protocol)
		{
			X2AP_TRACE(X2AP_ERROR,"%s: X2AP: RESOURCE STATUS REQUEST:Error Indication detected",__FUNCTION__);
			x2ap_rsu_build_and_send_failure_msg(p_trgt->eNB1_measurement_Id,
					X2AP_RSU_MEASUREMENT_ID_MAX,  /* SPR_5966 */
					p_x2ap_gb_ctx,p_enb_ctx,
					&error_indication.criticality_diagnostics,
					(U8)error_indication.cause.u.protocol,
					p_x2ap_rsu_ctx);
		}
		else
		{
			x2ap_rsu_build_and_send_failure_msg(p_trgt->eNB1_measurement_Id,
					X2AP_RSU_MEASUREMENT_ID_MAX,  /* SPR_5966 */
					p_x2ap_gb_ctx,p_enb_ctx,
					&error_indication.criticality_diagnostics,
					fail_cause,
					p_x2ap_rsu_ctx);
		}
		return X2AP_FAILURE;
	}

	else
	{
		if (p_trgt->registration_reqd == X2AP_START)
		{
			if(p_trgt->bitmask & X2AP_RSU_START_ENB2_MEAS_ID_PRESENT)
			{
				fail_cause = x2ap_abstract_syntax_error_falsely_constructed_message;
				x2ap_rsu_build_and_send_failure_msg(p_trgt->eNB1_measurement_Id,
						X2AP_RSU_MEASUREMENT_ID_MAX,  /* SPR_5966 */
						p_x2ap_gb_ctx,p_enb_ctx,
						X2AP_P_NULL,
						fail_cause,
						p_x2ap_rsu_ctx);
			}
			while(X2AP_P_NULL != p_temp)
			{
				p_next = (rsu_enb_meas_id_search_node_t*)x2ap_st_get_next_node((x2ap_search_tree_node_t*)p_temp);
				if (p_temp->x2ap_rsu_ctx.peer_enb_id == p_enb_ctx->peer_enodeb_id)
				{
					/* SPR_5966 */
					if((p_temp->x2ap_rsu_ctx.enb1_meas_id == p_trgt->eNB1_measurement_Id) &&
							(p_temp->x2ap_rsu_ctx.src_or_trg_meas_id == TRG_MEAS_ID))
						/* SPR_5966 */
					{
						X2AP_TRACE(X2AP_ERROR,"[%s]: Measrurement is already going with this measurement ID\n", __FUNCTION__);
						fail_cause = x2ap_existingMeasurementID;
						result = X2AP_FAILURE; 
						break;
					} 
				}
				p_temp = p_next;
			}

			if (X2AP_SUCCESS == result)
			{

				/*Validating the information received from the peer EnodeB*/
				/*a. Validate the Cell IDs mentioned in the X2AP: RESOURCE STATUS REQUEST.
				  - If any of the cell ID is not valid, then report X2AP: RESOURCE STATUS FAILURE with cause as "CellNotAvailable".*/

				for (cell_counter = 0; cell_counter < p_trgt->cell_to_report_id.num_served_cell; cell_counter++)
				{
					for(count = 0; count < p_x2ap_gb_ctx->num_served_cells; count++)
					{
						if (X2AP_MEMCMP(p_trgt->cell_to_report_id.cell_id
									[cell_counter].cell_info.cell_identity,
									p_x2ap_gb_ctx->served_cell_info[
									p_x2ap_gb_ctx->act_cell_info[count].cell_id].
									ecgi.eutran_cell_id,
									HOME_ENB_ID_OCTET_SIZE*sizeof(U8)))
						{
							X2AP_TRACE(X2AP_ERROR,"X2AP: Resource Status request: CellNotAvailablie");
							fail_cause = x2ap_cell_not_available;
							result = X2AP_FAILURE; 
						}
						else
						{
							success_cell++;
							break;
						}
					} 
				}
				/*Validate if atleast one of the bits 1,2,3,4 of Report_Characteristics_IE is set to 1.
				  - If none of the bits is set then report -2AP: RESOURCE STATUS FAILURE with cause as "ReportCharacteristicsEmpty".*/

				if(X2AP_NULL == p_trgt->reportcharacteristics_bitmask)
				{
					X2AP_TRACE(X2AP_ERROR,"X2AP: Resource Status request: ReportCharacteristicsEmpty");
					fail_cause = x2ap_reportCharacteristicsEmpty;
					result = X2AP_FAILURE; 
				}

				/* Validate Report_Periodicity_IE.
				   - If no value is specified then report X2AP: RESOURCE STATUS FAILURE with cause as "NoReportPeriodicity".  */

				if(!is_rsu_periodicity_present)
				{
					X2AP_TRACE(X2AP_ERROR,"X2AP: Resource Status request: NoReportPeriodicity");
					fail_cause = x2ap_noReportPeriodicity;
					result = X2AP_FAILURE; 
				}
			}


			if (result == X2AP_FAILURE && success_cell != p_trgt->cell_to_report_id.num_served_cell && !(p_trgt->bitmask & X2AP_PARTIAL_SUCCESS_IND_PRESENT)) 
			{
				x2ap_rsu_build_and_send_failure_msg(p_trgt->eNB1_measurement_Id,X2AP_RSU_MEASUREMENT_ID_MAX, /* SPR_5966 */
						p_x2ap_gb_ctx,p_enb_ctx, X2AP_P_NULL,fail_cause,p_x2ap_rsu_ctx);
			}

			else if (result == X2AP_FAILURE && success_cell == X2AP_NULL) 
			{
				x2ap_rsu_build_and_send_failure_msg(p_trgt->eNB1_measurement_Id,X2AP_RSU_MEASUREMENT_ID_INVALID,
						p_x2ap_gb_ctx,p_enb_ctx, X2AP_P_NULL,fail_cause,p_x2ap_rsu_ctx);
			}
			else
			{
				/*Create context and then send the RESOURCE STARTUS START REQUEST TO RRM*/ 
				result = x2ap_rsu_build_and_send_rrm_start_req(p_trgt,p_x2ap_gb_ctx,p_enb_ctx); 
			}
			return result;     
		}
		else if (p_trgt->registration_reqd == X2AP_STOP)
		{
			/* Fetch Measurement context based on measurement ID of eNB1 and eNB2.
			   If Measurement context is not existing report X2AP: RESOURCE STATUS FAILURE with cause as "UnknowneNBMeasurementID".*/

			p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),
					&(p_trgt->eNB2_measurement_Id));
			if(X2AP_P_NULL != p_rsu_tree_node)
			{
				/* get the measurement context */
				p_x2ap_rsu_ctx = &(p_rsu_tree_node->x2ap_rsu_ctx);
				if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_SEND_RSU )
				{
					/* Validate if eNB2 Measurement ID is existing. If Measurement ID is not existing report 
X2AP: RESOURCE STATUS FAILURE with cause as "UnknowneNBMeasurementID". 
eNB2 is the receiver of X2AP: RESOURCE STATUS REQUEST.*/
					if (p_x2ap_rsu_ctx->enb1_meas_id != p_trgt->eNB1_measurement_Id)
					{
						X2AP_TRACE(X2AP_ERROR,"X2AP: Resource Status request: UnknowneNBMeasurementID");
						x2ap_rsu_build_and_send_failure_msg(p_trgt->eNB1_measurement_Id,p_trgt->eNB2_measurement_Id,
								p_x2ap_gb_ctx,p_enb_ctx, X2AP_P_NULL,x2ap_unknown_eNB_Measurement_ID,p_x2ap_rsu_ctx);
						result = X2AP_FAILURE; 
					}
					else
					{
						/*Create context and then send the RESOURCE STATUS * (STOP) REQUEST TO RRM*/ 
						result = x2ap_rsu_build_and_send_rrm_stop_req(p_x2ap_gb_ctx,p_trgt->eNB2_measurement_Id); 
					}
				}
				else
				{
					X2AP_TRACE(X2AP_ERROR, "Invalid X2AP RSU State --> [%d]\n",p_x2ap_rsu_ctx->state);
					result = X2AP_FAILURE; 
				}
			}
			else
			{
				X2AP_TRACE(X2AP_ERROR,"X2AP: Resource Status request: UnknowneNBMeasurementID");
				x2ap_rsu_build_and_send_failure_msg(p_trgt->eNB1_measurement_Id,X2AP_RSU_MEASUREMENT_ID_MAX, /* SPR_5966 */
						p_x2ap_gb_ctx,p_enb_ctx, X2AP_P_NULL,x2ap_unknown_eNB_Measurement_ID,p_x2ap_rsu_ctx);
				result = X2AP_FAILURE; 
			}
		}
		return result;
	}
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_internal_encode_failure_msg 
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_buff      - pointer to ASN.1 encoded messag
 *                  p_buff_size - pointer to size of result buffer
 *                  p_resource_status_failure - pointer to 
 *                                             rrc_x2ap_rsu_failure_t
 * Outputs        : p_buff (ASN.1 encoded message)
 *                p_buff_size (size of result buffer)
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function used to encode resource status failure. 
 ********************************************************************************/
x2ap_return_et x2ap_rsu_internal_encode_failure_msg 
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_x2ap_rsu_failure_t  *p_resource_status_failure
 )
{
	x2ap_return_et                   result = X2AP_FAILURE;
	x2ap_X2AP_PDU                    x2ap_pdu;
	x2ap_ResourceStatusFailure  *p_x2ap_msg = PNULL;

	X2AP_UT_TRACE_ENTER();

	X2AP_ASSERT(PNULL != pctxt);
	X2AP_ASSERT(PNULL != p_buff);
	X2AP_ASSERT(PNULL != p_buff_size);
	X2AP_ASSERT(PNULL != p_resource_status_failure);

	do
	{
		/* Init X2AP PDU */
		asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
		/* Set Pdu type to Unsuccessful Outcome */
		x2ap_pdu.t = T_x2ap_X2AP_PDU_unsuccessfulOutcome;

		x2ap_pdu.u.unsuccessfulOutcome =
			rtxMemAllocType(pctxt, x2ap_UnsuccessfulOutcome);
		if (PNULL == x2ap_pdu.u.unsuccessfulOutcome)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			break;
		}

		asn1Init_x2ap_UnsuccessfulOutcome(x2ap_pdu.u.unsuccessfulOutcome);

		x2ap_pdu.u.unsuccessfulOutcome->procedureCode = ASN1V_x2ap_id_resourceStatusReportingInitiation; 

		x2ap_pdu.u.unsuccessfulOutcome->criticality = x2ap_reject;

		/* Set the initiating message type to resource status failure */
		x2ap_pdu.u.unsuccessfulOutcome->value.t = T1x2ap__resourceStatusReportingInitiation; 

		x2ap_pdu.u.unsuccessfulOutcome->value.u.resourceStatusReportingInitiation = rtxMemAllocType(pctxt,x2ap_ResourceStatusFailure);
		if (PNULL == x2ap_pdu.u.unsuccessfulOutcome->value.u.resourceStatusReportingInitiation)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			break;
		}
		p_x2ap_msg = x2ap_pdu.u.unsuccessfulOutcome->value.u.resourceStatusReportingInitiation;

		asn1Init_x2ap_ResourceStatusFailure(p_x2ap_msg);
		if (RT_OK != asn1Append_x2ap_ResourceStatusFailure_protocolIEs_1(
					pctxt,
					&p_x2ap_msg->protocolIEs,
					p_resource_status_failure->eNB1_measurement_Id))
		{
			break;
		}

		if (RT_OK != asn1Append_x2ap_ResourceStatusFailure_protocolIEs_2(
					pctxt,
					&p_x2ap_msg->protocolIEs,
					p_resource_status_failure->eNB2_measurement_Id))
		{
			break;
		}

		if (RT_OK != asn1Append_x2ap_ResourceStatusFailure_protocolIEs_3(
					pctxt,
					&p_x2ap_msg->protocolIEs,
					&p_resource_status_failure->cause))
		{
			break;
		}
		/*Need to be done later*/
		if (
				X2AP_RSU_FAILURE_CRIT_DIAGNO_PRESENT   
				&
				p_resource_status_failure->bitmask)
		{
			if (RT_OK != asn1Append_x2ap_ResourceStatusFailure_protocolIEs_4(
						pctxt,
						&p_x2ap_msg->protocolIEs,
						&p_resource_status_failure->criticality_diagnostics))
			{
				break;
			}
		}

		if (
				X2AP_RSU_COMP_FAILURE_INF_LIST   
				&
				p_resource_status_failure->bitmask)
		{
			if (RT_OK != asn1Append_x2ap_ResourceStatusFailure_protocolIEs_5(
						pctxt,
						&p_x2ap_msg->protocolIEs,
						&p_resource_status_failure->complete_failure_cause_inf_list))
			{
				break;
			}
		}

		/* ASN Encode message */
		pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

		if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
			break;
		}

		*p_buff_size = (U16)pe_GetMsgLen(pctxt);

		/* Print the asn encoded message */
		
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : Resource Status Request", &x2ap_pdu);

		result=X2AP_SUCCESS;
	} while(0);
	X2AP_UT_TRACE_EXIT();
	return result;
}
/*******************************************************************************
 * Function Name : x2ap_rsu_decode_start_msg
 * Inputs        : p_x2ap_gl_ctx - pointer to X2AP global context
 *                 p_enb_ctx - pointer to eNb context
 * Outputs       : Decoded message p_intrl
 * Returns       : X2AP_SUCCESS / X2AP_FAILURE
 * Description   : This function used to decode the RSU start Request received 
 *               by peer enb.
 ********************************************************************************/
x2ap_return_et x2ap_rsu_decode_start_msg
(
 x2ap_gb_context_t  *p_x2ap_gl_ctx,
 x2ap_peer_enb_context_t *p_enb_ctx,
 OSCTXT      *p_asn1_ctx,
 x2ap_ResourceStatusRequest       *p_3gpp,    /* unpacked src */
 rrc_x2ap_rsu_start_req_t    *p_intrl,    /* dest */
 U8          *rsu_periodicity_fg,
 rrc_x2ap_error_indication_t *p_error_indication
 )
{

	U32  index = X2AP_NULL;
	OSRTDListNode  *p_node     = PNULL;
	x2ap_ResourceStatusRequest_protocolIEs_element *p_protocol_IE = PNULL;
	x2ap_return_et       retVal = X2AP_SUCCESS;
	x2ap_error_ind_ie_list_t   iE_list;
	x2ap_error_ind_bool_t send_err_ind;
	U16 ie_list_index = 0;
	x2ap_bool_et send_tse = X2AP_FALSE;
	x2ap_message_data_t message_map =
	{7, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
		{{0, ASN1V_x2ap_id_ENB1_Measurement_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{1, ASN1V_x2ap_id_ENB2_Measurement_ID, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{2, ASN1V_x2ap_id_Registration_Request, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{3, ASN1V_x2ap_id_ReportCharacteristics, x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{4, ASN1V_x2ap_id_CellToReport, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{5, ASN1V_x2ap_id_ReportingPeriodicity, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{6, ASN1V_x2ap_id_PartialSuccessIndicator, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
		}};


	X2AP_UT_TRACE_ENTER();

	X2AP_ASSERT(PNULL != p_3gpp);
	X2AP_ASSERT(PNULL != p_intrl);

	X2AP_MEMSET(&iE_list, X2AP_NULL, sizeof(x2ap_error_ind_ie_list_t));
	X2AP_MEMSET(&send_err_ind, X2AP_FALSE, sizeof(x2ap_error_ind_bool_t));

	do
	{
		p_node = p_3gpp->protocolIEs.head;
		/* Save the decoded Protocol IEs */
		for (index =0; index < p_3gpp->protocolIEs.count; index++)
		{
			if (X2AP_P_NULL == p_node)
			{
				X2AP_TRACE(X2AP_ERROR,"%s: "
						"next node returned NULL",
						__FUNCTION__);
				break;
			}
			p_protocol_IE =
				(x2ap_ResourceStatusRequest_protocolIEs_element *)
				p_node->data;
			switch (p_protocol_IE->id)
			{
				case ASN1V_x2ap_id_ENB1_Measurement_ID:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									&p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_1))
						{
							p_intrl->eNB1_measurement_Id = p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_1;
						}
					}
					break;

				case ASN1V_x2ap_id_ENB2_Measurement_ID:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									&p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_2))
						{
							p_intrl->eNB2_measurement_Id = p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_2;
							p_intrl->bitmask |= X2AP_RSU_START_ENB2_MEAS_ID_PRESENT;
						}
					}
					break;

				case ASN1V_x2ap_id_Registration_Request:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									&p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_3))
						{
							p_intrl->registration_reqd = p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_3;
						}
					}
					break;
					/*Decoding IE 4 and 5*/
				case ASN1V_x2ap_id_ReportCharacteristics:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_4))
						{
							rrc_cp_unpack_U32(&(p_intrl->reportcharacteristics_bitmask),
									p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_4->data,
									"reportcharacteristics_bitmask"); 
							if (X2AP_NULL == p_intrl->reportcharacteristics_bitmask)
							{
								x2ap_update_message_map(&message_map,
										DATA_MISSING, index, p_protocol_IE->id);
								break;
							}
						}
					}
					break;
				case ASN1V_x2ap_id_CellToReport:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_5))
						{
							/*Calling the function to decode the cell to report list*/
							retVal = x2ap_rsu_decode_cell_to_report(p_intrl,
									p_protocol_IE, &send_tse);
						}
						/* This will only happen when the element list exceeds
						 * the max number */
						if (X2AP_TRUE == send_tse)
						{
							X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
									"in ASN.1 Sequence, Sending Transfer Syntax Error");
							x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
							retVal = X2AP_FAILURE;
							break;
						}
					}
					break;  
				case ASN1V_x2ap_id_ReportingPeriodicity:
					{ 
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									&(p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_6)))
						{
							p_intrl->rsu_periodicity = p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_6;
							*rsu_periodicity_fg = X2AP_TRUE;
						}
					}
					break;

				case ASN1V_x2ap_id_PartialSuccessIndicator: 
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									&(p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_7)))
						{
							p_intrl->bitmask |=
								X2AP_PARTIAL_SUCCESS_IND_PRESENT;
							p_intrl->partial_success_indicator = p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_7;
						}
					}    
					break;

				default:
					/* Class - 1 Error - Unknown IE */
					/* Base on Criticality fill the value */
					X2AP_TRACE(X2AP_ERROR, "%s:"\
							"invalid Protocol IE id",__FUNCTION__);
					x2ap_add_to_err_ind_ie_list(&iE_list,
							p_protocol_IE->criticality,
							p_protocol_IE->id,
							&ie_list_index,
							&send_err_ind,
							X2AP_FALSE,
							X2AP_FALSE);
					/*SPR_19067_FIX_START*/
                    if (x2ap_reject == p_protocol_IE->criticality) 
                    {
                        retVal = X2AP_FAILURE;
                    }
                    /*SPR_19067_FIX_STOP*/
			}
			p_node= p_node->next;
		}
	}while(0);
	/* Parse the map for Error Indication */
	if ((X2AP_SUCCESS == x2ap_parse_message_map(
					p_asn1_ctx,
					p_x2ap_gl_ctx,
					p_enb_ctx,
					&message_map,
					&iE_list,
					&ie_list_index,
					&send_err_ind,
					ASN1V_x2ap_id_resourceStatusReportingInitiation,
					T_x2ap_X2AP_PDU_initiatingMessage,
					x2ap_reject,
					p_error_indication))
	   )
	{
		X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED in rsu_decode_start_msg");

		retVal = X2AP_FAILURE;
	}

	X2AP_UT_TRACE_EXIT();

	return retVal;

}

/*******************************************************************************
 * Function Name : x2ap_rsu_decode_cell_to_report
 * Inputs        : pointer to rrc_x2ap_rsu_start_req_t 
 *                 pointer to x2ap_ResourceStatusRequest_protocolIEs_element
 * Outputs       : Decoded message p_intrl
 * Returns       : X2AP_SUCCESS / X2AP_FAILURE    
 * Description   : This function used to decode the cell report requested 
 *                 by peer enb.
 ********************************************************************************/
	x2ap_return_et 
x2ap_rsu_decode_cell_to_report(rrc_x2ap_rsu_start_req_t *p_x2ap_rsu_start_req,
		x2ap_ResourceStatusRequest_protocolIEs_element *p_protocol_IE,
		x2ap_bool_et *p_send_tse)
{
	U16 count = 0;
    /* SPR 18305 Fix Start */
    U8 num_bytes = 0;
    /* SPR 18305 Fix End */
	OSRTDListNode * p_node = PNULL;
	OSRTDList * p_cell_to_report_list = PNULL;
	p_cell_to_report_list = p_protocol_IE->value.u._x2ap_ResourceStatusRequest_IEs_5;
	p_node = p_cell_to_report_list->head;

	/*Storing the value of number of served cell*/
	p_x2ap_rsu_start_req->cell_to_report_id.num_served_cell = p_cell_to_report_list->count;

	/* SPR 13041 Fix Start */ 
	if(MAX_CELLIN_ENB < p_cell_to_report_list->count)
	{
		*p_send_tse = X2AP_TRUE;
		X2AP_UT_TRACE_EXIT();
		return X2AP_FAILURE; 
		/* SPR 13041 Fix Stop */
	}

	/* SPR 13041 Fix Start */
	if (MAX_SERVED_CELLS < p_x2ap_rsu_start_req->cell_to_report_id.num_served_cell)
	{
		X2AP_TRACE(X2AP_WARNING,"%s: ""Served Cells greater than 8 are not supported ",
				__FUNCTION__);
	}

	for(count = 0; (count< p_cell_to_report_list->count) &&
			(count < MAX_SERVED_CELLS) ; count++)/* SPR 13041 Fix Stop */
	{
		X2AP_ASSERT(PNULL != p_node);
		X2AP_ASSERT(PNULL != p_node->data);

		/* Copying PLMN identity */
		x2ap_parse_asn_plmn_identity(&p_x2ap_rsu_start_req->cell_to_report_id.cell_id[count].cell_info.plmn_identity,
				((x2ap_CellToReport_List_element *)(p_node->data))->value.cell_ID.pLMN_Identity.data);/*Need To Confirm*/

		/* Copying EUTRAN-ID */	
        /* SPR 18305 Fix Start */
        num_bytes = (((x2ap_CellToReport_List_element *)(p_node->data))->value.cell_ID.eUTRANcellIdentifier.numbits/8);

        if ((((x2ap_CellToReport_List_element *)(p_node->data))->value.cell_ID.eUTRANcellIdentifier.numbits%8))
        {
            /* In case of Remainder bits, one more octet needs to be copied */
            num_bytes += 1;
        }

		X2AP_MEMCPY(&p_x2ap_rsu_start_req->cell_to_report_id.cell_id[count].cell_info.cell_identity,
				&((x2ap_CellToReport_List_element *)(p_node->data))->value.cell_ID.eUTRANcellIdentifier.data,
				HOME_ENB_ID_OCTET_SIZE);/*Need To Confirm*/
        /* SPR 19768 Fix Start */
        p_x2ap_rsu_start_req->cell_to_report_id.cell_id[count].cell_info.cell_identity[num_bytes - 1] =
            (p_x2ap_rsu_start_req->cell_to_report_id.cell_id[count].cell_info.cell_identity[num_bytes - 1] & 0xF0);
        /* SPR 19768 Fix Stop */

		p_node = p_node->next;
	}
	return X2AP_SUCCESS;

}

/*******************************************************************************
 * FUNCTION NAME : x2ap_rsu_decode_measurement_intiation_list
 * DESCRIPTION   : This function used to decode the measurement initiation res
 *                 list sent by peer enb
 * INPUTS        : pointer to x2ap_MeasurementInitiationResult_List
 * OUTPUTS       : p_x2ap_rsu_measurement_res_list
 * RETURNS       : X2AP_SUCCESS / X2AP_FAILURE    
 ********************************************************************************/

	x2ap_return_et 
x2ap_rsu_decode_measurement_intiation_list(x2ap_MeasurementInitiationResult_List *p_measurement_initiation_result_list,
		x2ap_rrm_rsu_measurement_res_t *p_x2ap_rsu_measurement_res_list)
{
	U16 count = 0;
	U16 count1 = 0;
	U16 num_measurements = 0;
	OSRTDListNode * p_node = PNULL;
	OSRTDListNode * p_node_failure_list = PNULL;
	OSRTDList * p_measurement_failure_list = PNULL;
	p_node = p_measurement_initiation_result_list->head;

	/*Storing the value of number of served cell*/
	X2AP_MEMSET(p_x2ap_rsu_measurement_res_list,0,sizeof(x2ap_rrm_rsu_measurement_res_t));
	p_x2ap_rsu_measurement_res_list->num_cell = p_measurement_initiation_result_list->count;


	for(count = 0; count< p_measurement_initiation_result_list->count ; count++)
	{
		X2AP_ASSERT(PNULL != p_node);
		X2AP_ASSERT(PNULL != p_node->data);

		/* Copying PLMN identity */
		x2ap_parse_asn_plmn_identity(&p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].cell_info.plmn_identity,
				((x2ap_MeasurementInitiationResult_List_element *)(p_node->data))->value.cell_ID.pLMN_Identity.data);/*Need To Confirm*/

		/* Copying EUTRAN-ID */	
		X2AP_MEMCPY(&p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].cell_info.cell_identity,
				((x2ap_MeasurementInitiationResult_List_element *)(p_node->data))->value.cell_ID.eUTRANcellIdentifier.data,
				((x2ap_MeasurementInitiationResult_List_element *)(p_node->data))->value.cell_ID.eUTRANcellIdentifier.numbits/8);/*Need To Confirm*/
		if(((x2ap_MeasurementInitiationResult_List_element *)(p_node->data))->value.m.measurementFailureCause_ListPresent)
		{
			p_measurement_failure_list = &((x2ap_MeasurementInitiationResult_List_element *)(p_node->data))->value.measurementFailureCause_List;
			p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].bitmask |= X2AP_RRM_FAILED_CAUSE_ITEM_PRESENT; 
			if(PNULL != p_measurement_failure_list)
			{
				num_measurements = p_measurement_failure_list->count;
			}
			/* Integration FIX start*/
			p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].bitmask |= 
				X2AP_RRM_FAILED_CAUSE_NUM_ITEM_PRESENT; 
			/* Integration FIX stop*/
			p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].num_measurement = num_measurements;
			if(PNULL != p_measurement_failure_list)
			{
				p_node_failure_list = p_measurement_failure_list->head;
			}
			for(count1 = 0; count1 < num_measurements; count1++)
			{
				X2AP_ASSERT(PNULL != p_node_failure_list);
				X2AP_ASSERT(PNULL != p_node_failure_list->data);
                /* SPR 20674 Fix Start */
                /* Code Deleted */
				rrc_cp_unpack_U32(&p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].
                measurement_failure_cause_item[count1].report_characteristics,
                ((x2ap_MeasurementFailureCause_List_element *)(p_node_failure_list->data))->value.
                measurementFailedReportCharacteristics.data,"reportcharacteristics_bitmask"); 
                /* SPR 20674 Fix End */
				p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].measurement_failure_cause_item[count1].rrm_cause.value = ((x2ap_MeasurementFailureCause_List_element *)(p_node_failure_list->data))->value.cause.u.radioNetwork;
				p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].measurement_failure_cause_item[count1].rrm_cause.type  = ((x2ap_MeasurementFailureCause_List_element *)(p_node_failure_list->data))->value.cause.t;
				p_node_failure_list = p_node_failure_list->next;
			}
		} 
		p_node = p_node->next;
	}
	return X2AP_SUCCESS;

}

/*******************************************************************************
 * FUNCTION NAME : x2ap_rsu_decode_complete_failure_list
 * DESCRIPTION   : This function used to decode the complete failure
 *                 list sent by peer enb
 * INPUTS        : pointer to x2ap_CompleteFailureCauseInformation_List
 * OUTPUTS       : p_x2ap_rsu_measurement_res_list
 * RETURNS       : X2AP_SUCCESS / X2AP_FAILURE    
 ********************************************************************************/

	x2ap_return_et 
x2ap_rsu_decode_complete_failure_list(x2ap_CompleteFailureCauseInformation_List *p_complete_failure_cause_inf_list,
		x2ap_rrm_rsu_measurement_res_t *p_x2ap_rsu_measurement_res_list)
{
	U16 count = 0;
	U16 count1 = 0;
	U16 num_measurements = 0;
	OSRTDListNode * p_node = PNULL;
	OSRTDListNode * p_node_failure_list = PNULL;
	OSRTDList * p_measurement_failure_list = PNULL;
	p_node = p_complete_failure_cause_inf_list->head;
	X2AP_MEMSET(p_x2ap_rsu_measurement_res_list,0,sizeof(x2ap_rrm_rsu_measurement_res_t));

	/*Storing the value of number of served cell*/
	p_x2ap_rsu_measurement_res_list->num_cell = p_complete_failure_cause_inf_list->count;


	for(count = 0; count < p_complete_failure_cause_inf_list->count ; count++)
	{
		X2AP_ASSERT(PNULL != p_node);
		X2AP_ASSERT(PNULL != p_node->data);

		x2ap_parse_asn_plmn_identity(&p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].cell_info.plmn_identity,
				((x2ap_CompleteFailureCauseInformation_List_element *)(p_node->data))->value.cell_ID.pLMN_Identity.data);/*Need To Confirm*/

		X2AP_MEMCPY(&p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].cell_info.cell_identity,
				&((x2ap_CompleteFailureCauseInformation_List_element *)(p_node->data))->value.cell_ID.eUTRANcellIdentifier.data,
				((x2ap_CompleteFailureCauseInformation_List_element *)(p_node->data))->value.cell_ID.eUTRANcellIdentifier.numbits/8);/*Need To Confirm*/
		p_measurement_failure_list = &((x2ap_CompleteFailureCauseInformation_List_element *)(p_node->data))->value.measurementFailureCause_List;
		if(p_measurement_failure_list->count)
		{
			p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].bitmask |= X2AP_RRM_FAILED_CAUSE_ITEM_PRESENT; 
			num_measurements = p_measurement_failure_list->count;
			/*R10 FIX START*/
			p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].bitmask |= X2AP_RRM_FAILED_CAUSE_NUM_ITEM_PRESENT; 
			/*R10 FIX END*/
			p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].num_measurement = num_measurements;
		}
		p_node_failure_list = p_measurement_failure_list->head;
		for(count1 = 0; count1 < num_measurements; count1++)
		{
			X2AP_ASSERT(PNULL != p_node_failure_list);
			X2AP_ASSERT(PNULL != p_node_failure_list->data);
            /* SPR 20674 Fix Start */
            /* Code Deleted */
			rrc_cp_unpack_U32(&p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].
            measurement_failure_cause_item[count1].report_characteristics,
            ((x2ap_MeasurementFailureCause_List_element *)(p_node_failure_list->data))->value.
            measurementFailedReportCharacteristics.data,"reportcharacteristics_bitmask"); 
            /* SPR 20674 Fix End */
			p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].measurement_failure_cause_item[count1].rrm_cause.value = ((x2ap_MeasurementFailureCause_List_element *)(p_node_failure_list->data))->value.cause.u.radioNetwork;
			p_x2ap_rsu_measurement_res_list->rsu_measurement_res_item[count].measurement_failure_cause_item[count1].rrm_cause.type  = ((x2ap_MeasurementFailureCause_List_element *)(p_node_failure_list->data))->value.cause.t;
			p_node_failure_list = p_node_failure_list->next;
		}
		p_node = p_node->next;
	}
	return X2AP_SUCCESS;

}

/*****************************************************************************
 * Function Name  : x2ap_rsu_build_and_send_rrm_start_req 
 * Inputs         : p_x2ap_start_req - pointer to p_x2ap_start_req
 *                  p_x2ap_gb_ctx - pointer to x2ap global context
 *                  p_enb_ctx - pointer to peer eNB context
 * Outputs        : p_x2ap_start_req
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function used to build and send the resource status
 *                  request to RRM module
 ********************************************************************************/
x2ap_return_et x2ap_rsu_build_and_send_rrm_start_req(  rrc_x2ap_rsu_start_req_t    *p_x2ap_start_req,    /* dest */
		x2ap_gb_context_t *p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t *p_enb_ctx)      /* X2ap global context*/
{
	/* SPR 22310 : CID 115964 Fix Start */
	x2ap_timer_t timer_id = X2AP_NULL;
	/* SPR 22310 : CID 115964 Fix End */
	rsu_enb_meas_id_search_node_t* p_insert_node = X2AP_P_NULL;
	U32 enb_meas_Id = X2AP_NULL;
	U16 cell_counter = 0;
	x2ap_return_et retVal = X2AP_FAILURE;
	x2ap_rrm_rsu_enb_start_req_t *p_x2ap_rrm_rsu_enb_start_req = X2AP_P_NULL;

	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);
	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_start_req);
	X2AP_ASSERT(X2AP_P_NULL != p_enb_ctx);


	p_x2ap_rrm_rsu_enb_start_req = (x2ap_rrm_rsu_enb_start_req_t*)x2ap_mem_get(sizeof(x2ap_rrm_rsu_enb_start_req_t));
	if (X2AP_P_NULL == p_x2ap_rrm_rsu_enb_start_req)
	{
		X2AP_TRACE(X2AP_FATAL,"\nMemory allocation failed.");
		return X2AP_FAILURE;
	}

	X2AP_MEMSET(p_x2ap_rrm_rsu_enb_start_req,0,sizeof(x2ap_rrm_rsu_enb_start_req_t));

	/*Create Measurement ID and Measurement Context. */
	/*get a measurement ID from the pool */

	enb_meas_Id = x2ap_rsu_get_meas_id(p_x2ap_gb_ctx);
	if ( X2AP_RSU_MEASUREMENT_ID_INVALID == enb_meas_Id)
	{
		X2AP_TRACE(X2AP_WARNING,"\nInValid MeasureMent ID ");
		/* coverity_fix_63253_start */
		x2ap_mem_free(p_x2ap_rrm_rsu_enb_start_req);
		/* coverity_fix_63253_stop */
		return X2AP_FAILURE;
	}

	/*Filling the information*/
	p_x2ap_rrm_rsu_enb_start_req->report_characteristics = p_x2ap_start_req->reportcharacteristics_bitmask;

	p_x2ap_rrm_rsu_enb_start_req->enb_meas_id = enb_meas_Id;

	p_x2ap_rrm_rsu_enb_start_req->num_cell = p_x2ap_start_req->cell_to_report_id.num_served_cell;

	p_x2ap_rrm_rsu_enb_start_req->peer_gb_enb_id = p_enb_ctx->gb_enb_id;

	for (cell_counter = 0; cell_counter < p_x2ap_rrm_rsu_enb_start_req->num_cell; cell_counter++)
	{
		X2AP_MEMCPY(&p_x2ap_rrm_rsu_enb_start_req->cell_info[cell_counter].plmn_identity,&p_x2ap_start_req->cell_to_report_id.cell_id[cell_counter].cell_info.plmn_identity,sizeof(plmn_identity_t));

		X2AP_MEMCPY(&p_x2ap_rrm_rsu_enb_start_req->cell_info[cell_counter].cell_identity,&p_x2ap_start_req->cell_to_report_id.cell_id[cell_counter].cell_info.cell_identity,HOME_ENB_ID_OCTET_SIZE);

	}

	p_x2ap_rrm_rsu_enb_start_req->report_periodicity =
		p_x2ap_start_req->rsu_periodicity ;
	p_insert_node = x2ap_mem_get(sizeof(rsu_enb_meas_id_search_node_t));
	if(X2AP_P_NULL == p_insert_node)
	{
		X2AP_TRACE(X2AP_FATAL,"\nMemeory allocation failed ");
		/* coverity_fix_63253_start */
		x2ap_mem_free(p_x2ap_rrm_rsu_enb_start_req);
		/* coverity_fix_63253_stop */
		return X2AP_FAILURE;
	}
	/* SPR 7573 -RSU fix */
	X2AP_MEMSET(p_insert_node,0,sizeof(rsu_enb_meas_id_search_node_t));
	/* SPR 7573 -RSU fix */
	p_insert_node->x2ap_rsu_ctx.peer_enb_id = p_enb_ctx->peer_enodeb_id;
	/* SPR_5966 */
	p_insert_node->x2ap_rsu_ctx.self_meas_id = enb_meas_Id;
	p_insert_node->x2ap_rsu_ctx.enb2_meas_id = enb_meas_Id;
	p_insert_node->x2ap_rsu_ctx.src_or_trg_meas_id = TRG_MEAS_ID;

	if(p_x2ap_start_req->bitmask & X2AP_PARTIAL_SUCCESS_IND_PRESENT)
	{
		p_insert_node->x2ap_rsu_ctx.partial_success_indicator_present = 1;
		p_x2ap_rrm_rsu_enb_start_req->bitmask |=
			ENB_START_PARTIAL_SUCCESS;
		p_x2ap_rrm_rsu_enb_start_req->partial_success_indicator_present = p_x2ap_start_req->partial_success_indicator;
	}


	/* 5. c. Set Measurement ID of Peer ENB as Invalid. */
	p_insert_node->x2ap_rsu_ctx.enb1_meas_id = p_x2ap_start_req->eNB1_measurement_Id; 
	/* SPR_5966 */
	p_insert_node->x2ap_rsu_ctx.rrm_transaction_id = 0; /*transaction_Id;  Need to confirm*/
	p_insert_node->x2ap_rsu_ctx.state = X2AP_RSU_STATE_START_ONGOING;

	/* Send Message to RRM */
	retVal = rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_start_req(p_x2ap_rrm_rsu_enb_start_req,
			RRC_X2AP_MODULE_ID,
			RRC_RRM_MODULE_ID, 
			0/*trans_id*/,
			RRC_NULL);
	if (X2AP_FAILURE == retVal)
	{
		X2AP_TRACE(X2AP_ERROR," X2AP : RESOURCE STATUS START REQUEST not sent to RRM(2)");
	}

	/*Start the timer*/
	if (X2AP_SUCCESS == x2ap_rsu_start_timer(X2AP_RSU_RRM_GUARD_TIMER,
				p_enb_ctx,
				p_x2ap_gb_ctx->config_param.timer_conf_info.
				x2ap_rsu_rrm_guard_tim_val,
				&timer_id,
				p_insert_node->x2ap_rsu_ctx.enb2_meas_id))
	{
		X2AP_TRACE(X2AP_INFO,
				"X2AP_RSU_RRM_GUARD_TIMER Started Successfully");
	}

	/*store the timer id in measurement context*/
	p_insert_node->x2ap_rsu_ctx.timer_id = timer_id;
	p_insert_node->x2ap_rsu_ctx.timer_type = X2AP_RSU_RRM_GUARD_TIMER;

	/*Insert the node in the tree*/
	x2ap_st_insert_node(&p_x2ap_gb_ctx->x2ap_rsu_context_tree,&p_insert_node->nodeAnchor);

	/* coverity_fix_63253_start */
	x2ap_mem_free(p_x2ap_rrm_rsu_enb_start_req);
	/* coverity_fix_63253_stop */
	return retVal;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_enb_timer_expiry_evt_hdl 
 * Inputs         : p_timer_buf - pointer to timer buffer data
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS or X2AP_SUCCESS
 * Description    : This function handles the X2AP_RSU_ENB_GUARD_TIMER expiry
 *         message
 ********************************************************************************/
x2ap_return_et x2ap_rsu_enb_timer_expiry_evt_hdl(x2ap_timer_buffer_t *p_timer_buf ,
		x2ap_gb_context_t *p_x2ap_gb_ctx)
{
	rsu_enb_meas_id_search_node_t* p_rsu_tree_node = X2AP_P_NULL;   
	U8 counter = X2AP_NULL;

	x2ap_rsu_context_t*     p_x2ap_rsu_ctx = X2AP_P_NULL;
	x2ap_peer_enb_context_t *p_enb_ctx = X2AP_P_NULL;

	/* Get tree node */ 
   /* SPR 21515 Fix Start */
    U32 meas_id = RRC_NULL;
    meas_id = *((U32 *)(p_timer_buf->p_timer_specific_data)); 
	/* Get tree node */ 
	X2AP_TRACE(X2AP_INFO,"[%s] p_timer_specific_data %d \t %d",	__FUNCTION__,*((U32 *)(p_timer_buf->p_timer_specific_data)), meas_id);
	p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),
			&meas_id);
   /* SPR 21515 Fix End */
	if(X2AP_P_NULL != p_rsu_tree_node)
	{
		/* get the measurement context */
		p_x2ap_rsu_ctx = &p_rsu_tree_node->x2ap_rsu_ctx;       
		for(counter = 0 ; counter < MAX_PEER_ENB ; counter++)
		{
			/* Bug:9129 */
			if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].peer_enodeb_id == p_x2ap_rsu_ctx->peer_enb_id)
					&& (p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].op_status == OPERATIONAL))
				/* Bug:9129 */
			{
				p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
				break;
			}
			else
				continue;
		}
		if(X2AP_P_NULL == p_enb_ctx)
		{
			X2AP_TRACE(X2AP_ERROR, "[%s]: Peer Enb Context is NULL while fetching from x2ap global context\n", __FUNCTION__);
			return X2AP_FAILURE;
		}
	}
	else
	{
		X2AP_TRACE(X2AP_ERROR, "[%s]: Measurement Context does Not exist for this Measurement ID -- Ignoring Message\n", __FUNCTION__);
		return X2AP_FAILURE;
	}
	if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_START_ONGOING) 
	{
		/* send the RESOURCE STATUS REQUEST (START) Failure to RRM */ 
		x2ap_rsu_send_rrm_start_resp(p_x2ap_rsu_ctx->rrm_transaction_id,
				X2AP_FAILURE, p_x2ap_rsu_ctx->enb1_meas_id, x2ap_unspecified_2, p_enb_ctx /*x2ap_rsu_gb_enb_id_fix*/
				,X2AP_P_NULL,X2AP_NULL);

		/*Delete the context from tree*/
		x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),&(p_rsu_tree_node->nodeAnchor)); 
    /* SPR 21515 Fix Start */
    /* Stop the timer X2AP_RSU_RRM_GUARD_TIMER. */
		x2ap_stop_timer(p_x2ap_rsu_ctx->timer_id);
		p_x2ap_rsu_ctx->timer_id = X2AP_P_NULL; 
    /* SPR 21515 Fix End */
 
		x2ap_mem_free(&p_rsu_tree_node->nodeAnchor);
		return X2AP_SUCCESS;
	}
	else if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_STOP_ONGOING)
	{
		/* send the RESOURCE STATUS REQUEST (STOP) Failure to RRM */ 
		x2ap_rsu_build_and_send_rrm_stop_resp(p_x2ap_rsu_ctx->rrm_transaction_id,
				X2AP_FAILURE,p_x2ap_rsu_ctx->enb1_meas_id,p_enb_ctx->gb_enb_id,x2ap_unspecified_2);

		/*Delete the context from tree*/
		x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),&(p_rsu_tree_node->nodeAnchor)); 
		x2ap_mem_free(&p_rsu_tree_node->nodeAnchor);
		return X2AP_SUCCESS;

	}    

	else
	{
		X2AP_TRACE(X2AP_ERROR, "X2AP: TIMER EXPIRY EVENT OCCURED IN Invalid State -- [%d]", p_x2ap_rsu_ctx->state);
		return X2AP_FAILURE;   
	}
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_rrm_timer_expiry_evt_hdl 
 * Inputs         : p_timer_buf - pointer to timer buffer data
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS or X2AP_SUCCESS 
 * Description    : This function handles the X2AP_RSU_RRM_GUARD_TIMER expiry 
 *                  message
 ********************************************************************************/
x2ap_return_et x2ap_rsu_rrm_timer_expiry_evt_hdl(x2ap_timer_buffer_t *p_timer_buf , 
		x2ap_gb_context_t *p_x2ap_gb_ctx)
{

	rsu_enb_meas_id_search_node_t* p_rsu_tree_node = X2AP_P_NULL;   

	x2ap_rsu_context_t*     p_x2ap_rsu_ctx = X2AP_P_NULL;
	x2ap_peer_enb_context_t *p_enb_ctx = X2AP_P_NULL;
	U8 counter = X2AP_NULL;
	/* Get tree node */ 
	p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),
			(p_timer_buf->p_timer_specific_data));

	if(X2AP_P_NULL != p_rsu_tree_node)
	{
		/* get the measurement context */
		p_x2ap_rsu_ctx = &p_rsu_tree_node->x2ap_rsu_ctx;       
		for(counter = 0 ; counter < MAX_PEER_ENB; counter++)
		{
			/* Bug:9129 */
			if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].peer_enodeb_id == p_x2ap_rsu_ctx->peer_enb_id)
					&& (p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].op_status == OPERATIONAL))
				/* Bug:9129 */
			{
				p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
				break;
			}
		}    
		if(X2AP_P_NULL == p_enb_ctx)
		{
			X2AP_TRACE(X2AP_ERROR, "[%s]: Peer Enb Context is NULL while fetching from x2ap global context\n", __FUNCTION__);
			return X2AP_FAILURE;
		}
	}
	else
	{
		X2AP_TRACE(X2AP_ERROR, "[%s]: No measurement context exist for this timer expiry --- X2AP_RSU_RRM_GUARD_TIMER\n", __FUNCTION__);
		return X2AP_FAILURE;   
	}

	/* Check the state and Send Appropriate message */
	if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_START_ONGOING)
	{
		/* send the RESOURCE STATUS FAILURE MESSAGE TO PEER eNb */ 
		x2ap_rsu_build_and_send_failure_msg(p_x2ap_rsu_ctx->enb1_meas_id,p_x2ap_rsu_ctx->enb2_meas_id,
				p_x2ap_gb_ctx, p_enb_ctx,  X2AP_P_NULL,x2ap_unspecified_2,p_x2ap_rsu_ctx);

       /* SPR 15858 Fix Start */
       /* send the RESOURCE STATUS REQUEST (START) Failure to RRM */ 
       x2ap_rsu_send_rrm_start_resp(p_x2ap_rsu_ctx->rrm_transaction_id,
               X2AP_FAILURE, p_x2ap_rsu_ctx->enb1_meas_id, x2ap_unspecified_2, p_enb_ctx /*x2ap_rsu_gb_enb_id_fix*/
               ,X2AP_P_NULL,X2AP_NULL);

       /* SPR 15858 Fix Stop */

		/*Delete the context from tree*/
		x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),&(p_rsu_tree_node->nodeAnchor)); 
		x2ap_mem_free(&p_rsu_tree_node->nodeAnchor);
	}
	else if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_STOP_ONGOING)
	{
		/* send the RESOURCE STATUS FAILURE MESSAGE TO PEER eNb */ 
		x2ap_rsu_build_and_send_failure_msg(p_x2ap_rsu_ctx->enb1_meas_id,p_x2ap_rsu_ctx->enb2_meas_id,
				p_x2ap_gb_ctx, p_enb_ctx,  X2AP_P_NULL,x2ap_measurement_temporarily_not_available,p_x2ap_rsu_ctx);

       /* SPR 15858 Fix Start */
       /* send the RESOURCE STATUS REQUEST (STOP) Failure to RRM */ 
       x2ap_rsu_build_and_send_rrm_stop_resp(p_x2ap_rsu_ctx->rrm_transaction_id,
               X2AP_FAILURE,p_x2ap_rsu_ctx->enb1_meas_id,p_enb_ctx->gb_enb_id,x2ap_unspecified_2);
       /* SPR 15858 Fix Stop */

		/*Delete the context from tree*/
		x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),&(p_rsu_tree_node->nodeAnchor)); 
		x2ap_mem_free(&p_rsu_tree_node->nodeAnchor);
	}
	return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_process_rrm_rsu_start_res
 * Inputs         : p_api - pointer to input api data
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 * Outputs        : None 
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS 
 * Description    : This function process the X2AP_RRM_RSU_RRM_RES received 
 *                  from the rrm
 ********************************************************************************/
x2ap_return_et x2ap_process_rrm_rsu_start_res( void             *p_api,
		x2ap_gb_context_t   *p_x2ap_gb_ctx)
{
	x2ap_rrm_rsu_rrm_start_res_t *p_x2ap_rrm_rsu_rrm_res;
	S32 length_read = X2AP_NULL;
	x2ap_return_et response = X2AP_FAILURE;
	U8 counter = X2AP_NULL;

	/* Get tree node */ 
	rsu_enb_meas_id_search_node_t* p_rsu_tree_node = X2AP_P_NULL;   
	x2ap_rsu_context_t*     p_x2ap_rsu_ctx = X2AP_P_NULL;

	x2ap_peer_enb_context_t *p_enb_ctx = X2AP_P_NULL;

	X2AP_UT_TRACE_ENTER();
	X2AP_ASSERT(X2AP_P_NULL != p_api);
	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

	p_x2ap_rrm_rsu_rrm_res = (x2ap_rrm_rsu_rrm_start_res_t*)
		x2ap_mem_get(sizeof(x2ap_rrm_rsu_rrm_start_res_t));
	if(X2AP_P_NULL == p_x2ap_rrm_rsu_rrm_res)
	{
		X2AP_TRACE(X2AP_FATAL,"\nMemeory allocation failed ");
		return X2AP_FAILURE;
	}

	/* Decode the received message from RRM */
	response = rrc_il_parse_x2ap_rrm_rsu_rrm_start_res(
			p_x2ap_rrm_rsu_rrm_res,
			(U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
			x2ap_get_api_buf_size(p_api) - 
			RRC_FULL_INTERFACE_HEADERS_SIZE,
			&length_read);

	if(X2AP_SUCCESS == response)
	{

		/*Get the mesurement id from the context*/
		p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),
				&(p_x2ap_rrm_rsu_rrm_res->enb_meas_id));
		if(X2AP_P_NULL != p_rsu_tree_node)
		{
			/* get the measurement context */
			p_x2ap_rsu_ctx = &p_rsu_tree_node->x2ap_rsu_ctx;
			if(p_x2ap_rrm_rsu_rrm_res->bitmask & ENB_START_PARTIAL_SUCCESS)
			{
				X2AP_MEMCPY(&p_x2ap_rsu_ctx->x2ap_rsu_measurement_res_list,&p_x2ap_rrm_rsu_rrm_res->rrm_rsu_measurement_res_list,sizeof(p_x2ap_rrm_rsu_rrm_res->rrm_rsu_measurement_res_list));
				p_x2ap_rsu_ctx->rrm_rsu_measurement_res_list_present = 1;
			}
			for(counter = 0 ; counter < MAX_PEER_ENB; counter++)
			{
				/* SPR 7573 RSU fix */
				if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].peer_enodeb_id
							== p_x2ap_rsu_ctx->peer_enb_id) && 
						(p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].op_status == OPERATIONAL))
					/* SPR 7573 RSU fix */
				{
					p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
					break;
				}
			}    
		}
		if(X2AP_P_NULL == p_enb_ctx)
		{
			X2AP_TRACE(X2AP_ERROR, "[%s]: No measurement/peer enb context for this meas ID\n", __FUNCTION__);
			/* coverity_fix_63257_start */
			x2ap_mem_free(p_x2ap_rrm_rsu_rrm_res);
			/* coverity_fix_63257_stop */
			return X2AP_FAILURE;   
		}

		/* Stop the timer X2AP_RSU_RRM_GUARD_TIMER. */
		x2ap_stop_timer(p_x2ap_rsu_ctx->timer_id);
		p_x2ap_rsu_ctx->timer_id = X2AP_P_NULL; 

		/*Check the state of RSU SM*/
		if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_START_ONGOING)
		{
			X2AP_TRACE(X2AP_INFO, "x2ap rsu response received from RRM in RSU [%d] State\n",p_x2ap_rsu_ctx->state);

			if (p_x2ap_rrm_rsu_rrm_res->response == X2AP_SUCCESS) 
			{
				X2AP_TRACE(X2AP_ERROR, "[%s]: Rsu Start response received from RRM : SUCCESS ", __FUNCTION__);
				/*Build and send the X2AP: Resource Status response to Peer Enb */
				x2ap_rsu_build_and_send_response_msg(p_x2ap_gb_ctx,p_x2ap_rsu_ctx,p_enb_ctx,X2AP_P_NULL);

				/* Move RSU_SM to X2AP_RSU_STATE_SEND_RSU. */
				p_x2ap_rsu_ctx->state = X2AP_RSU_STATE_SEND_RSU;
			}
			else if (p_x2ap_rrm_rsu_rrm_res->response == X2AP_FAILURE)
			{
				X2AP_TRACE(X2AP_ERROR, "[%s]: Rsu Start response received from RRM : FAILURE ", __FUNCTION__);
				/*Build and send the X2AP: Resource Status response to Peer Enb */
				x2ap_rsu_build_and_send_failure_msg(p_x2ap_rsu_ctx->enb1_meas_id,p_x2ap_rsu_ctx->enb2_meas_id,
						p_x2ap_gb_ctx,p_enb_ctx, X2AP_P_NULL,p_x2ap_rrm_rsu_rrm_res->rrm_cause.value,p_x2ap_rsu_ctx);

				/*Delete Measurement Context*/
				x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),&(p_rsu_tree_node->nodeAnchor));
				x2ap_mem_free(&p_rsu_tree_node->nodeAnchor);

				/* Move RSU_SM to X2AP_RSU_STATE_IDLE.*/
				/* To be done/confirm */
			}      
		}
		else
		{
			X2AP_TRACE(X2AP_ERROR, "x2ap rsu response received from RRM in Invalid State [%d]\n", p_x2ap_rsu_ctx->state);
			/* coverity_fix_63257_start */
            response = X2AP_FAILURE;
			/* coverity_fix_63257_stop */
		}    
	}
	else
	{
		X2AP_TRACE(X2AP_WARNING, "X2AP_RRM_RSU_RRM_RES decode failed");
		/* coverity_fix_63257_start */
        response = X2AP_FAILURE;
		/* coverity_fix_63257_stop */
	}
	/* coverity_fix_63257_start */
	x2ap_mem_free(p_x2ap_rrm_rsu_rrm_res);
	/* coverity_fix_63257_stop */
	return response;
}

/*****************************************************************************
 * Function Name  : x2ap_process_rrm_rsu_stop_res 
 * Inputs         : p_api - pointer to input api data
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 * Outputs        : None 
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS
 * Description    : This function process the X2AP_RRM_RSU_RRM_STOP_RES received
 *                  from the rrm
 ********************************************************************************/
x2ap_return_et x2ap_process_rrm_rsu_stop_res( void             *p_api,
		x2ap_gb_context_t   *p_x2ap_gb_ctx)
{
	x2ap_rrm_rsu_rrm_stop_res_t *p_x2ap_rrm_rsu_rrm_res;
	S32 length_read = X2AP_NULL;
	x2ap_return_et response = X2AP_FAILURE;
	U8  counter = X2AP_NULL;

	/* Get tree node */ 
	rsu_enb_meas_id_search_node_t* p_rsu_tree_node = X2AP_P_NULL;   
	x2ap_rsu_context_t*     p_x2ap_rsu_ctx = X2AP_P_NULL;

	x2ap_peer_enb_context_t *p_enb_ctx = X2AP_P_NULL;

	X2AP_UT_TRACE_ENTER();
	X2AP_ASSERT(X2AP_P_NULL != p_api);
	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

	p_x2ap_rrm_rsu_rrm_res = (x2ap_rrm_rsu_rrm_stop_res_t*)
		x2ap_mem_get(sizeof(x2ap_rrm_rsu_rrm_stop_res_t));
	if(X2AP_P_NULL == p_x2ap_rrm_rsu_rrm_res)
	{
		X2AP_TRACE(X2AP_FATAL,"\nMemeory allocation failed ");
		return X2AP_FAILURE;
	}

	/* Decode the received message from RRM */
	response = rrc_il_parse_x2ap_rrm_rsu_rrm_stop_res(
			p_x2ap_rrm_rsu_rrm_res,
			(U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
			x2ap_get_api_buf_size(p_api) - 
			RRC_FULL_INTERFACE_HEADERS_SIZE,
			&length_read);

	if(X2AP_SUCCESS == response)
	{

		/*Get the mesurement id from the context*/
		p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),
				&(p_x2ap_rrm_rsu_rrm_res->enb_meas_id));
		if(X2AP_P_NULL != p_rsu_tree_node)
		{
			/* get the measurement context */
			p_x2ap_rsu_ctx = &p_rsu_tree_node->x2ap_rsu_ctx;
			for(counter = 0 ; counter < MAX_PEER_ENB; counter++)
			{
				/* SPR 7573 RSU fix */
				if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].peer_enodeb_id
							== p_x2ap_rsu_ctx->peer_enb_id) && 
						(p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].op_status == OPERATIONAL))
					/* SPR 7573 RSU fix */
				{
					p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
					break;
				}
			} 
			if(X2AP_P_NULL == p_enb_ctx)
			{
				X2AP_TRACE(X2AP_ERROR, "[%s]:Peer Enb Context is NULL while fetching from x2ap global context\n", __FUNCTION__);
				/* coverity_fix_63259_start */
				x2ap_mem_free(p_x2ap_rrm_rsu_rrm_res);
				/* coverity_fix_63259_stop */
				return X2AP_FAILURE;
			}

			/* Stop the timer X2AP_RSU_RRM_GUARD_TIMER. */
			x2ap_stop_timer(p_x2ap_rsu_ctx->timer_id);
			p_x2ap_rsu_ctx->timer_id = X2AP_P_NULL; 
		}
		else
		{
			X2AP_TRACE(X2AP_ERROR, "[%s]:No measurement context exist for this meas ID\n", __FUNCTION__);
			/* coverity_fix_63259_start */
			x2ap_mem_free(p_x2ap_rrm_rsu_rrm_res);
			/* coverity_fix_63259_stop */
			return X2AP_FAILURE;   
		}
		if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_STOP_ONGOING)
		{
			X2AP_TRACE(X2AP_INFO, "x2ap rsu response received fromm RRM in RSU [%d] State\n",p_x2ap_rsu_ctx->state);
			if (X2AP_SUCCESS == p_x2ap_rrm_rsu_rrm_res->response) 
			{

				/*Build and send the X2AP: Resource Status response to Peer Enb */
				x2ap_rsu_build_and_send_response_msg(p_x2ap_gb_ctx,p_x2ap_rsu_ctx,p_enb_ctx,X2AP_P_NULL);

				/*Delete Measurement Context*/
				x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),&(p_rsu_tree_node->nodeAnchor));
				x2ap_mem_free(&p_rsu_tree_node->nodeAnchor);

			}
			else if (X2AP_FAILURE == p_x2ap_rrm_rsu_rrm_res->response)
			{
				/* Build and send the X2AP: Resource Status Failure to Peer Enb*/ 
				x2ap_rsu_build_and_send_failure_msg(p_x2ap_rsu_ctx->enb1_meas_id,p_x2ap_rsu_ctx->enb2_meas_id,
						p_x2ap_gb_ctx,p_enb_ctx,X2AP_P_NULL,p_x2ap_rrm_rsu_rrm_res->rrm_cause.value,p_x2ap_rsu_ctx);
			}
		}    
		else
		{
			X2AP_TRACE(X2AP_ERROR, "x2ap rsu response received from RRM in Invalid State [%d]\n", p_x2ap_rsu_ctx->state);
			/* coverity_fix_63259_start */
			x2ap_mem_free(p_x2ap_rrm_rsu_rrm_res);
			/* coverity_fix_63259_stop */
			return X2AP_FAILURE;
		}    
	}
	else
	{
		X2AP_TRACE(X2AP_WARNING, "X2AP_RRM_RSU_RRM_RES decode failed");
		/* coverity_fix_63259_start */
		x2ap_mem_free(p_x2ap_rrm_rsu_rrm_res);
		/* coverity_fix_63259_stop */
		return X2AP_FAILURE;
	}
	/* coverity_fix_63259_start */
	x2ap_mem_free(p_x2ap_rrm_rsu_rrm_res);
	/* coverity_fix_63259_stop */
	return response;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_build_and_send_response_msg 
 * Inputs         : p_x2ap_gb_ctx - pointer to global context
 *                  p_x2ap_rsu_ctx - pointer to the rsu measurement context
 *                  p_enb_ctx - pointer to the peer enodeb context
 *                  p_criticality_diagnostics - pointer to criticality diagnostics
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function builds and sends ResourceStatusResponse message
 *       to X2AP module.
 ********************************************************************************/

x2ap_return_et x2ap_rsu_build_and_send_response_msg(x2ap_gb_context_t *p_x2ap_gb_ctx,
		x2ap_rsu_context_t* p_x2ap_rsu_ctx,
		x2ap_peer_enb_context_t *p_enb_ctx,
		x2ap_CriticalityDiagnostics *p_criticality_diagnostics)

{
	x2ap_return_et result = X2AP_FAILURE;
	OSCTXT asn1_ctx;
	U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
	U16    asn_msg_len = 0;

	rrc_x2ap_rsu_response_t msg;

	x2ap_peer_enb_msg_t  peer_enb_msg;

	X2AP_UT_TRACE_ENTER();
	X2AP_ASSERT(PNULL != p_x2ap_rsu_ctx);
	X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);
	X2AP_ASSERT(PNULL != p_enb_ctx);

	X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
	/* Init ASN context */
	if (RT_OK != rtInitContext(&asn1_ctx))
	{
		X2AP_TRACE(X2AP_ERROR,
				"[x2ap_ResourceStatusResponse] "
				"ASN context initialization failed");
		X2AP_UT_TRACE_EXIT();
		return X2AP_FAILURE;
	}

	/* Reset message */
	X2AP_MEMSET(&msg, 0, sizeof(msg));
	X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));

	/* Fill message */
	/*Fill Old eNB1_measurement_Id received in RSU start request*/
	msg.eNB1_measurement_Id = p_x2ap_rsu_ctx->enb1_meas_id;

	/*Fetch new Measurement Id for peer enb and fill the message*/ 
	msg.eNB2_measurement_Id = p_x2ap_rsu_ctx->enb2_meas_id;


	/*Fill Criticality Diagnostics*/
	/*As of now p_criticality_diagnostics is NULL*/
	if (PNULL != p_criticality_diagnostics)
	{
		msg.criticality_diagnostics = *p_criticality_diagnostics;
		msg.bitmask |= X2AP_RSU_FAILURE_CRIT_DIAGNO_PRESENT;
	}
	if(p_x2ap_rsu_ctx->partial_success_indicator_present && p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_START_ONGOING && p_x2ap_rsu_ctx->rrm_rsu_measurement_res_list_present)
	{
		msg.bitmask |=
			X2AP_RSU_RESPONSE_MSRMNT_INIT_RESULT;
		if(X2AP_FAILURE == x2ap_rsu_build_measurement_initiation_result_list(&msg,&p_x2ap_rsu_ctx->x2ap_rsu_measurement_res_list,&asn1_ctx))
		{
			X2AP_TRACE(X2AP_ERROR,"[x2ap_measurement_init_result] Build failure");
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
			return X2AP_FAILURE;
		}
	}

	/* Encode message */
	result = x2ap_rsu_internal_encode_response_msg(&asn1_ctx,
			asn_msg_buff,
			&asn_msg_len,
			&msg); 
	if (X2AP_SUCCESS!=result)
	{
		X2AP_TRACE(X2AP_ERROR,
				"[x2ap_ResourceStatusResponse] Build failure");
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
		return result;
	}
	peer_enb_msg.p_msg = asn_msg_buff;
	peer_enb_msg.msg_len = asn_msg_len;
    /* SPR_14436_Fix Start */
    peer_enb_msg.stream_id = p_enb_ctx->stream_info.
	stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
    /* SPR_14436_Fix Stop */

	if (INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd)
	{
		result = x2ap_sctp_sm_entry
			(X2AP_SCTP_SEND_DATA_EV,(U8*)&peer_enb_msg,p_x2ap_gb_ctx,p_enb_ctx);
		if (X2AP_SUCCESS == result)
		{
			/*Changing the state to X2AP_RSU_STATE_LISTEN_RSU*/
			p_x2ap_rsu_ctx->state = X2AP_RSU_STATE_LISTEN_RSU;   
		}     
	}    
	/* Free ASN Context */
	rtFreeContext(&asn1_ctx);

	X2AP_UT_TRACE_EXIT();

	return result;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_internal_encode_response_msg 
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_buff - pointer to result ASN.1 encoded message
 *                  p_buff_size - size of result buffer 
 * Outputs        : p_buff (ASN.1 encoded message),
 *                p_buff_size (size of result buffer)
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : his function used to encode resource status response.
 ********************************************************************************/
x2ap_return_et x2ap_rsu_internal_encode_response_msg 
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_x2ap_rsu_response_t  *p_resource_status_response
 )
{
	x2ap_return_et                   result = X2AP_FAILURE;
	x2ap_X2AP_PDU                    x2ap_pdu;
	x2ap_ResourceStatusResponse      *p_x2ap_msg = PNULL;

	X2AP_UT_TRACE_ENTER();

	X2AP_ASSERT(PNULL != pctxt);
	X2AP_ASSERT(PNULL != p_buff);
	X2AP_ASSERT(PNULL != p_buff_size);
	X2AP_ASSERT(PNULL != p_resource_status_response);

	do
	{
		/* Init X2AP PDU */
		asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
		/* Set Pdu type to Unsuccessful Outcome */
		x2ap_pdu.t = T_x2ap_X2AP_PDU_successfulOutcome;

		x2ap_pdu.u.successfulOutcome =
			rtxMemAllocType(pctxt, x2ap_SuccessfulOutcome);
		if (PNULL == x2ap_pdu.u.successfulOutcome)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			break;
		}

		asn1Init_x2ap_SuccessfulOutcome(x2ap_pdu.u.successfulOutcome);

		x2ap_pdu.u.successfulOutcome->procedureCode = ASN1V_x2ap_id_resourceStatusReportingInitiation; 

		x2ap_pdu.u.unsuccessfulOutcome->criticality = x2ap_reject;

		/* Set the initiating message type to resource status response */
		x2ap_pdu.u.successfulOutcome->value.t = T1x2ap__resourceStatusReportingInitiation; 

		x2ap_pdu.u.successfulOutcome->value.u.resourceStatusReportingInitiation = rtxMemAllocType(pctxt,x2ap_ResourceStatusResponse);
		if (PNULL == x2ap_pdu.u.successfulOutcome->value.u.resourceStatusReportingInitiation)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			break;
		}
		p_x2ap_msg = x2ap_pdu.u.successfulOutcome->value.u.resourceStatusReportingInitiation;

		asn1Init_x2ap_ResourceStatusResponse(p_x2ap_msg);
		if (RT_OK != asn1Append_x2ap_ResourceStatusResponse_protocolIEs_1(
					pctxt,
					&p_x2ap_msg->protocolIEs,
					p_resource_status_response->eNB1_measurement_Id))
		{
			break;
		}

		if (RT_OK != asn1Append_x2ap_ResourceStatusResponse_protocolIEs_2(
					pctxt,
					&p_x2ap_msg->protocolIEs,
					p_resource_status_response->eNB2_measurement_Id))
		{
			break;
		}
		/*Need to be done later*/
		if(p_resource_status_response->bitmask & X2AP_RSU_RESPONSE_MSRMNT_INIT_RESULT)
		{
			if (RT_OK != asn1Append_x2ap_ResourceStatusResponse_protocolIEs_4(
						pctxt,
						&p_x2ap_msg->protocolIEs,
						&p_resource_status_response->measurement_initiation_result_list))
			{
				break;
			}

		}
		/* ASN Encode message */
		pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

		if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
			break;
		}

		*p_buff_size = (U16)pe_GetMsgLen(pctxt);

		/* Print the asn encoded message */
	
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : Resource Status Request", &x2ap_pdu);

		result=X2AP_SUCCESS;
	} while(0);
	X2AP_UT_TRACE_EXIT();
	return result;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_process_failure_msg 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_ResourceStatusFailure
 *                  p_trgt - pointer to rrc_x2ap_rsu_failure_t
 *                  p_x2ap_gb_ctx - pointer to the global context
 *                  p_enb_ctx - pointer to peer eNB context
 * Outputs        : p_trgt  
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function used to process the X2AP RESOURCE STATUS FAILURE
 *                  received by the peer eNb
 ********************************************************************************/
x2ap_return_et x2ap_rsu_process_failure_msg
(
 OSCTXT      *p_asn1_ctx,
 x2ap_ResourceStatusFailure    *p_3gpp,    /* unpacked src */
 rrc_x2ap_rsu_failure_t       *p_trgt,    /* dest */
 x2ap_gb_context_t            *p_x2ap_gb_ctx,      /* X2ap global context*/
 x2ap_peer_enb_context_t          *p_enb_ctx

 )
{

	x2ap_return_et result = X2AP_FAILURE; 

	/* Get tree node */
	rsu_enb_meas_id_search_node_t* p_rsu_tree_node = X2AP_P_NULL;
	x2ap_rsu_context_t*     p_x2ap_rsu_ctx = X2AP_P_NULL;
	U8 counter = 0;
	x2ap_rrm_rsu_measurement_res_t x2ap_rsu_measurement_res_list;
	U8 partial_success_indication_present = 0;

	X2AP_UT_TRACE_ENTER();

	/*1. Decode the asn message received */
	if(X2AP_FAILURE == x2ap_rsu_decode_faliure_msg(p_x2ap_gb_ctx, p_enb_ctx,
				p_asn1_ctx, p_3gpp, p_trgt))
	{
		X2AP_TRACE(X2AP_ERROR,"%s: X2AP: RESOURCE STATUS FAILURE and decode failed.",__FUNCTION__);
		return X2AP_FAILURE;
	}

	else 
	{
		/*Fetching the RSU measurement context from the tree*/
		/*Get the mesurement id from the context*/
		X2AP_TRACE(X2AP_INFO,"%s: X2AP: RESOURCE STATUS FAILURE and decode Success.",__FUNCTION__);

		p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),
				&(p_trgt->eNB1_measurement_Id));

		if(X2AP_P_NULL != p_rsu_tree_node)
		{
			/* get the measurement context */
			p_x2ap_rsu_ctx = &p_rsu_tree_node->x2ap_rsu_ctx;
			for(counter = 0 ; counter < MAX_PEER_ENB ; counter++)
			{
				/* SPR 7573 RSU fix */
				if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].peer_enodeb_id
							== p_x2ap_rsu_ctx->peer_enb_id) && 
						(p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].op_status == OPERATIONAL))
					/* SPR 7573 RSU fix */
				{
					p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
					break;
				}
				else
					continue;
			}
		}
		else
		{
			X2AP_TRACE(X2AP_ERROR, "[%s]: Measurement Context does Not exist for this Measurement ID -- Ignoring Message\n", __FUNCTION__);
			return X2AP_FAILURE;
		}

		if(X2AP_P_NULL != p_x2ap_rsu_ctx)
		{
			/* Stop the timer X2AP_RSU_ENB_GUARD_TIMER. */
			x2ap_stop_timer(p_x2ap_rsu_ctx->timer_id);
			p_x2ap_rsu_ctx->timer_id = X2AP_P_NULL; 
		}
		if(p_trgt->bitmask & X2AP_RSU_COMP_FAILURE_INF_LIST)
		{
			x2ap_rsu_decode_complete_failure_list(&p_trgt->complete_failure_cause_inf_list,&x2ap_rsu_measurement_res_list);
			partial_success_indication_present = 1;
			X2AP_MEMCPY(&p_x2ap_rsu_ctx->x2ap_rsu_measurement_res_list,&x2ap_rsu_measurement_res_list,sizeof(p_x2ap_rsu_ctx->x2ap_rsu_measurement_res_list));
		}
		/* Validate the measurement context after fetching the context */
		if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_START_ONGOING)
		{
			X2AP_TRACE(X2AP_INFO,"RSU state is X2AP_RSU_STATE_START_ONGOING");
			/* Send RESOURCE STATUS REQUEST(START) Failed indication to RRM.*/
			x2ap_rsu_send_rrm_start_resp(p_x2ap_rsu_ctx->rrm_transaction_id,
					X2AP_FAILURE,p_x2ap_rsu_ctx->enb1_meas_id,
					p_trgt->cause.u.radioNetwork, p_enb_ctx /*x2ap_rsu_gb_enb_id_fix*/
					,&x2ap_rsu_measurement_res_list,partial_success_indication_present);

			/* Delete Measurement context.*/
		        /* SPR 16404 FIX START*/	
			X2AP_TRACE(X2AP_ERROR, "[%s]: Measurement context is deleted for measrurement ID [%u]\n",__FUNCTION__,p_x2ap_rsu_ctx->enb1_meas_id);
       			x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),&(p_rsu_tree_node->nodeAnchor)); 
			x2ap_mem_free(&p_rsu_tree_node->nodeAnchor);
                        /* SPR 16404 FIX STop*/
                        
       			result = X2AP_SUCCESS;

		}
		else if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_STOP_ONGOING ) 
		{
			/* Send RESOURCE STATUS REQUEST(STOP) Failed indication to RRM.*/
			X2AP_TRACE(X2AP_INFO,"RSU state is X2AP_RSU_STATE_STOP_ONGOING");
			x2ap_rsu_build_and_send_rrm_stop_resp(p_x2ap_rsu_ctx->rrm_transaction_id,
					X2AP_FAILURE,p_x2ap_rsu_ctx->enb1_meas_id,p_enb_ctx->gb_enb_id,p_trgt->cause.u.radioNetwork);

			if (x2ap_unknown_eNB_Measurement_ID == p_trgt->cause.u.radioNetwork) 
			{
				x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),&(p_rsu_tree_node->nodeAnchor));
				x2ap_mem_free(&p_rsu_tree_node->nodeAnchor);
				X2AP_TRACE(X2AP_ERROR, "[%s]: Measurement context is deleted for measrurement ID [%u] as radio network cause is unknown enb2 measurement ID\n",__FUNCTION__,p_x2ap_rsu_ctx->enb1_meas_id);
			}
			/* Move RSU_SM to X2AP_RSU_STATE_LISTEN_RSU.*/
			p_x2ap_rsu_ctx->state = X2AP_RSU_STATE_LISTEN_RSU;
		}
		else
		{
			X2AP_TRACE(X2AP_ERROR, "X2AP RESOURCE STATUS FAILURE Received in Invalid state[%d] or Measurement ID is Invalid\n",p_x2ap_rsu_ctx->state);
			result = X2AP_FAILURE;
		}
	}
	return result;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_decode_faliure_msg 
 * Inputs         : p_x2ap_gl_ctx  - pointer to the global context
 *                  p_enb_ctx - pointer to peer eNB context
 *                  p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_ResourceStatusFailure
 *                  p_msg - pointer to rrc_x2ap_rsu_failure_t
 * Outputs        : p_msg - decoded message
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function used to dceode the X2AP:RESOURCE STATUS FAILURE
 *                  received by the peer eNb.
 ********************************************************************************/
x2ap_return_et x2ap_rsu_decode_faliure_msg
(
 x2ap_gb_context_t  *p_x2ap_gl_ctx,
 x2ap_peer_enb_context_t *p_enb_ctx,
 OSCTXT      *p_asn1_ctx,
 x2ap_ResourceStatusFailure *p_3gpp,    /* unpacked src */
 rrc_x2ap_rsu_failure_t    *p_msg    /* dest */
 )
{

	U32  index = X2AP_NULL;
	OSRTDListNode  *p_node     = PNULL;
	x2ap_ResourceStatusFailure_protocolIEs_element *p_protocol_IE = PNULL;
	x2ap_return_et       retVal = X2AP_SUCCESS;
	x2ap_error_ind_ie_list_t   iE_list;
	x2ap_error_ind_bool_t send_err_ind;
	U16 ie_list_index = 0;
	/*x2ap_bool_et send_tse = X2AP_FALSE;*/
	x2ap_message_data_t message_map =
	{5, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
		{{0, ASN1V_x2ap_id_ENB1_Measurement_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{1, ASN1V_x2ap_id_ENB2_Measurement_ID, x2ap_mandatory,x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{2, ASN1V_x2ap_id_Cause, x2ap_mandatory,x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{3, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional,x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{4, ASN1V_x2ap_id_CompleteFailureCauseInformation_List, x2ap_optional,x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}}};


	X2AP_UT_TRACE_ENTER();

	X2AP_ASSERT(PNULL != p_3gpp);
	X2AP_ASSERT(PNULL != p_msg);

	X2AP_MEMSET(&iE_list, 0, sizeof(x2ap_error_ind_ie_list_t));
	X2AP_MEMSET(&send_err_ind, 0, sizeof(x2ap_error_ind_bool_t));

	p_msg->bitmask = 0;
	do
	{
		p_node = p_3gpp->protocolIEs.head;
		/* Save the decoded Protocol IEs */
		for (index =0; index < p_3gpp->protocolIEs.count; index++)
		{
			if (!p_node)
			{
				X2AP_TRACE(X2AP_ERROR,"%s: "
						"next node returned NULL",
						__FUNCTION__);
				break;
			}
			p_protocol_IE =
				(x2ap_ResourceStatusFailure_protocolIEs_element *)
				p_node->data;
			switch (p_protocol_IE->id)
			{
				case ASN1V_x2ap_id_ENB1_Measurement_ID:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									&p_protocol_IE->value.u._x2ap_ResourceStatusFailure_IEs_1))
						{
							p_msg->eNB1_measurement_Id =
								p_protocol_IE->value.u._x2ap_ResourceStatusFailure_IEs_1;
						}
					}
					break;

				case ASN1V_x2ap_id_ENB2_Measurement_ID:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									&(p_protocol_IE->value.u._x2ap_ResourceStatusFailure_IEs_2)))
						{
							p_msg->eNB2_measurement_Id = p_protocol_IE->value.u._x2ap_ResourceStatusFailure_IEs_2;
						}
					}
					break;

				case ASN1V_x2ap_id_Cause:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									p_protocol_IE->value.u._x2ap_ResourceStatusFailure_IEs_3))
						{
							p_msg->cause = *p_protocol_IE->value.u._x2ap_ResourceStatusFailure_IEs_3;
						}
					}
					break;

				case ASN1V_x2ap_id_CriticalityDiagnostics:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									p_protocol_IE->value.u._x2ap_ResourceStatusFailure_IEs_4))
						{
							p_msg->criticality_diagnostics = 
								*p_protocol_IE->value.u._x2ap_ResourceStatusFailure_IEs_4;
							p_msg->bitmask =
								X2AP_RSU_FAILURE_CRIT_DIAGNO_PRESENT;
						}
					}
					break;

				case ASN1V_x2ap_id_CompleteFailureCauseInformation_List:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									p_protocol_IE->value.u._x2ap_ResourceStatusFailure_IEs_5))
						{
							p_msg->complete_failure_cause_inf_list = 
								*p_protocol_IE->value.u._x2ap_ResourceStatusFailure_IEs_5;
							p_msg->bitmask =
								X2AP_RSU_COMP_FAILURE_INF_LIST;
						}
					}
					break;

				default:
					/* Class - 1 Error - Unknown IE */
					/* Base on Criticality fill the value */
					X2AP_TRACE(X2AP_ERROR, "%s:"\
							"invalid Protocol IE id",__FUNCTION__);
					x2ap_add_to_err_ind_ie_list(&iE_list,
							p_protocol_IE->criticality,
							p_protocol_IE->id,
							&ie_list_index,
							&send_err_ind,
							X2AP_FALSE,
							X2AP_FALSE);
					/*SPR_19067_FIX_START*/
                    if (x2ap_reject == p_protocol_IE->criticality) 
                    {
                        retVal = X2AP_FAILURE;
                    }
                    /*SPR_19067_FIX_STOP*/
			}
			p_node= p_node->next;
		}
	}while(0);
	/* Parse the map for Error Indication */
	if ((X2AP_SUCCESS == x2ap_parse_message_map(
					p_asn1_ctx,
					p_x2ap_gl_ctx,
					p_enb_ctx,
					&message_map,
					&iE_list,
					&ie_list_index,
					&send_err_ind,
					ASN1V_x2ap_id_resourceStatusReportingInitiation,
					T_x2ap_X2AP_PDU_unsuccessfulOutcome,
					x2ap_reject,
					(rrc_x2ap_error_indication_t *)PNULL))
	   )
	{
		X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED in"
				"rsu_decode_faliure_msg");

		retVal = X2AP_FAILURE;
	}

	X2AP_UT_TRACE_EXIT();

	return retVal;

}
/*****************************************************************************
 * Function Name  : x2ap_rsu_process_response_msg 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_ResourceStatusResponse
 *                  p_trgt - pointer to rrc_x2ap_rsu_response_t
 *                  p_x2ap_gb_ctx - pointer to X2ap global context
 *                  p_enb_ctx - pointer to peer eNB context
 * Outputs        : p_trgt
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function used to dceode the X2AP:RESOURCE STATUS responce
 *                  message received by the peer eNb
 ********************************************************************************/
x2ap_return_et x2ap_rsu_process_response_msg
(
 OSCTXT      *p_asn1_ctx,
 x2ap_ResourceStatusResponse    *p_3gpp,    /* unpacked src */
 rrc_x2ap_rsu_response_t       *p_trgt,    /* dest */
 x2ap_gb_context_t            *p_x2ap_gb_ctx,      /* X2ap global context*/
 x2ap_peer_enb_context_t      *p_enb_ctx
 )
{

	x2ap_return_et result = X2AP_FAILURE; 

	/* Get tree node */
	rsu_enb_meas_id_search_node_t* p_rsu_tree_node = X2AP_P_NULL;
	x2ap_rsu_context_t*     p_x2ap_rsu_ctx = X2AP_P_NULL;
	U8 counter = 0;
	x2ap_rrm_rsu_measurement_res_t x2ap_rsu_measurement_res_list;
	U8 partial_success_indication_present = 0;

	X2AP_UT_TRACE_ENTER();

	/*1. Decode the asn message received */
	if(X2AP_FAILURE == x2ap_rsu_decode_response_msg(p_x2ap_gb_ctx, p_enb_ctx,
				p_asn1_ctx, p_3gpp, p_trgt))
	{
		X2AP_TRACE(X2AP_ERROR,"%s: X2AP: RESOURCE STATUS RESPONSE asn decode failed.",__FUNCTION__);
		return X2AP_FAILURE;
	}

	else 
	{
		/*Fetching the RSU measurement context from the tree*/
		/*Get the mesurement id from the context*/

		p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),
				&(p_trgt->eNB1_measurement_Id));

		if(X2AP_P_NULL != p_rsu_tree_node)
		{
			/* get the measurement context */
			p_x2ap_rsu_ctx = &p_rsu_tree_node->x2ap_rsu_ctx;
			for(counter = 0 ; counter < MAX_PEER_ENB ; counter++)
			{
				/* SPR 7573 RSU fix */
				if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].peer_enodeb_id
							== p_x2ap_rsu_ctx->peer_enb_id) && 
						(p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].op_status == OPERATIONAL))
					/* SPR 7573 RSU fix */
				{
					p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
					break;
				}
			}
		}
		else
		{
			X2AP_TRACE(X2AP_ERROR, "[%s]: Measurement Context does Not exist for this Measurement ID -- Ignoring Message\n", __FUNCTION__);
			return X2AP_FAILURE;
		}

		if(p_trgt->bitmask & X2AP_RSU_RESPONSE_MSRMNT_INIT_RESULT)
		{
			x2ap_rsu_decode_measurement_intiation_list(&p_trgt->measurement_initiation_result_list,&x2ap_rsu_measurement_res_list);
			partial_success_indication_present = 1;
			X2AP_MEMCPY(&p_x2ap_rsu_ctx->x2ap_rsu_measurement_res_list,&x2ap_rsu_measurement_res_list,sizeof(p_x2ap_rsu_ctx->x2ap_rsu_measurement_res_list));
		}

		/* Stop the timer X2AP_RSU_ENB_TIMER. */  
		x2ap_stop_timer(p_x2ap_rsu_ctx->timer_id);
		p_x2ap_rsu_ctx->timer_id = X2AP_P_NULL; 

		if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_START_ONGOING)
		{ 
			X2AP_TRACE(X2AP_ERROR, "[%s]: Sending RSU start resp to RRM : SUCCESS " , __FUNCTION__);

			/* Send RESOURCE STATUS REQUEST(START) SUCCESS indication to RRM.*/
			x2ap_rsu_send_rrm_start_resp(p_x2ap_rsu_ctx->rrm_transaction_id,
					X2AP_SUCCESS, p_x2ap_rsu_ctx->enb1_meas_id,x2ap_unspecified_2,p_enb_ctx/*x2ap_rsu_gb_enb_id_fix*/,
					&x2ap_rsu_measurement_res_list,partial_success_indication_present);

			/*Get the enb2 meas_id from the RSU response message */
			p_x2ap_rsu_ctx->enb2_meas_id = p_trgt->eNB2_measurement_Id;

			/* Move RSU_SM to X2AP_RSU_STATE_LISTEN_RSU.*/
			p_x2ap_rsu_ctx->state = X2AP_RSU_STATE_LISTEN_RSU; 
		}
		else if (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_STOP_ONGOING)
		{
			X2AP_TRACE(X2AP_ERROR, "[%s]: Sending RSU stop resp to RRM : SUCCESS " , __FUNCTION__);

			/* Send RESOURCE STATUS REQUEST(STOP) Success indication to RRM.*/
			/* SPR_5966 */
			x2ap_rsu_build_and_send_rrm_stop_resp(p_x2ap_rsu_ctx->rrm_transaction_id
					, X2AP_SUCCESS, p_x2ap_rsu_ctx->enb1_meas_id,p_enb_ctx->gb_enb_id,x2ap_unspecified_2);
			/* SPR_5966 */

			/* Delete Measurement Context.*/
			x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),&(p_rsu_tree_node->nodeAnchor));
			x2ap_mem_free(&p_rsu_tree_node->nodeAnchor);

			/* Move RSU_SM to X2AP_RSU_STATE_IDLE. */
			/*Need to confirm */
		} 
	}   
	return result;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_decode_response_msg 
 * Inputs         : p_x2ap_gl_ctx - pointer to X2AP global context
 *                  p_enb_ctx - pointer to peer eNB context
 *                  p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_ResourceStatusResponse
 *                  p_msg - pointer to rrc_x2ap_rsu_response_t
 * Outputs        : p_msg - decoded message
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE 
 * Description    : This function used to dceode the X2AP:RESOURCE STATUS RESPONSE
 *                  received by the peer eNb
 ********************************************************************************/
x2ap_return_et x2ap_rsu_decode_response_msg
(
 x2ap_gb_context_t  *p_x2ap_gl_ctx,
 x2ap_peer_enb_context_t *p_enb_ctx,
 OSCTXT      *p_asn1_ctx,
 x2ap_ResourceStatusResponse *p_3gpp,    /* unpacked src */
 rrc_x2ap_rsu_response_t    *p_msg    /* dest */
 )
{

	U32  index = X2AP_NULL;
	OSRTDListNode  *p_node     = PNULL;
	x2ap_ResourceStatusResponse_protocolIEs_element *p_protocol_IE = PNULL;
	x2ap_return_et       retVal = X2AP_SUCCESS;
	x2ap_error_ind_ie_list_t   iE_list;
	x2ap_error_ind_bool_t send_err_ind;
	U16 ie_list_index = 0;

	x2ap_message_data_t message_map =
	{4, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
		{{0, ASN1V_x2ap_id_ENB1_Measurement_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{1, ASN1V_x2ap_id_ENB2_Measurement_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{2, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{3, ASN1V_x2ap_id_MeasurementInitiationResult_List, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
		}};

	X2AP_UT_TRACE_ENTER();

	X2AP_MEMSET(&iE_list, X2AP_NULL, sizeof(x2ap_error_ind_ie_list_t));
	X2AP_MEMSET(&send_err_ind, X2AP_FALSE, sizeof(x2ap_error_ind_bool_t));

	X2AP_ASSERT(PNULL != p_3gpp);
	X2AP_ASSERT(PNULL != p_msg);

	p_msg->bitmask = 0;
	do
	{
		p_node = p_3gpp->protocolIEs.head;
		/* Save the decoded Protocol IEs */
		for (index =0; index < p_3gpp->protocolIEs.count; index++)
		{
			if (!p_node)
			{
				X2AP_TRACE(X2AP_ERROR,"%s: "
						"next node returned NULL",
						__FUNCTION__);
				break;
			}
			p_protocol_IE =
				(x2ap_ResourceStatusResponse_protocolIEs_element *)
				p_node->data;
			switch (p_protocol_IE->id)
			{
				case ASN1V_x2ap_id_ENB1_Measurement_ID:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									&p_protocol_IE->value.u._x2ap_ResourceStatusResponse_IEs_1))
						{
							p_msg->eNB1_measurement_Id = p_protocol_IE->value.u._x2ap_ResourceStatusResponse_IEs_1;
						}
					}
					break;

				case ASN1V_x2ap_id_ENB2_Measurement_ID:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									&p_protocol_IE->value.u._x2ap_ResourceStatusResponse_IEs_2))
						{
							p_msg->eNB2_measurement_Id = p_protocol_IE->value.u._x2ap_ResourceStatusResponse_IEs_2;
						}
					}
					break;

				case ASN1V_x2ap_id_CriticalityDiagnostics:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									p_protocol_IE->value.u._x2ap_ResourceStatusResponse_IEs_3))
						{
							p_msg->criticality_diagnostics = 
								*p_protocol_IE->value.u._x2ap_ResourceStatusResponse_IEs_3;
							p_msg->bitmask =
								X2AP_RSU_RESPONSE_CRIT_DIAGNO_PRESENT;
						}
					}
					break;

				case ASN1V_x2ap_id_MeasurementInitiationResult_List:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									p_protocol_IE->value.u._x2ap_ResourceStatusResponse_IEs_4))
						{
							p_msg->bitmask |=
								X2AP_RSU_RESPONSE_MSRMNT_INIT_RESULT;
							p_msg->measurement_initiation_result_list = 
								*p_protocol_IE->value.u._x2ap_ResourceStatusResponse_IEs_4;
						}
					}
					break;

				default:
					/* Class - 1 Error - Unknown IE */
					/* Base on Criticality fill the value */
					X2AP_TRACE(X2AP_ERROR, "%s:"\
							"invalid Protocol IE id",__FUNCTION__);
					x2ap_add_to_err_ind_ie_list(&iE_list,
							p_protocol_IE->criticality,
							p_protocol_IE->id,
							&ie_list_index,
							&send_err_ind,
							X2AP_FALSE,
							X2AP_FALSE);
					/*SPR_19067_FIX_START*/
                      if (x2ap_reject == p_protocol_IE->criticality) 
                      {
                          retVal = X2AP_FAILURE;
                      }
                      /*SPR_19067_FIX_STOP*/
			}
			p_node= p_node->next;
		}
	}while(0);
	/* Parse the map for Error Indication */
	if ((X2AP_SUCCESS == x2ap_parse_message_map(
					p_asn1_ctx,
					p_x2ap_gl_ctx,
					p_enb_ctx,
					&message_map,
					&iE_list,
					&ie_list_index,
					&send_err_ind,
					ASN1V_x2ap_id_resourceStatusReportingInitiation,
					T_x2ap_X2AP_PDU_successfulOutcome,
					x2ap_reject,
					(rrc_x2ap_error_indication_t *)PNULL))
	   )
	{
		X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED in"
				"rsu_decode_response_msg");

		retVal = X2AP_FAILURE;
	}

	X2AP_UT_TRACE_EXIT();

	return retVal;

}
/*****************************************************************************
 * Function Name  : x2ap_process_rrm_rsu_update 
 * Inputs         : p_api - pointer to input API buffer
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 * Outputs        : None
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS
 * Description    : This function process the X2AP_RRM_RSU_RRM_UPDATE received 
 *                  fromm the rrm
 ********************************************************************************/

x2ap_return_et x2ap_process_rrm_rsu_update( void             *p_api,
		x2ap_gb_context_t   *p_x2ap_gb_ctx)
{
	x2ap_rrm_rsu_rrm_update_t *p_x2ap_rrm_rsu_rrm_update;
	S32 length_read = X2AP_NULL;
	x2ap_return_et response = X2AP_FAILURE;
	U16 trans_id = X2AP_NULL;
	U8 counter = 0;

	/* Get tree node */ 
	rsu_enb_meas_id_search_node_t* p_rsu_tree_node = X2AP_P_NULL;   
	x2ap_rsu_context_t*     p_x2ap_rsu_ctx = X2AP_P_NULL;

	x2ap_peer_enb_context_t *p_enb_ctx = X2AP_P_NULL;

	/*Get the structure in case update is received in case of null Context */
	x2ap_rrm_rsu_enb_stop_req_t *p_x2ap_rrm_rsu_enb_stop_req = X2AP_P_NULL;

	X2AP_UT_TRACE_ENTER();
	X2AP_ASSERT(X2AP_P_NULL != p_api);
	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

	/* Get the transaction id */
	trans_id = x2ap_get_transaction_id(p_api);

	p_x2ap_rrm_rsu_rrm_update = (x2ap_rrm_rsu_rrm_update_t*)
		x2ap_mem_get(sizeof(x2ap_rrm_rsu_rrm_update_t));
	if(X2AP_P_NULL == p_x2ap_rrm_rsu_rrm_update)
	{
		X2AP_TRACE(X2AP_FATAL,"\nMemeory allocation failed ");
		return X2AP_FAILURE;
	}

	/* Decode the received message from RRM */
	response = rrc_il_parse_x2ap_rrm_rsu_rrm_update(
			p_x2ap_rrm_rsu_rrm_update,
			(U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
			x2ap_get_api_buf_size(p_api) - 
			RRC_FULL_INTERFACE_HEADERS_SIZE,
			&length_read);

	if(X2AP_SUCCESS == response)
	{
		/*Get the mesurement id from the context*/
		p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),
				&(p_x2ap_rrm_rsu_rrm_update->enb_meas_id));
		if(X2AP_P_NULL != p_rsu_tree_node)
		{
			/* get the measurement context */
			p_x2ap_rsu_ctx = &p_rsu_tree_node->x2ap_rsu_ctx;       
			for(counter = 0 ; counter < MAX_PEER_ENB ; counter++)
			{
				/* SPR 7573 RSU fix */
				if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].peer_enodeb_id
							== p_x2ap_rsu_ctx->peer_enb_id) && 
						(p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].op_status == OPERATIONAL))
					/* SPR 7573 RSU fix */
				{
					p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
					break;
				}
			}
			if(X2AP_P_NULL == p_enb_ctx)
			{
				X2AP_TRACE(X2AP_ERROR, "[%s]: Peer Enb Context is NULL while fetching from x2ap global context\n", __FUNCTION__);
				/* coverity_fix_63323_start */
				x2ap_mem_free((void *)p_x2ap_rrm_rsu_rrm_update);
				/* coverity_fix_63323_stop */
				return X2AP_FAILURE;
			}
		}
		else
		{
			X2AP_TRACE(X2AP_ERROR, "RSU context is NULL for measurement ID -- [%u] [%s]\n",p_x2ap_rrm_rsu_rrm_update->enb_meas_id, __FUNCTION__);
			/* Sending RESOURCE STATUS (STOP) TO RRM */
			p_x2ap_rrm_rsu_enb_stop_req = (x2ap_rrm_rsu_enb_stop_req_t *)x2ap_mem_get(sizeof(x2ap_rrm_rsu_enb_stop_req_t));
			/* SPR 12964 Fix Start */
			if(X2AP_P_NULL == p_x2ap_rrm_rsu_enb_stop_req)
			{
				X2AP_TRACE(X2AP_ERROR, "[%s]: Unable to Allocate memory\n", __FUNCTION__);
				/* SPR 19838 : coverity 11182 fix start */
				x2ap_mem_free((void *)p_x2ap_rrm_rsu_rrm_update);
				/* SPR 19838 : coverity 11182 fix end */
				return X2AP_FAILURE;
			}
			/* SPR 12964 Fix Stop */
			X2AP_MEMSET(p_x2ap_rrm_rsu_enb_stop_req,0,sizeof(x2ap_rrm_rsu_enb_stop_req_t));

			/* Fill the value of the meas ID */
			p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id = p_x2ap_rrm_rsu_rrm_update->enb_meas_id;
			X2AP_MEMCPY(&p_x2ap_rrm_rsu_enb_stop_req->peer_gb_enb_id,&p_x2ap_rrm_rsu_rrm_update->peer_gb_enb_id,sizeof(x2_gb_enb_id_t));
			response = rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_stop_req(
					p_x2ap_rrm_rsu_enb_stop_req,
					RRC_X2AP_MODULE_ID, /* src module id */
					RRC_RRM_MODULE_ID, 
					trans_id, /* transaction_id */ 
					RRC_NULL);
			/* coverity_fix_63323_start */
			x2ap_mem_free((void *)p_x2ap_rrm_rsu_rrm_update);
			/* coverity_fix_63323_stop */
			/* SPR 20007 : CID 110666 fix start */
			if (PNULL != p_x2ap_rrm_rsu_enb_stop_req)
			{
				x2ap_mem_free((void *)p_x2ap_rrm_rsu_enb_stop_req);
			}
			/* SPR 20007 : CID 110666 fix end */
			return response;   
		} 
		/*Validate the meas id received with the measurement context and State */
		/* SPR_5966 */
		if ((p_x2ap_rsu_ctx->enb2_meas_id == p_x2ap_rrm_rsu_rrm_update->enb_meas_id) && (p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_SEND_RSU))
			/* SPR_5966 */
		{
			X2AP_TRACE(X2AP_INFO, "[%s]: Meas ID is matched successfully", __FUNCTION__);

			/*Build and send the X2AP: RESOURCE STATUS UPDATE to Peer Enb */
			x2ap_rsu_build_and_send_update_msg(p_x2ap_gb_ctx,p_x2ap_rsu_ctx,p_x2ap_rrm_rsu_rrm_update,p_enb_ctx);

		}
		else
		{
			X2AP_TRACE(X2AP_ERROR, "X2AP: RESOURCE STATUS UPDATE is received in invalid state or Meas ID is not Valid\n");
			/* coverity_fix_63323_start */
			x2ap_mem_free((void *)p_x2ap_rrm_rsu_rrm_update);
			/* coverity_fix_63323_stop */
			return X2AP_FAILURE;
		}
	}
	else
	{
		X2AP_TRACE(X2AP_WARNING, "X2AP_RRM_RSU_RRM_UPDATE decode failed");
		/* coverity_fix_63323_start */
		x2ap_mem_free((void *)p_x2ap_rrm_rsu_rrm_update);
		/* coverity_fix_63323_stop */
		return X2AP_FAILURE;
	}
	/* coverity_fix_63323_start */
	x2ap_mem_free((void *)p_x2ap_rrm_rsu_rrm_update);
	/* coverity_fix_63323_stop */
	return response; 
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_build_and_send_update_msg 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_x2ap_rsu_ctx - pointer to x2ap_rsu_context_t
 *                  p_x2ap_rrm_rsu_rrm_update - pointer to 
 *                                                p_x2ap_rrm_rsu_rrm_update
 *                  p_enb_ctx - pointer to peer eNb context
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function builds and sends ResourceStatusResponse message
 *       to X2AP module.
 ********************************************************************************/

x2ap_return_et x2ap_rsu_build_and_send_update_msg(x2ap_gb_context_t *p_x2ap_gb_ctx,
		x2ap_rsu_context_t* p_x2ap_rsu_ctx,
		x2ap_rrm_rsu_rrm_update_t
		*p_x2ap_rrm_rsu_rrm_update,
		x2ap_peer_enb_context_t *p_enb_ctx)
{
	x2ap_return_et result = X2AP_FAILURE;
	OSCTXT asn1_ctx;
	U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
	U16    asn_msg_len = 0;

	rrc_x2ap_rsu_update_t msg;
	x2ap_peer_enb_msg_t  peer_enb_msg;



	X2AP_UT_TRACE_ENTER();
	X2AP_ASSERT(PNULL != p_x2ap_rsu_ctx);
	X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);
	X2AP_ASSERT(PNULL != p_x2ap_rrm_rsu_rrm_update);

	/* Init ASN context */
	if (RT_OK != rtInitContext(&asn1_ctx))
	{
		X2AP_TRACE(X2AP_ERROR,
				"[x2ap_ResourceStatusUpdate] "
				"ASN context initialization failed");
		X2AP_UT_TRACE_EXIT();
		return X2AP_FAILURE;
	}

	/* Reset message */
	X2AP_MEMSET(&msg, 0, sizeof(msg));
	X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));
	X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

	/* Build X2AP RSU REQUEST  TO send to eNB */
	msg.eNB1_measurement_Id = p_x2ap_rsu_ctx->enb1_meas_id;
	msg.eNB2_measurement_Id = p_x2ap_rsu_ctx->enb2_meas_id;

	if(X2AP_FAILURE == x2ap_rsu_build_update_list(&msg, p_x2ap_rrm_rsu_rrm_update,&asn1_ctx))
	{
		X2AP_TRACE(X2AP_ERROR,"[x2ap_ResourceStatusUpdate] Build failure");
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
		return X2AP_FAILURE;
	}

	/* Encode message */
	result = x2ap_rsu_internal_encode_update_msg(&asn1_ctx,
			asn_msg_buff,
			&asn_msg_len,
			&msg); 

	if(X2AP_SUCCESS == result)
    {
        peer_enb_msg.p_msg = asn_msg_buff;
        peer_enb_msg.msg_len = asn_msg_len;
        /* SPR_14436_Fix Start */
        peer_enb_msg.stream_id = p_enb_ctx->stream_info.
            stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
        /* SPR_14436_Fix Stop */

        if (INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd)
        {
            /*Sending the  X2 RSU START REQUEST to Peer eNB via SCTP */
            result = x2ap_sctp_sm_entry
                (X2AP_SCTP_SEND_DATA_EV,(U8*)&peer_enb_msg,p_x2ap_gb_ctx,p_enb_ctx);

            /*Changing the state to X2AP_RSU_STATE_LISTEN_RSU*/
            /* p_x2ap_rsu_ctx->state = X2AP_RSU_STATE_LISTEN_RSU;   */
        }    
    }    
	else
	{
		X2AP_TRACE(X2AP_ERROR,
				"[x2ap_ResourceStatusUpdate] Encode failure");
         /*SPR_18125_START*/
         rtFreeContext(&asn1_ctx);
         /*SPR_18125_END*/
		return X2AP_FAILURE;
	}

	/* Free ASN Context */
	rtFreeContext(&asn1_ctx);

	X2AP_UT_TRACE_EXIT();

	return result;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_build_update_list
 * Inputs         : p_x2ap_res_status_update - pointer to rrc_x2ap_rsu_update_t
 *                : p_x2ap_rrm_rsu_rrm_update - pointer to structure recvd 
 *                  from RRM.
 *                : p_asn1_ctx - pointer to OSCTXT
 * Outputs        : p_x2ap_rrc_x2ap_rsu_update - x2ap rsu Update
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE 
 * Description    : This function is called to build x2ap rsu Update.
 *****************************************************************************/
x2ap_return_et x2ap_rsu_build_update_list(rrc_x2ap_rsu_update_t   *p_x2ap_res_status_update, x2ap_rrm_rsu_rrm_update_t *p_x2ap_rrm_rsu_update, OSCTXT *p_asn1_ctx)
{
	/* coverity_fix_54718_start */
	OSRTDListNode* p_node = PNULL;
	U16 meas_result_ct = 0;
	/* eICIC changes start */
	U8 counter = RRC_NULL;
	/* eICIC changes stop */
	x2ap_CellMeasurementResult_List_element *p_elem = X2AP_P_NULL;

	x2ap_CellMeasurementResult_List *p_cell_meas_result_list;
	p_cell_meas_result_list = &p_x2ap_res_status_update->cell_meas_result_list;
	asn1Init_x2ap_CellMeasurementResult_List(p_cell_meas_result_list);

	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_res_status_update);
	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);
	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_rrm_rsu_update);

	for (meas_result_ct = 0; meas_result_ct < p_x2ap_rrm_rsu_update->num_reports ; meas_result_ct++)
	{
		rtxDListAllocNodeAndData(p_asn1_ctx,
				x2ap_CellMeasurementResult_List_element,
				&p_node,
				&p_elem);

		if (X2AP_P_NULL == p_node)
		{
			X2AP_TRACE(X2AP_ERROR,"Can't allocate p_node");

			rtxDListFreeAll(p_asn1_ctx, p_cell_meas_result_list);

			X2AP_UT_TRACE_EXIT();
			return X2AP_FAILURE;
		}

		asn1Init_x2ap_CellMeasurementResult_List_element(p_elem);
		asn1Init_x2ap_CellMeasurementResult_Item(&p_elem->value);

		p_elem->id = ASN1V_x2ap_id_CellMeasurementResult_Item;
		p_elem->criticality = x2ap_ignore;

		p_elem->value.cell_ID.pLMN_Identity.numocts = MCC_OCTET_SIZE;
		x2ap_compose_asn_plmn_identity(
				p_elem->value.cell_ID.pLMN_Identity.data,
				&(p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].cell_info.plmn_identity));

		/* copy cell_identity */
		p_elem->value.cell_ID.eUTRANcellIdentifier.numbits = 8*sizeof(p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].cell_info.cell_identity);
		p_elem->value.cell_ID.eUTRANcellIdentifier.numbits =(p_elem->value.cell_ID.eUTRANcellIdentifier.numbits < MAX_CELL_ID_BITS ? p_elem->value.cell_ID.eUTRANcellIdentifier.numbits : MAX_CELL_ID_BITS);

		X2AP_MEMCPY(((void*)p_elem->value.cell_ID.eUTRANcellIdentifier.data),
				((const void*)p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].cell_info.cell_identity),
				p_elem->value.cell_ID.eUTRANcellIdentifier.numbits/8+1);

		if (PRB_PERIODIC_REPORT_PRESENT & p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.bitmask)
		{
			/*updating the value for PRB usage*/
			p_elem->value.radioResourceStatus.dL_GBR_PRB_usage     =
				p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.radio_resource_status.dl_gbr_prb_usage;
			p_elem->value.radioResourceStatus.uL_GBR_PRB_usage     =
				p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.radio_resource_status.ul_gbr_prb_usage;
			p_elem->value.radioResourceStatus.dL_non_GBR_PRB_usage =
				p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.radio_resource_status.dl_non_gbr_prb_usage;
			p_elem->value.radioResourceStatus.uL_non_GBR_PRB_usage =
				p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.radio_resource_status.ul_non_gbr_prb_usage;
			p_elem->value.radioResourceStatus.dL_Total_PRB_usage   =
				p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.radio_resource_status.dl_total_gbr_prb_usage;
			p_elem->value.radioResourceStatus.uL_Total_PRB_usage   =
				p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.radio_resource_status.ul_total_gbr_prb_usage;
			p_elem->value.m.radioResourceStatusPresent = 1;

		}         
		if (TNL_LOAD_IND_PERIODIC_REPORT_PRESENT & p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.bitmask)
		{
            /* SPR 20622 Fix Start */
			p_elem->value.s1TNLLoadIndicator.dLS1TNLLoadIndicator =
				(p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.s1_tnl_load_indicator.dl_s1_tnl_load_indicator - 1);
			p_elem->value.s1TNLLoadIndicator.uLS1TNLLoadIndicator =
				(p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.s1_tnl_load_indicator.ul_s1_tnl_load_indicator - 1);
            /* SPR 20622 Fix Stop */
			p_elem->value.m.s1TNLLoadIndicatorPresent = 1;    
		}       
		if (LOAD_IND_PERIODIC_REPORT_PRESENT & p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.bitmask)
		{
            /* SPR 20622 Fix Start */
			p_elem->value.hWLoadIndicator.dLHWLoadIndicator =
				(p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.hardware_load_indicator.dl_hardware_load_indicator - 1);
			p_elem->value.hWLoadIndicator.uLHWLoadIndicator =
				(p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.hardware_load_indicator.ul_hardware_load_indicator - 1);
            /* SPR 20622 Fix Stop */
			p_elem->value.m.hWLoadIndicatorPresent = 1;
		}
		if (COMPOSITE_AVAILABLE_CAPACITY_PERIODIC_REPORT_PRESENT &  p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.bitmask)
		{
			OSRTDListNode* p_cacg_node = X2AP_P_NULL;
			x2ap_CellMeasurementResult_Item_iE_Extensions* p_cell_cacg_list = X2AP_P_NULL;
			x2ap_CellMeasurementResult_Item_iE_Extensions_element *p_cacg_elem = X2AP_P_NULL;

			p_cell_cacg_list = &p_elem->value.iE_Extensions;
			p_elem->value.m.iE_ExtensionsPresent = 1;

			asn1Init_x2ap_CellMeasurementResult_Item_iE_Extensions(p_cell_cacg_list);

			rtxDListAllocNodeAndData(p_asn1_ctx,
					x2ap_CellMeasurementResult_Item_iE_Extensions_element,
					&p_cacg_node,
					&p_cacg_elem);

			if (X2AP_P_NULL == p_cacg_node)
			{
				X2AP_TRACE(X2AP_ERROR,"Can't allocate p_node");

				rtxDListFreeAll(p_asn1_ctx, p_cell_cacg_list);

				X2AP_UT_TRACE_EXIT();
				return X2AP_FAILURE;
			}

			asn1Init_x2ap_CellMeasurementResult_Item_iE_Extensions_element(p_cacg_elem);

			p_cacg_elem->id = ASN1V_x2ap_id_CompositeAvailableCapacityGroup;
			p_cacg_elem->criticality = x2ap_ignore;
			/*SPR 5496 Start */
			p_cacg_elem->extensionValue.t = T47x2ap___x2ap_CellMeasurementResult_Item_ExtIEs_1;
			p_cacg_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1 = rtxMemAllocType(p_asn1_ctx, x2ap_CompositeAvailableCapacityGroup);
			if (X2AP_P_NULL == p_cacg_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1)
			{
				X2AP_TRACE(X2AP_ERROR,"Can't allocate x2ap_CompositeAvailableCapacityGroup");
				X2AP_UT_TRACE_EXIT();
				return X2AP_FAILURE;
			}
			asn1Init_x2ap_CompositeAvailableCapacityGroup(p_cacg_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1);
			/*SPR 5496 Stop*/
			p_cacg_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1->dL_CompositeAvailableCapacity.capacityValue
				= p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.composite_available_capacity_group.dl_composite_available_capacity.cell_capacity_value;
			if(CELL_CAPACITY_CLASS_VALUE_PRESENT &
					p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.composite_available_capacity_group.dl_composite_available_capacity.bitmask)  
			{
				p_cacg_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1->dL_CompositeAvailableCapacity.cellCapacityClassValue
					=
					p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.composite_available_capacity_group.dl_composite_available_capacity.cell_capacity_class_value;
				p_cacg_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1->dL_CompositeAvailableCapacity.m.cellCapacityClassValuePresent = 1;
			} 

			p_cacg_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1->uL_CompositeAvailableCapacity.capacityValue
				=
				p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.composite_available_capacity_group.ul_composite_available_capacity.cell_capacity_value;
			if (CELL_CAPACITY_CLASS_VALUE_PRESENT &
					p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.composite_available_capacity_group.ul_composite_available_capacity.bitmask)
			{
				p_cacg_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1->uL_CompositeAvailableCapacity.cellCapacityClassValue
					=
					p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.composite_available_capacity_group.ul_composite_available_capacity.cell_capacity_class_value;
				p_cacg_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1->uL_CompositeAvailableCapacity.m.cellCapacityClassValuePresent = 1;
			}

			if (0 == rtxDListAppend(p_asn1_ctx,p_cell_cacg_list, (void*)p_cacg_elem))
			{ 
				return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
			}
		}
		/* eICIC changes start */
		if (ABS_STATUS_PERIODIC_REPORT_PRESENT &  p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.bitmask)
		{
			OSRTDListNode* p_abs_status_node = X2AP_P_NULL;
			x2ap_CellMeasurementResult_Item_iE_Extensions* p_cell_abs_status_list = X2AP_P_NULL;
			x2ap_CellMeasurementResult_Item_iE_Extensions_element *p_abs_status_elem = X2AP_P_NULL;

			p_cell_abs_status_list = &p_elem->value.iE_Extensions;
			p_elem->value.m.iE_ExtensionsPresent = 1;

                        /*Bug 481 Start */
                        if (!(COMPOSITE_AVAILABLE_CAPACITY_PERIODIC_REPORT_PRESENT &  p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.bitmask))
			{
			asn1Init_x2ap_CellMeasurementResult_Item_iE_Extensions(p_cell_abs_status_list);
			}
                        /*Bug 481 End */

			rtxDListAllocNodeAndData(p_asn1_ctx,
					x2ap_CellMeasurementResult_Item_iE_Extensions_element,
					&p_abs_status_node,
					&p_abs_status_elem);

			if (X2AP_P_NULL == p_abs_status_node)
			{
				X2AP_TRACE(X2AP_ERROR,"Can't allocate p_node");

				rtxDListFreeAll(p_asn1_ctx, p_cell_abs_status_list);

				X2AP_UT_TRACE_EXIT();
				return X2AP_FAILURE;
			}

			asn1Init_x2ap_CellMeasurementResult_Item_iE_Extensions_element(p_abs_status_elem);

			p_abs_status_elem->id = ASN1V_x2ap_id_ABS_Status;
			p_abs_status_elem->criticality = x2ap_ignore;
			p_abs_status_elem->extensionValue.t = T47x2ap___x2ap_CellMeasurementResult_Item_ExtIEs_2;
			p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2 = rtxMemAllocType(p_asn1_ctx, x2ap_ABS_Status);
			if (X2AP_P_NULL == p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2)
			{
				X2AP_TRACE(X2AP_ERROR,"Can't allocate x2ap_ABS_Status");
				X2AP_UT_TRACE_EXIT();
				return X2AP_FAILURE;
			}
			asn1Init_x2ap_ABS_Status(p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2);

			p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->dL_ABS_status
				= p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.abs_status.dl_abs_status;

			if(X2AP_USABLE_ABS_INFORMATION_FDD_PRESENT &
					p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.abs_status.x2ap_usable_abs_information.bitmask)  
			{
				p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.t = 
					T_x2ap_UsableABSInformation_fdd;

				p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.u.fdd =
					rtxMemAllocType(p_asn1_ctx, x2ap_UsableABSInformationFDD);
				if (PNULL == p_abs_status_elem->extensionValue.u.
						_x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.u.fdd)
				{
					X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
					X2AP_UT_TRACE_EXIT();
					return X2AP_FAILURE;
				}

				asn1Init_x2ap_UsableABSInformationFDD(p_abs_status_elem->extensionValue.u.
						_x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.u.fdd);

				p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->
					usableABSInformation.u.fdd->usable_abs_pattern_info.numbits = MAX_SUBFRAME_CONFIG_FDD_NUMBITS; 

				for(counter = 0; counter < X2AP_ABS_PATTERN_SIZE_FDD; counter++)
				{
					p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->
						usableABSInformation.u.fdd->usable_abs_pattern_info.data[counter] =
						p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.abs_status.x2ap_usable_abs_information.
						usable_abs_info_fdd.usable_abs_pattern_fdd_info[counter];
				}
			} 
			else if (X2AP_USABLE_ABS_INFORMATION_TDD_PRESENT &
					p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.abs_status.x2ap_usable_abs_information.bitmask)
			{
				p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.t = 
					T_x2ap_UsableABSInformation_tdd;

				p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.u.tdd =
					rtxMemAllocType(p_asn1_ctx, x2ap_UsableABSInformationTDD);
				if (PNULL == p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.u.tdd)
				{
					X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
					X2AP_UT_TRACE_EXIT();
					return X2AP_FAILURE;
				}

				asn1Init_x2ap_UsableABSInformationTDD(p_abs_status_elem->extensionValue.u.
						_x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.u.tdd);

				p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->
					usableABSInformation.u.tdd->usaable_abs_pattern_info.numbits = MAX_SUBFRAME_CONFIG0_TDD_NUMBITS;

				p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.
					u.tdd->usaable_abs_pattern_info.data = rtxMemAlloc(p_asn1_ctx,(sizeof(U8)*X2AP_ABS_PATTERN_SIZE_TDD));

				if(X2AP_P_NULL == p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->
						usableABSInformation.u.tdd->usaable_abs_pattern_info.data)
				{
					X2AP_TRACE(X2AP_ERROR, "[%s] Unable to allocate memory",__FUNCTION__);
					return X2AP_FAILURE;
				}

				X2AP_MEMCPY((void *)p_abs_status_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->
						usableABSInformation.u.tdd->usaable_abs_pattern_info.data, 
						p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.abs_status.x2ap_usable_abs_information.
						usable_abs_info_tdd.usable_abs_pattern_tdd_info,
						sizeof(p_x2ap_rrm_rsu_update->rsu_report[meas_result_ct].rsu_info.abs_status.
							x2ap_usable_abs_information.usable_abs_info_tdd.usable_abs_pattern_tdd_info));

			}
			else
			{

				X2AP_TRACE(X2AP_WARNING,"[%s] No usable abs information present[usable_abs_info_fdd or usable_abs_info_tdd should be present].",
						__FUNCTION__);

			}

			if (0 == rtxDListAppend(p_asn1_ctx,p_cell_abs_status_list, (void*)p_abs_status_elem))
			{ 
				return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
			}
		}
		/* eICIC changes stop */

		if (0 == rtxDListAppend(p_asn1_ctx,p_cell_meas_result_list, (void*)p_elem))
		{ 
			return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
		}
	}
	return X2AP_SUCCESS;
}
/* coverity_fix_54718_stop */

/*****************************************************************************
 * Function Name  : x2ap_rsu_internal_encode_update_msg 
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_buff - pointer to result ASN.1 encoded message
 *                  p_buff_size - size of result buffer
 *                  p_resource_status_update - pointer to rrc_x2ap_rsu_update_t
 * Outputs        : p_buff (ASN.1 encoded message),
 *                p_buff_size (size of result buffer)
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function used to encode resource status update message 
 ********************************************************************************/
x2ap_return_et x2ap_rsu_internal_encode_update_msg 
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_x2ap_rsu_update_t  *p_resource_status_update
 )
{
	x2ap_return_et                   result = X2AP_FAILURE;
	x2ap_X2AP_PDU                    x2ap_pdu;
	x2ap_ResourceStatusUpdate  *p_x2ap_msg = PNULL;

	X2AP_UT_TRACE_ENTER();

	X2AP_ASSERT(PNULL != pctxt);
	X2AP_ASSERT(PNULL != p_buff);
	X2AP_ASSERT(PNULL != p_buff_size);
	X2AP_ASSERT(PNULL != p_resource_status_update);

	do
	{
		/* Init X2AP PDU */
		asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
		/* Set Pdu type to Initiating Message */
		x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

		x2ap_pdu.u.initiatingMessage =
			rtxMemAllocType(pctxt, x2ap_InitiatingMessage);
		if (PNULL == x2ap_pdu.u.initiatingMessage)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			break;
		}

		asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

		x2ap_pdu.u.initiatingMessage->procedureCode = ASN1V_x2ap_id_resourceStatusReporting; 

		x2ap_pdu.u.initiatingMessage->criticality = x2ap_ignore;

		/* Set the initiating message type to resource status failure */
		x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__resourceStatusReporting; 

		x2ap_pdu.u.initiatingMessage->value.u.resourceStatusReporting = rtxMemAllocType(pctxt,x2ap_ResourceStatusUpdate);
		if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.resourceStatusReporting)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			break;
		}
		p_x2ap_msg = x2ap_pdu.u.initiatingMessage->value.u.resourceStatusReporting;

		asn1Init_x2ap_ResourceStatusUpdate(p_x2ap_msg);
		if (RT_OK != asn1Append_x2ap_ResourceStatusUpdate_protocolIEs_1(
					pctxt,
					&p_x2ap_msg->protocolIEs,
					p_resource_status_update->eNB1_measurement_Id))
		{
			break;
		}

		if (RT_OK != asn1Append_x2ap_ResourceStatusUpdate_protocolIEs_2(
					pctxt,
					&p_x2ap_msg->protocolIEs,
					p_resource_status_update->eNB2_measurement_Id))
		{
			break;
		}

		if (RT_OK != asn1Append_x2ap_ResourceStatusUpdate_protocolIEs_3(
					pctxt,
					&p_x2ap_msg->protocolIEs,
					&p_resource_status_update->cell_meas_result_list))
		{
			break;
		}
		/* ASN Encode message */
		pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

		if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
			break;
		}

		*p_buff_size = (U16)pe_GetMsgLen(pctxt);

		/* Print the asn encoded message */
		
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : Resource Status Request", &x2ap_pdu);

		result=X2AP_SUCCESS;
	} while(0);
	X2AP_UT_TRACE_EXIT();
	return result;
}
/****************************************************************************
 * Function Name  : x2ap_rsu_process_update_msg
 * Inputs         : p_msg_ptr -     Pointer to message buffer
 *                : p_x2ap_gb_ctx -  pointer to X2AP global context
 *                : p_enb_ctx   -   Pointer to peer eNB context
 * Outputs        : X2AP_SUCCESS or X2AP_FAILURE
 * Returns        : None
 * Description    : This function is called to process the resource status 
 *                  mesaage received from peer enb. 
 ****************************************************************************/                  
void x2ap_rsu_process_update_msg(U8 *p_msg_ptr, 
		x2ap_gb_context_t  *p_x2ap_gb_ctx, 
		x2ap_peer_enb_context_t *p_enb_ctx)
{
	rrc_x2ap_rsu_update_t        x2ap_rsu_update;
	x2ap_peer_enb_msg_t          *p_peer_enb_msg = X2AP_P_NULL;
	U8                           *p_asn_msg;
	U16                          *p_asn_msg_len;
	x2ap_X2AP_PDU                x2ap_pdu;
	OSCTXT                       asn1_ctx;
	x2ap_ResourceStatusUpdate    *p_ResourceStatusUpdate = X2AP_P_NULL;

	X2AP_UT_TRACE_ENTER();

	p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;
	/* Received pointer should not be NULL */
	X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
	X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
	X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);

	p_asn_msg = (p_peer_enb_msg)->p_msg;
	p_asn_msg_len = (U16*)&(p_peer_enb_msg->msg_len);

	/* Init ASN1 context */
	if ( 0 != rtInitContext(&asn1_ctx) )
	{
		/* Drop message */
		X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
				"context initialization failed.",__FUNCTION__);
		return ;
	}

	/*Set pointer of asn buffer in asn context*/
	pu_setBuffer(&asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

	if (0 != asn1PD_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
	{
		/* Drop message */
		X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
				"decode failed.",__FUNCTION__);
		/* Send Transfer Syntax Error */
		x2ap_build_and_send_err_indication(p_x2ap_gb_ctx, p_enb_ctx, PNULL);
        /* SPR 16118 Fix Start */
        /* Free ASN Context */
        rtFreeContext(&asn1_ctx);
        /* SPR 16118 Fix End */
		return ;
	}
	else
	{
		X2AP_TRACE(X2AP_INFO,"X2AP_MSG : Resource Status Update \n");
		
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : Resource Status Request", &x2ap_pdu);
	}

	if (T_x2ap_X2AP_PDU_initiatingMessage == x2ap_pdu.t)
	{
		p_ResourceStatusUpdate = x2ap_pdu.u.initiatingMessage->value.u.resourceStatusReporting;
		if (X2AP_FAILURE ==
				x2ap_rsu_process_update(&asn1_ctx,p_ResourceStatusUpdate,&x2ap_rsu_update,p_x2ap_gb_ctx,p_enb_ctx))
		{
            /* SPR 16118 Fix Start */
            X2AP_TRACE(X2AP_INFO,"x2ap_rsu_process_update failed \n");
            /* SPR 16118 Fix End */
		}
	}
	else
	{
		X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
				"Invalid Message received.",__FUNCTION__);
        /* SPR 16118 Fix Start */
        /* SPR 16118 Fix End */
	}
	/*klockwork Fix*/
    /* SPR 16118 Fix Start */
    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    /* SPR 16118 Fix End */

	/*klockwork Fix*/
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_process_update 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_ResourceStatusUpdate
 *                  p_trgt - pointer to rrc_x2ap_rsu_update_t
 *                  p_x2ap_gb_ctx - pointer to X2ap global context
 *                  p_enb_ctx - pointer to peer eNB context 
 * Outputs        : p_trgt - decoded message and processing
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function used to process the X2AP:RESOURCE STATUS UPDATE
 *               received by the peer eNb..
 ********************************************************************************/
x2ap_return_et x2ap_rsu_process_update
(
 OSCTXT      *p_asn1_ctx,
 x2ap_ResourceStatusUpdate    *p_3gpp,    /* unpacked src */
 rrc_x2ap_rsu_update_t         *p_trgt,    /* dest */
 x2ap_gb_context_t             *p_x2ap_gb_ctx,      /* X2ap global context*/
 x2ap_peer_enb_context_t          *p_enb_ctx
 )
{

	x2ap_return_et result = X2AP_SUCCESS;
	U32 trans_Id = X2AP_NULL;

	x2ap_rrm_rsu_enb_update_ind_t    x2ap_rrm_rsu_rrm_update; 

	/* Get tree node */
	rsu_enb_meas_id_search_node_t* p_rsu_tree_node = X2AP_P_NULL;
	x2ap_rsu_context_t*     p_x2ap_rsu_ctx = X2AP_P_NULL;

	X2AP_UT_TRACE_ENTER();

	memset_wrapper(&x2ap_rrm_rsu_rrm_update, X2AP_NULL, sizeof(x2ap_rrm_rsu_enb_update_ind_t));

	/*1. Decode the asn message received */
	if(X2AP_FAILURE == x2ap_rsu_decode_update_msg(p_x2ap_gb_ctx, p_enb_ctx,
				p_asn1_ctx, p_3gpp,
				p_trgt,&x2ap_rrm_rsu_rrm_update))
	{
		X2AP_TRACE(X2AP_ERROR,"%s: X2AP: RESOURCE STATUS UPDATE and decode failed.",__FUNCTION__);
		return X2AP_FAILURE;
	}

	else 
	{
		/*Fetching the RSU measurement context from the tree*/
		/*Get the mesurement id from the context*/
		p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),
				&(p_trgt->eNB1_measurement_Id));

		if(X2AP_P_NULL != p_rsu_tree_node)
		{
			/* get the measurement context */
			p_x2ap_rsu_ctx = &p_rsu_tree_node->x2ap_rsu_ctx;
		}
		else
		{
			X2AP_TRACE(X2AP_ERROR, "[%s]: Measurement Context does not exist for this Measurement ID ", __FUNCTION__);
			return X2AP_FAILURE;
		}

		/* Stop the timer X2AP_RSU_ENB_TIMER. -------- Need to confirm */  
		//x2ap_stop_timer(&p_x2ap_rsu_ctx->timer_id);
		//p_x2ap_rsu_ctx->timer_id = X2AP_P_NULL;

		trans_Id = p_x2ap_rsu_ctx->rrm_transaction_id;

		/* 3. Validate the measurement context after fetching the context */
		if(p_x2ap_rsu_ctx->enb2_meas_id == p_trgt->eNB2_measurement_Id)
		{

			if( p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_LISTEN_RSU)
			{
				/* send X2AP_RRM_RSU_ENB_UPDATE_IND to RRM.*/
				if(RRC_FAILURE == rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_update_ind(&x2ap_rrm_rsu_rrm_update,
							RRC_X2AP_MODULE_ID, RRC_RRM_MODULE_ID, trans_Id, RRC_NULL))
				{
					X2AP_TRACE(X2AP_ERROR,"Sending X2AP_RRM_RSU_ENB_UPDATE_IND to RRM Failed\n ");
					result = X2AP_FAILURE;
				}    
			}
			else
			{
				X2AP_TRACE(X2AP_ERROR, "X2AP RSU Inavalid State --> [%d]",p_x2ap_rsu_ctx->state);
				result = X2AP_FAILURE;

			}

		}
	}
	return result;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_decode_update_msg 
 * Inputs         : p_x2ap_gl_ctx - pointer to X2AP global context
 *                  p_enb_ctx - pointer to  peer enb context
 *                  p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_ResourceStatusUpdate
 *                  p_msg - pointer to rrc_x2ap_rsu_update_t
 *                  p_x2ap_rrm_rsu_rrm_update - pointer to 
 *                                              x2ap_rrm_rsu_enb_update_ind_t
 * Outputs        : p_msg - decoded message 
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function used to decode the X2AP:RESOURCE STATUS UPDATE
 *                  received by the peer eNb
 ********************************************************************************/
x2ap_return_et x2ap_rsu_decode_update_msg
(
 x2ap_gb_context_t  *p_x2ap_gl_ctx,
 x2ap_peer_enb_context_t *p_enb_ctx,
 OSCTXT      *p_asn1_ctx,
 x2ap_ResourceStatusUpdate *p_3gpp,    /* unpacked src */
 rrc_x2ap_rsu_update_t    *p_msg,    /* dest */
 x2ap_rrm_rsu_enb_update_ind_t *p_x2ap_rrm_rsu_rrm_update
 )
{
	U32  index = X2AP_NULL;
	OSRTDListNode  *p_node     = PNULL;
	x2ap_ResourceStatusUpdate_protocolIEs_element *p_protocol_IE = PNULL;

	x2ap_return_et       retVal = X2AP_SUCCESS;
	x2ap_error_ind_ie_list_t   iE_list;
	x2ap_error_ind_bool_t send_err_ind;
	U16 ie_list_index = 0;
	x2ap_bool_et send_tse = X2AP_FALSE;

	x2ap_message_data_t message_map =
	{3, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
		{{0, ASN1V_x2ap_id_ENB1_Measurement_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{1, ASN1V_x2ap_id_ENB2_Measurement_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
			{2, ASN1V_x2ap_id_CellMeasurementResult, x2ap_mandatory,x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
		}};

	X2AP_UT_TRACE_ENTER();
	X2AP_MEMSET(&iE_list, X2AP_NULL, sizeof(x2ap_error_ind_ie_list_t));
	X2AP_MEMSET(&send_err_ind, X2AP_FALSE, sizeof(x2ap_error_ind_bool_t));


	X2AP_ASSERT(PNULL != p_3gpp);
	X2AP_ASSERT(PNULL != p_msg);

	/*Fill the peer enb ID */
	p_x2ap_rrm_rsu_rrm_update->peer_gb_enb_id = p_enb_ctx->gb_enb_id;

	do
	{
		p_node = p_3gpp->protocolIEs.head;
		/* Save the decoded Protocol IEs */
		for (index =0; index < p_3gpp->protocolIEs.count; index++)
		{
			if (!p_node)
			{
				X2AP_TRACE(X2AP_ERROR,"%s: "
						"next node returned NULL",
						__FUNCTION__);
				break;
			}
			p_protocol_IE =
				(x2ap_ResourceStatusUpdate_protocolIEs_element *)
				p_node->data;
			switch (p_protocol_IE->id)
			{
				case ASN1V_x2ap_id_ENB1_Measurement_ID:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									&p_protocol_IE->value.u._x2ap_ResourceStatusUpdate_IEs_1))
						{
							p_msg->eNB1_measurement_Id = p_protocol_IE->value.u._x2ap_ResourceStatusUpdate_IEs_1;
							p_x2ap_rrm_rsu_rrm_update->enb_meas_id = p_msg->eNB1_measurement_Id;/* Need to Confirm */
						}
					}
					break;

				case ASN1V_x2ap_id_ENB2_Measurement_ID:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									&p_protocol_IE->value.u._x2ap_ResourceStatusUpdate_IEs_2))
						{
							p_msg->eNB2_measurement_Id = p_protocol_IE->value.u._x2ap_ResourceStatusUpdate_IEs_2;
						}
					}
					break;

				case ASN1V_x2ap_id_CellMeasurementResult:
					{
						if (X2AP_SUCCESS == x2ap_validate_ie_value(
									&message_map,
									index,
									p_protocol_IE->id,
									p_protocol_IE->value.u._x2ap_ResourceStatusUpdate_IEs_3))
						{
							p_msg->cell_meas_result_list = 
								*p_protocol_IE->value.u._x2ap_ResourceStatusUpdate_IEs_3;

							/* Now decode the Cell Measurement result list */
							if(X2AP_FAILURE == x2ap_rsu_decode_measurement_result_list(
										&(p_msg->cell_meas_result_list),
										p_x2ap_rrm_rsu_rrm_update,
										&message_map,
										(U8)index,
										(U8)p_protocol_IE->id,
										&send_tse))
							{
								X2AP_TRACE(X2AP_INFO, "%s:decode Measurement Result"
										"Failed" ,__FUNCTION__);
								return X2AP_FAILURE;
							}
							/* This will only happen when the element list exceeds
							 * the max number */
							if (X2AP_TRUE == send_tse)
							{
								X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
										"in ASN.1 Sequence, Sending Transfer Syntax Error");
								x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
								retVal = X2AP_FAILURE;
								break;
							}
						}

						break;
					}

				default:
					/* Class - 1 Error - Unknown IE */
					/* Base on Criticality fill the value */
					X2AP_TRACE(X2AP_ERROR, "%s:"\
							"invalid Protocol IE id",__FUNCTION__);
					x2ap_add_to_err_ind_ie_list(&iE_list,
							p_protocol_IE->criticality,
							p_protocol_IE->id,
							&ie_list_index,
							&send_err_ind,
							X2AP_FALSE,
							X2AP_FALSE);
					/*SPR_19067_FIX_START*/
                    if (x2ap_reject == p_protocol_IE->criticality) 
                    {
                        retVal = X2AP_FAILURE;
                    }
                    /*SPR_19067_FIX_STOP*/
			}
			p_node= p_node->next;
		}
	}while(0);
	/* Parse the map for Error Indication */
	if ((X2AP_SUCCESS == x2ap_parse_message_map(
					p_asn1_ctx,
					p_x2ap_gl_ctx,
					p_enb_ctx,
					&message_map,
					&iE_list,
					&ie_list_index,
					&send_err_ind,
					ASN1V_x2ap_id_resourceStatusReporting,
					T_x2ap_X2AP_PDU_initiatingMessage,
					x2ap_ignore,
					(rrc_x2ap_error_indication_t *)PNULL))
	   )
	{
		X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED in rsu_decode_update_msg ");

		retVal = X2AP_FAILURE;
	}

	X2AP_UT_TRACE_EXIT();

	return retVal;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_decode_measurement_result_list 
 * Inputs         : p_cell_meas_result_list - pointer to 
 *                                            x2ap_CellMeasurementResult_List
 *                  p_rsu_update_list - pointer to x2ap_rrm_rsu_enb_update_ind_t
 *                  p_ie_order_map - pointer to x2ap_message_data_t
 *                  order_index - order index
 *                  id - id
 *                  p_send_tse - value of range from x2ap_bool_et
 * Outputs        : p_rsu_update_list 
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function decodes and fill the Meas result list into 
 *                  Structure
 ********************************************************************************/

x2ap_return_et x2ap_rsu_decode_measurement_result_list(
		x2ap_CellMeasurementResult_List *p_cell_meas_result_list,
		x2ap_rrm_rsu_enb_update_ind_t     *p_rsu_update_list,
		x2ap_message_data_t *p_ie_order_map,
		U8 order_index,
		U8 id,
		x2ap_bool_et *p_send_tse)
{
	OSRTDListNode* p_node = PNULL;
	x2ap_return_et ret = X2AP_SUCCESS;
	x2ap_CellMeasurementResult_List_element *p_elem = X2AP_P_NULL;
	/* eICIC changes start */
	OSRTDListNode* p_node2 = PNULL;
	x2ap_CellMeasurementResult_Item_iE_Extensions_element *p_elem2 = X2AP_P_NULL;
    /*Bug 481 Start */
    U32 count = 0; 
    /*Bug 481 End */
    /* eICIC_Changes_End */

	X2AP_UT_TRACE_ENTER();
	X2AP_ASSERT(X2AP_P_NULL!=p_rrc_x2ap_rsu_update);
	X2AP_ASSERT(X2AP_P_NULL!=p_cell_meas_result_list);
	X2AP_ASSERT(X2AP_P_NULL!=p_rsu_update_list);

	/* SPR 13041 Fix Start */
	if(MAX_CELLIN_ENB < p_cell_meas_result_list->count)
	{
		*p_send_tse = X2AP_TRUE;
		X2AP_UT_TRACE_EXIT();
		return X2AP_FAILURE; 
		/* SPR 13041 Fix Stop */
	}

	/* SPR 13041 Fix Start */
	if (MAX_SERVED_CELLS < p_cell_meas_result_list->count)
	{
		X2AP_TRACE(X2AP_WARNING,"%s: ""Served Cells greater than 8 are not supported ",
				__FUNCTION__);
	}

	for( p_node = p_cell_meas_result_list->head,
			p_rsu_update_list->num_reports = 0; (X2AP_P_NULL!=p_node) &&
			(p_rsu_update_list->num_reports < MAX_SERVED_CELLS );
			p_node=p_node->next,p_rsu_update_list->num_reports++) /* SPR 13041 Fix Stop */
	{
		p_elem = (x2ap_CellMeasurementResult_List_element*)p_node->data;
		if (PNULL == p_elem)
		{
			continue;
		}
		if (ASN1V_x2ap_id_CellMeasurementResult_Item != p_elem->id)
		{
			X2AP_TRACE(X2AP_WARNING,
					"ASN1V_x2ap_id_CellMeasurementResult_Item != p_elem"
					"->id (%i)",
					p_elem->id);
			x2ap_update_message_map(p_ie_order_map, DATA_MISSING, order_index, id);
			ret = X2AP_FAILURE;
			break;

		}
		/* Copying PLMN identity */
		x2ap_parse_asn_plmn_identity(&p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].cell_info.plmn_identity,
				((x2ap_CellMeasurementResult_List_element *)(p_node->data))->value.cell_ID.pLMN_Identity.data);

		/* Copying EUTRAN-ID */
		/*SPR 11666 Start */
		X2AP_MEMCPY(&p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].cell_info.cell_identity,
				((x2ap_CellMeasurementResult_List_element *)(p_node->data))->value.cell_ID.eUTRANcellIdentifier.data,
				HOME_ENB_ID_OCTET_SIZE);
		/*SPR 11666 Stop */

		if (p_elem->value.m.radioResourceStatusPresent)
		{
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.bitmask |= PRB_PERIODIC_REPORT_PRESENT; 
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.radio_resource_status.dl_gbr_prb_usage = 
				p_elem->value.radioResourceStatus.dL_GBR_PRB_usage;         
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.radio_resource_status.ul_gbr_prb_usage =  
				p_elem->value.radioResourceStatus.uL_GBR_PRB_usage;         
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.radio_resource_status.dl_non_gbr_prb_usage = 
				p_elem->value.radioResourceStatus.dL_non_GBR_PRB_usage;           
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.radio_resource_status.ul_non_gbr_prb_usage = 
				p_elem->value.radioResourceStatus.uL_non_GBR_PRB_usage;          
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.radio_resource_status.dl_total_gbr_prb_usage = 
				p_elem->value.radioResourceStatus.dL_Total_PRB_usage;           
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.radio_resource_status.ul_total_gbr_prb_usage = 
				p_elem->value.radioResourceStatus.uL_Total_PRB_usage;          
		}

		if (p_elem->value.m.s1TNLLoadIndicatorPresent)
		{
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.bitmask |= TNL_LOAD_IND_PERIODIC_REPORT_PRESENT;
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.s1_tnl_load_indicator.dl_s1_tnl_load_indicator = 
				p_elem->value.s1TNLLoadIndicator.dLS1TNLLoadIndicator;
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.s1_tnl_load_indicator.ul_s1_tnl_load_indicator = 
				p_elem->value.s1TNLLoadIndicator.uLS1TNLLoadIndicator;
		}
		if (p_elem->value.m.hWLoadIndicatorPresent)
		{
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.bitmask |= LOAD_IND_PERIODIC_REPORT_PRESENT;
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.hardware_load_indicator.dl_hardware_load_indicator = 
				p_elem->value.hWLoadIndicator.dLHWLoadIndicator;      
			p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.hardware_load_indicator.ul_hardware_load_indicator = 
				p_elem->value.hWLoadIndicator.uLHWLoadIndicator;      
		}

		if (p_elem->value.m.iE_ExtensionsPresent)
		{
			/* eICIC changes start */
			p_node2 = p_elem->value.iE_Extensions.head;
           /*Bug 481 Start */
	   for (count=0; count<p_elem->value.iE_Extensions.count; count++)
	   {
	   if (!p_node2)
	   {
	      X2AP_TRACE(X2AP_ERROR, "%s: ""next node returned NULL",
	            __FUNCTION__);        
              ret = X2AP_FAILURE;
              break;
	   }
           /*Bug 481 End */
			p_elem2 = (x2ap_CellMeasurementResult_Item_iE_Extensions_element*)p_node2->data;
			if (PNULL == p_elem2)
			{
				continue;
			}
			if (ASN1V_x2ap_id_CompositeAvailableCapacityGroup != p_elem2->id && ASN1V_x2ap_id_ABS_Status != p_elem2->id)
			{
				X2AP_TRACE(X2AP_WARNING,
						"ASN1V_x2ap_id_CompositeAvailableCapacityGroup != p_elem2 and ASN1V_x2ap_id_ABS_Status != p_elem2->id"
						"->id (%i)",
						p_elem2->id);
				x2ap_update_message_map(p_ie_order_map, DATA_MISSING, order_index, id);
				ret = X2AP_FAILURE;
				break;

			}
			if(T47x2ap___x2ap_CellMeasurementResult_Item_ExtIEs_1 == p_elem2->extensionValue.t)
			{
				p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.bitmask |= COMPOSITE_AVAILABLE_CAPACITY_PERIODIC_REPORT_PRESENT;

				/*Call the function to decode the CACG list*/
				if(X2AP_FAILURE ==
						x2ap_rsu_decode_cacg_list(
							&(p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].
								rsu_info.composite_available_capacity_group),
							&(p_elem->value.iE_Extensions)))
				{
					X2AP_TRACE(X2AP_ERROR, "CACG decode failure\n");
					ret = X2AP_FAILURE;
				} 
			}
			else if(T47x2ap___x2ap_CellMeasurementResult_Item_ExtIEs_2 == p_elem2->extensionValue.t)
			{
				p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].rsu_info.bitmask |= ABS_STATUS_PERIODIC_REPORT_PRESENT;

				/*Call the function to decode the ASPR list*/
				if(X2AP_FAILURE ==
						x2ap_rsu_decode_abs_status_list(
							&(p_rsu_update_list->rsu_report[p_rsu_update_list->num_reports].
								rsu_info.abs_status),
							&(p_elem->value.iE_Extensions)))
				{
					X2AP_TRACE(X2AP_ERROR, "ABS Status Periodic Report : ABS pattern received contains no useful information\n");
					ret = X2AP_FAILURE;
				} 
			}
           /*Bug 481 Start */
	   p_node2 = p_node2->next; 
           /*Bug 481 End */
	   }// end of for loop
           /* eICIC_Changes_End */
		}
	}

	X2AP_UT_TRACE_EXIT();
	return ret;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_decode_cacg_list 
 * Inputs         : p_x2ap_rsu_cacg - pointer to x2ap_rrm_rsu_cacg_t
 *                  p_cacg_list - pointer to 
 *                                  x2ap_CellMeasurementResult_Item_iE_Extensions
 * Outputs        : p_x2ap_rsu_cacg 
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE 
 * Description    : This function decodes and fill the Meas result list into 
 *                  Structure
 ********************************************************************************/

x2ap_return_et x2ap_rsu_decode_cacg_list(
		x2ap_rrm_rsu_cacg_t     *p_x2ap_rsu_cacg,
		x2ap_CellMeasurementResult_Item_iE_Extensions *p_cacg_list)
{
	OSRTDListNode* p_node = PNULL;
	x2ap_CellMeasurementResult_Item_iE_Extensions_element *p_elem = X2AP_P_NULL;


	U16 cacg_counter = 0;
	X2AP_UT_TRACE_ENTER();
	X2AP_ASSERT(X2AP_P_NULL!=p_x2ap_rsu_cacg);
	X2AP_ASSERT(X2AP_P_NULL!=p_cacg_list);

	p_node = p_cacg_list->head;

	for (cacg_counter = 0; cacg_counter < p_cacg_list->count; cacg_counter++) 
	{
		X2AP_ASSERT(X2AP_P_NULL != p_node);
		X2AP_ASSERT(X2AP_P_NULL != p_node->data);

		p_elem = (x2ap_CellMeasurementResult_Item_iE_Extensions_element *)p_node->data;
		if (PNULL == p_elem)
		{
			continue;
		}

		p_x2ap_rsu_cacg->dl_composite_available_capacity.cell_capacity_value =  p_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1->dL_CompositeAvailableCapacity.capacityValue;
		if (p_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1->dL_CompositeAvailableCapacity.m.cellCapacityClassValuePresent)
		{
			p_x2ap_rsu_cacg->dl_composite_available_capacity.bitmask = CELL_CAPACITY_CLASS_VALUE_PRESENT;
			p_x2ap_rsu_cacg->dl_composite_available_capacity.cell_capacity_class_value = p_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1->dL_CompositeAvailableCapacity.cellCapacityClassValue;
		}

		p_x2ap_rsu_cacg->ul_composite_available_capacity.cell_capacity_value = p_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1->uL_CompositeAvailableCapacity.capacityValue;
		if (p_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1->uL_CompositeAvailableCapacity.m.cellCapacityClassValuePresent)
		{   
			p_x2ap_rsu_cacg->ul_composite_available_capacity.bitmask = CELL_CAPACITY_CLASS_VALUE_PRESENT;
			p_x2ap_rsu_cacg->ul_composite_available_capacity.cell_capacity_class_value = p_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_1->uL_CompositeAvailableCapacity.cellCapacityClassValue;
		}
		cacg_counter++;
		p_node = p_node->next;
	}    
	return X2AP_SUCCESS;
}
/* eICIC changes start */
/*************************************************************************************
 * Function Name  : x2ap_rsu_decode_abs_status_list 
 * Inputs         : p_x2ap_rsu_aspr - pointer to x2ap_rrm_abs_status_t 
 *                  p_aspr_list     - pointer to 
 *                                    x2ap_CellMeasurementResult_Item_iE_Extensions
 * Outputs        : p_x2ap_rsu_aspr 
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE 
 * Description    : This function decodes and fill the Meas result list into 
 *                  Structure
 *************************************************************************************/

x2ap_return_et x2ap_rsu_decode_abs_status_list(
		x2ap_rrm_abs_status_t     *p_x2ap_rsu_aspr,
		x2ap_CellMeasurementResult_Item_iE_Extensions *p_aspr_list)
{
	OSRTDListNode* p_node = PNULL;
	x2ap_CellMeasurementResult_Item_iE_Extensions_element *p_elem = X2AP_P_NULL;

	U8 count = X2AP_NULL;
	U16 aspr_counter = X2AP_NULL;
	/* eICIC code changes start */
	U8 zero_flag = X2AP_TRUE;
	/* eICIC code changes stop */
	X2AP_UT_TRACE_ENTER();
	X2AP_ASSERT(X2AP_P_NULL!=p_x2ap_rsu_aspr);
	X2AP_ASSERT(X2AP_P_NULL!=p_aspr_list);

	p_node = p_aspr_list->head;

	for (aspr_counter = 0; aspr_counter < p_aspr_list->count; aspr_counter++) 
	{
		X2AP_ASSERT(X2AP_P_NULL != p_node);
		X2AP_ASSERT(X2AP_P_NULL != p_node->data);
		/* eICIC code changes start */
		zero_flag = X2AP_TRUE;
		/* eICIC code changes stop */
		p_elem = (x2ap_CellMeasurementResult_Item_iE_Extensions_element *)p_node->data;
		if (PNULL == p_elem)
		{
			continue;
		}

		/* eICIC_Changes_Start */
                /*Bug 481 Start */
		if(T47x2ap___x2ap_CellMeasurementResult_Item_ExtIEs_1 == p_elem->extensionValue.t)
		{
		   
                        X2AP_TRACE(X2AP_INFO, "Composite Group Node");
		        p_node = p_node->next;
			continue;
		}
                 /*Bug 481 End */
		/* eICIC_Changes_End */

		p_x2ap_rsu_aspr->dl_abs_status =  
			p_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->dL_ABS_status;

		if (T_x2ap_UsableABSInformation_fdd == p_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.t)
		{
			p_x2ap_rsu_aspr->x2ap_usable_abs_information.bitmask = X2AP_USABLE_ABS_INFORMATION_FDD_PRESENT;

			for(count = 0; count < X2AP_ABS_PATTERN_SIZE_FDD; count++)
			{
				p_x2ap_rsu_aspr->x2ap_usable_abs_information.usable_abs_info_fdd.usable_abs_pattern_fdd_info[count] = 
					p_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.u.fdd->usable_abs_pattern_info.data[count];
				/* eICIC code changes start */
				if(X2AP_NULL != (X2AP_NULL | 
							(p_x2ap_rsu_aspr->x2ap_usable_abs_information.usable_abs_info_fdd.usable_abs_pattern_fdd_info[count])))
				{
					zero_flag = X2AP_FALSE;
				}
				/* eICIC code changes stop */
			}

		}
		else if (T_x2ap_UsableABSInformation_tdd == p_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.t)
		{   
			p_x2ap_rsu_aspr->x2ap_usable_abs_information.bitmask = X2AP_USABLE_ABS_INFORMATION_TDD_PRESENT;

			for(count = 0; count < X2AP_ABS_PATTERN_SIZE_TDD; count++)
			{
				p_x2ap_rsu_aspr->x2ap_usable_abs_information.usable_abs_info_tdd.usable_abs_pattern_tdd_info[count] =
					p_elem->extensionValue.u._x2ap_CellMeasurementResult_Item_ExtIEs_2->usableABSInformation.u.
					tdd->usaable_abs_pattern_info.data[count];
				/* eICIC code changes start */
				if(X2AP_NULL != (X2AP_NULL |
							(p_x2ap_rsu_aspr->x2ap_usable_abs_information.usable_abs_info_tdd.usable_abs_pattern_tdd_info[count])))
				{
					zero_flag = X2AP_FALSE;
				}
				/* eICIC code changes stop */
			}
		}
		if(X2AP_TRUE == zero_flag)
		{
			X2AP_UT_TRACE_EXIT();
			return X2AP_FAILURE;
		}
		p_node = p_node->next;
	}   
	X2AP_UT_TRACE_EXIT();
	return X2AP_SUCCESS;
}
/* eICIC changes stop */
/*****************************************************************************
 * Function Name  : x2ap_process_rrm_rsu_start_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the Resource Status start request
 *                  received from RRM module.
 ****************************************************************************/
void x2ap_process_rrm_rsu_start_req(
		void             *p_api,
		x2ap_gb_context_t   *p_x2ap_gb_ctx)
{
	x2ap_rrm_rsu_rrm_start_req_t *p_rrm_rsu_start_req = X2AP_P_NULL;
	x2ap_peer_enb_context_t *p_peer_enb_ctx = X2AP_NULL;/*x2ap_rsu_gb_enb_id_fix*/
	S32 length_read = X2AP_NULL;
	x2ap_return_et response = X2AP_FAILURE;
	U16 trans_id = X2AP_NULL;
	x2ap_timer_buffer_t  x2ap_rsu_res_status_tim_buf;
	U8 peer_enodeb_id = X2AP_NULL;
	U32 meas_id = 0;
	U8 counter = X2AP_NULL;/*x2ap_rsu_gb_enb_id_fix*/

	X2AP_UT_TRACE_ENTER();
	X2AP_ASSERT(X2AP_P_NULL != p_api);
	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

	X2AP_MEMSET (&x2ap_rsu_res_status_tim_buf, X2AP_NULL, sizeof(x2ap_timer_buffer_t));
	/* Get the transaction id */
	trans_id = x2ap_get_transaction_id(p_api);


	p_rrm_rsu_start_req = (x2ap_rrm_rsu_rrm_start_req_t*)
		x2ap_mem_get(sizeof(x2ap_rrm_rsu_rrm_start_req_t));
	if(X2AP_P_NULL == p_rrm_rsu_start_req)
	{
		X2AP_TRACE(X2AP_FATAL,"\nMemeory allocation failed ");
		return;
	}

	/* Decode the received message from RRM */
	response = rrc_il_parse_x2ap_rrm_rsu_rrm_start_req(
			p_rrm_rsu_start_req ,
			(U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
			x2ap_get_api_buf_size(p_api) - 
			RRC_FULL_INTERFACE_HEADERS_SIZE,
			&length_read);

	if(X2AP_SUCCESS == response)
	{

		/* 2. Validate Target Cell IDs. If invalid, return failure. */
		if (X2AP_FAILURE == x2ap_validate_peer_cell_info_and_get_peer_enb_context(p_x2ap_gb_ctx,
					p_rrm_rsu_start_req,
					&peer_enodeb_id))

		{
			X2AP_TRACE(X2AP_WARNING, "[%s]: Error ! resource status Request(START) -- Target Cell ID mismatch\n",__FUNCTION__);
			x2ap_rsu_send_rrm_start_resp(trans_id,X2AP_FAILURE,meas_id,x2ap_cell_not_available, X2AP_NULL /*x2ap_rsu_gb_enb_id_fix*/
					,X2AP_P_NULL,X2AP_NULL); 
            /* SPR 20007 : CID 110663 fix start */
            if (PNULL != p_rrm_rsu_start_req)
            {
                x2ap_mem_free(p_rrm_rsu_start_req);
            }
            /* SPR 20007 : CID 110663 fix end */
			return;
		}
		/*x2ap_rsu_gb_enb_id_fix Start*/        
		/* fetch the peer enodeB context using peer enb id as key */
		for(counter = 0 ; counter < MAX_PEER_ENB ; counter++)
		{
			/* Bug:9129 */
			if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].peer_enodeb_id == peer_enodeb_id)
					&& (p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].op_status == OPERATIONAL))
				/* Bug:9129 */
			{
				p_peer_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
				break;

			} 
		}    
		/*x2ap_rsu_gb_enb_id_fix Stop*/
		/* 3. Verify that RRM has requested atleast one of the following 4 reports. */
		if ( 0 == p_rrm_rsu_start_req->report_characteristics )
		{
			X2AP_TRACE(X2AP_WARNING, "[%s]: Error ! resource status Request(START) -- Report characteristics empty\n",__FUNCTION__);
			/*x2ap_rsu_gb_enb_id_fix Start*/
			x2ap_rsu_send_rrm_start_resp(trans_id, X2AP_FAILURE, meas_id,
					x2ap_reportCharacteristicsEmpty, p_peer_enb_ctx /*x2ap_rsu_gb_enb_id_fix*/
					,X2AP_P_NULL,X2AP_NULL);
            /* SPR 20007 : CID 110663 fix start */
            if (PNULL != p_rrm_rsu_start_req)
            {
                x2ap_mem_free(p_rrm_rsu_start_req);
            }
            /* SPR 20007 : CID 110663 fix end */
			return;
		}

		/* Calling the  Event Handler to handle the below steps */
		if (X2AP_FAILURE ==
				x2ap_rsu_res_status_rrm_start_req_evt_hdl(p_x2ap_gb_ctx,trans_id,p_rrm_rsu_start_req,peer_enodeb_id ))
		{
			x2ap_rsu_send_rrm_start_resp(trans_id, X2AP_FAILURE, meas_id,
					x2ap_unspecified_2, p_peer_enb_ctx /*x2ap_rsu_gb_enb_id_fix*/
					,X2AP_P_NULL,X2AP_NULL);
		}

	} 
	else
	{
		X2AP_TRACE(X2AP_WARNING, "x2ap RRM RSU Start Request decode failed");
		x2ap_rsu_send_rrm_start_resp(trans_id, X2AP_FAILURE, meas_id, x2ap_unspecified_2, X2AP_NULL /*x2ap_rsu_gb_enb_id_fix*/
				,X2AP_P_NULL,X2AP_NULL);
	}
    /* SPR 20007 : CID 110663 fix start */
    if (PNULL != p_rrm_rsu_start_req)
    {
        x2ap_mem_free(p_rrm_rsu_start_req);
    }
    /* SPR 20007 : CID 110663 fix end */
	X2AP_UT_TRACE_EXIT();
}
/*****************************************************************************
 * Function Name  : x2ap_process_rrm_rsu_stop_req 
 * Inputs         : p_api - pointer to input api data
 *                  p_x2ap_gb_ctx - pointer to x2AP global context
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function process the X2AP_RRM_RSU_RRM_STOP_REQ received
 *                  from the rrm
 ********************************************************************************/

x2ap_return_et x2ap_process_rrm_rsu_stop_req( void             *p_api,
		x2ap_gb_context_t   *p_x2ap_gb_ctx)

{
	x2ap_rrm_rsu_rrm_stop_req_t *p_rrm_rsu_stop_req;
	S32 length_read = X2AP_NULL;
	x2ap_return_et response = X2AP_FAILURE;
	U16 trans_id = X2AP_NULL;
	x2ap_timer_t timer_id;
	U8 counter = 0;

	/* Get tree node */ 
	rsu_enb_meas_id_search_node_t* p_rsu_tree_node = X2AP_P_NULL;   
	x2ap_rsu_context_t*     p_x2ap_rsu_ctx = X2AP_P_NULL;

	x2ap_peer_enb_context_t *p_enb_ctx = X2AP_P_NULL;

	X2AP_UT_TRACE_ENTER();
	X2AP_ASSERT(X2AP_P_NULL != p_api);
	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

	/* Get the transaction id */
	trans_id = x2ap_get_transaction_id(p_api);

	p_rrm_rsu_stop_req = (x2ap_rrm_rsu_rrm_stop_req_t*)
		x2ap_mem_get(sizeof(x2ap_rrm_rsu_rrm_stop_req_t));
	if(X2AP_P_NULL == p_rrm_rsu_stop_req)
	{
		X2AP_TRACE(X2AP_FATAL,"\nMemeory allocation failed ");
		return X2AP_FAILURE;
	}

	/* Decode the received message from RRM */
	response = rrc_il_parse_x2ap_rrm_rsu_rrm_stop_req(
			p_rrm_rsu_stop_req,
			(U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
			x2ap_get_api_buf_size(p_api) - 
			RRC_FULL_INTERFACE_HEADERS_SIZE,
			&length_read);

	if(X2AP_SUCCESS == response)
	{
		/*Get the mesurement id from the context*/

		p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),
				&(p_rrm_rsu_stop_req->enb_meas_id));

		if(X2AP_P_NULL != p_rsu_tree_node)
		{
			/* get the measurement context */
			p_x2ap_rsu_ctx = &p_rsu_tree_node->x2ap_rsu_ctx;       
			if (X2AP_P_NULL == p_x2ap_rsu_ctx)
			{
				X2AP_TRACE(X2AP_WARNING,
						"No Node found in RSU search tree for meas id: %d\n",
						p_rrm_rsu_stop_req->enb_meas_id);
                 /* SPR 15285 Fix Start */
                 x2ap_rsu_build_and_send_rrm_stop_resp(trans_id, X2AP_FAILURE, p_rrm_rsu_stop_req->enb_meas_id,
                                           p_rrm_rsu_stop_req->peer_gb_enb_id, x2ap_unknown_eNB_Measurement_ID);
                 /* SPR 15285 Fix End */
				/* coverity_fix_63258_start */
				x2ap_mem_free(p_rrm_rsu_stop_req);
				/* coverity_fix_63258_stop */
				return X2AP_FAILURE;
			}   
			for(counter = 0 ; counter < MAX_PEER_ENB ; counter++)
			{
				/* Bug:9129 */
				if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].peer_enodeb_id
							== p_x2ap_rsu_ctx->peer_enb_id) &&
						(p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].op_status == OPERATIONAL))
					/* Bug:9129 */
				{
					p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
					break;
				}
			}
			if(X2AP_P_NULL == p_enb_ctx)
			{
                 /* SPR 15285 Fix Start */
                 x2ap_rsu_build_and_send_rrm_stop_resp(trans_id, X2AP_FAILURE, p_rrm_rsu_stop_req->enb_meas_id,
                                             p_rrm_rsu_stop_req->peer_gb_enb_id, x2ap_unspecified_2);
                 /* SPR 15285 Fix End */
				X2AP_TRACE(X2AP_ERROR, "[%s]:Peer Enb Context is NULL while fetching from x2ap global context\n", __FUNCTION__);
				/* coverity_fix_63258_start */
				x2ap_mem_free(p_rrm_rsu_stop_req);
				/* coverity_fix_63258_stop */
				return X2AP_FAILURE;
			}
		}
		else
		{
		    /*SPR 14799 Fix Start*/
		    X2AP_TRACE(X2AP_ERROR,"[%s]: context does not exist for this measurement ID -- Sending FAILURE response to RRM ", __FUNCTION__);
		    x2ap_rsu_build_and_send_rrm_stop_resp(trans_id,X2AP_FAILURE,p_rrm_rsu_stop_req->enb_meas_id,
			    p_rrm_rsu_stop_req->peer_gb_enb_id,x2ap_unspecified_2);
		    /*SPR 14799 Fix Stop*/
		    /* coverity_fix_63258_start */
		    x2ap_mem_free(p_rrm_rsu_stop_req);
		    /* coverity_fix_63258_stop */
		    return X2AP_FAILURE;   
		}

		if(p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_LISTEN_RSU)
		{
		    /*Build and send the X2AP: RESOURCE STATUS REQUEST(STOP) to Peer Enb */
		    x2ap_rsu_build_and_send_stop_msg(p_x2ap_gb_ctx,p_x2ap_rsu_ctx,p_enb_ctx);

		    /*Start Timer : X2AP_RSU_ENB_GUARD_TIMER.*/
		    if (X2AP_SUCCESS == x2ap_rsu_start_timer(X2AP_RSU_ENB_GUARD_TIMER,
				p_enb_ctx,
				p_x2ap_gb_ctx->config_param.timer_conf_info.
				x2ap_rsu_enb_guard_tim_val,
				&timer_id,
				p_x2ap_rsu_ctx->enb1_meas_id)) 
		    {
			X2AP_TRACE(X2AP_INFO,"X2AP_RSU_ENB_GUARD_TIMER Started Successfully\n");

			/*Change the State to X2AP_RSU_STATE_STOP_ONGOING*/
			p_x2ap_rsu_ctx->state = X2AP_RSU_STATE_STOP_ONGOING ;

			/* save the timer id in RSU context */
			p_x2ap_rsu_ctx->timer_id = timer_id;
		    }
		    else
		    {
			/* coverity_fix_63258_start */
			x2ap_mem_free(p_rrm_rsu_stop_req);
			/* coverity_fix_63258_stop */
			return X2AP_FAILURE;
		    } 

		} 
		else
		{
		    X2AP_TRACE(X2AP_ERROR,
			    "Message Received in Invalid State [%d]\n", p_x2ap_rsu_ctx->state);
		    x2ap_rsu_build_and_send_rrm_stop_resp(p_x2ap_rsu_ctx->rrm_transaction_id,
			    X2AP_FAILURE,p_x2ap_rsu_ctx->enb1_meas_id,p_rrm_rsu_stop_req->peer_gb_enb_id,x2ap_unspecified_2);
		}
	}
	else
	{
	    X2AP_TRACE(X2AP_WARNING, "X2AP_RRM_RSU_RRM_STOP_REQ decode failed");
	    /* coverity_fix_63258_start */
	    x2ap_mem_free(p_rrm_rsu_stop_req);
	    /* coverity_fix_63258_stop */
	    return X2AP_FAILURE;
	}
	/* coverity_fix_63258_start */
	x2ap_mem_free(p_rrm_rsu_stop_req);
	/* coverity_fix_63258_stop */
	return X2AP_SUCCESS;
	X2AP_UT_TRACE_EXIT();
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_build_and_send_stop_msg 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP  global context
 *                  p_x2ap_rsu_ctx - pointer to rsu measurement context
 *                  p_enb_ctx - pointer to peer enodeb context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function builds and sends ResourceStatusRequest(STOP)
 *                message to X2AP module.
 ********************************************************************************/
x2ap_return_et 
x2ap_rsu_build_and_send_stop_msg(x2ap_gb_context_t *p_x2ap_gb_ctx,
	x2ap_rsu_context_t* p_x2ap_rsu_ctx,
	x2ap_peer_enb_context_t *p_enb_ctx
	)
{
    x2ap_return_et result = X2AP_FAILURE;
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16    asn_msg_len = 0;
    OSCTXT asn1_ctx;
    rrc_x2ap_rsu_start_req_t msg;

    x2ap_peer_enb_msg_t  peer_enb_msg;

    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));


    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(PNULL != p_x2ap_rsu_ctx);
    X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);
    X2AP_ASSERT(PNULL != p_rrm_rsu_stop_req);
    X2AP_ASSERT(PNULL != p_enb_ctx);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
	X2AP_TRACE(X2AP_ERROR,
		"[x2ap_ResourceStatusRequest] ASN context initialization failed");
	X2AP_UT_TRACE_EXIT();
	return X2AP_FAILURE;
    }
    /* Reset message */
    X2AP_MEMSET(&msg, 0, sizeof(msg));
    X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));

    /*Build the message */
    msg.eNB1_measurement_Id = p_x2ap_rsu_ctx->enb1_meas_id;
    msg.eNB2_measurement_Id = p_x2ap_rsu_ctx->enb2_meas_id;
    msg.registration_reqd = X2AP_STOP; 

    /* Encode message */
    result = x2ap_rsu_encode_stop_req(&asn1_ctx,
	    &msg,
	    asn_msg_buff,
	    &asn_msg_len);
    if (X2AP_SUCCESS == result)
    {
	peer_enb_msg.p_msg = asn_msg_buff;
	peer_enb_msg.msg_len = asn_msg_len;
    /* SPR_14436_Fix Start */
	peer_enb_msg.stream_id = p_enb_ctx->stream_info.
		stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
	/* SPR_14436_Fix Stop */
	if (INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd)
	{
	    /*Sending the  X2 RSU START REQUEST to Peer eNB via SCTP */
	    result = x2ap_sctp_sm_entry
		(X2AP_SCTP_SEND_DATA_EV,(U8*)&peer_enb_msg,p_x2ap_gb_ctx,p_enb_ctx);
	}    
    }     
    else
    {
	X2AP_TRACE(X2AP_ERROR,
		"[x2ap_ResourceStatusRequest(STOP)] Encode failure");
	result = X2AP_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();

    return result;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_encode_stop_req 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_rsu_start_req - pointer to rrc_x2ap_rsu_start_req_t
 *                  p_buff - pointer to result ASN.1 encoded message
 *                  p_buff_size - size of result buffer
 * Outputs        : p_buff (ASN.1 encoded message),
 *               p_buff_size (size of result buffer)
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function used to encode rsu status stop request.
 ********************************************************************************/
x2ap_return_et x2ap_rsu_encode_stop_req 
(
 OSCTXT                     *p_asn1_ctx,
 rrc_x2ap_rsu_start_req_t  *p_rsu_start_req,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size   /* size of result buffer */
 )
{

    x2ap_return_et           result = X2AP_FAILURE;
    x2ap_X2AP_PDU               x2ap_pdu;
    x2ap_ResourceStatusRequest  *p_x2ap_msg = X2AP_P_NULL;
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_rsu_start_req);

    do
    {
	/* Init X2AP PDU */
	asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
	/* Set Pdu type to Initiating message */
	x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

	x2ap_pdu.u.initiatingMessage =
	    rtxMemAllocType(p_asn1_ctx, x2ap_InitiatingMessage);
	if (PNULL == x2ap_pdu.u.initiatingMessage)
	{
	    X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
	    break;
	}

	asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

	x2ap_pdu.u.initiatingMessage->procedureCode =
	    ASN1V_x2ap_id_resourceStatusReportingInitiation; /*Confirm*/

	x2ap_pdu.u.initiatingMessage->criticality = x2ap_reject;

	/* Set the initiating message type to resource status reporting initiation */
	x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__resourceStatusReportingInitiation;

	x2ap_pdu.u.initiatingMessage->value.u.resourceStatusReportingInitiation = 
	    rtxMemAllocType(p_asn1_ctx,x2ap_ResourceStatusRequest);
	if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.resourceStatusReportingInitiation)
	{
	    X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
	    break;
	}
	p_x2ap_msg = x2ap_pdu.u.initiatingMessage->value.u.resourceStatusReportingInitiation;

	asn1Init_x2ap_ResourceStatusRequest(p_x2ap_msg);

	/*Encode IE 1*/
	if (RT_OK !=
		asn1Append_x2ap_ResourceStatusRequest_protocolIEs_1(p_asn1_ctx,&p_x2ap_msg->protocolIEs,p_rsu_start_req->eNB1_measurement_Id))
	{
	    break;
	}
	/*Encode IE 2*/
	if (RT_OK !=
		asn1Append_x2ap_ResourceStatusRequest_protocolIEs_2(p_asn1_ctx,&p_x2ap_msg->protocolIEs,p_rsu_start_req->eNB2_measurement_Id))
	{
	    break;
	}
	/*Encode IE 3*/
	if (RT_OK !=
		asn1Append_x2ap_ResourceStatusRequest_protocolIEs_3(p_asn1_ctx,&p_x2ap_msg->protocolIEs,p_rsu_start_req->registration_reqd))
	{
	    break;
	}

	/* ASN Encode message */
	pu_setBuffer(p_asn1_ctx, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

	if (RT_OK != asn1PE_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu))
	{
	    X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
	    break;
	} 

	*p_buff_size = (U16)pe_GetMsgLen(p_asn1_ctx);

	
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU : Resource Status"
                "Request(STOP) Message sent on X2AP Interface ", &x2ap_pdu);

	result=X2AP_SUCCESS;
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_build_and_send_rrm_stop_req 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2ap global context
 *                  meas_ID - measurement id
 * Outputs        : None
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function used to build and send the resource status
 *                 request(STOP) to RRM module
 ********************************************************************************/

x2ap_return_et x2ap_rsu_build_and_send_rrm_stop_req(x2ap_gb_context_t  *p_x2ap_gb_ctx,      /* X2ap global context*/
	U32 meas_ID) 
{
    /* SPR 22310 : CID 115965 Fix Start */
    x2ap_timer_t timer_id = X2AP_NULL;
    /* SPR 22310 : CID 115965 Fix End */
    rsu_enb_meas_id_search_node_t *p_rsu_tree_node = X2AP_P_NULL;
    x2ap_rsu_context_t      *p_x2ap_rsu_ctx = X2AP_P_NULL;
    U8 counter = X2AP_NULL;
    x2ap_return_et retVal = X2AP_FAILURE;
    x2ap_rrm_rsu_enb_stop_req_t *p_x2ap_rrm_rsu_enb_stop_req = X2AP_P_NULL;
    x2ap_peer_enb_context_t *p_enb_ctx = X2AP_P_NULL;


    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    p_x2ap_rrm_rsu_enb_stop_req = (x2ap_rrm_rsu_enb_stop_req_t *)x2ap_mem_get(sizeof(x2ap_rrm_rsu_enb_stop_req_t));
    if(X2AP_P_NULL == p_x2ap_rrm_rsu_enb_stop_req)
    {
	X2AP_TRACE(X2AP_FATAL,"\nMemeory allocation failed ");
	return X2AP_FAILURE;
    }

    X2AP_MEMSET(p_x2ap_rrm_rsu_enb_stop_req,0,sizeof(x2ap_rrm_rsu_enb_stop_req_t));
    /* Fetch the measeurement context from the tree*/
    p_rsu_tree_node = (rsu_enb_meas_id_search_node_t*)x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),
	    &(meas_ID));
    if(X2AP_P_NULL != p_rsu_tree_node)
    {
	/* get the measurement context */
	p_x2ap_rsu_ctx = &p_rsu_tree_node->x2ap_rsu_ctx;
	for(counter = 0 ; counter < MAX_PEER_ENB ; counter++)
	{
	    /* Bug:9129 */
	    if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].peer_enodeb_id == p_x2ap_rsu_ctx->peer_enb_id)
		    && (p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].op_status == OPERATIONAL))
		/* Bug:9129 */
	    {
		p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
		break;
	    }
	}
	if(X2AP_P_NULL == p_enb_ctx)
	{
	    X2AP_TRACE(X2AP_ERROR, "[%s]:Peer Enb Context is NULL while fetching from x2ap global context\n", __FUNCTION__);
	    /* coverity_fix_63255_start */
	    x2ap_mem_free(p_x2ap_rrm_rsu_enb_stop_req);
	    /* coverity_fix_63255_stop */
	    return X2AP_FAILURE;
	}
    }
    else
    {
	X2AP_TRACE(X2AP_ERROR, "[%s]:No measurement context exist for this meas ID\n", __FUNCTION__);
	/* coverity_fix_63255_start */
	x2ap_mem_free(p_x2ap_rrm_rsu_enb_stop_req);
	/* coverity_fix_63255_stop */
	return X2AP_FAILURE;
    }

    /* Fill the values of the Structure */
    p_x2ap_rrm_rsu_enb_stop_req->enb_meas_id = meas_ID;
    X2AP_MEMCPY(&p_x2ap_rrm_rsu_enb_stop_req->peer_gb_enb_id,&p_enb_ctx->gb_enb_id,sizeof(x2_gb_enb_id_t));

    retVal = rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_stop_req(
	    (p_x2ap_rrm_rsu_enb_stop_req),
	    RRC_X2AP_MODULE_ID, 
	    RRC_RRM_MODULE_ID, 
	    p_x2ap_rsu_ctx->rrm_transaction_id,
	    RRC_NULL);
    if (X2AP_FAILURE == retVal)
    {
	X2AP_TRACE(X2AP_ERROR," X2AP : RESOURCE STATUS STOP REQUEST not sent to RRM(2)");
    }

    /*Start the timer*/
    if (X2AP_SUCCESS == x2ap_rsu_start_timer(X2AP_RSU_RRM_GUARD_TIMER,
		p_enb_ctx,
		p_x2ap_gb_ctx->config_param.timer_conf_info.
		x2ap_rsu_rrm_guard_tim_val,
		&timer_id,
		meas_ID))
    {
	X2AP_TRACE(X2AP_INFO,
		"X2AP_RSU_RRM_GUARD_TIMER Started Successfully");
    }

    /*store the timer id in measurement context*/
    p_rsu_tree_node->x2ap_rsu_ctx.timer_id = timer_id;
    p_rsu_tree_node->x2ap_rsu_ctx.timer_type = X2AP_RSU_RRM_GUARD_TIMER;

    /* Change the state to X2AP_RSU_STATE_STOP_ONGOING */
    p_rsu_tree_node->x2ap_rsu_ctx.state =X2AP_RSU_STATE_STOP_ONGOING;
    /*Insert the node in the tree*/
    /*SPR 5519 Start */
    /* x2ap_st_insert_node(&p_x2ap_gb_ctx->x2ap_rsu_context_tree,&p_rsu_tree_node->nodeAnchor);*/
    /*SPR 5519 Stop */

    /* coverity_fix_63255_start */
    x2ap_mem_free(p_x2ap_rrm_rsu_enb_stop_req);
    /* coverity_fix_63255_stop */
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_rsu_event_x2_conn_down_evt_hdl 
 * Inputs         : p_x2ap_gb_ctx - pointer to  x2ap global context
 *                  p_enb_ctx - pointer to peer enb context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : This function deletes all the measurements going with the RRM 
 ********************************************************************************/
    x2ap_return_et 
x2ap_rsu_event_x2_conn_down_evt_hdl(x2ap_gb_context_t  *p_x2ap_gb_ctx ,
	x2ap_peer_enb_context_t *p_enb_ctx)

{
    /* Fetch all the measurement context for the peer eNB */
    rsu_enb_meas_id_search_node_t *p_temp = PNULL;    
    rsu_enb_meas_id_search_node_t *p_next = PNULL; 

    p_temp = (rsu_enb_meas_id_search_node_t*)x2ap_st_get_first_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree));
    p_next = p_temp;

    while(X2AP_P_NULL != p_temp)
    {
	p_next = (rsu_enb_meas_id_search_node_t*)x2ap_st_get_next_node((x2ap_search_tree_node_t*)p_temp); 
	if (p_temp->x2ap_rsu_ctx.peer_enb_id == p_enb_ctx->peer_enodeb_id)
	{
	    if((p_temp->x2ap_rsu_ctx.state == X2AP_RSU_STATE_START_ONGOING) || (p_temp->x2ap_rsu_ctx.state == X2AP_RSU_STATE_STOP_ONGOING))
	    {
		/* Stop the timer whichever is running */
		X2AP_TRACE(X2AP_INFO,"Stoping the timer : %p\n", p_temp->x2ap_rsu_ctx.timer_id);
		x2ap_stop_timer(p_temp->x2ap_rsu_ctx.timer_id);
		p_temp->x2ap_rsu_ctx.timer_id = X2AP_P_NULL;

		/* Delete the measurement context */
		X2AP_TRACE(X2AP_INFO,"X2 Setup is down --- Context is Deleted for Measurement ID : %d\n", p_temp->x2ap_rsu_ctx.enb1_meas_id);
		x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree), &p_temp->nodeAnchor);
		x2ap_mem_free(p_temp);
		p_temp = X2AP_P_NULL;
	    }
	    else if ((p_temp->x2ap_rsu_ctx.state == X2AP_RSU_STATE_SEND_RSU )|| ( p_temp->x2ap_rsu_ctx.state == X2AP_RSU_STATE_LISTEN_RSU))
	    {
		/* Delete the measurement context */
		X2AP_TRACE(X2AP_INFO,"X2 Setup is down --- Context is Deleted for Measurement ID : %d\n", p_temp->x2ap_rsu_ctx.enb1_meas_id);
		x2ap_st_delete_node(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree), &p_temp->nodeAnchor);
		x2ap_mem_free(p_temp);
		p_temp = X2AP_P_NULL;
	    }
	} 
	p_temp = p_next; 
    }
    return X2AP_SUCCESS; 
}

/*****************************************************************************
 * Function Name  : x2ap_rsu_build_and_send_failure_msg 
 * Inputs         : enb1_meas_id - eNB measurement id
 *                  enb2_meas_id - eNB measurement id
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_ctx - pointer to peer eNB context
 *                  p_criticality_diagnostics - pointer to criticality diagnostics
 *                  cause - failure cause
 *                  p_x2ap_rsu_ctx - pointer to x2ap_rsu_context_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function builds and sends ResourceStatusFailure message
 *       to X2AP module.
 ********************************************************************************/

x2ap_return_et x2ap_rsu_build_and_send_failure_msg(
	U32                enb1_meas_id,
	U32                enb2_meas_id,    
	x2ap_gb_context_t  *p_x2ap_gb_ctx,
	x2ap_peer_enb_context_t *p_enb_ctx,
	x2ap_CriticalityDiagnostics *p_criticality_diagnostics,
	U8 cause,
	x2ap_rsu_context_t *p_x2ap_rsu_ctx
	)
{
    x2ap_return_et result = X2AP_FAILURE;
    OSCTXT asn1_ctx;
    U8     asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16    asn_msg_len = 0;

    rrc_x2ap_rsu_failure_t msg;

    x2ap_peer_enb_msg_t  peer_enb_msg;
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(PNULL != p_x2ap_rsu_start_req);
    X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
	X2AP_TRACE(X2AP_ERROR,
		"[x2ap_ResourceStatusFailure] "
		"ASN context initialization failed");
	X2AP_UT_TRACE_EXIT();
	return X2AP_FAILURE;
    }
    /* Reset message */
    X2AP_MEMSET(&msg, 0, sizeof(msg));
    X2AP_MEMSET(asn_msg_buff,0,sizeof(X2AP_MAX_ASN1_BUF_LEN));

    /* Fill message */
    /*Fill Old eNB1_measurement_Id received in RSU start request*/
    msg.eNB1_measurement_Id = enb1_meas_id;

    /*Fetch new Measurement Id for peer enb and fill the message*/ 
    msg.eNB2_measurement_Id = enb2_meas_id;

    /* Fill the cause */

    if (PNULL != p_criticality_diagnostics)
    {
	X2AP_TRACE(X2AP_INFO,
		"[x2ap_ResourceStatusFailure] Filling Cause and criticality_diagnostics");
	msg.cause.t = T_x2ap_Cause_protocol;
	msg.cause.u.protocol = cause;

	msg.criticality_diagnostics = *p_criticality_diagnostics;
	msg.bitmask |= X2AP_RSU_FAILURE_CRIT_DIAGNO_PRESENT;
    }
    else
    {
	msg.cause.t = T_x2ap_Cause_radioNetwork;
	msg.cause.u.radioNetwork = cause;
    }
    if(p_x2ap_rsu_ctx != PNULL && p_x2ap_rsu_ctx->partial_success_indicator_present && p_x2ap_rsu_ctx->state == X2AP_RSU_STATE_START_ONGOING && p_x2ap_rsu_ctx->rrm_rsu_measurement_res_list_present)
    {
	msg.bitmask |=
	    X2AP_RSU_COMP_FAILURE_INF_LIST;
	if(X2AP_FAILURE == x2ap_rsu_complete_failure_cause_measurement_list(&msg,&p_x2ap_rsu_ctx->x2ap_rsu_measurement_res_list,&asn1_ctx))
	{    
	    X2AP_TRACE(X2AP_ERROR,"[x2ap_measurement_init_result] Build failure");
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
	    return X2AP_FAILURE;
	}
    }

    /* Encode message */
    result = x2ap_rsu_internal_encode_failure_msg(&asn1_ctx,
	    asn_msg_buff,
	    &asn_msg_len,
	    &msg); 
    if (X2AP_SUCCESS == result)
    {
	peer_enb_msg.p_msg = asn_msg_buff;
	peer_enb_msg.msg_len = asn_msg_len;
    /* SPR_14436_Fix Start */
	 peer_enb_msg.stream_id = p_enb_ctx->stream_info.
		stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
	 /* SPR_14436_Fix Stop */

	if (INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd)
	{
	    /*Sending the X2AP RESOURCE STATUS FAILURE to Peer eNB via SCTP */
	    result = x2ap_sctp_sm_entry
		(X2AP_SCTP_SEND_DATA_EV,(U8*)&peer_enb_msg,p_x2ap_gb_ctx,p_enb_ctx);
	}    
    }    
    else
    {
	X2AP_TRACE(X2AP_ERROR,
		"[x2ap_ResourceStatusFailure] Encode failure");
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();

    return result;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_send_rrm_start_resp
 * Inputs         : transaction_id - transaction id
 *                  response - response
 *                  meas_id - measurement id
 *                  cause - cause
 *                  p_peer_enb_ctx - pointer to peer eNB context
 *                  x2ap_rsu_measurement_res_list - 
 *                                  pointer to x2ap_rrm_rsu_measurement_res_t
 *                  partial_success_present - partial success present flag
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : Send X2AP_RRM_RSU_START_RES_ENB_RRM from X2AP to RRM  
 ********************************************************************************/
x2ap_return_et x2ap_rsu_send_rrm_start_resp
(
 U32                                transaction_id,
 x2ap_return_et                     response,
 U32                                meas_id,
 U8                                 cause,
 x2ap_peer_enb_context_t            *p_peer_enb_ctx, /*x2ap_rsu_gb_enb_id_fix*/
 x2ap_rrm_rsu_measurement_res_t     *x2ap_rsu_measurement_res_list,
 U8                                 partial_success_present
 )
{
    x2ap_rrm_rsu_enb_start_res_t   rrm_rsu_enb_start_resp;
    x2ap_return_et  result = X2AP_FAILURE;

    X2AP_MEMSET(&rrm_rsu_enb_start_resp, 0, sizeof(x2ap_rrm_rsu_enb_start_res_t));
    rrm_rsu_enb_start_resp.response = response;
    /*x2ap_rsu_gb_enb_id_fix Start*/
    if(X2AP_NULL != p_peer_enb_ctx)
    {
	X2AP_MEMCPY(&rrm_rsu_enb_start_resp.global_enb_id, &p_peer_enb_ctx->gb_enb_id, sizeof(x2_gb_enb_id_t));
	rrm_rsu_enb_start_resp.bitmask |= ENB_ID_IS_PRESENT;
    }
    /*x2ap_rsu_gb_enb_id_fix Stop*/

    if((response == X2AP_SUCCESS) && (meas_id != 0))
    {
	/*Store the meas id in the response*/
	rrm_rsu_enb_start_resp.bitmask |= ENB_MEAS_ID_IS_PRESENT;
	rrm_rsu_enb_start_resp.enb_meas_id = meas_id;
	if(partial_success_present == 1 && x2ap_rsu_measurement_res_list !=NULL)
	{
	    rrm_rsu_enb_start_resp.bitmask |= ENB_START_PARTIAL_SUCCESS;
	    X2AP_MEMCPY(&rrm_rsu_enb_start_resp.rrm_rsu_measurement_res_list,(const void *)x2ap_rsu_measurement_res_list,sizeof(rrm_rsu_enb_start_resp.rrm_rsu_measurement_res_list));
	}
    }
    else
    {
	rrm_rsu_enb_start_resp.bitmask |= ENB_START_RES_RRM_CAUSE_IS_PRESENT;
	rrm_rsu_enb_start_resp.rrm_cause.type = T_x2ap_Cause_radioNetwork; 
	rrm_rsu_enb_start_resp.rrm_cause.value = (U16)cause; 
	if(partial_success_present == 1 && x2ap_rsu_measurement_res_list !=NULL)
	{
	    rrm_rsu_enb_start_resp.bitmask |= ENB_START_PARTIAL_SUCCESS;
	    X2AP_MEMCPY(&rrm_rsu_enb_start_resp.rrm_rsu_measurement_res_list,(const void *)x2ap_rsu_measurement_res_list,sizeof(rrm_rsu_enb_start_resp.rrm_rsu_measurement_res_list));
	}
    }

    /* Send message to RRM */
    result = rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_start_res(&rrm_rsu_enb_start_resp,
	    RRC_X2AP_MODULE_ID, RRC_RRM_MODULE_ID, transaction_id, RRC_NULL);

    return result;
}
/*****************************************************************************
 * Function Name  : x2ap_rsu_build_and_send_rrm_stop_resp 
 * Inputs         : transaction_id - transaction id
 *                  response - response
 *                  meas_id - measurement id
 *                  gb_enb_id - type of x2_gb_enb_id_t
 *                  cause - cause
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : Send X2AP_RRM_RSU_STOP_RES_ENB_RRM from X2AP to RRM
 ********************************************************************************/
x2ap_return_et x2ap_rsu_build_and_send_rrm_stop_resp
(
 U32                                transaction_id,
 x2ap_return_et                     response,
 U32                                meas_id,
 x2_gb_enb_id_t                     gb_enb_id,
 U8                                 cause
 )
{
    x2ap_rrm_rsu_enb_stop_res_t   rrm_rsu_enb_stop_resp;
    x2ap_return_et  res = X2AP_FAILURE;

    X2AP_MEMSET(&rrm_rsu_enb_stop_resp, 0,sizeof(x2ap_rrm_rsu_enb_stop_res_t));

    rrm_rsu_enb_stop_resp.response = response;
    rrm_rsu_enb_stop_resp.enb_meas_id = meas_id;
    rrm_rsu_enb_stop_resp.peer_gb_enb_id = gb_enb_id;
    //X2AP_MEMCPY(&rrm_rsu_enb_stop_resp.peer_gb_enb_id, &gb_enb_id, sizeof(x2_gb_enb_id_t));

    if(response == X2AP_FAILURE)
    {
	rrm_rsu_enb_stop_resp.bitmask |= ENB_STOP_RES_RRM_CAUSE_IS_PRESENT;
	rrm_rsu_enb_stop_resp.rrm_cause.type = T_x2ap_Cause_radioNetwork;
	rrm_rsu_enb_stop_resp.rrm_cause.value = (U16)cause;
    }

    /* Send message */
    res = rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_stop_res(&rrm_rsu_enb_stop_resp,
	    RRC_X2AP_MODULE_ID, RRC_RRM_MODULE_ID, transaction_id, RRC_NULL);

    return res;
}
