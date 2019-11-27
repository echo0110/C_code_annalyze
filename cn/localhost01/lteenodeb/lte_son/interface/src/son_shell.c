/****************************************************************************
*
* ARICENT -
*
* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
* SPR-612 Coverity Fix Starts
* $Id: son_shell.c $
* SPR-612 Coverity Fix Ends
*
****************************************************************************
*
* File Description: This file contains the definitions of QSHELL, QSYSOP, QWAIT
*                   structures and their member functions of CSPL which are
*                   used by SON for memory allocation, external communication etc.
*
****************************************************************************
* Revision Details
* ----------------
*
* SPR-612 Coverity Fix Starts
* $Log: son_shell.c $
* SPR-612 Coverity Fix Ends
* Revision 0.1  2012/01/02 09:50:42  gur24562
* Initial code
*
****************************************************************************/

/****************************************************************************
* Project Includes
****************************************************************************/
#include <son_utils.h>
/*+ SPR 18399*/
/*- SPR 18399*/

/* SPR 20585 Fix Start */
#include "pthread.h"
/* SPR 20585 Fix End */
#define NEXT_EXPIRY_TIMER 500

const son_8 *p_shell_facility_name = "SON_SHELL"; /*S_global variable used in SON Shell*/
static fd_set son_fd_set;
son_u32 max_sd_value = 0;
son_s8 son_oam_addr[IP_PORT_STR_LEN] = {0};
son_s8 son_rrm_addr[IP_PORT_STR_LEN] = {0};
son_s8 son_rrc_addr[IP_PORT_STR_LEN] = {0};
son_s8 son_phy_addr[IP_PORT_STR_LEN] = {0};
/* PR 7715 Fix Start */
extern const son_s8 *SON_MIF_OAM_EVENT_NAMES[];
/* PR 7715 Fix End */

/*+ SPR 18399*/
/*- SPR 18399*/
/*****************************************************************************
 * Function Name  : son_malloc
 * Inputs         : size - number of memory bytes to be allocated
 * Outputs        : None
 * Returns        : Pointer to the allocated memory
 * Description    : It is the wrapper function for the host OS malloc routine
 ****************************************************************************/
/*SPR 18200 Start*/
son_void_t *
/*SPR 18200 end*/
son_malloc
(
    son_u32 size
)
{
    return malloc(size);
}

/*****************************************************************************
 * Function Name  : son_free
 * Inputs         : p_msg - Pointer to the memory to be freed
 * Outputs        : None
 * Returns        : None
 * Description    : It is the wrapper function for the host OS free routine
 ****************************************************************************/
static son_void_t
son_free
(
    son_void_t *p_msg
)
{
    free(p_msg);
}

/*****************************************************************************
 * Function Name  : son_vprintf
 * Inputs         : p_format - format string
 *                  arg - variable arguments list
 * Outputs        : None
 * Returns        : number of characters printed
 *                  a negative value in case of output error
 * Description    : It is the wrapper function for the host OS vfprintf routine
 *
 ****************************************************************************/
static son_s32
son_vprintf
(
    const char *p_format,
    va_list arg
)
{
    return vfprintf(stderr, p_format, arg);
}

/*****************************************************************************
 * Function Name  : son_abort
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : It is the wrapper function for the host OS abort routine
 *
 ****************************************************************************/
/* + SPR 17439 */
son_void_t
son_abort(void)
/* - SPR 17439 */
{
    abort();
}

/*****************************************************************************
 * Function Name  : son_walltime
 * Inputs         : None
 * Outputs        : p_now_time - Pointer to the current day time value
 * Returns        : None
 * Description    : Gets the current system time in seconds and microseconds,
 *                  and fill up the QTIME structure passed.
 ****************************************************************************/
static son_void_t
son_walltime
(
    QTIME *p_now_time
)
{
    struct timeval T = {0};

    SON_ASSERT(SON_PNULL != p_now_time);
    son_gettimeofday( &T, 0 );
    p_now_time->s = T.tv_sec;
    p_now_time->us = T.tv_usec;
}

/*****************************************************************************
 * Function Name  : son_timedwait
 * Inputs         : p_info_listener - Pointer to the listener object
 *                  p_qtime - Time for which to wait for data
 * Outputs        : None
 * Returns        : None
 * Description    : It waits until either the listener object is ready for
 *                  read, or until the specified timeout, if any has expired.
 ****************************************************************************/
