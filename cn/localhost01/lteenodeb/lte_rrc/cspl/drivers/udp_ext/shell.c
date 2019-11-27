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

static int	makeserver( const struct sockaddr_in *self )
{
	int	sd, send_buf_size = 64000, recv_buf_size = 64000;
	char	sockname[32];

	printaddress( sockname, self );
	qvLog( 0, "Binding to %s\n", sockname );

	if( (sd = socket( PF_INET, SOCK_DGRAM, 0 )) < 0 ) {
		errorabort("socket");
	}
/* Changes regarding VXWORKS Compilation */    
/* Start SPR 5921 Changes. */    
#ifdef VXWORKS
    if (setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char *)&send_buf_size,
														sizeof(int)) < 0)
#else  
	if (setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (const void *)&send_buf_size,
														sizeof(int)) < 0)
#endif 
/* End SPR 5921 Changes. */            
	{
        errorabort("setsockopt");
	}
/* Start SPR 5921 Changes. */
#ifdef VXWORKS
    if (setsockopt(sd, SOL_SOCKET, SO_RCVBUF, (char *)&recv_buf_size,
														sizeof(int)) < 0)
#else
    if (setsockopt(sd, SOL_SOCKET, SO_RCVBUF, (const void *)&recv_buf_size,
															sizeof(int)) < 0)
#endif  
/* End SPR 5921 Changes. */            
    {
        errorabort("setsockopt");
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

	unsigned char	*M, *payload, *p_M;
	unsigned int	mlen, no_bytes_to_read, no_of_bytes_read;

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

	mlen = SH_HDRSIZE + attrsize;
	if( payloadsize ) {
		mlen += payloadsize;
	} 

	M = (unsigned char *)malloc( mlen );
	no_bytes_to_read = 10000;
	no_of_bytes_read = 0;
	p_M = M;
	while (no_of_bytes_read < mlen)
	{
		if ((mlen - no_of_bytes_read) < no_bytes_to_read)
			no_bytes_to_read = mlen - no_of_bytes_read;
 
			if( (n = recvfrom(u->sd, (char *)p_M, no_bytes_to_read, 0, 0, 0)) <= 0 ) {
			errorabort("recvfrom");
		}
			no_of_bytes_read +=n;
			p_M += n; 
	}

	memcpy( buffer, M + SH_HDRSIZE, attrsize );
	if (payloadsize)
	{
		payload = qvMsgReserve( buffer, 0, payloadsize );
		memcpy( payload, M + SH_HDRSIZE + attrsize, payloadsize );
	}
	free( M );

	if( (unsigned int)no_of_bytes_read < (payloadsize + attrsize + SH_HDRSIZE) ) {
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
	unsigned char	*M, *p_M;
	unsigned int	mlen, no_of_bytes_sent, no_bytes_to_send = 10000;
    /* Start SPR 5921 Changes. */
    int             n;
    /* End SPR 5921 Changes. */

	printaddress( sockname, to );
	qvLog( 0, "Sending to %s [%08lX -> %08lX]\n", sockname, (unsigned long)sender, (unsigned long)receiver ); 

	memset( header, 0, sizeof(header) );
	SH_SETATTRSIZE(header, size); 
	SH_SETPAYLOADSIZE(header, qvMsgSize(message, 0)); 
	SH_SETSOURCE(header, qvGetServiceName(sender)); 
	SH_SETDESTINATION(header, qvGetServiceName(receiver)); 
	SH_SETPRIORITY(header, qvPriority(message)); 

	mlen = SH_HDRSIZE + size + payloadsize ;
	M = (unsigned char *)malloc(mlen) ;

	memcpy( M, header, SH_HDRSIZE );
	memcpy( M + SH_HDRSIZE, message, size );
	qvMsgExtract( message, 0, M + SH_HDRSIZE + size, payloadsize ); 

	no_of_bytes_sent = 0;
	p_M = M;
	while (no_of_bytes_sent < mlen)
	{
		if ((mlen - no_of_bytes_sent) < no_bytes_to_send)
			no_bytes_to_send = mlen - no_of_bytes_sent;
		if( (n=sendto( u->sd, (char *)p_M, no_bytes_to_send, 0, (struct sockaddr *)to, sizeof(*to) )) < 0 ) {
			errorabort("sendto");
		}
		no_of_bytes_sent +=n;
		p_M += n; 
	}
	free( M );
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
