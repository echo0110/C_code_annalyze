/************************************************************************************
*
*   FILE NAME  : socket_sctp_tools.c
*
*   DESCRIPTION: functions for transferring messages via SCTP sockets.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "simTypes.h"
#include "simLogger.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "socket_sctp_tools.h"
#include "simuScheduler.h"

/************************************************************************************
 * Local definitions
 ************************************************************************************/
#ifndef IPPROTO_SCTP
    #define IPPROTO_SCTP 132
#endif

/************************************************************************************
 * Global variables
 ************************************************************************************/

/************************************************************************************
 * Local variables
 ************************************************************************************/

static UInt32 sctp_no_of_ip_addr = 1;
static SocketType cli_sctpSocketFd = SOCKET_ERROR;

/************************************************************************************
  Functions implementation
 ************************************************************************************/

#ifdef ARICENT_SCTP_IPR




/****************************************************************************
 * Function Name  : sctpSocketInit
 * Inputs         : Host name, sockaddr_in, Port Number, isBlocking
 * Outputs        :
 * Returns        : Socket file descriptor or SOCKET_ERROR
 * Variables      :
 * Description    : to initialize SCTP socket with address/port
 ****************************************************************************/
	/*SPR 20633 Fix Start*/
SocketType sctpSocketInit(void *host_name, UInt16 port, UInt8 isBlocking)
{
#ifdef WIN32
    unsigned long nb = 1;
#endif
    SocketType sockfd = SCTP_SOCKET_ERROR;
        int ret;
	struct addrinfo hint, *res = NULL;
        /*COV_START*/
	//struct in_addr *inp;
        /*COV_STOP*/
	struct in6_addr inp6;
        /*COV_START*/
	//inp = (struct in_addr*)malloc(sizeof(struct in_addr));
        /*COV_STOP*/
	memset(&hint, '\0', sizeof(hint));
	hint.ai_family = PF_UNSPEC;
	hint.ai_flags = AI_NUMERICHOST;

	ret = getaddrinfo(host_name, NULL, &hint, &res);     
	if (ret) {
		puts("Invalid address");
		puts(gai_strerror(ret));
		return 1;
	}
	if(res->ai_family == AF_INET) 
    {
		SIM_LOG(SIM_LOG_INFO,"%s is an ipv4 address\n",host_name); 

    SIM_LOG(SIM_LOG_INFO, "SCTP: Creating socket Host['%s'], Port[%i]",
            host_name, port);

    if ( SCTP_SOCKET_ERROR == (sockfd = sctp_socket(SCTP_AF_INET, SOCK_STREAM, IPPROTO_SCTP)) )
    {
        SIM_LOG(SIM_LOG_CRITICAL, "SCTP: creating socket error: %i",
                sk_errno);
        return SOCKET_ERROR;
    }
		struct sockaddr_in s;
		(s).sin_family = SCTP_AF_INET;
        	/*COV_START*/
		inet_aton((char *)host_name,&s.sin_addr);
		//(s).sin_addr = *inp;
	        /*COV_STOP*/
		(s).sin_port = htons(port);
		memset(&((s).sin_zero),0,8);
		SIM_LOG(SIM_LOG_INFO, "SCTP: sin_port[%u]: sin_addr[%d]",
				(s).sin_port, (s).sin_addr.s_addr);



		/* Bind the port to specific socket for listening */
		if ( SCTP_SOCKET_ERROR == sctp_bindx(
					sockfd,
					(struct sockaddr *)&s,
					sctp_no_of_ip_addr, SCTP_BINDX_ADD_ADDR) )
    {
			SIM_LOG(SIM_LOG_CRITICAL, "SCTP: Bind failed %i", sk_errno);
            return SOCKET_ERROR;
        }

	} 
	else if (res->ai_family == AF_INET6) {
		SIM_LOG(SIM_LOG_INFO,"%s is an ipv6 address\n",host_name); 
		SIM_LOG(SIM_LOG_INFO, "SCTP: Creating socket Host['%s'], Port[%i]",
				host_name, port);

		if ( SCTP_SOCKET_ERROR == (sockfd = sctp_socket(SCTP_AF_INET6, SOCK_STREAM, IPPROTO_SCTP)) )
        {
			SIM_LOG(SIM_LOG_CRITICAL, "SCTP: creating socket error: %i",
					sk_errno);
			return SOCKET_ERROR;
        }

		struct sockaddr_in6  s;
		(s).sin6_family = SCTP_AF_INET6;
		inet_pton(SCTP_AF_INET6,(char *)host_name,(void *)&inp6);
		(s).sin6_addr = inp6;
		(s).sin6_port = htons(port);
		SIM_LOG(SIM_LOG_INFO, "SCTP: sin_port[%u]: sin_addr[%d]",
				(s).sin6_port, (s).sin6_addr);
    if ( SCTP_SOCKET_ERROR == sctp_bindx(
                                sockfd,
					(struct sockaddr*)&s,
                                sctp_no_of_ip_addr, SCTP_BINDX_ADD_ADDR) )
    {
        SIM_LOG(SIM_LOG_CRITICAL, "SCTP: Bind failed %i", sk_errno);
        return SOCKET_ERROR;
    }



	} else {
		printf("%s is an is unknown address format %d\n",host_name,res->ai_family);
	}

	freeaddrinfo(res);
	/*SPR 20633 Fix End*/

    SIM_LOG( SIM_LOG_DETAIL, "SCTP: binding succeeded" );

    if (SCTP_SOCKET_ERROR == sctp_listen(sockfd,1))
    {
       SIM_LOG(SIM_LOG_CRITICAL,"SCTP: Listen Failed: socket_fd:%d",sockfd);
       return SCTP_SOCKET_ERROR;
    }

    return sockfd;
}

