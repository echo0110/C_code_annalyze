/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Sample Encapsulating Shell -- for vxWorks                              **/
/**                                                                        **/
/****************************************************************************/

#include	<cspl.h>
#include	<stacklayer.h>
#include	<shell.h>

/* Start SPR 5921 Changes. */
void	uistart( void * );
void	__apisend( char *, unsigned int, unsigned int, unsigned long , unsigned long, signed char );
void	apisend( int from, int to, int api, char *param, char *payload);
/* End SPR 5921 Changes. */

typedef	struct {
	unsigned char	*message;
	unsigned char	header[SH_HDRSIZE];
	unsigned int	length;
} MSG_T;

extern	int		sysClkRateGet(void);
#define	SYS_CLKRATE	sysClkRateGet()

static	void	sendout( MSG_T *M )
{
	/** Here, we will simply print the hex dump of the message **/
	qvLog( 0, "Sending Message" );
	qvLogDump(0, M->message, M->length, 0);
	free( M->message );
}

/** Listener Structure ******************************************************/
typedef	struct	{
	MSG_Q_ID
		mq;
	MSG_T	anchor;
	int	result;
} LISTENER;

/** Listener Methods ********************************************************/
static	void	s_timedwait( void *S, const QTIME *T )
{
	LISTENER	*u = (LISTENER *)S;
	int	timeout;

	if( u->result != ERROR ) {
		return;
	}

	if( T ) {
		if( T->s | T->us ) {
			ULONG r = 0UL;

			r += (T->s)*SYS_CLKRATE;
			r += ((T->us)*SYS_CLKRATE)/1000000;
			timeout = (int)r;
		} else {
			timeout = NO_WAIT;
		}
	} else {
		timeout = WAIT_FOREVER;
	}

	u->result = msgQReceive( u->mq, (char *)(&(u->anchor)), sizeof(MSG_T), timeout );
}

static	void	s_send( void *S, const void *message, QMODULE sender, QMODULE receiver, void *data, unsigned int size )
{
	U8	*B;
	MSG_T	M;
	unsigned int payloadsize = qvMsgSize( message, 0 );

	memset( M.header, 0, sizeof(M.header) );
	SH_SETATTRSIZE(M.header, size); 
	SH_SETPAYLOADSIZE(M.header, qvMsgSize(message, 0)); 
	SH_SETSOURCE(M.header, qvGetServiceName(sender)); 
	SH_SETDESTINATION(M.header, qvGetServiceName(receiver)); 
	SH_SETPRIORITY(M.header, qvPriority(message)); 

	B = (U8 *)malloc( size + payloadsize );

	memcpy(B, message, size );
	if( payloadsize ) {
		qvMsgExtract(message, 0, B + size, payloadsize );
	}

	M.message = B;
	M.length = size + payloadsize;

	sendout( &M );
}

static	void	*s_receive( void *S, QMODULE *sender, QMODULE *receiver, signed char *priority )
{
	LISTENER	*u = (LISTENER *)S;
	U8	*buffer;
	void	*r;
	unsigned int attrsize, payloadsize;

	if( u->result == ERROR ) {
		return 0;
	}

	qvLog(0, "Message Received");
	qvLogDump(0, u->anchor.message, u->anchor.length, 0);

	*sender = qvGetService(SH_GETSOURCE(u->anchor.header)); 
	*receiver = qvGetService(SH_GETDESTINATION(u->anchor.header));
	attrsize = SH_GETATTRSIZE(u->anchor.header);
	payloadsize = SH_GETPAYLOADSIZE(u->anchor.header);
	*priority = SH_GETPRIORITY(u->anchor.header);

	buffer = (U8 *)qvMsgAllocFromModule(*receiver, attrsize);

	memcpy( buffer, u->anchor.message, attrsize );

	if( payloadsize ) {
		U8	*pbuf = (U8 *)qvMsgReserve( buffer, 0, payloadsize );
		memcpy( pbuf, u->anchor.message + attrsize, payloadsize );
	} 

	r = (void *)buffer;	

	free( u->anchor.message );

	u->result = msgQReceive( u->mq, (char *)(&(u->anchor)), sizeof(MSG_T), NO_WAIT );
	return r;
}

