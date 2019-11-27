/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: s1ap_uecc.c,v 1.5 2010/10/13 07:04:25 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : This is the source file of the S1AP UECC.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: s1ap_uecc.c,v $
 * Revision 1.5  2010/10/13 07:04:25  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.4.4.21  2010/10/06 12:44:31  gur18569
 * fixed a wrong log
 *
 * Revision 1.4.4.20  2010/10/06 06:03:33  gur18569
 * updated a log
 *
 * Revision 1.4.4.19  2010/10/05 12:41:10  gur22047
 * compilation warning removed
 *
 * Revision 1.4.4.18  2010/10/04 13:53:59  gur04518
 * Removed TABS
 *
 * Revision 1.4.4.17  2010/10/04 13:31:54  gur04518
 * removed backslash
 *
 * Revision 1.4.4.16  2010/10/04 10:23:10  gur16768
 * removed context not found logs
 *
 * Revision 1.4.4.15  2010/10/04 07:07:11  gur18569
 * fixed ssit bugs
 *
 * Revision 1.4.4.14  2010/10/02 11:23:54  gur18569
 * fixed ssit bug
 *
 * Revision 1.4.4.13  2010/09/22 15:12:58  gur25381
 * updated for kclockwork warning fixes
 *
 * Revision 1.4.4.12  2010/09/09 10:11:31  gur22047
 * modified_during_s1ap_UT added_stream_id
 *
 * Revision 1.4.4.11  2010/08/30 10:46:54  gur22047
 * files_modified during UT
 *
 * Revision 1.4.4.10  2010/08/16 13:20:58  gur22047
 * lokesh_review_incorporate
 *
 * Revision 1.4.4.9  2010/08/16 12:41:16  gur22047
 * lokesh_review_incorporate
 *
 * Revision 1.4.4.8  2010/08/16 12:10:57  gur22047
 * lokesh_review_incorporate
 *
 * Revision 1.4.4.7  2010/08/05 08:10:44  gur15697
 * merging round 2
 *
 * Revision 1.4.4.6  2010/07/12 10:15:37  gur22047
 * code review comments incorporate
 *
 * Revision 1.4.4.5  2010/07/01 09:59:43  gur22047
 * s1ap changes
*
* Revision 1.4.4.4  2010/06/30 12:37:05  gur22047
* s1ap changes
*
* Revision 1.4.4.3  2010/06/29 06:18:35  gur22047
* internal source and s1ap code
*
* Revision 1.4.4.2  2010/06/24 13:01:29  gur22047
* internal source and s1ap code
*
* Revision 1.4.4.1  2010/06/22 05:32:55  gur22047
* Internal Interface update
*
* Revision 1.4  2010/04/06 04:04:04  gur18569
* reset fwk chg
*
* Revision 1.3  2010/03/24 09:50:45  gur18569
* Merged files from Rel 1.0 support branch
*
* Revision 1.2  2010/01/04 16:10:05  ukr15916
* no message
*
* Revision 1.1.2.20  2009/12/28 11:18:17  gur21006
* Cell id validation check added
*
* Revision 1.1.2.19  2009/12/28 10:14:01  gur21006
* system calls removed
*
* Revision 1.1.2.18  2009/12/28 05:09:42  gur18569
* fixed indentation
*
* Revision 1.1.2.17  2009/12/27 08:39:04  gur18569
* removed tabs
*
* Revision 1.1.2.16  2009/12/26 12:59:34  gur21006
* validation check added
*
* Revision 1.1.2.15  2009/12/26 12:05:01  gur21006
* Warnings removed
*
* Revision 1.1.2.14  2009/12/23 11:05:20  gur20470
* reverted back to 1.1.2.12 and made changes in flag L3_SSIT_UPD_SETUP
*
* Revision 1.1.2.12  2009/12/23 06:42:20  gur21006
* Warning Removed
*
* Revision 1.1.2.11  2009/12/22 14:36:49  gur18569
* fixed 154
*
* Revision 1.1.2.10  2009/12/19 03:50:33  gur18569
* added s1ap_delete_context
*
* Revision 1.1.2.9  2009/12/18 04:23:11  gur18569
* checking ue id pair presence : spr 183
*
* Revision 1.1.2.8  2009/12/16 09:53:19  gur18569
* fixed SPR:179
*
* Revision 1.1.2.7  2009/12/09 15:17:29  gur18569
* removing case of ASN1V_s1ap_id_InitialContextSetup as Only UE context release complete is handled
*
* Revision 1.1.2.6  2009/12/08 15:11:08  gur18569
* Fixed SPR:162
*
* Revision 1.1.2.5  2009/12/08 14:13:13  gur18569
* using message id from ASN files SPR:161
*
* Revision 1.1.2.4  2009/12/04 13:55:02  gur18569
* changes in mme allocation logic
*
* Revision 1.1.2.3  2009/12/03 10:11:36  gur18569
* integration changes
*
* Revision 1.1.2.2  2009/11/26 18:33:49  ukr16018
* Merge S1AP and RRC (from branch dev_rel_1_0).
*
* Revision 1.1.2.1  2009/11/25 13:11:05  gur18569
* Shifted to level of rrc dir
*
* Revision 1.1.2.12  2009/11/23 10:27:30  gur18569
* fixed ut bug
*
* Revision 1.1.2.11  2009/11/22 11:46:54  gur18569
* ut fixes
*
* Revision 1.1.2.10  2009/11/20 08:42:18  gur18569
* changed l3_sctp_send handling
*
* Revision 1.1.2.9  2009/11/18 13:36:06  gur20470
* Modified type of p_node
*
* Revision 1.1.2.8  2009/11/18 12:05:39  gur20470
* Incorporated review comments
*
* Revision 1.1.2.7  2009/11/17 14:13:57  gur20470
* Incorporated review comments
*
* Revision 1.1.2.6  2009/11/17 10:19:44  gur20052
*  Review Comments are incorporated
*
* Revision 1.1.2.5  2009/11/09 09:18:23  gur18569
* removed compilation warnings
*
* Revision 1.1.2.4  2009/10/29 17:30:39  gur20052
*  Updated for API and message ID handling
*
* Revision 1.1.2.3  2009/10/29 15:26:28  gur20052
*  Added Error handling for l3_sctp_send
*
* Revision 1.1.2.2  2009/10/23 16:09:56  gur18569
* removed compilation errors
*
* Revision 1.1.2.1  2009/10/23 11:42:23  gur20052
* Initial Version Added
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
#include "s1ap_uecc.h"
#include "s1ap_api.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "rrc_s1ap_asn_intrl_enc_3gpp.h"
#include "rrc_s1ap_asn_intrl_dec_3gpp.h"
#include "rrc_common_md.h"
#include "rrc_logging.h"
/*SPR 8708 Fix Start*/
#include "rrc_common_utils.h"
/*SPR 8708 Fix Stop*/

/****************************************************************************
  Private Definitions
 ****************************************************************************/


/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
void  process_uecc_ue_release_req(
        s1ap_gb_context_t * p_s1ap_gb_ctx,  
        U8 * p_api);

void  process_uecc_failure_msg_req(
        s1ap_gb_context_t * p_s1ap_gb_ctx,  
        U8 * p_api);

void  s1ap_process_location_report_message_to_mme(
        void*  p_asn_msg,
        U32    asn_msg_len,
        U8     mme_id,
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        ue_s1ap_context_t    *p_s1ap_context);

ho_ongoing_ue_ctx_node_t *search_ho_list_node(
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        s1ap_ue_associated_sig_msg_req_t *p_ue_associated_sig_req);

ho_ongoing_ue_ctx_node_t *search_ho_list_node_to_be_released(
        s1ap_gb_context_t       *p_s1ap_gb_ctx,
        s1ap_ue_release_ind_t   *p_ue_release_ind);

void process_uecc_ho_ongoing_ue_rel_ind(
        s1ap_gb_context_t * p_s1ap_gb_ctx,  
        U8 * p_api);

void  delete_ue_ctx_from_ho_temp_list(
        s1ap_gb_context_t       * p_s1ap_gb_ctx,  
        mme_ue_s1ap_id_list_t   * mme_ue_s1ap_id_list);

rrc_return_et build_and_send_allocate_mme_resp(
        U32 enb_ue_s1ap_id,
        U8  rel_of_mme,
/*BUG 604 changes start*/
        U8  mme_id,
        U16 ue_index,
/* mme grp id feature changes start */
        U8 matching_gummei_to_be_sent);
/* mme grp id feature changes start */
/*BUG 604 changes stop*/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/
#define NO_MATCHING_MME              0
#define PARTIAL_MATCHING_MME         1
#define MATCHING_MME                 2
#define PARTIAL_MATCHING_MMEGI_MME   3
/* SPR 22789 Fix Start */ 
/* Code Deleted */
/* SPR 22789 Fix End */ 
#define NO_MATCHING_GUMMEI           0
#define MATCHING_GUMMEI              1
/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Local Functions Prototypes
 ****************************************************************************/
U8  match_registered_mme_info ( 
        s1ap_gb_context_t* p_s1ap_gb_ctx, 
        mme_context_t* p_mme_ctx,
        s1ap_allocate_mme_req_t * p_alloc_mme_req);

/*SPR 8708 Fix Start*/
U8  match_registered_mme_info_by_mmec (
        /*SPR 17777 +-*/
        mme_context_t* p_mme_ctx,
        s1ap_allocate_mme_req_t * p_alloc_mme_req);
/*SPR 8708 Fix Stop*/
U8  match_overload_start_gummei_info(
        s1ap_gummei_t* p_gummei_list,
        U16     num_gummei,
        s1ap_allocate_mme_req_t * p_alloc_mme_req,
        mme_context_t* p_mme_ctx);

/*REL 1.2:HANDOVER PROCEDURE START*/                        
/****************************************************************************
 * Functions implementation
 ****************************************************************************/
/****************************************************************************
 * Function Name  : s1ap_uecc_process_ue_context_release
 * Inputs         : p_s1ap_gb_ctx -Pointer to the UECC global context data
 *                  p_ue_associated_sig_req - Pointer to ue_assoc_sig_req
 *                  cell_id - Cell identifier
 *                  ue_id  - UE_Index
 *                  p_message - Message pointer
 *                  message_buff_len - Message buffer length
 * Outputs        : None
 * Returns        : RRC_FAILURE / RRC_SUCCESS
 * Description    : This function processes s1ap_ue_associated_sig_msg_req
 *                  message for s1ap_UEContextRelease
 ****************************************************************************/
static rrc_return_et s1ap_uecc_process_ue_context_release(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        s1ap_ue_associated_sig_msg_req_t *p_ue_associated_sig_req,
        U8 cell_id, U16 ue_id, U8 *p_message, U32 message_buff_len)
{
	ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL;
    /*SPR 21219 25Jan FIX START*/
	U16 stream_id     = INVALID_NUM_SCTP_STREAM;
    /*SPR 21219 25Jan FIX STOP*/
	mme_context_t * p_mme_ctx           = PNULL;
	ue_s1ap_context_t   *p_s1ap_ue_context = PNULL;
	U8 ho_flag = RRC_FALSE;
	search_tree_node_t * p_node = PNULL;

	RRC_S1AP_UT_TRACE_ENTER();

	get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
			&p_s1ap_ue_context,
			p_ue_associated_sig_req->enb_ue_s1ap_id);

	if (p_s1ap_ue_context == PNULL)                    
	{
		if(UE_ASSOC_SIG_MSG_REQ_MME_ID_PRESENT
				& p_ue_associated_sig_req->bitmask)
		{
			p_mme_ctx = p_s1ap_gb_ctx->
				mme_contexts[p_ue_associated_sig_req->mme_id];
			p_ho_ongoing_ue_ctx_node = 
				search_ho_list_node (p_s1ap_gb_ctx,
						p_ue_associated_sig_req);

			if(PNULL != p_ho_ongoing_ue_ctx_node)
			{
				stream_id = p_ho_ongoing_ue_ctx_node->
					ho_ongoing_ue_ctx.stream_id;

				list_delete_node(&p_s1ap_gb_ctx->
						ho_ongoing_ue_ctx_list,
						(void *)p_ho_ongoing_ue_ctx_node);

				rrc_mem_free((void*)
						p_ho_ongoing_ue_ctx_node);
				/* + Coverity 81493 */
				//p_ho_ongoing_ue_ctx_node = PNULL;
				/* - Coverity 81493 */
				RRC_S1AP_TRACE(RRC_INFO,
						"[%s] HO ONGOING Temporary UE Context "
						"DELETED on UE Context Release",
						__FUNCTION__);
                /*BUG 604 changes start*/
                /*Free s1ap_ue_id Ctx*/
                s1ap_ueid_free(p_s1ap_gb_ctx,
                        p_ue_associated_sig_req->enb_ue_s1ap_id);
                /*BUG 604 changes stop*/

				ho_flag = RRC_TRUE;
			}
		}
		else
		{
			RRC_S1AP_TRACE(RRC_WARNING, 
					"UE_CONTEXT_RELEASE_COMPLETE received "
					"without MME Id when context does not "
					"exists at S1AP for enb_ue_s1ap_id = "
					"%u. Dropping message.",
					p_ue_associated_sig_req->
					enb_ue_s1ap_id );
			RRC_S1AP_UT_TRACE_EXIT();
			return RRC_FAILURE;
		}
	}
	if (ho_flag != RRC_TRUE )
	{
		if (PNULL != p_s1ap_ue_context)
		{
			stream_id =  p_s1ap_ue_context->stream_id;
			p_mme_ctx = p_s1ap_gb_ctx->
				mme_contexts[p_s1ap_ue_context->mme_id];
			/*delete entry from  from search tree*/
			p_node = st_find_tree_node (
					(search_tree_t *)&
					(p_mme_ctx->mmeUeS1apId_search_tree),
					(void*)(&p_s1ap_ue_context->mme_ue_s1ap_id) ); 

			if (!p_node)
			{
				RRC_S1AP_TRACE(RRC_WARNING,
						"[%s] Entry not found in search tree\n",
						__FUNCTION__);
			}
			else
			{
				st_delete_node(
						(search_tree_t *)&
						(p_mme_ctx->mmeUeS1apId_search_tree),
						(search_tree_node_t *) p_node); 
				rrc_mem_free((void*)p_node);
				/* + coverity 81493 */
				//p_node = PNULL;
				/* - coverity 81493 */
			}

            /* SPR 22789 Fix Start */
            RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx,p_s1ap_ue_context,p_ue_associated_sig_req->enb_ue_s1ap_id);
            /*BUG 604 changes start*/
            /*Free s1ap_ue_id Ctx*/
            s1ap_ueid_free(p_s1ap_gb_ctx,p_ue_associated_sig_req->enb_ue_s1ap_id);
            /*BUG 604 changes stop*/
            RRC_S1AP_TRACE(RRC_INFO,
                    "[%s] Deleted entry at cellid %d ueid %d\n",
                    __FUNCTION__,
                    cell_id,ue_id);
            /* SPR 22789 Fix End */
        }
        /* SPR 22789 Fix Start */
        /* Code Moved */
        /* SPR 22789 Fix Start */
    }
	if (p_mme_ctx->current_mme_state != MME_ACTIVE)
	{
		RRC_S1AP_TRACE(RRC_WARNING, "%s:"
				" MME context with id %d is not active.",
				__FUNCTION__,p_mme_ctx->mme_id);
		RRC_S1AP_UT_TRACE_EXIT();
		return RRC_FAILURE;
	}
    /*SPR 21219 25Jan FIX START*/
	else if (INVALID_NUM_SCTP_STREAM != stream_id)
    /*SPR 21219 25Jan FIX STOP*/
	{
		if (SCTP_SOCKET_ERROR ==l3_sctp_send(
					p_mme_ctx->sctp_sd,
					p_message,
					message_buff_len,
					stream_id))
		{
			RRC_S1AP_TRACE(RRC_WARNING, "%s:"
					" l3_sctp_send send failed ue ctx rel",
					__FUNCTION__);
			RRC_S1AP_UT_TRACE_EXIT();
			return RRC_FAILURE;
		}
	}


	RRC_S1AP_UT_TRACE_EXIT();
	return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : s1ap_uecc_process_handover_resource_alloc
 * Inputs         : p_s1ap_gb_ctx -Pointer to the UECC global context data
 *                  p_ue_associated_sig_req - Pointer to ue_assoc_sig_req
 *                  p_message - Message pointer
 *                  message_buff_len - Message buffer length
 * Outputs        : None
 * Returns        : RRC_FAILURE / RRC_SUCCESS
 * Description    : This function processes s1ap_ue_associated_sig_msg_req
 *                  message for s1ap_HandoverResourceAllocation
 ****************************************************************************/
