/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: server.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains functions related to http. 
 *
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Feb,2012                    Initial
 *   2012/11/07 gur24604  Initial Version 1.0
 *   2013/18/03 Anant K Sharma(24604)  removed warnings
 *
 ******************************************************************************/
	
/*******************************************************************************
 *
 *           STANDARD LIBRARY FILES
 *
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <event_handler.h>
#include <ManagementServerModel.h>
#include "strfun.h"
#include "logger.h"
#include <ipr_cwmp_server.h>
#include <pthread.h>
#include<ipr_cwmp_read_config.h>//OAM SI code changes
#include "common_def.h"
//Added this prototype of cspl API to remove warning as there is no interface in cspl code 
void      yeMd5Digest( const unsigned char *buffer, unsigned int length, unsigned char result[16] );

/******************************************************************************
Global Variables
******************************************************************************/
/*connection requests recieved */
UInt32 g_ipr_cwmp_con_req = 0;
/*SPR 16545 Fixed Start*/
/*  coverity 94850 fix start */
S32 tr069_gettimeofday_wrapper(struct timeval *p_tv);
/*  coverity 94850 fix end */
/*SPR 16545 Fixed End*/

/* No. of Connection requests recieved */
UInt32 g_ipr_cwmp_con_req_counter = 0;

/* Global vaiable to indicate if session is opened */
extern SInt32 g_session_manager_session_open;
/* SPR 16568 FIX START */
/*SPR 17610 Fixed Start*/
extern UInt8 HeNB_SELF_IP[40];
/* SPR 16568 FIX END */
/*SPR 17610 Fixed End*/
/****************************************************************************
 * Function Name  : ipr_cwmp_http_server 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : Void
 * Description    : This function is used to start a http server 
 *                 
 ****************************************************************************/
	
