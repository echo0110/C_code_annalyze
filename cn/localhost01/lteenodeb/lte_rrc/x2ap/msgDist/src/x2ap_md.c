/*******************************************************************************
 *
 *  FILE NAME   : x2ap_msg_dist.c
 *
 *  DESCRIPTION : This file contains the definition of functions which will be
 *                used to dispatch messages to appropriate message handler. 
 *
 *  DATE 24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
 *
 *  Copyright 2007, Aricent Inc.
 *
 *******************************************************************************/

#include "x2ap_db.h"
#include "x2ap_md.h"
#include "x2ap_utils.h"
#include "x2ap_api.h"
#include "sm_entry.h"
#include "sctp_ipr.h"
#include "rrc_msg_mgmt.h"
#include "rrc_common_utils.h"
#include "x2ap_rsu_sm.h"
#include "x2ap_cell_activation.h"
/* SPR_14436_Fix Start */ 
#ifdef LKSCTP
#include "sctp_kernel.h"
#endif
#if 0
#ifdef ARICENT_SCTP_IPR
#include "sctp_ipr.h"
#endif
#endif
/* SPR_14436_Fix Stop */ 
/*SPR_16533_START*/
#include "asn_dec_wrappers.h"
#include "rrc_defines.h"
/*SPR_16533_END*/
#if 0
#ifdef ARICENT_SCTP_IPR
#include "s_uisusk.h"
#endif
#endif

#include <lteMisc.h>
#define MAX_PEER_ADDR 50
/******************************************************************************
  Private Definitions
 *****************************************************************************/


/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* GDB Fix - compiler warnings*/
extern void x2ap_process_son_messages(void* p_api, x2ap_gb_context_t   *p_x2ap_gb_ctx);
extern x2ap_return_et x2ap_enb_reset_timer_expiry_evt_hdl(x2ap_timer_buffer_t *p_timer_buf , x2ap_gb_context_t *p_x2ap_gb_ctx);
extern void x2ap_popu_sctp_conf_info_from_gb_db(
        x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info,
        x2ap_sctp_conf_info_t      *p_x2ap_sctp_conf_info);

static  void    x2ap_process_peer_enb_messages(               
                void                 *p_api,                
                x2ap_gb_context_t    *p_gb_ctx);

/*SPR_16533_START*/
#if 0				
#ifdef ARICENT_SCTP_IPR
static  void    x2ap_process_pend_conn_ind(
                x2ap_gb_context_t *p_gb_ctxt,
                sctp_sd_t         server_sd);
#endif
#endif
/*SPR_16533_END*/

static  void    x2ap_create_enb_contx_for_new_enb(
                x2ap_gb_context_t     *p_gb_db,
                x2_enb_comm_info_t    *p_enb_comm_info,
                sctp_sd_t             sctp_sd);
#ifdef LKSCTP
extern S32 x2ap_sctp_accept(
         S32 socket_desc,
         struct sockaddr ** pp_peerAddrs,
         U32 * p_addr_count,
/*spr8341 IPv6 fix start*/
         struct sockaddr_in6   **pp_peerAddr6,
         U8 addr_family);
/*spr8341 IPv6 fix end*/
#endif

extern x2ap_return_et x2ap_enb_mobility_change_timer_expiry_evt_hdl(
        x2ap_timer_buffer_t *p_timer_buf,
        x2ap_gb_context_t *p_x2ap_gb_ctx);

extern x2ap_return_et x2ap_mobility_change_rrm_guard_timer_expiry_evt_hdl(
        x2ap_timer_buffer_t *p_timer_buf,
        x2ap_gb_context_t *p_x2ap_gb_ctx);

/*SPR_18216_START*/
extern x2ap_return_et x2ap_build_x2setup_fail_resp(
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2ap_bool_et    timeTowait_present,
        rrc_x2ap_error_indication_t *p_error_indication,
        x2ap_error_ind_bool_t *p_send_err_ind );

#ifdef LKSCTP    
x2ap_return_et  x2ap_validate_x2setup_req(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        U8 *p_asn_msg,
        U16 asn_msg_len);
#endif
/*SPR_18216_END*/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern void x2ap_enb_config_update_response_timer_expiry(
                            x2ap_timer_buffer_t *p_timer_buf, 
                            x2ap_gb_context_t *p_x2ap_gb_ctx);

extern void x2ap_enb_config_update_time_to_wait_expiry(
                            x2ap_timer_buffer_t *p_timer_buf, 
                            x2ap_gb_context_t *p_x2ap_gb_ctx);

#ifdef LINUX_PC_TEST
// add by liaomin begin
extern U16 x2ap_get_peer_enb_streamnum_for_test(S32 sockSd);
// add by liaomin end
#endif

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/


#define X2AP_BUFFER_SHOULD_BE_RELEASED 1
/*SPR_16533_START*/
#ifdef LKSCTP

//#ifndef RRC_UNIT_TEST_FRAMEWORK  
static void x2ap_get_sctp_ip_addr(
        U8 *p_header, x2_enb_comm_info_t *enb_comm_info)
{
    X2AP_ASSERT(p_header != X2AP_P_NULL);
    /* SPR 20633 changes start */
    struct sockaddr_storage *from_addr = (struct sockaddr_storage *) p_header;
    /* SPR 20633 changes end */
    
    enb_comm_info->bitmask = 0;

    /* SPR 20633 changes start */
    if(from_addr->ss_family == AF_INET)
    {
        struct sockaddr_in *from_addr4 = (struct sockaddr_in *) from_addr;
        enb_comm_info->port = ntohs_wrapper(from_addr4->sin_port);
        enb_comm_info->num_ip_addr = 1;
        enb_comm_info->bitmask |= X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT;
        enb_comm_info->bitmask |= X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT;
        l3_strncpy_wrapper (((S8 *)enb_comm_info->ip_addr[0].ip_addr),
                (const S8 *)(inet_ntoa_wrapper(from_addr4->sin_addr)), 
                MAX_X2AP_IP_ADDRESS_LENGTH);
        X2AP_TRACE(X2AP_INFO, "IPv4 %s Port %d ",enb_comm_info->ip_addr[0].ip_addr, enb_comm_info->port);
    }
    else if(from_addr->ss_family == AF_INET6)
    {
         struct sockaddr_in6 *from_addr6 = (struct sockaddr_in6 *) from_addr;
         enb_comm_info->port = ntohs_wrapper(from_addr6->sin6_port);
         enb_comm_info->num_ipv6_addr = 1;
         enb_comm_info->bitmask |= X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT;
         enb_comm_info->bitmask |= X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT;

         inet_ntop_wrapper(AF_INET6,(const void *)&(from_addr6->sin6_addr),
                 ((char*)enb_comm_info->ipv6_addr[0].ipv6_addr),
                 MAX_X2AP_IPV6_ADDRESS_LENGTH);

        X2AP_TRACE(X2AP_INFO, "IPv6 %s Port %d ",enb_comm_info->ipv6_addr[0].ipv6_addr, enb_comm_info->port);
    }
    /* SPR 20633 changes end */
    return ;
}
//#endif
#endif
/*SPR_16533_END*/

/*****************************************************************************
 * Function Name  : x2ap_process_msg
 * Inputs         : p_api - The message received.
 *                  p_x2ap_gb_ctx - Pointer to X2AP global context.
 * Outputs        : None
 * Returns        : X2AP_BUFFER_SHOULD_BE_RELEASED -
 *                     Encapsulating shell should release the buffer.
 * Description    : This is the main entry point of X2AP module - CSPL 
 *                  callback. It receives all the external APIs received from 
 *                  external entities and internal X2AP entities and invokes 
 *                  appropriate handlers based on incoming module ID and API ID.
 *                  This function always returns nonzero value, i.e. the buffer 
 *                  is always released by the encapsulating shell.
 *****************************************************************************/
S32 x2ap_process_msg(
        void *p_api,        /* Pointer to input API buffer */
        void *p_gl_ctx)     /* Pointer to the X2AP global context data */
{
    x2ap_gb_context_t *p_x2ap_gb_ctx = X2AP_P_NULL;

    /* SPR 19273 Fix Start */
    SET_CELL_AND_UE_INDEX(0xff, 0xffff);
    /* SPR 19273 Fix End */


    /* Precondition check */
    /* Check general message integrity too */
    if ( (X2AP_P_NULL == p_api) || (X2AP_P_NULL == p_gl_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR, "x2ap_process_msg: "\
                "Either api or global ctx in NULL");
        /* Encapsulating shell should release the buffer */
        return X2AP_BUFFER_SHOULD_BE_RELEASED;
    }
    else if (RRC_SUCCESS != rrc_check_cspl_header((U8*)p_api))
    {
        X2AP_TRACE(X2AP_WARNING, "x2ap_process_msg: bad message header - "
                "ignore msg");
        /* Encapsulating shell should release the buffer */
        return X2AP_BUFFER_SHOULD_BE_RELEASED;
    }

    p_x2ap_gb_ctx = (x2ap_gb_context_t *)p_gl_ctx;

    switch (x2ap_get_src_module_id((U8*)p_api))
    {
        case RRC_OAM_MODULE_ID:
        {
        	if(((p_x2ap_gb_ctx->is_son_present) && (X2AP_OAM_PROC_SUPP_REQ != (x2ap_get_api_id((U8*)p_api)))) 
                    && ((X2AP_OAM_GET_LOG_LEVEL_REQ != (x2ap_get_api_id((U8*)p_api))) && (X2AP_OAM_SET_LOG_LEVEL_REQ != (x2ap_get_api_id((U8*)p_api))))
                    && ((X2AP_OAM_LOG_ENABLE_REQ != (x2ap_get_api_id((U8*)p_api))) && (X2AP_OAM_LOG_DISABLE_REQ != (x2ap_get_api_id((U8*)p_api))))
#ifdef LINUX_PC_TEST
                    && (X2AP_OAM_CLEANUP_REQ != (x2ap_get_api_id((U8*)p_api)))
#endif
                    )
        	{
        		X2AP_TRACE(X2AP_INFO, "SON module is present, X2AP will ignore APIs from OAM");
        	}
        	else
        	{
        		x2ap_process_oam_messages(p_api, p_x2ap_gb_ctx);
        	}
        }
            break;
        case RRC_X2AP_PEER_eNODEB_MODULE_ID:
            x2ap_process_peer_enb_messages(p_api, p_x2ap_gb_ctx);
            break;
        case RRC_CSC_MODULE_ID:
            x2ap_process_csc_messages(p_api, p_x2ap_gb_ctx);
            break;
        case RRC_RRM_MODULE_ID:
            x2ap_process_rrm_messages(p_api, p_x2ap_gb_ctx);
            break;
        /* CSR_00040799 Fix Start */
        case RRC_UECCMD_MODULE_ID:
        /* CSR_00040799 Fix End */
        case RRC_UECC_MODULE_ID:
            x2ap_process_uecc_messages(p_api, p_x2ap_gb_ctx);
            break;
        case RRC_SON_MODULE_ID:
            x2ap_process_son_messages(p_api, p_x2ap_gb_ctx);
            break;
        default:
            X2AP_TRACE(X2AP_WARNING, "x2ap_process_msg: API from unexpected "
                    "module received");
            break;
    }

    /* Encapsulating shell should release the buffer */
    return X2AP_BUFFER_SHOULD_BE_RELEASED;
}

/*****************************************************************************
 * Function Name  : x2ap_process_timer_msg
 * Inputs         : timer_id - The id of the timer expired.
 *                  p_timer_buf - Pointer to timer buffer which is allocated
 *                                when starting a timer.
 *                  p_gl_ctx - Pointer to X2AP global context.
 * Outputs        : None
 * Returns        : None
 * Description    : It handles all the timer expiry events for X2AP module.
 *       The timer buffer should always start with x2ap_timer_buffer_t
 *****************************************************************************/
