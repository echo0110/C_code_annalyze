/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include	<ui.h>

int	ui_send( void *server, void *destination, unsigned char *message, unsigned int size, char *peername )
{
	struct sockaddr_in	
		*to = (struct sockaddr_in *)destination;
	int	n, sd = (int)server;

	if( (n = sendto( sd, (char *)message, size, 0, (struct sockaddr *)(to), sizeof(*to) )) < 0 ) {
		perror("sendto");
		abort();
	}

	if( peername && (n >= 0) ) {
		printaddress( peername, to );
	}
	return n;
}

int	ui_receive( void *server, unsigned char *buffer, unsigned int maxsize, char *peername )
{
	struct sockaddr_in	
		from;
	int	n,
		sd = (int)server,
		fromlen = sizeof(from);

	if( (n = recvfrom( sd, (char *)buffer, maxsize, 0, (struct sockaddr *)&from, &fromlen )) < 0 ) {
		perror("recvfrom");
		abort();
	}

	if( peername && (n >= 0) ) {
		printaddress( peername, &from );
	}
	return n;
}

void	*ui_makeserver( char *name )
{
	int	sd;
	struct	sockaddr_in	self;

	makeaddress( name, &self, htonl(INADDR_ANY) );

	if( (sd = socket( PF_INET, SOCK_DGRAM, 0 )) < 0 ) {
		perror("socket");
		abort();
	}

	if( bind(sd, (struct sockaddr *)&self, sizeof(self) ) < 0 ) {
		perror("bind");
		abort();
	}

	return (void *)sd;
}

void	*ui_hash( char *name )
{
	struct	sockaddr_in	*d = (struct sockaddr_in *)malloc( sizeof(struct sockaddr_in) );

	makeaddress( name, d, htonl(INADDR_LOOPBACK) );

	return (void *)d;
}

void	ui_free( void *d )
{
	free( d );
}

