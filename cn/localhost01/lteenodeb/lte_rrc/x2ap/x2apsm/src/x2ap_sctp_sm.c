/*******************************************************************************
 *
 *  FILE NAME   : x2ap_sctp_fsm.c
 *
 *  DESCRIPTION : This file contains the sctp state machine and the state
 *                machine Entry function definition. 
 *
 *  DATE 24-June-2010  NAME Sudhanshu Kumar Srivastav
 *                     REFERENCE  36.423_880(3GPP Spec) REASON
 *
 *  Copyright 2007, Aricent Inc.
 *
 *******************************************************************************/

#include "x2ap_evthdlr.h"
#include "x2ap_sctp_sm.h"
#include "x2ap_tracing.h"
#include "x2ap_timer.h"
#include "x2ap_utils.h"
#include "sctp_ipr.h"
#include "x2ap_api.h"
#include "asn_dec_wrappers.h"
#include "x2ap_md.h"
#include "x2ap_ue_asso_sig_sm.h"
#include "sm_entry.h"
#include "sctp_kernel.h"
#include "x2ap_rsu_sm.h"
#include "rrc_logging.h"

#include "x2ap_error_indication.h"
#include "x2ap_cell_activation.h"
#if 0
#ifdef RRC_UNIT_TEST_FRAMEWORK
#include "rrc_ut_stub_db.h"
#endif
#endif

#define X2AP_MAX_SCTP_CONN_RETRY_COUNT 1


X2AP_SCTP_SM_EVENT(x2ap_sctp_ignore_msg);
X2AP_SCTP_SM_EVENT(x2ap_sctp_idle_conn);
X2AP_SCTP_SM_EVENT(x2ap_sctp_wait_conn_comm_up);
X2AP_SCTP_SM_EVENT(x2ap_sctp_wait_conn_lost);
X2AP_SCTP_SM_EVENT(x2ap_sctp_wait_conn_close);
X2AP_SCTP_SM_EVENT(x2ap_sctp_wait_conn_timr_exp);
X2AP_SCTP_SM_EVENT(x2ap_sctp_act_conn_lost);
X2AP_SCTP_SM_EVENT(x2ap_sctp_act_peer_rstrt);
X2AP_SCTP_SM_EVENT(x2ap_sctp_act_shutdown);
X2AP_SCTP_SM_EVENT(x2ap_sctp_act_conn_close);
X2AP_SCTP_SM_EVENT(x2ap_sctp_act_snd_data);
X2AP_SCTP_SM_EVENT(x2ap_sctp_rcv_data);
X2AP_SCTP_SM_EVENT(x2ap_sctp_rel_ongo_conn_lost);
X2AP_SCTP_SM_EVENT(x2ap_sctp_rel_ongo_conn_clsd);
X2AP_SCTP_SM_EVENT(x2ap_sctp_idle_oam_add_enb);
X2AP_SCTP_SM_EVENT(x2ap_sctp_idle_pend_conn);
X2AP_SCTP_SM_EVENT(x2ap_sctp_act_server_conn_close);
X2AP_SCTP_SM_EVENT(x2ap_sctp_conn_up_and_recv_sctp_msg);
    /*SPR_16533_START*/
        /* Code Removed */
    /*SPR_16533_END*/


/*****************************************************************************
   * DATA
   *      DESCRIPTION: This is used for the logging of X2AP Sctp States.
   ****************************************************************************/
U8 *X2AP_SCTP_FSM_STATES_NAMES[] =
{
   (U8 *)"X2AP_SCTP_IDLE_ST",  
   (U8 *)"X2AP_SCTP_W_COMM_UP",
   (U8 *)"X2AP_SCTP_ESTABLISHED_ST",  
   (U8 *)"X2AP_SCTP_RELEASE_ONGOING_ST",
   (U8 *)"X2AP_SCTP_MAX_ST"
};

/******************************************************************************
   * DATA DESCRIPTION : This is used for the logging of X2AP Sctp Events.
   ******************************************************************************/
U8 *X2AP_SCTP_FSM_EVENTS_NAMES[] =
{
   (U8 *)"X2AP_SCTP_CONNECT_EV",
   (U8 *)"X2AP_SCTP_COMM_UP_EV",
   (U8 *)"X2AP_SCTP_CONN_LOST_EV",
   (U8 *)"X2AP_SCTP_CONN_RSTRT_EV",
   (U8 *)"X2AP_SCTP_CLOSED_EV",
   (U8 *)"X2AP_SCTP_SHUT_DOWN_EV",
   (U8 *)"X2AP_SCTP_CONN_CLOSE_EV",
   (U8 *)"X2AP_SCTP_SEND_DATA_EV",
   (U8 *)"X2AP_SCTP_RCVD_DATA_EV",
   (U8 *)"X2AP_SCTP_CONNECT_TMR_EXP_EV",
   (U8 *)"X2AP_SCTP_PENDING_CONN_EV",
   (U8 *)"X2AP_SCTP_CLOSE_ALL_X2AP_SCTP_CONN_EV",
   (U8 *)"X2AP_SCTP_MAX_EV"
};

/*****************************************************************************
   * DATA
   *      DESCRIPTION: This is used for the logging of X2AP UE Associated 
   *                   Signalling States.
   ****************************************************************************/
U8 *X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[] =
{
     (U8 *)"X2AP_UE_ASSO_SIG_IDLE_ST",
     (U8 *)"X2AP_UE_ASSO_SIG_ACTIVE_ST",
     (U8 *)"X2AP_UE_ASSO_SIG_MAX_STATE"
};

/******************************************************************************
 * DATA DESCRIPTION : This enum is used for the logging of X2AP UE Associated 
 *                    Signalling Events.
 ******************************************************************************/
U8 *X2AP_UE_ASSO_SIG_FSM_EVENTS_NAMES[] =
{
    (U8 *)"X2AP_UE_ASSO_SIG_X2_LINK_UP_EV",
    (U8 *)"X2AP_UE_ASSO_SIG_MSG_IND_EV",
    (U8 *)"X2AP_UE_ASSO_SIG_MSG_REQ_EV",
    (U8 *)"X2AP_UE_ASSO_SIG_X2_LINK_DOWN_EV",
    (U8 *)"X2AP_UE_ASSO_SIG_MAX_EVENT"
};


/* GDB Fix - compiler warning introduce in 31 Dec release, don't Aricent look at these? */
extern rrc_return_et rrc_x2apCommon_send_x2ap_enb_link_down_ind(    x2ap_enb_link_down_ind_t  *p_x2ap_enb_link_down_ind,
                                                                    U16                 src_module_id,
                                                                    U16                 dest_module_id,
                                                                    U16                 transaction_id,
                                                                    U8                  cell_index
                                                                );

static  x2ap_return_et x2ap_open_client_asso_start_timer( 
        x2ap_peer_enb_context_t *p_enb_cntxt_arr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);


void x2ap_popu_sctp_conf_info_from_gb_db(
        x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info,
        x2ap_sctp_conf_info_t      *p_x2ap_sctp_conf_info);

static  x2ap_return_et x2ap_start_sctp_timer(
        x2ap_timer_id_en   timer_type,
        U32                timer_value,
        x2ap_timer_t      *p_timer_id,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr);

static  x2ap_return_et  x2ap_sctp_sm_map_event(
        U16 api,
        x2ap_sctp_event_et *p_event);

/* klocwork Fix*/
static  x2ap_return_et  x2ap_sctp_validate_event(
        U16 api,
        x2ap_sctp_event_et *p_event);
/* klocwork Fix*/
extern x2ap_return_et x2ap_process_load_information_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

