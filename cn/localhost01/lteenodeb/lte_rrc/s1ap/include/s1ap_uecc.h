/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: s1ap_uecc.h,v 1.5 2010/10/13 07:04:10 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : This is the header file of the S1AP UECC.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: s1ap_uecc.h,v $
 * Revision 1.5  2010/10/13 07:04:10  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.3.4.1  2010/07/21 08:43:09  gur15697
 * HO ERAB code Merging changes
 *
 * Revision 1.3  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.7  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.6  2009/12/27 08:32:51  gur18569
 * removed tabs
 *
 * Revision 1.1.2.5  2009/12/22 14:39:07  gur18569
 * fixed 154
 *
 * Revision 1.1.2.4  2009/12/19 03:53:01  gur18569
 * change in signature of process dl nas messsage
 *
 * Revision 1.1.2.3  2009/12/18 04:20:30  gur18569
 * added ue id pair presence argument
 *
 * Revision 1.1.2.2  2009/12/08 14:13:13  gur18569
 * using message id from ASN files SPR:161
 *
 * Revision 1.1.2.1  2009/11/25 13:11:02  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.2  2009/10/23 17:09:24  gur18569
 * Removed compilation errors
 *
 * Revision 1.1.2.1  2009/10/23 11:44:09  gur20052
 * Initial version added
 *
 *
 ****************************************************************************/

#ifndef _S1AP_UECC_H_
#define _S1AP_UECC_H_


/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "s1ap_logger.h"
#include "s1ap_global_ctx.h"
#include "s1ap_utils.h"
#include "s1ap_mme_fsm.h"
#include "search_tree_wrap.h"
#include "rrc_s1ap_uecc_intf.h"
#include "rrc_asn_enc_dec.h"
#include "rrc_s1ap_asn_intrl_dec_3gpp.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define EUTRAN_POOL_INDEX   0


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
void s1ap_process_uecc_messages( 
        void    *p_api,
        s1ap_gb_context_t *p_s1ap_gb_context);

rrc_return_et  s1ap_allocate_mme_id(
        U32 * p_mme_id ,
        s1ap_gb_context_t * p_s1ap_gb_context,
        s1ap_allocate_mme_req_t * p_alloc_mme_req,
    /*BUG 604 changes start*/
/* Bug 8198 Fix Start */
        U8     procedure_code,
        U32    enb_ue_s1ap_id);
/* Bug 8198 Fix End */
    /*BUG 604 changes stop*/

rrc_return_et  allocate_mme_by_round_robin(
        U32* p_mme_id,
        s1ap_gb_context_t* p_s1ap_gb_context,
        s1ap_allocate_mme_req_t * p_alloc_mme_req,
    /*BUG 604 changes start*/
/* Bug 8198 Fix Start */
        U8     procedure_code,
        U32    enb_ue_s1ap_id);
/* Bug 8198 Fix End */
    /*BUG 604 changes stop*/
/*SPR 8708 Fix Start*/
rrc_return_et  s1ap_allocate_by_mmec(
        U32* p_mme_id,
        s1ap_gb_context_t* p_s1ap_gb_context,
        s1ap_allocate_mme_req_t * p_alloc_mme_req);/*SPR 17777 +-*/
/*SPR 8708 Fix Stop*/

U8  s1ap_allocate_stream_id ( mme_context_t * p_mme_ctx);

extern U32 s1ap_get_api_id (U8 * p_api);

extern void active_mme_list_delete(
        s1ap_gb_context_t   * p_s1ap_gb_ctx,
        U16      mme_index);

void  process_ue_rel_reset_timer_exp(
        s1ap_gb_context_t * p_s1ap_gb_ctx,  
        mme_context_t       *p_mme_context,
        U32 * p_data) ;

