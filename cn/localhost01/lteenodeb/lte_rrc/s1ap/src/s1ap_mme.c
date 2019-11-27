/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 *  File Details
 *  ------------
 *  $Id: s1ap_mme.c,v 1.11 2010/10/13 07:04:22 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file contains the procedures that processes the 
 *  messages coming from MME to be delivered from s1ap CSPL 
 *  module to other CSPL modules.
 *
 ****************************************************************************
 *
 *  Revision Details
 *  ----------------
 *  $Log: s1ap_mme.c,v $
 *  Revision 1.11  2010/10/13 07:04:22  gur18569
 *  Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 *  Revision 1.8.4.16  2010/10/07 11:00:55  gur04518
 *  map type ASN1V_s1ap_id_Paging to S1_PAGING
 *
 *  Revision 1.8.4.15  2010/10/07 07:02:15  gur04518
 *  corrected the enum against the switch case check
 *
 *  Revision 1.8.4.14  2010/10/05 12:40:43  gur22047
 *  compilation warning removed
 *
 *  Revision 1.8.4.13  2010/10/04 13:31:54  gur04518
 *  removed backslash
 *
 *  Revision 1.8.4.12  2010/09/22 15:12:30  gur25381
 *  updated for kclockwork warning fixes
 *
 *  Revision 1.8.4.11  2010/09/09 10:09:48  gur22047
 *  modified_during_s1ap_UT added_stream_id
 *
 *  Revision 1.8.4.10  2010/08/30 10:46:05  gur22047
 *  files_modified during UT
 *
 *  Revision 1.8.4.9  2010/08/16 13:43:26  gur22047
 *  lokesh_review_incorporate
 *
 *  Revision 1.8.4.8  2010/08/16 12:10:03  gur22047
 *  lokesh_review_incorporate
 *
 *  Revision 1.8.4.7  2010/08/05 08:10:44  gur15697
 *  merging round 2
 *
 *  Revision 1.8.4.6  2010/07/21 11:56:35  gur18569
 *  incorporated code comments
 *
 *  Revision 1.8.4.5  2010/07/21 08:44:21  gur15697
 *  HO ERAB code Merging changes
 *
 *  Revision 1.8.4.4  2010/07/12 10:15:03  gur22047
 *  code review comments incorporate
 *
 *  Revision 1.8.4.3  2010/06/29 10:10:28  gur22047
 *  s1ap changes
 *
 *  Revision 1.8.4.2  2010/06/29 06:18:28  gur22047
 *  internal source and s1ap code
 *
 *  Revision 1.8.4.1  2010/06/24 12:58:32  gur22047
 *  internal source and s1ap code
 *  Revision 1.10  2010/07/24 07:18:48  gur04733
 *  Checking On 24 July
 *
 *  Revision 1.9  2010/06/22 09:31:59  gur21231
 *  ERAB RELEASE
 *
 *  Revision 1.8.2.2  2010/07/01 05:20:39  gur10248
 *  retrive old version before s1ap asn
 *
 *  Revision 1.8.2.1  2010/06/30 07:28:17  gur10248
 *  new s1ap folder
 *
 *  Revision 1.8  2010/04/06 04:03:40  gur18569
 *  reset fwk chg
 *
 *  Revision 1.7  2010/03/24 09:50:45  gur18569
 *  Merged files from Rel 1.0 support branch
 *
 *  Revision 1.6  2010/03/04 07:01:31  gur21006
 *  comments incorporated
 *
 *  Revision 1.5  2010/03/04 06:39:24  gur21006
 *  code comments incorporated
 *
 *  Revision 1.4  2010/03/04 06:18:57  gur21006
 *  code comments incorporated
 *
 *  Revision 1.3  2010/02/18 04:25:20  gur10248
 *  LTE_RRC_Rel1.1_18feb
 *
 *  Revision 1.2  2010/01/04 16:10:05  ukr15916
 *  no message
 *
 *  Revision 1.1.2.16  2009/12/28 10:14:16  gur21006
 *  system calls removed
 *
 *  Revision 1.1.2.15  2009/12/28 05:09:42  gur18569
 *  fixed indentation
 *
 *  Revision 1.1.2.14  2009/12/27 08:39:04  gur18569
 *  removed tabs
 *
 *  Revision 1.1.2.13  2009/12/26 12:05:01  gur21006
 *  Warnings removed
 *
 *  Revision 1.1.2.12  2009/12/24 10:26:51  gur21006
 *  Warnings removed
 *
 *  Revision 1.1.2.11  2009/12/23 06:40:06  gur21006
 *  Warning Removed
 *
 *  Revision 1.1.2.10  2009/12/22 14:37:41  gur18569
 *  fixed 154
 *
 *  Revision 1.1.2.9  2009/12/19 03:51:12  gur18569
 *  change in signature of process dl nas messsage
 *
 *  Revision 1.1.2.8  2009/12/18 04:23:40  gur18569
 *  checking ue id pair presence : spr 183
 *
 *  Revision 1.1.2.7  2009/12/15 14:54:54  gur18569
 *  Checkin whether any reset EP is active or not before processing reset ack req
 *
 *  Revision 1.1.2.6  2009/12/11 04:46:26  gur21006
 *  Added support for sctp shutdown event
 *
 *  Revision 1.1.2.5  2009/12/08 14:13:13  gur18569
 *  using message id from ASN files SPR:161
 *
 *  Revision 1.1.2.4  2009/12/04 11:51:20  gur21006
 *  New functions added
 *
 *  Revision 1.1.2.3  2009/12/03 10:11:36  gur18569
 *  integration changes
 *
 *  Revision 1.1.2.2  2009/11/26 18:33:49  ukr16018
 *  Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 *  Revision 1.1.2.1  2009/11/25 13:11:05  gur18569
 *  Shifted to level of rrc dir
 *
 *  Revision 1.1.2.18  2009/11/23 15:09:13  gur18569
 *  ut fixes
 *
 *  Revision 1.1.2.17  2009/11/23 10:55:19  gur21006
 *  Each timer id initialised to NULL after rrc_stop_timer
 *
 *  Revision 1.1.2.16  2009/11/22 12:05:43  gur18569
 *  freeing deleted nodes from search tree
 *
 *  Revision 1.1.2.15  2009/11/22 11:48:54  gur18569
 *  ut fixes
 *
 *  Revision 1.1.2.14  2009/11/21 13:11:17  gur21006
 *  ERROR check added
 *
 *  Revision 1.1.2.13  2009/11/20 14:06:12  gur21006
 *  Segmentation fault removed
 *
 *  Revision 1.1.2.12  2009/11/20 09:17:59  gur21006
 *  Arguments for s1 setup resp and resp failure changed
 *
 *  Revision 1.1.2.11  2009/11/19 13:02:09  gur18569
 *  fixed ut bug
 *
 *  Revision 1.1.2.10  2009/11/19 12:48:53  gur20470
 *  Included error handling for checking NULL against cell context created
 *
 *  Revision 1.1.2.9  2009/11/19 06:31:48  gur21006
 *  Review comments incorporated
 *
 *  Revision 1.1.2.8  2009/11/19 06:02:39  gur20470
 *  Included Error Logs
 *
 *  Revision 1.1.2.7  2009/11/18 12:36:26  gur20470
 *  Review comments incorporated
 *
 *  Revision 1.1.2.6  2009/11/17 13:46:25  gur20470
 *  Incorporated review comments
 *
 *  Revision 1.1.2.5  2009/11/10 11:41:55  gur18569
 *  review comments incorporated
 *
 *  Revision 1.1.2.4  2009/11/08 11:07:29  gur20470
 *  removed compilation warnings
 *
 *  Revision 1.1.2.3  2009/10/27 06:14:53  gur18569
 *  added case ASN_MSG_TO_FORWARD
 *
 *  Revision 1.1.2.2  2009/10/26 09:21:50  gur21006
 *  File modified
 *
 *  Revision 1.1.2.1  2009/10/23 16:10:17  gur18569
 *  Initial version
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "s1ap_mme.h"
#include "s1ap_mme_fsm.h"
#include "s1ap_enb_config_fsm.h"
#include "s1ap_csc.h"
#include "s1ap_uecc.h"
#include "s1ap_oam.h"
#include "s1ap_asn_codec.h"
#include "rrc_s1apRrm_intf.h"
#include "rrc_s1apRrm_il_composer.h"
#include "list_wrap.h"
/* etws_start*/
#include "s1ap_enb_mgr_fsm.h"
#include "s1ap_error_indication.h"
/* etws_end*/
#include "rrc_common_md.h"
#include "rrc_logging.h"
/* rim_start */
#include "s1ap_rrm_rim_intl.h"
/* rim_end */
#include "s1ap_lppa_asn_codec.h"
/*SPR 19183 START*/
#include "s1ap_rrm.h"
/*SPR 19183 END*/

 /****************************************************************************
   Private Definitions
  ****************************************************************************/

 /****************************************************************************
   Private Types
  ****************************************************************************/
 /****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/
static rrc_return_et compare_reset_info(s1ap_reset_info_t * p_recv_reset_info,
        s1ap_reset_info_t * p_reset_info);
static void  process_mme_initiated_reset_all_proc( 
         /* Pointer to S1AP global context */
         s1ap_gb_context_t * p_s1ap_gb_context,
         /* Pointer to mme context */
         mme_context_t * p_mme_context,
         /* Pointer to reset info */
         s1ap_reset_info_t *p_reset_info,
         rrc_s1ap_error_indication_t *p_err_ind,
         s1ap_error_ind_bool_t *p_send_err_ind);
static void s1ap_process_reset_ack(void  *p_asn_msg,      
         mme_context_t   *p_mme_context,
         s1ap_gb_context_t    *p_s1ap_gb_ctx,
         U16 asn_msg_len);
static void process_mme_initiated_reset_part(s1ap_gb_context_t *p_s1ap_gb_ctx,
        mme_context_t *p_mme_context,
        s1ap_reset_info_t *p_reset_info,
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_error_ind_bool_t *p_send_err_ind);
static void s1ap_process_ho_request(
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        s1ap_MME_UE_S1AP_ID mme_ue_s1ap_id,
        U16 stream_id,
        U8 mme_id,
        U8 release_of_mme,
        void  *p_asn_msg,
        U16 asn_msg_len);
static void s1ap_process_mme_error_indication(
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        U8 mme_id,
        void  *p_asn_msg,
        U16 asn_msg_len);
static void s1ap_process_overload_start(
        mme_context_t *p_mme_context,
        U8 mme_id,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len);
static void s1ap_process_overload_stop(
        mme_context_t *p_mme_context,
        U8 mme_id,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len);
static void s1ap_process_enb_config_update_ack(U8 mme_id,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len);
static void s1ap_process_enb_config_update_failure(U8 mme_id,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len);
static void s1ap_process_mme_configuration_update(
        mme_context_t* p_mme_context,
        void* p_asn_msg,
        U16 asn_msg_len);
static void s1ap_process_mme_direct_info_transfer(
/*SPR 19183 START*/       
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
/*SPR 19183 END*/
        mme_context_t* p_mme_context,
        U8 mme_id,
        void *p_asn_msg,
        U16 asn_msg_len);

static void s1ap_process_mme_configuration_transfer(
        mme_context_t* p_mme_context,    
        U8 mme_id,
        s1ap_gb_context_t *p_s1ap_gb_ctxt,
        void *p_asn_msg);

static void  s1ap_process_location_reporting_control(
        U8 mme_id,
        void*  p_asn_msg,
        U16    asn_msg_len,
        s1ap_ENB_UE_S1AP_ID enb_ue_s1ap_id,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        U8                   ue_id_pair_presence,
        U32                    mme_ue_s1ap_id,
        mme_context_t         *p_mme_context,
        U16                   stream_id);

/* etws_start*/
static void s1ap_process_write_replace_warning_request(
        U8 mme_id,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len);
/* etws_end*/

void s1ap_process_kill_request(U8 mme_id,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len);

static void s1ap_mme_error_ind_proc_code_s1setup(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        U8 mme_id);/*SPR 17777 +-*/

static void s1ap_mme_error_ind_proc_code_reset(
        s1ap_gb_context_t *p_s1ap_gb_ctx);/*SPR 17777 +-*/

static void s1ap_mme_error_ind_proc_code_enb_configuration_update(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        U8 mme_id);/*SPR 17777 +-*/

/*SPR 19183 START*/
static rrc_return_et rim_parse_incoming_tlv(
   s1ap_rrm_mme_direct_info_transfer_t *p_s1ap_rrm_direct_info,
   s1ap_gb_context_t *p_s1ap_gb_ctx);
/*SPR 19183 END*/


static rrc_return_et compose_intl_rim_err_pdu(
        rim_information_t *p_rim_info,
        /*SPR 19183 START*/
        ran_information_error_rim_t *p_raninfo_err_rim,
        /*SPR 19183 END*/
        U8 rim_info[MAX_BSSGP_PDU_LEN],
        U32      len_rim_info,
        U8       *p_cause);

static void s1ap_process_dl_ue_asso_lppa_transport(U8 mme_id, 
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len);

static void s1ap_process_dl_non_ue_asso_lppa_transport(U8 mme_id, 
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len);

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/*SPR 21220 Start*/
extern U32 delete_s1ap_context( U32 mme_ue_s1ap_id,
        mme_context_t * p_mme_context,
        U32* deleted_enb_ue_s1apid,
        s1ap_gb_context_t *p_s1ap_gb_ctx);
/*SPR 21220 Stop*/

extern void fill_and_send_error_ind_msg(mme_context_t     *p_mme_context,
                        U8                   ue_id_pair_presence,
                        U32             enb_ue_s1ap_id,            
                        U32             mme_ue_s1ap_id,
                        U32             s1ap_cause,
                        U16            *p_stream_id
                        );
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
/*****************************************************************************
 * Function Name  : s1ap_mme_handle_mme_msg_type
 * Inputs         : p_s1ap_gb_context - Pointer to S1AP global context data
 *                  pp_s1ap_context - Pointer to ue_s1ap_context_t
 *                  p_mme_context - Pointer to mme_context_t
 *                  p_asn_msg 
 *                  asn_msg_len
 *                  stream_id
 *                  mme_id
 *                  release_of_mme
 * Outputs        : 
 * Returns        : none
 * Description    : This function handles the MME MESSAGE message_type
 ****************************************************************************/