void    x2ap_process_timer_msg(
        x2ap_timer_t timer_id,   /* Identity of the expired timer */
        void        *p_timer_buf,/* Pointer to data associated with the timer */
        void        *p_gl_ctx)   /* Pointer to the X2AP global context data */
{
    x2ap_gb_context_t   *p_x2ap_gb_ctx = X2AP_P_NULL;
    x2ap_timer_buffer_t *p_x2ap_timer_buf = X2AP_P_NULL;
    x2ap_peer_enb_context_t  *p_enb_context = X2AP_P_NULL;
    x2ap_bool_et is_auto_delete = X2AP_FALSE;
    x2_sctp_sd_search_node_t  *p_sd_tree_node = X2AP_P_NULL;
    /*SPR_16533_DELTA2_START*/
#ifdef LKSCTP
    sctp_ip_search_node_t    *p_ip_tree_node = X2AP_P_NULL;
#endif
    /*SPR_16533_DELTA2_END*/
    U32 loop = X2AP_NULL;
    /* Bug 219 Fix Start */
    U8 *peer_enodeb_id = X2AP_P_NULL;
    /* Bug 219 Fix Stop */

    /* Precondition check */
    X2AP_ASSERT(X2AP_P_NULL!= p_timer_buf);
    X2AP_ASSERT(X2AP_P_NULL!= p_gl_ctx);

    p_x2ap_gb_ctx = (x2ap_gb_context_t *)p_gl_ctx;
    p_x2ap_timer_buf = (x2ap_timer_buffer_t *)p_timer_buf;
    
    is_auto_delete = p_x2ap_timer_buf->auto_delete;
    /* Bug 219 Fix Start */
    if (X2AP_CSI_IND_GUARD_TIMER != p_x2ap_timer_buf->timer_type
            && X2AP_OAM_INIT_TIMER != p_x2ap_timer_buf->timer_type
            && X2AP_OAM_HEALTH_MONITOR_TIMER != p_x2ap_timer_buf->timer_type
            && X2AP_SCTP_CONNECT_TIMER != p_x2ap_timer_buf->timer_type)
    /* Bug 219 Fix Stop */
    {
        X2AP_TRACE(X2AP_BRIEF, "TimerId[%p] timer_type: %u", timer_id, p_x2ap_timer_buf->timer_type);
	    /*SPR_16533_DELTA2_START*/
#ifdef LKSCTP
        if(X2AP_X2_SETUP_RSP_TIMER == p_x2ap_timer_buf->timer_type ||
                X2AP_X2_SETUP_TIME_TO_WAIT_TIMER == p_x2ap_timer_buf->timer_type)
        {
            /* Get Tree Node */
            /* SPR 20633 changes start */
            if(g_sctp_server_fd_ipv6 == p_x2ap_timer_buf->sd)
            {
                p_ip_tree_node =  (sctp_ip_search_node_t *)x2ap_st_find_tree_node(
                        &(p_x2ap_gb_ctx->enb_ctxt_trees.ipv6_search_tree),
                        &(p_x2ap_timer_buf->ip_addr));
            }

            else
            {
                p_ip_tree_node =  (sctp_ip_search_node_t *)x2ap_st_find_tree_node(
                        &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
                        &(p_x2ap_timer_buf->ip_addr));
            }
            /* SPR 20633 changes end */
            if (X2AP_P_NULL != p_ip_tree_node)
            {
                /* Get ENB Context */
                p_enb_context = p_ip_tree_node->p_enb_context;

            }
            else
            {
                X2AP_TRACE(X2AP_ERROR, "x2ap_process_timer_msg: p_ip_tree_node is NULL");
                if (X2AP_TRUE == is_auto_delete)
                {
                    x2ap_mem_free(p_timer_buf);
                }
                return;   
            }
        }
        else
        {
#endif
            /* Get Tree Node */
            p_sd_tree_node =  (x2_sctp_sd_search_node_t*)x2ap_st_find_tree_node(
                    &(p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree),
                    &(p_x2ap_timer_buf->sd));

            if (X2AP_P_NULL != p_sd_tree_node)
            {   
                /* Get ENB Context */
                p_enb_context = p_sd_tree_node->p_enb_context;

            }   
            else
            {   
                X2AP_TRACE(X2AP_ERROR, "x2ap_process_timer_msg: p_sd_tree_node is NULL");
                if (X2AP_TRUE == is_auto_delete)
                {   
                    x2ap_mem_free(p_timer_buf);
                }   
                return;   
            } 
#ifdef LKSCTP  
        }
#endif
        /*SPR_16533_DELTA2_END*/
    }

    switch (p_x2ap_timer_buf->timer_type)
    {
        case X2AP_OAM_INIT_TIMER:
        {
            X2AP_TRACE(X2AP_BRIEF, "X2AP_OAM_INIT_TIMER expired");
	    if (p_x2ap_gb_ctx->x2ap_init_sm_blk.oam_init_ind_tim_id != 
                    X2AP_P_NULL)
            {
                x2ap_process_init_timer();
            }
            else
            {
                X2AP_TRACE(X2AP_ERROR, "[%s]: x2ap_process_timer_msg: Init Timer "
                        "Already Expired", __FUNCTION__);
            }
            break;
        }
        case X2AP_X2_SETUP_RSP_TIMER:
        {
            X2AP_TRACE(X2AP_WARNING, "X2AP_X2_SETUP_RSP_TIMER expired");
		x2setup_sm_entry(X2_SETUP_RESP_TMR_EXP_EV,
                    (U8*)p_x2ap_timer_buf, p_x2ap_gb_ctx, p_enb_context);
            break;
        }
        case X2AP_SCTP_CONNECT_TIMER:
        {
            X2AP_TRACE(X2AP_WARNING, "X2AP_SCTP_CONNECT_TIMER expired");
            /* Bug 219 Fix Start */
            peer_enodeb_id = (U8*)p_x2ap_timer_buf->p_timer_specific_data;
            p_enb_context = &(p_x2ap_gb_ctx->peer_enb_cntxt_arr[*peer_enodeb_id]);
            if (PNULL != p_enb_context)
            {
                x2ap_sctp_sm_entry(X2AP_SCTP_CONNECT_TMR_EXP_EV,
                    (U8*)p_x2ap_timer_buf, p_x2ap_gb_ctx, p_enb_context);
            }
            /* Bug 219 Fix Stop */
            break;
        }
        case X2AP_X2_SETUP_TIME_TO_WAIT_TIMER:
        {
		X2AP_TRACE(X2AP_INFO, "X2AP_X2_SETUP_TIME_TO_WAIT_TIMER expired");
            x2setup_sm_entry(X2_SETUP_TIME_WAIT_TMR_EXP_EV,
                    (U8*)p_x2ap_timer_buf, p_x2ap_gb_ctx, p_enb_context);

            break;
        }
        case X2AP_CSI_IND_GUARD_TIMER:
        {
            X2AP_TRACE(X2AP_INFO, "[%s]received X2AP_CSI_IND_GUARD_TIMER Exp",
                    __FUNCTION__);
            p_x2ap_gb_ctx->cell_up_guard_tim_id = X2AP_P_NULL;
            for (loop = 0; loop < p_x2ap_gb_ctx->num_peer_enb; ++loop)
            {
                x2setup_sm_entry(CELL_SETUP_IND_TIMER_EXP,
                       (U8*)p_x2ap_timer_buf, p_x2ap_gb_ctx,
                        &p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop]);
            }
            break;
        }   
        case X2AP_ENB_CONFIG_UPDATE_RSP_TIMER:
        {
            X2AP_TRACE(X2AP_WARNING,
                     "[%s]received  X2AP_ENB_CONFIG_UPDATE_RSP_TIMER expiry",
                     __FUNCTION__);

            if (X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT & p_enb_context->bitmask)
            {
                /* endc config update */
                x2ap_endc_config_update_response_timer_expiry(p_x2ap_timer_buf, p_x2ap_gb_ctx);
            }
            else
            {
                /* enb config update */
                x2ap_enb_config_update_response_timer_expiry(p_x2ap_timer_buf, p_x2ap_gb_ctx);
            }
            break;
        }   
        case X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER:
        {
            X2AP_TRACE(X2AP_INFO,
                     "[%s]received  X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER expiry",
                     __FUNCTION__);

            if (X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT & p_enb_context->bitmask)
            {
                /* endc config update */
                x2ap_endc_config_update_time_to_wait_expiry(p_x2ap_timer_buf, p_x2ap_gb_ctx);
            }
            else
            {
                /* enb config update */
                x2ap_enb_config_update_time_to_wait_expiry(p_x2ap_timer_buf, p_x2ap_gb_ctx);
            }
            break;
        }
        case X2AP_RSU_ENB_GUARD_TIMER:
        {
           X2AP_TRACE(X2AP_WARNING,
                    "[%s]received  X2AP_RSU_ENB_GUARD_TIMER expiry",
                    __FUNCTION__);
           x2ap_rsu_enb_timer_expiry_evt_hdl(p_x2ap_timer_buf, p_x2ap_gb_ctx);
 
           break;
        }
        case X2AP_RSU_RRM_GUARD_TIMER:
        {
           X2AP_TRACE(X2AP_WARNING,
                    "[%s]received  X2AP_RSU_RRM_GUARD_TIMER expiry",
                    __FUNCTION__);
           x2ap_rsu_rrm_timer_expiry_evt_hdl(p_x2ap_timer_buf, p_x2ap_gb_ctx);

           break;
        }
        case X2AP_ENB_RESET_TIMER:
        {
           X2AP_TRACE(X2AP_WARNING,
                    "[%s]received  X2AP_ENB_RESET_TIMER expiry",
                    __FUNCTION__);
           x2ap_enb_reset_timer_expiry_evt_hdl(p_x2ap_timer_buf, p_x2ap_gb_ctx);

           break;
        }
        case X2AP_CELL_ACTIVATION_TIMER:
        {
           X2AP_TRACE(X2AP_INFO,
                    "[%s]received  X2AP_CELL_ACTIVATION_TIMER expiry",
                    __FUNCTION__);
           x2ap_cell_activation_timer_expiry_hdl(p_x2ap_timer_buf, p_x2ap_gb_ctx);

           break;
        }

        case X2AP_ENB_MOBILITY_CH_TIMER:
        {
           X2AP_TRACE(X2AP_WARNING,
                    "[%s]received  X2AP_ENB_MOBILITY_CH_TIMER expiry",
                    __FUNCTION__);
           x2ap_enb_mobility_change_timer_expiry_evt_hdl(p_x2ap_timer_buf, p_x2ap_gb_ctx);

           break;
        }
        /*spr_fix*/
        case X2AP_MOBILITY_CH_RRM_GUARD_TIMER:
        {
            X2AP_TRACE(X2AP_WARNING,
                    "[%s]received  X2AP_MOBILITY_CH_RRM_GUARD_TIMER expiry",
                    __FUNCTION__);
            x2ap_mobility_change_rrm_guard_timer_expiry_evt_hdl(p_x2ap_timer_buf,
                    p_x2ap_gb_ctx);
            break;
        }
        
        case X2AP_CELL_ACT_RRM_GUARD_TIMER:
        {
           X2AP_TRACE(X2AP_WARNING,
                    "[%s]received  X2AP_CELL_ACT_RRM_GUARD_TIMER expiry",
                    __FUNCTION__);
           x2ap_cell_activation_rrm_guard_timer_expiry_hdl(p_x2ap_timer_buf, p_x2ap_gb_ctx);
           break;
        }
       case X2AP_OAM_HEALTH_MONITOR_TIMER:

            X2AP_TRACE(X2AP_DETAILED, "x2ap_process_timer_msg: X2AP_OAM_HEALTH_MONITOR_TIMER expired");
            rrc_update_health_monitor_variable(X2AP_HEALTH_MONITOR_EXPIRY_EVENT);
            break;
        /*spr_fix*/
        default:
        X2AP_TRACE(X2AP_ERROR, 
                "x2ap_process_timer_msg: timer event for "
                "unexpected timer type received");
        break;
    }

    if (X2AP_TRUE == is_auto_delete)
    {
        x2ap_mem_free(p_timer_buf);
    }

}
/*SPR_16533_START*/
#if 0		
#ifdef ARICENT_SCTP_IPR
/*****************************************************************************
 * Function Name  : x2ap_process_peer_enb_messages
 * Inputs         : p_api - Pointer to input API buffer 
 *              p_x2ap_gb_context - Pointer to the X2AP global context data
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called by x2ap message handler upon 
 *              the reception of eNB messages to process them, which
 *              then calls the internal procedures to process the 
 *                  received messages.
 ****************************************************************************/
