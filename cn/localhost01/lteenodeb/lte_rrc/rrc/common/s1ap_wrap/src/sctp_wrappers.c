/***************************************************************************
 *
 *  ARICENT 
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: sctp_wrappers.c,v 1.9 2010/10/13 07:02:20 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : This file contains the sctp wrappers functions. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: sctp_wrappers.c,v $
 * Revision 1.9  2010/10/13 07:02:20  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.5.4.5  2010/10/07 13:45:27  gur22047
 * compilation warning removed
 *
 * Revision 1.5.4.4  2010/10/04 13:53:58  gur04518
 * Removed TABS
 *
 * Revision 1.5.4.3  2010/09/15 08:04:51  gur16768
 * updated for inserting steam id into the message
 *
 * Revision 1.5.4.2  2010/09/13 13:03:34  gur18569
 * AFTER MERGE WITH LTE_L3_Rel_2_0_1_1_1 branch
 *
 * Revision 1.5.4.1  2010/08/30 10:38:54  gur22047
 * HO_ut_files_modified
 *
 * Revision 1.5  2010/04/07 15:36:19  gur21006
 * warnings removed
 *
 * Revision 1.4  2010/03/31 06:11:11  gur20470
 * Changed the return type to sctp_sd_t type from U32
 *
 * Revision 1.3  2010/03/24 09:50:40  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2.2.10  2010/03/15 11:58:09  gur18569
 * removed warnings
 *
 * Revision 1.2.2.9  2010/03/05 08:57:58  gur21006
 * char changed to S8, int changed to S32
 *
 * Revision 1.2.2.8  2010/03/05 05:57:54  gur21006
 * warnings removed
 *
 * Revision 1.2.2.7  2010/02/12 15:12:57  gur04518
 * fixed warnings
 *
 * Revision 1.2.2.6  2010/02/02 13:18:15  ukr15916
 * tabs->spaces expanded
 *
 * Revision 1.2.2.5  2010/02/01 07:58:00  gur18569
 * removed tabs
 *
 * Revision 1.2.2.4  2010/01/29 11:21:51  gur21006
 * S1AP_UT_FLAG shifted for sctp_fd
 *
 * Revision 1.2.2.3  2010/01/28 16:04:52  gur20470
 * removed warnings
 *
 * Revision 1.2.2.2  2010/01/28 12:25:09  gur21006
 * lksctp support added
 *
 * Revision 1.2.2.1  2010/01/28 10:53:29  gur20470
 * Removed Compilation Warnings
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.31  2009/12/30 11:39:07  gur21006
 * check added in l3_sctp_receive
 *
 * Revision 1.1.2.30  2009/12/26 12:07:32  gur21006
 * Warnings Removed
 *
 * Revision 1.1.2.29  2009/12/23 06:26:34  gur21006
 * Warnings Removed
 *
 * Revision 1.1.2.28  2009/12/22 14:35:36  gur18569
 * fixed 164
 *
 * Revision 1.1.2.27  2009/12/21 05:15:32  gur21006
 * Removed sctp_bindx
 *
 * Revision 1.1.2.26  2009/12/16 04:43:47  gur18569
 * changed max end point value
 *
 * Revision 1.1.2.25  2009/12/11 06:15:36  gur21006
 * UT trace removed from l3_open_sctp_association
 *
 * Revision 1.1.2.24  2009/12/11 04:44:35  gur21006
 * Fixed SPR - 165
 *
 * Revision 1.1.2.21  2009/12/09 09:25:43  gur21006
 * SPR 163 - Updated log levels
 *
 * Revision 1.1.2.20  2009/12/09 08:17:36  gur21006
 * Notification part updated
 *
 * Revision 1.1.2.19  2009/12/03 10:28:48  ukr18877
 * function s1ap_set_sd_from_header moved from s1ap_utils to sctp_wrappers
 *
 * Revision 1.1.2.18  2009/12/02 07:14:08  gur20470
 * Made l3_sctp_init_stack a non static function
 *
 * Revision 1.1.2.17  2009/12/01 15:09:12  gur18569
 * integration changes
 *
 * Revision 1.1.2.16  2009/11/29 09:16:24  gur18569
 * changes during integration with aricent sctp ipr
 *
 * Revision 1.1.2.15  2009/11/25 14:03:24  gur18569
 * added S1AP_UT_TESTING related changes
 *
 * Revision 1.1.2.14  2009/11/23 07:25:52  gur21006
 * Function defination of l3_sctp_close_association changed again
 *
 * Revision 1.1.2.12  2009/11/20 07:14:56  gur21006
 * function prototype of l3_open_sctp_association modified
 *
 * Revision 1.1.2.11  2009/11/19 06:32:26  gur21006
 * Review comments incorporated
 *
 * Revision 1.1.2.10  2009/11/18 11:05:25  gur20470
 * Modified for UT under flag S1AP_UT_TESTING
 *
 * Revision 1.1.2.9  2009/11/16 10:21:39  gur21006
 * code for setting rto and heart beat interval added
 *
 * Revision 1.1.2.8  2009/11/11 09:53:33  gur21006
 * l3_sctp_close_socket Function added
 *
 * Revision 1.1.2.7  2009/11/09 10:24:53  gur18569
 * using SCTP_FD_SET instead of FD_SET
 *
 * Revision 1.1.2.6  2009/11/06 05:59:56  gur21006
 * Compilation warning removed
 *
 * Revision 1.1.2.5  2009/11/05 12:20:04  gur21006
 * Compilation errors removed
 *
 * Revision 1.1.2.4  2009/11/05 11:15:00  gur21006
 * Comments incorporated
 *
 * Revision 1.1.2.3  2009/11/04 11:09:21  gur21006
 * Compilation errors removed
 *
 * Revision 1.1.2.2  2009/10/28 04:15:37  gur21006
 * Declared Private variables as Static
 *
 * Revision 1.1.2.1  2009/10/26 11:37:38  gur21006
 * Initial Version
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Library Includes
 ***************************************************************************/
#if 0
#ifdef RRC_UNIT_TEST_FRAMEWORK
#include "rrc_ut_stub_db.h"
#endif
#endif
#include "cspl.h"
#include "sctp_wrappers.h"
#include "stacklayer.h"
#include "rrc_defines.h"
#include "rrc_common_utils.h"
#include "s1ap_global_ctx.h"
#include "rrc_msg_mgmt.h"
#include "s1ap_api.h"
#include "s1ap_logger.h"
#include "x2ap_api.h"  /* L3-X2AP Integration Activity */ 
#include <lteMisc.h>
/* GDB fix - duplicate symbols at link time */
U8 g_sctp_sd_count;
/*SPR_16533_START*/
#ifdef LKSCTP
/* SPR 20633 changes start */
S32 g_sctp_server_fd_ipv4 = INVALID_SCTP_SOCKET;
S32 g_sctp_server_fd_ipv6 = INVALID_SCTP_SOCKET;
/* SPR 20633 changes end */
typedef struct 
{
    U32 assoc_id;
    S32 socket;
}g_stored_assoc_info_t;

/*SPR_16533_DELTA1_START*/
g_stored_assoc_info_t g_stored_assoc_info[MAX_PEER_ENB] = 
                      {{0,-1},{0,-1},{0,-1},{0,-1},{0,-1},{0,-1},{0,-1},{0,-1},
                      {0,-1},{0,-1},{0,-1},{0,-1},{0,-1},{0,-1},{0,-1},{0,-1},
                      {0,-1},{0,-1},{0,-1},{0,-1},{0,-1},{0,-1},{0,-1},{0,-1},
                      {0,-1},{0,-1},{0,-1},{0,-1},{0,-1},{0,-1},{0,-1},{0,-1}};
/*SPR_16533_DELTA1_END*/
#endif
/*SPR_16533_END*/
#if 0
#ifdef ARICENT_SCTP_IPR
/* SPR 20633 changes start */
S32 g_sctp_server_fd = INVALID_SCTP_SOCKET;
/* SPR 20633 changes end */

/* GDB Fix - compiler warnings */
extern sctp_return_t sctp_db_validate_config_stack_address( sctp_bindx_flag_et        operation,
                                                            sctp_U32bit               naddr,
                                                            sctp_sockaddr_st          *p_addrlist,
                                                            sctp_error_t              *p_ecode);
#endif
#endif	
#ifdef LINUX_PC_TEST
extern void pctest_send_sctp_close_to_pc(U32 sd);
extern S32 pctest_forward_mme_message_to_pc(    U16          src_module_id,U16 dst_module_id,U16 api_id,U32 socketId,U32 streamId,const void * pmsg, U32 msglen);
#endif
/****************************************************************************
 * Private Variables
 ****************************************************************************/

/* Maintained for sctp sockets*/
static sctp_sd_t sctp_sd_list[MAX_NUM_MME] = {};
#ifdef LKSCTP
U32 init_stack_flag = RRC_FALSE;
#else 
/*SPR 20633 Start*/
U32 init_stack_flag_v4 = RRC_FALSE;
U32 init_stack_flag_v6 = RRC_FALSE;
/*SPR 20633 Start*/
#endif
U32 is_sctp_stck_env_set = RRC_FALSE;
/*SPR 20633 Fix Start*/
U32 sctp_bindx_flag_IPv4 = RRC_FALSE;
U32 sctp_bindx_flag_IPv6 = RRC_FALSE;
/*SPR 20633 Fix End*/

/*sctp_nb_fix_start*/
/*sctp_nb_fix_stop*/
/*SPR_16533_START*/
U32    s1_non_blocking_flag[MAX_NUM_MME] = {};
/*SPR_16533_END*/
static U8 connection_count = 0;     /* Maintaining connection count*/
/**************************************************************************

  Common Functions

***************************************************************************/
/*SPR_16533_START*/
#ifdef LKSCTP
static void s1ap_set_sctp_addr(
    /* SPR 20633 changes start */
    U8 *p_header ,struct sockaddr_storage *from_addr 
    /* SPR 20633 changes end */ 
)
{
    RRC_ASSERT(p_header != PNULL);
    /* SPR 20633 changes start */
	    l3_memcpy_wrapper((p_header),(void *)from_addr,sizeof(struct sockaddr_storage));
    /* SPR 20633 changes end */
}
#endif
/*SPR_16533_END*/

/******************************************************************************
 *   Function Name     : is_sctp_stack_init
 *   Input             : void
 *
 *   Output            : bool
 *   Returns           : RRC_TRUE- SCTP stack is init.
 *                       RRC_FALSE- SCTP stack is not init.
 *   Description       : This func returns whether SCTP stack is init or not.
******************************************************************************/
rrc_bool_et is_sctp_stack_init(void)
{
    /*SPR 20633 Start*/
#ifdef LKSCTP
    if(init_stack_flag == RRC_TRUE)
        return RRC_TRUE;
    else
        return RRC_FALSE;
//#elif ARICENT_SCTP_IPR
#if 0
    if(init_stack_flag_v4 == RRC_TRUE || init_stack_flag_v6==RRC_TRUE)
        return RRC_TRUE;
    else
        return RRC_FALSE;
#endif		
#endif

    /*SPR 20633 End*/
}



/******************************************************************************
 * Function Name  : s1ap_set_stream_from_header
 * Inputs         : p_header - ptr to the message buffer
 *                  value - STREAM value to be set in message buffer
 *
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the consequent two bytes to
 *            set the stream value.
******************************************************************************/
static void s1ap_set_stream_from_header(
    U8 *p_header ,U16 value
)
{
    RRC_ASSERT(p_header != PNULL);
    p_header[0] = (U8) (value & 0xFF00);
    p_header[1] = (U8) (value & 0x00FF);
}

/*****************************************************************************
 * Function Name  : s1ap_set_sctp_stream
 * Inputs         : p_header - ptr to the message buffer
 *                  value - STREAM value to be set in message buffer
 * Outputs        : None
 * Returns        : None
 * Description    : This function calls the internal function that processes
 *            the consequent two bytes to set the stream value.
 ****************************************************************************/
static void s1ap_set_sctp_stream(
    U16 *p_header ,U16 value
)
{
    s1ap_set_stream_from_header(&(((U8 *)p_header)[0]),(U16)value);
}


/******************************************************************************
 * Function Name  : s1ap_set_sd_from_header
 * Inputs         : p_header - ptr to the message buffer
 *                  value - SD value to be set in message buffer
 *
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the consequent four bytes to
 *            set the sd value.
******************************************************************************/
static void s1ap_set_sd_from_header(
    U8 *p_header ,U32 value
)
{
    RRC_ASSERT(p_header != PNULL);
    p_header[0] = (U8) (value & 0xFF000000);
    p_header[1] = (U8) (value & 0x00FF0000);
    p_header[2] = (U8) (value & 0x0000FF00);
    p_header[3] = (U8) (value & 0x000000FF);
}

/*****************************************************************************
 * Function Name  : s1ap_set_sctp_sd
 * Inputs         : p_header - ptr to the message buffer
 *                  value - SD value to be set in message buffer
 * Outputs        : None
 * Returns        : None
 * Description    : This function calls the internal function that processes
 *            the consequent four bytes to set the sd value.
 ****************************************************************************/
static void s1ap_set_sctp_sd(
    U32 *p_header ,S32 value
)
{
    s1ap_set_sd_from_header(&(((U8 *)p_header)[0]),(U32)value);
}

/******************************************************************************
 *   Function Name    : l3_sctp_connectx
 *   Input            : connSock       -   Socket Descriptor of the point
 *                      p_mme_comm_info  -   Pointer to mme_comm_info_t.
 *                                         be listening.
 *   Output           : Connected Socket
 *   Returns          : 0 on Success and -1 on Error
 *   Descrpition      : This Function initiates an association to a peer
 *                      with multiple IP addresses.
 ******************************************************************************/
static S32 l3_sctp_connectx(
        S32       connSock,
        mme_comm_info_t *p_mme_comm_info)
{
    U32 counter = RRC_NULL;
    U8 no_of_ip_addr = RRC_NULL;
    struct sockaddr_in6 servaddr6[MAX_IP_ADDR];
    struct sockaddr_in servaddr[MAX_IP_ADDR];
    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {
        no_of_ip_addr = p_mme_comm_info->num_ipv6_addr;
    }
    else
    {
        no_of_ip_addr = p_mme_comm_info->num_ip_addr;
    }

#ifdef LKSCTP_1_0_11
    sctp_assoc_t sctp_assoc;
    memset_wrapper(&sctp_assoc, 0, sizeof(sctp_assoc_t));
#endif
    /* Specify the peer endpoint(s) to which we'll connect */

    for(counter = 0; counter < no_of_ip_addr; counter++)
    {
        if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)
        {
            bzero_wrapper( (void *)&servaddr6[counter], sizeof(servaddr6[0]) );
            servaddr6[counter].sin6_family     = 10;
            servaddr6[counter].sin6_port        = htons_wrapper(p_mme_comm_info->port);
            RRC_TRACE(RRC_INFO,"MME Server IP ADDRESS = %s and Port = %u", 
                    (const S8*)(p_mme_comm_info->ipv6_addr[counter].ipv6_addr),
                    p_mme_comm_info->port);
            if (inet_pton_wrapper(10,(const char *)p_mme_comm_info->ipv6_addr[counter].ipv6_addr,
                        (void*)&servaddr6[counter].sin6_addr) != 1)
            {
                RRC_TRACE(RRC_WARNING,"l3_sctp_connectx: Couldnot convert the "
                        "INET6 address");
                return -1;
            }
        }
        else
        {
            bzero_wrapper( (void *)&servaddr[counter], sizeof(servaddr[0]) );
            servaddr[counter].sin_family      = (SA_FAMILY_T)AF_INET;
            servaddr[counter].sin_port        = htons_wrapper(p_mme_comm_info->port);
            servaddr[counter].sin_addr.s_addr = 
                inet_addr_wrapper((const char*)(p_mme_comm_info->ip_addr[counter].ip_addr));
            RRC_TRACE(RRC_INFO,"MME Server IP ADDRESS = %s and Port = %u", 
                    (const S8*)(p_mme_comm_info->ip_addr[counter].ip_addr),
                    p_mme_comm_info->port);
        }
    }

    /* Connect to the server */
    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)
    {
#ifdef LKSCTP_1_0_11
        return  sctp_connectx(
                connSock,
                (struct sockaddr *)&servaddr6,
                no_of_ip_addr, &sctp_assoc);
#else 
        return  sctp_connectx(
                connSock,
                (struct sockaddr *)&servaddr6,
                no_of_ip_addr );
#endif
    }
    else
    {
#ifdef LKSCTP_1_0_11
    return  sctp_connectx(
                         connSock,
                         (struct sockaddr *)&servaddr,
                         no_of_ip_addr, &sctp_assoc);
#else
        /* Connect to the server */
        return  sctp_connectx(
                connSock,
                (struct sockaddr *)&servaddr,
                no_of_ip_addr );
#endif
    }

}


/****************************************************************************
 *
 * Aricent IPR functions
 *
 ***************************************************************************/

#if 0
#ifdef ARICENT_SCTP_IPR
    /* L3-X2AP Integration Activity */
    /* static sctp_fd_set_st sctp_fd; */
    sctp_fd_set_st sctp_fd;
/******************************************************************************
 *   Function Name    : l3_sctp_close_association
 *   INPUT            : socket  -   Socket Descriptor of the point
 *   Output           : None.
 *   Returns          : Returns 0 on success or -1 on Error.
 *   Description      : This Function performs the graceful shutdown.
******************************************************************************/
rrc_return_et l3_sctp_close_association( sctp_sd_t socket)
{

    U32 counter = RRC_NULL;
    if(socket < 0)
    {
        RRC_TRACE(RRC_WARNING,"Invalid value of socket received");
        return RRC_FAILURE;
    }
#ifdef LINUX_PC_TEST
	pctest_send_sctp_close_to_pc((U32)socket);
	return RRC_SUCCESS;
#endif
    if (S1AP_SCTP_ERROR == sctp_close(socket))
    {
        RRC_TRACE(RRC_WARNING,"Problem in closing socket");
        return RRC_FAILURE;
    }
    SCTP_FD_CLR((U32)socket,&sctp_fd);

    for(counter = 0; counter < connection_count;counter++)
    {
        if (socket == sctp_sd_list[counter])
        {
            sctp_sd_list[counter] = sctp_sd_list[connection_count - 1];
            sctp_sd_list[connection_count - 1 ] = INVALID_SCTP_SOCKET;
            connection_count--;
            break;
        }
    }

    /* Decrementing count for total connection */

    RRC_TRACE(RRC_INFO,
            "SCTP : connection_count decremented to %d",connection_count);
    return RRC_SUCCESS;
}

/******************************************************************************
 *   Function Name    : l3_sctp_close_socket
 *   INPUT            : socket  -   Socket Descriptor of the point
 *   Output           : None.
 *   Returns          : Returns 0 on success or -1 on Error.
 *   Description      : This Function closes the socket descriptor.
******************************************************************************/
rrc_return_et l3_sctp_close_socket(S32 socket)
{
    if (S1AP_SCTP_ERROR == sctp_close(socket))
    {
        RRC_TRACE(RRC_WARNING,"Problem in closing socket");
        return RRC_FAILURE;
    }
    return RRC_SUCCESS;
}


