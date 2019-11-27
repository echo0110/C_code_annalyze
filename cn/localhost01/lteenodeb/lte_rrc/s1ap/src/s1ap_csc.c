/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 *  File Details
 *  ------------
 *  $Id: s1ap_csc.c,v 1.13 2010/10/13 07:04:19 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file contains the procedures that processes the messages to be 
 *  delivered from s1ap CSPL module to csc CSPL module and vice versa.
 *
 ****************************************************************************
 *
 *  Revision Details
 *  ----------------
 *  $Log: s1ap_csc.c,v $
 *  Revision 1.13  2010/10/13 07:04:19  gur18569
 *  Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 *  Revision 1.12.4.5  2010/10/04 13:53:59  gur04518
 *  Removed TABS
 *
 *  Revision 1.12.4.4  2010/10/04 13:31:54  gur04518
 *  removed backslash
 *
 *  Revision 1.12.4.3  2010/09/10 06:40:54  gur22047
 *  after mem_free doing ho_ctx NULL
 *
 *  Revision 1.12.4.2  2010/09/09 10:08:44  gur22047
 *  modified during s1ap UT
 *
 *  Revision 1.12.4.1  2010/08/16 12:09:36  gur22047
 *  lokesh_review_incorporate
 *
 *  Revision 1.12  2010/05/14 09:36:58  gur19827
 *  checklist,warning_removal,dst mod id,tdd,phy cell id added
 *
 *  Revision 1.11  2010/04/06 12:42:30  gur18569
 *  fixed ssit bug
 *
 *  Revision 1.10  2010/04/06 09:24:11  gur18569
 *  removed warning in delete ue record
 *
 *  Revision 1.9  2010/04/06 04:03:28  gur18569
 *  reset fwk chg
 *
 *  Revision 1.8  2010/03/30 06:48:39  gur18569
 *  Optimized Reset Procedure
 *
 *  Revision 1.7  2010/03/25 10:05:53  gur18569
 *   added functionality of Handling eNB Init Partial reset with More that 256 ue ids
 *
 *  Revision 1.6  2010/03/24 09:50:45  gur18569
 *  Merged files from Rel 1.0 support branch
 *
 *  Revision 1.5  2010/03/10 06:48:04  gur21006
 *  code review comments incorporated
 *
 *  Revision 1.4  2010/03/04 07:04:34  gur21006
 *  code comments incorporated
 *
 *  Revision 1.3  2010/02/18 04:25:20  gur10248
 *  LTE_RRC_Rel1.1_18feb
 *
 *  Revision 1.2  2010/01/04 16:10:05  ukr15916
 *  no message
 *
 *  Revision 1.1.2.8  2009/12/28 11:18:08  gur21006
 *  Cell id validation check added
 *
 *  Revision 1.1.2.7  2009/12/28 10:13:56  gur21006
 *  system calls removed
 *
 *  Revision 1.1.2.6  2009/12/28 05:09:42  gur18569
 *  fixed indentation
 *
 *  Revision 1.1.2.5  2009/12/27 08:39:04  gur18569
 *  removed tabs
 *
 *  Revision 1.1.2.4  2009/12/26 12:05:01  gur21006
 *  Warnings removed
 *
 *  Revision 1.1.2.3  2009/12/23 06:30:12  gur21006
 *  Warnings Removed
 *
 *  Revision 1.1.2.2  2009/12/03 09:24:08  ukr18877
 *  Points 5, 10 and 3MM (CSC) from mail "Questions about S1AP" fixed. Parameter RUNNER_RRC_UECC_FT set to 0 (for SSIT testing), OAM connect and cleanup SSIT tests added
 *
 *  Revision 1.1.2.1  2009/11/25 13:11:05  gur18569
 *  Shifted to level of rrc dir
 *
 *  Revision 1.1.2.4  2009/11/22 11:49:55  gur18569
 *  initializing cell context
 *
 *  Revision 1.1.2.3  2009/11/17 13:46:14  gur20470
 *  Incorporated review comments
 *
 *  Revision 1.1.2.2  2009/11/09 09:18:23  gur18569
 *  removed compilation warnings
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
#include "s1ap_csc.h"
#include "s1ap_api.h"
#include "s1ap_timers.h"
#include "s1ap_mme_fsm.h"
#include "rrc_s1ap_csc_intf.h"
#include "search_tree_wrap.h"
#include "s1ap_enb_mgr_fsm.h"
#include "s1ap_error_indication.h"
#include "rrc_logging.h"


/****************************************************************************
  Private Definitions
 ****************************************************************************/

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
void build_and_send_reset_part_to_mme(
        U8 cell_index,
        U8 num_cells_reg, 
        cell_s1ap_context_t *p_cell_s1ap_context,
    s1ap_gb_context_t *p_s1ap_gb_context);

static void s1ap_delete_all_ue_contexts_from_cell_context(s1ap_gb_context_t *p_s1ap_gb_context,
cell_s1ap_context_t *p_cell_s1ap_context);

    /*BUG 604 changes start*/
static void delete_ue_record(
        s1ap_gb_context_t *p_s1ap_gb_context,
        ue_s1ap_context_t  *p_ue_s1ap_context,
    mme_context_t *p_mme_context);
    /*BUG 604 changes stop*/

rrc_return_et parse_s1ap_paging(mme_context_t *p_mme_context,
                U8 *p_asn_msg,
                U16 asn_msg_len);
/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/*****************************************************************************
 * Function Name  : process_paging_proc
 * Inputs         : p_mme_context - pointer to mme_context_t
 *                  p_asn_msg - Pointer to input API buffer 
 *                  p_s1ap_gb_context - Pointer to the S1AP global context data
 *                  asn_msg_len - Length of the asn decoded msg
 * Outputs        : None 
 * Returns        : none
 * Description    : This function send the paging messsage indication to 
 *                  CSC module of RRC on receiving the paging message 
 *                  from MME.
 ****************************************************************************/
