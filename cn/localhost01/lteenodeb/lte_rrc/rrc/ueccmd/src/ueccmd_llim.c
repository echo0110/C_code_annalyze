/******************************************************************************
*
*   FILE NAME:
*       ueccmd_llim.c
*
*   DESCRIPTION:
*       This is the source file of the UECC MD LLIM .
*       UECC MD LLIM interracts with RRC LLIM module.
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_defines.h"
#include "ueccmd_oamh.h"
#include "rrc_intrl_api.h"
#include "rrc_oamh_intf.h"
#include "rrc_msg_mgmt.h"
#include "ueccmd_utils.h"
#include "ueccmd_logger.h"
#include "rrc_common_utils.h"
#include "rrc_handlers.h"
#include "rrc_uecc_llim_intf.h"
#include "ueccmd_llim.h"
#include "rrc_logging.h"
#include "uecc_utils.h"
#include "rrc_logging.h"

/* Re-establish Fix Start */
static rrc_return_et uecc_fill_ue_conn_reestab_info(
        rrc_uecc_llim_ccch_data_ind_t  *p_ccch_data_ind,
        ueccmd_ue_conn_reestab_node_t  *ue_conn_reestab_node);
/*SPR_18530_Fix_Start*/
static rrc_return_et ueccmd_llim_build_and_send_rrc_connection_reestablishment_reject 
(
    void  *p_api
);
/*SPR_18530_Fix_Ends*/
/* Re-establish Fix Stop */


