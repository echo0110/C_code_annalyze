/********************************************************************************      
*********************************************************************************
**      FILE NAME:
**              logger_main.c
**
**      DESCRIPTION:
**              This file contains the main function of Logger.
**              Set ts=4 for best viewing :-)
**
**
**      DATE                    AUTHOR                          REF
        REASON
**      ----                    ------                          ---
        ------
**      21Jan2003               svaid                           ---
**
**       Copyright (C) 2006 Aricent Inc . All Rights Reserved
******************************************************************************/

#include "logger.h"
#include "logger_init.h"

unsigned short  	port;
S8		 			ipa_s[64];
S16                 server_fd = -1;
U8                  buffer[1000];
logger_cfg_params_t	cfg_params;
FILE			*p_log_file=NULL;

void
logger_port_trace
(
	const S8      *fmt_str,
	...
)
{
        va_list                 ap;
        FILE                    *tmp_log_file;

		tmp_log_file = stdout;

        fprintf(tmp_log_file,":%s:", "LOGGER_TRACE");
        va_start(ap, fmt_str);
        vfprintf(tmp_log_file, fmt_str, ap );
        va_end(ap);
        fprintf(tmp_log_file, "\n");
        fflush(tmp_log_file);
}


static void
logger_ready_trace
(
        void
)
{
        printf("------------------ LOGGER STARTS --> WAITING TO RECV MSGS -------------\n");

        printf("        ------------------ READY TO LOG --------------------\n"
);
}


void    makeaddress(const S8 *s, struct sockaddr_in *address, unsigned long defaultaddress )
{
        memset( address, 0, sizeof(*address) );
        address->sin_family = AF_INET;

        if( strchr(s, ':') ) 
		{
                sscanf( s, "%[^:]:%hu", ipa_s, &port );
                address->sin_port = htons( port );
                address->sin_addr.s_addr = resolve(ipa_s);
        } 
		else
		{
                address->sin_port = port = htons( atoi(s) );
                address->sin_addr.s_addr = defaultaddress;
        }
}

void    printaddress( S8 *s, const struct sockaddr_in *address )
{
        unsigned long   ipa = ntohl(address->sin_addr.s_addr);
        unsigned short  port = ntohs(address->sin_port);

        sprintf( s, "%u.%u.%u.%u:%hu",
                        YGETBYTE(ipa,3),
                        YGETBYTE(ipa,2),
                        YGETBYTE(ipa,1),
                        YGETBYTE(ipa,0),
                        port );
}

void
logger_open_log_file
(
	void
)
{
	S8	*log_file_name=NULL;
	
	log_file_name = getenv("LOG_FILE_NAME");
	if( log_file_name == NULL )
	{
		printf("LOG File Name Unknown!! Cann't Proceed.., Logging Disabled\n"); 
		exit(0);
	}
	else
	{
		if(p_log_file == NULL)
		{
			p_log_file = fopen(log_file_name, "a");
			if ( p_log_file == NULL)
			{
				puts("Cannot Open File");
				exit(0);
			}
		}
	}
}

int 
main
(
	int argc,
	char *argv[]
)
{
	 fd_set          	  	read_fd,save_fd;
	 S8				data[80];
	 struct sockaddr_in      	address;
	 logger_boolean_t		ret=0;
#ifdef TCP
     S16                     	i,free_index,count,index;
	 logger_boolean_t		flag=0;
#endif

	
	 printf("Logger IP:Port\tNum_Client_Processes\n");
	 if ( argc != 3 )
	 {
		 puts("Insufficient arguments");
		 exit(0);
	 }
	 printf("%s\t%s\t%s\t\n",argv[0],argv[1],argv[2]);
	 
	 printf("Log Server: %s\n", argv[1] );
	 makeaddress( argv[1], &address, htonl(INADDR_ANY) );
	 printf("Logger ---> Address:Port\n");
	 printaddress( data, &address);
	 printf("%s\n",data);

	 cfg_params.num_client_processes = atoi(argv[2]);
#ifdef TCP
	 cfg_params.p_fd = (S32 *) malloc( sizeof(S32) * cfg_params.num_client_processes);
	 for(i=0;i<cfg_params.num_client_processes;i++)
	 {
		 *(cfg_params.p_fd + i) = -1;
	 }
#endif

	 ret = logger_init_glob_data();
	 if(ret == 0)
	 {
		 printf("Logger Initialisation Failed\n");
		 exit(0);
	 }

	 logger_open_log_file();
	
	 server_fd = logger_server_start(port);
	 printf("Log Server Fd: %d\n",server_fd);

	 FD_ZERO(&save_fd);
	 FD_SET(server_fd, &save_fd);

	 read_fd = save_fd;
	 logger_ready_trace();
#ifdef	TCP
	 while ( select( FD_SETSIZE, &read_fd, NULL, NULL, NULL) > 0 )
	 {
		if (FD_ISSET(server_fd, &read_fd) && (server_fd != -1))
		{
			printf("Server Fd Set\n");
			flag = LOGGER_FALSE;
			for (free_index = 0;free_index<cfg_params.num_client_processes;free_index++)
            {
	 	               if (cfg_params.p_fd[free_index] == -1)
                       {
					       flag = LOGGER_TRUE;
                           break;
	                   }
            }
			if(flag)
			{
				cfg_params.p_fd[free_index] = logger_wait_for_client_connection(server_fd);
			}
			else
			{
				printf("No more process can further connect to Logger\n");
			}
            if (cfg_params.p_fd[free_index] < 0)
            {
                printf("Logger : Client Connection Failed");
            }
			else
			{
				printf("Logger Server Accepted a New Client Connection\n");
                FD_SET(cfg_params.p_fd[free_index], &save_fd);
			}
         }

		for (count = 0; count < cfg_params.num_client_processes ; count++)
        {
			if ((cfg_params.p_fd[count] != -1) && FD_ISSET(cfg_params.p_fd[count], &read_fd))
        	{
	        	printf("Sock Fd %d Set\n",count);
	            if( ! logger_recv(cfg_params.p_fd[count]))
				{
					close(cfg_params.p_fd[count]);
	               	FD_CLR(cfg_params.p_fd[count], &read_fd);
		            FD_CLR(cfg_params.p_fd[count], &save_fd);
                    for (index = 0; index < cfg_params.num_client_processes;index++)
                    {
                	    if (cfg_params.p_fd[index] == cfg_params.p_fd[count])
                        {
                    		break;
                        }
                    }
		            cfg_params.p_fd[index] = -1;
				}
				else
				{
	               	FD_CLR(cfg_params.p_fd[count], &read_fd);
				}
       		}
		}
        read_fd = save_fd;
        }
#endif
#ifdef UDP
	 while ( select( FD_SETSIZE, &read_fd, NULL, NULL, NULL) > 0 )
	 {
		if (FD_ISSET(server_fd, &read_fd) && (server_fd != -1))
        {
 			printf("Server Fd Set\n");
	        if( ! logger_recv(server_fd))
			{
				printf("Received Message Invalid\n");
			}
			else
			{
				printf("Received Message Processed\n");
			}
		}
      }
#endif
	free(p_glob_data);
	fclose(p_log_file);
    return 1;
}
		
	 
 

	 

	 





