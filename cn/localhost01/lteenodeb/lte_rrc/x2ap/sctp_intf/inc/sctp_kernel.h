/*******************************************************************************
*
*  FILE NAME   : l_kernel_sctp.h
*
*  DESCRIPTION : This file contains the structure and function declaration
*                required to handle messages from linux based SCTP stack 
*                interface.
*
*  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _L_KERNEL_SCTP_H_
#define _L_KERNEL_SCTP_H_

#ifdef  LKSCTP

#include "commoninc.h"
#include "rrc_x2apOam_intf.h"
#include "x2ap_utils.h"
#include "x2ap_db.h"
/*L3-X2AP Drop2 Integration Start */
#include <sys/socket.h>
#include <netinet/sctp.h>
/*L3-X2AP Drop2 Integration End */
    
#define SCTP_PAYLOAD_PROTOCOL_ID_X2AP 27
#define SCTP_SOCKET_ERROR            (-1)
#define SCTP_ADDRESS_UNREACHABLE           SCTP_ADDR_UNREACHABLE
#define SCTP_ADDRESS_AVAILABLE             SCTP_ADDR_AVAILABLE
#define SCTP_COMMUNICATION_UP              SCTP_COMM_UP
#define SCTP_COMMUNICATION_LOST            SCTP_COMM_LOST
#define SCTP_SHUTDOWN_COMPLETE             SCTP_SHUTDOWN_COMP
#define SCTP_CANT_START_ASSOC              SCTP_CANT_STR_ASSOC
#define SCTP_PARTIAL_DELIVERY              SCTP_PARTIAL_DELIVERY_EVENT
#define SCTP_ADAPTION_EVENT                SCTP_ADAPTATION_INDICATION

/*L3-X2AP Drop2 Integration Start*/
extern fd_set sctp_fd;

fd_set g_sctp_fd_set;
U8 g_sctp_sd_count;     /* Maintaining connection count*/
/* SPR 20633 changes start */
S32 g_sctp_server_fd_ipv4; /* Maintaining server sock
                            for pending conn indication*/
S32 g_sctp_server_fd_ipv6;
/* SPR 20633 changes end */
/*L3-X2AP Drop2 Integration End */

typedef struct{
    struct sctp_initmsg init_msg;
    struct sctp_rtoinfo rtoinfo;
    struct sctp_paddrparams paddr;
    /*sctp parameters start*/
    struct sctp_assocparams assocparams;
    /*sctp parameters stop*/
    /*dscp_marking_start*/
#define X2AP_DSCP_VALUE_PRESENT 0x01
    U16 bitmask;
    U8 x2ap_dscp_value;/*^O,X2AP_DSCP_VALUE_PRESENT,H,0,63^*/
    /*dscp_marking_stop*/
}x2ap_sctp_assoc_conf_info_t;

typedef struct {
    x2_enb_comm_info_t          *p_comm_info;
    x2ap_sctp_fd_info_t         *p_sctp_fd_info;
    x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info;
}x2ap_sctp_conn_info_t;

S32 x2ap_open_sctp_server_setsockopt_wrapper(
        S32 server_sctp_sd,
        x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info,
        /*dscp_marking_start*/
        U8 ipv6_flag);
        /*dscp_marking_stop*/

S32 x2ap_open_sctp_server(
        x2ap_gb_context_t *p_x2ap_gb_ctx,        
        x2ap_sctp_conn_info_t *p_server_info,
        U32 num_peer_enb);

/* SPR 20633 changes start */
S32 x2ap_open_sctp_server_ipv4(
        x2ap_gb_context_t *p_x2ap_gb_ctx,        
        x2ap_sctp_conn_info_t *p_server_info,
        U32 num_peer_enb);
/* SPR 20633 changes end */


S32 x2ap_open_sctp_client_setsockopt_wrapper(
        S32  connSock,
        x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info,
        /*dscp_marking_start*/
        U8 ipv6_flag);
        /*dscp_marking_stop*/

S32 x2ap_open_sctp_client(
        x2ap_sctp_conn_info_t *p_connect_server_info,
        x2ap_sctp_conn_info_t *p_client_info,
        /* SPR_14436_Fix Start */
        x2ap_peer_enb_context_t *p_enb_cntxt_arr);
        /* SPR_14436_Fix Stop */

    /*SPR_16533_START*/
        /* Code Removed */
    /*SPR_16533_END*/
/*****************************************************************************
 *   Function Name     : x2ap_sctp_send
 *   INPUT             : socket_desc       -      It is the socket_desc descriptor of an
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
 *   INPUT            : socket_desc  -   Socket Descriptor of the point
 *   Output           : None.
 *   Returns          : Returns 0 on success or -1 on Error.
 *   Description      : This Function performs the graceful shutdown.
 ******************************************************************************/
/* Coverity Fix 95479 Start */
x2ap_return_et x2ap_close_association( sctp_sd_t socket_desc);
/* Coverity Fix 95479 End */

/******************************************************************************
 *   Function Name    : x2ap_close_socket
 *   INPUT            : socket_desc  -   Socket Descriptor of the point
 *   Output           : None.
 *   Returns          : Returns 0 on success or -1 on Error.
 *   Description      : This Function closes the socket_desc descriptor.
 ******************************************************************************/
S32 x2ap_close_socket(S32 socket_desc);


U32 x2ap_sctp_init_stack(
            U8 cspl_already_intialised,
            x2_ip_addr_t *addr,
            U32 no_of_ip_addr);

/* ifdef for LKSCTP */
#endif

#endif