static void s1ap_mme_handle_mme_msg_type(
        s1ap_gb_context_t  *p_s1ap_gb_context,
        ue_s1ap_context_t  *pp_s1ap_context,
        mme_context_t      *p_mme_context,
        void *p_asn_msg,
        U16  asn_msg_len,
        U16  stream_id,
        U8   mme_id,
        U8   release_of_mme)
{
    /* CID: 113474 Start */
    U32 ue_id  = RRC_NULL;          /* id of UE in RRC */
    /* CID: 113474 End */
    U8 cell_id = MAX_NUM_CELLS;        /* id to identify a cell in enB*/
    s1ap_message_type_et   message_type;
    s1ap_ProcedureCode procedureCode = INVALID_S1AP_PROCEDURE_CODE;
    U8 ue_id_pair_presence = RRC_NULL;
    s1ap_EP_et  msg_type = INVALID_S1AP_EP;    /* type of msg received from MME */
    s1ap_ENB_UE_S1AP_ID enb_ue_s1ap_id = MAX_ENB_UE_S1AP_ID; 
    /* id to identify ue over s1 interface in enB */
    s1ap_MME_UE_S1AP_ID mme_ue_s1ap_id = MAX_MME_UE_S1AP_ID; 
    /* id to identify ue over s1 interface in MME */
    rrc_return_et response = RRC_FAILURE;    
    RRC_S1AP_UT_TRACE_ENTER();

    /* Call the asn decode function to decode the msg_type,
       enb_ue_s1ap_id,mme_ue_s1ap_id together */
    response = s1ap_partially_decode_asn_msg (
            p_asn_msg,
            asn_msg_len,
            &msg_type,
            &mme_ue_s1ap_id,
            &enb_ue_s1ap_id,
            &ue_id_pair_presence,
            &procedureCode,
            &message_type);

    if (RRC_FAILURE == response)
    {
        /* Transfer Syntax Error */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN " 
                "decode failed.",__FUNCTION__);
        /* SPR 2793 Start */
        s1ap_build_and_send_err_indication(p_mme_context, PNULL, PNULL);
        return;
    }

    switch (msg_type)
    {
        case ASN_MSG_TO_FORWARD:
            {
                if (ue_id_pair_presence & ENB_UE_S1AP_ID_PRESENT)
                {

                    /* Get ue id from enb_ue_s1ap_id */
                    ue_id = s1ap_get_ue_id(enb_ue_s1ap_id);
                    
                    SET_UE_INDEX(ue_id);

                    if (ue_id >= MAX_NUM_UE)
                    {
                        RRC_S1AP_TRACE(RRC_WARNING,
                                "UE id is greater than max range");
                        return;
                    }


                    get_ue_ctx_from_ue_id(p_s1ap_gb_context,
                            &pp_s1ap_context, enb_ue_s1ap_id);
                    if ( pp_s1ap_context  != PNULL )
                    {
                        /* Get cell id from enb_ue_s1ap_id */
                        cell_id = s1ap_get_cell_id(p_s1ap_gb_context, enb_ue_s1ap_id);

                        SET_CELL_INDEX(cell_id);

                        /*SPR_21818_START*/
                        RRC_S1AP_TRACE(RRC_INFO,
                                "[%s]:[Cell id:%d] [UE:%d] [enb_ue_s1ap_id:%u] [mme_ue_s1ap_id:%u]",
                                __FUNCTION__,cell_id, pp_s1ap_context->ue_index, enb_ue_s1ap_id, mme_ue_s1ap_id);
                        /*SPR_21818_END*/
                        if ((cell_id >= MAX_NUM_CELLS) || 
                                (!(p_s1ap_gb_context->
                                   cell_s1ap_contexts[cell_id])))
                        {
                            RRC_S1AP_TRACE(RRC_WARNING,
                                    "Cell id is invalid\n");
                            return;
                        }
                    }
                }

                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: DownlinkNASTransport [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_dl_nas_msg(
                        p_s1ap_gb_context,
                        &pp_s1ap_context,
                        enb_ue_s1ap_id,
                        mme_ue_s1ap_id,
                        p_mme_context,
                        p_asn_msg,
                        asn_msg_len,
                        ue_id_pair_presence,
                        stream_id,
                        procedureCode,
                        message_type);
                break;
            }

            /*REL 1.2:HANDOVER PROCEDURE START*/                    
        case HO_RESOURCE_ALLOCATION:
            {
                /* Create the ho_ongoing_ue_ctx_list_node in 
                 * ho_ongoing_ue_ctx_list.*/
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: HandoverRequest [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_ho_request(
                        p_s1ap_gb_context,
                        mme_ue_s1ap_id,
                        stream_id,
                        mme_id,
                        release_of_mme,
                        p_asn_msg,
                        asn_msg_len);
                break;
            }
            /*REL 1.2:HANDOVER PROCEDURE END*/                     

        case S1_SETUP_RESPONSE:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: S1SetupResponse [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                mme_fsm_process_event (
                        MME_EVENT_S1_SETUP_RSP,
                        p_asn_msg,
                        &asn_msg_len,
                        p_s1ap_gb_context,
                        p_mme_context);
                break;
            }

        case S1_SETUP_FAILURE:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: S1SetupFailure [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                mme_fsm_process_event (
                        MME_EVENT_S1_SETUP_FAILURE,
                        p_asn_msg,
                        &asn_msg_len,
                        p_s1ap_gb_context,
                        p_mme_context);
                break;
            }

        case RESET:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: Reset [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_mme_initiated_reset(
                        p_s1ap_gb_context,
                        p_mme_context,
                        p_asn_msg,
                        asn_msg_len);
                break;
            }

        case RESET_ACK:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: ResetAcknowledge [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_reset_ack(
                        p_asn_msg,
                        p_mme_context,
                        p_s1ap_gb_context,
                        asn_msg_len);
                break;
            }

        case S1_PAGING:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: Paging [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_paging_proc(
                        p_mme_context,
                        p_asn_msg,
                        asn_msg_len);
                break;
            }

        case MME_OVERLOAD_START:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: OverloadStart [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_overload_start(
                        p_mme_context,
                        mme_id,
                        p_s1ap_gb_context,
                        p_asn_msg,
                        asn_msg_len);
                break;
            }

        case MME_OVERLOAD_STOP:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: OverloadStop [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_overload_stop(
                        p_mme_context,
                        mme_id,
                        p_s1ap_gb_context,
                        p_asn_msg,
                        asn_msg_len);

                break;
            }

        case eNB_CONFIGURATION_UPDATE_ACK:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: ENBConfigurationUpdateAcknowledge [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_enb_config_update_ack(mme_id,
                        p_s1ap_gb_context,
                        p_asn_msg,
                        asn_msg_len);
                break;
            }

        case eNB_CONFIGURATION_UPDATE_FAILURE:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: ENBConfigurationUpdateFailure [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_enb_config_update_failure(mme_id,
                        p_s1ap_gb_context,
                        p_asn_msg,
                        asn_msg_len);
                break;
            }

        case MME_CONFIGURATION_UPDATE:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: MMEConfigurationUpdate [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_mme_configuration_update(
                        p_mme_context,
                        (void*)p_asn_msg,
                        asn_msg_len);
                break;
            }

        case LOCATION_REPORTING_CONTROL:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: LocationReportingControl [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_location_reporting_control(
                        mme_id,
                        p_asn_msg,
                        asn_msg_len,
                        enb_ue_s1ap_id,
                        p_s1ap_gb_context,
                        ue_id_pair_presence,
                        mme_ue_s1ap_id,
                        p_mme_context,
                        stream_id);
                break;
            }

        case MME_DIRECT_INFO_TRANSFER:
            {    
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: MMEDirectInformationTransfer [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_mme_direct_info_transfer(
                        /*SPR 19183 START*/ 
                        p_s1ap_gb_context,
                        /*SPR 19183 END*/
                        p_mme_context,
                        mme_id,
                        (void*)p_asn_msg,
                        asn_msg_len);
                break;
            }

        case MME_CONFIGURATION_TRANSFER:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: MMEDirectInformationTransfer [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_mme_configuration_transfer(
                        p_mme_context,                                
                        mme_id,
                        p_s1ap_gb_context,
                        (void*)p_asn_msg);
                break;
            }

        case ERROR_INDICATION:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: ErrorIndication [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_mme_error_indication(
                        p_s1ap_gb_context,
                        mme_id,
                        p_asn_msg,
                        asn_msg_len);
                break;
            }
        case WRITE_REPLACE_WARNING:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: WriteReplaceWarningRequest [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_write_replace_warning_request(
                        mme_id,
                        p_s1ap_gb_context,
                        p_asn_msg,
                        asn_msg_len);

                break;
            }

        case KILL_REQUEST: 
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: KillRequest [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_kill_request(
                        mme_id,
                        p_s1ap_gb_context,
                        p_asn_msg,
                        asn_msg_len);

                break;

            } 
            /*LLPA_start*/
        case LPPA_UE_ASSO_DL_TRANSPORT:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: DownlinkUEAssociatedLPPaTransport [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_dl_ue_asso_lppa_transport(mme_id,
                        p_s1ap_gb_context,
                        p_asn_msg,
                        asn_msg_len);

                break;
            }

        case LPPA_NON_UE_ASSO_DL_TRANSPORT:
            {
                RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: DownlinkNonUEAssociatedLPPaTransport [MME-S1AP-Id:%d eNB-S1AP-Id:%d] %s", mme_ue_s1ap_id, enb_ue_s1ap_id, change_data_to_str(asn_msg_len, p_asn_msg));
                s1ap_process_dl_non_ue_asso_lppa_transport(mme_id,
                        p_s1ap_gb_context,
                        p_asn_msg,
                        asn_msg_len);
                break;
            }
            /*LLPA_end*/

        default:
            {
                RRC_S1AP_TRACE(
                        RRC_ERROR,
                        "Processing MME Messages :"
                        " Unknown Message Type %d", msg_type);
            }
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_mme_messages
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_context - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function is called by s1ap message handler upon 
 *                  the reception of MME messages to process them, which
 *                  then calls the internal procedures to process the 
 *                  received messages.
 ****************************************************************************/
void s1ap_process_mme_messages(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context)
{
    void *p_asn_msg = PNULL;          /* ptr to asn msg */
//#ifndef RRC_UNIT_TEST_FRAMEWORK
    void *p_node    = PNULL;            /* ptr to node returned */
//#endif
    ue_s1ap_context_t *pp_s1ap_context    = PNULL;  /* UE context in s1ap */
    mme_context_t *p_mme_context          = PNULL;  /* mme context at s1ap */
    /* Precondition - p_api should point to right message */

//#ifndef RRC_UNIT_TEST_FRAMEWORK
    U32 sctp_sd = RRC_NULL;         /* Socket descriptor allocated on 
                                       opening connection with MME*/
//#endif
    U8 mme_id  = MAX_NUM_MME;          /* id of mme per mme context */

    /*REL 1.2:HANDOVER PROCEDURE START*/
    /* id of stream in an SCTP association between eNB and MME.*/
    U16 stream_id  = MAX_NUM_SCTP_STREAM;  
    /*REL 1.2:HANDOVER PROCEDURE END*/

    U16 asn_msg_len = 0;              /* length of the asn decoded msg */
    U8      release_of_mme  = RRC_NULL;  /* Release Info of MME */
    RRC_S1AP_UT_TRACE_ENTER();

    if (rrc_get_api_buf_size((U8*)p_api) < RRC_API_HEADER_SIZE + 
            SOCKET_DESC_SIZE)
    {
        RRC_S1AP_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }
//#ifndef RRC_UNIT_TEST_FRAMEWORK
    /* Get sctp socket descriptor */
    sctp_sd = s1ap_get_sctp_sd((U32*)((U8*)p_api + RRC_API_HEADER_SIZE));
//#endif
    /*REL 1.2:HANDOVER PROCEDURE START*/
    /* get stream id */
    stream_id = s1ap_get_stream_id((U16*)((U8*)p_api + RRC_API_HEADER_SIZE +
                SCTP_SD_OCTET_SIZE));

    /* Get the ASN message */
    p_asn_msg = (void*)((U8*) p_api + RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE +
            SCTP_STREAM_ID_SIZE);

    /* Get the asn msg length */
    asn_msg_len = (U16)(rrc_get_api_buf_size((U8*)p_api) - 
            ( RRC_API_HEADER_SIZE + SOCKET_DESC_SIZE + SCTP_STREAM_ID_SIZE));
    /*REL 1.2:HANDOVER PROCEDURE END*/

//#ifndef RRC_UNIT_TEST_FRAMEWORK
    /* Get mme_id from the search tree whose key value is the sd allocated 
       while opening connection with MME */
    p_node = st_find_tree_node( 
            &(p_s1ap_gb_context->sctp_socket_desc_search_tree),
            &sctp_sd);
    if (PNULL != p_node)
    {
        mme_id = ((sctp_sd_search_node_t *)p_node)->mmeId;
        RRC_S1AP_TRACE(
                RRC_INFO,
                "MMEId[%d] found sctp sd [%d] stream_id[%d]", mme_id, sctp_sd, stream_id);
    }
    else
    {
        RRC_S1AP_TRACE(
                RRC_WARNING,
                "[%s] No node found for the sctp sd provided %d node count %d "
                " tree address [0x%p]",
                __FUNCTION__,
                sctp_sd,
                st_get_node_count(&(p_s1ap_gb_context->
                        sctp_socket_desc_search_tree)),
                &(p_s1ap_gb_context->sctp_socket_desc_search_tree));
        return;
    }            
//#else
//    mme_id = 0;
//#endif

    /* Get the MME context from which the message is received */
    p_mme_context = p_s1ap_gb_context->mme_contexts[mme_id];
    release_of_mme = p_mme_context->connection_info.rel_of_mme;


    switch (rrc_get_api_id((U8*)p_api))
    {
        case MME_MESSAGE:
            {
                s1ap_mme_handle_mme_msg_type(p_s1ap_gb_context, pp_s1ap_context, 
                                             p_mme_context, p_asn_msg, asn_msg_len, 
                                             stream_id, mme_id, release_of_mme );
                break;
            }

        case SCTP_CONN_FAILURE_IND:
            {
                RRC_S1AP_TRACE(
                        RRC_INFO,
                        "Processing SCTP_CONN_FAILURE_IND");
                mme_fsm_process_event (
                        MME_EVENT_SCTP_CONN_FAILURE_IND,/*coverity fix start*/
                        p_api,
                        p_api,/*coverity fix stop*/
                        p_s1ap_gb_context,
                        p_mme_context);
                break;
            }

        case SCTP_CONN_RECOVERY_IND:
            {
                RRC_S1AP_TRACE(
                        RRC_INFO,
                        "Processing SCTP_CONN_RECOVERY_IND");
                mme_fsm_process_event (
                        MME_EVENT_SCTP_CONN_RECOVERY_IND,/*coverity fix start*/
                        p_api,
                        p_api,/*coverity fix stop*/
                        p_s1ap_gb_context,
                        p_mme_context);
                break;
            }

        case SCTP_CONN_SHUTDOWN_EVENT:
            {
                RRC_S1AP_TRACE(
                        RRC_INFO,
                        "Processing SCTP_CONN_SHUTDOWN_EVENT");
                mme_fsm_process_event (
                        MME_EVENT_SCTP_SHUTDOWN,/*coverity fix start*/
                        p_api,
                        p_api,/*coverity fix stop*/
                        p_s1ap_gb_context,
                        p_mme_context);
                break;
            }
/*Bug 9050 fixes start*/
        case SCTP_ASSOC_UP_EVENT:
            {
                RRC_S1AP_TRACE(
                        RRC_INFO,
                        "Processing SCTP_ASSOC_UP_EVENT");
                mme_fsm_process_event (
                        MME_EVENT_SCTP_ASSOC_UP,
                        p_api,
                        p_api,
                        p_s1ap_gb_context,
                        p_mme_context);
                break;
            }
        case SCTP_ASSOC_DOWN_EVENT:
            {
                RRC_S1AP_TRACE(
                        RRC_INFO,
                        "Processing SCTP_ASSOC_DOWN_EVENT");
                mme_fsm_process_event (
                        MME_EVENT_SCTP_ASSOC_DOWN,
                        p_api,
                        p_api,
                        p_s1ap_gb_context,
                        p_mme_context);
                break;
            }
/*Bug 9050 fixes end*/
        default:
            {
                RRC_S1AP_TRACE(RRC_ERROR, "Processing MME and SCTP Messages :"
                        " Unknown Message Type");
                break;
            }
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : s1ap_process_mme_initiated_reset
 * Inputs         : p_s1ap_gb_ctx - Pointer to S1AP global context.
 *                  p_mme_context - Pointer to MME context.
 *                  p_asn_msg - Pointer to RESET Asn message buffer.
 *                  asn_msg_len - Length of the asm Message received.
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles MME initiaited RESET.
 *****************************************************************************/
void s1ap_process_mme_initiated_reset(s1ap_gb_context_t *p_s1ap_gb_ctx,
        mme_context_t *p_mme_context,
        U8 * p_asn_msg,
        U16  asn_msg_len)
{
    reset_req_t reset_req;

    rrc_s1ap_error_indication_t error_indication;
    s1ap_error_ind_bool_t send_err_ind;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT((PNULL != p_mme_context) || (PNULL != p_asn_msg) ||
            (PNULL != p_s1ap_gb_ctx));

    memset_wrapper((void*)&reset_req,0,sizeof(reset_req_t));
    memset_wrapper(&error_indication, 0, sizeof(rrc_s1ap_error_indication_t));

    if (RRC_FAILURE == parse_asn_reset(
                p_s1ap_gb_ctx,
                p_mme_context,
                p_asn_msg,
                asn_msg_len,
                &reset_req,
                &error_indication,
                &send_err_ind))
    {
        RRC_S1AP_TRACE(RRC_WARNING, 
                "parse_reset returned failure");
        return;
    }

    if (!reset_req.reset_info.num_of_items)
    {
        RRC_S1AP_TRACE(RRC_INFO, "%s: "
                "RESET REQ: S1 INTERFACE\n",__FUNCTION__);
        /* Case : S1 Interface ( Reset All)
         */
        process_mme_initiated_reset_all_proc(p_s1ap_gb_ctx,
                p_mme_context,
                &reset_req.reset_info,
                &error_indication,
                &send_err_ind);
        /*CSR 51442 Start*/
        ALARM_MSG_L3(RRC_MODULE_ID, S1AP_RESET_ALARM_ID, MAJOR_ALARM, INVALID_ERR_CAUSE_ID);
        /*CSR 51442 Stop*/
    }
    else
    {
        RRC_S1AP_TRACE(RRC_INFO, "%s: "
                "RESET REQ: PART OF S1 INTERFACE\n",__FUNCTION__);
        /* Case : Part of S1 Interface ( Partial Reset )
         */
        process_mme_initiated_reset_part(p_s1ap_gb_ctx,
                p_mme_context,
                &reset_req.reset_info,
                &error_indication,
                &send_err_ind);
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_mme_initiated_reset_part
 * Inputs         : p_s1ap_gb_ctx - Pointer to S1AP global context.
 *                  p_mme_context - Pointer to MME context.
 *                  p_reset_info - This contains information decoded from the
 *                                 Asn message.
 *                  p_err_ind - pointer to rrc_s1ap_error_indication_t
 *                  p_send_err_ind - pointer to s1ap_error_ind_bool_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles MME initiaited part of S1 
 *                  Interface RESET.
 *****************************************************************************/
void process_mme_initiated_reset_part(s1ap_gb_context_t *p_s1ap_gb_ctx,
        mme_context_t *p_mme_context,
        s1ap_reset_info_t *p_reset_info,
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_error_ind_bool_t *p_send_err_ind)
{
    ue_s1ap_id_pair_t *id_pair_list = p_reset_info->id_pair;
    mmeUeS1apId_search_node_t * p_mmeUeS1apId_st_node = PNULL;
    
    /*REL 1.2:HANDOVER PROCEDURE START*/
    ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL;
    /*REL 1.2:HANDOVER PROCEDURE END*/

    static s1ap_reset_msg_ind_t cell_specific_reset_ind[MAX_NUM_CELLS];
    U16 id_pair_count = 0;
    U8  cell_index = 0;
    U16 asn_msg_len  = 0;
    U8 if_found = 0;
    U8 asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN_6K];
    rrc_bool_t mme_ue_s1ap_id_present = RRC_FALSE;
    U32 cell_id = RRC_NULL;
    U32 ue_id = RRC_NULL;
    s1ap_ENB_UE_S1AP_ID enbUeS1apId = MAX_NUM_UE;         
    ue_s1ap_context_t   *p_s1ap_ue_context = PNULL;
    /* Id to identify ue over s1 interface in enB */
    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper((void*)&cell_specific_reset_ind,0,
            sizeof(s1ap_reset_msg_ind_t) * MAX_NUM_CELLS);

    /* Iterate through the Ue Id Pair list and distribute them into
     * cell_specific_reset_ind array depending upon their cell id
     */
    for (id_pair_count = 0; id_pair_count < p_reset_info->num_of_items;
            id_pair_count++)
    {
        mme_ue_s1ap_id_present = RRC_FALSE;

        if (id_pair_list[id_pair_count].bitmask & ENB_UE_S1AP_ID_PRESENT)
        {
            enbUeS1apId = id_pair_list[id_pair_count].enbUeS1apId;
            cell_id = s1ap_get_cell_id(p_s1ap_gb_ctx, enbUeS1apId);
            ue_id =  s1ap_get_ue_id(enbUeS1apId);

        }
        else if (id_pair_list[id_pair_count].bitmask & MME_UE_S1AP_ID_PRESENT)
        { 
            p_mmeUeS1apId_st_node = (mmeUeS1apId_search_node_t *)
             st_find_tree_node(&p_mme_context->mmeUeS1apId_search_tree,
                    &id_pair_list[id_pair_count].mmeUeS1apId);
            
            if (!p_mmeUeS1apId_st_node)
            {
                /*REL 1.2:HANDOVER PROCEDURE START*/
                /*RRC_S1AP_TRACE(RRC_WARNING, "%s:"\*/
                RRC_S1AP_TRACE(RRC_INFO, "[%s]: "
                        "enbUeS1apId is not preset in the search tree",
                        __FUNCTION__);
                p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
                    list_first_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list);

                while(p_ho_ongoing_ue_ctx_node)
                {

                    if ( (p_mme_context->mme_id ==
                        p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mme_id) &&
                        id_pair_list[id_pair_count].mmeUeS1apId ==
                        p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mmeUeS1apId)
                    {
                        /* Match found!! */
                        RRC_S1AP_TRACE(RRC_INFO,"[%s] HO ONGOING UE CTX NODE " 
                                "Match found\n",__FUNCTION__);
                        /*Delete ho_ongoing_ue_ctx from the
                         * ho_ongoing_ue_ctx_list */
                        list_delete_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list,
                                (void *)p_ho_ongoing_ue_ctx_node);

                        if (PNULL != p_ho_ongoing_ue_ctx_node)
                        {
                            /*SPR_21818_START*/
                            get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                                    &p_s1ap_ue_context,
                                    p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.enb_ue_s1ap_id);
                            if (PNULL != p_s1ap_ue_context)
                            {
                                /* SPR 22789 Fix Start */
                                RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx,p_s1ap_ue_context,
                                        p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.enb_ue_s1ap_id);
                                /* SPR 22789 Fix End */
                            }
                            /*SPR_21818_END*/
                            /*BUG 604 changes start*/
                            /*Free s1ap_ue_id Ctx*/
                            s1ap_ueid_free(p_s1ap_gb_ctx,p_ho_ongoing_ue_ctx_node->
                                    ho_ongoing_ue_ctx.enb_ue_s1ap_id);
                            /*BUG 604 changes stop*/
                            rrc_mem_free((void*)p_ho_ongoing_ue_ctx_node);
                            p_ho_ongoing_ue_ctx_node = PNULL;
                        }

                        RRC_S1AP_TRACE(RRC_INFO,"[%s] HO ONGOING UE Context DELETED",
                            __FUNCTION__);

                        cell_specific_reset_ind[0].bitmask |= 
                            S1AP_RESET_MSG_IND_MME_UE_S1AP_ID_LIST_PRESENT;

                        cell_specific_reset_ind[0].
                            mme_ue_s1ap_id_list.mme_ue_s1ap_id[
                            cell_specific_reset_ind[0].
                            mme_ue_s1ap_id_list.count++] =
                            id_pair_list[id_pair_count].mmeUeS1apId;
                        if_found = 1;
                        break;
                    }

                    p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
                        list_next_node(&p_ho_ongoing_ue_ctx_node->node);
                }
                /*REL 1.2:HANDOVER PROCEDURE END*/
                if (if_found == 1)
                {
                    if_found = 0;
                    continue;
                }
            }
            else 
            {
                RRC_S1AP_TRACE(RRC_INFO, "[%s]: "
                    "p_mmeUeS1apId_st_node is Present",
                    __FUNCTION__);
                mme_ue_s1ap_id_present = RRC_TRUE;

                enbUeS1apId = p_mmeUeS1apId_st_node->enbUeS1apId;
                cell_id = s1ap_get_cell_id(p_s1ap_gb_ctx, enbUeS1apId);
                ue_id =  s1ap_get_ue_id(enbUeS1apId);
            }

        }
        else
        {
            /*RRC_S1AP_TRACE(RRC_WARNING, "%s:"\*/
            RRC_S1AP_TRACE(RRC_INFO, "[%s]: "
                    "Neither enbUeS1apId or mmeUeS1apId is valid",
                    __FUNCTION__);
            continue;
        }

        if (cell_id >= MAX_NUM_CELLS || ue_id >= MAX_NUM_UE)
        {
            /*RRC_S1AP_TRACE(RRC_WARNING, "%s:"\*/
            RRC_S1AP_TRACE(RRC_INFO, "[%s]: "
                    "Either cell id %d or ue id %d invalid",
                    __FUNCTION__,cell_id,ue_id);
            continue;
        }

        if (!p_s1ap_gb_ctx->cell_s1ap_contexts[cell_id])
        {
            /*RRC_S1AP_TRACE(RRC_WARNING, "%s:"\*/
            RRC_S1AP_TRACE(RRC_INFO, "[%s]: "
                    "Cell context doesn't exist at id %d",
                     __FUNCTION__,cell_id);
            continue;
        }

        get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                &p_s1ap_ue_context,
                enbUeS1apId);

        if (!p_s1ap_ue_context)
        {
            /*RRC_S1AP_TRACE(RRC_WARNING, "%s:"\*/
            RRC_S1AP_TRACE(RRC_INFO, "[%s]: "
                    "UE context doesn't exist at id %d",__FUNCTION__,ue_id);
            continue;
        }

        if (mme_ue_s1ap_id_present == RRC_FALSE)
        {
            p_mmeUeS1apId_st_node = (mmeUeS1apId_search_node_t *)
            st_find_tree_node(
                    &p_mme_context->mmeUeS1apId_search_tree,
                    &(p_s1ap_ue_context->mme_ue_s1ap_id));

            if (!p_mmeUeS1apId_st_node)
            {
                RRC_S1AP_TRACE(RRC_INFO, "[%s]: "
                        "mmeUeS1apId is not present in the search tree",
                        __FUNCTION__);
                continue;
            }
        }

        /*SPR 15427 Fix Start*/
        /*Code Removed*/
        /*SPR 15427 Fix Stop*/

        st_delete_node(&p_mme_context->mmeUeS1apId_search_tree,
                (search_tree_node_t*)p_mmeUeS1apId_st_node);

        if (PNULL != p_mmeUeS1apId_st_node)
        {
            rrc_mem_free(p_mmeUeS1apId_st_node);
            p_mmeUeS1apId_st_node = PNULL;
        }
        /*BUG 604 changes start*/
        cell_specific_reset_ind[cell_id].ue_index_list.ue_index[
            cell_specific_reset_ind[cell_id].ue_index_list.count++] =
            p_s1ap_ue_context->ue_index;
        /*BUG 604 changes stop*/

        /*BUG 604 changes start*/
        if (PNULL != p_s1ap_ue_context)
        {
            /* SPR 22789 Fix Start */
            /*BUG 604 changes start*/
            /*Free s1ap_ue_id Ctx*/
            s1ap_ueid_free(p_s1ap_gb_ctx, p_s1ap_ue_context->enb_ue_s1ap_id);
            /*BUG 604 changes stop*/
            RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx, p_s1ap_ue_context, p_s1ap_ue_context->enb_ue_s1ap_id);
            /* Code Deleted */
            /* SPR 22789 Fix End */
        }
        /*BUG 604 changes stop*/
    }

    /* For each Cell if atleast one enbUeS1apId corresponding to that
     * cell is present , then send S1AP_RESET_MSG_IND message 
     */
    for (cell_index = 0; cell_index < MAX_NUM_CELLS; cell_index++)
    {
/*BUG 604 changes start*/
        if (cell_specific_reset_ind[cell_index].ue_index_list.count)
        {
            cell_specific_reset_ind[cell_index].bitmask |= 
                S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT;
/*BUG 604 changes stop*/
        }
        if (cell_specific_reset_ind[cell_index].mme_ue_s1ap_id_list.count)
        {
            cell_specific_reset_ind[cell_index].bitmask |= 
                S1AP_RESET_MSG_IND_MME_UE_S1AP_ID_LIST_PRESENT;
        }
/*BUG 604 changes start*/
        if(S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT == 
            cell_specific_reset_ind[cell_index].bitmask ||
           S1AP_RESET_MSG_IND_MME_UE_S1AP_ID_LIST_PRESENT ==
            cell_specific_reset_ind[cell_index].bitmask)
/*BUG 604 changes stop*/
        {      
            s1ap_build_and_send_reset_ind_msg( 
                    &cell_specific_reset_ind[cell_index],RRC_UECCMD_MODULE_ID);
        }
    }

    /* Compose RESET ACK Asn message and send to the MME
     */
    if (RRC_SUCCESS == compose_asn_reset_ack(p_reset_info,
                asn_msg_buff,
                &asn_msg_len,
                p_err_ind,
                p_send_err_ind))
    {
        if (S1AP_SCTP_ERROR == l3_sctp_send(
                    p_mme_context->sctp_sd,
                    asn_msg_buff,
                    asn_msg_len,
                    p_mme_context->stream_info.stream_ids
                    [SCTP_COMMON_PROC_STREAMS]))
        {

            /*RRC_S1AP_TRACE(RRC_WARNING, "%s:"\*/
            RRC_S1AP_TRACE(RRC_INFO, "[%s]: "
                    "l3_sctp_send returned failure ",__FUNCTION__);
        }
        else
        {
            RRC_S1AP_TRACE(RRC_INFO, "[%s]: "
                    "Reset Ack sent to MME ",__FUNCTION__);
        }
    }
    else
    {
        /*RRC_S1AP_TRACE(RRC_WARNING, "%s:"\*/
        RRC_S1AP_TRACE(RRC_INFO, "[%s]: "
                "compose_asn_reset_ack returned failure",__FUNCTION__);
    }

    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : s1ap_process_reset_ack
 * Inputs         : p_asn_msg - pointer to received ASN message
 *                  p_mme_context - pointer to MME context 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *                  asn_msg_len - length of received ASN message  
 * Outputs        : None 
 * Returns        : None
 * Description    : The function is called upon reception of RESET_ACK from
 *                  a MME.
 *****************************************************************************/
void s1ap_process_reset_ack(void  *p_asn_msg,      
        mme_context_t   *p_mme_context,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        U16 asn_msg_len)
{
    reset_context_node_t * p_reset_context_node = PNULL;
    reset_context_node_t * p_reset_context_next_node = PNULL;
    reset_req_t  reset_req ;

    RRC_S1AP_UT_TRACE_ENTER();


    memset_wrapper((void*)&reset_req,RRC_NULL,sizeof(reset_req_t));

    if (RRC_FAILURE == parse_asn_reset_ack (p_mme_context,
                p_asn_msg,
                asn_msg_len,
                &reset_req.reset_info))
    {
        RRC_S1AP_TRACE(RRC_WARNING, "s1ap_process_reset_ack: "
                "parse_reset_ack returned failure");
    }

    p_reset_context_next_node = (reset_context_node_t *)
    list_first_node(&p_s1ap_gb_ctx->
            reset_context_list);

    RRC_S1AP_TRACE(RRC_INFO, "%s RESET CONTEXT LIST count %d : ",
                    __FUNCTION__,
                    list_count(&p_s1ap_gb_ctx->reset_context_list));

    while(p_reset_context_next_node)
    {
        p_reset_context_node = p_reset_context_next_node;

        if ( (p_reset_context_node->reset_context.mme_reset_info
                     [p_mme_context->mme_id])
                && (RRC_SUCCESS == compare_reset_info(&reset_req.reset_info,
                        &(p_reset_context_node->reset_context.
                           mme_reset_info[p_mme_context->mme_id]->reset_info))))
        {
            /* Match found!! */
            RRC_S1AP_TRACE(RRC_INFO,"%s RESET ACK "
                    "Match found\n",__FUNCTION__);

            /* Delete S1AP UE Contexts related to received 
               RESET ACK */
            if (!p_reset_context_node->reset_context.is_s1ap_ctx_deleted)
            {
                delete_reset_ue_s1ap_ctx(p_s1ap_gb_ctx,
                        &reset_req,
                        p_mme_context->mme_id,
                        p_reset_context_node->reset_context.is_ue_ctx_deleted);
            }

            /* Decrement Number of RESET ACKs expected */
            p_reset_context_node->reset_context.mme_count--;

            if (PNULL != p_reset_context_node->reset_context.
                    mme_reset_info[p_mme_context->mme_id])
            {
                rrc_mem_free(p_reset_context_node->reset_context.
                        mme_reset_info[p_mme_context->mme_id]);
                p_reset_context_node->reset_context.
                    mme_reset_info[p_mme_context->mme_id] = PNULL;
            }

            if (!p_reset_context_node->reset_context.mme_count)
            {
                RRC_S1AP_TRACE(RRC_INFO,"%s No More RESET ACKs "
                        "expected for this RESET EP\n",__FUNCTION__);
                /* If no More ACKs are expected */
                /* Stop RESET Timer */
                rrc_stop_timer(p_reset_context_node->reset_context.timer_id);
                p_reset_context_node->reset_context.timer_id = PNULL;

                if ( RRC_MAX_EXT_MODULE_ID 
                        >= p_reset_context_node->reset_context.
                        send_resp_to_module)
                {
                    switch (p_reset_context_node->reset_context.
                            send_resp_to_module)
                    {
                        case RRC_OAM_MODULE_ID:
                            s1ap_build_and_send_oam_reset_resp(p_s1ap_gb_ctx);
                            RRC_S1AP_TRACE(RRC_INFO,"%s RESPONSE Sent to "
                                    "OAM\n",__FUNCTION__);
                            break;

                        default:
                            RRC_S1AP_TRACE(RRC_WARNING,"%s Resp to be sent to "
                                    "UNKNOWN MODULE\n",__FUNCTION__);
                    }
                }
                /* Delete reset_context from the reset_context_list */
                list_delete_node(&p_s1ap_gb_ctx->reset_context_list,
                        (void *)p_reset_context_node);

                if (PNULL != p_reset_context_node)
                {
                    rrc_mem_free((void*)p_reset_context_node);
                    RRC_S1AP_TRACE(RRC_INFO,"%s RESET Context DELETED"
                            "\n",__FUNCTION__);
                    RRC_S1AP_TRACE(RRC_INFO, "%s RESET CONTEXT LIST count %d : ",
                            __FUNCTION__,
                            list_count(&p_s1ap_gb_ctx->reset_context_list));
                }
            }
            return;
        }

        p_reset_context_next_node = (reset_context_node_t *)
            list_next_node(&p_reset_context_node->node);
    }

    RRC_S1AP_TRACE(RRC_INFO, "%s RESET CONTEXT LIST count %d : ",
            __FUNCTION__,list_count(&p_s1ap_gb_ctx->reset_context_list));

    RRC_S1AP_TRACE(RRC_WARNING,"%s UKNOWN RESET ACK RECEIVED\n",
            __FUNCTION__);

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_mme_initiated_reset_all_proc 
 * Inputs         : p_s1ap_gb_context  - Pointer to the S1AP global context data
 *                  p_mme_context      - Pointer to MME context
 *                  p_reset_info - pointer to s1ap_reset_info_t
 *                  p_err_ind - pointer to rrc_s1ap_error_indication_t
 *                  p_send_err_ind - pointer to s1ap_error_ind_bool_t 
 * Outputs        : None
 * Returns        : none
 * Description    : This function is called when Reset type is RESET_ALL.
 *                  This message Prepares a list enbUeS1apId and send them to
 *                  all supported cells. It also clears the ue context from the
 *                  s1ap global context.
 ******************************************************************************/
void  process_mme_initiated_reset_all_proc( 
        /* Pointer to S1AP global context */
        s1ap_gb_context_t * p_s1ap_gb_context,
        /* Pointer to mme context */
        mme_context_t * p_mme_context,
        /* Pointer to reset info */
        s1ap_reset_info_t *p_reset_info,
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_error_ind_bool_t *p_send_err_ind)
{
    U8 asn_msg_buff[S1AP_MAX_ASN1_BUF_LEN_6K]  = {0};

    U16 asn_msg_len  = RRC_NULL;
    RRC_S1AP_UT_TRACE_ENTER();

    process_reset_all(p_s1ap_gb_context, p_mme_context);

    if (RRC_SUCCESS == compose_asn_reset_ack(p_reset_info,
                asn_msg_buff,
                &asn_msg_len,
                p_err_ind,
                p_send_err_ind))
    {
        if (MME_ACTIVE == p_mme_context->current_mme_state)
        {
            RRC_S1AP_TRACE(RRC_INFO, "%s: "
                    "Sending  RESET ACK to MME \n",__FUNCTION__);
            if (S1AP_SCTP_ERROR == l3_sctp_send(
                        p_mme_context->sctp_sd,
                        asn_msg_buff,
                        asn_msg_len,
                        p_mme_context
                        ->stream_info.stream_ids[
                        SCTP_COMMON_PROC_STREAMS]))

            {

                RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                        "l3_sctp_send returned failure ",__FUNCTION__);
            }
            else
            {
                RRC_S1AP_TRACE(RRC_INFO, "%s: "
                        "Reset Ack sent to MME ",__FUNCTION__);
            }
        }
        else
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "Cannot send reset all: MME state is not active ",
                    __FUNCTION__);
        }
    }
    else
    {
        RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                "compose_asn_reset_ack returned failure",__FUNCTION__);
    }

    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name : compare_reset_info
 * Inputs        : p_recv_reset_info  - Pointer to the reset info received in
 *                                     RESET ACK
 *                 p_reset_info      - Pointer to  reset info existing in
 *                                     reset_context
 * Outputs       : None
 * Returns       : RRC_SUCCESS - If both p_recv_reset_info and p_reset_info
 *                                 match
 * Description   : This function is called when Reset type is RESET_ALL.
 ******************************************************************************/
