/*******************************************************************************
*
*  FILE NAME   : x2ap_ue_asso_sig_sm.c
*
*  DESCRIPTION : This file contains the state machine and the state machine Entry
*                function definitionin for UE Associated Signalling Messages.
*
*  DATE 15-Sep-2011  NAME Hari Oum Sharan REFERENCE  36.423_960(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#include "x2ap_api.h"
#include "x2ap_evthdlr.h"
#include "x2ap_tracing.h"
#include "x2ap_ue_asso_sig_sm.h"
#include "rrc_x2ap_uecc_intf.h"
#include "rrc_msg_mgmt.h"
#include "x2ap_utils.h"
#include "rrc_intrl_api.h"
#include "rrc_common_utils.h"
#include "rrc_common_md.h"
#include "sm_entry.h"

X2AP_UE_ASSO_SIG_SM_EVENT(x2ap_ue_asso_sig_x2_link_up_msg_hdl);
X2AP_UE_ASSO_SIG_SM_EVENT(x2ap_ue_asso_sig_unexpected_msg);
X2AP_UE_ASSO_SIG_SM_EVENT(x2ap_ue_asso_sig_msg_ind_hdl);
X2AP_UE_ASSO_SIG_SM_EVENT(x2ap_ue_asso_sig_msg_req_hdl);
X2AP_UE_ASSO_SIG_SM_EVENT(x2ap_ue_asso_sig_x2_link_down_msg_hdl);

x2ap_return_et x2ap_ue_asso_sig_sm_map_event(U32 api,
                                     en_x2ap_ue_asso_sig_event *p_event);

/****************************************************************************
 * Function Name  : x2setup_sm
 * Inputs         : x2ap_ue_asso_sig_sm - an array 
 * Outputs        : x2ap_ue_asso_sig_sm
 * Returns        : None
 * Description    : This is Initialization of X2AP UE Associated 
 *                : Signalling SMc.
 ****************************************************************************/
static x2ap_ue_asso_sig_sm_action_routine 
          x2ap_ue_asso_sig_sm[X2AP_UE_ASSO_SIG_MAX_STATE][X2AP_UE_ASSO_SIG_MAX_EVENT] =
{
    /* X2AP_UE_ASSO_SIG_IDLE_ST */
    {
       x2ap_ue_asso_sig_x2_link_up_msg_hdl,  /* X2AP_UE_ASSO_SIG_X2_LINK_UP_EV */
       x2ap_ue_asso_sig_unexpected_msg,   /* X2AP_UE_ASSO_SIG_MSG_EV */
       x2ap_ue_asso_sig_unexpected_msg,   /* X2AP_UE_ASSO_SIG_MSG_REQ_EV */
       x2ap_ue_asso_sig_unexpected_msg    /* X2AP_UE_ASSO_SIG_X2_LINK_DOWN_EV */
    },
    /* X2AP_UE_ASSO_SIG_ACTIVE_ST */
    {
       x2ap_ue_asso_sig_unexpected_msg,   /* X2AP_UE_ASSO_SIG_X2_LINK_UP_EV*/
       x2ap_ue_asso_sig_msg_ind_hdl,      /* X2AP_UE_ASSO_SIG_MSG_IND_EV */
       x2ap_ue_asso_sig_msg_req_hdl,      /* X2AP_UE_ASSO_SIG_MSG_REQ_EV */
       x2ap_ue_asso_sig_x2_link_down_msg_hdl /* X2AP_UE_ASSO_SIG_X2_LINK_DOWN_EV*/
    },
};

/****************************************************************************
 * Function Name  : x2ap_ue_asso_sig_sm_entry
 * Inputs         : event         :    event_id
 *                  p_msg_ptr     : Pointer to message buffer
 *                  p_x2ap_gl_ctx : pointer to X2AP global context
 *                  p_enb_ctx     : Pointer er eNB context
 *                  proc_code     : procedure code
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to invoke the ue associated 
 *                  Signalling Message SM. 
 ****************************************************************************/
