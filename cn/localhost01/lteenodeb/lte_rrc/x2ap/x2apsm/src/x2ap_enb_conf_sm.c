/*******************************************************************************
*
*  FILE NAME   : x2ap_enb_conf_sm.c
*
*  DESCRIPTION : This file contains the ENB Configuration Update state machine
*                and the state machine Entry function definition.
*
*  DATE 24-June-2010  NAME Tauheed Ahmed
*                     REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2010, Aricent Inc.
*
*******************************************************************************/

#include "x2setup_sm.h"
#include "x2ap_evthdlr.h"
#include "x2ap_enb_conf_sm.h"
#include "x2ap_tracing.h"
#include "x2ap_timer.h"
#include "x2ap_utils.h"
#include "asn_enc_wrappers.h"
#include "x2ap_api.h"
#include "sm_entry.h"
#include "rrc_x2apOam_il_composer.h"
#include "rrc_x2apRrm_il_composer.h"
#include "asn_dec_wrappers.h"
#include "rrc_defines.h"
#include "rrc_intrl_api.h"
#include "rrc_msg_mgmt.h"
#include "rrc_x2apCommon_il_composer.h"
#include "rrc_x2apCommon_il_parser.h"
#ifdef LTE_EMBMS_SUPPORTED
#include "rrc_x2apRrm_il_parser.h"

extern void x2ap_compose_cell_id_from_ecgi(x2_gb_enb_id_t global_enb_id, U8* eutran_cell_id, U8* p_cell_id);
#endif
void x2ap_process_neigh_ecgi_enb_id_mapping(
    x2ap_gb_context_t *p_x2ap_gb_ctx,
    x2ap_neigh_ecgi_enb_id_map_list_t *p_neigh_ecgi_enb_id_map);

void x2ap_enb_config_update_time_to_wait_expiry(
        x2ap_timer_buffer_t *p_timer_buf,
        x2ap_gb_context_t *p_x2ap_gb_ctx);

void x2ap_enb_config_update_response_timer_expiry(
        x2ap_timer_buffer_t *p_timer_buf, 
        x2ap_gb_context_t *p_x2ap_gb_ctx);

extern void x2ap_send_enb_config_update_response(
        x2ap_gb_context_t* p_x2ap_gb_ctx, 
        x2ap_enb_config_update_resp_t* p_enb_config_response, 
        U8 result, 
        U16 dst_id, 
        U16 trans_id);
#ifdef LTE_EMBMS_SUPPORTED
U8 x2ap_apply_updated_mbms_sf_info_ind
(
    x2ap_gb_context_t            *p_x2ap_gb_ctx, 
    rrm_updated_mbms_sf_info_t   *p_rrm_updated_mbms_sf_info,
     x2ap_enb_config_update_req_t *p_enb_cfg_req
);

void fill_enb_configuration_update_for_peer_enbs(
		x2ap_gb_context_t            *p_x2ap_gb_ctx, 
		x2ap_enb_config_update_req_t *p_enb_cfg_req,
    /* COV_94224_FIX_START */
                U8 *p_no_cells,
    /* COV_94224_FIX_END */
                U8 gb_ctx_index);
/* SPR 18736 changes start */
void x2ap_convert_cell_id_from_ecgi(U8* eutran_cell_id, U8* p_cell_id);
/* SPR 18736 changes end */
#endif


/* Bug 4691 Changes Start */
extern void x2ap_build_and_send_update_info(
        x2ap_nbour_info_t   *p_nbour_info);
/* Bug 4691 Changes End */

x2ap_return_et
x2ap_process_enb_config_update_enb_req(
    U8* p_msg_ptr,
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_peer_enb_context_t* p_enb_cntx);

x2ap_return_et
x2ap_process_enb_config_update_ack(
		U8* p_msg_ptr,
		 x2ap_gb_context_t* p_x2ap_gb_ctx,
		 x2ap_peer_enb_context_t* p_enb_cntx);

x2ap_return_et
x2ap_process_enb_config_update_failure(
		U8* p_msg_ptr,
		 x2ap_gb_context_t* p_x2ap_gb_ctx,
		 x2ap_peer_enb_context_t* p_enb_cntx);

void x2ap_process_enb_config_update_req(
        void* p_api, 
        x2ap_gb_context_t* p_x2ap_gb_ctx, 
        U16 dest_module_id);

void x2ap_apply_enb_config_update_req(
        x2ap_gb_context_t* p_x2ap_gb_ctx, 
        x2ap_enb_config_update_req_t *p_enb_cfg_req);

void x2ap_process_served_cell_to_add_list(
        x2ap_gb_context_t* p_x2ap_gb_ctx, 
        x2ap_served_cells_to_add_list_t *p_served_cell);

void x2ap_process_served_cell_to_modify_list(
        x2ap_gb_context_t* p_x2ap_gb_ctx, 
        x2ap_served_cells_to_modify_list_t *p_served_cell);

void x2ap_process_served_cell_to_delete_list(
        x2ap_gb_context_t* p_x2ap_gb_ctx, 
        x2ap_served_cells_to_delete_list_t *p_served_cell);

void x2ap_process_gu_group_id_to_delete_list(
        x2ap_gb_context_t* p_x2ap_gb_ctx, 
        x2ap_gu_group_id_list_t* p_delete_gu_id);

void x2ap_process_gu_group_id_to_add_list(
        x2ap_gb_context_t* p_x2ap_gb_ctx, 
        x2ap_gu_group_id_list_t* p_add_gu_id);

void x2ap_delete_nbour_cell_info(
        x2ap_gb_context_t* p_x2ap_gb_ctx, U8 cell_id);

void
x2ap_build_eNB_config_upd_resp
(
 x2ap_enb_conf_ack_t  *pENB_config_upd_resp
);

void
x2ap_build_and_send_enb_configuration_update_failure(
		 x2ap_gb_context_t* p_x2ap_gb_ctx,
		 x2ap_peer_enb_context_t* p_enb_cntx,
		 x2ap_enb_conf_upd_fail_t* p_eNB_config_upd_fail,
		 U8* p_asn_msg_buff,
		 U16* p_asn_msg_len,
		 U8 timeTowait_present);

void
x2ap_send_enb_configuration_update_ack(
		 x2ap_gb_context_t* p_x2ap_gb_ctx,
		 x2ap_peer_enb_context_t* p_enb_cntx,
		 x2ap_enb_conf_ack_t* p_eNB_config_upd_resp,
		 U8* p_asn_msg_buff,
		 U16* p_asn_msg_len);

void
x2ap_build_eNB_config_upd_resp_fail
(
 x2ap_enb_conf_upd_fail_t  *pENB_config_upd_resp_fail,
 x2ap_gb_context_t         *p_x2ap_gl_ctx,
 x2ap_bool_et               timeTowait_present
);

void
x2ap_enb_config_update_fill_cause(x2_cause_t* p_src, x2ap_cause_t* p_dst);

void x2ap_convert_time_to_wait_index_to_duration(U8 time_to_wait_index, 
        U32* p_timer_duration);

void    x2ap_populate_nbour_cell_info(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_nbour_info_t* p_neighbour_cell_info,
        U8 served_cell_id,
        x2_gb_enb_id_t *p_gb_enb_id);

void x2ap_populate_served_cell_info(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_cell_info_t* p_x2ap_gb_ctxt_cell_info,
        x2ap_served_cell_info_t* p_cell_info);

x2ap_return_et x2ap_send_enb_config_update_req_peer(
	x2ap_gb_context_t* p_x2ap_gb_ctx,
	x2ap_peer_enb_context_t* p_peer_enb_ctx,
	U8* p_asn_msg,
	U16 asn_msg_len,
	x2ap_timer_id_en timer_type);

x2ap_return_et
x2ap_process_peer_added_served_cells
(
	x2ap_enb_config_update_ind_t       *pENB_config_upd_req,
	x2ap_gb_context_t         *p_x2ap_gb_ctx,
	x2ap_peer_enb_context_t* p_enb_cntx);

x2ap_return_et
x2ap_process_peer_deleted_served_cells
(
	 x2ap_enb_config_update_ind_t       *pENB_config_upd_req,
	 x2ap_gb_context_t         *p_x2ap_gb_ctx,
	 x2ap_peer_enb_context_t* p_enb_cntx);

x2ap_return_et
x2ap_process_peer_modified_served_cells
(
	x2ap_enb_config_update_ind_t* pENB_config_upd_req,
	x2ap_gb_context_t* p_x2ap_gb_ctx,
	x2ap_peer_enb_context_t* p_enb_cntx);

x2ap_return_et
x2ap_process_peer_added_gu_id_list
(
	 x2ap_enb_config_update_ind_t* pENB_config_upd_req,
	 x2ap_peer_enb_context_t* p_enb_cntx);
x2ap_return_et
x2ap_process_peer_deleted_gu_id_list
(
	 x2ap_enb_config_update_ind_t* pENB_config_upd_req,
	 x2ap_peer_enb_context_t* p_enb_cntx);

#ifdef ENDC_ENABLED
void x2ap_build_nr_srvd_cell_to_management_list
(
    x2ap_nr_cell_info_t *p_nr_cell_info,
    x2ap_nr_srvd_cell_to_management_list_t *p_nr_srvd_cell_to_management_list
);
void x2ap_build_nr_srvd_cell_to_modify_list
(
    x2ap_nr_srvd_cell_to_modify_t *p_x2ap_nr_srvd_cell_modify,
    x2ap_nr_srvd_cell_to_modify_list_t *p_rrm_nr_srvd_cell_modify
);
void x2ap_build_nr_srvd_cell_to_delete_list
(
    x2ap_nr_srvd_cell_to_delete_t *p_x2ap_nr_srvd_cell_delete,
    x2ap_nr_srvd_cell_to_delete_list_t *p_rrm_nr_srvd_cell_delete_list
);
void x2ap_build_endc_config_update_ind
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_peer_enb_context_t* p_enb_cntx,
    x2_config_update_endc_en_gnb_t *p_endc_config_update_req, 
    x2ap_endc_config_update_en_gnb_ind_t *p_endc_config_update_ind
);
void x2ap_build_endc_config_upd_resp_fail
(
 x2ap_endc_config_update_failure_t  *pendc_config_upd_resp_fail,
 x2ap_gb_context_t         *p_x2ap_gl_ctx,
 x2ap_bool_et               timeTowait_present
 );
 void x2ap_send_endc_configuration_update_ack(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t* p_enb_cntx,
		x2ap_endc_config_update_ack_t* p_endc_config_upd_resp,
		UInt8* p_asn_msg_buff,
		UInt16* p_asn_msg_len);
void x2ap_build_and_send_endc_configuration_update_failure(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t* p_enb_cntx,
		x2ap_endc_config_update_failure_t* p_endc_config_upd_fail,
		UInt8* p_asn_msg_buff,
		UInt16* p_asn_msg_len,
		x2ap_bool_et timeTowait_present
);
 static x2ap_return_et x2ap_check_if_peer_enb_srv_cell_available
(
 x2ap_gb_context_t   *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx,
 x2ap_nr_ecgi_t         *p_x2ap_ecgi
);
static x2ap_return_et x2ap_validate_endc_peer_modified_served_cells
(
	x2ap_endc_config_update_en_gnb_req_t* pENB_config_upd_req,
	x2ap_gb_context_t* p_x2ap_gb_ctx,
	x2ap_peer_enb_context_t* p_enb_cntx
);
x2ap_return_et x2ap_validate_endc_config_update_req
(
	x2ap_endc_config_update_en_gnb_req_t* pENB_config_upd_req,
	x2ap_gb_context_t* p_x2ap_gb_ctx,
	x2ap_peer_enb_context_t* p_enb_cntx
);
void x2ap_populate_served_nr_cell_info
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_nr_neigh_info_t* p_neighbour_cell_info,
    x2ap_nr_ecgi_t *p_old_nr_cgi,
    U8 peer_enodeb_id
);
void x2ap_populate_nbour_nr_cell_info
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_nr_neigh_info_t * p_neighbour_cell_info,
    U8 served_cell_id,
    x2_gb_gnb_id_t *p_gb_gnb_id
);
x2ap_return_et
x2ap_process_endc_peer_added_served_cells
(
 x2_config_update_endc_en_gnb_t       *pENB_config_upd_req,
 x2ap_gb_context_t         *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx
);
x2ap_return_et
x2ap_process_endc_peer_deleted_served_cells
(
 x2_config_update_endc_en_gnb_t       *pENB_config_upd_req,
 x2ap_gb_context_t         *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx
);
x2ap_return_et
x2ap_process_endc_peer_modifyed_served_cells
(
 x2_config_update_endc_en_gnb_t* pENB_config_upd_req,
 x2ap_gb_context_t* p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx
);
x2ap_return_et x2ap_process_endc_config_update_en_gnb_req
(
 U8* p_msg_ptr,
 x2ap_gb_context_t* p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx
 );


x2ap_return_et x2ap_process_eutra_nr_served_cell_to_add_list
(
    x2ap_gb_context_t* p_x2ap_gb_ctx, 
    x2ap_served_eutra_nr_cells_to_add_list_t *p_served_cell
);
x2ap_return_et x2ap_apply_endc_config_update_req
(
    x2ap_gb_context_t* p_x2ap_gb_ctx, 
    x2ap_endc_config_update_req_t *p_endc_cfg_req
);
x2ap_return_et x2ap_send_endc_config_update_req_peer
(
    x2ap_gb_context_t* p_x2ap_gb_ctx, 
    x2ap_peer_enb_context_t* p_peer_enb_ctx, 
    U8* p_asn_msg, 
    U16 asn_msg_len, 
    x2ap_timer_id_en timer_type
);
void x2ap_endc_config_update_response_timer_expiry
(
    x2ap_timer_buffer_t *p_timer_buf, 
    x2ap_gb_context_t *p_x2ap_gb_ctx
);
void x2ap_endc_config_update_time_to_wait_expiry
(
    x2ap_timer_buffer_t *p_timer_buf, 
    x2ap_gb_context_t *p_x2ap_gb_ctx
);
 void x2ap_process_endc_config_update_req
(
    void* p_api, 
    x2ap_gb_context_t* p_x2ap_gb_ctx, 
    U16 dest_module_id
);
x2ap_return_et
x2ap_process_endc_config_update_ack
(
    U8* p_msg_ptr,
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_peer_enb_context_t* p_enb_cntx
);
x2ap_return_et
x2ap_process_endc_config_update_failure(
    U8* p_msg_ptr,
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_peer_enb_context_t* p_enb_cntx);
#endif

/*****************************************************************************
 * Function Name  : x2ap_delete_nbour_cell_info 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global data 
 *                  cell_id - cell id 
 * Outputs        : None
 * Returns        : None
 * Description    : This function delete nbour cell infoformation 
 ********************************************************************************/
void x2ap_delete_nbour_cell_info(x2ap_gb_context_t* p_x2ap_gb_ctx, U8 cell_id)
{
	U16 num_nbour =0;
	U16 nbour_count =0;
    /*Bug_11608_fix_start*/
	U8 count = 0;
    /*Bug_11608_fix_stop*/
	x2ap_ecgi_t ecgi;
	x2ap_nbr_ecgi_search_node_t *p_node = X2AP_P_NULL;

	X2AP_MEMSET(&ecgi, 0, sizeof(x2ap_ecgi_t));

	num_nbour = p_x2ap_gb_ctx->served_cell_info[cell_id].num_nbour;

	X2AP_TRACE(X2AP_INFO, "Deleting all %u nbour cells of serving cell with cell_id:%u .\n", num_nbour, cell_id);

	for(nbour_count=0; nbour_count< MAX_NEIGHBOURS && num_nbour; nbour_count++)
	{
		/* enb_configured changes start */
        /* Coverity Fix 64327 Start */
		if ((cell_id < MAX_NUM_CELL_IN_X2AP) && 
                p_x2ap_gb_ctx->relation_table[cell_id][nbour_count].relation != NOT_NEIGHBOUR)
		{
        /* Coverity Fix 64327 Start */
			/* enb_configured changes stop */
			ecgi = p_x2ap_gb_ctx->relation_table[cell_id][nbour_count].ecgi;
			num_nbour--;

			p_node = (x2ap_nbr_ecgi_search_node_t *)x2ap_st_find_tree_node((x2ap_search_tree_t *)&(p_x2ap_gb_ctx->nbr_ecgi_search_tree), (void*)(&ecgi) );
			if(p_node== X2AP_P_NULL)
			{
				X2AP_TRACE(X2AP_WARNING, "Neighbour ECGI not found in tree.\n");
			}
			else
			{
			    /*Bug_11608_fix_start*/
			    for (count = 0; count < MAX_NUM_CELL_IN_X2AP; count ++)
			    {
				/* Coverity Fix 81651 Start */
				if (!(X2AP_MEMCMP(&p_x2ap_gb_ctx->relation_table[count][nbour_count].ecgi,
						&ecgi, sizeof(ecgi))) && 
					(p_x2ap_gb_ctx->relation_table[count][nbour_count].relation != NOT_NEIGHBOUR)) 
				    /* Coverity Fix 81651 Stop */
				{
				    break;
				}
			    }
			    if (MAX_NUM_CELL_IN_X2AP == count)
			    {
/* SPR 16405 START */ 
					X2AP_TRACE(X2AP_INFO,"logical_id:%d\n", p_node->logical_id);
					X2AP_MEMSET(&p_x2ap_gb_ctx->peer_enb_cell_info[p_node->logical_id], 0x00, sizeof(x2ap_peer_enb_cell_info_t));
					x2ap_st_delete_node(&p_x2ap_gb_ctx->nbr_ecgi_search_tree, &(p_node->nodeAnchor));
					x2ap_mem_free(&p_node->nodeAnchor);

					p_x2ap_gb_ctx->num_peer_enb_cells--;
					/*SPR 16405 STOP*/

			    }
			    /*Bug_11608_fix_stop*/
			    /* Coverity Fix 64327 Start */
			    /*Coverity Fix 64328 Start */
			    p_x2ap_gb_ctx->served_cell_info[cell_id].num_nbour--;
			    X2AP_TRACE(X2AP_INFO,"Total number of peer eNB neighbour cells:%u", p_x2ap_gb_ctx->num_peer_enb_cells);
			    if (nbour_count < MAX_NEIGHBOURS)
			    {
				p_x2ap_gb_ctx->relation_table[cell_id][nbour_count].relation =NOT_NEIGHBOUR;
				/* enb_configured changes start */
				X2AP_MEMSET(&(p_x2ap_gb_ctx->relation_table[cell_id][nbour_count].ecgi),0, sizeof(x2ap_ecgi_t));
			    }
			    /*Coverity Fix 64328 Stop */
			    /*Coverity Fix 64327 Stop */
			}
		}
		/* enb_configured changes stop */
	}
}

/*****************************************************************************
 * Function Name  : x2ap_process_gu_group_id_to_add_list 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_add_gu_id - pointer to x2ap_gu_group_id_list_t
 * Outputs        : p_add_gu_id
 * Returns        : None
 * Description    : this function adds GU group Id to List 
 ********************************************************************************/
void x2ap_process_gu_group_id_to_add_list(x2ap_gb_context_t* p_x2ap_gb_ctx, x2ap_gu_group_id_list_t* p_add_gu_id)
{
	X2AP_TRACE(X2AP_INFO, "%u GU group id to add.\n", p_add_gu_id->num_gu_group_id);

	if(p_x2ap_gb_ctx->group_ids.num_gu_group_id + p_add_gu_id->num_gu_group_id > MAX_ENB_POOLS)
	{
		X2AP_TRACE(X2AP_INFO, "Total number of GU group id greater than maximum MAX_ENB_POOLS.\n");
		return;
	}

	X2AP_MEMCPY ( (void*)&(p_x2ap_gb_ctx->group_ids.group_id_arr[p_x2ap_gb_ctx->group_ids.num_gu_group_id]), 
			(const void*)(p_add_gu_id->gu_id_list), 
			(p_add_gu_id->num_gu_group_id * sizeof(x2ap_gu_group_id_t)) );

	p_x2ap_gb_ctx->group_ids.num_gu_group_id += p_add_gu_id->num_gu_group_id;

	X2AP_TRACE(X2AP_INFO, "Total GU group id:%u.\n", p_x2ap_gb_ctx->group_ids.num_gu_group_id);
}

/*****************************************************************************
 * Function Name  : x2ap_process_gu_group_id_to_delete_list 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2Ap global context
 *                  p_delete_gu_id - pointer to x2ap_gu_group_id_list_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function delets the GU group Id from list 
 ********************************************************************************/
void x2ap_process_gu_group_id_to_delete_list(x2ap_gb_context_t* p_x2ap_gb_ctx, x2ap_gu_group_id_list_t* p_delete_gu_id)
{
	U8 gu_id_count = 0;
	U8 gu_id_counter = 0;
	U8 gu_id_found = X2AP_FAILURE;

	X2AP_TRACE(X2AP_INFO, "%u GU group id to delete.\n", p_delete_gu_id->num_gu_group_id);

	for(gu_id_count=0; gu_id_count< p_delete_gu_id->num_gu_group_id; gu_id_count++)
	{
		gu_id_found = X2AP_FAILURE;

		/* Search GU group id information location*/
		for(gu_id_counter=0;
				((gu_id_counter<p_x2ap_gb_ctx->group_ids.num_gu_group_id)
				 &&(gu_id_found!=X2AP_SUCCESS)
				 /* klocwork fix */ 
				 && (gu_id_counter < MAX_ENB_POOLS));/* klocwork fix */  
				gu_id_counter++)
		{
			if(!(X2AP_MEMCMP(&p_x2ap_gb_ctx->group_ids.group_id_arr[gu_id_counter], &p_delete_gu_id->gu_id_list[gu_id_count],sizeof(x2ap_gu_group_id_t))))
			{
				gu_id_found = X2AP_SUCCESS;

				/* Delete GU group id information at the location*/
				X2AP_MEMSET(&p_x2ap_gb_ctx->group_ids.group_id_arr[gu_id_counter], 0x00, sizeof(x2ap_gu_group_id_t));

				/* Copy last GU group id information to the locations*/
				X2AP_MEMCPY(&p_x2ap_gb_ctx->group_ids.group_id_arr[gu_id_counter],&p_x2ap_gb_ctx->group_ids.group_id_arr[p_x2ap_gb_ctx->group_ids.num_gu_group_id-1] , sizeof(x2ap_gu_group_id_t));

				/* Decrement number of GU group id*/
				p_x2ap_gb_ctx->group_ids.num_gu_group_id--;
			}
		}

		if(gu_id_found == X2AP_FAILURE)
		{
			X2AP_TRACE(X2AP_ERROR, "%u GU group id in the list not found.\n", gu_id_count+1);
		}
	}
}

/*****************************************************************************
 * Function Name  : x2ap_process_served_cell_to_delete_list 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_served_cell - pointer to x2ap_served_cells_to_delete_list_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function deletes served cell with cell_id info from 
 *                  global context
 ********************************************************************************/
void x2ap_process_served_cell_to_delete_list(x2ap_gb_context_t* p_x2ap_gb_ctx, x2ap_served_cells_to_delete_list_t *p_served_cell)
{
	U8 cell_count = 0;

	X2AP_TRACE(X2AP_INFO, "%u serving cell to delete present\n", p_served_cell->num_served_cell);

	/* Coverity Fix 81605 Start */
	for (cell_count = 0; ( cell_count < p_served_cell->num_served_cell ) &&
    		(cell_count < MAX_NUM_CELL_IN_X2AP) ; cell_count++)
	{
	    if ( p_served_cell->cell_id_list[cell_count] < MAX_NUM_CELL_IN_X2AP )
	    {
		X2AP_TRACE(X2AP_INFO, "Delete served cell with cell_id %u info from global context.\n", p_served_cell->cell_id_list[cell_count]);
                /* delete neighbour cell */
		x2ap_delete_nbour_cell_info(p_x2ap_gb_ctx, p_served_cell->cell_id_list[cell_count]);
		/* delete neighbour nr cell */
		x2ap_delete_nbour_nr_cell_info(p_x2ap_gb_ctx, p_served_cell->cell_id_list[cell_count]);
		X2AP_MEMSET(&p_x2ap_gb_ctx->served_cell_info[p_served_cell->cell_id_list[cell_count]], 0x00, sizeof(x2ap_cell_info_t));
	    }
	    else
	    {
		X2AP_TRACE(X2AP_ERROR, "Invalid Cell_ID :%u\n", p_served_cell->cell_id_list[cell_count]);
	    }
	    /* Coverity Fix 81605 Stop */
	    p_x2ap_gb_ctx->num_served_cells--;
	}
}

/*****************************************************************************
 * Function Name  : x2ap_process_served_cell_to_modify_list 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_served_cell - pointer to x2ap_served_cells_to_modify_list_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function resets the served cell Id in the global context
 ********************************************************************************/
void x2ap_process_served_cell_to_modify_list(x2ap_gb_context_t* p_x2ap_gb_ctx, x2ap_served_cells_to_modify_list_t *p_served_cell)
{
	U8 cell_count = 0;
	x2ap_cell_info_t* p_target = X2AP_P_NULL;
	x2ap_served_cell_info_t* p_source = X2AP_P_NULL;

	X2AP_TRACE(X2AP_INFO, "%u serving cell to modify present\n", p_served_cell->num_served_cell);

	/* Coverity Fix 81512 Start */
	for (cell_count = 0; (cell_count < p_served_cell->num_served_cell) &&
		(cell_count < MAX_NUM_CELL_IN_X2AP); cell_count++)
	{
	    p_source = &p_served_cell->cell_list[cell_count].served_cell_info.cell_info;
	    p_target = &p_x2ap_gb_ctx->served_cell_info[p_source->cell_id];

	    if ( p_served_cell->cell_list[cell_count].old_cell_id < MAX_NUM_CELL_IN_X2AP )
	    { 
		/*SPR 21554 Start*/
		/*Code Deleted*/
        /*SPR 21554 End*/
        /* Update Served cell info in Global eNB Context */
		x2ap_populate_served_cell_info(p_x2ap_gb_ctx, p_target, p_source);
		/*p_target->num_nbour = p_served_cell->cell_list[cell_count].served_cell_info.nbour_cell_info.num_neighbour_cell;*/

		if(p_served_cell->cell_list[cell_count].served_cell_info.nbour_cell_info.num_neighbour_cell==0)
		{
		    /*Bug_11608_fix_start*/
		    X2AP_TRACE(X2AP_INFO, "cell_count=%u No neighbour cell present for serving cell with cell_index:%u\n", cell_count, p_source->cell_id);
		    /*Bug_11608_fix_stop*/
		}
		else
		{
		    U16 nbour_count = 0;
		    x2ap_nbour_cell_info_t *p_nbour_cell_info = &p_served_cell->cell_list[cell_count].served_cell_info.nbour_cell_info;

		    /*Bug_11608_fix_start*/
		    X2AP_TRACE(X2AP_INFO, "%u neighbour cell present for serving cell with cell_index:%u\n", cell_count, p_source->cell_id);
		    /*Bug_11608_fix_stop*/

		    /* Populate neighbour cell info */
		    for (nbour_count = 0; nbour_count < p_nbour_cell_info->num_neighbour_cell; ++nbour_count)
		    {
			x2ap_populate_nbour_cell_info(p_x2ap_gb_ctx, &p_nbour_cell_info->neighbour_info_arr[nbour_count], p_source->cell_id, NULL);
		    }
		}
	    }
	    else
	    {
		   	X2AP_TRACE(X2AP_ERROR, "Invalid old cell_id:%u\n", p_served_cell->cell_list[cell_count].old_cell_id);
	    }
		/* Coverity Fix 81512 Stop */
    }
}

