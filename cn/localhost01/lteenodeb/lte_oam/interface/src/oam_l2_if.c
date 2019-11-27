
/****************************************************************************
 * File Name : oam_l2_if.c
 *
 * Description : This file contains functions to handle messages to
 *              and from L2 to OAM.
 *
 * Revision Histroy:
 *
 * Date            Author            References            Comments
 *----------     -------------     ------------          -------------
 * May,2012        Shuchi Priya      OAM Design Document   Initial Version
 * September,2012  Vivek K Gupta     OAM Rel 1.0           Changes in "OAM_SET_PARAMS_RESP"
 *                                                        due to api header redesign.
 * Sept,2013      Raveen Sharma     CSR 51606 Fix         Code Changes for CSR Fix
 * Sept,2013     Vinod Vyas       OAM Design Doc         Code change for Ipv6 support
 * May 2014      Shalu Sagar       Klockwork Fix
 * July,2014     Aditi Singh        SPR 9620 Fix        Guard timer Added
 * Oct 2014      Swetank Srivastava SPR 13617
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "oam_l2_if.h"
#include <oam_config_tcb.h>
#include <oam_sim.h>
#include <oam_sim_utils.h>

/* CSR 57877 FIx Start */
#include <oam_mgmt_intf_api.h>
/* CSR 57877 FIx End*/

/* Global Variable */
UInt16 kpiToReportBitMap;
LOCAL UInt8 kpiPdcpConBitMap;

LOCAL oam_get_log_level_l2_all_t oam_get_log_level_l2_all;
/*SPR 19771 FIXED START*/
/*Code removed*/
/*SPR 19771 FIXED END*/
extern UInt8 SET_L2_ALL_FLAG;
/*SPR 17842 START*/
extern UInt16 global_trans_bitmap_l2_log;
extern UInt16 global_trans_l2_log_flag;
/*SPR 17842 END */
extern SInt32 Config_Flag;
extern oam_prov_req_t oam_prov_req;
/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Function Name  : oam_send_mac_modify_layer_req
 * Description    : This function sends modify request to MAC
 * Inputs         : p_mac_modify_req :Pointer to MAC modify layer request
 *                : src_module_id    :Source module identifier
 *                : transaction_id   :transaction identifier
 * Outputs        : p_error_code     :Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_send_mac_modify_layer_req
