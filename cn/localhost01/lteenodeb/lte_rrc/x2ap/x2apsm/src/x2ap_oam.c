/*******************************************************************************
 *
 *  FILE NAME   : x2ap_oam.c
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
#include "rrc_x2apOam_il_parser.h"
#include "rrc_x2apOam_il_composer.h"
#include "x2ap_csc.h"
#include "rrc_handlers.h"
#include "rrc_x2apCommon_intf.h"
#include "x2ap_tracing.h"

//#ifdef ARICENT_SCTP_IPR
#if 0
#include "sctp_ipr.h"
#include "s_stats.h"
#include "s_mibdef.h"
#endif
//#else
#include "sctp_kernel.h"
//#endif

#include <lteMisc.h>

#ifdef LINUX_PC_TEST
#include "x2ap_test.h"

extern X2AP_SCTP_SETTING_S g_x2ap_sctp_setting;

#endif

/*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL
QLOCK x2ap_lock;
#endif
/*Lockless Implementation stop*/


/* GDB Fix - compiler warning */
extern void x2ap_populate_configuration_parameters(x2ap_conf_param_t* p_tgt, x2ap_conf_param_t* p_src);

void x2ap_process_oam_set_log_level_req(
                 void                   *p_api,
                 x2ap_gb_context_t *p_x2ap_gb_ctx);

void x2ap_process_oam_get_log_level_req(
                 void                   *p_api,
                 x2ap_gb_context_t *p_x2ap_gb_ctx);

void x2ap_process_oam_log_enable_req(
                 void                   *p_api,
                 x2ap_gb_context_t *p_x2ap_gb_ctx);

U8 get_oam_log_level_from_x2ap_log_level
(
    U8 x2ap_log_level 
    );

void x2ap_process_oam_prov_req(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);

extern void x2ap_popu_sctp_conf_info_from_gb_db(
        x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info,
        x2ap_sctp_conf_info_t      *p_x2ap_sctp_conf_info);


/* SPR-16334 START */
extern void x2ap_process_enb_config_update_req(
        void              *p_api,
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        U16               dest_module_id);
/* SPR-16334 END */



x2ap_return_et  x2ap_create_server(
        x2ap_gb_context_t *p_x2ap_gb_ctx);

void    x2ap_invoke_sctp_sm(
        x2ap_gb_context_t* p_x2ap_gb_ctx);

void    x2ap_oam_populate_served_cell_info(
        x2ap_gb_context_t        *p_x2ap_gb_ctx, 
        x2ap_cell_info_t         *p_x2ap_gb_ctxt_cell_info,
        x2apOam_srvd_cell_info_t *p_x2ap_oam_cell_info,
        U8                       cell_id);
void    x2ap_upd_neighbour_cell_info(
        x2ap_gb_context_t        *p_x2ap_gb_ctx,
        x2ap_neighbour_info_t    *p_neighbour_cell_info,
        U8                        served_cell_id);
void    x2ap_process_oam_cleanup_req(
        void                 *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);
void x2ap_process_oam_link_setup_req(
        void                 *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);

x2ap_return_et x2ap_start_csi_guard_tim(
        x2ap_timer_id_en   timer_type,
        U32                timer_value,
        x2ap_timer_t      *p_timer_id);

x2ap_return_et x2ap_build_and_send_s1ap_comm_info(
        x2ap_gb_context_t *p_x2ap_gb_ctx);

/* RSU Changes Start */
extern x2ap_return_et x2ap_build_and_send_csc_cell_status_req(
        x2ap_gb_context_t *p_x2ap_gb_ctx);
extern const void * key_of_rsu_enb_meas_id_search_tree(
        const YTNODE * p_node);

extern S32 compare_rsu_enb_meas_id_search_tree(
        const void * p_elem1,
        const void * p_elem2);

extern void rsu_st_init(
      /* Pointer to tree being initialised */
      x2ap_search_tree_t *p_tree,

      /* Pointer to compare function :
       it returns 0 if A==B, -1 if A<B and 1 if A>B */
      S32 (*compare)(const void *, const void *),

      /* Pointer to function returning Key */
      const void *(*keyof)(const x2ap_search_tree_t *));

extern void x2ap_process_add_enb(
        void               *p_api,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        U16                dest_module_id);

extern void x2ap_process_del_enb(
        void               *p_api,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        U16                dest_module_id);
/*sctp parameters start*/
#if 0		
#ifdef ARICENT_SCTP_IPR
void x2ap_oam_convert_sctp_mib_status(
       sctpmib_assoc_state_et *p_assoc_state,
      U32 *p_s1ap_oam_sctp_status);
#endif
#endif
void x2ap_process_oam_get_sctp_status_req(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);

x2ap_return_et x2ap_build_oam_get_sctp_status_resp_with_bitmask(
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
	x2ap_oam_get_sctp_status_req_t  *p_x2ap_oam_get_sctp_status_req,
	x2ap_oam_get_sctp_status_resp_t  *p_x2ap_oam_get_sctp_status_resp,
	U8 *gb_enb_id_count,
	U8 *failed_enb_count);

x2ap_return_et x2ap_build_oam_get_sctp_status_resp_without_bitmask(
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
	x2ap_oam_get_sctp_status_resp_t  *p_x2ap_oam_get_sctp_status_resp,
	U8 *gb_enb_id_count);

/*sctp parameters stop*/

void x2ap_process_oam_proc_req
(
    void *p_api
    /*SPR 17777 +-*/
);

void x2ap_oam_send_proc_res
(
    U16 src_module_id,
    U16 trans_id
);

void start_x2ap_health_monitor_timer
(
 /*SPR 17777 +-*/
    U32                  duration
);

/*Bug 9305 start*/
extern U32 x2ap_health_monitor_val;
/*Bug 9305 end*/

/*SPR_20175_FIX_START*/
extern void x2ap_stop_x2_handover_procedure(
		x2ap_peer_enb_context_t  *p_peer_enb_ctx);

extern void x2ap_stop_resource_status_update_procedure(
		x2ap_gb_context_t *p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t  *p_peer_enb_ctx);

extern void x2ap_stop_enb_configuration_update_procedure(
		x2ap_peer_enb_context_t  *p_peer_enb_ctx);

extern void x2ap_send_reset_ind(
		x2ap_gb_context_t *p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t* p_peer_enb_cntx,
		x2ap_Cause cause);
/*SPR_20175_FIX_STOP*/

/*****************************************************************************
 * Function Name  : x2ap_process_oam_messages
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to x2AP global context.
 * Outputs        : None
 * Returns        : None
 * Description    : This function is invoked to handle OAM messages to x2AP.
******************************************************************************/
void x2ap_process_oam_messages(
        /* Pointer to input API buffer */
        void            *p_api,         

        /* Pointer to the x2AP global context data */
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{

    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    X2AP_UT_TRACE_ENTER();
    switch (x2ap_get_api_id((U8*)p_api))
    {
       case X2AP_OAM_PROVISION_REQ:
            x2ap_process_oam_prov_req(p_api, p_x2ap_gb_ctx);
            break;
       case X2AP_OAM_CLEANUP_REQ:
            x2ap_process_oam_cleanup_req(p_api, p_x2ap_gb_ctx);
            break;
       case X2AP_OAM_LINK_SETUP_REQ:
            x2ap_process_oam_link_setup_req(p_api, p_x2ap_gb_ctx);
            break;
       case X2AP_ADD_ENB_REQ:
            x2ap_process_add_enb(p_api, p_x2ap_gb_ctx, RRC_OAM_MODULE_ID);
            break;
       case X2AP_DEL_ENB_REQ:
            x2ap_process_del_enb(p_api, p_x2ap_gb_ctx, RRC_OAM_MODULE_ID);
            break;
       /*sctp parameters start*/
       case X2AP_OAM_GET_SCTP_STATUS_REQ:
            x2ap_process_oam_get_sctp_status_req(p_api, p_x2ap_gb_ctx);
            break;
       /*sctp parameters stop*/
       case X2AP_OAM_PROC_SUPP_REQ:
            x2ap_process_oam_proc_req(p_api);/*SPR 17777 +-*/
            break;
	    /* SPR-16334 START */            
       case X2AP_ENB_CONFIG_UPDATE_REQ:
	    x2ap_process_enb_config_update_req(p_api, p_x2ap_gb_ctx, RRC_OAM_MODULE_ID);
	    break;
	    /* SPR-16334 END */            
       case X2AP_OAM_SET_LOG_LEVEL_REQ:
            x2ap_process_oam_set_log_level_req(p_api, p_x2ap_gb_ctx);
            break;
       case X2AP_OAM_GET_LOG_LEVEL_REQ:
            x2ap_process_oam_get_log_level_req(p_api, p_x2ap_gb_ctx);
            break;
       case X2AP_OAM_LOG_ENABLE_REQ:
            x2ap_process_oam_log_enable_req(p_api, p_x2ap_gb_ctx);
            break;
       default:
            X2AP_TRACE(X2AP_WARNING,
                    "x2ap_process_oam_messages: "
                    "unexpected API from OAM module received");
            break;
    }
    X2AP_UT_TRACE_EXIT();
}

/******************************************************************************
 * *   FUNCTION NAME: x2ap_process_oam_set_log_level_req 
 * *   INPUTS       : void                  *p_api
 * *                  x2ap_gb_context_t     *p_x2ap_gb_context.
 * *   OUTPUTS      :
 * *   DESCRIPTION:
 * *       This function handles X2AP_OAM_SET_LOG_LEVEL_REQ msg
 * *
 * *   RETURNS:
 * *       None
 * ******************************************************************************/
void x2ap_process_oam_set_log_level_req(
        void                   *p_api,
        x2ap_gb_context_t      *p_x2ap_gb_context)
{
    x2ap_oam_set_log_level_req_t    in_message;
    x2ap_oam_set_log_level_resp_t   out_message;
    S32 length_read    = RRC_NULL;
    U16 transaction_id = RRC_NULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_api);
    X2AP_ASSERT(PNULL != p_x2ap_gb_context);

    memset_wrapper(&out_message, 0, sizeof(out_message));
   if(RRC_SUCCESS !=  rrc_il_parse_x2ap_oam_set_log_level_req(
                       &in_message,(U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                       rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                        &length_read))
   {
       X2AP_TRACE(X2AP_WARNING, "Bad message - ignore.");
       X2AP_UT_TRACE_EXIT();
       return;
   }

   transaction_id = rrc_get_transaction_id(p_api);

#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    /* Set new log level */
        x2ap_change_logging_parameters(p_x2ap_gb_context,
                get_x2ap_log_level_from_oam_log_level(in_message.log_level),
                        p_x2ap_gb_context->log_enabled);
#endif

   out_message.response = RRC_SUCCESS;

  rrc_x2apOam_il_send_x2ap_oam_set_log_level_resp(&out_message,
                                              RRC_X2AP_MODULE_ID,
                                              RRC_OAM_MODULE_ID,
                                              transaction_id,
                                              RRC_NULL);

   X2AP_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: x2ap_process_oam_get_log_level_req 
*   INPUTS       : void                  *p_api
*                  x2ap_gb_context_t     *p_x2ap_gb_context 
*   OUTPUTS      :
*   DESCRIPTION:
*       This function handles X2AP_OAM_GET_LOG_LEVEL_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
void x2ap_process_oam_get_log_level_req 
(
    void                  *p_api,
    x2ap_gb_context_t     *p_x2ap_gb_context
)
{
    x2ap_oam_get_log_level_resp_t    out_message;
    U16 transaction_id = RRC_NULL;


    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_api);
    X2AP_ASSERT(PNULL != p_x2ap_gb_context);

    memset_wrapper(&out_message, 0, sizeof(out_message));
    transaction_id = rrc_get_transaction_id(p_api);
    /*SPR 17777 +-*/
    if ((rrc_bool_et)p_x2ap_gb_context->log_enabled == RRC_TRUE)
    {
        out_message.log_on_off = OAM_LOG_ON;
        out_message.bitmask |= X2AP_OAM_LOG_LEVEL_PRESENT;
        out_message.log_level = get_oam_log_level_from_x2ap_log_level(p_x2ap_gb_context->log_level);
    }
    else
    {
        out_message.log_on_off = OAM_LOG_OFF;
    }
    rrc_x2apOam_il_send_x2ap_oam_get_log_level_resp(
                        &out_message,
                        RRC_X2AP_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        transaction_id,
                        RRC_NULL);

    X2AP_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: x2ap_oam_process_log_enable_req
*
*   INPUTS       : void                  *p_api
*                  x2ap_gb_context_t *p_x2ap_gb_context
*   OUTPUTS      :
*   DESCRIPTION:
*       This function handles X2AP_OAM_LOG_ENABLE_REQ msg
*
*   RETURNS:
*       None
******************************************************************************/
void x2ap_process_oam_log_enable_req
(
    void                  *p_api,
    x2ap_gb_context_t *p_x2ap_gb_context
)
{
    x2ap_oam_log_enable_req_t    in_message;
    x2ap_oam_log_enable_resp_t   out_message;
    S32 length_read         = RRC_NULL;
    U16 transaction_id      = RRC_NULL;
    /* Coverity Fix 97153 Start */
    x2ap_bool_et log_enabled = X2AP_TRUE;
    /* Coverity Fix 97153 End */
    X2AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_x2ap_gb_context);

    memset_wrapper(&out_message, 0, sizeof(out_message));

    if(RRC_SUCCESS !=
       rrc_il_parse_x2ap_oam_log_enable_req(
            &in_message,
            (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read))
    {
        X2AP_TRACE(RRC_WARNING, "Bad message - ignore.");
        X2AP_UT_TRACE_EXIT();
        return;
    }
    transaction_id = rrc_get_transaction_id(p_api);

    if (OAM_LOG_ON == in_message.log_on_off)
    {
        /* Coverity Fix 97153 Start */
        log_enabled = X2AP_TRUE;
        /* Coverity Fix 97153 End */
    }
    else
    {
        /* Coverity Fix 97153 Start */
        log_enabled = X2AP_FALSE;
        /* Coverity Fix 97153 End */
    }
#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    /* Enable/Disable logging */
    x2ap_change_logging_parameters(p_x2ap_gb_context, p_x2ap_gb_context->
        log_level, log_enabled);
#endif
/* (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */

    out_message.response = RRC_SUCCESS;

    rrc_x2apOam_il_send_x2ap_oam_log_enable_resp(
                        &out_message,
                        RRC_X2AP_MODULE_ID,
                        RRC_OAM_MODULE_ID,
                        transaction_id,
                        RRC_NULL);

    X2AP_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: get_oam_log_level_from_x2ap_log_level
*   INPUTS       : U8 x2ap_log_level
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function returns OAM log level identifier from X2AP log level
*       identifier
*
*   RETURNS:
*       OAM log level identifier
******************************************************************************/
U8 get_oam_log_level_from_x2ap_log_level
(
    U8 x2ap_log_level /* X2AP log level identifier */
    )
{
    U8 oam_log_level = RRC_NULL;

    switch (x2ap_log_level)
    {
        case X2AP_FATAL:
            oam_log_level = (U8)OAM_LOG_LEVEL_FATAL;
            break;

        case X2AP_ERROR:
            oam_log_level = (U8)OAM_LOG_LEVEL_ERROR;
            break;

        case X2AP_WARNING:
            oam_log_level = (U8)OAM_LOG_LEVEL_WARNING;
            break;

        case X2AP_INFO:
            oam_log_level = (U8)OAM_LOG_LEVEL_INFO;
            break;

        case X2AP_BRIEF:
            oam_log_level = (U8)OAM_LOG_LEVEL_BRIEF;
            break;

        case X2AP_DETAILED:
            oam_log_level = (U8)OAM_LOG_LEVEL_DETAILED;
            break;

        case X2AP_DETAILEDALL:
            oam_log_level = (U8)OAM_LOG_LEVEL_DETAILEDALL;
            break;

        default:
            X2AP_ASSERT(RRC_FALSE);
    }

    return oam_log_level;
}

/*****************************************************************************
 * Function Name  : x2ap_process_oam_prov_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the provisioning request
 *                  received from OAM module to establish SCTP
 *                  associations and sends the response back to OAM.
 ****************************************************************************/
void x2ap_process_oam_prov_req(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{
    x2ap_return_et  response = X2AP_FAILURE;
    x2ap_info_t    *p_x2ap_info = X2AP_P_NULL;
    x2ap_oam_provision_req_t *p_x2ap_prov_req = X2AP_P_NULL;
    x2ap_oam_provision_resp_t x2ap_oam_prov_resp = {0} ;
    S32             length_read = X2AP_NULL;
    U16             trans_id = X2AP_NULL;
    U32             loop = X2AP_NULL;

    static x2ap_bool_et is_initialised_once = X2AP_FALSE;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    /* Get the transaction id */
    trans_id = x2ap_get_transaction_id(p_api);

    /* Check the state of x2ap */
    if (X2AP_WF_PROVISIONING_REQ_ST != 
            p_x2ap_gb_ctx->x2ap_init_sm_blk.x2ap_current_state)
    {
        X2AP_TRACE(X2AP_WARNING, "X2ap is in active state but should ideally wait for provisioning request");
        return;
    }

    p_x2ap_prov_req = (x2ap_oam_provision_req_t*)
        x2ap_mem_get(sizeof(x2ap_oam_provision_req_t));

    if (X2AP_P_NULL == p_x2ap_prov_req)
    {
	return;
    }
    /* Decode the received message from OAM */
    response = rrc_il_parse_x2ap_oam_provision_req(
            p_x2ap_prov_req ,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            x2ap_get_api_buf_size(p_api) - 
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read);

#if (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    x2ap_change_logging_parameters(p_x2ap_gb_ctx,
        get_x2ap_log_level_from_oam_log_level(p_x2ap_prov_req->x2ap_info.log_level),
         p_x2ap_gb_ctx->log_enabled);
#endif
/* (LOGGER_TYPE == RRC_ADV_LOGGER) || (LOGGER_TYPE == RRC_INTERNAL_LOGGER) */

    if (X2AP_FAILURE == response)
    {
        x2ap_oam_prov_resp.response = X2AP_FAILURE;
        /* Encode and send the provisioning response to OAM */
        X2AP_TRACE(X2AP_WARNING, "x2ap OAM PROV REQ decode failed x2ap OAM PROV RESP Sent with Response:%d", 
                x2ap_oam_prov_resp.response);
        response = rrc_x2apOam_il_send_x2ap_oam_provision_resp(
                &x2ap_oam_prov_resp,
                RRC_X2AP_MODULE_ID,
                RRC_OAM_MODULE_ID,
                trans_id,
                RRC_NULL);
        if (X2AP_FAILURE == response)
        {
            X2AP_TRACE(X2AP_WARNING, "x2ap OAM PROV RESP encode failed");
        }

        /*valgrind_fix start*/
        if (X2AP_P_NULL != p_x2ap_prov_req)
        {
            x2ap_mem_free(p_x2ap_prov_req);
        }
        /*valgrind_fix stop*/

       return ;
    }
    else if(p_x2ap_prov_req->bitmask & RRC_X2AP_PROVISION_INFO_PRESENT)
    {
        if (X2AP_FALSE == is_initialised_once)
        {
            if (p_x2ap_gb_ctx->x2ap_init_sm_blk.oam_init_ind_tim_id)
            {
                x2ap_stop_timer(p_x2ap_gb_ctx->x2ap_init_sm_blk.
                        oam_init_ind_tim_id);
                p_x2ap_gb_ctx->x2ap_init_sm_blk.oam_init_ind_tim_id = 
                    X2AP_P_NULL;
            }
        }

        p_x2ap_info = &(p_x2ap_prov_req->x2ap_info);

        /* Save local ENB Communication Info in Global DB */
        X2AP_MEMCPY( (void*)(&(p_x2ap_gb_ctx->local_enb_info)),
                (const void* )(&p_x2ap_info->local_enb_comm_info),
                sizeof(x2_enb_comm_info_t) );

        /* Populate Local Global ENB */
        X2AP_MEMCPY((void*)(&(p_x2ap_gb_ctx->global_enb_id)),
                (const void* )(&p_x2ap_info->global_enb_id),
                sizeof(x2_gb_enb_id_t));

        /* Populate SON IP address */
        if(p_x2ap_prov_req->bitmask & SON_MODULE_INFO_PRESENT)
        {
            U8 min_ip_value[IPV4_ADDRESS_LENGTH] = {RRC_NULL};
            U8 max_ip_value[IPV4_ADDRESS_LENGTH] = {255,255,255,255};

            X2AP_TRACE(X2AP_INFO, "SON module communication info present, X2AP will ignore APIs from OAM\n");
            if (!(memcmp_wrapper(min_ip_value, p_x2ap_prov_req->son_comm_info.ip_addr, IPV4_ADDRESS_LENGTH)) ||
                !(memcmp_wrapper(max_ip_value, p_x2ap_prov_req->son_comm_info.ip_addr, IPV4_ADDRESS_LENGTH)))
            {
            	X2AP_TRACE(X2AP_INFO, "Invalid SON IP address: %u.%u.%u.%u:%u.\n",
                		p_x2ap_prov_req->son_comm_info.ip_addr[0],
                		p_x2ap_prov_req->son_comm_info.ip_addr[1],
                		p_x2ap_prov_req->son_comm_info.ip_addr[2],
                		p_x2ap_prov_req->son_comm_info.ip_addr[3],
                		p_x2ap_prov_req->son_comm_info.port);

                /*valgrind_fix start*/
                if (X2AP_P_NULL != p_x2ap_prov_req)
                {
                    x2ap_mem_free(p_x2ap_prov_req);
                }
                /*valgrind_fix stop*/
            	return;
            }
            else
            {
            	p_x2ap_gb_ctx->is_son_present = TRUE;
/*Coverity Fix 20196 Start*/
            	snprintf_wrapper((Char8 *)&(((comm_info_init_param_t
                                    *)(p_x2ap_gb_ctx->p_comm_info_int))->comm_info.comm_info_upper_layer[3][0]),
                         MAX_LEN_COMM_INFO,
                        (const Char8*) "%u.%u.%u.%u:%u",
                        p_x2ap_prov_req->son_comm_info.ip_addr[0],
                        p_x2ap_prov_req->son_comm_info.ip_addr[1],
                        p_x2ap_prov_req->son_comm_info.ip_addr[2],
                        p_x2ap_prov_req->son_comm_info.ip_addr[3],
                        p_x2ap_prov_req->son_comm_info.port);
/*Coverity Fix 20196 End*/
                X2AP_TRACE(X2AP_INFO, "SON IP address: %u.%u.%u.%u:%u.\n",
                		p_x2ap_prov_req->son_comm_info.ip_addr[0],
                		p_x2ap_prov_req->son_comm_info.ip_addr[1],
                		p_x2ap_prov_req->son_comm_info.ip_addr[2],
                		p_x2ap_prov_req->son_comm_info.ip_addr[3],
                		p_x2ap_prov_req->son_comm_info.port);
            }
        }
        else
        {
        	X2AP_TRACE(X2AP_INFO, "SON module communication info not present\n");
        }
        
        if(p_x2ap_prov_req->x2ap_info.conf_param.timer_conf_info.bitmask & X2AP_OAM_X2_HEALTH_MONITORING_TIMER_PRESENT)
        {
/*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL
            x2ap_lock = qvNewLock(); 
#endif
/*Lockless Implementation stop*/
            start_x2ap_health_monitor_timer(/*SPR 17777 +-*/
                    (p_x2ap_prov_req->x2ap_info.conf_param.timer_conf_info.x2ap_health_mon_time_int * 1000));
        }

        /* Copy Number of Peer ENB */
        p_x2ap_gb_ctx->num_peer_enb = p_x2ap_info->num_peer_enb;

        if(p_x2ap_gb_ctx->num_peer_enb ==0)
        {
        	X2AP_TRACE(X2AP_INFO, "No peer eNB present\n");
        }
        else
        {
			for (loop = 0; loop < p_x2ap_gb_ctx->num_peer_enb; ++loop)

			{
				X2AP_TRACE(X2AP_INFO, "%u peer eNB present\n", p_x2ap_gb_ctx->num_peer_enb);

				/* update peer enodeB Id */
				p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].peer_enodeb_id = (U8)loop;

				/* Populate Peer ENB Communication Info */
				X2AP_MEMCPY( (void*)(&(p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].
								enb_comm_info) ), (const void* )
						(&p_x2ap_info->peer_enb_comm_info[loop]),
						sizeof(x2_enb_comm_info_t) );
				p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].sctp_sd =
															INVALID_SCTP_SOCKET;
				p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].op_status = NOT_ASSOCIATED;

				X2AP_MEMSET(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].enb_config_sm_ctx, 0x00, sizeof(x2ap_enb_config_sm_context_t));

				p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].x2_setup_sm_ctx.
											 x2Setup_curr_state = X2_SETUP_IDLE_ST;

                p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].is_enb_conf = X2AP_TRUE;
			}
        }