/*****************************************************************************
 * Function Name  : x2ap_process_served_cell_to_add_list 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_served_cell-  pointer to x2ap_served_cells_to_add_list_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function addds the served cell Id to global context  
 ********************************************************************************/
void x2ap_process_served_cell_to_add_list(x2ap_gb_context_t* p_x2ap_gb_ctx, x2ap_served_cells_to_add_list_t *p_served_cell)
{
	U8 cell_count = 0;

	X2AP_TRACE(X2AP_INFO, "%u existing serving cells\n", p_x2ap_gb_ctx->num_served_cells);
	X2AP_TRACE(X2AP_INFO, "%u serving cell to add present\n", p_served_cell->num_served_cell);

	for (cell_count = 0; cell_count < p_served_cell->num_served_cell; ++cell_count)
	{
		x2ap_cell_info_t* p_target = X2AP_P_NULL;
		x2ap_served_cell_info_t* p_source = X2AP_P_NULL;

		p_source = &p_served_cell->cell_list[cell_count].cell_info;
		p_target = &p_x2ap_gb_ctx->served_cell_info[p_source->cell_id];

		/* Initialise active served cell info in Global eNB Context */
        /* Bug 4644 start */
        /* Code Removed */
        /* Bug 4644 end */
		/*SPR 21554 +-*/
		/* Update Served cell info in Global eNB Context */
		x2ap_populate_served_cell_info(p_x2ap_gb_ctx, p_target, p_source);
		/*p_target->num_nbour = p_served_cell->cell_list[cell_count].nbour_cell_info.num_neighbour_cell;*/
		p_x2ap_gb_ctx->num_served_cells++;

		if(p_served_cell->cell_list[cell_count].nbour_cell_info.num_neighbour_cell==0)
		{
			X2AP_TRACE(X2AP_INFO, "No neighbour cell present for %u serving cell with cell id:%u\n", cell_count, p_source->cell_id);
		}
		else
		{
			U16 nbour_count = 0;
			x2ap_nbour_cell_info_t *p_nbour_cell_info = &p_served_cell->cell_list[cell_count].nbour_cell_info;

			X2AP_TRACE(X2AP_INFO, "%u neighbour cell present for %u serving cell with cell id:%u\n", 
					p_target->num_nbour, cell_count, p_source->cell_id);

			/* Populate neighbour cell info */
			for (nbour_count = 0; nbour_count < p_nbour_cell_info->num_neighbour_cell; ++nbour_count)
			{
				x2ap_populate_nbour_cell_info(p_x2ap_gb_ctx, &p_nbour_cell_info->neighbour_info_arr[nbour_count], p_source->cell_id, NULL);
			}
		}
	}
}
/*****************************************************************************
 * Function Name  : x2ap_apply_enb_config_update_req 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cfg_req - pointer to x2ap_enb_config_update_req_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function applies the eNB configuraton update request 
 ********************************************************************************/
void x2ap_apply_enb_config_update_req(x2ap_gb_context_t* p_x2ap_gb_ctx, x2ap_enb_config_update_req_t *p_enb_cfg_req)
{
	if(p_enb_cfg_req->bitmask &X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT)
	{
		X2AP_TRACE(X2AP_INFO, "Served cells to add list present.\n");
		x2ap_process_served_cell_to_add_list(p_x2ap_gb_ctx, &p_enb_cfg_req->add_served_cell);
	}
	else
	{
		X2AP_TRACE(X2AP_INFO, "Served cells to add list not present.\n");
	}
	if(p_enb_cfg_req->bitmask &X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT)
	{
		X2AP_TRACE(X2AP_INFO, "Served cells to modify list present.\n");
		x2ap_process_served_cell_to_modify_list(p_x2ap_gb_ctx, &p_enb_cfg_req->mod_served_cell);
	}
	else
	{
		X2AP_TRACE(X2AP_INFO, "Served cells to modify list not present.\n");
	}
	if(p_enb_cfg_req->bitmask &X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT)
	{
		X2AP_TRACE(X2AP_INFO, "Served cells to delete list present.\n");
		x2ap_process_served_cell_to_delete_list(p_x2ap_gb_ctx, &p_enb_cfg_req->del_served_cell);
	}
	else
	{
		X2AP_TRACE(X2AP_INFO, "Served cells to delete list not present.\n");
	}
	if(p_enb_cfg_req->bitmask &X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
	{
		X2AP_TRACE(X2AP_INFO, "GU group id to add list present.\n");
		x2ap_process_gu_group_id_to_add_list(p_x2ap_gb_ctx, &p_enb_cfg_req->add_gu_id);
	}
	else
	{
		X2AP_TRACE(X2AP_INFO, "GU group id to add list not present.\n");
	}
	if(p_enb_cfg_req->bitmask &X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
	{
		X2AP_TRACE(X2AP_INFO, "GU group id to delete list present.\n");
		x2ap_process_gu_group_id_to_delete_list(p_x2ap_gb_ctx, &p_enb_cfg_req->delete_gu_id);
	}
	else
	{
		X2AP_TRACE(X2AP_INFO, "GU group id to delete list not present.\n");
	}
	if(p_enb_cfg_req->bitmask & X2AP_NEIGH_ECGI_ENB_ID_MAPPING_PRESENT)
	{
		X2AP_TRACE(X2AP_INFO, "Neigh ECGI-ENB ID mapping list present.\n");
		x2ap_process_neigh_ecgi_enb_id_mapping(p_x2ap_gb_ctx, 
				&p_enb_cfg_req->neigh_ecgi_enb_id_map);
	}
	else
	{
		X2AP_TRACE(X2AP_INFO, "Neigh ECGI-ENB ID mapping list not present.\n");
	}
}
#ifdef LTE_EMBMS_SUPPORTED
/*****************************************************************************
 * Function Name  : x2ap_process_enb_config_update_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  	p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *                  dest_module_id - destination id
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the enb config update request from
 *                  RRM/SON
 ****************************************************************************/
void x2ap_process_updated_msbms_sf_info_ind(void* p_api, x2ap_gb_context_t* 
		p_x2ap_gb_ctx)
{
    rrm_updated_mbms_sf_info_t                  rrm_updated_mbms_sf_info;
    /* COV_94361_FIX_START */
    x2ap_enb_config_update_req_t                *p_enb_cfg_req;
    /* COV_94361_FIX_END */
    U8            response                            = X2AP_FAILURE;
    U8            asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16           asn_msg_len                         = X2AP_NULL;
    S32           length_read                         = X2AP_NULL;
    /* COV_94406_FIX_START */
    /* Code Deleted */
    /* COV_94406_FIX_END */
    U8            enb_count                           = X2AP_ZERO;
    U8            max_enb_count                       = X2AP_ZERO;
    U8            enb_found                           = X2AP_FAILURE;
    x2ap_peer_enb_context_t*           p_peer_enb_ctx = X2AP_P_NULL;
    U8            reset_ongoing                       = X2AP_FALSE;

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);
    X2AP_MEMSET(&rrm_updated_mbms_sf_info, X2AP_NULL, 
            sizeof(rrm_updated_mbms_sf_info_t));

    /* COV_94361_FIX_START */
    p_enb_cfg_req = (x2ap_enb_config_update_req_t*) 
        x2ap_mem_get(sizeof(x2ap_enb_config_update_req_t));

    if( X2AP_P_NULL == p_enb_cfg_req)
    {
        X2AP_TRACE(X2AP_WARNING, "x2ap enb config update request"
                " Memory allocation failed");
        return;
    }

    X2AP_MEMSET(p_enb_cfg_req, X2AP_NULL, 
            sizeof(x2ap_enb_config_update_req_t));
    /* COV_94361_FIX_END */
    /* Decode the received message from RRM */
     response = rrc_il_parse_rrm_updated_mbms_sf_info(
       &rrm_updated_mbms_sf_info,
       (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
       x2ap_get_api_buf_size(p_api) -
       RRC_FULL_INTERFACE_HEADERS_SIZE,
       &length_read); 

    if (X2AP_SUCCESS == response)
    {
        /*   Update the Global context with received information 
         * and fill ECU structure from global context */
        response = x2ap_apply_updated_mbms_sf_info_ind(p_x2ap_gb_ctx,
                &rrm_updated_mbms_sf_info,
                /* COV_94361_FIX_START */
                p_enb_cfg_req);
        /* COV_94361_FIX_END */
        if (X2AP_FAILURE == response)
        {
            X2AP_TRACE(X2AP_ERROR, "Fill updated mbms_sf_info failed");
            /* COV_94361_FIX_START */
            if( X2AP_P_NULL != p_enb_cfg_req)
            {
                x2ap_mem_free(p_enb_cfg_req);
            }
            /* COV_94361_FIX_END */
            return ;
        }
    }
    else
    {
        X2AP_TRACE(X2AP_WARNING, "x2ap enb config update request"
                " parsing failed");
        /* COV_94361_FIX_START */
        if( X2AP_P_NULL != p_enb_cfg_req)
        {
            x2ap_mem_free(p_enb_cfg_req);
        }
        /* COV_94361_FIX_END */
        return;
    }
    /* COV_94361_FIX_START */
    if(p_enb_cfg_req->num_peer_enb != X2AP_ZERO)
    {
/* SPR 18736 changes start */
        for(enb_count =0; enb_count<p_enb_cfg_req->num_peer_enb; enb_count++)
        {
            if(p_x2ap_gb_ctx->x2ap_link_info[enb_count].connection_status == X2AP_LINK_UP)
            {
                    X2AP_TRACE(X2AP_INFO,"X2 link available with peer eNB %d",enb_count+1);
                    break;
            }
        }
        if (enb_count == p_enb_cfg_req->num_peer_enb)
        {
            /* COV_94361_FIX_START */
            if( X2AP_P_NULL != p_enb_cfg_req)
            {
                x2ap_mem_free(p_enb_cfg_req);
            }
            /* COV_94361_FIX_END */
            return;
        }
/* SPR 18736 changes end */
        if(!(x2ap_encode_eNB_Configuration_Update_req(p_x2ap_gb_ctx, 
                        p_enb_cfg_req, asn_msg_buff, &asn_msg_len)))
        {
            X2AP_TRACE(X2AP_ERROR,"Encoding eNB Configuration"
                    " Update failed");
            /* COV_94361_FIX_START */
            if( X2AP_P_NULL != p_enb_cfg_req)
            {
                x2ap_mem_free(p_enb_cfg_req);
            }
            /* COV_94361_FIX_END */
            return;
        }

        for(enb_count =0; enb_count<p_enb_cfg_req->num_peer_enb; enb_count++)
        {
            for(max_enb_count=0; max_enb_count<MAX_PEER_ENB &&
                    enb_found!=X2AP_SUCCESS; max_enb_count++)
            {
                enb_found = X2AP_FAILURE;
                reset_ongoing = X2AP_FALSE;
                if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[max_enb_count].
                            op_status == OPERATIONAL) &&
                        (!(X2AP_MEMCMP(&(p_enb_cfg_req->enb_info[enb_count]), 
                                       &p_x2ap_gb_ctx->
                                       peer_enb_cntxt_arr[max_enb_count].
                                       gb_enb_id, sizeof(x2_gb_enb_id_t)))))
                    /* COV_94361_FIX_END */

                {
                    if(p_x2ap_gb_ctx->peer_enb_cntxt_arr[max_enb_count].
                            x2ap_reset_sm_ctx.state == 
                            X2_RESET_W_RESET_RESP_ST)
                    {
                        reset_ongoing = X2AP_TRUE;		
                        break;
                    }       
                    else
                    {
                        enb_found = X2AP_SUCCESS;
                        p_peer_enb_ctx = &p_x2ap_gb_ctx->
                            peer_enb_cntxt_arr[max_enb_count];
                        break;
                    }
                }
            }
            if(enb_found == X2AP_FAILURE)
            {
                if (reset_ongoing == X2AP_TRUE)
                {
                    X2AP_TRACE(X2AP_ERROR,"Reset is ongoing"
                            " with the peer eNB id %u\n", enb_count+1);
                }
                else
                {
                    X2AP_TRACE(X2AP_ERROR,"Peer eNB %u in the list"
                            " is not operational\n", enb_count+1);
                }

            }
            else
            {
                switch(p_peer_enb_ctx->enb_config_sm_ctx.state)
                {
                    case X2AP_ENB_CONFIG_ACTIVE_ST:
                    {
                        X2AP_TRACE(X2AP_INFO,"Sending eNB Configuration"
                                " Update to Peer eNB %u in the list\n",
                                enb_count+1);

                        /* Send eNB CONFIGURATION UPDATE to peer eNB */
                        if(!(x2ap_send_enb_config_update_req_peer(
                                        p_x2ap_gb_ctx, p_peer_enb_ctx,
                                        asn_msg_buff, asn_msg_len,
                                        X2AP_ENB_CONFIG_UPDATE_RSP_TIMER)))
                        {
                            X2AP_TRACE(X2AP_ERROR,"Sending eNBid"
                                    " Configuration Update failed");
                            /* COV_94361_FIX_START */
                            if( X2AP_P_NULL != p_enb_cfg_req)
                            {
                                x2ap_mem_free(p_enb_cfg_req);
                            }
                            /* COV_94361_FIX_END */
                            return;
                        }
                        else
                        {
                            /* Save eNB Configuration Update request */
                            /* COV_94361_FIX_START */
                            X2AP_MEMCPY(&p_peer_enb_ctx->
                                    enb_config_sm_ctx.enb_config_req,
                                    p_enb_cfg_req,sizeof(
                                        x2ap_enb_config_update_req_t));
                            /* COV_94361_FIX_END */

                            p_peer_enb_ctx->enb_config_sm_ctx.req_retry_count = 
                                p_x2ap_gb_ctx->config_param.timer_conf_info.
                                eNB_Config_Update_retry_cnt;

                            p_peer_enb_ctx->enb_config_sm_ctx.
                                time_to_wait_retry_count =
                                p_x2ap_gb_ctx->config_param.timer_conf_info.
                                eNB_Config_Update_time_to_wait_retry_cnt;

/* SPR 18736 changes start */
                           p_peer_enb_ctx->enb_config_sm_ctx.dst_module_id =  RRC_RRM_MODULE_ID;
/* SPR 18736 changes end */
                        }
                    }break;
                    case X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST:
                    {
                        /* Save eNB Configuration Update request */
                        /* COV_94361_FIX_START */
                        X2AP_MEMCPY(&p_peer_enb_ctx->enb_config_sm_ctx.
                                enb_config_req, p_enb_cfg_req, 
                                sizeof(x2ap_enb_config_update_req_t));
                        /* COV_94361_FIX_END */

                    }break;
                    case X2AP_ENB_CONFIG_IDLE_ST:
                    case X2AP_ENB_CONFIG_W_CONF_RESP_ST:
                    {
                        X2AP_TRACE(X2AP_ERROR,"Invalid state: %u",
                                p_peer_enb_ctx->enb_config_sm_ctx.state);

                    }break;
                }
            }
        }
    }
    /* COV_94361_FIX_START */
    if( X2AP_P_NULL != p_enb_cfg_req)
    {
        x2ap_mem_free(p_enb_cfg_req);
    }
    /* COV_94361_FIX_END */
    X2AP_UT_TRACE_EXIT();
}

/*****************************************************************************
* Function Name  : x2ap_apply_updated_mbms_sf_info_ind
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *                  p_rrm_updated_mbms_sf_info - pointer to 
 *                  rrm_updated_mbms_sf_info_t
 *                  p_enb_cfg_req - pointer to x2ap_enb_config_update_req_t
 * Outputs        : None
 * Returns        : none
 * Description    : This function updates the global context and fill the 
 *                  enb_cfg_update request stucture
 ****************************************************************************/
U8 x2ap_apply_updated_mbms_sf_info_ind
(
    x2ap_gb_context_t            *p_x2ap_gb_ctx, 
    rrm_updated_mbms_sf_info_t   *p_rrm_updated_mbms_sf_info,
     x2ap_enb_config_update_req_t *p_enb_cfg_req
)
{   
    U8 num_cells = X2AP_NULL;
    /* COV_94333_FIX_START */
    /* Code deleted */
    /* COV_94333_FIX_END */
    U8 counter = X2AP_ZERO;
    /* COV_94131_FIX_START */
    /* Code deleted */
    /* COV_94131_FIX_END */
    U8 no_cells = X2AP_ZERO;
/* SPR 18736 changes start */
    U8 cell_id = X2AP_ZERO;
/* SPR 18736 changes end */
    X2AP_UT_TRACE_ENTER();
    
    X2AP_ASSERT (X2AP_P_NULL != p_rrm_updated_mbms_sf_info);
    
    num_cells = p_rrm_updated_mbms_sf_info->num_cells;
    
    if (num_cells == X2AP_ZERO)
    {
      X2AP_TRACE(X2AP_ERROR,"RRM SF info indication received from RRM"  
                         " With Zero Cell list");
      return X2AP_FAILURE;
    }
    /* COV_94131_FIX_START */
/* SPR 18736 changes start */
    for (counter = X2AP_ZERO; counter < num_cells; counter++)
        /* COV_94131_FIX_END */
    {
        x2ap_convert_cell_id_from_ecgi(p_rrm_updated_mbms_sf_info->cell_info[counter].cell_id.eutran_cell_id, &cell_id);
        /*UT Fix Start */
        if (cell_id == p_x2ap_gb_ctx->act_cell_info[counter].cell_id && 
              X2AP_TRUE == p_x2ap_gb_ctx->act_cell_info[counter].is_cell_active)
        {
            if (p_rrm_updated_mbms_sf_info->cell_info[counter].bitmask & 
                    RRM_X2AP_MBMS_CONFIG_APPLY_PRESENT)
                /*UT Fix Start */
            {
                /* COV_94286_FIX_START */
                X2AP_MEMCPY(p_x2ap_gb_ctx->served_cell_info[cell_id].
                        mbsfn_subframe_info.mbsfn_info, 
                        p_rrm_updated_mbms_sf_info->
                        cell_info[counter].mbsfn_subframe_info.
                        mbsfn_info, MAX_NUM_MBSFN);
                /* COV_94286_FIX_END */

                p_x2ap_gb_ctx->served_cell_info[cell_id].
                    mbsfn_subframe_info.num_mbsfn = 
                    p_rrm_updated_mbms_sf_info->
                    cell_info[counter].mbsfn_subframe_info.num_mbsfn;

                p_x2ap_gb_ctx->served_cell_info[cell_id].bitmask |=
                    X2AP_MBSFN_SUBFRAME_INFO_PRESENT;
                fill_enb_configuration_update_for_peer_enbs(p_x2ap_gb_ctx,
                        p_enb_cfg_req, &no_cells,cell_id);
            }
            else
            {
                X2AP_MEMSET(&(p_x2ap_gb_ctx->served_cell_info[cell_id].
                            mbsfn_subframe_info), X2AP_ZERO, 
                        sizeof(x2ap_mbsfn_subframe_info_t));
                p_x2ap_gb_ctx->served_cell_info[cell_id].bitmask = 
                    X2AP_ZERO;
            }
        }   
        else
        {
            X2AP_TRACE(X2AP_ERROR,"Received cell id:%d is not active in system",cell_id);  
        }
        p_enb_cfg_req->mod_served_cell.num_served_cell = no_cells;
        /* Setting the bitmask for modified served cells */
        p_enb_cfg_req->bitmask |= X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT;

        for (counter = X2AP_ZERO; counter < p_x2ap_gb_ctx->num_peer_enb;
                counter++)
        {
            p_enb_cfg_req->num_peer_enb =  p_x2ap_gb_ctx->num_peer_enb;

            /* COV_94507_FIX_START */
            X2AP_MEMCPY(&p_enb_cfg_req->enb_info[counter].plmn_identity.plmn_id,
                    p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].gb_enb_id.
                    plmn_identity.plmn_id, MAX_PLMN_ID_BYTES);   
            /* COV_94507_FIX_END */

            X2AP_MEMCPY(&p_enb_cfg_req->enb_info[counter].enb_id,
                    &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].gb_enb_id.
                    enb_id, sizeof (x2_enb_id_info_t));   
        }
    }
/* SPR 18736 changes start */
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/* SPR 18736 changes start */
void x2ap_convert_cell_id_from_ecgi(/*x2_gb_enb_id_t global_enb_id,*/ U8* eutran_cell_id, U8* p_cell_id)
{
    U8 local_eutran_cell_id[HOME_ENB_ID_OCTET_SIZE];
    X2AP_MEMCPY(local_eutran_cell_id, eutran_cell_id, HOME_ENB_ID_OCTET_SIZE);

    *p_cell_id = (local_eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE] & 0xf0) >> 4;
    *p_cell_id |= (local_eutran_cell_id[MACRO_ENB_ID_OCTET_SIZE-1] & 0xf) << 4;
    return;
}
/* SPR 18736 changes end */
/*****************************************************************************
 * Function Name  : fill_enb_configuration_update_for_peer_enbs
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *                  p_enb_cfg_req - pointer to x2ap_enb_config_update_req_t
 * Outputs        : p_no_cells- to store the count of served cells in ECU
 * Returns        : none
 * Description    : create and fill the ECU structure from global for ASN
 *                  encoding 
 ****************************************************************************/
void fill_enb_configuration_update_for_peer_enbs(
		x2ap_gb_context_t            *p_x2ap_gb_ctx, 
		x2ap_enb_config_update_req_t *p_enb_cfg_req,
    /* COV_94224_FIX_START */
                U8 *p_no_cells,
                U8 gb_ctx_index
)
{
    U8 cell_id = X2AP_ZERO;
    U16 neigh_count = X2AP_ZERO;
      /* Filling PCI in ECU */
    X2AP_UT_TRACE_ENTER();

	p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].
		served_cell_info.cell_info.pci = p_x2ap_gb_ctx->
		served_cell_info[gb_ctx_index].pci; 
      /* Filling cell_id in ECU */
/* SPR 18736 changes Start */
	p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].
		served_cell_info.cell_info.cell_id = gb_ctx_index;
/* SPR 18736 changes End */
       
      /* Filing old cell_id */
	p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].
		old_cell_id = cell_id;

      /* Filling tracking area code in ECU */
	X2AP_MEMCPY(&p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].
			served_cell_info.cell_info.tac, p_x2ap_gb_ctx->
			served_cell_info[gb_ctx_index].track_area_code.tac,
/* COV_94241_FIX_START */
            MAX_TAC_BYTES);
/* COV_94241_FIX_END */

      /* Filling PLMN identity  in ECU */
	X2AP_MEMCPY(&p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].
			served_cell_info.cell_info.bcast_plmn_info.plmn_identity,
            p_x2ap_gb_ctx->served_cell_info[gb_ctx_index].
            broadcast_plmn_info.plmn_identity,
/* COV_94241_FIX_START */
			MAX_NUM_BPLMN);
/* COV_94241_FIX_END */
	p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].
        served_cell_info.cell_info.bcast_plmn_info.num_bcast_plmn_id = p_x2ap_gb_ctx->
        served_cell_info[gb_ctx_index].broadcast_plmn_info.
        num_bcast_plmn_id;

	X2AP_MEMCPY( (void*)(&(p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].
					served_cell_info.cell_info.choice_eutra_mode)),
			(const void* ) (&(p_x2ap_gb_ctx->
					served_cell_info[gb_ctx_index].choice_eutra_mode)),
			sizeof(x2ap_choice_eutra_mode_info_t));

/* SPR 18736 changes Start */
    p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.bitmask = 0x01;
/* SPR 18736 changes End */

	X2AP_MEMCPY((void*)&(p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].
				served_cell_info.cell_info.mbsfn_subframe_info),
			(const void* )&(p_x2ap_gb_ctx->
				served_cell_info[gb_ctx_index].mbsfn_subframe_info),
			sizeof(x2ap_mbsfn_subframe_info_t));

	p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].
		served_cell_info.cell_info.bitmask |= 
		X2AP_MBSFN_SUBFRAME_INFO_PRESENT;

		/* Fill neighbour cell info */

		p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].
		served_cell_info.nbour_cell_info.num_neighbour_cell = p_x2ap_gb_ctx->
		num_peer_enb_cells;
     /* COV_94377_FIX_START */
	for (neigh_count = X2AP_ZERO; neigh_count<p_x2ap_gb_ctx->num_peer_enb_cells;
                                             neigh_count++)
     /* COV_94377_FIX_END */
	{
		/* Copy ECGI */
		X2AP_MEMCPY(&p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].served_cell_info.
					nbour_cell_info.neighbour_info_arr[neigh_count].ecgi, 
					&p_x2ap_gb_ctx->peer_enb_cell_info[neigh_count].
					peer_cell_info.ecgi,
				sizeof(x2ap_ecgi_t));

		/* Copy PCI */
		X2AP_MEMCPY(&p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].served_cell_info.
					nbour_cell_info.neighbour_info_arr[neigh_count].pci, 
					&p_x2ap_gb_ctx->peer_enb_cell_info[neigh_count].
					peer_cell_info.pci,
				sizeof(U32));

		/* Copy EARFCN */
		X2AP_MEMCPY(&p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].served_cell_info.
					nbour_cell_info.neighbour_info_arr[neigh_count].earfcn, 
					&p_x2ap_gb_ctx->peer_enb_cell_info[neigh_count].
					peer_cell_info.earfcn,
				sizeof(x2ap_earfcn_t));

               p_enb_cfg_req->mod_served_cell.cell_list[*p_no_cells].served_cell_info.
               nbour_cell_info.num_neighbour_cell = neigh_count;
	}
	(*p_no_cells)++; 
    /* COV_94224_FIX_END */
    X2AP_UT_TRACE_EXIT();
}                         
#endif
/*****************************************************************************
 * Function Name  : x2ap_process_enb_config_update_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  	p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *                  dest_module_id - destination id
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the enb config update request from RRM/SON
 ****************************************************************************/