void* ipr_cwmp_http_server (void *args)
{
    SInt32 sd, connfd, retbind;
    struct sockaddr_in serveraddress, cliaddr;
    /*SPR 17610 Fixed Start*/
    struct sockaddr_in6  serveraddress6 ,cliaddr6;
    /*SPR 17610 Fixed End*/
    if(NULL == args)
    {
        TRACE (LOG_CRITICAL ,"Acs Connection Listner", "Args NULL\n");
    }    

    socklen_t len;
    SInt32 ret;
    UInt8 ip_address[IP_ADDR_LEN + 1] = {'\0'};
    UInt8 EventCode[MAX_STR_LEN + 1] = {'\0'};
    UInt8 buf[MAX_BUFFER_LENGTH] = {'\0'};
    UInt8  *databuf = NULL;
    databuf = (UInt8*) ALLOCATE(MAX_BUFFER_LENGTH + 1);
    if(NULL ==databuf)
    {
        perror ("MEm error ");
        exit (1);
    }

    UInt8* auth[2] = {PNULL};
    UInt8 skip_authenticate = 0;
    UInt32 option = 1;
    sEventStruct *newEvent = PNULL;
    TRACE (LOG_CRITICAL, "Acs Connection Listner", "Entering in %s ",
            __FUNCTION__);

    if (strncasecmp((const char *)auth_type , NONE_AUTH , NONE_AUTH_SIZE) == 0)
        skip_authenticate = 1;
    TRACE (LOG_CRITICAL, "Acs Connection Listner", "Authentication Type : %s", auth_type);

    TRACE (LOG_CRITICAL, "Acs Connection Listner", "http server port is %d ",
            http_server_port);
    ENB_MEMSET (&serveraddress, 0, sizeof (serveraddress));
    /*SPR 17610 Fixed Start*/
    ENB_MEMSET (&serveraddress6, 0, sizeof (serveraddress6));

    /*SPR 17610 Fixed End*/


    /*SPR 17610 Fixed Start*/
    struct addrinfo hint, *res = NULL;

    memset(&hint, '\0', sizeof(hint));

    hint.ai_family = PF_UNSPEC;
    hint.ai_flags = AI_NUMERICHOST;
    TRACE(LOG_CRITICAL ,"Acs Connection Listner", "HeNB Self IP Address = %s",HeNB_SELF_IP);

    ret = getaddrinfo((const char *)HeNB_SELF_IP,NULL,&hint, &res);
    if (ret) {
        TRACE(LOG_CRITICAL ,"Acs Connection Listner", "getaddrinfo Returns Failure %d",errno);
        /* SPR 22156 FIXED START */
        return NULL;
        /* SPR 22156 FIXED END */

    }
    if(res->ai_family == AF_INET) {
        sd = ENB_SOCKET(AF_INET, SOCK_STREAM, 0);
        if (sd < 0)
        {
            perror ("socket");

            /* SPR 18726 FIXED START*/
            //CODE REMOVE
            /*Coverity 106590 Fix Start*/
            DEALLOCATE(databuf);
            /*coverity 106590 Fix End*/
            /* coverity 106588 fixed start */
            /* SPR 22314 Start */
            TR069_FREE_ADDRINFO(res);
            /* SPR 22314 End */
            /* coverity 106588 fixed end */
            return NULL;
            /* SPR 18726 FIXED END*/


        }

        TRACE (LOG_CRITICAL, "Acs Connection Listner", "is an is known ipv4 address format : %d",res->ai_family);
        serveraddress.sin_family = AF_INET;
        serveraddress.sin_port = ENB_HTONS(http_server_port);	//PORT NO
        /* SPR 16568 FIX START */
        serveraddress.sin_addr.s_addr = inet_addr(
                (const char *)HeNB_SELF_IP);
        /* SPR 16568 FIX END */
        if( -1 == ENB_SETSOCKOPT(sd,SOL_SOCKET, SO_REUSEADDR, &option, sizeof(UInt32)))
        {
            /* coverity 106588 fixed start */
            /* SPR 22314 Start */
            TR069_FREE_ADDRINFO(res);
            /* SPR 22314 End */
            /* coverity 106588 fixed end */
            perror ("Socket set Fails\n");
            exit (1);
        }
        retbind =
            ENB_BIND(sd, (struct sockaddr *) &serveraddress, sizeof (serveraddress));
        if (-1 == retbind)
        {
            /* SPR 21747 FIXED START */
            perror ("Bind Fails first time so going for retry \n");
            TRACE (LOG_CRITICAL, "Acs Connection Listner", "Binding second time");
            for(;;)
            {
                TR069_USLEEP (2000000);
                retbind =   ENB_BIND(sd, (struct sockaddr *) &serveraddress, sizeof (serveraddress));
                if (-1 == retbind)
                {
                    perror ("Bind Fails while retrying\n");
                }
                else
                    break;
            }    
                /* Code Removed */
                /* SPR 21747 FIXED END */

            /* SPR 18726 FIXED START*/
            //CODE REMOVE
            /* SPR 18726 FIXED END*/
        }
        ENB_LISTEN (sd, 5);
    }
    else if(res->ai_family == AF_INET6) {
        sd = ENB_SOCKET(AF_INET6, SOCK_STREAM, 0);
        if (sd < 0)
        {
            perror ("socket");
            /* SPR 18726 FIXED START*/
            //CODE REMOVE
            /* coverity 40531 fixed start*/
            //ENB_CLOSE(sd);
            /* coverity 40531 fixed end*/
            /*Coverity 106590 Fix Start*/
            DEALLOCATE(databuf);
            /*coverity 106590 Fix End*/
            /* coverity 106588 fixed start */
            /* SPR 22314 Start */
            TR069_FREE_ADDRINFO(res);
            /* SPR 22314 End */
            /* coverity 106588 fixed end */
            return NULL;
            /* SPR 18726 FIXED END*/

        }

        TRACE (LOG_CRITICAL, "Acs Connection Listner", "is an is known ipv6 address format : %d",res->ai_family);
        serveraddress6.sin6_family = AF_INET6;
        serveraddress6.sin6_port  = ENB_HTONS(http_server_port);
        /* SPR 16568 FIX START */
        //serveraddress6.sin6_addr.s6_addr = inet_addr(
        //        (const char *)HeNB_SELF_IP);
        inet_pton(AF_INET6 , (const char *)HeNB_SELF_IP,( char *) &serveraddress6.sin6_addr.s6_addr);

        /* SPR 16568 FIX END */
        if( -1 == ENB_SETSOCKOPT(sd,SOL_SOCKET, SO_REUSEADDR, &option, sizeof(UInt32)))
        {
            perror ("Socket set Fails\n");
            /* SPR 18726 FIXED START*/
            //CODE REMOVE

            /* coverity 40623 fixed start*/
            ENB_CLOSE(sd);
            /* coverity 40623 fixed end*/
            /*Coverity 106590 Fix Start*/
            DEALLOCATE(databuf);
            /*coverity 106590 Fix End*/
            /* coverity 106588 fixed start */
            /* SPR 22314 Start */
            TR069_FREE_ADDRINFO(res);
            /* SPR 22314 End */
            /* coverity 106588 fixed end */
            return NULL;
            /* SPR 18726 FIXED END*/

        }
        retbind =
            ENB_BIND(sd, (struct sockaddr *) &serveraddress6, sizeof (serveraddress6));
        if (-1 == retbind)
        {
            /* SPR 21747 FIXED START */
            perror ("Bind Fails first time so going for retry \n");
            TRACE (LOG_CRITICAL, "Acs Connection Listner", "Binding second time");
            for(;;)
            {    
               TR069_USLEEP (2000000);
               retbind =    ENB_BIND(sd, (struct sockaddr *) &serveraddress, sizeof (serveraddress));
               if (-1 == retbind)
               {
                   perror ("Bind Fails while retrying\n");
               }
               else
                   break;
            }       
                        
            /* Code Removed */
            /* SPR 21747 FIXED END */
        }
        ENB_LISTEN (sd, 5);

    }
    else {

        TRACE (LOG_CRITICAL, "Acs Connection Listner", "is an is unknown address format : %d",res->ai_family);
        /* coverity 106588 fixed start */
        /* SPR 22314 Start */
        TR069_FREE_ADDRINFO(res);
        /* SPR 22314 End */
        /* coverity 106588 fixed end */
        return NULL; //Coverity 147334 

    }

    /*SPR 17610 Fixed End*/

    /*Beginning of the Main Server Processing Loop */
    for (;;)
    {
        auth[0] = (UInt8*)"";
        auth[1] = (UInt8*)"";
        TRACE (LOG_CRITICAL, "Acs Connection Listner", "Server is %s ",
                "waiting");
        /*SPR 17610 Fixed Start*/
        if(res->ai_family == AF_INET) {

            len = sizeof(cliaddr);
            connfd = ENB_ACCEPT(sd, (struct sockaddr *) &cliaddr, &len);
            if (connfd < 0)
            {
                perror ("Accept Fails\n");
                /* SPR 18726 FIXED START*/   
                /* coverity 106590 fixed start*/
                DEALLOCATE(databuf);
                /* coverity 106590 fixed end*/
                //CODE REMOVE
                /* coverity 40623 fixed start*/
                ENB_CLOSE(sd);
                /* coverity 40623 fixed end*/
                /* coverity 106588 fixed start */
                /* SPR 22314 Start */
                TR069_FREE_ADDRINFO(res);
                /* SPR 22314 End */
                /* coverity 106588 fixed end */
                return NULL;
                /* SPR 18726 FIXED END*/   

            }

            memset(buf,'\0',IP_ADDR_LEN + 1);
            memset(ip_address,'\0',IP_ADDR_LEN + 1);


            if (ENB_INET_NTOP(AF_INET, &cliaddr.sin_addr, (char*)buf, sizeof (buf)) == NULL) {
                perror("inet_ntop");
                /* SPR 18726 FIXED START*/
                //CODE REMOVE
                //* coverity 106590 fixed start*/
                DEALLOCATE(databuf);
                /* coverity 106590 fixed end*/
                /* coverity 40623 fixed start*/
                ENB_CLOSE(sd);
                /* coverity 40623 fixed end*/
                /* coverity 55781 fixed start*/
                ENB_CLOSE(connfd);
                /* coverity 55781 fixed end*/
                /* coverity 106588 fixed start */
                /* SPR 22314 Start */
                TR069_FREE_ADDRINFO(res);
                /* SPR 22314 End */
                /* coverity 106588 fixed end */

                return NULL;
                /* SPR 18726 FIXED END*/

            }

            TRACE (LOG_CRITICAL, "Acs Connection Listner", "Connection from %s\n",
                    buf);

            ENB_STRNCPY((char*)ip_address, (char*)
                    buf,IP_ADDR_LEN);

        }
        else if(res->ai_family == AF_INET6)  {

            len = sizeof(cliaddr6);
            connfd = ENB_ACCEPT(sd, (struct sockaddr *) &cliaddr6, &len);
            if (connfd < 0)
            {
                perror ("Accept Fails\n");
                /* SPR 18726 FIXED START*/   
                //CODE REMOVE
                /* coverity 106590 fixed start*/
                DEALLOCATE(databuf);
                /* coverity 106590 fixed end*/
                /* coverity 40623 fixed start*/
                ENB_CLOSE(sd);
                /* coverity 40623 fixed end*/
                /* coverity 106588 fixed start */
                /* SPR 22314 Start */
                TR069_FREE_ADDRINFO(res);
                /* SPR 22314 End */
                /* coverity 106588 fixed end */

                return NULL;
                /* SPR 18726 FIXED END*/   

            }

            memset(buf,'\0',IP_ADDR_LEN + 1);
            memset(ip_address,'\0',IP_ADDR_LEN + 1);


            if (ENB_INET_NTOP(AF_INET6, &cliaddr6.sin6_addr, (char*)buf, sizeof (buf)) == NULL) {
                perror("inet_ntop");
                /* coverity 106590 fixed start*/
                DEALLOCATE(databuf);
                /* coverity 106590 fixed end*/

                /* SPR 18726 FIXED START*/
                //CODE REMOVE
                /* coverity 40623 fixed start*/
                ENB_CLOSE(sd);
                /* coverity 40623 fixed end*/
                /* coverity 55781 fixed start*/
                ENB_CLOSE(connfd);
                /* coverity 55781 fixed end*/

                /* coverity 106588 fixed start */
                /* SPR 22314 Start */
                TR069_FREE_ADDRINFO(res);
                /* SPR 22314 End */
                /* coverity 106588 fixed end */

                return NULL;
                /* SPR 18726 FIXED END*/


            }
            ENB_STRNCPY((char*)ip_address, (char*)
                    buf,IP_ADDR_LEN);

        }
        else {

            TRACE (LOG_CRITICAL, "Acs Connection Listner","Acs Connection Listner fail not ipv4 /ipv6" );
            /* SPR 18726 FIXED START*/   
            /* coverity 106590 fixed start*/
            DEALLOCATE(databuf);
            /* coverity 106590 fixed end*/

            /* coverity 106588 fixed start */
            /* SPR 22314 Start */
            TR069_FREE_ADDRINFO(res);
            /* SPR 22314 End */
            /* coverity 106588 fixed end */
            return NULL;
            /* SPR 18726 FIXED END*/   


        }
        /*SPR 17610 Fixed End*/

        //Reading data
        TRACE (LOG_CRITICAL, "Acs Connection Listner", "Ready to read data\n");
        ret = ENB_READ(connfd, databuf, 2048);
        if (ret < 0)
        {
            /* Coverity 55781 + */
            ENB_CLOSE(connfd);
            /* Coverity 55781 - */
            perror ("Read Fails\n");
            // exit (1);
            TRACE (LOG_CRITICAL, "Acs Connection Listner",
                    "ENB_READ Received from client\n");
            continue;	
        }
        databuf[ret] = '\0';
        TRACE (LOG_CRITICAL, "Acs Connection Listner",
                "Received from client\n%s", databuf);
        TRACE (LOG_CRITICAL, "Acs Connection Listner",
                "FINISHED SERVING ONE CLIENT\n");
        ipr_cwmp_decoder(databuf, auth);

        TRACE (LOG_CRITICAL, "Acs Connection Listner",
                "IN SERVER USERNAME = %s \t PASSWORD = %s\n", auth[0], auth[1]);
        if (PNULL != auth[0])
        {
            UInt8 * passd = ipr_cwmp_getConnectionRequestPassword();
            UInt8 * user_name = ipr_cwmp_getConnectionRequestUsername();
            if((NULL != passd)   && (NULL !=  user_name))	
            {

                //cov fix
                if (skip_authenticate || ((ENB_STRNCMP((char*)auth[0], (const char*)user_name, strlen((const char*)user_name) + 1) == 0)
                            && (( ENB_STRNCMP((char*)auth[1], (const char*)passd, strlen((const char*)passd) +1)) == 0 )
                            ))
                {
                    if (g_ipr_cwmp_con_req > 0 && g_session_manager_session_open)
                    {
                        ipr_cwmp_send_headers(connfd, 503, (UInt8*)"Service Unavailable", PNULL,
                                (UInt8*)"text/plain", 0, 0, ip_address);
                        g_ipr_cwmp_con_req_counter = 0;
                    }
                    else if (g_ipr_cwmp_con_req > 0 && !g_session_manager_session_open)
                    {
                        g_ipr_cwmp_con_req_counter++;
                        if (g_ipr_cwmp_con_req_counter > 100)
                        {
                            ipr_cwmp_send_headers(connfd, 503, (UInt8*)"Service Unavailable", PNULL,
                                    (UInt8*)"text/plain", 0, 0, ip_address);
                        }
                        else
                        {
                            ipr_cwmp_send_headers (connfd, 200, (UInt8*)"OK", PNULL, (UInt8*)"text/plain", 0,
                                    0, ip_address);
                        }
                    }
                    else
                    {
                        ipr_cwmp_send_headers(connfd, 200, (UInt8*)"OK", PNULL, (UInt8*)"text/plain", 0, 0,
                                ip_address);
                        g_ipr_cwmp_con_req_counter = 0;
                    }
                    ENB_STRNCPY((char*)EventCode, "6 CONNECTION REQUEST", 22);
                    newEvent = (sEventStruct *) ALLOCATE(sizeof (sEventStruct));
                    if (newEvent)
                    {
                        newEvent->EventCode = ALLOCATE(EVENT_CODE_SIZE);
                        if (newEvent->EventCode != PNULL)
                        {
                            ENB_STRNCPY((char*)newEvent->EventCode, "6 CONNECTION REQUEST",
                                    EVENT_CODE_SIZE);
                            newEvent->CommandKey = ALLOCATE (COMMAND_KEY_SIZE);
                            if(NULL ==newEvent->CommandKey)
                            {
                                exit (1);
                            }
                            ENB_STRNCPY((char*)newEvent->CommandKey, "", 2);
                        }
                        lock_EQueue();
                        if (g_ipr_cwmp_con_req == 0)
                        {
                            ipr_cwmp_addEvent(newEvent, PNULL);
                            g_ipr_cwmp_con_req++;
                        }
                        unlock_EQueue();
                    }
                }
                else
                {
                    ipr_cwmp_send_headers(connfd, 401, (UInt8*)"OK", PNULL, (UInt8*)"text/plain", 0, 0,
                            ip_address);
                }
            }

            /* SPR 20480 FIXED START */
            else if(skip_authenticate)
            {
                ipr_cwmp_send_headers(connfd, 200, (UInt8*)"OK", PNULL, (UInt8*)"text/plain", 0, 0,
                        ip_address);
                g_ipr_cwmp_con_req_counter = 0;
                ENB_STRNCPY((char*)EventCode, "6 CONNECTION REQUEST", 22);
                newEvent = (sEventStruct *) ALLOCATE(sizeof (sEventStruct));
                if (newEvent)
                {
                    newEvent->EventCode = ALLOCATE(EVENT_CODE_SIZE);
                    if (newEvent->EventCode != PNULL)
                    {
                        ENB_STRNCPY((char*)newEvent->EventCode, "6 CONNECTION REQUEST",
                                EVENT_CODE_SIZE);
                        newEvent->CommandKey = ALLOCATE (COMMAND_KEY_SIZE);
                        if(NULL ==newEvent->CommandKey)
                        {
                            /* coverity 106590 fixed start*/
                            DEALLOCATE(databuf);
                            /* coverity 106590 fixed end*/

                            exit (1);
                        }
                        ENB_STRNCPY((char*)newEvent->CommandKey, "", 2);
                    }
                    lock_EQueue();
                    if (g_ipr_cwmp_con_req == 0)
                    {
                        ipr_cwmp_addEvent(newEvent, PNULL);
                        g_ipr_cwmp_con_req++;
                    }
                    /* SPR 21563_1 FIXED START */
                    else
                    {
                      DEALLOCATE(newEvent->CommandKey);
                      DEALLOCATE(newEvent->EventCode);
                      DEALLOCATE(newEvent);
                    }
                      /* SPR 21563_1 FIXED END */
                    unlock_EQueue();
                }
                else
                {
                    ipr_cwmp_send_headers(connfd, 401, (UInt8*)"OK", PNULL, (UInt8*)"text/plain", 0, 0,
                            ip_address);
                }
            }
            else 
            {
                ipr_cwmp_send_headers(connfd, 401, (UInt8*)"OK", PNULL, (UInt8*)"text/plain", 0, 0,
                        ip_address);
            }        

            /* SPR 20480 FIXED END */

        }
        else
        {
            ipr_cwmp_send_headers(connfd, 401, (UInt8*)"OK", PNULL, (UInt8*)"text/plain", 0, 0,
                    ip_address);
        }
        ENB_CLOSE(connfd);
        TRACE (LOG_CRITICAL, "Acs Connection Listner", "Exiting in %s ",
                __FUNCTION__);

    }
    DEALLOCATE(databuf);
    ENB_CLOSE(sd);
}
	