/************************************************************************************
 * FUNCTION NAME    : sctpSocketClose
 * DESCRIPTION      : Close SCTP socket.
 * PARAMATERS       :
 * RETURN VALUE     :
 ************************************************************************************/
void sctpSocketClose(SocketType* p_socket_fd)
{
    int ret_code = 0;
    if ( *p_socket_fd != SOCKET_ERROR )
    {
        if (SCTP_SOCKET_ERROR == (ret_code = sctp_close(*p_socket_fd)))
        {
            SIM_LOG(SIM_LOG_CRITICAL, "SCTP: Close socket failed: %u",ret_code );
        }
        else
        {
            SIM_LOG(SIM_LOG_INFO, "SCTP: Socket closed socket_fd: %u",*p_socket_fd );
        }

        *p_socket_fd = SOCKET_ERROR;
    }
}

/************************************************************************************
 * FUNCTION NAME    : sctpSocketRead
 * DESCRIPTION      : Reads message from RX SCTP socket
 * PARAMATERS       : buf - data from socket will be placed here.
 *                          Should have SOCKET_BUFFER_SIZE length.
 *                    SInt32 timeout
 * RETURN VALUE     : Length of read message.
                      If there is no message it returns 0 (no wait for a message).
 ************************************************************************************/
UInt32 sctpSocketRead(UInt8 *buf,SInt32 timeout)
{
    Int bytesRead = 0;
    UInt32 addrLen = 0;
    struct sockaddr_in sockAddr;
    Int sock_flags = 0;
    sctp_fd_set_st local_sctp_fd;
    struct timeval  sel_timeval;
    struct sctp_sndrcvinfo sndrcvinfo;

    if ( SOCKET_ERROR == cli_sctpSocketFd )
    {
        return 0; /* socket is not initialized */
    }


    SCTP_FD_ZERO(&local_sctp_fd);
    SCTP_FD_SET(cli_sctpSocketFd, &local_sctp_fd);

    sel_timeval.tv_sec = 0;
    sel_timeval.tv_usec = timeout * 1000;
    if (SCTP_SOCKET_ERROR != sctp_select(cli_sctpSocketFd + 1, &local_sctp_fd, 0, 0, &sel_timeval))
    {
        if (SCTP_FD_ISSET(cli_sctpSocketFd, &local_sctp_fd))
        {
            bytesRead = sctp_sctp_recvmsg(
                            cli_sctpSocketFd,
                            (void *)buf, SOCKET_BUFFER_SIZE,
                            (struct sockaddr *)&sockAddr, &addrLen,
                            &sndrcvinfo, &sock_flags );
            if ( SCTP_SOCKET_ERROR == bytesRead )
            {
                SIM_LOG(SIM_LOG_WARNING, "SCTP: sctp_sctp_recvmsg failed :%i ", sk_errno);
            }
            else if(sock_flags & SCTP_MSG_NOTIFICATION)
            {
                if ((((sctp_notification_ut *)buf)->
                                sn_header.sn_type) ==  SCTP_SHUTDOWN_EVENT)
                {
                    SIM_LOG(SIM_LOG_INFO, "SCTP: Sockets shutdown notification");
                    sctpSocketClose(&cli_sctpSocketFd);
                    //Listen_for_conn_req(); //TODO
                    bytesRead = 0;
                }
            }
        }
    }
    else
    {
        SIM_LOG(SIM_LOG_WARNING, "SCTP: Select failed :%i ", sk_errno);
    }

    if( bytesRead <= 0)
    {
        return 0; /*if no message, bytesRead will be -1 */
    }

    return bytesRead;
}

