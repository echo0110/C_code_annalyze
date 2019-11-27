/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: s1ap_oam.h,v 1.8 2010/10/13 07:04:09 gur18569 Exp $
 *
 ****************************************************************************
 *
 *  File Description : The file contains the structures and function 
 *                     declarations to handle OAM requests.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: s1ap_oam.h,v $
 * Revision 1.8  2010/10/13 07:04:09  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.6.4.1  2010/08/05 08:10:44  gur15697
 * merging round 2
 *
 * Revision 1.7  2010/07/24 07:18:48  gur04733
 * Checking On 24 July
 *
 * Revision 1.6.2.2  2010/07/01 05:29:19  gur10248
 * retrive old version before s1ap asn
 *
 * Revision 1.6.2.1  2010/06/30 07:28:16  gur10248
 * new s1ap folder
 *
 * Revision 1.6  2010/04/06 04:03:17  gur18569
 * reset fwk chg
 *
 * Revision 1.5  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.4  2010/03/04 07:09:46  gur21006
 * s1ap_csc.h inclusion removed
 *
 * Revision 1.3  2010/02/18 04:25:20  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.4  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.3  2009/12/27 08:32:51  gur18569
 * removed tabs
 *
 * Revision 1.1.2.2  2009/12/22 14:38:31  gur18569
 * fixed 154
 *
 * Revision 1.1.2.1  2009/11/25 13:11:02  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.4  2009/11/20 08:40:43  gur18569
 * changes during UT
 *
 * Revision 1.1.2.3  2009/11/17 14:04:20  gur20470
 * Incorporated review comments
 *
 * Revision 1.1.2.2  2009/11/10 11:41:25  gur18569
 * review comments incorporated
 *
 * Revision 1.1.2.1  2009/10/23 16:11:39  gur18569
 * Initial version
 *
 *
 *
 ****************************************************************************/

#ifndef _S1AP_OAM_H_
#define _S1AP_OAM_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "s1ap_global_ctx.h"
#include "s1ap_timers.h"
#include "rrc_s1ap_uecc_intf.h"
#include "rrc_s1apOam_il_composer.h"
#include "rrc_s1apOam_il_parser.h"
#include "s1ap_api.h"
#include "search_tree_wrap.h"
#include "rrc_msg_mgmt.h"
#include "s1ap_logger.h"
#include "rrc_s1apOam_intf.h"
/*Coverity Fix 29163 Start */
 //#include "s1ap_asn_codec.h"
/*Coverity Fix 29163 End */
#include "sctp_wrappers.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
void s1ap_process_oam_messages(
        /* Pointer to input API buffer */
        void            *p_api,         

        /* Pointer to the S1AP global context data */
        s1ap_gb_context_t    *p_s1ap_gl_ctx);

void s1ap_process_init_timer( 
        /* Pointer to buffer associated with timer msg */

        /* Pointer to the S1AP global context data */
        s1ap_gb_context_t        *p_s1ap_gl_ctx);

void s1ap_build_and_send_reset_ind_msg( 
        s1ap_reset_msg_ind_t *p_s1ap_reset_ind,
        U32 dest_mod_id);

rrc_return_et s1ap_build_and_send_oam_reset_resp(
        s1ap_gb_context_t *p_s1ap_gl_ctx);

void s1ap_process_mme_reset_timer_expiry( 
        s1ap_timer_buffer_t     *p_s1ap_timer_buf,
        s1ap_gb_context_t        *p_s1ap_gl_ctx);

/* Prototype of s1ap oam provision request procedure*/
void process_s1ap_oam_prov_req(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

/* Prototype of s1ap oam cleanup request procedure*/
void process_cleanup_s1ap(
        s1ap_gb_context_t    *p_s1ap_gb_context);

/* Prototype of delete ue context procedure */
void s1ap_delete_all_ue_contexts(
        s1ap_gb_context_t   *p_s1ap_gb_context,
        rrc_bool_et);

/* Prototype of sending cleanup resp */
void s1ap_send_oam_cleanup_resp(
        U16 src_module_id,
        U16 trans_id);

void s1ap_process_oam_reset_req(
        void            *p_api,      
        s1ap_gb_context_t    *p_s1ap_gl_ctx);

void s1ap_process_oam_provision_req(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void s1ap_process_oam_cleanup_req(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void s1ap_oam_process_stats_req(
        void *p_api,
        s1ap_gb_context_t *p_s1ap_gb_context);

void s1ap_add_and_process_reset_context(
        s1ap_gb_context_t    *p_s1ap_gl_ctx,
        s1ap_enb_reset_info_t  *p_reset_input_info,
        U8 send_resp_to_module,
        U8,U8,U8);

void delete_reset_ue_s1ap_ctx(s1ap_gb_context_t *p_s1ap_gb_ctx,
        reset_req_t *p_reset_info,
        U8 mme_id,
        U8 is_ue_ctx_deleted);
void s1ap_oam_process_enb_config_update(
        void *p_api,
        s1ap_gb_context_t *p_s1ap_gb_context);

void s1ap_oam_process_close_mme_conn(
        void *p_api, 
        s1ap_gb_context_t *p_s1ap_gb_ctx);

void s1ap_oam_process_reestab_mme_conn(
        void *p_api, 
        s1ap_gb_context_t *p_s1ap_gb_ctx);

rrc_return_et send_oam_mme_connection_status_resp(
        s1ap_oam_mme_conn_status_response_t *p_oam_resp,
        U16 trans_id);
rrc_return_et s1ap_build_and_send_oam_s1ap_link_status_ind(
           s1ap_gb_context_t *p_s1ap_gb_ctx,
           mme_context_t     *p_mme_ctx,
           U8                status);
rrc_return_et build_and_send_enb_config_update_resp_to_oam(
        U8 mme_id,
        s1ap_oam_enb_config_update_resp_cause_et response,
        /*SPR 17777 +-*/
        U16 trans_id);
                                                                        
void s1ap_oam_process_get_sctp_status_req(
        void *p_api, 
        s1ap_gb_context_t *p_s1ap_gb_ctx);

rrc_return_et s1ap_oam_build_get_sctp_status_resp_with_bitmask(
	s1ap_gb_context_t *p_s1ap_gb_ctx,
	s1ap_oam_get_sctp_status_req_t *p_s1ap_oam_get_sctp_status_req,
	s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp,
	U8	*failed_mme_count,
	U8	*mme_status_count
		);

rrc_return_et s1ap_oam_build_get_sctp_status_resp_without_bitmask(
	s1ap_gb_context_t *p_s1ap_gb_ctx,
	s1ap_oam_get_sctp_status_resp_t *p_s1ap_oam_get_sctp_status_resp,
	U8	*mme_status_count);

void s1ap_oam_send_proc_res
(
        U16 src_module_id,
        U16 trans_id
);

void start_s1ap_health_monitor_timer
(
 /*SPR 17777 +-*/
   U32                  duration
);

#endif  /* _S1AP_OAM_H_ */
