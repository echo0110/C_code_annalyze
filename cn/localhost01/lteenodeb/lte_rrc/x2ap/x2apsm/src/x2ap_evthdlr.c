/*******************************************************************************
*
*  FILE NAME   : x2ap_evthdlr.c 
*
*  DESCRIPTION : This file contains the functional definition of event handler 
*                of diffrent events.
*
*  DATE 24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/

#include "commoninc.h"
#include "x2ap_evthdlr.h"
#include "x2ap_db.h"
#include "x2ap_tracing.h"
#include "x2ap_timer.h"
#include "x2ap_utils.h"
#include "sm_entry.h"
#include "x2ap_uecc.h"
#include "asn_enc_wrappers.h"
#include "asn_dec_wrappers.h"
#include "rrc_x2apCsc_intf.h"
#include "rrc_x2apOam_intf.h"
#include "rrc_x2apRrm_intf.h"
#include "x2ap_api.h"
#include "x2ap_msg_framework.h"
#include "x2ap_init.h"
#include "rrc_x2apRrm_il_composer.h"
#include "rrc_x2apOam_il_composer.h"
#include "rrc_x2apCommon_il_composer.h"
#include "x2ap_ue_asso_sig_sm.h"
#include "rrc_x2ap_uecc_intf.h"
#include "x2ap_rsu_sm.h"

#include "rrc_x2apEnb_intf.h"
#include "x2ap_error_indication.h"
#include "rrc_logging.h"
/*SPR_16533_START*/
#ifdef LKSCTP
#include "sctp_kernel.h"
#endif
/*SPR_16533_END*/

/******************************************************************************
 * FUNCTION PROTOTYPE
 ******************************************************************************/

/******************************************************************************
 * Private Definitions
 ******************************************************************************/
#define X2_SETUP_REQ_GUARD_TIMER_DURATION       10000        
#define X2AP_SETUP_FAIL_MISC_CAUSE_TYPE         4
#define X2AP_MISC_CAUSE                         4

/* GDB Fix - compiler warning */
extern void x2ap_compose_cell_id_from_ecgi(x2_gb_enb_id_t global_enb_id, U8* eutran_cell_id, U8* p_cell_id);
/* SPR22925 Start */
extern S32 inet_pton_wrapper(S32 af, const char *src, void *dst);
extern U32 htonl_wrapper(uint32_t hostlong);
/* SPR22925 End */

x2ap_return_et x2ap_send_link_up_ind_to_son(
        x2_setup_resp_t  *p_x2_setup_resp,
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        /*SPR 17777 +-*/
	x2_enb_comm_info_t* p_enb_comm_info);

#ifdef ENDC_ENABLED
x2ap_return_et
x2ap_build_endc_x2setup_req
(
    en_dc_x2_setup_req_t  *p_x2_setup_req,
    U16                   num_act_cell,
    U16                   *p_act_cell_id,
    x2ap_gb_context_t     *p_x2ap_gl_ctx
);
#endif

x2ap_return_et x2ap_build_x2setup_req(x2_setup_req_t    *p_x2_setup_req,
                                      U16                 num_act_cell,
                                      U16                *p_act_cell_id,
                                      x2ap_gb_context_t *p_x2ap_gl_ctx);

x2ap_return_et x2ap_build_x2setup_fail_resp(x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                           x2ap_peer_enb_context_t *p_enb_ctx,
                                           x2ap_bool_et    timeTowait_present,
                                rrc_x2ap_error_indication_t *p_error_indication,
                                x2ap_error_ind_bool_t *p_send_err_ind );

x2ap_return_et x2ap_build_x2setup_resp(U16                 num_act_cell,
                                       U16                *p_act_cell_id,
                                       x2ap_gb_context_t *p_x2ap_gl_ctx,
                                       x2ap_peer_enb_context_t *p_enb_ctx);

void x2ap_update_cell_info_for_req(x2_setup_req_t      *p_x2_setup_req,
                                   U16                   num_act_cell,
                                   U16                  *p_act_cell_index,
                                   x2ap_gb_context_t   *p_x2ap_gl_ctx);

void x2ap_update_cell_info_for_resp(x2_setup_resp_t     *p_x2_setup_resp,
                                    U16                   num_act_cell,
                                    U16                  *p_act_cell_index,
                                    x2ap_gb_context_t   *p_x2ap_gl_ctx);

x2ap_return_et x2ap_send_link_down_ind_to_oam_rrm(
                                        x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_send_link_down_ind_to_son(
                            x2ap_gb_context_t   *p_x2ap_gl_ctx,
                            x2ap_peer_enb_context_t *p_enb_ctx);

void x2ap_init_cell_info(x2ap_gb_context_t       *p_x2ap_gl_ctx,
                         x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_send_link_up_ind_to_rrm (x2_setup_resp_t
        *p_x2_setup_resp);/*SPR 17777 +-*/

void x2ap_build_x2_link_up_ind(x2_setup_resp_t           *p_x2_setup_resp,
                               x2ap_rrm_x2_link_up_ind_st *p_rrm_link_up_ind);/*SPR 17777 +-*/

void x2ap_build_x2_enb_link_up_ind(x2_setup_resp_t           *p_x2_setup_resp,
                               x2ap_enb_link_up_ind_t *p_link_up_ind,
                               /*SPR 17777 +-*/
                               x2ap_gb_context_t         *p_x2ap_gl_ctx);

/******************************************************************************
 * Function Name  : x2_setup_resp_evt_handler
 * Inputs         : p_msg_ptr  -    Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when x2 setup response event is
 *                  received from peer eNB.
 ******************************************************************************/
x2ap_return_et
x2_setup_resp_evt_handler
(
    U8                 *p_msg_ptr,
    x2ap_gb_context_t  *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t *p_enb_ctx
);

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
x2ap_return_et
x2ap_send_link_up_ind_to_rrm_endc
(
    x2ap_gb_context_t       *p_x2ap_gl_ctx,
    en_dc_x2_setup_req_t    *p_x2_setup_resp
);

x2ap_return_et
x2ap_build_endc_x2setup_fail_resp
(
    x2ap_gb_context_t              *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t        *p_enb_ctx,
    x2ap_bool_et                   timeTowait_present,
    rrc_x2ap_error_indication_t    *p_error_indication,
    x2ap_error_ind_bool_t          *p_send_err_ind
);

void
x2ap_build_x2_link_up_ind_endc
(
    en_dc_x2_setup_resp_t           *p_x2_setup_resp,
    x2ap_rrm_x2_link_up_ind_st     *p_rrm_link_up_ind
);

void
x2ap_update_nr_cell_info_for_req
(
    en_dc_x2_setup_req_t      *p_x2_setup_req,
    U16                       num_act_cell,
    U16                       *p_act_cell_index,
    x2ap_gb_context_t         *p_x2ap_gl_ctx
);

/******************************************************************************
 * Function Name  : en_dc_x2_setup_resp_evt_handler
 * Inputs         : p_msg_ptr  -    Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when x2 setup response event is
 *                  received from peer eNB.
 ******************************************************************************/
x2ap_return_et
en_dc_x2_setup_resp_evt_handler
(
    U8                 *p_msg_ptr,
    x2ap_gb_context_t  *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t *p_enb_ctx
);

void
x2ap_build_nr_x2_link_up_ind
(
    x2ap_gb_context_t              *p_x2ap_gl_ctx,
    en_dc_x2_setup_req_t           *p_x2_setup_resp,
    x2ap_rrm_x2_link_up_ind_st      *p_rrm_link_up_ind
);

void
x2ap_update_nr_cell_info_for_resp
(
    en_dc_x2_setup_resp_t     *p_x2_setup_resp,
    U16                       num_act_cell,
    U16                       *p_act_cell_index,
    x2ap_gb_context_t         *p_x2ap_gl_ctx
);

x2ap_return_et
x2ap_build_endc_x2setup_resp
(
    U16                        num_act_cell,
    U16                        *p_act_cell_id,
    x2ap_gb_context_t          *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t    *p_enb_ctx
);

void
x2ap_served_nr_cell_info_update
(
    x2ap_gb_context_t       *p_x2ap_gl_ctx,
    U8                      status,
    U8                      peer_enodeb_id,
    en_dc_x2_setup_req_t    *p_x2setup_req
);
/* MENB CHANGES - END */
#endif

/*RSU Changes Start*/
void x2ap_served_cell_info_update(x2ap_gb_context_t *p_x2ap_gl_ctx,
                                  U8 status,
                                  U8 peer_enodeb_id,
                                  x2_setup_req_t *p_x2setup_req);

/*RSU Changes End */

#ifdef LINUX_PC_TEST
//for LKSCTP
extern S32 pctest_forward_x2ap_lksctp_message_to_pc
(
    U16     src_module_id,  /* Source module Id */
    U16     dst_module_id,  /* Destination module Id */
    U16     api_id,         /* API Id /msg id*/
    U32 socketId,
    U16 streamId,
    struct sockaddr *pstPeerAddr,    /*already converted to net byteorder by caller*/
    const void * pmsg,
    U32 msglen
);
#endif

/*****************************************************************************
 * Function Name  : x2ap_active_procedure_sm_ctx_state 
 * Inputs         : p_enb_ctx - peer eNB context
 * Outputs        : None
 * Returns        : None
 * Description    : This function used to set procedure sm context state to ACTIVE. 
 ********************************************************************************/
 void x2ap_active_procedure_sm_ctx_state(x2ap_peer_enb_context_t   *p_enb_ctx)
 {
    X2AP_UT_TRACE_ENTER();
    
    p_enb_ctx->enb_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
    p_enb_ctx->endc_config_sm_ctx.state = X2AP_ENB_CONFIG_ACTIVE_ST;
    p_enb_ctx->x2ap_reset_sm_ctx.state = X2_RESET_ACTIVE_ST;
    p_enb_ctx->x2ap_mobility_ch_sm_ctx.state = X2_MOBILITY_CHANGE_ACTIVE_ST;
    p_enb_ctx->x2ap_cell_act_sm_ctx.state = X2_CELL_ACT_ACTIVE_ST;

    X2AP_UT_TRACE_EXIT();
 }

/*****************************************************************************
 * Function Name  : x2ap_unexpected_msg 
 * Inputs         : p_msg_ptr - pointer to msg
 *                  p_x2ap_gl_ctx - pointer to X2AP global context
 *                  p_enb_ctx - peer eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function used to handle unexpected event received by SM. 
 ********************************************************************************/
x2ap_return_et x2ap_unexpected_msg (U8                         *p_msg_ptr,
                                    x2ap_gb_context_t         *p_x2ap_gl_ctx,
                                    x2ap_peer_enb_context_t   *p_enb_ctx)
{
    x2ap_return_et retVal = X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();
    X2AP_TRACE(X2AP_DETAILED,"Unused variable p_msg_ptr:%p p_x2ap_gl_ctx:%p p_enb_ctx:%p",
            p_msg_ptr,p_x2ap_gl_ctx,p_enb_ctx);

    X2AP_TRACE(X2AP_WARNING,"[%s]::Invalid Event :"
                    "unexpected event received!",
                   __FUNCTION__);
    retVal = X2AP_SUCCESS;

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : cell_setup_ind_tmr_exp_hdl
 * Inputs         : p_msg_ptr   -   Pointer to message buffer
 *                : p_x2ap_gl_ctx - pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when cell setup indication timer is 
 *                  being expired.
 ******************************************************************************/
x2ap_return_et cell_setup_ind_tmr_exp_hdl (U8                  *p_msg_ptr,
                                           x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                           x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et        retVal = X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();
    /*SPR_17628_START*/
#ifdef LKSCTP
    U16                   num_act_cell = 0;
    U16                   act_cell_id[MAX_NUM_CELL_IN_X2AP]={0};
        /* Check if there is any cell is operational or not */
        x2ap_check_active_cell(&num_act_cell,
                               act_cell_id,
                               p_x2ap_gl_ctx);

        /* if atleast one cell is active */
        if (num_act_cell > 0)
        {
            retVal = x2ap_sctp_sm_entry(
                    X2AP_SCTP_CONNECT_EV,
                    (U8*)p_x2ap_gl_ctx,
                    p_x2ap_gl_ctx,
                    p_enb_ctx);

        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,"No Cell is Active, Not invoking SCTP FSM");
        }
//#elif ARICENT_SCTP_IPR
#if 0
    /* Build and send the x2 setup request for peer eNB */
    retVal = x2setup_request_msg_init (p_msg_ptr,
                                       p_x2ap_gl_ctx,
                                       p_enb_ctx);
#endif	
#endif
    /*SPR_17628_END*/
    X2AP_UT_TRACE_EXIT();
    return retVal;
}


/******************************************************************************
 * Function Name  : csc_cell_setup_ind_hdl
 * Inputs         : p_msg_ptr    -  Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx     - Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when cell setup is UP & operational
 *                  received from CSC.This function is invoked per ENB context.
 ******************************************************************************/
x2ap_return_et csc_cell_setup_ind_hdl (U8                  *p_msg_ptr,
                                       x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                       x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    U8                    cell_index = 0;
    x2ap_csc_cell_add_t  *p_csc_cell_ind = X2AP_P_NULL;

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);
    
    p_csc_cell_ind =(x2ap_csc_cell_add_t*)p_msg_ptr;
    
    if(X2AP_FAILURE == x2ap_get_cell_context_by_cell_id(
                    p_x2ap_gl_ctx,p_csc_cell_ind->cell_index,
                    &cell_index))
    {
        return retVal;
    }
        

    if (X2AP_TRUE == p_x2ap_gl_ctx->act_cell_info[cell_index].is_cell_active)
    {
        /* Mark the cell as active */
        p_x2ap_gl_ctx->act_cell_info[cell_index].is_cell_active = X2AP_TRUE;
        /* Check the Guard timer expired or not */
        if (!(p_x2ap_gl_ctx->cell_up_guard_tim_id))
        {
            /*SPR_17448_START*/
#ifdef LKSCTP
            /* Build and send the x2 setup request for peer eNB */
            x2ap_sctp_sm_entry(
                    X2AP_SCTP_CONNECT_EV,
                    /*coverity_fix*/
                    (U8*)p_x2ap_gl_ctx,
                    /*coverity_fix*/
                    p_x2ap_gl_ctx,
                    p_enb_ctx);
#else
            retVal = x2setup_request_msg_init (p_msg_ptr,
                                               p_x2ap_gl_ctx,
                                               p_enb_ctx); 
#endif	    
            /*SPR_17448_END*/
        }
        else
        { 
            X2AP_TRACE(X2AP_INFO,"Wait for cell_up_guard_timer"
                       " Expiry & Update cell");
            retVal = X2AP_SUCCESS;
        }
    }
    else
    {
        X2AP_TRACE(X2AP_INFO,"Cell is not marked as active");
        retVal = X2AP_SUCCESS;
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : x2setup_guard_timer_exp_hdl
 * Inputs         : p_msg_ptr  -    Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when guard timer for x2 setup
 *                  is being expired.
 ******************************************************************************/
x2ap_return_et x2setup_guard_timer_exp_hdl (U8                 *p_msg_ptr,
                                       x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                       x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et                  retVal = X2AP_FAILURE;
    x2_gb_enb_id_t                  gb_enb_id; 

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    x2_gb_gnb_id_t                  gb_gnb_id; 
    X2AP_MEMSET (&gb_gnb_id, X2AP_NULL, sizeof(x2_gb_gnb_id_t));
    /* MENB CHANGES - END */
    #endif

    X2AP_UT_TRACE_ENTER();

    X2AP_MEMSET (&gb_enb_id, X2AP_NULL, sizeof(x2_gb_enb_id_t));
    /* Inavlidate x2 Setup Timer Id Since Timer Expired */
    p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;
    /* Adding 1 for the first attempt i.e. "try" count, other count is "retry" count*/
    if ((p_x2ap_gl_ctx->config_param.timer_conf_info.x2ap_setup_retry_cnt + 1) == 
                                p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count)
    {
        #ifdef ENDC_ENABLED
        /* MENB CHANGES - START */
        if ( X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT &
            p_enb_ctx->bitmask )
        {
            gb_gnb_id = p_enb_ctx->gb_gnb_id;

            /* Send X2AP_SCTP_CONN_CLOSE_EV event to SCTP SM */
            retVal = x2ap_sctp_sm_entry(X2AP_SCTP_CONN_CLOSE_EV,
                    (U8 *)&gb_gnb_id,
                    p_x2ap_gl_ctx,
                    p_enb_ctx );
        }
        else
        {
        /* MENB CHANGES - END */
        #endif
        gb_enb_id = p_enb_ctx->gb_enb_id;

        /* Send X2AP_SCTP_CONN_CLOSE_EV event to SCTP SM */
        retVal = x2ap_sctp_sm_entry(X2AP_SCTP_CONN_CLOSE_EV,
                (U8 *)&gb_enb_id,
                p_x2ap_gl_ctx,
                p_enb_ctx );
        #ifdef ENDC_ENABLED
        /* MENB CHANGES - START */
        }
        /* MENB CHANGES - END */
        #endif

        /* Change state to IDLE */
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_IDLE_ST;
        p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = 0;
        p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
    }
    else
    {
        X2AP_TRACE(X2AP_INFO,"Resend x2Setup request to peer eNB");
        /* Build and send the x2 setup request for peer eNB */
        retVal = x2setup_request_msg_init (p_msg_ptr,
                                           p_x2ap_gl_ctx,
                                           p_enb_ctx);
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : csc_ind_in_x2Setup_hdl
 * Inputs         : p_msg_ptr  -    Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when cell is up during x2 setup 
 *                  on going from CSC.This function is invoked per ENB context.
 ******************************************************************************/
x2ap_return_et csc_ind_in_x2Setup_hdl (U8                 *p_msg_ptr,
                                       x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                       x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et                  retVal = X2AP_FAILURE;
    U8                              cell_index = 0;
    x2ap_csc_cell_add_t            *p_csc_cell_ind = X2AP_P_NULL;
    X2AP_UT_TRACE_ENTER();

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_TRACE(X2AP_DETAILED,"Unused variable in FSM:%p",p_enb_ctx);
    p_csc_cell_ind = (x2ap_csc_cell_add_t*)p_msg_ptr;
    cell_index = (U8)(p_csc_cell_ind->cell_index);

    if (X2AP_TRUE != p_x2ap_gl_ctx->act_cell_info[cell_index].is_cell_active)
    {
        /* Mark the cell as active */
        p_x2ap_gl_ctx->act_cell_info[cell_index].is_cell_active = X2AP_TRUE;
    }
    else
    {
        X2AP_TRACE(X2AP_INFO,"Cell is already marked as active");
        retVal = X2AP_SUCCESS;
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
/******************************************************************************
 * Function Name  : x2ap_update_nr_cell_info_for_resp
 * Inputs         : p_x2ap_gl_ctx -   pointer to X2AP global context
 *                : p_x2setup_resp -  Pointer of x2 setup request
 *                : num_act_cell  -   Number of active cell
 *                : p_act_cell_index - Pointer to served cell index
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to update cell info in global db 
 *                  for received response from peer gNB.
 ******************************************************************************/
void
x2ap_update_nr_cell_info_for_resp
(
    en_dc_x2_setup_resp_t   *p_x2setup_resp,
    U16                     num_act_cell,
    U16                     *p_act_cell_index,
    x2ap_gb_context_t       *p_x2ap_gl_ctx
)
{
    /* TB_FIX_START */    
    S32    peer_cell_count = X2AP_NULL;
    U16    cell_count      = X2AP_NULL;

    X2AP_UT_TRACE_ENTER();

    /*
     * Received pointer should not be NULL
     */
    X2AP_ASSERT(p_x2setup_resp != X2AP_P_NULL);
    X2AP_ASSERT(p_act_cell_index != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_TRACE(X2AP_DETAILED,"Servd cell count in endc x2 setup setup res is [%d]\n",
            p_x2setup_resp->nr_cell_info.num_served_nr_cell);

    X2AP_TRACE(X2AP_DETAILED,"peer_cell_count is [%d] , start filling at this index in global context \n",
            peer_cell_count);

    for (cell_count = 0; cell_count < p_x2setup_resp->nr_cell_info.num_served_nr_cell
            && cell_count < MAX_SERVED_CELLS;
            cell_count++ )
    {
        peer_cell_count = x2ap_get_nbr_index_from_nr_cgi(p_x2ap_gl_ctx, 
                p_x2setup_resp->nr_cell_info.served_nr_cell_arr[cell_count].nr_cell_id,
                X2AP_TRUE, 0);

        if (X2AP_NULL > peer_cell_count)
        {
            X2AP_TRACE(X2AP_ERROR,"nr_cgi error from endc x2setup response. \n");
            break;
        }
        
        X2AP_TRACE(X2AP_DETAILED,"num_peer_enb_cells in global context is [%d]\n",
                p_x2ap_gl_ctx->num_peer_enb_cells);
                
        /* coverity_<280095>_fix_start */
        p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].current_cell_info = RRC_FALSE;
        /* coverity_<280095>_fix_end */
        /*
         * Global gNB ID
         */
        p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
            peer_gb_gnb_id = p_x2setup_resp->nr_cell_info.global_gnb_id;

        /*
         * NR pci
         */
        p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
            peer_cell_info.nr_pci = p_x2setup_resp->nr_cell_info.served_nr_cell_arr
            [cell_count].nr_pci;

        /*
         * NR ecgi
         */
        p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
            peer_cell_info.nr_ecgi =
            p_x2setup_resp->nr_cell_info.served_nr_cell_arr[cell_count].
            nr_cell_id;

        /*
         * Five GS TAC
         */
        if(X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT &
                p_x2setup_resp->nr_cell_info.served_nr_cell_arr[cell_count].bitmask)
        {
            X2AP_TRACE(X2AP_DETAILED,"x2 reps X2AP_NR_CELL_INFO_5gs_TAC_PRESENT peer_cell_count %d",peer_cell_count);
            p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
                peer_cell_info.bitmask |=
                NR_CELL_INFO_FIVE_GS_TAC_PRESENT;

            X2AP_MEMCPY(&p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
                peer_cell_info.five_gs_tac,
                &p_x2setup_resp->nr_cell_info.served_nr_cell_arr[cell_count].
                fiveGS_track_area_code,sizeof(x2ap_five_gs_tac_t));
        }

        /*
         * Configured TAC
         */
        if(X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT &
                p_x2setup_resp->nr_cell_info.served_nr_cell_arr[cell_count].bitmask)
        {
            X2AP_TRACE(X2AP_DETAILED,"x2 reps X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT peer_cell_count %d",peer_cell_count);

            p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
                peer_cell_info.bitmask |=
                NR_CELL_INFO_CONFIGURED_TAC_PRESENT;

            X2AP_MEMCPY(&p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
                peer_cell_info.configured_tac,
                &p_x2setup_resp->nr_cell_info.served_nr_cell_arr[cell_count].
                configured_tac,
                sizeof(x2ap_tac_t));
        }

        /*
         * Brodcast PLMN
         */
        p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
            peer_cell_info.broadcast_plmn_info =
            p_x2setup_resp->nr_cell_info.served_nr_cell_arr[cell_count].
            broadcast_plmn_info;

        /*
         * NR Mode
         */
        X2AP_MEMCPY(&(p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
                    peer_cell_info.choice_nr_mode),&(p_x2setup_resp->
                        nr_cell_info.served_nr_cell_arr[cell_count].
                        choice_nr_mode),sizeof(x2ap_choice_nr_mode_info_t));
        X2AP_TRACE(X2AP_DETAILED,"In endc x2setup resp, choice mode bitmask is [%d] \n",
                p_x2setup_resp->nr_cell_info.served_nr_cell_arr[cell_count].
                choice_nr_mode.bitmask);
        if( p_x2setup_resp->nr_cell_info.served_nr_cell_arr[cell_count].
                choice_nr_mode.bitmask & X2AP_NR_MODE_FDD_PRESENT)
        {
            X2AP_MEMCPY(&(p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count]
                        .peer_cell_info.nr_freq_info),&(p_x2setup_resp->
                            nr_cell_info.served_nr_cell_arr[cell_count].
                            choice_nr_mode.nr_fdd_info.nr_ul_freq_info),sizeof(x2ap_nr_freq_info_t));
        }
        else if(p_x2setup_resp->nr_cell_info.served_nr_cell_arr[cell_count].
                choice_nr_mode.bitmask & X2AP_NR_MODE_TDD_PRESENT)
        {
            X2AP_MEMCPY(&(p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count]
                        .peer_cell_info.nr_freq_info),&(p_x2setup_resp->
                            nr_cell_info.served_nr_cell_arr[cell_count].
                            choice_nr_mode.nr_tdd_info.nr_freq_info),sizeof(x2ap_nr_freq_info_t));
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,"Invalid mode, bitmask set in setup req is [%d] \n", p_x2setup_resp->nr_cell_info.served_nr_cell_arr[cell_count].choice_nr_mode.bitmask);
        }

#ifndef LINUX_PC_TEST
        /*
         * Measurement Timing Config
         */
        x2ap_build_x2ap_meas_timing_config(&p_x2setup_resp->nr_cell_info.served_nr_cell_arr[cell_count].meas_timing_config,
            &p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].peer_cell_info.x2ap_meas_timing_config[0],
            &p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].peer_cell_info.meas_timing_config_count);
#endif
    }
    /* TB_FIX_END */    
    X2AP_UT_TRACE_EXIT();
}
/* MENB CHANGES - END */
#endif

/******************************************************************************
 * Function Name  : x2ap_update_cell_info_for_resp
 * Inputs         : p_x2ap_gl_ctx -   pointer to X2AP global context
 *                : p_x2setup_resp -  Pointer of x2 setup request
 *                : num_act_cell  -   Number of active cell
 *                : p_act_cell_index - Pointer to served cell index
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to update cell info in global db 
 *                  for received response from peer eNB.
 ******************************************************************************/
void x2ap_update_cell_info_for_resp(x2_setup_resp_t   *p_x2setup_resp,
                                    U16                 num_act_cell,
                                    U16                *p_act_cell_index,
                                    x2ap_gb_context_t *p_x2ap_gl_ctx)
{
    U16                               cell_index = 0;
    U32                               pci = 0;
    x2ap_ecgi_t                      ecgi;
    U32                               ngh_pci = 0;
    x2ap_ecgi_t                      ngh_ecgi;
    U16                               peer_cell_count = 0;
    U16                               peer_ngh_count = 0;
    U16                               ngh_cell_count = 0;
    U16                               act_cell_count = 0;

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_x2setup_resp != X2AP_P_NULL);
    X2AP_ASSERT(p_act_cell_index != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);

    X2AP_MEMSET (&ecgi, X2AP_NULL, sizeof(x2ap_ecgi_t));
    X2AP_MEMSET (&ngh_ecgi, X2AP_NULL, sizeof(x2ap_ecgi_t));

    for (act_cell_count = 0; act_cell_count < num_act_cell; act_cell_count++)
    {
        pci = p_x2ap_gl_ctx->served_cell_info[p_act_cell_index[cell_index]].pci;
        ecgi =
             p_x2ap_gl_ctx->served_cell_info[p_act_cell_index[cell_index]].ecgi;
        for (peer_cell_count = 0;
                   peer_cell_count < p_x2setup_resp->num_served_cell;
                                                              peer_cell_count++)
        {
            for ( peer_ngh_count = 0;
                  peer_ngh_count < p_x2setup_resp->served_cell_arr
                                  [peer_cell_count].nbour_cell_info.
                                  num_neighbour_cell;
                  peer_ngh_count++ )
            {
                ngh_pci = p_x2setup_resp->served_cell_arr[peer_cell_count].
                        nbour_cell_info.neighbour_info_arr[peer_ngh_count].pci;
                ngh_ecgi = p_x2setup_resp->served_cell_arr[peer_cell_count].
                       nbour_cell_info.neighbour_info_arr[peer_ngh_count].ecgi;
                /* Check for the Direct Active neighbour cell */
                if (ngh_pci == pci) 
                {
                   for (ngh_cell_count = 0;
                               ngh_cell_count < MAX_NEIGHBOURS; ngh_cell_count++)
                   {
                       if(p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                          .peer_cell_info.pci ==
                          p_x2setup_resp->served_cell_arr[peer_cell_count].
                                                       served_cell_info.pci)
                       {
                          /* Update relation table */
                          p_x2ap_gl_ctx->relation_table
                          [p_act_cell_index[cell_index]]
                          [ngh_cell_count].relation = DIRECT_NEIGHBOUR_ACTIVE;
                          /* Update neighbour cell info */
                          /* Global eNB ID */
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                             .peer_gb_enb_id = p_x2setup_resp->global_enb_id;
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                             .enb_type =
                           p_x2setup_resp->global_enb_id.enb_id.presence_bitmask;
                          /* pci */
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                            .peer_cell_info.pci = p_x2setup_resp->served_cell_arr
                               [peer_cell_count].served_cell_info.pci;
                          /* ecgi */
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                            .peer_cell_info.ecgi =
                                   p_x2setup_resp->served_cell_arr
                                      [peer_cell_count].served_cell_info.cell_id;
                          /* TAC */
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                            .peer_cell_info.track_area_code =
                                 p_x2setup_resp->served_cell_arr[peer_cell_count]
                                           .served_cell_info.track_area_code;
                          /* Brodcast PLMN */
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                            .peer_cell_info.broadcast_plmn_info =
                                 p_x2setup_resp->served_cell_arr[peer_cell_count]
                                     .served_cell_info.broadcast_plmn_info;
                          /* eUTRA Mode */
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                            .peer_cell_info.choice_eutra_mode = p_x2setup_resp->
                                served_cell_arr[peer_cell_count].
                                          served_cell_info.choice_eutra_mode;
                          /* Anteena port if present */
                          if (p_x2setup_resp->
                                served_cell_arr[peer_cell_count].
                                          served_cell_info.bitmask)
                          {
                              p_x2ap_gl_ctx->peer_enb_cell_info[
                              ngh_cell_count].peer_cell_info.num_antenna_port = 
                              p_x2setup_resp->served_cell_arr[peer_cell_count]
                              .served_cell_info.num_antenna_port;
                          }
#ifdef LTE_EMBMS_SUPPORTED
                            if (X2AP_MBSFN_SUBFRAME_INFO_PRESENT & p_x2setup_resp->
                                    served_cell_arr[peer_cell_count].served_cell_info.bitmask)
                            {
                                p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count].
                                    peer_cell_info.bitmask |= 
                                    X2AP_MBSFN_SUBFRAME_INFO_PRESENT;
                                X2AP_MEMCPY(&p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count].
                                        peer_cell_info.mbsfn_subframe_info,
                                        &p_x2setup_resp->served_cell_arr[peer_cell_count].
                                        served_cell_info.mbsfn_subframe_info,
                                        sizeof(x2ap_mbsfn_subframe_info_t));
                            }
#endif
                          break;
                       }
                   }
                }
            }
        }
        cell_index++;
    }
    X2AP_UT_TRACE_EXIT();
}

/******************************************************************************
 * Function Name  : x2ap_update_cell_info_for_req
 * Inputs         : p_x2ap_gl_ctx -    pointer to X2AP global context
 *                : p_enb_ctx     -   Pointer to eNB context
 *                : p_x2setup_req -   Pointer of x2 setup request
 *                : num_act_cell  -   Number of active cell
 *                : p_act_cell_index -  Pointer to served cell index
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to update cell info in global db
 *                  for received request from peer eNB..
 ******************************************************************************/
void x2ap_update_cell_info_for_req(x2_setup_req_t    *p_x2setup_req,
                                   U16                 num_act_cell,
                                   U16                *p_act_cell_index,
                                   x2ap_gb_context_t *p_x2ap_gl_ctx)
{
    U16                               act_cell_count = 0;
    U16                               cell_index = 0;
    U32                               pci = 0;
    x2ap_ecgi_t                      ecgi;
    U32                               ngh_pci = 0;
    x2ap_ecgi_t                      ngh_ecgi;
    U16                               peer_cell_count = 0;
    U16                               peer_ngh_count = 0;
    U16                               ngh_cell_count = 0;

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_x2setup_req != X2AP_P_NULL);
    X2AP_ASSERT(p_act_cell_index != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);

    X2AP_MEMSET (&ecgi, X2AP_NULL, sizeof(x2ap_ecgi_t));
    X2AP_MEMSET (&ngh_ecgi, X2AP_NULL, sizeof(x2ap_ecgi_t));

    for (act_cell_count = 0; act_cell_count < num_act_cell; act_cell_count++)
    {
        pci = p_x2ap_gl_ctx->served_cell_info[p_act_cell_index[cell_index]].pci;
        ecgi =
             p_x2ap_gl_ctx->served_cell_info[p_act_cell_index[cell_index]].ecgi;
        for (peer_cell_count = 0;
                   peer_cell_count < p_x2setup_req->num_served_cell;
                                                              peer_cell_count++)
        {
            for ( peer_ngh_count = 0; 
                  peer_ngh_count < p_x2setup_req->served_cell_arr
                                  [peer_cell_count].nbour_cell_info.
                                  num_neighbour_cell; 
                  peer_ngh_count++ )
            {
                ngh_pci = p_x2setup_req->served_cell_arr[peer_cell_count].
                        nbour_cell_info.neighbour_info_arr[peer_ngh_count].pci;
                ngh_ecgi = p_x2setup_req->served_cell_arr[peer_cell_count].
                       nbour_cell_info.neighbour_info_arr[peer_ngh_count].ecgi;
                /* Check for the Direct Active neighbour cell */
                if (ngh_pci == pci)  
                {
                   for (ngh_cell_count = 0;
                               ngh_cell_count < MAX_NEIGHBOURS; ngh_cell_count++)
                   {
                       if(p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                          .peer_cell_info.pci ==  
                          p_x2setup_req->served_cell_arr[peer_cell_count].
                                                       served_cell_info.pci)
                       {
                          /* Update relation table */
                          p_x2ap_gl_ctx->relation_table
                          [p_act_cell_index[cell_index]]
                          [ngh_cell_count].relation = DIRECT_NEIGHBOUR_ACTIVE;
                          /* Update neighbour cell info */
                          /* Global eNB ID */
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                             .peer_gb_enb_id = p_x2setup_req->global_enb_id;
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                             .enb_type = 
                           p_x2setup_req->global_enb_id.enb_id.presence_bitmask; 
                          /* pci */
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                            .peer_cell_info.pci = p_x2setup_req->served_cell_arr
                               [peer_cell_count].served_cell_info.pci;
                          /* ecgi */
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                            .peer_cell_info.ecgi = 
                                   p_x2setup_req->served_cell_arr
                                      [peer_cell_count].served_cell_info.cell_id;
                          /* TAC */
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                            .peer_cell_info.track_area_code = 
                                 p_x2setup_req->served_cell_arr[peer_cell_count]
                                           .served_cell_info.track_area_code;
                          /* Brodcast PLMN */
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                            .peer_cell_info.broadcast_plmn_info = 
                                 p_x2setup_req->served_cell_arr[peer_cell_count]
                                     .served_cell_info.broadcast_plmn_info;
                          /* eUTRA Mode */
                          p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count]
                            .peer_cell_info.choice_eutra_mode = p_x2setup_req->
                                served_cell_arr[peer_cell_count].
                                          served_cell_info.choice_eutra_mode;
                          /* Anteena port if present */
                          if (p_x2setup_req->
                                served_cell_arr[peer_cell_count].
                                          served_cell_info.bitmask)
                          {
                              p_x2ap_gl_ctx->peer_enb_cell_info[
                              ngh_cell_count].peer_cell_info.num_antenna_port =
                              p_x2setup_req->served_cell_arr[peer_cell_count]
                              .served_cell_info.num_antenna_port;
                          }

                          if (X2AP_PRACH_CONFIG_INFO_PRESENT & p_x2setup_req->
                                  served_cell_arr[peer_cell_count].served_cell_info.bitmask)
                          {
                              p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count].
                                          peer_cell_info.bitmask |= X2AP_PRACH_CONFIG_INFO_PRESENT;
                              X2AP_MEMCPY(&p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count].peer_cell_info.prach_config,
                                          &p_x2setup_req->served_cell_arr[peer_cell_count].served_cell_info.prach_config,
                                          sizeof(x2ap_prach_configuration_info_t));
                          }
#ifdef LTE_EMBMS_SUPPORTED
                            if (X2AP_MBSFN_SUBFRAME_INFO_PRESENT & p_x2setup_req->
                                    served_cell_arr[peer_cell_count].served_cell_info.bitmask)
                            {
                                p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count].
                                    peer_cell_info.bitmask |= 
                                    X2AP_MBSFN_SUBFRAME_INFO_PRESENT;
                                X2AP_MEMCPY(&p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_count].
                                        peer_cell_info.mbsfn_subframe_info,
                                        &p_x2setup_req->served_cell_arr[peer_cell_count].
                                        served_cell_info.mbsfn_subframe_info,
                                        sizeof(x2ap_mbsfn_subframe_info_t));
                            }
#endif
                          break; 
                       }
                   } 
                }
            }
        }
        cell_index++;
    }
    X2AP_UT_TRACE_EXIT();
}
/******************************************************************************
 * Function Name  : x2ap_build_x2setup_resp
 * Inputs         : num_act_cell - number of activated cells
 *                  p_act_cell_id - sctivated cell id 
 *                  p_x2ap_gl_ctx  pointer to X2AP global context
 *                  p_enb_ctx      Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called to build and send x2 setup success
 *                  response to peer eNB.
 ******************************************************************************/