void x2ap_process_enb_config_update_req(void* p_api, x2ap_gb_context_t* p_x2ap_gb_ctx, U16 dest_module_id)
{
	x2ap_return_et response = X2AP_SUCCESS;
	static x2ap_enb_config_update_req_t enb_cfg_req;
	x2ap_enb_config_update_resp_t enb_cfg_resp;
	U8 asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
	U16 asn_msg_len = X2AP_NULL;
	S32 length_read = X2AP_NULL;
	U16 trans_id = X2AP_NULL;

	X2AP_UT_TRACE_ENTER();

	X2AP_ASSERT(X2AP_P_NULL != p_api);
	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

	/* Get the transaction id */
	trans_id = x2ap_get_transaction_id(p_api);

	X2AP_MEMSET(&enb_cfg_req, 0, sizeof(x2ap_enb_config_update_req_t));

	/* Decode the received message from OAM/SON */
	response = rrc_parse_x2ap_enb_config_update_req(
			&enb_cfg_req,
			(U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
			x2ap_get_api_buf_size(p_api) -
			RRC_FULL_INTERFACE_HEADERS_SIZE,
			&length_read);

	if (X2AP_SUCCESS == response)
	{
		if(enb_cfg_req.bitmask ==0)
		{
			U8 response_count =0;
			X2AP_TRACE(X2AP_WARNING, "enb config update request incorrect bitmask 0 received.\n");
			for(response_count =0; response_count<enb_cfg_req.num_peer_enb; response_count++)
			{
				X2AP_MEMCPY(&enb_cfg_resp.enb2_gb_id, &enb_cfg_req.enb_info[response_count] , sizeof(x2_gb_enb_id_t));

				enb_cfg_resp.cause.type = T_x2ap_Cause_radioNetwork;
				enb_cfg_resp.cause.value = x2ap_unspecified_2;

				/* Send eNB Configuration Update response */
				x2ap_send_enb_config_update_response(p_x2ap_gb_ctx, &enb_cfg_resp, X2AP_FAILURE, dest_module_id, trans_id);
			}
		}
		else
		{
			U8 enb_count =0;
			U8 max_enb_count =0;
			U8 enb_found =X2AP_FAILURE;
			x2ap_peer_enb_context_t* p_peer_enb_ctx = X2AP_P_NULL;
			U8 reset_ongoing = X2AP_FALSE;

			/* Apply eNB Configuration Update message*/
			x2ap_apply_enb_config_update_req(p_x2ap_gb_ctx, &enb_cfg_req);

			if(enb_cfg_req.num_peer_enb != 0)
			{
				/* Encode eNB Configuration Update message*/
				if(!(x2ap_encode_eNB_Configuration_Update_req(p_x2ap_gb_ctx, &enb_cfg_req, asn_msg_buff, &asn_msg_len)))
				{
					X2AP_TRACE(X2AP_ERROR,"Encoding eNB Configuration Update failed");
					return;
				}
			}

			/* Fetch peer eNB context */
			for(enb_count =0; enb_count<enb_cfg_req.num_peer_enb; enb_count++)
			{
			       enb_found = X2AP_FAILURE;
				reset_ongoing = X2AP_FALSE;
				
				for(max_enb_count=0; max_enb_count<MAX_PEER_ENB && enb_found!=X2AP_SUCCESS; max_enb_count++)
				{
					//enb_found = X2AP_FAILURE;
					//reset_ongoing = X2AP_FALSE;
					if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[max_enb_count].op_status == OPERATIONAL) &&
							(!(X2AP_MEMCMP(&enb_cfg_req.enb_info[enb_count], &p_x2ap_gb_ctx->peer_enb_cntxt_arr[max_enb_count].gb_enb_id, sizeof(x2_gb_enb_id_t)))))
					{
						if(p_x2ap_gb_ctx->peer_enb_cntxt_arr[max_enb_count].x2ap_reset_sm_ctx.state == X2_RESET_W_RESET_RESP_ST)
						{
							reset_ongoing = X2AP_TRUE;		
							break;
						}       
						else
						{
							enb_found = X2AP_SUCCESS;
							p_peer_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[max_enb_count];
							break;
						}
					}
				}

				if(enb_found == X2AP_FAILURE)
				{
					if (reset_ongoing == X2AP_TRUE)
					{
						X2AP_TRACE(X2AP_ERROR,"Reset is ongoing with the peer eNB id %u\n", enb_count+1);
					}
					else
					{
						X2AP_TRACE(X2AP_ERROR,"Peer eNB %u in the list is not operational\n", enb_count+1);
					}

					enb_cfg_resp.cause.type = T_x2ap_Cause_radioNetwork;
					enb_cfg_resp.cause.value = x2ap_unspecified_2;

					X2AP_MEMCPY(&enb_cfg_resp.enb2_gb_id, &enb_cfg_req.enb_info[enb_count], sizeof(x2_gb_enb_id_t));

					/* Send eNB Configuration Update response */
					x2ap_send_enb_config_update_response(p_x2ap_gb_ctx, &enb_cfg_resp, X2AP_FAILURE, dest_module_id, trans_id);
				}
				else
				{
					switch(p_peer_enb_ctx->enb_config_sm_ctx.state)
					{
						case X2AP_ENB_CONFIG_ACTIVE_ST:
							{
								X2AP_TRACE(X2AP_INFO,"Sending eNB Configuration Update to Peer eNB %u in the list\n", enb_count+1);

								/* Send eNB CONFIGURATION UPDATE to peer eNB */
								if(!(x2ap_send_enb_config_update_req_peer(p_x2ap_gb_ctx, p_peer_enb_ctx, asn_msg_buff, asn_msg_len, X2AP_ENB_CONFIG_UPDATE_RSP_TIMER)))
								{
									X2AP_TRACE(X2AP_ERROR,"Sending eNB Configuration Update failed");
									return;
								}
								else
								{
									/* Save eNB Configuration Update request */
									X2AP_MEMCPY(&p_peer_enb_ctx->enb_config_sm_ctx.enb_config_req, &enb_cfg_req, sizeof(x2ap_enb_config_update_req_t));

									p_peer_enb_ctx->enb_config_sm_ctx.req_retry_count = 
										p_x2ap_gb_ctx->config_param.timer_conf_info.eNB_Config_Update_retry_cnt;

									p_peer_enb_ctx->enb_config_sm_ctx.time_to_wait_retry_count =
										p_x2ap_gb_ctx->config_param.timer_conf_info.eNB_Config_Update_time_to_wait_retry_cnt;

									p_peer_enb_ctx->enb_config_sm_ctx.transaction_id = trans_id;

									p_peer_enb_ctx->enb_config_sm_ctx.dst_module_id = dest_module_id;
								}
							}break;
						case X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST:
							{
								/* Save eNB Configuration Update request */
								X2AP_MEMCPY(&p_peer_enb_ctx->enb_config_sm_ctx.enb_config_req, &enb_cfg_req, sizeof(x2ap_enb_config_update_req_t));
							}break;
						case X2AP_ENB_CONFIG_IDLE_ST:
						case X2AP_ENB_CONFIG_W_CONF_RESP_ST:
							{
								X2AP_TRACE(X2AP_ERROR,"Invalid state: %u.n", p_peer_enb_ctx->enb_config_sm_ctx.state);

								enb_cfg_resp.cause.type = T_x2ap_Cause_radioNetwork;
								enb_cfg_resp.cause.value = x2ap_unspecified_2;

								X2AP_MEMCPY(&enb_cfg_resp.enb2_gb_id, &enb_cfg_req.enb_info[enb_count], sizeof(x2_gb_enb_id_t));

								/* Send eNB Configuration Update response */
								x2ap_send_enb_config_update_response(p_x2ap_gb_ctx, &enb_cfg_resp, X2AP_FAILURE, dest_module_id, trans_id);
							}break;
					}
				}
			}
		}
	}
	else
	{
		X2AP_TRACE(X2AP_WARNING, "x2ap enb config update request parsing failed\n");
	}
}

/*****************************************************************************
 * Function Name  : x2ap_send_enb_config_update_req_peer 
 * Inputs         : p_x2ap_gb_ctx - pointyer to x2AP global context
 *                  p_peer_enb_ct-  pointer to pext
 *                  p_asn_msg - pointer to asn encoded msg 
 *                  asn_msg_len - pointer to asn encoded msg length 
 *                  timer_type - time id
 * Outputs        : p_asn_msg , asn_msg_len 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function sends the eNB configuration request to the peer
 ********************************************************************************/
x2ap_return_et x2ap_send_enb_config_update_req_peer(x2ap_gb_context_t* p_x2ap_gb_ctx, x2ap_peer_enb_context_t* p_peer_enb_ctx, U8* p_asn_msg, U16 asn_msg_len, x2ap_timer_id_en timer_type)
{
	x2ap_timer_t timer_id;
	x2ap_timer_buffer_t timer_buf;
	x2ap_peer_enb_msg_t  peer_enb_msg;
	U32 timer_value = 0;

	X2AP_MEMSET(&timer_id,X2AP_NULL,sizeof(x2ap_timer_t));
	X2AP_MEMSET(&timer_buf,X2AP_NULL,sizeof(x2ap_timer_buffer_t));
	X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

	peer_enb_msg.p_msg = p_asn_msg;
	peer_enb_msg.msg_len = asn_msg_len;
    /* SPR_14436_Fix Start */
    peer_enb_msg.stream_id = p_peer_enb_ctx->stream_info.
	stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
    /* SPR_14436_Fix Stop */

	if (INVALID_SCTP_SOCKET != p_peer_enb_ctx->sctp_sd)
	{
		if(!(x2ap_sctp_sm_entry(X2AP_SCTP_SEND_DATA_EV, (U8*)&peer_enb_msg, p_x2ap_gb_ctx, p_peer_enb_ctx)))
		{
			X2AP_TRACE(X2AP_INFO,"Sending X2AP_SCTP_SEND_DATA_EV failed\n");
			return X2AP_FAILURE;
		}
	}
	else
	{
		X2AP_TRACE(X2AP_INFO,"Invalid peer eNB SCTP socket descriptor\n");
		return X2AP_FAILURE;
	}

	timer_buf.auto_delete = X2AP_FALSE;
	timer_buf.sd = p_peer_enb_ctx->sctp_sd;
	timer_buf.p_timer_specific_data = (void*)p_peer_enb_ctx;
	timer_buf.timer_type = timer_type;

	if(timer_type == X2AP_ENB_CONFIG_UPDATE_RSP_TIMER)
	{
		timer_value = p_x2ap_gb_ctx->config_param.timer_conf_info.eNB_Config_Update_retry_timer_val;
	}
	else if(timer_type == X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER)
	{
		x2ap_convert_time_to_wait_index_to_duration(p_x2ap_gb_ctx->config_param.timer_conf_info.x2ap_time_to_wait_index,&timer_value);
		timer_value = timer_value + p_x2ap_gb_ctx->config_param.timer_conf_info.eNB_Config_Update_time_to_wait_offset;
	}

	timer_id = x2ap_start_timer(timer_value, &timer_buf, sizeof(timer_buf), X2AP_FALSE);

	if(timer_type == X2AP_ENB_CONFIG_UPDATE_RSP_TIMER)
	{
		p_peer_enb_ctx->enb_config_sm_ctx.state = X2AP_ENB_CONFIG_W_CONF_RESP_ST ;
		p_peer_enb_ctx->enb_config_sm_ctx.req_timer_id = timer_id;
    	X2AP_TRACE(X2AP_INFO,"Started X2AP_ENB_CONFIG_UPDATE_RSP_TIMER with timer_id:%p and duration %u\n", timer_id, timer_value);
		X2AP_TRACE(X2AP_INFO,"enb_config_sm: State changed to X2AP_ENB_CONFIG_W_CONF_RESP_ST\n");
	}
	else if(timer_type == X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER)
	{
		p_peer_enb_ctx->enb_config_sm_ctx.state = X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST ;
		p_peer_enb_ctx->enb_config_sm_ctx.time_to_wait_timer_id = timer_id;
    	X2AP_TRACE(X2AP_INFO,"Started X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER with timer_id:%p and duration %u\n", timer_id, timer_value);
		X2AP_TRACE(X2AP_INFO,"enb_config_sm: State changed to X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST\n");
	}
	return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_populate_served_cell_info 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_x2ap_gb_ctxt_cell_info - pointer to p_x2ap_gb_ctxt_cell_info
 *                  p_cell_info - pointer to x2ap_served_cell_info_t
 * Outputs        : p_x2ap_gb_ctxt_cell_info
 * Returns        : None
 * Description    : This function populates the served cell infomation in 
 *                  global context
 ********************************************************************************/
void x2ap_populate_served_cell_info(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_cell_info_t* p_x2ap_gb_ctxt_cell_info,
		x2ap_served_cell_info_t* p_cell_info)
{
	X2AP_UT_TRACE_ENTER();
		/*SPR 21554 +- */ 
	p_x2ap_gb_ctxt_cell_info->pci = p_cell_info->pci;

	X2AP_MEMCPY( (void*)(&(p_x2ap_gb_ctxt_cell_info->track_area_code)),
			(const void* ) (&(p_cell_info->tac)),
			sizeof(x2ap_tac_t));

	X2AP_MEMCPY( (void*)(&(p_x2ap_gb_ctxt_cell_info->broadcast_plmn_info)),
			(const void* ) (&(p_cell_info->bcast_plmn_info)),
			sizeof(x2ap_broadcast_plmn_t));

	X2AP_MEMCPY( (void*)(&(p_x2ap_gb_ctxt_cell_info->choice_eutra_mode)),
			(const void* ) (&(p_cell_info->choice_eutra_mode)),
			sizeof(x2ap_choice_eutra_mode_info_t));

	if(p_cell_info->bitmask & NUM_ANTENNA_PORT_BITMASK)
	{
		X2AP_TRACE(X2AP_INFO, "Number of antenna port info present\n");

		p_x2ap_gb_ctxt_cell_info->bitmask |= NUM_ANTENNA_PORT_BITMASK;

		p_x2ap_gb_ctxt_cell_info->num_antenna_port =
			p_cell_info->num_antenna_port;
	}
	else
	{
		X2AP_TRACE(X2AP_INFO, "Number of antenna port info not present\n");
	}
	if(p_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
	{
		X2AP_TRACE(X2AP_INFO, "PRACH configuration info present\n");

		p_x2ap_gb_ctxt_cell_info->bitmask |= X2AP_PRACH_CONFIG_INFO_PRESENT;

		X2AP_MEMCPY((void*)&(p_x2ap_gb_ctxt_cell_info->prach_config),
				(const void* )&(p_cell_info->prach_config),
				sizeof(x2ap_prach_configuration_info_t));
	}
	else
	{
		X2AP_TRACE(X2AP_INFO, "PRACH configuration info not present\n");
	}
	if(p_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
	{
		X2AP_TRACE(X2AP_INFO, "MBSFN subframe info present\n");

		p_x2ap_gb_ctxt_cell_info->bitmask |= X2AP_MBSFN_SUBFRAME_INFO_PRESENT;

		X2AP_MEMCPY((void*)&(p_x2ap_gb_ctxt_cell_info->mbsfn_subframe_info),
				(const void* )&(p_cell_info->mbsfn_subframe_info),
				sizeof(x2ap_mbsfn_subframe_info_t));
	}
	else
	{
		X2AP_TRACE(X2AP_INFO, "MBSFN subframe info not present\n");
	}

	/*SPR 10077 Fix Start*/
	if(X2AP_CSG_ID_INFO_PRESENT & p_cell_info->bitmask)
	{
		X2AP_TRACE(X2AP_INFO, "CSG ID info present\n");
		p_x2ap_gb_ctxt_cell_info->bitmask |= X2AP_CSG_ID_INFO_PRESENT;

		X2AP_MEMCPY((void*)&(p_x2ap_gb_ctxt_cell_info->csg_identity),
				(const void* )&(p_cell_info->csg_identity),
				sizeof(p_x2ap_gb_ctxt_cell_info->csg_identity));
	}
	else
	{
		X2AP_TRACE(X2AP_INFO, "CSG ID info not present\n");
	}
	/*SPR 10077 Fix Stop*/

	X2AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : x2ap_populate_nbour_cell_info 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_neighbour_cell_info - pointer to x2ap_nbour_info_t
 *                  served_cell_id - served cell id
 *                  p_gb_enb_id - dlobal eNb id
 * Outputs        : p_x2ap_gb_ctx
 * Returns        : None
 * Description    : this function populates neighbour cell information in to the 
 *                  global context
 ********************************************************************************/
void x2ap_populate_nbour_cell_info(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_nbour_info_t* p_neighbour_cell_info,
		U8 served_cell_id,
		x2_gb_enb_id_t *p_gb_enb_id)
{
	S32 nbour_cell_idx = 0;

	X2AP_UT_TRACE_ENTER();

	/* Get neighbour cell index */
	nbour_cell_idx = x2ap_get_nbr_index_from_ecgi(p_x2ap_gb_ctx, p_neighbour_cell_info->ecgi, X2AP_TRUE, served_cell_id);

	if ((nbour_cell_idx == -1) || (nbour_cell_idx >= MAX_NEIGHBOURS))
	{
		X2AP_TRACE(X2AP_WARNING, "Incorrect neighbour cell index:%u.\n", nbour_cell_idx);
		X2AP_UT_TRACE_EXIT();
		return;
	}

	/* Copy EARFCN */
	X2AP_MEMCPY((void*)&(p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].
				peer_cell_info.earfcn),
			(const void*)(&(p_neighbour_cell_info->earfcn)),
			sizeof(x2ap_earfcn_t));

	/* Copy PCI */
	X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->
				peer_enb_cell_info[nbour_cell_idx].
				peer_cell_info.pci),
			(const void*)(&(p_neighbour_cell_info->pci)),
			sizeof(U32));

	/* Copy ECGI */
	X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->
				peer_enb_cell_info[nbour_cell_idx].
				peer_cell_info.ecgi),
			(const void*)(&(p_neighbour_cell_info->ecgi)),
			sizeof(x2ap_ecgi_t));

	/* Copy GB_ENB_ID */
	if (NULL != p_gb_enb_id)
	{
		X2AP_MEMCPY((void*)&(p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].peer_gb_enb_id),
				(const void*)(p_gb_enb_id),
				sizeof(x2_gb_enb_id_t));
	}

	/* Increment neighbour count */
	p_x2ap_gb_ctx->served_cell_info[served_cell_id].num_nbour++;

	/* Update served/neighbour relation table */
	p_x2ap_gb_ctx->relation_table[served_cell_id][nbour_cell_idx].relation = DIRECT_NEIGHBOUR_ACTIVE;
	p_x2ap_gb_ctx->relation_table[served_cell_id][nbour_cell_idx].ecgi = p_neighbour_cell_info->ecgi;

	X2AP_TRACE(X2AP_INFO, "Neighbour cell with pci:%u added for cell_id:%u.\n", p_neighbour_cell_info->pci, served_cell_id);
	X2AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : x2ap_process_enb_config_update_ack 
 * Inputs         : p_msg_ptr - pointer to msg 
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE 
 * Description    : This function processes eNB configuration update ack
 ********************************************************************************/
x2ap_return_et
x2ap_process_enb_config_update_ack(
		U8* p_msg_ptr,
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t* p_enb_cntx)
{
	x2ap_return_et retVal = X2AP_FAILURE;
	x2ap_enb_conf_ack_t enb_conf_ack;
	x2ap_peer_enb_msg_t* p_peer_enb_msg = X2AP_P_NULL;
	U32 result;
    /*SPR_19067_FIX_START*/
    rrc_x2ap_error_indication_t  error_indication;
    x2ap_error_ind_bool_t        send_err_ind;
    OSCTXT 			 asn1_ctx;
    /*SPR_19067_FIX_STOP*/

	X2AP_UT_TRACE_ENTER();

	p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;

	X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
	X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
	X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);
	X2AP_ASSERT(p_enb_cntx != X2AP_P_NULL);

	X2AP_MEMSET(&enb_conf_ack, 0x00, sizeof(x2ap_enb_conf_ack_t));
    /*SPR_19067_FIX_START*/
    retVal = x2ap_decode_eNB_config_update_ack(&enb_conf_ack, &result, 
		(p_peer_enb_msg)->p_msg, 
		&(p_peer_enb_msg->msg_len), 
		&error_indication,
	        &send_err_ind,
		&asn1_ctx,
		p_x2ap_gb_ctx,
		p_enb_cntx);
    /*SPR_19067_FIX_STOP*/

	if (X2AP_SUCCESS == retVal)
	{
		switch(p_enb_cntx->enb_config_sm_ctx.state)
		{
			case X2AP_ENB_CONFIG_IDLE_ST:
			case X2AP_ENB_CONFIG_ACTIVE_ST:
			case X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST:
				{
					X2AP_TRACE(X2AP_ERROR,"Invalid state. Ignore message.%u.n", p_enb_cntx->enb_config_sm_ctx.state);
				}
				break;
			case X2AP_ENB_CONFIG_W_CONF_RESP_ST:
				{
					U16 dest_module_id =0;
					U16 trans_id =0;

					x2ap_enb_config_update_resp_t enb_cfg_resp;

					X2AP_TRACE(X2AP_ERROR,"Stop X2AP_ENB_CONFIG_UPDATE_RSP_TIMER.");

					X2AP_MEMSET(&enb_cfg_resp, 0x00, sizeof(x2ap_enb_config_update_resp_t));
					if(enb_conf_ack.bitmask & X2AP_CRITICALITY_DIAGNOS_PRESENT)
					{
						X2AP_TRACE(X2AP_INFO,"criticality diagnos is present");
					}

					/* Stop eNB Config Update Response guard Timer */
					x2ap_stop_timer(p_enb_cntx->enb_config_sm_ctx.req_timer_id);

					/* change state to X2AP_ENB_CONFIG_ACTIVE_ST */
					p_enb_cntx->enb_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
					p_enb_cntx->enb_config_sm_ctx.req_timer_id = 0;

					dest_module_id = p_enb_cntx->enb_config_sm_ctx.dst_module_id;
					trans_id = p_enb_cntx->enb_config_sm_ctx.transaction_id;

					X2AP_MEMCPY(&enb_cfg_resp.enb2_gb_id, &p_enb_cntx->gb_enb_id, sizeof(x2_gb_enb_id_t));
/* SPR 18736 changes Start */
                    if (RRC_RRM_MODULE_ID != dest_module_id)
                    {
					/* Send eNB Configuration Update response */
					x2ap_send_enb_config_update_response(p_x2ap_gb_ctx, &enb_cfg_resp, X2AP_SUCCESS, dest_module_id, trans_id);
                    }
					X2AP_MEMSET(&p_enb_cntx->enb_config_sm_ctx, 0x00, sizeof(x2ap_enb_config_sm_context_t));
/* SPR 18736 changes End */
					p_enb_cntx->enb_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
				}
				break;
		}
	}
    /*SPR_19067_FIX_START*/
    rtFreeContext(&asn1_ctx);
    /*SPR_19067_FIX_STOP*/

	X2AP_UT_TRACE_EXIT();
	return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_process_enb_config_update_failure 
 * Inputs         : p_msg_ptr -      pointer to msg buuffer
 *                  p_x2ap_gb_ctx - pointer to X2Ap global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function processes eNB configuration update failure
 ********************************************************************************/
x2ap_return_et
x2ap_process_enb_config_update_failure(
		U8* p_msg_ptr,
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t* p_enb_cntx)
{
	x2ap_return_et retVal = X2AP_FAILURE;
	x2ap_enb_conf_upd_fail_t enb_conf_failure;
	x2ap_peer_enb_msg_t* p_peer_enb_msg = X2AP_P_NULL;
	U32 result;

    /*SPR_19067_FIX_START*/
    rrc_x2ap_error_indication_t  error_indication;
    x2ap_error_ind_bool_t        send_err_ind;
    OSCTXT                       asn1_ctx;
    /*SPR_19067_FIX_STOP*/

	X2AP_UT_TRACE_ENTER();

	p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;

	X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
	X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
	X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);
	X2AP_ASSERT(p_enb_cntx != X2AP_P_NULL);

	X2AP_MEMSET(&enb_conf_failure, 0x00, sizeof(x2ap_enb_conf_upd_fail_t));
    /*SPR_19067_FIX_START*/
    retVal = x2ap_decode_eNB_config_update_resp_fail(&enb_conf_failure, 
		&result, 
		(p_peer_enb_msg)->p_msg, 
		&(p_peer_enb_msg->msg_len),
		p_x2ap_gb_ctx,
		p_enb_cntx,
		&error_indication,
		&send_err_ind,
		&asn1_ctx);
    /*SPR_19067_FIX_STOP*/


	if (X2AP_SUCCESS == retVal)
	{
		switch(p_enb_cntx->enb_config_sm_ctx.state)
		{
			case X2AP_ENB_CONFIG_IDLE_ST:
			case X2AP_ENB_CONFIG_ACTIVE_ST:
			case X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST:
				{
					X2AP_TRACE(X2AP_ERROR,"Invalid state. Ignore message.%u.n", p_enb_cntx->enb_config_sm_ctx.state);
				}
				break;
			case X2AP_ENB_CONFIG_W_CONF_RESP_ST:
				{
					U16 dest_module_id =0;
					U16 trans_id =0;

					if(enb_conf_failure.bitmask & X2AP_CRITICALITY_DIAGNOSTICS_PRESENT)
					{
						X2AP_TRACE(X2AP_INFO,"criticality diagnos is present");
					}

					X2AP_TRACE(X2AP_ERROR,"Stop X2AP_ENB_CONFIG_UPDATE_RSP_TIMER.");
					x2ap_stop_timer(p_enb_cntx->enb_config_sm_ctx.req_timer_id);
					p_enb_cntx->enb_config_sm_ctx.req_timer_id = 0;
					dest_module_id = p_enb_cntx->enb_config_sm_ctx.dst_module_id;
					trans_id = p_enb_cntx->enb_config_sm_ctx.transaction_id;

					if(enb_conf_failure.bitmask & X2AP_TIME_TO_WAIT_INDEX_PRESENT)
					{
						U32 timer_value =0;
						x2ap_timer_t timer_id;
						x2ap_timer_buffer_t timer_buf;
						x2ap_enb_config_update_wait_ind_t enb_time_to_wait_ind;

						X2AP_MEMSET(&enb_time_to_wait_ind, 0x00, sizeof(x2ap_enb_config_update_wait_ind_t));
						X2AP_MEMCPY(&enb_time_to_wait_ind.enb1_gb_id, &p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));
						X2AP_MEMCPY(&enb_time_to_wait_ind.enb2_gb_id, &p_enb_cntx->gb_enb_id, sizeof(x2_gb_enb_id_t));
						enb_time_to_wait_ind.time_to_wait = enb_conf_failure.time_to_wait;

						/* Send eNB Configuration Update response */
						rrc_x2apCommon_send_x2ap_enb_config_update_wait_ind(&enb_time_to_wait_ind, RRC_X2AP_MODULE_ID, dest_module_id, trans_id, RRC_NULL);

						/* Start X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST */
						x2ap_convert_time_to_wait_index_to_duration(p_x2ap_gb_ctx->config_param.timer_conf_info.x2ap_time_to_wait_index,&timer_value);
						timer_value = timer_value + p_x2ap_gb_ctx->config_param.timer_conf_info.eNB_Config_Update_time_to_wait_offset;

						timer_buf.auto_delete = X2AP_FALSE;
						timer_buf.sd = p_enb_cntx->sctp_sd;
						timer_buf.p_timer_specific_data = (void*)p_enb_cntx;
						timer_buf.timer_type = X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER;

						timer_id = x2ap_start_timer(timer_value, &timer_buf, sizeof(timer_buf), X2AP_FALSE);

						p_enb_cntx->enb_config_sm_ctx.state = X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST ;
						p_enb_cntx->enb_config_sm_ctx.time_to_wait_timer_id = timer_id;

					    X2AP_TRACE(X2AP_INFO,"Started X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER with timer_id %p and duration %u\n", timer_id, timer_value);
						X2AP_TRACE(X2AP_INFO,"enb_config_sm: State changed to X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST\n");
					}
					else
					{
						x2ap_enb_config_update_resp_t enb_cfg_resp;
						X2AP_MEMSET(&enb_cfg_resp, 0x00, sizeof(x2ap_enb_config_update_resp_t));

						/* change state to ACTIVE */
						x2ap_enb_config_update_fill_cause(&enb_conf_failure.cause,&enb_cfg_resp.cause);

						X2AP_MEMCPY(&enb_cfg_resp.enb2_gb_id, &p_enb_cntx->gb_enb_id, sizeof(x2_gb_enb_id_t));

						/* Send eNB Configuration Update response */
						x2ap_send_enb_config_update_response(p_x2ap_gb_ctx, &enb_cfg_resp, X2AP_FAILURE, dest_module_id, trans_id);
						X2AP_MEMSET(&p_enb_cntx->enb_config_sm_ctx, 0x00, sizeof(x2ap_enb_config_sm_context_t));
						p_enb_cntx->enb_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
					}
				}
				break;
		}
	}
    /*SPR_19067_FIX_START*/
    rtFreeContext(&asn1_ctx);
    /*SPR_19067_FIX_STOP*/

	X2AP_UT_TRACE_EXIT();
	return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_convert_time_to_wait_index_to_duration 
 * Inputs         : time_to_wait_index - unsigned char
 *                  p_timer_duration - pointer to duration 
 * Outputs        : p_timer_duration
 * Returns        : None
 * Description    : This function converts time_to_wait_index to the duration of 
 *                  wait
 ********************************************************************************/