(
 oam_mac_modify_req_params_t *p_mac_modify_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg          = OAM_NULL;
	UInt8 *p_mac_msg      = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_length = sizeof(oam_mac_modify_req_params_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	/*BUG_ID-125*/
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_MODIFY_LAYER_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_MODIFY_LAYER_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Incrementing the pointer to point to the structure
	   oam_mac_init_req_params_t */
	p_mac_msg = p_mac_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_memcpy((void *)p_mac_msg,p_mac_modify_req,sizeof(oam_mac_modify_req_params_t));

	/* Send message to MAC*/
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_MODIFY_LAYER_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_handle_mac_modify_layer_cnf
 * Description    : This function handles modify cnf received from MAC
 * Inputs         : p_api_buf :pointer to MAC modify layer config
 * Outputs        : error_code:Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_handle_mac_modify_layer_cnf
(
 void* p_api_buf

 )
{
	void                   *p_data            =  OAM_NULL;
	oam_mac_modify_resp_t  *p_mac_modify_resp =  OAM_NULL;
	UInt16 trans_id = OAM_ZERO;
	UInt16 dest_id = OAM_ZERO;
	oam_error_code_et p_error_code;
	UInt16 trans_id_resp=OAM_ZERO;
	trans_id = get_word_from_header(p_api_buf);
	OAM_FUN_ENTRY_TRACE();
	/* SPR 9620 CHANGE START */
#ifndef OAM_UT_FRAMEWORK
	if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(trans_id, &trans_id_resp, &dest_id))
	{
		OAM_LOG(OAM,OAM_ERROR, "oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id function failed");
		return OAM_FAILURE;
	}
#endif
	/* SPR 9620 CHANGE END */

	p_data            = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_mac_modify_resp = (oam_mac_modify_resp_t *)p_data;


	/*check the response received from MAC */
	if (OAM_SUCCESS == p_mac_modify_resp->responseCode)
	{
		OAM_LOG(OAM,OAM_INFO,"MAC_MODIFY_LAYER_CNF[SUCCESS] received"
				" from MAC");

		oam_construct_n_send_set_resp_to_usr(OAM_RESULT_SUCCESS,
				OAM_ZERO,OAM_NULL,OAM_ZERO,
				OAM_VERSION_ID, OAM_MODULE_ID,
				dest_id, trans_id, OAM_ZERO, OAM_ZERO,
				&p_error_code);
	}
	else
	{
		OAM_LOG(OAM,OAM_INFO,"MAC_MODIFY_LAYER_CNF[FAILURE] with cause[%d]",
				p_mac_modify_resp->responseCode);

		oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
				OAM_ZERO,OAM_NULL,OAM_ZERO,
				OAM_VERSION_ID, OAM_MODULE_ID,
				dest_id, trans_id, OAM_ZERO, OAM_ZERO,
				&p_error_code);
		return OAM_FAILURE;   
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_handle_mac_init_layer_cnf
 * Description    : This function handles mac init layer cnf received from MAC
 * Inputs         : p_api_buf :pointer to MAC init layer config
 * Outputs        : p_mac_init_resp
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
	oam_return_et
oam_handle_mac_init_layer_cnf(oam_mac_init_resp_params_t *p_mac_init_resp,
		void* p_api_buf)
{
	OAM_FUN_ENTRY_TRACE();


	OAM_NULL_CHECK(OAM_NULL != p_api_buf); 

	p_api_buf          = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	oam_memcpy(p_mac_init_resp, p_api_buf,
			sizeof(oam_mac_init_resp_params_t));

	/*check the response from MAC*/
	if (OAM_MAC_SUCCESS == p_mac_init_resp->responseCode) {
		OAM_LOG(OAM, OAM_INFO,
				"Received MAC_INIT_LAYER_CNF with Status as SUCCESS");
	}
	else {            /*BUG_ID:90*/
		OAM_LOG(OAM, OAM_WARNING, "Received MAC_INIT_LAYER_CNF Failure with"
				" Response Code[%d]", p_mac_init_resp->responseCode);
		/* CA TDD Changes Start */
		/* Code Removed */
		if(MAC_INIT_LAYER_REQ_MULTI_CELL_WITHOUT_DL_UL_SPLIT
				== p_mac_init_resp->responseCode) {
			/* CA TDD Changes  End */
			OAM_LOG(OAM, OAM_WARNING, "MAC Layer initialization failed as "
					"multi cell is configured without DL UL split");
		}
		else if(MAC_INIT_LAYER_REQ_CA_INVALID_SCH_STRATEGY
				== p_mac_init_resp->responseCode) {
			OAM_LOG(OAM, OAM_WARNING, "MAC Layer initialization failed as CA "
					"is configured with strategy set to PFS or RR");
		}
		else if(MAC_INIT_LAYER_REQ_INVALID_NUM_OF_CELLS ==
				p_mac_init_resp->responseCode) {
			OAM_LOG(OAM, OAM_WARNING, "MAC Layer initialization failed as "
					"Number of cells configured is greater than 2");
		}
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_handle_mac_init_cell_cnf
 * Description    : This function handles mac init cell cnf received from MAC
 * Inputs         : p_api_buf :pointer to MAC init layer config
 * Outputs        : mac_init_cell_resp 
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_handle_mac_init_cell_cnf
(
 oam_mac_init_resp_params_t  *p_mac_init_cell_resp,
 void* p_api_buf
 )
{
	oam_return_et retVal = OAM_FAILURE;


	OAM_FUN_ENTRY_TRACE();

	OAM_NULL_CHECK(OAM_NULL != p_api_buf);

	p_api_buf = (UInt8*)p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	oam_memcpy(p_mac_init_cell_resp, p_api_buf,
			sizeof(oam_mac_init_resp_params_t));

	if(OAM_MAC_SUCCESS ==
			(oam_mac_return_et)p_mac_init_cell_resp->responseCode) {
		OAM_LOG(OAM, OAM_INFO,
				"Received MAC_INIT_CELL_CNF with Status as SUCCESS");
		retVal = OAM_SUCCESS;
	}
	else if(OAM_MAC_FAILURE ==
			(oam_mac_return_et)p_mac_init_cell_resp->responseCode) {
		OAM_LOG(OAM, OAM_WARNING, "MAC_INIT_CELL_CNF Failure received from MAC");
	}
	else {
		OAM_LOG(OAM, OAM_WARNING,
				"Received MAC_INIT_CELL_CNF Failure with Response Code[%s]",
				oam_mac_resp_code_to_str(p_mac_init_cell_resp->responseCode));
		retVal = OAM_SUCCESS;

	}

	OAM_FUN_EXIT_TRACE();
	return retVal;
}


/****************************************************************************
 * Function Name  : oam_handle_pdcp_init_layer_cnf
 * Description    : Handles pdcp init layer cnf received from PDCP
 * Inputs         : p_api_buf :pointer to PDCP init layer confirmation
 * Outputs        : none  
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_handle_pdcp_init_layer_cnf
(
 void *p_api_buf

 )
{
	oam_pdcp_cnf_t *p_pdcp_cnf;
	p_pdcp_cnf = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	OAM_NULL_CHECK(OAM_NULL != p_api_buf);

	p_api_buf     = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_pdcp_cnf = (oam_pdcp_cnf_t *)p_api_buf;

	/* check the response code received from PDCP*/
	if (OAM_SUCCESS != p_pdcp_cnf->responseCode)
	{
		OAM_LOG(OAM,OAM_WARNING, "Received PDCP_INIT_LAYER_CNF Failure with"
				" Response Code[%d]",p_pdcp_cnf->responseCode);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	else
	{
		/*BUG_ID-114*/
		OAM_LOG(OAM,OAM_INFO,"Received PDCP_INIT_LAYER_CNF with status as SUCCESS");
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_handle_mac_get_kpi_cnf
 * Description    : This function handles KPI cnf received from MAC
 * Inputs         : p_api_buf :pointer to MAC get kpi confirmation
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_handle_mac_get_kpi_cnf
(
 void* p_api_buf
 )
{
	oam_mac_Kpi_resp_t *p_mac_kpi_resp;
	p_mac_kpi_resp = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	p_api_buf      = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_mac_kpi_resp =(oam_mac_Kpi_resp_t *)p_api_buf;

	/*check the response code received from MAC*/
	if (OAM_SUCCESS == p_mac_kpi_resp->responseCode)
	{
		OAM_LOG(OAM,OAM_INFO,"Received MAC_GET_KPI_CNF with status as SUCCESS. "
                "Number of Active UE : %d , Number of RACH Ind  : %d", 
				p_mac_kpi_resp->numOfActiveUE, p_mac_kpi_resp->numOfRachInd);
	}
	else
	{
		OAM_LOG(OAM,OAM_INFO,"MAC_GET_KPI_CNF [FAILURE] with cause[%d]",
				p_mac_kpi_resp->responseCode);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_send_mac_cleanup_req
 * Description    : This function sends cleanup request to MAC
 * Inputs         : src_module_id  :Source module identifier
 : transaction_id :transaction identifier
 * Outputs        : p_error_code   :Pointet to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_send_mac_cleanup_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 /*SPR 21369 Start*/
 UInt8 cell_indx,
 /*SPR 21369 End*/
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	p_msg = OAM_NULL;
	UInt8 *p_mac_msg;
	p_mac_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
		p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_CLEANUP_LAYER_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_CLEANUP_LAYER_REQ,
			OAM_ZERO,
			/*SPR 21369 Start*/
            cell_indx);
			/*SPR 21369 End*/

	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_CLEANUP_LAYER_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_send_mac_get_stats_req
 * Description    : This function sends stats request to MAC
 * Inputs         : src_module_id  :Source module identifier
 : transaction_id :transaction identifier
 * Outputs        : p_error_code   :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_send_mac_get_stats_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	p_msg = OAM_NULL;
	UInt8 *p_mac_msg;
	p_mac_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_GET_STATS_REQ , msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_GET_STATS_REQ,
			OAM_ZERO,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_GET_STATS_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_send_mac_get_kpi_req
 * Description    : This function sends KPI request to MAC
 * Inputs         : src_module_id  :Source module identifier
 *                : transaction_id :transaction identifier
 * Outputs        : p_error_code   :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_send_mac_get_kpi_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	p_msg     = OAM_NULL;
	UInt8 *p_mac_msg;
	p_mac_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_GET_KPI_REQ , msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_GET_KPI_REQ,
			OAM_ZERO,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_GET_KPI_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_send_pdcp_cleanup_req
 * Description    : This function sends cleanup request to PDCP
 * Inputs         : src_module_id  :Source module identifier
 *                : transaction_id :transaction identifier
 * Outputs        : p_error_code   :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_send_pdcp_cleanup_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 /*SPR 21369 Start*/
 UInt8 cell_indx,
 /*SPR 21369 End*/
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	p_msg    = OAM_NULL;
	UInt8 *p_pdcp_msg;
	p_pdcp_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_pdcp_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_CLEANUP_LAYER_REQ , msg_api_length);

	/* Fill interface header */
	p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_pdcp_msg,
			transaction_id,
			src_module_id,
			PDCP_MODULE_ID,
			PDCP_CLEANUP_LAYER_REQ,
			OAM_ZERO,
			/*SPR 21369 Start*/
            cell_indx);
			/*SPR 21369 End*/

	/* Send message to PDCP */
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_CLEANUP_LAYER_REQ message to PDCP with error code %d",
				*p_error_code);

		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_send_get_pdcp_stats_req
 * Description    : This function sends stats request to PDCP
 * Inputs         : src_module_id  :Source module identifier
 *                : transaction_id :transaction identifier
 * Outputs        : p_error_code   :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_send_get_pdcp_stats_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	p_msg    = OAM_NULL;
	UInt8 *p_mac_msg;
	p_mac_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_GET_STATS_REQ , msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			PDCP_MODULE_ID,
			PDCP_GET_STATS_REQ,
			OAM_ZERO,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to PDCP */
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_GET_STATS_REQ message to PDCP with error code %d",
				*p_error_code);

		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_send_mac_init_layer_req
 * Description    : This function sends init request to MAC
 * Inputs         : p_mac_init_req :Pointer to MAC init layer request
 *                : src_module_id  :Source module identifier
 *                : transaction_id :transaction identifier
 * Outputs        : p_error_code   :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_send_mac_init_layer_req
(
 oam_mac_init_req_params_t *p_mac_init_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 UInt8 cell_index,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	p_msg    = OAM_NULL;
	UInt8 *p_mac_msg;
	p_mac_msg = OAM_NULL;

    /* SPR 23209 FIXED START*/
    UInt8 count = OAM_ZERO;
    /* SPR 23209 FIXED END */

	OAM_FUN_ENTRY_TRACE();

	OAM_NULL_CHECK(OAM_NULL != p_mac_init_req); 

	msg_length     = sizeof(oam_mac_init_req_params_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;


	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_INIT_LAYER_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;

	oam_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
			MAC_MODULE_ID, MAC_INIT_LAYER_REQ, msg_length, cell_index);

	/* Incrementing the pointer to point to the structure
	   oam_mac_init_req_params_t */
	p_mac_msg = p_mac_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_memcpy((void *)p_mac_msg, p_mac_init_req, sizeof(oam_mac_init_req_params_t));

    /* SPR 23209 FIXED START*/
    OAM_LOG(OAM,OAM_DETAILED,"\n MOCN PARAMS for MAC INIT REQ: \n");
    OAM_LOG(OAM,OAM_DETAILED,"\n Num Of Operators:- [%d]",p_mac_init_req->mocnParams.numOfOperator);

    for(count = OAM_ZERO; count < p_mac_init_req->mocnParams.numOfOperator ; count++)
        {
            OAM_LOG(OAM,OAM_DETAILED,"\n Resources for operator [%d] \n prbUsageMeasForgettingFactor [%d] ,"
                    " prbUsageMeasResetPeriod [%d] ,operator_id[%d] , dedicatedDLPRBsPerOperator [%d],"
                    " dedicatedULPRBsPerOperator [%d],reservedDLPRBsPerOperator [%d] reservedULPRBsPerOperator [%d]",
                    count+OAM_ONE,
                    p_mac_init_req->mocnParams.prbUsageMeasForgettingFactor,
                    p_mac_init_req->mocnParams.prbUsageMeasResetPeriod,
                    p_mac_init_req->mocnParams.mocnPerOperatorParams[count].operator_id,
                    p_mac_init_req->mocnParams.mocnPerOperatorParams[count].dedicatedDLPRBsPerOperator,
                    p_mac_init_req->mocnParams.mocnPerOperatorParams[count].dedicatedULPRBsPerOperator,
                    p_mac_init_req->mocnParams.mocnPerOperatorParams[count].reservedDLPRBsPerOperator,
                    p_mac_init_req->mocnParams.mocnPerOperatorParams[count].reservedULPRBsPerOperator);
        }
    /* SPR 23209 FIXED END */

	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_INIT_LAYER_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_send_mac_init_cell_req
 * Description    : This function sends init request to MAC
 * Inputs         : p_mac_init_cell_req :Pointer to MAC init cell request
 *                : src_module_id  :Source module identifier
 *                : transaction_id :transaction identifier
 * Outputs        : p_error_code   :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_send_mac_init_cell_req
(
 oam_mac_init_cell_req_params_t *p_mac_init_cell_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code,
 UInt8 cell_index
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	p_msg    = OAM_NULL;
	UInt8 *p_mac_msg;
	p_mac_msg = OAM_NULL;
    /* SPR 23504/SES-341 Fix Start */
    UInt8 count = OAM_ZERO;
    /* SPR 23504/SES-341 Fix End */

	OAM_FUN_ENTRY_TRACE();

	OAM_NULL_CHECK(OAM_NULL != p_mac_init_cell_req); 

	msg_length     = sizeof(oam_mac_init_cell_req_params_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;


	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_INIT_CELL_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;

	oam_construct_interface_api_header(p_mac_msg, transaction_id, src_module_id,
			MAC_MODULE_ID, MAC_INIT_CELL_REQ,
			msg_length,
			cell_index);

	/* Incrementing the pointer to point to the structure
	   oam_mac_init_req_params_t */
	p_mac_msg = p_mac_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_memcpy((void *)p_mac_msg, p_mac_init_cell_req, sizeof(oam_mac_init_cell_req_params_t));
    /* SPR 23504/SES-341 Fix Start */
    OAM_LOG(OAM,OAM_DETAILED,"\n MOCN PARAMS for MAC INIT CELL REQ: \n");
    OAM_LOG(OAM,OAM_DETAILED,"\n Num Of Operators:- [%d]",p_mac_init_cell_req->mocnParams.numOfOperator);

    for(count = OAM_ZERO; count < p_mac_init_cell_req->mocnParams.numOfOperator ; count++)
    {
        OAM_LOG(OAM,OAM_DETAILED,"\n Resources for operator [%d] \n prbUsageMeasForgettingFactor [%d] ,"
                " prbUsageMeasResetPeriod [%d] ,operator_id[%d] , dedicatedDLPRBsPerOperator [%d],"
                " dedicatedULPRBsPerOperator [%d],reservedDLPRBsPerOperator [%d] reservedULPRBsPerOperator [%d]",
                count+OAM_ONE,
                p_mac_init_cell_req->mocnParams.prbUsageMeasForgettingFactor,
                p_mac_init_cell_req->mocnParams.prbUsageMeasResetPeriod,
                p_mac_init_cell_req->mocnParams.mocnPerOperatorParams[count].operator_id,
                p_mac_init_cell_req->mocnParams.mocnPerOperatorParams[count].dedicatedDLPRBsPerOperator,
                p_mac_init_cell_req->mocnParams.mocnPerOperatorParams[count].dedicatedULPRBsPerOperator,
                p_mac_init_cell_req->mocnParams.mocnPerOperatorParams[count].reservedDLPRBsPerOperator,
                p_mac_init_cell_req->mocnParams.mocnPerOperatorParams[count].reservedULPRBsPerOperator);
    }
    /* SPR 23504/SES-341 Fix End */
	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_INIT_CELL_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_send_pdcp_init_layer_req
 * Description    : This function sends init request to PDCP
 * Inputs         : p_pdcp_init_req :Pointer to PDCP init layer request
 *                : src_module_id   :Source module identifier
 *                : transaction_id  :transaction identifier
 * Outputs        : p_error_code    :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_send_pdcp_init_layer_req
(
 oam_pdcp_init_layer_req_t *p_pdcp_init_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length      = OAM_ZERO;
	oam_counter_t msg_api_length  = OAM_ZERO;
	UInt8 *p_msg;
	p_msg     = OAM_NULL;
	UInt8 *p_pdcp_msg;
	p_pdcp_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	OAM_NULL_CHECK(OAM_NULL != p_pdcp_init_req); 

	/* get message API length*/
	msg_length = sizeof(oam_pdcp_init_layer_req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_pdcp_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_INIT_LAYER_REQ, msg_api_length);

	/* Fill interface header */
	p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_pdcp_msg,
			transaction_id,
			src_module_id,
			PDCP_MODULE_ID,
			PDCP_INIT_LAYER_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid
            );

	/* Incrementing the pointer to point to the structure
	   oam_pdcp_init_layer_req_t */
	p_pdcp_msg = p_pdcp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_memcpy((void *)p_pdcp_msg, p_pdcp_init_req, sizeof(oam_pdcp_init_layer_req_t));

	/* Send message to PDCP*/
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_INIT_LAYER_REQ message to PDCP with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/***********************i*****************************************************
 * Description    : This function sends log level request to MAC
 * Inputs         : p_mac_set_log_level_req : Pointer to MAC send set log level
 *                  request
 *                : src_module_id   : Source module identifier
 *                : transaction_id  : transaction identifier
 * Outputs        : p_error_code    : pointer to Error_code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_send_set_log_level_req
(
 oam_mac_log_level_req_t *p_mac_set_log_level_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length      = OAM_ZERO;
	oam_counter_t msg_api_length  = OAM_ZERO;
	UInt8 *p_msg;
	p_msg     = OAM_NULL;
	UInt8 *p_mac_msg;
	p_mac_msg        = OAM_NULL;
	oam_mac_log_level_req_t *p_temp_log_level;
	p_temp_log_level = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	/* Get API length */
	msg_length  = sizeof(oam_mac_log_level_req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_SET_LOG_LEVEL_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_SET_LOG_LEVEL_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Incrementing the pointer to point to the structure
	   oam_mac_log_level_req_t */
	p_mac_msg = p_mac_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_temp_log_level=(oam_mac_log_level_req_t *)p_mac_msg;
	p_temp_log_level->logLevel = p_mac_set_log_level_req->logLevel;

	/* Send the message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_SET_LOG_LEVEL_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}


	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pdcp_send_set_log_level_req
 * Description    : This function sends logLevel request to PDCP
 * Inputs         : p_pdcp_set_log_level_req :Pointer to PDCP set log level
 *                  request
 *                : src_module_id   :Source Module Identifier
 *                : transaction_id  :Transaction Identifier
 * Outputs        : p_error_code    :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_pdcp_send_set_log_level_req
(
 oam_pdcp_log_level_req_t *p_pdcp_set_log_level_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length           = OAM_ZERO;
	oam_counter_t msg_api_length       = OAM_ZERO;
	UInt8 *p_msg;
	p_msg      = OAM_NULL;
	UInt8 *p_pdcp_msg;
	p_pdcp_msg = OAM_NULL;
	oam_pdcp_log_level_req_t *p_temp_log_level;
	p_temp_log_level = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_length  = sizeof(oam_pdcp_log_level_req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/*Allocate Buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_pdcp_msg = p_msg;

	oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_SET_LOG_LEVEL_REQ, msg_api_length);

	p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_pdcp_msg,
			transaction_id,
			src_module_id,
			PDCP_MODULE_ID,
			PDCP_SET_LOG_LEVEL_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_pdcp_msg = p_pdcp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_temp_log_level=(oam_pdcp_log_level_req_t*)p_pdcp_msg;
	p_temp_log_level->logLevel = p_pdcp_set_log_level_req->logLevel;

	/* OAM send message to PDCP */
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_SET_LOG_LEVEL_REQ message to PDCP with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_mac_send_set_log_category_req
 * Description    : This function sends log category request to MAC
 * Inputs         : p_mac_set_log_category_req:Pointer to MAC send set log
 *                  category request
 *                : src_module_id    :Source module identifier
 *                : transaction_id   :transaction identifier
 * Outputs        : p_error_code     :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_send_set_log_category_req
(
 oam_mac_enable_log_category_req_t *p_mac_set_log_category_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	p_msg    = OAM_NULL;
	UInt8 *p_mac_msg;
	p_mac_msg = OAM_NULL;
	oam_mac_enable_log_category_req_t *p_temp_log_cat;
	p_temp_log_cat = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	/* Get API length */
	msg_length  = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_ENABLE_LOG_CATEGORY_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_ENABLE_LOG_CATEGORY_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Incrementing the pointer to point to the structure
	   oam_mac_log_category_req_t */
	p_mac_msg    = p_mac_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_temp_log_cat =(oam_mac_enable_log_category_req_t *)p_mac_msg;
	p_temp_log_cat->logCategory = p_mac_set_log_category_req->logCategory;

	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_ENABLE_LOG_CATEGORY_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}


	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;

}

/***************************************************************************
 * Function Name  : oam_pdcp_send_set_log_category_req
 * Description    : This function sends logcategory request to PDCP
 * Inputs         : p_pdcp_set_log_category_req  :Pointer to PDCP set log
 *                  category request
 *                : src_module_id   :Source module identifier
 *                : transaction_id  :Transaction identifier
 * Outputs        : p_error_code    :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_pdcp_send_set_log_category_req
(
 oam_pdcp_enable_log_category_req_t *p_pdcp_set_log_category_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et  *p_error_code
 )
{
	oam_counter_t msg_length       = OAM_ZERO;
	oam_counter_t msg_api_length   = OAM_ZERO;
	UInt8 *p_msg;
	p_msg  = OAM_NULL;
	UInt8 *p_pdcp_msg;
	p_pdcp_msg = OAM_NULL;
	oam_pdcp_enable_log_category_req_t *p_temp_log_cat;
	p_temp_log_cat = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	/* Get API length */
	msg_length  = sizeof(oam_pdcp_enable_log_category_req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_pdcp_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_ENABLE_LOG_CATEGORY_REQ, msg_api_length);

	/* Fill interface header */
	p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_pdcp_msg,
			transaction_id,
			src_module_id,
			PDCP_MODULE_ID,
			PDCP_ENABLE_LOG_CATEGORY_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Incrementing the pointer to point to the structure
	   oam_pdcp_enable_log_category_req_t */
	p_pdcp_msg = p_pdcp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_temp_log_cat = (oam_pdcp_enable_log_category_req_t *)p_pdcp_msg;
	p_temp_log_cat->logCategory = p_pdcp_set_log_category_req->logCategory;

	/* Send message to PDCP */
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_ENABLE_LOG_CATEGORY_REQ message to PDCP with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_mac_send_disable_log_category_req
 * Description    : This function send disable log category request to MAC
 * Inputs         : p_mac_set_log_category_req :Pointer to MAC send disable
 *                  log category request
 *                : src_module_id   :Source module identifier
 *                : transaction_id  :transaction identifier
 * Outputs        : p_error_code    :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_send_disable_log_category_req
(
 oam_mac_disable_log_category_req_t *p_mac_disable_log_category_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	UInt8 *p_mac_msg;
	oam_mac_disable_log_category_req_t  *p_temp_log_cat;
	p_msg          = OAM_NULL;
	p_mac_msg      = OAM_NULL;
	p_temp_log_cat = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	/* Get API length */
	msg_length  = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_DISABLE_LOG_CATEGORY_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_DISABLE_LOG_CATEGORY_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Incrementing the pointer to point to the structure
	   oam_mac_log_category_req_t */
	p_mac_msg = p_mac_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_temp_log_cat = (oam_mac_disable_log_category_req_t *)p_mac_msg;
	p_temp_log_cat->logCategory = p_mac_disable_log_category_req->logCategory;

	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_DISABLE_LOG_CATEGORY_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_pdcp_send_disable_log_category_req
 * Description    : This function sends disable logcategory request to PDCP
 * Inputs         : p_pdcp_set_log_category_req :Pointer to PDCP send disable
 *                  log category request
 *                : src_module_id   :Source module identifier
 *                : transaction_id  :transaction identifier
 * Outputs        : p_error_code    :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_pdcp_send_disable_log_category_req
(
 oam_pdcp_disable_log_category_req_t *p_pdcp_disable_log_category_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length              = OAM_ZERO;
	oam_counter_t msg_api_length          = OAM_ZERO;
	UInt8 *p_msg;
	UInt8 *p_pdcp_msg;
	oam_pdcp_disable_log_category_req_t *p_temp_log_cat;
	p_msg          = OAM_NULL;
	p_pdcp_msg     = OAM_NULL;
	p_temp_log_cat = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	/* Get API length */
	msg_length  = sizeof(oam_pdcp_disable_log_category_req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_pdcp_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_DISABLE_LOG_CATEGORY_REQ, msg_api_length);

	/* Fill interface header */
	p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_pdcp_msg,
			transaction_id,
			src_module_id,
			PDCP_MODULE_ID,
			PDCP_DISABLE_LOG_CATEGORY_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Incrementing the pointer to point to the structure
	   oam_pdcp_log_category_req_t */
	p_pdcp_msg = p_pdcp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_temp_log_cat = (oam_pdcp_disable_log_category_req_t *)p_pdcp_msg;
	p_temp_log_cat->logCategory = p_pdcp_disable_log_category_req->logCategory;

	/* Send message to PDCP*/
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_DISABLE_LOG_CATEGORY_REQ message to PDCP with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_mac_send_get_log_level_req
 * Description    : This function sends request to MAC to get loglevel info
 * Inputs         : src_module_id   :Source module identifier
 *                : transaction_id  :transaction identifier
 * Outputs        : p_error_code    :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_send_get_log_level_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	UInt8 *p_mac_msg;
	p_msg          = OAM_NULL;
	p_mac_msg      = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_GET_LOG_LEVEL_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_GET_LOG_LEVEL_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_GET_LOG_LEVEL_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pdcp_send_get_log_level_req
 * Description    : This function sends request to PDCP to get loglevel info
 * Inputs         : src_module_id  :Source Module Identifier
 *                : transaction_id :Transaction Identifier
 * Outputs        : p_error_code   :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_pdcp_send_get_log_level_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	UInt8 *p_pdcp_msg;
	p_msg          = OAM_NULL;
	p_pdcp_msg      = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/*Allocate Buffer*/
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_pdcp_msg = p_msg;

	oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_GET_LOG_LEVEL_REQ, msg_api_length);

	p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_pdcp_msg,
			transaction_id,
			src_module_id,
			PDCP_MODULE_ID,
			PDCP_GET_LOG_LEVEL_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* OAM send message to PDCP */
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_GET_LOG_LEVEL_REQ message to PDCP with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_mac_send_get_log_category_req
 * Description    : This function sends request to MAC to get logcategory info
 * Inputs         : src_module_id  :Source module identifier
 *                : transaction_id :transaction identifier
 * Outputs        : p_error_code   :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_send_get_log_category_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	UInt8 *p_mac_msg;
	p_msg          = OAM_NULL;
	p_mac_msg      = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_GET_LOG_CATEGORY_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_GET_LOG_CATEGORY_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to MAC */
	if(	OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_GET_LOG_CATEGORY_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pdcp_send_get_log_category_req
 * Description    : This function sends request to PDCP to get log category info
 * Inputs         : src_module_id  :Source module identifier
 *                : transaction_id :Transaction identifier
 * Outputs        : p_error_code   :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_pdcp_send_get_log_category_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et  *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	UInt8 *p_pdcp_msg;
	p_msg          = OAM_NULL;
	p_pdcp_msg     = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_pdcp_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_GET_LOG_CATEGORY_REQ, msg_api_length);

	/* Fill interface header */
	p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_pdcp_msg,
			transaction_id,
			src_module_id,
			PDCP_MODULE_ID,
			PDCP_GET_LOG_CATEGORY_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to PDCP */
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_GET_LOG_CATEGORY_REQ message to PDCP with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_mac_get_loglevel_resp
 * Description    : This function provide current loglevel of mac module to oam
 * Inputs         : p_api_buf :pointer to MAC get loglevel response
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_get_loglevel_resp
(
 void* p_api_buf
    /*SPR 17777 fix*/

 )
{
	/*SPR 13617 START*/
	oam_mac_get_log_level_resp_t *p_oam_get_loglevel_mac;
	/*SPR 13617 END*/
	p_oam_get_loglevel_mac = OAM_NULL;
	/* void *timer_ptr; */
	OAM_FUN_ENTRY_TRACE();
	UInt16 tcb_detail = get_word_from_header(p_api_buf + 0);
	p_api_buf      = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	/*SPR 13617 START*/
	p_oam_get_loglevel_mac = (oam_mac_get_log_level_resp_t *)p_api_buf;
	/*SPR 13617 END*/


	OAM_LOG(OAM,OAM_INFO,"L2-MAC LOG LEVEL INFORMATION, Loglevel = %d",p_oam_get_loglevel_mac->logLevel);

	UInt32 bitmask = 0;
	oam_get_log_level_l2_all.mac_loglevel = p_oam_get_loglevel_mac->logLevel;
	/*SPR 17842 START*/
	if(bitmask == OAM_ZERO && global_trans_l2_log_flag!= OAM_ONE)
	{  
	/*SPR 17842 END*/
		/*SPR 13617 START*/
		oam_qcli_send_All_response(&oam_get_log_level_l2_all,
				tcb_detail,OAM_GET_LOG_LEVEL_MAC_RESP,sizeof(oam_get_log_level_l2_all_t));
		/*SPR 13617 END*/

	}
	/*SPR 17842 START*/
        else
        {
                global_trans_bitmap_l2_log|= OAM_ONE;
                if(global_trans_bitmap_l2_log==OAM_FIFTEEN) 
     		{
                    oam_qcli_send_All_response(&oam_get_log_level_l2_all,
				tcb_detail,OAM_GET_LOG_LEVEL_L2_RESP,sizeof(oam_get_log_level_l2_all_t));
                global_trans_l2_log_flag = OAM_ZERO;
                global_trans_bitmap_l2_log=OAM_ZERO;
                }
        }

	/*SPR 17842 END*/

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_rlc_get_loglevel_resp
 * Description    : This function provide current loglevel of rlc module to oam
 * Inputs         : p_api_buf :pointer to RLC get loglevel response
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_rlc_get_loglevel_resp
(
 void* p_api_buf
    /*SPR 17777 fix*/
)
{
    Oam_LogLevel_Req_t *p_oam_get_loglevel = OAM_NULL;
    OAM_FUN_ENTRY_TRACE();
    UInt16 tcb_detail = get_word_from_header(p_api_buf + 0);
    p_api_buf      = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    p_oam_get_loglevel = (Oam_LogLevel_Req_t *)p_api_buf;

    OAM_LOG(OAM,OAM_INFO,"L2-RLC LOG LEVEL INFORMATION, Loglevel = %d",p_oam_get_loglevel->logLevel);

    UInt32 bitmask = 0;

    /*SPR 17842 START*/
       /*code Deleted*/ 
    /*SPR 17842 END*/
    oam_get_log_level_l2_all.rlc_loglevel = p_oam_get_loglevel->logLevel;
    /*SPR 17842 START*/
    if(bitmask == OAM_ZERO && global_trans_l2_log_flag!=OAM_ONE)
    {  
   /*SPR 17842 END */
     oam_qcli_send_All_response(&oam_get_log_level_l2_all,
                tcb_detail,OAM_GET_LOG_LEVEL_RLC_RESP,sizeof(oam_get_log_level_l2_all_t));
    }
    /*SPR 17842 START*/
    else
    {
                global_trans_bitmap_l2_log|= OAM_TWO;
                if(global_trans_bitmap_l2_log==OAM_FIFTEEN) 
     		{  oam_qcli_send_All_response(&oam_get_log_level_l2_all,
				tcb_detail,OAM_GET_LOG_LEVEL_L2_RESP,sizeof(oam_get_log_level_l2_all_t));
                global_trans_l2_log_flag = OAM_ZERO;
                global_trans_bitmap_l2_log=OAM_ZERO;
                }
    }
    /*SPR 17842 END*/
OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pdcp_get_loglevel_resp
 * Description    : This function provide current loglevel of pdcp module to oam
 * Inputs         : p_api_buf :pointer to PDCP get loglevel response
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_pdcp_get_loglevel_resp
(
 void* p_api_buf
    /*SPR 17777 fix*/
)
{
    oam_pdcp_log_level_req_t *p_oam_get_loglevel = OAM_NULL;
    OAM_FUN_ENTRY_TRACE();
    UInt16 tcb_detail = get_word_from_header(p_api_buf + 0);
    p_api_buf      = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    p_oam_get_loglevel = (oam_pdcp_log_level_req_t *)p_api_buf;


    OAM_LOG(OAM,OAM_INFO,"L2-PDCP LOG LEVEL INFORMATION, Loglevel = %d",p_oam_get_loglevel->logLevel);

    UInt32 bitmask = 0;
    oam_get_log_level_l2_all.pdcp_loglevel = p_oam_get_loglevel->logLevel;
    /*SPR 17842 START*/
    if(bitmask == OAM_ZERO && global_trans_l2_log_flag!=OAM_ONE)
    {  
     /*SPR 17842 END */
        oam_qcli_send_All_response(&oam_get_log_level_l2_all,
                tcb_detail,OAM_GET_LOG_LEVEL_PDCP_RESP,sizeof(oam_get_log_level_l2_all_t));
    }
    /*SPR 17842 START*/
    else
    {
                global_trans_bitmap_l2_log|= OAM_FOUR;
                if(global_trans_bitmap_l2_log==OAM_FIFTEEN) 
     		{

                  oam_qcli_send_All_response(&oam_get_log_level_l2_all,
				tcb_detail,OAM_GET_LOG_LEVEL_L2_RESP,sizeof(oam_get_log_level_l2_all_t));
                global_trans_l2_log_flag = OAM_ZERO;
                global_trans_bitmap_l2_log=OAM_ZERO;
                }  

    }
    /*SPR 17842 END*/
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_egtpu_get_loglevel_resp
 * Description    : This function provide current loglevel of egtpu module to oam
 * Inputs         : p_api_buf :pointer to EGTPU get loglevel response
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_egtpu_get_loglevel_resp
(
 void* p_api_buf
    /*SPR 17777 fix*/
)
{
    oam_relay_Gtpu_Get_Log_Lvl_Req_t *p_oam_get_loglevel = OAM_NULL;
    OAM_FUN_ENTRY_TRACE();
    UInt16 tcb_detail = get_word_from_header(p_api_buf + 0);
    p_api_buf      = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    p_oam_get_loglevel = (oam_relay_Gtpu_Get_Log_Lvl_Req_t *)p_api_buf;

    UInt32 bitmask = 0;

    oam_cp_unpack_UInt32(&oam_get_log_level_l2_all.gtpu_loglevel, p_oam_get_loglevel
      ,"log_level"
    );
    OAM_LOG(OAM,OAM_INFO,"L2-EGTPU LOG LEVEL INFORMATION, Loglevel = %d",oam_get_log_level_l2_all.gtpu_loglevel);
    /*SPR 17842 START*/
    if(bitmask == OAM_ZERO && global_trans_l2_log_flag!=OAM_ONE)
    {  
     /*SPR 17842 END */

        oam_qcli_send_All_response(&oam_get_log_level_l2_all,
                tcb_detail,OAM_GET_LOG_LEVEL_GTPU_RESP,sizeof(oam_get_log_level_l2_all_t));
    }
    /*SPR 17842 START*/
    else
    {
                global_trans_bitmap_l2_log|= OAM_EIGHT;
                if(global_trans_bitmap_l2_log==OAM_FIFTEEN) 
     		{
                     oam_qcli_send_All_response(&oam_get_log_level_l2_all,
				tcb_detail,OAM_GET_LOG_LEVEL_L2_RESP,sizeof(oam_get_log_level_l2_all_t));	
                global_trans_l2_log_flag = OAM_ZERO;
                global_trans_bitmap_l2_log=OAM_ZERO;
                }
    }
    /*SPR 17842 END*/
OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pdcp_get_logcategory_resp
 * Description    : This function provide current logcategory of pdcp module
 *                  to oam
 * Inputs         : p_api_buf   :pointer to PDCP get logcategory response
 * Outputs        : p_error_code:pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_pdcp_get_logcategory_resp
(
 void* p_api_buf,
/*SPR 19771 FIXED START*/
 oam_error_code_et *p_error_code
/*SPR 19771 FIXED END*/
    /*SPR 17777 fix*/

 )
{
	UInt16 trans_id =OAM_ZERO;
	UInt16 dest_id =OAM_ZERO;
	/* SPR 55317 - Redundant Code Removed Here */
	oam_pdcp_get_log_category_resp_t oam_get_logcategory_pdcp;
    /*SPR 19771 FIXED START*/
    UInt8 *p_msg = OAM_NULL;
    UInt8 *p_pdcp_msg = OAM_NULL;
    oam_counter_t msg_api_length = OAM_ZERO;
    /*SPR 19771 FIXED END*/

	OAM_FUN_ENTRY_TRACE();


	if(OAM_FAILURE == oam_process_tcb_for_layer_resp(
				p_api_buf,&trans_id,&dest_id))
	{
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
    /*SPR 19771 FIXED START*/
    /* code removed */ 
    /*SPR 19771 FIXED END*/


	p_api_buf      = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	oam_get_logcategory_pdcp.logCategory =(UInt64)(((oam_pdcp_get_log_category_resp_t *)p_api_buf)->
			logCategory);

	/* check loglevel is set or not*/
	if( OAM_ZERO == oam_get_logcategory_pdcp.logCategory)
	{
		OAM_LOG(OAM,OAM_INFO,"No logCategory is enabled as Received"
				" from PDCP");
	}

    /*SPR 19771 FIXED START*/
    msg_api_length = sizeof(oam_pdcp_get_log_category_resp_t) + OAM_CSPL_HDR_SIZE + OAM_MGMT_INTF_HEADER_SIZE;
    if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
                *p_error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
	}
    oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
    p_pdcp_msg = p_msg;
    /* Fill CSPL header */
    oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID,OAM_MODULE_ID,
            dest_id,OAM_GET_LOG_CAT_PDCP_RESP, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_pdcp_msg, trans_id,OAM_MODULE_ID,
            dest_id,OAM_GET_LOG_CAT_PDCP_RESP, sizeof(oam_pdcp_get_log_category_resp_t) ,oam_prov_req.cell_cntxt.curr_cell_cid);
    p_pdcp_msg = p_pdcp_msg + OAM_MGMT_INTF_HEADER_SIZE;
    oam_memcpy(p_pdcp_msg,&oam_get_logcategory_pdcp,sizeof(oam_pdcp_get_log_category_resp_t));
    if(OAM_SUCCESS == oam_send_message(p_msg, dest_id, p_error_code))
    {
        OAM_LOG(OAM,OAM_INFO,"Sending message to Management Interface");
    }
    else
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Sending message with error code %d",
                *p_error_code);
        /* Coverity 100045 Fix Start */
        /* Coverity CID 85274 Fix Start */
        //oam_mem_free((void *)p_msg, p_error_code);
        /* Coverity CID 85274 Fix End */
        /* Coverity 100045 Fix End */
	OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    /*SPR 19771 FIXED END*/

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_mac_get_logcategory_resp
 * Description    : This function provide current logcategory of mac module to
 *                  oam
 * Inputs         : p_api_buf    :pointer to MAC get logcategory response
 * Outputs        : p_error_code :pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_get_logcategory_resp
(
 void* p_api_buf,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt16 trans_id =OAM_ZERO;
	UInt16 dest_id =OAM_ZERO;
	UInt8 *p_msg;
	UInt8 *p_mac_msg;
	p_msg          = OAM_NULL;
	p_mac_msg      = OAM_NULL;
	oam_mac_get_log_category_req_t  oam_get_logcategory_mac;

	OAM_FUN_ENTRY_TRACE();


	if(OAM_FAILURE == oam_process_tcb_for_layer_resp(p_api_buf,&trans_id,&dest_id))
	{
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_api_buf      = p_api_buf + OAM_MGMT_INTF_HEADER_SIZE;
	oam_get_logcategory_mac.logCategory = (((oam_mac_get_log_category_req_t *)(p_api_buf))->logCategory) ;

	msg_api_length = sizeof(oam_mac_get_log_category_req_t) + OAM_CSPL_HDR_SIZE + OAM_MGMT_INTF_HEADER_SIZE;


	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			dest_id, OAM_GET_LOG_CAT_MAC_RESP, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			trans_id,
			OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_CAT_MAC_RESP,
			sizeof(oam_mac_get_log_category_req_t),
			oam_prov_req.cell_cntxt.curr_cell_cid);


	p_mac_msg = p_mac_msg + OAM_MGMT_INTF_HEADER_SIZE;
	oam_memcpy(p_mac_msg,&oam_get_logcategory_mac.logCategory,sizeof(UInt64));
	/* Send message to MAC */
	if( OAM_SUCCESS == oam_send_message(p_msg, dest_id,p_error_code))
	{
		OAM_LOG(OAM,OAM_INFO,"Sending OAM_GET_LOG_CAT_MAC_RESP message to Management Interface");
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending OAM_GET_LOG_CAT_MAC_RESP message with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_pdcp_config_kpi_stats_req
 * Description    : This function sends the pdcp config kpi stats request to
 *                  PDCP
 * Inputs         : p_pdcp_config_kpi_stats_req :Pointer to PDCP configure kpi
 *                  stats request
 *                : src_module_id   :Source module identifier
 *                : transaction_id  :transaction identifier
 * Outputs        : p_error_code    :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_pdcp_config_kpi_stats_req
(
 oam_pdcp_config_KPI_stats_req_t *p_pdcp_config_kpi_stats_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 /*SPR 21941 Start*/
 oam_error_code_et *p_error_code,
 UInt8 l2_instance
 /*SPR 21941 End*/
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	UInt8 *p_pdcp_msg;
	p_msg          = OAM_NULL;
	p_pdcp_msg     = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	/* Get API length */
	msg_length  = sizeof(oam_pdcp_config_KPI_stats_req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +  LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_pdcp_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_CONFIGURE_KPI_STATS_REQ, msg_api_length);

	/* Fill interface header */
	p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_pdcp_msg,
			transaction_id,
			src_module_id,
			PDCP_MODULE_ID,
			PDCP_CONFIGURE_KPI_STATS_REQ,
			msg_length,
 /*SPR 21941 Start*/
			L2_INSTANCE_CELL_INFO(l2_instance,OAM_ZERO).cell_idx);
 /*SPR 21941 End*/


	/* Incrementing the pointer to point to the structure
	   oam_pdcp_config_KPI_stats_req_t */
	p_pdcp_msg = p_pdcp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_memcpy((void *)p_pdcp_msg, p_pdcp_config_kpi_stats_req,
			sizeof(oam_pdcp_config_KPI_stats_req_t));

	/* OAM send message to PDCP */
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_CONFIGURE_KPI_STATS_REQ message to PDCP with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	Config_Flag=1;

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pdcp_kpi_stat_ind
 * Description    : This function provide indication for PDCP KPI Statistics                  
 * Inputs         : p_api_buf :pointer to PDCP kpi stats indication
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_pdcp_kpi_stat_ind
(
 void* p_api_buf
 )
{
	oam_counter_t loop_counter = OAM_ZERO;
	UInt8 flag         = OAM_ZERO;
	oam_pdcp_KPI_stats_ind_t *p_oam_pdcp_kpi_stats_ind;
	p_oam_pdcp_kpi_stats_ind = OAM_NULL;
	OAM_FUN_ENTRY_TRACE();

	p_api_buf                    = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_oam_pdcp_kpi_stats_ind     = (oam_pdcp_KPI_stats_ind_t *)p_api_buf;

	OAM_LOG(OAM,OAM_INFO,"Received PDCP_KPI_STATS_IND from PDCP");

	/* To Create Current Time Stamp Value */
	create_time_stamp();
	OAM_LOG(OAM,OAM_DETAILED, "Current TimeStamp[%s]. PDCP_KPI_STATS_IND with duration[%d]", 
        time_buffer, p_oam_pdcp_kpi_stats_ind->durationTTI/1000);

	/* loop for MAX_QCI */
	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
	{
		/*if (loop_counter < OAM_ZERO || loop_counter >= MAX_QCI)
		  {
		  OAM_LOG(OAM,OAM_ERROR,"Invalid value for the for loop");
		  OAM_FUN_EXIT_TRACE();
		  return OAM_FAILURE;
		  }*/
		/* check for each counter value */
		if(p_oam_pdcp_kpi_stats_ind->totalSduRcvdDlperQci[loop_counter] != OAM_ZERO)
		{
			OAM_LOG(OAM,OAM_DETAILED, "  For QCI [%d]", loop_counter+1);

			/* kpi's corrosponding to which bit is set*/
			if(kpiPdcpConBitMap & 1 )
			{
				OAM_LOG(OAM,OAM_DETAILED, " totalDlDrbDiscardRatePerQci [%.02f]",
						(float)(p_oam_pdcp_kpi_stats_ind->
							totalDlDrbDiscardRatePerQci[loop_counter] * 1000000)/
						(float)(p_oam_pdcp_kpi_stats_ind->
							totalSduRcvdDlperQci[loop_counter]));
			}
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
/*spr 19296 fix start*/
			if(kpiPdcpConBitMap & 2 )
/*spr 19296 fix end*/
			{
				OAM_LOG(OAM,OAM_DETAILED, " totalDlDrbUuLossRatePerQci [%.02f]",
						(float)(p_oam_pdcp_kpi_stats_ind->
							totalUlLossRatePerQci[loop_counter] * 1000000)/
						(float)(p_oam_pdcp_kpi_stats_ind->
							totalSduRcvdUlperQci[loop_counter]));
			}
			flag = 1;
		}
	}
	/*BUG_ID-121*/
	if(OAM_ZERO == flag )
	{
		/*BUG_ID-119*/
		OAM_LOG(OAM,OAM_WARNING, " All Stats Received in PDCP_KPI_STATS_IND"
				" are NULL");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_mac_kpi_stat_ind
 * Description    : This function provide indication for MAC KPI Statistics
 *                  from OAM
 * Inputs         : p_api_buf :pointer to MAC kpi stats indication
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_kpi_stat_ind
(
 void* p_api_buf
 )
{
	UInt8*    p_msg;
	p_msg = OAM_NULL;
	oam_mac_KPI_stats_ind_t *p_oam_mac_kpi_stats_ind;
	p_oam_mac_kpi_stats_ind = OAM_NULL;
    /* SPR 23209 FIXED START */
    oam_counter_t loop_counter2  = OAM_ZERO;
    /* SPR 23209 FIXED END */

	oam_counter_t loop_counter  = OAM_ZERO;
	UInt32 durationTTI        = OAM_ZERO;
	UInt32 PRB                = OAM_ZERO;

	OAM_FUN_ENTRY_TRACE();

	p_msg      = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_oam_mac_kpi_stats_ind = (oam_mac_KPI_stats_ind_t *)p_msg;
	durationTTI = p_oam_mac_kpi_stats_ind->DurationTTI;
	PRB=200;

	OAM_LOG(OAM,OAM_INFO,"Received  MAC_KPI_STATS_IND with duration[%u]",/*BUG_ID-126*/
			p_oam_mac_kpi_stats_ind->DurationTTI);

	/* To Create Current Time Stamp Value */
	create_time_stamp();
	OAM_LOG(OAM,OAM_DETAILED, "  Current TimeStamp[%s]", time_buffer);

	/* kpi's corrosponding to which bit is set */
	if(kpiToReportBitMap & 1 )
	{
		OAM_LOG(OAM,OAM_DETAILED, "  MAC_KPI_STATS_IND with totalPRBUsageDL[%.02f]",
				(float)(p_oam_mac_kpi_stats_ind-> totalPRBUsageDL * 100)/
				(float)(PRB * durationTTI)) ;
	}
	if(kpiToReportBitMap & 2 )
	{
		OAM_LOG(OAM,OAM_DETAILED, "  MAC_KPI_STATS_IND with totalPRBUsageUL[%.02f]",
				(float)(p_oam_mac_kpi_stats_ind->totalPRBUsageUL * 100)/
				(float)(PRB * durationTTI));
	}

	/* loop for RRM_MAX_QCI */ /*BUG_ID-110*/
	for(loop_counter = OAM_ZERO;loop_counter < MAX_QCI;loop_counter++)
	{
		/*BUG_ID-111*/
		/*if (loop_counter < OAM_ZERO || loop_counter >= MAX_QCI)
		  {
		  OAM_LOG(OAM,OAM_ERROR,"Invalid value for the for loop");
		  OAM_FUN_EXIT_TRACE();
		  return OAM_FAILURE;

		  }*/
		if((511 == kpiToReportBitMap)|| (3 < kpiToReportBitMap &&
					64 !=  kpiToReportBitMap && 256 != kpiToReportBitMap ) )
		{
			if(OAM_ZERO != p_oam_mac_kpi_stats_ind->totalPRBUsagePerQciDL[loop_counter] ||
					OAM_ZERO != p_oam_mac_kpi_stats_ind->totalPRBUsagePerQciUL[loop_counter] ||
					OAM_ZERO != p_oam_mac_kpi_stats_ind->numberOfActiveUEPerQciDL[loop_counter]||
					OAM_ZERO != p_oam_mac_kpi_stats_ind->numberOfActiveUEPerQciUL[loop_counter]||
					OAM_ZERO != p_oam_mac_kpi_stats_ind->totalPktDelayPerQciDL[loop_counter])
			{
				OAM_LOG(OAM,OAM_DETAILED, "  For QCI [%d]", loop_counter+1);

				if(kpiToReportBitMap & 4 )
				{
					OAM_LOG(OAM,OAM_DETAILED, "  totalPRBUsagePerQciDL[%.02f]",
							(float)(p_oam_mac_kpi_stats_ind->
								totalPRBUsagePerQciDL[loop_counter]* 100)/
							(float)(PRB * durationTTI));
				}

				if(kpiToReportBitMap & 8 )
				{
					OAM_LOG(OAM,OAM_DETAILED, "  totalPRBUsagePerQciUL[%.02f]",
							(float)(p_oam_mac_kpi_stats_ind->
								totalPRBUsagePerQciUL[loop_counter]* 100)/
							(float)(PRB * durationTTI));
				}

				if(kpiToReportBitMap & 16 )
				{
					OAM_LOG(OAM,OAM_DETAILED, "  numberOfActiveUEPerQciDL[%.02f]",
							((float)((p_oam_mac_kpi_stats_ind->
									numberOfActiveUEPerQciDL[loop_counter]*
									ACTIVE_USER_SAMPLING_TIMER_TTI)/durationTTI)/
							 1000.00));
				}

				if(kpiToReportBitMap & 32 )
				{
					OAM_LOG(OAM,OAM_DETAILED, "  numberOfActiveUEPerQciUL[%.02f]",
							((float)((p_oam_mac_kpi_stats_ind->
									numberOfActiveUEPerQciUL[loop_counter]*
									ACTIVE_USER_SAMPLING_TIMER_TTI)/durationTTI)/
							 1000.00));
				}

				if(( 0 != (kpiToReportBitMap & 128)) && (p_oam_mac_kpi_stats_ind->
							totalPktAckedInDLPerQci[loop_counter]) )
				{
					OAM_LOG(OAM,OAM_DETAILED, " totalPktDelayPerQciDL[%.02f]",
							(float)(p_oam_mac_kpi_stats_ind->
								totalPktDelayPerQciDL[loop_counter])/
							(float)(p_oam_mac_kpi_stats_ind->
								totalPktAckedInDLPerQci[loop_counter]));
				}
			}
			/*BUG_ID-112*/
			else
			{
                /* SPR 22304 Fix Start */
				OAM_LOG(OAM,OAM_WARNING,"The value of entered structure member is equals to ZERO");
                /* SPR 22304 Fix End */
			}
		}
	}

    /* SPR 23209 FIXED START*/
    /* loop for RRM_MAX_QCI */ /*BUG_ID-110*/
    for(loop_counter = OAM_ZERO;loop_counter < OAM_MAX_PLMNLIST_OBJECTS;loop_counter++)
    {
        OAM_LOG(OAM,OAM_DETAILED, "\n\t  For OPERATOR [%d]",loop_counter + 1 );
        OAM_LOG(OAM,OAM_DETAILED, "\n\t  operatorId [%d]",p_oam_mac_kpi_stats_ind->
                kpiPerOperator[loop_counter].operatorId );

        for(loop_counter2 = OAM_ZERO;loop_counter2 < MAX_QCI ;loop_counter2++)
        {
            if((511 == kpiToReportBitMap)|| (3 < kpiToReportBitMap &&
                        64 !=  kpiToReportBitMap && 256 != kpiToReportBitMap ) )
            {
                if( OAM_ZERO != p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].
                        totalPRBUsagePerQciDL[loop_counter2] ||
                        OAM_ZERO != p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].
                        totalPRBUsagePerQciUL[loop_counter2] ||
                        OAM_ZERO != p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].
                        numberOfActiveUEPerQciDL[loop_counter2]||
                        OAM_ZERO != p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].
                        numberOfActiveUEPerQciUL[loop_counter2]
                  )
                {
                    OAM_LOG(OAM,OAM_DETAILED, "\n\t  For QCI [%d]", loop_counter+1);
                    if(kpiToReportBitMap & 4 )
                    {
                        OAM_LOG(OAM,OAM_DETAILED, "\n\t  totalPRBUsagePerQciDL[%.02f]",
                                (float)(p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].
                                    totalPRBUsagePerQciDL[loop_counter2]* 100)/
                                (float)(PRB * durationTTI));
                    }

                    if(kpiToReportBitMap & 8 )
                    {
                        OAM_LOG(OAM,OAM_DETAILED, "\n\t  totalPRBUsagePerQciUL[%.02f]",
                                (float)(p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].
                                    totalPRBUsagePerQciUL[loop_counter2]* 100)/
                                (float)(PRB * durationTTI));
                    }

                    if(kpiToReportBitMap & 16 )
                    {
                        OAM_LOG(OAM,OAM_DETAILED, "\n\t  numberOfActiveUEPerQciDL[%.02f]",
                                ((float)((p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].
                                        numberOfActiveUEPerQciDL[loop_counter2]*
                                        ACTIVE_USER_SAMPLING_TIMER_TTI)/durationTTI)/
                                 1000.00));
                    }

                    if(kpiToReportBitMap & 32 )
                    {
                        OAM_LOG(OAM,OAM_DETAILED, "\n\t  numberOfActiveUEPerQciUL[%.02f]",
                                ((float)((p_oam_mac_kpi_stats_ind->kpiPerOperator[loop_counter].
                                        numberOfActiveUEPerQciUL[loop_counter2]*
                                        ACTIVE_USER_SAMPLING_TIMER_TTI)/durationTTI)/
                                 1000.00));
                    }

                }
                /*BUG_ID-112*/
                else
                {
                    OAM_LOG(OAM,OAM_WARNING,"\nThe value of entered structure member is equals to ZERO");
                }
            }
        }
    }
    /* SPR 23209 FIXED END */

	if(kpiToReportBitMap & 64 )
	{
		OAM_LOG(OAM,OAM_DETAILED, " rcvdRandomAccessPreamblesContentionFree"
				" [%.02f]",(float)(p_oam_mac_kpi_stats_ind->
			rcvdRandomAccessPreamblesContentionFree * 1000)/
				(float)durationTTI);

		OAM_LOG(OAM,OAM_DETAILED, " rcvdRandomAccessPreamblesGroupA[%.02f]",
				(float)(p_oam_mac_kpi_stats_ind->
					rcvdRandomAccessPreamblesGroupA * 1000)/
				(float)durationTTI);
		OAM_LOG(OAM,OAM_DETAILED, " rcvdRandomAccessPreamblesGroupB[%.02f]",
				(float)(p_oam_mac_kpi_stats_ind->
					rcvdRandomAccessPreamblesGroupB  * 1000)/
				(float)durationTTI);
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_mac_config_kpi_stats_req
 * Description    : This function sends the mac config kpi stats request to MAC
 * Inputs         : p_mac_config_kpi_stats_req :Pointer to MAC configure kpi
 *                  stats request
 *                : src_module_id    :Source module identifier
 *                : transaction_id   :transaction identifier
 * Outputs        : p_error_code     :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_config_kpi_stats_req
(
 oam_mac_config_KPI_stats_req_t *p_mac_config_kpi_stats_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 /*SPR 21941 Start*/
 oam_error_code_et *p_error_code,
 UInt8 l2_instance
 /*SPR 21941 End*/
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	p_msg = OAM_NULL;
	UInt8 *p_mac_msg;
	p_mac_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	/* Get API length */
	msg_length  = sizeof(oam_mac_config_KPI_stats_req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +  LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_CONFIGURE_KPI_STATS_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_CONFIGURE_KPI_STATS_REQ,
			msg_length,
            /*SPR 21941 Start*/
			L2_INSTANCE_CELL_INFO(l2_instance,OAM_ZERO).cell_idx);
            /*SPR 21941 End*/

	/* Incrementing the pointer to point to the structure
	   oam_mac_config_KPI_stats_req_t */
	p_mac_msg = p_mac_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_memcpy((void *)p_mac_msg, p_mac_config_kpi_stats_req,
			sizeof(oam_mac_config_KPI_stats_req_t));

	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_CONFIGURE_KPI_STATS_REQ message to MAC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	Config_Flag=1;

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/* CSR_00057386 */

/****************************************************************************
 * Function Name  : oam_mac_get_ue_status_req
 * Description    : This function sends mac ue status request to MAC
 * Inputs         : p_mac_ue_status_req :Pointer to MAC ue status request
 *                : src_module_id    :Source module identifier
 *                : transaction_id   :transaction identifier
 * Outputs        : p_error_code     :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_get_ue_status_req
(
 oam_mac_ue_status_req_t *p_mac_ue_status_req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	p_msg = OAM_NULL;
	UInt8 *p_mac_msg;
	p_mac_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	/* Get API length */
	msg_length  = sizeof(oam_mac_ue_status_req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +  LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_GET_STATUS_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_GET_STATUS_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Incrementing the pointer to point to the structure
	   oam_mac_config_KPI_stats_req_t */
	p_mac_msg = p_mac_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_memcpy((void *)p_mac_msg, p_mac_ue_status_req,
			sizeof(oam_mac_ue_status_req_t));

	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_GET_STATUS_REQ message to MAC with error code %d",
				*p_error_code);

		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_mac_get_ue_status_resp
 * Description    : This function provide ue status received from mac
 * Inputs         : p_api_buf :pointer to MAC get ue status response
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_get_ue_status_resp
(
 void* p_api_buf,
 oam_error_code_et *p_error_code

 )
{
	UInt8 *temp_arr[MAC_STATUS_ARR_SIZE];     /* this is the array name */
	UInt16 size_y = MAC_2D_ARR_SIZE; /* this variable will be used for the second dimension */
	UInt8 str_count = OAM_ZERO;
	UInt16 trans_id = OAM_ZERO;
	UInt16 dst_id = QCLI_MODULE_ID; 

	oam_mac_ue_status_resp_t *p_mac_ue_status_resp = OAM_NULL;
	oam_counter_t   loop_counter      = OAM_ZERO;

	OAM_FUN_ENTRY_TRACE();

	trans_id = get_word_from_header((UInt8 *)p_api_buf);
	p_api_buf = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_mac_ue_status_resp = (oam_mac_ue_status_resp_t*)p_api_buf;

	if(OAM_SUCCESS != p_mac_ue_status_resp->responseCode)
	{
		*p_error_code = OAM_INTERNAL_ERROR;
		OAM_LOG(OAM,OAM_WARNING," Response Code Failure received from MAC for UE STATUS ");
		/*CSR00064481 FIX*/
		/* CSR_69212_FIX_START */
		oam_construct_n_send_gpv_response((UInt8 *)"UeStatus.MAC",temp_arr,str_count,
				trans_id,p_error_code,dst_id,OAM_ZERO ,OAM_ZERO);
		/* CSR_69212_FIX_END */
		/*CSR00064481 FIX*/
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	/*  allocate storage for an array of pointer to char */
	for (loop_counter = OAM_ZERO; loop_counter < MAC_STATUS_ARR_SIZE; loop_counter++)
	{
		if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt16)),(void *)&temp_arr[loop_counter],p_error_code))
		{
                /* Coverity CID 72343 Fix Start */
                oam_counter_t loop_release_mem = OAM_ZERO;
                for(;loop_release_mem < loop_counter;loop_release_mem++)
                {
                    oam_mem_free(temp_arr[loop_release_mem],p_error_code);
                }
                /*+-coverity_87742_fix*/
                /* Coverity CID 72343 Fix End */
                OAM_LOG(OAM, OAM_ERROR,
                        "Memory allocation to temp_arr failed"
                        " with Error Code:%d",
					*p_error_code);
			return OAM_FAILURE;
		}
		oam_memset(temp_arr[loop_counter], 0x00, (size_y * sizeof(UInt16)));
	}


	/* KlockWork_Fix */ 
	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","numOfUe",
				p_mac_ue_status_resp->numOfUe);


	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","rnti",
				p_mac_ue_status_resp->macUEStatusInfo[0].rnti);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","uePriority",
				p_mac_ue_status_resp->macUEStatusInfo[0].uePriority);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","numTxAntenna",
				p_mac_ue_status_resp->macUEStatusInfo[0].numTxAntenna);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","dlTxMode",
				p_mac_ue_status_resp->macUEStatusInfo[0].dlTxMode);


	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","dlNumOfLayer",
				p_mac_ue_status_resp->macUEStatusInfo[0].dlNumOfLayer);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","dlNumOfLCs",
				p_mac_ue_status_resp->macUEStatusInfo[0].dlNumOfLCs);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","dlMaxRB",
				p_mac_ue_status_resp->macUEStatusInfo[0].dlMaxRB);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","dlModSchemeCodeWord0",
				p_mac_ue_status_resp->macUEStatusInfo[0].dlModSchemeCodeWord0);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","dlModSchemeCodeWord1",
				p_mac_ue_status_resp->macUEStatusInfo[0].dlModSchemeCodeWord1);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","dlMcsIndexCodeWord0",
				p_mac_ue_status_resp->macUEStatusInfo[0].dlMcsIndexCodeWord0);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","dlMcsIndexCodeWord1",
				p_mac_ue_status_resp->macUEStatusInfo[0].dlMcsIndexCodeWord1);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","dlQueueLoad",
				p_mac_ue_status_resp->macUEStatusInfo[0].dlQueueLoad);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","dlCodeBookIndex",
				p_mac_ue_status_resp->macUEStatusInfo[0].dlCodeBookIndex);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","cqiIndicator",
				p_mac_ue_status_resp->macUEStatusInfo[0].cqiIndicator);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","simultaneousAckNackAndCQI",
				p_mac_ue_status_resp->macUEStatusInfo[0].simultaneousAckNackAndCQI);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","ulNumOfLCs",
				p_mac_ue_status_resp->macUEStatusInfo[0].ulNumOfLCs);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","bsrNet0",
				p_mac_ue_status_resp->macUEStatusInfo[0].bsrNet0);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","bsrNet1",
				p_mac_ue_status_resp->macUEStatusInfo[0].bsrNet1);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","bsrNet2",
				p_mac_ue_status_resp->macUEStatusInfo[0].bsrNet2);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","bsrNet3",
				p_mac_ue_status_resp->macUEStatusInfo[0].bsrNet3);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","ulModScheme",
				p_mac_ue_status_resp->macUEStatusInfo[0].ulModScheme);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","ulMcsIndex",
				p_mac_ue_status_resp->macUEStatusInfo[0].ulMcsIndex);

	UInt8 i;

	/* klocwork fix 1263 */ 
	for( i=OAM_ZERO; i<MAX_LOGICAL_CHANNEL; i++)
	{
		if(str_count < MAC_STATUS_ARR_SIZE)
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s[%d].%s=%d","macUeDLLogChStatus",i,"lcId",
					p_mac_ue_status_resp->macUEStatusInfo[0].macUeDLLogChStatus[i].lcId);
		if(str_count < MAC_STATUS_ARR_SIZE)
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s[%d].%s=%d","macUeDLLogChStatus",i,"lcPriority",
					p_mac_ue_status_resp->macUEStatusInfo[0].macUeDLLogChStatus[i].lcPriority);
		if(str_count < MAC_STATUS_ARR_SIZE)
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s[%d].%s=%d","macUeDLLogChStatus",i,"queueLoad",
					p_mac_ue_status_resp->macUEStatusInfo[0].macUeDLLogChStatus[i].queueLoad);
		if(str_count < MAC_STATUS_ARR_SIZE)
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s[%d].%s=%d","macUeULLogChStatus",i,"lcId",
					p_mac_ue_status_resp->macUEStatusInfo[0].macUeULLogChStatus[i].lcId);
		if(str_count < MAC_STATUS_ARR_SIZE)
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s[%d].%s=%d","macUeULLogChStatus",i,"lcgId",
					p_mac_ue_status_resp->macUEStatusInfo[0].macUeULLogChStatus[i].lcgId);
		if(str_count < MAC_STATUS_ARR_SIZE)
			oam_sprintf((char *)(char *)temp_arr[str_count++],"%s[%d].%s=%d","macUeULLogChStatus",i,"bsrNetOfLCG",
					p_mac_ue_status_resp->macUEStatusInfo[0].macUeULLogChStatus[i].bsrNetOfLCG);
	}

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","tpcPUCCHRNTI",
				p_mac_ue_status_resp->macUEStatusInfo[0].tpcPUCCHRNTI);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","tpcPUSCHRNTI",
				p_mac_ue_status_resp->macUEStatusInfo[0].tpcPUSCHRNTI);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","absolutePowerCorrectionPUSCH",
				p_mac_ue_status_resp->macUEStatusInfo[0].absolutePowerCorrectionPUSCH);

	if(str_count < MAC_STATUS_ARR_SIZE)
		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","absolutePowerCorrectionPUCCH",
				p_mac_ue_status_resp->macUEStatusInfo[0].absolutePowerCorrectionPUCCH);

	/* KlockWork_Fix */ 
	/* CSR_69212_FIX_START */
	oam_construct_n_send_gpv_response((UInt8 *)"UeStatus.MAC",temp_arr,str_count,
			trans_id,p_error_code,dst_id,OAM_ZERO ,OAM_ZERO);
	/* CSR_69212_FIX_END */

    /* SPR 21028 Fix Start */
	for (loop_counter = OAM_ZERO; loop_counter < MAC_STATUS_ARR_SIZE; loop_counter++)
	{
        oam_mem_free(temp_arr[loop_counter],p_error_code);
    }
    /* SPR 21028 Fix End */

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_mac_send_mac_ue_sinr_ta_req
 * Description    : This function sends ue status request to MAC
 * Inputs         : ueIndex        :Index of the UE
 *                : src_module_id  :Source module identifier
 *                : transaction_id :transaction identifier
 * Outputs        : p_error_code   :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_send_mac_ue_sinr_ta_req
