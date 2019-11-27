/********************************************************************************      
*********************************************************************************
**      FILE NAME:
**              logger_nw_recv.c
**
**      DESCRIPTION:
**              This file contains the wrapper functions for logger tcp services.
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
#include "logger_print.h"

U16	sender;

static void
logger_hex_dump
(
        U8              *p_buffer,
        U16             length
)
{
        S16     i = 0, count = 0;

        printf("\nHex Dump of Message follows \n");

        for(i = 0; i < length; i++)
        {
                printf("%4x", *p_buffer++);

                if(count++ == 15)
                {
                        printf("\n");

                        count   = 0;
                }
        }

        printf("\n");
}

S16 
logger_server_start
(
	unsigned short port
)
{
	S16 serverfd = -1;
	serverfd = setup_server(port);

	return(serverfd);
}

#ifdef TCP
S16
logger_wait_for_client_connection
(
	S16 server_fd
)
{
	S16 newsockfd = -1;
	newsockfd = make_connection( server_fd);

	return(newsockfd);
}
#endif

unsigned char   *yfGetBuf( unsigned char *buf, unsigned int count, unsigned char *newbuffer )
{
        unsigned char   *P = buf;
        unsigned int    i;

        if (newbuffer == NULL)
                return (P+count);

        if( count > sizeof(unsigned long) ) {
                memcpy( newbuffer, P, count );
                P += count;
        } else {
                for( i=0; i<count; i++ ) {
                        newbuffer[i] = *P++;
                }
        }

        return P;
}

unsigned char   *yfGetU16( unsigned char *buf, unsigned short *value )
{
        unsigned short  cvalue;
        unsigned char   *Q = (unsigned char *)(&cvalue);
        unsigned char   *P = buf;

        if (value == NULL)
                return (P + sizeof(cvalue));

        P = yfGetBuf( P, sizeof(cvalue), Q );
        *value = ntohs(cvalue);
        return P;
}

unsigned short  yfGetU16At( unsigned char *buf, int offset )
{
        unsigned short  cvalue;

        yfGetU16( buf+offset, &cvalue );
        return cvalue;
}

unsigned char   *yfGetU32( unsigned char *buf, unsigned long *value )
{
        unsigned long   cvalue;
        unsigned char   *Q = (unsigned char *)(&cvalue);
        unsigned char   *P = buf;

        if (value == NULL)
                return (P + sizeof(cvalue));

        P = yfGetBuf( P, sizeof(cvalue), Q );
        *value = ntohl(cvalue);
        return P;
}

unsigned long   yfGetU32At( unsigned char *buf, int offset )
{
        unsigned long   cvalue;

        yfGetU32( buf+offset, &cvalue );
        return cvalue;
}

S16 
logger_recv
(
	S32	p_fd	
)
{
	U16	buf_len=0, bytes_read=0 , attrlen,receiver,payloadlen;
	S16 	mod_index, api_index, src_index, dst_index;

	printf("LOGGER:Entering logger_recv\n");
#ifdef TCP
	bytes_read = nw_read( p_fd, buffer, SH_HDRSIZE);
	if( bytes_read != SH_HDRSIZE)
	{
		printf("Shell Header recved is not recognised\n");
		return 0;	
	}
	buf_len += bytes_read;
#endif
#ifdef UDP
	bytes_read = nw_peek( p_fd, buffer, SH_HDRSIZE);
	if( bytes_read != SH_HDRSIZE)
	{
		printf("Shell Header recved is not recognised\n");
		return 0;	
	}
#endif
	printf("Shell Header Read: %d bytes\n",bytes_read);
	logger_hex_dump(buffer, SH_HDRSIZE);
	bytes_read=0;

	sender = SH_GETSOURCE(buffer);
	receiver = SH_GETDESTINATION(buffer);

	if (receiver != LOGGER_MODULE_ID)
	{
		printf("Message not meant for Logger..Incorrect Receiver\n");
		return 0;
	}
	attrlen = SH_GETATTRSIZE(buffer);
	payloadlen = SH_GETPAYLOADSIZE(buffer);
#ifdef TCP
	bytes_read = nw_read( p_fd, buffer + SH_HDRSIZE, attrlen + payloadlen);
	if( bytes_read != (attrlen + payloadlen))
	{
		printf("Logger API recved is not recognised\n");
		return 0;
	}
	printf("Logger API Read: %d bytes\n",bytes_read); 
	buf_len += bytes_read;
#endif
#ifdef UDP
	bytes_read = nw_read( p_fd, buffer, SH_HDRSIZE + attrlen + payloadlen);
	if( bytes_read != (SH_HDRSIZE + attrlen + payloadlen))
	{
		printf("Logger API recved is not recognised\n");
		return 0;
	}
	printf("Logger API Read: %d bytes\n",(bytes_read - (SH_HDRSIZE + payloadlen))); 
	buf_len += bytes_read;
#endif
	logger_hex_dump(buffer + SH_HDRSIZE, LOGGER_API_SIZE);

	printf("Total %d bytes of buffer recved\n",buf_len);

	logger_get_api_traversal_info(buffer + SH_HDRSIZE);

	if( logger_search(&mod_index , &api_index, &src_index, &dst_index) )
		logger_print_and_log_to_file(mod_index, api_index, src_index, dst_index);
	else
	{
		printf("EITHER MODULE or API INFO UNKNOWN TO LOGGER");
		return 0;
	}
	printf("LOGGER:Exiting logger_recv\n");
	return 1;
}

		




	