void    x2ap_process_peer_enb_messages(
         void                 *p_api,                
         x2ap_gb_context_t    *p_x2ap_gb_context)
{
    x2ap_peer_enb_context_t    *p_enb_cntxt = X2AP_P_NULL;
    x2_sctp_sd_search_node_t      *p_sd_tree_node = X2AP_P_NULL;
    void                        *p_asn_msg = X2AP_P_NULL;
    sctp_sd_t                  sctp_sd = INVALID_SCTP_SOCKET;
    U16                         api_id = X2AP_NULL;
    U16                         asn_msg_len = X2AP_NULL;
    x2ap_peer_enb_msg_t        msg_buffer;

    /* SPR_14436_Fix Start */ 
     /* id of stream in an SCTP association between eNB and peer eNB.*/
    U16 stream_id  = MAX_NUM_X2AP_SCTP_STREAM;  
    /* SPR_14436_Fix Stop */ 

#ifdef LINUX_PC_TEST
// add by liaomin begin
    rrc_return_et retval = RRC_SUCCESS;
    test_sctp_message_t *p_test_sctp_message = X2AP_P_NULL;
    S32 length_read = X2AP_NULL;
// add by liaomin end
#endif


    X2AP_MEMSET(&msg_buffer, X2AP_NULL, sizeof(x2ap_peer_enb_msg_t));
    if (x2ap_get_api_buf_size((U8*)p_api) < RRC_API_HEADER_SIZE + 
           X2AP_SOCKET_DESC_SIZE)
    {
        X2AP_TRACE(X2AP_WARNING, "Wrong message size - ignore.");
        X2AP_UT_TRACE_EXIT();
        return;
    }

#ifdef LINUX_PC_TEST
// add by liaomin begin
    p_test_sctp_message = (test_sctp_message_t *)x2ap_mem_get(sizeof(test_sctp_message_t));
    if (X2AP_P_NULL == p_test_sctp_message)
    {
        X2AP_TRACE(X2AP_WARNING, "p_test_sctp_message - NULL.");
        return;
    }

    retval = rrc_il_parse_test_sctp_message(
    	p_test_sctp_message,
    	(U8*)p_api + RRC_API_HEADER_SIZE,
    	x2ap_get_api_buf_size((U8*)p_api) - RRC_API_HEADER_SIZE,
    	&length_read);
// add by liaomin end
#endif

    /* Get sctp socket descriptor */
#ifdef LINUX_PC_TEST
// add by liaomin begin
    sctp_sd = p_test_sctp_message->ulSocketID;
// add by liaomin end
#else
    sctp_sd = x2ap_get_sctp_sd((U32*)((U8*)p_api + RRC_API_HEADER_SIZE));
#endif
    if (INVALID_SCTP_SOCKET == sctp_sd)
    {
        X2AP_TRACE(X2AP_ERROR, "[%s], Invalid SD [%d] received in Message from "
                "Peer ENB.", __FUNCTION__, sctp_sd);
        return;
    }

    /* SPR_14436_Fix Start */ 
    /* get stream id */
#ifdef LINUX_PC_TEST
// add by liaomin begin
    stream_id = p_test_sctp_message->usStreamID;
    x2ap_mem_free (p_test_sctp_message);
// add by liaomin end
#else
    stream_id = x2ap_get_stream_id((U16*)((U8*)p_api + RRC_API_HEADER_SIZE +
                SCTP_SD_OCTET_SIZE));
#endif
    
    msg_buffer.stream_id = stream_id;    
    /* SPR_14436_Fix Stop */ 

    api_id = x2ap_get_api_id((U8*)p_api);
    if ( X2AP_SCTP_PEND_CONN_IND != api_id)
    {
        /* Get Tree Node */
        p_sd_tree_node =  (x2_sctp_sd_search_node_t*)x2ap_st_find_tree_node(
                &(p_x2ap_gb_context->enb_ctxt_trees.sd_search_tree),
                &sctp_sd);

        if (X2AP_P_NULL != p_sd_tree_node)        
        {
            /* Get ENB Context */
            p_enb_cntxt = p_sd_tree_node->p_enb_context;

        }
        else
        {
            X2AP_TRACE(
                    X2AP_WARNING,
                    "[%s] No node found for the sctp sd provided %d node count %d "
                    " tree address [0x%p]",
                    __FUNCTION__,
                    sctp_sd,
                    x2ap_st_get_node_count((&(p_x2ap_gb_context->enb_ctxt_trees.
                                sd_search_tree))),
                    &(p_x2ap_gb_context->enb_ctxt_trees.sd_search_tree));
            return;
        }            
    }
    
    switch (api_id)
    {
        case X2AP_SCTP_PEND_CONN_IND:
        {
            x2ap_process_pend_conn_ind(p_x2ap_gb_context,
                    sctp_sd);
            break;
        }           
        case X2AP_ENB_MESSAGE:
        {
            /* Get the ASN message */
            p_asn_msg = (void*)((U8*) p_api + RRC_API_HEADER_SIZE + 
                    SCTP_SD_OCTET_SIZE + X2AP_SCTP_STREAM_ID_SIZE);

            /* Get the asn msg length */
            asn_msg_len = (U16)(x2ap_get_api_buf_size((U8*)p_api) -
                    ( RRC_API_HEADER_SIZE + X2AP_SOCKET_DESC_SIZE + 
                      X2AP_SCTP_STREAM_ID_SIZE));

            msg_buffer.p_msg = p_asn_msg;
            msg_buffer.msg_len = asn_msg_len;
            x2ap_sctp_sm_entry(api_id, (U8*)&msg_buffer,
                    p_x2ap_gb_context, p_enb_cntxt);
            break;

        }
        case X2AP_SCTP_SHUT_DOWN_INIT_IND:
        case X2AP_SCTP_COMM_UP_IND:
        case X2AP_SCTP_CLOSED_IND:
        case X2AP_SCTP_CONN_RESTART_IND:
        case X2AP_SCTP_CONN_FAILURE_IND:
        {
            x2ap_sctp_sm_entry(api_id,
                (U8*)p_api, p_x2ap_gb_context,
                p_enb_cntxt);
            break;
        }
        default:
        {
            X2AP_TRACE(X2AP_ERROR, "Invalid APIID received!!");
            break;
        }
    }
    return;
}


#endif
#endif

/*SPR_16533_END*/
#ifdef LKSCTP
/*****************************************************************************
 * Function Name  : x2ap_process_peer_enb_messages
 * Inputs         : p_api - Pointer to input API buffer 
 *              p_x2ap_gb_context - Pointer to the X2AP global context data
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called by x2ap message handler upon 
 *              the reception of eNB messages to process them, which
 *              then calls the internal procedures to process the 
 *                  received messages.
 ****************************************************************************/