void s1ap_process_paging_proc(    
        mme_context_t *p_mme_context,
        void            *p_asn_msg,                
        U16             asn_msg_len)
{
    void *p_msg = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_asn_msg);

    /* ARGUSED */

    if (RRC_SUCCESS == parse_s1ap_paging(p_mme_context,
                p_asn_msg,
                asn_msg_len))
    {
    /* Send Paging message indication to CSC  */
    p_msg = s1ap_alloc_intrl_msg(
            RRC_CSC_MODULE_ID,
            S1AP_PAGING_IND,     
            asn_msg_len);
    if (PNULL!=p_msg)
    {

        l3_memcpy_wrapper(
                (void*)( (U8*) p_msg + RRC_API_HEADER_SIZE),
                (const void*)  p_asn_msg,
                asn_msg_len);

        /* Send response */
        rrc_send_message(
                p_msg, 
                RRC_CSC_MODULE_ID);
    }        
    }
    else
    {
        /* Failure */
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : parse_s1ap_paging 
 * Inputs         : p_mme_context - pointer to MME context 
 *                  p_asn_msg - Pointer to input API buffer 
 *                  asn_msg_len - Length of the asn decoded msg
 * Outputs        : None
 * Returns        : None
 * Description    : This function parses the S1AP paging message. 
 ****************************************************************************/
rrc_return_et parse_s1ap_paging(mme_context_t *p_mme_context,
                U8 *p_asn_msg,
                U16 asn_msg_len)
{
    S1AP_PDU        s1ap_pdu;
    OSCTXT          asn1_ctx;
    s1ap_Paging *p_s1ap_paging = PNULL;
    s1ap_Paging_protocolIEs_element *p_paging_elem = PNULL;
    U32  index = RRC_NULL;

    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    s1ap_error_ind_bool_t      send_err_ind;
    rrc_s1ap_error_indication_t  err_ind_content;
    rrc_return_et result = RRC_SUCCESS;
    OSRTDListNode  *p_node     = PNULL;

    s1ap_message_data_t message_map = 
    {7, RRC_FALSE, RRC_FALSE, 0, 0, 0,
    /* eMTC changes stop */
      { {0, ASN1V_s1ap_id_UEIdentityIndexValue, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
        {1, ASN1V_s1ap_id_UEPagingID, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
        {2, ASN1V_s1ap_id_pagingDRX, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
        {3, ASN1V_s1ap_id_CNDomain, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
        {4, ASN1V_s1ap_id_TAIList, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
        {5, ASN1V_s1ap_id_CSG_IdList, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
        {6, ASN1V_s1ap_id_PagingPriority, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};
        /* eMTC changes stop */


    memset_wrapper(&iE_list, RRC_NULL, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, RRC_NULL, sizeof(s1ap_error_ind_bool_t));
    memset_wrapper(&err_ind_content, RRC_NULL, sizeof(rrc_s1ap_error_indication_t));

    /* Init ASN1 context */
    if (0 != rtInitContext(&asn1_ctx))
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }


    asn1Init_S1AP_PDU(&s1ap_pdu);

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            s1ap_build_and_send_err_indication(p_mme_context, PNULL, PNULL);

            result = RRC_FAILURE;
            break;
        }
        
        
        RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: Paging %s", change_data_to_str(asn_msg_len, p_asn_msg));
        rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN, "Paging_Message", &s1ap_pdu);        

        p_s1ap_paging = s1ap_pdu.u.initiatingMessage->value.u.paging;

        p_node = p_s1ap_paging->protocolIEs.head;

        for (index =0; index < p_s1ap_paging->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_TRACE(RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_paging_elem = (s1ap_Paging_protocolIEs_element *)
                p_node->data;

            switch(p_paging_elem->id)
            {
                case ASN1V_s1ap_id_UEIdentityIndexValue:
                    {
                        if (RRC_SUCCESS != validate_ie_value(&message_map,
                                    index,
                                    p_paging_elem->id,
                                    p_paging_elem->value.u._PagingIEs_1))
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "Validation Failed for"
                            "Paging IE1 : ""S1AP PAGING");    
                        }
                        break;
                    }
                case ASN1V_s1ap_id_UEPagingID:
                    {
                        if (RRC_SUCCESS != validate_ie_value(&message_map,
                                    index,
                                    p_paging_elem->id,
                                    p_paging_elem->value.u._PagingIEs_2))
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "Validation Failed for"
                            "Paging IE2 : ""S1AP PAGING");    
                        }
                        break;
                    }
                case ASN1V_s1ap_id_pagingDRX:
                    {
                        if (RRC_SUCCESS != validate_ie_value(&message_map,
                                    index,
                                    p_paging_elem->id,
                                    &p_paging_elem->value.u._PagingIEs_3))
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "Validation Failed for"
                            "Paging IE3 : ""S1AP PAGING");    
                        }
                        break;
                    }
                case ASN1V_s1ap_id_CNDomain:
                    {
                        if (RRC_SUCCESS != validate_ie_value(&message_map,
                                    index,
                                    p_paging_elem->id,
                                    &p_paging_elem->value.u._PagingIEs_4))
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "Validation Failed for"
                            "Paging IE4 : ""S1AP PAGING");    
                        }
                        break;
                    }
                case ASN1V_s1ap_id_TAIList:
                    {
                        if (RRC_SUCCESS != validate_ie_value(&message_map,
                                    index,
                                    p_paging_elem->id,
                                    p_paging_elem->value.u._PagingIEs_5))
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "Validation Failed for"
                            "Paging IE5 : ""S1AP PAGING");    
                        }
                        break;
                    }
                case ASN1V_s1ap_id_CSG_IdList:
                    {
                        if (RRC_SUCCESS != validate_ie_value(&message_map,
                                    index,
                                    p_paging_elem->id,
                                    p_paging_elem->value.u._PagingIEs_6))
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "Validation Failed for"
                            "Paging IE6 : ""S1AP PAGING");    
                        }
                        break;
                    }
                case ASN1V_s1ap_id_PagingPriority:
                    {
                        if (RRC_SUCCESS != validate_ie_value(&message_map,
                                    index,
                                    p_paging_elem->id,
                                    &p_paging_elem->value.u._PagingIEs_7))
                        {
                            RRC_S1AP_TRACE(RRC_INFO, "Validation Failed for"
                            "Paging IE7 : ""S1AP PAGING");
                        }
                        break;
                    }    
                /* eMTC changes stop */
                default:
                    add_to_err_ind_ie_list(&iE_list,
                            p_paging_elem->criticality,
                            p_paging_elem->id,
                            &ie_list_index,
                            &send_err_ind,
                            RRC_FALSE,
                            RRC_FALSE);
            }
            p_node = p_node->next;
        }
    }while(0);

    if (RRC_SUCCESS == parse_message_map(
                &asn1_ctx,
                p_mme_context,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_Paging,
                T_S1AP_PDU_initiatingMessage,
                s1ap_ignore,
                &err_ind_content))
    {
        RRC_TRACE(RRC_INFO, "Error Indication Detected");
        result = RRC_FAILURE;
    }

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/*****************************************************************************
 * Function Name  : process_csc_messages
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_context - Pointer to the S1AP global context data
 * Outputs        : None 
 * Returns        : none
 * Description    : This function processes the messsages coming from 
 *                  CSC module of RRC
 ****************************************************************************/
