/*******************************************************************************
*
*  FILE NAME   : sctp_kernel.c
*
*  DESCRIPTION : This file contains the definition of functions which will be
*                used to handle the sages from/to the linux based SCTP stack 
*                interface.
*
*  DATE 24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#if 0
#ifdef RRC_UNIT_TEST_FRAMEWORK
#include "rrc_ut_stub_db.h"
#endif
#endif

#ifdef  LKSCTP

#include "sctp_kernel.h"
#include "x2ap_utils.h"
#include "rrc_msg_mgmt.h"
#include "rrc_common_utils.h"
#include <lteMisc.h>
/****************************************************************************
 * Private Variables
 ****************************************************************************/

/* Maintained for sctp sockets*/

/* Bug-4680 FIX START */
/* 1-server_fd and 32-client_fds(for peer eNodeB's) */
sctp_sd_t g_sctp_sd_list[MAX_PEER_ENB+1] = {};
/* Bug-4680 FIX END */

    /*SPR_16533_START*/
        /* Code Removed */
    /*SPR_16533_END*/

/*****************************************************************************
 * Function Name  : x2ap_open_sctp_server_setsockopt_wrapper 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_server_info - pointer to x2ap_sctp_conn_info_t
 *                  num_peer_enb - peer eNB num 
 * Outputs        : None
 * Returns        : Signed int 
 * Description    : This function opens a socket for sctp server  
 ********************************************************************************/
S32 x2ap_open_sctp_server_setsockopt_wrapper(
        S32 server_sctp_sd,
        x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info,
        /* dscp_marking_start */
        U8 ipv6_flag)     
        /* dscp_marking_stop */
{
    /*dscp_marking_start*/
    U8 tos = RRC_NULL;
    U32 tclass = RRC_NULL;
    U32 on = 1; //For IPv6
    /*dscp_marking_stop*/
    /*L3-X2AP Drop2 Integration */ 
    /* SPR 16146 Start */
    S32 fl = fcntl_wrapper(server_sctp_sd, F_GETFL, 0);
    /* Coverity Fix 61207 Start */
    if (-1 == fl)
    {
         X2AP_TRACE(X2AP_DETAILED, " Failure returned by the function fcntl_wrapper");
    }
    /* Coverity Fix 61207 End */
    /* SPR 16146 Stop */
    /* coverity 86867 */
    if ((SCTP_SOCKET_ERROR != fl) || fcntl_wrapper(server_sctp_sd, F_SETFL, fl | O_NONBLOCK) != SCTP_SOCKET_ERROR)
    {
        if (setsockopt_wrapper(server_sctp_sd, IPPROTO_SCTP,
                    SCTP_INITMSG, &(p_sctp_assoc_conf_info->init_msg), 
                    sizeof(p_sctp_assoc_conf_info->init_msg) ) == 
                SCTP_SOCKET_ERROR) 
        {
            /*unable to set socket options thus deleting created socket*/
            X2AP_TRACE(X2AP_INFO," Server socket property SCTP_INITMSG not set" 
                    "[errno:%d]", errno);
            if( close_wrapper(server_sctp_sd) == 0 ) 
            {
                /* association closed successfully */
                /* Trace the successful closure of socket */
                X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            }
            else 
            {
                /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
                X2AP_TRACE(X2AP_WARNING,
                        " Server socket close failure, errno: %d", errno);
            }

            return SCTP_SOCKET_ERROR;
        }

        X2AP_TRACE(X2AP_INFO,"SCTP : setsockopt for INIT success: instream=%d, iostream=%d, max_attempts=%d, max_init_timeo=%d",
            p_sctp_assoc_conf_info->init_msg.sinit_max_instreams,  p_sctp_assoc_conf_info->init_msg.sinit_num_ostreams,
             p_sctp_assoc_conf_info->init_msg.sinit_max_attempts,  p_sctp_assoc_conf_info->init_msg.sinit_max_init_timeo);
        
        /* sctp parameters start */
        if (setsockopt_wrapper(server_sctp_sd, IPPROTO_SCTP,
                   SCTP_ASSOCINFO, &(p_sctp_assoc_conf_info->assocparams), 
                    sizeof(p_sctp_assoc_conf_info->assocparams) ) == 
                SCTP_SOCKET_ERROR) 
        {
            /*unable to set socket options thus deleting created socket*/
            X2AP_TRACE(X2AP_INFO," Server socket property SCTP_ASSOCINFO not set" 
                    "[errno:%d]", errno);
            if( close_wrapper(server_sctp_sd) == 0 ) 
            {
                /* association closed successfully */
                /* Trace the successful closure of socket */
                X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            }
            else 
            {
                /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
                X2AP_TRACE(X2AP_WARNING,
                        " Server socket close failure, errno: %d", errno);
            }

            return SCTP_SOCKET_ERROR;
        }
        if (setsockopt_wrapper(server_sctp_sd, IPPROTO_SCTP,
                   SCTP_RTOINFO, &(p_sctp_assoc_conf_info->rtoinfo), 
                    sizeof(p_sctp_assoc_conf_info->rtoinfo) ) == 
                SCTP_SOCKET_ERROR) 
        {
            /*unable to set socket options thus deleting created socket*/
            X2AP_TRACE(X2AP_INFO," Server socket property SCTP_RTOINFO not set" 
                    "[errno:%d]", errno);
            if( close_wrapper(server_sctp_sd) == 0 ) 
            {
                /* association closed successfully */
                /* Trace the successful closure of socket */
                X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            }
            else 
            {
                /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
                X2AP_TRACE(X2AP_WARNING,
                        " Server socket close failure, errno: %d", errno);
            }

            return SCTP_SOCKET_ERROR;
        }
  
        X2AP_TRACE(X2AP_INFO,"SCTP : setsockopt for RTO success: associd=%d, initial_rto=%d, max_rto=%d, min_rto=%d",
            p_sctp_assoc_conf_info->rtoinfo.srto_assoc_id,  p_sctp_assoc_conf_info->rtoinfo.srto_initial,
             p_sctp_assoc_conf_info->rtoinfo.srto_max,  p_sctp_assoc_conf_info->rtoinfo.srto_min);
            
        /*dscp_marking_start*/
        if( X2AP_DSCP_VALUE_PRESENT & p_sctp_assoc_conf_info->bitmask)
        {
            if (1 == ipv6_flag)
            {
                if (0 > setsockopt_wrapper(server_sctp_sd, IPPROTO_IPV6, IPV6_RECVTCLASS, &on, sizeof(on)))
                {
                    RRC_TRACE(RRC_WARNING,"  Error in option RECVTCLASS errno = %d, %s\n",errno,strerror(errno));
                }
                tclass = p_sctp_assoc_conf_info->x2ap_dscp_value << 2;
                if (setsockopt_wrapper(server_sctp_sd, IPPROTO_IPV6,
                            IPV6_TCLASS, &tclass, 
                            sizeof(tclass)) == 
                            SCTP_SOCKET_ERROR) 
                {
                    /*unable to set socket options thus deleting created socket*/
                    X2AP_TRACE(X2AP_INFO," Server socket property DSCP Value  not set for IPv6" 
                            "[errno:%d]", errno);
                    if( close_wrapper(server_sctp_sd) == 0 ) 
                    {
                        /* association closed successfully */
                        /* Trace the successful closure of socket */
                        X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
                    }
                    else 
                    {
                        /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
                        X2AP_TRACE(X2AP_WARNING,
                               " Server socket close failure, errno: %d", errno);
                    }

                    return SCTP_SOCKET_ERROR;
                }
                else 
                {
                    /* BUG 11440 Fix Start */
                    X2AP_TRACE(X2AP_DETAILED,
                            " DSCP Value set for IPv6: %d",p_sctp_assoc_conf_info->x2ap_dscp_value);
                    /* BUG 11440 Fix Stop */
                }
            }
            else
            {
                /*Bug 9804 Fix +*/
                tos = p_sctp_assoc_conf_info->x2ap_dscp_value << 2;
                if (setsockopt_wrapper(server_sctp_sd, IPPROTO_IP,
                            IP_TOS, &tos, 
                            sizeof(tos) ) == 
                        SCTP_SOCKET_ERROR) 
                {
                    /*unable to set socket options thus deleting created socket*/
                    X2AP_TRACE(X2AP_INFO," Server socket property DSCP Value  not set" 
                            "[errno:%d]", errno);
                    if( close_wrapper(server_sctp_sd) == 0 ) 
                    {
                        /* association closed successfully */
                        /* Trace the successful closure of socket */
                        X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
                    }
                    else 
                    {
                        /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
                        X2AP_TRACE(X2AP_WARNING,
                                " Server socket close failure, errno: %d", errno);
                    }

                    return SCTP_SOCKET_ERROR;
                }
                else 
                {
                    /* BUG 11440 Fix Start */
                    X2AP_TRACE(X2AP_DETAILED,
                            " DSCP Value set: %d",p_sctp_assoc_conf_info->x2ap_dscp_value);
                    /* BUG 11440 Fix Stop */
                }
                /*Bug 9804 Fix -*/
            }
        }
        /*dscp_marking_stop*/

        /* sctp parameters stop */
    }
    /* coverity 86867 */
    else
    {
        X2AP_TRACE(X2AP_INFO," Server socket property SOCK_NONBLOCK not set" 
                " [errno:%d]", errno);
        /*unable to set socket options thus deleting created socket*/
        if( close_wrapper(server_sctp_sd) == 0 ) 
        {
            /* association closed successfully */
            /* Trace the successful closure of socket */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d", errno);
        }

        return SCTP_SOCKET_ERROR;
    }
    return server_sctp_sd;
}
/* SPR 20633 changes start */
/*****************************************************************************
 * Function Name  : x2ap_open_sctp_server 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_server_info - pointer to x2ap_sctp_conn_info_t
 *                  num_peer_enb - peer eNB num 
 * Outputs        : None
 * Returns        : Signed int 
 * Description    : This function opens a socket for sctp server  
 ********************************************************************************/