rrc_return_et compare_reset_info(s1ap_reset_info_t * p_recv_reset_info,
        s1ap_reset_info_t * p_reset_info)
{
    U16 ue_count = 0;

    if (p_recv_reset_info->num_of_items !=
            p_reset_info->num_of_items)
    {
        return RRC_FAILURE;
    }

    while(ue_count != p_recv_reset_info->num_of_items)
    {
        if (p_recv_reset_info->id_pair[ue_count].enbUeS1apId !=
                  p_reset_info->id_pair[ue_count].enbUeS1apId)
        {
            return RRC_FAILURE;
        }
        ue_count++;
    }

    return RRC_SUCCESS;
}

/* SPR 15199 Start */
/****************************************************************************
 * Function Name  : s1ap_handover_failure_intrl_enc
 * Inputs         : pctxt (for memory allocation),
 *                  p_ue_handover_failure (HO failure msg)
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_s1ap_pdu (S1AP pdu)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs handover failure into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et s1ap_handover_failure_intrl_enc
(
    OSCTXT                      *pctxt,         /* for memory allocation */
    U8                          *p_buff,        /* ASN.1 encoded message */
    rrc_s1ap_handover_failure_t *p_ue_handover_failure,
    S1AP_PDU                    *p_s1ap_pdu
)
{
    rrc_return_et result = RRC_FAILURE;
    s1ap_HandoverFailure *p_s1ap_msg;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_ue_handover_failure);

    do
    {
        asn1Init_S1AP_PDU(p_s1ap_pdu);
        p_s1ap_pdu->t = T_S1AP_PDU_unsuccessfulOutcome;

        p_s1ap_pdu->u.unsuccessfulOutcome =
            rtxMemAllocType(pctxt, UnsuccessfulOutcome);
        if (PNULL == p_s1ap_pdu->u.unsuccessfulOutcome)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_UnsuccessfulOutcome(p_s1ap_pdu->u.unsuccessfulOutcome);

        p_s1ap_pdu->u.unsuccessfulOutcome->procedureCode = 
                                                   ASN1V_s1ap_id_HandoverResourceAllocation;

        p_s1ap_pdu->u.unsuccessfulOutcome->criticality = s1ap_reject;

        p_s1ap_pdu->u.unsuccessfulOutcome->value.t = T1_handoverResourceAllocation;
        p_s1ap_pdu->u.unsuccessfulOutcome->value.u.handoverResourceAllocation =
            rtxMemAllocType(pctxt, s1ap_HandoverFailure);
        
        if (PNULL == p_s1ap_pdu->u.unsuccessfulOutcome->value.u.handoverResourceAllocation)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }
        
        p_s1ap_msg = p_s1ap_pdu->u.unsuccessfulOutcome->value.u.handoverResourceAllocation;

        asn1Init_s1ap_HandoverFailure(p_s1ap_msg);

        if (RT_OK != asn1Append_s1ap_HandoverFailure_protocolIEs_1(
                                pctxt,
                                &p_s1ap_msg->protocolIEs,
                                p_ue_handover_failure->mme_ue_s1ap_id))
        {
            break;
        }


        if (RT_OK != asn1Append_s1ap_HandoverFailure_protocolIEs_2(
                                pctxt,
                                &p_s1ap_msg->protocolIEs,
                                &p_ue_handover_failure->cause))
        {
            break;
        }

        if (RRC_S1AP_HANDOVER_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT & 
            p_ue_handover_failure->bitmask)
        {
            if (RT_OK != asn1Append_s1ap_HandoverFailure_protocolIEs_3(
                                   pctxt,
                                   &p_s1ap_msg->protocolIEs,
                                   &p_ue_handover_failure->criticality_diagnostics))
            {
                break;
            }
        }
        
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, p_s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding failed.");
            break;
        }

        result=RRC_SUCCESS;
    } while(0);

    RRC_S1AP_UT_TRACE_EXIT();
    return result;

}


