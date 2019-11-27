/*******************************************************************************
 *  FILE NAME:  s_clpapi.c
 *
 *  DESCRIPTION:
 *  This file is part of client build of SCTP stack. Here we do
 *  the static parsing of API structure called by the client code.
 *
 *  DATE         NAME            REFERENCE       REASON
 *  -----------------------------------------------------
 *
 *  02June 2000  Sandeep Mahajan  -           .Original Creation
 *  13Sep  2001  Yogesh Gahlaut   -           Rel 3.0 Changes
 *  02jan02      hbhondwe        SCTP_IPC_TCP   ipc changes for tcp
 *  10Oct02      Manish Rajpal      SPR 4950    send iovec
 *  10Oct02      Manish Rajpal SPR 4953    Reuseaddr in Config Assoc
 *  30Oct02      Manish Rajpal SPR 5031    accpt response dump
 *  27Now03      Vivek Bansal              CSPL Changes
 *  11Jan04      Vivek Bansal              Changes for local IPC failure
 *                                         indication to the application.
 *  02Nov04  Svakumar     REL 6.0                SCTP/FUNC-CONGCON-5
 *  4Feb05       Amit Ray                  Rel 5.0.4
 *  1May13     Pranav Sharma   SPR 21107   CSR 44034
 *  1May13      Pranav          SPR 21127  CSR 40731-GCC 4.7 warning removal
 *  1July13     Rohan Pawa       SPR 21141       CSR 50452
 *  25Feb14     Rohan Pawa                       CSR 69326
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

/*  any restriction on send API, max buffer sise.. there should be .. more than
 *  4500 bytes not allowed
 */

#define  SCTP_MODULE "CL-SU"

#include <s_cotyps.h>
#include <s_closmem.h>
#include <s_osmem.h>
#include <s_clpapi.h>
#include <s_cobuf.h>
#include <s_cles.h>
#include <s_clpapi.h>

/******* *  forward declarations  *************/


sctp_return_t
sctp_chk_api_bind(
    sctp_Pvoid_t      p_elements,
    sctp_Error_t      *p_ecode );

sctp_return_t
sctp_chk_api_bindx(
    sctp_Pvoid_t      p_elements,
    sctp_Error_t      *p_ecode );

sctp_return_t
sctp_chk_api_config_assoc(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ); /* error code */

sctp_return_t
sctp_chk_api_set_fthresh(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ); /* error code */

sctp_return_t
sctp_chk_api_connect(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ); /* error code */

static sctp_return_t
sctp_chk_api_connect_ext(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ); /* error code */

static sctp_return_t
sctp_chk_api_send(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ); /* error code */

static sctp_return_t
sctp_chk_api_send_iovec(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ); /* error code */

static sctp_return_t
sctp_chk_api_hb_action(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ); /* error code */

static sctp_return_t
sctp_chk_api_set_primary(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ); /* error code */

static sctp_return_t
sctp_chk_api_get_srtt(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ); /* error code */

