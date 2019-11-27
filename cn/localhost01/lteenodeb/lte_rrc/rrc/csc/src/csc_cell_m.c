/******************************************************************************
*
*   FILE NAME:
*       csc_cell_m.c
*
*   DESCRIPTION:
*       This header file contains in points of CSC cell manager submodule
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*   06 June 2009    VasylS      ---------       Cleanup support
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "csc_cell_m.h"
#include "rrc_defines.h"
#include "csc_logger.h"
#include "rrc_common_utils.h"
#include "csc_cell_ctx.h"
#include "csc_cell_m_timers.h"
#include "csc_cell_m_fsm.h"
#include "rrc_msg_mgmt.h"
#include "csc_utils.h"
#include "rrc_intrl_api.h"
#include "rrc_ext_api.h"
#include "rrc_rrm_intf.h"
#include "rrc_csc_llim_intf.h"
#include "rrc_rrm_il_parser.h"

#include "csc_sfn_m.h"
#include "rrc_s1ap_csc_intf.h"
#include "rrc_csc_uecc_intf.h"
#include "rrc_s1ap_asn_intrl.h"
#include "rrc_s1ap_asn_intrl_dec.h"
#include "rrc_asn_enc_dec.h"
#include "rtsrc/rtCopy.h"

#include "csc_paging_m.h"
#include "rrc_oamh_intf.h"
#include "csc_oamh.h"
#include "rrc_x2apCsc_intf.h"

/*****************************************************************************
   * Private Function Prototypes
****************************************************************************/

/* SPR 1643 Start */
static rrc_return_t validate_paging_plmn_identity(
    TAIListIntrl            *p_tai_list); 
/* SPR 1643 Stop */
/* SPR 20908 Start */
/******************************************************************************
 * *   FUNCTION NAME: ext_comm_info_lower_layer_update
 * *
 * *   DESCRIPTION:
 * *       Cleans up CSC Cell context but paging is not deinitialized again.
 * *
 * *   RETURNS:
 * *       None
 * *
 * ******************************************************************************/
void ext_comm_info_lower_layer_update
(
    rrc_oam_ext_comm_info_t *p_comm_info,
    rrc_l2_comm_info_t      *p_l2comm_info 
)
{
    U8 count;
    RRC_ASSERT(PNULL != p_comm_info);
    RRC_ASSERT(PNULL != p_l2comm_info);

    p_comm_info->bitmask = RRC_OAM_IP_ADDR_PRESENT;
    p_comm_info->transport_mode = OAM_TRANSPORT_MODE_UDP;

    for (count=0; count < MAC_IP_ADDRESS_LENGTH; count++)
    {
        p_comm_info->ip_addr[count] = p_l2comm_info->ip_addr[count];
    }

    p_comm_info->port = p_l2comm_info->port;

}

/* SPR 20908 End */


/******************************************************************************
*   FUNCTION NAME: cell_m_rrm_cell_setup_req_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION:
*       This function partially process RRC_RRM_CELL_SETUP_REQ message
*       and call FSM.
*
*
******************************************************************************/
void cell_m_rrm_cell_setup_req_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    rrc_return_t    result = RRC_FAILURE;
    static rrc_rrm_cell_setup_req_t cell_setup_req;
    /* SPR 20908 Start */
    void *p_intrl_msg = PNULL;
    rrc_oamh_communication_info_ind_t *p_oamh_communication_info_ind = PNULL;
    /* SPR 20908 End */
    csc_cell_ctx_t *p_cell_ctx  = PNULL;
    S32 length_read             = RRC_NULL;
    rrm_fail_cause_et       fail_cause = RRM_RRC_INTERNAL_FAILURE;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    memset_wrapper(&cell_setup_req, 0 ,sizeof(rrc_rrm_cell_setup_req_t));

    do {
        /* Precondition - p_api should point to right message */
        if (RRC_SUCCESS != rrc_il_parse_rrc_rrm_cell_setup_req(
                    &cell_setup_req,
                    (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                    rrc_get_api_buf_size(p_api) -
                    RRC_FULL_INTERFACE_HEADERS_SIZE,
                    &length_read))
        {
            fail_cause = RRM_RRC_INVALID_CONFIGURATION_RECV;
            RRC_CSC_TRACE(RRC_WARNING, "Bad message - ignore.");
            RRC_CSC_UT_TRACE_EXIT();
            /* Drop message */
            break;
        }

        SET_CELL_INDEX(cell_setup_req.cell_index);   

        /* Special case on create */
        /* Search in DB cell with cell_setup_req.cell_index - should be absent */
        if (PNULL != p_csc_gl_ctx->p_p_cell_ctx)
        {
            p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[cell_setup_req.cell_index];
            if (PNULL != p_cell_ctx)
            {
                RRC_CSC_TRACE(RRC_WARNING,
                        "[CELL:%u] Already exist", cell_setup_req.cell_index);
                break;
            }

            /* Create and add to DB */
            p_cell_ctx = csc_cell_ctx_create(p_csc_gl_ctx,
                    cell_setup_req.cell_index);
            if (PNULL == p_cell_ctx)
            {
                RRC_CSC_TRACE(RRC_WARNING,
                        "[CELL:%u] Context initialization failed",
                        cell_setup_req.cell_index);
                break;
            }
            p_csc_gl_ctx->p_p_cell_ctx[cell_setup_req.cell_index] = p_cell_ctx;

            /* SPR 20908 Start */

            p_csc_gl_ctx->p_p_cell_ctx[cell_setup_req.cell_index]->p_api = (void *) rrc_mem_get(rrc_get_api_buf_size(p_api));     
            l3_memcpy_wrapper(p_csc_gl_ctx->p_p_cell_ctx[cell_setup_req.cell_index]->p_api,p_api,rrc_get_api_buf_size(p_api));

            p_csc_gl_ctx->p_p_cell_ctx[cell_setup_req.cell_index]->associated_layer2_inst = cell_setup_req.cell_config_param.associated_layer2_inst; 
            p_csc_gl_ctx->p_p_cell_ctx[cell_setup_req.cell_index]->l2_comm_info_params = cell_setup_req.cell_config_param.l2_comm_info_params;

            p_intrl_msg = csc_alloc_intrl_msg( 
                    RRC_OAMH_MODULE_ID,
                    CSC_OAMH_OAM_COMMUNICATION_INFO_IND,
                    sizeof (rrc_oamh_communication_info_ind_t));

            /*   Send CSC_OAMH_OAM_COMMUNICATION_INFO_IND to OAMH*/ 
            if (PNULL != p_intrl_msg)
            {
                /*Fill and send message */
                p_oamh_communication_info_ind = (rrc_oamh_communication_info_ind_t *)((U8 *)p_intrl_msg + 
                        RRC_API_HEADER_SIZE);
                p_oamh_communication_info_ind->cell_index = p_cell_ctx->cell_index;
                /* filling MAC comm info */
                ext_comm_info_lower_layer_update(&p_oamh_communication_info_ind->ext_comm_info_lower_layer[RRC_NULL],
                        &p_csc_gl_ctx->p_p_cell_ctx[cell_setup_req.cell_index]->l2_comm_info_params.mac_comm_info);
                p_oamh_communication_info_ind->ext_comm_info_lower_layer[RRC_NULL].ext_module_id = RRC_MAC_MODULE_ID;                 

                /* filling RLC comm info */
                ext_comm_info_lower_layer_update(&p_oamh_communication_info_ind->ext_comm_info_lower_layer[RRC_ONE],
                        &p_csc_gl_ctx->p_p_cell_ctx[cell_setup_req.cell_index]->l2_comm_info_params.rlc_comm_info);
                p_oamh_communication_info_ind->ext_comm_info_lower_layer[RRC_ONE].ext_module_id = RRC_RLC_MODULE_ID;                 

                if(cell_setup_req.cell_config_param.l2_comm_info_params.bitmask & RRC_RRM_NON_CRAN_PDCP_COMM_INFO_PRESENT)
                {
                    /* filling PDCP comm info */
                    ext_comm_info_lower_layer_update(&p_oamh_communication_info_ind->ext_comm_info_lower_layer[RRC_TWO],
                            &p_csc_gl_ctx->p_p_cell_ctx[cell_setup_req.cell_index]->l2_comm_info_params.pdcp_comm_info);
                    p_oamh_communication_info_ind->ext_comm_info_lower_layer[RRC_TWO].ext_module_id = RRC_PDCP_MODULE_ID;                 
                }
                /* filling S1U comm info */
                ext_comm_info_lower_layer_update(&p_oamh_communication_info_ind->ext_comm_info_lower_layer[RRC_THREE],
                        &p_csc_gl_ctx->p_p_cell_ctx[cell_setup_req.cell_index]->l2_comm_info_params.egtpu_comm_info);
                p_oamh_communication_info_ind->ext_comm_info_lower_layer[RRC_THREE].ext_module_id = RRC_S1U_MODULE_ID;                 

                /* filling PHY comm info */
                ext_comm_info_lower_layer_update(&p_oamh_communication_info_ind->ext_comm_info_lower_layer[RRC_FOUR],
                        &p_csc_gl_ctx->p_p_cell_ctx[cell_setup_req.cell_index]->l2_comm_info_params.phy_comm_info);
                p_oamh_communication_info_ind->ext_comm_info_lower_layer[RRC_FOUR].ext_module_id = RRC_PHY_MODULE_ID;                 

                /*SPR 21096 Start*/
                p_oamh_communication_info_ind->l2_instance_id = cell_setup_req.cell_config_param.associated_layer2_inst;
                /*SPR 21096 End*/

                rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
            }
            /* SPR 20908 End */
            result = RRC_SUCCESS;
        }
    } while (0);

    if (RRC_SUCCESS != result)
    {
        /* Send failure response */
        if (RRC_SUCCESS != csc_send_rrm_cell_setup_resp(
                    rrc_get_transaction_id(p_api),
                    cell_setup_req.cell_index,
                    RRC_FAILURE,
                    fail_cause))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                    "Unable to RESP FAILURE to RRM.");
        }
    }

    RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: cell_m_rrm_cell_delete_req_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*