#ifdef ENDC_ENABLED
        /* MENB CHNAGES - START */

        U32    loop_gnb    =    X2AP_NULL;
        if ( X2AP_NULL == p_x2ap_info->num_peer_gnb )
        {
        	X2AP_TRACE(X2AP_INFO, "No peer gNB present\n");
        }
        else
        {
			for ( loop = (p_x2ap_gb_ctx->num_peer_gnb + p_x2ap_gb_ctx->num_peer_enb);
                loop < (p_x2ap_info->num_peer_gnb + p_x2ap_gb_ctx->num_peer_enb); ++loop )

			{
				X2AP_TRACE(X2AP_INFO, "%u peer gNB present\n", p_x2ap_info->num_peer_gnb);

				/*
                 * update peer gnodeB Id
                 */
				p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].peer_enodeb_id = (U8)loop;

				/*
                 * Populate Peer GNB Communication Info
                 */
                X2AP_MEMCPY( (void*)(&(p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].
                                enb_comm_info) ), (const void* )
                        (&p_x2ap_info->peer_gnb_comm_info[loop_gnb]),
                        sizeof(x2_enb_comm_info_t) );

				p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].sctp_sd =
															INVALID_SCTP_SOCKET;
				p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].op_status = NOT_ASSOCIATED;

				X2AP_MEMSET(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].enb_config_sm_ctx,
                    0x00, sizeof(x2ap_enb_config_sm_context_t));
                            X2AP_MEMSET(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].endc_config_sm_ctx,
                    0x00, sizeof(x2ap_endc_config_sm_context_t));

				p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].x2_setup_sm_ctx.
											 x2Setup_curr_state = X2_SETUP_IDLE_ST;

                p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].is_enb_conf = X2AP_TRUE;

                p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].peer_enb_type = X2AP_TRUE;
                loop_gnb++;
			}

            /* Copy Number of Peer GNB */
            p_x2ap_gb_ctx->num_peer_gnb = p_x2ap_info->num_peer_gnb;
        }
        /* MENB CHNAGES - END */
#endif
        /* Populate Served Cell Info */
        p_x2ap_gb_ctx->num_served_cells = p_x2ap_info->srvd_cell.
            num_srvd_cell;
        if(p_x2ap_gb_ctx->num_served_cells==0)
        {
        	X2AP_TRACE(X2AP_INFO, "No serving cell present\n");
        }
        else
        {
            U8 cell_id = 0;

            X2AP_TRACE(X2AP_INFO, "%u serving cell present\n", p_x2ap_gb_ctx->num_served_cells);

            for (loop = 0; loop < p_x2ap_gb_ctx->num_served_cells;
                    ++loop)
            {

                cell_id = p_x2ap_info->srvd_cell.
                    srvd_cell_arr[loop].served_cell_info.cell_id;

                /* Initialise active served cell info in Global eNB Context */
                   p_x2ap_gb_ctx->act_cell_info[loop].cell_id = (U8)cell_id;
                   p_x2ap_gb_ctx->act_cell_info[loop].is_cell_active =
                       X2AP_FALSE;

                /* Update Served cell info in Global eNB Context */
                x2ap_oam_populate_served_cell_info(p_x2ap_gb_ctx, 
                        &(p_x2ap_gb_ctx->served_cell_info[cell_id]),
                        & (p_x2ap_info->srvd_cell.srvd_cell_arr[loop].
                            served_cell_info), cell_id); 

                {
               	p_x2ap_gb_ctx->served_cell_info[cell_id].num_nbour = p_x2ap_info->srvd_cell.srvd_cell_arr[loop].nbour_cell_info.num_neighbour_cell;

                    if(p_x2ap_info->srvd_cell.srvd_cell_arr[loop].nbour_cell_info.num_neighbour_cell==0)
                    {
                    	X2AP_TRACE(X2AP_INFO, "No neighbour cell present for %u serving cell with cell id:%u\n",loop, cell_id);
                    }
                    else
					{
                    	U32 count = 0;

                    	X2AP_TRACE(X2AP_INFO, "%u neighbour cell present for %u serving cell with cell id:%u\n", p_x2ap_info->srvd_cell.srvd_cell_arr[loop].nbour_cell_info.num_neighbour_cell, loop, cell_id);

                    	/* Populate Peer cell info */
                    	x2ap_nbour_cell_arry_t *p_nbour_cell_info =
							&(p_x2ap_info->srvd_cell.srvd_cell_arr[loop].
									nbour_cell_info);

						for (count = 0; count < p_nbour_cell_info->num_neighbour_cell;
								++count)
						{
							/*  Update neighbour cell info and relation table */
							x2ap_upd_neighbour_cell_info(p_x2ap_gb_ctx,
									&(p_nbour_cell_info->neighbour_info_arr[count]),
									cell_id);
						}
					}
                }
            }
        }

        /* Populate Local Group Id list */
        if(p_x2ap_info->x2ap_gu_grp_id.num_gu_group_id==0)
        {
        	X2AP_TRACE(X2AP_WARNING, "No GU group id present\n");
        	p_x2ap_info->x2ap_gu_grp_id.num_gu_group_id = 0;
        }
        else
        {
        	X2AP_TRACE(X2AP_WARNING, "%u GU group id present\n", p_x2ap_info->x2ap_gu_grp_id.num_gu_group_id);

        	X2AP_MEMCPY ( (void*)&(p_x2ap_gb_ctx->group_ids),
                (const void*)&(p_x2ap_info->x2ap_gu_grp_id),
                sizeof(x2ap_gu_grp_ids_t) );
        }
        
        /* Populate X2AP setup time to wait index */
        if(p_x2ap_info->conf_param.timer_conf_info.bitmask
                & X2AP_TIME_TO_WAIT_INDEX_PRESENT)
        {
            if(X2AP_FAILURE == validate_x2setup_time_to_wait_index_range(
                        p_x2ap_info->conf_param.timer_conf_info.
                        x2ap_time_to_wait_index))
            {
                /*valgrind_fix start*/
                if (X2AP_P_NULL != p_x2ap_prov_req)
                {
                    x2ap_mem_free(p_x2ap_prov_req);
                }
                /*valgrind_fix stop*/
                return; 
            }
        }
        
        /* Populate SCTP/TIMER Configuration Parameters */
        x2ap_populate_configuration_parameters( &(p_x2ap_gb_ctx->config_param),&(p_x2ap_info->conf_param));
        /* Start CSI Indication Guard Timer */
        if(p_x2ap_gb_ctx->num_peer_enb > 0)
        {   
            if (X2AP_SUCCESS == x2ap_start_csi_guard_tim(X2AP_CSI_IND_GUARD_TIMER,
                        p_x2ap_gb_ctx->config_param.timer_conf_info.
                        x2ap_csi_guard_tim_val, 
                        &(p_x2ap_gb_ctx->cell_up_guard_tim_id)) )
            {
                X2AP_TRACE(X2AP_INFO, "Creating X2AP Server");
                if (X2AP_SUCCESS == x2ap_create_server(p_x2ap_gb_ctx))
                {
                    x2ap_oam_prov_resp.response = X2AP_SUCCESS;
                }
            }
            x2ap_invoke_sctp_sm(p_x2ap_gb_ctx);
        }
#ifdef ENDC_ENABLED
        /* MENB CHNAGES - START */
        else if(p_x2ap_gb_ctx->num_peer_gnb > 0)
        {   
            X2AP_TRACE(X2AP_INFO, "Creating X2AP Server");
            if (X2AP_SUCCESS == x2ap_create_server(p_x2ap_gb_ctx) )
            {
                x2ap_oam_prov_resp.response = X2AP_SUCCESS;
            }
            x2ap_invoke_sctp_sm(p_x2ap_gb_ctx);
        }
        /* MENB CHNAGES - END */