static rrc_return_et s1ap_uecc_process_handover_resource_alloc(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        s1ap_ue_associated_sig_msg_req_t *p_ue_associated_sig_req,
        U8 cell_id, U8 *p_message, U32 message_buff_len)
{
    ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL;
    s1ap_message_type_et   message_type;
    s1ap_ENB_UE_S1AP_ID enb_ue_s1ap_id = MAX_ENB_UE_S1AP_ID;
    ue_s1ap_context_t * p_s1ap_ctx      = PNULL;
    s1ap_MME_UE_S1AP_ID mme_ue_s1ap_id = MAX_MME_UE_S1AP_ID;
    U8 ue_id_pair_presence = RRC_NULL;
    U32 mme_id       = MAX_NUM_MME;
    U16 stream_id     = MAX_NUM_SCTP_STREAM;
    mme_context_t * p_mme_ctx           = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    p_ho_ongoing_ue_ctx_node = 
        search_ho_list_node (p_s1ap_gb_ctx,
                p_ue_associated_sig_req);

    if(PNULL == p_ho_ongoing_ue_ctx_node)
    {
        RRC_S1AP_TRACE(RRC_FATAL,
                "[%s]: HO ongoing UE context "
                "node doesn't exist ",
                __FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    message_type = p_ue_associated_sig_req->message_type;

    switch (p_ue_associated_sig_req->message_type)
    {
        case SUCCESSFUL_OUTCOME:
            {
                enb_ue_s1ap_id=
                    p_ue_associated_sig_req->enb_ue_s1ap_id;

                p_s1ap_ctx = (ue_s1ap_context_t *)
                    rrc_mem_get(sizeof(ue_s1ap_context_t));

                if(PNULL == p_s1ap_ctx)
                {
                    RRC_S1AP_TRACE(RRC_FATAL, 
                            "rrc_mem_get failed!!");
                    list_delete_node(&p_s1ap_gb_ctx->
                            ho_ongoing_ue_ctx_list,
                            (void *)p_ho_ongoing_ue_ctx_node);
                    rrc_mem_free((void*)
                            p_ho_ongoing_ue_ctx_node);
                    /* + Coverity 81350 */
                    //p_ho_ongoing_ue_ctx_node = PNULL;
                    /* - Coverity 81350 */
                    RRC_S1AP_TRACE(RRC_INFO,
                            "%s HO ONGOING Temporary UE Context "
                            "DELETED at SUCCESSFUL_OUTCOME "
                            "due to s1ap ctx is not present",
                            __FUNCTION__);
                    RRC_S1AP_UT_TRACE_EXIT();
                    return RRC_FAILURE;
                }

                RRC_S1AP_TRACE(
                        RRC_INFO,
                        "Processing MME Messages :"
                        " for HandoverResourceAllocation\n");

                mme_ue_s1ap_id =
                    p_ho_ongoing_ue_ctx_node->
                    ho_ongoing_ue_ctx.mmeUeS1apId;
                ue_id_pair_presence |= 
                    ENB_UE_S1AP_ID_PRESENT |
                    MME_UE_S1AP_ID_PRESENT;

                /* get mme_id and  stream_id */
                mme_id = p_ho_ongoing_ue_ctx_node->
                    ho_ongoing_ue_ctx.mme_id;
                stream_id = p_ho_ongoing_ue_ctx_node->
                    ho_ongoing_ue_ctx.stream_id;

                /* SPR 22789 Fix Start */
                /* Code Moved */
                /* SPR 22789 Fix End */
                /* SPR 19286 Fix Start */
                if( MAX_NUM_CELLS > cell_id)
                {
                    p_s1ap_ctx->cell_index = cell_id;
                }
                else
                {
                    RRC_S1AP_TRACE(RRC_INFO,"[%s]: "
                            "CELL id %d is not in valid range (%d)"
                            "Hence setting the cell index in S1AP_CONTEXT to First cell index i.e zero\n"
                            ,__FUNCTION__,
                            cell_id,MAX_NUM_CELLS);

                    p_s1ap_ctx->cell_index = RRC_NULL;
                }    
                /* SPR 19286 Fix End */


                p_mme_ctx = 
                    p_s1ap_gb_ctx->mme_contexts[mme_id];

                p_s1ap_ctx->mme_ue_s1ap_id = mme_ue_s1ap_id;
                p_s1ap_ctx->is_mme_ue_s1ap_id_valid = 
                    RRC_FALSE;

                p_s1ap_ctx->stream_id = stream_id;
                p_s1ap_ctx->mme_id =  p_ho_ongoing_ue_ctx_node->
                    ho_ongoing_ue_ctx.mme_id;
                /*BUG 604 changes start*/
                /*Fill ue_index in ue_s1ap_ctx*/
                p_s1ap_ctx->ue_index = p_ue_associated_sig_req->ue_index;
                /*Fill enb_ue_s1ap_id in ue_s1ap_ctx*/
                p_s1ap_ctx->enb_ue_s1ap_id = 
                    p_ue_associated_sig_req->enb_ue_s1ap_id;
                /*BUG 604 changes stop*/

                /* SPR 22789 Fix Start */
                set_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                        p_s1ap_ctx,
                        enb_ue_s1ap_id);
                /* SPR 22789 Fix End */

                s1ap_process_dl_nas_msg(
                        p_s1ap_gb_ctx,
                        &p_s1ap_ctx,
                        /*BUG 604 changes start*/
                        p_ue_associated_sig_req->enb_ue_s1ap_id,
                        /*BUG 604 changes stop*/
                        mme_ue_s1ap_id,
                        p_mme_ctx,
                        PNULL,
                        0,
                        ue_id_pair_presence,
                        stream_id,
                        INVALID_S1AP_PROCEDURE_CODE,
                        message_type);

                if ( MME_ACTIVE != p_mme_ctx->
                        current_mme_state)
                {
                    /*MME is not Active         *
                     * hence Discard the message*/
                    RRC_S1AP_TRACE(RRC_INFO,
                            "[%s]:MME is not Active "
                            "hence Discard the message\n"
                            ,__FUNCTION__); 
                    list_delete_node(&p_s1ap_gb_ctx->
                            ho_ongoing_ue_ctx_list,
                            (void *)p_ho_ongoing_ue_ctx_node);
                    /* SPR 3840 starts */
                    /*BUG 604 changes start*/
                    build_and_send_s1ap_ue_rel_req_int(
                            p_ue_associated_sig_req->enb_ue_s1ap_id,
                            p_s1ap_ctx->ue_index);
                    /*BUG 604 changes stop*/
                    /* SPR 3840 ends */

                    if (PNULL != p_ho_ongoing_ue_ctx_node)
                    {
                        rrc_mem_free((void*)
                                p_ho_ongoing_ue_ctx_node);
                        /* + Coverity 81350 */
                        //p_ho_ongoing_ue_ctx_node = PNULL;
                        /* - Coverity 81350 */
                        RRC_S1AP_TRACE(RRC_INFO,
                                "[%s] HO ONGOING Temporary UE Context "
                                "DELETED at SUCCESSFUL_OUTCOME if "
                                "current_mme_state is not MME_ACTIVE",
                                __FUNCTION__);
                    }
                    RRC_S1AP_UT_TRACE_EXIT();
                    return RRC_FAILURE;
                }              
                if (SCTP_SOCKET_ERROR ==l3_sctp_send(
                            p_mme_ctx->sctp_sd,
                            p_message,
                            message_buff_len,
                            stream_id))
                {
                    RRC_S1AP_TRACE(RRC_WARNING, "%s:"
                            " l3_sctp_send send failed",
                            __FUNCTION__);
                }

                /* Delete ho_ongoing_ue_ctx from 
                 * the ho_ongoing_ue_ctx_list */
                list_delete_node(&p_s1ap_gb_ctx->
                        ho_ongoing_ue_ctx_list,
                        (void *)p_ho_ongoing_ue_ctx_node);

                if (PNULL != p_ho_ongoing_ue_ctx_node)
                {
                    rrc_mem_free((void*)
                            p_ho_ongoing_ue_ctx_node);
                    /* + Coverity 81350 */
                    //p_ho_ongoing_ue_ctx_node = PNULL;
                    /* - Coverity 81350 */
                    RRC_S1AP_TRACE(RRC_INFO,
                            "[%s]HO ONGOING Temporary UE Context "
                            "DELETED at SUCCESSFUL_OUTCOME",
                            __FUNCTION__);
                }
                break;
            }   
        case UNSUCCESSFUL_OUTCOME:
            { 
                /* get mme_id and  stream_id */
                mme_id = p_ho_ongoing_ue_ctx_node->
                    ho_ongoing_ue_ctx.mme_id;
                stream_id = p_ho_ongoing_ue_ctx_node->
                    ho_ongoing_ue_ctx.stream_id;
                p_mme_ctx = 
                    p_s1ap_gb_ctx->mme_contexts[mme_id];

                if (SCTP_SOCKET_ERROR ==l3_sctp_send(
                            p_mme_ctx->sctp_sd,
                            p_message,
                            message_buff_len,
                            stream_id))
                {
                    RRC_S1AP_TRACE(RRC_WARNING, "%s:"
                            " l3_sctp_send send failed",
                            __FUNCTION__);
                }

                /* Delete ho_ongoing_ue_ctx from the 
                 * ho_ongoing_ue_ctx_list */
                list_delete_node(&p_s1ap_gb_ctx->
                        ho_ongoing_ue_ctx_list,
                        (void *)p_ho_ongoing_ue_ctx_node);
                if (PNULL != p_ho_ongoing_ue_ctx_node)
                {
                    /*BUG 604 changes start*/
                    /*Free s1ap_ue_id Ctx*/
                    s1ap_ueid_free(p_s1ap_gb_ctx,
                            p_ue_associated_sig_req->enb_ue_s1ap_id);
                    /*BUG 604 changes stop*/
                    rrc_mem_free(
                            (void*)p_ho_ongoing_ue_ctx_node);
                    /* + Coverity 81350 */
                    //p_ho_ongoing_ue_ctx_node = PNULL;
                    /* - Coverity 81350 */
                    RRC_S1AP_TRACE(RRC_INFO,
                            "[%s]HO ONGOING Temporary UE Context "
                            "DELETED at UNSUCCESSFUL_OUTCOME",
                            __FUNCTION__);
                }
                break;
            }
        default:
            {
                RRC_S1AP_TRACE(RRC_ERROR, 
                        "[%s]Processing HO RESOURCE ALLOCATION"
                        "Messages : Unknown Outcome ::%d\n",
                        __FUNCTION__,
                        p_ue_associated_sig_req->message_type);    
                list_delete_node(&p_s1ap_gb_ctx->
                        ho_ongoing_ue_ctx_list,
                        (void *)p_ho_ongoing_ue_ctx_node);
                RRC_S1AP_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
    }
    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : s1ap_uecc_process_ue_assoc_sig_msg_req
 * Inputs         : p_s1ap_gb_ctx -Pointer to the UECC global context data
 *                  p_alloc_mme_req - Pointer to s1ap_allocate_mme_req_t
 *                  p_api - Pointer to buffer containing input API
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes s1ap_ue_associated_sig_msg_req
 *                  message
 ****************************************************************************/
static void s1ap_uecc_process_ue_assoc_sig_msg_req(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        s1ap_allocate_mme_req_t   *p_alloc_mme_req,
        U8    *p_api)
{
    s1ap_ue_associated_sig_msg_req_t *p_ue_associated_sig_req = PNULL;
    U8 cell_id      = MAX_NUM_CELLS;
    U16 ue_id       = MAX_NUM_UE;
    U8 *p_message    = PNULL;
    U32 message_buff_len= 0;
    mme_context_t * p_mme_ctx           = PNULL;
    U16 stream_id     = MAX_NUM_SCTP_STREAM;
    ue_s1ap_context_t   *p_s1ap_ue_context = PNULL;
    ue_s1ap_context_t * p_s1ap_ctx      = PNULL;
    rrc_return_et cause;
    U32 mme_id       = MAX_NUM_MME;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_S1AP_TRACE(RRC_DETAILED,"Unused variable in fsm :%p",p_alloc_mme_req);

    p_ue_associated_sig_req = (s1ap_ue_associated_sig_msg_req_t *)p_api;
    if((p_ue_associated_sig_req->procedure_code != 
                ASN1V_s1ap_id_initialUEMessage )
            && ( p_ue_associated_sig_req->procedure_code != 
                ASN1V_s1ap_id_HandoverResourceAllocation )
            && ( p_ue_associated_sig_req->procedure_code != 
                ASN1V_s1ap_id_UEContextRelease )
            && ( p_ue_associated_sig_req->procedure_code != 
                ASN1V_s1ap_id_PathSwitchRequest ))
    {
        cell_id     = s1ap_get_cell_id
            (p_s1ap_gb_ctx, p_ue_associated_sig_req->enb_ue_s1ap_id);
    }
    else
    {
        cell_id = p_ue_associated_sig_req->cell_index;
    }
    /* SPR 19929 Fix Start*/
    if (p_ue_associated_sig_req->procedure_code != ASN1V_s1ap_id_HandoverResourceAllocation)
    {
        ue_id       = s1ap_get_ue_id
            (p_ue_associated_sig_req->enb_ue_s1ap_id);
        if (ue_id >= MAX_NUM_UE)
        {
            RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Error ::"
                    "UE id %d is invalid \n"
                    ,__FUNCTION__,ue_id);
            RRC_S1AP_UT_TRACE_EXIT();
            return ;
        }
        RRC_S1AP_TRACE(RRC_INFO,"[%s]:enbUeS1apId "
                "%d CELL id is %d UE id %d"
                " \n ",__FUNCTION__,
                p_ue_associated_sig_req->enb_ue_s1ap_id,
                cell_id,ue_id);
    }

    SET_CELL_AND_UE_INDEX(cell_id, ue_id);

    p_message    = &p_ue_associated_sig_req->message_buff_p[0] ;
    message_buff_len = p_ue_associated_sig_req->message_buff_size;

    if (cell_id >= MAX_NUM_CELLS || 
            !p_s1ap_gb_ctx->cell_s1ap_contexts[cell_id])
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Error ::"
                "Cell id %d is invalid \n"
                ,__FUNCTION__,cell_id);
        RRC_S1AP_UT_TRACE_EXIT();
        return ;
    }

    /* SPR 19929 Fix Stop*/
    /*switch (p_ue_associated_sig_req->message_id)*/
    switch (p_ue_associated_sig_req->procedure_code)
    {
        case ASN1V_s1ap_id_initialUEMessage:
            { 
                RRC_S1AP_TRACE(RRC_INFO,
                        "[%s]:ASN1V_s1ap_id_initialUEMessage recvd"
                        " \n ",__FUNCTION__);

                get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                        &p_s1ap_ctx,
                        p_ue_associated_sig_req->enb_ue_s1ap_id);
                if (PNULL == p_s1ap_ctx)
                {
                    RRC_S1AP_TRACE(RRC_ERROR,
                            "[%s]:Error ::UE Context Not"
                            " Present\n ",__FUNCTION__);
                    /*BUG 604 changes start*/
                    s1ap_ueid_free(p_s1ap_gb_ctx,
                            p_ue_associated_sig_req->enb_ue_s1ap_id);
                    build_and_send_s1ap_ue_rel_req_int(
                            p_ue_associated_sig_req->enb_ue_s1ap_id,
                            p_ue_associated_sig_req->ue_index);
                    /*BUG 604 changes stop*/
                    RRC_S1AP_UT_TRACE_EXIT();
                    return ;
                }

                p_mme_ctx = p_s1ap_gb_ctx->mme_contexts[p_s1ap_ctx->mme_id];
                stream_id = p_s1ap_ctx->stream_id;

                /*Sending the s1ap context fields in Gobal Context */
                /* SPR 22789 Fix Start */
                RRC_S1AP_TRACE(RRC_INFO,"Sending Initial UE Message for ue_id %d to MME %u",
                        ue_id, p_s1ap_ctx->mme_id);
                /* SPR 22789 Fix End */
#ifndef LINUX_PC_TEST						
                if (SCTP_SOCKET_ERROR ==l3_sctp_send(
                            p_mme_ctx->sctp_sd,
                            p_message,
                            message_buff_len,
                            stream_id))
#else
				if(SCTP_SOCKET_ERROR ==pctest_forward_mme_message_to_pc(
							RRC_S1AP_MODULE_ID,
							RRC_MME_MODULE_ID,
							MME_MESSAGE_PS_TO_TEST,
							p_mme_ctx->sctp_sd,
							p_mme_ctx->stream_info.stream_ids[SCTP_COMMON_PROC_STREAMS], //stream id
							p_message,
							message_buff_len))

#endif
                {
                    RRC_S1AP_TRACE(RRC_WARNING, "%s:"
                            " l3_sctp_send send failed initial ue msg",
                            __FUNCTION__);
                    /*BUG 604 changes start*/
                    s1ap_ueid_free(p_s1ap_gb_ctx,
                            p_ue_associated_sig_req->enb_ue_s1ap_id);
                    build_and_send_s1ap_ue_rel_req_int(
                            p_ue_associated_sig_req->enb_ue_s1ap_id,
                            p_s1ap_ctx->ue_index);
                    /*BUG 604 changes stop*/
                    /*SPR_21818_START*/
                    /* SPR 22789 Fix Start */
                    RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx,p_s1ap_ctx,p_ue_associated_sig_req->enb_ue_s1ap_id);
                    /* SPR 22789 Fix End */
                    /*SPR_21818_END*/
                    RRC_S1AP_UT_TRACE_EXIT();
                    return;
                }
                RRC_S1AP_TRACE(RRC_INFO,"[%s]:Sending "
                        "S1AP Update UE Associated Msg Ind for "
                        "ue_id %d",
                        __FUNCTION__,ue_id);
                /*BUG 604 changes start*/
                if( RRC_FAILURE == 
                        s1ap_build_and_send_ue_associated_mme_update_status_ind(
                            p_ue_associated_sig_req->enb_ue_s1ap_id,
                            p_mme_ctx,p_s1ap_ctx->ue_index))
                    /*BUG 604 changes stop*/
                {
                    RRC_S1AP_TRACE(RRC_WARNING, "%s:"
                            " rrc_send_message send failed update \
                            ue associated mesg ind",
                            __FUNCTION__);
                    RRC_S1AP_UT_TRACE_EXIT();
                    return;
                }
                break;
            }
        case ASN1V_s1ap_id_UEContextRelease:
            {
                RRC_S1AP_TRACE(RRC_INFO,"[%s]:"
                        " UE_CONTEXT_RELEASE_COMPLETE recvd \n "
                        ,__FUNCTION__);

                if(RRC_SUCCESS != s1ap_uecc_process_ue_context_release(
                            p_s1ap_gb_ctx, p_ue_associated_sig_req,
                            cell_id, ue_id,
                            p_message, message_buff_len))
                {
                    RRC_S1AP_UT_TRACE_EXIT();
                    return;
                }
                break;
            }
        case ASN1V_s1ap_id_HandoverResourceAllocation:
            { 
                RRC_S1AP_TRACE(RRC_INFO,
                        "[%s]:ASN1V_s1ap_id_HandoverResourceAllocation" 
                        "recvd \n ",__FUNCTION__);

                if(RRC_SUCCESS != s1ap_uecc_process_handover_resource_alloc(
                            p_s1ap_gb_ctx, p_ue_associated_sig_req, 
                            cell_id, p_message, message_buff_len))
                {
                    RRC_S1AP_UT_TRACE_EXIT();
                    return;
                }

                break;
            }

        case ASN1V_s1ap_id_LocationReport :
            {
                RRC_S1AP_TRACE(RRC_INFO,
                        "[%s]Processing LOCATION REPORT",
                        __FUNCTION__);

                /* SPR 2793 Start */

                get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                        &p_s1ap_ctx,
                        p_ue_associated_sig_req->enb_ue_s1ap_id);

                if (PNULL == p_s1ap_ctx)
                {
                    RRC_S1AP_TRACE(RRC_ERROR,
                            "[%s]p_s1ap_ctx NULL IN LOCATION REPORT ",
                            __FUNCTION__);
                    RRC_S1AP_UT_TRACE_EXIT();
                    return;
                }

                s1ap_process_location_report_message_to_mme(
                        p_message,
                        message_buff_len,
                        p_ue_associated_sig_req->mme_id,
                        p_s1ap_gb_ctx,
                        p_s1ap_ctx);
                break;
            }

        case ASN1V_s1ap_id_LocationReportingFailureIndication :
            {
				if (PNULL == p_s1ap_ctx)
				{
					RRC_S1AP_TRACE(RRC_ERROR,
							"[%s]p_s1ap_ctx NULL IN LOCATION REPORT ",
							__FUNCTION__);
					RRC_S1AP_UT_TRACE_EXIT();
					return;
				}
				RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: LocationReport eNB-S1AP-Id:%u MME-S1AP-Id:%u %s",
				        p_ue_associated_sig_req->enb_ue_s1ap_id, p_s1ap_ctx->mme_ue_s1ap_id, change_data_to_str(message_buff_len, p_message));

                /* SPR 2793 Start */
                get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                        &p_s1ap_ctx,
                        p_ue_associated_sig_req->enb_ue_s1ap_id);

                if (PNULL == p_s1ap_ctx)
                {
                    RRC_S1AP_TRACE(RRC_ERROR,
                            "[%s]p_s1ap_ctx NULL IN LOCATION REPORT FAILURE",
                            __FUNCTION__);
                    RRC_S1AP_UT_TRACE_EXIT();
                    return;
                }

                s1ap_process_location_report_message_to_mme(
                        p_message,
                        message_buff_len,
                        p_ue_associated_sig_req->mme_id,
                        p_s1ap_gb_ctx,
                        p_s1ap_ctx);

                break;
            }
        case ASN1V_s1ap_id_ErrorIndication :
            {
                /* SPR 2793 Start */
                get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                        &p_s1ap_ctx,
                        p_ue_associated_sig_req->enb_ue_s1ap_id);

                p_mme_ctx = p_s1ap_gb_ctx->
                    mme_contexts[p_ue_associated_sig_req->mme_id];

                if (PNULL == p_s1ap_ctx)
                {
                    cause = 
                        send_msg_to_mme(p_mme_ctx, p_message, message_buff_len,
                                PNULL);
                }
                else
                {
                    cause = 
                        send_msg_to_mme(p_mme_ctx, p_message, message_buff_len,
                                &p_s1ap_ctx->stream_id);
                }

                /* SPR 2793 Stop */
                if( RRC_SUCCESS != cause)
                {
                    RRC_S1AP_TRACE(RRC_ERROR, 
                            "Unable to send message to MME %d",
                            p_mme_ctx->mme_id);
                    RRC_S1AP_UT_TRACE_EXIT();
                    return;
                }
                break;
            }

        case ASN1V_s1ap_id_PathSwitchRequest :
            {
                RRC_S1AP_TRACE(RRC_INFO,
                        "[%s]:ASN1V_s1ap_id_PathSwitchRequest recvd"
                        " \n ",__FUNCTION__);
                /*Storing the UE context in the 
                 * S1AP global contexts*/
                get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                        &p_s1ap_ue_context,
                        p_ue_associated_sig_req->enb_ue_s1ap_id);

                if (PNULL != p_s1ap_ue_context)
                {
                    RRC_S1AP_TRACE(RRC_ERROR,
                            "[%s]:Error ::UE Context Already"
                            " Present\n ",__FUNCTION__);
                    RRC_S1AP_UT_TRACE_EXIT();
                    return ;
                }
                mme_id = p_ue_associated_sig_req->mme_id;

                /* Bug 8198 Fix Start */
                /* Overload condition for MME is not checked here as this will be done 
                 * by RRM only .If RRM is admitting the UE then UE will not be released
                 * even if MME is in overload condition */
                /* Bug 8198 Fix End */

                /*Create ue_context at specific index 
                 * in s1ap_gb_context*/
                /* Allocate Memory for UE global context 
                 * data structure */
                p_s1ap_ctx = (ue_s1ap_context_t *)
                    rrc_mem_get(sizeof(ue_s1ap_context_t));

                if (PNULL == p_s1ap_ctx)
                {
                    RRC_S1AP_TRACE(RRC_ERROR, 
                            "[%s]:ue_s1ap_context_t: unable to"
                            " Allocate Memory for UE S1ap global "
                            "context!!\n", __FUNCTION__);
                    RRC_S1AP_UT_TRACE_EXIT();
                    return;
                }    

                memset_wrapper((void*) p_s1ap_ctx, 0,
                        sizeof(ue_s1ap_context_t));

                p_mme_ctx = p_s1ap_gb_ctx->mme_contexts[mme_id];
                stream_id = s1ap_allocate_stream_id(p_mme_ctx);

                if (MAX_NUM_SCTP_STREAM == stream_id )
                {
                    RRC_S1AP_TRACE(RRC_INFO,"[%s]:Stream "
                            "ID Allocation Failure\n",
                            __FUNCTION__);

                    if (PNULL != p_s1ap_ctx)
                    {
                        /* SPR 22789 Fix Start */
                        RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx,p_s1ap_ctx,p_ue_associated_sig_req->enb_ue_s1ap_id);
                        /* SPR 22789 Fix End */
                        /*BUG 604 changes start*/
                        /*Free s1ap_ue_id Ctx*/
                        s1ap_ueid_free(p_s1ap_gb_ctx,
                                p_ue_associated_sig_req->enb_ue_s1ap_id);
                        /*BUG 604 changes stop*/
                    }
                    RRC_S1AP_UT_TRACE_EXIT();
                    return ;
                }

                set_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                        p_s1ap_ctx,
                        p_ue_associated_sig_req->enb_ue_s1ap_id);
                /* SPR 19286 Fix Start */
                if( MAX_NUM_CELLS > cell_id)
                {
                    p_s1ap_ctx->cell_index = cell_id;
                }
                else
                {
                    RRC_S1AP_TRACE(RRC_INFO,"[%s]: "
                            "CELL id %d is not in valid range (%d)"
                            "Hence setting the cell index in S1AP_CONTEXT to First cell index i.e zero\n"
                            ,__FUNCTION__,
                            cell_id,MAX_NUM_CELLS);

                    p_s1ap_ctx->cell_index = RRC_NULL;
                }    
                /* SPR 19286 Fix End */

                /*SPR_FIX_20833_START*/
                /*code deleted*/
                /*SPR_FIX_20833_END*/
                /*set s1ap context fields in Gobal Context */
                p_s1ap_ctx->mme_id      =(U8) mme_id;
                p_s1ap_ctx->stream_id   = stream_id; 
                /*BUG 604 changes start*/
                /*Fill ue_index in ue_s1ap_ctx*/
                p_s1ap_ctx->ue_index = p_ue_associated_sig_req->ue_index;
                /*Fill enb_ue_s1ap_id in ue_s1ap_ctx*/
                p_s1ap_ctx->enb_ue_s1ap_id = 
                    p_ue_associated_sig_req->enb_ue_s1ap_id;
                /*BUG 604 changes stop*/


                /*Sending the s1ap context fields in 
                 * Gobal Context */
                RRC_S1AP_TRACE(RRC_INFO,"[%s]:Sending "
                        "Path Switch Request for ue_id %d",
                        __FUNCTION__,ue_id);
                if (SCTP_SOCKET_ERROR ==l3_sctp_send(
                            p_mme_ctx->sctp_sd,
                            p_message,
                            message_buff_len,
                            stream_id))
                {
                    RRC_S1AP_TRACE(RRC_WARNING, "%s:"
                            " l3_sctp_send send failed initial ue msg",
                            __FUNCTION__);
                    RRC_S1AP_UT_TRACE_EXIT();
                    return;
                }
                break;
            }
        default:
            send_asn_msg_to_mme(p_s1ap_gb_ctx,
                    p_ue_associated_sig_req,
                    p_ue_associated_sig_req->enb_ue_s1ap_id,
                    p_message,
                    message_buff_len);
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : s1ap_process_uecc_messages
 * Inputs         : p_api - Pointer to buffer containing input API
 *                  p_s1ap_gb_ctx -Pointer to the UECC global context data
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when a message is received from
 *                  UECC and is to be sent on UE associated logical S1 
 *                  connection.
 ****************************************************************************/