/*****************************************************************************
 *   Function Name     : l3_sctp_send
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
S32 l3_sctp_send(
        sctp_sd_t              socket,
        const void            *p_message,
        sctp_socklen_t         msg_len,
        U32                    stream_no)
{
#ifdef LATENCY_TRACE_ON
    struct timespec  tp;
    struct tm      *tm;
    clock_gettime_wrapper(CLOCK_PROCESS_CPUTIME_ID, &tp);
    tm = localtime_wrapper(&tp.tv_sec);

    if (start_logging == 1)
    {
        g_latency_data[g_index].send_rcv_flag = RRC_SEND;
        g_latency_data[g_index].src_mod_id = 0xAAAA;
        g_latency_data[g_index].dest_mod_id = 0xAAAA;
        g_latency_data[g_index].api_id = 0xAAAA;
        l3_memcpy_wrapper(&g_latency_data[g_index].val_time, tm, 
                sizeof(g_latency_data[g_index].val_time)); 
        g_latency_data[g_index].time_u_secs = tp.tv_nsec; 
        g_index++;
    }
#endif

    S32    send_result = 0;
    if(socket < 0)
    {
        RRC_TRACE(RRC_FATAL, "l3_sctp_send: Invalid file Descriptor");
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
        return -1;
    }

    if(NULL == p_message)
    {
        RRC_TRACE(RRC_FATAL, "l3_sctp_send: Data to be sent NULL pointer");
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_NULL_POINTER_ERR_CAUSE_ID);
        return -1;
    }

#ifdef MEM_PROFILE_ON
    qvPoolStats();
#endif

    send_result =  sctp_sctp_sendmsg(
            socket,
            p_message,
            msg_len,
            NULL,
            0,
            SCTP_PAYLOAD_PROTOCOL_ID_S1AP,
            0,
            (U16)stream_no,
            0,
            0);
    if(send_result < 0)
    {
        RRC_TRACE(RRC_FATAL, "l3_sctp_send: sctp_sctp_sendmsg failed invalide paramenter");
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
    }
    return send_result;
}




/******************************************************************************
 *   Function Name     : l3_sctp_init_stack
 *   Input             : cspl_already_intialised -  already stack is initialized 
 *                                                   or not
 *                       p_enb_comm_info - pointer to enb_comm_info_t
 *   Output            : None.
 *   Returns           : Returns 0 on Error and 1 on Success.
 *   Description       : This Function Initialises the Aricent's SCTP stack.
******************************************************************************/

U32 l3_sctp_init_stack(
            U8 cspl_already_intialised,
            enb_comm_info_t *p_enb_comm_info)
/*            ip_addr_t *addr, */
/*            U32 no_of_ip_addr) */
{

    sctp_error_t                 sctpErrorCode = E_NOERROR;
    U32 counter = 0;
    sctp_init_options_st         sctpInitOptions;
    U8    no_of_ip_addrs         = RRC_NULL;

/*SPR 20633 Start*/
    U32 ipv6_count = 0;
    if ((RRC_TRUE == init_stack_flag_v4) && (RRC_TRUE == init_stack_flag_v6))
    {
        return SCTP_SUCCESS;
    }
/*SPR 20633 End*/
    SCTP_FD_ZERO(&sctp_fd);


    
    if ((RRC_FALSE == is_sctp_stck_env_set) && 
            (SCTP_FAILURE == 
             sctp_set_stack_envt((sctp_Boolean_t)cspl_already_intialised,
                 PNULL)))
    {
        RRC_TRACE(RRC_WARNING," SCTP Set Env Return Failure");
                return SCTP_FAILURE;
    }
    else
    {
        is_sctp_stck_env_set = RRC_TRUE;
    }


    RRC_TRACE(RRC_INFO," SCTP Set Env set");
    /* Setting all the Init options */
    sctpInitOptions.capability.ecn_capable            = SCTP_FALSE;
    sctpInitOptions.capability.pmtu_capable           = SCTP_FALSE;
    sctpInitOptions.capability.nat_capable            = SCTP_FALSE;
    sctpInitOptions.capability.ack_policy             = 2;
    sctpInitOptions.capability.stale_cookie_policy    = 0;
    sctpInitOptions.capability.stack_type             = SCTP_OVER_INET;

    sctpInitOptions.max_appl                          = 10;
    sctpInitOptions.max_endpoint                      = 190;
    sctpInitOptions.max_tcb                           = 100;
    sctpInitOptions.rand_seed                         = 1;
/*SPR 20633 Start*/
    if((p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT && p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)&&
        ((RRC_FALSE == init_stack_flag_v4) && (RRC_FALSE == init_stack_flag_v6)))
    {
        no_of_ip_addrs                 = p_enb_comm_info->num_ipv6_addr + p_enb_comm_info->num_ip_addr;
        sctpInitOptions.capability.stack_type             = SCTP_OVER_INET6;
    }

    else if((p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT) && (RRC_FALSE == init_stack_flag_v6))
    {
        no_of_ip_addrs                 = p_enb_comm_info->num_ipv6_addr;
        sctpInitOptions.capability.stack_type             = SCTP_OVER_INET6;
    }
    else if((p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT) && (RRC_FALSE == init_stack_flag_v4))
    {
        no_of_ip_addrs                 = p_enb_comm_info->num_ip_addr;
        sctpInitOptions.capability.stack_type             = SCTP_OVER_INET;
    }
    else
    {
                RRC_TRACE(RRC_INFO,"No IP present, neither IPV4 nor IPv6"); //modify by wood
    }

/*SPR 20633 end*/
    sctpInitOptions.no_of_local_addrs                 = no_of_ip_addrs;

/*SPR 20633 Start*/
    if(p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT)
        {
        for (counter = 0; counter < p_enb_comm_info->num_ip_addr; counter++)
            {
                sctpInitOptions.list_of_local_addrs[counter].family = SCTP_AF_INET;
                memset_wrapper(&sctpInitOptions.list_of_local_addrs[counter].ip.v4.addr,0,4);
                RRC_TRACE(RRC_INFO,"EnodeB Client IP addr : %s",p_enb_comm_info->ip_addr[counter].ip_addr);
                sctpInitOptions.list_of_local_addrs[counter].ip.v4.addr =
                    htonl_wrapper(inet_addr_wrapper((char*)(p_enb_comm_info->ip_addr[counter].ip_addr)));
            }
    }
    if(p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {
        for (ipv6_count = 0; ipv6_count < p_enb_comm_info->num_ipv6_addr; ipv6_count++)
            {
                struct in6_addr             ip6_addr;
                sctpInitOptions.list_of_local_addrs[counter].family = 10;
                memset_wrapper(&sctpInitOptions.list_of_local_addrs[counter].ip.v6.addr,0,16);
            RRC_TRACE(RRC_INFO,"EnodeB Client IPV6 addr : %s",p_enb_comm_info->ipv6_addr[ipv6_count].ipv6_addr);
            if (inet_pton_wrapper(10,(const char *) p_enb_comm_info->ipv6_addr[ipv6_count].ipv6_addr, 
                            (void*)&ip6_addr) != 1)
                {
                    RRC_TRACE(RRC_WARNING,"l3_sctp_init_stack:Couldnot convert the "
                            "INET6 address");
                    return (SCTP_FAILURE);
                }
                sctpInitOptions.list_of_local_addrs[counter].ip.v6.addr.in6_u.u6_addr32[0] = ntohl_wrapper(ip6_addr.s6_addr32[0]);
                sctpInitOptions.list_of_local_addrs[counter].ip.v6.addr.in6_u.u6_addr32[1] = ntohl_wrapper(ip6_addr.s6_addr32[1]);
                sctpInitOptions.list_of_local_addrs[counter].ip.v6.addr.in6_u.u6_addr32[2] = ntohl_wrapper(ip6_addr.s6_addr32[2]);
                sctpInitOptions.list_of_local_addrs[counter].ip.v6.addr.in6_u.u6_addr32[3] = ntohl_wrapper(ip6_addr.s6_addr32[3]);
            counter++;

            }
        }
/*SPR 20633 End*/
    sctpInitOptions.MaxPerAssoc.streams        = 10;
    sctpInitOptions.MaxPerAssoc.min_tx_buffers = 50;
    sctpInitOptions.MaxPerAssoc.med_tx_buffers = 200;
    sctpInitOptions.MaxPerAssoc.med_tx_buffers = 200;
    sctpInitOptions.MaxPerAssoc.max_tx_buffers = 20;
    sctpInitOptions.MaxPerAssoc.min_rx_buffers = 150;
    sctpInitOptions.MaxPerAssoc.med_rx_buffers = 70;
    sctpInitOptions.MaxPerAssoc.max_rx_buffers = 20;

    sctpInitOptions.min_reserve_buffers = 10;
    sctpInitOptions.med_reserve_buffers = 10;
    sctpInitOptions.max_reserve_buffers = 10;

    sctpInitOptions.standard_udp_port = 0 ;

    sctpInitOptions.checksum = SCTP_CRC32;

    sctpInitOptions.hostname = NULL;

    if( SCTP_SUCCESS == sctp_api_init_stack(&sctpInitOptions, &sctpErrorCode))
    {
/*SPR 20633 Start*/
        if(no_of_ip_addrs > 0 && p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_ADDR_PRESENT)
        {
            init_stack_flag_v6 = RRC_TRUE;
            init_stack_flag_v4 = RRC_TRUE;
        }
        else if(no_of_ip_addrs > 0)
        {
            init_stack_flag_v4 = RRC_TRUE;
        }
/*SPR 20633 End*/
        return SCTP_SUCCESS;
    }
    else
    {
        return SCTP_FAILURE;
    }
}

/******************************************************************************
 *   Function Name     : open_sctp_assoc_init_sctp_stack
 *   Input             : p_enb_comm_info
 *   Output            : None.
 *   Returns           : Returns -1 on Error and 1 on Success.
 *   Description       : This Function Initialises the SCTP stack at
 *                       open_sctp_assoc
******************************************************************************/
static S8 open_sctp_assoc_init_sctp_stack(
        enb_comm_info_t *p_enb_comm_info)
{
    U16  mme_count     = RRC_NULL;
    U8 cspl_already_initialised = 1;
    sctp_sockaddr_st       addr_list[MAX_NUM_IP_ADDR];
    U16 addr_count = RRC_NULL;
    ipv6_addr_t *p_ipv6_addr = NULL;
    sctp_error_t           ecode;
    ip_addr_t             *addr;

    RRC_S1AP_UT_TRACE_ENTER();
/*SPR 20633 Start*/
    if (RRC_FALSE == init_stack_flag_v4 && RRC_FALSE == init_stack_flag_v6 )
/*SPR 20633 Start*/
    {
        RRC_TRACE(RRC_INFO," Initializing sctp_sd_list..");
        /*Initiating sctp_sd_list*/
        for (mme_count = 0 ; mme_count < MAX_NUM_MME ; mme_count++ )
        {
            sctp_sd_list[mme_count] = INVALID_SCTP_SOCKET; 
        }
        if (SCTP_FAILURE == l3_sctp_init_stack(cspl_already_initialised,
                    p_enb_comm_info))
        {

            RRC_TRACE(RRC_WARNING,"SCTP INIT Failure, error no: %d\n", sk_errno);
            return S1AP_SCTP_ERROR;
        }
        else
        {

            RRC_TRACE(RRC_INFO," SCTP INIT Success\n");
        }
    }
    /* L3-S1AP Integration Activity */
    else
    {
        if(p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_ADDR_PRESENT)
        {
            memset_wrapper(addr_list, 0, sizeof(addr_list));
            for (addr_count =0; addr_count < p_enb_comm_info->num_ipv6_addr;addr_count++)
            {
                p_ipv6_addr = &(p_enb_comm_info->ipv6_addr[addr_count]);
                addr_list[addr_count].family = 10;
                if (inet_pton_wrapper(10,(const char *)p_ipv6_addr[0].ipv6_addr,
                            (void*)&addr_list[addr_count].ip.v6.addr) != 1)
                {
                    RRC_TRACE(RRC_WARNING,"l3_open_sctp_association: Couldn't convert INET6 address");
                    return S1AP_SCTP_ERROR;
                }

                addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[0] = ntohl_wrapper(addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[0]);
                addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[1] = ntohl_wrapper(addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[1]);
                addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[2] = ntohl_wrapper(addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[2]);
                addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[3] = ntohl_wrapper(addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[3]);
            }

            if(sctp_db_validate_config_stack_address(SCTP_BINDX_ADD_ADDR, (U32)p_enb_comm_info->num_ipv6_addr, 
                        addr_list, &ecode))
            {
                RRC_TRACE(RRC_INFO,"Calling sctp_api_config_stack_addresses()");
                if(SCTP_FAILURE == sctp_api_config_stack_addresses(SCTP_BINDX_ADD_ADDR,
                            (U32)p_enb_comm_info->num_ipv6_addr,
                            addr_list,
                            &ecode))
                {
                    RRC_TRACE(RRC_WARNING,"SCTP configuration parameter failure, error no: %d", sk_errno);
                    return S1AP_SCTP_ERROR;
                }
                else
                {
                    RRC_TRACE(RRC_DETAILED,"sctp_api_config_stack_addresses() SUCCESS");
                }
            }

        }

        else
        {
            memset_wrapper(addr_list, 0, sizeof(addr_list));
            for (addr_count =0; addr_count < p_enb_comm_info->num_ip_addr;addr_count++)
            {
                addr = &(p_enb_comm_info->ip_addr[addr_count]);
                addr_list[addr_count].family = SCTP_AF_INET;
                addr_list[addr_count].ip.v4.addr = htonl_wrapper(inet_addr_wrapper((const char*)(addr[0].ip_addr)));
            }

            if(sctp_db_validate_config_stack_address(SCTP_BINDX_ADD_ADDR, (U32)p_enb_comm_info->num_ip_addr, addr_list, &ecode))
            {
                RRC_TRACE(RRC_INFO,"Calling sctp_api_config_stack_addresses()");
                if(SCTP_FAILURE == sctp_api_config_stack_addresses(SCTP_BINDX_ADD_ADDR,
                            (U32)p_enb_comm_info->num_ip_addr,
                            addr_list,
                            &ecode))
                {
                    RRC_TRACE(RRC_WARNING,"SCTP configuration parameter failure, error no: %d", sk_errno);
                    return S1AP_SCTP_ERROR;
                }
                else
                {
                    RRC_TRACE(RRC_DETAILED,"sctp_api_config_stack_addresses() SUCCESS");
                }
            }
        }
    }
    RRC_S1AP_UT_TRACE_EXIT();
    return 1;
}

/******************************************************************************
 *   Function Name     : l3_open_sctp_association
 *   Input             : p_mme_comm_info   -      Pointer to MME related
 *                                                communication Information.
 *                       p_enb_comm_info   -      Pointer to eNB related
 *                                                communication Information.
 *   Output            : Connected Socket Descriptor.
 *   Returns           : Returns connected socket descriptor on Success or -1 
 *                       on Error.
 *   Description       : This Function performs the sequence of steps to gain a
 *                       connected socket.
******************************************************************************/
S32 l3_open_sctp_association(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        mme_context_t *p_mme_ctx,
        enb_comm_info_t *p_enb_comm_info)
{
    U16  counter       = RRC_NULL;
    S32  connSock      = RRC_NULL;
    mme_comm_info_t *p_mme_comm_info = &(p_mme_ctx->connection_info);
    ip_addr_t *p_mme_addr = &(p_mme_comm_info->ip_addr[0]);
    U16 mme_port = p_mme_comm_info->port;
    U32 nostreams = p_mme_comm_info->num_streams;
    U32 nistreams = p_mme_comm_info->num_streams;
    U8 no_mme_ip_addr = RRC_NULL;//p_mme_comm_info->num_ip_addr;
    U32 heartbeat_int = p_mme_comm_info->heart_beat_timer;
    S32 return_value  = RRC_NULL;
    U8 number_of_streams = RRC_NULL;
    U32 noDelay = 1;
    struct sockaddr_in6 servaddr6[MAX_IP_ADDR];
    struct sockaddr_in servaddr[MAX_IP_ADDR];
    ipv6_addr_t *p_mme_addr_ipv6 = NULL;
/*sctp parameters start*/
    s1ap_sctp_conf_info_t *p_sctp_params = PNULL;
/*sctp parameters stop*/
/*Bug 9050 fixes start*/
    U32 blocked = 1;
/*Bug 9050 fixes end*/
 /*SPR 15570 Fix Start*/
    p_mme_ctx->is_conn_ongoing = RRC_FALSE;
 /*SPR 15570 Fix Stop*/


    RRC_S1AP_UT_TRACE_ENTER();
    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)
    {
        p_mme_addr_ipv6 = &(p_mme_comm_info->ipv6_addr[0]);
    }
    struct sctp_event_subscribe events;
    struct sctp_paddrparams paddr;


    S8 family        = SCTP_AF_INET;

    sctp_socklen_t optlen = 0;
    sctp_initmsg_st initmsg;
    sctp_rtoinfo_st rtoinfo;
    /*sctp parameters start*/
    sctp_assocparams_st assocparams;
    /*sctp parameters stop*/
    /* dscp_marking_start */
    #ifdef SCTP_IP_HEADER_PARAMS
    sctp_assoc_ip_hdr_params_st header_params;
    #endif
    /* dscp_marking_stop */

    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {
        no_mme_ip_addr = p_mme_comm_info->num_ipv6_addr;
    }
    else
    {
        no_mme_ip_addr = p_mme_comm_info->num_ip_addr;
    }
    /*sctp parameters start */
    if(RRC_S1AP_SCTP_CONFIG_PARAMS_PRESENT == p_s1ap_gb_ctx->bitmask)
    {
        p_sctp_params = &(p_s1ap_gb_ctx->s1ap_sctp_conf_param);
    }
    /*sctp parameters stop */
    
    /* Initializing the SCTP stack */
    if ( 1 != open_sctp_assoc_init_sctp_stack(p_enb_comm_info))
    {
        return S1AP_SCTP_ERROR;
    }

    /* Create an SCTP TCP-Style Socket */
    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {
        connSock = sctp_socket( 10, SCTP_SOCK_STREAM, IPPROTO_SCTP);
    }
    else
    {
        connSock = sctp_socket( SCTP_AF_INET, SCTP_SOCK_STREAM, IPPROTO_SCTP);
    }


    if (connSock < 0)
    {
        RRC_TRACE(RRC_WARNING,"SCTP : sctp_socket failed");
        return S1AP_SCTP_ERROR;
    }

/*Bug 9050 fixes start*/
    if (sctp_setsockopt(connSock,SOL_SOCKET,
                            SCTP_SO_NONBLOCKING, &blocked, sizeof(U32)) < 0)
    {
            RRC_TRACE(RRC_WARNING,"SCTP : sctp_setsockopt failed for non blocking socket ");
            RRC_TRACE(RRC_WARNING,"SCTP : non blocking-error no: %d", sk_errno);
            l3_sctp_close_socket(connSock);
            return S1AP_SCTP_ERROR;
    }
    else
    {
            RRC_TRACE(RRC_WARNING,"SCTP : sctp_setsockopt SUCCESS for non blocking socket");
    }
/*Bug 9050 fixes end*/

    /* Populating Structure for SCTP 4-way handshake */
    memset_wrapper(&initmsg, 0, sizeof(initmsg));

    initmsg.sinit_num_ostreams  = nostreams;
    initmsg.sinit_max_instreams = nistreams;
    /*sctp parameters start*/
    initmsg.sinit_max_init_timeo = SCTP_DEFAULT_INIT_TIMEOUT; 
    if(PNULL != p_sctp_params)
    {
        initmsg.sinit_max_attempts = p_sctp_params->init_max_attempts;
    }
    else
    {
        initmsg.sinit_max_attempts  = RRC_SCTP_DEFAULT_MAX_INIT_RTX;
    }
    /*sctp parameters stop*/
    return_value = sctp_setsockopt(
                                   connSock,
                                   IPPROTO_SCTP,
                                   SCTP_SO_INITMSG,
                                   &initmsg,
                                   sizeof(initmsg));

    if (return_value < 0)
    {
        RRC_TRACE(RRC_WARNING,"SCTP : sctp_setsockopt for INIT failed");
        l3_sctp_close_socket(connSock);
        return S1AP_SCTP_ERROR;
    }

    RRC_TRACE(RRC_INFO,"SCTP : setsockopt for INIT success");

    /*sctp parameters start*/
    /* Populating Structure for SCTP 4-way handshake */
    memset_wrapper(&assocparams, 0, sizeof(assocparams));

    /*code review comments */
    assocparams.sasoc_assoc_id = (U32)connSock;
    
    if(PNULL != p_sctp_params)
    {
            assocparams.sasoc_asocmaxrxt = p_sctp_params->assoc_max_retrans;
            assocparams.sasoc_cookie_life = p_sctp_params->valid_cookie_life;
    }
    else
    {
        assocparams.sasoc_asocmaxrxt = RRC_SCTP_DEFAULT_ASSOC_MAX_RTX;
        assocparams.sasoc_cookie_life = RRC_SCTP_DEFAULT_VAL_COOKIE_LIFE;

    }
    return_value = sctp_setsockopt(
            connSock,
            IPPROTO_SCTP,
            SCTP_SO_ASSOCINFO,
            &assocparams,
            sizeof(assocparams));

    if (return_value < 0)
    {
        RRC_TRACE(RRC_WARNING,"SCTP : sctp_setsockopt for assocparams failed");
        l3_sctp_close_socket(connSock);
        return S1AP_SCTP_ERROR;
    }
    RRC_TRACE(RRC_INFO,"SCTP : setsockopt for assocparams Success");

    /*sctp parameters end*/

    /* dscp_marking_start */
#ifdef SCTP_IP_HEADER_PARAMS
    if( p_s1ap_gb_ctx->s1ap_sctp_conf_param.bitmask & S1AP_OAM_DSCP_VALUE_PRESENT )
    {       
            memset_wrapper(&header_params, 0, sizeof(sctp_assoc_ip_hdr_params_st));

            header_params.sip_assoc_id = (U32)connSock;
            if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
            {
   	            header_params.sip_tclass = p_s1ap_gb_ctx->s1ap_sctp_conf_param.s1ap_dscp_value << 2;
            }
            else
            {
   	            header_params.sip_tos = p_s1ap_gb_ctx->s1ap_sctp_conf_param.s1ap_dscp_value << 2;
                                                    // tos is now replaced by higher 6 bits of DSCP value and lower 2 bits are of ECN  
            }
	        return_value = sctp_setsockopt(
                                   connSock,
                                   IPPROTO_SCTP,
                                   SCTP_IP_DSCP,
                                   &header_params,
                                   sizeof(sctp_assoc_ip_hdr_params_st));

	        if (return_value < 0)
	        {
		        RRC_TRACE(RRC_WARNING,"sctp_api_config_assoc_ctl  failed : dscp value is not set ");
		        l3_sctp_close_socket(connSock);
		        return S1AP_SCTP_ERROR;
	        }
	        else
	        {
		        RRC_TRACE(RRC_INFO," dscp value is succeefully set by : %d",
				        p_s1ap_gb_ctx->s1ap_sctp_conf_param.s1ap_dscp_value);
	        }
    }
#endif
    /* dscp_marking_stop */
    /* SPR 20689 Fix Start */
    /*Code Deleted */
    /* SPR 20689 Fix Stop */
    return_value = sctp_getsockopt(
                                   connSock,
                                   IPPROTO_SCTP,
                                   SCTP_SO_INITMSG,
                                   &initmsg,
                                   &optlen);
    if (return_value < 0)
    {
        RRC_TRACE(RRC_ERROR,"SCTP : Unable to retreive Number of streams");
        return S1AP_SCTP_ERROR;
    }
    else
    {
        number_of_streams = (U8)initmsg.sinit_num_ostreams;
        if(initmsg.sinit_num_ostreams  <  2 ||
                initmsg.sinit_max_instreams <  2)
        {
            RRC_TRACE(RRC_WARNING,
                    "SCTP : Less Number of streams have been opened");
            return S1AP_SCTP_ERROR;
        }
        else if (initmsg.sinit_num_ostreams  >  10 )
        {
            RRC_TRACE(RRC_WARNING,"SCTP : More than 10 streams are open");
            return S1AP_SCTP_ERROR;
        }
        else
        {
            (p_mme_ctx->stream_info).number_of_streams = number_of_streams;
            for (counter = 0; counter < number_of_streams; counter++)
            {
                (p_mme_ctx->stream_info).stream_ids[counter] = counter;
            }

        }
    }

    memset_wrapper(&rtoinfo, 0, sizeof(rtoinfo));

    rtoinfo.srto_assoc_id = (U32)connSock;
    /*sctp parameters start*/
    if(PNULL != p_sctp_params)
    {
        rtoinfo.srto_initial = p_sctp_params->rto_initial;
        rtoinfo.srto_max = p_sctp_params->rto_max;
        rtoinfo.srto_min = p_sctp_params->rto_min;
    }
    else
    {
        rtoinfo.srto_initial = RRC_SCTP_DEFAULT_RTO_INITIAL;
        rtoinfo.srto_max = RRC_SCTP_DEFAULT_RTO_MAX;
        rtoinfo.srto_min = RRC_SCTP_DEFAULT_RTO_MIN;
    }
    /*sctp parameters end*/
    return_value =  sctp_setsockopt( connSock,IPPROTO_SCTP , SCTP_SO_RTOINFO,
            (const void *)&rtoinfo, sizeof(rtoinfo));

    if (return_value < 0)
    {
        RRC_TRACE(RRC_WARNING,"SCTP : RTO Interval is not set.");
        l3_sctp_close_socket(connSock);
        return S1AP_SCTP_ERROR;

    }

    RRC_TRACE(RRC_INFO,"SCTP : RTO Interval is set.");

    /* SPR 20689 Fix Start */
    memset_wrapper( (void *)&events, 0, sizeof(events) );

    /* Populating SCTP Events structure*/
    events.sctp_association_event  = 1;
    events.sctp_data_io_event      = 1;
    events.sctp_address_event      = 1;
    events.sctp_send_failure_event = 1;
    events.sctp_peer_error_event   = 1;
    events.sctp_shutdown_event     = 1;

    return_value = sctp_setsockopt(connSock, IPPROTO_SCTP, 
            SCTP_SO_SET_EVENTS, &events, sizeof (events));

#if 0
#ifdef RRC_UNIT_TEST_FRAMEWORK
    return_value = 1;
#endif
#endif
    if (return_value < 0)
    {
        RRC_TRACE(RRC_WARNING,
                "SCTP : sctp_setsockopt for setting events failed");
        l3_sctp_close_socket(connSock);
        return S1AP_SCTP_ERROR;
    }
    RRC_TRACE(RRC_INFO,"SCTP : sctp_setsockopt for setting events Success");
    /* Getting connected with the Peer with multiple IPs */
    return_value = 
        l3_sctp_connectx(connSock,p_mme_comm_info);
    if (return_value < 0)
    {
        /*SPR 15570 Fix Start*/
        if(EINPROGRESS == errno)
        {
            RRC_TRACE(RRC_WARNING,
                    "SCTP : l3_sctp_connectx in progress:- [%d]",errno);
            p_mme_ctx->is_conn_ongoing = RRC_TRUE;

            return connSock ;
        }
        else
        {
            RRC_TRACE(RRC_WARNING,
                    "SCTP : l3_sctp_connectx failed, Error Code [%d]",sk_errno);
            l3_sctp_close_socket(connSock);
            return S1AP_SCTP_ERROR ;
        }
        /*SPR 15570 Fix Stop*/
    }


    RRC_TRACE(RRC_INFO,"SCTP : connectx Success");
    /* SPR 20689 Fix Stop */

    for (counter = 0; counter < no_mme_ip_addr; counter++)
    {
        memset_wrapper(&paddr, 0, sizeof(struct sctp_paddrparams));

        if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)
        {
            bzero_wrapper( (void *)&servaddr6[0], sizeof(servaddr6[0]) );
            servaddr6[counter].sin6_family     = 10;
            servaddr6[counter].sin6_port        = htons_wrapper(mme_port);
            if(inet_pton_wrapper(10,(const char *)p_mme_addr_ipv6->ipv6_addr, 
                        (void*)&servaddr6[counter].sin6_addr)!=1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert INET6 adress while setting HeartBeat");
                return S1AP_SCTP_ERROR;
            }
            paddr.spp_address = *(sctp_sockaddr_storage_st *)&servaddr6[0];
        p_mme_addr_ipv6++;
        }
        else
        {

            bzero_wrapper( (void *)&servaddr[0], sizeof(servaddr[0]) );
            servaddr[counter].sin_family      = (SA_FAMILY_T)family;
            servaddr[counter].sin_port        = htons_wrapper(mme_port);
            servaddr[counter].sin_addr.s_addr = 
                inet_addr_wrapper((const char*) p_mme_addr->ip_addr );
            paddr.spp_address = *(sctp_sockaddr_storage_st *)&servaddr[0];
        p_mme_addr++;
        }

        paddr.spp_assoc_id                = (U32)connSock;
        paddr.spp_hbinterval              = heartbeat_int;
        /*sctp parameters start*/
        if(PNULL != p_sctp_params)
        {
            paddr.spp_pathmaxrxt = p_sctp_params->path_max_retrans;
        }
        else
        {
            paddr.spp_pathmaxrxt = RRC_SCTP_DEFAULT_PATH_MAX_RTX;
        }
        /*sctp parameters end*/

        return_value= sctp_setsockopt( connSock,IPPROTO_SCTP , SCTP_PEER_ADDR_PARAMS,
                           (const void *)&paddr, sizeof(paddr));

        if (return_value < 0)
        {
            RRC_TRACE(RRC_WARNING,"SCTP : HeartBeat Interval is not set.");
            l3_sctp_close_socket(connSock);
            return S1AP_SCTP_ERROR;
        }
        RRC_TRACE(RRC_INFO,"SCTP : HeartBeat Interval is set.");
    }


    /* SPR 20689 Fix Start */
    /*Code Deleted*/
    /* SPR 20689 Fix Stop */
    return_value =  sctp_setsockopt(
                       connSock,
                       IPPROTO_SCTP,
                       SCTP_SO_NODELAY,
                       &noDelay,
                       sizeof(noDelay));
    if (return_value < 0) 
    {    
        RRC_TRACE(RRC_WARNING,"SCTP :"
                " sctp_setsockopt(NODELAY) failed");
        l3_sctp_close_socket(connSock);
        return S1AP_SCTP_ERROR;
    }


    

    for(counter = 0; counter < MAX_NUM_MME;counter++)
    {
        if (INVALID_SCTP_SOCKET == sctp_sd_list[counter])
        {
            sctp_sd_list[counter] = connSock;
            connection_count++;
            break;
        }
    }


    RRC_TRACE(RRC_INFO,
            "SCTP : connection_count incremented to %d",connection_count);
    SCTP_FD_SET((U32)connSock,&sctp_fd);
    return connSock;

}