x2ap_return_et x2ap_build_x2setup_resp(U16                 num_act_cell,
                                       U16                *p_act_cell_id,
                                       x2ap_gb_context_t *p_x2ap_gl_ctx,
                                       x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    static x2_setup_resp_t      x2_setup_resp;
    U16                   srvd_cell_count = 0;
    U16                   ngh_cell_count = 0;
    U16                   act_cell_count = 0;
    U16                   ngh_index_count = 0;
    U8                    gr_index_count = 0;
    U16                   num_ngh_cell = 0;
    U16                   ngh_cell_index[MAX_NEIGHBOURS] = {0};
    U8                    asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16                   asn_msg_len = 0;
    x2ap_peer_enb_msg_t   peer_enb_msg;
    x2_gb_enb_id_t        gb_enb_id;

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_setup_resp,X2AP_NULL,sizeof(x2_setup_resp_t));
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    X2AP_MEMSET(&gb_enb_id,X2AP_NULL,sizeof(x2_gb_enb_id_t));

    /* Fill the global eNB Id */
    x2_setup_resp.global_enb_id = p_x2ap_gl_ctx->global_enb_id;
    /* Fill Number of served cell */
    x2_setup_resp.num_served_cell = num_act_cell;
    /* Fill served cell Info */
    for (srvd_cell_count = 0; srvd_cell_count < num_act_cell; srvd_cell_count++)
    {
        x2_setup_resp.served_cell_arr[srvd_cell_count].
                                        served_cell_info.bitmask =
           p_x2ap_gl_ctx->served_cell_info
                                     [p_act_cell_id[act_cell_count]].bitmask;

        x2_setup_resp.served_cell_arr[srvd_cell_count].served_cell_info.pci =
           p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].pci;

        x2_setup_resp.served_cell_arr[srvd_cell_count].
              served_cell_info.cell_id =
           p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].ecgi;

        x2_setup_resp.served_cell_arr[srvd_cell_count].
              served_cell_info.track_area_code =
           p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
              track_area_code;

        x2_setup_resp.served_cell_arr[srvd_cell_count].
              served_cell_info.broadcast_plmn_info =
           p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
              broadcast_plmn_info;

        x2_setup_resp.served_cell_arr[srvd_cell_count].
              served_cell_info.choice_eutra_mode =
        p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
              choice_eutra_mode;

        if (x2_setup_resp.served_cell_arr
                                [srvd_cell_count].served_cell_info.bitmask &NUM_ANTENNA_PORT_BITMASK)
        {
            x2_setup_resp.served_cell_arr[srvd_cell_count].
              served_cell_info.num_antenna_port =
            (x2ap_num_antenna_port_et)p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
              num_antenna_port; 
        }

        if(X2AP_PRACH_CONFIG_INFO_PRESENT & p_x2ap_gl_ctx->
                        served_cell_info[p_act_cell_id[act_cell_count]].bitmask)
        {
            x2_setup_resp.served_cell_arr[srvd_cell_count].
                        served_cell_info.bitmask |= X2AP_PRACH_CONFIG_INFO_PRESENT;
            X2AP_MEMCPY(&x2_setup_resp.served_cell_arr[srvd_cell_count].served_cell_info.prach_config,
                        &p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].prach_config,
                        sizeof(x2ap_prach_configuration_info_t));
        }
#ifdef LTE_EMBMS_SUPPORTED

        if(X2AP_MBSFN_SUBFRAME_INFO_PRESENT & p_x2ap_gl_ctx->
                served_cell_info[p_act_cell_id[act_cell_count]].bitmask)
        {
            x2_setup_resp.served_cell_arr[srvd_cell_count].
                served_cell_info.bitmask |= X2AP_MBSFN_SUBFRAME_INFO_PRESENT;
            X2AP_MEMCPY(&x2_setup_resp.served_cell_arr[srvd_cell_count].
                    served_cell_info.mbsfn_subframe_info,
                    &p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
                    mbsfn_subframe_info,
                    sizeof(x2ap_mbsfn_subframe_info_t));
        }
#endif

        /* SPR 10077 Fix Start */
        if(X2AP_CSG_ID_INFO_PRESENT & p_x2ap_gl_ctx->
                served_cell_info[p_act_cell_id[act_cell_count]].bitmask)
        {
            x2_setup_resp.served_cell_arr[srvd_cell_count].
                served_cell_info.bitmask |= X2AP_CSG_ID_INFO_PRESENT;
            X2AP_MEMCPY((void*)&(x2_setup_resp.served_cell_arr[srvd_cell_count].
                        served_cell_info.csg_identity),
                    (const void* )&(p_x2ap_gl_ctx->
                        served_cell_info[p_act_cell_id[act_cell_count]].csg_identity),
                    sizeof(x2_setup_resp.served_cell_arr[srvd_cell_count].served_cell_info.csg_identity));
        }
        /* SPR 10077 Fix Stop */

        /* Find the neighbour info for the served cell */
        x2ap_get_ngh_cell_index(&num_ngh_cell,
                                ngh_cell_index,
                                p_act_cell_id[act_cell_count],
                                p_x2ap_gl_ctx,
                                X2AP_FALSE);

        /* Fill number of neighbour cell */
        x2_setup_resp.served_cell_arr[srvd_cell_count].
                  nbour_cell_info.num_neighbour_cell = num_ngh_cell;

        /* Fill the neighbour cell Info */
        for (ngh_cell_count = 0; ngh_cell_count < num_ngh_cell; ngh_cell_count++)
        {
            x2_setup_resp.served_cell_arr[srvd_cell_count].
                 nbour_cell_info.neighbour_info_arr[ngh_cell_count].ecgi =
            p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_index[ngh_index_count]].
                 peer_cell_info.ecgi;
            x2_setup_resp.served_cell_arr[srvd_cell_count].
                 nbour_cell_info.neighbour_info_arr[ngh_cell_count].pci =
            p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_index[ngh_index_count]].
                 peer_cell_info.pci;
            x2_setup_resp.served_cell_arr[srvd_cell_count].
                 nbour_cell_info.neighbour_info_arr[ngh_cell_count].earfcn =
            p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_index[ngh_index_count]].
                 peer_cell_info.earfcn;
            ngh_index_count++;
        }
         X2AP_MEMSET(&ngh_cell_index, 0, MAX_NEIGHBOURS);
ngh_index_count = 0;
        act_cell_count++;
    }
    /* Fill number of GU Group Id */
    x2_setup_resp.num_gu_group_id = p_x2ap_gl_ctx->group_ids.num_gu_group_id;
    /* Fill GU Group Id List */
    for (gr_index_count = 0;
                    gr_index_count < x2_setup_resp.num_gu_group_id;
                                                              gr_index_count++)
    {
        x2_setup_resp.group_id_arr[gr_index_count] =
                        p_x2ap_gl_ctx->group_ids.group_id_arr[gr_index_count];
    }
    /* ASN Encoding */
    retVal = x2ap_encode_x2setup_resp(&(x2_setup_resp),
                                     asn_msg_buff,
                                     &asn_msg_len );

    if (X2AP_SUCCESS == retVal)
    {
        peer_enb_msg.p_msg = asn_msg_buff;
        peer_enb_msg.msg_len = asn_msg_len;
        /* SPR_14436_Fix Start */
        peer_enb_msg.stream_id = p_enb_ctx->stream_info.
            stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
        /* SPR_14436_Fix Stop */
        /*Sending the  X2 SETUP RESPONSE to Peer eNB via SCTP */
        retVal = x2ap_sctp_sm_entry (X2AP_SCTP_SEND_DATA_EV,
                (U8 *)(&peer_enb_msg),
                p_x2ap_gl_ctx,
                p_enb_ctx);
        if (X2AP_SUCCESS == retVal)
        {
            X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP RESPONSE");
        }
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/******************************************************************************
 * Function Name  : x2ap_build_x2setup_fail_resp
 * Inputs         : p_x2ap_gl_ctx - pointer to X2AP global context
 *                : p_enb_ctx   -  Pointer to eNB context
 *                  timeTowait_present - boolean value
 *                  p_error_indication - error indicatiob data 
 *                  p_snd_err_ind - pointer to x2ap_error_ind_bool_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called to build and send x2 setup failure
 *                  response to peer eNB with or without time to wait IE.
 ******************************************************************************/
x2ap_return_et x2ap_build_x2setup_fail_resp(x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                            x2ap_peer_enb_context_t *p_enb_ctx,
                                            x2ap_bool_et    timeTowait_present,
                                rrc_x2ap_error_indication_t *p_error_indication,
                                x2ap_error_ind_bool_t *p_snd_err_ind)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    x2_setup_fail_t      x2_setup_fail_resp;
    U8                    asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16                   asn_msg_len = 0;
    x2_gb_enb_id_t        gb_enb_id;
    x2ap_peer_enb_msg_t  peer_enb_msg;

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_setup_fail_resp,X2AP_NULL,sizeof(x2_setup_fail_t));
    X2AP_MEMSET(&gb_enb_id,X2AP_NULL,sizeof(x2_gb_enb_id_t));
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    /* Build the x2 setup failure resp */
    if (X2AP_TRUE == timeTowait_present)
    {
        x2_setup_fail_resp.cause.cause_type = T_x2ap_Cause_radioNetwork;//T_x2ap_Cause_misc; //X2AP_SETUP_FAIL_MISC_CAUSE_TYPE;
        /*SPR_18216_START*/
        x2_setup_fail_resp.cause.cause_info.misc_cause = 
            x2ap_cell_not_available;
            //x2ap_not_enough_user_plane_processing_resources;// X2AP_MISC_CAUSE; 
        x2_setup_fail_resp.time_to_wait = (x2ap_time_to_wait_et)p_x2ap_gl_ctx->config_param.
                                 timer_conf_info.x2ap_time_to_wait_index;
    }
    else if ((p_error_indication != X2AP_P_NULL) && (p_error_indication->cause.t 
                == T_x2ap_Cause_protocol))
    {
        x2_setup_fail_resp.cause.cause_type = T_x2ap_Cause_protocol;
        x2_setup_fail_resp.cause.cause_info.protocol_cause=
                    p_error_indication->cause.u.protocol;
        x2_setup_fail_resp.time_to_wait = SEC_1;
    }
    else if (p_error_indication != X2AP_P_NULL)
    {
        x2_setup_fail_resp.cause.cause_type = p_error_indication->cause.t;
        switch (p_error_indication->cause.t)
        {
            case T_x2ap_Cause_radioNetwork:
                x2_setup_fail_resp.cause.cause_info.radio_nw_cause = p_error_indication->cause.u.radioNetwork;
                break;
            case T_x2ap_Cause_transport:
                x2_setup_fail_resp.cause.cause_info.transport_cause = p_error_indication->cause.u.transport;
                break;
            case T_x2ap_Cause_protocol:
                x2_setup_fail_resp.cause.cause_info.protocol_cause = p_error_indication->cause.u.protocol;
                break;
            case T_x2ap_Cause_misc:
                x2_setup_fail_resp.cause.cause_info.misc_cause = p_error_indication->cause.u.misc;
                break;
            default:
                x2_setup_fail_resp.cause.cause_type = T_x2ap_Cause_radioNetwork;
                x2_setup_fail_resp.cause.cause_info.radio_nw_cause = x2ap_unspecified_2;
                break;    
        }
    }
    else
    {
        x2_setup_fail_resp.cause.cause_type = T_x2ap_Cause_transport;
        x2_setup_fail_resp.cause.cause_info.transport_cause = 
            x2ap_transport_resource_unavailable;
        /*SPR_18216_END*/
        x2_setup_fail_resp.time_to_wait = SEC_1;
    }

    X2AP_MEMSET(&(x2_setup_fail_resp.critc_diagno),
                 0,
                sizeof(x2ap_crit_diagno_t));           

    /* ASN Encoding */
    retVal = x2ap_encode_x2setup_failure_resp(&(x2_setup_fail_resp),
                                              asn_msg_buff,
                                              &asn_msg_len,
                                          p_error_indication,
                                            p_snd_err_ind );

    if (X2AP_SUCCESS == retVal)
    {
        peer_enb_msg.p_msg = asn_msg_buff;
        peer_enb_msg.msg_len = asn_msg_len;
        /* SPR_14436_Fix Start */
        peer_enb_msg.stream_id = p_enb_ctx->stream_info.
            stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
        /* SPR_14436_Fix Stop */

        /*Sending the  X2 SETUP REQUEST to Peer eNB via SCTP */
        retVal = x2ap_sctp_sm_entry (X2AP_SCTP_SEND_DATA_EV, 
                (U8 *)(&peer_enb_msg),
                p_x2ap_gl_ctx,
                p_enb_ctx);
        if (X2AP_SUCCESS == retVal)
        {
            X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP FAIL %s",
                change_data_to_str(peer_enb_msg.msg_len, (U8 *)peer_enb_msg.p_msg)); 
            
            X2AP_TRACE(X2AP_INFO, "X2 SETUP FAILURE Msg sent to "
                    "Peer ENodeB ID : %d", p_enb_ctx->peer_enodeb_id);
        }
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR,"[%s]::ASN Encoding of X2 Setup Failed"
                   ,__FUNCTION__);
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : x2_setup_fail_resp_hdl
 * Inputs         : p_msg_ptr   -   Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when x2 setup failure response 
 *                  is received from peer eNB.
 ******************************************************************************/
x2ap_return_et x2_setup_fail_resp_hdl (U8                 *p_msg_ptr,
                                       x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                       x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    x2_setup_fail_t      x2_setup_fail_resp;
    x2ap_peer_enb_msg_t  *p_peer_enb_msg = X2AP_P_NULL;
    x2ap_timer_buffer_t  x2_setup_rsp_timer_buf;
    x2_gb_enb_id_t       gb_enb_id;
    U32                   timer_value = 0;

	/*SPR_19067_FIX_START*/
    OSCTXT                       asn1_ctx;
    /*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;
    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_setup_rsp_timer_buf,X2AP_NULL,sizeof(x2ap_timer_buffer_t));
    X2AP_MEMSET(&x2_setup_fail_resp,X2AP_NULL,sizeof(x2_setup_fail_t));
    X2AP_MEMSET(&gb_enb_id,X2AP_NULL,sizeof(x2_gb_enb_id_t));

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP FAIL Received");
    
    /* Decode the ASN request */
    retVal = x2ap_decode_x2setup_failure_resp(
            p_x2ap_gl_ctx,
            p_enb_ctx,
            &x2_setup_fail_resp,
            (p_peer_enb_msg)->p_msg,
			/*SPR_19067_FIX_START*/
            &(p_peer_enb_msg->msg_len),
    	    &asn1_ctx);
            /*SPR_19067_FIX_STOP*/
    if (X2AP_SUCCESS == retVal)
    {
        /* Stop X2 Setup Response guard Timer */
        x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;

        X2AP_TRACE(X2AP_INFO,
                                "X2AP_MSG: X2 SETUP FAIL");
        
        /* Adding 1 for the first attempt i.e. "try" count, other count is "retry" count*/
        if ( ((p_x2ap_gl_ctx->config_param.timer_conf_info.
                  x2ap_setup_retry_cnt + 1) == 
                     p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count) ||
        /* SPR 10858 Fix Start */
             (!(X2_SETUP_FAIL_TIME_TO_WAIT_PRESENT & x2_setup_fail_resp.bitmask) ) ||
        /* SPR 10858 Fix Stop */
             (PROTOCOL_CAUSE ==  x2_setup_fail_resp.cause.cause_type) )
        {
            gb_enb_id = p_enb_ctx->gb_enb_id;
            /* Send X2AP_SCTP_CONN_CLOSE_EV event to SCTP SM */
            retVal = x2ap_sctp_sm_entry(X2AP_SCTP_CONN_CLOSE_EV,
                                        (U8 *)&gb_enb_id,
                                        p_x2ap_gl_ctx,
                                        p_enb_ctx );
            /* Change state to IDLE */
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state =
                                                 X2_SETUP_IDLE_ST;
            /* reset the timer ids and retry count */
            p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = 0;
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;
            p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
 
            /* Send Link down indication to OAM and RRM*/
        }
        /* SPR 10858 Fix Start */
        else if (X2_SETUP_FAIL_TIME_TO_WAIT_PRESENT & x2_setup_fail_resp.bitmask)
        /* SPR 10858 Fix Stop */
        {
            /* Start guard timer for X2_Setup_Rsp*/
            x2_setup_rsp_timer_buf.timer_type =
                                    X2AP_X2_SETUP_TIME_TO_WAIT_TIMER ;
            x2_setup_rsp_timer_buf.auto_delete  = X2AP_TRUE;
            x2_setup_rsp_timer_buf.sd    = p_enb_ctx->sctp_sd;
            /*SPR_16533_DELTA2_START*/
#ifdef LKSCTP
            if(p_enb_ctx->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
            {
                l3_strncpy_wrapper((S8*)x2_setup_rsp_timer_buf.ip_addr,
                        (const S8*)p_enb_ctx->enb_comm_info.ipv6_addr[0].ipv6_addr,
                        MAX_X2AP_IPV6_ADDRESS_LENGTH);

            }
            else
            {
                l3_strncpy_wrapper((S8*)x2_setup_rsp_timer_buf.ip_addr,
                        (const S8*)p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr,
                        MAX_X2AP_IP_ADDRESS_LENGTH);
            }
#endif
            /*SPR_16533_DELTA2_END*/


            switch(x2_setup_fail_resp.time_to_wait)
            {
                case SEC_1:
                            timer_value = 1*1000;
                            break;  
                case SEC_2:
                            timer_value = 2*1000;
                            break;  
                case SEC_5:
                            timer_value = 5*1000;
                            break;  
                case SEC_10:
                            timer_value = 10*1000;
                            break;  
                case SEC_20:
                            timer_value = 20*1000;
                            break;  
                case SEC_60:
                            timer_value = 60*1000;
                            break; 
                default:  
                            X2AP_TRACE(X2AP_INFO, 
                                    "[%s] [Line no:%d] Invalid Time to" 
                                    " wait value received", 
                                    __FUNCTION__, __LINE__);
							/*SPR_19067_FIX_START*/
			    			rtFreeContext(&asn1_ctx);
			    			/*SPR_19067_FIX_STOP*/

                            return X2AP_FAILURE;
                       break;                        
            }           
            p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = 
                         x2ap_start_timer(timer_value,
                                          &x2_setup_rsp_timer_buf,
                                          sizeof(x2ap_timer_buffer_t),
                                          X2AP_FALSE);
            
            /* Change state to X2_SETUP_TIME_TO_WAIT_EXP_ST */
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state =
                                          X2_SETUP_TIME_TO_WAIT_EXP_ST;
        }
    }
    else
    {
        /* Send Error Indication to peer eNB */
        X2AP_TRACE(X2AP_INFO,"---Error Indication Detected---");
        retVal = X2AP_SUCCESS;
    } 

	 /*SPR_19067_FIX_START*/
    rtFreeContext(&asn1_ctx);
    /*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : x2ap_send_link_down_ind_to_oam_rrm
 * Inputs         : p_enb_ctx -     Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called to send x2 link down indication to
 *                  OAM and RRM.
 ******************************************************************************/
x2ap_return_et x2ap_send_link_down_ind_to_oam_rrm(
                                x2ap_peer_enb_context_t *p_enb_ctx) 
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    x2ap_oam_x2_link_down_t        oam_link_down_ind;
    x2ap_rrm_x2_link_down_ind_t    rrm_link_down_ind;
    X2AP_UT_TRACE_ENTER();
    X2AP_MEMSET(&oam_link_down_ind,X2AP_NULL,sizeof(x2ap_oam_x2_link_down_t));
    X2AP_MEMSET(&rrm_link_down_ind,X2AP_NULL,sizeof(x2ap_rrm_x2_link_down_ind_t));

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    if ( X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT &
        p_enb_ctx->bitmask ) 
    {
        oam_link_down_ind.bitmask |= X2AP_OAM_X2_LINK_DOWN_GNB_GB_ID_PRESENT; 
        /* Send X2 link down to OAM */
        oam_link_down_ind.peer_gnb_gb_id = p_enb_ctx->gb_gnb_id;
    }
    else
    {
    /* MENB CHANGES - END */
    #endif
        oam_link_down_ind.bitmask |= X2AP_OAM_X2_LINK_DOWN_ENB_GB_ID_PRESENT; 
        /* Send X2 link down to OAM */
        oam_link_down_ind.peer_enb_gb_id = p_enb_ctx->gb_enb_id;
    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    }
    /* MENB CHANGES - END */
    #endif

    /*SPR 14503 Fix Start*/
    if ((p_enb_ctx->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT) &&
        (p_enb_ctx->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT))
	{
		oam_link_down_ind.bitmask |= X2AP_OAM_X2_LINK_DOWN_IPV6_ADDRESS_PRESENT;
		l3_memcpy_wrapper(oam_link_down_ind.ipv6_addr , p_enb_ctx->
			enb_comm_info.ipv6_addr[0].ipv6_addr , MAX_X2AP_IPV6_ADDRESS_LENGTH);
	}
	else
	{
		oam_link_down_ind.bitmask |= X2AP_OAM_X2_LINK_DOWN_IPV4_ADDRESS_PRESENT;
		l3_memcpy_wrapper(oam_link_down_ind.ip_addr , p_enb_ctx->
			enb_comm_info.ip_addr[0].ip_addr , MAX_X2AP_IP_ADDRESS_LENGTH);
	}
	oam_link_down_ind.bitmask |= X2AP_OAM_X2_LINK_DOWN_PORT_PRESENT;
	oam_link_down_ind.port = p_enb_ctx->enb_comm_info.port;

    /*SPR 14503 Fix Start*/

    /* Get API length */
    if (RRC_FAILURE == 
            rrc_x2apOam_il_send_x2ap_oam_x2_link_down (&oam_link_down_ind,
                RRC_X2AP_MODULE_ID,RRC_OAM_MODULE_ID, 0, RRC_NULL))
    {
        return X2AP_FAILURE;
    }

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    if ( X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT &
        p_enb_ctx->bitmask )
    {
        rrm_link_down_ind.bitmask |= X2AP_RRM_LINK_DOWN_GB_GNB_ID_PRESENT;
        /* Send X2 link down to RRM */
        rrm_link_down_ind.peer_gb_gnb_id = p_enb_ctx->gb_gnb_id;
    }
    else
    {
    /* MENB CHANGES - END */
    #endif
        rrm_link_down_ind.bitmask |= X2AP_RRM_LINK_DOWN_GB_ENB_ID_PRESENT;
        /* Send X2 link down to RRM */
        rrm_link_down_ind.peer_gb_enb_id = p_enb_ctx->gb_enb_id;
    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    }
    /* MENB CHANGES - END */
    #endif

    /* Get API length */
    if (RRC_FAILURE == 
            rrc_x2apRrm_il_send_x2ap_rrm_x2_link_down_ind(&rrm_link_down_ind, 
                RRC_X2AP_MODULE_ID,RRC_RRM_MODULE_ID, 0, RRC_NULL))
    {
        return X2AP_FAILURE;
    } 

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : x2ap_send_link_down_ind_to_son
 * Inputs         : p_x2ap_gl_ctx - Pointer to x2ap global context
 *                  p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called to send x2 link down ind to SON
 *****************************************************************************/
x2ap_return_et x2ap_send_link_down_ind_to_son(
                x2ap_gb_context_t   *p_x2ap_gl_ctx,
                x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    x2ap_enb_link_down_ind_t    son_link_down_ind;
    X2AP_UT_TRACE_ENTER();
    X2AP_MEMSET(&son_link_down_ind, X2AP_NULL, sizeof(x2ap_enb_link_down_ind_t));

    son_link_down_ind.bitmask |= X2AP_PEER_ENB_ID_INFO_PRESENT;
    X2AP_MEMCPY(&(son_link_down_ind.enb1_gb_id), &(p_x2ap_gl_ctx->global_enb_id),
                    sizeof(x2_gb_enb_id_t));
    X2AP_MEMCPY(&(son_link_down_ind.enb2_gb_id), &(p_enb_ctx->gb_enb_id),
                    sizeof(x2_gb_enb_id_t));

    if (RRC_FAILURE == rrc_x2apCommon_send_x2ap_enb_link_down_ind(
                            &son_link_down_ind, RRC_X2AP_MODULE_ID,
                            RRC_SON_MODULE_ID, 0, RRC_NULL))
    {
        retVal = X2AP_FAILURE;
    }
            
    X2AP_UT_TRACE_EXIT();
    return retVal;
}


/******************************************************************************
 * Function Name  : x2ap_send_link_up_ind_to_oam
 * Inputs         : p_enb_ctx  -    Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called to send x2 link up indication to
 *                  OAM.
 ******************************************************************************/
x2ap_return_et x2ap_send_link_up_ind_to_oam(
                                x2ap_peer_enb_context_t *p_enb_ctx) 
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;

    x2ap_oam_x2_link_up_t          oam_link_up_ind;
    X2AP_UT_TRACE_ENTER();

    X2AP_MEMSET(&oam_link_up_ind,X2AP_NULL,sizeof(x2ap_oam_x2_link_up_t));
    /* Send X2 link up to OAM */
    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    if ( X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT &
        p_enb_ctx->bitmask )
    {
        X2AP_TRACE(X2AP_INFO,"GB GNB ID present ");
        oam_link_up_ind.bitmask |= X2AP_OAM_X2_LINK_UP_GNB_ID_PRESENT;
        X2AP_MEMCPY(&(oam_link_up_ind.peer_gnb_gb_id), &(p_enb_ctx->gb_gnb_id) ,
            sizeof(x2_gb_gnb_id_t));
    }
    else
    {
        X2AP_TRACE(X2AP_INFO,"GB ENB ID present ");
    /* MENB CHANGES - END */
    #endif
        oam_link_up_ind.bitmask |= X2AP_OAM_X2_LINK_UP_ENB_ID_PRESENT;
        oam_link_up_ind.peer_enb_gb_id = p_enb_ctx->gb_enb_id;

/* SPR-16334 START */
        X2AP_MEMCPY(&(oam_link_up_ind.peer_enb_gb_id), &(p_enb_ctx->gb_enb_id) , sizeof(x2_gb_enb_id_t));
    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    }
    /* MENB CHANGES - END */
    #endif

/* SPR-16334 START */
//    X2AP_MEMCPY(&(oam_link_up_ind.peer_enb_gb_id), &(p_enb_ctx->gb_enb_id) , sizeof(x2_gb_enb_id_t));
    X2AP_MEMCPY(&(oam_link_up_ind.enb_comm_info), &(p_enb_ctx->enb_comm_info) , sizeof(x2_enb_comm_info_t));
/* SPR-16334 END */


    /*SPR 14503 Fix Start*/
    if ((p_enb_ctx->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT) &&
        (p_enb_ctx->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT))
        {
		oam_link_up_ind.bitmask |= X2AP_OAM_X2_LINK_UP_IPV6_ADDRESS_PRESENT;
		l3_memcpy_wrapper(oam_link_up_ind.ipv6_addr , p_enb_ctx->
			enb_comm_info.ipv6_addr[0].ipv6_addr , MAX_X2AP_IPV6_ADDRESS_LENGTH);
		

	}
	else
	{
		oam_link_up_ind.bitmask |= X2AP_OAM_X2_LINK_UP_IPV4_ADDRESS_PRESENT;
		l3_memcpy_wrapper(oam_link_up_ind.ip_addr , p_enb_ctx->
			enb_comm_info.ip_addr[0].ip_addr ,MAX_X2AP_IP_ADDRESS_LENGTH );

	}
	oam_link_up_ind.bitmask |= X2AP_OAM_X2_LINK_UP_PORT_PRESENT;
	oam_link_up_ind.port = p_enb_ctx->enb_comm_info.port;	

    /*SPR 14503 Fix Stop*/

    /* Get API length */
    if (RRC_FAILURE == rrc_x2apOam_il_send_x2ap_oam_x2_link_up(
                &oam_link_up_ind,
                RRC_X2AP_MODULE_ID,RRC_OAM_MODULE_ID, 0, RRC_NULL))
    {
        return X2AP_FAILURE;
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}


/******************************************************************************
 * Function Name  : x2ap_build_x2_link_up_ind
 * Inputs         : p_x2_setup_resp - pointer to x2_setup_resp_t
 *                  p_rrm_link_up_ind -- pointer to x2ap_rrm_x2_link_up_ind_st
 *                  num_act_cell - num of activated cell
 *                  p_act_cell_id - cell id
 *                  p_x2ap_gl_ctx - pointer to X2AP global context
 * Outputs        : p_rrm_link_up_ind Message for rrm interface
 * Returns        : void 
 * Description    : This function is called to build link UP indication
 ******************************************************************************/
void x2ap_build_x2_link_up_ind(x2_setup_resp_t           *p_x2_setup_resp,
                               x2ap_rrm_x2_link_up_ind_st *p_rrm_link_up_ind)
                               /*SPR 17777 +-*/
{
    /*U16               ngh_index_count = 0;*/
    U8                gu_count = 0;
    /*U16               ngh_cell_index[MAX_NEIGHBOURS] ={0};*/
    U16               ngh_srvd_cell_count = 0;
    U16               peer_ngh_cell_count = 0;
    X2AP_UT_TRACE_ENTER();

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    p_rrm_link_up_ind->bitmask |= RRM_X2AP_EUTRA_CELL_INFO_PRESENT;
    /* MENB CHANGES - END */
    #endif

    /* Fill the global eNB Id */
    p_rrm_link_up_ind->eutra_cell_info.global_enb_id = p_x2_setup_resp->global_enb_id;

    p_rrm_link_up_ind->eutra_cell_info.num_served_cell = p_x2_setup_resp->num_served_cell;

    /* peer served cell count */
    for(ngh_srvd_cell_count = 0; ngh_srvd_cell_count <
            p_x2_setup_resp->num_served_cell; ngh_srvd_cell_count++)
    {
        /* Fill the local cell ID */
        p_rrm_link_up_ind->eutra_cell_info.cell_info[ngh_srvd_cell_count].served_cell_id =
            ngh_srvd_cell_count;
        p_rrm_link_up_ind->eutra_cell_info.cell_info[ngh_srvd_cell_count].
            ngh_dir_cell_arr.served_cell_info = p_x2_setup_resp->
            served_cell_arr[ngh_srvd_cell_count].served_cell_info;
        p_rrm_link_up_ind->eutra_cell_info.cell_info[ngh_srvd_cell_count].
            ngh_dir_cell_arr.nbour_cell_info.num_neighbour_cell = 
            p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].
            nbour_cell_info.num_neighbour_cell;
        /* peer neighbour cell count */
        for(peer_ngh_cell_count = 0; peer_ngh_cell_count <
                p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].
                  nbour_cell_info.num_neighbour_cell; peer_ngh_cell_count++)
        {
            p_rrm_link_up_ind->eutra_cell_info.cell_info[ngh_srvd_cell_count].
                ngh_dir_cell_arr.nbour_cell_info.neighbour_info_arr[
                peer_ngh_cell_count].ecgi = p_x2_setup_resp->
                    served_cell_arr[ngh_srvd_cell_count].nbour_cell_info
                    .neighbour_info_arr[peer_ngh_cell_count].ecgi;
            /* Fill PCI*/
            p_rrm_link_up_ind->eutra_cell_info.cell_info[ngh_srvd_cell_count].
                ngh_dir_cell_arr.nbour_cell_info.neighbour_info_arr[
                peer_ngh_cell_count].pci = p_x2_setup_resp->
                    served_cell_arr[ngh_srvd_cell_count].nbour_cell_info
                    .neighbour_info_arr[peer_ngh_cell_count].pci;
            /* Fill EARFCN */
            p_rrm_link_up_ind->eutra_cell_info.cell_info[ngh_srvd_cell_count].
                ngh_dir_cell_arr.nbour_cell_info.neighbour_info_arr[
                peer_ngh_cell_count].earfcn = p_x2_setup_resp->
                    served_cell_arr[ngh_srvd_cell_count].nbour_cell_info
                    .neighbour_info_arr[peer_ngh_cell_count].earfcn;
        }
    }
    p_rrm_link_up_ind->eutra_cell_info.num_gu_group_id = p_x2_setup_resp->num_gu_group_id;
    for (gu_count = 0; gu_count < p_rrm_link_up_ind->eutra_cell_info.num_gu_group_id;gu_count++)
    {
        p_rrm_link_up_ind->eutra_cell_info.group_id_arr[gu_count] =
            p_x2_setup_resp->group_id_arr[gu_count];
    }
    X2AP_UT_TRACE_EXIT();
}

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
/******************************************************************************
 * Function Name  : x2ap_build_rrm_srvd_nr_cell_info
 * Inputs         : p_x2ap_srvd_nr_cell_info - pointer to x2ap_srvd_nr_cell_info_t
 *                  p_rrm_srvd_nr_cell_info -- pointer to x2ap_rrm_srvd_nr_cell_info_t
 *                  
 * Outputs        : p_rrm_srvd_nr_cell_info Message for rrm interface
 * Returns        : void 
 * Description    : This function is called to build x2ap_rrm_srvd_nr_cell_info_t
 ******************************************************************************/