x2ap_return_et x2ap_ue_asso_sig_sm_entry (U32                api,
                                 U8                *p_msg_ptr,
                                 x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                 x2ap_peer_enb_context_t *p_enb_ctx,
                                 x2ap_ProcedureCode       proc_code)
{
    x2ap_return_et    retVal = X2AP_FAILURE;
    en_x2ap_ue_asso_sig_state  state = X2AP_UE_ASSO_SIG_MAX_STATE;
    en_x2ap_ue_asso_sig_event  event = X2AP_UE_ASSO_SIG_MAX_EVENT;
 
    X2AP_UT_TRACE_ENTER();
	 /* SPR -17891 CHANGES START */
    state = (en_x2ap_ue_asso_sig_state)p_enb_ctx->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state; 
    	/* SPR -17891 CHANGES END */
    retVal = x2ap_ue_asso_sig_sm_map_event(api,
                                   &event);
#if 0
#ifdef RRC_UNIT_TEST_FRAMEWORK
    /* MK Fix for Outgoing Messages. */
    if(X2AP_UE_ASSO_SIG_MSG_REQ_EV == event || 
            X2AP_UE_ASSO_SIG_MSG_IND_EV == event)
    {
        state = X2AP_UE_ASSO_SIG_ACTIVE_ST;
    }
#endif
#endif

    if (X2AP_SUCCESS == retVal)
    {                              

        if ( (state < X2AP_UE_ASSO_SIG_MAX_STATE) && 
              (event < X2AP_UE_ASSO_SIG_MAX_EVENT) )
        {
            X2AP_UE_ASSO_SIG_FSM_EVENT_TRACE(state,event);
            
            retVal = (x2ap_ue_asso_sig_sm [state][event])(p_msg_ptr,
                                                 p_x2ap_gl_ctx,
                                                 p_enb_ctx, proc_code); 

            X2AP_TRACE(X2AP_INFO, "### State:%s Event:%s Handled ###", 
                              X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[(state)], 
                              X2AP_UE_ASSO_SIG_FSM_EVENTS_NAMES[(event)])
        }
        else
        {
           X2AP_TRACE(X2AP_WARNING,"[%s]::Invalid Event :"
                      "unexpected state or event!",
                      __FUNCTION__);
        }
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}


/*****************************************************************************
 * Function Name  : x2ap_ue_asso_sig_x2_link_up_msg_hdl
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 *                  proc_code           : procedure code
 * Outputs        : None
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS
 * Description    : This function connect as client to peer eNB.
 *****************************************************************************/
x2ap_return_et  x2ap_ue_asso_sig_x2_link_up_msg_hdl(
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr,
        x2ap_ProcedureCode       proc_code /*X2AP_NULL*/)
{
    x2ap_return_et ret_val = X2AP_FAILURE;
    /* ARGUSED */
    X2AP_UT_TRACE_ENTER(); 
    X2AP_TRACE(X2AP_DETAILED,"Unused variable in fsm p_msg_ptr:%p proc_code:%d p_x2ap_gb_ctx:%p",
            p_msg_ptr,proc_code,p_x2ap_gb_ctx);
    
    p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.
      x2ap_ue_asso_sig_curr_state = X2AP_UE_ASSO_SIG_ACTIVE_ST;

    ret_val = X2AP_SUCCESS;

    X2AP_UT_TRACE_EXIT(); 
    return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_ue_asso_sig_x2_link_down_msg_hdl
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 *                  proc_code           : procedure code
 * Outputs        : None
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS
 * Description    : This function shall Invoke X2 Setup SM with connection
 *                  Down event.
 *****************************************************************************/
x2ap_return_et  x2ap_ue_asso_sig_x2_link_down_msg_hdl(
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr,
        x2ap_ProcedureCode       proc_code)
{
    x2ap_return_et ret_val = X2AP_FAILURE;
    X2AP_UT_TRACE_ENTER(); 
   
    X2AP_TRACE(X2AP_DETAILED,"Unused variable in fsm p_msg_ptr:%p proc_code:%d p_x2ap_gb_ctx:%p",
            p_msg_ptr,proc_code,p_x2ap_gb_ctx);
    /*Chnage UE Asso Sig state from Active to Idle*/   
    p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.
      x2ap_ue_asso_sig_curr_state = X2AP_UE_ASSO_SIG_IDLE_ST;
    ret_val = X2AP_SUCCESS;

    X2AP_UT_TRACE_EXIT(); 
    return ret_val;
}

/*******************************************************************************
 * Function Name : x2ap_ue_asso_sig_unexpected_msg
 * Inputs        : p_msg_ptr            : pointer to msg 
 *                 p_x2ap_gl_ctx        : pointer to X2AP global context
 *                 p_enb_ctx            : pointer to peer eNB context
 *                 proc_code            : procedure code 
 * Outputs       : None
 * REturns       : X2AP_SUCCESS or X2AP_FAILURE
 * Description   : This function used to handle unexpected event received by SM.
*******************************************************************************/
x2ap_return_et x2ap_ue_asso_sig_unexpected_msg (U8             *p_msg_ptr,
                                    x2ap_gb_context_t         *p_x2ap_gl_ctx,
                                    x2ap_peer_enb_context_t   *p_enb_ctx,
                                    x2ap_ProcedureCode       proc_code /* X2AP_NULL */)
{
    x2ap_return_et retVal = X2AP_FAILURE;
    X2AP_UT_TRACE_ENTER();
    X2AP_TRACE(X2AP_DETAILED,"Unused variable in fsm p_msg_ptr:%p proc_code:%x p_x2ap_gb_ctx:%p p_enb_ctx:%p",
            p_msg_ptr,proc_code,p_x2ap_gl_ctx,p_enb_ctx);

    X2AP_TRACE(X2AP_WARNING,"[%s]::Invalid Event : "
                    "unexpected event received!",
                   __FUNCTION__);
    retVal = X2AP_SUCCESS;

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_ue_asso_sig_msg_ind_hdl
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 *                  proc_code           : procedure code
 * Outputs        : None
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS
 * Description    : This function used to handle UE Associated Signalling
 *                  Message Event received by SM..
 *****************************************************************************/
x2ap_return_et  x2ap_ue_asso_sig_msg_ind_hdl(
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr,
        x2ap_ProcedureCode       proc_code)
{
    void *p_intrl_msg            = PNULL;
    x2ap_ue_associated_sig_msg_ind_t    *msg_p = PNULL;
    x2ap_peer_enb_msg_t  *p_asn_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;
    rrc_module_id_t       uecc_module_id;
    /* SPR_14436_Fix Start */
    x2ap_ho_ongoing_ue_ctx_node_t * p_ho_ongoing_ue_ctx_node = PNULL; 
    /* SPR_14436_Fix Stop */

    X2AP_UT_TRACE_ENTER(); 

    X2AP_ASSERT(PNULL != p_x2ap_gb_ctx);
    X2AP_ASSERT(PNULL != p_enb_cntxt_arr);

    p_intrl_msg = x2ap_alloc_intrl_msg(
            RRC_UECC_MODULE_ID,
            X2AP_UE_ASSOCIATED_SIG_MSG_IND,
            (U16)(sizeof(x2ap_ue_associated_sig_msg_ind_t) 
                + p_asn_msg->msg_len));

    if (PNULL == p_intrl_msg)
    {
        /*out of memory*/
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    msg_p = (x2ap_ue_associated_sig_msg_ind_t *)((U8 *)p_intrl_msg +
            RRC_API_HEADER_SIZE);

    msg_p->peer_enodeb_id = p_enb_cntxt_arr->peer_enodeb_id; 
    msg_p->x2ap_procedure_code = proc_code; 

    X2AP_MEMCPY((void*)((U8*)p_intrl_msg + RRC_API_HEADER_SIZE +
                sizeof(x2ap_ue_associated_sig_msg_ind_t)),
            (const void*)p_asn_msg->p_msg,
            p_asn_msg->msg_len);

    X2AP_TRACE(X2AP_INFO, "[%s]: Forwarding message to UECC\n",
            __FUNCTION__);

    if (((proc_code == ASN1V_x2ap_id_handoverPreparation) || 
                (proc_code == ASN1V_x2ap_id_handoverCancel)) &&
            (p_asn_msg->newEnbUeX2apId == INVALID_UE_X2AP_ID)
       )
    {
        /* SPR_14436_Fix Start */
        if (proc_code == ASN1V_x2ap_id_handoverPreparation)
        {
            /* Create a ho_ongoing_ue_ctx */
            p_ho_ongoing_ue_ctx_node = (x2ap_ho_ongoing_ue_ctx_node_t *)
                rrc_mem_get(sizeof(x2ap_ho_ongoing_ue_ctx_node_t));

            if(PNULL == p_ho_ongoing_ue_ctx_node)
            {    
                X2AP_TRACE(X2AP_FATAL, "rrc_mem_get failed!!");
                /* Coverity Fix 48269 Start */
                x2ap_msg_mem_free(p_intrl_msg);
                /* Coverity Fix 48269 End */
                return X2AP_FAILURE;
            }    

            memset_wrapper(p_ho_ongoing_ue_ctx_node,0,sizeof(x2ap_ho_ongoing_ue_ctx_node_t));

            /* Set old_enb_ue_x2ap_id */
            p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.old_enb_ue_x2ap_id = p_asn_msg->oldEnbUeX2apId;

            /* Set stream_id*/
            p_ho_ongoing_ue_ctx_node->ho_ongoing_ue_ctx.stream_id = p_asn_msg->stream_id;

            list_push_head(&p_x2ap_gb_ctx->ho_ongoing_ue_ctx_list,
                    &p_ho_ongoing_ue_ctx_node->node);
        }
        /* SPR_14436_Fix Stop */

        /* Send Message to UECC MD*/
        rrc_send_message(
                p_intrl_msg,
                RRC_UECCMD_MODULE_ID);
    }
    else
    {
        /* SPR 18072 Fix Start */
        if(p_x2ap_gb_ctx->no_of_uecc_instances > 1)
        { /* SPR 18072 Fix Stop */
            /*SPR 21162 Start*/
            switch(proc_code)
            {
                case ASN1V_x2ap_id_handoverPreparation:
 
                case ASN1V_x2ap_id_uEContextRelease:
                    {
                        uecc_module_id  = RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                                p_asn_msg->oldEnbUeX2apId);
                    }
                    break;
               
                case ASN1V_x2ap_id_snStatusTransfer:
                
                case ASN1V_x2ap_id_handoverCancel:
                   {
                       uecc_module_id  = RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                            p_asn_msg->newEnbUeX2apId);
                   }
                    break;
#ifdef ENDC_ENABLED
                /*NR_DC Code Change Start*/
                case ASN1V_x2ap_id_sgNBAdditionPreparation:
                case ASN1V_x2ap_id_meNBinitiatedSgNBRelease:
                case ASN1V_x2ap_id_sgNBinitiatedSgNBRelease:
                case ASN1V_x2ap_id_secondaryRATDataUsageReport:
                    {
                        uecc_module_id  = RRC_GET_UECC_MODULE_INDEX_FROM_X2AP(
                               p_asn_msg->mEnbUeX2apId);
                    }
                    break;
                /*NR_DC Code Change Stop*/
#endif

                default :
                    X2AP_TRACE(X2AP_INFO, "[%s]: Neither newEnbUeX2apId nor oldEnbUeX2apId present"
                            "in deceod message\n", __FUNCTION__);
                    X2AP_UT_TRACE_EXIT();
                    /* coverity_fix_48269_start */
                    x2ap_msg_mem_free(p_intrl_msg);
                    /* coverity_fix_48269_stop */
                    return X2AP_FAILURE;
            }

            /*SPR 21162 Stop*/
        
        if ((uecc_module_id - RRC_UECC_1_MODULE_ID) < 
                p_x2ap_gb_ctx->no_of_uecc_instances)
        {
            /* Send Message to UECC*/
            rrc_send_message(
                    p_intrl_msg,
                    uecc_module_id);
        }
        else
        {
            X2AP_TRACE(X2AP_INFO, "[%s]: Invalid UECC instance Id .. igonore.\n",
                    __FUNCTION__);
            /* coverity_fix_48269_start */
            x2ap_msg_mem_free(p_intrl_msg);
            /* coverity_fix_48269_stop */
        }
        /* SPR 18072 Fix Start */
    }
    else
    {
        /* Send Message to UECC*/
        rrc_send_message(
                p_intrl_msg,
                RRC_UECC_1_MODULE_ID);

    }/* SPR 18072 Fix Stop */
     }
     X2AP_UT_TRACE_EXIT();
     return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_ue_asso_sig_msg_req_hdl
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 *                  proc_code           : procedure code
 * Outputs        : None
 * Returns        : X2AP_FAILURE or X2AP_SUCCESS
 * Description    : This function used to handle UE Associated Signalling
 *                  Message Event received by SM.
 *                  This function shall send message to peer ENB Via SCTP.
 *****************************************************************************/
x2ap_return_et  x2ap_ue_asso_sig_msg_req_hdl(
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr,
        x2ap_ProcedureCode       proc_code)
{
    x2ap_return_et retVal = X2AP_FAILURE;
    X2AP_UT_TRACE_ENTER();

    X2AP_TRACE(X2AP_DETAILED,"Unused variable in fsm :%d ",proc_code);


    retVal = x2ap_sctp_sm_entry (X2AP_SCTP_SEND_DATA_EV,
                           p_msg_ptr,
                           p_x2ap_gb_ctx,
                           p_enb_cntxt_arr);
    X2AP_UT_TRACE_EXIT();
    return retVal;

}


/****************************************************************************
 * Function Name  : x2ap_ue_asso_sig_sm_map_event
 * Inputs         : api          :  api ID
 *                  p_event      : event ID
 * Outputs        : p_event        event ID
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to map api ID with SM event IDs
 ****************************************************************************/
x2ap_return_et x2ap_ue_asso_sig_sm_map_event(U32 api,
                                     en_x2ap_ue_asso_sig_event *p_event)
{
    x2ap_return_et    retVal = X2AP_SUCCESS;
    switch (api)
    {
        case X2AP_UE_ASSO_SIG_X2_LINK_UP_EV:
            *p_event = X2AP_UE_ASSO_SIG_X2_LINK_UP_EV;
            break;
        case X2AP_UE_ASSO_SIG_MSG_IND_EV:
            *p_event = X2AP_UE_ASSO_SIG_MSG_IND_EV;
            break;
        case X2AP_UE_ASSO_SIG_MSG_REQ_EV:
            *p_event = X2AP_UE_ASSO_SIG_MSG_REQ_EV;
            break;
        case X2AP_UE_ASSO_SIG_X2_LINK_DOWN_EV:
            *p_event = X2AP_UE_ASSO_SIG_X2_LINK_DOWN_EV;
            break;
        default :
           retVal = X2AP_FAILURE;
           X2AP_TRACE(X2AP_INFO, "Wrong API ID received %d \n", api); 
           break;
    }
    return retVal;
}