#ifdef LTE_EMBMS_SUPPORTED
/******************************************************************************
 *   Function Name     : handle_m2ap_sctp_fd
 *   Input             : p_notification
 *   Output            : p_api_id
 *   Returns           : Returns 0 on Error and 1 on Success.
 *   Description       : This Function handles the m2ap sctp fd
 ******************************************************************************/
static U8 handle_m2ap_sctp_fd(
        sctp_notification_ut *p_notification,
        U16 *p_api_id)
{
    /* Notification message types */
    sctp_paddr_change_st   * p_sn_padr_change = PNULL;
    sctp_assoc_change_st   * p_sn_assoc_change = PNULL;

    switch (p_notification->
            sn_header.sn_type)
    {
        case SCTP_PEER_ADDR_CHANGE:
        {
            RRC_TRACE(RRC_INFO,"Notification message of type "
                    "SCTP_PEER_ADDR_CHANGE for spc_assoc_id %d\n",
                    p_notification->sn_padr_change.spc_assoc_id);

            p_sn_padr_change = &(p_notification->sn_padr_change);

            switch (p_sn_padr_change->spc_state)

            {
                case SCTP_ADDRESS_UNREACHABLE:
                {
                    RRC_TRACE(RRC_INFO,
                            "Address Unreachable for spc_assoc_id %d\n",
                            p_sn_padr_change->spc_assoc_id);
                    break;
                }
                case SCTP_ADDRESS_AVAILABLE:
                {
                    RRC_TRACE(RRC_INFO,
                            "Peer is available again!\n");
                    *p_api_id = M2AP_SCTP_CONN_RECOVERY_IND;
                    return 0;
                }

                default:
                {
                    RRC_TRACE(RRC_INFO,"Unknown!!!!!!!\n");
                    return 0;
                }
            }

            break;

        }
        case SCTP_ASSOC_CHANGE:
        {
            RRC_TRACE(RRC_INFO,"Notification message of type "
                    "SCTP_ASSOC_CHANGE for sac_assoc_id %d\n",
                    p_notification->sn_assoc_change.sac_assoc_id);

            p_sn_assoc_change = &(p_notification->
                    sn_assoc_change);

            switch(p_sn_assoc_change->sac_state)
            {
                case SCTP_COMMUNICATION_UP:
                {
                    RRC_TRACE(RRC_INFO,
                            "SCTP: A new association has just been started");
                    *p_api_id = M2AP_SCTP_ASSOC_UP_EVENT;
                    break;
                }
                case SCTP_COMMUNICATION_LOST:
                {
                    RRC_TRACE(RRC_INFO,
                            "SCTP: An SCTP association has" 
                            "just been lost for sac_assoc_id %d",p_sn_assoc_change->sac_assoc_id);
                    *p_api_id = M2AP_SCTP_CONN_FAILURE_IND;
                    break;
                }
                case SCTP_RESTART:
                {
                    RRC_TRACE(RRC_INFO,
                            "SCTP: Peer associated with the sac_assoc_id %d"\
                            " has been restarted ",p_sn_assoc_change->sac_assoc_id);
                    *p_api_id = M2AP_SCTP_CONN_RECOVERY_IND;
                    break;
                }
                case SCTP_SHUTDOWN_COMPLETE:
                {
                    RRC_TRACE(RRC_INFO,
                            "SCTP: Shutdown completed with the peer entity"\
                            " associated with the sac_assoc_id %d",p_sn_assoc_change->sac_assoc_id);
                    return 0;
                }
                case SCTP_CANT_START_ASSOC:
                {
                    RRC_TRACE(RRC_INFO,
                            "SCTP: The peer did not respond to an association"\
                            " setup attempt");
                    RRC_TRACE(RRC_WARNING,
                            "SCTP: : SCTP_CANT_START_ASSOC");
                    *p_api_id = M2AP_SCTP_ASSOC_DOWN_EVENT;
                    break;
                }


                /* Used only in HSS specific interface */
                case SCTP_ENDPOINT_DOWN_RETRIEVABLE:
                {
                    RRC_TRACE(RRC_INFO,
                            "SCTP: SCTP_ENDPOINT_DOWN_RETRIEVABLE ");
                    return 0;
                }

                case SCTP_ENDPOINT_ADDR_CHNG:
                {
                    RRC_TRACE(RRC_INFO,
                            "SCTP: SCTP_ENDPOINT_ADDR_CHNG");
                    return 0;
                }
                default:
                {
                    RRC_TRACE(RRC_WARNING,
                            "Unknown State for received Notification");
                    return 0;
                }
            }/* End of switch */
            break;
        }
        case SCTP_REMOTE_ERROR:
        {
            RRC_TRACE(RRC_WARNING,"SCTP: SCTP REMOTE ERROR");
            return 0;
        }

        case SCTP_SEND_FAILED:
        {
            RRC_TRACE(RRC_WARNING,"SCTP: SCTP SEND FAILED");
            return 0;

        }
        case SCTP_SHUTDOWN_EVENT:
        {
            RRC_TRACE(RRC_WARNING,
                    "SCTP: Peer entity associated with sse_assoc_id %d"\
                    " has initiated SHUTDOWN", p_notification->sn_shutdown_event.sse_assoc_id);
            *p_api_id = M2AP_SCTP_CONN_SHUTDOWN_EVENT;
            break;
        }
        case SCTP_ADAPTION_EVENT:
        {
            RRC_TRACE(RRC_WARNING,"SCTP: SCTP ADAPTION EVENT ");
            return 0;
        }
        case SCTP_PARTIAL_DELIVERY:
        {
            RRC_TRACE(RRC_WARNING,"SCTP PARTIAL DELIVERY");
            return 0;
        }


        default:
        {
            RRC_TRACE(RRC_INFO,"Unknown Notification!!!!!!!\n");
            return 0;
        }
    }
    return 1;
}
#endif