/************************************************************************************
 * FUNCTION NAME    : sctpSocketWrite
 * DESCRIPTION      : Writes message from to TX SCTP socket
 * PARAMATERS       : buffer,bufferLen
 * RETURN VALUE     : Number of really transferred bytes. If OK should be == bufferLen
 ************************************************************************************/
UInt32 sctpSocketWrite(UInt8 *buffer, UInt32 bufferLen)
{
    Int res_len = 0;

    if ( SOCKET_ERROR == cli_sctpSocketFd )
    {
        return 0; /* socket is not initialized */
    }

    res_len = sctp_sctp_sendmsg(
                cli_sctpSocketFd,
                (void *)buffer, bufferLen,
                PNULL,0,0,0,0,0,0);

    if ( SCTP_SOCKET_ERROR == res_len )
    {
        SIM_LOG(SIM_LOG_WARNING, "SCTP: Write failed :%i ", sk_errno);
    }

    return res_len;
}

/************************************************************************************
 * FUNCTION NAME    : sctpSocketListenAndAccept
 * DESCRIPTION      : Listen and accept connection on SCTP socket
 * PARAMATERS       : socket_fd
 * RETURN VALUE     : Socket file descriptor or SOCKET_ERROR
 ************************************************************************************/
UInt32 sctpSocketListenAndAccept(SocketType socket_fd)
{
    UInt32 addrLen = sizeof(struct sockaddr_in);
    struct sockaddr_in sockAddr;
    struct sctp_event_subscribe events;

    SIM_LOG(SIM_LOG_INFO,"============SCTP server is listening==============");

    bzero(&events,sizeof(events));
    events.sctp_data_io_event = 1;
    events.sctp_association_event  = 1;
    events.sctp_address_event      = 1;
    events.sctp_send_failure_event = 1;
    events.sctp_peer_error_event   = 1;
    events.sctp_shutdown_event     = 1;

    if (SCTP_SOCKET_ERROR ==
        (cli_sctpSocketFd = sctp_accept(
                        socket_fd,
                        (struct sockaddr *)&sockAddr, &addrLen)))
    {
        SIM_LOG(SIM_LOG_INFO, "SCTP: Accept failed: %i", sk_errno);
        return SCTP_SOCKET_ERROR;
    }
    else
    {
        SIM_LOG(SIM_LOG_INFO,"SCTP: Connection accepted Host[%08X] Port[%04X]",
                ntohl(sockAddr.sin_addr.s_addr), ntohs(sockAddr.sin_port)  );
    }

    if (SCTP_SOCKET_ERROR ==
                            sctp_setsockopt(
                                cli_sctpSocketFd,
                                IPPROTO_SCTP,
                                SCTP_SO_SET_EVENTS,
                                (const void*)&events,sizeof(events)) )
    {
        SIM_LOG(SIM_LOG_CRITICAL, "SCTP: set sock opt failed: %i", sk_errno);
        return SCTP_SOCKET_ERROR;
    }

    return 0;
}