void s1ap_process_csc_messages(    
        void            *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context)
{
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_context);

    switch (rrc_get_api_id((U8*)p_api))
    {
        case S1AP_CELL_REG_REQ:
            {
                RRC_S1AP_TRACE(RRC_INFO, "Processing CSC Messages : "
                        "S1AP CELL REG REQ");    
                    process_s1ap_cell_register_req(
                            p_api,
                            p_s1ap_gb_context);
                break;                
            }
        case S1AP_CELL_DEREG_REQ:
            {
                RRC_S1AP_TRACE(RRC_INFO, "Processing CSC Messages : "
                        "S1AP CELL DREG REQ");    
                    process_s1ap_cell_deregister_req(
                            p_api,
                            p_s1ap_gb_context);
                break;                
            }
        case S1AP_PWS_RESP:
            {
                RRC_S1AP_TRACE(RRC_INFO, "Processing CSC Messages : "
                        "S1AP_PWS_RESP");    
                    process_s1ap_pws_resp(
                            p_api,
                            p_s1ap_gb_context);
                break;                
            }
        case S1AP_PWS_FAILURE_RESP:
            {
                RRC_S1AP_TRACE(RRC_INFO, "Processing CSC Messages : "
                        "S1AP_PWS_FAILURE_RESP");    
                    process_s1ap_pws_failure_resp(
                            p_api,
                            p_s1ap_gb_context);
                break;                
            }
        case S1AP_KILL_RESP:
            {
                RRC_S1AP_TRACE(RRC_INFO, "Processing CSC Messages : "
                        "S1AP_KILL_RESP");    
                    process_s1ap_kill_resp(
                            p_api,
                            p_s1ap_gb_context);
                break;                
            }
        case S1AP_CELL_AREA_INFO_UPDATE_IND:
            {
                RRC_S1AP_TRACE(RRC_INFO, "Processing CSC Messages : "
                        "S1AP CELL REG REQ");    
                    process_s1ap_cell_area_info_update_ind(
                            p_api,
                            p_s1ap_gb_context);
                break;                
            }
        
        default:
            {
                RRC_S1AP_TRACE(RRC_ERROR, "Processing CSC Messages : "
                        "Unknown API : %d",rrc_get_api_id((U8*)p_api));    
                    break;
            }
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_s1ap_cell_register_req
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_context - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the messsages coming  
 *                  _s1ap_cell_register_req messgage from CSC.
 ****************************************************************************/
void process_s1ap_cell_register_req(    
        void            *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context)
{
    s1ap_cell_reg_req_t *p_s1ap_cell_reg_req = PNULL;
    cell_s1ap_context_t *p_cell_s1ap_context = PNULL;
    U8 cell_index = MAX_NUM_CELLS;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_context);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size((U8*)p_api) != RRC_API_HEADER_SIZE + 
            sizeof(s1ap_cell_reg_req_t))
    {
        RRC_S1AP_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }
    p_s1ap_cell_reg_req = (s1ap_cell_reg_req_t *)((U8 *)p_api + 
            RRC_API_HEADER_SIZE);

    /* Get the cell index from buffer */
    cell_index =     p_s1ap_cell_reg_req->cell_index;
    
    SET_CELL_INDEX(cell_index);

    if(MAX_NUM_CELLS <= cell_index)
    {
        RRC_S1AP_TRACE(RRC_WARNING, "[%s] Invalid cell index\n",__FUNCTION__);
        return;
    }    
    if (PNULL != p_s1ap_gb_context->cell_s1ap_contexts[cell_index])
    {
        RRC_S1AP_TRACE(RRC_WARNING, "Cell context already exists at cell index "
                "%d",cell_index);
        return;
    }        

    p_cell_s1ap_context = (cell_s1ap_context_t *)rrc_mem_get(
            sizeof(cell_s1ap_context_t));

    if(PNULL == p_cell_s1ap_context)
    {
        RRC_S1AP_TRACE(RRC_WARNING, "Memory Allocation failed for cell ctxt\n");
        return;
    }    

    memset_wrapper((void*)p_cell_s1ap_context,RRC_NULL,sizeof(cell_s1ap_context_t));

    /* Create the context of cell at cell index in s1ap global context */
    RRC_S1AP_TRACE(RRC_INFO, "Cell context created at cell index "
            "%d",cell_index);

    /*Storing the PLMN ID in the cell context*/
    p_cell_s1ap_context->s1ap_cell_area_info.cell_index = cell_index;

    l3_memcpy_wrapper(p_cell_s1ap_context->s1ap_cell_area_info.plmn_id.plmn_id,
        p_s1ap_gb_context->s1_setup_info.gb_enb_id.plmn_identity.plmn_id,
        sizeof(p_s1ap_gb_context->s1_setup_info.gb_enb_id.plmn_identity.plmn_id));
    
    /*Storing the cell area info in cell context*/
    p_cell_s1ap_context->s1ap_cell_area_info.cell_area_info =
        p_s1ap_cell_reg_req->cell_area_info; 

    p_cell_s1ap_context->max_num_supported_ue = p_s1ap_cell_reg_req->max_num_supported_ue;

    p_s1ap_gb_context->cell_s1ap_contexts[cell_index] = p_cell_s1ap_context;
    RRC_S1AP_UT_TRACE_EXIT();
}