/******************************************************************************
 *   Function Name     : handle_x2ap_sctp_fd
 *   Input             : p_notification
 *   Output            : p_api_id
 *   Returns           : Returns 0 on Error and 1 on Success.
 *   Description       : This Function handles the x2ap sctp fd
******************************************************************************/
static U8 handle_x2ap_sctp_fd(
        sctp_notification_ut *p_notification,
        U16 *p_api_id)
{
    /* Notification message types */
    sctp_paddr_change_st   * p_sn_padr_change = PNULL;
    sctp_assoc_change_st   * p_sn_assoc_change = PNULL;

    switch (p_notification->
            sn_header.sn_type)
    {
        case SCTP_PEER_ADDR_CHANGE:
            {
                RRC_TRACE(RRC_INFO,"Notification message of type "
                        "SCTP_PEER_ADDR_CHANGE for spc_assoc_id %d",
                        p_notification->sn_padr_change.spc_assoc_id);

                p_sn_padr_change = &(p_notification->sn_padr_change);

                switch (p_sn_padr_change->spc_state)
                {
                    case SCTP_ADDRESS_UNREACHABLE:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "Address Unreachable for spc_assoc_id %d",
                                    p_sn_padr_change->spc_assoc_id);
                            *p_api_id = X2AP_SCTP_CONN_FAILURE_IND;
                            break;
                        }
                    case SCTP_ADDRESS_AVAILABLE:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "Peer is available again!");
                            /*api_id = X2AP_SCTP_CONN_RESTART_IND;*/
                            return 0;
                        }

                    default:
                        {
                            RRC_TRACE(RRC_INFO,"Unknown!!!!!!!");
                            return 0;
                        }
                }

                break;

            }
        case SCTP_ASSOC_CHANGE:
            {
                RRC_TRACE(RRC_INFO,"Notification message of type "
                        "SCTP_ASSOC_CHANGE for sac_assoc_id %d",
                        p_notification->sn_assoc_change.sac_assoc_id);

                p_sn_assoc_change = &(p_notification->
                        sn_assoc_change);

                switch(p_sn_assoc_change->sac_state)
                {
                    case SCTP_COMMUNICATION_UP:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: A new association has just been started");
                            *p_api_id = X2AP_SCTP_COMM_UP_IND;
                            break;
                        }
                    case SCTP_COMMUNICATION_LOST:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: An SCTP association has" 
                                    "just been lost for sac_assoc_id %d",p_sn_assoc_change->sac_assoc_id);
                            *p_api_id = X2AP_SCTP_CONN_FAILURE_IND;
                            break;
                        }
                    case SCTP_RESTART:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: Peer associated with the sac_assoc_id %d"\
                                    " has been restarted ",p_sn_assoc_change->sac_assoc_id);
                            *p_api_id = X2AP_SCTP_CONN_RESTART_IND;
                            break;
                        }
                    case SCTP_SHUTDOWN_COMPLETE:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: Shutdown completed with the peer entity"\
                                    " associated with the sac_assoc_id %d",p_sn_assoc_change->sac_assoc_id);
                            /* Because shutdown was initiated by us, we will send 
                             *        close indication to ap
                             */
                            *p_api_id = X2AP_SCTP_CLOSED_IND;
                            break;
                        }
                    case SCTP_CANT_START_ASSOC:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: The peer did not respond" 
                                    "to an association setup attempt");
                            *p_api_id = X2AP_SCTP_CONN_FAILURE_IND;
                            break;
                        }

                        /* Used only in HSS specific interface */
                    case SCTP_ENDPOINT_DOWN_RETRIEVABLE:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: SCTP_ENDPOINT_DOWN_RETRIEVABLE ");
                            return 0;
                        }

                    case SCTP_ENDPOINT_ADDR_CHNG:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: SCTP_ENDPOINT_ADDR_CHNG");
                            return 0;
                        }
                    default:
                        {
                            RRC_TRACE(RRC_WARNING,
                                    "Unknown State for received Notification");
                            return 0;
                        }
                }/* End of switch */
                break;
            }
        case SCTP_REMOTE_ERROR:
            {
                RRC_TRACE(RRC_WARNING,"SCTP: SCTP REMOTE ERROR");
                return 0;
            }

        case SCTP_SEND_FAILED:
            {
                RRC_TRACE(RRC_WARNING,"SCTP: SCTP SEND FAILED");
                return 0;

            }
        case SCTP_SHUTDOWN_EVENT:
            {
                RRC_TRACE(RRC_WARNING,
                        "SCTP: Peer entity associated with sse_assoc_id %d"\
                        " has initiated SHUTDOWN", p_notification->sn_shutdown_event.sse_assoc_id);
                *p_api_id = X2AP_SCTP_SHUT_DOWN_INIT_IND;
                break;
            }
        case SCTP_ADAPTION_EVENT:
            {
                RRC_TRACE(RRC_WARNING,"SCTP: SCTP ADAPTION EVENT ");
                return 0;
            }
        case SCTP_PARTIAL_DELIVERY:
            {
                RRC_TRACE(RRC_WARNING,"SCTP PARTIAL DELIVERY");
                return 0;
            }


        default:
            {
                RRC_TRACE(RRC_INFO,"Unknown Notification!!!!!!!");
                return 0;
            }
    }
    return 1;
}

/******************************************************************************
 *   Function Name     : handle_s1ap_sctp_fd
 *   Input             : p_notification
 *   Output            : p_api_id
 *   Returns           : Returns 0 on Error and 1 on Success.
 *   Description       : This Function handles the s1ap sctp fd
******************************************************************************/
static U8 handle_s1ap_sctp_fd(
        sctp_notification_ut *p_notification,
        U16 *p_api_id)
{
    /* Notification message types */
    sctp_paddr_change_st   * p_sn_padr_change = PNULL;
    sctp_assoc_change_st   * p_sn_assoc_change = PNULL;

    switch (p_notification->
            sn_header.sn_type)
    {
        case SCTP_PEER_ADDR_CHANGE:
            {
                RRC_TRACE(RRC_INFO,"Notification message of type "
                        "SCTP_PEER_ADDR_CHANGE for spc_assoc_id %d",
                        p_notification->sn_padr_change.spc_assoc_id);
                p_sn_padr_change = &(p_notification->
                        sn_padr_change);
                switch (p_sn_padr_change->spc_state)
                {
                    case SCTP_ADDRESS_UNREACHABLE:
                        {
                            /* SPR_9640 Fix + */
                            break;
                            /* SPR_9640 Fix - */
                        }
                    case SCTP_ADDRESS_AVAILABLE:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "Peer is available again!");
                            *p_api_id = SCTP_CONN_RECOVERY_IND;
                            break;
                        }

                    default:
                        {
                            RRC_TRACE(RRC_INFO,"Unknown!!!!!!!");
                            return 0;
                        }
                }

                break;
            }
        case SCTP_ASSOC_CHANGE:
            {
                RRC_TRACE(RRC_INFO,"Notification message of type "
                        "SCTP_ASSOC_CHANGE for sac_assoc_id %d",
                        p_notification->sn_assoc_change.sac_assoc_id);

                p_sn_assoc_change = &(p_notification->
                        sn_assoc_change);

                switch(p_sn_assoc_change->sac_state)
                {
                    case SCTP_COMMUNICATION_UP:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: A new association has just been started");

                            /*Bug 9050 fixes start*/
                            RRC_TRACE(RRC_WARNING,
                                    "SCTP: : SCTP_COMMUNICATION_UP");
                            *p_api_id = SCTP_ASSOC_UP_EVENT; 
                            break;
                            /*Bug 9050 fixes end*/
                        }
                    case SCTP_COMMUNICATION_LOST:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: An SCTP association has" 
                                    "just been lost for sac_assoc_id %d",p_sn_assoc_change->sac_assoc_id);
                            *p_api_id = SCTP_CONN_FAILURE_IND;

                            break;
                        }
                    case SCTP_RESTART:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: Peer associated with the sac_assoc_id %d"\
                                    " has been restarted ",p_sn_assoc_change->sac_assoc_id);

                            *p_api_id = SCTP_CONN_RECOVERY_IND;
                            break;
                        }
                    case SCTP_SHUTDOWN_COMPLETE:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: Shutdown completed with the peer entity"\
                                    " associated with the sac_assoc_id %d",p_sn_assoc_change->sac_assoc_id);
                            /*SPR 20805 Start */
                            *p_api_id = SCTP_CONN_SHUTDOWN_EVENT;
                            break;
                            /*SPR 20805 Stop */
                        }
                    case SCTP_CANT_START_ASSOC:
                        {

                            RRC_TRACE(RRC_INFO,
                                    "SCTP: The peer did not respond to an association"\
                                    " setup attempt");
                                /*Bug 9050 fixes start*/
                                        RRC_TRACE(RRC_WARNING,
                                            "SCTP: : SCTP_CANT_START_ASSOC");
                                        *p_api_id = SCTP_ASSOC_DOWN_EVENT; 
                                        break;
                                /*Bug 9050 fixes end*/
                        }

                    case SCTP_ENDPOINT_DOWN_RETRIEVABLE:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: SCTP_ENDPOINT_DOWN_RETRIEVABLE ");

                            return 0;
                        }

                    case SCTP_ENDPOINT_ADDR_CHNG:
                        {
                            RRC_TRACE(RRC_INFO,
                                    "SCTP: SCTP_ENDPOINT_ADDR_CHNG");

                            return 0;
                        }
                    default:
                        {
                            RRC_TRACE(RRC_WARNING,
                                    "Unknown State for received Notification");

                            return 0;
                        }
                }/* End of switch */
                break;
            }
        case SCTP_REMOTE_ERROR:
            {
                RRC_TRACE(RRC_WARNING,"SCTP: SCTP REMOTE ERROR");
                return 0;
            }

        case SCTP_SEND_FAILED:
            {
                RRC_TRACE(RRC_WARNING,"SCTP: SCTP SEND FAILED");
                return 0;

            }
        case SCTP_SHUTDOWN_EVENT:
            {
                RRC_TRACE(RRC_WARNING,
                        "SCTP: Peer entity associated with sse_assoc_id %d"\
                        " has initiated SHUTDOWN", p_notification->sn_shutdown_event.sse_assoc_id);
                /*SPR 20805 Start*/
                return 0;
                /*SPR 20805 Stop*/

            }
        case SCTP_ADAPTION_EVENT:
            {
                RRC_TRACE(RRC_WARNING,"SCTP: SCTP ADAPTION EVENT ");
                return 0;
            }
        case SCTP_PARTIAL_DELIVERY:
            {
                RRC_TRACE(RRC_WARNING,"SCTP PARTIAL DELIVERY");
                return 0;
            }

        default:
            {
                RRC_TRACE(RRC_INFO,"Unknown Notification!!!!!!!");
                return 0;
            }
    }
    return 1;
}


/*******************************************************************************
 *   Function Name    : l3_sctp_receive
 *   Input            : None
 *   Output           : sender   -         Pointer to Sender Module ID.
 *                      receiver -         Pointer to Receiver Module ID.
 *                      priority           Pointer to priority.
 *   Returns          : Returns pointer the message buffer which is sent to 
 *                      Destination Module ID.
 *                      Returns NULL if no message is available
 *   Description      : This Function receives data.It is also used for handling
 *                      notifications.
*******************************************************************************/
void* l3_sctp_receive(QMODULE *sender, QMODULE *receiver, S8 *priority)
{
    static S8 buffer[MAX_BUFFER_SIZE] = {0};
    S8 * msg_buf = PNULL;

    U16 api_id = RRC_NULL;
    U16 msg_buf_len = RRC_NULL;
    S32 socket = RRC_NULL;
    S32 bytes_read = RRC_NULL;
    S32 flags = RRC_NULL;

    static U32 counter = RRC_NULL;

    /* L3-X2AP Integration Activity - Start */    
    static U32 x2ap_sd_counter = RRC_NULL;
    s1ap_x2ap_sctp_fd s1ap_x2ap_sctp_flag = SCTP_FD_NOT_SET;
    /* L3-X2AP Integration Activity - End */    

    /* L3-XWAP Integration Activity - Start */
    /* L3-XWAP Integration Activity - End */

    rrc_module_id_t int_dst_id = RRC_NULL;
    rrc_module_id_t src_id = RRC_NULL;

    struct timeval  zero;
    struct sctp_sndrcvinfo sndrcvinfo;
    struct sockaddr from_addr;

    sctp_fd_set_st local_sctp_fd;
    sctp_socklen_t from_addr_len = 0;
    sctp_notification_ut *p_notification = PNULL;

#ifdef LTE_EMBMS_SUPPORTED
    /* SPR 17998 Changes Start */
    U8 isDataFrmMceDetected = RRC_ZERO;
    /* SPR 17998 Changes End */
#endif

    memset_wrapper(&buffer, 0, MAX_BUFFER_SIZE);
    memset_wrapper((struct sctp_sndrcvinfo *)&sndrcvinfo, 0, sizeof(struct sctp_sndrcvinfo));
    /* Dispatch SCTP stack packets */
    {
        sig_q_shell_st *p_shell = PNULL;
        QMODULE     from, to;
        QSHELL *S = PNULL;
        sig_q_time_st q_time;

        q_time.s = 0;
        q_time.us = 0;
        sig_get_qshell(&p_shell);
        S = p_shell;
        S->timedwait(PNULL,&q_time);
        S->receive(PNULL, &from, &to, priority);

    }
    SCTP_FD_ZERO(&local_sctp_fd);


    /* BUG:9050 */
    if ((!connection_count) && (!g_sctp_sd_count)
       )
    {
        return 0;
    }
/* BUG:9050 */

    local_sctp_fd = sctp_fd;

    zero.tv_sec = 0;
    zero.tv_usec = 0;

    if (SCTP_SOCKET_ERROR == 
            sctp_select(SCTP_FD_SETSIZE - 1, &local_sctp_fd, 0, 0, &zero))
    {
        RRC_TRACE(RRC_FATAL,
            "Problem with sctp_select in l3_sctp_receive. System error code %d",
            sk_errno);
        ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
        return 0;
    }


    /* check which socket descriptor is active*/
    for(counter = 0 ; counter < connection_count; counter++)
    {

        if ( (INVALID_SCTP_SOCKET != sctp_sd_list[counter]) &&
                SCTP_FD_ISSET((U32)sctp_sd_list[counter], &local_sctp_fd))
        {
            socket = sctp_sd_list[counter];
            RRC_TRACE(RRC_INFO,"SD is set for sctp_sd(S1AP) %d ",socket);
            s1ap_x2ap_sctp_flag = S1AP_SCTP_FD_SET; /* L3-X2AP Integration Activity */
            break;
        }
    }
    /* L3-X2AP Integration Activity - Start */
    for(x2ap_sd_counter = 0 ; x2ap_sd_counter < g_sctp_sd_count; x2ap_sd_counter++)
    {

        if ( (INVALID_SCTP_SOCKET != g_sctp_sd_list[x2ap_sd_counter]) &&
                SCTP_FD_ISSET((U32)g_sctp_sd_list[x2ap_sd_counter], &local_sctp_fd))
        {
            socket = g_sctp_sd_list[x2ap_sd_counter];
            RRC_TRACE(RRC_INFO,"SD is set for sctp_sd(X2AP) %d ",socket);
            s1ap_x2ap_sctp_flag = X2AP_SCTP_FD_SET;
            break;
        }
    }
#ifdef LTE_EMBMS_SUPPORTED
    if(SCTP_FD_ISSET((U32)g_m2ap_sctp_sd, &local_sctp_fd))
    {
        socket = g_m2ap_sctp_sd ; 
        RRC_TRACE(RRC_INFO,"SD is set for sctp_sd(M2AP) %d \n",socket);
        s1ap_x2ap_sctp_flag = M2AP_SCTP_FD_SET;
        isDataFrmMceDetected = RRC_ONE; 
    }
#endif
    /* L3-X2AP Integration Activity - End */

    /* L3-XWAP Integration Activity - Start */
    /* L3-XWAP Integration Activity - End */

    /* CSR 00077022 Fix Start */
    if (SCTP_FD_NOT_SET == s1ap_x2ap_sctp_flag 
       )
    {
        /* SPR 13564 Fix Start */
        /* SPR 13564 Fix Stop */
        return 0;
    }
    /* CSR 00077022 Fix Stop */

#ifndef LTE_EMBMS_SUPPORTED
    if ((counter == connection_count) && (x2ap_sd_counter == g_sctp_sd_count)
#else
/* SPR 17998 Changes Start */
    if ((counter == connection_count) && (x2ap_sd_counter == g_sctp_sd_count)
        && RRC_ZERO == isDataFrmMceDetected)
#endif
/* SPR 17998 Changes End */
       )
    {
        /* There is no activity detected on any of the sctp sockets 
         * so return back successfully
         */
        return 0;
    }


    /* Reading message on the active socket */
    /* SPR 20633 changes start */
    if ( (socket == g_sctp_server_fd)&& 
            (s1ap_x2ap_sctp_flag == X2AP_SCTP_FD_SET) )
    {
        RRC_TRACE(RRC_INFO,
                    "SCTP: Activity on server socket g_sctp_server_fd[%d], socket[%d]\n",
                    g_sctp_server_fd, socket);
        api_id = X2AP_SCTP_PEND_CONN_IND;
    }
    /* SPR 20633 changes end */
    else
    {
        if(socket < 0)
        {
            RRC_TRACE(RRC_FATAL, "l3_sctp_receive: Invalid file Descriptor");
            ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
            return 0;
        }

        if ( S1AP_SCTP_ERROR == (bytes_read = sctp_sctp_recvmsg(
                        socket,
                        (void *)buffer,
                        sizeof(buffer),
                        &from_addr,
                        &from_addr_len,
                        (struct sctp_sndrcvinfo *)&sndrcvinfo,
                        (int *)(&flags) )))

        {
            RRC_TRACE(RRC_ERROR,
                    "sctp_sctp_recvmsg returned failure %d flags 0x%x !!",
                    bytes_read,flags);
            ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
            return 0;
        }

        /* Checking whether it is a message or a notification */


        RRC_TRACE(RRC_INFO,
                "sctp_sctp_recvmsg returned %d successfully, flags 0x%x !!",
                bytes_read,flags);
        if( s1ap_x2ap_sctp_flag == S1AP_SCTP_FD_SET )
        {

            if(flags & SCTP_MSG_NOTIFICATION)
            {
                p_notification = (sctp_notification_ut *)buffer;

                if(0 == handle_s1ap_sctp_fd(p_notification, &api_id))
                {
                    return 0;
                }
            }
            else
            {
                api_id = MME_MESSAGE;
            }
        }
        else if(s1ap_x2ap_sctp_flag == X2AP_SCTP_FD_SET)
        {

            if(flags & SCTP_MSG_NOTIFICATION)
            {
                p_notification = (sctp_notification_ut *)buffer;

                if(0 == handle_x2ap_sctp_fd(p_notification, &api_id))
                {
                    return 0;
                }
            }
            else
            {
                api_id = X2AP_ENB_MESSAGE;
            }

        }
#ifdef LTE_EMBMS_SUPPORTED
        else if(s1ap_x2ap_sctp_flag == M2AP_SCTP_FD_SET)
        {
            if (flags & SCTP_MSG_NOTIFICATION)
            {
                p_notification = (sctp_notification_ut *)buffer;

                if ( 0 == handle_m2ap_sctp_fd(p_notification, &api_id))
                {
                    return 0;
                }
            }
            else
            {
                api_id = MCE_MESSAGE;
            }
        }
#endif

        /* L3-XWAP Integration Activity - End */
        else
        {
            RRC_TRACE(RRC_INFO,
                    "Socket Flag is not set although Socket activity received");
        }
    }


    /* Computing total length */
    msg_buf_len = (U16)(RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE + 
            SCTP_STREAM_ID_SIZE + bytes_read);
    msg_buf = (S8*) rrc_msg_mem_get(msg_buf_len);

    if (msg_buf == PNULL)
    {
        /* Not enough memory */
        RRC_TRACE(RRC_ERROR,"Memory Allocation failed in sctp_receive");
        return 0;
    }

    s1ap_set_sctp_sd(
            (U32*) (msg_buf + RRC_API_HEADER_SIZE) ,socket);
    s1ap_set_sctp_stream(
            (U16*) (msg_buf + RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE) ,
            sndrcvinfo.sinfo_stream);


    if(bytes_read != 0)
    {
        l3_memcpy_wrapper(
                (msg_buf + RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE + SCTP_STREAM_ID_SIZE),
                buffer,
                (U32)bytes_read);      /* copying received ASN message */
    }

    /* L3-X2AP Integration Activity - Start */
    if ( s1ap_x2ap_sctp_flag == S1AP_SCTP_FD_SET )
    {
        src_id = RRC_MME_MODULE_ID;
        int_dst_id = RRC_S1AP_MODULE_ID;
    }
    else if ( s1ap_x2ap_sctp_flag == X2AP_SCTP_FD_SET )
    {
        src_id = RRC_X2AP_PEER_eNODEB_MODULE_ID;
        int_dst_id = RRC_X2AP_MODULE_ID;
    }
    /* L3-X2AP Integration Activity - End */

    /* L3-XWAP Integration Activiy - Start */
#ifdef LTE_EMBMS_SUPPORTED
    else
    {
        src_id = RRC_MCE_MODULE_ID;    /* Discusss*/
        int_dst_id = RRC_M2AP_MODULE_ID;
    }
#endif
    /* L3-XWAP Integration Activity - End */

    /* Fill the RRC API header */
    rrc_construct_api_header(
            (U8 *)msg_buf,
            RRC_VERSION_ID,
            src_id,
            int_dst_id,
            api_id,
            msg_buf_len);
#ifdef LATENCY_TRACE_ON
    struct timespec  tp;
    struct tm      *tm;

    clock_gettime_wrapper(CLOCK_PROCESS_CPUTIME_ID, &tp);
    tm = localtime_wrapper(&tp.tv_sec);

    if (start_logging == 1)
    {
        g_latency_data[g_index].send_rcv_flag = RRC_RECEIVE;
        g_latency_data[g_index].src_mod_id = 0xAAAA;
        g_latency_data[g_index].dest_mod_id = 0xAAAA;
        g_latency_data[g_index].api_id = 0xAAAA;
        l3_memcpy_wrapper(&g_latency_data[g_index].val_time, tm, 
                sizeof(g_latency_data[g_index].val_time)); 
        g_latency_data[g_index].time_u_secs = tp.tv_nsec; 
        g_index++;
    }
#endif

    *sender         = qvGetService(src_id);
    *receiver       = qvGetService(int_dst_id);
    *priority       = 0;

    RRC_ASSERT(0 != *sender);
    RRC_ASSERT(0 != *receiver);

#ifdef LTE_EMBMS_SUPPORTED
    if (s1ap_x2ap_sctp_flag == X2AP_SCTP_FD_SET)
    {
        RRC_TRACE(RRC_INFO,"Message is sent to X2AP module\n");
    }
    else if (s1ap_x2ap_sctp_flag == S1AP_SCTP_FD_SET)
    {
        RRC_TRACE(RRC_INFO,"Message is sent to S1AP module\n");
    }
    else
    {
        RRC_TRACE(RRC_INFO,"Message is sent to M2aP module\n");
    }

#else
    if (s1ap_x2ap_sctp_flag == X2AP_SCTP_FD_SET)
    {
        RRC_TRACE(RRC_DETAILED,"Message is sent to X2AP module");
    }
    else
    {
        RRC_TRACE(RRC_DETAILED,"Message is sent to S1AP module");
    }
#endif

    return msg_buf;
}



