/*******************************************************************************
 *
 *  FILE NAME   : x2ap_common_intf.c
 *
 *  DESCRIPTION : This file contains the oam message handlers.
 *
 *  DATE 24-June-2010  NAME Sudhanshu Kumar Srivastav
 *                     REFERENCE  36.423_880(3GPP Spec) REASON
 *
 *  Copyright 2007, Aricent Inc.
 *
 *******************************************************************************/

#include "commoninc.h"
#include "x2ap_db.h"
#include "rrc_x2apOam_intf.h"
#include "x2ap_utils.h"
#include "sctp_ipr.h"
#include "x2ap_timer.h"
#include "x2ap_api.h"
#include "x2ap_cp_common.h"
#include "x2setup_sm.h"
#include "rrc_msg_mgmt.h"
#include "rrc_common_utils.h"
#include "sm_entry.h"
#include "x2ap_init.h"
#include "rrc_x2apCommon_il_parser.h"
#include "rrc_x2apCommon_il_composer.h"

//#ifdef ARICENT_SCTP_IPR
//#include "sctp_ipr.h"
//#else
#include "sctp_kernel.h"
//#endif

void    x2ap_process_add_enb(void* p_api, x2ap_gb_context_t   *p_x2ap_gb_ctx, U16 dest_module_id);
void    x2ap_process_del_enb(void* p_api, x2ap_gb_context_t   *p_x2ap_gb_ctx, U16 dest_module_id);
void    x2ap_process_enb_config_update_req(void* p_api, x2ap_gb_context_t   *p_x2ap_gb_ctx, U16 dest_module_id);

extern  x2ap_return_et  x2ap_create_server(
	        x2ap_gb_context_t *p_x2ap_gb_ctx);

extern void x2ap_stop_resource_status_update_procedure(
		x2ap_gb_context_t *p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t  *p_peer_enb_ctx);

extern void x2ap_stop_x2_handover_procedure(
		x2ap_peer_enb_context_t  *p_peer_enb_ctx);

extern void x2ap_stop_enb_configuration_update_procedure(
		x2ap_peer_enb_context_t  *p_peer_enb_ctx);

extern void x2ap_stop_mobility_setting_change_procedure(
        x2ap_peer_enb_context_t  *p_enb_ctx);

extern void x2ap_stop_reset_procedure( 
                x2ap_peer_enb_context_t  *p_enb_ctx);
/*****************************************************************************
 * Function Name  : x2ap_process_oam_add_enb
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *                  dest_module_id - destinatiom id
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the add enb request from OAM/SON to
 *                  establish SCTP associations with New ENB and sends the
 *                  response back to OAM/SON.
 ****************************************************************************/