void
x2ap_build_x2ap_meas_timing_config
(
    nr_rrc_MeasurementTimingConfiguration *p_nr_rrc_meas_timing_config,
    x2ap_meas_timing_config_t *p_x2ap_meas_timing_config_arr,
    U8 *p_meas_timing_config_count
)
{
    U8                          index = X2AP_NULL;
    OSRTDListNode               *p_node = PNULL; 
    nr_rrc_MeasTimingList       *p_meas_timing_list = PNULL;
    nr_rrc_MeasTiming           *p_meas_timing_item = PNULL;
    nr_rrc_MeasurementTimingConfiguration *p_meas_timing_config = PNULL;
    x2ap_meas_timing_config_t *p_x2ap_meas_timing_config = X2AP_P_NULL;

    X2AP_UT_TRACE_ENTER();

    /* Meas timing config */
    if ( ( T_nr_rrc_MeasurementTimingConfiguration_criticalExtensions_c1 ==
                p_nr_rrc_meas_timing_config->criticalExtensions.t ) &&
            ( PNULL != p_nr_rrc_meas_timing_config->criticalExtensions.u.c1 ) &&
            ( T_nr_rrc_MeasurementTimingConfiguration_criticalExtensions_c1_measTimingConf ==
              p_nr_rrc_meas_timing_config->criticalExtensions.u.c1->t ) &&
            ( PNULL !=  p_nr_rrc_meas_timing_config->criticalExtensions.u.c1->u.measTimingConf )
       )
    {
        if ( p_nr_rrc_meas_timing_config->criticalExtensions.u.c1->u.measTimingConf->m.
                measTimingPresent )
        {
            p_meas_timing_list = &(p_nr_rrc_meas_timing_config->criticalExtensions.u.c1->
                        u.measTimingConf->measTiming);
            p_node = p_meas_timing_list->head;
            for ( index = X2AP_NULL; index < p_meas_timing_list->count;
                    index++ )
            {
                if ( !p_node )
                {
                    X2AP_TRACE(X2AP_ERROR,
                            "%s: next node returned NULL",
                            __FUNCTION__);
                    break;
                }

                p_x2ap_meas_timing_config = p_x2ap_meas_timing_config_arr + index; 

                p_meas_timing_item =
                    (nr_rrc_MeasTiming*)(p_node->data);

                if ( p_meas_timing_item->m.frequencyAndTimingPresent )
                {
                    /*
                     * Fill nr_rrc_ARFCN_ValueNR 
                     */
                    p_x2ap_meas_timing_config->carrier_freq =
                        p_meas_timing_item->frequencyAndTiming.carrierFreq; 
/* NR_BAND_FIX_START */
                    /*
                     * Fill nr_rrc_SubcarrierSpacing
                     */
                    p_x2ap_meas_timing_config->ssb_subcarrier_spacing =
                        p_meas_timing_item->frequencyAndTiming.ssbSubcarrierSpacing; 
/* NR_BAND_FIX_END */


                     /*
                      * Fill nr_rrc_SSB_MTC 
                      */
                     switch ( p_meas_timing_item->frequencyAndTiming.
                             ssb_MeasurementTimingConfiguration.periodicityAndOffset.t )
                     {
                        /*
                        * Fill periodicityAndOffset 
                        */
                        case T_nr_rrc_SSB_MTC_periodicityAndOffset_sf5:
                        {
                           p_x2ap_meas_timing_config->
                               ssb_meas_timimg_configuration.
                               x2ap_periodicity_and_offset.bitmask |=
                               X2AP_PERIODICTY_OFFSET_SF5_PRESENT;

                           p_x2ap_meas_timing_config->
                               ssb_meas_timimg_configuration.
                               x2ap_periodicity_and_offset.sf5 =
                               p_meas_timing_item->frequencyAndTiming.
                               ssb_MeasurementTimingConfiguration.periodicityAndOffset.
                               u.sf5; 
                           break;
                        }

                        case T_nr_rrc_SSB_MTC_periodicityAndOffset_sf10:
                        {
                            p_x2ap_meas_timing_config->
                                ssb_meas_timimg_configuration.
                                x2ap_periodicity_and_offset.bitmask |=
                                X2AP_PERIODICTY_OFFSET_SF10_PRESENT;

                            p_x2ap_meas_timing_config->
                                ssb_meas_timimg_configuration.
                                x2ap_periodicity_and_offset.sf10 =
                                p_meas_timing_item->frequencyAndTiming.
                                ssb_MeasurementTimingConfiguration.periodicityAndOffset.
                                u.sf10; 
                            break;
                        }

                        case T_nr_rrc_SSB_MTC_periodicityAndOffset_sf20:
                        {
                            p_x2ap_meas_timing_config->
                                ssb_meas_timimg_configuration.
                                x2ap_periodicity_and_offset.bitmask |=
                                X2AP_PERIODICTY_OFFSET_SF20_PRESENT;

                            p_x2ap_meas_timing_config->
                                ssb_meas_timimg_configuration.
                                x2ap_periodicity_and_offset.sf20 =
                                p_meas_timing_item->frequencyAndTiming.
                                ssb_MeasurementTimingConfiguration.periodicityAndOffset.
                                u.sf20; 
                            break;
                        }

                        case T_nr_rrc_SSB_MTC_periodicityAndOffset_sf40:
                        {
                            p_x2ap_meas_timing_config->
                                ssb_meas_timimg_configuration.
                                x2ap_periodicity_and_offset.bitmask |=
                                X2AP_PERIODICTY_OFFSET_SF40_PRESENT;

                            p_x2ap_meas_timing_config->
                                ssb_meas_timimg_configuration.
                                x2ap_periodicity_and_offset.sf40 =
                                p_meas_timing_item->frequencyAndTiming.
                                ssb_MeasurementTimingConfiguration.periodicityAndOffset.
                                u.sf40; 
                            break;
                        }

                        case T_nr_rrc_SSB_MTC_periodicityAndOffset_sf80:
                        {
                            p_x2ap_meas_timing_config->
                                ssb_meas_timimg_configuration.
                                x2ap_periodicity_and_offset.bitmask |=
                                X2AP_PERIODICTY_OFFSET_SF80_PRESENT;

                            p_x2ap_meas_timing_config->
                                ssb_meas_timimg_configuration.
                                x2ap_periodicity_and_offset.sf80 =
                                p_meas_timing_item->frequencyAndTiming.
                                ssb_MeasurementTimingConfiguration.periodicityAndOffset.
                                u.sf80; 
                            break;
                        }

                        case T_nr_rrc_SSB_MTC_periodicityAndOffset_sf160:
                        {
                            p_x2ap_meas_timing_config->
                                ssb_meas_timimg_configuration.
                                x2ap_periodicity_and_offset.bitmask |=
                                X2AP_PERIODICTY_OFFSET_SF160_PRESENT;

                            p_x2ap_meas_timing_config->
                                ssb_meas_timimg_configuration.
                                x2ap_periodicity_and_offset.sf160 =
                                p_meas_timing_item->frequencyAndTiming.
                                ssb_MeasurementTimingConfiguration.periodicityAndOffset.
                                u.sf160; 
                            break;
                        }

                        default:
                        {
                            X2AP_TRACE(X2AP_ERROR,"%s: Invalid SSB MTC periodicity offset",
                                    __FUNCTION__);
                            break;
                        }
                    }

                    /*
                     * Fill duration
                     */
                    p_x2ap_meas_timing_config->ssb_meas_timimg_configuration.
                        x2ap_mtc_duration =
                        p_meas_timing_item->frequencyAndTiming.
                            ssb_MeasurementTimingConfiguration.duration; 
                }
            }

            *p_meas_timing_config_count = p_meas_timing_list->count;
        }
    }
    else
    {
        RRC_TRACE(RRC_ERROR,"Measurement timing config buffer is NULL");
    }

    X2AP_UT_TRACE_EXIT();
}

/******************************************************************************
 * Function Name  : x2ap_build_rrm_srvd_nr_cell_info
 * Inputs         : p_x2ap_srvd_nr_cell_info - pointer to x2ap_srvd_nr_cell_info_t
 *                  p_rrm_srvd_nr_cell_info -- pointer to x2ap_rrm_srvd_nr_cell_info_t
 *                  
 * Outputs        : p_rrm_srvd_nr_cell_info Message for rrm interface
 * Returns        : void 
 * Description    : This function is called to build x2ap_rrm_srvd_nr_cell_info_t
 ******************************************************************************/
void
x2ap_build_rrm_srvd_nr_cell_info
(
    x2ap_srvd_nr_cell_info_t *p_x2ap_srvd_nr_cell_info,
    x2ap_rrm_srvd_nr_cell_info_t *p_rrm_srvd_nr_cell_info
)
{
    X2AP_UT_TRACE_ENTER();

    /* Fill the local cell ID */
    //        p_rrm_srvd_nr_cell_info.served_cell_id =
    //                      srvd_cell_count;
    p_rrm_srvd_nr_cell_info->nr_pci = p_x2ap_srvd_nr_cell_info->nr_pci;
    l3_memcpy_wrapper(&p_rrm_srvd_nr_cell_info->nr_cell_id, 
            &p_x2ap_srvd_nr_cell_info->nr_cell_id,
            sizeof(x2ap_nr_ecgi_t));

    if ( X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT & 
            p_x2ap_srvd_nr_cell_info->bitmask )
    {
        p_rrm_srvd_nr_cell_info->bitmask |=
            X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT;

        l3_memcpy_wrapper(&p_rrm_srvd_nr_cell_info->fiveGS_track_area_code,
            &p_x2ap_srvd_nr_cell_info->fiveGS_track_area_code,
            sizeof(x2ap_five_gs_tac_t));
    }

    if ( X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT & 
         p_x2ap_srvd_nr_cell_info->bitmask )
    {
        p_rrm_srvd_nr_cell_info->bitmask |=
            X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT;

        l3_memcpy_wrapper(&p_rrm_srvd_nr_cell_info->configured_tac, 
            &p_x2ap_srvd_nr_cell_info->configured_tac,
            sizeof(x2ap_tac_t));
    }
  
#ifndef LINUX_PC_TEST
    /* Meas timing config */
    x2ap_build_x2ap_meas_timing_config(&p_x2ap_srvd_nr_cell_info->meas_timing_config,
        &p_rrm_srvd_nr_cell_info->x2ap_meas_timing_config[0],
        &p_rrm_srvd_nr_cell_info->meas_timing_config_count);
#endif

    l3_memcpy_wrapper(&p_rrm_srvd_nr_cell_info->broadcast_plmn_info, 
        &p_x2ap_srvd_nr_cell_info->broadcast_plmn_info,
        sizeof(x2ap_broadcast_plmn_t));
    p_rrm_srvd_nr_cell_info->choice_nr_mode.bitmask =
        p_x2ap_srvd_nr_cell_info->choice_nr_mode.bitmask;

    X2AP_TRACE(X2AP_DETAILED,"in rrm_srvd_nr_cell_info choice_nr_mode.bitmask is [%d]\n",
                p_x2ap_srvd_nr_cell_info->choice_nr_mode.bitmask);
    if ( X2AP_NR_MODE_FDD_PRESENT &
        p_x2ap_srvd_nr_cell_info->choice_nr_mode.bitmask )
    {
       p_rrm_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_ul_freq_info.nr_earfcn =
           p_x2ap_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_ul_freq_info.nr_earfcn; 

       p_rrm_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_ul_freq_info.num_freq_bands =
           p_x2ap_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_ul_freq_info.num_freq_bands; 

       l3_memcpy_wrapper(&p_rrm_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_ul_freq_info.freq_band_info, 
               &p_x2ap_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_ul_freq_info.freq_band_info
               , sizeof(x2ap_nr_freq_band_info_t));
       l3_memcpy_wrapper(&p_rrm_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_dl_freq_info.freq_band_info, 
               &p_x2ap_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_dl_freq_info.freq_band_info
               , sizeof(x2ap_nr_freq_band_info_t));
       p_rrm_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_dl_freq_info.nr_earfcn =
           p_x2ap_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_dl_freq_info.nr_earfcn; 

       p_rrm_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_dl_freq_info.num_freq_bands =
           p_x2ap_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_dl_freq_info.num_freq_bands; 

       l3_memcpy_wrapper(&p_rrm_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_dl_tx_bdwidth, 
               &p_x2ap_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_dl_tx_bdwidth,
               sizeof(x2ap_nr_tx_bandwidth_t));

       l3_memcpy_wrapper(&p_rrm_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_ul_tx_bdwidth, 
               &p_x2ap_srvd_nr_cell_info->choice_nr_mode.nr_fdd_info.nr_ul_tx_bdwidth,
               sizeof(x2ap_nr_tx_bandwidth_t));
    }
    else if ( X2AP_NR_MODE_TDD_PRESENT &
        p_x2ap_srvd_nr_cell_info->choice_nr_mode.bitmask )
    {
        l3_memcpy_wrapper(&p_rrm_srvd_nr_cell_info->choice_nr_mode.nr_tdd_info.nr_freq_info.freq_band_info, 
                &p_x2ap_srvd_nr_cell_info->choice_nr_mode.nr_tdd_info.nr_freq_info.freq_band_info
                , sizeof(x2ap_nr_freq_band_info_t));
        p_rrm_srvd_nr_cell_info->choice_nr_mode.nr_tdd_info.nr_freq_info.nr_earfcn =
            p_x2ap_srvd_nr_cell_info->choice_nr_mode.nr_tdd_info.nr_freq_info.nr_earfcn;

        p_rrm_srvd_nr_cell_info->choice_nr_mode.nr_tdd_info.nr_freq_info.num_freq_bands =
            p_x2ap_srvd_nr_cell_info->choice_nr_mode.nr_tdd_info.nr_freq_info.num_freq_bands;

        l3_memcpy_wrapper(&p_rrm_srvd_nr_cell_info->choice_nr_mode.nr_tdd_info.nr_tx_bandwidth, 
                &p_x2ap_srvd_nr_cell_info->choice_nr_mode.nr_tdd_info.nr_tx_bandwidth, 
                sizeof(x2ap_nr_tx_bandwidth_t));
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR,"Neither TDD nor FDD! ");
    }

    X2AP_UT_TRACE_EXIT();
}

/******************************************************************************
 * Function Name  : x2ap_build_rrm_srvd_nr_cell_info
 * Inputs         : p_x2ap_srvd_nr_cell_info - pointer to x2ap_srvd_nr_cell_info_t
 *                  p_rrm_srvd_nr_cell_info -- pointer to x2ap_rrm_srvd_nr_cell_info_t
 *                  
 * Outputs        : p_rrm_srvd_nr_cell_info Message for rrm interface
 * Returns        : void 
 * Description    : This function is called to build rrm_srvd_nr_cell_info
 ******************************************************************************/
void
x2ap_build_nr_neigh_cell_arry
(
    x2ap_nr_neigh_cell_arry_t *p_x2ap_nr_neigh_cell_info,
    x2ap_nr_neigh_cell_arry_t *p_rrm_nr_neigh_cell_arry
)
{
    U16 freq_band_count = X2AP_NULL;
    U16 sul_band_count = X2AP_NULL;

    X2AP_UT_TRACE_ENTER();
    
    p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_pci = 
            p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_pci;

    X2AP_TRACE(X2AP_DETAILED,"neighbour served cell nr pci =%d\n",
        p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_pci);
        
    l3_memcpy_wrapper(&p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_cell_id, 
        &p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_cell_id,
        sizeof(x2ap_nr_ecgi_t));

    if(X2AP_FIVE_GS_TAC_PRESENT & 
            p_x2ap_nr_neigh_cell_info->nr_neigh_info.bitmask)
    {
        X2AP_TRACE(X2AP_DETAILED,"5GS TAC present\n ");
        p_rrm_nr_neigh_cell_arry->nr_neigh_info.bitmask |=
            X2AP_FIVE_GS_TAC_PRESENT;

        l3_memcpy_wrapper(&p_rrm_nr_neigh_cell_arry->nr_neigh_info.fivegs_tac,
            &p_x2ap_nr_neigh_cell_info->nr_neigh_info.fivegs_tac,
            sizeof(x2ap_five_gs_tac_t));
    }

    if(X2AP_CONFIGURED_TAC_PRESENT & 
            p_x2ap_nr_neigh_cell_info->nr_neigh_info.bitmask)
    {
        p_rrm_nr_neigh_cell_arry->nr_neigh_info.bitmask |=
            X2AP_CONFIGURED_TAC_PRESENT;

        l3_memcpy_wrapper(&p_rrm_nr_neigh_cell_arry->nr_neigh_info.configured_tac,
            &p_x2ap_nr_neigh_cell_info->nr_neigh_info.configured_tac,
            sizeof(x2ap_tac_t));
    }

    p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_freq_info.nr_earfcn =
        p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_freq_info.nr_earfcn;

    p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_freq_info.num_freq_bands =
        p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_freq_info.num_freq_bands;

    for(freq_band_count = X2AP_NULL; freq_band_count <
            p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_freq_info.num_freq_bands;
            freq_band_count++)
    {
        p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_freq_info.freq_band_info[freq_band_count].
            freq_band_indicator_nr =
            p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_freq_info.freq_band_info[freq_band_count].
                freq_band_indicator_nr;

        p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_freq_info.freq_band_info[freq_band_count].
            num_supported_sul_band =
            p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_freq_info.freq_band_info[freq_band_count].
                num_supported_sul_band;

        for(sul_band_count = X2AP_NULL; sul_band_count <
                p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_freq_info.freq_band_info[freq_band_count].
                num_supported_sul_band;
                sul_band_count++)
        {
            X2AP_TRACE(X2AP_DETAILED,"SUL BAND SUPPORTED");

            p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_freq_info.freq_band_info[freq_band_count].
                supported_sul_freq_band_indicator_nr[sul_band_count] =
                p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_freq_info.freq_band_info[freq_band_count].
                supported_sul_freq_band_indicator_nr[sul_band_count];  
        } 
     }

     if ( X2AP_NR_ARFCN_SUL_PRESENT &
             p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_freq_info.bitmask )
     {

         p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_freq_info.bitmask |=
             X2AP_NR_ARFCN_SUL_PRESENT;
         p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_freq_info.nr_earfcn_sul =
             p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_freq_info.nr_earfcn_sul;
     }

     if ( X2AP_SUL_INFORMATION_PRESENT &
             p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_freq_info.bitmask )
     {
         p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_freq_info.bitmask |=
             X2AP_SUL_INFORMATION_PRESENT;

         p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_freq_info.nr_sul_information =
             p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_freq_info.nr_sul_information;
     }

     l3_memcpy_wrapper(&p_rrm_nr_neigh_cell_arry->nr_neigh_info.nr_neigh_Mode_Info,
        &p_x2ap_nr_neigh_cell_info->nr_neigh_info.nr_neigh_Mode_Info,
        sizeof(x2ap_nr_neigh_mode_info_t));   

    X2AP_UT_TRACE_EXIT();    
}

/******************************************************************************
 * Function Name  : x2ap_build_nr_x2_link_up_ind
 * Inputs         : x2ap_gb_context_t     *p_x2ap_gl_ctx,
 *                  p_x2_setup_resp - pointer to x2_setup_resp_t
 *                  p_rrm_link_up_ind -- pointer to x2ap_rrm_x2_link_up_ind_st
 *                  num_act_cell - num of activated cell
 *                  p_act_cell_id - cell id
 *                  p_x2ap_gl_ctx - pointer to X2AP global context
 * Outputs        : p_rrm_link_up_ind Message for rrm interface
 * Returns        : void 
 * Description    : This function is called to build link UP indication
 ******************************************************************************/
void
x2ap_build_nr_x2_link_up_ind
(
    x2ap_gb_context_t              *p_x2ap_gl_ctx,
    en_dc_x2_setup_req_t           *p_x2_setup_resp,
    x2ap_rrm_x2_link_up_ind_st     *p_rrm_link_up_ind
)
{
    U16                         srvd_cell_count = RRC_NULL;
    U8                          sgnb_index = X2AP_NULL;
    
    X2AP_UT_TRACE_ENTER();

    p_rrm_link_up_ind->bitmask |= RRM_X2AP_NR_CELL_INFO_PRESENT;

    /* Fill the global gNB Id */
    p_rrm_link_up_ind->nr_cell_info.global_gnb_id =
        p_x2_setup_resp->nr_cell_info.global_gnb_id;

    for ( sgnb_index = RRC_NULL; sgnb_index < MAX_PEER_ENB; sgnb_index++ )
    {
        if ( X2_ENB_COMM_INFO_DEFAULT_SGNB_PRESENT &
           p_x2ap_gl_ctx->peer_enb_cntxt_arr[sgnb_index].enb_comm_info.bitmask )
        {
            if ( !memcmp_wrapper(&p_x2ap_gl_ctx->peer_enb_cntxt_arr[sgnb_index].gb_gnb_id,
                        &p_x2_setup_resp->nr_cell_info.global_gnb_id,
                        sizeof(x2_gb_gnb_id_t)))
            {
                p_rrm_link_up_ind->nr_cell_info.default_sgnb =
                    p_x2ap_gl_ctx->peer_enb_cntxt_arr[sgnb_index].enb_comm_info.default_sgnb;
                X2AP_TRACE(X2AP_DETAILED,"X2_ENB_COMM_INFO_DEFAULT_SGNB_PRESENT and value of default_sgnb %d",
                        p_rrm_link_up_ind->nr_cell_info.default_sgnb);
                break;
            }
        }
        else
        {
            p_rrm_link_up_ind->nr_cell_info.default_sgnb = RRC_NULL;
            X2AP_TRACE(X2AP_DETAILED,"X2_ENB_COMM_INFO_DEFAULT_SGNB_ not present and value of default_sgnb %d",
                p_rrm_link_up_ind->nr_cell_info.default_sgnb);
        }
    }
    
    p_rrm_link_up_ind->nr_cell_info.num_served_cell =
        p_x2_setup_resp->nr_cell_info.num_served_nr_cell;
    
    //p_rrm_link_up_ind->nr_cell_info.num_neigh_cell =
    //    p_x2_setup_resp->nr_cell_info.num_neigh_cell;

    /* peer served cell count */
    X2AP_TRACE(X2AP_DETAILED,"Served cell count in endc x2 setup resp is [%d] \n",p_x2_setup_resp->nr_cell_info.num_served_nr_cell);

    for(srvd_cell_count = X2AP_NULL; srvd_cell_count <
            p_x2_setup_resp->nr_cell_info.num_served_nr_cell;
            srvd_cell_count++)
    {
        x2ap_build_rrm_srvd_nr_cell_info(&p_x2_setup_resp->nr_cell_info.served_nr_cell_arr[srvd_cell_count],
            &p_rrm_link_up_ind->nr_cell_info.served_cell_info[srvd_cell_count]);                         
    }

    p_rrm_link_up_ind->nr_cell_info.num_neigh_cell = 0;
#if 0
    /* peer neighbour served cell count */
    p_rrm_link_up_ind->nr_cell_info.num_neigh_cell = 
        p_x2_setup_resp->nr_cell_info.num_neigh_cell;

    for(neigh_cell_count = X2AP_NULL; neigh_cell_count <
            p_x2_setup_resp->nr_cell_info.num_neigh_cell; neigh_cell_count++)
    {
        p_rrm_link_up_ind->nr_cell_info.bitmask |= NR_NEIGHBOUR_INFO_PRESENT;
        X2AP_TRACE(X2AP_DETAILED,"NR_NEIGHBOUR_INFO_PRESENT =%d\n",
            p_rrm_link_up_ind->nr_cell_info.bitmask);

        x2ap_build_nr_neigh_cell_arry(&p_x2_setup_resp->nr_cell_info.nr_neigh_cell_arr[neigh_cell_count],
            p_rrm_link_up_ind->nr_cell_info.nr_neigh_cell_arr[neigh_cell_count]);  
    }
#endif 

    X2AP_UT_TRACE_EXIT();
}

/******************************************************************************
 * Function Name  : x2ap_send_nr_link_up_ind_to_rrm
 * Inputs         : x2ap_gb_context_t          *p_x2ap_gl_ctx,
 *                  p_x2_setup_req - pointer to x2_setup_req_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when x2 setup link is UP to send
 *                  to rrm.
 ******************************************************************************/