#endif
        else
        {
            /* coverity fix 277490 */
             X2AP_TRACE(X2AP_INFO, "Creating X2AP Server");
             if (X2AP_SUCCESS == x2ap_create_server(p_x2ap_gb_ctx))
             {
                 x2ap_oam_prov_resp.response = X2AP_SUCCESS;
             }
             else
             {
                 X2AP_TRACE(X2AP_ERROR, "X2AP server creation failed");
             }
             /* coverity fix 277490 */
        }

        /* Encode and send the provisioning response to OAM */
        response = rrc_x2apOam_il_send_x2ap_oam_provision_resp(
                &x2ap_oam_prov_resp,
                RRC_X2AP_MODULE_ID,
                RRC_OAM_MODULE_ID,
                trans_id,
                RRC_NULL);

        /* GDB Fix - compiler warning (&& is a higher precedence than ||. Compiler wants to make sure
                     that is what you want so suggest parentheses) */
        if ( X2AP_FAILURE == response || (x2ap_oam_prov_resp.response == X2AP_FAILURE && p_x2ap_gb_ctx->num_peer_enb) )
        {
            X2AP_TRACE(X2AP_INFO, "Stop CSI Indication Timer");
            x2ap_stop_timer(p_x2ap_gb_ctx->cell_up_guard_tim_id);
            p_x2ap_gb_ctx->cell_up_guard_tim_id = X2AP_P_NULL;

            /*valgrind_fix start*/
            if (X2AP_P_NULL != p_x2ap_prov_req)
            {
                x2ap_mem_free(p_x2ap_prov_req);
            }
            /*valgrind_fix stop*/
            return ;
        }
        p_x2ap_gb_ctx->is_prov_done = X2AP_TRUE;
        if (X2AP_FAILURE == 
                x2ap_build_and_send_csc_cell_status_req(p_x2ap_gb_ctx))
        {
            X2AP_TRACE(X2AP_INFO, "Unable to send csc_cell_status_req to CSC.");
        }

/*TNL Changes start */        
        if (X2AP_FAILURE == 
                x2ap_build_and_send_s1ap_comm_info(p_x2ap_gb_ctx))
        {
            X2AP_TRACE(X2AP_INFO, "Unable to send x2ap_s1ap_comm_info to S1AP");
        }

/* TNL changes End */
        x2ap_mem_free(p_x2ap_prov_req);
        /*Fix */
        p_x2ap_prov_req = X2AP_P_NULL;
        p_x2ap_gb_ctx->x2ap_init_sm_blk.x2ap_current_state = X2AP_ACTIVE_ST;

        /* Initialize RSU context tree*/
        x2ap_search_tree_init(&(p_x2ap_gb_ctx->x2ap_rsu_context_tree),
                &compare_rsu_enb_meas_id_search_tree,
                 key_of_rsu_enb_meas_id_search_tree);
     }
/*SPR 19237 fix start */
	else
/*SPR 19237 fix end */
    {
        /*Stop sending X2AP_OAM_INIT_IND to OAM*/
        if (X2AP_FALSE == is_initialised_once)
        {
            if (p_x2ap_gb_ctx->x2ap_init_sm_blk.oam_init_ind_tim_id)
            {
                x2ap_stop_timer(p_x2ap_gb_ctx->x2ap_init_sm_blk.
                        oam_init_ind_tim_id);
                p_x2ap_gb_ctx->x2ap_init_sm_blk.oam_init_ind_tim_id = 
                    X2AP_P_NULL;
            }
        }

        /* Encode and send the provisioning response to OAM */
        x2ap_oam_prov_resp.response = X2AP_SUCCESS;

        response = rrc_x2apOam_il_send_x2ap_oam_provision_resp(
                &x2ap_oam_prov_resp,
                RRC_X2AP_MODULE_ID,
                RRC_OAM_MODULE_ID,
                trans_id,
                RRC_NULL);
	/* + Coverity 81473 */
    	if (response == X2AP_FAILURE)
    	{
        	X2AP_TRACE(X2AP_WARNING, "Sending of X2AP OAM provision Response"
         	" failed");
    	}
	/* - Coverity 81473 */
    }

/*valgrind_fix start*/
    if (X2AP_P_NULL != p_x2ap_prov_req)
    {
        x2ap_mem_free(p_x2ap_prov_req);
    }
/*valgrind_fix stop*/

    X2AP_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
 * Function Name  : x2ap_oam_populate_served_cell_info
 * Inputs         : p_x2ap_gb_ctx - Pointer to X2AP global context
 *                  p_x2ap_gb_ctxt_cell_info - Pointer to Served Cell Context
 *                  p_x2ap_oam_cell_info - Pointer to x2apOam_srvd_cell_info_t
 *                  cell_id - cell id
 * Outputs        : p_x2ap_gb_ctxt_cell_info
 * Returns        : None
 * Description    : This function processes OAM init timer message. It creates
 *                  and send X2AP_OAM_INIT_IND to OAM module.
 ******************************************************************************/
void    x2ap_oam_populate_served_cell_info(
        x2ap_gb_context_t        *p_x2ap_gb_ctx, 
        x2ap_cell_info_t         *p_x2ap_gb_ctxt_cell_info,
        x2apOam_srvd_cell_info_t *p_x2ap_oam_cell_info,
        U8                       cell_id)
{
    X2AP_UT_TRACE_ENTER();
    /*SPR 21554 Start*/
    x2ap_populate_ecgi_from_cell_id( 
                p_x2ap_gb_ctx,
                cell_id,
                &p_x2ap_gb_ctxt_cell_info->ecgi);
    /*SPR 21554 End*/

    p_x2ap_gb_ctxt_cell_info->pci = p_x2ap_oam_cell_info->pci;

    p_x2ap_gb_ctxt_cell_info->bitmask = p_x2ap_oam_cell_info->bitmask;

    X2AP_MEMCPY( (void*)(&(p_x2ap_gb_ctxt_cell_info->track_area_code)),
            (const void* ) (&(p_x2ap_oam_cell_info->track_area_code)),
            sizeof(x2ap_tac_t));

    X2AP_MEMCPY( (void*)(&(p_x2ap_gb_ctxt_cell_info->broadcast_plmn_info)),
            (const void* ) (&(p_x2ap_oam_cell_info->broadcast_plmn_info)), 
            sizeof(x2ap_broadcast_plmn_t));   

    X2AP_MEMCPY( (void*)(&(p_x2ap_gb_ctxt_cell_info->choice_eutra_mode)),
            (const void* ) (&(p_x2ap_oam_cell_info->choice_eutra_mode)), 
            sizeof(x2ap_choice_eutra_mode_info_t));

    if(p_x2ap_oam_cell_info->bitmask & NUM_ANTENNA_PORT_BITMASK)
    {
    	p_x2ap_gb_ctxt_cell_info->num_antenna_port =
            p_x2ap_oam_cell_info->num_antenna_port;
    	X2AP_TRACE(X2AP_INFO, "Number of antenna port info present %d", p_x2ap_gb_ctxt_cell_info->num_antenna_port);
    }
    else
    {
    	X2AP_TRACE(X2AP_INFO, "Number of antenna port info not present\n");
    }
    if(p_x2ap_oam_cell_info->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
    {

    	X2AP_MEMCPY((void*)&(p_x2ap_gb_ctxt_cell_info->prach_config),
    				   (const void* )&(p_x2ap_oam_cell_info->prach_config),
    				   sizeof(x2ap_prach_configuration_info_t));
    	X2AP_TRACE(X2AP_INFO, "PRACH configuration info present root_sequence_index[%u],"
                "zero_correlation_zone_config[%u], high_speed_flag[%d], prach_frequency_offset[%d]",
                p_x2ap_oam_cell_info->prach_config.root_sequence_index, p_x2ap_oam_cell_info->prach_config.zero_correlation_zone_config,
                p_x2ap_oam_cell_info->prach_config.high_speed_flag, p_x2ap_oam_cell_info->prach_config.prach_frequency_offset);

        if (p_x2ap_oam_cell_info->prach_config.bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
        {
            X2AP_TRACE(X2AP_INFO, "prach_config_index[%u]", p_x2ap_oam_cell_info->prach_config.prach_config_index);
        }

    }
    else
    {
    	X2AP_TRACE(X2AP_INFO, "PRACH configuration info not present\n");
    }
    if(p_x2ap_oam_cell_info->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
    {
    	X2AP_TRACE(X2AP_INFO, "MBSFN subframe info present\n");

    	X2AP_MEMCPY((void*)&(p_x2ap_gb_ctxt_cell_info->mbsfn_subframe_info),
    				   (const void* )&(p_x2ap_oam_cell_info->mbsfn_subframe_info),
    				   sizeof(x2ap_mbsfn_subframe_info_t));
    }
    else
    {
    	X2AP_TRACE(X2AP_INFO, "MBSFN subframe info not present\n");
    }
    if(p_x2ap_oam_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {
	    p_x2ap_gb_ctxt_cell_info->bitmask |=
				X2AP_CSG_ID_INFO_PRESENT;

    	X2AP_MEMCPY((void*)&(p_x2ap_gb_ctxt_cell_info->csg_identity),
    				   (const void* )&(p_x2ap_oam_cell_info->csg_identity),
    				   sizeof(p_x2ap_gb_ctxt_cell_info->csg_identity));
    	X2AP_TRACE(X2AP_INFO, "CSG ID info present 0x%x%x%x%x",
                p_x2ap_oam_cell_info->csg_identity[0], p_x2ap_oam_cell_info->csg_identity[1],
                p_x2ap_oam_cell_info->csg_identity[2], p_x2ap_oam_cell_info->csg_identity[3]);
    }

    X2AP_UT_TRACE_EXIT();
    return;
}
/*****************************************************************************
 * Function Name  : x2ap_upd_neighbour_cell_info
 * Inputs         : p_x2ap_gb_ctxt - Pointer to Global Context.
 *                  p_neighbour_cell_info - Pointer to Neighbour Cell Info.
 *                  served_cell_id - served cell id
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes update Neighbour Cell Info in 
 *                  Global DB
******************************************************************************/
void    x2ap_upd_neighbour_cell_info(
        x2ap_gb_context_t        *p_x2ap_gb_ctx,
        x2ap_neighbour_info_t    *p_neighbour_cell_info,
        U8                        served_cell_id)
{ 
    S32 peer_cell_idx = 0;
    X2AP_UT_TRACE_ENTER();
    
    if (served_cell_id >= MAX_NUM_CELL_IN_X2AP)
    {
        X2AP_TRACE(X2AP_WARNING, "%s:"
                "Served Cell Id is wrong=%d.",__FUNCTION__,served_cell_id);
        X2AP_UT_TRACE_EXIT();
        return;
    }
    
    /* get Peer Cell Logical Id */
    peer_cell_idx = x2ap_get_nbr_index_from_ecgi(p_x2ap_gb_ctx, 
            p_neighbour_cell_info->ecgi, X2AP_TRUE, served_cell_id);
    if ((peer_cell_idx == -1) || (peer_cell_idx >= MAX_NEIGHBOURS))
    {
        X2AP_TRACE(X2AP_WARNING, "%s:"
                "Peer Cell Logical Id is not correct.",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return;
    }

    /* Populate peer Cell Info */
    if(peer_cell_idx < MAX_NEIGHBOURS && served_cell_id < MAX_NUM_CELL_IN_X2AP)
    {
        if (X2AP_EUTRA_MODE_FDD == 
                p_neighbour_cell_info->bitmask)
        {
            X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->
                        peer_enb_cell_info[peer_cell_idx].
                        peer_cell_info.choice_eutra_mode.
                        choice_eutra_mode.fdd_info.dl_earfcn),
                    (const void*)(&(p_neighbour_cell_info->earfcn)),
                    sizeof(x2ap_earfcn_t));
        }
        else
        {
            X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->
                        peer_enb_cell_info[peer_cell_idx].
                        peer_cell_info.choice_eutra_mode.
                        choice_eutra_mode.tdd_info.earfcn), 
                    (const void*)(&(p_neighbour_cell_info->earfcn)),
                    sizeof(x2ap_earfcn_t));
        }
        X2AP_MEMCPY((void*)&(p_x2ap_gb_ctx->peer_enb_cell_info[peer_cell_idx].
                    peer_cell_info.earfcn),
                (const void*)(&(p_neighbour_cell_info->earfcn)),
                sizeof(x2ap_earfcn_t));

        X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->
                    peer_enb_cell_info[peer_cell_idx].
                    peer_cell_info.pci), 
                (const void*)(&(p_neighbour_cell_info->pci)),
                sizeof(U32));

        X2AP_MEMCPY( (void*)&(p_x2ap_gb_ctx->
                    peer_enb_cell_info[peer_cell_idx].
                    peer_cell_info.ecgi), 
                (const void*)(&(p_neighbour_cell_info->ecgi)),
                sizeof(x2ap_ecgi_t));

        /* update Served/neighbour Relation Table */
        p_x2ap_gb_ctx->relation_table[served_cell_id][peer_cell_idx].relation =
            DIRECT_NEIGHBOUR_INACTIVE;
        p_x2ap_gb_ctx->relation_table[served_cell_id][peer_cell_idx].ecgi = p_neighbour_cell_info->ecgi;
    }
    X2AP_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
 * Function Name  : x2ap_process_init_timer
 * Inputs         : p_x2ap_timer_buf - Pointer to timer buffer.
 *                  p_x2ap_gb_ctx - Pointer to X2AP global context.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes OAM init timer message. It creates
 *                  and send X2AP_OAM_INIT_IND to OAM module.