void x2ap_process_add_enb(
        void                 *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        U16 dest_module_id)
{
	x2ap_return_et response = X2AP_SUCCESS;
	x2ap_add_enb_req_t *p_x2ap_add_enb = X2AP_P_NULL;
	static x2ap_add_enb_res_t x2ap_add_enb_resp;
	x2ap_peer_enb_context_t *p_peer_enb_cntxt = X2AP_P_NULL;

	S32 length_read = X2AP_NULL;
	U16 trans_id = X2AP_NULL;
	U8 loop = X2AP_NULL;
	/*klokwork fix*/
	U8 count = 0, enb_count = 0;
	/*klokwork fix*/
	/* X2 Link Fix start */
	rrc_bool_et is_peer_enb_present = RRC_FALSE; 
	/* X2 Link Fix stop */
	/*spr8341 IPv6 fix start*/
	struct sockaddr_in6         p_addr6;
	/*spr8341 IPv6 fix end*/
	/* BUG 9100 Fix Start */
	sctp_ip_search_node_t      *p_ip_node = X2AP_P_NULL;
	U8 fail_count = 0;
	U8 succ_count = 0;
	/* BUG 9100 Fix Stop */

	X2AP_UT_TRACE_ENTER();

	X2AP_ASSERT(X2AP_P_NULL != p_api);
	X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

	X2AP_MEMSET(&x2ap_add_enb_resp, 0, sizeof(x2ap_add_enb_res_t));
	/* Get the transaction id */
	trans_id = x2ap_get_transaction_id(p_api);

	/* Check the state of x2ap */
	if (X2AP_ACTIVE_ST != p_x2ap_gb_ctx->x2ap_init_sm_blk.x2ap_current_state)
	{
		X2AP_TRACE(X2AP_WARNING, "X2ap is in active state but should \
				ideally wait for provisioning request");
		return;
	}

	p_x2ap_add_enb = (x2ap_add_enb_req_t*)
		x2ap_mem_get(sizeof(x2ap_add_enb_req_t));
	if(X2AP_P_NULL == p_x2ap_add_enb)
	{
		X2AP_TRACE(X2AP_FATAL,"[%s]\nMemeory allocation failed ",__FUNCTION__);
		return;
	}

	/* Decode the received message from OAM/SON */
	response = rrc_parse_x2ap_add_enb_req(
			p_x2ap_add_enb,
			(U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
			x2ap_get_api_buf_size(p_api) -
			RRC_FULL_INTERFACE_HEADERS_SIZE,
			&length_read);

	if (X2AP_SUCCESS == response)
	{
		X2AP_TRACE(X2AP_DETAILED, "x2ap ADD ENB REQ decode success");

		if(p_x2ap_gb_ctx->num_peer_enb + p_x2ap_add_enb->num_peer_enb >
				MAX_PEER_ENB)
		{
            /*SPR_20743_FIX_START*/
            X2AP_TRACE(X2AP_WARNING, "sum of peer enb count = [%d] and peer enb to be added[%d]"
                 "should not be greater than MAX_PEER_ENB", p_x2ap_gb_ctx->num_peer_enb, p_x2ap_add_enb->num_peer_enb);
            /*SPR_20743_FIX_END*/

			response = X2AP_FAILURE;
			x2ap_add_enb_resp.bitmask = X2AP_FAILURE_ENB_ADD_LIST_PRESENT;
			/* BUG 9100 Fix Start */
			X2AP_MEMCPY(&x2ap_add_enb_resp.gb_enb_id,
					&p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));
			/* BUG 9100 Fix Stop */
			x2ap_add_enb_resp.failure_enb_list.num_peer_enb = p_x2ap_add_enb->num_peer_enb;
			X2AP_MEMCPY(x2ap_add_enb_resp.failure_enb_list.enb_comm_info,
					p_x2ap_add_enb->enb_info,
					p_x2ap_add_enb->num_peer_enb*sizeof(x2_enb_comm_info_t));
		}

		if (response == X2AP_SUCCESS)
		{
			/* BUG 9100 Fix Start */
			for (loop = 0, enb_count = 0; 
					((loop < MAX_PEER_ENB) && (enb_count < p_x2ap_add_enb->num_peer_enb) &&
					 (enb_count < MAX_PEER_ENB) && (succ_count < MAX_PEER_ENB)); loop++)
			{
                /* SPR 20633 changes start */
				if(p_x2ap_add_enb->enb_info[enb_count].bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT)
                /* SPR 20633 changes end */
				{
					for (count=0; ((count < p_x2ap_add_enb->enb_info[enb_count].num_ip_addr) &&
								(count < MAX_NUM_IP_ADDR)); ++count)
					{
						p_ip_node = (sctp_ip_search_node_t *)
							x2ap_st_find_tree_node(&p_x2ap_gb_ctx->enb_ctxt_trees.
									ip_search_tree,  &p_x2ap_add_enb->enb_info[enb_count].ip_addr[count].ip_addr[0]);
						if (p_ip_node != X2AP_P_NULL)
						{
							X2AP_TRACE(X2AP_INFO, "[%s] enb_comm_info.ip_addr[%d].ip_addr [%s], "
									"Client IP Address Found in Configuration!!", __FUNCTION__,
									enb_count, p_x2ap_add_enb->enb_info[enb_count].ip_addr[count].ip_addr);
							break;
						}
						else
						{
							X2AP_TRACE(X2AP_INFO, "[%s] enb_comm_info.ip_addr[%d].ip_addr [%s], "
									"Client IP Address Not Found in Configuration!!", __FUNCTION__, 
									enb_count, p_x2ap_add_enb->enb_info[enb_count].ip_addr[count].ip_addr);
						}
					}
				}
                /* SPR 20633 changes start */
				else if(p_x2ap_add_enb->enb_info[enb_count].bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
                /* SPR 20633 changes end */
				{
					for (count=0; ((count < p_x2ap_add_enb->enb_info[enb_count].num_ipv6_addr) &&
								(count < MAX_NUM_IP_ADDR)); ++count)
					{
						p_ip_node = (sctp_ip_search_node_t *)
							x2ap_st_find_tree_node(&p_x2ap_gb_ctx->enb_ctxt_trees.
									ipv6_search_tree,  &p_x2ap_add_enb->enb_info[enb_count].ipv6_addr[count].ipv6_addr[0]);
						if (p_ip_node != X2AP_P_NULL)
						{
							X2AP_TRACE(X2AP_INFO, "[%s] enb_comm_info.ipv6_addr[%d].ipv6_addr [%s], "
									"Client IPv6 Address Found in Configuration!!",  __FUNCTION__,
									enb_count, p_x2ap_add_enb->enb_info[enb_count].ipv6_addr[count].ipv6_addr);
							break;
						}
						else
						{
							X2AP_TRACE(X2AP_INFO, "[%s] enb_comm_info.ipv6_addr[%d].ipv6_addr [%s], "
									"Client IPv6 Address Not Found in Configuration!!", 
									__FUNCTION__, enb_count, p_x2ap_add_enb->enb_info[enb_count].ipv6_addr[count].ipv6_addr);
						}
					}
				}

				if ((p_ip_node != X2AP_P_NULL) && (X2AP_P_NULL != p_ip_node->p_enb_context))
				{
					if(OPERATIONAL == p_ip_node->p_enb_context->op_status)
					{
						X2AP_TRACE(X2AP_WARNING, "Peer eNodeB is in Operational state");
						x2ap_add_enb_resp.bitmask |= X2AP_FAILURE_ENB_ADD_LIST_PRESENT;
						X2AP_MEMCPY(&x2ap_add_enb_resp.gb_enb_id,
								&p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));
						x2ap_add_enb_resp.failure_enb_list.num_peer_enb++;
						X2AP_MEMCPY(&x2ap_add_enb_resp.failure_enb_list.enb_comm_info[fail_count],
								&p_x2ap_add_enb->enb_info[enb_count],
								sizeof(x2_enb_comm_info_t));
						fail_count++;
						enb_count++;
					}
					else
					{
                        /*Bug_12827_fix_start*/
                        if (X2AP_SCTP_ESTABLISHED_ST != 
                                p_ip_node->p_enb_context->sctp_sm_ctx.
                                cur_sctp_sm_state)
                        {
                            p_ip_node->p_enb_context->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_IDLE_ST;
                            p_peer_enb_cntxt = p_ip_node->p_enb_context;
                            X2AP_TRACE(X2AP_INFO, "peer_enodeb_id=%d found", p_ip_node->p_enb_context->peer_enodeb_id);
                            x2ap_sctp_sm_entry( X2AP_SCTP_CONNECT_EV,
                                    (U8*)p_x2ap_gb_ctx,
                                    p_x2ap_gb_ctx,
                                    p_peer_enb_cntxt);
                            p_ip_node->p_enb_context->is_enb_conf = X2AP_TRUE;
                        }
                        /*Bug_12827_fix_stop*/
						x2ap_add_enb_resp.bitmask |= X2AP_SUCCESS_ENB_ADD_LIST_PRESENT;
						X2AP_MEMCPY(&x2ap_add_enb_resp.gb_enb_id,
								&p_x2ap_gb_ctx->global_enb_id,
								sizeof(x2_gb_enb_id_t));
						x2ap_add_enb_resp.success_enb_list.num_peer_enb++;
						X2AP_MEMCPY(&x2ap_add_enb_resp.success_enb_list.enb_comm_info[succ_count],
								&p_x2ap_add_enb->enb_info[enb_count],
								sizeof(x2_enb_comm_info_t));
						succ_count++;
						enb_count++;
					}
				}
				else
				{
					/* BUG 9100 Fix Stop */
					/* X2 Link Fix start */
					is_peer_enb_present = RRC_FALSE; 
					/* X2 Link Fix stop */
					if (!p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].is_enb_conf)
					{
						/*SPR_16784_START*/
						X2AP_MEMSET((void*)&(p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop]), 
								X2AP_NULL, sizeof(x2ap_peer_enb_context_t));
						/*SPR_16784_END*/
						X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].enb_comm_info),
								(const void*)&(p_x2ap_add_enb->enb_info[enb_count]),
								sizeof(x2_enb_comm_info_t) );

						p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].sctp_sm_ctx.
							cur_sctp_sm_state = X2AP_SCTP_IDLE_ST;

						p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].peer_enodeb_id = (U8)loop;
						p_peer_enb_cntxt = &(p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop]);
                        /* SPR 20633 changes start */
						if(p_peer_enb_cntxt->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT )
                        /* SPR 20633 changes end */
						{
							for (count = 0; ((count < p_peer_enb_cntxt->enb_comm_info.num_ip_addr) &&
										(count < MAX_NUM_IP_ADDR)); ++count)
							{
								sctp_ip_search_node_t* 	p_temp_node = X2AP_P_NULL;

								/*Create Tree Node */
								sctp_ip_search_node_t  *p_tree_node = (sctp_ip_search_node_t*)
									x2ap_mem_get(sizeof(sctp_ip_search_node_t));

								if (PNULL == p_tree_node)
								{
									X2AP_TRACE(X2AP_WARNING, "Memory allocation failed.");
									/* coverity_fix_63326_start */
									x2ap_mem_free((void *)p_x2ap_add_enb);
									/* coverity_fix_63326_stop */
									return;
								}
								/* Copy IP Address in tree node */
								/*Coverity Fix 20194 Start*/
								l3_strncpy_wrapper ((S8 *)(p_tree_node->enb_ip_address),
										(const S8 *)(p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].
											enb_comm_info.ip_addr[count].ip_addr),
										MAX_X2AP_IP_ADDRESS_LENGTH);
								/*Coverity Fix 20194 End*/
								p_tree_node->p_enb_context = p_peer_enb_cntxt;

								p_temp_node = (sctp_ip_search_node_t *)
									x2ap_st_find_tree_node(&p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree, 
											&(p_tree_node->enb_ip_address[0]));

								if(p_temp_node == X2AP_P_NULL)
								{
									/* Insert in ip_port tree*/
									x2ap_st_insert_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree), 
											&(p_tree_node->nodeAnchor));
								}
								else
								{
									X2AP_TRACE(X2AP_INFO, "IP Address present in tree");
									/* Update p_enb_context in tree */
									p_temp_node->p_enb_context = p_peer_enb_cntxt;
									/* X2 Link Fix start */
									is_peer_enb_present = RRC_TRUE;
									/* X2 Link Fix stop */
								}
							}
						} 
						else
						{
							for (count = 0; ((count < p_peer_enb_cntxt->enb_comm_info.num_ipv6_addr) &&
										(count < MAX_NUM_IP_ADDR)); ++count)
							{
								sctp_ip_search_node_t* 	p_temp_node = X2AP_P_NULL;

								/*Create Tree Node */
								sctp_ip_search_node_t  *p_tree_node = (sctp_ip_search_node_t*)
									x2ap_mem_get(sizeof(sctp_ip_search_node_t));

								if (PNULL == p_tree_node)
								{
									X2AP_TRACE(X2AP_WARNING, "Memory allocation failed.");
									/* coverity_fix_63326_start */
									x2ap_mem_free((void *)p_x2ap_add_enb);
									/* coverity_fix_63326_stop */
									return;
								}
								/* Copy IP Address in tree node */
								l3_strncpy_wrapper ((S8 *)(p_tree_node->enb_ipv6_address),
										(const S8 *)(p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].
											enb_comm_info.ipv6_addr[count].ipv6_addr),
										MAX_X2AP_IPV6_ADDRESS_LENGTH);
								/*spr8341 IPv6 fix start*/
								bzero_wrapper( (void *)&p_addr6, sizeof(struct sockaddr_in6) );

								if(inet_pton_wrapper(AF_INET6,(const char *)(p_tree_node->enb_ipv6_address),
											(void *)(&p_addr6.sin6_addr)) != 1)
								{
									X2AP_TRACE(X2AP_WARNING, "X2ap inet_pton_wrapper failed");
									/* coverity_fix_63326_start */
									x2ap_mem_free((void *)p_x2ap_add_enb);
									/* coverity_fix_63326_stop */
									/* coverity_fix_63327_start */
									x2ap_mem_free((void *)p_tree_node);
									/* coverity_fix_63327_stop */
									return;
								}

								inet_ntop_wrapper(AF_INET6,(const void *)&p_addr6.sin6_addr,
										(char *)(p_tree_node->enb_ipv6_address),
										MAX_X2AP_IPV6_ADDRESS_LENGTH);
								p_tree_node->p_enb_context = p_peer_enb_cntxt;

								p_temp_node = (sctp_ip_search_node_t *)
									x2ap_st_find_tree_node(&p_x2ap_gb_ctx->enb_ctxt_trees.ipv6_search_tree, 
											&(p_tree_node->enb_ipv6_address[0]));
								/*spr8341 IPv6 fix end*/

								if(p_temp_node == X2AP_P_NULL)
								{
									/* Insert in ip_port tree*/
									x2ap_st_insert_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.ipv6_search_tree), 
											&(p_tree_node->nodeAnchor));
								}
								else
								{
									X2AP_TRACE(X2AP_INFO, "IPV6 Address present in tree");
									/* Update p_enb_context in tree */
									p_temp_node->p_enb_context = p_peer_enb_cntxt;
									/* X2 Link Fix start */
									is_peer_enb_present = RRC_TRUE;
									/* X2 Link Fix stop */
								}
							}
						}

						x2ap_sctp_sm_entry(
								X2AP_SCTP_CONNECT_EV,
								/*coverity_fix*/
								(U8*)p_x2ap_gb_ctx,
								/*coverity_fix*/
								p_x2ap_gb_ctx,
								p_peer_enb_cntxt);

						/* X2 Link Fix start */
						if(RRC_FALSE == is_peer_enb_present)
						{
							p_x2ap_gb_ctx->num_peer_enb++;
							p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].is_enb_conf = X2AP_TRUE;
						}
						/* X2 Link Fix stop */
						x2ap_add_enb_resp.bitmask |= X2AP_SUCCESS_ENB_ADD_LIST_PRESENT;
						X2AP_MEMCPY(&x2ap_add_enb_resp.gb_enb_id,
								&p_x2ap_gb_ctx->global_enb_id,
								sizeof(x2_gb_enb_id_t));
						x2ap_add_enb_resp.success_enb_list.num_peer_enb++;
						X2AP_MEMCPY(&x2ap_add_enb_resp.success_enb_list.enb_comm_info[succ_count],
								&p_x2ap_add_enb->enb_info[enb_count],
								sizeof(x2_enb_comm_info_t));
						succ_count++;
						enb_count++;
					}
				}
			}
		}

		/* Encode and send the add enb response OAM/SON */
		response = rrc_x2apCommon_send_x2ap_add_enb_res(
				&x2ap_add_enb_resp,
				RRC_X2AP_MODULE_ID,
				dest_module_id,
				trans_id,
				RRC_NULL);
		/* + Coverity 81478 */
		if ( X2AP_FAILURE == response )
        	{
            		X2AP_TRACE(X2AP_ERROR, "[%s] X2AP ADD ENB RESP send failure",
                    		__FUNCTION__);
        	}
		/* - Coverity 81478 */
	}
	else
	{
		X2AP_TRACE(X2AP_WARNING, "x2ap ADD ENB REQ decode failed");
	}
	/* BUG 9100 Fix Start */
	x2ap_mem_free(p_x2ap_add_enb);
	/* BUG 9100 Fix Stop */

	X2AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : x2ap_process_del_enb
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *                  dest_module_id - Destination module id
 *
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the delete enb request from OAM/SON to
 *                  terminate SCTP associations with Peer ENB and sends the
 *                  response back to OAM/SON.
 ****************************************************************************/