(
 oam_counter_t    ueIndex,
 oam_counter_t    src_module_id,
 oam_counter_t    transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	p_msg  = OAM_NULL;
	UInt8 *p_mac_msg;
	p_mac_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	/* Get API Length */
	msg_length += sizeof(oam_counter_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_UE_SINR_TA_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_UE_SINR_TA_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Incrementing the pointer to point to the structure
	   oam_mac_init_req_params_t */
	p_mac_msg = p_mac_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_memcpy((void *)p_mac_msg,&ueIndex,sizeof(oam_counter_t));

	/* Send message to MAC*/
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_UE_SINR_TA_REQ message to MAC with error code %d",
				*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_mac_get_mac_ue_sinr_ta_resp
 * Description    : This function provide ue specific SINR and TA information
 *                  received from mac
 * Inputs         : p_api_buf :pointer to MAC get mac ue sinr ta response
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_mac_get_mac_ue_sinr_ta_resp
(
 void* p_api_buf,
 oam_error_code_et *p_error_code

 )
{
	/* Cov Cid 62087 Fix */
	UInt8 **temp_arr;     /* this is the array name */
	UInt16 size_x; /* this variable will be used for the first  dimension */
	UInt16 size_y; /* this variable will be used for the second dimension */
	UInt8 str_count = OAM_ZERO;
	UInt16 trans_id = OAM_ZERO;
	UInt16 dest_id = QCLI_MODULE_ID;
	oam_mac_ue_sinr_ta_resp_t *p_mac_ue_sinr_ta_resp = OAM_NULL;
	oam_counter_t   loop_counter      = OAM_ZERO;
	size_x = OAM_THOUSAND;
	size_y = OAM_TWO_THOUSAND;
	OAM_FUN_ENTRY_TRACE();

	trans_id = get_word_from_header((UInt8 *)p_api_buf + OAM_ZERO);
	p_api_buf = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_mac_ue_sinr_ta_resp = (oam_mac_ue_sinr_ta_resp_t*)p_api_buf;



	/*SPR_9022_FIX_START*/
	if(OAM_SUCCESS == p_mac_ue_sinr_ta_resp->responseCode)
	{
		/*SPR_9022_FIX_END*/
		/*  allocate storage for an array of pointer to char */
		if (OAM_FAILURE == oam_mem_alloc((size_x * sizeof(UInt16 *)),(void *)&temp_arr,p_error_code))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to temp_arr failed"
					" with Error Code:%d",
					*p_error_code);
			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;

		}
		for (loop_counter = OAM_ZERO; loop_counter < size_x; loop_counter++)
		{
			if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt16)),(void *)&temp_arr[loop_counter],p_error_code))
			{
				OAM_LOG(OAM, OAM_ERROR,
						"Memory allocation to temp_arr failed"
						" with Error Code:%d",
						*p_error_code);

				/* Coverity ID 72343 Fix Start*/
				for (loop_counter = OAM_ZERO;loop_counter <= size_x;loop_counter++)
				{
					oam_mem_free(temp_arr[loop_counter],p_error_code);
				}  
				oam_mem_free(temp_arr,p_error_code);
				/* Coverity ID 72343 Fix End*/
				return OAM_FAILURE;
			}
			oam_memset(temp_arr[loop_counter], 0x00, (size_y * sizeof(UInt16)));	
		}

		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","ueIndex",
				p_mac_ue_sinr_ta_resp->ueIndex);

		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","averageULSINR",
				p_mac_ue_sinr_ta_resp->averageULSINR);

		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","averageDLSINRCW0",
				p_mac_ue_sinr_ta_resp->averageDLSINRCW0);

		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","averageDLSINRCW1",
				p_mac_ue_sinr_ta_resp->averageDLSINRCW1);

		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","timingAdvanceValue",
				p_mac_ue_sinr_ta_resp->timingAdvanceValue);

		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","uplinkPathloss",
				p_mac_ue_sinr_ta_resp->uplinkPathloss);

		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","uplinkBler",
				p_mac_ue_sinr_ta_resp->uplinkBler);

		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","downlinkBlerCW0",
				p_mac_ue_sinr_ta_resp->downlinkBlerCW0);

		oam_sprintf((char *)(char *)temp_arr[str_count++],"%s=%d","downlinkBlerCW1",
				p_mac_ue_sinr_ta_resp->downlinkBlerCW1);


		oam_construct_n_send_gpv_response((UInt8 *)"Show.UeSINR.MAC",temp_arr,str_count,
				trans_id,p_error_code,dest_id,0 ,0);

        /* OAM BCOM Code Comment Changes Start */
        for (loop_counter = OAM_ZERO; loop_counter < size_x; loop_counter++)
        {
            oam_mem_free(temp_arr[loop_counter],p_error_code);
        }
		oam_mem_free(temp_arr, p_error_code);
        /* OAM BCOM Code Comment Changes End */
	}
	/*SPR_9022_FIX_START*/
	else
	{
		oam_error_code_et p_err;
		oam_construct_n_send_get_resp_to_usr(OAM_RESULT_FAILURE,
				OAM_ERR_INTERNAL_ERROR,
				OAM_VERSION, OAM_MODULE_ID,
				dest_id, trans_id,
				OAM_ZERO, OAM_ZERO,
				"Failure From Mac", &p_err);
	}
	/*SPR_9022_FIX_END*/

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_send_mac_start_phy_cnf
 * Description    : This function sends start phy cnf to MAC
 * Inputs         : src_module_id  :Source module identifier
 *                : transaction_id :transaction identifier
 * Outputs        : p_error_code   :pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_send_mac_start_phy_cnf