#endif
#endif	




/*****************************************************************************/


#ifdef LKSCTP




        fd_set sctp_fd;


/******************************************************************************
 *   Function Name    : l3_sctp_close_association
 *   INPUT            : socket  -   Socket Descriptor of the point
 *   Output           : None.
 *   Returns          : Returns 0 on success or -1 on Error.
 *   Description      : This Function performs the graceful shutdown.
 ******************************************************************************/
rrc_return_et l3_sctp_close_association( sctp_sd_t socket)
{

#ifdef LINUX_PC_TEST
	pctest_send_sctp_close_to_pc((U32)socket);
	return RRC_SUCCESS;
#endif

//#ifndef RRC_UNIT_TEST_FRAMEWORK
	U32 counter = RRC_NULL;
	/* Coverity_41845 Fix Start */
	/* Coverity_41844 Fix Start */
	if(socket < 0) 
	{    
		RRC_TRACE(RRC_WARNING,"Invalid value of socket received");
		return RRC_FAILURE;
	}
	/* Coverity_41844 Fix End */
	/* Coverity_41845 Fix End */
	if (S1AP_SCTP_ERROR == close_wrapper(socket))
     {
         RRC_TRACE(RRC_WARNING,"Problem in closing socket");
         return RRC_FAILURE;
     }
     lteFD_CLR((S32)socket,&sctp_fd);

    for(counter = 0; counter < connection_count;counter++)
    {
        if (socket == sctp_sd_list[counter])
        {
            sctp_sd_list[counter] = sctp_sd_list[connection_count - 1];
            sctp_sd_list[connection_count - 1 ] = INVALID_SCTP_SOCKET;
            connection_count--;
            break;
        }
    }

    /* Decrementing count for total connection */
    if (RRC_NULL == connection_count)
    {
        init_stack_flag = RRC_FALSE;
    }

    RRC_TRACE(RRC_INFO,
            "SCTP : connection_count decremented to %d",connection_count);
//#endif
    return RRC_SUCCESS;
}



/******************************************************************************
 *   Function Name    : l3_sctp_close_socket
 *   INPUT            : socket  -   Socket Descriptor of the point
 *   Output           : None.
 *   Returns          : Returns 0 on success or -1 on Error.
 *   Description      : This Function closes the socket descriptor.
 ******************************************************************************/
rrc_return_et l3_sctp_close_socket(S32 socket)
{
    if (S1AP_SCTP_ERROR == close_wrapper(socket))
    {
        RRC_TRACE(RRC_WARNING,"Problem in closing socket");
        return RRC_FAILURE;
    }
    return RRC_SUCCESS;
}

/*****************************************************************************
 *   Function Name     : l3_sctp_send
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
S32 l3_sctp_send(
            sctp_sd_t                    socket,
            const void            *p_message,
            socklen_t              msg_len,
            U32                    stream_no)
{
    S32 send_result = 0;
#ifdef LINUX_PC_TEST
	pctest_forward_mme_message_to_pc(
			RRC_S1AP_MODULE_ID,
			RRC_MME_MODULE_ID,
			MME_MESSAGE_PS_TO_TEST,
			socket,
			stream_no, 
			p_message,
			msg_len);
	char * pStr = change_data_to_str(msg_len, (U8*)p_message);
	RRC_TRACE(RRC_INFO, "l3_sctp_send: %s",pStr);
	return send_result;
#endif
//#ifndef RRC_UNIT_TEST_FRAMEWORK
    if(socket < 0)
    {
        RRC_TRACE(RRC_FATAL, "l3_sctp_send: Invalid file Descriptor");
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
        return -1;
    }

    if(NULL == p_message)
    {
        RRC_TRACE(RRC_FATAL, "l3_sctp_send: Data to be sent NULL pointer");
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_NULL_POINTER_ERR_CAUSE_ID);
        return -1;
    }
    send_result = sctp_sendmsg(
               socket,
               p_message,
               msg_len,
               NULL,
               0,
               /* SPR_14436_Fix Start */
               htonl_wrapper(SCTP_PAYLOAD_PROTOCOL_ID_S1AP),
               /* SPR_14436_Fix Stop */
               0,
              (U16)stream_no,
               0,
               0);
    if(send_result < 0)
    {
        RRC_TRACE(RRC_FATAL, "l3_sctp_send: sctp_sctp_sendmsg failed invalide paramenter");
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
    }
//#else
#if 0
    U16 src_module_id = RRC_MODULE_ID;
    U16 dst_module_id = RRC_MME_MODULE_ID;

    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_s1ap_msg;

    /* Get API length */
    msg_length = msg_len;

    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):S1AP_MSG", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_s1ap_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_s1ap_msg, RRC_VERSION_ID, src_module_id,
            dst_module_id, MME_MESSAGE, msg_api_length);

    /* Fill interface header */
    p_s1ap_msg = p_s1ap_msg + RRC_API_HEADER_SIZE;

    memcpy(p_s1ap_msg, p_message, msg_len);

    rrc_send_message(p_msg, dst_module_id);

#endif
//#endif

    return send_result;
}

/*sctp_nb_fix_start*/
/******************************************************************************
 *   Function Name     : l3_open_sctp_association
 *   Input             : p_s1ap_gb_ctx - Pointer to s1ap global context
 *                       p_mme_comm_info   -      Pointer to MME related
 *                                                communication Information.
 *                       p_enb_comm_info   -      Pointer to eNB related
 *                                                communication Information.
 *   Output            : Connected Socket Descriptor.
 *   Returns           : Returns connected socket descriptor on Success or -1 on
 *                       Error.
 *   Description       : This Function performs the sequence of steps to gain a
 *                       connected socket.
 ******************************************************************************/
S32 l3_open_sctp_association(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        mme_context_t *p_mme_ctx,
        enb_comm_info_t *p_enb_comm_info)
{
    U32  counter       = RRC_NULL;
    S32  connSock      = RRC_NULL;
    U16  mme_count     = RRC_NULL;

    /* Coverity_61208 Fix Start */
    /* Coverity_61208 Fix End */

    mme_comm_info_t *p_mme_comm_info = &(p_mme_ctx->connection_info);
    U32 nostreams = p_mme_comm_info->num_streams;
    U32 nistreams = p_mme_comm_info->num_streams;
    S32 return_value  = RRC_NULL;
    /*SPR 8313 FIX start*/
//#ifndef RRC_UNIT_TEST_FRAMEWORK
    const U32 optVal = 1;
    /*SPR 8313 FIX stop*/
//#endif
    struct sockaddr_in6 enb_local_sctp_addr6[MAX_NUM_IP_ADDR];
    struct sockaddr_in enb_local_sctp_addr[MAX_NUM_IP_ADDR];
    struct sctp_event_subscribe events;
/*sctp parameters start*/
    s1ap_sctp_conf_info_t *p_sctp_params = PNULL;
/*sctp parameters stop*/


    struct sctp_initmsg initmsg;
    /*sctp parameters start*/
    struct sctp_assocparams assocparams;
    /*dscp_marking_start*/
    U8     tos;
    U32    on = 1, tclass;  //For IPv6
    /*dscp_marking_stop*/
    /*sctp parameters stop*/

    U8 num_of_ip_addr = RRC_NULL;

    /*SPR 15570 Fix Start*/
    p_mme_ctx->is_conn_ongoing = RRC_FALSE;
    /*SPR 15570 Fix Stop*/

/*sctp parameters start */
    if(RRC_S1AP_SCTP_CONFIG_PARAMS_PRESENT == p_s1ap_gb_ctx->bitmask)
    {
        p_sctp_params = &(p_s1ap_gb_ctx->s1ap_sctp_conf_param);
    }
    /*sctp parameters stop */

    /* For binding in Multihoming*/
    if (RRC_FALSE == init_stack_flag)
    {
        RRC_TRACE(RRC_INFO," Initializing sctp_sd_list..");
        /*Initiating sctp_sd_list*/
        for (mme_count = 0; mme_count < MAX_NUM_MME ; mme_count++ )
        {
            sctp_sd_list[mme_count] = INVALID_SCTP_SOCKET; 
        }
    }

    RRC_TRACE(RRC_INFO," Creating socket..");
    /* Create an SCTP TCP-Style Socket */
/*SPR 20633 Fix START*/
    if((p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_ADDR_PRESENT) && (p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT))
    {
        connSock = socket_wrapper( PF_INET6, SOCK_STREAM, IPPROTO_SCTP);
        num_of_ip_addr = p_enb_comm_info->num_ipv6_addr; 
    }
    if((p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV4_ADDR_PRESENT) && (!(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)))
    {
/*SPR 20633 Fix End*/
        connSock = socket_wrapper( PF_INET, SOCK_STREAM, IPPROTO_SCTP);
        num_of_ip_addr = p_enb_comm_info->num_ip_addr; 
    }

    if (connSock < 0)
    {
        RRC_TRACE(RRC_WARNING,"SCTP : sctp_socket failed");
        return S1AP_SCTP_ERROR;
    }
    struct timeval start_time;
    if ( 0 ==  gettimeofday_wrapper(&start_time, NULL))
    {
        p_s1ap_gb_ctx->sctp_start_time.tv_sec = (U32)start_time.tv_sec;
        p_s1ap_gb_ctx->sctp_start_time.tv_usec = (U32)start_time.tv_usec;
        RRC_TRACE(RRC_BRIEF,"start_time sec = %u and usec = %u",p_s1ap_gb_ctx->sctp_start_time.tv_sec,
                   p_s1ap_gb_ctx->sctp_start_time.tv_usec);
    } 
    else
    {
        RRC_TRACE(RRC_ERROR,"SCTP gettimeofday:  failed: Did not get start time");
    }

    /* Populating Structure for SCTP 4-way handshake */
    memset_wrapper(&initmsg, 0, sizeof(initmsg));

    initmsg.sinit_num_ostreams  = nostreams;
    initmsg.sinit_max_instreams = nistreams;
    /*sctp parameters start*/
    initmsg.sinit_max_init_timeo = RRC_SCTP_DEFAULT_INIT_TIMEOUT;
    if(PNULL != p_sctp_params)
    {
        initmsg.sinit_max_attempts = p_sctp_params->init_max_attempts;
    }
    else
    {
        initmsg.sinit_max_attempts  = RRC_SCTP_DEFAULT_MAX_INIT_RTX;
    }
    /*sctp parameters end*/

    return_value = setsockopt_wrapper(
            connSock,
            IPPROTO_SCTP,
            SCTP_INITMSG,
            &initmsg,
            sizeof(initmsg));
#if 0
#ifdef RRC_UNIT_TEST_FRAMEWORK
    return_value = 1;
#endif
#endif

    if (return_value < 0)
    {
        RRC_TRACE(RRC_WARNING,"SCTP : sctp_setsockopt for INIT failed");
        l3_sctp_close_socket(connSock);
        return S1AP_SCTP_ERROR;
    }

    RRC_TRACE(RRC_INFO,"SCTP : setsockopt for INIT success");

    /*sctp parameters start*/
    /* Populating Structure for SCTP 4-way handshake */
    memset_wrapper(&assocparams, 0, sizeof(assocparams));

    assocparams.sasoc_assoc_id = connSock;
    if(PNULL != p_sctp_params)
    {
            assocparams.sasoc_asocmaxrxt = p_sctp_params->assoc_max_retrans;
            assocparams.sasoc_cookie_life = p_sctp_params->valid_cookie_life;
    }
    else
    {
        assocparams.sasoc_asocmaxrxt = RRC_SCTP_DEFAULT_ASSOC_MAX_RTX;
        assocparams.sasoc_cookie_life = RRC_SCTP_DEFAULT_VAL_COOKIE_LIFE;
    }
    return_value = setsockopt_wrapper(
            connSock,
            IPPROTO_SCTP,
            SCTP_ASSOCINFO,
            &assocparams,
            sizeof(assocparams));
#if 0	
#ifdef RRC_UNIT_TEST_FRAMEWORK
    return_value = 1;
#endif
#endif

    if (return_value < 0)
    {
        RRC_TRACE(RRC_WARNING,"SCTP : setsockopt for assocparams failed");
        l3_sctp_close_socket(connSock);
        return S1AP_SCTP_ERROR;
    }

    /*sctp parameters end*/

    /*Multi-Homing */
    for (counter = 0; 
            counter < num_of_ip_addr;
            counter++)
    {
    /* SPR 20633 changes start */
        if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    /* SPR 20633 changes end */
        {

            enb_local_sctp_addr6[counter].sin6_family = 10;
/* coverity_60513 start */
            if(inet_pton_wrapper(10, 
                    (const char *)p_enb_comm_info->ipv6_addr[counter].ipv6_addr, 
                    &enb_local_sctp_addr6[counter].sin6_addr)!= 1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert INET6 adress while setting HeartBeat");
                l3_sctp_close_socket(connSock);/*cov 61218 +-*/
                return S1AP_SCTP_ERROR;
            }
/* coverity_60513 stop */
            RRC_TRACE(RRC_INFO, "eNB's SCTP IP ADDR : %s : PORT : %u",
                    p_enb_comm_info->ipv6_addr[counter].ipv6_addr,
                    p_enb_comm_info->port);
            enb_local_sctp_addr6[counter].sin6_port = htons_wrapper(p_enb_comm_info->port);
        }
        else
        {
            enb_local_sctp_addr[counter].sin_family = AF_INET;
/* coverity_60513 start */
            if (inet_pton_wrapper(AF_INET, 
                    (const char *)p_enb_comm_info->ip_addr[counter].ip_addr, 
                    &(enb_local_sctp_addr[counter].sin_addr))!=1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert INET6 adress while setting HeartBeat");
                l3_sctp_close_socket(connSock);/*cov 61218 +-*/
                return S1AP_SCTP_ERROR;
            }
/* coverity_60513 stop */
            RRC_TRACE(RRC_INFO, "eNB's SCTP IP ADDR : %s : PORT : %u",
                    p_enb_comm_info->ip_addr[counter].ip_addr,
                    p_enb_comm_info->port);
            enb_local_sctp_addr[counter].sin_port = htons_wrapper(p_enb_comm_info->port);
            memset_wrapper(&(enb_local_sctp_addr[counter].sin_zero),0,8);

        }
    }

//#ifndef RRC_UNIT_TEST_FRAMEWORK
    /*SPR 8313 FIX start*/
    if (setsockopt_wrapper(connSock, SOL_SOCKET,
                SO_REUSEADDR, &optVal, sizeof(U32)) == SCTP_SOCKET_ERROR)
    {
        RRC_TRACE(RRC_INFO," Connection socket property SO_REUSEADDR not set"
                "[errno:%d]", errno);
        /*unable to setsockopt thus deleting created socket_desc*/
        if(l3_sctp_close_socket(connSock)  == 0 )
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
/* coverity_94616 start */
            RRC_TRACE(RRC_INFO, "Connection socket closed is: %d",connSock);
/* coverity_94616 stop */
        }
        else
        {
            /* failure in closing socket_desc [%d] */
            RRC_TRACE(RRC_WARNING,
                    " Connection socket close failure, errno: %d", errno);
        }

        return SCTP_SOCKET_ERROR;
    }
//#endif
/*SPR 20633 Fix START*/
    if((p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_ADDR_PRESENT) && (p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT))
    {
        if(sctp_bindx_flag_IPv6 == RRC_FALSE)
        {
            return_value = sctp_bindx(connSock, (struct sockaddr*)&enb_local_sctp_addr6, 
                    num_of_ip_addr, 
                    SCTP_BINDX_ADD_ADDR);
        }
    }
    if((p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV4_ADDR_PRESENT) && (!(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)))
    {
        if(sctp_bindx_flag_IPv4 == RRC_FALSE)
        {
            return_value = sctp_bindx(connSock, (struct sockaddr*)&enb_local_sctp_addr, 
                    num_of_ip_addr, 
                    SCTP_BINDX_ADD_ADDR);
/*SPR 20633 Fix End*/
        }
    }
    /*Cov 93799 +*/
    if (return_value < 0)
    {
        RRC_TRACE(RRC_INFO,"LK SCTP BINDx: Problem in Bind %d",
                return_value);
        /* coverity_61218 start */
        if(l3_sctp_close_socket(connSock)  == 0 )
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
            RRC_TRACE(RRC_INFO, "Connection socket closed is: %d",connSock);
        }
        else
        {
            /* failure in closing socket_desc [%d] */
            RRC_TRACE(RRC_WARNING,
                    " Connection socket close failure, errno: %d", errno);
        }
        /* coverity_61218 stop */
        return S1AP_SCTP_ERROR;

    }
    /*Cov 93799 -*/
    else
    {
/*SPR 20633 Fix START*/
	    if((p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV6_ADDR_PRESENT) && (p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT))
	    {
		    sctp_bindx_flag_IPv6 = RRC_TRUE;
                    RRC_TRACE(RRC_INFO,"LK SCTP BINDx: Successfully Bind with IPv6");
	    }
	    if(p_enb_comm_info->bitmask & ENB_COMM_INFO_IPV4_ADDR_PRESENT && !(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT))
	    {
		    sctp_bindx_flag_IPv4 = RRC_TRUE;
            RRC_TRACE(RRC_INFO,"LK SCTP BINDx: Successfully Bind with IPv4");
	    }
/*SPR 20633 Fix End*/
    }
    /*SPR 8313 FIX stop*/
/*Bug 9050 fixes start*/
/* coverity_61206 start */
//#ifndef RRC_UNIT_TEST_FRAMEWORK
    S32 fl = fcntl_wrapper(connSock, F_GETFL, 0);
    if (-1 == fl)
    {
         RRC_TRACE(RRC_INFO, " Failure returned by the function fcntl_wrapper");
    }
/* coverity_61206 stop */
    if (fcntl_wrapper(connSock, F_SETFL, fl | O_NONBLOCK) == SCTP_SOCKET_ERROR)
    {
        RRC_TRACE(RRC_WARNING,
                "SCTP : Non blocking failed [%d]",errno);
        l3_sctp_close_socket(connSock);
        return S1AP_SCTP_ERROR;
    }