static sctp_U8bit *
sctp_build_api_register_su(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_deregister_su(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_socket(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_bind(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_listen(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_config_assoc(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_config_assoc_ctl(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_set_fthresh(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_connect(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_connect_ext(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_accept(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_accept_ext(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_send(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_send_ext(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_send_iovec(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ); /* pointer to api elements */

static sctp_U8bit *
sctp_build_api_receive(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_get_status(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_get_dest_status(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_receive_ext(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_receive_part(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_close(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_close_ext(
    sctp_U8bit    *p_buffer,   /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ); /* pointer to api elements */

static sctp_U8bit *
sctp_build_api_hb_action(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_set_primary(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_get_srtt(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_set_restart_option(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_ret_unsent(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_ret_unacked(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_flush_buffers(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_get_stream_status(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_abort_retrieve(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_bindx(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_config_cong_reg_def(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_get_cong_reg_def(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_config_txrx_num_frag(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_get_free_txrx_num_frag(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

static sctp_U8bit *
sctp_build_api_connectx(
    sctp_U8bit        *p_buffer,
    sctp_Pvoid_t      p_elements );

sctp_U8bit *
( *sctp_client_build_func_tbl[SCTP_NUM_SUREQUEST_API] )
( sctp_U8bit * , sctp_Pvoid_t )  =
{
    sctp_build_api_register_su,         /* register su */
    sctp_build_api_deregister_su,       /* de-register su */
    sctp_build_api_socket,              /* socket  */
    sctp_build_api_bind,                /* bind */
    sctp_build_api_listen,              /* listen */
    sctp_build_api_config_assoc,        /* config assoc */
    sctp_build_api_set_fthresh,         /* set failure threshold */
    sctp_build_api_connect,             /* connect  */
    sctp_build_api_connect_ext,         /* connect extended */
    sctp_build_api_accept,              /* accept */
    sctp_build_api_accept_ext,          /* accept extended */
    sctp_build_api_send,                /* send */
    sctp_build_api_send_ext,            /* send Extended*/
    sctp_build_api_receive,             /* receive */
    sctp_build_api_receive_ext,         /* receive Extended */
    sctp_build_api_receive_part,        /* receive partial data */
    sctp_build_api_close,               /* close */
    sctp_build_api_hb_action,           /* heartbeat action */
    sctp_build_api_set_primary,         /* set primary destination */
    sctp_build_api_get_srtt,            /* get srtt */
    sctp_build_api_get_status,          /* get status */
    sctp_build_api_get_dest_status,     /* get destination status */
    sctp_build_api_set_restart_option,  /* Set Restart option */
    sctp_build_api_ret_unsent,          /* Retrieve unsent*/
    sctp_build_api_ret_unacked,         /* Retrieve unacked*/
    sctp_build_api_flush_buffers,       /* Flush buffers*/
    sctp_build_api_config_assoc_ctl,    /* config assoc_ctl*/
    sctp_build_api_get_stream_status,   /* Get Stream Staus*/
    sctp_build_api_abort_retrieve,      /* Abort Retrieve*/
    sctp_build_api_send_iovec,          /* send iovec request*/
    sctp_build_api_bindx,                /* bindx request*/
    sctp_build_api_config_cong_reg_def,   /*config_cong_reg_def*/
    sctp_build_api_get_cong_reg_def,      /** get_cong_reg_def**/
    sctp_build_api_config_txrx_num_frag,  /** config_txrx_num_frag**/
    sctp_build_api_get_free_txrx_num_frag,  /** config_txrx_num_frag**/
    sctp_build_api_connectx,                 /*Connectx API*/
    sctp_build_api_close_ext                /* Close Extended */

};
/* API buffer building functions ***********************************************/
sctp_return_t
( *sctp_client_chk_func_tbl[SCTP_NUM_SUREQUEST_API] )
( sctp_Pvoid_t p_elements,   sctp_Error_t *p_ecode ) =
{
    NULL,                                   /* register su */
    NULL,                                   /* de-register su */
    NULL,                                   /* socket  */
    sctp_chk_api_bind,                      /* bind */
    NULL,                                   /* Listen */
    sctp_chk_api_config_assoc,              /* config assoc */
    sctp_chk_api_set_fthresh,               /* set failure threshold */
    sctp_chk_api_connect,                   /* connect  */
    sctp_chk_api_connect_ext,               /* connect extended */
    NULL,                                   /* accept */
    NULL,                                   /* accept extended */
    sctp_chk_api_send,                      /* send */
    NULL,                                   /* send Extended*/
    NULL,                                   /* receive */
    NULL,                                   /* receive Extended */
    NULL,                                   /* receive partial data */
    NULL,                                   /* close  */
    sctp_chk_api_hb_action,                 /* heartbeat action */
    sctp_chk_api_set_primary,               /* set primary destination */
    sctp_chk_api_get_srtt,                  /* get srtt */
    NULL,                                   /* get status */
    NULL,                                   /* get destination status */
    NULL,                                   /* Set Restart Option  */
    NULL,                                   /* Ret unsent*/
    NULL,                                   /* Ret unacked*/
    NULL,                                   /* Flush buffers*/
    NULL,                                   /* config assoc ctl*/
    NULL,                                   /* Get Stream Status */
    NULL,                                   /* Abort Retrieve */
    sctp_chk_api_send_iovec,                /* Send Iovec */
    sctp_chk_api_bindx
};
/*********** * functions for processing resp apis *****************/

sctp_void_t
sctp_su_process_stack_buffer(
    sctp_U8bit              *p_buffer,
    sctp_len_t              nooctets );

static sctp_return_t
sctp_su_proc_api_register_su_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_deregister_su_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_socket_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_bind_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_listen_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_config_assoc_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_config_assoc_ctl_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_set_fthresh_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_connect_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_connect_ext_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_accept_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_accept_ext_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_send_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_send_ext_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_receive_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_receive_ext_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_receive_part_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_close_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_hb_action_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_set_primary_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_get_srtt_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_get_status_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_get_dest_status_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_set_restart_option_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_ret_unsent_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_ret_unacked_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_ret_unsent_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_ret_unacked_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_flush_buffers_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_get_stream_status_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_abort_retrieve_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_bindx_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_config_cong_reg_def_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_get_cong_reg_def_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_config_txrx_num_frag_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_get_free_txrx_num_frag_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_api_connectx_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );


sctp_return_t
( *sctp_client_proc_resp_tbl[SCTP_NUM_SURESP_API] )
( sctp_U8bit * ,   sctp_len_t )    =
{
    sctp_su_proc_api_register_su_resp,
    sctp_su_proc_api_deregister_su_resp,
    sctp_su_proc_api_socket_resp,
    sctp_su_proc_api_bind_resp,
    sctp_su_proc_api_listen_resp,
    sctp_su_proc_api_config_assoc_resp,
    sctp_su_proc_api_set_fthresh_resp,
    sctp_su_proc_api_connect_resp,
    sctp_su_proc_api_connect_ext_resp,
    sctp_su_proc_api_accept_resp,
    sctp_su_proc_api_accept_ext_resp,
    sctp_su_proc_api_send_resp,
    sctp_su_proc_api_send_ext_resp,
    sctp_su_proc_api_receive_resp,
    sctp_su_proc_api_receive_ext_resp,
    sctp_su_proc_api_receive_part_resp,
    sctp_su_proc_api_close_resp,
    sctp_su_proc_api_hb_action_resp,
    sctp_su_proc_api_set_primary_resp,
    sctp_su_proc_api_get_srtt_resp,
    sctp_su_proc_api_get_status_resp,
    sctp_su_proc_api_get_dest_status_resp,
    sctp_su_proc_api_set_restart_option_resp,
    sctp_su_proc_api_ret_unsent_resp,
    sctp_su_proc_api_ret_unacked_resp,
    sctp_su_proc_api_flush_buffers_resp,
    sctp_su_proc_api_config_assoc_ctl_resp,
    sctp_su_proc_api_get_stream_status_resp,
    sctp_su_proc_api_abort_retrieve_resp,
    sctp_su_proc_api_bindx_resp,
    sctp_su_proc_api_config_cong_reg_def_resp,   /*config_cong_reg_def*/
    sctp_su_proc_api_get_cong_reg_def_resp,      /** get_cong_reg_def**/
    sctp_su_proc_api_config_txrx_num_frag_resp,  /** config_txrx_num_frag**/
    sctp_su_proc_api_get_free_txrx_num_frag_resp,  /** config_txrx_num_frag**/
    sctp_su_proc_api_connectx_resp               /*Connectx Response*/

};
/********************* functions to process notifications **************/
static sctp_return_t
sctp_su_proc_op_error_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_dest_status_chg_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_endpoint_status_chg_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_congestion_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_sys_error_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_conn_pending_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_data_pending_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_snd_failure_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_part_data_pending_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_recv_congestion_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_adaption_ind_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_peer_data_drop_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_shutdown_event_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );

static sctp_return_t
sctp_su_proc_lcl_ipc_error_ind_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len );


sctp_return_t
( *sctp_client_proc_notf_api_tbl[SCTP_NUM_SU_NOTIF_APIID] )
( sctp_U8bit * ,   sctp_len_t )    =
{
    sctp_su_proc_op_error_notf,
    NULL,
    sctp_su_proc_dest_status_chg_notf,
    sctp_su_proc_endpoint_status_chg_notf,
    sctp_su_proc_congestion_notf,
    NULL,
    sctp_su_proc_sys_error_notf,
    sctp_su_proc_conn_pending_notf,
    sctp_su_proc_data_pending_notf,
    sctp_su_proc_snd_failure_notf,
    sctp_su_proc_part_data_pending_notf,
    sctp_su_proc_recv_congestion_notf,
    sctp_su_proc_adaption_ind_notf,
    sctp_su_proc_peer_data_drop_notf,
    sctp_su_proc_shutdown_event_notf,
    sctp_su_proc_lcl_ipc_error_ind_notf
};


/***************************************************************************
 *  FUNCTION : send_to_sctp
 *
 *  DESCRIPTION:
 *  This function is the main API call function from client
 *  code and it does the possible static parsing on any API
 *  call and then calls the dispatch buffer to send it to
 *  the core stack.
 *  RETURN VALUE:
 *  SCTP_SUCCESS
 *  SCTP_FAILURE
 *
 *************************************************************************/
sctp_return_t
send_to_sctp(
    sctp_trans_id_t     transaction_id,
    sctp_api_id_t       api_id,
    sctp_src_id_t       src_id,
    sctp_dest_id_t      dest_id,
    sctp_Pvoid_t        p_elements,
    sctp_Error_t        *p_ecode )
{
    sctp_U16bit         index_api_tbl;
    sctp_return_t       ret;
    sctp_U32bit         noctets;
    sctp_U8bit          *p_su_buffer;
    sctp_U8bit          *p_buffer;

    /* check if API is valid  */
    if( ( api_id < ( SCTP_MIN_SUREQUST_APIID ) )
            || ( api_id > SCTP_MAX_SUREQUST_APIID ) )
    {
        *p_ecode = SCTP_INVALID_API_ID;
        return( SCTP_FAILURE );
    }

    /* Do I need to check for valid src and dest id */
    if( p_elements == SCTP_NULL )
    {
        *p_ecode = SCTP_INVALID_API_ID;
        return( SCTP_FAILURE );
    }

    index_api_tbl = api_id - SCTP_MIN_SUREQUST_APIID;

    if( sctp_client_chk_func_tbl[index_api_tbl] != NULL )
    {
        ret = ( *sctp_client_chk_func_tbl[index_api_tbl] )( p_elements,
                                                            p_ecode );

        if( ret  == SCTP_FAILURE )
        {
            return( SCTP_FAILURE );
        }
    }

    p_su_buffer = ( sctp_U8bit * )sctp_client_get_mesg_buffer( SCTP_MAX_API_SIZE );

    if( SCTP_NULL == p_su_buffer )
    {
        return SCTP_FAILURE;
    }

    p_buffer = p_su_buffer;
    __MEMSET( p_buffer, 0, SCTP_MAX_API_SIZE );

    /*
     * if a valid api buffer build api buffer and  send the buffer
     * to core stack
     */

    p_buffer = sctp_build_api_header( p_buffer, api_id, transaction_id,
                                      src_id, dest_id );

    p_buffer = ( *sctp_client_build_func_tbl[index_api_tbl] )( p_buffer,
                                                               p_elements );

    noctets = p_buffer - p_su_buffer;

    /* fill the api length field */
    p_buffer = p_su_buffer;

    sctp_buffer_set_UlongAtOffset( p_buffer, APIBUF_LENGTH_OFFSET,
                                   ( sctp_U32bit )noctets );

    /* set p_buffer to point to the first element for dispatch */
    p_buffer = p_su_buffer;

    ret = sctp_dispatch_buffer( p_buffer, noctets, src_id, dest_id, p_ecode );

    if( SCTP_FAILURE == ret )
    {
        sctp_client_free_mesg_buffer( &p_buffer );
    }

    return ( ret );
}

/***** * pasring functions may do only syntax checking *********************/

/***************************************************************************
 *  FUNCTION :  sctp_chk_api_bind
 *
 *  DESCRIPTION:
 *    This function performs static checks on parameter passed for bind  API.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_chk_api_bind(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ) /* error code */
{

    sctp_U32bit       no_of_su_addrs, hostname_len;

    sctp_msg_bind_st *p_bind;
    p_bind = ( sctp_msg_bind_st * ) p_elements;

    no_of_su_addrs =  p_bind->no_of_su_addrs;

    if( no_of_su_addrs > SCTP_MAX_TRANSPORT_ADDR )
    {
        *p_ecode = EINV_PARAMS;
        return( SCTP_FAILURE );
    }

    /* some static checking on IP address */
    hostname_len = strlen( ( const char * )p_bind->hostname );

    if( hostname_len == SCTP_MAX_HOSTNAME_LEN )
    {
        /* Non NULL terminated Hostname string  */
        *p_ecode = EINV_PARAMS;
        return( SCTP_FAILURE );
    }

    return( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :  sctp_chk_api_bindx
 *
 *  DESCRIPTION:
 *    This function performs static checks on parameter passed for bindx  API.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_chk_api_bindx(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ) /* error code */
{

    /* CSR 69326 fix starts */
    /* sctp_sockaddr_st  *p_addr_list;*/
    sctp_sockaddr_list_st  p_addr_list;
    /* CSR 69326 fix ends */
    sctp_U32bit       no_of_su_addrs, count;

    sctp_msg_bindx_st *p_bind;
    p_bind = ( sctp_msg_bindx_st * ) p_elements;

    no_of_su_addrs =  p_bind->no_of_su_addrs;

    if( ( no_of_su_addrs > SCTP_MAX_TRANSPORT_ADDR ) ||
            ( no_of_su_addrs == 0 ) )
    {
        *p_ecode = EINV_PARAMS;
        return( SCTP_FAILURE );
    }

    /* CSR 69326 fix starts */
    /* p_addr_list= ( sctp_sockaddr_st * )(p_bind->list_su_addrs);*/
    sigtran_memcpy( ( sigtran_pvoid_t )p_addr_list, ( sigtran_pvoid_t )p_bind->list_su_addrs, sizeof( sctp_sockaddr_list_st ) );
    /* CSR 69326 fix ends */

    for( count = 0; count < no_of_su_addrs; count++ )
    {
        if( SCTP_IS_INADDR_ANY( p_addr_list[count] ) != SCTP_FALSE )
        {
            *p_ecode = EINV_PARAMS;
            return( SCTP_FAILURE );
        }
    }

    return( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :  sctp_chk_api_config_assoc
 *
 *  DESCRIPTION:
 *    This function performs static checks on parameter passed in
 *    config socket API.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_chk_api_config_assoc(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ) /* error code */
{
    sctp_U16bit rto_alpha, rto_beta;
    sctp_U32bit rto_init, rto_min, rto_max;

    sctp_msg_config_assoc_st *p_config_assoc;
    p_config_assoc = ( sctp_msg_config_assoc_st * ) p_elements;


    /* static checks on RTO parameters,  */
    rto_init = p_config_assoc->protocol_parameters.rto_init;
    rto_min = p_config_assoc->protocol_parameters.rto_min;
    rto_max = p_config_assoc->protocol_parameters.rto_max;

    /*rto_max should be more than rto_min */
    if( ( rto_max < rto_min ) || ( rto_init < rto_min ) || ( rto_init > rto_max ) )
    {
        *p_ecode = EINV_PARAMS;
        return( SCTP_FAILURE );
    }

    /* static checks on rto_alpha and rto_beta parameters,  */

    rto_alpha = p_config_assoc->protocol_parameters.rto_alpha_percent;
    rto_beta = p_config_assoc->protocol_parameters.rto_beta_percent;
    /* ?? alpha & beta should be between 0-100, alpha should be less than beta*/

    if( ( rto_alpha > SCP_MAX_PERCENTAGE_VALUE )
            || ( rto_beta > SCP_MAX_PERCENTAGE_VALUE )
            || ( rto_alpha > rto_beta ) )
    {
        *p_ecode = EINV_PARAMS;
        return( SCTP_FAILURE );
    }

    /* static checks on retrans parameters   */
    if( p_config_assoc->protocol_parameters.path_max_retrans >
            p_config_assoc->protocol_parameters.assoc_max_retrans )
    {
        *p_ecode = EINV_PARAMS;
        return( SCTP_FAILURE );
    }

    return( SCTP_SUCCESS );

}

/***************************************************************************
 *  FUNCTION : sctp_chk_api_set_fthresh
 *
 *  DESCRIPTION:
 *    This function performs static checks on parameter passed in
 *    set failire threshold API.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_chk_api_set_fthresh(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ) /* error code */
{


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_elements );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/
    return( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :  sctp_chk_api_connect
 *
 *  DESCRIPTION:
 *    This function performs static checks on parameter passed in
 *    conect API.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_chk_api_connect(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ) /* error code */
{

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_elements );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/
    return( SCTP_SUCCESS );

}


/***************************************************************************
 *  FUNCTION :  sctp_chk_api_connect_ext
 *
 *  DESCRIPTION:
 *    This function performs static checks on parameter passed in
 *    conect extended API.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *************************************************************************/
static sctp_return_t
sctp_chk_api_connect_ext(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ) /* error code */
{

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_elements );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/
    return( SCTP_SUCCESS );

}

/***************************************************************************
 *  FUNCTION :  sctp_chk_api_send
 *
 *  DESCRIPTION:
 *    This function performs static checks on parameter passed in
 *    send API.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *************************************************************************/
static sctp_return_t
sctp_chk_api_send(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ) /* error code */
{

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_elements );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/

    return( SCTP_SUCCESS );

}

/***************************************************************************
 *  FUNCTION :  sctp_chk_api_send_ext
 *
 *  DESCRIPTION:
 *    This function performs static checks on parameter passed in
 *    send Extended API.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *************************************************************************/
static sctp_return_t
sctp_chk_api_send_iovec(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ) /* error code */
{
    sctp_msg_send_iovec_st  *p_msg = ( sctp_msg_send_iovec_st * )p_elements;
    sctp_U32bit count;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


    for( count = 0; count < p_msg->iovec_size; count++ )
    {
        if( ( p_msg->p_iovec[count].iov_len ) &&
                ( !p_msg->p_iovec[count].iov_base ) )
        {
            return SCTP_FAILURE;
        }
    }

    return( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :  sctp_chk_api_hb_action
 *
 *  DESCRIPTION:
 *    This function performs static checks on parameter passed in
 *  heartbeat action API.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
**************************************************************************/
static sctp_return_t
sctp_chk_api_hb_action(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ) /* error code */
{

    sctp_U32bit hb_action;

    sctp_msg_hb_action_st *p_hb_action =
        ( sctp_msg_hb_action_st * )p_elements;
    hb_action = ( sctp_U32bit ) p_hb_action->heartbeat_action;

    if( ( hb_action != SCTP_HB_ENABLE )
            && ( hb_action != SCTP_HB_DISABLE )
            && ( hb_action != SCTP_HB_REQUEST ) )
    {
        /* Heartbeat action requeted not valid  */
        *p_ecode = EINV_PARAMS;
        return( SCTP_FAILURE );
    }

    return( SCTP_SUCCESS );
}



/***************************************************************************
 *  FUNCTION :  sctp_chk_api_set_primary
 *
 *  DESCRIPTION:
 *    This function performs static checks on parameter passed in
 *    set primary API.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *************************************************************************/
static sctp_return_t
sctp_chk_api_set_primary(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ) /* error code */
{

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_elements );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/
    return( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :  sctp_chk_api_get_srtt
 *
 *  DESCRIPTION:
 *    This function performs static checks on parameter passed in
 *    get srtt API.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *************************************************************************/
static sctp_return_t
sctp_chk_api_get_srtt(
    sctp_Pvoid_t      p_elements, /* api elements */
    sctp_Error_t      *p_ecode ) /* error code */
{

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_elements );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/
    return( SCTP_SUCCESS );

}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_register_su
 *
 *  DESCRIPTION:
 *   This function build API header to be sent across to stack code
 *   API elements are passed to this function and it convert them into
 *   buffer format.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_register_su(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED
    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_register_su_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_U16bit       su_id;

    sctp_msg_register_su_st *p_register_su =
        ( sctp_msg_register_su_st * )p_elements;

    su_id = p_register_su->su_id;
    p_buffer = sctp_buffer_set_Ushort( p_buffer, su_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_deregister_su
 *
 *  DESCRIPTION:
 *    This function builds Deregister API header to be sent across to
 *     stack code. API elements are passed to this function and it
 *     convert them into buffer format.
 *  RETURN VALUE:
 *    SCTP_U8bit *
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_deregister_su(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_deregister_su_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_U16bit appl_id;
    sctp_msg_deregister_su_st *p_deregister_su =
        ( sctp_msg_deregister_su_st * )p_elements;
    appl_id = p_deregister_su->appl_id;

    p_buffer = sctp_buffer_set_Ushort( p_buffer, appl_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_socket
 *
 *  DESCRIPTION:
 *    This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *    buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_socket(
    sctp_U8bit    *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t  p_elements )  /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_socket_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_U16bit appl_id;
    sctp_U32bit addr_family;
    sctp_msg_socket_st *p_socket = ( sctp_msg_socket_st * )p_elements;

    appl_id = p_socket->appl_id;
    p_buffer = sctp_buffer_set_Ushort( p_buffer, appl_id );
    addr_family = p_socket->addr_family;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, addr_family );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_bind
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_bind(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_bind_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_U32bit count;
    sctp_sockaddr_st *p_addr_list;
    sctp_U16bit hostname_len;

    sctp_msg_bind_st *p_bind = ( sctp_msg_bind_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_bind->association_id );
    /* sctp  port */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, ( sctp_U16bit )p_bind->sctp_port );
    /* number of su address */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      ( sctp_U32bit )p_bind->no_of_su_addrs );

    p_addr_list = ( sctp_sockaddr_st * )( p_bind->list_su_addrs );
    hostname_len = strlen( ( const char * )p_bind->hostname );

    for( count = 0; count < p_bind->no_of_su_addrs; count++ )
    {
        /* Address */
        p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                              &p_addr_list[count] );
    }

    for( count = 0; count < hostname_len; count++ )
    {
        p_buffer = sctp_buffer_set_Uchar( p_buffer, p_bind->hostname[count] );
    }

    p_buffer = sctp_buffer_set_Uchar( p_buffer, '\0' );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}


/***************************************************************************
 *  FUNCTION :  sctp_build_api_bindx
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_bindx(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_bindx_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_U32bit count;
    sctp_sockaddr_st *p_addr_list;

    sctp_msg_bindx_st *p_bindx = ( sctp_msg_bindx_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_bindx->association_id );
    /* sctp  port */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, ( sctp_U16bit )p_bindx->flags );
    /* number of su address */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      ( sctp_U32bit )p_bindx->no_of_su_addrs );

    p_addr_list = ( sctp_sockaddr_st * )( p_bindx->list_su_addrs );

    for( count = 0; count < p_bindx->no_of_su_addrs; count++ )
    {
        /* Address */
        p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                              &p_addr_list[count] );
    }

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_listen
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_listen(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_listen_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_listen_st *p_listen = ( sctp_msg_listen_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_listen->association_id );
    /* Back Log */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_listen->back_log );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_config_assoc
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *    buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_config_assoc(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_config_assoc_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_U16bit tmp_U16bit;
    sctp_U32bit tmp_U32bit;
    sctp_U8bit  tmp_U8bit;

    sctp_msg_config_assoc_st *p_config_assoc =
        ( sctp_msg_config_assoc_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_assoc->association_id );

    /* Protocol Parameters */

    /*SPR 4953*/
    /* rto_init */
    tmp_U32bit = ( sctp_U32bit ) p_config_assoc->protocol_parameters.rto_init;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* rto_min */
    tmp_U32bit = ( sctp_U32bit ) p_config_assoc->protocol_parameters.rto_min;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* rto_max */
    tmp_U32bit = ( sctp_U32bit ) p_config_assoc->protocol_parameters.rto_max;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* max_burst */
    tmp_U32bit = ( sctp_U32bit ) p_config_assoc->protocol_parameters.max_burst;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* valid cookie life */
    tmp_U32bit =
        ( sctp_U32bit ) p_config_assoc->protocol_parameters.valid_cookie_life;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* assoc_max_retrans */
    tmp_U32bit =
        ( sctp_U32bit ) p_config_assoc->protocol_parameters.assoc_max_retrans;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /*hb_interval*/
    tmp_U32bit = ( sctp_U32bit ) p_config_assoc->protocol_parameters.hb_interval;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* path_max_retrans */
    tmp_U32bit =
        ( sctp_U32bit ) p_config_assoc->protocol_parameters.path_max_retrans;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* max_init_retrans */
    tmp_U32bit =
        ( sctp_U32bit ) p_config_assoc->protocol_parameters.max_init_retrans;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* max_init_path_retrans */
    tmp_U32bit =
        ( sctp_U32bit ) p_config_assoc->protocol_parameters.max_init_path_retrans;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* bundling time */
    tmp_U32bit =
        ( sctp_U32bit ) p_config_assoc->protocol_parameters.bundling_time;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* sack_period time */
    tmp_U32bit =
        ( sctp_U32bit ) p_config_assoc->protocol_parameters.sack_period;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* sack_frequency */
    tmp_U32bit =
        ( sctp_U32bit ) p_config_assoc->protocol_parameters.sack_frequency;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* pmtu capable*/
    tmp_U32bit =
        ( sctp_U32bit ) p_config_assoc->protocol_parameters.pmtu_capable;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* mtu_size */
    tmp_U16bit =
        ( sctp_U16bit ) p_config_assoc->protocol_parameters.mtu_size;
    p_buffer = sctp_buffer_set_Ushort( p_buffer, tmp_U16bit );


    /* adaption_ind */
    tmp_U32bit =
        ( sctp_U32bit ) p_config_assoc->protocol_parameters.adaption_ind;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* hb_info_length  */
    tmp_U16bit =
        ( sctp_U16bit ) p_config_assoc->protocol_parameters.hb_info_length;
    p_buffer = sctp_buffer_set_Ushort( p_buffer, tmp_U16bit );

    /* rto_alpha_percent */
    tmp_U16bit =
        ( sctp_U16bit ) p_config_assoc->protocol_parameters.rto_alpha_percent;
    p_buffer = sctp_buffer_set_Ushort( p_buffer, tmp_U16bit );

    /* rto_beta_percent */
    tmp_U16bit =
        ( sctp_U16bit ) p_config_assoc->protocol_parameters.rto_beta_percent;
    p_buffer = sctp_buffer_set_Ushort( p_buffer, tmp_U16bit );

    /* num_in_streams */
    tmp_U16bit =
        ( sctp_U16bit ) p_config_assoc->protocol_parameters.num_in_streams;
    p_buffer = sctp_buffer_set_Ushort( p_buffer, tmp_U16bit );

    /* num_out_streams */
    tmp_U16bit =
        ( sctp_U16bit ) p_config_assoc->protocol_parameters.num_out_streams;
    p_buffer = sctp_buffer_set_Ushort( p_buffer, tmp_U16bit );

    /* Reuse Addr */
    tmp_U8bit =
        ( sctp_U8bit ) p_config_assoc->protocol_parameters.re_use_addr;
    p_buffer = sctp_buffer_set_Uchar( p_buffer, tmp_U8bit );

    /* high_congestion_level */
    tmp_U8bit =
        ( sctp_U8bit ) p_config_assoc->protocol_parameters.dont_fragment;
    p_buffer = sctp_buffer_set_Uchar( p_buffer, tmp_U8bit );

#ifdef SCTP_IP_HEADER_PARAMS
    tmp_U8bit =
        ( sctp_U8bit ) p_config_assoc->protocol_parameters.ip_header_params.ttl;
    p_buffer = sctp_buffer_set_Uchar( p_buffer, tmp_U8bit );

    tmp_U8bit =
        ( sctp_U8bit ) p_config_assoc->protocol_parameters.ip_header_params.tos;
    p_buffer = sctp_buffer_set_Uchar( p_buffer, tmp_U8bit );
#ifdef SCTP_INET6_SUPPORT
    tmp_U8bit =
        ( sctp_U8bit ) p_config_assoc->protocol_parameters.ip_header_params.hoplimit;
    p_buffer = sctp_buffer_set_Uchar( p_buffer, tmp_U8bit );

    tmp_U8bit =
        ( sctp_U8bit ) p_config_assoc->protocol_parameters.ip_header_params.tclass;
    p_buffer = sctp_buffer_set_Uchar( p_buffer, tmp_U8bit );
#endif
#endif

    /*SPR 4953*/

    /* Payload Protocol Identifier */
    tmp_U32bit =
        ( sctp_U32bit ) p_config_assoc->protocol_parameters.ppi;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* HB.Max.Burst*/
    tmp_U32bit =
        ( sctp_U32bit ) p_config_assoc->protocol_parameters.hb_max_burst;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}
/***************************************************************************
 *  FUNCTION :  sctp_build_api_config_assoc_ctl
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
**
**************************************************************************/
static sctp_U8bit *
sctp_build_api_config_assoc_ctl(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_config_assoc_ctl_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_U32bit tmp_U32bit;

    sctp_msg_config_assoc_ctl_st *p_config_assoc_ctl =
        ( sctp_msg_config_assoc_ctl_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_config_assoc_ctl->association_id );

    /* Protocol Parameter*/
    tmp_U32bit = ( sctp_U32bit ) p_config_assoc_ctl->prot_param;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* parameter value */
    tmp_U32bit = ( sctp_U32bit ) p_config_assoc_ctl->param_value;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_set_fthresh
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_set_fthresh(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_set_fthresh_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_set_fthresh_st *p_set_fthresh =
        ( sctp_msg_set_fthresh_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_set_fthresh->association_id );

    /* address */
    p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                          &p_set_fthresh->dest_address );

    /* path_max_retrans */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_set_fthresh->path_max_retrans );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_connect
 *
 *  DESCRIPTION:
 *    This function build API header to be sent across to stack code
 *    API elements are passed to this function and it convert them into
 *    buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_build_api_connect(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_connect_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_connect_st *p_connect = ( sctp_msg_connect_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_connect->association_id );

    /* destination sctp  port */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_connect->dest_sctp_port );

    /* Address */
    p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                          &p_connect->connect_dest_address );

    /* Payload prot Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_connect->payload_prot_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}


/***************************************************************************
 *  FUNCTION :  sctp_build_api_connect_ext
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *     pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_connect_ext(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_connect_ext_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_connect_ext_st *p_connect_ext =
        ( sctp_msg_connect_ext_st * )p_elements;
    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_connect_ext->association_id );

    /* destination sctp  port */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_connect_ext->dest_sctp_port );

    /* num_out_streams */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_connect_ext->num_out_streams );

    /* num_in_streams */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_connect_ext->num_in_streams );

    /* address  */
    p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                          &p_connect_ext->connect_dest_address );

    /* Payload prot Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_connect_ext->payload_prot_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_accept
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_accept(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_accept_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_accept_st *p_accept =
        ( sctp_msg_accept_st * )p_elements;
    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_accept->association_id );

    /* Payload prot Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_accept->payload_prot_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_accept_ext
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *     pointer to buffer
**
**************************************************************************/
static sctp_U8bit *
sctp_build_api_accept_ext(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_accept_ext_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_accept_ext_st *p_accept_ext =
        ( sctp_msg_accept_ext_st * )p_elements;
    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_accept_ext->association_id );

    /* Payload prot Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_accept_ext->payload_prot_id );

    /* num_out_streams */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_accept_ext->num_out_streams );

    /* num_in_streams */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_accept_ext->num_in_streams );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_send
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_send(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

    sctp_msg_send_st  *p_send;
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_send_st ) - sizeof( sctp_U8bit * ),
                                       ( sctp_U8bit * )p_elements );

    p_send = ( sctp_msg_send_st * )p_elements;

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       p_send->data_size, p_send->data_payload );

#else

    sctp_U32bit         count;

    p_send = ( sctp_msg_send_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_send->association_id );

    /* context */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_send->context );

    /* stream Id */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_send->stream_id );

    /* Address  */
    p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                          &p_send->dest_address );

    /* Data size */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_send->data_size );

    for( count = 0; count < p_send->data_size; count++ )
    {
        p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                          ( sctp_U8bit ) * ( p_send->data_payload + count ) );
    }

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_send_ext
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_send_ext(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

    sctp_msg_send_ext_st  *p_send_ext;
    sctp_iovec_st         p_iovec[1];

    p_send_ext = ( sctp_msg_send_ext_st * )p_elements;
    p_iovec->iov_len  = p_send_ext->data_size;
    p_iovec->iov_base = ( char * )p_send_ext->data_payload;

    p_send_ext->data_size = 1;
    p_send_ext->data_payload = ( sctp_U8bit * )p_iovec;

    p_buffer = sctp_build_api_send_iovec( p_buffer, p_elements );

    /*restore structure*/
    p_send_ext->data_size    = p_iovec->iov_len;
    p_send_ext->data_payload = ( sctp_U8bit * )p_iovec->iov_base;

    return p_buffer;
}



/***************************************************************************
 *  FUNCTION :  sctp_build_api_send_iovec
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.This function send sctp_send_ext request to the stack
 *     cause data is already being packed in single buffer
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_send_iovec(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{
    sctp_msg_send_iovec_st *p_send_iovec;
    sctp_iovec_st          *p_iovec;
    sctp_U32bit             count = 0, data_size = 0;
    sctp_U8bit             *p_start = p_buffer;

#ifdef SCTP_STRUCT_BASED
    /*first pack the buffer */
    p_buffer = p_buffer + sizeof( sctp_msg_send_ext_st ) - sizeof( sctp_U8bit * );

    p_send_iovec = ( sctp_msg_send_iovec_st * )p_elements;
    p_iovec = p_send_iovec->p_iovec;

    for( count = 0; count < p_send_iovec->iovec_size; count++ )
    {
        if( p_iovec[count].iov_len && p_iovec[count].iov_base )
        {
            p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                               p_iovec[count].iov_len, ( sctp_U8bit * )p_iovec[count].iov_base );
            data_size += p_iovec[count].iov_len;
        }
    }

    /*this maps to data size in send ext*/
    p_send_iovec->iovec_size = data_size;

    sctp_buffer_set_Buffer( p_start,
                            sizeof( sctp_msg_send_ext_st ) - sizeof( sctp_U8bit * ),
                            p_elements );
#else

    sctp_U32bit tmp_U32bit;
    sctp_U8bit  tmp_U8bit;

    p_send_iovec = ( sctp_msg_send_iovec_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_send_iovec->association_id );

    /* Data size */
    /* this is being updated after packing data, make sure its at
     * same offset as below, currently 4*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer, 0 );

    /* context */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_send_iovec->context );

    /* Address  */
    p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                          &p_send_iovec->dest_address );


    /* life time */
    tmp_U32bit = ( sctp_U32bit ) p_send_iovec->life_time;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

    /* order flag */
    tmp_U8bit = ( sctp_U8bit ) p_send_iovec->order_flag;
    p_buffer = sctp_buffer_set_Uchar( p_buffer, tmp_U8bit );

    /* bundle flag */
    tmp_U8bit = ( sctp_U8bit ) p_send_iovec->bundle_flag;
    p_buffer = sctp_buffer_set_Uchar( p_buffer, tmp_U8bit );

    /* stream Id */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_send_iovec->stream_id );

    /* Payload prot Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      ( sctp_U32bit )p_send_iovec->payload_prot_id );

    /* Data */
    p_iovec = p_send_iovec->p_iovec;

    for( count = 0; count < p_send_iovec->iovec_size; count++ )
    {
        if( p_iovec[count].iov_len && p_iovec[count].iov_base )
        {
            p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                               p_iovec[count].iov_len, ( sctp_U8bit * )p_iovec[count].iov_base );
            data_size += p_iovec[count].iov_len;
        }
    }

    /* now set the data size*/
    sctp_buffer_set_UlongAtOffset( p_start, 4, data_size );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :    sctp_build_api_receive
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_receive(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_receive_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_receive_st *p_receive =
        ( sctp_msg_receive_st * )p_elements;
    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_receive->association_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}


/***************************************************************************
 *  FUNCTION :  sctp_build_api_get_status
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_get_status(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_get_status_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_get_status_st *p_get_status   =
        ( sctp_msg_get_status_st * )p_elements;
    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_get_status->association_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_get_dest_status
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_get_dest_status(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_get_deststatus_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_get_deststatus_st *p_get_dest_status  =
        ( sctp_msg_get_deststatus_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_get_dest_status->association_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}


/***************************************************************************
 *  FUNCTION :  sctp_build_api_receive_ext
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_receive_ext(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_receive_ext_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_receive_ext_st *p_receive_ext =
        ( sctp_msg_receive_ext_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_receive_ext->association_id );

    /* peek flag */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_receive_ext->peek_flag );

    /* stream Id */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_receive_ext->stream_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_receive_part
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_receive_part(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_receive_part_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_receive_part_st *p_receive_part =
        ( sctp_msg_receive_part_st * )p_elements;
    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_receive_part->association_id );

    /* peek flag */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_receive_part->peek_flag );

    /* stream Id */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_receive_part->stream_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_close
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_close(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_close_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_U16bit tmp_U16bit;

    sctp_msg_close_st *p_close = ( sctp_msg_close_st * )p_elements;
    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_close->association_id );

    /* order flag */
    tmp_U16bit = ( sctp_U16bit ) p_close->graceful;
    p_buffer = sctp_buffer_set_Ushort( p_buffer, tmp_U16bit );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_close_ext
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_close_ext(
    sctp_U8bit    *p_buffer,  /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED
    sctp_msg_close_ext_st *p_close = ( sctp_msg_close_ext_st * )p_elements;
    int i;
    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_close_ext_st ), ( sctp_U8bit * )p_elements );

    for( i = 0; i < p_close->cause_len; i++ )
    {
        p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                          ( sctp_U8bit ) * p_close->cause );
        p_close->cause++;
    }

#else

    sctp_msg_close_ext_st *p_close = ( sctp_msg_close_ext_st * )p_elements;
    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_close->association_id );

    /* cause_length */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_close->cause_len );

    /* Data */
    p_buffer = sctp_buffer_set_Buffer( p_buffer, p_close->cause_len,
                                       p_close->cause );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *
 *  FUNCTION :  sctp_build_api_hb_action
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_hb_action(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_hb_action_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_U32bit tmp_U32bit;
    sctp_U8bit tmp_U8bit;
    sctp_msg_hb_action_st *p_hb_action = ( sctp_msg_hb_action_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_hb_action->association_id );

    /* Address  */
    p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                          &p_hb_action->dest_address );

    /* hearbeat_action */
    tmp_U8bit = ( sctp_U8bit ) p_hb_action->heartbeat_action;
    p_buffer = sctp_buffer_set_Uchar( p_buffer, tmp_U8bit );

    /* heartbeat_interval */
    tmp_U32bit = ( sctp_U32bit ) p_hb_action->heartbeat_interval;
    p_buffer = sctp_buffer_set_Ulong( p_buffer, tmp_U32bit );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_set_primary
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_set_primary(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_set_primary_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_set_primary_st *p_set_primary =
        ( sctp_msg_set_primary_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_set_primary->association_id );

    /* Address  */
    p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                          &p_set_primary->dest_address );

    /* Address  */
    p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                          &p_set_primary->src_address );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_get_srtt
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_get_srtt(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_get_srtt_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_get_srtt_st *p_get_srtt = ( sctp_msg_get_srtt_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_get_srtt->association_id );

    /* Address  */
    p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                          &p_get_srtt->dest_address );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_set_restart_option
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_set_restart_option(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_set_restart_option_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_set_restart_option_st *p_restart_option =
        ( sctp_msg_set_restart_option_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_restart_option->association_id );

    /* restart option */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_restart_option->restart_option );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}


/***************************************************************************
 *  FUNCTION :   sctp_build_api_ret_unsent
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_ret_unsent(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_ret_unsent_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_ret_unsent_st *p_ret_unsent = ( sctp_msg_ret_unsent_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_ret_unsent->association_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_ret_unacked
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_ret_unacked(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_ret_unacked_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_ret_unacked_st *p_ret_unacked =
        ( sctp_msg_ret_unacked_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_ret_unacked->association_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_flush_buffers
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_flush_buffers(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_flush_buffers_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_flush_buffers_st *p_flush_buffers =
        ( sctp_msg_flush_buffers_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_flush_buffers->association_id );
#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_stream_status
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_get_stream_status(
    sctp_U8bit    *p_buffer,      /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_get_stream_status_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_get_stream_status_st *p_get_stream_status =
        ( sctp_msg_get_stream_status_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_get_stream_status->association_id );
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_get_stream_status->stream_id );
#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_abort_retrieve
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_abort_retrieve(
    sctp_U8bit    *p_buffer,  /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_abort_retrieve_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_abort_retrieve_st *p_abort_ret =
        ( sctp_msg_abort_retrieve_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_abort_ret->association_id );
#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_config_cong_reg_def
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_config_cong_reg_def(
    sctp_U8bit    *p_buffer,  /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_config_cong_reg_def_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_config_cong_reg_def_st *p_config_cong_req =
        ( sctp_msg_config_cong_reg_def_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_cong_req->association_id );
    p_buffer = sctp_buffer_set_Uchar( p_buffer, p_config_cong_req->cong_reg_def.no_cong_lvl_abate );
    p_buffer = sctp_buffer_set_Uchar( p_buffer, p_config_cong_req->cong_reg_def.no_cong_lvl_onset );
    p_buffer = sctp_buffer_set_Uchar( p_buffer, p_config_cong_req->cong_reg_def.low_cong_lvl_abate );
    p_buffer = sctp_buffer_set_Uchar( p_buffer, p_config_cong_req->cong_reg_def.low_cong_lvl_onset );
    p_buffer = sctp_buffer_set_Uchar( p_buffer, p_config_cong_req->cong_reg_def.med_cong_lvl_abate );
    p_buffer = sctp_buffer_set_Uchar( p_buffer, p_config_cong_req->cong_reg_def.med_cong_lvl_onset );
    p_buffer = sctp_buffer_set_Uchar( p_buffer, p_config_cong_req->cong_reg_def.high_cong_lvl_abate );
    p_buffer = sctp_buffer_set_Uchar( p_buffer, p_config_cong_req->cong_reg_def.high_cong_lvl_onset );

    p_buffer = sctp_buffer_set_Uchar( p_buffer, ( sctp_U8bit )p_config_cong_req->is_transmit );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}


/***************************************************************************
 *  FUNCTION :   sctp_build_api_get_cong_reg_def
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_get_cong_reg_def(
    sctp_U8bit    *p_buffer,  /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_get_cong_reg_def_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_get_cong_reg_def_st *p_config_cong_req =
        ( sctp_msg_get_cong_reg_def_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_cong_req->association_id );
    p_buffer = sctp_buffer_set_Uchar( p_buffer, ( sctp_U8bit )p_config_cong_req->is_transmit );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_config_txrx_num_frag
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_config_txrx_num_frag(
    sctp_U8bit    *p_buffer,  /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_config_txrx_num_frag_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_config_txrx_num_frag_st *p_config_req =
        ( sctp_msg_config_txrx_num_frag_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_req->association_id );
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_req->count_frags.min_num_frag );
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_req->count_frags.med_num_frag );
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_req->count_frags.max_num_frag );
    p_buffer = sctp_buffer_set_Uchar( p_buffer, ( sctp_U8bit )p_config_req->is_transmit );
    p_buffer = sctp_buffer_set_Uchar( p_buffer, ( sctp_U8bit )p_config_req->is_increment );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :   sctp_build_api_get_free_txrx_num_frag
 *
 *  DESCRIPTION:
 *     This function build API header to be sent across to stack code
 *     API elements are passed to this function and it convert them into
 *     buffer format.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_get_free_txrx_num_frag(
    sctp_U8bit    *p_buffer,  /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_get_free_tr_num_frag_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_get_free_tr_num_frag_st *p_get_txrx_req =
        ( sctp_msg_get_free_tr_num_frag_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_get_txrx_req->association_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}


static sctp_U8bit *
sctp_build_api_connectx(
    sctp_U8bit    *p_buffer,  /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_connectx_st ), ( sctp_U8bit * )p_elements );
#else
    sctp_U32bit count;
    sctp_sockaddr_st *p_addr_list;

    sctp_msg_connectx_st *p_connectx = ( sctp_msg_connectx_st * )p_elements;

    /*Assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_connectx->association_id );

    /*Destination sctp  port */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_connectx->dest_sctp_port );

    /*Number of Dest Addrs*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      ( sctp_U32bit )p_connectx->num_dest_addrs );

    /*List of Addrs*/
    p_addr_list = ( sctp_sockaddr_st * )( p_connectx->connect_dest_address );

    for( count = 0; count < p_connectx->num_dest_addrs; count++ )
    {
        /* Address */
        p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                              &p_addr_list[count] );
    }

    /*Blocking*/
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       ( sctp_U16bit )p_connectx->block );

    /*Payload protocol id*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_connectx->payload_prot_id );

    /*Out Streams*/
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_connectx->p_num_ostreams );

    /*In streams*/
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_connectx->p_num_istreams );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}
/*
 * Following are the functions to process apis recvd from the stack
 */

