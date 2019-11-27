/*******************************************************************************
*
*  FILE NAME   : ari_sctp.h
*
*  DESCRIPTION : This file contains the structure and function declaration
*                required to handle messages from Aricent SCTP stack
*                interface.
*
*  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _ARI_SCTP_H_
#define _ARI_SCTP_H_
#if 0
#ifdef  ARICENT_SCTP_IPR

#include "s_select.h"
#include "commoninc.h"
#include "x2ap_utils.h"
#include "x2ap_db.h"

#define SCTP_PAYLOAD_PROTOCOL_ID_X2AP 27

/* L3-X2AP Integration Activity - Start */

extern sctp_fd_set_st sctp_fd;

/* GDB fix - duplicate symbols at link time */
extern sctp_fd_set_st g_sctp_fd_set;

 /* L3-X2AP Integration Activity - End */

typedef struct{
    sctp_initmsg_st init_msg;
    sctp_rtoinfo_st rtoinfo;
    sctp_paddrparams_st paddr;
    /*sctp parameters start*/
    sctp_assocparams_st assocparams;
    /*sctp parameters stop*/
    /*dscp_marking_start*/
    U16 bitmask;
#define X2AP_DSCP_VALUE_PRESENT 0x01
    /*dscp_marking_stop*/
    /* SPR-22189 START */
    U8              spp_dscp;
    /* SPR-22189 END */
}x2ap_sctp_assoc_conf_info_t;

typedef struct {
    x2_enb_comm_info_t          *p_comm_info;
    x2ap_sctp_fd_info_t         *p_sctp_fd_info;
    x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info;
}x2ap_sctp_conn_info_t;

S32 x2ap_open_sctp_server_sctp_setsockopt(
	S32 server_sctp_sd, 
	x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info);


S32 x2ap_open_sctp_server(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_sctp_conn_info_t *p_server_info,
        U32 num_peer_enb);

/* SPR 20633 changes start */
S32 x2ap_open_sctp_server_ipv4(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_sctp_conn_info_t *p_server_info,
        U32 num_peer_enb);
S32 x2ap_open_sctp_server_both(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_sctp_conn_info_t *p_server_info,
        U32 num_peer_enb);
/* SPR 20633 changes end */


S32 x2ap_open_sctp_client_sctp_setsockopt(
	S32 connSock, 
	x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info);

S32 x2ap_open_sctp_client(
        x2ap_sctp_conn_info_t *p_connect_server_info,/* SPR_14436_Fix Start */
        x2ap_sctp_conn_info_t *p_client_info,
	x2ap_peer_enb_context_t *p_enb_cntxt_arr);/* SPR_14436_Fix Stop */

/*****************************************************************************
 *   Function Name     : x2ap_sctp_send
 *   INPUT             : socket       -      It is the socket descriptor of an
 *                                           SCTP endpoint.
 *                       p_message    -      It is the message to be sent.
 *                       msg_len      -      It is the size of the message or
 *                                           the size of buffer.
 *                       stream_no    -      It is the stream on which data is
 *                                           to be sent.
 *   Output            : None.
 *   Returns           : Returns how many bytes sent.
 *                       Returns -1 on Error.
 *   Description       : This Function transmits data.
 *****************************************************************************/
S32 x2ap_sctp_send(
            S32                    socket_desc,
            const void            *p_message,
            socklen_t              msg_len,
            U32                    stream_no);
/******************************************************************************
 *   Function Name    : x2ap_close_association
 *   INPUT            : socket  -   Socket Descriptor of the point
 *   Output           : None.
 *   Returns          : Returns 0 on success or -1 on Error.
 *   Description      : This Function performs the graceful shutdown.
 ******************************************************************************/
x2ap_return_et x2ap_close_association( sctp_sd_t socket_desc);


/******************************************************************************
 *   Function Name    : x2ap_close_socket
 *   INPUT            : socket  -   Socket Descriptor of the point
 *   Output           : None.
 *   Returns          : Returns 0 on success or -1 on Error.
 *   Description      : This Function closes the socket descriptor.
 ******************************************************************************/
x2ap_return_et x2ap_close_socket(S32 socket_desc);


void* x2ap_sctp_receive(QMODULE *sender, QMODULE *receiver, S8 *priority);
S32 x2ap_sctp_accept(
        S32 socket_desc,
        struct sockaddr ** pp_peerAddrs, 
        U32 * p_addr_count,
/*spr8341 IPv6 fix start*/        
        struct sockaddr_in6   **pp_peerAddr6,
        U8 addr_family);
/*spr8341 IPv6 fix end*/        

/* ifdef for ARICENT_SCTP_IPR */ 
#endif
#endif
#endif