void    x2ap_process_peer_enb_messages(
         void                 *p_api,                
         x2ap_gb_context_t    *p_x2ap_gb_context)
{
    x2ap_peer_enb_context_t    *p_enb_cntxt = X2AP_P_NULL;
#if 0
#ifdef RRC_UNIT_TEST_FRAMEWORK
    x2ap_EP_et msg_type_ut_fw = X2SETUP_REQ;
    if (!p_x2ap_gb_context->peer_enb_cntxt_arr[0].is_enb_conf)
    {
        p_x2ap_gb_context->peer_enb_cntxt_arr[0].is_enb_conf = 1;
    }
    p_enb_cntxt = &p_x2ap_gb_context->peer_enb_cntxt_arr[0];
    p_enb_cntxt->peer_enodeb_id = 0;
    p_enb_cntxt->x2_setup_sm_ctx.x2Setup_curr_state = 1; //curr state
    p_enb_cntxt->x2_setup_sm_ctx.x2setup_retry_count = 3; //retry count
    p_enb_cntxt->x2ap_reset_sm_ctx.state = 1; //idle
    p_enb_cntxt->x2ap_reset_sm_ctx.retry_count = 3;
    p_enb_cntxt->x2ap_reset_sm_ctx.trans_id = 1;
#endif
#endif
    x2_sctp_sd_search_node_t      *p_sd_tree_node = X2AP_P_NULL;
    void                        *p_asn_msg = X2AP_P_NULL;
    sctp_sd_t                  sctp_sd = INVALID_SCTP_SOCKET;
    U16                         api_id = X2AP_NULL;
    U16                         asn_msg_len = X2AP_NULL;
    x2ap_peer_enb_msg_t        msg_buffer;

    /* SPR_14436_Fix Start */ 
     /* id of stream in an SCTP association between eNB and peer eNB.*/
//#ifndef RRC_UNIT_TEST_FRAMEWORK
    sctp_ip_search_node_t      *p_ip_node = X2AP_P_NULL;
    U16 stream_id  = MAX_NUM_X2AP_SCTP_STREAM;  
    socklen_t optlen = 0;
    U32                         loop = X2AP_NULL;
//#endif
    /* SPR_14436_Fix Stop */ 

/*SPR_16533_START*/
    struct sctp_status status;
    /* Coverity Fix 94886 Start */
    S32 ret_val = X2AP_NULL;
    /* Coverity Fix 94886 End */
    x2_enb_comm_info_t         enb_comm_info;
//#ifndef RRC_UNIT_TEST_FRAMEWORK
    optlen = sizeof(status);
//#endif
/*SPR_16533_END*/
    /*SPR_17274_START*/
    struct sctp_paddrparams paddr;
    /* SPR 20633 changes start */
    struct sockaddr *from_addr = PNULL;
    /* SPR 20633 changes end */
    /*SPR_17274_END*/

#ifdef LINUX_PC_TEST
    U16 stream_num = 0;
#endif

    X2AP_MEMSET(&msg_buffer, X2AP_NULL, sizeof(x2ap_peer_enb_msg_t));
    /* SPR 20633 :CID 111564 start */
    X2AP_MEMSET(&status, X2AP_NULL, sizeof(struct sctp_status));
    /* SPR 20633 :CID 111564 End */
    X2AP_MEMSET(&enb_comm_info, X2AP_NULL, sizeof(x2_enb_comm_info_t));
    if (x2ap_get_api_buf_size((U8*)p_api) < RRC_API_HEADER_SIZE + 
           X2AP_SOCKET_DESC_SIZE)
    {
        X2AP_TRACE(X2AP_WARNING, "Wrong message size - ignore.");
        X2AP_UT_TRACE_EXIT();
        return;
    }
    
//#ifndef RRC_UNIT_TEST_FRAMEWORK

    /* Get sctp socket descriptor */
    sctp_sd = x2ap_get_sctp_sd((U32*)((U8*)p_api + RRC_API_HEADER_SIZE));
    if (INVALID_SCTP_SOCKET == sctp_sd)
    {
        X2AP_TRACE(X2AP_ERROR, "[%s], Invalid SD [%d] received in Message from "
                "Peer ENB.", __FUNCTION__, sctp_sd);
        return;
    }

    /* SPR_14436_Fix Start */ 
    /* get stream id */
    stream_id = x2ap_get_stream_id((U16*)((U8*)p_api + RRC_API_HEADER_SIZE +
                SCTP_SD_OCTET_SIZE));
    
    msg_buffer.stream_id = stream_id;    
    /* SPR_14436_Fix Stop */ 
/*SPR_16533_START*/
    x2ap_get_sctp_ip_addr(((U8*)p_api + RRC_API_HEADER_SIZE + 
                SCTP_SD_OCTET_SIZE + SCTP_STREAM_ID_SIZE), 
                &enb_comm_info);
/*SPR_16533_END*/

    api_id = x2ap_get_api_id((U8*)p_api);
/*SPR_16533_START*/
        /* Get Tree Node */

    /* SPR 20633 changes start */
    X2AP_TRACE(X2AP_DETAILED, "g_sctp_server_fd_ipv4 = %d, g_sctp_server_fd_ipv6 = %d, sctp_sd = %d\n",g_sctp_server_fd_ipv4, g_sctp_server_fd_ipv6, sctp_sd);
    if(g_sctp_server_fd_ipv6 == sctp_sd || g_sctp_server_fd_ipv4 == sctp_sd)
    /* SPR 20633 changes end */
    {
        /* Notification has been recieved on the Server Socket Descriptor */
        /* search enb ip based tree */
        X2AP_TRACE(X2AP_DETAILED, "Notification has been recieved on the Server Socket Descriptor \n");
        /* SPR 20633 changes start */
        if(enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)//for ipv4 or ipv6
        {
            p_ip_node = (sctp_ip_search_node_t *)
                x2ap_st_find_tree_node(&p_x2ap_gb_context->enb_ctxt_trees.
                        ipv6_search_tree,  &enb_comm_info.ipv6_addr[0].ipv6_addr[0]);
        }
        else if(enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT)
        {
            p_ip_node = (sctp_ip_search_node_t *)
                x2ap_st_find_tree_node(&p_x2ap_gb_context->enb_ctxt_trees.
                        ip_search_tree,  &enb_comm_info.ip_addr[0].ip_addr[0]);
        }
        /* SPR 20633 changes end */
        if(p_ip_node != PNULL)
        {
            X2AP_TRACE(X2AP_DETAILED, " IP Node found in IP TREE\n");
            /* enb has been added in the ip based tree */
            p_enb_cntxt = p_ip_node->p_enb_context;
        }
        else
        {
            X2AP_TRACE(X2AP_WARNING, "IP Node NOT found in IP TREE\n");
            /* Notification has been recieved for an eNB not in context */
            if(api_id == X2AP_SCTP_COMM_UP_IND)
            {
                /* Insert new Node in IP Tree */
                p_enb_cntxt = x2ap_create_enb_contx_for_new_enb_in_ip_tree(
                        p_x2ap_gb_context,
                        &enb_comm_info, sctp_sd);
            }
            else
            {
                X2AP_TRACE(X2AP_WARNING, "Unexpected msg recieved on server socket\n");
                return ;
            }

        }

        if(p_enb_cntxt == PNULL)
        {
            X2AP_TRACE(X2AP_WARNING, "Peer enb context cannot be created \n");
            return ;
        }
    }
    else
    {
        p_sd_tree_node =  (x2_sctp_sd_search_node_t*)x2ap_st_find_tree_node(
                &(p_x2ap_gb_context->enb_ctxt_trees.sd_search_tree),
                &sctp_sd);

        if (X2AP_P_NULL != p_sd_tree_node)        
        {
            /* Get ENB Context */
            p_enb_cntxt = p_sd_tree_node->p_enb_context;
            X2AP_TRACE(
                    X2AP_DETAILED,
                    "p_enb_cntxt found");
        }
        else
        {
            X2AP_TRACE(X2AP_INFO, "Peer enb context not found for socket desc = %d\n",sctp_sd);
        }
    }
/*SPR_16533_END*/
//#else
//    api_id = x2ap_get_api_id((U8*)p_api);
//#endif   

    
    switch (api_id)
    {
        case X2AP_ENB_MESSAGE:
        {
            /* Get the ASN message */
            /* SPR 20633 changes start*/
            p_asn_msg = (void*)((U8*) p_api + RRC_API_HEADER_SIZE + 
/*SPR_16533_START*/
                        SCTP_SD_OCTET_SIZE + X2AP_SCTP_STREAM_ID_SIZE + sizeof(struct sockaddr_storage));
/*SPR_16533_END*/
            
            /* Get the asn msg length */
                asn_msg_len = (U16)(x2ap_get_api_buf_size((U8*)p_api) -
                    ( RRC_API_HEADER_SIZE + X2AP_SOCKET_DESC_SIZE + 
/*SPR_16533_START*/
                      X2AP_SCTP_STREAM_ID_SIZE + sizeof(struct sockaddr_storage)));
/*SPR_16533_END*/
            /* SPR 20633 changes end */
            msg_buffer.p_msg = p_asn_msg;
            msg_buffer.msg_len = asn_msg_len;

/*SPR_16533_START*/
            if(p_sd_tree_node == PNULL)
            {
                X2AP_TRACE(
                        X2AP_INFO,
                        "[%s] No node found for the sctp sd provided %d node count %d "
                        " tree address [0x%p]",
                        __FUNCTION__,
                        sctp_sd,
                        x2ap_st_get_node_count((&(p_x2ap_gb_context->enb_ctxt_trees.
                                    sd_search_tree))),
                        &(p_x2ap_gb_context->enb_ctxt_trees.sd_search_tree));


                x2ap_UE_X2AP_ID       oldEnbUeX2apId;
                x2ap_UE_X2AP_ID       newEnbUeX2apId;
#ifdef ENDC_ENABLED
   /*NR_DC Code Change Start*/
                U32    mEnbUeX2apId = RRC_INVALID_DC_UE_X2AP_ID;
                U32    sgnbUeX2apId = RRC_INVALID_DC_UE_X2AP_ID;
   /*NR_DC Code Change Stop*/
#endif
                x2ap_ProcedureCode    proc_code = X2AP_NULL;
                x2ap_EP_et msg_type = X2SETUP_REQ;
                /* SPR 20633 changes start */
//#ifndef RRC_UNIT_TEST_FRAMEWORK
                if(enb_comm_info.bitmask & 
                   X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
                {
                    p_ip_node = (sctp_ip_search_node_t *)
                        x2ap_st_find_tree_node(&p_x2ap_gb_context->enb_ctxt_trees.
                                ipv6_search_tree,  &enb_comm_info.ipv6_addr[0].ipv6_addr[0]);
                }
                else if(enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT)
                {
                p_ip_node = (sctp_ip_search_node_t *)
                    x2ap_st_find_tree_node(&p_x2ap_gb_context->enb_ctxt_trees.
                            ip_search_tree,  &enb_comm_info.ip_addr[0].ip_addr[0]);
                }
                /* SPR 20633 changes end */

                if(p_ip_node != PNULL)
                {
//#else
//                if(1)
//                {
//#endif
//#ifndef RRC_UNIT_TEST_FRAMEWORK
                    p_enb_cntxt = p_ip_node->p_enb_context;
                    if (X2_SETUP_ACTIVE_ST != p_enb_cntxt->x2_setup_sm_ctx.x2Setup_curr_state)   
                    {
//#endif
                    ret_val = x2ap_partially_decode_asn_msg (
                            p_x2ap_gb_context,p_enb_cntxt,
                            p_asn_msg, asn_msg_len,
                            &msg_type, &proc_code, &oldEnbUeX2apId,
                                &newEnbUeX2apId
#ifdef ENDC_ENABLED
   /*NR_DC Code Change Start*/
                                ,&mEnbUeX2apId,
                                &sgnbUeX2apId
   /*NR_DC Code Change Stop*/
#endif
                                );

                    if (X2AP_FAILURE == ret_val)
                    {
                        X2AP_TRACE (X2AP_DETAILED, "Decode failed for asn msg !!");
                        return;
                    }
#if 0
#ifdef RRC_UNIT_TEST_FRAMEWORK
                        msg_type_ut_fw = msg_type;
#endif
#endif

                    /*SPR_16533_DELTA2_START*/
                    if ((X2_SETUP_RESP == msg_type) || 
                            (X2SETUP_REQ == msg_type) || 
#ifdef ENDC_ENABLED
   /*NR_DC Code Change Start*/
                            (ENDC_X2SETUP_RESP == msg_type) ||
                            (ENDC_X2SETUP_REQ == msg_type) ||
                            (ENDC_X2SETUP_FAIL == msg_type) ||
   /*NR_DC Code Change Stop*/
#endif
                            (X2_SETUP_FAIL == msg_type))
                        /*SPR_16533_DELTA2_END*/
                    {
//#ifndef RRC_UNIT_TEST_FRAMEWORK
                            X2AP_TRACE (X2AP_DETAILED, "X2Setup Req or Resp"
                                        "recieved. org sd = %u and mod sd = %u"
                                        "node pointer = %p and ctxt pointer ="
                                        "%p", p_enb_cntxt->sctp_sd, sctp_sd,
                                        p_ip_node, p_enb_cntxt);
//#endif
                        /* create peer enb context in sd tree */
                        x2ap_create_enb_contx_for_new_enb_in_sd_tree(
                                p_x2ap_gb_context,p_enb_cntxt,sctp_sd);
                        /* Coverity Fix 94886 Start */
//#ifndef RRC_UNIT_TEST_FRAMEWORK
#ifdef LINUX_PC_TEST
                        stream_num = x2ap_get_peer_enb_streamnum_for_test(sctp_sd);
                        if (X2AP_NULL != stream_num)
                        {
                            p_enb_cntxt->stream_info.number_of_streams = stream_num;
                        }
                         X2AP_TRACE(X2AP_DETAILED, "No of streams = %d fo socket = %d",stream_num,sctp_sd);
#else
                        ret_val = getsockopt_wrapper(
                                    sctp_sd,
                                    SOL_SCTP,
                                    SCTP_STATUS,
                                    &status,
                                    &optlen);
                        if (-1 == ret_val)
                        {
                            X2AP_TRACE (X2AP_ERROR, "Failure returned by the function getsockopt_wrapper");
                        }
                        /* Coverity Fix 94886 End */
                        p_enb_cntxt->stream_info.number_of_streams = status.sstat_outstrms;
                        X2AP_TRACE(X2AP_DETAILED, "No of streams = %d fo socket = %d",status.sstat_outstrms,sctp_sd);
#endif
                        for (loop = 0; loop <  (p_enb_cntxt->stream_info).number_of_streams; loop++)
                        {
                            (p_enb_cntxt->stream_info).stream_ids[loop] = loop;
                        }
//#endif
                        /*SPR_18216_START*/
                        if(X2SETUP_REQ == msg_type)
                        {
                            if(X2AP_SUCCESS != 
                                    x2ap_validate_x2setup_req(p_x2ap_gb_context,p_enb_cntxt,
                                        p_asn_msg,asn_msg_len))
                            {
                                X2AP_TRACE(X2AP_DETAILED, "x2Setup request validation failed");
                                return;
                            }
                        }
                        /*SPR_18216_END*/
                    }
//#ifndef RRC_UNIT_TEST_FRAMEWORK
                    }
//#endif
                }
                else
                {
                    X2AP_TRACE(
                            X2AP_DETAILED,
                            "Creating new enb cntxt");
                    x2ap_create_enb_contx_for_new_enb(p_x2ap_gb_context,
                            &enb_comm_info, sctp_sd);
                }

                /*SPR_17153_START*/
                if( X2AP_OAM_DSCP_VALUE_PRESENT & 
                        p_x2ap_gb_context->config_param.sctp_conf_param.bitmask)
                {
                    if(!(enb_comm_info.bitmask & 
                                X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT))
                    {
                        U8 tos = 
                            p_x2ap_gb_context->config_param.sctp_conf_param.x2ap_dscp_value << 2;
                        if (0 > setsockopt_wrapper(sctp_sd, IPPROTO_IP,
                                    IP_TOS, &tos, 
                                    sizeof(tos)) ) 
                        {
                            /*unable to set socket options thus deleting created socket*/
                            X2AP_TRACE(X2AP_INFO," SCTP socket property DSCP Value  not set" 
                                    "[errno:%d]", errno);
                            /* SPR 20633 changes start */
                            if(sctp_sd != g_sctp_server_fd_ipv4)
                            /* SPR 20633 changes end */
                            {
                                X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
                                            sctp_sd);
                                x2ap_close_association(sctp_sd);
                                x2ap_delete_assoc_id(sctp_sd);
                            }
                            x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
                            p_node_to_delete = x2ap_st_find_tree_node(&(p_x2ap_gb_context->enb_ctxt_trees.
                                        sd_search_tree), &sctp_sd);
                            if (X2AP_P_NULL != p_node_to_delete)
                            {
                                /* Delete Socket Discriptor from Tree */
                                x2ap_st_delete_node(&(p_x2ap_gb_context->enb_ctxt_trees.sd_search_tree),
                                        p_node_to_delete);
                                x2ap_mem_free(p_node_to_delete);
                            }
                            
                            p_node_to_delete = x2ap_st_find_tree_node(
                                    &(p_x2ap_gb_context->enb_ctxt_trees.ip_search_tree),
                                    &(enb_comm_info.ip_addr[0].ip_addr));
                            if (X2AP_P_NULL != p_node_to_delete)
                            {
                                p_enb_cntxt = ((sctp_ip_search_node_t*)p_node_to_delete)->p_enb_context;
                                /*SPR_20743_FIX_START*/
                                x2ap_delete_peer_enb_from_search_tree(
                                        p_x2ap_gb_context,
                                        &(p_x2ap_gb_context->enb_ctxt_trees.ip_search_tree),
                                        p_enb_cntxt,
                                        p_node_to_delete);
                                /*SPR_20743_FIX_END*/

                            }
                        }
                        else 
                        {
                            X2AP_TRACE(X2AP_WARNING,
                                    " DSCP Value set: %d\n",
                                    p_x2ap_gb_context->config_param.sctp_conf_param.x2ap_dscp_value);
                        }
                    }
                    else
                    {
                        U32 on = 1, tclass = RRC_NULL;
                        if (0 > setsockopt_wrapper(sctp_sd, IPPROTO_IPV6, IPV6_RECVTCLASS, &on, sizeof(on)))
                        {
                            RRC_TRACE(RRC_WARNING,"  Error in option RECVTCLASS errno = %d, %s\n",
                                        errno,strerror(errno));
                        }
                        tclass = p_x2ap_gb_context->config_param.sctp_conf_param.x2ap_dscp_value << 2;
                        if (0 > setsockopt_wrapper(sctp_sd, IPPROTO_IPV6,
                                    IPV6_TCLASS, &tclass, 
                                    sizeof(tclass)) ) 
                        {
                            /*unable to set socket options thus deleting created socket*/
                            X2AP_TRACE(X2AP_INFO," SCTP socket property DSCP Value  not set for IPv6" 
                                    "[errno:%d]", errno);

                            /* SPR 20633 changes start */
                            if(sctp_sd != g_sctp_server_fd_ipv6)
                            /* SPR 20633 changes end */
                            {
                                X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
                                            sctp_sd);
                                x2ap_close_association(sctp_sd);
                                x2ap_delete_assoc_id(sctp_sd);
                            }
                            x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
                            p_node_to_delete = x2ap_st_find_tree_node(&(p_x2ap_gb_context->enb_ctxt_trees.
                                        sd_search_tree), &sctp_sd);
                            if (X2AP_P_NULL != p_node_to_delete)
                            {
                                /* Delete Socket Discriptor from Tree */
                                x2ap_st_delete_node(&(p_x2ap_gb_context->enb_ctxt_trees.sd_search_tree),
                                        p_node_to_delete);
                                x2ap_mem_free(p_node_to_delete);
                            }
                           /* SPR 20633 changes start */ 
                            p_node_to_delete = x2ap_st_find_tree_node(
                                    &(p_x2ap_gb_context->enb_ctxt_trees.ipv6_search_tree),
                                    &(enb_comm_info.ipv6_addr[0].ipv6_addr));
                            if (X2AP_P_NULL != p_node_to_delete)
                            {
                                p_enb_cntxt = ((sctp_ip_search_node_t*)p_node_to_delete)->p_enb_context;
                                /*SPR_20743_FIX_START*/
                                x2ap_delete_peer_enb_from_search_tree(
                                        p_x2ap_gb_context,
                                        &(p_x2ap_gb_context->enb_ctxt_trees.ipv6_search_tree),
                                        p_enb_cntxt,
                                        p_node_to_delete);
                                /*SPR_20743_FIX_END*/
                            }
                            /* SPR 20633 changes end */

                        }
                        else 
                        {
                            X2AP_TRACE(X2AP_WARNING,
                                    " DSCP Value set for IPv6: %d\n",
                                    p_x2ap_gb_context->config_param.sctp_conf_param.x2ap_dscp_value);
                        }

                    }
                }
                /*SPR_17153_END*/
                /*SPR_17274_START*/
                memset_wrapper(&paddr, 0, sizeof(struct sctp_paddrparams));
                /*SPR 20633 changes start */
                from_addr = (struct sockaddr *)((U8*)p_api + RRC_API_HEADER_SIZE +
                            SCTP_SD_OCTET_SIZE + SCTP_STREAM_ID_SIZE);
                /*SPR 20633 changes end */
                if(PNULL != from_addr)
                {
                    /* Coverity Fix 94884 start */
                    l3_memcpy_wrapper(&paddr.spp_address,
                            from_addr,sizeof(struct sockaddr_storage));
                    /* Coverity Fix 94884 end */
                }
#ifndef LINUX_PC_TEST   // add by liaomin

                /* Coverity Fix 94886 Start */
                /* SPR 20633: CID 111563 Start */
//#ifndef RRC_UNIT_TEST_FRAMEWORK
                if(INVALID_SCTP_SOCKET == sctp_sd)
                {
                /* SPR 20633: CID 111563 End */
                    ret_val = getsockopt_wrapper(
                                sctp_sd,
                                IPPROTO_SCTP,
                                SCTP_STATUS,
                                &status,	
                                &optlen);
                    if (-1 == ret_val)
                    {
                        X2AP_TRACE(X2AP_DETAILED, "Failure returned by the function getsockopt_wrapper");
                    }
                /* SPR 20633: CID 111563 Start */
                }
//#endif
                /* SPR 20633: CID 111563 End */
                /* Coverity Fix 94886 End */
                /* Set heartbeat Interval in milliseconds */
                paddr.spp_assoc_id = status.sstat_assoc_id;
				paddr.spp_hbinterval = 1000 * 
                                    (p_x2ap_gb_context->config_param.sctp_conf_param.hbinterval);
				paddr.spp_flags                   = SPP_HB_ENABLE;
                paddr.spp_pathmaxrxt = p_x2ap_gb_context->config_param.sctp_conf_param.pathmaxrxt;

                /* SPR 20633: CID 111563 Start */
//#ifndef RRC_UNIT_TEST_FRAMEWORK
                if(INVALID_SCTP_SOCKET == sctp_sd)
                {
                    /* SPR 20633: CID 111563 End */
				if(0 > setsockopt_wrapper( sctp_sd,SOL_SCTP , SCTP_PEER_ADDR_PARAMS,
						(const void *)&paddr, sizeof(paddr)))
                {
                    X2AP_TRACE(X2AP_INFO," SCTP socket property SCTP_PEER_ADDR_PARAMS not set" 
                            "[errno:%d]", errno);
                    /* SPR 20633 changes start */
                    if((sctp_sd != g_sctp_server_fd_ipv4) && (sctp_sd != g_sctp_server_fd_ipv6))
                    /* SPR 20633 changes end */
                    {
                        X2AP_TRACE(X2AP_INFO,"Closing X2 association for socket = %d",
                                sctp_sd);
                        x2ap_close_association(sctp_sd);
                        x2ap_delete_assoc_id(sctp_sd);
                    }
                    x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
                    p_node_to_delete = x2ap_st_find_tree_node(&(p_x2ap_gb_context->enb_ctxt_trees.
                                sd_search_tree), &sctp_sd);
                    if (X2AP_P_NULL != p_node_to_delete)
                    {
                        /* Delete Socket Discriptor from Tree */
                        x2ap_st_delete_node(&(p_x2ap_gb_context->enb_ctxt_trees.sd_search_tree),
                                p_node_to_delete);
                        x2ap_mem_free(p_node_to_delete);
                    }
                    /* SPR 20633 changes start */
                    if(enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
                    {
                        p_node_to_delete = x2ap_st_find_tree_node(
                                &(p_x2ap_gb_context->enb_ctxt_trees.ipv6_search_tree),
                                &(enb_comm_info.ipv6_addr[0].ipv6_addr));
                        if (X2AP_P_NULL != p_node_to_delete)
                        {
                            p_enb_cntxt = ((sctp_ip_search_node_t*)p_node_to_delete)->p_enb_context;
                            /*SPR_20743_FIX_START*/
                            x2ap_delete_peer_enb_from_search_tree(
                                    p_x2ap_gb_context,
                                    &(p_x2ap_gb_context->enb_ctxt_trees.ip_search_tree),
                                    p_enb_cntxt,
                                    p_node_to_delete);
                            /*SPR_20743_FIX_END*/

                        }
                    }
                    else
                    {
                        p_node_to_delete = x2ap_st_find_tree_node(
                                &(p_x2ap_gb_context->enb_ctxt_trees.ip_search_tree),
                                &(enb_comm_info.ip_addr[0].ip_addr));
                        if (X2AP_P_NULL != p_node_to_delete)
                        {
                            p_enb_cntxt = ((sctp_ip_search_node_t*)p_node_to_delete)->p_enb_context;
                            x2ap_st_delete_node(
                                    &(p_x2ap_gb_context->enb_ctxt_trees.ip_search_tree),
                                    p_node_to_delete);
                            x2ap_mem_free(p_node_to_delete);
                            p_enb_cntxt->is_enb_conf = 0; 
                        }
                    }
                    /* SPR 20633 changes end */
                }
                    /* SPR 20633: CID 111563 Start */
                }
//#endif 

#endif  // LINUX_PC_TEST
                /* SPR 20633: CID 111563 End */

                /*SPR_17274_END*/
            }       
            else         
            {
                /* Get ENB Context */
                p_enb_cntxt = p_sd_tree_node->p_enb_context;
                X2AP_TRACE(
                        X2AP_DETAILED,
                        "p_enb_cntxt found");
            }
/*SPR_16533_END*/

#if 0
#ifdef RRC_UNIT_TEST_FRAMEWORK
            if (!((X2_SETUP_RESP == msg_type_ut_fw) || 
                    (X2SETUP_REQ == msg_type_ut_fw) || 
                    (X2_SETUP_FAIL == msg_type_ut_fw)))
            {
                X2AP_TRACE(X2AP_DETAILED, "[RRC_UNIT_TEST_FRAMEWORK]:"
                        "State Changed from [%d] to [%d]",
                        p_enb_cntxt->x2_setup_sm_ctx.
                        x2Setup_curr_state,
                        X2_SETUP_ACTIVE_ST);

                p_enb_cntxt->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_ACTIVE_ST;
            }
#endif
#endif

            x2ap_sctp_sm_entry(api_id, (U8*)&msg_buffer,
                    p_x2ap_gb_context, p_enb_cntxt);
            break;

        }
/*SPR_16533_START*/
        case X2AP_SCTP_CONN_FAILURE_IND:
        {
            X2AP_TRACE(X2AP_INFO, "Recieved X2AP_SCTP_CONN_FAILURE_IND");

            /*SPR_16533_DELTA2_START*/
                if(p_enb_cntxt != X2AP_P_NULL)
                {
                        p_enb_cntxt->x2_setup_sm_ctx.x2Setup_curr_state = 
                            X2_SETUP_IDLE_ST;
                    x2ap_sctp_sm_entry(api_id, (U8*)&msg_buffer,
                            p_x2ap_gb_context, p_enb_cntxt);
                }
                else
                {
                    X2AP_TRACE(X2AP_ERROR, "Peer enb context not found");
                }
                /*SPR_16533_DELTA2_END*/
            break;
        }
        case X2AP_SCTP_COMM_UP_IND:
        {
            if(p_enb_cntxt != PNULL)
            {
                    X2AP_TRACE(X2AP_INFO, "X2AP_SCTP_COMM_UP_IND: changing sctp state to established");
                    p_enb_cntxt->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_ESTABLISHED_ST;
                    /*SPR_16533_DELTA2_START*/
                    if (X2AP_P_NULL != p_enb_cntxt->sctp_sm_ctx.conn_timer_id)
                    {
                        x2ap_stop_timer(p_enb_cntxt->sctp_sm_ctx.conn_timer_id);
                        p_enb_cntxt->sctp_sm_ctx.conn_timer_id = X2AP_P_NULL;
                    };
                    /*SPR_16533_DELTA2_END*/
            }
            break;
        }
        case X2AP_SCTP_SHUT_DOWN_INIT_IND:
        case X2AP_SCTP_CLOSED_IND:
        case X2AP_SCTP_CONN_RESTART_IND:
/*SPR_16533_END*/
	{
		/* Coverity_94883 Fix Start */
		if(p_enb_cntxt != X2AP_P_NULL)
		{

			x2ap_sctp_sm_entry(api_id,
					(U8*)p_api, p_x2ap_gb_context,
					p_enb_cntxt);
		}
		/* Coverity_94883 Fix End */

		break;
        }
        default:
        {
            X2AP_TRACE(X2AP_ERROR, "Invalid APIID received!!");
            break;
        }
    }
    return;
}
#endif
/*SPR_16533_START*/
#if 0
#ifdef ARICENT_SCTP_IPR
/*****************************************************************************
 * Function Name  : x2ap_process_pend_conn_ind
 * Inputs         : p_gb_ctxt - Pointer to the X2AP global context data
 *                  server_sd - Server SD
 * Outputs        : None 
 * Returns        : None
 * Description    : This function process pendiing connection event received 
 *                  from SCTP.
 ****************************************************************************/
