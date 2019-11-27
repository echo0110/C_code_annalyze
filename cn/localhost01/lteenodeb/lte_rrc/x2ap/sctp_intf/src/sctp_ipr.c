/*******************************************************************************
*
*  FILE NAME   : sctp_ipr.c
*
*  DESCRIPTION : This file contains the definition of functions which will be
*                used to handle the sages from/to the Aricent SCTP stack
*                interface.
*
*  DATE 24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#if 0
#ifdef  ARICENT_SCTP_IPR

#include "x2ap_utils.h"
#include "sctp_ipr.h"
#include "x2ap_api.h"
#include "rrc_msg_mgmt.h"

/* GDB fix - duplicate symbols at link time */
sctp_fd_set_st g_sctp_fd_set;


/* GDB Fix - compiler warnings */
extern sctp_return_t sctp_db_validate_config_stack_address( sctp_bindx_flag_et        operation,
                                                            sctp_U32bit               naddr,
                                                            sctp_sockaddr_st          *p_addrlist,
                                                            sctp_error_t              *p_ecode);


/****************************************************************************
 * Private Variables
 ****************************************************************************/

/* Maintained for sctp sockets*/

S32 g_sctp_select_sd;

S32        g_sctp_sd_list[MAX_PEER_ENB] = {};

U32 x2ap_sctp_init_stack(
            U8 cspl_already_intialised,
            x2_enb_comm_info_t          *p_comm_info);

static S32 x2ap_sctp_connectx(
        S32       connSock,
        x2_enb_comm_info_t *p_comm_info);

#ifdef LINUX_PC_TEST
#if 0
extern S32 g_x2ap_sctp_client_fds[MAX_PEER_ENB];
extern U16 g_x2ap_sctp_client_fd_curr_count;

S32 test_sctp_setsockopt(void)
{
    return 0;
}

S32 test_x2ap_open_sctp_client(x2ap_peer_enb_context_t *p_enb_cntxt_arr)
{
    S32  connSock      = INVALID_SCTP_SOCKET;
    U16 sd_index;
    U16 counter;
    
    /* Get an SCTP socket */
    if (RRC_NULL != g_x2ap_sctp_client_fd_curr_count)
    {
        connSock = g_x2ap_sctp_client_fds[0];
        for (sd_index = 1; sd_index < g_x2ap_sctp_client_fd_curr_count; sd_index++)
            g_x2ap_sctp_client_fds[sd_index-1] = g_x2ap_sctp_client_fds[sd_index];
        
        g_x2ap_sctp_client_fds[g_x2ap_sctp_client_fd_curr_count-1] = INVALID_SCTP_SOCKET;
        g_x2ap_sctp_client_fd_curr_count--;

        if ( PNULL != p_enb_cntxt_arr )
        {
            p_enb_cntxt_arr->stream_info.number_of_streams = 1;

            for (counter = 0; counter < p_enb_cntxt_arr->stream_info.number_of_streams; counter++)
            {
                p_enb_cntxt_arr->stream_info.stream_ids[counter] = counter;
            }
        }

        //SCTP_FD_SET(connSock, &(g_sctp_fd_set) );
        //g_sctp_sd_list[g_sctp_sd_count] = connSock;
        //g_sctp_sd_count++;

        //SCTP_FD_SET(connSock, &(sctp_fd) );
    }

    RRC_TRACE(RRC_INFO,"test_x2ap_open_sctp_client:connSock=%d\n", connSock);
    
    return connSock;
}
#endif
#endif

/******************************************************************************/
/*******************************************************************************
 * Function Name : open_sctp_server_init_sctp_stack
 * Input         : p_comm_info
 * Output        : 
 * Returns       : U8 ( -1 if failure, 1 if success)
 * Description   : This Function is used to init_sctp_stack in open_sctp_server
 *******************************************************************************/
static S8 open_sctp_server_init_sctp_stack(
        /*SPR 20633 Start*/
        x2_enb_comm_info_t *p_comm_info,U8 addr_family)
        /*SPR 20633 End*/
{
    U16   peer_enb_count = X2AP_NULL;
    U8 cspl_already_initialised = 1;

    /* L3-X2AP Integration Activity - Start */
    x2_ip_addr_t             *addr;
    U16                    addr_count = X2AP_NULL;
    x2_ipv6_addr_t           *p_ipv6_addr = PNULL;
    sctp_sockaddr_st       addr_list[MAX_NUM_IP_ADDR];
    sctp_error_t           ecode;
    /* L3-X2AP Integration Activity - End */
    X2AP_UT_TRACE_ENTER();
    /* SPR 20633 Fix Start */
    if((p_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT) && (p_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_NUM_ADDR_PRESENT))
    {

        RRC_TRACE(RRC_INFO," Initializing g_sctp_sd_list..\n");
        if (X2AP_FALSE == init_stack_flag_v6)
        {
            sctp_stack_state = SCTP_STATE_UNINTIALISED; 
            /*Initiating g_sctp_sd_list*/
            for (peer_enb_count = 0; peer_enb_count < MAX_PEER_ENB ; peer_enb_count++ )
            {
                g_sctp_sd_list[peer_enb_count] = INVALID_SCTP_SOCKET; 
            }
            if (SCTP_FAILURE == x2ap_sctp_init_stack(cspl_already_initialised,
                        p_comm_info))
            {
                X2AP_TRACE(X2AP_WARNING,"INIT failure for IPv6, error no: %d\n", sk_errno);
                return X2AP_SCTP_ERROR;
            }
            else
            {
                X2AP_TRACE(X2AP_INFO," SCTP INIT Success IPv6\n");
            }
        }
        /* L3-X2AP Integration Activity */
        else
        {
            X2AP_MEMSET(addr_list, 0, sizeof(addr_list));
            for(addr_count=0; addr_count < p_comm_info->num_ipv6_addr; addr_count++)
            {
                p_ipv6_addr = &(p_comm_info->ipv6_addr[addr_count]);
                addr_list[addr_count].family = 10;
                if (inet_pton_wrapper(10,(const char *)p_ipv6_addr[0].ipv6_addr,
                            (void*)&addr_list[addr_count].ip.v6.addr) != 1)
                {
                    RRC_TRACE(RRC_WARNING,"l3_open_sctp_association: Couldn't convert INET6 address");
                    return X2AP_SCTP_ERROR;
                }

                addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[0] = ntohl_wrapper(addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[0]);
                addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[1] = ntohl_wrapper(addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[1]);
                addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[2] = ntohl_wrapper(addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[2]);
                addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[3] = ntohl_wrapper(addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[3]);
            }

            if(sctp_db_validate_config_stack_address(SCTP_BINDX_ADD_ADDR, (U32)p_comm_info->num_ipv6_addr, addr_list, &ecode))
            {
                X2AP_TRACE(X2AP_INFO,"Calling sctp_api_config_stack_addresses()\n");
                if(SCTP_FAILURE == sctp_api_config_stack_addresses(SCTP_BINDX_ADD_ADDR,
                            (U32)p_comm_info->num_ipv6_addr,
                            addr_list,
                            &ecode))
                {
                    X2AP_TRACE(X2AP_WARNING,
                            "SCTP configuration parameter failure, error no: %d\n", 
                            sk_errno);
                    return X2AP_SCTP_ERROR;
                }
                else
                {
                    X2AP_TRACE(X2AP_INFO,"sctp_api_config_stack_addresses() SUCCESS IPV6\n");
                }
            }
            else
            {
                X2AP_TRACE(X2AP_INFO,"sctp_api_config_stack_addresses() FAILURE IPV6\n");
            }
        }
        if (X2AP_FALSE == init_stack_flag_v4)
        {
            sctp_stack_state = SCTP_STATE_UNINTIALISED; 
            /*Initiating g_sctp_sd_list*/
            for (peer_enb_count = g_sctp_sd_count; peer_enb_count < MAX_PEER_ENB ; peer_enb_count++ )
            {
                g_sctp_sd_list[peer_enb_count] = INVALID_SCTP_SOCKET; 
            }
            if (SCTP_FAILURE == x2ap_sctp_init_stack(cspl_already_initialised,
                        p_comm_info))
            {
                X2AP_TRACE(X2AP_WARNING,"INIT failure for IPv4, error no: %d\n", sk_errno);
                return X2AP_SCTP_ERROR;
            }
            else
            {
                X2AP_TRACE(X2AP_INFO," SCTP INIT Success for IPv4\n");
            }
        }
        /* L3-X2AP Integration Activity */
        else
        {
            X2AP_MEMSET(addr_list, 0, sizeof(addr_list));
            for(addr_count=0; addr_count < p_comm_info->num_ip_addr; addr_count++)
            {
                addr = &(p_comm_info->ip_addr[addr_count]);
                addr_list[addr_count].family = SCTP_AF_INET;
                addr_list[addr_count].ip.v4.addr = htonl_wrapper(inet_addr_wrapper((const char*)(addr[0].ip_addr)));
            }

            if(sctp_db_validate_config_stack_address(SCTP_BINDX_ADD_ADDR, (U32)p_comm_info->num_ip_addr, addr_list, &ecode))
            {
                X2AP_TRACE(X2AP_INFO,"Calling sctp_api_config_stack_addresses()\n");
                if(SCTP_FAILURE == sctp_api_config_stack_addresses(SCTP_BINDX_ADD_ADDR,
                            (U32)p_comm_info->num_ip_addr,
                            addr_list,
                            &ecode))
                {
                    X2AP_TRACE(X2AP_WARNING,
                            "SCTP configuration parameter failure, error no: %d\n", 
                            sk_errno);
                    return X2AP_SCTP_ERROR;
                }
                else
                {
                    X2AP_TRACE(X2AP_INFO,"sctp_api_config_stack_addresses() SUCCESS\n");
                }
            }
            else
            {
                X2AP_TRACE(X2AP_INFO,"sctp_api_config_stack_addresses() FAILURE IPV4\n");
            }
        }

    }
    else
    {
        /* SPR 20633 Fix End */
    if(SCTP_AF_INET6 == addr_family)
    {

        RRC_TRACE(RRC_INFO," Initializing g_sctp_sd_list..\n");
        if (X2AP_FALSE == init_stack_flag_v6)
        {
           sctp_stack_state = SCTP_STATE_UNINTIALISED; 
        /*Initiating g_sctp_sd_list*/
        for (peer_enb_count = 0; peer_enb_count < MAX_PEER_ENB ; peer_enb_count++ )
        {
            g_sctp_sd_list[peer_enb_count] = INVALID_SCTP_SOCKET; 
        }
        if (SCTP_FAILURE == x2ap_sctp_init_stack(cspl_already_initialised,
                    p_comm_info))
        {
            /* SPR 20633 fix start */
            X2AP_TRACE(X2AP_WARNING,"INIT failure for IPv6, error no: %d\n", sk_errno);
            /* SPR 20633 fix end */
            return X2AP_SCTP_ERROR;
        }
        else
        {
                /* SPR 20633 fix start */
            X2AP_TRACE(X2AP_INFO," SCTP INIT Success IPv6\n");
                /* SPR 20633 fix end */
        }
    }
    /* L3-X2AP Integration Activity */
    else
    {
            X2AP_MEMSET(addr_list, 0, sizeof(addr_list));
            for(addr_count=0; addr_count < p_comm_info->num_ipv6_addr; addr_count++)
            {
                p_ipv6_addr = &(p_comm_info->ipv6_addr[addr_count]);
                addr_list[addr_count].family = 10;
                if (inet_pton_wrapper(10,(const char *)p_ipv6_addr[0].ipv6_addr,
                        (void*)&addr_list[addr_count].ip.v6.addr) != 1)
                {
                    RRC_TRACE(RRC_WARNING,"l3_open_sctp_association: Couldn't convert INET6 address");
                    return X2AP_SCTP_ERROR;
                }

                addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[0] = ntohl_wrapper(addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[0]);
                addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[1] = ntohl_wrapper(addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[1]);
                addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[2] = ntohl_wrapper(addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[2]);
                addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[3] = ntohl_wrapper(addr_list[addr_count].ip.v6.addr.in6_u.u6_addr32[3]);
            }

            if(sctp_db_validate_config_stack_address(SCTP_BINDX_ADD_ADDR, (U32)p_comm_info->num_ipv6_addr, addr_list, &ecode))
            {
                X2AP_TRACE(X2AP_INFO,"Calling sctp_api_config_stack_addresses()\n");
                if(SCTP_FAILURE == sctp_api_config_stack_addresses(SCTP_BINDX_ADD_ADDR,
                            (U32)p_comm_info->num_ipv6_addr,
                            addr_list,
                            &ecode))
                {
                    X2AP_TRACE(X2AP_WARNING,
                            "SCTP configuration parameter failure, error no: %d\n", 
                            sk_errno);
                    return X2AP_SCTP_ERROR;
                }
                else
                {
                    X2AP_TRACE(X2AP_INFO,"sctp_api_config_stack_addresses() SUCCESS IPV6\n");
                }
            }
                /* SPR 20633 Fix Start */
                else
                {
                    X2AP_TRACE(X2AP_INFO,"sctp_api_config_stack_addresses() FAILURE IPV6\n");
                }
                /* SPR 20633 Fix End */
        }
    }
    else
    {
        if (X2AP_FALSE == init_stack_flag_v4)
        {
           sctp_stack_state = SCTP_STATE_UNINTIALISED; 
            /*Initiating g_sctp_sd_list*/
            for (peer_enb_count = g_sctp_sd_count; peer_enb_count < MAX_PEER_ENB ; peer_enb_count++ )
            {
                g_sctp_sd_list[peer_enb_count] = INVALID_SCTP_SOCKET; 
            }
            if (SCTP_FAILURE == x2ap_sctp_init_stack(cspl_already_initialised,
                        p_comm_info))
            {
                /* SPR 20633 fix start */
                X2AP_TRACE(X2AP_WARNING,"INIT failure for IPv4, error no: %d\n", sk_errno);
                /* SPR 20633 fix end */
                return X2AP_SCTP_ERROR;
            }
            else
            {
                /* SPR 20633 fix start */
                X2AP_TRACE(X2AP_INFO," SCTP INIT Success for IPv4\n");
                /* SPR 20633 fix end */
            }
        }
        /* L3-X2AP Integration Activity */
        else
        {
            X2AP_MEMSET(addr_list, 0, sizeof(addr_list));
            for(addr_count=0; addr_count < p_comm_info->num_ip_addr; addr_count++)
            {
                addr = &(p_comm_info->ip_addr[addr_count]);
                addr_list[addr_count].family = SCTP_AF_INET;
                addr_list[addr_count].ip.v4.addr = htonl_wrapper(inet_addr_wrapper((const char*)(addr[0].ip_addr)));
            }

            if(sctp_db_validate_config_stack_address(SCTP_BINDX_ADD_ADDR, (U32)p_comm_info->num_ip_addr, addr_list, &ecode))
            {
                X2AP_TRACE(X2AP_INFO,"Calling sctp_api_config_stack_addresses()\n");
                if(SCTP_FAILURE == sctp_api_config_stack_addresses(SCTP_BINDX_ADD_ADDR,
                            (U32)p_comm_info->num_ip_addr,
                            addr_list,
                            &ecode))
                {
                    X2AP_TRACE(X2AP_WARNING,
                            "SCTP configuration parameter failure, error no: %d\n", 
                            sk_errno);
                    return X2AP_SCTP_ERROR;
                }
                else
                {
                    X2AP_TRACE(X2AP_INFO,"sctp_api_config_stack_addresses() SUCCESS\n");
                }
            }
                /* SPR 20633 Fix Start */
                else
                {
                    X2AP_TRACE(X2AP_INFO,"sctp_api_config_stack_addresses() FAILURE IPV4\n");
                }
            }
            /* SPR 20633 Fix Start */
        }
    }
    X2AP_UT_TRACE_EXIT();
    return 1;
}

