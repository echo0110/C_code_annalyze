/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include	<ui.h>


#define	NAMEOFF	YOFFSETOF(struct sockaddr_un, sun_path)

int	ui_send( void *server, void *destination, unsigned char *message, unsigned int size, char *peername )
{
	struct sockaddr_un	
		*to = (struct sockaddr_un *)destination;
	int	n, sd = (int)server;

	n = sendto( sd, (char *)message, size, 0, (struct sockaddr *)(to), NAMEOFF + strlen(to->sun_path) + 1 );

	if( peername && (n >= 0) ) {
		strcpy( peername, to->sun_path );
	}
	return n;
}

int	ui_receive( void *server, unsigned char *buffer, unsigned int maxsize, char *peername )
{
	struct sockaddr_un	
		from;
	int	n,
		sd = (int)server,
		fromlen = sizeof(from);

	n = recvfrom( sd, (char *)buffer, maxsize, 0, (struct sockaddr *)&from, &fromlen );

	if( peername && (n >= 0) ) {
		strncpy( peername, from.sun_path, fromlen - NAMEOFF );
	}
	return n;
}

void	*ui_makeserver( char *name )
{
	int	sd;
	struct	sockaddr_un	self;

	memset( &self, 0, sizeof(self) );
	self.sun_family = AF_UNIX;
	strcpy( self.sun_path, name );

	if( (sd = socket( PF_UNIX, SOCK_DGRAM, 0 )) < 0 ) {
		perror("socket");
		abort();
	}

	unlink( name );
	if( bind(sd, (struct sockaddr *)&self, NAMEOFF + strlen(name) + 1 ) < 0 ) {
		perror("bind");
		abort();
	}

	return (void *)sd;
}

void	*ui_hash( char *name )
{
	struct	sockaddr_un	*d = (struct sockaddr_un *)malloc( sizeof(struct sockaddr_un) );

	memset( d, 0, sizeof(*d) );
	d->sun_family = AF_UNIX;
	strcpy( d->sun_path, name );

	return (void *)d;
}

void	ui_free( void *d )
{
	free( d );
}