x2ap_return_et
x2ap_send_nr_link_up_ind_to_rrm
(
    x2ap_gb_context_t     *p_x2ap_gl_ctx,
    en_dc_x2_setup_req_t  *p_x2_setup_req
)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    static x2ap_rrm_x2_link_up_ind_st      rrm_link_up_ind;

    X2AP_UT_TRACE_ENTER();
    
    X2AP_MEMSET(&rrm_link_up_ind,X2AP_NULL,sizeof(x2ap_rrm_x2_link_up_ind_st));

    /*
     * Send X2 link down to RRM
     */
    x2ap_build_nr_x2_link_up_ind(p_x2ap_gl_ctx,p_x2_setup_req,
                              &rrm_link_up_ind);

    if ( RRC_FAILURE == 
            rrc_x2apRrm_il_send_x2ap_rrm_x2_link_up_ind_st((&rrm_link_up_ind),
            RRC_X2AP_MODULE_ID,RRC_RRM_MODULE_ID, RRC_NULL, RRC_NULL) )

    {
        X2AP_TRACE(X2AP_ERROR,"Build and send returned failure for link up ind to RRM ");
        return X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/* MENB CHANGES - END */
#endif

/******************************************************************************
 * Function Name  : x2ap_send_link_up_ind_to_rrm
 * Inputs         : p_x2_setup_resp - pointer to x2_setup_resp_t
 *                  p_x2ap_gl_ctx - pointer to  X2AP global context 
 *                  num_act_cell - num of activated celll
 *                  p_act_cell_id - activated cell id
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when x2 setup link is UP to send
 *                  to rrm.
 ******************************************************************************/
x2ap_return_et x2ap_send_link_up_ind_to_rrm(
        x2_setup_resp_t  *p_x2_setup_resp)
        /*SPR 17777 +-*/
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    static x2ap_rrm_x2_link_up_ind_st      rrm_link_up_ind;
    X2AP_UT_TRACE_ENTER();
    
    X2AP_MEMSET(&rrm_link_up_ind,X2AP_NULL,sizeof(x2ap_rrm_x2_link_up_ind_st));
    /* Send X2 link down to RRM */
    x2ap_build_x2_link_up_ind(p_x2_setup_resp,
                              &rrm_link_up_ind);
    /*SPR 17777 +-*/

    if (RRC_FAILURE == 
            rrc_x2apRrm_il_send_x2ap_rrm_x2_link_up_ind_st((&rrm_link_up_ind),
            RRC_X2AP_MODULE_ID,RRC_RRM_MODULE_ID, 0, RRC_NULL))

    {
        return X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/* MENB CHANGES - START */
/******************************************************************************
 * Function Name  : x2_setup_resp_evt_hdl
 * Inputs         : p_msg_ptr  -    Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when x2 setup response event is
 *                  received from peer eNB.
 ******************************************************************************/
x2ap_return_et
x2_setup_resp_evt_hdl
(
    U8                 *p_msg_ptr,
    x2ap_gb_context_t  *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t *p_enb_ctx
)
{
    x2ap_return_et                 retVal = X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    if (  X2AP_TRUE == p_enb_ctx->peer_enb_type )
    {
        /*
         * Function handler for EN DC X2 Setup 
         */
        retVal = en_dc_x2_setup_resp_evt_handler(
                p_msg_ptr,p_x2ap_gl_ctx,p_enb_ctx);
    }
    else
    {
    /* MENB CHANGES - END */
    #endif
        /*
         * Function handler for X2 Setup
         */
        retVal = x2_setup_resp_evt_handler(
                p_msg_ptr,p_x2ap_gl_ctx,p_enb_ctx);
    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    }
    /* MENB CHANGES - END */
    #endif

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
/******************************************************************************
 * Function Name  : en_dc_x2_setup_resp_evt_handler
 * Inputs         : p_msg_ptr  -    Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when x2 setup response event is
 *                  received from peer eNB.
 ******************************************************************************/
x2ap_return_et
en_dc_x2_setup_resp_evt_handler
(
    U8                         *p_msg_ptr,
    x2ap_gb_context_t          *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t    *p_enb_ctx
)
{
    x2ap_return_et                  retVal = X2AP_FAILURE;
    U16                             num_act_cell = X2AP_NULL;
    U16                             act_cell_id[MAX_NUM_CELL_IN_X2AP] ={X2AP_NULL};

    static en_dc_x2_setup_resp_t    x2_setup_resp;
    sctp_gb_gnb_id_search_node_t    *p_gnb_id_node = X2AP_P_NULL;
    x2ap_peer_enb_msg_t             *p_peer_enb_msg = X2AP_P_NULL;
    OSCTXT                          asn1_ctx;

    X2AP_UT_TRACE_ENTER();

    /*
     * Received pointer should not be NULL
     */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;
    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);
    X2AP_MEMSET(&x2_setup_resp,X2AP_NULL,sizeof(en_dc_x2_setup_resp_t));

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: ENDC X2 SETUP RESPONSE Received");
    
    /*
     * Check if there is any cell is operational or not
     */
    x2ap_check_active_cell(&num_act_cell,
                           act_cell_id,
                           p_x2ap_gl_ctx);

    /*
     * Decode the ASN request
     */
    retVal = x2ap_decode_endc_x2setup_resp(
        p_x2ap_gl_ctx,
        p_enb_ctx,
        &x2_setup_resp,
        (p_peer_enb_msg)->p_msg,
        &(p_peer_enb_msg->msg_len),
        &asn1_ctx
        );
    if ( X2AP_SUCCESS == retVal )
    {
        X2AP_TRACE(X2AP_INFO,"---ENDC X2AP Response Decoding Success---");
        /*
         * Stop X2 Setup Response guard Timer
         */
        x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;

        /*
         * change state to ACTIVE
         */          
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_ACTIVE_ST;
        p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = X2AP_NULL;
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_NULL;
        p_enb_ctx->op_status = OPERATIONAL;

        /*
         * update the cell info in the global context
         */
        x2ap_update_nr_cell_info_for_resp(&x2_setup_resp,
                                       num_act_cell,
                                       act_cell_id,
                                       p_x2ap_gl_ctx);

        p_enb_ctx->bitmask |= X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT;
        p_enb_ctx->gb_gnb_id = x2_setup_resp.nr_cell_info.global_gnb_id;
        p_gnb_id_node = (sctp_gb_gnb_id_search_node_t *)x2ap_st_find_tree_node(
                        &(p_x2ap_gl_ctx->enb_ctxt_trees.gnb_id_search_tree),
                        &(p_enb_ctx->gb_gnb_id));
        if ( !p_gnb_id_node )
        {
            p_gnb_id_node = x2ap_mem_get(sizeof(sctp_gb_gnb_id_search_node_t));
            if ( PNULL != p_gnb_id_node )
            {
                p_gnb_id_node->gb_gnb_id = p_enb_ctx->gb_gnb_id;
                p_gnb_id_node->p_enb_context = p_enb_ctx;
                x2ap_st_insert_node(
                        &(p_x2ap_gl_ctx->enb_ctxt_trees.gnb_id_search_tree),
                        &(p_gnb_id_node->nodeAnchor));
            }
        }

        /*
         * Send X2 Link UP indication to UECC
         */
        if ( X2AP_FAILURE ==
                x2ap_build_and_send_nr_status_ind(p_enb_ctx,
                    p_x2ap_gl_ctx,
                    X2AP_LINK_UP,
                    x2_setup_resp.nr_cell_info.num_served_nr_cell,
                    &(x2_setup_resp.nr_cell_info.served_nr_cell_arr[X2AP_NULL]),
                    p_x2ap_gl_ctx->total_ue_supported) )
        {
            X2AP_TRACE(X2AP_WARNING, "%s:"
                    " rrc_send_message send failed x2ap status \
                    msg ind",
                    __FUNCTION__);
        }

        /*
         * Update the x2ap global context to save the served cell info of the peer EnodeB
         */
        /*x2ap_served_cell_info_update(p_x2ap_gl_ctx,
                X2AP_LINK_UP,
                p_enb_ctx->peer_enodeb_id,
                (en_dc_x2_setup_req_t *)&x2_setup_resp);      */
        x2ap_served_nr_cell_info_update(p_x2ap_gl_ctx,
                X2AP_LINK_UP,
                p_enb_ctx->peer_enodeb_id,
                (en_dc_x2_setup_req_t *)&x2_setup_resp); 

        /* set procedure sm context state to ACTIVE */
        x2ap_active_procedure_sm_ctx_state(p_enb_ctx); 
       
        //X2AP_TRACE(X2AP_INFO,
        //                        "X2AP_MSG: ENDC X2 SETUP RESPONSE");

         retVal = x2ap_send_link_up_ind_to_oam(p_enb_ctx);
         if ( X2AP_FAILURE == retVal )
         {
            X2AP_TRACE(X2AP_ERROR,"returned value of x2ap_send_link_up_ind_to_oam is [%d]", retVal);
         }
         else
         {
            X2AP_TRACE(X2AP_DETAILED,"Sent x2ap_send_link_up_ind_to_oam successfully");
         }

         /*
          * Send X2 Link UP indication to RRM
          */
         retVal = x2ap_send_nr_link_up_ind_to_rrm(p_x2ap_gl_ctx, (en_dc_x2_setup_req_t *)&x2_setup_resp);
         if ( X2AP_FAILURE == retVal )
         {
            X2AP_TRACE(X2AP_ERROR,"returned value of x2ap_send_link_up_ind_to_rrm is [%d]", retVal);
         }
         else
         {
            X2AP_TRACE(X2AP_DETAILED,"Sent x2ap_send_link_up_ind_to_rrm successfully");
         }
         ALARM_MSG_L3(RRC_MODULE_ID, RRC_X2AP_CONNECTION_SUCCESS_ALARM_ID, NOTIFICATION, INVALID_ERR_CAUSE_ID);

        /*
         * Invoke UE Associated sig msg FSM
         */
        x2ap_ue_asso_sig_sm_entry(X2AP_UE_ASSO_SIG_X2_LINK_UP_EV,
                                   p_msg_ptr,
                                   p_x2ap_gl_ctx,
                                   p_enb_ctx, X2AP_NULL);  
    }
    else
    {
        /*
         * Send Error Indication to peer gNB
         */
        X2AP_TRACE(X2AP_INFO,"---Error Indication Detected---");
        retVal = X2AP_SUCCESS;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/* MENB CHNAGES - END */
#endif

/******************************************************************************
 * Function Name  : x2_setup_resp_evt_handler
 * Inputs         : p_msg_ptr  -    Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when x2 setup response event is
 *                  received from peer eNB.
 ******************************************************************************/
x2ap_return_et x2_setup_resp_evt_handler(U8                 *p_msg_ptr,
                                     x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                     x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et                 retVal = X2AP_FAILURE;
    U16                            num_act_cell = 0;
    U16                            act_cell_id[MAX_NUM_CELL_IN_X2AP] ={0};
    static x2_setup_resp_t               x2_setup_resp;
    x2ap_peer_enb_msg_t          *p_peer_enb_msg = X2AP_P_NULL;
    /*x2ap_rrm_x2_link_up_ind_st     x2_link_up_ind={0};*/
    U16                            count = 0;
    /*x2ap_status_msg_ind_t         *msg_p = X2AP_P_NULL;*/     
    sctp_gb_enb_id_search_node_t *p_enb_id_node = X2AP_P_NULL;
 

	/*SPR_19067_FIX_START*/
    OSCTXT                       asn1_ctx;
    /*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;
    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_setup_resp,X2AP_NULL,sizeof(x2_setup_resp_t));

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP RESPONSE Received");

    /* Check if there is any cell is operational or not */
    x2ap_check_active_cell(&num_act_cell,
                           act_cell_id,
                           p_x2ap_gl_ctx);

    /* Decode the ASN request */
    retVal = x2ap_decode_x2setup_resp(
            p_x2ap_gl_ctx,
            p_enb_ctx,
            &x2_setup_resp,
            (p_peer_enb_msg)->p_msg,
            /*SPR_19067_FIX_START*/
            &(p_peer_enb_msg->msg_len),
            &asn1_ctx);
            /*SPR_19067_FIX_STOP*/

    if (X2AP_SUCCESS == retVal)
    {
        /* Stop X2 Setup Response guard Timer */
        x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;
        /* change state to ACTIVE */
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_ACTIVE_ST;
        p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = 0;
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = 0;
        p_enb_ctx->op_status = OPERATIONAL;

        /* update the cell info in the global context */
        x2ap_update_cell_info_for_resp(&x2_setup_resp,
                                       num_act_cell,
                                       act_cell_id,
                                       p_x2ap_gl_ctx);
        p_enb_ctx->x2ap_gu_grp_id.num_gu_group_id = x2_setup_resp.num_gu_group_id;
        for (count = 0; count < p_enb_ctx->x2ap_gu_grp_id.num_gu_group_id; count ++)
        {
            X2AP_MEMCPY(&(p_enb_ctx->x2ap_gu_grp_id.group_id_arr[count]), 
                    &(x2_setup_resp.group_id_arr[count]), sizeof(x2ap_gu_group_id_t));
        }        

        p_enb_ctx->gb_enb_id = x2_setup_resp.global_enb_id;
        p_enb_id_node = (sctp_gb_enb_id_search_node_t *)x2ap_st_find_tree_node(
                        &(p_x2ap_gl_ctx->enb_ctxt_trees.enb_id_search_tree),
                        &(p_enb_ctx->gb_enb_id));
        if (!p_enb_id_node)
        {
            p_enb_id_node = x2ap_mem_get(sizeof(sctp_gb_enb_id_search_node_t));
            if (PNULL != p_enb_id_node)
            {
                p_enb_id_node->gb_enb_id = p_enb_ctx->gb_enb_id;
                p_enb_id_node->p_enb_context = p_enb_ctx;
                x2ap_st_insert_node(
                        &(p_x2ap_gl_ctx->enb_ctxt_trees.enb_id_search_tree),
                        &(p_enb_id_node->nodeAnchor));
            }
        }
        /* Send X2 Link UP indication to UECC */
        if(X2AP_FAILURE ==
                x2ap_build_and_send_status_ind(p_enb_ctx,
                    X2AP_LINK_UP,
                    x2_setup_resp.num_served_cell,
                    /* CSR_00040799 Fix Start */
                    &(x2_setup_resp.served_cell_arr[0]),
                    p_x2ap_gl_ctx->total_ue_supported))
                    /* CSR_00040799 Fix End */
        {
            X2AP_TRACE(X2AP_WARNING, "%s:"
                    " rrc_send_message send failed x2ap status \
                    msg ind",
                    __FUNCTION__);
        }
        
        /*RSU Changes Start
        Update the x2ap global context to save the served cell info of the peer EnodeB*/
        x2ap_served_cell_info_update(p_x2ap_gl_ctx,
                X2AP_LINK_UP,
                p_enb_ctx->peer_enodeb_id,
                (x2_setup_req_t*)&x2_setup_resp);        
        /*RSU Changes End */

        /* set procedure sm context state to ACTIVE */
        x2ap_active_procedure_sm_ctx_state(p_enb_ctx); 
        
        X2AP_TRACE(X2AP_INFO,
                                "X2AP_MSG: X2 SETUP RESPONSE");

        if(p_x2ap_gl_ctx->is_son_present)
        {
	    /* Send X2 Link UP indication to SON */
            retVal = x2ap_send_link_up_ind_to_son(&x2_setup_resp,
                                              p_x2ap_gl_ctx,
                                              /*SPR 17777 +-*/
					      &p_enb_ctx->enb_comm_info);
            if(!retVal)
            {
                X2AP_TRACE(X2AP_ERROR,"Sending enb link up indication failed.\n");        
            }
        }
        // else rm by wood for x2ap ho test
        {
         retVal = x2ap_send_link_up_ind_to_oam(p_enb_ctx);
         /* Coverity_fix_81362_start */
         if(X2AP_FAILURE == retVal)
         {
            X2AP_TRACE(X2AP_DETAILED,"returned value of x2ap_send_link_up_ind_to_oam is [%d]", retVal);
         }
         /* Coverity_fix_81362_end */
        /* Send X2 Link UP indication to RRM */
        retVal = x2ap_send_link_up_ind_to_rrm(&x2_setup_resp);
        /*SPR 17777 +-*/
        }
        /*CSR 51443 Start*/
        ALARM_MSG_L3(RRC_MODULE_ID, RRC_X2AP_CONNECTION_SUCCESS_ALARM_ID, NOTIFICATION, INVALID_ERR_CAUSE_ID);
        /*CSR 51443 Stop*/

        /*x2ap-start*/
        /*Invoke UE Associated sig msg FSM*/
        x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_X2_LINK_UP_EV,
                                   p_msg_ptr,
                                   p_x2ap_gl_ctx,
                                   p_enb_ctx, X2AP_NULL);  
        /*x2ap-end*/
    }
    else
    {
        /* Send Error Indication to peer eNB */
        X2AP_TRACE(X2AP_INFO,"---Error Indication Detected---");
        retVal = X2AP_SUCCESS;
    }

	/*SPR_19067_FIX_START*/
    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);
    /*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/******************************************************************************
 * Function Name  : x2setup_req_idle_hdl
 * Inputs         : p_msg_ptr -     Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when x2 setup request event is
 *                  received from peer eNB.
 ******************************************************************************/
x2ap_return_et x2setup_req_idle_hdl  (U8                 *p_msg_ptr,
                                     x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                     x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    U16                   num_act_cell = 0;
    U16                   act_cell_id[MAX_NUM_CELL_IN_X2AP]={0};
    x2ap_bool_et              time_to_wait_flag = X2AP_FALSE;  
    rrc_x2ap_error_indication_t error_indication;
    
    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);
    /* if the cell setup guard timer is running */

    X2AP_MEMSET(&error_indication,X2AP_NULL,sizeof(rrc_x2ap_error_indication_t));

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP REQUEST Received");

    if(p_x2ap_gl_ctx->config_param.timer_conf_info.bitmask
            & X2AP_TIME_TO_WAIT_INDEX_PRESENT)
    {
        time_to_wait_flag = X2AP_TRUE;
    } 
    if (p_x2ap_gl_ctx->cell_up_guard_tim_id)
    {
        /* Fill Cause in Error Indication */
        error_indication.cause.t = T_x2ap_Cause_radioNetwork;
        error_indication.cause.u.radioNetwork = x2ap_cell_not_available;
        
        /* send failure response with time to wait */
        retVal = x2ap_build_x2setup_fail_resp(p_x2ap_gl_ctx,
                                              p_enb_ctx,
                                              time_to_wait_flag,
                                              &error_indication,//X2AP_P_NULL,
                                              X2AP_P_NULL);
    }
    else
    {
        /* Check if there is any cell is operational or not */
        x2ap_check_active_cell(&num_act_cell,
                               act_cell_id,
                               p_x2ap_gl_ctx);

        /* if atleast one cell is active */
        if (!num_act_cell)
        {
            /* Fill Cause in Error Indication */
            error_indication.cause.t = T_x2ap_Cause_radioNetwork;
            error_indication.cause.u.radioNetwork = x2ap_cell_not_available;
            
            /* send failure response with time to wait */
            retVal = x2ap_build_x2setup_fail_resp(p_x2ap_gl_ctx,
                                                  p_enb_ctx,
                                                  time_to_wait_flag,
                                                  &error_indication,//X2AP_P_NULL,
                                                  X2AP_P_NULL);
        }
        else
        {
            retVal = x2_setup_req_evt_hdl(p_msg_ptr,
                                          p_x2ap_gl_ctx,
                                          p_enb_ctx);
        }
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/*SPR_18154_START*/
/******************************************************************************
 * Function Name  : x2_setup_req_w_x2_setup_resp_evt_hdl 
 * Inputs         : p_msg_ptr -     Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when x2 setup request event is 
 *                  received from peer eNB in X2_SETUP_W_X2_SETUP_RESP_ST state.
 ******************************************************************************/
x2ap_return_et x2_setup_req_w_x2_setup_resp_evt_hdl(
                                     U8                 *p_msg_ptr,
                                     x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                     x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    U16                   num_act_cell = 0;
    U16                   act_cell_id[MAX_NUM_CELL_IN_X2AP]={0};
    static x2_setup_req_t       x2_setup_req;
    x2ap_peer_enb_msg_t  *p_peer_enb_msg = X2AP_P_NULL;
    sctp_gb_enb_id_search_node_t *p_enb_id_node = X2AP_P_NULL;
    rrc_x2ap_error_indication_t error_indication;
    x2ap_error_ind_bool_t        send_err_ind;

	/*SPR_19067_FIX_START*/
    OSCTXT                       asn1_ctx;
    /*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;
    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_setup_req,X2AP_NULL,sizeof(x2_setup_req_t));
    X2AP_MEMSET(&error_indication,X2AP_NULL,
                    sizeof(rrc_x2ap_error_indication_t));
    X2AP_MEMSET(&send_err_ind,X2AP_NULL,
                    sizeof(x2ap_error_ind_bool_t));

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP REQUEST Received");
    
    /* Check if there is any cell is operational or not */
    x2ap_check_active_cell(&num_act_cell,
                           act_cell_id,
                           p_x2ap_gl_ctx);
    if (!num_act_cell)
    {
        x2ap_bool_et              time_to_wait_flag = X2AP_FALSE;  

        if(p_x2ap_gl_ctx->config_param.timer_conf_info.bitmask
                & X2AP_TIME_TO_WAIT_INDEX_PRESENT)
        {
            time_to_wait_flag = X2AP_TRUE;
        } 

         /* Fill Cause in Error Indication */
        error_indication.cause.t = T_x2ap_Cause_radioNetwork;
        error_indication.cause.u.radioNetwork = x2ap_cell_not_available;
        
        /* send failure response with time to wait */
        retVal = x2ap_build_x2setup_fail_resp(p_x2ap_gl_ctx,
                p_enb_ctx,
                time_to_wait_flag,
                &error_indication,//X2AP_P_NULL,
                X2AP_P_NULL);

        /*
         * change state to IDLE
         */
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_IDLE_ST;
        p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = X2AP_NULL;
        
        if (X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId 
        || X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id)
        {
            if (X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId)
            {
                /*
                * Stop X2 Setup Response guard Timer
                */
                x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
                p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;
            }

            if (X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id)
            {
                /*
                * Stop X2 Setup time to wait guard Timer
                */
                x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id);
                p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
            }
        }

        /*SPR_21694_START*/
        /* Code Removed */
        X2AP_TRACE(X2AP_INFO, "%s: Active cell count is 0."
                " X2Setup Failure Resp Sent with time_to_wait.",
                __FUNCTION__);
        /*SPR_21694_END*/

        return retVal;
    }


    /* Decode the ASN request */
    retVal = x2ap_decode_x2setup_req(
                p_x2ap_gl_ctx,
                p_enb_ctx,
                &x2_setup_req,
                (p_peer_enb_msg)->p_msg,
                &(p_peer_enb_msg->msg_len),
                &error_indication,
				/*SPR_19067_FIX_START*/
                &send_err_ind,
                &asn1_ctx);
                /*SPR_19067_FIX_STOP*/

    if (X2AP_SUCCESS == retVal)
    { 
        U32 count = 0;
        if (X2AP_P_NULL !=  p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId
                || (X2AP_P_NULL !=  
                    p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id))
        {
            if (X2AP_P_NULL !=  p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id)
            {
                x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id);
                p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
            }
            if (X2AP_P_NULL !=  p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId)
            {
                x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
                p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = 0;
            }
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_ACTIVE_ST;
            p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = 0;
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = 0;
            p_enb_ctx->op_status = OPERATIONAL;
            p_enb_ctx->is_x2setup_sent = X2AP_FALSE;
        }

        /* BUG 11545 Fix Start */
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_ACTIVE_ST;
        p_enb_ctx->op_status = OPERATIONAL;
        /* BUG 11545 Fix Stop */

        /* update the cell info in the global context */
        x2ap_update_cell_info_for_req(&x2_setup_req,
                num_act_cell,
                act_cell_id,
                p_x2ap_gl_ctx);
        p_enb_ctx->x2ap_gu_grp_id.num_gu_group_id = x2_setup_req.num_gu_group_id;
        for (count = 0; count < p_enb_ctx->x2ap_gu_grp_id.num_gu_group_id; count ++)
        {
            X2AP_MEMCPY(&(p_enb_ctx->x2ap_gu_grp_id.group_id_arr[count]),
                    &(x2_setup_req.group_id_arr[count]), sizeof(x2ap_gu_group_id_t));
        }

        p_enb_ctx->gb_enb_id = x2_setup_req.global_enb_id;
        p_enb_id_node = (sctp_gb_enb_id_search_node_t *)x2ap_st_find_tree_node(
                &(p_x2ap_gl_ctx->enb_ctxt_trees.enb_id_search_tree),
                &(p_enb_ctx->gb_enb_id));
        if (!p_enb_id_node)
        {    
            p_enb_id_node = x2ap_mem_get(sizeof(sctp_gb_enb_id_search_node_t));
            if (PNULL != p_enb_id_node)
            {
                p_enb_id_node->gb_enb_id = p_enb_ctx->gb_enb_id;                       
                p_enb_id_node->p_enb_context = p_enb_ctx;
                x2ap_st_insert_node(
                        &(p_x2ap_gl_ctx->enb_ctxt_trees.enb_id_search_tree),
                        &(p_enb_id_node->nodeAnchor));
            }
        }     
        /*RSU Changes Start*/
        /*Update the x2ap global context to save the served cell info of the peer EnodeB*/
         x2ap_served_cell_info_update(p_x2ap_gl_ctx,
                      X2AP_LINK_UP,
  		      p_enb_ctx->peer_enodeb_id,
              &x2_setup_req);        
         /*RSU Changes End */

        /* set procedure sm context state to ACTIVE */
        x2ap_active_procedure_sm_ctx_state(p_enb_ctx);
        
        X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP RESPONSE");
         /* Build and send x2 setup resp to peer enb */
         retVal = x2ap_build_x2setup_resp(num_act_cell,
                 act_cell_id,
                 p_x2ap_gl_ctx,
                 p_enb_ctx); 
         /* coverity 107103 +*/
         if(!retVal)
         {
             X2AP_TRACE(X2AP_ERROR,"Sending x2 setup resp to peer enb failed.\n");
         }
         /* coverity 107103 -*/

        p_enb_ctx->flag_for_consecutive_x2setup_resp_failure = X2AP_TRUE;

        /*if (p_enb_ctx->is_x2setup_sent == X2AP_FALSE)*/
        {
            /* Send X2 Link UP indication to UECC */
            if(X2AP_FAILURE ==
                    x2ap_build_and_send_status_ind(p_enb_ctx,
                        X2AP_LINK_UP,
                        x2_setup_req.num_served_cell,
                        /* CSR_00040799 Fix Start */
                        &(x2_setup_req.served_cell_arr[0]),
                        p_x2ap_gl_ctx->total_ue_supported))
                        /* CSR_00040799 Fix End */
            {
                X2AP_TRACE(X2AP_WARNING, "%s:"
                        " rrc_send_message send failed x2ap status \
                        msg ind",
                        __FUNCTION__);
            }
	    
	    if(p_x2ap_gl_ctx->is_son_present)
	    {
            	/* Send X2 Link UP indication to SON */
            	retVal = x2ap_send_link_up_ind_to_son((x2_setup_resp_t *)&x2_setup_req,
                	                              p_x2ap_gl_ctx,
						      &p_enb_ctx->enb_comm_info);
            	if(!retVal)
            	{
                	X2AP_TRACE(X2AP_ERROR,"Sending enb link up indication failed.\n");
            	}
        }
        else
        {
            retVal = x2ap_send_link_up_ind_to_oam(p_enb_ctx);
	    /* coverity 107104 +*/
	    if(!retVal)
	    {
		    X2AP_TRACE(X2AP_ERROR,"Sending X2 link up indication to OAM failed.\n");
	    }
	    /* coverity 107104 -*/
            /* Send X2 Link UP indication to RRM */
            retVal = x2ap_send_link_up_ind_to_rrm((x2_setup_resp_t *)&x2_setup_req);
        }

            /*Invoke UE Associated sig msg FSM*/
            x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_X2_LINK_UP_EV,
                    p_msg_ptr,
                    p_x2ap_gl_ctx,
                    p_enb_ctx, X2AP_NULL);  
        }

    }
    else
    {
        /* Send failure response without time to wait */
        retVal = x2ap_build_x2setup_fail_resp(p_x2ap_gl_ctx,
                                              p_enb_ctx,
                                              X2AP_FALSE,
                                              &error_indication,
                                              &send_err_ind);

        /* bug 114842 */
        /*
         * change state to IDLE
         */
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_IDLE_ST;
        p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = X2AP_NULL;
        
        if (X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId 
        || X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id)
        {
            if (X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId)
            {
                /*
                * Stop X2 Setup Response guard Timer
                */
                x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
                p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;
            }

            if (X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id)
            {
                /*
                * Stop X2 Setup time to wait guard Timer
                */
                x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id);
                p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
            }
        }                                    
    }

	/*SPR_19067_FIX_START*/
    rtFreeContext(&asn1_ctx);
   /*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/*SPR_18154_END*/

/******************************************************************************
 * Function Name  : x2_setup_req_evt_hdl
 * Inputs         : p_msg_ptr -     Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when x2 setup request event is 
 *                  received from peer eNB.
 ******************************************************************************/
x2ap_return_et x2_setup_req_evt_hdl (U8                 *p_msg_ptr,
                                     x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                     x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    U16                   num_act_cell = 0;
    U16                   act_cell_id[MAX_NUM_CELL_IN_X2AP]={0};
    static x2_setup_req_t       x2_setup_req;
    x2ap_peer_enb_msg_t  *p_peer_enb_msg = X2AP_P_NULL;
    sctp_gb_enb_id_search_node_t *p_enb_id_node = X2AP_P_NULL;
    rrc_x2ap_error_indication_t error_indication;
    x2ap_error_ind_bool_t        send_err_ind;


	/*SPR_19067_FIX_START*/
	OSCTXT                       asn1_ctx;
	/*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;
    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_setup_req,X2AP_NULL,sizeof(x2_setup_req_t));
    X2AP_MEMSET(&error_indication,X2AP_NULL,
                    sizeof(rrc_x2ap_error_indication_t));
    X2AP_MEMSET(&send_err_ind,X2AP_NULL,
                    sizeof(x2ap_error_ind_bool_t));

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP REQUEST Received");
    
    /* Check if there is any cell is operational or not */
    x2ap_check_active_cell(&num_act_cell,
                           act_cell_id,
                           p_x2ap_gl_ctx);
    /*SPR_18154_START*/
    if (!num_act_cell)
    {
        x2ap_bool_et              time_to_wait_flag = X2AP_FALSE;  

        if(p_x2ap_gl_ctx->config_param.timer_conf_info.bitmask
                & X2AP_TIME_TO_WAIT_INDEX_PRESENT)
        {
            time_to_wait_flag = X2AP_TRUE;
        } 

         /* Fill Cause in Error Indication */
        error_indication.cause.t = T_x2ap_Cause_radioNetwork;
        error_indication.cause.u.radioNetwork = x2ap_cell_not_available;

        X2AP_TRACE(X2AP_WARNING,"no active cell available. ");
        
        /* send failure response with time to wait */
        retVal = x2ap_build_x2setup_fail_resp(p_x2ap_gl_ctx,
                p_enb_ctx,
                time_to_wait_flag,
                &error_indication,//X2AP_P_NULL,
                X2AP_P_NULL);

        p_enb_ctx->op_status = ASSOCIATED;

        return retVal;
    }
    /*SPR_18154_END*/

    /* Decode the ASN request */
    retVal = x2ap_decode_x2setup_req(
                p_x2ap_gl_ctx,
                p_enb_ctx,
                &x2_setup_req,
                (p_peer_enb_msg)->p_msg,
                &(p_peer_enb_msg->msg_len),
                &error_indication,
                /*SPR_19067_FIX_START*/
                &send_err_ind,
                &asn1_ctx);
                /*SPR_19067_FIX_STOP*/



    if (X2AP_SUCCESS == retVal)
    { 
        U32 count = 0;
        if (X2AP_P_NULL !=  p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId
                || (X2AP_P_NULL !=  
                    p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id))
        {
            if (X2AP_P_NULL !=  p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id)
            {
                x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id);
                p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
            }
            if (X2AP_P_NULL !=  p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId)
            {
                x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
                p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = 0;
            }
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_ACTIVE_ST;
            p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = 0;
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = 0;
            p_enb_ctx->op_status = OPERATIONAL;
            p_enb_ctx->is_x2setup_sent = X2AP_FALSE;
        }

        /* BUG 11545 Fix Start */
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_ACTIVE_ST;
        p_enb_ctx->op_status = OPERATIONAL;
        /* BUG 11545 Fix Stop */

        /* update the cell info in the global context */
        x2ap_update_cell_info_for_req(&x2_setup_req,
                num_act_cell,
                act_cell_id,
                p_x2ap_gl_ctx);
        p_enb_ctx->x2ap_gu_grp_id.num_gu_group_id = x2_setup_req.num_gu_group_id;
        for (count = 0; count < p_enb_ctx->x2ap_gu_grp_id.num_gu_group_id; count ++)
        {
            X2AP_MEMCPY(&(p_enb_ctx->x2ap_gu_grp_id.group_id_arr[count]),
                    &(x2_setup_req.group_id_arr[count]), sizeof(x2ap_gu_group_id_t));
        }

        p_enb_ctx->gb_enb_id = x2_setup_req.global_enb_id;
        p_enb_id_node = (sctp_gb_enb_id_search_node_t *)x2ap_st_find_tree_node(
                &(p_x2ap_gl_ctx->enb_ctxt_trees.enb_id_search_tree),
                &(p_enb_ctx->gb_enb_id));
        if (!p_enb_id_node)
        {    
            p_enb_id_node = x2ap_mem_get(sizeof(sctp_gb_enb_id_search_node_t));
            if (PNULL != p_enb_id_node)
            {
                p_enb_id_node->gb_enb_id = p_enb_ctx->gb_enb_id;                       
                p_enb_id_node->p_enb_context = p_enb_ctx;
                x2ap_st_insert_node(
                        &(p_x2ap_gl_ctx->enb_ctxt_trees.enb_id_search_tree),
                        &(p_enb_id_node->nodeAnchor));
            }
        }     
        /*RSU Changes Start*/
        /*Update the x2ap global context to save the served cell info of the peer EnodeB*/
         x2ap_served_cell_info_update(p_x2ap_gl_ctx,
                      X2AP_LINK_UP,
  		      p_enb_ctx->peer_enodeb_id,
              &x2_setup_req);        
         /*RSU Changes End */

        /* set procedure sm context state to ACTIVE */
        x2ap_active_procedure_sm_ctx_state(p_enb_ctx);

        X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP RESPONSE");
         /* Build and send x2 setup resp to peer enb */
	 retVal = x2ap_build_x2setup_resp(num_act_cell,
			 act_cell_id,
			 p_x2ap_gl_ctx,
			 p_enb_ctx); 			 

	 /* Coverity ID 81403 Starts */
	 if(X2AP_SUCCESS != retVal)
	 {
		 X2AP_TRACE(X2AP_WARNING,"Failure returned by function x2ap_build_x2setup_resp ");
	 }
	 /* Coverity ID 81403 Ends */

	 /*if (p_enb_ctx->is_x2setup_sent == X2AP_FALSE)*/
	 /* Send X2 Link UP indication to UECC */
	 if(X2AP_FAILURE ==
			 x2ap_build_and_send_status_ind(p_enb_ctx,
                        X2AP_LINK_UP,
                        x2_setup_req.num_served_cell,
                        /* CSR_00040799 Fix Start */
                        &(x2_setup_req.served_cell_arr[0]),
                        p_x2ap_gl_ctx->total_ue_supported))
                        /* CSR_00040799 Fix End */
            {
                X2AP_TRACE(X2AP_WARNING, "%s:"
                        " rrc_send_message send failed x2ap status \
                        msg ind",
                        __FUNCTION__);
            }
	    
	    if(p_x2ap_gl_ctx->is_son_present)
	    {
            	/* Send X2 Link UP indication to SON */
            	retVal = x2ap_send_link_up_ind_to_son((x2_setup_resp_t *)&x2_setup_req,
                	                              p_x2ap_gl_ctx,
                                                  /*SPR 17777 +-*/
						      &p_enb_ctx->enb_comm_info);
            	if(!retVal)
            	{
                	X2AP_TRACE(X2AP_ERROR,"Sending enb link up indication failed.\n");
            	}
        }
        else
        {
            retVal = x2ap_send_link_up_ind_to_oam(p_enb_ctx);
            if(!retVal)
            {
               	X2AP_TRACE(X2AP_ERROR,"Sending enb link up indication failed.\n");
            }
            /* Send X2 Link UP indication to RRM */
            retVal = x2ap_send_link_up_ind_to_rrm((x2_setup_resp_t *)&x2_setup_req);
            /*SPR 17777 +-*/
        }

            /*CSR 51443 Start*/
            ALARM_MSG_L3(RRC_MODULE_ID, RRC_X2AP_CONNECTION_SUCCESS_ALARM_ID, NOTIFICATION, INVALID_ERR_CAUSE_ID);
            /*CSR 51443 Stop*/
            
            /*Invoke UE Associated sig msg FSM*/
            x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_X2_LINK_UP_EV,
                    p_msg_ptr,
                    p_x2ap_gl_ctx,
                    p_enb_ctx, X2AP_NULL);  

    }
    else
    {
        /* Send failure response without time to wait */
        retVal = x2ap_build_x2setup_fail_resp(p_x2ap_gl_ctx,
                                              p_enb_ctx,
                                              X2AP_FALSE,
                                              &error_indication,
                                              &send_err_ind);
    }

	/*SPR_19067_FIX_START*/
    rtFreeContext(&asn1_ctx);
   /*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : sctp_conn_down_evt_hdl
 * Inputs         : p_msg_ptr  -    Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when X2 Association is down
 *                  received from OAM.This function is invoked per ENB context.
 ******************************************************************************/
x2ap_return_et time_to_wait_exp_hdl (U8                 *p_msg_ptr,
                                     x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                     x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et                  retVal = X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);
    p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
 
    /* Build and send the x2 setup request for peer eNB */
    retVal = x2setup_request_msg_init (p_msg_ptr,
                                       p_x2ap_gl_ctx,
                                       p_enb_ctx);
    
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : x2ap_init_cell_info
 * Inputs         : p_x2ap_gl_ctx  pointer to X2AP global context
 *                : p_enb_ctx      Pointer to eNB context
 * Outputs        : None
 * Returns        : None
 * Description    : This function inits the cell information
 ******************************************************************************/
void x2ap_init_cell_info(x2ap_gb_context_t       *p_x2ap_gl_ctx,
                         x2ap_peer_enb_context_t *p_enb_ctx)
{
    U16                   num_act_cell = 0;
    U16                   act_cell_id[MAX_NUM_CELL_IN_X2AP]={0};
    U16                   num_ngh_cell=0;
    U16                   ngh_cell_id[MAX_NEIGHBOURS]={0};
    U16                   reset_ngh_cell_index = 0;
    U16                   cell_count = 0;
    U16                   ngh_count = 0;

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);

    /* Check if there is any cell is operational or not */
    x2ap_check_active_cell(&num_act_cell,
                           act_cell_id,
                           p_x2ap_gl_ctx);
    for (num_ngh_cell = 0; num_ngh_cell < MAX_NEIGHBOURS; num_ngh_cell++)
    {
        #ifdef ENDC_ENABLED
        /* MENB CHANGES - START */
        if ( X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT &
            p_enb_ctx->bitmask )
        {
            if ( X2AP_FIRST_IS_EQUAL_TO_SECOND == x2ap_compare_gnb_id_search_tree(
                        (const void *)(&p_enb_ctx->gb_gnb_id),
                        (const void *)(&(p_x2ap_gl_ctx->peer_enb_cell_info[num_ngh_cell].
                                peer_gb_gnb_id) ) ) )
            {
                ngh_cell_id[reset_ngh_cell_index] = num_ngh_cell;
                reset_ngh_cell_index++;
            }
        }
        else
        {
        /* MENB CHANGES - END */
        #endif
            if ( X2AP_FIRST_IS_EQUAL_TO_SECOND == x2ap_compare_enb_id_search_tree(
                        (const void *)(&p_enb_ctx->gb_enb_id),
                        (const void *)(&(p_x2ap_gl_ctx->peer_enb_cell_info[num_ngh_cell].
                                peer_gb_enb_id) ) ) )
            {
                ngh_cell_id[reset_ngh_cell_index] = num_ngh_cell;
                reset_ngh_cell_index++;
            }
        #ifdef ENDC_ENABLED
        /* MENB CHANGES - START */
        }
        /* MENB CHANGES - END */
        #endif
    }
    if (reset_ngh_cell_index)
    {
        for(cell_count = 0; cell_count < num_act_cell; cell_count++)
        {
            for (ngh_count = 0; ngh_count < reset_ngh_cell_index; ngh_count++)
            {
                if (DIRECT_NEIGHBOUR_ACTIVE == p_x2ap_gl_ctx->relation_table
                             [act_cell_id[cell_count]][ngh_cell_id[ngh_count]].relation)
                {
                    p_x2ap_gl_ctx->relation_table[act_cell_id[cell_count]]
                       [ngh_cell_id[ngh_count]].relation = DIRECT_NEIGHBOUR_INACTIVE;
                }
            }
        }
    }
    X2AP_UT_TRACE_EXIT();
}

/******************************************************************************
 * Function Name  : x2_setup_active_sctp_conn_down_evt_hdl
 * Inputs         : p_msg_ptr   -   Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called to handle when we get any event to 
 *                  down the active sctp connection
 ******************************************************************************/