rrc_return_et s1ap_process_dl_nas_msg(    
        s1ap_gb_context_t *p_s1ap_gb_ctx ,
        ue_s1ap_context_t    **p_s1ap_context, 
        U32             enb_ue_s1ap_id,            
        U32             mme_ue_s1ap_id,             
        mme_context_t     *p_mme_context,     
        void             *p_asn_msg,                  
        U16             asn_msg_len,
        U8              ue_id_pair_presence,
        U16             stream_id,
        s1ap_ProcedureCode procedureCode,
        s1ap_message_type_et  message_type);

rrc_return_et check_for_mme_overload(
        mme_overload_response_et mme_overload_response,
        U32 rrc_establishment_cause,
        s1ap_allocate_mme_req_t * p_alloc_mme_req,
        mme_context_t* p_mme_ctx,
        active_mme_id_t *p_mme_id_arr,
/* Bug 8198 Fix Start */
        U8  match_result);
/* Bug 8198 Fix End */

void send_asn_msg_to_mme(
        s1ap_gb_context_t                 *p_s1ap_gb_ctx,
        s1ap_ue_associated_sig_msg_req_t  *p_ue_associated_sig_req,
        U16                                ue_id,
        U8                                *p_message,
        U32                               message_buff_len
);

/*SPR 21220 Start*/
U32 delete_s1ap_context( U32 mme_ue_s1ap_id,
        mme_context_t * p_mme_context,
        U32* deleted_enb_ue_s1apid,
        s1ap_gb_context_t *p_s1ap_gb_ctx);
/*SPR 21220 Stop*/
void fill_and_send_error_ind_msg(mme_context_t     *p_mme_context,
                        U8                   ue_id_pair_presence,
                        U32             enb_ue_s1ap_id,            
                        U32             mme_ue_s1ap_id,
                        U32             s1ap_cause,
                        U16            *p_stream_id);

rrc_return_et s1ap_build_and_send_ue_associated_mme_update_status_ind
(
 U32 enb_ue_s1ap_id,
/*BUG 604 changes start*/
 mme_context_t     *p_mme_context,
 U16               ue_index
/*BUG 604 changes stop*/
 );

void s1ap_fill_gummei_info
(
 mme_context_t*    p_mme_context,
 s1ap_gummei_t*    p_gummei
 );

rrc_return_et build_and_send_s1ap_ue_rel_req_int(
    /*BUG 604 changes start*/
                U32 enb_ue_s1ap_id,
                U16 ue_index);
    /*BUG 604 changes stop*/

void  s1ap_process_ueccmd_messages(
        void    *p_api,
        s1ap_gb_context_t *p_s1ap_gb_ctx);

void s1ap_fill_protocol_event_params(
        EVENT_EXTERNAL_HEADER *p_event_header,
        U32 encoded_msg_len, 
        U16 event_id,
        U32 message_direction);

rrc_return_et  allocate_mme_by_weight_factor(
        U32* p_mme_id,
        s1ap_gb_context_t* p_s1ap_gb_context,
        s1ap_allocate_mme_req_t * p_alloc_mme_req,
    /*BUG 604 changes start*/
    /* Bug 8198 Fix Start */
        U8     procedure_code,
        U32    enb_ue_s1ap_id);
    /* Bug 8198 Fix End */
    /*BUG 604 changes stop*/
/* SPR 22789 Fix Start */
void active_mme_pool_list_insert(
        mme_context_t*  p_mme_ctx,
        list_t* p_active_mme_pool_area_list,
        U32*     p_total,
        U32* p_total_conn_ue);

rrc_return_et allocate_mme_by_mme_selection_value(
        U32*    p_mme_id,
        list_t* p_active_mme_pool_area_list,
        U32     rel_mme_capacity_total,
        U32     total_conn_ue,
        s1ap_gb_context_t* p_s1ap_gb_ctx);
/* SPR 22789 Fix End */

void build_and_send_mme_overload_ind(
        U32 enb_ue_s1ap_id,
    /*BUG 604 changes start*/
        s1ap_gb_context_t   *p_s1ap_gb_ctx,
        U16  ue_index);
    /*BUG 604 changes stop*/

#endif   /* _S1AP_UECC_H_ */