extern x2ap_return_et x2ap_process_radio_link_failure_indication(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

extern x2ap_return_et x2ap_process_reset_request_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

extern x2ap_return_et x2ap_process_reset_response_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

extern x2ap_return_et x2ap_process_enb_config_update_enb_req(
        U8* p_msg_ptr,
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t* p_enb_cntx );

extern x2ap_return_et x2ap_process_enb_config_update_ack(
		U8* p_msg_ptr,
	    x2ap_gb_context_t* p_x2ap_gb_ctx,
	    x2ap_peer_enb_context_t* p_enb_cntx);

extern x2ap_return_et x2ap_process_enb_config_update_failure(
		U8* p_msg_ptr,
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t* p_enb_cntx);

extern x2ap_return_et x2ap_process_handover_report(
		U8* p_msg_ptr,
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t* p_enb_cntx);

extern x2ap_return_et x2ap_process_mobility_change_request_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

extern x2ap_return_et x2ap_process_mobility_change_response_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

extern x2ap_return_et x2ap_process_mobility_change_failure_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

extern x2ap_return_et x2ap_process_endc_config_update_en_gnb_req(
         U8* p_msg_ptr,
         x2ap_gb_context_t* p_x2ap_gb_ctx,
         x2ap_peer_enb_context_t* p_enb_cntx);    
/*SPR_16533_START*/
#ifdef LKSCTP
typedef struct 
{
    U32 assoc_id;
    S32 socket;
}g_stored_assoc_info_t;

extern g_stored_assoc_info_t g_stored_assoc_info[MAX_PEER_ENB];
#endif
/*SPR_16533_END*/

#ifdef LINUX_PC_TEST
//for ARICENT_SCTP_IPR
extern S32 pctest_forward_mme_message_to_pc
(
    U16     src_module_id,  /* Source module Id */
    U16     dst_module_id,  /* Destination module Id */
    U16     api_id,         /* API Id /msg id*/
    U32 socketId,
    U32 streamId,
    const void * pmsg,
    U32 msglen
);

extern S32 x2ap_get_sock_fd_for_test(x2ap_peer_enb_context_t *p_enb_ctx);
#endif

static  x2ap_sctp_sm_action_routine x2ap_sctp_fsm[X2AP_SCTP_MAX_ST]
[X2AP_SCTP_MAX_EV]=
{
    /* X2AP_SCTP_IDLE */
    {
        x2ap_sctp_idle_conn,  /* X2AP_SCTP_CONNECT_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_COMM_UP_EV */ 
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CONN_LOST_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CONN_RSTRT_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CLOSED_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_SHUT_DOWN_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CONN_CLOSE_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_SEND_DATA_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_RCVD_DATA_EV*/
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CONNECT_TMR_EXP_EV */
        x2ap_sctp_idle_pend_conn, /* X2AP_SCTP_PENDING_CONN_EV */
        x2ap_sctp_act_server_conn_close /* X2AP_SCTP_CLOSE_ALL_X2AP_SCTP_CONN_EV */
    },
    /*SPR_16533_START*/
    /* Code Removed */
    /*SPR_16533_END*/
    /* X2AP_SCTP_W_CONNECT */
    {
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CONNECT_EV */
        x2ap_sctp_wait_conn_comm_up, /* X2AP_SCTP_COMM_UP_EV */ 
        x2ap_sctp_wait_conn_lost, /* X2AP_SCTP_CONN_LOST_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CONN_RSTRT_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CLOSED_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_SHUT_DOWN_EV */
        x2ap_sctp_wait_conn_close, /* X2AP_SCTP_CONN_CLOSE_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_SEND_DATA_EV */
        //x2ap_sctp_ignore_msg, /* X2AP_SCTP_RCVD_DATA_EV*/
        x2ap_sctp_conn_up_and_recv_sctp_msg, /* X2AP_SCTP_RCVD_DATA_EV*/
        x2ap_sctp_wait_conn_timr_exp, /* X2AP_SCTP_CONNECT_TMR_EXP_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_PENDING_CONN_EV */
        x2ap_sctp_ignore_msg /* X2AP_SCTP_CLOSE_ALL_X2AP_SCTP_CONN_EV */
    },
    /* X2AP_SCTP_ACTIVE */
    {
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CONNECT_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_COMM_UP_EV */ 
        x2ap_sctp_act_conn_lost,/* X2AP_SCTP_CONN_LOST_EV */
        x2ap_sctp_act_peer_rstrt, /* X2AP_SCTP_CONN_RSTRT_EV */
        x2ap_sctp_ignore_msg,     /* X2AP_SCTP_CLOSED_EV */
        x2ap_sctp_act_shutdown, /* X2AP_SCTP_SHUT_DOWN_EV */
        x2ap_sctp_act_conn_close, /* X2AP_SCTP_CONN_CLOSE_EV */
        x2ap_sctp_act_snd_data, /* X2AP_SCTP_SEND_DATA_EV */
        x2ap_sctp_rcv_data, /* X2AP_SCTP_RCVD_DATA_EV*/
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CONNECT_TMR_EXP_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_PENDING_CONN_EV */
        x2ap_sctp_act_server_conn_close /* X2AP_SCTP_CLOSE_ALL_X2AP_SCTP_CONN_EV */
    },
    /* X2AP_SCTP_RELEASE_ONGOING_ST */
    {
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CONNECT_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_COMM_UP_EV */ 
        x2ap_sctp_rel_ongo_conn_lost, /* X2AP_SCTP_CONN_LOST_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CONN_RSTRT_EV */
        x2ap_sctp_rel_ongo_conn_clsd, /* X2AP_SCTP_CLOSED_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_SHUT_DOWN_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CONN_CLOSE_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_SEND_DATA_EV */
        x2ap_sctp_rcv_data,/* X2AP_SCTP_RCVD_DATA_EV*/
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_CONNECT_TMR_EXP_EV */
        x2ap_sctp_ignore_msg, /* X2AP_SCTP_PENDING_CONN_EV */
        x2ap_sctp_ignore_msg /* X2AP_SCTP_CLOSE_ALL_X2AP_SCTP_CONN_EV */
    }
};
/*SPR_16533_START*/
#ifdef LKSCTP
/*****************************************************************************
 * Function Name  : x2ap_delete_assoc_id
 * Inputs         : sctp_sd: socket_descriptor
 * Outputs        : None
 * Returns        : None 
 * Description    : This function deletes the assoc_id corresponding to the 
 *                  socket descriptor provided and packs the g_stored_assoc_info
 *                  structure.
 *****************************************************************************/
void x2ap_delete_assoc_id(S32 sctp_sd)
{
    U8 counter_1 = 0;
    U8 counter_2 = 0;

    for (counter_1 = 0; counter_1 < MAX_PEER_ENB; counter_1++)
    {   
        if(g_stored_assoc_info[counter_1].socket == sctp_sd)
        {
            X2AP_TRACE(X2AP_DETAILED, "assoc_id = %d will be removed",
                    g_stored_assoc_info[counter_1].assoc_id);
            g_stored_assoc_info[counter_1].assoc_id = 0;
            g_stored_assoc_info[counter_1].socket = INVALID_SCTP_SOCKET;
            break;  
        }
    }   
    for (counter_2 = counter_1; counter_2 < (MAX_PEER_ENB - 1) ; counter_2++)
    {   
        /* Shift all present ASSOC. ID to thier left array index */
        if(g_stored_assoc_info[counter_2 + 1].socket != INVALID_SCTP_SOCKET)
        {
            g_stored_assoc_info[counter_2].socket  
                = g_stored_assoc_info[counter_2 + 1].socket; 
            g_stored_assoc_info[counter_2].assoc_id 
                = g_stored_assoc_info[counter_2 + 1].assoc_id; 
        }
    }    
}
#endif
/*SPR_16533_END*/

/*****************************************************************************
 * Function Name  : x2ap_sctp_conn_up_and_recv_sctp_msg
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *                  X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function brings Sctp connection to established state
 *                  and processes X2 Setup request received from Peer eNB.
 *                  
 *****************************************************************************/
x2ap_return_et x2ap_sctp_conn_up_and_recv_sctp_msg(
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr)
{
    x2ap_return_et ret_val = X2AP_SUCCESS;

    /*SPR_16533_START*/
    /* Code Removed */
    /*SPR_16533_END*/

/*SPR_16533_DELTA3_START*/
    /* Change the SCTP state to Active as 
        Data must has been recieved on an active association*/
    p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_ESTABLISHED_ST; 

    /* First Process the data recieved */
    if (X2AP_FAILURE == x2ap_sctp_rcv_data(p_msg_ptr,
                p_x2ap_gb_ctx, p_enb_cntxt_arr))
    {
        X2AP_TRACE(
                X2AP_INFO,
                "p_sctp_rcv_data failedp_sctp_rcv_data failed");
        return X2AP_FAILURE;
    }
    /* Check if X2 state is still is Idle */
    if(p_enb_cntxt_arr->x2_setup_sm_ctx.x2Setup_curr_state == X2_SETUP_IDLE_ST)
    {
        /* Invoke SM to send X2_Setup_Req */
        if (X2AP_FAILURE == x2ap_sctp_wait_conn_comm_up(p_msg_ptr,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr))
        {
            X2AP_TRACE(
                    X2AP_INFO,
                    "x2ap_sctp_wait_conn_comm_up failed");
            return X2AP_FAILURE;
        }
    }
/*SPR_16533_DELTA3_END*/
    return ret_val;
}


/*****************************************************************************
 * Function Name  : x2ap_sctp_ignore_msg
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr : pointer to x2ap_peer_enb_context_t
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function will ignore invalid event received in any 
 *                  state.
 *****************************************************************************/
x2ap_return_et x2ap_sctp_ignore_msg( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr)
{
    /* ARGUSED */
    X2AP_TRACE(X2AP_DETAILED, "Received Invalid SM ,"
            "Unused variable in fsm p_msg_ptr:%p,p_x2ap_gb_ctx:%p,p_enb_cntxt_arr:%p",
            p_msg_ptr,p_x2ap_gb_ctx,p_enb_cntxt_arr);

    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_sctp_sm_entry
 * Inputs         : api:  This Api will be mapped to SM Event
 *                  p_msg_ptr - pointer to msg
 *                  p_x2ap_gb_ctx: pointer to X2AP global context
 *                  p_enb_cntxt_arr: ENB Context Specific to SCTP SM 
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function invoke SCTP Event Handler
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_sm_entry(
        U16                api,
        U8                 *p_msg_ptr, 
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
    x2ap_return_et ret_val = X2AP_SUCCESS;        
    x2ap_sctp_event_et event = X2AP_SCTP_MAX_EV;
    
    /* klocwork Fix*/
    if(api < X2AP_SCTP_MAX_EV)
    {
       ret_val = x2ap_sctp_validate_event(api, &event) ;
    }
    else if ( api > X2AP_EXTERNAL_API_BASE && api <= X2AP_SCTP_SHUT_DOWN_INIT_IND  )
    {
       ret_val =  x2ap_sctp_sm_map_event(api, &event); 
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR, "Invalid API or event %d",api);
        ret_val = X2AP_FAILURE ;   
    }
    /* klocwork Fix*/
    /*SPR_18216_START*/
    if(p_enb_cntxt_arr != PNULL)
    {
        if ( (X2AP_SUCCESS == ret_val)
                && (event < X2AP_SCTP_MAX_EV))
        {
#if 0        
#ifdef RRC_UNIT_TEST_FRAMEWORK
            if(p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state == 0) {
                p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_ESTABLISHED_ST;

                p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.
                    x2ap_ue_asso_sig_curr_state = X2AP_UE_ASSO_SIG_ACTIVE_ST;

            }
#endif
#endif
            X2AP_TRACE(X2AP_DETAILED, "### x2ap_sctp_sm Processing State:%d Event:%d ###",
                    p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state, event);

            x2ap_sctp_fsm[p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state]
                [event](p_msg_ptr, p_x2ap_gb_ctx, p_enb_cntxt_arr);

            X2AP_TRACE(X2AP_DETAILED, "### x2ap_sctp_sm: State:%d, Handled Event:%d ###",
                    p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state, event);

        }
        else
        {
            ret_val = X2AP_FAILURE;
        }
    }
    else
    {
        X2AP_TRACE(X2AP_INFO, "peer_enb_context does not exist");
        ret_val = X2AP_FAILURE;
    }
    /*SPR_18216_END*/
    return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_sctp_idle_conn
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function connect as client to peer eNB.
 *****************************************************************************/

x2ap_return_et  x2ap_sctp_idle_conn( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
    x2ap_return_et ret_val = X2AP_FAILURE;
    LOCAL_SCTP_SETUP_ATTEMPT_STRUCT *p_local_sctp_setup_att = PNULL;
    /* Open Client Association and start connect Timer */
    ret_val = x2ap_open_client_asso_start_timer(p_enb_cntxt_arr, p_x2ap_gb_ctx);
    X2AP_TRACE(X2AP_DETAILEDALL,"Unused variable in fsm :%p",p_msg_ptr);

    if (X2AP_SUCCESS == ret_val)
    {
        /*SPR_16533_START*/
        /* Code Removed */
        /*SPR_16533_END*/
        /* Change SM State to Wait Connect State */
        p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_W_COMM_UP;

        /*Generating Protocl Event LOCAL_SCTP_SETUP_ATTEMPT*/ 
        p_local_sctp_setup_att = rrc_mem_get(sizeof(LOCAL_SCTP_SETUP_ATTEMPT_STRUCT));
        if (PNULL != p_local_sctp_setup_att)
        {
            memset_wrapper(p_local_sctp_setup_att, RRC_NULL, sizeof(LOCAL_SCTP_SETUP_ATTEMPT_STRUCT));
            p_local_sctp_setup_att->header.length = sizeof(LOCAL_SCTP_SETUP_ATTEMPT_STRUCT);
            p_local_sctp_setup_att->header.event_id = LOCAL_SCTP_SETUP_ATTEMPT;

            l3_memcpy_wrapper(p_local_sctp_setup_att->EVENT_PARAM_IP_ADDRESS ,p_x2ap_gb_ctx->
                    local_enb_info.ip_addr[0].ip_addr, MAX_X2AP_IP_ADDRESS_LENGTH);

            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_sctp_setup_att);
        }
    }
    /*SPR_16533_START*/
#ifdef LKSCTP
    if (X2AP_FAILURE == x2ap_sctp_wait_conn_comm_up(p_msg_ptr,
                               p_x2ap_gb_ctx, p_enb_cntxt_arr))
    {
        X2AP_TRACE(
                X2AP_INFO,
                 "x2ap_sctp_wait_conn_comm_up failed");
        ret_val = X2AP_FAILURE;
    }
#endif
    /*SPR_16533_END*/
  
    return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_sctp_idle_oam_add_enb
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function open client association for new added ENB 
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_idle_oam_add_enb( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
    x2ap_return_et ret_val = X2AP_FAILURE;

    /* Open Client Association and start connect Timer */
    ret_val = x2ap_open_client_asso_start_timer(p_enb_cntxt_arr, p_x2ap_gb_ctx);
    X2AP_TRACE(X2AP_DETAILED,"Unused variable in fsm :%p",p_msg_ptr);

    if (X2AP_SUCCESS == ret_val)
    {
        /* Change SM State to Wait Connect State */
        p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_W_COMM_UP;
    }

    return ret_val; 
}
/*****************************************************************************
 * Function Name  : x2ap_sctp_idle_pend_conn
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function change SM state to wait for communication 
 *                  up state in case client connection is pending.
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_idle_pend_conn( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
    LOCAL_SCTP_SETUP_RESULT_STRUCT  *p_local_sctp_setup_result = PNULL;
    p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_W_COMM_UP;

    /*Generating Protocl Event LOCAL_SCTP_SETUP_RESULT*/
    p_local_sctp_setup_result = rrc_mem_get(sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));
    if (PNULL != p_local_sctp_setup_result)
    {
        memset_wrapper(p_local_sctp_setup_result, RRC_NULL, 
                sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));

        p_local_sctp_setup_result->header.length = sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT);
        p_local_sctp_setup_result->header.event_id = LOCAL_SCTP_SETUP_RESULT;

        p_local_sctp_setup_result->EVENT_PARAM_SCTP_SETUP_RESULT = EVENT_VALUE_SUCCESS;
        p_local_sctp_setup_result->EVENT_PARAM_SCTP_ENDPOINT_TYPE = EVENT_VALUE_EXTERNAL_ENB;
        l3_memcpy_wrapper(p_local_sctp_setup_result->EVENT_PARAM_IP_ADDRESS ,p_x2ap_gb_ctx->
            local_enb_info.ip_addr[0].ip_addr, MAX_X2AP_IP_ADDRESS_LENGTH);
        p_local_sctp_setup_result->EVENT_PARAM_SCTP_CAUSE = EVENT_VALUE_REMOTE;

        rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_sctp_setup_result);
    }
    
    X2AP_TRACE(X2AP_DETAILED,"Unused variable in fsm p_msg_ptr:%p,p_x2ap_gb_ctx:%p",
            p_msg_ptr,p_x2ap_gb_ctx);
    return X2AP_SUCCESS;
}
/*****************************************************************************
 * Function Name  : x2ap_sctp_wait_conn_comm_up
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function invoke X2 Setup SM and change SM state to 
 *                  established state.
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_wait_conn_comm_up( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
    x2ap_return_et ret_val = X2AP_FAILURE;
    LOCAL_SCTP_SETUP_RESULT_STRUCT  *p_local_sctp_setup_result = PNULL;

    /*Generating Protocl Event LOCAL_SCTP_SETUP_RESULT*/
    p_local_sctp_setup_result = rrc_mem_get(sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));
    if (PNULL != p_local_sctp_setup_result)
    {
        memset_wrapper(p_local_sctp_setup_result, RRC_NULL, 
                sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));

        p_local_sctp_setup_result->header.length = sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT);
        p_local_sctp_setup_result->header.event_id = LOCAL_SCTP_SETUP_RESULT;

        p_local_sctp_setup_result->EVENT_PARAM_SCTP_SETUP_RESULT = EVENT_VALUE_SUCCESS;
        p_local_sctp_setup_result->EVENT_PARAM_SCTP_ENDPOINT_TYPE = EVENT_VALUE_EXTERNAL_ENB;
        l3_memcpy_wrapper(p_local_sctp_setup_result->EVENT_PARAM_IP_ADDRESS, p_x2ap_gb_ctx->
            local_enb_info.ip_addr[0].ip_addr, MAX_X2AP_IP_ADDRESS_LENGTH);
        p_local_sctp_setup_result->EVENT_PARAM_SCTP_CAUSE = EVENT_VALUE_REMOTE;

        rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_sctp_setup_result);
    }
    /*SPR_16533_START*/
#if 0	
#ifdef ARICENT_SCTP_IPR
    /* Stop connect Timer */
    if (X2AP_P_NULL != p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id)
    {
        x2ap_stop_timer(p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id);
        p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id = X2AP_P_NULL;
    };