x2ap_return_et x2_setup_active_sctp_conn_down_evt_hdl (
                                       U8      *p_msg_ptr,
                                       x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                       x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    X2AP_UT_TRACE_ENTER();

    if(X2AP_FAILURE == sctp_conn_down_evt_hdl(p_msg_ptr,
                            p_x2ap_gl_ctx, p_enb_ctx))
    {
        retVal = X2AP_FAILURE;
    }
    else
    {
        /*Invoke UE Associated sig msg FSM*/
        x2ap_ue_asso_sig_sm_entry (X2AP_UE_ASSO_SIG_X2_LINK_DOWN_EV,
                p_msg_ptr,
                p_x2ap_gl_ctx,
                p_enb_ctx, X2AP_NULL); 

        /* RSU Changes Start */
        /* Calling the function to delete all the Resource Status Measurements */
        x2ap_rsu_event_x2_conn_down_evt_hdl(p_x2ap_gl_ctx,
                p_enb_ctx);

        /* RSU Changes End */
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/*SPR_18154_START*/
/******************************************************************************
 * Function Name  : x2_setup_active_fail_resp_hdl
 * Inputs         : p_msg_ptr   -   Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called to handle X2 Setup Resp Failure 
 *                  in ACTIVE State
 ******************************************************************************/
x2ap_return_et x2_setup_active_fail_resp_hdl(
                                U8      *p_msg_ptr,
                                x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    X2AP_UT_TRACE_ENTER();

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP FAIL Received");

    /*Check if this is the first message recived from peer eNB after sending X2
     * Setup Response */
    if(X2AP_TRUE == p_enb_ctx->flag_for_consecutive_x2setup_resp_failure)
    {
        X2AP_TRACE(X2AP_WARNING,"[%s]::Recieved consecutive X2_SETUP_RESP_FAILURE"
                " after sending X2 Setup Response ",
                __FUNCTION__);

        p_enb_ctx->flag_for_consecutive_x2setup_resp_failure = X2AP_FALSE;
        /*Call Failure Response Handler*/
        if(X2AP_SUCCESS == x2_setup_fail_resp_hdl(p_msg_ptr, p_x2ap_gl_ctx,p_enb_ctx))
        {
            /*Send Link Down Indication */
            if ( p_enb_ctx->op_status == OPERATIONAL) 
            {
                p_enb_ctx->op_status = ASSOCIATED;
                p_enb_ctx->is_x2setup_sent = X2AP_FALSE;

                /* Send X2 Link DOWN indication to UECC */
                if(X2AP_FAILURE ==
                        x2ap_build_and_send_status_ind(
                            p_enb_ctx, 
                            X2AP_LINK_DOWN,
                            0,
                            X2AP_P_NULL,
                            p_x2ap_gl_ctx->total_ue_supported))
                {
                    X2AP_TRACE(X2AP_WARNING, "%s:"
                            " rrc_send_message send failed x2ap status \
                            msg ind",
                            __FUNCTION__);
                }

                if (!p_enb_ctx->is_del_enb_ongoing)
                {
                    if(p_x2ap_gl_ctx->is_son_present)
                    {
                        /* Send X2 link down to SON */
                        retVal = x2ap_send_link_down_ind_to_son(
                                p_x2ap_gl_ctx, p_enb_ctx);
                    }
                    else
                    {
                        /* Send X2 link down to OAM and RRM */
                        retVal = x2ap_send_link_down_ind_to_oam_rrm(p_enb_ctx);
                    }
                }

            }
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR,"[%s]::Error in handling x2 setup response failure"
                    " Ignoring Message",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
        }
    }
    else /* Ignore this Message */
    {
        X2AP_TRACE(X2AP_WARNING,"[%s]::Recieved Invalid Message X2_SETUP_RESP_FAILURE:"
                " Ignoring Message",
                __FUNCTION__);
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/*SPR_18154_END*/
/******************************************************************************
 * Function Name  : sctp_conn_down_evt_hdl
 * Inputs         : p_msg_ptr  -    Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called to handle sctp connection down event
 ******************************************************************************/
x2ap_return_et sctp_conn_down_evt_hdl (U8                 *p_msg_ptr,
                                       x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                       x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et                  retVal = X2AP_FAILURE;
    LOCAL_EVENT_X2_CONN_RELEASE_STRUCT *p_event_x2_conn_rel = PNULL;
    /* ARGUSED */
     
    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);
   
    X2AP_TRACE(X2AP_DETAILED, "Unused variable in FSM :%p",p_msg_ptr);
    p_event_x2_conn_rel = rrc_mem_get(sizeof(LOCAL_EVENT_X2_CONN_RELEASE_STRUCT));
    if (PNULL != p_event_x2_conn_rel)
    {
        memset_wrapper(p_event_x2_conn_rel, RRC_NULL, sizeof(LOCAL_EVENT_X2_CONN_RELEASE_STRUCT));
        p_event_x2_conn_rel->header.length = sizeof(LOCAL_EVENT_X2_CONN_RELEASE_STRUCT);
        p_event_x2_conn_rel->header.event_id = LOCAL_EVENT_X2_CONN_RELEASE;

        if (p_x2ap_gl_ctx->global_enb_id.enb_id.presence_bitmask & X2AP_OAM_MACRO_ENB_ID_PRESENT)
        {
            l3_memcpy_wrapper(p_event_x2_conn_rel->EVENT_PARAM_GENBID, p_x2ap_gl_ctx->global_enb_id.enb_id.
                macro_enb_id.eNB_id, MACRO_ENB_ID_OCTET_SIZE);
        }
        else if(p_x2ap_gl_ctx->global_enb_id.enb_id.presence_bitmask & X2AP_OAM_HOME_ENB_ID_PRESENT)
        {
            l3_memcpy_wrapper(&p_event_x2_conn_rel->EVENT_PARAM_GENBID, p_x2ap_gl_ctx->global_enb_id.enb_id.
                home_enb_id.eNB_id, HOME_ENB_ID_OCTET_SIZE);
        }
        
        l3_memcpy_wrapper(p_event_x2_conn_rel->EVENT_PARAM_ENB_IP_ADDRESS,
                p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr,
                MAX_X2AP_IP_ADDRESS_LENGTH);

        p_event_x2_conn_rel->EVENT_PARAM_X2_RELEASE_CAUSE = EVENT_VALUE_X2_CONN_LOST; 

        rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_x2_conn_rel);
    }
            
    /* Stop all running timer */
    if (p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId)
    {
        x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;
    }
    if (p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id)
    {
        x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id);
        p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
    }
    if (p_x2ap_gl_ctx->cell_up_guard_tim_id)
    {
        x2ap_stop_timer(p_x2ap_gl_ctx->cell_up_guard_tim_id);
        p_x2ap_gl_ctx->cell_up_guard_tim_id = X2AP_P_NULL;
    }
    /* Initialize the cell Info for the peer eNB */
    x2ap_init_cell_info(p_x2ap_gl_ctx, p_enb_ctx);

    /* change state to IDLE */
    p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_IDLE_ST;
    

    if ( p_enb_ctx->op_status == OPERATIONAL) 
    {
        p_enb_ctx->op_status = NOT_ASSOCIATED;
        p_enb_ctx->is_x2setup_sent = X2AP_FALSE;

        /*CSR 51443 Start*/
        ALARM_MSG_L3(RRC_MODULE_ID, RRC_X2AP_CONNECTION_FAILURE_ALARM_ID, CRITICAL_ALARM, INVALID_ERR_CAUSE_ID);
        /*CSR 51443 Stop*/

        // MR116656
        #ifdef ENDC_ENABLED
        if (X2AP_TRUE == p_enb_ctx->peer_enb_type)
        {
            /*
            * Send X2 Link DOWN indication to UECC
            */
            if ( X2AP_FAILURE ==
                   x2ap_build_and_send_nr_status_ind(
                       p_enb_ctx,
                       p_x2ap_gl_ctx, 
                       X2AP_LINK_DOWN,
                       X2AP_NULL,
                       X2AP_P_NULL,
                       p_x2ap_gl_ctx->total_ue_supported) )
            {
               X2AP_TRACE(X2AP_WARNING, "%s:"
                    " rrc_send_message send failed ENDC x2ap status \
                    msg ind",
                    __FUNCTION__);
            }
        }
        else
        {
        #endif
        
        /* Send X2 Link DOWN indication to UECC */
        if(X2AP_FAILURE ==
                x2ap_build_and_send_status_ind(
                    p_enb_ctx, 
                    X2AP_LINK_DOWN,
                    0,
                    /* CSR_00040799 Fix Start */
                    X2AP_P_NULL,
                    p_x2ap_gl_ctx->total_ue_supported))
                    /* CSR_00040799 Fix End */
        {
            X2AP_TRACE(X2AP_WARNING, "%s:"
                    " rrc_send_message send failed x2ap status \
                    msg ind",
                    __FUNCTION__);
        }
        #ifdef ENDC_ENABLED
        }
        #endif

        if (!p_enb_ctx->is_del_enb_ongoing)
        {
            if(p_x2ap_gl_ctx->is_son_present)
            {
                /* Send X2 link down to SON */
                retVal = x2ap_send_link_down_ind_to_son(
                        p_x2ap_gl_ctx, p_enb_ctx);
            }
            else
            {
                /* Send X2 link down to OAM and RRM */
                retVal = x2ap_send_link_down_ind_to_oam_rrm(p_enb_ctx);
            }
        }
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/****************************************************************************
 * Function Name  : x2setup_request_msg_init
 * Inputs         : p_msg_ptr    -  Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1 
 * Description    : This function is called when X2 Association is UP
 *                  received from OAM.This function is invoked per ENB context
 *                  to build and send x2setup request.
 ****************************************************************************/
x2ap_return_et x2setup_request_msg_init (U8                 *p_msg_ptr,
                                         x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                         x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    /*SPR_16533_DELTA2_START*/
#ifdef LKSCTP
    S32                   sctp_sendmsg_ret_val = 0;
    /* SPR 18692 Fix Start */
    sctp_ip_search_node_t      *p_ip_node = X2AP_P_NULL;
    /* SPR 18692 Fix End */
#endif
    /*SPR_16533_DELTA2_END*/
    U8                    asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16                   asn_msg_len = 0;
    x2ap_timer_buffer_t  x2_setup_rsp_timer_buf;
    U16                   num_act_cell = 0;
    U16                   act_cell_id[MAX_NUM_CELL_IN_X2AP] = {0};
    static x2_setup_req_t       x2_setup_req;
#ifdef ENDC_ENABLED
    static en_dc_x2_setup_req_t    en_dc_x2_setup_req;
#endif
    x2ap_peer_enb_msg_t  peer_enb_msg;
    x2_gb_enb_id_t       gb_enb_id;
    /* ARGUSED */
 
    X2AP_UT_TRACE_ENTER();
   /* Received pointer should not be NULL */
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET (&x2_setup_rsp_timer_buf, X2AP_NULL, sizeof(x2ap_timer_buffer_t));
    X2AP_MEMSET (&x2_setup_req, X2AP_NULL, sizeof(x2_setup_req_t));
#ifdef ENDC_ENABLED
    X2AP_MEMSET (&en_dc_x2_setup_req, X2AP_NULL, sizeof(en_dc_x2_setup_req_t));
#endif
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));
    X2AP_MEMSET(&gb_enb_id,X2AP_NULL,sizeof(x2_gb_enb_id_t));

    X2AP_TRACE(X2AP_DETAILEDALL,"Unused variable in FSM %p",p_msg_ptr);
    /* Check the Guard timer expired or not */
    if (p_x2ap_gl_ctx->cell_up_guard_tim_id)
    {
        X2AP_TRACE(X2AP_INFO,"------Wait for cell_up_guard_timer Expiry------");
        retVal = X2AP_SUCCESS;
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }
    else 
    {
        /* Check if there is any cell is operational or not */
        x2ap_check_active_cell(&num_act_cell,
                               act_cell_id,
                               p_x2ap_gl_ctx);

        /* if atleast one cell is active */
        if (num_act_cell)
        {
            #ifdef ENDC_ENABLED
            /* MENB CHANGES - START */
            if ( X2AP_TRUE == p_enb_ctx->peer_enb_type )
            {
                /* ASN Encoding */
                x2ap_build_endc_x2setup_req(&en_dc_x2_setup_req,
                        num_act_cell,
                        act_cell_id,
                        p_x2ap_gl_ctx);

                /*X2AP_TRACE(X2AP_INFO,
                                        "X2AP_MSG: ENDC X2 SETUP REQUEST");     */   

                retVal = x2ap_encode_endc_x2setup_req(
                        &en_dc_x2_setup_req,
                        asn_msg_buff,
                        &asn_msg_len );

                if ( X2AP_SUCCESS == retVal )
                {
                    peer_enb_msg.p_msg = asn_msg_buff;
                    peer_enb_msg.msg_len = asn_msg_len;
                    peer_enb_msg.stream_id = p_enb_ctx->stream_info.
                        stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
                    if (INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd)
                    {
#ifdef LKSCTP
                        /* For One to Many SCTP Socket, X2_SETUP Req (First Msg) will be
                         * directly sent by the client as in this case sending a
                         * message will cause attempt for creation of new
                         * association if not already created.Also, as the server
                         * and client socket is same before X2_SETUP, it is
                         * assumed that the socket_descriptor is valid */
                        if ( p_enb_ctx->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT )
                        {
                            struct sockaddr_in6 serverAddr6 = {0};
                            serverAddr6.sin6_family = AF_INET6;

                            if(  inet_pton_wrapper(AF_INET6,
                                        (const char*)(p_enb_ctx->enb_comm_info.ipv6_addr[0].ipv6_addr),
                                        (void *)&(serverAddr6.sin6_addr.s6_addr)) !=1)
                            {
                                X2AP_TRACE(X2AP_DETAILED,"Couldn't convert INET6 adress while setting HeartBeat");
                                return X2AP_FAILURE;
                            }
                            serverAddr6.sin6_port = (unsigned short)htons_wrapper(p_enb_ctx->enb_comm_info.port);

                            X2AP_TRACE(X2AP_INFO, "Peer eNB : socket desc %d s6_addr %s sin6_port %d ", p_enb_ctx->sctp_sd,
                                    p_enb_ctx->enb_comm_info.ipv6_addr[0].ipv6_addr, p_enb_ctx->enb_comm_info.port);
                            socklen_t from_len = (socklen_t) sizeof(struct sockaddr_in6);
                            sctp_sendmsg_ret_val = sctp_sendmsg(
                                    p_enb_ctx->sctp_sd,
                                    peer_enb_msg.p_msg,
                                    peer_enb_msg.msg_len,
                                    (struct sockaddr *)&serverAddr6,
                                    from_len,
                                    htonl_wrapper(SCTP_PAYLOAD_PROTOCOL_ID_X2AP),
                                    0,
                                    X2AP_NULL,
                                    0,
                                    0);


                        }
                        else
                        {
                            struct sockaddr_in serverAddr = {0};
                            serverAddr.sin_family = AF_INET;
                            serverAddr.sin_addr.s_addr =  
                                inet_addr_wrapper((const char*)(p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr)) ;
                            serverAddr.sin_port = (unsigned short)htons_wrapper(p_enb_ctx->enb_comm_info.port);

                            X2AP_TRACE(X2AP_DETAILED, "Peer eNB : socket desc %d s_addr %s sin_port %d", p_enb_ctx->sctp_sd,
                                    p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr, p_enb_ctx->enb_comm_info.port);

                            socklen_t from_len = (socklen_t) sizeof(struct sockaddr_in);
#ifdef LINUX_PC_TEST
                    sctp_sendmsg_ret_val = pctest_forward_x2ap_lksctp_message_to_pc(
                        RRC_X2AP_MODULE_ID,  /* Source module Id */
                        RRC_X2AP_PEER_eNODEB_MODULE_ID,  /* Destination module Id */
                        X2AP_ENB_MESSAGE_PS_TO_TEST,         /* API Id /msg id*/
                        p_enb_ctx->sctp_sd,     /*socket*/
                        X2AP_NULL,  /*stream id*/
                       (struct sockaddr *)&serverAddr,   
                       peer_enb_msg.p_msg,
                       peer_enb_msg.msg_len);
#else
                            sctp_sendmsg_ret_val = sctp_sendmsg(
                                    p_enb_ctx->sctp_sd,
                                    peer_enb_msg.p_msg,
                                    peer_enb_msg.msg_len,
                                    (struct sockaddr *)&serverAddr,
                                    from_len,
                                    htonl_wrapper(SCTP_PAYLOAD_PROTOCOL_ID_X2AP),
                                    0,
                                    X2AP_NULL,
                                    0,
                                    0);
#endif

                        }
#endif
#if 0
#ifdef ARICENT_SCTP_IPR
                        /*Sending the  X2 SETUP REQUEST to Peer eNB via SCTP */
                        retVal = x2ap_sctp_sm_entry (X2AP_SCTP_SEND_DATA_EV,
                                (U8 *)&peer_enb_msg,
                                p_x2ap_gl_ctx,
                                p_enb_ctx);
#endif
#endif
#ifdef LKSCTP
                        X2AP_TRACE(X2AP_DETAILED, "sctp_sendmsg retVal= %d",sctp_sendmsg_ret_val);
                        if (sctp_sendmsg_ret_val > 0 )
#endif
#if 0
#ifdef ARICENT_SCTP_IPR
                            if(X2AP_SUCCESS == retVal)
#endif
#endif	

                            {
                                X2AP_TRACE(X2AP_INFO,
                                        "X2AP_MSG: ENDC X2 SETUP REQUEST %s",
                                        change_data_to_str(peer_enb_msg.msg_len, peer_enb_msg.p_msg)); 
                                        
                                /* Change state and wait for X2 Setup Response */
                                p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = 
                                    X2_SETUP_W_X2_SETUP_RESP_ST;

                                p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count =
                                    p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count + 1;
                                p_enb_ctx->is_x2setup_sent = X2AP_TRUE;
                                //X2AP_TRACE(X2AP_INFO,
                                //        "X2AP_MSG: ENDC X2 SETUP REQUEST");
                                /* Start guard timer for X2_Setup_Rsp*/
                                x2_setup_rsp_timer_buf.timer_type = 
                                    X2AP_X2_SETUP_RSP_TIMER ;
                                x2_setup_rsp_timer_buf.auto_delete  = X2AP_TRUE;
                                x2_setup_rsp_timer_buf.sd    = p_enb_ctx->sctp_sd;

#ifdef LKSCTP
                                if(p_enb_ctx->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
                                {
                                    l3_strncpy_wrapper((S8*)x2_setup_rsp_timer_buf.ip_addr,
                                            (const S8*)p_enb_ctx->enb_comm_info.ipv6_addr[0].ipv6_addr,
                                            MAX_X2AP_IPV6_ADDRESS_LENGTH);

                                }
                                else
                                {
                                    l3_strncpy_wrapper((S8*)x2_setup_rsp_timer_buf.ip_addr,
                                            (const S8 *)p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr,
                                            MAX_X2AP_IP_ADDRESS_LENGTH);
                                }
                                p_ip_node = (sctp_ip_search_node_t *)
                                    x2ap_st_find_tree_node(&p_x2ap_gl_ctx->enb_ctxt_trees.
                                            ip_search_tree,  &p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr);
                                if (X2AP_P_NULL == p_ip_node)
                                {
                                    X2AP_TRACE(X2AP_INFO,"[%s] ip_addr [%s] "
                                            "NOT found in ip_search_tree so inserting it ",__FUNCTION__,
                                            p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr);
                                    p_ip_node = (sctp_ip_search_node_t*)
                                        x2ap_mem_get(sizeof(sctp_ip_search_node_t));
                                    if (PNULL == p_ip_node)
                                    {
                                        X2AP_TRACE(X2AP_WARNING, "Memory allocation failed.");
                                        return X2AP_FAILURE;
                                    }
                                    /* Copy IP Address in tree node */
                                    l3_strncpy_wrapper ((S8 *)(p_ip_node->enb_ip_address),
                                            (const S8 *)(p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr),
                                            MAX_X2AP_IP_ADDRESS_LENGTH);
                                    p_ip_node->p_enb_context = p_enb_ctx;
                                    /* Insert in ip_port tree*/
                                    x2ap_st_insert_node(&(p_x2ap_gl_ctx->enb_ctxt_trees.ip_search_tree),
                                            &(p_ip_node->nodeAnchor));

                                }
#endif

                                p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = 
                                    x2ap_start_timer(
                                            p_x2ap_gl_ctx->config_param.
                                            timer_conf_info.x2ap_setup_retry_tim_val,
                                            &x2_setup_rsp_timer_buf,
                                            sizeof(x2ap_timer_buffer_t),
                                            X2AP_FALSE);
                            }
                            else
                            {
                                X2AP_TRACE(X2AP_ERROR, "[%s]::sctp_sendmsg failed"
                                        "cannot send msg to peer enb",
                                        __FUNCTION__);
                                X2AP_TRACE(X2AP_WARNING,
                                        " errno:%d\n",errno);
                            }
                    }
                    else
                    {
                        X2AP_TRACE(X2AP_ERROR, "[%s]::PEER eNode-B SD is not"
                                " Set canot send Request Message",
                                __FUNCTION__);
                    }
                }
                else
                {
                    X2AP_TRACE(X2AP_ERROR,
                            "[%s]::ASN Encoding of EN DC X2 Setup Failed"
                            ,__FUNCTION__);
                }
            }
            else
            {
            #endif
            /* Build X2 SETUP REQUEST TO send to eNB */
            x2ap_build_x2setup_req(&x2_setup_req,
                    num_act_cell,
                    act_cell_id,
                    p_x2ap_gl_ctx);
            /* MENB CHANGES - END */

            /*X2AP_TRACE(X2AP_INFO,
                                        "X2AP_MSG: X2 SETUP REQUEST");*/
                                        
            retVal = x2ap_encode_x2setup_req(
                    &(x2_setup_req),
                    asn_msg_buff,
                    &asn_msg_len );

            if (X2AP_SUCCESS == retVal)
            {
                peer_enb_msg.p_msg = asn_msg_buff;
                peer_enb_msg.msg_len = asn_msg_len;
                /* SPR_14436_Fix Start */
                peer_enb_msg.stream_id = p_enb_ctx->stream_info.
                    stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
                /* SPR_14436_Fix Stop */
                if (INVALID_SCTP_SOCKET != p_enb_ctx->sctp_sd)
                {
                    /*SPR_16533_START*/
#ifdef LKSCTP
                    /* For One to Many SCTP Socket, X2_SETUP Req (First Msg) will be
                     * directly sent by the client as in this case sending a
                     * message will cause attempt for creation of new
                     * association if not already created.Also, as the server
                     * and client socket is same before X2_SETUP, it is
                     * assumed that the socket_descriptor is valid */
            /* SPR 20633 changes start */
            /* Code deleted */
		    if(p_enb_ctx->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
		    {
                struct sockaddr_in6 serverAddr6 = {0};
			    serverAddr6.sin6_family = AF_INET6;

			    /* Coverity_99670 Fix Start */
			    if(  inet_pton_wrapper(AF_INET6,
						    (const char*)(p_enb_ctx->enb_comm_info.ipv6_addr[0].ipv6_addr),
						    (void *)&(serverAddr6.sin6_addr.s6_addr)) !=1)
			    {
				    X2AP_TRACE(X2AP_DETAILED,"Couldn't convert INET6 adress while setting HeartBeat");
				    return X2AP_FAILURE;
			    }
                serverAddr6.sin6_port = (unsigned short)htons_wrapper(p_enb_ctx->enb_comm_info.port);

                X2AP_TRACE(X2AP_INFO, "Peer eNB : socket desc %d s6_addr %s sin6_port %d ", p_enb_ctx->sctp_sd,
                        p_enb_ctx->enb_comm_info.ipv6_addr[0].ipv6_addr, p_enb_ctx->enb_comm_info.port);
                socklen_t from_len = (socklen_t) sizeof(struct sockaddr_in6);
                                        /*SPR_16533_DELTA2_START*/
                    sctp_sendmsg_ret_val = sctp_sendmsg(
                    /*SPR_16533_DELTA2_END*/
                            p_enb_ctx->sctp_sd,
                            peer_enb_msg.p_msg,
                            peer_enb_msg.msg_len,
                            (struct sockaddr *)&serverAddr6,
                            from_len,
                            /*SPR_16533_DELTA2_START*/
                            htonl_wrapper(SCTP_PAYLOAD_PROTOCOL_ID_X2AP),
                            /*SPR_16533_DELTA2_END*/
                            0,
                            X2AP_NULL,
                            0,
                            0);


			 /* Coverity_99670 Fix End */
                    }
                    else
                    {
                        struct sockaddr_in serverAddr = {0};
                        serverAddr.sin_family = AF_INET;
                        serverAddr.sin_addr.s_addr =  
                            inet_addr_wrapper((const char*)(p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr)) ;
                    serverAddr.sin_port = (unsigned short)htons_wrapper(p_enb_ctx->enb_comm_info.port);
                        //socklen_t from_len = (socklen_t) sizeof(struct sockaddr_in);

                    X2AP_TRACE(X2AP_DETAILED, "Peer eNB : socket desc %d s_addr %s sin_port %d", p_enb_ctx->sctp_sd,
                            p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr, p_enb_ctx->enb_comm_info.port);

                        socklen_t from_len = (socklen_t) sizeof(struct sockaddr_in);
#ifdef LINUX_PC_TEST
                    sctp_sendmsg_ret_val = pctest_forward_x2ap_lksctp_message_to_pc(
                        RRC_X2AP_MODULE_ID,  /* Source module Id */
                        RRC_X2AP_PEER_eNODEB_MODULE_ID,  /* Destination module Id */
                        X2AP_ENB_MESSAGE_PS_TO_TEST,         /* API Id /msg id*/
                        p_enb_ctx->sctp_sd,     /*socket*/
                        X2AP_NULL,  /*stream id*/
                       (struct sockaddr *)&serverAddr,   
                       peer_enb_msg.p_msg,
                       peer_enb_msg.msg_len);
#else 
                    /*SPR_16533_DELTA2_START*/
                    sctp_sendmsg_ret_val = sctp_sendmsg(
                    /*SPR_16533_DELTA2_END*/
                            p_enb_ctx->sctp_sd,
                            peer_enb_msg.p_msg,
                            peer_enb_msg.msg_len,
                            (struct sockaddr *)&serverAddr,
                            from_len,
                            /*SPR_16533_DELTA2_START*/
                            htonl_wrapper(SCTP_PAYLOAD_PROTOCOL_ID_X2AP),
                            /*SPR_16533_DELTA2_END*/
                            0,
                            X2AP_NULL,
                            0,
                            0);
#endif

                    }
                    /* SPR 20633 changes end */

#endif
#if 0					
#ifdef ARICENT_SCTP_IPR
                    /*Sending the  X2 SETUP REQUEST to Peer eNB via SCTP */
                    retVal = x2ap_sctp_sm_entry (X2AP_SCTP_SEND_DATA_EV,
                      (U8 *)&peer_enb_msg,
                      p_x2ap_gl_ctx,
                      p_enb_ctx);
#endif
#endif
#ifdef LKSCTP
                    X2AP_TRACE(X2AP_DETAILED, "sctp_sendmsg retVal= %d",sctp_sendmsg_ret_val);
                    if (sctp_sendmsg_ret_val > 0 )
#endif
#if 0
#ifdef ARICENT_SCTP_IPR
		    if(X2AP_SUCCESS == retVal)
#endif
#endif
                        /*SPR_16533_END*/
                    {
                        X2AP_TRACE(X2AP_INFO,
                                        "X2AP_MSG: ENDC X2 SETUP REQUEST %s",
                                        change_data_to_str(peer_enb_msg.msg_len, peer_enb_msg.p_msg)); 
                                        
                        /* Change state and wait for X2 Setup Response */
                        p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = 
                            X2_SETUP_W_X2_SETUP_RESP_ST;

                        p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count =
                            p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count + 1;
                        p_enb_ctx->is_x2setup_sent = X2AP_TRUE;
                        //X2AP_TRACE(X2AP_INFO,
                        //        "X2AP_MSG: X2 SETUP REQUEST");
                        /* Start guard timer for X2_Setup_Rsp*/
                        x2_setup_rsp_timer_buf.timer_type = 
                            X2AP_X2_SETUP_RSP_TIMER ;
                        x2_setup_rsp_timer_buf.auto_delete  = X2AP_TRUE;
                        x2_setup_rsp_timer_buf.sd    = p_enb_ctx->sctp_sd;

                        /*SPR_16533_DELTA2_START*/
#ifdef LKSCTP
                        if(p_enb_ctx->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
                        {
                            l3_strncpy_wrapper((S8*)x2_setup_rsp_timer_buf.ip_addr,
                                    (const S8*)p_enb_ctx->enb_comm_info.ipv6_addr[0].ipv6_addr,
                                    MAX_X2AP_IPV6_ADDRESS_LENGTH);

                        }
                        else
                        {
                            l3_strncpy_wrapper((S8*)x2_setup_rsp_timer_buf.ip_addr,
                                    (const S8 *)p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr,
                                    MAX_X2AP_IP_ADDRESS_LENGTH);
                        }
						/* SPR 18692 Fix Start */
						p_ip_node = (sctp_ip_search_node_t *)
								x2ap_st_find_tree_node(&p_x2ap_gl_ctx->enb_ctxt_trees.
												ip_search_tree,  &p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr);
						if (X2AP_P_NULL == p_ip_node)
						{
							X2AP_TRACE(X2AP_INFO,"[%s] ip_addr [%s] "
												"NOT found in ip_search_tree so inserting it ",__FUNCTION__,
												p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr);
							p_ip_node = (sctp_ip_search_node_t*)
										x2ap_mem_get(sizeof(sctp_ip_search_node_t));
							if (PNULL == p_ip_node)
							{
									X2AP_TRACE(X2AP_WARNING, "Memory allocation failed.");
									return X2AP_FAILURE;
							}
							/* Copy IP Address in tree node */
							l3_strncpy_wrapper ((S8 *)(p_ip_node->enb_ip_address),
												(const S8 *)(p_enb_ctx->enb_comm_info.ip_addr[0].ip_addr),
												MAX_X2AP_IP_ADDRESS_LENGTH);
							p_ip_node->p_enb_context = p_enb_ctx;
							/* Insert in ip_port tree*/
							x2ap_st_insert_node(&(p_x2ap_gl_ctx->enb_ctxt_trees.ip_search_tree),
												&(p_ip_node->nodeAnchor));

						}
						/* SPR 18692 Fix End */
#endif
                        /*SPR_16533_DELTA2_END*/

                        p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = 
                            x2ap_start_timer(
                                    p_x2ap_gl_ctx->config_param.
                                    timer_conf_info.x2ap_setup_retry_tim_val,
                                    &x2_setup_rsp_timer_buf,
                                    sizeof(x2ap_timer_buffer_t),
                                    X2AP_FALSE);
                    }
                    /*SPR_16533_DELTA2_START*/
                    else
                    {
                        /*SPR_16533_START*/
                        X2AP_TRACE(X2AP_INFO, "[%s]::sctp_sendmsg failed"
                                "cannot send msg to peer enb",
                                __FUNCTION__);
                        X2AP_TRACE(X2AP_WARNING,
                                " errno:%d\n",errno);
                        /*SPR_16533_END*/
                    }
                    /*SPR_16533_DELTA2_END*/
                }
                else
                {
                    X2AP_TRACE(X2AP_INFO, "[%s]::PEER eNode-B SD is not"
                            " Set canot send Request Message",
                            __FUNCTION__);
                }
            }
			else
			{
				X2AP_TRACE(X2AP_ERROR,
								"[%s]::ASN Encoding of X2 Setup Failed"
								,__FUNCTION__);
			}
            #ifdef ENDC_ENABLED
            /* MENB CHANGES - START */
            } 
            /* MENB CHANGES - END */
            #endif
        }
        else
        {
            X2AP_TRACE(X2AP_INFO,"------No cell cell is up wait "
                       "for cell setup indication------");
            retVal = X2AP_SUCCESS;
            X2AP_UT_TRACE_EXIT();
            return retVal;
        }
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/****************************************************************************
 * Function Name  : x2ap_build_x2setup_req
 * Inputs         : p_x2_setup_req - pointer to x2_setup_req_t
 *                  p_x2ap_gl_ctx  - pointer to X2AP global context
 *                  num_act_cell - number of active cell
 *                  p_act_cell_id - logical id of active cell
 * Outputs        : p_x2_setup_req - x2 setup request
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called to build x2setup request.
 ****************************************************************************/
x2ap_return_et x2ap_build_x2setup_req(x2_setup_req_t    *p_x2_setup_req,
                                      U16                 num_act_cell,
                                      U16                *p_act_cell_id,
                                      x2ap_gb_context_t *p_x2ap_gl_ctx)
{
    x2ap_return_et    retVal = X2AP_SUCCESS;
    U16               srvd_cell_count = 0;
    U16               ngh_cell_count = 0;
    U16               act_cell_count = 0;
    U16               ngh_index_count = 0;
    U8                gr_index_count = 0;
    U16               num_ngh_cell = 0;
    U16               ngh_cell_index[MAX_NEIGHBOURS] = {0};

    X2AP_UT_TRACE_ENTER();
    /* Fill the global eNB Id */
    p_x2_setup_req->global_enb_id = p_x2ap_gl_ctx->global_enb_id;
    /* Fill Number of served cell */
    p_x2_setup_req->num_served_cell = num_act_cell;
    /* Fill served cell Info */
    for (srvd_cell_count = 0; srvd_cell_count < num_act_cell; srvd_cell_count++)
    {
        p_x2_setup_req->served_cell_arr[srvd_cell_count].
                                        served_cell_info.bitmask =
           p_x2ap_gl_ctx->served_cell_info
                                     [p_act_cell_id[act_cell_count]].bitmask;

        p_x2_setup_req->served_cell_arr[srvd_cell_count].served_cell_info.pci =
           p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].pci;

        /* Generate the ecgi and filling in the global context also */ 
        /*SPR 21554 Start */
        X2AP_MEMCPY((void *)p_x2_setup_req->served_cell_arr[srvd_cell_count].served_cell_info.cell_id.plmn_identity.plmn_id , 
                (const void *)p_x2ap_gl_ctx->served_cell_info[srvd_cell_count].broadcast_plmn_info.plmn_identity[RRC_ZERO].plmn_id,
                MAX_PLMN_ID_BYTES);
        X2AP_MEMCPY((void *)p_x2_setup_req->served_cell_arr[srvd_cell_count].served_cell_info.cell_id.eutran_cell_id,
                (const void *)p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].ecgi.eutran_cell_id,
                HOME_ENB_ID_OCTET_SIZE);
        /*SPR 21554 End */

        p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].ecgi = 
              p_x2_setup_req->served_cell_arr[srvd_cell_count].
                                                    served_cell_info.cell_id; 

        p_x2_setup_req->served_cell_arr[srvd_cell_count].
              served_cell_info.track_area_code = 
           p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
              track_area_code;

        p_x2_setup_req->served_cell_arr[srvd_cell_count].
              served_cell_info.broadcast_plmn_info =
           p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
              broadcast_plmn_info;

        p_x2_setup_req->served_cell_arr[srvd_cell_count].
              served_cell_info.choice_eutra_mode =
        p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
              choice_eutra_mode;

        if (p_x2_setup_req->served_cell_arr
                                [srvd_cell_count].served_cell_info.bitmask)
        {
            p_x2_setup_req->served_cell_arr[srvd_cell_count].
              served_cell_info.num_antenna_port =
            (x2ap_num_antenna_port_et)p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
              num_antenna_port;
            X2AP_MEMCPY(p_x2_setup_req->served_cell_arr[srvd_cell_count].served_cell_info.csg_identity,
                     p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].csg_identity,
                     sizeof(p_x2_setup_req->served_cell_arr[srvd_cell_count].served_cell_info.csg_identity));
        }

        if (X2AP_PRACH_CONFIG_INFO_PRESENT & p_x2ap_gl_ctx->
                        served_cell_info[p_act_cell_id[act_cell_count]].bitmask)
        {
            p_x2_setup_req->served_cell_arr[srvd_cell_count].
                served_cell_info.bitmask |= X2AP_PRACH_CONFIG_INFO_PRESENT;
            X2AP_MEMCPY(&p_x2_setup_req->served_cell_arr[srvd_cell_count].served_cell_info.prach_config,
                        &p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].prach_config,
                        sizeof(x2ap_prach_configuration_info_t));
        }

#ifdef LTE_EMBMS_SUPPORTED

        if(X2AP_MBSFN_SUBFRAME_INFO_PRESENT & p_x2ap_gl_ctx->
                served_cell_info[p_act_cell_id[act_cell_count]].bitmask)
        {
            p_x2_setup_req->served_cell_arr[srvd_cell_count].
                served_cell_info.bitmask |= X2AP_MBSFN_SUBFRAME_INFO_PRESENT;
            X2AP_MEMCPY(&p_x2_setup_req->served_cell_arr[srvd_cell_count].
                    served_cell_info.mbsfn_subframe_info,
                    &p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
                    mbsfn_subframe_info,
                    sizeof(x2ap_mbsfn_subframe_info_t));
        }
#endif

        /* Find the neighbour info for the served cell */
        x2ap_get_ngh_cell_index(&num_ngh_cell,
                                ngh_cell_index,
                                p_act_cell_id[act_cell_count],
                                p_x2ap_gl_ctx,
                                X2AP_FALSE); 

        /* Fill number of neighbour cell */
        p_x2_setup_req->served_cell_arr[srvd_cell_count].
                  nbour_cell_info.num_neighbour_cell = num_ngh_cell;

        /* Fill the neighbour cell Info */
        for (ngh_cell_count = 0; ngh_cell_count < num_ngh_cell; ngh_cell_count++)
        {
            p_x2_setup_req->served_cell_arr[srvd_cell_count].
                 nbour_cell_info.neighbour_info_arr[ngh_cell_count].ecgi = 
            p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_index[ngh_index_count]].
                 peer_cell_info.ecgi;
            p_x2_setup_req->served_cell_arr[srvd_cell_count].
                 nbour_cell_info.neighbour_info_arr[ngh_cell_count].pci =
            p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_index[ngh_index_count]].
                 peer_cell_info.pci;
            p_x2_setup_req->served_cell_arr[srvd_cell_count].
                 nbour_cell_info.neighbour_info_arr[ngh_cell_count].earfcn =
            p_x2ap_gl_ctx->peer_enb_cell_info[ngh_cell_index[ngh_index_count]].
                 peer_cell_info.earfcn;
            ngh_index_count++;                
        }
        X2AP_MEMSET(&ngh_cell_index, 0, MAX_NEIGHBOURS);
        ngh_index_count = 0;
        act_cell_count++;
    }
    /* Fill number of GU Group Id */
    p_x2_setup_req->num_gu_group_id = p_x2ap_gl_ctx->group_ids.num_gu_group_id;
    /* Fill GU Group Id List */
    for (gr_index_count = 0;
                    gr_index_count < p_x2_setup_req->num_gu_group_id;
                                                              gr_index_count++)
    {
        p_x2_setup_req->group_id_arr[gr_index_count] =
                        p_x2ap_gl_ctx->group_ids.group_id_arr[gr_index_count]; 
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

#ifdef ENDC_ENABLED
/****************************************************************************
 * Function Name  : x2ap_build_endc_x2setup_req
 * Inputs         : p_x2_setup_req - pointer to x2_setup_req_t
 *                  p_x2ap_gl_ctx  - pointer to X2AP global context
 *                  num_act_cell - number of active cell
 *                  p_act_cell_id - logical id of active cell
 * Outputs        : p_x2_setup_req - x2 setup request
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called to build x2setup request.
 ****************************************************************************/
x2ap_return_et
x2ap_build_endc_x2setup_req
(
     en_dc_x2_setup_req_t  *p_x2_setup_req,
     U16                   num_act_cell,
     U16                   *p_act_cell_id,
     x2ap_gb_context_t     *p_x2ap_gl_ctx
)
{
/* TB_FIX_START */
    x2ap_return_et         retVal = X2AP_SUCCESS;
    U16                    srvd_cell_count = RRC_ZERO;
    U16                    nr_neigh_count = RRC_ZERO;
    U16                    peer_count = RRC_ZERO;
    U16                    act_cell_count = RRC_ZERO;
    U16                    freq_num_count = RRC_ZERO;
    U16                    sul_band_count = RRC_ZERO;

    X2AP_UT_TRACE_ENTER();

    /*filling eutra_cell_info */

    /*
     * Fill the global eNB Id
     */
    p_x2_setup_req->eutra_cell_info.global_enb_id =
        p_x2ap_gl_ctx->global_enb_id;

    /* Fill Number of served cell */
    p_x2_setup_req->eutra_cell_info.num_served_cell =
        num_act_cell;

    /* Fill served cell Info */
    for (srvd_cell_count = RRC_ZERO; srvd_cell_count < num_act_cell;
        srvd_cell_count++)
    {
        p_x2_setup_req->eutra_cell_info.served_cell_arr[srvd_cell_count].
            served_cell_info.pci =
            p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].pci;

        /* Generate the ecgi and filling in the global context also */ 
        X2AP_MEMCPY((void *)p_x2_setup_req->eutra_cell_info.
                served_cell_arr[srvd_cell_count].
                served_cell_info.cell_id.plmn_identity.plmn_id , 
                (const void *)p_x2ap_gl_ctx->served_cell_info[srvd_cell_count].
                broadcast_plmn_info.
                plmn_identity[RRC_ZERO].plmn_id,
                MAX_PLMN_ID_BYTES);

        X2AP_MEMCPY((void *)p_x2_setup_req->eutra_cell_info.
                served_cell_arr[srvd_cell_count].
                served_cell_info.cell_id.eutran_cell_id,
                (const void *)p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
                ecgi.eutran_cell_id,
                HOME_ENB_ID_OCTET_SIZE);

        /*filling ecgi*/
        p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].ecgi = 
            p_x2_setup_req->eutra_cell_info.served_cell_arr[srvd_cell_count].
            served_cell_info.cell_id;
 
        /*filling track area code */
        p_x2_setup_req->eutra_cell_info.served_cell_arr[srvd_cell_count].
            served_cell_info.track_area_code = 
            p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
            track_area_code;

        /*filling broadcast palm info*/
        p_x2_setup_req->eutra_cell_info.served_cell_arr[srvd_cell_count].
            served_cell_info.broadcast_plmn_info =
            p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
            broadcast_plmn_info;

        /*filling choice eutra mode */
        p_x2_setup_req->eutra_cell_info.served_cell_arr[srvd_cell_count].
            served_cell_info.choice_eutra_mode =
            p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
            choice_eutra_mode;

        act_cell_count++;
    }

    /* 
     * NR Neigbour Info 
     */
    X2AP_TRACE(X2AP_DETAILED,"num_peer_enb_cells in global context is [%d]\n",
        p_x2ap_gl_ctx->num_peer_enb_cells);

    /* Fill nr neighbour Info */
    for ( peer_count = RRC_ZERO; peer_count < p_x2ap_gl_ctx->num_peer_enb_cells;
        peer_count++)
    {
        X2AP_TRACE(X2AP_DETAILED,"For peer_cell number [%d] :- current_Cell_info is [%d] "
                "oam_configured is [%d]  \n", peer_count,p_x2ap_gl_ctx->
                peer_enb_cell_info[peer_count].current_cell_info,
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].oam_configured);

        if ( ( X2AP_NULL == p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                    current_cell_info ) &&
                ( X2AP_NULL == p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].oam_configured )
           )
        {
            /* Fill Number of nr neighbour info */
            p_x2_setup_req->eutra_cell_info.num_neigh_cell++;

            /*filling bitmask*/
            p_x2_setup_req->eutra_cell_info.bitmask |=
                X2_SETUP_NR_NEIGHBOUR_INFO_PRESENT;

            p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                nr_neigh_info.bitmask |= 
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.bitmask; 

            p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                nr_neigh_info.nr_pci =
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.nr_pci;

            /* Generate the ecgi and filling in the global context also */
            X2AP_MEMCPY((void *)p_x2_setup_req->eutra_cell_info.
                    nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_cell_id.plmn_identity.plmn_id ,
                    (const void *)p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                    peer_cell_info.nr_ecgi.
                    plmn_identity.plmn_id,
                    MAX_PLMN_ID_BYTES);

            X2AP_MEMCPY((void *)p_x2_setup_req->eutra_cell_info.
                    nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_cell_id.nr_cell_id,
                    (const void *) p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                    peer_cell_info.nr_ecgi.nr_cell_id,
                    NR_CELL_ID_OCTET_SIZE);

            /*filling ecgi*/
            /*
               p_x2ap_gl_ctx->peer_enb_cell_info[nr_neigh_count].peer_cell_info.ecgi =
               p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
               nr_neigh_info.nr_cell_id;
               */
            if ( NR_CELL_INFO_CONFIGURED_TAC_PRESENT &
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.bitmask )
            {
            X2AP_TRACE(X2AP_DETAILED,"x2 reps X2AP_NR_CELL_INFO_config_TAC_PRESENT ");
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.bitmask |= X2AP_CONFIGURED_TAC_PRESENT; 

                /*filling configured tack code */
                X2AP_MEMCPY((void *)p_x2_setup_req->eutra_cell_info.
                        nr_neigh_cell_arr[nr_neigh_count].nr_neigh_info.configured_tac.tac,
                        (const void *)p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                        peer_cell_info.configured_tac.tac,
                        MAX_TAC_BYTES);

            }
            if ( NR_CELL_INFO_FIVE_GS_TAC_PRESENT &
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.bitmask)
            {
            X2AP_TRACE(X2AP_DETAILED,"x2 reps X2AP_NR_CELL_INFO_5gs_TAC_PRESENT ");
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.bitmask |= X2AP_FIVE_GS_TAC_PRESENT; 

                /*filling fivegs tack code */
                X2AP_MEMCPY((void *)p_x2_setup_req->eutra_cell_info.
                        nr_neigh_cell_arr[nr_neigh_count].nr_neigh_info.fivegs_tac.tac,
                        (const void *)p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                        peer_cell_info.five_gs_tac.tac,
                        MAX_FIVE_GS_TAC_BYTES);

            }

            /*Filling nr neigh mode info */  
            p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                nr_neigh_info.nr_neigh_Mode_Info.bitmask |=
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                choice_nr_mode.bitmask;

            X2AP_TRACE(X2AP_DETAILED,"global ctxt choice_nr_mode "
                    "bitmask [%d]:[%d]\n",peer_count,
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                    peer_cell_info.choice_nr_mode.bitmask);

            if ( X2AP_NR_MODE_FDD_PRESENT & p_x2ap_gl_ctx->
                    peer_enb_cell_info[peer_count].peer_cell_info.
                    choice_nr_mode.bitmask)
            {
                X2AP_TRACE(X2AP_DETAILED,"X2AP_NR_MODE_FDD_PRESENT \n");

                /*filling bitmask of ul nr frequency info*/
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.bitmask |=
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    choice_nr_mode.nr_fdd_info.nr_ul_freq_info.bitmask;

                /*filling nr_earfcn */
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.nr_earfcn =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    choice_nr_mode.nr_fdd_info.nr_ul_freq_info.nr_earfcn;

                if( X2AP_NR_ARFCN_SUL_PRESENT & p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                        peer_cell_info.choice_nr_mode.nr_fdd_info.nr_ul_freq_info.bitmask)
                {
                    /*filling nr earfcn sul*/
                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.nr_earfcn_sul =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_fdd_info.nr_ul_freq_info.nr_earfcn_sul;
                }

                /* filling sul information*/
                if( X2AP_SUL_INFORMATION_PRESENT & p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                        peer_cell_info.choice_nr_mode.nr_fdd_info.nr_ul_freq_info.bitmask)
                {
                    /*filling nr_sul_arfcn*/
                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.
                        nr_sul_information.nr_sul_arfcn =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_fdd_info.nr_ul_freq_info.nr_sul_information.nr_sul_arfcn;

                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.
                        nr_sul_information.nr_tx_band_width.nr_scs =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_fdd_info.nr_ul_freq_info.nr_sul_information.
                        nr_tx_band_width.nr_scs;

                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.nr_sul_information.
                        nr_tx_band_width.nr_nrb =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_fdd_info.nr_ul_freq_info.nr_sul_information.
                        nr_tx_band_width.nr_nrb;

                }

                /*filling freq num band count */
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.num_freq_bands =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    choice_nr_mode.nr_fdd_info.nr_ul_freq_info.num_freq_bands;

                for ( freq_num_count = RRC_ZERO; freq_num_count < p_x2ap_gl_ctx->
                        peer_enb_cell_info[peer_count].
                        peer_cell_info.choice_nr_mode.nr_fdd_info.nr_ul_freq_info.
                        num_freq_bands; freq_num_count++ )
                {
                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.
                        freq_band_info[freq_num_count].freq_band_indicator_nr =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_fdd_info.nr_ul_freq_info.freq_band_info[freq_num_count].
                        freq_band_indicator_nr;

                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.
                        freq_band_info[freq_num_count].num_supported_sul_band =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_fdd_info.nr_ul_freq_info.freq_band_info[freq_num_count].
                        num_supported_sul_band;

                    for ( sul_band_count = RRC_ZERO; sul_band_count < p_x2ap_gl_ctx->
                            peer_enb_cell_info[peer_count].
                            peer_cell_info.choice_nr_mode.nr_fdd_info.nr_ul_freq_info.
                            freq_band_info[freq_num_count].num_supported_sul_band; sul_band_count++)
                    {
                        p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                            nr_neigh_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.
                            freq_band_info[freq_num_count].
                            supported_sul_freq_band_indicator_nr[sul_band_count] =
                            p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                            choice_nr_mode.nr_fdd_info.nr_ul_freq_info.freq_band_info[freq_num_count].
                            supported_sul_freq_band_indicator_nr[sul_band_count];
                    }
                }

                /*filling bitmask of dl nr frequency info*/
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.bitmask |=
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    choice_nr_mode.nr_fdd_info.nr_dl_freq_info.bitmask;

                /*filling nr_earfcn */
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.nr_earfcn =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    choice_nr_mode.nr_fdd_info.nr_dl_freq_info.nr_earfcn;

                if( X2AP_NR_ARFCN_SUL_PRESENT & p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                        peer_cell_info.choice_nr_mode.nr_fdd_info.nr_dl_freq_info.bitmask)
                {
                    /*filling nr earfcn sul*/
                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.nr_earfcn_sul =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_fdd_info.nr_dl_freq_info.nr_earfcn_sul;
                }

                /* filling sul information*/
                if( X2AP_SUL_INFORMATION_PRESENT & p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                        peer_cell_info.choice_nr_mode.nr_fdd_info.nr_dl_freq_info.bitmask)
                {
                    /*filling nr_sul_arfcn*/
                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.
                        nr_sul_information.nr_sul_arfcn =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_fdd_info.nr_dl_freq_info.nr_sul_information.nr_sul_arfcn;

                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.
                        nr_sul_information.nr_tx_band_width.nr_scs =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_fdd_info.nr_dl_freq_info.nr_sul_information.
                        nr_tx_band_width.nr_scs;

                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.
                        nr_sul_information.nr_tx_band_width.nr_nrb =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_fdd_info.nr_dl_freq_info.nr_sul_information.
                        nr_tx_band_width.nr_nrb;
                }

                /*filling freq num band count */
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.num_freq_bands =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    choice_nr_mode.nr_fdd_info.nr_dl_freq_info.num_freq_bands;

                for ( freq_num_count=RRC_ZERO;
                        freq_num_count < p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                        peer_cell_info.choice_nr_mode.nr_fdd_info.nr_dl_freq_info.
                        num_freq_bands;freq_num_count++ )
                {
                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.
                        freq_band_info[freq_num_count].freq_band_indicator_nr =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_fdd_info.nr_dl_freq_info.freq_band_info[freq_num_count].
                        freq_band_indicator_nr;

                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.
                        freq_band_info[freq_num_count].num_supported_sul_band =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_fdd_info.nr_dl_freq_info.freq_band_info[freq_num_count].
                        num_supported_sul_band;

                    for ( sul_band_count = RRC_ZERO;
                            sul_band_count < p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                            peer_cell_info.choice_nr_mode.nr_fdd_info.nr_dl_freq_info.
                            freq_band_info[freq_num_count].num_supported_sul_band; sul_band_count++)
                    {
                        p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                            nr_neigh_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.
                            freq_band_info[freq_num_count].
                            supported_sul_freq_band_indicator_nr[sul_band_count] =
                            p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                            choice_nr_mode.nr_fdd_info.nr_dl_freq_info.freq_band_info[freq_num_count].
                            supported_sul_freq_band_indicator_nr[sul_band_count];
                    }
                }
            }

            /* 
             * filling tdd 
             */
            if(X2AP_NR_MODE_TDD_PRESENT & p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                peer_cell_info.
                choice_nr_mode.bitmask)
            {
                X2AP_TRACE(X2AP_DETAILED,"X2AP_NR_MODE_TDD_PRESENT \n");
                /*filling bitmask of nr freq info for tdd*/
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info.bitmask |=
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    choice_nr_mode.nr_tdd_info.nr_freq_info.bitmask;
                /*filling nr_earfcn */
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info.nr_earfcn =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    choice_nr_mode.nr_tdd_info.nr_freq_info.nr_earfcn;

                if( X2AP_NR_ARFCN_SUL_PRESENT & p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                        peer_cell_info.choice_nr_mode.nr_tdd_info.nr_freq_info.bitmask)
                {
                    /*filling nr earfcn sul*/
                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info.nr_earfcn_sul =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_tdd_info.nr_freq_info.nr_earfcn_sul;

                }

                /* filling sul information*/
                if( X2AP_SUL_INFORMATION_PRESENT & p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                        peer_cell_info.choice_nr_mode.nr_tdd_info.nr_freq_info.bitmask)
                {
                    /*filling nr_sul_arfcn*/
                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info.
                        nr_sul_information.nr_sul_arfcn =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_tdd_info.nr_freq_info.nr_sul_information.nr_sul_arfcn;

                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info.
                        nr_sul_information.nr_tx_band_width.nr_scs =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_tdd_info.nr_freq_info.nr_sul_information.
                        nr_tx_band_width.nr_scs;

                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info.
                        nr_sul_information.nr_tx_band_width.nr_nrb =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_tdd_info.nr_freq_info.nr_sul_information.
                        nr_tx_band_width.nr_nrb;
                }

                /*filling freq num band count */
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info.num_freq_bands =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    choice_nr_mode.nr_tdd_info.nr_freq_info.num_freq_bands;

                /* TB_FIX2_START */
                for ( freq_num_count = RRC_ZERO; freq_num_count < p_x2ap_gl_ctx->
                        peer_enb_cell_info[peer_count].
                        peer_cell_info.choice_nr_mode.nr_tdd_info.nr_freq_info.
                        num_freq_bands; freq_num_count++ )
                    /* TB_FIX2_END */
                {
                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info.
                        freq_band_info[freq_num_count].freq_band_indicator_nr =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_tdd_info.nr_freq_info.freq_band_info[freq_num_count].
                        freq_band_indicator_nr;

                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info.
                        freq_band_info[freq_num_count].num_supported_sul_band =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        choice_nr_mode.nr_tdd_info.nr_freq_info.freq_band_info[freq_num_count].
                        num_supported_sul_band;

                    for ( sul_band_count = RRC_ZERO; sul_band_count < p_x2ap_gl_ctx->
                            peer_enb_cell_info[peer_count].
                            peer_cell_info.choice_nr_mode.nr_tdd_info.nr_freq_info.
                            freq_band_info[freq_num_count].num_supported_sul_band; sul_band_count++)
                    {
                        p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                            nr_neigh_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info.
                            freq_band_info[freq_num_count].
                            supported_sul_freq_band_indicator_nr[sul_band_count] =
                            p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                            choice_nr_mode.nr_tdd_info.nr_freq_info.freq_band_info[freq_num_count].
                            supported_sul_freq_band_indicator_nr[sul_band_count];
                    }
                }
            }

            /*
             * filling NR Frequency Info
             */
            /*filling bitmask of nr freq info for tdd*/
            p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                nr_neigh_info.nr_freq_info.bitmask |=
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                nr_freq_info.bitmask;

            /*filling nr_earfcn */
            p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                nr_neigh_info.nr_freq_info.nr_earfcn =
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                nr_freq_info.nr_earfcn;

            if( X2AP_NR_ARFCN_SUL_PRESENT & p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                peer_cell_info.nr_freq_info.bitmask)
            {
                /*filling nr earfcn sul*/
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_freq_info.nr_earfcn_sul =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    nr_freq_info.nr_earfcn_sul;

            }

            /* filling sul information*/
            if( X2AP_SUL_INFORMATION_PRESENT & p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].
                peer_cell_info.nr_freq_info.bitmask)
            {
                /*filling nr_sul_arfcn*/
                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_freq_info.nr_sul_information.nr_sul_arfcn =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    nr_freq_info.nr_sul_information.nr_sul_arfcn;

                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_freq_info.nr_sul_information.nr_tx_band_width.nr_scs =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    nr_freq_info.nr_sul_information.nr_tx_band_width.nr_scs;

                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_freq_info.nr_sul_information.nr_tx_band_width.nr_nrb =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    nr_freq_info.nr_sul_information.nr_tx_band_width.nr_nrb;

            }

            /*filling freq num band count */
            p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                nr_neigh_info.nr_freq_info.num_freq_bands =
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                nr_freq_info.num_freq_bands;

            for ( freq_num_count=RRC_ZERO; freq_num_count < p_x2ap_gl_ctx->
                 peer_enb_cell_info[peer_count].
                 peer_cell_info.nr_freq_info.num_freq_bands;freq_num_count++ )
            {

                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_freq_info.
                    freq_band_info[freq_num_count].freq_band_indicator_nr =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    nr_freq_info.freq_band_info[freq_num_count].freq_band_indicator_nr;

                p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                    nr_neigh_info.nr_freq_info.
                    freq_band_info[freq_num_count].num_supported_sul_band =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                    nr_freq_info.freq_band_info[freq_num_count].num_supported_sul_band;


                for ( sul_band_count = RRC_ZERO; sul_band_count < p_x2ap_gl_ctx->
                        peer_enb_cell_info[peer_count].
                        peer_cell_info.nr_freq_info.freq_band_info[freq_num_count].
                        num_supported_sul_band; sul_band_count++)
                {
                    p_x2_setup_req->eutra_cell_info.nr_neigh_cell_arr[nr_neigh_count].
                        nr_neigh_info.nr_freq_info.
                        freq_band_info[freq_num_count].
                        supported_sul_freq_band_indicator_nr[sul_band_count] =
                        p_x2ap_gl_ctx->peer_enb_cell_info[peer_count].peer_cell_info.
                        nr_freq_info.freq_band_info[freq_num_count].
                        supported_sul_freq_band_indicator_nr[sul_band_count];
                }
            }
            nr_neigh_count++;
        }
    }