void    x2ap_process_pend_conn_ind(
        x2ap_gb_context_t *p_gb_ctxt,
        sctp_sd_t         server_sd)
{

     struct sockaddr            *p_peer_addr[MAX_PEER_ADDR] = {X2AP_P_NULL};
     struct sockaddr_in         *p_addr = X2AP_P_NULL;
/*spr8341 IPv6 fix start*/     
     struct sockaddr_in6         *p_addr6 = X2AP_P_NULL;
     struct sockaddr_in6         *p_peer_addr6[MAX_PEER_ADDR] = {X2AP_P_NULL};
/*spr8341 IPv6 fix end*/
     sctp_sd_t                  sctp_sd = INVALID_SCTP_SOCKET;
     x2_enb_comm_info_t         enb_comm_info;
     sctp_ip_search_node_t      *p_ip_node = X2AP_P_NULL;
     x2_sctp_sd_search_node_t      *p_sd_search_node = X2AP_P_NULL;
     U32 loop = 0;
     U32 num_ip_addr =0;
     /* BUG 9804 Fix +*/
     /*dscp_marking_start*/
     U8 tos = RRC_NULL;
     U32 on = 1, tclass = RRC_NULL;
     /* BUG 9804 Fix -*/
     /*dscp_marking_stop*/
    /* SPR_14436_Fix Start */
    U8 counter = 0;
#ifdef LKSCTP
    struct sctp_status status;
    socklen_t optlen = 0;
#else
    sctp_initmsg_st initmsg;
    sctp_socklen_t optlen = 0;
#endif
    /* SPR_14436_Fix Stop */
     /* SPR 21960 Changes - Starts */
     x2ap_sctp_assoc_conf_info_t   sctp_conf_info;
     X2AP_MEMSET(&sctp_conf_info, X2AP_NULL, 
             sizeof(x2ap_sctp_assoc_conf_info_t));
  /* Populate SCTP Configuration Parameters from Global DB */
     x2ap_popu_sctp_conf_info_from_gb_db(&(sctp_conf_info),
                 &(p_gb_ctxt->config_param.sctp_conf_param));
     /* SPR 21960 Changes - Ends */

     X2AP_MEMSET(&enb_comm_info, X2AP_NULL, sizeof(x2_enb_comm_info_t));

/*spr8341 IPv6 fix start*/
     if(!(p_gb_ctxt->local_enb_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT))
     {
         sctp_sd = x2ap_sctp_accept(server_sd, &p_peer_addr[0], 
                 (U32 *)(&num_ip_addr),&p_peer_addr6[0],
                 AF_INET);
         if (INVALID_SCTP_SOCKET == sctp_sd)
         {
             X2AP_TRACE(X2AP_INFO, "[%s]: Invalid Socket Discriptor!!", __FUNCTION__);
             return;
         }
	
	 /* SPR 21960 Changes - Starts */
         sctp_conf_info.paddr.spp_address =
             *(sctp_sockaddr_storage_st *) p_peer_addr[0];
         /* SPR 21960 Changes - Ends */

	 /* SPR_14436_Fix Start */ 
#ifdef LKSCTP
         optlen = sizeof(status);
	 getsockopt_wrapper(
			 sctp_sd,
			 SOL_SCTP,
			 SCTP_STATUS,
			 &status,
			 &optlen);
#endif

#ifdef ARICENT_SCTP_IPR
         memset_wrapper(&initmsg, 0, sizeof(initmsg));
 /*Coverity_CID_88185_FIX_START*/
    if(SK_SUCCESS !=  sctp_getsockopt(
            sctp_sd,
            IPPROTO_SCTP,
            SCTP_SO_INITMSG,
            &initmsg,
            &optlen))
    {
        X2AP_TRACE(X2AP_ERROR, "sctp_getsockopt failed with errno :%d",errno);
    }
    /*Coverity_CID_88185_FIX_END*/


#endif
    /* SPR_14436_Fix Stop */ 
/* coverity_96703 start */
    /* BUG 9804 Fix +*/
    /*dscp_marking_start*/
    if( X2AP_OAM_DSCP_VALUE_PRESENT & p_gb_ctxt->config_param.sctp_conf_param.bitmask)
    {
	    tos = p_gb_ctxt->config_param.sctp_conf_param.x2ap_dscp_value << 2;
	    if (0 > setsockopt_wrapper(sctp_sd, IPPROTO_IP,
				    IP_TOS, &tos, 
				    sizeof(tos)) ) 
	    {
		    /*unable to set socket options thus deleting created socket*/
		    X2AP_TRACE(X2AP_INFO," Server socket property DSCP Value  not set" 
				    "[errno:%d]", errno);
		    if( close_wrapper(sctp_sd) == 0 ) 
		    {
			    /* association closed successfully */
			    /* Trace the successful closure of socket */
			    X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", sctp_sd);
		    }
		    else 
		    {
			    /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
			    X2AP_TRACE(X2AP_WARNING,
					    " Server socket close failure, errno: %d", errno);
		    }
	    }
	    else 
	    {
		    X2AP_TRACE(X2AP_WARNING,
				    " DSCP Value set: %d\n",p_gb_ctxt->config_param.sctp_conf_param.x2ap_dscp_value);
	    }
    }
    /*dscp_marking_stop*/
    /* BUG 9804 Fix -*/

    enb_comm_info.num_ip_addr = num_ip_addr;
    /* SPR 20633 Fix start */
    enb_comm_info.bitmask |= X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT;
    enb_comm_info.bitmask |= X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT;
    /* SPR 20633 Fix end */

    for (loop=0; (loop < enb_comm_info.num_ip_addr) ; ++loop)
    {
	    if(X2AP_P_NULL != (p_addr = (struct sockaddr_in*)(p_peer_addr[loop])))
	    {
		    l3_strncpy_wrapper (((S8 *)enb_comm_info.ip_addr[loop].ip_addr),
				    (const S8 *)(inet_ntoa_wrapper(p_addr->sin_addr)), 
				    MAX_X2AP_IP_ADDRESS_LENGTH);

		    X2AP_TRACE(X2AP_INFO, "Peer Client IP Address [%d]: %s",loop, 
				    enb_comm_info.ip_addr[loop].ip_addr);

		    enb_comm_info.port = ntohs_wrapper(p_addr->sin_port);

		    p_ip_node = (sctp_ip_search_node_t *)
			    x2ap_st_find_tree_node(&p_gb_ctxt->enb_ctxt_trees.
					    ip_search_tree,  &enb_comm_info.ip_addr[loop].ip_addr[0]);
		    if (p_ip_node != X2AP_P_NULL)
		    {
			    X2AP_TRACE(X2AP_INFO, 
					    "[%s] enb_comm_info.ip_addr[%d].ip_addr [%s], "
					    "Client IP Address Found in Configuration!!",
					    __FUNCTION__,
					    loop, enb_comm_info.ip_addr[loop].ip_addr);

			    break;
		    }
		    else
		    {
			    X2AP_TRACE(X2AP_INFO,
					    "[%s] enb_comm_info.ip_addr[%d].ip_addr [%s], "
					    "Client IP Address Not Found in Configuration!!", 
					    __FUNCTION__, loop, enb_comm_info.ip_addr[loop].ip_addr);
		    }
	    }
    }
     }
     else
     {
	     sctp_sd = x2ap_sctp_accept(server_sd, &p_peer_addr[0], 
			     (U32 *)(&num_ip_addr),&p_peer_addr6[0],
			     AF_INET6);
	     if (INVALID_SCTP_SOCKET == sctp_sd)
	     {
		     X2AP_TRACE(X2AP_INFO, "[%s]: Invalid Socket Discriptor!!", __FUNCTION__);
		     return;
	     }
        /* SPR 21960 Changes - Starts */
         sctp_conf_info.paddr.spp_address =
             *(sctp_sockaddr_storage_st *) p_peer_addr6[0];
        /* SPR 21960 Changes - Ends */

/* SPR 20633 Fix Start */        
#ifdef ARICENT_SCTP_IPR
        memset_wrapper(&initmsg, 0, sizeof(initmsg));
        /*Coverity_CID_88185_FIX_START*/
        if(SK_SUCCESS !=  sctp_getsockopt(
                    sctp_sd,
                    IPPROTO_SCTP,
                    SCTP_SO_INITMSG,
                    &initmsg,
                    &optlen))
        {
            X2AP_TRACE(X2AP_ERROR, "sctp_getsockopt failed with errno :%d",errno);
        }
        /*Coverity_CID_88185_FIX_END*/


#endif
/* SPR 20633 Fix End */

	     /* BUG 9804 Fix +*/
	     /*dscp_marking_start*/
	     if( X2AP_OAM_DSCP_VALUE_PRESENT & p_gb_ctxt->config_param.sctp_conf_param.bitmask)
	     {
		     if (0 > setsockopt_wrapper(sctp_sd, IPPROTO_IPV6, IPV6_RECVTCLASS, &on, sizeof(on)))
		     {
			     RRC_TRACE(RRC_WARNING,"  Error in option RECVTCLASS errno = %d, %s\n",errno,strerror(errno));
		     }
		     tclass = p_gb_ctxt->config_param.sctp_conf_param.x2ap_dscp_value << 2;
		     if (0 > setsockopt_wrapper(sctp_sd, IPPROTO_IPV6,
					     IPV6_TCLASS, &tclass, 
					     sizeof(tclass)) ) 
		     {
			     /*unable to set socket options thus deleting created socket*/
			     X2AP_TRACE(X2AP_INFO," Server socket property DSCP Value  not set for IPv6" 
					     "[errno:%d]", errno);
			     if( close_wrapper(sctp_sd) == 0 ) 
			     {
				     /* association closed successfully */
				     /* Trace the successful closure of socket */
				     X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", sctp_sd);
			     }
			     else 
			     {
				     /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
				     X2AP_TRACE(X2AP_WARNING,
						     " Server socket close failure, errno: %d", errno);
			     }
		     }
		     else 
		     {
			     X2AP_TRACE(X2AP_WARNING,
					     " DSCP Value set for IPv6: %d\n",p_gb_ctxt->config_param.sctp_conf_param.x2ap_dscp_value);
		     }
	     }
	     /*dscp_marking_stop*/
	     /* BUG 9804 Fix -*/

	     enb_comm_info.num_ipv6_addr = num_ip_addr;
        /* SPR 20633 Fix start */
        enb_comm_info.bitmask |= X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT;
        enb_comm_info.bitmask |= X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT;
        /* SPR 20633 Fix start */
	     for (loop=0; (loop < enb_comm_info.num_ipv6_addr) ; ++loop)
	     {
		     if(X2AP_P_NULL != (p_addr6 = (p_peer_addr6[loop])))
		     {
			     inet_ntop_wrapper(AF_INET6,(const void *)&p_addr6->sin6_addr,
					     ((char*)enb_comm_info.ipv6_addr[loop].ipv6_addr),
					     MAX_X2AP_IPV6_ADDRESS_LENGTH);

			     X2AP_TRACE(X2AP_INFO, "Peer Client IP Address [%d]: %s",loop, 
					     enb_comm_info.ipv6_addr[loop].ipv6_addr);

			     enb_comm_info.port = ntohs_wrapper(p_addr6->sin6_port);

			     p_ip_node = (sctp_ip_search_node_t *)
				     x2ap_st_find_tree_node(&p_gb_ctxt->enb_ctxt_trees.
						     ipv6_search_tree,  &enb_comm_info.ipv6_addr[loop].ipv6_addr[0]);
			     if (p_ip_node != X2AP_P_NULL)
			     {
				     X2AP_TRACE(X2AP_INFO, 
						     "[%s] enb_comm_info.ipv6_addr[%d].ipv6_addr [%s], "
						     "Client IPv6 Address Found in Configuration!!",
						     __FUNCTION__,
						     loop, enb_comm_info.ipv6_addr[loop].ipv6_addr);

				     break;
			     }
			     else
			     {
				     X2AP_TRACE(X2AP_INFO,
						     "[%s] enb_comm_info.ipv6_addr[%d].ipv6_addr [%s], "
						     "Client IPv6 Address Not Found in Configuration!!", 
						     __FUNCTION__, loop, enb_comm_info.ipv6_addr[loop].ipv6_addr);
			     }
		     }
	     }
     }
     /*spr8341 IPv6 fix end*/
    /* SPR 21960 Changes - Starts */
     if (sctp_setsockopt(sctp_sd, IPPROTO_SCTP,
        SCTP_PEER_ADDR_PARAMS, &(sctp_conf_info.paddr),
        sizeof(sctp_conf_info.paddr)) == -1) 
     {
         /*unable to set socket options thus deleting created socket*/
         X2AP_TRACE(X2AP_INFO," Server socket property SCTP_PEER_ADDR_PARAMS not set"
                 "[errno:%d]",sk_errno);
         X2AP_TRACE(X2AP_INFO," Default Server socket property Applied");
     }
     /* SPR 21960 Changes - Ends */

     /* Peer ENB is already in Configuration list */
     if (p_ip_node != X2AP_P_NULL)
     {
	     p_sd_search_node = (x2_sctp_sd_search_node_t*)
		     x2ap_mem_get(sizeof(x2_sctp_sd_search_node_t));
	     if(PNULL != p_sd_search_node)
	     {
		     p_sd_search_node->sctp_sd = sctp_sd;
		     p_sd_search_node->p_enb_context = p_ip_node->p_enb_context;
		     p_ip_node->p_enb_context->sctp_sd = sctp_sd;
		     x2ap_st_insert_node(&p_gb_ctxt->enb_ctxt_trees.sd_search_tree,
				     &(p_sd_search_node->nodeAnchor));
		     X2AP_TRACE(X2AP_INFO, "[%s] New Accepted SD [%d] Inserted in SD Tree",
				     __FUNCTION__, sctp_sd);
		     /* SPR_14436_Fix Start */
#ifdef LKSCTP
		     (p_ip_node->p_enb_context->stream_info).number_of_streams = status.sstat_outstrms;
#else
		     (p_ip_node->p_enb_context->stream_info).number_of_streams = initmsg.sinit_num_ostreams;
#endif
		     for (counter = 0; counter <  (p_ip_node->p_enb_context->stream_info).number_of_streams; counter++)
		     {
			     (p_ip_node->p_enb_context->stream_info).stream_ids[counter] = counter;
		     }
		     /* SPR_14436_Fix Stop */
		     /*coverirty_fix*/
		     x2ap_sctp_sm_entry(X2AP_SCTP_PENDING_CONN_EV,
				     (U8*)p_gb_ctxt, p_gb_ctxt,
				     p_ip_node->p_enb_context);
		     /*coverirty_fix*/
	     }
     }
     else
     {
	     x2ap_create_enb_contx_for_new_enb(p_gb_ctxt,
			     &enb_comm_info, sctp_sd);
     }
     for(loop = 0; loop < MAX_PEER_ADDR; loop++)
     {
#ifdef ARICENT_SCTP_IPR
	     sctp_freepaddrs(p_peer_addr[loop]);
#endif

#ifdef LKSCTP
	     sctp_freepaddrs_wrapper(p_peer_addr[loop]);
#endif

     }
     /*return;*/
}
#endif
#endif
/*SPR_16533_END*/
/* coverity_96703 stop */