void x2ap_convert_time_to_wait_index_to_duration(U8 time_to_wait_index, U32* p_timer_duration)
{
	U32 timer = 0;
	switch(time_to_wait_index)
	{
		case SEC_1:
			timer = 1*1000;
			break;
		case SEC_2:
			timer = 2*1000;
			break;
		case SEC_5:
			timer = 5*1000;
			break;
		case SEC_10:
			timer = 10*1000;
			break;
		case SEC_20:
			timer = 20*1000;
			break;
		case SEC_60:
			timer = 60*1000;
			break;
		default:
			X2AP_TRACE(X2AP_ERROR,"Invalid timer to wait index");
			break;
	}
	*p_timer_duration = timer;
}

/*****************************************************************************
 * Function Name  : x2ap_enb_config_update_fill_cause 
 * Inputs         : p_src - pointer to x2ap_cause_t
 * Outputs        : p_dst - pointer to x2ap_cause_t
 * Returns        : None
 * Description    : This function fills eNB configuration update cause 
 ********************************************************************************/
	void
x2ap_enb_config_update_fill_cause(x2_cause_t* p_src, x2ap_cause_t* p_dst)
{
	if (RADIO_NW_LAYER_CAUSE == p_src->cause_type)
	{
		p_dst->type = RADIO_NW_LAYER_CAUSE;
		p_dst->value = p_src->cause_info.radio_nw_cause;
	}
	else if (TRANSPORT_LAYER_CAUSE == p_src->cause_type)
	{
		p_dst->type = TRANSPORT_LAYER_CAUSE;
		p_dst->value = p_src->cause_info.transport_cause ;
	}
	else if (PROTOCOL_CAUSE == p_src->cause_type)
	{
		p_dst->type = PROTOCOL_CAUSE;
		p_dst->value = p_src->cause_info.protocol_cause;
	}
	else if (MISC_CAUSE == p_src->cause_type)
	{
		p_dst->type = MISC_CAUSE;
		p_dst->value = p_src->cause_info.misc_cause;
	}
}

/*****************************************************************************
 * Function Name  : x2ap_process_enb_config_update_enb_req 
 * Inputs         : p_msg_ptr - pointer to msg
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function the eNB configuration update eNB
 ********************************************************************************/
x2ap_return_et
x2ap_process_enb_config_update_enb_req
(
 U8* p_msg_ptr,
 x2ap_gb_context_t* p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx
 )
{
	x2ap_return_et            retVal = X2AP_SUCCESS;
	static x2ap_enb_config_update_ind_t      eNB_config_upd_req;
	x2ap_enb_conf_ack_t     eNB_config_upd_resp;
	x2ap_enb_conf_upd_fail_t eNB_config_upd_resp_fail;

	U8 asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN];
	U16 asn_msg_len = 0;
	x2ap_bool_et timeTowait_present = X2AP_FALSE;
	x2ap_peer_enb_msg_t* p_peer_enb_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr ;

    /*SPR_19067_FIX_START*/
    rrc_x2ap_error_indication_t  error_indication;
    x2ap_error_ind_bool_t        send_err_ind;
    OSCTXT                       asn1_ctx;
    /*SPR_19067_FIX_STOP*/

	X2AP_UT_TRACE_ENTER();

	/* Received pointer should not be NULL */
	X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
	X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);
	X2AP_ASSERT(p_enb_cntx != X2AP_P_NULL);

	/* Init the messages to be send */
	X2AP_MEMSET(&eNB_config_upd_resp,0,sizeof(x2ap_enb_conf_ack_t));
	X2AP_MEMSET(&eNB_config_upd_resp_fail,0,sizeof(x2ap_enb_conf_upd_fail_t));
	X2AP_MEMSET(&eNB_config_upd_req,0,sizeof(x2ap_enb_config_update_ind_t));

	switch(p_enb_cntx->enb_config_sm_ctx.state )
	{
		case X2AP_ENB_CONFIG_IDLE_ST:
			{
				X2AP_TRACE(X2AP_ERROR,"X2 interface not operational. ");
				timeTowait_present = X2AP_TRUE;
				retVal = X2AP_FAILURE;
			}break;
		case X2AP_ENB_CONFIG_W_CONF_RESP_ST:
		case X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST:
			break;
		case X2AP_ENB_CONFIG_ACTIVE_ST:
			break;
		default:
			X2AP_TRACE(X2AP_ERROR,"Invalid state.\n");
			retVal = X2AP_FAILURE;
			break;
	}

	if (X2AP_SUCCESS == retVal)
	{
		retVal = x2ap_decode_eNB_config_update_req(/*SPR_19067_FIX_START*/p_x2ap_gb_ctx,
                /*SPR_19067_FIX_STOP*/
				&eNB_config_upd_req,
				(p_peer_enb_msg)->p_msg,
				&(p_peer_enb_msg->msg_len),
                                                p_enb_cntx,/*SPR_19067_FIX_START*/
						&error_indication,
				                &send_err_ind,
				                &asn1_ctx);
					        /*SPR_19067_FIX_STOP*/

		if(eNB_config_upd_req.bitmask ==0)
		{
			/*Only message type present in the message, send eNB CONFIGURATION ACKNOWLEDGE*/
			x2ap_send_enb_configuration_update_ack(p_x2ap_gb_ctx, p_enb_cntx, &eNB_config_upd_resp, asn_msg_buff, &asn_msg_len);

        /*SPR_19067_FIX_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_19067_FIX_STOP*/

			return X2AP_SUCCESS;
		}

		/* if added cells present in req */
		if ((X2AP_SUCCESS == retVal) && (X2AP_NULL != eNB_config_upd_req.add_served_cell.num_served_cell))
		{
			retVal = x2ap_process_peer_added_served_cells(&eNB_config_upd_req, p_x2ap_gb_ctx,p_enb_cntx);
			X2AP_TRACE(X2AP_INFO,"Peer added cells processed ");
		}
		/* if deleted cells present in req */
		if ((X2AP_SUCCESS == retVal) && (X2AP_NULL != eNB_config_upd_req.del_served_cell.num_served_cell))
		{
			retVal = x2ap_process_peer_deleted_served_cells(&eNB_config_upd_req,p_x2ap_gb_ctx, p_enb_cntx);
			X2AP_TRACE(X2AP_INFO,"Peer deleted cells processed ");
		}

		/* if modified cells present in req */
		if ((X2AP_SUCCESS == retVal) && (X2AP_NULL != eNB_config_upd_req.mod_served_cell.num_served_cell))
		{
			retVal = x2ap_process_peer_modified_served_cells(&eNB_config_upd_req,p_x2ap_gb_ctx, p_enb_cntx);
		}

		/* if added gu group ids present in req */
		if ((X2AP_SUCCESS == retVal) && (X2AP_NULL != eNB_config_upd_req.add_gu_id.num_gu_group_id))
		{
			retVal = x2ap_process_peer_added_gu_id_list(&eNB_config_upd_req, p_enb_cntx);
		}

		/* if deleted gu group ids present in req */
		if ((X2AP_SUCCESS == retVal) && (X2AP_NULL != eNB_config_upd_req.delete_gu_id.num_gu_group_id))
		{
			retVal = x2ap_process_peer_deleted_gu_id_list(&eNB_config_upd_req,p_enb_cntx);
		}
	}
	if (X2AP_SUCCESS == retVal)
	{
		U16 dst_module_id = RRC_RRM_MODULE_ID;

		/* Send eNB Configuration Update Indication to SON/RRM*/
		if(p_x2ap_gb_ctx->is_son_present)
		{
			dst_module_id = RRC_SON_MODULE_ID;
		}

		X2AP_MEMCPY(&eNB_config_upd_req.enb1_gb_id, &p_enb_cntx->gb_enb_id, sizeof(x2_gb_enb_id_t));
		X2AP_MEMCPY(&eNB_config_upd_req.enb2_gb_id, &p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));

		retVal = rrc_x2apCommon_send_x2ap_enb_config_update_ind(&eNB_config_upd_req, RRC_X2AP_MODULE_ID, dst_module_id, 0, RRC_NULL);

		/* if all fields of  eNB Config Update Req has been processed successfully */
		if (X2AP_SUCCESS == retVal)
		{
			/* Build eNB CONFIGURATION UPDATE ACKNOWLEDGE */
			x2ap_build_eNB_config_upd_resp(&eNB_config_upd_resp);

			/* Send eNB CONFIGURATION UPDATE ACKNOWLEDGE */
			x2ap_send_enb_configuration_update_ack(p_x2ap_gb_ctx, p_enb_cntx, &eNB_config_upd_resp, asn_msg_buff, &asn_msg_len);
		}
		else
		{
			X2AP_TRACE(X2AP_INFO,"Sending eNB Configuration Update Indication failed.\n");
		}
	}
	else
	{
		/* Build and Send eNB CONFIGURATION UPDATE FAILURE */
		x2ap_build_and_send_enb_configuration_update_failure(p_x2ap_gb_ctx, p_enb_cntx, &eNB_config_upd_resp_fail, asn_msg_buff, &asn_msg_len, timeTowait_present);
	}

   /*SPR_19067_FIX_START*/
    rtFreeContext(&asn1_ctx);
   /*SPR_19067_FIX_STOP*/

	X2AP_UT_TRACE_EXIT();
	return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_process_peer_added_served_cells 
 * Inputs         : pENB_config_upd_req - pointer to x2ap_enb_config_update_ind_t
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function processes peer added served cells  
 ********************************************************************************/
x2ap_return_et
x2ap_process_peer_added_served_cells
(
 x2ap_enb_config_update_ind_t       *pENB_config_upd_req,
 x2ap_gb_context_t         *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx)
{
	U8 cell_count =0;
	U8 cell_counter =0;
	U8 nbour_count = 0;
	U8 num_nbour =0;
	U8 nbour_found =X2AP_FAILURE;

	X2AP_TRACE(X2AP_INFO, "%u peer serving cell to add present\n", pENB_config_upd_req->add_served_cell.num_served_cell);

	/* Search for what serving cell neighbour cell info has been added */
	for(cell_count=0; cell_count<pENB_config_upd_req->add_served_cell.num_served_cell; cell_count++)
	{
		nbour_found = X2AP_FAILURE;
		num_nbour = pENB_config_upd_req->add_served_cell.cell_list[cell_count].nbour_cell_info.num_neighbour_cell;

		for(nbour_count=0; nbour_count<num_nbour && nbour_found!=X2AP_SUCCESS; nbour_count++)
		{
			/*coverity fix 61211*/
			for(cell_counter=0; ((cell_counter<p_x2ap_gb_ctx->num_served_cells) &&
						(cell_counter < MAX_NUM_CELLS)) &&
					/*coverity fix 61211*/
					nbour_found!=X2AP_SUCCESS; cell_counter++)
			{
				if(p_x2ap_gb_ctx->served_cell_info[cell_counter].pci ==
						pENB_config_upd_req->add_served_cell.cell_list[cell_count].nbour_cell_info.neighbour_info_arr[nbour_count].pci)
				{
					x2ap_nbour_info_t nbour_info;
					x2ap_ecgi_t ecgi;

					x2ap_compose_ecgi_from_cell_id(p_enb_cntx->gb_enb_id, pENB_config_upd_req->add_served_cell.cell_list[cell_count].cell_info.cell_id, &ecgi);

					nbour_info.pci = pENB_config_upd_req->add_served_cell.cell_list[cell_count].cell_info.pci;

					X2AP_MEMCPY(&nbour_info.ecgi, &ecgi, sizeof(x2ap_ecgi_t));

					if(X2AP_EUTRA_MODE_FDD & pENB_config_upd_req->add_served_cell.cell_list[cell_count].cell_info.choice_eutra_mode.choice_eutra_mode.bitmask)
					{
						nbour_info.earfcn = pENB_config_upd_req->add_served_cell.cell_list[cell_count].cell_info.choice_eutra_mode.choice_eutra_mode.fdd_info.dl_earfcn;
					}
					else if(X2AP_EUTRA_MODE_TDD & pENB_config_upd_req->add_served_cell.cell_list[cell_count].cell_info.choice_eutra_mode.choice_eutra_mode.bitmask)
					{
						nbour_info.earfcn = pENB_config_upd_req->add_served_cell.cell_list[cell_count].cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.earfcn;
					}

#ifdef LTE_EMBMS_SUPPORTED
                                        /*COV_2NOV_START*/
					U16 nbour_cell_idx = 0;
                                        /*COV_2NOV_STOP*/
					nbour_cell_idx = x2ap_get_nbr_index_from_ecgi(p_x2ap_gb_ctx, 
							nbour_info.ecgi, X2AP_TRUE, cell_counter);
					/* Copy MBSFBN INFORMATION */
					if((X2AP_MBSFN_SUBFRAME_INFO_PRESENT & pENB_config_upd_req->
							mod_served_cell.cell_list[cell_count].
                                        /*COV_2NOV_START*/
							served_cell_info.cell_info.bitmask) && (nbour_cell_idx < MAX_NEIGHBOURS))
					{/*COV_2NOV_STOP*/

						p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].
							peer_cell_info.bitmask |= X2AP_MBSFN_SUBFRAME_INFO_PRESENT;
						X2AP_MEMCPY(&p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].
								peer_cell_info.mbsfn_subframe_info,
								&pENB_config_upd_req->mod_served_cell.cell_list[cell_count].
								served_cell_info.cell_info.mbsfn_subframe_info,
								sizeof(x2ap_mbsfn_subframe_info_t));
					}
#endif

					/* Add cell_count neighbour for cell_counter served_cell*/
					x2ap_populate_nbour_cell_info(p_x2ap_gb_ctx, &nbour_info, cell_counter, &p_enb_cntx->gb_enb_id);
                    /* Bug 4691 Changes Start */
                    x2ap_build_and_send_update_info(&nbour_info);
                    /* Bug 4691 Changes End */
					nbour_found = X2AP_SUCCESS;
				}
			}
		}

		if(nbour_found == X2AP_SUCCESS)
		{
			X2AP_TRACE(X2AP_WARNING, "%u served cell in add list added in neighbour list", cell_count);
		}
		else
		{
			X2AP_TRACE(X2AP_WARNING, "%u served cell in add list not added in neighbour list", cell_count);
		}
	}
	return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_process_peer_deleted_served_cells 
 * Inputs         : pENB_config_upd_req - pointer to x2ap_enb_config_update_ind_t
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function proeceses the deleted served cells of peer eNB
 ********************************************************************************/
x2ap_return_et
x2ap_process_peer_deleted_served_cells
(
 x2ap_enb_config_update_ind_t       *pENB_config_upd_req,
 x2ap_gb_context_t         *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx)
{
	U8 cell_id =0;
	U8 old_cell_id =0;
	U8 cell_count =0;
	x2ap_ecgi_t ecgi;
	x2ap_nbr_ecgi_search_node_t* p_node = X2AP_P_NULL;

	X2AP_TRACE(X2AP_INFO, "%u peer serving cell to delete present\n", pENB_config_upd_req->del_served_cell.num_served_cell);

	for(cell_count=0; cell_count<pENB_config_upd_req->del_served_cell.num_served_cell; cell_count++)
	{
		X2AP_MEMSET(&ecgi, 0x00, sizeof(x2ap_ecgi_t));
		old_cell_id = pENB_config_upd_req->del_served_cell.cell_id_list[cell_count];

		x2ap_compose_ecgi_from_cell_id(p_enb_cntx->gb_enb_id, old_cell_id, &ecgi);

		p_node = (x2ap_nbr_ecgi_search_node_t*)x2ap_st_find_tree_node((x2ap_search_tree_t *)&(p_x2ap_gb_ctx->nbr_ecgi_search_tree), (void*)(&ecgi) );
		if(p_node== X2AP_P_NULL)
		{
			X2AP_TRACE(X2AP_WARNING, "Neighbour ECGI not found in tree.\n");
		}
		else
		{
			cell_id = p_node->serving_cell_index;
			x2ap_st_delete_node(&p_x2ap_gb_ctx->nbr_ecgi_search_tree, &(p_node->nodeAnchor));
			x2ap_mem_free(&p_node->nodeAnchor);
			X2AP_MEMSET(&p_x2ap_gb_ctx->peer_enb_cell_info[p_node->logical_id], 0x00, sizeof(x2ap_peer_enb_cell_info_t));
			p_x2ap_gb_ctx->served_cell_info[cell_id].num_nbour--;
			p_x2ap_gb_ctx->num_peer_enb_cells--;
			p_x2ap_gb_ctx->relation_table[cell_id][p_node->logical_id].relation =NOT_NEIGHBOUR;
		}
	}
	return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_process_peer_modified_served_cells 
 * Inputs         : pENB_config_upd_req - pointer to x2ap_enb_config_update_ind_t
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function processes the modified served cells of peer eNB
 ********************************************************************************/
x2ap_return_et
x2ap_process_peer_modified_served_cells
(
 x2ap_enb_config_update_ind_t* pENB_config_upd_req,
 x2ap_gb_context_t* p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx)
{
	U8 cell_count =0;
	U8 cell_counter =0;
	U8 nbour_count = 0;
	U8 num_nbour =0;
	U8 nbour_found =X2AP_FAILURE;

	X2AP_TRACE(X2AP_INFO, "%u peer serving cell to modify present\n", pENB_config_upd_req->mod_served_cell.num_served_cell);

	/* Search for what serving cell neighbour cell info has been modified */
	for(cell_count=0; cell_count<pENB_config_upd_req->mod_served_cell.num_served_cell; cell_count++)
	{
		nbour_found = X2AP_FAILURE;
		num_nbour = pENB_config_upd_req->mod_served_cell.cell_list[cell_count].served_cell_info.nbour_cell_info.num_neighbour_cell;

		for(nbour_count=0; nbour_count<num_nbour && nbour_found!=X2AP_SUCCESS; nbour_count++)
		{
			/*coverity fix 61212*/
			for(cell_counter=0; ((cell_counter<p_x2ap_gb_ctx->num_served_cells) &&
						(cell_counter < MAX_NUM_CELLS)) && 
					/*coverity fix 61212*/
					nbour_found!=X2AP_SUCCESS; cell_counter++)
			{
				if(p_x2ap_gb_ctx->served_cell_info[cell_counter].pci ==
						pENB_config_upd_req->mod_served_cell.cell_list[cell_count].served_cell_info.nbour_cell_info.neighbour_info_arr[nbour_count].pci)
				{
					x2ap_ecgi_t ecgi;
					x2ap_nbour_info_t nbour_info;
					x2ap_nbr_ecgi_search_node_t* p_node = X2AP_P_NULL;
					/* SPR 7573 start*/
					x2ap_nbr_ecgi_search_node_t* p_next = X2AP_P_NULL;

					X2AP_MEMSET(&ecgi, 0x00, sizeof(x2ap_ecgi_t));
					x2ap_compose_ecgi_from_cell_id(p_enb_cntx->gb_enb_id, pENB_config_upd_req->mod_served_cell.cell_list[cell_count].old_cell_id, &ecgi);

					/* Add cell_count neighbour for cell_counter served_cell*/
					/*Delete if old neighbour present with old_ecgi*/
					p_node = (x2ap_nbr_ecgi_search_node_t*)x2ap_st_get_first_node(&(p_x2ap_gb_ctx->nbr_ecgi_search_tree));
					p_next = p_node;
					while(X2AP_P_NULL != p_node)
					{
						p_next = (x2ap_nbr_ecgi_search_node_t*)x2ap_st_get_next_node((x2ap_search_tree_node_t*)p_node);

						if (X2AP_MEMCMP(&ecgi, &p_node->ecgi, sizeof(x2ap_ecgi_t)))
						{
							X2AP_TRACE(X2AP_WARNING, "Neighbour ECGI not found in tree.\n");
						}
						else
						{
							U8 cell_id = p_node->serving_cell_index;
							x2ap_st_delete_node(&p_x2ap_gb_ctx->nbr_ecgi_search_tree, &(p_node->nodeAnchor));
							x2ap_mem_free(&p_node->nodeAnchor);
							X2AP_MEMSET(&p_x2ap_gb_ctx->peer_enb_cell_info[p_node->logical_id], 0x00, sizeof(x2ap_peer_enb_cell_info_t));
							p_x2ap_gb_ctx->served_cell_info[cell_id].num_nbour--;
							p_x2ap_gb_ctx->num_peer_enb_cells--;
							p_x2ap_gb_ctx->relation_table[cell_id][p_node->logical_id].relation =NOT_NEIGHBOUR;
						}
						p_node = p_next;
					}
					/* Commenting original code, as some problem with CSPL search */
					/*p_node = (x2ap_nbr_ecgi_search_node_t*)x2ap_st_find_tree_node((x2ap_search_tree_t *)&(p_x2ap_gb_ctx->nbr_ecgi_search_tree), (void*)(&ecgi) );
					  if(p_node== X2AP_P_NULL)
					  {
					  X2AP_TRACE(X2AP_WARNING, "Neighbour ECGI not found in tree.\n");
					  }
					  else
					  {
					  U8 cell_id = p_node->serving_cell_index;
					  x2ap_st_delete_node(&p_x2ap_gb_ctx->nbr_ecgi_search_tree, &(p_node->nodeAnchor));
					  x2ap_mem_free(&p_node->nodeAnchor);
					  X2AP_MEMSET(&p_x2ap_gb_ctx->peer_enb_cell_info[p_node->logical_id], 0x00, sizeof(x2ap_peer_enb_cell_info_t));
					  p_x2ap_gb_ctx->served_cell_info[cell_id].num_nbour--;
					  p_x2ap_gb_ctx->num_peer_enb_cells--;
					  p_x2ap_gb_ctx->relation_table[cell_id][p_node->logical_id].relation =NOT_NEIGHBOUR;
					  }*/
					/* SPR 7573 end*/

					/* Add neighbour info with new_ecgi*/
					X2AP_MEMSET(&ecgi, 0x00, sizeof(x2ap_ecgi_t));
					x2ap_compose_ecgi_from_cell_id(p_enb_cntx->gb_enb_id, pENB_config_upd_req->mod_served_cell.cell_list[cell_count].served_cell_info.cell_info.cell_id, &ecgi);
					nbour_info.pci =pENB_config_upd_req->mod_served_cell.cell_list[cell_count].served_cell_info.cell_info.pci;

					X2AP_MEMCPY(&nbour_info.ecgi, &ecgi, sizeof(x2ap_ecgi_t));

#ifdef LTE_EMBMS_SUPPORTED
                                        /*COV_2NOV_START*/
					U16 nbour_cell_idx = 0;
                                        /*COV_2NOV_STOP*/
					nbour_cell_idx = x2ap_get_nbr_index_from_ecgi(p_x2ap_gb_ctx, 
							nbour_info.ecgi, X2AP_TRUE, cell_counter);
					/* Copy MBSFBN INFORMATION */
                                        /*COV_2NOV_START*/
					if(X2AP_MBSFN_SUBFRAME_INFO_PRESENT & pENB_config_upd_req->
							mod_served_cell.cell_list[cell_count].
							served_cell_info.cell_info.bitmask && (nbour_cell_idx < MAX_NEIGHBOURS))
					{/*COV_2NOV_STOP*/

						p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].
							peer_cell_info.bitmask |= X2AP_MBSFN_SUBFRAME_INFO_PRESENT;
						X2AP_MEMCPY(&p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].
								peer_cell_info.mbsfn_subframe_info,
								&pENB_config_upd_req->mod_served_cell.cell_list[cell_count].
								served_cell_info.cell_info.mbsfn_subframe_info,
								sizeof(x2ap_mbsfn_subframe_info_t));
					}
#endif

					if(X2AP_EUTRA_MODE_FDD & pENB_config_upd_req->mod_served_cell.cell_list[cell_count].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.bitmask)
					{
						nbour_info.earfcn = pENB_config_upd_req->mod_served_cell.cell_list[cell_count].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.fdd_info.dl_earfcn;
					}
					else if(X2AP_EUTRA_MODE_TDD & pENB_config_upd_req->mod_served_cell.cell_list[cell_count].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.bitmask)
					{
						nbour_info.earfcn = pENB_config_upd_req->mod_served_cell.cell_list[cell_count].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.earfcn;
					}

					/* Add cell_count neighbour for cell_counter served_cell*/
					x2ap_populate_nbour_cell_info(p_x2ap_gb_ctx, &nbour_info, cell_counter, &p_enb_cntx->gb_enb_id);
                    /* Bug 4691 Changes Start */
                    x2ap_build_and_send_update_info(&nbour_info);
                    /* Bug 4691 Changes End */

					nbour_found = X2AP_SUCCESS;
				}
			}
		}

		if(nbour_found == X2AP_SUCCESS)
		{
			X2AP_TRACE(X2AP_DETAILED, "%u served cell in modified list added in neighbour list", cell_count);
		}
		else
		{
			X2AP_TRACE(X2AP_WARNING, "%u served cell in modified list not added in neighbour list", cell_count);
		}
	}
	return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_process_peer_added_gu_id_list 
 * Inputs         : pENB_config_upd_req - pointer to x2ap_enb_config_update_ind_t
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer e4Nb context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function processes added GU id of peer eNB
 ********************************************************************************/