static son_void_t
son_timedwait
(
    son_void_t *p_info_listener,
    const QTIME *p_qtime
)
{
    struct timeval timeout = {0};

    SON_ASSERT(SON_PNULL != p_info_listener);
    son_listener_t *p_son_listener = (son_listener_t *)p_info_listener;

    /* Clearing son_fd_set by filling all zeros. */
    FD_ZERO(&son_fd_set);

    /* Inserting sockets into son_fd_set. */
    FD_SET(p_son_listener->sd_udp_rrm, &son_fd_set);
    FD_SET(p_son_listener->sd_udp_rrc, &son_fd_set);
    FD_SET(p_son_listener->sd_udp_phy, &son_fd_set);
    FD_SET(p_son_listener->sd_udp_oam, &son_fd_set);

    if (SON_PNULL != p_qtime)
    {
        timeout.tv_sec = p_qtime->s;
        timeout.tv_usec = p_qtime->us;
    }
    else
    {
        timeout.tv_sec = 0;
        timeout.tv_usec = NEXT_EXPIRY_TIMER;
    }

    if (-1 == select(max_sd_value + 1, &son_fd_set, 0, 0, &timeout))
    {
        SON_ENCAPS_WARNING("son_timedwait: Select failed for SON\n");
    }
} /* son_timedwait */

/*****************************************************************************
 * Function Name  : son_sockaddr
 * Inputs         : p_ip_add_port_str - Pointer to the IP and Port string
 *                  p_sockaddr - Pointer to the socket address
 * Outputs        : None
 * Returns        : SON_TRUE  - Socket IP address and port is received
 *                : SON_FALSE - Socket IP address and port is not received
 * Description    : It is used to get socket ip address and port.
 ****************************************************************************/
static son_bool_et
son_sockaddr
(
    son_s8 *p_ip_add_port_str,
    struct sockaddr_in *p_sockaddr
)
{
    son_bool_et ret       = SON_FALSE;
    son_s8 *p_ip_addr_str = SON_PNULL;
    son_s8 *p_port_str    = SON_PNULL;

    if (SON_PNULL == p_ip_add_port_str)
    {
        SON_ENCAPS_WARNING("son_sockaddr: Pointer to IP port string is NULL");
    }
    else
    {
        /* Typecast result of strtok to son_s8 pointer to remove compilation
         * warning */
        p_ip_addr_str = (son_s8 *)SON_STRTOK(p_ip_add_port_str, ":");
        if (SON_PNULL == p_ip_addr_str)
        {
            SON_ENCAPS_WARNING("son_sockaddr: Failed to find server IP address");
        }
        else
        {
            /* Typecast result of strtok to son_s8 pointer to remove compilation
             * warning */
            p_port_str = (son_s8 *)SON_STRTOK(SON_PNULL, ":");
            if (SON_PNULL == p_port_str)
            {
                SON_ENCAPS_WARNING("son_sockaddr: Failed to find server port number");
            }
            else
            {
                p_sockaddr->sin_family = PF_INET;
                /* Typecast atoi result to son_u16 to remove klocwork warning */
                p_sockaddr->sin_port = htons((son_u16)son_atoi((const char *)p_port_str));
                p_sockaddr->sin_addr.s_addr = inet_addr((const char *)p_ip_addr_str);
                ret = SON_TRUE;
            }
        }
    }

    return ret;
} /* son_sockaddr */

/*****************************************************************************
 * Function Name  : son_send
 * Inputs         : p_info_listener - Pointer to the listener object
 *                  p_message - Pointer to the message buffer
 *                  p_sender - Module which sent the message
 *                  p_receiver - Module which should receive the message
 *                  p_data - Pointer to module specific data registered with CSPL
 *                  size - Size of the message
 * Outputs        : None
 * Returns        : None
 * Description    : It is used by CSPl for sending message over sockets.
 ****************************************************************************/