/***************************************************************************
 *  FUNCTION :  sctp_su_process_stack_buffer
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
sctp_void_t
sctp_su_process_stack_buffer(
    sctp_U8bit   *p_buffer,
    sctp_len_t   nooctets )
{
    sctp_api_id_t api_id;
    sctp_U16bit index_api_id;
    sctp_U16bit req_len;

    /*
     * check if length > header
     */
    if( nooctets < SCTP_API_HEADER_LEN )
    {
        /* report error */
        return;
    }

    /* get the header fields */
    api_id = sctp_buffer_get_UshortAtOffset( p_buffer, 0 );
    req_len = ( sctp_U16bit )sctp_buffer_get_UlongAtOffset( p_buffer, 12 );

    SCTP_CLIENT_TRACE( ( "Got API resp ID : %d", api_id ) );

    if( ( api_id >= SCTP_MIN_SURESP_APIID ) && ( api_id <= SCTP_MAX_SURESP_APIID ) )
    {
        /*api is resp api */
        index_api_id = api_id - SCTP_MIN_SURESP_APIID;
        ( *sctp_client_proc_resp_tbl[index_api_id] )( p_buffer, req_len );
    }

    else
    {
        if( ( api_id >= SCTP_MIN_SU_NOTIF_APIID )
                && ( api_id <= SCTP_MAX_SU_NOTIF_APIID ) )
        {
            /* api is notf api */
            index_api_id = api_id - SCTP_MIN_SU_NOTIF_APIID;

            if( sctp_client_proc_notf_api_tbl[index_api_id] != SCTP_NULL )
            {
                ( *sctp_client_proc_notf_api_tbl[index_api_id] )( p_buffer,
                                                                  req_len );
            }
        }
    }

    return ;
}