/************************************************************************************
 * FUNCTION NAME    : sctpWaitReset
 * DESCRIPTION      : wait for reset on SCTP socket
 * PARAMATERS       : none
 * RETURN VALUE     : Read Length / 0
 ************************************************************************************/
UInt32 sctpWaitReset(void)
{
    UInt8           buf[SOCKET_BUFFER_SIZE];
    UInt32          bytesRead = 0;
    struct sockaddr_in sockAddr;
    Int sock_flags = 0;
    UInt32 addrLen = 0;
    struct sctp_sndrcvinfo sndrcvinfo;

    if ( SOCKET_ERROR == cli_sctpSocketFd )
    {
        SIM_LOG(SIM_LOG_INFO, "%s: SOCKET_ERROR == cli_sctpSocketFd",
        		__FUNCTION__);
        return SIM_FALSE; /* socket is not initialized */
    }

    esLock();

    bytesRead = sctp_sctp_recvmsg(
                    cli_sctpSocketFd,
                    (void *)buf, SOCKET_BUFFER_SIZE,
                    (struct sockaddr *)&sockAddr, &addrLen,
                    &sndrcvinfo, &sock_flags );
    if ( SCTP_SOCKET_ERROR == bytesRead )
    {
        SIM_LOG(SIM_LOG_WARNING, "%s: sctp_sctp_recvmsg failed :%i ",
			__FUNCTION__,sk_errno);
    }
    else if(sock_flags & SCTP_MSG_NOTIFICATION)
    {
        if ((((sctp_notification_ut *)buf)->
                        sn_header.sn_type) ==  SCTP_SHUTDOWN_EVENT)
        {
            SIM_LOG(SIM_LOG_INFO, "%s: Sockets shutdown notification",
            		__FUNCTION__);
            sctpSocketClose(&cli_sctpSocketFd);
            //Listen_for_conn_req(); //TODO
            bytesRead = 0;
        }
    }

    esUnlock();

    return bytesRead;
}

#endif








#ifdef LKSCTP




/****************************************************************************
 * Function Name  : sctpSocketInit
 * Inputs         : Host name, sockaddr_in, Port Number, isBlocking
 * Outputs        :
 * Returns        : Socket file descriptor or SOCKET_ERROR
 * Variables      :
 * Description    : to initialize SCTP socket with address/port
 ****************************************************************************/