/*****************************************************************************
 * Function Name  : process_s1ap_cell_deregister_req
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_context - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the cell deregistration message  
 *                  coming from CSC module of RRC.
 ****************************************************************************/
void process_s1ap_cell_deregister_req(    
        void            *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context)
{
    U8 cell_count = 0;
    U8 num_cells_reg = 0;
    U8 mme_id = 0;
    s1ap_cell_dreg_req_t *p_s1ap_cell_dereg_req = PNULL;
    cell_s1ap_context_t *p_cell_s1ap_context = PNULL;
    
    U8 cell_index = RRC_NULL;
    
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_context);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size((U8*)p_api) != RRC_API_HEADER_SIZE + 
            sizeof(s1ap_cell_dreg_req_t))
    {
        RRC_S1AP_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }
    
    p_s1ap_cell_dereg_req = (s1ap_cell_dreg_req_t *)((U8 *)p_api + 
            RRC_API_HEADER_SIZE);

    /* Get the cell index from buffer */
    cell_index =     p_s1ap_cell_dereg_req->cell_index;
    
    SET_CELL_INDEX(cell_index);

    if(MAX_NUM_CELLS <= cell_index)
    {
        RRC_S1AP_TRACE(RRC_WARNING, "[%s] Invalid cell index\n",__FUNCTION__);
        return;
    }    

    if (PNULL == p_s1ap_gb_context->cell_s1ap_contexts[cell_index])
    {
        RRC_S1AP_TRACE(RRC_WARNING, 
                "Cell context does not exists at cell index "
                "%d",cell_index);
        RRC_S1AP_UT_TRACE_EXIT();
        return; 
    }        
    else
    {
        for (cell_count = 0; cell_count < MAX_NUM_CELLS; 
                cell_count++)
        {
            if(p_s1ap_gb_context->cell_s1ap_contexts[cell_count])
            {
                num_cells_reg++;
            }
        }

        p_cell_s1ap_context = p_s1ap_gb_context->cell_s1ap_contexts[cell_index];
        build_and_send_reset_part_to_mme(
                                     cell_index,
                                     num_cells_reg,
                                     p_cell_s1ap_context,
                                     p_s1ap_gb_context);
    
        if(PNULL != p_s1ap_gb_context->p_pws_info)
        {
            mme_id = p_s1ap_gb_context->p_pws_info->mme_id;
            /* call the enb mgr appropriate fsm */
            enb_mgr_fsm_write_replace_warning_process_event(
                    ENB_EV_CELL_DREG_REQ,
                    p_api,
                    p_s1ap_cell_dereg_req,
                    p_s1ap_gb_context,
                    p_s1ap_gb_context->mme_contexts[mme_id]);
        }
    }        

}

/*****************************************************************************
 * Function Name  : process_s1ap_cell_area_info_update_ind
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_context - Pointer to the S1AP global context data
 * Outputs        : None 
 * Returns        : None
 * Description    : This function processes the messsages coming from 
 *                  s1ap_cell_area_info_update_ind message CSC module of RRC.
 ****************************************************************************/
