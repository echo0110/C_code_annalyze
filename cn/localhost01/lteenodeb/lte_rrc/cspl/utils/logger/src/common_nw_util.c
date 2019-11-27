/*******************************************************************************
**      FILE NAME:
**              common_nw_util.c 
**
**      DESCRIPTION:
**              This file contains all the functions realting to the socket connections.
**
**      DATE       NAME         REFERENCE       REASON
**      -----------------------------------------------------
**	30Jan2003  svaid			N/W utilities(TCP,UDP)	
**
**   Copyright (C) 2006 Aricent Inc . All Rights Reserved
********************************************************************************/
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

/*
**======Prototypes (static)
*/
extern int errno;

/****************************************************************************
**    FUNCTION :
**              setup_client
**
****************************************************************************
**    DESCRIPTION:
**              This function is used to setup client
****************************************************************************/
int
setup_client
( char * serv_ipstr, 
  u_short serv_tcpport )
{
	int			sockfd;
	long			serv_ipaddr;
	struct sockaddr_in	saddr;
#ifdef UDP
	struct sockaddr_in	me;
#endif
	
	if ((serv_ipaddr = inet_addr (serv_ipstr)) == -1)
	{
		fprintf (stderr, "Invalid IP address\n");
		exit (1);
	}

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons (serv_tcpport);
	memcpy ((char *)&saddr.sin_addr.s_addr, 
		(char *)&serv_ipaddr, sizeof (long));
#ifdef TCP
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return (-1);
	}
#endif
#ifdef	UDP 
	if ((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		return (-1);
	}

	me.sin_family = AF_INET;
	me.sin_port = htons(0);
	me.sin_addr.s_addr = htonl(INADDR_ANY);
    	if (bind( sockfd, (struct sockaddr *) &me, sizeof(me) ) < 0) 
	{
	      perror("Unable to Bind (client)");
	      close(sockfd);
	      return -1;
	}

#endif

	if (connect (sockfd, (struct sockaddr *)&saddr, 
		     sizeof (struct sockaddr_in)) < 0)
    {
		fprintf(stderr,"\n Unable to connect to IP :[%s]::Port [%d] "
					"due to %s \n", serv_ipstr,serv_tcpport,strerror(errno));
		close (sockfd);
		return (-1);
	}

	return sockfd;
}


/****************************************************************************
**    FUNCTION :
**              setup_server
**
****************************************************************************
**    DESCRIPTION:
**              This function is used to setup server 
****************************************************************************/
int
setup_server( u_short serv_tcpport )
{
	const int  on = 1;		/* for setsockopt() */
	int	sock;
	struct sockaddr_in	saddr;

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons (serv_tcpport);
	saddr.sin_addr.s_addr = htonl (INADDR_ANY);
#ifdef TCP
	if ((sock = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return (-1);
	}
#endif
#ifdef	UDP 
	if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		return (-1);
	}
#endif

	if (setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, (const void*)&on, 
				sizeof (int)) < 0)
	{
		
		close (sock);
		return (-1);
	}
	if (bind (sock, (struct sockaddr *)&saddr, sizeof (saddr)) < 0)
	{
		fprintf(stderr,"Unable to bind to Port :[%d] due to %s \n",
			serv_tcpport,strerror(errno));
		close (sock);
		exit(1);
	}
#ifdef TCP
	if (listen (sock, 1) < 0)
	{
		fprintf(stderr,"Unable to listen to Port :[%d] due to %s \n",
			serv_tcpport,strerror(errno));
		close (sock);
		return (-1);
	}
#endif
	return sock;
}


#ifdef	TCP
/****************************************************************************
**    FUNCTION :
**              make_connection 
**
****************************************************************************
**    DESCRIPTION:
**              This function is used to make connections  
****************************************************************************/
int
make_connection( int sock )
{
	int			newsock, fromlen;
	struct sockaddr_in	from;
	struct linger		lg;

	fromlen = sizeof (from);
	if ((newsock = accept (sock, (struct sockaddr *)&from, &fromlen)) < 0)
	{
		fprintf(stderr,"Unable to accept connection for socket :[%d] "
							"due to %s \n", sock,strerror(errno));
		return (-1);
	}

	lg.l_onoff = 1;
	/* l_linger is set to default */
	lg.l_linger = 5;
	if (setsockopt (newsock, SOL_SOCKET, SO_LINGER, 
			(char *)&lg, sizeof (struct linger)) < 0)
	{
		close (sock);
		return (-1);
	}

	return newsock;
}


