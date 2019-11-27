/*******************************************************************************
 *
 *  FILE NAME   : x2ap_rrm.c
 *
 *  DESCRIPTION : 
 *
 *  Copyright 2009, Aricent Inc.
 *
 *******************************************************************************/
#include "commoninc.h"
#include "x2ap_db.h"
#include "rrc_x2apOam_intf.h"
#include "x2ap_utils.h"
#include "sctp_ipr.h"
#include "x2ap_api.h" 
#include "rrc_x2apOam_il_parser.h" 
#include "rrc_x2apRrm_il_parser.h" 
#include "x2ap_utils.h" 
#include "rrc_msg_mgmt.h"
#include "x2ap_init.h"
#include "sm_entry.h"
#include "rrc_x2apRrm_il_composer.h"
#include "x2ap_enb_conf_sm.h"

void x2ap_process_rrm_gu_grpid_add(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);
void x2ap_process_rrm_gu_grpid_del(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);

void    x2ap_process_rrm_rsu_start_req(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);
void    x2ap_process_rrm_rsu_start_res(
        void            *p_api, 
        x2ap_gb_context_t *p_x2ap_gb_ctx);
void    x2ap_process_rrm_rsu_stop_res(
        void            *p_api, 
        x2ap_gb_context_t *p_x2ap_gb_ctx);
void    x2ap_process_rrm_rsu_update(
        void            *p_api,  
        x2ap_gb_context_t *p_x2ap_gb_ctx);
void    x2ap_process_rrm_rsu_stop_req(
        void            *p_api,  
        x2ap_gb_context_t *p_x2ap_gb_ctx);
void    x2ap_process_rrm_load_information(
        void            *p_api, 
        x2ap_gb_context_t *p_x2ap_gb_ctx);

void    x2ap_process_rrm_reset_req(
        void            *p_api, 
        x2ap_gb_context_t *p_x2ap_gb_ctx);

extern void x2ap_process_enb_config_update_req(
        void              *p_api, 
        x2ap_gb_context_t *p_x2ap_gb_ctx, 
        U16               dest_module_id);

#ifdef LTE_EMBMS_SUPPORTED
void x2ap_process_updated_msbms_sf_info_ind(void* p_api, x2ap_gb_context_t*
        p_x2ap_gb_ctx);
#endif 

extern void x2ap_process_reset_req(
        void              *p_api, 
        x2ap_gb_context_t *p_x2ap_gb_ctx); 

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
 * Function Name  : x2ap_process_rrm_messages
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to x2AP global context.
 * Outputs        : None
 * Returns        : None
 * Description    : This function is invoked to handle OAM messages to x2AP.
 ******************************************************************************/
void x2ap_process_rrm_messages(
        /* Pointer to input API buffer */
        void            *p_api,         

        /* Pointer to the x2AP global context data */
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{
    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    switch (x2ap_get_api_id((U8*)p_api))
    {
        case X2AP_RRM_RSU_RRM_START_REQ:
            x2ap_process_rrm_rsu_start_req(p_api, p_x2ap_gb_ctx);
            break;
        case X2AP_RRM_RSU_RRM_START_RES:
            x2ap_process_rrm_rsu_start_res(p_api, p_x2ap_gb_ctx);
            break;
        case X2AP_RRM_RSU_RRM_STOP_RES:
            x2ap_process_rrm_rsu_stop_res(p_api, p_x2ap_gb_ctx);
            break;
        case X2AP_RRM_RSU_RRM_UPDATE:
            x2ap_process_rrm_rsu_update(p_api, p_x2ap_gb_ctx); 
            break;             
        case X2AP_RRM_RSU_RRM_STOP_REQ:
            x2ap_process_rrm_rsu_stop_req(p_api, p_x2ap_gb_ctx); 
	        break;
        case X2AP_RRM_LI_RRM_LOAD_INFORMATION_REQ:
            x2ap_process_rrm_load_information(p_api, p_x2ap_gb_ctx);
	        break;
        case X2AP_ENB_CONFIG_UPDATE_REQ:
            x2ap_process_enb_config_update_req(p_api, p_x2ap_gb_ctx, RRC_RRM_MODULE_ID);
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
#ifdef LTE_EMBMS_SUPPORTED
        case X2AP_RRM_UPDATED_MBMS_SF_INFO_IND:
            x2ap_process_updated_msbms_sf_info_ind(p_api, p_x2ap_gb_ctx);
	    break;
#endif
#ifdef ENDC_ENABLED
        case X2AP_ENDC_CONFIG_UPDATE_REQ:
        x2ap_process_endc_config_update_req(p_api, p_x2ap_gb_ctx, RRC_RRM_MODULE_ID);
        break;
#endif
        default:
            X2AP_TRACE(X2AP_WARNING,
                    "x2ap_process_rrm_messages: "
                    "unexpected API from RRM module received");
            break;
    }
}