void process_s1ap_cell_area_info_update_ind(    
        void            *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context)
{
    s1ap_cell_area_info_update_ind_t *p_s1ap_cell_area_update_ind = PNULL;
    cell_s1ap_context_t *p_cell_s1ap_context = PNULL;
    U8 cell_index = MAX_NUM_CELLS;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_context);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size((U8*)p_api) != RRC_API_HEADER_SIZE + 
            sizeof(s1ap_cell_area_info_update_ind_t))
    {
        RRC_S1AP_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }
    p_s1ap_cell_area_update_ind = (s1ap_cell_area_info_update_ind_t*)
      ((U8 *)p_api + RRC_API_HEADER_SIZE);

    /* Get the cell index from buffer */
    cell_index =     p_s1ap_cell_area_update_ind->cell_index;
  
    SET_CELL_INDEX(cell_index);

    if(MAX_NUM_CELLS <= cell_index)
    {
      RRC_S1AP_TRACE(RRC_WARNING, "[%s] Invalid cell index\n",__FUNCTION__);
      return;
    }    
    if (PNULL == (p_cell_s1ap_context =
          p_s1ap_gb_context->cell_s1ap_contexts[cell_index]))
    {
      RRC_S1AP_TRACE(RRC_WARNING, "Cell context does not exist at cell index "
          "%d",cell_index);
        return;
    }        

    /*Storing the cell area info in cell context*/
    if(p_s1ap_cell_area_update_ind->cell_area_info.bitmask &
        S1AP_CELL_INFO_UPDATE_IND_CELL_ID_PRESENT)
    {
      l3_memcpy_wrapper(p_cell_s1ap_context->s1ap_cell_area_info.cell_area_info.cell_id, 
          p_s1ap_cell_area_update_ind->cell_area_info.cell_id,CELL_ID_OCTET_SIZE);
      p_cell_s1ap_context->s1ap_cell_area_info.cell_area_info.bitmask |=
        S1AP_CELL_INFO_UPDATE_IND_CELL_ID_PRESENT;
    }

    if(p_s1ap_cell_area_update_ind->cell_area_info.bitmask &
        S1AP_CELL_INFO_UPDATE_IND_TAC_PRESENT)
    {
      l3_memcpy_wrapper(p_cell_s1ap_context->s1ap_cell_area_info.cell_area_info.tac, 
          p_s1ap_cell_area_update_ind->cell_area_info.tac,TAC_OCTET_SIZE);
      p_cell_s1ap_context->s1ap_cell_area_info.cell_area_info.bitmask |=
        S1AP_CELL_INFO_UPDATE_IND_TAC_PRESENT;
    }

    if(p_s1ap_cell_area_update_ind->cell_area_info.bitmask &
        S1AP_CELL_INFO_UPDATE_IND_EMERGENCY_ID_PRESENT)
    {
      l3_memcpy_wrapper(p_cell_s1ap_context->s1ap_cell_area_info.cell_area_info.emergency_id, 
          p_s1ap_cell_area_update_ind->cell_area_info.emergency_id,EMERGENCY_AREA_SIZE);
      p_cell_s1ap_context->s1ap_cell_area_info.cell_area_info.bitmask |=
       S1AP_CELL_INFO_UPDATE_IND_EMERGENCY_ID_PRESENT;
    }

    RRC_S1AP_UT_TRACE_EXIT();
}




/*****************************************************************************
 * Function Name  : build_and_send_reset_part_to_mme
 * Inputs         : cell_index - Index of Cell context.
 *                  num_cells_reg - Total number of cells presently registered.
 *                  p_cell_s1ap_context - Pointer to cell context 
 *                  p_s1ap_gb_context - Pointer to the S1AP global context data
 * Outputs        : None
 * Returns        : None
 *  Description   : This function builds the RESET-part message and sends to 
 *                  all impacted MMEs.
 ****************************************************************************/

