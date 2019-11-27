/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Sample Encapsulating Shell -- Using Unix Domain Sockets                **/
/**                                                                        **/
/****************************************************************************/

#include	<shell.h>

#define	MAXSEG	32

static	void	errorabort( const char *s )
{
	char	*e = strerror(errno);
	qvPanic("%s: %s", s, e );
}

static int	makeserver( char *s )
{
	int	sd;
    /* Start SPR 5903 Changes. */
    long unsigned int len;
    /* End SPR 5903 Changes. */
	struct	sockaddr_un	self;

	memset( &self, 0, sizeof(self) );
	self.sun_family = AF_UNIX;
   
    /* Klocwork fix */
    /* Start SPR 5903 Changes. */    
	len=strlen(s);
	if(len < sizeof(self.sun_path))
	{
		strncpy( self.sun_path, s, (sizeof(self.sun_path) - 1));
		self.sun_path[(sizeof(self.sun_path) - 1)]='\0';
	}
	else
	{
		qvPanic("%s: size is more than destination string", s );
	}
    /* End SPR 5903 Changes. */
    
	if( (sd = socket( PF_UNIX, SOCK_DGRAM, 0 )) < 0 ) {
		errorabort("socket");
	}

	unlink( s );
	if( bind(sd, (struct sockaddr *)&self, sizeof(self) ) < 0 ) {
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
	struct	msghdr	M;
	struct	iovec	E[16];
	void	*buffer;
	int	n;
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

	if( (unsigned int)n < (payloadsize + attrsize + SH_HDRSIZE) ) {
		return 0;
	} else {
		return buffer;
	}
}

static	void	s_send( void *S, const void *message, QMODULE sender, QMODULE receiver, void *data, unsigned int size )
{
	LISTENER	*u = (LISTENER *)S;
	struct iovec	E[1+MAXSEG];
	struct msghdr	M;
	unsigned int	i;
	void		*last;
	struct		sockaddr_un to;
	char		*sockname = (char *)data;
	unsigned int	nseg;
	unsigned char	
		header[SH_HDRSIZE];
    /* Start SPR 5903 Changes. */
    long unsigned int len;
    /* End SPR 5903 Changes. */
    
	qvLog( 0, "Sending to %s [%08lX -> %08lX]\n", sockname, (unsigned long)sender, (unsigned long)receiver );

	nseg = qvMsgSegCount(message);

	memset( &to, 0, sizeof(to) );
	to.sun_family = AF_UNIX;
   
    /* Klocwork fix */ 
    /* Start SPR 5903 Changes. */	
	len=strlen(sockname);
	if(len < sizeof(to.sun_path))
	{
		strncpy( to.sun_path, sockname, (sizeof(to.sun_path) - 1));
		to.sun_path[(sizeof(to.sun_path) - 1)]='\0';
	}
	else
	{
		qvPanic("%s: size is more than destination string", sockname );
	}
    /* End SPR 5903 Changes. */
	memset( header, 0, sizeof(header) );
	SH_SETATTRSIZE(header, size); 
	SH_SETPAYLOADSIZE(header, qvMsgSize(message, 0)); 
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
		QSEGMENT var_seg;

		last = qvMsgSegNext( message, last, &var_seg );
		E[M.msg_iovlen].iov_base = (char *)var_seg.base;
		E[M.msg_iovlen].iov_len = var_seg.size;
		M.msg_iovlen++;
	}

	if( sendmsg( u->sd, &M, 0 ) < 0 ) {
		errorabort("sendmsg");
	}
}

static void *s_open( void *data )
{
	LISTENER	*L = (LISTENER *)qvSysMalloc(sizeof(LISTENER));

	L->sd = makeserver( (char *)data );
	return L;
}

static void s_close( void *data )
{
	LISTENER	*L = (LISTENER *)data;

	close( L->sd );
	qvSysFree( data );
}

const QSHELL	__operations = {
	s_timedwait,
	s_send,
	s_receive,
	s_open,
	s_close,
	0
};

const QSHELL	*operations = &__operations;