*   DESCRIPTION:
*       This function process the Cell delete Request received from RRM
*       and call FSM.
*
*
******************************************************************************/
void cell_m_rrm_cell_delete_req_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
	rrc_rrm_cell_del_req_t cell_del_req;
	csc_cell_ctx_t *p_cell_ctx  = PNULL;
	S32 length_read             = RRC_NULL;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

	/* Precondition - p_api should point to right message */
	if (RRC_SUCCESS != rrc_il_parse_rrc_rrm_cell_del_req(&cell_del_req,
				(U8* )p_api +
				RRC_FULL_INTERFACE_HEADERS_SIZE,
				rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
				&length_read))
	{
		RRC_CSC_TRACE(RRC_WARNING, "Bad message - ignore.");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

        SET_CELL_INDEX(cell_del_req.cell_index);

	/* spr_11782_fix_start */
	if(PNULL != p_csc_gl_ctx->p_p_cell_ctx)
	{
		p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[cell_del_req.cell_index];
	}
	if((PNULL != p_csc_gl_ctx->p_p_cell_ctx) && (PNULL != p_cell_ctx) &&
			(p_csc_gl_ctx->p_p_cell_ctx[cell_del_req.cell_index]->cell_index == cell_del_req.cell_index))

	{
		/* spr_11782_fix_stop */
		/* SPR 21369 Start */
		if(cell_del_req.bitmask & RRC_RRM_INSTANCE_RESET_FLAG_PRESENT)
		{
			p_cell_ctx->l2_instance_reset_flag = RRC_ONE;
		}
		/* SPR 21369 End */

		/* Normal message processing */
		cell_m_fsm_process_event(CELL_M_EVENT_RRM_DELETE_CELL_REQ,
				p_api,
				&cell_del_req,
				p_cell_ctx,
				p_csc_gl_ctx);
	}
	else
	{
		RRC_CSC_TRACE(RRC_WARNING, "Bad message - ignore.");
	}

	RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: cell_m_s1ap_paging_ind_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*
*   DESCRIPTION:
*       This function fully process S1AP_PAGING_IND message.
*
*
******************************************************************************/
void cell_m_s1ap_paging_ind_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    s1ap_paging_ind_t   *p_s1ap_paging_ind              = PNULL;
    csc_cell_ctx_t      *p_cell_ctx                     = PNULL;
    rrc_counter_t       s1ap_paging_msg_buffer_length   = RRC_NULL;
    U8                  cell_index = 0;
    /* Decode ASN.1 encoded part of message from S1AP */
    OSCTXT      asn1_ctx;
    S1AP_PDU    s1ap_pdu;
    rrc_s1ap_asn_intrl_paging_t s1ap_asn_intrl_paging;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

    /* Precondition - p_api should point to right message */
    p_s1ap_paging_ind = (s1ap_paging_ind_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);
    if (rrc_get_api_buf_size(p_api) < RRC_API_HEADER_SIZE + sizeof
        (s1ap_paging_ind_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    s1ap_paging_msg_buffer_length = (rrc_counter_t)(rrc_get_api_buf_size(p_api) -
            (RRC_API_HEADER_SIZE));

    /* Init ASN.1 context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        /* Ignore message */
        RRC_CSC_TRACE(RRC_ERROR, "Couldn't init ASN.1 context.");
    }
    else
    {
        do
        {
            pu_setBuffer(&asn1_ctx, p_s1ap_paging_ind->paging_msg_buf,
                    s1ap_paging_msg_buffer_length, ASN_S1AP_BUF_TYPE);
            if (RT_OK != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
            {
                /* Ignore message */
                /* I guess we should init Error Indication procedure,
                 *  eNB originated here */
                RRC_CSC_TRACE(RRC_WARNING, "Decoding of ASN.1 S1AP message"
                        " failed. Message ignored.");
                break;
            }

            if (T_S1AP_PDU_initiatingMessage != s1ap_pdu.t)
            {
                /* Ignore message */
                RRC_CSC_TRACE(RRC_WARNING, "Not Initiating Message is "
                        "recieved from S1AP! Current Type of Message == %d. "
                        "Message ignored.",
                        s1ap_pdu.t);
                break;
            }

            /* Check procedure code. In S1AP_PAGING_IND we could get
             *  ONLY S1AP paging message. */
            if (ASN1V_s1ap_id_Paging != s1ap_pdu.u.initiatingMessage->
                    procedureCode)
            {
                /* Ignore message */
                RRC_CSC_TRACE(RRC_WARNING, "In S1AP_PAGING_IND we could "
                        "get ONLY S1AP Paging message. Current Procedure Code "
                        "== %d. Message ignored.",
                        s1ap_pdu.u.initiatingMessage->procedureCode);
                break;
            }

            if (s1ap_ignore != s1ap_pdu.u.initiatingMessage->criticality)
            {
                /* Ignore message */
                RRC_CSC_TRACE(RRC_WARNING, "For S1AP Paging message "
                        "Message Type criticality should be ignore. Current "
                        "criticality == %d. Message ignored.",
                        s1ap_pdu.u.initiatingMessage->criticality);
                break;
            }

            /* Decode S1AP Paging message into appropriate internal ASN.1
             *  based representation */
            if (RRC_SUCCESS != rrc_s1ap_asn_intrl_dec_paging(
                        &s1ap_asn_intrl_paging,
                        s1ap_pdu.u.
                        initiatingMessage->value.u.paging))

            {
                /* Ignore message */
                RRC_CSC_TRACE(RRC_WARNING, "ASN.1 representation of S1AP "
                        "PAGING:PLMN ID Range Invalid"
                        "Message ignored.");
                break;
            }

            /* SPR 1643 Start */
            /* PLMN Identity Validation */
            if (RRC_SUCCESS != validate_paging_plmn_identity(
                        &s1ap_asn_intrl_paging.tai_list))
            {
                /* Ignore message */
                RRC_CSC_TRACE(RRC_WARNING, 
                        "PAGING:PLMN ID Range Invalid"
                        "Message ignored.");
                break;
            }
            /* SPR 1643 Stop*/

            /* Here we have decoded S1AP paging message.
             * Let's start processing */
            for(cell_index = 0;cell_index < MAX_NUM_CELLS; cell_index++)
            {

                if (PNULL != p_csc_gl_ctx->p_p_cell_ctx[cell_index])
                {
                    SET_CELL_INDEX(cell_index);

                    p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[cell_index];

                    /* SPR 22939 fix start */
                    if ((RRC_S1AP_INTRL_PAGING_DRX_PRESENT & s1ap_asn_intrl_paging.bitmask)
                            &&
                        (p_cell_ctx->cfg_cell_param.sib_type_2_Info.
                             radio_resource_config_common_sib.pcch_config.
                             default_paging_cycle > s1ap_asn_intrl_paging.paging_drx))
                    {
                        /* Ignore message */
                        RRC_CSC_TRACE(RRC_WARNING,
                                "Paging DRX of MME is less then eNodeB");
                        break;
                    }
                    /* SPR 22939 fix stop */

                    if (RRC_SUCCESS != csc_paging_process_ue_paging_request(
                                p_cell_ctx,
                                &s1ap_asn_intrl_paging,
                                sfn_m_get_sfn(p_cell_ctx),
                                p_csc_gl_ctx))
                    {
                        /* Ignore message */
                        RRC_CSC_TRACE(RRC_WARNING,
                                "Paging message not successful for %d cell index",cell_index);
                    }
                }
            }

        }
        while(0);

        /* Free ASN.1 context */
        rtFreeContext(&asn1_ctx);
    }


    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: cell_m_llim_config_cell_resp_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*
*   DESCRIPTION:
*       This function partially process CSC_LLIM_CONFIG_CELL_RESP message
*       and call FSM.
*
******************************************************************************/
void cell_m_llim_config_cell_resp_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    rrc_csc_llim_config_cell_resp_t *p_llim_config_cell_resp = PNULL;
    csc_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_csc_llim_config_cell_resp_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    p_llim_config_cell_resp = (rrc_csc_llim_config_cell_resp_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_llim_config_cell_resp->cell_index);

    /* Search in DB cell with p_config_cell_resp->cell_index-should be present*/
    if ( (PNULL == p_csc_gl_ctx->
              p_p_cell_ctx[p_llim_config_cell_resp->cell_index]) || 
         (p_csc_gl_ctx->p_p_cell_ctx[p_llim_config_cell_resp->cell_index]->
              cell_index != p_llim_config_cell_resp->cell_index) )
    {
        /* ignore */
        RRC_CSC_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {

        p_cell_ctx = p_csc_gl_ctx->
            p_p_cell_ctx[p_llim_config_cell_resp->cell_index];

        /* Normal message processing */
        cell_m_fsm_process_event(CELL_M_EVENT_LLIM_CONFIG_CELL_RESP,
                                 p_api,
                                 p_llim_config_cell_resp,
                                 p_cell_ctx,
                                 p_csc_gl_ctx);
    }

    RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
*   FUNCTION NAME: cell_m_llim_reconfig_cell_resp_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*
*   DESCRIPTION:
*       This function partially process CSC_LLIM_CONFIG_CELL_RESP message
*       and call FSM.
*
*
******************************************************************************/
void cell_m_llim_reconfig_cell_resp_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    rrc_csc_llim_reconfig_cell_resp_t *p_llim_reconfig_cell_resp = PNULL;
    csc_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_csc_llim_reconfig_cell_resp_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    p_llim_reconfig_cell_resp = (rrc_csc_llim_reconfig_cell_resp_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_llim_reconfig_cell_resp->cell_index);

    /* Search in DB cell with p_reconfig_cell_resp->cell_index-should be present*/
    if ( (PNULL == p_csc_gl_ctx->
                p_p_cell_ctx[p_llim_reconfig_cell_resp->cell_index]) ||
            (p_csc_gl_ctx->p_p_cell_ctx[p_llim_reconfig_cell_resp->cell_index]->
                cell_index != p_llim_reconfig_cell_resp->cell_index) )
    {
        /* ignore */
        RRC_CSC_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
        p_cell_ctx = p_csc_gl_ctx->
            p_p_cell_ctx[p_llim_reconfig_cell_resp->cell_index];

        /* Normal message processing */
        cell_m_fsm_process_event(CELL_M_EVENT_LLIM_RECONFIG_CELL_RESP,
                                 p_api,
                                 p_llim_reconfig_cell_resp,
                                 p_cell_ctx,
                                 p_csc_gl_ctx);
        
    }
    
    RRC_CSC_UT_TRACE_EXIT();
}


/******************************************************************************
*   FUNCTION NAME: cell_m_llim_delete_cell_resp_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*
*   DESCRIPTION:
*       This function partially process CSC_LLIM_DELETE_CELL_RESP message
*       and call FSM.
******************************************************************************/
void cell_m_llim_delete_cell_resp_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    rrc_csc_llim_delete_cell_resp_t *p_llim_delete_cell_resp = PNULL;
    csc_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_csc_llim_delete_cell_resp_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    p_llim_delete_cell_resp = (rrc_csc_llim_delete_cell_resp_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_llim_delete_cell_resp->cell_index);

    /* Search in DB cell with p_config_cell_resp->cell_index-should be present*/
    if ( (PNULL == p_csc_gl_ctx->
                p_p_cell_ctx[p_llim_delete_cell_resp->cell_index]) ||
            (p_csc_gl_ctx->p_p_cell_ctx[p_llim_delete_cell_resp->cell_index]->
             cell_index != p_llim_delete_cell_resp->cell_index) )
    {
        /* ignore */
        RRC_CSC_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
        p_cell_ctx = p_csc_gl_ctx->
            p_p_cell_ctx[p_llim_delete_cell_resp->cell_index];
        

        /* Normal message processing */
        cell_m_fsm_process_event(CELL_M_EVENT_LLIM_DELETE_CELL_RESP,
                                 p_api,
                                 p_llim_delete_cell_resp,
                                 p_cell_ctx,
                                 p_csc_gl_ctx);
    }

    RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: cell_m_uecc_del_cell_resp_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*
*   DESCRIPTION:
*       This function, process delete all ue resp from UECC,
******************************************************************************/
void cell_m_uecc_del_cell_resp_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
    rrc_csc_uecc_delete_all_ue_resp_t *p_uecc_delete_cell_resp = PNULL;
    csc_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
       (rrc_csc_uecc_delete_all_ue_resp_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    p_uecc_delete_cell_resp = (rrc_csc_uecc_delete_all_ue_resp_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_uecc_delete_cell_resp->cell_index);

    /* Search in DB cell with p_del_cell_resp->cell_index-should be present*/
    if ( (PNULL == p_csc_gl_ctx->
                p_p_cell_ctx[p_uecc_delete_cell_resp->cell_index]) ||
            (p_csc_gl_ctx->p_p_cell_ctx[p_uecc_delete_cell_resp->cell_index]->
                cell_index != p_uecc_delete_cell_resp->cell_index) )
    {
        /* ignore */
        RRC_CSC_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
        p_cell_ctx = p_csc_gl_ctx->
            p_p_cell_ctx[p_uecc_delete_cell_resp->cell_index];

        /* Normal message processing */
        cell_m_fsm_process_event(CELL_M_EVENT_UECC_DELETE_ALL_UE_RESP,
                                 p_api,
                                 p_uecc_delete_cell_resp,
                                 p_cell_ctx,
                                 p_csc_gl_ctx);
    }

}
/******************************************************************************
*   FUNCTION NAME: cell_m_oamh_cleanup_req_process_msg
*   INPUT        : csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*
*   DESCRIPTION:
*       This function, for each cell, partially process cleanup request,
*       call FSM and cleanup Cell DB.
******************************************************************************/
void cell_m_oamh_cleanup_req_process_msg
(
    /* Pointer to the CSC global context data */
    csc_gl_ctx_t    *p_csc_gl_ctx
)
{
    csc_cell_ctx_t *p_cell_ctx = PNULL;
    U8              cell_index;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    
    if( p_csc_gl_ctx->p_p_cell_ctx != PNULL)
    {
        /* For each cell */
        for(cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
        {
            SET_CELL_INDEX(cell_index);

            p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[cell_index];
            if (PNULL != p_cell_ctx)
            {
                /* Process this special message */
                cell_m_fsm_process_event( CELL_M_EVENT_OAMH_CLEANUP_REQ,/*coverity fix start*/
                        ((void *)p_csc_gl_ctx),
                        ((void *)p_csc_gl_ctx),/*coverity fix stop*/
                        p_cell_ctx,
                        p_csc_gl_ctx);
            }
        }
        if(0 == p_csc_gl_ctx->num_allocated_cell_ctx_curr)
        {
            rrc_mem_free(p_csc_gl_ctx->p_p_cell_ctx);
            p_csc_gl_ctx->p_p_cell_ctx = PNULL;
            RRC_CSC_TRACE(RRC_BRIEF, "[CELL_DB] Cleaned up");
        }
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: csc_cell_m_process_timer_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*
*   DeSCRIPTION  : This function partially process Cell manager timer message
*                  and call FSM.
*
******************************************************************************/
void csc_cell_m_process_timer_msg
(
    rrc_timer_t         timer_id,           /* Identity of the expired timer */
    /* Pointer to buffer associated with timer msg */
    csc_timer_buf_t     *p_csc_timer_buf,
    /* Pointer to the CSC global context data */
    csc_gl_ctx_t        *p_csc_gl_ctx
)
{
	csc_cell_ctx_t *p_cell_ctx = PNULL;
	csc_cell_m_timer_buf_t *p_cell_m_timer_buf = (csc_cell_m_timer_buf_t
			*)p_csc_timer_buf;

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_cell_m_timer_buf);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);
	RRC_ASSERT(sizeof(csc_cell_m_timer_buf_t) == sizeof(csc_timer_buf_t) +
			sizeof(rrc_cell_index_t) + sizeof(cell_m_fsm_event_et));
	RRC_ASSERT(CSC_SUBMODULE_CELL_M == p_cell_m_timer_buf->target_submodule);
	RRC_CSC_TRACE(RRC_INFO, "csc_cell_m_process_timer_msg :  timer id:"
            "%p event id: %u,submodule id: %u expired",
			timer_id,p_cell_m_timer_buf->cell_m_event,
			p_csc_timer_buf->target_submodule);

	/* We have only cell specific timer event */
	/* Search in DB cell with p_cell_m_timer_buf->cell_index-should be present*/
	/* spr_11782_fix_start */
	if ( (PNULL == p_csc_gl_ctx->p_p_cell_ctx) || (PNULL == p_csc_gl_ctx->p_p_cell_ctx[p_cell_m_timer_buf->cell_index]) ||
			(p_csc_gl_ctx->p_p_cell_ctx[p_cell_m_timer_buf->cell_index]->
			 cell_index != p_cell_m_timer_buf->cell_index) )
	{
		/* spr_11782_fix_stop */
		/* ignore */
		RRC_CSC_TRACE(RRC_ERROR, "Cell context not found - ignore.");
	}
	else
	{
		p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[p_cell_m_timer_buf->cell_index];
		/* Normal message processing */
		cell_m_fsm_process_event(p_cell_m_timer_buf->cell_m_event,
				timer_id,
				p_cell_m_timer_buf,
				p_cell_ctx,
				p_csc_gl_ctx);
	}

	RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: cell_m_rrm_cell_reconfig_req_process_msg
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
*
*   DESCRIPTION:
*       This function partially process RRC_RRM_CELL_RECONFIG_REQ message
*       and call FSM.
******************************************************************************/
void cell_m_rrm_cell_reconfig_req_process_msg
(
    void            *p_api,         /* Pointer to input API buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
)
{
	static rrc_rrm_cell_reconfig_req_t cell_reconfig_req;
	csc_cell_ctx_t *p_cell_ctx  = PNULL;
	S32 length_read             = RRC_NULL;
	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_csc_gl_ctx);
	RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);
	memset_wrapper(&cell_reconfig_req, 0, sizeof(rrc_rrm_cell_reconfig_req_t));
	/* Precondition - p_api should point to right message */
	/*ALERT -- need to generate parse function again from PUP tool*/
	if (RRC_SUCCESS != rrc_il_parse_rrc_rrm_cell_reconfig_req(   &cell_reconfig_req,
				(U8* )p_api +
				RRC_FULL_INTERFACE_HEADERS_SIZE,
				rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
				&length_read))
	{
		RRC_CSC_TRACE(RRC_WARNING, "Bad message - ignore.");
		RRC_CSC_UT_TRACE_EXIT();
		return;
	}

	/* Search in Global cell context, the existence of cell  - should be Present */
	/* spr_11782_fix_start */
	if (PNULL !=  p_csc_gl_ctx->p_p_cell_ctx)
	{
		p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[cell_reconfig_req.cell_index];
	}

        SET_CELL_INDEX(cell_reconfig_req.cell_index);

	if((PNULL == p_csc_gl_ctx->p_p_cell_ctx) || (PNULL == p_cell_ctx)||
			(p_cell_ctx->cell_index != cell_reconfig_req.cell_index))
	{
		/* spr_11782_fix_stop */
		/* Cell Does not  exist */
		if(RRC_SUCCESS != 
				csc_send_rrm_cell_reconfig_resp_failure( rrc_get_transaction_id(p_api),
					cell_reconfig_req.cell_index,
					RRC_FAILURE,
					RRM_RRC_INTERNAL_FAILURE))
		{
			RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
			RRC_CSC_UT_TRACE_EXIT();
			return;
		}
	}
	else
	{
		/* Normal message processing */
		cell_m_fsm_process_event( CELL_M_EVENT_RRM_RECONFIG_CELL_REQ,
				p_api,
				&cell_reconfig_req,
				p_cell_ctx,
				p_csc_gl_ctx);
	}

	RRC_CSC_UT_TRACE_EXIT();
}

/* SPR 1643 Start */
/****************************************************************************
 * Function Name  : validate_paging_plmn_identity 
 * Inputs         : p_tai_list pointer points to TAI list recived in PAGING.
 * Outputs        : None 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function validates PLMN identiy range
 *                  received in PAGING.
 ****************************************************************************/
static rrc_return_t validate_paging_plmn_identity
(
    s1ap_TAIList            *p_tai_list 
)
{
    OSRTDListNode       *p_tai_list_node                = RRC_NULL;
    s1ap_TAIList_element    *p_tai_list_elem            = RRC_NULL;
    U16 count                                           = 0;

    p_tai_list_node = p_tai_list->head;

    /* TAI List - PLMN Validation */
    for (count =0; 
         count < p_tai_list->count;
         count++)
    {
        RRC_ASSERT(PNULL != p_tai_list_node);
        RRC_ASSERT(PNULL != p_tai_list_node->data);

        p_tai_list_elem = (s1ap_TAIList_element *)p_tai_list_node->data;
        if ((p_tai_list_elem->value.t != T53__TAIItemIEs_1) ||
                (p_tai_list_elem->value.u._TAIItemIEs_1 == PNULL))
        {
            continue;
        }

        /* PLMN ID Octets Validation */
        if (p_tai_list_elem->value.u._TAIItemIEs_1->tAI.pLMNidentity.numocts
                 == MAX_NUM_OCTETS )    
        {
            /* Validation - MCC/MNC and Filler */ 
            if (RRC_FAILURE == 
                    is_valid_plmn_identity(p_tai_list_elem->value.u._TAIItemIEs_1->tAI.
                        pLMNidentity.data))
            {
                RRC_CSC_TRACE(RRC_WARNING,
                               "PLMN ID Range Check Fail for Index [%d]",count);
                return RRC_FAILURE;
            }
        }
        else
        {
            RRC_CSC_TRACE(RRC_WARNING,
                    "PLMN ID : Number of octets not equal to 3 for Index [%d]",
                    count);
            return RRC_FAILURE;
        }
        
       p_tai_list_node = p_tai_list_node->next;
    }
    return RRC_SUCCESS;
}

/* SPR 1643 Stop */


/******************************************************************************
 *   FUNCTION NAME: cell_m_s1ap_pws_req
*   INPUT        : void            *p_api
*                  csc_gl_ctx_t    *p_csc_gl_ctx
*   OUTPUT       : None
*   RETURNS      : None
 *
 *   DESCRIPTION:
 *       This function process S1AP_PWS_REQ message from S1AP
 *       (eNB Manager)and call FSM.
 ******************************************************************************/
void cell_m_s1ap_pws_req
(
 void            *p_api,         /* Pointer to input API buffer */
 csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
 )
{
    s1ap_pws_req_t    *p_s1ap_pws_req = PNULL;
    csc_cell_ctx_t    *p_cell_ctx  = PNULL;
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

    if (rrc_get_api_buf_size(p_api) < RRC_API_HEADER_SIZE + sizeof
            (s1ap_pws_req_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    /* Precondition - p_api should point to right message */
    p_s1ap_pws_req = (s1ap_pws_req_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_s1ap_pws_req->cell_index);

    /* Search in Global cell context, the existence of cell  - should be Present */
    p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[p_s1ap_pws_req->cell_index];
    if( (PNULL == p_cell_ctx)||
            (p_cell_ctx->cell_index != p_s1ap_pws_req->cell_index))
    {
        /* Cell Does not  exist */
        csc_send_s1ap_pws_resp_failure( 
                p_s1ap_pws_req->cell_index,
                p_s1ap_pws_req->transaction_id,
                RRC_FAILURE,
                RRM_RRC_INTERNAL_FAILURE);
    }
    else
    {
        /* Normal message processing */
        cell_m_fsm_process_event( CELL_M_EVENT_ENB_MGR_PWS_REQ,
                p_api,
                p_s1ap_pws_req,
                p_cell_ctx,
                p_csc_gl_ctx);
    }
    RRC_CSC_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: cell_m_rrm_updated_pws_si_list_resp_process_msg
 *   INPUT        : void            *p_api
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *
 *   DESCRIPTION:
 *       This function partially process RRC_RRM_CELL_RECONFIG_REQ message
 *       and call FSM.
 ******************************************************************************/
void cell_m_rrm_updated_pws_si_list_resp_process_msg 
(
 void            *p_api,         /* Pointer to input API buffer */
 csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
 )
{
    rrc_rrm_updated_pws_si_list_resp_t 
        *p_updated_pws_si_list_resp;

    csc_cell_ctx_t *p_cell_ctx  = PNULL;
    S32 length_read             = RRC_NULL;
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

    p_updated_pws_si_list_resp = rrc_mem_get(
            sizeof(rrc_rrm_updated_pws_si_list_resp_t));
    if(PNULL == p_updated_pws_si_list_resp)
    {
        RRC_CSC_TRACE(RRC_FATAL, "Memory Allocation Failure");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }
    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != 
            rrc_il_parse_rrc_rrm_updated_pws_si_list_resp(   
                p_updated_pws_si_list_resp,
                (U8* )p_api +
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Bad message - ignore.");
	/* Coverity Fix 63293 Start */
	if ( PNULL != p_updated_pws_si_list_resp )
	{
	    rrc_mem_free(p_updated_pws_si_list_resp);
	    p_updated_pws_si_list_resp = PNULL;
	}
    	/* Coverity Fix 63293 Stop */
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    /* Search in Global cell context, the existence of cell  - should be Present */
    p_cell_ctx = p_csc_gl_ctx->
        p_p_cell_ctx[p_updated_pws_si_list_resp->cell_index];

    SET_CELL_INDEX(p_updated_pws_si_list_resp->cell_index);

    if (PNULL != p_cell_ctx)
    {
        if (p_cell_ctx->cell_index != p_updated_pws_si_list_resp->cell_index)
        {
            /*Send cnf failure*/
            csc_send_rrm_updated_pws_si_list_cnf(p_cell_ctx, RRC_FAILURE);
      }
        else
        {
            /* Normal message processing */
            cell_m_fsm_process_event( CELL_M_EVENT_RRM_UPDATED_PWS_SI_RESP,
                    p_api,
                    p_updated_pws_si_list_resp,
                    p_cell_ctx,
                    p_csc_gl_ctx);
        }
    }

    /* Coverity Fix 63293 Start */
    if ( PNULL != p_updated_pws_si_list_resp )
    {
        rrc_mem_free(p_updated_pws_si_list_resp);
        p_updated_pws_si_list_resp = PNULL;
    }
    /* Coverity Fix 63293 Stop */
    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_llim_pws_resp_process_msg
 *   INPUT        : void            *p_api
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION:
 *       This function partially process CSC_LLIM_PWS_RESP
 *       message
 *       and call FSM.
 ******************************************************************************/
void cell_m_llim_pws_resp_process_msg
(
 void            *p_api,         /* Pointer to input API buffer */
 csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
 )
{
    rrc_csc_llim_pws_resp_t *p_llim_pws_resp = PNULL;
    csc_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_pws_resp_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    p_llim_pws_resp = (rrc_csc_llim_pws_resp_t*)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_llim_pws_resp->cell_index);

    /* Search in DB cell with p_reconfig_cell_resp->cell_index-should be present*/
    if ( (PNULL == p_csc_gl_ctx->p_p_cell_ctx[p_llim_pws_resp->cell_index]) ||
            (p_csc_gl_ctx->p_p_cell_ctx[p_llim_pws_resp->cell_index]->
             cell_index != p_llim_pws_resp->cell_index) )
    {
        /* ignore */
        RRC_CSC_TRACE(RRC_WARNING, "Cell context not found -"
                "ignore.");
    }
    else
    {
        p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[p_llim_pws_resp->cell_index];

        /* Normal message processing */
        cell_m_fsm_process_event(CELL_M_EVENT_LLIM_PWS_RESP,
                p_api,
                p_llim_pws_resp,
                p_cell_ctx,
                p_csc_gl_ctx);
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_llim_updated_pws_si_resp_process_msg
 *   INPUT        : void            *p_api
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION:
 *       This function partially process
 *       CSC_LLIM_UPDATED_PWS_SI_RESP message
 *       and call FSM.
 ******************************************************************************/
void cell_m_llim_updated_pws_si_resp_process_msg
(
 void            *p_api,         /* Pointer to input API buffer */
 csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
 )
{
  rrc_csc_llim_updated_pws_si_resp_t 
    *p_llim_updated_pws_si_resp = PNULL;

  csc_cell_ctx_t *p_cell_ctx = PNULL;

  RRC_CSC_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_api);
  RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

  /* Precondition - p_api should point to right message */
  if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
          (rrc_csc_llim_updated_pws_si_resp_t))
  {
      RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
      RRC_CSC_UT_TRACE_EXIT();
      return;
  }

  p_llim_updated_pws_si_resp = 
      (rrc_csc_llim_updated_pws_si_resp_t*)
      ((U8* )p_api +
       RRC_API_HEADER_SIZE);

  SET_CELL_INDEX(p_llim_updated_pws_si_resp->cell_index);

  /* Search in DB cell with p_reconfig_cell_resp->cell_index-should be present*/
  if ((PNULL == p_csc_gl_ctx->
          p_p_cell_ctx[p_llim_updated_pws_si_resp->cell_index]) ||
          (p_csc_gl_ctx->p_p_cell_ctx[p_llim_updated_pws_si_resp->cell_index]->
           cell_index != p_llim_updated_pws_si_resp->
           cell_index) )
  {
      /* ignore */
      RRC_CSC_TRACE(RRC_WARNING, "Cell context not found - ignore.");
  }
  else
  {
      p_cell_ctx = p_csc_gl_ctx->
          p_p_cell_ctx[p_llim_updated_pws_si_resp->cell_index];

      if(PNULL != p_cell_ctx)
      {
          /* Normal message processing */
          cell_m_fsm_process_event(   CELL_M_EVENT_LLIM_UPDATED_PWS_SI_RESP,
                  p_api,
                  p_llim_updated_pws_si_resp,
                  p_cell_ctx,
                  p_csc_gl_ctx);
      }
      else
      {    
          /* ignore */
          RRC_CSC_TRACE(RRC_WARNING, "Cell context not exist - ignore.");
      }
  }

  RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_s1ap_pws_failure_req
 *   INPUT        : void            *p_api
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *
 *   DESCRIPTION:
 *       This function process S1AP_PWS_FAILURE_REQ
 *       message from S1AP (eNB Manager)and call FSM.
 ******************************************************************************/
void cell_m_s1ap_pws_failure_req
(
 void            *p_api,         /* Pointer to input API buffer */
 csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
 )
{
  s1ap_pws_failure_req_t *p_s1ap_pws_failure_req = PNULL;
  csc_cell_ctx_t              *p_cell_ctx  = PNULL;
  RRC_CSC_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_api);
  RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

  if (rrc_get_api_buf_size(p_api) < RRC_API_HEADER_SIZE + sizeof
      (s1ap_pws_failure_req_t))
  {
    RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
    RRC_CSC_UT_TRACE_EXIT();
    return;
  }
  
  /* Precondition - p_api should point to right message */
  p_s1ap_pws_failure_req = 
    (s1ap_pws_failure_req_t*)((U8* )p_api + RRC_API_HEADER_SIZE);

  SET_CELL_INDEX(p_s1ap_pws_failure_req->cell_index);

  /* Search in Global cell context, the existence of cell  - should be Present */
  p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[p_s1ap_pws_failure_req->cell_index];
  if( (PNULL == p_cell_ctx)||
      (p_cell_ctx->cell_index != p_s1ap_pws_failure_req->cell_index))
  {
    RRC_CSC_TRACE(RRC_WARNING, "Cell Id doesn't exist.");
  }
  else
  {
    /* Normal message processing */
    cell_m_fsm_process_event( CELL_M_EVENT_ENB_MGR_PWS_FAILURE_REQ,
        p_api,
        p_s1ap_pws_failure_req,
        p_cell_ctx,
        p_csc_gl_ctx);
  }
  RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_s1ap_kill_req
 *   INPUT        : void            *p_api
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *
 *   DESCRIPTION:
 *       This function process S1AP_KILL_REQ message from S1AP
 *       (eNB Manager)and call FSM.
 ******************************************************************************/
void cell_m_s1ap_kill_req
(
 void            *p_api,         /* Pointer to input API buffer */
 csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
 )
{
  s1ap_kill_req_t   *p_s1ap_kill_req = PNULL;
  csc_cell_ctx_t    *p_cell_ctx  = PNULL;
  RRC_CSC_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_api);
  RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

  if (rrc_get_api_buf_size(p_api) < RRC_API_HEADER_SIZE + sizeof
      (s1ap_kill_req_t))
  {
    RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
    RRC_CSC_UT_TRACE_EXIT();
    return;
  }

  /* Precondition - p_api should point to right message */
  p_s1ap_kill_req = (s1ap_kill_req_t *)((U8* )p_api +
      RRC_API_HEADER_SIZE);

  SET_CELL_INDEX(p_s1ap_kill_req->cell_index);

  /* Search in Global cell context, the existence of cell  - should be Present */
  p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[p_s1ap_kill_req->cell_index];
  if( (PNULL == p_cell_ctx)||
      (p_cell_ctx->cell_index != p_s1ap_kill_req->cell_index))
  {
    /* Cell Does not  exist */
    csc_send_s1ap_kill_resp_failure( 
        p_s1ap_kill_req->cell_index,
        p_s1ap_kill_req->transaction_id,
        RRC_FAILURE,
        RRC_RRM_UNKNOWN_FAILURE);
  }
  else
  {
    /* Normal message processing */
    cell_m_fsm_process_event( CELL_M_EVENT_ENB_MGR_KILL_REQ,
        p_api,
        p_s1ap_kill_req,
        p_cell_ctx,
        p_csc_gl_ctx);
  }
  RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_s1ap_kill_failure_ind
 *   INPUT        : void            *p_api
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *
 *   DESCRIPTION:
 *       This function process S1AP_KILL_FAILURE_IND 
 *       message from S1AP (eNB Manager)and call FSM.
 ******************************************************************************/
void cell_m_s1ap_kill_failure_ind
(
 void            *p_api,         /* Pointer to input API buffer */
 csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
 )
{
  s1ap_kill_failure_ind_t *p_s1ap_kill_failure_ind = PNULL;
  csc_cell_ctx_t              *p_cell_ctx  = PNULL;
  RRC_CSC_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_api);
  RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

  if (rrc_get_api_buf_size(p_api) < RRC_API_HEADER_SIZE + sizeof
      (s1ap_kill_failure_ind_t))
  {
    RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
    RRC_CSC_UT_TRACE_EXIT();
    return;
  }
  
  /* Precondition - p_api should point to right message */
  p_s1ap_kill_failure_ind = 
    (s1ap_kill_failure_ind_t*)((U8* )p_api + RRC_API_HEADER_SIZE);

  SET_CELL_INDEX(p_s1ap_kill_failure_ind->cell_index);

  /* Search in Global cell context, the existence of cell  - should be Present */
  p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[p_s1ap_kill_failure_ind->cell_index];
  if( (PNULL == p_cell_ctx)||
      (p_cell_ctx->cell_index != p_s1ap_kill_failure_ind->cell_index))
  {
    RRC_CSC_TRACE(RRC_WARNING, "Cell Id doesn't exist.");
  }
  else
  {
    /* Normal message processing */
    cell_m_fsm_process_event( CELL_M_EVENT_ENB_MGR_KILL_FAILURE_IND,
        p_api,
        p_s1ap_kill_failure_ind,
        p_cell_ctx,
        p_csc_gl_ctx);
  }
  RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: cell_m_x2ap_csc_cell_status_req_process_and_send_resp_msg 
 *   INPUT        : void            *p_api
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *
 *   DESCRIPTION:
 *       This function process X2AP_CSC_CELL_STATUS_REQ message from X2AP
 *       and sends X2AP_CSC_CELL_STATUS_RESP.
 ******************************************************************************/
void cell_m_x2ap_csc_cell_status_req_process_and_send_resp_msg
(
 void            *p_api,         /* Pointer to input API buffer */
 csc_gl_ctx_t    *p_csc_gl_ctx   /* Pointer to the CSC global context data */
 )
{
    x2ap_csc_cell_status_req_t   *p_x2ap_csc_cell_status_req = PNULL;
    x2ap_csc_cell_status_resp_t  *p_x2ap_csc_cell_status_resp = PNULL;
    void *p_intrl_msg = PNULL;
    csc_cell_ctx_t    *p_cell_ctx  = PNULL;
    U16 count = 0;
    U16 index = 0;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

    if (rrc_get_api_buf_size(p_api) < RRC_API_HEADER_SIZE + sizeof
            (x2ap_csc_cell_status_req_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }
    /* Precondition - p_api should point to right message */
    p_x2ap_csc_cell_status_req = (x2ap_csc_cell_status_req_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    /* memory allocation for response msg */
    p_intrl_msg = csc_alloc_intrl_msg(RRC_X2AP_MODULE_ID,
            X2AP_CSC_CELL_STATUS_RESP,
            sizeof(x2ap_csc_cell_status_resp_t));
    
    if(PNULL == p_intrl_msg)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        return;
    }

    if (PNULL == p_intrl_msg )
    {
        RRC_CSC_TRACE(RRC_WARNING, "Memory Allocation failed");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }
    
    p_x2ap_csc_cell_status_resp =
        (x2ap_csc_cell_status_resp_t*)
        ((U8 *)p_intrl_msg +
         RRC_API_HEADER_SIZE);
    
    if ( PNULL != p_x2ap_csc_cell_status_resp )
    {

        p_x2ap_csc_cell_status_resp->num_served_cells 
            = p_x2ap_csc_cell_status_req->num_served_cells;
        p_x2ap_csc_cell_status_resp->no_of_uecc_instances = 
            p_csc_gl_ctx->no_of_uecc_instances;
        /* Search in Global cell context, the existence of cell  - should be Present */
        for ( index = 0; index<p_x2ap_csc_cell_status_req->num_served_cells; index++) 
        {
            SET_CELL_INDEX(p_x2ap_csc_cell_status_req->cell_info[index].cell_index);

            p_x2ap_csc_cell_status_resp->cell_status_info[index].cell_index
                = p_x2ap_csc_cell_status_req->cell_info[index].cell_index;
            /* SPR 19723 Fix Start */
            if (PNULL != p_csc_gl_ctx->p_p_cell_ctx)
            {
            if ( (p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[p_x2ap_csc_cell_status_req->
                        cell_info[index].cell_index]) == PNULL)
            {
                /* Cell Does not  exist */
                RRC_CSC_TRACE(RRC_INFO, "Cell does not exist. %d", 
                        p_x2ap_csc_cell_status_req->cell_info[index].cell_index);

                p_x2ap_csc_cell_status_resp->cell_status_info[index].status 
                    = X2AP_CSC_CELL_DOWN;
            }
            else
            {
                if((p_cell_ctx->cell_index 
                            == p_x2ap_csc_cell_status_req->cell_info[index].cell_index) 
                        && (CELL_M_STATE_CELL_IN_SERVICE 
                            == CELL_M_FSM_GET_CURRENT_STATE(p_cell_ctx)))
                {
                    p_x2ap_csc_cell_status_resp->cell_status_info[count].status 
                        = X2AP_CSC_CELL_UP;
                }
                else
                {
                    p_x2ap_csc_cell_status_resp->cell_status_info[count].status 
                        = X2AP_CSC_CELL_DOWN;
                }
            }
        }
            else
            {
                p_x2ap_csc_cell_status_resp->cell_status_info[count].status 
                    = X2AP_CSC_CELL_DOWN;
            }
            /* SPR 19723 Fix Stop */
        }
    }

    RRC_CSC_TRACE(RRC_DETAILED, "Sending CSC Cell Status Resp to X2AP");
    rrc_send_message(p_intrl_msg,
            RRC_X2AP_MODULE_ID);
    RRC_CSC_UT_TRACE_EXIT();
}


/*******************************************************************************
 *  FUNCTION NAME : cell_m_rrm_cell_start_req_process_msg
 *   INPUT        : void            *p_api
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *
 *  DESCRIPTION : This function process RRC_RRM_CELL_START_REQ message and 
 *                call FSM. 
 *******************************************************************************/ 
void cell_m_rrm_cell_start_req_process_msg
(
    void            *p_api,          /* pointer to the input api buffer */
    csc_gl_ctx_t    *p_csc_gl_ctx    /* pointer to the CSC global context data */
)
{
    rrc_rrm_cell_start_req_t    cell_start_req;
    csc_cell_ctx_t              *p_cell_ctx       = PNULL;
    S32 length_read         = RRC_NULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);


    /* prepare the message */

    if (RRC_SUCCESS != rrc_il_parse_rrc_rrm_cell_start_req(
                &cell_start_req,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))      

    {    
        RRC_CSC_TRACE(RRC_ERROR, "Bad message - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        /* Drop message */
        return;
    }    

    SET_CELL_INDEX(cell_start_req.cell_index);

    /* Search in Global cell context, the existence of cell  - should be Present */
    p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[cell_start_req.cell_index];
    if( (PNULL == p_cell_ctx)||
        (p_cell_ctx->cell_index != cell_start_req.cell_index))
    {
        /* Cell Does not  exist */
        if(RRC_SUCCESS != csc_send_rrm_cell_start_resp( 
                    rrc_get_transaction_id(p_api),
                    cell_start_req.cell_index,
                    RRC_FAILURE,
                    RRM_RRC_INTERNAL_FAILURE))
         {
             RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
             RRC_CSC_UT_TRACE_EXIT();
             return;
          }
    }     
    else
    {
        /* Normal message processing */
        cell_m_fsm_process_event( CELL_M_EVENT_RRM_CELL_START_REQ,
                                  p_api,
                                  &cell_start_req,
                                  p_cell_ctx,
                                  p_csc_gl_ctx);
    }                           
    
    RRC_CSC_UT_TRACE_EXIT();

}

/*******************************************************************************
 *
 *  FUNCTION NAME : cell_m_rrm_cell_stop_req_process_msg
 *   INPUT        : void            *p_api
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *  
 *  DESCRIPTION   : This function process RRC_RRM_CELL_STOP_REQ message and 
 *                call FSM. 
 ******************************************************************************/
void cell_m_rrm_cell_stop_req_process_msg
(
     void            *p_api,          /* pointer to the input api buffer */
     csc_gl_ctx_t    *p_csc_gl_ctx    /* pointer to the CSC global context data */
)
{
     rrc_rrm_cell_stop_req_t    cell_stop_req;
     csc_cell_ctx_t             *p_cell_ctx      = PNULL;
     S32 length_read         = RRC_NULL;

     RRC_CSC_UT_TRACE_ENTER();

     RRC_ASSERT(PNULL != p_api);
     RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);


     /* prepare the message */

     if (RRC_SUCCESS != rrc_il_parse_rrc_rrm_cell_stop_req(
                 &cell_stop_req,
                 (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                 rrc_get_api_buf_size(p_api) -
                 RRC_FULL_INTERFACE_HEADERS_SIZE,
                 &length_read))

     {
         RRC_CSC_TRACE(RRC_ERROR, "Bad message - ignore.");
         RRC_CSC_UT_TRACE_EXIT();
         /* Drop message */
         return;
     } 

    SET_CELL_INDEX(cell_stop_req.cell_index);

    /* Search in Global cell context, the existence of cell  - should be Present */
    p_cell_ctx = p_csc_gl_ctx->p_p_cell_ctx[cell_stop_req.cell_index];
    if( (PNULL == p_cell_ctx)||
        (p_cell_ctx->cell_index != cell_stop_req.cell_index))
    {
        /* Cell Does not  exist */
        if(RRC_SUCCESS != csc_send_rrm_cell_stop_resp( rrc_get_transaction_id(p_api),
                    cell_stop_req.cell_index,
                    RRC_FAILURE,
                    RRM_RRC_INTERNAL_FAILURE))
        {
            RRC_CSC_TRACE(RRC_WARNING, "Unable to send RESP FAILURE TO RRM");
            RRC_CSC_UT_TRACE_EXIT();
            return;
        }
    }     
    else
    {
        /* Normal message processing */
        cell_m_fsm_process_event( CELL_M_EVENT_RRM_CELL_STOP_REQ,
                                  p_api,
                                  &cell_stop_req,
                                  p_cell_ctx,
                                  p_csc_gl_ctx);
    }                           

    RRC_CSC_UT_TRACE_EXIT();
 }
/*******************************************************************************
 *  FUNCTION NAME : cell_m_llim_cell_start_resp_process_msg
 *   INPUT        : void            *p_api
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *    
 *    DESCRIPTION : This function process CSC_LLIM_CELL_START_RESP message 
 *******************************************************************************/
void cell_m_llim_cell_start_resp_process_msg
(
  void            *p_api,           /* pointer to the inpu api buffer */
  csc_gl_ctx_t    *p_csc_gl_ctx    /* pointer to teh CSC global context data */
)
{
    rrc_csc_llim_cell_start_resp_t *p_llim_cell_start_resp = PNULL;
    csc_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_cell_start_resp_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    p_llim_cell_start_resp = (rrc_csc_llim_cell_start_resp_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_llim_cell_start_resp->cell_index);

    /* Search in DB cell with p_config_cell_resp->cell_index-should be present*/
    if ( (PNULL == p_csc_gl_ctx->
            p_p_cell_ctx[p_llim_cell_start_resp->cell_index]) ||
         (p_csc_gl_ctx->p_p_cell_ctx[p_llim_cell_start_resp->cell_index]->
            cell_index != p_llim_cell_start_resp->cell_index) )
    {
        /* ignore */
        RRC_CSC_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
        p_cell_ctx = p_csc_gl_ctx->
            p_p_cell_ctx[p_llim_cell_start_resp->cell_index];

        /* Normal message processing */
        cell_m_fsm_process_event(CELL_M_EVENT_LLIM_CELL_START_RESP,
                p_api,
                p_llim_cell_start_resp,
                p_cell_ctx,
                p_csc_gl_ctx);
    }

    RRC_CSC_UT_TRACE_EXIT();
}

/*******************************************************************************
 *   FUNCTION NAME: cell_m_llim_cell_stop_resp_process_msg
 *   INPUT        : void            *p_api
 *                  csc_gl_ctx_t    *p_csc_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function process CSC_LLIM_CELL_STOP_RESP message
 *******************************************************************************/
void cell_m_llim_cell_stop_resp_process_msg
(
  void            *p_api,           /* pointer to the inpu api buffer */
  csc_gl_ctx_t    *p_csc_gl_ctx    /* pointer to teh CSC global context data */
)
{
    rrc_csc_llim_cell_stop_resp_t *p_llim_cell_stop_resp = PNULL;
    csc_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_csc_gl_ctx);
    RRC_ASSERT(PNULL != p_csc_gl_ctx->p_p_cell_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_cell_stop_resp_t))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    p_llim_cell_stop_resp = (rrc_csc_llim_cell_stop_resp_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_llim_cell_stop_resp->cell_index);

    /* Search in DB cell with p_config_cell_resp->cell_index-should be present*/
    if ( (PNULL == p_csc_gl_ctx->
            p_p_cell_ctx[p_llim_cell_stop_resp->cell_index]) ||
         (p_csc_gl_ctx->p_p_cell_ctx[p_llim_cell_stop_resp->cell_index]->
            cell_index != p_llim_cell_stop_resp->cell_index) )
    {
        /* ignore */
        RRC_CSC_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
        p_cell_ctx = p_csc_gl_ctx->
            p_p_cell_ctx[p_llim_cell_stop_resp->cell_index];
        /* Normal message processing */
        cell_m_fsm_process_event(CELL_M_EVENT_LLIM_CELL_STOP_RESP,
                p_api,
                p_llim_cell_stop_resp,
                p_cell_ctx,
                p_csc_gl_ctx);
    }

    RRC_CSC_UT_TRACE_EXIT();
}