x2ap_return_et
x2ap_process_peer_added_gu_id_list
(
 x2ap_enb_config_update_ind_t* pENB_config_upd_req,
 x2ap_peer_enb_context_t* p_enb_cntx)
{
	X2AP_TRACE(X2AP_INFO, "%u GU group id to add.\n", pENB_config_upd_req->add_gu_id.num_gu_group_id);

	if(p_enb_cntx->x2ap_gu_grp_id.num_gu_group_id + pENB_config_upd_req->add_gu_id.num_gu_group_id > MAX_ENB_POOLS)
	{
		X2AP_TRACE(X2AP_INFO, "Total GU group id for peer eNB greater than maximum value MAX_ENB_POOLS.\n");
		return X2AP_FAILURE;
	}

	X2AP_MEMCPY(&p_enb_cntx->x2ap_gu_grp_id.group_id_arr[p_enb_cntx->x2ap_gu_grp_id.num_gu_group_id-1],
			pENB_config_upd_req->add_gu_id.gu_id_list,
			(pENB_config_upd_req->add_gu_id.num_gu_group_id)*sizeof(x2ap_gu_group_id_t));

	p_enb_cntx->x2ap_gu_grp_id.num_gu_group_id+= pENB_config_upd_req->add_gu_id.num_gu_group_id;

	X2AP_TRACE(X2AP_INFO, "Total GU group id for peer eNB:%u.\n", p_enb_cntx->x2ap_gu_grp_id.num_gu_group_id);
	return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_process_peer_deleted_gu_id_list 
 * Inputs         : pENB_config_upd_req - pointer to x2ap_enb_config_update_ind_t
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/ X2AP_FAILURE
 * Description    : This function processes the deleted GU id of the peer eNB
 ********************************************************************************/
x2ap_return_et
x2ap_process_peer_deleted_gu_id_list
(
 x2ap_enb_config_update_ind_t* pENB_config_upd_req,
 x2ap_peer_enb_context_t* p_enb_cntx)
{
	U8 gu_id_found=X2AP_FAILURE;
	U8 gu_id_count=0;
	U8 gu_id_counter=0;

	X2AP_TRACE(X2AP_INFO, "%u GU group id to delete.\n", pENB_config_upd_req->delete_gu_id.num_gu_group_id);

	for(gu_id_count=0; gu_id_count< pENB_config_upd_req->delete_gu_id.num_gu_group_id; gu_id_count++)
	{
		gu_id_found = X2AP_FAILURE;

		/* Search GU group id information location*/
		for(gu_id_counter=0; ( (gu_id_counter<p_enb_cntx->x2ap_gu_grp_id.num_gu_group_id) && (gu_id_found!=X2AP_SUCCESS)
					/* Klockwork Fix */
					&& (gu_id_counter < MAX_ENB_POOLS)); 
				/* Klockwork Fix */
				gu_id_counter++)
		{
			if(!(X2AP_MEMCMP(&pENB_config_upd_req->delete_gu_id.gu_id_list[gu_id_count], &p_enb_cntx->x2ap_gu_grp_id.group_id_arr[gu_id_counter],sizeof(x2ap_gu_group_id_t))))
			{
				gu_id_found = X2AP_SUCCESS;

				/* Delete GU group id information at the location*/
				X2AP_MEMSET(&p_enb_cntx->x2ap_gu_grp_id.group_id_arr[gu_id_counter], 0x00, sizeof(x2ap_gu_group_id_t));

				/* Copy last GU group id information to the locations*/
				X2AP_MEMCPY(&p_enb_cntx->x2ap_gu_grp_id.group_id_arr[gu_id_counter],
						&p_enb_cntx->x2ap_gu_grp_id.group_id_arr[p_enb_cntx->x2ap_gu_grp_id.num_gu_group_id-1],
						sizeof(x2ap_gu_group_id_t));

				/* Decrement number of GU group id*/
				p_enb_cntx->x2ap_gu_grp_id.num_gu_group_id--;
			}
		}

		if(gu_id_found == X2AP_FAILURE)
		{
			X2AP_TRACE(X2AP_ERROR, "%u GU group id in the list not found.\n", gu_id_count+1);
		}
	}
	return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_build_eNB_config_upd_resp 
 * Inputs         : pENB_config_upd_resp - pointer to x2ap_enb_conf_ack_t
 *                  p_x2ap_gl_ctx - pointer to X2AP global data
 * Outputs        : None
 * Returns        : None
 * Description    : This function builds the eNB configuaration update responce 
 ********************************************************************************/
void
x2ap_build_eNB_config_upd_resp
(
 x2ap_enb_conf_ack_t  *pENB_config_upd_resp
 )
{
	pENB_config_upd_resp->bitmask = 0;
	X2AP_MEMSET(&(pENB_config_upd_resp->criticality_diagnostics),0, sizeof(x2ap_crit_diagno_t));
}

/*****************************************************************************
 * Function Name  : x2ap_build_eNB_config_upd_resp_fail 
 * Inputs         : pENB_config_upd_resp_fail - pointer to x2ap_enb_conf_upd_fail_t
 *                  p_x2ap_gl_ctx - pointer to x2AP global context
 *                  timeTowait_present - type of x2ap_bool_et
 * Outputs        : pENB_config_upd_resp_fail
 * Returns        : None
 * Description    : This function builds the the eNB configuaration update 
 *                  failure responce
 ********************************************************************************/
void
x2ap_build_eNB_config_upd_resp_fail
(
 x2ap_enb_conf_upd_fail_t  *pENB_config_upd_resp_fail,
 x2ap_gb_context_t         *p_x2ap_gl_ctx,
 x2ap_bool_et               timeTowait_present
 )
{
	pENB_config_upd_resp_fail->cause.cause_type = T_x2ap_Cause_radioNetwork;
	pENB_config_upd_resp_fail->cause.cause_info.radio_nw_cause = x2ap_unspecified_2;

	if (X2AP_TRUE == timeTowait_present)
	{
		pENB_config_upd_resp_fail->bitmask = X2AP_TIME_TO_WAIT_INDEX_PRESENT;
		pENB_config_upd_resp_fail->time_to_wait = (x2ap_time_to_wait_et)p_x2ap_gl_ctx->
			config_param.timer_conf_info.x2ap_time_to_wait_index; 
	}

	X2AP_MEMSET(&(pENB_config_upd_resp_fail->criticality_diagnostics), 0x00, sizeof(x2ap_crit_diagno_t));
}

/*****************************************************************************
 * Function Name  : x2ap_enb_config_update_response_timer_expiry 
 * Inputs         : p_timer_buf - pointer to timer buffer
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes ths eNB configuaration update timer 
 *                  expiry responce
 ********************************************************************************/
void x2ap_enb_config_update_response_timer_expiry(x2ap_timer_buffer_t *p_timer_buf, x2ap_gb_context_t *p_x2ap_gb_ctx)
{
	x2ap_peer_enb_context_t* p_enb_cntx =X2AP_P_NULL;
	p_enb_cntx = (x2ap_peer_enb_context_t*)p_timer_buf->p_timer_specific_data;

	if(p_enb_cntx->enb_config_sm_ctx.state == X2AP_ENB_CONFIG_W_CONF_RESP_ST)
	{
		/* Stop X2AP_ENB_CONFIG_UPDATE_RSP_TIMER*/
		x2ap_stop_timer(p_enb_cntx->enb_config_sm_ctx.req_timer_id);
        
        /* SPR 16743 FIX START */
        p_enb_cntx->enb_config_sm_ctx.req_timer_id = X2AP_P_NULL;
        /* SPR 16743 FIX STOP */

		if(p_enb_cntx->enb_config_sm_ctx.req_retry_count>0)
		{
			U8 asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
			U16 asn_msg_len = X2AP_NULL;

			X2AP_TRACE(X2AP_INFO,"eNB Configuration Update retry count:%u", p_enb_cntx->enb_config_sm_ctx.req_retry_count);
			p_enb_cntx->enb_config_sm_ctx.req_retry_count--;

			X2AP_TRACE(X2AP_INFO,"eNB Configuration Update retry left:%u", p_enb_cntx->enb_config_sm_ctx.req_retry_count);

			if(!(x2ap_encode_eNB_Configuration_Update_req(p_x2ap_gb_ctx,&p_enb_cntx->enb_config_sm_ctx.enb_config_req, asn_msg_buff, &asn_msg_len)))
			{
				X2AP_TRACE(X2AP_ERROR,"Encoding eNB Configuration Update failed");;
				return;
			}
			X2AP_TRACE(X2AP_INFO,"eNB Configuration Update sent to Peer eNB again.\n");

			/* Send eNB CONFIGURATION UPDATE to peer eNB */
            /*SPR_18154_START*/
            if(p_enb_cntx->op_status == OPERATIONAL)
            {
			if(!(x2ap_send_enb_config_update_req_peer(p_x2ap_gb_ctx, p_enb_cntx, asn_msg_buff, asn_msg_len, X2AP_ENB_CONFIG_UPDATE_RSP_TIMER)))
			{
				X2AP_TRACE(X2AP_ERROR,"Sending eNB Configuration Update failed");
				return;
			}
		}
            /*SPR_18154_END*/
		}
		else
		{
			x2ap_enb_config_update_resp_t enb_cfg_resp;

			X2AP_TRACE(X2AP_INFO, "eNB Configuration Update retry count exhausted\n");
			X2AP_MEMSET(&enb_cfg_resp, 0x00, sizeof(x2ap_enb_config_update_resp_t));

			X2AP_MEMCPY(&enb_cfg_resp.enb2_gb_id, &p_enb_cntx->gb_enb_id , sizeof(x2_gb_enb_id_t));

			/* spr_fix_12819_start */
			enb_cfg_resp.cause.type = X2AP_CAUSE_INTERNAL;
			enb_cfg_resp.cause.value = ENB_CONFIG_UPDATE_TIMER_EXPIRY;
			/* spr_fix_12819_stop */

			/* Send eNB Configuration Update response */
			x2ap_send_enb_config_update_response(p_x2ap_gb_ctx, &enb_cfg_resp, X2AP_FAILURE, p_enb_cntx->enb_config_sm_ctx.dst_module_id, p_enb_cntx->enb_config_sm_ctx.transaction_id);
			X2AP_MEMSET(&p_enb_cntx->enb_config_sm_ctx, 0x00, sizeof(x2ap_enb_config_sm_context_t));
			p_enb_cntx->enb_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
		}
	}
	else
	{
		X2AP_TRACE(X2AP_ERROR, "Invalid state:%u.\n", p_enb_cntx->enb_config_sm_ctx.state);
	}
}

/*****************************************************************************
 * Function Name  : x2ap_enb_config_update_time_to_wait_expiry 
 * Inputs         : p_timer_buf - pointer to timer buffer 
 *                  p_x2ap_gb_ctx - pointer to x2AP global context
 * Outputs        : None
 * Returns        : None 
 * Description    : This function processes the enB config update time to wait  
 ********************************************************************************/
void x2ap_enb_config_update_time_to_wait_expiry(x2ap_timer_buffer_t *p_timer_buf, x2ap_gb_context_t *p_x2ap_gb_ctx)
{
	x2ap_peer_enb_context_t* p_enb_cntx =X2AP_P_NULL;
	p_enb_cntx = (x2ap_peer_enb_context_t*)p_timer_buf->p_timer_specific_data;

	if(p_enb_cntx->enb_config_sm_ctx.state == X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST)
	{
		/* Stop X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER*/
		x2ap_stop_timer(p_enb_cntx->enb_config_sm_ctx.time_to_wait_timer_id);
        
        /* SPR 16743 FIX START */
        p_enb_cntx->enb_config_sm_ctx.time_to_wait_timer_id = X2AP_P_NULL;
        /* SPR 16743 FIX STOP */

		if(p_enb_cntx->enb_config_sm_ctx.time_to_wait_retry_count>0)
		{
			U8 asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
			U16 asn_msg_len = X2AP_NULL;

			X2AP_TRACE(X2AP_INFO,"eNB Configuration Update time to wait retry count:%u", p_enb_cntx->enb_config_sm_ctx.time_to_wait_retry_count);

			p_enb_cntx->enb_config_sm_ctx.time_to_wait_retry_count--;

			X2AP_TRACE(X2AP_INFO,"eNB Configuration Update time to wait retry left:%u", p_enb_cntx->enb_config_sm_ctx.time_to_wait_retry_count);

			if(!(x2ap_encode_eNB_Configuration_Update_req(p_x2ap_gb_ctx,&p_enb_cntx->enb_config_sm_ctx.enb_config_req, asn_msg_buff, &asn_msg_len)))
			{
				X2AP_TRACE(X2AP_ERROR,"Encoding eNB Configuration Update failed");;
				return;
			}
			X2AP_TRACE(X2AP_INFO,"eNB Configuration Update sent to Peer eNB again.\n");

			/* Send eNB CONFIGURATION UPDATE to peer eNB */
			if(!(x2ap_send_enb_config_update_req_peer(p_x2ap_gb_ctx, p_enb_cntx, asn_msg_buff, asn_msg_len, X2AP_ENB_CONFIG_UPDATE_RSP_TIMER)))
			{
				X2AP_TRACE(X2AP_ERROR,"Sending eNB Configuration Update failed");
				return;
			}
		}
		else
		{
			x2ap_enb_config_update_resp_t enb_cfg_resp;

			X2AP_TRACE(X2AP_INFO, "eNB Configuration Update retry count exhausted\n");
			X2AP_MEMSET(&enb_cfg_resp, 0x00, sizeof(x2ap_enb_config_update_resp_t));

			X2AP_MEMCPY(&enb_cfg_resp.enb2_gb_id, &p_enb_cntx->gb_enb_id , sizeof(x2_gb_enb_id_t));

			enb_cfg_resp.cause.type = T_x2ap_Cause_radioNetwork;
			enb_cfg_resp.cause.value = x2ap_unspecified_2;

			/* Send eNB Configuration Update response */
			x2ap_send_enb_config_update_response(p_x2ap_gb_ctx, &enb_cfg_resp, X2AP_FAILURE, p_enb_cntx->enb_config_sm_ctx.dst_module_id, p_enb_cntx->enb_config_sm_ctx.transaction_id);
			X2AP_MEMSET(&p_enb_cntx->enb_config_sm_ctx, 0x00, sizeof(x2ap_enb_config_sm_context_t));
			p_enb_cntx->enb_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
		}
	}
	else
	{
		X2AP_TRACE(X2AP_ERROR, "Invalid state:%u.\n", p_enb_cntx->enb_config_sm_ctx.state);
	}

}

/*****************************************************************************
 * Function Name  : x2ap_send_enb_configuration_update_ack 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNb context
 *                  p_eNB_config_upd_resp - pointer to x2ap_enb_conf_ack_t
 *                  p_asn_msg_buff - pointre to ASN encoded buffer
 *                  p_asn_msg_len - pointer to  ASN encoded buffer sizw
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends eNB configuration update ack
 ********************************************************************************/
void
x2ap_send_enb_configuration_update_ack(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t* p_enb_cntx,
		x2ap_enb_conf_ack_t* p_eNB_config_upd_resp,
		U8* p_asn_msg_buff,
		U16* p_asn_msg_len)
{
	x2ap_return_et retVal = X2AP_FAILURE;
	x2ap_peer_enb_msg_t  peer_enb_msg;

	X2AP_MEMSET(&peer_enb_msg, X2AP_NULL, sizeof(x2ap_peer_enb_msg_t));

	retVal = x2ap_encode_eNB_Configuration_Update_ack(p_eNB_config_upd_resp, p_asn_msg_buff, p_asn_msg_len);

	if(X2AP_SUCCESS ==retVal)
	{
		peer_enb_msg.p_msg = p_asn_msg_buff;
		peer_enb_msg.msg_len = *p_asn_msg_len;
		peer_enb_msg.ue_ass_msg=X2AP_FALSE;
        /* SPR_14436_Fix Start */
	    peer_enb_msg.stream_id = p_enb_cntx->stream_info.
		   stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
	    /* SPR_14436_Fix Stop */


		retVal = x2ap_sctp_sm_entry (X2AP_SCTP_SEND_DATA_EV, (U8 *)(&peer_enb_msg), p_x2ap_gb_ctx, p_enb_cntx);
		if (X2AP_SUCCESS == retVal)
		{
			X2AP_TRACE(X2AP_INFO,"eNB CONFIGURATION UPDATE ACKNOWLEDGE sent to peer eNB.\n");
		}
		else
		{
			X2AP_TRACE(X2AP_ERROR,"eNB CONFIGURATION UPDATE ACKNOWLEDGE sending failed to peer eNB.\n");
		}
	}
	else
	{
		X2AP_TRACE(X2AP_ERROR,"eNB CONFIGURATION UPDATE ACKNOWLEDGE encoding failed.\n");
	}
}


/*****************************************************************************
 * Function Name  : x2ap_build_and_send_enb_configuration_update_failure 
 * Inputs         : p_x2ap_gb_ctx - pointer tp X2AP global contxet
 *                  p_enb_cntx - peer eNb context
 *                  p_eNB_config_upd_fail - pointer to p_eNB_config_upd_fail,
 *                  p_asn_msg_buff - pointer to ASN encoded buffer
 *                  p_asn_msg_len -  pointer to ASN encoded buffer length 
 *                  timeTowait_present - time to wait is there or not
 * Outputs        : None
 * Returns        : None
 * Description    : this function builds and sends eNB config update failure
 ********************************************************************************/
void
x2ap_build_and_send_enb_configuration_update_failure(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t* p_enb_cntx,
		x2ap_enb_conf_upd_fail_t* p_eNB_config_upd_fail,
		U8* p_asn_msg_buff,
		U16* p_asn_msg_len,
		U8 timeTowait_present)
{
	x2ap_return_et retVal = X2AP_FAILURE;

	x2ap_build_eNB_config_upd_resp_fail(p_eNB_config_upd_fail, p_x2ap_gb_ctx, (x2ap_bool_et)timeTowait_present);
	retVal = x2ap_encode_eNB_Configuration_Update_failure_resp(p_eNB_config_upd_fail, p_asn_msg_buff, p_asn_msg_len);

	if (X2AP_SUCCESS == retVal)
	{
		x2ap_peer_enb_msg_t  peer_enb_msg;
		X2AP_MEMSET(&peer_enb_msg, X2AP_NULL, sizeof(x2ap_peer_enb_msg_t));

		peer_enb_msg.p_msg = p_asn_msg_buff;
		peer_enb_msg.msg_len = *p_asn_msg_len;
		/* SPR_14436_Fix Start */
		peer_enb_msg.stream_id = p_enb_cntx->stream_info.
			stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
		/* SPR_14436_Fix Stop */

		retVal = x2ap_sctp_sm_entry (X2AP_SCTP_SEND_DATA_EV, ((U8 *)&peer_enb_msg), p_x2ap_gb_ctx, p_enb_cntx);
		if (X2AP_SUCCESS == retVal)
		{
			X2AP_TRACE(X2AP_INFO,"eNB CONFIGURATION UPDATE FAILURE sent to peer eNB.\n");
		}
		else
		{
			X2AP_TRACE(X2AP_INFO,"eNB CONFIGURATION UPDATE FAILURE sending failed to peer eNB.\n");
		}
	}
	else
	{
		X2AP_TRACE(X2AP_ERROR,"eNB CONFIGURATION UPDATE FAILURE encoding failed.\n");
	}
}

/*****************************************************************************
 * Function Name  : x2ap_process_neigh_ecgi_enb_id_mapping
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_neigh_ecgi_enb_id_map - pointer to 
 *                                            x2ap_neigh_ecgi_enb_id_map_list_t
 * Outputs        : p_x2ap_gb_ctx
 * Returns        : None
 * Description    : this function process_neighbour  ecgi enb id mapping
 ********************************************************************************/
void x2ap_process_neigh_ecgi_enb_id_mapping(
		x2ap_gb_context_t *p_x2ap_gb_ctx,
		x2ap_neigh_ecgi_enb_id_map_list_t *p_neigh_ecgi_enb_id_map)
{
	U16 neigh_info_count = 0;
	S32 nbour_cell_idx = 0;

	X2AP_UT_TRACE_ENTER();

	for(neigh_info_count = 0; 
			neigh_info_count < p_neigh_ecgi_enb_id_map->neigh_info_count; neigh_info_count++)
	{
		nbour_cell_idx = x2ap_get_nbr_index_from_ecgi(p_x2ap_gb_ctx,
				p_neigh_ecgi_enb_id_map->neigh_ecgi_enb_id_info[neigh_info_count].ecgi, 
				X2AP_FALSE, 0);
		/*klockwork Fix*/
		if (nbour_cell_idx != -1)
		{
			if(!(X2AP_MEMCMP(&(p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].peer_cell_info.ecgi),
							&(p_neigh_ecgi_enb_id_map->neigh_ecgi_enb_id_info[neigh_info_count].ecgi),
							sizeof(x2ap_ecgi_t))))
			{
				X2AP_TRACE(X2AP_DETAILED,"Peer eNB Cell found, updating the gb_enb_id.\n");
				X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].peer_gb_enb_id),
						(const void*)(&(p_neigh_ecgi_enb_id_map->neigh_ecgi_enb_id_info[neigh_info_count].enb_id)),
						sizeof(x2_gb_enb_id_t));
			}
		}
		else
		{
			X2AP_TRACE(X2AP_WARNING, "Incorrect neighbour cell index:%u.\n", nbour_cell_idx);
		}/*klockwork Fix*/
	}

	X2AP_UT_TRACE_EXIT();
}

#ifdef ENDC_ENABLED
/*EN_DC_CHANGES_START*/
/*****************************************************************************
 * Function Name  : x2ap_delete_nbour_nr_cell_info 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global data 
 *                  cell_id - cell id 
 * Outputs        : None
 * Returns        : None
 * Description    : This function delete nbour nr cell infoformation 
 ********************************************************************************/
void x2ap_delete_nbour_nr_cell_info(x2ap_gb_context_t* p_x2ap_gb_ctx, U8 cell_id)
{
    U16 num_nbour =0;
    U16 nbour_count =0;
    U8 count = 0;
    x2ap_nr_ecgi_t nrcgi;
    x2ap_nbr_nr_cgi_search_node_t *p_node = X2AP_P_NULL;

    X2AP_MEMSET(&nrcgi, 0, sizeof(x2ap_nr_ecgi_t));

    if (cell_id < MAX_SERVED_CELLS)
    {
        p_node = (x2ap_nbr_nr_cgi_search_node_t *)x2ap_st_find_tree_node((x2ap_search_tree_t *)&(p_x2ap_gb_ctx->nbr_nr_cgi_search_tree), (void*)(&nrcgi) );
        if(p_node== X2AP_P_NULL)
        {
            X2AP_TRACE(X2AP_WARNING, "Neighbour NRCGI not found in tree.\n");
        }
         else
         {
             X2AP_TRACE(X2AP_INFO,"logical_id:%d\n", p_node->logical_id);
             X2AP_MEMSET(&p_x2ap_gb_ctx->peer_enb_cell_info[p_node->logical_id], 0x00, sizeof(x2ap_peer_enb_cell_info_t));
             x2ap_st_delete_node(&p_x2ap_gb_ctx->nbr_nr_cgi_search_tree, &(p_node->nodeAnchor));
             x2ap_mem_free(&p_node->nodeAnchor);

             p_x2ap_gb_ctx->num_peer_enb_cells--;
	 }
    }
}

/*****************************************************************************
 * Function Name  : x2ap_populate_served_nr_cell_info 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_neighbour_cell_info - pointer to x2ap_nr_neigh_info_t
 *                  peer_enodeb_id - peer gNB id
 * Outputs        : p_x2ap_gb_ctx
 * Returns        : None
 * Description    : this function populates neighbour cell information in to the 
 *                  global context
 ********************************************************************************/
void x2ap_populate_served_nr_cell_info(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_nr_neigh_info_t* p_neighbour_cell_info,
		x2ap_nr_ecgi_t *p_old_nr_cgi,
		U8 peer_enodeb_id)
{
    U8 peer_enb_ct = X2AP_NULL;
    U8 cell_ct = X2AP_NULL;
    x2ap_bool_et b_found = X2AP_FALSE;

    X2AP_ASSERT(PNULL!= p_x2ap_gb_ctx);
    X2AP_ASSERT(PNULL!= p_neighbour_cell_info);
    X2AP_ASSERT(PNULL!= p_old_nr_cgi);

    X2AP_UT_TRACE_ENTER();

    if ( MAX_PEER_GNB <= peer_enodeb_id )
    {
        X2AP_TRACE(X2AP_WARNING,"Peer EnodeB Id received is out of range : %d",
            peer_enodeb_id);

        X2AP_UT_TRACE_EXIT();
        return;
    }

    for ( peer_enb_ct = X2AP_NULL; peer_enb_ct < MAX_PEER_GNB && X2AP_FALSE == b_found; peer_enb_ct++ )
    {
        if ( peer_enb_ct == peer_enodeb_id )
        {
            x2ap_nr_ecgi_t * p_temp_nr_ecgi = (x2ap_nr_ecgi_t *)p_x2ap_gb_ctx->x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info;

            for (cell_ct = 0; 
                     (cell_ct < p_x2ap_gb_ctx->x2ap_link_info[peer_enb_ct].num_served_cells) 
                     && cell_ct < MAX_SERVED_CELLS
                      && (p_temp_nr_ecgi) ; cell_ct++)
            {
                 if(!X2AP_MEMCMP(&p_temp_nr_ecgi[cell_ct],p_old_nr_cgi,sizeof(x2ap_nr_ecgi_t)))
                 {
                    X2AP_MEMCPY((void *)p_x2ap_gb_ctx->x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info + (sizeof(x2ap_nr_ecgi_t) * cell_ct),
                                    (const void*)(&p_neighbour_cell_info->nr_cell_id),
                                    sizeof(x2ap_nr_ecgi_t));
                                    
                    b_found = X2AP_TRUE;
                    break;
                 }
            }

            if (X2AP_FAILURE == b_found && MAX_SERVED_CELLS > cell_ct)
            {
                X2AP_MEMCPY((void *)p_x2ap_gb_ctx->x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info + (sizeof(x2ap_nr_ecgi_t) * cell_ct),
                    (const void*)(&p_neighbour_cell_info->nr_cell_id),sizeof(x2ap_nr_ecgi_t));

                p_x2ap_gb_ctx->x2ap_link_info[peer_enb_ct].num_served_cells++;    
                b_found = X2AP_TRUE;
            }
            else if (MAX_SERVED_CELLS <= cell_ct)
            {
                X2AP_TRACE(X2AP_WARNING,"update nr_cgi of link_info received is out of link_info num_served_cells range");
            }
        }  
    }
    X2AP_UT_TRACE_EXIT();
}	

/*****************************************************************************
 * Function Name  : x2ap_populate_nbour_nr_cell_info 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_neighbour_cell_info - pointer to x2ap_nr_neigh_info_t
 *                  served_cell_id - served cell id
 *                  p_gb_enb_id - global eNb id
 * Outputs        : p_x2ap_gb_ctx
 * Returns        : None
 * Description    : this function populates neighbour cell information in to the 
 *                  global context
 ********************************************************************************/
void x2ap_populate_nbour_nr_cell_info(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_nr_neigh_info_t *p_neighbour_cell_info,
		U8 served_cell_id,
		x2_gb_gnb_id_t *p_gb_gnb_id)
{
    S32 nbour_cell_idx = 0;
    rrc_counter_t loop = 0;

    X2AP_UT_TRACE_ENTER();

    /* Get neighbour cell index */
    nbour_cell_idx = x2ap_get_nbr_index_from_nr_cgi(p_x2ap_gb_ctx, p_neighbour_cell_info->nr_cell_id, X2AP_TRUE, served_cell_id);

    if ((nbour_cell_idx == -1) || (nbour_cell_idx >= MAX_NEIGHBOURS))
    {
        X2AP_TRACE(X2AP_WARNING, "Incorrect neighbour cell index:%u.\n", nbour_cell_idx);
        X2AP_UT_TRACE_EXIT();
        return;
    }

    /* Copy ARFCN */
    X2AP_MEMCPY((void*)&(p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].peer_cell_info.nr_freq_info.nr_earfcn),
        (const void*)(&(p_neighbour_cell_info->nr_freq_info.nr_earfcn)),
        sizeof(U32));

    /* Copy PCI */
    X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].peer_cell_info.nr_pci),
        (const void*)(&(p_neighbour_cell_info->nr_pci)),
        sizeof(U32));

    /* Copy CGI */
    X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].peer_cell_info.nr_ecgi),
        (const void*)(&(p_neighbour_cell_info->nr_cell_id)),
        sizeof(x2ap_nr_ecgi_t));

    if (X2AP_NR_NEIGHBOUR_FDD_PRESENT & p_neighbour_cell_info->nr_neigh_Mode_Info.bitmask)
    {
        /* Copy FDD */
        X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].peer_cell_info.choice_nr_mode.nr_fdd_info.nr_dl_freq_info),
            (const void*)(&(p_neighbour_cell_info->nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info)),
            sizeof(x2ap_nr_freq_info_t));

        X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].peer_cell_info.choice_nr_mode.nr_fdd_info.nr_ul_freq_info),
            (const void*)(&(p_neighbour_cell_info->nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info)),
            sizeof(x2ap_nr_freq_info_t));    
    }
    else if (X2AP_NR_NEIGHBOUR_TDD_PRESENT & p_neighbour_cell_info->nr_neigh_Mode_Info.bitmask)
    {
        /* Copy TDD */
        X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].peer_cell_info.choice_nr_mode.nr_tdd_info.nr_freq_info),
            (const void*)(&(p_neighbour_cell_info->nr_neigh_Mode_Info.tdd_info.nr_freq_info)),
            sizeof(x2ap_nr_freq_info_t));  
    }

#ifndef LINUX_PC_TEST
    p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].peer_cell_info.meas_timing_config_count = 
        p_neighbour_cell_info->meas_timing_config_count;

    for (loop = 0; loop < p_neighbour_cell_info->meas_timing_config_count; loop++)    
    {
        /* Copy meas timing config */
        X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].peer_cell_info.x2ap_meas_timing_config[loop]),
            (const void*)(&(p_neighbour_cell_info->x2ap_meas_timing_config[loop])),
            sizeof(x2ap_meas_timing_config_t));
    }    