void x2ap_process_del_enb(void* p_api, 
                          x2ap_gb_context_t   *p_x2ap_gb_ctx, 
                          U16 dest_module_id)
{
    x2ap_return_et   response = X2AP_FAILURE;
    x2ap_delete_enb_req_t   *p_x2ap_del_enb_req = X2AP_P_NULL;
    x2ap_delete_enb_res_t   x2ap_del_enb_resp;

    S32   length_read = X2AP_NULL;
    U16   trans_id = X2AP_NULL;
    U8 o_c = X2AP_NULL;
    U8 count = 0, ip_deleted = 0;
    x2ap_peer_enb_context_t *p_enb_ctx = X2AP_P_NULL;
/*spr8341 IPv6 fix start*/
    struct sockaddr_in6         p_addr6;
    U8  enb_ipv6_address[MAX_X2AP_IPV6_ADDRESS_LENGTH];
/*spr8341 IPv6 fix end*/


    X2AP_UT_TRACE_ENTER();
    
    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);


    /* Get the transaction id */
    trans_id = x2ap_get_transaction_id(p_api);

    p_x2ap_del_enb_req = (x2ap_delete_enb_req_t*)
        x2ap_mem_get(sizeof(x2ap_delete_enb_req_t));
    if(X2AP_P_NULL == p_x2ap_del_enb_req)
    {
        X2AP_TRACE(X2AP_FATAL,"[%s]\nMemeory allocation failed ",__FUNCTION__);
        return;
    }
    X2AP_MEMSET(p_x2ap_del_enb_req, X2AP_NULL, sizeof(x2ap_delete_enb_req_t));
    X2AP_MEMSET(&x2ap_del_enb_resp, X2AP_NULL, sizeof(x2ap_delete_enb_res_t));

    /* Decode the received message from SON/OAM */
    response = rrc_parse_x2ap_delete_enb_req(
            p_x2ap_del_enb_req ,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            x2ap_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read);


    if (X2AP_SUCCESS == response)
    {
        for(o_c = 0; o_c < MAX_PEER_ENB; o_c ++)
        {
            if(X2AP_TRUE ==
                  p_x2ap_gb_ctx->peer_enb_cntxt_arr[o_c].is_enb_conf)
            {
                 if (0 == X2AP_MEMCMP(&p_x2ap_del_enb_req->gb_enb_id,
				      &p_x2ap_gb_ctx->peer_enb_cntxt_arr[o_c].gb_enb_id,
				      sizeof(x2_gb_enb_id_t)))
                 {
                     p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[o_c];
                     break;
                 }
            }
        }
        if(p_enb_ctx == X2AP_P_NULL)
        {
            /*SPR_17670_START*/
            /*Setting delete response as success as no peer eNB context exists*/
            response = X2AP_SUCCESS;
            /*SPR_17670_END*/
            X2AP_TRACE(X2AP_WARNING, "No peer eNB context found");
        }
        else
        {
            p_enb_ctx->is_del_enb_ongoing = X2AP_TRUE;

            /* Stop RESET procedure */
            x2ap_stop_reset_procedure(p_enb_ctx);

            /* Stop X2 HANDOVER procedure */
            x2ap_stop_x2_handover_procedure(p_enb_ctx);

            /* Stop RESOURCE STATUS UPDATE procedure */
            x2ap_stop_resource_status_update_procedure(
                    p_x2ap_gb_ctx, p_enb_ctx);

            /* Stop eNB CONFIGURATION UPDATE procedure */
            x2ap_stop_enb_configuration_update_procedure(
                    p_enb_ctx);

            /* Stop Mobility Setting Change procedure */
            x2ap_stop_mobility_setting_change_procedure(
                     p_enb_ctx);
            
            /*Send link down indication to OAM*/
            response = x2ap_sctp_sm_entry(X2AP_SCTP_CONN_CLOSE_EV,
                    (U8 *)&p_enb_ctx->gb_enb_id,
                    p_x2ap_gb_ctx,
                    p_enb_ctx );

            /* Decrementing the number of peer enb count */
                            /*SPR_20743_FIX_START*/
                             /*code deleted*/
                            /*SPR_20743_FIX_END*/
/*spr8341 IPv6 fix start*/
            if(!(p_enb_ctx->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT))
            {
/*spr8341 IPv6 fix end*/
                for (count = 0; count < p_enb_ctx->enb_comm_info.num_ip_addr;
                        count++)
                {
                    x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
                    p_node_to_delete = x2ap_st_find_tree_node(
                            &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
                            &(p_enb_ctx->enb_comm_info.ip_addr[count]));
                    if (X2AP_P_NULL != p_node_to_delete)
                    {
                        /*SPR_20743_FIX_START*/
                        /*p_x2ap_gl_ctx->num_peer_enb will be decremented inside x2ap_delete_peer_enb_from_search_tree
                        only once for all the IP's of an eNB and after decrementing p_peer_enb_ctxt->is_enb_conf will be set to RRC_FALSE*/
                        x2ap_delete_peer_enb_from_search_tree(
                            p_x2ap_gb_ctx,
                                &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
                            p_enb_ctx,
                                p_node_to_delete);
                        /*SPR_20743_FIX_END*/
                        ++ip_deleted;
                    }
                }
            }
/*spr8341 IPv6 fix start*/
            else
            {
                for (count = 0; count < p_enb_ctx->enb_comm_info.num_ipv6_addr;
                        count++)
                {
                    x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
                    X2AP_MEMSET(enb_ipv6_address, X2AP_NULL,
                            MAX_X2AP_IPV6_ADDRESS_LENGTH);
                    l3_strncpy_wrapper ((S8 *)(enb_ipv6_address),
                            (const S8 *)(p_enb_ctx->enb_comm_info.
                                ipv6_addr[count].ipv6_addr),
                            MAX_X2AP_IPV6_ADDRESS_LENGTH);
                    bzero_wrapper( (void *)&p_addr6, sizeof(struct sockaddr_in6) );

                    if(inet_pton_wrapper(AF_INET6,(const char *)(enb_ipv6_address),
                                (void *)(&p_addr6.sin6_addr)) != 1)
                    {
                        X2AP_TRACE(X2AP_WARNING, "X2ap inet_pton_wrapper failed");
                        /* coverity_fix_63328_start */
                        x2ap_mem_free((void *)p_x2ap_del_enb_req);
                        /* coverity_fix_63328_stop */
                        return;
                    }

                    inet_ntop_wrapper(AF_INET6,(const void *)&p_addr6.sin6_addr,
                            (char *)(enb_ipv6_address),
                            MAX_X2AP_IPV6_ADDRESS_LENGTH);
                    p_node_to_delete = x2ap_st_find_tree_node(
                            &(p_x2ap_gb_ctx->enb_ctxt_trees.ipv6_search_tree),
                            &(enb_ipv6_address));
                    if (X2AP_P_NULL != p_node_to_delete)
                    {
                        /*SPR_20743_FIX_START*/
                        /*p_x2ap_gl_ctx->num_peer_enb will be decremented inside x2ap_delete_peer_enb_from_search_tree
                        only once for all the IP's of an eNB and after decrementing p_peer_enb_ctxt->is_enb_conf will be set to RRC_FALSE*/
                        x2ap_delete_peer_enb_from_search_tree(
                            p_x2ap_gb_ctx,
                                &(p_x2ap_gb_ctx->enb_ctxt_trees.ipv6_search_tree),
                            p_enb_ctx,
                                p_node_to_delete);
                        /*SPR_20743_FIX_END*/
                        ++ip_deleted;
                    }
                }
            }
            /* coverity_fix_63317_start */
            for (count = 0; count < p_enb_ctx->x2ap_gu_grp_id.num_gu_group_id; count++)
            {
                x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
                p_node_to_delete = x2ap_st_find_tree_node(
                        &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
                        &(p_enb_ctx->x2ap_gu_grp_id.group_id_arr[count]));
                if (X2AP_P_NULL != p_node_to_delete)
                {
                    x2ap_st_delete_node(
                            &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
                            p_node_to_delete);
                }
            }
            /* coverity_fix_63317_stop */
/*spr8341 IPv6 fix end*/
            X2AP_TRACE (X2AP_INFO,"Deleted %u ip_search_tree.\n", ip_deleted);

            X2AP_MEMSET(p_enb_ctx, X2AP_NULL, sizeof(x2ap_peer_enb_context_t));
        }

        /* Send DEL_ENB_RESP */
        x2ap_del_enb_resp.bitmask = 0;
        X2AP_MEMCPY(&x2ap_del_enb_resp.enb1_gb_id, 
                &p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));
        X2AP_MEMCPY(&x2ap_del_enb_resp.enb2_gb_id,
                &p_x2ap_del_enb_req->gb_enb_id, sizeof(x2_gb_enb_id_t));

        x2ap_del_enb_resp.response = response;

        if( !(rrc_x2apCommon_send_x2ap_delete_enb_res(
                        &x2ap_del_enb_resp,
                        RRC_X2AP_MODULE_ID,
                        dest_module_id,
                        trans_id,
                        RRC_NULL)))
        {
            X2AP_TRACE(X2AP_ERROR, "[%s]: X2AP DELETE ENB RESP send failure",
                    __FUNCTION__);
        }
    }
    else
    {
        X2AP_TRACE(X2AP_WARNING, "X2AP_DELETE_ENB_REQ decode failed");
    }

    X2AP_UT_TRACE_EXIT();
    /* coverity_fix_63328_start */
    x2ap_mem_free((void *)p_x2ap_del_enb_req);
    /* coverity_fix_63328_stop */
    return;
}