/******************************************************************************
 *   FUNCTION NAME: uecc_s1ap_build_and_send_handover_failure
 *
 *   INPUTS       : uecc_gb_context_t   *p_uecc_gb_contex
 *                  uecc_ue_context_t  *p_ue_context
 *                  U32        mme_ue_s1ap_id
 *                  U8         mme_id
 *                  s1ap_Cause *p_cause
 *                  U16        stream_id
 *                  sctp_sd_t  sctp_sd (socket descriptor)
 *   OUTPUTS      : None
 *   DESCRIPTION:
 *       This function builds and sends s1ap_HandoverFailure message
 *       to MME 
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
rrc_return_et s1ap_build_and_send_handover_failure(
        U32        mme_ue_s1ap_id,
        /*SPR 17777 +-*/
        s1ap_Cause *p_cause,
        U16        stream_id,
        sctp_sd_t  sctp_sd
        )
{
    rrc_return_et result = RRC_FAILURE;
    OSCTXT asn1_ctx;
    rrc_s1ap_handover_failure_t msg;
    /* Coverity 93934 Fix Start */
    U8 msg_buff_p[S1AP_MAX_ASN1_BUF_LEN]={0};
    /* Coverity 93934 Fix End */
    EVENT_EXTERNAL_HEADER   *p_event_header = PNULL;
    U32                     encoded_msg_len = 0;
    S1AP_PDU s1ap_pdu;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_cause);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR, "[s1ap_HandoverFailure] "
                "ASN context initialization failed");
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {

        /* Reset message */
        memset_wrapper(&msg, RRC_NULL, sizeof(msg));

        /* Fill message */
        msg.bitmask = RRC_NULL;

        msg.mme_ue_s1ap_id = mme_ue_s1ap_id;
        msg.cause = *p_cause;


        /* Encode message */
        result = s1ap_handover_failure_intrl_enc(
                &asn1_ctx,
                msg_buff_p,
                &msg,
                &s1ap_pdu);

        if (RRC_SUCCESS!=result)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "[s1ap_HandoverFailure] "
                    "Build failure");
            break;
        }
        else
        {
            encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);

            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU : ", &s1ap_pdu);

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                 encoded_msg_len);
            if (PNULL != p_event_header)
            {
                memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                            encoded_msg_len));

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }
        /*BUG 604 changes stop*/ 

        if (SCTP_SOCKET_ERROR ==l3_sctp_send(
                    sctp_sd,
                    &msg,
                    encoded_msg_len,
                    stream_id))
        {
            RRC_S1AP_TRACE(RRC_WARNING,
                    "%s:"
                    "l3_sctp_send send failed",
                    __FUNCTION__);
        }
        RRC_S1AP_TRACE(RRC_INFO, "[S1AP PROTOCOL] HO FAILURE SENT");
    } while (0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}
/* SPR 15199 End */

/*****************************************************************************
 * Function Name  : s1ap_process_ho_request
 * Inputs         : p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *                  mme_ue_s1ap_id - id of ue over s1 interface in MME
 *                  stream_id - the stream id to be updated
 *                  mme_id - the mme Index to be updated
 *                  release_of_mme - Release Info of the MME
 *                  p_asn_msg - Pointer to asn decoded msg
 *                  asn_msg_len - Length of the asn decoded msg
 * Outputs        : None
 * Returns        : None
 * Description    : The function processes HO request.
 *****************************************************************************/
void s1ap_process_ho_request(
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        s1ap_MME_UE_S1AP_ID mme_ue_s1ap_id,
        U16 stream_id,
        U8 mme_id,
        U8 release_of_mme,
        void  *p_asn_msg,
        U16 asn_msg_len)
{
    ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL;
    void *p_intrl_msg            = PNULL;
    s1ap_ue_associated_sig_msg_ind_t    *msg_p = PNULL;
    /* SPR 15199 Start */
    s1ap_Cause          cause = {0};
    /* SPR 15199 End */

    /* SPR 2251 Start */
    mme_context_t     *p_mme_context = PNULL;
    search_tree_node_t *p_node = PNULL;
    s1ap_reset_msg_ind_t s1ap_reset_ind;
    U32 deleted_enb_ue_s1apid = 0xFFFFFFFF;
    U8 ue_id_pair_presence  = RRC_NULL;
/*BUG 604 changes start*/
    U32  enb_ue_s1ap_id = RRC_NULL;
/*BUG 604 changes stop*/
    /*SPR 21220 Start*/
    U32 ue_index;
    /*SPR 21220 Stop*/

    p_mme_context =
        p_s1ap_gb_ctx->mme_contexts[mme_id];

    /* SPR 2551 Stop */

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_asn_msg);

    /* SPR 2251 Start */
    p_node = st_find_tree_node(
            &p_mme_context->mmeUeS1apId_search_tree,
            &mme_ue_s1ap_id);

    if (PNULL != p_node)
    {
        memset_wrapper((void*)&s1ap_reset_ind,RRC_NULL,sizeof(s1ap_reset_msg_ind_t));

                /* Prepare S1AP RESET IND msg to be sent to uecc */
                /* Fill reset indication message fields */

                /*SPR 21220 Start*/
                ue_index = delete_s1ap_context( mme_ue_s1ap_id,
                        p_mme_context,
                        &deleted_enb_ue_s1apid,
                        p_s1ap_gb_ctx);

                if (ue_index != 0xFFFFFFFF)
                {
/*BUG 604 changes start*/
                    s1ap_reset_ind.bitmask |= 
                        S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT;
/*BUG 604 changes stop*/

                    RRC_S1AP_TRACE(RRC_INFO, 
                            "[%s]Deleting context for UE_INDEX %u "
                            "and adding it to RESET IND list",
                            __FUNCTION__,ue_index);
       /*BUG 604 changes start*/
                    s1ap_reset_ind.ue_index_list.ue_index[
                        s1ap_reset_ind.ue_index_list.count] =
                        ue_index;
                    s1ap_reset_ind.ue_index_list.count++;
       /*BUG 604 changes stop*/
                
                    RRC_S1AP_TRACE(RRC_ERROR, "[%s] mme Ue s1ap id INVALID"
                        " Entry in search tree found"
                        " Sending S1AP RESET IND for UE_INDEX 0x%x",
                        __FUNCTION__,ue_index);

                    /* SPR 2551 Start */
                    ue_id_pair_presence |= ENB_UE_S1AP_ID_PRESENT;
                    /* SPR 2551 Stop */
                }

                s1ap_reset_ind.bitmask |= 
                    S1AP_RESET_MSG_IND_MME_UE_S1AP_ID_LIST_PRESENT;

                s1ap_reset_ind.mme_ue_s1ap_id_list.mme_ue_s1ap_id[
                    s1ap_reset_ind.mme_ue_s1ap_id_list.count] =
                    mme_ue_s1ap_id;
                s1ap_reset_ind.mme_ue_s1ap_id_list.count++;

                s1ap_build_and_send_reset_ind_msg( 
                        &s1ap_reset_ind,RRC_UECCMD_MODULE_ID);

                    RRC_S1AP_TRACE(RRC_ERROR, "[%s] mme Ue s1ap id INVALID"
                        " Entry in search tree found"
                        " Sending S1AP RESET IND for mmeues1apid 0x%x\n",
                        __FUNCTION__,mme_ue_s1ap_id);

                /* SPR 2551 Start */
                ue_id_pair_presence |= MME_UE_S1AP_ID_PRESENT;

                fill_and_send_error_ind_msg(p_mme_context,
                        ue_id_pair_presence,
                        ue_index,
                        mme_ue_s1ap_id,
                        s1ap_unknown_mme_ue_s1ap_id,
                        &stream_id);
                
                /* SPR 2551 Stop */

                return;

    }
    /*SPR 21220 Stop*/
    /* SPR 2551 Stop */


/*BUG 604 changes start*/
    enb_ue_s1ap_id = s1ap_ueid_allocate(p_s1ap_gb_ctx);

    if(enb_ue_s1ap_id == p_s1ap_gb_ctx->total_ue_supported)
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Error enb_ue_s1ap_id"
                "is not allocated\n ",__FUNCTION__);
        /* SPR 15199 Start */
        cause.t = T_s1ap_Cause_radioNetwork;
        cause.u.radioNetwork = s1ap_no_radio_resources_available_in_target_cell;

        (void)s1ap_build_and_send_handover_failure(
                mme_ue_s1ap_id,
                /*SPR 17777 +-*/
                &cause,
                stream_id,
                p_mme_context->sctp_sd);
        /* SPR 15199 End */
        return;
    }
/*BUG 604 changes stop*/
    /* Create a ho_ongoing_ue_ctx */
    p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
        rrc_mem_get(sizeof(ho_ongoing_ue_ctx_node_t));

    if(PNULL == p_ho_ongoing_ue_ctx_node)
    {
        RRC_S1AP_TRACE(RRC_FATAL, "rrc_mem_get failed!!");
        return;
    }

    memset_wrapper(p_ho_ongoing_ue_ctx_node,0,sizeof(ho_ongoing_ue_ctx_node_t));

    /* Set mme_ue_s1ap_id */
    p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mmeUeS1apId = mme_ue_s1ap_id;

    /* Set stream_id*/
    p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.stream_id = stream_id;

    /* Set mme_id*/
    p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mme_id = mme_id;

/*BUG 604 changes start*/
    p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.enb_ue_s1ap_id = enb_ue_s1ap_id;
/*BUG 604 changes stop*/

    list_push_head(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list,
            &p_ho_ongoing_ue_ctx_node->node);

    p_intrl_msg = s1ap_alloc_intrl_msg(
            RRC_UECC_MODULE_ID,
            S1AP_UE_ASSOCIATED_SIG_MSG_IND,
            (U16)(sizeof(s1ap_ue_associated_sig_msg_ind_t) + asn_msg_len));

    if (PNULL == p_intrl_msg)
    {
        /*out of memory*/
        RRC_S1AP_TRACE(RRC_FATAL, "rrc_mem_get failed!!");

        if (PNULL != p_ho_ongoing_ue_ctx_node)
        {
            /*BUG 604 changes start*/
            /*Free s1ap_ue_id Ctx*/
            s1ap_ueid_free(p_s1ap_gb_ctx,enb_ue_s1ap_id);
            /*BUG 604 changes stop*/

            /*SPR_21818_START*/
            /*Delete ho_ongoing_ue_ctx from the ho_ongoing_ue_ctx_list */
            list_delete_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list,
                    (void *)p_ho_ongoing_ue_ctx_node);
            /*SPR_21818_END*/

            rrc_mem_free((void*)p_ho_ongoing_ue_ctx_node);
            RRC_S1AP_TRACE(RRC_INFO,"%s HO ONGOING UE Context DELETED \n",__FUNCTION__);
        }

        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    msg_p = (s1ap_ue_associated_sig_msg_ind_t *)((U8 *)p_intrl_msg +
            RRC_API_HEADER_SIZE);
    msg_p->bitmask |= UE_ASSOC_SIG_MSG_IND_MME_ID_PRESENT;
    msg_p->mme_id = mme_id;
    msg_p->bitmask |= UE_ASSOC_SIG_MSG_IND_REL_OF_MME_PRESENT;
    msg_p->rel_of_mme = release_of_mme;

    /*Filling Procedure code to be used by UECC MD */
    msg_p->s1ap_procedure_code = ASN1V_s1ap_id_HandoverResourceAllocation;
    msg_p->message_type = INITIATING_MESSAGE;
/*BUG 604 changes start*/
    /*This is the first message in case of Handover, so sending ue_index as
     * invalid*/
    msg_p->ue_index = 0xFF;
    msg_p->bitmask |= UE_ASSOC_SIG_MSG_IND_ENB_UE_S1APID_PRESENT;
    msg_p->enb_ue_s1ap_id = enb_ue_s1ap_id;