******************************************************************************/
void x2ap_process_init_timer()
{
    U8    *p_api = X2AP_P_NULL;
    U8    *p_msg = X2AP_P_NULL;
    U16   msg_len = CV_HDRSIZE + RRC_INTERFACE_API_HEADER_SIZE;

    X2AP_UT_TRACE_ENTER();

    /* ARGUSED */

    p_api =(U8*) x2ap_msg_mem_get(msg_len);

    if (X2AP_P_NULL == p_api)
    {
        X2AP_TRACE(X2AP_WARNING, "%s:"\
                "x2ap_msg_mem_get failed.",__FUNCTION__);
        return;
    }

    X2AP_TRACE(X2AP_DETAILED, "%s:"\
            "Send INIT Message to OAM.",__FUNCTION__);
    p_msg = p_api;

    /* Fill CSPL Header */
    x2ap_construct_api_header(p_msg, X2AP_VERSION_ID, RRC_X2AP_MODULE_ID,
            RRC_OAM_MODULE_ID, X2AP_OAM_INIT_IND, msg_len);

    /* Fill Interface Header */
    p_msg = p_msg + CV_HDRSIZE;

    x2ap_construct_interface_api_header(p_msg, X2AP_TRANSACTION_ID_ABSENT,
            RRC_X2AP_MODULE_ID, RRC_OAM_MODULE_ID, X2AP_OAM_INIT_IND, 0);

    X2AP_TRACE(X2AP_INFO, "Src(%d)->Dst(%d):X2AP_OAM_INIT_IND", RRC_X2AP_MODULE_ID, RRC_OAM_MODULE_ID);
    /* Send message to OAM */
    rrc_send_message(
            /*SPR 20172 Fix Start*/
            p_api,
            /*SPR 20172 Fix End*/
            RRC_OAM_MODULE_ID);



    X2AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : x2ap_create_server
 * Inputs         : p_x2ap_gb_ctx       : pointer to X2AP global context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function call sctp wrapper to create server. 
 *****************************************************************************/
x2ap_return_et  x2ap_create_server(
        x2ap_gb_context_t *p_x2ap_gb_ctx)
{
    /* SPR 20633 changes start */
//#ifdef ARICENT_SCTP_IPR
//    S32 server_fd = X2AP_NULL;
//#else
    S32 server_fd_ipv6 = X2AP_NULL;
    S32 server_fd_ipv4 = X2AP_NULL;
//#endif
    /* SPR 20633 changes end */
    x2ap_sctp_assoc_conf_info_t  sctp_conf_info;
    x2ap_sctp_conn_info_t        sctp_server_info;

    X2AP_UT_TRACE_ENTER();
    X2AP_MEMSET(&sctp_conf_info, X2AP_NULL,
                sizeof(x2ap_sctp_assoc_conf_info_t));
    X2AP_MEMSET(&sctp_server_info, X2AP_NULL,
                sizeof(x2ap_sctp_conn_info_t));
    /* Populate SCTP Configuration Parameters from Global DB */
    x2ap_popu_sctp_conf_info_from_gb_db( &(sctp_conf_info),
            &( p_x2ap_gb_ctx->config_param.sctp_conf_param));

    /* Fill SCTP Server Info */
    sctp_server_info.p_comm_info = &(p_x2ap_gb_ctx->local_enb_info);
    sctp_server_info.p_sctp_assoc_conf_info = &(sctp_conf_info);

#ifdef LINUX_PC_TEST
    // initial value is 0,set to invalid value
    p_x2ap_gb_ctx->server_fd_ipv4 = INVALID_SCTP_SOCKET;
    p_x2ap_gb_ctx->server_fd_ipv6 = INVALID_SCTP_SOCKET;
#endif

    /*sctp parameters start*/
    /* SPR 20633 Changes start */

//#ifdef ARICENT_SCTP_IPR
#if 0
    if ((X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT & p_x2ap_gb_ctx->local_enb_info.bitmask) && (X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT & p_x2ap_gb_ctx->local_enb_info.bitmask))
    {
#ifdef LINUX_PC_TEST
        server_fd = x2ap_get_sock_fd_for_test(X2AP_P_NULL);
#else
        server_fd = x2ap_open_sctp_server_both(p_x2ap_gb_ctx, &sctp_server_info, 
                MAX_PEER_ENB);
#endif
        /*sctp parameters stop*/

        /* Insert Server FD in both trees for local info */
        if (server_fd == INVALID_SCTP_SOCKET)
        {
            X2AP_TRACE(X2AP_ERROR, "[%s]: Server Creation Failed!!", __FUNCTION__);
            return X2AP_FAILURE;
        }
        else
        {
            X2AP_TRACE(X2AP_INFO, 
                    "Server Creation Successful and server fd=%d !!",server_fd);
            /* Store the server_fd in Global Context */
            p_x2ap_gb_ctx->server_fd = server_fd;
        }
    }
    else if(X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT & p_x2ap_gb_ctx->local_enb_info.bitmask)
    {
#ifdef LINUX_PC_TEST
        server_fd = x2ap_get_sock_fd_for_test(X2AP_P_NULL);
#else
        server_fd = x2ap_open_sctp_server(p_x2ap_gb_ctx, &sctp_server_info, 
                MAX_PEER_ENB);
#endif
        /*sctp parameters stop*/

        /* Insert Server FD in both trees for local info */
        if (server_fd == INVALID_SCTP_SOCKET)
        {
            X2AP_TRACE(X2AP_ERROR, "[%s]: Server Creation Failed!!", __FUNCTION__);
            return X2AP_FAILURE;
        }
        else
        {
            X2AP_TRACE(X2AP_INFO, 
                    "Server Creation Successful and server fd=%d !!",server_fd);
            /* Store the server_fd in Global Context */
            p_x2ap_gb_ctx->server_fd = server_fd;
        }
    }
    else if(X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT & p_x2ap_gb_ctx->local_enb_info.bitmask)
    {
#ifdef LINUX_PC_TEST
        server_fd = x2ap_get_sock_fd_for_test(X2AP_P_NULL);
#else
        server_fd = x2ap_open_sctp_server_ipv4(p_x2ap_gb_ctx, &sctp_server_info, 
                MAX_PEER_ENB);
#endif
        /*sctp parameters stop*/

        /* Insert Server FD in both trees for local info */
        if (server_fd == INVALID_SCTP_SOCKET)
        {
            X2AP_TRACE(X2AP_ERROR, "[%s]: Server Creation Failed!!", __FUNCTION__);
            return X2AP_FAILURE;
        }
        else
        {
            X2AP_TRACE(X2AP_INFO, 
                    "Server Creation Successful and server fd=%d !!",server_fd);
            /* Store the server_fd in Global Context */
            p_x2ap_gb_ctx->server_fd = server_fd;
        }
    }
#endif	
//#else
    if(X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT & p_x2ap_gb_ctx->local_enb_info.bitmask)
    {
#ifdef LINUX_PC_TEST
        server_fd_ipv6 = x2ap_get_sock_fd_for_test(X2AP_P_NULL);
        g_sctp_server_fd_ipv6 = server_fd_ipv6;
#else
        server_fd_ipv6 = x2ap_open_sctp_server(p_x2ap_gb_ctx, &sctp_server_info, 
            MAX_PEER_ENB);
#endif
    /*sctp parameters stop*/

    /* Insert Server FD in both trees for local info */
        if (server_fd_ipv6 == INVALID_SCTP_SOCKET)
    {
        X2AP_TRACE(X2AP_ERROR, "[%s]: Server Creation Failed!!", __FUNCTION__);
        return X2AP_FAILURE;
    }
    else
    {
        X2AP_TRACE(X2AP_INFO, 
                    "Server Creation Successful and server fd=%d !!",server_fd_ipv6);
        /* Store the server_fd in Global Context */
            p_x2ap_gb_ctx->server_fd_ipv6 = server_fd_ipv6;
        }
    }
    if(X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT & p_x2ap_gb_ctx->local_enb_info.bitmask)
    {
#ifdef LINUX_PC_TEST
        server_fd_ipv4 = x2ap_get_sock_fd_for_test(X2AP_P_NULL);
        g_sctp_server_fd_ipv4 = server_fd_ipv4;
#else
        server_fd_ipv4 = x2ap_open_sctp_server_ipv4(p_x2ap_gb_ctx, &sctp_server_info, 
                MAX_PEER_ENB);
#endif
        /*sctp parameters stop*/

        /* Insert Server FD in both trees for local info */
        if (server_fd_ipv4 == INVALID_SCTP_SOCKET)
        {
            X2AP_TRACE(X2AP_ERROR, "[%s]: Server Creation Failed!!", __FUNCTION__);
            return X2AP_FAILURE;
        }
        else
        {
            X2AP_TRACE(X2AP_INFO, 
                    "Server Creation Successful and server fd=%d !!",server_fd_ipv4);
            /* Store the server_fd in Global Context */
            p_x2ap_gb_ctx->server_fd_ipv4 = server_fd_ipv4;
        }
    }

//#endif

    /* SPR 20633 changes end */
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;        
}

/*****************************************************************************
 * Function Name  : x2ap_popu_sctp_conf_info_from_gb_db 
 * Inputs         : p_sctp_assoc_conf_info - pointer to 
 *                                           x2ap_sctp_assoc_conf_info_t
 *                  p_x2ap_sctp_conf_info - pointer to x2ap_sctp_conf_info_t
 * Outputs        : p_sctp_assoc_conf_info 
 * Returns        : None
 * Description    : This function SCTP COnfiguration info in Global DB
 ********************************************************************************/
extern void x2ap_popu_sctp_conf_info_from_gb_db(
        x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info,
        x2ap_sctp_conf_info_t      *p_x2ap_sctp_conf_info)
{
    X2AP_UT_TRACE_ENTER();
    p_sctp_assoc_conf_info->init_msg.sinit_num_ostreams =
        p_x2ap_sctp_conf_info->init_num_ostreams;//init_max_instreams;
    p_sctp_assoc_conf_info->init_msg.sinit_max_instreams = 
        p_x2ap_sctp_conf_info->init_max_instreams;
    p_sctp_assoc_conf_info->init_msg.sinit_max_attempts =
        p_x2ap_sctp_conf_info->init_max_attempts;
    p_sctp_assoc_conf_info->init_msg.sinit_max_init_timeo = 
        p_x2ap_sctp_conf_info->init_max_init_timeo;
    p_sctp_assoc_conf_info->rtoinfo.srto_initial = 
        p_x2ap_sctp_conf_info->rto_initial;
    p_sctp_assoc_conf_info->rtoinfo.srto_max =
        p_x2ap_sctp_conf_info->rto_max;
    p_sctp_assoc_conf_info->rtoinfo.srto_min = 
        p_x2ap_sctp_conf_info->rto_min;
    p_sctp_assoc_conf_info->paddr.spp_hbinterval = 
        p_x2ap_sctp_conf_info->hbinterval;
    p_sctp_assoc_conf_info->paddr.spp_pathmaxrxt = 
        p_x2ap_sctp_conf_info->pathmaxrxt;
    /*sctp parameters start*/
    if(X2AP_SCTP_ASSOC_MAX_RETRANS_PRESENT & p_x2ap_sctp_conf_info->bitmask)
    {
        p_sctp_assoc_conf_info->assocparams.sasoc_asocmaxrxt = 
            p_x2ap_sctp_conf_info->assoc_max_retrans;
    }
    else
    {
        p_sctp_assoc_conf_info->assocparams.sasoc_asocmaxrxt =
            RRC_SCTP_DEFAULT_ASSOC_MAX_RTX;
    }
    if(X2AP_SCTP_VAL_COOKIE_LIFE_PRESENT & p_x2ap_sctp_conf_info->bitmask)
    {
        p_sctp_assoc_conf_info->assocparams.sasoc_cookie_life =
            p_x2ap_sctp_conf_info->valid_cookie_life;
    }
    else
    {
        p_sctp_assoc_conf_info->assocparams.sasoc_cookie_life =
            RRC_SCTP_DEFAULT_VAL_COOKIE_LIFE;
    }
    /*sctp parameters stop*/
    /* dscp_marking_start */
    if(X2AP_OAM_DSCP_VALUE_PRESENT & p_x2ap_sctp_conf_info->bitmask)
    {
//#ifdef ARICENT_SCTP_IPR    
#if 0
        p_sctp_assoc_conf_info->bitmask |= X2AP_DSCP_VALUE_PRESENT;
	/* SPR-22189 START */
        p_sctp_assoc_conf_info->spp_dscp = 
            p_x2ap_sctp_conf_info->x2ap_dscp_value;
	/* SPR-22189 END */
#endif           
//#else
        p_sctp_assoc_conf_info->bitmask |= X2AP_DSCP_VALUE_PRESENT;
        p_sctp_assoc_conf_info->x2ap_dscp_value =
            p_x2ap_sctp_conf_info->x2ap_dscp_value;
//#endif
    }
    /* dscp_marking_stop */
    X2AP_UT_TRACE_EXIT();
};

/*****************************************************************************
 * Function Name  : x2ap_invoke_sctp_sm
 * Inputs         : p_x2ap_gb_ctx       : pointer to X2AP global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function call udpate IP Search tree for enb Context
 *                  and invoke SCTP SM.
 *****************************************************************************/
void    x2ap_invoke_sctp_sm(
        x2ap_gb_context_t *p_x2ap_gb_ctx)
{
	/*U8  num_peer_enb = X2AP_NULL;*/
	U8  count = X2AP_NULL;
	U32  loop = 0;
    /*NR_DC Changes Start*/
    U8  num_peer_nodeB = X2AP_NULL;
    /*NR_DC Changes Stop*/

	x2ap_peer_enb_context_t     *p_peer_enb_ctxt = X2AP_P_NULL; 

	/*Converting to IPv6Address Compressed Format start*/
	struct sockaddr_in6         p_addr6;
	/*Converting to IPv6Address Compressed Format end*/


	X2AP_UT_TRACE_ENTER();
/*NR_DC Changes Start*/
#ifdef ENDC_ENABLED
    num_peer_nodeB = p_x2ap_gb_ctx->num_peer_enb + p_x2ap_gb_ctx->num_peer_gnb;
    X2AP_TRACE(X2AP_INFO, 
            "Number of peer eNB[%d] gNB[%d]\n", 
            p_x2ap_gb_ctx->num_peer_enb,
            p_x2ap_gb_ctx->num_peer_gnb);
#else
    num_peer_nodeB = p_x2ap_gb_ctx->num_peer_enb;
    X2AP_TRACE(X2AP_INFO, 
            "Number of peer eNB[%d]\n", 
            p_x2ap_gb_ctx->num_peer_enb);
#endif
	for ( count = 0 ; count < num_peer_nodeB; ++count)
/*NR_DC Changes Stop*/
	{
		/* Create ENB Context */
		p_peer_enb_ctxt = &(p_x2ap_gb_ctx->peer_enb_cntxt_arr[count]);

		/* Initialize SCTP SM State to Idle */
		p_peer_enb_ctxt->sctp_sm_ctx.cur_sctp_sm_state = 
			X2AP_SCTP_IDLE_ST;
		/*spr8341 IPv6 fix start*/
		if(!(p_peer_enb_ctxt->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT))
		{
			for (loop = 0; loop < p_peer_enb_ctxt->enb_comm_info.num_ip_addr;
					++loop)
			{
				/*Create Tree Node */
				sctp_ip_search_node_t  *p_tree_node = 
					(sctp_ip_search_node_t*)x2ap_mem_get
					(sizeof(sctp_ip_search_node_t));

				if (PNULL != p_tree_node)
				{
					/* Copy IP Address in tree node */
					/*Coverity Fix 20195 Start*/

					l3_strncpy_wrapper ((S8 *)(p_tree_node->enb_ip_address),
							(const S8 *)(p_peer_enb_ctxt->enb_comm_info.ip_addr[loop].ip_addr),
							MAX_X2AP_IP_ADDRESS_LENGTH);

					/*Coverity Fix 20195 End*/
					p_tree_node->p_enb_context = p_peer_enb_ctxt;
					/* Insert in ip_port tree*/
					x2ap_st_insert_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
								ip_search_tree), &(p_tree_node->nodeAnchor));
				}

			}
		}
		else
		{
			for (loop = 0; loop < p_peer_enb_ctxt->enb_comm_info.num_ipv6_addr;
					++loop)
			{
				/*Create Tree Node */
				sctp_ip_search_node_t  *p_tree_node = 
					(sctp_ip_search_node_t*)x2ap_mem_get
					(sizeof(sctp_ip_search_node_t));

				if (PNULL != p_tree_node)
				{

					/*Converting to IPv6Address Compressed Format start*/
					l3_strncpy_wrapper ((S8 *)(p_tree_node->enb_ipv6_address),
							(const S8 *)(p_peer_enb_ctxt->enb_comm_info.
								ipv6_addr[loop].ipv6_addr),
							MAX_X2AP_IPV6_ADDRESS_LENGTH);
					bzero_wrapper( (void *)&p_addr6, sizeof(struct sockaddr_in6) );

					if(inet_pton_wrapper(AF_INET6,(const char *)(p_tree_node->enb_ipv6_address),
								(void *)(&p_addr6.sin6_addr)) != 1)
					{
						X2AP_TRACE(X2AP_WARNING, "X2ap inet_pton_wrapper failed");
						return;
					}

					inet_ntop_wrapper(AF_INET6,(const void *)&p_addr6.sin6_addr,
							(char *)(p_tree_node->enb_ipv6_address),
							MAX_X2AP_IPV6_ADDRESS_LENGTH);
					/*Converting to IPv6Address Compressed Format end*/
					p_tree_node->p_enb_context = p_peer_enb_ctxt;
					/* Insert in ip_port tree*/
					x2ap_st_insert_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
								ipv6_search_tree), &(p_tree_node->nodeAnchor));
				}
			}
		}
		/*spr8341 IPv6 fix end*/
		/*SPR_17448_START*/
#ifdef LKSCTP
		if (!(p_x2ap_gb_ctx->cell_up_guard_tim_id))
		{
			x2ap_sctp_sm_entry(
					X2AP_SCTP_CONNECT_EV,
					/*coverity_fix*/
					(U8*)p_x2ap_gb_ctx,
					/*coverity_fix*/
					p_x2ap_gb_ctx,
					p_peer_enb_ctxt);
		}
		else
		{
			X2AP_TRACE(X2AP_WARNING, "Cell is not up, not invoking sctp sm");
		}
#else
		x2ap_sctp_sm_entry(
				X2AP_SCTP_CONNECT_EV,
				/*coverity_fix*/
				(U8*)p_x2ap_gb_ctx,
				/*coverity_fix*/
				p_x2ap_gb_ctx,
				p_peer_enb_ctxt);
#endif    
		/*SPR_17448_END*/
	}

	X2AP_UT_TRACE_EXIT();
	return;        
}


/*****************************************************************************
 * Function Name  : x2ap_process_oam_cleanup_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the cleanup request
 *                  received from OAM module to to cloase SCTP
 *                  associations and sends the response back to OAM.
 ****************************************************************************/