/***************************************************************************
 *  FUNCTION : sctp_su_proc_api_register_su_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_register_su_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t     ret = SCTP_SUCCESS;
    sctp_dest_id_t    dest_id;
    sctp_src_id_t     src_id;
    sctp_trans_id_t   transaction_id;
    sctp_msg_register_su_res_st reg_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
    sctp_U32bit u32_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/



    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_register_su_resp " ) );

    __MEMSET( &reg_resp, 0, sizeof( sctp_msg_register_su_res_st ) );

    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_register_su_res_st ), ( sctp_U8bit * )&reg_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &u32_temp );
    reg_resp.result = ( sctp_return_t )u32_temp;
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &u32_temp );
    reg_resp.errorno = ( sctp_error_t )u32_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    reg_resp.su_id = ( sctp_U16bit )u16_temp;

    if( reg_resp.result == SCTP_SUCCESS )
    {
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &reg_resp.appl_id );
    }

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Register SU:SU Id:%d,Appl Id:%d,result:%d,\
        errorno:%d", reg_resp.su_id, reg_resp.appl_id, reg_resp.result,
                         reg_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_REGISTER_SU_RESP, &reg_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_deregister_su_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_deregister_su_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )

{
    sctp_return_t ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_deregister_su_res_st dereg_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_deregister_su_resp " ) );

    __MEMSET( &dereg_resp, 0, sizeof( sctp_msg_deregister_su_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_deregister_su_res_st ), ( sctp_U8bit * )&dereg_resp );
#else
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    dereg_resp.appl_id = u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    dereg_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    dereg_resp.error = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::DeRegister SU:Appl Id:%d,result:%d,\
        errorno:%d", dereg_resp.appl_id, dereg_resp.result, dereg_resp.error ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_DEREGISTER_SU_RESP, &dereg_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_socket_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_socket_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_socket_res_st socket_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
    sctp_U32bit u32_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_socket_resp " ) );

    __MEMSET( &socket_resp, 0, sizeof( sctp_msg_socket_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_socket_res_st ) , ( sctp_U8bit * )&socket_resp );
#else
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    socket_resp.appl_id = u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    socket_resp.result = ( sctp_return_t ) u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    socket_resp.error = ( sctp_error_t )u16_temp;

    if( socket_resp.result == SCTP_SUCCESS )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &u32_temp );
        socket_resp.assoc_id = u32_temp;
    }

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Socket:Appl Id:%d,Assoc Id:%u,result:%d,\
        errorno:%d", socket_resp.appl_id, socket_resp.assoc_id,
                         socket_resp.result, socket_resp.error ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_SOCKET_RESP, &socket_resp );

    return( ret );

}

/***************************************************************************
 *  FUNCTION : sctp_su_proc_api_bind_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_bind_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_bind_res_st bind_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U32bit u32_temp;
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_bind_resp " ) );

    __MEMSET( &bind_resp, 0, sizeof( sctp_msg_bind_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_bind_res_st ), ( sctp_U8bit * )&bind_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &u32_temp );
    bind_resp.association_id = u32_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    bind_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    bind_resp.error = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */
    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Bind:Assoc Id:%u,result:%d,errorno:%d",
                         bind_resp.association_id, bind_resp.result, bind_resp.error ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_BIND_RESP, &bind_resp );

    return( ret );
}


/***************************************************************************
 *  FUNCTION : sctp_su_proc_api_bind_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_bindx_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_bindx_res_st bindx_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U32bit u32_temp;
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_bindx_resp " ) );

    __MEMSET( &bindx_resp, 0, sizeof( sctp_msg_bindx_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_bindx_res_st ), ( sctp_U8bit * )&bindx_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &u32_temp );
    bindx_resp.association_id = u32_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    bindx_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    bindx_resp.error = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */
    SCTP_CLIENT_TRACE( ( "API Response::Bindx:Assoc Id:%u,result:%d,errorno:%d",
                         bindx_resp.association_id, bindx_resp.result, bindx_resp.error ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_BINDX_RESP, &bindx_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION : sctp_su_proc_api_listen_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_listen_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t           ret = SCTP_SUCCESS;
    sctp_dest_id_t          dest_id;
    sctp_src_id_t           src_id;
    sctp_trans_id_t         transaction_id;
    sctp_msg_listen_res_st  listen_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U32bit u32_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_listen_resp " ) );

    __MEMSET( &listen_resp, 0, sizeof( sctp_msg_listen_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_listen_res_st ), ( sctp_U8bit * )&listen_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &u32_temp );
    listen_resp.association_id = u32_temp;
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &u32_temp );
    listen_resp.result = ( sctp_return_t )u32_temp;
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &u32_temp );
    listen_resp.error = ( sctp_error_t )u32_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Listen:Assoc Id:%u,result:%d,errorno:\
        %d", listen_resp.association_id, listen_resp.result,
                         listen_resp.error ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_LISTEN_RESP, &listen_resp );

    return( ret );
}


/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_config_assoc_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_config_assoc_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )

{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_config_assoc_res_st config_assoc_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
    sctp_U32bit u32_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_config_assoc_resp " ) );

    __MEMSET( &config_assoc_resp, 0, sizeof( sctp_msg_config_assoc_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_config_assoc_res_st ),
                                           ( sctp_U8bit * )&config_assoc_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &u32_temp );
    config_assoc_resp.association_id = u32_temp;

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    config_assoc_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    config_assoc_resp.error = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */
    SCTP_CLIENT_TRACE( ( "API Response::config Socket:Assoc Id:%u,result:%d,\
        errorno:%d",
                         config_assoc_resp.association_id,
                         config_assoc_resp.result, config_assoc_resp.error ) );


    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_CONFIG_ASSOC_RESP, &config_assoc_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_config_assoc_ctl_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_config_assoc_ctl_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )

{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_config_assoc_ctl_res_st config_assoc_ctl_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U32bit u32_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_config_assoc_ctl_resp " ) );

    __MEMSET( &config_assoc_ctl_resp, 0,
              sizeof( sctp_msg_config_assoc_ctl_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_config_assoc_ctl_res_st ),
                                           ( sctp_U8bit * )&config_assoc_ctl_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &u32_temp );
    config_assoc_ctl_resp.association_id = u32_temp;

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &u32_temp );
    config_assoc_ctl_resp.result = ( sctp_return_t )u32_temp;

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &u32_temp );
    config_assoc_ctl_resp.error = ( sctp_error_t )u32_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::config Socket:Assoc Id:%u,result:%d,\
        errorno:%d",
                         config_assoc_ctl_resp.association_id,
                         config_assoc_ctl_resp.result, config_assoc_ctl_resp.error ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_CONFIG_ASSOC_CTL_RESP, &config_assoc_ctl_resp );

    return( ret );
}


/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_set_fthresh_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_set_fthresh_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )

{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_set_fthresh_res_st fthresh_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_set_fthresh_resp " ) );

    __MEMSET( &fthresh_resp, 0, sizeof( sctp_msg_set_fthresh_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_fthresh_res_st ), ( sctp_U8bit * )&fthresh_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &fthresh_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    fthresh_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    fthresh_resp.error = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Set Fthresht:Assoc Id:%u,result:%d,\
        errorno:%d",
                         fthresh_resp.association_id, fthresh_resp.result, fthresh_resp.error ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_SET_FTHRESH_RESP, &fthresh_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_connect_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_connect_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )

{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_connect_res_st connect_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_connect_resp " ) );

    __MEMSET( &connect_resp, 0, sizeof( sctp_msg_connect_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_connect_res_st ), ( sctp_U8bit * )&connect_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &connect_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    connect_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    connect_resp.error = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Connect:Assoc Id:%u,result:%d,errorno:%d\
        ", connect_resp.association_id, connect_resp.result,
                         connect_resp.error ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_CONNECT_RESP, &connect_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_connect_ext_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_connect_ext_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_connect_ext_res_st connect_ext_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_connect_ext_resp " ) );

    __MEMSET( &connect_ext_resp, 0, sizeof( sctp_msg_connect_ext_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_connect_ext_res_st ), ( sctp_U8bit * )&connect_ext_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &connect_ext_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    connect_ext_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    connect_ext_resp.error = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */
    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Connect Ext:Assoc Id:%u,result:%d,\
        errorno:%d",
                         connect_ext_resp.association_id,
                         connect_ext_resp.result, connect_ext_resp.error ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_CONNECT_EXT_RESP, &connect_ext_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_accept_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_accept_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_accept_res_st accept_resp;
    sctp_U32bit count;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_accept_resp " ) );

    __MEMSET( &accept_resp, 0, sizeof( sctp_msg_accept_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_accept_res_st ), ( sctp_U8bit * )&accept_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &accept_resp.association_id );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    accept_resp.result = ( sctp_return_t )u16_temp;

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    accept_resp.errorno = ( sctp_error_t )u16_temp;

    if( accept_resp.result == SCTP_SUCCESS )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &accept_resp.nos_of_dest_trans_addrs );

        for( count = 0; count < accept_resp.nos_of_dest_trans_addrs; count++ )
        {
            /* address type   */
            p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                        &accept_resp.list_of_dest_trans_addrs[count] );
        }

        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
        accept_resp.sctp_port = u16_temp;
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &accept_resp.new_association_id );
    }

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Accept:Assoc Id:%u,No of addrs:%u,\
        Sctp port:%d, New Assoc Id=%u,result:%d,errorno:%d",
                         accept_resp.association_id,
                         accept_resp.nos_of_dest_trans_addrs, accept_resp.sctp_port,
                         accept_resp.new_association_id, accept_resp.result,
                         accept_resp.errorno ) );

    /*SPR 5031*/
    if( SCTP_SUCCESS == accept_resp.result )
    {
        for( count = 0; count < accept_resp.nos_of_dest_trans_addrs; count++ )
        {
            SCTP_CLIENT_TRACE( ( "Index= %u,address\r", count ) );
            SCTP_CLIENT_TRACE_ADDR( &accept_resp.list_of_dest_trans_addrs[count] );

        }
    }

    /*SPR 5031 */

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_ACCEPT_RESP, &accept_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_accept_ext_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_accept_ext_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )

{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_accept_ext_res_st accept_ext_resp;
    sctp_U32bit count;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_accept_ext_resp " ) );

    __MEMSET( &accept_ext_resp, 0, sizeof( sctp_msg_accept_ext_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_accept_ext_res_st ), ( sctp_U8bit * )&accept_ext_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &accept_ext_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    accept_ext_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    accept_ext_resp.errorno = ( sctp_error_t )u16_temp;

    if( accept_ext_resp.result == SCTP_SUCCESS )
    {
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
        accept_ext_resp.num_out_streams = u16_temp;
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
        accept_ext_resp.num_in_streams = u16_temp;

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &accept_ext_resp.nos_of_dest_trans_addrs );

        for( count = 0;
                count < accept_ext_resp.nos_of_dest_trans_addrs;
                count++ )
        {
            /* address type   */
            p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                        &accept_ext_resp.list_of_dest_trans_address[count] );
        }

        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
        accept_ext_resp.sctp_port = u16_temp;
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &accept_ext_resp.new_association_id );
    }

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */
    SCTP_CLIENT_TRACE( ( "API Response::Accept Ext:Assoc Id:%u,In Streams:%d:\
        Out Streams:%d", accept_ext_resp.association_id,
                         accept_ext_resp.num_in_streams, accept_ext_resp.num_out_streams ) );

    SCTP_CLIENT_TRACE( ( "No of addrs:%u,Sctp port:%d,New Assoc Id=%u,result:%d,\
        errorno:%d", accept_ext_resp.nos_of_dest_trans_addrs,
                         accept_ext_resp.sctp_port, accept_ext_resp.new_association_id,
                         accept_ext_resp.result, accept_ext_resp.errorno ) );

    /*SPR 5031*/
    if( SCTP_SUCCESS == accept_ext_resp.result )
    {
        for( count = 0; count < accept_ext_resp.nos_of_dest_trans_addrs; count++ )
        {
            SCTP_CLIENT_TRACE( ( "Index= %u,address\r", count ) );
            SCTP_CLIENT_TRACE_ADDR(
                &accept_ext_resp.list_of_dest_trans_address[count] );
        }
    }

    /*SPR 5031*/
    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_ACCEPT_EXT_RESP, &accept_ext_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_send_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_send_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )

{
    sctp_return_t         ret = SCTP_SUCCESS;
    sctp_dest_id_t        dest_id;
    sctp_src_id_t         src_id;
    sctp_trans_id_t       transaction_id;
    sctp_msg_send_res_st  send_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit                u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_send_resp " ) );

    __MEMSET( &send_resp, 0, sizeof( sctp_msg_send_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_send_res_st ) , ( sctp_U8bit * )&send_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &send_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    send_resp.result = ( sctp_return_t ) u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    send_resp.errorno = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Send:Assoc Id:%u,result:%d,errorno:%d",
                         send_resp.association_id, send_resp.result, send_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_SEND_RESP, &send_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_send_ext_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_send_ext_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t    ret = SCTP_SUCCESS;
    sctp_dest_id_t   dest_id;
    sctp_src_id_t    src_id;
    sctp_trans_id_t  transaction_id;
    sctp_msg_send_ext_res_st send_ext_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit                    u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_send_ext_resp " ) );

    __MEMSET( &send_ext_resp, 0, sizeof( sctp_msg_send_ext_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_send_ext_res_st ), ( sctp_U8bit * )&send_ext_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &send_ext_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    send_ext_resp.result = ( sctp_return_t ) u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    send_ext_resp.errorno = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Send Ext:Assoc Id:%u,result:%d,errorno:\
        %d", send_ext_resp.association_id,
                         send_ext_resp.result, send_ext_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_SEND_EXT_RESP, &send_ext_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_receive_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_receive_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t            ret = SCTP_SUCCESS;
    sctp_dest_id_t           dest_id;
    sctp_src_id_t            src_id;
    sctp_trans_id_t          transaction_id;
    sctp_U8bit               *p_temp = NULL;
    sctp_msg_receive_res_st  receive_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_receive_resp " ) );

    __MEMSET( &receive_resp, 0, sizeof( sctp_msg_receive_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_receive_res_st ) - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )&receive_resp );
    receive_resp.data = SCTP_NULL;

    if( receive_resp.result == SCTP_SUCCESS )
    {
        receive_resp.data = ( sctp_U8bit * )
                            sctp_client_get_buffer( receive_resp.data_size );

        p_temp = receive_resp.data ;

        if( receive_resp.data == SCTP_NULL )
        {
            /* malloc fails : error */
            SCTP_CLIENT_TRACE( ( "ERROR :: sctp_client_get_buffer(%d) failed.", receive_resp.data_size ) );
        }

        else
        {
            __MEMSET( receive_resp.data, 0, receive_resp.data_size );
            p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                   receive_resp.data_size, receive_resp.data );
        }
    }

#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &receive_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    receive_resp.result = ( sctp_return_t ) u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    receive_resp.errorno = ( sctp_error_t )u16_temp;

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &receive_resp.protocol_id );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &receive_resp.data_size );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    receive_resp.stream_id = u16_temp;

    if( receive_resp.result == SCTP_SUCCESS )
    {
        receive_resp.data = ( sctp_U8bit * )
                            sctp_client_get_buffer( receive_resp.data_size );

        p_temp = receive_resp.data ;

        if( receive_resp.data == SCTP_NULL )
        {
            /* malloc fails : error */
            SCTP_CLIENT_TRACE( ( "ERROR :: sctp_client_get_buffer(%d) failed.", receive_resp.data_size ) );
        }

        else
        {
            __MEMSET( receive_resp.data, 0, receive_resp.data_size );

            p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                   receive_resp.data_size, receive_resp.data );
        }
    }

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Receive:Assoc Id:%u,PPI:%u,Size:%u,\
        Stream:%d, result:%d,errorno:%dData ...", receive_resp.association_id,
                         receive_resp.protocol_id, receive_resp.data_size,
                         receive_resp.stream_id, receive_resp.result, receive_resp.errorno ) );

    /* SPR : 18527 */
    if( receive_resp.result == SCTP_SUCCESS && receive_resp.data != SCTP_NULL )
        /* SPR : 18527 */
    {
        SCTP_CL_PRINT_HEX( receive_resp.data, receive_resp.data_size );
    }

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_RECEIVE_RESP, &receive_resp );

    if( ( receive_resp.result == SCTP_SUCCESS ) && ( p_temp != SCTP_NULL ) )
    {
        sctp_client_free_buffer( p_temp );
    }

    return( ret );
}

/***************************************************************************
 *    FUNCTION :
 *  sctp_su_proc_api_receive_ext_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_receive_ext_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t               ret = SCTP_SUCCESS;
    sctp_dest_id_t              dest_id;
    sctp_src_id_t               src_id;
    sctp_trans_id_t             transaction_id;
    sctp_U8bit                  *p_temp = SCTP_NULL;
    sctp_msg_receive_ext_res_st receive_ext_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit                 u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_receive_ext_resp " ) );

    __MEMSET( &receive_ext_resp, 0, sizeof( sctp_msg_receive_ext_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_receive_ext_res_st ) - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )&receive_ext_resp );
    receive_ext_resp.data = NULL;

    if( receive_ext_resp.result == SCTP_SUCCESS )
    {
        receive_ext_resp.data = ( sctp_U8bit * )
                                sctp_client_get_buffer( receive_ext_resp.data_size );

        p_temp = receive_ext_resp.data ;

        if( receive_ext_resp.data  == NULL )
        {
            /* malloc fails : error */
            SCTP_CLIENT_TRACE( ( "ERROR :: sctp_client_get_buffer(%d) failed.", receive_ext_resp.data_size ) );
        }

        else
        {
            __MEMSET( receive_ext_resp.data, 0, receive_ext_resp.data_size );
            p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                   receive_ext_resp.data_size, receive_ext_resp.data );
        }
    }

#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &receive_ext_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    receive_ext_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    receive_ext_resp.errorno = ( sctp_error_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    receive_ext_resp.stream_id = u16_temp;
    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                &receive_ext_resp.dest_address );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &receive_ext_resp.protocol_id );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &receive_ext_resp.data_size );

    if( receive_ext_resp.result == SCTP_SUCCESS )
    {
        receive_ext_resp.data = ( sctp_U8bit * )
                                sctp_client_get_buffer( receive_ext_resp.data_size );

        p_temp = receive_ext_resp.data ;

        if( receive_ext_resp.data == NULL )
        {
            /* malloc fails : error */
            SCTP_CLIENT_TRACE( ( "ERROR :: sctp_client_get_buffer(%d) failed.", receive_ext_resp.data_size ) );
        }

        else
        {
            __MEMSET( receive_ext_resp.data, 0, receive_ext_resp.data_size );
            p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                   receive_ext_resp.data_size, receive_ext_resp.data );
        }
    }

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Receive Ext:Assoc Id:%u,PPI:%u,Size:%u,\
        Stream:%d,result:%d,errorno:%dData ...",
                         receive_ext_resp.association_id, receive_ext_resp.protocol_id,
                         receive_ext_resp.data_size, receive_ext_resp.stream_id,
                         receive_ext_resp.result, receive_ext_resp.errorno ) );

    /* SPR : 18527 */
    if( receive_ext_resp.result == SCTP_SUCCESS && receive_ext_resp.data != SCTP_NULL )
        /* SPR : 18527 */
    {
        SCTP_CL_PRINT_HEX( receive_ext_resp.data, receive_ext_resp.data_size );
    }

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_RECEIVE_EXT_RESP, &receive_ext_resp );

    if( ( receive_ext_resp.result == SCTP_SUCCESS ) && ( p_temp != SCTP_NULL ) )
    {
        sctp_client_free_buffer( p_temp );
    }

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_receive_part_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_receive_part_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t                 ret = SCTP_SUCCESS;
    sctp_dest_id_t                dest_id;
    sctp_src_id_t                 src_id;
    sctp_trans_id_t               transaction_id;
    sctp_U8bit                    *p_temp = SCTP_NULL;
    sctp_msg_receive_part_res_st  receive_part_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit                   u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_receive_part_resp " ) );

    __MEMSET( &receive_part_resp, 0, sizeof( sctp_msg_receive_part_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_receive_part_res_st ) - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )&receive_part_resp );
    receive_part_resp.data = SCTP_NULL;

    if( receive_part_resp.result == SCTP_SUCCESS )
    {
        receive_part_resp.data =
            ( sctp_U8bit * )sctp_client_get_buffer( receive_part_resp.data_size );

        p_temp = receive_part_resp.data;

        if( receive_part_resp.data == NULL )
        {
            /* malloc fails : error */
            SCTP_CLIENT_TRACE( ( "ERROR :: sctp_client_get_buffer(%d) Failed.",
                                 receive_part_resp.data_size ) );
        }

        else
        {
            __MEMSET( receive_part_resp.data, 0, receive_part_resp.data_size );
            p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                   receive_part_resp.data_size, receive_part_resp.data );
        }
    }