#endif

    /* tac */

    /* Copy GB_GNB_ID */
    if (NULL != p_gb_gnb_id)
    {
        X2AP_MEMCPY((void*)&(p_x2ap_gb_ctx->peer_enb_cell_info[nbour_cell_idx].peer_gb_gnb_id),
            (const void*)(p_gb_gnb_id),
            sizeof(x2_gb_gnb_id_t));
    }

    X2AP_TRACE(X2AP_INFO, "Neighbour cell with nr_pci:%u added for cell_id:%u.\n", p_neighbour_cell_info->nr_pci, served_cell_id);
    X2AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : x2ap_process_served_cell_to_add_list 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_served_cell-  pointer to x2ap_served_cells_to_add_list_t
 * Outputs        : None
 * Returns        : x2ap_return_et
 * Description    : This function addds the served cell Id to global context  
 ********************************************************************************/
x2ap_return_et x2ap_process_eutra_nr_served_cell_to_add_list
(
    x2ap_gb_context_t* p_x2ap_gb_ctx, 
    x2ap_served_eutra_nr_cells_to_add_list_t *p_served_cell
)
{
    U8 cell_count = 0;
    U8 local_cell_count = 0;

    X2AP_TRACE(X2AP_INFO, "%u existing serving cells\n", p_x2ap_gb_ctx->num_served_cells);
    X2AP_TRACE(X2AP_INFO, "%u serving cell to add present\n", p_served_cell->num_served_eutra_nr_cell);

    if (MAX_NUM_CELL_IN_X2AP < p_x2ap_gb_ctx->num_served_cells + p_served_cell->num_served_eutra_nr_cell)
    {
        X2AP_TRACE(X2AP_INFO, "existing and new add serving cells exceed max served cell.\n");
        return X2AP_FAILURE;
    }

    for (cell_count = 0; cell_count < p_served_cell->num_served_eutra_nr_cell; ++cell_count)
    {
        x2ap_cell_info_t* p_target = X2AP_P_NULL;
        x2ap_served_cell_info_t* p_source = X2AP_P_NULL;

        p_source = &p_served_cell->eutra_nr_cell_list[cell_count].eutra_cell_info;

        if (MAX_SERVED_CELLS > p_source->cell_id)
        {
            p_target = &p_x2ap_gb_ctx->served_cell_info[p_source->cell_id];

            /* Initialise active served cell info in Global eNB Context */

            /* Update Served cell info in Global eNB Context */
            x2ap_populate_served_cell_info(p_x2ap_gb_ctx, p_target, p_source);

            p_x2ap_gb_ctx->num_served_cells++;
    		
            if (NR_NEIGHBOUR_INFO_PRESENT & p_served_cell->eutra_nr_cell_list[cell_count].bitmask)  
            {
                if(0 == p_served_cell->eutra_nr_cell_list[cell_count].nr_neighbour_cell_info.num_neigh_cell)
                {
                    X2AP_TRACE(X2AP_INFO, "No neighbour nr cell present for %u serving cell with cell id:%u\n", cell_count, p_source->cell_id);
                }
                else
                {
                    U16 nbour_count = 0;
                    x2ap_nr_nbour_cell_info_t *p_nbour_cell_info = &p_served_cell->eutra_nr_cell_list[cell_count].nr_neighbour_cell_info;

                    X2AP_TRACE(X2AP_INFO, "%u neighbour nr cell present for %u serving cell with cell id:%u\n", 
        		    p_nbour_cell_info->num_neigh_cell, cell_count, p_source->cell_id);

                    /* Populate neighbour nr cell info */
                    for (nbour_count = 0; nbour_count < p_nbour_cell_info->num_neigh_cell; ++nbour_count)
                    {
                        x2ap_populate_nbour_nr_cell_info(p_x2ap_gb_ctx, 
        		        &p_nbour_cell_info->nr_neigh_cell_arr[nbour_count].nr_neigh_info, p_source->cell_id, NULL);
                    }
                }
            }
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR, "Invalid cell_id:%u\n", p_source->cell_id);
            return X2AP_FAILURE;
        }
    }

    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_process_eutra_nr_served_cell_to_modify_list 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_served_cell-  pointer to x2ap_served_eutra_nr_cells_to_modify_list_t
 * Outputs        : None
 * Returns        : x2ap_return_et
 * Description    : This function addds the served cell Id to global context  
 ********************************************************************************/
x2ap_return_et x2ap_process_eutra_nr_served_cell_to_modify_list
(
    x2ap_gb_context_t* p_x2ap_gb_ctx, 
    x2ap_served_eutra_nr_cells_to_modify_list_t *p_served_cell
)
{
    U8 cell_count = 0;
    x2ap_cell_info_t* p_target = X2AP_P_NULL;
    x2ap_served_cell_info_t* p_source = X2AP_P_NULL;

    X2AP_TRACE(X2AP_INFO, "%u serving cell to modify present\n", p_served_cell->num_served_eutra_nr_cell);

    if (MAX_SERVED_CELLS < p_served_cell->num_served_eutra_nr_cell)
    {
        X2AP_FAILURE;
    }

    /* Coverity Start */
    for (cell_count = 0; cell_count < p_served_cell->num_served_eutra_nr_cell; cell_count++)
    {
        p_source = &p_served_cell->eutra_nr_cell_list[cell_count].served_eutra_nr_cell_info.eutra_cell_info;

        if (MAX_SERVED_CELLS > p_source->cell_id)
        {
            p_target = &p_x2ap_gb_ctx->served_cell_info[p_source->cell_id];

            if ( p_served_cell->eutra_nr_cell_list[cell_count].old_cell_id < MAX_SERVED_CELLS )
            { 
                /*Start*/
                /* Update Served cell info in Global eNB Context */
                x2ap_populate_served_cell_info(p_x2ap_gb_ctx, p_target, p_source);
       
                if(0 == p_served_cell->eutra_nr_cell_list[cell_count].served_eutra_nr_cell_info.nr_neighbour_cell_info.num_neigh_cell)
                {
                    X2AP_TRACE(X2AP_INFO, "cell_count=%u No neighbour nr cell present for serving cell with cell_index:%u\n", cell_count, p_source->cell_id);
                }
                else
                {
                    U16 nbour_count = 0;
                    x2ap_nr_nbour_cell_info_t *p_nbour_cell_info = 
                        &p_served_cell->eutra_nr_cell_list[cell_count].served_eutra_nr_cell_info.nr_neighbour_cell_info;

                    X2AP_TRACE(X2AP_INFO, "%u neighbour nr cell present for serving cell with cell_index:%u\n", 
    		        p_nbour_cell_info->num_neigh_cell, p_source->cell_id);

                    /* Populate neighbour nr cell info */
                    for (nbour_count = 0; nbour_count < p_nbour_cell_info->num_neigh_cell; ++nbour_count)
                    {
                        x2ap_populate_nbour_nr_cell_info(
                            p_x2ap_gb_ctx, &p_nbour_cell_info->nr_neigh_cell_arr[nbour_count].nr_neigh_info, 
                            p_source->cell_id, NULL);
                    }
                }
            }
            else
            {
                X2AP_TRACE(X2AP_ERROR, "Invalid old cell_id:%u\n", p_served_cell->eutra_nr_cell_list[cell_count].old_cell_id);
                return X2AP_FAILURE;
            }
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR, "Invalid cell_id:%u\n", p_source->cell_id);
            return X2AP_FAILURE;
        }
	/* Stop */
    }

    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_process_served_cell_to_add_list 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_served_cell-  pointer to x2ap_served_cells_to_add_list_t
 * Outputs        : None
 * Returns        : x2ap_return_et
 * Description    : This function addds the served cell Id to global context  
 ********************************************************************************/
x2ap_return_et x2ap_process_eutra_nr_served_cell_to_delete_list(x2ap_gb_context_t* p_x2ap_gb_ctx, 
    x2ap_served_eutra_nr_cells_to_delete_list_t *p_served_cell)
{
	U8 cell_count = 0;

	X2AP_TRACE(X2AP_INFO, "%u serving cell to delete present\n", p_served_cell->num_served_cell);

	if (MAX_SERVED_CELLS < p_served_cell->num_served_cell)
	{
	    return X2AP_FAILURE;
	}

	/* Coverity Start */
	for (cell_count = 0; cell_count < p_served_cell->num_served_cell; cell_count++)
	{
	    if ( p_served_cell->cell_id_list[cell_count] < MAX_SERVED_CELLS )
	    {
		X2AP_TRACE(X2AP_INFO, "Delete served cell with cell_id %u info from global context.\n", p_served_cell->cell_id_list[cell_count]);
                /* delete neighbour cell */
		x2ap_delete_nbour_cell_info(p_x2ap_gb_ctx, p_served_cell->cell_id_list[cell_count]);
                /* delete neighbour nr cell */
		x2ap_delete_nbour_nr_cell_info(p_x2ap_gb_ctx, p_served_cell->cell_id_list[cell_count]);
		X2AP_MEMSET(&p_x2ap_gb_ctx->served_cell_info[p_served_cell->cell_id_list[cell_count]], 0x00, sizeof(x2ap_cell_info_t));
	    }
	    else
	    {
		X2AP_TRACE(X2AP_ERROR, "Invalid Cell_ID :%u\n", p_served_cell->cell_id_list[cell_count]);
		return X2AP_FAILURE;
	    }
	    /* Coverity Stop */
	    p_x2ap_gb_ctx->num_served_cells--;
	}

	return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_apply_endc_config_update_req 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_endc_cfg_req - pointer to x2ap_endc_config_update_req_t
 * Outputs        : None
 * Returns        : x2ap_return_et
 * Description    : This function applies the ENDC configuraton update request 
 ********************************************************************************/
x2ap_return_et x2ap_apply_endc_config_update_req
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,   
    x2ap_endc_config_update_req_t *p_endc_cfg_req
)
{   
    x2ap_return_et retVal = X2AP_SUCCESS;
    
    if(X2AP_ENDC_CONFIGUPDATE_ENB_CELLASSISTANCE_INFO_PRESENT & p_endc_cfg_req->bitmask)   
    {      
        X2AP_TRACE(X2AP_INFO, "Cell assistance info present.\n");

        if (X2AP_CELL_ASSISTANCE_FULL_LIST_PRESENT & p_endc_cfg_req->cell_assistance_info.bitmask)
        {
            X2AP_TRACE(X2AP_INFO, "Cell assistance info:full list present.full ind:%d.\n", 
                p_endc_cfg_req->cell_assistance_info.full_nr_cells_ind);
        }
        else if (X2AP_CELL_ASSISTANCE_LIMITED_LIST_PRESENT & p_endc_cfg_req->cell_assistance_info.bitmask)
        {
            X2AP_TRACE(X2AP_INFO, "Cell assistance info:partial list present.%d num of nr cgi.\n", 
                p_endc_cfg_req->cell_assistance_info.partial_nr_cells.nrcgi_count);
        }
    }   
    else
    {        
        X2AP_TRACE(X2AP_INFO, "Cell assistance info not present.\n");
    }

   if(X2AP_SERVED_EUTRA_CELLS_TO_ADD_LIST_PRESENT & p_endc_cfg_req->bitmask)
   {
        X2AP_TRACE(X2AP_INFO, "Served cells to add list present.\n");
        retVal &= x2ap_process_eutra_nr_served_cell_to_add_list(p_x2ap_gb_ctx, &p_endc_cfg_req->add_served_cell);
    }
    else
    {      
        X2AP_TRACE(X2AP_INFO, "Served cells to add list not present.\n");
    }
   
    if(X2AP_SERVED_EUTRA_CELLS_TO_MODIFY_LIST_PRESENT & p_endc_cfg_req->bitmask)
    {
        X2AP_TRACE(X2AP_INFO, "Served cells to modify list present.\n");
        retVal &= x2ap_process_eutra_nr_served_cell_to_modify_list(p_x2ap_gb_ctx, &p_endc_cfg_req->mod_served_cell);
    }
    else
    {     
        X2AP_TRACE(X2AP_INFO, "Served cells to modify list not present.\n");
    }
   
    if(X2AP_SERVED_EUTRA_CELLS_TO_DELETE_LIST_PRESENT & p_endc_cfg_req->bitmask)
    {
        X2AP_TRACE(X2AP_INFO, "Served cells to delete list present.\n");
        retVal &= x2ap_process_eutra_nr_served_cell_to_delete_list(p_x2ap_gb_ctx, &p_endc_cfg_req->del_served_cell);
    }
    else
    {      
        X2AP_TRACE(X2AP_INFO, "Served cells to delete list not present.\n");
    }

    return retVal;
}

void x2ap_build_nr_srvd_cell_to_management_list
(  
    x2ap_nr_cell_info_t *p_nr_cell_info,
    x2ap_nr_srvd_cell_to_management_list_t *p_nr_srvd_cell_to_management_list
)
{
    U16 srvd_cell_count = X2AP_NULL;
    
    X2AP_UT_TRACE_ENTER();

    p_nr_srvd_cell_to_management_list->num_nr_srvd_cell_to_management = 
        p_nr_cell_info->num_served_nr_cell;

    for (srvd_cell_count = 0; srvd_cell_count < p_nr_srvd_cell_to_management_list->num_nr_srvd_cell_to_management
        && srvd_cell_count < MAX_SERVED_CELLS;
        srvd_cell_count++)
    {
        p_nr_srvd_cell_to_management_list->nr_served_cell_management_list[srvd_cell_count].bitmask = 0;
 
        x2ap_build_rrm_srvd_nr_cell_info(&p_nr_cell_info->served_nr_cell_arr[srvd_cell_count], 
            &p_nr_srvd_cell_to_management_list->nr_served_cell_management_list[srvd_cell_count].served_cell_info);

        /* nr neighbour cell info not add */
    }

    X2AP_UT_TRACE_EXIT();
}

void x2ap_build_nr_srvd_cell_to_modify_list
(
    x2ap_nr_srvd_cell_to_modify_t *p_x2ap_nr_srvd_cell_modify,
    x2ap_nr_srvd_cell_to_modify_list_t *p_rrm_nr_srvd_cell_modify)
{
    U16 srvd_cell_count = X2AP_NULL;
    
    X2AP_UT_TRACE_ENTER();

    p_rrm_nr_srvd_cell_modify->num_nr_srvd_cell_to_modify = 
        p_x2ap_nr_srvd_cell_modify->num_nr_srvd_cell_modify;

    for (srvd_cell_count = 0; srvd_cell_count < p_rrm_nr_srvd_cell_modify->num_nr_srvd_cell_to_modify
        && srvd_cell_count < MAX_SERVED_CELLS;
        srvd_cell_count++)
    {
        p_rrm_nr_srvd_cell_modify->nr_served_cell_modify_list[srvd_cell_count].bitmask = 0;
       
        X2AP_MEMCPY((void *)&p_rrm_nr_srvd_cell_modify->nr_served_cell_modify_list[srvd_cell_count].old_nr_cgi,
            (const void*)(&p_x2ap_nr_srvd_cell_modify->nr_cell_infos[srvd_cell_count].old_nr_cgi),
            sizeof(x2ap_nr_ecgi_t));

        if (X2AP_ENDC_X2_NR_DEACT_IND_PRESENT & 
            p_x2ap_nr_srvd_cell_modify->nr_cell_infos[srvd_cell_count].bitmask)
        {
            p_rrm_nr_srvd_cell_modify->nr_served_cell_modify_list[srvd_cell_count].bitmask |=
                RRC_ENDC_X2_NR_DEACT_IND_PRESENT;
            
            p_rrm_nr_srvd_cell_modify->nr_served_cell_modify_list[srvd_cell_count].deact_ind =
                p_x2ap_nr_srvd_cell_modify->nr_cell_infos[srvd_cell_count].deact_ind;
        
        }
 
        x2ap_build_rrm_srvd_nr_cell_info(&p_x2ap_nr_srvd_cell_modify->nr_cell_infos[srvd_cell_count].srvd_nr_cell_info, 
            &p_rrm_nr_srvd_cell_modify->nr_served_cell_modify_list[srvd_cell_count].served_cell_info);
       
            /* nr neighbour cell info not add */
    }

    X2AP_UT_TRACE_EXIT();
}

void x2ap_build_nr_srvd_cell_to_delete_list
(
    x2ap_nr_srvd_cell_to_delete_t *p_x2ap_nr_srvd_cell_delete,
    x2ap_nr_srvd_cell_to_delete_list_t *p_rrm_nr_srvd_cell_delete_list)
{
    U16 srvd_cell_count = X2AP_NULL;

    X2AP_UT_TRACE_ENTER();

    p_rrm_nr_srvd_cell_delete_list->num_nr_srvd_cell_delete = 
        p_x2ap_nr_srvd_cell_delete->num_delete_nrcells_endc;
    
    for (srvd_cell_count = 0; srvd_cell_count < p_rrm_nr_srvd_cell_delete_list->num_nr_srvd_cell_delete
        && srvd_cell_count < MAX_SERVED_CELLS;
        srvd_cell_count++)
    {
        X2AP_MEMCPY((void *)&p_rrm_nr_srvd_cell_delete_list->nr_served_cell_delete_list[srvd_cell_count],
          (const void*)(&p_x2ap_nr_srvd_cell_delete->eutra_nr_cell_delete[srvd_cell_count]),
          sizeof(x2ap_nr_ecgi_t));
    }

    X2AP_UT_TRACE_EXIT();
}

void x2ap_build_endc_config_update_ind
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_peer_enb_context_t* p_enb_cntx,
    x2_config_update_endc_en_gnb_t *p_endc_config_update_req, 
    x2ap_endc_config_update_en_gnb_ind_t *p_endc_config_update_ind
)
{
    X2AP_UT_TRACE_ENTER();
          
    X2AP_MEMCPY((void *)&p_endc_config_update_ind->global_en_gnb_id,
        (const void*)(&p_enb_cntx->gb_gnb_id),
        sizeof(x2_gb_gnb_id_t));
    
    if (X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MANAGEMENTLIST_PRESENT & 
        p_endc_config_update_req->bitmask) 
    {
        p_endc_config_update_ind->bitmask |= 
            X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MANAGEMENTLIST_PRESENT;
        
        x2ap_build_nr_srvd_cell_to_management_list(&p_endc_config_update_req->eutra_nr_cell_management,
            &p_endc_config_update_ind->eutra_nr_cell_management);
    }

    if (X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MODIFYLIST_PRESENT & 
        p_endc_config_update_req->bitmask) 
    {
        p_endc_config_update_ind->bitmask |= 
            X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MODIFYLIST_PRESENT;
        
        x2ap_build_nr_srvd_cell_to_modify_list(&p_endc_config_update_req->eutra_nr_cell_modify,
            &p_endc_config_update_ind->eutra_nr_cell_modify);
    }
    
    if (X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_DELETELIST_PRESENT & 
        p_endc_config_update_req->bitmask) 
    {
        p_endc_config_update_ind->bitmask |= 
            X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_DELETELIST_PRESENT;

        x2ap_build_nr_srvd_cell_to_delete_list(&p_endc_config_update_req->eutra_nr_cell_delete,
            &p_endc_config_update_ind->eutra_nr_cell_delete);
    }
    
    X2AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : x2ap_build_endc_config_upd_resp_fail 
 * Inputs         : pendc_config_upd_resp_fail - pointer to x2ap_endc_config_update_failure_t
 *                  p_x2ap_gl_ctx - pointer to x2AP global context
 *                  timeTowait_present - type of x2ap_bool_et
 * Outputs        : pENB_config_upd_resp_fail
 * Returns        : None
 * Description    : This function builds the the eNB configuaration update 
 *                  failure responce
 ********************************************************************************/
void x2ap_build_endc_config_upd_resp_fail
(
 x2ap_endc_config_update_failure_t  *pendc_config_upd_resp_fail,
    x2ap_gb_context_t         *p_x2ap_gl_ctx,
     x2ap_bool_et timeTowait_present
)
{
    pendc_config_upd_resp_fail->cause.cause_type = T_x2ap_Cause_radioNetwork;
    pendc_config_upd_resp_fail->cause.cause_info.radio_nw_cause = x2ap_unspecified_2;

    if (X2AP_TRUE == timeTowait_present)
    {
        pendc_config_upd_resp_fail->bitmask = X2AP_TIME_TO_WAIT_INDEX_PRESENT;
        pendc_config_upd_resp_fail->time_to_wait = (x2ap_time_to_wait_et)p_x2ap_gl_ctx->config_param.timer_conf_info.x2ap_time_to_wait_index; 
    }

    X2AP_MEMSET(&(pendc_config_upd_resp_fail->criticality_diagnostics), 0x00, sizeof(x2ap_crit_diagno_t));
}

/*****************************************************************************
 * Function Name  : x2ap_send_endc_configuration_update_ack 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNb context
 *                  p_endc_config_upd_resp - pointer to x2ap_endc_config_update_ack_t
 *                  p_asn_msg_buff - pointre to ASN encoded buffer
 *                  p_asn_msg_len - pointer to  ASN encoded buffer sizw
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends eNB configuration update ack
 ********************************************************************************/
void x2ap_send_endc_configuration_update_ack(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t* p_enb_cntx,
		x2ap_endc_config_update_ack_t* p_endc_config_upd_resp,
		UInt8* p_asn_msg_buff,
		UInt16* p_asn_msg_len)
{
	x2ap_return_et retVal = X2AP_FAILURE;
	x2ap_peer_enb_msg_t  peer_enb_msg;

	X2AP_MEMSET(&peer_enb_msg, X2AP_NULL, sizeof(x2ap_peer_enb_msg_t));

	retVal = x2ap_encode_ENDC_Configuration_Update_ack(p_endc_config_upd_resp, p_asn_msg_buff, p_asn_msg_len);
	if(X2AP_SUCCESS ==retVal)
	{
		peer_enb_msg.p_msg = p_asn_msg_buff;
		peer_enb_msg.msg_len = *p_asn_msg_len;
		peer_enb_msg.ue_ass_msg=X2AP_FALSE;
	    peer_enb_msg.stream_id = p_enb_cntx->stream_info.
		   stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];


		retVal = x2ap_sctp_sm_entry (X2AP_SCTP_SEND_DATA_EV, (UInt8 *)(&peer_enb_msg), p_x2ap_gb_ctx, p_enb_cntx);
		if (X2AP_SUCCESS == retVal)
		{
			X2AP_TRACE(X2AP_INFO,"ENDC CONFIGURATION UPDATE ACKNOWLEDGE sent to peer gNB.");
		}
		else
		{
			X2AP_TRACE(X2AP_ERROR,"ENDC CONFIGURATION UPDATE ACKNOWLEDGE sending failed to peer gNB.");
		}
	}
	else
	{
		X2AP_TRACE(X2AP_ERROR,"ENDC CONFIGURATION UPDATE ACKNOWLEDGE encoding failed.");
	}
}


/*****************************************************************************
 * Function Name  : x2ap_build_and_send_endc_configuration_update_failure 
 * Inputs         : p_x2ap_gb_ctx - pointer tp X2AP global contxet
 *                  p_enb_cntx - peer eNb context
 *                  p_eNB_config_upd_fail - pointer to p_eNB_config_upd_fail,
 *                  p_asn_msg_buff - pointer to ASN encoded buffer
 *                  p_asn_msg_len -  pointer to ASN encoded buffer length 
 *                  timeTowait_present - time to wait is there or not
 * Outputs        : None
 * Returns        : None
 * Description    : this function builds and sends eNB config update failure
 ********************************************************************************/
void x2ap_build_and_send_endc_configuration_update_failure(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t* p_enb_cntx,
		x2ap_endc_config_update_failure_t* p_endc_config_upd_fail,
		UInt8* p_asn_msg_buff,
		UInt16* p_asn_msg_len,
		x2ap_bool_et timeToWait_present
		)
{
	x2ap_return_et retVal = X2AP_FAILURE;
   
        OSCTXT                ctxt;
	x2ap_build_endc_config_upd_resp_fail(p_endc_config_upd_fail, p_x2ap_gb_ctx,timeToWait_present);

	retVal = x2ap_encode_endc_config_update_failure(p_endc_config_upd_fail, p_asn_msg_buff, p_asn_msg_len);

	if (X2AP_SUCCESS == retVal)
	{
		x2ap_peer_enb_msg_t  peer_enb_msg;
		X2AP_MEMSET(&peer_enb_msg, X2AP_NULL, sizeof(x2ap_peer_enb_msg_t));

		peer_enb_msg.p_msg = p_asn_msg_buff;
		peer_enb_msg.msg_len = *p_asn_msg_len;
		peer_enb_msg.stream_id = p_enb_cntx->stream_info.
			stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];

		retVal = x2ap_sctp_sm_entry (X2AP_SCTP_SEND_DATA_EV, ((UInt8 *)&peer_enb_msg), p_x2ap_gb_ctx, p_enb_cntx);
		if (X2AP_SUCCESS == retVal)
		{
			X2AP_TRACE(X2AP_INFO,"ENDC CONFIGURATION UPDATE FAILURE sent to peer gNB.");
		}
		else
		{
			X2AP_TRACE(X2AP_INFO,"ENDC CONFIGURATION UPDATE FAILURE sending failed to peer gNB.");
		}
	}
	else
	{
		X2AP_TRACE(X2AP_ERROR,"ENDC CONFIGURATION UPDATE FAILURE encoding failed.");
	}
}

/*****************************************************************************
 * Function Name  : x2ap_check_if_peer_enb_srv_cell_available
 * Inputs         : pENB_config_upd_req - pointer to x2ap_enb_config_update_ind_t
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function checks if the given cell is available
 *                  in the serving cell list of the peer eNB
 ********************************************************************************/
static x2ap_return_et x2ap_check_if_peer_enb_srv_cell_available
(
 x2ap_gb_context_t   *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx,
 x2ap_nr_ecgi_t         *p_x2ap_ecgi
)
{
    UInt8 cell_ct = 0;
    x2ap_return_et retval = X2AP_FAILURE;
    x2ap_nr_ecgi_t * p_temp_nr_ecgi = (x2ap_nr_ecgi_t *)p_x2ap_gb_ctx->x2ap_link_info[p_enb_cntx->peer_enodeb_id].p_nr_srvd_cell_info;

    for (cell_ct = 0; 
         (cell_ct < p_x2ap_gb_ctx->x2ap_link_info[p_enb_cntx->peer_enodeb_id].num_served_cells) 
          && (p_temp_nr_ecgi) ; cell_ct++)
    {
         if(!X2AP_MEMCMP(&p_temp_nr_ecgi[cell_ct],p_x2ap_ecgi,sizeof(x2ap_nr_ecgi_t)))
         {
            X2AP_TRACE(X2AP_DETAILED,"Cell Match Found");
            retval = X2AP_SUCCESS;
            break;
         }
    }
    return retval;
}


/*****************************************************************************
 * Function Name  : x2ap_validate_endc_peer_modified_served_cells
 * Inputs         : pENB_config_upd_req - pointer to x2ap_enb_config_update_ind_t
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function processes the modified served cells of peer eNB
 *        If serving cell is present in the neighbour list of a peer-cell,
 *        then peer-cell shall also be present in the NRT of the serving cell,
 *        In case peer-cell is not present,then failure shall be returned.
 ********************************************************************************/