void x2ap_process_oam_cleanup_req(
        void                 *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{

    x2ap_return_et              response = X2AP_FAILURE;
    x2ap_oam_cleanup_resp_t    x2ap_oam_cleanup_resp = {0} ;
    /*NR_DC Changes Start*/
    U8  num_peer_nodeB = X2AP_NULL;
    /*NR_DC Changes Stop*/

    U32 loop = X2AP_NULL;
    U16 trans_id = 0;

    /*SPR_20175_FIX_START*/
    x2ap_Cause          cause = {0};
    /* Code Removed */
    /*SPR_20175_FIX_STOP*/

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    /* Get the transaction id */
    trans_id = x2ap_get_transaction_id(p_api);

    /* Check the state of x2ap */
    if (X2AP_ACTIVE_ST != p_x2ap_gb_ctx->x2ap_init_sm_blk.x2ap_current_state)
    {
        X2AP_TRACE(X2AP_WARNING, "X2ap is not in active state");
    }
    else
    {
        /*SPR_20175_FIX_START*/
        cause.t = T_x2ap_Cause_misc;
        cause.u.misc = x2ap_om_intervention;
        /*SPR_20175_FIX_STOP*/
        /*NR_DC Changes Start*/
#ifdef ENDC_ENABLED
        num_peer_nodeB = p_x2ap_gb_ctx->num_peer_enb + p_x2ap_gb_ctx->num_peer_gnb;
        X2AP_TRACE(X2AP_INFO, 
                "Number of peer eNB[%d] gNB[%d]\n", 
                p_x2ap_gb_ctx->num_peer_enb,
                p_x2ap_gb_ctx->num_peer_gnb);
#else
        num_peer_nodeB = p_x2ap_gb_ctx->num_peer_enb;
        X2AP_TRACE(X2AP_INFO, 
                "Number of peer eNB[%d]\n", 
                p_x2ap_gb_ctx->num_peer_enb);
#endif
        for (loop = 0; loop < num_peer_nodeB; ++loop)
            /*NR_DC Changes Stop*/
        {
            /*SPR_20175_FIX_START*/
            /* Stop X2 HANDOVER procedure */
            x2ap_stop_x2_handover_procedure(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop]);

            /* Stop RESOURCE STATUS UPDATE procedure */
            x2ap_stop_resource_status_update_procedure(p_x2ap_gb_ctx, &p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop]);

            /* Stop eNB CONFIGURATION UPDATE procedure */
            x2ap_stop_enb_configuration_update_procedure(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop]);

            /* Stop ENDC CONFIGURATION UPDATE procedure */
            x2ap_stop_endc_configuration_update_procedure(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop]);

            /*Build and send Send reset indication to RRM.*/
            x2ap_send_reset_ind(p_x2ap_gb_ctx, &p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop], cause);
            /*SPR_20175_FIX_STOP*/
            /* Connection close event for all the peer_enb Socket Fds */
            x2ap_sctp_sm_entry(
                    X2AP_SCTP_CONN_CLOSE_EV,
                    (U8*)p_x2ap_gb_ctx,
                    p_x2ap_gb_ctx,
                    &p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop]);
        }
        /* Connection close event for the Server Socket Fd */
        x2ap_sctp_sm_entry(
                X2AP_SCTP_CLOSE_ALL_X2AP_SCTP_CONN_EV,
                /*coverity_fix*/
                (U8*)p_x2ap_gb_ctx,
                /*coverity_fix*/
                p_x2ap_gb_ctx,
                &p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop]);

        /*SPR_20175_FIX_START*/
        /* Code Removed */	
        /*SPR_20175_FIX_STOP*/

        /*SPR_20175_7Jul_FIX_START*/
        /*Code Removed*/
        /*SPR_20175_7Jul_FIX_STOP*/

#ifdef LINUX_PC_TEST
    test_x2ap_global_context_reset(p_x2ap_gb_ctx);
    test_x2ap_init();
#endif

        x2ap_init_int(p_x2ap_gb_ctx);

        /* set the state to Wait for init cnf from OAM */
        p_x2ap_gb_ctx->x2ap_init_sm_blk.x2ap_current_state = 
            X2AP_WF_PROVISIONING_REQ_ST;
        /*Lockless Implementation start*/
#ifndef LOCKLESS_IMPL
        if(PNULL != x2ap_lock)
        {
            qvDestroyLock(x2ap_lock);
            x2ap_lock = PNULL;
        }
#endif
        /*Lockless Implementation stop*/
    }

    x2ap_oam_cleanup_resp.response = X2AP_SUCCESS;
    /* Encode and send the provisioning response to OAM */
    response = rrc_x2apOam_il_send_x2ap_oam_cleanup_resp(
            &x2ap_oam_cleanup_resp,
            RRC_X2AP_MODULE_ID,
            RRC_OAM_MODULE_ID,
            trans_id,
            RRC_NULL);
    if (response == X2AP_FAILURE)
    {
        X2AP_TRACE(X2AP_WARNING, "Decoding of X2AP OAM CLeanup Response"
         " failed");
    }
    X2AP_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
 * Function Name  : x2ap_start_csi_guard_tim
 * Inputs         : timer_type    : Timer Type
 *                  timer_value   : Timer Value
 *                  p_timer_id - timer id
 * Outputs        : p_timer_id : Timer Id
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS
 * Description    : This start csi guard timer 
 *****************************************************************************/
x2ap_return_et 
x2ap_start_csi_guard_tim
(
 x2ap_timer_id_en   timer_type,
 U32                timer_value,
 x2ap_timer_t      *p_timer_id
 )
{
    x2ap_return_et ret_val = X2AP_SUCCESS;
    x2ap_timer_buffer_t timer_buf;

    X2AP_UT_TRACE_ENTER();
    X2AP_MEMSET(&timer_buf, 0, sizeof(x2ap_timer_buffer_t));
    
    timer_buf.timer_type = timer_type;
    timer_buf.auto_delete = X2AP_TRUE;

    *p_timer_id = x2ap_start_timer(
            timer_value, &timer_buf,
            sizeof(timer_buf), X2AP_FALSE );

    X2AP_TRACE(X2AP_INFO,"[%s] Start CSI Gurad timer: %d Timer Value:%d",
            __FUNCTION__, timer_type, timer_value);
    X2AP_UT_TRACE_EXIT();
    return ret_val;
}
/*****************************************************************************
 * Function Name  : x2ap_process_oam_link_setup_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the link setup request
 *                  received from OAM module to open closed association 
 *                  with early associated ENB and send response back to OAM.
 ****************************************************************************/
void x2ap_process_oam_link_setup_req(
        void                 *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{
    x2ap_return_et              response = X2AP_FAILURE;
    x2ap_oam_x2_link_setup_resp_t x2ap_oam_link_setup_resp = {0} ;
    x2ap_oam_x2_link_setup_req_t  *p_x2_link_setup_req = X2AP_P_NULL;
    sctp_gb_enb_id_search_node_t  *p_gb_enb_id_node = X2AP_P_NULL;
    U16 trans_id = 0;
    S32 length_read = 0;

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    /* Get the transaction id */
    trans_id = x2ap_get_transaction_id(p_api);

    /* Check the state of x2ap */
    if (X2AP_ACTIVE_ST != p_x2ap_gb_ctx->x2ap_init_sm_blk.x2ap_current_state)
    {
        X2AP_TRACE(X2AP_WARNING, "X2ap is in active state but should \
                ideally wait for provisioning request");
        return;
    }

    p_x2_link_setup_req = (x2ap_oam_x2_link_setup_req_t*)
        x2ap_mem_get(sizeof(x2ap_oam_x2_link_setup_req_t));

    if (PNULL == p_x2_link_setup_req)
    {
        X2AP_TRACE(X2AP_WARNING, "Unable to allocate memory");
        return;
    }

    /* Decode the received message from OAM */
    response = rrc_il_parse_x2ap_oam_x2_link_setup_req(
            p_x2_link_setup_req,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            x2ap_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read);

    x2ap_oam_link_setup_resp.response = response;
    if (X2AP_SUCCESS == response)
    {
        p_gb_enb_id_node = 
             (sctp_gb_enb_id_search_node_t *)x2ap_st_find_tree_node(
                 (const x2ap_search_tree_t *)(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                 enb_id_search_tree)),
                 (const void *)(&(p_x2_link_setup_req->peer_enb_gb_id)) );
        if (p_gb_enb_id_node)
        {
            if (INVALID_SCTP_SOCKET == p_gb_enb_id_node->p_enb_context->sctp_sd)
            {
                x2ap_sctp_sm_entry(
                     X2AP_SCTP_CONNECT_EV,
                     (U8*)p_x2ap_gb_ctx,
                     p_x2ap_gb_ctx,
                     p_gb_enb_id_node->p_enb_context);
            }
        }
        else
        {
            X2AP_TRACE(X2AP_INFO, "ENB ID Not found");
	    x2ap_oam_link_setup_resp.response = X2AP_FAILURE;
        }        
    }


    /* Encode and send the provisioning response to OAM */
    response = rrc_x2apOam_il_send_x2ap_oam_x2_link_setup_resp(
            &x2ap_oam_link_setup_resp,
            RRC_X2AP_MODULE_ID,
            RRC_OAM_MODULE_ID,
            trans_id,
            RRC_NULL);
    /* + Coverity 81399 */
    if (X2AP_FAILURE == response)
    {
    	X2AP_TRACE(X2AP_WARNING, "x2ap OAM link setup resp sending failed");
    }
    /* - Coverity 81399 */

    /* coverity_fix_63304_start */
    x2ap_mem_free(p_x2_link_setup_req);
    /* Coverity ID 85907 Fix Start*/
    /*p_x2_link_setup_req = X2AP_P_NULL; */
    /* Coverity ID 85907 Fix End*/
    /* coverity_fix_63304_stop */
    X2AP_UT_TRACE_EXIT();
    return;
}
/*sctp parameters start */
/*****************************************************************************
* Function Name  : x2ap_oam_convert_sctp_mib_status
* Inputs         : p_assoc_state - pointer to sctpmib_assoc_state_et
*                  p_s1ap_oam_sctp_status - pointer to sctp status
* Outputs        : p_s1ap_oam_sctp_status
* Returns        : None
* Description    : This function converts sctp mib status into
* s1ap_oam_sctp_status 
****************************************************************************/
#if 0
#ifdef ARICENT_SCTP_IPR
void  x2ap_oam_convert_sctp_mib_status(
        sctpmib_assoc_state_et *p_assoc_state, 
        U32 *p_s1ap_oam_sctp_status )
{
    X2AP_UT_TRACE_ENTER();
    switch(*p_assoc_state)
    {
        case SM_CLOSED: 
            *p_s1ap_oam_sctp_status = S1AP_OAM_SCTP_CLOSED;
             break;
        case SM_COOKIE_WAIT:
            *p_s1ap_oam_sctp_status = S1AP_OAM_SCTP_COOKIE_WAIT;
             break;
        case SM_COOKIE_ECHOD: 
            *p_s1ap_oam_sctp_status = S1AP_OAM_SCTP_COOKIE_ECHOED;
             break;
        case SM_ESTABLISHED: 
            *p_s1ap_oam_sctp_status = S1AP_OAM_SCTP_ESTABLISHED;
             break;
        case SM_SHUTDOWN_PENDING: 
            *p_s1ap_oam_sctp_status = S1AP_OAM_SCTP_SHUTDOWN_PENDING;
             break;  
        case SM_SHUTDOWN_SENT: 
            *p_s1ap_oam_sctp_status =  S1AP_OAM_SCTP_SHUTDOWN_SENT;
             break;
        case SM_SHUTDOWN_RECEIVED: 
            *p_s1ap_oam_sctp_status =  S1AP_OAM_SCTP_SHUTDOWN_RECEIVED;
             break;
        case SM_SHUTDOWN_ACK_SENT: 
            *p_s1ap_oam_sctp_status =  S1AP_OAM_SCTP_SHUTDOWN_ACK_SENT;
             break;
        default:
            RRC_TRACE(RRC_WARNING,"Invalid State");
            break;
    }
    X2AP_UT_TRACE_EXIT();
    
}
#endif
#endif
/*sctp parameters start*/
/*****************************************************************************
 * Function Name  : x2ap_build_oam_get_sctp_status_resp_with_bitmask 
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the link setup request
 *                  received from OAM module to open closed association 
 *                  with early associated ENB and send response back to OAM.
 ****************************************************************************/