/*****************************************************************************
 * Function Name  : x2ap_open_sctp_server_sctp_setsockopt
 * Inputs         : p_connect_server_info - pointer to x2ap_sctp_conn_info_t
 *                  p_client_info - pointer to x2ap_sctp_conn_info_t
 * Outputs        : None
 * Returns        : signed int 
 * Description    : this function opens a connection for sctp server
 ********************************************************************************/
S32 x2ap_open_sctp_server_sctp_setsockopt(
	S32 server_sctp_sd, 
	x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info)
{
    U32 blocked = 1;
    if (sctp_setsockopt(server_sctp_sd, SOL_SOCKET, 
                SCTP_SO_NONBLOCKING, &blocked, sizeof(U32)) != -1) 
    {

        if (sctp_setsockopt(server_sctp_sd, SOL_SOCKET,
                    SCTP_SO_INITMSG, 
                    &(p_sctp_assoc_conf_info->init_msg), 
                    sizeof(p_sctp_assoc_conf_info->init_msg) ) == -1) {
            /*unable to set socket options thus deleting created socket*/
            X2AP_TRACE(X2AP_INFO," Server socket property SCTP_SO_INITMSG not set" 
                    "[errno:%d]",sk_errno);
            if( sctp_close(server_sctp_sd) == 0 ) 
            {
                /* association closed successfully */ 
                /* Trace the successful closure of socket */
                X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            }
            else 
            {
                /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
                X2AP_TRACE(X2AP_WARNING, 
                        " Server socket close failure, errno: %d",sk_errno);
            }
            return INVALID_SCTP_SOCKET;
        }
	

        /*sctp parameters start*/
        if (sctp_setsockopt(server_sctp_sd, SOL_SOCKET,
                    SCTP_SO_ASSOCINFO, 
                    &(p_sctp_assoc_conf_info->assocparams), 
                    sizeof(p_sctp_assoc_conf_info->assocparams) ) == -1) {
            /*unable to set socket options thus deleting created socket*/
            X2AP_TRACE(X2AP_INFO," Server socket property SCTP_SO_ASSOCINFO not set" 
                    "[errno:%d]",sk_errno);
            if( sctp_close(server_sctp_sd) == 0 ) 
            {
                /* association closed successfully */ 
                /* Trace the successful closure of socket */
                X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            }
            else 
            {
                /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
                X2AP_TRACE(X2AP_WARNING, 
                        " Server socket close failure, errno: %d",sk_errno);
            }
            return INVALID_SCTP_SOCKET;
        }

        if (sctp_setsockopt(server_sctp_sd, IPPROTO_SCTP,
                    SCTP_SO_RTOINFO, 
                    &(p_sctp_assoc_conf_info->rtoinfo), 
                    sizeof(p_sctp_assoc_conf_info->rtoinfo) ) == -1) {
            /*unable to set socket options thus deleting created socket*/
            X2AP_TRACE(X2AP_INFO," Server socket property SCTP_SO_RTOINFO not set" 
                    "[errno:%d]",sk_errno);
            if( sctp_close(server_sctp_sd) == 0 ) 
            {
                /* association closed successfully */ 
                /* Trace the successful closure of socket */
                X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            }
            else 
            {
                /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
                X2AP_TRACE(X2AP_WARNING, 
                        " Server socket close failure, errno: %d",sk_errno);
            }
            return INVALID_SCTP_SOCKET;
        }
        /*sctp parameters stop*/
    }
    else
    {
        X2AP_TRACE(X2AP_INFO," Server socket property SCTP_SO_NONBLOCKING not set" 
                " [errno:%d]",sk_errno);
        /*unable to set socket options thus deleting created socket*/
        if( sctp_close(server_sctp_sd) == 0 ) 
        {
            /* association closed successfully */ 
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING,
                    " Server socket close failure, errno: %d",sk_errno);
        }
        return INVALID_SCTP_SOCKET;
    }
    return server_sctp_sd;
}
/* SPR 20633 changes start */
/*****************************************************************************
 * Function Name  : x2ap_open_sctp_server_ipv4  
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_server_info - pointer to x2ap_sctp_conn_info_t
 *                  num_peer_enb - num of peer eNBs
 * Outputs        : None
 * Returns        : signed int 
 * Description    : This function opens a sctp connection for server
 ********************************************************************************/