//#endif
/*Bug 9050 fixes end*/

    memset_wrapper( (void *)&events, 0, sizeof(events) );

    /* Populating SCTP Events structure*/
    events.sctp_association_event  = 1;
    events.sctp_data_io_event      = 1;
    events.sctp_address_event      = 1;
    events.sctp_send_failure_event = 1;
    events.sctp_peer_error_event   = 1;
    events.sctp_shutdown_event     = 1;

    return_value = setsockopt_wrapper(connSock, SOL_SCTP, SCTP_EVENTS, &events,
            sizeof (events));
#if 0	
#ifdef RRC_UNIT_TEST_FRAMEWORK
    return_value = 1;
#endif
#endif
    if (return_value < 0)
    {
        RRC_TRACE(RRC_WARNING,"SCTP : sctp_setsockopt for setting events failed");
        l3_sctp_close_socket(connSock);
        return S1AP_SCTP_ERROR;
    }
    RRC_TRACE(RRC_DETAILED,"SCTP : sctp_setsockopt for setting events Success");

    /*dscp_marking_start*/
    if( p_s1ap_gb_ctx->s1ap_sctp_conf_param.bitmask & S1AP_OAM_DSCP_VALUE_PRESENT )
    {
        if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)
        {   
            return_value = setsockopt_wrapper( connSock,IPPROTO_IPV6,IPV6_RECVTCLASS,&on,sizeof(on));
            if (return_value < 0)
            {
                RRC_TRACE(RRC_WARNING,"  Error in option RECVTCLASS errno = %d, %s",errno,strerror(errno));
            }
            tclass = p_s1ap_gb_ctx->s1ap_sctp_conf_param.s1ap_dscp_value << 2;
            return_value = setsockopt_wrapper( connSock,IPPROTO_IPV6,IPV6_TCLASS,&tclass,sizeof(tclass));
        }
        else
        {    
            tos = p_s1ap_gb_ctx->s1ap_sctp_conf_param.s1ap_dscp_value << 2;
            return_value = setsockopt_wrapper( connSock,IPPROTO_IP,IP_TOS,&tos,sizeof(tos));
        }
        if (return_value < 0)
        {
            RRC_TRACE(RRC_WARNING,"SCTP : DSCP is not set. errno = %d, %s",errno,strerror(errno));
            l3_sctp_close_socket(connSock);
            return S1AP_SCTP_ERROR;
        }
        RRC_TRACE(RRC_INFO,"SCTP : DSCP is set.");
    }
    /*dscp_marking_stop*/

    /* Getting connected with the Peer with multiple IPs */
    return_value = 
        l3_sctp_connectx(connSock,p_mme_comm_info);
#if 0	
#ifdef RRC_UNIT_TEST_FRAMEWORK
    return_value = 1;
#endif
#endif

    if (return_value < 0)
    {
/* BUG:9050 */
        if(EINPROGRESS == errno)
        {
            RRC_TRACE(RRC_WARNING,
                    "SCTP : l3_sctp_connectx in progress:- [%d]",errno);
            /*SPR 15570 Fix Start*/
            p_mme_ctx->is_conn_ongoing = RRC_TRUE;

            return connSock ;
            /*SPR 15570 Fix Stop*/
        }
        else
        {
            RRC_TRACE(RRC_WARNING,
                    "SCTP : l3_sctp_connectx failed, Error Code [%d]",errno);
            l3_sctp_close_socket(connSock);
            return S1AP_SCTP_ERROR;
        }
/* BUG:9050 */
    }

    for(counter = 0; counter < MAX_NUM_MME ;counter++)
    {
        if (INVALID_SCTP_SOCKET == sctp_sd_list[counter])
        {
            sctp_sd_list[counter] = connSock;
            connection_count++;
            break;
        }
    }


    RRC_TRACE(RRC_INFO,
            "SCTP : connection_count incremented to %d",connection_count);
    lteFD_SET((S32)connSock,&sctp_fd);
    init_stack_flag = RRC_TRUE;
#if 0	
#ifdef RRC_UNIT_TEST_FRAMEWORK
    return 1;
#endif
#endif

    return connSock;
}

/******************************************************************************
 *   Function Name     : l3_sctp_association_setup
 *   Input             : p_s1ap_gb_ctx - Pointer to s1ap global context
 *                       p_mme_comm_info   -      Pointer to MME related
 *                                                communication Information.
 *                       p_enb_comm_info   -      Pointer to eNB related
 *                                                communication Information.
 *   Output            : Connected Socket Descriptor.
 *   Returns           : Returns connected socket descriptor on Success or -1 on
 *                       Error.
 *   Description       : This Function performs the sequence of steps to enable
 *                       various socket options
 ******************************************************************************/
 /* Coverity_86875 Fix Start */
S32 l3_sctp_association_setup(
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        mme_context_t *p_mme_ctx,
        enb_comm_info_t *p_enb_comm_info)
  /* Coverity_86875 Fix End */

{
    U32  counter       = RRC_NULL;
    S32  connSock      = RRC_NULL;

  /* Coverity_86875 Fix Start */
  /* Coverity_86875 Fix End */

    mme_comm_info_t *p_mme_comm_info = &(p_mme_ctx->connection_info);
    ip_addr_t *p_mme_addr = NULL;
    ipv6_addr_t *p_mme_addr_ipv6 = NULL;
    U16 mme_port = p_mme_comm_info->port;
    U8 no_mme_ip_addr = p_mme_comm_info->num_ip_addr;
    U32 heartbeat_int = p_mme_comm_info->heart_beat_timer;
    S32 return_value  = RRC_NULL;
    U32 number_of_streams = RRC_NULL;
    U32 noDelay = 1;

    struct sockaddr_in servaddr[MAX_IP_ADDR];
    struct sockaddr_in6 servaddr6[MAX_IP_ADDR];
    struct sctp_paddrparams paddr;
/*sctp parameters start*/
    s1ap_sctp_conf_info_t *p_sctp_params = PNULL;
/*sctp parameters stop*/


    S8 family        = PF_INET;
    socklen_t optlen = 0;

    struct sctp_rtoinfo rtoinfo;
    struct sctp_status  status;

    if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)
    {
        p_mme_addr_ipv6 = &(p_mme_comm_info->ipv6_addr[0]);

    }
    else
    {
        p_mme_addr = &(p_mme_comm_info->ip_addr[0]);
    }
    
    /*sctp parameters start */
    if(RRC_S1AP_SCTP_CONFIG_PARAMS_PRESENT == p_s1ap_gb_ctx->bitmask)
    {
        p_sctp_params = &(p_s1ap_gb_ctx->s1ap_sctp_conf_param);
    }
    /*sctp parameters stop */


    /* Create an SCTP TCP-Style Socket */


    connSock = p_mme_ctx->sctp_sd;


    optlen = sizeof(status);
    return_value = getsockopt_wrapper(
            connSock,
            SOL_SCTP,
            SCTP_STATUS,
            &status,
            &optlen);
    if (return_value < 0)
    {
        RRC_TRACE(RRC_ERROR,"SCTP : Unable to retreive Number of streams");
        RRC_TRACE(RRC_INFO,"SCTP :  Error code :[%d] : %s ",errno,strerror(errno));
        return S1AP_SCTP_ERROR;
    }
    else
    {
        number_of_streams = status.sstat_outstrms;
        if(status.sstat_outstrms  <  2 ||
                status.sstat_instrms <  2)
        {
            RRC_TRACE(RRC_WARNING,"SCTP : Less Number of streams have been opened");
            return S1AP_SCTP_ERROR;
        }
        else
        {
            (p_mme_ctx->stream_info).number_of_streams = number_of_streams;
            for (counter = 0; counter < number_of_streams; counter++)
            {
                (p_mme_ctx->stream_info).stream_ids[counter] = counter;
            }

        }
    }

    memset_wrapper(&rtoinfo, 0, sizeof(rtoinfo));

    rtoinfo.srto_assoc_id = connSock;
    /*sctp parameters start*/
    if(PNULL != p_sctp_params)
    {
        rtoinfo.srto_initial = p_sctp_params->rto_initial;
        rtoinfo.srto_max = p_sctp_params->rto_max;
        rtoinfo.srto_min = p_sctp_params->rto_min;
    }
    else
    {
        rtoinfo.srto_initial = RRC_SCTP_DEFAULT_RTO_INITIAL;
        rtoinfo.srto_max = RRC_SCTP_DEFAULT_RTO_MAX;
        rtoinfo.srto_min = RRC_SCTP_DEFAULT_RTO_MIN;
    }
    /*sctp parameters end*/

    return_value =  setsockopt_wrapper( connSock,IPPROTO_SCTP , SCTP_RTOINFO,
            (const void *)&rtoinfo, sizeof(rtoinfo));

    if (return_value < 0)
    {
        RRC_TRACE(RRC_WARNING,"SCTP : RTO Interval is not set.");
        l3_sctp_close_socket(connSock);
        return S1AP_SCTP_ERROR;

    }

    RRC_TRACE(RRC_DETAILED,"SCTP : RTO Interval is set.");
    for (counter = 0; counter < no_mme_ip_addr; counter++)
    {
        memset_wrapper(&paddr, 0, sizeof(struct sctp_paddrparams));

        if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)
        {
            bzero_wrapper( (void *)&servaddr6[counter], sizeof(servaddr6[counter]) );
            servaddr6[counter].sin6_family     = 10;
            servaddr6[counter].sin6_port        = htons_wrapper(mme_port);
            if(inet_pton_wrapper(10,(const char *)p_mme_addr_ipv6->ipv6_addr, 
                        (void*)&servaddr6[counter].sin6_addr)!=1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert INET6 adress while setting HeartBeat");
                return S1AP_SCTP_ERROR;
            }
            /*valgrind_fix start*/
/* coverity_86877 start */
            l3_memcpy_wrapper(&paddr.spp_address,&servaddr6,sizeof(struct sockaddr_storage));
/* coverity_86877 stop */
            /*valgrind_fix stop*/
            p_mme_addr_ipv6++;
        }
        else
        {

            bzero_wrapper( (void *)&servaddr[counter], sizeof(servaddr[counter]) );
            servaddr[counter].sin_family      = (SA_FAMILY_T)family;
            servaddr[counter].sin_port        = htons_wrapper(mme_port);
            servaddr[counter].sin_addr.s_addr = 
                inet_addr_wrapper((const char*) p_mme_addr->ip_addr );
            /*valgrind_fix start*/
/* coverity_86877 start */
            l3_memcpy_wrapper(&paddr.spp_address,&servaddr,sizeof(struct sockaddr_storage));
/* coverity_86877 stop */
            /*valgrind_fix stop*/
            p_mme_addr++;
        }

        paddr.spp_assoc_id                = (U32)connSock;
        paddr.spp_hbinterval              = heartbeat_int;
        /* SPR 13663 Fix Start */
        paddr.spp_flags                   = SPP_HB_ENABLE;
        /* SPR 13663 Fix Stop */

        /*sctp parameters start*/
        if(PNULL != p_sctp_params)
        {
            paddr.spp_pathmaxrxt = p_sctp_params->path_max_retrans;
        }
        else
        {
            paddr.spp_pathmaxrxt = RRC_SCTP_DEFAULT_PATH_MAX_RTX;
        }
        /*sctp parameters end*/
        return_value = setsockopt_wrapper( connSock,IPPROTO_SCTP , SCTP_PEER_ADDR_PARAMS,
                (const void *)&paddr, sizeof(paddr));


        if (return_value < 0)
        {
            RRC_TRACE(RRC_WARNING,"SCTP : HeartBeat Interval is not set.");
            l3_sctp_close_socket(connSock);
            return S1AP_SCTP_ERROR;
        }
        RRC_TRACE(RRC_DETAILED,"SCTP : HeartBeat Interval is set.");
    }

    /* Getting connected with the Peer with multiple IPs */

    return_value =  setsockopt_wrapper(
            connSock,
            SOL_SCTP,
            SCTP_NODELAY,
            &noDelay,
            sizeof(noDelay));
    if (return_value < 0)
    {
        RRC_TRACE(RRC_WARNING,"SCTP :"
                "setsockopt(NODELAY) failed");
        l3_sctp_close_socket(connSock);
        return S1AP_SCTP_ERROR;
    }

    return connSock;

}
/*sctp_nb_fix_stop*/



/************************************************************************************
 *   Function Name    : l3_sctp_receive
 *   Input            : QMODULE *sender
 *                      QMODULE *receiver
 *                      S8 *priority
 *   Output           : sender   -         Pointer to Sender Module ID.
 *                      receiver -         Pointer to Receiver Module ID.
 *                      priority           Pointer to priority.
 *   Returns          : Returns pointer the message buffer which is sent to Destination
 *                      Module ID.
 *                      Returns NULL if no message is available
 *   Description      : This Function receives data. It is also used for handling
 *                      notifications.
 *************************************************************************************/