x2ap_return_et x2ap_build_oam_get_sctp_status_resp_with_bitmask(
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
	x2ap_oam_get_sctp_status_req_t  *p_x2ap_oam_get_sctp_status_req,
	x2ap_oam_get_sctp_status_resp_t  *p_x2ap_oam_get_sctp_status_resp,
	U8 *gb_enb_id_count,
	U8 *failed_enb_count)
{
    x2ap_peer_enb_context_t  *p_enb_ctx = X2AP_P_NULL;

#ifdef ENDC_ENABLED
    /* MENB CHNAGES - START */
    U8                       temp_gb_gnb_id_count  = X2AP_NULL;
    U8                       temp_failed_gnb_count = X2AP_NULL;
    U8                       num_peer_gnb          = X2AP_NULL;
    /* MENB CHNAGES - END */
#endif

    U8 num_peer_enb     = 0;
    U8 counter          = 0;
    U8 temp_gb_enb_id_count  = 0;
    U8 temp_failed_enb_count = 0;
    S32 connSock        = 0;

//#ifdef ARICENT_SCTP_IPR
#if 0
    sctpmib_assoc_table_st  *p_assoc_tab = PNULL;
    sctp_error_t             error_code = E_NOERROR;
    sctp_stats_et            stats_type = SCTP_ASSOC_STATS;
    U8 ret = 0;
    sctp_sockaddr_st *sctp_sin6;
    sctp_sockaddr_st *sctp_sin;
    U32 s1ap_sctp_status = RRC_NULL;
    struct in_addr in;
    char str[MAX_IP_ADDRESS_LENGTH];
#endif
//#else
    socklen_t optlen = 0;
    struct sctp_status  status;
    struct sctp_paddrinfo *spinfo;
    struct sockaddr_in6 *s_in6;
    struct sockaddr_in6 sa_in6;
    struct sockaddr_in *s_in;
    struct sockaddr_in sa_in;
    char tempname[MAX_X2AP_IP_ADDRESS_LENGTH];
    U16 local_port = 0;
    U32 socklen = RRC_NULL;
    S32 return_type  = 0;
//#endif
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

//#ifdef ARICENT_SCTP_IPR
#if 0
    p_assoc_tab = (sctpmib_assoc_table_st*)
        x2ap_mem_get(sizeof(sctpmib_assoc_table_st));

    if (PNULL == p_assoc_tab)
    {
        X2AP_TRACE(X2AP_WARNING,"Unable to allocate memory");
        return X2AP_FAILURE;
    }
#endif	
//#else
    memset_wrapper(&status, 0, sizeof(status));
//#endif
    /* Coverity Fix 86872 Start */
    for(num_peer_enb = 0;num_peer_enb < p_x2ap_oam_get_sctp_status_req->
        x2ap_oam_gb_enb_status_list.num_peer_enb;num_peer_enb++)
    {
        for(counter = 0; counter <MAX_PEER_ENB; counter++)
        {
            X2AP_TRACE(X2AP_WARNING, "Trying to get peer enb context");
            if(!(X2AP_MEMCMP(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].gb_enb_id,
                &(p_x2ap_oam_get_sctp_status_req->
                    x2ap_oam_gb_enb_status_list.gb_enb_id[num_peer_enb]), 
                sizeof(x2_gb_enb_id_t))))
            {
                p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
                X2AP_TRACE(X2AP_WARNING, "get peer enb context");
                break;
            }
        }
        if (PNULL == p_enb_ctx)
        {
            X2AP_TRACE(X2AP_WARNING, "peer enb context is NULL");
            if(!(X2AP_OAM_GLOBAL_ENB_FAILED_LIST_PRESENT &
                p_x2ap_oam_get_sctp_status_resp->bitmask))
            {
                p_x2ap_oam_get_sctp_status_resp->bitmask |=
                    X2AP_OAM_GLOBAL_ENB_FAILED_LIST_PRESENT; 
            }
            p_x2ap_oam_get_sctp_status_resp->gb_enb_failed_list.
                failed_global_enb_info[temp_failed_enb_count].cause = 
                X2AP_OAM_ENB_ID_NOT_FOUND;
            X2AP_MEMCPY(&p_x2ap_oam_get_sctp_status_resp->gb_enb_failed_list.
                failed_global_enb_info[temp_failed_enb_count].global_enb_id,
                    &(p_x2ap_oam_get_sctp_status_req->
                        x2ap_oam_gb_enb_status_list.gb_enb_id[num_peer_enb]),
                    sizeof(x2_gb_enb_id_t));
            temp_failed_enb_count ++;
            continue;
        }
        if(p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state != X2_SETUP_ACTIVE_ST)
        {
            X2AP_TRACE(X2AP_WARNING, "X2AP STATE is not Active");
            if(!(X2AP_OAM_GLOBAL_ENB_FAILED_LIST_PRESENT &
                p_x2ap_oam_get_sctp_status_resp->bitmask))
            {
                p_x2ap_oam_get_sctp_status_resp->bitmask |=
                    X2AP_OAM_GLOBAL_ENB_FAILED_LIST_PRESENT; 
            }
            p_x2ap_oam_get_sctp_status_resp->gb_enb_failed_list.
                failed_global_enb_info[temp_failed_enb_count].cause = 
                    X2AP_OAM_STATE_NOT_ACTIVE;
            X2AP_MEMCPY(&p_x2ap_oam_get_sctp_status_resp->gb_enb_failed_list.
                failed_global_enb_info[temp_failed_enb_count].global_enb_id,
                &(p_x2ap_oam_get_sctp_status_req->
                    x2ap_oam_gb_enb_status_list.gb_enb_id[num_peer_enb]),
                sizeof(x2_gb_enb_id_t));
            temp_failed_enb_count ++;
            continue;
        }
        else
        {
            if (INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd)
            {
                temp_gb_enb_id_count ++;
                connSock = p_enb_ctx->sctp_sd; 
//#ifdef ARICENT_SCTP_IPR
#if 0
                if(SCTP_FAILURE == sctp_api_enable_stats(stats_type,&error_code))
                {
                    X2AP_TRACE(X2AP_WARNING,"Unable to set SCTP Stats:SCTP_ASSOC_STATS");
                    if(PNULL != p_assoc_tab)
                    {
                        x2ap_mem_free(p_assoc_tab);
                    }
                    return X2AP_FAILURE;
                }					
                ret = sctpmib_get_assoc_table_var(
                        connSock,
                        SCTP_NULL,
                        SCTP_NULL,
                        p_assoc_tab,
                        &error_code);
#endif
//#else
#ifndef LINUX_PC_TEST
                memset_wrapper(&status, 0, sizeof(status));
                optlen = sizeof(status);
                return_type = getsockopt_wrapper(
                        connSock,
                        IPPROTO_SCTP,
                        SCTP_STATUS,
                        &status,
                        &optlen);
                spinfo = &status.sstat_primary;
                if (spinfo->spinfo_address.ss_family == AF_INET6) 
                {
                    s_in6 = (struct sockaddr_in6 *)&spinfo->spinfo_address;
                        inet_ntop_wrapper(AF_INET6, &s_in6->sin6_addr, tempname,
                        sizeof (tempname));

                    /*getsockname_wrapper to fill in this socket's local address*/
                    socklen = sizeof (sa_in6);
                    if (getsockname_wrapper((S32)connSock, (struct sockaddr *)&sa_in6, (socklen_t *)&socklen) == -1) 
                    {
                        X2AP_TRACE(X2AP_ERROR, "getsockname_wrapper failed");
                        return X2AP_FAILURE;
                    }
                    local_port = (int) ntohs_wrapper(sa_in6.sin6_port);
                    X2AP_TRACE(X2AP_INFO, "Local port is: %d\n", local_port);
                } 
                else if (spinfo->spinfo_address.ss_family == AF_INET) 
                {
                    s_in = (struct sockaddr_in *)&spinfo->spinfo_address;
                        inet_ntop_wrapper(AF_INET, &s_in->sin_addr, tempname, sizeof (tempname));

                    /*getsockname_wrapper to fill in this socket's local address*/
                    socklen = sizeof (sa_in);
                    if (getsockname_wrapper((S32)connSock, (struct sockaddr *)&sa_in,
                        (socklen_t *)&socklen) == -1) 
                    {
                        X2AP_TRACE(X2AP_ERROR, "getsockname_wrapper failed");
                        return X2AP_FAILURE;
                    }
                    local_port = (int) ntohs_wrapper(sa_in.sin_port);
                    X2AP_TRACE(X2AP_INFO, "Local port is: %d\n", local_port);
                } 
                else 
                {
                    X2AP_TRACE(X2AP_ERROR, "ss_family is not found");
                    return X2AP_FAILURE;
                }
#endif
//#endif
//#ifdef ARICENT_SCTP_IPR
//                if (SCTP_FAILURE == ret)
//#else
                if (return_type < 0)
//#endif
                {
                    X2AP_TRACE(X2AP_ERROR,"SCTP : Unable to retreive sctp status parameters");
#if 0
#ifdef ARICENT_SCTP_IPR
                    if(PNULL != p_assoc_tab)
                    {
                        x2ap_mem_free(p_assoc_tab);
                    }
#endif
#endif
                    return X2AP_FAILURE;
                }
                else
                {
                    if(!(X2AP_OAM_GLOBAL_ENB_STATUS_PARAMS_LIST_PRESENT & 
                        p_x2ap_oam_get_sctp_status_resp->bitmask))
                    {
                        p_x2ap_oam_get_sctp_status_resp->bitmask |= 
                            X2AP_OAM_GLOBAL_ENB_STATUS_PARAMS_LIST_PRESENT;
                    }
                    X2AP_MEMCPY(&p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].global_enb_id,&p_enb_ctx->gb_enb_id,
                        sizeof(x2_gb_enb_id_t));
//#ifdef ARICENT_SCTP_IPR
#if 0
                    x2ap_oam_convert_sctp_mib_status(&p_assoc_tab->assoc_state,
                        &s1ap_sctp_status);
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].status = s1ap_sctp_status;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].in_streams = p_assoc_tab->assoc_in_streams;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].out_streams = p_assoc_tab->assoc_out_streams;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].local_port = p_assoc_tab->local_sctp_port;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].start_time.tv_sec = 
                    (U32)p_assoc_tab->assoc_start_time.tv_sec;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].start_time.tv_usec = 
                    (U32)p_assoc_tab->assoc_start_time.tv_usec;
                    /*Retrieving Primary Peer Address */
                    if (p_assoc_tab->rem_prim_add.family == AF_INET6)
                    {
                        X2AP_TRACE(X2AP_INFO,"Family is AF_INET6");
                        sctp_sin6 = &p_assoc_tab->rem_prim_add;
                        inet_ntop_wrapper(AF_INET6, &(sctp_sin6->ip.v6.addr),str,INET_ADDRSTRLEN);
                        l3_memcpy_wrapper(&p_x2ap_oam_get_sctp_status_resp->
                            x2ap_oam_gb_enb_status_params_list.staus_params[num_peer_enb].
                                primary_peer_addr,str,
                            (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
                    }
                    else if (p_assoc_tab->rem_prim_add.family == AF_INET)
                    {
                        X2AP_TRACE(X2AP_INFO,"Family is AF_INET");
                        sctp_sin = &p_assoc_tab->rem_prim_add;
                        in.s_addr = SCTP_HTONL(sctp_sin->ip.v4.addr);
                        l3_memcpy_wrapper(&p_x2ap_oam_get_sctp_status_resp->
                            x2ap_oam_gb_enb_status_params_list.staus_params[num_peer_enb].
                                primary_peer_addr,inet_ntoa_wrapper(in),
                            (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
                    }
                    else
                    {
                        X2AP_TRACE(X2AP_ERROR, "Unknown Family name");
                        if(PNULL != p_assoc_tab)
                        {
                            x2ap_mem_free(p_assoc_tab);
                        }
                        return X2AP_FAILURE;
                    }
#endif
//#else
#ifndef LINUX_PC_TEST
                                /* Bug 3826 Start */
                                if ( status.sstat_state != 0)
                                {
                                    //Update the status value to conform to the interface API document.
                                    status.sstat_state = status.sstat_state + 2;
                                }
                                /* Bug 3826 End */

                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].status = status.sstat_state;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].in_streams = status.sstat_instrms;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].out_streams = status.sstat_outstrms;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].local_port = local_port;
                    l3_memcpy_wrapper(&p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].primary_peer_addr,tempname,
                        (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));

                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].start_time.tv_sec = 
                            p_x2ap_gb_ctx->sctp_start_time.tv_sec;

                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                            staus_params[num_peer_enb].start_time.tv_usec = 
                                p_x2ap_gb_ctx->sctp_start_time.tv_usec;
#endif
//#endif
                } 
            }
        }
    }
    if(X2AP_OAM_GLOBAL_ENB_STATUS_PARAMS_LIST_PRESENT & 
        p_x2ap_oam_get_sctp_status_resp->bitmask)
    {
        p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
            num_peer_enb = temp_gb_enb_id_count;
    }
    if(X2AP_OAM_GLOBAL_ENB_FAILED_LIST_PRESENT & 
        p_x2ap_oam_get_sctp_status_resp->bitmask)
    {
        p_x2ap_oam_get_sctp_status_resp->gb_enb_failed_list.
            count = temp_failed_enb_count;
    }

    *gb_enb_id_count = temp_gb_enb_id_count;		
    *failed_enb_count = temp_failed_enb_count;
    /* Coverity Fix 86872 end */

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    for ( num_peer_gnb = X2AP_NULL; num_peer_gnb < p_x2ap_oam_get_sctp_status_req->
        x2ap_oam_gb_gnb_status_list.num_peer_gnb; num_peer_gnb++ )
    {
        for ( counter = X2AP_NULL; counter < MAX_PEER_ENB; counter++ )
        {
            X2AP_TRACE(X2AP_WARNING, "Trying to get peer gnb context");

            if ( X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT &
                p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].bitmask )
            {
                if( !(X2AP_MEMCMP(&p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter].gb_gnb_id,
                                &(p_x2ap_oam_get_sctp_status_req->
                                    x2ap_oam_gb_gnb_status_list.gb_gnb_id[num_peer_gnb]), 
                                sizeof(x2_gb_gnb_id_t))) )
                {
                    p_enb_ctx = &p_x2ap_gb_ctx->peer_enb_cntxt_arr[counter];
                    X2AP_TRACE(X2AP_WARNING, "get peer gnb context");
                    break;
                }
            }
        }

        if ( PNULL == p_enb_ctx )
        {
            X2AP_TRACE(X2AP_WARNING, "peer gnb context is NULL");
            if( !(X2AP_OAM_GLOBAL_GNB_FAILED_LIST_PRESENT &
                p_x2ap_oam_get_sctp_status_resp->bitmask) )
            {
                p_x2ap_oam_get_sctp_status_resp->bitmask |=
                    X2AP_OAM_GLOBAL_GNB_FAILED_LIST_PRESENT; 
            }

            p_x2ap_oam_get_sctp_status_resp->gb_gnb_failed_list.
                failed_global_gnb_info[temp_failed_gnb_count].cause = 
                X2AP_OAM_GNB_ID_NOT_FOUND;

            X2AP_MEMCPY(&p_x2ap_oam_get_sctp_status_resp->gb_gnb_failed_list.
                failed_global_gnb_info[temp_failed_gnb_count].global_gnb_id,
                    &(p_x2ap_oam_get_sctp_status_req->
                        x2ap_oam_gb_gnb_status_list.gb_gnb_id[num_peer_gnb]),
                    sizeof(x2_gb_gnb_id_t));

            temp_failed_gnb_count++;
            continue;
        }
        if ( X2_SETUP_ACTIVE_ST != p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state )
        {
            X2AP_TRACE(X2AP_WARNING, "X2AP STATE is not Active");
            if( !(X2AP_OAM_GLOBAL_GNB_FAILED_LIST_PRESENT &
                p_x2ap_oam_get_sctp_status_resp->bitmask) )
            {
                p_x2ap_oam_get_sctp_status_resp->bitmask |=
                    X2AP_OAM_GLOBAL_GNB_FAILED_LIST_PRESENT; 
            }

            p_x2ap_oam_get_sctp_status_resp->gb_gnb_failed_list.
                failed_global_gnb_info[temp_failed_gnb_count].cause = 
                    X2AP_OAM_STATE_NOT_ACTIVE;

            X2AP_MEMCPY(&p_x2ap_oam_get_sctp_status_resp->gb_gnb_failed_list.
                failed_global_gnb_info[temp_failed_gnb_count].global_gnb_id,
                &(p_x2ap_oam_get_sctp_status_req->
                    x2ap_oam_gb_gnb_status_list.gb_gnb_id[num_peer_gnb]),
                sizeof(x2_gb_gnb_id_t));

            temp_failed_gnb_count++;
            continue;
        }
        else
        {
            if ( INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd )
            {
                temp_gb_gnb_id_count++;
                connSock = p_enb_ctx->sctp_sd; 
//#ifdef ARICENT_SCTP_IPR
#if 0
                if ( SCTP_FAILURE == sctp_api_enable_stats(stats_type,&error_code) )
                {
                    X2AP_TRACE(X2AP_WARNING,"Unable to set SCTP Stats:SCTP_ASSOC_STATS");
                    if ( PNULL != p_assoc_tab )
                    {
                        x2ap_mem_free(p_assoc_tab);
                    }
                    return X2AP_FAILURE;
                }					
                ret = sctpmib_get_assoc_table_var(
                        connSock,
                        SCTP_NULL,
                        SCTP_NULL,
                        p_assoc_tab,
                        &error_code);
#endif
//#else
#ifndef LINUX_PC_TEST
                memset_wrapper(&status, 0, sizeof(status));
                optlen = sizeof(status);
                return_type = getsockopt_wrapper(
                        connSock,
                        IPPROTO_SCTP,
                        SCTP_STATUS,
                        &status,
                        &optlen);
                spinfo = &status.sstat_primary;
                if ( AF_INET6 == spinfo->spinfo_address.ss_family ) 
                {
                    s_in6 = (struct sockaddr_in6 *)&spinfo->spinfo_address;
                        inet_ntop_wrapper(AF_INET6, &s_in6->sin6_addr, tempname,
                        sizeof (tempname));

                    /*getsockname_wrapper to fill in this socket's local address*/
                    socklen = sizeof (sa_in6);
                    if ( -1 == getsockname_wrapper((S32)connSock, (struct sockaddr *)&sa_in6, (socklen_t *)&socklen) ) 
                    {
                        X2AP_TRACE(X2AP_ERROR, "getsockname_wrapper failed");
                        return X2AP_FAILURE;
                    }
                    local_port = (int) ntohs_wrapper(sa_in6.sin6_port);
                    X2AP_TRACE(X2AP_INFO, "Local port is: %d\n", local_port);
                } 
                else if ( AF_INET == spinfo->spinfo_address.ss_family ) 
                {
                    s_in = (struct sockaddr_in *)&spinfo->spinfo_address;
                        inet_ntop_wrapper(AF_INET, &s_in->sin_addr, tempname, sizeof (tempname));

                    /*getsockname_wrapper to fill in this socket's local address*/
                    socklen = sizeof (sa_in);
                    if ( getsockname_wrapper((S32)connSock, (struct sockaddr *)&sa_in,
                        (socklen_t *)&socklen) == -1 ) 
                    {
                        X2AP_TRACE(X2AP_ERROR, "getsockname_wrapper failed");
                        return X2AP_FAILURE;
                    }
                    local_port = (int) ntohs_wrapper(sa_in.sin_port);
                    X2AP_TRACE(X2AP_INFO, "Local port is: %d\n", local_port);
                } 
                else 
                {
                    X2AP_TRACE(X2AP_ERROR, "ss_family is not found");
                    return X2AP_FAILURE;
                }
#endif
//#endif
//#ifdef ARICENT_SCTP_IPR
//                if ( SCTP_FAILURE == ret )
//#else
                if ( X2AP_NULL > return_type )
//#endif
                {
                    X2AP_TRACE(X2AP_ERROR,"SCTP : Unable to retreive sctp status parameters");
#if 0
#ifdef ARICENT_SCTP_IPR
                    if ( PNULL != p_assoc_tab )
                    {
                        x2ap_mem_free(p_assoc_tab);
                    }
#endif
#endif
                    return X2AP_FAILURE;
                }
                else
                {
                    if ( !(X2AP_OAM_GLOBAL_GNB_STATUS_PARAMS_LIST_PRESENT & 
                        p_x2ap_oam_get_sctp_status_resp->bitmask) )
                    {
                        p_x2ap_oam_get_sctp_status_resp->bitmask |= 
                            X2AP_OAM_GLOBAL_GNB_STATUS_PARAMS_LIST_PRESENT;
                    }
                    X2AP_MEMCPY(&p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_gnb].global_gnb_id,&p_enb_ctx->gb_gnb_id,
                        sizeof(x2_gb_gnb_id_t));
//#ifdef ARICENT_SCTP_IPR
#if 0
                    x2ap_oam_convert_sctp_mib_status(&p_assoc_tab->assoc_state,
                        &s1ap_sctp_status);
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_enb].status = s1ap_sctp_status;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_enb].in_streams = p_assoc_tab->assoc_in_streams;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_enb].out_streams = p_assoc_tab->assoc_out_streams;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_enb].local_port = p_assoc_tab->local_sctp_port;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_enb].start_time.tv_sec = 
                    (U32)p_assoc_tab->assoc_start_time.tv_sec;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_enb].start_time.tv_usec = 
                    (U32)p_assoc_tab->assoc_start_time.tv_usec;

                    /*
                     * Retrieving Primary Peer Address
                     */
                    if ( AF_INET6 == p_assoc_tab->rem_prim_add.family )
                    {
                        X2AP_TRACE(X2AP_INFO,"Family is AF_INET6");
                        sctp_sin6 = &p_assoc_tab->rem_prim_add;
                        inet_ntop_wrapper(AF_INET6, &(sctp_sin6->ip.v6.addr),str,INET_ADDRSTRLEN);
                        l3_memcpy_wrapper(&p_x2ap_oam_get_sctp_status_resp->
                            x2ap_oam_gb_gnb_status_params_list.staus_params[num_peer_gnb].
                                primary_peer_addr,str,
                            (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
                    }
                    else if ( AF_INET == p_assoc_tab->rem_prim_add.family )
                    {
                        X2AP_TRACE(X2AP_INFO,"Family is AF_INET");
                        sctp_sin = &p_assoc_tab->rem_prim_add;
                        in.s_addr = SCTP_HTONL(sctp_sin->ip.v4.addr);
                        l3_memcpy_wrapper(&p_x2ap_oam_get_sctp_status_resp->
                            x2ap_oam_gb_gnb_status_params_list.staus_params[num_peer_gnb].
                                primary_peer_addr,inet_ntoa_wrapper(in),
                            (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
                    }
                    else
                    {
                        X2AP_TRACE(X2AP_ERROR, "Unknown Family name");
                        if ( PNULL != p_assoc_tab )
                        {
                            x2ap_mem_free(p_assoc_tab);
                        }
                        return X2AP_FAILURE;
                    }
#endif
//#else
#ifndef LINUX_PC_TEST
                    if ( X2AP_NULL != status.sstat_state )
                    {
                        /*
                         * Update the status value to conform to the interface API document
                         */
                        status.sstat_state = status.sstat_state + 2;
                    }

                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_enb].status = status.sstat_state;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_enb].in_streams = status.sstat_instrms;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_enb].out_streams = status.sstat_outstrms;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_enb].local_port = local_port;
                    l3_memcpy_wrapper(&p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_enb].primary_peer_addr,tempname,
                        (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));

                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                        staus_params[num_peer_enb].start_time.tv_sec = 
                            p_x2ap_gb_ctx->sctp_start_time.tv_sec;

                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                            staus_params[num_peer_enb].start_time.tv_usec = 
                                p_x2ap_gb_ctx->sctp_start_time.tv_usec;