/*BUG 604 changes stop*/

    SET_UE_INDEX(msg_p->ue_index);

    l3_memcpy_wrapper((void*)((U8*)p_intrl_msg + RRC_API_HEADER_SIZE +
                sizeof(s1ap_ue_associated_sig_msg_ind_t)),
            (const void*)p_asn_msg,
            asn_msg_len);

    RRC_S1AP_TRACE(RRC_INFO, "[%s]: Forwarding message to UECC\n",
            __FUNCTION__);
    /* Send response */
    rrc_send_message(
            p_intrl_msg,
            RRC_UECCMD_MODULE_ID);

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
* Function Name  : s1ap_process_overload_start 
* Inputs         : p_mme_context - pointer to MME context 
*                  mme_id - Pointer to the mme Index to be updated
*                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
*                  p_asn_msg - Pointer to asn decoded msg
*                  asn_msg_len - Length of the asn decoded msg
* Outputs        : None
* Returns        : None
* Description    : The function processes MME Overload Start Message
*****************************************************************************/
void s1ap_process_overload_start(
    mme_context_t *p_mme_context,
    U8                   mme_id,
    s1ap_gb_context_t    *p_s1ap_gb_ctx,
    void                 *p_asn_msg,
    U16                  asn_msg_len)
{
    parsed_overload_start_t    parsed_overload_start_message;
    
    U8 counter = RRC_NULL;
    U16 num_gummei = RRC_NULL;
    
    mme_overload_response_et mme_overload_response = 
                                                  OVERLOAD_START_INVALID_VALUE;

    s1ap_gummei_t              gummei_list[MAX_NUM_MMECS];

    memset_wrapper(gummei_list,0,(sizeof(s1ap_gummei_t) * MAX_NUM_MMECS));
    
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_asn_msg);
    
    memset_wrapper((void*)&parsed_overload_start_message,0,
                          sizeof(parsed_overload_start_t));
    
    if ( RRC_FAILURE == parse_asn_overload_start(
                p_mme_context,
                p_asn_msg,
                asn_msg_len,
                &parsed_overload_start_message))
    {
        RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                "parse_overload_start returned failure", __FUNCTION__);

        return;
    }

    mme_overload_response = convert_overload_response(
                    parsed_overload_start_message.overload_action);

    if(parsed_overload_start_message.bitmask & S1AP_S1_OVERLOAD_START_GUMMEILIST_PRESENT)
    {
        num_gummei = s1ap_decode_overload_gummei_list(gummei_list,&parsed_overload_start_message.s1ap_gummelist);
    }


    if (mme_overload_response == OVERLOAD_START_INVALID_VALUE)
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "mme_overload_response is Invalid",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    /* Check if the mme_id in the active_mme_list 
     */
    for (counter = 0; counter < MAX_NUM_MME; counter++)
    {
        if (p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].mme_id == mme_id)
        {
            p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].overload_response =
                mme_overload_response;

            if(num_gummei)
            {
                p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].gummei_list_present = RRC_TRUE,
                    p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].num_gummei = num_gummei;

                l3_memcpy_wrapper(&p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].gummei_list, 
                        &gummei_list,
                        (sizeof(s1ap_gummei_t) * num_gummei));
            }

            if(parsed_overload_start_message.bitmask & S1AP_S1_OVERLOAD_START_TRAFFIC_LOAD_RED_IND_PRESENT)
            {
                p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].traffic_load_ind_present = RRC_TRUE;
                p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].traffic_load_ind = parsed_overload_start_message.s1ap_traffic_load_red_ind;
            }
            /* SPR 22753 Fix Start */
            p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].total_no_of_calls = RRC_ZERO;
            p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].total_rejected_calls = RRC_ZERO;

            RRC_S1AP_TRACE(RRC_INFO, "Overload Response[%d] traffic_load_ind_present[%d] traffic_load_ind[%d] updated for MME_ID: [%d]",
                mme_overload_response,
                    p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].traffic_load_ind_present,
                    p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].traffic_load_ind,
                mme_id);
            /* SPR 22753 Fix End */
            break;
        }
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
* Function Name  : s1ap_process_overload_stop
* Inputs         : p_mme_context - pointer to MME context 
*                  mme_id - Pointer to the mme Index to be updated
*                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
*                  p_asn_msg - Pointer to asn decoded msg
*                  asn_msg_len - Length of the asn decoded msg
* Outputs        : None
* Returns        : None
* Description    : The function processes MME Overload Stop Message
*****************************************************************************/
void s1ap_process_overload_stop(
    mme_context_t *p_mme_context,
        U8 mme_id,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len)
{
    parsed_overload_stop_t    parsed_overload_stop_message; 
    s1ap_gummei_t              gummei_list[MAX_NUM_MMECS];
    U8 counter = RRC_NULL;


    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_asn_msg);
    U8 count = RRC_NULL;
    U8 count1 = RRC_NULL;
    U8 del_gummei_count = RRC_NULL;
    U16 num_gummei = RRC_NULL;
    memset_wrapper(gummei_list,0,(sizeof(s1ap_gummei_t) * MAX_NUM_MMECS));
    memset_wrapper(&parsed_overload_stop_message, 0, (sizeof(parsed_overload_stop_t)));


    if (RRC_FAILURE == parse_asn_overload_stop(
                p_mme_context,
                p_asn_msg,
                asn_msg_len,
                &parsed_overload_stop_message))
    {
        RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                "parse_overload_start returned failure", __FUNCTION__);
        return;
    }

    /* Check if the mme_id in the active_mme_list 
    */
    if(parsed_overload_stop_message.bitmask & S1AP_S1_OVERLOAD_STOP_GUMMEILIST_PRESENT)
    {
        num_gummei = s1ap_decode_overload_gummei_list(gummei_list,&parsed_overload_stop_message.s1ap_gummelist);
    }

    for (counter = 0; counter < MAX_NUM_MME; counter++)
    {
        if (p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].mme_id == mme_id)
        {
            if(num_gummei)
            {
                for(count = 0;count < num_gummei;count++)
                {
                    for(count1 = 0;count1 < p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].num_gummei;count1++)
                    {
                        if ( !memcmp_wrapper(&p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].gummei_list[count1].plmn_identity.data,
                                    &gummei_list[count].plmn_identity.data,
                                    gummei_list[count].plmn_identity.numocts))
                        {
                            if ( !memcmp_wrapper(&p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].gummei_list[count1].grp_id.data, 
                                        &gummei_list[count].grp_id.data,gummei_list[count].grp_id.numocts))
                            {
                                if ( !memcmp_wrapper(&(p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].gummei_list[count1].mme_code.data),
                                            &gummei_list[count].mme_code.data,gummei_list[count].mme_code.numocts))
                                {
                                    memset_wrapper(&p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].gummei_list[count1],0,sizeof(s1ap_gummei_t));
                                    del_gummei_count++;
                                    break;
                                }
                            }
                        }
                    }
                }
                if(del_gummei_count == num_gummei)
                {
                    p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].overload_response =
                        OVERLOAD_START_INVALID_VALUE;
                    p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].traffic_load_ind_present =
                        RRC_FALSE;
                    p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].traffic_load_ind =
                        RRC_NULL;
                    p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].num_gummei =
                        RRC_NULL;
                    /* SPR 22753 Fix Start */
                    p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].total_no_of_calls = 
                        RRC_ZERO;
                    p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].total_rejected_calls = 
                        RRC_ZERO;
                    /* SPR 22753 Fix End */
                }
            }
            else
            {
                p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].overload_response =
                    OVERLOAD_START_INVALID_VALUE;
                p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].traffic_load_ind_present =
                    RRC_FALSE;
                p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].traffic_load_ind =
                    RRC_NULL;
                p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].num_gummei =
                    RRC_NULL;
                /* SPR 22753 Fix Start */
                p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].total_no_of_calls = 
                    RRC_ZERO;
                p_s1ap_gb_ctx->active_mme_info.mme_id_arr[counter].total_rejected_calls = 
                    RRC_ZERO;
                /* SPR 22753 Fix End */
            }
            return;
        }
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return;
}
/*****************************************************************************
* Function Name  : s1ap_process_enb_config_update_ack
* Inputs         : mme_id - Pointer to the mme Index to be updated
*                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
*                  p_asn_msg - Pointer to asn decoded msg
*                  asn_msg_len - Length of the asn decoded msg
* Outputs        : None
* Returns        : None
* Description    : The function processes eNB config update Message for 
*                  acknowledged mode
*****************************************************************************/
void s1ap_process_enb_config_update_ack(U8 mme_id,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len)
{
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn_msg);

    if (RRC_FAILURE == parse_asn_enb_config_update_ack(
                p_s1ap_gb_ctx->mme_contexts[mme_id],
                p_asn_msg,
                asn_msg_len))
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s: "
                "parse_asn_enb_config_update_ack failure",__FUNCTION__);
        return;
    }

    /* Push the event S1AP_MME_EV_ENB_CONFIG_ACK_RECV */
    enb_fsm_config_update_process_event(S1AP_MME_EV_ENB_CONFIG_ACK_RECV,/*coverity fix start*/
            p_asn_msg,
            p_asn_msg,/*coverity fix stop*/
            p_s1ap_gb_ctx,
            p_s1ap_gb_ctx->mme_contexts[mme_id]);

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
* Function Name  : s1ap_process_dl_ue_asso_lppa_transport 
* Inputs         : mme_id - Pointer to the mme Index to be updated
*                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
*                  p_asn_msg - Pointer to asn decoded msg
*                  asn_msg_len - Length of the asn decoded msg
* Outputs        : None
* Returns        : None
* Description    : The function processes DL UE Associated LPPA Transport.
*****************************************************************************/
void s1ap_process_dl_ue_asso_lppa_transport(U8 mme_id, 
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len)
{
    dl_ue_asso_lppa_trans_t ue_asso_lppa_trans;
    OSCTXT  asn1_ctx;
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn_msg);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    memset_wrapper(&ue_asso_lppa_trans, 0, sizeof(dl_ue_asso_lppa_trans_t));
    
    if(0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_UT_TRACE_EXIT();
        return;
    }

    if (RRC_FAILURE == s1ap_parse_asn_dl_ue_associated_lppa_transport(
                         &asn1_ctx,
                         p_s1ap_gb_ctx,
                         p_s1ap_gb_ctx->mme_contexts[mme_id],
                         p_asn_msg, asn_msg_len, &ue_asso_lppa_trans))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
               "s1ap_parse_asn_dl_ue_associated_lppa_transport failure");
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        return;
    }

    if (RRC_FAILURE == s1ap_decode_asn_s1ap_LPPa_PDU(p_s1ap_gb_ctx,
               p_s1ap_gb_ctx->mme_contexts[mme_id], &ue_asso_lppa_trans,
               PNULL, UE_ASSO_LPPA))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                         "s1ap_decode_asn_s1ap_LPPa_PDU failure");
    }
    /*SPR_18125_START*/
    rtFreeContext(&asn1_ctx);
    /*SPR_18125_END*/
    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
* Function Name  : s1ap_process_dl_non_ue_asso_lppa_transport 
* Inputs         : mme_id - Pointer to the mme Index to be updated
*                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
*                  p_asn_msg - Pointer to asn decoded msg
*                  asn_msg_len - Length of the asn decoded msg
* Outputs        : None
* Returns        : None
* Description    : The function processes dl non ue associated LPPA Message
*****************************************************************************/
void s1ap_process_dl_non_ue_asso_lppa_transport(U8 mme_id, 
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len)
{
    dl_non_ue_asso_lppa_trans_t  non_ue_asso_lppa_trans;
    OSCTXT  asn1_ctx;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn_msg);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    memset_wrapper(&non_ue_asso_lppa_trans, 0, sizeof(dl_non_ue_asso_lppa_trans_t));
    
    if(0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_UT_TRACE_EXIT();
        return;
    }
    
    if (RRC_FAILURE == s1ap_parse_asn_dl_non_ue_associated_lppa_transport(
                &asn1_ctx,
                /*SPR 17777 +-*/
                p_s1ap_gb_ctx->mme_contexts[mme_id],
                p_asn_msg,
                asn_msg_len,
                &non_ue_asso_lppa_trans))
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s: "
                "s1ap_parse_asn_dl_non_ue_associated_lppa_transport failure",__FUNCTION__);
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        return;
    }
    
    if (RRC_FAILURE == s1ap_decode_asn_s1ap_LPPa_PDU(
                p_s1ap_gb_ctx,
                p_s1ap_gb_ctx->mme_contexts[mme_id],
                PNULL, &non_ue_asso_lppa_trans, NON_UE_ASSO_LPPA))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                         "s1ap_decode_asn_s1ap_LPPa_PDU failure");
    }

    /*SPR_18125_START*/
    rtFreeContext(&asn1_ctx);
    /*SPR_18125_END*/
    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