/* TB_FIX_END */

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
#endif

/******************************************************************************
 * Function Name  : csc_cell_status_resp_hdl
 * Inputs         : p_msg_ptr -     Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when cell setup is UP & operational
 *                  received from CSC.This function is invoked per ENB context.
 ******************************************************************************/
x2ap_return_et csc_cell_status_resp_hdl (U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    U8                    cell_index = 0;
    U16 cell_count    =   0;
    x2ap_csc_cell_status_resp_t  *p_csc_cell_status_resp = X2AP_P_NULL;

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    p_csc_cell_status_resp =(x2ap_csc_cell_status_resp_t*)p_msg_ptr;

    for(cell_count=0; cell_count < (p_csc_cell_status_resp->num_served_cells);
            cell_count++)
    {
        if(X2AP_FAILURE == x2ap_get_cell_context_by_cell_id(p_x2ap_gl_ctx,
                p_csc_cell_status_resp->cell_status_info[cell_count].cell_index,
                &cell_index))
        {
            return retVal;
        }
        if (X2AP_TRUE == p_x2ap_gl_ctx->act_cell_info[cell_index].is_cell_active)
        {
            /* Mark the cell as active */
            p_x2ap_gl_ctx->act_cell_info[cell_index].is_cell_active = X2AP_TRUE;
            /* Check the Guard timer expired or not */
            if (!(p_x2ap_gl_ctx->cell_up_guard_tim_id))
            {
                /* Build and send the x2 setup request for peer eNB */
                retVal = x2setup_request_msg_init (p_msg_ptr,
                        p_x2ap_gl_ctx,
                        p_enb_ctx);
            }
            else
            {
                X2AP_TRACE(X2AP_INFO,"Wait for cell_up_guard_timer"
                        " Expiry & Update cell");
                retVal = X2AP_SUCCESS;
            }
        }
        else
        {
            X2AP_TRACE(X2AP_INFO,"Cell is not marked as active");
            retVal = X2AP_SUCCESS;
        }
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/******************************************************************************
 * Function Name  : x2ap_served_cell_info_update
 * Inputs         : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                  status - link status 
 *                  p_enb_ctx      Pointer to eNB context
 *                  x2_setup_req_t - pointer to the x2setup info
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to update the served cell info 
 *                  in the x2ap global context
 ******************************************************************************/
void x2ap_served_cell_info_update(x2ap_gb_context_t *p_x2ap_gl_ctx,
                    		  U8 status,
                                  U8 peer_enodeb_id,
                                  x2_setup_req_t *p_x2setup_req)
{
    U8 peer_enb_ct = X2AP_NULL;
    U8 cell_ct = X2AP_NULL;

    X2AP_ASSERT(PNULL!=p_x2ap_gl_ctx);
    X2AP_ASSERT(PNULL!=p_x2setup_req);

    X2AP_UT_TRACE_ENTER();

    for (peer_enb_ct = 0; peer_enb_ct<MAX_PEER_ENB; peer_enb_ct++)
    {
        if (peer_enb_ct == peer_enodeb_id)
        {
            p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].presence_flag = RRC_TRUE;
           /* p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].peer_gb_enb_id = p_x2setup_req->peer_gb_enb_id;*/
            p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].connection_status = status;

            if(X2AP_LINK_UP == p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].connection_status)
            {
                p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].num_served_cells = p_x2setup_req->num_served_cell;

                #ifdef ENDC_ENABLED
                /* MENB CHANGES - START */
                if ( X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT &
                    p_x2ap_gl_ctx->peer_enb_cntxt_arr[peer_enb_ct].bitmask )
                {
                    if ( ( PNULL != (p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info =
                                    x2ap_mem_get(sizeof(x2ap_ecgi_t)*p_x2setup_req->num_served_cell)) ) &&
                            (p_x2setup_req->num_served_cell <= MAX_SERVED_CELLS) )
                    {
                        for (cell_ct = X2AP_NULL; cell_ct < p_x2ap_gl_ctx->
                                x2ap_link_info[peer_enb_ct].num_served_cells; cell_ct++)
                        {
                            p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].presence_flag |=
                                X2AP_NR_SERVED_CELL_INFO_PRESENT;

                            X2AP_MEMCPY((void *)p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].
                                    p_nr_srvd_cell_info + (sizeof(x2ap_ecgi_t) * cell_ct),
                                    (const void*)(&p_x2setup_req->served_cell_arr[cell_ct].
                                        served_cell_info.cell_id),sizeof(x2ap_ecgi_t));
                        }
                    }
                }
                else
                {
                /* MENB CHANGES - END */
                #endif
                /*Bug_12881_fix_start*/
                if((PNULL != (p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].p_srvd_cell_info = x2ap_mem_get(sizeof(x2ap_ecgi_t)*p_x2setup_req->num_served_cell))) && (p_x2setup_req->num_served_cell <= MAX_SERVED_CELLS))
                {
                    for (cell_ct = 0; cell_ct < p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].num_served_cells; cell_ct++)
                    {
                            X2AP_MEMCPY((void *)p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].p_srvd_cell_info + (sizeof(x2ap_ecgi_t) * cell_ct),(const void*)(&p_x2setup_req->served_cell_arr[cell_ct].served_cell_info.cell_id),sizeof(x2ap_ecgi_t));/*Need t confirm*/
                    }
                }
                /*Bug_12881_fix_stop*/
                #ifdef ENDC_ENABLED
                /* MENB CHANGES - START */
                }
                /* MENB CHANGES - END */
                #endif
            }
            else
            {
                /* Free already existing served cell Info if any */
	        if(X2AP_P_NULL != p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].p_srvd_cell_info)
                {
                    x2ap_mem_free(p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].p_srvd_cell_info);
                    p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].p_srvd_cell_info = X2AP_P_NULL;
                }

                #ifdef ENDC_ENABLED
                /* MENB CHANGES - START */
                /*
                 * Free already existing NR served cell Info if any
                 */
                if(X2AP_P_NULL != p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info)
                {
                    x2ap_mem_free(p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info);
                    p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info = X2AP_P_NULL;
                }
                /* MENB CHANGES - END */
                #endif
            }
            break;
        }
        if(MAX_PEER_ENB == peer_enodeb_id)
        {
            X2AP_TRACE(X2AP_WARNING,"Peer EnodeB Id received is out of range : %d", peer_enodeb_id);
        }
    }
    X2AP_UT_TRACE_EXIT();
    
}

/*****************************************************************************
 * Function Name  : x2ap_send_link_up_ind_to_son 
 * Inputs         : p_x2_setup_resp - pointer to x2_setup_resp_t
 *                  p_x2ap_gl_ctx - pointer to X2AP global context
 *                  num_act_cell - num of active cells
 *                  p_act_cell_id - activated cell id
 *                  p_enb_comm_info - pointer to x2_enb_comm_info_t
 * Outputs        : None
 * Returns        : X2XP_SUCCESS/X2AP_FAILURE
 * Description    : this function sends X2AP Link Up indication to RRC module
 ********************************************************************************/
x2ap_return_et x2ap_send_link_up_ind_to_son(
        x2_setup_resp_t  *p_x2_setup_resp,
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        /*SPR 17777 +-*/
	x2_enb_comm_info_t* p_enb_comm_info)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    static x2ap_enb_link_up_ind_t link_up_ind;

    X2AP_UT_TRACE_ENTER();
    X2AP_MEMSET(&link_up_ind,X2AP_NULL,sizeof(x2ap_enb_link_up_ind_t));
    X2AP_MEMCPY(&link_up_ind.enb_comm_info, p_enb_comm_info , sizeof(x2_enb_comm_info_t));
    x2ap_build_x2_enb_link_up_ind(p_x2_setup_resp,
                              &link_up_ind,
                              /*SPR 17777 +-*/
                              p_x2ap_gl_ctx);

    if(RRC_FAILURE == rrc_x2apCommon_send_x2ap_enb_link_up_ind(
			&link_up_ind, RRC_X2AP_MODULE_ID, RRC_SON_MODULE_ID, 0, RRC_NULL))
    {
        return X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_build_x2_enb_link_up_ind 
 * Inputs         : p_x2_setup_resp - pointer to X2AP global context
 *                  p_link_up_ind - pointer to x2ap_enb_link_up_ind_t
 *                  num_act_cell - num of activated cells
 *                  p_act_cell_id - activated cell id
 *                  p_x2ap_gl_ctx - pointer to x2AP global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function  builds x2_enb link up indication 
 ********************************************************************************/
void x2ap_build_x2_enb_link_up_ind(x2_setup_resp_t           *p_x2_setup_resp,
                               x2ap_enb_link_up_ind_t *p_link_up_ind,
                               /*SPR 17777 +-*/
                               x2ap_gb_context_t         *p_x2ap_gl_ctx)
{
    U8 cell_id =0;
    U8                gu_count = 0;

    U16               ngh_srvd_cell_count = 0;
    U16               peer_ngh_cell_count = 0;
    X2AP_UT_TRACE_ENTER();
    
    /* Local eNB */
    X2AP_MEMCPY(&p_link_up_ind->enb1_gb_id,
                &p_x2ap_gl_ctx->global_enb_id,
                sizeof(x2_gb_enb_id_t));

    /* Peer eNB */
    X2AP_MEMCPY(&p_link_up_ind->enb2_gb_id,
                &p_x2_setup_resp->global_enb_id,
                sizeof(x2_gb_enb_id_t));

    p_link_up_ind->num_served_cell = p_x2_setup_resp->num_served_cell;

    /* peer served cell count */
    for(ngh_srvd_cell_count = 0; ngh_srvd_cell_count <
            p_x2_setup_resp->num_served_cell; ngh_srvd_cell_count++)
    {
		p_link_up_ind->served_cell_info[ngh_srvd_cell_count].cell_info.bitmask=
                p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].served_cell_info.bitmask;

        /* SPR-6463 */
        p_link_up_ind->served_cell_info[ngh_srvd_cell_count].cell_info.pci =
          (U16)p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].served_cell_info.pci;
        /* SPR-6463 */

    	x2ap_compose_cell_id_from_ecgi(
		p_x2_setup_resp->global_enb_id,
		p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].served_cell_info.cell_id.eutran_cell_id,
		&cell_id);
	X2AP_MEMCPY(
		&p_link_up_ind->served_cell_info[ngh_srvd_cell_count].cell_info.cell_id,
                &cell_id,
		sizeof(U8));
        X2AP_MEMCPY(
		&p_link_up_ind->served_cell_info[ngh_srvd_cell_count].cell_info.tac,
                &p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].served_cell_info.track_area_code.tac,
		MAX_TAC_BYTES);
        X2AP_MEMCPY(
		&p_link_up_ind->served_cell_info[ngh_srvd_cell_count].cell_info.bcast_plmn_info,
                &p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].served_cell_info.broadcast_plmn_info,
		sizeof(x2ap_broadcast_plmn_t));
        X2AP_MEMCPY(
		&p_link_up_ind->served_cell_info[ngh_srvd_cell_count].cell_info.choice_eutra_mode.choice_eutra_mode,
                &p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].served_cell_info.choice_eutra_mode.choice_eutra_mode,
		sizeof(x2ap_choice_eutra_mode_t));
        X2AP_MEMCPY(
		&p_link_up_ind->served_cell_info[ngh_srvd_cell_count].cell_info.num_antenna_port,
                &p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].served_cell_info.num_antenna_port,
		sizeof(U8));
        X2AP_MEMCPY(
		&p_link_up_ind->served_cell_info[ngh_srvd_cell_count].cell_info.prach_config,
                &p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].served_cell_info.prach_config,
		sizeof(x2ap_prach_configuration_info_t));
        X2AP_MEMCPY(
		&p_link_up_ind->served_cell_info[ngh_srvd_cell_count].cell_info.mbsfn_subframe_info,
                &p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].served_cell_info.mbsfn_subframe_info,
		sizeof(x2ap_mbsfn_subframe_info_t));
        /* SPR 10077 Fix Start */
        X2AP_MEMCPY((void*)&(p_link_up_ind->served_cell_info[ngh_srvd_cell_count].cell_info.csg_identity),
                       (const void* )&(p_x2_setup_resp->
                       served_cell_arr[ngh_srvd_cell_count].served_cell_info.csg_identity),
                       sizeof(p_link_up_ind->served_cell_info[ngh_srvd_cell_count].cell_info.csg_identity));
        /* SPR 10077 Fix Stop */
        
	/* Number of neighbours */
	p_link_up_ind->served_cell_info[ngh_srvd_cell_count].nbour_cell_info.num_neighbour_cell = 
		p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].nbour_cell_info.num_neighbour_cell;

	for(peer_ngh_cell_count = 0; peer_ngh_cell_count <
                p_x2_setup_resp->served_cell_arr[ngh_srvd_cell_count].
                  nbour_cell_info.num_neighbour_cell; peer_ngh_cell_count++)
        {
             /* ECGI */
	     p_link_up_ind->served_cell_info[ngh_srvd_cell_count].
             nbour_cell_info.neighbour_info_arr[
             peer_ngh_cell_count].ecgi = p_x2_setup_resp->
             served_cell_arr[ngh_srvd_cell_count].nbour_cell_info
             .neighbour_info_arr[peer_ngh_cell_count].ecgi;

             /* PCI */
             p_link_up_ind->served_cell_info[ngh_srvd_cell_count].
             nbour_cell_info.neighbour_info_arr[
             peer_ngh_cell_count].pci = p_x2_setup_resp->
             served_cell_arr[ngh_srvd_cell_count].nbour_cell_info
             .neighbour_info_arr[peer_ngh_cell_count].pci;

             /* Fill EARFCN */
             p_link_up_ind->served_cell_info[ngh_srvd_cell_count].
             nbour_cell_info.neighbour_info_arr[
             peer_ngh_cell_count].earfcn = p_x2_setup_resp->
             served_cell_arr[ngh_srvd_cell_count].nbour_cell_info
             .neighbour_info_arr[peer_ngh_cell_count].earfcn;
        }
    }
    
    if(p_x2_setup_resp->num_gu_group_id)
    {
        p_link_up_ind->gu_group_id.num_gu_group_id = p_x2_setup_resp->num_gu_group_id;
        p_link_up_ind->bitmask =X2AP_GU_GROUP_ID_LIST_PRESENT;
    
    for (gu_count = 0; gu_count < p_link_up_ind->gu_group_id.num_gu_group_id;gu_count++)
    {
        p_link_up_ind->gu_group_id.gu_id_list[gu_count] =
                              p_x2_setup_resp->group_id_arr[gu_count];
    }
    }
    X2AP_UT_TRACE_EXIT();
}


/* SPR 13707 Fix Start */
/******************************************************************************
 * Function Name  : x2_setup_active_x2_setup_req_evt_hdl 
 * Inputs         : p_msg_ptr -     Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when x2 setup request event is 
 *                  received from peer eNB.
 ******************************************************************************/