#endif
//#endif
                } 
            }
        }
    }

    if ( X2AP_OAM_GLOBAL_GNB_STATUS_PARAMS_LIST_PRESENT & 
        p_x2ap_oam_get_sctp_status_resp->bitmask )
    {
        p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
            num_peer_gnb = temp_gb_gnb_id_count;
        *gb_enb_id_count = temp_gb_gnb_id_count;		
    }

    if ( X2AP_OAM_GLOBAL_GNB_FAILED_LIST_PRESENT & 
        p_x2ap_oam_get_sctp_status_resp->bitmask )
    {
        p_x2ap_oam_get_sctp_status_resp->gb_gnb_failed_list.
            count = temp_failed_gnb_count;
        *failed_enb_count = temp_failed_gnb_count;
    }
    /* MENB CHANGES - END */
    #endif
#if 0
#ifdef ARICENT_SCTP_IPR
    if(PNULL != p_assoc_tab)
    {
        x2ap_mem_free(p_assoc_tab);
    }
#endif
#endif
    return X2AP_SUCCESS;
}
/*****************************************************************************
 * Function Name  : x2ap_build_oam_get_sctp_status_resp_without_bitmask 
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the link setup request
 *                  received from OAM module to open closed association 
 *                  with early associated ENB and send response back to OAM.
 ****************************************************************************/
x2ap_return_et x2ap_build_oam_get_sctp_status_resp_without_bitmask(
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
	x2ap_oam_get_sctp_status_resp_t  *p_x2ap_oam_get_sctp_status_resp,
	U8 *gb_enb_id_count)
{
    sctp_gb_enb_id_search_node_t  *p_gb_enb_id_node = X2AP_P_NULL;

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    sctp_gb_gnb_id_search_node_t  *p_gb_gnb_id_node = X2AP_P_NULL;
    /* MENB CHANGES - END */
    #endif

    U8 num_peer_enb     = 0;
    U8 temp_gb_enb_id_count  = 0;
    S32 connSock        = 0;

#ifdef ENDC_ENABLED
    /* MENB CHNAGES - START */
    U8 num_peer_gnb     = X2AP_NULL;
    /* MENB CHNAGES - END */
#endif

//#ifdef ARICENT_SCTP_IPR
#if 0
    sctpmib_assoc_table_st  *p_assoc_tab = PNULL;
    sctp_error_t             error_code = E_NOERROR;
    sctp_stats_et            stats_type = SCTP_ASSOC_STATS;
    U8 ret = 0;
    sctp_sockaddr_st *sctp_sin6;
    sctp_sockaddr_st *sctp_sin;
    U32 s1ap_sctp_status = RRC_NULL;
    struct in_addr in;
    /*struct in6_addr in6;*/
    char str[MAX_IP_ADDRESS_LENGTH];
#endif
//#else
    socklen_t optlen = 0;
    struct sctp_status  status;
    struct sctp_paddrinfo *spinfo;
    struct sockaddr_in6 *s_in6;
    struct sockaddr_in6 sa_in6;
    struct sockaddr_in *s_in;
    struct sockaddr_in sa_in;
    char tempname[MAX_X2AP_IP_ADDRESS_LENGTH];
    U16 local_port = 0;
    U32 socklen = RRC_NULL;
    S32 return_type  = 0;
//#endif
    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

//#ifdef ARICENT_SCTP_IPR
#if 0
    p_assoc_tab = (sctpmib_assoc_table_st*)
        x2ap_mem_get(sizeof(sctpmib_assoc_table_st));

    if (PNULL == p_assoc_tab)
    {
        X2AP_TRACE(X2AP_WARNING,"Unable to allocate memory");
        return X2AP_FAILURE;
    }
#endif
//#else
    memset_wrapper(&status, 0, sizeof(status));
//#endif
    /* Coverity Fix 86880 Start */
    for (num_peer_enb = 0; num_peer_enb < MAX_PEER_ENB ; num_peer_enb++)
    {
        #ifdef ENDC_ENABLED
        /* MENB CHNAGES - START */
        if ( X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT &
            p_x2ap_gb_ctx->peer_enb_cntxt_arr[num_peer_gnb].bitmask )
        {
            p_gb_gnb_id_node = 
                (sctp_gb_gnb_id_search_node_t *)x2ap_st_find_tree_node(
                        (const x2ap_search_tree_t *)(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                                gnb_id_search_tree)),
                        (const void *)(&(p_x2ap_gb_ctx->
                                peer_enb_cntxt_arr[num_peer_gnb].gb_gnb_id)));
            if ( PNULL == p_gb_gnb_id_node )
            {
                X2AP_TRACE(X2AP_INFO, "GNB ID Not found");
                continue;
            }

            if ( PNULL == p_gb_gnb_id_node->p_enb_context )
            {
                X2AP_TRACE(X2AP_INFO, "GNB Context Not found");
                continue;
            }

            if ( X2_SETUP_ACTIVE_ST != p_gb_gnb_id_node->p_enb_context->
                    x2_setup_sm_ctx.x2Setup_curr_state )
            {
                X2AP_TRACE(X2AP_WARNING, "X2AP STATE is not Active");
                continue;
            }
            else
            {
                if ( INVALID_SCTP_SOCKET != p_gb_gnb_id_node->p_enb_context->sctp_sd )
                {
                    temp_gb_enb_id_count ++;
                    connSock = p_gb_gnb_id_node->p_enb_context->sctp_sd; 
//#ifdef ARICENT_SCTP_IPR
#if 0
                    if ( SCTP_FAILURE == sctp_api_enable_stats(stats_type,&error_code) )
                    {
                        X2AP_TRACE(X2AP_WARNING,"Unable to set SCTP Stats:SCTP_ASSOC_STATS");
                        if ( PNULL != p_assoc_tab )
                        {
                            x2ap_mem_free(p_assoc_tab);
                        }
                        return X2AP_FAILURE;
                    }					
                    ret = sctpmib_get_assoc_table_var(
                            connSock,
                            SCTP_NULL,
                            SCTP_NULL,
                            p_assoc_tab,
                            &error_code);
#endif
//#else
                    memset_wrapper(&status, 0, sizeof(status));
                    optlen = sizeof(status);
                    return_type = getsockopt_wrapper(
                            connSock,
                            IPPROTO_SCTP,
                            SCTP_STATUS,
                            &status,
                            &optlen);
                    spinfo = &status.sstat_primary;
                    if ( AF_INET6 == spinfo->spinfo_address.ss_family ) 
                    {
                        s_in6 = (struct sockaddr_in6 *)&spinfo->spinfo_address;
                        inet_ntop_wrapper(AF_INET6, &s_in6->sin6_addr, tempname,
                                sizeof (tempname));

                        /*getsockname_wrapper to fill in this socket's local address*/
                        socklen = sizeof (sa_in6);
                        if ( -1 == getsockname_wrapper((S32)connSock, (struct sockaddr *)&sa_in6,
                                    (socklen_t *)&socklen) ) 
                        {
                            X2AP_TRACE(X2AP_ERROR, "getsockname_wrapper failed");
                            return X2AP_FAILURE;
                        }
                        local_port = (int) ntohs_wrapper(sa_in6.sin6_port);
                        X2AP_TRACE(X2AP_INFO, "Local port is: %d", local_port);
                    } 
                    else if ( AF_INET == spinfo->spinfo_address.ss_family ) 
                    {
                        s_in = (struct sockaddr_in *)&spinfo->spinfo_address;
                        inet_ntop_wrapper(AF_INET, &s_in->sin_addr, tempname, sizeof (tempname));

                        /*getsockname_wrapper to fill in this socket's local address*/
                        socklen = sizeof (sa_in);
                        if ( -1 == getsockname_wrapper((S32)connSock, (struct sockaddr *)&sa_in,
                                    (socklen_t *)&socklen) ) 
                        {
                            X2AP_TRACE(X2AP_ERROR, "getsockname_wrapper failed");
                            return X2AP_FAILURE;
                        }
                        local_port = (int) ntohs_wrapper(sa_in.sin_port);
                        X2AP_TRACE(X2AP_INFO, "Local port is: %d", local_port);
                    } 
                    else 
                    {
                        X2AP_TRACE(X2AP_ERROR, "ss_family is not matched");
                        return X2AP_FAILURE;
                    }

//#endif
//#ifdef ARICENT_SCTP_IPR
//                    if ( SCTP_FAILURE == ret )
//#else
                        if ( X2AP_NULL > return_type )
//#endif
                        {
                            X2AP_TRACE(X2AP_ERROR,"SCTP : Unable to retreive sctp status parameters");
#if 0
#ifdef ARICENT_SCTP_IPR
                            if ( PNULL != p_assoc_tab )
                            {
                                x2ap_mem_free(p_assoc_tab);
                            }
#endif
#endif
                            return X2AP_FAILURE;
                        }
                        else
                        {
                            if ( !(X2AP_OAM_GLOBAL_GNB_STATUS_PARAMS_LIST_PRESENT & 
                                        p_x2ap_oam_get_sctp_status_resp->bitmask) )
                            {
                                p_x2ap_oam_get_sctp_status_resp->bitmask |= 
                                    X2AP_OAM_GLOBAL_GNB_STATUS_PARAMS_LIST_PRESENT;
                            }
                            X2AP_MEMCPY(&p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                                    staus_params[num_peer_gnb].global_gnb_id,&p_gb_gnb_id_node->gb_gnb_id,
                                    sizeof(x2_gb_gnb_id_t));
//#ifdef ARICENT_SCTP_IPR
#if 0
                            x2ap_oam_convert_sctp_mib_status(&p_assoc_tab->assoc_state,
                                    &s1ap_sctp_status);
                            p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                                staus_params[num_peer_enb].status = s1ap_sctp_status;
                            p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                                staus_params[num_peer_enb].in_streams = p_assoc_tab->assoc_in_streams;
                            p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                                staus_params[num_peer_enb].out_streams = p_assoc_tab->assoc_out_streams;
                            p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                                staus_params[num_peer_enb].local_port = p_assoc_tab->local_sctp_port;
                            p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                                staus_params[num_peer_enb].start_time.tv_sec = 
                                (U32)p_assoc_tab->assoc_start_time.tv_sec;
                            p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                                staus_params[num_peer_enb].start_time.tv_usec = 
                                (U32)p_assoc_tab->assoc_start_time.tv_usec;
                            /*Retrieving Primary Peer Address */
                            if ( AF_INET6 == p_assoc_tab->rem_prim_add.family )
                            {
                                X2AP_TRACE(X2AP_INFO,"Family is AF_INET6");
                                sctp_sin6 = &p_assoc_tab->rem_prim_add;
                                inet_ntop_wrapper(AF_INET6, &(sctp_sin6->ip.v6.addr),str,INET_ADDRSTRLEN);
                                l3_memcpy_wrapper(&p_x2ap_oam_get_sctp_status_resp->
                                        x2ap_oam_gb_enb_status_params_list.staus_params[num_peer_enb].
                                        primary_peer_addr,str,
                                        (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
                            }
                            else if ( AF_INET == p_assoc_tab->rem_prim_add.family )
                            {
                                X2AP_TRACE(X2AP_INFO,"Family is AF_INET");
                                sctp_sin = &p_assoc_tab->rem_prim_add;
                                in.s_addr = SCTP_HTONL(sctp_sin->ip.v4.addr);
                                l3_memcpy_wrapper(&p_x2ap_oam_get_sctp_status_resp->
                                        x2ap_oam_gb_enb_status_params_list.staus_params[num_peer_enb].
                                        primary_peer_addr,inet_ntoa_wrapper(in),
                                        (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
                            }
                            else
                            {
                                X2AP_TRACE(X2AP_ERROR, "Unknown Family name");
                                if ( PNULL != p_assoc_tab )
                                {
                                    x2ap_mem_free(p_assoc_tab);
                                }
                                return X2AP_FAILURE;
                            }

#endif
//#else
                            if ( X2AP_NULL != status.sstat_state )
                            {
                                /*
                                 * Update the status value to conform to the interface API document
                                 */
                                status.sstat_state = status.sstat_state + 2;
                            }
                            p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                                staus_params[num_peer_gnb].status = status.sstat_state;
                            p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                                staus_params[num_peer_gnb].in_streams = status.sstat_instrms;
                            p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                                staus_params[num_peer_gnb].out_streams = status.sstat_outstrms;
                            p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                                staus_params[num_peer_gnb].local_port = local_port;
                            l3_memcpy_wrapper(&p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                                    staus_params[num_peer_gnb].primary_peer_addr,tempname,
                                    (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));

                            p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                                staus_params[num_peer_gnb].start_time.tv_sec = 
                                p_x2ap_gb_ctx->sctp_start_time.tv_sec;

                            p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
                                staus_params[num_peer_gnb].start_time.tv_usec = 
                                p_x2ap_gb_ctx->sctp_start_time.tv_usec;
//#endif
                        } 
                }
            }
        }
        else
        {
        /* MENB CHNAGES - END */
        #endif

        p_gb_enb_id_node = 
           (sctp_gb_enb_id_search_node_t *)x2ap_st_find_tree_node(
                   (const x2ap_search_tree_t *)(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                        enb_id_search_tree)),
                   (const void *)(&(p_x2ap_gb_ctx->
                        peer_enb_cntxt_arr[num_peer_enb].gb_enb_id)));
        if (PNULL == p_gb_enb_id_node)
        {
            X2AP_TRACE(X2AP_INFO, "ENB ID Not found");
            continue;
        }
        if (PNULL == p_gb_enb_id_node->p_enb_context)
        {
            X2AP_TRACE(X2AP_INFO, "ENB Context Not found");
            continue;
        }
        if(X2_SETUP_ACTIVE_ST != p_gb_enb_id_node->p_enb_context->x2_setup_sm_ctx.x2Setup_curr_state)
        {
            X2AP_TRACE(X2AP_WARNING, "X2AP STATE is not Active");
            continue;
        }
        else
        {
            if (INVALID_SCTP_SOCKET != p_gb_enb_id_node->p_enb_context->sctp_sd)
            {
                temp_gb_enb_id_count ++;
                connSock = p_gb_enb_id_node->p_enb_context->sctp_sd; 
//#ifdef ARICENT_SCTP_IPR
#if 0
                if(SCTP_FAILURE == sctp_api_enable_stats(stats_type,&error_code))
                {
                    X2AP_TRACE(X2AP_WARNING,"Unable to set SCTP Stats:SCTP_ASSOC_STATS");
                    if(PNULL != p_assoc_tab)
                    {
                        x2ap_mem_free(p_assoc_tab);
                    }
                    return X2AP_FAILURE;
                }					
                ret = sctpmib_get_assoc_table_var(
                       connSock,
                       SCTP_NULL,
                       SCTP_NULL,
                       p_assoc_tab,
                       &error_code);
#endif
//#else
                memset_wrapper(&status, 0, sizeof(status));
                optlen = sizeof(status);
                return_type = getsockopt_wrapper(
                       connSock,
                       IPPROTO_SCTP,
                       SCTP_STATUS,
                       &status,
                       &optlen);
                spinfo = &status.sstat_primary;
                if (spinfo->spinfo_address.ss_family == AF_INET6) 
                {
                    s_in6 = (struct sockaddr_in6 *)&spinfo->spinfo_address;
                            inet_ntop_wrapper(AF_INET6, &s_in6->sin6_addr, tempname,
                            sizeof (tempname));

                    /*getsockname_wrapper to fill in this socket's local address*/
                    socklen = sizeof (sa_in6);
                    if (getsockname_wrapper((S32)connSock, (struct sockaddr *)&sa_in6, (socklen_t *)&socklen) == -1) 
                    {
                                X2AP_TRACE(X2AP_ERROR, "getsockname_wrapper failed");
                                return X2AP_FAILURE;
                    }
                    local_port = (int) ntohs_wrapper(sa_in6.sin6_port);
                    X2AP_TRACE(X2AP_INFO, "Local port is: %d", local_port);
                } 
                else if (spinfo->spinfo_address.ss_family == AF_INET) 
                {
                    s_in = (struct sockaddr_in *)&spinfo->spinfo_address;
                    inet_ntop_wrapper(AF_INET, &s_in->sin_addr, tempname, sizeof (tempname));

                    /*getsockname_wrapper to fill in this socket's local address*/
                    socklen = sizeof (sa_in);
                    if (getsockname_wrapper((S32)connSock, (struct sockaddr *)&sa_in,(socklen_t *)&socklen) == -1) 
                    {
                        X2AP_TRACE(X2AP_ERROR, "getsockname_wrapper failed");
                        return X2AP_FAILURE;
                    }
                    local_port = (int) ntohs_wrapper(sa_in.sin_port);
                    X2AP_TRACE(X2AP_INFO, "Local port is: %d", local_port);
                } 
                else 
                {
                    X2AP_TRACE(X2AP_ERROR, "ss_family is not matched");
                    return X2AP_FAILURE;
                }

//#endif
//#ifdef ARICENT_SCTP_IPR
//                if (SCTP_FAILURE == ret)
//#else
                if (return_type < 0)
//#endif
                {
                    X2AP_TRACE(X2AP_ERROR,"SCTP : Unable to retreive sctp status parameters");
#if 0
#ifdef ARICENT_SCTP_IPR
                    if(PNULL != p_assoc_tab)
                    {
                        x2ap_mem_free(p_assoc_tab);
                    }
#endif
#endif
                    return X2AP_FAILURE;
                }
                else
                {
                    if(!(X2AP_OAM_GLOBAL_ENB_STATUS_PARAMS_LIST_PRESENT & 
                       p_x2ap_oam_get_sctp_status_resp->bitmask))
                    {
                        p_x2ap_oam_get_sctp_status_resp->bitmask |= 
                           X2AP_OAM_GLOBAL_ENB_STATUS_PARAMS_LIST_PRESENT;
                    }
                    X2AP_MEMCPY(&p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].global_enb_id,&p_gb_enb_id_node->gb_enb_id,
                            sizeof(x2_gb_enb_id_t));
//#ifdef ARICENT_SCTP_IPR
#if 0
                    x2ap_oam_convert_sctp_mib_status(&p_assoc_tab->assoc_state,
                        &s1ap_sctp_status);
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].status = s1ap_sctp_status;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].in_streams = p_assoc_tab->assoc_in_streams;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].out_streams = p_assoc_tab->assoc_out_streams;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].local_port = p_assoc_tab->local_sctp_port;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].start_time.tv_sec = 
                    (U32)p_assoc_tab->assoc_start_time.tv_sec;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                    staus_params[num_peer_enb].start_time.tv_usec = 
                       (U32)p_assoc_tab->assoc_start_time.tv_usec;
                    /*Retrieving Primary Peer Address */
                    if (p_assoc_tab->rem_prim_add.family == AF_INET6)
                    {
                        X2AP_TRACE(X2AP_INFO,"Family is AF_INET6");
                        sctp_sin6 = &p_assoc_tab->rem_prim_add;
                        inet_ntop_wrapper(AF_INET6, &(sctp_sin6->ip.v6.addr),str,INET_ADDRSTRLEN);
                        l3_memcpy_wrapper(&p_x2ap_oam_get_sctp_status_resp->
                        x2ap_oam_gb_enb_status_params_list.staus_params[num_peer_enb].
                            primary_peer_addr,str,
                            (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
                    }
                    else if (p_assoc_tab->rem_prim_add.family == AF_INET)
                    {
                        X2AP_TRACE(X2AP_INFO,"Family is AF_INET");
                        sctp_sin = &p_assoc_tab->rem_prim_add;
                        in.s_addr = SCTP_HTONL(sctp_sin->ip.v4.addr);
                        l3_memcpy_wrapper(&p_x2ap_oam_get_sctp_status_resp->
                            x2ap_oam_gb_enb_status_params_list.staus_params[num_peer_enb].
                            primary_peer_addr,inet_ntoa_wrapper(in),
                            (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
                    }
                    else
                    {
                        X2AP_TRACE(X2AP_ERROR, "Unknown Family name");
                        if(PNULL != p_assoc_tab)
                        {
                            x2ap_mem_free(p_assoc_tab);
                        }
                        return X2AP_FAILURE;
                    }

#endif
//#else
		    /* Bug 3826 Start */
		    if ( status.sstat_state != 0)
		    {
		        //Update the status value to conform to the interface API document.
		        status.sstat_state = status.sstat_state + 2;
		    }
		    /* Bug 3826 End */
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].status = status.sstat_state;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].in_streams = status.sstat_instrms;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].out_streams = status.sstat_outstrms;
                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].local_port = local_port;
                    l3_memcpy_wrapper(&p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].primary_peer_addr,tempname,
                           (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));

                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].start_time.tv_sec = 
                    p_x2ap_gb_ctx->sctp_start_time.tv_sec;

                    p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
                        staus_params[num_peer_enb].start_time.tv_usec = 
                            p_x2ap_gb_ctx->sctp_start_time.tv_usec;