void  s1ap_process_uecc_messages(
        void    *p_api,
        s1ap_gb_context_t *p_s1ap_gb_ctx )
{
    search_tree_node_t * p_node = PNULL;
    ue_s1ap_context_t * p_s1ap_ctx      = PNULL;
    mme_context_t * p_mme_ctx           = PNULL;


    s1ap_allocate_mme_req_t          *p_alloc_mme_req = PNULL;
    s1ap_ue_release_ind_t   *p_ue_release_ind = PNULL;

    ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL;

    U8 cell_id      = MAX_NUM_CELLS;
    U8 ho_flag = RRC_FALSE;
    /* CID: 113470 Start */
    U16 ue_id       = RRC_NULL; 
    /* CID: 113470 End */
    U16 api_id = RRC_NULL; 
    /*coverity fix 61046*/
    U32 mme_id       = MAX_NUM_MME - 1;
    /*coverity fix 61046*/
    U16 stream_id     = MAX_NUM_SCTP_STREAM;
    /* Bug 8198 Fix Start */
    U8 procedure_code = RRC_NULL;
   /* Bug 8198 Fix End */
    /*BUG 604 changes start*/
    U32  enb_ue_s1ap_id = RRC_NULL;
    /*SPR 8708 Fix Start*/
     U32 invalid_plmn_id = RRC_INVALID_PLMN_ID;
    /*SPR 8708 Fix Stop*/
    s1ap_intra_cell_ho_success_ind_t *p_intra_cell_ho_success_ind = PNULL;
    /*BUG 604 changes stop*/

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(p_api != PNULL);
    RRC_ASSERT(p_s1ap_gb_ctx != PNULL);

    if ( S1AP_ACTIVE != p_s1ap_gb_ctx->s1ap_current_state )
    {
        RRC_S1AP_TRACE(RRC_FATAL, "[%s]:S1AP Current state is NOT ACTIVE"
                "Hence Discarding the message\n",
                __FUNCTION__);
        return ;

    }
    api_id = rrc_get_api_id((U8*)p_api);


    switch (api_id)
    {
        case S1AP_ALLOCATE_MME_REQ :
                p_alloc_mme_req = (s1ap_allocate_mme_req_t *)
                    ((U8*)p_api + CV_HDRSIZE);
			/*bugid 117989 begin*/
          	if(RRC_FAILURE == s1ap_check_cell_index(p_alloc_mme_req->cell_index,p_s1ap_gb_ctx))
          	{
          		RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Error cell index"
                            "is not regisetered\n ",__FUNCTION__);
                build_and_send_allocate_mme_resp(
                         RRC_INVALID_ENB_UE_S1AP_ID,
                         RRC_INVALID_REL_OF_MME,RRC_INVALID_MME_ID,
                         p_alloc_mme_req->ue_index,
                        /* mme grp id feature changes start */
                         p_s1ap_gb_ctx->matching_gummei_to_be_sent);
                return;
          	}
			/*bugid 117989 end*/          
                SET_CELL_AND_UE_INDEX(p_alloc_mme_req->cell_index, p_alloc_mme_req->ue_index);

            /*BUG 604 changes start*/
                enb_ue_s1ap_id = s1ap_ueid_allocate(p_s1ap_gb_ctx);
                if(enb_ue_s1ap_id == p_s1ap_gb_ctx->total_ue_supported)
                {
                    RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Error enb_ue_s1ap_id"
                            "is not allocated\n ",__FUNCTION__);
                /* SPR 16149 Fix Start */
                    build_and_send_allocate_mme_resp(
                         RRC_INVALID_ENB_UE_S1AP_ID,
                         RRC_INVALID_REL_OF_MME,RRC_INVALID_MME_ID,
                         p_alloc_mme_req->ue_index,
                        /* mme grp id feature changes start */
                         p_s1ap_gb_ctx->matching_gummei_to_be_sent);

                /* SPR 16149 Fix Stop */
                    return;
                }
            /*BUG 604 changes stop*/
                /* Bug 8198 Fix Start */
                /*if establishment cause is valid then fill procedure code as 
                 * initial ue message otherwise path_switch_req*/
                if(RRC_INVALID_ESTABLISHMENT_CAUSE == p_alloc_mme_req->rrc_establishment_cause)
                {
                    procedure_code = ASN1V_s1ap_id_PathSwitchRequest;
                }
		    /*SPR 8708 Fix Start*/
                
		 else if(!memcmp_wrapper(&invalid_plmn_id,
                    			p_alloc_mme_req->plmn_identity.data,
                                        p_alloc_mme_req->plmn_identity.numocts)) 
		 {
                 	procedure_code = RRC_RrcConnectionReq_proc_code;
  		 }
		 else
		 {
                    	procedure_code = ASN1V_s1ap_id_initialUEMessage;
		 }
		    /*SPR 8708 Fix Stop*/
                



                /*Allocating the mme*/
                if ((!p_s1ap_gb_ctx->
                            active_mme_info.active_mme_count)||
                        (RRC_FAILURE ==  
                         s1ap_allocate_mme_id( &mme_id,
                             p_s1ap_gb_ctx, 
                         /*BUG 604 changes start*/
                             p_alloc_mme_req,procedure_code,
                             enb_ue_s1ap_id)))
                /*BUG 604 changes stop*/
                {
                    if(ASN1V_s1ap_id_initialUEMessage == procedure_code)
                    {
                        RRC_S1AP_TRACE(RRC_INFO,"[%s]:MME is "
                                "not allocated\n",__FUNCTION__);
                        /* SPR 22302 Fix Start */
                        s1ap_ueid_free(p_s1ap_gb_ctx,
                                    enb_ue_s1ap_id);
                        if(RRC_FALSE == p_s1ap_gb_ctx->is_sel_mmes_overloaded)
                        { 
                            /*BUG 604 changes start*/
                            build_and_send_s1ap_ue_rel_req_int(
                                    enb_ue_s1ap_id,p_alloc_mme_req->ue_index);
                        /* SPR 22302 Fix Stop */
/*BUG 604 changes stop*/
                    }
                    return ;
                }
                else
                {
                    /*BUG 604 changes start*/
                    /*SPR 8708 Fix Start*/
                    s1ap_ueid_free(p_s1ap_gb_ctx,
                            enb_ue_s1ap_id);
                    /*SPR 8708 Fix Stop*/
                    build_and_send_allocate_mme_resp(enb_ue_s1ap_id,
                            RRC_INVALID_REL_OF_MME,RRC_INVALID_MME_ID,
                            p_alloc_mme_req->ue_index,
                            /* mme grp id feature changes start */
                            p_s1ap_gb_ctx->matching_gummei_to_be_sent);
                    /*BUG 604 changes stop*/
                    p_s1ap_gb_ctx->matching_gummei_to_be_sent = RRC_FALSE;
                    /* mme grp id feature changes stop */
                    return ;
                }
            }
            /* coverity_fix_61046 start */
            if(mme_id >= MAX_NUM_MME)
            {
                RRC_S1AP_TRACE(RRC_ERROR,
                        "[%s]:invalid mme_id",__FUNCTION__);
                return;
            }
            /* coverity_fix_61046 stop */
            p_mme_ctx = p_s1ap_gb_ctx->mme_contexts[mme_id];

            /*SPR 8708 Fix Start*/
            if((ASN1V_s1ap_id_initialUEMessage == procedure_code) || (RRC_RrcConnectionReq_proc_code == procedure_code)) 
                /*SPR 8708 Fix Stop*/
            {
                /*Create ue_context at specific index 
                 * in s1ap_gb_context*/
                /* Allocate Memory for UE global context 
                 * data structure */
                p_s1ap_ctx = (ue_s1ap_context_t *)
                    rrc_mem_get(sizeof(ue_s1ap_context_t));

                if (PNULL == p_s1ap_ctx)
                {
                    RRC_S1AP_TRACE(RRC_ERROR, 
                            "[%s]:ue_s1ap_context_t: unable to"
                            " Allocate Memory for UE S1ap global "
                            "context!!\n", __FUNCTION__);
                    /*BUG 604 changes start*/
                    s1ap_ueid_free(p_s1ap_gb_ctx,
                            enb_ue_s1ap_id);
                    /*SPR 8708 Fix Start*/
                    if (RRC_RrcConnectionReq_proc_code == procedure_code)
                    {
                        build_and_send_allocate_mme_resp(enb_ue_s1ap_id,
                                RRC_INVALID_REL_OF_MME,RRC_INVALID_MME_ID,
                                p_alloc_mme_req->ue_index,
                                /* mme grp id feature changes start */
                                p_s1ap_gb_ctx->matching_gummei_to_be_sent);
                        p_s1ap_gb_ctx->matching_gummei_to_be_sent = RRC_FALSE;
                        /* mme grp id feature changes stop */

                    }
                    else
                    {
                        build_and_send_s1ap_ue_rel_req_int(
                                enb_ue_s1ap_id,p_alloc_mme_req->ue_index);
                    }
                    /*SPR 8708 Fix Stop*/
                    /*BUG 604 changes stop*/
                    return;
                }    

                memset_wrapper((void*) p_s1ap_ctx, 0,
                        sizeof(ue_s1ap_context_t));

                stream_id = s1ap_allocate_stream_id(p_mme_ctx);

                if (MAX_NUM_SCTP_STREAM == stream_id )
                {
                    RRC_S1AP_TRACE(RRC_INFO,"[%s]:Stream "
                            "ID Allocation Failure\n",
                            __FUNCTION__);

                    if (PNULL != p_s1ap_ctx)
                    {
                        /* SPR 22789 Fix Start */
                        RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx,p_s1ap_ctx,enb_ue_s1ap_id);
                        /* SPR 22789 Fix End */
                        /*SPR 21220 Start*/
                        /*Code Removed*/
                        /*SPR 21220 Stop*/
                    }
                    /*BUG 604 changes start*/
                    s1ap_ueid_free(p_s1ap_gb_ctx,
                            enb_ue_s1ap_id);
                    /*SPR 8708 Fix Start*/
                    if (RRC_RrcConnectionReq_proc_code == procedure_code)
                    {
                        build_and_send_allocate_mme_resp(enb_ue_s1ap_id,
                                RRC_INVALID_REL_OF_MME,RRC_INVALID_MME_ID,
                                p_alloc_mme_req->ue_index,
                                /* mme grp id feature changes start */
                                p_s1ap_gb_ctx->matching_gummei_to_be_sent);
                        p_s1ap_gb_ctx->matching_gummei_to_be_sent = RRC_FALSE;
                        /* mme grp id feature changes stop */

                    }
                    else
                    {
                        build_and_send_s1ap_ue_rel_req_int(
                                enb_ue_s1ap_id,p_alloc_mme_req->ue_index);
                    }
                    /*SPR 8708 Fix Stop*/
                    /*BUG 604 changes stop*/

                    return ;
                }
                /* SPR 19286 Fix Start */
                if( MAX_NUM_CELLS > p_alloc_mme_req->cell_index)
                {
                    p_s1ap_ctx->cell_index = p_alloc_mme_req->cell_index;
                }
                else
                {
                    RRC_S1AP_TRACE(RRC_INFO,"[%s]: "
                            "CELL id %d is not in valid range (%d)"
                            "Hence setting the cell index in S1AP_CONTEXT to First cell index i.e zero \n"
                            ,__FUNCTION__,
                            p_alloc_mme_req->cell_index,MAX_NUM_CELLS);

                    p_s1ap_ctx->cell_index = RRC_NULL;
                }    
                /* SPR 19286 Fix End */



                /*BUG 604 changes start*/
                /* store ue_index received in allocate_mme_req*/
                p_s1ap_ctx->ue_index      = p_alloc_mme_req->ue_index;
                p_s1ap_ctx->enb_ue_s1ap_id = enb_ue_s1ap_id;
                /* SPR 22789 Fix Start */
                /*set s1ap context fields in Gobal Context */
                p_s1ap_ctx->mme_id      =(U8) mme_id;
                p_s1ap_ctx->stream_id   = stream_id;
                /* SPR 22789 Fix End */

                set_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                        p_s1ap_ctx,
                        enb_ue_s1ap_id);
                /*BUG 604 changes stop*/

                /* SPR 22789 Fix Start */ 
                /* Code Moved */
                /* SPR 22789 Fix End */ 
            }
            /* Bug 8198 Fix End */

            /*BUG 604 changes start*/
            build_and_send_allocate_mme_resp(enb_ue_s1ap_id,
                    p_mme_ctx->connection_info.rel_of_mme, mme_id,
                    p_alloc_mme_req->ue_index,
                    /* mme grp id feature changes start */
                    p_s1ap_gb_ctx->matching_gummei_to_be_sent);
            /*BUG 604 changes stop*/
            p_s1ap_gb_ctx->matching_gummei_to_be_sent = RRC_FALSE;
            /* mme grp id feature changes stop */

            break;
        case S1AP_UE_ASSOCIATED_SIG_MSG_REQ :
            {
                s1ap_uecc_process_ue_assoc_sig_msg_req(p_s1ap_gb_ctx, p_alloc_mme_req,
                        ((U8*)p_api + CV_HDRSIZE));
                break;
            }
        case S1AP_UE_RELEASE_MSG_REQ :
            {
                process_uecc_ue_release_req (p_s1ap_gb_ctx,
                        ((U8*)p_api + CV_HDRSIZE));
                break;
            }
        case S1AP_FAILURE_MSG_REQ :
            {
                process_uecc_failure_msg_req (p_s1ap_gb_ctx,
                        ((U8*)p_api + CV_HDRSIZE));
                break;
            }

        case S1AP_UE_RELEASE_IND:
            p_ue_release_ind = (s1ap_ue_release_ind_t *)
                ((U8*)p_api + CV_HDRSIZE);
            cell_id = s1ap_get_cell_id
                (p_s1ap_gb_ctx, p_ue_release_ind->enb_ue_s1ap_id);
            ue_id = s1ap_get_ue_id
                (p_ue_release_ind->enb_ue_s1ap_id);

            SET_CELL_AND_UE_INDEX(cell_id, ue_id);

            if (cell_id >= MAX_NUM_CELLS ||
                    !p_s1ap_gb_ctx->cell_s1ap_contexts[cell_id])
            {
                RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Error ::"
                        "Cell id[%d] is invalid "
                        ,__FUNCTION__,cell_id);
                return ;
            }
            if (ue_id >= MAX_NUM_UE)
            {
                RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Error ::"
                        "UE id[%d] is invalid "
                        ,__FUNCTION__,ue_id);
                return ;
            }
            RRC_S1AP_TRACE(RRC_INFO,"[%s]: enbUeS1apId[%d] "
                    "CELL id[%d] UE id[%d] "
                    ,__FUNCTION__,
                    p_ue_release_ind->enb_ue_s1ap_id,
                    cell_id,ue_id);

            get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                    &p_s1ap_ctx, p_ue_release_ind->enb_ue_s1ap_id);
            if (p_s1ap_ctx == PNULL)
            {
                /*coverity 81487 fix start*/
                //p_mme_ctx = p_s1ap_gb_ctx->
                // mme_contexts[p_ue_release_ind->mme_id];
                /*coverity 81487 fix end*/
                p_ho_ongoing_ue_ctx_node =
                    search_ho_list_node_to_be_released(p_s1ap_gb_ctx,
                            p_ue_release_ind);

                if(PNULL != p_ho_ongoing_ue_ctx_node)
                {
                    /* coverity fix 81375 start */
                    // stream_id = p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.stream_id;
                    /* coverity fix 81375 end */

                    list_delete_node(&p_s1ap_gb_ctx->
                            ho_ongoing_ue_ctx_list,
                            (void *)p_ho_ongoing_ue_ctx_node);

                    rrc_mem_free((void*)
                            p_ho_ongoing_ue_ctx_node);
                    /* + Coverity 81334 */
                    //p_ho_ongoing_ue_ctx_node = PNULL;
                    /* - Coverity 81334 */
                    /*BUG 604 changes start*/
                    /*Free s1ap_ue_id Ctx*/
                    s1ap_ueid_free(p_s1ap_gb_ctx,
                            p_ue_release_ind->enb_ue_s1ap_id);
                    /*BUG 604 changes stop*/
                    RRC_S1AP_TRACE(RRC_INFO,
                            "[%s] HO ONGOING Temporary UE Context "
                            "DELETED on UE Context Release",
                            __FUNCTION__);
                    ho_flag = RRC_TRUE;
                }
                else
                {
                    RRC_S1AP_TRACE(RRC_INFO,
                            "[%s] The ho_ongoing_ue_ctx_node_t is NULL. "
                            ,__FUNCTION__);
                }
            }
            if (ho_flag != RRC_TRUE )
            {
                if (PNULL != p_s1ap_ctx)
                {
                    /* coverity fix 81375 start */
                    //stream_id =  p_s1ap_ctx->stream_id;
                    /* coverity fix 81375 end */

                    //if (0xFF != p_ue_release_ind->mme_id) //remark for bugid 117290
                    {
                        p_mme_ctx = p_s1ap_gb_ctx->
                            mme_contexts[p_s1ap_ctx->mme_id];
                        /*delete entry from  from search tree*/
                        p_node = st_find_tree_node (
                                (search_tree_t *)&
                                (p_mme_ctx->mmeUeS1apId_search_tree),
                                (void*)(&p_s1ap_ctx->mme_ue_s1ap_id) );

                        if (!p_node)
                        {
                            RRC_S1AP_TRACE(RRC_WARNING,
                                    "[%s] Entry not found in search tree ",
                                    __FUNCTION__);
                        }
                        else
                        {
                            st_delete_node(
                                    (search_tree_t *)&
                                    (p_mme_ctx->mmeUeS1apId_search_tree),
                                    (search_tree_node_t *) p_node);
                            rrc_mem_free((void*)p_node);
                            /* + Coverity 81334 */
                            //p_node = PNULL;
                            /* - Coverity 81334 */
                        }
                    }

                    /* SPR 22789 Fix Start */
                    RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx,p_s1ap_ctx,p_ue_release_ind->enb_ue_s1ap_id);
                    /* SPR 22789 Fix End */
                    /*BUG 604 changes start*/
                    /*Free s1ap_ue_id Ctx*/
                    s1ap_ueid_free(p_s1ap_gb_ctx,
                            p_ue_release_ind->enb_ue_s1ap_id);
                    /*BUG 604 changes stop*/
                    /* SPR 22789 Fix Start */
                    RRC_S1AP_TRACE(RRC_INFO, "Deleted entry at Cell id[%d] UE id[%d] ",
                            cell_id, ue_id);
                    /* SPR 22789 Fix End */
                }
                /* SPR 22789 Fix Start */
                /* Code Moved */
                /* SPR 22789 Fix End */
            }
            break;

        case S1AP_HO_ONGOING_UE_RELEASE_IND :
            {
                process_uecc_ho_ongoing_ue_rel_ind (p_s1ap_gb_ctx,
                        ((U8*)p_api + CV_HDRSIZE));
                break;
            }
            /*BUG 604 changes start*/
        case S1AP_INTRA_CELL_HO_SUCCESS_IND :
            {
                p_intra_cell_ho_success_ind = (s1ap_intra_cell_ho_success_ind_t *)
                    ((U8*)p_api + CV_HDRSIZE);
                cell_id = s1ap_get_cell_id
                    (p_s1ap_gb_ctx, p_intra_cell_ho_success_ind->enb_ue_s1ap_id);

                SET_CELL_AND_UE_INDEX(cell_id, p_intra_cell_ho_success_ind->ue_index);

                if (cell_id >= MAX_NUM_CELLS ||
                        !p_s1ap_gb_ctx->cell_s1ap_contexts[cell_id])
                {
                    RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Error ::"
                            "Cell id[%d] is invalid "
                            ,__FUNCTION__,cell_id);
                    return ;
                }
                RRC_S1AP_TRACE(RRC_INFO,"[%s]: enbUeS1apId[%d] "
                        "CELL id[%d] "
                        ,__FUNCTION__,
                        p_intra_cell_ho_success_ind->enb_ue_s1ap_id,
                        cell_id);

                get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                        &p_s1ap_ctx, p_intra_cell_ho_success_ind->enb_ue_s1ap_id);
                if ( PNULL == p_s1ap_ctx )
                {
                    RRC_S1AP_TRACE(RRC_WARNING,"UE context doesn't exist");
                    return;
                }
                /*Update the ue_index in s1ap_ue_ctx in case of IntraCell HO
                 * success*/
                p_s1ap_ctx->ue_index = p_intra_cell_ho_success_ind->ue_index;

                break;
            }
            /*BUG 604 changes stop*/
        default:
            {
                RRC_S1AP_TRACE(RRC_ERROR, "[%s]Processing UECC Messages :"
                        "Unknown API ::%d\n",__FUNCTION__,api_id);    
                break;
            }

    }
    RRC_S1AP_UT_TRACE_EXIT();
}


/****************************************************************************
 * Function Name  : send_asn_msg_to_mme
 * Inputs         : p_s1ap_gb_ctx - Pointer to the UECC global context data
 *                  p_ue_associated_sig_req -UE Associated Signalling Message 
 *                  cell_id - Cell ID 
 *                  ue_id - UE ID
 *                  p_message -Message to be sent
 *                  message_buff_len - Message Length
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends ASN msgs to MME.
 ****************************************************************************/

void send_asn_msg_to_mme(
        s1ap_gb_context_t                 *p_s1ap_gb_ctx,
        s1ap_ue_associated_sig_msg_req_t  *p_ue_associated_sig_req,
        U16                                ue_id, /* enbS1ApId */
        U8                                *p_message,
        U32                               message_buff_len
        )
{
    ue_s1ap_context_t* p_s1ap_ctx      = PNULL;
    mme_context_t    * p_mme_ctx       = PNULL;

    ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL;

    U32 mme_id       = MAX_NUM_MME;
    U16 stream_id    = MAX_NUM_SCTP_STREAM;

    /*SPR_21818_START*/
    mmeUeS1apId_search_node_t *p_mmeUeS1apId_st_node = PNULL;
    /*SPR_21818_END*/
    RRC_S1AP_UT_TRACE_ENTER();


    get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
            &p_s1ap_ctx, ue_id);

    if ( PNULL == p_s1ap_ctx )
    {
        p_ho_ongoing_ue_ctx_node = 
            search_ho_list_node (p_s1ap_gb_ctx,
                    p_ue_associated_sig_req);

        if(PNULL == p_ho_ongoing_ue_ctx_node)
        {
            RRC_S1AP_TRACE(RRC_ERROR,
                    "[%s]: UE Context not found",
                    __FUNCTION__);

            return;
        }

        /* get mme_id and  stream_id */
        mme_id = p_ho_ongoing_ue_ctx_node->
            ho_ongoing_ue_ctx.mme_id;
        stream_id = p_ho_ongoing_ue_ctx_node->
            ho_ongoing_ue_ctx.stream_id;
        p_mme_ctx = p_s1ap_gb_ctx->
            mme_contexts[mme_id];

        RRC_S1AP_TRACE(RRC_INFO,
                "[%s]: ASN message is forwarded to MME %d"
                " \n ",__FUNCTION__,mme_id);
    }
    else
    {
        p_mme_ctx = p_s1ap_gb_ctx->
            mme_contexts[p_s1ap_ctx->mme_id];
        stream_id = p_s1ap_ctx->stream_id;

        RRC_S1AP_TRACE(RRC_INFO,
                "[%s]: ASN message is forwarded to MME %d"
                " \n ",__FUNCTION__,p_s1ap_ctx->mme_id);
    }

    if ( MME_ACTIVE != p_mme_ctx->current_mme_state)
    {
        /*MME is not Active hence Discard the message*/
        RRC_S1AP_TRACE(RRC_INFO,
                "[%s]:MME is not Active "
                "hence Discard the message\n"
                ,__FUNCTION__); 
        /* SPR 3840 starts */
/*BUG 604 changes start*/
      /* Coverity ID 81276 Fix Start*/
            build_and_send_s1ap_ue_rel_req_int(
                    p_ue_associated_sig_req->enb_ue_s1ap_id,
                    p_ue_associated_sig_req->ue_index);
            /*Free s1ap_ue_id Ctx*/
            s1ap_ueid_free(p_s1ap_gb_ctx,
                    p_ue_associated_sig_req->enb_ue_s1ap_id);
        /* Coverity ID 81276 Fix End*/
/*BUG 604 changes stop*/
        /* SPR 3840 ends */

        /*SPR_21818_START*/
        if (!p_s1ap_ctx)
        {
            RRC_S1AP_TRACE(RRC_INFO, "[%s]: "
                    "UE context doesn't exist at id %d",__FUNCTION__,ue_id);
        }
        else
        {
            p_mmeUeS1apId_st_node = (mmeUeS1apId_search_node_t *)
                st_find_tree_node(
                    &p_mme_ctx->mmeUeS1apId_search_tree,
                    &(p_s1ap_ctx->mme_ue_s1ap_id));

            if (!p_mmeUeS1apId_st_node)
            {
                RRC_S1AP_TRACE(RRC_INFO, "[%s]: "
                        "mmeUeS1apId is not present in the search tree",
                        __FUNCTION__);
            }
            else
            {
                st_delete_node(&p_mme_ctx->mmeUeS1apId_search_tree,
                        (search_tree_node_t*)p_mmeUeS1apId_st_node);

                if (PNULL != p_mmeUeS1apId_st_node)
                {
                    rrc_mem_free(p_mmeUeS1apId_st_node);
                    p_mmeUeS1apId_st_node = PNULL;
                }
            }
            /* SPR 22789 Fix Start */
            RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx,p_s1ap_ctx,p_ue_associated_sig_req->enb_ue_s1ap_id);
            /* SPR 22789 Fix End */
        }
        /*SPR_21818_END*/
        /* SPR 22789 Fix Start */
        /* Code Deleted */
        /* SPR 22789 Fix End */
        return;
    }             

    if (SCTP_SOCKET_ERROR == l3_sctp_send(
                p_mme_ctx->sctp_sd,
                p_message,
                message_buff_len,
                stream_id))
    {
        RRC_S1AP_TRACE(RRC_WARNING,
                "[%s]:l3_sctp_send"
                " failed\n",__FUNCTION__);
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/*REL 1.2:HANDOVER PROCEDURE END*/                        
/****************************************************************************
 * Function Name  : process_uecc_ue_release_req
 * Inputs         : p_s1ap_gb_ctx -  Pointer to S1AP global ctx
 *                  p_api - Pointer to api buffer received from UECC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes UE release request from UECC.
 ****************************************************************************/
void  process_uecc_ue_release_req(
        s1ap_gb_context_t * p_s1ap_gb_ctx,  
        U8 * p_api) 
{
    search_tree_node_t *p_node = PNULL;
    s1ap_ue_release_msg_req_t * p_s1ap_ue_release_msg_req = 
        (s1ap_ue_release_msg_req_t*)p_api;
    ue_s1ap_context_t * p_s1ap_ctx      = PNULL;
    U8 cell_id      = MAX_NUM_CELLS;
    /* CID: 113467 Start */
    U16 ue_id       = RRC_NULL;
    /* CID: 113467 End */
    mme_context_t * p_mme_context = PNULL;
    s1ap_enb_reset_info_t  reset_req;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_S1AP_TRACE(RRC_INFO,"[%s]:UE Release request"
            " received from UECC for %d\n"
            ,__FUNCTION__,p_s1ap_ue_release_msg_req->enb_ue_s1ap_id);

    cell_id     = s1ap_get_cell_id(p_s1ap_gb_ctx, p_s1ap_ue_release_msg_req->
                enb_ue_s1ap_id);
    ue_id       = s1ap_get_ue_id(p_s1ap_ue_release_msg_req->enb_ue_s1ap_id);

    SET_CELL_AND_UE_INDEX(cell_id, ue_id);

    if (cell_id >= MAX_NUM_CELLS || !p_s1ap_gb_ctx->cell_s1ap_contexts[cell_id])
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Error ::Cell id %d is invalid \n"
                ,__FUNCTION__,cell_id);
        return ;
    }

    if (ue_id >= MAX_NUM_UE)
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Error ::UE id %d is invalid \n"
                ,__FUNCTION__,ue_id);
        return ;
    }

    get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
            &p_s1ap_ctx, p_s1ap_ue_release_msg_req->enb_ue_s1ap_id);
    if ( PNULL == p_s1ap_ctx )
    {
        RRC_S1AP_TRACE(RRC_WARNING,"[%s]: UE context doesn't exist at %d\n",
                __FUNCTION__,ue_id);
        return;
    }

    /* check if sending of RESET message of MME is required for
       release UE context */
    /*SPR_21818_START*/
    p_mme_context = p_s1ap_gb_ctx->mme_contexts[p_s1ap_ctx->mme_id];

    if ((p_s1ap_ue_release_msg_req->mme_release_required) &&
            (p_s1ap_gb_ctx->active_mme_info.active_mme_count) &&
            (PNULL != p_mme_context) &&
            (MME_ACTIVE == p_mme_context->current_mme_state))
    /*SPR_21818_END*/
    {
        memset_wrapper((void*)&reset_req, 0 ,
                sizeof(s1ap_enb_reset_info_t));

        /*Build RESET MSG to MME with 
         * choice type :part of s1 interface */

        reset_req.num_of_items = 1;
        reset_req.enbUeS1apId_arr[0].enbUeS1apId = 
            p_s1ap_ue_release_msg_req->enb_ue_s1ap_id;
        reset_req.enbUeS1apId_arr[0].mme_id = p_s1ap_ctx->mme_id;

        /**** Cause ****/

	/*SPR 15610 Fix Start*/
	if (p_s1ap_ue_release_msg_req->release_cause_valid)
	{
		reset_req.cause =
			 p_s1ap_ue_release_msg_req->cause; 

	}
	else
	{
        reset_req.cause.t = T_s1ap_Cause_radioNetwork;
        reset_req.cause.u.radioNetwork = 
            s1ap_release_due_to_eutran_generated_reason;
	}
	/*SPR 15610 Fix Stop*/
	s1ap_add_and_process_reset_context(
			p_s1ap_gb_ctx,
			&reset_req,
			(RRC_MAX_EXT_MODULE_ID +1),0,0,1);
    }
    else
    {
	    /* Release the UE context */
	    if (p_s1ap_ctx->is_mme_ue_s1ap_id_valid)
	    {
		    /*SPR_17633_START*/
		    p_mme_context = 
			    p_s1ap_gb_ctx->mme_contexts[p_s1ap_ctx->mme_id];

		    if(PNULL != p_mme_context)
		    {
			    p_node = st_find_tree_node (
					    (search_tree_t *)&(p_mme_context->mmeUeS1apId_search_tree),
					    (void*)(&p_s1ap_ctx->mme_ue_s1ap_id) ); 

			    if (!p_node)
			    {
				    RRC_S1AP_TRACE(RRC_WARNING,
						    "[%s] Entry not found in search tree\n",
						    __FUNCTION__);
			    }
			    else
			    {
				    st_delete_node(
						    (search_tree_t *)&
						    (p_mme_context->mmeUeS1apId_search_tree),
						    (search_tree_node_t *) p_node); 

				    if (PNULL != p_node)
				    {
					    rrc_mem_free((void*)p_node);
					    p_node = PNULL;
				    }
			    }
		    }
		    /*SPR_17633_END*/
	    }

        if (PNULL != p_s1ap_ctx)
        {
            /*BUG 604 changes start*/
            /*Free s1ap_ue_id Ctx*/
            s1ap_ueid_free(p_s1ap_gb_ctx,
                    p_s1ap_ctx->enb_ue_s1ap_id);
            /*BUG 604 changes stop*/
            /* SPR 22789 Fix Start */
            RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx,p_s1ap_ctx, p_s1ap_ctx->enb_ue_s1ap_id);
            /* SPR 22789 Fix End */
        }
        /* SPR 22789 Fix Start */
        /* Code Deleted */
        /* SPR 22789 Fix End */
    }
}