/*SPR 20633 Fix start*/
SocketType sctpSocketInit(void *host_name,UInt16 port, UInt8 isBlocking)
{
#ifdef WIN32
    unsigned long nb = 1;
#endif
    SocketType sockfd = SCTP_SOCKET_ERROR;
    int ret;
    struct addrinfo hint, *res = NULL;
    struct in_addr *inp;
    struct in6_addr inp6;
    inp = (struct in_addr*)malloc(sizeof(struct in_addr));
    memset(&hint, '\0', sizeof(hint));
    hint.ai_family = PF_UNSPEC;
    hint.ai_flags = AI_NUMERICHOST;

    ret = getaddrinfo(host_name, NULL, &hint, &res);     
    if (ret) {
	    puts("Invalid address");
	    puts(gai_strerror(ret));
	    return 1;
    }
    if(res->ai_family == AF_INET) 
    { 
		SIM_LOG(SIM_LOG_INFO,"%s is an ipv4 address\n",host_name); 

    SIM_LOG(SIM_LOG_INFO, "SCTP: Creating socket Host['%s'], Port[%i]",
            host_name, port);

    if ( SCTP_SOCKET_ERROR == (sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) )
    {
        SIM_LOG(SIM_LOG_CRITICAL, "SCTP: creating socket error: %i",
                errno);
        return SOCKET_ERROR;
    }

    struct sockaddr_in s;
    (s).sin_family = AF_INET;
    inet_aton((char *)host_name,inp);
    (s).sin_addr=*inp;
    (s).sin_port = htons(port);
    memset(&((s).sin_zero),0,8);
    SIM_LOG(SIM_LOG_INFO, "SCTP: sin_port[%u]: sin_addr[%d]",
            (s).sin_port, (s).sin_addr.s_addr);

    /* Bind the port to specific socket for listening */
    if ( SCTP_SOCKET_ERROR == sctp_bindx(
                                sockfd,
                                (struct sockaddr*)&s,
                                sctp_no_of_ip_addr, SCTP_BINDX_ADD_ADDR) )
        {
        SIM_LOG(SIM_LOG_CRITICAL, "SCTP: Bind failed %i", errno);
            return SOCKET_ERROR;
        }
    } 
    else if (res->ai_family == AF_INET6) {
	    SIM_LOG(SIM_LOG_INFO,"%s is an ipv6 address\n",host_name); 
	    SIM_LOG(SIM_LOG_INFO, "SCTP: Creating socket Host['%s'], Port[%i]",
			    host_name, port);

	    if ( SCTP_SOCKET_ERROR == (sockfd = socket(AF_INET6, SOCK_STREAM, IPPROTO_SCTP)) )
	    {
		    SIM_LOG(SIM_LOG_CRITICAL, "SCTP: creating socket error: %i", errno);
		    return SOCKET_ERROR;
	    }

	    struct sockaddr_in6  s;
	    (s).sin6_family = AF_INET6;
	    inet_pton(AF_INET6,(char *)host_name,(void *)&inp6);
	    (s).sin6_addr = inp6;
	    (s).sin6_port = htons(port);
	    SIM_LOG(SIM_LOG_INFO, "SCTP: sin_port[%u]: sin_addr[%d]",
			    (s).sin6_port, (s).sin6_addr);
    if ( SCTP_SOCKET_ERROR == sctp_bindx(
                                sockfd,
				    (struct sockaddr*)&s,
                                sctp_no_of_ip_addr, SCTP_BINDX_ADD_ADDR) )
    {
        SIM_LOG(SIM_LOG_CRITICAL, "SCTP: Bind failed %i", errno);
        return SOCKET_ERROR;
    }



    } else {
	    printf("%s is an is unknown address format %d\n",host_name,res->ai_family);
    }

    freeaddrinfo(res);


/*SPR 20633 Fix End*/
    SIM_LOG( SIM_LOG_DETAIL, "SCTP: binding succeeded" );

    if (SCTP_SOCKET_ERROR == listen(sockfd,1))
    {
       SIM_LOG(SIM_LOG_CRITICAL,"SCTP: Listen Failed: socket_fd:%d",sockfd);
       return SCTP_SOCKET_ERROR;
    }

    return sockfd;
}

/************************************************************************************
 * FUNCTION NAME    : sctpSocketClose
 * DESCRIPTION      : Close SCTP socket.
 * PARAMATERS       :
 * RETURN VALUE     :
 ************************************************************************************/
void sctpSocketClose(SocketType* p_socket_fd)
{
    int ret_code = 0;
    if ( *p_socket_fd != SOCKET_ERROR )
    {
        if (SCTP_SOCKET_ERROR == (ret_code = close(*p_socket_fd)))
        {
            SIM_LOG(SIM_LOG_CRITICAL, "SCTP: Close socket failed: %u",ret_code );
        }
        else
        {
            SIM_LOG(SIM_LOG_INFO, "SCTP: Socket closed socket_fd: %u",*p_socket_fd );
        }

        *p_socket_fd = SOCKET_ERROR;
    }
}

