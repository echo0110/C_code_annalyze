/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: s1ap_rrm.h,v 1.3 2011/01/03 04:25:20 gur25381 Exp $
 ****************************************************************************
 *
 *  File Description : 
 *  This is the header file of s1ap_csc module which includes 
 *  common header files and various data structures exchanged.
 *
 ****************************************************************************
 *
 *
 ****************************************************************************/
#ifndef _S1AP_RRM_H_
#define _S1AP_RRM_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "s1ap_global_ctx.h"
#include "rrc_s1apRrm_intf.h"
#include "rrc_s1apRrm_il_composer.h"
#include "rrc_s1apRrm_il_parser.h"
#include "s1ap_asn_codec.h"
#include "s1ap_rrm_rim_intl.h"
#include "rrc_s1apRrm_intf.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*SPR 19183 START*/
#define RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE 6
/*SPR 19183 END*/

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
 /*SPR 19697 Fix Start*/
void s1ap_generate_plmn_identity_for_rim_to_int(
        plmn_identity_t    *p_plmn_identity,
        OSOCTET                 *p_s1ap_plmn_identity);

 /*SPR 19697 Fix End*/


void s1ap_process_rrm_messages(
        /* Pointer to input API buffer */
        void            *p_api,         

        /* Pointer to the S1AP global context data */
        s1ap_gb_context_t    *p_s1ap_gl_ctx);


/* Prototype of s1ap rrm MME ENB Direct Info Transfer procedure*/
void process_s1ap_rrm_enb_direct_info_transfer_msg(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_rrm_enb_config_transfer_msg(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_rrm_pws_resp_msg(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_rrm_kill_resp_msg(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

rrc_return_et compose_rim_info_req(
        rim_information_req_t* p_src_msg, 
        ran_information_req_rim_t* p_tgt_msg);

void process_s1ap_rrm_rim_info_req(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_rrm_rim_info(void);
                            /*SPR 17777 +-*/

void process_s1ap_rrm_rim_info_ack(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_rrm_rim_info_error(void);
                            /*SPR 17777 +-*/

void process_s1ap_rrm_rim_info_app_error(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void send_rim_info_app_error(U8 *p_trg_msg,
                    U32 msg_length,
                    rim_information_app_err_t *message, 
                    s1ap_gb_context_t         *p_s1ap_gb_ctx);

void send_rim_info_req(U8 *p_trg_msg,
                    U32 msg_length,
                    rim_information_req_t    *message, 
                    s1ap_gb_context_t        *p_s1ap_gb_ctx);

void process_s1ap_rrm_lppa_meas_init_resp(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_rrm_lppa_meas_init_fail(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_rrm_lppa_meas_fail_ind(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_rrm_lppa_meas_report(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_rrm_lppa_otdoa_info_resp(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_rrm_lppa_otdoa_info_fail(
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_rrm_lppa_error_ind_req(
        void             *p_api,
        s1ap_gb_context_t    *p_s1ap_gb_context);

/*SPR 19183 START*/
rrc_return_et rim_asn_encode_global_enb_id
(
 rim_global_enb_id_t   *p_elem_source,
 rim_cell_id_intl_t    *p_elem_target
 );
void send_rim_info_error(U8 *p_trg_msg,
                    U32 msg_length,
                    rim_routing_info_t       *p_dest_cell,
                    U8                       mme_id,
                    s1ap_gb_context_t        *p_s1ap_gb_ctx);
/*SPR 19183 END*/
#endif /* _S1AP_RRM_H_ */