(
 UInt16 src_module_id,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	p_msg = OAM_NULL;
	UInt8 *p_mac_msg;
	p_mac_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, MAC_STARTPHY_CNF, msg_api_length);
	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			MAC_STARTPHY_CNF,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Increment the pointer p_mac_msg*/
	/* Coverity ID 79018 Fix Start*/
	/*  p_mac_msg = p_mac_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;  */
	/* Coverity ID 79018 Fix End*/

	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending MAC_STARTPHY_CNF message to MAC with error code %d",
				*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	return OAM_SUCCESS;
}

/**************************EGTPU Start***************************************
 ***************************************************************************
 ***************************************************************************/
/*******************************************************************************
 * Function Name: oam_get_egtpu_initdb_req_len
 * Description  : This function calcultes size of structure
 *		         oam_egtpu_initdb_req_t
 * Inputs       : p_egtpu_initdb_req_t : pointer to srturcture type
 *			     oam_egtpu_initdb_req_t
 * Outputs      : NONE
 * Returns      : msg_length
 ********************************************************************************/

oam_length_t
oam_get_egtpu_initdb_req_len
(
 oam_egtpu_initdb_req_t *p_egtpu_initdb_req_t
 )
{
	OAM_FUN_ENTRY_TRACE();
	oam_length_t msg_length = OAM_ZERO;
	oam_counter_t loop_counter1 = OAM_ZERO;
	oam_counter_t loop_counter2 = OAM_ZERO;

	OAM_NULL_CHECK(OAM_NULL != p_egtpu_initdb_req_t);

	msg_length  = sizeof(p_egtpu_initdb_req_t->noRabRec.initSize);
	msg_length += sizeof(p_egtpu_initdb_req_t->noRabRec.stepSize);
	msg_length += sizeof(p_egtpu_initdb_req_t->noRabRec.maxSize);
	msg_length += sizeof(p_egtpu_initdb_req_t->noPeerRec);
    msg_length += sizeof(p_egtpu_initdb_req_t->logLevel);
	msg_length += sizeof(p_egtpu_initdb_req_t->numMyAddr4);
	msg_length += sizeof(p_egtpu_initdb_req_t->numMyAddr6);

	for(loop_counter1 = OAM_ZERO;
			loop_counter1 < p_egtpu_initdb_req_t->numMyAddr4;
			loop_counter1++)
	{
		msg_length += sizeof(p_egtpu_initdb_req_t->myAddr4.length);

		for(loop_counter2 = OAM_ZERO;
				loop_counter2 < p_egtpu_initdb_req_t->myAddr4.length;
				loop_counter2++)
		{
			msg_length += sizeof(p_egtpu_initdb_req_t->myAddr4.value[
					loop_counter2]);
		}
	}


	for(loop_counter1 = 0; loop_counter1 < p_egtpu_initdb_req_t->numMyAddr6; loop_counter1++)
	{
		msg_length += sizeof(p_egtpu_initdb_req_t->myAddr4.length);
		loop_counter2 = 0;
		for(loop_counter2 = OAM_ZERO;
				loop_counter2 < p_egtpu_initdb_req_t->myAddr6.length;
				loop_counter2++)
		{
			msg_length += sizeof(p_egtpu_initdb_req_t->myAddr4.value[  
					loop_counter2]);
		}
	}


	msg_length += sizeof(p_egtpu_initdb_req_t->echoReqTimer);
	msg_length += sizeof(p_egtpu_initdb_req_t->maxRetry);	
	/*+ Fix for HM params +*/
	msg_length += sizeof(p_egtpu_initdb_req_t->healthMonitoringTimeInterval);	
	/*- Fix for HM params -*/

/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
	msg_length += sizeof(p_egtpu_initdb_req_t->maxMspToBeBuffered);	

	msg_length += sizeof(p_egtpu_initdb_req_t->dataForwardingGuardTimerInterval);	
	msg_length += sizeof(p_egtpu_initdb_req_t->x2uDelStatusThreshold.x2uDeliveryStatusTriggerTimer);	
	msg_length += sizeof(p_egtpu_initdb_req_t->x2uDelStatusThreshold.x2uDeliveryStatusTriggerPktCnt);	
	msg_length += sizeof(p_egtpu_initdb_req_t->x2uDelStatusThreshold.x2uDeliveryStatusTriggerPktSize);	
	msg_length += sizeof(p_egtpu_initdb_req_t->isGtpuNRHeaderEnabled);	
#endif
/* OPTION3X Changes End */

	OAM_LOG(OAM,OAM_DETAILED,"Length::oam_egtpu_initdb_req_t == %d",
			msg_length);
	OAM_FUN_EXIT_TRACE();
	return msg_length;
}
/******************************************************************************
 * Function Name: oam_compose_egtpu_initdb_req
 * Description  : This function composes parameters of structure
 *		         oam_egtpu_initdb_req_t
 * Inputs       : p_egtpu_initdb_req_t : pointer to structure type
 *				 oam_egtpu_initdb_req_t
 * Outputs      : p_p_buffer   : Message buffer
 * Returns      : void
 ********************************************************************************/

oam_return_et
oam_compose_egtpu_initdb_req
(
 UInt8  **p_p_buffer,
 oam_egtpu_initdb_req_t *p_egtpu_initdb_req_t
 )
{
	oam_counter_t loop_counter1 = OAM_ZERO;
	oam_counter_t loop_counter2 = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();
	OAM_NULL_CHECK(OAM_NULL != p_p_buffer);
	OAM_NULL_CHECK(OAM_NULL != *p_p_buffer);

	/* Composing the message */
	oam_cp_pack_UInt32(*p_p_buffer, 
			&p_egtpu_initdb_req_t->noRabRec.initSize
			, "curr_size"
			);

	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->noRabRec.initSize);

	oam_cp_pack_UInt32(*p_p_buffer,
			&p_egtpu_initdb_req_t->noRabRec.stepSize
			, "step_size"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->noRabRec.stepSize);

	oam_cp_pack_UInt32(*p_p_buffer, 
			&p_egtpu_initdb_req_t->noRabRec.maxSize
			, "max_size"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->noRabRec.maxSize);

	oam_cp_pack_UInt16(*p_p_buffer, 
			&p_egtpu_initdb_req_t->noPeerRec
			, "num_peer_rec"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->noPeerRec);

	oam_cp_pack_UInt8(*p_p_buffer, 
			&p_egtpu_initdb_req_t->logLevel
			, "log_level"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->logLevel);

	oam_cp_pack_UInt8(*p_p_buffer, 
			&p_egtpu_initdb_req_t->numMyAddr4
			, "num_my_addr"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->numMyAddr4);

	oam_cp_pack_UInt8(*p_p_buffer, 
			&p_egtpu_initdb_req_t->numMyAddr6
			, "num_my_addr"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->numMyAddr6);

	/* composing oam_egtpu_my_address_t */

	for(loop_counter1 = OAM_ZERO;
			loop_counter1 < p_egtpu_initdb_req_t->numMyAddr4;
			loop_counter1++)
	{
		oam_cp_pack_UInt16(*p_p_buffer, 
				&p_egtpu_initdb_req_t->myAddr4.length
				, "length"
				);
		*p_p_buffer += sizeof(p_egtpu_initdb_req_t->myAddr4.length);

		for(loop_counter2 = OAM_ZERO;
				loop_counter2 <p_egtpu_initdb_req_t->myAddr4.length;
				loop_counter2++)
		{
			oam_cp_pack_UInt8(*p_p_buffer,&p_egtpu_initdb_req_t->myAddr4.value[loop_counter2]


					,"my_addr[]"
					);
			*p_p_buffer += sizeof(p_egtpu_initdb_req_t->myAddr4.value[loop_counter2]);
		}
	}


	/* composing oam_egtpu_my_address_t */

	for(loop_counter1 = OAM_ZERO;
			loop_counter1 < p_egtpu_initdb_req_t->numMyAddr6;
			loop_counter1++)
	{
		oam_cp_pack_UInt16(*p_p_buffer, 
				&p_egtpu_initdb_req_t->myAddr6.length
				, "length"
				);
		*p_p_buffer += sizeof(p_egtpu_initdb_req_t->myAddr6.length);

		for(loop_counter2 = OAM_ZERO;
				loop_counter2 <p_egtpu_initdb_req_t->myAddr6.length;
				loop_counter2++)
		{
			oam_cp_pack_UInt8(*p_p_buffer,&p_egtpu_initdb_req_t->myAddr6.value[loop_counter2]

					,"my_addr[]"
					);
			*p_p_buffer += sizeof(p_egtpu_initdb_req_t->myAddr6.value[loop_counter2]);
		}
	}
	/* SPR 10597 Changes Start */
	oam_cp_pack_UInt32(*p_p_buffer, 
			&p_egtpu_initdb_req_t->echoReqTimer
			, "echoReqTimer"
			);   
	/* SPR 10597 Changes End */
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->echoReqTimer);

	oam_cp_pack_UInt8(*p_p_buffer, 
			&p_egtpu_initdb_req_t->maxRetry
			, "maxRetry"
			);   
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->maxRetry);
	/*+ Fix for HM params +*/
	oam_cp_pack_UInt16(*p_p_buffer,
			&p_egtpu_initdb_req_t->healthMonitoringTimeInterval
			, "healthMonitoringTimeInterval"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->healthMonitoringTimeInterval);
	/*- Fix for HM params -*/

/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
    /* Filling  maxMspToBeBuffered not in API but in structure */
	oam_cp_pack_UInt8(*p_p_buffer,
			&p_egtpu_initdb_req_t->maxMspToBeBuffered
			, "maxMspToBeBuffered"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->maxMspToBeBuffered);

    /* Filling  dataForwardingGuardTimerInterval */
	oam_cp_pack_UInt16(*p_p_buffer,
			&p_egtpu_initdb_req_t->dataForwardingGuardTimerInterval
			, "dataForwardingGuardTimerInterval"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->dataForwardingGuardTimerInterval);

    /* Filling  x2uDeliveryStatusTriggerTimer */
	oam_cp_pack_UInt16(*p_p_buffer,
			&p_egtpu_initdb_req_t->x2uDelStatusThreshold.x2uDeliveryStatusTriggerTimer
			, "x2uDeliveryStatusTriggerTimer"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->x2uDelStatusThreshold.x2uDeliveryStatusTriggerTimer);

    /* Filling  x2uDeliveryStatusTriggerPktCnt */
	oam_cp_pack_UInt16(*p_p_buffer,
			&p_egtpu_initdb_req_t->x2uDelStatusThreshold.x2uDeliveryStatusTriggerPktCnt
			, "x2uDeliveryStatusTriggerPktCnt"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->x2uDelStatusThreshold.x2uDeliveryStatusTriggerPktCnt);

    /* Filling  x2uDeliveryStatusTriggerPktSize */
	oam_cp_pack_UInt16(*p_p_buffer,
			&p_egtpu_initdb_req_t->x2uDelStatusThreshold.x2uDeliveryStatusTriggerPktSize
			, "x2uDeliveryStatusTriggerPktSize"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->x2uDelStatusThreshold.x2uDeliveryStatusTriggerPktSize);

 /* Filling  isGtpuNRHeaderEnabled */
	oam_cp_pack_UInt8(*p_p_buffer,
			&p_egtpu_initdb_req_t->isGtpuNRHeaderEnabled
			, "isGtpuNRHeaderEnabled"
			);
	*p_p_buffer += sizeof(p_egtpu_initdb_req_t->isGtpuNRHeaderEnabled);

#endif
/* OPTION3X Changes End */
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}



/******************************************************************************
 * Function Name: oam_send_egtpu_initdb_req
 * Description  : This function constructs and sends OAM_RRM_INIT_CONFIG_REQ
 *		         message
 * Inputs	   : p_egtpu_initdb_req_t :pointer to srturcture type
 *				 oam_egtpu_initdb_req_t
 *                src_module_id :Source module identifier
 *                transaction_id :Interface transaction identifier
 * Outputs      : p_err_code :Pointer to error code
 * Returns      : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_send_egtpu_initdb_req
(
 oam_egtpu_initdb_req_t  *p_egtpu_initdb_req_t,
 UInt16			  src_module_id,
 UInt16			  transaction_id,
 oam_error_code_et	       *p_err_code
 )
{

	OAM_FUN_ENTRY_TRACE();
	oam_length_t msg_length = OAM_ZERO;
	oam_length_t  msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_oam_msg = OAM_NULL;

	OAM_NULL_CHECK(OAM_NULL != p_egtpu_initdb_req_t);
	/* Get API length */
	msg_length = oam_get_egtpu_initdb_req_len(
			p_egtpu_initdb_req_t);



	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +
		LOWER_LAYER_INTERFACE_HEADER_SIZE;
	OAM_LOG(OAM,OAM_DETAILED,"EGTPU_INITDB_REQ:payload: length %d message: length %d",
			msg_length, msg_api_length);

	/* Allocate buffer */
	if (OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg, p_err_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_err_code);
		OAM_FUN_EXIT_TRACE();
		/* Not enough memory */
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_oam_msg = p_msg;


	/* Fill CSPL header */
	oam_construct_api_header(p_oam_msg, OAM_VERSION_ID, src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID, EGTPU_INITDB_REQ,msg_api_length);

	/* Fill interface header */
	p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
	OAM_LOG(OAM,OAM_DETAILED,
			"Adding interface header : p_oam_msg ");

	oam_construct_interface_api_header(p_oam_msg,
			transaction_id,
			src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,
			EGTPU_INITDB_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Fill OAM_EGTPU_INITDB_REQ message */
	p_oam_msg = p_oam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_compose_egtpu_initdb_req(&p_oam_msg, p_egtpu_initdb_req_t);

	/* Send message to */
	if(OAM_SUCCESS == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID, p_err_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Successful Sending message "
				"EGTPU_INITDB_REQ to RRM");
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in sending message "
				"EGTPU_INITDB_REQ error code %d",
				*p_err_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}




/*******************************************************************************
 *Function Name  : oam_send_egtpu_get_max_retries_req
 *Description    : This function sends get max retries request to EGTPU
 *Inputs         : p_relay_Gtpu_Get_Max_Retries_Req : Pointer to API specific information
 *               : src_module_id : Source module identifier
 *               : transaction_id : Interface transaction identifier
 *Outputs        : p_error_code : Pointer to Error Code
 *Returns        : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/
oam_return_et
oam_send_egtpu_get_max_retries_req
(
 oam_relay_Gtpu_Get_Max_Retries_Req_t *p_relay_Gtpu_Get_Max_Retries_Req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t  msg_length = OAM_ZERO;
	oam_length_t  msg_api_length = OAM_ZERO ;
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_egtpu_msg = OAM_NULL;

	/* Calculate length oam_egtpu_initdb_req_t */
	msg_length  = sizeof(oam_relay_Gtpu_Get_Max_Retries_Req_t);

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +
		LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Check for  Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_egtpu_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, OAM_VERSION_ID, src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,EGTPU_GET_MAX_RETRIES_REQ,msg_api_length);


	/* Fill interface header */
	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_egtpu_msg,
			transaction_id,
			src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,
			EGTPU_GET_MAX_RETRIES_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Fill EGTPU_INITDB_REQ message */
	p_egtpu_msg = p_egtpu_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Composing the message */


	oam_cp_pack_UInt8(p_egtpu_msg, &p_relay_Gtpu_Get_Max_Retries_Req->timerType
			, "timer_Type"
			);

	p_egtpu_msg += sizeof(p_relay_Gtpu_Get_Max_Retries_Req->timerType);


	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				,"Error in function oam_send_message to send EGTPU_GET_MAX_RETRIES_REQ : err = %d!",*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 87044 Fix Start*/
		/*code Deleted*/
		/*Coverity 87044 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/*******************************************************************************
 *Function Name  : oam_send_egtpu_set_max_retries_req
 *Description    : This function sends set max retries request to EGTPU
 *Inputs         : p_relay_Gtpu_Set_Max_Retries_Req : Pointer to API specific information
 *               : src_module_id : Source module identifier
 *               : transaction_id : Interface transaction identifier
 *Outputs        : p_error_code : pointer to error code
 *Returns        : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/
oam_return_et
oam_send_egtpu_set_max_retries_req
(
 oam_relay_Gtpu_Set_Max_Retries_Req_t *p_relay_Gtpu_Set_Max_Retries_Req,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t  msg_length = OAM_ZERO;
	oam_length_t  msg_api_length = OAM_ZERO ;
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_egtpu_msg = OAM_NULL;

	/* Calculate length oam_egtpu_initdb_req_t */
	msg_length  = sizeof(oam_relay_Gtpu_Set_Max_Retries_Req_t);

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +
		LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Check for  Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_egtpu_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, OAM_VERSION_ID, src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,EGTPU_SET_MAX_RETRIES_REQ,msg_api_length);


	/* Fill interface header */
	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_egtpu_msg,
			transaction_id,
			src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,
			EGTPU_SET_MAX_RETRIES_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Fill EGTPU_INITDB_REQ message */
	p_egtpu_msg = p_egtpu_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Composing the message */
	oam_cp_pack_UInt8(p_egtpu_msg, &p_relay_Gtpu_Set_Max_Retries_Req->timerType
			, "timer_Type"
			);

	p_egtpu_msg += sizeof(p_relay_Gtpu_Set_Max_Retries_Req->timerType);

	oam_cp_pack_U16(p_egtpu_msg,&p_relay_Gtpu_Set_Max_Retries_Req->maxNumOfRetries
			, "max_Num_Of_Retries"
		       );
	p_egtpu_msg += sizeof(p_relay_Gtpu_Set_Max_Retries_Req->maxNumOfRetries);

	/* Check for Send message to EGTPU *//*BUG_ID-261*/
	if(OAM_FAILURE  == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				, "Error in function oam_send_message to send EGTPU_SET_MAX_RETRIES_REQ: %d!",*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/*CSR 00051094 start*/
/*******************************************************************************
 * Function Name  : oam_send_egtpu_stats_req
 * Description    : This function sends set timer request to EGTPU
 * Inputs         : p_egtpu_set_timer_req : Pointer to API specific information
 *                : src_module_id : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
Returns         : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/
oam_return_et
oam_send_egtpu_stats_req(
		relayGtpuGetStatsReq *p_egtpu_stats_req,
		oam_counter_t                  src_module_id,
		oam_counter_t                  transaction_id,
		oam_error_code_et    *p_error_code
		)
{
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_egtpu_msg = OAM_NULL;
	oam_length_t msg_length,msg_api_length;
	msg_length = OAM_ZERO;
	msg_api_length = OAM_ZERO;
	/* Calculate length of oam_relay_Gtpu_Set_Timer_Req_t */
	msg_length  = sizeof(p_egtpu_stats_req->statsType);
	msg_length += sizeof(p_egtpu_stats_req->resetFlag);


	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +
		LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Check for  Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_egtpu_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, OAM_VERSION_ID, src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID, PR_GTPU_GET_STATS_REQ, msg_api_length);


	/* Fill interface header */
	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_egtpu_msg,
			transaction_id,
			src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,
			PR_GTPU_GET_STATS_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Fill EGTPU_STATS_REQ message */
	p_egtpu_msg = p_egtpu_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Composing the message */
	oam_cp_pack_UInt8(p_egtpu_msg, &p_egtpu_stats_req->statsType
			,"statsType"
			);
	p_egtpu_msg += sizeof(UInt8);

	oam_cp_pack_UInt8(p_egtpu_msg, &p_egtpu_stats_req->resetFlag
			,"resetFlag"
			);

	p_egtpu_msg += sizeof(UInt8);

	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				, "Error in function oam_send_message to send PR_GTPU_GET_STATS_REQ : %d!",*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_parse_egtpu_stats_resp
 * Description    : This function sends set timer request to EGTPU
 * Inputs         : p_getStatsResp : Pointer to API specific information
 *                : p_src: 
 *                : length_left: 
 *                : length_read: 
