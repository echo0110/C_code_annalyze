/******************************************************************************
 *
 *   FILE NAME:
 *       uecc_x2ap.c
 *
 *   DESCRIPTION:
 *       This is the source file of the UECC X2AP.
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *   28 Sep 2011     Sumit Goel  ---------       Initial
 *
 *   Copyright (c) 2009, Aricent Inc. All Rights Reserved
 *
 ******************************************************************************/

/****************************************************************************
 * Includes
 ****************************************************************************/
#include "rrc_defines.h"
#include "uecc_logger.h"
#include "uecc_global_ctx.h"
#include "rrc_msg_mgmt.h"
#include "rrc_common_utils.h"
#include "rrc_x2ap_uecc_intf.h"
#include "asn_enc_wrappers.h"
#include "asn_dec_wrappers.h"
/* + SPR 15630 */
#include "InformationElements.h"
#include "Constant-definitions.h"
#include "InformationElements_1.h"
#include "InformationElements_2.h"
#include "InformationElements_3.h"
#include "Internode-definitions.h"
#include "PDU-definitions.h"
#include "PDU-definitions_1.h"
/* - SPR 15630 */
#include "rrc_x2apEnb_intf.h"
#include "rrc_intrl_api.h"
#include "uecc_x2ap.h"
#include "uecc_s1ap.h"
#include "uecc_rrm.h"
#include "uecc_llim.h"
#include "uecc_fsm_engine.h"
#include "rrc_common_md.h"
#include "rrc_logging.h"
#include "uecc_llim_utils.h"
#include "x2ap_uecc.h"
/*CR 90 changes start*/
#include "rrc_shared_stats.h"
/*CR 90 changes stop*/

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
#include "asn_enc_wrappers.h"
#include "asn_dec_wrappers.h"
/*NR_DC Code Change Stop*/
#endif


/******************************************************************************
 * Private variable definitions
 *****************************************************************************/
static rrc_return_et uecc_x2ap_fill_rlf_report(
        uecc_gb_context_t *p_uecc_gb_context,
        uecc_ue_context_t *p_ue_context,
        RLF_Report_r9 *p_rlfReport_r9,
        rrc_rlf_report_t *p_x2ap_rlf_report);

static rrc_return_et uecc_x2ap_encode_RLF_Report(
        uecc_ue_context_t  *p_ue_context,
        U8                 *p_buff,
        U16                *p_buff_size);

static rrc_return_et uecc_x2ap_decode_RLF_Report(
        /*SPR_18125_START*/
        OSCTXT             *p_asn1_ctx,
        /*SPR_18125_END*/
        uecc_gb_context_t  *p_uecc_gb_context,
        RLF_Report_r9      *p_container,
        U8                 *p_buff,
        U16                buff_size);

static rrc_return_et uecc_x2ap_build_x2ap_e_rab_admitted_list
(
 uecc_ue_context_t* p_ue_context,
 OSCTXT *p_asn1_ctx,
 x2ap_E_RABs_Admitted_List *p_e_rab_admitted_list
 );

static rrc_return_et uecc_x2ap_build_x2ap_e_rab_not_admitted_list
(
 uecc_ue_context_t* p_ue_context,
 OSCTXT *p_asn1_ctx,
 x2ap_E_RAB_List *p_e_rab_not_admitted_list
 );

static rrc_return_et uecc_x2ap_build_erab_subject_to_status_transfer_list
(
    uecc_ue_context_t* p_ue_context,
    rrc_uecc_llim_sn_hfn_status_resp_t  *p_sn_hfn_status_resp,
    OSCTXT *p_asn1_ctx,
    x2ap_E_RABs_SubjectToStatusTransfer_List 
                      *p_erab_subject_to_status_transfer_list
 );  


static void uecc_x2ap_generate_plmn_identity_int(
    OSOCTET            *p_x2ap_plmn_identity,
    plmn_identity_t    *p_plmn_identity);

static rrc_return_et uecc_x2ap_build_ue_context_information( 
    uecc_ue_context_t               *p_ue_context,
    OSCTXT                          *p_asn1_ctxt,
    rrc_x2ap_handover_request_t     *p_x2ap_ho_req_msg,
    rrc_rrm_ho_required_t           *p_rrc_rrm_ho_required);

static rrc_return_et uecc_x2ap_build_x2ap_UE_HistoryInformation(
    uecc_ue_context_t           *p_ue_context,  /* UECC global context */ 
    OSCTXT                      *p_asn1_ctx,
    x2ap_UE_HistoryInformation  *p_ue_history_info,
    ue_history_t                *p_ue_history_at_rrc_rrm_intf);

rrc_return_et rrc_x2ap_last_visited_utran_cell_information_intrl_enc(
    uecc_gb_context_t           *p_uecc_gb_context,
    OSCTXT      *pctxt,         
    U8          *p_buff,       
    U16         *p_buff_size,
    ranap_LastVisitedUTRANCell_Item  *p_last_visited_utran_cell_Item);

static rrc_return_et uecc_x2ap_build_x2ap_E_RABInformationList(
    uecc_ue_context_t              *p_ue_context,
    OSCTXT                         *p_asn1_ctx,    
    x2ap_E_RABs_ToBeSetup_List     *p_erab_info_list);

static rrc_return_et uecc_x2ap_build_x2ap_RRCContext(
    uecc_ue_context_t              *p_ue_context,
    OSCTXT                         *p_asn1_ctx,    
    x2ap_UE_ContextInformation     *p_x2ap_ue_ctx_info);

static rrc_return_et uecc_x2ap_build_x2ap_HO_RestrictionList(
 uecc_ue_context_t               *p_ue_context,
 OSCTXT                          *p_asn1_ctxt,
 x2ap_UE_ContextInformation      *p_x2ap_ue_ctx_info);

#ifdef ENDC_EANBLED
static rrc_return_et rrc_populate_ho_restriction_list
(
    uecc_ue_context_t               *p_ue_context,
    x2ap_HandoverRestrictionList    *p_x2ap_ho_restricit_list,
    OSCTXT                          *p_asn1_ctxt
);
#endif

void uecc_x2ap_log_error_indication_message(
    uecc_gb_context_t           *p_uecc_gb_context,
    rrc_x2ap_error_indication_t *p_err_ind
);

void uecc_x2ap_log_criticality_diagostics(
        uecc_gb_context_t           *p_uecc_gb_context,
        x2ap_CriticalityDiagnostics *p_criticality_diagostics
);

extern void uecc_oamh_s1_ho_start_cell_traffic_trace
(
 uecc_gb_context_t *p_uecc_gb_context,
 uecc_ue_context_t *p_ue_context
);

extern void x2ap_reset_parse_asn_x2ap_cause(
            x2ap_Cause asn_cause,
            x2ap_cause_t* p_cause);

extern rrc_return_et uecc_rrm_build_meas_result_eutra(
    uecc_gb_context_t       *p_uecc_gb_context,
    uecc_ue_context_t       *p_ue_context,
    meas_result_eutra_t*    p_meas_result_eutra,
    MeasResultEUTRA*        p_asn_meas_results_eutra
);

extern rrc_return_et uecc_rrm_build_meas_result_utra(
     meas_result_utra_t*    p_meas_result_utra,
     MeasResultUTRA*        p_asn_meas_result_utra,
     uecc_ue_context_t*     p_ue_context,
     uecc_gb_context_t*     p_uecc_gb_context
);

extern rrc_return_et uecc_rrm_build_meas_result_geran(
     uecc_ue_context_t           *p_ue_context,
     meas_result_geran_t*    p_meas_result_geran,
     MeasResultGERAN*        p_asn_meas_result_geran
);

extern rrc_return_et uecc_rrm_build_meas_results_cdma2000(
    meas_results_cdma2000_t*    p_meas_results_cdma2000,
    MeasResultsCDMA2000*        p_asn_meas_results_cdma2000,
    uecc_ue_context_t*          p_ue_context,
    uecc_gb_context_t*          p_uecc_gb_context
);

/* SPR 21803 Fix Start */
extern rrc_return_et uecc_check_x2ap_message
(
    uecc_gb_context_t *p_uecc_gb_context    /* UECC global context */
);
/* SPR 21803 Fix Stop */


#ifdef ENDC_ENABLED
/****************************************************************************
 * Function Name  : uecc_x2ap_read_secondary_rat_usage_report_list
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  x2ap_SecondaryRATUsageReportList* - secondary rat usage report list
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion read secondary rat usage report list and 
 *                  put in share data
 ****************************************************************************/
rrc_return_t uecc_x2ap_read_secondary_rat_usage_report_list(
        uecc_ue_context_t*                                  p_ue_context,    
        x2ap_SecondaryRATUsageReportList*   p_sec_rat_usage_rep_list
);

/****************************************************************************
 * Function Name  : uecc_x2ap_handle_secondary_rat_data_usage_report_msg_rcvd
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : rrc_return_et
 * Description    : This is the function for Stroing SecondaryRatDataUsageReport
 *                  message in uecc context.
 ****************************************************************************/
rrc_return_et uecc_x2ap_handle_secondary_rat_data_usage_report_msg_rcvd(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
);
        
 /******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_secondary_rat_data_usage_report_msg
 *
 *   Input:  
 *          OSCTXT              *p_asn1_ctx
 *          x2ap_X2AP_PDU       *p_x2ap_pdu
 *          uecc_gb_context_t*  p_uecc_gb_context
 *          U8                  peer_enodeb_id
 *          U8                  *buffer
 *          void                *p_api
 *   Outputs : None
 *   DESCRIPTION:
 *       This function processes X2AP Process Secondary Rat data usage report message.
 *
 *   RETURNS:     None
 *
 ******************************************************************************/
void uecc_x2ap_process_secondary_rat_data_usage_report_msg
(
    OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    U8                  *buffer
);

/******************************************************************************
*   FUNCTION NAME: uecc_x2ap_process_sgnb_initiated_sgnb_release_req_msg
*   INPUTS       : OSCTXT*             p_asn1_ctx 
*                  x2ap_X2AP_PDU       *p_x2ap_pdu
*                  uecc_gb_context_t   *p_uecc_gb_context
*                  U8                  peer_enodeb_id
*                  U8                  *buffer
*   OUTPUTS      : None
*   DESCRIPTION  : This function handles the sgnb initiated sgnb release
*                  request.
*   RETURNS:     : None
*******************************************************************************/
void uecc_x2ap_process_sgnb_initiated_sgnb_release_req_msg
(
    OSCTXT*             p_asn1_ctx,
    x2ap_X2AP_PDU       *p_x2ap_pdu,
    uecc_gb_context_t   *p_uecc_gb_context,
    U8                  peer_enodeb_id,
    U8                  *buffer
);

/*NR_DC Code Changes Start*/
extern void uecc_fsm_rm_erab_setup_deallocate_erab(
    uecc_ue_context_t*  p_ue_context,           /* UECC UE context */
    U8                  erab_id                 /* E_RAB ID */
);
extern void map_x2ap_cause_to_s1ap_cause(
        x2ap_Cause x2apCause,
        s1ap_Cause *p_cause
);

static rrc_return_et uecc_x2ap_build_rrc_menb_to_sgnb_container(
    uecc_ue_context_t                    *p_uecc_ue_context,
    x2ap_MeNBtoSgNBContainer             *p_menb_to_sgnb_container,
    OSCTXT                               *pctxt);

static rrc_return_et uecc_x2ap_build_x2ap_sgnb_add_e_rab_admitted_list(
    uecc_ue_context_t                       *p_ue_context,
    OSCTXT                                  *p_asn1_ctx,
    x2ap_E_RABs_ToBeAdded_SgNBAddReqList    *p_e_rab_admitted_list);
static rrc_return_et uecc_x2ap_build_x2ap_sgnb_e_rab_release_list(
    uecc_ue_context_t                       *p_ue_context,
    OSCTXT                                  *p_asn1_ctx,
    x2ap_E_RABs_ToBeReleased_SgNBRelReqList *p_e_rab_to_release_list);
/*Error_Handling_changes*/
static  rrc_return_et check_for_api_id_mismatch(
		uecc_ue_context_t  	*p_ue_context,
		uecc_gb_context_t   *p_uecc_gb_context,
		msg_type_et 		msg_type,
		OSCTXT*  			p_asn1_ctx,
		U32 				menb_ue_x2ap_id,
		U32 				sgnb_ue_x2ap_id,
		U16 				proc_code,
		U8 					triggering_msg,
		U8       			peer_enodeb_id,
		void*               msg_p
);
/*Error_Handling_changes*/
/*NR_DC Code Changes Stop*/
#endif
/*SPR 15896 Fix Start*/
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_ue_assoc_msg_max_trace_handler
 *   INPUTS       : uecc_ue_context_t* p_ue_context
 *                  U8    *buffer
 *                  U32   buffer_len
 *                  char  *msg_name    : Message name
 *                  U8    recv_or_send : 0 - msg received / 1 - msg sent
 *   OUTPUTS      : None
 *   DESCRIPTION  : This function handles the UE Trace for X2AP messages.
 *   RETURNS:     : None
 *******************************************************************************/
void uecc_x2ap_ue_assoc_msg_max_trace_handler( 
              uecc_ue_context_t  *p_ue_context, 
              U8                 *buffer, 
              U32                buffer_len,
              char               *msg_name,
              U8                 recv_or_send)
{
    U8 header_buffer[100];
    U8  end[20] ;
    U32 index = 0;

    /* klockwork Fix */ 
    memset_wrapper(header_buffer,RRC_NULL,sizeof(header_buffer));
    memset_wrapper(end,RRC_NULL,sizeof(end));
    /* klockwork Fix */ 
    
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (p_ue_context->m.traceActivated)
    {
	    /*SPR 15875 Fix Start*/
	    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
			    (p_ue_context->p_ue_trace_activation_info != PNULL) &&
			    (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & X2) &&
			    ((MAXIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_trace_activation_info->trace_depth) ||
			     (MAXIMUM == p_ue_context->p_ue_trace_activation_info->trace_depth)))
	    {
	            P_RRC_TRACE(p_ue_context, set_maximum_bitmask(),GEN_MME, "\n <msg function=\"X2AP\" name=\"%s\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
				    msg_name, difftime_wrapper(p_ue_context->p_ue_trace_activation_info->rawtime));
	    }
	    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
			    (p_ue_context->p_ue_cell_trace_activation_info != PNULL) &&
			    (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & X2) &&
			    ((MAXIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION == p_ue_context->p_ue_cell_trace_activation_info->trace_depth) ||
			     (MAXIMUM == p_ue_context->p_ue_cell_trace_activation_info->trace_depth)))
	    {
	            P_RRC_TRACE(p_ue_context, set_maximum_bitmask(),GEN_CELL, "\n <msg function=\"X2AP\" name=\"%s\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
				    msg_name, difftime_wrapper(p_ue_context->p_ue_cell_trace_activation_info->rawtime));
	    }
	    /*SPR 15875 Fix Stop*/

	    if(0 == recv_or_send)
	    {
		    P_RRC_TRACE(p_ue_context, set_maximum_bitmask(), X2,"\n <initiator type=\"ENB\">%s</initiator>",
				    p_ue_context->p_gb_context->x2ap_ctx.x2ap_link_info[p_ue_context->
				    x2ap_context.peer_enodeb_id].enb_comm_info.ip_addr[0].ip_addr);
	    }
	    else
	    {
		    P_RRC_TRACE(p_ue_context, set_maximum_bitmask (),X2,"\n <target type=\"ENB\">%s</target>",
				    p_ue_context->p_gb_context->x2ap_ctx.x2ap_link_info[p_ue_context->
				    x2ap_context.peer_enodeb_id].enb_comm_info.ip_addr[0].ip_addr);
	    }
	    P_RRC_TRACE(p_ue_context, set_maximum_bitmask(), X2,"<rawMsg protocol=\"X2AP\" version=\"001\">");
	    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
			    (PNULL != p_ue_context->p_ue_trace_activation_info) &&
			    (PNULL != p_ue_context->p_ue_trace_activation_info->fp) &&
			    (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & X2) &&
			    ((p_ue_context->p_ue_trace_activation_info->trace_depth == MAXIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION) ||
			     (p_ue_context->p_ue_trace_activation_info->trace_depth == MAXIMUM)))
	    {
		    for (index = 0; index < buffer_len; index++)
		    {
			    fprintf_wrapper(p_ue_context->p_ue_trace_activation_info->fp, (const S8 *)"%02x", buffer[index]);
		    }
	    }
	    if ((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) && 
			    (PNULL != p_ue_context->p_ue_cell_trace_activation_info) &&
			    (PNULL != p_ue_context->p_ue_cell_trace_activation_info->fp) &&
			    (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & X2) &&
			    ((p_ue_context->p_ue_cell_trace_activation_info->trace_depth == MAXIMUM_WITHOUT_VENDOR_SPECIFIC_EXTENSION) ||
			     (p_ue_context->p_ue_cell_trace_activation_info->trace_depth == MAXIMUM)))
	    {
		    for (index = 0; index < buffer_len; index++)
		    {
			    fprintf_wrapper(p_ue_context->p_ue_cell_trace_activation_info->fp, (const S8 *)"%02x", buffer[index]);
		    }
	    }
	    P_RRC_TRACE(p_ue_context, set_maximum_bitmask(), X2," </rawMsg>\n </msg>");
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*SPR 15896 Fix Stop*/

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_ue_associated_sig_msg_ind
 *
*   INPUTS        : void *p_api
*                   uecc_gb_context_t *p_uecc_gb_context
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function processes X2AP_UE_ASSOCIATED_SIG_MSG_IND
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void uecc_x2ap_process_ue_associated_sig_msg_ind
(
    void *p_api,
    uecc_gb_context_t *p_uecc_gb_context
)
{
    x2ap_ue_associated_sig_msg_ind_t *p_x2ap_ue_associated_sig_msg_ind;
    OSCTXT asn1_ctx;
    x2ap_X2AP_PDU x2ap_pdu;
    U16 buf_size=0, encoded_len=0;

    rrc_return_et response = RRC_SUCCESS;
    U8              peer_enodeb_id = RRC_NULL;
/*BUG 604 changes start*/
    U32             enb_ue_s1ap_id = RRC_NULL;
    U16             source_ue_index = RRC_NULL;
    U16             target_ue_index = RRC_NULL;
    U8              security_key_present = RRC_FALSE;
/*BUG 604 changes stop*/

    RRC_ASSERT(PNULL!=p_api);
    RRC_ASSERT(PNULL!=p_uecc_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    /* SPR 21803 Fix Start */
    if(RRC_FAILURE == uecc_check_x2ap_message(p_uecc_gb_context))
    {
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }
    /* SPR 21803 Fix Stop */

    asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);

    
    
    buf_size = rrc_get_api_buf_size(p_api);
    /* coverity_93935 start */
    if(buf_size < ((U16)RRC_API_HEADER_SIZE + 
           (U16)sizeof(x2ap_ue_associated_sig_msg_ind_t)))
    /* coverity_93935 stop */
    {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
               facility_name, RRC_ERROR, 
               "[x2ap_ue_assoc_sig_msg_ind] Wrong Msg size = %d\n",
               buf_size);
       RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
       return;
    }

    /* coverity_93935 start */
   encoded_len = (buf_size - (U16)RRC_API_HEADER_SIZE); 
    /* coverity_93935 stop */

   /* Init ASN Context */
   if(RT_OK != rtInitContext(&asn1_ctx))
   {
       RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
               facility_name, RRC_ERROR,
               "[X2AP_UE_ASSOCIATED_SIG_MSG_IND]ASN Context Init failure");
       RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
       return;
   }

    p_x2ap_ue_associated_sig_msg_ind = 
        (x2ap_ue_associated_sig_msg_ind_t*)((U8*)p_api + RRC_API_HEADER_SIZE);

    peer_enodeb_id  = p_x2ap_ue_associated_sig_msg_ind->peer_enodeb_id;

    do
    {
        response = rrc_x2ap_pdu_intrl_dec(&asn1_ctx,
                p_x2ap_ue_associated_sig_msg_ind->message_buff_p,
                encoded_len,
                &x2ap_pdu);

        if(RRC_FAILURE == response)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR,
                    "[X2AP_UE_ASSOCIATED_SIG_MSG_IND] x2ap_pdu \
                    decoding_failure");
            /* Send Error Indication with Transfer Syntax Error.
             * Criticality Diagnostics are not required. */ 

            if((RRC_INTER_CELL_HO_PROC_CODE != 
                  p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code) &&
                   (RRC_INTER_CELL_HO_CANCEL_PROC_CODE != 
/*BUG 604 changes start*/
                     p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code) &&
                   (RRC_INTRA_CELL_HO_PROC_CODE !=
                     p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code))
/*BUG 604 changes stop*/
            {
                uecc_x2ap_build_and_send_err_indication(peer_enodeb_id, PNULL);
            }
            break;
        }

        /* Verify the procedure code for X2AP PDU */

        if(T_x2ap_X2AP_PDU_initiatingMessage == x2ap_pdu.t)
        {
            if( ASN1V_x2ap_id_handoverPreparation ==
                    x2ap_pdu.u.initiatingMessage->procedureCode)
            {
                /* Get destination module id from the received message */

                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, 
                        RRC_INFO,"X2AP_MSG: HANDOVER REQUEST %s",
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));
/*BUG 604 changes start*/
                if(X2AP_UE_ASSOC_SIG_MSG_IND_S1_PEER_PRESENT &
                        p_x2ap_ue_associated_sig_msg_ind->bitmask)
                {
                    enb_ue_s1ap_id = 
                        p_x2ap_ue_associated_sig_msg_ind->enb_ue_s1ap_id; 
                }
                if(X2AP_UE_ASSOC_SIG_MSG_IND_SRC_UE_INDEX_PRESENT & 
                        p_x2ap_ue_associated_sig_msg_ind->bitmask)
                {
                    source_ue_index = 
                        p_x2ap_ue_associated_sig_msg_ind->source_ue_index;
                }
/*BUG 604 changes stop*/
               /*BUG 604 changes start*/
                if (X2AP_UE_ASSOC_SIG_MSG_REQ_SECURITY_FLAG_PRESENT &
                        p_x2ap_ue_associated_sig_msg_ind->bitmask)
                {
                    security_key_present = 
                      p_x2ap_ue_associated_sig_msg_ind->is_security_key_present;
                }
               /*BUG 604 changes stop*/

                uecc_x2ap_process_handover_request_msg(
                        &asn1_ctx,
                        &x2ap_pdu,
                        p_uecc_gb_context,
                        peer_enodeb_id,
                        p_uecc_gb_context->uecc_module_id,
                        &p_x2ap_ue_associated_sig_msg_ind->message_buff_p[0],
/*BUG 604 changes start*/
                        p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code,
                        enb_ue_s1ap_id,
                        source_ue_index,
                        security_key_present);
/*BUG 604 changes stop*/

                break;
            }
            else if( ASN1V_x2ap_id_snStatusTransfer ==
                    x2ap_pdu.u.initiatingMessage->procedureCode)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, 
                        RRC_INFO,"X2AP_MSG: SN STATUS TRANSFER %s",
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));

                uecc_x2ap_process_sn_status_trnsfr_msg(
                        &asn1_ctx,
                        &x2ap_pdu,
                        p_uecc_gb_context,
                        peer_enodeb_id,
                        /*SPR_17976_START*/
                        &p_x2ap_ue_associated_sig_msg_ind->message_buff_p[0],
                        p_api);
                        /*SPR_17976_END*/
                break;
            }
            else if( ASN1V_x2ap_id_uEContextRelease ==
                    x2ap_pdu.u.initiatingMessage->procedureCode)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, 
                        RRC_INFO,"X2AP_MSG: UE CONTEXT RELEASE %s",
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));

                uecc_x2ap_process_ue_context_release_msg(
                        &asn1_ctx,
                        &x2ap_pdu,
                        p_uecc_gb_context,
                        peer_enodeb_id,
                        &p_x2ap_ue_associated_sig_msg_ind->message_buff_p[0]);
                break;
            }
            else if(  ASN1V_x2ap_id_handoverCancel ==
                    x2ap_pdu.u.initiatingMessage->procedureCode)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, 
                        RRC_INFO,"X2AP_MSG: LOCAL_X2_HANDOVER_CANCEL %s",
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));

                uecc_x2ap_process_handover_cancel_msg(
                        &asn1_ctx,
                        &x2ap_pdu,
                        p_uecc_gb_context,
                        peer_enodeb_id,
                        &p_x2ap_ue_associated_sig_msg_ind->message_buff_p[0]);
                break;
            }
            else if( ASN1V_x2ap_id_errorIndication ==
                    x2ap_pdu.u.initiatingMessage->procedureCode)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, RRC_INFO,
                        "X2AP_MSG: ERROR INDICATION %s",
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));
                /*HO:x2ap_error_ind:start*/ 
                if (x2ap_pdu.u.initiatingMessage->value.t ==
                        T1x2ap__errorIndication)
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                            facility_name, RRC_INFO,"X2AP_MSG: ERROR INDICATION %s",
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p)); 
                    /* Message Type Criticality is s1ap_ignore*/
                    uecc_x2ap_process_error_indication(&x2ap_pdu,
                            p_uecc_gb_context,
			    &asn1_ctx,
                            /*SPR_16618_START*/
                            peer_enodeb_id,
                            /*SPR_16618_END*/
			    &p_x2ap_ue_associated_sig_msg_ind->message_buff_p[0]);
                    break;
                }
            }
#ifdef ENDC_ENABLED
            /*NR_DC Code Changes Start*/
            else if( ASN1V_x2ap_id_sgNBinitiatedSgNBRelease ==
                    x2ap_pdu.u.initiatingMessage->procedureCode)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, RRC_INFO,
                        "X2AP_MSG: SGNB INITIATED SGNB RELEASE REQ"
                        "Peer eNodeB Id: %u %s",
                        peer_enodeb_id,
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));
                uecc_x2ap_process_sgnb_initiated_sgnb_release_req_msg(
                        &asn1_ctx,
                        &x2ap_pdu,
                        p_uecc_gb_context,
                        peer_enodeb_id,
                        &p_x2ap_ue_associated_sig_msg_ind->message_buff_p[0]);
                break;
            }
            else if( ASN1V_x2ap_id_secondaryRATDataUsageReport ==
                    x2ap_pdu.u.initiatingMessage->procedureCode)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, RRC_INFO,
                        "X2AP_MSG: SECONDARY RAT DATA USAGE REPORT"
                        "Peer eNodeB Id: %u %s",
                        peer_enodeb_id,
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));
    
                uecc_x2ap_process_secondary_rat_data_usage_report_msg(
                        &asn1_ctx,
                        &x2ap_pdu,
                        p_uecc_gb_context,
                        peer_enodeb_id,
                        &p_x2ap_ue_associated_sig_msg_ind->message_buff_p[0]);
                break;
            }
            /*NR_DC Code Changes Stop*/
#endif
            else
            {
                /* Unknown or Not Supported Initiating Procedure Code */
                if(x2ap_ignore  != x2ap_pdu.u.initiatingMessage->criticality)
                {
                    if ((p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code != 
                            RRC_INTER_CELL_HO_PROC_CODE) &&
                            (p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code !=
/*BUG 604 changes start*/
                             RRC_INTER_CELL_HO_CANCEL_PROC_CODE) &&
                            (p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code !=
                             RRC_INTRA_CELL_HO_PROC_CODE))
/*BUG 604 changes stop*/
                    {
                        /* Build and send Error Indication */
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                                p_uecc_gb_context->facility_name, RRC_INFO,
                                "%d - Unexpected Procedure Code, " 
                                "Sending X2AP Error Indication %s",
                                x2ap_pdu.u.initiatingMessage->procedureCode,
                                change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));

                        uecc_x2ap_build_and_send_error_indication_unknown_proc(
                                p_uecc_gb_context, 
                                peer_enodeb_id,
                                x2ap_pdu.u.initiatingMessage->procedureCode,
                                x2ap_pdu.u.initiatingMessage->criticality,
                                T_x2ap_X2AP_PDU_initiatingMessage
                                );
                        break;
                    }
                }
                else
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                            p_uecc_gb_context->facility_name, 
                            RRC_INFO,"Unexpected Procedure Code \
                            with Ignore Criticality - %d", 
                            x2ap_pdu.u.initiatingMessage->procedureCode);
                    break;
                }
            }
        }
        else if( T_x2ap_X2AP_PDU_successfulOutcome == x2ap_pdu.t)
        {
            if( ASN1V_x2ap_id_handoverPreparation ==
                    x2ap_pdu.u.successfulOutcome->procedureCode)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, RRC_INFO,
                        "X2AP_MSG: HANDOVER REQUEST ACK %s",
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));
/*BUG 604 changes start*/
                if(X2AP_UE_ASSOC_SIG_MSG_IND_TRG_UE_INDEX_PRESENT & 
                        p_x2ap_ue_associated_sig_msg_ind->bitmask)
                {
                    target_ue_index = 
                        p_x2ap_ue_associated_sig_msg_ind->target_ue_index;
                }
/*BUG 604 changes stop*/

                uecc_x2ap_process_handover_request_ack_msg(
                        &asn1_ctx,
                        &x2ap_pdu,
                        p_uecc_gb_context,
                        peer_enodeb_id,
/*BUG 604 changes start*/
                        &p_x2ap_ue_associated_sig_msg_ind->message_buff_p[0],
                        target_ue_index);
/*BUG 604 changes stop*/
                break;
            }

#ifdef ENDC_ENABLED
            /*NR_DC Code Changes Start*/
            else if(ASN1V_x2ap_id_sgNBAdditionPreparation == 
                    x2ap_pdu.u.successfulOutcome->procedureCode )
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, 
                        RRC_INFO,"X2AP_MSG: SgNB ADDITION REQUEST ACK,"
                        "Peer eNodeB Id: %u %s",
                        p_x2ap_ue_associated_sig_msg_ind->peer_enodeb_id,
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));
                uecc_x2ap_process_sgnb_addition_req_ack_msg(
                        &asn1_ctx,
                        &x2ap_pdu,
                        p_uecc_gb_context,
                        peer_enodeb_id,
                        &p_x2ap_ue_associated_sig_msg_ind->message_buff_p[0]);

                break;
            }
            /*NR_DC Code Changes Stop*/
            /*EN-DC_changes_start*/
            else if( ASN1V_x2ap_id_meNBinitiatedSgNBRelease ==
                    x2ap_pdu.u.successfulOutcome->procedureCode)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, RRC_INFO,
                        "X2AP_MSG: SGNB RELEASE REQ ACK %s",
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));

                uecc_x2ap_process_sgnb_release_request_ack(
                        &asn1_ctx,
                        &x2ap_pdu,
                        p_uecc_gb_context,
                        peer_enodeb_id,
                        &p_x2ap_ue_associated_sig_msg_ind->message_buff_p[0]);
                break;
            }
            /*EN-DC_changes_end*/
#endif

            else
            {
                /* Unknown or Not Supported Successful Outcome Procedure Code */
                if(x2ap_ignore  != x2ap_pdu.u.successfulOutcome->criticality)
                {
                    if ((p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code != 
                            RRC_INTER_CELL_HO_PROC_CODE) &&
                            (p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code !=
/*BUG 604 changes start*/
                             RRC_INTER_CELL_HO_CANCEL_PROC_CODE) &&
                            (p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code !=
                             RRC_INTRA_CELL_HO_PROC_CODE))
/*BUG 604 changes stop*/
                    {
                        /* Build and send Error Indication */
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                                p_uecc_gb_context->facility_name, RRC_INFO,
                                "%d - Unexpected Procedure Code, " 
                                "in SuccessfulOutcome - "
                                "Sending X2AP Error Indication",
                                x2ap_pdu.u.successfulOutcome->procedureCode);

                        uecc_x2ap_build_and_send_error_indication_unknown_proc(
                                p_uecc_gb_context, 
                                peer_enodeb_id,
                                x2ap_pdu.u.successfulOutcome->procedureCode,
                                x2ap_pdu.u.successfulOutcome->criticality,
                                T_x2ap_X2AP_PDU_successfulOutcome
                                );
                        break;
                    }
                }
                else
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                            p_uecc_gb_context->facility_name, 
                            RRC_INFO,"Unexpected Procedure Code "
                            "with Ignore Criticality - %d", 
                            x2ap_pdu.u.successfulOutcome->procedureCode);
                    break;
                }
            }
        }
        else if (T_x2ap_X2AP_PDU_unsuccessfulOutcome ==  x2ap_pdu.t)
        {
            if( ASN1V_x2ap_id_handoverPreparation ==
                    x2ap_pdu.u.unsuccessfulOutcome->procedureCode)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_INFO,"X2AP_MSG: HANDOVER PREPARATION FAILURE %s",
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));

                uecc_x2ap_process_handover_prep_failure_msg(
                        &asn1_ctx,
                        &x2ap_pdu,
                        p_uecc_gb_context,
                        peer_enodeb_id,
                        &p_x2ap_ue_associated_sig_msg_ind->message_buff_p[0]);
                break;
            }
#ifdef ENDC_ENABLED
            /*NR_DC Code Changes Start*/
            else if(ASN1V_x2ap_id_sgNBAdditionPreparation == 
                    x2ap_pdu.u.unsuccessfulOutcome->procedureCode)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_INFO,"X2AP_MSG: SGNB ADDITION REQUEST REJECT %s",
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));

                uecc_x2ap_process_sgnb_addition_req_reject(
                        &asn1_ctx,
                        &x2ap_pdu,
                        p_uecc_gb_context,
                        peer_enodeb_id,
                        &p_x2ap_ue_associated_sig_msg_ind->message_buff_p[0]);
                break;
            }
            else if(ASN1V_x2ap_id_meNBinitiatedSgNBRelease == 
                    x2ap_pdu.u.unsuccessfulOutcome->procedureCode)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_INFO,"X2AP_MSG: SGNB RELEASE REQUEST REJECT %s",
                        change_data_to_str(encoded_len, p_x2ap_ue_associated_sig_msg_ind->message_buff_p));

                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_INFO,"X2AP_MSG: Ignoring SGNB RELEASE REQUEST REJECT, Not Supported!");
                break;
            }
            /*NR_DC Code Changes Stop*/
#endif
            else
            {
                /* Unknown or Not Supported Unsuccessful Outcome Procedure Code */
                if(x2ap_ignore  != x2ap_pdu.u.unsuccessfulOutcome->criticality)
                {
                    if ((p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code != 
                                RRC_INTER_CELL_HO_PROC_CODE) &&
                            (p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code !=
/*BUG 604 changes start*/
                             RRC_INTER_CELL_HO_CANCEL_PROC_CODE) &&
                            (p_x2ap_ue_associated_sig_msg_ind->x2ap_procedure_code !=
                             RRC_INTRA_CELL_HO_PROC_CODE))
/*BUG 604 changes stop*/
                    {
                        /* Build and send Error Indication */
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                                p_uecc_gb_context->facility_name, RRC_INFO,
                                "%d - Unexpected Procedure Code, "
                                "in UnsuccessfulOutcome - "
                                "Sending X2AP Error Indication",
                                x2ap_pdu.u.unsuccessfulOutcome->procedureCode);

                        uecc_x2ap_build_and_send_error_indication_unknown_proc(
                                p_uecc_gb_context, 
                                peer_enodeb_id,
                                x2ap_pdu.u.unsuccessfulOutcome->procedureCode,
                                x2ap_pdu.u.unsuccessfulOutcome->criticality,
                                T_x2ap_X2AP_PDU_unsuccessfulOutcome
                                );
                        break;
                    }
                }
                else
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                            facility_name, RRC_INFO,"Unexpected Procedure Code \
                            with Ignore Criticality - %d", 
                            x2ap_pdu.u.unsuccessfulOutcome->procedureCode);
                    break;
                }
            }
        }
        else
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR, "Incorrect X2AP PDU =%i",x2ap_pdu.t);
        }
    } while(0);

    rtFreeContext(&asn1_ctx);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/* Bug 4691 Changes Start */
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_update_nbour_info
 *
 *   INPUTS        : void *p_api
 *                   uecc_gb_context_t *p_uecc_gb_context
 *   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function processes X2AP_UPDATE_NBOUR_INFO
 *
 *   RETURNS:
 *       None
 ******************************************************************************/
void uecc_x2ap_process_update_nbour_info(
        void *p_api,
        uecc_gb_context_t *p_uecc_gb_context
        )
{
    x2ap_nbour_info_t* p_x2ap_nbour_info = PNULL;
    U8 pci  = RRC_NULL;
    U16 buf_size = 0;
    U8 peer_enodeb_id, serving_cell_index = 0;
    U16 num_served_cells = 0;
    void* p_srvd_cell_info = PNULL;

    RRC_ASSERT(PNULL!=p_api);
    RRC_ASSERT(PNULL!=p_uecc_gb_context);

   p_x2ap_nbour_info =
        (x2ap_nbour_info_t *)((U8*)p_api + RRC_API_HEADER_SIZE);	

    buf_size = rrc_get_api_buf_size(p_api);
    /* Precondition - p_api should point to right message */
    if (buf_size < RRC_API_HEADER_SIZE + sizeof(x2ap_nbour_info_t))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                p_uecc_gb_context->facility_name, RRC_WARNING,
                "[X2AP_UPDATE_NBOUR_INFO] "
                "Wrong message size - ignore.");

        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }
	
    rrc_return_et retVal = RRC_FAILURE;

    for(peer_enodeb_id = 0; peer_enodeb_id < MAX_PEER_ENB; peer_enodeb_id ++)
    {
        if(RRC_TRUE == 
                p_uecc_gb_context->x2ap_ctx.x2ap_link_info[peer_enodeb_id].
                connection_status)
        {

            if( PNULL != (p_srvd_cell_info = p_uecc_gb_context->x2ap_ctx.
                        x2ap_link_info[peer_enodeb_id].p_srvd_cell_info))
            {
                num_served_cells = p_uecc_gb_context->x2ap_ctx.
                    x2ap_link_info[peer_enodeb_id].num_served_cells;

                for(serving_cell_index = 0 ;serving_cell_index < num_served_cells; serving_cell_index ++)
                {
                    if (!memcmp_wrapper(p_x2ap_nbour_info->ecgi.eutran_cell_id,
                                ((x2ap_srvd_cell_info_t*)((U8*)p_srvd_cell_info +
                                 (sizeof(x2ap_srvd_cell_info_t)* serving_cell_index)))->
                                cell_id.eutran_cell_id,
                                HOME_ENB_ID_OCTET_SIZE*sizeof(U8)))
                    {
                            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_INFO,"Peer ENodeB Id and Serving Cell Index for Given ECGI is %d and %d\n",
                            peer_enodeb_id,serving_cell_index);
                            retVal  =  RRC_SUCCESS;
                            break;
                    }
					
                }

                if(retVal  ==  RRC_SUCCESS)
                {
                    break;
                }
            }
        }
    }
	
    if(MAX_PEER_ENB == peer_enodeb_id)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,"No serving cell info present in peer eNodeB Context"
                "ECGI value received is invalid");
    }
    else
    {

        pci = P_SRVD_CELL_INFO(p_uecc_gb_context->x2ap_ctx.
            x2ap_link_info[peer_enodeb_id].p_srvd_cell_info,
            serving_cell_index)->pci;

        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
            p_uecc_gb_context->facility_name,
            RRC_INFO,"For Peer enodeb id %d serving_cell_index %d Current PCI value is %d and Updated PCI value is %d \n",
            peer_enodeb_id,serving_cell_index,pci,p_x2ap_nbour_info->pci);

        P_SRVD_CELL_INFO(p_uecc_gb_context->x2ap_ctx.
            x2ap_link_info[peer_enodeb_id].p_srvd_cell_info,
            serving_cell_index)->pci = p_x2ap_nbour_info->pci;
    }
			
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);

}
/* Bug 4691 Changes End */

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_status_msg_ind
 *
*   INPUTS        : void *p_api
*                   uecc_gb_context_t *p_uecc_gb_context
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function processes X2AP_STATUS_MSG_IND
 *
 *   RETURNS:
 *       None
 ******************************************************************************/
void uecc_x2ap_process_status_msg_ind(
        void *p_api,
        uecc_gb_context_t *p_uecc_gb_context
        )
{
    x2ap_status_msg_ind_t *p_x2ap_status_msg_ind = PNULL;
    U8 peer_enb_ct = RRC_NULL;
    U16 buf_size = 0;
    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/

    RRC_ASSERT(PNULL!=p_api);
    RRC_ASSERT(PNULL!=p_uecc_gb_context);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    p_x2ap_status_msg_ind =
        (x2ap_status_msg_ind_t *)((U8*)p_api + RRC_API_HEADER_SIZE);

    buf_size = rrc_get_api_buf_size(p_api);
    /* Precondition - p_api should point to right message */
    if (buf_size <
            RRC_API_HEADER_SIZE +
            sizeof(x2ap_status_msg_ind_t))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                p_uecc_gb_context->facility_name, RRC_WARNING,
                "[X2AP_STATUS_MSG_IND] "
                "Wrong message size - ignore.");

        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }

    /* Inserting global eNB ID and connection status */
    for (peer_enb_ct = 0; peer_enb_ct<MAX_PEER_ENB; peer_enb_ct++)
    {
        if(peer_enb_ct == p_x2ap_status_msg_ind->peer_enodeb_id)
        {
            p_uecc_gb_context->x2ap_ctx.x2ap_link_info[peer_enb_ct].
                presence_flag = RRC_TRUE;

            #ifdef ENDC_ENABLED 
            /* MENB CHANGES - START */
            if ( X2AP_STATUS_MSG_IND_PEER_GB_GNB_ID_PRESENT &
                    p_x2ap_status_msg_ind->bitmask )
            {
                p_uecc_gb_context->x2ap_ctx.x2ap_link_info[peer_enb_ct].
                    peer_gb_gnb_id = p_x2ap_status_msg_ind->peer_gb_gnb_id;

                p_uecc_gb_context->x2ap_ctx.x2ap_link_info[peer_enb_ct].
                    presence_flag |=
                        X2AP_PEER_GB_GNB_ID_PRESENT;
            }
            else
            {
            /* MENB CHANGES - END */
            #endif
            p_uecc_gb_context->x2ap_ctx.x2ap_link_info[peer_enb_ct].
                peer_gb_enb_id = p_x2ap_status_msg_ind->peer_gb_enb_id;
            #ifdef ENDC_ENABLED
            /* MENB CHANGES - START */
            }
            /* MENB CHANGES - END */
            #endif

            p_uecc_gb_context->x2ap_ctx.x2ap_link_info[peer_enb_ct].
                connection_status = p_x2ap_status_msg_ind->connection_status;

            if (p_x2ap_status_msg_ind->bitmask
                        & X2AP_STATUS_MSG_IND_ENB_COMM_INFO_PRESENT)
            {
                l3_memcpy_wrapper((void *)&(p_uecc_gb_context->x2ap_ctx.
                            x2ap_link_info[p_x2ap_status_msg_ind->peer_enodeb_id].enb_comm_info),
                         (const void*)&(p_x2ap_status_msg_ind->enb_comm_info),
                          sizeof(x2_enb_comm_info_t));
            }

            RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                    p_uecc_gb_context->facility_name,RRC_BRIEF,
                    "[X2AP_STATUS_MSG_IND] Received Connection status: %d"
                    "for Peer EnodeB Id : %d",
                    p_x2ap_status_msg_ind->connection_status,
                    peer_enb_ct );
            if(X2AP_LINK_UP == p_x2ap_status_msg_ind->connection_status)
            {
                /* Inserting served cell info in uecc context */
                if (p_x2ap_status_msg_ind->bitmask
                        & X2AP_STATUS_MSG_IND_SERVED_CELL_ARRAY_PRESENT)
                {
                    /* Free the already existing served cell Info if any */
                    if(PNULL != p_uecc_gb_context->x2ap_ctx.
                            x2ap_link_info[peer_enb_ct].p_srvd_cell_info)
                    {
                        rrc_mem_free(p_uecc_gb_context->x2ap_ctx.
                                x2ap_link_info[peer_enb_ct].p_srvd_cell_info);
                        p_uecc_gb_context->x2ap_ctx.
                            x2ap_link_info[peer_enb_ct].p_srvd_cell_info = PNULL;
                    }

                    p_uecc_gb_context->x2ap_ctx.
                        x2ap_link_info[peer_enb_ct].num_served_cells =
                        p_x2ap_status_msg_ind->srvd_cell.num_served_cells;

                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                            facility_name, RRC_INFO, "%d Serving Cells present"
                            " in Peer EnodeB Id %d ",
                            p_x2ap_status_msg_ind->srvd_cell.num_served_cells,
                            peer_enb_ct);
                    if((PNULL != (p_uecc_gb_context->x2ap_ctx.
                           x2ap_link_info[peer_enb_ct].p_srvd_cell_info = 
                            rrc_mem_get(sizeof(x2ap_srvd_cell_info_t)*
                         p_x2ap_status_msg_ind->srvd_cell.num_served_cells))) &&
                         (p_x2ap_status_msg_ind->srvd_cell.num_served_cells <=
                           MAX_SERVED_CELLS))
                    {
                        l3_memcpy_wrapper((void *)p_uecc_gb_context->x2ap_ctx.
                                x2ap_link_info[p_x2ap_status_msg_ind->peer_enodeb_id].
                                p_srvd_cell_info,
                                (const void
                                 *)(&p_x2ap_status_msg_ind->srvd_cell.srvd_cell),
                                (sizeof(x2ap_srvd_cell_info_t)* 
                                 p_x2ap_status_msg_ind->srvd_cell.num_served_cells));
                    }

                }
                #ifdef ENDC_ENABLED
                /* MENB CHANGES - START */
                else if ( X2AP_STATUS_MSG_IND_NR_SERVED_CELL_ARRAY_PRESENT &
                    p_x2ap_status_msg_ind->bitmask )
                {
                    /*
                     * Free the already existing NR served cell Info if any
                     */
                    if(PNULL != p_uecc_gb_context->x2ap_ctx.
                            x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info)
                    {
                        rrc_mem_free(p_uecc_gb_context->x2ap_ctx.
                                x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info);
                        p_uecc_gb_context->x2ap_ctx.
                            x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info = PNULL;
                    }

                    p_uecc_gb_context->x2ap_ctx.
                        x2ap_link_info[peer_enb_ct].num_served_cells =
                        p_x2ap_status_msg_ind->nr_srvd_cell.num_served_nr_cells;

                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                            facility_name, RRC_INFO, "%d NR Serving Cells present"
                            " in Peer EnodeB Id %d ",
                            p_x2ap_status_msg_ind->nr_srvd_cell.num_served_nr_cells,
                            peer_enb_ct);
                    /* coverity fix 277509 */
                    if((PNULL != (p_uecc_gb_context->x2ap_ctx.
                                    x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info = 
                                    rrc_mem_get(sizeof(x2ap_srvd_nr_cell_info_t)*
                                        p_x2ap_status_msg_ind->nr_srvd_cell.num_served_nr_cells))) &&
                            (p_x2ap_status_msg_ind->nr_srvd_cell.num_served_nr_cells <=
                             MAX_SERVED_NR_CELLS))
                    /* coverity fix 277509 */
                    {
                        p_uecc_gb_context->x2ap_ctx.x2ap_link_info[peer_enb_ct].presence_flag |=
                            X2AP_NR_SERVED_CELL_INFO_PRESENT;

                        l3_memcpy_wrapper((void *)p_uecc_gb_context->x2ap_ctx.
                                x2ap_link_info[p_x2ap_status_msg_ind->peer_enodeb_id].
                                p_nr_srvd_cell_info,
                                (const void
                                 *)(&p_x2ap_status_msg_ind->nr_srvd_cell.srvd_cell),
                                (sizeof(x2ap_srvd_nr_cell_info_t)* 
                                 p_x2ap_status_msg_ind->nr_srvd_cell.num_served_nr_cells));
                    }
                }
                /* MENB CHANGES - END */
                #endif
            }
            else
            {
                /* Free already existing served cell Info if any */
                if(PNULL != p_uecc_gb_context->x2ap_ctx.
                        x2ap_link_info[peer_enb_ct].p_srvd_cell_info)
                {
                    rrc_mem_free(p_uecc_gb_context->x2ap_ctx.
                            x2ap_link_info[peer_enb_ct].p_srvd_cell_info);
                    p_uecc_gb_context->x2ap_ctx.
                        x2ap_link_info[peer_enb_ct].p_srvd_cell_info = PNULL;
                }

                #ifdef ENDC_ENABLED 
                /* MENB CHANGES - START */
                /* Free already existing nr served cell Info if any */
                if(PNULL != p_uecc_gb_context->x2ap_ctx.
                        x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info)
                {
                    rrc_mem_free(p_uecc_gb_context->x2ap_ctx.
                            x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info);
                    p_uecc_gb_context->x2ap_ctx.
                        x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info = PNULL;
                }
                /* MENB CHANGES - END */
                #endif
            }
                
            break;
        }
        if(MAX_PEER_ENB == p_x2ap_status_msg_ind->peer_enodeb_id)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR,"Peer EnodeB Id received from X2AP is out of"
                    "range : %d", p_x2ap_status_msg_ind->peer_enodeb_id);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
            
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_handover_request_msg
 *
*   INPUTS        : OSCTXT*            p_asn1_ctx
*                   x2ap_X2AP_PDU       *p_x2ap_pdu
*                   uecc_gb_context_t *p_uecc_gb_context
*                   U8                  peer_enodeb_id
*                   rrc_module_id_t     dst_module_id
*                   U8                  *buffer
*                   U8                  procedure_code
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function processes X2AP Handover Request message
 *
 *   RETURNS:     None
 *
 ******************************************************************************/
void uecc_x2ap_process_handover_request_msg
(
    OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    rrc_module_id_t     dst_module_id,
    U8                  *buffer,
/*BUG 604 changes start*/
    U8                  procedure_code,
    U32                 enb_ue_s1ap_id,
    U16                 source_ue_index,
    U8                  security_key_present
/*BUG 604 changes stop*/
)
{
    rrc_ue_index_t  ue_index =  RRC_NULL;
    x2ap_Cause      result;
    rrc_x2ap_handover_request_t msg;
    uecc_ue_context_t*   p_ue_context = PNULL;
    U8 index = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;
    rrc_x2ap_error_indication_t error_indication;
    uecc_x2ap_error_ind_bool_t        send_err_ind;
    rrc_return_et response = RRC_FAILURE;
    U16     index_counter = RRC_NULL;
    EVENT_EXTERNAL_HEADER           *p_event_header = PNULL;
    U32                             encoded_msg_len = 0;
    
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);	
    
    memset_wrapper(&error_indication, RRC_NULL, sizeof(rrc_x2ap_error_indication_t));
    memset_wrapper(&send_err_ind, RRC_NULL, sizeof(uecc_x2ap_error_ind_bool_t));
    do
    {
        memset_wrapper (&msg, RRC_NULL,
             sizeof(rrc_x2ap_handover_request_t));
        if(PNULL ==
                p_x2ap_pdu->u.initiatingMessage->value.u.handoverPreparation)
        {
            result.t = T_x2ap_Cause_protocol;
            result.u.protocol = x2ap_transfer_syntax_error;

            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR, "[X2AP_Handover_Request] "
                    "handoverPreparation field is NULL");
            break;
        }

        /* Decode message */
        response = x2ap_internal_decode_handover_request(
                p_asn1_ctx,
                p_x2ap_pdu->u.initiatingMessage->value.u.handoverPreparation,
                &msg,
                peer_enodeb_id,
                &error_indication,
                &send_err_ind
                );
        if (response == RRC_FAILURE)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, " Handover Request "
                    "Decoding Failure");
        }
        /*match the peer_enodeb_id and x2ap_Old_UE_X2AP_ID if these are 
          already present in ue contexts*/ 
        for (index_counter = 0;
            index_counter < p_uecc_gb_context->total_ue_supported;
            index_counter++)
        {
            if (PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter])
            {
/*BUG 604 changes start*/
                /*SPR_16604_START*/
                if  ( (ASN1V_x2ap_id_handoverPreparation == procedure_code) &&
                     (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                    x2ap_context.peer_enodeb_id == peer_enodeb_id) && 
                    (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                    x2ap_context.old_enb_ue_x2ap_id == msg.x2ap_Old_UE_X2AP_ID) &&
                    (PNULL != p_uecc_gb_context->ue_db.
                     ue_contexts[index_counter]->ho_info.p_ho_info) &&
                    (PNULL != p_uecc_gb_context->ue_db.
                     ue_contexts[index_counter]->ho_info.p_ho_info->p_trg_ho_info))
                {
                    /* SPR 16750 Fix Start */
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                            p_uecc_gb_context->facility_name, 
                            RRC_ERROR, " Handover Request - failure: "
                            "received x2ap_Old_UE_X2AP_ID = %u "
                            "is already present = % u", msg.x2ap_Old_UE_X2AP_ID,
                            (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                             x2ap_context.old_enb_ue_x2ap_id));
                    /*SPR_16604_END*/
                    error_indication.bitmask |= RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
                    error_indication.old_enb_ue_x2ap_id = msg.x2ap_Old_UE_X2AP_ID; 
                    error_indication.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                    error_indication.cause.t = T_x2ap_Cause_radioNetwork;
                    error_indication.cause.u.radioNetwork = x2ap_unknown_old_eNB_UE_X2AP_ID;
                    error_indication.bitmask |= RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;
                    error_indication.criticality_diagnostics.m.procedureCodePresent = 1;
                    error_indication.criticality_diagnostics.procedureCode =
                                          ASN1V_x2ap_id_handoverPreparation;
                    error_indication.criticality_diagnostics.m.triggeringMessagePresent = 1;
                    error_indication.criticality_diagnostics.triggeringMessage =
                                                                x2ap_initiating_message;
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                            facility_name, RRC_INFO,
                            "[x2ap_ErrorIndication] push event - UECC_EV_X2AP_ERROR_INDICATION");
                    if (RRC_NULL == p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                                       x2ap_context.m.error_indication_ongoing)
                    {
                        p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                             x2ap_context.m.error_indication_ongoing = 1;
                        uecc_fsm_process_event(
                            p_uecc_gb_context->ue_db.ue_contexts[index_counter],
                            UECC_EV_X2AP_ERROR_INDICATION,
                            &error_indication);
                    }
                    /* SPR 16750 Fix Stop */
                    response = RRC_FAILURE;
                    break;
                }
/*BUG 604 changes stop*/
            }
        }

        if (X2AP_TRUE == send_err_ind.send_err_indication_reject_ie_present ||
                response == RRC_FAILURE)
        {
            result.t = T_x2ap_Cause_protocol;
            result.u.protocol = x2ap_abstract_syntax_error_falsely_constructed_message;

            if (RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT & error_indication.bitmask)
            {
                /* Send HO Preparation Failure Msg */
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, " Sending Handover Preparation "
                    "Failure message to X2AP Module");

                uecc_x2ap_build_and_send_handover_prep_failure_at_init(
                    p_uecc_gb_context,
                    msg.x2ap_Old_UE_X2AP_ID,
                    &result,
                    peer_enodeb_id,
                    &error_indication,
                    procedure_code);
            }
            else
            {
                /* Send X2AP Error-Indication Message to X2AP Module */
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, " Sending X2AP Error-Indication  "
                    "message to X2AP Module");

/*BUG 604 changes start*/
                if (ASN1V_x2ap_id_handoverPreparation == procedure_code)
/*BUG 604 changes stop*/
                {
                    uecc_x2ap_build_and_send_err_indication( 
                            peer_enodeb_id,      /* Peer EnodeB ID */
                            &error_indication);
                }
            }

            break;
        }

        if ( PNULL != p_uecc_gb_context->p_p_csc_context )
        {
            for ( index = 0; index < MAX_NUM_CELLS; index++ )
            {
                if ( PNULL != p_uecc_gb_context->p_p_csc_context[index] )
                {
                    if ( PNULL != p_uecc_gb_context->p_p_csc_context[index]->
                            p_csc_init_setup_ind)
                    {
                        if (RRC_NULL == memcmp_wrapper (p_uecc_gb_context->
                                    p_p_csc_context[index]->p_csc_init_setup_ind->
                                    sib_type_1_Info.
                                    cell_access_related_info.cell_Id,
                                    msg.target_cell_id.
                                    eUTRANcellIdentifier.data,
                                    /* SPR 15180 FIX START */
                                    /* SPR 19596 Fix Start */
                                    MACRO_ENB_ID_OCTET_SIZE))
                        {
                            if ((msg.target_cell_id.eUTRANcellIdentifier.data[3] & 0xF0) ==
                                    (p_uecc_gb_context->p_p_csc_context[index]->p_csc_init_setup_ind->
                                     sib_type_1_Info.cell_access_related_info.cell_Id[3] & 0xF0))
                                /* SPR 19596 Fix End*/
                            {
                                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                                        p_uecc_gb_context->facility_name, RRC_INFO,
                                        "[X2AP_Handover_Request] "
                                        "cell identity is matched at [%u] cell index", index);
                                cell_index = index;
                                /* SPR 18233 Fix Start */
                                if (RRM_RRC_CELL_BARRED == p_uecc_gb_context->p_p_csc_context[index]->
                                     p_csc_init_setup_ind->sib_type_1_Info.cell_access_related_info.cell_barred ) 
                                {
                                    result.t = T_x2ap_Cause_radioNetwork;
                                    result.u.radioNetwork = x2ap_cell_not_available;
                                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                                        facility_name, RRC_WARNING,
                                        "[X2AP Handover Request ] Call "
                                        " not allowed in Cell Barred State");

                                    /* Send HO Preparation Failure Msg */
                                    uecc_x2ap_build_and_send_handover_prep_failure_at_init(
                                           p_uecc_gb_context,
                                           msg.x2ap_Old_UE_X2AP_ID,
                                           &result,
                                           peer_enodeb_id,
                                           (rrc_x2ap_error_indication_t *)PNULL,
                                           procedure_code);
                                    return;
                                }
                                else
                                {
                                break;
                            }
                                /* SPR 18233 Fix Start */
                            }
                            /* SPR 15180 Fix Stop */
                        }
                    }
                }
            }

            if ( RRC_INVALID_CELL_INDEX == cell_index )
            {
                /* Fill Handover Preparation failure cause */
                result.t = T_x2ap_Cause_radioNetwork;
                /*SPR_17484_START*/
                result.u.radioNetwork = x2ap_cell_not_available;
                /*SPR_17484_END*/
                /* UE context allocation fail */
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,
                        "[X2AP Handover Request ] Cell Index not available ");

                /* Send HO Preparation Failure Msg */
                uecc_x2ap_build_and_send_handover_prep_failure_at_init(
                        p_uecc_gb_context,
                        msg.x2ap_Old_UE_X2AP_ID,
                        &result,
                        peer_enodeb_id,
                        (rrc_x2ap_error_indication_t *)PNULL,
                        procedure_code);
                break;
            }
        }

        if (ASN1V_x2ap_id_handoverPreparation == procedure_code) 
        {
            x2ap_srvd_cell_info_t *served_cell_info;
            served_cell_info = (x2ap_srvd_cell_info_t *)p_uecc_gb_context->x2ap_ctx.x2ap_link_info[peer_enodeb_id].p_srvd_cell_info;
            if ( PNULL != p_uecc_gb_context->p_p_csc_context )
            {
                if ((CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG &
                            p_uecc_gb_context->p_p_csc_context[
                            cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
                            cell_access_related_info.presence_bitmask) &&
                        (p_uecc_gb_context->p_p_csc_context[
                         cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
                         cell_access_related_info.csg_indication == RRC_FALSE)
                   )
                {           
                    if(!(msg.bitmask & X2AP_HANDOVER_REQ_CSG_MEMBERSHIP_STATUS_PRESENT))
                    {
                        result.t = T_x2ap_Cause_radioNetwork;
                        result.u.radioNetwork = x2ap_unspecified_2;

                        /* send HO Prep FAILURE*/
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                                facility_name, RRC_ERROR, 
                                "[X2AP Handover Request]: CSG MemberShip Status not present");

                        /* Send HO Preparation Failure Msg */
                        uecc_x2ap_build_and_send_handover_prep_failure_at_init(
                                p_uecc_gb_context,
                                msg.x2ap_Old_UE_X2AP_ID,
                                &result,
                                peer_enodeb_id,
                                (rrc_x2ap_error_indication_t *)PNULL,
                                procedure_code);
                        break;
                    }
                }                           
            if (CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG &
                   p_uecc_gb_context->p_p_csc_context[
                   cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
                   cell_access_related_info.presence_bitmask)
                   {
                       if(served_cell_info->bitmask & X2AP_CSG_ID_INFO_PRESENT)
                       {
                           if(!memcmp_wrapper(served_cell_info->csg_identity,p_uecc_gb_context->p_p_csc_context[cell_index]->p_csc_init_setup_ind->sib_type_1_Info.cell_access_related_info.csg_identity,sizeof(served_cell_info->csg_identity)))
                           {
            		           RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    		      facility_name, RRC_INFO, 
                    		      "[X2AP Handover Request]: CSG ID  of source and target matched");
                           }
                           else
                           {
            			       result.t = T_x2ap_Cause_radioNetwork;
            			       result.u.radioNetwork = x2ap_unspecified_2;

             		           /* send HO Prep FAILURE*/
            		           RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    	           facility_name, RRC_ERROR, 
                    		       "[X2AP Handover Request]: CSG ID mismatch of source and target");
           		                   /* Send HO Preparation Failure Msg */
           		                uecc_x2ap_build_and_send_handover_prep_failure_at_init(
                  		              p_uecc_gb_context,
                    		          msg.x2ap_Old_UE_X2AP_ID,
                    		          &result,
                    		          peer_enodeb_id,
                    		          (rrc_x2ap_error_indication_t *)PNULL,
                    		          procedure_code);
                            break;
                           }
                       }
                       else
                       {
            			   result.t = T_x2ap_Cause_radioNetwork;
            			   result.u.radioNetwork = x2ap_unspecified_2;

             		       /* send HO Prep FAILURE*/
            		       RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    	      facility_name, RRC_ERROR, 
                    		  "[X2AP Handover Request]: CSGID info not present");
           		              /* Send HO Preparation Failure Msg */
           		              uecc_x2ap_build_and_send_handover_prep_failure_at_init(
                  		              p_uecc_gb_context,
                    		          msg.x2ap_Old_UE_X2AP_ID,
                    		          &result,
                    		          peer_enodeb_id,
                    		          (rrc_x2ap_error_indication_t *)PNULL,
                    		          procedure_code);
                            break;
				       }
                                    
                   }
               }
            }


        /* allecate UE context */
        ue_index = uecc_ue_ctx_allocate(p_uecc_gb_context, dst_module_id);

        /* Bug 650 Fix Start */
        if ((MAX_NUM_SUPPORTED_UE_PER_UECC == RRC_GET_UE_INDEX(ue_index)) ||
                (p_uecc_gb_context->total_ue_supported == ue_index ))
        /* Bug 650 Fix End */
        {
            /* Fill Handover Preparation failure cause */
            result.t = T_x2ap_Cause_radioNetwork;
            result.u.radioNetwork =
                x2ap_no_radio_resources_available_in_target_cell;

            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR, 
                    "[X2AP Handover Request]: Max UE limit reached in cell");

            /* Send HO Preparation Failure Msg */
            uecc_x2ap_build_and_send_handover_prep_failure_at_init(
                    p_uecc_gb_context,
                    msg.x2ap_Old_UE_X2AP_ID,
                    &result,
                    peer_enodeb_id,
                    (rrc_x2ap_error_indication_t *)PNULL,
                    procedure_code);
            break;
        }
        if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
                    p_uecc_gb_context,cell_index))
        {
            result.t = T_x2ap_Cause_radioNetwork;
            result.u.radioNetwork = x2ap_cell_not_available;

            /* send HO Prep FAILURE*/
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR, 
                    "[X2AP Handover Request]: Cell not avialable");

            /* Send HO Preparation Failure Msg */
            uecc_x2ap_build_and_send_handover_prep_failure_at_init(
                    p_uecc_gb_context,
                    msg.x2ap_Old_UE_X2AP_ID,
                    &result,
                    peer_enodeb_id,
                    (rrc_x2ap_error_indication_t *)PNULL,
                    procedure_code);
	        /* SPR 20686 Fix Start */
             uecc_ue_ctx_free(p_uecc_gb_context, ue_index);
            /* SPR 20686 Fix End */
            break;
        }

        /* init UE context variables, UECC FSM engine and Statistic engine */
        uecc_ue_ctx_init(p_uecc_gb_context, ue_index, RRC_INVALID_RNTI, cell_index);

        /* Get UE context */
        p_ue_context = uecc_ue_ctx_get(p_uecc_gb_context, ue_index);

        if(PNULL != p_ue_context)
        {
            /* Store old ue x2ap id */
            p_ue_context->x2ap_context.old_enb_ue_x2ap_id =
                msg.x2ap_Old_UE_X2AP_ID;

            /* Store peer enb id */
            p_ue_context->x2ap_context.peer_enodeb_id =
                peer_enodeb_id;
            
            /* Store mme ue s1ap id */
            /* Bug 897 Fix Start */
            //p_ue_context->m.mme_ue_id_present = 1; 
            /* Bug 897 Fix Stop */
            p_ue_context->mme_ue_id = 
                msg.rrc_x2ap_ue_ctx_info.ue_context_info.mME_UE_S1AP_ID;
/*BUG 604 changes start*/
            if(RRC_INTRA_CELL_HO_PROC_CODE == procedure_code)
            {
                /*Fill s1ap_ue_id same as in source side*/
                p_ue_context->s1ap_ue_id = enb_ue_s1ap_id; 
                /*Store source_ue_index in ho_info*/
                p_ue_context->ho_info.source_ue_index = 
                    source_ue_index;
                /*BUG 604 changes start*/
                p_ue_context->ho_info.security_key_present =
                    security_key_present;
                /*BUG 604 changes stop*/

            }
/*BUG 604 changes stop*/
            
            /* Store gummei */
            /* Bug 8198 Fix Start */
            p_ue_context->m.source_gummei_present = 1; 
            /* Bug 8198 Fix End */
            p_ue_context->gummei.plmn_identity.numocts
                = msg.gummei.gU_Group_ID.pLMN_Identity.numocts;
            l3_memcpy_wrapper(((void*)&p_ue_context->gummei.plmn_identity.data),
                       ((const void*)&msg.gummei.gU_Group_ID.pLMN_Identity.data),
                       p_ue_context->gummei.plmn_identity.numocts);

            p_ue_context->gummei.grp_id.numocts
                = msg.gummei.gU_Group_ID.mME_Group_ID.numocts;
            l3_memcpy_wrapper(((void*)&p_ue_context->gummei.grp_id.data),
                       ((const void*)&msg.gummei.gU_Group_ID.mME_Group_ID.data),
                       p_ue_context->gummei.grp_id.numocts);

            p_ue_context->gummei.mme_code.numocts
                = msg.gummei.mME_Code.numocts;
            l3_memcpy_wrapper(((void*)&p_ue_context->gummei.mme_code.data),
                       ((const void*)&msg.gummei.mME_Code.data),
                       p_ue_context->gummei.mme_code.numocts);

            /* Store plmn identity */
            /* Overwrite plmn_identity in UE Context */
            memset_wrapper(&p_ue_context->plmn_identity.plmn_identity,
                        RRC_NULL,
                        sizeof(p_ue_context->plmn_identity.plmn_identity));
            /*spr_22256_changes_start*/
            /* update number of UE */
            if((PNULL != p_ue_context->p_gb_context) &&
                    (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]) &&
                    (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->p_rrc_uecc_statistic))
            {
                uecc_status_cell_update_num_of_ue(p_ue_context->p_gb_context,
                        p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->
                        cell_index]->p_rrc_uecc_statistic, RRC_TRUE);
            }
            /*spr_22256_changes_end*/
	    
/*BUG 604 changes start*/
            if (ASN1V_x2ap_id_handoverPreparation == procedure_code)
/*BUG 604 changes stop*/
            {
                /* Generating Protocol Event LOCAL_X2_HANDOVER_REQUEST*/
                encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);
                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                        encoded_msg_len);
                if (PNULL != p_event_header)
                {
                    memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                                encoded_msg_len));
                    rrc_uecc_fill_protocol_event_params(p_ue_context,
                            p_event_header,
                            (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len),
                            LOCAL_X2_HANDOVER_REQUEST,
                            MESSAGE_DIRECTION_RECEIVED);

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)buffer,
                            encoded_msg_len);

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }
            }

            if(msg.rrc_x2ap_ue_ctx_info.ue_context_info.m.
                    handoverRestrictionListPresent)
            {

                uecc_s1ap_generate_plmn_identity_to_int(
                        &p_ue_context->plmn_identity.plmn_identity,
                        msg.rrc_x2ap_ue_ctx_info.ue_context_info.
                        handoverRestrictionList.servingPLMN.data);
            }
            else
            {
               /*Storing plmn_identity from target cell id*/
                uecc_s1ap_generate_plmn_identity_to_int(
                        &p_ue_context->plmn_identity.plmn_identity,
/*SPR 22843 Fix Start*/
                        msg.gummei.gU_Group_ID.pLMN_Identity.data);
/*SPR 22843 Fix End*/
            }
            
            /* SPR 16240 START */
            if(msg.rrc_x2ap_ue_ctx_info.ue_context_info.m.
                    locationReportingInformationPresent)
            {
                p_ue_context->m.location_reporting_info_present = RRC_TRUE;

                p_ue_context->location_reporting_event_type
                    = s1ap_change_of_serve_cell;
                p_ue_context->location_reporting_report_area
                    = msg.rrc_x2ap_ue_ctx_info.ue_context_info.
                        locationReportingInformation.reportArea;
            }
            /* SPR 16240 END */

            /* X2 TRACE code */
            /* TRACE_fix */
	    /*SPR 15896 Fix Start*/
                 if(!(p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
                 ( X2AP_HANDOVER_REQ_TRACE_ACTIVATION_PRESENT & msg.bitmask))
	    /*SPR 15896 Fix Stop*/
            /* TRACE_fix */
            {
                /* Allocate memory to store trace activation control and
                 * configuration parameters in Ue context */
                if (PNULL == (p_ue_context->p_ue_trace_activation_info =
                            (ue_trace_activation_info_t*)
                            rrc_mem_get(sizeof(ue_trace_activation_info_t))))
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,
                            RRC_FATAL, "[x2ap_TraceStart]"
                            "Memory Allocation Failed");
		    /* SPR 20686 Fix Start */
		    result.t = T_x2ap_Cause_radioNetwork;
            	    result.u.radioNetwork = x2ap_unspecified_2;

		    uecc_x2ap_build_and_send_handover_prep_failure_at_init(
                    p_uecc_gb_context,
                    msg.x2ap_Old_UE_X2AP_ID,
                    &result,
                    peer_enodeb_id,
                    (rrc_x2ap_error_indication_t *)PNULL,
                    procedure_code);

		    uecc_ue_ctx_free(p_uecc_gb_context, ue_index);
                     
		    /* SPR 20686 Fix End */
                    break;
                }
                else
                {
                    memset_wrapper(p_ue_context->p_ue_trace_activation_info, RRC_NULL,
                            sizeof(ue_trace_activation_info_t));
		 /*SPR 15896 Fix Start*/ 
                    /* Set trace activation flag */
                    p_ue_context->m.traceActivated |= RRC_TRACE_ACTIVATION_MME_INITIATE;
		 /*SPR 15896 Fix Stop*/ 
                    /* store e_utran_trace_id */
                    p_ue_context->p_ue_trace_activation_info->trace_id_info.
                    eutran_trace_id.numoctet = 
                        msg.trace_activation.eUTRANTraceID.numocts;
                    l3_memcpy_wrapper(
                            p_ue_context->p_ue_trace_activation_info->trace_id_info.
                            eutran_trace_id.data,
                            msg.trace_activation.eUTRANTraceID.data,
                            msg.trace_activation.eUTRANTraceID.numocts);

                    /* store interfaces to trace */
                    p_ue_context->p_ue_trace_activation_info->interfaces_to_trace = 0;
                    if (msg.trace_activation.interfacesToTrace.data[0] & TRACE_S1AP_INTERFACE)
                        p_ue_context->p_ue_trace_activation_info->interfaces_to_trace |= S1;
                    if (msg.trace_activation.interfacesToTrace.data[0] & TRACE_X2AP_INTERFACE)
                        p_ue_context->p_ue_trace_activation_info->interfaces_to_trace |= X2;
                    if (msg.trace_activation.interfacesToTrace.data[0] & TRACE_UU_INTERFACE)
                        p_ue_context->p_ue_trace_activation_info->interfaces_to_trace |= UU;

                    /* store trace depth */
                    p_ue_context->p_ue_trace_activation_info->trace_depth =
                        (trace_depth_et)msg.trace_activation.traceDepth; 

                    /* store trace collection entity info */
                    if (!(msg.trace_activation.traceCollectionEntityIPAddress.numbits
                                % RRC_OCTET_SIZE))
                    {
                        p_ue_context->p_ue_trace_activation_info->
                            trace_collection_entity_info.length =
                            (U8)(msg.trace_activation.traceCollectionEntityIPAddress.numbits
                            / RRC_OCTET_SIZE);
                    }
                    else
                    {
                        p_ue_context->p_ue_trace_activation_info->
                            trace_collection_entity_info.length =
                            (U8)((msg.trace_activation.traceCollectionEntityIPAddress.numbits
                             / RRC_OCTET_SIZE) + 1);
                    }
                    l3_memcpy_wrapper(p_ue_context->p_ue_trace_activation_info->
                            trace_collection_entity_info.data,
                            msg.trace_activation.traceCollectionEntityIPAddress.data,
                            p_ue_context->p_ue_trace_activation_info->
                            trace_collection_entity_info.length);
		    /*SPR 15896 Fix Start*/
                    /* create and open trace file */
                    if (PNULL == p_ue_context->p_ue_trace_activation_info->fp)
                    {
                        uecc_generate_trace_file_name(p_ue_context,TRACE_X2AP_INITIATED);
                    }

		    /*SPR 15896 Fix Stop*/
                }
            }
            /* TRACE_fix */
            else
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_INFO, 
                        "Cell Traffic Trace is already ON or TRACE ACTIVATION IE not present!!");
            }
            /* TRACE_fix */

		/*SPR 15896 Fix Start*/
	    if(!(p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
		/*SPR 15896 Fix Stop*/
			    (PNULL != p_uecc_gb_context->p_p_csc_context)  &&
			    p_uecc_gb_context->p_p_csc_context[p_ue_context->cell_index]->cell_trace_ongoing == RRC_TRUE )
	    {

                /* Call the function to activate trace for existing UE's in cell 
                 * Same function is used by S1 as well as X2 */
                uecc_oamh_s1_ho_start_cell_traffic_trace(p_uecc_gb_context, p_ue_context);
            }

            /* X2 HandoverRequest message Tracing */

            if (ASN1V_x2ap_id_handoverPreparation == procedure_code) 
            {
		    /* spr_11591_fix_start */
		    /* X2_TRACE fix */
		    /* last argument - 0 indicates HandoverRequest is received */
		    uecc_x2ap_trace_handover_request(p_ue_context, &msg, encoded_msg_len, buffer, 0);
		    /* X2_TRACE fix */
		    /* spr_11591_fix_stop */
                p_ue_context->ho_info.s1_or_x2_handover = X2_HO;
            }
            else if (RRC_INTER_CELL_HO_PROC_CODE == procedure_code)
            {
                p_ue_context->ho_info.s1_or_x2_handover = INTER_CELL_HO;
                /*CR 019 changes start*/ 
                p_ue_context->ho_info.inter_cell_ho_cause = msg.cause;
                /*CR 019 changes stop*/
            } 
/*BUG 604 changes start*/
            else if (RRC_INTRA_CELL_HO_PROC_CODE == procedure_code)
            {
                p_ue_context->ho_info.s1_or_x2_handover = INTRA_CELL_HO;
        		/*SPR_17284_START*/
                p_ue_context->ho_info.intra_cell_ho_cause = msg.cause;
        		/*SPR_17284_END*/
            } 
/*BUG 604 changes stop*/

            uecc_fsm_process_event(
              p_ue_context,
              UECC_EV_X2AP_HO_REQUEST,
              &msg);
        }
        else 
        {
            result.t = T_x2ap_Cause_misc;
            result.u.misc = x2ap_unspecified;
            
            /* send HO Prep FAILURE*/
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                    p_uecc_gb_context->facility_name, RRC_ERROR, 
                    "[X2AP Handover Request]: UE Context Allocation Failure");

            /* Send HO Preparation Failure Msg */
            uecc_x2ap_build_and_send_handover_prep_failure_at_init(
                    p_uecc_gb_context,
                    msg.x2ap_Old_UE_X2AP_ID,
                    &result,
                    peer_enodeb_id,
                    (rrc_x2ap_error_indication_t *)PNULL,
                    procedure_code);
            break;
        } 
    }
    while (0);

    /* SPR 16750 Fix Start */
    if ((RRC_FAILURE == response) &&
            (ASN1V_x2ap_id_handoverPreparation == procedure_code))
    {
        if ((RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT & error_indication.bitmask) &&
                (T_x2ap_Cause_radioNetwork & error_indication.cause.t) &&
                (x2ap_unknown_old_eNB_UE_X2AP_ID & error_indication.cause.u.radioNetwork))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "Send X2AP ERROR Indication for HO REQUEST");
            uecc_x2ap_build_and_send_err_indication( 
                            peer_enodeb_id,      /* Peer EnodeB ID */
                            &error_indication);
        }
    }
    /* SPR 16750 Fix Stop */
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/*SPR_16756_START*/
/******************************************************************************
 *   FUNCTION NAME: rrc_x2ap_get_enb_ue_x2ap_id_from_handover_req_ack
 *
*   INPUTS        : uecc_gb_context_t *p_uecc_gb_context
*                   x2ap_HandoverRequestAcknowledge      *p_3gpp
*                   x2ap_UE_X2AP_ID                      *p_old_enb_ue_x2ap_id
*                   x2ap_UE_X2AP_ID                      *p_new_enb_ue_x2ap_id
*                   U8                  peer_enodeb_id
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function processes to get old and new enb ue x2ap id from Handover
 *       Request Ack message
 *
 *   RETURNS:     None
 *
 ******************************************************************************/
rrc_return_et rrc_x2ap_get_enb_ue_x2ap_id_from_handover_req_ack (
        uecc_gb_context_t                    *p_uecc_gb_context, 
        x2ap_HandoverRequestAcknowledge      *p_3gpp,
        x2ap_UE_X2AP_ID                      *p_old_enb_ue_x2ap_id,
        x2ap_UE_X2AP_ID                      *p_new_enb_ue_x2ap_id
)
/*SPR_16756_END*/

{
    OSRTDListNode             *p_node = PNULL;
    x2ap_HandoverRequestAcknowledge_protocolIEs_element 
        *p_protocol_IE = PNULL;
    rrc_return_et   result = RRC_SUCCESS;
    U32   index = RRC_NULL;
    
    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                result = RRC_FAILURE;
                break;
            }

            p_protocol_IE =
                (x2ap_HandoverRequestAcknowledge_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {   
                case ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID:
                    {
                        *(p_old_enb_ue_x2ap_id)= 
                            p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_1;
                    }
		    break;
		    /*SPR_16756_START*/
		case ASN1V_x2ap_id_New_eNB_UE_X2AP_ID:
		    {
			    *(p_new_enb_ue_x2ap_id)= 
				    p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_2;
		    }
		    break;
		    /*SPR_16756_END*/
		default:
		    {
                        break;                  
                    }
            }
            p_node= p_node->next;            
        }                    
    }while (0);

    RRC_UT_TRACE_EXIT();
    return result;    
}


/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_handover_request_ack_msg
 *
*   INPUTS        : OSCTXT*            p_asn1_ctx
*                   x2ap_X2AP_PDU       *p_x2ap_pdu
*                   uecc_gb_context_t *p_uecc_gb_context
*                   U8                  peer_enodeb_id
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function processes X2AP Handover Request Ack message
 *
 *   RETURNS:     None
 *
 ******************************************************************************/
void uecc_x2ap_process_handover_request_ack_msg
( 
    OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
/*BUG 604 changes start*/
    U8                  *buffer,
    U16                 target_ue_index
/*BUG 604 changes stop*/
)
{
	uecc_ue_context_t               *p_ue_context = PNULL;
	rrc_x2ap_handover_req_ack_t     msg;
	/* GDB fix - compiler warning */
	x2ap_UE_X2AP_ID                 old_enb_ue_x2ap_id  = 0; 
	/*SPR_16756_START*/
	x2ap_UE_X2AP_ID                 new_enb_ue_x2ap_id = 0;
	/*SPR_16756_END*/

	rrc_x2ap_error_indication_t     error_indication;
	rrc_return_et                   response = RRC_SUCCESS;
	U16     index_counter = RRC_NULL;
	LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE_STRUCT 
		*p_x2_handover_req_ack = PNULL;
	U32                         encoded_msg_len = 0;
	OSRTDListNode* p_node = PNULL;
	x2ap_E_RAB_List_element *p_elem = PNULL;
	x2ap_E_RAB_List_element *p_erab_not_admit_elem = PNULL;
	x2ap_E_RABs_Admitted_List_element *p_erab_admit_elem = PNULL;
	OSRTDListNode* p_node_msg = PNULL;
    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/

	RRC_ASSERT(PNULL!=p_x2ap_pdu);
	RRC_ASSERT(PNULL!=p_x2ap_pdu->u.successfulOutcome);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

	memset_wrapper(&msg, 0, sizeof(rrc_x2ap_handover_req_ack_t));
	memset_wrapper(&error_indication, 0, sizeof(rrc_x2ap_error_indication_t));



	do
	{
		if (PNULL==p_x2ap_pdu->u.successfulOutcome->value.u.handoverPreparation)
		{
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name, RRC_WARNING,
					"[X2AP Handover Request Ack] handoverPreparation field is PNULL");
			break;
		}
		/*SPR_16756_START*/
		rrc_x2ap_get_enb_ue_x2ap_id_from_handover_req_ack(
				p_uecc_gb_context, 
				p_x2ap_pdu->u.successfulOutcome->value.u.handoverPreparation,
				&old_enb_ue_x2ap_id,
				&new_enb_ue_x2ap_id
				);
		/*SPR_16756_END*/
		/*SPR 5253 Start */
		/*BUG:604 Fixes start*/
		/*SPR_16618_START*/

        /*SPR_17792_start*/
        if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
        {
            p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                    p_uecc_gb_context,
                    peer_enodeb_id, old_enb_ue_x2ap_id, OLD_X2AP_UE_ID);
        }
        else
        {
            p_ue_context = uecc_ue_ctx_get_by_x2_id_src_intra_cell_ho(
                    p_uecc_gb_context,old_enb_ue_x2ap_id, OLD_X2AP_UE_ID);
        }
	
        /*SPR_17792_end*/
            /*SPR_16618_END*/
            /*BUG:604 Fixes stop*/
            /*SPR 5253 Stop */

		if (PNULL==p_ue_context )
		{

            /*SPR_17792_start*/
            if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
            {
                /* Error. Can't find UE context */
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,
                        "[X2AP Handover Request Ack] UE context not found");

                /* Drop message */
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
                /*SPR_16756_START*/
                error_indication.old_enb_ue_x2ap_id = old_enb_ue_x2ap_id;
                /*SPR_16604_START*/
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
                error_indication.new_enb_ue_x2ap_id = new_enb_ue_x2ap_id;
                /*SPR_16604_END*/
                /*SPR_16756_END*/
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                error_indication.cause.t = T_x2ap_Cause_radioNetwork;
                error_indication.cause.u.radioNetwork = x2ap_unknown_old_eNB_UE_X2AP_ID;
                /* SPR 16750 Fix Start */
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;
                error_indication.criticality_diagnostics.m.procedureCodePresent = 1;
                error_indication.criticality_diagnostics.procedureCode =
                    ASN1V_x2ap_id_handoverPreparation;
                error_indication.criticality_diagnostics.m.triggeringMessagePresent = 1;
                error_indication.criticality_diagnostics.triggeringMessage =
                    x2ap_successful_outcome;
            }
            /* SPR 16750 Fix Stop */
            response = RRC_FAILURE;

			break;
	
            /*SPR_17792_end*/
		}
		else
		{
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name, RRC_INFO, 
					"[X2AP Handover Request Ack] UE context found");
		}


		if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
					p_uecc_gb_context,p_ue_context->cell_index))
		{
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name, RRC_WARNING,"cell not available");
			RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
			return;
		}
		/*
		 * If X2 Handover Flag in UE Context i.e. X2 handover procedure is ongoing, 
		 * then only the message would be processed,
		 * Else the message should be discarded
		 */	  	  
		/*BUG 604 changes start*/
		if((X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover) ||
				(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
				(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover))
			/*BUG 604 changes stop*/
		{	
			/* Decode message */ 
			if( X2AP_FAILURE == x2ap_internal_decode_handover_request_ack(
						p_asn1_ctx,
						&p_ue_context->ho_info.p_ho_info->ho_s1ap_asn1_ctx,
						p_x2ap_pdu->u.successfulOutcome->value.u.handoverPreparation,
						&msg,
						peer_enodeb_id))
			{
				RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
						facility_name, RRC_INFO, " Handover Command "
						"Decoding Failure");
				break;
			}
			else
			{
				RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
						facility_name, RRC_INFO, " Handover Command "
						"Decoding Success");

				if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
				{
					/*Generating Protocol Events LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE */
					encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);
					p_x2_handover_req_ack = rrc_mem_get(sizeof(LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE_STRUCT) + 
							encoded_msg_len);
					if (PNULL != p_x2_handover_req_ack)
					{
						memset_wrapper(p_x2_handover_req_ack, RRC_NULL, 
								(sizeof(LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE_STRUCT) + encoded_msg_len));

						p_x2_handover_req_ack->header.length = sizeof(LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE_STRUCT) 
							+ encoded_msg_len;
						p_x2_handover_req_ack->header.event_id = LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE;

						if ((PNULL != p_uecc_gb_context->p_p_csc_context) && (PNULL !=
									p_uecc_gb_context->p_p_csc_context[p_ue_context->cell_index])
								&& (PNULL != p_uecc_gb_context->p_p_csc_context[p_ue_context->cell_index]->
									p_csc_init_setup_ind))
						{
							l3_memcpy_wrapper(&p_x2_handover_req_ack->header.EVENT_PARAM_CELL_ID, p_uecc_gb_context->
									p_p_csc_context[p_ue_context->cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
									cell_access_related_info.cell_Id, sizeof(U32));
						}
						p_x2_handover_req_ack->header.EVENT_PARAM_EUTRANCELL_FROID = p_x2_handover_req_ack->header.
							EVENT_PARAM_CELL_ID;

						if (p_ue_context->m.mme_ue_id_present)
						{
							p_x2_handover_req_ack->header.EVENT_PARAM_MMES1APID = p_ue_context->mme_ue_id;
						}

						p_x2_handover_req_ack->header.EVENT_PARAM_RAC_UE_REF = p_ue_context->s1ap_ue_id;

						p_x2_handover_req_ack->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_RECEIVED;
						p_x2_handover_req_ack->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;

						for (p_node = msg.e_rab_not_admitted_list.head;
								PNULL != p_node; p_node=p_node->next)
						{
							p_elem = (x2ap_E_RAB_List_element*)p_node->data;

							RRC_ASSERT(PNULL!=p_elem);

							if (ASN1V_x2ap_id_E_RAB_Item!=p_elem->id)
							{
								RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
										p_ue_context->p_gb_context->facility_name, RRC_WARNING,
										"ASN1V_x2ap_id_E_RAB_Item! =p_elem"
										"->id (%i)",
										p_elem->id);
								break;
							}
							/* Filling Bitmap for Protocol Event
							 * LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE*/
							p_x2_handover_req_ack->EVENT_PARAM_HO_OUT_PREP_ERAB_FAIL_BITMAP |=
								(1 << p_elem->value.u._x2ap_E_RAB_ItemIEs_1->e_RAB_ID);
						}

						l3_memcpy_wrapper((void*)(((U8*)p_x2_handover_req_ack) + 
									sizeof(LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE_STRUCT)),
								(const void*)p_asn1_ctx,
								encoded_msg_len);
						rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_x2_handover_req_ack);
					}
				}
			}


			/*BUG 604 changes start*/
			/* BUG 10849 changes start */
			if (X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
				/* BUG 10849 changes stop */
			{
				/*BUG 604 changes stop*/
				/*match the peer_enodeb_id and x2ap_Old_UE_X2AP_ID if these are 
				  already present in ue contexts*/ 
				for (index_counter = 0;
						index_counter < p_uecc_gb_context->total_ue_supported;
						index_counter++)
				{
					if (PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter])
					{
						/*SPR_16604_START*/
						/* SPR 16750 Fix Start */
						if( (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->ue_index != 
									p_ue_context->ue_index) &&
								(p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
								 /* SPR 16750 Fix Stop */
								 x2ap_context.m.new_enb_ue_x2ap_id_present) && 
								(p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
								 x2ap_context.new_enb_ue_x2ap_id == msg.new_ue_x2ap_id) && 
								(p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
								 x2ap_context.peer_enodeb_id == peer_enodeb_id) &&
								(PNULL != p_uecc_gb_context->ue_db.
								 ue_contexts[index_counter]->ho_info.p_ho_info) &&
								(PNULL != p_uecc_gb_context->ue_db.
								 ue_contexts[index_counter]->ho_info.p_ho_info->p_src_ho_info))
						{
							/* SPR 16750 Fix Start */
							RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
									facility_name, RRC_ERROR," Handover Request - failure: " 
									"received new_ue_x2ap_id = %u "
									"is already present in existing ue contexts = %u", msg.new_ue_x2ap_id,
									(p_uecc_gb_context->ue_db.ue_contexts[index_counter]->ue_index));
							/* SPR 16750 Fix Stop */
							error_indication.bitmask |=
								RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
							error_indication.old_enb_ue_x2ap_id = msg.old_ue_x2ap_id;
							/*SPR_16604_END*/
							error_indication.bitmask |=
								RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
							error_indication.new_enb_ue_x2ap_id = msg.new_ue_x2ap_id;
							error_indication.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
							error_indication.cause.t = T_x2ap_Cause_radioNetwork;
							error_indication.cause.u.radioNetwork =
								x2ap_unknown_new_eNB_UE_X2AP_ID;
							/* SPR 16750 Fix Start */
							error_indication.bitmask |= RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;
							error_indication.criticality_diagnostics.m.procedureCodePresent = 1;
							error_indication.criticality_diagnostics.procedureCode =
								ASN1V_x2ap_id_handoverPreparation;
							error_indication.criticality_diagnostics.m.triggeringMessagePresent = 1;
							error_indication.criticality_diagnostics.triggeringMessage =
								x2ap_successful_outcome;
							if (RRC_NULL == p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
									x2ap_context.m.error_indication_ongoing)
							{
								p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
									x2ap_context.m.error_indication_ongoing = 1;
								uecc_fsm_process_event(
										p_uecc_gb_context->ue_db.ue_contexts[index_counter],
										UECC_EV_X2AP_ERROR_INDICATION,
										&error_indication);
							}
							/* SPR 16750 Fix Stop */
							response = RRC_FAILURE;
							break;
						}
					}
				}
				/*BUG 604 changes start*/
			}
			/*BUG 604 changes stop*/
			if (response == RRC_FAILURE)
			{
				break;
			}

			/* spr_11591_fix_start */
			if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
            {
                /* spr_11591_fix_stop */
                /*SPR 15896 Fix Start*/
                if (p_ue_context->m.traceActivated)
                {
                    /*SPR 15875 Fix Start*/
                    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
                            (p_ue_context->p_ue_trace_activation_info != PNULL) &&
                            (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & X2))
                    {
                        P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_MME, "\n <msg function=\"X2AP\" name=\"Handover Request Ack\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
                                difftime_wrapper(p_ue_context->p_ue_trace_activation_info->rawtime));
                    }
                    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
                            (p_ue_context->p_ue_cell_trace_activation_info != PNULL) &&
                            (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & X2))
                    {
                        P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL, "\n <msg function=\"X2AP\" name=\"Handover Request Ack\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
                                difftime_wrapper(p_ue_context->p_ue_cell_trace_activation_info->rawtime));
                    }
                    /*SPR 15875 Fix Stop*/
                    P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), X2,"\n <initiator type=\"ENB\">%s</initiator>",
                            p_ue_context->p_gb_context->x2ap_ctx.
                            x2ap_link_info[p_ue_context->x2ap_context.peer_enodeb_id].enb_comm_info.ip_addr[0].ip_addr);

                    rrc_trace_max_file_write(p_ue_context,X2AP_HANDOVER_REQUEST_ACK,encoded_msg_len, buffer, X2);

                    P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask (),X2, "\n <ieGroup name=\"E-RABs Admitted List\">");
                    for (p_node_msg = msg.e_rab_admitted_list.head; PNULL!=p_node_msg;
                            p_node_msg=p_node_msg->next)
                    {
                        p_erab_admit_elem=(x2ap_E_RABs_Admitted_List_element*)p_node_msg->data;
                        if ( p_erab_admit_elem == PNULL)
                        {
                            continue;
                        }
                        P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"E-RABs Admitted Item\">");
                        P_RRC_TRACE (p_ue_context,set_minimum_medium_both_bitmask (), X2, "\n <ie name=\"E-RAB_ID\">%d</ie>  ",
                                p_erab_admit_elem->value.e_RAB_ID);
                        P_RRC_TRACE (p_ue_context,set_minimum_medium_both_bitmask(), X2, "\n </ieGroup>");
                    }
                    P_RRC_TRACE (p_ue_context,set_minimum_medium_both_bitmask(), X2, "\n </ieGroup>");

                    P_RRC_TRACE (p_ue_context,set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"E-RABs NotAdmitted List\">");
                    for (p_node_msg = msg.e_rab_not_admitted_list.head; PNULL!=p_node_msg;
                            p_node_msg=p_node_msg->next)
                    {
                        p_erab_not_admit_elem=(x2ap_E_RAB_List_element*)p_node_msg->data;
                        if ( (p_erab_not_admit_elem == PNULL) || 
                                (PNULL == p_erab_not_admit_elem->value.u._x2ap_E_RAB_ItemIEs_1))
                        {
                            continue;
                        }
                        P_RRC_TRACE (p_ue_context,set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"E-RABs List Item\">");
                        P_RRC_TRACE (p_ue_context,set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"E-RAB_ID\">%d</ie>  ",
                                p_erab_not_admit_elem->value.u._x2ap_E_RAB_ItemIEs_1->e_RAB_ID);
                        P_RRC_TRACE (p_ue_context,set_minimum_medium_both_bitmask(), X2, "\n </ieGroup>");
                    }
                    P_RRC_TRACE (p_ue_context,set_minimum_medium_both_bitmask(), X2, "\n</ieGroup>");
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(),X2,"\n </msg>");
                    /*SPR 15896 Fix Stop*/
                }


            }
			/*BUG 604 changes start*/
			if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
			{
				/*Store the target_ue_index in ho_info*/
				p_ue_context->ho_info.target_ue_index = 
					target_ue_index;
			}
			/*BUG 604 changes stop*/
			/* Call      event handler *
			 * and pass API data to FSM. */
			/* WARNING. The uecc_event_queue_push_event can't be used here because
			 * the stack variable is used for storing the API data
			 * */
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name, RRC_INFO, 
					"Handover Request Ack : push event UECC_EV_X2AP_HO_REQ_ACK");
			uecc_fsm_process_event(
					p_ue_context,
					UECC_EV_X2AP_HO_REQ_ACK, 
					&msg);
		}
		else
		{
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name, RRC_WARNING, 
					"Handover Request Ack message is discarded");
			break;
		}


	} while (0);
	if (RRC_FAILURE == response)
	{
		if ((RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT &
					error_indication.bitmask) ||
				(RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT &
				 error_indication.bitmask) ||
				(RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT &
				 error_indication.bitmask))
		{
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name, RRC_INFO, 
					"Handover Request Ack message: "
					"build and send error-indication message");
			uecc_x2ap_build_and_send_err_indication(
					peer_enodeb_id,
					&error_indication);
			/* SPR 16750 Fix Start */
			if ((PNULL != p_ue_context) && 
					(RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT & error_indication.bitmask) &&
					(T_x2ap_Cause_radioNetwork & error_indication.cause.t) &&
					(x2ap_unknown_new_eNB_UE_X2AP_ID & error_indication.cause.u.radioNetwork))
			{
				if (RRC_NULL == p_ue_context->x2ap_context.m.error_indication_ongoing)
				{
					p_ue_context->x2ap_context.m.error_indication_ongoing = 1;
					uecc_fsm_process_event(
							p_ue_context,
							UECC_EV_X2AP_ERROR_INDICATION,
							&error_indication);
				}
			}
			/* SPR 16750 Fix Stop */
		}
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_handover_prep_failure_msg
 *
*   INPUTS        : OSCTXT*            p_asn1_ctx
*                   x2ap_X2AP_PDU       *p_x2ap_pdu
*                   uecc_gb_context_t *p_uecc_gb_context
*                   U8                  peer_enodeb_id
*                   U8                  *buffer
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function processes X2AP Handover Preparation Fail message
 *
 *   RETURNS:     None
 *
 ******************************************************************************/
void uecc_x2ap_process_handover_prep_failure_msg
(
    OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    U8                  *buffer
)
{
    uecc_ue_context_t               *p_ue_context = PNULL;
    rrc_x2ap_handover_prep_fail_t   msg;
    
    /*rrc_x2ap_error_indication_t     error_indication;
    rrc_return_et                   response = RRC_SUCCESS;*/
    EVENT_EXTERNAL_HEADER           *p_event_header = PNULL;
    U32                             encoded_msg_len = 0;
    /* SPR 16750 Fix Start */
    rrc_x2ap_error_indication_t     error_indication;
    /* SPR 16750 Fix Stop */

    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/

    RRC_ASSERT(PNULL!=p_x2ap_pdu);
    RRC_ASSERT(PNULL!=p_x2ap_pdu->u.unsuccessfulOutcome);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    
    /* initializing the msg with all bits set to zero */
    memset_wrapper(&msg, 0, sizeof(rrc_x2ap_handover_prep_fail_t));
    do
    {
        if (PNULL == p_x2ap_pdu->u.unsuccessfulOutcome->value.u.
                handoverPreparation)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING,
                    "[X2AP Handover Preparation Failure] "
                    "handoverPreparation field is PNULL");
            break;
        }

        /*X2AP FIX_HO_7 : Start*/
        /* Decode message */
        if( X2AP_FAILURE == x2ap_internal_decode_handover_prep_fail(
              p_asn1_ctx,      
              p_x2ap_pdu->u.unsuccessfulOutcome->value.u.handoverPreparation,
              &msg,
              peer_enodeb_id
              ))
        {
          RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                  facility_name, RRC_INFO, " Handover Preparation Failure"
              "Decoding Failure");
          break;
        }
        else
        {
           
            rrc_asn1PrtToStr_X2AP_PDU(RRC_ASN, "X2AP_PDU", p_x2ap_pdu);
        }

/*BUG:604 Fixes start*/
        /*SPR_16618_START*/
        /*SPR_17792_start*/
        if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
        {
            p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                    p_uecc_gb_context,
                    peer_enodeb_id,msg.x2ap_Old_UE_X2AP_ID, OLD_X2AP_UE_ID);
        }
        else
        {
            p_ue_context = uecc_ue_ctx_get_by_x2_id_src_intra_cell_ho(
                    p_uecc_gb_context,msg.x2ap_Old_UE_X2AP_ID, OLD_X2AP_UE_ID);
        }

        /*SPR_17792_end*/

        /*SPR_16618_END*/
/*BUG:604 Fixes stop*/

        if (PNULL != p_ue_context )
        {
            if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
                        p_uecc_gb_context,p_ue_context->cell_index))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,"cell not available");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                break;
            }
        }

        /* X2AP FIX_AG_2 : Start*/ /*Removed HO_TYPE Checked*/
        /* Process message */
        /* Get context using old enb ue x2ap id */

        if (PNULL == p_ue_context )
        {
            /*SPR_17792_start*/
            if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
            {
                /* Error. Can't find UE context */
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,
                        "[X2AP Handover Preparation Failure] UE context not found");

                /* Drop message */
                /* SPR 16750 Fix Start */
                memset_wrapper(&error_indication, 0, sizeof(rrc_x2ap_error_indication_t));
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
                error_indication.old_enb_ue_x2ap_id = msg.x2ap_Old_UE_X2AP_ID;
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                error_indication.cause.t = T_x2ap_Cause_radioNetwork;
                error_indication.cause.u.radioNetwork = x2ap_unknown_old_eNB_UE_X2AP_ID;
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;
                error_indication.criticality_diagnostics.m.procedureCodePresent = 1;
                error_indication.criticality_diagnostics.procedureCode =
                    ASN1V_x2ap_id_handoverPreparation;
                error_indication.criticality_diagnostics.m.triggeringMessagePresent = 1;
                error_indication.criticality_diagnostics.triggeringMessage =
                    x2ap_unsuccessful_outcome;
                uecc_x2ap_build_and_send_err_indication(
                        peer_enodeb_id,
                        &error_indication);
                /* SPR 16750 Fix Stop */
            }
            /*SPR_17792_end*/
 
          break;
        }
/*BUG 604 changes start*/
        else if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
/*BUG 604 changes stop*/
        {
            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                 encoded_msg_len);
            if (PNULL != p_event_header)
            {
                memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                            encoded_msg_len));
                rrc_uecc_fill_protocol_event_params(p_ue_context,
                        p_event_header,
                        (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len),
                        LOCAL_X2_HANDOVER_PREPARATION_FAILURE,
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)buffer,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
/*BUG 604 changes start*/
/*BUG 604 changes stop*/

	    	/*SPR 15896 Fix Start*/
        if (p_ue_context->m.traceActivated)
        {
	    	/*SPR 15875 Fix Start*/
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
				    (p_ue_context->p_ue_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_MME,"\n <msg function=\"X2AP\" name=\"Handover Preparation Failure\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_trace_activation_info->rawtime));
		    }
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
				    (p_ue_context->p_ue_cell_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"\n <msg function=\"X2AP\" name=\"Handover Preparation Failure\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_cell_trace_activation_info->rawtime));
		    }
	    	/*SPR 15875 Fix Stop*/
                P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), X2,"\n <initiator type=\"ENB\">%s</initiator>",
                    p_ue_context->p_gb_context->x2ap_ctx.
                    x2ap_link_info[p_ue_context->x2ap_context.peer_enodeb_id].enb_comm_info.ip_addr[0].ip_addr);

                rrc_trace_max_file_write(p_ue_context,X2AP_HANDOVER_PREP_FAILURE,encoded_msg_len, buffer, X2);

                uecc_x2ap_p_rrc_trace_for_cause(p_ue_context,msg.cause);
                P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(),X2,"\n </msg>");
	    	/*SPR 15896 Fix Stop*/
        }
/*BUG 604 changes start*/
    }
/*BUG 604 changes stop*/

        /* X2AP FIX_AG_2 : End*/

        /* Call      event handler *
         * and pass API data to FSM. */
        /* WARNING. The uecc_event_queue_push_event can't be used here because
         * the stack variable is used for storing the API data
         * */
        uecc_fsm_process_event(
            p_ue_context,
            UECC_EV_X2AP_HO_PREPARATION_FAIL,                
            &msg);

    } while (0);
 
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
               
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_handover_cancel_msg
 *
*   INPUTS        : OSCTXT*            p_asn1_ctx
*                   x2ap_X2AP_PDU       *p_x2ap_pdu
*                   uecc_gb_context_t *p_uecc_gb_context
*                   U8                  peer_enodeb_id
*                   U8                  *buffer
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function processes X2AP Handover Cancel message
 *
 *   RETURNS:     None
 *
 ******************************************************************************/
void uecc_x2ap_process_handover_cancel_msg
(
    OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    U8                  *buffer
)
{
	uecc_ue_context_t               *p_ue_context = PNULL;
	rrc_x2ap_handover_cancel_t      msg;
	EVENT_EXTERNAL_HEADER           *p_event_header = PNULL;
	U32                             encoded_msg_len = 0;
	/*SPR 15896 Fix Start*/
	/*Code Removed*/
	/*SPR 15896 Fix Stop*/
	/* SPR 16750 Fix Start */
	rrc_x2ap_error_indication_t     error_indication;
	uecc_ue_context_t               *p_second_ue_context = PNULL;
	/* SPR 16750 Fix Stop */

	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
	/* SPR 16750 Fix Start */
	memset_wrapper(&error_indication, 0, sizeof(rrc_x2ap_error_indication_t));
	/* SPR 16750 Fix Stop */

	do
	{
		if(PNULL ==
				p_x2ap_pdu->u.initiatingMessage->value.u.handoverCancel)
		{
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name, RRC_ERROR, "[X2AP_Handover_Cancel]\
					handoverCancel field is NULL");
			break;
		}

		/* Decode message */
		if( X2AP_FAILURE == x2ap_internal_decode_handover_cancel(
					p_asn1_ctx,
					p_x2ap_pdu->u.initiatingMessage->value.u.handoverCancel,
					&msg,
					peer_enodeb_id
					))
		{
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name, RRC_INFO, " Handover Cancel "
					"Decoding Failure");
			break;
		}

		/*SPR 5253 Start */
		/*BUG 604 changes start*/
        /*SPR_16618_START*/
        /*SPR_17792_start*/
        if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
        {
            p_ue_context = uecc_ue_ctx_get_by_x2ap_id_ho_cancel(p_uecc_gb_context,
                    peer_enodeb_id, msg.x2ap_Old_UE_X2AP_ID, OLD_X2AP_UE_ID);

        }
        else
        {
            p_ue_context = uecc_ue_ctx_get_by_x2_id_intra_cell_ho_cancel(
                    p_uecc_gb_context,msg.x2ap_Old_UE_X2AP_ID, OLD_X2AP_UE_ID);

        }
        /*SPR_17792_end*/

        /*SPR_16618_END*/
		/*BUG 604 changes stop*/
		/*SPR 5253 Stop */

		if (PNULL != p_ue_context )
		{
			if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
						p_uecc_gb_context,p_ue_context->cell_index))
			{
				RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
						facility_name, RRC_WARNING,"cell not active");
				RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
				break;
			}
		}
		/* X2AP FIX_AG_2 : Start*/

        if (PNULL == p_ue_context )
        {

            /*SPR_17792_start*/
            if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
            {
                /* Error. Can't find UE context */
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,
                        "[X2AP Handover Cancel] UE context not found");
                /* SPR 16750 Fix Start */
                /* Drop message */
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
                error_indication.old_enb_ue_x2ap_id = msg.x2ap_Old_UE_X2AP_ID;
                if (X2AP_HANDOVER_CANCEL_NEW_UE_X2AP_ID_PRESENT &
                        msg.bitmask)
                {
                    error_indication.bitmask |=
                        RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
                    error_indication.new_enb_ue_x2ap_id = msg.x2ap_New_UE_X2AP_ID;
                }
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                error_indication.cause.t = T_x2ap_Cause_radioNetwork;
                error_indication.cause.u.radioNetwork = x2ap_unknown_old_eNB_UE_X2AP_ID;
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;
                error_indication.criticality_diagnostics.m.procedureCodePresent = 1;
                error_indication.criticality_diagnostics.procedureCode =
                    ASN1V_x2ap_id_handoverCancel;
                uecc_x2ap_build_and_send_err_indication(
                        peer_enodeb_id,
                        &error_indication);

                if (X2AP_HANDOVER_CANCEL_NEW_UE_X2AP_ID_PRESENT &
                        msg.bitmask)
                {
                    /* Get the UE Context matched with the new_enb_ue_x2ap_id */
                    /* contained in the X2AP Handover Cancel Message */
                    p_ue_context = uecc_ue_ctx_get_by_x2ap_id_ho_cancel(p_uecc_gb_context,
                            peer_enodeb_id, msg.x2ap_New_UE_X2AP_ID, NEW_X2AP_UE_ID);
                    if ((PNULL != p_ue_context) && (RRC_NULL == p_ue_context->
                                x2ap_context.m.error_indication_ongoing))
                    {
                        p_ue_context->x2ap_context.m.error_indication_ongoing = 1;

                        uecc_fsm_process_event(
                                p_ue_context,
                                UECC_EV_X2AP_ERROR_INDICATION,
                                &error_indication);
                    }
                }
                /* SPR 16750 Fix Stop */
            }		
            /*SPR_17792_end*/
			break;
		}
		/* SPR 16750 Fix Start */
		else if ((X2AP_HANDOVER_CANCEL_NEW_UE_X2AP_ID_PRESENT &
					msg.bitmask) && 
				((!(p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present)) ||
				 (p_ue_context->x2ap_context.new_enb_ue_x2ap_id !=
				  msg.x2ap_New_UE_X2AP_ID)))
		{
            /*SPR_17792_start*/
            if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,
                        "[X2AP UE CTX REL] NEW ENB UE X2AP ID does not match");
                /* Drop message */
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
                error_indication.old_enb_ue_x2ap_id = msg.x2ap_Old_UE_X2AP_ID;
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
                error_indication.new_enb_ue_x2ap_id = msg.x2ap_New_UE_X2AP_ID;
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                error_indication.cause.t = T_x2ap_Cause_radioNetwork;
                error_indication.cause.u.radioNetwork = x2ap_unknown_pair_of_UE_X2AP_ID;
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;
                error_indication.criticality_diagnostics.m.procedureCodePresent = 1;
                error_indication.criticality_diagnostics.procedureCode =
                    ASN1V_x2ap_id_handoverCancel;
                uecc_x2ap_build_and_send_err_indication(
                        peer_enodeb_id,
                        &error_indication);

                if (RRC_NULL == p_ue_context->x2ap_context.m.error_indication_ongoing)
                {
                    p_ue_context->x2ap_context.m.error_indication_ongoing = 1;

                    uecc_fsm_process_event(
                            p_ue_context,
                            UECC_EV_X2AP_ERROR_INDICATION,
                            &error_indication);
                }

                /* Get the second UE Context matched with the new_enb_ue_x2ap_id */
                /* contained in the X2AP UE Context Release Message */
                p_second_ue_context = uecc_ue_ctx_get_by_x2ap_id_ho_cancel(p_uecc_gb_context,
                        peer_enodeb_id, msg.x2ap_New_UE_X2AP_ID, NEW_X2AP_UE_ID);
                if ((PNULL != p_second_ue_context) && (RRC_NULL == p_second_ue_context->
                            x2ap_context.m.error_indication_ongoing))
                {
                    p_second_ue_context->x2ap_context.m.error_indication_ongoing = 1;

                    uecc_fsm_process_event(
                            p_second_ue_context,
                            UECC_EV_X2AP_ERROR_INDICATION,
                            &error_indication);
                }
            }		
            /*SPR_17792_end*/

			break;
		}
		/* SPR 16750 Fix Stop */
		/*BUG 604 changes start*/
		else if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
			/*BUG 604 changes stop*/
		{
			encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);
			p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
					encoded_msg_len);
			if (PNULL != p_event_header)
			{
				memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
							encoded_msg_len));
				rrc_uecc_fill_protocol_event_params(p_ue_context,
						p_event_header,
						(sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len),
						LOCAL_X2_HANDOVER_CANCEL,
						MESSAGE_DIRECTION_RECEIVED);

				l3_memcpy_wrapper((void*)((U8*)p_event_header + 
							sizeof(EVENT_EXTERNAL_HEADER)),
						(const void*)buffer,
						encoded_msg_len);

				rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
			}
         	/*SPR 15896 Fix Start*/
            if (p_ue_context->m.traceActivated)
            {
                /*SPR 15875 Fix Start*/
                if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
                        (p_ue_context->p_ue_trace_activation_info != PNULL) &&
                        (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & X2))
                {
                    P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_MME, "\n <msg function=\"X2AP\" name=\"Handover Cancel\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
                            difftime_wrapper(p_ue_context->p_ue_trace_activation_info->rawtime));
                }
                if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
                        (p_ue_context->p_ue_cell_trace_activation_info != PNULL) &&
                        (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & X2))
                {
                    P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL, "\n <msg function=\"X2AP\" name=\"Handover Cancel\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
                            difftime_wrapper(p_ue_context->p_ue_cell_trace_activation_info->rawtime));
                }
                /*SPR 15875 Fix Stop*/
                P_RRC_TRACE(p_ue_context, set_all_trace_bitmask (),X2,"\n <initiator type=\"ENB\">%s</initiator>",
                        p_ue_context->p_gb_context->x2ap_ctx.
                        x2ap_link_info[p_ue_context->x2ap_context.peer_enodeb_id].enb_comm_info.ip_addr[0].ip_addr);

                rrc_trace_max_file_write(p_ue_context,X2AP_HANDOVER_CANCEL,encoded_msg_len, buffer, X2);

                uecc_x2ap_p_rrc_trace_for_cause(p_ue_context,msg.cause);
                P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask (),X2,"\n </msg>");
                /*SPR 15896 Fix Stop*/
            }
			/* spr_11591_fix_start */
		}
		/* spr_11591_fix_stop */

		/* X2AP FIX_AG_2 : End*/

		/* Call      event handler *
		 * and pass API data to FSM. */
		/* WARNING. The uecc_event_queue_push_event can't be used here because
		 * the stack variable is used for storing the API data
		 * */
		uecc_fsm_process_event(
				p_ue_context,
				UECC_EV_X2AP_HO_CANCEL,
				&msg);

	} while (0);
	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);

}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_x2ap_cause
 *
*   INPUTS        : uecc_gb_context_t *p_uecc_gb_context
*                   x2ap_Cause*         p_cause
*                   rrm_cause_t*        p_rrm_cause
*   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function builds x2ap_Cause
 *
 *   RETURNS:
 *       None
 *
 *******************************************************************************/
rrc_return_et uecc_x2ap_build_x2ap_cause(
        uecc_gb_context_t*  p_uecc_gb_context,
        x2ap_Cause*         p_cause,
        rrm_cause_t*        p_rrm_cause
        )
{
    rrc_return_et result = RRC_SUCCESS;


    RRC_ASSERT(PNULL != p_cause);
    RRC_ASSERT(PNULL != p_rrm_cause);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    p_cause->t = p_rrm_cause->type;
    p_cause->u.protocol = 0;

    switch (p_cause->t)
    {
        case T_x2ap_Cause_radioNetwork:
            p_cause->u.radioNetwork = p_rrm_cause->value;
            break;
        case T_x2ap_Cause_transport:
            p_cause->u.transport = p_rrm_cause->value;
            break;
        case T_x2ap_Cause_protocol:
            p_cause->u.protocol = p_rrm_cause->value;
            break;
        case T_x2ap_Cause_misc:
            p_cause->u.misc = p_rrm_cause->value;
            break;
        default:
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                    p_uecc_gb_context->facility_name, RRC_WARNING,
                    "[RRC_RRM_RECEIVED_MSG] Invalid cause type:%d",
                    p_cause->t);
            result = RRC_FAILURE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);

    return result;
}

/******************************************************************************
 *   FUNCTION NAME: 
 *              fill_x2ap_cause
 *   Input:  
 *          uecc_gb_context_t*  p_uecc_gb_context
 *          x2ap_Cause          *p_x2ap_cause
 *          rrm_cause_t         *p_rrm_cause
 *   Outputs :
 *       None
 *   DESCRIPTION:
 *       This function fill the x2ap_casue paramter using rrm_cause.
 *
 *   RETURNS: 
 *      RRC_SUCCESS/RRC_FAILURE 
 ******************************************************************************/
rrc_return_et fill_x2ap_cause(
        uecc_gb_context_t*  p_uecc_gb_context,
        x2ap_Cause          *p_x2ap_cause,
        rrm_cause_t         *p_rrm_cause
        )
{
    /* SPR_7888_fix */
    rrc_return_et result = RRC_SUCCESS;
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    switch(p_rrm_cause->type)
    {
        case T_x2ap_Cause_radioNetwork:
            p_x2ap_cause->t = p_rrm_cause->type;
            if (X2AP_CAUSE_RADIO_NETWORK_MAX_VALUE >= p_rrm_cause->value)
            {
                p_x2ap_cause->u.radioNetwork = p_rrm_cause->value;
            }
            else
            {
                result = RRC_FAILURE;
            }
            break;
        case T_x2ap_Cause_transport:
            p_x2ap_cause->t = p_rrm_cause->type;
            if (X2AP_CAUSE_TRANSPORT_MAX_VALUE >= p_rrm_cause->value)
            {
                p_x2ap_cause->u.transport = p_rrm_cause->value;
            }
            else
            {
                result = RRC_FAILURE;
            }
            break;
        case T_x2ap_Cause_protocol:
            p_x2ap_cause->t = p_rrm_cause->type;
            if (X2AP_CAUSE_PROTOCOL_MAX_VALUE >= p_rrm_cause->value)
            {
                p_x2ap_cause->u.protocol = p_rrm_cause->value;
            }
            else
            {
                result = RRC_FAILURE;
            }
            break;
        case T_x2ap_Cause_misc:
            p_x2ap_cause->t = p_rrm_cause->type;
            if (X2AP_CAUSE_MISC_MAX_VALUE >= p_rrm_cause->value)
            {
                p_x2ap_cause->u.misc = p_rrm_cause->value;
            }
            else
            {
                result = RRC_FAILURE;
            }
            break;
        default:
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                    p_uecc_gb_context->facility_name, RRC_ERROR,
                    "Wrong cause type appeared %d",p_x2ap_cause->t);
            result = RRC_FAILURE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
    /* SPR_7888_fix */
}

/******************************************************************************
 *   FUNCTION NAME: fill_rrm_cause_from_x2ap_cause
 *   Input:  
 *          uecc_gb_context_t*  p_uecc_gb_context
 *          rrm_cause_t         *p_rrm_cause
 *          x2ap_Cause          *p_x2ap_cause
 *   Outputs :
 *       None
 *
 *   DESCRIPTION:
 *       This function fill the s1ap_casue paramter using rrm_cause.
 *
 *   RETURNS: N/A
 ******************************************************************************/
void fill_rrm_cause_from_x2ap_cause(
        uecc_gb_context_t*  p_uecc_gb_context,
        rrm_cause_t         *p_rrm_cause,
        x2ap_Cause          *p_x2ap_cause
        )
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    p_rrm_cause->type = (U8)p_x2ap_cause->t;
    switch(p_x2ap_cause->t)
    {
        case T_x2ap_Cause_radioNetwork:
            p_rrm_cause->value = (U16)p_x2ap_cause->u.radioNetwork;
            break;
        case T_x2ap_Cause_transport:
            p_rrm_cause->value = (U16)p_x2ap_cause->u.transport;
            break;
        case T_x2ap_Cause_protocol:
            p_rrm_cause->value = (U16)p_x2ap_cause->u.protocol;
            break;
        case  T_x2ap_Cause_misc:
            p_rrm_cause->value = (U16)p_x2ap_cause->u.misc;
            break;
        default:
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name, RRC_WARNING,
                    "Wrong cause type appeared %d",p_x2ap_cause->t);
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_generate_plmn_identity_int
 *
 *   Input:  
 *         OSOCTET            *p_x2ap_plmn_identity
 *          plmn_identity_t    *p_plmn_identity
 *   Outputs : None
 *
 *   DESCRIPTION:
 *       This function converts PLMN identiy from internal format to
 *       X2AP format according to TS 36.413.
 *
 *   RETURNS:
 *       Number of octets in resulting buffer
 *
 ******************************************************************************/
static void uecc_x2ap_generate_plmn_identity_int(
        OSOCTET            *p_x2ap_plmn_identity,
        plmn_identity_t    *p_plmn_identity)
{
    /* TS 3GPP TS 36.413 V8.5.1 - 9.2.3.8  PLMN Identity
     * - digits 0 to 9, encoded 0000 to 1001,
     * - 1111 used as filler digit,
     * two digits per octet,
     * - bits 4 to 1 of octet n encoding digit 2n-1
     * - bits 8 to 5 of octet n encoding digit 2n
     *
     * -The Selected PLMN identity consists of 3 digits from MCC
     * followed by either
     * -a filler digit plus 2 digits from MNC (in case of 2 digit MNC) or
     * -3 digits from MNC (in case of a 3 digit MNC).
     * */

    /* MCC should be present always */
    RRC_ASSERT(PLMN_IDENTITY_MCC_PRESENCE_FLAG &
            p_plmn_identity->presence_bitmask);

    /* PLMN Identity 1st octet : Pack first 2 digits of MCC */
    p_x2ap_plmn_identity[0] =
        (OSOCTET)( (p_plmn_identity->mcc[0] & 0xF) |
                   ((p_plmn_identity->mcc[1] & 0xF) << 4));

    /* PLMN Identity 2nd octet: Pack 3rd digit of MCC */
    p_x2ap_plmn_identity[1] =
        (OSOCTET) (p_plmn_identity->mcc[2] & 0xF);

    if (2 == p_plmn_identity->mnc.count)
    {
        /* Add filler digit */
        p_x2ap_plmn_identity[1] |= 0xF0;

        /* PLMN Identity 3rd octet : 1st and 2nd digit of MNC */
        p_x2ap_plmn_identity[2] =
            (OSOCTET) ((p_plmn_identity->mnc.mnc[0] & 0xF) |
                       ((p_plmn_identity->mnc.mnc[1] & 0xF) << 4));
    }
    else
    {
        /* PLMN Identity 3rd octet : Pack 1st digit of MNC */
        p_x2ap_plmn_identity[1] =
            (OSOCTET)(p_x2ap_plmn_identity[1] |
                      ((p_plmn_identity->mnc.mnc[0] & 0xF) << 4));

        /* PLMN Identity 3rd octet : 2nd and 3rd digit of MNC */
        p_x2ap_plmn_identity[2] =
            (OSOCTET)  ((p_plmn_identity->mnc.mnc[1] & 0xF) |
                        ((p_plmn_identity->mnc.mnc[2] & 0xF) << 4));
    }
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_x2ap_E_RABInformationList
 *
 *   Input:  
 *          uecc_gb_context_t*  p_uecc_gb_context
 *          OSCTXT *p_asn1_ctx
 *          x2ap_E_RABs_ToBeSetup_List *p_erab_info_list
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds e_rab_to_be_setup_list_st 
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
static rrc_return_et uecc_x2ap_build_x2ap_E_RABInformationList(
        uecc_ue_context_t* p_ue_context,
        OSCTXT *p_asn1_ctx,
        x2ap_E_RABs_ToBeSetup_List *p_erab_info_list
        )
{
    OSRTDListNode* p_node = PNULL;
    x2ap_E_RABs_ToBeSetup_List_element *p_elem = PNULL;
    rrc_erab_list_t     *p_erab_list = PNULL;
    rrc_counter_t   erab_count = RRC_NULL, inner_count = RRC_NULL;
    /* CSR 00077460 Fix Start */
    U32 num_bytes = 0;
    U32 num_bits = 0;
    /* CSR 00077460 Fix Stop */


    RRC_ASSERT(PNULL!=p_ue_context);
    RRC_ASSERT(PNULL!=p_asn1_ctx);
    RRC_ASSERT(PNULL!=p_erab_info_list);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    asn1Init_x2ap_E_RABs_ToBeSetup_List (p_erab_info_list);
    p_erab_list = &(p_ue_context->ho_info.p_ho_info->p_src_ho_info->
            s1_src_ho_info.p_rrc_rrm_ho_required->source_to_target_container
            .eNBContainer.erab_dl_fwding_list);

    for (; erab_count < MAX_ERAB_COUNT; erab_count++)
    {
        if(PNULL == p_ue_context->p_e_rab_list[erab_count])
            continue;

         rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_E_RABs_ToBeSetup_List_element,
                &p_node,
                &p_elem);

        if (PNULL == p_node)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                    p_ue_context->p_gb_context->facility_name, 
                    RRC_FATAL,"Can't allocate p_node");
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return RRC_FAILURE;
        }
        asn1Init_x2ap_E_RABs_ToBeSetup_List_element(p_elem);

        p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1 = 
            rtxMemAllocType(p_asn1_ctx, x2ap_E_RABs_ToBeSetup_Item);

        if (PNULL == p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_FATAL,
                    " ASN malloc failed.");
            break;
        }
        asn1Init_x2ap_E_RABs_ToBeSetup_Item(p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1);
        p_elem->value.t = T6x2ap___x2ap_E_RABs_ToBeSetup_ItemIEs_1;

        p_elem->id = ASN1V_x2ap_id_E_RABs_ToBeSetup_Item;
        p_elem->criticality = x2ap_ignore;
    
        p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_ID 
           = p_ue_context->p_e_rab_list[erab_count]->e_RAB_ID;

        p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.qCI
          = p_ue_context->p_e_rab_list[erab_count]->e_RABlevelQoSParameters.qCI;

        p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.\
            allocationAndRetentionPriority.priorityLevel
            = p_ue_context->p_e_rab_list[erab_count]->e_RABlevelQoSParameters.\
            allocationRetentionPriority.priorityLevel;

        p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.\
            allocationAndRetentionPriority.pre_emptionCapability
            = p_ue_context->p_e_rab_list[erab_count]->e_RABlevelQoSParameters.\
            allocationRetentionPriority.pre_emptionCapability;

        p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.\
            allocationAndRetentionPriority.pre_emptionVulnerability
            = p_ue_context->p_e_rab_list[erab_count]->e_RABlevelQoSParameters.\
            allocationRetentionPriority.pre_emptionVulnerability;

        p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.\
            allocationAndRetentionPriority.m.iE_ExtensionsPresent = 0;

        p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.\
            m.gbrQosInformationPresent = 0;
        if (p_ue_context->p_e_rab_list[erab_count]->e_RABlevelQoSParameters.m.\
                gbrQosInformationPresent)
        {
            p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.\
                m.gbrQosInformationPresent = 1;

            p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.\
                gbrQosInformation.e_RAB_MaximumBitrateDL
                = p_ue_context->p_e_rab_list[erab_count]->\
               e_RABlevelQoSParameters.gbrQosInformation.e_RAB_MaximumBitrateDL;

            p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.\
                gbrQosInformation.e_RAB_MaximumBitrateUL
                = p_ue_context->p_e_rab_list[erab_count]->\
               e_RABlevelQoSParameters.gbrQosInformation.e_RAB_MaximumBitrateUL;

            p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.\
                gbrQosInformation.e_RAB_GuaranteedBitrateDL
                = p_ue_context->p_e_rab_list[erab_count]->\
                e_RABlevelQoSParameters.gbrQosInformation.\
                e_RAB_GuaranteedBitrateDL;

            p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.\
                gbrQosInformation.e_RAB_GuaranteedBitrateUL
                = p_ue_context->p_e_rab_list[erab_count]->e_RABlevelQoSParameters.\
                gbrQosInformation.e_RAB_GuaranteedBitrateUL;

            p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.\
                gbrQosInformation.m.iE_ExtensionsPresent = 0;
        }      

        if ((p_ue_context->ho_info.p_ho_info->p_src_ho_info->s1_src_ho_info.
              p_rrc_rrm_ho_required->source_to_target_container.eNBContainer.
              bitmask &
              SOURCE_ENB_TO_TARGET_ENB_TRANSPARENT_CONTAINER_ERAB_DL_FWDING_LIST_PRESENT)
              && (PNULL != p_erab_list))
        {
            p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->m.dL_ForwardingPresent = 0;
            for(inner_count = 0; inner_count < p_erab_list->erab_count;
                    inner_count++)
            {
                if (p_ue_context->p_e_rab_list[erab_count]->e_RAB_ID ==
                        p_erab_list->erab_id[inner_count])
                {
                    p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->m.dL_ForwardingPresent = 1;
                    p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->dL_Forwarding = x2ap_dL_forwardingProposed;
                }
            }
        }
        /* CSR 00077460 Fix Start */
        /* transportLayerAddress */
        num_bits = p_ue_context->p_e_rab_list[erab_count]->transportLayerAddress.numbits;

        if (num_bits % 8)
        {
            num_bytes = (num_bits/8) + 1;
        }
        else
        {
            num_bytes = num_bits/8;
        }

        p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->uL_GTPtunnelEndpoint.transportLayerAddress.data =
            (OSOCTET*)rtxMemAlloc(p_asn1_ctx, num_bytes);

        if (PNULL == 
                p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->uL_GTPtunnelEndpoint.transportLayerAddress.data)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, 
                    RRC_ERROR, "ASN malloc failed.");
            return RRC_FAILURE;
        }
        l3_memcpy_wrapper(((void*)p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->uL_GTPtunnelEndpoint.\
               transportLayerAddress.data),
              (p_ue_context->p_e_rab_list[erab_count]->transportLayerAddress.data),
              num_bytes);

        p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->uL_GTPtunnelEndpoint.transportLayerAddress.numbits =
            (OSUINT32)(num_bits);
        /* CSR 00077460 Fix Stop */

        RRC_ASSERT(sizeof(p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->uL_GTPtunnelEndpoint.gTP_TEID.data) ==
                sizeof(p_ue_context->p_e_rab_list[erab_count]->gTP_TEID.data));
        p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->uL_GTPtunnelEndpoint.\
            gTP_TEID.numocts 
            = sizeof(p_ue_context->p_e_rab_list[erab_count]->gTP_TEID.data );

        l3_memcpy_wrapper(p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->uL_GTPtunnelEndpoint.gTP_TEID.data,
                   p_ue_context->p_e_rab_list[erab_count]->gTP_TEID.data,
                   p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->uL_GTPtunnelEndpoint.gTP_TEID.numocts);

        p_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->m.iE_ExtensionsPresent=0;

        rtxDListAppendNode(p_erab_info_list, p_node);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_x2ap_RRCContext
 *
 *   Input:  
 *          uecc_gb_context_t*  p_uecc_gb_context
 *          OSCTXT              *p_asn1_ctx
 *          x2ap_UE_ContextInformation *p_x2ap_ue_ctx_info
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds RRC Context (HO Preparation Information)  
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
static rrc_return_et uecc_x2ap_build_x2ap_RRCContext(
        uecc_ue_context_t* p_ue_context,
        OSCTXT *p_asn1_ctx,
        x2ap_UE_ContextInformation *p_x2ap_ue_ctx_info
        )
{
    rrc_return_et                       result = RRC_FAILURE;
    HandoverPreparationInformation      handover_prep_info;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    memset_wrapper(&handover_prep_info, 0, sizeof(handover_prep_info));
    

     do
    {
        handover_prep_info.criticalExtensions.t
            = T_HandoverPreparationInformation_criticalExtensions_c1;

        handover_prep_info.criticalExtensions.u.c1 = rtxMemAllocType(p_asn1_ctx,
                HandoverPreparationInformation_criticalExtensions_c1);

        if(PNULL == handover_prep_info.criticalExtensions.u.c1)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_FATAL,
                    "[uecc_x2ap_build_rrc_context]"
                    "Fail to allocate memory]");
            break;
        }
        asn1Init_HandoverPreparationInformation_criticalExtensions_c1 (
                handover_prep_info.criticalExtensions.u.c1);

        handover_prep_info.criticalExtensions.u.c1->t =
            T_HandoverPreparationInformation_criticalExtensions_c1_handoverPreparationInformation_r8;

        handover_prep_info.criticalExtensions.u.c1->u.
            handoverPreparationInformation_r8=
            rtxMemAllocType(p_asn1_ctx,
                    HandoverPreparationInformation_r8_IEs);

        if( PNULL == handover_prep_info.criticalExtensions.u.c1->u.
                handoverPreparationInformation_r8 )
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_FATAL,
                    "[uecc_x2ap_build_rrc_context]"
                    "Fail to allocate memory]");
            break;
        }

        memset_wrapper(handover_prep_info.criticalExtensions.u.c1->u.
                handoverPreparationInformation_r8,0,sizeof(
                    HandoverPreparationInformation_r8_IEs));

        asn1Init_HandoverPreparationInformation_r8_IEs (
                handover_prep_info.criticalExtensions.u.c1->u.
                handoverPreparationInformation_r8);

         if (RRC_SUCCESS !=
                uecc_s1ap_build_HandoverPreparationInformation(
                    p_ue_context,
                    p_asn1_ctx,
                    handover_prep_info.criticalExtensions.u.c1->u.
                    handoverPreparationInformation_r8))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[uecc_x2ap_build_rrc_context]"
                    "Fail to fill Handover Information");
            break;
        }

        /* Encode RRC Context */
        if(RRC_SUCCESS != x2ap_internal_encode_rrc_context(p_asn1_ctx,
                    &(p_x2ap_ue_ctx_info->rRC_Context),
                    &handover_prep_info))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[uecc_x2ap_build_rrc_context]"
                    "RRC Context Encoding Failure");
            break;
        }

      result = RRC_SUCCESS;
    }while(0);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_x2ap_HO_RestrictionList 
 *
 *   Input:  
 *          uecc_ue_context_t*  p_ue_context
 *          OSCTXT              *p_asn1_ctx
 *          x2ap_UE_ContextInformation *p_x2ap_ue_ctx_info
 *   Outputs : None
 *   DESCRIPTION:
 *       This function build the handover restriction list which will further 
 *       added to HO_REQUEST message going to target eNB.
 *
 *   RETURNS: RRC_SUCCESS or RRC_FAILURE
 *
 *******************************************************************************/
static rrc_return_et uecc_x2ap_build_x2ap_HO_RestrictionList
(
 uecc_ue_context_t               *p_ue_context,
 OSCTXT                          *p_asn1_ctxt,
 x2ap_UE_ContextInformation      *p_x2ap_ue_ctx_info
 )
{
    rrc_return_et           result = RRC_SUCCESS;
    U8             count = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctxt);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* filling serving plmn (M) */
    asn1Init_x2ap_PLMN_Identity(&p_x2ap_ue_ctx_info->handoverRestrictionList.\
            servingPLMN);
    p_x2ap_ue_ctx_info->handoverRestrictionList.servingPLMN.numocts
        =   p_ue_context->ics_handover_restriction_list.servingPLMN.numocts;

    l3_memcpy_wrapper((void *)p_x2ap_ue_ctx_info->handoverRestrictionList.servingPLMN.\
                       data,
            (const void *)p_ue_context->ics_handover_restriction_list.\
                       servingPLMN.data,
            p_ue_context->ics_handover_restriction_list.servingPLMN.numocts);

    /* filling equivalent plmn (O) */
    asn1Init_x2ap_EPLMNs(&p_x2ap_ue_ctx_info->handoverRestrictionList.\
                            equivalentPLMNs);
    p_x2ap_ue_ctx_info->handoverRestrictionList.m.equivalentPLMNsPresent = 0;
    if (p_ue_context->ics_handover_restriction_list.m.equivalentPLMNsPresent)
    {
        p_x2ap_ue_ctx_info->handoverRestrictionList.m.equivalentPLMNsPresent =1;
        p_x2ap_ue_ctx_info->handoverRestrictionList.equivalentPLMNs.n
            =   p_ue_context->ics_handover_restriction_list.equivalentPLMNs.n;

        for (; count<(p_ue_context->ics_handover_restriction_list.\
                        equivalentPLMNs.n); count++ )
        {
            p_x2ap_ue_ctx_info->handoverRestrictionList.equivalentPLMNs.\
                    elem[count].numocts
                = p_ue_context->ics_handover_restriction_list.equivalentPLMNs.\
                    elem[count].numocts;

            l3_memcpy_wrapper((void *)p_x2ap_ue_ctx_info->handoverRestrictionList.\
                    equivalentPLMNs.elem[count].data,
                    (const void *)p_ue_context->ics_handover_restriction_list.\
                    equivalentPLMNs.elem[count].data,
                    p_ue_context->ics_handover_restriction_list.\
                        equivalentPLMNs.elem[count].numocts);
        }
    }

    /* filling forbidden TAs (O) */
    p_x2ap_ue_ctx_info->handoverRestrictionList.m.forbiddenTAsPresent = 0;
    if (p_ue_context->ics_handover_restriction_list.m.forbiddenTAsPresent)
    {
        p_x2ap_ue_ctx_info->handoverRestrictionList.m.forbiddenTAsPresent = 1;

        asn1Copy_x2ap_ForbiddenTAs(p_asn1_ctxt,  
                &(p_ue_context->ics_handover_restriction_list.forbiddenTAs),
                &(p_x2ap_ue_ctx_info->handoverRestrictionList.forbiddenTAs));
    }

    /* filling forbidden LAs (O) */
    p_x2ap_ue_ctx_info->handoverRestrictionList.m.forbiddenLAsPresent = 0;
    if (p_ue_context->ics_handover_restriction_list.m.forbiddenLAsPresent)
    {
        p_x2ap_ue_ctx_info->handoverRestrictionList.m.forbiddenLAsPresent = 1;
        asn1Copy_x2ap_ForbiddenTAs(p_asn1_ctxt,  
                &(p_ue_context->ics_handover_restriction_list.forbiddenLAs),
                &(p_x2ap_ue_ctx_info->handoverRestrictionList.forbiddenLAs));
    }

    /* filling forbidden Inter RATs (O) */
    p_x2ap_ue_ctx_info->handoverRestrictionList.m.forbiddenInterRATsPresent = 0;
    if (p_ue_context->ics_handover_restriction_list.m.forbiddenInterRATsPresent)
    {

        p_x2ap_ue_ctx_info->handoverRestrictionList.m.\
                forbiddenInterRATsPresent = 1;
        p_x2ap_ue_ctx_info->handoverRestrictionList.forbiddenInterRATs
            =   p_ue_context->ics_handover_restriction_list.forbiddenInterRATs;

    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_ue_context_information 
 *
 *   Input:  
 *          uecc_ue_context_t*  p_ue_context
 *          OSCTXT              *p_asn1_ctx
 *          rrc_x2ap_handover_request_t     *p_x2ap_ho_req_msg
 *           rrc_rrm_ho_required_t           *p_rrc_rrm_ho_required
 *   Outputs : None
 *   DESCRIPTION:
 *       This function build and encode the ue context information container 
 *       which will further added to HO_REQUEST message going to target eNB.
 *
 *   RETURNS: RRC_SUCCESS or RRC_FAILURE
 *
 *******************************************************************************/
static rrc_return_et uecc_x2ap_build_ue_context_information
(
 uecc_ue_context_t               *p_ue_context,
 OSCTXT                          *p_asn1_ctxt,
 rrc_x2ap_handover_request_t     *p_x2ap_ho_req_msg,
 rrc_rrm_ho_required_t           *p_rrc_rrm_ho_required
 )
{
    rrc_return_et           result = RRC_FAILURE;
    x2ap_UE_ContextInformation   *p_ue_context_info = PNULL;
    U8 peer_enodeb_id = 0;
    U8 serving_cell_index = 0;
    utran_EARFCN earfcn = 0;
    U32 pci = 0;
    U8 index = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctxt);
    RRC_ASSERT(PNULL != p_rrc_rrm_ho_required);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_ue_context_info =
        &(p_x2ap_ho_req_msg->rrc_x2ap_ue_ctx_info.ue_context_info);

    do
    {
        p_ue_context_info->mME_UE_S1AP_ID = p_ue_context->mme_ue_id;
	    if(p_ue_context->m.mme_ue_id2_present == 1 && p_ue_context->p_gb_context->enbType == HOME_ENB)
        {
	        p_ue_context_info->mME_UE_S1AP_ID = p_ue_context->mme_ue_id2;
        }

        /* UE Security Capabilities (M) */
        if ( (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) && 
               (p_ue_context->ue_ctx_mod_security_flag) && 
                 (PNULL != p_ue_context->p_curr_proc_data) &&
                   (p_ue_context->p_curr_proc_data->m.SecurityCapability_present) )
        {
            asn1Init_x2ap_UESecurityCapabilities(&p_ue_context_info->
                uESecurityCapabilities);

            p_ue_context_info->uESecurityCapabilities.encryptionAlgorithms.numbits    
              = p_ue_context->p_curr_proc_data->ue_security_capabilities.encryptionAlgorithms.
                 numbits;

            p_ue_context_info->uESecurityCapabilities.encryptionAlgorithms.data
              = (OSOCTET*)rtxMemAlloc(p_asn1_ctxt,
                    p_ue_context->p_curr_proc_data->ue_security_capabilities.encryptionAlgorithms.
                    numbits/8);

            if (PNULL == 
                p_ue_context_info->uESecurityCapabilities.encryptionAlgorithms.
                data)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                    p_ue_context->p_gb_context->facility_name, 
                    RRC_ERROR, "ASN malloc failed.");
                return RRC_FAILURE;
            }

            l3_memcpy_wrapper(((void*)p_ue_context_info->uESecurityCapabilities.
                                encryptionAlgorithms.data),
                ((const void *)p_ue_context->p_curr_proc_data->ue_security_capabilities.
                                encryptionAlgorithms.data),
                p_ue_context->p_curr_proc_data->ue_security_capabilities.encryptionAlgorithms.
                                numbits/8);

            p_ue_context_info->uESecurityCapabilities.integrityProtectionAlgorithms.
                    numbits    
               = p_ue_context->p_curr_proc_data->ue_security_capabilities.
                    integrityProtectionAlgorithms.numbits;

            p_ue_context_info->uESecurityCapabilities.integrityProtectionAlgorithms.
              data
               = (OSOCTET*)rtxMemAlloc(p_asn1_ctxt,
                    p_ue_context->p_curr_proc_data->ue_security_capabilities.
                        integrityProtectionAlgorithms.numbits/8);

            if (PNULL == p_ue_context_info->uESecurityCapabilities.
                    integrityProtectionAlgorithms.data)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
                return RRC_FAILURE;
            }

            l3_memcpy_wrapper(((void*)p_ue_context_info->uESecurityCapabilities.
                               integrityProtectionAlgorithms.data),
                ((const void *)p_ue_context->p_curr_proc_data->ue_security_capabilities.
                               integrityProtectionAlgorithms.data),
                p_ue_context->p_curr_proc_data->ue_security_capabilities.encryptionAlgorithms.
                               numbits/8);
        }
        else 
        {
        asn1Init_x2ap_UESecurityCapabilities(&p_ue_context_info->
                uESecurityCapabilities);

        p_ue_context_info->uESecurityCapabilities.encryptionAlgorithms.numbits    
            = p_ue_context->ue_security_capabilities.encryptionAlgorithms.
            numbits;

        p_ue_context_info->uESecurityCapabilities.encryptionAlgorithms.data
            = (OSOCTET*)rtxMemAlloc(p_asn1_ctxt,
                    p_ue_context->ue_security_capabilities.encryptionAlgorithms.
                    numbits/8);

        if (PNULL == 
                p_ue_context_info->uESecurityCapabilities.encryptionAlgorithms.
                data)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                    p_ue_context->p_gb_context->facility_name, 
                    RRC_ERROR, "ASN malloc failed.");
            return RRC_FAILURE;
        }

        l3_memcpy_wrapper(((void*)p_ue_context_info->uESecurityCapabilities.
                                encryptionAlgorithms.data),
                ((const void *)p_ue_context->ue_security_capabilities.
                                encryptionAlgorithms.data),
                p_ue_context->ue_security_capabilities.encryptionAlgorithms.
                                numbits/8);

        p_ue_context_info->uESecurityCapabilities.integrityProtectionAlgorithms.
                    numbits    
            = p_ue_context->ue_security_capabilities.
                    integrityProtectionAlgorithms.numbits;

       p_ue_context_info->uESecurityCapabilities.integrityProtectionAlgorithms.
           data
            = (OSOCTET*)rtxMemAlloc(p_asn1_ctxt,
                    p_ue_context->ue_security_capabilities.
                        integrityProtectionAlgorithms.numbits/8);

        if (PNULL == p_ue_context_info->uESecurityCapabilities.
                    integrityProtectionAlgorithms.data)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_ERROR, "ASN malloc failed.");
            return RRC_FAILURE;
        }

        l3_memcpy_wrapper(((void*)p_ue_context_info->uESecurityCapabilities.
                               integrityProtectionAlgorithms.data),
                ((const void *)p_ue_context->ue_security_capabilities.
                               integrityProtectionAlgorithms.data),
                p_ue_context->ue_security_capabilities.encryptionAlgorithms.
                               numbits/8);
        }
        /*BUG 604 changes stop*/
            
        if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
            /* Get peer_enodeb and peer serving cell index */
            peer_enodeb_id = p_ue_context->x2ap_context.peer_enodeb_id;
            serving_cell_index = p_ue_context->x2ap_context.serving_cell_index;

            /* Get pci value for peer serving cell */
            pci = P_SRVD_CELL_INFO(p_ue_context->p_gb_context->x2ap_ctx.
                    x2ap_link_info[peer_enodeb_id].p_srvd_cell_info,
                    serving_cell_index)->pci;
            /* Bug 4691 Changes Start */
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_INFO,
                    "pci value used to generate Key is %u ",pci);
            /* Bug 4691 Changes End */
            /* 
             *  Depending upon the eutran_mode info received for serving cell of peer
             *  eNodeB, get earfcn value 
             */
            if(P_SRVD_CELL_INFO(p_ue_context->p_gb_context->x2ap_ctx.x2ap_link_info[peer_enodeb_id].
                        p_srvd_cell_info, serving_cell_index)->choice_eutra_mode. 
                    choice_eutra_mode.bitmask & X2AP_EUTRA_MODE_FDD)
            {

                earfcn = (utran_EARFCN)P_SRVD_CELL_INFO(p_ue_context->p_gb_context->x2ap_ctx.
                        x2ap_link_info[peer_enodeb_id].p_srvd_cell_info,
                        serving_cell_index)->choice_eutra_mode.choice_eutra_mode.fdd_info.
                    dl_earfcn.earfcn; 


            }
            else if(P_SRVD_CELL_INFO(p_ue_context->p_gb_context->x2ap_ctx.
                        x2ap_link_info[peer_enodeb_id].p_srvd_cell_info,
                        serving_cell_index)->choice_eutra_mode.choice_eutra_mode.bitmask
                    & X2AP_EUTRA_MODE_TDD)
            {
                earfcn = (utran_EARFCN)P_SRVD_CELL_INFO(p_ue_context->p_gb_context->x2ap_ctx.
                        x2ap_link_info[peer_enodeb_id].p_srvd_cell_info,
                        serving_cell_index)->choice_eutra_mode.choice_eutra_mode.tdd_info.
                    earfcn.earfcn; 

            }
            else
            {
                /* eutran mode presence bitmask is not set */
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "%s: neither FDD or TDD"
                        " Presence bitmask not set",__FUNCTION__);
                result = RRC_FAILURE;
                break;
            }
        }
        /*BUG 604 changes start*/
        if((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
        /*BUG 604 changes stop*/
        {
            if ( PNULL != p_ue_context->p_gb_context->p_p_csc_context )
            {
                for ( index = 0; index < MAX_NUM_CELLS; index++ )
                {
                    if ( PNULL != p_ue_context->p_gb_context->p_p_csc_context[index] )
                    {
                        if ( PNULL != p_ue_context->p_gb_context->p_p_csc_context[index]->
                                p_csc_init_setup_ind)
                        {
                            if( RRM_TARGET_ID_ENB_ID_PRESENT & 
                                    p_rrc_rrm_ho_required->target_id.bitmask)
                            {
                                if (RRC_NULL == memcmp_wrapper (p_ue_context->p_gb_context->
                                            p_p_csc_context[index]->p_csc_init_setup_ind->
                                            sib_type_1_Info.
                                            cell_access_related_info.cell_Id,
                                            p_rrc_rrm_ho_required->target_id.enb_id.
                                            global_enb_id.cell_identity,
                                            CELL_ID_OCTET_SIZE ))
                                {
                                   /*BUG 604 changes start*/
                                    if(INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
                                    {
                                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                                                p_ue_context->p_gb_context->facility_name, RRC_INFO,
                                                "[INTER_CELL_HO] "
                                                "cell identity is matched at [%u] cell index", index);
                                    }
                                    else
                                    {
                                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                                                p_ue_context->p_gb_context->facility_name, RRC_INFO,
                                                "[INTRA_CELL_HO] "
                                                "cell identity is matched at [%u] cell index", index);
                                    }
                                     /*BUG 604 changes stop*/
                                    cell_index = index;
                                    p_ue_context->p_gb_context->inter_cell_ho_cell_index = index;
                                    break;
                                }
                            }
                        }
                    }
                }

                if ( RRC_INVALID_CELL_INDEX == cell_index )
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,
                            RRC_ERROR, "[INTER_CELL_HO]"
                            "Cell Index not available");
                    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }
                else
                {
                    pci = (U32)p_ue_context->p_gb_context->p_p_csc_context[
                        cell_index]->p_csc_init_setup_ind->phys_cell_id;
                    earfcn = p_ue_context->p_gb_context->p_p_csc_context[
                        cell_index]->p_csc_init_setup_ind->eutra_dl_carrier_freq;
                }

            }
        }
        /* AS Security Information(M) */
        p_ue_context_info->aS_SecurityInformation.key_eNodeB_star.numbits = 256;

        /*BUG 604 changes start*/
        /* UE Security Capabilities (M) */
        if ( (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) && 
               (p_ue_context->ue_ctx_mod_security_flag) && 
                 (PNULL != p_ue_context->p_curr_proc_data) &&
                   (p_ue_context->p_curr_proc_data->m.SecurityKey_present) )
        {
            p_ue_context_info->aS_SecurityInformation.nextHopChainingCount = 0;

            /* Generate Security key */
            rrc_generate_kenb_star(
                     (U16)pci,
                     (U16)earfcn,
                     &p_ue_context_info->aS_SecurityInformation.
                     key_eNodeB_star.data[0],
                     &p_ue_context->p_curr_proc_data->security_key.data[0]);
            /*SPR_17143_START*/
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                    p_ue_context->p_gb_context->facility_name, RRC_INFO,
                    "[UE:%u] KeNB* security_key - "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]",
                    p_ue_context->ue_index, 
                    p_ue_context_info->aS_SecurityInformation.
                    key_eNodeB_star.data[0],
                    p_ue_context_info->aS_SecurityInformation.
                    key_eNodeB_star.data[1],
                    p_ue_context_info->aS_SecurityInformation.
                    key_eNodeB_star.data[2],
                    p_ue_context_info->aS_SecurityInformation.
                    key_eNodeB_star.data[3],
                    p_ue_context_info->aS_SecurityInformation.
                    key_eNodeB_star.data[4],
                    p_ue_context_info->aS_SecurityInformation.
                    key_eNodeB_star.data[5],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[6],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[7],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[8],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[9],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[10],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[11],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[12],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[13],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[14],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[15],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[16],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[17],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[18],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[19],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[20],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[21],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[22],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[23],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[24],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[25],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[26],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[27],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[28],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[29],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[30],
                    p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[31]);

            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                    p_ue_context->p_gb_context->facility_name, RRC_INFO,
                    "KeNB* Curr Proc Security Key Data  - "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
                    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]",
                    p_ue_context->p_curr_proc_data->security_key.data[0],
                    p_ue_context->p_curr_proc_data->security_key.data[1],
                    p_ue_context->p_curr_proc_data->security_key.data[2],
                    p_ue_context->p_curr_proc_data->security_key.data[3],
                    p_ue_context->p_curr_proc_data->security_key.data[4],
                    p_ue_context->p_curr_proc_data->security_key.data[5],
                    p_ue_context->p_curr_proc_data->security_key.data[6],
                    p_ue_context->p_curr_proc_data->security_key.data[7],
                    p_ue_context->p_curr_proc_data->security_key.data[8],
                    p_ue_context->p_curr_proc_data->security_key.data[9],
                    p_ue_context->p_curr_proc_data->security_key.data[10],
                    p_ue_context->p_curr_proc_data->security_key.data[11],
                    p_ue_context->p_curr_proc_data->security_key.data[12],
                    p_ue_context->p_curr_proc_data->security_key.data[13],
                    p_ue_context->p_curr_proc_data->security_key.data[14],
                    p_ue_context->p_curr_proc_data->security_key.data[15],
                    p_ue_context->p_curr_proc_data->security_key.data[16],
                    p_ue_context->p_curr_proc_data->security_key.data[17],
                    p_ue_context->p_curr_proc_data->security_key.data[18],
                    p_ue_context->p_curr_proc_data->security_key.data[19],
                    p_ue_context->p_curr_proc_data->security_key.data[20],
                    p_ue_context->p_curr_proc_data->security_key.data[21],
                    p_ue_context->p_curr_proc_data->security_key.data[22],
                    p_ue_context->p_curr_proc_data->security_key.data[23],
                    p_ue_context->p_curr_proc_data->security_key.data[24],
                    p_ue_context->p_curr_proc_data->security_key.data[25],
                    p_ue_context->p_curr_proc_data->security_key.data[26],
                    p_ue_context->p_curr_proc_data->security_key.data[27],
                    p_ue_context->p_curr_proc_data->security_key.data[28],
                    p_ue_context->p_curr_proc_data->security_key.data[29],
                    p_ue_context->p_curr_proc_data->security_key.data[30],
                    p_ue_context->p_curr_proc_data->security_key.data[31]);
            /*SPR_17143_END*/
        }
        else
        {
        if(RRC_TRUE == p_ue_context->nh_ncc_pair.used_flag)
        {
            /* Generate KeNB* from KeNB using Horizontal key 
               derivation algorithm */
            /* Copy the value of NCC as stored in UE context */
            p_ue_context_info->aS_SecurityInformation.nextHopChainingCount 
                = p_ue_context->ncc;

            if(!p_ue_context->ue_ctx_mod_security_flag)
            {
                /* Generate Security key */
                rrc_generate_kenb_star(
                        (U16)pci,
                        (U16)earfcn,
                        &p_ue_context_info->aS_SecurityInformation.
                        key_eNodeB_star.data[0],
                        &p_ue_context->security_key.data[0]
			);
		/*SPR_17143_START*/
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
				p_ue_context->p_gb_context->facility_name, RRC_INFO,
				"[UE:%u] KeNB* security_key - "
				"[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
				"[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
				"[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
				"[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]",
				p_ue_context->ue_index, 
				p_ue_context_info->aS_SecurityInformation.
				key_eNodeB_star.data[0],
				p_ue_context_info->aS_SecurityInformation.
				key_eNodeB_star.data[1],
				p_ue_context_info->aS_SecurityInformation.
				key_eNodeB_star.data[2],
				p_ue_context_info->aS_SecurityInformation.
				key_eNodeB_star.data[3],
				p_ue_context_info->aS_SecurityInformation.
				key_eNodeB_star.data[4],
				p_ue_context_info->aS_SecurityInformation.
				key_eNodeB_star.data[5],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[6],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[7],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[8],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[9],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[10],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[11],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[12],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[13],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[14],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[15],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[16],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[17],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[18],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[19],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[20],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[21],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[22],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[23],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[24],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[25],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[26],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[27],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[28],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[29],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[30],
				p_ue_context_info->aS_SecurityInformation.
					key_eNodeB_star.data[31]);

		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
				p_ue_context->p_gb_context->facility_name, RRC_INFO,
				"KeNB* UE ctxt Security Key Data  - "
				"[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
				"[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
				"[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
				"[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]",
				p_ue_context->security_key.data[0],
				p_ue_context->security_key.data[1],
				p_ue_context->security_key.data[2],
				p_ue_context->security_key.data[3],
				p_ue_context->security_key.data[4],
				p_ue_context->security_key.data[5],
				p_ue_context->security_key.data[6],
				p_ue_context->security_key.data[7],
				p_ue_context->security_key.data[8],
				p_ue_context->security_key.data[9],
				p_ue_context->security_key.data[10],
				p_ue_context->security_key.data[11],
				p_ue_context->security_key.data[12],
				p_ue_context->security_key.data[13],
				p_ue_context->security_key.data[14],
				p_ue_context->security_key.data[15],
				p_ue_context->security_key.data[16],
				p_ue_context->security_key.data[17],
				p_ue_context->security_key.data[18],
				p_ue_context->security_key.data[19],
				p_ue_context->security_key.data[20],
				p_ue_context->security_key.data[21],
				p_ue_context->security_key.data[22],
				p_ue_context->security_key.data[23],
				p_ue_context->security_key.data[24],
				p_ue_context->security_key.data[25],
				p_ue_context->security_key.data[26],
				p_ue_context->security_key.data[27],
				p_ue_context->security_key.data[28],
				p_ue_context->security_key.data[29],
				p_ue_context->security_key.data[30],
				p_ue_context->security_key.data[31]);
		/*SPR_17143_END*/
	    }
	    else
            {
                if((PNULL != p_ue_context->p_curr_proc_data)&&
                        (p_ue_context->p_curr_proc_data->m.SecurityKey_present))
                {
                    /* Generate Security key */
                    rrc_generate_kenb_star(
                            (U16)pci,
                            (U16)earfcn,
                            &p_ue_context_info->aS_SecurityInformation.
                            key_eNodeB_star.data[0],
                            &p_ue_context->p_curr_proc_data->security_key.data[0]
                            );
                }
            }
/*BUG 604 changes stop*/
        }
        else
        {
            /* Generate KeNB* from NH/NCC pair using Vertical key 
               derivation algorithm */
            /* Copy the unused value of NCC as stored in UE context in nh/ncc pair */
            p_ue_context_info->aS_SecurityInformation.nextHopChainingCount 
                = p_ue_context->nh_ncc_pair.ncc;

            /* Generate Security key */
            rrc_generate_kenb_star(
                    (U16)pci,
                    (U16)earfcn,
                    &p_ue_context_info->aS_SecurityInformation.
                    key_eNodeB_star.data[0],
                    &(p_ue_context->nh_ncc_pair.nh.data[0])
                    );
	    /*SPR_17143_START*/
	    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
			    p_ue_context->p_gb_context->facility_name, RRC_INFO,
			    "[UE:%u] KeNB* security_key - "
			    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
			    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
			    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
			    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]",
			    p_ue_context->ue_index, 
			    p_ue_context_info->aS_SecurityInformation.
			    key_eNodeB_star.data[0],
			    p_ue_context_info->aS_SecurityInformation.
			    key_eNodeB_star.data[1],
			    p_ue_context_info->aS_SecurityInformation.
			    key_eNodeB_star.data[2],
			    p_ue_context_info->aS_SecurityInformation.
			    key_eNodeB_star.data[3],
			    p_ue_context_info->aS_SecurityInformation.
			    key_eNodeB_star.data[4],
			    p_ue_context_info->aS_SecurityInformation.
			    key_eNodeB_star.data[5],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[6],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[7],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[8],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[9],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[10],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[11],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[12],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[13],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[14],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[15],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[16],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[17],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[18],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[19],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[20],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[21],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[22],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[23],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[24],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[25],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[26],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[27],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[28],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[29],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[30],
			    p_ue_context_info->aS_SecurityInformation.
				    key_eNodeB_star.data[31]);

	    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
			    p_ue_context->p_gb_context->facility_name, RRC_INFO,
			    "KeNB* nh ncc pair -> nh Data  - "
			    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
			    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
			    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] "
			    "[%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X] [%.2X]",
			    p_ue_context->nh_ncc_pair.nh.data[0],
			    p_ue_context->nh_ncc_pair.nh.data[1],
			    p_ue_context->nh_ncc_pair.nh.data[2],
			    p_ue_context->nh_ncc_pair.nh.data[3],
			    p_ue_context->nh_ncc_pair.nh.data[4],
			    p_ue_context->nh_ncc_pair.nh.data[5],
			    p_ue_context->nh_ncc_pair.nh.data[6],
			    p_ue_context->nh_ncc_pair.nh.data[7],
			    p_ue_context->nh_ncc_pair.nh.data[8],
			    p_ue_context->nh_ncc_pair.nh.data[9],
			    p_ue_context->nh_ncc_pair.nh.data[10],
			    p_ue_context->nh_ncc_pair.nh.data[11],
			    p_ue_context->nh_ncc_pair.nh.data[12],
			    p_ue_context->nh_ncc_pair.nh.data[13],
			    p_ue_context->nh_ncc_pair.nh.data[14],
			    p_ue_context->nh_ncc_pair.nh.data[15],
			    p_ue_context->nh_ncc_pair.nh.data[16],
			    p_ue_context->nh_ncc_pair.nh.data[17],
			    p_ue_context->nh_ncc_pair.nh.data[18],
			    p_ue_context->nh_ncc_pair.nh.data[19],
			    p_ue_context->nh_ncc_pair.nh.data[20],
			    p_ue_context->nh_ncc_pair.nh.data[21],
			    p_ue_context->nh_ncc_pair.nh.data[22],
			    p_ue_context->nh_ncc_pair.nh.data[23],
			    p_ue_context->nh_ncc_pair.nh.data[24],
			    p_ue_context->nh_ncc_pair.nh.data[25],
			    p_ue_context->nh_ncc_pair.nh.data[26],
			    p_ue_context->nh_ncc_pair.nh.data[27],
			    p_ue_context->nh_ncc_pair.nh.data[28],
			    p_ue_context->nh_ncc_pair.nh.data[29],
			    p_ue_context->nh_ncc_pair.nh.data[30],
			    p_ue_context->nh_ncc_pair.nh.data[31]);
	    /*SPR_17143_END*/
	}
        }

/*BUG 604 changes start*/
        if(!p_ue_context->ue_ctx_mod_security_flag)
        {
/*BUG 604 changes stop*/
            /* UE Aggregate Maximum Bit Rate (M) */
            p_ue_context_info->uEaggregateMaximumBitRate.
                uEaggregateMaximumBitRateDownlink
                = p_ue_context->ue_aggregate_maximum_bitrate.
                uEaggregateMaximumBitRateDL;

            p_ue_context_info->uEaggregateMaximumBitRate.
                uEaggregateMaximumBitRateUplink
                = p_ue_context->ue_aggregate_maximum_bitrate.
                uEaggregateMaximumBitRateUL;
/*BUG 604 changes start*/
        }
        else
        {
            if((PNULL != p_ue_context->p_curr_proc_data)&&
                    (p_ue_context->p_curr_proc_data->m.AggMaxBitRate))
            {
                /* UE Aggregate Maximum Bit Rate (M) */
                p_ue_context_info->uEaggregateMaximumBitRate.
                    uEaggregateMaximumBitRateDownlink
                    = p_ue_context->p_curr_proc_data->ue_aggregate_maximum_bitrate.
                    uEaggregateMaximumBitRateDL;

                p_ue_context_info->uEaggregateMaximumBitRate.
                    uEaggregateMaximumBitRateUplink
                    = p_ue_context->p_curr_proc_data->ue_aggregate_maximum_bitrate.
                    uEaggregateMaximumBitRateUL;
                /* spr_12227_fix_start */
            }
            else
            {
                /* UE Aggregate Maximum Bit Rate (M) */
                p_ue_context_info->uEaggregateMaximumBitRate.
                    uEaggregateMaximumBitRateDownlink
                    = p_ue_context->ue_aggregate_maximum_bitrate.
                    uEaggregateMaximumBitRateDL;

                p_ue_context_info->uEaggregateMaximumBitRate.
                    uEaggregateMaximumBitRateUplink
                    = p_ue_context->ue_aggregate_maximum_bitrate.
                    uEaggregateMaximumBitRateUL;
            }
                /* spr_12227_fix_stop */
        }
/*BUG 604 changes stop*/

        /* Subscriber Profile ID for RAT/Frequency priority(O) */
        if (p_rrc_rrm_ho_required->source_to_target_container.eNBContainer.
          bitmask &
          SOURCE_ENB_TO_TARGET_ENB_TRANSPARENT_CONTAINER_SUB_PROFILE_ID_PRESENT)
        {
            p_ue_context_info->m.subscriberProfileIDforRFPPresent = 1;
            p_ue_context_info->subscriberProfileIDforRFP = 
                p_rrc_rrm_ho_required->source_to_target_container.eNBContainer.
                sub_profile_id;
        }

        /* E-RABs To Be Setup List(M) */
        if (RRC_SUCCESS != uecc_x2ap_build_x2ap_E_RABInformationList(
                    p_ue_context,
                    p_asn1_ctxt,
                    (&p_ue_context_info->e_RABs_ToBeSetup_List)))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_HandoverRequest]"
                    "Failed to fill e_RABInformationList");
            break;
        }

        /* RRC Context(M) */
        if (RRC_SUCCESS != uecc_x2ap_build_x2ap_RRCContext(
                    p_ue_context,
                    p_asn1_ctxt,
                    p_ue_context_info))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_HandoverRequest]"
                    "Failed to fill RRCContext(Handover Preparation Information)");
            break;
        }

        /* filling Handover Restriction List(O) */
        if (p_ue_context->m.ho_restriction_list_present)
        {
            uecc_x2ap_build_x2ap_HO_RestrictionList(
                        p_ue_context,
                        p_asn1_ctxt,
                        p_ue_context_info);
            p_ue_context_info->m.handoverRestrictionListPresent = 1; 
        }

        /* Location Reporting Information(O) */
        if ((p_ue_context->m.location_reporting_info_present)
             && (s1ap_change_of_serve_cell 
                 == p_ue_context->location_reporting_event_type))
        {
            p_ue_context_info->m.locationReportingInformationPresent = 1;
            p_ue_context_info->locationReportingInformation.eventType
                = x2ap_change_of_serving_cell;
            p_ue_context_info->locationReportingInformation.reportArea
                = x2ap_ecgi;
        }

        /* Information Build Successful */
        result = RRC_SUCCESS;
    }while(0);


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * Function Name  : rrc_x2ap_last_visited_utran_cell_information_intrl_enc
 * Inputs         : p_uecc_gb_context
 *                  pctxt (for memory allocation),
 *                  p_last_visited_utran_cell_Item
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs ranap_LastVisitedUTRANCell_Item
 *                  into s1ap_LastVisitedUTRANCellInformation ASN
 ****************************************************************************/
rrc_return_et rrc_x2ap_last_visited_utran_cell_information_intrl_enc
(
    uecc_gb_context_t           *p_uecc_gb_context,
    OSCTXT      *pctxt,       
    U8          *p_buff,       
    U16         *p_buff_size,   
    ranap_LastVisitedUTRANCell_Item  *p_last_visited_utran_cell_Item
)
{
    rrc_return_et  result = RRC_FAILURE;


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);
    RRC_ASSERT(PNULL != p_last_visited_utran_cell_Item);

    do
    {
        pu_setBuffer(pctxt, p_buff, UECC_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_ranap_LastVisitedUTRANCell_Item (pctxt,
                    p_last_visited_utran_cell_Item))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                    p_uecc_gb_context->facility_name, RRC_ERROR,
                "[rrc_x2ap_last_visited_utran_cell_information_intrl_enc]\
                    ASN1 encoding failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result = RRC_SUCCESS;
    }while(0);

    RRC_UT_TRACE_EXIT();
    return result;

}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_x2ap_LastVisitedUTRANCellInformation
 *
 *   Input:  
 *          uecc_ue_context_t*  p_ue_context
 *          last_visited_utran_cell_information_t
 *                       *p_last_visited_utran_cell_information
 *          OSCTXT              *p_asn1_ctx
 *          x2ap_LastVisitedUTRANCellInformation *p_utran_cell_info
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds x2ap_LastVisitedUTRANCellInformation
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
rrc_return_et uecc_x2ap_build_x2ap_LastVisitedUTRANCellInformation(
        uecc_ue_context_t           *p_ue_context,
        last_visited_utran_cell_information_t
        *p_last_visited_utran_cell_information,
        OSCTXT *p_asn1_ctx,
        x2ap_LastVisitedUTRANCellInformation *p_utran_cell_info
        )
{
    ranap_LastVisitedUTRANCell_Item*
        p_last_visited_utran_cell_item = PNULL;
    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;

    U8 sig_msg_ind_buf[sizeof(x2ap_ue_associated_sig_msg_ind_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_ind_t *p_sig_msg_ind =
        (x2ap_ue_associated_sig_msg_ind_t *)sig_msg_ind_buf;
    U16                   message_buff_size = RRC_NULL;
    
    RRC_ASSERT(PNULL!=p_last_visited_utran_cell_information);
    RRC_ASSERT(PNULL!=p_asn1_ctx);
    RRC_ASSERT(PNULL!=p_utran_cell_info);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_last_visited_utran_cell_item =
        (ranap_LastVisitedUTRANCell_Item*)rtxMemAlloc(p_asn1_ctx,
                           sizeof(ranap_LastVisitedUTRANCell_Item));
    if (PNULL == p_last_visited_utran_cell_item)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name, RRC_FATAL, "ASN malloc failed.");
        return RRC_FAILURE;
    }

    asn1Init_ranap_LastVisitedUTRANCell_Item(p_last_visited_utran_cell_item);
    /* copy plmn identity */
    p_last_visited_utran_cell_item->uTRAN_CellID.pLMNidentity.numocts = 3;
    /*initiallzing the memory with all set to zero */
    memset_wrapper((void *)(p_last_visited_utran_cell_item->uTRAN_CellID.\
                pLMNidentity.data), 0, 3);
    uecc_x2ap_generate_plmn_identity_int(
            (OSOCTET *)p_last_visited_utran_cell_item->uTRAN_CellID.\
                        pLMNidentity.data,
                &(p_last_visited_utran_cell_information->
                utran_cell_id.plmn_identity));

    /* copy cell Id */
    l3_memcpy_wrapper((U8*)&p_last_visited_utran_cell_item->uTRAN_CellID.cellID,
            (U8*)p_last_visited_utran_cell_information->utran_cell_id.\
                cell_identity,
            sizeof(p_last_visited_utran_cell_information->utran_cell_id.
                cell_identity));

    /* copy cell type */
    p_last_visited_utran_cell_item->cellType = (ranap_CellType)
        p_last_visited_utran_cell_information->cell_type.cell_size;

    /* copy time_UE_StayedInCell */
    p_last_visited_utran_cell_item->time_UE_StayedInCell =
        p_last_visited_utran_cell_information->time_ue_stayed_in_cell;

    memset_wrapper(sig_msg_req_buf,0,sizeof(x2ap_ue_associated_sig_msg_req_t) + 
            X2AP_MAX_ASN1_BUF_LEN);
    memset_wrapper(sig_msg_ind_buf,0,sizeof(x2ap_ue_associated_sig_msg_ind_t)+ 
            X2AP_MAX_ASN1_BUF_LEN);

    if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
    {
        if( RRC_SUCCESS !=
                rrc_x2ap_last_visited_utran_cell_information_intrl_enc(
                    p_ue_context->p_gb_context, 
                    p_asn1_ctx,
                    &p_sig_msg_req->message_buff_p[0],
                    &p_sig_msg_req->message_buff_size,
                    p_last_visited_utran_cell_item))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, 
                    RRC_ERROR, "Fail to decode x2ap_last_visited_"
                    "utran_cell_information");
            return RRC_FAILURE;
        }
        p_utran_cell_info->numocts
            = p_sig_msg_req->message_buff_size;
        p_utran_cell_info->data =
            (OSOCTET*)rtxMemAlloc(p_asn1_ctx,
                    p_sig_msg_req->message_buff_size);
        if (PNULL ==  p_utran_cell_info->data)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_FATAL, "ASN malloc failed.");
            return RRC_FAILURE;
        }

        memset_wrapper((void *)p_utran_cell_info->data, 0,
                p_sig_msg_req->message_buff_size);
        l3_memcpy_wrapper((U8*)p_utran_cell_info->data,
                p_sig_msg_req->message_buff_p,
                p_sig_msg_req->message_buff_size);
    }
/*BUG 604 changes start*/
    else if( (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
            (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes start*/
    {
        if( RRC_SUCCESS !=
                rrc_x2ap_last_visited_utran_cell_information_intrl_enc(
                    p_ue_context->p_gb_context, 
                    p_asn1_ctx,
                    &p_sig_msg_ind->message_buff_p[0],
                    &message_buff_size,
                    p_last_visited_utran_cell_item))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, 
                    RRC_ERROR, "Fail to decode x2ap_last_visited_"
                    "utran_cell_information");
            return RRC_FAILURE;
        }
        p_utran_cell_info->numocts
            = message_buff_size;
        p_utran_cell_info->data =
            (OSOCTET*)rtxMemAlloc(p_asn1_ctx,
                    message_buff_size);
        if (PNULL ==  p_utran_cell_info->data)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_FATAL, "ASN malloc failed.");
            return RRC_FAILURE;
        }

        memset_wrapper((void *)p_utran_cell_info->data, 0,
                message_buff_size);
        l3_memcpy_wrapper((U8*)p_utran_cell_info->data,
                p_sig_msg_ind->message_buff_p,
                message_buff_size);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_x2ap_UE_HistoryInformation
 *
 *   Input:  
 *          uecc_ue_context_t*  p_ue_context
 *          OSCTXT              *p_asn1_ctx
 *          x2ap_UE_HistoryInformation  *p_ue_history_info
 *          ue_history_t                *p_ue_history_at_rrc_rrm_intf
 *   Outputs : None
 *   DESCRIPTION:
 *       This function build and encode the ue history information which will 
 *       further added to HO_REQUEST message going to target eNB.
 *
 *   RETURNS: 
 *       RRC_SUCCESS or RRC_FAILURE
 *
 *******************************************************************************/
static rrc_return_et uecc_x2ap_build_x2ap_UE_HistoryInformation
(
 uecc_ue_context_t           *p_ue_context,
 OSCTXT                      *p_asn1_ctxt,
 x2ap_UE_HistoryInformation  *p_ue_history_info,
 ue_history_t                *p_ue_history_at_rrc_rrm_intf
 )
{

    OSRTDListNode               *p_node = PNULL;
    x2ap_LastVisitedCell_Item   *p_elem = PNULL;
    last_visited_cell_list_t    *p_last_visited_cell_list= PNULL;
    rrc_counter_t               count = 0;
    last_visited_e_utran_cell_information_t
        *p_last_visited_e_utran_cell_information = PNULL;
    last_visited_utran_cell_information_t
        *p_last_visited_utran_cell_information= PNULL;


    RRC_ASSERT(PNULL != p_asn1_ctxt);
    RRC_ASSERT(PNULL != p_ue_history_at_rrc_rrm_intf);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    asn1Init_x2ap_UE_HistoryInformation(p_ue_history_info);
    p_last_visited_cell_list =
        &(p_ue_history_at_rrc_rrm_intf->last_visited_cell_list);

    for (; count<p_last_visited_cell_list->count; count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctxt,
                x2ap_LastVisitedCell_Item,
                &p_node,
                &p_elem);

        if (PNULL==p_node)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_FATAL,
                    "Can't allocate p_node");
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return RRC_FAILURE;
        }

        asn1Init_x2ap_LastVisitedCell_Item(p_elem);

        if(p_last_visited_cell_list->last_visited_cell_info[count].bitmask
                & LAST_VISITED_CELL_INFO_E_UTRAN_CELL_INFORMATION_PRESENT )
        {
            p_last_visited_e_utran_cell_information =
                &(p_last_visited_cell_list->last_visited_cell_info[count].
                        last_visited_e_utran_cell_information);
            p_elem->t = T_x2ap_LastVisitedCell_Item_e_UTRAN_Cell;

            /* memory allocation */
            p_elem->u.e_UTRAN_Cell =
                (x2ap_LastVisitedEUTRANCellInformation*)
                rtxMemAlloc(p_asn1_ctxt,
                        sizeof(x2ap_LastVisitedEUTRANCellInformation));
            if (PNULL==  p_elem->u.e_UTRAN_Cell)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_FATAL, 
                        "ASN malloc failed.");
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return RRC_FAILURE;
            }

            asn1Init_x2ap_LastVisitedEUTRANCellInformation(p_elem->u.e_UTRAN_Cell);
            p_elem->u.e_UTRAN_Cell->m.iE_ExtensionsPresent = 0;

            /* copy plmn identity */
            p_elem->u.e_UTRAN_Cell->global_Cell_ID.pLMN_Identity.numocts = MCC_OCTET_SIZE;
            uecc_x2ap_generate_plmn_identity_int(
                    p_elem->u.e_UTRAN_Cell->global_Cell_ID.pLMN_Identity.data,
                    &(p_last_visited_e_utran_cell_information->
                        global_cell_id.plmn_identity));

            /* copy cell Id */
            p_elem->u.e_UTRAN_Cell->global_Cell_ID.eUTRANcellIdentifier.numbits = 8*sizeof(
                    p_last_visited_e_utran_cell_information->\
                    global_cell_id.cell_identity);
            p_elem->u.e_UTRAN_Cell->global_Cell_ID.eUTRANcellIdentifier.numbits
                =(p_elem->u.e_UTRAN_Cell->global_Cell_ID.eUTRANcellIdentifier.numbits < MAX_CELL_ID_BITS?
                        p_elem->u.e_UTRAN_Cell->global_Cell_ID.eUTRANcellIdentifier.numbits : MAX_CELL_ID_BITS);

            l3_memcpy_wrapper(p_elem->u.e_UTRAN_Cell->global_Cell_ID.eUTRANcellIdentifier.data,
                    p_last_visited_e_utran_cell_information->\
                    global_cell_id.cell_identity,
                    sizeof(p_last_visited_e_utran_cell_information->
                        global_cell_id.cell_identity));

            /* copy cell type */
            p_elem->u.e_UTRAN_Cell->cellType.m.iE_ExtensionsPresent = 0;
            p_elem->u.e_UTRAN_Cell->cellType.cell_Size =
                p_last_visited_e_utran_cell_information->cell_type.cell_size;

            /* copy time_UE_StayedInCell */
            p_elem->u.e_UTRAN_Cell->time_UE_StayedInCell =
                p_last_visited_e_utran_cell_information->time_ue_stayed_in_cell;
        }

        else if(p_last_visited_cell_list->last_visited_cell_info[count].bitmask
                & LAST_VISITED_CELL_INFO_UTRAN_CELL_INFORMATION_PRESENT)
        {
            p_last_visited_utran_cell_information =
                &(p_last_visited_cell_list->last_visited_cell_info[count].
                        last_visited_utran_cell_information);
            p_elem->t = T_x2ap_LastVisitedCell_Item_uTRAN_Cell;

            /* memory allocation */
            p_elem->u.uTRAN_Cell =
                (x2ap_LastVisitedUTRANCellInformation*)
                rtxMemAlloc(p_asn1_ctxt,
                        sizeof(x2ap_LastVisitedUTRANCellInformation));
            if (PNULL==  p_elem->u.uTRAN_Cell)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_FATAL, 
                        "ASN malloc failed.");
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return RRC_FAILURE;
            }
            asn1Init_x2ap_LastVisitedUTRANCellInformation(p_elem->u.uTRAN_Cell);
            if (RRC_SUCCESS !=
                    uecc_x2ap_build_x2ap_LastVisitedUTRANCellInformation(
                        p_ue_context, 
                        p_last_visited_utran_cell_information,
                        p_asn1_ctxt,
                        p_elem->u.uTRAN_Cell))
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR, 
                        "Failure in filling "
                        "p_last_visited_utran_cell_information");
                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return RRC_FAILURE;
            }
        }
        rtxDListAppendNode(p_ue_history_info, p_node);
    }


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_handover_request 
 *
 *   Input:  
 *          uecc_ue_context_t*  p_ue_context
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds, encode and sends x2ap_HandoverRequest message
 *       to X2AP module.
 *
 *   RETURNS:
 *       RRC_SUCCESS or RRC_FAILURE 
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_build_and_send_handover_request 
(
 uecc_ue_context_t*      p_ue_context
 )
{
    rrc_return_et result = RRC_FAILURE;
    OSCTXT asn1_ctx;
    rrc_rrm_ho_required_t *p_rrc_rrm_ho_required = PNULL;
    x2ap_ECGI       *p_x2ap_eutran_cgi = PNULL;
    x2ap_GUMMEI     *p_x2ap_gummei = PNULL;
    rrc_x2ap_handover_request_t     msg;
    U16                             bitmask = 0;
    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN]; 
    U16    x2ap_ueid = RRC_NULL;
    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;
    U8 sig_msg_ind_buf[sizeof(x2ap_ue_associated_sig_msg_ind_t)+
        X2AP_MAX_ASN1_BUF_LEN]; 
    x2ap_ue_associated_sig_msg_ind_t *p_sig_msg_ind =
        (x2ap_ue_associated_sig_msg_ind_t *)sig_msg_ind_buf;
    
    U16 message_buff_size = RRC_NULL;
    
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (PNULL ==  p_ue_context->ho_info.p_ho_info->p_src_ho_info->
            s1_src_ho_info.p_rrc_rrm_ho_required)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[x2ap_HandoverRequest] p_rrc_rrm_ho_required is NULL");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    /* assigning the p_rrc_rrm_ho_required */
    p_rrc_rrm_ho_required =  p_ue_context->ho_info.p_ho_info->
        p_src_ho_info->s1_src_ho_info.p_rrc_rrm_ho_required;

    /* MME Connection Status */
    if (RRC_FALSE == p_ue_context->p_gb_context->mme_connection_status)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[x2ap_HandoverRequest] MME connection isn't present");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }
    
    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[x2ap_HandoverRequest] ASN context initialization failed");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    do
    {
        /* coverity fix 149480 */
        p_x2ap_eutran_cgi =(x2ap_ECGI*) rtxMemAlloc(&asn1_ctx,sizeof(x2ap_ECGI));
        if(p_x2ap_eutran_cgi == PNULL)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                      p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                      "failed to Allocate p_x2ap_eutran_cgi");
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            break;
        }
        /* coverity fix 149480 */
        /* coverity fix 149389 */
        p_x2ap_gummei =(x2ap_GUMMEI*) rtxMemAlloc(&asn1_ctx,sizeof(x2ap_GUMMEI));
        if(p_x2ap_gummei == PNULL)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                     p_ue_context->p_gb_context->facility_name,RRC_FATAL,
                     "failed to Allocate p_x2ap_gummei");
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            break;
        }
        /* coverity fix 149480 */
        
        /* Reset message */
        memset_wrapper(&msg, 0, sizeof(msg));
        memset_wrapper(sig_msg_req_buf,0, (sizeof(x2ap_ue_associated_sig_msg_req_t) 
                    + X2AP_MAX_ASN1_BUF_LEN)); 
        memset_wrapper(sig_msg_ind_buf,0, (sizeof(x2ap_ue_associated_sig_msg_ind_t) 
                    + X2AP_MAX_ASN1_BUF_LEN)); 
            
        /* Filling message */

        /*Creation of old X2AP_UEID */
        if ((x2ap_ueid = uecc_x2ap_ueid_allocate(p_ue_context->p_gb_context,
                    p_ue_context->ue_index)) == INVALID_X2AP_UE_ID)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[ x2ap_HandoverRequest] Old X2AP_UEID \
                    allocation failed");
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            break;
        }
        p_ue_context->x2ap_context.old_enb_ue_x2ap_id
            = x2ap_ueid;

        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, 
                    RRC_INFO, "OLD X2 UE ID = [%d] generated for UE Index = [%d]",
                    x2ap_ueid, p_ue_context->ue_index);

        
        /* Filling Old eNB UE X2AP ID (M) */
        msg.x2ap_Old_UE_X2AP_ID = p_ue_context->x2ap_context.old_enb_ue_x2ap_id;

        /* Filling Cause (M) */
        if ( RRC_SUCCESS != 
                fill_x2ap_cause(
                    p_ue_context->p_gb_context, 
                    &msg.cause, 
                    &p_rrc_rrm_ho_required->cause))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, 
                    RRC_ERROR, "[x2ap_HandoverRequest]"
                    "Failed to fill x2ap_cause");
            break;

        }

        /* Filling Target Cell ID (M) */
        p_x2ap_eutran_cgi = &(msg.target_cell_id);
        asn1Init_x2ap_ECGI(p_x2ap_eutran_cgi);
        p_x2ap_eutran_cgi->pLMN_Identity.numocts = MCC_OCTET_SIZE;
        uecc_x2ap_generate_plmn_identity_int(
                p_x2ap_eutran_cgi->pLMN_Identity.data,
                &(p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.
                    plmn_identity));

        /* copy cell_identity */
        p_x2ap_eutran_cgi->eUTRANcellIdentifier.numbits = 8*sizeof(
                p_rrc_rrm_ho_required->target_id.enb_id.global_enb_id.
                cell_identity);
        p_x2ap_eutran_cgi->eUTRANcellIdentifier.numbits =
            (p_x2ap_eutran_cgi->eUTRANcellIdentifier.numbits < MAX_CELL_ID_BITS ?
             p_x2ap_eutran_cgi->eUTRANcellIdentifier.numbits : MAX_CELL_ID_BITS);

        l3_memcpy_wrapper(((void*)p_x2ap_eutran_cgi->eUTRANcellIdentifier.data),
                ((const void*)p_rrc_rrm_ho_required->target_id.enb_id.
                 global_enb_id.cell_identity),
                p_x2ap_eutran_cgi->eUTRANcellIdentifier.numbits/8+1);
        p_x2ap_eutran_cgi->m.iE_ExtensionsPresent = 0;

        /* Filling GUMMEI (M) */
        p_x2ap_gummei = &(msg.gummei);
        asn1Init_x2ap_GUMMEI(p_x2ap_gummei);

        p_x2ap_gummei->m.iE_ExtensionsPresent = 0;

        /* SPR 16477 Fix Start */
        if (RRC_RRM_UE_HO_REQUIRED_GUMMEI_INFO_PRESENT & p_rrc_rrm_ho_required->bitmask)
        {
           p_x2ap_gummei->gU_Group_ID.pLMN_Identity.numocts = MCC_OCTET_SIZE;
           uecc_x2ap_generate_plmn_identity_int(
                p_x2ap_gummei->gU_Group_ID.pLMN_Identity.data,
                &(p_rrc_rrm_ho_required->gummei_info.plmn_identity));

            p_x2ap_gummei->gU_Group_ID.mME_Group_ID.numocts
               = MAX_MME_GRP_BYTES;        
            l3_memcpy_wrapper(((void*)p_x2ap_gummei->gU_Group_ID.mME_Group_ID.data),
                ((const void *)p_rrc_rrm_ho_required->gummei_info.mme_group_id),
                MAX_MME_GRP_BYTES);

            p_x2ap_gummei->mME_Code.numocts = sizeof(U8);
            l3_memcpy_wrapper(((void*)p_x2ap_gummei->mME_Code.data),
                ((const void *)&(p_rrc_rrm_ho_required->gummei_info.mmec)),
                sizeof(U8));
        }
        else
        {
        p_x2ap_gummei->gU_Group_ID.pLMN_Identity.numocts
            = p_ue_context->gummei.plmn_identity.numocts;
        l3_memcpy_wrapper(((void*)p_x2ap_gummei->gU_Group_ID.pLMN_Identity.data),
                ((const void *)p_ue_context->gummei.plmn_identity.
                 data),
                p_ue_context->gummei.plmn_identity.numocts);

        p_x2ap_gummei->gU_Group_ID.mME_Group_ID.numocts
            = p_ue_context->gummei.grp_id.numocts;        
        l3_memcpy_wrapper(((void*)p_x2ap_gummei->gU_Group_ID.mME_Group_ID.data),
                ((const void *)p_ue_context->gummei.grp_id.data),
                p_ue_context->gummei.grp_id.numocts);

        p_x2ap_gummei->mME_Code.numocts 
            = p_ue_context->gummei.mme_code.numocts;
        l3_memcpy_wrapper(((void*)p_x2ap_gummei->mME_Code.data),
                ((const void *)p_ue_context->gummei.mme_code.data),
                p_ue_context->gummei.mme_code.numocts);
        }
        /* SPR 16477 Fix Stop */
        /* Filling UE Context Information & RRC Context (M)*/
        if(p_rrc_rrm_ho_required->bitmask
                & RRC_RRM_UE_HO_ADM_RE_SRC_TO_TARGET_CONTAINER_PRESENT)
        {
            bitmask = p_rrc_rrm_ho_required->source_to_target_container.bitmask;
            if (bitmask & SOURCE_TO_TARGET_TRANSPARENT_ENB_CONTAINER_PRESENT)
            {
                if(RRC_SUCCESS !=
                        uecc_x2ap_build_ue_context_information(
                            p_ue_context, 
                            &asn1_ctx, 
                            &msg, 
                            p_rrc_rrm_ho_required))
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,
                            RRC_ERROR, "[x2ap_HandoverRequest]"
                            "Failed to build UE_ContextInformation");
                    break;
                }
            }
            else
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,
                        RRC_ERROR, "[x2ap_HandoverRequest]"
                        "Source to Target eNB Container is not present" 
                        "in RRC_RRM_HO_REQUIRED");
                break;

            }
        }
        else
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_ERROR, "[x2ap_HandoverRequest]"
                    "Source to Target Container is not present" 
                    "in RRC_RRM_HO_REQUIRED");
            break;
        }

        /* filling UE History Information (M) */
        if (RRC_SUCCESS != uecc_x2ap_build_x2ap_UE_HistoryInformation(
                    p_ue_context,
                    &asn1_ctx,
                    &(msg.ue_history_info),
                    &(p_rrc_rrm_ho_required->source_to_target_container.
                        eNBContainer.ue_histroy)))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[uecc_x2ap_HandoverRequest]"
                    "Failed to fill UE_HistoryInformation");
            break;
        }

        /* filling SRVCC Operation Possible (O) */
        if ( p_rrc_rrm_ho_required->bitmask
                & RRC_RRM_UE_HO_ADM_REQ_SRVCC_HO_OP_POSSIBLE_PRESENT)
        {
            msg.bitmask |=
                X2AP_HANDOVER_REQ_SRVCC_OPERATION_POSSIBLE_PRESENT;
            /*srvcc-u start*/
            msg.srv_operation_possible = x2ap_possible;
            /*srvcc-u stop*/
        }

	
    /*BUG 604 changes start*/
    if(p_ue_context->p_gb_context->enbType == HOME_ENB  &&
            RRC_MME_REL10 <= p_ue_context->rel_of_mme )
    {
        if ( (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) && 
               (p_ue_context->ue_ctx_mod_security_flag) && 
                 (PNULL != p_ue_context->p_curr_proc_data) &&
                   (p_ue_context->p_curr_proc_data->m.CsgMembershipStatus) )
        {
	        msg.bitmask |=
                 X2AP_HANDOVER_REQ_CSG_MEMBERSHIP_STATUS_PRESENT;
            msg.csg_membership_status = p_ue_context->p_curr_proc_data->
                                   csg_membership_status;
        }
        else if (p_ue_context->m.CsgMembershipStatus)
	{
	    msg.bitmask |=
                 X2AP_HANDOVER_REQ_CSG_MEMBERSHIP_STATUS_PRESENT;
            msg.csg_membership_status = p_ue_context->csg_membership_status;
	}
    }
    /*BUG 604 changes stop*/

        /* filling TraceActivation */
        /* CELL_TRACE_fix */
	/*SPR 15896 Fix Start*/
                if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
                (PNULL != p_ue_context->p_ue_trace_activation_info))
	/*SPR 15896 Fix Stop*/
        /* CELL_TRACE_fix */
        {
            memset_wrapper(&msg.trace_activation, 0, sizeof(x2ap_TraceActivation));
            msg.trace_activation.eUTRANTraceID.numocts = 
                p_ue_context->p_ue_trace_activation_info->trace_id_info.eutran_trace_id.numoctet;
            l3_memcpy_wrapper(msg.trace_activation.eUTRANTraceID.data,
                       p_ue_context->p_ue_trace_activation_info->trace_id_info.
                          eutran_trace_id.data,
                       msg.trace_activation.eUTRANTraceID.numocts);

            msg.trace_activation.interfacesToTrace.numbits = 8;
            msg.trace_activation.interfacesToTrace.data[0] = 0;
            if (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & S1)
                msg.trace_activation.interfacesToTrace.data[0] |= TRACE_S1AP_INTERFACE;
            if (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & X2)
                msg.trace_activation.interfacesToTrace.data[0] |= TRACE_X2AP_INTERFACE;
            if (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & UU)
                msg.trace_activation.interfacesToTrace.data[0] |= TRACE_UU_INTERFACE;

            msg.trace_activation.traceDepth = p_ue_context->p_ue_trace_activation_info->trace_depth;

            msg.trace_activation.traceCollectionEntityIPAddress.numbits = 
                p_ue_context->p_ue_trace_activation_info->trace_collection_entity_info.length * RRC_OCTET_SIZE;
            msg.trace_activation.traceCollectionEntityIPAddress.data =
                (OSOCTET*)rtxMemAlloc(&asn1_ctx, 
                        p_ue_context->p_ue_trace_activation_info->trace_collection_entity_info.length);
            /* SPR 12964 Fix Start */
            if(PNULL == msg.trace_activation.traceCollectionEntityIPAddress.data)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_FATAL, 
                        "ASN malloc failed.");
                break;
            }
            /* SPR 12964 Fix Stop */
            l3_memcpy_wrapper((void*)msg.trace_activation.traceCollectionEntityIPAddress.data,
                    (const void*)p_ue_context->p_ue_trace_activation_info->trace_collection_entity_info.data,
                    p_ue_context->p_ue_trace_activation_info->trace_collection_entity_info.length);

            msg.trace_activation.m.iE_ExtensionsPresent = 0;
            msg.bitmask |= X2AP_HANDOVER_REQ_TRACE_ACTIVATION_PRESENT;
        }

        if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            /* encoding the handover request messgae */
            result = x2ap_internal_encode_handover_req(&asn1_ctx,
                    &p_sig_msg_req->message_buff_p[0],
                    &p_sig_msg_req->message_buff_size,
                    &msg);

            if (RRC_SUCCESS != result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_HandoverRequest] Build failure");
                break;
            }
            else
            {
                encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);

                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                        encoded_msg_len);
                if (PNULL != p_event_header)
                {
                    memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                                encoded_msg_len));

                    rrc_uecc_fill_protocol_event_params(p_ue_context,
                            p_event_header,
                            (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len),
                            LOCAL_X2_HANDOVER_REQUEST,
                            MESSAGE_DIRECTION_SEND);

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)&p_sig_msg_req->message_buff_p[0],
                            encoded_msg_len);

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }
            }
            p_sig_msg_req->procedure_code = ASN1V_x2ap_id_handoverPreparation;

            p_sig_msg_req->message_type = X2AP_INITIATING_MESSAGE;

            p_sig_msg_req->ue_id = p_ue_context->ue_index;

            p_sig_msg_req->bitmask |=
                X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT;

            p_sig_msg_req->old_enb_ue_x2ap_id 
                = p_ue_context->x2ap_context.old_enb_ue_x2ap_id;

            p_sig_msg_req->peer_enodeb_id = 
                p_ue_context->x2ap_context.peer_enodeb_id;

            p_sig_msg_req->ecgi = P_SRVD_CELL_INFO(p_ue_context->p_gb_context->
                    x2ap_ctx.x2ap_link_info[p_ue_context->x2ap_context.peer_enodeb_id].
                    p_srvd_cell_info, p_ue_context->x2ap_context.serving_cell_index)->
                cell_id;

            /* X2 HandoverRequest message Tracing */
            /* X2_TRACE fix */
            /* last argument - 1 indicates HandoverRequest is being sent */
            uecc_x2ap_trace_handover_request(p_ue_context, &msg, p_sig_msg_req->message_buff_size, 
                    &p_sig_msg_req->message_buff_p[0], 1);
            /* X2_TRACE fix */

            result = uecc_rrc_send_internal_msg(
                    RRC_X2AP_MODULE_ID,
                    X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                        p_sig_msg_req->message_buff_size),
                    sig_msg_req_buf);

            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_INFO, 
                    "X2AP_MSG: HANDOVER REQUEST %s",
                    change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
        }
/*BUG 604 changes start*/
        else if( (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover) ||
                  (INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
        {
            /* encoding the handover request messgae */
            result = x2ap_internal_encode_handover_req(&asn1_ctx,
                    &p_sig_msg_ind->message_buff_p[0],
                    &message_buff_size,
                    &msg);

            if (RRC_SUCCESS != result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_HandoverRequest] Build failure");
                break;
            }
            p_sig_msg_ind->x2ap_procedure_code = RRC_INTER_CELL_HO_PROC_CODE; 
            p_sig_msg_ind->peer_enodeb_id = RRC_INVALID_PEER_ENODEB_ID; 
            p_sig_msg_ind->served_cell_id = p_ue_context->cell_index;
/*BUG 604 changes start*/
            if(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
            {
                /* In Intra cell HO,fill enb_ue_s1ap_id as s1_peer is present*/
                p_sig_msg_ind->bitmask |= X2AP_UE_ASSOC_SIG_MSG_IND_S1_PEER_PRESENT;
                p_sig_msg_ind->enb_ue_s1ap_id = p_ue_context->s1ap_ue_id;
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTRA_CELL_HO_PROC_CODE;
                /*Sending source_ue_index*/
                p_sig_msg_ind->bitmask |= X2AP_UE_ASSOC_SIG_MSG_IND_SRC_UE_INDEX_PRESENT;
                p_sig_msg_ind->source_ue_index = p_ue_context->ue_index;

                /*BUG 604 changes start*/
               if ( (p_ue_context->ue_ctx_mod_security_flag) && 
                      (PNULL != p_ue_context->p_curr_proc_data) )
               {
                   if ( p_ue_context->p_curr_proc_data->m.SecurityKey_present )
                   {
                         p_sig_msg_ind->bitmask |= 
                             X2AP_UE_ASSOC_SIG_MSG_REQ_SECURITY_FLAG_PRESENT;
                         p_sig_msg_ind->is_security_key_present = RRC_TRUE;
                   }
               }
               /*BUG 604 changes stop*/
            }
            else if(INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
            {
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTER_CELL_HO_PROC_CODE; 
            }
/*BUG 604 changes stop*/
            
            result = uecc_rrc_send_inter_cell_internal_msg(
                    RRC_UECCMD_MODULE_ID,
                    X2AP_UE_ASSOCIATED_SIG_MSG_IND,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_ind_t) +
                        message_buff_size),
                    sig_msg_ind_buf);
/*BUG 604 changes start*/
            if(INTRA_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_INFO, 
                        "INTRA_CELL_HO: HANDOVER REQUEST");
            }
            else
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_INFO, 
                        "INTER_CELL_HO: HANDOVER REQUEST");
            }
/*BUG 604 changes stop*/
        }

    } while (0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    rtxMemFreePtr(&asn1_ctx,p_x2ap_gummei);
    rtxMemFreePtr(&asn1_ctx,p_x2ap_eutran_cgi);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}
            
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_ue_context_release_msg
 *
 *   Input:  
 *          OSCTXT              *p_asn1_ctx
 *          x2ap_X2AP_PDU       *p_x2ap_pdu
 *          uecc_gb_context_t*  p_uecc_gb_context
 *          U8                  peer_enodeb_id
 *          U8                  *buffer
 *   Outputs : None
 *   DESCRIPTION:
 *       This function processes X2AP Handover Cancel message
 *
 *   RETURNS:     None
 *
 ******************************************************************************/
void uecc_x2ap_process_ue_context_release_msg
(
    OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    U8                  *buffer
)
{
    uecc_ue_context_t               *p_ue_context = PNULL;
    rrc_x2ap_ue_context_release_t   msg;
    EVENT_EXTERNAL_HEADER           *p_event_header = PNULL;
    U32                             encoded_msg_len = 0;
    /*CR 90 changes start*/
    U8                              counter = 0;
    U8                              qci_value = 0;
    /*CR 90 changes stop*/
    /* SPR 16750 Fix Start */
    rrc_x2ap_error_indication_t     error_indication;
    uecc_ue_context_t               *p_second_ue_context = PNULL;
    /* SPR 16750 Fix Stop */
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    
    /* SPR 16750 Fix Start */
    memset_wrapper(&error_indication, 0, sizeof(rrc_x2ap_error_indication_t));
    /* SPR 16750 Fix Stop */
    do
    {
        if(PNULL ==
                p_x2ap_pdu->u.initiatingMessage->value.u.uEContextRelease)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR, "[X2AP UE CONTEXT RELEASE] \
                    ue ctx release field is NULL");
            break;
        }
        
        /* Decode message */
        if( X2AP_FAILURE == x2ap_internal_decode_ue_context_release(
                    p_asn1_ctx,
                    p_x2ap_pdu->u.initiatingMessage->value.u.uEContextRelease,
                    &msg,
                    peer_enodeb_id
                    ))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, " ue_context_release "
                    "Decoding Failure");
            break;
            
        }    
/*SPR 5253 Start */
/*BUG:604 Fixes start*/
        /*SPR_16618_START*/
        /*SPR_17792_start*/
        if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
        {
            p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                    p_uecc_gb_context,
                    peer_enodeb_id,msg.x2ap_Old_UE_X2AP_ID,OLD_X2AP_UE_ID);
        }
        else
        {
            p_ue_context = uecc_ue_ctx_get_by_x2_id_src_intra_cell_ho(
                    p_uecc_gb_context,msg.x2ap_Old_UE_X2AP_ID,OLD_X2AP_UE_ID);
        }

        /*SPR_17792_end*/
        /*SPR_16618_END*/
        /*BUG:604 Fixes stop*/
/*SPR 5253 Stop */
        if (PNULL != p_ue_context )
        {
            if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
                        p_uecc_gb_context,p_ue_context->cell_index))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING, "cell not active");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                break;
            }
        }

/* X2AP FIX_AG_2 : Start*/

        if (PNULL == p_ue_context )
        {
		
            /*SPR_17792_start*/

            if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
            {
                /* Error. Can't find UE context */
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,
                        "[X2AP UE CTX REL] UE context not found");
                /* SPR 16750 Fix Start */
                /* Drop message */
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
                error_indication.old_enb_ue_x2ap_id = msg.x2ap_Old_UE_X2AP_ID;
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
                error_indication.new_enb_ue_x2ap_id = msg.x2ap_New_UE_X2AP_ID;
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                error_indication.cause.t = T_x2ap_Cause_radioNetwork;
                error_indication.cause.u.radioNetwork = x2ap_unknown_old_eNB_UE_X2AP_ID;
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;
                error_indication.criticality_diagnostics.m.procedureCodePresent = 1;
                error_indication.criticality_diagnostics.procedureCode =
                    ASN1V_x2ap_id_uEContextRelease;
                uecc_x2ap_build_and_send_err_indication(
                        peer_enodeb_id,
                        &error_indication);

                /* Get the UE Context matched with the new_enb_ue_x2ap_id */
                /* contained in the X2AP UE Context Release Message */
                p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(p_uecc_gb_context,
                        peer_enodeb_id, msg.x2ap_New_UE_X2AP_ID, NEW_X2AP_UE_ID);
                if ((PNULL != p_ue_context) && (RRC_NULL == p_ue_context->
                            x2ap_context.m.error_indication_ongoing))
                {
                    p_ue_context->x2ap_context.m.error_indication_ongoing = 1;

                    uecc_fsm_process_event(
                            p_ue_context,
                            UECC_EV_X2AP_ERROR_INDICATION,
                            &error_indication);
                }
                /* SPR 16750 Fix Stop */
            }		
            /*SPR_17792_end*/
            break;
        }
        /* SPR 16750 Fix Start */
        else if ((!(p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present)) ||
                   (p_ue_context->x2ap_context.new_enb_ue_x2ap_id !=
                       msg.x2ap_New_UE_X2AP_ID))
        {
            /*SPR_17792_start*/
            if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,
                        "[X2AP UE CTX REL] NEW ENB UE X2AP ID does not match");
                /* Drop message */
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
                error_indication.old_enb_ue_x2ap_id = msg.x2ap_Old_UE_X2AP_ID;
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
                error_indication.new_enb_ue_x2ap_id = msg.x2ap_New_UE_X2AP_ID;
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                error_indication.cause.t = T_x2ap_Cause_radioNetwork;
                error_indication.cause.u.radioNetwork = x2ap_unknown_pair_of_UE_X2AP_ID;
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;
                error_indication.criticality_diagnostics.m.procedureCodePresent = 1;
                error_indication.criticality_diagnostics.procedureCode =
                    ASN1V_x2ap_id_uEContextRelease;
                uecc_x2ap_build_and_send_err_indication(
                        peer_enodeb_id,
                        &error_indication);

                if (RRC_NULL == p_ue_context->x2ap_context.m.error_indication_ongoing)
                {
                    p_ue_context->x2ap_context.m.error_indication_ongoing = 1;

                    uecc_fsm_process_event(
                            p_ue_context,
                            UECC_EV_X2AP_ERROR_INDICATION,
                            &error_indication);
                }

                /* Get the second UE Context matched with the new_enb_ue_x2ap_id */
                /* contained in the X2AP UE Context Release Message */
                p_second_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(p_uecc_gb_context,
                        peer_enodeb_id, msg.x2ap_New_UE_X2AP_ID, NEW_X2AP_UE_ID);
                if ((PNULL != p_second_ue_context) && (RRC_NULL == p_second_ue_context->
                            x2ap_context.m.error_indication_ongoing))
                {
                    p_second_ue_context->x2ap_context.m.error_indication_ongoing = 1;

                    uecc_fsm_process_event(
                            p_second_ue_context,
                            UECC_EV_X2AP_ERROR_INDICATION,
                            &error_indication);
                }
            }		
            /*SPR_17792_end*/

            break;
        }
        /* SPR 16750 Fix Stop */
/*BUG 604 changes start*/
        else if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
/*BUG 604 changes stop*/
        {
            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                            encoded_msg_len));
                rrc_uecc_fill_protocol_event_params(p_ue_context,
                        p_event_header,
                        (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len),
                        LOCAL_UE_CONTEXT_RELEASE,
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)buffer,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
/*BUG 604 changes start*/

        /* TRACE_changes_start */
        uecc_x2ap_ue_assoc_msg_max_trace_handler(p_ue_context, buffer, encoded_msg_len, "UE Context Release", 0);
        /* TRACE_changes_start */

        /*CR 90 changes start*/
         /* SPR 21070 Fix Start */ 
        if ((PNULL != p_ue_context) && (PNULL != g_rrc_stats.p_rrc_stats) &&
		(PNULL != p_ue_context->uecc_fsm.p_current_state->s_name) && 
             	(strcmp_wrapper(p_ue_context->uecc_fsm.p_current_state->s_name,(const S8 *)"UECC_UE_CONNECTION_RELEASE_ONGOING")))
         /* SPR 21070 Fix End */
        {
            for (counter = 0; counter < MAX_ERAB_COUNT; counter++)
            {   
                if (PNULL != p_ue_context->p_e_rab_list[counter])
                {     
                    qci_value = p_ue_context->p_e_rab_list[counter]->
                        e_RABlevelQoSParameters.qCI;

                    /* SPR 13441 Fix Start */
                    if ( (RRC_MIN_QCI_GBR_LIMIT <= qci_value) &&
                            (RRC_MAX_QCI_NON_GBR_LIMIT >= qci_value) )
                    {
                    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                            g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                            p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].
                            rrc_erab_related_stats.erab_release_stats.
                            erab_relAttNbr_qci[RRC_UPDATED_QCI_INDEX(qci_value)],
                            RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);

                    RRC_UPDATE_PERFORMANCE_COUNTER(p_ue_context->p_gb_context,
                            g_rrc_stats.p_rrc_stats->
                            rrc_uecc_stats.rrc_uecc_instance_stats[p_ue_context->
                            p_gb_context->offset_uecc_mod_id].
                            rrc_uecc_cell_stats[p_ue_context->cell_index].rrc_erab_related_stats.
                            erab_release_stats.erab_relAttNbr_sum, 
                            RRC_UPDATED_QCI_INDEX(qci_value), RRC_MAX_QCI_INDEX);
                    }
                    /* SPR 13441 Fix End */
                }
            }
        }
    }
/*BUG 604 changes stop*/
        /*CR 90 changes stop*/

/* X2AP FIX_AG_2 : End*/

        /* Call      event handler *
         * and pass API data to FSM. */
        /* WARNING. The uecc_event_queue_push_event can't be used here because
         * the stack variable is used for storing the API data
         * */
        uecc_fsm_process_event(
                p_ue_context,
                UECC_EV_X2AP_UE_CTX_RELEASE, 
                &msg);

    } while (0);
    
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/*SPR_17976_START*/
 /******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_sn_status_trnsfr_msg
 *
 *   Input:  
 *          OSCTXT              *p_asn1_ctx
 *          x2ap_X2AP_PDU       *p_x2ap_pdu
 *          uecc_gb_context_t*  p_uecc_gb_context
 *          U8                  peer_enodeb_id
 *          U8                  *buffer
 *          void                *p_api
 *   Outputs : None
 *   DESCRIPTION:
 *       This function processes X2AP Process SN Status Transfer message.
 *
 *   RETURNS:     None
 *
 ******************************************************************************/
void uecc_x2ap_process_sn_status_trnsfr_msg
(
    OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    U8                  *buffer,
    void                *p_api
)
/*SPR_17976_END*/
{
    uecc_ue_context_t               *p_ue_context = PNULL;
    rrc_x2ap_sn_status_transfer_t   msg;
    /* SPR 16750 Fix Start */
    uecc_ue_context_t               *p_second_ue_context = PNULL;
    /* SPR 16750 Fix Stop */

    rrc_x2ap_error_indication_t     error_indication;
    rrc_return_et                   response = RRC_SUCCESS;
    EVENT_EXTERNAL_HEADER           *p_event_header = PNULL;
    U32                             encoded_msg_len = 0;
    OSRTDListNode  *p_node_msg = PNULL;
    x2ap_E_RABs_SubjectToStatusTransfer_List_element *p_erab_elem = PNULL;
    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    
    memset_wrapper(&error_indication, 0, sizeof(rrc_x2ap_error_indication_t));

    do
    {
        if(PNULL ==
                p_x2ap_pdu->u.initiatingMessage->value.u.snStatusTransfer)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR, "[X2AP_SN STATUS TRANSFER]"
                    " sn status transfer field is NULL");
            break;
        }
        
        /* Decode message */
        if( X2AP_FAILURE == x2ap_internal_decode_sn_status_transfer(
                    p_asn1_ctx,                    
                    p_x2ap_pdu->u.initiatingMessage->value.u.snStatusTransfer,
                    &msg,
                    peer_enodeb_id
                    ))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR,
             "[X2AP SN STATUS TRANSFER] Decoding Failure");
            break;
        }    
        else
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR,
             "[X2AP SN STATUS TRANSFER] Decoding Success");
        }    

/*SPR 5253 Start */
/*BUG 604 changes start*/
        /*SPR_16618_START*/
        /*SPR_17792_start*/
        if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
        {
            p_ue_context = uecc_ue_ctx_get_by_x2ap_id_ho_cancel(p_uecc_gb_context,
                    peer_enodeb_id, msg.x2ap_New_UE_X2AP_ID, NEW_X2AP_UE_ID);

        }
        else
        {
            p_ue_context = uecc_ue_ctx_get_by_x2_id_intra_cell_ho_cancel(
                    p_uecc_gb_context, msg.x2ap_New_UE_X2AP_ID, NEW_X2AP_UE_ID);
        }
        /*SPR_17792_end*/
        /*SPR_16618_END*/
/*BUG 604 changes stop*/
/*SPR 5253 Stop */

        if (PNULL != p_ue_context )
        {
            if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
                        p_uecc_gb_context,p_ue_context->cell_index))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING, "cell not active");
                break;
            }

            /*SPR_17976_START*/
            if(RRC_TRUE == p_ue_context->trg_ho_fail_rrc_cre_flag)
            {
                if(0 == 
                        TENB_CRE_Q_GET_COUNT(&p_ue_context->p_tenb_cre_event_queue))
                {
                   /*Coverity Fix 98783 Start */ 
                    uecc_event_queue_push_tenb_cre_event((uecc_fsm_event_et)ASN1V_x2ap_id_snStatusTransfer,
                   /*Coverity Fix 98783 End */ 
                            p_ue_context, 
                            p_api, 
                            &p_ue_context->p_tenb_cre_event_queue);
                }
                else
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                            facility_name, RRC_WARNING,
                            "X2AP SN STATUS TRANSFER msg is ignored because reestablishment "
                            "procedure ongoing during handover for ue_index: %d"
                            " and one event is already queued",
                            p_ue_context->ue_index );
                }
                break;
            }
            /*SPR_17976_END*/
        }
        
        /*SPR_17976_START*/
        rrc_asn1PrtToStr_X2AP_PDU(RRC_ASN, "X2AP_PDU", p_x2ap_pdu);
        /*SPR_17976_END*/
/* X2AP FIX_AG_2 : Start*/

        if (PNULL == p_ue_context )
        {
            /*SPR_17792_start*/
            if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
            {
                /* Error. Can't find UE context */
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,
                        "[X2AP SN STATUS TRANSFER] UE context not found");
                /*SPR_16604_START*/
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
                error_indication.old_enb_ue_x2ap_id = msg.x2ap_Old_UE_X2AP_ID;
                /*SPR_16604_END*/
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
                error_indication.new_enb_ue_x2ap_id = msg.x2ap_New_UE_X2AP_ID;
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                error_indication.cause.t = T_x2ap_Cause_radioNetwork;
                error_indication.cause.u.radioNetwork = x2ap_unknown_new_eNB_UE_X2AP_ID;
                /* SPR 16750 Fix Start */
                error_indication.criticality_diagnostics.m.procedureCodePresent = 1;
                error_indication.criticality_diagnostics.procedureCode =
                    ASN1V_x2ap_id_snStatusTransfer;
                /* Get the UE Context matched with the old_enb_ue_x2ap_id */
                /* received in the X2AP SN STATUS TRANSFER Message */
                p_ue_context = uecc_ue_ctx_get_by_x2ap_id_ho_cancel(p_uecc_gb_context,
                        peer_enodeb_id, msg.x2ap_Old_UE_X2AP_ID, OLD_X2AP_UE_ID);
                if ((PNULL != p_ue_context) && (RRC_NULL == p_ue_context->
                            x2ap_context.m.error_indication_ongoing))
                {
                    p_ue_context->x2ap_context.m.error_indication_ongoing = 1;

                    uecc_fsm_process_event(
                            p_ue_context,
                            UECC_EV_X2AP_ERROR_INDICATION,
                            &error_indication);
                }
                /* SPR 16750 Fix Stop */
            }
            /*SPR_17792_end*/

            response = RRC_FAILURE;

            break;
        }
        else
        {   /* SPR 16750 Fix Start */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
             "[X2AP SN STATUS TRANSFER] UE context for NEW_X2AP_UE_ID %u found", 
             msg.x2ap_New_UE_X2AP_ID);
            /* SPR 16750 Fix Stop */
        }    
        if ((peer_enodeb_id == p_ue_context->x2ap_context.peer_enodeb_id) &&
                (p_ue_context->x2ap_context.old_enb_ue_x2ap_id !=
                msg.x2ap_Old_UE_X2AP_ID ))
        {   /* SPR 16750 Fix Start */
            /*SPR_17792_start*/
            if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_INFO,
                        "[X2AP SN STATUS TRANSFER] Old_UE_X2AP_ID is not matched. "
                        "Sending UECC_EV_X2AP_ERROR_INDICATION to UE ID [%d]", p_ue_context->ue_index); 
                /* SPR 16750 Fix Stop */
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
                error_indication.old_enb_ue_x2ap_id = msg.x2ap_Old_UE_X2AP_ID;
                /*SPR_16604_START*/
                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
                error_indication.new_enb_ue_x2ap_id = msg.x2ap_New_UE_X2AP_ID;
                /*SPR_16604_END*/
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                error_indication.cause.t = T_x2ap_Cause_radioNetwork;
                /* SPR 16750 Fix Start */
                error_indication.cause.u.radioNetwork =
                    x2ap_unknown_pair_of_UE_X2AP_ID; 
                error_indication.criticality_diagnostics.m.procedureCodePresent = 1;
                error_indication.criticality_diagnostics.procedureCode =
                    ASN1V_x2ap_id_snStatusTransfer;

                if (RRC_NULL == p_ue_context->x2ap_context.m.error_indication_ongoing)
                {
                    p_ue_context->x2ap_context.m.error_indication_ongoing = 1;
                    uecc_fsm_process_event(
                            p_ue_context,
                            UECC_EV_X2AP_ERROR_INDICATION,
                            &error_indication);
                }
                /* Get the second matching UE context with old eNB UE X2AP Id */ 
                p_second_ue_context = uecc_ue_ctx_get_by_x2ap_id_ho_cancel(p_uecc_gb_context,
                        peer_enodeb_id, msg.x2ap_Old_UE_X2AP_ID, OLD_X2AP_UE_ID);

                if (PNULL != p_second_ue_context )
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                            facility_name, RRC_INFO,
                            "[X2AP SN STATUS TRANSFER] Sending UECC_EV_X2AP_ERROR_INDICATION to UE ID [%d]", 
                            p_second_ue_context->ue_index); 
                    if (RRC_NULL == p_second_ue_context->x2ap_context.m.error_indication_ongoing)
                    {
                        p_second_ue_context->x2ap_context.m.error_indication_ongoing = 1;
                        uecc_fsm_process_event(
                                p_second_ue_context,
                                UECC_EV_X2AP_ERROR_INDICATION,
                                &error_indication);
                    }
                }
                /* SPR 16750 Fix Stop */
            }
            /*SPR_17792_end*/
            response = RRC_FAILURE;
            break;
        }

    /*BUG 604 changes start*/
        if ((PNULL != p_ue_context) && (INTER_CELL_HO != 
                    p_ue_context->ho_info.s1_or_x2_handover)&&
                (INTRA_CELL_HO !=
                 p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
        {
            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                            encoded_msg_len));
                rrc_uecc_fill_protocol_event_params(p_ue_context,
                        p_event_header,
                        (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len),
                        LOCAL_SN_STATUS_TRANSFER,
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)buffer,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

    /*BUG 604 changes start*/
        if(X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
        {
    /*BUG 604 changes stop*/
	    	/*SPR 15896 Fix Start*/
        if (p_ue_context->m.traceActivated)
        {
	    	/*SPR 15875 Fix Start*/
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
				    (p_ue_context->p_ue_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(),GEN_MME, "\n <msg function=\"X2AP\" name=\"SN STATUS TRANSFER\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_trace_activation_info->rawtime));
		    }
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
				    (p_ue_context->p_ue_cell_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(),GEN_CELL, "\n <msg function=\"X2AP\" name=\"SN STATUS TRANSFER\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_cell_trace_activation_info->rawtime));
            }
            /*SPR 15875 Fix Stop*/
            /* coverity_97194 start */
            if(p_ue_context->x2ap_context.peer_enodeb_id < MAX_PEER_ENB)            	
                P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(),X2,"\n <initiator type=\"ENB\">%s</initiator>",
                        p_ue_context->p_gb_context->x2ap_ctx.
                        x2ap_link_info[p_ue_context->x2ap_context.peer_enodeb_id].enb_comm_info.ip_addr[0].ip_addr);
            /* coverity_97194 stop */

            rrc_trace_max_file_write(p_ue_context,X2AP_SN_STATUS_TRANSFER,encoded_msg_len, buffer, X2);

                P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(),X2,"\n <ieGroup name=\"E-RABs_SubjectTo_StatusTransfer_List\">");
	    	/*SPR 15896 Fix Stop*/
                for (p_node_msg = msg.erab_subject_to_status_transfer_list.head; PNULL!=p_node_msg;
                        p_node_msg=p_node_msg->next)
                {
                    p_erab_elem=(x2ap_E_RABs_SubjectToStatusTransfer_List_element*)p_node_msg->data;
                    if ( p_erab_elem == PNULL)
                    {
                        continue;
                    }

	    	/*SPR 15896 Fix Start*/
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n <ieGroup name=\"E-RABs_SubjectTo_StatusTransfer_Item\">");
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n <ie name=\"E-RAB ID\">%d</ie>",p_erab_elem->value.e_RAB_ID);
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup>");
                }
                P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup>");
                P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </msg>");
	    	/*SPR 15896 Fix Stop*/
        }
    /*BUG 604 changes start*/
        }
    /*BUG 604 changes stop*/
/* X2AP FIX_AG_2 : End*/

        /* Call      event handler *
         * and pass API data to FSM. */
        /* WARNING. The uecc_event_queue_push_event can't be used here because
         * the stack variable is used for storing the API data
         * */
       RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
       "[X2AP SN STATUS TRANSFER]: push event - UECC_EV_X2AP_SN_STATUS_TRANSFER");

        uecc_fsm_process_event(
                p_ue_context,
                UECC_EV_X2AP_SN_STATUS_TRANSFER, 
                &msg);

    } while (0);
    
    if (RRC_FAILURE == response)
    {
        if ((RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT &
             error_indication.bitmask) ||
             (RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT &
              error_indication.bitmask) ||
             (RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT &
              error_indication.bitmask))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "build and send error-indication message");

            if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
            {
                uecc_x2ap_build_and_send_err_indication(
                        peer_enodeb_id,
                        &error_indication);
            }
        }
    }
    
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************
 * Build and Send function                *
 ******************************************/ 

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_handover_request_ack
 *
 *   Input:  
 *          uecc_ue_context_t*      p_ue_context
 *           x2ap_CriticalityDiagnostics *p_criticality_diagnostics
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds and sends HO Request Ack message
 *       to X2AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_build_and_send_handover_request_ack
(
 uecc_ue_context_t*      p_ue_context,
 x2ap_CriticalityDiagnostics *p_criticality_diagnostics
 )
{
    rrc_return_et result = RRC_FAILURE;
    OSCTXT asn1_ctx;
    rrc_x2ap_handover_req_ack_t  msg;
    x2ap_TargeteNBtoSource_eNBTransparentContainer 
        target_enb_to_source_enb_transparent_container;
    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    U16   x2ap_ueid = RRC_NULL;
    LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE_STRUCT 
				*p_x2_handover_req_ack = PNULL;
    U32                         encoded_msg_len = 0;
    U8 count = 0;
    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;
    U8 sig_msg_ind_buf[sizeof(x2ap_ue_associated_sig_msg_ind_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_ind_t *p_sig_msg_ind =
        (x2ap_ue_associated_sig_msg_ind_t *)sig_msg_ind_buf;
    rrc_module_id_t       uecc_module_id = RRC_INVALID_UECC_MODULE_ID;
    U16                   message_buff_size = RRC_NULL;
    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/
    U32 e_rab_index = 0;

    RRC_ASSERT(PNULL != p_ue_context);
    if(PNULL == p_ue_context->p_gb_context)
    {
        /* uecc_global context is null */
        return RRC_FAILURE;
    }

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (RRC_FALSE == p_ue_context->p_gb_context->mme_connection_status)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                " MME connection isn't present");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[ x2ap_HandoverRequestAcknowledge] ASN context initialization failed");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    do
    {
        /* Reset message */
        memset_wrapper(&msg, 0, sizeof(msg));
        memset_wrapper(sig_msg_req_buf,0,sizeof(x2ap_ue_associated_sig_msg_req_t) + 
                X2AP_MAX_ASN1_BUF_LEN);
        memset_wrapper(sig_msg_ind_buf,0,sizeof(x2ap_ue_associated_sig_msg_ind_t) + 
                X2AP_MAX_ASN1_BUF_LEN);
        
        asn1Init_x2ap_TargeteNBtoSource_eNBTransparentContainer(
                &target_enb_to_source_enb_transparent_container);

        /* Fill message */
        msg.old_ue_x2ap_id = p_ue_context->x2ap_context.old_enb_ue_x2ap_id;

        /*Creation of new X2AP_UEID */
        if ((x2ap_ueid = uecc_x2ap_ueid_allocate(p_ue_context->p_gb_context,
                    p_ue_context->ue_index)) == INVALID_X2AP_UE_ID)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[ x2ap_HandoverRequestAcknowledge] New X2AP_UEID \
                    allocation failed");
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            break;
        }

        p_ue_context->x2ap_context.new_enb_ue_x2ap_id
            = x2ap_ueid;

        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, 
                    RRC_INFO, "New eNB-UE-X2AP-ID [%u] generated for UE Index = [%d]",
                    x2ap_ueid, p_ue_context->ue_index);
        p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present = 1; 

        msg.new_ue_x2ap_id = 
            p_ue_context->x2ap_context.new_enb_ue_x2ap_id;

        if (RRC_SUCCESS != uecc_x2ap_build_x2ap_e_rab_admitted_list(  
                    p_ue_context, &asn1_ctx, &msg.e_rab_admitted_list))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_HandoverRequestAcknowledge]"
                    "uecc_x2ap_build_x2ap_e_rab_admitted_list failure");

            break;
        }

        if ((PNULL != p_ue_context->ho_info.p_ho_info) && p_ue_context->ho_info.p_ho_info->
                p_trg_ho_info->s1_trg_ho_info.erab_failed_list.count)
        {
            if (RRC_SUCCESS != uecc_x2ap_build_x2ap_e_rab_not_admitted_list( 
                        p_ue_context, 
                        &asn1_ctx, &msg.e_rab_not_admitted_list))
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_HandoverRequestAcknowledge]"
                        "uecc_x2ap_build_x2ap_e_rab_not_admitted_list failure");

                break;
            }
            else
            {
                msg.bitmask |= 
                    X2AP_HANDOVER_REQ_ACK_ERAB_NOT_ADMITTED_LIST_PRESENT;
            }
        }

        if (RRC_SUCCESS != uecc_x2ap_build_rrc_ho_command(
                    p_ue_context,
                    &msg,
                    &asn1_ctx))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_HandoverRequestAcknowledge]"
                    "uecc_x2ap_build_rrc_ho_command failure");

            break;
        }


        if (PNULL != p_criticality_diagnostics)
        {
            msg.criticality_diagnostics = *p_criticality_diagnostics;
            msg.bitmask |= X2AP_HANDOVER_REQ_ACK_CRIT_DIAGNO_PRESENT;
        }

        if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            /* Encode message */
            result = x2ap_internal_encode_handover_req_ack(&asn1_ctx,
                    &p_sig_msg_req->message_buff_p[0],
                    &p_sig_msg_req->message_buff_size,
                    &msg);
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_HandoverRequestAcknowledge] Build failure");
                break;
            }
            else
            {
                /*Generating Protocol Events LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE */
                encoded_msg_len = (U32)p_sig_msg_req->message_buff_size;
                p_x2_handover_req_ack = rrc_mem_get(sizeof(LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE_STRUCT) + 
                        encoded_msg_len);
                if (PNULL != p_x2_handover_req_ack)
                {
                    memset_wrapper(p_x2_handover_req_ack, RRC_NULL, 
                            (sizeof(LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE_STRUCT) 
                             + encoded_msg_len));

                    p_x2_handover_req_ack->header.length = sizeof(LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE_STRUCT) 
                        + encoded_msg_len;
                    p_x2_handover_req_ack->header.event_id = LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE;

                    if ((PNULL != p_ue_context->p_gb_context->p_p_csc_context) && 
                        (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]) && 
                        (PNULL != p_ue_context->p_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                                p_csc_init_setup_ind))
                    {
                        l3_memcpy_wrapper(&p_x2_handover_req_ack->header.EVENT_PARAM_CELL_ID, 
                                p_ue_context->p_gb_context->
                                p_p_csc_context[p_ue_context->cell_index]->
                                p_csc_init_setup_ind->sib_type_1_Info.
                                cell_access_related_info.cell_Id, sizeof(U32));
                    }
                    p_x2_handover_req_ack->header.EVENT_PARAM_EUTRANCELL_FROID = 
                        p_x2_handover_req_ack->header.EVENT_PARAM_CELL_ID;

                    if (p_ue_context->m.mme_ue_id_present)
                    {
                        p_x2_handover_req_ack->header.EVENT_PARAM_MMES1APID = 
                            p_ue_context->mme_ue_id;
                    }

                    p_x2_handover_req_ack->header.EVENT_PARAM_RAC_UE_REF = 
                        p_ue_context->s1ap_ue_id;

                    p_x2_handover_req_ack->EVENT_PARAM_MESSAGE_DIRECTION = 
                        MESSAGE_DIRECTION_SEND;
                    p_x2_handover_req_ack->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;

                    l3_memcpy_wrapper((void*)(((U8*)p_x2_handover_req_ack) + 
                                sizeof(LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE_STRUCT)),
                            (const void*)&p_sig_msg_req->message_buff_p[0],
                            encoded_msg_len);

                    if (msg.bitmask & X2AP_HANDOVER_REQ_ACK_ERAB_NOT_ADMITTED_LIST_PRESENT)
                    {
                        if ((PNULL != p_ue_context->ho_info.p_ho_info) && (PNULL != 
                                    p_ue_context->ho_info.p_ho_info->p_trg_ho_info))
                        {
                            for (count = 0; (count < p_ue_context->ho_info.p_ho_info->
                                        p_trg_ho_info->s1_trg_ho_info.erab_failed_list.count) &&
                                    (count < MAX_ERAB_COUNT); count++)
                            {
                                p_x2_handover_req_ack->EVENT_PARAM_HO_OUT_PREP_ERAB_FAIL_BITMAP =
                                    ( 1 << p_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                                      s1_trg_ho_info.erab_failed_list.e_rab_failed_to_add_list[count].
                                      e_rab_id);
                            }
                        }
                    }

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_x2_handover_req_ack);
                }   	    
            }
            p_sig_msg_req->procedure_code = 
                ASN1V_x2ap_id_handoverPreparation;

            p_sig_msg_req->message_type = X2AP_SUCCESSFUL_OUTCOME;

            p_sig_msg_req->cell_index = p_ue_context->cell_index;

            p_sig_msg_req->ue_id = p_ue_context->ue_index;

            p_sig_msg_req->bitmask |=
                X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT;
            p_sig_msg_req->old_enb_ue_x2ap_id = 
                p_ue_context->x2ap_context.old_enb_ue_x2ap_id;

            p_sig_msg_req->bitmask |= 
                X2AP_UE_ASSOC_SIG_MSG_REQ_NEW_ENB_UE_X2AP_ID_PRESENT;
            p_sig_msg_req->new_enb_ue_x2ap_id = 
                p_ue_context->x2ap_context.new_enb_ue_x2ap_id;

            p_sig_msg_req->peer_enodeb_id = 
                p_ue_context->x2ap_context.peer_enodeb_id;

            /* Is trace activated */
	    	/*SPR 15896 Fix Start*/
            if (p_ue_context->m.traceActivated)
            {
	    	    /*SPR 15875 Fix Start*/
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
				    (p_ue_context->p_ue_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_MME, "\n<msg function=\"X2AP\" name=\"Handover Request Ack\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_trace_activation_info->rawtime));
		    }
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
				    (p_ue_context->p_ue_cell_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL, "\n<msg function=\"X2AP\" name=\"Handover Request Ack\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_cell_trace_activation_info->rawtime));
		    }
	    	    /*SPR 15875 Fix Stop*/
                P_RRC_TRACE(p_ue_context, set_all_trace_bitmask (),X2,"\n <target type=\"ENB\">%s</target>",
                        p_ue_context->p_gb_context->x2ap_ctx.
                        x2ap_link_info[p_ue_context->x2ap_context.peer_enodeb_id].enb_comm_info.ip_addr[0].ip_addr);

                /* ASN.1 encoded message to logged */
                rrc_trace_max_file_write(p_ue_context, X2AP_HANDOVER_REQUEST_ACK,
                        p_sig_msg_req->message_buff_size,
                        &p_sig_msg_req->message_buff_p[0], X2);

                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n <ieGroup name=\"E-RABs_Admitted_List\">");
                    for (e_rab_index=0; e_rab_index < MAX_ERAB_COUNT; e_rab_index++)
                    {
                        if (PNULL == p_ue_context->p_e_rab_list[e_rab_index])
                        {
                            continue;
                        }
                        P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n <ieGroup name=\"E-RABs_Admitted_Item\">");
                        P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"E-RAB_ID\">%d</ie> ",
                                p_ue_context->p_e_rab_list[e_rab_index]->uecc_x2ap_drb_ctx.e_RAB_ID);
                        P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup>");
                    }
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup>");

                    if( (PNULL != p_ue_context->ho_info.p_ho_info)  && 
                            (PNULL != p_ue_context->ho_info.p_ho_info->p_trg_ho_info) &&
                            (p_ue_context->ho_info.p_ho_info->p_trg_ho_info->s1_trg_ho_info.
                             erab_failed_list.count > 0))
                    {
                        P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n <ieGroup name=\"E-RABs_NotAdmitted_List\">");
                        for (e_rab_index=0; 
                                e_rab_index < p_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                                s1_trg_ho_info.erab_failed_list.count; 
                                e_rab_index++)
                        {
                            P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n <ieGroup name=\"E-RABs List Item\">");
                            P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"E-RAB_ID\">%d</ie> ",
                                    p_ue_context->ho_info.p_ho_info->p_trg_ho_info->s1_trg_ho_info.erab_failed_list.
                                    e_rab_failed_to_add_list[e_rab_index].e_rab_id);
                            P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup>");
                        }
                        P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup>");
                    }
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(),X2,"\n </msg>");
	    	/*SPR 15896 Fix Stop*/
            }

            /* Send message to X2AP  */
            result = uecc_rrc_send_internal_msg(
                    RRC_X2AP_MODULE_ID,
                    X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                        p_sig_msg_req->message_buff_size),
                    sig_msg_req_buf);

            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_INFO,
                    "X2AP_MSG: HANDOVER REQUEST ACKNOWLEDGE %s",
                    change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
        }
/*BUG 604 changes start*/
        else if( (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
        {
            result = x2ap_internal_encode_handover_req_ack(&asn1_ctx,
                    &p_sig_msg_ind->message_buff_p[0],
                    &message_buff_size,
                    &msg);
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_HandoverRequestAcknowledge] Build failure");
                break;
            }
/*BUG 604 changes start*/
            if(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
            {
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTER_CELL_HO_PROC_CODE; 
            }
            else if(INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
            {
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTRA_CELL_HO_PROC_CODE;
                /*Fill the target_ue_index*/
                p_sig_msg_ind->bitmask |= X2AP_UE_ASSOC_SIG_MSG_IND_TRG_UE_INDEX_PRESENT;
                p_sig_msg_ind->target_ue_index = p_ue_context->ue_index;
            }
/*BUG 604 changes stop*/
            p_sig_msg_ind->peer_enodeb_id = RRC_INVALID_PEER_ENODEB_ID; 
            p_sig_msg_ind->served_cell_id = p_ue_context->cell_index;

            uecc_module_id  = (U8)RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                    msg.old_ue_x2ap_id);

            result = uecc_rrc_send_inter_cell_internal_msg(
                    uecc_module_id,
                    X2AP_UE_ASSOCIATED_SIG_MSG_IND,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_ind_t) +
                        message_buff_size),
                    sig_msg_ind_buf);

        /*BUG 604 changes stop*/
            if(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_INFO, 
                        "INTER_CELL_HO: HANDOVER REQUEST ACKNOWLEDGE");
            }
            else
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_INFO, 
                        "INTRA_CELL_HO: HANDOVER REQUEST ACKNOWLEDGE");
            }
        /*BUG 604 changes stop*/
        }

    } while (0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return result;
}


/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_x2ap_e_rab_admitted_list
 *
 *   Input:  
 *          uecc_ue_context_t* p_ue_context
 *          OSCTXT              *p_asn1_ctx
 *          x2ap_E_RABs_Admitted_List *p_e_rab_admitted_list
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds x2ap_E_RABs_Admitted_List
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
static rrc_return_et uecc_x2ap_build_x2ap_e_rab_admitted_list
(
 uecc_ue_context_t* p_ue_context,
 OSCTXT *p_asn1_ctx,
 x2ap_E_RABs_Admitted_List *p_e_rab_admitted_list
 )
{
    uecc_ho_drb_forward_info_list_t          
        **pp_erab_trg_ho_forward_list = PNULL;
    OSRTDListNode* p_node = PNULL;
    x2ap_E_RABs_Admitted_List_element  *p_elem = PNULL;
    U8 e_rab_index = 0;
    /*U8 *p_transport_addr = PNULL;*/



    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_e_rab_admitted_list);
    RRC_ASSERT(PNULL != p_ue_context->ho_info.p_ho_info);
    RRC_ASSERT(PNULL != p_ue_context->ho_info.p_ho_info->p_trg_ho_info);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    asn1Init_x2ap_E_RABs_Admitted_List(p_e_rab_admitted_list);

    for (e_rab_index=0; 
            e_rab_index < MAX_ERAB_COUNT; 
            e_rab_index++)
    {
        if (PNULL == p_ue_context->p_e_rab_list[e_rab_index]) 
        {
            continue;
        }

        rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_E_RABs_Admitted_List_element,
                &p_node,
                &p_elem);

        if (PNULL == p_node)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, 
                    RRC_FATAL,"Can't allocate p_node");

            rtxDListFreeAll (p_asn1_ctx, p_e_rab_admitted_list);

            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return RRC_FAILURE;
        }

        asn1Init_x2ap_E_RABs_Admitted_List_element(p_elem);
        asn1Init_x2ap_E_RABs_Admitted_Item(&p_elem->value);

        p_elem->id = ASN1V_x2ap_id_E_RABs_Admitted_Item;
        p_elem->criticality = x2ap_ignore;

        p_elem->value.e_RAB_ID = p_ue_context->
            p_e_rab_list[e_rab_index]->uecc_x2ap_drb_ctx.e_RAB_ID;

/*BUG 604 changes start*/
        if (INTRA_CELL_HO !=  p_ue_context->ho_info.s1_or_x2_handover)
        {
/*BUG 604 changes stop*/
        pp_erab_trg_ho_forward_list = &p_ue_context->ho_info.
            p_ho_info->p_trg_ho_info->s1_trg_ho_info.
            p_erab_trg_ho_forward_list[0];

        if (pp_erab_trg_ho_forward_list[e_rab_index])
        {
            if ((UECC_FORWARDING_DIRECTION_UL == 
                        pp_erab_trg_ho_forward_list[e_rab_index]->
                        forward_direction) ||
                    (UECC_FORWARDING_DIRECTION_BOTH == 
                     pp_erab_trg_ho_forward_list[e_rab_index]->forward_direction))
            {
                p_elem->value.uL_GTP_TunnelEndpoint.gTP_TEID = 
                    pp_erab_trg_ho_forward_list[e_rab_index]->
                    x2ap_drb_forward_info[UL_FWD_INFO_INDEX].teid_self;

                p_elem->value.uL_GTP_TunnelEndpoint.transportLayerAddress = 
                    pp_erab_trg_ho_forward_list[e_rab_index]->
                    x2ap_drb_forward_info[UL_FWD_INFO_INDEX].
                    transportLayerAddress;

                p_elem->value.m.uL_GTP_TunnelEndpointPresent = 1;
            }
            
            if ((UECC_FORWARDING_DIRECTION_DL ==   
                        pp_erab_trg_ho_forward_list[e_rab_index]->
                        forward_direction) ||
                    (UECC_FORWARDING_DIRECTION_BOTH == 
                     pp_erab_trg_ho_forward_list[e_rab_index]->forward_direction))
            {
                p_elem->value.dL_GTP_TunnelEndpoint.gTP_TEID = 
                    pp_erab_trg_ho_forward_list[e_rab_index]->
                    x2ap_drb_forward_info[DL_FWD_INFO_INDEX].teid_self;

                p_elem->value.dL_GTP_TunnelEndpoint.transportLayerAddress =
                    pp_erab_trg_ho_forward_list[e_rab_index]->
                    x2ap_drb_forward_info[DL_FWD_INFO_INDEX].
                    transportLayerAddress;

                p_elem->value.m.dL_GTP_TunnelEndpointPresent = 1;
            }
        }
        }
/*BUG 604 changes stop*/
        rtxDListAppendNode(
                p_e_rab_admitted_list,
                p_node);
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_x2ap_e_rab_not_admitted_list
 *
 *   Input:  
 *          uecc_ue_context_t* p_ue_context
 *          OSCTXT              *p_asn1_ctx
 *          x2ap_E_RABs_Admitted_List *p_e_rab_not_admitted_list
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds x2ap_E_RAB_List
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
static rrc_return_et uecc_x2ap_build_x2ap_e_rab_not_admitted_list
(
 uecc_ue_context_t* p_ue_context,
 OSCTXT *p_asn1_ctx,
 x2ap_E_RAB_List *p_e_rab_not_admitted_list
 )
{
    U16 max_erab_count = 0;
    OSRTDListNode* p_node = PNULL;
    x2ap_E_RAB_List_element  *p_elem = PNULL;
    U8 count = 0;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_e_rab_not_admitted_list);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    asn1Init_x2ap_E_RAB_List(p_e_rab_not_admitted_list);

    max_erab_count = p_ue_context->ho_info.p_ho_info->p_trg_ho_info->
        s1_trg_ho_info.erab_failed_list.count;

    for (count = 0; 
            count < max_erab_count; 
            count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_E_RAB_List_element,
                &p_node,
                &p_elem);

        if (PNULL == p_node)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, 
                    RRC_FATAL,"Can't allocate p_node");

            rtxDListFreeAll (p_asn1_ctx, p_e_rab_not_admitted_list);

            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return RRC_FAILURE;
        }

        asn1Init_x2ap_E_RAB_List_element(p_elem);
        p_elem->value.u._x2ap_E_RAB_ItemIEs_1 = 
            rtxMemAllocType(p_asn1_ctx, x2ap_E_RAB_Item);

        if (PNULL == p_elem->value.u._x2ap_E_RAB_ItemIEs_1)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_FATAL,
                    " ASN malloc failed.");
            break;
        }
        asn1Init_x2ap_E_RAB_Item(p_elem->value.u._x2ap_E_RAB_ItemIEs_1);
        p_elem->value.t = T286x2ap___x2ap_E_RAB_ItemIEs_1;

        p_elem->id = ASN1V_x2ap_id_E_RAB_Item;
        p_elem->criticality = x2ap_ignore;

        p_elem->value.u._x2ap_E_RAB_ItemIEs_1->e_RAB_ID = 
            p_ue_context->ho_info.p_ho_info->p_trg_ho_info->
            s1_trg_ho_info.erab_failed_list.
            e_rab_failed_to_add_list[count].e_rab_id;


        p_elem->value.u._x2ap_E_RAB_ItemIEs_1->cause = 
            p_ue_context->ho_info.p_ho_info->p_trg_ho_info->
            s1_trg_ho_info.erab_failed_list.
            e_rab_failed_to_add_list[count].x2ap_cause;

        rtxDListAppendNode(
                p_e_rab_not_admitted_list,
                p_node);
    }
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_rrc_ho_command
 *
 *   Input:  
 *          uecc_ue_context_t*  *p_uecc_ue_context
 *          rrc_x2ap_handover_req_ack_t* p_handover_request_msg
 *          OSCTXT         *pctxt
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds and HandoverCommand  message
 *
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_build_rrc_ho_command(
        uecc_ue_context_t   *p_uecc_ue_context,
        rrc_x2ap_handover_req_ack_t* p_handover_request_msg,
        OSCTXT         *pctxt
)
{
    HandoverCommand*                        p_ho_cmd_msg = PNULL;
    x2ap_TargeteNBtoSource_eNBTransparentContainer* p_container = PNULL;
    rrc_return_et                           result = RRC_FAILURE;
    /*valgrind_fix_start*/
    //OSCTXT         asn1_ctx;
    /*valgrind_fix_stop*/
    /*U8 msg[X2AP_MAX_ASN1_BUF_LEN];
    OSOCTET  *p_data = PNULL;*/


    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_handover_request_msg);
    RRC_ASSERT(PNULL != pctxt);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
     
    p_ho_cmd_msg = &(p_handover_request_msg->
                    target_to_src_enb_transparent_cont.ho_command);

    p_container = &(p_handover_request_msg->
                        target_to_src_enb_transparent_cont.
                            target_to_source_transparent_container);
    

    memset_wrapper(p_ho_cmd_msg, 0, sizeof(HandoverCommand));

    /*valgrind_fix_start*/
    /* Init ASN1 context */
 /*  if (RT_OK != rtInitContext(&asn1_ctx)) 
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
                p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[HandoverCommand] "
                "ASN1 context initialization failed.");
        return RRC_FAILURE;
    }*/
    /*valgrind_fix_stop*/

    do
    {
        /*Fill HandoverCommand*/
        p_ho_cmd_msg->criticalExtensions.t = 
            T_HandoverCommand_criticalExtensions_c1;
        p_ho_cmd_msg->criticalExtensions.u.c1 = 
            (HandoverCommand_criticalExtensions_c1*)
            rtxMemAllocType(pctxt, HandoverCommand_criticalExtensions_c1);

        if (PNULL == p_ho_cmd_msg->criticalExtensions.u.c1)
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_FATAL,
                    "[HandoverCommand] ASN malloc failed.");
            break;
        }

        p_ho_cmd_msg->criticalExtensions.u.c1->t =
            T_HandoverCommand_criticalExtensions_c1_handoverCommand_r8;

        p_ho_cmd_msg->criticalExtensions.u.c1->u.handoverCommand_r8 = 
            rtxMemAllocType(pctxt, HandoverCommand_r8_IEs);

        if (PNULL == p_ho_cmd_msg->criticalExtensions.u.c1->u.handoverCommand_r8)
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_FATAL,
                    "[HandoverCommand] ASN malloc failed.");
            break;
        }

        asn1Init_HandoverCommand_r8_IEs(p_ho_cmd_msg->
                criticalExtensions.u.c1->u.handoverCommand_r8);

        if (RRC_SUCCESS != uecc_s1ap_build_rrc_con_reconfig(
                    p_uecc_ue_context,
                    &(p_ho_cmd_msg->criticalExtensions.u.c1->
                    u.handoverCommand_r8->handoverCommandMessage),
                    pctxt))
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[HandoverCommand]"
                    "uecc_s1ap_build_rrc_con_reconfig failure");

            break;
        }

        if (X2AP_SUCCESS != x2ap_internal_encode_handover_command(
                    pctxt,
                    p_ho_cmd_msg,
                    p_container
                    ))
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[uecc_x2ap_build_rrc_ho_command]"
                    "rrc_x2ap_Handover_Command_intrl_enc"
                    " returned failure");

            break;
        }

        result = RRC_SUCCESS;

    }while(0);

    /* Free ASN Context */
    /*valgrind_fix_start*/
//    rtFreeContext(&asn1_ctx);
    /*valgrind_fix_stop*/

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_handover_cancel
 *
 *   Input:  
 *          uecc_ue_context_t* p_ue_context
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds and sends x2ap_HandoverCancel message
 *       to X2AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_build_and_send_handover_cancel
(
 uecc_ue_context_t*      p_ue_context
 )
{
    rrc_return_et result = RRC_FAILURE;
    OSCTXT asn1_ctx;
    rrc_x2ap_handover_cancel_t msg;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;
    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;
    U8 sig_msg_ind_buf[sizeof(x2ap_ue_associated_sig_msg_ind_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_ind_t *p_sig_msg_ind =
        (x2ap_ue_associated_sig_msg_ind_t *)sig_msg_ind_buf;
    rrc_module_id_t       uecc_module_id = RRC_INVALID_UECC_MODULE_ID;
    U16                   message_buff_size = RRC_NULL;
    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);


    if (RRC_FALSE == p_ue_context->p_gb_context->mme_connection_status)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                " MME connection isn't present");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[x2ap_HandoverCancel] ASN context initialization failed");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    do
    {
        /* Reset message */
        memset_wrapper(&msg, 0, sizeof(msg));
        memset_wrapper(sig_msg_req_buf,0,sizeof(x2ap_ue_associated_sig_msg_req_t) + 
                X2AP_MAX_ASN1_BUF_LEN);
        memset_wrapper(sig_msg_ind_buf,0,sizeof(x2ap_ue_associated_sig_msg_ind_t) + 
                X2AP_MAX_ASN1_BUF_LEN);

        /* Fill message */
        /*Fill Old eNB UE X2AP ID*/
        msg.x2ap_Old_UE_X2AP_ID = 
            p_ue_context->x2ap_context.old_enb_ue_x2ap_id;

        /*Fill New eNB UE X2AP ID*/
        if (p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present)
        {
            msg.x2ap_New_UE_X2AP_ID = 
                p_ue_context->x2ap_context.new_enb_ue_x2ap_id;
            
            msg.bitmask |= X2AP_HANDOVER_CANCEL_NEW_UE_X2AP_ID_PRESENT;
        }
        /*Fill Cause*/
        msg.cause = p_ue_context->ho_info.x2ap_cause;


        if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            /* Encode message */
            result = x2ap_internal_encode_handover_cancel(&asn1_ctx,
                    &p_sig_msg_req->message_buff_p[0],
                    &p_sig_msg_req->message_buff_size,
                    &msg); 
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_HandoverCancel] Build failure");
                break;
            }
            else
            {
                encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);
                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                        encoded_msg_len);
                if (PNULL != p_event_header)
                {
                    memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                                encoded_msg_len));

                    rrc_uecc_fill_protocol_event_params(p_ue_context,
                            p_event_header,
                            (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len),
                            LOCAL_X2_HANDOVER_CANCEL,
                            MESSAGE_DIRECTION_SEND);

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)&p_sig_msg_req->message_buff_p[0],
                            encoded_msg_len);

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }
            }

            p_sig_msg_req->procedure_code = ASN1V_x2ap_id_handoverCancel;
            p_sig_msg_req->message_type = X2AP_INITIATING_MESSAGE;

            p_sig_msg_req->ue_id = p_ue_context->ue_index;

            p_sig_msg_req->bitmask |=
                X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT;
            p_sig_msg_req->old_enb_ue_x2ap_id = 
                p_ue_context->x2ap_context.old_enb_ue_x2ap_id;

            p_sig_msg_req->peer_enodeb_id = 
                p_ue_context->x2ap_context.peer_enodeb_id;

            /* Is trace activated */
	    	/*SPR 15896 Fix Start*/
            if (p_ue_context->m.traceActivated)
            {
	    	    /*SPR 15875 Fix Start*/
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
				    (p_ue_context->p_ue_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(),GEN_MME, "\n<msg function=\"X2AP\" name=\"Handover Cancel\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_trace_activation_info->rawtime));
		    }
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
				    (p_ue_context->p_ue_cell_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(),GEN_CELL, "\n<msg function=\"X2AP\" name=\"Handover Cancel\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_cell_trace_activation_info->rawtime));
		    }
	    	    /*SPR 15875 Fix Stop*/
                 P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), X2,"\n <target type=\"ENB\">%s</target>",
                        p_ue_context->p_gb_context->x2ap_ctx.
                        x2ap_link_info[p_ue_context->x2ap_context.peer_enodeb_id].enb_comm_info.ip_addr[0].ip_addr);

                /* ASN.1 encoded message to logged */
                rrc_trace_max_file_write(p_ue_context, X2AP_HANDOVER_CANCEL,
                        p_sig_msg_req->message_buff_size,
                        &p_sig_msg_req->message_buff_p[0], X2);

                    uecc_x2ap_p_rrc_trace_for_cause(p_ue_context, msg.cause);
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </msg>");
	    	/*SPR 15896 Fix Stop*/
            }

            /* Send message to X2AP  */
            result = uecc_rrc_send_internal_msg(
                    RRC_X2AP_MODULE_ID,
                    X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                        p_sig_msg_req->message_buff_size),
                    sig_msg_req_buf);

            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_INFO,
                    "X2AP_MSG: HANDOVER CANCEL %s", change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
        }
    /*BUG 604 changes start*/
        else if( (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
        {
            /* Encode message */
            result = x2ap_internal_encode_handover_cancel(&asn1_ctx,
                    &p_sig_msg_ind->message_buff_p[0],
                    &message_buff_size,
                    &msg); 
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_HandoverCancel] Build failure");
                break;
            }
            p_sig_msg_ind->peer_enodeb_id = RRC_INVALID_PEER_ENODEB_ID; 
            p_sig_msg_ind->served_cell_id = p_ue_context->cell_index;

            if (p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present)
            {
/*BUG 604 changes start*/
                if(INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
                {
                    p_sig_msg_ind->x2ap_procedure_code = RRC_INTER_CELL_HO_PROC_CODE; 
                }
                else
                {
                    p_sig_msg_ind->x2ap_procedure_code = RRC_INTRA_CELL_HO_PROC_CODE; 
                }
/*BUG 604 changes stop*/
                uecc_module_id  = (U8)RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                        msg.x2ap_New_UE_X2AP_ID);
            }
            else
            {
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTER_CELL_HO_CANCEL_PROC_CODE; 
                uecc_module_id = RRC_UECCMD_MODULE_ID;
            }

            result = uecc_rrc_send_inter_cell_internal_msg(
                    uecc_module_id,
                    X2AP_UE_ASSOCIATED_SIG_MSG_IND,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_ind_t) +
                        message_buff_size),
                    sig_msg_ind_buf);

/*BUG 604 changes start*/
            if(INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_INFO,
                        "INTER_CELL_HO: HANDOVER CANCEL");
            }
            else
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_INFO, 
                        "INTRA_CELL_HO: HANDOVER REQUEST");
            }
/*BUG 604 changes stop*/

        }

    } while (0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_handover_prep_failure
 *
 *   Input:  
 *          uecc_ue_context_t* p_ue_context
 *          x2ap_CriticalityDiagnostics *p_criticality_diagnostics
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds and sends x2ap_HandoverPreparationFailure message
 *       to X2AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_build_and_send_handover_prep_failure
(
  uecc_ue_context_t*      p_ue_context,
  x2ap_CriticalityDiagnostics *p_criticality_diagnostics
 )
{
    rrc_return_et result = RRC_FAILURE;
    OSCTXT asn1_ctx;
    rrc_x2ap_handover_prep_fail_t msg;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;
    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;
    U8 sig_msg_ind_buf[sizeof(x2ap_ue_associated_sig_msg_ind_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_ind_t *p_sig_msg_ind =
        (x2ap_ue_associated_sig_msg_ind_t *)sig_msg_ind_buf;
    rrc_module_id_t       uecc_module_id = RRC_INVALID_UECC_MODULE_ID;
    U16                   message_buff_size = RRC_NULL;
    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/

    RRC_ASSERT(PNULL != p_ue_context);
    /* Coverity 63641 Fix Start */
    if (PNULL == p_ue_context->p_gb_context)
    {
	    return result;
    }
    /* Coverity 63641 Fix Stop */

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (RRC_FALSE == p_ue_context->p_gb_context->mme_connection_status)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                " MME connection isn't present");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[x2ap_HandoverPreparationFailure] "
                "ASN context initialization failed");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    do
    {
        /* Reset message */
        memset_wrapper(&msg, 0, sizeof(msg));
        memset_wrapper(sig_msg_req_buf,0,sizeof(x2ap_ue_associated_sig_msg_req_t) + 
                X2AP_MAX_ASN1_BUF_LEN);
        memset_wrapper(sig_msg_ind_buf,0,sizeof(x2ap_ue_associated_sig_msg_ind_t) + 
                X2AP_MAX_ASN1_BUF_LEN);

        /* Fill message */
        /*Fill Old eNB UE X2AP ID*/
        msg.x2ap_Old_UE_X2AP_ID = 
            p_ue_context->x2ap_context.old_enb_ue_x2ap_id;
       
        /*Fill cause*/ 
        msg.cause = p_ue_context->ho_info.x2ap_cause;

        /*Fill Criticality Diagnostics*/
        if (PNULL != p_criticality_diagnostics)
        {
            msg.criticality_diagnostics = *p_criticality_diagnostics;
            msg.bitmask |= X2AP_HANDOVER_PREP_FAIL_CRIT_DIAGNO_PRESENT;
        }

        if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            /* Encode message */
            result = x2ap_internal_encode_handover_preparation_failure(&asn1_ctx,
                    &p_sig_msg_req->message_buff_p[0],
                    &p_sig_msg_req->message_buff_size,
                    &msg); 
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_HandoverPreparationFailure] Build failure");
                break;
            }
            else
            {
                /*Generating Protocol Event LOCAL_X2_HANDOVER_PREPARATION_FAILURE*/
                encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);

                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                        encoded_msg_len);
                if (PNULL != p_event_header)
                {
                    memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                                encoded_msg_len));

                    rrc_uecc_fill_protocol_event_params(p_ue_context,
                            p_event_header,
                            (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len),
                            LOCAL_X2_HANDOVER_PREPARATION_FAILURE,
                            MESSAGE_DIRECTION_SEND);

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)&p_sig_msg_req->message_buff_p[0],
                            encoded_msg_len);

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }
            }

            p_sig_msg_req->procedure_code = ASN1V_x2ap_id_handoverPreparation;
            p_sig_msg_req->message_type = X2AP_UNSUCCESSFUL_OUTCOME; 

            p_sig_msg_req->ue_id = p_ue_context->ue_index;

            p_sig_msg_req->bitmask |=
                X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT;
            p_sig_msg_req->old_enb_ue_x2ap_id = 
                p_ue_context->x2ap_context.old_enb_ue_x2ap_id;

            /* Is trace activated */
	    	/*SPR 15896 Fix Start*/
            if (p_ue_context->m.traceActivated)
            {
	    	    /*SPR 15875 Fix Start*/
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
				    (p_ue_context->p_ue_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_MME,"\n<msg function=\"X2AP\" name=\"Handover Preparation Failure\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_trace_activation_info->rawtime));
		    }
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
				    (p_ue_context->p_ue_cell_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"\n<msg function=\"X2AP\" name=\"Handover Preparation Failure\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_cell_trace_activation_info->rawtime));
		    }
	    	    /*SPR 15875 Fix Stop*/
                P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), X2,"\n <target type=\"ENB\">%s</target>",
                        p_ue_context->p_gb_context->x2ap_ctx.
                        x2ap_link_info[p_ue_context->x2ap_context.peer_enodeb_id].enb_comm_info.ip_addr[0].ip_addr);

                /* ASN.1 encoded message to logged */
                rrc_trace_max_file_write(p_ue_context, X2AP_HANDOVER_PREP_FAILURE,
                        p_sig_msg_req->message_buff_size,
                        &p_sig_msg_req->message_buff_p[0], X2);

                    uecc_x2ap_p_rrc_trace_for_cause(p_ue_context, msg.cause);
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </msg>");
	    	/*SPR 15896 Fix Stop*/
            }
            /* SPR_17846_START*/
            p_sig_msg_req->peer_enodeb_id =
                p_ue_context->x2ap_context.peer_enodeb_id;
            /* SPR_17846_END*/

            /* Send message to X2AP  */
            result = uecc_rrc_send_internal_msg(
                    RRC_X2AP_MODULE_ID,
                    X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                        p_sig_msg_req->message_buff_size),
                    sig_msg_req_buf);

            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                    p_ue_context->p_gb_context->facility_name, RRC_INFO, 
                    "X2AP_MSG: HANDOVER PREPARATION FAILURE %s", change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
        }
/*BUG 604 changes start*/
        else if( (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
        {
            result = x2ap_internal_encode_handover_preparation_failure(&asn1_ctx,
                    &p_sig_msg_ind->message_buff_p[0],
                    &message_buff_size,
                    &msg); 
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_HandoverPreparationFailure] Build failure");
                break;
            }
/*BUG 604 changes start*/
            if(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
            {
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTER_CELL_HO_PROC_CODE; 
            }
            else
            {
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTRA_CELL_HO_PROC_CODE; 
            }
/*BUG 604 changes stop*/
            p_sig_msg_ind->peer_enodeb_id = RRC_INVALID_PEER_ENODEB_ID; 
            p_sig_msg_ind->served_cell_id = p_ue_context->cell_index;
            
            uecc_module_id  = (U8)RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                msg.x2ap_Old_UE_X2AP_ID);
            
            result = uecc_rrc_send_inter_cell_internal_msg(
                    uecc_module_id,
                    X2AP_UE_ASSOCIATED_SIG_MSG_IND,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_ind_t) +
                        message_buff_size),
                    sig_msg_ind_buf);
/*BUG 604 changes start*/
            if(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
            {

                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                        p_ue_context->p_gb_context->facility_name, RRC_INFO, 
                        "INTER_CELL HO: HANDOVER PREPARATION FAILURE %s", change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
            }
            else
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                        p_ue_context->p_gb_context->facility_name, RRC_INFO, 
                        "INTRA_CELL HO: HANDOVER PREPARATION FAILURE %s", change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
            }
/*BUG 604 changes stop*/
            
        }

    } while (0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_handover_prep_failure_at_init
 *
 *   Input:  
 *          uecc_gb_context_t* p_uecc_gb_context
 *          x2ap_UE_X2AP_ID                  old_ue_x2ap_id
 *          x2ap_Cause                       *p_failure_cause
 *          x2ap_CriticalityDiagnostics      *p_criticality_diagnostics
 *          U8                               peer_enodeb_id
 *          rrc_x2ap_error_indication_t      *p_err_ind_content
 *          U8                               procedure_code
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds and sends x2ap_HandoverPreparationFailure message
 *       to X2AP module before allocation of Ue Context.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_build_and_send_handover_prep_failure_at_init
(
    uecc_gb_context_t                *p_uecc_gb_context,
    x2ap_UE_X2AP_ID                  old_ue_x2ap_id,
    x2ap_Cause                       *p_failure_cause,
    U8                               peer_enodeb_id,
    rrc_x2ap_error_indication_t      *p_err_ind_content,
    U8                               procedure_code
)
{
    rrc_return_et result = RRC_FAILURE;
    OSCTXT asn1_ctx;
    rrc_x2ap_handover_prep_fail_t msg;
    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;
    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;
    U8 sig_msg_ind_buf[sizeof(x2ap_ue_associated_sig_msg_ind_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_ind_t *p_sig_msg_ind =
        (x2ap_ue_associated_sig_msg_ind_t *)sig_msg_ind_buf;
    
    rrc_module_id_t       uecc_module_id = RRC_INVALID_UECC_MODULE_ID;
    U16                   message_buff_size = RRC_NULL;

    RRC_ASSERT(PNULL != p_failure_cause);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);


    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                p_uecc_gb_context->facility_name, RRC_ERROR,
                "[x2ap_HandoverPreparationFailure] "
                "ASN context initialization failed");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return RRC_FAILURE;
    }

    do
    {
        /* Reset message */
        memset_wrapper(&msg, 0, sizeof(msg));
        memset_wrapper(sig_msg_req_buf,0,sizeof(x2ap_ue_associated_sig_msg_req_t) + 
                X2AP_MAX_ASN1_BUF_LEN);
        memset_wrapper(sig_msg_ind_buf,0,sizeof(x2ap_ue_associated_sig_msg_ind_t) + 
                X2AP_MAX_ASN1_BUF_LEN);

        /* Fill message */
        /*Fill Old eNB UE X2AP ID*/
        msg.x2ap_Old_UE_X2AP_ID = old_ue_x2ap_id;
       
        /*Fill cause*/ 
        msg.cause = *p_failure_cause;

        /*Fill Criticality Diagnostics*/
        if (PNULL != p_err_ind_content)
        {
            msg.criticality_diagnostics = p_err_ind_content->criticality_diagnostics;
            msg.bitmask |= X2AP_HANDOVER_PREP_FAIL_CRIT_DIAGNO_PRESENT;
        }

        if(ASN1V_x2ap_id_handoverPreparation == procedure_code)
        {
            /* Encode message */
            result = x2ap_internal_encode_handover_preparation_failure(&asn1_ctx,
                    &p_sig_msg_req->message_buff_p[0],
                    &p_sig_msg_req->message_buff_size,
                    &msg); 
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_HandoverPreparationFailure] Build failure");
                break;
            }
            else
            {
                /*Generating Protocol Event LOCAL_X2_HANDOVER_PREPARATION_FAILURE*/
                encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);

                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                        encoded_msg_len);
                if (PNULL != p_event_header)
                {
                    memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                                encoded_msg_len));

                    p_event_header->length = (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len);
                    p_event_header->event_id = LOCAL_X2_HANDOVER_PREPARATION_FAILURE;

                    if ( (PNULL != 
                                p_uecc_gb_context->p_p_csc_context) && (PNULL != 
                                    p_uecc_gb_context->p_p_csc_context[p_sig_msg_req->cell_index])
                            && (PNULL != p_uecc_gb_context->p_p_csc_context[p_sig_msg_req->cell_index]->
                                p_csc_init_setup_ind))
                    {
                        l3_memcpy_wrapper(&p_event_header->EVENT_PARAM_CELL_ID, p_uecc_gb_context->
                                p_p_csc_context[p_sig_msg_req->cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
                                cell_access_related_info.cell_Id, sizeof(U32));
                    }
                    p_event_header->EVENT_PARAM_EUTRANCELL_FROID = p_event_header->
                        EVENT_PARAM_CELL_ID;
                    p_event_header->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;
                    p_event_header->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)&p_sig_msg_req->message_buff_p[0],
                            encoded_msg_len);

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }
            }

            p_sig_msg_req->procedure_code = ASN1V_x2ap_id_handoverPreparation;
            p_sig_msg_req->message_type = X2AP_UNSUCCESSFUL_OUTCOME; 
            /* UE Index is don't care at Init */
            p_sig_msg_req->bitmask |=
                X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT;
            p_sig_msg_req->old_enb_ue_x2ap_id = old_ue_x2ap_id;
            p_sig_msg_req->peer_enodeb_id = peer_enodeb_id;

            /* Send message to X2AP  */
            result = uecc_rrc_send_internal_msg(
                    RRC_X2AP_MODULE_ID,
                    X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                        p_sig_msg_req->message_buff_size),
                    sig_msg_req_buf);

            RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                    p_uecc_gb_context->facility_name, RRC_INFO,
                    "X2AP_MSG: HANDOVER PREPARATION FAILURE %s", change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
        }
/*BUG 604 changes start*/
        else if((RRC_INTER_CELL_HO_PROC_CODE == procedure_code) ||
                (RRC_INTRA_CELL_HO_PROC_CODE == procedure_code))
/*BUG 604 changes stop*/
        {
            result = x2ap_internal_encode_handover_preparation_failure(&asn1_ctx,
                    &p_sig_msg_ind->message_buff_p[0],
                    &message_buff_size,
                    &msg); 
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                        p_uecc_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_HandoverPreparationFailure] Build failure");
                break;
            }
            p_sig_msg_ind->x2ap_procedure_code = RRC_INTER_CELL_HO_PROC_CODE; 
            p_sig_msg_ind->peer_enodeb_id = RRC_INVALID_PEER_ENODEB_ID; 
            p_sig_msg_ind->served_cell_id = p_uecc_gb_context->inter_cell_ho_cell_index;
/*BUG 604 changes start*/
            if(RRC_INTER_CELL_HO_PROC_CODE == procedure_code)
            {
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTER_CELL_HO_PROC_CODE; 
            }
            else
            {
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTRA_CELL_HO_PROC_CODE; 
            }
/*BUG 604 changes stop*/

            uecc_module_id  = (U8)RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                    msg.x2ap_Old_UE_X2AP_ID);

            result = uecc_rrc_send_inter_cell_internal_msg(
                    uecc_module_id,
                    X2AP_UE_ASSOCIATED_SIG_MSG_IND,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_ind_t) +
                        message_buff_size),
                    sig_msg_ind_buf);

/*BUG 604 changes start*/
            if(RRC_INTER_CELL_HO_PROC_CODE == procedure_code)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name, RRC_INFO,
                        "INTER_CELL_HO: HANDOVER PREPARATION FAILURE %s", change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
            }
            else
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name, RRC_INFO,
                        "INTRA_CELL_HO: HANDOVER PREPARATION FAILURE %s", change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
            }
/*BUG 604 changes stop*/

        }

    } while (0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);

    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_ue_context_release
 *
 *   Input:  
 *          uecc_ue_context_t* p_ue_context
 *          x2ap_Cause              *p_cause
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds and sends x2ap_UEContextRelease message
 *       to X2AP module
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
rrc_return_et uecc_x2ap_build_and_send_ue_context_release(
        uecc_ue_context_t*      p_ue_context
        )
{
    rrc_return_et result = RRC_FAILURE;
    OSCTXT asn1_ctx;
    rrc_x2ap_ue_context_release_t msg;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;
    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;
    U8 sig_msg_ind_buf[sizeof(x2ap_ue_associated_sig_msg_ind_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_ind_t *p_sig_msg_ind =
        (x2ap_ue_associated_sig_msg_ind_t *)sig_msg_ind_buf;
    
    rrc_module_id_t       uecc_module_id = RRC_INVALID_UECC_MODULE_ID;
    U16                   message_buff_size = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (RRC_FALSE == p_ue_context->p_gb_context->mme_connection_status)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                " MME connection isn't present");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[x2ap_UEContextRelease] ASN context initialization failed");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    do
    {
        /* Reset message */
        memset_wrapper(&msg, 0, sizeof(msg));
        memset_wrapper(sig_msg_req_buf,0,sizeof(x2ap_ue_associated_sig_msg_req_t) + 
                X2AP_MAX_ASN1_BUF_LEN);
        memset_wrapper(sig_msg_ind_buf,0,sizeof(x2ap_ue_associated_sig_msg_ind_t) + 
                X2AP_MAX_ASN1_BUF_LEN);

        /* Fill message */

        /*Fill Old eNB UE X2AP ID*/
        msg.x2ap_Old_UE_X2AP_ID = p_ue_context->x2ap_context.old_enb_ue_x2ap_id;

        /*Fill New eNB UE X2AP ID*/

#ifdef ENDC_ENABLED
        /*NR_DC Code Change Start*/
        /*
         * If UE has SCG bearer type in any eRAB the EN-DC procedure were 
         * ongoing so target node is a gNB.
         */
        if ( RRC_NULL < rrc_uecc_count_scg_bearer(p_ue_context) )
        {
            /*Fill Old eNB UE X2AP ID as Zero */
            //msg.x2ap_Old_UE_X2AP_ID = RRC_NULL; 
            msg.x2ap_Old_UE_X2AP_ID =
                p_ue_context->x2ap_context.menb_ue_x2ap_id; 

            msg.x2ap_New_UE_X2AP_ID = 
                p_ue_context->x2ap_context.menb_ue_x2ap_id;

            if(p_ue_context->x2ap_context.is_menb_ext_present)
            {
                msg.bitmask |=
                    X2AP_UE_CONTEXT_RELEASE_NEW_UE_X2AP_ID_EXT_PRESENT;
                msg.x2ap_New_UE_X2AP_ID_ext =
                    p_ue_context->x2ap_context.menb_ue_x2ap_id_ext;
            }

            /*Fill SgNB UE X2AP ID*/
            if(p_ue_context->x2ap_context.m.sgnb_ue_x2ap_id_present)
            {
                msg.bitmask |=
                    X2AP_UE_CONTEXT_RELEASE_SGNB_UE_X2AP_ID_PRESENT;
                msg.x2ap_SgNB_UE_X2AP_ID =
                    p_ue_context->x2ap_context.sgnb_ue_x2ap_id;
            }
        }
        else
        {
            /*NR_DC Code Change Stop*/
#endif
            if (p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present)
            {
                msg.x2ap_New_UE_X2AP_ID = 
                    p_ue_context->x2ap_context.new_enb_ue_x2ap_id;
            }
#ifdef ENDC_ENABLED
        }
#endif

        if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            /* Encode message */
            result=x2ap_internal_encode_ue_context_release(&asn1_ctx,
                    &p_sig_msg_req->message_buff_p[0],&p_sig_msg_req->message_buff_size,
                    &msg);
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_UEContextRelease] Build failure");
                break;
            }
            else
            {
                encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);
                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                        encoded_msg_len);
                if (PNULL != p_event_header)
                {
                    memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                                encoded_msg_len));
                    rrc_uecc_fill_protocol_event_params(p_ue_context,
                            p_event_header,
                            (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len),
                            LOCAL_UE_CONTEXT_RELEASE,
                            MESSAGE_DIRECTION_SEND);

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)&p_sig_msg_req->message_buff_p[0],
                            encoded_msg_len);

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }
            }

            p_sig_msg_req->procedure_code = ASN1V_x2ap_id_uEContextRelease;
            p_sig_msg_req->message_type = X2AP_INITIATING_MESSAGE;
            p_sig_msg_req->ue_id = p_ue_context->ue_index;

            p_sig_msg_req->bitmask |=
                X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT;

            p_sig_msg_req->old_enb_ue_x2ap_id = 
                p_ue_context->x2ap_context.old_enb_ue_x2ap_id;

            p_sig_msg_req->bitmask |= 
                X2AP_UE_ASSOC_SIG_MSG_REQ_NEW_ENB_UE_X2AP_ID_PRESENT;
            p_sig_msg_req->new_enb_ue_x2ap_id = 
                p_ue_context->x2ap_context.new_enb_ue_x2ap_id;

            /* CSR 00082153 Fix Start */
            p_sig_msg_req->peer_enodeb_id = 
                p_ue_context->x2ap_context.peer_enodeb_id;
            /* CSR 00082153 Fix Stop */

            /* TRACE_changes_start */
            uecc_x2ap_ue_assoc_msg_max_trace_handler(p_ue_context, &p_sig_msg_req->message_buff_p[0],
                                         p_sig_msg_req->message_buff_size, "UE Context Release", 1);
            /* TRACE_changes_start */

	    /* SPR_6461 Start */
            /* Check if UECC_STORE_UE_CONTEXT_TIMER is running. 
             * Start the timer if not running */
            if (PNULL == 
                    p_ue_context->timers_data .timers_ids[UECC_STORE_UE_CONTEXT_TIMER])
            {
                if ( RRC_SUCCESS != uecc_ue_timer_start(
                            p_ue_context->p_gb_context,
                            UECC_STORE_UE_CONTEXT_TIMER,
                            &p_ue_context->p_gb_context->ue_timers_data,
                            &p_ue_context->timers_data,
                            p_ue_context->ue_index))
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                            "UECC_STORE_UE_CONTEXT_TIMER start failed");
                }
            }
	    /* SPR_6461 Stop */

            /* Send message to X2AP  */
            result = uecc_rrc_send_internal_msg(
                    RRC_X2AP_MODULE_ID,
                    X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                        p_sig_msg_req->message_buff_size),
                    sig_msg_req_buf);

            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_INFO,
                    "X2AP_MSG: UE CONTEXT RELEASE %s", change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
        }
/*BUG 604 changes start*/
        else if((INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover) ||
                (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover))
/*BUG 604 changes stop*/
        {
            /* Encode message */
            result=x2ap_internal_encode_ue_context_release(&asn1_ctx,
                    &p_sig_msg_ind->message_buff_p[0],&message_buff_size,
                    &msg);
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_UEContextRelease] Build failure");
                break;
            }
/*BUG 604 changes start*/
            if(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover) 
            {
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTER_CELL_HO_PROC_CODE; 
            }
            else
            {
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTRA_CELL_HO_PROC_CODE; 
            }
/*BUG 604 changes stop*/
            p_sig_msg_ind->peer_enodeb_id = RRC_INVALID_PEER_ENODEB_ID; 
            p_sig_msg_ind->served_cell_id = p_ue_context->cell_index;

            uecc_module_id  = (U8)RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                    msg.x2ap_Old_UE_X2AP_ID);

            result = uecc_rrc_send_inter_cell_internal_msg(
                    uecc_module_id,
                    X2AP_UE_ASSOCIATED_SIG_MSG_IND,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_ind_t) +
                        message_buff_size),
                    sig_msg_ind_buf);

/*BUG 604 changes start*/
            if(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_INFO,
                        "INTER_CELL_HO: UE CONTEXT RELEASE");
            }
            else
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_INFO,
                        "INTRA_CELL_HO: UE CONTEXT RELEASE");
            }
/*BUG 604 changes stop*/

        }
#ifdef ENDC_ENABLED
        /*EN-DC_changes_start*/
        else
        {
            /* Encode message */
            result=x2ap_internal_encode_ue_context_release(&asn1_ctx,
                    &p_sig_msg_req->message_buff_p[0],&p_sig_msg_req->message_buff_size,
                    &msg);
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_UEContextRelease] Build failure");
                break;
            }
            else
            {
                encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);
                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                        encoded_msg_len);
                if (PNULL != p_event_header)
                {
                    memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                                encoded_msg_len));
                    rrc_uecc_fill_protocol_event_params(p_ue_context,
                            p_event_header,
                            (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len),
                            LOCAL_UE_CONTEXT_RELEASE,
                            MESSAGE_DIRECTION_SEND);

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)&p_sig_msg_req->message_buff_p[0],
                            encoded_msg_len);

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }
            }

            p_sig_msg_req->procedure_code = ASN1V_x2ap_id_uEContextRelease;
            p_sig_msg_req->message_type = X2AP_INITIATING_MESSAGE;
            p_sig_msg_req->ue_id = p_ue_context->ue_index;

            p_sig_msg_req->bitmask |=
                X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT;

            p_sig_msg_req->old_enb_ue_x2ap_id = 
                p_ue_context->x2ap_context.old_enb_ue_x2ap_id;

            p_sig_msg_req->bitmask |= 
                X2AP_UE_ASSOC_SIG_MSG_REQ_NEW_ENB_UE_X2AP_ID_PRESENT;
            p_sig_msg_req->new_enb_ue_x2ap_id = 
                p_ue_context->x2ap_context.new_enb_ue_x2ap_id;

            p_sig_msg_req->peer_enodeb_id = 
                p_ue_context->x2ap_context.peer_enodeb_id;

            /* TRACE_changes_start */
            uecc_x2ap_ue_assoc_msg_max_trace_handler(p_ue_context, &p_sig_msg_req->message_buff_p[0],
                    p_sig_msg_req->message_buff_size, "UE Context Release", 1);
            /* TRACE_changes_start */

            /* Check if UECC_STORE_UE_CONTEXT_TIMER is running. 
             * Start the timer if not running */
            if (PNULL == 
                    p_ue_context->timers_data .timers_ids[UECC_STORE_UE_CONTEXT_TIMER])
            {
                if ( RRC_SUCCESS != uecc_ue_timer_start(
                            p_ue_context->p_gb_context,
                            UECC_STORE_UE_CONTEXT_TIMER,
                            &p_ue_context->p_gb_context->ue_timers_data,
                            &p_ue_context->timers_data,
                            p_ue_context->ue_index))
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                            "UECC_STORE_UE_CONTEXT_TIMER start failed");
                }
            }

            /* Send message to X2AP  */
            result = uecc_rrc_send_internal_msg(
                    RRC_X2AP_MODULE_ID,
                    X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                        p_sig_msg_req->message_buff_size),
                    sig_msg_req_buf);

            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_INFO,
                    "X2AP_MSG: UE CONTEXT RELEASE %s", change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));

        }
        /*EN-DC_changes_end*/
#endif

    } while (0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return result;
}


/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_sn_status_transfer
 *
 *   Input:  
 *          uecc_ue_context_t* p_ue_context
 *          rrc_uecc_llim_sn_hfn_status_resp_t  *p_sn_hfn_status_resp
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds and sends  x2ap_SNStatusTransfer message
 *       to X2AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_build_and_send_sn_status_transfer
(
 uecc_ue_context_t*      p_ue_context,
 rrc_uecc_llim_sn_hfn_status_resp_t  *p_sn_hfn_status_resp
 )
{
    rrc_return_et result = RRC_FAILURE;
    OSCTXT asn1_ctx;
    rrc_x2ap_sn_status_transfer_t msg;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;
    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;
    U8 sig_msg_ind_buf[sizeof(x2ap_ue_associated_sig_msg_ind_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_ind_t *p_sig_msg_ind =
        (x2ap_ue_associated_sig_msg_ind_t *)sig_msg_ind_buf;
    
    rrc_module_id_t       uecc_module_id = RRC_INVALID_UECC_MODULE_ID;
    U16                   message_buff_size = RRC_NULL;
    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/
    OSRTDListNode  *p_node_info = PNULL;
    x2ap_E_RABs_SubjectToStatusTransfer_List_element *p_info = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_sn_hfn_status_resp);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (RRC_FALSE == p_ue_context->p_gb_context->mme_connection_status)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                " MME connection isn't present");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    if (0 == p_sn_hfn_status_resp->sn_hfn_status_list.count)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[x2ap_SNStatusTransfer] sn_hfn_status_list.count = 0");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }


    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[x2ap_SNStatusTransfer] ASN context initialization failed");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    do
    {
        /* Reset message */
        memset_wrapper(&msg, 0, sizeof(msg));
        memset_wrapper(sig_msg_req_buf,0,sizeof(x2ap_ue_associated_sig_msg_req_t) + 
                X2AP_MAX_ASN1_BUF_LEN);
        memset_wrapper(sig_msg_ind_buf,0,sizeof(x2ap_ue_associated_sig_msg_ind_t) + 
                X2AP_MAX_ASN1_BUF_LEN);

        /* Fill message */
        msg.x2ap_Old_UE_X2AP_ID = p_ue_context->x2ap_context.old_enb_ue_x2ap_id;
        
        if (p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present)
        {
            msg.x2ap_New_UE_X2AP_ID = 
                p_ue_context->x2ap_context.new_enb_ue_x2ap_id;
        }

        if (RRC_SUCCESS!=
                uecc_x2ap_build_erab_subject_to_status_transfer_list(
                    p_ue_context,
                    p_sn_hfn_status_resp,
                    &asn1_ctx,
                    &msg.erab_subject_to_status_transfer_list))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_SNStatusTransfer]"
                    "uecc_x2ap_build_erab_subject_to_status_transfer_list "
                    " failure");
            break;
        }

        if (X2_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
        {
            /* Encode message */
            result = x2ap_internal_encode_sn_status_transfer(
                    &asn1_ctx,
                    &p_sig_msg_req->message_buff_p[0],
                    &p_sig_msg_req->message_buff_size,
                    &msg); 
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_SNStatusTransfer] Build failure");
                break;
            }
            else
            {
                encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);
                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                        encoded_msg_len);
                if (PNULL != p_event_header)
                {
                    memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                                encoded_msg_len));
                    rrc_uecc_fill_protocol_event_params(p_ue_context,
                            p_event_header,
                            (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len),
                            LOCAL_SN_STATUS_TRANSFER,
                            MESSAGE_DIRECTION_SEND);

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)&p_sig_msg_req->message_buff_p[0],
                            encoded_msg_len);

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }
            }

            p_sig_msg_req->procedure_code = ASN1V_x2ap_id_snStatusTransfer;
            p_sig_msg_req->message_type = X2AP_INITIATING_MESSAGE;
            p_sig_msg_req->ue_id = p_ue_context->ue_index;

            p_sig_msg_req->bitmask |=
                X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT;
            p_sig_msg_req->old_enb_ue_x2ap_id = 
                p_ue_context->x2ap_context.old_enb_ue_x2ap_id;

            /* CSR 00082153 Fix Start */
            p_sig_msg_req->peer_enodeb_id = 
                p_ue_context->x2ap_context.peer_enodeb_id;
            /* CSR 00082153 Fix Stop */

        /* Is trace activated */
	    	/*SPR 15896 Fix Start*/
        if (p_ue_context->m.traceActivated)
        {
	    	/*SPR 15875 Fix Start*/
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
				    (p_ue_context->p_ue_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_MME, "\n<msg function=\"X2AP\" name=\"SN Status Transfer\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_trace_activation_info->rawtime));
		    }
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
				    (p_ue_context->p_ue_cell_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL, "\n<msg function=\"X2AP\" name=\"SN Status Transfer\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_cell_trace_activation_info->rawtime));
		    }
	    	/*SPR 15875 Fix Stop*/
            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), X2,"\n <target type=\"ENB\">%s</target>",
                p_ue_context->p_gb_context->x2ap_ctx.
                x2ap_link_info[p_ue_context->x2ap_context.peer_enodeb_id].enb_comm_info.ip_addr[0].ip_addr);

            /* ASN.1 encoded message to logged */
            rrc_trace_max_file_write(p_ue_context, X2AP_SN_STATUS_TRANSFER,
                    p_sig_msg_req->message_buff_size,
                    &p_sig_msg_req->message_buff_p[0], X2);

                P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n <ieGroup name=\"E-RABs_SubjectTo_StatusTransfer_List\">");

                for (p_node_info = msg.erab_subject_to_status_transfer_list.head; PNULL!=p_node_info;
                        p_node_info=p_node_info->next)
                {
                    p_info=(x2ap_E_RABs_SubjectToStatusTransfer_List_element*)p_node_info->data;
                    if(p_info == PNULL)
                    {
                        continue;
                    }
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n <ieGroup name=\"E-RABs_SubjectTo_StatusTransfer_Item\">");
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"E-RAB id\">%d</ie> ", p_info->value.e_RAB_ID);
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup>");
                }

                P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup>");
                P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </msg>");
	    	/*SPR 15896 Fix Stop*/
        }

            /* Send message to X2AP  */
            result = uecc_rrc_send_internal_msg(
                    RRC_X2AP_MODULE_ID,
                    X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                        p_sig_msg_req->message_buff_size),
                    sig_msg_req_buf);

            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_INFO,
                    "X2AP_MSG: SN STATUS TRANSFER %s", change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
        }
    /*BUG 604 changes start*/
        else if( (INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)||
                (INTRA_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover))
    /*BUG 604 changes stop*/
        {
            /* Encode message */
            result = x2ap_internal_encode_sn_status_transfer(
                     &asn1_ctx,
                     &p_sig_msg_ind->message_buff_p[0],
                     &message_buff_size,
                     &msg); 
            if (RRC_SUCCESS!=result)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[x2ap_SNStatusTransfer] Build failure");
                break;
            }
/*BUG 604 changes start*/
            if(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
            {
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTER_CELL_HO_PROC_CODE; 
            }
            else
            {
                p_sig_msg_ind->x2ap_procedure_code = RRC_INTRA_CELL_HO_PROC_CODE;
            }
/*BUG 604 changes stop*/
            p_sig_msg_ind->peer_enodeb_id = RRC_INVALID_PEER_ENODEB_ID; 
            p_sig_msg_ind->served_cell_id = p_ue_context->cell_index;
            
            uecc_module_id  = (U8)RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                msg.x2ap_New_UE_X2AP_ID);
            
            result = (rrc_return_et)uecc_rrc_send_inter_cell_internal_msg(
                    uecc_module_id,
                    X2AP_UE_ASSOCIATED_SIG_MSG_IND,
                    (U16)(sizeof(x2ap_ue_associated_sig_msg_ind_t) +
                        message_buff_size),
                    sig_msg_ind_buf);

/*BUG 604 changes start*/
            if(INTER_CELL_HO ==  p_ue_context->ho_info.s1_or_x2_handover)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_INFO, 
                        "INTER_CELL_HO: SN STATUS TRANSFER");
            }
            else
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_INFO, 
                        "INTRA_CELL_HO: SN STATUS TRANSFER");
            }
/*BUG 604 changes stop*/

        }
        
    } while (0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_erab_subject_to_status_transfer_list
 *
 *   Input:  
 *          uecc_ue_context_t* p_ue_context
 *          rrc_uecc_llim_sn_hfn_status_resp_t  *p_sn_hfn_status_resp
 *          OSCTXT              *p_asn1_ctx
 *          x2ap_E_RABs_SubjectToStatusTransfer_List
 *                                *p_erab_subject_to_status_transfer_list
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds x2ap_E_RABs_SubjectToStatusTransfer_List
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
rrc_return_et uecc_x2ap_build_erab_subject_to_status_transfer_list
(
    uecc_ue_context_t* p_ue_context,
    rrc_uecc_llim_sn_hfn_status_resp_t  *p_sn_hfn_status_resp,
    OSCTXT *p_asn1_ctx,
    x2ap_E_RABs_SubjectToStatusTransfer_List 
                      *p_erab_subject_to_status_transfer_list
 )    
{
    U32 erab_id =INVALID_ERAB_ID;
    U8 e_rab_index = RRC_NULL;
    rrc_counter_t count = RRC_NULL;
    rrc_bool_et  erab_id_match_flag  = RRC_FALSE;    
    OSRTDListNode* p_node = PNULL;
    x2ap_E_RABs_SubjectToStatusTransfer_List_element *p_elem = PNULL;
    uecc_e_rab_not_admitted_to_add_list_t
                    * p_uecc_e_rab_not_admitted_to_add_list = PNULL;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_sn_hfn_status_resp);
    RRC_ASSERT(PNULL != p_erab_subject_to_status_transfer_list);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    p_uecc_e_rab_not_admitted_to_add_list = &(p_ue_context->ho_info.p_ho_info->
                        p_src_ho_info->s1_src_ho_info.erab_not_admitted_list);

    asn1Init_x2ap_E_RABs_SubjectToStatusTransfer_List(
            p_erab_subject_to_status_transfer_list);

    for (e_rab_index=0; 
            e_rab_index < p_sn_hfn_status_resp->sn_hfn_status_list.count; 
            e_rab_index++)
    {
        erab_id = get_erab_id_from_lc_id(p_ue_context, 
                p_sn_hfn_status_resp->sn_hfn_status_list.
                sn_hfn_status[e_rab_index].lc_id); 

        if (INVALID_ERAB_ID <= erab_id)
        {
            continue;
        }
        erab_id_match_flag = RRC_FALSE;
        for(count = 0; count < (p_uecc_e_rab_not_admitted_to_add_list->count) 
                ; count++)
        {
            if (erab_id == p_uecc_e_rab_not_admitted_to_add_list->
                    e_rab_not_admitted_to_add_list[count].e_rab_id)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_INFO,
                        "[%s] The e_RAB_ID[%d] is not admitted at target side " 
                        "so not sending sn-hfn status for this e_RAB_ID "
                        ,__FUNCTION__,erab_id);
                erab_id_match_flag = RRC_TRUE;
                break;
            }
        }
        if(RRC_TRUE == erab_id_match_flag)
        {
            continue;
        }
            
        rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_E_RABs_SubjectToStatusTransfer_List_element,
                &p_node,
                &p_elem);

        if (PNULL == p_node)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_FATAL,
                    "Can't allocate p_node");
            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return RRC_FAILURE;
        }

        asn1Init_x2ap_E_RABs_SubjectToStatusTransfer_List_element(p_elem);
        asn1Init_x2ap_E_RABs_SubjectToStatusTransfer_Item(&p_elem->value);

        p_elem->id = ASN1V_x2ap_id_E_RABs_SubjectToStatusTransfer_Item;
        p_elem->criticality = x2ap_ignore;

        p_elem->value.e_RAB_ID = erab_id;

        if (UECC_LLIM_RECEIVE_STATUS_OF_UL_PDCP_SDU_PRESENT &
                p_sn_hfn_status_resp->
                sn_hfn_status_list.sn_hfn_status[e_rab_index].bitmask)
        {
            p_elem->value.receiveStatusofULPDCPSDUs.numbits 
                = MAX_UL_PDCP_SDU * 8;

            l3_memcpy_wrapper(((void*)p_elem->value.receiveStatusofULPDCPSDUs.data),
                    ((const void *)p_sn_hfn_status_resp->sn_hfn_status_list.
                     sn_hfn_status[e_rab_index].
                     status_of_ul_pdcp_sdu.receive_status_of_ul_pdcp_sdu),
                    MAX_UL_PDCP_SDU);

            p_elem->value.m.receiveStatusofULPDCPSDUsPresent = RRC_TRUE;
        }

        p_elem->value.uL_COUNTvalue.pDCP_SN = 
            p_sn_hfn_status_resp->sn_hfn_status_list.sn_hfn_status[e_rab_index].
            ul_count_value.sn_count;
        p_elem->value.uL_COUNTvalue.hFN = 
            p_sn_hfn_status_resp->sn_hfn_status_list.sn_hfn_status[e_rab_index].
            ul_count_value.hfn_count;


        p_elem->value.dL_COUNTvalue.pDCP_SN = 
            p_sn_hfn_status_resp->sn_hfn_status_list.sn_hfn_status[e_rab_index].
            dl_count_value.sn_count;
        p_elem->value.dL_COUNTvalue.hFN = 
            p_sn_hfn_status_resp->sn_hfn_status_list.sn_hfn_status[e_rab_index].
            dl_count_value.hfn_count;

        rtxDListAppendNode(
                p_erab_subject_to_status_transfer_list, 
                p_node);
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_asn1PrtToStr_X2AP_PDU
*
 *   Input:  
 *          U32 log_level
 *          const char* name
 *           x2ap_X2AP_PDU* pvalue
 *   Outputs : None
*   DESCRIPTION:
*       This function prints ASN X2AP_PDUs using RRC logging mechanism.
*
*   RETURNS:
*       void
*
*****************************************************************************/
void rrc_asn1PrtToStr_X2AP_PDU(
                            U32 log_level, 
                            const char* name, 
                            x2ap_X2AP_PDU* pvalue)
{
    int rrc_result =0;

/* SPR 16017 Fix Start */ 
    if(RRC_SUCCESS == rrc_check_logger_type(log_level))    
/* SPR 16017 Fix Stop */
    {
        if((name != NULL) && (pvalue != NULL))
        {
            S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);

            if(pBuff != NULL)
            {
                memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);

                /* Converted to char* as required by ASN function */
                rrc_result = asn1PrtToStr_x2ap_X2AP_PDU(
                                                        name, 
                                                        pvalue, 
                                                        (char*)pBuff, 
                                                        MAX_ASN_PRINT_BUFF_SIZE);  

                if(rrc_result == 0)
                {/* SPR 13700 Fix start */
                    RRC_TRACE_ASN((S32)log_level, "%s\n", pBuff);/* SPR 13700 Fix stop */
                }
                else
                {
                    RRC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be increased");
                }
                rrc_mem_free(pBuff);
            }
        }
        else
        {
            RRC_TRACE(RRC_WARNING, "x2ap_X2AP_PDU Info to be printed is missing");
        }
    }
    //RRC_LOG_ASN_HEX_DUMP(name, msgName, p_buff, msgLen);
}


/******************************************************************************
  *   FUNCTION NAME: rrc_asn1PrtToStr_handover_prep
  *
 *   Input:  
 *          U32                 log_level
 *          const char*         name
 *          HandoverPreparationInformation *pvalue
 *   Outputs : None
  *   DESCRIPTION:
  *       This function prints ASN X2AP_PDUs using RRC logging mechanism.
  *
  *   RETURNS:
  *       void
  *
  *****************************************************************************/
void rrc_asn1PrtToStr_handover_preparation(
        U32                 log_level,
        const char*         name,
        HandoverPreparationInformation* pvalue,
        const char*      msgName,
        U8 *             p_buff,
        U16              msgLen) 
{
    int rrc_result =0;

    if (rrc_get_loglevel() >= log_level)
    {
        if((name != NULL) && (pvalue != NULL))
        {
            S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);
    
            if(pBuff != NULL)
            {
                memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);
    
                rrc_result = 
                    asn1PrtToStr_HandoverPreparationInformation(
                            name,
                            pvalue,
                            (char*)pBuff,
                            MAX_ASN_PRINT_BUFF_SIZE);
    
                if(rrc_result == 0)
                {/* SPR 13700 Fix start */
                    RRC_TRACE_ASN((S32)log_level,"%s\n",pBuff);/* SPR 13700 Fix stop */
                }
                else
                {
                    RRC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be"
                            "increased");
                }
                rrc_mem_free(pBuff);
            }
        }
        else
        {
            RRC_TRACE(RRC_WARNING, "x2ap_X2AP_PDU Info"
                    "to be printed is missing");
        }
    }
    RRC_LOG_ASN_HEX_DUMP(name, msgName, p_buff, msgLen);
}

/******************************************************************************
  *   FUNCTION NAME: rrc_asn1PrtToStr_handover_command
  *
 *   Input:  
 *          U32 log_level
 *          const char* name
 *          HandoverCommand* pvalue
 *   Outputs : None
  *   DESCRIPTION:
  *       This function prints ASN X2AP_PDUs using RRC logging mechanism.
  *
  *   RETURNS:
  *       void
  *
  *****************************************************************************/
void rrc_asn1PrtToStr_handover_command(
        U32 log_level,
        const char* name,
        HandoverCommand* pvalue,
        const char*      msgName,
        U8 *             p_buff,
        U16              msgLen) 
{
    int rrc_result =0;

    if (rrc_get_loglevel() >= log_level)
    {
        if((name != NULL) && (pvalue != NULL))
        {
            S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);

            if(pBuff != NULL)
            {
                memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);

                rrc_result = 
                    asn1PrtToStr_HandoverCommand(
                            name,
                            pvalue,
                            (char*)pBuff,
                            MAX_ASN_PRINT_BUFF_SIZE);

                if(rrc_result == 0)
                {/* SPR 13700 Fix start */
                    RRC_TRACE_ASN((S32)log_level,"%s\n",pBuff);/* SPR 13700 Fix stop */
                }
                else
                {
                    RRC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be"
                            "increased");
                }
                rrc_mem_free(pBuff);
            }
        }
        else
        {
            RRC_TRACE(RRC_WARNING, "x2ap_X2AP_PDU Info"
                    "to be printed is missing");
        }
    }
    RRC_LOG_ASN_HEX_DUMP(name, msgName, p_buff, msgLen);
}


/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_ue_release_ind
 *
 *   Input:  
 *          uecc_ue_context_t* p_ue_context
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds and sends x2ap_ue_release_ind message
 *       to X2AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_build_and_send_ue_release_ind
(
    uecc_ue_context_t                *p_ue_context
)
{
    rrc_return_et result = RRC_FAILURE;
    x2ap_ue_release_ind_t x2ap_ue_release_ind;

    
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    memset_wrapper(&x2ap_ue_release_ind, RRC_NULL, sizeof(x2ap_ue_release_ind_t));

    /* Reset message */
    memset_wrapper(&x2ap_ue_release_ind, 0, sizeof(x2ap_ue_release_ind_t));

    /* Fill message */
    /*Fill UE ID*/
    x2ap_ue_release_ind.ue_id = p_ue_context->ue_index;
    x2ap_ue_release_ind.peer_enodeb_id = 
        p_ue_context->x2ap_context.peer_enodeb_id;

    /* Send message to X2AP  */
    if(RRC_SUCCESS == uecc_rrc_send_internal_msg(
            RRC_X2AP_MODULE_ID,
            X2AP_UE_RELEASE_IND,
            (U16)(sizeof(x2ap_ue_release_ind_t)),
            &x2ap_ue_release_ind))
    {
       result = RRC_SUCCESS;
    }

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
            p_ue_context->p_gb_context->facility_name, 
            RRC_INFO, "sending UE RELEASE IND success.");


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_reset_msg_ind
 *
 *   Input: 
 *          void               *p_api 
 *          uecc_ue_context_t* p_ue_context
 *   Outputs : None
 *   DESCRIPTION:
 *       This function processes X2AP_RESET_MSG_IND
 *
 *   RETURNS:
 *       None
 ******************************************************************************/
void uecc_x2ap_process_reset_msg_ind(
        void *p_api,
        uecc_gb_context_t *p_uecc_gb_context
        )
{
	/* coverity_fix_54699_start */
	x2ap_reset_msg_ind_t *p_x2ap_reset_msg_ind = PNULL;
	U16 buf_size = 0;
	U16 index_counter = RRC_NULL;
	U16 ue_count =0;
    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/

	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

	RRC_ASSERT(PNULL!=p_api);
	RRC_ASSERT(PNULL!=p_uecc_gb_context);

    /* SPR 21803 Fix Start */
    if(RRC_FAILURE == uecc_check_x2ap_message(p_uecc_gb_context))
    {
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }
    /* SPR 21803 Fix Stop */

	p_x2ap_reset_msg_ind =
		(x2ap_reset_msg_ind_t *)((U8*)p_api + RRC_API_HEADER_SIZE);

	buf_size = rrc_get_api_buf_size(p_api);
	/* Precondition - p_api should point to right message */
	if (buf_size <
			RRC_API_HEADER_SIZE +
			sizeof(x2ap_reset_msg_ind_t))
	{
		RRC_UECC_TRACE(p_uecc_gb_context->context_index,
				p_uecc_gb_context->facility_name,
				RRC_WARNING,
				"[X2AP_RESET_MSG_IND] "
				"Wrong message size - ignore.");

		RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
		return;
	}

	if(MAX_PEER_ENB == p_x2ap_reset_msg_ind->peer_enodeb_id)
	{
		RRC_UECC_TRACE(p_uecc_gb_context->context_index,
				p_uecc_gb_context->facility_name,
				RRC_ERROR,"Peer EnodeB Id received from X2AP is invalid"
				":%u", p_x2ap_reset_msg_ind->peer_enodeb_id);
	}

	for (index_counter = 0;
			index_counter < p_uecc_gb_context->total_ue_supported;
			index_counter++)
	{
		if (PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter])
		{
			if(p_uecc_gb_context->ue_db.ue_contexts[index_counter]->x2ap_context.peer_enodeb_id 
					== p_x2ap_reset_msg_ind->peer_enodeb_id)
			{
				/*Send the Event to the UECC main FSM */
				uecc_fsm_process_event(
						p_uecc_gb_context->ue_db.ue_contexts[index_counter],
						UECC_EV_X2AP_RESET_MSG_IND,
						p_x2ap_reset_msg_ind);
				ue_count++;
			}
		}
	}
	RRC_UECC_TRACE(p_uecc_gb_context->context_index,
			p_uecc_gb_context->facility_name,
			RRC_INFO,"Stopping handover over X2 interface for %u UEs.", ue_count);

	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
	return;
}
/* coverity_fix_54699_stop */


/****************************************************************************
 * Function Name  : uecc_x2ap_encode_RLF_Report
 * Inputs         : uecc_ue_context_t,
 *                  p_buff : message buffer
 *                  p_buff_size : message buffer length
 * Outputs        : none
 * Returns        : rrc_return_et
 * Description    : This function encodes RLF Report IE.
 ***************************************************************************/
rrc_return_et uecc_x2ap_encode_RLF_Report(
        uecc_ue_context_t  *p_ue_context,
        U8                 *p_buff,
        U16                *p_buff_size)
{
    rrc_return_et  result = RRC_FAILURE;
    OSCTXT         asn1_ctx;

    RRC_ASSERT(PNULL != p_ue_context);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "[ uecc_x2ap_encode_RLF_Report] ASN context initialization failed");
        return RRC_FAILURE;
    }

    do
    {
        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_RRC_BUF_TYPE);

        if (RT_OK != asn1PE_RLF_Report_r9(&asn1_ctx, 
                        &p_ue_context->rlf_proc_flags.rlf_report))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "ASN1 encoding for RLF_Report_r9 failed.");
            break;
        }

        *p_buff_size = (U16)pe_GetMsgLen(&asn1_ctx);
        result = RRC_SUCCESS;
    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    return result;;
}

/*SPR_18125_START*/
/****************************************************************************
 * Function Name  : uecc_x2ap_decode_RLF_Report
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  uecc_gb_context_t,
 *                  p_container
 *                  p_buff
 *                  buff_size
 * Outputs        : none
 * Returns        : rrc_return_et
 * Description    : This function decodes RLF Report IE.
 ***************************************************************************/
rrc_return_et uecc_x2ap_decode_RLF_Report(
        OSCTXT             *p_asn1_ctx,
        uecc_gb_context_t  *p_uecc_gb_context,
        RLF_Report_r9      *p_container,
        U8                 *p_buff,
        U16                buff_size)
{
    rrc_return_et result = RRC_SUCCESS;

    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_container);

    do
    {
        pu_setBuffer(p_asn1_ctx, p_buff, buff_size, ASN_RRC_BUF_TYPE);
        if ( RT_OK != asn1PD_RLF_Report_r9(
                        p_asn1_ctx, p_container))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name, RRC_ERROR,
                    "ASN1 decode for RLF_Report_r9 failed.");
            result = RRC_FAILURE;
            break;
        }
    }while(0);
    return result;
}
/*SPR_18125_END*/

/****************************************************************************
 * Function Name  : x2ap_build_and_send_reestablishment_ind
 * Inputs         : uecc_ue_context_t
 * Outputs        : None
 * Returns        : rrc_return_et
 * Description    : This function  build and send x2ap reestab_conn_setup_reconfig
 *                  to x2ap module.
 ***************************************************************************/
rrc_return_et x2ap_build_and_send_reestablishment_ind(
                uecc_ue_context_t *p_ue_context)
{
    U8 msg_ind_buf[sizeof(x2ap_ue_reestab_conn_setup_reconfig_t)+X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_reestab_conn_setup_reconfig_t *p_reestab_conn_setup_reconfig = 
        (x2ap_ue_reestab_conn_setup_reconfig_t *)msg_ind_buf;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_ue_context);

    memset_wrapper(msg_ind_buf, 0, sizeof(x2ap_ue_reestab_conn_setup_reconfig_t)+X2AP_MAX_ASN1_BUF_LEN);

    if (p_ue_context->p_reestab_ue_identity != PNULL)
    {
        /* Filling reestab_conn_setup_reconfig message */
        p_reestab_conn_setup_reconfig->bitmask = 0;
        p_reestab_conn_setup_reconfig->cell_index = p_ue_context->cell_index;
        p_reestab_conn_setup_reconfig->crnti      = p_ue_context->p_reestab_ue_identity->crnti;
        p_reestab_conn_setup_reconfig->pci        = p_ue_context->p_reestab_ue_identity->pci;
        p_reestab_conn_setup_reconfig->short_mac_i = 
                                p_ue_context->p_reestab_ue_identity->short_mac_i;

        if(p_ue_context->rlf_proc_flags.m.rlf_report_present == 1)
        {
            p_reestab_conn_setup_reconfig->bitmask |= X2AP_RLF_REPORT_CONTAINER_PRESENT;

            if (RRC_FAILURE == uecc_x2ap_encode_RLF_Report(p_ue_context,
                    &p_reestab_conn_setup_reconfig->rlf_report.message_buff_p[0],
                    &p_reestab_conn_setup_reconfig->rlf_report.message_buff_size))
            {
                p_reestab_conn_setup_reconfig->bitmask ^= X2AP_RLF_REPORT_CONTAINER_PRESENT;
            }
        }

        /* Send Reestab Ind to X2AP Module */
        if(RRC_FAILURE ==  uecc_rrc_send_internal_msg(
                    RRC_X2AP_MODULE_ID,
                    X2AP_UE_REESTABLISHMENT_CONN_SETUP_RECONFIGURE,
                    (U16)(sizeof(x2ap_ue_reestab_conn_setup_reconfig_t) +
                        p_reestab_conn_setup_reconfig->rlf_report.message_buff_size),
                    msg_ind_buf))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,
                        RRC_ERROR,
                        "Sending X2AP_UE_REESTABLISHMENT_IND failed.");
            return RRC_FAILURE;
        }
    }
    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,
            RRC_ERROR, "reestab_ue_identity info is null.");
        return RRC_FAILURE;
    }

    /* SPR 17007 Fix Start */
    /* Deallocate memory previously allocated for
      RRCConnectionReestablishmentReq only if Re-establishment is not ongoing*/
    if ((PNULL != p_ue_context->p_reestab_ue_identity) &&
           (strcmp_wrapper(p_ue_context->uecc_fsm.p_current_state->s_name,
              (const S8 *)"UECC_UE_CONNECTION_RE_ESTABLISHMENT_ONGOING"))) 
    /* SPR 17007 Fix Stop */
    {
        rrc_mem_free(p_ue_context->p_reestab_ue_identity);
        p_ue_context->p_reestab_ue_identity = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}
/*CA_PHASE3_INTER_CELL_MRO_START*/
/****************************************************************************
 * Function Name  : x2ap_build_and_send_rlf_ind
 * Inputs         : uecc_ue_context_t   *p_ue_context
 *                  uecc_gb_context_t   *p_uecc_gb_context
 *                  Pointe to ue context and ue global context
 * Outputs        : None
 * Returns        : rrc_return_et
 * Description    : This function  build and send x2ap_ue_rlf_resp to
 *                  to x2ap module.
 ***************************************************************************/
rrc_return_et x2ap_build_and_send_rlf_ind(
        uecc_ue_context_t   *p_ue_context, /*UECC UE Context*/
        uecc_gb_context_t   *p_uecc_gb_context /*UECC Global Context*/
)
{
    /* + Coverity 63620 */
    static x2ap_ue_rlf_resp_t rlf_resp;/*Internal API that is to be sent to x2ap module*/
    /* - Coverity 63620 */

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_uecc_gb_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    memset_wrapper(&rlf_resp, 0, sizeof(x2ap_ue_rlf_resp_t));

    if (PNULL != p_ue_context->p_reestab_ue_identity)
    {
        rlf_resp.rlf_ind_flag = RRC_TRUE;
        rlf_resp.rlf_ind.too_late_inter_cell_ho_flg = RRC_TRUE;
        rlf_resp.ho_report_flag = RRC_FALSE;
        rlf_resp.rlf_ind.failure_cell_pci = 
            p_ue_context->p_reestab_ue_identity->pci;
        rlf_resp.rlf_ind.crnti = 
            p_ue_context->p_reestab_ue_identity->crnti;
        rlf_resp.rlf_ind.bitmask |= X2AP_SHORT_MAC_I_PRESENT;
        rlf_resp.rlf_ind.short_mac_i = 
            p_ue_context->p_reestab_ue_identity->short_mac_i;
        rlf_resp.cell_index = p_ue_context->cell_index;
        rlf_resp.ue_index = p_ue_context->ue_index;

        if (RRC_TRUE == p_ue_context->rlf_proc_flags.m.rlf_report_present)
        {
            rlf_resp.rlf_ind.bitmask |= X2AP_RLF_REPORT_PRESENT;

            if (RRC_SUCCESS != uecc_x2ap_fill_rlf_report(
                        p_uecc_gb_context,
                        p_ue_context,
                        &p_ue_context->rlf_proc_flags.rlf_report,
                        &rlf_resp.rlf_report))
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,
                        RRC_ERROR,
                        "uecc_x2ap_fill_rlf_report failed.");
                rlf_resp.rlf_ind.bitmask ^= X2AP_RLF_REPORT_PRESENT;
            }
        }
        if(RRC_SUCCESS !=  uecc_rrc_send_internal_msg(
                    RRC_X2AP_MODULE_ID,
                    X2AP_UE_RLF_RESP,
                    (U16)(sizeof(x2ap_ue_rlf_resp_t)),
                    &rlf_resp))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,
                    "Sending X2AP_UE_RLF_RESP failed.");
        }
        else
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_INFO, "[X2AP_UE_RLF_RESP] sent to x2ap module");
        }
    }
    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_ERROR, "reestab_ue_identity info is null.");
        return RRC_FAILURE;
    }

    /* Deallocate memory previously allocated for
       RRCConnectionReestablishmentReq */
    if (PNULL != p_ue_context->p_reestab_ue_identity)
    {
        rrc_mem_free(p_ue_context->p_reestab_ue_identity);
        p_ue_context->p_reestab_ue_identity = PNULL;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}
/*CA_PHASE3_INTER_CELL_MRO_STOP*/

/****************************************************************************
 * Function Name  : uecc_x2ap_fill_rlf_report
 * Inputs         : uecc_gb_context_t
 *                  uecc_ue_context_t
 *                  RLF_Report_r9
 *                  rrc_rlf_report_t
 * Outputs        : None
 * Returns        : void
 * Description    : This function  
 ***************************************************************************/
rrc_return_et uecc_x2ap_fill_rlf_report(
                        uecc_gb_context_t *p_uecc_gb_context,
                        uecc_ue_context_t *p_ue_context,
                        RLF_Report_r9 *p_rlfReport_r9,
                        rrc_rlf_report_t *p_x2ap_rlf_report)
{
    rrc_return_et result = RRC_SUCCESS;
    OSRTDListNode*      p_node = PNULL;
    OSRTDListNode*      p_node1 = PNULL;
    U8 i = 0,j=0,list_count =0;
    MeasResult2EUTRA_r9 *p_MeasResult2EUTRA_r9;
    MeasResult2UTRA_r9  *p_MeasResult2UTRA_r9;
    MeasResult2CDMA2000_r9  *p_meas_cdma;
    /*CR 764 changes start*/
    U8 length = RRC_NULL;
    /*CR 764 changes stop*/
    

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_rlfReport_r9);

    do
    {
    p_x2ap_rlf_report->meas_result_serv_cell.rsrp_result = 
        p_rlfReport_r9->measResultLastServCell_r9.rsrpResult_r9;
    if(1 == p_rlfReport_r9->measResultLastServCell_r9.m.rsrqResult_r9Present)
    {
        p_x2ap_rlf_report->meas_result_serv_cell.bitmask |=
            RRC_UE_INFO_MEAS_RESULT_RSRQ_PRESENT;

        p_x2ap_rlf_report->meas_result_serv_cell.rsrq_result =
            p_rlfReport_r9->measResultLastServCell_r9.rsrqResult_r9;
    }
    if(1 == p_rlfReport_r9->m.measResultNeighCells_r9Present)
    {
        p_x2ap_rlf_report->bitmask |=
            RRC_UE_INFO_MEAS_RESULT_NEIGH_CELLS_PRESENT;

        if(1 == p_rlfReport_r9->measResultNeighCells_r9.m.measResultListEUTRA_r9Present)
        {
            p_x2ap_rlf_report->meas_result_neigh_cell.bitmask |=
                RRC_UE_INFO_MEAS_RESULT_LIST_EUTRA_PRESENT;

            p_x2ap_rlf_report->meas_result_neigh_cell.meas_result_list_eutra.count = 
                (U8)p_rlfReport_r9->measResultNeighCells_r9.measResultListEUTRA_r9.count;

            p_node = p_rlfReport_r9->measResultNeighCells_r9.measResultListEUTRA_r9.head;

            if ((p_rlfReport_r9->measResultNeighCells_r9.
                        measResultListEUTRA_r9.count >
                        ARRSIZE(p_x2ap_rlf_report->
                            meas_result_neigh_cell.
                            meas_result_list_eutra.meas_result_eutra)) ||
                    (p_rlfReport_r9->measResultNeighCells_r9.
                     measResultListEUTRA_r9.count <1))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_WARNING,
                        "uecc_rrm_build_and_send_ue_information_resp wrong list count [%u] \
                        for MeasResultList2EUTRA_r9",
                        p_rlfReport_r9->measResultNeighCells_r9.measResultListEUTRA_r9.count);
                break;
            }    
            for(i = 0; i< p_rlfReport_r9->
                    measResultNeighCells_r9.measResultListEUTRA_r9.count; i++)
            {
                RRC_ASSERT(PNULL != p_node);
                RRC_ASSERT(PNULL != p_node->data);

                p_MeasResult2EUTRA_r9 = (MeasResult2EUTRA_r9*)p_node->data;
                p_x2ap_rlf_report->meas_result_neigh_cell.meas_result_list_eutra.
                    meas_result_eutra[i].carrier_freq = p_MeasResult2EUTRA_r9->carrierFreq_r9;

                p_x2ap_rlf_report->meas_result_neigh_cell.meas_result_list_eutra.
                    meas_result_eutra[i].meas_result_list_eutra.count = 
                    (U8)p_MeasResult2EUTRA_r9->measResultList_r9.count;
                p_node1 = p_MeasResult2EUTRA_r9->measResultList_r9.head; 

                if ((p_MeasResult2EUTRA_r9->measResultList_r9.count >
                            ARRSIZE(p_x2ap_rlf_report->
                                meas_result_neigh_cell.
                                meas_result_list_eutra.meas_result_eutra[i].meas_result_list_eutra
                                .meas_result_eutra)) ||
                        (p_MeasResult2EUTRA_r9->measResultList_r9.count <1))
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,
                            "uecc_rrm_build_and_send_ue_information_resp wrong list count [%u] \
                            for MeasResultEUTRA",
                            p_MeasResult2EUTRA_r9->measResultList_r9.count);
                    break;
                }    
                for(j = 0; j < p_MeasResult2EUTRA_r9->measResultList_r9.count; j++)
                {
                    RRC_ASSERT(PNULL != p_node1);
                    RRC_ASSERT(PNULL != p_node1->data);
                    result = uecc_rrm_build_meas_result_eutra(p_uecc_gb_context,
                            p_ue_context,
                            &p_x2ap_rlf_report->
                            meas_result_neigh_cell.meas_result_list_eutra.
                            meas_result_eutra[i].meas_result_list_eutra.meas_result_eutra[j],
                            p_node1->data); 

                    if (RRC_FAILURE == result)
                    {   
                        break;
                    }    
                    p_node1 = p_node1->next;
                }
                p_node = p_node->next;
            }

        }

        if(1 == p_rlfReport_r9->measResultNeighCells_r9.m.measResultListUTRA_r9Present)
        {
            p_x2ap_rlf_report->meas_result_neigh_cell.bitmask |=
                RRC_UE_INFO_MEAS_RESULT_LIST_UTRA_PRESENT;

            p_x2ap_rlf_report->meas_result_neigh_cell.meas_result_list_utra.count = 
                (U8)p_rlfReport_r9->measResultNeighCells_r9.measResultListUTRA_r9.count;

            p_node = p_rlfReport_r9->measResultNeighCells_r9.measResultListUTRA_r9.head;

            if ((p_rlfReport_r9->measResultNeighCells_r9.
                        measResultListUTRA_r9.count >
                        ARRSIZE(p_x2ap_rlf_report->
                            meas_result_neigh_cell.
                            meas_result_list_utra.meas_result_utra)) ||
                    (p_rlfReport_r9->measResultNeighCells_r9.
                     measResultListUTRA_r9.count <1))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_WARNING,
                        "uecc_rrm_build_and_send_ue_information_resp wrong list count [%u] \
                        for MeasResultList2UTRA_r9",
                        p_rlfReport_r9->measResultNeighCells_r9.measResultListUTRA_r9.count);
                break;
            }    
            for(i = 0; i< p_rlfReport_r9->
                    measResultNeighCells_r9.measResultListUTRA_r9.count; i++)
            {
                RRC_ASSERT(PNULL != p_node);
                RRC_ASSERT(PNULL != p_node->data);

                p_MeasResult2UTRA_r9 = (MeasResult2UTRA_r9*)p_node->data;
                p_x2ap_rlf_report->meas_result_neigh_cell.meas_result_list_utra.
                    meas_result_utra[i].carrier_freq = p_MeasResult2UTRA_r9->carrierFreq_r9;

                p_x2ap_rlf_report->meas_result_neigh_cell.meas_result_list_utra.
                    meas_result_utra[i].meas_result_list_utra.count = 
                    (U8)p_MeasResult2UTRA_r9->measResultList_r9.count;
                p_node1 = p_MeasResult2UTRA_r9->measResultList_r9.head;

                if ((p_MeasResult2UTRA_r9->measResultList_r9.count >
                            ARRSIZE(p_x2ap_rlf_report->
                                meas_result_neigh_cell.
                                meas_result_list_utra.meas_result_utra[i].meas_result_list_utra.
                                meas_result_utra)) ||
                        (p_MeasResult2UTRA_r9->measResultList_r9.count <1))
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,
                            "uecc_rrm_build_and_send_ue_information_resp wrong list count [%u] \
                            for MeasResultUTRA",
                            p_MeasResult2UTRA_r9->measResultList_r9.count);
                    break;
                }    
                for(j = 0; j < p_MeasResult2UTRA_r9->measResultList_r9.count; j++)
                {
                    RRC_ASSERT(PNULL != p_node1);
                    RRC_ASSERT(PNULL != p_node1->data);
                    result =  uecc_rrm_build_meas_result_utra(
                            &p_x2ap_rlf_report->
                            meas_result_neigh_cell.meas_result_list_utra.meas_result_utra[i].
                            meas_result_list_utra.meas_result_utra[j],
                            p_node1->data,
                            p_ue_context,
                            p_uecc_gb_context);

                    if (RRC_FAILURE == result)
                    {
                        break;
                    }    
                    p_node1 = p_node1->next;
                }
                p_node = p_node->next;
            }
        }

        if(1 == p_rlfReport_r9->measResultNeighCells_r9.m.measResultListGERAN_r9Present)
        {

            p_x2ap_rlf_report->meas_result_neigh_cell.bitmask |=
                RRM_MEAS_RESULT_LIST_GERAN_PRESENT;

            p_x2ap_rlf_report->meas_result_neigh_cell.
                meas_result_list_geran.count=
                (U8)(p_rlfReport_r9->measResultNeighCells_r9.
                        measResultListGERAN_r9.count);


            p_node = p_rlfReport_r9->measResultNeighCells_r9.
                measResultListGERAN_r9.head;
            if ((p_rlfReport_r9->measResultNeighCells_r9.
                        measResultListGERAN_r9.count >
                        ARRSIZE(p_x2ap_rlf_report->
                            meas_result_neigh_cell.
                            meas_result_list_geran.meas_result_geran)) ||
                    (p_rlfReport_r9->measResultNeighCells_r9.
                     measResultListGERAN_r9.count <1))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_WARNING,
                        "uecc_rrm_build_meas_results_geran wrong list count [%u]",
                        p_rlfReport_r9->measResultNeighCells_r9.measResultListGERAN_r9.count);
                break;
            }    
            for (list_count = 0; list_count < p_rlfReport_r9->
                    measResultNeighCells_r9.measResultListGERAN_r9.count; list_count++)
            {
                RRC_ASSERT(PNULL != p_node);
                RRC_ASSERT(PNULL != p_node->data);

                result = uecc_rrm_build_meas_result_geran(
                        p_ue_context,
                        &p_x2ap_rlf_report->meas_result_neigh_cell.
                        meas_result_list_geran.
                        meas_result_geran[list_count],
                        p_node->data);
                if (RRC_FAILURE == result)
                {
                    break;
                }    
                p_node = p_node->next;
            }    
        }

        if(1 == p_rlfReport_r9->measResultNeighCells_r9.m.measResultsCDMA2000_r9Present)
        {

            p_x2ap_rlf_report->meas_result_neigh_cell.bitmask |=
                RRC_UE_INFO_MEAS_RESULT_LIST_CDMA2000_PRESENT;

            p_x2ap_rlf_report->meas_result_neigh_cell.meas_result_list2_cdma2000.count = 
                (U8)p_rlfReport_r9->measResultNeighCells_r9.measResultsCDMA2000_r9.count;

            p_node = p_rlfReport_r9->measResultNeighCells_r9.measResultsCDMA2000_r9.head;

            if ((p_rlfReport_r9->measResultNeighCells_r9.
                        measResultsCDMA2000_r9.count >
                        ARRSIZE(p_x2ap_rlf_report->
                            meas_result_neigh_cell.
                            meas_result_list2_cdma2000.meas_result_cdma2000)) ||
                    (p_rlfReport_r9->measResultNeighCells_r9.
                     measResultsCDMA2000_r9.count <1))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_WARNING,
                        "uecc_rrm_build_and_send_ue_information_resp wrong list count [%u] \
                        for MeasResultList2CDMA2000",
                        p_rlfReport_r9->measResultNeighCells_r9.measResultsCDMA2000_r9.count);
                break;
            }    
            for(i = 0; i< p_rlfReport_r9->
                    measResultNeighCells_r9.measResultsCDMA2000_r9.count; i++)
            {
                RRC_ASSERT(PNULL != p_node);
                RRC_ASSERT(PNULL != p_node->data);

                p_meas_cdma = (MeasResult2CDMA2000_r9*)p_node->data;

                p_x2ap_rlf_report->meas_result_neigh_cell.meas_result_list2_cdma2000.
                    meas_result_cdma2000[i].carrier_freq_cdma2000.band_class = 
                    (U8)p_meas_cdma->carrierFreq_r9.bandClass;

                p_x2ap_rlf_report->meas_result_neigh_cell.meas_result_list2_cdma2000.
                    meas_result_cdma2000[i].carrier_freq_cdma2000.arfcn = p_meas_cdma->carrierFreq_r9.arfcn;

                result = uecc_rrm_build_meas_results_cdma2000(
                        &p_x2ap_rlf_report->
                        meas_result_neigh_cell.meas_result_list2_cdma2000.meas_result_cdma2000[i].
                        meas_result_cdma2000,
                        &(p_meas_cdma->measResultList_r9),
                        p_ue_context,
                        p_uecc_gb_context);

                if (RRC_FAILURE == result)
                {
                    break;
                }    

                p_node = p_node->next;
            }
        }
    }
    if(1 == p_rlfReport_r9->m._v2ExtPresent)
    {    
        if(1 == p_rlfReport_r9->m.reestablishmentCellId_r10Present)
        {
             p_x2ap_rlf_report->bitmask |=
                             RRC_UE_INFO_REESTABLISMENT_CELLID_R10_PRESENT;
	     if ( p_rlfReport_r9->reestablishmentCellId_r10.plmn_Identity.m.mccPresent)
	     {
		     p_x2ap_rlf_report->reestablishment_cellid_r10.plmn_identity.presence_bitmask |=  PLMN_IDENTITY_MCC_PRESENCE_FLAG;
		     l3_memcpy_wrapper(p_x2ap_rlf_report->reestablishment_cellid_r10.plmn_identity.mcc, 
				     p_rlfReport_r9->reestablishmentCellId_r10.plmn_Identity.mcc.elem,
				     p_rlfReport_r9->reestablishmentCellId_r10.plmn_Identity.mcc.n); 
	     }
	     p_x2ap_rlf_report->reestablishment_cellid_r10.plmn_identity.mnc.count = p_rlfReport_r9->reestablishmentCellId_r10.plmn_Identity.mnc.n;

	     l3_memcpy_wrapper(p_x2ap_rlf_report->reestablishment_cellid_r10.plmn_identity.mnc.mnc,
			     p_rlfReport_r9->reestablishmentCellId_r10.plmn_Identity.mnc.elem,
			     p_rlfReport_r9->reestablishmentCellId_r10.plmn_Identity.mnc.n); 
         
         if (!(p_rlfReport_r9->reestablishmentCellId_r10.cellIdentity.numbits % RRC_OCTET_SIZE))
         {
            length = (U8)(p_rlfReport_r9->reestablishmentCellId_r10.
                cellIdentity.numbits/RRC_OCTET_SIZE);
         }
         else
         {
            length = (U8)((p_rlfReport_r9->reestablishmentCellId_r10.
                cellIdentity.numbits/RRC_OCTET_SIZE) + 1);
         }
	     
         l3_memcpy_wrapper( p_x2ap_rlf_report->reestablishment_cellid_r10.cell_identity, 
			     p_rlfReport_r9->reestablishmentCellId_r10.cellIdentity.data, 
			     length ); 
        }
        
        if(1 == p_rlfReport_r9->m.timeConnFailure_r10Present)
        {
            p_x2ap_rlf_report->bitmask |=
                             RRC_UE_INFO_TIME_CONN_FAILURE_R10_PRESENT;
            p_x2ap_rlf_report->time_conn_failure_r10 = 
                      p_rlfReport_r9->timeConnFailure_r10;
        }         
    
        if(1 == p_rlfReport_r9->m.connectionFailureType_r10Present)
        {
            p_x2ap_rlf_report->bitmask |=
                             RRC_UE_INFO_CONN_FAILURE_TYPE_R10_PRESENT;
            p_x2ap_rlf_report->connection_failure_type_r10 = 
                      p_rlfReport_r9->connectionFailureType_r10;
        }         
        if (p_rlfReport_r9->m.failedPCellId_r10Present)
        {
            p_x2ap_rlf_report->bitmask |=
                RRC_UE_INFO_FAILED_PCELLID_PRESENT;

            p_x2ap_rlf_report->failed_pcellid.bitmask |= 
                p_rlfReport_r9->failedPCellId_r10.t;

            switch(p_rlfReport_r9->failedPCellId_r10.t)
            {
                case T_RLF_Report_r9_failedPCellId_r10_cellGlobalId_r10:
                    {
                        if (p_rlfReport_r9->failedPCellId_r10.u.cellGlobalId_r10->
                                plmn_Identity.m.mccPresent)
                        {
                            p_x2ap_rlf_report->failed_pcellid.cell_global_id.plmn_identity.
                                presence_bitmask |= PLMN_IDENTITY_MCC_PRESENCE_FLAG;
                            
                            l3_memcpy_wrapper(p_x2ap_rlf_report->failed_pcellid.cell_global_id.
                                plmn_identity.mcc, 
                                p_rlfReport_r9->failedPCellId_r10.u.cellGlobalId_r10->plmn_Identity.
                                mcc.elem, MCC_OCTET_SIZE);
                        }
                        
                        p_x2ap_rlf_report->failed_pcellid.cell_global_id.plmn_identity.
                            mnc.count = (U8)p_rlfReport_r9->failedPCellId_r10.u.cellGlobalId_r10->
                            plmn_Identity.mcc.n;
                        
                        l3_memcpy_wrapper(p_x2ap_rlf_report->failed_pcellid.cell_global_id.
                                    plmn_identity.mnc.mnc, p_rlfReport_r9->failedPCellId_r10.u.
                                    cellGlobalId_r10->plmn_Identity.mnc.elem, 
                                    p_rlfReport_r9->failedPCellId_r10.u.cellGlobalId_r10->
                                    plmn_Identity.mnc.n);                                                    
                        if (!(p_rlfReport_r9->failedPCellId_r10.u.cellGlobalId_r10->
                            cellIdentity.numbits % RRC_OCTET_SIZE))
                        {
                            length = (U8)(p_rlfReport_r9->failedPCellId_r10.u.cellGlobalId_r10->
                                cellIdentity.numbits/RRC_OCTET_SIZE);
                        }
                        else
                        {
                            length = (U8)((p_rlfReport_r9->failedPCellId_r10.u.cellGlobalId_r10->
                                cellIdentity.numbits/RRC_OCTET_SIZE) + 1);
                        }
                        l3_memcpy_wrapper(p_x2ap_rlf_report->failed_pcellid.cell_global_id.cell_identity,
                                   p_rlfReport_r9->failedPCellId_r10.u.cellGlobalId_r10->
                                   cellIdentity.data, length);
                    }
                    break;

                case T_RLF_Report_r9_failedPCellId_r10_pci_arfcn_r10 :
                    {
                        p_x2ap_rlf_report->failed_pcellid.pci_arfcn.phys_cell_id = 
                            p_rlfReport_r9->failedPCellId_r10.u.pci_arfcn_r10->physCellId_r10;

                        p_x2ap_rlf_report->failed_pcellid.pci_arfcn.carrier_freq = 
                            p_rlfReport_r9->failedPCellId_r10.u.pci_arfcn_r10->carrierFreq_r10;
                    }break;
                
                default :
                    RRC_UECC_TRACE((p_uecc_gb_context)->context_index,
                            (p_uecc_gb_context)->facility_name,
                            RRC_WARNING,
                            " Invalid ????");
            }
            
        }
        
        if (p_rlfReport_r9->m.locationInfo_r10Present)
        {   
            p_x2ap_rlf_report->bitmask |= RRC_UE_INFO_RLF_REPORT_LOACTION_INFO_PRESENT;

            switch(p_rlfReport_r9->locationInfo_r10.locationCoordinates_r10.t)
            {
                case T_LocationInfo_r10_locationCoordinates_r10_ellipsoid_Point_r10 :
                    {
                        p_x2ap_rlf_report->location_info.location_coordiate.bitmask |= 
                            RRC_RLF_LOCATION_CORD_ELLIPSOID_POINT_PRESENT;

                        p_x2ap_rlf_report->location_info.location_coordiate.
                            ellipsoid_point.data_length = p_rlfReport_r9->locationInfo_r10.
                            locationCoordinates_r10.u.ellipsoid_Point_r10->numocts;

                        l3_memcpy_wrapper(p_x2ap_rlf_report->location_info.location_coordiate.
                            ellipsoid_point.data, p_rlfReport_r9->locationInfo_r10.
                            locationCoordinates_r10.u.ellipsoid_Point_r10->data, 
                            p_rlfReport_r9->locationInfo_r10.locationCoordinates_r10.u.
                            ellipsoid_Point_r10->numocts);
                            
                    }break;

                case T_LocationInfo_r10_locationCoordinates_r10_ellipsoidPointWithAltitude_r10 :
                    {
                        p_x2ap_rlf_report->location_info.location_coordiate.bitmask |=
                            RRC_RLF_LOCATION_CORD_ELLIPSOID_POINT_ALTITUDE_PRESENT;

                        p_x2ap_rlf_report->location_info.location_coordiate.
                            ellipsoid_point_altitude.data_length = p_rlfReport_r9->
                            locationInfo_r10.locationCoordinates_r10.u.
                            ellipsoidPointWithAltitude_r10->numocts;

                        l3_memcpy_wrapper(p_x2ap_rlf_report->location_info.location_coordiate.
                            ellipsoid_point_altitude.data, p_rlfReport_r9->locationInfo_r10.
                            locationCoordinates_r10.u.ellipsoidPointWithAltitude_r10->data,
                            p_rlfReport_r9->locationInfo_r10.locationCoordinates_r10.u.
                            ellipsoidPointWithAltitude_r10->numocts);
                    
                    }break;
               
                 default :
                    RRC_UECC_TRACE((p_uecc_gb_context)->context_index,
                            (p_uecc_gb_context)->facility_name,
                            RRC_WARNING,
                            " Invalid ????");
            }
                
            if (p_rlfReport_r9->locationInfo_r10.m.horizontalVelocity_r10Present)
            {
                p_x2ap_rlf_report->location_info.bitmask |= 
                    RRC_RLF_LOCATION_INFO_HORIZON_VELOCITY_PRESENT;
                
                p_x2ap_rlf_report->location_info.horizontal_velocity.data_length = 
                    p_rlfReport_r9->locationInfo_r10.horizontalVelocity_r10.numocts;

                l3_memcpy_wrapper(p_x2ap_rlf_report->location_info.horizontal_velocity.data,
                    p_rlfReport_r9->locationInfo_r10.horizontalVelocity_r10.data,
                    p_rlfReport_r9->locationInfo_r10.horizontalVelocity_r10.numocts);        
            }
            
            if (p_rlfReport_r9->locationInfo_r10.m.gnss_TOD_msec_r10Present)
            {
                p_x2ap_rlf_report->location_info.bitmask |=
                    RRC_RLF_LOCATION_INFO_GNSS_TOD_MSEC_PRESENT;

                p_x2ap_rlf_report->location_info.gnss_tod_msec.data_length = 
                    p_rlfReport_r9->locationInfo_r10.gnss_TOD_msec_r10.numocts;

                l3_memcpy_wrapper(p_x2ap_rlf_report->location_info.gnss_tod_msec.data,
                    p_rlfReport_r9->locationInfo_r10.gnss_TOD_msec_r10.data,
                    p_rlfReport_r9->locationInfo_r10.gnss_TOD_msec_r10.numocts);
            }
        }

        if (p_rlfReport_r9->m.previousPCellId_r10Present)
        {
            p_x2ap_rlf_report->bitmask |= RRC_UE_INFO_PREV_PCELLID_PRESENT;

            if (p_rlfReport_r9->previousPCellId_r10.plmn_Identity.m.mccPresent)
            {
                p_x2ap_rlf_report->previous_pcell_id.plmn_identity.presence_bitmask |=
                    PLMN_IDENTITY_MCC_PRESENCE_FLAG;

                l3_memcpy_wrapper(p_x2ap_rlf_report->previous_pcell_id.plmn_identity.mcc,
                    p_rlfReport_r9->previousPCellId_r10.plmn_Identity.mcc.elem,
                    MCC_OCTET_SIZE);
            }
            
            p_x2ap_rlf_report->previous_pcell_id.plmn_identity.mnc.count = 
                (U8)p_rlfReport_r9->previousPCellId_r10.plmn_Identity.mnc.n;

            l3_memcpy_wrapper(p_x2ap_rlf_report->previous_pcell_id.plmn_identity.mnc.mnc,
                p_rlfReport_r9->previousPCellId_r10.plmn_Identity.mnc.elem,
                p_rlfReport_r9->previousPCellId_r10.plmn_Identity.mnc.n);
            
            if (!(p_rlfReport_r9->previousPCellId_r10.cellIdentity.numbits % RRC_OCTET_SIZE))
            {
               length = (U8)(p_rlfReport_r9->previousPCellId_r10.cellIdentity.
                    numbits/RRC_OCTET_SIZE);
            }
            else
            {
                length = (U8)((p_rlfReport_r9->previousPCellId_r10.cellIdentity.
                    numbits/RRC_OCTET_SIZE) + 1);
            }
            
            l3_memcpy_wrapper(p_x2ap_rlf_report->previous_pcell_id.cell_identity,
                        p_rlfReport_r9->previousPCellId_r10.cellIdentity.data,
                        length);            
        }
    }
    }while(0);

    return result;
}

/****************************************************************************
 * Function Name  : uecc_x2ap_build_and_send_rlf_resp
 * Inputs         : uecc_gb_context_t
 *                  x2ap_rlf_ind_info_t
 *                  ue_index
 *                  ue_context_released_flag
 * Outputs        : None
 * Returns        : void
 * Description    : This function  build and send x2ap_ue_rlf_resp to x2ap
 *                  module
 ***************************************************************************/
void uecc_x2ap_build_and_send_rlf_resp(
        uecc_gb_context_t *p_uecc_gb_context,
        x2ap_rlf_ind_info_t  *p_rlf_ind,
        U16                ue_index,
        U8                 ue_context_released_flag)
{
    static x2ap_ue_rlf_resp_t rlf_resp;
    RLF_Report_r9 container;
    /* BUG 13303 Fix Start */
    uecc_ue_context_t  *p_ue_context = PNULL;
    U16     index_counter = RRC_NULL;
    /* BUG 13303 Fix Stop */     

    /*SPR_18125_START*/
    OSCTXT              asn1_ctx;
    /*SPR_18125_END*/

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_ASSERT(PNULL != p_uecc_gb_context);
    RRC_ASSERT(PNULL != p_rlf_ind);

    memset_wrapper(&rlf_resp, 0, sizeof(x2ap_ue_rlf_resp_t));

    rlf_resp.rlf_ind_flag = RRC_TRUE;
    l3_memcpy_wrapper(&rlf_resp.rlf_ind,
            p_rlf_ind, sizeof(x2ap_rlf_ind_info_t));

    /*SPR_18125_START*/
    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name, RRC_ERROR,
                "ASN context initialization failed");
        return;
    }
    /*SPR_18125_END*/

    if(p_rlf_ind->bitmask & X2AP_RLF_REPORT_PRESENT)
    {
        memset_wrapper(&container,0, sizeof(RLF_Report_r9));
        if (RRC_SUCCESS == uecc_x2ap_decode_RLF_Report(
                    /*SPR_18125_START*/
                    &asn1_ctx,
                    /*SPR_18125_END*/
                    p_uecc_gb_context, &container,
                    p_rlf_ind->rlf_report.message_buff_p, 
                    p_rlf_ind->rlf_report.message_buff_size))
        {
            rlf_resp.rlf_ind.bitmask |= X2AP_RLF_REPORT_PRESENT;
            /*coverity Fix 87055 start */
            p_ue_context = uecc_ue_ctx_get(p_uecc_gb_context,ue_index);
            if ( PNULL == p_ue_context )
	    {
		    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
				    p_uecc_gb_context->facility_name,
				    RRC_WARNING,
				    "UE context is not exist for ue_index :%d",ue_index);
		    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
		    return;

	    }
            if (RRC_SUCCESS != uecc_x2ap_fill_rlf_report(
                     p_uecc_gb_context,p_ue_context,
                     &container, &rlf_resp.rlf_report))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"uecc_x2ap_fill_rlf_report failed.");
                rlf_resp.rlf_ind.bitmask ^= X2AP_RLF_REPORT_PRESENT;
            }
            /*coverity Fix 87055 stop */
        }
    }
    rlf_resp.ue_index = ue_index;
    rlf_resp.ue_context_released = ue_context_released_flag;
    rlf_resp.ho_report_flag = RRC_FALSE;

    /* BUG 13303 Fix Start */
    for (index_counter = 0;
         index_counter < p_uecc_gb_context->total_ue_supported;
         index_counter++)
    {
        if (PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter])
        {
            p_ue_context = p_uecc_gb_context->ue_db.ue_contexts[index_counter];

    	    rlf_resp.cell_index = p_ue_context->cell_index;

	    p_ue_context = PNULL;
	}
    }
    /* BUG 13303 Fix Stop */

    /* Send x2ap_ue_rlf_resp_t to X2AP Module */
    if(RRC_FAILURE ==  uecc_rrc_send_internal_msg(
                RRC_X2AP_MODULE_ID,
                X2AP_UE_RLF_RESP,
                (U16)(sizeof(x2ap_ue_rlf_resp_t)),
                &rlf_resp))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "Sending X2AP_UE_RLF_RESP failed.");
    }
    else
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                p_uecc_gb_context->facility_name, 
                RRC_INFO, "[X2AP_UE_RLF_RESP] sent to x2ap module");
    }

    /*SPR_18125_START*/
    rtFreeContext(&asn1_ctx);
    /*SPR_18125_END*/
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/****************************************************************************
 * Function Name  : x2ap_build_and_send_rlf_resp
 * Inputs         : uecc_gb_context_t
 *                  rlf_ind_flag
 *                  ho_report_flag
 *                  uecc_ue_context_t
 * Outputs        : None
 * Returns        : void
 * Description    : This function  build and send x2ap_ue_rlf_resp to
 *                  x2ap module.
 ***************************************************************************/
void x2ap_build_and_send_rlf_resp(
                uecc_gb_context_t *p_uecc_gb_context,
                U8                rlf_ind_flag,
                U8                ho_report_flag,
                uecc_ue_context_t *p_ue_context)
{
    static x2ap_ue_rlf_resp_t rlf_resp;
    RLF_Report_r9 container;
    /*SPR_18125_START*/
    OSCTXT              asn1_ctx;
    /*SPR_18125_END*/

	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
            p_uecc_gb_context->facility_name, RRC_INFO,
            "rlf_ind_flag :%d . ho_report_flag :%d.",
            rlf_ind_flag,ho_report_flag);

    RRC_ASSERT(PNULL != p_uecc_gb_context);

    memset_wrapper(&rlf_resp, 0, sizeof(x2ap_ue_rlf_resp_t));

    /*SPR_18125_START*/
    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name, RRC_ERROR,
                "ASN context initialization failed");
        return;
    }
    /*SPR_18125_END*/

    if(rlf_ind_flag == RRC_TRUE)
    {
        RRC_ASSERT(PNULL != p_ue_context);
        rlf_resp.rlf_ind_flag = rlf_ind_flag;
        l3_memcpy_wrapper(&rlf_resp.rlf_ind,
                &p_ue_context->x2ap_rlf_ind,
            sizeof(x2ap_rlf_ind_info_t));

        if(p_ue_context->x2ap_rlf_ind.bitmask &
                X2AP_RLF_REPORT_PRESENT )
        {
            memset_wrapper(&container,0, sizeof(RLF_Report_r9));
                            /*SPR_18125_START*/
            if (RRC_SUCCESS == uecc_x2ap_decode_RLF_Report(
                            &asn1_ctx,
                            p_uecc_gb_context,
                            /*SPR_18125_END*/
                              &container, p_ue_context->p_rlf_report_buff,
                             p_ue_context->x2ap_rlf_ind.rlf_report.message_buff_size))
            {
                rlf_resp.rlf_ind.bitmask |= X2AP_RLF_REPORT_PRESENT;

                if (RRC_SUCCESS != uecc_x2ap_fill_rlf_report(
                        p_uecc_gb_context,p_ue_context,&container, &rlf_resp.rlf_report))
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"uecc_x2ap_fill_rlf_report failed.");
                    rlf_resp.rlf_ind.bitmask ^= X2AP_RLF_REPORT_PRESENT;
                }
            }
            rrc_mem_free(p_ue_context->p_rlf_report_buff);
        }

        if(p_ue_context->x2ap_rlf_ind.bitmask &
                X2AP_RLF_CONN_SETUP_INDICATOR_PRESENT )
        {
            rlf_resp.rlf_ind.bitmask |= X2AP_RLF_CONN_SETUP_INDICATOR_PRESENT;
            rlf_resp.rlf_ind.conn_setup_indicator = p_ue_context->x2ap_rlf_ind.conn_setup_indicator;
        }
        rlf_resp.ue_index = p_ue_context->ue_index;
        rlf_resp.ue_context_released = RRC_FALSE;
	/*SPR Fix RLF Indication start*/
        rlf_resp.cell_index = p_ue_context->cell_index;
	/*SPR Fix RLF Indication stop*/
        if((ho_report_flag == RRC_TRUE) && 
            (PNULL != p_ue_context->ho_info.p_ho_info) &&
            (PNULL != p_ue_context->ho_info.p_ho_info->p_trg_ho_info) &&
            /*CA_PHASE3_INTER_CELL_MRO_START*/
            ((X2_HO == p_ue_context->ho_info.s1_or_x2_handover) ||
             ((INTER_CELL_HO == p_ue_context->ho_info.s1_or_x2_handover) &&
             (RRC_TRUE == p_ue_context->ho_report_internal_flag))))
            /*CA_PHASE3_INTER_CELL_MRO_STOP*/
        {
            RRC_ASSERT(PNULL != p_ue_context);
            rlf_resp.ho_report_flag = ho_report_flag;
            rlf_resp.ho_report.bitmask = 0;
            l3_memcpy_wrapper(&rlf_resp.ho_report.enb1_gb_id,
                    &p_ue_context->x2ap_rlf_ind.enb1_gb_id,
                    sizeof(x2_gb_enb_id_t));
            /* rlf_resp.ho_report.enb2_gb_id to be filled at x2ap */
            /*CA_PHASE3_INTER_CELL_MRO_START*/
            /*peer_ennode_id should not be filled at the time of inter cell
             * handover*/
            if (X2_HO == p_ue_context->ho_info.s1_or_x2_handover)
            {
                rlf_resp.ho_report.peer_enodeb_id = p_ue_context->
                    x2ap_context.peer_enodeb_id;
            }
            rlf_resp.cell_index = p_ue_context->cell_index;
            /*CA_PHASE3_INTER_CELL_MRO_STOP*/

            l3_memcpy_wrapper(&rlf_resp.ho_report.src_ecgi.eutran_cell_id,
                    &p_ue_context->ho_info.p_ho_info->p_trg_ho_info->
                        rrc_x2ap_ho_request.rrc_x2ap_ue_ctx_info.rRC_Container.
                        as_Config.sourceSystemInformationBlockType1.
                        cellAccessRelatedInfo.cellIdentity.data,
                    HOME_ENB_ID_OCTET_SIZE);

            l3_memcpy_wrapper(&rlf_resp.ho_report.ho_cause,
                    &p_ue_context->ho_report_cause, sizeof(x2ap_cause_t));

            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_INFO, "cause_type:%d, cause:%d.",
                    rlf_resp.ho_report.ho_cause.type, rlf_resp.ho_report.ho_cause.value);

            l3_memcpy_wrapper(&rlf_resp.ho_report.fail_cell_ecgi.plmn_identity.plmn_id,
                    &p_ue_context->ho_info.p_ho_info->x2ap_targetCell_ID.
                    pLMN_Identity.data,
                    MAX_PLMN_ID_BYTES);

            l3_memcpy_wrapper(&rlf_resp.ho_report.fail_cell_ecgi.eutran_cell_id,
                    &p_ue_context->ho_info.p_ho_info->x2ap_targetCell_ID.
                    eUTRANcellIdentifier.data,
                    HOME_ENB_ID_OCTET_SIZE);
        }
        else
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name, RRC_ERROR,
                    "Either ho_report is not set or handover context is NULL.");
            rlf_resp.ho_report_flag = RRC_FALSE;
        }

        /* Send x2ap_ue_rlf_resp_t to X2AP Module */
        if(RRC_SUCCESS !=  uecc_rrc_send_internal_msg(
                RRC_X2AP_MODULE_ID,
                X2AP_UE_RLF_RESP,
                (U16)(sizeof(x2ap_ue_rlf_resp_t)),
                &rlf_resp))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "Sending X2AP_UE_RLF_RESP failed.");
        }
        else
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                       p_uecc_gb_context->facility_name, 
                       RRC_INFO, "[X2AP_UE_RLF_RESP] sent to x2ap module");
        }
        p_ue_context->ho_report_flag = RRC_FALSE;
        /*CA_PHASE3_INTER_CELL_MRO_START*/
       /*resetting the internal flag that is set for inter cell ho*/
        p_ue_context->ho_report_internal_flag = RRC_FALSE;
        /*CA_PHASE3_INTER_CELL_MRO_STOP*/
    }

    /*SPR_18125_START*/
    rtFreeContext(&asn1_ctx);
    /*SPR_18125_END*/
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
 * FUNCTION NAME: uecc_x2ap_process_rlf_msg_ind
 *
 *   Input: 
 *          void               *p_api 
 *          uecc_gb_context_t* p_uecc_gb_context
 *   Outputs : none   
 * DESCRIPTION:
 *     This function processes X2AP_UE_RLF_MSG_IND
 *
 * RETURNS:
 *     None
 * ***************************************************************************/
void uecc_x2ap_process_rlf_msg_ind(
        void *p_api,
        uecc_gb_context_t *p_uecc_gb_context)
{
    U8 msg_ind_buff[sizeof(x2ap_rlf_ind_info_t)+X2AP_MAX_ASN1_BUF_LEN];
    x2ap_rlf_ind_info_t  *p_rlf_ind = (x2ap_rlf_ind_info_t *)msg_ind_buff;
    uecc_ue_context_t  *p_ue_context = PNULL;
    U8 rlf_ind_flag = RRC_FALSE;
    U8 ho_report_flag = RRC_FALSE;
    U16 buf_size = 0;
    U16     index_counter = RRC_NULL;
    U8 ue_ctxt_found = RRC_FALSE;
    U8 rlf_resp_sent = RRC_FALSE;
    ue_context_info_list_node_t *p_ue_context_info_list_node = PNULL;
    U16  ue_index = 0;
    U8   ue_context_released = RRC_FALSE;
    U8   ue_context_info_matched = RRC_FALSE;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_ASSERT(PNULL!=p_api);
    RRC_ASSERT(PNULL!=p_uecc_gb_context);

    /* SPR 21803 Fix Start */
    if(RRC_FAILURE == uecc_check_x2ap_message(p_uecc_gb_context))
    {
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }
    /* SPR 21803 Fix Stop */

    p_rlf_ind = (x2ap_rlf_ind_info_t *)((U8*)p_api + RRC_API_HEADER_SIZE);
    buf_size = rrc_get_api_buf_size(p_api);

    /* Precondition - p_api should point to right message */
    if (buf_size <
            RRC_API_HEADER_SIZE + sizeof(x2ap_rlf_ind_info_t))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_WARNING,
                "[X2AP_UE_RLF_MSG_IND] Wrong message size - ignore.");
        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }

    p_ue_context_info_list_node = (ue_context_info_list_node_t*)
        list_first_node(&p_uecc_gb_context->ue_context_info_list);

    while(p_ue_context_info_list_node)
    {
        if ((p_rlf_ind->crnti == p_ue_context_info_list_node->ue_ctx_info.crnti)
                    &&(p_rlf_ind->failure_cell_pci == 
                        p_ue_context_info_list_node->ue_ctx_info.pci))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name, RRC_INFO,
                "CRNTI and PCI matched from UE_CONTEXT_INFO_LIST");
            ue_index = p_ue_context_info_list_node->ue_ctx_info.ue_index;
            ue_context_released = 
                p_ue_context_info_list_node->ue_ctx_info.ue_context_released;

            if (p_rlf_ind->bitmask & X2AP_SHORT_MAC_I_PRESENT)
            {
                if(p_rlf_ind->short_mac_i == 
                        p_ue_context_info_list_node->ue_ctx_info.short_mac_i)
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_INFO,"SHORT_MAC_I matched");
                    ue_context_info_matched = RRC_TRUE;
                    break;
                }
                else
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_INFO,"SHORT_MAC_I did not match");
                    ue_context_info_matched = RRC_FALSE;
                }
            }
            else
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_INFO,"SHORT_MAC_I not present");
                ue_context_info_matched = RRC_TRUE;
                break;
            }
        }
        p_ue_context_info_list_node = (ue_context_info_list_node_t*)
                list_next_node(&p_ue_context_info_list_node->node);
    }

    if(RRC_TRUE == ue_context_info_matched)
    {
        uecc_x2ap_build_and_send_rlf_resp(p_uecc_gb_context,
                                          p_rlf_ind, ue_index,
                                          ue_context_released);
    }
    else
    {
    /* CRNTI, Failure Cell PCI and shortMAC-I validation */
    RRC_ASSERT(PNULL != p_uecc_gb_context->ue_db.ue_contexts);

    
    for (index_counter = 0;
         index_counter < p_uecc_gb_context->total_ue_supported;
         index_counter++)
    {
        if (PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter])
        {
            ue_ctxt_found = RRC_TRUE;
            p_ue_context = p_uecc_gb_context->ue_db.ue_contexts[index_counter];

            p_ue_context->ho_report_flag = RRC_FALSE;
            if (p_rlf_ind->bitmask & X2AP_RLF_CONN_SETUP_INDICATOR_PRESENT)
            {
                l3_memcpy_wrapper(&p_ue_context->x2ap_rlf_ind,
                        p_rlf_ind, 
                        sizeof(x2ap_rlf_ind_info_t));

                if(p_ue_context->x2ap_rlf_ind.bitmask &
                        X2AP_RLF_REPORT_PRESENT )
                {
                    p_ue_context->p_rlf_report_buff = 
                        rrc_mem_get(p_rlf_ind->rlf_report.message_buff_size);
                    if (PNULL == p_ue_context->p_rlf_report_buff)
                    {
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_ERROR, "rrc_mem_get failed.");
                        rlf_ind_flag = RRC_FALSE;
                        break;
                    }
                    l3_memcpy_wrapper(p_ue_context->p_rlf_report_buff,
                            p_rlf_ind->rlf_report.message_buff_p,
                            p_rlf_ind->rlf_report.message_buff_size);
                }
                rlf_ind_flag = RRC_TRUE;
            }
            else
            {
            if (p_ue_context->crnti == p_rlf_ind->crnti)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_INFO, "crnti matched.");
                if(p_uecc_gb_context->p_p_csc_context[p_ue_context->cell_index]->
                   p_csc_init_setup_ind->phys_cell_id ==
                        p_rlf_ind->failure_cell_pci)
                {
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_INFO, "failure_cell_pci matched.");
                    l3_memcpy_wrapper(&p_ue_context->x2ap_rlf_ind,
                               p_rlf_ind, sizeof(x2ap_rlf_ind_info_t));

                    if ( uecc_check_store_ue_context_timer(p_ue_context) || 
                        ( (p_ue_context->uecc_fsm.p_current_state->s_name != PNULL) &&
                          !strcmp_wrapper(p_ue_context->uecc_fsm.p_current_state->s_name,
                           (const S8 *)"UECC_UE_TRG_HO_ONGOING")) )
                    {
                        ho_report_flag = RRC_TRUE;
                        /*CA_PHASE3_INTER_CELL_MRO_START*/
                        /*setting the flag in ue context in case of too early
                          inter cell ho*/
                        if (RRC_TRUE == p_ue_context->x2ap_rlf_ind.
                                too_early_inter_cell_ho_flg)
                        {
                            p_ue_context->ho_report_internal_flag = RRC_TRUE;
                        }
                        /*CA_PHASE3_INTER_CELL_MRO_STOP*/
                    }
                    p_ue_context->ho_report_flag = ho_report_flag;

                    if(p_ue_context->x2ap_rlf_ind.bitmask &
                                           X2AP_RLF_REPORT_PRESENT )
                    {
                        p_ue_context->p_rlf_report_buff = 
                            rrc_mem_get(p_rlf_ind->rlf_report.message_buff_size);
                        if (PNULL == p_ue_context->p_rlf_report_buff)
                        {
                            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_ERROR, "rrc_mem_get failed.");
                            rlf_ind_flag = RRC_FALSE;
                            break;
                        }
                        l3_memcpy_wrapper(p_ue_context->p_rlf_report_buff,
                                p_rlf_ind->rlf_report.message_buff_p,
                                p_rlf_ind->rlf_report.message_buff_size);
                    }

                    /* if short_mac_i is present, send UECC_LLIM_MAC_I_REQ */
                    if (p_rlf_ind->bitmask & X2AP_SHORT_MAC_I_PRESENT)
                    {
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_INFO, "short_mac_i present.");
                        if((p_ue_context->ho_info.p_ho_info != PNULL) &&
                          (p_ue_context->ho_info.p_ho_info->p_src_ho_info != PNULL) &&
                          (p_ue_context->ho_info.p_ho_info->p_src_ho_info->
                               short_mac_I[0].short_mac_i == p_rlf_ind->short_mac_i))
                        {
                            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                    p_uecc_gb_context->facility_name,
                                    RRC_INFO, "short_mac_i matched.");
                            rlf_ind_flag = RRC_TRUE;
                        }
                        else
                        {
                            p_ue_context->x2ap_rlf_ind_recvd_flag = RRC_TRUE;
                            uecc_llim_rlf_ind_build_and_send_mac_i_req(p_ue_context);
                            rlf_resp_sent = RRC_TRUE;
                        }
                    }
                    else
                    {
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                                p_uecc_gb_context->facility_name,
                                RRC_INFO, "short_mac_i not present.");
                        rlf_ind_flag = RRC_TRUE;
                    }
                }
            }
            }
        }
    }
    if ((ue_ctxt_found == RRC_FALSE) || (rlf_resp_sent == RRC_FALSE))
    {
	/*klockwork Fix*/
        if (PNULL != p_ue_context)
	{
	    x2ap_build_and_send_rlf_resp(p_uecc_gb_context,
                                 rlf_ind_flag,
                                 ho_report_flag, 
                                 p_ue_context);
	}
	/*klockwork Fix*/
    }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
 * FUNCTION NAME: uecc_check_store_ue_context_timer
 *
 *   Input: 
 *         uecc_ue_context_t* p_ue_context
 *   Outputs : None
 * DESCRIPTION:
 *     This function checks if the UECC_STORE_UE_CONTEXT_TIMER is running
 *
 * RETURNS:
 *     RRC_TRUE or RRC_FALSE
 * ***************************************************************************/
rrc_return_t uecc_check_store_ue_context_timer(
        uecc_ue_context_t  *p_ue_context)
{
    if (PNULL != p_ue_context->timers_data .timers_ids[UECC_STORE_UE_CONTEXT_TIMER])
    {
        /* Timer is running */
        return RRC_TRUE;
    }
    return RRC_FALSE;
}

/*SPR_16618_START*/
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_error_indication
 *
 *   Input: 
 *        x2ap_X2AP_PDU            *p_x2ap_pdu
 *        uecc_gb_context_t   *p_uecc_gb_context
 *        OSCTXT*             p_asn1_ctx
*         U8                  peer_enodeb_id
 *        U8                  *buffer
 *   Outputs : None
 *   DESCRIPTION:
 *       This function processes s1ap_UEContextReleaseCommand
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void uecc_x2ap_process_error_indication
(
 x2ap_X2AP_PDU            *p_x2ap_pdu,    /* Pointer to x2ap pdu */
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 OSCTXT*             p_asn1_ctx, /* ASN Context Pointer */
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 U8                  *buffer
 )
/*SPR_16618_END*/
{
    /* SPR 16750 Fix Start */
    /* Code Removed */
    /* SPR 16750 Fix Stop */
    rrc_return_et result = RRC_SUCCESS;
    rrc_x2ap_error_indication_t     msg;
    rrc_bool_et                     is_processing_reqd = RRC_FALSE;
    EVENT_EXTERNAL_HEADER           *p_event_header = PNULL;
    LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT  *p_event_x2_err_ind = PNULL;
    U32				                encoded_msg_len = 0;
	
    /*Error_Handling_changes*/
#ifdef ENDC_ENABLED
    rrc_bool_et                     unknown_apid_cause_prsnt = RRC_FALSE;
	uecc_ue_context_t               *p_ue_context = PNULL;
	uecc_ue_context_t               *p_second_ue_context = PNULL;
#endif
    /*Error_Handling_changes*/

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
        facility_name, RRC_INFO,"Recived Error Indication");

    RRC_ASSERT(PNULL!=p_x2ap_pdu);
    RRC_ASSERT(PNULL!=p_x2ap_pdu->u.initiatingMessage);


    do
    {
        memset_wrapper(&msg,0,sizeof(rrc_x2ap_error_indication_t));
        if (PNULL==p_x2ap_pdu->u.initiatingMessage->value.u.errorIndication)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING,
                 "[x2ap_ErrorIndication] errorIndication field is PNULL");
            /* Drop message */
            break;
        }

        /* Decode message */
        result = rrc_x2ap_error_indication_intrl_dec(
                p_x2ap_pdu->u.initiatingMessage->value.u.errorIndication,
                &msg);

        if (RRC_SUCCESS != result)
        {
            /* Message parsing error */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name, RRC_WARNING,
                    "[x2ap_ErrorIndication] Parsing error");
            /* Drop message */
            break;
        }
        else
        {
   	    /*Generating Protocol Event LOCAL_X2_ERROR_INDICATION*/
            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_ERROR_INDICATION, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)buffer,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
	    
	    /*Generating Protocol Event LOCAL_EVENT_X2_ERROR_INDICATION*/
            p_event_x2_err_ind = rrc_mem_get(sizeof(LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT));
            if (PNULL != p_event_x2_err_ind)
            {
                memset_wrapper(p_event_x2_err_ind, RRC_NULL, 
                        sizeof(LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT));
                p_event_x2_err_ind->header.length = sizeof(LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT);
                p_event_x2_err_ind->header.event_id = LOCAL_EVENT_X2_ERROR_INDICATION;

                p_event_x2_err_ind->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_RECEIVED;

                if (msg.bitmask & RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT)
                {
                    p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE_GROUP = 
                        (U32)(msg.cause.t);

        /* CSR 51448 START */
                    switch(p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE_GROUP)
                    {
                        case T_x2ap_Cause_radioNetwork:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE = 
                                msg.cause.u.radioNetwork;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    (U32)(msg.cause.t), msg.cause.u.radioNetwork);
                            break;

                        case T_x2ap_Cause_transport:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                                msg.cause.u.transport;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    (U32)(msg.cause.t), msg.cause.u.transport);
                            break;
                        case T_x2ap_Cause_protocol:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                                msg.cause.u.protocol;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    (U32)(msg.cause.t), msg.cause.u.protocol);
                            break;
                        case T_x2ap_Cause_misc:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                                msg.cause.u.misc;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    (U32)(msg.cause.t), msg.cause.u.misc);
                            break;
                    }
        /* CSR 51448 STOP */
                }
                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_x2_err_ind);
            }
        }

        /*SPR_16618_START*/
        if ((msg.bitmask & 
                RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT) &&
                (msg.criticality_diagnostics.m.procedureCodePresent))
        /*SPR_16618_END*/
        {
            switch(msg.criticality_diagnostics.procedureCode)
            {
                #ifdef ENDC_ENABLED
                /* MENB CHANGES - START */
                case ASN1V_x2ap_id_endcX2Setup:
                /* MENB CHANGES - END */
                #endif
                case ASN1V_x2ap_id_loadIndication:
                case ASN1V_x2ap_id_x2Setup:
                case ASN1V_x2ap_id_reset:
                case ASN1V_x2ap_id_eNBConfigurationUpdate:
                case ASN1V_x2ap_id_resourceStatusReportingInitiation:
                case ASN1V_x2ap_id_resourceStatusReporting:
                case ASN1V_x2ap_id_mobilitySettingsChange:
                case ASN1V_x2ap_id_rLFIndication:
                case ASN1V_x2ap_id_handoverReport:
                case ASN1V_x2ap_id_errorIndication:
                case ASN1V_x2ap_id_cellActivation:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_INFO,
                        "Error Indication Received, Procedure Code:%d", 
                        msg.criticality_diagnostics.procedureCode);
                    break;
                default:
                    is_processing_reqd = RRC_TRUE;
                    break;
            }
        }
        /*Error_Handling_changes*/
#ifdef ENDC_ENABLED
        if((msg.bitmask & RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT || 
                msg.bitmask & RRC_X2AP_ERROR_IND_OLD_SGNB_UE_X2AP_ID_PRESENT) &&
                (msg.bitmask & RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT) &&
                (msg.cause.t == T_x2ap_Cause_radioNetwork) &&
                ((x2ap_unknown_new_eNB_UE_X2AP_ID ==  msg.cause.u.radioNetwork) ||
                (x2ap_unknown_old_en_gNB_UE_X2AP_ID ==  msg.cause.u.radioNetwork) ||
                (x2ap_unknown_pair_of_UE_X2AP_ID ==  msg.cause.u.radioNetwork)))
        {
            is_processing_reqd = RRC_TRUE;
            unknown_apid_cause_prsnt = RRC_TRUE;
        }
#endif
        /*Error_Handling_changes*/

        if (is_processing_reqd == RRC_TRUE)
        {
            /* Log the message */
            uecc_x2ap_log_error_indication_message(p_uecc_gb_context, &msg);

            /*Error_Handling_changes*/
#ifdef ENDC_ENABLED
            if(msg.bitmask & RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT)
            {
                p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                        p_uecc_gb_context, peer_enodeb_id,
                        msg.new_enb_ue_x2ap_id, MENB_X2AP_UE_ID);

                if(((PNULL != p_ue_context) && (p_ue_context->x2ap_context.m.sgnb_ue_x2ap_id_present) &&
                            (msg.bitmask & RRC_X2AP_ERROR_IND_OLD_SGNB_UE_X2AP_ID_PRESENT) &&
                            (msg.old_sgnb_ue_x2ap_id != p_ue_context->x2ap_context.sgnb_ue_x2ap_id)) ||
                        ( (PNULL == p_ue_context) &&
                          (msg.bitmask & RRC_X2AP_ERROR_IND_OLD_SGNB_UE_X2AP_ID_PRESENT)))
                {
                    p_second_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                            p_uecc_gb_context, peer_enodeb_id,
                            msg.old_sgnb_ue_x2ap_id, SGNB_X2AP_UE_ID);

                    if(PNULL != p_ue_context)
                    {
                        unknown_apid_cause_prsnt = RRC_TRUE;
                    }
                    else
                    {
                        p_ue_context = p_second_ue_context;
                    }
                }

                if(RRC_TRUE == unknown_apid_cause_prsnt)
                {
                    if(PNULL != p_ue_context)
                    {
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                                facility_name, RRC_INFO,
                                "[x2ap_ErrorIndication] push event - "
                                "UECC_EV_X2AP_ERROR_INDICATION for UE Index [%d] matching (New) MeNB UE X2 ID %u",
                                p_ue_context->ue_index, msg.new_enb_ue_x2ap_id);

                        uecc_fsm_process_event(
                                p_ue_context,
                                UECC_EV_X2AP_ERROR_INDICATION,
                                &msg);
                    }
                    if(PNULL != p_second_ue_context)
                    {
                        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                                facility_name, RRC_INFO,
                                "[x2ap_ErrorIndication] push event - "
                                "UECC_EV_X2AP_ERROR_INDICATION for UE Index [%d] matching (Old) SgNB UE X2 ID %u",
                                p_second_ue_context->ue_index, msg.old_sgnb_ue_x2ap_id);

                        uecc_fsm_process_event(
                                p_second_ue_context,
                                UECC_EV_X2AP_ERROR_INDICATION,
                                &msg);
                    }
                    break;
                }
            }
#endif
            /*Error_Handling_changes*/

            /*SPR_16618_START*/
            /* Get context using X2AP id */
            switch(msg.criticality_diagnostics.procedureCode)
            {
        /* SPR 16750 Fix Start */
                case ASN1V_x2ap_id_handoverPreparation: 
                    if(msg.criticality_diagnostics.m.triggeringMessagePresent)
                    {
                        if(x2ap_initiating_message == 
                                msg.criticality_diagnostics.triggeringMessage)
                        {/* eNB is acting as SeNB for this UE */
                            uecc_x2ap_handle_src_enb_error_ind (p_uecc_gb_context, 
                            &msg, peer_enodeb_id,encoded_msg_len, buffer);
                        }
                        else
                        {/* eNB is acting as TeNB for this UE */
                            uecc_x2ap_handle_trg_enb_error_ind (p_uecc_gb_context, 
                            &msg, peer_enodeb_id,encoded_msg_len, buffer);
                        }
                    }
                    break;
                case ASN1V_x2ap_id_handoverCancel: 
                case ASN1V_x2ap_id_snStatusTransfer:
                    /* eNB is acting as SeNB for this UE */
                    uecc_x2ap_handle_src_enb_error_ind (p_uecc_gb_context, 
                                                            &msg,peer_enodeb_id,encoded_msg_len, buffer);
                    break;
                case ASN1V_x2ap_id_uEContextRelease: 
                    /* eNB is acting as TeNB for this UE */
                    uecc_x2ap_handle_trg_enb_error_ind (p_uecc_gb_context,&msg ,peer_enodeb_id, 
                                                                        encoded_msg_len, buffer);
                    break;
                /*Error_Handling_changes*/
#ifdef ENDC_ENABLED
                case ASN1V_x2ap_id_sgNBAdditionPreparation:
                case ASN1V_x2ap_id_meNBinitiatedSgNBRelease:
                case ASN1V_x2ap_id_sgNBinitiatedSgNBRelease:
                /*ENDC Case */
                uecc_x2ap_handle_endc_error_ind( 
                        p_uecc_gb_context,&msg ,peer_enodeb_id,
                        encoded_msg_len, buffer);
                break;
#endif
                /*Error_Handling_changes*/
            }
            /*SPR_16618_END*/

            }
        if ( (!(msg.bitmask & 
                RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT)) ||
                 (!(msg.criticality_diagnostics.m.procedureCodePresent)) ||
                    ((ASN1V_x2ap_id_handoverPreparation == 
                        msg.criticality_diagnostics.procedureCode) &&
                          (!(msg.criticality_diagnostics.m.triggeringMessagePresent))))
        {/* It is not clear that eNB is acting as SeNB or TeNB. So, L3 will compare
            received X2AP Id with all UEs irrespective of the eNB type i.e. this
            eNB is acting as SeNB or TeNB */
            if ( (msg.bitmask & RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT) ||
                   (msg.bitmask & RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT) )
            {
                uecc_x2ap_handle_unknown_error_ind (p_uecc_gb_context, &msg, peer_enodeb_id,
                                                         encoded_msg_len, buffer);
            }
        }
     /*SPR 16750 Fix Stop */
    } while (0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return;
}

/****************************************************************************
* Function Name  : uecc_x2ap_build_and_send_error_indication_unknown_proc
* Inputs         : uecc_gb_context_t   *p_uecc_gb_context
*                  U8                  peer_enodeb_id
*                  x2ap_ProcedureCode proc_code
*                  x2ap_Criticality proc_criticality
*                  x2ap_TriggeringMessage triggering_msg
*                  p_ue_context_release_complete
* Outputs        : None
* Returns        : RRC_SUCCESS / RRC_FAILURE
* Description    : packs p_error_indication into x2ap_X2AP_PDU ASN
****************************************************************************/
rrc_return_et uecc_x2ap_build_and_send_error_indication_unknown_proc(
        uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
        U8                  peer_enodeb_id,      /* Peer EnodeB ID */
        x2ap_ProcedureCode proc_code,
        x2ap_Criticality proc_criticality,
        x2ap_TriggeringMessage triggering_msg
        )
{
    rrc_x2ap_error_indication_t  error_indication;
    rrc_return_et result = RRC_SUCCESS;

    RRC_UT_TRACE_ENTER();

    memset_wrapper(&error_indication, RRC_NULL, sizeof(rrc_x2ap_error_indication_t));

    error_indication.bitmask = 0;

    uecc_x2ap_fill_error_indication_values(&error_indication,
            proc_code,
            triggering_msg,
            proc_criticality);

    error_indication.bitmask |= RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;

    if (RRC_SUCCESS != uecc_x2ap_build_and_send_err_indication(
                peer_enodeb_id,
                &error_indication))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_WARNING,"%s : Failed", __FUNCTION__);
        result = RRC_FAILURE;
    }

    return result; 
}

/****************************************************************************
 * Function Name  :uecc_x2ap_log_criticality_diagostics 
 * Inputs         : uecc_gb_context_t           *p_uecc_gb_context 
 *                  x2ap_CriticalityDiagnostics *p_criticality_diagostics
 * Outputs        : None
 * Returns        : None
 * Description    : Class -2 Errors 
 *                : NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
 ****************************************************************************/
void uecc_x2ap_log_criticality_diagostics(
        uecc_gb_context_t           *p_uecc_gb_context,
        x2ap_CriticalityDiagnostics *p_criticality_diagostics)
{
    x2ap_CriticalityDiagnostics_IE_List_element* p_item = PNULL;
    OSRTDListNode* p_node = PNULL;
    U8 count = X2AP_NULL;


    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
         facility_name, RRC_INFO,"[CRITICALITY DIAGNOSTICS]");

    RRC_ASSERT(PNULL!=p_criticality_diagostics);
   
    if (p_criticality_diagostics->m.procedureCodePresent)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_INFO,"[CD] ProcedureCode = %d",
                p_criticality_diagostics->procedureCode);
    }

    if (p_criticality_diagostics->m.triggeringMessagePresent)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_INFO,"[CD] TriggeringMessage = %d",
                p_criticality_diagostics->triggeringMessage);
    }

    if (p_criticality_diagostics->m.procedureCriticalityPresent)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_INFO,"[CD] ProcedureCriticality = %d",
                p_criticality_diagostics->procedureCriticality);
    }

    if (p_criticality_diagostics->m.iEsCriticalityDiagnosticsPresent)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name, RRC_INFO,"[CD] iEsCriticalityDiagnostics ");

        p_node = p_criticality_diagostics->iEsCriticalityDiagnostics.head;
        for (count = 0; count < p_criticality_diagostics->iEsCriticalityDiagnostics.count;
                count++)
        {
            p_item = (x2ap_CriticalityDiagnostics_IE_List_element*)p_node->data;

            if (PNULL == p_item)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,"[CD] Pointer NULL ");
                break;
            }

            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
               facility_name, RRC_INFO,"------iE_Criticality %d",p_item->iECriticality);
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
               facility_name, RRC_INFO,"------iE_ID %d", p_item->iE_ID);

            switch (p_item->typeOfError)
            {
                case 0:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                       facility_name,RRC_INFO,"[CD]-%d-------TypeOfError:not-understood",
                       p_item->iECriticality);
                    break;
                case 1:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                       facility_name,RRC_INFO,"[CD]-%d------TypeOfError: missing",
                            p_item->iECriticality);
                    break;
                default:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                       facility_name,RRC_INFO,"[CD]Unknwon Type of Error");
            }
            p_node = p_node->next;
        }
         
    }

}

/****************************************************************************
* Function Name  :uecc_x2ap_log_error_indication_message
* Inputs         : uecc_gb_context_t           *p_uecc_gb_context 
*                  rrc_x2ap_error_indication_t *p_err_ind
* Outputs        : None
* Returns        : None
* Description    : Class -2 Errors 
*                : NOTE: NOT HANDLING SEQUENCE & CHOICE HERE
****************************************************************************/
void uecc_x2ap_log_error_indication_message(
        uecc_gb_context_t           *p_uecc_gb_context,
        rrc_x2ap_error_indication_t *p_err_ind)
{

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
        facility_name,RRC_INFO,"[ERROR INDICATION MESSAGE]");

    if (RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT & 
            p_err_ind->bitmask)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name,RRC_INFO,"[EI] OLD ENB UE X2AP ID = %d ",
            p_err_ind->old_enb_ue_x2ap_id);
    }

    if (RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT & 
            p_err_ind->bitmask)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name,RRC_INFO,"[EI] NEW ENB UE X2AP ID = %d ",
                                    p_err_ind->new_enb_ue_x2ap_id);
    }

    if (RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT &
            p_err_ind->bitmask)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name,RRC_INFO,"[EI] Cause Type: =%d [EI] Cause Value =%d ",
                p_err_ind->cause.t, p_err_ind->cause.u.radioNetwork);
    }

    if (RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT & 
            p_err_ind->bitmask)
    {
        uecc_x2ap_log_criticality_diagostics(p_uecc_gb_context, &p_err_ind->criticality_diagnostics);
    }

}
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_error_indication
 *   INPUTS       :
 *       uecc_gb_context_t           *p_uecc_gb_context
 *       U8                  peer_enodeb_id
 *       x2ap_UE_X2AP_ID     old_enb_ue_x2ap_id
 *       rrc_bool_et         old_enb_ue_x2ap_id_present
 *       x2ap_UE_X2AP_ID     new_enb_ue_x2ap_id
 *       rrc_bool_et         new_enb_ue_x2ap_id_presen
 *       x2ap_CriticalityDiagnostics *p_criticality_diagnostics
 *       rrc_bool_et         criticality_diagnostics_present
 *       x2ap_Cause*         p_cause
 *       uecc_ue_context_t*  p_ue_context
 *   OUTPUTS    : None
 *   DESCRIPTION:
 *       This function builds and sends x2ap_ErrorIndication message
 *       to X2AP module
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
rrc_return_et uecc_x2ap_build_and_send_error_indication(
        uecc_gb_context_t           *p_uecc_gb_context,
        U8                  peer_enodeb_id,
        x2ap_UE_X2AP_ID     old_enb_ue_x2ap_id,
        rrc_bool_et         old_enb_ue_x2ap_id_present,
        x2ap_UE_X2AP_ID     new_enb_ue_x2ap_id,
        rrc_bool_et         new_enb_ue_x2ap_id_present,
        x2ap_CriticalityDiagnostics *p_criticality_diagnostics,
        rrc_bool_et         criticality_diagnostics_present,
        x2ap_Cause*         p_cause,
        uecc_ue_context_t*  p_ue_context
        )
{
    rrc_return_et result = RRC_FAILURE;
    rrc_bool_et   is_cause_set = RRC_FALSE;
    OSCTXT asn1_ctx;
    rrc_x2ap_error_indication_t msg;
    U8 p_message[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN] = {0};
    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)p_message;
    LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT  *p_event_x2_err_ind = PNULL;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;

    RRC_ASSERT(PNULL != p_cause);


    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
            facility_name,RRC_ERROR,
                "[x2ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }
    
    /* Reset message */
    memset_wrapper(&msg, RRC_NULL, sizeof(rrc_x2ap_error_indication_t));
    memset_wrapper(p_sig_msg_req,RRC_NULL,sizeof(x2ap_ue_associated_sig_msg_req_t));

    p_sig_msg_req->peer_enodeb_id = peer_enodeb_id;
    do
    {
        p_sig_msg_req->procedure_code = ASN1V_x2ap_id_errorIndication;
       
        if ((RRC_FALSE == old_enb_ue_x2ap_id_present)&&
                (RRC_FALSE == new_enb_ue_x2ap_id_present))
        {
            msg.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
            msg.cause.t = T_x2ap_Cause_radioNetwork;
            msg.cause.u.radioNetwork = x2ap_unknown_pair_of_UE_X2AP_ID;
            is_cause_set = RRC_TRUE;
        }
        
        if (RRC_TRUE == old_enb_ue_x2ap_id_present)
        {
            msg.old_enb_ue_x2ap_id = old_enb_ue_x2ap_id;
            msg.bitmask |= RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
            p_sig_msg_req->bitmask |= X2AP_UE_ASSOC_SIG_MSG_REQ_OLD_ENB_UE_X2AP_ID_PRESENT;
            p_sig_msg_req->old_enb_ue_x2ap_id = old_enb_ue_x2ap_id;

        }
        else if ((RRC_FALSE == old_enb_ue_x2ap_id_present) && (is_cause_set == RRC_FALSE))
        {
                msg.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                msg.cause.t = T_x2ap_Cause_radioNetwork;
                msg.cause.u.radioNetwork = x2ap_unknown_old_eNB_UE_X2AP_ID;
                is_cause_set = RRC_TRUE;
        }

        if (RRC_TRUE == new_enb_ue_x2ap_id_present)
        {
            msg.new_enb_ue_x2ap_id = new_enb_ue_x2ap_id;
            msg.bitmask |= RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
            p_sig_msg_req->bitmask |= X2AP_UE_ASSOC_SIG_MSG_REQ_NEW_ENB_UE_X2AP_ID_PRESENT;
            p_sig_msg_req->new_enb_ue_x2ap_id = new_enb_ue_x2ap_id;
        }
        else if ((RRC_FALSE == new_enb_ue_x2ap_id_present) && (is_cause_set == RRC_FALSE))
        {
            msg.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
            msg.cause.t = T_x2ap_Cause_radioNetwork;
            msg.cause.u.radioNetwork = x2ap_unknown_new_eNB_UE_X2AP_ID;
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name,RRC_WARNING,
                    "[x2ap_ErrorIndication] new enb id not present");
            msg.new_enb_ue_x2ap_id = 0; /*not known*/
            is_cause_set = RRC_TRUE;
        }

        if ((PNULL != p_cause) && (is_cause_set == RRC_FALSE))
        {
            msg.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
            msg.cause = *p_cause;
        }

        /* CRITICALITY_DIAGNOSTICS*/
        /* Fill criticality_diagnostics */
        if ( (criticality_diagnostics_present == RRC_TRUE) && 
                (PNULL != p_criticality_diagnostics))
        {

            msg.bitmask |= 
                RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;

            msg.criticality_diagnostics =*p_criticality_diagnostics;
        }

        /* Encode message */
        result = rrc_uecc_x2ap_error_indication_intrl_enc(&asn1_ctx,
                &p_sig_msg_req->message_buff_p[0],&p_sig_msg_req->message_buff_size,
                &msg);
        if (RRC_SUCCESS != result)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name,RRC_ERROR,
                    "[%s][x2ap_ErrorIndication] Build failure",__FUNCTION__);
            break;
        }
        else
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name,RRC_INFO,
                    "[%s][x2ap_ErrorIndication] Build Success", __FUNCTION__);

            /* Generating Protocol Event LOCAL_X2_ERROR_INDICATION*/
            encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                            encoded_msg_len));
                rrc_uecc_fill_protocol_event_params(p_ue_context,
                        p_event_header,
                        (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len),
                        LOCAL_X2_ERROR_INDICATION,
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)&p_sig_msg_req->message_buff_p[0],
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
            
            /* Generating Protocol Event LOCAL_EVENT_X2_ERROR_INDICATION*/
            p_event_x2_err_ind = rrc_mem_get(sizeof(LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT));
            if (PNULL != p_event_x2_err_ind)
            {
                memset_wrapper(p_event_x2_err_ind, RRC_NULL, 
                        sizeof(LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT));
                p_event_x2_err_ind->header.length = sizeof(LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT);
                p_event_x2_err_ind->header.event_id = LOCAL_EVENT_X2_ERROR_INDICATION;

                p_event_x2_err_ind->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;

                if (msg.bitmask & RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT)
                {
                    p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE_GROUP = 
                        (U32)(msg.cause.t);

			/* CSR 51448 START */
                    switch(p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE_GROUP)
                    {
                        case T_x2ap_Cause_radioNetwork:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE = 
                                msg.cause.u.radioNetwork;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    (U32)(msg.cause.t), msg.cause.u.radioNetwork);
                            break;

                        case T_x2ap_Cause_transport:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                                msg.cause.u.transport;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    (U32)(msg.cause.t), msg.cause.u.transport);
                            break;
                        case T_x2ap_Cause_protocol:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                                msg.cause.u.protocol;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    (U32)(msg.cause.t), msg.cause.u.protocol);
                            break;
                        case T_x2ap_Cause_misc:
                            p_event_x2_err_ind->EVENT_PARAM_3GPP_CAUSE =
                                msg.cause.u.misc;
                            ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_X2AP_ERROR_INDICATION_ALARM_ID, 
                                    NOTIFICATION, "cause type = %d\n and cause value = %d",
                                    (U32)(msg.cause.t), msg.cause.u.misc);
                            break;
                    }
			/* CSR 51448 STOP */
                }
                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_x2_err_ind);
            }
        }

    } while (0);
    
    /* TRACE_changes_start */
    uecc_x2ap_ue_assoc_msg_max_trace_handler(p_ue_context, &p_sig_msg_req->message_buff_p[0],
                                       p_sig_msg_req->message_buff_size, "Error Indication", 1);
    /* TRACE_changes_start */

    if (RRC_SUCCESS == result)
    {
        /* Send message to X2AP  */
        result = uecc_rrc_send_internal_msg(
                RRC_X2AP_MODULE_ID,
                X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                      p_sig_msg_req->message_buff_size),
                p_message);

        if(RRC_FAILURE == result)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name,RRC_INFO,
            "[%s]-[X2AP PROTOCOL] ERROR INDICATION SENT\n", __FUNCTION__);
        }
    }
    else
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                facility_name,RRC_WARNING,
            "Unable to encode the X2AP ERROR Indication message");
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);


    return result;
}   

/****************************************************************************
 * Function Name  : x2ap_build_and_send_conn_setup_reconfig_rlf_report
 * Inputs         : uecc_ue_context_t
 * Outputs        :
 * Returns        : rrc_return_et
 * Description    : This function  build and send conn_setup_reconfig_rlf_report
 *                  to x2ap module.
 ***************************************************************************/

rrc_return_et x2ap_build_and_send_conn_setup_reconfig_rlf_report(
                uecc_ue_context_t *p_ue_context)
{
    U8 msg_ind_buf[sizeof(x2ap_ue_reestab_conn_setup_reconfig_t)+X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_reestab_conn_setup_reconfig_t *p_reestab_conn_setup_reconfig = 
        (x2ap_ue_reestab_conn_setup_reconfig_t *)msg_ind_buf;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_ue_context);

    memset_wrapper(msg_ind_buf, 0, sizeof(x2ap_ue_reestab_conn_setup_reconfig_t)+X2AP_MAX_ASN1_BUF_LEN);

    if (p_ue_context->rlf_proc_flags.rlf_report.failedPCellId_r10.u.pci_arfcn_r10 !=PNULL)
    {
        /* Filling reestab_conn_setup_reconfig message */
        p_reestab_conn_setup_reconfig->bitmask = 0;
        p_reestab_conn_setup_reconfig->cell_index = p_ue_context->cell_index;
        p_reestab_conn_setup_reconfig->crnti      = 0;
        p_reestab_conn_setup_reconfig->pci = p_ue_context->rlf_proc_flags.rlf_report.failedPCellId_r10.u.pci_arfcn_r10->physCellId_r10;
        p_reestab_conn_setup_reconfig->short_mac_i = 0;  

        if(p_ue_context->rlf_proc_flags.m.rlf_report_present == 1)
        {
            p_reestab_conn_setup_reconfig->bitmask |= X2AP_RLF_REPORT_CONTAINER_PRESENT;
            p_reestab_conn_setup_reconfig->bitmask |= X2AP_RLF_CONN_SETUP_INDICATOR;

            if (RRC_FAILURE == uecc_x2ap_encode_RLF_Report(p_ue_context,
                    &p_reestab_conn_setup_reconfig->rlf_report.message_buff_p[0],
                    &p_reestab_conn_setup_reconfig->rlf_report.message_buff_size))
            {
                p_reestab_conn_setup_reconfig->bitmask ^= X2AP_RLF_REPORT_CONTAINER_PRESENT;
            }
        }

        /* Send Reestab Ind to X2AP Module */
        if(RRC_FAILURE ==  uecc_rrc_send_internal_msg(
                    RRC_X2AP_MODULE_ID,
                    X2AP_UE_REESTABLISHMENT_CONN_SETUP_RECONFIGURE,
                    (U16)(sizeof(x2ap_ue_reestab_conn_setup_reconfig_t) + 
                        p_reestab_conn_setup_reconfig->rlf_report.message_buff_size),
                    msg_ind_buf))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,
                        RRC_ERROR,
                        "Sending x2ap_build_and_send_rlf_report failed.");
            return RRC_FAILURE;
        }
    }
    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_ERROR, "PCI ARCFN IS NULL");
        return RRC_FAILURE;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_p_rrc_trace_for_cause
 *
 *
 *   DESCRIPTION:
 *
 *   RETURNS:
 *       void
 *
 ******************************************************************************/
void uecc_x2ap_p_rrc_trace_for_cause(
        uecc_ue_context_t    *p_ue_context,
        x2ap_Cause           cause)
{
    /* SPR 15783 Start */
	    	/*SPR 15896 Fix Start*/
    P_RRC_TRACE (p_ue_context, set_all_trace_bitmask(), X2, "\n <ieGroup name=\"cause\">");
    P_RRC_TRACE(p_ue_context,set_all_trace_bitmask(), X2,"\n <ie name=\"type\">%d</ie>",cause.t - 1);
    switch(cause.t)
    {
         case T_x2ap_Cause_radioNetwork: 
         P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), X2,"\n <ie name=\"value\">%d</ie>",cause.u.radioNetwork);
         break;

         case T_x2ap_Cause_transport: 
         P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), X2,"\n <ie name=\"value\">%d</ie>",cause.u.transport);
         break;

         case T_x2ap_Cause_protocol: 
         P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), X2,"\n <ie name=\"value\">%d</ie>",cause.u.protocol);
         break; 

         case T_x2ap_Cause_misc:
         P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), X2,"\n <ie name=\"value\">%d</ie>",cause.u.misc);
         break; 
    }
    /* SPR 15783 Stop */
    P_RRC_TRACE (p_ue_context, set_all_trace_bitmask(), X2,"\n </ieGroup> ");
	    	/*SPR 15896 Fix Stop*/
}


/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_trace_handover_request
 *
 *
 *   DESCRIPTION:
 *
 *   RETURNS:
 *       void
 *
 ******************************************************************************/
void uecc_x2ap_trace_handover_request(
        uecc_ue_context_t    *p_ue_context,
        rrc_x2ap_handover_request_t *p_msg,
        U32                  encoded_msg_len,
        U8                   *buffer,
        U8                   send_or_recv)
{
    OSRTDListNode  *p_node_msg = PNULL;
    OSRTDListNode  *p_node_info = PNULL;
    x2ap_E_RABs_ToBeSetup_List_element* p_erab_elem = PNULL;
    x2ap_LastVisitedCell_Item *p_info = PNULL;
    /*SPR 15896 Fix Start*/
    /*Code Removed*/
    /*SPR 15896 Fix Stop*/

    /* Is trace activated */
	    	/*SPR 15896 Fix Start*/
    if (p_ue_context->m.traceActivated)
    {
	    /*SPR 15875 Fix Start*/
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_MME_INITIATE) &&
				    (p_ue_context->p_ue_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_MME,"\n <msg function=\"X2AP\" name=\"Handover Request\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_trace_activation_info->rawtime));
		    }
		    if((p_ue_context->m.traceActivated & RRC_TRACE_ACTIVATION_CELL_INITIATE) &&
				    (p_ue_context->p_ue_cell_trace_activation_info != PNULL) &&
				    (p_ue_context->p_ue_cell_trace_activation_info->interfaces_to_trace & X2))
		    {
                            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), GEN_CELL,"\n <msg function=\"X2AP\" name=\"Handover Request\" changeTime=\"%.3f\" vendorSpecific=\"false\">",
					    difftime_wrapper(p_ue_context->p_ue_cell_trace_activation_info->rawtime));
		    }
	    	/*SPR 15896 Fix Stop*/
	    /*SPR 15875 Fix Stop*/
        /* X2_TRACE fix */
        if (0 == send_or_recv)
        {
	    	/*SPR 15896 Fix Start*/
            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), X2,"\n <initiator type=\"ENB\">%s</initiator>",
                p_ue_context->p_gb_context->x2ap_ctx.
                x2ap_link_info[p_ue_context->x2ap_context.peer_enodeb_id].enb_comm_info.ip_addr[0].ip_addr);
	    	/*SPR 15896 Fix Stop*/
        }
        else
        {
	    	/*SPR 15896 Fix Start*/
            P_RRC_TRACE(p_ue_context, set_all_trace_bitmask(), X2,"\n <target type=\"ENB\">%s</target>",
                p_ue_context->p_gb_context->x2ap_ctx.
                x2ap_link_info[p_ue_context->x2ap_context.peer_enodeb_id].enb_comm_info.ip_addr[0].ip_addr);
	    	/*SPR 15896 Fix Stop*/
        }
        /* X2_TRACE fix */

	    	/*SPR 15896 Fix Start*/
        rrc_trace_max_file_write(p_ue_context,X2AP_HANDOVER_REQUEST,encoded_msg_len, buffer, X2);


            /* Cause */
            uecc_x2ap_p_rrc_trace_for_cause(p_ue_context,p_msg->cause);

            /* Target ID */
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"Target Cell_ID\">");
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"PLMN_Identity\">%02x%02x%02x</ie>  ", 
                    p_msg->target_cell_id.pLMN_Identity.data[0],p_msg->target_cell_id.pLMN_Identity.data[1], 
                    p_msg->target_cell_id.pLMN_Identity.data[2]);

            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"eUTRAN_Cell_ID\">%02x%02x%02x%x</ie>  ", 
                    p_msg->target_cell_id.eUTRANcellIdentifier.data[0], p_msg->target_cell_id.eUTRANcellIdentifier.data[1],
                    p_msg->target_cell_id.eUTRANcellIdentifier.data[2], p_msg->target_cell_id.eUTRANcellIdentifier.data[3]>>4);
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup> ");

            /* GUMMEI */
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"GUMMEI\">");
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"GU Group_ID\">");
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"PLMN_Identity\">%02x%02x%02x</ie>  ", 
                    p_msg->gummei.gU_Group_ID.pLMN_Identity.data[0], p_msg->gummei.gU_Group_ID.pLMN_Identity.data[1],
                    p_msg->gummei.gU_Group_ID.pLMN_Identity.data[2]);
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"MME_Group_ID\">%02x%02x</ie>  ",
                    p_msg->gummei.gU_Group_ID.mME_Group_ID.data[0], p_msg->gummei.gU_Group_ID.mME_Group_ID.data[1]);
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup> ");

            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"MME Code\">%02x</ie>  ",
                    p_msg->gummei.mME_Code.data[0]);
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup> ");

            /* UE_Context_Information */
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"UE_Context_Information\">");
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"E-RABs_ToBeSetup_List\">");
            for (p_node_msg = p_msg->rrc_x2ap_ue_ctx_info.ue_context_info.e_RABs_ToBeSetup_List.head; PNULL!=p_node_msg;
                    p_node_msg=p_node_msg->next)
            {
                p_erab_elem=(x2ap_E_RABs_ToBeSetup_List_element*)p_node_msg->data;
                if ( (p_erab_elem == PNULL) || (p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1 == PNULL))
                {
                    continue;
                }

                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"E-RABs_ToBeSetup_Item\">");
                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"E-RAB id\">%d</ie>  ", 
                        p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_ID);

                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"E-RAB Level-QoS Parameters\">");
                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"QCI\">%d</ie>  ", 
                        p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.qCI);
                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"Allocation Retenion\">");
                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"Priority Level\">%d</ie>  ",
                        p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.
                        allocationAndRetentionPriority.priorityLevel);
                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"Premeption Capability\">%d</ie>  ",
                        p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.
                        allocationAndRetentionPriority.pre_emptionCapability);
                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"Premeption Venerability\">%d</ie>  ",
                        p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.
                        allocationAndRetentionPriority.pre_emptionVulnerability);
                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup> "); /* Allocation Retenion group */

                if (p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->
                        e_RAB_Level_QoS_Parameters.m.gbrQosInformationPresent == 1)
                {
                    P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"GBR_QosInformation\">");
                    P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"Max Bitrate DL\">%u</ie>  ",
                            p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.
                            gbrQosInformation.e_RAB_MaximumBitrateDL);
                    P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"Max Bitrate UL\">%u</ie>  ",
                            p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.
                            gbrQosInformation.e_RAB_MaximumBitrateUL);
                    P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"Guranteed Bitrate DL\">%u</ie>  ",
                            p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.
                            gbrQosInformation.e_RAB_GuaranteedBitrateDL);
                    P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"Guranteed Bitrate UL\">%u</ie>  ",
                            p_erab_elem->value.u._x2ap_E_RABs_ToBeSetup_ItemIEs_1->e_RAB_Level_QoS_Parameters.
                            gbrQosInformation.e_RAB_GuaranteedBitrateUL);
                    P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup> ");  /* GBR_QosInformation */
                }
                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup> ");  /* E-RAB Level-QoS Parameters group */
                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup> ");  /* E-RABs_ToBeSetup_Item group */
            }
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup> ");  /* E-RABs_ToBeSetup_List group */
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup> ");  /* UE-Context-Information group */


            /* UE-History-Information */
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"UE-History-Information\">");
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"LastVisitied-CellList\">");
            for (p_node_info = p_msg->ue_history_info.head; PNULL!=p_node_info;
                    p_node_info=p_node_info->next)
            {
                p_info=(x2ap_LastVisitedCell_Item*)p_node_info->data;
                if(p_info == PNULL)
                {
                    continue;
                }
                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"LastVisitied-CellInformation\">");
                if(T_x2ap_LastVisitedCell_Item_e_UTRAN_Cell == p_info->t)
                {
                    if(p_info->u.e_UTRAN_Cell != PNULL)
                    {
                        P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"LastVisited-EUTRANCellInformation\">");
                        P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ieGroup name=\"Global Cell_ID\">");
                        P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"PLMN_Identity\">%02x%02x%02x</ie>  ",
                                p_info->u.e_UTRAN_Cell->global_Cell_ID.pLMN_Identity.data[0], 
                                p_info->u.e_UTRAN_Cell->global_Cell_ID.pLMN_Identity.data[1],
                                p_info->u.e_UTRAN_Cell->global_Cell_ID.pLMN_Identity.data[2]);
                        P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(),X2, "\n <ie name=\"eUTRAN_Cell_ID\">%02x%02x%02x%x</ie>  ",
                                p_info->u.e_UTRAN_Cell->global_Cell_ID.eUTRANcellIdentifier.data[0],
                                p_info->u.e_UTRAN_Cell->global_Cell_ID.eUTRANcellIdentifier.data[1],
                                p_info->u.e_UTRAN_Cell->global_Cell_ID.eUTRANcellIdentifier.data[2],
                                p_info->u.e_UTRAN_Cell->global_Cell_ID.eUTRANcellIdentifier.data[3]>>4);
                        P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(),X2,"\n </ieGroup> ");

                        P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(),X2, "\n <ie name=\"Cell_Type Cell_Size\">%d</ie>  ",
                                p_info->u.e_UTRAN_Cell->cellType.cell_Size);
                        P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(),X2, "\n <ie name=\"Time_UE_StayedInCell\">%d</ie>  ",
                                p_info->u.e_UTRAN_Cell->time_UE_StayedInCell);
                        P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(),X2,"\n </ieGroup> ");
                    }
                }
                else if(T_x2ap_LastVisitedCell_Item_uTRAN_Cell == p_info->t)
                {
                    if(p_info->u.uTRAN_Cell != PNULL)
                    {
                        P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"LastVisitedUTRANCellInformation\">%s</ie>  ",
                                p_info->u.uTRAN_Cell->data);
                    }
                }
                else if(T_x2ap_LastVisitedCell_Item_gERAN_Cell == p_info->t)
                {
                    P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2, "\n <ie name=\"LastVisitedGERANCellInformation\"></ie>  ");
                }
                P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup> "); /* LastVisitied-CellInformation group */
            }
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup> "); /* LastVisitied-CellList group */
            P_RRC_TRACE (p_ue_context, set_minimum_medium_both_bitmask(), X2,"\n </ieGroup> "); /* UE-History-Information group */
            P_RRC_TRACE(p_ue_context,set_minimum_medium_both_bitmask(), X2,"\n </msg>");
	    	/*SPR 15896 Fix Stop*/
    }
}

/* SPR 16750 Fix Start */
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_handle_src_enb_error_ind
 *
 *   Input: 
 *        uecc_gb_context_t            *p_uecc_gb_context
 *        rrc_x2ap_error_indication_t  *p_msg
 *        U8                           peer_enodeb_id
 *        U32				           encoded_msg_len
 *        U8                           *buffer
 *   Outputs : None
 *   DESCRIPTION:
 *       This function processes X2AP Error Indication of the UEs acting as 
 *       source eNodeb i.e. for messages X2 Handover Request, X2 Handover Cancel and 
 *       X2 SN Status Transfer. It processes the error indication for all the UE contexts 
 *       matching the old eNB UE X2AP Id and new eNB UE X2AP Id received in the 
 *       error indication message.
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void uecc_x2ap_handle_src_enb_error_ind
(
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 rrc_x2ap_error_indication_t     *p_msg,  /* X2AP Error Indication Message */
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 U32				 encoded_msg_len,     /* Encoded Message Length */
 U8                  *buffer
)
{
    uecc_ue_context_t               *p_ue_context = PNULL;
    uecc_ue_context_t               *p_second_ue_context = PNULL;
    rrc_bool_et                     rrc_get_second_ue_ctx = RRC_FALSE;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_ASSERT(PNULL!=p_msg);

    if (p_msg->bitmask & RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT)
    {
        p_ue_context= uecc_ue_ctx_get_by_x2ap_id_src_enb(p_uecc_gb_context,
                peer_enodeb_id, p_msg->old_enb_ue_x2ap_id, OLD_X2AP_UE_ID);
        if ((PNULL != p_ue_context) && (RRC_NULL == p_ue_context->
                                   x2ap_context.m.error_indication_ongoing))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "[x2ap_ErrorIndication] push event - "
                    "UECC_EV_X2AP_ERROR_INDICATION for UE Index [%d] matching old ENB UE X2 ID %u",
                    p_ue_context->ue_index, p_msg->old_enb_ue_x2ap_id);

            uecc_x2ap_ue_assoc_msg_max_trace_handler(p_ue_context, 
                    buffer, encoded_msg_len, "Error Indication", 0);

            p_ue_context->x2ap_context.m.error_indication_ongoing = 1;
            uecc_fsm_process_event(
                    p_ue_context,
                    UECC_EV_X2AP_ERROR_INDICATION,
                    p_msg);
        }
    }
    if ((PNULL != p_ue_context) && 
            (p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present) &&
            (p_msg->bitmask & RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT) &&
            (p_ue_context->x2ap_context.new_enb_ue_x2ap_id !=
             p_msg->new_enb_ue_x2ap_id))
    {
        rrc_get_second_ue_ctx = RRC_TRUE;
    }
    else if (p_msg->bitmask & RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT)
    {
        rrc_get_second_ue_ctx = RRC_TRUE;
    }
    if (RRC_TRUE == rrc_get_second_ue_ctx)
    {
        p_second_ue_context = 
            uecc_ue_ctx_get_by_x2ap_id_src_enb(p_uecc_gb_context,
                    peer_enodeb_id, p_msg->new_enb_ue_x2ap_id, NEW_X2AP_UE_ID);
        if ((PNULL != p_second_ue_context) && (RRC_NULL == p_second_ue_context->
                    x2ap_context.m.error_indication_ongoing))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "[x2ap_ErrorIndication] push event - "
                    "UECC_EV_X2AP_ERROR_INDICATION for UE Index [%d] matching New ENB UE X2 ID %u",
                    p_second_ue_context->ue_index, p_msg->new_enb_ue_x2ap_id);

            uecc_x2ap_ue_assoc_msg_max_trace_handler(p_second_ue_context, 
                    buffer, encoded_msg_len, "Error Indication", 0);

            p_second_ue_context->x2ap_context.m.error_indication_ongoing = 1;
            uecc_fsm_process_event(
                    p_second_ue_context,
                    UECC_EV_X2AP_ERROR_INDICATION,
                    p_msg);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_handle_trg_enb_error_ind
 *
 *   Input: 
 *        uecc_gb_context_t            *p_uecc_gb_context
 *        rrc_x2ap_error_indication_t  *p_msg
 *        U8                           peer_enodeb_id
 *        U32				           encoded_msg_len
 *        U8                           *buffer
 *   Outputs : None
 *   DESCRIPTION:
 *       This function processes X2AP Error Indication of the UEs acting as 
 *       target eNodeb i.e. for messages X2 Handover Request ACK, X2 Handover 
 *       Preparation Failure and X2 UE Context Release. It processes the 
 *       error indication for all the UE contexts matching the old eNB UE X2AP Id 
 *       and new eNB UE X2AP Id received in the error indication message.
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void uecc_x2ap_handle_trg_enb_error_ind
(
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 rrc_x2ap_error_indication_t     *p_msg,  /* X2AP Error Indication Message */
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 U32				 encoded_msg_len,     /* Encoded Message Length */
 U8                  *buffer
)
{
    uecc_ue_context_t               *p_ue_context = PNULL;
    uecc_ue_context_t               *p_second_ue_context = PNULL;
    rrc_bool_et                     rrc_get_second_ue_ctx = RRC_FALSE;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_ASSERT(PNULL!=p_msg);

    if (p_msg->bitmask & RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT)
    {
        p_ue_context= uecc_ue_ctx_get_by_x2ap_id_ho_cancel(p_uecc_gb_context,
                peer_enodeb_id, p_msg->old_enb_ue_x2ap_id, OLD_X2AP_UE_ID);
        if ((PNULL != p_ue_context) && (RRC_NULL == p_ue_context->
                              x2ap_context.m.error_indication_ongoing))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "[x2ap_ErrorIndication] push event - "
                    "UECC_EV_X2AP_ERROR_INDICATION for UE Index [%d] matching old ENB UE X2 ID\n",
                    p_ue_context->ue_index);

            uecc_x2ap_ue_assoc_msg_max_trace_handler(p_ue_context, 
                    buffer, encoded_msg_len, "Error Indication", 0);

            p_ue_context->x2ap_context.m.error_indication_ongoing = 1;
            uecc_fsm_process_event(
                    p_ue_context,
                    UECC_EV_X2AP_ERROR_INDICATION,
                    p_msg);
        }
    }
    if ((PNULL != p_ue_context) && 
            (p_ue_context->x2ap_context.m.new_enb_ue_x2ap_id_present) &&
            (p_msg->bitmask & RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT) &&
            (p_ue_context->x2ap_context.new_enb_ue_x2ap_id !=
             p_msg->new_enb_ue_x2ap_id))
    {
        rrc_get_second_ue_ctx = RRC_TRUE;
    }
    else if (p_msg->bitmask & RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT)
    {
        rrc_get_second_ue_ctx = RRC_TRUE;
    }
    if (RRC_TRUE == rrc_get_second_ue_ctx)
    {
        p_second_ue_context = 
            uecc_ue_ctx_get_by_x2ap_id_ho_cancel(p_uecc_gb_context,
                    peer_enodeb_id, p_msg->new_enb_ue_x2ap_id, NEW_X2AP_UE_ID);
        /*Coverity_CID_92761_START*/
        if ((PNULL != p_second_ue_context) && (RRC_NULL == 
                   p_second_ue_context->x2ap_context.m.error_indication_ongoing))
        /*Coverity_CID_92761_END*/
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "[x2ap_ErrorIndication] push event - "
                    "UECC_EV_X2AP_ERROR_INDICATION for UE Index [%d] matching New ENB UE X2 ID\n",
                    p_second_ue_context->ue_index);

            uecc_x2ap_ue_assoc_msg_max_trace_handler(p_second_ue_context, 
                    buffer, encoded_msg_len, "Error Indication", 0);

            /*Coverity_CID_92760_START*/
            p_second_ue_context->x2ap_context.m.error_indication_ongoing = 1;
            /*Coverity_CID_92760_END*/
            uecc_fsm_process_event(
                    p_second_ue_context,
                    UECC_EV_X2AP_ERROR_INDICATION,
                    p_msg);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_handle_unknown_error_ind
 *
 *   Input: 
 *        uecc_gb_context_t            *p_uecc_gb_context
 *        rrc_x2ap_error_indication_t  *p_msg
 *        U8                           peer_enodeb_id
 *        U32				           encoded_msg_len
 *        U8                           *buffer
 *   Outputs : None
 *   DESCRIPTION:
 *       This function processes X2AP Error Indication of the UEs whose 
 *       state is unknow i.e. whether they are acting as source eNB or 
 *       as target eNB. It processes the error indication for all the UE contexts 
 *       matching the old eNB UE X2AP Id and new eNB UE X2AP Id received in the 
 *       error indication message.
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void uecc_x2ap_handle_unknown_error_ind
(
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 rrc_x2ap_error_indication_t     *p_msg,  /* X2AP Error Indication Message */
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 U32				 encoded_msg_len,     /* Encoded Message Length */
 U8                  *buffer
)
{
    U16                  index_counter = RRC_NULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_ASSERT(PNULL!=p_msg);

    if (p_msg->bitmask & RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT)
    {
        for (index_counter = 0;
            index_counter < p_uecc_gb_context->total_ue_supported;
            index_counter++)
        {
            if ((PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter]) &&
                  (PNULL !=  p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     ho_info.p_ho_info) && (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                      x2ap_context.old_enb_ue_x2ap_id == p_msg->old_enb_ue_x2ap_id)  &&
                    (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                      x2ap_context.peer_enodeb_id == peer_enodeb_id) &&
                     (RRC_NULL == p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                               x2ap_context.m.error_indication_ongoing))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "[x2ap_ErrorIndication] push event - "
                    "UECC_EV_X2AP_ERROR_INDICATION for UE Index [%d] matching old ENB UE X2 ID\n",
                    p_uecc_gb_context->ue_db.ue_contexts[index_counter]->ue_index);

                uecc_x2ap_ue_assoc_msg_max_trace_handler(p_uecc_gb_context->ue_db.ue_contexts[index_counter], 
                    buffer, encoded_msg_len, "Error Indication", 0);
    
                p_uecc_gb_context->ue_db.ue_contexts[index_counter]->x2ap_context.
                                                      m.error_indication_ongoing = 1;

                uecc_fsm_process_event(
                    p_uecc_gb_context->ue_db.ue_contexts[index_counter],
                    UECC_EV_X2AP_ERROR_INDICATION,
                    p_msg);
            }
        }
    }
    if (p_msg->bitmask & RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT)
    {
        for (index_counter = 0;
            index_counter < p_uecc_gb_context->total_ue_supported;
            index_counter++)
        {
            if ((PNULL != p_uecc_gb_context->ue_db.ue_contexts[index_counter]) &&
                  (PNULL !=  p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                     ho_info.p_ho_info) && (p_uecc_gb_context->ue_db.
                     ue_contexts[index_counter]->x2ap_context.m.new_enb_ue_x2ap_id_present) &&
                     (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                      x2ap_context.new_enb_ue_x2ap_id == p_msg->new_enb_ue_x2ap_id)  &&
                    (p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                      x2ap_context.peer_enodeb_id == peer_enodeb_id) &&
                    (RRC_NULL == p_uecc_gb_context->ue_db.ue_contexts[index_counter]->
                       x2ap_context.m.error_indication_ongoing))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "[x2ap_ErrorIndication] push event - "
                    "UECC_EV_X2AP_ERROR_INDICATION for UE Index [%d] matching new ENB UE X2 ID\n",
                    p_uecc_gb_context->ue_db.ue_contexts[index_counter]->ue_index);

                uecc_x2ap_ue_assoc_msg_max_trace_handler(p_uecc_gb_context->ue_db.ue_contexts[index_counter], 
                    buffer, encoded_msg_len, "Error Indication", 0);

                p_uecc_gb_context->ue_db.ue_contexts[index_counter]->x2ap_context.
                                                      m.error_indication_ongoing = 1;

                uecc_fsm_process_event(
                    p_uecc_gb_context->ue_db.ue_contexts[index_counter],
                    UECC_EV_X2AP_ERROR_INDICATION,
                    p_msg);
            }
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/* SPR 16750 Fix Stop */

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_update_nr_nbour_info
 *
 *   INPUTS        : void *p_api
 *                   uecc_gb_context_t *p_uecc_gb_context
 *   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function processes X2AP_UPDATE_NR_NBOUR_INFO
 *
 *   RETURNS:
 *       None
 ******************************************************************************/
void uecc_x2ap_process_update_nr_nbour_info(
        void *p_api,
        uecc_gb_context_t *p_uecc_gb_context
        )
{
    x2ap_nr_neigh_info_t * p_x2ap_nr_nbour_info = PNULL;
    U8 nr_pci  = RRC_NULL;
    U16 buf_size = 0;
    U8 peer_enodeb_id, serving_cell_index = 0;
    U16 num_served_cells = 0;
    void* p_nr_srvd_cell_info = PNULL;

    RRC_ASSERT(PNULL!=p_api);
    RRC_ASSERT(PNULL!=p_uecc_gb_context);

   p_x2ap_nr_nbour_info =
        (x2ap_nr_neigh_info_t *)((U8*)p_api + RRC_API_HEADER_SIZE);	

    buf_size = rrc_get_api_buf_size(p_api);
    /* Precondition - p_api should point to right message */
    if (buf_size < RRC_API_HEADER_SIZE + sizeof(x2ap_nr_neigh_info_t))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                p_uecc_gb_context->facility_name, RRC_WARNING,
                "[X2AP_UPDATE_NR_NBOUR_INFO] "
                "Wrong message size - ignore.");

        RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
        return;
    }
	
    rrc_return_et retVal = RRC_FAILURE;

    for(peer_enodeb_id = 0; peer_enodeb_id < MAX_PEER_ENB; peer_enodeb_id ++)
    {
        if(RRC_TRUE == 
                p_uecc_gb_context->x2ap_ctx.x2ap_link_info[peer_enodeb_id].connection_status)
        {

            if( PNULL != (p_nr_srvd_cell_info = p_uecc_gb_context->x2ap_ctx.x2ap_link_info[peer_enodeb_id].p_nr_srvd_cell_info))
            {
                num_served_cells = p_uecc_gb_context->x2ap_ctx.x2ap_link_info[peer_enodeb_id].num_served_cells;

                for(serving_cell_index = 0 ;serving_cell_index < num_served_cells; serving_cell_index ++)
                {
                    if (!memcmp_wrapper(p_x2ap_nr_nbour_info->nr_cell_id.nr_cell_id,
                                ((x2ap_srvd_nr_cell_info_t*)((U8*)p_nr_srvd_cell_info +
                                 (sizeof(x2ap_srvd_nr_cell_info_t)* serving_cell_index)))->
                                nr_cell_id.nr_cell_id,
                                NR_CELL_ID_OCTET_SIZE*sizeof(U8)))
                    {
                            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_INFO,"Peer ENodeB Id and Serving Cell Index for Given NRCGI is %d and %d\n",
                            peer_enodeb_id,serving_cell_index);
                            retVal  =  RRC_SUCCESS;
                            break;
                    }
					
                }

                if(retVal  ==  RRC_SUCCESS)
                {
                    break;
                }
            }
        }
    }
	
    if(MAX_PEER_ENB == peer_enodeb_id)
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,"No serving cell info present in peer eNodeB Context"
                "ECGI value received is invalid");
    }
    else
    {

        nr_pci = P_SRVD_NR_CELL_INFO(p_uecc_gb_context->x2ap_ctx.
            x2ap_link_info[peer_enodeb_id].p_nr_srvd_cell_info,
            serving_cell_index)->nr_pci;

        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
            p_uecc_gb_context->facility_name,
            RRC_INFO,"For Peer enodeb id %d serving_cell_index %d Current NRPCI value is %d and Updated NRPCI value is %d \n",
            peer_enodeb_id,serving_cell_index,nr_pci,p_x2ap_nr_nbour_info->nr_pci);

        P_SRVD_NR_CELL_INFO(p_uecc_gb_context->x2ap_ctx.
            x2ap_link_info[peer_enodeb_id].p_nr_srvd_cell_info,
            serving_cell_index)->nr_pci = p_x2ap_nr_nbour_info->nr_pci;
    }
			
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);

}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_sgnb_addition_req_reject
 *
 *   INPUTS        : OSCTXT*            p_asn1_ctx
 *                   x2ap_X2AP_PDU       *p_x2ap_pdu
 *                   uecc_gb_context_t *p_uecc_gb_context
 *                   U8                  peer_enodeb_id
 *   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function processes X2AP SgNB Addition Request Reject message
 *
 *   RETURNS:     None
 *
 ******************************************************************************/
void uecc_x2ap_process_sgnb_addition_req_reject
( 
    OSCTXT*             p_asn1_ctx,         /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu,        /* pointer to X2AP PDU */
    uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,     /* Peer EnodeB ID */
    U8                  *buffer
)
{
	uecc_ue_context_t                   *p_ue_context = PNULL;
    rrc_x2ap_sgnb_addition_req_rej_t   msg;

	RRC_ASSERT(PNULL!=p_x2ap_pdu);
	RRC_ASSERT(PNULL!=p_x2ap_pdu->u.unsuccessfulOutcome);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

	memset_wrapper(&msg, 0, sizeof(rrc_x2ap_sgnb_addition_req_rej_t));
	do
	{
		if (PNULL == 
		        p_x2ap_pdu->u.unsuccessfulOutcome->value.u.sgNBAdditionPreparation)
		{
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name, RRC_WARNING,
					"[X2AP SgNB Addition Request Reject] sgNBAdditionPreparation field is PNULL");
			break;
		}

        if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
        {
            p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                    p_uecc_gb_context,
                    peer_enodeb_id,msg.menb_ue_x2ap_id,MENB_X2AP_UE_ID);
        }
/*Error_Handling_changes*/
            /*Check for API-ID missmatch*/
            if(RRC_FAILURE == check_for_api_id_mismatch(p_ue_context,
            	p_uecc_gb_context,
            	first_returned_message,
            	p_asn1_ctx,
            	msg.menb_ue_x2ap_id,
            	msg.sgnb_ue_x2ap_id,
            	p_x2ap_pdu->u.successfulOutcome->procedureCode,
            	p_x2ap_pdu->t,
            	peer_enodeb_id,
            	&msg
            	))
            {
 				RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    	facility_name, RRC_WARNING, 
                    	"SgNB Addition Request Ack Error : UECC_EV_X2AP_ERROR_INDICATION");

            	break;
            }
/*Error_Handling_changes*/

		if (PNULL==p_ue_context )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR, " SgNB Addition Request Reject "
                    "p_ue_context is PNULL!");
            break;
    
        }
        else
        {
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name, RRC_INFO, 
					"[X2AP SgNB Addition Request Reject] UE context found");
        }

        if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
					p_uecc_gb_context,p_ue_context->cell_index))
		{
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name, RRC_WARNING,"cell not available");
			RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
			return;
        }
#if 0
        /* Decode message */ 
        if( X2AP_FAILURE == x2ap_internal_decode_sgnb_addition_request_reject(
                    p_asn1_ctx,
                    p_x2ap_pdu->u.unsuccessfulOutcome->value.u.sgNBAdditionPreparation,
                    &msg,
                    peer_enodeb_id))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, " SgNB Addition Request Reject "
                    "Decoding Failure");
            break;
        }
        else
#endif
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, " SgNB Addition Request Reject "
                    "Decoding Success");
            /* Call      event handler *
			 * and pass API data to FSM. */
			/* WARNING. The uecc_event_queue_push_event can't be used here because
			 * the stack variable is used for storing the API data
			 * */
			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
					facility_name, RRC_INFO, 
					"SgNB Addition Request Reject :"
                    " push event UECC_EV_X2AP_SGNB_ADDITION_REQ_REJECT");
			uecc_fsm_process_event(
					p_ue_context,
					UECC_EV_X2AP_SGNB_ADDITION_REQ_REJECT,
					&msg);
        }
    } while(RRC_NULL);
	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/*****************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_x2ap_sgnb_add_e_rab_admitted_list
 *
 *   DESCRIPTION:
 *       This function builds x2ap_E_RABs_ToBeAdded_SgNBAddReqList 
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ****************************************************************************/
static rrc_return_et uecc_x2ap_build_x2ap_sgnb_add_e_rab_admitted_list(
        uecc_ue_context_t                       *p_ue_context,
        OSCTXT                                  *p_asn1_ctx,
        x2ap_E_RABs_ToBeAdded_SgNBAddReqList    *p_e_rab_admitted_list)
{
    x2ap_E_RABs_ToBeAdded_SgNBAddReqList_element    *p_elem                 = PNULL;
    x2ap_E_RABs_ToBeAdded_SgNBAddReq_Item           *p_elem_item            = PNULL;

    OSRTDListNode                                   *p_node                 = PNULL;
    U8                                              erab_index              = RRC_NULL;
    U8                                              drb_count               = RRC_NULL;
    U8                                              erab_id                 = RRC_NULL;
    U8                                              erab_count              = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_e_rab_admitted_list);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    asn1Init_x2ap_E_RABs_Admitted_List(p_e_rab_admitted_list);

    p_ue_context->endc_sgnb_add_erab_list.counter = RRC_NULL;
    if ( PNULL != p_ue_context->p_curr_proc_data )
    {
        if ( p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                erab_info_data.count )
        {
            erab_count = p_ue_context->p_curr_proc_data->u.
                erab_setup_request_data.erab_info_data.count;
        }
        else if ( p_ue_context->p_curr_proc_data->u.
                rrm_dc_bearer_change_request_data.drb_to_modify_list.drb_count )
        {
            erab_count = p_ue_context->p_curr_proc_data->u.
                rrm_dc_bearer_change_request_data.drb_to_modify_list.drb_count;
        }
        for (erab_index = RRC_NULL; 
                (erab_index < erab_count); erab_index++)
        {
            if ( p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                    erab_info_data.count )
            {
                erab_id = p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                    erab_info_data.erab_info[erab_index].erab_id;
            }
            else if ( p_ue_context->p_curr_proc_data->u.
                    rrm_dc_bearer_change_request_data.drb_to_modify_list.drb_count )
            {
                erab_id = p_ue_context->p_curr_proc_data->u.
                    rrm_dc_bearer_change_request_data.drb_to_modify_list.drb_config[erab_index].erab_id;
            }

            if (PNULL == p_ue_context->p_e_rab_list[erab_id]) 
            {
                continue;
            }
            if(SN_TERMINATED_SCG_BEARER == p_ue_context->p_e_rab_list[
                    erab_id]->dc_bearer_type
                    /* OPTION3X Changes Start */
                    || SN_TERMINATED_SPLIT_BEARER == p_ue_context->p_e_rab_list[
                    erab_id]->dc_bearer_type) 
                /* OPTION3X Changes End */
            {
                rtxDListAllocNodeAndData(p_asn1_ctx,
                        x2ap_E_RABs_ToBeAdded_SgNBAddReqList_element,
                        &p_node,
                        &p_elem);

                if (PNULL == p_node)
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, 
                            RRC_FATAL,"Can't allocate p_node "
                            "MeNB UE X2AP ID [%u] "
                            "Peer eNodeB Id: [%u] "
                            "UE Index: [%u] "
                            "Cell Index: [%u]",
                            p_ue_context->x2ap_context.menb_ue_x2ap_id,
                            p_ue_context->x2ap_context.peer_enodeb_id,
                            p_ue_context->ue_index,
                            p_ue_context->cell_index);

                    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }

                asn1Init_x2ap_E_RABs_ToBeAdded_SgNBAddReqList_element(p_elem);

                p_elem->id = ASN1V_x2ap_id_E_RABs_ToBeAdded_SgNBAddReq_Item;
                p_elem->criticality = x2ap_reject;

                p_elem->value.t = 
                    T122x2ap___x2ap_E_RABs_ToBeAdded_SgNBAddReq_ItemIEs_1;


                /* Allocate Memory */
                p_elem_item = 
                    rtxMemAllocType(p_asn1_ctx,
                            x2ap_E_RABs_ToBeAdded_SgNBAddReq_Item);
                if (PNULL == p_elem_item)
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, 
                            RRC_FATAL,"Can't allocate p_elem_item "
                            "MeNB UE X2AP ID [%u] "
                            "Peer eNodeB Id: [%u] "
                            "UE Index: [%u] "
                            "Cell Index: [%u]",
                            p_ue_context->x2ap_context.menb_ue_x2ap_id,
                            p_ue_context->x2ap_context.peer_enodeb_id,
                            p_ue_context->ue_index,
                            p_ue_context->cell_index);

                    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }
                p_elem->value.u._x2ap_E_RABs_ToBeAdded_SgNBAddReq_ItemIEs_1 =
                    p_elem_item ;

                asn1Init_x2ap_E_RABs_ToBeAdded_SgNBAddReq_Item(p_elem_item);

                /* Fill E-RAB ID */
                p_elem_item->e_RAB_ID = 
                    p_ue_context->p_e_rab_list[erab_id]->e_RAB_ID;

                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                        "[x2ap_SgNBAdditionRequest] ERAB ID [%d] ",
                        p_elem_item->e_RAB_ID);

                /* Fill D-RAB ID */
            	if(PNULL != p_ue_context->p_curr_proc_data &&
                    	UECC_RRM_DC_BEARER_CHANGE_PROC ==
                    	p_ue_context->p_curr_proc_data->t)
            	{
                	RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        	p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                        	"[x2ap_SgNBAdditionRequest] Currnt proc is DC_BEARER_CHANGE");

                	for (drb_count = RRC_ZERO;
                        	drb_count < p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                        	drb_to_modify_list.drb_count;
                        	drb_count++)
                	{
                    	if (p_elem_item->e_RAB_ID ==
                            	p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                            	drb_to_modify_list.drb_config[drb_count].erab_id)
                    	{
                        	p_elem_item->drb_ID =
                            	p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                            	drb_to_modify_list.drb_config[drb_count].new_drb_id;

                        	break;
                    	}
                	}
            	}
            	else
            	{
                p_elem_item->drb_ID = 
                    p_ue_context->p_e_rab_list[erab_id]->drb_config.drb_id;
            	}

                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                        "[x2ap_SgNBAdditionRequest] DRB ID [%d] ",
                        p_elem_item->drb_ID);

                /*Update ENDC resource Config*/
                p_elem_item->en_DC_ResourceConfiguration.pDCPatSgNB = x2ap_present; 
                p_elem_item->en_DC_ResourceConfiguration.mCGresources = x2ap_not_present_1;
                p_elem_item->en_DC_ResourceConfiguration.sCGresources = x2ap_present_2; 

                /* OPTION3X Changes Start */
                if (SN_TERMINATED_SPLIT_BEARER == p_ue_context->p_e_rab_list[
                        erab_id]->dc_bearer_type)
                {
                    p_elem_item->en_DC_ResourceConfiguration.mCGresources = x2ap_present_1;

                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                            "[x2ap_SgNBAdditionRequest] MCG resources are present for drb id[%d] ",
                            p_elem_item->drb_ID);
                }
                else
                {
                    p_elem_item->en_DC_ResourceConfiguration.mCGresources = x2ap_not_present_1;

                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                            "[x2ap_SgNBAdditionRequest] MCG resources are not present for drb id[%d] ",
                            p_elem_item->drb_ID);
                }
                /* OPTION3X Changes End */

                /*update Choice resource Configuration*/
                /*check if pDCPatSgNB in en_DC_ResourceConfiguration ie present*/
                if(x2ap_present == p_elem_item->en_DC_ResourceConfiguration.pDCPatSgNB)
                {
                    p_elem_item->resource_configuration.t = 
                        T_x2ap_E_RABs_ToBeAdded_SgNBAddReq_Item_resource_configuration_sgNBPDCPpresent;

                    /* Allocate Memory */
                    p_elem_item->resource_configuration.u.sgNBPDCPpresent = 
                        rtxMemAllocType(p_asn1_ctx,
                                x2ap_E_RABs_ToBeAdded_SgNBAddReq_Item_SgNBPDCPpresent);
                    if (PNULL ==
                            p_elem_item->resource_configuration.u.sgNBPDCPpresent)
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name, 
                                RRC_FATAL,"Can't allocate p_elem_item->resource_configuration.u.sgNBPDCPpresent "
                                "MeNB UE X2AP ID [%u] "
                                "Peer eNodeB Id: [%u] "
                                "UE Index: [%u] "
                                "Cell Index: [%u]",
                                p_ue_context->x2ap_context.menb_ue_x2ap_id,
                                p_ue_context->x2ap_context.peer_enodeb_id,
                                p_ue_context->ue_index,
                                p_ue_context->cell_index);

                        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                        return RRC_FAILURE;
                    }

                    asn1Init_x2ap_E_RABs_ToBeAdded_SgNBAddReq_Item_SgNBPDCPpresent(
                            p_elem_item->resource_configuration.u.sgNBPDCPpresent);
                    /*update full QOS params*/
                    {
                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                            full_E_RAB_Level_QoS_Parameters.qCI =
                            p_ue_context->p_e_rab_list[erab_id]->\
                            e_RABlevelQoSParameters.qCI;

                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                            full_E_RAB_Level_QoS_Parameters.allocationAndRetentionPriority.
                            priorityLevel =
                            p_ue_context->p_e_rab_list[erab_id]->\
                            e_RABlevelQoSParameters.allocationRetentionPriority.priorityLevel;

                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                            full_E_RAB_Level_QoS_Parameters.allocationAndRetentionPriority.
                            pre_emptionCapability =
                            p_ue_context->p_e_rab_list[erab_id]->\
                            e_RABlevelQoSParameters.allocationRetentionPriority.pre_emptionCapability;

                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                            full_E_RAB_Level_QoS_Parameters.allocationAndRetentionPriority.
                            pre_emptionVulnerability =
                            p_ue_context->p_e_rab_list[erab_id]->\
                            e_RABlevelQoSParameters.allocationRetentionPriority.pre_emptionVulnerability;

                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                            full_E_RAB_Level_QoS_Parameters.allocationAndRetentionPriority.
                            m.iE_ExtensionsPresent = 0;


                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                            full_E_RAB_Level_QoS_Parameters.
                            m.iE_ExtensionsPresent = 0;

                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                            full_E_RAB_Level_QoS_Parameters.
                            m.gbrQosInformationPresent = 0;

                        if (p_ue_context->p_e_rab_list[erab_id]->\
                                e_RABlevelQoSParameters.m.gbrQosInformationPresent)
                        {
                            if ( ( p_ue_context->p_e_rab_list[erab_id]->\
                                e_RABlevelQoSParameters.qCI >= RRC_ONE ) &&
                                ( p_ue_context->p_e_rab_list[erab_id]->\
                                e_RABlevelQoSParameters.qCI <= RRC_FOUR ) )
                            {
                                p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                    full_E_RAB_Level_QoS_Parameters.m.gbrQosInformationPresent = 1;

                                p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                    full_E_RAB_Level_QoS_Parameters.gbrQosInformation.\
                                    e_RAB_MaximumBitrateDL = 
                                    p_ue_context->p_e_rab_list[erab_id]->\
                                    e_RABlevelQoSParameters.gbrQosInformation.e_RAB_MaximumBitrateDL;

                                p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                    full_E_RAB_Level_QoS_Parameters.gbrQosInformation.\
                                    e_RAB_MaximumBitrateUL = 
                                    p_ue_context->p_e_rab_list[erab_id]->\
                                    e_RABlevelQoSParameters.gbrQosInformation.e_RAB_MaximumBitrateUL;

                                p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                    full_E_RAB_Level_QoS_Parameters.gbrQosInformation.\
                                    e_RAB_GuaranteedBitrateDL = 
                                    p_ue_context->p_e_rab_list[erab_id]->\
                                    e_RABlevelQoSParameters.gbrQosInformation.e_RAB_GuaranteedBitrateDL;

                                p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                    full_E_RAB_Level_QoS_Parameters.gbrQosInformation.\
                                    e_RAB_GuaranteedBitrateUL = 
                                    p_ue_context->p_e_rab_list[erab_id]->\
                                    e_RABlevelQoSParameters.gbrQosInformation.e_RAB_GuaranteedBitrateUL;
                            }                    
                        }                    

                    }

                    /*Update max_MCG_admit QOS Params*/
                    p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                        m.max_MCG_admit_E_RAB_Level_QoS_ParametersPresent = 0;

                    /*This IE shall only be present if MCG Resource and SCG Resources IEs in 
                     * EN-DC Resource Configuration IE are set to the value "present(1)", and
                     * GBR QoS Information IE is present in  Full E-RAB Level QoS Parameters IE*/
                        if(x2ap_present_1 == p_elem_item->en_DC_ResourceConfiguration.mCGresources &&
                                x2ap_present_2 == p_elem_item->en_DC_ResourceConfiguration.sCGresources &&
                                RRC_TRUE == p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                full_E_RAB_Level_QoS_Parameters.m.gbrQosInformationPresent)
                        {
                            p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                m.max_MCG_admit_E_RAB_Level_QoS_ParametersPresent = 1;

                            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                    p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                                    "[x2ap_SgNBAdditionRequest] GBR QOS information present for drb id[%d] ",
                                    p_elem_item->drb_ID);

                            /* OPTION3X Changes Start */
                            p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                max_MCG_admit_E_RAB_Level_QoS_Parameters.e_RAB_MaximumBitrateDL =
                                p_ue_context->p_e_rab_list[erab_id]->\
                                e_RABlevelQoSParameters.gbrQosInformation.e_RAB_MaximumBitrateDL;

                            p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                max_MCG_admit_E_RAB_Level_QoS_Parameters.e_RAB_MaximumBitrateUL =
                                p_ue_context->p_e_rab_list[erab_id]->\
                                e_RABlevelQoSParameters.gbrQosInformation.e_RAB_MaximumBitrateUL;

                            p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                max_MCG_admit_E_RAB_Level_QoS_Parameters.e_RAB_GuaranteedBitrateDL =
                                p_ue_context->p_e_rab_list[erab_id]->\
                                e_RABlevelQoSParameters.gbrQosInformation.e_RAB_GuaranteedBitrateDL;

                            p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                max_MCG_admit_E_RAB_Level_QoS_Parameters.e_RAB_GuaranteedBitrateUL =
                                p_ue_context->p_e_rab_list[erab_id]->\
                                e_RABlevelQoSParameters.gbrQosInformation.e_RAB_GuaranteedBitrateUL;

                            p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                max_MCG_admit_E_RAB_Level_QoS_Parameters.m.iE_ExtensionsPresent = RRC_ZERO;
                            /* OPTION3X Changes End */

                        }

                        /*update dL_Forwarding IE - currently not supported*/
                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                            m.dL_ForwardingPresent = 0;

                        /*update meNB_DL_GTP_TEIDatMCG IE - supported only in OPTION3X*/
                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                            m.meNB_DL_GTP_TEIDatMCGPresent = 0;

                        /* OPTION3X Changes Start */
                        if (x2ap_present_1 == p_elem_item->en_DC_ResourceConfiguration.mCGresources &&
                                SN_TERMINATED_SPLIT_BEARER == p_ue_context->p_e_rab_list[erab_id]->dc_bearer_type &&
                                /* Check if UECC_RRM_DC_BEARER_CHANGE_PROC is curr proc Data*/
                                PNULL != p_ue_context->p_curr_proc_data &&
                                UECC_RRM_DC_BEARER_CHANGE_PROC & p_ue_context->p_curr_proc_data->t)
                        {
                            for (drb_count = RRC_ZERO;
                                    drb_count < p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                    drb_to_modify_list.drb_count;
                                    drb_count++)
                            {
                                if ( p_elem_item->e_RAB_ID == 
                                        p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                        drb_to_modify_list.drb_config[drb_count].erab_id)
                                {
                                    /* Check if Self TeID is Present in curr proc Data*/
                                    if (RRC_ONE == p_ue_context->p_curr_proc_data->u.\
                                            rrm_dc_bearer_change_request_data.
                                            drb_to_modify_list.drb_config[drb_count].m.
                                            rrc_x2u_dl_teid_self_updated
                                            &&
                                            RRC_ONE == p_ue_context->p_curr_proc_data->u.\
                                            rrm_dc_bearer_change_request_data.
                                            drb_to_modify_list.drb_config[drb_count].m.
                                            rrc_x2u_dl_trasport_addr_self_updated)
                                    {
                                        /* Update MeNB DL GTP TeID */
                                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                            meNB_DL_GTP_TEIDatMCG.gTP_TEID.numocts = 
                                            sizeof(p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                                    meNB_DL_GTP_TEIDatMCG.gTP_TEID.data);

                                        rrc_pack_U32(
                                                p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                                meNB_DL_GTP_TEIDatMCG.gTP_TEID.data,
                                                &p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                                drb_to_modify_list.drb_config[drb_count].dl_x2u_teid_self);

                                        /* Update MeNB DL transportLayerAddress */
                                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                            meNB_DL_GTP_TEIDatMCG.transportLayerAddress.data =
                                            (OSOCTET*)rtxMemAlloc(p_asn1_ctx, 
                                                    p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                                    drb_to_modify_list.drb_config[drb_count].dl_x2u_transport_layer_addr_self.data_length);

                                        if (PNULL == 
                                                p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                                meNB_DL_GTP_TEIDatMCG.transportLayerAddress.data)
                                        {
                                            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                                    p_ue_context->p_gb_context->facility_name, 
                                                    RRC_ERROR, "ASN malloc failed.");
                                            return RRC_FAILURE;
                                        }

                                        l3_memcpy_wrapper(
                                                (void *)p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                                meNB_DL_GTP_TEIDatMCG.transportLayerAddress.data,
                                                (U8 *)p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                                drb_to_modify_list.drb_config[drb_count].dl_x2u_transport_layer_addr_self.data,
                                                p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                                drb_to_modify_list.drb_config[drb_count].dl_x2u_transport_layer_addr_self.data_length);

                                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                            meNB_DL_GTP_TEIDatMCG.transportLayerAddress.numbits =
                                            p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                            drb_to_modify_list.drb_config[drb_count].dl_x2u_transport_layer_addr_self.data_length * 8;

                                        /* Update the flag to MeNB DL GTP Tunnel At MCG */
                                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                            m.meNB_DL_GTP_TEIDatMCGPresent = 1;

                                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                                p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                                                "[x2ap_SgNBAdditionRequest] Self TEID updated for drb id[%d] ",
                                                p_elem_item->drb_ID);
                                    }
                                    break;
                                }
                            }
                        }
                        /* OPTION3X Changes End */

                        /*update s1_UL_GTPtunnelEndpoint IE*/
                        asn1Copy_x2ap_TransportLayerAddress (p_asn1_ctx,
                                (x2ap_TransportLayerAddress *)&p_ue_context->
                                p_e_rab_list[erab_id]->transportLayerAddress,
                                &p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                s1_UL_GTPtunnelEndpoint.transportLayerAddress);

                        p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                            s1_UL_GTPtunnelEndpoint.gTP_TEID.numocts = 
                            p_ue_context->p_e_rab_list[erab_id]->gTP_TEID.numocts;

                        l3_memcpy_wrapper (p_elem_item->resource_configuration.u.sgNBPDCPpresent->
                                s1_UL_GTPtunnelEndpoint.gTP_TEID.data,
                                p_ue_context->p_e_rab_list[erab_id]->gTP_TEID.data,
                                p_ue_context->p_e_rab_list[erab_id]->gTP_TEID.numocts);

                        /*update x2ap_E_RABs_ToBeAdded_SgNBAddReq_Item_SgNBPDCPpresent_iE_Extensions 
                         * IE - currently not supported*/
                                            p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                                            m.iE_ExtensionsPresent = 0;

                }
                else
                {
                    p_elem_item->resource_configuration.t = 
                        T_x2ap_E_RABs_ToBeAdded_SgNBAddReq_Item_resource_configuration_sgNBPDCPnotpresent;
                }

                rtxDListAppendNode(
                        p_e_rab_admitted_list,
                        p_node);
                p_ue_context->endc_sgnb_add_erab_list.counter++;
            }
        }
    }
    else
    {
    }


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return RRC_SUCCESS;
}

/* Remove when populating nrSecurityCapability from uecc_ctx */
#define MAX_NUM_BITS 16
/******************************************************************************
 *   FUNCTION NAME: rrc_populate_nr_ue_security_capability 
 *
 *   Input:  
 *          uecc_ue_context_t*              p_ue_context
 *          x2ap_NRUESecurityCapabilities*  p_nr_security_cap
 *   Outputs :
 *   DESCRIPTION:
 *       This function fills the security capability 
 *   RETURNS: None
 ******************************************************************************/
    void rrc_populate_nr_ue_security_capability 
(
 uecc_ue_context_t               *p_ue_context,      
 x2ap_NRUESecurityCapabilities   *p_nr_security_cap,
 OSCTXT                          *pctxt
 )
{
    RRC_ASSERT (PNULL != pctxt);
    RRC_ASSERT (PNULL != p_ue_context);
    RRC_ASSERT (PNULL != p_nr_security_cap);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Setting NO IE extension is Present */
    p_nr_security_cap->m.iE_ExtensionsPresent = RRC_NULL;

    /*Setting NULL ciphring*/
    p_nr_security_cap->nRencryptionAlgorithms.numbits = MAX_NUM_BITS;

    p_nr_security_cap->nRencryptionAlgorithms.data =
        (OSOCTET*)rtxMemAlloc(pctxt,
                p_nr_security_cap->nRencryptionAlgorithms.numbits);

    if (PNULL== p_nr_security_cap->nRencryptionAlgorithms.data)
    {
        RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                (p_ue_context->p_gb_context)->facility_name,
                RRC_FATAL, "ASN malloc failed.");
        return;
    }

    /*Setting NULL ciphring*/
    p_nr_security_cap->nRintegrityProtectionAlgorithms.numbits = MAX_NUM_BITS;

    p_nr_security_cap->nRintegrityProtectionAlgorithms.data =
        (OSOCTET*)rtxMemAlloc(pctxt,
                p_nr_security_cap->nRintegrityProtectionAlgorithms.numbits);

    if (PNULL== p_nr_security_cap->nRintegrityProtectionAlgorithms.data)
    {
        RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                (p_ue_context->p_gb_context)->facility_name,
                RRC_FATAL, "ASN malloc failed.");
        return;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}

/******************************************************************************
 *   FUNCTION NAME: rrc_populate_sgnb_securtiy_key 
 *
 *   Input:  
 *          uecc_ue_context_t*              p_ue_context
 *          x2ap_SgNBSecurityKey*           p_sgnb_sec_key
 *   Outputs :
 *   DESCRIPTION:
 *       This function fills the SgNB Security Key 
 *   RETURNS: None
 ******************************************************************************/
    void rrc_populate_sgnb_securtiy_key 
(
 uecc_ue_context_t       *p_ue_context,      
 x2ap_SgNBSecurityKey    *p_sgnb_sec_key
 )
{
    /*initialise SgNB Security key*/
    asn1Init_x2ap_SgNBSecurityKey(p_sgnb_sec_key);

    /*generate SgNB security key using sk_counter and k_enb*/
    rrc_generate_skgnb(p_ue_context->sk_counter_r15,
            &p_sgnb_sec_key->data[0],
            &p_ue_context->security_key.data[0]);

    /*Set SgNB Security Key Size*/
    p_sgnb_sec_key->numbits = SGNB_SEC_KEY_SIZE; 
}

#if 0
/******************************************************************************
 *   FUNCTION NAME: rrc_extract_nr_dc_x2_ids
 *
 *   Input:  
 *          uecc_ue_context_t*            p_ue_context
 *   Outputs :
 *          x2ap_UE_X2AP_ID               *p_menb_ue_x2ap_id
 *          x2ap_UE_X2AP_ID_Extension     *p_menb_ue_x2ap_id_ext
 *   DESCRIPTION:
 *       This function splits 2bytes MeNB X2AP ID & MeNB X2AP ID ext 
 *       from MeNB UE X2AP ID stored in UE Context
 *
 *   RETURNS: None
 *
 ******************************************************************************/
    void rrc_extract_nr_dc_x2_ids 
(
 uecc_ue_context_t          *p_ue_context,      /* UE context */
 x2ap_UE_X2AP_ID            *p_menb_ue_x2ap_id,   /* MeNB UE X2AP ID */
 x2ap_UE_X2AP_ID_Extension  *p_menb_ue_x2ap_id_ext /* MeNB UE X2AP ID EXT*/
 )
{
    *p_menb_ue_x2ap_id_ext = RRC_NULL;

    RRC_ASSERT (PNULL != p_ue_context);

    if (PNULL != p_ue_context)
    {
        /* Last 8 bits stored are MeNB UE X2AP ID */
        *p_menb_ue_x2ap_id = (x2ap_UE_X2AP_ID)(p_ue_context->x2ap_context.
                menb_ue_x2ap_id & RRC_INVALID_UE_INDEX);

        if (p_ue_context->x2ap_context.is_menb_ext_present)
        {
            *p_menb_ue_x2ap_id_ext = (x2ap_UE_X2AP_ID_Extension)((p_ue_context->
                        x2ap_context.menb_ue_x2ap_id >> EXT_BIT ) & 
                    RRC_INVALID_UE_INDEX);
        }
    }
}
#endif
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_drx_info_mcg 
 *   DESCRIPTION  : This function builds nr_rrc_DRX_Info 
 *   RETURNS      : RRC_FAILURE/RRC_SUCCESS
 ******************************************************************************/
static rrc_return_et
uecc_x2ap_build_drx_info_mcg
(
    uecc_ue_context_t               *p_uecc_ue_context,
    nr_rrc_DRX_Info                 *p_asn1_list,
    rrc_drx_config_t                *p_rrm_config_drx_config_mcg,
    OSCTXT                          *p_asn1_ctx
)
{
    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctx);

        RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                (p_uecc_ue_context->p_gb_context)->facility_name,
                RRC_DETAILED, "long drx %d present",p_rrm_config_drx_config_mcg->drx_config_param.long_drx_cycle_start_offset_r15.bitmask);
    switch ( p_rrm_config_drx_config_mcg->drx_config_param.
        long_drx_cycle_start_offset_r15.bitmask )
    {
        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_10_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms10;

            p_asn1_list->drx_LongCycleStartOffset.u.ms10 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_10;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_20_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms20;

            p_asn1_list->drx_LongCycleStartOffset.u.ms20 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_20;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_32_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms32;

            p_asn1_list->drx_LongCycleStartOffset.u.ms32 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_32;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_40_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms40;

            p_asn1_list->drx_LongCycleStartOffset.u.ms40 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_40;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_64_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms64;

            p_asn1_list->drx_LongCycleStartOffset.u.ms64 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_64;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_80_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms80;

            p_asn1_list->drx_LongCycleStartOffset.u.ms80 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_80;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_128_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms128;

            p_asn1_list->drx_LongCycleStartOffset.u.ms128 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_128;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_160_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms160;

            p_asn1_list->drx_LongCycleStartOffset.u.ms160 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_160;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_256_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms256;

            p_asn1_list->drx_LongCycleStartOffset.u.ms256 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_256;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_320_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms320;

            p_asn1_list->drx_LongCycleStartOffset.u.ms320 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_320;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_512_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms512;

            p_asn1_list->drx_LongCycleStartOffset.u.ms512 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_512;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_640_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms640;

            p_asn1_list->drx_LongCycleStartOffset.u.ms640 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_640;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_1024_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms1024;

            p_asn1_list->drx_LongCycleStartOffset.u.ms1024 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_1024;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_1280_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms1280;

            p_asn1_list->drx_LongCycleStartOffset.u.ms1280 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_1280;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_2048_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms2048;

            p_asn1_list->drx_LongCycleStartOffset.u.ms2048 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_2048;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_2560_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms2560;

            p_asn1_list->drx_LongCycleStartOffset.u.ms2560 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_2560;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_5120_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms5120;

            p_asn1_list->drx_LongCycleStartOffset.u.ms5120 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_5120;

            break;
        }

        case RRC_LONG_DRX_CYCLE_START_OFFSET_R15_SF_10240_PRESENT:
        {
            p_asn1_list->drx_LongCycleStartOffset.t =
                T_nr_rrc_DRX_Config_drx_LongCycleStartOffset_ms10240;

            p_asn1_list->drx_LongCycleStartOffset.u.ms10240 =
                p_rrm_config_drx_config_mcg->drx_config_param.
                    long_drx_cycle_start_offset_r15.sf_10240;

            break;
        }
    }
        RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                (p_uecc_ue_context->p_gb_context)->facility_name,
                RRC_DETAILED, "long drx %d present",p_asn1_list->drx_LongCycleStartOffset.t);

    if ( RRC_DRX_CONFIG_SHORT_DRX_PRESENT &
        p_rrm_config_drx_config_mcg->drx_config_param.bitmask )
    {
        RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                (p_uecc_ue_context->p_gb_context)->facility_name,
                RRC_DETAILED, "short drx present");
        p_asn1_list->m.shortDRXPresent = RRC_ONE;

        p_asn1_list->shortDRX.drx_ShortCycle = p_rrm_config_drx_config_mcg->
            drx_config_param.short_drx.short_drx_cycle;

        p_asn1_list->shortDRX.drx_ShortCycleTimer = p_rrm_config_drx_config_mcg->
            drx_config_param.short_drx.short_drx_cycle_timer;
    }

    return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_config_restrict_info_scg 
 *
 *   DESCRIPTION:
 *       This function builds nr_rrc_ConfigRestrictInfoSCG  
 *
 *   RETURNS: RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
static rrc_return_et uecc_x2ap_build_config_restrict_info_scg(
        uecc_ue_context_t               *p_uecc_ue_context,
        nr_rrc_ConfigRestrictInfoSCG    *p_asn1_list,
        config_restrict_info_t          *p_rrm_config_restrict_info_scg,
        OSCTXT                          *p_asn1_ctx
        )
{
    U16    band_comb_count = RRC_NULL;
    U16    band_count = RRC_NULL;
    OSRTDListNode    *p_node        = PNULL;
    nr_rrc_BandCombinationInfo        *p_elem    = PNULL;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctx);

    asn1Init_nr_rrc_ConfigRestrictInfoSCG(p_asn1_list);

    if(p_rrm_config_restrict_info_scg->bitmask &
            NR_CG_CONFIG_RESTRICT_INFO_SCG_SERV_CELL_INDEX_PRESENT)
    {
        p_asn1_list->m.servCellIndexRangeSCGPresent = 1;

        p_asn1_list->servCellIndexRangeSCG.lowBound = 
            p_rrm_config_restrict_info_scg->scg_serv_cell_index_range.
            low_bound_cell_index;

        p_asn1_list->servCellIndexRangeSCG.upBound= 
            p_rrm_config_restrict_info_scg->scg_serv_cell_index_range.
            up_bound_cell_index;
    }
    if ( NR_CG_CONFIG_RESTRICT_INFO_ALLOWED_BC_LIST_PRESENT &
            p_rrm_config_restrict_info_scg->bitmask )
    {
        asn1Init_nr_rrc_BandCombinationInfoList(&p_asn1_list->allowedBC_ListMRDC);

        p_asn1_list->m.allowedBC_ListMRDCPresent = RRC_ONE;

        band_comb_count = p_rrm_config_restrict_info_scg->
            allowed_bc_list_mrdc.band_comb_count;

        for ( band_count = RRC_NULL; band_count < band_comb_count;
                band_count++ )
        {
            rtxDListAllocNodeAndData(p_asn1_ctx,
                    nr_rrc_BandCombinationInfo,
                    &p_node,
                    &p_elem);

            if ( PNULL == p_node )
            {
                RRC_UECC_TRACE(p_uecc_e_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, 
                        RRC_FATAL,"Can't allocate p_node for "
                        "nr_rrc_BandCombinationInfo");

                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return RRC_FAILURE;
            }
            asn1Init_nr_rrc_BandCombinationInfo(p_elem);    
            p_elem->bandCombinationIndex = 
                p_rrm_config_restrict_info_scg->allowed_bc_list_mrdc.
                band_comb_index[band_count];
            p_elem->allowedFeatureSetsList.n = 1;
            p_elem->allowedFeatureSetsList.elem[0] = 1;

            /* append node to the list */
            rtxDListAppendNode(
                    &(p_asn1_list->allowedBC_ListMRDC), 
                    p_node);
        }
    }

    if ( NR_CG_CONFIG_RESTRICT_INFO_POWER_COORD_FR1_PRESENT &
        p_rrm_config_restrict_info_scg->bitmask )
    {
        RRC_UECC_TRACE(p_uecc_e_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name, 
                RRC_DETAILED,"Power coordination FR1 is present in "
                "config restrict info scg");

        p_asn1_list->m.powerCoordination_FR1Present = RRC_ONE;

        if ( POWER_COORDINATION_FR1_P_MAX_NR_FR1_PRESENT &
                p_rrm_config_restrict_info_scg->power_coordination_fr1.bitmask )
        {
            RRC_UECC_TRACE(p_uecc_e_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, 
                    RRC_DETAILED,"Pmax NR FR1 is present in "
                    "Power coordination FR1 ");
            p_asn1_list->powerCoordination_FR1.m.p_maxNR_FR1Present = RRC_ONE;

            p_asn1_list->powerCoordination_FR1.p_maxNR_FR1 =
                p_rrm_config_restrict_info_scg->power_coordination_fr1.p_max_nr_fr1;
        }
        if ( POWER_COORDINATION_FR1_P_MAX_EUTRA_PRESENT &
                p_rrm_config_restrict_info_scg->power_coordination_fr1.bitmask )
        {
            RRC_UECC_TRACE(p_uecc_e_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, 
                    RRC_DETAILED,"Pmax eutra is present in "
                    "Power coordination FR1 ");
            p_asn1_list->powerCoordination_FR1.m.p_maxEUTRAPresent = RRC_ONE;

            p_asn1_list->powerCoordination_FR1.p_maxEUTRA =
                p_rrm_config_restrict_info_scg->power_coordination_fr1.p_max_eutra;
        }
        if ( POWER_COORDINATION_FR1_P_MAX_UE_FR1_PRESENT &
                p_rrm_config_restrict_info_scg->power_coordination_fr1.bitmask )
        {
            RRC_UECC_TRACE(p_uecc_e_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, 
                    RRC_DETAILED,"Pmax ue FR1 is present in "
                    "Power coordination FR1 ");
        }
    }

    if ( NR_CG_CONFIG_RESTRICT_INFO_MAX_MEAS_FREQ_SCG_NR_PRESENT &
        p_rrm_config_restrict_info_scg->bitmask )
    {
        RRC_UECC_TRACE(p_uecc_e_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name, 
                RRC_DETAILED,"Max meas freq SCG NR is present in "
                "config restrict info scg");

        p_asn1_list->m.maxMeasFreqsSCG_NRPresent = RRC_ONE;

        p_asn1_list->maxMeasFreqsSCG_NR =
            p_rrm_config_restrict_info_scg->max_meas_freq_scg_nr;
    }

    if ( NR_CG_CONFIG_RESTRICT_INFO_MAX_MEAS_ID_SCG_NR_PRESENT &
        p_rrm_config_restrict_info_scg->bitmask )
    {
        RRC_UECC_TRACE(p_uecc_e_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name, 
                RRC_DETAILED,"Max meas identities SCG NR is present in "
                "config restrict info scg");

        p_asn1_list->m.maxMeasIdentitiesSCG_NRPresent = RRC_ONE;

        p_asn1_list->maxMeasIdentitiesSCG_NR =
            p_rrm_config_restrict_info_scg->max_meas_id_scg_nr;
    }

    return RRC_SUCCESS;
}


/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_mn_candidate_cell_info_list 
 *
 *   DESCRIPTION:
 *       This function builds candidateCellInfoListMN 
 *
 *   RETURNS: RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
static rrc_return_et uecc_x2ap_build_mn_candidate_cell_info_list(
        uecc_ue_context_t               *p_uecc_ue_context,
        nr_rrc_MeasResultList2NR        *p_asn1_list,
        rrc_rrm_meas_result_list2_nr_t  *p_rrm_mn_candid_cell_info_list,
        OSCTXT                          *p_asn1_ctx
        )
{
    nr_rrc_MeasResult2NR        *p_asn1_elem    = PNULL;
    OSRTDListNode               *p_node1        = PNULL;
    meas_result_list_nr_t         *p_rrm_nr_meas_result_list = PNULL;
    nr_rrc_MeasResultNR         *p_asn2_elem    = PNULL;
    OSRTDListNode               *p_node2        = PNULL;
    OSRTDListNode                 *p_node3        = PNULL;
    OSRTDListNode                 *p_node4        = PNULL;
    OSRTDListNode                 *p_node5        = PNULL;
    OSRTDListNode                 *p_node6        = PNULL;
    nr_rrc_ResultsPerCSI_RS_Index *p_asn3_elem    = PNULL;
    nr_rrc_ResultsPerSSB_Index    *p_asn4_elem    = PNULL;
    nr_rrc_ResultsPerSSB_Index    *p_asn5_elem    = PNULL;
    nr_rrc_ResultsPerCSI_RS_Index *p_asn6_elem    = PNULL;
    U8                            index           = RRC_NULL;
    U8                            ssb_index       = RRC_NULL;
    rrc_rrm_meas_cell_results_t   *p_rrm_meas_cell_results = PNULL;
    rrc_rrm_meas_rs_idx_results_t *p_meas_rs_idx_results = PNULL;
    rrc_rrm_meas_cell_results_t   *p_meas_cell_results = PNULL;
    rrc_rrm_meas_rs_idx_results_t *p_meas_rs_idx_results_2 = PNULL;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctx);

    U8 meas_result_count    = 0;
    U8 ms_neigh_cell_count  = 0;
    U8 counter              = 0;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    rtxDListInit(p_asn1_list);

    if (p_rrm_mn_candid_cell_info_list->count > MAX_FREQ)
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "candidate_cell_info_list_mn : invalid count=%i",
                p_rrm_mn_candid_cell_info_list->count);

        return RRC_FAILURE;
    }

    meas_result_count = p_rrm_mn_candid_cell_info_list->count;

    for(counter = 0; counter < meas_result_count; counter++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, nr_rrc_MeasResult2NR,
                &p_node1, &p_asn1_elem);

        if (PNULL == p_node1)
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "mn_candididate_cell_info : ASN malloc failed");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            return RRC_FAILURE;
        }

        asn1Init_nr_rrc_MeasResult2NR(p_asn1_elem);

        x2ap_srvd_nr_cell_info_t *p_served_nr_cell_info;
        p_served_nr_cell_info = (x2ap_srvd_nr_cell_info_t *)p_uecc_ue_context->
            p_gb_context->x2ap_ctx.x2ap_link_info[p_uecc_ue_context->x2ap_context.peer_enodeb_id].
                p_nr_srvd_cell_info;

        if ( X2AP_NR_MODE_FDD_PRESENT &
            p_served_nr_cell_info->choice_nr_mode.bitmask )
        {
            p_asn1_elem->m.refFreqCSI_RSPresent = RRC_ONE;
            p_asn1_elem->refFreqCSI_RS =
                p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                    nr_ul_freq_info.nr_earfcn;
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                    "[x2ap_SgNBAdditionRequest]"
                    "refFreqCSI_RS present");
        }
        else
        {
            p_asn1_elem->m.refFreqCSI_RSPresent = RRC_NULL;
        }

        /* update ssbFrequency */
        if ( RRC_RRM_MEAS_RESULT2_NR_SSB_FREQ_PRESENT &
                p_rrm_mn_candid_cell_info_list->rrc_rrm_meas_result2_nr[counter].bitmask )
        {
            p_asn1_elem->m.ssbFrequencyPresent = RRC_ONE;

            p_asn1_elem->ssbFrequency =
                p_rrm_mn_candid_cell_info_list->
                rrc_rrm_meas_result2_nr[counter].ssb_frequency;

            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                    "[x2ap_SgNBAdditionRequest]"
                    "ssbFrequency present");
        }
        else
        {
            p_asn1_elem->m.ssbFrequencyPresent = RRC_NULL;
        }

        /* update measResultServingCell */
        if ( RRC_RRM_MEAS_RESULT2_NR_MEAS_RES_SERVING_PRESENT &
                p_rrm_mn_candid_cell_info_list->rrc_rrm_meas_result2_nr[counter].bitmask )
        {
            p_asn1_elem->m.measResultServingCellPresent = RRC_ONE;

            /* update nr_rrc_PhysCellId */
            if ( RRC_RRM_MEAS_RESULT_NR_PHY_CELL_ID_PRESENT &
                    p_rrm_mn_candid_cell_info_list->rrc_rrm_meas_result2_nr[counter].
                    meas_result_serving_cell.bitmask )
            {
                p_asn1_elem->measResultServingCell.m.physCellIdPresent = RRC_ONE;

                p_asn1_elem->measResultServingCell.physCellId =
                    p_rrm_mn_candid_cell_info_list->rrc_rrm_meas_result2_nr[counter].
                    meas_result_serving_cell.phy_cell_id;

                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                        "[x2ap_SgNBAdditionRequest]"
                        "nr_rrc_PhysCellId present");
            }
            else
            {
                p_asn1_elem->measResultServingCell.m.physCellIdPresent = RRC_NULL;
            }

            /* update nr_rrc_MeasResultNR_measResult */
            /* update cellResults */
            /* update resultsSSB_Cell */
            p_asn1_elem->measResultServingCell.measResult.cellResults.m.
                resultsSSB_CellPresent = RRC_NULL;

            p_rrm_meas_cell_results = &(p_rrm_mn_candid_cell_info_list->rrc_rrm_meas_result2_nr[counter].
                meas_result_serving_cell.rrc_rrm_meas_result.meas_cell_results);

            if ( RRC_RRM_MEAS_CELL_RESULT_SSB_RESULT_PRESENT &
                    p_rrm_meas_cell_results->bitmask )
            {
                p_asn1_elem->measResultServingCell.measResult.cellResults.m.
                    resultsSSB_CellPresent = RRC_ONE;

                p_asn1_elem->measResultServingCell.measResult.cellResults.
                    resultsSSB_Cell.m.rsrpPresent = RRC_NULL;
                p_asn1_elem->measResultServingCell.measResult.cellResults.
                    resultsSSB_Cell.m.rsrqPresent = RRC_NULL;
                p_asn1_elem->measResultServingCell.measResult.cellResults.
                    resultsSSB_Cell.m.sinrPresent = RRC_NULL;

                /* update rsrp */
                if ( RRC_RRM_MEAS_QUANTITY_RES_RSRP_PRESENT &
                        p_rrm_meas_cell_results->result_ssb_cell.bitmask )
                {
                    p_asn1_elem->measResultServingCell.measResult.cellResults.
                        resultsSSB_Cell.m.rsrpPresent = RRC_ONE;

                    p_asn1_elem->measResultServingCell.measResult.cellResults.
                        resultsSSB_Cell.rsrp =
                        p_rrm_meas_cell_results->result_ssb_cell.rsrp;
                }

                /* update rsrq */
                if ( RRC_RRM_MEAS_QUANTITY_RES_RSRQ_PRESENT &
                        p_rrm_meas_cell_results->result_ssb_cell.bitmask )
                {
                    p_asn1_elem->measResultServingCell.measResult.cellResults.
                        resultsSSB_Cell.m.rsrqPresent = RRC_ONE;

                    p_asn1_elem->measResultServingCell.measResult.cellResults.
                        resultsSSB_Cell.rsrq =
                        p_rrm_meas_cell_results->result_ssb_cell.rsrq;
                }

                /* update sinr */
                if ( RRC_RRM_MEAS_QUANTITY_RES_SINR_PRESENT &
                        p_rrm_meas_cell_results->result_ssb_cell.bitmask )
                {
                    p_asn1_elem->measResultServingCell.measResult.cellResults.
                        resultsSSB_Cell.m.sinrPresent = RRC_ONE;

                    p_asn1_elem->measResultServingCell.measResult.cellResults.
                        resultsSSB_Cell.sinr =
                        p_rrm_meas_cell_results->result_ssb_cell.sinr;
                }
            }

            /* update resultsCSI_RS_Cell */
            p_asn1_elem->measResultServingCell.measResult.cellResults.m.
                resultsCSI_RS_CellPresent = RRC_NULL;

            if ( RRC_RRM_MEAS_CELL_RESULT_CSI_RS_RESULT_PRESENT &
                    p_rrm_meas_cell_results->bitmask )
            {
                p_asn1_elem->measResultServingCell.measResult.cellResults.m.
                    resultsCSI_RS_CellPresent = RRC_ONE;

                p_asn1_elem->measResultServingCell.measResult.cellResults.
                    resultsCSI_RS_Cell.m.rsrpPresent = RRC_NULL;
                p_asn1_elem->measResultServingCell.measResult.cellResults.
                    resultsCSI_RS_Cell.m.rsrqPresent = RRC_NULL;
                p_asn1_elem->measResultServingCell.measResult.cellResults.
                    resultsCSI_RS_Cell.m.sinrPresent = RRC_NULL;

                /* update rsrp */
                if ( RRC_RRM_MEAS_QUANTITY_RES_RSRP_PRESENT &
                        p_rrm_meas_cell_results->result_csi_rs_cell.bitmask )
                {
                    p_asn1_elem->measResultServingCell.measResult.cellResults.
                        resultsCSI_RS_Cell.m.rsrpPresent = RRC_ONE;

                    p_asn1_elem->measResultServingCell.measResult.cellResults.
                        resultsCSI_RS_Cell.rsrp =
                        p_rrm_meas_cell_results->result_csi_rs_cell.rsrp;
                }

                /* update rsrq */
                if ( RRC_RRM_MEAS_QUANTITY_RES_RSRQ_PRESENT &
                        p_rrm_meas_cell_results->result_csi_rs_cell.bitmask )
                {
                    p_asn1_elem->measResultServingCell.measResult.cellResults.
                        resultsCSI_RS_Cell.m.rsrqPresent = RRC_ONE;

                    p_asn1_elem->measResultServingCell.measResult.cellResults.
                        resultsCSI_RS_Cell.rsrq =
                        p_rrm_meas_cell_results->result_csi_rs_cell.rsrq;
                }

                /* update sinr */
                if ( RRC_RRM_MEAS_QUANTITY_RES_SINR_PRESENT &
                        p_rrm_meas_cell_results->result_csi_rs_cell.bitmask )
                {
                    p_asn1_elem->measResultServingCell.measResult.cellResults.
                        resultsCSI_RS_Cell.m.sinrPresent = RRC_ONE;

                    p_asn1_elem->measResultServingCell.measResult.cellResults.
                        resultsCSI_RS_Cell.sinr =
                        p_rrm_meas_cell_results->result_csi_rs_cell.sinr;
                }
            }

            /* update rsIndexResults */
            if ( RRC_RRM_MEAS_RESULT_RS_IDX_RESULT_PRESENT &
                    p_rrm_mn_candid_cell_info_list->rrc_rrm_meas_result2_nr[counter].
                    meas_result_serving_cell.rrc_rrm_meas_result.bitmask )
            {
                p_asn1_elem->measResultServingCell.measResult.m.rsIndexResultsPresent =
                    RRC_ONE;

                p_asn1_elem->measResultServingCell.measResult.rsIndexResults.m.
                    resultsSSB_IndexesPresent = RRC_NULL;
                p_asn1_elem->measResultServingCell.measResult.rsIndexResults.m.
                    resultsCSI_RS_IndexesPresent = RRC_NULL;

                p_meas_rs_idx_results = &(p_rrm_mn_candid_cell_info_list->
                    rrc_rrm_meas_result2_nr[counter].
                    meas_result_serving_cell.rrc_rrm_meas_result.meas_rs_idx_results);

                /* update resultsSSB_Indexes */
                if ( RRC_RRM_MEAS_RESULT_RS_PER_SSB_PRESENT &
                        p_meas_rs_idx_results->bitmask )
                {
                    p_asn1_elem->measResultServingCell.measResult.rsIndexResults.m.
                        resultsSSB_IndexesPresent = RRC_ONE;

                    for ( ssb_index = RRC_NULL; ssb_index <
                            p_meas_rs_idx_results->results_per_ssb_idx_list.count;
                            ssb_index++ )
                    {
                        rtxDListAllocNodeAndData(p_asn1_ctx, nr_rrc_ResultsPerSSB_Index,
                                &p_node4, &p_asn4_elem);

                        if ( PNULL == p_node4 )
                        {
                            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                                    "nr_rrc_ResultsPerSSB_Index : ASN malloc failed");
                            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                            return RRC_FAILURE;
                        }

                        /* update ssb_Index */
                        p_asn4_elem->ssb_Index = p_meas_rs_idx_results->
                            results_per_ssb_idx_list.rrc_rrm_results_per_ssb_idx[ssb_index].ssb_index;

                        p_asn4_elem->m.ssb_ResultsPresent = RRC_NULL;

                        /* update ssb_result */
                        if ( RRC_RRM_RESULT_PER_SSB_SSB_RESULTS_PRESENT &
                                p_meas_rs_idx_results->results_per_ssb_idx_list.
                                    rrc_rrm_results_per_ssb_idx[ssb_index].bitmask )
                        {
                            p_asn4_elem->m.ssb_ResultsPresent = RRC_ONE;

                            p_asn4_elem->ssb_Results.m.rsrpPresent = RRC_NULL;
                            p_asn4_elem->ssb_Results.m.rsrpPresent = RRC_NULL;
                            p_asn4_elem->ssb_Results.m.rsrpPresent = RRC_NULL;

                            /* update rsrp */
                            if ( RRC_RRM_MEAS_QUANTITY_RES_RSRP_PRESENT &
                                    p_meas_rs_idx_results->results_per_ssb_idx_list.
                                    rrc_rrm_results_per_ssb_idx[ssb_index].
                                    ssb_results.bitmask )
                            {
                                p_asn4_elem->ssb_Results.m.rsrpPresent = RRC_ONE;

                                p_asn4_elem->ssb_Results.rsrp =
                                    p_meas_rs_idx_results->results_per_ssb_idx_list.
                                    rrc_rrm_results_per_ssb_idx[ssb_index].
                                    ssb_results.rsrp;
                            }

                            /* update rsrq */
                            if ( RRC_RRM_MEAS_QUANTITY_RES_RSRQ_PRESENT &
                                    p_meas_rs_idx_results->results_per_ssb_idx_list.
                                    rrc_rrm_results_per_ssb_idx[ssb_index].
                                    ssb_results.bitmask )
                            {
                                p_asn4_elem->ssb_Results.m.rsrqPresent = RRC_ONE;

                                p_asn4_elem->ssb_Results.rsrq =
                                    p_meas_rs_idx_results->results_per_ssb_idx_list.
                                    rrc_rrm_results_per_ssb_idx[ssb_index].
                                    ssb_results.rsrq;
                            }

                            /* update sinr */
                            if ( RRC_RRM_MEAS_QUANTITY_RES_SINR_PRESENT &
                                    p_meas_rs_idx_results->results_per_ssb_idx_list.
                                    rrc_rrm_results_per_ssb_idx[ssb_index].
                                    ssb_results.bitmask )
                            {
                                p_asn4_elem->ssb_Results.m.sinrPresent = RRC_ONE;

                                p_asn4_elem->ssb_Results.sinr =
                                    p_meas_rs_idx_results->results_per_ssb_idx_list.
                                    rrc_rrm_results_per_ssb_idx[ssb_index].
                                    ssb_results.sinr;
                            }
                        }

                        /* append node to the list */
                        rtxDListAppendNode(
                                &(p_asn1_elem->measResultServingCell.measResult.rsIndexResults.
                                    resultsSSB_Indexes), 
                                p_node4);
                    }
                }

                /* update resultsCSI_RS_Indexes */
                if ( RRC_RRM_MEAS_RESULT_RS_PER_CSI_RS_PRESENT &
                        p_meas_rs_idx_results->bitmask )
                {
                    p_asn1_elem->measResultServingCell.measResult.rsIndexResults.m.
                        resultsCSI_RS_IndexesPresent = RRC_ONE;

                    for ( index = RRC_NULL; index <
                            p_meas_rs_idx_results->results_per_csi_rs_idx_list.count;
                            index++ )
                    {
                        rtxDListAllocNodeAndData(p_asn1_ctx, nr_rrc_ResultsPerCSI_RS_Index,
                                &p_node3, &p_asn3_elem);

                        if ( PNULL == p_node3 )
                        {
                            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                                    "nr_rrc_ResultsPerCSI_RS_Index : ASN malloc failed");
                            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                            return RRC_FAILURE;
                        }

                        /* update csi_RS_Index */
                        p_asn3_elem->csi_RS_Index =
                            p_meas_rs_idx_results->results_per_csi_rs_idx_list.
                            rrc_rrm_results_per_csi_rs_idx[index].
                            csi_rs_index;

                        p_asn3_elem->m.csi_RS_ResultsPresent = RRC_NULL;

                        /* update csi_RS_Results */
                        if ( RRC_RRM_RESULT_PER_CSI_RS_CSI_RS_RESULTS_PRESENT &
                                p_meas_rs_idx_results->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].bitmask )
                        {
                            p_asn3_elem->m.csi_RS_ResultsPresent = RRC_ONE;

                            p_asn3_elem->csi_RS_Results.m.rsrpPresent = RRC_NULL;
                            p_asn3_elem->csi_RS_Results.m.rsrpPresent = RRC_NULL;
                            p_asn3_elem->csi_RS_Results.m.rsrpPresent = RRC_NULL;

                            /* update rsrp */
                            if ( RRC_RRM_MEAS_QUANTITY_RES_RSRP_PRESENT &
                                    p_meas_rs_idx_results->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].
                                    csi_rs_results.bitmask )
                            {
                                p_asn3_elem->csi_RS_Results.m.rsrpPresent = RRC_ONE;

                                p_asn3_elem->csi_RS_Results.rsrp =
                                    p_meas_rs_idx_results->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].
                                    csi_rs_results.rsrp;
                            }

                            /* update rsrq */
                            if ( RRC_RRM_MEAS_QUANTITY_RES_RSRQ_PRESENT &
                                    p_meas_rs_idx_results->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].
                                    csi_rs_results.bitmask )
                            {
                                p_asn3_elem->csi_RS_Results.m.rsrqPresent = RRC_ONE;

                                p_asn3_elem->csi_RS_Results.rsrq =
                                    p_meas_rs_idx_results->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].
                                    csi_rs_results.rsrq;
                            }

                            /* update sinr */
                            if ( RRC_RRM_MEAS_QUANTITY_RES_SINR_PRESENT &
                                    p_meas_rs_idx_results->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].
                                    csi_rs_results.bitmask )
                            {
                                p_asn3_elem->csi_RS_Results.m.sinrPresent = RRC_ONE;

                                p_asn3_elem->csi_RS_Results.sinr =
                                    p_meas_rs_idx_results->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].
                                    csi_rs_results.sinr;
                            }
                        }

                        /* append node to the list */
                        rtxDListAppendNode(
                                &(p_asn1_elem->measResultServingCell.measResult.rsIndexResults.
                                    resultsCSI_RS_Indexes), 
                                p_node3);
                    }
                }
            }
            else
            {
                p_asn1_elem->measResultServingCell.measResult.
                    m.rsIndexResultsPresent = RRC_NULL;
            }
        }
        else
        {
            p_asn1_elem->m.measResultServingCellPresent = RRC_NULL;
        }

        /*update measResultNeighCellListNR*/
        if ( RRC_RRM_MEAS_RESULT2_NR_MEAS_RES_NEIGH_PRESENT &
                p_rrm_mn_candid_cell_info_list->rrc_rrm_meas_result2_nr[counter].bitmask )
        {
            p_asn1_elem->m.measResultNeighCellListNRPresent = 1;
            p_rrm_nr_meas_result_list = 
                &p_rrm_mn_candid_cell_info_list->rrc_rrm_meas_result2_nr[counter].
                meas_result_neigh_cell_list_nr;

            for(ms_neigh_cell_count = 0; 
                    ms_neigh_cell_count < p_rrm_nr_meas_result_list->count;
                    ms_neigh_cell_count++)
            {
                rtxDListAllocNodeAndData(p_asn1_ctx, nr_rrc_MeasResultNR,
                        &p_node2, &p_asn2_elem);

                if (PNULL == p_node2)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                            "nr_rrc_MeasResultNR : ASN malloc failed");
                    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }

                if ( p_rrm_nr_meas_result_list->meas_result_nr[ms_neigh_cell_count].bitmask &
                        RRC_RRM_MEAS_RESULT_NR_PHY_CELL_ID_PRESENT )
                {
                    p_asn2_elem->m.physCellIdPresent = 1;

                    p_asn2_elem->physCellId = 
                        p_rrm_nr_meas_result_list->meas_result_nr[ms_neigh_cell_count].phy_cell_id;
                }

                /*update nr_rrc_MeasResultNR_measResult*/
                p_asn2_elem->measResult.cellResults.m.resultsSSB_CellPresent = 0;
                p_asn2_elem->measResult.cellResults.m.resultsCSI_RS_CellPresent = 0;
                p_asn2_elem->measResult.m.rsIndexResultsPresent = 0;

                p_meas_cell_results = &(p_rrm_nr_meas_result_list->
                    meas_result_nr[ms_neigh_cell_count].
                    rrc_rrm_meas_result.meas_cell_results);

                /*update nr_rrc_MeasResultNR_measResult_cellResults*/
                if ( RRC_RRM_MEAS_CELL_RESULT_SSB_RESULT_PRESENT &
                        p_meas_cell_results->bitmask )
                {
                    p_asn2_elem->measResult.cellResults.m.
                        resultsSSB_CellPresent = RRC_ONE;

                    /*update resultsSSB_Cell*/
                    if ( RRC_RRM_MEAS_CELL_RESULT_SSB_RESULT_PRESENT &
                            p_meas_cell_results->bitmask )
                    {
                        /*update nr_rrc_RSRP_Range*/
                        if ( RRC_RRM_MEAS_QUANTITY_RES_RSRP_PRESENT &
                                p_meas_cell_results->result_ssb_cell.bitmask )
                        {
                            p_asn2_elem->measResult.cellResults.resultsSSB_Cell.m.rsrpPresent =
                                RRC_ONE;

                            p_asn2_elem->measResult.cellResults.resultsSSB_Cell.rsrp =
                                p_meas_cell_results->result_ssb_cell.rsrp;
                        }

                        /*update nr_rrc_RSRQ_Range*/
                        if ( RRC_RRM_MEAS_QUANTITY_RES_RSRQ_PRESENT &
                                p_meas_cell_results->result_ssb_cell.bitmask )
                        {
                            p_asn2_elem->measResult.cellResults.resultsSSB_Cell.m.rsrqPresent =
                                RRC_ONE;

                            p_asn2_elem->measResult.cellResults.resultsSSB_Cell.rsrq =
                                p_meas_cell_results->result_ssb_cell.rsrq;
                        }

                        /*update nr_rrc_SINR_Range*/
                        if ( RRC_RRM_MEAS_QUANTITY_RES_SINR_PRESENT &
                                p_meas_cell_results->result_ssb_cell.bitmask )
                        {
                            p_asn2_elem->measResult.cellResults.resultsSSB_Cell.m.sinrPresent =
                                RRC_ONE;

                            p_asn2_elem->measResult.cellResults.resultsSSB_Cell.sinr =
                                p_meas_cell_results->result_ssb_cell.sinr;
                        }
                    }

                    /*update resultsCSI_RS_Cell*/
                    if ( RRC_RRM_MEAS_CELL_RESULT_CSI_RS_RESULT_PRESENT &
                            p_meas_cell_results->bitmask )
                    {
                        p_asn2_elem->measResult.cellResults.m.resultsSSB_CellPresent = RRC_ONE;

                        /*update nr_rrc_RSRP_Range*/
                        if ( RRC_RRM_MEAS_QUANTITY_RES_RSRP_PRESENT &
                                p_meas_cell_results->result_csi_rs_cell.bitmask )
                        {
                            p_asn2_elem->measResult.cellResults.resultsCSI_RS_Cell.m.rsrpPresent =
                                RRC_ONE;

                            p_asn2_elem->measResult.cellResults.resultsCSI_RS_Cell.rsrp =
                                p_meas_cell_results->result_csi_rs_cell.rsrp;
                        }

                        /*update nr_rrc_RSRQ_Range*/
                        if ( RRC_RRM_MEAS_QUANTITY_RES_RSRQ_PRESENT &
                                p_meas_cell_results->result_csi_rs_cell.bitmask )
                        {
                            p_asn2_elem->measResult.cellResults.resultsCSI_RS_Cell.m.rsrqPresent =
                                RRC_ONE;

                            p_asn2_elem->measResult.cellResults.resultsCSI_RS_Cell.rsrq =
                                p_meas_cell_results->result_csi_rs_cell.rsrq;
                        }

                        /*update nr_rrc_SINR_Range*/
                        if ( RRC_RRM_MEAS_QUANTITY_RES_SINR_PRESENT &
                                p_meas_cell_results->result_csi_rs_cell.bitmask )
                        {
                            p_asn2_elem->measResult.cellResults.resultsCSI_RS_Cell.m.sinrPresent =
                                RRC_ONE;

                            p_asn2_elem->measResult.cellResults.resultsCSI_RS_Cell.sinr =
                                p_meas_cell_results->result_csi_rs_cell.sinr;
                        }
                    }
                }

                /*update nr_rrc_MeasResultNR_measResult_rsIndexResults*/
                if ( RRC_RRM_MEAS_RESULT_RS_IDX_RESULT_PRESENT &
                        p_rrm_nr_meas_result_list->meas_result_nr[ms_neigh_cell_count].
                        rrc_rrm_meas_result.bitmask )
                {
                    p_asn2_elem->measResult.m.rsIndexResultsPresent = RRC_ONE;

                    p_meas_rs_idx_results_2 = &(p_rrm_nr_meas_result_list->
                        meas_result_nr[ms_neigh_cell_count].
                        rrc_rrm_meas_result.meas_rs_idx_results);

                    /*update nr_rrc_ResultsPerSSB_IndexList*/
                    if ( RRC_RRM_MEAS_RESULT_RS_PER_SSB_PRESENT &
                            p_meas_rs_idx_results_2->bitmask )
                    {
                        p_asn2_elem->measResult.rsIndexResults.m.resultsSSB_IndexesPresent =
                            RRC_ONE;

                        for ( ssb_index = RRC_NULL; ssb_index <
                                p_meas_rs_idx_results_2->results_per_ssb_idx_list.count;
                                ssb_index++ )
                        {
                            rtxDListAllocNodeAndData(p_asn1_ctx, nr_rrc_ResultsPerSSB_Index,
                                    &p_node5, &p_asn5_elem);

                            if ( PNULL == p_node5 )
                            {
                                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                                        p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                                        "nr_rrc_ResultsPerSSB_Index : ASN malloc failed");
                                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                                return RRC_FAILURE;
                            }

                            /* update ssb_Index */
                            p_asn5_elem->ssb_Index =
                                p_meas_rs_idx_results_2->results_per_ssb_idx_list.
                                rrc_rrm_results_per_ssb_idx[ssb_index].ssb_index;

                            p_asn5_elem->m.ssb_ResultsPresent = RRC_NULL;

                            /* update ssb_result */
                            if ( RRC_RRM_RESULT_PER_SSB_SSB_RESULTS_PRESENT &
                                    p_meas_rs_idx_results_2->results_per_ssb_idx_list.
                                        rrc_rrm_results_per_ssb_idx[ssb_index].bitmask )
                            {
                                p_asn5_elem->m.ssb_ResultsPresent = RRC_ONE;

                                p_asn5_elem->ssb_Results.m.rsrpPresent = RRC_NULL;
                                p_asn5_elem->ssb_Results.m.rsrpPresent = RRC_NULL;
                                p_asn5_elem->ssb_Results.m.rsrpPresent = RRC_NULL;

                                /* update rsrp */
                                if ( RRC_RRM_MEAS_QUANTITY_RES_RSRP_PRESENT &
                                        p_meas_rs_idx_results_2->results_per_ssb_idx_list.
                                        rrc_rrm_results_per_ssb_idx[ssb_index].
                                        ssb_results.bitmask )
                                {
                                    p_asn5_elem->ssb_Results.m.rsrpPresent = RRC_ONE;

                                    p_asn5_elem->ssb_Results.rsrp =
                                        p_meas_rs_idx_results_2->results_per_ssb_idx_list.
                                        rrc_rrm_results_per_ssb_idx[ssb_index].
                                        ssb_results.rsrp;
                                }

                                /* update rsrq */
                                if ( RRC_RRM_MEAS_QUANTITY_RES_RSRQ_PRESENT &
                                        p_meas_rs_idx_results_2->results_per_ssb_idx_list.
                                        rrc_rrm_results_per_ssb_idx[ssb_index].
                                        ssb_results.bitmask )
                                {
                                    p_asn5_elem->ssb_Results.m.rsrqPresent = RRC_ONE;

                                    p_asn5_elem->ssb_Results.rsrq =
                                        p_meas_rs_idx_results_2->results_per_ssb_idx_list.
                                        rrc_rrm_results_per_ssb_idx[ssb_index].
                                        ssb_results.rsrq;
                                }

                                /* update sinr */
                                if ( RRC_RRM_MEAS_QUANTITY_RES_SINR_PRESENT &
                                        p_meas_rs_idx_results_2->results_per_ssb_idx_list.
                                        rrc_rrm_results_per_ssb_idx[ssb_index].
                                        ssb_results.bitmask )
                                {
                                    p_asn5_elem->ssb_Results.m.sinrPresent = RRC_ONE;

                                    p_asn5_elem->ssb_Results.sinr =
                                        p_meas_rs_idx_results_2->results_per_ssb_idx_list.
                                        rrc_rrm_results_per_ssb_idx[ssb_index].
                                        ssb_results.sinr;
                                }
                            }

                            /* append node to the list */
                            rtxDListAppendNode(
                                    &(p_asn2_elem->measResult.rsIndexResults.
                                        resultsSSB_Indexes), 
                                    p_node5);
                        }
                    }

                /*update nr_rrc_ResultsPerCSI_RS_IndexList*/
                if ( RRC_RRM_MEAS_RESULT_RS_PER_CSI_RS_PRESENT &
                        p_rrm_nr_meas_result_list->meas_result_nr[ms_neigh_cell_count].
                        rrc_rrm_meas_result.bitmask )
                {
                    p_asn2_elem->measResult.rsIndexResults.m.resultsCSI_RS_IndexesPresent =
                        RRC_ONE;

                    for ( index = RRC_NULL; index <
                            p_meas_rs_idx_results_2->results_per_csi_rs_idx_list.count;
                            index++ )
                    {
                        rtxDListAllocNodeAndData(p_asn1_ctx, nr_rrc_ResultsPerCSI_RS_Index,
                                &p_node6, &p_asn6_elem);

                        if ( PNULL == p_node6 )
                        {
                            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                                    "nr_rrc_ResultsPerCSI_RS_Index : ASN malloc failed");
                            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                            return RRC_FAILURE;
                        }

                        /* update csi_RS_Index */
                        p_asn6_elem->csi_RS_Index =
                            p_meas_rs_idx_results_2->results_per_csi_rs_idx_list.
                            rrc_rrm_results_per_csi_rs_idx[index].
                            csi_rs_index;

                        p_asn6_elem->m.csi_RS_ResultsPresent = RRC_NULL;

                        /* update csi_RS_Results */
                        if ( RRC_RRM_RESULT_PER_CSI_RS_CSI_RS_RESULTS_PRESENT &
                                p_meas_rs_idx_results_2->results_per_csi_rs_idx_list.
                                rrc_rrm_results_per_csi_rs_idx[index].bitmask )
                        {
                            p_asn6_elem->m.csi_RS_ResultsPresent = RRC_ONE;

                            p_asn6_elem->csi_RS_Results.m.rsrpPresent = RRC_NULL;
                            p_asn6_elem->csi_RS_Results.m.rsrpPresent = RRC_NULL;
                            p_asn6_elem->csi_RS_Results.m.rsrpPresent = RRC_NULL;

                            /* update rsrp */
                            if ( RRC_RRM_MEAS_QUANTITY_RES_RSRP_PRESENT &
                                    p_meas_rs_idx_results_2->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].
                                    csi_rs_results.bitmask )
                            {
                                p_asn6_elem->csi_RS_Results.m.rsrpPresent = RRC_ONE;

                                p_asn6_elem->csi_RS_Results.rsrp =
                                    p_meas_rs_idx_results_2->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].
                                    csi_rs_results.rsrp;
                            }

                            /* update rsrq */
                            if ( RRC_RRM_MEAS_QUANTITY_RES_RSRQ_PRESENT &
                                    p_meas_rs_idx_results_2->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].
                                    csi_rs_results.bitmask )
                            {
                                p_asn6_elem->csi_RS_Results.m.rsrqPresent = RRC_ONE;

                                p_asn6_elem->csi_RS_Results.rsrq =
                                    p_meas_rs_idx_results_2->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].
                                    csi_rs_results.rsrq;
                            }

                            /* update sinr */
                            if ( RRC_RRM_MEAS_QUANTITY_RES_SINR_PRESENT &
                                    p_meas_rs_idx_results_2->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].
                                    csi_rs_results.bitmask )
                            {
                                p_asn6_elem->csi_RS_Results.m.sinrPresent = RRC_ONE;

                                p_asn6_elem->csi_RS_Results.sinr =
                                    p_meas_rs_idx_results_2->results_per_csi_rs_idx_list.
                                    rrc_rrm_results_per_csi_rs_idx[index].
                                    csi_rs_results.sinr;
                            }
                        }

                        /* append node to the list */
                        rtxDListAppendNode(
                                &(p_asn2_elem->measResult.rsIndexResults.
                                    resultsCSI_RS_Indexes), 
                                p_node6);
                    }
                }
/* coverity_280043_fix_start */
                }
/* coverity_280043_fix_end */
                /* append node to the list */
                rtxDListAppendNode(
                        &(p_asn1_elem->measResultNeighCellListNR), 
                        p_node2);
            }
        }
        else
        {
            p_asn1_elem->m.measResultNeighCellListNRPresent = RRC_NULL;
        }

        /* append node to the list */
        rtxDListAppendNode(p_asn1_list, p_node1);
    }
    return RRC_SUCCESS;
}
/******************************************************************************
 *   FUNCTION NAME:  uecc_x2ap_fill_ue_capabilityinfo_from_ue_capability_rat_cont_list
 *
 *   DESCRIPTION:
 *       This function fills ue_CapabilityInfo in MeNBToSgNBContainer from
 *       ue_capabilityRATContainerList
 *
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_fill_ue_capabilityinfo_from_ue_capability_rat_cont_list(
        uecc_ue_context_t               *p_ue_context,
        OSDynOctStr                     *p_ue_CapabilityInfo,
        OSCTXT                          *pctxt)
{
    U8                              msg[X2AP_MAX_ASN1_BUF_LEN];
    rrc_return_t                    rrc_result =0;
    rrc_return_et                   result = RRC_SUCCESS;
    OSRTDListNode*                  p_node = PNULL;
    nr_rrc_UE_CapabilityRAT_ContainerList  
        nr_ue_CapabilityRAT_ContainerList;
    nr_rrc_UE_CapabilityRAT_Container
        *p_elem = PNULL;
    UE_Radio_Capability_RAT_Container_t 
        *p_ue_radio_capability = PNULL;
    nr_rrc_UE_CapabilityRAT_ContainerList  
        nr_ue_CapabilityRAT_ContainerList_1;
    OSCTXT                          asn1_ctx;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_CapabilityInfo);
    RRC_ASSERT(PNULL != pctxt);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    asn1Init_nr_rrc_UE_CapabilityRAT_ContainerList(&nr_ue_CapabilityRAT_ContainerList);

    memset_wrapper(msg, RRC_NULL, sizeof(X2AP_MAX_ASN1_BUF_LEN));

    for (p_ue_radio_capability = (UE_Radio_Capability_RAT_Container_t*) 
            ylFirst (&p_ue_context->ue_radio_capability);
            p_ue_radio_capability; 
            p_ue_radio_capability = (UE_Radio_Capability_RAT_Container_t*) 
            ylNext((YLNODE*)p_ue_radio_capability))
    {
        if ( (RRC_TRUE == p_ue_radio_capability->valid_rat_string) && 
                /*Check for NR / EUTRA_NR RAT Types*/
                ( (nr == p_ue_radio_capability->rat_type) || 
                  (eutra_nr == p_ue_radio_capability->rat_type) 
                ) 
           )
        {
            rtxDListAllocNodeAndData(pctxt,
                    nr_rrc_UE_CapabilityRAT_Container,
                    &p_node,
                    &p_elem);

            if (PNULL==p_node)
            {
                RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                        (p_ue_context->p_gb_context)->facility_name,
                        RRC_FATAL,"Can't allocate p_node");
                result = RRC_FAILURE;
                break;
            }

            asn1Init_nr_rrc_UE_CapabilityRAT_Container(p_elem);

            p_elem->rat_Type = 
                (nr == p_ue_radio_capability->rat_type) ? nr_rrc_nr_1 : nr_rrc_eutra_nr ; 

            p_elem->ue_CapabilityRAT_Container.numocts = 
                p_ue_radio_capability->numocts;

            p_elem->ue_CapabilityRAT_Container.data =
                (OSOCTET*)rtxMemAlloc(pctxt,
                        p_elem->ue_CapabilityRAT_Container.numocts);
            if (PNULL== p_elem->ue_CapabilityRAT_Container.data)
            {
                RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                        (p_ue_context->p_gb_context)->facility_name,
                        RRC_FATAL, "ASN malloc failed.");
                result = RRC_FAILURE;
                break;
            }
            l3_memcpy_wrapper((void*)(p_elem->ue_CapabilityRAT_Container.data),
                    /* SPR 13502 Fix Start */
                    (U8 *)(p_ue_radio_capability->p_rat_string),
                    /* SPR 13502 Fix Stop */
                    p_elem->ue_CapabilityRAT_Container.numocts);

            /* Decode and print UE-RAT-CAPABILTY */
            if ( nr == p_ue_radio_capability->rat_type  &&
                    RRC_SUCCESS == rrc_check_logger_type(RRC_ASN))
            {
                RRC_TRACE(RRC_DETAILED,"NR UE radio capab");
                nr_rrc_UE_NR_Capability cap_buff;

                pu_setBuffer (pctxt,
                        (OSOCTET *)p_elem->ue_CapabilityRAT_Container.data,
                        p_elem->ue_CapabilityRAT_Container.numocts,
                        FALSE);

                if (RT_OK != asn1PD_nr_rrc_UE_NR_Capability(
                            pctxt,
                            &cap_buff))
                {
                    rtxErrPrint(pctxt);
                    RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                            (p_ue_context->p_gb_context)->facility_name,
                            RRC_FATAL,
                            "ASN1 decoding failed for nr_rrc_UE_NR_Capability");
                }
                else
                {
                    S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);

                    if(pBuff != NULL)
                    {
                        memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);

                        /* Converted to char* as required by ASN function */
                        rrc_result = asn1PrtToStr_nr_rrc_UE_NR_Capability(
                                "NR UE CapabilityRAT Container --> nr_rrc_UE_NR_Capability",
                                &cap_buff,
                                (char*)pBuff,
                                MAX_ASN_PRINT_BUFF_SIZE);

                        if(rrc_result == 0)
                        {
                            RRC_TRACE_ASN((S32)RRC_ASN, "%s\n", pBuff);
                        }
                        else
                        {
                            RRC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be increased");
                        }
                        rrc_mem_free(pBuff);
                    }
                }
            }
            if ( eutra_nr == p_ue_radio_capability->rat_type &&
                    RRC_SUCCESS == rrc_check_logger_type(RRC_ASN))
            {
                RRC_TRACE(RRC_DETAILED,"mrdc UE radio capab");
                nr_rrc_UE_MRDC_Capability cap_buff;

                pu_setBuffer (pctxt,
                        (OSOCTET *)p_elem->ue_CapabilityRAT_Container.data,
                        p_elem->ue_CapabilityRAT_Container.numocts,
                        FALSE);

                if (RT_OK != asn1PD_nr_rrc_UE_MRDC_Capability(
                            pctxt,
                            &cap_buff))
                {
                    rtxErrPrint(pctxt);
                    RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                            (p_ue_context->p_gb_context)->facility_name,
                            RRC_FATAL,
                            "ASN1 decoding failed for nr_rrc_UE_MRDC_Capability");
                }
                else
                {

                    S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);

                    if(pBuff != NULL)
                    {
                        memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);

                        /* Converted to char* as required by ASN function */
                        rrc_result = asn1PrtToStr_nr_rrc_UE_MRDC_Capability(
                                "EUTRA-NR UE CapabilityRAT Container --> nr_rrc_UE_MRDC_Capability",
                                &cap_buff,
                                (char*)pBuff,
                                MAX_ASN_PRINT_BUFF_SIZE);

                        if(rrc_result == 0)
                        {
                            RRC_TRACE_ASN((S32)RRC_ASN, "%s\n", pBuff);
                        }
                        else
                        {
                            RRC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be increased");
                        }
                        rrc_mem_free(pBuff);
                    }
                }
            }

            rtxDListAppendNode(
                    &nr_ue_CapabilityRAT_ContainerList,
                    p_node);
        }
    }

    pu_setBuffer (pctxt, msg, X2AP_MAX_ASN1_BUF_LEN, FALSE);

    if (RT_OK != asn1PE_nr_rrc_UE_CapabilityRAT_ContainerList(
                pctxt, 
                &nr_ue_CapabilityRAT_ContainerList))
    {
        RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                (p_ue_context->p_gb_context)->facility_name,
                RRC_FATAL,
                "ASN1 encoding failed for nr_rrc_UE_CapabilityRAT_Container");
        result = RRC_FAILURE;
    }
    else
    {
        p_ue_CapabilityInfo->numocts = 
            (U16)pe_GetMsgLen (pctxt);
        p_ue_CapabilityInfo->data = 
            (U8 *)rtxMemAlloc (pctxt, p_ue_CapabilityInfo->numocts);
        if (PNULL == p_ue_CapabilityInfo->data)
        {
            RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                    (p_ue_context->p_gb_context)->facility_name,
                    RRC_FATAL, "ASN malloc failed.");
            result = RRC_FAILURE;
        }
        else
        {
            l3_memcpy_wrapper(
                    (void *)p_ue_CapabilityInfo->data,
                    msg,
                    p_ue_CapabilityInfo->numocts);
        }
        /* Init ASN context */
        if (RT_OK != rtInitContext(&asn1_ctx))
        {
            /* ASN context initialization failed */
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, 
                    RRC_ERROR, "[x2ap_SgNBAdditionRequest] "
                    "ASN context initialization failed "
                    "MeNB UE X2AP ID [%u] "
                    "UE Index: [%u] "
                    "Cell Index: [%u]", 
                    p_ue_context->x2ap_context.menb_ue_x2ap_id,
                    p_ue_context->ue_index,
                    p_ue_context->cell_index);

            RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
            return RRC_FAILURE;
        }

        if(RRC_SUCCESS == rrc_check_logger_type(RRC_ASN))
        {
            U32                                     encoded_msg_len = 0;

            encoded_msg_len = (U32)pe_GetMsgLen(pctxt);
            pu_setBuffer(&asn1_ctx, msg,
                    (encoded_msg_len), FALSE);


            if (RT_OK == asn1PD_nr_rrc_UE_CapabilityRAT_ContainerList(
                        &asn1_ctx, 
                        &nr_ue_CapabilityRAT_ContainerList_1))
            {

                S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);
                if(pBuff != NULL)
                {
                    memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);

                    /* Converted to char* as required by ASN function */
                    rrc_result = asn1PrtToStr_nr_rrc_UE_CapabilityRAT_ContainerList(
                            "nr_rrc_UE_CapabilityRAT_Container",
                            &nr_ue_CapabilityRAT_ContainerList_1,
                            (char*)pBuff,
                            MAX_ASN_PRINT_BUFF_SIZE);

                    if(rrc_result == 0)
                    {
                        RRC_TRACE_ASN((S32)RRC_ASN, "%s\n", pBuff);
                    }
                    else
                    {
                        RRC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be increased");
                    }
                    rrc_mem_free(pBuff);
                }
                rtFreeContext(&asn1_ctx);
            }
            else
            {
                RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                        (p_ue_context->p_gb_context)->facility_name,
                        RRC_FATAL,
                        "ASN1 decoding failed for nr_rrc_UE_CapabilityRAT_Container");
                rtxErrPrint(&asn1_ctx);
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/******************************************************************************
 *   FUNCTION NAME: rrc_check_if_nr_capab_present 
 *
 *   DESCRIPTION:
 *       This function Checks if NR/NR-DC Capab is present in UECapabilityInfo.
 *
 *   RETURNS:
 *       RRC_TRUE/RRC_FALSE
 ******************************************************************************/
rrc_bool_et rrc_check_if_nr_capab_present(
        uecc_ue_context_t               *p_uecc_ue_context)
{
    rrc_bool_et result = RRC_FALSE;
    UE_Radio_Capability_RAT_Container_t 
        *p_ue_radio_capability = PNULL;

    for (p_ue_radio_capability = (UE_Radio_Capability_RAT_Container_t*) 
            ylFirst (&p_uecc_ue_context->ue_radio_capability);
            p_ue_radio_capability; 
            p_ue_radio_capability = (UE_Radio_Capability_RAT_Container_t*) 
            ylNext((YLNODE*)p_ue_radio_capability))
    {
        if ( (RRC_TRUE == p_ue_radio_capability->valid_rat_string) && 
                /*Check for NR / EUTRA_NR RAT Types*/
                ( (nr == p_ue_radio_capability->rat_type) || 
                  (eutra_nr == p_ue_radio_capability->rat_type) 
                ) 
           )
        {
            result = RRC_TRUE;
            break;
        }
    }

    if(RRC_TRUE == result)
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                "NR/MRDC Capability Info present"
                " in UECapability");
    }
    else
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                "NR/MRDC Capability Info not present"
                " in UECapability");
    }

    return result;
}
/* OPTION3X Changes Start */
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_mcg_rb_config  
 *
 *   DESCRIPTION:
 *       This function builds mcg_RB_Config(nr_rrc_RadioBearerConfig) 
 *
 *   RETURNS: RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
static rrc_return_et uecc_x2ap_build_mcg_rb_config(
        uecc_ue_context_t               *p_uecc_ue_context,
        OSDynOctStr                     *p_asn1_oct_str,
        radio_bearer_config_t           *p_rrm_mcg_rb_config,
        OSCTXT                          *p_asn1_ctx
        )
{
    rrc_return_et               result  = RRC_FAILURE;
    rrc_return_t                    rrc_result =0;
    nr_rrc_RadioBearerConfig    mcg_rb_config; 
    OSRTDListNode               *p_node = RRC_NULL;
    nr_rrc_DRB_ToAddMod         *p_elem = RRC_NULL;
    U8                          drb_idx = RRC_ZERO;
//    nr_rrc_PDCP_Config*         p_pdcp_conf = PNULL;

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_asn1_oct_str);
    RRC_ASSERT(PNULL != p_rrm_mcg_rb_config);
    RRC_ASSERT(PNULL != p_asn1_ctx);

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    memset_wrapper(&mcg_rb_config, RRC_ZERO, sizeof(nr_rrc_RadioBearerConfig));

    do
    {
        /*Fill drb_ToAddModList*/
        if ( RADIO_BEARER_CONFIG_DRB_TO_ADD_MOD_LIST_PRESENT &
                p_rrm_mcg_rb_config->bitmask)
        {
            /* List Init Done */
            asn1Init_nr_rrc_DRB_ToAddModList(&mcg_rb_config.drb_ToAddModList);
            /* Condition to be added */
            for(drb_idx = RRC_ZERO;
                    drb_idx < p_rrm_mcg_rb_config->drb_to_add_mod_list.count;
                    drb_idx++)
            {
                rtxDListAllocNodeAndData(p_asn1_ctx,
                        nr_rrc_DRB_ToAddMod,
                        &p_node,
                        &p_elem);

                if (PNULL == p_node)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_FATAL,"Can't allocate p_node "
                            "MeNB UE X2AP ID [%u] "
                            "Peer eNodeB Id: [%u] "
                            "UE Index: [%u] "
                            "Cell Index: [%u]",
                            p_uecc_ue_context->x2ap_context.menb_ue_x2ap_id,
                            p_uecc_ue_context->x2ap_context.peer_enodeb_id,
                            p_uecc_ue_context->ue_index,
                            p_uecc_ue_context->cell_index);

                    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }

                asn1Init_nr_rrc_DRB_ToAddMod(p_elem);

                /*Update DRB to ADDMod*/
                {
                    /* Update DRB_ID */
                    p_elem->drb_Identity = p_rrm_mcg_rb_config->drb_to_add_mod_list.
                        drb_to_add_mod[drb_idx].drb_id;

                    /*cnAssociation*/
                    if(DRB_TO_ADD_MOD_CN_ASSOCIATION_PRESENT &
                            p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].bitmask)
                    {
                        p_elem->m.cnAssociationPresent = RRC_ONE;

                        if ( CN_ASSOCIATION_ERAB_ID_PRESENT &
                            p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                            cn_association.bitmask )
                        {
                            p_elem->cnAssociation.t =
                                T_nr_rrc_DRB_ToAddMod_cnAssociation_eps_BearerIdentity;

                            /* Update ERAB_ID */
                            p_elem->cnAssociation.u.eps_BearerIdentity = p_rrm_mcg_rb_config->drb_to_add_mod_list.
                                drb_to_add_mod[drb_idx].cn_association.erab_id;
                        }
                    }
                    /*reestablishPDCP :  Not Supported*/
                    if(DRB_TO_ADD_MOD_REESTABLISH_PDCP_PRESENT &
                            p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].bitmask)
                    {
                        //p_elem->m.reestablishPDCPPresent = RRC_ONE;
                    }
                    /*recoverPDCP : Not Supported*/
                    if(DRB_TO_ADD_MOD_RECOVER_PDCP_PRESENT &
                            p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].bitmask)
                    {
                        //p_elem->m.recoverPDCPPresent = RRC_ONE;
                    }
                    /*pdcp_Config :  Not Supported*/
#if 0
                    if(DRB_TO_ADD_MOD_PDCP_CONFIG_PRESENT &
                            p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].bitmask)
                    {
                        p_elem->m.pdcp_ConfigPresent = RRC_ONE;

                        p_elem->pdcp_Config.m.drbPresent = RRC_ONE;
                        p_elem->pdcp_Config.m.moreThanOneRLCPresent = RRC_NULL;
                        p_elem->pdcp_Config.m.t_ReorderingPresent = RRC_NULL;

                        p_pdcp_conf = &(p_elem->pdcp_Config);
 
                        p_pdcp_conf->m.drbPresent = RRC_ONE;

                        if ( RRC_PDCP_CONFIG_DISCARD_TIMER_PRESENT &
                            p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                                pdcp_config.bitmask )
                        {
                            p_pdcp_conf->drb.m.discardTimerPresent = RRC_ONE;

                            /* discardTimer */
                            p_pdcp_conf->drb.discardTimer = p_rrm_mcg_rb_config->drb_to_add_mod_list.
                                drb_to_add_mod[drb_idx].pdcp_config.discard_timer;
                        }

                        if ( RRC_PDCP_HEADER_COMPRESSION_ROHC_CONFIG_PRESENT &
                            p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                                pdcp_config.header_compression.bitmask )
                        {
                            /* ROHC enabled */
                            p_pdcp_conf->drb.headerCompression.t =
                                T_PDCP_Config_headerCompression_rohc;

                            p_pdcp_conf->drb.headerCompression.u.rohc =
                                rtxMemAllocType(p_asn1_ctx, nr_rrc_PDCP_Config_drb_headerCompression_rohc);
                            if (PNULL == p_pdcp_conf->drb.headerCompression.u.rohc)
                            {
                                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                                        p_uecc_ue_context->p_gb_context->facility_name,
                                        RRC_ERROR,
                                        "generate_drb_to_add_mod: ASN malloc failed");
                                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                                return RRC_FAILURE;
                            }

                            p_pdcp_conf->drb.headerCompression.u.rohc->m.drb_ContinueROHCPresent =
                                RRC_NULL;

                            /* maxCID */
                            p_pdcp_conf->drb.headerCompression.u.rohc->maxCID =
                                p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                                    pdcp_config.header_compression.rohc_config.max_cid;

                            /* profiles */
                            p_pdcp_conf->drb.headerCompression.u.rohc->profiles.profile0x0001 =
                                p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                                    pdcp_config.header_compression.rohc_config.
                                        rohc_profile.profile0x0001;
                            p_pdcp_conf->drb.headerCompression.u.rohc->profiles.profile0x0002 =
                                p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                                    pdcp_config.header_compression.rohc_config.
                                        rohc_profile.profile0x0002;
                            p_pdcp_conf->drb.headerCompression.u.rohc->profiles.profile0x0003 =
                                p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                                    pdcp_config.header_compression.rohc_config.
                                        rohc_profile.profile0x0003;
                            p_pdcp_conf->drb.headerCompression.u.rohc->profiles.profile0x0004 =
                                p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                                    pdcp_config.header_compression.rohc_config.
                                        rohc_profile.profile0x0004;
                            p_pdcp_conf->drb.headerCompression.u.rohc->profiles.profile0x0006 =
                                p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                                    pdcp_config.header_compression.rohc_config.
                                        rohc_profile.profile0x0006;
                            p_pdcp_conf->drb.headerCompression.u.rohc->profiles.profile0x0101 =
                                p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                                    pdcp_config.header_compression.rohc_config.
                                        rohc_profile.profile0x0101;
                            p_pdcp_conf->drb.headerCompression.u.rohc->profiles.profile0x0102 =
                                p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                                    pdcp_config.header_compression.rohc_config.
                                        rohc_profile.profile0x0102;
                            p_pdcp_conf->drb.headerCompression.u.rohc->profiles.profile0x0103 =
                                p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                                    pdcp_config.header_compression.rohc_config.
                                        rohc_profile.profile0x0103;
                            p_pdcp_conf->drb.headerCompression.u.rohc->profiles.profile0x0104 =
                                p_rrm_mcg_rb_config->drb_to_add_mod_list.drb_to_add_mod[drb_idx].
                                    pdcp_config.header_compression.rohc_config.
                                        rohc_profile.profile0x0104;
                        }
                        else
                        {
                            p_pdcp_conf->drb.headerCompressiont.t =
                                T_nr_rrc_PDCP_Config_drb_headerCompression_notUsed;
                        }
                    }
#endif
                }
                /*append node to list*/
                rtxDListAppendNode(
                        &mcg_rb_config.drb_ToAddModList,
                        p_node);
            }
            /* Update drb_ToAddModList Flag */
            mcg_rb_config.m.drb_ToAddModListPresent = RRC_ONE;
        }

        if (X2AP_SUCCESS != x2ap_internal_encode_nr_rrc_radio_bearer_config(
                    p_asn1_ctx,
                    &mcg_rb_config,
                    p_asn1_oct_str 
                    ))
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[uecc_x2ap_build_mcg_rb_config]"
                    " x2ap_internal_encode_nr_rrc_radio_bearer_config"
                    " returned failure");
            break;
        }
        else
        {
            if(RRC_SUCCESS == rrc_check_logger_type(RRC_ASN))
            {
                S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);

                if(pBuff != NULL)
                {
                    memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);

                    /* Converted to char* as required by ASN function */
                    rrc_result = asn1PrtToStr_nr_rrc_RadioBearerConfig(
                            "nr_rrc_RadioBearerConfig --> mcg_RB_Config",
                            &mcg_rb_config,
                            (char*)pBuff,
                            MAX_ASN_PRINT_BUFF_SIZE);

                    if(rrc_result == 0)
                    {
                        RRC_TRACE_ASN((S32)RRC_ASN, "%s\n", pBuff);
                    }
                    else
                    {
                        RRC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be increased");
                    }
                    rrc_mem_free(pBuff);
                }
            }
        }

        result = RRC_SUCCESS;
    } while(RRC_ZERO);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}
/* OPTION3X Changes End */
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_rrc_menb_to_sgnb_container 
 *
 *   DESCRIPTION:
 *       This function builds CG Config Info Container, send to SgNB.
 *
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_build_rrc_menb_to_sgnb_container(
        uecc_ue_context_t               *p_uecc_ue_context,
        x2ap_MeNBtoSgNBContainer        *p_menb_to_sgnb_container,
        OSCTXT                          *pctxt)
{
    rrc_return_et               result                 = RRC_FAILURE;
    nr_rrc_CG_ConfigInfo        nr_cg_config_info;
    nr_rrc_CG_ConfigInfo        *p_cg_config_info      = PNULL;
    cg_config_info_t            *p_rrm_cg_config_info  = PNULL;    
    nr_rrc_CG_ConfigInfo_IEs    *p_cg_conf_info_IEs    = PNULL; 

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_menb_to_sgnb_container);
    RRC_ASSERT(PNULL != pctxt);

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    p_cg_config_info = &nr_cg_config_info; 

/* OPTION3X Changes Start */
    if( p_uecc_ue_context->p_curr_proc_data && //add for bugid 117724
		(UECC_RRM_DC_BEARER_CHANGE_PROC ==
            p_uecc_ue_context->p_curr_proc_data->t))
    {
        /* In Case of DC Bearer Change Procedure */
        p_rrm_cg_config_info =
            &(p_uecc_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                    sgnb_add_param.cg_config_info);
    }
    else
    {
        /* In Case of ICS/ERABSP Procedure */
        p_rrm_cg_config_info =
            &(p_uecc_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                    uecc_cg_config_info);
    }
/* OPTION3X Changes End */

    memset_wrapper(p_cg_config_info, 0, sizeof(nr_rrc_CG_ConfigInfo));

    do
    {
        /* Fill CG Config Info */
        p_cg_config_info->criticalExtensions.t =
            T_nr_rrc_CG_ConfigInfo_criticalExtensions_c1;

        p_cg_config_info->criticalExtensions.u.c1 =
            (nr_rrc_CG_ConfigInfo_criticalExtensions_c1*)
            rtxMemAllocType(pctxt, nr_rrc_CG_ConfigInfo_criticalExtensions_c1);

        if (PNULL == p_cg_config_info->criticalExtensions.u.c1)
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_FATAL,
                    "[CG Config Info] ASN malloc failed.");
            break;
        }

        p_cg_config_info->criticalExtensions.u.c1->t =
            T_nr_rrc_CG_ConfigInfo_criticalExtensions_c1_cg_ConfigInfo;

        p_cg_config_info->criticalExtensions.u.c1->u.cg_ConfigInfo =
            (nr_rrc_CG_ConfigInfo_IEs*)
            rtxMemAllocType(pctxt, nr_rrc_CG_ConfigInfo_IEs);

        if (PNULL == p_cg_config_info->criticalExtensions.u.c1->
                u.cg_ConfigInfo)
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_FATAL,
                    "[CG Config Info] ASN malloc failed.");
            break;
        }

        p_cg_conf_info_IEs = p_cg_config_info->criticalExtensions.u.c1->u.
            cg_ConfigInfo;
        asn1Init_nr_rrc_CG_ConfigInfo_IEs(p_cg_conf_info_IEs);

        if (PNULL != p_rrm_cg_config_info)
        {
            if(RRC_TRUE == rrc_check_if_nr_capab_present(p_uecc_ue_context))
            {
                /*Fill the ue_CapabilityInfoPresent*/
                p_cg_conf_info_IEs->m.ue_CapabilityInfoPresent = 1;

                /* Fill the ue_CapabilityInfo string */
                if(RRC_SUCCESS != 
                        uecc_x2ap_fill_ue_capabilityinfo_from_ue_capability_rat_cont_list(
                            p_uecc_ue_context,
                            &p_cg_conf_info_IEs->ue_CapabilityInfo,
                            pctxt))
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_FATAL,
                            "[CG Config Info] Error occurred in filling ue_capabilityInfo"
                            " in cg-ConfigInfo.");
                    break;
                }
                else
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                            "[CG Config Info] ue_capabilityInfo updated"
                            " in cg-ConfigInfo.");
                }
            }

            /*Update the candidateCellInfoListMN*/
            if(p_rrm_cg_config_info->bitmask &
                    NR_CG_CONFIG_INFO_MN_CANDID_CELL_INFO_PRESENT)
            {
                if (RRC_SUCCESS != uecc_x2ap_build_mn_candidate_cell_info_list(
                            p_uecc_ue_context,
                            &p_cg_conf_info_IEs->candidateCellInfoListMN,
                            &p_rrm_cg_config_info->candidate_cell_info_list_mn,
                            pctxt))
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_FATAL,
                            "[CG Config Info] Error occurred in filling candidateCellInfoListMN"
                            " in cg-ConfigInfo.");
                    break;
                }
                else
                {
                    p_cg_conf_info_IEs->m.candidateCellInfoListMNPresent = 1;

                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                            "[CG Config Info] MN Candid Cell Info updated"
                            " in cg-ConfigInfo.");
                }
            }

            /*update Config restricted info*/
            if(p_rrm_cg_config_info->bitmask &
                    NR_CG_CONFIG_INFO_CONFIG_RESTRICT_INFO_PRESENT)
            {
                p_cg_conf_info_IEs->m.configRestrictInfoPresent = 1;

                if(RRC_SUCCESS == uecc_x2ap_build_config_restrict_info_scg(
                            p_uecc_ue_context,
                            &p_cg_conf_info_IEs->configRestrictInfo,
                            &p_rrm_cg_config_info->config_restrict_info_scg,pctxt))
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                            "[CG Config Info] configRestrictInfo updated"
                            " in cg-ConfigInfo.");
                }
            }

            /*update drx-info MCG*/
            if ( NR_CG_CONFIG_INFO_DRX_INFO_MCG_PRESENT &
                        p_rrm_cg_config_info->bitmask )
            {
                if ( ( RRC_DRX_CONFIG_PARAM_PRESENT &
                            p_rrm_cg_config_info->drx_config_mcg.bitmask ) &&
                        ( RRC_ONE ==  p_rrm_cg_config_info->drx_config_mcg.drx_config_type ) )
                {
                    p_cg_conf_info_IEs->m.drx_InfoMCGPresent = RRC_ONE;

                    if ( RRC_SUCCESS == uecc_x2ap_build_drx_info_mcg(
                                p_uecc_ue_context,
                                &p_cg_conf_info_IEs->drx_InfoMCG,
                                &p_rrm_cg_config_info->drx_config_mcg,pctxt))
                    {
                        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                                p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                                "[CG Config Info] drx_config_mcg updated"
                                " in cg-ConfigInfo.");
                    }
                }
                else
                {
                    p_cg_conf_info_IEs->m.drx_InfoMCGPresent = RRC_NULL;
                }
            }


            /* OPTION3X Changes Start */
            /* update mcg_RB_Config */
            if(p_rrm_cg_config_info->bitmask &
                    NR_CG_CONFIG_INFO_MCG_RB_CONFIG_PRESENT)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                            "[CG Config Info] updating mcg_RB_Config"
                            " in cg-ConfigInfo.");
                if(RRC_SUCCESS != uecc_x2ap_build_mcg_rb_config(
                            p_uecc_ue_context,
                            &p_cg_conf_info_IEs->mcg_RB_Config,
                            &p_rrm_cg_config_info->mcg_rb_config,
                            pctxt))
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                            "[CG Config Info] updating mcg_RB_Config"
                            " in cg-ConfigInfo failed.");
                    break;
                }
                else
                {
                    p_cg_conf_info_IEs->m.mcg_RB_ConfigPresent = 1;

                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                            "[CG Config Info] mcg_RB_Config updated successfully"
                            " in cg-ConfigInfo.");
                }
            }
            /* OPTION3X Changes End */
        }
        else
        {
            RRC_UECC_TRACE(
                    p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, 
                    RRC_FATAL,
                    "CG Config Info not present, returning failure");
            break;
        }

        if (X2AP_SUCCESS != x2ap_internal_encode_menb_to_sgnb_container(
                    pctxt,
                    p_cg_config_info,
                    p_menb_to_sgnb_container 
                    ))
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[uecc_x2ap_build_rrc_menb_to_sgnb_container]"
                    " x2ap_internal_encode_menb_to_sgnb_container"
                    " returned failure");

            break;
        }
        result = RRC_SUCCESS;

    }while(0);
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_menb_cell_id 
 *
 *   Input:  
 *          uecc_ue_context_t*            p_ue_context
 *          x2ap_ECGI*                    p_menb_cell_id
 *   Outputs :  None
 *   DESCRIPTION:
 *       This function updates the menb-cell-id from p_ue_context
 *
 *   RETURNS: None
 *
 ******************************************************************************/
    void uecc_x2ap_build_menb_cell_id 
(
 uecc_ue_context_t          *p_ue_context,      /* UE context */
 x2ap_ECGI                  *p_menb_cell_id
 )
{
    p_menb_cell_id->m.iE_ExtensionsPresent = 0;

    /* eutran_cgi.pLMNidentity*/
    uecc_x2ap_generate_plmn_identity_int(
            p_menb_cell_id->pLMN_Identity.data,
            &p_ue_context->plmn_identity.plmn_identity);

    p_menb_cell_id->pLMN_Identity.numocts =
        sizeof(p_menb_cell_id->pLMN_Identity.data);

    if (PNULL != p_ue_context->p_gb_context->p_p_csc_context)
    {
        if (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                [p_ue_context->cell_index])
        {
            if (PNULL != p_ue_context->p_gb_context->p_p_csc_context
                    [p_ue_context->cell_index]->p_csc_init_setup_ind)
            {
                /* eutran_cgi.cell_ID */
                /* 28 bits */
                RRC_ASSERT(sizeof(p_menb_cell_id->eUTRANcellIdentifier.data) ==
                        sizeof(p_ue_context->p_gb_context->p_p_csc_context[
                            p_ue_context->cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
                            cell_access_related_info.cell_Id));

                l3_memcpy_wrapper(p_menb_cell_id->eUTRANcellIdentifier.data,
                        p_ue_context->p_gb_context->p_p_csc_context[
                        p_ue_context->cell_index]->p_csc_init_setup_ind->sib_type_1_Info.
                        cell_access_related_info.cell_Id,
                        sizeof(p_menb_cell_id->eUTRANcellIdentifier.data));

            }
        }
    }
    p_menb_cell_id->eUTRANcellIdentifier.numbits = RRC_CELL_IDENTITY_BITS;
}

/******************************************************************************
 *   FUNCTION NAME: rrc_populate_ho_restriction_list 
 *   Input        : uecc_ue_context_t*           p_ue_context
 *                  x2ap_HandoverRestrictionList *p_x2ap_ho_restrict_list
 *                  OSCTXT                       *p_asn1_ctx
 *   Outputs      : None
 *   DESCRIPTION  : This function build the handover restriction list which will
                    further added to HO_REQUEST message going to target eNB.
 *   RETURNS      : RRC_SUCCESS or RRC_FAILURE
 *******************************************************************************/
static rrc_return_et
rrc_populate_ho_restriction_list
(
    uecc_ue_context_t               *p_ue_context,
    x2ap_HandoverRestrictionList    *p_x2ap_ho_restrict_list,
    OSCTXT                          *p_asn1_ctxt
)
{
    rrc_return_et           result = RRC_SUCCESS;
    U8                      count = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctxt);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* filling serving plmn (M) */
    asn1Init_x2ap_PLMN_Identity(&p_x2ap_ho_restrict_list->\
            servingPLMN);

    p_x2ap_ho_restrict_list->servingPLMN.numocts
        = p_ue_context->ics_handover_restriction_list.servingPLMN.numocts;

    l3_memcpy_wrapper((void *)p_x2ap_ho_restrict_list->servingPLMN.\
                       data,
            (const void *)p_ue_context->ics_handover_restriction_list.\
                       servingPLMN.data,
            p_ue_context->ics_handover_restriction_list.servingPLMN.numocts);

    /* filling equivalent plmn (O) */
    asn1Init_x2ap_EPLMNs(&p_x2ap_ho_restrict_list->\
                            equivalentPLMNs);

    p_x2ap_ho_restrict_list->m.equivalentPLMNsPresent = RRC_NULL;

    if ( p_ue_context->ics_handover_restriction_list.m.equivalentPLMNsPresent )
    {
        p_x2ap_ho_restrict_list->m.equivalentPLMNsPresent = RRC_ONE;

        p_x2ap_ho_restrict_list->equivalentPLMNs.n
            = p_ue_context->ics_handover_restriction_list.equivalentPLMNs.n;

        for (; count <(p_ue_context->ics_handover_restriction_list.\
                        equivalentPLMNs.n); count++ )
        {
            p_x2ap_ho_restrict_list->equivalentPLMNs.\
                    elem[count].numocts
                = p_ue_context->ics_handover_restriction_list.equivalentPLMNs.\
                    elem[count].numocts;

            l3_memcpy_wrapper((void *)p_x2ap_ho_restrict_list->\
                    equivalentPLMNs.elem[count].data,
                    (const void *)p_ue_context->ics_handover_restriction_list.\
                    equivalentPLMNs.elem[count].data,
                    p_ue_context->ics_handover_restriction_list.\
                        equivalentPLMNs.elem[count].numocts);
        }
    }

    /* filling forbidden TAs (O) */
    p_x2ap_ho_restrict_list->m.forbiddenTAsPresent = RRC_NULL;

    if ( p_ue_context->ics_handover_restriction_list.m.forbiddenTAsPresent )
    {
        p_x2ap_ho_restrict_list->m.forbiddenTAsPresent = RRC_ONE;

        asn1Copy_x2ap_ForbiddenTAs(p_asn1_ctxt,  
                &(p_ue_context->ics_handover_restriction_list.forbiddenTAs),
                &(p_x2ap_ho_restrict_list->forbiddenTAs));
    }

    /* filling forbidden LAs (O) */
    p_x2ap_ho_restrict_list->m.forbiddenLAsPresent = RRC_NULL;
    if ( p_ue_context->ics_handover_restriction_list.m.forbiddenLAsPresent )
    {
        p_x2ap_ho_restrict_list->m.forbiddenLAsPresent = RRC_ONE;

        asn1Copy_x2ap_ForbiddenTAs(p_asn1_ctxt,  
                &(p_ue_context->ics_handover_restriction_list.forbiddenLAs),
                &(p_x2ap_ho_restrict_list->forbiddenLAs));
    }

    /* filling forbidden Inter RATs (O) */
    p_x2ap_ho_restrict_list->m.forbiddenInterRATsPresent = RRC_NULL;
    if ( p_ue_context->ics_handover_restriction_list.m.forbiddenInterRATsPresent )
    {

        p_x2ap_ho_restrict_list->m.forbiddenInterRATsPresent = RRC_ONE;

        p_x2ap_ho_restrict_list->forbiddenInterRATs
            = p_ue_context->ics_handover_restriction_list.forbiddenInterRATs;
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_sgnb_addtion_request
 *
 *   DESCRIPTION:
 *       This function builds and sends SgNB Addition Request message
 *       to X2AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_build_and_send_sgnb_addtion_request(
        uecc_ue_context_t           *p_ue_context
        )
{
    U32                                 menb_ueid           = RRC_NULL;
    //x2ap_UE_X2AP_ID                     menb_ue_x2ap_id     = RRC_NULL;
    //x2ap_UE_X2AP_ID_Extension           menb_ue_x2ap_id_ext = RRC_INVALID_DC_UE_X2AP_ID;
    rrc_return_et                       result              = RRC_FAILURE;
    OSCTXT                              asn1_ctx;
    rrc_x2ap_sgnb_addition_request_t    msg;
    x2ap_MeNBtoSgNBContainer            menb_to_sgnb_transparent_container;

    OSRTDListNode               *p_node = RRC_NULL;
    x2ap_UEAggregateMaximumBitRate_iE_Extensions_element *p_elem = RRC_NULL;

    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;

    RRC_ASSERT(PNULL != p_ue_context);

    /* Check if Global UE context is NULL. */
    if(PNULL == p_ue_context->p_gb_context)
    {
        /* uecc_global context is null */
        return RRC_FAILURE;
    }
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        /* ASN context initialization failed */
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, 
                RRC_ERROR, "[x2ap_SgNBAdditionRequest] "
                "ASN context initialization failed "
                "MeNB UE X2AP ID [%u] "
                "UE Index: [%u] "
                "Cell Index: [%u]", 
                p_ue_context->x2ap_context.menb_ue_x2ap_id,
                p_ue_context->ue_index,
                p_ue_context->cell_index);

        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    if ( RRC_INVALID_UE_INDEX  == p_ue_context->x2ap_context.menb_ue_x2ap_id )
    {
        /* Creation of MeNB UE X2AP_UEID */
        if (RRC_INVALID_UE_INDEX == (menb_ueid =
                    (U16)uecc_x2ap_ueid_allocate(
                        p_ue_context->p_gb_context,
                        p_ue_context->ue_index)))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_SgNBAdditionRequest] MeNB UE X2AP ID "
                    "Creation Failed");

            return RRC_FAILURE;
        }
    }

    /*Store generated MeNB UE X2AP ID*/
    p_ue_context->x2ap_context.menb_ue_x2ap_id = menb_ueid;

    /* Set MeNB Extention Present to TRUE,
     *  * MeNB UE ID Generated in 32 bit. */
#if 0
    if(menb_ueid > MAX_VALID_DC_UE_ID)
    {
        p_ue_context->x2ap_context.
            is_menb_ext_present = RRC_TRUE;
    }
#endif

    do
    {
        /* Reset message */
        memset_wrapper(&msg, RRC_NULL,
                sizeof(rrc_x2ap_sgnb_addition_request_t));

        /* Memset as all fields will not be used. */
        memset_wrapper(sig_msg_req_buf, RRC_NULL,
                sizeof(x2ap_ue_associated_sig_msg_req_t) + 
                X2AP_MAX_ASN1_BUF_LEN);

        /* ASN Initialization of MeNB to SgNB Container. */
        asn1Init_x2ap_MeNBtoSgNBContainer(
                &menb_to_sgnb_transparent_container);

#if 0
        /* Extract X2AP IDs */
        rrc_extract_nr_dc_x2_ids(p_ue_context,
                &menb_ue_x2ap_id,
                &menb_ue_x2ap_id_ext);
#endif

        /* Fill MeNB UE X2AP ID */
        msg.menb_ue_x2ap_id = p_ue_context->x2ap_context.menb_ue_x2ap_id;

        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                "[x2ap_SgNBAdditionRequest] MeNB UE X2AP ID [%d] ",
                p_ue_context->x2ap_context.menb_ue_x2ap_id);

#if 0
        /* Fill MeNB UE X2AP ID Extension */
        if(p_ue_context->x2ap_context.is_menb_ext_present)
        {
            msg.menb_ue_x2ap_id_ext = menb_ue_x2ap_id_ext;
            msg.bitmask |= 
                X2AP_SGNB_ADD_REQ_MENB_UE_X2AP_ID_EXTENSION_PRESENT;
        }
#endif

        /*Fill NR UE Security Capabilities*/
        rrc_populate_nr_ue_security_capability(p_ue_context,
                &msg.nr_ue_security_capability,
                &asn1_ctx);

        /*Fill NR UE Security Capabilities*/
        rrc_populate_sgnb_securtiy_key(p_ue_context,
                &msg.sgnb_security_key);

        /*Fill SgNB UE Aggregate Maximum Bit Rate*/
        {
            msg.ue_aggregate_max_bitrate.uEaggregateMaximumBitRateDownlink = 
                (x2ap_BitRate)(p_ue_context->ue_agg_max_bit_rate_sgnb.\
                        ue_agg_max_bit_rate_dl);

            msg.ue_aggregate_max_bitrate.uEaggregateMaximumBitRateUplink = 
                (x2ap_BitRate)(p_ue_context->ue_agg_max_bit_rate_sgnb.\
                        ue_agg_max_bit_rate_ul);

            if ( RRC_RRM_EXT_UE_AGG_MAX_BITRATE_PRESENT &
                p_ue_context->ue_agg_max_bit_rate_sgnb.bitmask )
            {
                msg.ue_aggregate_max_bitrate.m.iE_ExtensionsPresent = RRC_ONE;

                rtxDListAllocNodeAndData(&asn1_ctx,
                        x2ap_UEAggregateMaximumBitRate_iE_Extensions_element,
                        &p_node,
                        &p_elem);

                if ( PNULL == p_node )
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, 
                            RRC_FATAL,"Can't allocate p_node for "
                            "x2ap_UEAggregateMaximumBitRate_iE_Extensions_element");

                    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }

                p_elem->id = ASN1V_x2ap_id_extended_uEaggregateMaximumBitRateUplink;

                p_elem->criticality = x2ap_ignore;

                p_elem->extensionValue.t = T358x2ap___x2ap_UEAggregate_MaximumBitrate_ExtIEs_2;

                p_elem->extensionValue.u._x2ap_UEAggregate_MaximumBitrate_ExtIEs_2 =
                    p_ue_context->ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_ext.
                        ext_ue_agg_max_bit_rate_ul; 

                /* append node to the list */
                rtxDListAppendNode(
                        &(msg.ue_aggregate_max_bitrate.iE_Extensions), 
                        p_node);

                rtxDListAllocNodeAndData(&asn1_ctx,
                        x2ap_UEAggregateMaximumBitRate_iE_Extensions_element,
                        &p_node,
                        &p_elem);

                if ( PNULL == p_node )
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, 
                            RRC_FATAL,"Can't allocate p_node for "
                            "x2ap_UEAggregateMaximumBitRate_iE_Extensions_element");

                    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }

                p_elem->id = ASN1V_x2ap_id_extended_uEaggregateMaximumBitRateDownlink;
                p_elem->criticality = x2ap_ignore;
                p_elem->extensionValue.t = T358x2ap___x2ap_UEAggregate_MaximumBitrate_ExtIEs_1;
                p_elem->extensionValue.u._x2ap_UEAggregate_MaximumBitrate_ExtIEs_1 =
                    p_ue_context->ue_agg_max_bit_rate_sgnb.ue_agg_max_bit_rate_ext.
                        ext_ue_agg_max_bit_rate_dl; 

                /* append node to the list */
                rtxDListAppendNode(
                        &(msg.ue_aggregate_max_bitrate.iE_Extensions), 
                        p_node);
            }
            else
            {
                msg.ue_aggregate_max_bitrate.m.iE_ExtensionsPresent = RRC_NULL; 
            }
        }

        /*Fill Selected PLMN*/

        /*Fill Handover Restriction List*/
        if ( RRC_ONE == p_ue_context->m.ho_restriction_list_present )
        {
            msg.bitmask |=
                X2AP_SGNB_ADD_REQ_HO_RESTRICITION_LIST_PRESENT;

            rrc_populate_ho_restriction_list(p_ue_context,
                    &msg.ho_restricition_list,
                    &asn1_ctx);
        }

        /*Fill E-RABs To Be Added List */
        if(RRC_SUCCESS !=
                uecc_x2ap_build_x2ap_sgnb_add_e_rab_admitted_list(
                    p_ue_context,
                    &asn1_ctx,
                    &msg.erab_to_add_list))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_SgNBAdditionRequest]"
                    "uecc_x2ap_build_x2ap_sgnb_add_e_rab_admitted_list failure "
                    "MeNB UE X2AP ID [%u] "
                    "Peer eNodeB Id: [%u] "
                    "UE Index: [%u] "
                    "Cell Index: [%u]", 
                    msg.menb_ue_x2ap_id,
                    p_ue_context->x2ap_context.
                    peer_enodeb_id,
                    p_ue_context->ue_index,
                    p_ue_context->cell_index);
            break;
        }

        /*Prepare MeNB to SgNB Container*/
        if (RRC_SUCCESS != uecc_x2ap_build_rrc_menb_to_sgnb_container(
                    p_ue_context,
                    &msg.menb_to_sgnb_container,
                    &asn1_ctx))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_SgNBAdditionRequest]"
                    "uecc_x2ap_build_rrc_menb_to_sgnb_container "
                    "failure");
            break;
        }

        /*Fill Subscriber Profile ID For RFP*/
        if ( p_ue_context->m.SpId )
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                    "[x2ap_SgNBAdditionRequest] "
                    "spid present");
            msg.bitmask |=
                X2AP_SGNB_ADD_REQ_SUBSCRIBER_PROFILE_ID_FOR_RFP_PRESENT;

            msg.subscriber_profile_id_for_rfp = p_ue_context->spid;
        }

        /*prepare MeNb Cell ID*/
        uecc_x2ap_build_menb_cell_id(
                p_ue_context, &msg.menb_cell_id);

        /*Encode message*/
        result = x2ap_encode_sgNB_addition_request(
                &asn1_ctx,
                &p_sig_msg_req->message_buff_p[RRC_NULL],
                &p_sig_msg_req->message_buff_size,
                &msg);
        if(RRC_SUCCESS != result)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_SgNBAdditionRequest] Build failure "
                    "MeNB UE X2AP ID [%u] "
                    "Peer gNB Id: [%u] "
                    "UE Index: [%u] "
                    "Cell Index: [%u]", 
                    msg.menb_ue_x2ap_id,
                    p_ue_context->x2ap_context.
                    peer_enodeb_id,
                    p_ue_context->ue_index,
                    p_ue_context->cell_index);

            break;
        }

        /* Fill Procedure Code. */
        p_sig_msg_req->procedure_code = 
            ASN1V_x2ap_id_sgNBAdditionPreparation;

        /* Fill Message Type. */
        p_sig_msg_req->message_type = X2AP_INITIATING_MESSAGE;

        /* Fill Cell Index. */
        p_sig_msg_req->cell_index = p_ue_context->cell_index;

        /* Fill UE ID.*/
        p_sig_msg_req->ue_id = p_ue_context->ue_index;

        /* Fill MeNB UE X2AP ID. */
        p_sig_msg_req->bitmask |=
            X2AP_UE_ASSOC_SIG_MSG_REQ_MENB_UE_X2AP_ID_PRESENT;
        p_sig_msg_req->menb_ue_x2ap_id = 
            p_ue_context->x2ap_context.menb_ue_x2ap_id;

        /* Fill Peer gNB ID. */
        p_sig_msg_req->peer_enodeb_id = 
            p_ue_context->x2ap_context.peer_enodeb_id;

        /* Send message to X2AP  */
        result = uecc_rrc_send_internal_msg(
                RRC_X2AP_MODULE_ID,
                X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                    p_sig_msg_req->message_buff_size),
                sig_msg_req_buf);

        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, 
                RRC_INFO, "X2AP_MSG: SGNB ADDITION REQUEST "
                "MeNB UE X2AP ID [%u] "
                "Peer gNB Id: [%u] "
                "UE Index: [%u] Cell Index: [%u] %s", 
                msg.menb_ue_x2ap_id,
                p_ue_context->x2ap_context.
                peer_enodeb_id,
                p_ue_context->ue_index,
                p_ue_context->cell_index,
                change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
    }while(RRC_NULL);

    if( RRC_SUCCESS == result)
    {
        /* Reset x2apUecontextReleased to False */
        p_ue_context->x2apUecontextReleased = RRC_FALSE;
    }
    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_sgnb_addition_req_ack_msg
 *
 *   INPUTS        : OSCTXT*            p_asn1_ctx
 *                   x2ap_X2AP_PDU       *p_x2ap_pdu
 *                   uecc_gb_context_t *p_uecc_gb_context
 *                   U8                  peer_enodeb_id
 *   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function processes X2AP SgNB Addition Request Ack message
 *
 *   RETURNS:     None
 *
 ******************************************************************************/
void uecc_x2ap_process_sgnb_addition_req_ack_msg
( 
 OSCTXT*             p_asn1_ctx,         /* ASN Context Pointer */
 x2ap_X2AP_PDU       *p_x2ap_pdu,        /* pointer to X2AP PDU */
 uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
 U8                  peer_enodeb_id,     /* Peer EnodeB ID */
 U8                  *buffer
 )
{
	/*Error_Handling_changes*/
	U8 decoding_fail = RRC_SUCCESS;
	/*Error_Handling_changes*/
    uecc_ue_context_t                   *p_ue_context = PNULL;
    rrc_x2ap_sgnb_addition_req_ack_t    msg;

    RRC_ASSERT(PNULL!=p_x2ap_pdu);
    RRC_ASSERT(PNULL!=p_x2ap_pdu->u.successfulOutcome);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    memset_wrapper(&msg, 0, sizeof(rrc_x2ap_sgnb_addition_req_ack_t));
    do
    {
        if (PNULL == 
                p_x2ap_pdu->u.successfulOutcome->value.u.sgNBAdditionPreparation)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING,
                    "[X2AP SgNB Addition Request Ack] sgNBAdditionPreparation field is PNULL");
            break;
        }

        /* Decode message */ 
        if( X2AP_FAILURE == x2ap_internal_decode_sgnb_addition_request_ack(
                    p_asn1_ctx,
                    p_x2ap_pdu->u.successfulOutcome->value.u.sgNBAdditionPreparation,
                    &msg,
                    peer_enodeb_id))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, " SgNB Addition Request Ack "
                    "Decoding Failure");
            decoding_fail = RRC_FAILURE;
            break;
        }
        else
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, " SgNB Addition Request Ack "
                    "Decoding Success");

            if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
            {
                p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                        p_uecc_gb_context,
                        peer_enodeb_id, 
                        msg.menb_ue_x2ap_id, 
                        MENB_X2AP_UE_ID);
            }
/*Error_Handling_changes*/
            /*Check for API-ID missmatch*/
            if(RRC_FAILURE == check_for_api_id_mismatch(p_ue_context,
            	p_uecc_gb_context,
            	first_returned_message,
            	p_asn1_ctx,
            	msg.menb_ue_x2ap_id,
            	msg.sgnb_ue_x2ap_id,
            	p_x2ap_pdu->u.successfulOutcome->procedureCode,
            	p_x2ap_pdu->t,
            	peer_enodeb_id,
            	&msg
            	))
            {

            	break;
            }
/*Error_Handling_changes*/

            if (PNULL==p_ue_context )
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_ERROR, " SgNB Addition Request Ack "
                        "p_ue_context is PNULL!");
                break;

            }
            else
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_INFO, 
                        "[X2AP SgNB Addition Request Ack] UE context found");
            }

            if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
                        p_uecc_gb_context,p_ue_context->cell_index))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,"cell not available");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                return;
            }

            /* Call      event handler *
             * and pass API data to FSM. */
            /* WARNING. The uecc_event_queue_push_event can't be used here because
             * the stack variable is used for storing the API data
             * */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, 
                    "SgNB Addition Request Ack : push event UECC_EV_X2AP_SGNB_ADDITION_REQ_ACK");
            uecc_fsm_process_event(
                    p_ue_context,
                    UECC_EV_X2AP_SGNB_ADDITION_REQ_ACK,
                    &msg);
        }
    } while(RRC_NULL);

    /*Error_Handling_changes*/
    if(RRC_FAILURE == decoding_fail)
    {
        if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
        {
            p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                    p_uecc_gb_context,
                    peer_enodeb_id, 
                    msg.menb_ue_x2ap_id, 
                    MENB_X2AP_UE_ID);
        }
        if(PNULL != p_ue_context)
        {
            uecc_fsm_process_event(
                    p_ue_context,
                    UECC_EV_X2AP_ERROR_INDICATION,
                    &msg);
        }
    }
    /*Error_Handling_changes*/

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/****************************************************************************
 * Function Name  : uecc_x2ap_sgnb_add_req_ack_process_cg_config 
 * Inputs         : uecc_ue_context_t   *p_ue_context
 nr_rrc_CG_Config  *p_cg_config
 * Outputs        : None
 * Returns        : RRC_SUCCESS / RRC_FAILURE 
 * Description    : This process and stores cg_config in uecc_ctx.
 ****************************************************************************/
rrc_return_t uecc_x2ap_sgnb_add_req_ack_process_cg_config(
        uecc_ue_context_t   *p_ue_context,
        nr_rrc_CG_Config    *p_cg_config
        )
{
    rrc_return_t      result = RRC_SUCCESS;

    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_cg_config);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    do
    {
        if ((T_nr_rrc_CG_Config_criticalExtensions_c1 ==
                    p_cg_config->criticalExtensions.t) && 
                (PNULL != p_cg_config->criticalExtensions.u.c1) &&
                (T_nr_rrc_CG_Config_criticalExtensions_c1_cg_Config ==
                 p_cg_config->criticalExtensions.u.c1->t) &&
                (PNULL != p_cg_config->criticalExtensions.u.c1->u.cg_Config))
        {
            /* TO DO*/
            if(PNULL == p_ue_context->p_sn_cg_config)
            {
                p_ue_context->p_sn_cg_config = (rrc_sn_cg_config_t *)
                    rrc_mem_get(sizeof(rrc_sn_cg_config_t));

                if (PNULL== p_ue_context->p_sn_cg_config)
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,
                            RRC_FATAL,
                            "Memory Allocation Failed");
                    result = RRC_FAILURE;
                    break;
                }
                memset_wrapper(p_ue_context->p_sn_cg_config, 
                        RRC_NULL, 
                        sizeof(rrc_sn_cg_config_t));
            }

            /*scg_CellGroupConfig*/
            if(
                    p_cg_config->criticalExtensions.u.c1->
                    u.cg_Config->m.scg_CellGroupConfigPresent 
                    &&
                    RRC_NULL < p_cg_config->criticalExtensions.u.c1->u.
                    cg_Config->scg_CellGroupConfig.numocts
              )
            {
                p_ue_context->p_sn_cg_config->bitmask |=
                    X2AP_SN_CG_CONFIG_SCG_CELLGROUP_CONFIG_PRESENT;

                p_ue_context->p_sn_cg_config->scg_CellGroupConfig.numocts = 
                    p_cg_config->criticalExtensions.u.c1->u.cg_Config->scg_CellGroupConfig.numocts;

                if (PNULL == 
                        (p_ue_context->p_sn_cg_config->scg_CellGroupConfig.data = 
                         (OSOCTET*)
                         rrc_mem_get(p_ue_context->p_sn_cg_config->\
                             scg_CellGroupConfig.numocts)))
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,
                            RRC_FATAL, "[Process SgNBAddReqAck Nr_Cg_Config]"
                            "Memory Allocation Failed");
                    result = RRC_FAILURE;
                    break;
                }

                l3_memcpy_wrapper(
                        (void *)(p_ue_context->p_sn_cg_config->scg_CellGroupConfig.data),
                        (const void*)(p_cg_config->criticalExtensions.u.c1->\
                            u.cg_Config->scg_CellGroupConfig.data),
                        p_ue_context->p_sn_cg_config->scg_CellGroupConfig.numocts);
            }
            else
            {
                /* As only SN_TERMINATED_SCG_BEARER is supported the scg_CellGroupConfig is
                 * is mendatory as need to be send in RRCConnectReconfiguration message in
                 * RRCConnectionReconfiguration-v1510-IEs, thus failing if not present.
                 */
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,
                        RRC_ERROR, "[Process SgNBAddReqAck Nr_Cg_Config]"
                        "scg_CellGroupConfig not Present/Invalid.");
                result = RRC_FAILURE;
                break;
            }

            /*scg_RB_Config*/
            if(p_cg_config->criticalExtensions.u.c1->
                    u.cg_Config->m.scg_RB_ConfigPresent 
                    &&
                    RRC_NULL < p_cg_config->criticalExtensions.u.c1->u.
                    cg_Config->scg_RB_Config.numocts
              )
            {
                p_ue_context->p_sn_cg_config->bitmask |=
                    X2AP_SN_CG_CONFIG_SCG_RB_CONFIG_PRESENT;

                p_ue_context->p_sn_cg_config->scg_RB_Config.numocts = 
                    p_cg_config->criticalExtensions.u.c1->u.cg_Config->scg_RB_Config.numocts;

                if (PNULL == 
                        (p_ue_context->p_sn_cg_config->scg_RB_Config.data = 
                         (OSOCTET*)
                         rrc_mem_get(p_ue_context->p_sn_cg_config->\
                             scg_RB_Config.numocts)))
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name,
                            RRC_FATAL, "[Process SgNBAddReqAck Nr_Cg_Config]"
                            "Memory Allocation Failed");
                    result = RRC_FAILURE;
                    break;
                }

                l3_memcpy_wrapper(
                        (void *)(p_ue_context->p_sn_cg_config->scg_RB_Config.data),
                        (const void*)(p_cg_config->criticalExtensions.u.c1->\
                            u.cg_Config->scg_RB_Config.data),
                        p_ue_context->p_sn_cg_config->scg_RB_Config.numocts);

            }
            else
            {
                /* As only SN_TERMINATED_SCG_BEARER is supported the scg_RB_Config is
                 * is mendatory as need to be send in RRCConnectReconfiguration message in
                 * RRCConnectionReconfiguration-v1510-IEs, thus failing if not present.
                 */
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,
                        RRC_ERROR, "[Process SgNBAddReqAck Nr_Cg_Config]"
                        "scg_RB_Config not Present/Invalid.");
                result = RRC_FAILURE;
                break;
            }


            /*configRestrictModReq*/
            if(p_cg_config->criticalExtensions.u.c1->
                    u.cg_Config->m.configRestrictModReqPresent)
            {
            }

            /*drx_InfoSCG*/
            if(p_cg_config->criticalExtensions.u.c1->
                    u.cg_Config->m.drx_InfoSCGPresent)
            {
            }

            /*candidateCellInfoListSN*/
            if(p_cg_config->criticalExtensions.u.c1->
                    u.cg_Config->m.candidateCellInfoListSNPresent)
            {
            }

            /*measConfigSN*/
            if(p_cg_config->criticalExtensions.u.c1->
                    u.cg_Config->m.measConfigSNPresent)
            {
            }

            /*selectedBandCombinationNR*/
            if(p_cg_config->criticalExtensions.u.c1->
                    u.cg_Config->m.selectedBandCombinationNRPresent)
            {
            }

            /*fr_InfoListSCG*/
            if(p_cg_config->criticalExtensions.u.c1->
                    u.cg_Config->m.fr_InfoListSCGPresent)
            {
            }

        }
    } while(RRC_NULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/****************************************************************************
 * Function Name  : uecc_x2ap_read_all_success_admitted_erab_list
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAckList* - Success 
 *                  admitted eRABs
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion read success admitted eRABs list and 
 *                  put in share data
 ****************************************************************************/
rrc_return_t uecc_x2ap_read_all_success_admitted_erab_list(
        uecc_ue_context_t*                                  p_ue_context,    
        x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAckList*   p_erab_admitted_list
        )
{
    U8                           index             = RRC_NULL;
    U8                           erab_count        = RRC_NULL;
    U8                           erb_id_sgnb       = RRC_NULL;
    OSRTDListNode                *p_node           = PNULL; 
    uecc_rm_erab_setup_info_t    *p_erab_info_list = PNULL; 
/* OPTION3X Changes Start */
    rrc_drb_to_modify_list_t     *p_erab_data_list = PNULL; 
/* OPTION3X Changes End */
    x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAck_Item
        *p_erab_admitted_item             
        = PNULL;
/* OPTION3X Changes Start */
    U8                           erb_curr_idx      = RRC_NULL;
    U8                           erb_curr_proc_count = RRC_NULL;
/* OPTION3X Changes End */
    U8    erab_present_at_menb = RRC_NULL;
    OSRTDListNode                *p_node_pdcp      = PNULL; 
    x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAck_Item_SgNBPDCPpresent_iE_Extensions_element
        *p_pdcp_sn_size_item = PNULL;

    U8                           qci               = RRC_NULL;
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_erab_admitted_list); 

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

/* OPTION3X Changes Start */
    if (UECC_RRM_DC_BEARER_CHANGE_PROC !=
            p_ue_context->p_curr_proc_data->t)
    {
        p_erab_info_list = 
            &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                    erab_info_data);

        erb_curr_proc_count = p_erab_info_list->count;
    }
    else
    {
        p_erab_data_list =
            &(p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                    drb_to_modify_list);

        erb_curr_proc_count = p_erab_data_list->drb_count;
    }
/* OPTION3X Changes End */

    for (p_node = p_erab_admitted_list->head;
            (p_node != PNULL); p_node = p_node->next)
    {
        p_erab_admitted_item = 
            (x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAck_Item*) 
            &(((x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAckList_element *)(p_node->data))->value);

/* OPTION3X Changes Start */
        for (index = RRC_NULL; index < erb_curr_proc_count; index++)
        {
            if (
                    (
                     (UECC_RRM_DC_BEARER_CHANGE_PROC != p_ue_context->p_curr_proc_data->t)
                     && (p_erab_info_list->erab_info[index].erab_id == 
                         (U8)p_erab_admitted_item->e_RAB_ID)
                    )
                    ||
                    (
                     (UECC_RRM_DC_BEARER_CHANGE_PROC == p_ue_context->p_curr_proc_data->t)
                     && (p_erab_data_list->drb_config[index].erab_id == 
                         (U8)p_erab_admitted_item->e_RAB_ID)
                    )
               )
                /* OPTION3X Changes End */
            {
                erab_count++; 
                RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                        (p_ue_context->p_gb_context)->facility_name,
                        RRC_DETAILED,
                        "[SGNB_ADD_REQ_ACK] Admitted ERAB [%d] at SgNB is present "
                        "present at MeNB [%d]",
                        p_erab_admitted_item->e_RAB_ID,(p_erab_info_list != RRC_NULL ? p_erab_info_list->erab_info[index].erab_id:
                            p_erab_data_list->drb_config[index].erab_id));
                /*Fetch the eRAB_ID from received drb_config*/
                erb_id_sgnb = (U8)p_erab_admitted_item->e_RAB_ID;

                /* Check if E_RAB ID present in UE Context */
                if (p_ue_context->p_e_rab_list[erb_id_sgnb] == PNULL)
                {
                    RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                            (p_ue_context->p_gb_context)->facility_name,
                            RRC_ERROR,
                            "[SGNB_ADD_REQ_ACK] Unknown E_RAB ID Received : %u", 
                            erb_id_sgnb);
                    /*Logical_Error*/
                    uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                    continue;
                    /*Logical_Error*/
                }
                else if (p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type !=
                        SN_TERMINATED_SCG_BEARER
                        /* OPTION3X Changes Start */
                        &&
                        p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type !=
                        SN_TERMINATED_SPLIT_BEARER)
                    /* OPTION3X Changes End */
                {
                    RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                            (p_ue_context->p_gb_context)->facility_name,
                            RRC_ERROR,
                            "[SGNB_ADD_REQ_ACK] RAB ID [%u] : Bearer Type [%u] is not"
                            /* OPTION3X Changes Start */
                            " SN_TERMINATED_SCG_BEARER/SN_TERMINATED_SPLIT_BEARER", 
                            /* OPTION3X Changes End */
                            erb_id_sgnb, 
                            p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type);
					/*Logical_Error*/
                    uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                    continue;
                    /*Logical_Error*/


                }
                else
                {
                    if ( ( T_x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAck_Item_resource_configuration_sgNBPDCPpresent == 
                               p_erab_admitted_item->resource_configuration.t ) &&
                         ( PNULL != 
                             p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent ))
                    {
                        /* OPTION3X Changes Start */
                        if(UECC_RRM_DC_BEARER_CHANGE_PROC !=
                                p_ue_context->p_curr_proc_data->t)
                        {
                            p_erab_info_list->erab_info[index].status = RRC_NO_ERROR;
                        }
                        /* OPTION3X Changes End */

                        p_ue_context->p_e_rab_list[erb_id_sgnb]->sgnb_s1ap_gtp_id = 
                            p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                            s1_DL_GTPtunnelEndpoint.gTP_TEID;

                        if( RRC_SUCCESS != rrc_copy_transportlayer_address(
                                &p_ue_context->p_e_rab_list[erb_id_sgnb]->\
                                sgnb_s1ap_transportLayerAddress, 
                                &p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                s1_DL_GTPtunnelEndpoint.transportLayerAddress,
                                p_ue_context
                                ))
                        {
                            RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                    (p_ue_context->p_gb_context)->facility_name,RRC_ERROR,
                                    "Returened failure from rrc_copy_transportlayer_address ");
                            /*Logical_Error*/
                            uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                            continue;
                            /*Logical_Error*/
                        }
                        if ( RRC_ONE ==
                                p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                m.uL_ConfigurationPresent )
                        {
                            if ( SN_TERMINATED_SPLIT_BEARER !=
                                    p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type )
                            {
                                RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                        (p_ue_context->p_gb_context)->facility_name,
                                        RRC_DETAILED,
                                        "[SGNB_ADD_REQ_ACK] RAB ID [%u] : UL config is present "
                                        "for SN_TERMINATED_SCG_BEARER %d", 
                                        erb_id_sgnb, 
                                        p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type);
								/*Logical_Error*/
                    			uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                    			continue;
                    			/*Logical_Error*/
                            }
                        }

                        if ( RRC_ONE ==
                                p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                m.mCG_E_RAB_Level_QoS_ParametersPresent )
                        {
                            if ( SN_TERMINATED_SPLIT_BEARER !=
                                    p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type )
                            {
                                RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                        (p_ue_context->p_gb_context)->facility_name,
                                        RRC_DETAILED,
                                        "[SGNB_ADD_REQ_ACK] RAB ID [%u] : MCG erab level QOS parameters "
                                        "is present for SN_TERMINATED_SCG_BEARER %d", 
                                        erb_id_sgnb, 
                                        p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type);
								/*Logical_Error*/
                    			uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                    			continue;
                    			/*Logical_Error*/
                            }
                            else
                            {
                                qci = p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                      mCG_E_RAB_Level_QoS_Parameters.qCI;
                                /*check for the QCI received. If it is not same to the one sent in SgNB Addition req
                                 * then reject the SgNB Addition Ack*/
                                if(qci != p_ue_context->p_e_rab_list[erb_id_sgnb]->e_RABlevelQoSParameters.qCI)
                                {
                                    RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                            (p_ue_context->p_gb_context)->facility_name,
                                            RRC_DETAILED,
                                            "[SGNB_ADD_REQ_ACK] RAB ID [%u] : MCG erab level QOS parameters "
                                            "is received for invalid QCI[%d] compared to one sent in "
                                            "SGNB Add Req[%d]", 
                                            erb_id_sgnb, 
                                            qci, p_ue_context->p_e_rab_list[erb_id_sgnb]->e_RABlevelQoSParameters.qCI);
									/*Logical_Error*/
                    				uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                    				continue;
                    				/*Logical_Error*/
                                }

                                if ( ( RRC_MIN_QCI_NON_GBR_LIMIT <= qci ) &&
                                        ( RRC_MAX_QCI_NON_GBR_LIMIT >= qci ) )
                                {
                                    RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                            (p_ue_context->p_gb_context)->facility_name,
                                            RRC_DETAILED,
                                            "[SGNB_ADD_REQ_ACK] RAB ID [%u] : MCG erab level QOS parameters "
                                            "is received for non GBR bearer %d", 
                                            erb_id_sgnb, 
                                            qci);
									/*Logical_Error*/
                    				uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                    				continue;
                    				/*Logical_Error*/
                                }
                            }
                        }

                        if ( RRC_TRUE ==
                                p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                m.rlc_ModePresent )
                        {
                            p_ue_context->p_e_rab_list[erb_id_sgnb]->m.rlcModePresent = RRC_ONE;

                            p_ue_context->p_e_rab_list[erb_id_sgnb]->rlc_mode_sgnb =
                                p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                rlc_Mode;
                            RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                    (p_ue_context->p_gb_context)->facility_name,
                                    RRC_DETAILED,
                                    "[SGNB_ADD_REQ_ACK]RLC MODE [%u]"
                                    "for SN_TERMINATED_SPLIT_BEARER ", 
                                    p_ue_context->p_e_rab_list[erb_id_sgnb]->rlc_mode_sgnb);
                            if ( RRC_RLC_CONFIG_AM_CONFIG_PRESENT &
                                    p_ue_context->p_e_rab_list[erb_id_sgnb]->drb_config.
                                    rlc_config_enb.rlc_config.bitmask )
                            {
                                if ( RLC_CONFIG_AM_MODE != p_ue_context->p_e_rab_list[erb_id_sgnb]->rlc_mode_sgnb )
                                {
                                    RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                            (p_ue_context->p_gb_context)->facility_name,
                                            RRC_ERROR,
                                            "[SGNB_ADD_REQ_ACK] RAB ID [%u] : RLC MODE mismatch "
                                            "for SN_TERMINATED_SPLIT_BEARER %d", 
                                            erb_id_sgnb, 
                                            p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type);
									/*Logical_Error*/
                    				uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                    				continue;
                    				/*Logical_Error*/
                                }
                            }
                            else
                            {
                                if ( RLC_CONFIG_AM_MODE == p_ue_context->p_e_rab_list[erb_id_sgnb]->rlc_mode_sgnb )
                                {
                                    RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                            (p_ue_context->p_gb_context)->facility_name,
                                            RRC_ERROR,
                                            "[SGNB_ADD_REQ_ACK] RAB ID [%u] : RLC MODE mismatch "
                                            "for SN_TERMINATED_SPLIT_BEARER %d", 
                                            erb_id_sgnb, 
                                            p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type);
									/*Logical_Error*/
                    				uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                    				continue;
                    				/*Logical_Error*/
                                }
                            }
                        }
                        else
                        {
                            if ( SN_TERMINATED_SPLIT_BEARER ==
                                    p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type )
                            {
                                RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                        (p_ue_context->p_gb_context)->facility_name,
                                        RRC_ERROR,
                                        "[SGNB_ADD_REQ_ACK] RAB ID [%u] : RLC MODE is not "
                                        "present for SN_TERMINATED_SPLIT_BEARER %d", 
                                        erb_id_sgnb, 
                                        p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type);
								/*Logical_Error*/
                    			uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                    			continue;
                    			/*Logical_Error*/
                            } 
                        }

                        if ( RRC_ONE ==
                                p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                m.iE_ExtensionsPresent )
                        {
                            for (p_node_pdcp = p_erab_admitted_item->
                                    resource_configuration.u.sgNBPDCPpresent->iE_Extensions.head;
                                    (p_node_pdcp != PNULL); p_node_pdcp = p_node_pdcp->next)
                            {
                                p_pdcp_sn_size_item = 
                                    (x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAck_Item_SgNBPDCPpresent_iE_Extensions_element*) 
                                    p_node_pdcp->data;
                                if ( T129x2ap___x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAck_Item_SgNBPDCPpresentExtIEs_1 ==
                                        p_pdcp_sn_size_item->extensionValue.t )
                                {

                                    if  ( RRC_NULL ==
                                            p_pdcp_sn_size_item->extensionValue.u.
                                            _x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAck_Item_SgNBPDCPpresentExtIEs_1 )
                                    {
                                        p_ue_context->p_e_rab_list[erb_id_sgnb]->m.pdcpSnSizePresent = RRC_ONE;
                                        p_ue_context->p_e_rab_list[erb_id_sgnb]->pdcp_sn_size_sgnb =
                                            12;
                                    }
                                    else if  ( RRC_ONE ==
                                            p_pdcp_sn_size_item->extensionValue.u.
                                            _x2ap_E_RABs_Admitted_ToBeAdded_SgNBAddReqAck_Item_SgNBPDCPpresentExtIEs_1 )
                                    {
                                        p_ue_context->p_e_rab_list[erb_id_sgnb]->m.pdcpSnSizePresent = RRC_ONE;
                                        p_ue_context->p_e_rab_list[erb_id_sgnb]->pdcp_sn_size_sgnb =
                                            18;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if ( SN_TERMINATED_SPLIT_BEARER ==
                                    p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type )
                            {
                                RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                        (p_ue_context->p_gb_context)->facility_name,
                                        RRC_ERROR,
                                        "[SGNB_ADD_REQ_ACK] RAB ID [%u] : PDCP SN size is not "
                                        "present for SN_TERMINATED_SPLIT_BEARER %d", 
                                        erb_id_sgnb, 
                                        p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type);
								/*Logical_Error*/
                    			uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                    			continue;
                    			/*Logical_Error*/
                            } 
                        }

                        /* OPTION3X Changes Start */
                        for(erb_curr_idx = RRC_ZERO;
                                erb_curr_idx < 
                                p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                drb_to_modify_list.drb_count;
                                erb_curr_idx++)
                        {
                            if(p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                    drb_to_modify_list.drb_config[erb_curr_idx].erab_id ==
                                    erb_id_sgnb)
                            {
                                /*update sgnb s1ap information in curr_proc_data_t*/
                                if(UECC_RRM_DC_BEARER_CHANGE_PROC == p_ue_context->p_curr_proc_data->t)
                                {
                                    p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                        drb_to_modify_list.drb_config[erb_curr_idx].sgnb_s1ap_gtp_id =
                                        p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                        s1_DL_GTPtunnelEndpoint.gTP_TEID;

                                    p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                        drb_to_modify_list.drb_config[erb_curr_idx].m.
                                        rrc_x2u_sgnb_s1ap_gtp_id_updated = RRC_TRUE;
                                     /*coverity_254789_fix_start*/
                                     if( RRC_SUCCESS != rrc_copy_transportlayer_address(
                                            &p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                            drb_to_modify_list.drb_config[erb_curr_idx].sgnb_s1ap_transportLayerAddress,
                                            &p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                            s1_DL_GTPtunnelEndpoint.transportLayerAddress,
                                            p_ue_context
                                            ))
                                     {
                                            RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                                          (p_ue_context->p_gb_context)->facility_name,RRC_ERROR,
                                                          "Returened failure from rrc_copy_transportlayer_address ");
											/*Logical_Error*/
                    						uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                    						continue;
                    						/*Logical_Error*/
                                     }

                                    /*coverity_254789_fix_end*/
                                    p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                        drb_to_modify_list.drb_config[erb_curr_idx].m.
                                        rrc_x2u_sgnb_s1ap_transportLayerAddress_updated = RRC_TRUE;

                                    if ( RRC_ONE ==
                                            p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                            m.uL_ConfigurationPresent )
                                    {
                                        RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                                (p_ue_context->p_gb_context)->facility_name,
                                                RRC_DETAILED,
                                                "[SGNB_ADD_REQ_ACK] RAB ID [%u] : UL config is not "
                                                "supported for SN_TERMINATED_SPLIT_BEARER %d", 
                                                erb_id_sgnb, 
                                                p_ue_context->p_e_rab_list[erb_id_sgnb]->dc_bearer_type);
                                    }
                                }


                                if(x2ap_present_1 ==
                                        p_erab_admitted_item->en_DC_ResourceConfiguration.mCGresources)
                                {
                                    if(RRC_ONE ==
                                            p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                            m.sgNB_UL_GTP_TEIDatPDCPPresent
                                            &&
                                            UECC_RRM_DC_BEARER_CHANGE_PROC == p_ue_context->p_curr_proc_data->t)
                                    {
                                        p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                            drb_to_modify_list.drb_config[erb_curr_idx].ul_x2u_teid_peer =
                                            p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                            sgNB_UL_GTP_TEIDatPDCP.gTP_TEID;

                                        p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                            drb_to_modify_list.drb_config[erb_curr_idx].m.
                                            rrc_x2u_ul_teid_peer_updated = RRC_ONE;
                                        /*coverity fix 254789*/
                                        if( RRC_SUCCESS !=  rrc_copy_transportlayer_address(
                                                &p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                                drb_to_modify_list.drb_config[erb_curr_idx].ul_x2u_transport_layer_addr,
                                                &p_erab_admitted_item->resource_configuration.u.sgNBPDCPpresent->\
                                                sgNB_UL_GTP_TEIDatPDCP.transportLayerAddress,
                                                p_ue_context ))
                                        {
                                            RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                                          (p_ue_context->p_gb_context)->facility_name,RRC_ERROR,
                                                          "Returened failure from rrc_copy_transportlayer_address ");
											/*Logical_Error*/
                    						uecc_x2ap_add_erab_to_failed_list(p_ue_context,p_erab_admitted_item->e_RAB_ID);
                    						continue;
                    						/*Logical_Error*/

                                        }
                                        /*coverity fix 254789*/
                                        
                                        p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                                            drb_to_modify_list.drb_config[erb_curr_idx].m.
                                            rrc_x2u_ul_trasport_addr_updated = RRC_ONE;
                                        break;
                                    }
                                }
                            }
                        }
                        /* OPTION3X Changes End */
                    }
                    else
                    {
                        RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                                (p_ue_context->p_gb_context)->facility_name,
                                RRC_ERROR,
                                "[SGNB_ADD_REQ_ACK] RAB ID [%u] : SgNB PCPD Not Present!", 
                                erb_id_sgnb);
                    }
                }
                erab_present_at_menb++;
                if(PNULL != p_erab_data_list)
                {
                    p_erab_data_list->drb_config[index].erab_success = RRC_TRUE;
                }	
                break;
            }
            /*Logical_Error Code_Remove*/
        }
    }
    if (RRC_ZERO == erab_present_at_menb)
    {
            RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                    (p_ue_context->p_gb_context)->facility_name,
                    RRC_DETAILED,
                    "[SGNB_ADD_REQ_ACK] None of the Admitted ERABs at SgNB are present "
                    "SN_TERMINATED_SCG_BEARER present at MeNB ");
            return RRC_FAILURE;
    }
    /* Currently checking that all the RAB that are send as RABsToBeAdded to SgNB
     * must be admitted at SgNB Successfully*/
    if (erab_count !=
            p_ue_context->endc_sgnb_add_erab_list.counter )
    {
        RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                (p_ue_context->p_gb_context)->facility_name,
                RRC_ERROR,
                "[SGNB_ADD_REQ_ACK] Admitted RAB count at SgNB is not same as SN_TERMINATED_SCG_BEARER Count. "
                "Admitted RAB Count:[%d] SN_TERMINATED_SCG_BEARER Count:[%d]", 
                erab_count, p_ue_context->endc_sgnb_add_erab_list.counter);
        /*Logical_Error +-*/
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : uecc_x2ap_read_all_failed_to_admit_erab_list
 * Inputs         : uecc_ue_context_t* -  p_ue_context
 *                  x2ap_E_RAB_List* - Failed eRABs list
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion read failed to admit eRAB list and 
 *                  put in share data
 ****************************************************************************/
void uecc_x2ap_read_all_failed_to_admit_erab_list(
        uecc_ue_context_t*  p_ue_context,    
        x2ap_E_RAB_List*    p_erab_failed_to_admit_list
        )
{
    U8                  fail_list_count  = RRC_NULL;
    OSRTDListNode       *p_node          = PNULL; 
    U8                  erab_count       = RRC_NULL;
    x2ap_E_RAB_Item     *p_erab_item     = PNULL;
    uecc_erab_failed_to_setup_list_t
        *p_failed_erab_list
        = PNULL;
    uecc_rm_erab_setup_info_t*  p_erab_setup_list = PNULL;
    rrc_drb_to_modify_list_t     *p_erab_data_list = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_erab_failed_to_admit_list);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

/* OPTION3X Changes Start */
    if (UECC_RRM_DC_BEARER_CHANGE_PROC ==
            p_ue_context->p_curr_proc_data->t) 
    {
        p_failed_erab_list = 
            &(p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                    failed_erab_data);
        p_erab_data_list =
            &(p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                    drb_to_modify_list);

    }
    else
    {
        p_failed_erab_list = 
            &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                    failed_erab_data);
        p_erab_setup_list =
            &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                    erab_info_data);

    }
/* OPTION3X Changes End */

    fail_list_count = p_failed_erab_list->count;

    /* Append Failed E_RAB ID's recv from SgNB to Failed List */
    for (p_node = 
            p_erab_failed_to_admit_list->head;
            ((p_node != PNULL) && (erab_count < MAX_ERAB_COUNT));
            p_node = p_node->next, erab_count++)
    {
        p_erab_item = 
            (x2ap_E_RAB_Item *)
            ((x2ap_E_RAB_List_element*)(p_node->data))->value.u._x2ap_E_RAB_ItemIEs_1;

        /* Put Fail Erab ID recv from SgNB in Fail List */
        p_failed_erab_list->erab_failed_to_setup_item[fail_list_count].
            e_RAB_ID = (U8)p_erab_item->e_RAB_ID;

        /* MAP CAUSE VALUE HERE AND STORE IN FAILED LIST */
        map_x2ap_cause_to_s1ap_cause(
                p_erab_item->cause,
                &p_failed_erab_list->erab_failed_to_setup_item[fail_list_count].
                cause
                );
        /*Logical_Error*/
        U8 erab_index = 0;
        if(PNULL != p_erab_setup_list)
        {
    		for(erab_index = RRC_NULL; erab_index < MAX_ERAB_COUNT; erab_index++)
            {
                if ( p_erab_setup_list->erab_info[erab_index].erab_id ==
                        p_erab_item->e_RAB_ID)
                {
                    p_erab_setup_list->erab_info[erab_index].status = RRC_INTERNAL_ERROR;
                    p_erab_setup_list->outerResponse = PARTIAL_SUCCESS;
                }
            }
        }
        else if(PNULL != p_erab_data_list)
        {
    		for(erab_index = RRC_NULL; erab_index < MAX_ERAB_COUNT; erab_index++)
    		{
        		if(p_erab_data_list->drb_config[erab_index].erab_id ==
        				p_erab_item->e_RAB_ID)
        		{
        			p_erab_data_list->drb_config[erab_index].erab_failed = RRC_TRUE;
        			p_failed_erab_list->erab_failed_to_setup_item[fail_list_count].new_lcId = p_erab_data_list->drb_config[erab_index].new_lc_id; 
        		}
        	}
        }
        /*Logical_Error*/

        /* Increment Fail List Count */
        fail_list_count++;

/* OPTION3X Changes Start */
        if (UECC_RRM_DC_BEARER_CHANGE_PROC !=
                p_ue_context->p_curr_proc_data->t)
        {
            /* Deallocate Failed E_RAB From UE Context */
            uecc_fsm_rm_erab_setup_deallocate_erab(
                    p_ue_context, 
                    (U8)p_erab_item->e_RAB_ID);
        }
/* OPTION3X Changes End */
    }

    /* Update Fail List Counter */
    p_failed_erab_list->count = fail_list_count;


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*Logical_Error*/
/****************************************************************************
 * Function Name  : uecc_x2ap_add_erab_to_failed_list
 * Inputs         : uecc_ue_context_t* -  p_ue_context
 *                  x2ap_E_RAB_List* - Failed eRABs list
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion add erab failed to not admit eRAB list and 
 *                  put in share data
 ****************************************************************************/
void uecc_x2ap_add_erab_to_failed_list(
        uecc_ue_context_t*  p_ue_context,    
        U8 erab_id
        )
{
    U8                  fail_list_count  = RRC_NULL;
    uecc_erab_failed_to_setup_list_t
        *p_failed_erab_list = PNULL;
    uecc_rm_erab_setup_info_t*  p_erab_setup_list = PNULL;
    rrc_drb_to_modify_list_t     *p_erab_data_list = PNULL;

    x2ap_Cause cause;
    cause.t = T_x2ap_Cause_protocol;
    cause.u.protocol = x2ap_semantic_error; 


    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    if (UECC_RRM_DC_BEARER_CHANGE_PROC ==
            p_ue_context->p_curr_proc_data->t) 
    {
        p_failed_erab_list = 
            &(p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                    failed_erab_data);
      p_erab_data_list =
            &(p_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.
                    drb_to_modify_list);

    }
    else
    {
        p_failed_erab_list = 
            &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                    failed_erab_data);
        p_erab_setup_list =
            &(p_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                    erab_info_data);
    }

    fail_list_count = p_failed_erab_list->count;

    /* Put Fail Erab ID recv from SgNB in Fail List */
    p_failed_erab_list->erab_failed_to_setup_item[fail_list_count].
        e_RAB_ID = (U8)erab_id;

    /* MAP CAUSE VALUE HERE AND STORE IN FAILED LIST */
    map_x2ap_cause_to_s1ap_cause(
            cause,
            &p_failed_erab_list->erab_failed_to_setup_item[fail_list_count].
            cause
            );
	   U8 erab_index = 0;
        if(PNULL != p_erab_setup_list)
        {
    		for(erab_index = RRC_NULL; erab_index < MAX_ERAB_COUNT; erab_index++)
            {
                if ( p_erab_setup_list->erab_info[erab_index].erab_id ==
                        erab_id)
                {
                    p_erab_setup_list->erab_info[erab_index].status = RRC_INTERNAL_ERROR;
                    p_erab_setup_list->outerResponse = PARTIAL_SUCCESS;
                }
            }
        }
        else if(PNULL != p_erab_data_list)
        {
    		for(erab_index = RRC_NULL; erab_index < MAX_ERAB_COUNT; erab_index++)
    		{
        		if(p_erab_data_list->drb_config[erab_index].erab_id ==
        				erab_id)
        		{
        			p_erab_data_list->drb_config[erab_index].erab_failed = RRC_TRUE;
        			p_failed_erab_list->erab_failed_to_setup_item[fail_list_count].new_lcId = p_erab_data_list->drb_config[erab_index].new_lc_id; 
        		}
        	}
        }

    /* Increment Fail List Count */
    fail_list_count++;

    if (UECC_RRM_DC_BEARER_CHANGE_PROC !=
            p_ue_context->p_curr_proc_data->t)
    {
        /* Deallocate Failed E_RAB From UE Context */
        uecc_fsm_rm_erab_setup_deallocate_erab(
                p_ue_context, 
                (U8)erab_id);
    }

    /* Update Fail List Counter */
    p_failed_erab_list->count = fail_list_count;
   RRC_UECC_TRACE((p_ue_context->p_gb_context)->context_index,
                        (p_ue_context->p_gb_context)->facility_name,
                        RRC_WARNING,
                        "[SGNB_ADD_REQ_ACK] Add Erab to Failed List  ERAB[%d]",erab_id);


    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
}
/*Logical_Error*/

/****************************************************************************
 * Function Name  : uecc_x2ap_handle_sgnb_addition_ack_msg_rcvd
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : rrc_return_et
 * Description    : This is the function for Stroing SgNBAdditionRequestAck
 *                  message in uecc context.
 ****************************************************************************/
rrc_return_et uecc_x2ap_handle_sgnb_addition_ack_msg_rcvd(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    rrc_return_et       result                              = RRC_SUCCESS;
    rrc_x2ap_sgnb_addition_req_ack_t
        *p_rrc_x2ap_sgnb_addition_req_ack   = PNULL;


    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    do
    {
        /* storing the SgNBAddRequestAck received */
        p_rrc_x2ap_sgnb_addition_req_ack = (rrc_x2ap_sgnb_addition_req_ack_t*)p_api;      

        if (p_ue_context->x2ap_context.menb_ue_x2ap_id == 
                p_rrc_x2ap_sgnb_addition_req_ack->menb_ue_x2ap_id)
        {
            /* Read Successful Admitted eRABs */
            if (RRC_SUCCESS == uecc_x2ap_read_all_success_admitted_erab_list(
                        p_ue_context,    
                        &p_rrc_x2ap_sgnb_addition_req_ack->erabs_admittied_list
                        ))
            {
                if ((X2AP_SGNB_ADD_REQ_ACK_ERABS_NOT_ADMITTED_LIST_PRESENT & 
                            p_rrc_x2ap_sgnb_addition_req_ack->bitmask))
                {
                    uecc_x2ap_read_all_failed_to_admit_erab_list(
                            p_ue_context, 
                            &p_rrc_x2ap_sgnb_addition_req_ack->erabs_not_admitted_list);
                }
                result = RRC_SUCCESS;
            }
            else
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        " SgNB Addition Request Ack "
                        "Error in processing admitted eRAB list!");
                result = RRC_FAILURE;
                break;
            }

            /*process SgNB Ue X2AP ID*/
            p_ue_context->x2ap_context.sgnb_ue_x2ap_id =
                p_rrc_x2ap_sgnb_addition_req_ack->sgnb_ue_x2ap_id;

            p_ue_context->x2ap_context.m.sgnb_ue_x2ap_id_present = RRC_TRUE;

            /*process SgNB to MeNB container*/
            if(RRC_FAILURE == uecc_x2ap_sgnb_add_req_ack_process_cg_config(
                        p_ue_context,
                        &p_rrc_x2ap_sgnb_addition_req_ack->sgnb_to_menb_container))
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        " SgNB Addition Request Ack "
                        "Error in processing sgnb_to_menb_container!");
                result = RRC_FAILURE;
                break;
            }

            /*CRITICALITY_DIAGNOSTICS PRESENT*/
            if (X2AP_SGNB_ADD_REQ_ACK_CRITICALITY_DIAGNOSTICS_PRESENT & 
                    p_rrc_x2ap_sgnb_addition_req_ack->bitmask)
            {
            }

            /*ADMITTED_SPLIT_SRBS PRESENT*/
            if (X2AP_SGNB_ADD_REQ_ACK_ADMITTED_SPLIT_SRBS_PRESENT & 
                    p_rrc_x2ap_sgnb_addition_req_ack->bitmask)
            {
            }

            /*SGNB_RESOURCE_COORDINATION_INFO PRESENT*/
            if (X2AP_SGNB_ADD_REQ_ACK_SGNB_RESOURCE_COORDINATION_INFO_PRESENT & 
                    p_rrc_x2ap_sgnb_addition_req_ack->bitmask)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                        " SgNB Addition Request Ack "
                        "SgNB resource coordiantion info is not supported!");
            }

            /*RRC_CONFIG_INDICATION PRESENT*/
            if (X2AP_SGNB_ADD_REQ_ACK_RRC_CONFIG_INDICATION_PRESENT & 
                    p_rrc_x2ap_sgnb_addition_req_ack->bitmask)
            {
            }
        }
        else
        {
        	RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                    " SgNB Addition Request Ack "
                    "MeNB_UE_X2AP_ID Mismatch");

        	return RRC_FAILURE;
        }

    } while(RRC_NULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_sgnb_reconfig_comp_resp_info 
 *
 *   DESCRIPTION:
 *       This function builds Response Information SgNBReconfComp, send to SgNB.
 *
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_build_sgnb_reconfig_comp_resp_info(
        uecc_ue_context_t               *p_ue_context,
        x2ap_ResponseInformationSgNBReconfComp
        *p_resp_info,
        OSCTXT                          *pctxt)
{
    rrc_return_et               result  = RRC_FAILURE;
    x2ap_MeNBtoSgNBContainer    *p_menb_to_sgnb_cont = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_resp_info);
    RRC_ASSERT(PNULL != pctxt);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    do
    {
        /* Fill x2ap_ResponseInformationSgNBReconfComp */
        p_resp_info->t = 
            T_x2ap_ResponseInformationSgNBReconfComp_success_SgNBReconfComp;

        p_resp_info->u.success_SgNBReconfComp = 
            (x2ap_ResponseInformationSgNBReconfComp_SuccessItem *)
            rtxMemAllocType(pctxt, x2ap_ResponseInformationSgNBReconfComp_SuccessItem);

        if (PNULL == p_resp_info->u.success_SgNBReconfComp)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_FATAL,
                    "[Response Information SgNBReconfigurationComplete] "
                    "ASN malloc failed.");
            break;

        }

        asn1Init_x2ap_ResponseInformationSgNBReconfComp_SuccessItem(
                p_resp_info->u.success_SgNBReconfComp);

        p_resp_info->u.success_SgNBReconfComp->m.iE_ExtensionsPresent = RRC_NULL;

        /*
         * If scg-ConfigResponseNR-r15 was present in RRCConnectionReconfigurationComplete
         * then copy the scg-ConfigResponseNR-r15 content in the menb_to_sgnb_container
         * of the SgNBReconfigurationComplete Message.
         */
        if (p_ue_context->m.scg_config_response_nr_present)
        {
            p_menb_to_sgnb_cont = (x2ap_MeNBtoSgNBContainer *)
                (&p_resp_info->u.success_SgNBReconfComp->meNBtoSgNBContainer);

            /*Fill octet string length*/
            p_menb_to_sgnb_cont->numocts = 
                p_ue_context->scg_config_response_nr.numocts;

            p_menb_to_sgnb_cont->data = (U8 *)rtxMemAlloc(
                    pctxt, 
                    p_menb_to_sgnb_cont->numocts);
            if (PNULL == p_menb_to_sgnb_cont->data)
            {
                /* ASN context memory allocation failed */
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, 
                        RRC_ERROR, "[x2ap_SgNBReconfigurationComplete] "
                        "ASN Memory allocation failed. "
                        "MeNB UE X2AP ID [%u] "
                        "UE Index: [%u] "
                        "Cell Index: [%u]", 
                        p_ue_context->x2ap_context.menb_ue_x2ap_id,
                        p_ue_context->ue_index,
                        p_ue_context->cell_index);

                break;
            }

            l3_memcpy_wrapper((void*)p_menb_to_sgnb_cont->data,
                    p_ue_context->scg_config_response_nr.data,
                    p_menb_to_sgnb_cont->numocts);

            /* Set IE Identifier */
            p_resp_info->u.success_SgNBReconfComp->m.meNBtoSgNBContainerPresent = RRC_ONE;
        }

        result = RRC_SUCCESS;

    }while(RRC_NULL);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_sgnb_reconfig_comp_resp_info_failure 
 *
 *   DESCRIPTION:
 *       This function builds Response Information SgNBReconfComp with failure
 *       cause.
 *
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
rrc_return_et uecc_x2ap_build_sgnb_reconfig_comp_resp_info_failure(
        uecc_ue_context_t               *p_ue_context,
        x2ap_ResponseInformationSgNBReconfComp
        *p_resp_info,
        OSCTXT                          *pctxt)
{
    rrc_return_et               result  = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_resp_info);
    RRC_ASSERT(PNULL != pctxt);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    do
    {
        /* Fill x2ap_ResponseInformationSgNBReconfComp */
        p_resp_info->t = 
            T_x2ap_ResponseInformationSgNBReconfComp_reject_by_MeNB_SgNBReconfComp;

        p_resp_info->u.reject_by_MeNB_SgNBReconfComp = 
            (x2ap_ResponseInformationSgNBReconfComp_RejectByMeNBItem *)
            rtxMemAllocType(pctxt, x2ap_ResponseInformationSgNBReconfComp_RejectByMeNBItem);

        if (PNULL == p_resp_info->u.reject_by_MeNB_SgNBReconfComp)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_FATAL,
                    "[Response Information SgNBReconfigurationComplete reject_by_MeNB_SgNBReconfComp] "
                    "ASN malloc failed.");
            break;
        }

        asn1Init_x2ap_ResponseInformationSgNBReconfComp_RejectByMeNBItem(
                p_resp_info->u.reject_by_MeNB_SgNBReconfComp);

        p_resp_info->u.reject_by_MeNB_SgNBReconfComp->m.iE_ExtensionsPresent = RRC_NULL;

        /* Fill the fail cause for SgNB Reconfiguration Complete failure */
        asn1Copy_x2ap_Cause( pctxt,
                (const x2ap_Cause*)&p_ue_context->sgnb_cause,
                &p_resp_info->u.reject_by_MeNB_SgNBReconfComp->cause
                );
        /*reset sgbnCausePresent flag in UECC CTX after use*/
        p_ue_context->m.sgnbCausePresent = RRC_ZERO;

        result = RRC_SUCCESS;

    }while(RRC_NULL);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return result;
}
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_sgnb_reconfiguration_complete
 *
 *   DESCRIPTION:
 *       This function builds and sends SgNB Reconfiguration Conplete message
 *       to X2AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
    rrc_return_et 
uecc_x2ap_build_and_send_sgnb_reconfiguration_complete(
        uecc_ue_context_t           *p_ue_context
        )
{
    //x2ap_UE_X2AP_ID                     menb_ue_x2ap_id     = RRC_NULL;
    //x2ap_UE_X2AP_ID_Extension           menb_ue_x2ap_id_ext = RRC_INVALID_DC_UE_X2AP_ID;
    rrc_return_et                       result              = RRC_FAILURE;
    OSCTXT                              asn1_ctx;
    rrc_x2ap_sgnb_reconfig_complete_t   msg;

    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;

    RRC_ASSERT(PNULL != p_ue_context);

    /* Check if Global UE context is NULL. */
    if(PNULL == p_ue_context->p_gb_context)
    {
        /* uecc_global context is null */
        return RRC_FAILURE;
    }
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        /* ASN context initialization failed */
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, 
                RRC_ERROR, "[x2ap_SgNBReconfigurationComplete] "
                "ASN context initialization failed "
                "MeNB UE X2AP ID [%u] "
                "UE Index: [%u] "
                "Cell Index: [%u]", 
                p_ue_context->x2ap_context.menb_ue_x2ap_id,
                p_ue_context->ue_index,
                p_ue_context->cell_index);

        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    do
    {
        /* Reset message */
        memset_wrapper(&msg, RRC_NULL,
                sizeof(rrc_x2ap_sgnb_reconfig_complete_t));

        /* Memset as all fields will not be used. */
        memset_wrapper(sig_msg_req_buf, RRC_NULL,
                sizeof(x2ap_ue_associated_sig_msg_req_t) + 
                X2AP_MAX_ASN1_BUF_LEN);

#if 0
        /* Extract X2AP IDs */
        rrc_extract_nr_dc_x2_ids(p_ue_context,
                &menb_ue_x2ap_id,
                &menb_ue_x2ap_id_ext);
#endif

        /* Fill MeNB UE X2AP ID */
        msg.menb_ue_x2ap_id = p_ue_context->x2ap_context.menb_ue_x2ap_id;
#if 0
        /* Fill MeNB UE X2AP ID Extension */
        if(p_ue_context->x2ap_context.is_menb_ext_present)
        {
            msg.menb_ue_x2ap_id_ext = menb_ue_x2ap_id_ext;
            msg.bitmask |= 
                X2AP_SGNB_RECONFIG_COMPLETE_MENB_UE_X2AP_ID_EXT_PRESENT;
        }
#endif
        /* Fill SgNB UE X2AP ID */
        msg.sgnb_ue_x2ap_id = 
            p_ue_context->x2ap_context.sgnb_ue_x2ap_id;

        /* Fill Response Information */
        if (RRC_SUCCESS != uecc_x2ap_build_sgnb_reconfig_comp_resp_info(
                    p_ue_context,
                    &msg.resp_info_sgnb_reconfig_comp,
                    &asn1_ctx))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_SeNBReconfigurationComplete]"
                    "uecc_x2ap_build_sgnb_reconfig_comp_resp_info "
                    "failure");
            break;
        }

        /*Encode message*/
        result = x2ap_encode_sgNB_reconfig_complete(
                &asn1_ctx,
                &p_sig_msg_req->message_buff_p[RRC_NULL],
                &p_sig_msg_req->message_buff_size,
                &msg);
        if(RRC_SUCCESS != result)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_SgNBReconfigurationComplete] Build failure "
                    "MeNB UE X2AP ID [%u] "
                    "Peer gNB Id: [%u] "
                    "UE Index: [%u] "
                    "Cell Index: [%u]", 
                    msg.menb_ue_x2ap_id,
                    p_ue_context->x2ap_context.
                    peer_enodeb_id,
                    p_ue_context->ue_index,
                    p_ue_context->cell_index);

            break;
        }

        /* Fill Procedure Code. */
        p_sig_msg_req->procedure_code = 
            ASN1V_x2ap_id_sgNBReconfigurationCompletion;

        /* Fill Message Type. */
        p_sig_msg_req->message_type = X2AP_INITIATING_MESSAGE;

        /* Fill Cell Index. */
        p_sig_msg_req->cell_index = p_ue_context->cell_index;

        /* Fill UE ID.*/
        p_sig_msg_req->ue_id = p_ue_context->ue_index;

        /* Fill MeNB UE X2AP ID. */
        p_sig_msg_req->bitmask |=
            X2AP_UE_ASSOC_SIG_MSG_REQ_MENB_UE_X2AP_ID_PRESENT;
        p_sig_msg_req->menb_ue_x2ap_id = 
            p_ue_context->x2ap_context.menb_ue_x2ap_id;

        /* Fill Peer gNB ID. */
        p_sig_msg_req->peer_enodeb_id = 
            p_ue_context->x2ap_context.peer_enodeb_id;

        /* Send message to X2AP  */
        result = uecc_rrc_send_internal_msg(
                RRC_X2AP_MODULE_ID,
                X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                    p_sig_msg_req->message_buff_size),
                sig_msg_req_buf);

        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, 
                RRC_INFO, "X2AP_MSG: SGNB RECONFIGURATION COMPLETE "
                "MeNB UE X2AP ID [%u] "
                "Peer gNB Id: [%u] "
                "UE Index: [%u] Cell Index: [%u] %s", 
                msg.menb_ue_x2ap_id,
                p_ue_context->x2ap_context.
                peer_enodeb_id,
                p_ue_context->ue_index,
                p_ue_context->cell_index,
                change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
    }while(RRC_NULL);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure
 *
 *   DESCRIPTION:
 *       This function builds and sends SgNB Reconfiguration Conplete message
 *       with failure to X2AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
rrc_return_et 
uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
    uecc_ue_context_t           *p_ue_context
)
{
//    x2ap_UE_X2AP_ID                     menb_ue_x2ap_id     = RRC_NULL;
//    x2ap_UE_X2AP_ID_Extension           menb_ue_x2ap_id_ext = RRC_INVALID_DC_UE_X2AP_ID;
    rrc_return_et                       result              = RRC_FAILURE;
    OSCTXT                              asn1_ctx;
    rrc_x2ap_sgnb_reconfig_complete_t   msg;

    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN];
    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;

    RRC_ASSERT(PNULL != p_ue_context);

    /* Check if Global UE context is NULL. */
    if(PNULL == p_ue_context->p_gb_context)
    {
        /* uecc_global context is null */
        return RRC_FAILURE;
    }
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        /* ASN context initialization failed */
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, 
                RRC_ERROR, "[x2ap_SgNBReconfigurationComplete] "
                "ASN context initialization failed "
                "MeNB UE X2AP ID [%u] "
                "UE Index: [%u] "
                "Cell Index: [%u]", 
                p_ue_context->x2ap_context.menb_ue_x2ap_id,
                p_ue_context->ue_index,
                p_ue_context->cell_index);

        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    do
    {
        /* Reset message */
        memset_wrapper(&msg, RRC_NULL,
                sizeof(rrc_x2ap_sgnb_reconfig_complete_t));

        /* Memset as all fields will not be used. */
        memset_wrapper(sig_msg_req_buf, RRC_NULL,
                sizeof(x2ap_ue_associated_sig_msg_req_t) + 
                X2AP_MAX_ASN1_BUF_LEN);

#if 0
        /* Extract X2AP IDs */
        rrc_extract_nr_dc_x2_ids(p_ue_context,
                &menb_ue_x2ap_id,
                &menb_ue_x2ap_id_ext);
#endif
        /* Fill MeNB UE X2AP ID */
        msg.menb_ue_x2ap_id = p_ue_context->x2ap_context.menb_ue_x2ap_id;
#if 0
        /* Fill MeNB UE X2AP ID Extension */
        if(p_ue_context->x2ap_context.is_menb_ext_present)
        {
            msg.menb_ue_x2ap_id_ext = menb_ue_x2ap_id_ext;
            msg.bitmask |= 
                X2AP_SGNB_RECONFIG_COMPLETE_MENB_UE_X2AP_ID_EXT_PRESENT;
        }
#endif
        /* Fill SgNB UE X2AP ID */
        msg.sgnb_ue_x2ap_id = 
            p_ue_context->x2ap_context.sgnb_ue_x2ap_id;

        /* Fill Response Information */
        if (RRC_SUCCESS != uecc_x2ap_build_sgnb_reconfig_comp_resp_info_failure(
                    p_ue_context,
                    &msg.resp_info_sgnb_reconfig_comp,
                    &asn1_ctx))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_SeNBReconfigurationComplete]"
                    "uecc_x2ap_build_sgnb_reconfig_comp_resp_info_failure "
                    "failure");
            break;
        }

        /*Encode message*/
        result = x2ap_encode_sgNB_reconfig_complete(
                &asn1_ctx,
                &p_sig_msg_req->message_buff_p[RRC_NULL],
                &p_sig_msg_req->message_buff_size,
                &msg);
        if(RRC_SUCCESS != result)
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_SgNBReconfigurationComplete] Build failure "
                    "MeNB UE X2AP ID [%u] "
                    "Peer gNB Id: [%u] "
                    "UE Index: [%u] "
                    "Cell Index: [%u]", 
                    msg.menb_ue_x2ap_id,
                    p_ue_context->x2ap_context.
                    peer_enodeb_id,
                    p_ue_context->ue_index,
                    p_ue_context->cell_index);

            break;
        }

        /* Fill Procedure Code. */
        p_sig_msg_req->procedure_code = 
            ASN1V_x2ap_id_sgNBReconfigurationCompletion;

        /* Fill Message Type. */
        p_sig_msg_req->message_type = X2AP_INITIATING_MESSAGE;

        /* Fill Cell Index. */
        p_sig_msg_req->cell_index = p_ue_context->cell_index;

        /* Fill UE ID.*/
        p_sig_msg_req->ue_id = p_ue_context->ue_index;

        /* Fill MeNB UE X2AP ID. */
        p_sig_msg_req->bitmask |=
            X2AP_UE_ASSOC_SIG_MSG_REQ_MENB_UE_X2AP_ID_PRESENT;
        p_sig_msg_req->menb_ue_x2ap_id = 
            p_ue_context->x2ap_context.menb_ue_x2ap_id;

        /* Fill Peer gNB ID. */
        p_sig_msg_req->peer_enodeb_id = 
            p_ue_context->x2ap_context.peer_enodeb_id;

        /* Send message to X2AP  */
        result = uecc_rrc_send_internal_msg(
                RRC_X2AP_MODULE_ID,
                X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                    p_sig_msg_req->message_buff_size),
                sig_msg_req_buf);

        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, 
                RRC_INFO, "X2AP_MSG: SGNB RECONFIGURATION COMPLETE "
                "MeNB UE X2AP ID [%u] "
                "Peer gNB Id: [%u] "
                "UE Index: [%u] Cell Index: [%u] %s", 
                msg.menb_ue_x2ap_id,
                p_ue_context->x2ap_context.
                peer_enodeb_id,
                p_ue_context->ue_index,
                p_ue_context->cell_index,
                change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));
    }while(RRC_NULL);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return result;
}
/*NR_DC Code Change Stop*/
/*EN-DC_changes_start*/
/*****************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_x2ap_sgnb_e_rab_release_list
 *
 *   DESCRIPTION:
 *       This function builds x2ap_E_RABs_ToBeReleased_SgNBRelReqList 
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ****************************************************************************/
static rrc_return_et uecc_x2ap_build_x2ap_sgnb_e_rab_release_list(
        uecc_ue_context_t                       *p_ue_context,
        OSCTXT                                  *p_asn1_ctx,
        x2ap_E_RABs_ToBeReleased_SgNBRelReqList *p_e_rab_to_release_list)
{
    x2ap_E_RABs_ToBeReleased_SgNBRelReqList_element *p_elem                 = PNULL;
    x2ap_E_RABs_ToBeReleased_SgNBRelReq_Item        *p_elem_item            = PNULL;
    OSRTDListNode                                   *p_node                 = PNULL;
    U8                                              erab_index              = RRC_NULL;
    U8                                              erab_max_count          = RRC_NULL;
    U8                                              erab_release_id         = RRC_NULL;
    rrc_return_et                                   ret_val = RRC_FAILURE;
    U8                                              erabs_to_release[MAX_ERAB_COUNT];

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_e_rab_to_release_list);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);
    memset_wrapper(&erabs_to_release, 0, sizeof(U8)*MAX_ERAB_COUNT);
    asn1Init_x2ap_E_RABs_ToBeReleased_SgNBRelReqList(p_e_rab_to_release_list);


    if ( RRC_TRUE == p_ue_context->partial_sgnb_rab_release)
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, 
                RRC_DETAILED,"Few SGNB rabs are released \n");
        erab_max_count= MAX_ERAB_COUNT;

        for (erab_index = RRC_NULL; erab_index < erab_max_count;erab_index++)
        {
            erabs_to_release[erab_index]= erab_index;
        }
    }
    else
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name, 
                RRC_DETAILED,"ALL Sgnb rabs are released \n");

        if ( PNULL != p_ue_context->p_curr_proc_data )
        {
            if ( p_ue_context->p_curr_proc_data->u.erab_release_command_data.
                erab_release_list.counter )
            {
                erab_max_count= p_ue_context->p_curr_proc_data->u.
                    erab_release_command_data.erab_release_list.counter;

                for (erab_index = RRC_NULL; erab_index < erab_max_count;erab_index++)
                {
                    erabs_to_release[erab_index]= p_ue_context->
                        p_curr_proc_data->u.erab_release_command_data. \
                            erab_release_list.erab_item[erab_index].e_RAB_ID;
                }
            }
        }
    }

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name, 
            RRC_DETAILED,"Fill rab list in sgnb release request\n");
    for (erab_index = RRC_NULL; erab_index < erab_max_count; erab_index++)
    {
        erab_release_id = erabs_to_release[erab_index];

        /*check the bearer type of the erab to be deleted*/
        if(PNULL != p_ue_context->p_e_rab_list[erab_release_id])
        {
            if((p_ue_context->p_e_rab_list[erab_release_id]->m.DC_BearerTypePresent) &&
                    (
                     SN_TERMINATED_SCG_BEARER ==
                     p_ue_context->p_e_rab_list[erab_release_id]->dc_bearer_type
                     ||
                     SN_TERMINATED_SPLIT_BEARER ==
                     p_ue_context->p_e_rab_list[erab_release_id]->dc_bearer_type
                    )
              )
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                        "erab_release_id %d\n",erab_release_id);

                rtxDListAllocNodeAndData(p_asn1_ctx,
                        x2ap_E_RABs_ToBeReleased_SgNBRelReqList_element,
                        &p_node,
                        &p_elem);

                if (PNULL == p_node)
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, 
                            RRC_DETAILED,"Can't allocate p_node "
                            "MeNB UE X2AP ID [%u] "
                            "Peer eNodeB Id: [%u] "
                            "UE Index: [%u] "
                            "Cell Index: [%u]",
                            p_ue_context->x2ap_context.menb_ue_x2ap_id,
                            p_ue_context->x2ap_context.peer_enodeb_id,
                            p_ue_context->ue_index,
                            p_ue_context->cell_index);

                    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }

                asn1Init_x2ap_E_RABs_ToBeReleased_SgNBRelReqList_element(p_elem);

                p_elem->id = ASN1V_x2ap_id_E_RABs_ToBeReleased_SgNBRelReq_Item;
                p_elem->criticality = x2ap_reject;

                p_elem->value.t = 
                    T177x2ap___x2ap_E_RABs_ToBeReleased_SgNBRelReq_ItemIEs_1;

                /* Allocate Memory */
                p_elem_item = 
                    rtxMemAllocType(p_asn1_ctx,
                            x2ap_E_RABs_ToBeReleased_SgNBRelReq_Item);
                if (PNULL == p_elem_item)
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, 
                            RRC_FATAL,"Can't allocate p_elem_item "
                            "MeNB UE X2AP ID [%u] "
                            "Peer eNodeB Id: [%u] "
                            "UE Index: [%u] "
                            "Cell Index: [%u]",
                            p_ue_context->x2ap_context.menb_ue_x2ap_id,
                            p_ue_context->x2ap_context.peer_enodeb_id,
                            p_ue_context->ue_index,
                            p_ue_context->cell_index);

                    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }
                p_elem->value.u._x2ap_E_RABs_ToBeReleased_SgNBRelReq_ItemIEs_1=
                    p_elem_item ;

                asn1Init_x2ap_E_RABs_ToBeReleased_SgNBRelReq_Item(p_elem_item);

                p_elem_item->m.iE_ExtensionsPresent=0;
                /* Fill E-RAB ID */
                p_elem_item->e_RAB_ID = erab_release_id; 

                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                        "[x2ap_SgNBRelease Request] ERAB ID [%d] ",
                        p_elem_item->e_RAB_ID);

                /*Update ENDC resource Config*/
                p_elem_item->en_DC_ResourceConfiguration.pDCPatSgNB = x2ap_present;
                p_elem_item->en_DC_ResourceConfiguration.mCGresources= x2ap_present_2; 
                p_elem_item->en_DC_ResourceConfiguration.sCGresources= x2ap_not_present_1; 

                /*update Choice resource Configuration*/
                /*check if pDCPatSgNB in en_DC_ResourceConfiguration ie present*/
                if(x2ap_present == p_elem_item->en_DC_ResourceConfiguration.pDCPatSgNB)
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                            "Fill pDCPatSgNB");
                    p_elem_item->resource_configuration.t = 
                        T_x2ap_E_RABs_ToBeReleased_SgNBRelReq_Item_resource_configuration_sgNBPDCPpresent;
                    /* Allocate Memory */
                    p_elem_item->resource_configuration.u.sgNBPDCPpresent = 
                        rtxMemAllocType(p_asn1_ctx,
                                x2ap_E_RABs_ToBeReleased_SgNBRelReq_Item_SgNBPDCPpresent);
                    if (PNULL ==
                            p_elem_item->resource_configuration.u.sgNBPDCPpresent)
                    {
                        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                                p_ue_context->p_gb_context->facility_name, 
                                RRC_DETAILED,"Can't allocate p_elem_item->resource_configuration.u.sgNBPDCPpresent "
                                "MeNB UE X2AP ID [%u] "
                                "Peer eNodeB Id: [%u] "
                                "UE Index: [%u] "
                                "Cell Index: [%u]",
                                p_ue_context->x2ap_context.menb_ue_x2ap_id,
                                p_ue_context->x2ap_context.peer_enodeb_id,
                                p_ue_context->ue_index,
                                p_ue_context->cell_index);

                        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                        return RRC_FAILURE;
                    }

                    asn1Init_x2ap_E_RABs_ToBeReleased_SgNBRelReq_Item_SgNBPDCPpresent(
                            p_elem_item->resource_configuration.u.sgNBPDCPpresent);

                    p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                        m.iE_ExtensionsPresent= 0;
                    p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                        m.uL_GTPtunnelEndpointPresent= 1;
                    p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                        m.dL_GTPtunnelEndpointPresent= 1;

                    p_elem_item->resource_configuration.u.sgNBPDCPpresent->uL_GTPtunnelEndpoint.m.iE_ExtensionsPresent=0;
                    p_elem_item->resource_configuration.u.sgNBPDCPpresent->dL_GTPtunnelEndpoint.m.iE_ExtensionsPresent=0;
                    /*update uL_GTPtunnelEndpoint IE*/
                    asn1Copy_x2ap_TransportLayerAddress (p_asn1_ctx,
                            (x2ap_TransportLayerAddress *)&p_ue_context->
                            p_e_rab_list[erab_release_id]->transportLayerAddress,
                            &p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                            uL_GTPtunnelEndpoint.transportLayerAddress);

                    p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                        uL_GTPtunnelEndpoint.gTP_TEID.numocts = 
                        p_ue_context->p_e_rab_list[erab_release_id]->gTP_TEID.numocts;

                    l3_memcpy_wrapper (p_elem_item->resource_configuration.u.sgNBPDCPpresent->
                            uL_GTPtunnelEndpoint.gTP_TEID.data,
                            p_ue_context->p_e_rab_list[erab_release_id]->gTP_TEID.data,
                            p_ue_context->p_e_rab_list[erab_release_id]->gTP_TEID.numocts);

                    /*update dL_GTPtunnelEndpoint IE*/
                    asn1Copy_x2ap_TransportLayerAddress (p_asn1_ctx,
                            (x2ap_TransportLayerAddress *)&p_ue_context->p_e_rab_list[erab_release_id]->\
                            sgnb_s1ap_transportLayerAddress,
                            &p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                            dL_GTPtunnelEndpoint.transportLayerAddress);

                    p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                        dL_GTPtunnelEndpoint.gTP_TEID.numocts = 
                        p_ue_context->p_e_rab_list[erab_release_id]->sgnb_s1ap_gtp_id.numocts;

                    l3_memcpy_wrapper (p_elem_item->resource_configuration.u.sgNBPDCPpresent->
                            dL_GTPtunnelEndpoint.gTP_TEID.data,
                            p_ue_context->p_e_rab_list[erab_release_id]->sgnb_s1ap_gtp_id.data,
                            p_ue_context->p_e_rab_list[erab_release_id]->sgnb_s1ap_gtp_id.numocts);

                }
                else
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                            "Don'tFill pDCPatSgNB");
                    p_elem_item->resource_configuration.t = 
                        T_x2ap_E_RABs_ToBeReleased_SgNBRelReq_Item_resource_configuration_sgNBPDCPnotpresent;
                }

                rtxDListAppendNode(
                        p_e_rab_to_release_list,
                        p_node);
                ret_val = RRC_SUCCESS;
            }
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return ret_val;
}

/******************************************************************************
 *   FUNCTION NAME: build_and_send_sgnb_release_request
 *
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds and sends SgNB Release Request message  
 *       to X2AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
rrc_return_et build_and_send_sgnb_release_request
(
    uecc_ue_context_t*  p_ue_context
)
{
    rrc_return_et                            result = RRC_FAILURE;
    OSCTXT                                   asn1_ctx;
    rrc_x2ap_sgnb_release_request_t          msg;

    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN];

    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_ue_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "ASN context initialization failed");
        return RRC_FAILURE;
    }
    do
    {
        /* Reset message */
        memset_wrapper(&msg, 0, sizeof(msg));
        memset_wrapper(sig_msg_req_buf,0,sizeof(x2ap_ue_associated_sig_msg_req_t) + 
                X2AP_MAX_ASN1_BUF_LEN);

        /* Fill message */
        /*Fill  MeNB UE X2AP ID*/
        msg.menb_ue_x2ap_id = p_ue_context->x2ap_context.menb_ue_x2ap_id;

        
        /*
         * If SgNBAdditionRequestAcknowledge is not received by some cause,
         * then MeNB will not be having any SgNB-UE-X2AP-ID to send in the
         * SgNBReleaseRequest.
         */
        if (p_ue_context->x2ap_context.m.sgnb_ue_x2ap_id_present)
        {
            /*Set  Bitmask*/
            msg.bitmask |=
                X2AP_SGNB_REL_REQ_SGNB_UE_X2AP_ID_PRESENT;

            /*Fill  SgNB UE X2AP ID*/
            msg.sgnb_ue_x2ap_id = p_ue_context->x2ap_context.sgnb_ue_x2ap_id;
        }

        /*Fill cause*/ 
        msg.cause = p_ue_context->sgnb_cause;

        /* Fill Rab related data */
        if ( RRC_SUCCESS == uecc_x2ap_build_x2ap_sgnb_e_rab_release_list(
            p_ue_context,&asn1_ctx,&(msg.erab_to_release_list)))
        {
          msg.bitmask |= X2AP_SGNB_REL_REQ_ERB_REL_LIST_PRESENT; 
        }

        /* Encode message */
        result = x2ap_internal_encode_sgnb_release_request(&asn1_ctx,
                &p_sig_msg_req->message_buff_p[0],
                &p_sig_msg_req->message_buff_size,
                &msg);
        if (RRC_SUCCESS!=result)
        {
            RRC_UECC_TRACE(p_ue_context->context_index, 
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_SgNBReleaseRequest] Build failure");
            break;
        }

        p_sig_msg_req->procedure_code = 
            ASN1V_x2ap_id_meNBinitiatedSgNBRelease;
        p_sig_msg_req->message_type = X2AP_INITIATING_MESSAGE;
        p_sig_msg_req->peer_enodeb_id = p_ue_context->x2ap_context.peer_enodeb_id;

        p_sig_msg_req->bitmask |=
            X2AP_UE_ASSOC_SIG_MSG_REQ_MENB_UE_X2AP_ID_PRESENT;
        p_sig_msg_req->menb_ue_x2ap_id = p_ue_context->x2ap_context.menb_ue_x2ap_id;

        /* Send message to X2AP  */
        result = uecc_rrc_send_internal_msg(
                RRC_X2AP_MODULE_ID,
                X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                    p_sig_msg_req->message_buff_size),
                sig_msg_req_buf);

        RRC_UECC_TRACE(p_ue_context->context_index, 
                p_ue_context->p_gb_context->facility_name, RRC_INFO,
                "X2AP_MSG: SGNB RELEASE REQUEST sent to X2AP %s",
                change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));

    } while (0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);


    return result;
}
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_sgnb_release_request_ack
 *
 *   DESCRIPTION:
 *       This function handle sgnb release request ack
 *       to X2AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
void uecc_x2ap_process_sgnb_release_request_ack(
        OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
        x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
        uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
        U8                  peer_enodeb_id,      /* Peer EnodeB ID */
        U8                  *buffer
        )
{
	/*Error_Handling_changes*/
	U8 decoding_fail = RRC_SUCCESS;
	/*Error_Handling_changes*/
    uecc_ue_context_t               *p_ue_context = PNULL;
    rrc_x2ap_sgnb_release_request_ack_t msg;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    memset_wrapper(&msg,0, sizeof(rrc_x2ap_sgnb_release_request_ack_t));

    do
    {
        if(PNULL == p_x2ap_pdu->u.successfulOutcome->value.u.meNBinitiatedSgNBRelease)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR, "[MENB INITIATED SGNB REL ACK] "
                    "SGNB release field is NULL");
            break;
        }
        /* Decode message */
        if( X2AP_FAILURE == x2ap_internal_decode_menb_initiated_sgnb_release_ack (
                    p_asn1_ctx,
                    p_x2ap_pdu->u.successfulOutcome->value.u.meNBinitiatedSgNBRelease,
                    &msg,
                    peer_enodeb_id
                    ))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, " menb_initiated_sgnb_release_ack "
                    "Decoding Failure");
            /*Error_Handling_changes*/
            decoding_fail = RRC_FAILURE;
            /*Error_Handling_changes*/
            break;

        }

        /* coverity_281241_fix_start */
        if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
        {
            p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                    p_uecc_gb_context,
                    peer_enodeb_id,
                    msg.menb_ue_x2ap_id, 
                    MENB_X2AP_UE_ID);

            /*Error_Handling_changes*/
        /*Check for API-ID missmatch*/
            if (PNULL != p_ue_context )
            {
        if ( first_message == p_ue_context->msg_type )
        {
            if(RRC_FAILURE == check_for_api_id_mismatch(p_ue_context,
                        p_uecc_gb_context,
                        first_returned_message,
                        p_asn1_ctx,
                        msg.menb_ue_x2ap_id,
                        msg.sgnb_ue_x2ap_id,
                        p_x2ap_pdu->u.successfulOutcome->procedureCode,
                        p_x2ap_pdu->t,
                        peer_enodeb_id,
                        &msg
                        ))
            {
                break;
            }
        }
        else
        {
            if(RRC_FAILURE == check_for_api_id_mismatch(p_ue_context,
                        p_uecc_gb_context,
                        middle_message,
                        p_asn1_ctx,
                        msg.menb_ue_x2ap_id,
                        msg.sgnb_ue_x2ap_id,
                        p_x2ap_pdu->u.successfulOutcome->procedureCode,
                        p_x2ap_pdu->t,
                        peer_enodeb_id,
                        &msg
                        ))
            {
                break;
            }
        }
                /*Error_Handling_changes*/
            if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
                        p_uecc_gb_context,p_ue_context->cell_index))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING, "cell not active");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                break;
            }
            uecc_fsm_process_event(
                    p_ue_context,
                    UECC_EV_X2AP_SGNB_RELEASE_REL_ACK, 
                    &msg);
        }
        }
        /* coverity_281241_fix_end */
    } while (0);
/*Error_Handling_changes*/

    if(RRC_FAILURE == decoding_fail)
    {
        if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
        {
            p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                    p_uecc_gb_context,
                    peer_enodeb_id, 
                    msg.menb_ue_x2ap_id, 
                    MENB_X2AP_UE_ID);
        }
        if(PNULL != p_ue_context)
        {
            uecc_fsm_process_event(
                    p_ue_context,
                    UECC_EV_X2AP_ERROR_INDICATION,
                    &msg);
        }
    }
    /*Error_Handling_changes*/

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_x2ap_process_sgnb_initiated_sgnb_release_req_msg
*   INPUTS       : OSCTXT*             p_asn1_ctx 
*                  x2ap_X2AP_PDU       *p_x2ap_pdu
*                  uecc_gb_context_t   *p_uecc_gb_context
*                  U8                  peer_enodeb_id
*                  U8                  *buffer
*   OUTPUTS      : None
*   DESCRIPTION  : This function handles the sgnb initiated sgnb release
*                  request.
*   RETURNS:     : None
*******************************************************************************/
void uecc_x2ap_process_sgnb_initiated_sgnb_release_req_msg
(
    OSCTXT*             p_asn1_ctx,
    x2ap_X2AP_PDU       *p_x2ap_pdu,
    uecc_gb_context_t   *p_uecc_gb_context,
    U8                  peer_enodeb_id,
    U8                  *buffer
)
{
	/*Error_Handling_changes*/
	U8 decoding_fail = RRC_SUCCESS;
	/*Error_Handling_changes*/
    uecc_ue_context_t                   *p_ue_context = PNULL;
    rrc_x2ap_sgnb_release_required_t    msg;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    memset_wrapper(&msg, RRC_NULL, sizeof(rrc_x2ap_sgnb_release_required_t));

    do
    {

        if ( PNULL == p_x2ap_pdu->u.initiatingMessage->value.u.sgNBinitiatedSgNBRelease )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR, "[SGNB INITIATED SGNB REL] "
                    "SGNB release field is NULL");
            break;
        }

        /*
         * Decode message
         */
        if ( X2AP_FAILURE == x2ap_internal_decode_sgnb_initiated_sgnb_release(
                    p_asn1_ctx,
                    p_x2ap_pdu->u.initiatingMessage->value.u.sgNBinitiatedSgNBRelease,
                    &msg,
                    peer_enodeb_id
                    ) )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO, " sgnb_initiated_sgnb_release_req "
                    "Decoding Failure");
            /*Error_Handling_changes*/
            decoding_fail = RRC_FAILURE;
            /*Error_Handling_changes*/
            break;

        }

        if ( RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id )
        {
            p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                    p_uecc_gb_context,
                    peer_enodeb_id,
                    msg.menb_ue_x2ap_id, 
                    MENB_X2AP_UE_ID);
        }
/*Error_Handling_changes*/
            /*Check for API-ID missmatch*/
            if(RRC_FAILURE == check_for_api_id_mismatch(p_ue_context,
            	p_uecc_gb_context,
            	middle_message,
            	p_asn1_ctx,
            	msg.menb_ue_x2ap_id,
            	msg.sgnb_ue_x2ap_id,
            	p_x2ap_pdu->u.initiatingMessage->procedureCode,
            	p_x2ap_pdu->t,
            	peer_enodeb_id,
            	&msg
            	))
            {

            	break;
            }
/*Error_Handling_changes*/

        if ( PNULL != p_ue_context )
        {
            if ( RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
                        p_uecc_gb_context,p_ue_context->cell_index) )
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING, "cell not active");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                break;
            }
            uecc_fsm_process_event(
                    p_ue_context,
                    UECC_EV_X2AP_SGNB_RELEASE_REL_REQ, 
                    &msg);
        }
        
    } while (RRC_NULL);
/*Error_Handling_changes*/
    if(RRC_FAILURE == decoding_fail)
    {
        if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
        {
            p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                    p_uecc_gb_context,
                    peer_enodeb_id, 
                    msg.menb_ue_x2ap_id, 
                    MENB_X2AP_UE_ID);
        }
        if(PNULL != p_ue_context)
        {
            uecc_fsm_process_event(
                    p_ue_context,
                    UECC_EV_X2AP_ERROR_INDICATION,
                    &msg);
        }


    }
    /*Error_Handling_changes*/

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_x2ap_sgnb_el_erab_rel_cnf_list
 *   Input:         uecc_ue_context_t* p_ue_context
 *                  OSCTXT              *p_asn1_ctx
 *                  x2ap_E_RABs_ToBeReleased_SgNBRelConfList *p_e_rab_rel_list
 *   Outputs :      None
 *   DESCRIPTION:   This function builds x2ap_E_RABs_ToBeReleased_SgNBRelConfList
 *   RETURNS:       RRC_SUCCESS/RRC_FAILURE
 *******************************************************************************/
static rrc_return_et
uecc_x2ap_build_x2ap_sgnb_el_erab_rel_cnf_list
(
    uecc_ue_context_t*                          p_ue_context,
    OSCTXT                                      *p_asn1_ctx,
    x2ap_E_RABs_ToBeReleased_SgNBRelConfList    *p_e_rab_rel_list
)
{
    OSRTDListNode*                                      p_node = PNULL;
    x2ap_E_RABs_ToBeReleased_SgNBRelConfList_element    *p_elem = PNULL;
    x2ap_E_RABs_ToBeReleased_SgNBRelConf_Item           *p_elem_item = PNULL;
    U8                                                  e_rab_index = RRC_NULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_e_rab_rel_list);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    asn1Init_x2ap_E_RABs_ToBeReleased_SgNBRelConfList(p_e_rab_rel_list);

    for ( e_rab_index = RRC_NULL; 
            e_rab_index < MAX_ERAB_COUNT; 
            e_rab_index++ )
    {
        if ( PNULL == p_ue_context->p_e_rab_list[e_rab_index]) 
        {
            continue;
        }

        if((p_ue_context->p_e_rab_list[e_rab_index]->m.DC_BearerTypePresent) &&
                (( SN_TERMINATED_SCG_BEARER ==
                   p_ue_context->p_e_rab_list[e_rab_index]->dc_bearer_type) ||
                 ( SN_TERMINATED_SPLIT_BEARER ==
                   p_ue_context->p_e_rab_list[e_rab_index]->dc_bearer_type))
          )
        {
            rtxDListAllocNodeAndData(p_asn1_ctx,
                    x2ap_E_RABs_ToBeReleased_SgNBRelConfList_element,
                    &p_node,
                    &p_elem);

            if ( PNULL == p_node )
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, 
                        RRC_FATAL,"Can't allocate p_node");

                rtxDListFreeAll (p_asn1_ctx, p_e_rab_rel_list);

                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return RRC_FAILURE;
            }

            asn1Init_x2ap_E_RABs_ToBeReleased_SgNBRelConfList_element(p_elem);


            /* Allocate Memory */
            p_elem_item = 
                rtxMemAllocType(p_asn1_ctx,
                        x2ap_E_RABs_ToBeReleased_SgNBRelConf_Item);
            if ( PNULL == p_elem_item )
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, 
                        RRC_FATAL,"Can't allocate p_elem_item "
                        "MeNB UE X2AP ID [%u] "
                        "Peer eNodeB Id: [%u] "
                        "UE Index: [%u] "
                        "Cell Index: [%u]",
                        p_ue_context->x2ap_context.menb_ue_x2ap_id,
                        p_ue_context->x2ap_context.peer_enodeb_id,
                        p_ue_context->ue_index,
                        p_ue_context->cell_index);

                RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                return RRC_FAILURE;
            }
            p_elem->value.u._x2ap_E_RABs_ToBeReleased_SgNBRelConf_ItemIEs_1 =
                p_elem_item;

            asn1Init_x2ap_E_RABs_ToBeReleased_SgNBRelConf_Item(p_elem_item);

            p_elem->id = ASN1V_x2ap_id_E_RABs_ToBeReleased_SgNBRelConf_Item;
            p_elem->criticality = x2ap_ignore;
            p_elem->value.t = T185x2ap___x2ap_E_RABs_ToBeReleased_SgNBRelConf_ItemIEs_1;

            p_elem_item->e_RAB_ID = p_ue_context->
                p_e_rab_list[e_rab_index]->e_RAB_ID;

            p_elem_item->en_DC_ResourceConfiguration.pDCPatSgNB = x2ap_present;

            p_elem_item->en_DC_ResourceConfiguration.mCGresources = x2ap_present_2;

            p_elem_item->en_DC_ResourceConfiguration.sCGresources = x2ap_not_present_1;

            if ( SN_TERMINATED_SPLIT_BEARER == p_ue_context->p_e_rab_list[e_rab_index]->
                    dc_bearer_type )
            {
                p_elem_item->en_DC_ResourceConfiguration.mCGresources = x2ap_present_1;
            }
            else
            {
                p_elem_item->en_DC_ResourceConfiguration.mCGresources = x2ap_not_present_1;
            }

            if ( x2ap_present == p_elem_item->en_DC_ResourceConfiguration.pDCPatSgNB )
            { 
                p_elem_item->resource_configuration.t =
                    T_x2ap_E_RABs_ToBeReleased_SgNBRelConf_Item_resource_configuration_sgNBPDCPpresent;

                /* Allocate Memory */
                p_elem_item->resource_configuration.u.sgNBPDCPpresent =
                    rtxMemAllocType(p_asn1_ctx,
                            x2ap_E_RABs_ToBeReleased_SgNBRelConf_Item_SgNBPDCPpresent);

                if ( PNULL ==
                        p_elem_item->resource_configuration.u.sgNBPDCPpresent )
                {
                    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                            p_ue_context->p_gb_context->facility_name, 
                            RRC_FATAL,"Can't allocate p_elem_item->resource_configuration.u.sgNBPDCPpresent "
                            "MeNB UE X2AP ID [%u] "
                            "Peer eNodeB Id: [%u] "
                            "UE Index: [%u] "
                            "Cell Index: [%u]",
                            p_ue_context->x2ap_context.menb_ue_x2ap_id,
                            p_ue_context->x2ap_context.peer_enodeb_id,
                            p_ue_context->ue_index,
                            p_ue_context->cell_index);

                    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }

                asn1Init_x2ap_E_RABs_ToBeReleased_SgNBRelConf_Item_SgNBPDCPpresent(
                        p_elem->value.u._x2ap_E_RABs_ToBeReleased_SgNBRelConf_ItemIEs_1->
                        resource_configuration.u.sgNBPDCPpresent);

                p_elem->value.u._x2ap_E_RABs_ToBeReleased_SgNBRelConf_ItemIEs_1->
                    resource_configuration.u.sgNBPDCPpresent->m.iE_ExtensionsPresent= 0;

                p_elem->value.u._x2ap_E_RABs_ToBeReleased_SgNBRelConf_ItemIEs_1->
                    resource_configuration.u.sgNBPDCPpresent->m.uL_GTPtunnelEndpointPresent =
                    RRC_ONE;

                p_elem->value.u._x2ap_E_RABs_ToBeReleased_SgNBRelConf_ItemIEs_1->
                    resource_configuration.u.sgNBPDCPpresent->m.dL_GTPtunnelEndpointPresent =
                    RRC_ONE;
                p_elem_item->resource_configuration.u.sgNBPDCPpresent->uL_GTPtunnelEndpoint.m.iE_ExtensionsPresent=0;
                p_elem_item->resource_configuration.u.sgNBPDCPpresent->dL_GTPtunnelEndpoint.m.iE_ExtensionsPresent=0;

                /*update uL_GTPtunnelEndpoint IE*/
                asn1Copy_x2ap_TransportLayerAddress (p_asn1_ctx,
                        (x2ap_TransportLayerAddress *)&p_ue_context->p_e_rab_list[p_elem_item->\
                        e_RAB_ID]->transportLayerAddress,
                        &p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                        uL_GTPtunnelEndpoint.transportLayerAddress);


                p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                    uL_GTPtunnelEndpoint.gTP_TEID.numocts = 
                    p_ue_context->p_e_rab_list[p_elem_item->e_RAB_ID]->sgnb_s1ap_gtp_id.numocts;

                l3_memcpy_wrapper (p_elem_item->resource_configuration.u.sgNBPDCPpresent->
                        uL_GTPtunnelEndpoint.gTP_TEID.data,
                        p_ue_context->p_e_rab_list[p_elem_item->e_RAB_ID]->sgnb_s1ap_gtp_id.data,
                        p_ue_context->p_e_rab_list[p_elem_item->e_RAB_ID]->sgnb_s1ap_gtp_id.numocts);

                /*update dL_GTPtunnelEndpoint IE*/
                asn1Copy_x2ap_TransportLayerAddress (p_asn1_ctx,
                        (x2ap_TransportLayerAddress *)&p_ue_context->p_e_rab_list[p_elem_item->\
                        e_RAB_ID]->sgnb_s1ap_transportLayerAddress,
                        &p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                        dL_GTPtunnelEndpoint.transportLayerAddress);


                p_elem_item->resource_configuration.u.sgNBPDCPpresent->\
                    dL_GTPtunnelEndpoint.gTP_TEID.numocts = 
                    p_ue_context->p_e_rab_list[p_elem_item->e_RAB_ID]->sgnb_s1ap_gtp_id.numocts;

                l3_memcpy_wrapper (p_elem_item->resource_configuration.u.sgNBPDCPpresent->
                        dL_GTPtunnelEndpoint.gTP_TEID.data,
                        p_ue_context->p_e_rab_list[p_elem_item->e_RAB_ID]->sgnb_s1ap_gtp_id.data,
                        p_ue_context->p_e_rab_list[p_elem_item->e_RAB_ID]->sgnb_s1ap_gtp_id.numocts);

            }

            rtxDListAppendNode(
                    p_e_rab_rel_list,
                    p_node);
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

    return RRC_SUCCESS;
}
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_sgnb_initiated_sgnb_release_confirm
 *
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds and sends SgNB Release confirm message  
 *       to X2AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
rrc_return_et
uecc_x2ap_build_and_send_sgnb_initiated_sgnb_release_confirm
(
    uecc_ue_context_t*  p_ue_context
)
{
    rrc_return_et                            result = RRC_FAILURE;
    OSCTXT                                   asn1_ctx;
    rrc_x2ap_sgnb_release_confirm_t          msg;

    U8 sig_msg_req_buf[sizeof(x2ap_ue_associated_sig_msg_req_t)+
        X2AP_MAX_ASN1_BUF_LEN];

    x2ap_ue_associated_sig_msg_req_t *p_sig_msg_req =
        (x2ap_ue_associated_sig_msg_req_t *)sig_msg_req_buf;

    RRC_ASSERT(PNULL != p_ue_context);

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,RRC_INFO,
            "uecc_x2ap_build_and_send_sgnb_initiated_sgnb_release_confirm inside ");

    /*
     * Init ASN context
     */
    if ( RT_OK != rtInitContext(&asn1_ctx) )
    {
        RRC_UECC_TRACE(p_ue_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "ASN context initialization failed");
        RRC_UECC_TRACE(p_ue_context->context_index,
                p_ue_context->p_gb_context->facility_name, RRC_INFO,
                "ASN context initialization failed");
        RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
        return RRC_FAILURE;
    }

    do
    {
        /*
         * Reset message
         */
        memset_wrapper(&msg, RRC_NULL, sizeof(msg));
        memset_wrapper(sig_msg_req_buf, RRC_NULL, sizeof(x2ap_ue_associated_sig_msg_req_t) + 
                X2AP_MAX_ASN1_BUF_LEN);

        /*
         * Fill message
         */
        /*
         * Fill MeNB UE X2AP ID
         */
        msg.menb_ue_x2ap_id = p_ue_context->x2ap_context.menb_ue_x2ap_id;

        /*
         * If SgNBAdditionRequestAcknowledge is not received by some cause,
         * then MeNB will not be having any SgNB-UE-X2AP-ID to send in the
         * SgNBReleaseConfirm.
         */
        if ( p_ue_context->x2ap_context.m.sgnb_ue_x2ap_id_present )
        {
            /*
             * Fill SgNB UE X2AP ID
             */
            msg.sgnb_ue_x2ap_id = p_ue_context->x2ap_context.sgnb_ue_x2ap_id;
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,RRC_INFO,
                    "uecc_x2ap_build_and_send_sgnb_initiated_sgnb_release_confirm fill sgnb id");
        }

        if (RRC_SUCCESS != uecc_x2ap_build_x2ap_sgnb_el_erab_rel_cnf_list(
                    p_ue_context, &asn1_ctx, &(msg.E_RABs_ToBeReleased_SgNBRelConfList)))
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_SgNB_Rel_Cnf]"
                    "uecc_x2ap_build_x2ap_sgnb_el_erab_rel_cnf_list failure");

            break;
        }

        /*
         * Encode message
         */
        result = x2ap_internal_encode_sgnb_release_confirm(&asn1_ctx,
                &p_sig_msg_req->message_buff_p[RRC_NULL],
                &p_sig_msg_req->message_buff_size,
                &msg);
        if ( RRC_SUCCESS != result )
        {
            RRC_UECC_TRACE(p_ue_context->context_index, 
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "[x2ap_SgNBReleaseConfirm] Build failure");
            RRC_UECC_TRACE(p_ue_context->context_index, 
                    p_ue_context->p_gb_context->facility_name, RRC_INFO,
                    "[x2ap_SgNBReleaseConfirm] Build failure");
            break;
        }
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,RRC_INFO,
            "uecc_x2ap_build_and_send_sgnb_initiated_sgnb_release_confirm inside  encode result %d",result);

        p_sig_msg_req->procedure_code = 
            ASN1V_x2ap_id_sgNBinitiatedSgNBRelease;
        p_sig_msg_req->message_type = X2AP_SUCCESSFUL_OUTCOME;
        p_sig_msg_req->peer_enodeb_id = p_ue_context->x2ap_context.peer_enodeb_id;

        p_sig_msg_req->bitmask |=
            X2AP_UE_ASSOC_SIG_MSG_REQ_MENB_UE_X2AP_ID_PRESENT;
        p_sig_msg_req->menb_ue_x2ap_id = p_ue_context->x2ap_context.menb_ue_x2ap_id;

        p_sig_msg_req->bitmask |=
            X2AP_UE_ASSOC_SIG_MSG_REQ_SGNB_UE_X2AP_ID_PRESENT;
        p_sig_msg_req->sgnb_ue_x2ap_id = p_ue_context->x2ap_context.sgnb_ue_x2ap_id;

        /*
         * Send message to X2AP
         */
        result = uecc_rrc_send_internal_msg(
                RRC_X2AP_MODULE_ID,
                X2AP_UE_ASSOCIATED_SIG_MSG_REQ,
                (U16)(sizeof(x2ap_ue_associated_sig_msg_req_t) +
                    p_sig_msg_req->message_buff_size),
                sig_msg_req_buf);

        RRC_UECC_TRACE(p_ue_context->context_index, 
                p_ue_context->p_gb_context->facility_name, RRC_INFO,
                "X2AP_MSG: SGNB RELEASE CONFIRM sent to X2AP %s",
                change_data_to_str(p_sig_msg_req->message_buff_size, p_sig_msg_req->message_buff_p));

    } while (RRC_NULL);

    /*
     * Free ASN Context
     */
    rtFreeContext(&asn1_ctx);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
            p_ue_context->p_gb_context->facility_name,RRC_INFO,
            "uecc_x2ap_build_and_send_sgnb_initiated_sgnb_release_confirm retunr");

    return result;
}
/*Error_Handling_changes*/
/******************************************************************************
 *   FUNCTION NAME: check_for_api_id_mismatch
 *
 *   Outputs : None
 *   DESCRIPTION:
 *       This function check for API-ID missmatch and send error ind to SgNB
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
static  rrc_return_et check_for_api_id_mismatch(
		uecc_ue_context_t  	*p_ue_context,
		uecc_gb_context_t   *p_uecc_gb_context,
		msg_type_et 		msg_type,
		OSCTXT*  			p_asn1_ctx,
		U32 				menb_ue_x2ap_id,
		U32 				sgnb_ue_x2ap_id,
		U16 				proc_code,
		U8 					triggering_msg,
		U8       			peer_enodeb_id,
		void*               msg_p
		)
{

	U8 	  send_error_ind = RRC_FALSE;
	uecc_ue_context_t   *p_second_ue_context = PNULL;
	rrc_x2ap_error_indication_t error_indication;

    /* coverity_280112_fix_start */
     memset_wrapper(&error_indication,RRC_NULL,sizeof(error_indication));
    /* coverity_280112_fix_end */

    /*Fetch UE_CONTEXT based on peer_enodeb_id & sgnb_ue_x2ap_id*/
    p_second_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                    p_uecc_gb_context,
                    peer_enodeb_id,
                    sgnb_ue_x2ap_id, 
                    SGNB_X2AP_UE_ID);

    /*Check if received unknown MeNB_X2AP_ID or SgNB_X2AP_ID already allocated to other  UE*/
    if(((msg_type == first_returned_message) || (msg_type == first_message)) && 
    		((PNULL == p_ue_context) || (PNULL != p_second_ue_context)))
    {
		send_error_ind = RRC_TRUE; 
    }

    /*Check if received Unknown or inconsistent MeNB_X2AP_ID */
    else if (((msg_type == middle_message) || (msg_type == last_message))
    		&& ((PNULL == p_ue_context) || (PNULL == p_second_ue_context) ||
    		((PNULL != p_second_ue_context) && (p_second_ue_context->x2ap_context.menb_ue_x2ap_id != menb_ue_x2ap_id))))
    {
		send_error_ind = RRC_TRUE; 
    }

    /*Send Error indication if it is not last message from peer node*/
    if((msg_type != last_message) && (RRC_TRUE == send_error_ind))
    {
		error_indication.bitmask |= RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
    	error_indication.new_enb_ue_x2ap_id = menb_ue_x2ap_id;
		error_indication.bitmask |= RRC_X2AP_ERROR_IND_OLD_SGNB_UE_X2AP_ID_PRESENT;
    	error_indication.old_sgnb_ue_x2ap_id = sgnb_ue_x2ap_id;

    	uecc_x2ap_fill_error_indication_values(&error_indication,
            	(x2ap_ProcedureCode)proc_code,
            	triggering_msg,
            	x2ap_notify);

        error_indication.cause.t = T_x2ap_Cause_radioNetwork;
        if(PNULL == p_ue_context && PNULL == p_second_ue_context)
        {
        	error_indication.cause.u.protocol = x2ap_unknown_pair_of_UE_X2AP_ID; 
        }
        else if(PNULL == p_ue_context)
        {
        	error_indication.cause.u.protocol = x2ap_unknown_new_eNB_UE_X2AP_ID; 
        }
        else
        {
        error_indication.cause.u.protocol = x2ap_unknown_old_en_gNB_UE_X2AP_ID; 
        }
		/*
    	error_indication.criticality_diagnostics.m.iEsCriticalityDiagnosticsPresent = 
        	RRC_X2AP_SET_MEMBER;

    	error_indication.bitmask |=
        	RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;

    	uecc_x2ap_build_criticality_diagnostics_list(p_asn1_ctx,
            	&error_indication.criticality_diagnostics.iEsCriticalityDiagnostics,
            	p_ie_list);*/

    	uecc_x2ap_build_and_send_err_indication(peer_enodeb_id,
            	&error_indication);

		/*Release UE-associated logical connection having the erroneous AP ID(s)*/
		if(PNULL != p_ue_context)
		{
 			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING, 
                    "Error Occured in SgNB specific procedure for UE %d : UECC_EV_X2AP_ERROR_INDICATION",p_ue_context->ue_index);

 	 		uecc_fsm_process_event(
                	p_ue_context,
                	UECC_EV_X2AP_ERROR_INDICATION,
                	&error_indication);
		}
        if(PNULL != p_second_ue_context)
        {
 			RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_WARNING, 
                    "Error Occured in SgNB specific procedure for UE %d : UECC_EV_X2AP_ERROR_INDICATION",p_second_ue_context->ue_index);

         	uecc_fsm_process_event(
                	p_second_ue_context,
                	UECC_EV_X2AP_ERROR_INDICATION,
                	&error_indication);
        }
    	return RRC_FAILURE;
	}
    return RRC_SUCCESS;

}

/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_handle_endc_error_ind
 *
 *   Input: 
 *        uecc_gb_context_t            *p_uecc_gb_context
 *        rrc_x2ap_error_indication_t  *p_msg
 *        U8                           peer_enodeb_id
 *        U32				           encoded_msg_len
 *        U8                           *buffer
 *   Outputs : None
 *   DESCRIPTION:
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void uecc_x2ap_handle_endc_error_ind 
(
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 rrc_x2ap_error_indication_t     *p_msg,  /* X2AP Error Indication Message */
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 U32				 encoded_msg_len,     /* Encoded Message Length */
 U8                  *buffer
)
{
    uecc_ue_context_t               *p_ue_context = PNULL;
    uecc_ue_context_t               *p_second_ue_context = PNULL;
    rrc_bool_et                     rrc_get_second_ue_ctx = RRC_FALSE;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_ASSERT(PNULL!=p_msg);

    /*check for MeNB UE X2AP ID*/
    if (p_msg->bitmask & RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT)
    {
        p_ue_context= uecc_ue_ctx_get_by_x2ap_id_src_enb(
                p_uecc_gb_context, peer_enodeb_id, 
                p_msg->new_enb_ue_x2ap_id, MENB_X2AP_UE_ID);

        if (PNULL != p_ue_context) 
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "[x2ap_ErrorIndication] push event - "
                    "UECC_EV_X2AP_ERROR_INDICATION for UE Index [%d] matching (New) MeNB UE X2 ID %u",
                    p_ue_context->ue_index, p_msg->new_enb_ue_x2ap_id);

            uecc_fsm_process_event(
                    p_ue_context,
                    UECC_EV_X2AP_ERROR_INDICATION,
                    p_msg);
        }
    }
    if ((PNULL != p_ue_context) && 
            (p_ue_context->x2ap_context.m.sgnb_ue_x2ap_id_present) &&
            (p_msg->bitmask & RRC_X2AP_ERROR_IND_OLD_SGNB_UE_X2AP_ID_PRESENT) &&
            (p_ue_context->x2ap_context.sgnb_ue_x2ap_id !=
             p_msg->old_sgnb_ue_x2ap_id))
    {
        rrc_get_second_ue_ctx = RRC_TRUE;
    }
    
    if (RRC_TRUE == rrc_get_second_ue_ctx)
    {
        p_second_ue_context = 
            uecc_ue_ctx_get_by_x2ap_id_src_enb(
                    p_uecc_gb_context,
                    peer_enodeb_id, 
                    p_msg->old_sgnb_ue_x2ap_id, SGNB_X2AP_UE_ID);

        if (PNULL != p_second_ue_context) 
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "[x2ap_ErrorIndication] push event - "
                    "UECC_EV_X2AP_ERROR_INDICATION for UE Index [%d] matching (Old) SgNB UE X2 ID %u",
                    p_second_ue_context->ue_index, p_msg->old_sgnb_ue_x2ap_id);

            uecc_fsm_process_event(
                    p_second_ue_context,
                    UECC_EV_X2AP_ERROR_INDICATION,
                    p_msg);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/*Error_Handling_changes*/

/****************************************************************************
 * Function Name  : uecc_x2ap_read_secondary_rat_usage_report_list
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  x2ap_SecondaryRATUsageReportList* - secondary rat usage report list
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion read secondary rat usage report list and 
 *                  put in share data
 ****************************************************************************/
rrc_return_t uecc_x2ap_read_secondary_rat_usage_report_list(
        uecc_ue_context_t*                                  p_ue_context,    
        x2ap_SecondaryRATUsageReportList*   p_sec_rat_usage_rep_list
)
{
    U8                           index             = RRC_NULL;
    U8                          index_erab      = RRC_NULL;
    OSRTDListNode                *p_node           = PNULL; 
    OSRTDListNode                *p_node_erab           = PNULL; 
    x2ap_SecondaryRATUsageReport_Item *p_sec_rat_usage_rep_item = PNULL;
    x2ap_E_RABUsageReport_Item *p_erab_usage_rep_item = PNULL;
    
    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_sec_rat_usage_rep_list); 

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    for (p_node = p_sec_rat_usage_rep_list->head;
            (p_node != PNULL && index < MAX_NUM_OF_ERAB); p_node = p_node->next, index++)
    {
        p_sec_rat_usage_rep_item = 
            ((x2ap_SecondaryRATUsageReportList_element *)(p_node->data))->value.u.
                _x2ap_SecondaryRATUsageReport_ItemIEs_1;

        /* erab id  */
        p_ue_context->secondary_rat_data_usage_report_list.
            rrc_s1ap_SecondaryRATDataUsageReport_ele[index].erab_id = 
            p_sec_rat_usage_rep_item->e_RAB_ID;  
            
        /* rat type */ 
        p_ue_context->secondary_rat_data_usage_report_list.
            rrc_s1ap_SecondaryRATDataUsageReport_ele[index].secondary_rat_type = 
            (rrc_s1ap_Secondary_Rat_Type_et)p_sec_rat_usage_rep_item->secondaryRATType;

        index_erab = RRC_NULL;
        /* erab usage report list */
        for (p_node_erab = p_sec_rat_usage_rep_item->e_RABUsageReportList.head;
                p_node_erab != PNULL && index_erab < MAX_NUM_OF_TIME_PERIODE; 
                p_node_erab = p_node_erab->next, index_erab++)
        {
            p_erab_usage_rep_item = 
                ((x2ap_E_RABUsageReportList_element *)(p_node_erab->data))->value.u.
                    _x2ap_E_RABUsageReport_ItemIEs_1;

            /* start time stamp */
            l3_memcpy_wrapper(p_ue_context->secondary_rat_data_usage_report_list.
                rrc_s1ap_SecondaryRATDataUsageReport_ele[index].erab_usage_report_list.
                erab_usage_report_item[index_erab].start_time_stamp, 
                p_erab_usage_rep_item->startTimeStamp.data, 4);

            /* end time stamp */
            l3_memcpy_wrapper(p_ue_context->secondary_rat_data_usage_report_list.
                rrc_s1ap_SecondaryRATDataUsageReport_ele[index].erab_usage_report_list.
                erab_usage_report_item[index_erab].end__time_stamp, 
                p_erab_usage_rep_item->endTimeStamp.data, 4); 

            /* UL usage count */
            p_ue_context->secondary_rat_data_usage_report_list.
                rrc_s1ap_SecondaryRATDataUsageReport_ele[index].erab_usage_report_list.
                erab_usage_report_item[index_erab].ul_usage_count = 
                    p_erab_usage_rep_item->usageCountUL; 

            /* DL usage count */
            p_ue_context->secondary_rat_data_usage_report_list.
                rrc_s1ap_SecondaryRATDataUsageReport_ele[index].erab_usage_report_list.
                erab_usage_report_item[index_erab].dl_usage_count = 
                    p_erab_usage_rep_item->usageCountDL;           
        }

        p_ue_context->secondary_rat_data_usage_report_list.
                rrc_s1ap_SecondaryRATDataUsageReport_ele[index].erab_usage_report_list.count = 
                    p_sec_rat_usage_rep_item->e_RABUsageReportList.count;           
    }

    p_ue_context->secondary_rat_data_usage_report_list.count = 
        p_sec_rat_usage_rep_list->count;

    p_ue_context->m.secondary_rat_data_usage_report_list_updated = RRC_TRUE;
    
    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return RRC_SUCCESS;
}       

/****************************************************************************
 * Function Name  : uecc_x2ap_handle_secondary_rat_data_usage_report_msg_rcvd
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void*   - api data
 * Outputs        : None
 * Returns        : rrc_return_et
 * Description    : This is the function for Stroing SecondaryRatDataUsageReport
 *                  message in uecc context.
 ****************************************************************************/
rrc_return_et uecc_x2ap_handle_secondary_rat_data_usage_report_msg_rcvd(
        uecc_ue_context_t*  p_ue_context,
        void*               p_api
        )
{
    rrc_return_et       result                              = RRC_SUCCESS;
    rrc_x2ap_secondary_rat_data_usage_report_t 
        *p_rrc_x2ap_sec_rat_data_usage_rep   = PNULL;

    RRC_ASSERT(PNULL != p_ue_context);
    RRC_ASSERT(PNULL != p_ue_context->p_gb_context);
    RRC_ASSERT(PNULL != p_api);
    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    do
    {
        /* storing the SecondaryRatDataUsageReport received */
        p_rrc_x2ap_sec_rat_data_usage_rep = (rrc_x2ap_secondary_rat_data_usage_report_t*)p_api;      

        if ((p_ue_context->x2ap_context.menb_ue_x2ap_id == 
                p_rrc_x2ap_sec_rat_data_usage_rep->menb_ue_x2ap_id) &&
                (RRC_TRUE == p_ue_context->x2ap_context.m.sgnb_ue_x2ap_id_present && 
                    p_ue_context->x2ap_context.sgnb_ue_x2ap_id == 
                    p_rrc_x2ap_sec_rat_data_usage_rep->sgnb_ue_x2ap_id))
        {
            /* Read secondary rat usage report list */
            if (RRC_SUCCESS == uecc_x2ap_read_secondary_rat_usage_report_list(
                        p_ue_context,    
                        &p_rrc_x2ap_sec_rat_data_usage_rep->secondary_rat_usage_rep_list))
            {
                result = RRC_SUCCESS;
            }
            else
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        " Secondary Rat Data Usage Report "
                        "Error in processing secondary rat usage report list!");
                result = RRC_FAILURE;
                break;
            }
        }
        else
        {
        	RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_WARNING,
                    " Secondary Rat Data Usage Report "
                    "MeNB_UE_X2AP_ID or SgNB_UE_X2AP_ID Mismatch");

        	return RRC_FAILURE;
        }

    } while(RRC_NULL);

    RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
    return result;
}

 /******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_process_secondary_rat_data_usage_report_msg
 *
 *   Input:  
 *          OSCTXT              *p_asn1_ctx
 *          x2ap_X2AP_PDU       *p_x2ap_pdu
 *          uecc_gb_context_t*  p_uecc_gb_context
 *          U8                  peer_enodeb_id
 *          U8                  *buffer
 *          void                *p_api
 *   Outputs : None
 *   DESCRIPTION:
 *       This function processes X2AP Process Secondary Rat data usage report message.
 *
 *   RETURNS:     None
 *
 ******************************************************************************/
void uecc_x2ap_process_secondary_rat_data_usage_report_msg
(
    OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    U8                  *buffer
)
{
    rrc_return_et                       retVal = RRC_SUCCESS;
    uecc_ue_context_t               *p_ue_context = PNULL;
    rrc_x2ap_secondary_rat_data_usage_report_t   msg;

    rrc_x2ap_error_indication_t     error_indication;
    rrc_return_et                   response = RRC_SUCCESS;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    
    memset_wrapper(&error_indication, 0, sizeof(rrc_x2ap_error_indication_t));

    do
    {
        if(PNULL ==
                p_x2ap_pdu->u.initiatingMessage->value.u.secondaryRATDataUsageReport)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR, "[X2AP_SECONDARY RAT DATA USAGE REPORT]"
                    " secondary rat data usage report field is NULL");
            break;
        }
        
        /* Decode message */
        if( X2AP_FAILURE == x2ap_internal_decode_secondary_rat_data_usage_report(
                    p_asn1_ctx,                    
                    p_x2ap_pdu->u.initiatingMessage->value.u.secondaryRATDataUsageReport,
                    &msg,
                    peer_enodeb_id
                    ))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR,
             "[X2AP SECONDARY RAT DATA USAGE REPORT] Decoding Failure");
            break;
        }    
        else
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_ERROR,
             "[X2AP SECONDARY RAT DATA USAGE REPORT] Decoding Success");
        }    

        if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
        {
            p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(
                    p_uecc_gb_context,
                    peer_enodeb_id,
                    msg.menb_ue_x2ap_id, 
                    MENB_X2AP_UE_ID);
        }

        if (PNULL != p_ue_context )
        {
            if(RRC_FAILURE == rrc_uecc_check_is_cell_valid_and_active(
                        p_uecc_gb_context,p_ue_context->cell_index))
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING, "cell not active");
                break;
            } 
        }
        
        rrc_asn1PrtToStr_X2AP_PDU(RRC_ASN, "X2AP_PDU", p_x2ap_pdu);
/* X2AP FIX_AG_2 : Start*/

        if (PNULL == p_ue_context )
        {
            if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
            {
                /* Error. Can't find UE context */
                RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_WARNING,
                        "[X2AP SECONDARY RAT DATA USAGE REPORT] UE context not found");

                error_indication.bitmask |=
                    RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
                error_indication.old_enb_ue_x2ap_id = msg.menb_ue_x2ap_id;
                error_indication.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                error_indication.cause.t = T_x2ap_Cause_radioNetwork;
                error_indication.cause.u.radioNetwork = x2ap_unknown_old_eNB_UE_X2AP_ID;
                error_indication.criticality_diagnostics.m.procedureCodePresent = 1;
                error_indication.criticality_diagnostics.procedureCode =
                    ASN1V_x2ap_id_secondaryRATDataUsageReport;
                    
                /* Get the UE Context matched with the menb_enb_ue_x2ap_id */
                /* received in the X2AP SECONDARY RAT DATA USAGE REPORT Message */
                p_ue_context = uecc_ue_ctx_get_by_x2ap_id_src_enb(p_uecc_gb_context,
                        peer_enodeb_id, msg.sgnb_ue_x2ap_id, SGNB_X2AP_UE_ID);
                        
                if ((PNULL != p_ue_context) && (RRC_NULL == p_ue_context->
                            x2ap_context.m.error_indication_ongoing))
                {
                    p_ue_context->x2ap_context.m.error_indication_ongoing = 1;

                    uecc_fsm_process_event(
                            p_ue_context,
                            UECC_EV_X2AP_ERROR_INDICATION,
                            &error_indication);
                }
            }

            response = RRC_FAILURE;

            break;
        }
        else
        { 
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
             "[X2AP SECONDARY RAT DATA USAGE REPORT] UE context for MENB_X2AP_UE_ID %u found", 
             msg.menb_ue_x2ap_id);
        }    
        
/* X2AP FIX_AG_2 : End*/
       /* fill ue context */
        if (PNULL != p_ue_context)
        {
            retVal = uecc_x2ap_handle_secondary_rat_data_usage_report_msg_rcvd(
                p_ue_context, &msg);
        }

        if (RRC_SUCCESS == retVal)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
            "[X2AP SECONDARY RAT DATA USAGE REPORT]: build and send - UECC_EV_X2AP_SECONDARY_RAT_DATA_USAGE_REPORT to S1AP");

            /* build and send msg to S1AP */
            uecc_s1ap_build_and_send_secodary_rat_data_usage_report(p_ue_context);
        }
        else
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                        facility_name, RRC_INFO,
                        "[X2AP SECONDARY RAT DATA USAGE REPORT] fill UE Context fail for UE ID [%d]. ",
                        p_ue_context->ue_index);
        }
    } while (0);
    
    if (RRC_FAILURE == response)
    {
        if ((RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT &
             error_indication.bitmask) ||
             (RRC_X2AP_ERROR_IND_OLD_SGNB_UE_X2AP_ID_PRESENT &
              error_indication.bitmask) ||
             (RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT &
              error_indication.bitmask))
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->
                    facility_name, RRC_INFO,
                    "build and send error-indication message");

            if (RRC_INVALID_PEER_ENODEB_ID != peer_enodeb_id)
            {
                uecc_x2ap_build_and_send_err_indication(
                        peer_enodeb_id,
                        &error_indication);
            }
        }
    }
    
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}
/*EN-DC_changes_end*/
#endif