static son_void_t
son_send
(
    son_void_t          *p_info_listener,
    const son_void_t    *p_message,
    QMODULE p_sender,
    QMODULE p_receiver,
    son_void_t          *p_data,
    son_u32 size
)
{
    son_listener_t *p_son_listener = SON_PNULL;
    struct  sockaddr_in serveraddress = {0};
    son_s32 sd_udp = -1;
    STACKAPIHDR cspl_hdr = {0};
    son_s8 ip_add_port_str[IP_PORT_STR_LEN] = {0};
    son_s32 bytes_sent  = 0;
    /*Start: OAM notification changes*/
    son_u16 event_id = SON_NULL;
    son_oam_error_info_t error_info;

    SON_MEMSET(&error_info, 0, sizeof(error_info));
    /*End: OAM notification changes*/

    SON_ASSERT(SON_PNULL != p_info_listener);
    SON_ASSERT(SON_PNULL != p_message);
    SON_ASSERT(SON_PNULL != p_data);

    son_u8 *p_msg = (son_u8 *)p_message;

    p_son_listener = (son_listener_t *)p_info_listener;

    son_parse_cspl_header(p_msg, &cspl_hdr);

    /* Extract the IP address and the port number on which to send the data */
    SON_MEMCPY(ip_add_port_str, p_data, IP_PORT_STR_LEN);
    ip_add_port_str[IP_PORT_STR_LEN - 1] = '\0';

/*+ SPR 18399*/
/*- SPR 18399*/
    /* Get Socket IP address and port number */
    if (SON_FALSE == son_sockaddr(ip_add_port_str, &serveraddress))
    {
        /*SPR 17777 +-*/
        SON_ENCAPS_WARNING("son_send: Unable to get socket address;"
                "Unused variable size=%p p_receiver=%p p_sender=%p",size,p_receiver,p_sender);
        /*SPR 17777 +-*/
    }
    else
    {
        /* Get the appropriate socket descriptor based on the destination
         * module */
        if (SON_RRM_MODULE_ID == cspl_hdr.to)
        {
            sd_udp = p_son_listener->sd_udp_rrm;
        }
        else if (SON_OAM_MODULE_ID == cspl_hdr.to)
        {
            sd_udp = p_son_listener->sd_udp_oam;

            /* Update the external header source module Id field to
             * SON_MODULE_ID */
            /* from (Higher order byte) */
            *((p_msg + CV_HDRSIZE + EXT_MSG_SRC_MODULE_ID_OFFSET))
                = (son_u8)((SON_MODULE_ID & 0xFF00) >> 8);

            /* from (Lower order byte) */
            *((p_msg + CV_HDRSIZE + EXT_MSG_SRC_MODULE_ID_OFFSET
               + 1)) = (son_u8)(SON_MODULE_ID & 0x00FF);
        }
        else if (SON_RRC_MODULE_ID == cspl_hdr.to)
        {
            sd_udp = p_son_listener->sd_udp_rrc;
        }
        else if (SON_PHY_MODULE_ID == cspl_hdr.to)
        {
            sd_udp = p_son_listener->sd_udp_phy;
        }
        else
        {
            SON_ENCAPS_WARNING("son_send: Message received with invalid destination "
                               "Id = %u", cspl_hdr.to);
        }

        /* Send the data on UDP socket */

        /* CV_HDRSIZE is decremented here as composer adds CSPL Header
         * Message API Length */
        if (-1 != sd_udp)
        {
            bytes_sent = sendto(sd_udp, p_msg + CV_HDRSIZE,
                cspl_hdr.paramlen - CV_HDRSIZE, 0, (struct sockaddr *)
                &serveraddress,
                (son_s16)sizeof(serveraddress));
            if (bytes_sent < 0)
            {
                if (errno == ENOBUFS || errno == ENOMEM)
                {
                    error_info.error_cause = SON_OAM_NO_FREE_BUFFER;
                }
                else if (errno == EINTR || errno == ENOTCONN || errno == EAGAIN
                         || errno == EDESTADDRREQ || errno == EISCONN ||
                         errno == EPIPE)
                {
                    error_info.error_cause = SON_OAM_INVALID_IPC_HANDLE;
                }
                else
                {
                    error_info.error_cause = SON_OAM_INVALID_PARAMS;
                }

                error_info.bitmask |= SON_OAM_EVENT_ERROR_CAUSE_PRESENT;
                error_info.error_type = SON_OAM_SW_PROCESSING_ERROR;

                SON_ENCAPS_WARNING("son_send: Unable to send data on UDP socket with "
                                   "socket fd = %d", sd_udp);
                /*send SON_OAM_EVENT_NOTIFICATION to OAM
                 *fill error info structure*/
                error_info.bitmask |= SON_OAM_EVENT_SEVERITY_PRESENT;
                error_info.severity = SON_OAM_MAJOR_ERROR;

                event_id = (son_u16)SON_OAM_SEND_FAIL;

                if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, SON_OAM_SOCKET_ERROR,
                        event_id, error_info))
                {
                    SON_ENCAPS_WARNING("son_send: SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
                }
            }
            /* PR 7715 Fix Start */
            else
            {
                if (SON_OAM_MODULE_ID == cspl_hdr.to)
                {
                    SON_ENCAPS_WARNING("Msg %s sent to OAM",
                            SON_MIF_OAM_EVENT_NAMES[cspl_hdr.api -
                            SON_OAM_INIT_IND]);
                }
            }
            /* PR 7715 Fix End */
        }
    }
} /* son_send */

/*****************************************************************************
 * Function Name  : son_handle_message
 * Inputs         : sd_udp - UDP socket descriptor on which data is to be
 *                  received
 *                  sender_module_id - Module identifier from which to receive
 *                  the data
 * Outputs        : p_sender - QMODULE structure for source module
 *                  p_receiver - QMODULE structure for destination module
 *                  p_priority - Priority of the message received.
 * Returns        : Pointer to the created buffer
 * Description    : It receives the data (external API header and data) at
 *                  given UDP socket and appends CSPL header to it
 ****************************************************************************/