Returns         : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/

oam_return_et
oam_parse_egtpu_stats_resp
(
 oam_egtpu_stats_res_t *p_getStatsResp,
 U8  *p_src,
 SInt32 length_left,
 UInt32 length_read
 )
{
	length_read = 0;
	oam_memset(p_getStatsResp, 0, sizeof(relayGtpuGetStatsResp));
	UInt8 count=OAM_ZERO;
	OAM_LOG(OAM,OAM_DETAILED,"Inside EGTPU PARSE API ID received in egtpu Msg Handler!!!" );

	if ((SInt64)length_read + (UInt32)sizeof(U8) > length_left)
	{
		OAM_LOG(OAM, OAM_WARNING, "Incoming message damaged!");
		return OAM_FAILURE;
	}

	oam_cp_unpack_UInt8(&p_getStatsResp->result, (p_src + ((U8)length_read))
			,"result"
			);
	length_read += sizeof(U8);

	/* Check for correct range [H - higher boundary] */
	if ((p_getStatsResp->result > 2))
	{
		OAM_LOG(OAM, OAM_WARNING, "Parameter [p_getStatsResp->result] should be in range [%d <= 2] !", p_getStatsResp->result);
		return OAM_FAILURE;
	}



	OAM_LOG(OAM, OAM_DETAILED, "Parameter p_getStatsResp->result] value = [%d] !", p_getStatsResp->result);
    if ((SInt64)(length_read + (UInt32)sizeof(U8)) > length_left)
	{
		OAM_LOG(OAM, OAM_WARNING, "Incoming message damaged!");
		return OAM_FAILURE;
	}


	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt8(&p_getStatsResp->statsType, (p_src + ((U8)length_read))
			,"statsType"

			);
	length_read += sizeof(U8);

	OAM_LOG(OAM, OAM_DETAILED, "Parameter p_getStatsResp->stats_type] value = [%d] !", p_getStatsResp->statsType);
    if ((SInt64)(length_read + (UInt32)sizeof(U8)) > length_left)
	{
		OAM_LOG(OAM, OAM_WARNING, "Incoming message damaged!");
		return OAM_FAILURE;
	}

	oam_cp_unpack_UInt16(&p_getStatsResp->length, (p_src + ((U16)length_read))
			,"length"

			);
	length_read += sizeof(U16);
	OAM_LOG(OAM, OAM_DETAILED, "Parameter p_getStatsResp->length] value = [%d] !", p_getStatsResp->length);
    if ((SInt64)(length_read + (UInt32)sizeof(U8)) > length_left)
	{
		OAM_LOG(OAM, OAM_WARNING, "Incoming message damaged!");
		return OAM_FAILURE;
	}

	for(count=0;count<p_getStatsResp->length;count++)
	{
		oam_cp_unpack_UInt32(&p_getStatsResp->value[count], (p_src + ((U32)length_read))
				,"value"

				);
		length_read += sizeof(U32);
	}


	return(OAM_SUCCESS);

}
char egtpu_stats_proto_msg[EGTPU_STATS_PROTO_MSG_SIZE][MAX_EGTPU_STATS_STR_SIZE]={{"EGTPU_STAT_ECHO_REQ_MSG_RECEIVED"},
	{"EGTPU_STAT_ECHO_REQ_MSG_SENT"},
	{"EGTPU_STAT_ECHO_RES_MSG_RECEIVED"},
	{"EGTPU_STAT_ECHO_RES_MSG_SENT"},
	{"EGTPU_STAT_SUPP_EXT_HDR_NOTI_MSG_RCVD"},
	{"EGTPU_STAT_SUPP_EXT_HDR_NOTI_MSG_SENT"},
	{"EGTPU_STAT_ERROR_IND_MSG_RECEIVED"},
	{"EGTPU_STAT_ERROR_IND_MSG_SENT"},
	{"EGTPU_STAT_PDU_RECEIVED"},
	{"EGTPU_STAT_PDU_SENT"},
	{"EGTPU_STAT_END_MARKER_MSG_RECEIVED"},
	{"EGTPU_STAT_END_MARKER_MSG_SENT"}
};
char egtpu_stats_api[EGTPU_STATS_API_SIZE][MAX_EGTPU_STATS_STR_SIZE]={{"EGTPU_STATS_API_UNITDATA_IND"},
	{"EGTPU_STATS_API_DATA_REQ"},
	{"EGTPU_STATS_API_RELOC_DATA_REQ"},
	{"EGTPU_STATS_API_SETUP_SAP_REQ"},
	{"EGTPU_STATS_API_RELEASE_SAP_REQ"},
	{"EGTPU_STATS_API_INITDB_REQ"},
	{"EGTPU_STATS_API_SET_TIMER_REQ"},
	{"EGTPU_STATS_API_GET_TIMER_REQ"},
	{"EGTPU_STATS_API_GET_MAX_RETRIES_REQ"},
	{"EGTPU_STATS_API_SET_MAX_RETRIES_REQ"},
	{"EGTPU_STATS_API_GET_STATS_REQ"},
	{"EGTPU_STATS_API_GET_INIT_STATS_REQ"},
	{"EGTPU_STATS_API_INIT_TRACE_LEVEL_REQ"},
	{"EGTPU_STATS_API_SET_TRACE_LEVEL_REQ"},
	{"EGTPU_STATS_API_GET_TRACE_LEVEL_REQ"},
	{"EGTPU_STATS_API_GET_CTXT_INFO_REQ"},
	{"EGTPU_STATS_API_UDP_UNITDATA_REQ"},
	{"EGTPU_STATS_API_RELAY_DATA_IND"},
	{"EGTPU_STATS_API_CP_SETUP_SAP_CNF"},
	{"EGTPU_STATS_API_CP_RELEASE_SAP_CNF"},
	{"EGTPU_STATS_API_CP_ERROR_IND"},
	{"EGTPU_STATS_API_CP_PATH_FAILURE_IND"},
	{"EGTPU_STATS_API_CP_END_MARKER_IND"},
	{"EGTPU_STATS_API_LM_INITDB_CNF"},
	{"EGTPU_STATS_API_LM_SET_TIMER_CNF"},
	{"EGTPU_STATS_API_LM_GET_TIMER_CNF"},
	{"EGTPU_STATS_API_LM_GET_MAX_RETRIES_CNF"},
	{"EGTPU_STATS_API_LM_SET_MAX_RETRIES_CNF"},
	{"EGTPU_STATS_API_LM_GET_STATS_CNF"},
	{"EGTPU_STATS_API_LM_GET_INIT_STATS_CNF"},
	{"EGTPU_STATS_API_LM_INIT_TRACE_LEVEL_CNF"},
	{"EGTPU_STATS_API_LM_SET_TRACE_LEVEL_CNF"},
	{"EGTPU_STATS_API_LM_GET_TRACE_LEVEL_CNF"},
	{"EGTPU_STATS_API_LM_GET_CTXT_INFO_CNF"},
	{"EGTPU_STATS_API_LM_ALARM_NOTIFICATION"},
	{"EGTPU_STATS_API_MODIFY_REORDERING_REQ"},
	{"EGTPU_STATS_API_LM_MODIFY_REORDERING_CNF"},
	{"EGTPU_STATS_API_CP_PATH_SUCCESS_IND"}
};

char egtpu_stats_errors[EGTPU_STATS_ERRORS_SIZE][MAX_EGTPU_STATS_STR_SIZE]={{"EGTPU_STAT_STATIC_MEMGET_FAIL"},
	{"EGTPU_STAT_DYNAMIC_MEMGET_FAIL"},
	{"EGTPU_STAT_HASH_CTXT_MEMGET_FAIL"},
	{"EGTPU_STAT_INSERT_DB_FAIL"},
	{"EGTPU_STAT_INSERT_HASH_TBL_FAILURE"},
	{"EGTPU_STAT_KEY_NOT_PRESENT"},
	{"EGTPU_STAT_SEND_FAIL"},
	{"EGTPU_STAT_START_TIMER_FAIL"},
	{"EGTPU_STAT_PDU_DISCARDED"},
	{"EGTPU_STAT_TIMER_CTXT_MEMGET_FAIL"}
};

char egtpu_stats_internal[EGTPU_STATS_INTERNAL_SIZE][MAX_EGTPU_STATS_STR_SIZE]={{"EGTPU_STAT_ECHO_TIMER_EXPIRY"},
};

char egtpu_stats_err_proto_msg[EGTPU_STATS_ERR_PROTO_MSG_SIZE][MAX_EGTPU_STATS_STR_SIZE]={{"EGTPU_STAT_ERR_GTP_MESSAGE_TOO_SHORT"},
	{"EGTPU_STAT_ERR_UNKNOWN_GTP_SIGNALLING_MESSAGE"},
	{"EGTPU_STAT_ERR_UNEXPECTED_GTP_SIGNALLING_MESSAGE"},
};


/*******************************************************************************
 * Function Name  : oam_handle_egtpu_stats_resp
 * Description    : This function processes the resp from EGTPU   
 * Inputs         : p_egtpu_set_timer_req : Pointer to API specific information
 *                : src_module_id : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
Returns         : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/

oam_return_et
oam_handle_egtpu_stats_resp
(
 /* SPR 17777 fix code removed */
 UInt16  trans_id,
 UInt16 dest_id,
 oam_egtpu_stats_res_t *p_getStatsResp,
 oam_error_code_et *p_err
 )
{
	UInt8 loop_counter=OAM_ZERO;
	UInt8 **temp_arr;     /* this is the array name */
	UInt8 size_x; /* this variable will be used for the first  dimension */
	UInt16 size_y; /* this variable will be used for the second dimension */
	/* suppose we want an array of char: a[4][100] */
	size_x = 39;
	size_y = 256;
    /* Coverity CID 72342, 88124 Fix Start */
    oam_return_et retVal = OAM_SUCCESS;
    /* Coverity CID 72342, 88124 Fix End */

	OAM_FUN_ENTRY_TRACE();
	/*  allocate storage for an array of pointer to char */
	if (OAM_FAILURE == oam_mem_alloc((size_x * sizeof(UInt8 *)),(void *)&temp_arr,p_err))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to temp_arr failed"
				" with Error Code:%d",
				*p_err);

        /* Coverity 88124 Fix Start */
        retVal = OAM_FAILURE;
		OAM_FUN_EXIT_TRACE();
        return retVal;
        /* Coverity 88124 Fix End */

	}

	/* for each pocharer, allocate storage for an array of chars */
	for (loop_counter = OAM_ZERO; loop_counter < size_x; loop_counter++)
	{
		if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)&temp_arr[loop_counter],p_err))
		{
			OAM_LOG(OAM, OAM_ERROR,
					"Memory allocation to temp_arr failed"
					" with Error Code:%d",
					*p_err);
			/* Coverity ID 72342 Fix Start*/
			for (loop_counter = OAM_ZERO;loop_counter <= size_x;loop_counter++)
			{
				oam_mem_free(temp_arr[loop_counter],p_err);
			}  
			oam_mem_free(temp_arr,p_err);
			/* Coverity ID 72342 Fix End*/

            /* Coverity 88124 Fix Start */
            retVal = OAM_FAILURE;
			OAM_FUN_EXIT_TRACE();
            return retVal;
            /* Coverity 88124 Fix End */

		}
		oam_memset(temp_arr[loop_counter], 0x00, (size_y * sizeof(UInt8)));
	}
	if (p_getStatsResp->statsType==OAM_ZERO)
	{
		for(loop_counter=OAM_ZERO;loop_counter<EGTPU_STATS_PROTO_MSG_SIZE;loop_counter++)
		{
			oam_sprintf((char *)temp_arr[loop_counter],"%s=%d",&egtpu_stats_proto_msg[loop_counter],
					p_getStatsResp->value[loop_counter]);
			OAM_LOG(OAM,OAM_DETAILED,"EGTPU STATS received %s = %d",egtpu_stats_proto_msg[loop_counter], p_getStatsResp->value[loop_counter]);
		}
		oam_construct_n_send_gpv_response((UInt8 *)"Show.Stats.L2.EGTPU",temp_arr,p_getStatsResp->length,
				trans_id,p_err,dest_id,OAM_ZERO,OAM_ZERO);

	}else if (p_getStatsResp->statsType==OAM_ONE)
	{
		for(loop_counter=OAM_ZERO;loop_counter<EGTPU_STATS_API_SIZE;loop_counter++)
		{
			oam_sprintf((char *)temp_arr[loop_counter],"%s=%d",egtpu_stats_api[loop_counter],
					p_getStatsResp->value[loop_counter]);
			OAM_LOG(OAM,OAM_DETAILED,"EGTPU STATS received %s = %d",egtpu_stats_api[loop_counter], p_getStatsResp->value[loop_counter]);
		}
		oam_construct_n_send_gpv_response((UInt8 *)"Show.Stats.L2.EGTPU",temp_arr,p_getStatsResp->length,
				trans_id,p_err,dest_id,OAM_ZERO,OAM_ZERO);

	}else if (p_getStatsResp->statsType==OAM_TWO)
	{
		for(loop_counter=OAM_ZERO;loop_counter<EGTPU_STATS_ERRORS_SIZE;loop_counter++)
		{
			oam_sprintf((char *)temp_arr[loop_counter],"%s=%d",egtpu_stats_errors[loop_counter],
					p_getStatsResp->value[loop_counter]);
			OAM_LOG(OAM,OAM_DETAILED,"EGTPU STATS received %s = %d",egtpu_stats_errors[loop_counter], p_getStatsResp->value[loop_counter]);
		}
		oam_construct_n_send_gpv_response((UInt8 *)"Show.Stats.L2.EGTPU",temp_arr,p_getStatsResp->length,
				trans_id,p_err,dest_id,OAM_ZERO,OAM_ZERO);

	}else if (p_getStatsResp->statsType==OAM_THREE)
	{
		for(loop_counter=OAM_ZERO;loop_counter<EGTPU_STATS_INTERNAL_SIZE;loop_counter++)
		{
			oam_sprintf((char *)temp_arr[loop_counter],"%s=%d",egtpu_stats_internal[loop_counter],
					p_getStatsResp->value[loop_counter]);
			OAM_LOG(OAM,OAM_DETAILED,"EGTPU STATS received %s = %d",egtpu_stats_internal[loop_counter], p_getStatsResp->value[loop_counter]);
		}
		oam_construct_n_send_gpv_response((UInt8 *)"Show.Stats.L2.EGTPU",temp_arr,p_getStatsResp->length,
				trans_id,p_err,dest_id,OAM_ZERO,OAM_ZERO);

	}
	else if (p_getStatsResp->statsType==OAM_FOUR)
	{
		for(loop_counter=OAM_ZERO;loop_counter<EGTPU_STATS_ERR_PROTO_MSG_SIZE;loop_counter++)
		{
			oam_sprintf((char *)temp_arr[loop_counter],"%s=%d",egtpu_stats_err_proto_msg[loop_counter],
					p_getStatsResp->value[loop_counter]);
			OAM_LOG(OAM,OAM_DETAILED,"EGTPU STATS received %s = %d",egtpu_stats_err_proto_msg[loop_counter], p_getStatsResp->value[loop_counter]);
		}
		oam_construct_n_send_gpv_response((UInt8 *)"Show.Stats.L2.EGTPU",temp_arr,p_getStatsResp->length,
				trans_id,p_err,dest_id,OAM_ZERO,OAM_ZERO);

	}else
	{
		oam_construct_n_send_get_resp_to_usr(
				OAM_RESULT_FAILURE,
				OAM_ERR_INTERNAL_ERROR,
				OAM_VERSION,
				OAM_MODULE_ID,dest_id,
				trans_id,
				OAM_ZERO,OAM_ZERO,"EGTPU LAYER FAILURE RESPONSE",
				p_err
				);
    /* Coverity CID 72342 Fix Start */    
	    retVal = OAM_FAILURE;	
	}
    for (loop_counter = OAM_ZERO; loop_counter < size_x; loop_counter++)
    {
        /*coverity_87033_fix_start*/
        oam_mem_free(temp_arr[loop_counter], p_err);
        /*coverity_87033_fix_end*/
    }
	oam_mem_free(temp_arr, p_err);
	OAM_FUN_EXIT_TRACE();
	return retVal;
    /* Coverity CID 72342 Fix End */
}

/*CSR 00051094 End*/


/*******************************************************************************
 * Function Name  : oam_send_egtpu_set_timer_req
 * Description    : This function sends set timer request to EGTPU
 * Inputs         : p_egtpu_set_timer_req : Pointer to API specific information
 *                : src_module_id : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
Returns         : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/
oam_return_et
oam_send_egtpu_set_timer_req(
		oam_relay_Gtpu_Set_Timer_Req_t *p_egtpu_set_timer_req,
		oam_counter_t                  src_module_id,
		oam_counter_t                  transaction_id,
		oam_error_code_et    *p_error_code
		)
{
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_egtpu_msg = OAM_NULL;
	oam_length_t msg_length,msg_api_length;
	msg_length = OAM_ZERO;
	msg_api_length = OAM_ZERO;
	/* Calculate length of oam_relay_Gtpu_Set_Timer_Req_t */
	msg_length  = sizeof(p_egtpu_set_timer_req->timerType);
	msg_length += sizeof(p_egtpu_set_timer_req->timerDuration);


	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +
		LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Check for  Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_egtpu_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, OAM_VERSION_ID, src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID, EGTPU_SET_TIMER_REQ, msg_api_length);


	/* Fill interface header */
	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_egtpu_msg,
			transaction_id,
			src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,
			EGTPU_SET_TIMER_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Fill EGTPU_INITDB_REQ message */
	p_egtpu_msg = p_egtpu_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Composing the message */
	oam_cp_pack_UInt8(p_egtpu_msg, &p_egtpu_set_timer_req->timerType
			,"timer_type"
			);
	p_egtpu_msg += sizeof(UInt8);

	oam_cp_pack_U32(p_egtpu_msg, &p_egtpu_set_timer_req->timerDuration
			,"timer_duration"
		       );

	p_egtpu_msg += sizeof(U32);

	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				, "Error in function oam_send_message to send EGTPU_SET_TIMER_REQ: %d!",*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_set_trace_level_req
 * Description    : This function sends set trace level request 
 * Inputs         : p_egtpu_set_trace_level_req : Pointer to API specific information
 *                : src_module_id : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
Returns         : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/

oam_return_et
oam_set_trace_level_req(
		oam_relay_Set_Trace_Lvl_Req_t *p_egtpu_set_trace_level_req,
		oam_counter_t                  src_module_id,
		oam_counter_t                  transaction_id,
		oam_error_code_et    *p_error_code
		)
{
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_egtpu_msg = OAM_NULL;
	oam_length_t msg_length,msg_api_length;
	msg_length = OAM_ZERO;
	msg_api_length = OAM_ZERO;
	/* Calculate length of oam_relay_Gtpu_Set_Timer_Req_t */
	msg_length  = sizeof(p_egtpu_set_trace_level_req->traceFlag);
	msg_length += sizeof(p_egtpu_set_trace_level_req->traceLevel);


	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +
		LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Check for  Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_egtpu_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, OAM_VERSION_ID, src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID, LM_SET_TRACE_LEVEL_REQ, msg_api_length);


	/* Fill interface header */
	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_egtpu_msg,
			transaction_id,
			src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,
			LM_SET_TRACE_LEVEL_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Fill EGTPU_INITDB_REQ message */
	p_egtpu_msg = p_egtpu_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Composing the message */



	oam_cp_pack_UInt8(p_egtpu_msg, &p_egtpu_set_trace_level_req->traceFlag
			,"trace_flag"
			);
	p_egtpu_msg += sizeof(UInt8);

	oam_cp_pack_UInt8(p_egtpu_msg, &p_egtpu_set_trace_level_req->traceLevel
			,"trace_level"
			);

	p_egtpu_msg += sizeof(UInt8);

	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				,"Error in function oam_send_message : %d!",*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 87046 Fix Start*/
		/*code Deleted*/
		/*Coverity 87046 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();

		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_egtpu_get_trace_level_req
 * Description    : This function is used to get trace level of egtpu
 * Inputs         : p_egtpu_get_trace_level_req : Pointer to structure type
 *                  oam_relay_Get_Trace_Lvl_Req_t
src_module_id : Source Module Identifier
 *                  transaction_id : Interface Transaction Identifier
 * Outputs        : p_error_code : Pointer to Error Code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_egtpu_get_trace_level_req(
		oam_relay_Get_Trace_Lvl_Req_t *p_egtpu_get_trace_level_req,
		oam_counter_t                 src_module_id,
		oam_counter_t                 transaction_id,
		oam_error_code_et *p_error_code
		)
{
	oam_counter_t msg_length = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg, *p_egtpu_msg;
	/* Get API length */
	msg_length  = sizeof(oam_relay_Get_Trace_Lvl_Req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		/* Not Enough Memory */
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_egtpu_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, transaction_id, src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID, LM_GET_TRACE_LEVEL_REQ, msg_length);

	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;
	/* Fill interface header */
	oam_construct_interface_api_header(p_egtpu_msg,
			transaction_id,
			src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,
			LM_GET_TRACE_LEVEL_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);
	p_egtpu_msg = p_egtpu_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	if(OAM_NULL == p_egtpu_get_trace_level_req)
	{
		UInt8 trace_level = 15;
		oam_memcpy(p_egtpu_msg, &trace_level, sizeof(oam_relay_Get_Trace_Lvl_Req_t));
	}
	else
		oam_memcpy(p_egtpu_msg, p_egtpu_get_trace_level_req, sizeof(oam_relay_Get_Trace_Lvl_Req_t));

	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				, "Error in function oam_send_message : %d!",*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_send_init_trace_level_cnf
 * Description    : This function sends init trace level confirmation message
 * Inputs         : p_egtpu_init_trace_level_cnf : Pointer to structure type
 *                  oam_relay_Init_Trace_Lvl_Cnf_t
 *                  src_module_id : Source Module Identifier
 *                  transaction_id : Interface Transaction Identifier
 * Outputs        : p_error_code : Pointer to Error Code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/

