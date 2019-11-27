#include        <pthread.h>
#include        <stdio.h>
#include        <stdlib.h>
#include <string.h>
#include 	<math.h>
#include        <errno.h>
#include        <string.h>
#include        <sys/types.h>
#include        <sys/socket.h>
#include        <netinet/in.h>
#include        <arpa/inet.h>
#include        <sys/wait.h>
#include        <fcntl.h>
#include        <unistd.h>
#include <time.h>
#include <event_handler.h>
#include <ManagementServerModel.h>
#include <ipr_cwmp_read_config.h>


#include "strfun.h"
#include "logger.h"


#define MYPORT 15284
#define DEBUGGER_MYPORT "DEBUGGER_MYPORT"
//#define MYPORT 19737
#define SERVER "webserver/1.1"
#define PROTOCOL "HTTP/1.1"
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"
#define PORT 80

#define DIGEST 1
#define BASIC (1 != DIGEST)
#define MAX_BUFFER_LENGTH 2048
#define MAX_HEADER_BUFFER_LEN 5000
#define IP_ADDR_LEN 40
#define STR_LEN 32
#define MAX_STR_LEN 64

void send_debug_response(int connfd, char *title);


/********************************************************************************
*
*       Function Name : http_server
*
*       Description   : This function is used to DEQUEUE the nodes form event queue
*                        and to create the event list.
*
*       Returns       : Returns event List
*                        If no event in EVENT QUEUE then returns NULL
*
**********************************************************************************/

void * debugger_server(void * args)
{
	int     sd,connfd,retbind;
	struct  sockaddr_in serveraddress,cliaddr;
	socklen_t len;
	int ret;
	char buf[MAX_BUFFER_LENGTH] = {'\0'};
	char *databuf = NULL;
    if(NULL == args)
    {
	   TRACE (LOG_CRITICAL ,"DEBUGGER", "Args NULL\n");
    }    
	databuf = (char *)ALLOCATE(MAX_BUFFER_LENGTH +1 );
	if(NULL == databuf)
	{
		TRACE (LOG_CRITICAL ,"DEBUGGER", "Entering in %s Malloc Failed",__FUNCTION__);
		return NULL;
	}
	memset(databuf,0, (MAX_BUFFER_LENGTH +1 ));
	int port = 0;
	char *str = NULL;
	char *str1 = NULL;
	char *name = NULL;
	char *value= NULL;

	TRACE (LOG_CRITICAL ,"DEBUGGER", "Entering in %s ",__FUNCTION__);

	TRACE (LOG_CRITICAL ,"DEBUGGER", " debugger port :%d", debugger_port);

	if (debugger_port == 0)
	{
		TRACE (LOG_CRITICAL ,"DEBUGGER", "Exiting from %s port is not exported",__FUNCTION__);
		DEALLOCATE(databuf);
		return NULL;
	}
	port = debugger_port;
	ENB_PRINTF ("\n port = %d\n", port);

	sd = socket( AF_INET, SOCK_STREAM, 0 );
	if (sd < 0 )
	{
		perror( "socket" );
		exit( 1 );
	}

	memset( &serveraddress, 0, sizeof(serveraddress) );
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(port);//PORT NO
	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);//ADDRESS
	retbind=bind(sd,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
	if(-1 == retbind)
	{
		perror("Bind Fails\n");
		exit(1);
	}
	listen(sd,5);
	/*Beginning of the Main Server Processing Loop*/

	for (;;)
	{
		TRACE (LOG_CRITICAL ,"DEBUGGER", "Server is %s ","waiting");
		len=sizeof(cliaddr);
		connfd=accept(sd,(struct sockaddr*)&cliaddr,&len);
		if (connfd < 0)
		{
			close(sd);
			DEALLOCATE(databuf);
			return NULL;
		}
		TRACE (LOG_CRITICAL ,"DEBUGGER", "Connection from %s\n",
				inet_ntop(AF_INET,&cliaddr.sin_addr,buf,sizeof(buf)));
		TRACE (LOG_CRITICAL ,"DEBUGGER","Ready to read data\n");
		while (strcmp(databuf,"quit")!=0)
		{
			ret=read(connfd, databuf, 2048);
			if (ret <= 0)
			{
				TRACE (LOG_CRITICAL ,"DEBUGGER", "%s read failed",__FUNCTION__);
				close(sd);
				close(connfd);
				DEALLOCATE(databuf);
				return NULL;
			}
			databuf[ret]='\0';

			if (strcmp(databuf,"quit\n")==0 || strcmp(databuf,"quit\r\n")==0 ){
				break;
			}
			str = strtok (databuf, "\n");
			if(str != NULL)
			{
				str1 = strchr ( str , '=');
				name =  strtok(str,"=");
			}
			if(str1 != NULL)
				value =  strtok(str1+1,"=");

			if (str1 != NULL)
			{

				ENB_PRINTF ( "\nName=  : %s\nValue=%s\n",  name, value);
				if ( name!=NULL && strstr (name , "LOG_LEVEL") != NULL)
				{
					if (NULL != value)
					{
						logger_setLogLevel(atoi(value));
					}
				}
				else if( name!=NULL)
				{
					logger_addModule(name , atoi(value));
				}
			}

			TRACE (LOG_CRITICAL ,"DEBUGGER", "Received from client\n%s", databuf);

			send_debug_response(connfd, "SUCCESS");
		}
		close(connfd);
		TRACE (LOG_CRITICAL ,"DEBUGGER", "Exiting in %s ",__FUNCTION__);
	}
	DEALLOCATE(databuf);
	close(sd);
}




/********************************************************************************
*
*       Function Name : send_headers
*
*       Description   : This function is used to DEQUEUE the nodes form event queue
*                        and to create the event list.
*
*       Returns       : Returns event List
*                        If no event in EVENT QUEUE then returns NULL
*
**********************************************************************************/

void send_debug_response(int connfd,  char *title)
{
	char  *buffer = NULL;
	buffer = (char *)ALLOCATE(MAX_HEADER_BUFFER_LEN +1 );
	if(NULL == buffer)
	{
		exit(0);
	}
	snprintf(buffer, MAX_HEADER_BUFFER_LEN, " %s\r\n>", title);
	if( -1 == write(connfd, buffer, strlen(buffer)))
	{
		exit(0);
	}
	DEALLOCATE(buffer);
}



/********************************************************************************
*
*       Function Name : start_server
*
*       Description   : This function is used to DEQUEUE the nodes form event queue
*                        and to create the event list.
*
*       Returns       : Returns event List
*                        If no event in EVENT QUEUE then returns NULL
*
**********************************************************************************/

int start_debugger(void)
{
        pthread_t ntid; /*Thread Identifier*/
        int status;
        /*Creating a thread*/
        TRACE (LOG_CRITICAL ,"DEBUGGER","In start_Server....\n");
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setstacksize(&attr, PTHREAD_STACK_SIZE);
        status=pthread_create(&ntid,&attr,debugger_server,NULL);
        pthread_attr_destroy(&attr);
        if (status != 0)
        {
                TRACE (LOG_CRITICAL ,"DEBUGGER","Error in Creating Thread\n");
                exit(status);
        }

	
        sleep(2);
	TRACE (LOG_CRITICAL ,"DEBUGGER","Exiitng from %s\n",__FUNCTION__);
        return 0;

}