static son_u8 *
son_handle_message
(
    son_s32 sd_udp,
    son_module_id_et sender_module_id,
    QMODULE             *p_sender,
    QMODULE             *p_receiver,
    son_s8              *p_priority
)
{
    struct sockaddr_in cli_addr = {SON_NULL};
    socklen_t length = SON_NULL;

    son_u8 ext_msg_api_hdr[SON_EXT_MAX_MSG_API_HDR_SIZE] = {SON_NULL};
    son_u8 *p_recv_buffer = SON_PNULL;
    son_s32 retval = SON_NULL;
    son_u32 buf_len = SON_NULL;
    son_u16 dest_module_id = SON_NULL;
    son_u16 api_id = SON_PROCEDURE_UNDEFINED;

    length = sizeof(struct sockaddr);
    /*Start: OAM notification changes*/
    son_u16 event_id = SON_NULL;
    son_oam_error_info_t error_info;

    SON_MEMSET(&error_info, 0, sizeof(error_info));
    /*fill error info structure*/
    error_info.error_type = SON_OAM_SW_PROCESSING_ERROR;
    error_info.bitmask |= SON_OAM_EVENT_ERROR_CAUSE_PRESENT;
    error_info.error_cause = SON_OAM_INVALID_PARAMS;
    error_info.bitmask |= SON_OAM_EVENT_SEVERITY_PRESENT;
    error_info.severity = SON_OAM_MAJOR_ERROR;

    event_id = (son_u16)SON_OAM_RECEIVE_FAIL;
    /*End: OAM notification changes*/

    /* Read the external API Header from PHY module */
    if (sender_module_id == SON_PHY_MODULE_ID)
    {
#ifdef __SON_FAPI__
        retval = recvfrom(sd_udp,
            ext_msg_api_hdr, SON_FAPI_MSG_API_HDR_SIZE, MSG_PEEK,
            (struct sockaddr *)&cli_addr, &length);

        if (retval != SON_FAPI_MSG_API_HDR_SIZE)
        {
            SON_ENCAPS_WARNING("son_handle_message: Error in recvfrom while reading"
                               "data from PHY(FAPI interface).");
            /*send SON_OAM_EVENT_NOTIFICATION to OAM*/

            if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, SON_OAM_SOCKET_ERROR,
                    event_id, error_info))
            {
                SON_ENCAPS_WARNING("son_handle_message: SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
            }

            return p_recv_buffer;
        }

        api_id = ext_msg_api_hdr[0];
        if (retval > SON_NULL)
        {
            buf_len = SON_NTOHS(*(U16 *)(ext_msg_api_hdr + 2)) + retval;
        }
        else
        {
            SON_ENCAPS_WARNING("son_handle_message: Error in recvfrom while reading"
                               "data from PHY(FAPI interface).");
            return p_recv_buffer;
        }

#endif /* ifdef __SON_FAPI__ */


        dest_module_id = SON_NMM_MODULE_ID;
    }
    /* Read the external API Header from RRM or OAM module */
    else
    {
        retval = recvfrom(sd_udp,
            ext_msg_api_hdr, EXT_MSG_API_HDR_SIZE, MSG_PEEK,
            (struct sockaddr *)&cli_addr, &length);
        if (EXT_MSG_API_HDR_SIZE != retval)
        {
            SON_ENCAPS_WARNING("son_handle_message: Error in recvfrom while "
                    "reading data from socket with fd = %d", sd_udp);
            
            /*send SON_OAM_EVENT_NOTIFICATION to OAM*/
            if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, 
                        SON_OAM_SOCKET_ERROR,
                        event_id, error_info))
            {
                SON_ENCAPS_WARNING("son_handle_message: "
                        "SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
            }

            return p_recv_buffer;
        }

        dest_module_id = son_get_word_from_header(
            &ext_msg_api_hdr[EXT_MSG_DEST_MODULE_ID_OFFSET]);
        api_id = son_get_word_from_header(&ext_msg_api_hdr[EXT_MSG_API_OFFSET]);

        buf_len = son_get_word_from_header(
            &ext_msg_api_hdr[EXT_MSG_BUF_LEN_OFFSET]);
    }

    *p_priority = SON_API_PRIORITY;

    /* Populate the sender and receiver Ids using the external API
     * header information */
    if (SON_OAM_MODULE_ID == sender_module_id)
    {
        /* All the messages received from OAM will be send to SMIF
         *  while its external header destination Id will be SON so
         *  set dest_module_id to SON_MIF_MODULE_ID */

        /* For messages received from RRM the external header will
         * specify the destined CSPL module Id */
        dest_module_id = SON_MIF_MODULE_ID;
    }
    else if (SON_RRC_MODULE_ID == sender_module_id)
    {
        /* All the messages received in release 1.2 from X2AP will be send
         *  to X2 module while its external header destination Id will be
         *  SON so set dest_module_id to SON_X2_MODULE_ID */
        dest_module_id = SON_X2_MODULE_ID;
    }

    /* Verify destination module Id */
    /* PR 917 fix Start */
    if ((SON_MIN_INT_MODULE_ID > dest_module_id ||
                SON_X2_MODULE_ID < dest_module_id) || 
            SON_LOGGER_MODULE_ID == dest_module_id)
    /* PR 917 fix End */
    {
        son_u8 *p_extra_data_arrray = SON_PNULL;
        p_extra_data_arrray = (son_u8 *)son_mem_get(buf_len);
        if(SON_PNULL == p_extra_data_arrray)
        {
            SON_ENCAPS_WARNING("son_handle_message: Not enough memory for the "
                    "to clean to msg  \n");
        }
        else
        {    
            SON_ENCAPS_WARNING("son_handle_message: Message (API Id = %u) "
                    "received with invalid destination module Id = %u. "
                    "So message will not be processed.",
                    api_id, dest_module_id);
            /* Start :SPR 8671 */
            /* Extra recvfrom() just to clean up socket buffer */
            /* Coverity ID 80112 Fix Start */
            retval = recvfrom(sd_udp, p_extra_data_arrray, buf_len,
                    0, (struct sockaddr *)&cli_addr, &length);
            if (retval < 0)
            {
                SON_ENCAPS_WARNING("son_handle_message: Error in recvfrom while "
                        "receiving message data. Negative value is returned");
            }
            /* Coverity ID 80112 Fix End */
            son_mem_free(p_extra_data_arrray);
            /* End :SPR 8671 */
        }
        return p_recv_buffer;
    }

    /* Populate the receiver module Id */
    *p_receiver = qvGetService(dest_module_id);
    /* Populate the sender module Id */
    *p_sender = qvGetService(sender_module_id);
    p_recv_buffer  = (son_u8 *)son_msg_mem_get
                         (CV_HDRSIZE + buf_len);

    if (SON_PNULL == p_recv_buffer)
    {
        son_u8 *p_extra_data_arrray = SON_PNULL;
        p_extra_data_arrray = (son_u8 *)son_mem_get(buf_len);
        if(SON_PNULL == p_extra_data_arrray)
        {
            SON_ENCAPS_WARNING("son_handle_message: Not enough memory for the "
                    "to clean to msg  \n");
        }
        else
        {
            /* Start :SPR 8671 */
            /* Extra recvfrom() just to clean up socket buffer */
            /* Coverity ID 80112 Fix Start */
            retval = recvfrom(sd_udp,p_extra_data_arrray, buf_len,
                    0, (struct sockaddr *)&cli_addr, &length);
            if (retval < 0)
            {
                SON_ENCAPS_WARNING("son_handle_message: Error in recvfrom while "
                        "receiving message data. Negative value is returned");
            }
            /* Coverity ID 80112 Fix End */
            /* End :SPR 8671 */
            son_mem_free(p_extra_data_arrray);
        }
    }
    else
    {
        retval = recvfrom(sd_udp, p_recv_buffer + CV_HDRSIZE, buf_len,
            0, (struct sockaddr *)&cli_addr, &length);

        if (retval <= 0)
        {
            SON_ENCAPS_WARNING("son_handle_message: Error in recvfrom while "
                    "receiving message data. Negative value is returned");
            son_msg_mem_free(p_recv_buffer);
            /*Coverity ID 83380 Starts */
            p_recv_buffer = SON_PNULL;
            /*Coverity ID 83380 Ends */
            /*send SON_OAM_EVENT_NOTIFICATION to OAM*/

            if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, 
                        SON_OAM_SOCKET_ERROR, event_id, error_info))
            {
                SON_ENCAPS_WARNING("son_handle_message: "
                        "SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
            }
            /*Coverity 83380 Fix Start*/
            return SON_PNULL;
            /*Coverity 83380 Fix End*/
        }
        else if ((son_s64)buf_len != retval)
        {
            SON_ENCAPS_WARNING("son_handle_message: Error in recvfrom while "
                    "receiving message ata");
            son_msg_mem_free(p_recv_buffer);
            /*Coverity ID 83380 Starts */
            p_recv_buffer = SON_PNULL;
            /*Coverity ID 83380 Ends */
            /*send SON_OAM_EVENT_NOTIFICATION to OAM*/

            if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, 
                        SON_OAM_SOCKET_ERROR, event_id, error_info))
            {
                SON_ENCAPS_WARNING("son_handle_message: "
                        "SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
            }
            /*Coverity 83380 Fix Start*/
            return SON_PNULL;
            /*Coverity 83380 Fix End*/
        }
        else
        {
            /*Appending CSPL header at top 16 bytes of this buffer*/
            SON_MEMSET(p_recv_buffer, SON_NULL, CV_HDRSIZE);
            son_construct_cspl_header((son_u8 *)p_recv_buffer,
                SON_VERSION_ID, sender_module_id,
                dest_module_id, api_id, SON_API_PRIORITY,
                retval);
        }
    }

    if((sender_module_id == SON_OAM_MODULE_ID) ||
            (sender_module_id == SON_RRM_MODULE_ID) )
    {
        SON_LOG_HEX_DUMP("RECV", p_recv_buffer, (CV_HDRSIZE + buf_len),
                sender_module_id, api_id, SON_VERSION_ID);
    }

    return p_recv_buffer;
} /* son_handle_message */

