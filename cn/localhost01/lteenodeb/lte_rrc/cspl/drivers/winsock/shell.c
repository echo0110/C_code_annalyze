/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Sample Encapsulating Shell -- Using TCP/Winsock                        **/
/**                                                                        **/
/****************************************************************************/

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	<errno.h>

#include	<shell.h>

#include	<sockutil.h>

#define	MAXSEG	32

static int	makeserver( const struct sockaddr_in *self )
{
	int	sd;
    /* Variable yes is undeclared and in the function it is used */
    /* Start SPR 5921 Changes. */
    char *yes;
    /* End SPR 5921 Changes. */
	char	sockname[32];
	unsigned long l=1;

	printaddress( sockname, self );
	qvLog( 0, "Binding to %s\n", sockname );

	if( (sd = socket( PF_INET, SOCK_STREAM, 0 )) < 0 ) {
		errorabort("socket");
	}

	if( ioctlsocket(sd, FIONBIO, &l) < 0 ) {
		errorabort("ioctlsocket");
	}

	if( setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes) ) < 0 ) {
		errorabort("setsockopt:REUSEADDR");
	}

	if( bind(sd, (struct sockaddr *)self, sizeof(*self) ) < 0 ) {
		errorabort("bind");
	}

	if( listen(sd, 5) < 0 ) {
		errorabort("listen");
	}

	return sd;
}

/** The `Listener' Structure ************************************************/
typedef	struct	{
	int	sd;
} LISTENER;

/** Listener Methods ********************************************************/
static	void	s_timedwait( void *S, const QTIME *T )
{
	LISTENER	*u = (LISTENER *)S;
	struct timeval
		timeout;
	int	n;
	fd_set	F;

	FD_ZERO( &F );
	FD_SET( u->sd, &F );
	if( T ) {
		timeout.tv_sec = T->s;
		timeout.tv_usec = T->us;
	}

	n = select( FD_SETSIZE - 1, &F, 0, 0, T?&timeout:0 );
	if( n < 0 ) {
		errorabort("select");
	}
}

static	void	*s_receive( void *S, QMODULE *sender, QMODULE *receiver, signed char *priority )
{
	LISTENER	*u = (LISTENER *)S;
	void	*buffer;
	struct	sockaddr_in fromaddress;
	int		n, nsd, fromlen;
	unsigned char	*payload;
	struct timeval
		zero;
	fd_set	F;
	unsigned char	
		header[SH_HDRSIZE];
	unsigned int
		attrsize, payloadsize;

	FD_ZERO( &F );
	FD_SET( u->sd, &F );
	zero.tv_sec = zero.tv_usec = 0;

	n = select( FD_SETSIZE - 1, &F, 0, 0, &zero );

	if( n < 0 ) {
		errorabort("select");
	}

	if( !FD_ISSET( u->sd, &F) ) {
		return 0;
	}

	fromlen = sizeof( fromaddress );
	if( (nsd = accept(u->sd, (struct sockaddr *)&fromaddress, &fromlen)) < 0 ) {
		errorabort("accept");
	}

	if( (n = recv(nsd, (char *)(header), SH_HDRSIZE, 0)) <= 0 ) {
		errorabort("recv:header");
	}

	*sender = qvGetService(SH_GETSOURCE(header)); 
	*receiver = qvGetService(SH_GETDESTINATION(header));
	attrsize = SH_GETATTRSIZE(header);
	payloadsize = SH_GETPAYLOADSIZE(header);
	*priority = SH_GETPRIORITY(header);

	buffer = qvMsgAllocFromModule(*receiver, attrsize);

	if( (n = recv(nsd, (char *)buffer, attrsize, 0 )) <= 0 ) {
		errorabort("recv:attr");
	}

	if( payloadsize ) {
		payload = qvMsgReserve( buffer, 0, payloadsize );
		if( (n = recv(nsd, (char *)payload, payloadsize, 0 )) <= 0 ) {
			errorabort("recv:payload");
		}
	}

	closesocket(nsd);

	return buffer;
}

static	void	s_send( void *S, const void *message, QMODULE sender, QMODULE receiver, void *data, unsigned int size )
{
	LISTENER	*u = (LISTENER *)S;
	int		sd;
	struct		sockaddr_in *to = (struct sockaddr_in *)data;
	char		sockname[32];
	unsigned char	*M;
	unsigned int	payloadsize, mlen;

	printaddress( sockname, to );
	qvLog( 0, "Sending to %s [%08lX -> %08lX]\n", sockname, (unsigned long)sender, (unsigned long)receiver );

	if( (sd = socket( PF_INET, SOCK_STREAM, 0 )) < 0 ) {
		errorabort("socket");
	}
	
	if( connect( sd, (struct sockaddr *)to, sizeof(*to) ) < 0 ) {
		errorabort("connect");
	}

	payloadsize = qvMsgSize(message, 0);

	mlen = SH_HDRSIZE + size + payloadsize;
	M = (unsigned char *)malloc( mlen );

	SH_SETATTRSIZE(M, size); 
	SH_SETPAYLOADSIZE(M, payloadsize); 
	SH_SETSOURCE(M, qvGetServiceName(sender)); 
	SH_SETDESTINATION(M, qvGetServiceName(receiver)); 
	SH_SETPRIORITY(M, qvPriority(message)); 
	memcpy( M + SH_HDRSIZE, message, size );
	qvMsgExtract( message, 0, M + SH_HDRSIZE + size, payloadsize ); 

	if( sendto( sd, (char *)M, mlen, 0, (struct sockaddr *)&to, sizeof(to) ) < 0 ) {
		errorabort("sendto");
	}

	free( M );
	closesocket( sd );
}

static void *s_open( void *data )
{
	LISTENER	*L = (LISTENER *)qvSysMalloc(sizeof(LISTENER));
	struct sockaddr_in
			address;

	makeaddress( (char *)data, &address, htonl(INADDR_ANY) );

	L->sd = makeserver( &address );
	return L;
}

static void s_close( void *data )
{
	LISTENER	*L = (LISTENER *)data;

	closesocket( L->sd );
	qvSysFree( data );
}

static void *s_hash( void *data, void *name )
{
	struct sockaddr_in	*address = (struct sockaddr_in *)qvSysMalloc( sizeof(struct sockaddr_in) );

	makeaddress( (char *)name, address, htonl(INADDR_LOOPBACK) );
	return address;
}

const QSHELL	__operations = {
	s_timedwait,
	s_send,
	s_receive,
	s_open,
	s_close,
	s_hash
};

const QSHELL	*operations = &__operations;