#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &receive_part_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    receive_part_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    receive_part_resp.errorno = ( sctp_error_t )u16_temp;

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    receive_part_resp.stream_id = u16_temp;
    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                &receive_part_resp.dest_address );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &receive_part_resp.protocol_id );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &receive_part_resp.data_size );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    receive_part_resp.ssn = u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    receive_part_resp.delivery_num = u16_temp;

    if( receive_part_resp.result == SCTP_SUCCESS )
    {
        receive_part_resp.data = ( sctp_U8bit * )
                                 sctp_client_get_buffer( receive_part_resp.data_size );

        p_temp = receive_part_resp.data;

        if( receive_part_resp.data == NULL )
        {
            /* malloc fails : error */
            SCTP_CLIENT_TRACE( ( "ERROR :: sctp_client_get_buffer(%d) Failed.", receive_part_resp.data_size ) );
        }

        else
        {
            __MEMSET( receive_part_resp.data, 0, receive_part_resp.data_size );

            p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                   receive_part_resp.data_size, receive_part_resp.data );
        }
    }

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Receive Partial:Assoc Id:%u,Stream:%d,\
        PPI:%u,Size:%u,ssn=%d,delivery_no=%d,result:%d,errorno:%d:Data ...",
                         receive_part_resp.association_id, receive_part_resp.stream_id,
                         receive_part_resp.protocol_id, receive_part_resp.data_size,
                         receive_part_resp.ssn, receive_part_resp.delivery_num,
                         receive_part_resp.result, receive_part_resp.errorno ) );

    /* SPR : 18527 */
    if( receive_part_resp.result == SCTP_SUCCESS && receive_part_resp.data != SCTP_NULL )
        /* SPR : 18527 */
    {
        SCTP_CL_PRINT_HEX( receive_part_resp.data, receive_part_resp.data_size );
    }

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_RECEIVE_PART_RESP, &receive_part_resp );

    if( ( receive_part_resp.result == SCTP_SUCCESS ) && ( p_temp != NULL ) )
    {
        sctp_client_free_buffer( p_temp );
    }

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_close_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_close_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t           ret = SCTP_SUCCESS;
    sctp_dest_id_t          dest_id;
    sctp_src_id_t           src_id;
    sctp_trans_id_t         transaction_id;
    sctp_msg_close_res_st   close_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit             u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_close_resp " ) );

    __MEMSET( &close_resp, 0, sizeof( sctp_msg_close_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_close_res_st ), ( sctp_U8bit * )&close_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &close_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    close_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    close_resp.errorno = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */
    SCTP_CLIENT_TRACE( ( "API Response::Close:Assoc Id%u,result:%d,errorno:%d",
                         close_resp.association_id, close_resp.result, close_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_CLOSE_RESP, &close_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_hb_action_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_hb_action_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t              ret = SCTP_SUCCESS;
    sctp_dest_id_t             dest_id;
    sctp_src_id_t              src_id;
    sctp_trans_id_t            transaction_id;
    sctp_msg_hb_action_res_st  hb_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit                u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_hb_action_resp" ) );

    __MEMSET( &hb_resp, 0, sizeof( sctp_msg_hb_action_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_hb_action_res_st ), ( sctp_U8bit * )&hb_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &hb_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    hb_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    hb_resp.errorno = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::HB Action:Assoc Id%u,result:%d,errorno:\
        %d", hb_resp.association_id, hb_resp.result, hb_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_HB_ACTION_RESP, &hb_resp );
    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_set_primary_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_set_primary_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t                ret = SCTP_SUCCESS;
    sctp_dest_id_t               dest_id;
    sctp_src_id_t                src_id;
    sctp_trans_id_t              transaction_id;
    sctp_msg_set_primary_res_st  set_primary_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit                  u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_set_primary_resp " ) );

    __MEMSET( &set_primary_resp, 0, sizeof( sctp_msg_set_primary_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_primary_res_st ),
                                           ( sctp_U8bit * )&set_primary_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &set_primary_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    set_primary_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    set_primary_resp.errorno = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Set Primary:Assoc Id%u,result:%d,errorno:\
        %d", set_primary_resp.association_id,
                         set_primary_resp.result, set_primary_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_SET_PRIMARY_RESP, &set_primary_resp );
    return( ret );
}

/***************************************************************************
 *  FUNCTION : sctp_su_proc_api_get_srtt_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_get_srtt_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t              ret = SCTP_SUCCESS;
    sctp_dest_id_t             dest_id;
    sctp_src_id_t              src_id;
    sctp_trans_id_t            transaction_id;
    sctp_msg_get_srtt_res_st   get_srtt_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit                u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_get_srtt_resp " ) );

    __MEMSET( &get_srtt_resp, 0, sizeof( sctp_msg_get_srtt_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_srtt_res_st ), ( sctp_U8bit * )&get_srtt_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &get_srtt_resp.association_id );
    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                &get_srtt_resp.dest_address );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    get_srtt_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    get_srtt_resp.errorno = ( sctp_error_t )u16_temp;

    if( get_srtt_resp.result == SCTP_SUCCESS )
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_srtt_resp.srtt_value );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Get Srtt Response:Assoc Id%u,Srtt:%u,\
        result:%d, errorno:%d", get_srtt_resp.association_id,
                         get_srtt_resp.srtt_value, get_srtt_resp.result, get_srtt_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_GET_SRTT_RESP, &get_srtt_resp );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_get_status_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_get_status_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t               ret = SCTP_SUCCESS;
    sctp_dest_id_t              dest_id;
    sctp_src_id_t               src_id;
    sctp_trans_id_t             transaction_id;
    sctp_msg_get_status_res_st  get_status_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_get_status_resp " ) );

    __MEMSET( &get_status_resp, 0, sizeof( sctp_msg_get_status_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );

    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_status_res_st ), ( sctp_U8bit * )&get_status_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_status_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_status_resp.errorno );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &get_status_resp.association_id );

    if( get_status_resp.result == SCTP_SUCCESS )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&get_status_resp.assoc_status.connection_state );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_status_resp.assoc_status.num_in_streams );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_status_resp.assoc_status.num_out_streams );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_status_resp.assoc_status.curr_receiver_win_size );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_status_resp.assoc_status.num_data_chnks_awaiting_ack );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_status_resp.assoc_status.num_data_chnks_awaiting_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_status_resp.assoc_status.last_acked_TSN );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_status_resp.assoc_status.last_sent_TSN );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_status_resp.assoc_status.num_msgs_awaiting_recv );

        p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                    &get_status_resp.assoc_status.primary_dest_addr );
    }

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    if( get_status_resp.result == SCTP_FAILURE )
    {
        SCTP_CLIENT_TRACE( ( "API Response::Get Status Response: API FAILURE: \
            Assoc Id %u, errorno:%d",
                             get_status_resp.association_id, get_status_resp.errorno ) );
    }

    else if( get_status_resp.result == SCTP_SUCCESS )
    {
        SCTP_CLIENT_TRACE( ( "API Response::Get Status Response: Assoc Id %u",
                             get_status_resp.association_id ) );

        SCTP_CLIENT_TRACE( ( "Connection state : %u",
                             get_status_resp.assoc_status.connection_state ) );

        SCTP_CLIENT_TRACE( ( "Primary destination address : " ) );
        SCTP_CLIENT_TRACE_ADDR( &get_status_resp.assoc_status.primary_dest_addr );

        SCTP_CLIENT_TRACE( ( "Number of input streams    : %u",
                             get_status_resp.assoc_status.num_in_streams ) );

        SCTP_CLIENT_TRACE( ( "Number of output streams : %u",
                             get_status_resp.assoc_status.num_out_streams ) );

        SCTP_CLIENT_TRACE( ( "Current receiver window size : %u",
                             get_status_resp.assoc_status.curr_receiver_win_size ) );

        SCTP_CLIENT_TRACE( ( "Number of data chunks awaiting ack : %u",
                             get_status_resp.assoc_status.num_data_chnks_awaiting_ack ) );

        SCTP_CLIENT_TRACE( ( "Number of data chunks awaiting send :%u",
                             get_status_resp.assoc_status.num_data_chnks_awaiting_sent ) );

        SCTP_CLIENT_TRACE( ( "Last acked TSN : %u",
                             get_status_resp.assoc_status.last_acked_TSN ) );

        SCTP_CLIENT_TRACE( ( "Last sent TSN :    %u",
                             get_status_resp.assoc_status.last_sent_TSN ) );

        SCTP_CLIENT_TRACE( ( "Number of messages awaiting receive : %u",
                             get_status_resp.assoc_status.num_msgs_awaiting_recv ) );

    }

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_GET_STATUS_RESP, &get_status_resp );
    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_get_dest_status_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_get_dest_status_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t                   ret = SCTP_SUCCESS;
    sctp_dest_id_t                  dest_id;
    sctp_src_id_t                   src_id;
    sctp_trans_id_t                 transaction_id;
    sctp_msg_get_deststatus_res_st  get_dest_status_resp;
    sctp_U32bit count;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_get_dest_status_resp " ) );

    __MEMSET( &get_dest_status_resp, 0, sizeof( sctp_msg_get_deststatus_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_deststatus_res_st ),
                                           ( sctp_U8bit * )&get_dest_status_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &get_dest_status_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    get_dest_status_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    get_dest_status_resp.errorno = ( sctp_error_t )u16_temp;

    if( get_dest_status_resp.result == SCTP_SUCCESS )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_dest_status_resp.nos_of_dest_trans_addrs );

        for( count = 0;
                count < get_dest_status_resp.nos_of_dest_trans_addrs;
                count++ )
        {
            p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                        &get_dest_status_resp.list_of_dest_trans_addrs[count] );
        }

        for( count = 0;
                count < get_dest_status_resp.nos_of_dest_trans_addrs;
                count++ )
        {
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_dest_status_resp.dest_param_list[count].srtt );
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_dest_status_resp.dest_param_list[count].rto );
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_dest_status_resp.dest_param_list[count].mtu_size );
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_dest_status_resp.dest_param_list[count].cwnd );
            /* Changes for CSR# 1-4267892 */
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_dest_status_resp.dest_param_list[count].allow_heartbeat );
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_dest_status_resp.dest_param_list[count].status );
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_dest_status_resp.dest_param_list[count].is_confirmed );
            /* Changes for CSR# 1-4267892 Ends*/
        }

        p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                    &get_dest_status_resp.primary_dest_transport_addr );
    }

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Dest Status:Assoc Id%u, result:%d, error:\
        %d, number Addrs=%u", get_dest_status_resp.association_id,
                         get_dest_status_resp.result, get_dest_status_resp.errorno,
                         get_dest_status_resp.nos_of_dest_trans_addrs ) );

    if( get_dest_status_resp.result == SCTP_SUCCESS )
    {
        for( count = 0;
                count < get_dest_status_resp.nos_of_dest_trans_addrs;
                count++ )
        {
            SCTP_CLIENT_TRACE( ( " Index:%u", count ) );
            SCTP_CLIENT_TRACE_ADDR(
                &get_dest_status_resp.list_of_dest_trans_addrs[count] );

            SCTP_CLIENT_TRACE( ( " SRTT:%u,RTO:%u,PMTU:%u,CWND:%u,ALLOW_HB:%u,STSTUS:%u,IS_CONFIRMED:%u",
                                 get_dest_status_resp.dest_param_list[count].srtt,
                                 get_dest_status_resp.dest_param_list[count].rto,
                                 get_dest_status_resp.dest_param_list[count].mtu_size,
                                 get_dest_status_resp.dest_param_list[count].cwnd,
                                 get_dest_status_resp.dest_param_list[count].allow_heartbeat,
                                 get_dest_status_resp.dest_param_list[count].status,
                                 get_dest_status_resp.dest_param_list[count].is_confirmed ) );
        }
    }

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_GET_DESTSTATUS_RESP, &get_dest_status_resp );
    return( ret );
}

/***************************************************************************
 *  FUNCTION :   sctp_su_proc_api_set_restart_option_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_set_restart_option_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_set_restart_option_res_st restart_option_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_set_restart_option_resp " ) );

    __MEMSET( &restart_option_resp, 0,
              sizeof( sctp_msg_set_restart_option_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_restart_option_res_st ),
                                           ( sctp_U8bit * )&restart_option_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &restart_option_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    restart_option_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    restart_option_resp.errorno = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Set Restart Option:Assoc Id%u, result:%d,\
        errorno:%d", restart_option_resp.association_id,
                         restart_option_resp.result, restart_option_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_SET_RESTART_OPTION_RESP, &restart_option_resp );
    return( ret );
}


/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_ret_unsent_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_ret_unsent_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_trans_id_t transaction_id;
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_msg_ret_unsent_res_st     unsent_resp;
    sctp_U8bit *p_temp = SCTP_NULL;
#ifndef SCTP_STRUCT_BASED
    sctp_U32bit count;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_ret_unsent_resp " ) );

    __MEMSET( &unsent_resp, 0, sizeof( sctp_msg_ret_unsent_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );

    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_ret_unsent_res_st ) - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )&unsent_resp );
    unsent_resp.data = NULL;

    if( unsent_resp.result == SCTP_SUCCESS )
    {
        unsent_resp.data = ( sctp_U8bit * )
                           sctp_client_get_buffer( unsent_resp.data_size );

        p_temp = unsent_resp.data ;

        if( unsent_resp.data == SCTP_NULL )
        {
            /* malloc fails : error */
        }
        else
        {
            __MEMSET( unsent_resp.data, 0, unsent_resp.data_size );

            p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                   unsent_resp.data_size,
                                                   unsent_resp.data );
        }
    }

#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&unsent_resp.result );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&unsent_resp.errorno );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &unsent_resp.association_id );

    if( unsent_resp.result == SCTP_SUCCESS )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &unsent_resp.context );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &unsent_resp.data_size );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &unsent_resp.ppi );
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &unsent_resp.stream );

        unsent_resp.data = ( sctp_U8bit * )
                           sctp_client_get_buffer( unsent_resp.data_size );

        p_temp = unsent_resp.data ;

        if( unsent_resp.data == SCTP_NULL )
        {
            /* malloc fails : error */
        }
        else
        {
            __MEMSET( unsent_resp.data, 0, unsent_resp.data_size );

            for( count = 0; count < unsent_resp.data_size; count++ )
            {
                p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                                      unsent_resp.data );
                unsent_resp.data++;
            }

            unsent_resp.data = p_temp;
        }
    }

#endif /* SCTP_STRUCT_BASED */

    if( unsent_resp.result == SCTP_FAILURE )
    {
        SCTP_CLIENT_TRACE( ( "API Response::Ret unsent : API FAILURE :Assoc Id \
            %u, errorno: %d", unsent_resp.association_id,
                             unsent_resp.errorno ) );
    }

    else if( unsent_resp.result == SCTP_SUCCESS )
    {
        SCTP_CLIENT_TRACE( ( "API Response::Ret unsent : API SUCCESS :Assoc Id\
            %u, errorno:%d", unsent_resp.association_id,
                             unsent_resp.errorno ) );

        SCTP_CLIENT_TRACE( ( "Context           : %u", unsent_resp.context ) );
        SCTP_CLIENT_TRACE( ( "Data Size         : %u", unsent_resp.data_size ) );
        SCTP_CLIENT_TRACE( ( "PPI               : %u", unsent_resp.ppi ) );
        SCTP_CLIENT_TRACE( ( "Stream ID         : %d",  unsent_resp.stream ) );

        /* SPR : 18527 */
        if( unsent_resp.data != SCTP_NULL )
            /* SPR : 18527 */
        {
            SCTP_CLIENT_TRACE( ( "Data              : " ) );
            SCTP_CL_PRINT_HEX( unsent_resp.data, unsent_resp.data_size );
        }

        else
        {
            SCTP_CLIENT_TRACE( ( "Data              : NULL" ) );
        }
    }

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_RETRIEVE_UNSENT_RESP, &unsent_resp );

    if( ( unsent_resp.result == SCTP_SUCCESS ) && ( p_temp != NULL ) )
    {
        sctp_client_free_buffer( p_temp );
    }

    return ( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_ret_unacked_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_ret_unacked_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_trans_id_t transaction_id;
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_U8bit      *p_temp = SCTP_NULL;
    sctp_msg_ret_unacked_res_st  unacked_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U32bit count;
#endif /* SCTP_STRUCT_BASED */


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_ret_unacked_resp " ) );

    __MEMSET( &unacked_resp, 0, sizeof( sctp_msg_ret_unacked_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );

    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_ret_unacked_res_st ) - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )&unacked_resp );
    unacked_resp.data = NULL;

    if( unacked_resp.result == SCTP_SUCCESS )
    {
        unacked_resp.data = ( sctp_U8bit * )
                            sctp_client_get_buffer( unacked_resp.data_size );

        p_temp = unacked_resp.data ;

        if( unacked_resp.data == NULL )
        {
            /* malloc fails : error */
        }
        else
        {
            __MEMSET( unacked_resp.data, 0, unacked_resp.data_size );

            p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                   unacked_resp.data_size, unacked_resp.data );
        }
    }

#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&unacked_resp.result );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&unacked_resp.errorno );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &unacked_resp.association_id );

    if( unacked_resp.result == SCTP_SUCCESS )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &unacked_resp.context );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &unacked_resp.data_size );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &unacked_resp.ppi );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&unacked_resp.incomplete_flag );

        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &unacked_resp.stream );
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &unacked_resp.ssn );

        unacked_resp.data = ( sctp_U8bit * )
                            sctp_client_get_buffer( unacked_resp.data_size );

        p_temp = unacked_resp.data ;

        if( unacked_resp.data == SCTP_NULL )
        {
            /* malloc fails : error */
        }
        else
        {
            __MEMSET( unacked_resp.data, 0, unacked_resp.data_size );

            for( count = 0; count < unacked_resp.data_size; count++ )
            {
                p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                                      unacked_resp.data );
                unacked_resp.data++;
            }

            unacked_resp.data = p_temp;
        }

    }

