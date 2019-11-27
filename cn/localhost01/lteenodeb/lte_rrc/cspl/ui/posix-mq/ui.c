/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include	<ui.h>
#include	<mqueue.h>

int	ui_send( void *server, void *destination, unsigned char *message, unsigned int size, char *peername )
{
	int	n;
	mqd_t	mq;

	if( (mq = mq_open( (char *)(destination), O_WRONLY )) == (mqd_t)(-1) ) {
		perror( "mq_open" );
		abort();
	}
	
	if( (mq_send( mq, (char *)message, size, 1 )) < 0 ) {
		perror( "mq_send" );
		abort();
	}

	if( mq_close( mq ) < 0 ) {
		perror( "mq_close" );
		abort();
	}

	n = size;

	if( peername && (n >= 0) ) {
		strcpy( peername, "other end" );
	}
	return n;
}

int	ui_receive( void *server, unsigned char *buffer, unsigned int maxsize, char *peername )
{
	int	n; 
	mqd_t	mq = (mqd_t)server;

	if ( (n = mq_receive( mq, (char *)buffer, MAXMSGSIZE, 0 )) < 0 ) {
		perror( "mq_receive" );
		abort();
	}

	if( peername && (n >= 0) ) {
		strcpy( peername, "other end" );
	}
	return n;
}

void	*ui_makeserver( char *name )
{
	mqd_t	mq;

	if( (mq = mq_open( name, O_RDONLY|O_CREAT, 0666, 0 )) == (mqd_t)(-1) ) {
		perror( "mq_open" );
		abort();
	}
	return (void *)mq;
}

void	*ui_hash( char *name )
{
	return (void *)name;
}

void	ui_free( void *d )
{
}