#endif
#endif
    /*SPR_16533_END*/
    p_enb_cntxt_arr->sctp_sm_ctx.connect_ret_count = 0;
    p_enb_cntxt_arr->op_status = ASSOCIATED;

    /* Change SM State to Wait Connect State */
    /*SPR_16533_DELTA2_START*/
#if 0	
#ifdef ARICENT_SCTP_IPR
    p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_ESTABLISHED_ST;
#endif
#endif
#ifdef LKSCTP
    /* SCTP State will be changed after recieving COMM_UP_IND */
#endif
    /*SPR_16533_DELTA2_END*/
    
    /* Invoke X2_SETUP SM with Event X2_SETUP_SCTP_X2_CONN_UP_EV */
    ret_val = x2setup_sm_entry(X2_SETUP_SCTP_X2_CONN_UP_EV, p_msg_ptr,
            p_x2ap_gb_ctx, p_enb_cntxt_arr);
    return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_sctp_wait_conn_lost
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context 
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function shall Stop wait for connect timer
 *                  and remove sctp sd from DB.
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_wait_conn_lost( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
    x2ap_return_et              ret_val = X2AP_FAILURE;
    x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
    LOCAL_SCTP_SETUP_RESULT_STRUCT  *p_local_sctp_setup_result = PNULL;
    X2AP_TRACE(X2AP_DETAILED,"Unused variable in fsm :%p",p_msg_ptr);

    /*Generating Protocl Event LOCAL_SCTP_SETUP_RESULT*/
    p_local_sctp_setup_result = rrc_mem_get(sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));
    if (PNULL != p_local_sctp_setup_result)
    {
        memset_wrapper(p_local_sctp_setup_result, RRC_NULL, 
                sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));

        p_local_sctp_setup_result->header.length = sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT);
        p_local_sctp_setup_result->header.event_id = LOCAL_SCTP_SETUP_RESULT;

        p_local_sctp_setup_result->EVENT_PARAM_SCTP_SETUP_RESULT = EVENT_VALUE_FAILURE;
        p_local_sctp_setup_result->EVENT_PARAM_SCTP_ENDPOINT_TYPE = EVENT_VALUE_EXTERNAL_ENB;
        l3_memcpy_wrapper(p_local_sctp_setup_result->EVENT_PARAM_IP_ADDRESS ,p_x2ap_gb_ctx->
            local_enb_info.ip_addr[0].ip_addr, MAX_X2AP_IP_ADDRESS_LENGTH);
        p_local_sctp_setup_result->EVENT_PARAM_SCTP_CAUSE = EVENT_VALUE_REMOTE;

        rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_sctp_setup_result);
    }

    /* Stop connect Timer */
    if (X2AP_P_NULL != p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id)
    {
        x2ap_stop_timer(p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id);
        p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id = X2AP_P_NULL;
    };
    /*SPR_16533_DELTA2_START*/
#ifdef LKSCTP
    if (X2AP_P_NULL !=  p_enb_cntxt_arr->x2_setup_sm_ctx.x2setup_time_to_wait_id)
    {
        x2ap_stop_timer(p_enb_cntxt_arr->x2_setup_sm_ctx.x2setup_time_to_wait_id);
        p_enb_cntxt_arr->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
    }
    if (X2AP_P_NULL !=  p_enb_cntxt_arr->x2_setup_sm_ctx.x2Setup_timerId)
    {
        x2ap_stop_timer(p_enb_cntxt_arr->x2_setup_sm_ctx.x2Setup_timerId);
        p_enb_cntxt_arr->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;
    }
#endif
    /*SPR_16533_DELTA2_END*/

    /*Bug_748_Fix_start*/
    /*p_enb_cntxt_arr->sctp_sm_ctx.connect_ret_count = 0;*/
    /*Bug_748_Fix_stop*/

    /* Delete Socket Discriptor from Tree */
    p_node_to_delete = x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                sd_search_tree), &p_enb_cntxt_arr->sctp_sd);
    if (X2AP_P_NULL != p_node_to_delete)
    {
        x2ap_st_delete_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree),
                p_node_to_delete);
        /*SPR_16533_DELTA2_START*/
        x2ap_mem_free(p_node_to_delete);
        /*SPR_16533_DELTA2_END*/
    }

    /*SPR_16533_START*/
#ifdef LKSCTP
    /* SPR 20633 changes start */
    if((p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv4) && (p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv6))
    /* SPR 20633 changes end */
    {
        X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
                    p_enb_cntxt_arr->sctp_sd);
#ifdef LINUX_PC_TEST
        ret_val = rrc_x2apOam_il_send_x2ap_oam_close_sctp_ind(
                RRC_X2AP_MODULE_ID,  /* Source module identifier */
                RRC_OAM_MODULE_ID,  /* Destination module identifier */
                p_enb_cntxt_arr->sctp_sd,
                RRC_NULL, /* Interface transaction identifier */
                RRC_NULL      /* cell index */
            );
#else
        ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd);
#endif
        /* Coverity_fix_92764_start */
        if(X2AP_FAILURE == ret_val)
        {
            X2AP_TRACE(X2AP_DETAILED,"return value of x2ap_close_association is %d",
                    ret_val);
        }
#ifndef LINUX_PC_TEST
        /* Coverity_fix_92764_end */
        x2ap_delete_assoc_id(p_enb_cntxt_arr->sctp_sd);
#endif
    }
//#elif ARICENT_SCTP_IPR
#if 0
    ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd); 
        /* Coverity_fix_92764_start */
        if(X2AP_FAILURE == ret_val)
        {
            X2AP_TRACE(X2AP_DETAILED,"return value of x2ap_close_association is %d",
                    ret_val);
        }
        /* Coverity_fix_92764_end */
#endif
#endif
    /*SPR_16533_END*/
    p_enb_cntxt_arr->sctp_sd = INVALID_SCTP_SOCKET;

    /*SPR_19018_START*/  
    /*Code Removed*/
    /* Link Down Indication will not be sent to SON if X2AP_SCTP_CONN_FAILURE_IND 
 * is received when SCTP association was not established */
    /*SPR_19018_END*/    

    /*Bug_748_Fix_start*/
    ret_val = x2ap_start_sctp_timer(X2AP_SCTP_CONNECT_TIMER, 
            p_x2ap_gb_ctx->config_param.timer_conf_info.
            sctp_connect_tim_val, &(p_enb_cntxt_arr->sctp_sm_ctx.
                conn_timer_id), p_enb_cntxt_arr);

    X2AP_TRACE(X2AP_INFO,"[%s]Start timer "
            "[X2AP_SCTP_CONNECT_TIMER]", __FUNCTION__);
    /*Bug_748_Fix_stop*/
    return ret_val;
}
/*****************************************************************************
 * Function Name  : x2ap_sctp_wait_conn_close
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context 
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function shall Stop wait for connect timer and
 *                  invoke X2AP SM.
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_wait_conn_close( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
    x2ap_return_et ret_val = X2AP_FAILURE;
    x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
    X2AP_TRACE(X2AP_DETAILED,"Unused variable in fsm :%p",p_msg_ptr);

    /* Stop connect Timer */
    if (X2AP_P_NULL != p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id)
    {
        x2ap_stop_timer(p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id);
        p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id = X2AP_P_NULL;
    };

    /* Initialize retry count */
    p_enb_cntxt_arr->sctp_sm_ctx.connect_ret_count = 0;

    p_node_to_delete = x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                sd_search_tree), &p_enb_cntxt_arr->sctp_sd);
    if (X2AP_P_NULL != p_node_to_delete)
    {
        /* Delete Socket Discriptor from Tree */
        x2ap_st_delete_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree),
                p_node_to_delete);
        /*SPR_16533_DELTA2_START*/
        x2ap_mem_free(p_node_to_delete);
        /*SPR_16533_DELTA2_END*/
    }

    /*SPR_16533_START*/
        /* Code Removed */
    /*SPR_16533_END*/

    /*SPR_16533_START*/
#ifdef LKSCTP
    /* SPR 20633 changes start */
    if((p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv4) && (p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv6))
    /* SPR 20633 changes end */
    {
        X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
                    p_enb_cntxt_arr->sctp_sd);
#ifdef LINUX_PC_TEST
        ret_val = rrc_x2apOam_il_send_x2ap_oam_close_sctp_ind(
                RRC_X2AP_MODULE_ID,  /* Source module identifier */
                RRC_OAM_MODULE_ID,  /* Destination module identifier */
                p_enb_cntxt_arr->sctp_sd,
                RRC_NULL, /* Interface transaction identifier */
                RRC_NULL      /* cell index */
            );
#else
        ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd);

        x2ap_delete_assoc_id(p_enb_cntxt_arr->sctp_sd);
#endif
    }
//#elif ARICENT_SCTP_IPR
//    ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd); 
#endif
    /*SPR_16533_END*/

    p_enb_cntxt_arr->sctp_sd = INVALID_SCTP_SOCKET;
    /*SPR_16728_START*/
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
            &(p_enb_cntxt_arr->enb_comm_info.ip_addr[0]));
    if (X2AP_P_NULL != p_node_to_delete)
    {
        /*SPR_20743_FIX_START*/
        x2ap_delete_peer_enb_from_search_tree(
            p_x2ap_gb_ctx,
                &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
            p_enb_cntxt_arr,
                p_node_to_delete);
        /*SPR_20743_FIX_END*/
    }
    /*SPR_16728_END*/

    /*SPR_18216_START*/
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
            (void*)(&p_enb_cntxt_arr->gb_enb_id));

    if (X2AP_P_NULL != p_node_to_delete)
    {
        x2ap_st_delete_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
                p_node_to_delete);

        x2ap_mem_free(p_node_to_delete);

        X2AP_TRACE (X2AP_DETAILED,"Deleted node from enb_id_search_tree.\n");
    }
    /*SPR_18216_END*/

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
            (void*)(&p_enb_cntxt_arr->gb_gnb_id));

    if ( X2AP_P_NULL != p_node_to_delete )
    {
        x2ap_st_delete_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
                p_node_to_delete);

        x2ap_mem_free(p_node_to_delete);

        X2AP_TRACE (X2AP_DETAILED,"Deleted node from gnb_id_search_tree.\n");
    }
    /* MENB CHANGES - END */
    #endif

    /* Change SM State to Wait Connect State */
    p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_IDLE_ST;
    return ret_val;
}


/*****************************************************************************
 * Function Name  : x2ap_sctp_wait_conn_timr_exp
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context 
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function handles sctp_wait_conn_timr_expiry 
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_wait_conn_timr_exp( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr)
{
    x2ap_return_et               ret_val = X2AP_FAILURE;
    x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
    LOCAL_SCTP_SETUP_RESULT_STRUCT  *p_local_sctp_setup_result = PNULL;
    /*BUG_11430_start*/
    x2ap_enb_link_down_ind_t link_down_info;
    X2AP_MEMSET(&link_down_info, 0, sizeof(x2ap_enb_link_down_ind_t));
    /*BUG_11430_stop*/

    /* ARGUSED */
    X2AP_TRACE(X2AP_DETAILED,"Unused variable in fsm :%p",p_msg_ptr);
    

    /*Generating Protocl Event LOCAL_SCTP_SETUP_RESULT*/
    p_local_sctp_setup_result = rrc_mem_get(sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));
    if (PNULL != p_local_sctp_setup_result)
    {
        memset_wrapper(p_local_sctp_setup_result, RRC_NULL, 
                sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT));

        p_local_sctp_setup_result->header.length = sizeof(LOCAL_SCTP_SETUP_RESULT_STRUCT);
        p_local_sctp_setup_result->header.event_id = LOCAL_SCTP_SETUP_RESULT;

        p_local_sctp_setup_result->EVENT_PARAM_SCTP_SETUP_RESULT = EVENT_VALUE_FAILURE;
        p_local_sctp_setup_result->EVENT_PARAM_SCTP_ENDPOINT_TYPE = EVENT_VALUE_EXTERNAL_ENB;
        l3_memcpy_wrapper(p_local_sctp_setup_result->EVENT_PARAM_IP_ADDRESS ,p_x2ap_gb_ctx->
            local_enb_info.ip_addr[0].ip_addr, MAX_X2AP_IP_ADDRESS_LENGTH);
        p_local_sctp_setup_result->EVENT_PARAM_SCTP_CAUSE = EVENT_VALUE_REMOTE;

        rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_local_sctp_setup_result);
    }
    
    /* Close already opened association */
    if (p_enb_cntxt_arr->sctp_sd != INVALID_SCTP_SOCKET)
    {
        /* Stopping x2 setup timers if running*/
        if (p_enb_cntxt_arr->x2_setup_sm_ctx.x2Setup_timerId)
        {
            x2ap_stop_timer(p_enb_cntxt_arr->x2_setup_sm_ctx.x2Setup_timerId);
            p_enb_cntxt_arr->x2_setup_sm_ctx.x2Setup_timerId = X2AP_P_NULL;
        }
        if (p_enb_cntxt_arr->x2_setup_sm_ctx.x2setup_time_to_wait_id)
        {
            x2ap_stop_timer(p_enb_cntxt_arr->x2_setup_sm_ctx.x2setup_time_to_wait_id);
            p_enb_cntxt_arr->x2_setup_sm_ctx.x2setup_time_to_wait_id = X2AP_P_NULL;
        }

    /*SPR_16533_DELTA2_START*/
#ifdef LKSCTP
        /* SPR 20633 changes start */
        if((p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv4) && (p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv6))
            /* SPR 20633 changes end */
        {
#ifdef LINUX_PC_TEST
            X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
                    p_enb_cntxt_arr->sctp_sd);
            ret_val = rrc_x2apOam_il_send_x2ap_oam_close_sctp_ind(
                RRC_X2AP_MODULE_ID,  /* Source module identifier */
                RRC_OAM_MODULE_ID,  /* Destination module identifier */
                p_enb_cntxt_arr->sctp_sd,
                RRC_NULL, /* Interface transaction identifier */
                RRC_NULL      /* cell index */
            );
#else
            ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd);
            /*SPR_20743_FIX_START*/
            x2ap_delete_assoc_id(p_enb_cntxt_arr->sctp_sd);
            /*SPR_20743_FIX_END*/