S32 x2ap_open_sctp_server_ipv4(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_sctp_conn_info_t *p_server_info,
        U32 num_peer_enb)
{
    U32    numIpAddress = 1; /* Value is kept 1 as for the first Ipaddress bind
                                 is done and for every other successive 
                                 IpAddress bindx is performend*/

    U32   optVal = 1;
    x2_enb_comm_info_t *p_comm_info = p_server_info->p_comm_info;
    S32 server_sctp_sd = INVALID_SCTP_SOCKET;
    struct sctp_event_subscribe events;
    U32 addr_family;         /* This variable is taken for facilitation of IPv6
                                enhancement */
    /*dscp_marking_start*/
#ifdef SCTP_IP_HEADER_PARAMS
    sctp_assoc_ip_hdr_params_st header_params;
#endif
    /*dscp_marking_stop*/
    U32 addr_len = RRC_NULL;
    U8  no_of_ip_addr = RRC_NULL;
    struct sockaddr_in selfENBServerAddr; 
    /* Bug 908 Fix Start */
    U32 noDelay = 1;
    /* Bug 908 Fix Stop */
    /* SPR_14436_Fix Start */
    U32 nostreams = X2AP_NULL;
    U32 nistreams = X2AP_NULL;

    nostreams = p_x2ap_gb_ctx->config_param.sctp_conf_param.init_num_ostreams;
    nistreams = p_x2ap_gb_ctx->config_param.sctp_conf_param.init_max_instreams;

    sctp_initmsg_st initmsg;
    S32 return_value  = RRC_NULL;
    /* SPR_14436_Fix Stop */

    X2AP_UT_TRACE_ENTER();

    addr_family = SCTP_AF_INET; 
    no_of_ip_addr = p_comm_info->num_ip_addr;
    /* This denotes the sock addr 
       structure usd as param in bind call */

    x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info = 
        p_server_info->p_sctp_assoc_conf_info;
    /*Cov 88188 fix start*/
    if(!p_sctp_assoc_conf_info)
    {
        return X2AP_SCTP_ERROR;
    }
    /*Cov 88188 fix end*/

    /*check that atleast one ip address needs to be configured by operator*/
    if( no_of_ip_addr <= 0 ) {
        return INVALID_SCTP_SOCKET;
    }


    /* Initializing the SCTP stack */
    if( 1 != open_sctp_server_init_sctp_stack(p_comm_info,addr_family))
    {
        return X2AP_SCTP_ERROR;
    }

        selfENBServerAddr.sin_family = SCTP_AF_INET;
        selfENBServerAddr.sin_addr.s_addr = 
            ( inet_addr_wrapper((const char *)(p_comm_info->ip_addr[0].ip_addr)) );
        selfENBServerAddr.sin_port = htons_wrapper(p_comm_info->port);

        X2AP_TRACE(X2AP_DETAILED,"[%s]::X2AP Server IP:%s and port:%d \n",__FUNCTION__,
                (p_comm_info->ip_addr[0].ip_addr),p_comm_info->port);
        addr_len = sizeof(struct sockaddr_in);


    /*Create a server socket for SCTP to run on*/
    if( (server_sctp_sd = sctp_socket(addr_family, SCTP_SOCK_STREAM, 
                    IPPROTO_SCTP)) == -1 ) {
        /* Error while creating socket. Free the memory allocated and 
           exit the code and return. Using multiple return in this function
           to avoid more than 4 degrees of nesting */
        /* socket call failed */
        X2AP_TRACE(X2AP_INFO,
                " creating a server socket failed [errno:%d]",sk_errno);
        return INVALID_SCTP_SOCKET;
    }
    if (sctp_setsockopt(server_sctp_sd, SOL_SOCKET,
                SCTP_SO_REUSEADDR, &optVal, sizeof(U32)) == -1) 
    {
        X2AP_TRACE(X2AP_INFO," Server socket property SCTP_SO_REUSEADDR not set"
                "[errno:%d]",sk_errno);
        return INVALID_SCTP_SOCKET;
    }

    /*Bind created socket to SCTP Port*/
        if( sctp_bind(server_sctp_sd, (struct sockaddr *)
                    &selfENBServerAddr, addr_len) == -1 ) {
            /* Error while binding socket. Free the memory allocated and
               exit the code and return. Using multiple return in this function
               to avoid more than 4 degrees of nesting */
            /* bind call failed for socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_INFO, " Server bind failure, errno: %d",sk_errno);
            /*unable to bind thus deleting created socket*/
            if( sctp_close(server_sctp_sd) == 0 ) {
                /*association closed successfully*/
                /* Trace the successful closure of socket */
                X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            }
            else 
            {
                /* failure in closing socket [%d] */
                X2AP_TRACE(X2AP_WARNING, 
                        " Server socket close failure, errno: %d",sk_errno);
            }

            return INVALID_SCTP_SOCKET;
        }
        

    /* As SCTP supports multi-homing thus more than one ipaddress can be 
       binded to a socket*/

    while(numIpAddress <  no_of_ip_addr)
    {
        /* check to be verified wrt null condition */
        selfENBServerAddr.sin_addr.s_addr = ( inet_addr_wrapper((const char *)(p_comm_info->
                        ip_addr[numIpAddress].ip_addr)) );

        if( sctp_bindx(server_sctp_sd, 
                    (struct sockaddr*)&selfENBServerAddr, (U32)1,
                    SCTP_BINDX_ADD_ADDR ) == -1) {
            /* Error while binding socket. Free the memory allocated and
               exit the code and return. Using multiple return in this function
               to avoid more than 4 degrees of nesting */
            X2AP_TRACE(X2AP_INFO, " sctp_bindx failure, errno: %d",sk_errno);

            /* failire in binding socket [%d], p_sctp_fd_info->server_sctp_fd */
            if( sctp_close(server_sctp_sd) == 0 ) {
                /*association closed successfully*/
                /* Trace the successful closure of socket */
                X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            }
            else 
            {
                /* failure in closing socket [%d] */
                X2AP_TRACE(X2AP_WARNING, 
                        " Server socket close failure, errno: %d",sk_errno);
            }
            return INVALID_SCTP_SOCKET;
        }
        numIpAddress++;
    }
    /*
     * Set non blocking using fcntl
     */
     if (INVALID_SCTP_SOCKET == x2ap_open_sctp_server_sctp_setsockopt(server_sctp_sd, p_sctp_assoc_conf_info))
     {
	 X2AP_TRACE(X2AP_INFO," [%s] x2ap_open_sctp_client_sctp_setsockopt failed"
                ,__FUNCTION__);
         return INVALID_SCTP_SOCKET;
     }

     /* dscp_marking_start */
#ifdef SCTP_IP_HEADER_PARAMS
     if(X2AP_DSCP_VALUE_PRESENT & p_sctp_assoc_conf_info->bitmask)
     { 
         memset_wrapper(&header_params, 0, sizeof(header_params));

         header_params.sip_assoc_id = (U32)server_sctp_sd;

	 /* SPR-22189 START */
         header_params.sip_tos = p_sctp_assoc_conf_info->spp_dscp << 2;
	 /* SPR-22189 END */

         if (0 > sctp_setsockopt(
                     server_sctp_sd,
                     IPPROTO_SCTP,
                     SCTP_IP_DSCP,
                     &header_params,
                     sizeof(sctp_assoc_ip_hdr_params_st)))
         {
             X2AP_TRACE(X2AP_WARNING,
                     "SCTP : sctp_setsockopt for setting dscp value failed\n");
             if( sctp_close(server_sctp_sd) == 0 )
             {
                 X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
             }
             else
             {
                 X2AP_TRACE(X2AP_WARNING,
                         " Server socket close failure, errno: %d",sk_errno);
             }
            return INVALID_SCTP_SOCKET;
        }
         else
         {
		 /* SPR-22189 START */
             X2AP_TRACE(X2AP_INFO, "dscp value is succeefully set by : %d",
                     p_sctp_assoc_conf_info->spp_dscp);
		/* SPR-22189 END */
         }
     }
#endif
     /* dscp_marking_stop */



    if (sctp_listen(server_sctp_sd, num_peer_enb) 
            == -1 ) 
    {
        /* Error while listening socket. Free the memory allocated and
           exit the code and return. Using multiple return in this function
           to avoid more than 4 degrees of nesting */
        /* listen failed for socket [%d], p_sctp_fd_info->server_sctp_fd */
        X2AP_TRACE(X2AP_WARNING, " listen() failure, errno: %d",sk_errno);
        /* listen call failed thus deleting created socket */
        if( sctp_close(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d",sk_errno);
    }

        return INVALID_SCTP_SOCKET;
    }

    /* SPR_14436_Fix Start */
    initmsg.sinit_num_ostreams  = nostreams;
    initmsg.sinit_max_instreams = nistreams;
    /*sctp parameters start*/
    initmsg.sinit_max_init_timeo = SCTP_DEFAULT_INIT_TIMEOUT; 
    /*Coverity_CID_94898_FIX_START*/
    initmsg.sinit_max_attempts = p_sctp_assoc_conf_info->init_msg.sinit_max_attempts;
    /*Coverity_CID_94898_FIX_END*/
    /*sctp parameters stop*/
    return_value = sctp_setsockopt(
                                   server_sctp_sd,
                                   IPPROTO_SCTP,
                                   SCTP_SO_INITMSG,
                                   &initmsg,
                                   sizeof(initmsg));

    if (return_value < 0) 
    {    
        X2AP_TRACE(X2AP_WARNING,"SCTP : sctp_setsockopt for INIT failed\n");
        x2ap_close_socket(server_sctp_sd);
        return X2AP_SCTP_ERROR;
    }    

    X2AP_TRACE(X2AP_INFO,"SCTP : setsockopt for INIT success\n");
    /* SPR_14436_Fix Stop */
   
     X2AP_MEMSET( (void *)&events, 0, sizeof(events) );

    /* Populating SCTP Events structure*/
    events.sctp_association_event  = 1;
    events.sctp_data_io_event      = 1;
    events.sctp_address_event      = 1;
    events.sctp_send_failure_event = 1;
    events.sctp_peer_error_event   = 1;
    events.sctp_shutdown_event     = 1;

    if (0 > sctp_setsockopt(server_sctp_sd, IPPROTO_SCTP, SCTP_SO_SET_EVENTS, 
                &events,
                sizeof (events)))
    {
        X2AP_TRACE(X2AP_WARNING,
                "SCTP : sctp_setsockopt for setting events failed\n");
        if( sctp_close(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d",sk_errno);
        }
        return INVALID_SCTP_SOCKET;
    }

    /* Bug 908 Fix Start */
    if (0 > sctp_setsockopt( server_sctp_sd, IPPROTO_SCTP,
                SCTP_SO_NODELAY, &noDelay, sizeof(noDelay)))
    {
        X2AP_TRACE(X2AP_WARNING,
                "SCTP : sctp_setsockopt for setting No-Delay failed\n");
        if( sctp_close(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d",sk_errno);
        }
        return INVALID_SCTP_SOCKET;
    }
    /* Bug 908 Fix Stop */

    /*
     * Add FD to set
     */
    SCTP_FD_SET(server_sctp_sd, &(g_sctp_fd_set));
    g_sctp_sd_list[g_sctp_sd_count] = server_sctp_sd;
    g_sctp_sd_count++;

    /* L3-X2AP Integration Activity */
    SCTP_FD_SET(server_sctp_sd, &(sctp_fd));

    /*
     * Store server fd in global static var
     */
    /* SPR 20633 Fix Start */
    g_sctp_server_fd = server_sctp_sd;
    /* SPR 20633 fix End */
    X2AP_UT_TRACE_EXIT();
    return server_sctp_sd;
}


/*****************************************************************************
 * Function Name  : x2ap_open_sctp_server  
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_server_info - pointer to x2ap_sctp_conn_info_t
 *                  num_peer_enb - num of peer eNBs
 * Outputs        : None
 * Returns        : signed int 
 * Description    : This function opens a sctp connection for server
 ********************************************************************************/
S32 x2ap_open_sctp_server(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_sctp_conn_info_t *p_server_info,
        U32 num_peer_enb)
{
    U32    numIpAddress = 1; /* Value is kept 1 as for the first Ipaddress bind
                                 is done and for every other successive 
                                 IpAddress bindx is performend*/

    U32   optVal = 1;
    x2_enb_comm_info_t *p_comm_info = p_server_info->p_comm_info;
    S32 server_sctp_sd = INVALID_SCTP_SOCKET;
    struct sctp_event_subscribe events;
    U32 addr_family;         /* This variable is taken for facilitation of IPv6
                                enhancement */
    /*dscp_marking_start*/
#ifdef SCTP_IP_HEADER_PARAMS
    sctp_assoc_ip_hdr_params_st header_params;
#endif
    /*dscp_marking_stop*/
    U32 addr_len = RRC_NULL;
    U8  no_of_ip_addr = RRC_NULL;
    struct sockaddr_in6 selfENBServerAddr6;
    /* Bug 908 Fix Start */
    U32 noDelay = 1;
    /* Bug 908 Fix Stop */
    /* SPR_14436_Fix Start */
    U32 nostreams = X2AP_NULL;
    U32 nistreams = X2AP_NULL;

    nostreams = p_x2ap_gb_ctx->config_param.sctp_conf_param.init_num_ostreams;
    nistreams = p_x2ap_gb_ctx->config_param.sctp_conf_param.init_max_instreams;

    sctp_initmsg_st initmsg;
    S32 return_value  = RRC_NULL;
    /* SPR_14436_Fix Stop */

    X2AP_UT_TRACE_ENTER();

    addr_family = 10;
    no_of_ip_addr = p_comm_info->num_ipv6_addr;
    /* This denotes the sock addr 
       structure usd as param in bind call */

    x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info = 
        p_server_info->p_sctp_assoc_conf_info;
    /*Cov 88188 fix start*/
    if(!p_sctp_assoc_conf_info)
    {
        return X2AP_SCTP_ERROR;
    }
    /*Cov 88188 fix end*/

    /*check that atleast one ip address needs to be configured by operator*/
    if( no_of_ip_addr <= 0 ) {
        return INVALID_SCTP_SOCKET;
    }


    /* Initializing the SCTP stack */
    if( 1 != open_sctp_server_init_sctp_stack(p_comm_info,addr_family))
        {
        return X2AP_SCTP_ERROR;
    }

        selfENBServerAddr6.sin6_family     = 10;
        selfENBServerAddr6.sin6_port        = htons_wrapper(p_comm_info->port);
        if(inet_pton_wrapper(10,(const char *)p_comm_info->ipv6_addr[0].ipv6_addr,
                        (void*)&selfENBServerAddr6.sin6_addr)!=1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert INET6 adress");
                return X2AP_SCTP_ERROR;
            }
        X2AP_TRACE(X2AP_INFO,"[%s]::X2AP Server IPv6:%s and port:%d \n",__FUNCTION__,
                (p_comm_info->ipv6_addr[0].ipv6_addr),p_comm_info->port);
        addr_len = sizeof(struct sockaddr_in6);


    /*Create a server socket for SCTP to run on*/
    if( (server_sctp_sd = sctp_socket(addr_family, SCTP_SOCK_STREAM, 
                    IPPROTO_SCTP)) == -1 ) {
        /* Error while creating socket. Free the memory allocated and 
           exit the code and return. Using multiple return in this function
           to avoid more than 4 degrees of nesting */
        /* socket call failed */
        X2AP_TRACE(X2AP_INFO,
                " creating a server socket failed [errno:%d]",sk_errno);
        return INVALID_SCTP_SOCKET;
    }
    if (sctp_setsockopt(server_sctp_sd, SOL_SOCKET,
                SCTP_SO_REUSEADDR, &optVal, sizeof(U32)) == -1) 
    {
        X2AP_TRACE(X2AP_INFO," Server socket property SCTP_SO_REUSEADDR not set"
                "[errno:%d]",sk_errno);
        return INVALID_SCTP_SOCKET;
    }

    /*Bind created socket to SCTP Port*/
        if( sctp_bind(server_sctp_sd, (struct sockaddr *)
                    &selfENBServerAddr6, addr_len) == -1 ) {
                /* Error while binding socket. Free the memory allocated and
                   exit the code and return. Using multiple return in this function
                   to avoid more than 4 degrees of nesting */
            /* bind call failed for socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_INFO, " Server bind failure, errno: %d",sk_errno);
            /*unable to bind thus deleting created socket*/
                if( sctp_close(server_sctp_sd) == 0 ) {
                    /*association closed successfully*/
                    /* Trace the successful closure of socket */
                    X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
                }
                else 
                {
                    /* failure in closing socket [%d] */
                    X2AP_TRACE(X2AP_WARNING,
                            " Server socket close failure, errno: %d",sk_errno);
                }
                return INVALID_SCTP_SOCKET;
            }



    /* As SCTP supports multi-homing thus more than one ipaddress can be 
       binded to a socket*/

    while(numIpAddress <  no_of_ip_addr)
    {
        /* check to be verified wrt null condition */
            if(inet_pton_wrapper(10,(const char *)p_comm_info->ipv6_addr[numIpAddress].ipv6_addr,
                        (void*)&selfENBServerAddr6.sin6_addr)!=1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert INET6 adress");
                return X2AP_SCTP_ERROR;
        }
            if( sctp_bindx(server_sctp_sd, 
                        (struct sockaddr*)&selfENBServerAddr6, (U32)1,
                        SCTP_BINDX_ADD_ADDR ) == -1) {
                /* Error while binding socket. Free the memory allocated and
                   exit the code and return. Using multiple return in this function
                   to avoid more than 4 degrees of nesting */
                X2AP_TRACE(X2AP_INFO, " sctp_bindx failure, errno: %d",sk_errno);

                /* failire in binding socket [%d], p_sctp_fd_info->server_sctp_fd */
                if( sctp_close(server_sctp_sd) == 0 ) {
                    /*association closed successfully*/
                    /* Trace the successful closure of socket */
                    X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
                }
                else 
                {
                    /* failure in closing socket [%d] */
                    X2AP_TRACE(X2AP_WARNING,
                            " Server socket close failure, errno: %d",sk_errno);
                }
                return INVALID_SCTP_SOCKET;
            }
        numIpAddress++;
    }
    /*
     * Set non blocking using fcntl
     */
     if (INVALID_SCTP_SOCKET == x2ap_open_sctp_server_sctp_setsockopt(server_sctp_sd, p_sctp_assoc_conf_info))
     {
	 X2AP_TRACE(X2AP_INFO," [%s] x2ap_open_sctp_client_sctp_setsockopt failed"
                ,__FUNCTION__);
         return INVALID_SCTP_SOCKET;
     }

     /* dscp_marking_start */
#ifdef SCTP_IP_HEADER_PARAMS
     if(X2AP_DSCP_VALUE_PRESENT & p_sctp_assoc_conf_info->bitmask)
     { 
         memset_wrapper(&header_params, 0, sizeof(header_params));

         header_params.sip_assoc_id = (U32)server_sctp_sd;

	 /* SPR-22189 START */
            header_params.sip_tclass = p_sctp_assoc_conf_info->spp_dscp << 2;
	 /* SPR-22189 END */

         if (0 > sctp_setsockopt(
                     server_sctp_sd,
                     IPPROTO_SCTP,
                     SCTP_IP_DSCP,
                     &header_params,
                     sizeof(sctp_assoc_ip_hdr_params_st)))
         {
             X2AP_TRACE(X2AP_WARNING,
                     "SCTP : sctp_setsockopt for setting dscp value failed\n");
             if( sctp_close(server_sctp_sd) == 0 )
             {
                 X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
             }
             else
             {
                 X2AP_TRACE(X2AP_WARNING,
                         " Server socket close failure, errno: %d",sk_errno);
             }
             return INVALID_SCTP_SOCKET;
         }
         else
         {
	 /* SPR-22189 START */
             X2AP_TRACE(X2AP_INFO, "dscp value is succeefully set by : %d",
                     p_sctp_assoc_conf_info->spp_dscp);
	     /* SPR-22189 END */
         }
     }
#endif
     /* dscp_marking_stop */



    if (sctp_listen(server_sctp_sd, num_peer_enb) 
            == -1 ) 
    {
        /* Error while listening socket. Free the memory allocated and
           exit the code and return. Using multiple return in this function
           to avoid more than 4 degrees of nesting */
        /* listen failed for socket [%d], p_sctp_fd_info->server_sctp_fd */
        X2AP_TRACE(X2AP_WARNING, " listen() failure, errno: %d",sk_errno);
        /* listen call failed thus deleting created socket */
        if( sctp_close(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d",sk_errno);
        }

        return INVALID_SCTP_SOCKET;
    }

    /* SPR_14436_Fix Start */
    initmsg.sinit_num_ostreams  = nostreams;
    initmsg.sinit_max_instreams = nistreams;
    /*sctp parameters start*/
    initmsg.sinit_max_init_timeo = SCTP_DEFAULT_INIT_TIMEOUT; 
    /*Coverity_CID_94898_FIX_START*/
    initmsg.sinit_max_attempts = p_sctp_assoc_conf_info->init_msg.sinit_max_attempts;
    /*Coverity_CID_94898_FIX_END*/
    /*sctp parameters stop*/
    return_value = sctp_setsockopt(
                                   server_sctp_sd,
                                   IPPROTO_SCTP,
                                   SCTP_SO_INITMSG,
                                   &initmsg,
                                   sizeof(initmsg));

    if (return_value < 0) 
    {    
        X2AP_TRACE(X2AP_WARNING,"SCTP : sctp_setsockopt for INIT failed\n");
        x2ap_close_socket(server_sctp_sd);
        return X2AP_SCTP_ERROR;
    }    

    X2AP_TRACE(X2AP_INFO,"SCTP : setsockopt for INIT success\n");
    /* SPR_14436_Fix Stop */
   
     X2AP_MEMSET( (void *)&events, 0, sizeof(events) );

    /* Populating SCTP Events structure*/
    events.sctp_association_event  = 1;
    events.sctp_data_io_event      = 1;
    events.sctp_address_event      = 1;
    events.sctp_send_failure_event = 1;
    events.sctp_peer_error_event   = 1;
    events.sctp_shutdown_event     = 1;

    if (0 > sctp_setsockopt(server_sctp_sd, IPPROTO_SCTP, SCTP_SO_SET_EVENTS, 
                &events,
                sizeof (events)))
    {
        X2AP_TRACE(X2AP_WARNING,
                "SCTP : sctp_setsockopt for setting events failed\n");
        if( sctp_close(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d",sk_errno);
        }
        return INVALID_SCTP_SOCKET;
    }

    /* Bug 908 Fix Start */
    if (0 > sctp_setsockopt( server_sctp_sd, IPPROTO_SCTP,
                SCTP_SO_NODELAY, &noDelay, sizeof(noDelay)))
    {
        X2AP_TRACE(X2AP_WARNING,
                "SCTP : sctp_setsockopt for setting No-Delay failed\n");
        if( sctp_close(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d",sk_errno);
        }
        return INVALID_SCTP_SOCKET;
    }
    /* Bug 908 Fix Stop */

    /*
     * Add FD to set
     */
    SCTP_FD_SET(server_sctp_sd, &(g_sctp_fd_set));
    g_sctp_sd_list[g_sctp_sd_count] = server_sctp_sd;
    g_sctp_sd_count++;
    /* SPR 20633 Fix Start */
    /* L3-X2AP Integration Activity */
    SCTP_FD_SET(server_sctp_sd, &(sctp_fd));

    /*
     * Store server fd in global static var
     */
    g_sctp_server_fd = server_sctp_sd;
    X2AP_UT_TRACE_EXIT();
    return server_sctp_sd;
}


S32 x2ap_open_sctp_server_both(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_sctp_conn_info_t *p_server_info,
        U32 num_peer_enb)
{

    U32   optVal = 1;
    x2_enb_comm_info_t *p_comm_info = p_server_info->p_comm_info;
    S32 server_sctp_sd = INVALID_SCTP_SOCKET;
    struct sctp_event_subscribe events;
    U32 addr_family;         /* This variable is taken for facilitation of IPv6
                                enhancement */
    /*dscp_marking_start*/
#ifdef SCTP_IP_HEADER_PARAMS
    sctp_assoc_ip_hdr_params_st header_params;
#endif
    /*dscp_marking_stop*/
    U32 addr_len = RRC_NULL;
    U8  no_of_ip_addr = RRC_NULL;
    struct sockaddr_in6 selfENBServerAddr6;
    struct sockaddr_in selfENBServerAddr; 
    /* Bug 908 Fix Start */
    U32 noDelay = 1;
    /* Bug 908 Fix Stop */
    /* SPR_14436_Fix Start */
    U32 nostreams = X2AP_NULL;
    U32 nistreams = X2AP_NULL;

    nostreams = p_x2ap_gb_ctx->config_param.sctp_conf_param.init_num_ostreams;
    nistreams = p_x2ap_gb_ctx->config_param.sctp_conf_param.init_max_instreams;

    sctp_initmsg_st initmsg;
    S32 return_value  = RRC_NULL;
    /* SPR_14436_Fix Stop */

    X2AP_UT_TRACE_ENTER();

    addr_family = 10;
    no_of_ip_addr = p_comm_info->num_ipv6_addr;
    /* This denotes the sock addr 
       structure usd as param in bind call */

    x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info = 
        p_server_info->p_sctp_assoc_conf_info;
    /*Cov 88188 fix start*/
    if(!p_sctp_assoc_conf_info)
    {
        return X2AP_SCTP_ERROR;
    }
    /*Cov 88188 fix end*/

    /*check that atleast one ip address needs to be configured by operator*/
    if( no_of_ip_addr <= 0 ) {
        return INVALID_SCTP_SOCKET;
    }


    /* Initializing the SCTP stack */
    if( 1 != open_sctp_server_init_sctp_stack(p_comm_info,addr_family))
        {
        return X2AP_SCTP_ERROR;
    }

        selfENBServerAddr6.sin6_family     = 10;
        selfENBServerAddr6.sin6_port        = htons_wrapper(p_comm_info->port);
        if(inet_pton_wrapper(10,(const char *)p_comm_info->ipv6_addr[0].ipv6_addr,
                        (void*)&selfENBServerAddr6.sin6_addr)!=1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert INET6 adress");
                return X2AP_SCTP_ERROR;
            }
        X2AP_TRACE(X2AP_INFO,"[%s]::X2AP Server IPv6:%s and port:%d \n",__FUNCTION__,
                (p_comm_info->ipv6_addr[0].ipv6_addr),p_comm_info->port);
        addr_len = sizeof(struct sockaddr_in6);




        selfENBServerAddr.sin_family = SCTP_AF_INET;
        selfENBServerAddr.sin_addr.s_addr = 
            ( inet_addr_wrapper((const char *)(p_comm_info->ip_addr[0].ip_addr)) );
        selfENBServerAddr.sin_port = htons_wrapper(p_comm_info->port);

        X2AP_TRACE(X2AP_INFO,"[%s]::X2AP Server IP:%s and port:%d \n",__FUNCTION__,
                (p_comm_info->ip_addr[0].ip_addr),p_comm_info->port);

    /*Create a server socket for SCTP to run on*/
    if( (server_sctp_sd = sctp_socket(addr_family, SCTP_SOCK_STREAM, 
                    IPPROTO_SCTP)) == -1 ) {
        /* Error while creating socket. Free the memory allocated and 
           exit the code and return. Using multiple return in this function
           to avoid more than 4 degrees of nesting */
        /* socket call failed */
        X2AP_TRACE(X2AP_INFO,
                " creating a server socket failed [errno:%d]",sk_errno);
        return INVALID_SCTP_SOCKET;
    }
    if (sctp_setsockopt(server_sctp_sd, SOL_SOCKET,
                SCTP_SO_REUSEADDR, &optVal, sizeof(U32)) == -1) 
    {
        X2AP_TRACE(X2AP_INFO," Server socket property SCTP_SO_REUSEADDR not set"
                "[errno:%d]",sk_errno);
        return INVALID_SCTP_SOCKET;
    }

    /*Bind created socket to SCTP Port*/
        if( sctp_bind(server_sctp_sd, (struct sockaddr *)
                    &selfENBServerAddr6, addr_len) == -1 ) {
                /* Error while binding socket. Free the memory allocated and
                   exit the code and return. Using multiple return in this function
                   to avoid more than 4 degrees of nesting */
            /* bind call failed for socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_INFO, " Server bind failure, errno: %d",sk_errno);
            /*unable to bind thus deleting created socket*/
                if( sctp_close(server_sctp_sd) == 0 ) {
                    /*association closed successfully*/
                    /* Trace the successful closure of socket */
                    X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
                }
                else 
                {
                    /* failure in closing socket [%d] */
                    X2AP_TRACE(X2AP_WARNING,
                            " Server socket close failure, errno: %d",sk_errno);
                }
                return INVALID_SCTP_SOCKET;
            }

        if( sctp_bindx(server_sctp_sd, (struct sockaddr *)
                    &selfENBServerAddr,(U32)1,SCTP_BINDX_ADD_ADDR) == -1 ) {
            /* Error while binding socket. Free the memory allocated and
               exit the code and return. Using multiple return in this function
               to avoid more than 4 degrees of nesting */
            /* bind call failed for socket [%d], p_sctp_fd_info->server_sctp_fd */
            fprintf(stderr,"SIDDHARTH BIND FAILED FOR IPV4");
            X2AP_TRACE(X2AP_INFO, " Server bind failure, errno: %d",sk_errno);
            /*unable to bind thus deleting created socket*/
            if( sctp_close(server_sctp_sd) == 0 ) {
                /*association closed successfully*/
                /* Trace the successful closure of socket */
                X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            }
            else 
            {
                /* failure in closing socket [%d] */
                X2AP_TRACE(X2AP_WARNING, 
                        " Server socket close failure, errno: %d",sk_errno);
            }

            return INVALID_SCTP_SOCKET;
        }


    /* As SCTP supports multi-homing thus more than one ipaddress can be 
       binded to a socket*/

    /*
     * Set non blocking using fcntl
     */
     if (INVALID_SCTP_SOCKET == x2ap_open_sctp_server_sctp_setsockopt(server_sctp_sd, p_sctp_assoc_conf_info))
     {
	 X2AP_TRACE(X2AP_INFO," [%s] x2ap_open_sctp_client_sctp_setsockopt failed"
                ,__FUNCTION__);
         return INVALID_SCTP_SOCKET;
     }

     /* dscp_marking_start */
#ifdef SCTP_IP_HEADER_PARAMS
     if(X2AP_DSCP_VALUE_PRESENT & p_sctp_assoc_conf_info->bitmask)
     { 
         memset_wrapper(&header_params, 0, sizeof(header_params));

         header_params.sip_assoc_id = (U32)server_sctp_sd;

	 /* SPR-22189 START */
            header_params.sip_tclass = p_sctp_assoc_conf_info->spp_dscp << 2;
         /* SPR-22189 END */

         if (0 > sctp_setsockopt(
                     server_sctp_sd,
                     IPPROTO_SCTP,
                     SCTP_IP_DSCP,
                     &header_params,
                     sizeof(sctp_assoc_ip_hdr_params_st)))
         {
             X2AP_TRACE(X2AP_WARNING,
                     "SCTP : sctp_setsockopt for setting dscp value failed\n");
             if( sctp_close(server_sctp_sd) == 0 )
             {
                 X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
             }
             else
             {
                 X2AP_TRACE(X2AP_WARNING,
                         " Server socket close failure, errno: %d",sk_errno);
             }
             return INVALID_SCTP_SOCKET;
         }
         else
         {
	 /* SPR-22189 START */
             X2AP_TRACE(X2AP_INFO, "dscp value is succeefully set by : %d",
                     p_sctp_assoc_conf_info->spp_dscp);
	 /* SPR-22189 END */
         }
     }
#endif
     /* dscp_marking_stop */



    if (sctp_listen(server_sctp_sd, num_peer_enb) 
            == -1 ) 
    {
        /* Error while listening socket. Free the memory allocated and
           exit the code and return. Using multiple return in this function
           to avoid more than 4 degrees of nesting */
        /* listen failed for socket [%d], p_sctp_fd_info->server_sctp_fd */
        X2AP_TRACE(X2AP_WARNING, " listen() failure, errno: %d",sk_errno);
        /* listen call failed thus deleting created socket */
        if( sctp_close(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d",sk_errno);
        }

        return INVALID_SCTP_SOCKET;
    }

    /* SPR_14436_Fix Start */
    initmsg.sinit_num_ostreams  = nostreams;
    initmsg.sinit_max_instreams = nistreams;
    /*sctp parameters start*/
    initmsg.sinit_max_init_timeo = SCTP_DEFAULT_INIT_TIMEOUT; 
    /*Coverity_CID_94898_FIX_START*/
    initmsg.sinit_max_attempts = p_sctp_assoc_conf_info->init_msg.sinit_max_attempts;
    /*Coverity_CID_94898_FIX_END*/
    /*sctp parameters stop*/
    return_value = sctp_setsockopt(
                                   server_sctp_sd,
                                   IPPROTO_SCTP,
                                   SCTP_SO_INITMSG,
                                   &initmsg,
                                   sizeof(initmsg));

    if (return_value < 0) 
    {    
        X2AP_TRACE(X2AP_WARNING,"SCTP : sctp_setsockopt for INIT failed\n");
        x2ap_close_socket(server_sctp_sd);
        return X2AP_SCTP_ERROR;
    }    

    X2AP_TRACE(X2AP_INFO,"SCTP : setsockopt for INIT success\n");
    /* SPR_14436_Fix Stop */
   
     X2AP_MEMSET( (void *)&events, 0, sizeof(events) );

    /* Populating SCTP Events structure*/
    events.sctp_association_event  = 1;
    events.sctp_data_io_event      = 1;
    events.sctp_address_event      = 1;
    events.sctp_send_failure_event = 1;
    events.sctp_peer_error_event   = 1;
    events.sctp_shutdown_event     = 1;

    if (0 > sctp_setsockopt(server_sctp_sd, IPPROTO_SCTP, SCTP_SO_SET_EVENTS, 
                &events,
                sizeof (events)))
    {
        X2AP_TRACE(X2AP_WARNING,
                "SCTP : sctp_setsockopt for setting events failed\n");
        if( sctp_close(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d",sk_errno);
        }
        return INVALID_SCTP_SOCKET;
    }

    /* Bug 908 Fix Start */
    if (0 > sctp_setsockopt( server_sctp_sd, IPPROTO_SCTP,
                SCTP_SO_NODELAY, &noDelay, sizeof(noDelay)))
    {
        X2AP_TRACE(X2AP_WARNING,
                "SCTP : sctp_setsockopt for setting No-Delay failed\n");
        if( sctp_close(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d",sk_errno);
        }
        return INVALID_SCTP_SOCKET;
    }
    /* Bug 908 Fix Stop */

    /*
     * Add FD to set
     */
    SCTP_FD_SET(server_sctp_sd, &(g_sctp_fd_set));
    g_sctp_sd_list[g_sctp_sd_count] = server_sctp_sd;
    g_sctp_sd_count++;
    /* L3-X2AP Integration Activity */
    SCTP_FD_SET(server_sctp_sd, &(sctp_fd));

    /*
     * Store server fd in global static var
     */
    g_sctp_server_fd = server_sctp_sd;
    X2AP_UT_TRACE_EXIT();
    return server_sctp_sd;
}
/* SPR 20633 changes end */
/*****************************************************************************
 * Function Name  : x2ap_open_sctp_client_sctp_setsockopt 
 * Inputs         : p_connect_server_info - pointer to x2ap_sctp_conn_info_t
 *                  p_client_info - pointer to x2ap_sctp_conn_info_t
 * Outputs        : None
 * Returns        : signed int 
 * Description    : this function opens a connection for sctp client
 ********************************************************************************/
S32 x2ap_open_sctp_client_sctp_setsockopt(
	S32 connSock, 
	x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info)
{
    U32 optVal =1;
    U32 blocked = 1;
    if (sctp_setsockopt(connSock, SOL_SOCKET,
                SCTP_SO_REUSEADDR, &optVal, sizeof(U32)) != -1) 
    {
        if (sctp_setsockopt(connSock, SOL_SOCKET,
                    SCTP_SO_NONBLOCKING, &blocked, sizeof(U32)) != -1) 
        {

            if (sctp_setsockopt(connSock, IPPROTO_SCTP, SCTP_SO_INITMSG, 
                        &(p_sctp_assoc_conf_info->init_msg), 
                        sizeof(p_sctp_assoc_conf_info->init_msg) ) == -1) 
            {
                X2AP_TRACE(X2AP_INFO," [%s] SCTP_SO_INITMSG failed errno: %d \n"
                        ,__FUNCTION__,sk_errno);
                /*unable to set socket options thus deleting created socket*/
                if( sctp_close(connSock) == 0 ) 
                {
                    /* association closed successfully */ 
                    /* Trace the successful closure of socket */
                    X2AP_TRACE(X2AP_INFO, "Client socket closed is: %d", connSock);
                }
                else 
                {
                    /* unable to close socket [%d], connSock */
                    X2AP_TRACE(X2AP_INFO," [%s] sctp_close() failed errno: %d \n"
                            ,__FUNCTION__,sk_errno);
                }
                return INVALID_SCTP_SOCKET;
            }

            /*sctp parameters start*/
            if (sctp_setsockopt(connSock, SOL_SOCKET,
                        SCTP_SO_ASSOCINFO, 
                        &(p_sctp_assoc_conf_info->assocparams), 
                        sizeof(p_sctp_assoc_conf_info->assocparams) ) == -1) {
                /*unable to set socket options thus deleting created socket*/
                X2AP_TRACE(X2AP_INFO," Client  socket property SCTP_SO_ASSOCINFO not set" 
                        "[errno:%d]",sk_errno);
                if( sctp_close(connSock) == 0 ) 
                {
                    /* association closed successfully */ 
                    /* Trace the successful closure of socket */
                    X2AP_TRACE(X2AP_INFO, "Client socket closed is: %d", connSock);
                }
                else 
                {
                    /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
                    X2AP_TRACE(X2AP_WARNING, 
                            " Client socket close failure, errno: %d",sk_errno);
                }
                return INVALID_SCTP_SOCKET;
            }

            if (sctp_setsockopt(connSock, IPPROTO_SCTP,
                        SCTP_SO_RTOINFO, 
                        &(p_sctp_assoc_conf_info->rtoinfo), 
                        sizeof(p_sctp_assoc_conf_info->rtoinfo) ) == -1) {
                /*unable to set socket options thus deleting created socket*/
                X2AP_TRACE(X2AP_INFO," Client socket property SCTP_SO_RTOINFO not set" 
                        "[errno:%d]",sk_errno);
                if( sctp_close(connSock) == 0 ) 
                {
                    /* association closed successfully */ 
                    /* Trace the successful closure of socket */
                    X2AP_TRACE(X2AP_INFO, "Client socket closed is: %d", connSock);
                }
                else 
                {
                    /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
                    X2AP_TRACE(X2AP_WARNING, 
                            " Client socket close failure, errno: %d",sk_errno);
                }
                return INVALID_SCTP_SOCKET;
            }
            /*sctp parameters stop*/
        }
        else
        {
            X2AP_TRACE(X2AP_INFO," [%s] SCTP_SO_NONBLOCKING failed errno: %d \n"
                    ,__FUNCTION__,sk_errno);
            return INVALID_SCTP_SOCKET;
        }
    }
    else
    {
        X2AP_TRACE(X2AP_INFO," [%s] SCTP_SO_REUSEADDR failed errno: %d \n"
                ,__FUNCTION__,sk_errno);
        return INVALID_SCTP_SOCKET;
    }

    X2AP_TRACE(X2AP_INFO,"SCTP : setsockopt for INIT success\n");
    return connSock;
}

/*****************************************************************************
 * Function Name  : x2ap_open_sctp_client 
 * Inputs         : p_connect_server_info - pointer to x2ap_sctp_conn_info_t
 *                  p_client_info - pointer to x2ap_sctp_conn_info_t
 * Outputs        : None
 * Returns        : signed int 
 * Description    : this function opens a connection for SCTP client 
 ********************************************************************************/
S32 x2ap_open_sctp_client(
        x2ap_sctp_conn_info_t 	*p_connect_server_info,/* SPR_14436_Fix Start */
        x2ap_sctp_conn_info_t 	*p_client_info, 
	x2ap_peer_enb_context_t *p_enb_cntxt_arr)/* SPR_14436_Fix Stop */
{
    U32 addr_family;         /* This variable is taken for facilitation of IPv6
                                enhancement */
    U8  no_of_ip_addr = RRC_NULL;
    S32 return_value = -1;
    x2_enb_comm_info_t *p_comm_info = p_connect_server_info->p_comm_info;
    x2_enb_comm_info_t *p_clint_info = p_client_info->p_comm_info;
    x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info = 
        p_connect_server_info->p_sctp_assoc_conf_info;
    S32  connSock      = INVALID_SCTP_SOCKET;
    struct sctp_event_subscribe events;
    U32 numIpAddress = 1;  
    U16 addr_len = RRC_NULL;
    U32 blocked = 1;
    struct sockaddr_in6 clientAddr6; 
    struct sockaddr_in clientAddr; 
    /*sctp parameters start*/
    struct sockaddr_in6 clientAddr_str6[MAX_NUM_IP_ADDR]; 
    struct sockaddr_in clientAddr_str[MAX_NUM_IP_ADDR]; 
    x2_ipv6_addr_t *p_mme_addr_ipv6;
    x2_ip_addr_t *p_mme_addr;
    U32 counter = 0;
    /*dscp_marking_start*/
#ifdef SCTP_IP_HEADER_PARAMS
    sctp_assoc_ip_hdr_params_st header_params;
#endif
    /*dscp_marking_stop*/
    /* SPR_14436_Fix Start */
    U32 nostreams = X2AP_NULL;
    U32 nistreams = X2AP_NULL;

    nostreams = 
        p_connect_server_info->p_sctp_assoc_conf_info->init_msg.sinit_num_ostreams;
    nistreams = 
        p_connect_server_info->p_sctp_assoc_conf_info->init_msg.sinit_max_instreams;
        
    sctp_socklen_t optlen = 0;
    sctp_initmsg_st initmsg;
    U32 number_of_streams = X2AP_NULL;
    /* SPR_14436_Fix Stop */

    /*sctp parameters stop */ 
    /* Bug 908 Fix Start */
    U32 noDelay = 1;
    /* Bug 908 Fix Stop */
    if(p_clint_info->bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {
        no_of_ip_addr = p_clint_info->num_ipv6_addr;
        addr_family = 10;
        addr_len = sizeof(struct sockaddr_in6);
    /*sctp parameters start */ 
        p_mme_addr_ipv6 = &(p_comm_info->ipv6_addr[0]);
    /*sctp parameters stop */ 
    }
    else
    {
        no_of_ip_addr = p_clint_info->num_ip_addr;
        addr_family = SCTP_AF_INET; 
        addr_len = sizeof(struct sockaddr_in);
    /*sctp parameters start */ 
        p_mme_addr = &(p_comm_info->ip_addr[0]);
    /*sctp parameters stop */ 
    }

    X2AP_TRACE(X2AP_INFO," Creating socket..\n");

    if((p_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT) && (p_clint_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT))
    {
        clientAddr6.sin6_family = 10;
        clientAddr6.sin6_port = htons_wrapper(p_clint_info->port);
        if(inet_pton_wrapper(AF_INET6,(const char *)p_clint_info->ipv6_addr[0].ipv6_addr,
                    (void*)&clientAddr6.sin6_addr)!=1)
        {
            RRC_TRACE(RRC_WARNING,"Couldn't convert INET6 adress");
            return X2AP_SCTP_ERROR;
        }
    X2AP_TRACE(X2AP_INFO,"[%s]::IP  %s and port to bind %d \n",__FUNCTION__,
            (p_clint_info->ipv6_addr[0].ipv6_addr),p_clint_info->port);
    }
    else
    {
        clientAddr.sin_family = SCTP_AF_INET;
        clientAddr.sin_addr.s_addr = 
            ( inet_addr_wrapper((const char*)(p_clint_info->ip_addr[0].ip_addr)) );
        clientAddr.sin_port = htons_wrapper(p_clint_info->port);
    X2AP_TRACE(X2AP_INFO,"[%s]::IP  %s and port to bind %d \n",__FUNCTION__,
            (p_clint_info->ip_addr[0].ip_addr),p_clint_info->port);
    }


    /* Create an SCTP TCP-Style Socket */
    connSock = sctp_socket(addr_family, SOCK_STREAM, IPPROTO_SCTP);

    X2AP_TRACE(X2AP_INFO,
            "[%s]::SD is set for client_sctp_sd is %d \n",__FUNCTION__,connSock);

    /*
     * Create non blocking socket using fcntl
     */

    if (connSock < 0)
    {
        X2AP_TRACE(X2AP_INFO," [%s] [connSock %d]socket() call failed errno: %d \n"
                ,__FUNCTION__,connSock,sk_errno);
        return INVALID_SCTP_SOCKET;
    }

    /* SPR_14436_Fix Start */
    initmsg.sinit_num_ostreams  = nostreams;
    initmsg.sinit_max_instreams = nistreams;
    /*sctp parameters start*/
    initmsg.sinit_max_init_timeo = SCTP_DEFAULT_INIT_TIMEOUT; 
    if(PNULL != p_sctp_assoc_conf_info)
    {    
        initmsg.sinit_max_attempts = p_sctp_assoc_conf_info->init_msg.sinit_max_attempts;
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
        X2AP_TRACE(X2AP_WARNING,"SCTP : sctp_setsockopt for INIT failed\n");
        x2ap_close_socket(connSock);
        return X2AP_SCTP_ERROR;
    }    

    X2AP_TRACE(X2AP_INFO,"SCTP : setsockopt for INIT success\n");
    /* SPR_14436_Fix Stop */


    if (INVALID_SCTP_SOCKET == x2ap_open_sctp_client_sctp_setsockopt(connSock,p_sctp_assoc_conf_info))
    {
        X2AP_TRACE(X2AP_INFO," [%s] x2ap_open_sctp_client_sctp_setsockopt failed"
                ,__FUNCTION__);
        return INVALID_SCTP_SOCKET;
	
    }

     /* dscp_marking_start */
#ifdef SCTP_IP_HEADER_PARAMS
     if(X2AP_DSCP_VALUE_PRESENT & p_sctp_assoc_conf_info->bitmask)
     { 
         memset_wrapper(&header_params, 0, sizeof(header_params));

         header_params.sip_assoc_id = (U32)connSock;

        if (p_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)
        {
	 /* SPR-22189 START */
            header_params.sip_tclass = p_sctp_assoc_conf_info->spp_dscp << 2;
	 /* SPR-22189 END */
        }
        else
        {
	 /* SPR-22189 START */
            header_params.sip_tos = p_sctp_assoc_conf_info->spp_dscp << 2;
	 /* SPR-22189 END */
        }

         if (0 > sctp_setsockopt(
                     connSock,
                     IPPROTO_SCTP,
                     SCTP_IP_DSCP,
                     &header_params,
                     sizeof(sctp_assoc_ip_hdr_params_st)))
         {
             X2AP_TRACE(X2AP_WARNING,
                     "SCTP : sctp_setsockopt for setting dscp value failed\n");
             if( sctp_close(connSock) == 0 )
             {
                 X2AP_TRACE(X2AP_INFO, "Client socket closed is: %d", connSock);
             }
             else
             {
                 X2AP_TRACE(X2AP_WARNING,
                         " Client socket close failure, errno: %d",sk_errno);
             }
             return INVALID_SCTP_SOCKET;
         }
         else
         {
	 /* SPR-22189 START */
             X2AP_TRACE(X2AP_INFO, "dscp value is succeefully set by : %d",
                     p_sctp_assoc_conf_info->spp_dscp);
	 /* SPR-22189 END */
         }
     }
#endif
     /* dscp_marking_stop */


    /*Bind created socket to SCTP Port*/
    if((p_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT)  )
    {
        if( sctp_bind(connSock, (struct sockaddr *)
                    &clientAddr6, addr_len) == -1 ) 
        {
            /* Error while binding socket. Free the memory allocated and
               exit the code and return. Using multiple return in this function
               to avoid more than 4 degrees of nesting */
            /* bind call failed for socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_INFO,"SCTP : sctp bind failure errno:%d",sk_errno);
            /*unable to bind thus deleting created socket*/
            if( sctp_close(connSock) == 0 ) 
            {
                /*association closed successfully*/
                /* Trace the successful closure of socket */
                X2AP_TRACE(X2AP_INFO, "Client socket closed is: %d", connSock);
            }
            else 
            {
                /* failure in closing socket [%d] */
                X2AP_TRACE(X2AP_INFO,"SCTP : socket close failure errno:%d",sk_errno);
            }

            return INVALID_SCTP_SOCKET;
        }
    }
    else
    {
        if( sctp_bind(connSock, (struct sockaddr *)
                    &clientAddr, addr_len) == -1 ) 
        {
            /* Error while binding socket. Free the memory allocated and
               exit the code and return. Using multiple return in this function
               to avoid more than 4 degrees of nesting */
            /* bind call failed for socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_INFO,"SCTP : sctp bind failure errno:%d",sk_errno);
            /*unable to bind thus deleting created socket*/
            if( sctp_close(connSock) == 0 ) 
            {
                /*association closed successfully*/
                /* Trace the successful closure of socket */
                X2AP_TRACE(X2AP_INFO, "Client socket closed is: %d", connSock);
            }
            else 
            {
                /* failure in closing socket [%d] */
                X2AP_TRACE(X2AP_INFO,"SCTP : socket close failure errno:%d",sk_errno);
            }

            return INVALID_SCTP_SOCKET;
        }

    }

    while(numIpAddress <  no_of_ip_addr)
    {
        if(p_clint_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT )
        {
            if(inet_pton_wrapper(10,(const char *)p_clint_info->ipv6_addr[numIpAddress].ipv6_addr,
                        (void*)&clientAddr6.sin6_addr)!=1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert INET6 adress");
                return X2AP_SCTP_ERROR;
            }
            if( sctp_bindx(connSock,
                        (struct sockaddr*)&clientAddr, (U32)1,
                        SCTP_BINDX_ADD_ADDR ) == -1) 
            {
                /* Error while binding socket. Free the memory allocated and
                   exit the code and return. Using multiple return in this function
                   to avoid more than 4 degrees of nesting */

                /* failire in binding socket [%d], p_sctp_fd_info->server_sctp_fd */
                X2AP_TRACE(X2AP_INFO, " sctp_bindx failure, errno: %d",sk_errno);

                if( sctp_close(connSock) == 0 ) 
                {
                    /*association closed successfully*/
                    /* Trace the successful closure of socket */
                    X2AP_TRACE(X2AP_INFO, "Client socket closed is: %d", connSock);
                }
                else 
                {
                    /* failure in closing socket [%d] */
                    X2AP_TRACE(X2AP_WARNING, 
                            " Client socket close failure, errno: %d",sk_errno);
                }

                return INVALID_SCTP_SOCKET;
            }
        }
        else
        {
            /* check to be verified wrt null condition */
            clientAddr.sin_addr.s_addr = ( inet_addr_wrapper ((const char *)(p_clint_info->
                            ip_addr[numIpAddress].ip_addr )) );

            if( sctp_bindx(connSock,
                        (struct sockaddr*)&clientAddr, (U32)1,
                        SCTP_BINDX_ADD_ADDR ) == -1) 
            {
                /* Error while binding socket. Free the memory allocated and
                   exit the code and return. Using multiple return in this function
                   to avoid more than 4 degrees of nesting */

                /* failire in binding socket [%d], p_sctp_fd_info->server_sctp_fd */
                X2AP_TRACE(X2AP_INFO, " sctp_bindx failure, errno: %d",sk_errno);

                if( sctp_close(connSock) == 0 ) 
                {
                    /*association closed successfully*/
                    /* Trace the successful closure of socket */
                    X2AP_TRACE(X2AP_INFO, "Client socket closed is: %d", connSock);
                }
                else 
                {
                    /* failure in closing socket [%d] */
                    X2AP_TRACE(X2AP_WARNING, 
                            " Client socket close failure, errno: %d",sk_errno);
                }

                return INVALID_SCTP_SOCKET;
            }
        }
        numIpAddress++;
    }

    /* SPR 13798 Start */
    /* SPR 13798 Stop */
    /* SPR_14436_Fix Start */
    return_value = sctp_getsockopt(
                                   connSock,
                                   IPPROTO_SCTP,
                                   SCTP_SO_INITMSG,
                                   &initmsg,
                                   &optlen);
    if (return_value < 0)
    {
        X2AP_TRACE(X2AP_ERROR,"SCTP : Unable to retreive Number of streams");
        return X2AP_SCTP_ERROR;
    }
    else
    {
        number_of_streams = (U8)initmsg.sinit_num_ostreams;
        if(initmsg.sinit_num_ostreams  <  2 ||
                initmsg.sinit_max_instreams <  2)
        {
            X2AP_TRACE(X2AP_WARNING,
                    "SCTP : Less Number of streams have been opened");
            return X2AP_SCTP_ERROR;
        }
        else if (initmsg.sinit_num_ostreams  >  10 )
        {
            X2AP_TRACE(X2AP_WARNING,"SCTP : More than 10 streams are open");
            return X2AP_SCTP_ERROR;
        }
        else
        {
        if ( PNULL != p_enb_cntxt_arr )
        {
                (p_enb_cntxt_arr->stream_info).number_of_streams = number_of_streams;

            for (counter = 0; counter < number_of_streams; counter++)
                {
                    (p_enb_cntxt_arr->stream_info).stream_ids[counter] = counter;
                }
        }
        }
    }
    /* SPR_14436_Fix Stop */

    X2AP_MEMSET( (void *)&events, 0, sizeof(events) );

    /* Populating SCTP Events structure*/
    events.sctp_association_event  = 1;
    events.sctp_data_io_event      = 1;
    events.sctp_address_event      = 1;
    events.sctp_send_failure_event = 1;
    events.sctp_peer_error_event   = 1;
    events.sctp_shutdown_event     = 1;

    return_value = sctp_setsockopt(connSock, IPPROTO_SCTP, SCTP_SO_SET_EVENTS, 
            &events, sizeof (events));
    if (return_value < 0)
    {
        X2AP_TRACE(X2AP_WARNING,
                "SCTP : sctp_setsockopt for setting events failed\n");
        if( sctp_close(connSock) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Client socket closed is: %d", connSock);
        }
        else 
        {
            /* failure in closing socket [%d] */
            X2AP_TRACE(X2AP_WARNING,
                    " Client socket close failure, errno: %d",sk_errno);
        }
        return INVALID_SCTP_SOCKET;
    }

    /* SPR 13798 Start */
    /* Getting connected with the Peer with multiple IPs */
    return_value = x2ap_sctp_connectx(connSock, p_comm_info);
    if (return_value < 0)
    {
        X2AP_TRACE(X2AP_WARNING,
                "SCTP : x2ap sctp connectx failure: errno: %d",sk_errno);
        if( sctp_close(connSock) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Client socket closed is: %d", connSock);
        }
        else 
        {
            /* failure in closing socket [%d] */
            X2AP_TRACE(X2AP_WARNING, 
                    " Client socket close failure, errno: %d",sk_errno);
        }
        return INVALID_SCTP_SOCKET;
    }

    /* SPR 13798 Stop */

    /* Bug 908 Fix Start */
    X2AP_MEMSET( (void *)&events, 0, sizeof(events) );

    /* Populating SCTP Events structure*/
    events.sctp_association_event  = 1;
    events.sctp_data_io_event      = 1;
    events.sctp_address_event      = 1;
    events.sctp_send_failure_event = 1;
    events.sctp_peer_error_event   = 1;
    events.sctp_shutdown_event     = 1;

    return_value = sctp_setsockopt(connSock, IPPROTO_SCTP, SCTP_SO_SET_EVENTS, 
            &events, sizeof (events));
    if (return_value < 0)
    {
        X2AP_TRACE(X2AP_WARNING,
                "SCTP : sctp_setsockopt for setting events failed\n");
        if( sctp_close(connSock) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Client socket closed is: %d", connSock);
        }
        else 
        {
            /* failure in closing socket [%d] */
            X2AP_TRACE(X2AP_WARNING, 
                    " Client socket close failure, errno: %d",sk_errno);
        }
        return INVALID_SCTP_SOCKET;
    }
    /* Bug 908 Fix Stop */
    /* Bug 908 Fix Start */
    if (0 > sctp_setsockopt( connSock, IPPROTO_SCTP, 
                SCTP_SO_NODELAY, &noDelay, sizeof(noDelay)))
    {
        X2AP_TRACE(X2AP_WARNING,
                "SCTP : sctp_setsockopt for setting No-Delay failed\n");
        if( sctp_close(connSock) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Client socket closed is: %d", connSock);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Client socket close failure, errno: %d",sk_errno);
        }
        return INVALID_SCTP_SOCKET;
    }
    /* Bug 908 Fix Stop */

    for(counter = 0 ; counter < no_of_ip_addr; counter++)
    {
        if(p_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT )
        {
            p_mme_addr_ipv6 = &(p_comm_info->ipv6_addr[0]);
            bzero_wrapper( (void *)&clientAddr_str6[0], sizeof(clientAddr_str6[0]) );
            clientAddr_str6[counter].sin6_family     = AF_INET6;
            clientAddr_str6[counter].sin6_port       = htons_wrapper(p_comm_info->port);
            if(inet_pton_wrapper(AF_INET6,(const char *)p_mme_addr_ipv6->ipv6_addr,
                        (void*)&clientAddr_str6[counter].sin6_addr)!=1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert INET6 adress");
                return X2AP_SCTP_ERROR;
            }
            X2AP_TRACE(X2AP_INFO,"[%s]::X2AP Server IPv6:%s and port:%d \n",__FUNCTION__,
                    (p_mme_addr_ipv6->ipv6_addr),p_comm_info->port);
            p_sctp_assoc_conf_info->paddr.spp_address = 
                *(sctp_sockaddr_storage_st *) &clientAddr_str6[0];
            p_mme_addr_ipv6++;
        }
        else
        {
            p_mme_addr = &(p_comm_info->ip_addr[0]);
            bzero_wrapper( (void *)&clientAddr_str[0], sizeof(clientAddr_str[0]) );
            clientAddr_str[counter].sin_family = SCTP_AF_INET;
            clientAddr_str[counter].sin_addr.s_addr = 
                ( inet_addr_wrapper((const char *)(p_mme_addr->ip_addr)) );
            clientAddr_str[counter].sin_port = htons_wrapper(p_comm_info->port);

            X2AP_TRACE(X2AP_INFO,"[%s]::X2AP Server IP:%s and port:%d \n",__FUNCTION__,
                    (p_mme_addr->ip_addr),p_comm_info->port);
            p_sctp_assoc_conf_info->paddr.spp_address = 
                *(sctp_sockaddr_storage_st *)&clientAddr_str[0];
            p_mme_addr++;
        }
        if (sctp_setsockopt(connSock, SOL_SOCKET,
                    SCTP_SO_NONBLOCKING, &blocked, sizeof(U32)) != -1) 
        {
            if (sctp_setsockopt(connSock, IPPROTO_SCTP,
                        SCTP_PEER_ADDR_PARAMS, 
                        &(p_sctp_assoc_conf_info->paddr), 
                        sizeof(p_sctp_assoc_conf_info->paddr) ) == -1) {
                /*unable to set socket options thus deleting created socket*/
                X2AP_TRACE(X2AP_INFO," Server socket property SCTP_PEER_ADDR_PARAMS not set" 
                        "[errno:%d]",sk_errno);
                if( sctp_close(connSock) == 0 ) 
                {
                    /* association closed successfully */ 
                    /* Trace the successful closure of socket */
                    X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", connSock);
                }
                else 
                {
                    /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
                    X2AP_TRACE(X2AP_WARNING, 
                            " Server socket close failure, errno: %d",sk_errno);
                }
                return INVALID_SCTP_SOCKET;
            }
        }
    }
    SCTP_FD_SET(connSock, &(g_sctp_fd_set) );
    g_sctp_sd_list[g_sctp_sd_count] = connSock;
    g_sctp_sd_count++;

    /* L3-X2AP Integration Activity */
    SCTP_FD_SET(connSock, &(sctp_fd) );

    return connSock;
}

/*
 * Helper fucntion for sctp_connectx.
 */
/*****************************************************************************
 * Function Name  : x2ap_sctp_connectx 
 * Inputs         : connSock - sock fd
 *                  p_comm_info - pointer to x2_enb_comm_info_t
 * Outputs        : None
 * Returns        : static signed int
 * Description    : This function creats sctp_connectx.
 ********************************************************************************/
static S32 x2ap_sctp_connectx(
        S32       connSock,
        x2_enb_comm_info_t *p_comm_info)
{
    U32 counter = 0;
    U8 no_enb_ip_addr = RRC_NULL;
    struct sockaddr_in6 servaddr6[MAX_NUM_IP_ADDR];
    struct sockaddr_in servaddr[MAX_NUM_IP_ADDR];
    if(p_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT)
    {
        no_enb_ip_addr = p_comm_info->num_ipv6_addr;
    }
    else
    {
        no_enb_ip_addr = p_comm_info->num_ip_addr;
    }



    /* Specify the peer endpoint(s) to which we'll connect */

    for(counter = 0; counter < no_enb_ip_addr; counter++)
    {
        if(p_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT )
        {
            bzero_wrapper( (void *)&servaddr6[counter], sizeof(servaddr6[0]) );
            servaddr6[counter].sin6_family      = 10;
            servaddr6[counter].sin6_port        = htons_wrapper(p_comm_info->port);
            if(inet_pton_wrapper(10,(const char *)p_comm_info->ipv6_addr[counter].ipv6_addr,
                        (void*)&servaddr6[counter].sin6_addr)!=1)
            {
                RRC_TRACE(RRC_WARNING,"x2ap_sctp_connectx: Couldn't convert INET6 adress");
                return X2AP_SCTP_ERROR;
            }
            X2AP_TRACE(X2AP_INFO,"[%s]::Server IPV6  %s and port to connect %d \n",__FUNCTION__,
                    (p_comm_info->ipv6_addr[counter].ipv6_addr),p_comm_info->port);
        }

        else
        {
            bzero_wrapper( (void *)&servaddr[counter], sizeof(servaddr[0]) );
            servaddr[counter].sin_family      = (SA_FAMILY_T)AF_INET;
            servaddr[counter].sin_port        = htons_wrapper(p_comm_info->port);
            servaddr[counter].sin_addr.s_addr = 
                (inet_addr_wrapper((const char *)(p_comm_info->ip_addr[counter].ip_addr)) );
            X2AP_TRACE(X2AP_INFO,"[%s]::Server IP  %s and port to connect %d \n",__FUNCTION__,
                    (p_comm_info->ip_addr[counter].ip_addr),p_comm_info->port);
        }


    }

    /* Connect to the server */
    if(p_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT )
    {
        return  sctp_connectx(
                connSock,
                (struct sockaddr *)&servaddr6,
                no_enb_ip_addr );
    }
    else
    {
        return  sctp_connectx(
                connSock,
                (struct sockaddr *)&servaddr,
                no_enb_ip_addr );
    }


}

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
            U32                    stream_no)
{
    S32 x2ap_send_result = 0;
    if(socket_desc < 0)
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_sctp_send: Invalid file Descriptor");
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
        return -1;
    }

    if(NULL == p_message)
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_sctp_send: Data to be sent NULL pointer");
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_NULL_POINTER_ERR_CAUSE_ID);
        return -1;
    }
    x2ap_send_result =  sctp_sctp_sendmsg(
            socket_desc,
            p_message,
            msg_len,
            NULL,
            SCTP_PAYLOAD_PROTOCOL_ID_X2AP,
            0,
            0,
            (U16)stream_no,
            0,
            0);
    if(x2ap_send_result < 0)
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_sctp_send: sctp_sendmsg failed");
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
    }
    return x2ap_send_result;
}

/*******************************************************************************
 *   Function Name    : x2ap_sctp_receive
 *   Input            : None
 *   Output           : sender   -         Pointer to Sender Module ID.
 *                      receiver -         Pointer to Receiver Module ID.
 *                      priority           Pointer to priority.
 *   Returns          : Returns pointer the message buffer which is sent to 
 *                      Destination Module ID.
 *                      Returns NULL if no message is available
 *   Description      : This Function receives data. It is also used for handling
 *                      notifications.
 *******************************************************************************/
void* x2ap_sctp_receive(QMODULE *sender, QMODULE *receiver, S8 *priority)
{
    S8 buffer[MAX_BUFFER_SIZE] = {0};
    S8 * msg_buf = X2AP_P_NULL;

    U16 api_id = X2AP_NULL;
    U16 msg_buf_len = X2AP_NULL;
    S32 socket_desc = X2AP_NULL;
    S32 bytes_read = X2AP_NULL;
    S32 flags = X2AP_NULL;

    static U32 counter = X2AP_NULL;

    x2ap_module_id_t int_dst_id = X2AP_NULL;
    x2ap_module_id_t src_id = X2AP_NULL;
    sctp_assoc_change_st * p_sn_assoc_change = X2AP_P_NULL;
    sctp_paddr_change_st  * p_sn_padr_change  = X2AP_P_NULL;

    struct timeval  zero;
    /* Coverity Fix 88305 Start */
    struct sctp_sndrcvinfo sndrcvinfo = {X2AP_NULL};
     /* Coverity Fix 88305 End */
    struct sockaddr from_addr;

    sctp_fd_set_st local_sctp_fd_set;
    socklen_t from_addr_len = 0;

    union sctp_notification *p_notification = X2AP_P_NULL;

      /* Dispatch SCTP stack packets */
    {
        sig_q_shell_st *p_shell = X2AP_P_NULL;
        QMODULE     from, to;
        signed char new_priority;
        QSHELL *S = X2AP_P_NULL;
        sig_q_time_st q_time;

        q_time.s = 0;
        q_time.us = 0;
        sig_get_qshell(&p_shell);
        S = p_shell;
         S->timedwait(X2AP_P_NULL,&q_time);
        S->receive(X2AP_P_NULL, &from, &to, &new_priority);

    }


    /* Notification message types */


    SCTP_FD_ZERO(&local_sctp_fd_set);


    if (!g_sctp_sd_count)
    {
        return 0;
    }

    local_sctp_fd_set = g_sctp_fd_set;

    zero.tv_sec = 0;
    zero.tv_usec = 0;

    if (X2AP_SCTP_ERROR == 
            sctp_select(SCTP_FD_SETSIZE - 1, &local_sctp_fd_set, 0, 0, &zero))
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_sctp_receive: Problem with select error code %d",sk_errno);
        ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
        return 0;
    }


    /* check which socket descriptor is active*/
    for(counter = 0 ; counter < g_sctp_sd_count; counter++)
    {
        if ( (INVALID_SCTP_SOCKET != g_sctp_sd_list[counter]) &&
                SCTP_FD_ISSET(g_sctp_sd_list[counter], &local_sctp_fd_set))
        {
            socket_desc = g_sctp_sd_list[counter];
            X2AP_TRACE(X2AP_INFO,"SD is set for sctp_sd %d \n",socket_desc);
            break;
        }
    }

    if (counter == g_sctp_sd_count)
    {
        counter =0;
        return 0;
    }

    /*
     * To check for pending connections on server
     */
    /* SPR 20633 Changes Start */
    if (socket_desc == g_sctp_server_fd)
    /* SPR 20633 changes end */
    {
        X2AP_TRACE(X2AP_INFO,"SCTP: Activity on server socket\n");
        api_id = X2AP_SCTP_PEND_CONN_IND;
    }
    else
    {
        if(socket_desc < 0)
        {
            X2AP_TRACE(X2AP_FATAL, "x2ap_sctp_receive: Invalid file Descriptor");
            ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
            return 0;
        }
        /* Reading message on the active socket */
        if ( X2AP_SCTP_ERROR == (bytes_read = sctp_sctp_recvmsg(

                        socket_desc,
                        (void *)buffer,
                        sizeof(buffer),
                        &from_addr,
                        &from_addr_len,
                        (struct sctp_sndrcvinfo *)&sndrcvinfo,
                        (int *)(&flags) )))

        {
            X2AP_TRACE(X2AP_FATAL,
                    "sctp_sctp_recvmsg returned failure %d flags 0x%x !!\n",
                    bytes_read,flags);
            ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
            return 0;
        }

        /* Checking whether it is a message or a notification */


        X2AP_TRACE(X2AP_INFO,
                "sctp_sctp_recvmsg returned %d successfully, flags 0x%x !!\n",
                bytes_read,flags);

        if(flags & SCTP_MSG_NOTIFICATION)
        {
            p_notification = (sctp_notification_ut *)buffer;

            switch (p_notification->sn_header.sn_type)
            {
                case SCTP_PEER_ADDR_CHANGE:
                    {
                        X2AP_TRACE(X2AP_INFO,"Notification message of type "
                                "SCTP_PEER_ADDR_CHANGE for sd %d\n",
                                socket_desc);

                        p_sn_padr_change = 
                            &(p_notification->sn_padr_change);

                        switch (p_sn_padr_change->spc_state)
                        {
                            case SCTP_ADDRESS_UNREACHABLE:
                                {
                                    X2AP_TRACE(X2AP_INFO,
                                            "Address Unreachable for sd %d\n",
                                            socket_desc);
                                    break;
                                }
                            case SCTP_ADDRESS_AVAILABLE:
                                {
                                    X2AP_TRACE(X2AP_INFO,
                                            "Peer is available again!\n");
                                    break;
                                }

                            default:
                                {
                                    X2AP_TRACE(X2AP_INFO,"Unknown!!!!!!!\n");
                                    return 0;
                                }
                        }

                        break;

                    }
                case SCTP_ASSOC_CHANGE:
                    {

                        X2AP_TRACE(X2AP_INFO,"Notification message of type "
                                "SCTP_ASSOC_CHANGE for sd %d\n",
                                socket_desc);

                        p_sn_assoc_change = &(p_notification->
                                sn_assoc_change);

                        switch(p_sn_assoc_change->sac_state)
                        {
                            case SCTP_COMMUNICATION_UP:
                                {
                                    X2AP_TRACE(X2AP_INFO,
                                            "SCTP: A new association has just been started");
                                    api_id = X2AP_SCTP_COMM_UP_IND;
                                    break;;
                                }
                            case SCTP_COMMUNICATION_LOST:
                                {
                                    X2AP_TRACE(X2AP_INFO,
                                            "SCTP: An SCTP association " 
                                            " has just been lost for sd %d",
                                            socket_desc);
                                    api_id = X2AP_SCTP_CONN_FAILURE_IND;
                                    break;
                                }
                            case SCTP_RESTART:
                                {
                                    X2AP_TRACE(X2AP_INFO,
                                            "SCTP: Peer associated with the sd %d"
                                            " has been restarted ",
                                            socket_desc);
                                    api_id = X2AP_SCTP_CONN_RESTART_IND;
                                    break;
                                }
                            case SCTP_SHUTDOWN_COMPLETE:
                                {
                                    X2AP_TRACE(X2AP_INFO,
                                            "SCTP: Shutdown completed with the "
                                            "peer entityassociated with the sd %d",
                                            socket_desc);
                                    api_id = X2AP_SCTP_CLOSED_IND;
                                    break;
                                }
                            case SCTP_CANT_START_ASSOC:
                                {
                                    X2AP_TRACE(X2AP_INFO,
                                            "SCTP: The peer did not respond to"
                                            " an association setup attempt");
                                    api_id = X2AP_SCTP_CONN_FAILURE_IND;
                                    break;
                                }

                                /* Used only in HSS specific interface */
                            case SCTP_ENDPOINT_DOWN_RETRIEVABLE:
                                {
                                    X2AP_TRACE(X2AP_INFO,
                                            "SCTP: SCTP_ENDPOINT_DOWN_RETRIEVABLE ");
                                    return 0;
                                }

                            case SCTP_ENDPOINT_ADDR_CHNG:
                                {
                                    X2AP_TRACE(X2AP_INFO,
                                            "SCTP: SCTP_ENDPOINT_ADDR_CHNG");
                                    return 0;
                                }
                            default:
                                {
                                    X2AP_TRACE(X2AP_WARNING,
                                            "Unknown State for received Notification");
                                    return 0;
                                }
                        }/* End of switch */
                        break;
                    }
                case SCTP_REMOTE_ERROR:
                    {
                        X2AP_TRACE(X2AP_WARNING,"SCTP: SCTP REMOTE ERROR");
                        return 0;
                    }

                case SCTP_SEND_FAILED:
                    {
                        X2AP_TRACE(X2AP_WARNING,"SCTP: SCTP SEND FAILED");
                        return 0;

                    }
                case SCTP_SHUTDOWN_EVENT:
                    {
                        X2AP_TRACE(X2AP_WARNING,
                                "SCTP: Peer entity associated with sd %d"\
                                " has initiated SHUTDOWN", socket_desc);
                        api_id = X2AP_SCTP_SHUT_DOWN_INIT_IND;
                        break;
                    }
                case SCTP_ADAPTION_EVENT:
                    {
                        X2AP_TRACE(X2AP_WARNING,"SCTP: SCTP ADAPTION EVENT ");
                        return 0;
                    }
                case SCTP_PARTIAL_DELIVERY:
                    {
                        X2AP_TRACE(X2AP_WARNING,"SCTP PARTIAL DELIVERY");
                        return 0;
                    }


                default:
                    {
                        X2AP_TRACE(X2AP_INFO,"Unknown Notification!!!!!!!\n");
                        return 0;
                    }
            }
        }
        else
        {
            api_id = X2AP_ENB_MESSAGE;
        }
    }

    /* SPR_14436_Fix Start */
    /* Computing total length */
    msg_buf_len = (U16)(RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE + X2AP_SCTP_STREAM_ID_SIZE + bytes_read);
    /* SPR_14436_Fix Stop */
    msg_buf = x2ap_msg_mem_get(msg_buf_len);

    if (msg_buf == X2AP_P_NULL)
    {
        /* Not enough memory */
        X2AP_TRACE(X2AP_ERROR,"Memory Allocation failed in sctp_receive\n");
        return X2AP_FAILURE;
    }

    x2ap_set_sctp_sd(
            (U32*) (msg_buf + RRC_API_HEADER_SIZE) ,(U32)socket_desc);
    /* SPR_14436_Fix Start */
    x2ap_set_sctp_stream(
            (U16*) (msg_buf + RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE ) ,
            sndrcvinfo.sinfo_stream);


    if(bytes_read != 0)
    {
        X2AP_MEMCPY(
                (msg_buf + RRC_API_HEADER_SIZE + SCTP_SD_OCTET_SIZE + X2AP_SCTP_STREAM_ID_SIZE),
                buffer,
                bytes_read);      /* copying received ASN message */
    }
    /* SPR_14436_Fix Stop */

    src_id = RRC_X2AP_PEER_eNODEB_MODULE_ID;
    int_dst_id = RRC_X2AP_MODULE_ID;

    /* Fill the X2AP API header */
    x2ap_construct_api_header(
            (U8 *)msg_buf,
            X2AP_VERSION_ID,
            src_id,
            int_dst_id,
            api_id,
            msg_buf_len);


    *sender         = qvGetService(src_id);
    *receiver       = qvGetService(int_dst_id);
    *priority       = 0;

    X2AP_ASSERT(0 != *sender);
    X2AP_ASSERT(0 != *receiver);
    X2AP_TRACE(X2AP_INFO,"Message is sent to X2AP module\n");
    return msg_buf;
}

/******************************************************************************
 *   Function Name    : x2ap_close_association
 *   INPUT            : socket_desc  -   Socket Descriptor of the point
 *   Output           : None.
 *   Returns          : Returns 0 on success or -1 on Error.
 *   Description      : This Function performs the graceful shutdown.
 ******************************************************************************/
x2ap_return_et x2ap_close_association( sctp_sd_t socket_desc)
{

    U32 counter = X2AP_NULL;
    if (X2AP_SCTP_ERROR == sctp_close(socket_desc))
    {
        X2AP_TRACE(X2AP_WARNING,"Problem in closing socket_desc\n");
        return X2AP_FAILURE;
    }
    SCTP_FD_CLR(socket_desc,&g_sctp_fd_set);

    /* L3-X2AP Integration Activity */
    SCTP_FD_CLR(socket_desc,&sctp_fd);
    
    for(counter = 0; counter < g_sctp_sd_count;counter++)
    {
        if (socket_desc == g_sctp_sd_list[counter])
        {
            g_sctp_sd_list[counter] = g_sctp_sd_list[g_sctp_sd_count - 1];
            g_sctp_sd_list[g_sctp_sd_count - 1 ] = INVALID_SCTP_SOCKET;
            g_sctp_sd_count--;
            break;
        }
    }

    /* Decrementing count for total connection */
    X2AP_TRACE(X2AP_INFO,"SCTP : g_sctp_sd_count decremented to %d "
            " and the sd closed is %d\n",
            g_sctp_sd_count,socket_desc);
    return X2AP_SUCCESS;
}



/******************************************************************************
 *   Function Name    : x2ap_close_socket
 *   INPUT            : socket_desc  -   Socket Descriptor of the point
 *   Output           : None.
 *   Returns          : Returns 0 on success or -1 on Error.
 *   Description      : This Function closes the socket_desc descriptor.
 ******************************************************************************/
x2ap_return_et x2ap_close_socket(S32 socket_desc)
{
    if (X2AP_SCTP_ERROR == sctp_close(socket_desc))
    {
        X2AP_TRACE(X2AP_WARNING,"Problem in closing socket_desc\n");
        return X2AP_FAILURE;
    }
    return X2AP_SUCCESS;
}

/******************************************************************************
 *   Function Name     : x2ap_sctp_init_stack
 * Input         : cspl_already_intialised - a check for SCTP inialization 
 *                 p_comm_info             - pointer to x2_enb_comm_info_t
 *   Output            : None.
 *   Returns           : Returns 0 on Error and 1 on Success.
 * Description     : This Function Initialises the Aricent's SCTP stack.
 ******************************************************************************/

U32 x2ap_sctp_init_stack(
            U8 cspl_already_intialised,
            x2_enb_comm_info_t          *p_comm_info)
{

    sctp_error_t                 sctpErrorCode = E_NOERROR;
    U32 counter = 0;
    sctp_init_options_st         sctpInitOptions;
    U8 no_of_ip_addr = RRC_NULL;
    struct in6_addr             ip6_addr;
/* SPR 20633 changes start */
        if (X2AP_TRUE == init_stack_flag_v6 && X2AP_TRUE == init_stack_flag_v4)
/* SPR 20633 changes end */
    {
        return SCTP_SUCCESS;
    }
    SCTP_FD_ZERO(&g_sctp_fd_set);


    if ((RRC_FALSE == is_sctp_stck_env_set) && 
            (SCTP_FAILURE == 
             sctp_set_stack_envt((sctp_Boolean_t)cspl_already_intialised, 
                 X2AP_P_NULL)))
    {
        X2AP_TRACE(X2AP_WARNING," SCTP Set Env Return Failure\n");
        return SCTP_FAILURE;
    }
    else
    {
        is_sctp_stck_env_set = RRC_TRUE;
    }

    X2AP_TRACE(X2AP_INFO," SCTP Set Env set\n");
    /* Setting all the Init options */
    sctpInitOptions.capability.ecn_capable            = SCTP_FALSE; 
    sctpInitOptions.capability.pmtu_capable           = SCTP_FALSE;
    sctpInitOptions.capability.nat_capable            = SCTP_FALSE;
    sctpInitOptions.capability.ack_policy             = 2;
    sctpInitOptions.capability.stale_cookie_policy    = 0;
    /* SPR 20633 changes start */
    if((p_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_NUM_ADDR_PRESENT) && (init_stack_flag_v6 == RRC_FALSE))
    {
        sctpInitOptions.capability.stack_type             = SCTP_OVER_INET6;
        no_of_ip_addr = p_comm_info->num_ipv6_addr;
    }
    else
    {
        sctpInitOptions.capability.stack_type             = SCTP_OVER_INET;
        no_of_ip_addr = p_comm_info->num_ip_addr;
    }
    /* SPR 20633 changes end */
    sctpInitOptions.max_appl                          = 10;
    sctpInitOptions.max_endpoint                      = 190;
    sctpInitOptions.max_tcb                           = 100;
    sctpInitOptions.rand_seed                         = 1;
    sctpInitOptions.no_of_local_addrs                 = no_of_ip_addr;

    for (counter = 0; counter < no_of_ip_addr; counter++)
    {
        if(p_comm_info->bitmask & X2_ENB_COMM_INFO_IPV6_IP_ADDR_PRESENT && (init_stack_flag_v6 == RRC_FALSE))
        {
            sctpInitOptions.list_of_local_addrs[counter].family = 10;
            X2AP_MEMSET(&sctpInitOptions.list_of_local_addrs[counter].ip.v6.addr,0,16);
            X2AP_TRACE(X2AP_INFO,"IP addr : %s\n",p_comm_info->ipv6_addr[counter].ipv6_addr);
            X2AP_TRACE(X2AP_INFO,"Number of IP Addr : %d\n",counter+1);
            if (inet_pton_wrapper(10,(const char *) p_comm_info->ipv6_addr[counter].ipv6_addr,
                        (void*)&ip6_addr) != 1)
            {
                RRC_TRACE(RRC_WARNING,"x2ap_sctp_init_stack:Couldnot convert the "
                        "INET6 address");
                return (SCTP_FAILURE);
            }
            sctpInitOptions.list_of_local_addrs[counter].ip.v6.addr.in6_u.u6_addr32[0] = ntohl_wrapper(ip6_addr.s6_addr32[0]);
            sctpInitOptions.list_of_local_addrs[counter].ip.v6.addr.in6_u.u6_addr32[1] = ntohl_wrapper(ip6_addr.s6_addr32[1]);
            sctpInitOptions.list_of_local_addrs[counter].ip.v6.addr.in6_u.u6_addr32[2] = ntohl_wrapper(ip6_addr.s6_addr32[2]);
            sctpInitOptions.list_of_local_addrs[counter].ip.v6.addr.in6_u.u6_addr32[3] = ntohl_wrapper(ip6_addr.s6_addr32[3]);
        }
        else if(p_comm_info->bitmask & X2_ENB_COMM_INFO_IPV4_IP_ADDR_PRESENT)
        {
            sctpInitOptions.list_of_local_addrs[counter].family = SCTP_AF_INET;
            X2AP_MEMSET(&sctpInitOptions.list_of_local_addrs[counter].ip.v4.addr,0,4);
            X2AP_TRACE(X2AP_INFO,"IP addr : %s\n",p_comm_info->ip_addr[counter].ip_addr);
            X2AP_TRACE(X2AP_INFO,"Number of IP Addr : %d\n",counter+1);
            sctpInitOptions.list_of_local_addrs[counter].ip.v4.addr =
                htonl_wrapper(inet_addr_wrapper((const char *)(p_comm_info->ip_addr[counter].ip_addr)));
        }
        /* SPR 20633 changes end */
    }

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
        return SCTP_SUCCESS;
    }
    else
    {
        return SCTP_FAILURE;
    }
}


/******************************************************************************
 *   Function Name    : x2ap_sctp_accept
 *   INPUT            : socket_desc  -   Socket Descriptor of the point
 *                 pp_peerAddrs -   pointer to sockaddr
 *                 p_addr_count - pointer to ddr counts
 *   Output           : None.
 * Returns       : sock fd
 * Description   : This Function accepts a new socket_desc.
 ******************************************************************************/
S32 x2ap_sctp_accept(
        S32 socket_desc,
        struct sockaddr ** pp_peerAddrs, 
        U32 * p_addr_count,
/*spr8341 IPv6 fix start*/        
        struct sockaddr_in6   **pp_peerAddr6,
        U8 addr_family)
/*spr8341 IPv6 fix end*/        
{
    S32 connSock = INVALID_SCTP_SOCKET;
    struct sockaddr * p_addr = X2AP_P_NULL; /*this is dynamically allocated by sctp stack*/
    S32 blocked = 1;
    /*spr8341 IPv6 fix start*/        
    struct sockaddr_in6   *p_addr6 = X2AP_P_NULL;
     /*spr8341 IPv6 fix start*/        
    /* Bug 908 Fix Start */
    U32 noDelay = 1;
    /* Bug 908 Fix Stop */
    connSock = sctp_accept(socket_desc, X2AP_P_NULL, X2AP_P_NULL);

    if (X2AP_SCTP_ERROR == connSock)
    {
        X2AP_TRACE(X2AP_WARNING,"Problem in accepting on socket_desc\n");
        return INVALID_SCTP_SOCKET;
    }
    if (sctp_setsockopt(connSock, SOL_SOCKET,
                SCTP_SO_NONBLOCKING, &blocked, sizeof(U32)) != -1) 
    {
        SCTP_FD_SET(connSock, &(g_sctp_fd_set));
        g_sctp_sd_list[g_sctp_sd_count] = connSock;
        g_sctp_sd_count++;

        /* L3-X2AP Integration Activity */
        SCTP_FD_SET(connSock, &(sctp_fd));

        /*spr8341 IPv6 fix start*/        
        if(AF_INET == addr_family)
        {
            /*NOTE: Aricent's stack does not return -1 for this API */ 
            *p_addr_count = sctp_getpaddrs(connSock, X2AP_NULL, &p_addr);
            *pp_peerAddrs = rrc_mem_get(sizeof(struct sockaddr) * (*p_addr_count)) ;
            if(X2AP_P_NULL != *pp_peerAddrs)
            {
                X2AP_MEMCPY(*pp_peerAddrs, p_addr, sizeof(struct sockaddr) * (*p_addr_count) );
            }
            else
            {
                sctp_freepaddrs(p_addr);
                return INVALID_SCTP_SOCKET;
            }

            sctp_freepaddrs(p_addr);
        }
        else if(AF_INET6 == addr_family)
        {
            *p_addr_count = sctp_getpaddrs(connSock, X2AP_NULL, (struct sockaddr **)&p_addr6);
            *pp_peerAddr6 = rrc_mem_get(sizeof(struct sockaddr_in6) * (*p_addr_count)) ;
            if(X2AP_P_NULL != *pp_peerAddr6)
            {
                X2AP_MEMCPY(*pp_peerAddr6, p_addr6, sizeof(struct sockaddr_in6) * (*p_addr_count) );
            }
            else
            {
                X2AP_TRACE(X2AP_WARNING,"x2ap_sctp_accept pp_peerAddrs is NULL \n");
                sctp_freepaddrs((struct sockaddr *)p_addr6);
                return INVALID_SCTP_SOCKET;
            }
            sctp_freepaddrs((struct sockaddr *)p_addr6);
        }
        else
        {
            X2AP_TRACE(X2AP_WARNING,
                    "Invalid address family :%d\n",addr_family);
            return INVALID_SCTP_SOCKET;
        }
        /*spr8341 IPv6 fix end*/        
    }
    else
    {
        return INVALID_SCTP_SOCKET;
    }
    
    /* Bug 908 Fix Start */
    if (0 > sctp_setsockopt( connSock, IPPROTO_SCTP,
                SCTP_SO_NODELAY, &noDelay, sizeof(noDelay)))
    {
        X2AP_TRACE(X2AP_WARNING,
                "SCTP : sctp_setsockopt for setting No-Delay failed\n");
        if( sctp_close(connSock) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Peer socket closed is: %d", connSock);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Peer socket close failure, errno: %d",sk_errno);
        }
        return INVALID_SCTP_SOCKET;
    }
    /* Bug 908 Fix Stop */

    return connSock;
}
#endif	
#endif