static void *s_open( void *data )
{
	LISTENER	*L = (LISTENER *)qvSysMalloc(sizeof(LISTENER));

	L->mq = msgQCreate( 1024, sizeof(MSG_T), MSG_Q_FIFO );
	L->result = ERROR;
	return L;
}

static void s_close( void *data )
{
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

/****************************************************************************/
/** Sample Command-line Client **********************************************/
/****************************************************************************/
static	unsigned int	xdigit( unsigned int d )
{
	/** We are doing this to avoid having to call sscanf, which is slightly heavy **/

	if( (d >= '0') && (d <= '9') ) {
		return d - '0';
	} else if( (d >= 'a') && (d <= 'f') ) {
		return d - 'a' + 10;
	} else if( (d >= 'A') && (d <= 'F') ) {
		return d - 'A' + 10;
	} else {
		return 0;
	}
}

static	int	checkstring( char *s )
{
	unsigned int	i, n; 

	if( !s ) {
		return 0;
	}

	n = strlen(s);

	if( n % 2 ) {
		return -1;
	}

	for( i=0; i<n; i++ ) {
		if( !isxdigit(s[i]) ) {
			return -2;
		}
	}

	return n/2;
}

LISTENER	*listener;

void	uistart( void *n )
{
	listener = (LISTENER *)n;
}

void	__apisend( char *s, unsigned int size, unsigned int payloadsize, unsigned long from, unsigned long to, signed char priority )
{
	unsigned char	*p;
	unsigned int	i, n = checkstring(s);
	MSG_T	M;

	if( n <= 0 ) {
		return;
	}

	p = (unsigned char *)malloc( n );

	for( i=0; i<n; i++ ) {
		p[i] = 16 * xdigit(s[2*i]) + xdigit(s[2*i+1]);
	}

	M.message = p;
	M.length = n;

	qvDump( M.message, M.length, 0 );

	memset( M.header, 0, sizeof(M.header) );
	SH_SETATTRSIZE(M.header, size); 
	SH_SETPAYLOADSIZE(M.header, payloadsize); 
	SH_SETSOURCE(M.header, from); 
	SH_SETDESTINATION(M.header, to); 
	SH_SETPRIORITY(M.header, priority); 

	msgQSend( listener->mq, (char *) &M, sizeof(MSG_T), WAIT_FOREVER, MSG_PRI_NORMAL );
}

void	apisend( int from, int to, int api, char *param, char *payload)
{
	int	paramlen, payloadlen;
	char	*s, *scan;

	if( !listener ) {
		qvPrintf("Please call <uistart> first\n");
		return;
	}

	paramlen = checkstring(param);
	payloadlen = checkstring(payload);

	if( (paramlen < 0) || (payloadlen < 0) ) {
		qvPrintf("Error in parameter or payload\n");
		return;
	}

	scan = s = (char *)malloc( 1 + 2*(CV_HDRSIZE + paramlen + payloadlen) );

	scan += sprintf( scan, "%02X", 0 );
	scan += sprintf( scan, "%04X", from );
	scan += sprintf( scan, "%04X", to );
	scan += sprintf( scan, "%04X", api );
	scan += sprintf( scan, "%04X", CV_HDRSIZE + paramlen + payloadlen );
	scan += sprintf( scan, "%04X", CV_HDRSIZE + paramlen );
	scan += sprintf( scan, "%04X", payloadlen );
	scan += sprintf( scan, "%02X", 0 );
	scan += sprintf( scan, "%04X", 0 );

	if( paramlen ) {
		scan += sprintf( scan, "%s", param );
	}

	if( payloadlen ) {
		scan += sprintf( scan, "%s", payload );
	}

	__apisend( s, CV_HDRSIZE + paramlen, payloadlen, from, to, 0 );
	free( s );
}