void* l3_sctp_receive(QMODULE *sender, QMODULE *receiver, S8 *priority)
{
/* coverity_20202 start */ 
    static S8 buffer[MAX_BUFFER_SIZE] = {0};
/* coverity_20202 stop */ 
    S8 * msg_buf = PNULL;

    U16 api_id = RRC_NULL;
    U16 msg_buf_len = RRC_NULL;
    S32 socket = RRC_NULL;
    S32 bytes_read = RRC_NULL;
    S32 flags = RRC_NULL;
    /* SPR 19078: CID 99981 Fix Start */
    S32 return_value  = RRC_NULL;
    /* SPR 19078: CID 99981 Fix End */
    static U32 counter = RRC_NULL;

    rrc_module_id_t int_dst_id = RRC_NULL;
    rrc_module_id_t src_id = RRC_NULL;

    struct timeval  zero;
    struct sctp_sndrcvinfo sndrcvinfo;
/*SPR_16533_START*/
    /* SPR 20633 changes start */
    struct sockaddr_storage from_addr;
    /* SPR 20633 changes end */
    static U32 peeled_socket_desc = 0;
    struct sctp_status status;
/*SPR_16533_END*/

    fd_set local_sctp_fd;
    socklen_t from_addr_len = 0;
/*SPR_16533_START*/
    /* SPR 20633 changes start */
    from_addr_len = sizeof(struct sockaddr_storage);
    /* SPR 20633 changes end */
/*SPR_16533_END*/

    union sctp_notification *p_notification = PNULL;

    /* Notification message types */
    struct sctp_assoc_change   * p_sn_assoc_change = PNULL;
    struct sctp_paddr_change   * p_sn_padr_change  = PNULL;

    /*L3-X2AP Drop2 Integration Start */    
    static U32 x2ap_sd_counter = RRC_NULL;

#ifdef LTE_EMBMS_SUPPORTED
    /* SPR 17998 Changes Start */
    U8 isDataFrmMceDetected = RRC_ZERO;
    /* SPR 17998 Changes End */
#endif
    s1ap_x2ap_sctp_fd s1ap_x2ap_sctp_flag = SCTP_FD_NOT_SET;
    /*L3-X2AP Drop2 Integration End */    


    lteFD_ZERO(&local_sctp_fd);
    memset( &from_addr, 0, sizeof(from_addr)); /*SPR 19078 - CoverityId-99984*/

    /* BUG:9050 */
    if ((!connection_count) && (!g_sctp_sd_count)
       )
    {
        return 0;
    }
    /* BUG:9050 */

    local_sctp_fd = sctp_fd;

    zero.tv_sec = 0;
    zero.tv_usec = 0;
    /* BUG:9050 */
    S32 opt_val = 0;
    socklen_t opt_len = 0;
    opt_len= sizeof(opt_val);
    /* BUG:9050 */

    memset_wrapper(&buffer, 0, MAX_BUFFER_SIZE);
    memset_wrapper((struct sctp_sndrcvinfo *)&sndrcvinfo, 0, sizeof(struct sctp_sndrcvinfo));

    if (SCTP_SOCKET_ERROR == select_wrapper(FD_SETSIZE - 1, &local_sctp_fd, 0, 0, &zero))
    {
        RRC_TRACE(RRC_FATAL,\
                "Problem with select in l3_sctp_receive. System error code %d",\
                errno);
        ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
        return 0;
    }


    /* check which socket descriptor is active*/
    for(counter = 0 ; counter < connection_count; counter++)
    {
            if ( (INVALID_SCTP_SOCKET != sctp_sd_list[counter]) &&
                  lteFD_ISSET((S32)sctp_sd_list[counter], &local_sctp_fd))
            {
                socket = sctp_sd_list[counter];
                RRC_TRACE(RRC_BRIEF, "SD is set for sctp_sd %d ",socket);
                s1ap_x2ap_sctp_flag = S1AP_SCTP_FD_SET; /*L3-X2AP Drop2 Integration */
                break;
            }
    }

    /*L3-X2AP Drop2 Integration Start */
    for(x2ap_sd_counter = 0 ; x2ap_sd_counter < g_sctp_sd_count; x2ap_sd_counter++)
    {

        if ( (INVALID_SCTP_SOCKET != g_sctp_sd_list[x2ap_sd_counter]) &&
                lteFD_ISSET((S32)g_sctp_sd_list[x2ap_sd_counter], &local_sctp_fd))
        {
            socket = g_sctp_sd_list[x2ap_sd_counter];
            RRC_TRACE(RRC_BRIEF,"SD is set for sctp_sd(X2AP) %d ",socket);
            s1ap_x2ap_sctp_flag = X2AP_SCTP_FD_SET;
            break;
        }
    }
#ifdef LTE_EMBMS_SUPPORTED
/* SPR 17998 Changes Start */
    if(FD_ISSET((U32)g_m2ap_sctp_sd, &local_sctp_fd))
    {
        socket = g_m2ap_sctp_sd ;
        RRC_TRACE(RRC_INFO,"SD is set for sctp_sd(M2AP) %d \n",socket);
        s1ap_x2ap_sctp_flag = M2AP_SCTP_FD_SET;
        isDataFrmMceDetected = RRC_ONE;
    }
/* SPR 17998 Changes End */
#endif


#ifndef LTE_EMBMS_SUPPORTED
    if ((counter == connection_count) && (x2ap_sd_counter == g_sctp_sd_count)
       )
#else
/* SPR 17998 Changes Start */
    if ((counter == connection_count) && (x2ap_sd_counter == g_sctp_sd_count)
        && RRC_ZERO == isDataFrmMceDetected)
/* SPR 17998 Changes End */
#endif
    {
        /* There is no activity detected on any of the sctp sockets
         * so return back successfully
         */
        return 0;
    }


    /* Reading message on the active socket */
/*SPR_16533_START*/
    /* X2AP_SCTP_PEND_CONN_IND API will not be used in One to Many type sctp conn */
    /* Code Removed */
    /* BUG:9050 */
    if (s1_non_blocking_flag[counter] == RRC_TRUE)
/*SPR_16533_END*/
    {
/*SPR 20633 Fix START*/
        for(counter = 0 ; counter < connection_count; counter++)
        {
            /* SPR 17106 Fix Start */
            if(INVALID_SCTP_SOCKET != sctp_sd_list[counter] && RRC_FALSE != s1_non_blocking_flag[counter])
            /* SPR 17106 Fix End */
            {
                return_value = getsockopt_wrapper(
                        sctp_sd_list[counter],
                        SOL_SOCKET,
                        SO_ERROR,
                        (&opt_val),
                        &opt_len);
                if(return_value < 0)
                {
                    RRC_TRACE(RRC_INFO,"problem with getsockopt_wrapper Error code :[%d]",
                            errno);
                    return 0;
                }
                if (0 == opt_val)
                {
                    RRC_TRACE(RRC_INFO,\
                            "l3_sctp_receive. communication is up");
                    socket = sctp_sd_list[counter];
                    s1ap_x2ap_sctp_flag = S1AP_SCTP_FD_SET; /*L3-X2AP Drop2 Integration */
                    api_id = SCTP_ASSOC_UP_EVENT;
                    s1_non_blocking_flag[counter] = RRC_FALSE;
                    break;
                }
                else
                {
                    RRC_TRACE(RRC_INFO,\
                            "l3_sctp_receive. communication is not up");
                    return 0;
                }
    }
        }
    }
    /* BUG:9050 */
/*SPR 20633 Fix End*/
    else
    {
    /*L3-X2AP Drop2 Integration End */

        /* Reading message on the active socket */
        if(socket < 0)
        {
            RRC_TRACE(RRC_FATAL, "l3_sctp_receive: Invalid file Descriptor");
            ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
            return 0;
        }
        /* BUG:9050 */
        if(lteFD_ISSET(socket, &local_sctp_fd))
        {
        if ( S1AP_SCTP_ERROR == (bytes_read = sctp_recvmsg(

                        socket,
                        (void *)buffer,
                        sizeof(buffer),
                            /*SPR_16533_START*/
                        (struct sockaddr *)&from_addr,
                            /*SPR_16533_END*/
                        &from_addr_len,
                        (struct sctp_sndrcvinfo *)&sndrcvinfo,
                        (int *)(&flags ))))

        {
            RRC_TRACE(RRC_ERROR,
                    "sctp_sctp_recvmsg returned failure %d flags 0x%x !!",
                    bytes_read,flags);
            ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
            return 0;
        }
        /* Checking whether it is a valid message or not */
        if(RRC_ZERO == bytes_read && RRC_ZERO == flags)
        {
            return 0;
        }

        /* Checking whether it is a message or a notification */

        RRC_TRACE(RRC_BRIEF,
                "Assoc. ID %d returned %d successfully, flags 0x%x !!",
                sndrcvinfo.sinfo_assoc_id, bytes_read,flags);

    /*L3-X2AP Drop2 Integration Start */
        if( s1ap_x2ap_sctp_flag == S1AP_SCTP_FD_SET )
        {
    /*L3-X2AP Drop2 Integration End */
            if(flags & MSG_NOTIFICATION)
            {
                p_notification = (union sctp_notification *)buffer;

                switch (p_notification->
                        sn_header.sn_type)
                {
                    case SCTP_PEER_ADDR_CHANGE:
                        {
                            RRC_TRACE(RRC_INFO,"Notification message of type "
                                    "SCTP_PEER_ADDR_CHANGE for sd %d",
                                    socket);
                            p_sn_padr_change = &(p_notification->
                                    sn_paddr_change);
                            switch (p_sn_padr_change->spc_state)
                            {
                                case SCTP_ADDRESS_UNREACHABLE:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                "Address Unreachable for sd %d",
                                                socket);
                                        return 0;
                                    }
                                case SCTP_ADDRESS_AVAILABLE:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                "Peer is available again!");
                                        return 0;
                                    }

                                default:
                                    {
                                        RRC_TRACE(RRC_INFO,"Unknown state %d", p_sn_padr_change->spc_state);
                                        return 0;
                                    }
                            }

                            break;

                        }
                    case SCTP_ASSOC_CHANGE:
                        {

                            RRC_TRACE(RRC_INFO,"Notification message of type "
                                    "SCTP_ASSOC_CHANGE for sd %d",
                                    socket);

                            p_sn_assoc_change = &(p_notification->
                                    sn_assoc_change);
                            switch(p_sn_assoc_change->sac_state)
                            {
                                case SCTP_COMMUNICATION_UP:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                " SCTP: A new association has"
                                                " just been started");
					    /*Bug 9050 fixes start*/
					    RRC_TRACE(RRC_WARNING,
							    "SCTP: : SCTP_COMMUNICATION_UP");
					    api_id = SCTP_ASSOC_UP_EVENT; 
					    break;
					    /*Bug 9050 fixes end*/
                                    }
                                case SCTP_COMMUNICATION_LOST:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                "SCTP: An SCTP association has"
                                                " just been lost for sd %d",
                                                socket);
                                        api_id = SCTP_CONN_FAILURE_IND;

                                        break;
                                    }
                                case SCTP_RESTART:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                "SCTP: Peer associated with"
                                                "the sd %d has been restarted",
                                                socket);
                                        api_id = SCTP_CONN_RECOVERY_IND;
                                        break;
                                    }
                                case SCTP_SHUTDOWN_COMPLETE:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                "SCTP: Shutdown completed with the"
                                                " peer entity associated with the sd %d",
                                                socket);
                                        return 0;
                                    }
                                case SCTP_CANT_START_ASSOC:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                "SCTP: The peer did not respond to" 
                                                " an association setup attempt");
                                /*Bug 9050 fixes start*/
                                            RRC_TRACE(RRC_WARNING,
                                                     "SCTP: : SCTP_CANT_START_ASSOC");
                                            api_id = SCTP_ASSOC_DOWN_EVENT; 
                                            break;
                                /*Bug 9050 fixes end*/
                                    }
                                default:
                                    {
                                        RRC_TRACE(RRC_WARNING,
                                                "Unknown State for"
                                                " received Notification");
                                        return 0;
                                    }
                            }/* End of switch */
                            break;
                        }
                    case SCTP_REMOTE_ERROR:
                        {
                            RRC_TRACE(RRC_WARNING,"SCTP: SCTP REMOTE ERROR");
                            return 0;
                        }

                    case SCTP_SEND_FAILED:
                        {
                            RRC_TRACE(RRC_WARNING,"SCTP: SCTP SEND FAILED");
                            return 0;

                        }
                    case SCTP_SHUTDOWN_EVENT:
                        {
                            RRC_TRACE(RRC_WARNING,
                                    "SCTP: Peer entity associated with sd %d"\
                                    " has initiated SHUTDOWN", socket);
                            api_id = SCTP_CONN_SHUTDOWN_EVENT;
                            break;
                        }
                    case SCTP_ADAPTION_EVENT:
                        {
                            RRC_TRACE(RRC_WARNING,"SCTP: SCTP ADAPTION EVENT ");
                            return 0;
                        }
                    case SCTP_PARTIAL_DELIVERY:
                        {
                            RRC_TRACE(RRC_WARNING,"SCTP PARTIAL DELIVERY");
                            return 0;
                        }


                    default:
                        {
                            RRC_TRACE(RRC_INFO,"Unknown Notification!!!!!!!");
                            return 0;
                        }
                }
            }
            else
            {
                api_id = MME_MESSAGE;
            }
    /*L3-X2AP Drop2 Integration Start */    
        }
        else if(s1ap_x2ap_sctp_flag == X2AP_SCTP_FD_SET)
        {
            if(flags & MSG_NOTIFICATION)
            {
                p_notification = (union sctp_notification *)buffer;

                switch (p_notification->
                        sn_header.sn_type)
                {
                    case SCTP_PEER_ADDR_CHANGE:
                        {
                            RRC_TRACE(RRC_INFO,"Notification message of type "
                                    "SCTP_PEER_ADDR_CHANGE for sd %d",
                                    socket);

                            p_sn_padr_change = 
                                &(p_notification->
                                        sn_paddr_change);

                            switch (p_sn_padr_change->spc_state)
                            {
                                case SCTP_ADDRESS_UNREACHABLE:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                "Address Unreachable for sd %d",
                                                socket);
                                        api_id = X2AP_SCTP_CONN_FAILURE_IND;
                                        break;
                                    }
                                case SCTP_ADDRESS_AVAILABLE:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                "Peer is available again!");
                                        break;
                                    }

                                default:
                                    {
                                        RRC_TRACE(RRC_INFO,"Unknown %d", p_sn_padr_change->spc_state);
                                        return 0;
                                    }
                            }

                            break;

                        }
                    case SCTP_ASSOC_CHANGE:
                        {

                            RRC_TRACE(RRC_INFO,"Notification message of type "
                                    "SCTP_ASSOC_CHANGE for sd %d",
                                    socket);

                            p_sn_assoc_change = &(p_notification->
                                    sn_assoc_change);

                            switch(p_sn_assoc_change->sac_state)
                            {
                                case SCTP_COMMUNICATION_UP:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                "SCTP: A new association has just been started");
                                        api_id = X2AP_SCTP_COMM_UP_IND;
                                        break;
                                    }
                                case SCTP_COMMUNICATION_LOST:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                "SCTP: An SCTP association has"
                                                " just been lost for sd %d"
                                                ,socket);
                                        api_id = X2AP_SCTP_CONN_FAILURE_IND;

                                        break;
                                    }
                                case SCTP_RESTART:
                                    {
                                        RRC_TRACE(RRC_INFO,"SCTP: Peer associated with the sd %d"\
                                                " has been restarted ",socket);
                                        api_id = X2AP_SCTP_CONN_RESTART_IND;
                                        break;
                                    }
                                case SCTP_SHUTDOWN_COMPLETE:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                "SCTP: Shutdown completed with the peer entity"\
                                                " associated with the sd %d",
                                                socket);
                                        /* Because shutdown was initiated by us, we will send 
                                         *        close indication to ap
                                         */
                                        api_id = X2AP_SCTP_CLOSED_IND;
                                        break;
                                    }
                                case SCTP_CANT_START_ASSOC:
                                    {
                                        RRC_TRACE(RRC_INFO,
                                                "SCTP: The peer did not respond to"
                                                " an association setup attempt");
                                        api_id = X2AP_SCTP_CONN_FAILURE_IND;
                                        break;
                                    }

                                default:
                                    {
                                        RRC_TRACE(RRC_WARNING,
                                                "Unknown State for received Notification");
                                        return 0;
                                    }
                            }/* End of switch */
                            break;
                        }
                    case SCTP_REMOTE_ERROR:
                        {
                            RRC_TRACE(RRC_WARNING,"SCTP: SCTP REMOTE ERROR");
                            return 0;
                        }

                    case SCTP_SEND_FAILED:
                        {
                            RRC_TRACE(RRC_WARNING,"SCTP: SCTP SEND FAILED");
                            return 0;

                        }
                    case SCTP_SHUTDOWN_EVENT:
                        {
                            RRC_TRACE(RRC_WARNING,
                                    "SCTP: Peer entity associated with sd %d"\
                                    " has initiated SHUTDOWN", socket);
                            api_id = X2AP_SCTP_SHUT_DOWN_INIT_IND;
                            /* SPR 16344 Start */
                            lteFD_CLR(socket,&sctp_fd);
                            /* SPR 16344 Stop */

                            break;
                        }
                        /* case SCTP_ADAPTATION_INDICATION: */
                    case SCTP_ADAPTION_EVENT:
                        {
                            RRC_TRACE(RRC_WARNING,"SCTP: SCTP ADAPTION EVENT ");
                            return 0;
                        }
                    case SCTP_PARTIAL_DELIVERY:
                        {
                            RRC_TRACE(RRC_WARNING,"SCTP PARTIAL DELIVERY");
                            return 0;
                        }


                    default:
                        {
                            RRC_TRACE(RRC_INFO,"Unknown Notification!!!!!!!");
                            return 0;
                        }
                }
            }
            else
            {
                api_id = X2AP_ENB_MESSAGE;
            }
        }
    /*L3-X2AP Drop2 Integration End */    
#ifdef LTE_EMBMS_SUPPORTED
        else if(s1ap_x2ap_sctp_flag == M2AP_SCTP_FD_SET)
        {
            if(flags & MSG_NOTIFICATION)
            {
                p_notification = (union sctp_notification *)buffer;

                switch (p_notification->sn_header.sn_type)
                {
                    case SCTP_PEER_ADDR_CHANGE:
                    {
                        RRC_TRACE(RRC_INFO,"Notification message of type "
                                            "SCTP_PEER_ADDR_CHANGE for sd %d\n",
                                            socket);

                        p_sn_padr_change =
                                        &(p_notification->
                                                sn_paddr_change);

                        switch (p_sn_padr_change->spc_state)
                        {
                            case SCTP_ADDRESS_UNREACHABLE:
                            {
                                RRC_TRACE(RRC_INFO,
                                           "Address Unreachable for sd %d\n",
                                            socket);
                                break;
                            }
                            case SCTP_ADDRESS_AVAILABLE:
                            {
                                RRC_TRACE(RRC_INFO,
                                           "Peer is available again!\n");
                                break;
                            }
                            default:
                            {
                                RRC_TRACE(RRC_INFO,"Unknown!!!!!!!\n");
                                return 0;
                            }
                        }
                        break;
                    }
                    case SCTP_ASSOC_CHANGE:
                    {
                        RRC_TRACE(RRC_INFO,"Notification message of type "
                                         "SCTP_ASSOC_CHANGE for sd %d\n",
                                         socket);

                        p_sn_assoc_change = &(p_notification->
                                            sn_assoc_change);

                        switch(p_sn_assoc_change->sac_state)
                        {
                            case SCTP_COMMUNICATION_UP:
                            {
                                RRC_TRACE(RRC_INFO,
                                             "SCTP: A new association has just been started");
                                api_id = M2AP_SCTP_ASSOC_UP_EVENT;
                                break;
                            }
                            case SCTP_COMMUNICATION_LOST:
                            {
                                RRC_TRACE(RRC_INFO,
                                              "SCTP: An SCTP association has"
                                              " just been lost for sd %d"
                                               ,socket);
                                api_id = M2AP_SCTP_CONN_FAILURE_IND;
                                break;
                            }
                            case SCTP_RESTART:
                            {
                                RRC_TRACE(RRC_INFO,"SCTP: Peer associated with the sd %d"\
                                                 " has been restarted ",socket);
                                api_id = M2AP_SCTP_CONN_RECOVERY_IND;
                                break;
                            }
                            case SCTP_SHUTDOWN_COMPLETE:
                            {
                                RRC_TRACE(RRC_INFO,
                                           "SCTP: Shutdown completed with the peer entity"\
                                           " associated with the sd %d",
                                           socket);
                                break;
                            }
                            case SCTP_CANT_START_ASSOC:
                            {
                                RRC_TRACE(RRC_INFO,
                                             "SCTP: The peer did not respond to"
                                             " an association setup attempt");
                                api_id = M2AP_SCTP_ASSOC_DOWN_EVENT;
                                break;
                            }
                            default:
                            {
                                RRC_TRACE(RRC_WARNING,
                                             "Unknown State for received Notification");
                                return 0;
                            }
                        }/* End of switch */
                        break;
                    }
                    case SCTP_REMOTE_ERROR:
                    {
                        RRC_TRACE(RRC_WARNING,"SCTP: SCTP REMOTE ERROR");
                        return 0;
                    }
                    case SCTP_SEND_FAILED:
                    {
                        RRC_TRACE(RRC_WARNING,"SCTP: SCTP SEND FAILED");
                        return 0;

                    }
                    case SCTP_SHUTDOWN_EVENT:
                    {
                        RRC_TRACE(RRC_WARNING,
                                "SCTP: Peer entity associated with sd %d"\
                                " has initiated SHUTDOWN", socket);
                        api_id = M2AP_SCTP_CONN_SHUTDOWN_EVENT;
                        break;
                    }
                    /* case SCTP_ADAPTATION_INDICATION: */
                    case SCTP_ADAPTION_EVENT:
                    {
                        RRC_TRACE(RRC_WARNING,"SCTP: SCTP ADAPTION EVENT ");
                        return 0;
                    }
                    case SCTP_PARTIAL_DELIVERY:
                    {
                        RRC_TRACE(RRC_WARNING,"SCTP PARTIAL DELIVERY");
                        return 0;
                    }


                    default:
                    {
                        RRC_TRACE(RRC_INFO,"Unknown Notification!!!!!!!\n");
                        return 0;
                    }
                }
            }
            else
            {
                api_id = MCE_MESSAGE;
            }
        }
#endif


        else
        {
            RRC_TRACE(RRC_INFO,
                    "Socket Flag is not set although Socket activity received");
        }
        }
        else
        {
            return 0;
        }
        /* BUG:9050 */
    }


    /* Computing total length */
/*SPR_16533_START*/
#ifdef LTE_EMBMS_SUPPORTED
    if (( s1ap_x2ap_sctp_flag == S1AP_SCTP_FD_SET ) ||
        ( s1ap_x2ap_sctp_flag == M2AP_SCTP_FD_SET )
       )
#else
    if ( s1ap_x2ap_sctp_flag == S1AP_SCTP_FD_SET )
