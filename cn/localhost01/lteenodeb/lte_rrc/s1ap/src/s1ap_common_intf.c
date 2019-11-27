
/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: s1ap_oam.c,v 1.8 2010/10/13 07:04:24 gur18569 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the definitions of functions handling
 *                     OAM requests.
 *
 ******************************************************************************/

#include "rrc_s1apCommon_intf.h"
#include "rrc_s1apCommon_il_composer.h"
#include "rrc_s1apCommon_il_parser.h"
#include "s1ap_global_ctx.h"
#include "rrc_msg_mgmt.h"
#include "rrc_common_utils.h"
#include "sctp_wrappers.h"
#include "list_wrap.h"
#include "s1ap_mme_fsm.h"
#include "s1ap_enb_config_fsm.h"

rrc_bool_et validate_dynamic_s1ap_oam_ip_addresses(
        s1ap_add_mme_req_t *p_s1ap_add_mme_info);

void  establish_dynamic_sctp_associations(
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        s1ap_add_mme_req_t* p_s1ap_add_mme_info);

extern S32
compare_mmeUeS1apId_search_tree(
        const void * p_elem1,
        const void * p_elem2);

extern const void *
key_of_mmeUeS1apId_search_tree(
        const YTNODE * p_node);

/*****************************************************************************
* Function Name  : s1ap_oam_process_add_mme_req
* Inputs         : p_api - Pointer to input API buffer
*                  p_s1ap_gb_ctx - Pointer to the S1AP global context
* Outputs        : None
* Returns        : None
* Description    : This function process the message to establisth the mme conn
****************************************************************************/
void s1ap_oam_process_add_mme_req(
        void *p_api,
        s1ap_gb_context_t *p_s1ap_gb_ctx)
{


    S32 length_read = RRC_NULL;
    rrc_return_et response = RRC_FAILURE;
    U16 trans_id = RRC_NULL;
    s1ap_add_mme_req_t s1ap_add_mme_req ;
    s1ap_add_mme_res_t s1ap_add_mme_resp = {0} ;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    /* Get the transaction id */
    trans_id = rrc_get_transaction_id(p_api);

    /* Decode the received message from OAM or SON */
        response = rrc_parse_s1ap_add_mme_req(
                &s1ap_add_mme_req ,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read);

        if (RRC_SUCCESS == response)
        {
            RRC_S1AP_TRACE(RRC_INFO, "S1AP OAM ADD MME REQ decode success");
            RRC_S1AP_TRACE(RRC_INFO, "MME count is ---- [%d]\n",s1ap_add_mme_req.num_mme);

            if (p_s1ap_gb_ctx->num_of_mme_prov == MAX_NUM_MME )
            {
                RRC_S1AP_TRACE(RRC_ERROR, "Maximum Number of MME already present in S1AP global context \n");
                s1ap_add_mme_resp.response = RRC_FAILURE;
                /* Encode and send the provisioning response to OAM */

                response = rrc_s1apCommon_send_s1ap_add_mme_res(
                            &s1ap_add_mme_resp,
                            RRC_S1AP_MODULE_ID,
			    RRC_OAM_MODULE_ID,
                            trans_id,
                            RRC_NULL);
		/* + Coverity 81425 */  
        	if ( RRC_FAILURE == response )
        	{
            		RRC_S1AP_TRACE(RRC_WARNING, "S1AP OAM ADD MME RESP sending failed");
        	}
		/* - Coverity 81425 */  
                return;
            }
            if ((p_s1ap_gb_ctx->num_of_mme_prov + s1ap_add_mme_req.num_mme) > MAX_NUM_MME)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "Number of MME reached maximum limit\n");
                s1ap_add_mme_resp.response = RRC_FAILURE;
                /* Encode and send the provisioning response to OAM */

                response = rrc_s1apCommon_send_s1ap_add_mme_res(
                            &s1ap_add_mme_resp,
                            RRC_S1AP_MODULE_ID,
			    RRC_OAM_MODULE_ID,
                            trans_id,
                            RRC_NULL);
		/* + Coverity 81425 */  
        	if ( RRC_FAILURE == response )
        	{
            		RRC_S1AP_TRACE(RRC_WARNING, "S1AP OAM ADD MME RESP sending failed");
        	}
		/* - Coverity 81425 */  
                return;
            }
            else
            {
                s1ap_add_mme_resp.response = RRC_SUCCESS;
            }


            if (RRC_FALSE ==
                    validate_dynamic_s1ap_oam_ip_addresses(&s1ap_add_mme_req))
            {
                s1ap_add_mme_resp.response = RRC_FAILURE;
            }
        }
        else
        {
            RRC_S1AP_TRACE(RRC_WARNING, "S1AP OAM ADD MME REQ decode failed");
            s1ap_add_mme_resp.response = RRC_FAILURE;
        }


        /* Encode and send the provisioning response to OAM */
        response = rrc_s1apCommon_send_s1ap_add_mme_res(
                &s1ap_add_mme_resp,
                RRC_S1AP_MODULE_ID,
		RRC_OAM_MODULE_ID,
                trans_id,
                RRC_NULL);

        if (RRC_SUCCESS == response)
        {
            if ( s1ap_add_mme_resp.response == RRC_SUCCESS)
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP OAM ADD MME RESP encode success");

                /* Function call to form sctp association */
                establish_dynamic_sctp_associations(p_s1ap_gb_ctx, &s1ap_add_mme_req);
                RRC_S1AP_TRACE(RRC_INFO, "S1AP has entered Active State");
            }

        }
        else
        {
            RRC_S1AP_TRACE(RRC_WARNING, "S1AP OAM ADD MME RESP encode failed");
            return;
        }
}