//#endif
                } 
            }
        }
        #ifdef ENDC_ENABLED
        /* MENB CHNAGES - START */
        }
        /* MENB CHNAGES - END */
        #endif
        *gb_enb_id_count = temp_gb_enb_id_count;
    }
    if(X2AP_OAM_GLOBAL_ENB_STATUS_PARAMS_LIST_PRESENT & 
        p_x2ap_oam_get_sctp_status_resp->bitmask)
    {
        p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_enb_status_params_list.
        num_peer_enb = temp_gb_enb_id_count;
        *gb_enb_id_count = temp_gb_enb_id_count;
    }

#ifdef ENDC_ENABLED
    /* MENB CHNAGES - START */
    if ( X2AP_OAM_GLOBAL_GNB_STATUS_PARAMS_LIST_PRESENT & 
        p_x2ap_oam_get_sctp_status_resp->bitmask )
    {
        p_x2ap_oam_get_sctp_status_resp->x2ap_oam_gb_gnb_status_params_list.
            num_peer_gnb = temp_gb_enb_id_count;
        *gb_enb_id_count = temp_gb_enb_id_count;
    }
    /* MENB CHNAGES - END */
#endif

    /* Coverity Fix 86880 end */
#if 0
#ifdef ARICENT_SCTP_IPR
    if(PNULL != p_assoc_tab)
    {
        x2ap_mem_free(p_assoc_tab);
    }
#endif
#endif
    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_process_oam_get_sctp_status_req
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the link setup request
 *                  received from OAM module to open closed association 
 *                  with early associated ENB and send response back to OAM.
 ****************************************************************************/
void x2ap_process_oam_get_sctp_status_req(
        void                 *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{
    x2ap_return_et              response = X2AP_FAILURE;
    x2ap_oam_get_sctp_status_resp_t  x2ap_oam_get_sctp_status_resp ;
    x2ap_oam_get_sctp_status_req_t  *p_x2ap_oam_get_sctp_status_req = X2AP_P_NULL;

    U16 trans_id = 0;
    S32 length_read = 0;
    U8 gb_enb_id_count  = 0;
    U8 failed_enb_count = 0;

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    /* Get the transaction id */
    trans_id = x2ap_get_transaction_id(p_api);

    /* Check the state of x2ap */
    if (X2AP_ACTIVE_ST != p_x2ap_gb_ctx->x2ap_init_sm_blk.x2ap_current_state)
    {
        X2AP_TRACE(X2AP_WARNING, "X2ap is in active state but should \
                ideally wait for provisioning request");
        return;
    }

    p_x2ap_oam_get_sctp_status_req = (x2ap_oam_get_sctp_status_req_t*)
        x2ap_mem_get(sizeof(x2ap_oam_get_sctp_status_req_t));

    if (PNULL == p_x2ap_oam_get_sctp_status_req)
    {
        X2AP_TRACE(X2AP_WARNING, "Unable to allocate memory");
        return;
    }

    X2AP_MEMSET(&x2ap_oam_get_sctp_status_resp, X2AP_NULL,
            sizeof(x2ap_oam_get_sctp_status_resp_t));
    /* Decode the received message from OAM */
    response = rrc_il_parse_x2ap_oam_get_sctp_status_req(
            p_x2ap_oam_get_sctp_status_req,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            x2ap_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read);

    if (X2AP_SUCCESS == response)
    {
        if(X2AP_OAM_GLOBAL_ENB_STATUS_LIST_PRESENT & 
                p_x2ap_oam_get_sctp_status_req->bitmask)        
        { 
            if (X2AP_FAILURE == x2ap_build_oam_get_sctp_status_resp_with_bitmask(
                        p_x2ap_gb_ctx,
                        p_x2ap_oam_get_sctp_status_req,
                        &x2ap_oam_get_sctp_status_resp,
                        &gb_enb_id_count,
                        &failed_enb_count
                        ))
            {
                X2AP_TRACE(X2AP_WARNING, 
                        "x2ap_build_oam_get_sctp_status_resp_with_bitmask failed ");
                /* coverity_fix_63303_start */
                x2ap_mem_free(p_x2ap_oam_get_sctp_status_req);
                /* coverity_fix_63303_stop */
                return;
            }
        }
        #ifdef ENDC_ENABLED
        /* MENB CHANGES - START */
        else if ( X2AP_OAM_GLOBAL_GNB_STATUS_LIST_PRESENT & 
                p_x2ap_oam_get_sctp_status_req->bitmask )        
        { 
            if ( X2AP_FAILURE == x2ap_build_oam_get_sctp_status_resp_with_bitmask(
                        p_x2ap_gb_ctx,
                        p_x2ap_oam_get_sctp_status_req,
                        &x2ap_oam_get_sctp_status_resp,
                        &gb_enb_id_count,
                        &failed_enb_count
                        ) )
            {
                X2AP_TRACE(X2AP_WARNING, 
                        "x2ap_build_oam_get_sctp_status_resp_with_bitmask failed ");
                x2ap_mem_free(p_x2ap_oam_get_sctp_status_req);
                return;
            }
        }
        /* MENB CHANGES - END */
        #endif
        else
        {
            if (X2AP_FAILURE == x2ap_build_oam_get_sctp_status_resp_without_bitmask(
                        p_x2ap_gb_ctx,
                        &x2ap_oam_get_sctp_status_resp,
                        &gb_enb_id_count
                        ))
            {
                X2AP_TRACE(X2AP_WARNING, 
                        "x2ap_build_oam_get_sctp_status_resp_without_bitmask failed ");
                /* coverity_fix_63303_start */
                x2ap_mem_free(p_x2ap_oam_get_sctp_status_req);
                /* coverity_fix_63303_stop */
                return;
            }
        }
    }

    if(gb_enb_id_count > 0)
    {
        x2ap_oam_get_sctp_status_resp.response = X2AP_SUCCESS;
    }
    else if(failed_enb_count > 0)
    {
        x2ap_oam_get_sctp_status_resp.response = RRC_PARTIAL_SUCCESS;
    }
    else
    {
        x2ap_oam_get_sctp_status_resp.bitmask = RRC_NULL;
        x2ap_oam_get_sctp_status_resp.response = RRC_FAILURE;
    }


    /* Encode and send the get sctp status response to OAM */
    if(RRC_FAILURE == rrc_x2apOam_il_send_x2ap_oam_get_sctp_status_resp(
                &x2ap_oam_get_sctp_status_resp,
                RRC_X2AP_MODULE_ID,
                RRC_OAM_MODULE_ID,
                trans_id,
                RRC_NULL))
    {
        X2AP_TRACE(X2AP_WARNING, 
                "x2ap_oam_get_sctp_status_resp encode failed ");
    }
    else
    {
        X2AP_TRACE(X2AP_INFO, 
                "x2ap_oam_get_sctp_status_resp sent to OAM" );
    }


    /* coverity_fix_63303_start */
    x2ap_mem_free(p_x2ap_oam_get_sctp_status_req);
    /* coverity_fix_63303_stop */
    X2AP_UT_TRACE_EXIT();
    return;
}
/*sctp parameters stop */
/******************************************************************************
 *   FUNCTION NAME: x2ap_process_oam_proc_req 
 *   Inputs         :p_x2ap_gb_ctx - Pointer to the X2AP global context data
 *   DESCRIPTION:
 *       This function checks the status of x2ap global health monitoring variable
 *       based on it sends X2AP_OAM_PROC_SUPP_RESP message to OAM
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/

void x2ap_process_oam_proc_req
(
    void *p_api
    /*SPR 17777 +-*/
)
{
    U16 trans_id = RRC_NULL;

    X2AP_TRACE(X2AP_INFO,
            "[X2AP_OAM_PROCESS_PROC_REQ] RECEIVED = %u",x2ap_health_monitor_val);
    /* Get the transaction id */
    trans_id = rrc_get_transaction_id(p_api);
    x2ap_oam_send_proc_res(RRC_X2AP_MODULE_ID,trans_id);

/*Lockless Implementation start*/
#ifdef LOCKLESS_IMPL
    sync_fetch_and_and_wrapper(&x2ap_health_monitor_val,0,PNULL);
#else
    sync_fetch_and_and_wrapper(&x2ap_health_monitor_val,0,x2ap_lock);
#endif
/*Lockless Implementation stop*/

}

/******************************************************************************
 *   FUNCTION NAME: x2ap_oam_send_proc_res 
 *   DESCRIPTION:
 *       This function constructs and sends X2AP_OAM_PROC_SUPP_RESP message to OAM
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/

void x2ap_oam_send_proc_res
(
    U16 src_module_id,
    U16 trans_id
)
{
    U8* p_msg = PNULL;

    U16   total_msg_len = CV_HDRSIZE + RRC_INTERFACE_API_HEADER_SIZE;
    U16   msg_len = 0;


    p_msg = (U8*) rrc_msg_mem_get(total_msg_len);

    if (PNULL == p_msg)
    {
        X2AP_TRACE(X2AP_WARNING, "%s:rrc_msg_mem_get failed",
                __FUNCTION__);
        return;
    }

    /* Fill CSPL header */
    rrc_construct_api_header(
            p_msg,
            RRC_VERSION_ID,
            src_module_id,
            RRC_OAM_MODULE_ID,
            X2AP_OAM_PROC_SUPP_RESP,
            total_msg_len);

    /* Fill interface header */
    rrc_construct_interface_api_header(
            p_msg + RRC_API_HEADER_SIZE,
            trans_id,
            src_module_id,
            RRC_OAM_MODULE_ID,
            X2AP_OAM_PROC_SUPP_RESP,
            msg_len,
            RRC_NULL);

    /* Send message to */
    rrc_send_message(
            /*SPR 20172 Fix Start*/
            p_msg,
            /*SPR 20172 Fix End*/
            RRC_OAM_MODULE_ID);
}

/******************************************************************************
 *   FUNCTION NAME: start_x2ap_health_monitor_timer 
 *   DESCRIPTION:
 *       This function starts the x2ap health monitor
 *       timer provided by the user if the duration 
 *       of timer is greater than zero
 *       
 *   RETURNS:
 *       none
 ******************************************************************************/

void start_x2ap_health_monitor_timer
(
 /*SPR 17777 +-*/
    U32                  duration
)
{
    x2ap_timer_buffer_t timer_buf;
    x2ap_timer_t timer;
    X2AP_UT_TRACE_ENTER();

    /*SPR 17777 +-*/
    if(duration == 0)
    {
        return;
    }

    timer_buf.timer_type = X2AP_OAM_HEALTH_MONITOR_TIMER;
    timer_buf.auto_delete = X2AP_FALSE;
    timer = x2ap_start_timer(duration,
            &timer_buf,
            sizeof(timer_buf),
            X2AP_TRUE);
    if ( PNULL == timer )
    {
        X2AP_TRACE(X2AP_FATAL,"Timer start error ");
    }
    X2AP_UT_TRACE_EXIT();
}