#endif
    {
        msg_buf_len = (U16)(RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE + 
                SCTP_STREAM_ID_SIZE + bytes_read);
        msg_buf = (S8*) rrc_msg_mem_get(msg_buf_len);

        if (msg_buf == PNULL)
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR,"Memory Allocation failed in sctp_receive");
            return 0;
        }

        s1ap_set_sctp_sd(
                (U32*) (msg_buf + RRC_API_HEADER_SIZE) ,socket);
        s1ap_set_sctp_stream(
                (U16*) (msg_buf + RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE) ,
                sndrcvinfo.sinfo_stream);


        if(bytes_read != 0)
        {
            l3_memcpy_wrapper(
                    (msg_buf + RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE + 
                     SCTP_STREAM_ID_SIZE),
                    buffer,
                    (U32)bytes_read);      /* copying received ASN message */
        }


    }
    if ( s1ap_x2ap_sctp_flag == X2AP_SCTP_FD_SET )
    {
        /* FOR X2AP case add the client address info in the buffer */
            /* SPR 20633 changes start */
            msg_buf_len = (U16)(RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE + 
                    SCTP_STREAM_ID_SIZE + sizeof(struct sockaddr_storage) + bytes_read);
            /* SPR 20633 changes end */
        /*SPR_18216_START*/
        /*Code Moved*/
        /*SPR_18216_END*/
        peeled_socket_desc = socket;

        if(api_id == X2AP_ENB_MESSAGE)
        {
            /* check assoc_id presence */
            U8 indx = 0;
            /*SPR_16533_DELTA1_START*/
            for(indx = 0; indx < MAX_PEER_ENB;indx ++)
                /*SPR_16533_DELTA1_END*/
            {
                if(g_stored_assoc_info[indx].assoc_id == (U32)sndrcvinfo.sinfo_assoc_id)/*SPR 17777 +-*/
                {
                    RRC_TRACE(RRC_DETAILED,"Msg recieved on already added assoc. id on socket = %d ",socket);
                    break;
                }
                else if((g_stored_assoc_info[indx].assoc_id == 0) && 
                        (g_stored_assoc_info[indx].socket == INVALID_SCTP_SOCKET))
                {
                    /* Bug-4680 FIX START */
                    if( g_sctp_sd_count >= MAX_PEER_ENB+1)
                    {
                        RRC_TRACE(RRC_ERROR,"The MAX g_sctp_sd_count is already reached %d\n",g_sctp_sd_count);
                        /* coverity-101191 START */
                        /* code deleted */
                        /* coverity-101191 END */

                        return 0;
                    }
                    /* Bug-4680 FIX END */

                    /* new association */
                    /*SPR_18216_START*/
                    peeled_socket_desc = sctp_peeloff(socket, sndrcvinfo.sinfo_assoc_id);
                    if(peeled_socket_desc == INVALID_SCTP_SOCKET)
                    {
                        RRC_TRACE(RRC_INFO,"sctp_peeloff returned invalid socket descriptor ");
                        return 0;
                    }
                    g_stored_assoc_info[indx].socket = peeled_socket_desc;
                    g_stored_assoc_info[indx].assoc_id = sndrcvinfo.sinfo_assoc_id;
                    /*SPR_18216_END*/
                    opt_len = sizeof(status);
		    /* SPR 19078: CID 99981 Fix Start */
		    return_value = getsockopt_wrapper(
					    peeled_socket_desc,
					    SOL_SCTP,
					    SCTP_STATUS,
					    &status,	
					    &opt_len);
    		    if (return_value < 0)
                {
                    RRC_TRACE(RRC_INFO,"SCTP : Unable to retreive Number of streams Error code :[%d] : %s ",errno,strerror(errno));
                    return 0;/*SPR 19078:CoverityId 107945+-*/
                }
		    else
		    {
				/* Bug-4680 FIX START */
			    RRC_TRACE(RRC_INFO,
                            "Peeled off ASSOC. ID = %d new SD = %d, no. of in streams = %d , %d, g_sctp_sd_count=%d\n",
                            status.sstat_assoc_id,peeled_socket_desc,status.sstat_instrms,status.sstat_outstrms,g_sctp_sd_count);
				/* Bug-4680 FIX END */
			    lteFD_SET((S32)peeled_socket_desc, &(sctp_fd));
			    g_sctp_sd_list[g_sctp_sd_count] = peeled_socket_desc;
			    g_sctp_sd_count++;
			    break;
		    }
		    /* SPR 19078: CID 99981 Fix End */
		}
	    }
            if(indx == MAX_PEER_ENB)
            {
                RRC_TRACE(RRC_ERROR,"All Associations are full");
                /* SPR 19078: Coverity Fix 101191 Start */
                /* Code Deleted */
                /* SPR 19078: Coverity Fix 101191 End */
                return 0;
            }
        }
        /*SPR_18216_START*/
        msg_buf = (S8*) rrc_msg_mem_get(msg_buf_len);
        /*SPR_18216_END*/
        if (msg_buf == PNULL)
        {
            /* Not enough memory */
            RRC_TRACE(RRC_ERROR,"Memory Allocation failed in sctp_receive");
            return 0;
        }


        s1ap_set_sctp_sd(
                (U32*) (msg_buf + RRC_API_HEADER_SIZE) ,peeled_socket_desc);
        s1ap_set_sctp_stream(
                (U16*) (msg_buf + RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE) ,
                sndrcvinfo.sinfo_stream);

        s1ap_set_sctp_addr(
                (U8*) (msg_buf + RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE + SCTP_STREAM_ID_SIZE) ,&from_addr);
        if(bytes_read != 0)
        {
            /* SPR 20633 changes start */
                l3_memcpy_wrapper(
                        (msg_buf + RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE + 
                         SCTP_STREAM_ID_SIZE + sizeof(struct sockaddr_storage)),
                        buffer,
                        (U32)bytes_read);      /* copying received ASN message */
            /* SPR 20633 changes end */
        }


    }
    /*SPR_16533_END*/
    /*L3-X2AP Drop2 Integration Start */ 
    if ( s1ap_x2ap_sctp_flag == S1AP_SCTP_FD_SET )
    {
        src_id = RRC_MME_MODULE_ID;
        int_dst_id = RRC_S1AP_MODULE_ID;
    }
    else if ( s1ap_x2ap_sctp_flag == X2AP_SCTP_FD_SET )
    {
        src_id = RRC_X2AP_PEER_eNODEB_MODULE_ID;
        int_dst_id = RRC_X2AP_MODULE_ID;
    }
    /*L3-X2AP Drop2 Integration End */ 
#ifdef LTE_EMBMS_SUPPORTED
    else if ( s1ap_x2ap_sctp_flag == M2AP_SCTP_FD_SET )
    {
        src_id = RRC_MCE_MODULE_ID;    
        int_dst_id = RRC_M2AP_MODULE_ID;
    }
#endif


    /* Fill the RRC API header */
    /* SPR 19078: CID 99982 Fix Start */
    if (msg_buf != PNULL)
    {
    	rrc_construct_api_header(
            	(U8 *)msg_buf,
            	RRC_VERSION_ID,
            	src_id,
            	int_dst_id,
            	api_id,
            	msg_buf_len);
    }
    else
    {
        RRC_TRACE(RRC_ERROR,"msg_buf is NULL");
    }
    /* SPR 19078: CID 99982 Fix End */
    *sender         = qvGetService(src_id);
    *receiver       = qvGetService(int_dst_id);
    *priority       = 0;

#ifdef LATENCY_TRACE_ON

    struct timeval  tv;
    struct timezone tz;
    struct tm      *tm;
    long    start;

    gettimeofday_wrapper(&tv, &tz);
    tm = localtime_wrapper(&tv.tv_sec);

    fprintf_wrapper(stderr,"TIMESTAMP-SCTP recv\t  %d:%02d:%02d:%d (~%ld ms)", tm->tm_hour,
		    tm->tm_min, tm->tm_sec, tv.tv_usec,
		    tm->tm_hour * 3600 * 1000 + tm->tm_min * 60 * 1000 +
		    tm->tm_sec * 1000 + tv.tv_usec / 1000);

    start = tm->tm_hour * 3600 * 1000 + tm->tm_min * 60 * 1000 +
	    tm->tm_sec * 1000 + tv.tv_usec / 1000;

    fprintf_wrapper(stderr,"SCTP Enter time: %ld",start);

#endif

    RRC_ASSERT(0 != *sender);
    RRC_ASSERT(0 != *receiver);
    if(s1ap_x2ap_sctp_flag == X2AP_SCTP_FD_SET)
    {
        RRC_TRACE(RRC_DETAILED,"Message is sent to X2AP module");
    }

    else if (s1ap_x2ap_sctp_flag == S1AP_SCTP_FD_SET)
    {
        RRC_TRACE(RRC_INFO,"Message is sent to S1AP module\n");
    }
#ifdef LTE_EMBMS_SUPPORTED

    else
    {
        RRC_TRACE(RRC_INFO,"Message is sent to M2AP module\n");
    }
#endif

    return msg_buf;
    
}


#endif

/*SPR 15570 Fix Start*/
/******************************************************************************
 *   Function Name     : l3_retry_connect
 *   Input             : p_s1ap_gb_ctx - Pointer to s1ap global context
 *                       p_mme_comm_info   -      Pointer to MME related
 *                                                communication Information.
 *   Output            : None 
 *   Returns           : Returns connected socket descriptor on Success or -1 on
 *                       Error.
 *   Description       : This Function performs the sequence of steps to gain a
 *                       connected socket.
 ******************************************************************************/
S32 l3_retry_connect(
		s1ap_gb_context_t *p_s1ap_gb_ctx,
		mme_context_t *p_mme_ctx)
{
	S32 sockfd = RRC_NULL ;
#ifdef LKSCTP
	fd_set local_sctp_fd;
#else
	sctp_fd_set_st local_sctp_fd;
#endif
	U32  counter       = RRC_NULL;
	U32 number_of_streams = RRC_NULL;
	U32 noDelay = 1;
	socklen_t opt_len = 0;
	S8 family        = PF_INET;
	S32 return_value  = RRC_NULL;
	ip_addr_t *p_mme_addr = NULL;
	ipv6_addr_t *p_mme_addr_ipv6 = NULL;
	struct timeval  zero;
#ifdef LKSCTP
	struct sctp_status status;
#else
	sctp_status_st status;
#endif
	struct sockaddr_in servaddr[MAX_IP_ADDR];
	struct sockaddr_in6 servaddr6[MAX_IP_ADDR];
	struct sctp_event_subscribe events;
	struct sctp_paddrparams paddr;
#ifdef LKSCTP
	struct sctp_rtoinfo rtoinfo;
#else
	sctp_rtoinfo_st rtoinfo;
#endif
	s1ap_sctp_conf_info_t *p_sctp_params = PNULL;
	mme_comm_info_t *p_mme_comm_info = &(p_mme_ctx->connection_info);
	U32 heartbeat_int = p_mme_comm_info->heart_beat_timer;
	U8 no_mme_ip_addr = p_mme_comm_info->num_ip_addr;
	U16 mme_port = p_mme_comm_info->port;

	zero.tv_sec = 0;
	zero.tv_usec = 0;
#ifdef LKSCTP
	FD_ZERO(&local_sctp_fd);
	FD_SET((S32)(p_mme_ctx->sctp_sd), &local_sctp_fd);
#else
    	SCTP_FD_ZERO(&local_sctp_fd);
	SCTP_FD_SET((U32)(p_mme_ctx->sctp_sd),&local_sctp_fd);
#endif


#ifdef LKSCTP
	return_value = select_wrapper(FD_SETSIZE - 1,0 , &local_sctp_fd, 0, &zero);
#else
	return_value = sctp_select(SCTP_FD_SETSIZE - 1,0, &local_sctp_fd, 0, &zero);
#endif
	if (SCTP_SOCKET_ERROR == return_value)
	{
		RRC_TRACE(RRC_FATAL,\
				"Problem with select in l3_retry_connect System error code %d",\
				errno);
		return INVALID_SCTP_SOCKET;
	}
	else 
	{
#ifdef LKSCTP
		return_value = FD_ISSET (p_mme_ctx->sctp_sd , &local_sctp_fd);
#else
		return_value = SCTP_FD_ISSET((U32)(p_mme_ctx->sctp_sd), &local_sctp_fd);
#endif
		if ( return_value)
		{
			sockfd = p_mme_ctx->sctp_sd ;
#ifdef LKSCTP
			opt_len = sizeof(status);
			return_value = getsockopt_wrapper(
					sockfd,
					SOL_SCTP,
					SCTP_STATUS,
					&status,	
					&opt_len);
#else
			return_value = sctp_getsockopt(
					sockfd,
					IPPROTO_SCTP,
					SCTP_SO_STATUS,
					&status,
					&opt_len);
#endif

			if (return_value < 0 || !(SCTP_COOKIE_ECHOED == status.sstat_state || SCTP_ESTABLISHED == status.sstat_state))
			{
				RRC_TRACE(RRC_INFO,"SCTP : Link is not up. Error code :[%d]",errno);
				return S1AP_SCTP_ERROR;
			}
			else
			{
				number_of_streams = status.sstat_outstrms;
				if(status.sstat_outstrms  <  2 ||
						status.sstat_instrms <  2)
				{
					RRC_TRACE(RRC_WARNING,
							"SCTP : Less Number of streams have been opened");
					return S1AP_SCTP_ERROR;
				}
				else if (status.sstat_outstrms  >  10 )
				{
					RRC_TRACE(RRC_WARNING,"SCTP : More than 10 streams are open");
					return S1AP_SCTP_ERROR;
				}
				else
				{
					(p_mme_ctx->stream_info).number_of_streams = number_of_streams;
					for (counter = 0; counter < number_of_streams; counter++)
					{
						(p_mme_ctx->stream_info).stream_ids[counter] = counter;
					}

				}
			}

			memset_wrapper(&rtoinfo, 0, sizeof(rtoinfo));

			rtoinfo.srto_assoc_id = sockfd;
			/*sctp parameters start*/
            /* coverity_fix 86807 ++*/
            if(RRC_S1AP_SCTP_CONFIG_PARAMS_PRESENT == p_s1ap_gb_ctx->bitmask)
            {
                p_sctp_params = &(p_s1ap_gb_ctx->s1ap_sctp_conf_param);
            }
            /* coverity_fix 86807 --*/

			if(PNULL != p_sctp_params)
			{
				rtoinfo.srto_initial = p_sctp_params->rto_initial;
				rtoinfo.srto_max = p_sctp_params->rto_max;
				rtoinfo.srto_min = p_sctp_params->rto_min;
			}
			else
			{
				rtoinfo.srto_initial = RRC_SCTP_DEFAULT_RTO_INITIAL;
				rtoinfo.srto_max = RRC_SCTP_DEFAULT_RTO_MAX;
				rtoinfo.srto_min = RRC_SCTP_DEFAULT_RTO_MIN;
			}
			/*sctp parameters end*/
#ifdef LKSCTP

			return_value =  setsockopt_wrapper( sockfd,IPPROTO_SCTP , SCTP_RTOINFO,
					(const void *)&rtoinfo, sizeof(rtoinfo));
#else
			return_value = sctp_setsockopt(
					sockfd,
					IPPROTO_SCTP,
					SCTP_SO_RTOINFO,
					(const void *)&rtoinfo,
					sizeof(rtoinfo));

#endif

			if (return_value < 0)
			{
				RRC_TRACE(RRC_WARNING,"SCTP : RTO Interval is not set.");

				/* setting einprogress_retry_count to avoid
				   more retries on this sockfd. This sockfd 
				   has to be closed */
				p_mme_ctx->sctp_einprogress_retry_count =
					SCTP_SELECT_RETRY_COUNT - 1;

				return S1AP_SCTP_ERROR;

			}

			RRC_TRACE(RRC_DETAILED,"SCTP : RTO Interval is set.");
			for (counter = 0; counter < no_mme_ip_addr; counter++)
			{
				memset_wrapper(&paddr, 0, sizeof(struct sctp_paddrparams));

				if(p_mme_comm_info->bitmask & MME_COMM_INFO_IPV6_ADDR_PRESENT)
				{
					p_mme_addr_ipv6 = &(p_mme_comm_info->ipv6_addr[0]);
					bzero_wrapper( (void *)&servaddr6[counter], sizeof(servaddr6[counter]) );
					servaddr6[counter].sin6_family     = 10;
					servaddr6[counter].sin6_port        = htons_wrapper(mme_port);
					if(inet_pton_wrapper(10,(const char *)p_mme_addr_ipv6->ipv6_addr, 
								(void*)&servaddr6[counter].sin6_addr)!=1)
					{
						RRC_TRACE(RRC_WARNING,"Couldn't convert INET6 adress while setting HeartBeat");
						return S1AP_SCTP_ERROR;
					}
					/*valgrind_fix start*/
					l3_memcpy_wrapper(&paddr.spp_address,&servaddr6,sizeof(struct sockaddr_in6));
					/*valgrind_fix stop*/
					p_mme_addr_ipv6++;
				}
				else
				{
					p_mme_addr = &(p_mme_comm_info->ip_addr[0]);

					bzero_wrapper( (void *)&servaddr[counter], sizeof(servaddr[counter]) );
					servaddr[counter].sin_family      = (SA_FAMILY_T)family;
					servaddr[counter].sin_port        = htons_wrapper(mme_port);
					servaddr[counter].sin_addr.s_addr = 
						inet_addr_wrapper((const char*) p_mme_addr->ip_addr );
					/*valgrind_fix start*/
					l3_memcpy_wrapper(&paddr.spp_address,&servaddr,sizeof(struct sockaddr_in));
					/*valgrind_fix stop*/
					p_mme_addr++;
				}

				paddr.spp_assoc_id                = (U32)sockfd;
				paddr.spp_hbinterval              = heartbeat_int;
#ifdef LKSCTP
				paddr.spp_flags                   = SPP_HB_ENABLE;
#endif
				/*sctp parameters start*/
				if(PNULL != p_sctp_params)
				{
					paddr.spp_pathmaxrxt = p_sctp_params->path_max_retrans;
				}
				else
				{
					paddr.spp_pathmaxrxt = RRC_SCTP_DEFAULT_PATH_MAX_RTX;
				}
				/*sctp parameters end*/
#ifdef LKSCTP
				return_value = setsockopt_wrapper( sockfd,IPPROTO_SCTP , SCTP_PEER_ADDR_PARAMS,
						(const void *)&paddr, sizeof(paddr));
#else
				return_value = sctp_setsockopt(
						sockfd,
						IPPROTO_SCTP,
						SCTP_PEER_ADDR_PARAMS,
						(const void *)&paddr,
						sizeof(paddr));

#endif


				if (return_value < 0)
				{
					RRC_TRACE(RRC_WARNING,"SCTP : HeartBeat Interval is not set.");

					/* setting einprogress_retry_count to avoid
					   more retries on this sockfd. This sockfd 
					   has to be closed */
					p_mme_ctx->sctp_einprogress_retry_count =
						SCTP_SELECT_RETRY_COUNT - 1;

					return S1AP_SCTP_ERROR;
				}
				RRC_TRACE(RRC_DETAILED,"SCTP : HeartBeat Interval is set.");

			}



			memset_wrapper( (void *)&events, 0, sizeof(events) );

			/* Populating SCTP Events structure*/
			events.sctp_association_event  = 1;
			events.sctp_data_io_event      = 1;
			events.sctp_address_event      = 1;
			events.sctp_send_failure_event = 1;
			events.sctp_peer_error_event   = 1;
			events.sctp_shutdown_event     = 1;
#ifdef LKSCTP
			return_value = setsockopt_wrapper(sockfd, SOL_SCTP, SCTP_EVENTS, &events,
					sizeof (events));
#else

			return_value = sctp_setsockopt(
					sockfd,
					IPPROTO_SCTP,
					SCTP_SO_SET_EVENTS,
					&events,
					sizeof(events));
#endif
			if (return_value < 0)
			{
				RRC_TRACE(RRC_WARNING,"SCTP : sctp_setsockopt for setting events failed");

				/* setting einprogress_retry_count to avoid
				   more retries on this sockfd. This sockfd 
				   has to be closed */
				p_mme_ctx->sctp_einprogress_retry_count =
					SCTP_SELECT_RETRY_COUNT - 1;

				return S1AP_SCTP_ERROR;
			}
			RRC_TRACE(RRC_BRIEF,"SCTP : sctp_setsockopt for setting events Success");

#ifdef LKSCTP
			return_value =  setsockopt_wrapper(
					sockfd,
					SOL_SCTP,
					SCTP_NODELAY,
					&noDelay,
					sizeof(noDelay));
#else
			return_value = sctp_setsockopt(
					sockfd,
					IPPROTO_SCTP,
					SCTP_SO_NODELAY,
					&noDelay,
					sizeof(noDelay));

#endif
			if (return_value < 0)
			{
				RRC_TRACE(RRC_WARNING,"SCTP :"
						"setsockopt(NODELAY) failed");

				/* setting einprogress_retry_count to avoid
				   more retries on this sockfd. This sockfd 
				   has to be closed */
				p_mme_ctx->sctp_einprogress_retry_count =
					SCTP_SELECT_RETRY_COUNT - 1;

				return S1AP_SCTP_ERROR;
			}



			for(counter = 0; counter < MAX_NUM_MME ;counter++)
			{
				if (INVALID_SCTP_SOCKET == sctp_sd_list[counter])
				{
					sctp_sd_list[counter] = sockfd;
					s1_non_blocking_flag[counter] = RRC_TRUE;
					connection_count++;
					break;
				}
			}


			RRC_TRACE(RRC_DETAILED,
					"SCTP : connection_count incremented to %d",connection_count);
#ifdef LKSCTP
			FD_SET((S32)sockfd,&sctp_fd);
            /*SPR 20633 Start*/
			init_stack_flag = RRC_TRUE;
            /*SPR 20633 End*/
#else
			SCTP_FD_SET((U32)(sockfd),&sctp_fd);
            /*SPR 20633 Start*/
			init_stack_flag_v4 = RRC_TRUE;
			init_stack_flag_v6 = RRC_TRUE;
            /*SPR 20633 End*/
#endif
			return sockfd ;
		}

	}

	return INVALID_SCTP_SOCKET;
}

/*SPR 15570 Fix Stop*/