#endif
        }
//#elif ARICENT_SCTP_IPR
//    ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd); 
#endif
    /*SPR_16533_DELTA2_END*/

        p_node_to_delete = x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                    sd_search_tree), &p_enb_cntxt_arr->sctp_sd);
        if (X2AP_P_NULL != p_node_to_delete)
        {
            x2ap_st_delete_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree),
                    p_node_to_delete);
            /*SPR_16533_DELTA2_START*/
            x2ap_mem_free(p_node_to_delete);
            /*SPR_16533_DELTA2_END*/
        }
        p_enb_cntxt_arr->sctp_sd = INVALID_SCTP_SOCKET;
    }
    p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id = X2AP_P_NULL;
    /*
     ** If retry count not exceeded then connect as client and start
     ** SCTP wait for connect timer 
     */
    p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id = X2AP_P_NULL;
    /*SPR_17808_START*/
    p_enb_cntxt_arr->x2_setup_sm_ctx.x2Setup_curr_state = 
        X2_SETUP_IDLE_ST;
    /*SPR_17808_END*/

    /*SPR_18216_START*/
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
            (void*)(&p_enb_cntxt_arr->gb_enb_id));

    if (X2AP_P_NULL != p_node_to_delete)
    {
        x2ap_st_delete_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
                p_node_to_delete);

        x2ap_mem_free(p_node_to_delete);

        X2AP_TRACE (X2AP_DETAILED,"Deleted node from enb_id_search_tree.\n");
    }
    /*SPR_18216_END*/

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
            (void*)(&p_enb_cntxt_arr->gb_gnb_id));

    if ( X2AP_P_NULL != p_node_to_delete )
    {
        x2ap_st_delete_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
                p_node_to_delete);

        x2ap_mem_free(p_node_to_delete);
        X2AP_TRACE (X2AP_DETAILED,"Deleted node from gnb_id_search_tree.\n");
    }
    /* MENB CHANGES - END */
    #endif

    /* Bug_10135 Fix Start */
    /*SPR_16533_DELTA2_START*/
#ifdef LKSCTP
        
    /* Bug_10135 Fix Stop */
    /* Adding 1 for the first attempt i.e. "try" count, other count is "retry" count*/
    if ((p_x2ap_gb_ctx->config_param.timer_conf_info.x2ap_setup_retry_cnt + 1)> 
                                p_enb_cntxt_arr->x2_setup_sm_ctx.x2setup_retry_count)
#endif
#if 0
#ifdef ARICENT_SCTP_IPR
    if ((p_x2ap_gb_ctx->config_param.timer_conf_info.sctp_connect_retry_cnt == 0)||
            (p_x2ap_gb_ctx->config_param.timer_conf_info.sctp_connect_retry_cnt >
             p_enb_cntxt_arr->sctp_sm_ctx.connect_ret_count))
#endif
#endif
    /*SPR_16533_DELTA2_END*/
    /* Bug_10135 Fix Stop */
    {
        /* open client association and start connect timer */
        ret_val = x2ap_open_client_asso_start_timer(p_enb_cntxt_arr, 
                p_x2ap_gb_ctx);

    /*SPR_16533_DELTA2_START*/
#ifdef LKSCTP
      if ((p_enb_cntxt_arr->sctp_sd != INVALID_SCTP_SOCKET) &&
          (X2AP_FAILURE == x2ap_sctp_wait_conn_comm_up(p_msg_ptr,
                                                       p_x2ap_gb_ctx, p_enb_cntxt_arr)))
      {
        X2AP_TRACE(
            X2AP_INFO,
            "x2ap_sctp_wait_conn_comm_up failed");
        return X2AP_FAILURE;
      }
#endif
    /*SPR_16533_DELTA2_END*/

    }
    /*
     ** Move to IDLE State   
     */
    else
    {
    /*BUG_11430_start*/
        /* Send link down indication to SON */
        if (X2AP_TRUE == p_x2ap_gb_ctx->is_son_present)
        {

            link_down_info.bitmask = X2AP_PEER_ENB_COMM_INFO_PRESENT;
            X2AP_MEMCPY(&link_down_info.enb1_gb_id, 
                    &p_x2ap_gb_ctx->global_enb_id, 
                    sizeof(x2_gb_enb_id_t));                            
            X2AP_MEMCPY(&link_down_info.enb_comm_info,
                    &p_enb_cntxt_arr->enb_comm_info,
                    sizeof(x2_enb_comm_info_t));
            if(!(rrc_x2apCommon_send_x2ap_enb_link_down_ind(&link_down_info,
                            RRC_X2AP_MODULE_ID,RRC_SON_MODULE_ID, 0, RRC_NULL)))
            {
                X2AP_TRACE(X2AP_INFO,"Sending X2 Link Down indication"
                        "to SON failed");
            }
        }
    /*BUG_11430_stop*/
        p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_IDLE_ST;
        p_enb_cntxt_arr->sctp_sm_ctx.connect_ret_count = 0;
    /*SPR_16533_DELTA2_START*/
#ifdef LKSCTP
        x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
        p_node_to_delete = x2ap_st_find_tree_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
                &(p_enb_cntxt_arr->enb_comm_info.ip_addr[0]));

        if (X2AP_P_NULL != p_node_to_delete)
        {
            /*SPR_20743_FIX_START*/
            x2ap_delete_peer_enb_from_search_tree(
                p_x2ap_gb_ctx,
                    &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
                p_enb_cntxt_arr,
                    p_node_to_delete);
            /*SPR_20743_FIX_END*/
        }
        /* SPR 18692 Fix Start */
        p_enb_cntxt_arr->x2_setup_sm_ctx.x2setup_retry_count = 0;
        /* SPR 18692 Fix End */
#endif
    /*SPR_16533_DELTA2_END*/
    }

    return ret_val;
}
/*****************************************************************************
 * Function Name  : x2ap_sctp_act_conn_lost
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context 
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function shall Stop wait for connect timer,
 *                  invoke X2AP SM with down event and shall open client 
 *                  association.
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_act_conn_lost( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
	x2ap_return_et ret_val = X2AP_FAILURE;
	x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;


	/* invoke x2_setup sm with event x2_setup_sctp_x2_conn_up_ev */
	/* Coverity ID 81446 Starts */
	x2setup_sm_entry(X2_SETUP_SCTP_X2_CONN_DOWN_EV, p_msg_ptr,
			/* Coverity ID 81446 Ends */
			p_x2ap_gb_ctx, p_enb_cntxt_arr);

    /*SPR_16533_START*/
#ifdef LKSCTP
    /* SPR 20633 changes start */
    if((p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv4) && (p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv6))
    /* SPR 20633 changes end */
    {
        X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
                    p_enb_cntxt_arr->sctp_sd);
#ifdef LINUX_PC_TEST
        ret_val = rrc_x2apOam_il_send_x2ap_oam_close_sctp_ind(
                RRC_X2AP_MODULE_ID,  /* Source module identifier */
                RRC_OAM_MODULE_ID,  /* Destination module identifier */
                p_enb_cntxt_arr->sctp_sd,
                RRC_NULL, /* Interface transaction identifier */
                RRC_NULL      /* cell index */
            );
#else
        ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd);
        x2ap_delete_assoc_id(p_enb_cntxt_arr->sctp_sd);
#endif
    }
//#elif ARICENT_SCTP_IPR
//    ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd); 
#endif
    /*SPR_16533_END*/

	p_node_to_delete = x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
				sd_search_tree), &p_enb_cntxt_arr->sctp_sd);

	/* Delete node from SCTP SD Tree */
	if (X2AP_P_NULL != p_node_to_delete)
	{
		x2ap_st_delete_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree),
				p_node_to_delete);
        /*SPR_16533_DELTA2_START*/
        x2ap_mem_free(p_node_to_delete);
        /*SPR_16533_DELTA2_END*/
    }
    p_enb_cntxt_arr->sctp_sd = INVALID_SCTP_SOCKET;

    /*SPR_18216_START*/
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
            (void*)(&p_enb_cntxt_arr->gb_enb_id));

    if (X2AP_P_NULL != p_node_to_delete)
    {
        x2ap_st_delete_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
                p_node_to_delete);

        x2ap_mem_free(p_node_to_delete);

        X2AP_TRACE (X2AP_DETAILED,"Deleted node from enb_id_search_tree.\n");
    }
    /*SPR_18216_END*/

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
            (void*)(&p_enb_cntxt_arr->gb_gnb_id));

    if ( X2AP_P_NULL != p_node_to_delete )
    {
        x2ap_st_delete_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
                p_node_to_delete);

        x2ap_mem_free(p_node_to_delete);
        X2AP_TRACE (X2AP_DETAILED,"Deleted node from gnb_id_search_tree.\n");
    }
    /* MENB CHANGES - END */
    #endif

    /*SPR_16533_DELTA2_START*/
    /* Stop connect Timer */
    if (X2AP_P_NULL != p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id)
    {
        x2ap_stop_timer(p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id);
        p_enb_cntxt_arr->sctp_sm_ctx.conn_timer_id = X2AP_P_NULL;
    };
    /*SPR_16533_DELTA2_END*/
	ret_val = x2ap_open_client_asso_start_timer(p_enb_cntxt_arr, p_x2ap_gb_ctx);

	/* Change SM State to Wait Connect State */
	p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_W_COMM_UP;

	return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_sctp_act_peer_rstrt
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context 
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function shall Invoke X2 Setup SM with connection 
 *                  Down event.
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_act_peer_rstrt( 
		U8                   *p_msg_ptr,
		x2ap_gb_context_t   *p_x2ap_gb_ctx,
		x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{

    x2ap_return_et ret_val = X2AP_FAILURE;
    X2AP_TRACE(X2AP_DETAILED, "%s: Peer Restarted!", __FUNCTION__);

    /* invoke x2_setup sm with event x2_setup_sctp_x2_conn_down_ev */

    ret_val = x2setup_sm_entry(X2_SETUP_SCTP_X2_CONN_DOWN_EV,
            p_msg_ptr, p_x2ap_gb_ctx, p_enb_cntxt_arr);


    return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_sctp_act_shutdown
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context 
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function shall Move SM State to X2AP_SCTP_RELEASE_ONGOING_ST.
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_act_shutdown( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
	x2ap_return_et ret_val = X2AP_FAILURE;
	x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;

	/* invoke x2_setup sm with event x2_setup_sctp_x2_conn_up_ev */
	ret_val = x2setup_sm_entry(X2_SETUP_SCTP_X2_CONN_DOWN_EV, p_msg_ptr,
			p_x2ap_gb_ctx, p_enb_cntxt_arr);
    /* coverity_81514 start */
    if(X2AP_FAILURE == ret_val)
    X2AP_TRACE(X2AP_DETAILED,"x2setup_sm_entry returned failure");
    /* coverity_81514 stop */
	/*SPR_16533_START*/
#ifdef LKSCTP
    /* SPR 20633 changes start */
	if((p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv4) && (p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv6))
    /* SPR 20633 changes end */
	{
		X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
				p_enb_cntxt_arr->sctp_sd);
#ifdef LINUX_PC_TEST
              ret_val = rrc_x2apOam_il_send_x2ap_oam_close_sctp_ind(
                RRC_X2AP_MODULE_ID,  /* Source module identifier */
                RRC_OAM_MODULE_ID,  /* Destination module identifier */
                p_enb_cntxt_arr->sctp_sd,
                RRC_NULL, /* Interface transaction identifier */
                RRC_NULL      /* cell index */
            );
#else
		ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd);

		x2ap_delete_assoc_id(p_enb_cntxt_arr->sctp_sd);
#endif
	}
//#elif ARICENT_SCTP_IPR
#if 0
	ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd); 
    /* coverity_81514 start */
    if(X2AP_FAILURE == ret_val)
    X2AP_TRACE(X2AP_DETAILED,"x2ap_close_association returned failure");
    /* coverity_81514 stop */