/****************************************************************************
 * Function Name  : ipr_cwmp_send_headers
 * Inputs         : UInt32 connfd, UInt32 status, UInt8 *title, UInt8 *extra, UInt8 *mime
 *                  UInt32 length, time_t date, UInt8 *ip_address
 * Outputs        : None
 * Returns        : Void
 * Description    : This function is used to send http header
 *
 ****************************************************************************/
	
void ipr_cwmp_send_headers(UInt32 connfd, UInt32 status, UInt8 *title, UInt8 *extra, UInt8 *mime,
		UInt32 length, time_t date, UInt8 *ip_address)
{
	time_t now = {0};
	UInt8 *timebuf= NULL;

	timebuf = (UInt8*) ALLOCATE(MAX_STR_LEN + MAX_STR_LEN + 1);
	if(NULL ==timebuf)
	{
		return ;
	}

	UInt8 *buffer = NULL;
	buffer = (UInt8*) ALLOCATE(MAX_HEADER_BUFFER_LEN + 1);
	if(NULL ==buffer)
	{
		DEALLOCATE(timebuf);
		return ;
	}

	ENB_SNPRINTF((char*)buffer, MAX_HEADER_BUFFER_LEN, "%s %d %s\r\n", PROTOCOL, status,
			title);
	ENB_SNPRINTF((char*)buffer + ENB_STRLEN((char*)buffer), ENB_STRLEN((char*)buffer), "Server: %s\r\n",
			SERVER);
	now = time (PNULL);
	struct tm *tr_time = gmtime(&now);
	if(NULL == tr_time)
	{
		DEALLOCATE(buffer);
		DEALLOCATE(timebuf);
		return ;

	}


	if (ENB_STRFTIME((char*)timebuf,( MAX_STR_LEN + MAX_STR_LEN + 1 ), RFC1123FMT, tr_time) == 0) {
		fprintf(stderr, "strftime returned 0");
		exit(EXIT_FAILURE);
	}

	ENB_SNPRINTF((char*)buffer + ENB_STRLEN((char*)buffer), ENB_STRLEN((char*)buffer), "Date: %s\r\n",
			timebuf);
	if (extra)
		ENB_SNPRINTF((char*)buffer + ENB_STRLEN((char*)buffer), ENB_STRLEN((char*)buffer), "%s\r\n", extra);
	if (mime)
		ENB_SNPRINTF((char*)buffer + ENB_STRLEN((char*)buffer), ENB_STRLEN((char*)buffer),
				"Content-Type: %s\r\n", mime);
	ENB_SNPRINTF((char*)buffer + ENB_STRLEN((char*)buffer), ENB_STRLEN((char*)buffer),
			"Content-Length: %d\r\n", length);
	if (date != -1)
	{
		struct tm * tr_time_1 = gmtime (&now);
		if(NULL == tr_time_1)
		{
			DEALLOCATE(buffer);
			DEALLOCATE(timebuf);
			return ;

		}


		if (ENB_STRFTIME((char*)timebuf, ( MAX_STR_LEN + MAX_STR_LEN + 1 ), RFC1123FMT, tr_time_1) == 0) {
			ENB_FPRINTF(stderr, "strftime returned 0");
			exit(EXIT_FAILURE);
		}
		ENB_SNPRINTF((char*)buffer + ENB_STRLEN((char*)buffer), ENB_STRLEN((char*)buffer),
				"Last-Modified: %s\r\n", timebuf);
	}
	if (DIGEST)
	{
		ipr_cwmp_digest_http_header(buffer, ip_address);
	}
	if (BASIC)
	{
		ENB_SNPRINTF((char*)buffer + ENB_STRLEN((char*)buffer), ENB_STRLEN((char*)buffer),
				"WWW-Authenticate: Basic realm=\"ACS CONNECTION REQUEST\"\n");
	}
	ENB_SNPRINTF((char*)buffer + ENB_STRLEN((char*)buffer), ENB_STRLEN((char*)buffer),
			"Connection: close\r\n");
	ENB_SNPRINTF((char*)buffer + ENB_STRLEN((char*)buffer), ENB_STRLEN((char*)buffer), "\r\n");

	if( -1 == ENB_WRITE(connfd, buffer, ENB_STRLEN((char*)buffer)))
    {
        /* Coverity 109688 fix start */
        TRACE (LOG_MAJOR,"ACS_CONN_LISTENER","Error in ENB_WRITE, error_number %d: %s",errno,strerror(errno));	
        /* Coverity 109688 fix end */
    }

	DEALLOCATE(buffer);
	DEALLOCATE(timebuf);

}