/************************************************************************************
 * FUNCTION NAME    : sctpSocketRead
 * DESCRIPTION      : Reads message from RX SCTP socket
 * PARAMATERS       : buf - data from socket will be placed here.
 *                          Should have SOCKET_BUFFER_SIZE length.
 * RETURN VALUE     : Length of read message.
                      If there is no message it returns 0 (no wait for a message).
 ************************************************************************************/
UInt32 sctpSocketRead(UInt8 *buf, SInt32 timeout)
{
    Int bytesRead = 0;
    UInt32 addrLen = 0;
    struct sockaddr_in sockAddr;
    Int sock_flags = 0;
    fd_set local_sctp_fd;
    struct timeval  sel_timeval;
    struct sctp_sndrcvinfo sndrcvinfo;

    if ( SOCKET_ERROR == cli_sctpSocketFd )
    {
        return 0; /* socket is not initialized */
    }


    FD_ZERO(&local_sctp_fd);
    FD_SET(cli_sctpSocketFd, &local_sctp_fd);

    sel_timeval.tv_sec = 0;
    sel_timeval.tv_usec = timeout * 1000;
    if (SCTP_SOCKET_ERROR != select(cli_sctpSocketFd + 1, &local_sctp_fd, 0, 0, &sel_timeval))
    {
        if (FD_ISSET(cli_sctpSocketFd, &local_sctp_fd))
        {
            bytesRead = sctp_recvmsg(
                            cli_sctpSocketFd,
                            (void *)buf, SOCKET_BUFFER_SIZE,
                            (struct sockaddr *)&sockAddr, &addrLen,
                            &sndrcvinfo, &sock_flags );
            if ( SCTP_SOCKET_ERROR == bytesRead )
            {
                SIM_LOG(SIM_LOG_WARNING, "SCTP: sctp_sctp_recvmsg failed :%i ", errno);
            }
            else if(sock_flags & MSG_NOTIFICATION)
            {
                if ((((union sctp_notification *)buf)->
                                sn_header.sn_type) ==  SCTP_SHUTDOWN_EVENT)
                {
                    SIM_LOG(SIM_LOG_INFO, "SCTP: Sockets shutdown notification");
                    sctpSocketClose(&cli_sctpSocketFd);
                    //Listen_for_conn_req(); //TODO
                    bytesRead = 0;
                }
            }
        }
    }
    else
    {
        SIM_LOG(SIM_LOG_WARNING, "SCTP: Select failed :%i ", errno);
    }

    if( bytesRead <= 0)
    {
        return 0; /*if no message, bytesRead will be -1 */
    }

    return bytesRead;
}

/************************************************************************************
 * FUNCTION NAME    : sctpSocketWrite
 * DESCRIPTION      : Writes message from to TX SCTP socket
 * PARAMATERS       : buffer,bufferLen
 * RETURN VALUE     : Number of really transferred bytes. If OK should be == bufferLen
 ************************************************************************************/
UInt32 sctpSocketWrite(UInt8 *buffer, UInt32 bufferLen)
{
    Int res_len = 0;

    if ( SOCKET_ERROR == cli_sctpSocketFd )
    {
        return 0; /* socket is not initialized */
    }

    res_len = sctp_sendmsg(
                cli_sctpSocketFd,
                (void *)buffer, bufferLen,
                PNULL,0,0,0,0,0,0);

    if ( SCTP_SOCKET_ERROR == res_len )
    {
        SIM_LOG(SIM_LOG_WARNING, "SCTP: Write failed :%i,%s ", errno,strerror(errno));
    }

    return res_len;
}

