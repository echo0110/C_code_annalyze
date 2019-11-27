/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Sample Porting Code -- For Unix Systems                                **/
/**                                                                        **/
/****************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<string.h>
#include	<fcntl.h>

#include	<cspl.h>
#include	<port.h>

/** Utility Routines for Sockets/TCP ****************************************/
#if defined(__sun__) || defined(__linux) || defined(__CYGWIN__)
#define	NAMERESOLVE
#endif

static const int yes = 1, no = 0;

static	void	errorabort( const char *s )
{
	char	*e = strerror(errno);
	fprintf(stderr, "%s: %s\n", s, e );
	abort();
}

#ifdef NAMERESOLVE
static unsigned long	resolve( char *name )
{
	struct hostent	
		*H;
	char	**p;
	int	plen;
	unsigned long
		resolved;

	resolved = inet_addr(name);
	if( resolved == (unsigned long)(-1) ) {
		H = gethostbyname( name );

		if( H && (p = H->h_addr_list) ) {
			memcpy( &(resolved), *p, sizeof(resolved) );
		} else {
			errorabort("gethostbyname");
			return (unsigned long)(-1);
		}
	} 

	return resolved;
}
#else
#define	resolve	inet_addr
#endif

static void	makeaddress(const char *s, struct sockaddr_in *address, unsigned long defaultaddress )
{
	memset( address, 0, sizeof(*address) );
	address->sin_family = AF_INET;

	if( strchr(s, ':') ) {
		unsigned short	port;
		char ipa_s[64];

		sscanf( s, "%[^:]:%hu", ipa_s, &port );
		address->sin_port = htons( port );
		address->sin_addr.s_addr = resolve(ipa_s);
	} else {
		address->sin_port = htons( atoi(s) );
		address->sin_addr.s_addr = defaultaddress;
	}
}
 
static void	printaddress( char *s, const struct sockaddr_in *address )
{
	unsigned long	ipa = ntohl(address->sin_addr.s_addr);
	unsigned short	port = ntohs(address->sin_port);

	sprintf( s, "%u.%u.%u.%u:%hu",
			YGETBYTE(ipa,3), 
			YGETBYTE(ipa,2), 
			YGETBYTE(ipa,1), 
			YGETBYTE(ipa,0), 
			port );
}