/*****************************************************************************
 * Function Name  : son_receive
 * Inputs         : p_info_listener - Pointer to the listener object
 * Outputs        : p_sender - Module which sent the message
 *                  p_receiver - Module which should receive the message
 *                  p_priority - Priority of the message
 * Returns        : son_void_t* - pointer to message buffer returned to CSPL
 * Description    : It is used to receive messages over sockets by CSPL
 ****************************************************************************/
static son_void_t *
son_receive
(
    son_void_t  *p_info_listener,
    QMODULE     *p_sender,
    QMODULE     *p_receiver,
    son_s8      *p_priority
)
{
    son_listener_t *p_son_listener = SON_PNULL;
    son_u8 *p_recv_buffer = SON_PNULL;

    SON_ASSERT(SON_PNULL != p_info_listener);

    p_son_listener = (son_listener_t *)p_info_listener;

    if (FD_ISSET(p_son_listener->sd_udp_rrm, &son_fd_set))
    {
        p_recv_buffer = son_handle_message(p_son_listener->sd_udp_rrm,
            SON_RRM_MODULE_ID, p_sender, p_receiver, p_priority);
    }
    else if (FD_ISSET(p_son_listener->sd_udp_rrc, &son_fd_set))
    {
        p_recv_buffer = son_handle_message(p_son_listener->sd_udp_rrc,
            SON_RRC_MODULE_ID, p_sender, p_receiver, p_priority);
    }
    else if (FD_ISSET(p_son_listener->sd_udp_phy, &son_fd_set))
    {
        p_recv_buffer = son_handle_message(p_son_listener->sd_udp_phy,
            SON_PHY_MODULE_ID, p_sender, p_receiver, p_priority);
    }
    else if (FD_ISSET(p_son_listener->sd_udp_oam, &son_fd_set))
    {
        p_recv_buffer = son_handle_message(p_son_listener->sd_udp_oam,
            SON_OAM_MODULE_ID, p_sender, p_receiver, p_priority);
    }
/*+ SPR 18399*/
/*- SPR 18399*/

    return p_recv_buffer;
} /* son_receive */