#endif
#endif
	/*SPR_16533_END*/

	p_node_to_delete = x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
				sd_search_tree), &p_enb_cntxt_arr->sctp_sd);

	/* Delete node from SCTP SD Tree */
	if (X2AP_P_NULL != p_node_to_delete)
	{
		x2ap_st_delete_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree),
				p_node_to_delete);
		/*SPR_16533_DELTA2_START*/
		x2ap_mem_free(p_node_to_delete);
		/*SPR_16533_DELTA2_END*/
	}

	/*SPR_16728_START*/
	p_node_to_delete = x2ap_st_find_tree_node(
			&(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
			&(p_enb_cntxt_arr->enb_comm_info.ip_addr[0]));
	if (X2AP_P_NULL != p_node_to_delete)
    {
        /*SPR_20743_FIX_START*/
        x2ap_delete_peer_enb_from_search_tree(
                p_x2ap_gb_ctx,
                &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
                p_enb_cntxt_arr,
                p_node_to_delete);
        /*SPR_20743_FIX_END*/
    }
	/*SPR_16728_END*/
    /*SPR_18216_START*/
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
            (void*)(&p_enb_cntxt_arr->gb_enb_id));

    if (X2AP_P_NULL != p_node_to_delete)
    {
        x2ap_st_delete_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
                p_node_to_delete);

        x2ap_mem_free(p_node_to_delete);

        X2AP_TRACE (X2AP_DETAILED,"Deleted node from enb_id_search_tree.\n");
    }
    /*SPR_18216_END*/

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
            (void*)(&p_enb_cntxt_arr->gb_gnb_id));

    if ( X2AP_P_NULL != p_node_to_delete )
    {
        x2ap_st_delete_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
                p_node_to_delete);

        x2ap_mem_free(p_node_to_delete);
        X2AP_TRACE (X2AP_DETAILED,"Deleted node from gnb_id_search_tree.\n");
    }
    /* MENB CHANGES - END */
    #endif

	p_enb_cntxt_arr->sctp_sd = INVALID_SCTP_SOCKET;

	/* Change SM State to IDLE State */
	p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_IDLE_ST;

	return ret_val;
}


/*****************************************************************************
 * Function Name  : x2ap_sctp_act_conn_close
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function shall close active connection
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_act_conn_close( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr)
{
    x2ap_return_et ret_val = X2AP_FAILURE;
    x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;

    if (p_enb_cntxt_arr->sctp_sd != INVALID_SCTP_SOCKET)
    {
        /*SPR_16533_START*/
#ifdef LKSCTP    
        /* SPR 20633 changes start */
        if((p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv4) && (p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv6))
        /* SPR 20633 changes end */
        {
            X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
                    p_enb_cntxt_arr->sctp_sd);
#ifdef LINUX_PC_TEST
              ret_val = rrc_x2apOam_il_send_x2ap_oam_close_sctp_ind(
                RRC_X2AP_MODULE_ID,  /* Source module identifier */
                RRC_OAM_MODULE_ID,  /* Destination module identifier */
                p_enb_cntxt_arr->sctp_sd,
                RRC_NULL, /* Interface transaction identifier */
                RRC_NULL      /* cell index */
            );
#else
            ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd);

            x2ap_delete_assoc_id(p_enb_cntxt_arr->sctp_sd);
#endif
        }
