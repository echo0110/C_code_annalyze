/******************************************************************************
*
*   FILE NAME:
*       ueccmd_md.c
*
*   DESCRIPTION:
*       This is the main file of the UECC MD module. This module provides
*       the main message handler and timer expiry handler functions of
*       RRC UECC MD module.
*
*   Copyright (c) 2012, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_defines.h"
#include "ueccmd_logger.h"
#include "rrc_common_utils.h"
#include "rrc_common_md.h"
#include "ueccmd_timers.h"
#include "ueccmd_global_ctx.h"
#include "rrc_msg_mgmt.h"

#include "rrc_ext_api.h"
#include "rrc_intrl_api.h"
#include "rrc_handlers.h"
#include "rrc_s1ap_uecc_intf.h"
#include "rrc_cp_common.h"
#include "ueccmd_utils.h"
#include "ueccmd_oamh.h"
#include "rrc_x2ap_uecc_intf.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "ueccmd_uecc.h"
#include "ueccmd_llim.h"
#include "rrc_rrm_intf.h"

/*
*   Global variables
*/

/* Used in logger for identifying UECC MD (just for avoid string duplication) */
const S8* rrc_ueccmd_log_facility_name = (const S8*)"eNodeB RRC UECCMD";


/******************************************************************************
*   FUNCTION NAME: ueccmd_rrm_process_msg
*   INPUT        : void             *p_api
*                  ueccmd_gl_ctx_t  *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : This function passes RRM messages to UECC MD
*                  and check incoming API id. This function partially parse the message
*                  to get the UE Index so that UECC MD forwards the message to particular
*                  UECC instance.
******************************************************************************/
static void ueccmd_rrm_process_msg
(
    void             *p_api,          /* Pointer to input API buffer */
    ueccmd_gl_ctx_t  *p_ueccmd_gl_ctx /* Pointer to the UECCMD global context */
)
{
    U16    ue_index = 0;
    U8     length = 0; 
    void   *p_intrl_msg = PNULL;
    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
       
        case RRC_RRM_ERB_RELEASE_IND:
        case RRC_RRM_UE_RELEASE_RESP:
        case RRC_RRM_HO_CANCEL_REQ:
        case RRC_RRM_UE_HO_CMD_RESP:
        case RRC_RRM_MEAS_CONFIG_REQ:
                length = RRC_FULL_INTERFACE_HEADERS_SIZE; 
                rrc_cp_unpack_U16(&ue_index, (U8 *)p_api + length, "ue_index");
               
                SET_UE_INDEX(ue_index);

                p_intrl_msg = (void *)ueccmd_alloc_intrl_msg(RRC_NULL,
                        RRC_NULL,
                        RRC_NULL, rrc_get_api_buf_size(p_api));

                if (PNULL != p_intrl_msg)
                {
                    l3_memcpy_wrapper(p_intrl_msg, p_api, rrc_get_api_buf_size(p_api));

                    if (p_ueccmd_gl_ctx->num_uecc_instances > RRC_GET_UECC_MODULE_INDEX(ue_index))
                    {
/* BUG 9135 Fix Start */
                        rrc_construct_api_header ((U8 *)p_intrl_msg, RRC_VERSION_ID, RRC_RRM_MODULE_ID, 
                                     RRC_GET_UECC_MODULE_ID(ue_index), rrc_get_api_id(p_api), 
                                     rrc_get_api_buf_size(p_api));
/* BUG 9135 Fix Stop */
                        rrc_send_message (p_intrl_msg, RRC_GET_UECC_MODULE_ID(ue_index)); 
                    }
                    else
                    {  
                        /* Changing ue_index to 0xFFFF */
                        ue_index = RRC_INVALID_UE_INDEX;
                        rrc_cp_pack_U16((U8 *)p_api + length, &ue_index, "ue_index");

                        SET_UE_INDEX(ue_index);

                        l3_memcpy_wrapper(p_intrl_msg, p_api, rrc_get_api_buf_size(p_api));

                        RRC_UECCMD_TRACE(RRC_WARNING, "ueccmd_rrm_process_msg: Invalid UECC Id ");
/* BUG 9135 Fix Start */
                        rrc_construct_api_header ((U8 *)p_intrl_msg, RRC_VERSION_ID, RRC_RRM_MODULE_ID, 
                                     RRC_UECC_1_MODULE_ID, rrc_get_api_id(p_api), 
                                     rrc_get_api_buf_size(p_api));
/* BUG 9135 Fix Stop */
                        rrc_send_message (p_intrl_msg, RRC_UECC_1_MODULE_ID);
                    }
                }
            break;
        case RRC_RRM_UE_ADMISSION_RESP:
        case RRC_RRM_ERB_SETUP_RESP:
        case RRC_RRM_ERB_MODIFY_RESP:
        case RRC_RRM_UE_CONNECTION_RELEASE_IND:
        case RRC_RRM_UE_CAPABILITY_ENQUIRY_REQ:
        case RRC_RRM_ERB_RELEASE_RESP:
        case RRC_RRM_HO_REQUIRED:
        case RRC_RRM_UE_HO_ADM_RESP:
        case RRC_RRM_UE_CONTEXT_MOD_RESP:
        case RRC_RRM_UE_RECONFIG_REQ:
        case RRC_RRM_CSFB_PARAMETERS_RESP_CDMA2000:
        case RRC_RRM_COUNTER_CHECK_REQ:
        case RRC_RRM_UE_INFORMATION_REQ:
/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
        case RRC_RRM_DC_BEARER_CHANGE_REQ:
#endif
/* OPTION3X Changes End */

                 length = RRC_FULL_INTERFACE_HEADERS_SIZE +
                     sizeof(rrc_bitmask_t);
                 rrc_cp_unpack_U16(&ue_index, (U8 *)p_api + length, "ue_index");

                 SET_UE_INDEX(ue_index);

                 p_intrl_msg = (void *)ueccmd_alloc_intrl_msg(RRC_NULL,
                        RRC_NULL,
                        RRC_NULL, rrc_get_api_buf_size(p_api));

                 if (PNULL != p_intrl_msg)
                 {
                     l3_memcpy_wrapper(p_intrl_msg, p_api, rrc_get_api_buf_size(p_api));

                     if (p_ueccmd_gl_ctx->num_uecc_instances > RRC_GET_UECC_MODULE_INDEX(ue_index))
                     {
/* BUG 9135 Fix Start */
                        rrc_construct_api_header ((U8 *)p_intrl_msg, RRC_VERSION_ID, RRC_RRM_MODULE_ID, 
                                     RRC_GET_UECC_MODULE_ID(ue_index), rrc_get_api_id(p_api), 
                                     rrc_get_api_buf_size(p_api));
/* BUG 9135 Fix Stop */
                         rrc_send_message (p_intrl_msg, RRC_GET_UECC_MODULE_ID(ue_index)); 
                     }
                     else
                     {   
                         /* Changing ue_index to 0xFFFF */
                         ue_index = RRC_INVALID_UE_INDEX;
                         rrc_cp_pack_U16((U8 *)p_api + length, &ue_index, "ue_index");

                         SET_UE_INDEX(ue_index);

                         l3_memcpy_wrapper(p_intrl_msg, p_api, rrc_get_api_buf_size(p_api));

                         RRC_UECCMD_TRACE(RRC_WARNING, "ueccmd_rrm_process_msg: Invalid UECC Id ");
/* BUG 9135 Fix Start */
                        rrc_construct_api_header ((U8 *)p_intrl_msg, RRC_VERSION_ID, RRC_RRM_MODULE_ID, 
                                     RRC_UECC_1_MODULE_ID, rrc_get_api_id(p_api), 
                                     rrc_get_api_buf_size(p_api));
/* BUG 9135 Fix Stop */
                         rrc_send_message (p_intrl_msg, RRC_UECC_1_MODULE_ID);
                     }
                 }
             break;
        default:
            RRC_UECCMD_TRACE(RRC_WARNING, "ueccmd_rrm_process_msg: unexpected API from "
                    "RRM module received");
            break;
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: ueccmd_insert_ue_ctx_release_node
*   INPUT        : void             *p_api
*                  ueccmd_gl_ctx_t  *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : This Function insert the node in ueccmd_ue_ctx_search_tree.
******************************************************************************/
static void ueccmd_insert_ue_ctx_release_node
( 
    ueccmd_gl_ctx_t    *p_ueccmd_gl_ctx,
    void               *p_api           /* Pointer to input API buffer */
)
{
    ueccmd_ue_ctx_release_node_t   *ue_ctx_release_node = PNULL;
    s1ap_ue_associated_sig_msg_ind_t    *p_sig_msg_ind = PNULL;
    U32                                 mme_ue_s1apid = RRC_NULL;
    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(p_api != PNULL);
    RRC_ASSERT(p_ueccmd_gl_ctx != PNULL);

    p_sig_msg_ind = (s1ap_ue_associated_sig_msg_ind_t *)((U8*)p_api +
                                RRC_API_HEADER_SIZE);

    mme_ue_s1apid = p_sig_msg_ind->mme_ue_s1apid;

    /*Check whether ue context release already ongoing for received UE */
   ue_ctx_release_node = (ueccmd_ue_ctx_release_node_t *)(st_find_tree_node( (const search_tree_t *)
                &(p_ueccmd_gl_ctx->ueccmd_ue_ctx_search_tree),
                (const void*)&mme_ue_s1apid));

   if ( ue_ctx_release_node != PNULL )
   {
       RRC_UECCMD_TRACE(RRC_WARNING, "UE Context release is already ongoing for %d .. dropping ", 
               mme_ue_s1apid);
       RRC_UECCMD_UT_TRACE_EXIT();
       return;
   }

   /* If node does not exists then insert node in search tree */
   ue_ctx_release_node = (ueccmd_ue_ctx_release_node_t*)rrc_mem_get(
           sizeof(ueccmd_ue_ctx_release_node_t));

   if (PNULL == ue_ctx_release_node)
   {
       RRC_UECCMD_TRACE(RRC_WARNING, "Unable to allocate memory.");
       RRC_UECCMD_UT_TRACE_EXIT();
       return;
   }

   memset_wrapper(ue_ctx_release_node, RRC_NULL, sizeof(ueccmd_ue_ctx_release_node_t));

   ue_ctx_release_node->mme_ue_id = mme_ue_s1apid;
   ue_ctx_release_node->num_resp_recv = RRC_NULL;

   memset_wrapper(ue_ctx_release_node->uecc_resp, RRC_NULL, sizeof(U8)*MAX_UECC_INSTANCES);

   st_insert_node (&(p_ueccmd_gl_ctx->ueccmd_ue_ctx_search_tree),
           &(ue_ctx_release_node->anchor));

   ueccmd_broadcast_msg(p_api, p_ueccmd_gl_ctx);

   RRC_UECCMD_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: ueccmd_s1ap_process_msg
*   INPUT        : void             *p_api
*                  ueccmd_gl_ctx_t  *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : This function passes S1AP messages to UECC MD Module
*                  and check incoming API id.
******************************************************************************/
static void ueccmd_s1ap_process_msg
(
    void            *p_api,           /* Pointer to input API buffer */
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx  /* Pointer to the UECC MD global context */
)
{


    s1ap_ue_associated_sig_msg_ind_t    *p_sig_msg_ind = PNULL;
    s1ap_status_msg_ind_t               *p_status_msg_ind = PNULL;
    s1ap_ueccmd_uecc_info_t             *p_uecc_info = PNULL;
    void                                *p_intrl_msg = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
        case S1AP_UE_ASSOCIATED_SIG_MSG_IND:

            p_sig_msg_ind = (s1ap_ue_associated_sig_msg_ind_t *)((U8*)p_api + 
                    RRC_API_HEADER_SIZE);
              
            SET_UE_INDEX(p_sig_msg_ind->ue_index);

            if((p_sig_msg_ind->s1ap_procedure_code == 
                    ASN1V_s1ap_id_HandoverResourceAllocation)&&
                    (p_sig_msg_ind->message_type == INITIATING_MESSAGE))
            {
                ueccmd_round_robin_ue_allocation_to_uecc(p_api, p_ueccmd_gl_ctx);
            }
            else
            {
                if ((p_sig_msg_ind->s1ap_procedure_code == 
                        ASN1V_s1ap_id_UEContextRelease)&&
                        (p_sig_msg_ind->message_type == INITIATING_MESSAGE))
                {
                    ueccmd_insert_ue_ctx_release_node(p_ueccmd_gl_ctx, p_api);
                }

                else if ((p_sig_msg_ind->s1ap_procedure_code == 
                        ASN1V_s1ap_id_ErrorIndication)&&
                        (p_sig_msg_ind->message_type == INITIATING_MESSAGE)) 
                {
                    ueccmd_broadcast_msg(p_api, p_ueccmd_gl_ctx);
                }
            }
            break;
        case S1AP_RESET_MSG_IND:
        case S1AP_STATUS_MSG_IND:
            /* Provide no of UECC instances to S1AP if S1ap STATUS is success */

            p_status_msg_ind = (s1ap_status_msg_ind_t *)((U8*)p_api + 
                    RRC_API_HEADER_SIZE);
            if (p_status_msg_ind->status == RRC_TRUE)
            {
                /* Send wake up indication to OAMH */
                p_intrl_msg = (void *)ueccmd_alloc_intrl_msg(RRC_UECCMD_MODULE_ID,
                        RRC_S1AP_MODULE_ID,
                        S1AP_UECCMD_UECC_INFO, sizeof(s1ap_ueccmd_uecc_info_t));

                if (PNULL != p_intrl_msg)
                {
                    p_uecc_info = (s1ap_ueccmd_uecc_info_t *)((U8*)p_intrl_msg + 
                            RRC_API_HEADER_SIZE);

                    p_uecc_info->num_uecc_instances =
                        p_ueccmd_gl_ctx->num_uecc_instances;
                    p_uecc_info->total_ue_supported =
                        p_ueccmd_gl_ctx->total_ue_supported;

                    /* Fill and send message */
                    rrc_send_message(p_intrl_msg, RRC_S1AP_MODULE_ID);
                }
            }

            ueccmd_broadcast_msg(p_api, p_ueccmd_gl_ctx);
            break;

        default:
            RRC_UECCMD_TRACE(RRC_WARNING, "ueccmd_s1ap_process_msg: unexpected "
                    "API from S1AP module received");
            break;
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: ueccmd_x2ap_process_msg
*   INPUT        : void             *p_api
*                  ueccmd_gl_ctx_t  *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : This function passes X2AP messages to UECCMD module
*                  and check incoming API id.
******************************************************************************/
static void ueccmd_x2ap_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECCMD global context */
 )
{
    x2ap_ue_associated_sig_msg_ind_t    *p_sig_msg_ind = PNULL;
    /* CSR_00040799 Fix Start */
    x2ap_status_msg_ind_t               *p_status_msg_ind = PNULL;
    x2ap_ueccmd_ue_info_t               *p_uecc_info = PNULL;
    void                                *p_intrl_msg = PNULL;
    /* CSR_00040799 Fix End */
    
    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
        
        case X2AP_UE_ASSOCIATED_SIG_MSG_IND:

            p_sig_msg_ind = (x2ap_ue_associated_sig_msg_ind_t *)
                ((U8*)p_api + RRC_API_HEADER_SIZE);

            if ( (p_sig_msg_ind->x2ap_procedure_code == 
                    ASN1V_x2ap_id_handoverPreparation) ||
                (p_sig_msg_ind->x2ap_procedure_code ==
                     RRC_INTER_CELL_HO_PROC_CODE) 
                /*BUG 604 changes start*/
                || (p_sig_msg_ind->x2ap_procedure_code ==
                    RRC_INTRA_CELL_HO_PROC_CODE ))
                /*BUG 604 changes stop*/ 
            {
                ueccmd_round_robin_ue_allocation_to_uecc(p_api, p_ueccmd_gl_ctx);
            }
            else
            {
            
                if ( (p_sig_msg_ind->x2ap_procedure_code == 
                        ASN1V_x2ap_id_handoverCancel) ||
                     (p_sig_msg_ind->x2ap_procedure_code ==
                        RRC_INTER_CELL_HO_CANCEL_PROC_CODE) )
                {    
                    ueccmd_broadcast_msg(p_api, p_ueccmd_gl_ctx);
                }
            }
                
            
            break;

        case X2AP_STATUS_MSG_IND:
    /* CSR_00040799 Fix Start */
            p_status_msg_ind = (x2ap_status_msg_ind_t *)((U8*)p_api + 
                    RRC_API_HEADER_SIZE);
            if ( X2AP_LINK_UP == p_status_msg_ind->connection_status )
            {
                /* Send wake up indication to OAMH */
                p_intrl_msg = (void *)ueccmd_alloc_intrl_msg(RRC_UECCMD_MODULE_ID,
                        RRC_X2AP_MODULE_ID,
                        X2AP_UECCMD_UE_INFO, sizeof(x2ap_ueccmd_ue_info_t));

                if (PNULL != p_intrl_msg)
                {
                    p_uecc_info = (x2ap_ueccmd_ue_info_t *)((U8*)p_intrl_msg + 
                            RRC_API_HEADER_SIZE);

                    p_uecc_info->total_ue_supported =
                        p_ueccmd_gl_ctx->total_ue_supported;
                    
                    p_uecc_info->peer_enodeb_id =
                        p_status_msg_ind->peer_enodeb_id ;
                    /* Fill and send message */
                    rrc_send_message(p_intrl_msg, RRC_X2AP_MODULE_ID);
                }
            }

        /* CSR_00040799 Fix End */
        case X2AP_RESET_MSG_IND:
        case X2AP_UE_RLF_MSG_IND:
        /* Bug 4691 Changes Start */
        case X2AP_UPDATE_NBOUR_INFO:
        case X2AP_UPDATE_NR_NBOUR_INFO:

            RRC_UECCMD_TRACE(RRC_INFO, "ueccmd_x2ap_process_msg: API Received is %u \n",
            rrc_get_api_id(p_api));
        /* Bug 4691 Changes End */
	
            ueccmd_broadcast_msg(p_api, p_ueccmd_gl_ctx);
            break;

        default:
        RRC_UECCMD_TRACE(RRC_WARNING, "ueccmd_x2ap_process_msg: unexpected API from "
                "X2AP module received");
        break;
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: ueccmd_oamh_process_msg
*   INPUT        : void             *p_api
*                  ueccmd_gl_ctx_t  *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : This function passes OAMH messages to UECCMD module
*                  and check incoming API id.
******************************************************************************/
static void ueccmd_oamh_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECCMD global context */
 )
{
    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
        case UECCMD_OAMH_INIT_CNF:
            ueccmd_oamh_init_cnf_process_msg (p_api, p_ueccmd_gl_ctx);
            break;

        case UECCMD_OAMH_PROV_REQ:
            ueccmd_oamh_prov_req_process_msg (p_api, p_ueccmd_gl_ctx);
            break;

        case UECC_OAMH_GET_CELL_STATUS_REQ:
            ueccmd_check_cell_state_and_broadcast_msg(p_ueccmd_gl_ctx, p_api,
                    UECCMD_GET_CELL_STATUS_PROC_RUNNING);
            break;

        case UECCMD_OAMH_CLEANUP_REQ:
            ueccmd_oamh_cleanup_req_process_msg(p_api, p_ueccmd_gl_ctx);
            break;

        case UECC_OAMH_CELL_TRAFFIC_TRACE_START:
            ueccmd_process_traffic_trace_start_msg(p_ueccmd_gl_ctx, p_api);
            break;

        case UECC_OAMH_CELL_TRAFFIC_TRACE_STOP:
            ueccmd_broadcast_msg (p_api, p_ueccmd_gl_ctx);
            break;

        case RRC_OAM_TRACE_FILE_TRANSFER_COMPLETE_IND:
            ueccmd_broadcast_msg (p_api, p_ueccmd_gl_ctx);
            break;

        case UECC_OAMH_GET_UE_STATUS_REQ:
            ueccmd_check_cell_state_and_broadcast_msg(p_ueccmd_gl_ctx, p_api,
                    UECCMD_GET_UE_STATUS_PROC_RUNNING); 
            break;
       /*lipa start*/
        case UECCMD_OAMH_ADD_DEL_LGW_IND:
             ueccmd_process_add_del_lgw_ind_msg(p_ueccmd_gl_ctx, p_api);
             break;
        /*lipa end*/
        default:
            RRC_UECCMD_TRACE(RRC_WARNING, "ueccmd_oamh_process_msg: unexpected API from "
                    "OAMH module received");
            break;
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: ueccmd_llim_process_msg
*   INPUT        : void             *p_api
*                  ueccmd_gl_ctx_t  *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : This function passes LLIM messages to UECCMD module and
*                  check incoming API id.
******************************************************************************/
static void ueccmd_llim_process_msg
(
    void            *p_api,          /* Pointer to input API buffer */
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECCMD global context */
)
{
    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
    case UECC_LLIM_CCCH_DATA_IND:
        ueccmd_uecc_llim_ccch_data_ind_process_msg (p_api, p_ueccmd_gl_ctx);
        break;
    case UECC_LLIM_S1U_PATH_FAILURE_IND:
    case UECC_LLIM_S1U_PATH_SUCCESS_IND:
         ueccmd_broadcast_msg (p_api, p_ueccmd_gl_ctx);
         break;

    default:
        RRC_UECCMD_TRACE(RRC_WARNING, "ueccmd_llim_process_msg: unexpected API from "
            "LLIM module received");
        break;
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: ueccmd_uecc_process_msg
*   INPUT        : void             *p_api
*                  ueccmd_gl_ctx_t  *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : This function passes UECC messages to UECC MD module and
*                  check incoming API id.
******************************************************************************/
static void ueccmd_uecc_process_msg
(
    void            *p_api,          /* Pointer to input API buffer */
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECCMD global context */
)

{
    s1ap_ue_associated_sig_msg_req_t    *p_sig_msg_req = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
        case UECC_OAMH_PROV_RESP:
            ueccmd_uecc_oamh_prov_resp_process_msg (p_api, p_ueccmd_gl_ctx);
            break;

        case UECC_CSC_DELETE_ALL_UE_RESP:
             ueccmd_uecc_csc_delete_all_ue_resp_process_msg(
                     p_api, p_ueccmd_gl_ctx);
             break;
        case UECC_OAMH_GET_CELL_STATUS_RESP:
            ueccmd_uecc_oamh_get_cell_status_resp_process_msg (
                    p_api, p_ueccmd_gl_ctx);
            break;

/*SPR_17727_START*/
        case UECCMD_UE_CON_RE_ESTABLISH_ONGOING:
            ueccmd_uecc_ue_con_re_establish_process_msg (p_api, p_ueccmd_gl_ctx);
            break;
/*SPR_17727_END*/
        
        case UECC_LLIM_UE_CON_RE_ESTABLISH_REJ_REQ:
            ueccmd_uecc_llim_ue_con_re_establish_rej_req_process_msg (
                    p_api, p_ueccmd_gl_ctx);
            break;

        case UECCMD_OAMH_CLEANUP_RESP:
            ueccmd_oamh_cleanup_resp_process_msg(p_api, p_ueccmd_gl_ctx);
            break;

        case UECC_OAMH_CELL_TRAFFIC_TRACE_START_ERROR_IND:
             ueccmd_oamh_cell_traffic_trace_start_error_ind_process_msg(
                    p_api, p_ueccmd_gl_ctx);
            break;

        case UECC_OAMH_GET_UE_STATUS_RESP:
             ueccmd_oamh_get_ue_status_resp_process_msg(p_api, p_ueccmd_gl_ctx);
             break;
        
        case S1AP_UE_ASSOCIATED_SIG_MSG_REQ:
             
             p_sig_msg_req = (s1ap_ue_associated_sig_msg_req_t *)((U8*)p_api + 
                    RRC_API_HEADER_SIZE);
              
             if (p_sig_msg_req->procedure_code == 
                   ASN1V_s1ap_id_ErrorIndication)
             {
                 ueccmd_s1ap_ue_associated_sig_msg_req_process_msg(
                     p_api, p_ueccmd_gl_ctx);
             }
             break;

        case UECCMD_UE_CONTEXT_RELEASE_ONGOING_IND:

             ueccmd_ue_context_release_ind_process_msg(
                     p_api, p_ueccmd_gl_ctx);
             break;

        case UECCMD_UECC_PROC_STATUS_IND:

             ueccmd_uecc_proc_status_ind_process_msg(
                   p_api, p_ueccmd_gl_ctx);
             break;
        default:
            RRC_UECCMD_TRACE(RRC_WARNING, "ueccmd_uecc_process_msg: unexpected API from "
                    "UECC module received");
            break;
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: ueccmd_csc_process_msg
*   INPUT        : void             *p_api
*                  ueccmd_gl_ctx_t  *p_ueccmd_gl_ctx
*   OUTPUT       : none
*   RETURNS      : None
*   DESCRIPTION  : This function passes CSC messages to UECC MD module and
*                  check incoming API id.
******************************************************************************/
static void ueccmd_csc_process_msg
(
    void            *p_api,           /* Pointer to input API buffer */
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx  /* Pointer to the UECCMD global context */
)

{
    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    switch (rrc_get_api_id(p_api))
    {
        case CSC_UECC_INIT_SETUP_IND:
        case CSC_UECC_CELL_RECONFIG_IND:
        case CSC_UECC_CELL_START_IND:
        case CSC_UECC_CELL_STOP_IND:
            ueccmd_broadcast_msg (p_api, p_ueccmd_gl_ctx);
            break;
        case CSC_UECC_DELETE_ALL_UE_REQ:
            ueccmd_check_cell_state_and_broadcast_msg(p_ueccmd_gl_ctx, p_api,
                    UECCMD_DELETE_ALL_UE_PROC_RUNNING);
            break;
        default:
            RRC_UECCMD_TRACE(RRC_WARNING, "ueccmd_csc_process_msg: unexpected API from "
                    "CSC module received");
            break;
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}


/******************************************************************************
*   FUNCTION NAME: rrc_ueccmd_process_msg
*   INPUT        : void *p_api
*                  void *p_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This is the main entry point of RRC UECC MD module - CSPL callback. It
*                  passes all the external APIs received from exetrnal entities RRM
*                  and internal RRC entities CSC, OAMH, LLIM, UECC to appropriate
*                  internal submodule UECC MD based on incoming module ID and API ID.
*                  This function always returns nonzero value, i.e. the buffer is always
*                  released by the encapsulating shell.
*
*   RETURNS      : RRC_BUFFER_SHOULD_BE_RELEASED - Encapsulating shell should release
*                  the buffer
*
******************************************************************************/
S32 rrc_ueccmd_process_msg
(
 void *p_api,        /* Pointer to input API buffer */
 void *p_gl_ctx      /* Pointer to UECCMD global context */
 )
{
    ueccmd_gl_ctx_t *p_ueccmd_gl_ctx = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    /* SPR 19273 Fix Start */
    SET_CELL_AND_UE_INDEX(0xff, 0xffff);
    /* SPR 19273 Fix End */

    /* Precondition check */
    /* Check general message integrity too */
    if ( (PNULL == p_api) || (PNULL == p_gl_ctx) )
    {
        RRC_UECCMD_TRACE(RRC_ERROR, "rrc_ueccmd_process_msg: somebody call us in "
                "wrong context");
        RRC_UECCMD_UT_TRACE_EXIT();
        /* Encapsulating shell should release the buffer */
        return RRC_BUFFER_SHOULD_BE_RELEASED;
    }

    if (RRC_SUCCESS != rrc_check_cspl_header(p_api))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "rrc_ueccmd_process_msg: bad message header -"
                " ignore msg");
        RRC_UECCMD_UT_TRACE_EXIT();
        /* Encapsulating shell should release the buffer */
        return RRC_BUFFER_SHOULD_BE_RELEASED;
    }

    p_ueccmd_gl_ctx = (ueccmd_gl_ctx_t *)p_gl_ctx;

    switch (rrc_get_src_module_id(p_api))
    {
        case RRC_RRM_MODULE_ID:
        ueccmd_rrm_process_msg(p_api, p_ueccmd_gl_ctx);
        break;

        case RRC_S1AP_MODULE_ID:
        ueccmd_s1ap_process_msg(p_api, p_ueccmd_gl_ctx);
        break;

        case RRC_OAMH_MODULE_ID:
        ueccmd_oamh_process_msg(p_api, p_ueccmd_gl_ctx);
        break;

        case RRC_LLIM_MODULE_ID:
        ueccmd_llim_process_msg(p_api, p_ueccmd_gl_ctx);
        break;

        case RRC_UECC_MODULE_ID:
        case RRC_UECC_1_MODULE_ID:
        case RRC_UECC_2_MODULE_ID:
        case RRC_UECC_3_MODULE_ID:
        case RRC_UECC_4_MODULE_ID:
        case RRC_UECC_5_MODULE_ID:
        case RRC_UECC_6_MODULE_ID:
        case RRC_UECC_7_MODULE_ID:
        case RRC_UECC_8_MODULE_ID:
        ueccmd_uecc_process_msg(p_api, p_ueccmd_gl_ctx);
        break;

        case RRC_X2AP_MODULE_ID:
        ueccmd_x2ap_process_msg(p_api, p_ueccmd_gl_ctx);
        break;

        case RRC_CSC_MODULE_ID:
        ueccmd_csc_process_msg(p_api, p_ueccmd_gl_ctx);
        break;
        default:
        RRC_UECCMD_TRACE(RRC_WARNING, "rrc_ueccmd_process_msg: API from unexpected "
                "module received");
        break;
    }

    RRC_UECCMD_UT_TRACE_EXIT();

    /* Encapsulating shell should release the buffer */
    return RRC_BUFFER_SHOULD_BE_RELEASED;
}

/******************************************************************************
*   FUNCTION NAME: rrc_ueccmd_process_timer_msg
*   INPUT        : rrc_timer_t timer_id
*                  void        *p_timer_buf
*                  void        *p_gl_ctx
*   OUTPUT       : none
*   RETURNS      : none
*   DESCRIPTION  : This is the main entry point of RRC UECC MD module - CSPL callback.
*                  It handles all the timer expiry events.
*                  The timer buffer is always allocated in the rrc_start_timer() function.
*                  This buffer should always start with ueccmd_timer_buf_t
*                  Timer buffer can be freed depending on information in ueccmd_timer_buf_t.
******************************************************************************/
void rrc_ueccmd_process_timer_msg
(
    rrc_timer_t timer_id,       /* Identity of the expired timer */
    void        *p_timer_buf,   /* Pointer to data associated with the timer */
    void        *p_gl_ctx       /* Pointer to the UECCMD global context */
)
{
    ueccmd_gl_ctx_t    *p_ueccmd_gl_ctx = PNULL;
    ueccmd_timer_buf_t *p_ueccmd_timer_buf = PNULL;
    void *p_intrl_msg = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    /* Precondition check */
    if ((PNULL == p_timer_buf) || (PNULL == p_gl_ctx))
    {
        RRC_UECCMD_TRACE(RRC_ERROR, "rrc_ueccmd_process_timer_msg: somebody call us "
            "in wrong context");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    p_ueccmd_gl_ctx = (ueccmd_gl_ctx_t *)p_gl_ctx;
    p_ueccmd_timer_buf = (ueccmd_timer_buf_t *)p_timer_buf;

    /* Currently we have only one timer for OAMH */
    if (timer_id == p_ueccmd_gl_ctx->oamh_init_ind_timer)
    {
        /* Send wake up indication to OAMH */
        p_intrl_msg = (void *)ueccmd_alloc_intrl_msg(RRC_UECCMD_MODULE_ID,
                RRC_OAMH_MODULE_ID,
                UECCMD_OAMH_INIT_IND, 0);

        if (PNULL != p_intrl_msg)
        {
            /* Fill and send message */
            rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
        }

        RRC_UECCMD_TRACE(RRC_INFO, "Init indication sent to OAMH...");
    }
    else
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Unknown timer id [%p] received", timer_id);
    }
    
    if (RRC_TRUE == p_ueccmd_timer_buf->auto_delete)
    {
        if (PNULL != p_timer_buf)
        {
            rrc_mem_free(p_timer_buf);
	    /* + Coverity 81539 */
            //p_timer_buf = PNULL;
	    /* - Coverity 81539 */
        }
    }
    RRC_UECCMD_UT_TRACE_EXIT();
}