static int	makeserver( const struct sockaddr_in *self )
{
	int	sd;

	if( (sd = socket( PF_INET, SOCK_STREAM, 0 )) < 0 ) {
		errorabort("socket");
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
	int	sd, nsd;
} LISTENER;

/** The Porting Functions ***************************************************/
static	void	*my_malloc( unsigned int size )
{
	return malloc( size );
}

static	void	my_free( void *m )
{
	free(m);
}

static	int	my_vprintf( const char *format, va_list A )
{
	return vfprintf( stderr, format, A );
}

static	void	my_abort(void)
{
	abort();
}

static	void	my_walltime( QTIME *now )
{
	struct timeval	T;

	gettimeofday( &T, 0 );
	now->s = T.tv_sec;
	now->us = T.tv_usec;
}

static	void	my_timedwait( void *S, const QTIME *T )
{
	LISTENER	*u = (LISTENER *)S;
	struct timeval
		timeout;
	int	n;
	fd_set	F;

	FD_ZERO( &F );
	FD_SET( u->nsd, &F );
	if( T ) {
		timeout.tv_sec = T->s;
		timeout.tv_usec = T->us;
	}

	n = select( FD_SETSIZE - 1, &F, 0, 0, T?&timeout:0 );
	if( n < 0 ) {
		errorabort("select");
	}
}

static	void	*my_receive( void *S, QMODULE *sender, QMODULE *receiver, signed char *priority )
{
	LISTENER	*u = (LISTENER *)S;
	struct sockaddr_in
			fromaddress;
	void	*buffer = 0;
	struct timeval
		zero;
	fd_set	F;
	unsigned char	
		header[SH_HDRSIZE];
	unsigned int
		attrsize, payloadsize;
	int	n, fromlen;

	FD_ZERO( &F );
	FD_SET( u->nsd, &F );
	zero.tv_sec = zero.tv_usec = 0;

	n = select( FD_SETSIZE - 1, &F, 0, 0, &zero );

	if( n < 0 ) {
		errorabort("select");
	}

	if( !FD_ISSET( u->nsd, &F) ) {
		return 0;
	}

	if( (n = read(u->nsd, (char *)(header), SH_HDRSIZE)) < 0 ) {
		errorabort("read:header");
	} else if ( !n ) {
		goto eof;
	}

	*sender = qvGetService(SH_GETSOURCE(header)); 
	*receiver = qvGetService(SH_GETDESTINATION(header));
	attrsize = SH_GETATTRSIZE(header);
	payloadsize = SH_GETPAYLOADSIZE(header);
	*priority = SH_GETPRIORITY(header);

	buffer = qvMsgAllocFromModule(*receiver, attrsize);

	if( (n = read(u->nsd, (char *)buffer, attrsize)) < 0 ) {
		errorabort("read:attr");
	} else if (!n) {
		goto eof;
	}

	if( payloadsize ) {
		unsigned char	*payload;

		payload = qvMsgReserve( buffer, 0, payloadsize );
		if( (n = read(u->nsd, (char *)payload, payloadsize)) < 0 ) {
			errorabort("read:payload");
		} else if (!n) {
			goto eof;
		}
	}

	return buffer;

eof:
	if( buffer ) {
		qvMsgFree( buffer );
	}
	close( u->nsd );
	qvLog( 0, "Connection Closed" );

	fromlen = sizeof( fromaddress );
	if( (u->nsd = accept(u->sd, (struct sockaddr *)&fromaddress, &fromlen)) < 0 ) {
		errorabort("accept");
	}
	qvLog( 0, "Connection Accepted" );
	return 0;
}

static	void	my_send( void *S, const void *message, QMODULE sender, QMODULE receiver, void *data, unsigned int size )
{
	LISTENER	*u = (LISTENER *)S;
	unsigned int	i;
	int		sd;
	void		*last;
	unsigned int	payloadsize, nseg;
	unsigned char	header[SH_HDRSIZE];

	qvLog( 0, "Sending [%08lX -> %08lX]\n", (unsigned long)sender, (unsigned long)receiver );

	payloadsize = qvMsgSize(message, 0);
	nseg = qvMsgSegCount(message);

	SH_SETATTRSIZE(header, size); 
	SH_SETPAYLOADSIZE(header, payloadsize); 
	SH_SETSOURCE(header, qvGetServiceName(sender)); 
	SH_SETDESTINATION(header, qvGetServiceName(receiver)); 
	SH_SETPRIORITY(header, qvPriority(message)); 

	if( write( u->nsd, (char *)header, SH_HDRSIZE) < 0 ) {
		errorabort("write");
	}

	if( write( u->nsd, (char *)message, size) < 0 ) {
		errorabort("write");
	}

	for( i=0, last = 0; i<nseg; i++ ) {
		QSEGMENT seg;

		last = qvMsgSegNext( message, last, &seg );
		if( write( u->nsd, (char *)(seg.base), seg.size) < 0 ) {
			errorabort("write");
		}
	}
}

static void *my_open( void *data )
{
	LISTENER	*L = (LISTENER *)qvSysMalloc(sizeof(LISTENER));
	struct sockaddr_in
			address,
			fromaddress;
	int		n, fromlen;

	makeaddress( (char *)data, &address, htonl(INADDR_ANY) );

	L->sd = makeserver( &address );

	fromlen = sizeof( fromaddress );
	if( (L->nsd = accept(L->sd, (struct sockaddr *)&fromaddress, &fromlen)) < 0 ) {
		errorabort("accept");
	}

	qvLog( 0, "Connection Accepted" );
	return L;
}

static void my_close( void *data )
{
	LISTENER	*L = (LISTENER *)data;

	close( L->sd );
	close( L->nsd );
	qvSysFree( data );
}

const QSYSOP my_os = {
	my_malloc,
	my_free,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	my_vprintf,
	my_vprintf,
	my_abort
};

const QWAIT my_waitstruct = {
	0,
	0,
	0,
	my_walltime
};


const QSHELL my_shell = {
	my_timedwait,
	my_send,
	my_receive,
	my_open,
	my_close,
	0
};