* Function Name  : s1ap_process_enb_config_update_failure
* Inputs         : mme_id - Pointer to the mme Index to be updated
*                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
*                  p_asn_msg - Pointer to asn decoded msg
*                  asn_msg_len - Length of the asn decoded msg
* Outputs        : None
* Returns        : None
* Description    : The function processes eNB config update failure message
*****************************************************************************/
void s1ap_process_enb_config_update_failure(U8 mme_id,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len)
{
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn_msg);

    /* NOTE: Parsing is handled by FSM to retain
     * certain values, so that we dont have to maintain
     * the data in global context */
    enb_fsm_config_update_process_event(S1AP_MME_EV_ENB_CONFIG_NACK,
            p_asn_msg,
            &asn_msg_len,
            p_s1ap_gb_ctx,
            p_s1ap_gb_ctx->mme_contexts[mme_id]);

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
* Function Name  : s1ap_process_mme_context_update
* Inputs         : p_mme_ctx - Pointer to the S1AP mme  context data
*                  p_asn_msg - Pointer to asn decoded msg
*                  asn_msg_len - Length of the asn decoded msg
* Outputs        : None
* Returns        : None
* Description    : The function processes mme_context update.
******************************************************************************/
void s1ap_process_mme_configuration_update(
        mme_context_t* p_mme_context,
        void* p_asn_msg,
        U16 asn_msg_len)
{
    rrc_return_et result = RRC_FAILURE;
    U8 asn_message[S1AP_MAX_ASN1_BUF_LEN]= {0};
    U16 message_buff_len = 0;
    mme_configuration_update_t mme_config_update;
    rrc_s1ap_error_indication_t error_indication;
    s1ap_error_ind_bool_t        send_err_ind;
    OSCTXT asn1_ctx;
    LOCAL_MME_CONFIGURATION_UPDATE_ERROR_STRUCT *p_mme_config_update_err = PNULL;
    s1ap_rrm_mme_info_t   s1ap_rrm_mme_info;        
    s1ap_oam_mme_info_t   s1ap_oam_mme_info;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_asn_msg);
    RRC_ASSERT(RRC_NULL != asn_msg_len);
    RRC_ASSERT(PNULL != p_mme_context);

    memset_wrapper(&mme_config_update, 
            RRC_NULL, 
            sizeof(mme_configuration_update_t));
    memset_wrapper(&error_indication, RRC_NULL,
            sizeof(rrc_s1ap_error_indication_t));

    memset_wrapper(&send_err_ind, RRC_NULL,
            sizeof(s1ap_error_ind_bool_t));

    memset_wrapper(&s1ap_rrm_mme_info, RRC_NULL, sizeof(s1ap_rrm_mme_info_t));
    memset_wrapper(&s1ap_oam_mme_info, RRC_NULL, sizeof(s1ap_oam_mme_info_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return;
    }

    /* Check if the MME is in ACTIVE State */
    if (p_mme_context->current_mme_state != MME_ACTIVE)
    {
        RRC_S1AP_TRACE(RRC_WARNING, "%s:"
                "Ignoring MME configuration Update received from MME id %d. "
                "MME not in  active state.",
                __FUNCTION__,p_mme_context->mme_id);

        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        return;
    }
    
	result = parse_asn_mme_configuration_update(
            &asn1_ctx,
            p_mme_context,
            p_asn_msg,
            asn_msg_len,
            &mme_config_update,
            &error_indication, 
            &send_err_ind);

    do
    {
        if(RRC_SUCCESS == result) 
        {
            /* If result is success update the mme_context and send
             * the Acknowledgement to MME */
            /* CSR_00070383 Fix + */
            /* CSR_00070383 Fix - */
            /* Updating the MME Context */
            if (MME_CONFIGURATION_MME_NAME_PRESENT & mme_config_update.bitmask)
            {
                l3_memcpy_wrapper(p_mme_context->mme_name, 
                        mme_config_update.mme_name,
                        MAX_MME_NAME_STR);
            }

            /* CSR_00070383 Fix + */
            /* CSR_00059128 Fix + */
            if (mme_config_update.bitmask & MME_CONFIGURATION_SERVED_GUMMEI)
            {
            l3_memcpy_wrapper(p_mme_context->p_served_gummeis, 
                    mme_config_update.gummei_info.p_served_gummeis,
                    sizeof(served_gummei_info_t));
            }
            /* CSR_00059128 Fix - */
            /* CSR_00070383 Fix - */

            if (MME_CONFIGURATION_REL_MME_CAPACITY_PRESENT & 
                    mme_config_update.bitmask)
            {
                p_mme_context->relative_mme_capacity = 
                    mme_config_update.relative_mme_capacity;
            }
            
            /* Send the Acknowledgment */
            result = compose_s1ap_mme_configuration_update_ack(
                    asn_message,
                        &message_buff_len,
                        &error_indication,
                        &send_err_ind);

            if(RRC_SUCCESS != result) 
            {
                RRC_S1AP_TRACE(RRC_WARNING, "Unable to process ACK message : "
                        "MME CONFIGURATION UPDATE ACK");
                result = RRC_FAILURE;
                break;
            }

            s1ap_rrm_mme_info.mme_id = p_mme_context->mme_id;

            l3_memcpy_wrapper(s1ap_rrm_mme_info.ip_addr, p_mme_context->connection_info.ip_addr[0].
                    ip_addr, MAX_IP_ADDRESS_LENGTH);

            s1ap_rrm_mme_info.port = p_mme_context->connection_info.port;

            if (mme_config_update.bitmask & MME_CONFIGURATION_MME_NAME_PRESENT)
            {        
                s1ap_rrm_mme_info.mme_info.bitmask |= S1AP_MME_NAME_PRESENT;
                l3_memcpy_wrapper(s1ap_rrm_mme_info.mme_info.mme_name, mme_config_update.mme_name, 
                        MAX_MME_NAME_STR);
            }
            
            if (mme_config_update.gummei_info.num_served_gummeis)
            {
                s1ap_fill_served_gummei_info(p_mme_context,&s1ap_rrm_mme_info.
                        mme_info.served_gummei_info);
                s1ap_rrm_mme_info.mme_info.bitmask |= S1AP_SERVED_GUMMEI_INFO_PRESENT;
            }
            
            if (mme_config_update.bitmask & MME_CONFIGURATION_REL_MME_CAPACITY_PRESENT)
            {
                s1ap_rrm_mme_info.mme_info.relative_mme_capacity = mme_config_update.
                    relative_mme_capacity;
                s1ap_rrm_mme_info.mme_info.bitmask |= S1AP_RELATIVE_MME_CAPACITY_PRESENT;
            }

            /* PUP encode and send message to RRM */
            result = rrc_s1apRrm_il_send_s1ap_rrm_mme_info(&s1ap_rrm_mme_info,
                    RRC_S1AP_MODULE_ID,
                    RRC_RRM_MODULE_ID,
                    RRC_TRANSACTION_ID_ABSENT,
                    RRC_NULL);

            if (RRC_SUCCESS == result)
            {
                RRC_S1AP_TRACE(RRC_INFO,
                        "s1ap_rrm_mme_info Message sent to RRM");
            }
            else
            {
                RRC_S1AP_TRACE(RRC_INFO,
                        "Unable to send s1ap_rrm_mme_info message to RRM");
                break;
            }

            l3_memcpy_wrapper(&s1ap_oam_mme_info, &s1ap_rrm_mme_info, sizeof(s1ap_rrm_mme_info_t));
            /* PUP encode and send message to OAM */
            result = rrc_s1apOam_il_send_s1ap_oam_mme_info(&s1ap_oam_mme_info,
                    RRC_S1AP_MODULE_ID,
                    RRC_OAM_MODULE_ID,
                    RRC_TRANSACTION_ID_ABSENT,
                    RRC_NULL);

            if (RRC_SUCCESS == result)
            {
                RRC_S1AP_TRACE(RRC_INFO,
                        "s1ap_oam_mme_info Message sent to OAM");
            }
            else
            {
                RRC_S1AP_TRACE(RRC_INFO,
                        "Unable to send s1ap_oam_mme_info message to OAM");
                break;
            }

        }
    } while(0);
    
    if ((RRC_FAILURE == result) && 
            (RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT & error_indication.bitmask))
    {
        memset_wrapper(asn_message, 0, S1AP_MAX_ASN1_BUF_LEN);

        result = compose_s1ap_mme_configuration_update_failure(
                asn_message,
                &message_buff_len,
                &error_indication,
                &send_err_ind);

        if(RRC_SUCCESS != result)
        {
            RRC_S1AP_TRACE(RRC_WARNING, "Unable to process NACK message : "
                    "MME CONFIGURATION UPDATE FAILURE ");
        }
        else
        {
            p_mme_config_update_err = 
                rrc_mem_get(sizeof(LOCAL_MME_CONFIGURATION_UPDATE_ERROR_STRUCT));
            if (PNULL != p_mme_config_update_err)
            {
                memset_wrapper(p_mme_config_update_err, RRC_NULL, 
                        sizeof(LOCAL_MME_CONFIGURATION_UPDATE_ERROR_STRUCT));
                p_mme_config_update_err->header.length = 
                    sizeof(LOCAL_MME_CONFIGURATION_UPDATE_ERROR_STRUCT);
                p_mme_config_update_err->header.event_id = 
                    LOCAL_MME_CONFIGURATION_UPDATE_ERROR;

                l3_memcpy_wrapper(p_mme_config_update_err->EVENT_PARAM_MME_IP_ADDRESS,
                        p_mme_context->connection_info.ip_addr[0].ip_addr,
                        (sizeof(U8) * MAX_IP_ADDRESS_LENGTH));
                s1ap_fill_gummei_info(p_mme_context,
                        &p_mme_config_update_err->EVENT_PARAM_GUMMEI);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_mme_config_update_err);
            }
        }
    }
    
    if (RRC_SUCCESS == result)
    {
        /* SPR 2793 Start */
        result = send_msg_to_mme(p_mme_context, asn_message, message_buff_len,
                 PNULL);

        if( RRC_SUCCESS != result)
        {
            RRC_S1AP_TRACE(RRC_ERROR, 
                    "Unable to send message to MME %d",
                    p_mme_context->mme_id);
        }
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
* Function Name  : s1ap_process_mme_configuration_transfer 
* Inputs         : p_mme_ctx - Pointer to the S1AP mme  context data
*                  mme_id -MME id
*                  s1ap_gb_context_t - pointer to S1AP global context 
*                  p_asn_msg - Pointer to asn decoded msg
*                  asn_msg_len - Length of the asn decoded msg
* Outputs        : None
* Returns        : None
* Description    : The function processes MME Coniguration Transfer
******************************************************************************/
void s1ap_process_mme_configuration_transfer(
        mme_context_t *p_mme_context,
        U8 mme_id,
        s1ap_gb_context_t *p_s1ap_gb_ctxt,
        void *p_asn_msg)
{
    s1ap_rrm_mme_config_transfer_t mme_config_transfer;
    rrc_return_et result = RRC_SUCCESS;
    U16 trans_id = 1;


    memset_wrapper(&mme_config_transfer,
            RRC_NULL,
            sizeof(s1ap_rrm_mme_config_transfer_t));

    RRC_S1AP_UT_TRACE_ENTER();
    /*To remove warning*/
    result = parse_s1ap_mme_configuration_transfer(
            p_mme_context,
            &mme_config_transfer,
            p_s1ap_gb_ctxt,
            p_asn_msg,
            sizeof(s1ap_rrm_mme_config_transfer_t));

    if (RRC_SUCCESS == result)
    {
        /* Assigne the MME ID */
        mme_config_transfer.mme_id = mme_id;

        RRC_S1AP_TRACE(RRC_INFO,"%s: "
                "Success while processing MME "
                " Configuration Transfer message, "
                "Sending message to RRM",
                __FUNCTION__);

        /* Send message to RRM */
        result = rrc_s1apRrm_il_send_s1ap_rrm_mme_config_transfer(
                &mme_config_transfer,
                RRC_S1AP_MODULE_ID,
                RRC_RRM_MODULE_ID,
                trans_id,
                RRC_NULL);

        if (RRC_SUCCESS == result)
        {
            RRC_S1AP_TRACE(RRC_INFO,"%s: "
                    "MME Config Transger "
                    " Message sent to RRM", __FUNCTION__);
        }
        else
        {
            RRC_S1AP_TRACE(RRC_INFO,"%s: "
                    "Unable to send message to RRM", __FUNCTION__);
        }
    }
    else
    {
        RRC_S1AP_TRACE(RRC_INFO,"%s: "
                "Error while processing MME Configuration"
                " Transfer message",
                __FUNCTION__);
    }

    RRC_S1AP_UT_TRACE_EXIT();

    return;
}

/*****************************************************************************
* Function Name  : s1ap_process_mme_direct_info_transfer 
* Inputs         : p_mme_ctx - Pointer to the S1AP mme  context data
*                  mme_id -  MME id
*                  p_asn_msg - Pointer to asn decoded msg
*                  asn_msg_len - Length of the asn decoded msg
* Outputs        : None
* Returns        : None
* Description    : The function processes MME Direct Information Transfer
******************************************************************************/
/*SPR 19183 START*/
void s1ap_process_mme_direct_info_transfer(s1ap_gb_context_t *p_s1ap_gb_ctx,
/*SPR 19183 END*/
        mme_context_t *p_mme_context,
        U8 mme_id,
        void *p_asn_msg,
        U16 asn_msg_len)
{
    s1ap_rrm_mme_direct_info_transfer_t mme_direct_info_transfer;
    rrc_return_et result = RRC_SUCCESS;

    /* Setting it to default */
    U16 trans_id = 1;

    memset_wrapper(&mme_direct_info_transfer, 
            RRC_NULL,
            sizeof(s1ap_rrm_mme_direct_info_transfer_t));

    RRC_S1AP_UT_TRACE_ENTER();

    result = parse_s1ap_mme_direct_info_transfer(
            p_mme_context,
            &mme_direct_info_transfer,
            p_asn_msg,
            asn_msg_len);

    if (RRC_SUCCESS == result)
    {
        /* Assigne the MME ID */
        mme_direct_info_transfer.mme_id = mme_id;

        RRC_S1AP_TRACE(RRC_INFO,"%s: "
                "Success while processing MME Direct"
                " Information Transfer message, "
                "Sending message to RRM",
                __FUNCTION__);

        /* Send message to RRM */
        result = rrc_s1apRrm_il_send_s1ap_rrm_mme_direct_info_transfer(
                &mme_direct_info_transfer,
                RRC_S1AP_MODULE_ID,
                RRC_RRM_MODULE_ID,
                trans_id,
                RRC_NULL);

        if (RRC_SUCCESS == result)
        {
            RRC_S1AP_TRACE(RRC_INFO,"%s: "
                    "Message sent to RRM", __FUNCTION__);
        }
        else
        {
            RRC_S1AP_TRACE(RRC_INFO,"%s: "
                    "Unable to send message to RRM", __FUNCTION__);
        }
    /*SPR 19183 START*/
        result = rim_parse_incoming_tlv(&mme_direct_info_transfer,p_s1ap_gb_ctx);
    /*SPR 19183 END*/

	/* + Coverity 81571 */
    	if ( RRC_FAILURE == result )
    	{
        	RRC_TRACE(RRC_INFO, "Unable to parse rim Message");
    	}
	/* - Coverity 81571 */
    }
    else
    {
        RRC_S1AP_TRACE(RRC_INFO,"%s: "
                "Error while processing MME Direct"
                " Information Transfer message",
                __FUNCTION__);
    }

    RRC_S1AP_UT_TRACE_EXIT();

    return;
}
  /*SPR 19183 START*/
/*****************************************************************************
 * Function Name  : rim_parse_incoming_tlv
 * Inputs         : p_s1ap_rrm_direct_info - Pointer to 
 *                                         s1ap_rrm_mme_direct_info_transfer_t
 *                : p_s1ap_gb_ctx   -Pointer to s1ap_gb_context_t
 * Outputs        : None
 * Returns        : Success/Failure
 * Description    : The function parse incoming TLVs
*******************************************************************************/
static rrc_return_et rim_parse_incoming_tlv
(
  s1ap_rrm_mme_direct_info_transfer_t *p_s1ap_rrm_direct_info,
  s1ap_gb_context_t *p_s1ap_gb_ctx
)
  /*SPR 19183 END*/
{
    rim_information_t rim_info;
    rim_information_req_t rim_info_req;
    rim_information_ack_t rim_info_ack;
    rim_information_app_err_t rim_info_app_err;
    rim_information_err_t rim_info_err;
   
    /*SPR 19183 START*/
    ran_information_error_rim_t  ran_info_err_rim;
    U32 msg_length = RRC_NULL;
    U8 *p_msg = RRC_PNULL;
    /*SPR 19183 END*/
    
    S32 length_parsed=0x00;
    S32 trans_id =0;
    rrc_return_et  result = RRC_SUCCESS;
    U8 cause = 0xFF; /* Max value 0xFF - reserved/unspecified */

  RRC_S1AP_UT_TRACE_ENTER();
  if (0 == p_s1ap_rrm_direct_info->len_rim_info)
  {
      RRC_S1AP_TRACE(RRC_INFO,"%s: "
          " 0 length recvd for RIM PDU, discard", __FUNCTION__);
      result = RRC_FAILURE;
      return result;
  }
  memset_wrapper(&rim_info, RRC_NULL, sizeof(rim_information_t));
  memset_wrapper(&rim_info_req, RRC_NULL, sizeof(rim_information_req_t));          
  memset_wrapper(&rim_info_ack, RRC_NULL, sizeof(rim_information_ack_t));
  memset_wrapper(&rim_info_app_err, RRC_NULL, sizeof(rim_information_app_err_t));
  memset_wrapper(&rim_info_err, RRC_NULL, sizeof(rim_information_err_t));
/*KlockWork fix*/
  /*SPR 19183 START*/
  memset_wrapper(&ran_info_err_rim, RRC_NULL, sizeof(ran_information_error_rim_t));
  /*SPR 19183 END*/
/*KlockWork fix*/
  

  switch (p_s1ap_rrm_direct_info->rim_info[0])
  {
      case RIM_RAN_INFO_PDU:
          {
              /* set app_id in pdu to 0. After parsing check for app id. If
               * app_id is also not avbl, means that parsing failed early on.
               * Can not even send error pdu back to src */

              rim_info.app_idty = 0;

              if( RRC_FAILURE == rrc_il_parse_ran_information_rim(
                          &rim_info,
                          p_s1ap_rrm_direct_info->rim_info,
                          p_s1ap_rrm_direct_info->len_rim_info,
                          &length_parsed, &cause))
	      {
		  RRC_S1AP_TRACE(RRC_INFO,"%s: "
			  " RIM PDU parsing failed - send error PDU to MME", __FUNCTION__);
		  /* coverity fix 81318 start */
		  //result = RRC_FAILURE;
		  /* coverity fix 81318 end */
		  if(rim_info.app_idty !=0)
		  {
		      /*Coverity 83412 Fix Start*/
		      /*
			 U8 *p_msg = rrc_mem_get(sizeof (U8) * MAX_BSSGP_PDU_LEN);

			 if (p_msg == PNULL)
			 {
			 return RRC_FAILURE;
			 }*/
		      /*Coverity 83412 Fix End*/
		      /* convert to internal structure message */
                      /*SPR 19183 START*/
                      if (RRC_FAILURE ==  compose_intl_rim_err_pdu(
                              &rim_info,
                                  &ran_info_err_rim,
                              p_s1ap_rrm_direct_info->rim_info,
                              p_s1ap_rrm_direct_info->len_rim_info,
                                  &cause))
                      {                              
                              
                          RRC_S1AP_TRACE(RRC_INFO,"%s: "
                                  " Unable to compose error PDU", __FUNCTION__);
                      }
                      else
                      {

                          /* Allocate buffer */
                          p_msg = rrc_mem_get(sizeof (U8) * MAX_BSSGP_PDU_LEN);
                          if ( p_msg != PNULL)
                          {
                              /*Encode TLV message*/
                              if (RRC_FAILURE == rrc_intl_compose_ran_information_error_rim(&p_msg,&ran_info_err_rim, &msg_length))
                              {
                      RRC_S1AP_TRACE(RRC_INFO,"%s: "
                                          "Fail to Encode Ran-Information-Error to TLV", __FUNCTION__);
                              }
                              else /*Send RIM Error PDU */
                              {
                                  send_rim_info_error(p_msg,msg_length,&rim_info.src_cell,p_s1ap_rrm_direct_info->mme_id, p_s1ap_gb_ctx);
                              }  

                              rrc_mem_free(p_msg);
                              p_msg = PNULL;
                          }
                          else
                          {
                              RRC_S1AP_TRACE(RRC_INFO,"%s: "
                                      " Unable to Allocate Memory for BSSGP PDU", __FUNCTION__);
                          }    
                      }
                      /*SPR 19183 END*/
                  }
              }
              else
              {
                  rim_info.bitmask |= RIM_INFO_MME_ID_PRESENT;
                  rim_info.mme_id = p_s1ap_rrm_direct_info->mme_id;
                    
                  if( RRC_SUCCESS == rrc_s1apRrm_il_send_rim_information(
                              &rim_info,
                              RRC_S1AP_MODULE_ID,
                              RRC_RRM_MODULE_ID,
                              trans_id,
                              RRC_NULL))
                  {

                      RRC_S1AP_TRACE(RRC_INFO,"%s: "
                              "Message sent to RRM", __FUNCTION__);
                  }
                  else
                  {
                      RRC_S1AP_TRACE(RRC_INFO,"%s: "
                              "Message sent to RRM", __FUNCTION__);
                  }
              }
              break;
          }
      case RIM_RAN_INFO_REQ_PDU:
          {
              RRC_S1AP_TRACE(RRC_WARNING, "PDU type not handled currently");
              break;
          }
      case RIM_RAN_INFO_ACK_PDU:
          {
              RRC_S1AP_TRACE(RRC_WARNING, "PDU type not handled currently");
              break;
          }
      case RIM_RAN_INFO_ERR_PDU:
          {
              if( RRC_FAILURE == rrc_il_parse_ran_information_error_rim(
                      &rim_info_err,
                      p_s1ap_rrm_direct_info->rim_info,
                      p_s1ap_rrm_direct_info->len_rim_info,
                      &length_parsed))
              {
                    RRC_S1AP_TRACE(RRC_WARNING, "error in error PDU - discard");

              }
              else
              {
                  rim_info_err.bitmask |= RIM_INFO_ERR_MME_ID_PRESENT;
                  rim_info_err.mme_id = p_s1ap_rrm_direct_info->mme_id;
                  if( RRC_SUCCESS == rrc_s1apRrm_il_send_rim_information_err(
                              &rim_info_err,
                              RRC_S1AP_MODULE_ID,
                              RRC_RRM_MODULE_ID,
                              trans_id,
                              RRC_NULL))
                  {

                      RRC_S1AP_TRACE(RRC_INFO,"%s: "
                              "Message sent to RRM", __FUNCTION__);
                  }
                  else
                  {
                      RRC_S1AP_TRACE(RRC_INFO,"%s: "
                              "Message sent to RRM", __FUNCTION__);
                  }
              }
              break;
          }
      case RIM_RAN_INFO_APP_ERR_PDU:
          {
              RRC_S1AP_TRACE(RRC_WARNING, "PDU type not handled currently");
              break;
          }
      default:
          {
              RRC_S1AP_TRACE(RRC_WARNING, "PDU type not handled currently");
              break;
          }
  }

    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/*SPR 19183 START*/
/*****************************************************************************
 * Function Name  : compose_intl_cell_id_from_routing_info
 * Inputs         : p_routing_info - Pointer to structure rim_routing_info_t
 * Outputs        : p_intl_cell_id - Pointer to structure rim_cell_id_intl_t 
 * Returns        : void
 * Description    : The function populates the rim internal cell identity 
 *                  strcture from rim routing info structure.
******************************************************************************/
static rrc_return_et
compose_intl_cell_id_from_routing_info(
 rim_routing_info_t *p_routing_info,
 rim_cell_id_intl_t *p_intl_cell_id
)
{
    rrc_return_et  retval = RRC_SUCCESS; 
   if(p_routing_info->bitmask & RIM_GERAN_ROUTING_ADDR_PRESENT)
   {
        p_intl_cell_id->routing_add_disc = RIM_ROUT_ADD_DISC_GERAN_CELL ;

        l3_memcpy_wrapper(p_intl_cell_id->cell_identifier,
                p_routing_info->geran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_intl_cell_id->cell_identifier + RIM_PLMN_OCTETS)),
                p_routing_info->geran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_intl_cell_id->cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_routing_info->geran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_intl_cell_id->cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                p_routing_info->geran_rtng_addr.geran_cell_idty,
                RIM_GERAN_CELL_OCTETS);

        p_intl_cell_id->length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + 
            RIM_GERAN_CELL_OCTETS;
   }
    
   if (p_routing_info->bitmask & RIM_UTRAN_ROUTING_ADDR_PRESENT)
   {
        p_intl_cell_id->routing_add_disc = RIM_ROUT_ADD_DISC_UTRAN_RNC;
        l3_memcpy_wrapper(p_intl_cell_id->cell_identifier,
                p_routing_info->utran_rtng_addr.routing_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_intl_cell_id->cell_identifier + RIM_PLMN_OCTETS)),
                p_routing_info->utran_rtng_addr.routing_idty.lac,
                RIM_LAC_OCTETS);

        l3_memcpy_wrapper(((p_intl_cell_id->cell_identifier + RIM_PLMN_OCTETS + RIM_LAC_OCTETS)),
                &p_routing_info->utran_rtng_addr.routing_idty.rac,
                sizeof(U8));

        l3_memcpy_wrapper(((p_intl_cell_id->cell_identifier + RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE)),
                p_routing_info->utran_rtng_addr.rnc_id,
                RIM_RNC_ID_OCTETS);
        
        p_intl_cell_id->length = RIM_ROUTING_AREA_IDENTIFICATION_OCTET_SIZE + 
            RIM_RNC_ID_OCTETS;
   }
    
    if (p_routing_info->bitmask & RIM_EUTRAN_ROUTING_ADDR_PRESENT)
    {
        p_intl_cell_id->routing_add_disc = RIM_ROUT_ADD_DISC_ENB_ID;
        l3_memcpy_wrapper(p_intl_cell_id->cell_identifier,
                p_routing_info->eutran_rtng_addr.ta_idty.plmn.plmn,
                RIM_PLMN_OCTETS);

        l3_memcpy_wrapper(((p_intl_cell_id->cell_identifier + RIM_PLMN_OCTETS)),
                p_routing_info->eutran_rtng_addr.ta_idty.tac,
                RIM_TAC_OCTETS);

        rim_asn_encode_global_enb_id(&p_routing_info->eutran_rtng_addr.enb, p_intl_cell_id);
    }
    return retval;
}
/*SPR 19183 END*/
/*****************************************************************************
 * Function Name  : compose_intl_rim_err_pdu
 * Inputs         : p_src_msg - structure pointer of rim_informatio
 *                  p_tgt_msg - internal structure pointer of 
 *                          rim error pdu, this will get updated for TLV addition
 *                  rim_info - buffer to store rim_info
 *                  len_rim_info - length of rim pdu
 *                  p_cause - pointer to cause  
 * Outputs        : rim error pdu
 * Returns        : Success/Failure
 * Description    : The function composes the rim error pdu to an internal
 *                  structure.
******************************************************************************/
static
rrc_return_et
compose_intl_rim_err_pdu(
        rim_information_t *p_src_msg,
        /*SPR 19183 START*/
        ran_information_error_rim_t *p_tgt_msg,
        /*SPR 19183 END*/
        U8 rim_info[MAX_BSSGP_PDU_LEN],
        U32      len_rim_info,
        U8       *p_cause)
{

    /*SPR 19183 START*/
    rrc_return_et  retval = RRC_SUCCESS; 
    p_tgt_msg->pdu_type = RIM_RAN_INFO_ERR_PDU;
    
    if((RRC_FAILURE == compose_intl_cell_id_from_routing_info(&p_src_msg->dst_cell,&p_tgt_msg->src_cell_id)) ||
       (RRC_FAILURE == compose_intl_cell_id_from_routing_info(&p_src_msg->src_cell,&p_tgt_msg->dst_cell_id)))
    {
          retval = RRC_FAILURE;
    }
    else
    {
		    p_tgt_msg->bitmask = RIM_INFO_DST_CELL_ID_PRESENT|RIM_INFO_SRC_CELL_ID_PRESENT|RIM_INFO_ERROR_PRESENT;

    /*Filling app id in error pdu*/
		    p_tgt_msg->rim_error.rim_app_id.rim_app_id = p_src_msg->app_idty;
		    p_tgt_msg->rim_error.bitmask |= RIM_INFO_ERROR_APP_ID_PRESENT;

    /*Filling error cause in error pdu */
		    l3_memcpy_wrapper(&p_tgt_msg->rim_error.rim_cause.cause,p_cause,sizeof(U8));
		    p_tgt_msg->rim_error.bitmask |= RIM_INFO_ERROR_CAUSE_PRESENT;

    /* Filling protocol version */
		    p_tgt_msg->rim_error.rim_protocol_ver.rim_protocol_ver =0x01; //Currently eNB supports only version 1
		    p_tgt_msg->rim_error.bitmask |= RIM_INFO_ERROR_PROTOCOL_VER_PRESENT;

    /* Filling the whole received pdu in error pdu section */
		    l3_memcpy_wrapper(p_tgt_msg->rim_error.error_in_pdu.error_pdu,rim_info,len_rim_info);
                    p_tgt_msg->rim_error.error_in_pdu.counter = len_rim_info;
		    p_tgt_msg->rim_error.bitmask |= RIM_INFO_ERROR_PDU_PRESENT;

		    /* Filling of son id in error information is not mentioned in spec */
    }
    return retval;
    /*SPR 19183 END*/
}