x2ap_return_et x2_setup_active_x2_setup_req_evt_hdl (U8                 *p_msg_ptr,
                                     		x2ap_gb_context_t  	*p_x2ap_gl_ctx,
                                     		x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    U16                   num_act_cell = 0;
    U16                   act_cell_id[MAX_NUM_CELL_IN_X2AP]={0};
    static x2_setup_req_t       x2_setup_req;
    x2ap_peer_enb_msg_t  *p_peer_enb_msg = X2AP_P_NULL;
    rrc_x2ap_error_indication_t error_indication;
    x2ap_error_ind_bool_t        send_err_ind;

	/*SPR_19067_FIX_START*/
	OSCTXT                       asn1_ctx;
	/*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_ENTER();
    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;
    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_setup_req,X2AP_NULL,sizeof(x2_setup_req_t));
    X2AP_MEMSET(&error_indication,X2AP_NULL,
                    sizeof(rrc_x2ap_error_indication_t));
    X2AP_MEMSET(&send_err_ind,X2AP_NULL,
                    sizeof(x2ap_error_ind_bool_t));

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP REQUEST Received");
    
    /* Check if there is any cell is operational or not */
    x2ap_check_active_cell(&num_act_cell,
                           act_cell_id,
                           p_x2ap_gl_ctx);

    /* Decode the ASN request */
    retVal = x2ap_decode_x2setup_req(
                p_x2ap_gl_ctx,
                p_enb_ctx,
                &x2_setup_req,
                (p_peer_enb_msg)->p_msg,
                &(p_peer_enb_msg->msg_len),
                &error_indication,
				/*SPR_19067_FIX_START*/
                &send_err_ind,
                &asn1_ctx);
        		/*SPR_19067_FIX_STOP*/

    if (X2AP_SUCCESS == retVal)
    { 
        X2AP_TRACE(X2AP_INFO,"X2 LINK is already up ");
        /* Build and send x2 setup resp to peer enb */
        X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP RESPONSE");
         retVal = x2ap_build_x2setup_resp(num_act_cell,
                 act_cell_id,
                 p_x2ap_gl_ctx,
                 p_enb_ctx); 
    }
    else
    {
        /* Send failure response without time to wait */
        retVal = x2ap_build_x2setup_fail_resp(p_x2ap_gl_ctx,
                                              p_enb_ctx,
                                              X2AP_FALSE,
                                              &error_indication,
                                              &send_err_ind);
    }

	/*SPR_19067_FIX_START*/
    rtFreeContext(&asn1_ctx);
   /*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/* SPR 13707 Fix Stop */

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
/******************************************************************************
 * Function Name  : x2ap_build_endc_x2setup_fail_resp
 * Inputs         : p_x2ap_gl_ctx      - pointer to X2AP global context
 *                : p_enb_ctx          - Pointer to eNB context
 *                  timeTowait_present - boolean value
 *                  p_error_indication - error indicatiob data 
 *                  p_snd_err_ind      - pointer to x2ap_error_ind_bool_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS
 *                  X2AP_FAILURE
 * Description    : This function is called to build and send ENDC x2 setup
 *                 failure response to peer gNB with or without time to wait IE.
 ******************************************************************************/
x2ap_return_et
x2ap_build_endc_x2setup_fail_resp
(
    x2ap_gb_context_t              *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t        *p_enb_ctx,
    x2ap_bool_et                   timeTowait_present,
    rrc_x2ap_error_indication_t    *p_error_indication,
    x2ap_error_ind_bool_t          *p_snd_err_ind
)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    x2_setup_fail_t       x2_setup_fail_resp;
    U8                    asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {X2AP_NULL};
    U16                   asn_msg_len = X2AP_NULL;
    x2ap_peer_enb_msg_t   peer_enb_msg;

    X2AP_UT_TRACE_ENTER();

    /*
     * Received pointer should not be NULL
     */
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_setup_fail_resp,X2AP_NULL,sizeof(x2_setup_fail_t));
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

    /*
     * Build the ENDC x2 setup failure resp
     */
    if ( X2AP_TRUE == timeTowait_present )
    {
        x2_setup_fail_resp.time_to_wait = 
            (x2ap_time_to_wait_et)p_x2ap_gl_ctx->config_param.
            timer_conf_info.x2ap_time_to_wait_index;
    }
    else
    {
        x2_setup_fail_resp.time_to_wait = SEC_1;
    }
    
    if ( X2AP_P_NULL != p_error_indication )
    {
        switch (p_error_indication->cause.t)
        {
            case T_x2ap_Cause_transport:
            {
                x2_setup_fail_resp.cause.cause_type = T_x2ap_Cause_transport;
                x2_setup_fail_resp.cause.cause_info.transport_cause =
                    p_error_indication->cause.u.transport;
            }
            break;
            case T_x2ap_Cause_protocol:
            {
                x2_setup_fail_resp.cause.cause_type = T_x2ap_Cause_protocol;
                x2_setup_fail_resp.cause.cause_info.protocol_cause =
                    p_error_indication->cause.u.protocol;
            }
            break;
            case T_x2ap_Cause_misc:
            {
                x2_setup_fail_resp.cause.cause_type = T_x2ap_Cause_misc;
                x2_setup_fail_resp.cause.cause_info.misc_cause =
                    p_error_indication->cause.u.misc;
            }
            break;
            case T_x2ap_Cause_radioNetwork:
            {
                x2_setup_fail_resp.cause.cause_type = T_x2ap_Cause_radioNetwork;
                x2_setup_fail_resp.cause.cause_info.radio_nw_cause =
                    p_error_indication->cause.u.radioNetwork;
            }
            break;
            default:
            {
                x2_setup_fail_resp.cause.cause_type = T_x2ap_Cause_radioNetwork;
                x2_setup_fail_resp.cause.cause_info.radio_nw_cause = x2ap_unspecified_2;
            }
            break;
        }
    }
    else
    {
        x2_setup_fail_resp.cause.cause_type = T_x2ap_Cause_radioNetwork;
        x2_setup_fail_resp.cause.cause_info.radio_nw_cause = x2ap_unspecified_2;
    }

    X2AP_MEMSET(&(x2_setup_fail_resp.critc_diagno),
                 X2AP_NULL,
                sizeof(x2ap_crit_diagno_t));

    /*
     * ASN Encoding
     */
     X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP FAIL");
    retVal = x2ap_encode_endc_x2setup_failure_resp(&(x2_setup_fail_resp),
                                          asn_msg_buff,
                                          &asn_msg_len,
                                          p_error_indication,
                                          p_snd_err_ind);

    if ( X2AP_SUCCESS == retVal )
    {
        peer_enb_msg.p_msg = asn_msg_buff;
        peer_enb_msg.msg_len = asn_msg_len;
        peer_enb_msg.stream_id = p_enb_ctx->stream_info.
            stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];

        /*
         * Sending the ENDC X2 SETUP REQUEST to Peer gNB via SCTP
         */
        retVal = x2ap_sctp_sm_entry (X2AP_SCTP_SEND_DATA_EV, 
                (U8 *)(&peer_enb_msg),
                p_x2ap_gl_ctx,
                p_enb_ctx);
        if (X2AP_SUCCESS == retVal)
        {
            X2AP_TRACE(X2AP_INFO, "ENDC X2 SETUP FAILURE Msg sent to "
                    "Peer ENodeB ID : %d", p_enb_ctx->peer_enodeb_id);
        }
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR,"[%s]::ASN Encoding of ENDC X2 Setup Failed"
                   ,__FUNCTION__);
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/******************************************************************************
 * Function Name  : endc_x2setup_req_idle_hdl
 * Inputs         : p_msg_ptr -     Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS
 *                  X2AP_FAILURE
 * Description    : This function is called when EN DC x2 setup request event is
 *                  received from peer gNB.
 ******************************************************************************/
x2ap_return_et
endc_x2setup_req_idle_hdl
(
    U8                         *p_msg_ptr,
    x2ap_gb_context_t          *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t    *p_enb_ctx
)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    U16                   num_act_cell = X2AP_NULL;
    U16                   act_cell_id[MAX_NUM_CELL_IN_X2AP]={X2AP_NULL};
    x2ap_bool_et          time_to_wait_flag = X2AP_FALSE; 
    rrc_x2ap_error_indication_t    error_indication;
 
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&error_indication,X2AP_NULL,
                    sizeof(rrc_x2ap_error_indication_t));

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: ENDC X2 SETUP REQUEST Received");            

    /*
     * if the cell setup guard timer is running
     */
    if ( p_x2ap_gl_ctx->config_param.timer_conf_info.bitmask
            & X2AP_TIME_TO_WAIT_INDEX_PRESENT )
    {
        time_to_wait_flag = X2AP_TRUE;
    } 
    if ( p_x2ap_gl_ctx->cell_up_guard_tim_id )
    {
        /* Fill Cause in Error Indication */
            error_indication.cause.t = T_x2ap_Cause_radioNetwork;
            error_indication.cause.u.radioNetwork = x2ap_cell_not_available;
        /*
         * send failure response with time to wait
         */
        retVal = x2ap_build_endc_x2setup_fail_resp(p_x2ap_gl_ctx,
                                              p_enb_ctx,
                                              time_to_wait_flag,
                                              &error_indication,//X2AP_P_NULL,
                                              X2AP_P_NULL);
    }
    else
    {
        /*
         * Check if there is any cell is operational or not
         */
        x2ap_check_active_cell(&num_act_cell,
                               act_cell_id,
                               p_x2ap_gl_ctx);

        /*
         * if atleast one cell is active
         */
        if ( !num_act_cell )
        {
            /* Fill Cause in Error Indication */
            error_indication.cause.t = T_x2ap_Cause_radioNetwork;
            error_indication.cause.u.radioNetwork = x2ap_cell_not_available;
            /*
             * send failure response with time to wait
             */
            retVal = x2ap_build_endc_x2setup_fail_resp(p_x2ap_gl_ctx,
                                                  p_enb_ctx,
                                                  time_to_wait_flag,
                                                  &error_indication,//X2AP_P_NULL,
                                                  X2AP_P_NULL);
        }
        else
        {
            retVal = endc_x2_setup_req_evt_hdl(p_msg_ptr,
                                          p_x2ap_gl_ctx,
                                          p_enb_ctx);
        }
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : endc_x2_setup_req_w_endc_x2_setup_resp_evt_hdl 
 * Inputs         : p_msg_ptr     -  Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx     -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS
 *                  X2AP_FAILURE
 * Description    : This function is called when endc x2 setup request event is 
 *                  received from peer gNB in X2_SETUP_W_X2_SETUP_RESP_ST state.
 ******************************************************************************/
x2ap_return_et
endc_x2_setup_req_w_x2_setup_resp_evt_hdl
(
    U8                         *p_msg_ptr,
    x2ap_gb_context_t          *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t    *p_enb_ctx
)
{
    U32                             peer_cell_count = X2AP_NULL;
    x2ap_return_et                  retVal = X2AP_FAILURE;
    U16                             num_act_cell = X2AP_NULL;
    U16                             act_cell_id[MAX_NUM_CELL_IN_X2AP]={X2AP_NULL};
    static en_dc_x2_setup_req_t      x2_setup_req;
    x2ap_peer_enb_msg_t             *p_peer_enb_msg = X2AP_P_NULL;
    sctp_gb_gnb_id_search_node_t    *p_gnb_id_node = X2AP_P_NULL;
    rrc_x2ap_error_indication_t     error_indication;
    x2ap_error_ind_bool_t           send_err_ind;
    OSCTXT                          asn1_ctx;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;

    X2AP_MEMSET(&x2_setup_req,X2AP_NULL,sizeof(en_dc_x2_setup_req_t));
    X2AP_MEMSET(&error_indication,X2AP_NULL,
                    sizeof(rrc_x2ap_error_indication_t));
    X2AP_MEMSET(&send_err_ind,X2AP_NULL,
                    sizeof(x2ap_error_ind_bool_t));

   X2AP_TRACE(X2AP_INFO,"X2AP_MSG: ENDC X2 SETUP REQUEST Received");

    do
    {
        /*
         * Check if there is any cell is operational or not
         */
        x2ap_check_active_cell(&num_act_cell,
                act_cell_id,
                p_x2ap_gl_ctx);
        if ( !num_act_cell )
        {
            x2ap_bool_et    time_to_wait_flag = X2AP_FALSE;  

            if ( p_x2ap_gl_ctx->config_param.timer_conf_info.bitmask
                    & X2AP_TIME_TO_WAIT_INDEX_PRESENT )
            {
                time_to_wait_flag = X2AP_TRUE;
            }

            /* Fill Cause in Error Indication */
            error_indication.cause.t = T_x2ap_Cause_radioNetwork;
            error_indication.cause.u.radioNetwork = x2ap_cell_not_available;

            /* send failure response with time to wait */
            retVal = x2ap_build_endc_x2setup_fail_resp(p_x2ap_gl_ctx,
                    p_enb_ctx,
                    time_to_wait_flag,
                    &error_indication,
                    X2AP_P_NULL);

            /*
             * change state to IDLE
             */
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_IDLE_ST;
            p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = X2AP_NULL;
        
            if (X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId 
            || X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id)
            {
                if (X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId)
                {
                    /*
                    * Stop X2 Setup Response guard Timer
                    */
                    x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
                    p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;
                }

                if (X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id)
                {
                    /*
                    * Stop X2 Setup time to wait guard Timer
                    */
                    x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id);
                    p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
                }
            } 

            X2AP_TRACE(X2AP_INFO, "%s: Active cell count is 0."
                    " ENDC X2Setup Failure Resp Sent with time_to_wait.",
                    __FUNCTION__);

            break;
        }

        /*
         * Decode the ASN request
         */
        X2AP_TRACE(X2AP_DETAILED,"Decode ENDC x2 setup req \n");
        retVal = x2ap_decode_endc_x2setup_req(
                p_x2ap_gl_ctx,
                p_enb_ctx,
                &x2_setup_req,
                (p_peer_enb_msg)->p_msg,
                &(p_peer_enb_msg->msg_len),
                &error_indication,
                &send_err_ind,
                &asn1_ctx);
        if ( X2AP_SUCCESS == retVal )
        { 
            if ( ( X2AP_P_NULL !=  p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId ) ||
                    ( X2AP_P_NULL !=  
                      p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id ) )
            {
                if ( X2AP_P_NULL != p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id )
                {
                    x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id);
                    p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
                }

                if ( X2AP_P_NULL != p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId )
                {
                    x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
                    p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_NULL;
                }
                p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_ACTIVE_ST;
                p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = X2AP_NULL;
                p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_NULL;
                p_enb_ctx->op_status = OPERATIONAL;
                p_enb_ctx->is_x2setup_sent = X2AP_FALSE;
            }

            p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_ACTIVE_ST;
            p_enb_ctx->op_status = OPERATIONAL;

            /*
             * update the cell info in the global context
             */
            x2ap_update_nr_cell_info_for_req(&x2_setup_req,
                    num_act_cell,
                    act_cell_id,
                    p_x2ap_gl_ctx);

            p_enb_ctx->bitmask |= X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT;
            p_enb_ctx->gb_gnb_id = x2_setup_req.nr_cell_info.global_gnb_id;

            p_gnb_id_node = (sctp_gb_gnb_id_search_node_t *)x2ap_st_find_tree_node(
                    &(p_x2ap_gl_ctx->enb_ctxt_trees.gnb_id_search_tree),
                    &(p_enb_ctx->gb_gnb_id));
            if ( !p_gnb_id_node )
            {    
                p_gnb_id_node = x2ap_mem_get(sizeof(sctp_gb_gnb_id_search_node_t));

                if ( PNULL != p_gnb_id_node )
                {
                    p_gnb_id_node->gb_gnb_id = p_enb_ctx->gb_gnb_id;                       
                    p_gnb_id_node->p_enb_context = p_enb_ctx;
                    x2ap_st_insert_node(
                            &(p_x2ap_gl_ctx->enb_ctxt_trees.gnb_id_search_tree),
                            &(p_gnb_id_node->nodeAnchor));
                }
            }    

            /*
             * Update the x2ap global context to save the served cell info of the peer GnodeB
             */
            x2ap_served_nr_cell_info_update(p_x2ap_gl_ctx,
                    X2AP_LINK_UP,
                    p_enb_ctx->peer_enodeb_id,
                    &x2_setup_req);        

            /* set procedure sm context state to ACTIVE */
            x2ap_active_procedure_sm_ctx_state(p_enb_ctx);
            
            X2AP_TRACE(X2AP_DETAILED,"Sending ENDC x2 setup resp to peer gnb \n");
            /*
             * Build and send ENDC x2 setup resp to peer gnb
             */
            X2AP_TRACE(X2AP_INFO,"X2AP_MSG: ENDC X2 SETUP RESPONSE");
            retVal = x2ap_build_endc_x2setup_resp(num_act_cell,
                            act_cell_id,
                            p_x2ap_gl_ctx,
                            p_enb_ctx); 
            if ( !retVal )
            {
                X2AP_TRACE(X2AP_ERROR,"Sending ENDC x2 setup resp to peer gnb failed.\n");
                break;
            }

            p_enb_ctx->flag_for_consecutive_endc_x2setup_resp_failure = X2AP_TRUE;

            for ( peer_cell_count = X2AP_NULL; peer_cell_count < p_x2ap_gl_ctx->num_peer_enb_cells;
                peer_cell_count++ )
            {
                if ( RRC_TRUE == p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].current_cell_info )
                {
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].current_cell_info = RRC_FALSE;
                }
            }

            /*
             * Send X2 Link UP indication to UECC
             */
            if ( X2AP_FAILURE ==
                    x2ap_build_and_send_nr_status_ind(p_enb_ctx,
                        p_x2ap_gl_ctx,
                        X2AP_LINK_UP,
                        x2_setup_req.nr_cell_info.num_served_nr_cell,
                        &(x2_setup_req.nr_cell_info.served_nr_cell_arr[X2AP_NULL]),
                        p_x2ap_gl_ctx->total_ue_supported) )
            {
                X2AP_TRACE(X2AP_WARNING, "%s:"
                        " rrc_send_message send failed ENDC x2ap status \
                        msg ind to UECC",
                        __FUNCTION__);
                break;
            }

#if 0
            if ( p_x2ap_gl_ctx->is_son_present )
            {
            //todo son
                /*
                 * Send X2 Link UP indication to SON
                 */
                retVal = x2ap_send_link_up_ind_to_son((x2_setup_resp_t *)&x2_setup_req,
                        p_x2ap_gl_ctx,
                        &p_enb_ctx->gnb_comm_info);
                if ( !retVal )
                {
                    X2AP_TRACE(X2AP_ERROR,"Sending gnb link up indication failed to SON.\n");
                    break;
                }
            }
            else
#endif
            {
                retVal = x2ap_send_link_up_ind_to_oam(p_enb_ctx);
                if ( !retVal )
                {
                    X2AP_TRACE(X2AP_ERROR,"Sending ENDC X2 link up indication to OAM failed.\n");
                    break;
                }

                /*
                 * Send X2 Link UP indication to RRM
                 */
                retVal = x2ap_send_link_up_ind_to_rrm_endc(p_x2ap_gl_ctx,
                    (en_dc_x2_setup_req_t *)&x2_setup_req);
                if ( !retVal )
                {
                    X2AP_TRACE(X2AP_ERROR,"Sending ENDC X2 link up indication to RRM failed.\n");
                    break;
                }
            }

            /*
             * Invoke UE Associated sig msg FSM
             */
            x2ap_ue_asso_sig_sm_entry(X2AP_UE_ASSO_SIG_X2_LINK_UP_EV,
                    p_msg_ptr,
                    p_x2ap_gl_ctx,
                    p_enb_ctx, X2AP_NULL);  
        }
        else
        {
            /*
             * Send failure response without time to wait
             */
            retVal = x2ap_build_endc_x2setup_fail_resp(p_x2ap_gl_ctx,
                    p_enb_ctx,
                    X2AP_FALSE,
                    &error_indication,
                    &send_err_ind);

            /* bug 114842 */
            /*
             * change state to IDLE
             */
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_IDLE_ST;
            p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = X2AP_NULL;
        
            if (X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId 
            || X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id)
            {
                if (X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId)
                {
                    /*
                    * Stop X2 Setup Response guard Timer
                    */
                    x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
                    p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;
                }

                if (X2AP_NULL != p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id)
                {
                    /*
                    * Stop X2 Setup time to wait guard Timer
                    */
                    x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id);
                    p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
                }
            }
        }
    }while(X2AP_NULL);

    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : endc_x2_setup_fail_resp_hdl
 * Inputs         : p_msg_ptr     -  Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx     -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS
 *                  X2AP_FAILURE
 * Description    : This function is called when ENDC x2 setup failure response 
 *                  is received from peer gNB.
 ******************************************************************************/
x2ap_return_et
endc_x2_setup_fail_resp_hdl
(
    U8                         *p_msg_ptr,
    x2ap_gb_context_t          *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t    *p_enb_ctx
)
{
    x2ap_return_et        retVal = X2AP_FAILURE;
    x2_setup_fail_t       x2_setup_fail_resp;
    x2ap_peer_enb_msg_t   *p_peer_enb_msg = X2AP_P_NULL;
    x2ap_timer_buffer_t   x2_setup_rsp_timer_buf;
    x2_gb_gnb_id_t        gb_gnb_id;
    U32                   timer_value = X2AP_NULL;
    OSCTXT                       asn1_ctx;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_setup_rsp_timer_buf,X2AP_NULL,sizeof(x2ap_timer_buffer_t));
    X2AP_MEMSET(&x2_setup_fail_resp,X2AP_NULL,sizeof(x2_setup_fail_t));
    X2AP_MEMSET(&gb_gnb_id,X2AP_NULL,sizeof(x2_gb_gnb_id_t));

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: ENDC X2 SETUP FAIL Received");

    p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;

    /*
     * Decode the ASN request
     */
    retVal = x2ap_decode_endc_x2setup_failure_resp(
            p_x2ap_gl_ctx,
            p_enb_ctx,
            &x2_setup_fail_resp,
            (p_peer_enb_msg)->p_msg,
            &(p_peer_enb_msg->msg_len),
    	    &asn1_ctx);
    if ( X2AP_SUCCESS == retVal )
    {
        /*
         * Stop X2 Setup Response guard Timer
         */
        x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
        p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;

        X2AP_TRACE(X2AP_INFO,
                                "X2AP_MSG: ENDC X2 SETUP FAIL");

        /*
         * Adding 1 for the first attempt i.e. "try" count, other count is "retry" count
         */
        if ( ( ( p_x2ap_gl_ctx->config_param.timer_conf_info.
                  x2ap_setup_retry_cnt + X2AP_ONE ) == 
                     p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count ) ||
             ( !( X2_SETUP_FAIL_TIME_TO_WAIT_PRESENT & x2_setup_fail_resp.bitmask ) ) ||
             ( PROTOCOL_CAUSE ==  x2_setup_fail_resp.cause.cause_type ) )
        {
            if ( X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT &
                p_enb_ctx->bitmask )
            {
                gb_gnb_id = p_enb_ctx->gb_gnb_id;
            }

            /*
             * Send X2AP_SCTP_CONN_CLOSE_EV event to SCTP SM
             */
            retVal = x2ap_sctp_sm_entry(X2AP_SCTP_CONN_CLOSE_EV,
                                        (U8 *)&gb_gnb_id,
                                        p_x2ap_gl_ctx,
                                        p_enb_ctx );
            if ( X2AP_SUCCESS == retVal )
            {
                X2AP_TRACE(X2AP_INFO,"X2AP_MSG: Sent SCTP CONN CLOSE");
            }
            else
            {
                X2AP_TRACE(X2AP_ERROR,"X2AP_MSG: Sent SCTP CONN CLOSE failure");
            }

            /*
             * Change state to IDLE
             */
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state =
                                                 X2_SETUP_IDLE_ST;

            /*
             * reset the timer ids and retry count
             */
            p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = X2AP_NULL;
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;
            p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
 
        }
        else if ( X2_SETUP_FAIL_TIME_TO_WAIT_PRESENT & x2_setup_fail_resp.bitmask )
        {
            /*
             * Start guard timer for ENDC X2_Setup_Rsp
             */
            x2_setup_rsp_timer_buf.timer_type =
                                    X2AP_X2_SETUP_TIME_TO_WAIT_TIMER;
            x2_setup_rsp_timer_buf.auto_delete = X2AP_TRUE;
            x2_setup_rsp_timer_buf.sd = p_enb_ctx->sctp_sd;

#ifdef LKSCTP
            if ( p_enb_ctx->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT )
            {
                l3_strncpy_wrapper((S8*)x2_setup_rsp_timer_buf.ip_addr,
                        (const S8*)p_enb_ctx->enb_comm_info.ipv6_addr[X2AP_NULL].ipv6_addr,
                        MAX_X2AP_IPV6_ADDRESS_LENGTH);

            }
            else
            {
                l3_strncpy_wrapper((S8*)x2_setup_rsp_timer_buf.ip_addr,
                        (const S8*)p_enb_ctx->enb_comm_info.ip_addr[X2AP_NULL].ip_addr,
                        MAX_X2AP_IP_ADDRESS_LENGTH);
            }
#endif

            switch ( x2_setup_fail_resp.time_to_wait )
            {
                case SEC_1:
                    timer_value = 1*1000;
                    break;  
                case SEC_2:
                    timer_value = 2*1000;
                    break;  
                case SEC_5:
                    timer_value = 5*1000;
                    break;  
                case SEC_10:
                    timer_value = 10*1000;
                    break;  
                case SEC_20:
                    timer_value = 20*1000;
                    break;  
                case SEC_60:
                    timer_value = 60*1000;
                    break; 
                default:  
                    X2AP_TRACE(X2AP_INFO, 
                        "[%s] [Line no:%d] Invalid Time to" 
                        " wait value received", 
                         __FUNCTION__, __LINE__);
			        rtFreeContext(&asn1_ctx);
                return X2AP_FAILURE;
            }
           
            p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = 
                         x2ap_start_timer(timer_value,
                                          &x2_setup_rsp_timer_buf,
                                          sizeof(x2ap_timer_buffer_t),
                                          X2AP_FALSE);
            
            /*
             * Change state to X2_SETUP_TIME_TO_WAIT_EXP_ST
             */
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state =
                                          X2_SETUP_TIME_TO_WAIT_EXP_ST;
        }
    }
    else
    {
        /*
         * Send Error Indication to peer gNB
         */
        X2AP_TRACE(X2AP_INFO,"---Error Indication Detected---");
        retVal = X2AP_SUCCESS;
    } 

    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/******************************************************************************
 * Function Name  : endc_x2_setup_active_fail_resp_hdl
 * Inputs         : p_msg_ptr     -  Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx     -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS
 *                  X2AP_FAILURE
 * Description    : This function is called to handle ENDC X2 Setup Resp Failure 
 *                  in ACTIVE State
 ******************************************************************************/
x2ap_return_et
endc_x2_setup_active_fail_resp_hdl
(
    U8                         *p_msg_ptr,
    x2ap_gb_context_t          *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t    *p_enb_ctx
)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;

    X2AP_UT_TRACE_ENTER();

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: ENDC X2 SETUP FAIL Received");

    /*
     * Check if this is the first message recived from peer eNB after sending ENDC X2
     * Setup Response
     */
    if ( X2AP_TRUE == p_enb_ctx->flag_for_consecutive_endc_x2setup_resp_failure )
    {
        X2AP_TRACE(X2AP_WARNING,"[%s]::Recieved consecutive ENDC_X2_SETUP_RESP_FAILURE"
                " after sending ENDC X2 Setup Response ",
                __FUNCTION__);

        p_enb_ctx->flag_for_consecutive_endc_x2setup_resp_failure = X2AP_FALSE;

        /*
         * Call Failure Response Handler
         */
        if ( X2AP_SUCCESS == endc_x2_setup_fail_resp_hdl(p_msg_ptr, p_x2ap_gl_ctx,p_enb_ctx) )
        {
            /*
             * Send Link Down Indication
             */
            if ( p_enb_ctx->op_status == OPERATIONAL ) 
            {
                p_enb_ctx->op_status = ASSOCIATED;
                p_enb_ctx->is_x2setup_sent = X2AP_FALSE;

                /*
                 * Send X2 Link DOWN indication to UECC
                 */
                if ( X2AP_FAILURE ==
                        x2ap_build_and_send_nr_status_ind(
                            p_enb_ctx,
                            p_x2ap_gl_ctx, 
                            X2AP_LINK_DOWN,
                            X2AP_NULL,
                            X2AP_P_NULL,
                            p_x2ap_gl_ctx->total_ue_supported) )
                {
                    X2AP_TRACE(X2AP_WARNING, "%s:"
                            " rrc_send_message send failed ENDC x2ap status \
                            msg ind",
                            __FUNCTION__);
                }

                if ( !p_enb_ctx->is_del_enb_ongoing )
                {
                    if ( p_x2ap_gl_ctx->is_son_present )
                    {
//todo son
#if 0
                        /*
                         * Send X2 link down to SON
                         */
                        retVal = x2ap_send_link_down_ind_to_son(
                                p_x2ap_gl_ctx, p_enb_ctx);
                        if ( !retVal )
                        {
                            X2AP_TRACE(X2AP_ERROR,"Sending ENDC X2 link up indication to SON failed.\n");
                        }
#endif
                    }
                    else
                    {
                        /*
                         * Send X2 link down to OAM and RRM
                         */
                        retVal = x2ap_send_link_down_ind_to_oam_rrm(p_enb_ctx);
                        if ( !retVal )
                        {
                            X2AP_TRACE(X2AP_ERROR,"Sending ENDC X2 link up indication to OAM-RRM failed.\n");
                        }
                    }
                }

            }
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR,"[%s]::Error in handling ENDC x2 setup response failure"
                    " Ignoring Message",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
        }
    }
    else
    {
        X2AP_TRACE(X2AP_WARNING,"[%s]::Recieved Invalid Message ENDC_X2_SETUP_RESP_FAILURE:"
                " Ignoring Message",
                __FUNCTION__);
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}


/******************************************************************************
 * Function Name  : endc_x2_setup_req_evt_hdl
 * Inputs         : p_msg_ptr     -  Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx     -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS
 *                  X2AP_FAILURE
 * Description    : This function is called when ENDC x2 setup request event is 
 *                  received from peer gNB.
 ******************************************************************************/
x2ap_return_et
endc_x2_setup_req_evt_hdl
(
    U8                         *p_msg_ptr,
    x2ap_gb_context_t          *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t    *p_enb_ctx
)
{
    x2ap_return_et                  retVal = X2AP_FAILURE;
    U16                             num_act_cell = X2AP_NULL;
    U16                             act_cell_id[MAX_NUM_CELL_IN_X2AP]={X2AP_NULL};
    static en_dc_x2_setup_req_t      x2_setup_req;
    x2ap_peer_enb_msg_t             *p_peer_enb_msg = X2AP_P_NULL;
    sctp_gb_gnb_id_search_node_t    *p_gnb_id_node = X2AP_P_NULL;
    rrc_x2ap_error_indication_t     error_indication;
    x2ap_error_ind_bool_t           send_err_ind;
	OSCTXT                          asn1_ctx;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_setup_req,X2AP_NULL,sizeof(en_dc_x2_setup_req_t));
    X2AP_MEMSET(&error_indication,X2AP_NULL,
                    sizeof(rrc_x2ap_error_indication_t));
    X2AP_MEMSET(&send_err_ind,X2AP_NULL,
                    sizeof(x2ap_error_ind_bool_t));

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: ENDC X2 SETUP REQUEST Received");

    p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;

    /*
     * Decode the ASN request
     */
    X2AP_TRACE(X2AP_DETAILED,"Decoding ENDC x2 setup req received from peer gnb \n");
    retVal = x2ap_decode_endc_x2setup_req(
                p_x2ap_gl_ctx,
                p_enb_ctx,
                &x2_setup_req,
                (p_peer_enb_msg)->p_msg,
                &(p_peer_enb_msg->msg_len),
                &error_indication,
                &send_err_ind,
                &asn1_ctx);

    if ( X2AP_SUCCESS == retVal )
    { 
        if ( X2AP_P_NULL != p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId
                || ( X2AP_P_NULL !=  
                    p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id ) )
        {
            if ( X2AP_P_NULL !=  p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id )
            {
                x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id);
                p_enb_ctx->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
            }
            if ( X2AP_P_NULL !=  p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId )
            {
                x2ap_stop_timer(p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId);
                p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_NULL;
            }
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_ACTIVE_ST;
            p_enb_ctx->x2_setup_sm_ctx.x2setup_retry_count = X2AP_NULL;
            p_enb_ctx->x2_setup_sm_ctx.x2Setup_timerId = X2AP_NULL;
            p_enb_ctx->op_status = OPERATIONAL;
            p_enb_ctx->is_x2setup_sent = X2AP_FALSE;
        }

        p_enb_ctx->x2_setup_sm_ctx.x2Setup_curr_state = X2_SETUP_ACTIVE_ST;
        p_enb_ctx->op_status = OPERATIONAL;

        /* Bug 116736  */ 
        /*
             * Check if there is any cell is operational or not
             */
       x2ap_check_active_cell(&num_act_cell,
                              act_cell_id,
                              p_x2ap_gl_ctx);

        /*
         * update the NR cell info in the global context
         */
        x2ap_update_nr_cell_info_for_req(&x2_setup_req,
                num_act_cell,
                act_cell_id,
                p_x2ap_gl_ctx);

        p_enb_ctx->bitmask |= X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT;
        p_enb_ctx->gb_gnb_id = x2_setup_req.nr_cell_info.global_gnb_id;

        p_gnb_id_node = (sctp_gb_gnb_id_search_node_t *)x2ap_st_find_tree_node(
                &(p_x2ap_gl_ctx->enb_ctxt_trees.gnb_id_search_tree),
                &(p_enb_ctx->gb_gnb_id));
        if ( !p_gnb_id_node )
        {    
            p_gnb_id_node = x2ap_mem_get(sizeof(sctp_gb_gnb_id_search_node_t));
            if ( PNULL != p_gnb_id_node )
            {
                p_gnb_id_node->gb_gnb_id = p_enb_ctx->gb_gnb_id;                       
                p_gnb_id_node->p_enb_context = p_enb_ctx;
                x2ap_st_insert_node(
                        &(p_x2ap_gl_ctx->enb_ctxt_trees.gnb_id_search_tree),
                        &(p_gnb_id_node->nodeAnchor));
            }
        }
     
        /*
         * Update the x2ap global context to save the served cell info of the peer GnodeB
         */
        x2ap_served_nr_cell_info_update(p_x2ap_gl_ctx,
              X2AP_LINK_UP,
  		      p_enb_ctx->peer_enodeb_id,
              &x2_setup_req);        

        /* set procedure sm context state to ACTIVE */
        x2ap_active_procedure_sm_ctx_state(p_enb_ctx);

        /*
         * Build and send x2 setup resp to peer gnb
         */
        X2AP_TRACE(X2AP_INFO,"X2AP_MSG: ENDC X2 SETUP RESPONSE");
	    retVal = x2ap_build_endc_x2setup_resp(num_act_cell,
			 act_cell_id,
			 p_x2ap_gl_ctx,
			 p_enb_ctx); 

	    if ( X2AP_SUCCESS != retVal )
        {
            X2AP_TRACE(X2AP_WARNING,"Failure returned by function x2ap_build_endc_x2setup_resp ");
        }

	    /*
         * Send ENDC X2 Link UP indication to UECC
         */
	    if ( X2AP_FAILURE ==
			 x2ap_build_and_send_nr_status_ind(p_enb_ctx,
                        p_x2ap_gl_ctx,
                        X2AP_LINK_UP,
                        x2_setup_req.nr_cell_info.num_served_nr_cell,
                        &(x2_setup_req.nr_cell_info.served_nr_cell_arr[X2AP_NULL]),
                        p_x2ap_gl_ctx->total_ue_supported))
        {
                X2AP_TRACE(X2AP_WARNING, "%s:"
                        " rrc_send_message send failed x2ap status \
                        NR msg ind",
                        __FUNCTION__);
        }
	    