//#elif ARICENT_SCTP_IPR
//    ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd); 
#endif
        /*SPR_16533_END*/

        p_enb_cntxt_arr->sctp_sm_ctx.connect_ret_count = 0;
        /* Change SM State to Idle State */
        p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = 
            X2AP_SCTP_IDLE_ST;

        p_node_to_delete = x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                    sd_search_tree), &p_enb_cntxt_arr->sctp_sd);

        /* Delete node from SCTP SD Tree */
        if (X2AP_P_NULL != p_node_to_delete)
        {
            x2ap_st_delete_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree),
                    p_node_to_delete);
            /*SPR_16533_DELTA2_START*/
            x2ap_mem_free(p_node_to_delete);
            /*SPR_16533_DELTA2_END*/
        }
        /*SPR_16728_START*/
        p_node_to_delete = x2ap_st_find_tree_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
                &(p_enb_cntxt_arr->enb_comm_info.ip_addr[0]));
        if (X2AP_P_NULL != p_node_to_delete)
        {
            /*SPR_20743_FIX_START*/
            x2ap_delete_peer_enb_from_search_tree(
                p_x2ap_gb_ctx,
                    &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
                p_enb_cntxt_arr,
                    p_node_to_delete);
            /*SPR_20743_FIX_END*/
        }
        /*SPR_16728_END*/
       
        
        /*SPR_18216_START*/
        p_node_to_delete = x2ap_st_find_tree_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
                (void*)(&p_enb_cntxt_arr->gb_enb_id));

        if (X2AP_P_NULL != p_node_to_delete)
        {
            x2ap_st_delete_node(
                    &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
                    p_node_to_delete);

            x2ap_mem_free(p_node_to_delete);

            X2AP_TRACE (X2AP_DETAILED,"Deleted node from enb_id_search_tree.\n");
        }
        /*SPR_18216_END*/

        #ifdef ENDC_ENABLED
        /* MENB CHANGES - START */
        p_node_to_delete = x2ap_st_find_tree_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
                (void*)(&p_enb_cntxt_arr->gb_gnb_id));

        if ( X2AP_P_NULL != p_node_to_delete )
        {
            x2ap_st_delete_node(
                    &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
                    p_node_to_delete);

            x2ap_mem_free(p_node_to_delete);
            X2AP_TRACE (X2AP_DETAILED,"Deleted node from gnb_id_search_tree.\n");
        }
        /* MENB CHANGES - END */
        #endif

        /* invoke x2_setup sm with event x2_setup_sctp_x2_conn_up_ev */
        ret_val = x2setup_sm_entry(X2_SETUP_SCTP_X2_CONN_DOWN_EV, p_msg_ptr,
                p_x2ap_gb_ctx, p_enb_cntxt_arr);
        
        p_enb_cntxt_arr->sctp_sd = INVALID_SCTP_SOCKET;

    }

    return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_sctp_act_snd_data
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function shall send message to peer ENB Via SCTP
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_act_snd_data( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr)
{
    x2ap_return_et         ret_val = X2AP_FAILURE;
    /* SPR 17213 Fix Start */
    /* code deleted */
    /* SPR 17213 Fix End */
    U8		          *p_msg_buff = X2AP_P_NULL;
    x2ap_peer_enb_msg_t  *p_sctp_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;
    X2AP_TRACE(X2AP_DETAILED,"Unused variable in fsm :%p",p_x2ap_gb_ctx);

    p_msg_buff = (U8*)x2ap_msg_mem_get(p_sctp_msg->msg_len);
    
    if (PNULL != p_msg_buff)
    {
        X2AP_MEMCPY(p_msg_buff , p_sctp_msg->p_msg, p_sctp_msg->msg_len);  
#ifdef LINUX_PC_TEST
#ifdef LKSCTP
        struct sockaddr_in serverAddr = {0};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr =  
            inet_addr_wrapper((const char*)(p_enb_cntxt_arr->enb_comm_info.ip_addr[0].ip_addr)) ;
        serverAddr.sin_port = (unsigned short)htons_wrapper(p_enb_cntxt_arr->enb_comm_info.port);
                        
        pctest_forward_x2ap_lksctp_message_to_pc(RRC_X2AP_MODULE_ID,
                RRC_X2AP_PEER_eNODEB_MODULE_ID,
                X2AP_ENB_MESSAGE_PS_TO_TEST,
                p_enb_cntxt_arr->sctp_sd,
                X2AP_NULL,      //p_sctp_msg->stream_id,
                (struct sockaddr *)(&serverAddr),   
                p_sctp_msg->p_msg,
                p_sctp_msg->msg_len);
#else
        pctest_forward_mme_message_to_pc(RRC_X2AP_MODULE_ID,
                RRC_X2AP_PEER_eNODEB_MODULE_ID,
                X2AP_ENB_MESSAGE_PS_TO_TEST,
                p_enb_cntxt_arr->sctp_sd,
                p_sctp_msg->stream_id,
                p_sctp_msg->p_msg,
                p_sctp_msg->msg_len);
#endif

        X2AP_TRACE(X2AP_DETAILED, "test:Sending the msg to Peer eNB via qvsend.msg_len=%d",p_sctp_msg->msg_len);
#else
        x2ap_sctp_send(p_enb_cntxt_arr->sctp_sd,
                p_msg_buff, p_sctp_msg->msg_len,/* SPR_14436_Fix Start */ 
                p_sctp_msg->stream_id); 
#endif

        /*SPR_18125_START*/
        x2ap_msg_mem_free(p_msg_buff);
        /*SPR_18125_END*/

 		/* SPR_14436_Fix Stop */
    }

    return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_sctp_rcv_data
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function shall distribute message received from Peer 
 *                  ENB.
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_rcv_data( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr)
{
    x2ap_return_et         ret_val = X2AP_FAILURE;
    x2ap_peer_enb_msg_t  *p_sctp_msg = (x2ap_peer_enb_msg_t*)p_msg_ptr;
    x2ap_EP_et            msg_type = X2SETUP_REQ;
    U16                   api = X2AP_NULL;
    x2ap_ProcedureCode    proc_code = X2AP_NULL;
    x2ap_UE_X2AP_ID       oldEnbUeX2apId;
    x2ap_UE_X2AP_ID       newEnbUeX2apId;
    U8                    current_state;

    rrc_x2ap_error_indication_t   error_ind;

#ifdef ENDC_ENABLED
   /*NR_DC Code Change Start*/
    U32                         mEnbUeX2apId = RRC_INVALID_DC_UE_X2AP_ID;
    U32                         sgnbUeX2apId = RRC_INVALID_DC_UE_X2AP_ID;
   /*NR_DC Code Change Stop*/
#endif

    X2AP_MEMSET(&oldEnbUeX2apId, X2AP_NULL, sizeof(x2ap_UE_X2AP_ID));
    X2AP_MEMSET(&newEnbUeX2apId, X2AP_NULL, sizeof(x2ap_UE_X2AP_ID));
    X2AP_MEMSET (&error_ind, X2AP_NULL, sizeof(rrc_x2ap_error_indication_t));

#ifdef ENDC_ENABLED
   /*NR_DC Code Change Start*/
    X2AP_MEMSET(&mEnbUeX2apId, X2AP_NULL, sizeof(U32));
    X2AP_MEMSET(&sgnbUeX2apId, X2AP_NULL, sizeof(U32));
   /*NR_DC Code Change Stop*/
#endif
    /* Call the asn decode function to decode the msg_type */
    ret_val = x2ap_partially_decode_asn_msg (
            p_x2ap_gb_ctx,p_enb_cntxt_arr,
            p_sctp_msg->p_msg, p_sctp_msg->msg_len,
            &msg_type, &proc_code, &oldEnbUeX2apId,
            &newEnbUeX2apId
#ifdef ENDC_ENABLED
   /*NR_DC Code Change Start*/
            ,&mEnbUeX2apId,
            &sgnbUeX2apId
   /*NR_DC Code Change Stop*/
#endif
            );

    if (X2AP_FAILURE == ret_val)
    {
        X2AP_TRACE (X2AP_INFO, "Decode failed for asn msg !!");
        return X2AP_FAILURE;
    }

#ifdef ENDC_ENABLED
   /*NR_DC Code Change Start*/
    if ((RRC_INVALID_DC_UE_X2AP_ID == mEnbUeX2apId) ||
          ((ASN1V_x2ap_id_sgNBAdditionPreparation == proc_code || 
            ASN1V_x2ap_id_secondaryRATDataUsageReport == proc_code) &&
              (RRC_INVALID_DC_UE_X2AP_ID == sgnbUeX2apId)) 
       )
    {
        X2AP_TRACE (X2AP_INFO, "Decode failed for asn msg !!");
        /*build and send logical error indication*/
        error_ind.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
        error_ind.cause.t = T_x2ap_Cause_protocol;
        error_ind.cause.u.protocol = 
            x2ap_abstract_syntax_error_reject;

        x2ap_build_and_send_err_indication(
                p_x2ap_gb_ctx,
                p_enb_cntxt_arr,
                &error_ind);
        return X2AP_FAILURE;
    }
   /*NR_DC Code Change Stop*/
#endif

    /*SPR_6379_fix Start*/
    if (X2_SETUP_ACTIVE_ST != p_enb_cntxt_arr->x2_setup_sm_ctx.x2Setup_curr_state)   
    /*SPR_6379_fix Stop*/
    {
        if ((X2SETUP_REQ == msg_type) ||
            (X2_SETUP_RESP == msg_type) ||
            #ifdef ENDC_ENABLED
            /* MENB CHANGES - START */
            (ENDC_X2SETUP_REQ == msg_type) ||
            (ENDC_X2SETUP_RESP == msg_type) ||
            (ENDC_X2SETUP_FAIL == msg_type) ||
            /* MENB CHANGES - END */
            #endif
            (X2_SETUP_FAIL == msg_type))
        {
            X2AP_TRACE (X2AP_DETAILED,"Received message is [%d] !!",msg_type);
        }
        else
        {
            X2AP_TRACE( X2AP_ERROR,
                "This is the first message received for a This specific TNL "
                "association so it should be X2 SETUP REQUEST, X2 SETUP "
                "RESPONSE, or X2 SETUP FAILURE !!");
            /*SPR_18154_START*/
            if(msg_type != ERROR_INDICATION)
            {
            /*build and send logical error indication*/
            error_ind.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
            error_ind.cause.t = T_x2ap_Cause_protocol;
            error_ind.cause.u.protocol = 
                  x2ap_message_not_compatible_with_receiver_state;
                     
            x2ap_build_and_send_err_indication(
                      p_x2ap_gb_ctx,
                      p_enb_cntxt_arr,
                      &error_ind);
            }
            /*SPR_18154_END*/
            return X2AP_FAILURE;
        }
    }
 
    if ((X2_RESET_W_RESET_RESP_ST == p_enb_cntxt_arr->x2ap_reset_sm_ctx.state)
        && (msg_type != RESET_REQUEST) && (msg_type != RESET_RESPONSE))
    {
        X2AP_TRACE (X2AP_INFO, "eNB Context is in W_RESET_RESP state.");
        return X2AP_FAILURE;
    }
 
    p_sctp_msg->oldEnbUeX2apId = oldEnbUeX2apId;
    p_sctp_msg->newEnbUeX2apId = newEnbUeX2apId;

    /*SPR_18154_START*/
    /*If flag for consecutive x2_setup_response_failure was TRUE and the recived
     * message is not X2_SETUP_FAILURE then turn off the flag*/
    if ((X2AP_TRUE == p_enb_cntxt_arr->flag_for_consecutive_x2setup_resp_failure ) &&
            (X2_SETUP_FAIL != msg_type))
    {
        p_enb_cntxt_arr->flag_for_consecutive_x2setup_resp_failure = X2AP_FALSE;
    }
    /*SPR_18154_END*/

    #ifdef ENDC_ENABLED
    /*
     * If flag for consecutive endc_x2_setup_response_failure was TRUE and the recived
     * message is not ENDC_X2SETUP_FAILURE then turn off the flag
     */
    if ( ( X2AP_TRUE == p_enb_cntxt_arr->flag_for_consecutive_endc_x2setup_resp_failure ) &&
            ( ENDC_X2SETUP_FAIL != msg_type ) ) 
    {
        p_enb_cntxt_arr->flag_for_consecutive_endc_x2setup_resp_failure = X2AP_FALSE;
    }
    #endif

    switch (msg_type)
    {
        #ifdef ENDC_ENABLED
        /* MENB CHANGES - START */
         case ENDC_X2SETUP_REQ:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            api = ENDC_X2_SETUP_REQ_EV;
            x2setup_sm_entry(api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        case ENDC_X2SETUP_RESP:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            api = X2_SETUP_RESP_EV;
            x2setup_sm_entry(api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        case ENDC_X2SETUP_FAIL:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            api = ENDC_X2_SETUP_FAIL_EV;
            x2setup_sm_entry(api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        case ENDC_CONFIGURATION_UPDATE_REQ:
        {
        	x2ap_process_endc_config_update_en_gnb_req((U8*)p_sctp_msg,p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        case ENDC_CONFIGURATION_UPDATE_RESP:
        {
        	x2ap_process_endc_config_update_ack((U8*)p_sctp_msg,p_x2ap_gb_ctx, p_enb_cntxt_arr);
        	break;
        }
        case ENDC_CONFIGURATION_UPDATE_FAIL:
        {
        	x2ap_process_endc_config_update_failure((U8*)p_sctp_msg,p_x2ap_gb_ctx, p_enb_cntxt_arr);
        	break;
        }
        /* MENB CHANGES - END */
        #endif

        case X2SETUP_REQ:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            api = X2_SETUP_REQ_EV;
            x2setup_sm_entry(api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        case X2_SETUP_RESP:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            api = X2_SETUP_RESP_EV;
            x2setup_sm_entry(api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        case X2_SETUP_FAIL:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            api = X2_SETUP_FAIL_EV;
            x2setup_sm_entry(api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        case ENB_CONFIGURE_UPDATE_REQ:
        {
        	x2ap_process_enb_config_update_enb_req((U8*)p_sctp_msg,p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        case ENB_CONFIGURE_UPDATE_RESP:
        {
        	x2ap_process_enb_config_update_ack((U8*)p_sctp_msg,p_x2ap_gb_ctx, p_enb_cntxt_arr);
        	break;
        }
        case ENB_CONFIGURE_UPDATE_FAIL:
        {
        	x2ap_process_enb_config_update_failure((U8*)p_sctp_msg,p_x2ap_gb_ctx, p_enb_cntxt_arr);
        	break;
        }
        case X2AP_UE_ASSO_SIG_MSG_IND_HO_REQ:
        {
            p_sctp_msg->ue_ass_msg = X2AP_TRUE;
            api = X2AP_UE_ASSO_SIG_MSG_IND_EV;

            current_state = p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state;
            x2ap_ue_asso_sig_sm_entry (api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr, proc_code);

            X2AP_TRACE (X2AP_INFO, "UE_X2AP_Index: %d Old State: %s Event %s New State %s ",
                     p_sctp_msg->oldEnbUeX2apId, X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[current_state],
                     X2AP_UE_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_IND_EV],
                     X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);

            break;
        } 
#ifdef ENDC_ENABLED
   /*NR_DC Code Change Start*/
        case X2AP_UE_ASSO_SIG_MSG_IND_SGNB_ADD_REQ_ACK:
        {
            p_sctp_msg->mEnbUeX2apId = mEnbUeX2apId;
            p_sctp_msg->sgnbUeX2apId = sgnbUeX2apId;
            p_sctp_msg->ue_ass_msg   = X2AP_TRUE;
            api = X2AP_UE_ASSO_SIG_MSG_IND_EV;
#if 0
#ifdef RRC_UNIT_TEST_FRAMEWORK
            p_enb_cntxt_arr->
                x2ap_ue_asso_sig_sm_ctx.
                x2ap_ue_asso_sig_curr_state = 
                X2AP_UE_ASSO_SIG_ACTIVE_ST;
#endif
#endif

            current_state = p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.
                                             x2ap_ue_asso_sig_curr_state;
            x2ap_ue_asso_sig_sm_entry (api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr, proc_code);
            
            X2AP_TRACE (X2AP_INFO, "MENB_UE_X2AP_Index: %d "
                        "Old State: %s"
                        "Event %s New State %s ",
                        p_sctp_msg->mEnbUeX2apId,
                        X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[
                        current_state],
                        X2AP_UE_ASSO_SIG_FSM_EVENTS_NAMES[
                        X2AP_UE_ASSO_SIG_MSG_IND_EV],
                        X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[
                        p_enb_cntxt_arr->
                        x2ap_ue_asso_sig_sm_ctx.
                        x2ap_ue_asso_sig_curr_state]);
            break;
        }
   /*NR_DC Code Change Stop*/
        //rekha 23april
        case X2AP_UE_ASSO_SIG_MSG_IND_SGNB_ADD_REQ_REJ:
        {
            p_sctp_msg->mEnbUeX2apId = mEnbUeX2apId;
            p_sctp_msg->sgnbUeX2apId = sgnbUeX2apId;
            p_sctp_msg->ue_ass_msg   = X2AP_TRUE;
            api = X2AP_UE_ASSO_SIG_MSG_IND_EV;
            current_state = p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.
                                             x2ap_ue_asso_sig_curr_state;
            x2ap_ue_asso_sig_sm_entry (api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr, proc_code);
            
            X2AP_TRACE (X2AP_INFO, "MENB_UE_X2AP_Index: %d "
                        "Old State: %s"
                        "Event %s New State %s ",
                        p_sctp_msg->mEnbUeX2apId,
                        X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[
                        current_state],
                        X2AP_UE_ASSO_SIG_FSM_EVENTS_NAMES[
                        X2AP_UE_ASSO_SIG_MSG_IND_EV],
                        X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[
                        p_enb_cntxt_arr->
                        x2ap_ue_asso_sig_sm_ctx.
                        x2ap_ue_asso_sig_curr_state]);
            break;
        }
        case X2AP_UE_ASSO_SIG_MSG_IND_SGNB_REL_REQ_ACK:
        {
            p_sctp_msg->mEnbUeX2apId = mEnbUeX2apId;
            p_sctp_msg->sgnbUeX2apId = sgnbUeX2apId;
            p_sctp_msg->ue_ass_msg   = X2AP_TRUE;
            api = X2AP_UE_ASSO_SIG_MSG_IND_EV;
#if 0
#ifdef RRC_UNIT_TEST_FRAMEWORK
            p_enb_cntxt_arr->
                x2ap_ue_asso_sig_sm_ctx.
                x2ap_ue_asso_sig_curr_state = 
                X2AP_UE_ASSO_SIG_ACTIVE_ST;
#endif
#endif

            current_state = p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.
                                             x2ap_ue_asso_sig_curr_state;
            x2ap_ue_asso_sig_sm_entry (api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr, proc_code);
            
            X2AP_TRACE (X2AP_INFO, "MENB_UE_X2AP_Index: %d "
                        "Old State: %s"
                        "Event %s New State %s ",
                        p_sctp_msg->mEnbUeX2apId,
                        X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[
                        current_state],
                        X2AP_UE_ASSO_SIG_FSM_EVENTS_NAMES[
                        X2AP_UE_ASSO_SIG_MSG_IND_EV],
                        X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[
                        p_enb_cntxt_arr->
                        x2ap_ue_asso_sig_sm_ctx.
                        x2ap_ue_asso_sig_curr_state]);
            break;
        }
        case X2AP_UE_ASSO_SIG_MSG_IND_SGNB_REL_REQ:
        {
            p_sctp_msg->mEnbUeX2apId = mEnbUeX2apId;
            p_sctp_msg->sgnbUeX2apId = sgnbUeX2apId;
            p_sctp_msg->ue_ass_msg   = X2AP_TRUE;
            api = X2AP_UE_ASSO_SIG_MSG_IND_EV;

            current_state = p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.
                                             x2ap_ue_asso_sig_curr_state;
            x2ap_ue_asso_sig_sm_entry (api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr, proc_code);
            
            X2AP_TRACE (X2AP_INFO, "SGNB_UE_X2AP_Index: %d "
                        "Old State: %s"
                        "Event %s New State %s ",
                        p_sctp_msg->mEnbUeX2apId,
                        X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[
                        current_state],
                        X2AP_UE_ASSO_SIG_FSM_EVENTS_NAMES[
                        X2AP_UE_ASSO_SIG_MSG_IND_EV],
                        X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[
                        p_enb_cntxt_arr->
                        x2ap_ue_asso_sig_sm_ctx.
                        x2ap_ue_asso_sig_curr_state]);
            break;
        }

        case X2AP_UE_ASSO_SIG_MSG_IND_SECONDARY_RAT_DATA_USAGE_REPORT:
        {
            p_sctp_msg->mEnbUeX2apId = mEnbUeX2apId;
            p_sctp_msg->sgnbUeX2apId = sgnbUeX2apId;
            p_sctp_msg->ue_ass_msg   = X2AP_TRUE;
            api = X2AP_UE_ASSO_SIG_MSG_IND_EV;

            current_state = p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state;
            x2ap_ue_asso_sig_sm_entry (api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr, proc_code);
            
            X2AP_TRACE (X2AP_INFO, "MENB_UE_X2AP_Index: %d "
                        "Old State: %s"
                        "Event %s New State %s ",
                        p_sctp_msg->mEnbUeX2apId,
                        X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[
                        current_state],
                        X2AP_UE_ASSO_SIG_FSM_EVENTS_NAMES[
                        X2AP_UE_ASSO_SIG_MSG_IND_EV],
                        X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[
                        p_enb_cntxt_arr->
                        x2ap_ue_asso_sig_sm_ctx.
                        x2ap_ue_asso_sig_curr_state]);
            break;
        }

   /*NR_DC Code Change Stop*/
#endif
        case X2AP_UE_ASSO_SIG_MSG_IND_HO_REQ_ACK:
        {
            p_sctp_msg->ue_ass_msg = X2AP_TRUE;
            api = X2AP_UE_ASSO_SIG_MSG_IND_EV;
            current_state = p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state;
            x2ap_ue_asso_sig_sm_entry (api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr, proc_code);

            X2AP_TRACE (X2AP_INFO, "UE_X2AP_Index: %d Old State: %s Event %s New State %s ",
                     p_sctp_msg->newEnbUeX2apId, X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[current_state],
                     X2AP_UE_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_IND_EV],
                     X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);
            break;
        } 
        case X2AP_UE_ASSO_SIG_MSG_IND_OTHERS:
        {
            p_sctp_msg->ue_ass_msg = X2AP_TRUE;
            api = X2AP_UE_ASSO_SIG_MSG_IND_EV;
            current_state = p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state;
            x2ap_ue_asso_sig_sm_entry (api, (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx, p_enb_cntxt_arr, proc_code);
            
            X2AP_TRACE (X2AP_INFO, "UE_X2AP_Index: %d Old State: %s Event %s New State %s ",
                     p_sctp_msg->newEnbUeX2apId, X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[current_state],
                     X2AP_UE_ASSO_SIG_FSM_EVENTS_NAMES[X2AP_UE_ASSO_SIG_MSG_IND_EV],
                     X2AP_UE_ASSO_SIG_FSM_STATES_NAMES[p_enb_cntxt_arr->x2ap_ue_asso_sig_sm_ctx.x2ap_ue_asso_sig_curr_state]);

            break;
        }
        case RESOURCE_STATUS_REQUEST:
        {
            p_sctp_msg->ue_ass_msg = X2AP_TRUE;
            x2ap_rsu_process_msg((U8*)p_sctp_msg,p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        case RESOURCE_STATUS_RESPONSE:
        {
            p_sctp_msg->ue_ass_msg = X2AP_TRUE;
            x2ap_rsu_process_msg((U8*)p_sctp_msg,p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        case RESOURCE_STATUS_FAILURE:
        {
            p_sctp_msg->ue_ass_msg = X2AP_TRUE;
            x2ap_rsu_process_msg((U8*)p_sctp_msg,p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        case RESOURCE_STATUS_UPDATE:
        {
            p_sctp_msg->ue_ass_msg = X2AP_TRUE;
            x2ap_rsu_process_update_msg((U8*)p_sctp_msg,p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        case LOAD_INFORMATION:
        {
            p_sctp_msg->ue_ass_msg = X2AP_TRUE;
            x2ap_process_load_information_msg((U8*)p_sctp_msg,
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr);
            break;
        } 
        case ERROR_INDICATION:
        {
            p_sctp_msg->ue_ass_msg = X2AP_TRUE;
            x2ap_process_error_indication_msg((U8*)p_sctp_msg,
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr);
            break;
        } 
        
        case RESET_REQUEST:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            x2ap_process_reset_request_msg((U8*)p_sctp_msg,
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr);
            break;
        } 
        
        case RESET_RESPONSE:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            x2ap_process_reset_response_msg((U8*)p_sctp_msg,
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr);
            break;
        } 

        case RADIO_LINK_FAILURE_INDICATION:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            x2ap_process_radio_link_failure_indication(
                    (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr);
            break;
        }
        
        case HANDOVER_REPORT:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            x2ap_process_handover_report((U8*)p_sctp_msg,
                            p_x2ap_gb_ctx, p_enb_cntxt_arr);
            break;
        }
        
        case CELL_ACTIVATION_REQUEST:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            x2ap_enb_process_cell_activation_request_msg (
                    (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr);
            break;
        }
        
        case CELL_ACTIVATION_RESPONSE:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            x2ap_enb_process_cell_activation_response_msg (
                    (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr);
            break;
        }
        case CELL_ACTIVATION_FAILURE:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            x2ap_enb_process_cell_activation_failure_msg (
                    (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr);
            break;
        }
        case MOBILITY_CHANGE_REQUEST:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            x2ap_process_mobility_change_request_msg(
                    (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr);
            break;
        }
        
        case MOBILITY_CHANGE_RESPONSE:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            x2ap_process_mobility_change_response_msg(
                    (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr);
            break;
        }
        case MOBILITY_CHANGE_FAILURE:
        {
            p_sctp_msg->ue_ass_msg = X2AP_FALSE;
            x2ap_process_mobility_change_failure_msg(
                    (U8*)p_sctp_msg,
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr);
            break;
        }
        
        default:
        {
            X2AP_TRACE(X2AP_ERROR, "Invalid Message received !!");
            ret_val = X2AP_FAILURE;     
            break;
        }
    }
    return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_sctp_rel_ongo_conn_lost
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context 
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function shall Invode X2Setup SM with Setup Down Event,
 *                  delete SD from SCTP SD Tree and Should move SM to IDLE State.
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_rel_ongo_conn_lost( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
    x2ap_return_et ret_val = X2AP_FAILURE;
    x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
    /* Delete Socket Discriptor from Tree */

    /* invoke x2_setup sm with event x2_setup_sctp_x2_conn_up_ev */
    ret_val = x2setup_sm_entry(X2_SETUP_SCTP_X2_CONN_DOWN_EV, p_msg_ptr,
            p_x2ap_gb_ctx, p_enb_cntxt_arr);

    /*SPR_16533_START*/
#ifdef LKSCTP
    /* SPR 20633 changes start */
    if((p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv4) && (p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv6))
    /* SPR 20633 changes end */
    {
        X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
                    p_enb_cntxt_arr->sctp_sd);
#ifdef LINUX_PC_TEST
              ret_val = rrc_x2apOam_il_send_x2ap_oam_close_sctp_ind(
                RRC_X2AP_MODULE_ID,  /* Source module identifier */
                RRC_OAM_MODULE_ID,  /* Destination module identifier */
                p_enb_cntxt_arr->sctp_sd,
                RRC_NULL, /* Interface transaction identifier */
                RRC_NULL      /* cell index */
            );
#else
        ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd);

	x2ap_delete_assoc_id(p_enb_cntxt_arr->sctp_sd);
#endif
    }
//#elif ARICENT_SCTP_IPR
//    ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd); 
#endif
    /*SPR_16533_END*/

    p_node_to_delete = x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                sd_search_tree), &p_enb_cntxt_arr->sctp_sd);
    if (X2AP_P_NULL != p_node_to_delete)
    {
        x2ap_st_delete_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree),
                p_node_to_delete);
        /*SPR_16533_DELTA2_START*/
        x2ap_mem_free(p_node_to_delete);
        /*SPR_16533_DELTA2_END*/
        p_enb_cntxt_arr->sctp_sd = INVALID_SCTP_SOCKET;
    }
    /*SPR_16728_START*/
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
            &(p_enb_cntxt_arr->enb_comm_info.ip_addr[0]));
    if (X2AP_P_NULL != p_node_to_delete)
    {
        /*SPR_20743_FIX_START*/
        x2ap_delete_peer_enb_from_search_tree(
            p_x2ap_gb_ctx,
                &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
            p_enb_cntxt_arr,
                p_node_to_delete);
        /*SPR_20743_FIX_END*/
    }
    /*SPR_16728_END*/
    /*SPR_18216_START*/
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
            (void*)(&p_enb_cntxt_arr->gb_enb_id));

    if (X2AP_P_NULL != p_node_to_delete)
    {
        x2ap_st_delete_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
                p_node_to_delete);

        x2ap_mem_free(p_node_to_delete);

        X2AP_TRACE (X2AP_DETAILED,"Deleted node from enb_id_search_tree.\n");
    }
    /*SPR_18216_END*/

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
            (void*)(&p_enb_cntxt_arr->gb_gnb_id));

    if ( X2AP_P_NULL != p_node_to_delete )
    {
        x2ap_st_delete_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
                p_node_to_delete);

        x2ap_mem_free(p_node_to_delete);
        X2AP_TRACE (X2AP_DETAILED,"Deleted node from gnb_id_search_tree.\n");
    }
    /* MENB CHANGES - END */
    #endif

    p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_IDLE_ST;

    return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_sctp_rel_ongo_conn_clsd
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context 
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function shall Invode X2Setup SM with Setup Down Event,
 *                 delete SD from SCTP SD Tree and Should move SM to IDLE State.
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_rel_ongo_conn_clsd( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
    x2ap_return_et ret_val = X2AP_FAILURE;
    x2ap_search_tree_node_t    *p_node_to_delete = X2AP_P_NULL;
    

    /* invoke x2_setup sm with event x2_setup_sctp_x2_conn_up_ev */
    ret_val = x2setup_sm_entry(X2_SETUP_SCTP_X2_CONN_DOWN_EV, p_msg_ptr,
            p_x2ap_gb_ctx, p_enb_cntxt_arr);

    /*SPR_16533_START*/
#ifdef LKSCTP
    /* SPR 20633 changes start */
    if((p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv4) && (p_enb_cntxt_arr->sctp_sd != g_sctp_server_fd_ipv6))
    /* SPR 20633 changes end */
    {
        X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
                    p_enb_cntxt_arr->sctp_sd);
#ifdef LINUX_PC_TEST
              ret_val = rrc_x2apOam_il_send_x2ap_oam_close_sctp_ind(
                RRC_X2AP_MODULE_ID,  /* Source module identifier */
                RRC_OAM_MODULE_ID,  /* Destination module identifier */
                p_enb_cntxt_arr->sctp_sd,
                RRC_NULL, /* Interface transaction identifier */
                RRC_NULL      /* cell index */
            );
#else
        ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd);

        x2ap_delete_assoc_id(p_enb_cntxt_arr->sctp_sd);
#endif
    }
//#elif ARICENT_SCTP_IPR
//    ret_val = x2ap_close_association(p_enb_cntxt_arr->sctp_sd); 
#endif
    /*SPR_16533_END*/

    /*Find And Delete SCTP Node From SCTO SD Tree */
    p_node_to_delete = x2ap_st_find_tree_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                sd_search_tree), &p_enb_cntxt_arr->sctp_sd);
    if (X2AP_P_NULL != p_node_to_delete)
    {
        x2ap_st_delete_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.sd_search_tree),
                p_node_to_delete);
        /*SPR_16533_DELTA2_START*/
        x2ap_mem_free(p_node_to_delete);
        /*SPR_16533_DELTA2_END*/
    }
    /*SPR_16728_START*/
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
            &(p_enb_cntxt_arr->enb_comm_info.ip_addr[0]));
    if (X2AP_P_NULL != p_node_to_delete)
    {
        /*SPR_20743_FIX_START*/
        x2ap_delete_peer_enb_from_search_tree(
            p_x2ap_gb_ctx,
                &(p_x2ap_gb_ctx->enb_ctxt_trees.ip_search_tree),
            p_enb_cntxt_arr,
                p_node_to_delete);
        /*SPR_20743_FIX_END*/
    }
    /*SPR_16728_END*/
    p_enb_cntxt_arr->sctp_sd = INVALID_SCTP_SOCKET;

    /*SPR_18216_START*/
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
            (void*)(&p_enb_cntxt_arr->gb_enb_id));

    if (X2AP_P_NULL != p_node_to_delete)
    {
        x2ap_st_delete_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.enb_id_search_tree),
                p_node_to_delete);

        x2ap_mem_free(p_node_to_delete);

        X2AP_TRACE (X2AP_DETAILED,"Deleted node from enb_id_search_tree.\n");
    }
    /*SPR_18216_END*/

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    p_node_to_delete = x2ap_st_find_tree_node(
            &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
            (void*)(&p_enb_cntxt_arr->gb_gnb_id));

    if ( X2AP_P_NULL != p_node_to_delete )
    {
        x2ap_st_delete_node(
                &(p_x2ap_gb_ctx->enb_ctxt_trees.gnb_id_search_tree),
                p_node_to_delete);

        x2ap_mem_free(p_node_to_delete);
        X2AP_TRACE (X2AP_DETAILED,"Deleted node from gnb_id_search_tree.\n");
    }
    /* MENB CHANGES - END */
    #endif

    p_enb_cntxt_arr->sctp_sm_ctx.cur_sctp_sm_state = X2AP_SCTP_IDLE_ST;

    return ret_val;
}