void build_and_send_reset_part_to_mme(
        U8 cell_index,
        U8 num_cells_reg, 
        cell_s1ap_context_t *p_cell_s1ap_context,
    s1ap_gb_context_t *p_s1ap_gb_context)
{
    U8 enb_ue_id_pair_list = 0;
    active_mme_info_t *p_active_mme_info        =  PNULL;
    ue_s1ap_context_t *p_ue_s1ap_context        =  PNULL;
    
    ho_ongoing_ue_ctx_node_t *p_ho_ongoing_ue_ctx_node = PNULL;
    ho_ongoing_ue_ctx_node_t *p_ho_ongoing_ue_ctx_node_next = PNULL;
 
    U16 ue_id             = RRC_NULL; 
    U32 enb_ue_s1ap_id    = RRC_NULL;
    s1ap_enb_reset_info_t  reset_input_info;
    U8    uecc_index = RRC_NULL;
    U16   ue_count = RRC_NULL;
    U16   max_enb_ue_id_pair_list = RRC_NULL;

    RRC_S1AP_UT_TRACE_ENTER();
    
    RRC_ASSERT(PNULL != p_cell_s1ap_context);
    RRC_ASSERT(PNULL != p_s1ap_gb_context);

    p_active_mme_info = &(p_s1ap_gb_context->active_mme_info);
   
    if(PNULL == p_active_mme_info ||
       RRC_NULL == p_active_mme_info->active_mme_count)
    {    
        RRC_S1AP_TRACE(RRC_ERROR,"[%s] : No active mme present",
                       __FUNCTION__);

        /* Free all ue_s1_contexts and cell context at S1AP */
        s1ap_delete_all_ue_contexts_from_cell_context(p_s1ap_gb_context,p_cell_s1ap_context);

        p_s1ap_gb_context->cell_s1ap_contexts[cell_index] = PNULL;
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    /* If eNB has only One cell registered then, Send
       RESET S1 Interface*/
    if (num_cells_reg == 1)
    {
        RRC_S1AP_TRACE(RRC_INFO,
                "[%s] num_cells_reg is equal to 1 ",
                __FUNCTION__);
        /* SPR 14969 Fix Start */
        /*Set the Cause value*/
        reset_input_info.cause.t = 
            T_s1ap_Cause_misc;
        reset_input_info.cause.u.misc = 
            s1ap_om_intervention;
        /* SPR 14969 Fix Stop */
        reset_input_info.num_of_items = 0;

        s1ap_add_and_process_reset_context(
                p_s1ap_gb_context,
                &reset_input_info,
                (RRC_MAX_EXT_MODULE_ID +1),
                1,1,0);
    }
    else
    {
        RRC_S1AP_TRACE(RRC_INFO,
                "[%s] num_cells_reg is not equal to 1 ",
                __FUNCTION__);
        memset_wrapper((void*)&reset_input_info, 0 ,sizeof(s1ap_enb_reset_info_t));

        max_enb_ue_id_pair_list = (p_s1ap_gb_context->
        cell_s1ap_contexts[cell_index]->max_num_supported_ue)/(MAX_UE_S1AP_ID_PAIR);

        if ( (p_s1ap_gb_context->cell_s1ap_contexts[cell_index]->max_num_supported_ue)%
                (MAX_UE_S1AP_ID_PAIR) > RRC_NULL)
        {
            max_enb_ue_id_pair_list++;
        }

        /* SPR 14969 Fix Start */
        /*Set the Cause value*/
        reset_input_info.cause.t = 
            T_s1ap_Cause_misc;
        reset_input_info.cause.u.misc = 
            s1ap_om_intervention;
        /* SPR 14969 Fix Stop */
        reset_input_info.num_of_items = 0;

        /* Iterate the ue context array for the Cell */
        for(uecc_index = 0; uecc_index< p_s1ap_gb_context->no_of_uecc_instances && 
                enb_ue_id_pair_list < max_enb_ue_id_pair_list; uecc_index++)
        {
            for (ue_id = 0; ue_id < p_s1ap_gb_context->total_ue_supported && 
                    enb_ue_id_pair_list < max_enb_ue_id_pair_list; ue_id++)
            {

                /*BUG 604 changes start*/
                if (PNULL != p_s1ap_gb_context->p_p_ue_s1ap_contexts)
                {
                    p_ue_s1ap_context = p_s1ap_gb_context->p_p_ue_s1ap_contexts[ue_id];
                    /*BUG 604 changes stop*/
                    if ( p_ue_s1ap_context == PNULL || p_ue_s1ap_context->cell_index !=
                            p_cell_s1ap_context->s1ap_cell_area_info.cell_index)
                    {
                        continue;
                    }

                    /*If any UE context is present add it to reset_input_info */
                    reset_input_info.enbUeS1apId_arr
                        [reset_input_info.num_of_items].
                        mme_id = p_ue_s1ap_context->mme_id;

                    /*Storing enb_ue_s1ap_id in RESET Info list*/
                    enb_ue_s1ap_id = ue_id;
                    /* First three bits are reserved for UECC Module Id */
                    enb_ue_s1ap_id |= (U32)uecc_index << 0x0D;

                    reset_input_info.enbUeS1apId_arr
                        [reset_input_info.num_of_items].
                        enbUeS1apId = enb_ue_s1ap_id;

                    reset_input_info.num_of_items++; 
                    ue_count++;

                    /* If number of UE is have reached MAX_UE_S1AP_ID_PAIR,
                       then break and send RESET Message to corresponding MME*/
                    if (MAX_UE_S1AP_ID_PAIR == 
                            reset_input_info.num_of_items)
                    {
                        s1ap_add_and_process_reset_context(
                                p_s1ap_gb_context,
                                &reset_input_info,
                                (RRC_MAX_EXT_MODULE_ID +1),
                                1,1,0);

                        memset_wrapper((void*)&reset_input_info, 0 ,sizeof(s1ap_enb_reset_info_t));

                        /* Now fill rest of the ue's in next index */
                        enb_ue_id_pair_list++;

                                /* SPR 14969 Fix Start */
                                /*Set the Cause value*/
                        reset_input_info.cause.t = 
                                    T_s1ap_Cause_misc;
                                reset_input_info.cause.u.misc = 
                                    s1ap_om_intervention;
                                /* SPR 14969 Fix Stop */
                        reset_input_info.num_of_items = 0;
                    }
                }
            }
        }
        if (reset_input_info.num_of_items > RRC_NULL)
            {
                s1ap_add_and_process_reset_context(
                        p_s1ap_gb_context,
                        &reset_input_info,
                        (RRC_MAX_EXT_MODULE_ID +1),
                        1,1,0);
            }
    }
    /* If eNB has only One cell registered delete HO temporary context*/
    if (num_cells_reg == 1)
    {
        RRC_S1AP_TRACE(RRC_INFO,
                "[%s] num_cells_reg is equal to 1 ",
                __FUNCTION__);
        /*Set the Cause value*/
        p_ho_ongoing_ue_ctx_node = (ho_ongoing_ue_ctx_node_t *)
            list_first_node(&p_s1ap_gb_context->ho_ongoing_ue_ctx_list);
        {
            while(p_ho_ongoing_ue_ctx_node)
            {
                /*SPR 21220 Start*/
                /*BUG 604 changes start*/
                /*Free s1ap_ue_id Ctx*/
                s1ap_ueid_free(p_s1ap_gb_context,
                        p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.enb_ue_s1ap_id);
                /*BUG 604 changes stop*/
                /*SPR 21220 Stop*/

                /* Delete ho_ongoing_ue_ctx from the ho_ongoing_ue_ctx_list */
                list_delete_node(&p_s1ap_gb_context->ho_ongoing_ue_ctx_list, (void
                            *)p_ho_ongoing_ue_ctx_node);
                RRC_S1AP_TRACE(RRC_INFO,"%s HO ONGOING UE Context DELETED"
                        "\n",__FUNCTION__);
                p_ho_ongoing_ue_ctx_node_next = (ho_ongoing_ue_ctx_node_t *)
                    list_next_node(&p_ho_ongoing_ue_ctx_node->node);
                rrc_mem_free((void*)p_ho_ongoing_ue_ctx_node);
                p_ho_ongoing_ue_ctx_node = PNULL;

                p_ho_ongoing_ue_ctx_node = p_ho_ongoing_ue_ctx_node_next; 
            }
        }
    }


    /* Deleting cell context */
    RRC_S1AP_TRACE(RRC_INFO,"%s Deleting Cell context"
            "\n",__FUNCTION__);
    s1ap_delete_all_ue_contexts_from_cell_context(p_s1ap_gb_context,p_cell_s1ap_context);
    p_s1ap_gb_context->cell_s1ap_contexts[cell_index] = PNULL;
    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : delete_ue_record
 * Inputs         : p_ue_s1ap_context - Pointer to UE context
 *                  p_mme_context- Pointer to the mme context
 * Outputs        : None
 * Returns        : None
 * Description    : This function deletes a particular UE context. 
 ****************************************************************************/
    
void delete_ue_record(
        /*BUG 604 changes start*/
                      s1ap_gb_context_t *p_s1ap_gb_context,
                      ue_s1ap_context_t  *p_ue_s1ap_context,
    /*BUG 604 changes stop*/
                      mme_context_t *p_mme_context )
{
    
    void *p_mmeUeS1apId_st_node = PNULL;
    
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_ue_s1ap_context);
    RRC_ASSERT(PNULL != p_mme_context);

    if (p_ue_s1ap_context->is_mme_ue_s1ap_id_valid)
    {
        /*Deleting from MME context*/
        p_mmeUeS1apId_st_node = (search_tree_node_t *)st_find_tree_node(
                &(p_mme_context->mmeUeS1apId_search_tree),
                &p_ue_s1ap_context->mme_ue_s1ap_id);

        /*BUG 604 changes start*/
        /*Free s1ap_ue_id Ctx*/
        s1ap_ueid_free(p_s1ap_gb_context,
                p_ue_s1ap_context->enb_ue_s1ap_id);
        /*BUG 604 changes stop*/
        /* Deleting UE context from cell context*/
        rrc_mem_free(p_ue_s1ap_context);

        if (!p_mmeUeS1apId_st_node)
        {
            RRC_S1AP_TRACE(RRC_WARNING, "[%s:] "
                    "mmeUeS1apId is not preset in the search tree",__FUNCTION__);
            RRC_S1AP_UT_TRACE_EXIT();
            return;
        }

        st_delete_node(&p_mme_context->mmeUeS1apId_search_tree,
                (search_tree_node_t*)p_mmeUeS1apId_st_node);
        rrc_mem_free(p_mmeUeS1apId_st_node);
    }
    else
    {
        /*BUG 604 changes start*/
        /*Free s1ap_ue_id Ctx*/
        s1ap_ueid_free(p_s1ap_gb_context,
                p_ue_s1ap_context->enb_ue_s1ap_id);
        /*BUG 604 changes stop*/
        /* Deleting UE context from cell context*/
        rrc_mem_free(p_ue_s1ap_context);
    }
    RRC_S1AP_UT_TRACE_EXIT();
    
}
                    
/*****************************************************************************
 * Function Name  : s1ap_delete_all_ue_contexts_from_cell_context
 * Inputs         : p_s1ap_gb_context - pointer to S1AP global context 
 *                  p_cell_s1ap_context - Pointer to the cell context
 * Outputs        : None
 * Returns        : none
 * Description    : This function cleans the memory allocated for all 
 *                  ue contexts in cell context of an UE.
 ****************************************************************************/
void s1ap_delete_all_ue_contexts_from_cell_context(
        s1ap_gb_context_t *p_s1ap_gb_context,
        cell_s1ap_context_t *p_cell_s1ap_context)    
{
                /*BUG 604 changes start*/
    //U16 uecc_count = 0;
                /*BUG 604 changes start*/
    U16 ue_count = 0;
    ue_s1ap_context_t *p_ue_ctx = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    if (PNULL == p_cell_s1ap_context)
    {
        RRC_S1AP_TRACE(RRC_WARNING,
    "[%s] : Unable to get the address of cell context",
    __FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return ;
    }

    /*BUG 604 changes start*/
    /* To delete the UE contexts cell wise */
    //for (uecc_count = 0; uecc_count < p_s1ap_gb_context->no_of_uecc_instances ; uecc_count++)
    /*BUG 604 changes start*/
    for (ue_count = 0; ue_count < p_s1ap_gb_context->total_ue_supported; ue_count++)
    {
        /*BUG 604 changes start*/
        if (PNULL != p_s1ap_gb_context->p_p_ue_s1ap_contexts)
        {
            p_ue_ctx = p_s1ap_gb_context->p_p_ue_s1ap_contexts[ue_count];

            if (p_ue_ctx != PNULL
                    && (p_ue_ctx->cell_index == 
                        p_cell_s1ap_context->s1ap_cell_area_info.cell_index))
            {
                RRC_S1AP_TRACE(RRC_INFO,
                        "[%s] : deleting ue record",
                        __FUNCTION__);
    /*BUG 604 changes start*/
                delete_ue_record( p_s1ap_gb_context,p_ue_ctx, p_s1ap_gb_context->
                        mme_contexts[p_ue_ctx->mme_id]);
    /*BUG 604 changes stop*/

                p_ue_ctx = PNULL;
                p_s1ap_gb_context->p_p_ue_s1ap_contexts[ue_count] = PNULL;
            }
        }
    }

    rrc_mem_free(p_cell_s1ap_context);
    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_s1ap_pws_resp
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_context - Pointer to the S1AP global context data
 * Outputs        : None 
 * Returns        : none
 * Description    : This function processes the messsages coming 
 *                  S1AP pws request from CSC module
 ****************************************************************************/
void process_s1ap_pws_resp(    
    void            *p_api,                
    s1ap_gb_context_t    *p_s1ap_gb_context)
{
  s1ap_pws_resp_t *p_s1ap_pws_resp = PNULL;
  U8 cell_index = MAX_NUM_CELLS;
  U8 mme_id = RRC_NULL;

  RRC_S1AP_UT_TRACE_ENTER();
  RRC_ASSERT(PNULL != p_api);
  RRC_ASSERT(PNULL != p_s1ap_gb_context);

  do {

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size((U8*)p_api) != RRC_API_HEADER_SIZE + 
        sizeof(s1ap_pws_resp_t))
    {
      RRC_S1AP_TRACE(RRC_WARNING, "Wrong message size - ignore.");
      RRC_S1AP_UT_TRACE_EXIT();
      return;
    }

    p_s1ap_pws_resp = (s1ap_pws_resp_t *)((U8 *)p_api + 
        RRC_API_HEADER_SIZE);

    /* Get the cell index from buffer */
    cell_index =     p_s1ap_pws_resp->cell_index;
    
    SET_CELL_INDEX(cell_index);

    if(MAX_NUM_CELLS <= cell_index)
    {
      RRC_S1AP_TRACE(RRC_WARNING, "[%s] Invalid cell index\n",__FUNCTION__);
      return;
    }   
    else if (PNULL == p_s1ap_gb_context->cell_s1ap_contexts[cell_index])
    {
      RRC_S1AP_TRACE(RRC_WARNING, "Cell context already exists at cell index "
          "%d",cell_index);
      return;
    }
    else
    {
            /* Received response from all the CSCs */

            /* call the appropriate fsm */
            enb_mgr_fsm_write_replace_warning_process_event(
                        ENB_EV_RRC_PWS_RESP,
                        p_api,
                        p_s1ap_pws_resp,
                        p_s1ap_gb_context,
                        p_s1ap_gb_context->mme_contexts[mme_id]);

    }

  }while (0);            

  RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : process_s1ap_pws_failure_resp
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_context - Pointer to the S1AP global context data
 * Outputs        : None 
 * Returns        : None
 * Description    : This function processes the messsage S1AP pws failure 
 *                  responce coming from CSC module.
 ****************************************************************************/
void process_s1ap_pws_failure_resp(    
    void            *p_api,                
    s1ap_gb_context_t    *p_s1ap_gb_context)
{
  s1ap_pws_failure_resp_t *p_s1ap_pws_failure_resp = PNULL;
  U8 cell_index = MAX_NUM_CELLS;
  U8 mme_id = RRC_NULL;

  RRC_S1AP_UT_TRACE_ENTER();
  RRC_ASSERT(PNULL != p_api);
  RRC_ASSERT(PNULL != p_s1ap_gb_context);

  do {

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size((U8*)p_api) != RRC_API_HEADER_SIZE + 
        sizeof(s1ap_pws_failure_resp_t))
    {
      RRC_S1AP_TRACE(RRC_WARNING, "Wrong message size - ignore.");
      RRC_S1AP_UT_TRACE_EXIT();
      return;
    }

    p_s1ap_pws_failure_resp = (s1ap_pws_failure_resp_t *)((U8 *)p_api + 
        RRC_API_HEADER_SIZE);

    /* Get the cell index from buffer */
    cell_index = p_s1ap_pws_failure_resp->cell_index;

    SET_CELL_INDEX(cell_index);

    if(MAX_NUM_CELLS <= cell_index)
    {
      RRC_S1AP_TRACE(RRC_WARNING, "[%s] Invalid cell index\n",__FUNCTION__);
      return;
    }   
    else if (PNULL == p_s1ap_gb_context->cell_s1ap_contexts[cell_index])
    {
      RRC_S1AP_TRACE(RRC_WARNING, "Cell context already exists at cell index "
          "%d",cell_index);
      return;
    }
    else
    {
            /* call the appropriate fsm */
            enb_mgr_fsm_write_replace_warning_process_event(
                        ENB_EV_RRC_PWS_RESP,
                        p_api,
                        p_s1ap_pws_failure_resp,
                        p_s1ap_gb_context,
                        p_s1ap_gb_context->mme_contexts[mme_id]);
    }

  }while (0);            

  RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : process_s1ap_kill_resp
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_context - Pointer to the S1AP global context data
 * Outputs        : None 
 * Returns        : None
 * Description    : This function processes the S1AP kill responce message
 *                  from CSC module.
 ****************************************************************************/
void process_s1ap_kill_resp(    
    void            *p_api,                
    s1ap_gb_context_t    *p_s1ap_gb_context)
{
  s1ap_kill_resp_t *p_s1ap_kill_resp = PNULL;
  U8 cell_index = MAX_NUM_CELLS;
  U8 mme_id = RRC_NULL;

  RRC_S1AP_UT_TRACE_ENTER();
  RRC_ASSERT(PNULL != p_api);
  RRC_ASSERT(PNULL != p_s1ap_gb_context);

  do {

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size((U8*)p_api) != RRC_API_HEADER_SIZE + 
        sizeof(s1ap_kill_resp_t))
    {
      RRC_S1AP_TRACE(RRC_WARNING, "Wrong message size - ignore.");
      RRC_S1AP_UT_TRACE_EXIT();
      return;
    }

    p_s1ap_kill_resp = (s1ap_kill_resp_t *)((U8 *)p_api + 
        RRC_API_HEADER_SIZE);

    /* Get the cell index from buffer */
    cell_index =     p_s1ap_kill_resp->cell_index;

    SET_CELL_INDEX(cell_index);

    if(MAX_NUM_CELLS <= cell_index)
    {
      RRC_S1AP_TRACE(RRC_WARNING, "[%s] Invalid cell index\n",__FUNCTION__);
      return;
    }   
    else if (PNULL == p_s1ap_gb_context->cell_s1ap_contexts[cell_index])
    {
      RRC_S1AP_TRACE(RRC_WARNING, "Cell context already exists at cell index "
          "%d",cell_index);
      return;
    }
    else
    {
            /* Received response from all the CSCs */

            /* call the appropriate fsm */
            enb_mgr_fsm_write_replace_warning_process_event(
                        ENB_EV_RRC_KILL_RESP,
                        p_api,
                        p_s1ap_kill_resp,
                        p_s1ap_gb_context,
                        p_s1ap_gb_context->mme_contexts[mme_id]);

    }

  }while (0);            

  RRC_S1AP_UT_TRACE_EXIT();
}