#if 0
	    if ( p_x2ap_gl_ctx->is_son_present )
        {
            //todo son
            /*
             * Send X2 Link UP indication to SON
             */
            retVal = x2ap_send_link_up_ind_to_son((x2_setup_resp_t *)&x2_setup_req,
                    p_x2ap_gl_ctx,
                    &p_enb_ctx->enb_comm_info);
            if ( !retVal )
            {
                X2AP_TRACE(X2AP_ERROR,"Sending Gnb link up indication failed to SON.\n");
            }
        }
        else
#endif
        {
            /*
             * Send X2 Link UP indication to OAM
             */
            retVal = x2ap_send_link_up_ind_to_oam(p_enb_ctx);
            if ( !retVal )
            {
               	X2AP_TRACE(X2AP_ERROR,"Sending Gnb link up indication failed to OAM.\n");
            }

            /*
             * Send X2 Link UP indication to RRM
             */
            retVal = x2ap_send_link_up_ind_to_rrm_endc(p_x2ap_gl_ctx,
                (en_dc_x2_setup_req_t *)&x2_setup_req);
            if ( !retVal )
            {
               	X2AP_TRACE(X2AP_ERROR,"Sending Gnb link up indication failed to RRM.\n");
            }
        }

        ALARM_MSG_L3(RRC_MODULE_ID, RRC_X2AP_CONNECTION_SUCCESS_ALARM_ID, NOTIFICATION, INVALID_ERR_CAUSE_ID);
            
        /*
         * Invoke UE Associated sig msg FSM
         */
        x2ap_ue_asso_sig_sm_entry(X2AP_UE_ASSO_SIG_X2_LINK_UP_EV,
                    p_msg_ptr,
                    p_x2ap_gl_ctx,
                    p_enb_ctx, X2AP_NULL);  
    }
    else
    {
        /*
         * Send failure response without time to wait
         */
        retVal = x2ap_build_endc_x2setup_fail_resp(p_x2ap_gl_ctx,
                                              p_enb_ctx,
                                              X2AP_FALSE,
                                              &error_indication,
                                              &send_err_ind);
    }

    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : x2ap_build_endc_x2setup_resp
 * Inputs         : num_act_cell  - number of activated cells
 *                  p_act_cell_id - sctivated cell id 
 *                  p_x2ap_gl_ctx - pointer to X2AP global context
 *                  p_enb_ctx     - Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS
 *                  X2AP_FAILURE
 * Description    : This function is called to build and send ENDC x2 setup success
 *                  response to peer gNB.
 ******************************************************************************/
x2ap_return_et
x2ap_build_endc_x2setup_resp
(
    U16                        num_act_cell,
    U16                        *p_act_cell_id,
    x2ap_gb_context_t          *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t    *p_enb_ctx
)
{
    /* TB_FIX_START */
    U32                             neigh_cell = X2AP_NULL;
    U32                             peer_cell = X2AP_NULL;
    x2ap_return_et                  retVal = X2AP_FAILURE;
    static en_dc_x2_setup_resp_t    x2_setup_resp;
    U16                             srvd_cell_count = X2AP_NULL;
    U16                             act_cell_count = X2AP_NULL;
    U8                              asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {X2AP_NULL};
    U16                             asn_msg_len = X2AP_NULL;
    x2ap_peer_enb_msg_t             peer_enb_msg;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_setup_resp,X2AP_NULL,sizeof(en_dc_x2_setup_resp_t));
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

    /*
     * Fill the global eNB Id
     */
    x2_setup_resp.bitmask |= X2AP_ENDC_X2_SETUP_RESP_EUTRA_CELL_INFO_PRESENT;
    x2_setup_resp.eutra_cell_info.global_enb_id = p_x2ap_gl_ctx->global_enb_id;

    /*
     * Fill Number of EUTRA served cell
     */
    x2_setup_resp.eutra_cell_info.num_served_cell = num_act_cell;

    /*
     * Fill served eutra cell Info
     */
    for ( srvd_cell_count = X2AP_NULL; srvd_cell_count < num_act_cell; srvd_cell_count++ )
    {
        x2_setup_resp.eutra_cell_info.served_cell_arr[srvd_cell_count].served_cell_info.pci =
           p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].pci;

        x2_setup_resp.eutra_cell_info.served_cell_arr[srvd_cell_count].served_cell_info.cell_id =
           p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].ecgi;

        x2_setup_resp.eutra_cell_info.served_cell_arr[srvd_cell_count].served_cell_info.broadcast_plmn_info =
           p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
              broadcast_plmn_info;

        x2_setup_resp.eutra_cell_info.served_cell_arr[srvd_cell_count].served_cell_info.choice_eutra_mode =
            p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].
                choice_eutra_mode;

        x2_setup_resp.eutra_cell_info.served_cell_arr[srvd_cell_count].served_cell_info.track_area_code =
            p_x2ap_gl_ctx->served_cell_info[p_act_cell_id[act_cell_count]].track_area_code;

        act_cell_count++;
    }

    for ( peer_cell = X2AP_NULL; peer_cell < p_x2ap_gl_ctx->
        num_peer_enb_cells; peer_cell++ )
    {
        if ( ( RRC_FALSE == p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].
            current_cell_info )
             && ( RRC_FALSE == p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].oam_configured )
           )
        {
            x2_setup_resp.eutra_cell_info.num_neigh_cell++;
            x2_setup_resp.eutra_cell_info.bitmask |=
                X2_SETUP_NR_NEIGHBOUR_INFO_PRESENT;

            x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                nr_neigh_info.nr_pci =
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].peer_cell_info.
                nr_pci;
 
            x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                nr_neigh_info.nr_cell_id =
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].peer_cell_info.
                nr_ecgi;
 
            x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                nr_neigh_info.nr_freq_info =
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].peer_cell_info.
                nr_freq_info; 
 
            x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                nr_neigh_info.nr_neigh_Mode_Info.bitmask =
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].peer_cell_info.
                choice_nr_mode.bitmask; 

            if ( X2AP_NR_MODE_FDD_PRESENT &
                x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                 nr_neigh_info.nr_neigh_Mode_Info.bitmask )
            {
               	X2AP_TRACE(X2AP_DETAILED,"nr mode fdd in x2 resp.[%d]\n",neigh_cell);
                x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                    nr_neigh_info.nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].peer_cell_info.
                    choice_nr_mode.nr_fdd_info.nr_ul_freq_info;
 
                x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                    nr_neigh_info.nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].peer_cell_info.
                    choice_nr_mode.nr_fdd_info.nr_dl_freq_info; 
            }
            else if ( X2AP_NR_MODE_TDD_PRESENT &
                x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                 nr_neigh_info.nr_neigh_Mode_Info.bitmask )
            {
                x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                    nr_neigh_info.nr_neigh_Mode_Info.tdd_info.nr_freq_info =
                    p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].peer_cell_info.
                    choice_nr_mode.nr_tdd_info.nr_freq_info; 
            }

            if ( X2AP_FIVE_GS_TAC_PRESENT &
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].peer_cell_info.
                bitmask )
            { 
               	X2AP_TRACE(X2AP_DETAILED,"5gs tac in x2 resp.\n");

                x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                    nr_neigh_info.bitmask |=
                    X2AP_FIVE_GS_TAC_PRESENT;

                X2AP_MEMCPY(&x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                    nr_neigh_info.fivegs_tac,
                    &p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].peer_cell_info.
                    five_gs_tac,sizeof(x2ap_five_gs_tac_t)); 
            }

            if ( X2AP_CONFIGURED_TAC_PRESENT &
                p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].peer_cell_info.
                bitmask )
            { 
               	X2AP_TRACE(X2AP_DETAILED,"config tac in x2 resp.\n");

                x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                    nr_neigh_info.bitmask |=
                    X2AP_CONFIGURED_TAC_PRESENT;

                X2AP_MEMCPY(&x2_setup_resp.eutra_cell_info.nr_neigh_cell_arr[neigh_cell].
                    nr_neigh_info.configured_tac,
                    &p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell].peer_cell_info.
                    configured_tac,sizeof(x2ap_tac_t)); 
            }
        }
    }

    /*
     * ASN Encoding
     */
    retVal = x2ap_encode_endc_x2setup_resp(&(x2_setup_resp),
                                     asn_msg_buff,
                                     &asn_msg_len );

    if ( X2AP_SUCCESS == retVal )
    {
        peer_enb_msg.p_msg = asn_msg_buff;
        peer_enb_msg.msg_len = asn_msg_len;
        peer_enb_msg.stream_id = p_enb_ctx->stream_info.
            stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
        /*
         * Sending the ENDC X2 SETUP RESPONSE to Peer gNB via SCTP
         */
        retVal = x2ap_sctp_sm_entry (X2AP_SCTP_SEND_DATA_EV,
                    (U8 *)(&peer_enb_msg),
                    p_x2ap_gl_ctx,
                    p_enb_ctx);
        if (X2AP_SUCCESS == retVal)
        {
            X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2 SETUP RESPONSE");
        }
    }

    /* TB_FIX_END */
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : x2ap_update_nr_cell_info_for_req
 * Inputs         : p_x2ap_gl_ctx    -  pointer to X2AP global context
 *                : p_enb_ctx        -  Pointer to eNB context
 *                : p_x2setup_req    -  Pointer of ENDC x2 setup request
 *                : num_act_cell     -  Number of active cell
 *                : p_act_cell_index -  Pointer to served cell index
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to update NR cell info in global db
 *                  for received request from peer gNB..
 ******************************************************************************/
void
x2ap_update_nr_cell_info_for_req
(
    en_dc_x2_setup_req_t    *p_x2setup_req,
    U16                     num_act_cell,
    U16                     *p_act_cell_index,
    x2ap_gb_context_t       *p_x2ap_gl_ctx
)
{
    /* TB_FIX_START */
    S32    peer_cell_count = X2AP_NULL;
    U16    peer_nr_cell_count = X2AP_NULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_x2setup_req != X2AP_P_NULL);
    X2AP_ASSERT(p_act_cell_index != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_TRACE(X2AP_DETAILED,"Servd cell count in endc x2 setup setup req is [%d]\n",
            p_x2setup_req->nr_cell_info.num_served_nr_cell);      

    /* modify for endc-x2setup,not judge nr_pci,TR196 not config nr cell currently */
    for ( peer_nr_cell_count = 0;
            peer_nr_cell_count < p_x2setup_req->nr_cell_info.num_served_nr_cell
            && peer_nr_cell_count < MAX_SERVED_CELLS;
            peer_nr_cell_count++)
    {
        peer_cell_count = x2ap_get_nbr_index_from_nr_cgi(p_x2ap_gl_ctx, 
                p_x2setup_req->nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].nr_cell_id, 
                X2AP_TRUE, 0);
                
        if (X2AP_NULL > peer_cell_count)
        {
            X2AP_TRACE(X2AP_ERROR,"nr_cgi error from endc x2setup response. \n");
            break;
        }
        
        p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].current_cell_info = RRC_TRUE; 
        
        /*
         * Update neighbour cell info
         */
        /*
         * Global gNB ID
         */
        p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count]
            .peer_gb_gnb_id = p_x2setup_req->nr_cell_info.global_gnb_id;

        /*
         * NR pci
         */
        p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count]
            .peer_cell_info.nr_pci = p_x2setup_req->nr_cell_info.served_nr_cell_arr
            [peer_nr_cell_count].nr_pci;/*p_x2setup_req->nr_cell_info.served_nr_cell_arr
            [peer_cell_count].nr_pci;*/

        /*
         * NR ecgi
         */
        p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count]
            .peer_cell_info.nr_ecgi = p_x2setup_req->nr_cell_info.served_nr_cell_arr
            [peer_nr_cell_count].nr_cell_id;
            /*p_x2setup_req->nr_cell_info.served_nr_cell_arr
            [peer_cell_count].nr_cell_id;*/

        /*
         * Brodcast PLMN
         */
        p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count]
            .peer_cell_info.broadcast_plmn_info = 
            p_x2setup_req->nr_cell_info.served_nr_cell_arr[peer_nr_cell_count]
            .broadcast_plmn_info;

        /* TB_FIX_START */
        /* NR Mode */
        X2AP_MEMCPY(&(p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count]
                    .peer_cell_info.choice_nr_mode),&(p_x2setup_req->
                        nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].
                        choice_nr_mode),sizeof(x2ap_choice_nr_mode_info_t));
        X2AP_TRACE(X2AP_DETAILED,"In endc x2setup req, choice mode bitmask is [%d] \n",
                p_x2setup_req->
                nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].choice_nr_mode.bitmask);
        if(p_x2setup_req->nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].
                choice_nr_mode.bitmask & X2AP_NR_MODE_FDD_PRESENT)
        {
            X2AP_MEMCPY(&(p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count]
                        .peer_cell_info.nr_freq_info),&(p_x2setup_req->
                            nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].
                            choice_nr_mode.nr_fdd_info.nr_ul_freq_info),sizeof(x2ap_nr_freq_info_t));
        }
        else if(p_x2setup_req->nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].
                choice_nr_mode.bitmask & X2AP_NR_MODE_TDD_PRESENT)
        {
            X2AP_MEMCPY(&(p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count]
                        .peer_cell_info.nr_freq_info),&(p_x2setup_req->
                            nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].
                            choice_nr_mode.nr_tdd_info.nr_freq_info),sizeof(x2ap_nr_freq_info_t));
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,"Invalid mode, bitmask set in setup req is [%d] \n", p_x2setup_req->nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].choice_nr_mode.bitmask);
        }
        /* TB_FIX_END */

        /* FiveGS TAC */
        if ( X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT &
                p_x2setup_req->nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].
                bitmask )
        {
            p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
                peer_cell_info.bitmask |=
                NR_CELL_INFO_FIVE_GS_TAC_PRESENT;

            X2AP_MEMCPY(&p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
                peer_cell_info.five_gs_tac,
                &p_x2setup_req->nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].
                fiveGS_track_area_code,sizeof(x2ap_five_gs_tac_t));
            X2AP_TRACE(X2AP_INFO,"5gs tac received from gNB");
        }

        /* Configured TAC */
        if ( X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT &
                p_x2setup_req->nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].
                bitmask )
        {
            p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
                peer_cell_info.bitmask |=
                NR_CELL_INFO_CONFIGURED_TAC_PRESENT;

            X2AP_MEMCPY(&p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].
                peer_cell_info.configured_tac, 
                &p_x2setup_req->nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].
                configured_tac, sizeof(x2ap_tac_t));
            X2AP_TRACE(X2AP_INFO,"Configured tac received from gNB");
        }

#ifndef LINUX_PC_TEST
        /* Meas timing config */
        x2ap_build_x2ap_meas_timing_config(&p_x2setup_req->nr_cell_info.served_nr_cell_arr[peer_nr_cell_count].meas_timing_config,
            &p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].peer_cell_info.x2ap_meas_timing_config[0],
            &p_x2ap_gl_ctx->peer_enb_cell_info[peer_cell_count].peer_cell_info.meas_timing_config_count);
#endif
    }
    X2AP_UT_TRACE_EXIT();
    /* TB_FIX_END */
}

/******************************************************************************
 * Function Name  : x2ap_served_nr_cell_info_update
 * Inputs         : p_x2ap_gl_ctx        - pointer to X2AP global context
 *                  status               - link status 
 *                  p_enb_ctx            - Pointer to eNB context
 *                  en_dc_x2_setup_req_t - pointer to the ENDC x2setup info
 * Outputs        : None
 * Returns        : None
 * Description    : This function is called to update the served cell info 
 *                  in the x2ap global context
 ******************************************************************************/
void x2ap_served_nr_cell_info_update
(
    x2ap_gb_context_t       *p_x2ap_gl_ctx,
    U8                      status,
    U8                      peer_enodeb_id,
    en_dc_x2_setup_req_t    *p_x2setup_req
)
{
    U8 peer_enb_ct = X2AP_NULL;
    U8 cell_ct     = X2AP_NULL;

    X2AP_ASSERT(PNULL!=p_x2ap_gl_ctx);
    X2AP_ASSERT(PNULL!=p_x2setup_req);

    X2AP_UT_TRACE_ENTER();

    for ( peer_enb_ct = X2AP_NULL; peer_enb_ct < MAX_PEER_ENB; peer_enb_ct++ )
    {
        if ( peer_enb_ct == peer_enodeb_id )
        {
            p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].presence_flag = RRC_TRUE;
            p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].connection_status = status;

            if ( X2AP_LINK_UP == p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].
                connection_status )
            {
                p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].num_served_cells = p_x2setup_req->
                    nr_cell_info.num_served_nr_cell;

                if ( X2AP_PEER_ENB_CONTEXT_GB_GNB_ID_PRESENT &
                    p_x2ap_gl_ctx->peer_enb_cntxt_arr[peer_enb_ct].bitmask )
                {
                    if ( ( PNULL != (p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].
                        p_nr_srvd_cell_info =
                                    x2ap_mem_get(sizeof(x2ap_nr_ecgi_t)*p_x2setup_req->
                        nr_cell_info.num_served_nr_cell)) ) &&
                            (p_x2setup_req->nr_cell_info.num_served_nr_cell <= MAX_SERVED_CELLS) )
                    {
                        for ( cell_ct = X2AP_NULL; cell_ct < p_x2ap_gl_ctx->
                                x2ap_link_info[peer_enb_ct].num_served_cells; cell_ct++ )
                        {
                            p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].presence_flag |=
                                X2AP_NR_SERVED_CELL_INFO_PRESENT;

                            X2AP_MEMCPY((void *)p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].
                                    p_nr_srvd_cell_info + (sizeof(x2ap_nr_ecgi_t) * cell_ct),
                                    (const void*)(&p_x2setup_req->nr_cell_info.served_nr_cell_arr[cell_ct].
                                        nr_cell_id),sizeof(x2ap_nr_ecgi_t));
                        }
                    }
                }
            }
            else
            {
                /*
                 * Free already existing NR served cell Info if any
                 */
                if ( X2AP_P_NULL != p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info )
                {
                    x2ap_mem_free(p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info);
                    p_x2ap_gl_ctx->x2ap_link_info[peer_enb_ct].p_nr_srvd_cell_info = X2AP_P_NULL;
                }
            }
            break;
        }
        if ( MAX_PEER_ENB == peer_enodeb_id )
        {
            X2AP_TRACE(X2AP_WARNING,"Peer EnodeB Id received is out of range : %d",
                peer_enodeb_id);
        }
    }
    X2AP_UT_TRACE_EXIT();
}

/******************************************************************************
 * Function Name  : x2ap_build_x2_link_up_ind_endc
 * Inputs         : p_x2_setup_resp   - pointer to en_dc_x2_setup_resp_t
 *                  p_rrm_link_up_ind - pointer to x2ap_rrm_x2_link_up_ind_st
 *                  num_act_cell      - num of activated cell
 *                  p_act_cell_id     - cell id
 *                  p_x2ap_gl_ctx     - pointer to X2AP global context
 * Outputs        : p_rrm_link_up_ind Message for rrm interface
 * Returns        : void 
 * Description    : This function is called to build link UP indication
 ******************************************************************************/
void
x2ap_build_x2_link_up_ind_endc
(
    en_dc_x2_setup_resp_t          *p_x2_setup_resp,
    x2ap_rrm_x2_link_up_ind_st     *p_rrm_link_up_ind
)
{
    U16                         ngh_srvd_cell_count = X2AP_NULL;
    U8                          index = X2AP_NULL;
    OSRTDListNode               *p_node = PNULL; 
    nr_rrc_MeasTimingList       *p_meas_timing_list = PNULL;
    nr_rrc_MeasTiming           *p_meas_timing_item = PNULL;
    nr_rrc_MeasurementTimingConfiguration *p_meas_timing_config = PNULL;

#ifdef LINUX_PC_TEST
    U32               meas_count = X2AP_NULL;
#endif

    X2AP_UT_TRACE_ENTER();

    p_rrm_link_up_ind->bitmask |= RRM_X2AP_NR_CELL_INFO_PRESENT;

    /*
     * Fill the global gNB Id
     */
    p_rrm_link_up_ind->nr_cell_info.global_gnb_id = p_x2_setup_resp->nr_cell_info.global_gnb_id;

    /*
     * Fill the Num served NR cells
     */
    p_rrm_link_up_ind->nr_cell_info.num_served_cell = p_x2_setup_resp->nr_cell_info.num_served_nr_cell;

    /*
     * peer served NR cell count
     */
    for ( ngh_srvd_cell_count = X2AP_NULL; ngh_srvd_cell_count <
            p_x2_setup_resp->nr_cell_info.num_served_nr_cell; ngh_srvd_cell_count++ )
    {
        p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count].bitmask
             = p_x2_setup_resp->nr_cell_info.
            served_nr_cell_arr[ngh_srvd_cell_count].bitmask;

        p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count].nr_pci
             = p_x2_setup_resp->nr_cell_info.
            served_nr_cell_arr[ngh_srvd_cell_count].nr_pci;

        p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count].nr_cell_id
             = p_x2_setup_resp->nr_cell_info.
            served_nr_cell_arr[ngh_srvd_cell_count].nr_cell_id;

        p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count].broadcast_plmn_info
             = p_x2_setup_resp->nr_cell_info.
            served_nr_cell_arr[ngh_srvd_cell_count].broadcast_plmn_info;

        p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count].choice_nr_mode
             = p_x2_setup_resp->nr_cell_info.
            served_nr_cell_arr[ngh_srvd_cell_count].choice_nr_mode;

        if ( X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT &
            p_x2_setup_resp->nr_cell_info.served_nr_cell_arr[ngh_srvd_cell_count].bitmask )
        {
            p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count].
                bitmask |= 
                X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT;

            X2AP_MEMCPY(&p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count].
                fiveGS_track_area_code,
                &p_x2_setup_resp->nr_cell_info.
                served_nr_cell_arr[ngh_srvd_cell_count].fiveGS_track_area_code,
                sizeof(x2ap_five_gs_tac_t));
        }

        if ( X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT &
            p_x2_setup_resp->nr_cell_info.served_nr_cell_arr[ngh_srvd_cell_count].bitmask )
        {
            p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count].
                bitmask |= 
                X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT;

            X2AP_MEMCPY(&p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count].
                configured_tac,
                &p_x2_setup_resp->nr_cell_info.
                served_nr_cell_arr[ngh_srvd_cell_count].configured_tac,sizeof(x2ap_tac_t));
        }

#ifdef LINUX_PC_TEST
        p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count].
            meas_timing_config_buf_len
            = p_x2_setup_resp->nr_cell_info.
            served_nr_cell_arr[ngh_srvd_cell_count].meas_timing_config_buf_len;

        for ( meas_count = X2AP_NULL; meas_count < p_x2_setup_resp->
            nr_cell_info.served_nr_cell_arr[ngh_srvd_cell_count].meas_timing_config_buf_len;
            meas_count++ )
        {
            p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count].
                measurement_timing_config_asn_buff[meas_count]
                = p_x2_setup_resp->nr_cell_info.
                served_nr_cell_arr[ngh_srvd_cell_count].measurement_timing_config_asn_buff[meas_count];
        }
#else
        /* Meas timing config */
        p_meas_timing_config = &(p_x2_setup_resp->nr_cell_info.
                served_nr_cell_arr[ngh_srvd_cell_count].meas_timing_config);

        if ( ( T_nr_rrc_MeasurementTimingConfiguration_criticalExtensions_c1 ==
                    p_meas_timing_config->criticalExtensions.t ) &&
                ( PNULL != p_meas_timing_config->criticalExtensions.u.c1 ) &&
                ( T_nr_rrc_MeasurementTimingConfiguration_criticalExtensions_c1_measTimingConf ==
                  p_meas_timing_config->criticalExtensions.u.c1->t ) &&
                ( PNULL !=  p_meas_timing_config->criticalExtensions.u.c1->u.measTimingConf )
           )
        {
            if ( p_meas_timing_config->criticalExtensions.u.c1->u.measTimingConf->m.
                    measTimingPresent )
            {
                p_meas_timing_list = &(p_meas_timing_config->criticalExtensions.u.c1->
                        u.measTimingConf->measTiming);
                p_node = p_meas_timing_list->head;
                for ( index = X2AP_NULL; index < p_meas_timing_list->count;
                        index++ )
                {
                    if ( !p_node )
                    {
                        X2AP_TRACE(X2AP_ERROR,
                                "%s: next node returned NULL",
                                __FUNCTION__);
                        break;
                    }

                    p_meas_timing_item =
                        (nr_rrc_MeasTiming*)(p_node->data);

                    if ( p_meas_timing_item->m.frequencyAndTimingPresent )
                    {
                        /*
                         * Fill nr_rrc_ARFCN_ValueNR 
                         */
                        p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                            x2ap_meas_timing_config[index].carrier_freq =
                            p_meas_timing_item->frequencyAndTiming.carrierFreq; 

                        /*
                         * Fill nr_rrc_SSB_MTC 
                         */
                        switch ( p_meas_timing_item->frequencyAndTiming.
                                ssb_MeasurementTimingConfiguration.periodicityAndOffset.t )
                        {
                            /*
                             * Fill periodicityAndOffset 
                             */
                            case T_nr_rrc_SSB_MTC_periodicityAndOffset_sf5:
                            {
                                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                                    x2ap_meas_timing_config[index].
                                    ssb_meas_timimg_configuration.
                                    x2ap_periodicity_and_offset.bitmask |=
                                    X2AP_PERIODICTY_OFFSET_SF5_PRESENT;

                                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                                    x2ap_meas_timing_config[index].
                                    ssb_meas_timimg_configuration.
                                    x2ap_periodicity_and_offset.sf5 =
                                    p_meas_timing_item->frequencyAndTiming.
                                    ssb_MeasurementTimingConfiguration.periodicityAndOffset.
                                    u.sf5; 
                                break;
                            }

                            case T_nr_rrc_SSB_MTC_periodicityAndOffset_sf10:
                            {
                                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                                    x2ap_meas_timing_config[index].
                                    ssb_meas_timimg_configuration.
                                    x2ap_periodicity_and_offset.bitmask |=
                                    X2AP_PERIODICTY_OFFSET_SF10_PRESENT;

                                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                                    x2ap_meas_timing_config[index].
                                    ssb_meas_timimg_configuration.
                                    x2ap_periodicity_and_offset.sf10 =
                                    p_meas_timing_item->frequencyAndTiming.
                                    ssb_MeasurementTimingConfiguration.periodicityAndOffset.
                                    u.sf10; 
                                break;
                            }

                            case T_nr_rrc_SSB_MTC_periodicityAndOffset_sf20:
                            {
                                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                                    x2ap_meas_timing_config[index].
                                    ssb_meas_timimg_configuration.
                                    x2ap_periodicity_and_offset.bitmask |=
                                    X2AP_PERIODICTY_OFFSET_SF20_PRESENT;

                                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                                    x2ap_meas_timing_config[index].
                                    ssb_meas_timimg_configuration.
                                    x2ap_periodicity_and_offset.sf20 =
                                    p_meas_timing_item->frequencyAndTiming.
                                    ssb_MeasurementTimingConfiguration.periodicityAndOffset.
                                    u.sf20; 
                                break;
                            }

                            case T_nr_rrc_SSB_MTC_periodicityAndOffset_sf40:
                            {
                                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                                    x2ap_meas_timing_config[index].
                                    ssb_meas_timimg_configuration.
                                    x2ap_periodicity_and_offset.bitmask |=
                                    X2AP_PERIODICTY_OFFSET_SF40_PRESENT;

                                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                                    x2ap_meas_timing_config[index].
                                    ssb_meas_timimg_configuration.
                                    x2ap_periodicity_and_offset.sf40 =
                                    p_meas_timing_item->frequencyAndTiming.
                                    ssb_MeasurementTimingConfiguration.periodicityAndOffset.
                                    u.sf40; 
                                break;
                            }

                            case T_nr_rrc_SSB_MTC_periodicityAndOffset_sf80:
                            {
                                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                                    x2ap_meas_timing_config[index].
                                    ssb_meas_timimg_configuration.
                                    x2ap_periodicity_and_offset.bitmask |=
                                    X2AP_PERIODICTY_OFFSET_SF80_PRESENT;

                                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                                    x2ap_meas_timing_config[index].
                                    ssb_meas_timimg_configuration.
                                    x2ap_periodicity_and_offset.sf80 =
                                    p_meas_timing_item->frequencyAndTiming.
                                    ssb_MeasurementTimingConfiguration.periodicityAndOffset.
                                    u.sf80; 
                                break;
                            }

                            case T_nr_rrc_SSB_MTC_periodicityAndOffset_sf160:
                            {
                                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                                    x2ap_meas_timing_config[index].
                                    ssb_meas_timimg_configuration.
                                    x2ap_periodicity_and_offset.bitmask |=
                                    X2AP_PERIODICTY_OFFSET_SF160_PRESENT;

                                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                                    x2ap_meas_timing_config[index].
                                    ssb_meas_timimg_configuration.
                                    x2ap_periodicity_and_offset.sf160 =
                                    p_meas_timing_item->frequencyAndTiming.
                                    ssb_MeasurementTimingConfiguration.periodicityAndOffset.
                                    u.sf160; 
                                break;
                            }

                            default:
                            {
                                X2AP_TRACE(X2AP_ERROR,"%s: Invalid SSB MTC periodicity offset",
                                        __FUNCTION__);
                                break;
                            }
                        }

                        /*
                         * Fill duration
                         */
                        p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                            x2ap_meas_timing_config[index].ssb_meas_timimg_configuration.
                            x2ap_mtc_duration =
                            p_meas_timing_item->frequencyAndTiming.
                            ssb_MeasurementTimingConfiguration.duration; 
                    }
                }
                p_rrm_link_up_ind->nr_cell_info.served_cell_info[ngh_srvd_cell_count]. 
                    meas_timing_config_count = p_meas_timing_list->count; 
            }
        }
        else
        {
            RRC_TRACE(RRC_ERROR,"Measurement timing config buffer is NULL");
        }
#endif
    }
    X2AP_UT_TRACE_EXIT();
}
/******************************************************************************
 * Function Name  : x2ap_send_link_up_ind_to_rrm_endc
 * Inputs         : x2ap_gb_context_t          *p_x2ap_gl_ctx,
 *                  p_x2_setup_resp - pointer to en_dc_x2_setup_resp_t
 *                  p_x2ap_gl_ctx   - pointer to  X2AP global context 
 *                  num_act_cell    - num of activated celll
 *                  p_act_cell_id   - activated cell id
 * Outputs        : None
 * Returns        : X2AP_SUCCESS
 *                  X2AP_FAILURE
 * Description    : This function is called when ENDC x2 setup link is UP to send
 *                  to rrm.
 ******************************************************************************/
x2ap_return_et
x2ap_send_link_up_ind_to_rrm_endc
(
    x2ap_gb_context_t     *p_x2ap_gl_ctx,
    en_dc_x2_setup_req_t  *p_x2_setup_req
)
{
    x2ap_return_et                         retVal = X2AP_SUCCESS;
    static x2ap_rrm_x2_link_up_ind_st      rrm_link_up_ind;

    X2AP_UT_TRACE_ENTER();
    
    X2AP_MEMSET(&rrm_link_up_ind,X2AP_NULL,sizeof(x2ap_rrm_x2_link_up_ind_st));

    /*
     * Send X2 link down to RRM
     */
    x2ap_build_nr_x2_link_up_ind(p_x2ap_gl_ctx,p_x2_setup_req,
                              &rrm_link_up_ind);

    if ( RRC_FAILURE == 
            rrc_x2apRrm_il_send_x2ap_rrm_x2_link_up_ind_st((&rrm_link_up_ind),
            RRC_X2AP_MODULE_ID,RRC_RRM_MODULE_ID, RRC_NULL, RRC_NULL) )
    {
        return X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * Function Name  : endc_x2_setup_active_x2_setup_req_evt_hdl 
 * Inputs         : p_msg_ptr     - Pointer to message buffer
 *                : p_x2ap_gl_ctx - Pointer to X2AP global context
 *                : p_enb_ctx     - Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS = 0
 *                  X2AP_FAILURE = -1
 * Description    : This function is called when EN DC x2 setup request event is 
 *                  received from peer gNB.
 ******************************************************************************/
x2ap_return_et
endc_x2_setup_active_x2_setup_req_evt_hdl
(
    U8                         *p_msg_ptr,
    x2ap_gb_context_t  	       *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t    *p_enb_ctx
)
{
    x2ap_return_et                    retVal = X2AP_FAILURE;
    U16                               num_act_cell = RRC_NULL;
    U16                               act_cell_id[MAX_NUM_CELL_IN_X2AP]={RRC_NULL};
    static en_dc_x2_setup_req_t       x2_setup_req;
    x2ap_peer_enb_msg_t               *p_peer_enb_msg = X2AP_P_NULL;
    rrc_x2ap_error_indication_t       error_indication;
    x2ap_error_ind_bool_t             send_err_ind;
	OSCTXT                            asn1_ctx;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_peer_enb_msg->p_msg != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gl_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_setup_req, X2AP_NULL, sizeof(en_dc_x2_setup_req_t));
    X2AP_MEMSET(&error_indication, X2AP_NULL,
                    sizeof(rrc_x2ap_error_indication_t));
    X2AP_MEMSET(&send_err_ind,X2AP_NULL,
                    sizeof(x2ap_error_ind_bool_t));

    X2AP_TRACE(X2AP_INFO,"X2AP_MSG: ENDC X2 SETUP REQUEST Received");

    p_peer_enb_msg = (x2ap_peer_enb_msg_t *)p_msg_ptr;

    /*
     * Check if there is any cell is operational or not
     */
    x2ap_check_active_cell(&num_act_cell,
                           act_cell_id,
                           p_x2ap_gl_ctx);

    /*
     * Decode the ASN request
     */
    retVal = x2ap_decode_endc_x2setup_req(
                p_x2ap_gl_ctx,
                p_enb_ctx,
                &x2_setup_req,
                (p_peer_enb_msg)->p_msg,
                &(p_peer_enb_msg->msg_len),
                &error_indication,
                &send_err_ind,
                &asn1_ctx);

    if ( X2AP_SUCCESS == retVal )
    { 
        X2AP_TRACE(X2AP_DETAILED,"X2 LINK is already up ");

        /*
         * Build and send EN DC x2 setup resp to peer gnb
         */
         X2AP_TRACE(X2AP_INFO,"X2AP_MSG: ENDC X2 SETUP RESPONSE");
        retVal = x2ap_build_endc_x2setup_resp(num_act_cell,
                 act_cell_id,
                 p_x2ap_gl_ctx,
                 p_enb_ctx); 
    }
    else
    {
        /*
         * Send failure response without time to wait
         */
        retVal = x2ap_build_endc_x2setup_fail_resp(p_x2ap_gl_ctx,
                                              p_enb_ctx,
                                              X2AP_FALSE,
                                              &error_indication,
                                              &send_err_ind);
    }

    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/* MENB CHANGES - END */
#endif