/****************************************************************************
 * Function Name  : ipr_cwmp_Authentication_Parser 
 * Inputs         : UInt8* buffer 
 *                  
 * Outputs        : None
 * Returns        : Void
 * Description    : This function is for future use 
 *
 ****************************************************************************/
	
void ipr_cwmp_Authentication_Parser (UInt8* buffer)
{
	UInt8* token = PNULL;
	token = (UInt8*)ENB_STRTOK((char*)buffer, " \n");
	while (token)
	{
	    TRACE (LOG_CRITICAL, "Acs Connection Listner", "Current token: %s.\n",
    	token);
    	token = (UInt8*)ENB_STRTOK((char*)PNULL, "\n");
	}
}
	
	
/********************************************************************************************
*
*       Function Name : ipr_cwmp_start_server
*
*       Description   : This function is used to start the http server.
*
*       Returns       : Returns IPR_CWMP_FAILURE on failure and IPR_CWMP_SUCCESS on success
*
**********************************************************************************************/
	
ipr_cwmp_ret_t ipr_cwmp_start_server (void)
{
	pthread_t ntid = {0};		/*Thread Identifier */
	UInt32 status = 0;
	/*Creating a thread */
	TRACE (LOG_CRITICAL, "Acs Connection Listner", "In start_Server....\n");

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, PTHREAD_STACK_SIZE);
	status = pthread_create (&ntid, &attr, ipr_cwmp_http_server, PNULL);
	pthread_attr_destroy(&attr);
	if (status != 0)
	{
		TRACE (LOG_CRITICAL, "Acs Connection Listner",
				"Error in Creating Thread\n");
		exit (status);
	}
	sleep (2);
	TRACE (LOG_CRITICAL, "Acs Connection Listner", "Exiitng from %s\n",
			__FUNCTION__);
	return IPR_CWMP_SUCCESS;
}
	
	
/********************************************************************************
*
*       Function Name : ipr_cwmp_time_stamp
*
*       Description   : This function is used to get the timestamp
*
*       Returns       : Returns timestamp
*
**********************************************************************************/
	
