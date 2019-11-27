/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Sample Encapsulating Shell -- Using System V IPC Message Queues        **/
/**                                                                        **/
/****************************************************************************/

#include	<signal.h>

#include	<shell.h>

#define	MAXMSGSIZE	1024

extern char *optarg;
extern int optind, opterr, optopt;

unsigned char	msgbuf[MAXMSGSIZE];

#ifdef __cplusplus
extern "C" {
#endif
	void	handler(int n)
	{
	/** Nothing **/
	}
#ifdef __cplusplus
}
#endif

void	handlesignal(int signal)
{
	struct sigaction sig;

	memset( &sig, 0, sizeof(sig) );
	sig.sa_handler = handler;
	sigaction( signal, &sig, 0 );
}

void	masksignal(int n)
{
	sigset_t	S;

	sigemptyset( &S );
	sigaddset( &S, n );
	sigprocmask( SIG_BLOCK, &S, 0 );
}

void	unmasksignal(int n)
{
	sigset_t	S;

	sigemptyset( &S );
	sigaddset( &S, n );
	sigprocmask( SIG_UNBLOCK, &S, 0 );
}

static	void	errorabort( const char *s )
{
	char	*e = strerror(errno);
	qvPanic("%s: %s", s, e );
}

static int	makeserver( char *name )
{
	int	mq;

	if( (mq = msgget( (key_t)atoi(name), IPC_CREAT|0666 )) < 0 ) {
		errorabort( "msgget" );
	}
	
	return mq;
}

/** The `Listener' Structure ************************************************/
typedef	struct	{
	int	n;
	int	mq;
	unsigned char	*buffer;
} LISTENER;

/** Listener Methods ********************************************************/
static	void	s_timedwait( void *S, const QTIME *T )
{
	LISTENER	*u = (LISTENER *)S;
	struct itimerval
		timeout, zero;
	int	n;

	if( u->n ) {
		return;
	}

	zero.it_value.tv_sec = 0;
	zero.it_value.tv_usec = 0;
	zero.it_interval.tv_sec = 0;
	zero.it_interval.tv_usec = 0;

	if( T ) {
		timeout.it_value.tv_sec = T->s;
		timeout.it_value.tv_usec = T->us;
		timeout.it_interval.tv_sec = 0;
		timeout.it_interval.tv_usec = 0;

		if( (T->s | T->us) == 0 ) {
			timeout.it_value.tv_usec = 1;
		}

		setitimer( ITIMER_REAL, &timeout, 0 );
	}

	u->n = msgrcv( u->mq, u->buffer, MAXMSGSIZE, 0, 0 );

	if( u->n < 0 ) {
		if( errno == EINTR ) {
			u->n = 0;
		} else {
			errorabort( "msgrcv" );
		}
	}

	setitimer( ITIMER_REAL, &zero, 0 );
}

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

	if( !u->n ) {
		return 0;
	}

	M = u->buffer + sizeof(long);
	n = u->n;

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

	r = buffer;

	/** Preparing for the next call to s_receive here. **/
	u->n = msgrcv( u->mq, u->buffer, MAXMSGSIZE, 0, IPC_NOWAIT );

	if( u->n < 0 ) {
		if( errno == ENOMSG ) {
			u->n = 0;
		} else {
			errorabort("msgrcv:IPC_NOWAIT");
		}
	}

	return r;
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
	int		mq;
	long		*mtype;
	unsigned int	payloadsize = qvMsgSize( message, 0 );

	qvLog( 0, "Sending to %s [%08lX -> %08lX]\n", sockname, (unsigned long)sender, (unsigned long)receiver );

	mlen = SH_HDRSIZE + size + payloadsize;
	mtype = (long *)u->buffer;
	*mtype = 1;
	M = u->buffer + sizeof(long);

	SH_SETATTRSIZE(M, size); 
	SH_SETPAYLOADSIZE(M, payloadsize); 
	SH_SETSOURCE(M, qvGetServiceName(sender)); 
	SH_SETDESTINATION(M, qvGetServiceName(receiver)); 
	SH_SETPRIORITY(M, qvPriority(message)); 

	memcpy( M + SH_HDRSIZE, message, size );
	qvMsgExtract( message, 0, M + SH_HDRSIZE + size, payloadsize ); 

	if( (mq = msgget( (key_t)atoi(sockname), 0 )) < 0 ) {
		perror( "msgget" );
		abort();
	}
	
	if( msgsnd( mq, (char *)u->buffer, mlen, 0 ) < 0 ) {
		errorabort( "msgsnd" );
	}
}


static void *s_open( void *data )
{
	LISTENER	*L = (LISTENER *)qvSysMalloc(sizeof(LISTENER));

	L->mq = makeserver( (char *)data );
	L->n = 0;
	L->buffer = msgbuf;
	handlesignal( SIGALRM );

	return L;
}

static void s_close( void *data )
{
	LISTENER	*L = (LISTENER *)data;

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