/*****************************************************************************
 * Function Name  : x2ap_sctp_act_server_conn_close
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function shall close active connection
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_act_server_conn_close( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr)
{
    x2ap_return_et ret_val = X2AP_FAILURE;
    /* ARGUSED */
    X2AP_TRACE(X2AP_DETAILED,"Unused variable in fsm p_msg_ptr:%p p_enb_cntxt_arr:%p",
            p_msg_ptr,p_enb_cntxt_arr);
    /* SPR 20633 changes start */
//#ifdef ARICENT_SCTP_IPR
#if 0
    if(p_x2ap_gb_ctx->server_fd != INVALID_SCTP_SOCKET)
    {
        ret_val = x2ap_close_association(p_x2ap_gb_ctx->server_fd);
    }
#endif
//#else
    if (p_x2ap_gb_ctx->server_fd_ipv4 != INVALID_SCTP_SOCKET)
    {
#ifdef LINUX_PC_TEST
        X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
				p_x2ap_gb_ctx->server_fd_ipv4);
        ret_val = rrc_x2apOam_il_send_x2ap_oam_close_sctp_ind(
                RRC_X2AP_MODULE_ID,  /* Source module identifier */
                RRC_OAM_MODULE_ID,  /* Destination module identifier */
                p_x2ap_gb_ctx->server_fd_ipv4,
                RRC_NULL, /* Interface transaction identifier */
                RRC_NULL      /* cell index */
            );
#else
        ret_val = x2ap_close_association(p_x2ap_gb_ctx->server_fd_ipv4);
#endif

	    p_x2ap_gb_ctx->server_fd_ipv4 = INVALID_SCTP_SOCKET;
    }
    if (p_x2ap_gb_ctx->server_fd_ipv6 != INVALID_SCTP_SOCKET)
    {
#ifdef LINUX_PC_TEST
        X2AP_TRACE(X2AP_DETAILED,"Closing X2 association for socket = %d",
				p_x2ap_gb_ctx->server_fd_ipv6);
        ret_val = rrc_x2apOam_il_send_x2ap_oam_close_sctp_ind(
                RRC_X2AP_MODULE_ID,  /* Source module identifier */
                RRC_OAM_MODULE_ID,  /* Destination module identifier */
                p_x2ap_gb_ctx->server_fd_ipv6,
                RRC_NULL, /* Interface transaction identifier */
                RRC_NULL      /* cell index */
            );
#else
        ret_val = x2ap_close_association(p_x2ap_gb_ctx->server_fd_ipv6);
#endif

	    p_x2ap_gb_ctx->server_fd_ipv6 = INVALID_SCTP_SOCKET;
    }
//#endif
    /* SPR 20633 changes end */
    return ret_val;
}



/*****************************************************************************
 * Function Name  : x2ap_open_client_asso_start_timer
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function open Client Association and state wait for 
 *                  connect timer.
 *****************************************************************************/