/****************************************************************************
 * Function Name  : s1ap_allocate_mme_id
 * Inputs         : p_mme_id - Pointer to the mme Index to be updated
 *                  p_s1ap_gb_ctx -  Pointer to S1AP global ctx
 *                  p_alloc_mme_req - pointer to s1ap_allocate_mme_req_t
 * Outputs        : None
 * Returns        : rrc_return_et - RRC_SUCCESS / RRC_FAILURE
 * Description    : This function is called to select the mme id from the
                    active mme based on p_registered_mme_info.
 ****************************************************************************/
rrc_return_et  s1ap_allocate_mme_id(
        U32 * p_mme_id ,
        s1ap_gb_context_t * p_s1ap_gb_ctx,  
        s1ap_allocate_mme_req_t * p_alloc_mme_req,
/* Bug 8198 Fix Start */
        U8    procedure_code,
/* Bug 8198 Fix End */
/*BUG 604 changes start*/
        U32   enb_ue_s1ap_id) 
/*BUG 604 changes stop*/
{
    rrc_return_et response = RRC_FAILURE;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_mme_id );
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx );
    RRC_ASSERT(PNULL != p_alloc_mme_req );
  /*SPR 8708 Fix Start*/ 
    if (RRC_RrcConnectionReq_proc_code == procedure_code)
    {
        /* SPR 22789 Fix Start */ 
        RRC_S1AP_TRACE(RRC_INFO, "Before Allocating MME %d by MMEC", *p_mme_id);
        response = s1ap_allocate_by_mmec(p_mme_id ,
                p_s1ap_gb_ctx ,
                p_alloc_mme_req);/*SPR 17777 +-*/
        if (RRC_SUCCESS == response )
        {
            RRC_S1AP_TRACE(RRC_INFO, "Allocated MME %d by MMEC", *p_mme_id);
        }
        else
        {
            RRC_S1AP_TRACE(RRC_ERROR,"Unable to allocate MME by mmec");
        }
        /* SPR 22789 Fix End */ 
    }
    else
    {
  /*SPR 8708 Fix Stop*/ 

    switch (p_s1ap_gb_ctx->mme_selection_algo)
    {
        case MME_ROUND_ROBIN_ALGO :
        {
            /*allocate_mme_by_round_robin*/ 
            response  = allocate_mme_by_round_robin(
                    p_mme_id ,
                    p_s1ap_gb_ctx,
            /* Bug 8198 Fix Start */
                    p_alloc_mme_req,procedure_code,
/*BUG 604 changes start*/
                    enb_ue_s1ap_id);
/*BUG 604 changes stop*/
            /* Bug 8198 Fix End */
            if (RRC_SUCCESS == response )
            {
                RRC_S1AP_TRACE(RRC_INFO,
                        "[%s]:allocate MME %d in round robin algorithm \n",__FUNCTION__,*p_mme_id);
                /* BUG_7995 */
                RRC_S1AP_TRACE(RRC_INFO, "GUMMEI used : PLMN:%02x%02x%02x MMEGI:%02x%02x MMEC:%02x",
                        p_alloc_mme_req->plmn_identity.data[0],p_alloc_mme_req->plmn_identity.data[1],
                        p_alloc_mme_req->plmn_identity.data[2],p_alloc_mme_req->mme_group_id.data[0],
                        p_alloc_mme_req->mme_group_id.data[1],p_alloc_mme_req->mmec.data[0]);
                /* BUG_7995 */
                RRC_S1AP_UT_TRACE_EXIT();
                return  response;
            }
            else
            {
                RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Unable to allocate MME in round robin algorithm \n",
                        __FUNCTION__);
            }
            break;
        }
        case MME_WEIGHT_FACTOR_ALGO :
        {
            response  = allocate_mme_by_weight_factor(
                    p_mme_id ,
                    p_s1ap_gb_ctx,
            /* Bug 8198 Fix Start */
                    p_alloc_mme_req,procedure_code,
/*BUG 604 changes start*/
                    enb_ue_s1ap_id);
/*BUG 604 changes stop*/
            /* Bug 8198 Fix End */
            if (RRC_SUCCESS == response )
            {
                /* SPR 22789 Fix Start */
                RRC_S1AP_TRACE(RRC_INFO,
                    "Allocate MME %d in weight factor algorithm",*p_mme_id);
                /* SPR 22789 Fix End */
                /* BUG_7995 */
                RRC_S1AP_TRACE(RRC_INFO, "GUMMEI used : PLMN:%02x%02x%02x MMEGI:%02x%02x MMEC:%02x",
                        p_alloc_mme_req->plmn_identity.data[0],p_alloc_mme_req->plmn_identity.data[1],
                        p_alloc_mme_req->plmn_identity.data[2],p_alloc_mme_req->mme_group_id.data[0],
                        p_alloc_mme_req->mme_group_id.data[1],p_alloc_mme_req->mmec.data[0]);
                /* BUG_7995 */
                RRC_S1AP_UT_TRACE_EXIT();
                return  response;
            }
            else
            {
                RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Unable to allocate MME in weight factor algorithm\n",
                        __FUNCTION__);
            }
            break;
        }
        default:
        {
            RRC_S1AP_TRACE(RRC_ERROR,"[%s]: algorithm type in s1ap_oam_provision_req is incorrect \n",
            __FUNCTION__);
        }
    }
/*SPR 8708 Fix Start*/
   }
/*SPR 8708 Fix Stop*/
    RRC_S1AP_UT_TRACE_EXIT();
    return response;
}

/*SPR 8708 Fix Start*/
/****************************************************************************
 * Function Name  : s1ap_allocate_by_mmec
 * Inputs         : p_mme_id    - Pointer to the mme Index to be updated
 *                  p_s1ap_gb_ctx - Pointer to S1AP global ctx
 *                  p_ue_associated_sig_req - pointer to s1ap_allocate_mme_req_t
 * Outputs        : None
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : This function is called to  allocate MME by the MMEC value 
 * 		    present in allocate_mme_req only.
 ****************************************************************************/
rrc_return_et s1ap_allocate_by_mmec(
        U32* p_mme_id,
        s1ap_gb_context_t* p_s1ap_gb_ctx,
        s1ap_allocate_mme_req_t * p_alloc_mme_req)/*SPR 17777 +-*/
{
    active_mme_info_t *p_mme_info = PNULL;
    U32 mme_id = MAX_NUM_MME; 
    U8 tempCount = S1AP_ZERO;
    U8 match_result = NO_MATCHING_MME;
    mme_overload_response_et mme_overload_response = OVERLOAD_START_INVALID_VALUE;
    static U8 allocated_mme_count = S1AP_ZERO;
    U8 mme_overload_flag = RRC_FALSE; 

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_mme_id);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx );
    RRC_ASSERT(PNULL != p_alloc_mme_req );

    p_mme_info = &(p_s1ap_gb_ctx->active_mme_info);
    
    if( S1AP_ZERO != allocated_mme_count)
    {
        p_mme_info->mme_idx_to_returned =
            (U8)( (p_mme_info->mme_idx_to_returned + 1)%
                    p_mme_info->active_mme_count);
    }
    
    tempCount = p_mme_info->active_mme_count;
    allocated_mme_count = 1;
    
    while (tempCount)
    {
        mme_id = p_mme_info->mme_id_arr[p_mme_info->mme_idx_to_returned].mme_id;

        if (MME_ACTIVE ==
                p_s1ap_gb_ctx->mme_contexts[mme_id]->current_mme_state )
        {

                mme_overload_response = (mme_overload_response_et)p_mme_info->mme_id_arr[p_mme_info->
                     mme_idx_to_returned].overload_response;

                match_result = match_registered_mme_info_by_mmec(/*SPR 17777+-*/
                     			p_s1ap_gb_ctx->mme_contexts[mme_id],
                     			p_alloc_mme_req);
                if ( MATCHING_MME == match_result)
            	{
                    if ( RRC_SUCCESS == check_for_mme_overload(
                                mme_overload_response,
                                p_alloc_mme_req->rrc_establishment_cause,
                                p_alloc_mme_req,
                                p_s1ap_gb_ctx->mme_contexts[mme_id],
                                &p_mme_info->mme_id_arr[p_mme_info->mme_idx_to_returned],
                                match_result))
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "Assigned MME ID [%d]"
                                " OVERLOADED STATE [%d]",
                                mme_id, 
                                mme_overload_response);
                        mme_overload_flag = RRC_TRUE;        
                    }
                    else
                    {
                        *p_mme_id  =  mme_id;
			  /* mme grp id feature changes start */          
                          p_s1ap_gb_ctx->matching_gummei_to_be_sent = RRC_TRUE;
                          /* mme grp id feature changes stop */
                        RRC_S1AP_TRACE(RRC_INFO,"[%s]:Registered MME found ID %d\n",
                                __FUNCTION__,mme_id);
                        RRC_S1AP_UT_TRACE_EXIT();
			return RRC_SUCCESS;
		    }
		}
		
             
        }
        p_mme_info->mme_idx_to_returned = 
            (U8)( (p_mme_info->mme_idx_to_returned + 1)%
                  p_mme_info->active_mme_count);
        tempCount--;
    }
    if(RRC_TRUE == mme_overload_flag)
    {
         RRC_S1AP_TRACE(RRC_INFO, "Assigned MME "
                " OVERLOADED, Releasing UE"
                " Establishment [%d]", p_alloc_mme_req->rrc_establishment_cause);
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_FAILURE;
}


/****************************************************************************
 * Function Name  : match_registered_mme_info_by_mmec
 * Inputs          :p_s1ap_gb_ctx - pointer to S1AP global context 
 *                  p_mme_ctx  - Pointer to mme context
 *                  p_alloc_mme_req - Conains registered_mme infomation
 *                                    that is matched with the one in
 *                                    mme context p_mme_ctx.
 * Outputs        : None
 * Returns        : RRC_SUCCESS - If match found.
 *                  RRC_FAILURE - If match not found.
 * Description    : This function compares MMEC information in 
 *                  p_alloc_mme_req with the one in p_mme_ctx.
 ****************************************************************************/
U8  match_registered_mme_info_by_mmec (
        /*SPR 17777 +-*/
        mme_context_t*     p_mme_ctx,
        s1ap_allocate_mme_req_t * p_alloc_mme_req)
{
    served_gummei_info_t *p_served_gummeis = p_mme_ctx->p_served_gummeis;
    U8 served_gummei_lte = 0;
    U32 mmec_count = 0;
    s1ap_ServedMMECs * p_served_mmecs = PNULL;

    RRC_ASSERT(PNULL != p_alloc_mme_req );
    /*SPR 17777 +-*/
    RRC_ASSERT(PNULL != p_mme_ctx );
    
    if (!p_served_gummeis)
    {
	    return NO_MATCHING_MME;
    }

    if (p_alloc_mme_req->bitmask & REGISTERED_MMEC_PRESENT)
    {
        p_served_mmecs = &p_served_gummeis[served_gummei_lte].
                  served_mmecs;

        for (mmec_count = 0; mmec_count < p_served_mmecs->n;
                     mmec_count++)
        {
            if ( (p_served_mmecs->elem[mmec_count].numocts ==
                     p_alloc_mme_req->mmec.numocts) && 
                   (!memcmp_wrapper(p_served_mmecs->elem[mmec_count].data,
                       p_alloc_mme_req->mmec.data,
                       p_alloc_mme_req->mmec.numocts)))
            {
                RRC_S1AP_TRACE(RRC_INFO,"[%s]:MATCHING_MME found",
                   __FUNCTION__);
                return MATCHING_MME;
            }
        }
    }

    RRC_S1AP_TRACE(RRC_INFO,"[%s]:NO_MATCHING_MME found",
            __FUNCTION__);

    return NO_MATCHING_MME;
}
/*SPR 8708 Fix Stop*/
/****************************************************************************
 * Function Name  : allocate_mme_by_round_robin
 * Inputs         : p_mme_id    - Pointer to the mme Index to be updated
 *                  p_s1ap_gb_ctx - Pointer to S1AP global ctx
 *                  p_ue_associated_sig_req - pointer to s1ap_allocate_mme_req_t
 * Outputs        : None
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : This function is called to  allocate  mme by round robin
 *                  algorithm applied on the active mme's only if Registered
 *                  MME is not found in S1AP global context.
 ****************************************************************************/
rrc_return_et  allocate_mme_by_round_robin(
        U32* p_mme_id,
        s1ap_gb_context_t* p_s1ap_gb_ctx,
        s1ap_allocate_mme_req_t * p_alloc_mme_req,
/* Bug 8198 Fix Start */
        U8 procedure_code, 
/* Bug 8198 Fix End */
/*BUG 604 changes start*/
        U32   enb_ue_s1ap_id) 
/*BUG 604 changes stop*/
{
    active_mme_info_t *p_mme_info = PNULL;
    U32 mme_id = MAX_NUM_MME; 
    U8 tempCount = S1AP_ZERO;
    U8 first_mme_index_rr = MAX_NUM_MME;
    U8 first_mme_mmegi_index_rr = MAX_NUM_MME;
    U8 match_result = NO_MATCHING_MME;
    mme_overload_response_et mme_overload_response = OVERLOAD_START_INVALID_VALUE;
    static U8 allocated_mme_count = S1AP_ZERO;
    U8 mme_overload_flag = RRC_FALSE; 

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_mme_id);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx );
    RRC_ASSERT(PNULL != p_alloc_mme_req );

    p_mme_info = &(p_s1ap_gb_ctx->active_mme_info);

#ifdef LINUX_PC_TEST
		//means there is no ue at present, so reset allocated_mme_count. just for test.
		if(p_s1ap_gb_ctx->total_ue_supported ==
			p_s1ap_gb_ctx->s1ap_ueid_ctx.not_allocated_ue_ids_count+1)
			allocated_mme_count = 0;
#endif
    
    if( S1AP_ZERO != allocated_mme_count)
    {
        p_mme_info->mme_idx_to_returned =
            (U8)( (p_mme_info->mme_idx_to_returned + 1)%
                    p_mme_info->active_mme_count);
    }
    
    tempCount = p_mme_info->active_mme_count;
    allocated_mme_count = 1;
    
    /* If Registered MME info is present, try allocating the Registred
     * MME*/
    while (tempCount)
    {
        mme_id = p_mme_info->mme_id_arr[p_mme_info->mme_idx_to_returned].mme_id;

        if (MME_ACTIVE ==
                p_s1ap_gb_ctx->mme_contexts[mme_id]->current_mme_state )
        {
/* Bug 8198 Fix Start */
            if(ASN1V_s1ap_id_initialUEMessage == procedure_code)
            {
                mme_overload_response = (mme_overload_response_et)p_mme_info->mme_id_arr[p_mme_info->
                    mme_idx_to_returned].overload_response;
            }
/* Bug 8198 Fix End */

            match_result = match_registered_mme_info(p_s1ap_gb_ctx,
                    p_s1ap_gb_ctx->mme_contexts[mme_id],
                    p_alloc_mme_req);

/* Bug 8198 Fix Start */
            if ( MATCHING_MME == match_result)
            {
                if(ASN1V_s1ap_id_initialUEMessage == procedure_code)
                {
                    if ( RRC_SUCCESS == check_for_mme_overload(
                                mme_overload_response,
                                p_alloc_mme_req->rrc_establishment_cause,
                                p_alloc_mme_req,
                                p_s1ap_gb_ctx->mme_contexts[mme_id],
                                &p_mme_info->mme_id_arr[p_mme_info->mme_idx_to_returned],
                                match_result))
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "Assigned MME ID [%d]"
                                " OVERLOADED STATE [%d]",
                                mme_id, 
                                mme_overload_response);
                        mme_overload_flag = RRC_TRUE;        
                    }
                    else
                    {
                        *p_mme_id  =  mme_id;
                        /* mme grp id feature changes start */
                        p_s1ap_gb_ctx->matching_gummei_to_be_sent = RRC_TRUE;
                        /* mme grp id feature changes stop */
                        RRC_S1AP_TRACE(RRC_INFO,"[%s]:Registered MME found ID %d\n",
                                __FUNCTION__,mme_id);
                        RRC_S1AP_UT_TRACE_EXIT();
                        return RRC_SUCCESS;
                    }
                }
                else
                {
                    *p_mme_id  =  mme_id;
                    RRC_S1AP_TRACE(RRC_INFO,"[%s]:Registered MME found ID %d\n",
                            __FUNCTION__,mme_id);
                    RRC_S1AP_UT_TRACE_EXIT();
                    return RRC_SUCCESS;
                }
            }
            else if (PARTIAL_MATCHING_MMEGI_MME == match_result)
            {
                if(first_mme_mmegi_index_rr == MAX_NUM_MME)
                {
                    if(ASN1V_s1ap_id_initialUEMessage == procedure_code)
                    {
                        if ( RRC_SUCCESS == check_for_mme_overload(
                                    mme_overload_response,
                                    p_alloc_mme_req->rrc_establishment_cause,
                                    p_alloc_mme_req,
                                    p_s1ap_gb_ctx->mme_contexts[mme_id],
                                    &p_mme_info->mme_id_arr[p_mme_info->mme_idx_to_returned],
                                    match_result))
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "Assigned MME ID [%d]"
                                    " OVERLOADED STATE [%d]",
                                    mme_id, 
                                    mme_overload_response);
                            mme_overload_flag = RRC_TRUE;        
                        }
                        else
                        {
                            first_mme_mmegi_index_rr = p_mme_info->mme_idx_to_returned;
                        }
                    }
                    else
                    {
                        first_mme_mmegi_index_rr = p_mme_info->mme_idx_to_returned;
                    }
                }
            }

            else if ( PARTIAL_MATCHING_MME == match_result)
            {
                if (first_mme_index_rr == MAX_NUM_MME)
                {
                    if(ASN1V_s1ap_id_initialUEMessage == procedure_code)
                    {
                        if ( RRC_SUCCESS == check_for_mme_overload( 
                                    mme_overload_response,
                                    p_alloc_mme_req->rrc_establishment_cause,
                                    p_alloc_mme_req,
                                    p_s1ap_gb_ctx->mme_contexts[mme_id],
                                    &p_mme_info->mme_id_arr[p_mme_info->mme_idx_to_returned],
                                    match_result))
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "Assigned MME ID [%d]"
                                    " OVERLOADED STATE [%d]",
                                    mme_id, 
                                    mme_overload_response);
                            mme_overload_flag = RRC_TRUE;        
                        }
                        else
                        {
                            first_mme_index_rr = p_mme_info->mme_idx_to_returned;
                        }
                    }
                    else
                    {
                        first_mme_index_rr = p_mme_info->mme_idx_to_returned;
                    }
                }
            }
