/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Sample Encapsulating Shell -- Using UDP                                **/
/**                                                                        **/
/****************************************************************************/

#include	<cspl.h>
#include	<shell.h>

#include	<sockutil.h>

#define	MAXSEG	32

static int	makeserver( const struct sockaddr_in *self )
{
	int	sd;
	char	sockname[32];

	printaddress( sockname, self );
	qvLog( 0, "Binding to %s\n", sockname );

	if( (sd = socket( PF_INET, SOCK_DGRAM, 0 )) < 0 ) {
		errorabort("socket");
	}

	if( bind(sd, (struct sockaddr *)self, sizeof(*self) ) < 0 ) {
		errorabort("bind");
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
	int	n;
	struct timeval
		zero;
	fd_set	F;
	unsigned char
		header[SH_HDRSIZE];
	unsigned int
		attrsize, payloadsize;

#ifndef SCATTER
	unsigned char	*M, *payload;
	unsigned int	mlen;
#else
	struct	msghdr	M;
	struct	iovec	E[16];
#endif

	FD_ZERO( &F );
	FD_SET( u->sd, &F );
	zero.tv_sec = zero.tv_usec = 0;

	if( select( FD_SETSIZE - 1, &F, 0, 0, &zero ) < 0 ) {
		errorabort("select");
	}

	if( !FD_ISSET( u->sd, &F ) ) {
		return 0;
	}

	if( (n = recvfrom(u->sd, (char *)(header), SH_HDRSIZE, MSG_PEEK, 0, 0)) <= 0 ) {
		errorabort("recvfrom:MSG_PEEK");
	}

	*sender = qvGetService(SH_GETSOURCE(header)); 
	*receiver = qvGetService(SH_GETDESTINATION(header));
	attrsize = SH_GETATTRSIZE(header);
	payloadsize = SH_GETPAYLOADSIZE(header);
	*priority = SH_GETPRIORITY(header);

	buffer = qvMsgAllocFromModule(*receiver, attrsize);
	
	/* Start SPR 4990 Changes. */ 
	if(buffer == NULL)
	{
		return 0;	
	}
	/* End SPR 4990 Changes. */ 
	
#ifndef SCATTER
	mlen = SH_HDRSIZE + attrsize;
	if( payloadsize ) {
		mlen += payloadsize;
	} 

	M = (unsigned char *)malloc( mlen );
	
	/* Start SPR 4990 Changes. */ 
	if(M == NULL)
	{
		const QMANIFEST	*Q = (const QMANIFEST *)qvGetMethods(*receiver);
	
		if( Q && !(Q->method.alloc )) 
		{
			qvMsgFree(buffer);
		}

		return 0;
	}
	/* End SPR 4990 Changes. */ 
	
	if( (n = recvfrom(u->sd, (char *)M, mlen, 0, 0, 0)) <= 0 ) {
		errorabort("recvfrom");
	}

	memcpy( buffer, M + SH_HDRSIZE, attrsize );
	payload = qvMsgReserve( buffer, 0, payloadsize );
	memcpy( payload, M + SH_HDRSIZE + attrsize, payloadsize );
	free( M );
#else
	memset( &M, 0, sizeof(M) );
	M.msg_name = 0;
	M.msg_namelen = 0;
	M.msg_iov = E;
	M.msg_iovlen = 0;

	E[M.msg_iovlen].iov_base = (char *)header;
	E[M.msg_iovlen].iov_len = SH_HDRSIZE;
	M.msg_iovlen++;

	E[M.msg_iovlen].iov_base = (char *)buffer;
	E[M.msg_iovlen].iov_len = attrsize;
	M.msg_iovlen++;

	if( payloadsize ) {
		E[M.msg_iovlen].iov_base = (char *)qvMsgReserve( buffer, 0, payloadsize );
		E[M.msg_iovlen].iov_len = payloadsize;
		M.msg_iovlen++;
	} 

	if( (n = recvmsg( u->sd, &M, 0 )) < 0 ) {
		errorabort("recvmsg");
	}
#endif

	if( (unsigned int)n < (payloadsize + attrsize + SH_HDRSIZE) ) {
		return 0;
	} else {
		return buffer;
	}
}

static	void	s_send( void *S, const void *message, QMODULE sender, QMODULE receiver, void *data, unsigned int size )
{
	LISTENER	*u = (LISTENER *)S;
	char		sockname[32];
	struct	sockaddr_in 
			*to = (struct sockaddr_in *)data;
	unsigned int	payloadsize = qvMsgSize( message, 0 );
	unsigned char	header[SH_HDRSIZE];
#ifndef SCATTER
	unsigned char	*M;
	unsigned int	mlen;
#else
	struct iovec	E[1+MAXSEG];
	struct msghdr	M;
	unsigned int	nseg;
#endif

	printaddress( sockname, to );
	qvLog( 0, "Sending to %s [%08lX -> %08lX]\n", sockname, (unsigned long)sender, (unsigned long)receiver ); 

	memset( header, 0, sizeof(header) );
	SH_SETATTRSIZE(header, size); 
	SH_SETPAYLOADSIZE(header, qvMsgSize(message, 0)); 
	SH_SETSOURCE(header, qvGetServiceName(sender)); 
	SH_SETDESTINATION(header, qvGetServiceName(receiver)); 
	SH_SETPRIORITY(header, qvPriority(message)); 

#ifndef SCATTER
	mlen = SH_HDRSIZE + size + payloadsize ;
	M = (unsigned char *)malloc(mlen) ;

	/* Start SPR 4990 Changes. */ 
	if(M == NULL)
	{
		return ;
	}
	/* End SPR 4990 Changes. */ 

	memcpy( M, header, SH_HDRSIZE );
	memcpy( M + SH_HDRSIZE, message, size );
	qvMsgExtract( message, 0, M + SH_HDRSIZE + size, payloadsize ); 

	if( sendto( u->sd, (char *)M, mlen, 0, (struct sockaddr *)to, sizeof(*to) ) < 0 ) {
		errorabort("sendto");
	}

	free( M );
#else
	nseg = qvMsgSegCount(message);

	memset( &M, 0, sizeof(M) );
	M.msg_name = (caddr_t)(to);
	M.msg_namelen = sizeof(*to);
	M.msg_iov = E;
	M.msg_iovlen = 0;

	E[M.msg_iovlen].iov_base = (char *)header;
	E[M.msg_iovlen].iov_len = SH_HDRSIZE;
	M.msg_iovlen++;

	E[M.msg_iovlen].iov_base = (char *)message;
	E[M.msg_iovlen].iov_len = size;
	M.msg_iovlen++;

	for( i=0, last = 0; i<nseg; i++ ) {
		QSEGMENT seg;

		last = qvMsgSegNext( message, last, &seg );
		E[M.msg_iovlen].iov_base = (char *)seg.base;
		E[M.msg_iovlen].iov_len = seg.size;
		M.msg_iovlen++;
	}

	if( sendmsg( u->sd, &M, 0 ) < 0 ) {
		errorabort("sendmsg");
	}
#endif
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

	close( L->sd );
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