/*****************************************************************************
 * Function Name  : validate_dynamic_s1ap_oam_ip_addresses
 * Inputs         : p_s1ap_oam_info - pointer to S1AP PROV INFO
 * Outputs        : None
 * Returns        : RRC_TRUE/RRC_FALSE
 * Description    : The function validates MME/eNB IP Addresses came in S1AP_
 *                  OAM_PROV_REQ
 * *****************************************************************************/
rrc_bool_et validate_dynamic_s1ap_oam_ip_addresses(
        s1ap_add_mme_req_t *p_s1ap_add_mme_info
        )
{

    U8 mme_counter = 0;
    U8 no_ip_count = 0;

    RRC_S1AP_UT_TRACE_ENTER();
    /* Check for MME IP Addresses */
    for (mme_counter = 0; mme_counter < p_s1ap_add_mme_info->num_mme; mme_counter++)
    {
        if(!(p_s1ap_add_mme_info->mme_comm_info_arr[mme_counter].bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT))   
        {
            for (no_ip_count = 0;
                    no_ip_count < p_s1ap_add_mme_info->mme_comm_info_arr[mme_counter].num_ip_addr;
                    no_ip_count++)
            {
                if (RRC_SUCCESS !=
                        validate_ip_addr(
                            p_s1ap_add_mme_info->mme_comm_info_arr[mme_counter].ip_addr[no_ip_count].ip_addr))
                {
                    RRC_S1AP_TRACE(RRC_ERROR,"[S1AP_OAM_PROV_REQ]: MME IP ADDRESS VALIDATION FAILED");
                    return RRC_FALSE;
                }
            }
        }
    }

    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_TRUE;
}

/*****************************************************************************
 * Function Name  : establish_dynamic_sctp_associations
 * Inputs         : p_s1ap_gb_ctx       : pointer to S1AP global context
                    p_s1ap_add_mme_info : Pointer to s1ap oam/son add mme req
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when S1AP_ADD_MME_REQ message is
 *                  received from OAM. The event in this case is 
 *                  EVENT_S1_MME_INIT_REQ.
 *****************************************************************************/