/* Bug 8198 Fix End */
        }
        p_mme_info->mme_idx_to_returned = 
            (U8)( (p_mme_info->mme_idx_to_returned + 1)%
                  p_mme_info->active_mme_count);
        tempCount--;
    }
    
    /*If PLMN id and MMEGI matches allocate that mme */
    if (first_mme_mmegi_index_rr != MAX_NUM_MME)
    {
        RRC_S1AP_TRACE(RRC_INFO,"[%s]:Registered MME found ID "
                "(only PLMN ID  and MMEGI matching) %d\n",
                __FUNCTION__,mme_id);

        p_mme_info->mme_idx_to_returned = first_mme_mmegi_index_rr;
        *p_mme_id  = p_mme_info->mme_id_arr[p_mme_info->mme_idx_to_returned].mme_id;

        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_SUCCESS;
    }
    /* If atleast PLMN ID is matched allocate that MME */
    else if (first_mme_index_rr != MAX_NUM_MME)
    {
        RRC_S1AP_TRACE(RRC_INFO,"[%s]:Registered MME found ID "
                "(only PLMN ID matching) %d\n",
                __FUNCTION__,mme_id);
        p_mme_info->mme_idx_to_returned = first_mme_index_rr;
        *p_mme_id  = p_mme_info->mme_id_arr[p_mme_info->mme_idx_to_returned].mme_id;

        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_SUCCESS;
    }
    else if(RRC_TRUE == mme_overload_flag)
    {
         RRC_S1AP_TRACE(RRC_INFO, "Assigned MME "
                " OVERLOADED, Releasing UE"
                " Establishment [%d]", p_alloc_mme_req->rrc_establishment_cause);
         /* Sending overload ind to UECC */
/*BUG 604 changes start*/
         build_and_send_mme_overload_ind(enb_ue_s1ap_id,
             p_s1ap_gb_ctx,p_alloc_mme_req->ue_index);
/*BUG 604 changes stop*/
         p_s1ap_gb_ctx->is_sel_mmes_overloaded = RRC_TRUE;
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_FAILURE;
}

/****************************************************************************
 * Function Name  : match_registered_mme_info
 * Inputs          :p_s1ap_gb_ctx - pointer to S1AP global context 
 *                  p_mme_ctx  - Pointer to mme context
 *                  p_alloc_mme_req - Conains registered_mme infomation
 *                                    that is matched with the one in
 *                                    mme context p_mme_ctx.
 * Outputs        : None
 * Returns        : RRC_SUCCESS - If match found.
 *                  RRC_FAILURE - If match not found.
 * Description    : This function compares Registered MME information 
 *                  p_alloc_mme_req with the one in p_mme_ctx.
 ****************************************************************************/
U8  match_registered_mme_info ( 
        s1ap_gb_context_t* p_s1ap_gb_ctx,
        mme_context_t*     p_mme_ctx,
        s1ap_allocate_mme_req_t * p_alloc_mme_req)
{
    list_t * p_mme_grp_id_list = PNULL;
    list_node_t * p_node = PNULL;
    served_gummei_info_t *p_served_gummeis = p_mme_ctx->p_served_gummeis;
    U8 served_gummei_lte = 0;
    U8 served_plmn_count = 0;
    U32 mme_grp_id_count = 0;
    U32 num_of_mme_group_ids = 0;
    U32 mmec_count = 0;
    s1ap_ServedPLMNs * p_servedPLMNs = PNULL;
    s1ap_ServedMMECs * p_served_mmecs = PNULL;

    RRC_ASSERT(PNULL != p_alloc_mme_req );
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx );
    RRC_ASSERT(PNULL != p_mme_ctx );
    
    if (!p_served_gummeis)
    {
        return NO_MATCHING_MME;
    }

    p_servedPLMNs = &p_served_gummeis[served_gummei_lte].servedPLMNs;

    for (served_plmn_count = 0; served_plmn_count < p_servedPLMNs->n; 
            served_plmn_count++)
    {
        /* Bug 488 Fix Start */
        if ((p_servedPLMNs->elem[served_plmn_count].numocts ==
               p_alloc_mme_req->plmn_identity.numocts) &&
             ( !memcmp_wrapper(p_servedPLMNs->elem[served_plmn_count].data,
                    p_alloc_mme_req->plmn_identity.data,
                    p_alloc_mme_req->plmn_identity.numocts)))
        /* Bug 488 Fix End */
        {
            break;
        }
    }

    if (served_plmn_count == p_servedPLMNs->n)
    {
        return NO_MATCHING_MME;
    }

    if ((p_alloc_mme_req->bitmask & REGISTERED_MMEGI_PRESENT) ||
            (p_alloc_mme_req->bitmask & REGISTERED_MMEC_PRESENT))
    {
        if (p_alloc_mme_req->bitmask & REGISTERED_MMEGI_PRESENT)
        {
            p_mme_grp_id_list = &p_served_gummeis[served_gummei_lte].
                mme_group_id_list;
            num_of_mme_group_ids = list_count(p_mme_grp_id_list);

            p_node = list_first_node(p_mme_grp_id_list);

            for (mme_grp_id_count = 0; mme_grp_id_count < num_of_mme_group_ids;
                    mme_grp_id_count++)
            {
                /* Bug 488 Fix Start */
                if ((((mme_group_id_list_node_t*) p_node)->mme_group_id.numocts == 
                         p_alloc_mme_req->mme_group_id.numocts ) &&
                    ( !memcmp_wrapper(((mme_group_id_list_node_t*) p_node)-> 
                                mme_group_id.data ,p_alloc_mme_req->mme_group_id.data,
                            p_alloc_mme_req->mme_group_id.numocts)))
                /* Bug 488 Fix End */
                {
                    break;
                }
                p_node = list_next_node(p_node);
            }

            if (mme_grp_id_count == num_of_mme_group_ids)
            {
                /*SPR 22789 Fix Start */
                /* Code Deleted */

                RRC_S1AP_TRACE(RRC_INFO,"Partial MATCHING_MME found: mmegi doesn't match");
                return PARTIAL_MATCHING_MME;
                /*SPR 22789 Fix End */
            }
        }

        if (p_alloc_mme_req->bitmask & REGISTERED_MMEC_PRESENT)
        {
            p_served_mmecs = &p_served_gummeis[served_gummei_lte].
                served_mmecs;

            for (mmec_count = 0; mmec_count < p_served_mmecs->n;
                    mmec_count++)
            {
                /* Bug 488 Fix Start */
                if ( (p_served_mmecs->elem[mmec_count].numocts ==
                        p_alloc_mme_req->mmec.numocts) && 
                     (!memcmp_wrapper(p_served_mmecs->elem[mmec_count].data,
                         p_alloc_mme_req->mmec.data,
                            p_alloc_mme_req->mmec.numocts)))
                /* Bug 488 Fix End */
                {
                    /* SPR 22789 Fix Start */
                    RRC_S1AP_TRACE(RRC_INFO,"MATCHING_MME found");
                    return MATCHING_MME;
                }
            }

            if (mmec_count == p_served_mmecs->n)
            {
                RRC_S1AP_TRACE(RRC_INFO,"MATCHING_MMEGI found mmec doesn't match");
                /* SPR 22789 Fix End */
                return PARTIAL_MATCHING_MMEGI_MME;
            }
        }
    }
    /* SPR 22789 Fix Start */ 
    /* Code Deleted */
    /* SPR 22789 Fix End */ 

    RRC_S1AP_TRACE(RRC_INFO,"PARTIAL_MATCHING_MME found");

    return PARTIAL_MATCHING_MME;
}

/****************************************************************************
 * Function Name  : s1ap_allocate_stream_id
 * Inputs          :p_mme_ctx  : Pointer to mme context
 * Outputs        : None
 * Returns        : stream_id
 * Description    : This function is called to  allocate  stream id  by 
 *                  round robin algorithm applied on the active streams in mme
 ****************************************************************************/
U8  s1ap_allocate_stream_id ( mme_context_t * p_mme_ctx)
{
    U8 stream_id = MAX_NUM_SCTP_STREAM ;
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_mme_ctx );

    if ( p_mme_ctx->current_mme_state == MME_ACTIVE)
    {
//#ifndef RRC_UNIT_TEST_FRAMEWORK
        stream_info_t *p_sctp_stream_info = &(p_mme_ctx->stream_info);
        if (S1AP_ZERO == p_sctp_stream_info->number_of_streams)
        {
            RRC_S1AP_TRACE(RRC_ERROR,"[%s]:No Stream  present, hence cannot"
                    " allocate the streams ID\n",__FUNCTION__);
            RRC_S1AP_UT_TRACE_EXIT();
            return stream_id;
        }
        if (MAX_NUM_SCTP_STREAM > (p_sctp_stream_info->stream_idx_returned + 1))
        {
        /*fix 114288 by wood at 19-06-19 begin*/
            p_sctp_stream_info->stream_idx_returned++;

            if (p_sctp_stream_info->stream_idx_returned ==
                    (p_sctp_stream_info->number_of_streams))
            {
                p_sctp_stream_info->stream_idx_returned = 0;
            }

			stream_id = (U8) p_sctp_stream_info->
				stream_ids[p_sctp_stream_info->stream_idx_returned];
		 /*fix 114288 by wood at 19-06-19 end*/

            RRC_S1AP_UT_TRACE_EXIT();
            return stream_id;
        }
//#else
//        return 0;
//#endif
    }
    else
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[%s]:MME in NOT in ACTIVE, hence cannot"
                " allocated the streams from  it\n",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return stream_id;
    }
    RRC_S1AP_UT_TRACE_EXIT();
    return stream_id;
}


/*****************************************************************************
 * Function Name  : fill_and_send_error_ind_msg
 * Inputs         : p_mme_context - pointer to MME context
 *                  ue_id_pair_presence - chexk for ue id pair presnce
 *                  enb_ue_s1ap_id - id of ue over s1 interface in enB 
 *                  mme_ue_s1ap_id - id of ue over s1 interface in MME
 *                  s1ap_cause  - cause
 *                  p_stream_id - stream id
 * Outputs        : none 
 * Returns        : none
 * Description    : This function fills and sends error indication message 
 *                  to the MME.
 ****************************************************************************/
void fill_and_send_error_ind_msg(mme_context_t     *p_mme_context,
                        U8                   ue_id_pair_presence,
                        U32             enb_ue_s1ap_id,            
                        U32             mme_ue_s1ap_id,
                        U32             s1ap_cause,
                        U16            *p_stream_id
                        )
{
    rrc_s1ap_error_indication_t    *p_err_ind = PNULL;

    p_err_ind = rrc_mem_get(sizeof(rrc_s1ap_error_indication_t));

    if (p_err_ind == PNULL)
    {
        RRC_S1AP_TRACE(RRC_ERROR,"[%s]: Memory Allocation Failed",
                __FUNCTION__);
        return;
    }

    memset_wrapper(p_err_ind, 0, sizeof(rrc_s1ap_error_indication_t));

    if (ue_id_pair_presence & ENB_UE_S1AP_ID_PRESENT)
    {
        p_err_ind->bitmask |= 
           RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT;
        p_err_ind->enb_ue_s1ap_id = enb_ue_s1ap_id;
    }
    if (ue_id_pair_presence & MME_UE_S1AP_ID_PRESENT)
    {
        p_err_ind->bitmask |= 
            RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT;
        p_err_ind->mme_ue_s1ap_id = mme_ue_s1ap_id;
    }

    p_err_ind->bitmask |=
        RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT;
    p_err_ind->cause.t = T_s1ap_Cause_radioNetwork;
    p_err_ind->cause.u.radioNetwork =
        s1ap_cause;


    s1ap_build_and_send_err_indication(p_mme_context, p_err_ind, p_stream_id);
    rrc_mem_free(p_err_ind);
}

/*****************************************************************************
 * Function Name  : process_dl_nas_msg
 * Inputs         : pp_s1ap_context - Pointer to the S1AP context of a UE
 *                  enb_ue_s1ap_id - id of ue over s1 interface in enB
 *                  mme_ue_s1ap_id - id of ue over s1 interface in MME
 *                  p_mme_context - Pointer to mme context of a MME
 *                  p_api - Pointer to asn decoded msg from MME
 *                  asn_msg_len - Length of the asn decoded msg
 * Outputs        : None 
 * Returns        : none
 * Description    : This function processes the downlink ue associated
 *                  NAS message or the initial context set up request
 *                  from MME and send the indication to uecc module.
 ****************************************************************************/