/*****************************************************************************
 * Function Name  : son_make_udp
 * Inputs         : p_self - Pointer to the address structure to be binded
 *                  to the UDP socket
 * Outputs        : None
 * Returns        : UDP socket descriptor
 * Description    : Creates a UDP socket, assigns an address to udp socket
 *                  and makes it ready for communication.
 ****************************************************************************/
static son_s32
son_make_udp
(
    const struct sockaddr_in *p_self
)
{
    son_s32 sd      = -1;

    if (SON_PNULL == p_self)
    {
        SON_PANIC("son_make_udp: NULL address passed to son_make_udp "
                  "function");
    }
    else
    {
        /* Create UDP socket */
        if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        {
            SON_PANIC("son_make_udp: Unable to create UDP socket");
        }
        else
        {     
            /* Binding address structures with a socket an return
             *  it if successful */
            if (bind(sd, (struct sockaddr *)p_self,
                        sizeof(struct sockaddr)) < 0)
            {
                if (!close(sd))
                {
                    SON_ENCAPS_WARNING("son_make_udp: Error while closing UDP"
                            " socket");
                }

                SON_PANIC("son_make_udp: Unable to bind passed address to socket "
                        "descriptor . Port No = %u, IP Address = %s",
                        htons(p_self->sin_port), inet_ntoa(p_self->sin_addr));
            }
        }
    }

    return sd;
} /* son_make_udp */

/*****************************************************************************
 * Function Name  : son_find_max_sd
 * Inputs         : sd1, sd2, sd3, sd4 - socket descriptors
 * Outputs        : None
 * Returns        : Maximum value of the socket fd
 * Description    : It finds the maximum socket value present in the listerner
 *                  object
 ****************************************************************************/
static son_s32
son_find_max_sd
(
    son_s32 sd1,
    son_s32 sd2,
    son_s32 sd3,
    son_s32 sd4
)
{
    son_s32 max_sd = sd1;

    if (sd2 > max_sd)
    {
        max_sd = sd2;
    }

    if (sd3 > max_sd)
    {
        max_sd = sd3;
    }

    if (sd4 > max_sd)
    {
        max_sd = sd4;
    }

    return max_sd;
} /* son_find_max_sd */

/*****************************************************************************
 * Function Name  : son_open
 * Inputs         : p_data - Pointer to data provided by CSPL
 * Outputs        : None
 * Returns        : son_void_t* - pointer to listener data structure
 * Description    : Opens the listener socket
 ****************************************************************************/