#endif /* SCTP_STRUCT_BASED */

    if( unacked_resp.result == SCTP_FAILURE )
    {
        SCTP_CLIENT_TRACE( ( "API Response::Ret unacked: API FAILURE :Assoc Id%u,\
            errorno:%d", unacked_resp.association_id, unacked_resp.errorno ) );
    }

    else if( unacked_resp.result == SCTP_SUCCESS )
    {
        SCTP_CLIENT_TRACE( ( "API Response::Ret unacked: API SUCCESS :Assoc Id\
            %u, errorno:%d", unacked_resp.association_id,
                             unacked_resp.errorno ) );

        SCTP_CLIENT_TRACE( ( "Context         :    %u", unacked_resp.context ) );
        SCTP_CLIENT_TRACE( ( "Data Size       :    %u", unacked_resp.data_size ) );
        SCTP_CLIENT_TRACE( ( "PPI             :    %u", unacked_resp.ppi ) );
        SCTP_CLIENT_TRACE( ( "Incomplete flag :    %d",
                             unacked_resp.incomplete_flag ) );
        SCTP_CLIENT_TRACE( ( "Stream ID       :    %d", unacked_resp.stream ) );
        SCTP_CLIENT_TRACE( ( "ssn             :    %d", unacked_resp.ssn ) );

        /* SPR : 18527 */
        if( unacked_resp.data != SCTP_NULL )
        {
            /* SPR : 18527 */
            SCTP_CLIENT_TRACE( ( "Data            :    " ) );
            SCTP_CL_PRINT_HEX( unacked_resp.data, unacked_resp.data_size );
        }

        else
        {
            SCTP_CLIENT_TRACE( ( "Data            : NULL" ) );
        }
    }

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_RETRIEVE_UNACKED_RESP, &unacked_resp );

    if( ( unacked_resp.result == SCTP_SUCCESS ) && ( p_temp != NULL ) )
    {
        sctp_client_free_buffer( p_temp );
    }

    return ( ret );

}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_flush_buffers_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_flush_buffers_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_flush_buffers_res_st     flush_buffers_resp;


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_flush_buffers_resp " ) );

    __MEMSET( &flush_buffers_resp, 0, sizeof( sctp_msg_flush_buffers_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_flush_buffers_res_st ),
                                           ( sctp_U8bit * )&flush_buffers_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&flush_buffers_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&flush_buffers_resp.errorno );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &flush_buffers_resp.association_id );
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Flush buffers: Assoc Id%u,result:%d,\
        errorno:%d", flush_buffers_resp.association_id,
                         flush_buffers_resp.result, flush_buffers_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_FLUSH_BUFFERS_RESP, &flush_buffers_resp );
    return( ret );
}


/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_get_stream_status_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_get_stream_status_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_get_stream_status_res_st  get_stream_status_resp;


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_get_stream_status_resp " ) );

    __MEMSET( &get_stream_status_resp, 0,
              sizeof( sctp_msg_get_stream_status_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_stream_status_res_st ),
                                           ( sctp_U8bit * )&get_stream_status_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_stream_status_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_stream_status_resp.errorno );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &get_stream_status_resp.association_id );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &get_stream_status_resp.stream_id );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &get_stream_status_resp.ssn );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::Get Stream Status: Assoc Id:%d,\
        Stream Id:%d, SSN:%d,result:%d,errorno:%d",
                         get_stream_status_resp.association_id,
                         get_stream_status_resp.stream_id,
                         get_stream_status_resp.ssn,
                         get_stream_status_resp.result,
                         get_stream_status_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_GET_STREAM_STATUS_RESP, &get_stream_status_resp );
    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_abort_retrieve_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_abort_retrieve_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_abort_retrieve_res_st  abort_retrieve_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_abort_retrieve_resp " ) );

    __MEMSET( &abort_retrieve_resp, 0,
              sizeof( sctp_msg_abort_retrieve_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_abort_retrieve_res_st ),
                                           ( sctp_U8bit * )&abort_retrieve_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &abort_retrieve_resp.association_id );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&abort_retrieve_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&abort_retrieve_resp.errorno );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&abort_retrieve_resp.data_pending );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */
    SCTP_CLIENT_TRACE( ( "API Response::Abort Retrieve: Assoc Id:%u,\
        Data Pending:%d, result:%d,errorno:%d",
                         abort_retrieve_resp.association_id,
                         abort_retrieve_resp.data_pending,
                         abort_retrieve_resp.result,
                         abort_retrieve_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_ABORT_RETRIEVE_RESP, &abort_retrieve_resp );
    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_config_cong_reg_def_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_config_cong_reg_def_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_config_cong_reg_def_resp_st  config_cong_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_config_cong_reg_def_resp " ) );

    __MEMSET( &config_cong_resp, 0,
              sizeof( sctp_msg_config_cong_reg_def_resp_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_config_cong_reg_def_resp_st ),
                                           ( sctp_U8bit * )&config_cong_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &config_cong_resp.association_id );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&config_cong_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&config_cong_resp.errorno );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::cong_reg_def_resp : Assoc Id:%u,\
        result:%d,errorno:%d",
                         config_cong_resp.association_id,
                         config_cong_resp.result,
                         config_cong_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_CONFIG_CONG_REG_DEF_RESP, &config_cong_resp );
    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_get_cong_reg_def_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_get_cong_reg_def_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_get_cong_reg_def_resp_st  config_cong_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_get_cong_reg_def_resp " ) );

    __MEMSET( &config_cong_resp, 0,
              sizeof( sctp_msg_get_cong_reg_def_resp_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_cong_reg_def_resp_st ),
                                           ( sctp_U8bit * )&config_cong_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &config_cong_resp.association_id );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&config_cong_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&config_cong_resp.errorno );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          ( sctp_U8bit * )&config_cong_resp.cong_reg_def.no_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          ( sctp_U8bit * )&config_cong_resp.cong_reg_def.no_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          ( sctp_U8bit * )&config_cong_resp.cong_reg_def.low_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          ( sctp_U8bit * )&config_cong_resp.cong_reg_def.low_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          ( sctp_U8bit * )&config_cong_resp.cong_reg_def.med_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          ( sctp_U8bit * )&config_cong_resp.cong_reg_def.med_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          ( sctp_U8bit * )&config_cong_resp.cong_reg_def.high_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          ( sctp_U8bit * )&config_cong_resp.cong_reg_def.high_cong_lvl_abate );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::cong_resp : Assoc Id:%u,\
        result:%d,errorno:%d",
                         config_cong_resp.association_id,
                         config_cong_resp.result,
                         config_cong_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_GET_CONG_REG_DEF_RESP, &config_cong_resp );
    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_config_txrx_num_frag_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_config_txrx_num_frag_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_config_txrx_num_frag_resp_st  config_resp;


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_config_txrx_num_frag_resp " ) );

    __MEMSET( &config_resp, 0,
              sizeof( sctp_msg_config_txrx_num_frag_resp_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_config_txrx_num_frag_resp_st ),
                                           ( sctp_U8bit * )&config_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &config_resp.association_id );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&config_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&config_resp.errorno );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::config_resp : Assoc Id:%u,\
        result:%d,errorno:%d",
                         config_resp.association_id,
                         config_resp.result,
                         config_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_CONFIG_TXRX_NUM_FRAG_RESP, &config_resp );
    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_api_get_free_txrx_num_frag_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_api_get_free_txrx_num_frag_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_get_free_tr_num_frag_resp_st  txrx_resp;


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_get_free_txrx_num_frag_resp " ) );

    __MEMSET( &txrx_resp, 0,
              sizeof( sctp_msg_get_free_tr_num_frag_resp_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_free_tr_num_frag_resp_st ),
                                           ( sctp_U8bit * )&txrx_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &txrx_resp.association_id );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&txrx_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&txrx_resp.errorno );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&txrx_resp.tx_num_frags.min_num_frag );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&txrx_resp.tx_num_frags.med_num_frag );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&txrx_resp.tx_num_frags.max_num_frag );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&txrx_resp.rx_num_frags.min_num_frag );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&txrx_resp.rx_num_frags.med_num_frag );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&txrx_resp.rx_num_frags.max_num_frag );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Response::txrx_resp : Assoc Id:%u,\
        result:%d,errorno:%d",
                         txrx_resp.association_id,
                         txrx_resp.result,
                         txrx_resp.errorno ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_GET_FREE_TR_NUM_FRAG_RESP, &txrx_resp );
    return( ret );
}


static sctp_return_t
sctp_su_proc_api_connectx_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )

{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_msg_connectx_res_st connectx_resp;
#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_api_connect_resp " ) );

    __MEMSET( &connectx_resp, 0, sizeof( sctp_msg_connectx_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_connectx_res_st ), ( sctp_U8bit * )&connectx_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &connectx_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    connectx_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    connectx_resp.error = ( sctp_error_t )u16_temp;
#endif /* SCTP_STRUCT_BASED */
    /*SPR 21141 changes starts */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /*SPR 21141 changes ends*/

    SCTP_CLIENT_TRACE( ( "API Response::Connectx:Assoc Id:%u,result:%d,errorno:%d\
        ", connectx_resp.association_id, connectx_resp.result,
                         connectx_resp.error ) );

    sctp_process_api_resp_from_stack( src_id, dest_id, transaction_id,
                                      SCTP_API_CONNECTX_RESP, &connectx_resp );

    return( ret );
}




/************** * notifications processing functions *****************/
/***************************************************************************
 *  FUNCTION :  sctp_su_proc_op_error_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_op_error_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_Pvoid_t p_temp = NULL;

    sctp_msg_op_error_peer_notf_st error_notf;

#ifndef SCTP_STRUCT_BASED

    sctp_U32bit count;

#endif


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_op_error_notf" ) );

    __MEMSET( &error_notf, 0, sizeof( sctp_msg_op_error_peer_notf_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_op_error_peer_notf_st ) - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )&error_notf );

    error_notf.sys_error_info = ( sctp_U8bit * )
                                sctp_client_get_buffer( error_notf.err_length );

    if( error_notf.sys_error_info != NULL )
    {
        p_temp = error_notf.sys_error_info;
        __MEMSET( error_notf.sys_error_info, 0, error_notf.err_length );

        p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                               error_notf.err_length,
                                               ( sctp_U8bit * )error_notf.sys_error_info );
    }

    else
    {
        /* error */
        ret =  SCTP_FAILURE;
    }

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &error_notf.association_id );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &error_notf.err_code );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &error_notf.err_length );

    error_notf.sys_error_info = ( sctp_U8bit * )
                                sctp_client_get_buffer( error_notf.err_length );

    if( error_notf.sys_error_info != NULL )
    {
        p_temp = error_notf.sys_error_info;
        __MEMSET( error_notf.sys_error_info, 0, error_notf.err_length );

        for( count = 0; count < error_notf.err_length; count++ )
            p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                                  ( error_notf.sys_error_info + count ) );

        error_notf.sys_error_info = ( sctp_U8bit * )p_temp;

    }

    else
    {
        /* error */
        ret =  SCTP_FAILURE;
    }

#endif /* SCTP_STRUCT_BASED */

    SCTP_CLIENT_TRACE( ( "API Notification::OP ERROR:Assoc Id%u,Err_Code:%d, \
        err_length:%d",
                         error_notf.association_id,
                         error_notf.err_code, error_notf.err_length ) );

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_OP_ERROR_NOTF, &error_notf );

    if( p_temp != NULL )
    {
        sctp_client_free_buffer( p_temp );
    }

    return( ret );
}


/***************************************************************************
 *  FUNCTION :    sctp_su_proc_dest_status_chg_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_dest_status_chg_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t     ret = SCTP_SUCCESS;
    sctp_dest_id_t    dest_id;
    sctp_src_id_t     src_id;

    sctp_msg_destination_status_chg_notf_st dest_status_chg;

#ifndef SCTP_STRUCT_BASED

    sctp_U16bit u16_temp;

#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_dest_status_chg_notf" ) );

    __MEMSET( &dest_status_chg, 0,
              sizeof( sctp_msg_destination_status_chg_notf_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_destination_status_chg_notf_st ),
                                           ( sctp_U8bit * )&dest_status_chg );
#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &dest_status_chg.association_id );
    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                &dest_status_chg.destination_ip_add );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    dest_status_chg.dest_act_status = ( sctp_sock_spc_event_et )u16_temp;

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Notification::DESTSTATCHANGE:AssocId%u,Dest Addr:",
                         dest_status_chg.association_id ) );
    SCTP_CLIENT_TRACE_ADDR( &dest_status_chg.destination_ip_add );

    SCTP_CLIENT_TRACE( ( " Status:%d", dest_status_chg.dest_act_status ) );

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_DESTINATION_STATUS_CHG_NOTF,
                                  &dest_status_chg );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_endpoint_status_chg_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_endpoint_status_chg_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_trans_id_t transaction_id;
    sctp_U32bit count;

    sctp_msg_endpoint_status_chg_notf_st    endpoint_status_chg;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_endpoint_status_chg_notf" ) );

    __MEMSET( &endpoint_status_chg, 0,
              sizeof( sctp_msg_endpoint_status_chg_notf_st ) );

    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;
    /*  SPR 21127 CHANGES START*/
    SCTP_UNUSED_VARIABLE( transaction_id );
    /*  SPR 21127 CHANGES END*/

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_endpoint_status_chg_notf_st ),
                                           ( sctp_U8bit * )&endpoint_status_chg );

    SCTP_CLIENT_TRACE( ( "API Notification::ENDPOINTSTATCHANGE:Assoc Id%u,\
        Status:%d", endpoint_status_chg.association_id,
                         endpoint_status_chg.endpoint_status ) );

    if( ( endpoint_status_chg.endpoint_status == SCTP_COMMUNICATION_UP ) ||
            ( endpoint_status_chg.endpoint_status == SCTP_RESTART ) )
    {
        count =
            endpoint_status_chg.status_change_data.endpoint_up.num_dest_addrs;
        SCTP_CLIENT_TRACE( ( " No. of destination addresses : %u", count ) );

        SCTP_CLIENT_TRACE( ( " No. of input streams         : %u",
                             endpoint_status_chg.status_change_data.endpoint_up.num_in_streams ) );

        SCTP_CLIENT_TRACE( ( " No. of output streams         : %u",
                             endpoint_status_chg.status_change_data.endpoint_up.
                             num_out_streams ) );

        SCTP_CLIENT_TRACE( ( "List of transport addresses : " ) );

        for( ; count > 0 ; count-- )
        {
            SCTP_CLIENT_TRACE_ADDR(
                &endpoint_status_chg.status_change_data.
                endpoint_up.list_of_addrs[count - 1] );
        }

    }

    else if( endpoint_status_chg.endpoint_status
             == SCTP_ENDPOINT_DOWN_RETRIEVABLE )
    {
        SCTP_CLIENT_TRACE( ( "    Last sent TSN  : %u",
                             endpoint_status_chg.status_change_data.
                             endpoint_retrievable.last_sent_tsn ) );

        SCTP_CLIENT_TRACE( ( "    Last acked TSN : %u",
                             endpoint_status_chg.status_change_data.
                             endpoint_retrievable.last_acked_tsn ) );
    }