static 
x2ap_return_et x2ap_validate_endc_peer_modified_served_cells
(
	x2ap_endc_config_update_en_gnb_req_t* pENB_config_upd_req,
	x2ap_gb_context_t* p_x2ap_gb_ctx,
	x2ap_peer_enb_context_t* p_enb_cntx
)
{
   UInt8 cell_count =0;
   for(cell_count=0; cell_count<pENB_config_upd_req->en_gNB.eutra_nr_cell_modify.num_nr_srvd_cell_modify; cell_count++)
   {
       /*If Cell with Old NRCGI is not present then return Failure*/
       if(X2AP_FAILURE == x2ap_check_if_peer_enb_srv_cell_available(
                   p_x2ap_gb_ctx,p_enb_cntx,
                   &pENB_config_upd_req->en_gNB.eutra_nr_cell_modify.nr_cell_infos[cell_count].old_nr_cgi))
       {
           X2AP_TRACE(X2AP_ERROR, "Neighbour with OLD NRCGI is not available in the NRT.");
           return X2AP_FAILURE;
       }/* If new NRCGI is already present then return Failure */
       else if(X2AP_SUCCESS == x2ap_check_if_peer_enb_srv_cell_available(
                   p_x2ap_gb_ctx,p_enb_cntx,
                   &pENB_config_upd_req->en_gNB.eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.nr_cell_id))
       {
           X2AP_TRACE(X2AP_ERROR, "new NRCGI is already present.");
           return X2AP_FAILURE;
       }
   }
   return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_validate_endc_config_update_req
 * Inputs         : pENB_config_upd_req - pointer to x2ap_enb_config_update_ind_t
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function validates the endc configuration udpate request API.
 ********************************************************************************/
x2ap_return_et x2ap_validate_endc_config_update_req
(
	x2ap_endc_config_update_en_gnb_req_t* pENB_config_upd_req,
	x2ap_gb_context_t* p_x2ap_gb_ctx,
	x2ap_peer_enb_context_t* p_enb_cntx
)
{
    x2ap_return_et retval = X2AP_SUCCESS;
    if (pENB_config_upd_req->en_gNB.bitmask &X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MODIFYLIST_PRESENT)
    {
        retval = x2ap_validate_endc_peer_modified_served_cells(pENB_config_upd_req,p_x2ap_gb_ctx,p_enb_cntx);
    }    
   return retval; 
}			

/*****************************************************************************
 * Function Name  : x2ap_process_endc_peer_added_served_cells 
 * Inputs         : pENB_config_upd_req - pointer to x2ap_enb_config_update_ind_t
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function processes peer added served cells  
 ********************************************************************************/
x2ap_return_et
x2ap_process_endc_peer_added_served_cells
(
 x2_config_update_endc_en_gnb_t       *pENB_config_upd_req,
 x2ap_gb_context_t         *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx)
{
    U8 cell_count =0;

    X2AP_TRACE(X2AP_INFO, "%u peer serving cell to add present\n", pENB_config_upd_req->eutra_nr_cell_management.num_served_nr_cell);

    /* Search for what serving cell neighbour cell info has been added */
    for(cell_count=0; cell_count<pENB_config_upd_req->eutra_nr_cell_management.num_served_nr_cell; cell_count++)
    {
        x2ap_nr_neigh_info_t nbour_info;

        X2AP_MEMSET(&nbour_info, 0x00, sizeof(x2ap_nr_neigh_info_t));
        
        nbour_info.nr_pci = pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].nr_pci;
        
        X2AP_MEMCPY(&nbour_info.nr_cell_id, 
            &pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].nr_cell_id, 
            sizeof(x2ap_nr_ecgi_t));

        if (X2AP_NR_MODE_FDD_PRESENT & pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].choice_nr_mode.bitmask)
        {
            nbour_info.nr_freq_info.nr_earfcn = pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].
                choice_nr_mode.nr_fdd_info.nr_dl_freq_info.nr_earfcn;

            nbour_info.nr_neigh_Mode_Info.bitmask |= X2AP_NR_NEIGHBOUR_FDD_PRESENT;
            
            X2AP_MEMCPY(&nbour_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info, 
	        &pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].choice_nr_mode.nr_fdd_info.nr_dl_freq_info, 
	        sizeof(x2ap_nr_freq_info_t));
	    X2AP_MEMCPY(&nbour_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info, 
	        &pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].choice_nr_mode.nr_fdd_info.nr_ul_freq_info, 
	        sizeof(x2ap_nr_freq_info_t));
        } 
        else if (X2AP_NR_MODE_TDD_PRESENT & pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].choice_nr_mode.bitmask)
        {
            nbour_info.nr_freq_info.nr_earfcn = pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].
                choice_nr_mode.nr_tdd_info.nr_freq_info.nr_earfcn;

            nbour_info.nr_neigh_Mode_Info.bitmask |= X2AP_NR_NEIGHBOUR_TDD_PRESENT;
            
            X2AP_MEMCPY(&nbour_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info, 
	        &pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].choice_nr_mode.nr_tdd_info.nr_freq_info, 
	        sizeof(x2ap_nr_freq_info_t));    
        }  

        if(X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT & 
            pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].bitmask)
	 {
	    nbour_info.bitmask |= X2AP_FIVE_GS_TAC_PRESENT;
            
            X2AP_MEMCPY(&nbour_info.fivegs_tac, 
	        &pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].fiveGS_track_area_code, 
	        sizeof(x2ap_five_gs_tac_t));
	 }

	 if(X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT & 
	    pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].bitmask)
	 {
	    nbour_info.bitmask |= X2AP_CONFIGURED_TAC_PRESENT;
            
            X2AP_MEMCPY(&nbour_info.configured_tac, 
	        &pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].configured_tac, 
	        sizeof(x2ap_tac_t));
	 }

#ifndef LINUX_PC_TEST
	 x2ap_build_x2ap_meas_timing_config(
	    &pENB_config_upd_req->eutra_nr_cell_management.served_nr_cell_arr[cell_count].meas_timing_config,
	    &nbour_info.x2ap_meas_timing_config[0], &nbour_info.meas_timing_config_count);
#endif

        /* update peer enb cell info */
        x2ap_populate_nbour_nr_cell_info(p_x2ap_gb_ctx, &nbour_info, 0, &p_enb_cntx->gb_gnb_id);

        /* update linkup info */
        x2ap_populate_served_nr_cell_info(p_x2ap_gb_ctx, &nbour_info,
            &nbour_info.nr_cell_id, p_enb_cntx->peer_enodeb_id);

        x2ap_build_and_send_nr_update_info(&nbour_info);
    }
    
    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_process_endc_peer_deleted_served_cells 
 * Inputs         : pENB_config_upd_req - pointer to x2ap_enb_config_update_ind_t
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function proeceses the deleted served cells of peer eNB
 ********************************************************************************/
x2ap_return_et
x2ap_process_endc_peer_deleted_served_cells
(
 x2_config_update_endc_en_gnb_t       *pENB_config_upd_req,
 x2ap_gb_context_t         *p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx)
{
    U8 cell_count =0;
    x2ap_nbr_nr_cgi_search_node_t* p_node = X2AP_P_NULL;

    X2AP_TRACE(X2AP_INFO, "%u peer serving cell to delete present\n", pENB_config_upd_req->eutra_nr_cell_delete.num_delete_nrcells_endc);

    for(cell_count=0; cell_count<pENB_config_upd_req->eutra_nr_cell_delete.num_delete_nrcells_endc; cell_count++)
    {
        p_node = (x2ap_nbr_nr_cgi_search_node_t*)x2ap_st_find_tree_node((x2ap_search_tree_t *)&(p_x2ap_gb_ctx->nbr_nr_cgi_search_tree),
                        (void*)(&pENB_config_upd_req->eutra_nr_cell_delete.eutra_nr_cell_delete[cell_count]) );

        if(p_node== X2AP_P_NULL)
        {
            X2AP_TRACE(X2AP_WARNING, "Neighbour CGI not found in tree.\n");
        }
        else
        {
            x2ap_st_delete_node(&p_x2ap_gb_ctx->nbr_nr_cgi_search_tree, &(p_node->nodeAnchor));
            x2ap_mem_free(&p_node->nodeAnchor);
            X2AP_MEMSET(&p_x2ap_gb_ctx->peer_enb_cell_info[p_node->logical_id], 0x00, sizeof(x2ap_peer_enb_cell_info_t));
            p_x2ap_gb_ctx->num_peer_enb_cells--;
        }
    }
    
    return X2AP_SUCCESS;
} 

/*****************************************************************************
 * Function Name  : x2ap_process_endc_peer_modifyed_served_cells 
 * Inputs         : pENB_config_upd_req - pointer to x2ap_endc_config_update_en_gnb_req_t
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function processes the modified served cells of peer GNB
 ********************************************************************************/
x2ap_return_et
x2ap_process_endc_peer_modifyed_served_cells
(
 x2_config_update_endc_en_gnb_t* pENB_config_upd_req,
 x2ap_gb_context_t* p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx)
{
    U8 cell_count =0;

    X2AP_TRACE(X2AP_INFO, "%u peer serving cell to modify present\n", pENB_config_upd_req->eutra_nr_cell_modify.num_nr_srvd_cell_modify);

    /* Search for what serving cell neighbour cell info has been modified */
    for(cell_count=0; cell_count<pENB_config_upd_req->eutra_nr_cell_modify.num_nr_srvd_cell_modify; cell_count++)
    {
        x2ap_nr_neigh_info_t nbour_info;
        x2ap_nbr_nr_cgi_search_node_t* p_node = X2AP_P_NULL;
        x2ap_nbr_nr_cgi_search_node_t* p_next = X2AP_P_NULL;
				
        /* Add cell_count neighbour for cell_counter served_cell*/
	 /*Delete if old neighbour present with old_cgi*/
        p_node = (x2ap_nbr_nr_cgi_search_node_t*)x2ap_st_get_first_node(&(p_x2ap_gb_ctx->nbr_nr_cgi_search_tree));
        p_next = p_node;
        
        while(X2AP_P_NULL != p_node)
        {
            p_next = (x2ap_nbr_nr_cgi_search_node_t*)x2ap_st_get_next_node((x2ap_search_tree_node_t*)p_node);

            if (X2AP_MEMCMP(&pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].old_nr_cgi, 
                    &p_node->nr_cgi, sizeof(x2ap_nr_ecgi_t)))
            {
                X2AP_TRACE(X2AP_WARNING, "Neighbour CGI not found in tree.\n");
            }
            else
	    {
                U8 cell_id = p_node->serving_cell_index;
		 x2ap_st_delete_node(&p_x2ap_gb_ctx->nbr_nr_cgi_search_tree, &(p_node->nodeAnchor));
		 x2ap_mem_free(&p_node->nodeAnchor);
		 X2AP_MEMSET(&p_x2ap_gb_ctx->peer_enb_cell_info[p_node->logical_id], 0x00, sizeof(x2ap_peer_enb_cell_info_t));
		 p_x2ap_gb_ctx->num_peer_enb_cells--;
            }
            
            p_node = p_next;
        }  

        X2AP_MEMSET(&nbour_info, 0x00, sizeof(x2ap_nr_neigh_info_t));
		 
        nbour_info.nr_pci = pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.nr_pci;

	 X2AP_MEMCPY(&nbour_info.nr_cell_id, 
	    &pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.nr_cell_id, 
	    sizeof(x2ap_nr_ecgi_t));

	 if(X2AP_NR_MODE_FDD_PRESENT & pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.choice_nr_mode.bitmask)
	 {
            nbour_info.nr_freq_info.nr_earfcn = 
                pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.choice_nr_mode.nr_fdd_info.nr_dl_freq_info.nr_earfcn;

            nbour_info.nr_neigh_Mode_Info.bitmask |= X2AP_NR_NEIGHBOUR_FDD_PRESENT;
            
            X2AP_MEMCPY(&nbour_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info, 
	        &pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.choice_nr_mode.nr_fdd_info.nr_dl_freq_info, 
	        sizeof(x2ap_nr_freq_info_t));
	    X2AP_MEMCPY(&nbour_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info, 
	        &pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.choice_nr_mode.nr_fdd_info.nr_ul_freq_info, 
	        sizeof(x2ap_nr_freq_info_t));
        }
        else if(X2AP_NR_MODE_TDD_PRESENT & pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.choice_nr_mode.bitmask)
        {
            nbour_info.nr_freq_info.nr_earfcn = 
                pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.choice_nr_mode.nr_tdd_info.nr_freq_info.nr_earfcn;

            nbour_info.nr_neigh_Mode_Info.bitmask |= X2AP_NR_NEIGHBOUR_TDD_PRESENT;
            
            X2AP_MEMCPY(&nbour_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info, 
	        &pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.choice_nr_mode.nr_tdd_info.nr_freq_info, 
	        sizeof(x2ap_nr_freq_info_t));
        }

        if(X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT & pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.bitmask)
	 {
	    nbour_info.bitmask |= X2AP_FIVE_GS_TAC_PRESENT;
            
            X2AP_MEMCPY(&nbour_info.fivegs_tac, 
	        &pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.fiveGS_track_area_code, 
	        sizeof(x2ap_five_gs_tac_t));
	 }

	 if(X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT & pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.bitmask)
	 {
	    nbour_info.bitmask |= X2AP_CONFIGURED_TAC_PRESENT;
            
            X2AP_MEMCPY(&nbour_info.configured_tac, 
	        &pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.configured_tac, 
	        sizeof(x2ap_tac_t));
	 }

#ifndef LINUX_PC_TEST
	 x2ap_build_x2ap_meas_timing_config(
	    &pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].srvd_nr_cell_info.meas_timing_config,
	    &nbour_info.x2ap_meas_timing_config[0], &nbour_info.meas_timing_config_count);
#endif

        /* Add cell_count neighbour for cell_counter served_cell*/
        x2ap_populate_nbour_nr_cell_info(p_x2ap_gb_ctx, &nbour_info, 0, &p_enb_cntx->gb_gnb_id);

        /* update linkup info */
        x2ap_populate_served_nr_cell_info(p_x2ap_gb_ctx, &nbour_info,
            &pENB_config_upd_req->eutra_nr_cell_modify.nr_cell_infos[cell_count].old_nr_cgi, p_enb_cntx->peer_enodeb_id);

        x2ap_build_and_send_nr_update_info(&nbour_info);
    }
    
    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_process_endc_config_update_en_gnb_req 
 * Inputs         : p_msg_ptr - pointer to msg
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer en-gNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function the endc configuration update en-gNB
 ********************************************************************************/
x2ap_return_et
x2ap_process_endc_config_update_en_gnb_req
(
 U8* p_msg_ptr,
 x2ap_gb_context_t* p_x2ap_gb_ctx,
 x2ap_peer_enb_context_t* p_enb_cntx
 )
{
    x2ap_return_et            retVal = X2AP_SUCCESS;
    x2ap_endc_config_update_en_gnb_req_t      *p_endc_config_upd_req = X2AP_P_NULL;
    x2ap_endc_config_update_ack_t         endc_config_upd_resp;
    x2ap_endc_config_update_failure_t     endc_config_upd_resp_fail;
    x2ap_endc_config_update_en_gnb_ind_t *p_endc_config_update_ind = X2AP_P_NULL;

    UInt8 *p_asn_msg_buff = X2AP_P_NULL;
    UInt16 asn_msg_len = 0;
    x2ap_peer_enb_msg_t* p_peer_enb_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr ;

    rrc_x2ap_error_indication_t  error_indication;
    x2ap_error_ind_bool_t        send_err_ind;
    OSCTXT                       asn1_ctx;
    UInt16                           cell_count = X2AP_NULL;
    x2ap_bool_et timeTowait_present = X2AP_FALSE;

    X2AP_UT_TRACE_ENTER();

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_cntx != X2AP_P_NULL);

    /*Initialize ASN Context */
    if (0 != rtInitContext(&asn1_ctx))
    {
        X2AP_TRACE(X2AP_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }

    p_endc_config_upd_req = (x2ap_endc_config_update_en_gnb_req_t*) 
    x2ap_mem_get(sizeof(x2ap_endc_config_update_en_gnb_req_t));
    X2AP_TRACE(X2AP_INFO, "x2ap endc config update request Info funciton");

    if( X2AP_P_NULL == p_endc_config_upd_req)
    {
        X2AP_TRACE(X2AP_WARNING, "x2ap endc config update request"
                " Memory allocation failed");
        return X2AP_FAILURE;
    }

    p_asn_msg_buff = x2ap_mem_get(X2AP_MAX_ASN1_BUF_LEN);
    if(X2AP_P_NULL == p_asn_msg_buff)
    {
        if(X2AP_P_NULL != p_endc_config_upd_req)
        {
            x2ap_mem_free(p_endc_config_upd_req);
        }  
        X2AP_TRACE(X2AP_WARNING, "x2ap endc config update request"
                " Memory allocation faied for p_asn_msg_buff");
        return X2AP_FAILURE;
    }

    p_endc_config_update_ind = (x2ap_endc_config_update_en_gnb_ind_t*) 
        x2ap_mem_get(sizeof(x2ap_endc_config_update_en_gnb_ind_t));
    X2AP_TRACE(X2AP_INFO, "x2ap endc config update indication Info funciton");

    if( X2AP_P_NULL == p_endc_config_update_ind)
    {
        X2AP_TRACE(X2AP_WARNING, "x2ap endc config update indication"
                " Memory allocation failed");
        return X2AP_FAILURE;
    }
    
    /* Init the messages to be send */
    X2AP_MEMSET(&endc_config_upd_resp,0,sizeof(x2ap_endc_config_update_ack_t));
    X2AP_MEMSET(&endc_config_upd_resp_fail,0,sizeof(x2ap_endc_config_update_failure_t));
    X2AP_MEMSET(p_endc_config_upd_req,0,sizeof(x2ap_endc_config_update_en_gnb_req_t));
    X2AP_MEMSET(p_asn_msg_buff,X2AP_NULL,X2AP_MAX_ASN1_BUF_LEN);
    X2AP_MEMSET(p_endc_config_update_ind,0,sizeof(x2ap_endc_config_update_en_gnb_ind_t));
    
    switch(p_enb_cntx->endc_config_sm_ctx.state )
    {
        case X2AP_ENB_CONFIG_IDLE_ST:
            {
                X2AP_TRACE(X2AP_ERROR,"X2 interface not operational. ");
                timeTowait_present = X2AP_TRUE;
                retVal = X2AP_FAILURE;
            }
            break;
        case X2AP_ENB_CONFIG_W_CONF_RESP_ST:
            break;
        case X2AP_ENB_CONFIG_ACTIVE_ST:
            break;
        default:
            X2AP_TRACE(X2AP_ERROR,"Invalid state.");
            retVal = X2AP_FAILURE;
            break;
    }

    if (X2AP_SUCCESS == retVal)
    {
        retVal = x2ap_decode_endc_config_update_en_gnb_req(p_x2ap_gb_ctx,
                p_enb_cntx,
                p_endc_config_upd_req,
                (p_peer_enb_msg)->p_msg,
                &(p_peer_enb_msg->msg_len),
                &error_indication,
                &send_err_ind,
                &asn1_ctx);

        if(p_endc_config_upd_req->bitmask ==0)
        {
            /*Only message type present in the message, send eNB CONFIGURATION ACKNOWLEDGE*/
            x2ap_send_endc_configuration_update_ack(p_x2ap_gb_ctx, p_enb_cntx, &endc_config_upd_resp, p_asn_msg_buff, &asn_msg_len);
        }
        else
        {
            if( p_endc_config_upd_req->bitmask & X2AP_ENDC_X2_CONFIG_UPDATE_INIT_EN_GNB_PRESENT )
            {
                if(p_endc_config_upd_req->en_gNB.bitmask == 0)
                {
                    /*Only message type present in the message, send eNB CONFIGURATION ACKNOWLEDGE*/
                    x2ap_send_endc_configuration_update_ack(p_x2ap_gb_ctx, p_enb_cntx, &endc_config_upd_resp, p_asn_msg_buff, &asn_msg_len);
                }
                else
                {
                    retVal = x2ap_validate_endc_config_update_req(p_endc_config_upd_req,p_x2ap_gb_ctx, p_enb_cntx); 

                    /* if added cells present in req */
                    if(p_endc_config_upd_req->en_gNB.bitmask &X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MANAGEMENTLIST_PRESENT)
                    {
                        if ((X2AP_SUCCESS == retVal) && (X2AP_NULL != p_endc_config_upd_req->en_gNB.eutra_nr_cell_management.num_served_nr_cell))
                        {
                            retVal = x2ap_process_endc_peer_added_served_cells(&p_endc_config_upd_req->en_gNB, p_x2ap_gb_ctx,p_enb_cntx);
                            X2AP_TRACE(X2AP_INFO,"Peer added cells processed ");
                        }
                    }

                    /* if deleted cells present in req */
                    if (p_endc_config_upd_req->en_gNB.bitmask & X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_DELETELIST_PRESENT)
                    {
                        if ((X2AP_SUCCESS == retVal) && (X2AP_NULL != p_endc_config_upd_req->en_gNB.eutra_nr_cell_delete.num_delete_nrcells_endc))
                        {
                            retVal = x2ap_process_endc_peer_deleted_served_cells(&p_endc_config_upd_req->en_gNB,p_x2ap_gb_ctx, p_enb_cntx);
                            X2AP_TRACE(X2AP_INFO,"Peer deleted cells processed ");
                        }
                    }

                    /* if modified cells present in req */
                    if(p_endc_config_upd_req->en_gNB.bitmask & X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MODIFYLIST_PRESENT)
                    {
                        if ((X2AP_SUCCESS == retVal) && (X2AP_NULL != p_endc_config_upd_req->en_gNB.eutra_nr_cell_modify.num_nr_srvd_cell_modify))
                        {
                            retVal = x2ap_process_endc_peer_modifyed_served_cells(&p_endc_config_upd_req->en_gNB,p_x2ap_gb_ctx, p_enb_cntx);
                            X2AP_TRACE(X2AP_INFO,"Peer modified cells processed ");
                        }
                    }

                    if (X2AP_SUCCESS == retVal)
                    {
                        U16 dst_module_id = RRC_RRM_MODULE_ID;
                        
                        /* Send endc Configuration Update Indication to SON/RRM*/
                        /*if(p_x2ap_gb_ctx->is_son_present)
                        {
                            dst_module_id = RRC_SON_MODULE_ID;
                        }*/

                        /* build and send to rrm/son */
                        x2ap_build_endc_config_update_ind(p_x2ap_gb_ctx, p_enb_cntx, 
                            &p_endc_config_upd_req->en_gNB, p_endc_config_update_ind);

                        retVal = rrc_x2apRrm_il_send_x2ap_endc_config_update_en_gnb_ind(p_endc_config_update_ind,
                            RRC_X2AP_MODULE_ID,dst_module_id, RRC_NULL, RRC_NULL);    

                        if(X2AP_SUCCESS == retVal)
                        {
                            /* Send endc CONFIGURATION UPDATE ACKNOWLEDGE */
                            x2ap_send_endc_configuration_update_ack(p_x2ap_gb_ctx, p_enb_cntx, &endc_config_upd_resp, p_asn_msg_buff, &asn_msg_len);
                        }
                    }

                    if (X2AP_FAILURE == retVal)
                    {
                        /* Build and Send endc CONFIGURATION UPDATE FAILURE */
                        x2ap_build_and_send_endc_configuration_update_failure(p_x2ap_gb_ctx, p_enb_cntx, &endc_config_upd_resp_fail, 
                                    p_asn_msg_buff, &asn_msg_len, timeTowait_present);
                    }
                }
            }
        }

        rtFreeContext(&asn1_ctx);
    }

    if(X2AP_P_NULL != p_asn_msg_buff)
    {
        x2ap_mem_free(p_asn_msg_buff);
        p_asn_msg_buff = X2AP_P_NULL;
    }

    if(X2AP_P_NULL != p_endc_config_upd_req)
    {
        x2ap_mem_free(p_endc_config_upd_req);
        p_endc_config_upd_req = X2AP_P_NULL;
    }  

    if(X2AP_P_NULL != p_endc_config_update_ind)
    {
        x2ap_mem_free(p_endc_config_update_ind);
        p_endc_config_update_ind = X2AP_P_NULL;
    }  

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_send_endc_config_update_req_peer 
 * Inputs         : p_x2ap_gb_ctx - pointyer to x2AP global context
 *                  p_peer_enb_ct-  pointer to pext
 *                  p_asn_msg - pointer to asn encoded msg 
 *                  asn_msg_len - pointer to asn encoded msg length 
 *                  timer_type - time id
 * Outputs        : p_asn_msg , asn_msg_len 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function sends the eNB configuration request to the peer
 ********************************************************************************/
x2ap_return_et x2ap_send_endc_config_update_req_peer
(
    x2ap_gb_context_t* p_x2ap_gb_ctx, 
    x2ap_peer_enb_context_t* p_peer_enb_ctx, 
    U8* p_asn_msg, 
    U16 asn_msg_len, 
    x2ap_timer_id_en timer_type
)
{
    x2ap_timer_t timer_id;
    x2ap_timer_buffer_t timer_buf;
    x2ap_peer_enb_msg_t  peer_enb_msg;
    U32 timer_value = 0;

    X2AP_MEMSET(&timer_id,X2AP_NULL,sizeof(x2ap_timer_t));
    X2AP_MEMSET(&timer_buf,X2AP_NULL,sizeof(x2ap_timer_buffer_t));
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

    peer_enb_msg.p_msg = p_asn_msg;
    peer_enb_msg.msg_len = asn_msg_len;
    peer_enb_msg.stream_id = p_peer_enb_ctx->stream_info.
	stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];

    if (INVALID_SCTP_SOCKET != p_peer_enb_ctx->sctp_sd)
    {
        if(!(x2ap_sctp_sm_entry(X2AP_SCTP_SEND_DATA_EV, (U8*)&peer_enb_msg, p_x2ap_gb_ctx, p_peer_enb_ctx)))
	{
	    X2AP_TRACE(X2AP_INFO,"Sending X2AP_SCTP_SEND_DATA_EV failed\n");
            return X2AP_FAILURE;
	}
    }
    else
    {
	X2AP_TRACE(X2AP_INFO,"Invalid peer gNB SCTP socket descriptor\n");
	return X2AP_FAILURE;
    }

    timer_buf.auto_delete = X2AP_FALSE;
    timer_buf.sd = p_peer_enb_ctx->sctp_sd;
    timer_buf.p_timer_specific_data = (void*)p_peer_enb_ctx;
    timer_buf.timer_type = timer_type;

    if(timer_type == X2AP_ENB_CONFIG_UPDATE_RSP_TIMER)
    {
	timer_value = p_x2ap_gb_ctx->config_param.timer_conf_info.eNB_Config_Update_retry_timer_val;
    }
    else if(timer_type == X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER)
    {
	x2ap_convert_time_to_wait_index_to_duration(p_x2ap_gb_ctx->config_param.timer_conf_info.x2ap_time_to_wait_index,&timer_value);
	timer_value = timer_value + p_x2ap_gb_ctx->config_param.timer_conf_info.eNB_Config_Update_time_to_wait_offset;
    }
    
    timer_id = x2ap_start_timer(timer_value, &timer_buf, sizeof(timer_buf), X2AP_FALSE);

    if(timer_type == X2AP_ENB_CONFIG_UPDATE_RSP_TIMER)
    {
	p_peer_enb_ctx->endc_config_sm_ctx.state = X2AP_ENB_CONFIG_W_CONF_RESP_ST ;
	p_peer_enb_ctx->endc_config_sm_ctx.req_timer_id = timer_id;
    	X2AP_TRACE(X2AP_INFO,"Started X2AP_ENB_CONFIG_UPDATE_RSP_TIMER with timer_id:%p and duration %u\n", timer_id, timer_value);
	X2AP_TRACE(X2AP_INFO,"endc_config_sm: State changed to X2AP_ENB_CONFIG_W_CONF_RESP_ST\n");
    }
    else if(timer_type == X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER)
    {
	p_peer_enb_ctx->endc_config_sm_ctx.state = X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST ;
	p_peer_enb_ctx->endc_config_sm_ctx.time_to_wait_timer_id = timer_id;
    	X2AP_TRACE(X2AP_INFO,"Started X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER with timer_id:%p and duration %u\n", timer_id, timer_value);
	X2AP_TRACE(X2AP_INFO,"endc_config_sm: State changed to X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST\n");
    }
    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_endc_config_update_response_timer_expiry 
 * Inputs         : p_timer_buf - pointer to timer buffer
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes ths ENDC configuaration update timer 
 *                  expiry responce
 ********************************************************************************/
void x2ap_endc_config_update_response_timer_expiry
(
    x2ap_timer_buffer_t *p_timer_buf, 
    x2ap_gb_context_t *p_x2ap_gb_ctx
)
{
    x2ap_peer_enb_context_t* p_enb_cntx =X2AP_P_NULL;
    p_enb_cntx = (x2ap_peer_enb_context_t*)p_timer_buf->p_timer_specific_data;

    if(X2AP_ENB_CONFIG_W_CONF_RESP_ST == p_enb_cntx->endc_config_sm_ctx.state)
    {
        /* Stop X2AP_ENB_CONFIG_UPDATE_RSP_TIMER*/
        /*x2ap_stop_timer(p_enb_cntx->endc_config_sm_ctx.req_timer_id); */
        p_enb_cntx->endc_config_sm_ctx.req_timer_id = X2AP_P_NULL;

        if(0 < p_enb_cntx->endc_config_sm_ctx.req_retry_count)
        {
	    U8 asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
	    U16 asn_msg_len = X2AP_NULL;

	    X2AP_TRACE(X2AP_INFO,"ENDC Configuration Update retry count:%u", p_enb_cntx->endc_config_sm_ctx.req_retry_count);
	    p_enb_cntx->endc_config_sm_ctx.req_retry_count--;

	    X2AP_TRACE(X2AP_INFO,"ENDC Configuration Update retry left:%u", p_enb_cntx->endc_config_sm_ctx.req_retry_count);

	    if(!(x2ap_encode_ENDC_Configuration_Update_req(p_x2ap_gb_ctx,&p_enb_cntx->endc_config_sm_ctx.endc_config_req, asn_msg_buff, &asn_msg_len)))
	    {
	        X2AP_TRACE(X2AP_ERROR,"Encoding ENDC Configuration Update failed");;
	        return;
	    }

	    X2AP_TRACE(X2AP_INFO,"ENDC Configuration Update sent to Peer gNB again.\n");

	    /* Send ENDC CONFIGURATION UPDATE to peer gNB */
            if(OPERATIONAL == p_enb_cntx->op_status)
            {
                if(!(x2ap_send_endc_config_update_req_peer(p_x2ap_gb_ctx, p_enb_cntx, asn_msg_buff, asn_msg_len, X2AP_ENB_CONFIG_UPDATE_RSP_TIMER)))
                {
                    X2AP_TRACE(X2AP_ERROR,"Sending ENDC Configuration Update failed");
		     return;
                }
            }
        }
        else
        {
            x2ap_endc_config_update_resp_t endc_cfg_resp;

            X2AP_TRACE(X2AP_INFO, "ENDC Configuration Update retry count exhausted\n");
            X2AP_MEMSET(&endc_cfg_resp, 0x00, sizeof(x2ap_endc_config_update_resp_t));

            X2AP_MEMCPY(&endc_cfg_resp.gnb_gb_id, &p_enb_cntx->gb_gnb_id, sizeof(x2_gb_gnb_id_t));

            endc_cfg_resp.cause.type = X2AP_CAUSE_INTERNAL;
            endc_cfg_resp.cause.value = ENB_CONFIG_UPDATE_TIMER_EXPIRY;

            /* Send ENDC Configuration Update response */
            x2ap_send_endc_config_update_response(p_x2ap_gb_ctx, &endc_cfg_resp, X2AP_FAILURE, 
                p_enb_cntx->endc_config_sm_ctx.dst_module_id, p_enb_cntx->endc_config_sm_ctx.transaction_id);
                
            X2AP_MEMSET(&p_enb_cntx->endc_config_sm_ctx, 0x00, sizeof(x2ap_endc_config_sm_context_t));
            p_enb_cntx->endc_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
        }
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR, "Invalid state:%u.\n", p_enb_cntx->endc_config_sm_ctx.state);
    }
}