static son_void_t *
son_open
(
    son_void_t *p_data
)
{
    son_listener_t *p_son_listener = (son_listener_t *)son_sys_mem_get(
        sizeof(son_listener_t));

    if (SON_PNULL == p_son_listener)
    {
        /*SPR 17777 +-*/
        SON_PANIC("son_open: Unable to get memory for listener structure"
                "Unused variable p_data=%p",p_data);
        /*SPR 17777 +-*/
    }
    else
    {
        struct sockaddr_in addr_for_udp_rrm = {0};
        struct sockaddr_in addr_for_udp_rrc = {0};
        struct sockaddr_in addr_for_udp_phy = {0};
        struct sockaddr_in addr_for_udp_oam = {0};

        /* Get Socket IP address and port number for OAM, RRM
         *  and PHY */
        if ((SON_FALSE == son_sockaddr(son_oam_addr, &addr_for_udp_oam)) ||
            (SON_FALSE == son_sockaddr(son_rrm_addr, &addr_for_udp_rrm)) ||
            (SON_FALSE == son_sockaddr(son_rrc_addr, &addr_for_udp_rrc)) ||
            (SON_FALSE == son_sockaddr(son_phy_addr, &addr_for_udp_phy)))
        {
            SON_PANIC("son_open: Unable to get socket address");
        }
        else
        {
            /* Opening a UDP Socket for communication with OAM, RRM and PHY */
            p_son_listener->sd_udp_rrm = son_make_udp(&addr_for_udp_rrm);
            p_son_listener->sd_udp_rrc = son_make_udp(&addr_for_udp_rrc);
            p_son_listener->sd_udp_phy = son_make_udp(&addr_for_udp_phy);
            p_son_listener->sd_udp_oam = son_make_udp(&addr_for_udp_oam);

            max_sd_value = son_find_max_sd(p_son_listener->sd_udp_rrm,
                p_son_listener->sd_udp_rrc, p_son_listener->sd_udp_oam,
                p_son_listener->sd_udp_phy);
        }
    }
/*+ SPR 18399*/
/*- SPR 18399*/

    return p_son_listener;
} /* son_open */

/*****************************************************************************
 * Function Name  : son_close
 * Inputs         : p_info_listener - Pointer to the listener object
 * Outputs        : None
 * Returns        : None
 * Description    : Close the listener socket
 ****************************************************************************/
static son_void_t
son_close
(
    son_void_t *p_info_listener
)
{
    SON_ASSERT(SON_PNULL != p_info_listener);

    son_listener_t *p_son_listener = (son_listener_t *)p_info_listener;

    if (close(p_son_listener->sd_udp_rrm))
    {
        SON_ENCAPS_WARNING("son_close: Error while closing UDP socket for RRM");
    }

    if (close(p_son_listener->sd_udp_rrc))
    {
        SON_ENCAPS_WARNING("son_close: Error while closing UDP socket for RRC");
    }

    if (close(p_son_listener->sd_udp_phy))
    {
        SON_ENCAPS_WARNING("son_close: Error while closing UDP socket for PHY");
    }

    if (close(p_son_listener->sd_udp_oam))
    {
        SON_ENCAPS_WARNING("son_close: Error while closing UDP socket for OAM");
    }

    son_sys_mem_free(p_info_listener);
} /* son_close */

#ifdef SON_IN_MEM_LOGGER

/****************************************************************************
 * Function Name  : son_newkey
 * Inputs         : None
 * Outputs        : None
 * Returns        : Newly created key for the thread
 * Description    : Create a new key for a thread
 ****************************************************************************/

static void *son_newkey(void)
{
	pthread_key_t	*key = (pthread_key_t *)son_malloc( sizeof(pthread_key_t) );

    if(SON_PNULL != key)
    {
        pthread_key_create( key, 0 );
    }
	return (void *)key;
}

/****************************************************************************
 * Function Name  : son_getkey
 * Inputs         : Key object
 * Outputs        : None
 * Returns        : Context stored in key
 * Description    : Return context stored for a calling thread
 ****************************************************************************/

static	void	*son_getkey( void *key )
{
	return	pthread_getspecific( *((pthread_key_t *)key) );
}

/****************************************************************************
 * Function Name  : son_setkey
 * Inputs         : Key, Value
 * Outputs        : None
 * Returns        : None
 * Description    : Function for setting the value corresponding to the key
 ****************************************************************************/

static	void	son_setkey( void *key, void *value )
{
	pthread_setspecific( *((pthread_key_t *)key), value );
}

/****************************************************************************
 * Function Name  : son_newlock
 * Inputs         : None
 * Outputs        : None
 * Returns        : Lock object
 * Description    : Returns new lock object
 ****************************************************************************/