rrc_return_et s1ap_process_dl_nas_msg(    
        s1ap_gb_context_t *p_s1ap_gb_ctx ,
        ue_s1ap_context_t    **pp_s1ap_context, 
        U32             enb_ue_s1ap_id,            
        U32             mme_ue_s1ap_id,             
        mme_context_t     *p_mme_context,     
        void             *p_asn_msg,                  
        U16             asn_msg_len,
        U8              ue_id_pair_presence,
        U16             stream_id,
        s1ap_ProcedureCode procedureCode,
        s1ap_message_type_et  message_type)
{
    search_tree_node_t *p_node = PNULL;
    mmeUeS1apId_search_node_t *p_insert_node = PNULL;
    s1ap_ue_associated_sig_msg_ind_t    *msg_p = PNULL;

    void *p_intrl_msg            = PNULL;

    U32 deleted_enb_ue_s1apid = 0xFFFFFFFF;
            /*SPR_21274_FIX_START*/
    U32 deleted_enb_ue_s1apid_1 = 0xFFFFFFFF;
            /*SPR_21274_FIX_STOP*/
    s1ap_reset_msg_ind_t s1ap_reset_ind;
    /*SPR 21220 Start*/
    /* SPR 17919 Fix Start */
    U32 ctx_ue_index = 0xFFFFFFFF;
    /* SPR 17919 Fix Stop */
    U32 ue_index = 0xFFFFFFFF;
    /*SPR 21220 Stop*/
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_mme_context);

    memset_wrapper((void*)&s1ap_reset_ind,RRC_NULL,sizeof(s1ap_reset_msg_ind_t));

    do 
    {

        /* SPR 4100 Start */
        if ((pp_s1ap_context != PNULL) && (*pp_s1ap_context != PNULL))
        {
            if (p_mme_context->mme_id != (*pp_s1ap_context)->mme_id)
            {
                RRC_S1AP_TRACE(RRC_INFO,
                               "MME ID = %d is not equal to UE Context "
                               "MME_ID = %d",
                               p_mme_context->mme_id,
                               (*pp_s1ap_context)->mme_id);

                    fill_and_send_error_ind_msg(p_mme_context,
                               ue_id_pair_presence,
                               enb_ue_s1ap_id,
                               mme_ue_s1ap_id,
                               s1ap_unknown_pair_ue_s1ap_id,
                               &stream_id);
                    return RRC_FAILURE;
            }
         }
        /* SPR 4100 Stop */

        
        if ( (ue_id_pair_presence & ENB_UE_S1AP_ID_PRESENT) && 
                (pp_s1ap_context != PNULL) &&
                (*pp_s1ap_context == PNULL))
        {

/*BUG 604 changes start*/
            s1ap_reset_ind.bitmask |= 
                S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT;
/*BUG 604 changes stop*/
            /* enb ue s1apId is invalid, hence finding if there is 
             * any entry of mme ue s1apId in search tree. If found
             * Include it in S1AP RESET IND msg and relsease it's resoures
             * in S1AP*/
            if ( (ue_id_pair_presence & MME_UE_S1AP_ID_PRESENT))
            {
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
/*BUG 604 changes stop*/

                    s1ap_build_and_send_reset_ind_msg(
                            &s1ap_reset_ind,RRC_UECCMD_MODULE_ID);

                    RRC_S1AP_TRACE(RRC_ERROR, "[%s] enb Ue s1ap id INVALID"
                            " Sending S1AP RESET IND for UE_INDEX %u",
                            __FUNCTION__,ue_index);

                }
                /*SPR 21220 Stop*/

            }

                /* SPR 2551 Start */
                    fill_and_send_error_ind_msg(p_mme_context,
                               ue_id_pair_presence,
                               enb_ue_s1ap_id,
                               mme_ue_s1ap_id,
                               s1ap_unknown_enb_ue_s1ap_id,
                               &stream_id);
                /* SPR 2551 Start */

            return RRC_FAILURE;

        }
        else if ((ue_id_pair_presence & MME_UE_S1AP_ID_PRESENT) &&
                (pp_s1ap_context != PNULL) &&
                (*pp_s1ap_context == PNULL))
        {
            /* we will forward this message to UECC as 
             * we need error handling */
            break;
            
        }
        else if ((ue_id_pair_presence & MME_UE_S1AP_ID_PRESENT) &&
                (PNULL != pp_s1ap_context) && (PNULL != *pp_s1ap_context) &&
                !((*pp_s1ap_context)->is_mme_ue_s1ap_id_valid))
        {
            /* to call a function whose input is the address of the 
               search tree whose value is mme_ue_s1ap_id */
            p_node = st_find_tree_node(
                    &p_mme_context->mmeUeS1apId_search_tree,
                    &mme_ue_s1ap_id);

            if ( (PNULL != p_node) &&
                   (ASN1V_s1ap_id_PathSwitchRequest == procedureCode) )
            {
                st_delete_node(
                        (search_tree_t *)&(p_mme_context->mmeUeS1apId_search_tree),
                        (search_tree_node_t *) p_node); 

                if (PNULL != p_node)
                {
                    rrc_mem_free((void*)p_node);
                    p_node = PNULL;
                }
            }

            if (PNULL != p_node)
            {
                /*SPR 21220 Start*/
                /* SPR 17919 Fix Start */
                ctx_ue_index = (U32)((*pp_s1ap_context)->ue_index);
                /* SPR 17919 Fix Stop */
/*BUG 604 changes start*/
                s1ap_reset_ind.bitmask |= 
                    S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT;
/*BUG 604 changes stop*/
                /* Prepare S1AP RESET IND msg to be sent to uecc */
                /* Fill reset indication message fields */

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
/*BUG 604 changes stop*/
                }

                if(ue_index != ctx_ue_index)
                {
                if ( ue_id_pair_presence & ENB_UE_S1AP_ID_PRESENT)
                {
/*BUG 604 changes start*/
                    s1ap_reset_ind.ue_index_list.ue_index[
                        s1ap_reset_ind.ue_index_list.count] =
                        /* SPR 17919 Fix Start */
                            ctx_ue_index;
                        /* SPR 17919 Fix Stop */
                    s1ap_reset_ind.ue_index_list.count++;
                        /*BUG 604 changes stop*/
                    }
                }
                /*SPR 21220 Stop*/
                
                /* Fix for RRC panic due to double memory free - START*/
                /*SPR 21220 Start*/
                /* SPR 17919 Fix Start */
                if ( (PNULL != pp_s1ap_context) && (*pp_s1ap_context != PNULL) &&
                        (ue_index != ctx_ue_index) )
        		/* SPR 17919 Fix Stop */
                /*SPR 21220 Stop*/
                {
                    /* DEBUG_DL_NAS Start */
                    /* SPR 22789 Fix Start */ 
                    RRC_S1AP_TRACE(RRC_INFO, "Releasing pp_s1ap_context is [%p]", *pp_s1ap_context);
                    /* DEBUG_DL_NAS End */
                    RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx,*pp_s1ap_context,enb_ue_s1ap_id);
                    /* SPR 22789 Fix End */
                }
                else
                {
                    RRC_S1AP_TRACE(RRC_INFO, "pp_s1ap_context is already freed");
                }
                /* Fix for RRC panic due to double memory free - END*/
                /* SPR 22789 Fix Start */
                /* Code Deleted */
                /* SPR 22789 Fix End */
                
                /*SPR 21220 Start*/
                if(deleted_enb_ue_s1apid != enb_ue_s1ap_id)
                {
                /*BUG 604 changes start*/
                /*Free s1ap_ue_id Ctx*/
                s1ap_ueid_free(p_s1ap_gb_ctx,
                        enb_ue_s1ap_id);
                }
                /*SPR 21220 Stop*/
                /*BUG 604 changes stop*/
                /* SPR 17919 Fix Start */
                /* Code Deleted */
                /* SPR 17919 Fix Stop */

                s1ap_build_and_send_reset_ind_msg( 
                        &s1ap_reset_ind,RRC_UECCMD_MODULE_ID);

                /*SPR_21818_START*/
                RRC_S1AP_TRACE(RRC_ERROR, "[%s] mme Ue s1ap id INVALID"
                        " Entry in search tree found"
                        " Sending S1AP RESET IND for enbues1apid %u",
                        __FUNCTION__,enb_ue_s1ap_id);
                /*SPR_21818_END*/

                /* SPR 2551 Start */
                    fill_and_send_error_ind_msg(p_mme_context,
                               ue_id_pair_presence,
                               enb_ue_s1ap_id,
                               mme_ue_s1ap_id,
                               s1ap_unknown_mme_ue_s1ap_id,
                               &stream_id);
                /* SPR 2551 Stop */

                return RRC_FAILURE;
            }
            else
            {
                /*SPR_21818_START*/
                RRC_S1AP_TRACE(RRC_INFO,"[%s]:UE Assoc S1 ctx "
                        "created for enbUeS1apid %u"
                        " mmeUes1apid %u cell id %u ue id %u",__FUNCTION__,
                        enb_ue_s1ap_id,mme_ue_s1ap_id,
                        s1ap_get_cell_id(p_s1ap_gb_ctx, enb_ue_s1ap_id),
                        s1ap_get_ue_id(enb_ue_s1ap_id));
                /*SPR_21818_END*/

                /* Set the following fields if s1ap context is valid and if 
                   mme_ue_s1ap_id has no entry in search tree */
                (*pp_s1ap_context)->mme_ue_s1ap_id = mme_ue_s1ap_id;
                (*pp_s1ap_context)->is_mme_ue_s1ap_id_valid = RRC_TRUE;

                p_insert_node = rrc_mem_get(sizeof(mmeUeS1apId_search_node_t));
                if(PNULL == p_insert_node)
                {
                    RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
                    return RRC_FAILURE;
                }
                if (PNULL != p_insert_node)
                {
                    p_insert_node->mmeUeS1apId = mme_ue_s1ap_id;
                    p_insert_node->enbUeS1apId = enb_ue_s1ap_id;
                    /* calling function to insert the mme_ue_s1ap_id in tree */
                    st_insert_node(
                            &p_mme_context->mmeUeS1apId_search_tree,
                            (search_tree_node_t*) p_insert_node);
                }
            }
        }        
        else if ((ue_id_pair_presence & MME_UE_S1AP_ID_PRESENT) &&
                (PNULL != pp_s1ap_context) && (PNULL != *pp_s1ap_context) &&
                mme_ue_s1ap_id != (*pp_s1ap_context)->mme_ue_s1ap_id)
        {
            /*BUG 604 changes start*/
            s1ap_reset_ind.bitmask |= 
                S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT;
            /*BUG 604 changes stop*/

            /*SPR_21274_FIX_START*/
            /*code deleted*/
            ue_index = delete_s1ap_context((*pp_s1ap_context)->mme_ue_s1ap_id,
                    p_mme_context,
                    &deleted_enb_ue_s1apid_1,
                    p_s1ap_gb_ctx);

            if (deleted_enb_ue_s1apid_1 != 0xFFFFFFFF)
            {        
                RRC_S1AP_TRACE(RRC_INFO, "[%s]Deleting context "
                        "for enbues1apid %d, corresponding to stored mmeUeS1APId"
                        "and adding it to RESET IND list \n",
                        __FUNCTION__,deleted_enb_ue_s1apid_1);
                s1ap_reset_ind.ue_index_list.ue_index[
                    s1ap_reset_ind.ue_index_list.count] =
                    deleted_enb_ue_s1apid_1;
                s1ap_reset_ind.ue_index_list.count++;
            }
            /*SPR_21274_FIX_STOP*/
            RRC_S1AP_TRACE(RRC_ERROR, "[%s]: MME ue s1ap id %d is inconsistent"
                    " with the one present in ue context %d and ue_index=%d\n",
                    __FUNCTION__,mme_ue_s1ap_id,
                    (*pp_s1ap_context)->mme_ue_s1ap_id,ue_index);

            /*SPR 21220 Start*/
            /* Code to add (*pp_s1ap_context)->ue_index in RESET list
               has been moved after ue_index 'if' */
            ue_index = delete_s1ap_context(mme_ue_s1ap_id,
                    p_mme_context,
                    &deleted_enb_ue_s1apid,
                    p_s1ap_gb_ctx);

            if (ue_index != 0xFFFFFFFF)
            {        
                RRC_S1AP_TRACE(RRC_INFO, "[%s]Deleting context "
                        "for UE_INDEX %u "
                        "and adding it to RESET IND list",
                        __FUNCTION__,ue_index);
                /*BUG 604 changes start*/ 
                s1ap_reset_ind.ue_index_list.ue_index[
                   s1ap_reset_ind.ue_index_list.count] =
                    ue_index;
                s1ap_reset_ind.ue_index_list.count++;
    /*BUG 604 changes stop*/ 
            }

            if(ue_index != (*pp_s1ap_context)->ue_index)
            {
                if ( ue_id_pair_presence & ENB_UE_S1AP_ID_PRESENT)
                {
                    /*BUG 604 changes start*/ 
                    s1ap_reset_ind.ue_index_list.ue_index[
                        s1ap_reset_ind.ue_index_list.count] =
                        (*pp_s1ap_context)->ue_index;
                    s1ap_reset_ind.ue_index_list.count++;
                    RRC_S1AP_TRACE(RRC_ERROR, "[%s] mme Ue s1ap id INVALID"
                            " Sending S1AP RESET IND for UE_INDEX %u",
                            __FUNCTION__,(*pp_s1ap_context)->ue_index);
                    /*BUG 604 changes stop*/ 
                    if(deleted_enb_ue_s1apid != (*pp_s1ap_context)->enb_ue_s1ap_id)
                    {
                        set_ue_ctx_from_ue_id(p_s1ap_gb_ctx, PNULL,
                                (*pp_s1ap_context)->enb_ue_s1ap_id);
                        s1ap_ueid_free(p_s1ap_gb_ctx,
                                (*pp_s1ap_context)->enb_ue_s1ap_id);

                        /*SPR_21818_START*/
                        p_node = st_find_tree_node(
                                &p_mme_context->mmeUeS1apId_search_tree,
                                (void*)(*pp_s1ap_context)->mme_ue_s1ap_id);
                        if (PNULL != p_node)
                        {
                            st_delete_node(
                                    (search_tree_t *)&(p_mme_context->mmeUeS1apId_search_tree),
                                    (search_tree_node_t *) p_node); 
                            rrc_mem_free((void*)p_node);
                            p_node = PNULL;
                        }
                        /*SPR_21818_END*/
                    }
                }
            }

            s1ap_build_and_send_reset_ind_msg( 
                    &s1ap_reset_ind,RRC_UECCMD_MODULE_ID);
            /*SPR 21220 Stop*/

            /* SPR 2551 Start */
            RRC_S1AP_TRACE(RRC_ERROR,
                    " MME_UE_S1AP_ID MATCH FAIL ");

                    fill_and_send_error_ind_msg(p_mme_context,
                               ue_id_pair_presence,
                               enb_ue_s1ap_id,
                               mme_ue_s1ap_id,
                               s1ap_unknown_pair_ue_s1ap_id,
                               &stream_id);
            /* SPR 2551 Stop */

            return RRC_FAILURE;
        }
    }
    while (0);
    /*REL 1.2:HANDOVER PROCEDURE START*/
    if (p_asn_msg != PNULL)
        /*REL 1.2:HANDOVER PROCEDURE END*/
    {
        p_intrl_msg = s1ap_alloc_intrl_msg(
                RRC_UECC_MODULE_ID,
                S1AP_UE_ASSOCIATED_SIG_MSG_IND,     
                (U16)(sizeof(s1ap_ue_associated_sig_msg_ind_t) + asn_msg_len));

        if (PNULL == p_intrl_msg)
        {
            /*out of memory*/
            RRC_S1AP_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        msg_p = (s1ap_ue_associated_sig_msg_ind_t *)((U8 *)p_intrl_msg +
                RRC_API_HEADER_SIZE);
        msg_p->bitmask |= UE_ASSOC_SIG_MSG_IND_MME_ID_PRESENT;
        msg_p->mme_id = p_mme_context->mme_id;
        msg_p->bitmask |= UE_ASSOC_SIG_MSG_IND_REL_OF_MME_PRESENT;
        msg_p->rel_of_mme = p_mme_context->connection_info.rel_of_mme;
        msg_p->message_type = message_type;

/*BUG 604 changes start*/
        if ((pp_s1ap_context != PNULL) && (*pp_s1ap_context != PNULL))
        {
             msg_p->ue_index = (*pp_s1ap_context)->ue_index;
        }
/*BUG 604 changes stop*/
        l3_memcpy_wrapper((void*)((U8*)p_intrl_msg + RRC_API_HEADER_SIZE +
                    sizeof(s1ap_ue_associated_sig_msg_ind_t)),
                (const void*)p_asn_msg,
                asn_msg_len);
        msg_p->s1ap_procedure_code = procedureCode;
        if (ue_id_pair_presence & MME_UE_S1AP_ID_PRESENT)
        {
            msg_p->bitmask |=  UE_ASSOC_SIG_MSG_IND_MME_UE_S1APID_PRESENT;
            msg_p->mme_ue_s1apid =  mme_ue_s1ap_id;
        }

        if ( ue_id_pair_presence & ENB_UE_S1AP_ID_PRESENT)
        {
            RRC_S1AP_TRACE(RRC_INFO, "[%s]: Forwarding message to UECC\n",
                    __FUNCTION__);
            /*BUG 604 changes start*/
            if ((RRC_GET_UECC_MODULE_ID(msg_p->ue_index) - 
                        RRC_UECC_1_MODULE_ID) < p_s1ap_gb_ctx->
                    no_of_uecc_instances)
            {
                /* Send response */
                rrc_send_message(
                        p_intrl_msg,
                        RRC_GET_UECC_MODULE_ID(msg_p->ue_index));
            }
            /*BUG 604 changes stop*/
            else
            {
		RRC_S1AP_TRACE(RRC_WARNING,"UECC instance does not exist");
		/* Coverity Fix 48268 Start */
		if ( PNULL != p_intrl_msg )
		{
		    /* SPR 15887 Start */
		    rrc_msg_mem_free(p_intrl_msg);
		    /* SPR 15887 Stop */
		    p_intrl_msg = PNULL;
		}
    		    /* Coverity Fix 48268 Stop */
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
    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}


/*SPR 21220 Start*/
/****************************************************************************
 * Function Name  : delete_s1ap_context
 * Inputs          :mme_ue_s1ap_id - id of ue over S1 interface in MME
 *                  p_mme_ctx  - Pointer to mme context
 *                  p_deleted_enb_ue_s1apid - pointer to deleted eNB UE S1AP Id
 *                  p_s1ap_gb_ctx - pointer to S1AP global context
 * Outputs        : None
 * Returns        : ue_index 
 * Description    : This function is called to delete s1ap context and
 *                  entry in search tree corresonding to mme_ue_s1ap_id
 ****************************************************************************/
U32 delete_s1ap_context( U32 mme_ue_s1ap_id,
        mme_context_t * p_mme_context,
        U32* p_deleted_enb_ue_s1apid,
        s1ap_gb_context_t *p_s1ap_gb_ctx)
{
    U32 ue_index = 0xFFFFFFFF;
    /*SPR 21220 Stop*/
    U8 cell_id      = MAX_NUM_CELLS;
    /* CID: 113465 Start */
    U16 ue_id       = RRC_NULL;
    /* CID: 113465 End */
    mmeUeS1apId_search_node_t *p_delete_node = PNULL;
    ue_s1ap_context_t     *p_s1ap_ue_context = PNULL;

    p_delete_node = (mmeUeS1apId_search_node_t*) st_find_tree_node(
            &p_mme_context->mmeUeS1apId_search_tree,
            &mme_ue_s1ap_id);

    if (PNULL != p_delete_node)
    {
        cell_id     = s1ap_get_cell_id(p_s1ap_gb_ctx, p_delete_node->enbUeS1apId);
        if(MAX_NUM_CELLS <= cell_id)
        {
            RRC_S1AP_TRACE(RRC_WARNING, 
                    " [%s] Invalid Cell Id\n",__FUNCTION__);
            /*SPR_21818_START*/
            return ue_index;
            /*SPR_21818_END*/
        }    
        ue_id       = s1ap_get_ue_id(p_delete_node->enbUeS1apId);
        if(MAX_NUM_UE <= ue_id)
        {
            RRC_S1AP_TRACE(RRC_WARNING, "[%s] Invalid UE Id\n",__FUNCTION__);
            /*SPR_21818_START*/
            return ue_index;
            /*SPR_21818_END*/
        }    
        
        get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
                &p_s1ap_ue_context, p_delete_node->enbUeS1apId);

        if ((p_s1ap_gb_ctx->cell_s1ap_contexts[cell_id]) &&
                (p_s1ap_ue_context))
        {
            /*BUG 604 changes start*/
            /*SPR 21220 Start*/
            ue_index = (U32)p_s1ap_ue_context->ue_index;
            /*SPR 21220 Stop*/
            /*BUG 604 changes stop*/

            /* SPR 22789 Fix Start */
            RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx,p_s1ap_ue_context,p_delete_node->enbUeS1apId);
            /* SPR 22789 Fix End */
        }

        /*SPR 21220 Start*/
        *p_deleted_enb_ue_s1apid = p_delete_node->enbUeS1apId; 
        /*SPR 21220 Stop*/
        /*SPR_21818_START*/
        st_delete_node(
                (search_tree_t *)&(p_mme_context->mmeUeS1apId_search_tree),
                (search_tree_node_t *) p_delete_node); 

        if (PNULL != p_delete_node)
        {
            rrc_mem_free((void*)p_delete_node);
	    /* + Coverity 81428 */
            //p_delete_node = PNULL;
	    /* - Coverity 81428 */
        }
        /*BUG 604 changes start*/
        /*Free s1ap_ue_id Ctx*/
        s1ap_ueid_free(p_s1ap_gb_ctx,(*p_deleted_enb_ue_s1apid));
        /*BUG 604 changes stop*/
        /*SPR_21818_END*/

    }
    else 
    {
        RRC_S1AP_TRACE(RRC_WARNING,"[%s] Entry not found in search tree\n",
                __FUNCTION__);
    }
    return ue_index;
}

/****************************************************************************
 * Function Name  : process_uecc_failure_msg_req
 * Inputs         : p_s1ap_gb_ctx -  Pointer to S1AP global ctx
 *                  p_api - Pointer to api buffer received from UECC.
 * Outputs        : None
 * Returns        : void
 * Description    : This function processes UECC failure msg req.
 ****************************************************************************/
void  process_uecc_failure_msg_req(
        s1ap_gb_context_t * p_s1ap_gb_ctx,  
        U8 * p_api) 
{
    s1ap_reset_msg_ind_t     s1ap_reset_ind ;
    s1ap_failure_msg_req_t * p_s1ap_failure_msg_req = 
        (s1ap_failure_msg_req_t*)p_api;
    ue_s1ap_context_t * p_s1ap_ctx      = PNULL;
    U8 cell_id      = MAX_NUM_CELLS;
    /* CID: 113468 Start */
    U16 ue_id       = RRC_NULL;
    /* CID: 113468 End */
    U16 ue_count    = 0;
    U16 max_ue_count    = 0;
    U32 enb_ue_s1ap_id       = 0;
    s1ap_enb_reset_info_t  reset_req;


    RRC_S1AP_UT_TRACE_ENTER();

    RRC_S1AP_TRACE(RRC_INFO,"[%s]:S1AP FAILURE MSG REQ received from UECC \n"
            ,__FUNCTION__);

    memset_wrapper((void*)&reset_req, 0 ,
            sizeof(s1ap_enb_reset_info_t));
    memset_wrapper((void*)&s1ap_reset_ind,RRC_NULL,sizeof(s1ap_reset_msg_ind_t));

	/*bugid 117336 begin*/
	if(p_s1ap_failure_msg_req->enb_ue_s1ap_id_list.count > p_s1ap_gb_ctx->total_ue_supported)
	{
		RRC_S1AP_TRACE(RRC_WARNING,"[%s]:S1AP_FAILURE_MSG_REQ para error : enb ue count is more than total_ue_supported\n"
	            ,__FUNCTION__);
		return;
	}	
	else if(p_s1ap_failure_msg_req->enb_ue_s1ap_id_list.count == 0 )
	{
		/*reset all */
		RRC_S1AP_TRACE(RRC_INFO,"[%s]:S1AP_FAILURE_MSG_REQ:enb_ue_s1ap_id_list.count = 0\n"
	            ,__FUNCTION__);

		/* Build and send S1AP_RESET_MSG_IND to UECC with bitmask field set to zero */
    	s1ap_reset_ind.bitmask = 0;
    	s1ap_build_and_send_reset_ind_msg(&s1ap_reset_ind,RRC_UECCMD_MODULE_ID);

		/*Build RESET MSG to MME with  choice type :s1 interface (RESET ALL) */

	    reset_req.num_of_items = 0;
	    /**** Cause ****/
	    /* BUG_9257 START */
	    reset_req.cause.t = T_s1ap_Cause_misc;
	    reset_req.cause.u.radioNetwork = s1ap_om_intervention;
	    /* BUG_9257 END */
	    s1ap_add_and_process_reset_context(
	            p_s1ap_gb_ctx,
	            &reset_req,
	            RRC_OAM_MODULE_ID,0,0,0);
	}
	else
	/*bugid 117336 begin*/
	{
	    RRC_S1AP_TRACE(RRC_INFO,"[%s]:S1AP_FAILURE_MSG_REQ_ENB_UE_S1AP_"
	            "ID_LIST_PRESENT present\n"
	            ,__FUNCTION__);
	    max_ue_count = p_s1ap_failure_msg_req->enb_ue_s1ap_id_list.count;

		/*BUG 604 changes start*/
	    s1ap_reset_ind.bitmask |= 
	        S1AP_RESET_MSG_IND_UE_INDEX_LIST_PRESENT;
	    for (ue_count = 0; ue_count < max_ue_count; ue_count++)
	    {
	        SET_UE_INDEX(ue_count);

	        enb_ue_s1ap_id = p_s1ap_failure_msg_req->enb_ue_s1ap_id_list.
	            enb_ue_s1ap_id[ue_count];

	        get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
	                &p_s1ap_ctx, enb_ue_s1ap_id);

	        if ( PNULL == p_s1ap_ctx )
	        {
	            RRC_S1AP_TRACE(RRC_DETAILED,
	                    "UE context doesn't exist");
	            continue;
	        }
	        s1ap_reset_ind.ue_index_list.ue_index[
	            s1ap_reset_ind.ue_index_list.count]= p_s1ap_ctx->ue_index;

	        s1ap_reset_ind.ue_index_list.count++;
	    }
		/*BUG 604 changes stop*/ 

		if (s1ap_reset_ind.ue_index_list.count == 0)
		{
			RRC_S1AP_TRACE(RRC_INFO,
	                    "no found relative UE context, abort from function");
			return;
		}
	    /* Should be sent cell index present in s1ap failure msg req */
	    s1ap_build_and_send_reset_ind_msg(&s1ap_reset_ind,RRC_UECCMD_MODULE_ID);

	    for (ue_count = 0; ue_count < max_ue_count; ue_count++)
	    {
	        enb_ue_s1ap_id = p_s1ap_failure_msg_req->enb_ue_s1ap_id_list.
	            enb_ue_s1ap_id[ue_count];
	        cell_id     = s1ap_get_cell_id(p_s1ap_gb_ctx, enb_ue_s1ap_id);
	        ue_id       = s1ap_get_ue_id(enb_ue_s1ap_id);
	 
	        SET_CELL_AND_UE_INDEX(cell_id, ue_id);

	        if (cell_id >= MAX_NUM_CELLS || 
	                !p_s1ap_gb_ctx->cell_s1ap_contexts[cell_id])
	        {
	            RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Error ::"
	                    "Cell id %d is invalid \n"
	                    ,__FUNCTION__,cell_id);
	            continue ;
	        }

	        if (ue_id >= MAX_NUM_UE)
	        {
	            RRC_S1AP_TRACE(RRC_ERROR,"[%s]:Error ::UE id %d is invalid \n"
	                    ,__FUNCTION__,ue_id);
	            continue ;
	        }

	        get_ue_ctx_from_ue_id(p_s1ap_gb_ctx,
	                &p_s1ap_ctx, enb_ue_s1ap_id);

	        if ( PNULL == p_s1ap_ctx )
	        {
	            RRC_S1AP_TRACE(RRC_DETAILED,
	                    "[%s]: UE context doesn't exist at %d\n",
	                    __FUNCTION__,ue_id);
	            continue;
	        }
	        reset_req.enbUeS1apId_arr[reset_req.num_of_items].enbUeS1apId = 
	            (U32) (ue_id);
	        reset_req.enbUeS1apId_arr[reset_req.num_of_items].mme_id = 
	            p_s1ap_ctx->mme_id;
	        reset_req.num_of_items++;
	    }

	    /*Build RESET MSG to MME with 
	     * choice type :part of s1 interface */

	    /**** Cause ****/
	    reset_req.cause.t = T_s1ap_Cause_radioNetwork;
	    reset_req.cause.u.radioNetwork = 
	        s1ap_release_due_to_eutran_generated_reason;
	}
    /* Delete Temporary HO List if mmeUeS1apId List present in failure Req */
    if (p_s1ap_failure_msg_req->bitmask & S1AP_FAILURE_MSG_REQ_MME_UE_S1AP_ID_LIST_PRESENT)
    {
        delete_ue_ctx_from_ho_temp_list(p_s1ap_gb_ctx, 
               &( p_s1ap_failure_msg_req->mme_ue_s1ap_id_list));
    }

    s1ap_add_and_process_reset_context(
            p_s1ap_gb_ctx,
            &reset_req,
            (RRC_MAX_EXT_MODULE_ID +1),0,0,0);

}
/*REL 1.2:HANDOVER PROCEDURE START*/
/*****************************************************************************
 * Function Name  : search_ho_list_node
 * Inputs         : p_s1ap_gb_ctx - Pointer to S1AP global context.
 *                  p_ue_associated_sig_req - Pointer to
 *                                            s1ap_ue_associated_sig_msg_req.
 * Outputs        : None
 * Returns        : ho_ongoing_ue_ctx_node
 * Description    : The function is called to search ho_ongoing_ue_ctx_node 
 *****************************************************************************/
ho_ongoing_ue_ctx_node_t * search_ho_list_node(
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        s1ap_ue_associated_sig_msg_req_t *p_ue_associated_sig_req)
{
    ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
        list_first_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list);

    if (p_ho_ongoing_ue_ctx_node != PNULL)
    {
        if ( ( p_ue_associated_sig_req->bitmask &
                    UE_ASSOC_SIG_MSG_REQ_MME_UE_S1AP_ID_PRESENT) &&
                ( p_ue_associated_sig_req->bitmask &
                  UE_ASSOC_SIG_MSG_REQ_MME_ID_PRESENT))
        {
            while(p_ho_ongoing_ue_ctx_node)
            {
                if ( (p_ue_associated_sig_req->mme_id ==
                            p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mme_id) && 
                        (p_ue_associated_sig_req->mme_ue_s1ap_id == 
                         p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mmeUeS1apId))
                {
                    /* Match found!! */
                    RRC_S1AP_TRACE(RRC_INFO,
                            "[%s]:Match found in HO temporary list\n",__FUNCTION__);

                    return p_ho_ongoing_ue_ctx_node;
                }

                p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
                    list_next_node(&p_ho_ongoing_ue_ctx_node->node);
            }
        }
        else
        {
            RRC_S1AP_TRACE(RRC_INFO,
                    "[%s]:mmeUeS1apId or mme_id not present"
                    ,__FUNCTION__);
            return PNULL;
        }
    }
    else
    {
        RRC_S1AP_TRACE(RRC_INFO,
                "[%s]:ho_ongoing_ue_ctx_node not present",__FUNCTION__);
        return PNULL;
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return PNULL;
}
/*REL 1.2:HANDOVER PROCEDURE END*/

/*****************************************************************************
 * Function Name  : check_for_mme_overload
 * Inputs         : mme_overload_response - pointer to mme_overload_response_et
 *                  rrc_establishment_cause - establishment cause
 * Outputs        : None
 * Returns        : RRC_SUCCESS / mme is overload,need release ue.
 					RRC_FAILURE / mme is normal.
 * Description    : The function check for the MME Overload and send the UE 
 *                  Release to UECC for the matching rrc_establishment_cause
 *****************************************************************************/
rrc_return_et check_for_mme_overload(
        mme_overload_response_et mme_overload_response,
        U32 rrc_establishment_cause,
        s1ap_allocate_mme_req_t * p_alloc_mme_req,
        mme_context_t* p_mme_ctx,
        active_mme_id_t *p_mme_id_arr,
/* Bug 8198 Fix Start */
        U8 match_result)
/* Bug 8198 Fix End */
{
    rrc_return_et return_value = RRC_FAILURE;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_mme_ctx);
    RRC_ASSERT(PNULL != p_mme_id_arr);
    /* SPR Fix 18880 Start */
    U32 invalid_plmn_id = RRC_INVALID_PLMN_ID;
    /* SPR Fix 18880 Stop */
    /* SPR 22753 Fix +- */
    U8 match_gummei_result   = S1AP_ZERO;
    U8 gummei_list_present = p_mme_id_arr->gummei_list_present;
    
    U8 traffic_load_ind_present = p_mme_id_arr->traffic_load_ind_present;
    s1ap_gummei_t*  p_gummei_list = p_mme_id_arr->gummei_list;


    switch (mme_overload_response)
    {
        case (REJ_ALL_RRC_CONN_MO_DT):
            {
                if (rrc_establishment_cause == 4 ||  /*RRC_ESTAB_CAUSE_MO_DATA*/
                        (rrc_establishment_cause == 5))  /*RRC_ESTAB_CAUSE_DELAY_TOELERANCE*/
                {
                    /* Send Release to UECC */
                    return_value = RRC_SUCCESS;
                }
                break;
            }

        case (REJ_ALL_RRC_CONN_MO_DT_MO_SIG):
            {
                if ((rrc_establishment_cause == 4)||
                        (rrc_establishment_cause == 3) || /*RRC_ESTAB_CAUSE_MO_SIGNALLING*/
                        (rrc_establishment_cause == 5)) 
                {
                    /* Send Release*/
                    return_value = RRC_SUCCESS;
                }
                break;
            }

        case (PERMIT_EMERGENCY_MO_ACCESS):
            {
                if ((rrc_establishment_cause == 0) || /*RRC_ESTAB_CAUSE_EMERGENCY*/
                        (rrc_establishment_cause == 2)) /*RRC_ESTAB_CAUSE_MT_ACCESS*/
                {
                    return_value = RRC_FAILURE;
                }
                else
                {
                    /* Send release*/
                    return_value = RRC_SUCCESS;
                }
                break;
            }
        /* CR 776 Changes */
        case (PERMIT_HIGH_PRIORITY_MT_ACCESS):
            {
                if ((rrc_establishment_cause == 1) || /*RRC_ESTAB_CAUSE_HIGHPRIORITY_ACCESS*/
                        (rrc_establishment_cause == 2)) /*RRC_ESTAB_CAUSE_MT_ACCESS*/
                {
                    return_value = RRC_FAILURE;
                }
                else
                {
                    /* Send release*/
                    return_value = RRC_SUCCESS;
                }
                break;
            }
        /* CR 776 Changes */
        case (REJ_ALL_RRC_DELAY_TOLERANCE):
            {
                if(rrc_establishment_cause == 5)
                {
                    return_value = RRC_SUCCESS;
                }
                break;

            }
        case (OVERLOAD_START_INVALID_VALUE):
        default:
            {
                return_value = RRC_FAILURE;
            }
    }