#ifdef SCTP_DYNAMIC_IP_SUPPORT

    else if( endpoint_status_chg.endpoint_status == SCTP_ENDPOINT_ADDR_CHNG )
    {
        SCTP_CLIENT_TRACE( ( "Local Address Change Status : %u(%u)",
                             endpoint_status_chg.status_change_data.endpoint_addr_chng.status,
                             endpoint_status_chg.status_change_data.endpoint_addr_chng.response ) );
        SCTP_CLIENT_TRACE( ( "source addresses : " ) );
        SCTP_CLIENT_TRACE_ADDR(
            &endpoint_status_chg.status_change_data.endpoint_addr_chng.
            src_addr );
    }

#endif

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &endpoint_status_chg.association_id );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&endpoint_status_chg.endpoint_status );

    SCTP_CLIENT_TRACE( ( "API Notification::ENDPOINTSTATCHANGE:Assoc Id%u,\
        Status:%d", endpoint_status_chg.association_id,
                         endpoint_status_chg.endpoint_status ) );

    if( ( endpoint_status_chg.endpoint_status == SCTP_COMMUNICATION_UP ) ||
            ( endpoint_status_chg.endpoint_status == SCTP_RESTART ) )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &endpoint_status_chg.status_change_data.endpoint_up.num_dest_addrs );

        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &endpoint_status_chg.status_change_data.endpoint_up.num_in_streams );

        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &endpoint_status_chg.status_change_data.
                                               endpoint_up.num_out_streams );

        count =  endpoint_status_chg.status_change_data.
                 endpoint_up.num_dest_addrs;

        SCTP_CLIENT_TRACE( ( " No. of destination addresses  :    %u", count ) );

        SCTP_CLIENT_TRACE( ( " No. of input streams          :    %d",
                             endpoint_status_chg.status_change_data.endpoint_up.num_in_streams ) );

        SCTP_CLIENT_TRACE( ( " No. of output streams         :    %d",
                             endpoint_status_chg.status_change_data.endpoint_up.
                             num_out_streams ) );

        SCTP_CLIENT_TRACE( ( "List of transport addresses    : " ) );

        for( ; count > 0 ; count-- )
        {
            p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                        &endpoint_status_chg.status_change_data.
                                                        endpoint_up.list_of_addrs[count - 1] );

            SCTP_CLIENT_TRACE_ADDR( &endpoint_status_chg.status_change_data.
                                    endpoint_up.list_of_addrs[count - 1] );

        }
    }

    else if( endpoint_status_chg.endpoint_status
             == SCTP_ENDPOINT_DOWN_RETRIEVABLE )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &endpoint_status_chg.status_change_data.
                                              endpoint_retrievable.last_sent_tsn );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &endpoint_status_chg.status_change_data.
                                              endpoint_retrievable.last_acked_tsn );

        SCTP_CLIENT_TRACE( ( "    Last sent TSN  : %u",
                             endpoint_status_chg.status_change_data.
                             endpoint_retrievable.last_sent_tsn ) );

        SCTP_CLIENT_TRACE( ( "    Last acked TSN : %u",
                             endpoint_status_chg.status_change_data.
                             endpoint_retrievable.last_acked_tsn ) );
    }

#ifdef SCTP_DYNAMIC_IP_SUPPORT

    else if( endpoint_status_chg.endpoint_status == SCTP_ENDPOINT_ADDR_CHNG )
    {
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &endpoint_status_chg.status_change_data.
                                              endpoint_addr_chng.status );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &endpoint_status_chg.status_change_data.
                                              endpoint_addr_chng.response );
#else
        /* CSR#1-4825810 Changes done for warning removal activity
         * for compilation with SUN cc */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&endpoint_status_chg.status_change_data.
                                              endpoint_addr_chng.status );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&endpoint_status_chg.status_change_data.
                                              endpoint_addr_chng.response );
#endif

        p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                    &endpoint_status_chg.status_change_data.
                                                    endpoint_addr_chng.src_addr );

        SCTP_CLIENT_TRACE( ( "Local Address Change Status : %d(%d)",
                             endpoint_status_chg.status_change_data.endpoint_addr_chng.status,
                             endpoint_status_chg.status_change_data.endpoint_addr_chng.response ) );

        SCTP_CLIENT_TRACE( ( "source addresses : " ) );
        SCTP_CLIENT_TRACE_ADDR(
            &endpoint_status_chg.status_change_data.endpoint_addr_chng.
            src_addr );
    }

#endif
#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_ENDPOINT_STATUS_CHG_NOTF,
                                  &endpoint_status_chg );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_congestion_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_congestion_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;

#ifndef SCTP_STRUCT_BASED

    sctp_U16bit u16_temp;

#endif /* SCTP_STRUCT_BASED */

    sctp_msg_congestion_notf_st    cong;


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_congestion_notf" ) );

    __MEMSET( &cong, 0, sizeof( sctp_msg_congestion_notf_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_congestion_notf_st ), ( sctp_U8bit * )&cong );
#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &cong.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    cong.cong_level = ( sctp_cong_level_et )u16_temp;

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Notification::CONGESTION:Assoc Id%u,Cong Level:\
        %d", cong.association_id, cong.cong_level ) );

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_CONGESTION_NOTF, &cong );

    return( ret );
}


/***************************************************************************
 *  FUNCTION :  sctp_su_proc_sys_error_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_sys_error_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;

#ifndef SCTP_STRUCT_BASED

    sctp_U16bit u16_temp;

#endif /* SCTP_STRUCT_BASED */

    sctp_msg_sys_error_notf_st    sys_error;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_sys_error_notf" ) );

    __MEMSET( &sys_error, 0, sizeof( sctp_msg_sys_error_notf_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_sys_error_notf_st ), ( sctp_U8bit * )&sys_error );
#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &sys_error.association_id );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    sys_error.errorno = ( sctp_error_t )u16_temp;

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Notification::SYSERROR:AssocId%u,Cong Level:%d",
                         sys_error.association_id, sys_error.errorno ) );

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_SYS_ERROR_NOTF, &sys_error );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_conn_pending_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_conn_pending_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t    ret = SCTP_SUCCESS;
    sctp_dest_id_t   dest_id;
    sctp_src_id_t    src_id;

    sctp_msg_connection_pending_notf_st    conn_pending;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_conn_pending_notf" ) );

    __MEMSET( &conn_pending, 0, sizeof( sctp_msg_connection_pending_notf_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_connection_pending_notf_st ),
                                           ( sctp_U8bit * )&conn_pending );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &conn_pending.association_id );

    SCTP_CLIENT_TRACE( ( "API Notification::CONNPENDING:AssocId%u",
                         conn_pending.association_id ) );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_CONNECTION_PENDING_NOTF,
                                  &conn_pending );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_data_pending_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_data_pending_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;

    sctp_msg_data_pending_notf_st  data_pending;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_data_pending_notf" ) );

    __MEMSET( &data_pending, 0, sizeof( sctp_msg_data_pending_notf_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

#ifdef SCTP_SEND_DATA_WITH_IND
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_data_pending_notf_st ) - sizeof( sctp_U32bit )
                                           - sizeof( sctp_U32bit ) - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )&data_pending );
#else
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_data_pending_notf_st ),
                                           ( sctp_U8bit * )&data_pending );
#endif /* SCTP_SEND_DATA_WITH_IND */

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &data_pending.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &data_pending.stream_id );
#endif /* SCTP_STRUCT_BASED */

#ifdef SCTP_SEND_DATA_WITH_IND
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &data_pending.ppi );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &data_pending.size );
    data_pending.data = p_req_buffer;
#endif /* SCTP_SEND_DATA_WITH_IND */


    SCTP_CLIENT_TRACE( ( "API Notification::DATAPENDING:Assoc Id%u,Stream Id:\
        %d", data_pending.association_id, data_pending.stream_id ) );

#ifdef SCTP_SEND_DATA_WITH_IND
    SCTP_CLIENT_TRACE( ( "PPI%u,Size:%u",
                         data_pending.ppi, data_pending.size ) );

    SCTP_CL_PRINT_HEX( data_pending.data, data_pending.size );
#endif /* SCTP_SEND_DATA_WITH_IND */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_DATA_PENDING_NOTF,
                                  &data_pending );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_snd_failure_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_snd_failure_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;
    sctp_msg_send_failure_notf_st  snd_failure_notf;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_snd_failure_notf" ) );

    __MEMSET( &snd_failure_notf, 0, sizeof( sctp_msg_send_failure_notf_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_send_failure_notf_st ),
                                           ( sctp_U8bit * )&snd_failure_notf );
#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &snd_failure_notf.association_id );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &snd_failure_notf.context );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&snd_failure_notf.cause_code );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Notification::SENDFAILURE Assoc Id :%u, \
        Context:%u, Cause code:%d", snd_failure_notf.association_id,
                         snd_failure_notf.context, snd_failure_notf.cause_code ) );

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_SEND_FAILURE_NOTF,
                                  &snd_failure_notf );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_part_data_pending_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_part_data_pending_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;

    sctp_msg_partial_datapending_notf_st    part_datapending_notf;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_part_data_pending_notf" ) );

    __MEMSET( &part_datapending_notf, 0,
              sizeof( sctp_msg_partial_datapending_notf_st ) );

    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_partial_datapending_notf_st ),
                                           ( sctp_U8bit * )&part_datapending_notf );
#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &part_datapending_notf.assoc_id );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &part_datapending_notf.stream );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Notification::PARTDATAPENDING Assoc Id :%u, \
        Stream :%d", part_datapending_notf.assoc_id,
                         part_datapending_notf.stream ) );

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_PART_DATA_PENDING_NOTF,
                                  &part_datapending_notf );

    return( ret );
}


/***************************************************************************
 *  FUNCTION :  sctp_su_proc_recv_congestion_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_recv_congestion_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;

#ifndef SCTP_STRUCT_BASED

    sctp_U16bit u16_temp;

#endif /* SCTP_STRUCT_BASED */

    sctp_msg_recv_congestion_notf_st    cong;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_congestion_notf" ) );

    __MEMSET( &cong, 0, sizeof( sctp_msg_recv_congestion_notf_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_recv_congestion_notf_st ), ( sctp_U8bit * )&cong );
#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &cong.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    cong.cong_level = ( sctp_cong_level_et )u16_temp;

#endif /* SCTP_STRUCT_BASED */

    SCTP_CLIENT_TRACE( ( "API Notification::RECV CONGESTION:Assoc Id%u,\
        Cong Level:%d", cong.association_id, cong.cong_level ) );

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_RECV_CONGESTION_NOTF, &cong );

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    return( ret );
}


/***************************************************************************
 *  FUNCTION :  sctp_su_proc_daption_ind_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_adaption_ind_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;

    sctp_msg_adaption_ind_notf_st    adap;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_adaption_ind_notf" ) );

    __MEMSET( &adap, 0, sizeof( sctp_msg_adaption_ind_notf_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_adaption_ind_notf_st ), ( sctp_U8bit * )&adap );
#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &adap.assoc_id );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &adap.adaption_bits );

#endif /* SCTP_STRUCT_BASED */
    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */


    SCTP_CLIENT_TRACE( ( "API Notification::Adaption Ind:Assoc Id%u,"
                         "Adaption ind:%x", adap.assoc_id, adap.adaption_bits ) );

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_ADAPTION_IND_NOTF, &adap );

    return( ret );
}
/***************************************************************************
 *  FUNCTION :  sctp_su_proc_peer_data_drop_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_peer_data_drop_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t    ret = SCTP_SUCCESS;
    sctp_dest_id_t   dest_id;
    sctp_src_id_t    src_id;
#ifndef SCTP_STRUCT_BASED
    sctp_U32bit      count;
#endif


    sctp_msg_peer_data_drop_notf_st     peer_data_drop;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_peer_data_drop_notf" ) );

    __MEMSET( &peer_data_drop, 0, sizeof( sctp_msg_peer_data_drop_notf_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_peer_data_drop_notf_st ),
                                           ( sctp_U8bit * )&peer_data_drop );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &peer_data_drop.association_id );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &peer_data_drop.ecode );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &peer_data_drop.buffer_len );

    for( count = 0; count < SCTP_MAX_MTUSIZE; count++ )
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              ( &( peer_data_drop.buffer[count] ) ) );


    SCTP_CLIENT_TRACE( ( "API Notification::PEER_DATA_DROP:AssocId%u",
                         peer_data_drop.association_id ) );

#endif /* SCTP_STRUCT_BASED */

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_PEER_DATA_DROP_NOTF,
                                  &peer_data_drop );

    return( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_su_proc_shutdown_event_notf
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_su_proc_shutdown_event_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t    ret = SCTP_SUCCESS;
    sctp_dest_id_t   dest_id;
    sctp_src_id_t    src_id;

    sctp_msg_shutdown_event_notf_st    shutdown_event;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_conn_pending_notf" ) );

    __MEMSET( &shutdown_event, 0, sizeof( sctp_msg_shutdown_event_notf_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_shutdown_event_notf_st ),
                                           ( sctp_U8bit * )&shutdown_event );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &shutdown_event.association_id );

    SCTP_CLIENT_TRACE( ( "API Notification::CONNPENDING:AssocId%u",
                         shutdown_event.association_id ) );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start*/
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end*/

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_SHUTDOWN_EVENT_NOTF,
                                  &shutdown_event );

    return( ret );
}


/******************************************************************************
**  Function Name       : sctp_su_proc_lcl_ipc_error_ind_notf
**
**    Description       : Function to IPC Failure notification to SU incase
**                        of TCP Transport.
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
static sctp_return_t sctp_su_proc_lcl_ipc_error_ind_notf(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_dest_id_t  dest_id;
    sctp_src_id_t   src_id;

    sctp_msg_lcl_ipc_error_notf_st    ipc_error_ind;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_su_proc_lcl_ipc_error_ind_notf" ) );

    __MEMSET( &ipc_error_ind, 0, sizeof( sctp_msg_lcl_ipc_error_notf_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_lcl_ipc_error_notf_st ), ( sctp_U8bit * )&ipc_error_ind );

    /*SPR 21107 changes start*/
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CLIENT_TRACE( ( "API Notification::IPC Failure Ind:SCTP Stack Id%u,"
                         "Error Code:%u", ipc_error_ind.dest_stk_id, ipc_error_ind.err_code ) );

    sctp_process_notf_from_stack( src_id, dest_id, SCTP_API_LCL_IPC_ERROR_NOTF,
                                  &ipc_error_ind );

    return( SCTP_SUCCESS );
}