S32 x2ap_open_sctp_server(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_sctp_conn_info_t *p_server_info,
        U32 num_peer_enb)
{
    U32    numIpAddress = 1; /* Value is kept 1 as for the first Ipaddress bind
                                is done and for every other successive 
                                IpAddress bindx is performend*/

    U32 optVal =1;
    U16 peer_enb_count = X2AP_NULL;
    struct sctp_event_subscribe events;
    U32 addr_family;         /* This variable is taken for facilitation of IPv6
                                enhancement */
    U8 num_of_ip_addrs = RRC_NULL;                            
    U16 addr_len       = RRC_NULL;
    /* dscp_marking_start */
    U8 ipv6_flag       = RRC_NULL;     
    /* dscp_marking_stop */
    struct sockaddr_in6 selfENBServerAddr6={0};
    /*sctp parameters start*/
    struct timeval start_time;
    /*sctp parameters stop */
    /* Bug 908 Fix Start */
    U32 noDelay = 1;
    /* Bug 908 Fix Stop */
    /* This denotes the sock addr 
       structure usd as param in bind call */

    x2_enb_comm_info_t *p_comm_info = p_server_info->p_comm_info;
    S32 server_sctp_sd = INVALID_SCTP_SOCKET;
    x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info = 
        p_server_info->p_sctp_assoc_conf_info;

    /* SPR_14436_Fix Start */
    U32 nostreams = X2AP_NULL;
    U32 nistreams = X2AP_NULL;

    nostreams = p_x2ap_gb_ctx->config_param.sctp_conf_param.init_num_ostreams;
    nistreams = p_x2ap_gb_ctx->config_param.sctp_conf_param.init_max_instreams;

    struct sctp_initmsg initmsg;
    S32 return_value  = RRC_NULL;
    /* SPR_14436_Fix Stop */
    addr_family = 10;
    num_of_ip_addrs = p_comm_info->num_ipv6_addr;
    addr_len = sizeof(struct sockaddr_in6);
    /* dscp_marking_start */
    ipv6_flag = 1;     
    /* dscp_marking_stop */
    if (X2AP_FALSE == init_stack_flag)
    {
        X2AP_TRACE (X2AP_INFO," Initializing sctp_sd_list..\n");
        /*Initiating sctp_sd_list*/
        for (peer_enb_count = 0 ; peer_enb_count < MAX_PEER_ENB ; peer_enb_count++ )
        {
            g_sctp_sd_list[peer_enb_count] = INVALID_SCTP_SOCKET; 
        }
    }


    /*check that atleast one ip address needs to be configured by operator*/
    if( num_of_ip_addrs <= 0 ) 
    {
        X2AP_TRACE(X2AP_INFO,"p_comm_info->num_ip_addr[%d] <= 0",
                p_comm_info->num_ip_addr);
        return SCTP_SOCKET_ERROR;
    }

    selfENBServerAddr6.sin6_family = 10;
    selfENBServerAddr6.sin6_port = htons_wrapper(p_comm_info->port);
    if(inet_pton_wrapper(10,(const char *)p_comm_info->ipv6_addr[0].ipv6_addr,
                (void*)&selfENBServerAddr6.sin6_addr)!=1)
    {
        RRC_TRACE(RRC_WARNING,"x2ap_open_sctp_server: Couldn't convert INET6 adress");
        return X2AP_SCTP_ERROR;
    }


    X2AP_TRACE(X2AP_INFO,"[%s]::IPV6  %s and port  %d \n",__FUNCTION__,
            p_comm_info->ipv6_addr[0].ipv6_addr,p_comm_info->port);

    X2AP_TRACE(X2AP_INFO,"[%s]::X2AP Server IPV6:%s and port:%d \n",__FUNCTION__,
            (p_comm_info->ipv6_addr[0].ipv6_addr),p_comm_info->port);

    /*Create a server socket_desc for SCTP to run on*/
    /*SPR_16533_START*/
    if( (server_sctp_sd = socket_wrapper(addr_family, SOCK_SEQPACKET, IPPROTO_SCTP)) == 
    /*SPR_16533_END*/
            SCTP_SOCKET_ERROR ) 
    {
        /* Error while creating socket_desc. Free the memory allocated and 
           exit the code and return. Using multiple return in this function
           to avoid more than 4 degrees of nesting */
        /* socket_desc call failed */
        X2AP_TRACE(X2AP_INFO," server socket() call failed [errno:%d]", errno);
        return INVALID_SCTP_SOCKET;
    }


    X2AP_TRACE(X2AP_INFO," [%s]::SD is set for server_sctp_sd is %d \n",
            __FUNCTION__,server_sctp_sd);
    /*sctp parameters start*/
    if ( 0 ==  gettimeofday_wrapper(&start_time, NULL))
    {
        p_x2ap_gb_ctx->sctp_start_time.tv_sec = (U32)start_time.tv_sec;
        p_x2ap_gb_ctx->sctp_start_time.tv_usec = (U32)start_time.tv_usec;
        X2AP_TRACE(X2AP_INFO,"x2ap:start_time sec = %u and usec = %u",p_x2ap_gb_ctx->sctp_start_time.tv_sec,
                p_x2ap_gb_ctx->sctp_start_time.tv_usec);
    } 
    else
    {
        X2AP_TRACE(X2AP_ERROR,"SCTP gettimeofday:  failed: Did not get start time\n");
    }
    /*sctp parameters stop*/

    /*L3-X2AP Drop2 Integration */
    if (setsockopt_wrapper(server_sctp_sd, SOL_SOCKET,
                SO_REUSEADDR, &optVal, sizeof(U32)) == SCTP_SOCKET_ERROR) 
    {
        X2AP_TRACE(X2AP_INFO," Server socket property SO_REUSEADDR not set"
                "[errno:%d]", errno);
        /*unable to setsockopt thus deleting created socket_desc*/
        if( close_wrapper(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            /* Coverity Fix 86819 Start */
            /* Code deleted */
            /* Coverity Fix 86819 end */
        }
        else 
        {
            /* failure in closing socket_desc [%d] */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d", errno);
        }

        return SCTP_SOCKET_ERROR;
    }

    /*Bind created socket_desc to SCTP Port*/
    /* if( bind(p_sctp_fd_info->server_sctp_fd, (struct sockaddr *) */
    if( bind_wrapper(server_sctp_sd, ( SockAddr *)
                &selfENBServerAddr6, addr_len) == SCTP_SOCKET_ERROR) 
    {
        /* Error while binding socket_desc. Free the memory allocated and
           exit the code and return. Using multiple return in this function
           to avoid more than 4 degrees of nesting */
        /* bind call failed for socket_desc [%d], p_sctp_fd_info->server_sctp_fd */
        X2AP_TRACE(X2AP_INFO, " Server bind failure, errno: %d", errno);
        /*unable to bind thus deleting created socket_desc*/
        if( close_wrapper(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            /* Coverity Fix 86819 Start */
            /* Code deleted */
            /* Coverity Fix 86819 End */
        }
        else 
        {
            /* failure in closing socket_desc [%d] */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d", errno);
        }

        return SCTP_SOCKET_ERROR;
    }



    /* As SCTP supports multi-homing thus more than one ipaddress can be 
       binded to a socket_desc*/

    while(numIpAddress < num_of_ip_addrs)
    {

        if(inet_pton_wrapper(10,(const char *)p_comm_info->ipv6_addr[numIpAddress].ipv6_addr,
                    (void*)&selfENBServerAddr6.sin6_addr)!=1)
        {
            RRC_TRACE(RRC_WARNING,"Couldn't convert INET6 adress");
            /* coverity_60522 start */                
            if( close_wrapper(server_sctp_sd) == 0 ) 
            {
                /*association closed successfully*/
                /* Trace the successful closure of socket_desc */
                /* p_sctp_fd_info->server_sctp_fd = -1; */
                X2AP_TRACE(X2AP_INFO,
                        "Server socket closed is: %d", server_sctp_sd);
                server_sctp_sd = INVALID_SCTP_SOCKET;
            }
            else
            {
                /* failure in closing socket_desc [%d] */
                X2AP_TRACE(X2AP_WARNING,
                        " Server socket close failure, errno: %d", errno);
            }
            /* coverity_60522 start */                
            return X2AP_SCTP_ERROR;
        }
        if( sctp_bindx(server_sctp_sd, 
                    (struct sockaddr*)&selfENBServerAddr6, (U32)1,
                    SCTP_BINDX_ADD_ADDR ) == SCTP_SOCKET_ERROR) 
        {
            /* Error while binding socket_desc. Free the memory allocated and
               exit the code and return. Using multiple return in this function
               to avoid more than 4 degrees of nesting */
            X2AP_TRACE(X2AP_INFO, " sctp_bindx failure, errno: %d", errno);
            /* failire in binding socket_desc [%d], p_sctp_fd_info->server_sctp_fd */

            if( close_wrapper(server_sctp_sd) == 0 ) 
            {
                /*association closed successfully*/
                /* Trace the successful closure of socket_desc */
                /* p_sctp_fd_info->server_sctp_fd = -1; */
                X2AP_TRACE(X2AP_INFO,
                        "Server socket closed is: %d", server_sctp_sd);
                /* Coverity Fix 86819 Start */
                /* Code Deleted */
                /* Coverity Fix 86819 End */
            }
            else 
            {
                /* failure in closing socket_desc [%d] */
                X2AP_TRACE(X2AP_WARNING,
                        " Server socket close failure, errno: %d", errno);
            }

            return SCTP_SOCKET_ERROR;
        }

        numIpAddress++;
    }
    /*
     * 
     * Set non blocking using fcntl
     */
    if (SCTP_SOCKET_ERROR  == x2ap_open_sctp_server_setsockopt_wrapper(server_sctp_sd, p_sctp_assoc_conf_info, ipv6_flag )) //dscp_marking
    {
        X2AP_TRACE(X2AP_WARNING,
                " x2ap_open_sctp_server_setsockopt_wrapper failed");
        return SCTP_SOCKET_ERROR;
    }

    if (listen_wrapper(server_sctp_sd, num_peer_enb) == SCTP_SOCKET_ERROR ) 
    {
        /* Error while listening socket_desc. Free the memory allocated and
           exit the code and return. Using multiple return in this function
           to avoid more than 4 degrees of nesting */
        /* listen failed for socket_desc [%d], p_sctp_fd_info->server_sctp_fd */
        X2AP_TRACE(X2AP_WARNING, " listen() failure, errno: %d", errno);

        /* listen call failed thus deleting created socket_desc */
        if( close_wrapper(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket_desc [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d", errno);
        }

        return SCTP_SOCKET_ERROR;
    }

#if 0   //SCTP_INITMSG already set in x2ap_open_sctp_server_setsockopt_wrapper above
    /* SPR_14436_Fix Start */
    initmsg.sinit_num_ostreams  = nostreams;
    initmsg.sinit_max_instreams = nistreams;
    /*sctp parameters start*/
    initmsg.sinit_max_init_timeo = RRC_SCTP_DEFAULT_INIT_TIMEOUT; 
    if(PNULL != p_sctp_assoc_conf_info)
    {    
        initmsg.sinit_max_attempts = p_sctp_assoc_conf_info->init_msg.sinit_max_attempts;
    }    
    else 
    {    
        initmsg.sinit_max_attempts  = RRC_SCTP_DEFAULT_MAX_INIT_RTX;
    }    
    /*sctp parameters stop*/
    return_value = setsockopt_wrapper(
            server_sctp_sd,
            IPPROTO_SCTP,
            SCTP_INITMSG,
            &initmsg,
            sizeof(initmsg));

    if (return_value < 0) 
    {    
        X2AP_TRACE(X2AP_WARNING,"SCTP : sctp_setsockopt for INIT failed\n");
        x2ap_close_socket(server_sctp_sd);
        return X2AP_SCTP_ERROR;
    }    

    X2AP_TRACE(X2AP_INFO,"SCTP : setsockopt for INIT success: nostream=%d, nistream=%d, maxtimeo=%d, maxattempt=%d",
        initmsg.sinit_num_ostreams, initmsg.sinit_max_instreams, initmsg.sinit_max_init_timeo, initmsg.sinit_max_attempts);
    /* SPR_14436_Fix Stop */
#endif

    /*L3-X2AP Drop2 Integration Start*/
    X2AP_MEMSET( (void *)&events, 0, sizeof(events) );

    /* Populating SCTP Events structure*/
    events.sctp_association_event  = 1;
    events.sctp_data_io_event      = 1;
    events.sctp_address_event      = 1;
    events.sctp_send_failure_event = 1;
    events.sctp_peer_error_event   = 1;
    events.sctp_shutdown_event     = 1;

    if (0 > setsockopt_wrapper(server_sctp_sd, IPPROTO_SCTP, SCTP_EVENTS, &events,
                sizeof (events)))
    {
        X2AP_TRACE(X2AP_WARNING,
                " SCTP : sctp_setsockopt for setting events failed\n");
        if( close_wrapper(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket_desc [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d", errno);
        }
        return SCTP_SOCKET_ERROR;
    }
    /*L3-X2AP Drop2 Integration End*/

    /* Bug 908 Fix Start */
    if (0 > setsockopt_wrapper(server_sctp_sd, IPPROTO_SCTP, SCTP_NODELAY, &noDelay,
                sizeof (noDelay)))
    {
        X2AP_TRACE(X2AP_WARNING,
                " SCTP : sctp_setsockopt for setting No-Delay failed\n");
        if( close_wrapper(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket_desc [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d", errno);
        }
        return SCTP_SOCKET_ERROR;
    }
    /* Bug 908 Fix Stop */

    /*
     * Add FD to set
     */
    lteFD_SET((S32)server_sctp_sd, &(g_sctp_fd_set));
    init_stack_flag = X2AP_TRUE;
    g_sctp_sd_list[g_sctp_sd_count] = server_sctp_sd;
    g_sctp_sd_count++;

    /*L3-X2AP Drop2 Integration */
    lteFD_SET((S32)server_sctp_sd, &(sctp_fd));

    /*
     * Store server fd in global static var
     */
    g_sctp_server_fd_ipv6 = server_sctp_sd;

    return server_sctp_sd;
}

/*****************************************************************************
 * Function Name  : x2ap_open_sctp_server_ipv4 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_server_info - pointer to x2ap_sctp_conn_info_t
 *                  num_peer_enb - peer eNB num 
 * Outputs        : None
 * Returns        : Signed int 
 * Description    : This function opens a socket for sctp server  
 ********************************************************************************/
S32 x2ap_open_sctp_server_ipv4(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_sctp_conn_info_t *p_server_info,
        U32 num_peer_enb)
{
    U32    numIpAddress = 1; /* Value is kept 1 as for the first Ipaddress bind
                                is done and for every other successive 
                                IpAddress bindx is performend*/

    U32 optVal =1;
    U16 peer_enb_count = X2AP_NULL;
    struct sctp_event_subscribe events;
    U32 addr_family;         /* This variable is taken for facilitation of IPv6
                                enhancement */
    U8 num_of_ip_addrs = RRC_NULL;                            
    U16 addr_len       = RRC_NULL;
    /* dscp_marking_start */
    U8 ipv6_flag       = RRC_NULL;     
    /* dscp_marking_stop */
    struct sockaddr_in selfENBServerAddr={0}; 
    /*sctp parameters start*/
    struct timeval start_time;
    /*sctp parameters stop */
    /* Bug 908 Fix Start */
    U32 noDelay = 1;
    /* Bug 908 Fix Stop */
    /* This denotes the sock addr 
       structure usd as param in bind call */

    x2_enb_comm_info_t *p_comm_info = p_server_info->p_comm_info;
    S32 server_sctp_sd = INVALID_SCTP_SOCKET;
    x2ap_sctp_assoc_conf_info_t *p_sctp_assoc_conf_info = 
        p_server_info->p_sctp_assoc_conf_info;

    /* SPR_14436_Fix Start */
    U32 nostreams = X2AP_NULL;
    U32 nistreams = X2AP_NULL;

    nostreams = p_x2ap_gb_ctx->config_param.sctp_conf_param.init_num_ostreams;
    nistreams = p_x2ap_gb_ctx->config_param.sctp_conf_param.init_max_instreams;

    struct sctp_initmsg initmsg;
    S32 return_value  = RRC_NULL;
    /* SPR_14436_Fix Stop */

    addr_family = AF_INET;
    num_of_ip_addrs = p_comm_info->num_ip_addr;
    addr_len = sizeof(struct sockaddr_in);

    if (X2AP_FALSE == init_stack_flag)
    {
        X2AP_TRACE (X2AP_INFO," Initializing sctp_sd_list..\n");
        /*Initiating sctp_sd_list*/
        for (peer_enb_count = 0 ; peer_enb_count < MAX_PEER_ENB ; peer_enb_count++ )
        {
            g_sctp_sd_list[peer_enb_count] = INVALID_SCTP_SOCKET; 
        }
    }


    /*check that atleast one ip address needs to be configured by operator*/
    if( num_of_ip_addrs <= 0 ) 
    {
        X2AP_TRACE(X2AP_INFO,"p_comm_info->num_ip_addr[%d] <= 0",
                p_comm_info->num_ip_addr);
        return SCTP_SOCKET_ERROR;
    }

    selfENBServerAddr.sin_family = AF_INET;
    selfENBServerAddr.sin_addr.s_addr = 
        inet_addr_wrapper((const char *)(p_comm_info->ip_addr[0].ip_addr));
    selfENBServerAddr.sin_port = htons_wrapper(p_comm_info->port);

    /*Create a server socket_desc for SCTP to run on*/
    /*SPR_16533_START*/
    if( (server_sctp_sd = socket_wrapper(addr_family, SOCK_SEQPACKET, IPPROTO_SCTP)) == 
            /*SPR_16533_END*/
            SCTP_SOCKET_ERROR ) 
    {
        /* Error while creating socket_desc. Free the memory allocated and 
           exit the code and return. Using multiple return in this function
           to avoid more than 4 degrees of nesting */
        /* socket_desc call failed */
        X2AP_TRACE(X2AP_INFO," X2AP Server IP:%s and port:%d server socket() call failed [errno:%d]",
                p_comm_info->ip_addr[0].ip_addr,p_comm_info->port, errno);
        return INVALID_SCTP_SOCKET;
    }


    X2AP_TRACE(X2AP_DETAILED," [%s]::SD is set for server_sctp_sd is %d \n",
            __FUNCTION__,server_sctp_sd);
    /*sctp parameters start*/
    if ( 0 ==  gettimeofday_wrapper(&start_time, NULL))
    {
        p_x2ap_gb_ctx->sctp_start_time.tv_sec = (U32)start_time.tv_sec;
        p_x2ap_gb_ctx->sctp_start_time.tv_usec = (U32)start_time.tv_usec;
        X2AP_TRACE(X2AP_INFO,"x2ap:start_time sec = %u and usec = %u",p_x2ap_gb_ctx->sctp_start_time.tv_sec,
                p_x2ap_gb_ctx->sctp_start_time.tv_usec);
    } 
    else
    {
        X2AP_TRACE(X2AP_ERROR,"SCTP gettimeofday:  failed: Did not get start time\n");
    }
    /*sctp parameters stop*/

    /*L3-X2AP Drop2 Integration */
    if (setsockopt_wrapper(server_sctp_sd, SOL_SOCKET,
                SO_REUSEADDR, &optVal, sizeof(U32)) == SCTP_SOCKET_ERROR) 
    {
        X2AP_TRACE(X2AP_INFO," Server socket property SO_REUSEADDR not set"
                "[errno:%d]", errno);
        /*unable to setsockopt thus deleting created socket_desc*/
        if( close_wrapper(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            /* Coverity Fix 86819 Start */
            /* Code deleted */
            /* Coverity Fix 86819 end */
        }
        else 
        {
            /* failure in closing socket_desc [%d] */
            X2AP_TRACE(X2AP_WARNING,
                    " Server socket close failure, errno: %d", errno);
        }

        return SCTP_SOCKET_ERROR;
    }

    /*Bind created socket_desc to SCTP Port*/
    /* if( bind(p_sctp_fd_info->server_sctp_fd, (struct sockaddr *) */
    if( bind_wrapper(server_sctp_sd, (SockAddr *)
                &selfENBServerAddr, addr_len) == SCTP_SOCKET_ERROR) 
    {
        /* Error while binding socket_desc. Free the memory allocated and
           exit the code and return. Using multiple return in this function
           to avoid more than 4 degrees of nesting */
        /* bind call failed for socket_desc [%d], p_sctp_fd_info->server_sctp_fd */
        X2AP_TRACE(X2AP_INFO, " Server bind failure, errno: %d", errno);
        /*unable to bind thus deleting created socket_desc*/
        if( close_wrapper(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
            /* Coverity Fix 86819 Start */
            /* Code Deleted */
            /* Coverity Fix 86819 End */

        }
        else 
        {
            /* failure in closing socket_desc [%d] */
            X2AP_TRACE(X2AP_WARNING,
                    " Server socket close failure, errno: %d", errno);
        }

        return SCTP_SOCKET_ERROR;
    }

    /* As SCTP supports multi-homing thus more than one ipaddress can be 
       binded to a socket_desc*/

    while(numIpAddress < num_of_ip_addrs)
    {
        /* check to be verified wrt null condition */
        selfENBServerAddr.sin_addr.s_addr = inet_addr_wrapper((const char *)
                (p_comm_info->ip_addr[numIpAddress].ip_addr));

        if( sctp_bindx(server_sctp_sd, 
                    (struct sockaddr*)&selfENBServerAddr, (U32)1,
                    SCTP_BINDX_ADD_ADDR ) == SCTP_SOCKET_ERROR) 
        {
            /* Error while binding socket_desc. Free the memory allocated and
               exit the code and return. Using multiple return in this function
               to avoid more than 4 degrees of nesting */
            X2AP_TRACE(X2AP_INFO, " sctp_bindx failure, errno: %d", errno);
            /* failire in binding socket_desc [%d], p_sctp_fd_info->server_sctp_fd */

            if( close_wrapper(server_sctp_sd) == 0 ) 
            {
                /*association closed successfully*/
                /* Trace the successful closure of socket_desc */
                /* p_sctp_fd_info->server_sctp_fd = -1; */
                X2AP_TRACE(X2AP_INFO,
                        "Server socket closed is: %d", server_sctp_sd);
                /* Coverity Fix 86819 Start */
                /* Code Deleted */
                /* Coverity Fix 86819 End */
            }
            else 
            {
                /* failure in closing socket_desc [%d] */
                X2AP_TRACE(X2AP_WARNING,
                        " Server socket close failure, errno: %d", errno);
            }

            return SCTP_SOCKET_ERROR;
        }
        numIpAddress++;
    }
    /*
     * 
     * Set non blocking using fcntl
     */
    if (SCTP_SOCKET_ERROR  == x2ap_open_sctp_server_setsockopt_wrapper(server_sctp_sd, p_sctp_assoc_conf_info, ipv6_flag )) //dscp_marking
    {
        X2AP_TRACE(X2AP_WARNING,
                " x2ap_open_sctp_server_setsockopt_wrapper failed");
        return SCTP_SOCKET_ERROR;
    }

    if (listen_wrapper(server_sctp_sd, num_peer_enb) == SCTP_SOCKET_ERROR ) 
    {
        /* Error while listening socket_desc. Free the memory allocated and
           exit the code and return. Using multiple return in this function
           to avoid more than 4 degrees of nesting */
        /* listen failed for socket_desc [%d], p_sctp_fd_info->server_sctp_fd */
        X2AP_TRACE(X2AP_WARNING, " listen() failure, errno: %d", errno);

        /* listen call failed thus deleting created socket_desc */
        if( close_wrapper(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket_desc [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING,
                    " Server socket close failure, errno: %d", errno);
        }

        return SCTP_SOCKET_ERROR;
    }

#if 0   //SCTP_INITMSG already set in x2ap_open_sctp_server_setsockopt_wrapper above
    /* SPR_14436_Fix Start */
    initmsg.sinit_num_ostreams  = nostreams;
    initmsg.sinit_max_instreams = nistreams;
    /*sctp parameters start*/
    initmsg.sinit_max_init_timeo = RRC_SCTP_DEFAULT_INIT_TIMEOUT; 
    if(PNULL != p_sctp_assoc_conf_info)
    {    
        initmsg.sinit_max_attempts = p_sctp_assoc_conf_info->init_msg.sinit_max_attempts;
    }    
    else 
    {    
        initmsg.sinit_max_attempts  = RRC_SCTP_DEFAULT_MAX_INIT_RTX;
    }    
    /*sctp parameters stop*/
    return_value = setsockopt_wrapper(
            server_sctp_sd,
            IPPROTO_SCTP,
            SCTP_INITMSG,
            &initmsg,
            sizeof(initmsg));

    if (return_value < 0) 
    {    
        X2AP_TRACE(X2AP_WARNING,"SCTP : sctp_setsockopt for INIT failed\n");
        x2ap_close_socket(server_sctp_sd);
        return X2AP_SCTP_ERROR;
    }    
    /* SPR_14436_Fix Stop */
#endif

    /*L3-X2AP Drop2 Integration Start*/
    X2AP_MEMSET( (void *)&events, 0, sizeof(events) );

    /* Populating SCTP Events structure*/
    events.sctp_association_event  = 1;
    events.sctp_data_io_event      = 1;
    events.sctp_address_event      = 1;
    events.sctp_send_failure_event = 1;
    events.sctp_peer_error_event   = 1;
    events.sctp_shutdown_event     = 1;

    if (0 > setsockopt_wrapper(server_sctp_sd, IPPROTO_SCTP, SCTP_EVENTS, &events,
                sizeof (events)))
    {
        X2AP_TRACE(X2AP_WARNING,
                " SCTP : sctp_setsockopt for setting events failed\n");
        if( close_wrapper(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket_desc [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d", errno);
        }
        return SCTP_SOCKET_ERROR;
    }
    /*L3-X2AP Drop2 Integration End*/

    /* Bug 908 Fix Start */
    if (0 > setsockopt_wrapper(server_sctp_sd, IPPROTO_SCTP, SCTP_NODELAY, &noDelay,
                sizeof (noDelay)))
    {
        X2AP_TRACE(X2AP_WARNING,
                " SCTP : sctp_setsockopt for setting No-Delay failed\n");
        if( close_wrapper(server_sctp_sd) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
            X2AP_TRACE(X2AP_INFO, "Server socket closed is: %d", server_sctp_sd);
        }
        else 
        {
            /* unable to close socket_desc [%d], p_sctp_fd_info->server_sctp_fd */
            X2AP_TRACE(X2AP_WARNING, 
                    " Server socket close failure, errno: %d", errno);
        }
        return SCTP_SOCKET_ERROR;
    }
    /* Bug 908 Fix Stop */

    /*
     * Add FD to set
     */
    lteFD_SET((S32)server_sctp_sd, &(g_sctp_fd_set));
    init_stack_flag = X2AP_TRUE;
    g_sctp_sd_list[g_sctp_sd_count] = server_sctp_sd;
    g_sctp_sd_count++;

    /*L3-X2AP Drop2 Integration */
    lteFD_SET((S32)server_sctp_sd, &(sctp_fd));

    /*
     * Store server fd in global static var
     */
    g_sctp_server_fd_ipv4 = server_sctp_sd;

    return server_sctp_sd;
}

/* SPR 20633 changes end */
    /*SPR_16533_START*/
        /* Code Removed */
    /*SPR_16533_END*/

/*
 * Helper fucntion for sctp_connectx.
 */
    /*SPR_16533_START*/
        /* Code Removed */
    /*SPR_16533_END*/

/*****************************************************************************
 * Function Name : x2ap_sctp_send
 * INPUT         : socket_desc  -  It is the socket_desc descriptor of an
 *                                         SCTP endpoint.
 *                 p_message    -  It is the message to be sent.
 *                 msg_len      -  It is the size of the message or
 *                                         the size of buffer.
 *                 stream_no    -  It is the stream on which data is
 *                                          to be sent.
 * Output       : None.
 * Returns      : Returns how many bytes sent.
 *                  Returns -1 on Error.
 * Description  : This Function transmits data.
 *****************************************************************************/
S32 x2ap_sctp_send(
            S32                    socket_desc,
            const void            *p_message,
            socklen_t              msg_len,
            U32                    stream_no)
{
    S32 x2ap_send_result = 0;
//#ifdef RRC_UNIT_TEST_FRAMEWORK
#if 0
    U16 src_module_id = RRC_X2AP_MODULE_ID;
    U16 dst_module_id = RRC_X2AP_PEER_eNODEB_MODULE_ID;

    U16 msg_length, msg_api_length;
    U8 *p_msg, *p_x2ap_msg;

    /* Get API length */
    msg_length = msg_len;

    if(msg_length == RRC_FAILURE)
    {
        return RRC_FAILURE;
    }
    RRC_TRACE(RRC_INFO, "Src(%u)->Dst(%u):X2AP_MSG", src_module_id, dst_module_id);

    msg_api_length = msg_length + RRC_API_HEADER_SIZE;

    /* Allocate buffer */
    p_msg = rrc_msg_mem_get(msg_api_length);

    if (p_msg == PNULL)
    {
        /* Not enough memory */
        return RRC_FAILURE;
    }

    memset_wrapper(p_msg, 0, msg_api_length);
    p_x2ap_msg = p_msg;

    /* Fill CSPL header */
    rrc_construct_api_header(p_x2ap_msg, RRC_VERSION_ID, src_module_id,
            dst_module_id, X2AP_ENB_MESSAGE, msg_api_length);

    /* Fill interface header */
    p_x2ap_msg = p_x2ap_msg + RRC_API_HEADER_SIZE;

    memcpy(p_x2ap_msg, p_message, msg_len);

    rrc_send_message(p_msg, dst_module_id);
#endif
//#else


    if(socket_desc < 0)
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_sctp_send: Invalid file Descriptor");
        ALARM_MSG_L3(X2AP_FATAL, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
        return -1;
    }

    if(NULL == p_message)
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_sctp_send: Data to be sent NULL pointer");
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_NULL_POINTER_ERR_CAUSE_ID);
        return -1;
    }
    x2ap_send_result = sctp_sendmsg(
               socket_desc,
               p_message,
               msg_len,
               NULL,
               0,
               /* SPR_14436_Fix Start */
               htonl_wrapper(SCTP_PAYLOAD_PROTOCOL_ID_X2AP),
               /* SPR_14436_Fix Stop */
               0,
              (U16)stream_no,
               0,
               0);
    if(x2ap_send_result < 0)
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_sctp_send: sctp_sendmsg failed");
        ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
    }
//#endif
    return x2ap_send_result;
}


/******************************************************************************
 * Function Name : x2ap_close_association
 * INPUT         : socket_desc  -   Socket Descriptor of the point
 * Output        : None.
 * Returns       : Returns 0 on success or -1 on Error.
 * Description   : This Function performs the graceful shutdown.
 ******************************************************************************/
/* Coverity Fix 95479 Start */
x2ap_return_et x2ap_close_association( sctp_sd_t socket_desc)
/* Coverity Fix 95479 End */
{

    U32 counter = X2AP_NULL;
    if (X2AP_SCTP_ERROR == close_wrapper(socket_desc))
    {
        X2AP_TRACE(X2AP_WARNING,"Problem in closing socket_desc\n");
        return X2AP_FAILURE;
    }
    lteFD_CLR((S32)socket_desc,&g_sctp_fd_set);

    /*L3-X2AP Drop2 Integration */
    lteFD_CLR((S32)socket_desc,&sctp_fd);

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
    if (X2AP_NULL == g_sctp_sd_count)
    {
        init_stack_flag = X2AP_FALSE;
    }
    X2AP_TRACE(X2AP_INFO,
            "SCTP : g_sctp_sd_count decremented to %d\n",g_sctp_sd_count);
    return X2AP_SUCCESS;
}



/******************************************************************************
 * Function Name : x2ap_close_socket
 * INPUT         : socket_desc  -   Socket Descriptor of the point
 * Output        : None.
 * Returns       : Returns 0 on success or -1 on Error.
 * Description   : This Function closes the socket_desc descriptor.
 ******************************************************************************/
S32 x2ap_close_socket(S32 socket_desc)
{
    if (X2AP_SCTP_ERROR == close_wrapper(socket_desc))
    {
        X2AP_TRACE(X2AP_WARNING,"Problem in closing socket_desc\n");
        return X2AP_FAILURE;
    }
    return X2AP_SUCCESS;
}

/******************************************************************************
 * Function Name : x2ap_sctp_accept
 * INPUT         : socket_desc  -   Socket Descriptor of the point
 * Output        : None.
 * Returns       : Returns 0 on success or -1 on Error.
 * Description   : This Function closes the socket_desc descriptor.
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

/*spr8341 IPv6 fix start*/        
    struct sockaddr_in6         *p_addr6 = X2AP_P_NULL;
/*spr8341 IPv6 fix end*/        
    /* Bug 908 Fix Start */
    U32 noDelay = 1;
    /* Bug 908 Fix Stop */
    /* SPR 16300 START */
    S32 ret_val = 0;
    /* SPR 16300 END */

    
    connSock = accept_wrapper(socket_desc, X2AP_P_NULL, X2AP_P_NULL);
    if (X2AP_SCTP_ERROR == connSock)
    {
        X2AP_TRACE(X2AP_WARNING,
                "Problem in accepting on socket_desc errno:%d\n",errno);
        return INVALID_SCTP_SOCKET;
    }

    lteFD_SET((S32)connSock, &(g_sctp_fd_set));
    g_sctp_sd_list[g_sctp_sd_count] = connSock;
    g_sctp_sd_count++;

    /*L3-X2AP Drop2 Integration */
    lteFD_SET((S32)connSock, &(sctp_fd));

/*spr8341 IPv6 fix start*/        
    if(AF_INET == addr_family)
    {
        /* SPR 16300 START */
        ret_val = sctp_getpaddrs(connSock, X2AP_NULL, &p_addr);

        if (X2AP_SCTP_ERROR == ret_val)
        /* SPR 16300 END */
        {
            X2AP_TRACE(X2AP_WARNING,
                    "Problem in getting peer addresses on socket_desc\n");
            x2ap_close_socket(connSock);
            return INVALID_SCTP_SOCKET;
        }

        /* SPR 16300 START */
        *p_addr_count = (U32)ret_val;
        /* SPR 16300 END */

        *pp_peerAddrs = rrc_mem_get(sizeof(struct sockaddr) * (*p_addr_count)) ;
        if(X2AP_P_NULL != *pp_peerAddrs)
        {
            X2AP_MEMCPY(*pp_peerAddrs, p_addr, sizeof(struct sockaddr) * (*p_addr_count) );
        }
        else
        {
		X2AP_TRACE(X2AP_WARNING,"x2ap_sctp_accept pp_peerAddrs is NULL \n");
		/* coverity_60523 start */		
		sctp_freepaddrs(p_addr);
		/* coverity_60523 stop */		
		return INVALID_SCTP_SOCKET;
	}
        sctp_freepaddrs(p_addr);
    }

    else if(AF_INET6 == addr_family)
    {
        /* SPR 16300 START */
        ret_val = sctp_getpaddrs(connSock, X2AP_NULL, (struct sockaddr **)&p_addr6);
        if (X2AP_SCTP_ERROR == ret_val)
        /* SPR 16300 END */
        {
            X2AP_TRACE(X2AP_WARNING,
                    "Problem in getting peer addresses on socket_desc\n");
            x2ap_close_socket(connSock);
            return INVALID_SCTP_SOCKET;
        }

        /* SPR 16300 START */
        *p_addr_count = (U32)ret_val;
        /* SPR 16300 END */
        *pp_peerAddr6 = rrc_mem_get(sizeof(struct sockaddr_in6) * (*p_addr_count)) ;
        if(X2AP_P_NULL != *pp_peerAddr6)
        {
            X2AP_MEMCPY(*pp_peerAddr6, p_addr6, sizeof(struct sockaddr_in6) * (*p_addr_count) );
        }
        else
        {
            X2AP_TRACE(X2AP_WARNING,"x2ap_sctp_accept pp_peerAddrs is NULL \n");
            /* SPR 19838 : Coverity 11157 fix start */
            sctp_freepaddrs((struct sockaddr *)p_addr6);
            /* SPR 19838 : Coverity 11157 fix end */
            return INVALID_SCTP_SOCKET;
        }
        /* SPR 19838 : Coverity 11157 fix start */
        if(X2AP_P_NULL != p_addr6)
        {
            sctp_freepaddrs((struct sockaddr *)p_addr6);
        }
        /* SPR 19838 : Coverity 11157 fix end */
    }
    else
    {
        X2AP_TRACE(X2AP_WARNING,
                "Invalid address family :%d\n",addr_family);
        return SCTP_SOCKET_ERROR;
    }
/*spr8341 IPv6 fix end*/        

    /* Bug 908 Fix Start */
    if (0 > setsockopt_wrapper(connSock, IPPROTO_SCTP, SCTP_NODELAY, &noDelay,
                sizeof (noDelay)))
    {
        X2AP_TRACE(X2AP_WARNING,
                " SCTP : sctp_setsockopt for setting No-Delay failed\n");
        if( close_wrapper(connSock) == 0 ) 
        {
            /*association closed successfully*/
            /* Trace the successful closure of socket_desc */
            X2AP_TRACE(X2AP_INFO, "Peer socket closed is: %d", connSock);
        }
        else 
        {
            X2AP_TRACE(X2AP_WARNING, 
                    " Peer socket close failure, errno: %d", errno);
        }
        return SCTP_SOCKET_ERROR;
    }
    /* Bug 908 Fix Stop */

    return connSock;
}

    /*SPR_16533_START*/
        /* Code Removed */
    /*SPR_16533_END*/
#endif