/* Bug 8198 Fix Start */
    if(RRC_SUCCESS == return_value && 
            RRC_TRUE == gummei_list_present && MATCHING_MME == match_result)
    {
        /* SPR Fix 18880 Start */
        /* will only match gummie if this is not a case of RRC Connection Request
         * otherwise for Initial Ue case, it will be checked */
        if(memcmp_wrapper(&invalid_plmn_id,
              p_alloc_mme_req->plmn_identity.data,
              p_alloc_mme_req->plmn_identity.numocts))
        {
            match_gummei_result = match_overload_start_gummei_info(p_gummei_list,
                                        p_mme_id_arr->num_gummei,p_alloc_mme_req,p_mme_ctx);
            if(MATCHING_GUMMEI == match_gummei_result)
            {
                RRC_S1AP_TRACE(RRC_INFO,"Complete matching GUMMEI found");
                return_value = RRC_SUCCESS;
            }
            else
            {
                RRC_S1AP_TRACE(RRC_INFO,"Complete matching GUMMEI not found");
                return_value = RRC_FAILURE;
            }
        }
        /* SPR Fix 18880 Stop */
    }
    /* SPR 22424 Fix Start */
    if ((RRC_SUCCESS == return_value) && (p_mme_id_arr->num_gummei != p_mme_ctx->num_served_gummeis))
    {
        return_value = RRC_FAILURE;
    }
    /* SPR 22424 Fix End */   
/* Bug 8198 Fix End */

    if(RRC_SUCCESS == return_value && 
            RRC_TRUE == traffic_load_ind_present)
    {
        /* SPR 22753 Fix Start */
        (p_mme_id_arr->total_no_of_calls)++;
        if(RRC_ONE == p_mme_id_arr->total_no_of_calls)
        { 
            (p_mme_id_arr->total_rejected_calls)++;
            return_value = RRC_SUCCESS;
        }
        else
        {
            if(RRC_ONE > (float)(((p_mme_id_arr->total_no_of_calls)*(p_mme_id_arr->traffic_load_ind))/RRC_HUNDRED)-(p_mme_id_arr->total_rejected_calls))
        {
            return_value = RRC_FAILURE;
        }
            else
            {
                (p_mme_id_arr->total_rejected_calls)++;
            }
        }
        RRC_S1AP_TRACE(RRC_INFO,"traffic_load_ind %d total_no_of_calls %d total_rejected_calls %d rrc_establishment_cause %d Call %s", 
                p_mme_id_arr->traffic_load_ind, p_mme_id_arr->total_no_of_calls, p_mme_id_arr->total_rejected_calls, 
                rrc_establishment_cause, (RRC_SUCCESS == return_value) ? "Rejected" : "Accepted");
        if(RRC_HUNDRED == p_mme_id_arr->total_no_of_calls)
        { 
            p_mme_id_arr->total_no_of_calls = RRC_ZERO;
            p_mme_id_arr->total_rejected_calls = RRC_ZERO;
        }
        /* SPR 22753 Fix End */
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return return_value;
}

/*****************************************************************************
 * * Function Name  : s1ap_process_location_report 
 * * Inputs         : p_asn_msg - Pointer to asn encoded message.
 *                    asn_msg_len - lenght of encoded message
 *                    mme_id - id of mme to which this report is to be sent 
 *                    p_s1ap_gb_ctx - pointer to S1AP global context
 *                    p_s1ap_context - pointer to ue_s1ap_context_t
 * * Outputs        : location report
 * * Returns        : nothing
 * * Description    : The function  sends location report message to mme having 
 *                   id of  mme_id 
 * *****************************************************************************/
void  s1ap_process_location_report_message_to_mme(
        void*  p_asn_msg,
        U32    asn_msg_len,
        U8     mme_id,
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        ue_s1ap_context_t    *p_s1ap_context
        )
{
    mme_context_t * p_mme_context = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_asn_msg);

    do 
    {
        RRC_S1AP_TRACE(RRC_INFO, "[%s]: Forwarding message to MME\n",
                __FUNCTION__);

        p_mme_context = p_s1ap_gb_ctx->mme_contexts[mme_id];

        if(PNULL == p_mme_context)
        {       
            RRC_S1AP_TRACE(RRC_WARNING,
                    "MME Context for mme_id %d is Null, Unable to"
                    " send the location report",mme_id);

            break;  
        }       
        if (MME_ACTIVE != p_mme_context->current_mme_state)
        {       
            RRC_S1AP_TRACE(RRC_WARNING,
                    "MME %d is not in active state, "
                    "location report failed", mme_id);
            break;  
        } 
        if (RRC_FAILURE == send_msg_to_mme(p_mme_context,
                    p_asn_msg,
                    asn_msg_len,
                    &p_s1ap_context->stream_id))
        {       
            RRC_S1AP_TRACE(RRC_WARNING,
                    "location report Message"
                    " failed to MME ID %d", mme_id);
            break;  
        }
    } while(0);
    RRC_S1AP_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
 * Function Name  : s1ap_fill_gummei_info 
 * Inputs         : p_mme_context - Pointer to mme context of a MME
 *                  p_gummei - pointer to s1ap_gummei_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills gummei information.
 ****************************************************************************/
void s1ap_fill_gummei_info(
        mme_context_t*    p_mme_context, 
        s1ap_gummei_t*    p_gummei)
{
    list_t * p_mme_grp_id_list = PNULL;
    list_node_t * p_node = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_mme_context);
    RRC_ASSERT(PNULL != p_mme_context->p_served_gummeis);
    RRC_ASSERT(PNULL != p_gummei);

    /* SPR 16038 Fix Start */
	do
	{
	     if ( PNULL == p_mme_context->p_served_gummeis )
        {
            RRC_S1AP_TRACE(RRC_WARNING,
                    "[s1ap_fill_gummei_info] : Served Gummeis list is NULL");
            break;
        }

    /* filling plmn id */
    p_gummei->plmn_identity.numocts 
        = p_mme_context->p_served_gummeis->servedPLMNs.elem[0].numocts;
    l3_memcpy_wrapper((void *)(&(p_gummei->plmn_identity.data)),
            (const void*)p_mme_context->p_served_gummeis->servedPLMNs.
            elem[0].data,
            p_mme_context->p_served_gummeis->servedPLMNs.
                elem[0].numocts);

    /* filling mme grp id */
    p_mme_grp_id_list = &(p_mme_context->p_served_gummeis[EUTRAN_POOL_INDEX].
            mme_group_id_list);
    if (PNULL != p_mme_grp_id_list)
    {
        p_node = list_first_node(p_mme_grp_id_list);
        if (PNULL != p_node)
        {
            p_gummei->grp_id.numocts = (((mme_group_id_list_node_t*)p_node)->
                    mme_group_id).numocts;
            l3_memcpy_wrapper((void *)(&(p_gummei->grp_id.data)),
                    (const void*)((((mme_group_id_list_node_t*)p_node)->
                                   mme_group_id).data),
                    (((mme_group_id_list_node_t*)p_node)->mme_group_id).
                        numocts);
        }
        else
        {
            RRC_S1AP_TRACE(RRC_INFO,
                    "MME Group Id list is NULL");
        }
    }
    /* filling mme code */
    p_gummei->mme_code.numocts 
        = p_mme_context->p_served_gummeis->served_mmecs.elem[0].numocts;
    l3_memcpy_wrapper((void *)(&(p_gummei->mme_code.data)),
            (const void*)p_mme_context->p_served_gummeis->served_mmecs.
            elem[0].data,
            p_mme_context->p_served_gummeis->served_mmecs.
                elem[0].numocts);

	}while (0);
	/* SPR 16038 Fix Stop */
	
    RRC_S1AP_UT_TRACE_EXIT();
}
    
/*****************************************************************************
 * Function Name  : s1ap_build_and_send_ue_associated_mme_update_status_ind 
 * Inputs         : enb_ue_s1ap_id - id of ue over S1 inteface in eNB
 *                  p_mme_context - Pointer to mme context of a MME
 * Outputs        : None
 * Returns        : none
 * Description    : This function  build and send ue associated mme update
 *                  message indication to uecc module.
 ****************************************************************************/
rrc_return_et s1ap_build_and_send_ue_associated_mme_update_status_ind(
        U32                 enb_ue_s1ap_id, 
/*BUG 604 changes start*/
        mme_context_t       *p_mme_context,
        U16                  ue_index)
/*BUG 604 changes stop*/
{
    s1ap_ue_associated_mme_update_status_ind_t *msg_p = PNULL;
    void                        *p_intrl_msg          = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_mme_context);

    p_intrl_msg = s1ap_alloc_intrl_msg(
            RRC_UECC_MODULE_ID,
            S1AP_UE_ASSOCIATED_MME_UPDATE_STATUS_IND,
            (U16)(sizeof(s1ap_ue_associated_mme_update_status_ind_t)));

    if (PNULL == p_intrl_msg)
    {
        /*out of memory*/
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    msg_p = (s1ap_ue_associated_mme_update_status_ind_t *)((U8 *)p_intrl_msg +
            RRC_API_HEADER_SIZE);

    /* filling enb ue s1ap id */
    msg_p->enb_ue_s1ap_id = enb_ue_s1ap_id;
    /*BUG 604 changes start*/
    /*Fill ue_index*/
    msg_p->ue_index = ue_index;
    /*BUG 604 changes stop*/

    s1ap_fill_gummei_info(p_mme_context, &(msg_p->gummei)); 
    
    RRC_S1AP_TRACE(RRC_INFO, "[%s]: Forwarding message to UECC\n",
            __FUNCTION__);

    /* Send Indication to UECC */
    /*BUG 604 changes start*/
    rrc_send_message(
                p_intrl_msg,
                RRC_GET_UECC_MODULE_ID(ue_index));
    /*BUG 604 changes stop*/

    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : search_ho_list_node_to_be_released
 * Inputs         : p_s1ap_gb_ctx - Pointer to S1AP global context.
 *                  p_ue_release_ind - Pointer to s1ap_ue_release_ind_t.
 * Outputs        : None
 * Returns        : ho_ongoing_ue_ctx_node
 * Description    : The function is called to search ho_ongoing_ue_ctx_node
 *****************************************************************************/
ho_ongoing_ue_ctx_node_t * search_ho_list_node_to_be_released(
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        s1ap_ue_release_ind_t *p_ue_release_ind)
{
    ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
        list_first_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list);

    if (p_ho_ongoing_ue_ctx_node != PNULL)
    {
        while(p_ho_ongoing_ue_ctx_node)
        {
            if ((p_ue_release_ind->mme_id ==
                        p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mme_id) &&
                    (p_ue_release_ind->mme_ue_s1ap_id ==
                     p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mmeUeS1apId))
            {
                /* Match found!! */
                RRC_S1AP_TRACE(RRC_INFO,
                        "[%s]:Match found in HO temporary list\n",__FUNCTION__);

                return p_ho_ongoing_ue_ctx_node;
            }

            p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
                list_next_node(&p_ho_ongoing_ue_ctx_node->node);
        }
    }
    else
    {
        RRC_S1AP_TRACE(RRC_INFO,
                "[%s]:ho_ongoing_ue_ctx_node not present",__FUNCTION__);
        return PNULL;
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return PNULL;
}

/*****************************************************************************
 * Function Name  : build_and_send_s1ap_ue_rel_req_int
 * Inputs         : enb_ue_s1ap_id - id of UE over S1 in eNB.
 * Outputs        : None
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : The function sends msg to UECC to release UE
 *****************************************************************************/
rrc_return_et build_and_send_s1ap_ue_rel_req_int(
/*BUG 604 changes start*/
        U32 enb_ue_s1ap_id,
        U16 ue_index)
/*BUG 604 changes stop*/
{
    U8 *p_api = PNULL;
    U8 *p_msg = PNULL;
    U16 msg_len = RRC_NULL;

    s1ap_ue_rel_req_int_t s1ap_ue_rel_req_int;

    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper((void *)&s1ap_ue_rel_req_int, 0, sizeof(s1ap_ue_rel_req_int_t));

    /* Fill the API to be sent to UECC */
    s1ap_ue_rel_req_int.enb_ue_s1ap_id = enb_ue_s1ap_id;
    /*BUG 604 changes start*/
    /*Fill ue_index*/
    s1ap_ue_rel_req_int.ue_index = ue_index;
    /*BUG 604 changes stop*/

        msg_len = (U16)sizeof(s1ap_ue_rel_req_int_t);

        p_api = (U8*)s1ap_alloc_intrl_msg(RRC_UECC_MODULE_ID,
                S1AP_UE_REL_REQ_INT,
                msg_len);
        if(PNULL == p_api)
        {
            RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
            return RRC_FAILURE;
        }

        p_msg = p_api + CV_HDRSIZE;
        if(PNULL == p_msg)
        {
            RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
            return RRC_FAILURE;
        }

        l3_memcpy_wrapper((void*)p_msg,(const void*)&s1ap_ue_rel_req_int,
                sizeof(s1ap_ue_rel_req_int_t));

         /*BUG 604 changes start*/
        /* Send message to UECC */
        rrc_send_message(
                p_api,
                RRC_GET_UECC_MODULE_ID(ue_index));
         /*BUG 604 changes stop*/

    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/****************************************************************************
 * Function Name  : delete_ue_ctx_from_ho_temp_list
 * Inputs         : p_s1ap_gb_ctx -  Pointer to S1AP global ctx
 *                : mme_ue_s1ap_id - id of UE over S1 interface in MME 
 * Outputs        : None
 * Returns        : None 
 * Description    : This function delete UE context from ho_temp_list.
 ****************************************************************************/
void  delete_ue_ctx_from_ho_temp_list(
        s1ap_gb_context_t * p_s1ap_gb_ctx,  
        mme_ue_s1ap_id_list_t   * mme_ue_s1ap_id_list)
{
    ho_ongoing_ue_ctx_node_t *p_ho_ongoing_ue_ctx_node = PNULL;
    ho_ongoing_ue_ctx_node_t *p_ho_ongoing_ue_ctx_node_next = PNULL;
    U8        ue_ctx_delete_flag = RRC_FALSE;
    /* +- SPR 18268 */
    U16        mmeUeId_index = RRC_NULL;
    /* +- SPR 18268 */
    /*SPR_21818_START*/
    ue_s1ap_context_t *p_ue_s1ap_context = PNULL ;   /* UE context in s1ap */
    /*SPR_21818_END*/

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != mme_ue_s1ap_id_list);
    
    for (mmeUeId_index=0; mmeUeId_index<mme_ue_s1ap_id_list->count; mmeUeId_index++)
    {
        SET_UE_INDEX(mmeUeId_index);

        p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
            list_first_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list);
        {
            while(p_ho_ongoing_ue_ctx_node)
            {

                /* Delete ho_ongoing_ue_ctx from the ho_ongoing_ue_ctx_list  */
                if (p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.mmeUeS1apId ==
                        mme_ue_s1ap_id_list->mme_ue_s1ap_id[mmeUeId_index])
                {
                    list_delete_node(&p_s1ap_gb_ctx->ho_ongoing_ue_ctx_list, (void
                                *)p_ho_ongoing_ue_ctx_node);

                    RRC_S1AP_TRACE(RRC_INFO,"%s HO ONGOING UE Context DELETED for mme_ue_s1ap_id: %u"
                            "\n",__FUNCTION__,mme_ue_s1ap_id_list->mme_ue_s1ap_id[mmeUeId_index]);

                    /*SPR_21818_START*/
                    get_ue_ctx_from_ue_id(p_s1ap_gb_ctx, &p_ue_s1ap_context, 
                            p_ho_ongoing_ue_ctx_node->
                            ho_ongoing_ue_ctx.enb_ue_s1ap_id);
                    if(PNULL != p_ue_s1ap_context)
                    {
                        /* SPR 22789 Fix Start */
                        RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx,p_ue_s1ap_context,
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
                    ue_ctx_delete_flag = RRC_TRUE;
                    break;
                }

                p_ho_ongoing_ue_ctx_node_next = (ho_ongoing_ue_ctx_node_t *)
                    list_next_node(&p_ho_ongoing_ue_ctx_node->node);

                p_ho_ongoing_ue_ctx_node = p_ho_ongoing_ue_ctx_node_next;
            }
        }
        if (!ue_ctx_delete_flag)
        {
            RRC_S1AP_TRACE(RRC_WARNING,"%s HO ONGOING UE Context not found for mme_ue_s1ap_id: %u",
                    __FUNCTION__,mme_ue_s1ap_id_list->mme_ue_s1ap_id[mmeUeId_index]);
        }

        ue_ctx_delete_flag = RRC_FALSE;
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : process_uecc_ho_ongoing_ue_rel_ind
 * Inputs         : p_s1ap_gb_ctx -  Pointer to S1AP global ctx
 *                : p_api - Pointer to api buffer received from UECC.
 * Outputs        : None
 * Returns        : void
 * Description    : This function processes UE release ind to UECC while 
 *                  handover is going on  
 ****************************************************************************/
void  process_uecc_ho_ongoing_ue_rel_ind(
        s1ap_gb_context_t * p_s1ap_gb_ctx,  
        U8 * p_api) 
{
    s1ap_ho_ongoing_ue_release_ind_t  *p_ho_ongoing_ue_release_ind = 
        (s1ap_ho_ongoing_ue_release_ind_t *)p_api;

    RRC_S1AP_UT_TRACE_ENTER();

    delete_ue_ctx_from_ho_temp_list(p_s1ap_gb_ctx, &(p_ho_ongoing_ue_release_ind->mme_ue_s1ap_id_list));

    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : s1ap_process_ueccmd_messages
 * Inputs         : p_api - Pointer to buffer containing input API
 *                  p_s1ap_gb_ctx -Pointer to the UECC global context data
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called when a message is received from
 *                  UECCMD  
 ****************************************************************************/
void  s1ap_process_ueccmd_messages(
        void    *p_api,
        s1ap_gb_context_t *p_s1ap_gb_ctx )
{
	s1ap_ueccmd_uecc_info_t   *p_uecc_info = PNULL;
	U16                        api_id      = RRC_NULL; 
	/*BUG 604 changes start*/ 
	/* coverity_fix_61166_start */ 
	rrc_return_t                result  = RRC_SUCCESS;
	/* coverity_fix_61166_stop */ 
	/*BUG 604 changes start*/ 

	RRC_S1AP_UT_TRACE_ENTER();

	RRC_ASSERT(p_api != PNULL);
	RRC_ASSERT(p_s1ap_gb_ctx != PNULL);

	if ( S1AP_ACTIVE != p_s1ap_gb_ctx->s1ap_current_state )
	{
		RRC_S1AP_TRACE(RRC_FATAL, "[%s]:S1AP Current state is NOT ACTIVE"
				"Hence Discarding the message\n",
				__FUNCTION__);
		return ;

	}
	api_id = rrc_get_api_id((U8*)p_api);

	switch (api_id)
	{
		case S1AP_UECCMD_UECC_INFO :
			p_uecc_info = (s1ap_ueccmd_uecc_info_t *)
				((U8*)p_api + CV_HDRSIZE);

			p_s1ap_gb_ctx->no_of_uecc_instances =
				p_uecc_info->num_uecc_instances;

			p_s1ap_gb_ctx->total_ue_supported =
				p_uecc_info->total_ue_supported;
			/*BUG 604 changes start*/
            /*SPR_21818_START*/
            /* Code moved inside 'else' */
            /*SPR_21818_END*/

			if (p_s1ap_gb_ctx->p_p_ue_s1ap_contexts == PNULL)
			{
				if (PNULL == (p_s1ap_gb_ctx->p_p_ue_s1ap_contexts = (ue_s1ap_context_t**)
							rrc_mem_get(sizeof(ue_s1ap_context_t*)*p_uecc_info->
								total_ue_supported)))
				{
					/* Not enough memory */
					RRC_S1AP_TRACE(RRC_WARNING, "NOT Enough Memory");
					break;
				}
				else
				{
					memset_wrapper(p_s1ap_gb_ctx->p_p_ue_s1ap_contexts, RRC_NULL, 
							sizeof(ue_s1ap_context_t*)*p_uecc_info->total_ue_supported);
                    		/*SPR_21818_START*/
                    		/* Initialize S1AP_UEID_CTX */
                    		result = s1ap_ueid_ctx_init(p_s1ap_gb_ctx); 
                    		if (RRC_FAILURE == result )
                    		{
                        	    RRC_S1AP_TRACE(RRC_WARNING,"unable to initialize s1ap ueid context ");
                        		return ;
                    		}
                    		/*SPR_21818_END*/
				}

			}
			/*BUG 604 changes stop*/
			break;

		default:
			{
				RRC_S1AP_TRACE(RRC_ERROR, "[%s]Processing UECCMD Messages :"
						"Unknown API ::%d\n",__FUNCTION__,api_id);    
				break;
			}

	}
	RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : build_and_send_allocate_mme_resp
 * Inputs         : enb_ue_s1ap_id - id of UE over S1 interface in eNB
 *                  rel_of_mme - release of MME
 *                  mme_id - MME id
 * Outputs        : None
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : The function sends allocate_mme_resp to UECC
 *****************************************************************************/
rrc_return_et build_and_send_allocate_mme_resp(
        U32 enb_ue_s1ap_id,
        U8  rel_of_mme,
/*BUG 604 changes start*/
        U8  mme_id,
        U16 ue_index,
        U8 matching_gummei_to_be_sent)
/*BUG 604 changes stop*/
{
    
    U8 *p_api = PNULL;
    U8 *p_msg = PNULL;
    U16 msg_len = RRC_NULL;

    s1ap_allocate_mme_resp_t s1ap_alloc_mme_resp;

    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper((void *)&s1ap_alloc_mme_resp, 0, sizeof(s1ap_allocate_mme_resp_t));

    /* Fill the API to be sent to UECC */
    s1ap_alloc_mme_resp.enb_ue_s1ap_id = enb_ue_s1ap_id;
    s1ap_alloc_mme_resp.rel_of_mme = rel_of_mme;
    s1ap_alloc_mme_resp.mme_id = mme_id;
/*BUG 604 changes start*/
    s1ap_alloc_mme_resp.ue_index = ue_index;
/*BUG 604 changes stop*/
    /* mme grp id feature changes start */
    s1ap_alloc_mme_resp.matching_gummei_to_be_sent = matching_gummei_to_be_sent;
    /* mme grp id feature changes stop */

    msg_len = (U16)sizeof(s1ap_allocate_mme_resp_t);

    p_api = (U8*)s1ap_alloc_intrl_msg(RRC_UECC_MODULE_ID,
            S1AP_ALLOCATE_MME_RESP,
            msg_len);
    if(PNULL == p_api)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        return RRC_FAILURE;
    }

    p_msg = p_api + CV_HDRSIZE;
    if(PNULL == p_msg)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        return RRC_FAILURE;
    }

    l3_memcpy_wrapper((void*)p_msg,(const void*)&s1ap_alloc_mme_resp,
            sizeof(s1ap_allocate_mme_resp_t));

    /* Send message to UECC */
/*BUG 604 changes start*/
    rrc_send_message(
            p_api,
            RRC_GET_UECC_MODULE_ID(ue_index));
/*BUG 604 changes stop*/

    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : s1ap_fill_protocol_event_params 
 * Inputs         : p_event_header - pointer to EVENT_EXTERNAL_HEADER
 *                  encoded_msg_len - length of encoded msg
 *                  event_id - id of  event
 *                  message_direction - direction of message
 * Outputs        : None
 * Returns        : None
 * Description    : This function Fills the Local Events' parameters .
 ********************************************************************************/
void s1ap_fill_protocol_event_params(
        EVENT_EXTERNAL_HEADER *p_event_header,
        U32 encoded_msg_len, 
        U16 event_id,
        U32 message_direction)
{
    RRC_S1AP_UT_TRACE_ENTER();

    if (PNULL != p_event_header)
    {
        memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len));

        p_event_header->length = (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len);
        p_event_header->event_id = event_id;

        p_event_header->EVENT_PARAM_MESSAGE_DIRECTION = message_direction;
        p_event_header->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;
    }
    
    RRC_S1AP_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : allocate_mme_by_weight_factor
 * Inputs         : p_mme_id    - Pointer to the mme Index to be updated
 *                  p_s1ap_gb_ctx - Pointer to S1AP global ctx
 *                  p_ue_associated_sig_req - pointer to s1ap_allocate_mme_req_t
 * Outputs        : None
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : This function is called to  allocate  mme by weight factor
 *                  based algorithm applied on the active mme's only if 
 *                  Registered MME is not found in S1AP global context.
 ****************************************************************************/
rrc_return_et  allocate_mme_by_weight_factor(
        U32* p_mme_id,
        s1ap_gb_context_t* p_s1ap_gb_ctx,
        s1ap_allocate_mme_req_t * p_alloc_mme_req,
        /* Bug 8198 Fix Start */
        U8 procedure_code,
        /* Bug 8198 Fix End */
        /*BUG 604 changes start*/
        U32   enb_ue_s1ap_id) 
        /*BUG 604 changes stop*/
{
    active_mme_info_t *p_mme_info = PNULL;
    /* SPR 22789 Fix Start */ 
    list_t active_mme_mmegi_pool_area_list;
    /* SPR 22789 Fix End */ 
    U32 mme_id = MAX_NUM_MME; 
    U8 tempCount = S1AP_ZERO;
    U8 match_result = NO_MATCHING_MME;
    mme_overload_response_et mme_overload_response = 
        OVERLOAD_START_INVALID_VALUE;
    U8 mme_overload_flag = RRC_FALSE; 
    U32 rel_mme_capacity_total = S1AP_ZERO;
    /* SPR 22789 Fix Start */ 
    /* Code Deleted */
    list_t active_mme_plmn_pool_area_list;
    /* SPR 22789 Fix End */   
    U8 matching_mme_found = RRC_FALSE; 
    /* SPR 22789 Fix Start */ 
    /* Code Deleted */
    /* SPR 22789 Fix End */ 
    rrc_return_et result = RRC_FAILURE;
    active_mme_pool_area_node_t *p_node = PNULL;
    /* SPR 22789 Fix Start */
    U32 mme_index = S1AP_ZERO;
    U32 total_conn_ue = S1AP_ZERO;
    /* SPR 22789 Fix End */
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_mme_id);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx );
    RRC_ASSERT(PNULL != p_alloc_mme_req );

    /* SPR 22789 Fix Start */ 
    p_mme_info = &(p_s1ap_gb_ctx->active_mme_info);

    /* Initialising the list */
    list_init(&active_mme_mmegi_pool_area_list);
    list_init(&active_mme_plmn_pool_area_list);
    /* SPR 22789 Fix End */   
    
    tempCount = p_mme_info->active_mme_count;
    /* If Registered MME info is present, try allocating the Registred MME*/
    while (tempCount)
    {
        /* SPR 22789 Fix Start */
        mme_id = p_mme_info->mme_id_arr[mme_index].mme_id;
        /* SPR 22789 Fix End */
        /* SPR 18608 19July Start */
        mme_overload_flag = RRC_FALSE;
        /* SPR 18608 19July Stop */

        if (MME_ACTIVE ==
                p_s1ap_gb_ctx->mme_contexts[mme_id]->current_mme_state )
        {
            /* Bug 8198 Fix Start */
            if(ASN1V_s1ap_id_initialUEMessage == procedure_code)
            {
                /* SPR 22789 Fix Start */
                mme_overload_response = (mme_overload_response_et)p_mme_info->mme_id_arr[mme_index].overload_response;
                /* SPR 22789 Fix End */
            }
            /* Bug 8198 Fix End */

            match_result = match_registered_mme_info(p_s1ap_gb_ctx,
                    p_s1ap_gb_ctx->mme_contexts[mme_id],
                    p_alloc_mme_req);

            /* Bug 8198 Fix Start */
            if ( MATCHING_MME == match_result)
            {
                if(ASN1V_s1ap_id_initialUEMessage == procedure_code)
                {
                    /* SPR 22789 Fix Start */
                    if ( RRC_SUCCESS == check_for_mme_overload(
                                mme_overload_response,
                                p_alloc_mme_req->rrc_establishment_cause,
                                p_alloc_mme_req,
                                p_s1ap_gb_ctx->mme_contexts[mme_id],
                                &p_mme_info->mme_id_arr[mme_index],
                                match_result))
                    /* SPR 22789 Fix End */
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "Assigned MME ID [%d]"
                                " OVERLOADED STATE [%d]",
                                mme_id, 
                                mme_overload_response);
                        mme_overload_flag = RRC_TRUE;        
                    }
                    else
                    {
                        *p_mme_id  =  mme_id;
                        /* mme grp id feature changes start */
                        p_s1ap_gb_ctx->matching_gummei_to_be_sent = RRC_TRUE;
                        /* mme grp id feature changes stop */
                        /* SPR 22789 Fix Start */
                        RRC_S1AP_TRACE(RRC_INFO,"Registered MME found ID %d", mme_id);
                        /* SPR 22789 Fix End */
                        matching_mme_found = RRC_TRUE;
                        result = RRC_SUCCESS;
                        break;
                    }
                }
                else
                {
                    *p_mme_id  =  mme_id;
                    /* SPR 22789 Fix Start */
                    RRC_S1AP_TRACE(RRC_INFO,"Registered MME found ID %d", mme_id);
                    /* SPR 22789 Fix End */
                    matching_mme_found = RRC_TRUE;
                    result = RRC_SUCCESS;
                    break;
                }
            }
            else if (PARTIAL_MATCHING_MMEGI_MME == match_result)
            {
                if(ASN1V_s1ap_id_initialUEMessage == procedure_code)
                {
                    /* SPR 22789 Fix Start */
                    if ( RRC_SUCCESS == check_for_mme_overload(
                                mme_overload_response,
                                p_alloc_mme_req->rrc_establishment_cause,
                                p_alloc_mme_req,
                                p_s1ap_gb_ctx->mme_contexts[mme_id],
                                &p_mme_info->mme_id_arr[mme_index],
                                match_result))
                    /* SPR 22789 Fix End */
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "Assigned MME ID [%d]"
                                " OVERLOADED STATE [%d]",
                                mme_id, mme_overload_response);
                        mme_overload_flag = RRC_TRUE;
                    }
                    else
                    {
                        /* SPR 22789 Fix Start */
                        active_mme_pool_list_insert(
                                p_s1ap_gb_ctx->mme_contexts[mme_id],
                                &active_mme_mmegi_pool_area_list,
                                &rel_mme_capacity_total,
                                &total_conn_ue);
                        /* SPR 22789 Fix End */
                    }
                }
                else
                {
                    /* SPR 22789 Fix Start */
                    active_mme_pool_list_insert(
                            p_s1ap_gb_ctx->mme_contexts[mme_id],
                            &active_mme_mmegi_pool_area_list,
                            &rel_mme_capacity_total,
                            &total_conn_ue);
                    /* SPR 22789 Fix End */
                }
            }
            /* SPR 22789 Fix Start */
            else if (PARTIAL_MATCHING_MME == match_result)
            {
                if(ASN1V_s1ap_id_initialUEMessage == procedure_code)
                {
                    if ( RRC_SUCCESS == check_for_mme_overload(
                                mme_overload_response,
                                p_alloc_mme_req->rrc_establishment_cause,
                                p_alloc_mme_req,
                                p_s1ap_gb_ctx->mme_contexts[mme_id],
                                &p_mme_info->mme_id_arr[mme_index],
                                match_result))
                    {
                        RRC_S1AP_TRACE(RRC_INFO, "Assigned MME ID [%d]"
                                " OVERLOADED STATE [%d]",
                                mme_id, mme_overload_response);
                        mme_overload_flag = RRC_TRUE;        
                    }
                    else
                    {
                        /* SPR 22789 Fix Start */
                        active_mme_pool_list_insert(
                                p_s1ap_gb_ctx->mme_contexts[mme_id],
                                &active_mme_plmn_pool_area_list,
                                &rel_mme_capacity_total,
                                &total_conn_ue);
                    }
                }
                else
                {
                    active_mme_pool_list_insert(
                            p_s1ap_gb_ctx->mme_contexts[mme_id],
                            &active_mme_plmn_pool_area_list,
                            &rel_mme_capacity_total,
                            &total_conn_ue);
                }
            }
            /* SPR 22789 Fix End */
            /* Bug 8198 Fix End */
        }

        tempCount--;
        /* SPR 22789 Fix Start */
        mme_index++;
        /* SPR 22789 Fix End */
    }
    if(RRC_FALSE == matching_mme_found)
    {
        /* If multiple MME present in the MME POOL ,allocating MME on the basis of weight factor */
        /* SPR 22789 Fix Start */
        if(list_count(&active_mme_mmegi_pool_area_list))
        {
            result = allocate_mme_by_mme_selection_value(
                        p_mme_id,
                        &active_mme_mmegi_pool_area_list,
                        rel_mme_capacity_total,
                        total_conn_ue,
                        p_s1ap_gb_ctx);
        }
        else if(list_count(&active_mme_plmn_pool_area_list)) 
        {
            result = allocate_mme_by_mme_selection_value(
                        p_mme_id,
                        &active_mme_plmn_pool_area_list,
                        rel_mme_capacity_total,
                        total_conn_ue,
                        p_s1ap_gb_ctx);
        }
        /* SPR 22789 Fix End */
        else if(RRC_TRUE == mme_overload_flag)
        {
            RRC_S1AP_TRACE(RRC_INFO, "Assigned MME OVERLOADED, Releasing UE"
                " Establishment [%d]", p_alloc_mme_req->rrc_establishment_cause);
            /* Sending overload ind to UECC */
            /*BUG 604 changes start*/
            build_and_send_mme_overload_ind(enb_ue_s1ap_id,
                    p_s1ap_gb_ctx,p_alloc_mme_req->ue_index);
            /*BUG 604 changes stop*/
            p_s1ap_gb_ctx->is_sel_mmes_overloaded = RRC_TRUE;
        }
    }
    /* SPR 22789 Fix Start */
    if(list_count(&active_mme_mmegi_pool_area_list))
    {
        while( PNULL != (p_node = 
                    (active_mme_pool_area_node_t*)
                    list_pop_head(&active_mme_mmegi_pool_area_list)) ) 
        {
            rrc_mem_free(p_node);
            p_node = PNULL;
        }
    }
    /* SPR 22789 Fix End */
    if(list_count(&active_mme_plmn_pool_area_list))
    {
        while( PNULL != (p_node = 
                    (active_mme_pool_area_node_t*)
                    list_pop_head(&active_mme_plmn_pool_area_list)) ) 
        {
            rrc_mem_free(p_node);
            p_node = PNULL;
        }
    }
    /* SPR 22789 Fix Start */
    /* Code Deleted */
    /* SPR 22789 Fix End */

    RRC_S1AP_UT_TRACE_EXIT();
    return result;
}