/*****************************************************************************
 * Function Name  : s1ap_process_location_reporting_control
 * Inputs         : mme_id - id of mme from where msg has come
 *                  p_asn_msg - Pointer to asn decoded msg
 *                  asn_msg_len - Length of the asn decoded msg
 *                  enb_ue_s1ap_id - ue id over s1 interface in  eNB 
 *                  p_s1ap_gb_ctx - pointer to S1AP gobal context 
 *                  ue_id_pair_presence - ue id pair presence
 *                  mme_ue_s1ap_id - ue id over s1 interface in  MME
 *                  stream_id - stream id
 * Outputs        : None
 * Returns        : None
 * Description    : The function processes location reporting control.
******************************************************************************/
void  s1ap_process_location_reporting_control
(     
 U8     mme_id,
 void*  p_asn_msg,
 U16    asn_msg_len,
 s1ap_ENB_UE_S1AP_ID enb_ue_s1ap_id,
 s1ap_gb_context_t    *p_s1ap_gb_ctx,
 U8                   ue_id_pair_presence,
 U32                    mme_ue_s1ap_id,
 mme_context_t         *p_mme_context,
 U16                   stream_id  
 )
{
    void *          p_intrl_msg = PNULL;
    s1ap_ue_associated_sig_msg_ind_t    *msg_p = PNULL;
    ue_s1ap_context_t     *pp_s1ap_ue_context = PNULL;
    U32 deleted_enb_ue_s1apid = 0xFFFFFFFF;
    s1ap_reset_msg_ind_t s1ap_reset_ind;
    /*SPR 21220 Start*/
    U32 ue_index;
    /*SPR 21220 Stop*/
    /* SPR_22097 Fix - Start */
    U32 deleted_enb_ue_s1apid_1 = 0xFFFFFFFF;
    U32 cause;
    /* SPR_22097 Fix - Stop */

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_asn_msg);

    memset_wrapper((void*)&s1ap_reset_ind,RRC_NULL,sizeof(s1ap_reset_msg_ind_t));
    get_ue_ctx_from_ue_id(p_s1ap_gb_ctx, 
            &pp_s1ap_ue_context, enb_ue_s1ap_id);

    if( pp_s1ap_ue_context == PNULL)
    {
        if ( ue_id_pair_presence & ENB_UE_S1AP_ID_PRESENT)
        {
            /* enb ue s1apId is invalid, hence finding if there is any entry of
             * mme ue s1apId in search tree. If found, include it in S1AP RESET IND
             * msg and relsease it's resoures in S1AP*/

            /* SPR_22097 Fix - Start */
            /* Set Cause to Unknown eNB UE S1AP ID by default in this case */
            cause = s1ap_unknown_enb_ue_s1ap_id;
            /* SPR_22097 Fix - Stop */

            if ( (ue_id_pair_presence & MME_UE_S1AP_ID_PRESENT))
            {
                /* SPR_22097 Fix - Start */
                /* Set Cause to Unknown Pair as enb_ue_s1ap_id and mme_ue_s1ap_id both received */
                cause = s1ap_unknown_pair_ue_s1ap_id;
                /* SPR_22097 Fix - Stop */

                /*SPR 21220 Start*/
                ue_index = delete_s1ap_context( mme_ue_s1ap_id,
                        p_mme_context,
                        &deleted_enb_ue_s1apid,
                        p_s1ap_gb_ctx);

                if (ue_index != 0xFFFFFFFF)
                {
                    RRC_S1AP_TRACE(RRC_INFO, 
                            "[%s]Deleting context for UE_INDEX %u "
                            "and adding it to RESET IND list",
                            __FUNCTION__,ue_index);
                    /*BUG 604 changes start*/
                    s1ap_reset_ind.ue_index_list.ue_index[
                        s1ap_reset_ind.ue_index_list.count] =
                        ue_index;
                    s1ap_reset_ind.ue_index_list.count++;

                    s1ap_reset_ind.bitmask |= 
                        S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT;
                    /*BUG 604 changes stop*/
                    s1ap_build_and_send_reset_ind_msg(
                            &s1ap_reset_ind,RRC_UECCMD_MODULE_ID);

                    RRC_S1AP_TRACE(RRC_ERROR, "[%s] enb Ue s1ap id INVALID"
                            " Sending S1AP RESET IND for UE_INDEX %u",
                            __FUNCTION__,ue_index);
                }
                /*SPR 21220 Stop*/
            }
            fill_and_send_error_ind_msg(p_mme_context,
                    ue_id_pair_presence,
                    enb_ue_s1ap_id,
                    mme_ue_s1ap_id,
                    cause,
                    &stream_id);
            /* SPR 2551 Start */
            return;
        }
    }
    /* SPR_22097 Fix - Start */    
    else
    {
        if (MME_UE_S1AP_ID_PRESENT & ue_id_pair_presence)
        {
            if (pp_s1ap_ue_context->mme_ue_s1ap_id != mme_ue_s1ap_id)
            {
                /* Finding if there is any entry of mme ue s1apId in search tree.
                 * and mme ue s1apId is invalid so Include it in S1AP RESET IND 
                 * msg and relsease it's resoures in S1AP*/

                RRC_S1AP_TRACE(RRC_ERROR, "[%s]: MME ue s1ap id %d is inconsistent"
                        " with the one present in ue context %d\n",
                        __FUNCTION__,mme_ue_s1ap_id,
                        pp_s1ap_ue_context->mme_ue_s1ap_id);

                /* Set Cause to Unknown Pair as enb_ue_s1ap_id and mme_ue_s1ap_id both received */
                cause = s1ap_unknown_pair_ue_s1ap_id;

                /* Finding the mme ue s1apId as per the recieved enb ue s1apId*/
                ue_index = delete_s1ap_context(pp_s1ap_ue_context->mme_ue_s1ap_id,
                        p_mme_context,
                        &deleted_enb_ue_s1apid_1,
                        p_s1ap_gb_ctx);
                if (ue_index != 0xFFFFFFFF)
                {
                    RRC_S1AP_TRACE(RRC_ERROR, "[%s]Deleting context for "
                            "UE_INDEX %u and adding it to RESET IND list",
                            __FUNCTION__, pp_s1ap_ue_context->ue_index);

                    s1ap_reset_ind.ue_index_list.ue_index[
                        s1ap_reset_ind.ue_index_list.count] = pp_s1ap_ue_context->ue_index;
                    s1ap_reset_ind.ue_index_list.count++;
                }

                /* Finding the enb ue s1apId as per the recieved mme ue s1apId*/
                ue_index = delete_s1ap_context( mme_ue_s1ap_id,
                        p_mme_context,
                        &deleted_enb_ue_s1apid,
                        p_s1ap_gb_ctx);

                if ((ue_index != 0xFFFFFFFF) && (ue_index != pp_s1ap_ue_context->ue_index))
                {
                    RRC_S1AP_TRACE(RRC_ERROR,
                            "[%s]Deleting context for UE_INDEX %u and adding it to RESET IND list",
                            __FUNCTION__,ue_index);

                    s1ap_reset_ind.ue_index_list.ue_index[
                        s1ap_reset_ind.ue_index_list.count] = ue_index;
                    s1ap_reset_ind.ue_index_list.count++;
                }

                if (RRC_NULL < s1ap_reset_ind.ue_index_list.count)
                {
                    s1ap_reset_ind.bitmask |= S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT;
                    s1ap_build_and_send_reset_ind_msg(
                            &s1ap_reset_ind,RRC_UECCMD_MODULE_ID);

                    RRC_S1AP_TRACE(RRC_ERROR, "[%s] mme Ue s1ap id INVALID"
                            " Sending S1AP RESET IND for enbues1apid 0x%x\n",
                            __FUNCTION__,enb_ue_s1ap_id);
                }
                fill_and_send_error_ind_msg(p_mme_context,
                        ue_id_pair_presence,
                        enb_ue_s1ap_id,
                        mme_ue_s1ap_id,
                        cause,
                        &stream_id);
                return;
            }
        }
        /* Send  location reporting control  message indication to UECC  */
        p_intrl_msg = s1ap_alloc_intrl_msg(
                RRC_UECC_MODULE_ID,
                S1AP_UE_ASSOCIATED_SIG_MSG_IND,
                (U16)(sizeof(s1ap_ue_associated_sig_msg_ind_t) + asn_msg_len));

        if (PNULL == p_intrl_msg)
        {
            /*out of memory*/
            RRC_S1AP_TRACE(RRC_FATAL, "rrc_mem_get failed!!");
            return ;
        }
        msg_p = (s1ap_ue_associated_sig_msg_ind_t *)((U8 *)p_intrl_msg +
                RRC_API_HEADER_SIZE);
        msg_p->bitmask |= UE_ASSOC_SIG_MSG_IND_MME_ID_PRESENT;
		//add by wood for 114111 begin
		msg_p->s1ap_procedure_code = ASN1V_s1ap_id_LocationReportingControl;
		msg_p->message_type = INITIATING_MESSAGE;
		//add by wood for 114111 end
        msg_p->mme_id = mme_id;
        /*BUG 604 changes start*/
        /*Fill ue_index*/
        msg_p->ue_index = pp_s1ap_ue_context->ue_index;
        /*BUG 604 changes stop*/

        SET_UE_INDEX(msg_p->ue_index);

        l3_memcpy_wrapper((void*)((U8*)p_intrl_msg + RRC_API_HEADER_SIZE +
                    sizeof(s1ap_ue_associated_sig_msg_ind_t)),
                (const void*)p_asn_msg,
                asn_msg_len);
        RRC_S1AP_TRACE(RRC_INFO, "[%s]: Forwarding message to UECC\n",
                __FUNCTION__);
        /*BUG 604 changes start*/
        if ( (RRC_GET_UECC_MODULE_ID(pp_s1ap_ue_context->ue_index) - RRC_UECC_1_MODULE_ID)
                < p_s1ap_gb_ctx->no_of_uecc_instances)
        {
            /* Send response */
            rrc_send_message(
                    p_intrl_msg,
                    RRC_GET_UECC_MODULE_ID(pp_s1ap_ue_context->ue_index));
        }
        /*BUG 604 changes stop*/
        else
        {
            RRC_S1AP_TRACE(RRC_WARNING,"UECC instance does not exist");
            /* coverity_fix_48264_start */
            if ( PNULL != p_intrl_msg )
            {
                /* SPR 15887 Start */
                rrc_msg_mem_free(p_intrl_msg);
                /* SPR 15887 Stop */
                p_intrl_msg = PNULL;
            }
            /* coverity_fix_48264_stop */
        }
    }
    /* SPR_22097 Fix - Stop */
    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : send_msg_to_mme 
 * Inputs         : p_mme_ctx - MME Context 
 *                  p_message - ASN Encoded Buffer
 *                  message_buff_len - Buffer Length
 *                  p_stream_id - pointer to stram id
 * Outputs        : None 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function send the message to MME
 ******************************************************************************/
rrc_return_et send_msg_to_mme(
	mme_context_t *p_mme_ctx, 
	U8* p_message, 
	U32 message_buff_len,
	U16 *p_stream_id)
{
    U16 stream_id = 0;
    rrc_return_et return_val = RRC_SUCCESS;

    RRC_S1AP_UT_TRACE_ENTER();

    /* SPR 2793 Start */
    if (PNULL != p_stream_id)
    {
	stream_id = *p_stream_id;
    }
    else
    {
	stream_id = p_mme_ctx->stream_info.stream_ids[SCTP_COMMON_PROC_STREAMS];
    }
    /* SPR 2793 Stop */

    /* Check if the MME is in ACTIVE State */
    if (p_mme_ctx->current_mme_state != MME_ACTIVE)
    {
	RRC_S1AP_TRACE(RRC_WARNING, "%s:"
		" MME context with id %d "
		"is not active.",
		__FUNCTION__,p_mme_ctx->mme_id);

	return_val = RRC_FAILURE;
    }
    else
    {
	RRC_S1AP_TRACE(RRC_INFO,"[%s]:Sending "
		"Message to MME %d",
		__FUNCTION__,p_mme_ctx->mme_id);

	/* Send the Message to MME */
#ifndef LINUX_PC_TEST		
	if (SCTP_SOCKET_ERROR ==l3_sctp_send(
		    p_mme_ctx->sctp_sd,
		    p_message,
		    message_buff_len,
		    stream_id))
#else
	if(SCTP_SOCKET_ERROR == pctest_forward_mme_message_to_pc(
				RRC_S1AP_MODULE_ID,
				RRC_MME_MODULE_ID,
				MME_MESSAGE_PS_TO_TEST,
				p_mme_ctx->sctp_sd,
				stream_id, //stream id
				p_message,
				message_buff_len));
	char * pmsg;
	pmsg = change_data_to_str(message_buff_len, p_message);
	RRC_S1AP_TRACE(RRC_INFO,"l3_sctp_send:%s ",pmsg);
	return return_val;
#endif     		    
	{
	    RRC_S1AP_TRACE(RRC_WARNING, "%s:"
		    " l3_sctp_send send failed",
		    __FUNCTION__);
	    return_val = RRC_FAILURE;
	}
    }
    RRC_S1AP_UT_TRACE_EXIT();

    return return_val;
}

/*****************************************************************************
 * Function Name  : s1ap_process_mme_error_indication
 * Inputs         : p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *                  mme_ue_s1ap_id - id of ue over s1 interface in MME
 *                  mme_id - Pointer to the mme Index to be updated
 *                  p_asn_msg - Pointer to asn decoded msg
 *                  asn_msg_len - Length of the asn decoded msg
 * Outputs        : None
 * Returns        : None
 * Description    : The function error indication from MME
 *****************************************************************************/