/************************************************************************************
 * FUNCTION NAME    : sctpSocketListenAndAccept
 * DESCRIPTION      : Listen and accept connection on SCTP socket
 * PARAMATERS       : socket_fd
 * RETURN VALUE     : Socket file descriptor or SOCKET_ERROR
 ************************************************************************************/
UInt32 sctpSocketListenAndAccept(SocketType socket_fd)
{
    UInt32 addrLen = sizeof(struct sockaddr_in);
    struct sockaddr_in sockAddr;
    struct sctp_event_subscribe events;

    SIM_LOG(SIM_LOG_INFO,"============SCTP server is listening==============");

    bzero(&events,sizeof(events));
    events.sctp_data_io_event = 1;
    events.sctp_association_event  = 1;
    events.sctp_address_event      = 1;
    events.sctp_send_failure_event = 1;
    events.sctp_peer_error_event   = 1;
    events.sctp_shutdown_event     = 1;

    if (SCTP_SOCKET_ERROR ==
        (cli_sctpSocketFd = accept(
                        socket_fd,
                        (struct sockaddr *)&sockAddr, &addrLen)))
    {
        SIM_LOG(SIM_LOG_INFO, "SCTP: Accept failed: %i", errno);
        return SCTP_SOCKET_ERROR;
    }
    else
    {
        SIM_LOG(SIM_LOG_INFO,"SCTP: Connection accepted Host[%08X] Port[%04X]",
                ntohl(sockAddr.sin_addr.s_addr), ntohs(sockAddr.sin_port)  );
    }

    if (SCTP_SOCKET_ERROR ==
                            setsockopt(
                                cli_sctpSocketFd,
                                SOL_SCTP,
                                SCTP_EVENTS,
                                (const void*)&events,sizeof(events)) )
    {
        SIM_LOG(SIM_LOG_CRITICAL, "SCTP: set sock opt failed: %i", errno);
        return SCTP_SOCKET_ERROR;
    }

    return 0;
}

/************************************************************************************
 * FUNCTION NAME    : sctpWaitReset
 * DESCRIPTION      : wait for reset on SCTP socket
 * PARAMATERS       : none
 * RETURN VALUE     : Read Length / 0
 ************************************************************************************/
UInt32 sctpWaitReset(void)
{
    UInt8           buf[SOCKET_BUFFER_SIZE];
    UInt32          bytesRead = 0;
    struct sockaddr_in sockAddr;
    Int sock_flags = 0;
    UInt32 addrLen = 0;
    struct sctp_sndrcvinfo sndrcvinfo;

    if ( SOCKET_ERROR == cli_sctpSocketFd )
    {
        SIM_LOG(SIM_LOG_INFO, "%s: SOCKET_ERROR == cli_sctpSocketFd",
        		__FUNCTION__);
        return SIM_FALSE; /* socket is not initialized */
    }

    esLock();

    bytesRead = sctp_recvmsg(
                    cli_sctpSocketFd,
                    (void *)buf, SOCKET_BUFFER_SIZE,
                    (struct sockaddr *)&sockAddr, &addrLen,
                    &sndrcvinfo, &sock_flags );
    if ( SCTP_SOCKET_ERROR == bytesRead )
    {
        SIM_LOG(SIM_LOG_WARNING, "%s: sctp_sctp_recvmsg failed :%i ",
			__FUNCTION__,errno);
    }
    else if(sock_flags & MSG_NOTIFICATION)
    {
        if ((((union sctp_notification *)buf)->
                        sn_header.sn_type) ==  SCTP_SHUTDOWN_EVENT)
        {
            SIM_LOG(SIM_LOG_INFO, "%s: Sockets shutdown notification",
            		__FUNCTION__);
            sctpSocketClose(&cli_sctpSocketFd);
            //Listen_for_conn_req(); //TODO
            bytesRead = 0;
        }
    }

    esUnlock();

    return bytesRead;
}

#endif