Char8* ipr_cwmp_time_stamp (void)
{
	Char8 *buffer = NULL;
	buffer = (Char8 *) ALLOCATE( 31);
	if(NULL == buffer)
	{
		return NULL;
	}
	struct timeval tv;
	time_t curtime;
    /* coverity 94850 fix start */
	ENB_GETTIMEOFDAY(&tv);
    /* coverity 94850 fix end */
	curtime = (time_t)tv.tv_sec;

	struct tm * tr_time = localtime(&curtime);
	if(NULL == tr_time)
	{
		DEALLOCATE(buffer);
		return NULL;
	}

	strftime((char*)buffer, 30, "%m-%d-%Y  %T.",tr_time);
	itoa(tv.tv_usec, &(buffer[ENB_STRLEN((char*)buffer)]));
	return buffer;
}
	
/********************************************************************************
*
*       Function Name : ipr_cwmp_digest_http_header
*
*       Description   : This function is used to digest http header.
*                        and to create the event list.
*
*       Returns       : Returns nothing
*
**********************************************************************************/
	
void ipr_cwmp_digest_http_header(UInt8* buffer, UInt8* ip_address)
{
	UInt8 nonce[STR_LEN + 1] = {'\0'};
	UInt8* temp_str = PNULL;
	Char8* time_str = PNULL;
	UInt8 temp_hex[STR_LEN + 1] = {'\0'};
	time_str = ipr_cwmp_time_stamp ();
	if(NULL == time_str)
	{
		return ;
	}
	temp_str = (UInt8*) ALLOCATE(ENB_STRLEN((char*)time_str) + 1 + ENB_STRLEN((char*)ip_address) + 1);
	if(NULL ==temp_str)
	{
		DEALLOCATE(time_str);
		return ;
	}
	ENB_STRNCPY((char*)temp_str, (char*)time_str, ENB_STRLEN((char*)time_str) + 1);
	ENB_STRNCAT((char*)temp_str, ":", 2);
	ENB_STRNCAT((char*)temp_str, (char*)ip_address, ENB_STRLEN((char*)ip_address) + 1);
	yeMd5Digest(temp_str, ENB_STRLEN((char*)temp_str) + 1, temp_hex);
	ipr_cwmp_hex_2_str(temp_hex, nonce);
	ENB_SNPRINTF((char*)buffer + ENB_STRLEN((char*)buffer), ENB_STRLEN((char*)buffer) + 1,
			"WWW-Authenticate: Digest realm=\"ACS CONNECTION REQUEST\",");
	ENB_SNPRINTF((char*)buffer + ENB_STRLEN((char*)buffer), ENB_STRLEN((char*)buffer) + 1, "nonce=\"%s\"\n",
			nonce);
	DEALLOCATE(temp_str);
	DEALLOCATE(time_str);
    /*  coverity 86585 fix start */
	/* code removed */
    /*  coverity 86585 fix end */
}

/*SPR 16545 Fixed Start*/
/****************************************************************************
 *  * Function Name  : gettimeofday_wrapper
 *   * Inputs         : TimeSpec
 *    * Outputs        : ts_p
 *     * Returns        : Void
 *      * Description    : This function is used
 *      to get the h/w clock time.
 *****************************************************************************/
/*  coverity 94850 fix start */
S32 tr069_gettimeofday_wrapper(struct timeval *p_tv)
/*  coverity 94850 fix end */
{
    S32 retVal = 0;
    struct timespec currentTimeStamp = {0};
    /* This parameter is currently not used
     * */
    /*  coverity 94850 fix start */
    /* code removed */
    /*  coverity 94850 fix end */
    retVal = clock_gettime(CLOCK_MONOTONIC_RAW,&currentTimeStamp);
/*SPR 21001 Changes end */
    p_tv->tv_sec = currentTimeStamp.tv_sec;
    p_tv->tv_usec = currentTimeStamp.tv_nsec / 1000;
    return retVal;
}

/*SPR 16545 Fixed End*/