/*SPR 22789 Fix Start */
/****************************************************************************
 * Function Name  : active_mme_pool_list_insert
 * Inputs         : p_s1ap_gb_ctx : pointer to S1AP global context
 *                  p_active_mme_pool_area_list - pointer to list_t
 *                  p_total_rel_capacity - pointer Total Relative Capacity of MME
 *                  p_total_conn_ue - pointer to Total Connected UE
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to insert mme_id in 
 *                  active_mme_pool_area.
 ****************************************************************************/
void active_mme_pool_list_insert(
        mme_context_t * p_mme_ctx,
        list_t *        p_active_mme_pool_area_list,
        U32 *           p_total_rel_capacity,
        U32 *           p_total_conn_ue)
/*SPR 22789 Fix End */
{
    active_mme_pool_area_node_t *p_active_mme_pool_area_node = PNULL;
    U32  mme_count = S1AP_ZERO;
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_mme_ctx);
    RRC_ASSERT(PNULL != p_active_mme_pool_area_list);

    mme_count = list_count(p_active_mme_pool_area_list);
    if (mme_count < MAX_NUM_MME)
    {
        /* Create active_mme_pool_srea_info  */
        p_active_mme_pool_area_node  = (active_mme_pool_area_node_t *)
            rrc_mem_get(sizeof(active_mme_pool_area_node_t));
        if(PNULL == p_active_mme_pool_area_node)
        {
            RRC_S1AP_TRACE(RRC_FATAL, "rrc_mem_get failed!!");
            RRC_S1AP_UT_TRACE_EXIT();
            return;
        }

        memset_wrapper(p_active_mme_pool_area_node,0,sizeof(active_mme_pool_area_node_t));

        /* Set mme_id */
        p_active_mme_pool_area_node->mme_id = p_mme_ctx->mme_id;

        /* Set relative mme capacity */
        p_active_mme_pool_area_node->relative_mme_capacity = 
            p_mme_ctx->relative_mme_capacity;

        *p_total_rel_capacity += (p_active_mme_pool_area_node->relative_mme_capacity);
        /* SPR 22789 Fix Start */
        *p_total_conn_ue += p_mme_ctx->ue_connected;

        list_push_tail(p_active_mme_pool_area_list,
                &p_active_mme_pool_area_node->node);

        RRC_S1AP_TRACE(RRC_INFO,"[%u] UE Conn to MME[%u] Capacity[%u] updated",
                p_mme_ctx->ue_connected,
                p_mme_ctx->mme_id,
                p_mme_ctx->relative_mme_capacity);
        /* SPR 22789 Fix End */
    }
    else
    {
        /* SPR 22789 Fix Start */
        RRC_S1AP_TRACE(RRC_WARNING,"Active MME Already reached to MAX_NUM_MME %u"
                " Hence cannot insert another MME %u",
                MAX_NUM_MME, p_mme_ctx->mme_id);
        /* SPR 22789 Fix End */
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/* SPR 22789 Fix Start */
/****************************************************************************
 * Function Name  : allocate_mme_by_mme_selection_value
 * Inputs         : p_mme_id  - pointer to MME id
 *                  p_active_mme_pool_area_list - pointer to list_t
 *                  rel_mme_capacity_total - mme capacity
 *                  total_conn_ue - Total Connected UE
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to allocate mme 
 *                  using mme_selection_value
 ****************************************************************************/
rrc_return_et allocate_mme_by_mme_selection_value(
	U32 *p_mme_id,
	list_t *p_active_mme_pool_area_list,
	U32 rel_mme_capacity_total,
	U32 total_conn_ue,
	s1ap_gb_context_t* p_s1ap_gb_ctx)
/* SPR 22789 Fix End */
{
    /* SPR 22789 Fix Start */
   active_mme_pool_area_node_t *p_node = PNULL;
   U32 max_mme_rel_cap = S1AP_ZERO;
   U16 ue_count = RRC_ZERO;
   float factor = RRC_ZERO;
   float factor1 = RRC_ZERO;
   RRC_S1AP_UT_TRACE_ENTER();

   RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
   RRC_ASSERT(PNULL != p_mme_id);
   RRC_ASSERT(PNULL != p_active_mme_pool_area_list);

   if(list_count(p_active_mme_pool_area_list) > 1)
   {
        /* Code Deleted */
        if (RRC_ZERO == total_conn_ue)
        {
            p_node = (active_mme_pool_area_node_t*)list_pop_head(p_active_mme_pool_area_list);
            if(PNULL != p_node)
            {
                max_mme_rel_cap = p_node->relative_mme_capacity;
                *p_mme_id = p_node->mme_id;
                rrc_mem_free(p_node);
            }

            while ((p_node = (active_mme_pool_area_node_t*)list_pop_head(p_active_mme_pool_area_list))) 
            {
                if ((PNULL != p_node) && (p_node->relative_mme_capacity > max_mme_rel_cap))
                {
                    max_mme_rel_cap = p_node->relative_mme_capacity;
                    *p_mme_id = p_node->mme_id;
                    RRC_S1AP_TRACE(RRC_INFO,"mme id assigned:%d",*p_mme_id);
                }
                rrc_mem_free(p_node);
            }
            RRC_S1AP_UT_TRACE_EXIT();
            return RRC_SUCCESS; 
        }
        else
        {
            p_node = (active_mme_pool_area_node_t*)list_pop_head(p_active_mme_pool_area_list);
            if(PNULL != p_node)
            {
                ue_count = p_s1ap_gb_ctx->mme_contexts[p_node->mme_id]->ue_connected;
                factor = ((float)(total_conn_ue * p_node->relative_mme_capacity)/(float)rel_mme_capacity_total) - ue_count;
                *p_mme_id = p_node->mme_id;
                RRC_S1AP_TRACE(RRC_INFO,"mme id assigned:%u", *p_mme_id);
                rrc_mem_free(p_node);
            }
            while ((p_node = (active_mme_pool_area_node_t*)list_pop_head(p_active_mme_pool_area_list)))
            {
                ue_count = p_s1ap_gb_ctx->mme_contexts[p_node->mme_id]->ue_connected;
                factor1 = ((float)(total_conn_ue * p_node->relative_mme_capacity)/(float)rel_mme_capacity_total) - ue_count;
                if (factor < factor1)
                {
                    *p_mme_id = p_node->mme_id;
                    factor = factor1;
                    RRC_S1AP_TRACE(RRC_INFO,"mme id assigned:%u", *p_mme_id);
                }
                rrc_mem_free(p_node);
            }
            RRC_S1AP_UT_TRACE_EXIT();
            return RRC_SUCCESS; 
        }
    }
    else
    {
        p_node = (active_mme_pool_area_node_t*)list_pop_head(p_active_mme_pool_area_list);
        if (p_node)
        {
            /* Allocate mme id */
            *p_mme_id  =  p_node->mme_id;
            rrc_mem_free(p_node);
            RRC_S1AP_TRACE(RRC_INFO,"mme id assigned:%d",*p_mme_id);
        }
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_SUCCESS; 
    }
    /* SPR 22789 Fix End */
}

/*****************************************************************************
 * Function Name  : build_and_send_mme_overload_ind
 * Inputs         : enb_ue_s1ap_id - id of UE over S1 inteface in eNB
 *                  p_s1ap_gb_ctx - Pointer to S1AP global context.
 * Outputs        : None
 * Returns        : NONE
 * Description    : The function send overload indication to UECC if selected
 *                  mme is overloaded.
 *****************************************************************************/
void build_and_send_mme_overload_ind(
        U32 enb_ue_s1ap_id,
/*BUG 604 changes start*/
        s1ap_gb_context_t   *p_s1ap_gb_ctx,
        U16                 ue_index)
/*BUG 604 changes stop*/
{
    U8 *p_api = PNULL;
    U8 *p_msg = PNULL;
    U16 msg_len = RRC_NULL;

    s1ap_overload_msg_ind_t s1ap_overload_msg_ind;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    
    memset_wrapper((void *)&s1ap_overload_msg_ind, 0, sizeof(s1ap_overload_msg_ind_t));

    /* Fill the API to be sent to UECC */
    s1ap_overload_msg_ind.enb_ue_s1ap_id = enb_ue_s1ap_id;
    s1ap_overload_msg_ind.cause.t = T_s1ap_Cause_misc;
    s1ap_overload_msg_ind.cause.u.misc = s1ap_control_processing_overload;
    /*BUG 604 changes start*/
    /*Fill ue_index*/
    s1ap_overload_msg_ind.ue_index = ue_index;
    /*BUG 604 changes stop*/

    msg_len = (U16)(sizeof(s1ap_overload_msg_ind_t));

    p_api = (U8*)s1ap_alloc_intrl_msg(RRC_UECC_MODULE_ID,
            S1AP_OVERLOAD_MSG_IND,
            msg_len);
    if(PNULL == p_api)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    p_msg = p_api + CV_HDRSIZE;
    if(PNULL == p_msg)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        /* Coverity Fix 48267 Start */
        if ( PNULL != p_api )
        { 
            /* SPR 15887 Start */
    	    rrc_msg_mem_free(p_api);
            /* SPR 15887 Stop */
    	    p_api = PNULL;
        }
        /* Coverity Fix 48267 Stop */
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }
    if (PNULL != p_api)
    {
        p_msg = p_api + CV_HDRSIZE;

        l3_memcpy_wrapper((void*)p_msg,(const void*)&s1ap_overload_msg_ind,
                sizeof(s1ap_overload_msg_ind_t));

/*BUG 604 changes start*/
        if ((RRC_GET_UECC_MODULE_ID(ue_index) -
                    RRC_UECC_1_MODULE_ID) < p_s1ap_gb_ctx->
                no_of_uecc_instances)
        {
            /* Send message to UECC */
            rrc_send_message(
                    p_api,
                    RRC_GET_UECC_MODULE_ID(ue_index));
        }
/*BUG 604 changes stop*/
        else
        {
	    RRC_S1AP_TRACE(RRC_WARNING,"UECC instance does not exist");
	    /* Coverity Fix 48267 Start */
	    if ( PNULL != p_api )
	    {
		/* SPR 15887 Start */
		rrc_msg_mem_free(p_api);
		/* SPR 15887 Stop */
		p_api = PNULL;
	    }
	    /* Coverity Fix 48267 Stop */
	}
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/* Bug 8198 Fix Start */
/****************************************************************************
 * Function Name  : match_overload_start_gummei_info
 * Inputs          :p_mme_ctx  : Pointer to mme context
 *                  p_gummei_lit : Pointer to gummei list
 *                  p_alloc_mme_req - Conains registered_mme infomation.
 * Outputs        : None
 * Returns        : RRC_SUCCESS - If match found.
 *                  RRC_FAILURE - If match not found.
 * Description    : This function compares gummelist of Overload messages for
 *                  overload condition
 ****************************************************************************/

U8  match_overload_start_gummei_info(
        s1ap_gummei_t* p_gummei_list,
        U16     num_gummei,
        s1ap_allocate_mme_req_t * p_alloc_mme_req,
        mme_context_t* p_mme_ctx)
{
    U16 count = 0;
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_alloc_mme_req );
    RRC_ASSERT(PNULL != p_mme_ctx);

    RRC_S1AP_TRACE(RRC_INFO,"mme id of overloaded MME is %d",p_mme_ctx->mme_id);

    for (count = 0; count < num_gummei;count++)
    {
        if ( !memcmp_wrapper(p_gummei_list[count].plmn_identity.data,
                    p_alloc_mme_req->plmn_identity.data,
                    p_alloc_mme_req->plmn_identity.numocts))
        {
            /* Matching PLMN found */
            RRC_S1AP_TRACE(RRC_DETAILED, "Matching PLMN Found");
        }
        else 
        {
            if (count == (num_gummei - 1))
            {
                RRC_S1AP_UT_TRACE_EXIT();
                return NO_MATCHING_GUMMEI;
            }
            else
            {
                continue;
            }
        }

        if ((p_alloc_mme_req->bitmask & REGISTERED_MMEGI_PRESENT) &&
                (p_alloc_mme_req->bitmask & REGISTERED_MMEC_PRESENT))
        {
            if ( ( p_gummei_list[count].grp_id.numocts == p_alloc_mme_req->mme_group_id.numocts) && 
                    (!memcmp_wrapper(&p_gummei_list[count].grp_id.data ,&p_alloc_mme_req->mme_group_id.data,
                                 p_gummei_list[count].grp_id.numocts)))
            {
                /* Matching PLMN AND MMEGI found */
                RRC_S1AP_TRACE(RRC_DETAILED, "Matching PLMN and MMEGI Found");
            }
            else
            {
                if(count == (num_gummei - 1))
                { 	
                    RRC_S1AP_UT_TRACE_EXIT();
                    return NO_MATCHING_GUMMEI;
                }
                else
                {
                    continue;
                }
            }
            if ( (p_gummei_list[count].mme_code.numocts == p_alloc_mme_req->mmec.numocts) && 
                    (!memcmp_wrapper(&(p_gummei_list[count].mme_code.data),
                                 &p_alloc_mme_req->mmec.data,
                                 p_alloc_mme_req->mmec.numocts)))
            {
                /* Matching GUMMEI Found */
                RRC_S1AP_UT_TRACE_EXIT();
                return MATCHING_GUMMEI;
            }
            else
            {
                if(count == (num_gummei - 1))
                { 	
                    RRC_S1AP_UT_TRACE_EXIT();
                    return NO_MATCHING_GUMMEI;
                }
                else
                {
                    continue;
                }
            }
        }
        else
        {
            if(count == (num_gummei - 1))
            { 	
                RRC_S1AP_UT_TRACE_EXIT();
                return NO_MATCHING_GUMMEI;
            }
            else
            {
                continue;
            }
        }

    }
    RRC_S1AP_UT_TRACE_EXIT();
    return NO_MATCHING_GUMMEI;

}
/* Bug 8198 Fix End */