/******************************************************************************
*   FUNCTION NAME: partial_decode_uecc_llim_ccch_data_ind
*   INPUT        : U8                *msg_type
*                  void              *p_api
*   OUTPUT       : none
*   RETURNS      : rrc_return_et
*   DESCRIPTION  : This function checks the message type received in
*                  UECC_LLIM_CCCH_DATA_IND message
******************************************************************************/
rrc_return_et partial_decode_uecc_llim_ccch_data_ind
(
    U8                *msg_type,        /* Destination */
    void              *p_api            /* Pointer to input API buffer */
)
{
    OSCTXT                          asn1_ctx;
    OSUINT32                        ui;
    S32                             stat = RRC_NULL;
    rrc_uecc_llim_ccch_data_ind_t   *p_ccch_data_ind = PNULL;
    rrc_return_et                   ret_val = RRC_SUCCESS;

    RRC_UECCMD_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( RT_OK != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_UECCMD_TRACE(RRC_ERROR,
                "partial_decode_uecc_llim_ccch_data_ind:"
                "ASN context initialization failed.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    /*get pointer on LLIM message body*/
    p_ccch_data_ind =
        (rrc_uecc_llim_ccch_data_ind_t *)((U8*)p_api + RRC_API_HEADER_SIZE);

    pu_setBuffer(&asn1_ctx, p_ccch_data_ind->data_buffer,
            p_ccch_data_ind->data_buffer_length, ASN_RRC_BUF_TYPE);

    stat = pd_ConsUnsigned (&asn1_ctx, &ui, 0, OSUINTCONST(1));

    if (stat != RRC_NULL)
    {
        /* Free ASN Context */
        rtFreeContext(&asn1_ctx);
        return RRC_FAILURE;
    }
    switch (ui)
    {
        /* c1 */
        case 0:
            stat = pd_ConsUnsigned (&asn1_ctx, &ui, 0, OSUINTCONST(1));
            if (stat != RRC_NULL)
            {
               ret_val =  RRC_FAILURE;
               break;
            }
            switch (ui) 
            {
                /* rrcConnectionReestablishmentRequest */
                case 0:
                    *msg_type = T_UL_CCCH_MessageType_c1_rrcConnectionReestablishmentRequest;
                    break;

                /* rrcConnectionRequest */
                case 1:
                    *msg_type = T_UL_CCCH_MessageType_c1_rrcConnectionRequest;
                    break;

                default:
                    ret_val = RRC_FAILURE;
            }
            break;
        default:
            ret_val = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UECCMD_UT_TRACE_EXIT();
    return (rrc_return_et)ret_val;
}

/* Re-establish Fix Start */
/******************************************************************************
*   FUNCTION NAME: uecc_fill_ue_conn_reestab_info
*   INPUT        : rrc_uecc_llim_ccch_data_ind_t  *p_ccch_data_ind
*                  ueccmd_ue_conn_reestab_node_t  *ue_conn_reestab_node
*   OUTPUT       : none
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*   DESCRIPTION  : This Function fill the re_establishment info required for rlf_ind
******************************************************************************/
rrc_return_et uecc_fill_ue_conn_reestab_info(
        rrc_uecc_llim_ccch_data_ind_t  *p_ccch_data_ind,
        ueccmd_ue_conn_reestab_node_t  *ue_conn_reestab_node)
{
    UL_CCCH_Message                 ul_ccch_message;
    OSCTXT                          asn1_ctx;
    RRCConnectionReestablishmentRequest_r8_IEs *p_reestab_r8 = PNULL;
    rrc_return_et result = RRC_SUCCESS;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(p_ccch_data_ind != PNULL);
    RRC_ASSERT(ue_conn_reestab_node != PNULL);

    /* Decode CCCH data from ASN1*/
    if ( RT_OK != rtInitContext(&asn1_ctx) )
    {
        RRC_UECCMD_TRACE(RRC_ERROR, "ASN context initialization failed.");
        result = RRC_FAILURE;
        return result;
    }
    do
    {
        pu_setBuffer(&asn1_ctx, p_ccch_data_ind->data_buffer,
                 p_ccch_data_ind->data_buffer_length, ASN_RRC_BUF_TYPE);

        if ( RT_OK != asn1PD_UL_CCCH_Message(&asn1_ctx, &ul_ccch_message) )
        {
            RRC_UECCMD_TRACE(RRC_ERROR, "ASN decode failed.");
            result = RRC_FAILURE;
            break;
        }

        if(PNULL == ul_ccch_message.message.u.c1)
        {
            RRC_UECCMD_TRACE(RRC_ERROR, "UL_CCCH_MessageType invalid field value.");
            result = RRC_FAILURE;
            break;
        }
        switch (ul_ccch_message.message.u.c1->t)
        {
            case T_UL_CCCH_MessageType_c1_rrcConnectionReestablishmentRequest:
                if ((PNULL == ul_ccch_message.message.u.c1->
                                u.rrcConnectionReestablishmentRequest) ||
                    (T_RRCConnectionReestablishmentRequest_criticalExtensions_rrcConnectionReestablishmentRequest_r8
                    != ul_ccch_message.message.u.c1->u.rrcConnectionReestablishmentRequest->
                        criticalExtensions.t) ||
                    (PNULL == ul_ccch_message.message.u.c1->u.rrcConnectionReestablishmentRequest->
                                criticalExtensions.u.rrcConnectionReestablishmentRequest_r8) )
                {
                    RRC_UECCMD_TRACE(RRC_ERROR, "rrcConnectionReestablishmentRequest invalid field value.");
                    result = RRC_FAILURE;
                    break;
                }

                p_reestab_r8 = ul_ccch_message.message.u.c1->u.rrcConnectionReestablishmentRequest->
                               criticalExtensions.u.rrcConnectionReestablishmentRequest_r8;
                memset_wrapper(&ue_conn_reestab_node->ue_reestab_info, 0, 
                            sizeof(ueccmd_ue_reestab_info_t));

                ue_conn_reestab_node->ue_reestab_info.cell_index = 
                            p_ccch_data_ind->cell_index;

                rrc_unpack_U16(&ue_conn_reestab_node->ue_reestab_info.crnti,
                               p_reestab_r8->ue_Identity.c_RNTI.data);

                ue_conn_reestab_node->ue_reestab_info.pci = 
                            p_reestab_r8->ue_Identity.physCellId;

                rrc_unpack_U16(&ue_conn_reestab_node->ue_reestab_info.short_mac_i,
                                p_reestab_r8->ue_Identity.shortMAC_I.data);

                break;

            default:
                RRC_UECCMD_TRACE(RRC_INFO, "Not rrcConnectionReestablishmentRequest msg.");
                result = RRC_FAILURE;
                break;
        }
    }while(0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UECCMD_UT_TRACE_EXIT();

    return result;
}
/* Re-establish Fix Stop */

/******************************************************************************
*   FUNCTION NAME: ueccmd_insert_conn_reestab_node
*   INPUT        : ueccmd_gl_ctx_t    *p_ueccmd_gl_ctx
*                  void               *p_api
*   OUTPUT       : none
*   RETURNS      : none
*   DESCRIPTION  : This Function insert the node in ueccmd_ue_conn_reestab_search_tree
******************************************************************************/
void ueccmd_insert_conn_reestab_node
( 
    ueccmd_gl_ctx_t    *p_ueccmd_gl_ctx,
    void               *p_api           /* Pointer to input API buffer */
)
{
    ueccmd_ue_conn_reestab_node_t       *ue_conn_reestab_node = PNULL;
    rrc_uecc_llim_ccch_data_ind_t       *p_ccch_data_ind = PNULL;
    /*SPR_17727_START*/
    U32                                 rnti = RRC_NULL;
    /*SPR_17727_END*/

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(p_api != PNULL);
    RRC_ASSERT(p_ueccmd_gl_ctx != PNULL);

    /*get pointer on LLIM message body*/
    p_ccch_data_ind =
        (rrc_uecc_llim_ccch_data_ind_t *)((U8*)p_api + RRC_API_HEADER_SIZE);
    /*SPR_17727_START*/
    rnti = ((((U32)(p_ccch_data_ind->cell_index)) << 16) & 0x00FF0000) | p_ccch_data_ind->rnti;
    /*SPR_17727_END*/
    SET_CELL_INDEX(p_ccch_data_ind->cell_index);


   /* SPR 20138 Fix Start */
   RRC_UECCMD_TRACE(RRC_INFO, "Re-establishment request received for RNTI=%d", rnti);
   /* SPR 20138 Fix Stop */
    /*Check whether ue context release already ongoing for received UE */
    ue_conn_reestab_node = (ueccmd_ue_conn_reestab_node_t *)(st_find_tree_node( (const search_tree_t *)
                &(p_ueccmd_gl_ctx->ueccmd_ue_conn_reestab_search_tree),
                (const void*)&rnti));

    if ( PNULL != ue_conn_reestab_node )
    {
       /* SPR 20138 Fix Start */
       /* Duplicate Node Found. Removing old node and creating new */
       RRC_UECCMD_TRACE(RRC_INFO, "Re-establishment ongoing for RNTI=%d", rnti);
       st_delete_node (&(p_ueccmd_gl_ctx->ueccmd_ue_conn_reestab_search_tree),
                &(ue_conn_reestab_node->anchor));

       rrc_mem_free((void *)ue_conn_reestab_node);
       ue_conn_reestab_node = PNULL;
       /* SPR 20138 Fix Stop */
    }
    /* SPR 19416 Fix Start */
    else
    {
        /* If node does not exists then insert node in search tree */
        ue_conn_reestab_node = (ueccmd_ue_conn_reestab_node_t*)rrc_mem_get(
                sizeof(ueccmd_ue_conn_reestab_node_t));

        if ( PNULL == ue_conn_reestab_node )
        {
            RRC_UECCMD_TRACE(RRC_WARNING, "Unable to allocate Memory");
            RRC_UECCMD_UT_TRACE_EXIT();
            return;
        }

        memset_wrapper(ue_conn_reestab_node, RRC_NULL, sizeof(ueccmd_ue_conn_reestab_node_t));

        ue_conn_reestab_node->rnti = rnti;
        ue_conn_reestab_node->num_resp_recv  = RRC_NULL;

        /* Re-establish Fix Start */
        if (RRC_FAILURE == uecc_fill_ue_conn_reestab_info
                (p_ccch_data_ind, ue_conn_reestab_node))
        {
            RRC_UECCMD_TRACE(RRC_WARNING, "Unable to decode Re-establishment message");
            rrc_mem_free(ue_conn_reestab_node);
            RRC_UECCMD_UT_TRACE_EXIT();
            return;
        }
        /* Re-establish Fix Stop */
        memset_wrapper(ue_conn_reestab_node->uecc_resp, RRC_NULL, 
                sizeof(U8)*MAX_UECC_INSTANCES);

        st_insert_node (&(p_ueccmd_gl_ctx->ueccmd_ue_conn_reestab_search_tree),
                &(ue_conn_reestab_node->anchor));
        /*SPR_17727_START*/
        RRC_UECCMD_TRACE(RRC_INFO, "Created reestb node for cell_index: %u and rnti: %u", 
                p_ccch_data_ind->cell_index, p_ccch_data_ind->rnti);
        /*SPR_17727_END*/
    }
    /* SPR 19416 Fix Stop */

    ueccmd_broadcast_msg(p_api, p_ueccmd_gl_ctx);

    RRC_UECCMD_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: ueccmd_uecc_llim_ccch_data_ind_process_msg
*   INPUT        : ueccmd_gl_ctx_t    *p_ueccmd_gl_ctx
*                  void               *p_api
*   OUTPUT       : none
*   RETURNS      : none
*
*   DESCRIPTION  : This function processes UECC_LLIM_CCCH_DATA_IND message
******************************************************************************/
void ueccmd_uecc_llim_ccch_data_ind_process_msg
(
    void            *p_api,          /* Pointer to input API buffer */
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
)
{
    rrc_return_et                   ret_code = RRC_SUCCESS;
    U8                              msg_type = RRC_NULL;
    rrc_uecc_llim_ccch_data_ind_t   *pdata = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);
    
    ret_code = partial_decode_uecc_llim_ccch_data_ind(&msg_type, p_api);

    if ( ret_code != RRC_SUCCESS )
    {
        RRC_UECCMD_TRACE(RRC_WARNING,
                "uecc_llim_ccch_data_ind_process_msg: ASN decode failed.");
	
/*SPR_18530_Fix_Start*/
	/* send UECC_LLIM_UE_CON_RE_ESTABLISH_REJ_REQ
         * (rrcConnectionReestablishmentReject) message to LLIM */ 
        if (RRC_SUCCESS !=
             ueccmd_llim_build_and_send_rrc_connection_reestablishment_reject(
                p_api))
        {
            RRC_UECCMD_TRACE(RRC_ERROR,
                  "Fail to send the RRCConnectionReestablishmentReject to UE");
        }
/*SPR_18530_Fix_Ends*/
        return;
    }

    pdata = (rrc_uecc_llim_ccch_data_ind_t *)((U8*)p_api + RRC_API_HEADER_SIZE);

    if ( msg_type == T_UL_CCCH_MessageType_c1_rrcConnectionRequest)
    {
        RRC_UECCMD_TRACE(RRC_BRIEF, "RRC_MSG: RRC_CONNECTION_REQUEST %s",
            change_data_to_str(pdata->data_buffer_length, pdata->data_buffer));
        /* Forward the message to particuar UECC based on round robin
         * algo */
        ueccmd_round_robin_ue_allocation_to_uecc(p_api, p_ueccmd_gl_ctx); 
    }
    else if ( msg_type == T_UL_CCCH_MessageType_c1_rrcConnectionReestablishmentRequest)
    {
        RRC_UECCMD_TRACE(RRC_BRIEF, "RRC_MSG: RRC_CONNECTION_REESTABLISHMENT_REQUEST %s",
            change_data_to_str(pdata->data_buffer_length, pdata->data_buffer));
        /*Check whether rrc connection reestab ongoing for requested RNTI
         * If not then Broadcasting the message to all UECC as UE Id 
         * not present in the message */
        ueccmd_insert_conn_reestab_node(p_ueccmd_gl_ctx,p_api);
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}
/*SPR_18530_Fix_Start*/
/******************************************************************************
*   FUNCTION NAME: 
*                ueccmd_llim_build_and_send_rrc_connection_reestablishment_reject
*   INPUTS      :  void                *p_api
*   OUTPUTS     : None
*   DESCRIPTION:
*       This function builds and sends RRC connection reestablishment reject
*       message to LLIM module within 
*       UECC_LLIM_UE_CON_RE_ESTABLISH_REJ_REQ message.
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et ueccmd_llim_build_and_send_rrc_connection_reestablishment_reject 
(
    void              *p_api            /* Pointer to input API buffer */
)
{
    U8  msg[sizeof(rrc_uecc_llim_con_re_establish_rej_req_t)+UECC_MAX_ASN1_BUF_LEN];
    rrc_uecc_llim_con_re_establish_rej_req_t*  
                                      p_llim_con_re_establish_rej_req = PNULL;
    U32                                         encoded_msg_len = 0;
    OSCTXT                                      asn1_ctx;
    DL_CCCH_Message                             dl_ccch_msg;
    DL_CCCH_MessageType_c1*                     p_dl_ccch_c1 = PNULL;
    RRCConnectionReestablishmentReject_r8_IEs*  
                                 p_rrc_con_reestablishment_reject_r8_ies = PNULL;
    rrc_uecc_llim_ccch_data_ind_t   *p_ccch_data_ind = PNULL;
    rrc_return_et                               response = RRC_FAILURE;
    RRC_UECCMD_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( RT_OK != rtInitContext(&asn1_ctx) )
    {
        RRC_UECCMD_TRACE(RRC_ERROR,
            "[RRCConnectionReestablishmentReject]"
            " ASN context initialization failed.");
        return RRC_FAILURE;
    }

    /*get pointer on LLIM message body*/
    p_ccch_data_ind =
        (rrc_uecc_llim_ccch_data_ind_t *)((U8*)p_api + RRC_API_HEADER_SIZE);

    do {
        /* Fill the values in the ASN structures that shall be encoded
         * by ASN Encoder */
        /* Set Downlink CCCH message type */
        asn1Init_DL_CCCH_MessageType(&(dl_ccch_msg.message));

        dl_ccch_msg.message.t = T_DL_CCCH_MessageType_c1;

        /* Allocate and choose RRCConnectionReject */
        p_dl_ccch_c1 = rtxMemAllocType(&asn1_ctx, DL_CCCH_MessageType_c1);
        if (PNULL == p_dl_ccch_c1)
        {
            RRC_UECCMD_TRACE(RRC_ERROR,
                "[RRCConnectionReestablishmentReject] ASN malloc failed.");
            break;
        }

        /* Intialize */
        asn1Init_DL_CCCH_MessageType_c1(p_dl_ccch_c1);
        dl_ccch_msg.message.u.c1 = p_dl_ccch_c1;

        p_dl_ccch_c1->t = 
            T_DL_CCCH_MessageType_c1_rrcConnectionReestablishmentReject;
        p_dl_ccch_c1->u.rrcConnectionReestablishmentReject =
            rtxMemAllocType(&asn1_ctx, RRCConnectionReestablishmentReject);
        if (PNULL == p_dl_ccch_c1->u.rrcConnectionReestablishmentReject)
        {
            RRC_UECCMD_TRACE(RRC_ERROR,
                "[RRCConnectionReestablishmentReject] ASN malloc failed.");
            break;
        }

        asn1Init_RRCConnectionReestablishmentReject
            (p_dl_ccch_c1->u.rrcConnectionReestablishmentReject);

        p_dl_ccch_c1->u.rrcConnectionReestablishmentReject->criticalExtensions.t =
            T_RRCConnectionReestablishmentReject_criticalExtensions_rrcConnectionReestablishmentReject_r8;
        p_rrc_con_reestablishment_reject_r8_ies = rtxMemAllocType(&asn1_ctx,
                RRCConnectionReestablishmentReject_r8_IEs);
        if (PNULL == p_rrc_con_reestablishment_reject_r8_ies)
        {
            RRC_UECCMD_TRACE(RRC_ERROR,
                "[RRCConnectionReestablishmentReject] ASN malloc failed.");
            break;
        }
        asn1Init_RRCConnectionReestablishmentReject_r8_IEs(
            p_rrc_con_reestablishment_reject_r8_ies);

        p_dl_ccch_c1->u.rrcConnectionReestablishmentReject->criticalExtensions.u.
            rrcConnectionReestablishmentReject_r8 = 
                p_rrc_con_reestablishment_reject_r8_ies;


        /* Prepare pointer on RRC internal message */
        p_llim_con_re_establish_rej_req = (rrc_uecc_llim_con_re_establish_rej_req_t*)msg;

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_llim_con_re_establish_rej_req->data_buffer,
            UECC_MAX_ASN1_BUF_LEN, ASN_RRC_BUF_TYPE);
        encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);

        if (RT_OK != asn1PE_DL_CCCH_Message(&asn1_ctx, &dl_ccch_msg))
        {
            RRC_UECCMD_TRACE(RRC_ERROR,
                "[RRCConnectionReestablishmentReject] ASN1 encoding failed.");
            break;
        }
        else
        {
            RRC_UECCMD_TRACE(RRC_INFO, "RRC_MSG: RRCConnectionReestablishmentReject %s",
                change_data_to_str(p_llim_con_re_establish_rej_req->data_buffer_length, p_llim_con_re_establish_rej_req->data_buffer));
            rrc_asn1PrtToStr_DL_CCCH_Message(RRC_ASN,"DL_CCCH_Message",
                    &dl_ccch_msg,"RRCConnectionReestablishmentReject",
                    p_llim_con_re_establish_rej_req->data_buffer,encoded_msg_len);
        }
        /* Prepare LLIM mesage */
        p_llim_con_re_establish_rej_req->data_buffer_length = 
            (rrc_counter_t)pe_GetMsgLen(&asn1_ctx);
        p_llim_con_re_establish_rej_req->crnti = p_ccch_data_ind->rnti;
        p_llim_con_re_establish_rej_req->cell_index = p_ccch_data_ind->cell_index;
        /* Send message to LLIM  */
        response = uecc_rrc_send_internal_msg(
                RRC_LLIM_MODULE_ID,
                UECC_LLIM_UE_CON_RE_ESTABLISH_REJ_REQ,
                (U16)(sizeof(rrc_uecc_llim_con_re_establish_rej_req_t) +
                    p_llim_con_re_establish_rej_req->data_buffer_length),
                msg);

    } while (0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);
    RRC_UECCMD_UT_TRACE_EXIT();
    return response;
}
/*SPR_18530_Fix_Ends*/
