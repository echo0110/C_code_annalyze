/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Sample Encapsulating Shell -- Using TCP                                **/
/**                                                                        **/
/****************************************************************************/

#include	<netdb.h>

#include	<cspl.h>
#include	<shell.h>

#include	<sockutil.h>

#define	MAXSEG	32

static int	makeserver( const struct sockaddr_in *self )
{
	int	sd;
	char	sockname[32];
    #ifdef VXWORKS
    int flag =1;
#endif

	printaddress( sockname, self );
	qvLog( 0, "Binding to %s\n", sockname );

	if( (sd = socket( PF_INET, SOCK_STREAM, 0 )) < 0 ) {
		errorabort("socket");
	}
    
#ifdef VXWORKS  
/* For vxworks 3rd argument should be passed as integer in ioctl */    
/* Start SPR 5921 Changes. */    
    if(ioctl(sd,FIONBIO,flag) < 0) {
/* End SPR 5921 Changes. */            
		errorabort("fcntl");
	}

#else
    if( fcntl(sd, F_SETFL, O_NONBLOCK) < 0 ) {
		errorabort("fcntl");
	}
#endif
    

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
	int		n, nsd;
	unsigned char	*payload;
	unsigned int	attrsize, payloadsize;
	unsigned char	header[SH_HDRSIZE];
/* For vxworks 4rd argument in accept call should be an integer */
/* Start SPR 5921 Changes. */    
#ifdef VXWORKS
    int fromlen;
#else
    unsigned int fromlen;     
#endif
/* End SPR 5921 Changes. */
	fromlen = sizeof( fromaddress );
	if( (nsd = accept(u->sd, (struct sockaddr *)&fromaddress, &fromlen)) < 0 ) {
		if( errno == EWOULDBLOCK ) {
			return 0;
		} else {
			errorabort("accept");
		}
	}

	if( (n = recv(nsd, (char *)(header), SH_HDRSIZE, MSG_WAITALL)) <= 0 ) {
		errorabort("recvfrom:MSG_PEEK");
	}

	*sender = qvGetService(SH_GETSOURCE(header)); 
	*receiver = qvGetService(SH_GETDESTINATION(header));
	attrsize = SH_GETATTRSIZE(header);
	payloadsize = SH_GETPAYLOADSIZE(header);
	*priority = SH_GETPRIORITY(header);

	buffer = qvMsgAllocFromModule(*receiver, attrsize);
	if( (n = recv(nsd, (char *)buffer, attrsize, MSG_WAITALL )) <= 0 ) {
		errorabort("recv:attr");
	}

	if( payloadsize ) {
		payload = qvMsgReserve( buffer, 0, payloadsize );
		if( (n = recv(nsd, (char *)payload, payloadsize, MSG_WAITALL )) <= 0 ) {
			errorabort("recv:payload");
		}
	}

	close(nsd);

	return buffer;
}

static	void	s_send( void *S, const void *message, QMODULE sender, QMODULE receiver, void *data, unsigned int size )
{
	unsigned int	i;
	int		sd;
	void		*last;
	struct		sockaddr_in *to = (struct sockaddr_in *)data;
	char		sockname[32];
	unsigned int	nseg;
	struct iovec	E[1+MAXSEG];
	struct msghdr	M;
	unsigned int	payloadsize = qvMsgSize(message, 0);
	unsigned char	header[SH_HDRSIZE];

	printaddress( sockname, to );
	qvLog( 0, "Sending to %s [%08lX -> %08lX]\n", sockname, (unsigned long)sender, (unsigned long)receiver );

	if( (sd = socket( PF_INET, SOCK_STREAM, 0 )) < 0 ) {
		errorabort("socket");
	}
	
	nseg = qvMsgSegCount(message);

	if( connect( sd, (struct sockaddr *)to, sizeof(*to) ) < 0 ) {
		errorabort("connect");
	}

	memset( header, 0, sizeof(header) );
	SH_SETATTRSIZE(header, size); 
	SH_SETPAYLOADSIZE(header, payloadsize); 
	SH_SETSOURCE(header, qvGetServiceName(sender)); 
	SH_SETDESTINATION(header, qvGetServiceName(receiver)); 
	SH_SETPRIORITY(header, qvPriority(message)); 

	memset( &M, 0, sizeof(M) );
	M.msg_name = (caddr_t)(&to);
	M.msg_namelen = sizeof(to);
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

	if( sendmsg( sd, &M, 0 ) < 0 ) {
		errorabort("sendmsg");
	}

	close( sd );
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
