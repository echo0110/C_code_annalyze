/*******************************************************************************
 *
 *  FILE NAME   : x2ap_son.c
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

extern void x2ap_process_add_enb(
        void                 *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        U16 dest_module_id);

extern void x2ap_process_del_enb(
        void* p_api, 
        x2ap_gb_context_t   *p_x2ap_gb_ctx, 
        U16 dest_module_id);

extern void x2ap_process_enb_config_update_req(
        void* p_api, 
        x2ap_gb_context_t* p_x2ap_gb_ctx, 
        U16 dest_module_id);

extern void x2ap_process_reset_req(
        void* p_api,
        x2ap_gb_context_t* p_x2ap_gb_ctx);

extern void x2ap_process_peer_cell_activation_req(
        void* p_api, 
        x2ap_gb_context_t* p_x2ap_gb_ctx); 

extern void x2ap_process_cell_activation_resp(
        void* p_api, 
        x2ap_gb_context_t* p_x2ap_gb_ctx); 

extern void  x2ap_process_mobility_change_req(
        void                *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);

extern void  x2ap_process_mobility_change_resp(
        void                *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);



/*****************************************************************************
 * Function Name  : x2ap_process_son_messages
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to x2AP global context.
 * Outputs        : None
 * Returns        : None
 * Description    : This function is invoked to handle SON messages to x2AP.
******************************************************************************/
void x2ap_process_son_messages(void* p_api, x2ap_gb_context_t   *p_x2ap_gb_ctx)
{
    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    X2AP_UT_TRACE_ENTER();
    switch (x2ap_get_api_id((U8*)p_api))
    {
        case X2AP_ADD_ENB_REQ:
            x2ap_process_add_enb(p_api, p_x2ap_gb_ctx, RRC_SON_MODULE_ID);
            break;
        case X2AP_DEL_ENB_REQ:
            x2ap_process_del_enb(p_api, p_x2ap_gb_ctx, RRC_SON_MODULE_ID);
            break;
        case X2AP_ENB_CONFIG_UPDATE_REQ:
            x2ap_process_enb_config_update_req(p_api, p_x2ap_gb_ctx, RRC_SON_MODULE_ID);
            break;
        case X2AP_RESET_REQ:
            x2ap_process_reset_req(p_api, p_x2ap_gb_ctx);
            break;
        case X2AP_MOBILITY_CHANGE_REQ:
            x2ap_process_mobility_change_req(p_api, p_x2ap_gb_ctx);
            break;
        case X2AP_MOBILITY_SETTING_CHANGE_RESP:
            x2ap_process_mobility_change_resp(p_api, p_x2ap_gb_ctx);
            break;        
       case X2AP_PEER_CELL_ACTIVATION_REQ:
           x2ap_process_peer_cell_activation_req(p_api, p_x2ap_gb_ctx);
	    break;
       case X2AP_CELL_ACTIVATION_RESP:
           x2ap_process_cell_activation_resp(p_api, p_x2ap_gb_ctx);
	    break;
#ifdef ENDC_ENABLED
        case X2AP_ENDC_CONFIG_UPDATE_REQ:
        x2ap_process_endc_config_update_req(p_api, p_x2ap_gb_ctx, RRC_SON_MODULE_ID);
        break;
#endif
       default:
            X2AP_TRACE(X2AP_WARNING,
                    "x2ap_process_son_messages: "
                    "unexpected API from SON module received");
            break;
    }
    X2AP_UT_TRACE_EXIT();
}