static	void	*son_newlock( void )
{
	pthread_mutex_t	*lock = (pthread_mutex_t *)son_malloc( sizeof(pthread_mutex_t) );

    if(SON_PNULL != lock)
    {
        pthread_mutex_init( lock, 0 );
    }
	return	(void *)lock;
}

/****************************************************************************
 * Function Name  : son_lock
 * Inputs         : Lock object
 * Outputs        : None
 * Returns        : None
 * Description    : Function for taking the lock
 ****************************************************************************/

static	void	son_lock( void *lock )
{
	pthread_mutex_lock( (pthread_mutex_t *)lock );
}

/****************************************************************************
 * Function Name  : son_unlock
 * Inputs         : Lock
 * Outputs        : None
 * Returns        : None
 * Description    : Function to unlock
 ****************************************************************************/

static	void	son_unlock( void *lock )
{
	pthread_mutex_unlock( (pthread_mutex_t *)lock );
}

/****************************************************************************
 * Function Name  : son_destroylock
 * Inputs         : Lock object
 * Outputs        : None
 * Returns        : None
 * Description    : Destroy the locking object created for mutex
 ****************************************************************************/

static	void	son_destroylock( void *lock )
{
	pthread_mutex_destroy( (pthread_mutex_t *)lock );
	SON_FREE( lock );
}

typedef	struct __waitdata {
	pthread_cond_t	condition;
	pthread_mutex_t	lock;
	int		value;
} __waitdata;

/****************************************************************************
 * Function Name  : son_newchannel
 * Inputs         : None
 * Outputs        : None
 * Returns        : wait data structure
 * Description    : Function to initialize mutex for communication between
 *                  threads
 ****************************************************************************/

static	void	*son_newchannel( void )
{
	__waitdata	*w = (__waitdata *)son_malloc( sizeof(__waitdata) );

    if(SON_PNULL != w)
    {
        pthread_cond_init(&(w->condition), 0);
        pthread_mutex_init( &(w->lock), 0 );
        w->value = 0;
    }
	return (void *)w;
}

/****************************************************************************
 * Function Name  : son_sleep
 * Inputs         : channel, timeout
 * Outputs        : None
 * Returns        : None
 * Description    : Function to sleep a thread when waiting for message
 ****************************************************************************/

static	void	son_sleep( void *channel, const QTIME *timeout )
{
	__waitdata	*w = (__waitdata *)channel;
	struct timespec	T;

    pthread_mutex_lock( &(w->lock) );
    if( !w->value ) {
        if( !timeout ) {
            /*coverity_115241 Start*/
            if(pthread_cond_wait( &(w->condition), &(w->lock) ))
                SON_ENCAPS_WARNING("pthread_cond_wait returned failure");
            /*coverity_115241 Start*/
        } else if ( timeout->s || timeout->us ) {
			struct timeval	_T;

			son_gettimeofday( &_T, 0 );

			_T.tv_sec += timeout->s;
			_T.tv_usec += timeout->us;
			while( _T.tv_usec >= 1000000L ) {
				_T.tv_usec -= 1000000L;
				_T.tv_sec++;
			}

			T.tv_sec = _T.tv_sec;
			T.tv_nsec = 1000 * _T.tv_usec;
            /* SPR 21978: CID 115455 Start */
			if(pthread_cond_timedwait( &(w->condition), &(w->lock), &T ))
            {
                 SON_ENCAPS_WARNING("pthread_cond_timedwait returned failure");
            }
            /* SPR 21978: CID 115455 End */
		} else {
			/** Nothing -- zero timeout was specified **/
		}
	}
	w->value = 0;
	pthread_mutex_unlock( &(w->lock) );
}

/****************************************************************************
 * Function Name  : son_wakeup
 * Inputs         : channel
 * Outputs        : None
 * Returns        : None
 * Description    : Function to wakeup the sleeping thread
 ****************************************************************************/

static	void	son_wakeup( void *channel )
{
	__waitdata	*w = (__waitdata *)channel;

	pthread_mutex_lock( &(w->lock) );
	w->value = 1;
	pthread_cond_signal( &(w->condition) );
	pthread_mutex_unlock( &(w->lock) );
}

const QSYSOP son_os = {
    son_malloc,
    son_free,
    son_newkey,
    son_getkey,
    son_setkey,
    son_newlock,
    son_destroylock,
    son_lock,
    son_unlock,
    son_vprintf,
    son_vprintf,
    son_abort
};

const QWAIT son_waitstruct = {
	son_newchannel,
	son_sleep,
	son_wakeup,
	son_walltime
};

#else
const QSYSOP son_os =
{
    son_malloc,
    son_free,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    son_vprintf,
    son_vprintf,
    son_abort
};

const QWAIT son_waitstruct =
{
    0,
    0,
    0,
    son_walltime
};

#endif

const QSHELL son_shell = {
    son_timedwait,
    son_send,
    son_receive,
    son_open,
    son_close,
    0
};