/****************************************************************************
**    FUNCTION :
**              nw_read  
**
****************************************************************************
**    DESCRIPTION:
**              This function is used to read from the N/W   
****************************************************************************/
int
nw_read( int sockfd, char *ptr, int nbytes )
{
	int	nleft, nread;

	nleft = nbytes;
	while (nleft > 0)
	{
		nread = read (sockfd, ptr, nleft);
		if (nread < 0)
			return nread;	/* error return < 0 */
		else 
		if (nread == 0)
		{
			return -1;		/* EOF */
		}
		
		nleft -= nread;
		ptr += nread;
	}
	return (nbytes - nleft);	/* return >= 0 */
}


/****************************************************************************
**    FUNCTION :
**              nw_write   
**
****************************************************************************
**    DESCRIPTION:
**              This function is used to write onto N/W   
****************************************************************************/
int
nw_write( int sockfd, char *ptr, int nbytes )
{
	int	nleft, nwritten;

	nleft = nbytes;
	while (nleft > 0)
	{
		nwritten = write (sockfd, ptr, nleft);
		if (nwritten < 0)
			return nwritten;	/* error return < 0 */
		
		nleft -= nwritten;
		ptr += nwritten;
	}
	return (nbytes - nleft);
}

/****************************************************************************
**    FUNCTION :
**              bind_connect
**
****************************************************************************
**    DESCRIPTION:
**              This function is used to setup client which first 
**				binds to a port and then connects to a server 
****************************************************************************/
int
bind_and_connect
( char * serv_ipstr, 
  u_short serv_tcpport ,u_short self_bindport)
{
	int			sockfd;
	const int  on = 1;		/* for setsockopt() */
	long			serv_ipaddr;
	struct sockaddr_in	saddr,self_addr;
	
	if ((serv_ipaddr = inet_addr (serv_ipstr)) == -1)
	{
		fprintf (stderr, "Invalid IP address\n");
		exit (1);
	}

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons (serv_tcpport);
	memcpy ((char *)&saddr.sin_addr.s_addr, 
		(char *)&serv_ipaddr, sizeof (long));

	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return (-1);
	}

	memset(&self_addr,NULL,sizeof(struct sockaddr_in));
	self_addr.sin_family = AF_INET;
	self_addr.sin_port = htons (self_bindport);

	if (setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&on, 
				sizeof (int)) < 0)
	{
		
		close (sockfd);
		return (-1);
	}
	/** Bind the socket to a self port **/
	if (bind(sockfd,(struct sockaddr *)&self_addr,
								sizeof (struct sockaddr_in))< 0)
	{
				close(sockfd);
				perror("Unable to bind\n");
				return (-1);
    }


	/** After bind , send connect request to the specified 
		server ipaddress and port **/
	if (connect (sockfd, (struct sockaddr *)&saddr, 
		     sizeof (struct sockaddr_in)) < 0)
    {
		close (sockfd);
		return (-1);
	}

	return sockfd;
}
#endif

#ifdef UDP
/* Write datagram out on sock.  UDP only! */
int nw_write(int sock, char *buf, int length) 
{
#ifndef WIN32
  return write(sock, buf, length);
#else
  return send(sock, buf, length, 0);
#endif
}

/* Block until a datagram is sent to sock.  After the datagram is
 * received, connect sock to the sender so that an Ack may be sent.
 * This establishes (more or less) a 2-way communications channel.
 * UDP only!
 */
int nw_read(int sock, char *buf, int length) 
{
  struct sockaddr_in cli;
  int cli_size;
  int retval;

  cli_size = sizeof(cli);
  retval = recvfrom(sock, buf, length, 0,(struct sockaddr *) &cli, &cli_size);
  
  return retval;
}
int nw_peek(int sock, char *buf, int length) 
{
  struct sockaddr_in cli;
  int cli_size;
  int retval;

  cli_size = sizeof(cli);
  retval = recvfrom(sock, buf, length, MSG_PEEK,(struct sockaddr *) &cli, &cli_size);
  
  return retval;
}
#endif