void  establish_dynamic_sctp_associations(
        s1ap_gb_context_t * p_s1ap_gb_ctx,
        s1ap_add_mme_req_t* p_s1ap_add_mme_info)
{

    U8 num_mme = RRC_NULL;
    U8 mme_count= 0;
    U8 mme_index = p_s1ap_gb_ctx->num_of_mme_prov;

    num_mme = p_s1ap_add_mme_info->num_mme;
    mme_context_t * p_mme_ctx = PNULL;

    /* SPR 13432 Fix Start */
    U8 num_mme_connected = S1AP_FALSE;
    /* SPR 13432 Fix Stop */

    /* Initialize the enb_config_updated_count to zero */
    p_s1ap_gb_ctx->enb_config_updated_gb_count = 0;

    for ( mme_count = 0 ; mme_count < num_mme ; mme_count++)
    {
        /*Create a mme_context at p_gb_context->mme_contexts[mme_count]*/
        /* Allocate Memory for UE global context data structure */
        p_mme_ctx = (mme_context_t *)rrc_mem_get(sizeof(
                    mme_context_t));

        if (PNULL == p_mme_ctx)
        {
            RRC_S1AP_TRACE(RRC_FATAL, "p_mme_ctx: unable to"
                    " Allocate Memory for MME Global context!!");
            return /*PNULL*/;
        }

        memset_wrapper((void*)p_mme_ctx, 0, sizeof(mme_context_t));

        /*Update the global context*/
        p_s1ap_gb_ctx->num_of_mme_prov++;

        /* fill mme comm info */
        p_mme_ctx->mme_id = mme_index;

        p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;
        /*Storing the UE context in the S1AP global contexts*/
        p_s1ap_gb_ctx->mme_contexts[mme_index++] = p_mme_ctx;

        /*Set the elements of the mme_context*/
        /*current_mme_state  = MME_IDLE*/
        p_mme_ctx->current_mme_state = MME_IDLE ;

        p_mme_ctx->current_enb_config_update_state =
            ENB_STATE_RECV_ENB_CONFIG_FROM_OAM;

        p_mme_ctx->p_enb_configuration_update = PNULL;

        /*Filling mme connection_info */
        l3_memcpy_wrapper ((void*)&p_mme_ctx->connection_info,
                (const void*)&(p_s1ap_add_mme_info->mme_comm_info_arr[mme_count]),
                sizeof ( mme_comm_info_t));


        p_mme_ctx->s1_config_retry_count = 0;
        p_mme_ctx->oam_transaction_id = 0;
        p_mme_ctx->enb_config_updated_count = 0;
        /*mmeUeS1apId Tree Initialisation*/
        st_init(&(p_mme_ctx->mmeUeS1apId_search_tree),
                &compare_mmeUeS1apId_search_tree,
                key_of_mmeUeS1apId_search_tree);

        /* SPR 13432 Fix Start */
        if (p_s1ap_gb_ctx->s1_conn_mode_one_flag)
        {
            if ((S1AP_FALSE == num_mme_connected) &&
		         (MAX_NUM_MME == p_s1ap_gb_ctx->last_selected_mme_id))
            {
        mme_fsm_process_event(
                MME_EVENT_S1_MME_PROV_REQ,
                &(p_mme_ctx->connection_info),
                &(p_s1ap_gb_ctx->enb_comm_info),
                p_s1ap_gb_ctx,
                p_mme_ctx);

                num_mme_connected = S1AP_TRUE;
            }
        }
        else
        {
            mme_fsm_process_event(
                    MME_EVENT_S1_MME_PROV_REQ,
                    &(p_mme_ctx->connection_info),
                    &(p_s1ap_gb_ctx->enb_comm_info),
                    p_s1ap_gb_ctx,
                    p_mme_ctx);
        }        
        /* SPR 13432 Fix Stop */
    }
}