oam_return_et
oam_send_init_trace_level_cnf
(
 oam_relay_Init_Trace_Lvl_Cnf_t   *p_egtpu_init_trace_level_cnf,
 oam_counter_t                    src_module_id,
 oam_counter_t                    transaction_id,
 oam_error_code_et      *p_error_code
 )
{
	oam_counter_t msg_length = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg, *p_egtpu_msg;
	/* Get API length */
	msg_length  = sizeof(oam_relay_Init_Trace_Lvl_Cnf_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		/* Not Enough Memory */
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_egtpu_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, transaction_id, src_module_id,
			EGTPU_MODULE_ID, EGTPU_LM_INIT_TRACE_LEVEL_CNF, msg_length);

	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;
	/* Fill interface header */
	oam_construct_interface_api_header(p_egtpu_msg,
			transaction_id,
			src_module_id,
			EGTPU_MODULE_ID,
			EGTPU_LM_INIT_TRACE_LEVEL_CNF,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);
	p_egtpu_msg = p_egtpu_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	oam_memcpy(p_egtpu_msg,p_egtpu_init_trace_level_cnf, sizeof(oam_relay_Init_Trace_Lvl_Cnf_t));

	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				, "Error in function oam_send_message to send EGTPU_LM_INIT_TRACE_LEVEL_CNF: %d!",*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_handle_egtpu_set_timer_cnf
 * Description    : This function handles oam_handle_egtpu_set_timer_cnf
 *                  message
 * Inputs         : p_api_buf :Pointer to api buffer
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/
oam_return_et
oam_handle_egtpu_set_timer_cnf(
		void *p_api_buf
		)
{
	void     *p_data = OAM_NULL;
	oam_relay_Gtpu_Set_Timer_Cnf_t *p_egtpu_set_timer_cnf = OAM_NULL;
	//*p_egtpu_set_timer_cnf = OAM_NULL;

	OAM_NULL_CHECK(OAM_NULL != p_api_buf); //null check

	p_data          = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	p_egtpu_set_timer_cnf = (oam_relay_Gtpu_Set_Timer_Cnf_t*)p_data;

	if (OAM_SUCCESS != p_egtpu_set_timer_cnf->result)
	{
		OAM_LOG(OAM,OAM_INFO,
				"Received EGTPU_LM_SET_TIMER_CNF Failure with Result[%d]"
				,p_egtpu_set_timer_cnf ->result);
	}
	else
	{
		OAM_LOG(OAM,OAM_INFO,
				"Received EGTPU_LM_SET_TIMER_CNF[%d]",p_egtpu_set_timer_cnf->timerType);
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_handle_egtpu_get_timer_cnf
 * Description    : This function handles oam_handle_egtpu_get_timer_cnf
 *                  message
 * Inputs         : p_api_buf :Pointer to api buffer
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/
oam_return_et
oam_handle_egtpu_get_timer_cnf(
		void *p_api_buf
		)
{

	void     *p_data = OAM_NULL;
	oam_relay_Gtpu_Get_Timer_Cnf_t *p_egtpu_get_timer_cnf = OAM_NULL;

	p_data          = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_egtpu_get_timer_cnf = (oam_relay_Gtpu_Get_Timer_Cnf_t*)p_data;
	if (OAM_SUCCESS != p_egtpu_get_timer_cnf->result)
	{
		OAM_LOG(OAM,OAM_INFO,
				"Received EGTPU_LM_GET_TIMER_CNF Failure with Result[%d]"
				,p_egtpu_get_timer_cnf->result);
	}
	else
	{
		OAM_LOG(OAM,OAM_INFO,
				"Received EGTPU_LM_GET_TIMER_CNF[%d]",p_egtpu_get_timer_cnf->timerType);
		// return OAM_SUCCESS;
	}
	return OAM_SUCCESS; //warning fix 
}

/****************************************************************************
 * Function Name  : oam_handle_egtpu_set_max_retries_entries_cnf
 * Description    : This function handles
 *                  oam_handle_egtpu_set_max_retries_entries_cnf message
 * Inputs         : p_api_buf :Pointer to api buffer
 *                  data_len :data length
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/

oam_return_et
oam_handle_egtpu_set_max_retries_entries_cnf(
		void *p_api_buf
    /*SPR 17777 fix*/
		)
{

	void     *p_data = OAM_NULL;
	oam_relay_Gtpu_Set_Max_Retries_Cnf_t *p_egtpu_set_max_retries_cnf = OAM_NULL;

	p_data          = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_egtpu_set_max_retries_cnf = (oam_relay_Gtpu_Set_Max_Retries_Cnf_t *)p_data;


	if (OAM_SUCCESS != p_egtpu_set_max_retries_cnf->result)
	{
		OAM_LOG(OAM,OAM_INFO,
				"Received EGTPU_LM_SET_MAX_RETRIES_CNF Failure with Result[%d]"
				,p_egtpu_set_max_retries_cnf->result);
	}
	else
	{
		OAM_LOG(OAM,OAM_INFO
				,"Received EGTPU_LM_SET_MAX_RETRIES_CNF timerType=[%d]"
				,p_egtpu_set_max_retries_cnf->timerType);
		//return OAM_SUCCESS;
	}

	return OAM_SUCCESS; //warning success
}


/****************************************************************************
 * Function Name  : oam_handle_egtpu_get_max_retries_entries_cnf
 * Description    : This function handles
 *                  oam_handle_egtpu_get_max_retries_entries_cnf message
 * Inputs         : p_api_buf :Pointer to api buffer
 *                  data_len :data length
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/
oam_return_et
oam_handle_egtpu_get_max_retries_entries_cnf(
		void *p_api_buf
    /*SPR 17777 fix*/
		)
{

	void     *p_data = OAM_NULL;
	// relay_Gtpu_Get_Max_Retries_Cnf_t *p_egtpu_cnf = OAM_NULL;
	relay_Gtpu_Get_Max_Retries_Cnf_t *p_egtpu_get_max_retries_cnf = OAM_NULL;

	p_data          = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_egtpu_get_max_retries_cnf = (relay_Gtpu_Get_Max_Retries_Cnf_t *)p_data;


	if (OAM_SUCCESS != p_egtpu_get_max_retries_cnf->result)
	{
		OAM_LOG(OAM,OAM_INFO,
				"Received EGTPU_LM_GET_MAX_RETRIES_CNF Failure with Result[%d]"
				,p_egtpu_get_max_retries_cnf->result);
	}
	else
	{
		OAM_LOG(OAM,OAM_INFO
				,"Received EGTPU_LM_GET_MAX_RETRIES_CNF timerType=[%d]"
				,p_egtpu_get_max_retries_cnf->timerType);
		//return OAM_SUCCESS;
	}

	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_handle_egtpu_set_trace_level_cnf
 * Description    : This function handles oam_handle_egtpu_set_trace_level_cnf
 *                  message
 * Inputs         : p_api_buf :pointer to api buffer
 *                  transaction_id :transaction identifier
 * Outputs        : p_error_code :Pointer to error code 
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/

oam_return_et
oam_handle_egtpu_set_trace_level_cnf(
		void *p_api_buf,
		oam_error_code_et *p_error_code
		)
{
	void     *p_data = OAM_NULL;
	oam_relay_Set_Trace_Lvl_Cnf_t *p_egtpu_set_trace_level_cnf = OAM_NULL;
	UInt16 trans_id = OAM_ZERO;
	UInt16 dest_id = OAM_ZERO;


	if(OAM_FAILURE == oam_process_tcb_for_layer_resp(p_api_buf,&trans_id,&dest_id))
	{
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_data          = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_egtpu_set_trace_level_cnf = (oam_relay_Set_Trace_Lvl_Cnf_t*)p_data;

	UInt16 msg_api_length = /* msg_length */ 8 +
		OAM_CSPL_HDR_SIZE +
		OAM_MGMT_INTF_HEADER_SIZE;
	UInt8 *p_egtpu_msg;
	UInt8 * p_msg;
	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		/* Not Enough Memory */
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_egtpu_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			dest_id,OAM_GET_LOG_LEVEL_GTPU_RESP, msg_api_length);

	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;

	/* Fill API header in message*/
	oam_construct_mgmt_api_hdr(p_egtpu_msg,
			trans_id,
			OAM_MODULE_ID,
			dest_id,
			OAM_SET_PARAMETER_VALUES_RESP,
			msg_api_length - OAM_CSPL_HDR_SIZE,
			OAM_ZERO);
    /*SPR 17777 fix*/

	p_egtpu_msg = p_egtpu_msg + OAM_MGMT_INTF_HEADER_SIZE;

	/*This is done because the encoding of result field is just opposite
	  to that assumed to be sent in the response message from OAM.*/
	*(p_egtpu_msg + 0) = 
		(OAM_SUCCESS == p_egtpu_set_trace_level_cnf->result)? OAM_ZERO : OAM_ONE;
	/* 2-Byte of error code, 1 reserved byte, 3 another reserver bytes
	 * and 1 error string length byte will all be zero in this case */
	oam_memset((p_egtpu_msg + 1), OAM_ZERO, 7); 

	/* No way to store the status of logging to oam_prov_req */

	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, dest_id, p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				, "Error in function oam_send_message to send OAM_SET_PARAMETER_VALUES_RESP: %d!",*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_handle_egtpu_get_trace_level_cnf
 * Description    : This function handles oam_handle_egtpu_get_trace_level_cnf
 *                  message
 * Inputs         : p_api_buf :pointer to api buffer
 *                  transaction_id :transaction identifier
 * Outputs        : p_error_code :Pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/

oam_return_et
oam_handle_egtpu_get_trace_level_cnf(
		void *p_api_buf,
    /*SPR 17777 fix*/
		oam_error_code_et *p_error_code
		)
{
	void     *p_data = OAM_NULL;
	oam_relay_Get_Trace_Lvl_Cnf_t *p_egtpu_get_trace_level_cnf = OAM_NULL;
	UInt16 trans_id = OAM_ZERO;
	UInt16 dest_id = OAM_ZERO;


	if(OAM_FAILURE == oam_process_tcb_for_layer_resp(p_api_buf,&trans_id,&dest_id))
	{
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_data          = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_egtpu_get_trace_level_cnf = (oam_relay_Get_Trace_Lvl_Cnf_t *)p_data;

	if (OAM_SUCCESS != p_egtpu_get_trace_level_cnf->result)
	{
		OAM_LOG(OAM,OAM_INFO,
				"Received EGTPU_LM_SET_TRACE_LEVEL_CNF Failure with Result[%d]"
				,p_egtpu_get_trace_level_cnf->result);
	}
	else
	{
		OAM_LOG(OAM,OAM_INFO
				,"Received EGTPU_LM_SET_TRACE_LEVEL_CNF timerType=[%d]"
				,p_egtpu_get_trace_level_cnf->traceLevel);
	}

	UInt16 msg_api_length = /* msg_length */ 8 +
		OAM_CSPL_HDR_SIZE +
		OAM_MGMT_INTF_HEADER_SIZE;
	UInt8 *p_egtpu_msg;
	UInt8 * p_msg;


	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		/* Not Enough Memory */
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_egtpu_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			dest_id,OAM_GET_LOG_LEVEL_GTPU_RESP, msg_api_length);

	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;

	/* Fill API header in message*/
	oam_construct_mgmt_api_hdr(p_egtpu_msg,
			trans_id,
			OAM_MODULE_ID,
			dest_id,
			OAM_SET_PARAMETER_VALUES_RESP,
			msg_api_length - OAM_CSPL_HDR_SIZE,
			OAM_ZERO);
    /*SPR 17777 fix*/

	p_egtpu_msg = p_egtpu_msg + OAM_MGMT_INTF_HEADER_SIZE;

	/*This is done because the encoding of result field is just opposite
	  to that assumed to be sent in the response message from OAM.*/
	*(p_egtpu_msg + 0) = 
		(OAM_SUCCESS == p_egtpu_get_trace_level_cnf->result)? OAM_ZERO : OAM_ONE;
	/* 2-Byte of error code, 1 reserved byte, 3 another reserver bytes
	 * and 1 error string length byte will all be zero in this case */
	oam_memset((p_egtpu_msg + 1), OAM_ZERO, 7); 

	/* No way to store the status of logging to oam_prov_req */


	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, dest_id,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				, "Error in function oam_send_message to send OAM_SET_PARAMETER_VALUES_RESP: %d!",*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_handle_get_egtpu_init_trace_level_cnf
 * Description    : This function handles oam_handle_get_egtpu_init_trace_level_cnf
 *                  message
 * Inputs         : p_api_buf :Pointer to api buffer
 *                  data_len :data length
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/

oam_return_et
oam_handle_get_egtpu_init_trace_level_cnf
(
 void *p_api_buf
    /*SPR 17777 fix*/
 )
{

	void     *p_data = OAM_NULL;
	oam_relay_Init_Trace_Lvl_Cnf_t *p_egtpu_get_init_trace_level_cnf;
	p_egtpu_get_init_trace_level_cnf  = OAM_NULL;

	p_data          = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_egtpu_get_init_trace_level_cnf = (oam_relay_Init_Trace_Lvl_Cnf_t *)p_data;


	if (OAM_SUCCESS != p_egtpu_get_init_trace_level_cnf->result)
	{
		OAM_LOG(OAM,OAM_INFO,
				"Received EGTPU_LM_GET_INIT_TRACE_LEVEL_CNF Failure with Result[%d]"
				,p_egtpu_get_init_trace_level_cnf->result);
	}
	return OAM_SUCCESS; //warning fix
}


/*******************************************************************************
 * Function Name  : oam_egtpu_set_log_level_req
 * Description    : This function sends set log level request 
 * Inputs         : p_egtpu_set_log_level_req : Pointer to API specific information
 *                : src_module_id : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
Returns         : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/

oam_return_et
oam_egtpu_set_log_level_req(
		oam_relay_Gtpu_Set_Log_Lvl_Req_t *p_egtpu_set_log_level_req,
		oam_counter_t                  src_module_id,
		oam_counter_t                  transaction_id,
		oam_error_code_et    *p_error_code
		)
{
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_egtpu_msg = OAM_NULL;
	oam_length_t msg_length,msg_api_length;
	msg_length = OAM_ZERO;
	msg_api_length = OAM_ZERO;
	/* Calculate length of oam_relay_Gtpu_Set_Log_Lvl_Req_t */
	msg_length  = sizeof(oam_relay_Gtpu_Set_Log_Lvl_Req_t);


	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +
		LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Check for  Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_egtpu_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, OAM_VERSION_ID, src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID, PR_GTPU_SET_LOG_LEVEL_REQ, msg_api_length);


	/* Fill interface header */
	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_egtpu_msg,
			transaction_id,
			src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,
			PR_GTPU_SET_LOG_LEVEL_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Fill EGTPU_INITDB_REQ message */
	p_egtpu_msg = p_egtpu_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Composing the message */

	oam_cp_pack_UInt32(p_egtpu_msg, &p_egtpu_set_log_level_req->logLevel
			,"log_level"
			);

	p_egtpu_msg += sizeof(UInt32);

	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				,"Error in function oam_send_message : %d!",*p_error_code);
		OAM_FUN_EXIT_TRACE();

		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_egtpu_get_log_level_req
 * Description    : This function is used to get log level of egtpu
 * Inputs         : p_egtpu_get_log_level_req : Pointer to structure type
 *                  oam_relay_Gtpu_Get_Log_Lvl_Req_t
 *                  src_module_id : Source Module Identifier
 *                  transaction_id : Interface Transaction Identifier
 * Outputs        : p_error_code : Pointer to Error Code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_egtpu_get_log_level_req(
		oam_relay_Gtpu_Get_Log_Lvl_Req_t  *p_egtpu_get_log_level_req,
		oam_counter_t                 src_module_id,
		oam_counter_t                 transaction_id,
		oam_error_code_et *p_error_code
		)
{
	oam_counter_t msg_length = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg, *p_egtpu_msg;
	/* Get API length */
	msg_length  = sizeof(oam_relay_Gtpu_Get_Log_Lvl_Req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		/* Not Enough Memory */
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_egtpu_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, transaction_id, src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID, PR_GTPU_GET_LOG_LEVEL_REQ, msg_length);

	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;
	/* Fill interface header */
	oam_construct_interface_api_header(p_egtpu_msg,
			transaction_id,
			src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,
			PR_GTPU_GET_LOG_LEVEL_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);
	p_egtpu_msg = p_egtpu_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	if(OAM_NULL == p_egtpu_get_log_level_req)
	{
		UInt32 log_level = LTE_L2_LOG_ERROR;
		oam_memcpy(p_egtpu_msg, &log_level, sizeof(oam_relay_Gtpu_Get_Log_Lvl_Req_t));
	}
	else
		oam_memcpy(p_egtpu_msg, p_egtpu_get_log_level_req, sizeof(oam_relay_Gtpu_Get_Log_Lvl_Req_t));

	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				, "Error in function oam_send_message : %d!",*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/**************************EGTPU End***************************************
 ***************************************************************************/

/**************************RLC  Start***************************************
 ****************************************************************************/

/******************************************************************************
 * Function Name  : oam_send_rlc_init_layer_req
 * Description    : This function sends init request to RLC
 * Inputs         : p_oam_rlc_init_req : Pointer to API specific information
 *                : src_module_id  : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code   : Pointer error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ********************************************************************************/
oam_return_et
oam_send_rlc_init_layer_req
(
 oam_rlc_init_layer_req_t *p_oam_rlc_init_req,
 UInt16                    src_module_id,
 UInt16                    transaction_id,
 UInt8                     cell_index,
 oam_error_code_et        *p_error_code
 )
{
	oam_length_t msg_length = OAM_ZERO;
	oam_length_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_rlc_msg = OAM_NULL;

	OAM_NULL_CHECK(OAM_NULL != p_oam_rlc_init_req);

	msg_length = sizeof(oam_rlc_init_layer_req_t);

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	OAM_LOG(OAM,OAM_DETAILED,"RLC_INIT_LAYER_REQ  length %d",msg_api_length);

	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_rlc_msg = p_msg;


	/* Fill CSPL header */
	oam_construct_api_header(p_rlc_msg, OAM_VERSION_ID, src_module_id,
			RLC_MODULE_ID, RLC_INIT_LAYER_REQ,
			msg_api_length);

	p_rlc_msg = p_rlc_msg + OAM_CSPL_HDR_SIZE;



	/* Fill interface header */
	oam_construct_interface_api_header(p_rlc_msg, transaction_id,
			src_module_id, RLC_MODULE_ID,
			RLC_INIT_LAYER_REQ,
			msg_length, cell_index);

	p_rlc_msg = p_rlc_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;


	/* Fill RLC_INIT_LAYER_REQ message */
	oam_memcpy((void *)p_rlc_msg, p_oam_rlc_init_req, sizeof(oam_rlc_init_layer_req_t));

	if(OAM_FAILURE == oam_send_message(p_msg, RLC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending RLC_INIT_LAYER_REQ message to RLC with error code %d",
				*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	return OAM_SUCCESS; 
}

/*CHANGE_1_3_1*/


/******************************************************************************
 * Function Name  : oam_send_rlc_ue_status_req
 * Description    : This function sends ue status to RLC
 * Inputs         : p_oam_rlc_ue_status_req : Pointer to API specific information
 *                : src_module_id  : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code   : Pointer error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ********************************************************************************/
oam_return_et
oam_send_rlc_ue_status_req
(
 oam_rlc_ue_status_req_t *p_oam_rlc_ue_status_req,
 UInt16                   src_module_id,
 UInt16                   transaction_id,
 oam_error_code_et       *p_error_code
 )
{
	oam_length_t msg_length = OAM_ZERO;
	oam_length_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_rlc_msg = OAM_NULL;

	OAM_NULL_CHECK(OAM_NULL != p_oam_rlc_ue_status_req);

	msg_length = sizeof(oam_rlc_ue_status_req_t);

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	OAM_LOG(OAM,OAM_DETAILED," RLC_GET_STATUS_REQ length %d",msg_api_length);

	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_rlc_msg = p_msg;


	/* Fill CSPL header */
	oam_construct_api_header(p_rlc_msg, OAM_VERSION_ID, src_module_id,
			RLC_MODULE_ID, RLC_GET_STATUS_REQ,
			msg_api_length);

	p_rlc_msg = p_rlc_msg + OAM_CSPL_HDR_SIZE;



	/* Fill interface header */
	oam_construct_interface_api_header(p_rlc_msg,
			transaction_id,
			src_module_id,
			RLC_MODULE_ID,
			RLC_GET_STATUS_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_rlc_msg = p_rlc_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;


	/* Fill RLC_GET_STATUS_REQ message */
	oam_memcpy((void *)p_rlc_msg, p_oam_rlc_ue_status_req , sizeof(oam_rlc_ue_status_req_t));

	if(OAM_FAILURE == oam_send_message(p_msg, RLC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending RLC_GET_STATUS_REQ message to RLC with error code %d",
				*p_error_code);
        /* OAM BCOM Code Comment Changes Start */
        /*Coverity 98828 Fix Start*/
        /*code Deleted*/
        /*Coverity 98828 Fix End*/
        /* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	return OAM_SUCCESS; 

}




/*CHANGE_1_3_1*/
//#endif

/******************************************************************************
 * fUnction Name  : oam_rlc_send_set_log_level_req
 * Description    : This function sends set log Level request to RLC
 * Inputs         : p_Oam_rlc_set_log_level_req : pointer to set Log Level Req structure
 *                : src_module_id : source module id
 *                : transaction_id : transcation module id
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rlc_send_set_log_level_req
(
 Oam_LogLevel_Req_t *p_Oam_rlc_set_log_level_req,
 UInt16             src_module_id,
 UInt16             transaction_id,
 oam_error_code_et  *p_error_code
 )
{
	oam_length_t msg_length = OAM_ZERO;
	oam_length_t msg_api_length = OAM_ZERO ;
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_rlc_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_length  = sizeof(p_Oam_rlc_set_log_level_req->logLevel);

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +  LOWER_LAYER_INTERFACE_HEADER_SIZE;

	OAM_LOG(OAM,OAM_DETAILED,"RLC_SET_LOG_LEVEL_REQ: length %d",msg_api_length);
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_rlc_msg = p_msg;


	oam_construct_api_header(p_rlc_msg, OAM_VERSION_ID, src_module_id,
			RLC_MODULE_ID,RLC_SET_LOG_LEVEL_REQ, msg_api_length);

	p_rlc_msg = p_rlc_msg + OAM_CSPL_HDR_SIZE;


	oam_construct_interface_api_header(p_rlc_msg,
			transaction_id,
			src_module_id,
			RLC_MODULE_ID,
			RLC_SET_LOG_LEVEL_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_rlc_msg = p_rlc_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;


	oam_memcpy(p_rlc_msg,&(p_Oam_rlc_set_log_level_req->logLevel),sizeof(p_Oam_rlc_set_log_level_req->logLevel));

	if(OAM_FAILURE == oam_send_message(p_msg, RLC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending RLC_SET_LOG_LEVEL_REQ message to RLC with error code %d",
				*p_error_code);
        /* OAM BCOM Code Comment Changes Start */
        /*Coverity 98828 Fix Start*/
        /*code Deleted*/
        /*Coverity 98828 Fix End*/
        /* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();

	return OAM_SUCCESS;


}

/****************************************************************************
 * Function Name  : oam_rlc_send_cleanup_req
 * Description    : This function sends cleanup request to RLC
 * Inputs         : src_module_id : source module id
 *                : transaction_id : transaction module id
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/

oam_return_et
oam_rlc_send_cleanup_req
(
 UInt16 src_module_id,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t msg_length = OAM_ZERO;
	oam_length_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg =OAM_NULL;
	UInt8 *p_rlc_msg =OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = OAM_CSPL_HDR_SIZE +  LOWER_LAYER_INTERFACE_HEADER_SIZE;

	if(OAM_FAILURE ==oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_rlc_msg = p_msg;


	oam_construct_api_header(p_rlc_msg, OAM_VERSION_ID, src_module_id,
			RLC_MODULE_ID, RLC_CLEANUP_LAYER_REQ, msg_api_length);

	p_rlc_msg = p_rlc_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_rlc_msg,
			transaction_id,
			src_module_id,
			RLC_MODULE_ID,
			RLC_CLEANUP_LAYER_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);


	if(OAM_FAILURE == oam_send_message(p_msg, RLC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending RLC_CLEANUP_LAYER_REQ message to RLC with error code %d",
				*p_error_code);
        /* OAM BCOM Code Comment Changes Start */
        /*Coverity 98828 Fix Start*/
        /*code Deleted*/
        /*Coverity 98828 Fix End*/
        /* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_rlc_send_get_log_level_req
 * Description    : This function sends request to RLC to get loglevel info
 * Inputs         : src_module_id : source module id
 *                : transaction_id : transaction module id
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/

oam_return_et
oam_rlc_send_get_log_level_req
(
 UInt16 src_module_id,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t msg_length = OAM_ZERO;
	oam_length_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg =OAM_NULL;
	UInt8 *p_rlc_msg =OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +  LOWER_LAYER_INTERFACE_HEADER_SIZE;
	OAM_LOG(OAM,OAM_DETAILED,"RLC_GET_LOG_LEVEL_REQ: length %d",msg_api_length);

	if(OAM_FAILURE ==oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_rlc_msg = p_msg;


	oam_construct_api_header(p_rlc_msg, OAM_VERSION_ID, src_module_id,
			RLC_MODULE_ID,RLC_GET_LOG_LEVEL_REQ, msg_api_length);

	p_rlc_msg = p_rlc_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_rlc_msg,
			transaction_id,
			src_module_id,
			RLC_MODULE_ID,
			RLC_GET_LOG_LEVEL_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);


	if(OAM_FAILURE == oam_send_message(p_msg, RLC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending RLC_GET_LOG_LEVEL_REQ message to RLC with error code %d",
				*p_error_code);
        /* OAM BCOM Code Comment Changes Start */
        /*Coverity 98828 Fix Start*/
        /*code Deleted*/
        /*Coverity 98828 Fix End*/
        /* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_rlc_send_disable_log_category_req
 * Description    : This function sends disable logcategory request to RLC
 * Inputs         : p_rlc_disable_log_category_req : Pointer to API specific information.
 *                : src_module_id  : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_rlc_send_disable_log_category_req
(
 Oam_LogCategory_Req_t *p_rlc_disable_log_category_req,
 UInt16 src_module_id,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t msg_length = OAM_ZERO;
	oam_length_t msg_api_length = OAM_ZERO ;
	UInt8 *p_msg = OAM_NULL, *p_rlc_msg = OAM_NULL;
	Oam_LogCategory_Req_t *temp_log_cat;

	OAM_FUN_ENTRY_TRACE();
	/* Get API length */

	msg_length = sizeof(p_rlc_disable_log_category_req->logCategory);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	OAM_LOG(OAM,OAM_DETAILED,"RLC_DISABLE_LOG_CATEGORY_REQ: length %d",msg_api_length);
	/* Allocate buffer */

	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();

		return OAM_FAILURE;
	}

	p_rlc_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_rlc_msg, OAM_VERSION_ID, src_module_id,
			RLC_MODULE_ID,RLC_DISABLE_LOG_CATEGORY_REQ, msg_api_length);
	/* Fill interface header */
	p_rlc_msg = p_rlc_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_rlc_msg,
			transaction_id,
			src_module_id,
			RLC_MODULE_ID,
			RLC_DISABLE_LOG_CATEGORY_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);


	/* Fill RLC_ENABLE_LOG_CATEGORY_REQ message */
	p_rlc_msg = p_rlc_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	temp_log_cat =(Oam_LogCategory_Req_t *)p_rlc_msg;
	/* Pack the elements */
	temp_log_cat->logCategory = p_rlc_disable_log_category_req->logCategory;


	/* Send message to */
	if(OAM_FAILURE == oam_send_message(p_msg, RLC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending RLC_DISABLE_LOG_CATEGORY_REQ message to RLC with error code %d",
				*p_error_code);
        /* OAM BCOM Code Comment Changes Start */
        /*Coverity 98828 Fix Start*/
        /*code Deleted*/
        /*Coverity 98828 Fix End*/
        /* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();




	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_rlc_send_set_log_category_req
 * Description    : This function sends logcategory request to RLC
 * Inputs         : p_rlc_set_log_category_req : Pointer to API specific information
 *                : src_module_id : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_rlc_send_set_log_category_req
(
 Oam_LogCategory_Req_t *p_rlc_set_log_category_req,
 UInt16 src_module_id,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t msg_length = OAM_ZERO;
	oam_length_t msg_api_length = OAM_ZERO;

	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_rlc_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	Oam_LogCategory_Req_t *temp_log_cat;
	/* Get API length */

	msg_length  = sizeof(p_rlc_set_log_category_req->logCategory);

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +  LOWER_LAYER_INTERFACE_HEADER_SIZE;
	OAM_LOG(OAM,OAM_DETAILED,"RLC_ENABLE_LOG_CATEGORY_REQ : length %d",msg_api_length);

	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_rlc_msg = p_msg;

	OAM_LOG(OAM,OAM_DETAILED," p_rlc_msg After Assignment(Before oam_construct_api_header):  %p", p_rlc_msg);
	/* Fill CSPL header */
	oam_construct_api_header(p_rlc_msg, OAM_VERSION_ID, src_module_id,
			RLC_MODULE_ID,RLC_ENABLE_LOG_CATEGORY_REQ, msg_api_length);

	/* Fill interface header */
	p_rlc_msg = p_rlc_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_rlc_msg,
			transaction_id,
			src_module_id,
			RLC_MODULE_ID,
			RLC_ENABLE_LOG_CATEGORY_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Fill RLC_ENABLE_LOG_CATEGORY_REQ message */
	p_rlc_msg = p_rlc_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	temp_log_cat = (Oam_LogCategory_Req_t *)p_rlc_msg;

	/* Pack the elements */
	temp_log_cat->logCategory = p_rlc_set_log_category_req->logCategory;

	/* Send message to RLC */
	if(OAM_FAILURE == oam_send_message(p_msg, RLC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending RLC_ENABLE_LOG_CATEGORY_REQ message to RLC with error code %d",
				*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/******************************************************************************
 * Function Name  : oam_rlc_send_get_log_category_req
 * Description    : This function sends request to RLC to get logcategory info
 * Inputs         : src_module_id : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rlc_send_get_log_category_req
(
 UInt16 src_module_id,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t msg_length = OAM_ZERO;
	oam_length_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_rlc_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE +  LOWER_LAYER_INTERFACE_HEADER_SIZE;

	OAM_LOG(OAM,OAM_DETAILED,"RLC_GET_LOG_CATEGORY_REQ: Length %d",msg_api_length);

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_rlc_msg = p_msg;


	/* Fill CSPL header */
	oam_construct_api_header(p_rlc_msg, OAM_VERSION_ID, src_module_id,
			RLC_MODULE_ID,RLC_GET_LOG_CATEGORY_REQ, msg_api_length);

	/* Fill interface header */
	p_rlc_msg = p_rlc_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_rlc_msg,
			transaction_id,
			src_module_id,
			RLC_MODULE_ID,
			RLC_GET_LOG_CATEGORY_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);


	/* Send message to RLC */
	if(OAM_FAILURE == oam_send_message(p_msg, RLC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending RLC_GET_LOG_CATEGORY_REQ message to RLC with error code %d",
				*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
                /* OAM BCOM Code Comment Changes End */
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_send_get_rlc_stats_req
 * Description    : This function sends get stats uest to RLC
 * Inputs         : src_module_id : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputr        : p_error_code : pointer to error_code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_send_get_rlc_stats_req
(
 UInt16            src_module_id,
 UInt16            transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_rlc_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	OAM_LOG(OAM,OAM_DETAILED,"RLC_GET_STATS_REQ: length %d",msg_api_length);

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	p_rlc_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_rlc_msg, OAM_VERSION_ID, src_module_id,
			RLC_MODULE_ID, RLC_GET_STATS_REQ , msg_api_length);


	/* Fill interface header */
	p_rlc_msg = p_rlc_msg + OAM_CSPL_HDR_SIZE;

	oam_construct_interface_api_header(p_rlc_msg,
			transaction_id,
			src_module_id,
			RLC_MODULE_ID,
			RLC_GET_STATS_REQ,
			0,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* check for Send message */

	if(OAM_FAILURE == oam_send_message(p_msg, RLC_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending RLC_GET_STATS_REQ message to RLC with error code %d",
				*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_handle_rlc_init_layer_cnf
 * Description    : This function Handles the init layer cnf from RLC
 * Inputs         : p_api_buf : pointer to API Message
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_handle_rlc_init_layer_cnf
(
 void*              p_api_buf
 )
{
	void *p_msg = OAM_NULL;
	Oam_Rlc_Cnf_t *p_rlc_init_layer_cnf = OAM_NULL;
	oam_return_et retVal = OAM_SUCCESS;

	OAM_FUN_ENTRY_TRACE();


	OAM_NULL_CHECK(OAM_NULL != p_api_buf); 

	p_msg = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;


	p_rlc_init_layer_cnf = (Oam_Rlc_Cnf_t *)p_msg;

	/* handlig of "Recieved Response */
	if(OAM_RLC_SUCCESS == (p_rlc_init_layer_cnf->responseCode))
	{
		OAM_LOG(OAM, OAM_INFO,
				"Success response received in RLC_INIT_LAYER_CNF");
	}
	else {
		OAM_LOG(OAM, OAM_INFO,
				"Failure response received in RLC_INIT_LAYER_CNF");
		retVal = OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return retVal;
}

/****************************************************************************
 * Function Name  : oam_rlc_get_logcategory_resp
 * Description    : This function provide current loglevel of rlc module to oam
 * Inputs         : p_api_buf : pointer to  API Buffer
 *                : msg_len : Total length of API Message
 *                  transaction_id :transaction identifier
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_rlc_get_logcategory_resp
(
 void* p_api_buf,
 UInt16 msg_len,
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t loop_counter = OAM_ZERO;
	UInt8* logcat_str = OAM_NULL;
	UInt8* p_msg   = OAM_NULL;
	UInt64 log_category = OAM_ZERO ;
	Oam_LogCategory_Req_t *p_oam_get_logcategory_rlc;

	void *p_recv_msg = OAM_NULL;

	UInt8 *p_rlc_msg = OAM_NULL;


	UInt16 msg_api_length = OAM_ZERO;

	OAM_FUN_ENTRY_TRACE();

	p_recv_msg = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	OAM_LOG(OAM,OAM_DETAILED,"ptr of p_recv_msg [After increment of 10[LOWER_LAYER_INTERFACE_HEADER_SIZE]= %p",p_recv_msg);

	p_oam_get_logcategory_rlc =(Oam_LogCategory_Req_t *)p_recv_msg;

	/* check for logCategory */
	if(p_oam_get_logcategory_rlc->logCategory == OAM_ZERO)
	{
		OAM_LOG(OAM,OAM_INFO," No logCategory is enabled as Received from RLC");
	}
	else
	{
		for(loop_counter = RLC_MIN_LOG_CAT; oam_logcategory_table[loop_counter].string_id; loop_counter++)
		{
			OAM_LOG(OAM,OAM_DETAILED,"oam_logcategory_table[%u].string_val=%llu", loop_counter, oam_logcategory_table[loop_counter].string_val);

			log_category = oam_logcategory_table[loop_counter].string_val &
				p_oam_get_logcategory_rlc->logCategory;

			/* Check for log_category to to string val */
			if (log_category == oam_logcategory_table[loop_counter].string_val)
			{
				OAM_LOG(OAM,OAM_DETAILED,"string_id = %s",oam_logcategory_table[loop_counter].string_id);

				if(OAM_FAILURE == oam_mem_alloc(sizeof(UInt8)*oam_strlen((const char *)(oam_logcategory_table[loop_counter].string_id)),(void *)&logcat_str,p_error_code))
				{
                    OAM_LOG(OAM, OAM_ERROR,
                            "Memory allocation to logcat_str failed"
                            " with Error Code:%d",
							*p_error_code);
					OAM_FUN_EXIT_TRACE();
					return OAM_FAILURE;
				}

				oam_strcpy((char *)logcat_str,(const char *)(oam_logcategory_table[loop_counter].string_id));
				OAM_LOG(OAM,OAM_DETAILED,"logcat_str[Test] = %s",logcat_str);
				/* Check for Null */
				if(  (RLC_MIN_LOG_CAT <= loop_counter) && (loop_counter <= RLC_MAX_LOG_CAT)  )
				{
					OAM_LOG(OAM,OAM_DETAILED,"RLC LOG CATEGORY: [ %s ]",logcat_str);
				}
				oam_mem_free(logcat_str, p_error_code);    
				logcat_str = OAM_NULL;
			}
			log_category=0;
		}/*end for */
	}/*end else */

	msg_api_length = msg_len + OAM_CSPL_HDR_SIZE;

	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_rlc_msg = p_msg;

	oam_construct_api_header(p_rlc_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			dest_id, OAM_GET_LOG_CAT_RLC_RESP,
			msg_api_length);

	p_rlc_msg = p_rlc_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_rlc_msg,
			transaction_id,
			OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_CAT_RLC_RESP,
			(msg_len - LOWER_LAYER_INTERFACE_HEADER_SIZE),
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_rlc_msg = p_rlc_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;


	oam_memcpy(p_rlc_msg,&(p_oam_get_logcategory_rlc->logCategory),sizeof(UInt64));

	if(OAM_SUCCESS == oam_send_message(p_msg, dest_id,p_error_code))
	{
		OAM_LOG(OAM,OAM_INFO,"Sending RLC_GET_LOG_CATEGORY_RESP message to RLC");
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending RLC_GET_LOG_CATEGORY_RESP message to RLC with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_handle_egtpu_init_layer_cnf
 * Description    : This function handles egtpu init layer cnf received from EGTPU
 * Inputs         : p_api_buf :pointer to EGTPU init layer config
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_handle_egtpu_init_layer_cnf
(
 void* p_api_buf
 )
{
	oam_egtpu_initdb_cnf_t *p_oam_egtpu_initdb_cnf;

	OAM_FUN_ENTRY_TRACE();
	p_api_buf          = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	p_oam_egtpu_initdb_cnf = (oam_egtpu_initdb_cnf_t *)p_api_buf;

	/*check the response from EGTPU which passes either of below value
	 *     #define EGTPU_SUCCESS              0
	 *     #define EGTPU_FAILURE              1
	 */

#define EGTPU_SUCCESS              0

	OAM_LOG(OAM,OAM_DETAILED, "EGTPU_LM_INITDB_CNF response = %d",p_oam_egtpu_initdb_cnf->result);
	if (EGTPU_SUCCESS != p_oam_egtpu_initdb_cnf->result)  
	{
		OAM_LOG(OAM,OAM_INFO, "EGTPU_LM_INITDB_CNF is Failure");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	else
	{
		OAM_LOG(OAM,OAM_DETAILED, "EGTPU_LM_INITDB_CNF is Success");
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_pr_egtpu_send_get_log_category_req
 * Description    : This function provide current log category of egtpu module to oam
 * Inputs         : src_module_id : Source module identifier
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/        
oam_return_et
oam_pr_egtpu_send_get_log_category_req
(
 oam_counter_t src_module_id,  /* Source module identifier */
 oam_counter_t transaction_id,/* Interface transaction identifier */
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = 0;
	oam_counter_t msg_api_length = 0;
	UInt8 *p_msg = OAM_NULL;
	UInt8 *p_mac_msg = OAM_NULL;
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,PR_GTPU_GET_LOG_CATEGORY_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			EGTPU_PACKET_RELAY_MODULE_ID,
			PR_GTPU_GET_LOG_CATEGORY_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);


	if( OAM_SUCCESS == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_INFO," Sending message to EGTPU");
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending message to EGTPU with error code %d",
				*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}



/****************************************************************************
 * Function Name  : oam_pr_egtpu_get_logcategory_resp
 * Description    : This function is used to response of get log category request 
 *                  of egtpu module.
 * Inputs         : p_api_buf : Pointer to API Buffer
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/      
oam_return_et
oam_pr_egtpu_get_logcategory_resp
(
 void* p_api_buf,
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg, *p_egtpu_msg;
	Oam_LogCategory_Req_t * p_egtpu_getlog_category_req =
		(Oam_LogCategory_Req_t *)(p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE);
	/* Get API length */
	msg_length  = sizeof(Oam_LogCategory_Req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		/* Not Enough Memory */
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_egtpu_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			dest_id,OAM_GET_LOG_CAT_EGTPU_RESP, msg_length);

	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;
	/* Fill interface header */
	oam_construct_interface_api_header(p_egtpu_msg,
			transaction_id,
			OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_CAT_EGTPU_RESP,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_egtpu_msg = p_egtpu_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	oam_memcpy(p_egtpu_msg,p_egtpu_getlog_category_req, sizeof(Oam_LogCategory_Req_t));

	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, dest_id,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				, "Error in function oam_send_message to send OAM_GET_LOG_CAT_EGTPU_RESP: %d!",*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;

}



/****************************************************************************
 * Function Name  : oam_egtpu_set_log_cat_enable_req
 * Description    : This function is used to send set log category enable request
 *                  of egtpu module.
 * Inputs         : p_api_buf : Pointer to API Buffer
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/       
oam_return_et
oam_egtpu_set_log_cat_enable_req
(
 void* p_api_buf,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg, *p_egtpu_msg;
	Oam_LogCategory_Req_t * p_egtpu_getlog_category_req =
		(Oam_LogCategory_Req_t *)(p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE);
	/* Get API length */
	msg_length  = sizeof(Oam_LogCategory_Req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		/* Not Enough Memory */
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_egtpu_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			EGTPU_PACKET_RELAY_MODULE_ID,PR_GTPU_ENABLE_LOG_CATEGORY_REQ, msg_length);

	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;
	/* Fill interface header */
	oam_construct_interface_api_header(p_egtpu_msg,
			transaction_id,
			OAM_MODULE_ID,
			EGTPU_PACKET_RELAY_MODULE_ID,
			PR_GTPU_ENABLE_LOG_CATEGORY_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_egtpu_msg = p_egtpu_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	oam_memcpy(p_egtpu_msg,&p_egtpu_getlog_category_req->logCategory, sizeof(Oam_LogCategory_Req_t));

	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				, "Error in function oam_send_message : %d!",*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/

		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}



/****************************************************************************
 * Function Name  : oam_egtpu_set_log_cat_disable_req
 * Description    : This function is used to send set log category disable request
 *                  of egtpu module.
 * Inputs         : p_api_buf : Pointer to API Buffer
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/       
oam_return_et
oam_egtpu_set_log_cat_disable_req
(
 void* p_api_buf,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg, *p_egtpu_msg;
	Oam_LogCategory_Req_t * p_egtpu_getlog_category_req =
		(Oam_LogCategory_Req_t *)(p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE);
	/* Get API length */
	msg_length  = sizeof(Oam_LogCategory_Req_t);
	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		/* Not Enough Memory */
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	p_egtpu_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_egtpu_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			EGTPU_PACKET_RELAY_MODULE_ID,PR_GTPU_DISABLE_LOG_CATEGORY_REQ, msg_length);

	p_egtpu_msg = p_egtpu_msg + OAM_CSPL_HDR_SIZE;
	/* Fill interface header */
	oam_construct_interface_api_header(p_egtpu_msg,
			transaction_id,
			OAM_MODULE_ID,
			EGTPU_PACKET_RELAY_MODULE_ID,
			PR_GTPU_DISABLE_LOG_CATEGORY_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_egtpu_msg = p_egtpu_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	oam_memcpy(p_egtpu_msg,&p_egtpu_getlog_category_req->logCategory, sizeof(Oam_LogCategory_Req_t));

	/* Check for Send message to EGTPU */
	if(OAM_FAILURE  == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR
				, "Error in function oam_send_message : %d!",*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 98828 Fix Start*/
		/*code Deleted*/
		/*Coverity 98828 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
/****************************************************************************
 * Function Name  : oam_send_pdcp_get_perf_stats_req
 * Description    : This function is used to send pdcp get perf stats request
 *                  of pdcp module.
 * Inputs         : src_module_id : src module id
 *                : transaction_id : Interface transaction identifier
 * Outputs        : p_error_code : pointer to error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/

oam_return_et
oam_send_pdcp_get_perf_stats_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{

	oam_counter_t msg_api_length = OAM_ZERO;
	oam_counter_t msg_length = OAM_ZERO;
	oam_pdcp_get_perf_stats_req_t oam_pdcp_get_perf_stats_req = {OAM_ZERO};
	UInt8 *p_msg;
	p_msg     = OAM_NULL;
	UInt8 *p_pdcp_msg;
	p_pdcp_msg = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	msg_api_length = msg_length + sizeof(oam_pdcp_get_perf_stats_req_t);

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Memory allocation to p_msg failed"
				" with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_pdcp_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
			PDCP_MODULE_ID, PDCP_GET_PERF_STATS_REQ , msg_api_length);

	/* Fill interface header */
	p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_pdcp_msg,
			transaction_id,
			src_module_id,
			PDCP_MODULE_ID,
			PDCP_GET_PERF_STATS_REQ,
			sizeof(oam_pdcp_get_perf_stats_req_t),
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_pdcp_msg = p_pdcp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	oam_pdcp_get_perf_stats_req.resetStats = OAM_ZERO;
	oam_pdcp_get_perf_stats_req.uePerfStatsToGetBitMap = 31;
	oam_pdcp_get_perf_stats_req.cellPerfStatsToGetBitMap = 31;

	oam_memcpy(p_pdcp_msg,&oam_pdcp_get_perf_stats_req, sizeof(oam_pdcp_get_perf_stats_req_t));
	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending PDCP_GET_PERF_STATS_REQ message to PDCP with error code %d",
				*p_error_code);
		/* OAM BCOM Code Comment Changes Start */
		/*Coverity 87045 Fix Start*/
		/*code Deleted*/
		/*Coverity 87045 Fix End*/
		/* OAM BCOM Code Comment Changes End */
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */
/*spr 22474 start*/
/****************************************************************************
 * * Function Name  : oam_send_mac_get_debug_info_req
 * * Description    : This function sends debug request to MAC
 * * Inputs         : src_module_id  :Source module identifier
 *                  : debugType : the type for reports needed from MAC
 *                  : transaction_id :transaction identifier
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
oam_return_et
oam_send_mac_get_debug_info_req
(
 UInt16 debugType,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id
 )
{
    oam_counter_t msg_api_length = OAM_ZERO;
    UInt8 *p_msg = OAM_NULL;
    UInt8 *p_mac_msg = OAM_NULL;
    MacGetDebugInfoReqParams *p_info_req = OAM_NULL;
    oam_error_code_et error_code = NO_ERROR;

    OAM_FUN_ENTRY_TRACE();

    msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE + sizeof(MacGetDebugInfoReqParams);

    /* Allocate buffer */
    if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg, &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d",
                error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
    p_mac_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
            MAC_MODULE_ID, OAM_MAC_GET_DEBUG_INFO_REQ, msg_api_length);

    /* Fill interface header */
    p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_mac_msg,
            transaction_id,
            src_module_id,
            MAC_MODULE_ID,
            OAM_MAC_GET_DEBUG_INFO_REQ,
            sizeof(MacGetDebugInfoReqParams),
            oam_prov_req.cell_cntxt.curr_cell_cid);

    p_info_req = (MacGetDebugInfoReqParams *)(p_mac_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE);
    p_info_req->debugType = debugType; 

    /* Send message to MAC */
    if(OAM_FAILURE == oam_send_message(p_msg, MAC_MODULE_ID, &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Sending OAM_MAC_GET_DEBUG_INFO_REQ message to MAC with error code %d",
                error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * * Function Name  : oam_send_rlc_get_debug_info_req
 * * Description    : This function sends debug request to RLC
 * * Inputs         : src_module_id  :Source module identifier
 *                  : debugType : the type for reports needed from MAC
 *                  : transaction_id :transaction identifier
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
oam_return_et
oam_send_rlc_get_debug_info_req
(
 UInt16 debugType,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id
 )
{
    oam_counter_t msg_api_length = OAM_ZERO;
    UInt8 *p_msg = OAM_NULL;
    UInt8 *p_rlc_msg = OAM_NULL;
    RlcGetDebugInfoReqParams *p_info_req = OAM_NULL;
    oam_error_code_et error_code = NO_ERROR;

    OAM_FUN_ENTRY_TRACE();

    msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE + sizeof(RlcGetDebugInfoReqParams);

    /* Allocate buffer */
    if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg, &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d",
                error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
    p_rlc_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_rlc_msg, OAM_VERSION_ID, src_module_id,
            RLC_MODULE_ID, OAM_RLC_GET_DEBUG_INFO_REQ, msg_api_length);

    /* Fill interface header */
    p_rlc_msg = p_rlc_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_rlc_msg,
            transaction_id,
            src_module_id,
            RLC_MODULE_ID,
            OAM_RLC_GET_DEBUG_INFO_REQ,
            sizeof(RlcGetDebugInfoReqParams),
            oam_prov_req.cell_cntxt.curr_cell_cid);

    p_info_req = (RlcGetDebugInfoReqParams *)(p_rlc_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE);
    p_info_req->debugType = debugType; 

    /* Send message to RLC */
    if(OAM_FAILURE == oam_send_message(p_msg, RLC_MODULE_ID, &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Sending OAM_RLC_GET_DEBUG_INFO_REQ message to RLC with error code %d",
                error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * * Function Name  : oam_send_pdcp_get_debug_info_req
 * * Description    : This function sends debug request to PDCP
 * * Inputs         : src_module_id  :Source module identifier
 *                  : debugType : the type for reports needed from MAC
 *                  : transaction_id :transaction identifier
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
oam_return_et
oam_send_pdcp_get_debug_info_req
(
 UInt16 debugType,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id
 )
{
    oam_counter_t msg_api_length = OAM_ZERO;
    UInt8 *p_msg = OAM_NULL;
    UInt8 *p_pdcp_msg = OAM_NULL;
    PdcpGetDebugInfoReqParams *p_info_req = OAM_NULL;
    oam_error_code_et error_code = NO_ERROR;

    OAM_FUN_ENTRY_TRACE();

    msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE + sizeof(MacGetDebugInfoReqParams);

    /* Allocate buffer */
    if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg, &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d",
                error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
    p_pdcp_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_pdcp_msg, OAM_VERSION_ID, src_module_id,
            PDCP_MODULE_ID, OAM_PDCP_GET_DEBUG_INFO_REQ, msg_api_length);

    /* Fill interface header */
    p_pdcp_msg = p_pdcp_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_pdcp_msg,
            transaction_id,
            src_module_id,
            PDCP_MODULE_ID,
            OAM_PDCP_GET_DEBUG_INFO_REQ,
            sizeof(PdcpGetDebugInfoReqParams),
            oam_prov_req.cell_cntxt.curr_cell_cid);

    p_info_req = (PdcpGetDebugInfoReqParams *)(p_pdcp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE);
    p_info_req->debugType = debugType;

    /* Send message to PDCP */
    if(OAM_FAILURE == oam_send_message(p_msg, PDCP_MODULE_ID, &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Sending OAM_PDCP_GET_DEBUG_INFO_REQ message to PDCP with error code %d",
                error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * * Function Name  : oam_send_egtpu_get_debug_info_req
 * * Description    : This function sends debug request to EGTPU
 * * Inputs         : src_module_id  :Source module identifier
 *                  : debugType : the type for reports needed from MAC
 *                  : transaction_id :transaction identifier
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
oam_return_et
oam_send_egtpu_get_debug_info_req
(
 UInt16 debugType,
 oam_counter_t src_module_id,
 oam_counter_t transaction_id
 )
{
    oam_counter_t msg_api_length = OAM_ZERO;
    UInt8 *p_msg = OAM_NULL;
    UInt8 *p_gtp_msg = OAM_NULL;
    GtpuGetDebugInfoReqParams *p_info_req = OAM_NULL;
    oam_error_code_et error_code = NO_ERROR;

    OAM_FUN_ENTRY_TRACE();

    msg_api_length = OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE + sizeof(MacGetDebugInfoReqParams);

    /* Allocate buffer */
    if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg, &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d",
                error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
    p_gtp_msg = p_msg;

    /* Fill CSPL header */
    oam_construct_api_header(p_gtp_msg, OAM_VERSION_ID, src_module_id,
            EGTPU_PACKET_RELAY_MODULE_ID, OAM_GTPU_GET_DEBUG_INFO_REQ, msg_api_length);

    /* Fill interface header */
    p_gtp_msg = p_gtp_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_interface_api_header(p_gtp_msg,
            transaction_id,
            src_module_id,
            EGTPU_PACKET_RELAY_MODULE_ID,
            OAM_GTPU_GET_DEBUG_INFO_REQ,
            sizeof(MacGetDebugInfoReqParams),
            oam_prov_req.cell_cntxt.curr_cell_cid);

    p_info_req = (GtpuGetDebugInfoReqParams *)(p_gtp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE);
    p_info_req->debugType = debugType; 

    /* Send message to GTPU */
    if(OAM_FAILURE == oam_send_message(p_msg, EGTPU_PACKET_RELAY_MODULE_ID, &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Sending OAM_EGTPU_GET_DEBUG_INFO_REQ message to EGTPU with error code %d",
                error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}


/****************************************************************************
 * * Function Name  : oam_handle_mac_get_debug_info_cnf
 * * Description    : This function handles debug response from MAC
 * * Inputs         : src_module_id  :Source module identifier
 *                  : debugType : the type for reports needed from MAC
 *                  : transaction_id :transaction identifier
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
oam_return_et
oam_handle_mac_get_debug_info_cnf
(
 void* p_api_buf,
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
)
{
    MacGetDebugInfoRespParams *p_mac_resp = OAM_NULL;
    UInt8 **str_arr = OAM_NULL; /* stores the strings[line][text] to be printed */
    UInt8 size_x = OAM_ZERO; /* Counts the number of strings to be printed */

    OAM_FUN_ENTRY_TRACE();

    /* Cell index is not used in current cases */
    UInt8 cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET)); 
    if (!(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid)&&
            (oam_get_rrm_cell_config_idx(cell_index) == OAM_ERROR_RETURN_VAL)) 
    {
        oam_construct_n_send_get_resp_to_usr(OAM_RESULT_FAILURE,
                OAM_ERR_REQUEST_DENIED,
                OAM_VERSION_ID, OAM_MODULE_ID,
                dest_id, transaction_id, OAM_ZERO, OAM_ZERO,
                "Received Invalid Cell Index",
                p_error_code);

        OAM_LOG(OAM,OAM_ERROR,"Received invalid cell_index:%d",cell_index);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_api_buf = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    p_mac_resp = (MacGetDebugInfoRespParams*)p_api_buf;

    if (OAM_SUCCESS == p_mac_resp->responseCode)
    {
        size_x = oam_get_mac_debug_info_prints (p_mac_resp, &str_arr);
    }
    else
    {
        OAM_LOG(OAM,OAM_ERROR,"Received Failure response from MAC for MAC_GET_DEBUG_INFO_RESP ");
        oam_construct_n_send_get_resp_to_usr(
                OAM_RESULT_FAILURE,
                OAM_ERR_INTERNAL_ERROR,
                OAM_VERSION,
                OAM_MODULE_ID,dest_id,
                transaction_id,
                OAM_ZERO,OAM_ZERO,"MAC LAYER FAILURE RESPONSE", 
                p_error_code     
                );
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    /* Send message to CLI if atleast one string is returned or MAC has returned SUCCESS */
    if (size_x)
    {
        oam_construct_n_send_gpv_response((UInt8 *)"Debug.Get.L2.MAC", str_arr, size_x,
                transaction_id, p_error_code, dest_id, OAM_ZERO, OAM_ZERO);
    }
    else
    {
        OAM_LOG(OAM,OAM_ERROR,"Received error in response from MAC in MAC_GET_DEBUG_INFO_RESP %u %u",
                 p_mac_resp->responseCode, size_x);

        oam_construct_n_send_get_resp_to_usr(
                OAM_RESULT_FAILURE,
                OAM_ERR_INTERNAL_ERROR,
                OAM_VERSION,
                OAM_MODULE_ID,dest_id,
                transaction_id,
                OAM_ZERO,OAM_ZERO,"MAC LAYER FAILURE RESPONSE", 
                p_error_code     
                );
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * * Function Name  : oam_get_mac_debug_info_prints
 * * Description    : This function checks the debug type and prepare string for print
 * * Inputs         : p_mac_resp : Input message response from MAC
 *                  : str_arr : string for print
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
UInt16
oam_get_mac_debug_info_prints
(
 MacGetDebugInfoRespParams* p_mac_resp,
 UInt8*** str_arr
)
{
    UInt16 num_str = OAM_ZERO;

    switch(p_mac_resp->debugType)
    {
        case GET_DEBUG_INFO_MEM_STATS:
        {
            num_str = oam_get_debug_mem_stats_prints(&p_mac_resp->u.csplMemPoolStats, str_arr);
            break;
        }
        case GET_DEBUG_INFO_MSG_STATS:
        {
            num_str = oam_get_debug_msg_stats_prints(&p_mac_resp->u.csplMsgPoolStats, str_arr);
            break;
        }
        default:
        {
            OAM_LOG(OAM, OAM_ERROR,
                       "Received wrong debugType:%u  in response from"
                       " MAC in MAC_OAM_GET_DEBUG_INFO_RESP with resp:%u",
                       p_mac_resp->debugType, p_mac_resp->responseCode);

            break;
        }
    }

    return num_str;
}

/****************************************************************************
 * * Function Name  : oam_handle_rlc_get_debug_info_cnf
 * * Description    : This function handles debug response from RLC
 * * Inputs         : src_module_id  :Source module identifier
 *                  : debugType : the type for reports needed from RLC
 *                  : transaction_id :transaction identifier
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
oam_return_et
oam_handle_rlc_get_debug_info_cnf
(
 void* p_api_buf,
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
)
{
    RlcGetDebugInfoRespParams *p_resp = OAM_NULL;
    UInt8 **str_arr = OAM_NULL; /* stores the strings[line][text] to be printed */
    UInt8 size_x = OAM_ZERO; /* Counts the number of strings to be printed */

    OAM_FUN_ENTRY_TRACE();

    /* Cell index is not used in current cases */
    UInt8 cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET)); 
    OAM_LOG(OAM,OAM_ERROR,"Inside oam_handle_rlc_get_debug_info_cnf cell_index = %d",cell_index);
    if (!(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid)&&
            (oam_get_rrm_cell_config_idx(cell_index) == OAM_ERROR_RETURN_VAL)) 
    {
        oam_construct_n_send_get_resp_to_usr(OAM_RESULT_FAILURE,
                OAM_ERR_REQUEST_DENIED,
                OAM_VERSION_ID, OAM_MODULE_ID,
                dest_id, transaction_id, OAM_ZERO, OAM_ZERO,
                "Received Invalid Cell Index",
                p_error_code);

        OAM_LOG(OAM,OAM_ERROR,"Received invalid cell_index:%d",cell_index);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_api_buf = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    p_resp = (RlcGetDebugInfoRespParams*)p_api_buf;

    if (OAM_SUCCESS == p_resp->responseCode)
    {
        OAM_LOG(OAM,OAM_ERROR,"Received success response from RLC for get_debug_info_resp");
        size_x = oam_get_rlc_debug_info_prints (p_resp, &str_arr);
    }
    else
    {
        OAM_LOG(OAM,OAM_ERROR,"Received failure response from RLC in RLC_OAM_GET_STATS_CNF");

        oam_construct_n_send_get_resp_to_usr(
                OAM_RESULT_FAILURE,
                OAM_ERR_INTERNAL_ERROR,
                OAM_VERSION,
                OAM_MODULE_ID,dest_id,
                transaction_id,
                OAM_ZERO,OAM_ZERO,"RLC LAYER FAILURE RESPONSE", 
                p_error_code
                );
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;

    }
    /* Send message to CLI if atleast one string is returned or RLC has returned SUCCESS */
    if (size_x)
    {
        oam_construct_n_send_gpv_response((UInt8 *)"Debug.Get.L2.RLC", str_arr, size_x,
                transaction_id, p_error_code, dest_id, OAM_ZERO, OAM_ZERO);
    }
    else
    {
        OAM_LOG(OAM,OAM_ERROR,"Received error in response from RLC in RLC_OAM_GET_STATS_CNF %u %u",
                 p_resp->responseCode, size_x);

        oam_construct_n_send_get_resp_to_usr(
                OAM_RESULT_FAILURE,
                OAM_ERR_INTERNAL_ERROR,
                OAM_VERSION,
                OAM_MODULE_ID,dest_id,
                transaction_id,
                OAM_ZERO,OAM_ZERO,"RLC LAYER FAILURE RESPONSE", 
                p_error_code
                );
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * * Function Name  : oam_get_rlc_debug_info_prints
 * * Description    : This function checks the debug type and prepare string for print
 * * Inputs         : p_resp : Input message response from RLC
 *                  : str_arr : string for print
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
UInt16
oam_get_rlc_debug_info_prints
(
 RlcGetDebugInfoRespParams* p_resp,
 UInt8*** str_arr
)
{
    UInt16 num_str = OAM_ZERO;

    switch(p_resp->debugType)
    {
        case GET_DEBUG_INFO_MEM_STATS:
        {
            num_str = oam_get_debug_mem_stats_prints(&p_resp->u.csplMemPoolStats, str_arr);
            break;
        }
        case GET_DEBUG_INFO_MSG_STATS:
        {
            num_str = oam_get_debug_msg_stats_prints(&p_resp->u.csplMsgPoolStats, str_arr);
            break;
        }
        default:
        {
            OAM_LOG(OAM, OAM_ERROR,
                       "Received wrong debugType:%u  in response from"
                       " RLC in RLC_OAM_GET_DEBUG_INFO_RESP with resp:%u",
                       p_resp->debugType, p_resp->responseCode);

            break;
        }
    }

    return num_str;
}

/****************************************************************************
 * * Function Name  : oam_handle_pdcp_get_debug_info_cnf
 * * Description    : This function handles debug response from PDCP
 * * Inputs         : src_module_id  :Source module identifier
 *                  : debugType : the type for reports needed from PDCP
 *                  : transaction_id :transaction identifier
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
oam_return_et
oam_handle_pdcp_get_debug_info_cnf
(
 void* p_api_buf,
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
)
{
    PdcpGetDebugInfoRespParams *p_resp = OAM_NULL;
    UInt8 **str_arr = OAM_NULL; /* stores the strings[line][text] to be printed */
    UInt8 size_x = OAM_ZERO; /* Counts the number of strings to be printed */

    OAM_FUN_ENTRY_TRACE();

    /* Cell index is not used in current cases */
    UInt8 cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET)); 
    if (!(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid)&&
            (oam_get_rrm_cell_config_idx(cell_index) == OAM_ERROR_RETURN_VAL)) 
    {
        oam_construct_n_send_get_resp_to_usr(OAM_RESULT_FAILURE,
                OAM_ERR_REQUEST_DENIED,
                OAM_VERSION_ID, OAM_MODULE_ID,
                dest_id, transaction_id, OAM_ZERO, OAM_ZERO,
                "Received Invalid Cell Index",
                p_error_code);

        OAM_LOG(OAM,OAM_ERROR,"Received invalid cell_index:%d",cell_index);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_api_buf = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    p_resp = (PdcpGetDebugInfoRespParams*)p_api_buf;

    if (OAM_SUCCESS == p_resp->responseCode)
    {
        size_x = oam_get_pdcp_debug_info_prints (p_resp, &str_arr);
    }
    else
    {
        OAM_LOG(OAM,OAM_ERROR,"Received failure response from PDCP in PDCP_OAM_GET_STATS_CNF");

        oam_construct_n_send_get_resp_to_usr(
                OAM_RESULT_FAILURE,
                OAM_ERR_INTERNAL_ERROR,
                OAM_VERSION,
                OAM_MODULE_ID,dest_id,
                transaction_id,
                OAM_ZERO,OAM_ZERO,"PDCP LAYER FAILURE RESPONSE",
                p_error_code
                );
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    /* Send message to CLI if atleast one string is returned or PDCP has returned SUCCESS */
    if (size_x)
    {
        oam_construct_n_send_gpv_response((UInt8 *)"Debug.Get.L2.PDCP", str_arr, size_x,
                transaction_id, p_error_code, dest_id, OAM_ZERO, OAM_ZERO);
    }
    else
    {
        OAM_LOG(OAM,OAM_ERROR,"Received error in response from PDCP in PDCP_OAM_GET_STATS_CNF %u %u",
                 p_resp->responseCode, size_x);

        oam_construct_n_send_get_resp_to_usr(
                OAM_RESULT_FAILURE,
                OAM_ERR_INTERNAL_ERROR,
                OAM_VERSION,
                OAM_MODULE_ID,dest_id,
                transaction_id,
                OAM_ZERO,OAM_ZERO,"PDCP LAYER FAILURE RESPONSE",
                p_error_code
                );
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * * Function Name  : oam_get_pdcp_debug_info_prints
 * * Description    : This function checks the debug type and prepare string for print
 * * Inputs         : p_resp : Input message response from PDCP
 *                  : str_arr : string for print
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
UInt16
oam_get_pdcp_debug_info_prints
(
 PdcpGetDebugInfoRespParams* p_resp,
 UInt8*** str_arr
)
{
    UInt16 num_str = OAM_ZERO;

    switch(p_resp->debugType)
    {
        case GET_DEBUG_INFO_MEM_STATS:
        {
            num_str = oam_get_debug_mem_stats_prints(&p_resp->u.csplMemPoolStats, str_arr);
            break;
        }
        case GET_DEBUG_INFO_MSG_STATS:
        {
            num_str = oam_get_debug_msg_stats_prints(&p_resp->u.csplMsgPoolStats, str_arr);
            break;
        }
        default:
        {
            OAM_LOG(OAM, OAM_ERROR,
                       "Received wrong debugType:%u  in response from"
                       " PDCP in PDCP_OAM_GET_DEBUG_INFO_RESP with resp:%u",
                       p_resp->debugType, p_resp->responseCode);

            break;
        }
    }

    return num_str;
}

/****************************************************************************
 * * Function Name  : oam_handle_gtpu_get_debug_info_cnf
 * * Description    : This function handles debug response from GTP
 * * Inputs         : src_module_id  :Source module identifier
 *                  : debugType : the type for reports needed from GTP
 *                  : transaction_id :transaction identifier
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
oam_return_et
oam_handle_gtpu_get_debug_info_cnf
(
 void* p_api_buf,
 UInt16 transaction_id,
 UInt16 dest_id,
 oam_error_code_et *p_error_code
)
{
    GtpuGetDebugInfoRespParams *p_resp = OAM_NULL;
    UInt8 **str_arr = OAM_NULL; /* stores the strings[line][text] to be printed */
    UInt8 size_x = OAM_ZERO; /* Counts the number of strings to be printed */

    OAM_FUN_ENTRY_TRACE();

    /* Cell index is not used in current cases */
    UInt8 cell_index = (UInt8)(*((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET)); 
    if (!(cell_index == oam_prov_req.cell_cntxt.curr_cell_cid)&&
            (oam_get_rrm_cell_config_idx(cell_index) == OAM_ERROR_RETURN_VAL)) 
    {
        oam_construct_n_send_get_resp_to_usr(OAM_RESULT_FAILURE,
                OAM_ERR_REQUEST_DENIED,
                OAM_VERSION_ID, OAM_MODULE_ID,
                dest_id, transaction_id, OAM_ZERO, OAM_ZERO,
                "Received Invalid Cell Index",
                p_error_code);

        OAM_LOG(OAM,OAM_ERROR,"Received invalid cell_index:%d",cell_index);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_api_buf = p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE;
    p_resp = (GtpuGetDebugInfoRespParams*)p_api_buf;

    if (OAM_SUCCESS == p_resp->responseCode)
    {
        size_x = oam_get_gtpu_debug_info_prints (p_resp, &str_arr);
    }
    else
    {
        OAM_LOG(OAM,OAM_ERROR,"Received error in response from GTPU in GTPU_OAM_GET_STATS_CNF %u %u",
                 p_resp->responseCode, size_x);

        oam_construct_n_send_get_resp_to_usr(
                OAM_RESULT_FAILURE,
                OAM_ERR_INTERNAL_ERROR,
                OAM_VERSION,
                OAM_MODULE_ID,dest_id,
                transaction_id,
                OAM_ZERO,OAM_ZERO,"GTPU LAYER FAILURE RESPONSE",
                p_error_code
                );
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    /* Send message to CLI if atleast one string is returned or GTPU has returned SUCCESS */
    if (size_x)
    {
        oam_construct_n_send_gpv_response((UInt8 *)"Debug.Get.L2.EGTPU", str_arr, size_x,
                transaction_id, p_error_code, dest_id, OAM_ZERO, OAM_ZERO);
    }
    else
    {
        OAM_LOG(OAM,OAM_ERROR,"Received error in response from GTPU in GTPU_OAM_GET_STATS_CNF %u %u",
                 p_resp->responseCode, size_x);

        oam_construct_n_send_get_resp_to_usr(
                OAM_RESULT_FAILURE,
                OAM_ERR_INTERNAL_ERROR,
                OAM_VERSION,
                OAM_MODULE_ID,dest_id,
                transaction_id,
                OAM_ZERO,OAM_ZERO,"GTPU LAYER FAILURE RESPONSE",
                p_error_code
                );
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/****************************************************************************
 * * Function Name  : oam_get_gtpu_debug_info_prints
 * * Description    : This function checks the debug type and prepare string for print
 * * Inputs         : p_resp : Input message response from GTP
 *                  : str_arr : string for print
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
UInt16
oam_get_gtpu_debug_info_prints
(
 GtpuGetDebugInfoRespParams* p_resp,
 UInt8*** str_arr
)
{
    UInt16 num_str = OAM_ZERO;

    switch(p_resp->debugType)
    {
        case GET_DEBUG_INFO_MEM_STATS:
        {
            num_str = oam_get_debug_mem_stats_prints(&p_resp->u.csplMemPoolStats, str_arr);
            break;
        }
        case GET_DEBUG_INFO_MSG_STATS:
        {
            num_str = oam_get_debug_msg_stats_prints(&p_resp->u.csplMsgPoolStats, str_arr);
            break;
        }
        default:
        {
            OAM_LOG(OAM, OAM_ERROR,
                       "Received wrong debugType:%u  in response from"
                       " GTPU in GTPU_OAM_GET_DEBUG_INFO_RESP with resp:%u",
                       p_resp->debugType, p_resp->responseCode);

            break;
        }
    }

    return num_str;
}
/*spr 22474 end*/