/*****************************************************************************
 * Function Name  : x2ap_create_enb_contx_for_new_enb
 * Inputs         : p_x2ap_gb_ctx - Pointer to the X2AP global context data
 *                  p_enb_comm_info -  Pointer to x2_enb_comm_info_t
 *                  sctp_sd - Pointer to sctp_sd_t
 * Outputs        : Tree node
 * Returns        : None
 * Description    : This function process pending connection event received 
 *                  for enb not avaiable in configuration.
 ****************************************************************************/

static void    x2ap_create_enb_contx_for_new_enb(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2_enb_comm_info_t *p_enb_comm_info,
        sctp_sd_t          sctp_sd)
{
    x2ap_peer_enb_context_t    *p_peer_enb_cntxt = X2AP_P_NULL;
    sctp_ip_search_node_t      *p_tree_node = X2AP_P_NULL; 
    x2_sctp_sd_search_node_t      *p_sd_search_node = X2AP_P_NULL;  
    U32                         count = X2AP_NULL;
    U32                         indx = MAX_PEER_ENB;
    U32                         loop = X2AP_NULL;
    /* Coverity Fix 94880 Start */
#ifdef LKSCTP
    S32 ret_val = X2AP_NULL;
#endif
    /* Coverity Fix 94880 End */
    /*SPR_16660_START*/
    U8 counter = 0;
#ifdef LKSCTP
    struct sctp_status status;
    socklen_t optlen = 0;
#else
    sctp_initmsg_st initmsg;
    sctp_socklen_t optlen = 0;
#endif
    /*SPR_16660_STOP*/

#ifdef LINUX_PC_TEST
// add by liaomin begin
U16 stream_num = 0;
// add by liaomin end
#endif

    /* new ENB context Index */
    for(loop = 0; loop < MAX_PEER_ENB; loop ++)
    {
        if (!p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].is_enb_conf)
        {
            indx = loop;
            break;
        }
    }
    if (indx == MAX_PEER_ENB)
    {
        X2AP_TRACE(X2AP_INFO, "[%s] No free eNB Context found !!!",
                     __FUNCTION__);
        return;
    }

    p_peer_enb_cntxt = &(p_x2ap_gb_ctx->peer_enb_cntxt_arr[indx]);

    /*SPR_16784_START*/
    X2AP_MEMSET(p_peer_enb_cntxt, X2AP_NULL, sizeof(x2ap_peer_enb_context_t));
    /*SPR_16784_END*/

    X2AP_MEMCPY( (void*)&(p_peer_enb_cntxt->enb_comm_info), 
            (const void*)p_enb_comm_info, sizeof(x2_enb_comm_info_t)); 

    p_x2ap_gb_ctx->peer_enb_cntxt_arr[indx].sctp_sm_ctx.
        cur_sctp_sm_state = X2AP_SCTP_IDLE_ST;

    /* insert ip address with enbContext in tree */
