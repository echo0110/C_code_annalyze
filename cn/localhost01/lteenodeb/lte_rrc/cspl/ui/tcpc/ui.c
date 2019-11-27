/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include	<ui.h>

int	csd;

int	ui_send( void *server, void *destination, unsigned char *message, unsigned int size, char *peername )
{
	int	n;

	if( (n = send( csd, (char *)message, size, 0 )) < 0 ) {
		perror("send");
		abort();
	}

	if( n == 0 ) {
		fprintf(stderr,"Connection Closed\n");
		exit(1);
	}

	if( peername && (n >= 0) ) {
		strcpy(peername, "peer");
	}
	return n;
}

int	ui_receive( void *server, unsigned char *buffer, unsigned int maxsize, char *peername )
{
	int	n;

	if( (n = recv( csd, (char *)buffer, MAXMSGSIZE, 0 )) < 0 ) {
		perror("recv");
		abort();
	}

	if( peername && (n >= 0) ) {
		strcpy(peername, "peer");
	}
	return n;
}

static const int yes = 1;

void	*ui_makeserver( char *name )
{
	struct	sockaddr_in	to;

	makeaddress( name, &to, htonl(INADDR_LOOPBACK) );

	if( (csd = socket( PF_INET, SOCK_STREAM, 0 )) < 0 ) {
		perror("socket");
		abort();
	}

	if( connect( csd, (struct sockaddr *)&to, sizeof(to) ) < 0 ) {
		perror("connect");
		abort();
	}

	return (void *)csd;
}

void	*ui_hash( char *name )
{
	return (void *)0;
}

void	ui_free( void *d )
{
}