void s1ap_process_mme_error_indication(
	s1ap_gb_context_t    *p_s1ap_gb_ctx,
	U8                   mme_id,
	void                 *p_asn_msg,
	U16                  asn_msg_len)
{
    void                                *p_intrl_msg = PNULL;
    s1ap_ue_associated_sig_msg_ind_t    *msg_p       = PNULL;
    rrc_s1ap_error_indication_t          error_indication;

    rrc_bool_et                         fwd_to_uecc = RRC_FALSE;
    LOCAL_EVENT_S1_ERROR_INDICATION_STRUCT *p_event_s1_error_ind = PNULL;
    /*BUG 604 changes start*/
    ue_s1ap_context_t     *pp_s1ap_ue_context = PNULL;
    /*BUG 604 changes stop*/

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_asn_msg);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    memset_wrapper(&error_indication, 0, sizeof(rrc_s1ap_error_indication_t));

    if (RRC_SUCCESS == parse_error_indication(
		&error_indication,
		p_asn_msg,
		asn_msg_len,
		p_s1ap_gb_ctx))
    {
	p_event_s1_error_ind = rrc_mem_get(sizeof(LOCAL_EVENT_S1_ERROR_INDICATION_STRUCT));

	if (PNULL != p_event_s1_error_ind)
	{
	    memset_wrapper(p_event_s1_error_ind, RRC_NULL, 
		    sizeof(LOCAL_EVENT_S1_ERROR_INDICATION_STRUCT));

	    p_event_s1_error_ind->header.length = sizeof(LOCAL_EVENT_S1_ERROR_INDICATION_STRUCT);
	    p_event_s1_error_ind->header.event_id = LOCAL_EVENT_S1_ERROR_INDICATION;

	    p_event_s1_error_ind->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_RECEIVED;
	    if((error_indication.bitmask &
			RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT))
	    {
		p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE_GROUP = error_indication.cause.t;

		/* CSR 51447 START */
		switch(p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE_GROUP)
		{
		    case T_s1ap_Cause_radioNetwork: 
			p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
			    error_indication.cause.u.radioNetwork;
			ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
				NOTIFICATION, "cause type = %d\n and cause value = %d",
				error_indication.cause.t, error_indication.cause.u.radioNetwork);
			break;

		    case T_s1ap_Cause_transport: 
			p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
			    error_indication.cause.u.transport;
			ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
				NOTIFICATION, "cause type = %d\n and cause value = %d",
				error_indication.cause.t, error_indication.cause.u.transport);
			break;

		    case T_s1ap_Cause_nas: 
			p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
			    error_indication.cause.u.nas;
			ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
				NOTIFICATION, "cause type = %d\n and cause value = %d",
				error_indication.cause.t, error_indication.cause.u.nas);
			break; 

		    case T_s1ap_Cause_protocol:
			p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
			    error_indication.cause.u.protocol;
			ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
				NOTIFICATION, "cause type = %d\n and cause value = %d",
				error_indication.cause.t, error_indication.cause.u.protocol);
			break; 

		    case T_s1ap_Cause_misc:
			p_event_s1_error_ind->EVENT_PARAM_3GPP_CAUSE = 
			    error_indication.cause.u.misc;
			ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_S1AP_ERROR_INDICATION_ALARM_ID, 
				NOTIFICATION, "cause type = %d\n and cause value = %d",
				error_indication.cause.t, error_indication.cause.u.misc);
			break; 
		}
		/* CSR 51447 STOP */
	    }
	    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, 
		    p_event_s1_error_ind);
	}

	log_error_indication_message(&error_indication); 
        /* SPR 16750 Fix Start */
        if ( (error_indication.bitmask & 
                RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT) ||
             (error_indication.bitmask & 
                RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT) ||
             (error_indication.bitmask & 
                RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT) )

        /* SPR 16750 Fix Stop */
	{
	    if (error_indication.criticality_diagnostics.m.procedureCodePresent)
	    { 
		RRC_S1AP_TRACE(RRC_INFO, "Error Indication Received Procedure" 
			"Code:%d",error_indication.
			criticality_diagnostics.procedureCode);
		switch(error_indication.criticality_diagnostics.procedureCode)
		{
		    case ASN1V_s1ap_id_Reset:
			s1ap_mme_error_ind_proc_code_reset(
				p_s1ap_gb_ctx);/*SPR 17777 +-*/
                        break; 
                    case ASN1V_s1ap_id_Paging:
                    case ASN1V_s1ap_id_uplinkNASTransport:
		   /*SPR 14212 Fix Start*/
                    case ASN1V_s1ap_id_MMEConfigurationUpdate:
		   /*SPR 14212 Fix Stop */
                    case ASN1V_s1ap_id_ErrorIndication:
                    case ASN1V_s1ap_id_S1Setup:
                        s1ap_mme_error_ind_proc_code_s1setup(
                                p_s1ap_gb_ctx,
                                mme_id);/*SPR 17777 +-*/
                        break;
                    case ASN1V_s1ap_id_ENBConfigurationUpdate:
                        s1ap_mme_error_ind_proc_code_enb_configuration_update(
                                p_s1ap_gb_ctx,
                                mme_id);/*SPR 17777 +-*/
                        break;
                    case ASN1V_s1ap_id_uplinkUEAssociatedLPPaTransport:
                        RRC_S1AP_TRACE(RRC_INFO, "lppa UE associated ErrorIndication!!");
                        break;

                    case ASN1V_s1ap_id_uplinkNonUEAssociatedLPPaTransport:
                        RRC_S1AP_TRACE(RRC_INFO, "lppa NON-UE associated ErrorIndication!!");
                        break;
                    
                    default:
                        fwd_to_uecc = RRC_TRUE;
                }
		/* SPR 16750 Fix Start */
	    }
	    else if ( (error_indication.bitmask & 
				    RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT) ||
			    (error_indication.bitmask & 
			     RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT) )
	    {
		    fwd_to_uecc = RRC_TRUE;
	    }
	    if (fwd_to_uecc == RRC_TRUE)
	    {
		    p_intrl_msg = s1ap_alloc_intrl_msg(
				    RRC_UECC_MODULE_ID,
				    S1AP_UE_ASSOCIATED_SIG_MSG_IND,
				    (U16)(sizeof(s1ap_ue_associated_sig_msg_ind_t) + asn_msg_len));

		    if (PNULL == p_intrl_msg)
		    {
			    /*out of memory*/
			    RRC_S1AP_TRACE(RRC_FATAL, "rrc_mem_get failed!!");
			    RRC_S1AP_UT_TRACE_EXIT();
			    return;
		    }

		    msg_p = (s1ap_ue_associated_sig_msg_ind_t *)((U8 *)p_intrl_msg +
				    RRC_API_HEADER_SIZE);
		    msg_p->bitmask |= UE_ASSOC_SIG_MSG_IND_MME_ID_PRESENT;
		    msg_p->mme_id = mme_id;
		    msg_p->s1ap_procedure_code = ASN1V_s1ap_id_ErrorIndication;
		    msg_p->message_type = INITIATING_MESSAGE;
		    /*BUG 604 changes start*/
		    /*Fill ue_index*/
		    get_ue_ctx_from_ue_id(p_s1ap_gb_ctx, 
				    &pp_s1ap_ue_context,error_indication.enb_ue_s1ap_id);
		    if(PNULL != pp_s1ap_ue_context)
		    {
			    msg_p->ue_index = pp_s1ap_ue_context->ue_index;
		    }
		    /*BUG 604 changes stop*/

		    l3_memcpy_wrapper((void*)((U8*)p_intrl_msg + RRC_API_HEADER_SIZE +
					    sizeof(s1ap_ue_associated_sig_msg_ind_t)),
				    (const void*)p_asn_msg,
				    asn_msg_len);

		    if ( error_indication.bitmask & 
				    RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT)
		    {
			    RRC_S1AP_TRACE(RRC_INFO, "[%s]: Forwarding message to UECC\n",
					    __FUNCTION__);
			    /*BUG 604 changes start*/
			    if(PNULL != pp_s1ap_ue_context)
			    {
				    if ((RRC_GET_UECC_MODULE_ID(
								    pp_s1ap_ue_context->ue_index) - RRC_UECC_1_MODULE_ID)
						    < p_s1ap_gb_ctx->no_of_uecc_instances)
				    {
					    /* Send response */
					    rrc_send_message(
							    p_intrl_msg,
							    RRC_GET_UECC_MODULE_ID(
								    pp_s1ap_ue_context->ue_index));
				    }
				    else
				    {
					    RRC_S1AP_TRACE(RRC_WARNING,"UECC instance does not exist");

				    }
			    }
			    else
			    {
				    /* Send response */
				    rrc_send_message(
						    p_intrl_msg,
						    RRC_UECC_1_MODULE_ID);
			    }
			    /*BUG 604 changes stop*/
		    }
		    else
		    {
			    RRC_S1AP_TRACE(RRC_INFO, "[%s]: Forwarding message to UECC MD\n",
					    __FUNCTION__);
			    /* Send response */
			    rrc_send_message(
					    p_intrl_msg,
					    RRC_UECCMD_MODULE_ID);

		    }
	    }
	    /* SPR 16750 Fix Stop */
	}

	/* SPR 13560 Fix Start */  
	else
	{

		p_intrl_msg = s1ap_alloc_intrl_msg(
				RRC_UECC_MODULE_ID,
				S1AP_UE_ASSOCIATED_SIG_MSG_IND,
				(U16)(sizeof(s1ap_ue_associated_sig_msg_ind_t) + asn_msg_len));

		if (PNULL == p_intrl_msg)
		{
			/*out of memory*/
			RRC_S1AP_TRACE(RRC_FATAL, "rrc_mem_get failed!!");
			RRC_S1AP_UT_TRACE_EXIT();
			return;
		}

		msg_p = (s1ap_ue_associated_sig_msg_ind_t *)((U8 *)p_intrl_msg +
				RRC_API_HEADER_SIZE);
		msg_p->bitmask |= UE_ASSOC_SIG_MSG_IND_MME_ID_PRESENT;
		msg_p->mme_id = mme_id;
		msg_p->s1ap_procedure_code = ASN1V_s1ap_id_ErrorIndication;
		msg_p->message_type = INITIATING_MESSAGE;


		/*Fill ue_index*/
		get_ue_ctx_from_ue_id(p_s1ap_gb_ctx, 
				&pp_s1ap_ue_context,error_indication.enb_ue_s1ap_id);
		if(PNULL != pp_s1ap_ue_context)
		{
			msg_p->ue_index = pp_s1ap_ue_context->ue_index;
		}

		l3_memcpy_wrapper((void*)((U8*)p_intrl_msg + RRC_API_HEADER_SIZE +
					sizeof(s1ap_ue_associated_sig_msg_ind_t)),
				(const void*)p_asn_msg,
				asn_msg_len);

		if ( error_indication.bitmask & 
				RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT)
		{
			RRC_S1AP_TRACE(RRC_INFO, "[%s]: Forwarding message to UECC\n",
					__FUNCTION__);
			if(PNULL != pp_s1ap_ue_context)
			{
				if ((RRC_GET_UECC_MODULE_ID(
								pp_s1ap_ue_context->ue_index) - RRC_UECC_1_MODULE_ID)
						< p_s1ap_gb_ctx->no_of_uecc_instances)
				{
					/* Send response */
					rrc_send_message(
							p_intrl_msg,
							RRC_GET_UECC_MODULE_ID(
								pp_s1ap_ue_context->ue_index));
				}
				else
				{
					RRC_S1AP_TRACE(RRC_WARNING,"UECC instance does not exist");

				}
			}
			else
			{
				/* Send response */
				rrc_send_message(
						p_intrl_msg,
						RRC_UECC_1_MODULE_ID);
			}
		}
		else
		{
			RRC_S1AP_TRACE(RRC_INFO, "[%s]: Forwarding message to UECC MD\n",
					__FUNCTION__);
			/* Send response */
			rrc_send_message(
					p_intrl_msg,
					RRC_UECCMD_MODULE_ID);

		}

	}
	/* SPR 13560 Fix Stop */  

    }
    else
    {
	    RRC_TRACE(RRC_INFO, "Unable to parse Error Indication Message");
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return;
}

/* etws_start*/
/*****************************************************************************
* Function Name  : s1ap_process_write_replace_warning_request
* Inputs         : mme_id - Pointer to the mme Index to be updated
*                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
*                  p_asn_msg - Pointer to asn decoded msg
*                  asn_msg_len - Length of the asn decoded msg
* Outputs        : None
* Returns        : None
* Description    : The function processes write-replace warning request Message
*****************************************************************************/
void s1ap_process_write_replace_warning_request(U8 mme_id,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len)
{
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn_msg);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    /* Calling the appropriate FSM */
    enb_mgr_fsm_write_replace_warning_process_event(
            ENB_EV_WRITE_REPLACE_WARNING_REQ,
            p_asn_msg,
            &asn_msg_len,
            p_s1ap_gb_ctx,
            p_s1ap_gb_ctx->mme_contexts[mme_id]);

    RRC_S1AP_UT_TRACE_EXIT();
}
/* etws_end*/

/*****************************************************************************
* Function Name  : s1ap_process_kill_request
* Inputs         : mme_id - Pointer to the mme Index to be updated
*                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
*                  p_asn_msg - Pointer to asn decoded msg
*                  asn_msg_len - Length of the asn decoded msg
* Outputs        : None
* Returns        : None
* Description    : The function processeses kil request
*****************************************************************************/
void s1ap_process_kill_request(U8 mme_id,
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len)
{
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn_msg);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    /* Calling the appropriate FSM */
    enb_mgr_fsm_write_replace_warning_process_event(
            ENB_EV_KILL_REQ,
            p_asn_msg,
            &asn_msg_len,
            p_s1ap_gb_ctx,
            p_s1ap_gb_ctx->mme_contexts[mme_id]);

    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
* Function Name  :  s1ap_mme_error_ind_proc_code_s1setup
* Inputs         :  p_s1ap_gb_ctx: Pointer to s1ap global context
*                   mme_id: Pointer to the mme Index to be updated
*                   p_error_indication: iPointer to parsed error indication msg 
* Outputs        :  None
* Returns        :  None
* Description    : This function handles the error indication received from 
*                  MME with procedure code S1Setup.  
****************************************************************************/
static void s1ap_mme_error_ind_proc_code_s1setup(
    s1ap_gb_context_t    *p_s1ap_gb_ctx,
    U8                   mme_id)/*SPR 17777 +-*/
{

    mme_context_t *p_mme_ctx = PNULL;  /* mme context at s1ap */
    rrc_return_et  response = RRC_FAILURE;

    RRC_S1AP_UT_TRACE_ENTER();
    /* L3 crash during debug mode - removed RRC_ASSERT  : 21691*/

    p_mme_ctx = p_s1ap_gb_ctx->mme_contexts[mme_id];

    if ((p_mme_ctx != PNULL ) && ( MME_W_S1_SETUP_RESP == MME_GET_FSM_STATE(p_mme_ctx)))
    {
        if (PNULL != p_mme_ctx->mme_timer_id)
        {
            rrc_stop_timer(p_mme_ctx->mme_timer_id);
            p_mme_ctx->mme_timer_id = PNULL;
        }

        p_mme_ctx->s1_config_retry_count = 0; 
        /*Change the state to MME_IDLE*/
        MME_SET_FSM_STATE(p_mme_ctx, MME_IDLE );
        RRC_S1AP_TRACE(RRC_INFO,"[%s]::Set State [MME_IDLE]",__FUNCTION__);

        /*Close the SCTP Association*/

        response = l3_sctp_close_association (p_mme_ctx->sctp_sd);

        if (RRC_FAILURE == response)
        {
            RRC_S1AP_TRACE(RRC_WARNING,"[%s]::ERROR in Closing Socket !!\n",
                    __FUNCTION__ );
        }

        ALARM_FMT_MSG_L3(RRC_MODULE_ID, RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID, 
                CRITICAL_ALARM, "MME IP Address = %s\n",
                p_mme_ctx->connection_info.ip_addr[0].ip_addr);

        delete_sctp_sd_from_tree(p_s1ap_gb_ctx,p_mme_ctx);
        p_mme_ctx->sctp_sd = INVALID_SCTP_SOCKET;
    }
    else
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "MME Context NULL at Index %d", mme_id);
    }
    
    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
* Function Name  : s1ap_mme_error_ind_proc_code_reset
* Inputs         :  p_s1ap_gb_ctx: Pointer to s1ap global context
*                   p_error_indication: iPointer to parsed error indication msg
* Outputs        : None
* Returns        : None
* Description    : This function handles the error indication received from 
*                  MME with procedure code RESET.  
*****************************************************************************/
static void s1ap_mme_error_ind_proc_code_reset(
        s1ap_gb_context_t *p_s1ap_gb_ctx)/*SPR 17777 +-*/
{
    reset_context_node_t *p_reset_context_node = PNULL;
    reset_context_node_t *p_reset_context_next_node = PNULL;
    U8                   mme_count = 0;

    RRC_S1AP_UT_TRACE_ENTER();


    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    /* L3 crash during debug mode - removed RRC_ASSERT  : 21691*/

    p_reset_context_next_node = (reset_context_node_t *)
        list_first_node(&p_s1ap_gb_ctx->
                reset_context_list);

    while(p_reset_context_next_node)
    {
        p_reset_context_node = p_reset_context_next_node;

        p_reset_context_next_node = (reset_context_node_t *)
            list_next_node(&p_reset_context_node->node);

        /* Stop the timer set for RESET message */
        if (p_reset_context_node->reset_context.timer_id)
        {
            rrc_stop_timer(p_reset_context_node->reset_context.timer_id);
            p_reset_context_node->reset_context.timer_id = PNULL;
        }

        for (mme_count = 0;
                mme_count < p_reset_context_node->reset_context.mme_count;
                mme_count++)
        {
            if (p_reset_context_node->reset_context.mme_reset_info[mme_count])
            {
                rrc_mem_free(p_reset_context_node->reset_context.
                        mme_reset_info[mme_count]);
                p_reset_context_node->reset_context.mme_reset_info
                    [mme_count] = PNULL;
            }
        }

        /* Delete S1AP contexts per MMEs RESET Info */
        for (mme_count = 0; mme_count < MAX_NUM_MME ; mme_count++)
        {
            if (p_reset_context_node->reset_context.mme_reset_info[mme_count])
            {
                if (!p_reset_context_node->reset_context.is_s1ap_ctx_deleted)
                {
                    delete_reset_ue_s1ap_ctx(p_s1ap_gb_ctx,
                            p_reset_context_node->reset_context.
                            mme_reset_info[mme_count],
                            mme_count,
                            p_reset_context_node->reset_context.is_ue_ctx_deleted);
                }

            }
        }

        /* Set the response to be sent to OAM to failure
         */
        p_s1ap_gb_ctx->s1ap_oam_tx_info.reset_response
            =  RRC_FAILURE;

        if ( RRC_MAX_EXT_MODULE_ID 
                >=  p_reset_context_node->reset_context.
                send_resp_to_module)
        {
            switch (p_reset_context_node->reset_context.
                    send_resp_to_module)
            {
                case RRC_OAM_MODULE_ID:
                    s1ap_build_and_send_oam_reset_resp(p_s1ap_gb_ctx);
                    RRC_TRACE(RRC_INFO,"%s RESPONSE Sent to "
                            "OAM\n",__FUNCTION__);
                    break;

                default:
                    RRC_TRACE(RRC_WARNING,"%s Resp to be sent to "
                            "UNKNOWN MODULE\n",__FUNCTION__);
            }
        }
        /* Remove the reset_context from global reset_context_list */
        list_delete_node(&p_s1ap_gb_ctx->reset_context_list,
                &p_reset_context_node->node);

        if (PNULL != p_reset_context_node)
        {
            rrc_mem_free((void*)p_reset_context_node);
            p_reset_context_node = PNULL;
        }
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
* Function Name  : s1ap_mme_error_ind_proc_code_enb_configuration_update 
* Inputs         : p_s1ap_gb_ctx: Pointer to s1ap global context
*                  mme_id: Pointer to the mme Index to be updated 
*                  p_error_indication: iPointer to parsed error indication msg
* Outputs        : None
* Returns        : None
* Description    : This function handles the error indication received from 
*                : MME with procedure code ENBConfigUpdate.
*****************************************************************************/
static void s1ap_mme_error_ind_proc_code_enb_configuration_update(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        U8 mme_id)/*SPR 17777 +-*/
{
    mme_context_t *p_mme_ctx = PNULL;  /* mme context at s1ap */
    enb_configuration_update_state_et enb_config_fsm_state = ENB_STATE_ENB_CONFIG_INVALID;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    /* L3 crash during debug mode - removed RRC_ASSERT  : 21691*/

    p_mme_ctx = p_s1ap_gb_ctx->mme_contexts[mme_id];
    /* Since none of the eNB config update will be in progess this time 
       set enb_config_updated_count to 0 */
    if (p_mme_ctx != PNULL)
    {
        enb_config_fsm_state = (enb_configuration_update_state_et)ENB_CONFIG_GET_FSM_STATE(p_mme_ctx);

        if (ENB_STATE_W_FOR_ENB_CONFIG_RESP == enb_config_fsm_state)
        {
            p_mme_ctx->enb_config_updated_count = 0;
            /* Set the state to ENB_STATE_ABNORMAL_ENB_CONFIG or 
               ENB_STATE_RECV_ENB_CONFIG_FROM_OAM  Currently,
               setting it to ENB_STATE_ABNORMAL_ENB_CONFIG, since don't want OAM to
               initiate same msg again */
            ENB_CONFIG_SET_FSM_STATE(p_mme_ctx, ENB_STATE_ABNORMAL_ENB_CONFIG);

            /*Stop enb_config_update_timer_id guard timer*/
            if (PNULL != p_mme_ctx->enb_config_update_timer_id)
            {
                RRC_S1AP_TRACE(RRC_INFO,"[%s]::Stop timer "
                        "[S1AP_ENB_CONFIGURAION_UPDATE_TIMER]",
                        __FUNCTION__);
                rrc_stop_timer(p_mme_ctx->enb_config_update_timer_id);
                p_mme_ctx->enb_config_update_timer_id = PNULL;
            }
            if (RRC_FAILURE == build_and_send_enb_config_update_resp_to_oam(
                        (U8)(p_mme_ctx->mme_id),
                        OAM_ENB_CONFIG_UPDATE_FAILURE,
                        /*SPR 17777 +-*/
                        p_mme_ctx->oam_transaction_id))
            {
                RRC_S1AP_TRACE(RRC_WARNING,"[OAM_ENB_CONFIG_UPDATE_FAILURE]"
                        "send failed");
            }
            if (PNULL != p_mme_ctx->p_enb_configuration_update)
            {
                rrc_mem_free(p_mme_ctx->p_enb_configuration_update);
                p_mme_ctx->p_enb_configuration_update = PNULL;
            }
        }
    }
    else
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "Either Procedure code in Error Indication is incorrect\
                 or MME Context NULL at Index %d", mme_id);
    }
    RRC_S1AP_UT_TRACE_EXIT();
}


