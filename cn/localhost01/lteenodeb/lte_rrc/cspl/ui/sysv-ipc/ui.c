/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include	<ui.h>

static unsigned char
	__rbuf[2048],
	__wbuf[2048];

int	ui_send( void *server, void *destination, unsigned char *message, unsigned int size, char *peername )
{
	int	n;
	int	mq;
	long	*mtype = (long *)__wbuf;

	*mtype = 1;
	memcpy( __wbuf + sizeof(long), message, size );
	if( (mq = msgget( (key_t)destination, 0 )) < 0 ) {
		perror( "msgget" );
		abort();
	}
	
	if( (n = msgsnd( mq, (char *)__wbuf, size, 0 )) < 0 ) {
		perror( "msgsnd" );
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
	int	n, mq = (int)server;

	if( (n = msgrcv( mq, __rbuf, MAXMSGSIZE, 0, 0 )) < 0 ) {
		perror( "msgrcv" );
		abort();
	}

	memcpy( buffer, __rbuf + sizeof(long), n );

	if( peername && (n >= 0) ) {
		strcpy( peername, "other end" );
	}
	return n;
}

void	*ui_makeserver( char *name )
{
	int	mq;

	if( (mq = msgget( (key_t)atoi(name), IPC_CREAT|0666 )) < 0 ) {
		perror( "msgget" );
		abort();
	}

	return (void *)mq;
}

void	*ui_hash( char *name )
{
	return (void *)atoi(name);
}

void	ui_free( void *d )
{
}