/*spr8341 IPv6 fix start*/
/* SPR 20633 changes start */
     if((p_peer_enb_cntxt->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT) && (p_x2ap_gb_ctx->local_enb_info.bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT)) 
/* SPR 20633 fix end */
     {
         for (count = 0; count < p_peer_enb_cntxt->enb_comm_info.num_ip_addr;
                 ++count)
         {
             /*create tree node */
             p_tree_node = (sctp_ip_search_node_t*)x2ap_mem_get
                 (sizeof(sctp_ip_search_node_t));
             if (PNULL != p_tree_node)
             {
                 /* copy ip address in tree node */
                 l3_strncpy_wrapper ((S8 *)(p_tree_node->enb_ip_address),
                         (const S8 *)(p_x2ap_gb_ctx->peer_enb_cntxt_arr[indx].
                             enb_comm_info.ip_addr[count].ip_addr), 
                         MAX_X2AP_IP_ADDRESS_LENGTH);

                 p_tree_node->p_enb_context = p_peer_enb_cntxt;

                 /* insert in ip_port tree*/
                 x2ap_st_insert_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                             ip_search_tree), &(p_tree_node->nodeAnchor));
             }
         }
     }
     /* SPR 20633 Fix Start */
     else if((p_peer_enb_cntxt->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT) && (p_x2ap_gb_ctx->local_enb_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT))
     /* SPR 20633 Fix End */
     {
         for (count = 0; count < p_peer_enb_cntxt->enb_comm_info.num_ipv6_addr;
                 ++count)
         {
             /*create tree node */
             p_tree_node = (sctp_ip_search_node_t*)x2ap_mem_get
                 (sizeof(sctp_ip_search_node_t));
             if (PNULL != p_tree_node)
             {
                 /* copy ip address in tree node */
                 l3_strncpy_wrapper ((S8 *)(p_tree_node->enb_ipv6_address),
                         (const S8 *)(p_x2ap_gb_ctx->peer_enb_cntxt_arr[indx].
                             enb_comm_info.ipv6_addr[count].ipv6_addr),
                         MAX_X2AP_IPV6_ADDRESS_LENGTH);

                 p_tree_node->p_enb_context = p_peer_enb_cntxt;

                 /* insert in ip_port tree*/
                 x2ap_st_insert_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                             ipv6_search_tree), &(p_tree_node->nodeAnchor));
             }
         }
     }
/*spr8341 IPv6 fix end*/
    /* Insert SD In sd tree */
    p_sd_search_node = (x2_sctp_sd_search_node_t*)
                x2ap_mem_get(sizeof(x2_sctp_sd_search_node_t));
    
    p_peer_enb_cntxt->sctp_sd = sctp_sd;
    p_peer_enb_cntxt->is_enb_conf = X2AP_TRUE;

    /* BUG:596 start */
    p_peer_enb_cntxt->peer_enodeb_id = indx;
    /* BUG:596 stop */
    /*SPR_16660_START*/ 
#ifdef LKSCTP
#ifndef LINUX_PC_TEST   // add by liaomin
    optlen = sizeof(status);
    /* Coverity Fix 94880 Start */
    ret_val = getsockopt_wrapper(
                sctp_sd,
                SOL_SCTP,
                SCTP_STATUS,
                &status,
                &optlen);
    if (-1 == ret_val)
    {
         X2AP_TRACE(X2AP_ERROR, "Failure returned by the function getsockopt_wrapper");
    }
    /* Coverity Fix 94880 End */
#endif
#endif
#if 0
#ifdef ARICENT_SCTP_IPR
    memset_wrapper(&initmsg, 0, sizeof(initmsg));
    /*Coverity_CID_88500_FIX_START*/
    if(SK_SUCCESS !=  sctp_getsockopt(
            sctp_sd,
            IPPROTO_SCTP,
            SCTP_SO_INITMSG,
            &initmsg,
            &optlen))
    {
        X2AP_TRACE(X2AP_ERROR, "sctp_getsockopt failed with errno :%d",errno);
        initmsg.sinit_num_ostreams = 0;/* Resetting value for proper validation */
    }
    /*Coverity_CID_88500_FIX_END*/
#endif
#endif
#ifdef LKSCTP
#ifndef LINUX_PC_TEST
    if(status.sstat_outstrms  <  2 ||
            status.sstat_instrms <  2)
    {
        RRC_TRACE(RRC_WARNING,
                "SCTP : Less Number of streams have been opened");
    }
    else if (status.sstat_outstrms  >  10 )
    {
        RRC_TRACE(RRC_WARNING,"SCTP : More than 10 streams are open");
    }
    else
    {
        (p_peer_enb_cntxt->stream_info).number_of_streams = status.sstat_outstrms;
#else
        stream_num = x2ap_get_peer_enb_streamnum_for_test(sctp_sd);
        if (X2AP_NULL != stream_num)
            p_peer_enb_cntxt->stream_info.number_of_streams = stream_num;
#endif
        for (counter = 0; counter <  (p_peer_enb_cntxt->stream_info).number_of_streams; counter++)
        {
            (p_peer_enb_cntxt->stream_info).stream_ids[counter] = counter;
        }
#ifndef LINUX_PC_TEST
    }
#endif
#else
    if(initmsg.sinit_num_ostreams  <  2 ||
            initmsg.sinit_max_instreams <  2)
    {
        X2AP_TRACE(X2AP_WARNING,
                "SCTP : Less Number of streams have been opened");
    }
    else if (initmsg.sinit_num_ostreams  >  10 )
    {
        X2AP_TRACE(X2AP_WARNING,"SCTP : More than 10 streams are open");
    }
    else
    {
        (p_peer_enb_cntxt->stream_info).number_of_streams = initmsg.sinit_num_ostreams;
        for (counter = 0; counter <  (p_peer_enb_cntxt->stream_info).number_of_streams; counter++)
        {
            (p_peer_enb_cntxt->stream_info).stream_ids[counter] = counter;
        }
    }
#endif
    /*SPR_16660_STOP*/ 

    if (PNULL != p_sd_search_node)
    {
        p_sd_search_node->sctp_sd = sctp_sd;
        if (X2AP_P_NULL != p_tree_node)
        {
            p_sd_search_node->p_enb_context =  p_tree_node->p_enb_context;
        }
        x2ap_st_insert_node(&p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree,
                &(p_sd_search_node->nodeAnchor));
    }
    /*coverity_fix*/
/*SPR_16533_START*/
#if 0
#ifdef ARICENT_SCTP_IPR
    x2ap_sctp_sm_entry(X2AP_SCTP_PENDING_CONN_EV,
            (U8*)p_x2ap_gb_ctx, p_x2ap_gb_ctx,
            p_peer_enb_cntxt);
#endif
#endif
/*SPR_16533_END*/
    
    /*coverity_fix*/
    /* increase number of enb */
    p_x2ap_gb_ctx->num_peer_enb = p_x2ap_gb_ctx->num_peer_enb + 1;
    return;
}
/*SPR_16533_START*/
#ifdef LKSCTP
/*****************************************************************************
 * Function Name  : x2ap_create_enb_contx_for_new_enb_in_ip_tree 
 * Inputs         : p_x2ap_gb_ctx - Pointer to the X2AP global context data
 *                  p_enb_comm_info -  Pointer to x2_enb_comm_info_t
 *                  sctp_sd - socket desc
 * Outputs        : Tree node
 * Returns        : None
 * Description    : This function inserts a node in ip tree upon recieving INIT
 *                  from peer enb
 ****************************************************************************/

x2ap_peer_enb_context_t*   x2ap_create_enb_contx_for_new_enb_in_ip_tree(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2_enb_comm_info_t *p_enb_comm_info,
        sctp_sd_t          sctp_sd)
{
    x2ap_peer_enb_context_t    *p_peer_enb_cntxt = X2AP_P_NULL;
    sctp_ip_search_node_t      *p_tree_node = X2AP_P_NULL; 
    sctp_ip_search_node_t      *p_ip_node = X2AP_P_NULL;
    U32                         count = X2AP_NULL;
    U32                         indx = MAX_PEER_ENB;
    U32                         loop = X2AP_NULL;

    /* new ENB context Index */
    for(loop = 0; loop < MAX_PEER_ENB; loop ++)
    {
        if (!p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].is_enb_conf)
        {
            indx = loop;
            break;
        }
    }
    if (indx == MAX_PEER_ENB)
    {
        X2AP_TRACE(X2AP_INFO, "[%s] No free eNB Context found !!!",
                     __FUNCTION__);
        return PNULL;
    }

    p_peer_enb_cntxt = &(p_x2ap_gb_ctx->peer_enb_cntxt_arr[indx]);
    /*SPR_16533_DELTA2_START*/
    X2AP_MEMSET(p_peer_enb_cntxt, X2AP_NULL, sizeof(x2ap_peer_enb_context_t));
    /*SPR_16533_DELTA2_END*/
    X2AP_MEMCPY( (void*)&(p_peer_enb_cntxt->enb_comm_info), 
            (const void*)p_enb_comm_info, sizeof(x2_enb_comm_info_t)); 

    p_x2ap_gb_ctx->peer_enb_cntxt_arr[indx].sctp_sm_ctx.
        cur_sctp_sm_state = X2AP_SCTP_IDLE_ST;

    /* insert ip address with enbContext in tree */
/*spr8341 IPv6 fix start*/
/* SPR 20366 changes start */
     if(!((p_peer_enb_cntxt->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT) && (p_x2ap_gb_ctx->local_enb_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)))
/* SPR 20366 changes end */
     {
         for (count = 0; count < p_peer_enb_cntxt->enb_comm_info.num_ip_addr;
                 ++count)
         {

             p_ip_node = (sctp_ip_search_node_t *)
                 x2ap_st_find_tree_node(&p_x2ap_gb_ctx->enb_ctxt_trees.
                         ip_search_tree, p_x2ap_gb_ctx->peer_enb_cntxt_arr[indx].
                             enb_comm_info.ip_addr[count].ip_addr);
             if(PNULL == p_ip_node)
             {
                 /*create tree node */
                 p_tree_node = (sctp_ip_search_node_t*)x2ap_mem_get
                     (sizeof(sctp_ip_search_node_t));
                 if (PNULL != p_tree_node )
                 {
                     /* copy ip address in tree node */
                     l3_strncpy_wrapper ((S8 *)(p_tree_node->enb_ip_address),
                             (const S8 *)(p_x2ap_gb_ctx->peer_enb_cntxt_arr[indx].
                                 enb_comm_info.ip_addr[count].ip_addr), 
                             MAX_X2AP_IP_ADDRESS_LENGTH);

                     p_tree_node->p_enb_context = p_peer_enb_cntxt;

                     /* insert in ip_port tree*/
                     x2ap_st_insert_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                                 ip_search_tree), &(p_tree_node->nodeAnchor));
                     X2AP_TRACE(X2AP_DETAILED, "Peer enb context inserted in ip tree");        
                 }
             }
         }
     }
     else
     {
         for (count = 0; count < p_peer_enb_cntxt->enb_comm_info.num_ipv6_addr;
                 ++count)
         {
             p_ip_node = (sctp_ip_search_node_t *)
		  /* Coverity_94887 Fix Start */
                 x2ap_st_find_tree_node(&p_x2ap_gb_ctx->enb_ctxt_trees.
                         ipv6_search_tree, p_x2ap_gb_ctx->peer_enb_cntxt_arr[indx].
                             enb_comm_info.ipv6_addr[count].ipv6_addr);
		  /* Coverity_94887 Fix End */

             if(PNULL == p_ip_node)
             {
                 /*create tree node */
                 p_tree_node = (sctp_ip_search_node_t*)x2ap_mem_get
                     (sizeof(sctp_ip_search_node_t));
                 if (PNULL != p_tree_node)
                 {
                     /* copy ip address in tree node */
                     l3_strncpy_wrapper ((S8 *)(p_tree_node->enb_ipv6_address),
                             (const S8 *)(p_x2ap_gb_ctx->peer_enb_cntxt_arr[indx].
                                 enb_comm_info.ipv6_addr[count].ipv6_addr),
                             MAX_X2AP_IPV6_ADDRESS_LENGTH);

                     p_tree_node->p_enb_context = p_peer_enb_cntxt;

                     /* insert in ip_port tree*/
                     x2ap_st_insert_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                                 ipv6_search_tree), &(p_tree_node->nodeAnchor));
                     X2AP_TRACE(X2AP_DETAILED, "Peer enb context inserted in ip tree");        
                 }
             }
         }
     }