x2ap_return_et  x2ap_open_client_asso_start_timer( 
        x2ap_peer_enb_context_t  *p_enb_cntxt_arr,
        x2ap_gb_context_t        *p_x2ap_gb_ctx)
{
    x2ap_return_et 		   ret_val = X2AP_FAILURE;
    x2ap_sctp_conn_info_t 	   connect_server_info;
    x2ap_sctp_conn_info_t 	   client_info;
    x2ap_sctp_assoc_conf_info_t   sctp_conf_info;
    S32                            client_fd = INVALID_SCTP_SOCKET;
    /*SPR_16533_START*/
#if 0	
#ifdef ARICENT_SCTP_IPR
    x2_sctp_sd_search_node_t  *p_tree_node = X2AP_P_NULL;
#endif
#endif
    /*SPR_16533_END*/
    X2AP_MEMSET(&connect_server_info, X2AP_NULL, sizeof(x2ap_sctp_conn_info_t));
    X2AP_MEMSET(&client_info, X2AP_NULL, sizeof(x2ap_sctp_conn_info_t));
    X2AP_MEMSET(&sctp_conf_info, X2AP_NULL, sizeof(x2ap_sctp_assoc_conf_info_t));

    /* Populate SCTP Configuration Parameters from Global DB */
    x2ap_popu_sctp_conf_info_from_gb_db( &(sctp_conf_info),
            &( p_x2ap_gb_ctx->config_param.sctp_conf_param));

    /* Pupulate Peer Server IP/Port Info*/
    connect_server_info.p_comm_info = &(p_enb_cntxt_arr->enb_comm_info);
    connect_server_info.p_sctp_assoc_conf_info = &(sctp_conf_info);

    client_info.p_comm_info = &(p_x2ap_gb_ctx->local_enb_info);
    /* Open Client Connection */
    /*SPR_16533_START*/
    /* SPR_14436_Fix Start */
#if 0
#ifdef ARICENT_SCTP_IPR
#ifdef LINUX_PC_TEST
    client_fd = x2ap_get_sock_fd_for_test(p_enb_cntxt_arr);
#else
    client_fd =  x2ap_open_sctp_client( &(connect_server_info), &(client_info), p_enb_cntxt_arr );
#endif
#endif
#endif
    /* SPR_14436_Fix Stop */
#ifdef LKSCTP
    /* SPR 20633 changes start */
    if(p_enb_cntxt_arr->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
    {
        client_fd = g_sctp_server_fd_ipv6; 
    }
    if(p_enb_cntxt_arr->enb_comm_info.bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT)
    {
        client_fd = g_sctp_server_fd_ipv4;
    }
    /* SPR 20633 changes end */
#endif
    X2AP_TRACE(X2AP_DETAILED,"Client_fd = %d \n",client_fd);
#ifdef LKSCTP
#ifdef LINUX_PC_TEST
    p_enb_cntxt_arr->sctp_sd = x2ap_get_sock_fd_for_test(p_enb_cntxt_arr);
    X2AP_TRACE(X2AP_INFO,"new_fd = %d \n",p_enb_cntxt_arr->sctp_sd);
#else
    p_enb_cntxt_arr->sctp_sd = client_fd;
#endif
    /*SPR_16533_DELTA2_START*/
    ret_val = x2ap_start_sctp_timer(X2AP_SCTP_CONNECT_TIMER, 
		    p_x2ap_gb_ctx->config_param.timer_conf_info.
		    sctp_connect_tim_val, &(p_enb_cntxt_arr->sctp_sm_ctx.
			    conn_timer_id), p_enb_cntxt_arr);

    X2AP_TRACE(X2AP_BRIEF,"[%s]Start timer "
		    "[X2AP_SCTP_CONNECT_TIMER]", __FUNCTION__);
    /*SPR_16533_DELTA2_END*/
#endif
    /*SPR_16533_END*/
    /*SPR_16533_START*/
#if 0
#ifdef ARICENT_SCTP_IPR
    /*SPR_16533_END*/

    if (INVALID_SCTP_SOCKET != client_fd)
    {
        p_enb_cntxt_arr->sctp_sd = client_fd;

        /* Create SD Tree Node */
        p_tree_node = (x2_sctp_sd_search_node_t*)x2ap_mem_get
            (sizeof(x2_sctp_sd_search_node_t));
        
        if (PNULL != p_tree_node)
        {
            p_tree_node->p_enb_context = p_enb_cntxt_arr;
            p_tree_node->sctp_sd = client_fd;

            ++(p_enb_cntxt_arr->sctp_sm_ctx.connect_ret_count);

            /* insert in sd_tree*/
            x2ap_st_insert_node(&(p_x2ap_gb_ctx->enb_ctxt_trees.
                        sd_search_tree), &(p_tree_node->nodeAnchor));

            X2AP_TRACE(X2AP_BRIEF,"[%s] insert sd[%d] to sd_tree ", __FUNCTION__, p_tree_node->sctp_sd);

            ret_val = x2ap_start_sctp_timer(X2AP_SCTP_CONNECT_TIMER, 
                    p_x2ap_gb_ctx->config_param.timer_conf_info.
                    sctp_connect_tim_val, &(p_enb_cntxt_arr->sctp_sm_ctx.
                        conn_timer_id), p_enb_cntxt_arr);

            X2AP_TRACE(X2AP_BRIEF,"[%s]Start timer "
                    "[X2AP_SCTP_CONNECT_TIMER]", __FUNCTION__);
        }
    }
    else
    {
        /* Bug 219 Fix Start */
        p_enb_cntxt_arr->sctp_sd = client_fd;
        ++(p_enb_cntxt_arr->sctp_sm_ctx.connect_ret_count);

        ret_val = x2ap_start_sctp_timer(X2AP_SCTP_CONNECT_TIMER, 
                    p_x2ap_gb_ctx->config_param.timer_conf_info.
                    sctp_connect_tim_val, &(p_enb_cntxt_arr->sctp_sm_ctx.
                        conn_timer_id), p_enb_cntxt_arr);

        X2AP_TRACE(X2AP_INFO,"Unable to open client socket. Start timer "
                    "[X2AP_SCTP_CONNECT_TIMER]");
        /* Bug 219 Fix Stop */
    }
    /*SPR_16533_START*/
#endif
#endif

    /*SPR_16533_END*/

    return ret_val;
}



/*****************************************************************************
 * Function Name  : x2ap_start_sctp_timer
 * Inputs         : timer_type    : Timer Type
 *                  timer_value   : Timer Value
 *                  p_timer_id - pointer to timer id 
 *                  p_enb_cntxt_arr: Peer ENB Context
 * Outputs        : p_timer_id : Timer Id
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function start Timer of timer_type with value timer 
 *                  value
 *****************************************************************************/
x2ap_return_et  x2ap_start_sctp_timer(
        x2ap_timer_id_en   timer_type,
        U32                timer_value,
        x2ap_timer_t      *p_timer_id,
        x2ap_peer_enb_context_t  *p_enb_cntxt_arr)
{
    x2ap_return_et ret_val = X2AP_FAILURE;
    x2ap_timer_buffer_t timer_buf;

    X2AP_MEMSET(&timer_buf, X2AP_NULL, sizeof(x2ap_timer_buffer_t));
    
    timer_buf.timer_type = timer_type;
    timer_buf.auto_delete = X2AP_TRUE;
    timer_buf.sd = p_enb_cntxt_arr->sctp_sd;
    
    /* Bug 219 Fix Start */
    if (timer_type == X2AP_SCTP_CONNECT_TIMER)
    {
        timer_buf.p_timer_specific_data = (U8*)x2ap_mem_get(sizeof(U8));
/* coverity_fix_7134611 start */
        if(PNULL == timer_buf.p_timer_specific_data)
        {
            return ret_val;
        }
        l3_memcpy_wrapper(timer_buf.p_timer_specific_data,&p_enb_cntxt_arr->peer_enodeb_id,sizeof(U8));
/* coverity_fix_7134611 stop */
    }
    /* Bug 219 Fix Stop */

    if(X2AP_P_NULL != p_timer_id)
    {
        *p_timer_id = x2ap_start_timer(
            timer_value, &timer_buf,
            sizeof(timer_buf), X2AP_FALSE);
    }

    if (X2AP_P_NULL != p_timer_id)
    {
        X2AP_TRACE(X2AP_INFO,"[%s] Start SCTP Related timer: %d Timer Value:%d",
                __FUNCTION__, timer_type, timer_value);
        ret_val = X2AP_SUCCESS;
    }
    return ret_val;
}

/*****************************************************************************
 * Function Name  : x2ap_sctp_sm_map_event
 * Inputs         : api: Api TO be mapped to event
 *                  p_event: State Machine event
 * Outputs        : p_event
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function api to state machien event 
 *****************************************************************************/
static  x2ap_return_et  x2ap_sctp_sm_map_event(
        U16 api,
        x2ap_sctp_event_et *p_event)
{
    x2ap_return_et ret_val = X2AP_SUCCESS;

    switch (api)
    {
        case X2AP_SCTP_COMM_UP_IND:
        {
            *p_event = X2AP_SCTP_COMM_UP_EV;
            break;
        }
        case X2AP_SCTP_CONN_FAILURE_IND:
        {
            *p_event = X2AP_SCTP_CONN_LOST_EV;
            break;
        }
        case X2AP_SCTP_CLOSED_IND:
        {
            *p_event = X2AP_SCTP_CLOSED_EV; 
            break;
        }
        case X2AP_SCTP_SHUT_DOWN_INIT_IND:
        {
            *p_event = X2AP_SCTP_SHUT_DOWN_EV;
            break;
        }
        case X2AP_ENB_MESSAGE:
        {
            *p_event = X2AP_SCTP_RCVD_DATA_EV;
            break;
        }
        case X2AP_SCTP_CONN_RESTART_IND:
        {
            *p_event = X2AP_SCTP_CONN_RSTRT_EV;
            break;
        }
        default:
        {
            X2AP_TRACE(X2AP_ERROR, "%s: Invalid API!", __FUNCTION__);
            ret_val = X2AP_FAILURE;
            break; 
        }
    }
    return ret_val;
}

/* klocwork Fix*/
/*****************************************************************************
 * Function Name  : x2ap_sctp_validate_event
 * Inputs         : api: maps event to event
 *                  p_event: State Machine event
 * Outputs        : p_event
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function api to state machien event 
 *****************************************************************************/
static  x2ap_return_et  x2ap_sctp_validate_event(
        U16 api,
        x2ap_sctp_event_et *p_event)
{
    x2ap_return_et ret_val = X2AP_SUCCESS;

    switch (api)
    {
        case X2AP_SCTP_CONN_CLOSE_EV:
        {
            *p_event = X2AP_SCTP_CONN_CLOSE_EV;
            break;
        }
        case X2AP_SCTP_CONNECT_EV:
        {
            *p_event = X2AP_SCTP_CONNECT_EV;
            break;
        }
        case X2AP_SCTP_SEND_DATA_EV:
        {
            *p_event = X2AP_SCTP_SEND_DATA_EV;
            break;
        }
        case X2AP_SCTP_CONNECT_TMR_EXP_EV:
        {
            *p_event = X2AP_SCTP_CONNECT_TMR_EXP_EV;
            break;        
        }
        case X2AP_SCTP_PENDING_CONN_EV:
        {
            *p_event = X2AP_SCTP_PENDING_CONN_EV;
            break;
        }
        case X2AP_SCTP_CLOSE_ALL_X2AP_SCTP_CONN_EV:
        {
            *p_event = X2AP_SCTP_CLOSE_ALL_X2AP_SCTP_CONN_EV;
            break;
        }
        default:
        {
            X2AP_TRACE(X2AP_ERROR, "%s: Invalid Event!", __FUNCTION__);
            ret_val = X2AP_FAILURE;
            break; 
        }
    }
    return ret_val;
}
/* klocwork Fix*/

    /*SPR_16533_START*/
        /* Code Removed */
    /*SPR_16533_END*/
/*SPR_16533_DELTA3_START*/
/*****************************************************************************
 * Function Name  : x2ap_sctp_act_conn
 * Inputs         : p_msg_ptr: Pointer to ENB Context specific to Sctp SM
 *                  p_x2ap_gb_ctx       : pointer to X2AP global context
 *                  p_enb_cntxt_arr     : peer ENB Context
 * Outputs        : None
 * Returns        : X2AP_FAILURE     - Indicates failed message processing
 *      X2AP_SUCCESS     - Indicates successful message processing
 * Description    : This function invoke X2 Setup SM on recieving CONNECT_EV in 
 *                  SCTP_ETABLISHED State
 *****************************************************************************/
x2ap_return_et  x2ap_sctp_act_conn( 
        U8                   *p_msg_ptr,
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
    x2ap_return_et ret_val = X2AP_FAILURE;

    p_enb_cntxt_arr->sctp_sm_ctx.connect_ret_count = 0;
    p_enb_cntxt_arr->op_status = ASSOCIATED;

    /* Invoke X2_SETUP SM with Event X2_SETUP_SCTP_X2_CONN_UP_EV */
    ret_val = x2setup_sm_entry(X2_SETUP_SCTP_X2_CONN_UP_EV, p_msg_ptr,
            p_x2ap_gb_ctx, p_enb_cntxt_arr);

    return ret_val;
}
/*SPR_16533_DELTA3_END*/