/*****************************************************************************
 * Function Name  : x2ap_endc_config_update_time_to_wait_expiry 
 * Inputs         : p_timer_buf - pointer to timer buffer 
 *                  p_x2ap_gb_ctx - pointer to x2AP global context
 * Outputs        : None
 * Returns        : None 
 * Description    : This function processes the endc config update time to wait  
 ********************************************************************************/
void x2ap_endc_config_update_time_to_wait_expiry
(
    x2ap_timer_buffer_t *p_timer_buf, 
    x2ap_gb_context_t *p_x2ap_gb_ctx
)
{
    x2ap_peer_enb_context_t* p_enb_cntx =X2AP_P_NULL;
    p_enb_cntx = (x2ap_peer_enb_context_t*)p_timer_buf->p_timer_specific_data;

    if(X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST == p_enb_cntx->endc_config_sm_ctx.state)
    {
        /* Stop X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER*/
        /*x2ap_stop_timer(p_enb_cntx->endc_config_sm_ctx.time_to_wait_timer_id);*/ 
        p_enb_cntx->endc_config_sm_ctx.time_to_wait_timer_id = X2AP_P_NULL;

        if(0 < p_enb_cntx->endc_config_sm_ctx.time_to_wait_retry_count)
        {
            U8 asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
            U16 asn_msg_len = X2AP_NULL;

            X2AP_TRACE(X2AP_INFO,"ENDC Configuration Update time to wait retry count:%u", p_enb_cntx->endc_config_sm_ctx.time_to_wait_retry_count);

            p_enb_cntx->endc_config_sm_ctx.time_to_wait_retry_count--;

            X2AP_TRACE(X2AP_INFO,"ENDC Configuration Update time to wait retry left:%u", p_enb_cntx->endc_config_sm_ctx.time_to_wait_retry_count);

            if(!(x2ap_encode_ENDC_Configuration_Update_req(p_x2ap_gb_ctx,&p_enb_cntx->endc_config_sm_ctx.endc_config_req, asn_msg_buff, &asn_msg_len)))
            {
                X2AP_TRACE(X2AP_ERROR,"Encoding ENDC Configuration Update failed");;
                return;
            }

            X2AP_TRACE(X2AP_INFO,"ENDC Configuration Update sent to Peer gNB again.\n");

            /* Send ENDC CONFIGURATION UPDATE to peer gNB */
            if(!(x2ap_send_endc_config_update_req_peer(p_x2ap_gb_ctx, p_enb_cntx, asn_msg_buff, asn_msg_len, X2AP_ENB_CONFIG_UPDATE_RSP_TIMER)))
            {
                X2AP_TRACE(X2AP_ERROR,"Sending ENDC Configuration Update failed");
                return;
            }
        }
        else
        {
            x2ap_endc_config_update_resp_t endc_cfg_resp;

            X2AP_TRACE(X2AP_INFO, "ENDC Configuration Update retry count exhausted\n");
            X2AP_MEMSET(&endc_cfg_resp, 0x00, sizeof(x2ap_endc_config_update_resp_t));

            X2AP_MEMCPY(&endc_cfg_resp.gnb_gb_id, &p_enb_cntx->gb_gnb_id , sizeof(x2_gb_gnb_id_t));

	    endc_cfg_resp.cause.type = T_x2ap_Cause_radioNetwork;
	    endc_cfg_resp.cause.value = x2ap_unspecified_2;

	    /* Send ENDC Configuration Update response */
	    x2ap_send_endc_config_update_response(p_x2ap_gb_ctx, &endc_cfg_resp, X2AP_FAILURE, 
	        p_enb_cntx->endc_config_sm_ctx.dst_module_id, p_enb_cntx->endc_config_sm_ctx.transaction_id);
		    
	    X2AP_MEMSET(&p_enb_cntx->endc_config_sm_ctx, 0x00, sizeof(x2ap_endc_config_sm_context_t));
		p_enb_cntx->endc_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
	}
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR, "Invalid state:%u.\n", p_enb_cntx->endc_config_sm_ctx.state);
    }

}

/*****************************************************************************
 * Function Name  : x2ap_process_endc_config_update_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  	p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *                  dest_module_id - destination id
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the endc config update request from RRM/SON
 ****************************************************************************/
void x2ap_process_endc_config_update_req
(
    void* p_api, 
    x2ap_gb_context_t* p_x2ap_gb_ctx, 
    U16 dest_module_id
)
{
    x2ap_return_et response = X2AP_SUCCESS;
    static x2ap_endc_config_update_req_t endc_cfg_req;
    x2ap_endc_config_update_resp_t endc_cfg_resp;
    U8 asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_msg_len = X2AP_NULL;
    S32 length_read = X2AP_NULL;
    U16 trans_id = X2AP_NULL;
    x2ap_return_et retVal = X2AP_SUCCESS;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    /* Get the transaction id */
    trans_id = x2ap_get_transaction_id(p_api);

    X2AP_MEMSET(&endc_cfg_req, 0, sizeof(x2ap_endc_config_update_req_t));
    X2AP_MEMSET(&endc_cfg_resp, 0, sizeof(x2ap_endc_config_update_resp_t));

    /* Decode the received message from RRM/SON */
    response = rrc_il_parse_x2ap_endc_config_update_req(
			&endc_cfg_req,
			(U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
			x2ap_get_api_buf_size(p_api) -
			RRC_FULL_INTERFACE_HEADERS_SIZE,
			&length_read);

    if (X2AP_SUCCESS == response)
    {
        if(endc_cfg_req.bitmask ==0)
	{
	    X2AP_TRACE(X2AP_WARNING, "endc config update request incorrect bitmask 0 received.\n");
	    retVal = X2AP_FAILURE;
        }
	else
	{
	    U8 gnb_count =0;
	    U8 max_gnb_count =0;
	    U8 gnb_found =X2AP_FAILURE;
	    x2ap_peer_enb_context_t* p_peer_enb_ctx = X2AP_P_NULL;
	    U8 reset_ongoing = X2AP_FALSE;

	    if(0 != endc_cfg_req.num_peer_gnb)
	    {
	        /* Encode ENDC Configuration Update message*/
		if(!(x2ap_encode_ENDC_Configuration_Update_req(p_x2ap_gb_ctx, &endc_cfg_req, asn_msg_buff, &asn_msg_len)))
		{
		    X2AP_TRACE(X2AP_ERROR,"Encoding ENDC Configuration Update failed");
		    retVal = X2AP_FAILURE;
		}
	    }

	    /* Apply ENDC Configuration Update message*/
	    if (X2AP_SUCCESS == retVal)
	    {
    	        if (!(x2ap_apply_endc_config_update_req(p_x2ap_gb_ctx, &endc_cfg_req)))
    	        {
    	            X2AP_TRACE(X2AP_ERROR,"update ENDC Configuration Update req failed");
                    retVal = X2AP_FAILURE;
    	        }
	    }

            if (X2AP_SUCCESS == retVal)
            {
                /* Fetch peer gNB context */
                for(gnb_count =0; gnb_count<endc_cfg_req.num_peer_gnb; gnb_count++)
	        {
		    gnb_found = X2AP_FAILURE;
		    reset_ongoing = X2AP_FALSE;
				
		    for(max_gnb_count=0; max_gnb_count<MAX_PEER_GNB && gnb_found!=X2AP_SUCCESS; max_gnb_count++)
		    {
		        if((p_x2ap_gb_ctx->peer_enb_cntxt_arr[max_gnb_count].op_status == OPERATIONAL) &&
			    (!(X2AP_MEMCMP(&endc_cfg_req.gnb_info[gnb_count], 
			        &p_x2ap_gb_ctx->peer_enb_cntxt_arr[max_gnb_count].gb_gnb_id, sizeof(x2_gb_gnb_id_t)))))
		        {
			    if(p_x2ap_gb_ctx->peer_enb_cntxt_arr[max_gnb_count].x2ap_reset_sm_ctx.state == X2_RESET_W_RESET_RESP_ST)
			    {
			        reset_ongoing = X2AP_TRUE;		
			        break;
			    }       
			    else
			    {
			        gnb_found = X2AP_SUCCESS;
			        p_peer_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[max_gnb_count];
			        break;
			    }
	                }
		    }

		    if(gnb_found == X2AP_FAILURE)
		    {
		        if (reset_ongoing == X2AP_TRUE)
		        {
			    X2AP_TRACE(X2AP_ERROR,"Reset is ongoing with the peer gNB id %u\n", gnb_count+1);
		        }
		        else
		        {
			    X2AP_TRACE(X2AP_ERROR,"Peer gNB %u in the list is not operational\n", gnb_count+1);
		        }

		        endc_cfg_resp.cause.type = T_x2ap_Cause_radioNetwork;
		        endc_cfg_resp.cause.value = x2ap_unspecified_2;

		        X2AP_MEMCPY(&endc_cfg_resp.gnb_gb_id, &endc_cfg_req.gnb_info[gnb_count], sizeof(x2_gb_gnb_id_t));

		        /* Send ENDC Configuration Update response */
		        x2ap_send_endc_config_update_response(p_x2ap_gb_ctx, &endc_cfg_resp, X2AP_FAILURE, dest_module_id, trans_id);
		    }
		    else
		    {
		        switch(p_peer_enb_ctx->endc_config_sm_ctx.state)
		        {
			    case X2AP_ENB_CONFIG_ACTIVE_ST:
			    {
			        X2AP_TRACE(X2AP_INFO,"Sending ENDC Configuration Update to Peer gNB %u in the list\n", gnb_count+1);

			        /* Send ENDC CONFIGURATION UPDATE to peer gNB */
			        if(!(x2ap_send_endc_config_update_req_peer(p_x2ap_gb_ctx, p_peer_enb_ctx, asn_msg_buff, asn_msg_len, X2AP_ENB_CONFIG_UPDATE_RSP_TIMER)))
			        {
				    X2AP_TRACE(X2AP_ERROR,"Sending ENDC Configuration Update failed");
				    return;
			        }
			        else
			        {
			            /* Save ENDC Configuration Update request */
				    X2AP_MEMCPY(&p_peer_enb_ctx->endc_config_sm_ctx.endc_config_req, &endc_cfg_req, sizeof(x2ap_endc_config_update_req_t));

				    p_peer_enb_ctx->endc_config_sm_ctx.req_retry_count = 
					p_x2ap_gb_ctx->config_param.timer_conf_info.eNB_Config_Update_retry_cnt;

				    p_peer_enb_ctx->endc_config_sm_ctx.time_to_wait_retry_count =
					p_x2ap_gb_ctx->config_param.timer_conf_info.eNB_Config_Update_time_to_wait_retry_cnt;

				    p_peer_enb_ctx->endc_config_sm_ctx.transaction_id = trans_id;

				    p_peer_enb_ctx->endc_config_sm_ctx.dst_module_id = dest_module_id;
			        }
			    }
			    break;
			    case X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST:
			    {
			        /* Save ENDC Configuration Update request */
			        X2AP_MEMCPY(&p_peer_enb_ctx->endc_config_sm_ctx.endc_config_req, &endc_cfg_req, sizeof(x2ap_endc_config_update_req_t));
			    }
			    break;
			    case X2AP_ENB_CONFIG_IDLE_ST:
			    case X2AP_ENB_CONFIG_W_CONF_RESP_ST:
			    {
			        X2AP_TRACE(X2AP_ERROR,"Invalid state: %u.n", p_peer_enb_ctx->endc_config_sm_ctx.state);

			        endc_cfg_resp.cause.type = T_x2ap_Cause_radioNetwork;
			        endc_cfg_resp.cause.value = x2ap_unspecified_2;

			        X2AP_MEMCPY(&endc_cfg_resp.gnb_gb_id, &endc_cfg_req.gnb_info[gnb_count], sizeof(x2_gb_gnb_id_t));

			        /* Send ENDC Configuration Update response */
			        x2ap_send_endc_config_update_response(p_x2ap_gb_ctx, &endc_cfg_resp, X2AP_FAILURE, dest_module_id, trans_id);
			    }
			    break;
		        }
		    }
	        }
	    }
	}

	if (X2AP_FAILURE == retVal)
	{
	    U8 response_count =0;
	    for(response_count =0; response_count<endc_cfg_req.num_peer_gnb; response_count++)
	    {
	        X2AP_MEMCPY(&endc_cfg_resp.gnb_gb_id, &endc_cfg_req.gnb_info[response_count] , 
		    sizeof(x2_gb_gnb_id_t));

		endc_cfg_resp.cause.type = T_x2ap_Cause_radioNetwork;
		endc_cfg_resp.cause.value = x2ap_unspecified_2;

		/* Send ENDC Configuration Update response */
		x2ap_send_endc_config_update_response(p_x2ap_gb_ctx, &endc_cfg_resp, X2AP_FAILURE, dest_module_id, trans_id);
            }
        }    
    }
    else
    {
	X2AP_TRACE(X2AP_WARNING, "x2ap endc config update request parsing failed\n");
    }
}

/*****************************************************************************
 * Function Name  : x2ap_process_endc_config_update_ack 
 * Inputs         : p_msg_ptr - pointer to msg 
 *                  p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_enb_cntx - peer gNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE 
 * Description    : This function processes ENDC configuration update ack
 ********************************************************************************/
x2ap_return_et
x2ap_process_endc_config_update_ack
(
    U8* p_msg_ptr,
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_peer_enb_context_t* p_enb_cntx
)
{
    x2ap_return_et retVal = X2AP_FAILURE;
    x2ap_endc_config_update_ack_t endc_config_update_ack;
    x2ap_peer_enb_msg_t* p_peer_enb_msg = X2AP_P_NULL;
    U32 result;
    rrc_x2ap_error_indication_t  error_indication;
    x2ap_error_ind_bool_t        send_err_ind;
    OSCTXT 			 asn1_ctx;

    X2AP_UT_TRACE_ENTER();

    p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;

    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_cntx != X2AP_P_NULL);

    X2AP_MEMSET(&endc_config_update_ack, 0x00, sizeof(x2ap_endc_config_update_ack_t));

    retVal = x2ap_decode_endc_config_update_ack(&endc_config_update_ack, &result, 
		(p_peer_enb_msg)->p_msg, 
		&(p_peer_enb_msg->msg_len), 
		&error_indication,
	        &send_err_ind,
		&asn1_ctx,
		p_x2ap_gb_ctx,
		p_enb_cntx);

    if (X2AP_SUCCESS == retVal)
    {
        switch(p_enb_cntx->endc_config_sm_ctx.state)
        {
            case X2AP_ENB_CONFIG_IDLE_ST:
	     case X2AP_ENB_CONFIG_ACTIVE_ST:
	     case X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST:
            {
                X2AP_TRACE(X2AP_ERROR,"Invalid state. Ignore message.%u.n", p_enb_cntx->endc_config_sm_ctx.state);
            }
            break;
            case X2AP_ENB_CONFIG_W_CONF_RESP_ST:
            {
                U16 dest_module_id =0;
		 U16 trans_id =0;

		 x2ap_endc_config_update_resp_t endc_cfg_resp;

		 X2AP_TRACE(X2AP_ERROR,"Stop X2AP_ENB_CONFIG_UPDATE_RSP_TIMER.");

		 X2AP_MEMSET(&endc_cfg_resp, 0x00, sizeof(x2ap_endc_config_update_resp_t));

		 /* Stop ENDC Config Update Response guard Timer */
                x2ap_stop_timer(p_enb_cntx->endc_config_sm_ctx.req_timer_id);

                /* change state to X2AP_ENB_CONFIG_ACTIVE_ST */
                p_enb_cntx->endc_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
                p_enb_cntx->endc_config_sm_ctx.req_timer_id = X2AP_NULL;

                dest_module_id = p_enb_cntx->endc_config_sm_ctx.dst_module_id;
                trans_id = p_enb_cntx->endc_config_sm_ctx.transaction_id;

		 X2AP_MEMCPY(&endc_cfg_resp.gnb_gb_id, &p_enb_cntx->gb_gnb_id, sizeof(x2_gb_gnb_id_t));
		 
		 if (X2AP_ENDC_CONFIG_UPDATE_RESPOND_EN_GNB_PRESENT & 
		    endc_config_update_ack.bitmask)
		 {
                    endc_cfg_resp.bitmask |= X2AP_NR_CELL_INFOMATION_PRESENT;

		     x2ap_build_nr_srvd_cell_to_management_list(&endc_config_update_ack.respond_en_gnb,
                        &endc_cfg_resp.nr_cell_information);

                    //if (RRC_RRM_MODULE_ID != dest_module_id)
                    //{
                        /* Send ENDC Configuration Update response */
                        x2ap_send_endc_config_update_response(p_x2ap_gb_ctx, &endc_cfg_resp, X2AP_SUCCESS, dest_module_id, trans_id);
                    //}  
                }
                else
                {
                    endc_cfg_resp.cause.type = T_x2ap_Cause_radioNetwork;
                    endc_cfg_resp.cause.value = x2ap_unspecified_2;

                    /* Send ENDC Configuration Update response */
                    x2ap_send_endc_config_update_response(p_x2ap_gb_ctx, &endc_cfg_resp, X2AP_FAILURE, dest_module_id, trans_id);
                }

                X2AP_MEMSET(&p_enb_cntx->endc_config_sm_ctx, 0x00, sizeof(x2ap_endc_config_sm_context_t));

		 p_enb_cntx->endc_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
            }
            break;
        }
    }

    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_process_endc_config_update_failure 
 * Inputs         : p_msg_ptr -      pointer to msg buuffer
 *                  p_x2ap_gb_ctx - pointer to X2Ap global context
 *                  p_enb_cntx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function processes ENDC configuration update failure
 ********************************************************************************/
x2ap_return_et
x2ap_process_endc_config_update_failure(
    U8* p_msg_ptr,
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_peer_enb_context_t* p_enb_cntx)
{
    x2ap_return_et retVal = X2AP_FAILURE;
    x2ap_endc_config_update_failure_t endc_conf_failure;
    x2ap_peer_enb_msg_t* p_peer_enb_msg = X2AP_P_NULL;
    U32 result;
    rrc_x2ap_error_indication_t  error_indication;
    x2ap_error_ind_bool_t        send_err_ind;
    OSCTXT                       asn1_ctx;

    X2AP_UT_TRACE_ENTER();

    p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;

    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_cntx != X2AP_P_NULL);

    X2AP_MEMSET(&endc_conf_failure, 0x00, sizeof(x2ap_endc_config_update_failure_t));

    retVal = x2ap_decode_endc_config_update_resp_fail(&endc_conf_failure, 
		&result, 
		(p_peer_enb_msg)->p_msg, 
		&(p_peer_enb_msg->msg_len),
		p_x2ap_gb_ctx,
		p_enb_cntx,
		&error_indication,
		&send_err_ind,
		&asn1_ctx);

    if (X2AP_SUCCESS == retVal)
    {
        switch(p_enb_cntx->endc_config_sm_ctx.state)
        {
            case X2AP_ENB_CONFIG_IDLE_ST:
            case X2AP_ENB_CONFIG_ACTIVE_ST:
            case X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST:
            {
                X2AP_TRACE(X2AP_ERROR,"Invalid state. Ignore message.%u.n", p_enb_cntx->endc_config_sm_ctx.state);
            }
            break;
            case X2AP_ENB_CONFIG_W_CONF_RESP_ST:
            {
                U16 dest_module_id =0;
                U16 trans_id =0;

                if(X2AP_ENDC_CONFIG_UPDATE_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT & endc_conf_failure.bitmask)
                {
                    X2AP_TRACE(X2AP_INFO,"criticality diagnos is present");
                }

                X2AP_TRACE(X2AP_ERROR,"Stop X2AP_ENB_CONFIG_UPDATE_RSP_TIMER.");
		 x2ap_stop_timer(p_enb_cntx->endc_config_sm_ctx.req_timer_id);
		 p_enb_cntx->endc_config_sm_ctx.req_timer_id = 0;
		 
		 dest_module_id = p_enb_cntx->endc_config_sm_ctx.dst_module_id;
		 trans_id = p_enb_cntx->endc_config_sm_ctx.transaction_id;

		 if(X2AP_TIME_TO_WAIT_INDEX_PRESENT & endc_conf_failure.bitmask && 
		    0 < p_enb_cntx->endc_config_sm_ctx.time_to_wait_retry_count)
                {
			U32 timer_value =0;
			x2ap_timer_t timer_id;
			x2ap_timer_buffer_t timer_buf;
			x2ap_endc_config_update_wait_ind_t endc_time_to_wait_ind;

			X2AP_MEMSET(&endc_time_to_wait_ind, 0x00, sizeof(x2ap_endc_config_update_wait_ind_t));
			X2AP_MEMCPY(&endc_time_to_wait_ind.enb_gb_id, &p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));
			X2AP_MEMCPY(&endc_time_to_wait_ind.gnb_gb_id, &p_enb_cntx->gb_gnb_id, sizeof(x2_gb_gnb_id_t));
			endc_time_to_wait_ind.time_to_wait = endc_conf_failure.time_to_wait;

			/* Send ENDC Configuration Update response */
			rrc_x2apRrm_il_send_x2ap_endc_config_update_wait_ind(&endc_time_to_wait_ind, RRC_X2AP_MODULE_ID, dest_module_id, trans_id, RRC_NULL);

			/* Start X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST */
			x2ap_convert_time_to_wait_index_to_duration(p_x2ap_gb_ctx->config_param.timer_conf_info.x2ap_time_to_wait_index,&timer_value);
			timer_value = timer_value + p_x2ap_gb_ctx->config_param.timer_conf_info.eNB_Config_Update_time_to_wait_offset;

			timer_buf.auto_delete = X2AP_FALSE;
			timer_buf.sd = p_enb_cntx->sctp_sd;
			timer_buf.p_timer_specific_data = (void*)p_enb_cntx;
			timer_buf.timer_type = X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER;

			timer_id = x2ap_start_timer(timer_value, &timer_buf, sizeof(timer_buf), X2AP_FALSE);

			p_enb_cntx->endc_config_sm_ctx.state = X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST ;
			p_enb_cntx->endc_config_sm_ctx.time_to_wait_timer_id = timer_id;

		       X2AP_TRACE(X2AP_INFO,"Started X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER with timer_id %p and duration %u\n", timer_id, timer_value);
			X2AP_TRACE(X2AP_INFO,"endc_config_sm: State changed to X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST\n");
		}
		else
		{
		    X2AP_TRACE(X2AP_INFO,"ENDC Configuration Update time_to_wait retry count exhausted.");
			
			x2ap_endc_config_update_resp_t endc_cfg_resp;
			X2AP_MEMSET(&endc_cfg_resp, 0x00, sizeof(x2ap_endc_config_update_resp_t));

			/* change state to ACTIVE */
			x2ap_enb_config_update_fill_cause(&endc_conf_failure.cause,&endc_cfg_resp.cause);

			X2AP_MEMCPY(&endc_cfg_resp.gnb_gb_id, &p_enb_cntx->gb_gnb_id, sizeof(x2_gb_gnb_id_t));

			/* Send ENDC Configuration Update response */
			x2ap_send_endc_config_update_response(p_x2ap_gb_ctx, &endc_cfg_resp, X2AP_FAILURE, dest_module_id, trans_id);

			X2AP_MEMSET(&p_enb_cntx->endc_config_sm_ctx, 0x00, sizeof(x2ap_endc_config_sm_context_t));
			p_enb_cntx->endc_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
		}
            }
            break;
        }
    }

    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

#endif