/*spr8341 IPv6 fix end*/
    
    p_peer_enb_cntxt->sctp_sd = sctp_sd;
    p_peer_enb_cntxt->is_enb_conf = X2AP_TRUE;

    /* BUG:596 start */
    p_peer_enb_cntxt->peer_enodeb_id = indx;
    /* BUG:596 stop */

    /* increase number of enb */
    p_x2ap_gb_ctx->num_peer_enb = p_x2ap_gb_ctx->num_peer_enb + 1;
    return p_peer_enb_cntxt;
}

/*****************************************************************************
 * Function Name  : x2ap_create_enb_contx_for_new_enb_in_sd_tree 
 * Inputs         : p_x2ap_gb_ctx - Pointer to the X2AP global context data
 *                  p_peer_enb_cntxt -  Pointer to x2ap_peer_enb_context_t
 *                  sctp_sd - socket desc.
 * Outputs        : Tree node
 * Returns        : None
 * Description    : This function inserts a node in sd tree for a peer enb upon
 *                  recieving X2 setup req/resp (first msg from peer enb)
 ****************************************************************************/

void    x2ap_create_enb_contx_for_new_enb_in_sd_tree(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_peer_enb_cntxt,
        sctp_sd_t          sctp_sd)
{
    x2_sctp_sd_search_node_t      *p_sd_search_node = X2AP_P_NULL;  
    x2_sctp_sd_search_node_t      *p_sd_tree_node = X2AP_P_NULL;

    X2AP_ASSERT(p_peer_enb_cntxt != X2AP_P_NULL);

    /* Coverity_94890 Fix Start */
    if (X2AP_P_NULL != p_peer_enb_cntxt)
    {
	    p_peer_enb_cntxt->sctp_sd = sctp_sd;
    }
    else
    {
	    X2AP_TRACE(X2AP_ERROR, "Peer enb context not found");
            return;
    }
    /* Coverity_94890 Fix End */

    p_sd_tree_node =  (x2_sctp_sd_search_node_t*)x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree),
            &sctp_sd);

    if(p_sd_tree_node == PNULL)
    {
        /* Insert SD In sd tree */
        p_sd_search_node = (x2_sctp_sd_search_node_t*)
            x2ap_mem_get(sizeof(x2_sctp_sd_search_node_t));


        if (PNULL != p_sd_search_node)
        {
            p_sd_search_node->sctp_sd = sctp_sd;
	    /* Coverity_94890 Fix Start */
                p_sd_search_node->p_enb_context =  p_peer_enb_cntxt;
	   /* Coverity_94890 Fix End */
            x2ap_st_insert_node(&p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree,
                    &(p_sd_search_node->nodeAnchor));
            X2AP_TRACE(X2AP_DETAILED, "Peer enb context inserted in sd tree");        
        }
    }
    return;
}
#endif
/*SPR_16533_END*/
/*SPR_18216_START*/
#ifdef LKSCTP    
/******************************************************************************
 * FUNCTION NAME  : x2ap_validate_x2setup_req 
 * INPUTS         : p_x2ap_gb_ctxt - pointer to X2AP global context
 *                  p_enb_ctx, - pointer to peer_enb_context
 *                  p_asn_msg, - pointer to asn message
 *                  asn_msg_len -  asn message length
 * OUTPUTS        : NONE
 * RETURNS        : X2AP_SUCCESS/X2AP_FAILURE
 * DESCRIPTION    : This function validates the x2Setup req recieved before
 *                  inserting the context into enbid search tree
 ******************************************************************************/

x2ap_return_et  x2ap_validate_x2setup_req(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        U8          *p_asn_msg,
        U16         asn_msg_len)
{
    x2ap_return_et        retVal = X2AP_SUCCESS;
    /* SPR 19078: Coverity Fix 107831 Start */
    S32                   ret_val = X2AP_NULL;
    /* SPR 19078: Coverity Fix 107831 End */
    /* SPR 19078: Coverity Fix 101195 Start */
    x2_setup_req_t        *p_x2_setup_req;
    /* SPR 19078: Coverity Fix 101195 End */
    rrc_x2ap_error_indication_t error_indication;
    x2ap_error_ind_bool_t        send_err_ind;
    x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
    U32 msg_len = asn_msg_len;
    /* SPR 19067 Fix Start */
    OSCTXT                     asn1_ctx;
    /* SPR 19067 Fix Stop */
    /* SPR 19078: Coverity Fix 101195 Start */
    p_x2_setup_req = (x2_setup_req_t *)
                 x2ap_mem_get(sizeof(x2_setup_req_t));
    if (X2AP_P_NULL == p_x2_setup_req)
    {
         X2AP_TRACE(X2AP_DETAILED,"Memory Allocation failure");
         return X2AP_FAILURE;
    }
    X2AP_MEMSET(p_x2_setup_req, X2AP_NULL, sizeof(x2_setup_req_t));
    /* SPR 19078: Coverity Fix 101195 Fix End */
    X2AP_MEMSET(&error_indication,X2AP_NULL,
            sizeof(rrc_x2ap_error_indication_t));
    X2AP_MEMSET(&send_err_ind,X2AP_NULL,
            sizeof(x2ap_error_ind_bool_t));

    /* Decode the ASN request */
    retVal = x2ap_decode_x2setup_req(
            p_x2ap_gb_ctx,
            p_enb_ctx,
            p_x2_setup_req,/* SPR 19078: Coverity Fix 101195 */
            p_asn_msg,
            &msg_len,
            &error_indication,
            /* SPR 19067 Fix Start */
            &send_err_ind,
            &asn1_ctx);
            /* SPR 19067 Fix Stop */


    if (X2AP_SUCCESS == retVal)
    {
        x2ap_peer_enb_context_t *p_old_peer_enb_ctx = X2AP_P_NULL;

        /*Check for duplicate entry in enbid search tree*/
        p_old_peer_enb_ctx = x2ap_get_peer_enb_ctxt_from_enbid(
                p_x2ap_gb_ctx,
                p_x2_setup_req->global_enb_id);/* SPR 19078: Coverity Fix 101195 */

        if(X2AP_P_NULL != p_old_peer_enb_ctx)
        {
            struct sctp_status status;
            socklen_t optlen = 0;
            optlen = sizeof(status);

            X2AP_TRACE(X2AP_WARNING,"Another peer enb is already present with the "
                    "enbid recieved in X2Setup req.\n");
            /* SPR 19078: Coverity Fix 101195 Fix Start */
            if(p_x2_setup_req->global_enb_id.enb_id.presence_bitmask & X2AP_OAM_HOME_ENB_ID_PRESENT)
            {
                X2AP_TRACE(X2AP_DETAILED, "Home eNB ID recieved in x2Setup req: eNB_id[0]:%d "
                        "eNB_id[1]:%d eNB_id[2]:%d eNB_id[3]:%d",
                         p_x2_setup_req->global_enb_id.enb_id.home_enb_id.eNB_id[0],
                         p_x2_setup_req->global_enb_id.enb_id.home_enb_id.eNB_id[1],
                         p_x2_setup_req->global_enb_id.enb_id.home_enb_id.eNB_id[2],
                         p_x2_setup_req->global_enb_id.enb_id.home_enb_id.eNB_id[3]);
            }
            /* SPR 19078: Coverity Fix 101195 Fix End */
            else
            {
                /* SPR 19078: Coverity Fix 101195 Fix End */
                X2AP_TRACE(X2AP_DETAILED, "Macro eNB ID recieved in x2Setup req: eNB_id[0]:%d "
                        "eNB_id[1]:%d eNB_id[2]:%d ",
                        p_x2_setup_req->global_enb_id.enb_id.macro_enb_id.eNB_id[0],
                        p_x2_setup_req->global_enb_id.enb_id.macro_enb_id.eNB_id[1],
                        p_x2_setup_req->global_enb_id.enb_id.macro_enb_id.eNB_id[2]);
                /* SPR 19078: Coverity Fix 101195 Fix End */
            }

            /* SPR 19078: Coverity Fix 107831 Start */
            ret_val = getsockopt_wrapper(
                        p_old_peer_enb_ctx->sctp_sd,
                        IPPROTO_SCTP,
                        SCTP_STATUS,
                        &status,
                        &optlen);
            if (X2AP_NULL != ret_val)
            {
                X2AP_TRACE(X2AP_DETAILED,"Failure returned by the function getsockopt_wrapper");
            }
            /* SPR 19078: Coverity Fix 107831 End */
            X2AP_TRACE(X2AP_WARNING,"SCTP Status of Old eNB socket = %d association = %d"
                    "in %d state.\n",p_old_peer_enb_ctx->sctp_sd,status.sstat_assoc_id,
                    status.sstat_state);

            if((SCTP_ESTABLISHED == status.sstat_state) || 
                    (SCTP_COOKIE_ECHOED == status.sstat_state))
            {
                X2AP_TRACE(X2AP_WARNING,"SCTP Status of Old eNB "
                        "in established state.\n");

                retVal = x2ap_build_x2setup_fail_resp(p_x2ap_gb_ctx,
                        p_enb_ctx,
                        X2AP_FALSE,
                        &error_indication,
                        &send_err_ind);
                /* SPR 19078: Coverity Fix 101196 Start */
                if (X2AP_FAILURE == retVal)
                {
                    X2AP_TRACE(X2AP_DETAILED,"failure returned by the function x2ap_build_x2setup_fail_resp");
                }
                /* SPR 19078: Coverity Fix 101196 End */
                if (p_enb_ctx->sctp_sd != INVALID_SCTP_SOCKET)
                {
                    /* SPR 20633 changes start */
                    if((p_enb_ctx->sctp_sd != g_sctp_server_fd_ipv4) && (p_enb_ctx->sctp_sd != g_sctp_server_fd_ipv6))
                    /* SPR 20633 changes end */
                    {
                        X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
                                p_enb_ctx->sctp_sd);
                        retVal = x2ap_close_association(p_enb_ctx->sctp_sd);
                        /* SPR 19078: Coverity Fix 101196 Start */
                        if (X2AP_FAILURE == retVal)
                        {
                            X2AP_TRACE(X2AP_DETAILED,"failure returned by the function  x2ap_close_association");
                        }
                        /* SPR 19078: Coverity Fix 101196 End */
                        x2ap_delete_assoc_id(p_enb_ctx->sctp_sd);
                    }

                    p_enb_ctx->sctp_sm_ctx.connect_ret_count = 0;
                    /* Change SM State to Idle State */
                    p_enb_ctx->sctp_sm_ctx.cur_sctp_sm_state = 
                        X2AP_SCTP_IDLE_ST;

                    p_node_to_delete = x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                                sd_search_tree), &p_enb_ctx->sctp_sd);

                    /* Delete node from SCTP SD Tree */
                    if (X2AP_P_NULL != p_node_to_delete)
                    {
                        x2ap_st_delete_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree),
                                p_node_to_delete);
                        X2AP_TRACE (X2AP_DETAILED,"Deleted node from sd_search_tree.\n");
                        x2ap_mem_free(p_node_to_delete);
                    }

                    p_node_to_delete = x2ap_st_find_tree_node(
                            &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
                            &(p_enb_ctx->enb_comm_info.ip_addr[0]));

                    if (X2AP_P_NULL != p_node_to_delete)
                    {
                        x2ap_st_delete_node(
                                &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
                                p_node_to_delete);
                        x2ap_mem_free(p_node_to_delete);
                        X2AP_TRACE (X2AP_DETAILED,"Deleted node from ip_search_tree.\n");

                        p_enb_ctx->is_enb_conf = 0; 
                    }

                    p_enb_ctx->sctp_sd = INVALID_SCTP_SOCKET;

                }

                retVal = X2AP_FAILURE;
            }
            else/* Old enb association is not in ACTIVE State */
            {
                X2AP_TRACE(X2AP_WARNING,"SCTP association of Old eNB "
                        "is not in established state.\n");

                retVal = x2ap_sctp_sm_entry (X2AP_SCTP_CONN_CLOSE_EV, 
                        (U8 *)(&p_old_peer_enb_ctx->gb_enb_id),
                        p_x2ap_gb_ctx,
                        p_old_peer_enb_ctx);

            }
        }
    }
    else
    {
        retVal = x2ap_build_x2setup_fail_resp(p_x2ap_gb_ctx,
                p_enb_ctx,
                X2AP_FALSE,
                &error_indication,
                &send_err_ind);
        /* SPR 19078: Coverity Fix 107826 Start */
        if (X2AP_FAILURE == retVal)
        {
            X2AP_TRACE(X2AP_DETAILED,"Failure returned by the function x2ap_build_x2setup_fail_resp.");
        }
        /* SPR 19078: Coverity Fix 107826 Start */
        retVal = X2AP_FAILURE;
    }
    /* SPR 19078: Coverity Fix 101195 Start */
    x2ap_mem_free(p_x2_setup_req);
    /* SPR 19078: Coverity Fix 101195 End */
    /*SPR_19067_FIX_START*/
    rtFreeContext(&asn1_ctx);    
    /*SPR_19067_FIX_STOP*/
    return retVal;
}
#endif
/*SPR_18216_END*/
