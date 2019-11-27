/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Sample Encapsulating Shell -- Using System V IPC Message Queues        **/
/**                                                                        **/
/****************************************************************************/

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<strings.h>
#include	<time.h>
#include	<limits.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<signal.h>
#include	<sys/types.h>
#include	<sys/times.h>
#include	<sys/time.h>
#include	<sys/stat.h>
#include	<pthread.h>
#include	<mqueue.h>
#include	<errno.h>

#include	<shell.h>

#define	MAXSEG	32
#define	MAXMSGSIZE	1024

extern char *optarg;
extern int optind, opterr, optopt;

unsigned char	msgbuf[MAXMSGSIZE];

static	void	errorabort( const char *s )
{
	char	*e = strerror(errno);
	qvPanic("%s: %s", s, e );
}

static mqd_t	makeserver( char *name )
{
	struct mq_attr 
		attr;
	mqd_t	mq;

	attr.mq_flags = 0;
	attr.mq_maxmsg = 128;
	attr.mq_msgsize = MAXMSGSIZE;

	if( (mq = mq_open( name, O_RDONLY | O_CREAT, 0666, attr )) == (mqd_t)(-1) ) {
		errorabort( "mq_open" );
	}
	
	return mq;
}

/** The `Listener' Structure ************************************************/
typedef	struct	{
	mqd_t	mq;
} LISTENER;

/** Listener Methods ********************************************************/
static	void	*s_receive( void *S, QMODULE *sender, QMODULE *receiver, signed char *priority )
{
	LISTENER		*u = (LISTENER *)S;
	unsigned int	N;
	void		*buffer;
	int		n;
	unsigned char	*M, *payload;
	unsigned int	mlen;
	void		*r;
	unsigned int	attrsize, payloadsize;

	qvLog(0, "Receiver Ready\n");
	if( (n = mq_receive( u->mq, (char *)msgbuf, MAXMSGSIZE, 0 )) < 0 ) {
			errorabort( "mq_receive" );
	}

	M = msgbuf;
	qvLogDump( 0, M, n, 0 );

	*sender = qvGetService(SH_GETSOURCE(M));
	*receiver = qvGetService(SH_GETDESTINATION(M));
	attrsize = SH_GETATTRSIZE(M);
	payloadsize = SH_GETPAYLOADSIZE(M);
	*priority = SH_GETPRIORITY(M);

	buffer = qvMsgAllocFromModule(*receiver, SH_HDRSIZE + attrsize);

	memcpy( buffer, M + SH_HDRSIZE, attrsize );

	payload = qvMsgReserve( buffer, 0, payloadsize );
	memcpy( payload, M + SH_HDRSIZE + attrsize, payloadsize );

	return buffer;
}

static	void	s_send( void *S, const void *message, QMODULE sender, QMODULE receiver, void *data, unsigned int size )
{
	LISTENER	*u = (LISTENER *)S;
	unsigned int	i;
	void		*last;
	char		*sockname = (char *)data;
	unsigned int	nseg;
	unsigned char	*M;
	unsigned int	mlen;
	mqd_t		mq;
	long		*mtype;
	unsigned int	payloadsize = qvMsgSize( message, 0 );

	qvLog( 0, "Sending to %s [%08lX -> %08lX]\n", sockname, (unsigned long)sender, (unsigned long)receiver );

	mlen = SH_HDRSIZE + size + payloadsize;
	M = (unsigned char *)malloc(mlen);

	SH_SETATTRSIZE(M, size); 
	SH_SETPAYLOADSIZE(M, payloadsize); 
	SH_SETSOURCE(M, qvGetServiceName(sender)); 
	SH_SETDESTINATION(M, qvGetServiceName(receiver)); 
	SH_SETPRIORITY(M, qvPriority(message)); 

	memcpy( M + SH_HDRSIZE, message, size );
	qvMsgExtract( message, 0, M + SH_HDRSIZE + size, payloadsize ); 

	if( (mq = mq_open( sockname, O_WRONLY )) == (mqd_t)(-1)) {
		errorabort( "mq_open" );
	}
	
	if( mq_send( mq, (char *)M, mlen, 1 ) < 0 ) {
		errorabort( "mq_send" );
	}

	if( (mq_close( mq )) < 0 ) {
		errorabort( "mq_close" );
	}

	free(M);
	
}

static void *s_open( void *data )
{
	LISTENER	*L = (LISTENER *)qvSysMalloc(sizeof(LISTENER));

	L->mq = makeserver( (char *)data );
	return L;
}

static void s_close( void *data )
{
	LISTENER	*L = (LISTENER *)data;

	mq_close( L->mq );
	qvSysFree( data );
}

const QSHELL	__operations = {
	0,
	s_send,
	s_receive,
	s_open,
	s_close,
	0
};

const QSHELL	*operations = &__operations;
